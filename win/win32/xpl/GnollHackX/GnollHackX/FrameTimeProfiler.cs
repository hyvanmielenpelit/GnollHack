using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Threading;

namespace GnollHackX
{
    public struct FrameTimeSample
    {
        public long FrameNumber;
        public long TicksFrameStart;
        public long TicksUpdateStart;
        public long TicksLockAttempt;
        public long TicksLockResult;
        public bool LockAcquired;
        public long TicksPaintStart;
        public long TicksPaintEnd;
        public long TicksFrameEnd;

        /* GC collection counts at frame start (gen 0, 1, 2) */
        public int GcCount0;
        public int GcCount1;
        public int GcCount2;

        /* Cumulative bytes allocated at frame start */
        public long AllocatedBytes;

        /* Managed heap size at frame start */
        public long HeapSizeBytes;
    }

    /// <summary>
    /// Records the GC collection-count deltas caused by a single
    /// forced GC call, along with a timestamp so the profiler can
    /// match it to the correct inter-frame gap.
    /// </summary>
    public struct ForcedGcDelta
    {
        public long Timestamp;
        public int DeltaGen0;
        public int DeltaGen1;
        public int DeltaGen2;
    }

    public struct FrameTimeStatistics
    {
        public float InterFrameAvgMs;
        public float InterFrameStdDevMs;
        public float InterFrameP95Ms;
        public float InterFrameP99Ms;
        public float InterFrameMaxMs;
        public float AvgUpdateMs;
        public float AvgLockWaitMs;
        public float AvgPaintMs;
        public int DroppedFrameCount;
        public float DroppedFramePct;
        public int LockFailCount;
        public float LockFailPct;
        public float FPS;
        public int SampleCount;

        /* Forced GC frame statistics (excluded from base stats) */
        public int GcFrameCount;
        public float GcAvgMs;
        public float GcStdDevMs;
        public float GcP95Ms;
        public float GcP99Ms;
        public float GcWorstMs;

        /* Runtime (non-forced) GC frame statistics (excluded from base stats) */
        public int RuntimeGcFrameCount;
        public float RuntimeGcAvgMs;
        public float RuntimeGcStdDevMs;
        public float RuntimeGcP95Ms;
        public float RuntimeGcP99Ms;
        public float RuntimeGcWorstMs;

        /* Pause-affected frame statistics (menu/text/command canvas transitions) */
        public int PauseFrameCount;

        /* GC generation breakdown: how many inter-frame gaps triggered each gen */
        public int GcGen0Count;
        public int GcGen1Count;
        public int GcGen2Count;

        /* Memory statistics */
        public float AllocationRateMBPerSec;
        public long HeapSizeBytes;
        public long Gen0SizeBytes;
        public long Gen1SizeBytes;
        public long Gen2SizeBytes;
        public long LohSizeBytes;
    }

    public static class FrameTimeProfiler
    {
        private static int _isEnabled = 0;
        public static bool IsEnabled
        {
            get { return Interlocked.CompareExchange(ref _isEnabled, 0, 0) != 0; }
            set
            {
                Interlocked.Exchange(ref _isEnabled, value ? 1 : 0);
                if (!value)
                {
                    /* Reset buffer so stale data is not reported when re-enabled */
                    Interlocked.Exchange(ref _writeIndex, -1);
                    Interlocked.Exchange(ref _lastRenderIndex, -1);
                }
            }
        }

        private const int BufferSize = 1800;
        private const int MaxExclusionEvents = 64;
        private const int MaxForcedGcEvents = 64;
        private static readonly FrameTimeSample[] _buffer = new FrameTimeSample[BufferSize];
        private static long _writeIndex = -1;
        private static readonly double _msPerTick = 1000.0 / Stopwatch.Frequency;

        /* Pre-allocated sort buffers to avoid GC pressure on mobile */
        private static readonly float[] _sortBuffer = new float[BufferSize];
        private static readonly float[] _gcSortBuffer = new float[BufferSize];
        private static readonly float[] _runtimeGcSortBuffer = new float[BufferSize];

        /*
         * Forced-GC delta ring buffer. Each entry records the actual
         * collection-count deltas (per generation) caused by a single
         * forced GC call, with a timestamp taken after the GC completes.
         * This allows GetStatistics to match forced-GC effects to the
         * correct inter-frame gap by timestamp, regardless of frame
         * boundaries — fixing the misattribution bug where forced GC
         * collection-count bumps could land in a different frame pair
         * than the MarkGcEvent counter increment.
         */
        private static readonly ForcedGcDelta[] _forcedGcDeltas = new ForcedGcDelta[MaxForcedGcEvents];
        private static long _forcedGcDeltaWriteIndex = -1;

        /*
         * Thread-local storage for MarkGcBefore/MarkGcAfter pair.
         * Each thread can have one outstanding MarkGcBefore call.
         * Using ThreadLocal to handle the case where forced GC is
         * called from different threads (main thread, background).
         */
        [ThreadStatic]
        private static int _beforeGen0;
        [ThreadStatic]
        private static int _beforeGen1;
        [ThreadStatic]
        private static int _beforeGen2;

        /*
         * Pause event ring buffer. A pause event is recorded when the active
         * canvas switches away from MainCanvas (e.g. menu, text, or command
         * grid becomes visible). Any inter-frame gap containing a pause
         * timestamp is excluded from base statistics.
         */
        private static readonly long[] _pauseTimestamps = new long[MaxExclusionEvents];
        private static long _pauseWriteIndex = -1;

        /*
         * Tracks whether the main canvas was active on the previous
         * CompositionTarget_Rendering call, so we can detect transitions.
         * 0 = main canvas active, 1 = non-main canvas active, -1 = unknown
         */
        private static int _wasNonMainCanvas = -1;

        /* Wrap-safe modulo that always returns a non-negative index */
        private static int SafeIndex(long value, int size)
        {
            return (int)((value % size + size) % size);
        }

        /// <summary>
        /// Call immediately before a forced GC.Collect to snapshot
        /// the current collection counts. Must be paired with a
        /// subsequent MarkGcAfter() call after the GC completes.
        /// </summary>
        public static void MarkGcBefore()
        {
            _beforeGen0 = GC.CollectionCount(0);
            _beforeGen1 = GC.CollectionCount(1);
            _beforeGen2 = GC.CollectionCount(2);
        }

        /// <summary>
        /// Call immediately after a forced GC.Collect completes.
        /// Records the collection-count deltas and a timestamp into
        /// the forced-GC delta ring buffer.
        /// </summary>
        public static void MarkGcAfter()
        {
            int deltaGen0 = GC.CollectionCount(0) - _beforeGen0;
            int deltaGen1 = GC.CollectionCount(1) - _beforeGen1;
            int deltaGen2 = GC.CollectionCount(2) - _beforeGen2;

            long idx = Interlocked.Increment(ref _forcedGcDeltaWriteIndex);
            _forcedGcDeltas[SafeIndex(idx, MaxForcedGcEvents)] = new ForcedGcDelta
            {
                Timestamp = Stopwatch.GetTimestamp(),
                DeltaGen0 = deltaGen0,
                DeltaGen1 = deltaGen1,
                DeltaGen2 = deltaGen2
            };
        }

        /// <summary>
        /// Call when the active canvas transitions away from MainCanvas
        /// (menu, text grid, or command grid becomes visible) to mark
        /// a pause event. Also call when returning to MainCanvas so
        /// the first post-pause inter-frame gap is excluded.
        /// </summary>
        public static void MarkPauseEvent()
        {
            long idx = Interlocked.Increment(ref _pauseWriteIndex);
            _pauseTimestamps[SafeIndex(idx, MaxExclusionEvents)] = Stopwatch.GetTimestamp();
        }

        /// <summary>
        /// Call from CompositionTarget_Rendering after determining the
        /// canvas type. Automatically detects transitions to/from
        /// MainCanvas and marks pause events.
        /// </summary>
        public static void TrackCanvasType(bool isMainCanvas)
        {
            int wasNonMain = _wasNonMainCanvas;
            if (isMainCanvas)
            {
                if (wasNonMain == 1)
                {
                    /* Returning to main canvas — mark so the first
                     * post-pause inter-frame gap is excluded */
                    MarkPauseEvent();
                }
                _wasNonMainCanvas = 0;
            }
            else
            {
                if (wasNonMain == 0 || wasNonMain == -1)
                {
                    /* Leaving main canvas — mark the transition */
                    MarkPauseEvent();
                }
                _wasNonMainCanvas = 1;
            }
        }

        public static void BeginFrame(long frameNumber)
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Increment(ref _writeIndex);
            int index = SafeIndex(idx, BufferSize);
            _buffer[index] = new FrameTimeSample
            {
                FrameNumber = frameNumber,
                TicksFrameStart = Stopwatch.GetTimestamp(),
                GcCount0 = GC.CollectionCount(0),
                GcCount1 = GC.CollectionCount(1),
                GcCount2 = GC.CollectionCount(2),
                AllocatedBytes =
#if GNH_MAUI
                    GC.GetTotalAllocatedBytes(false),
#else
                    0,
#endif
                HeapSizeBytes = GC.GetTotalMemory(false)
            };
        }

        /*
         * Index of the last frame that entered the render path (set by
         * StampUpdate, which runs synchronously during UpdateMainCanvas).
         * Paint-path stamps (lock, paint) use this instead of _writeIndex
         * because PaintSurface fires asynchronously after
         * CompositionTarget_Rendering returns and _writeIndex may have
         * already advanced to the next frame.
         */
        private static long _lastRenderIndex = -1;

        public static void StampUpdate()
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            Interlocked.Exchange(ref _lastRenderIndex, idx);
            _buffer[SafeIndex(idx, BufferSize)].TicksUpdateStart = Stopwatch.GetTimestamp();
        }

        public static void StampLockAttempt()
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksLockAttempt = Stopwatch.GetTimestamp();
        }

        public static void StampLockResult(bool acquired)
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            int index = SafeIndex(idx, BufferSize);
            _buffer[index].TicksLockResult = Stopwatch.GetTimestamp();
            _buffer[index].LockAcquired = acquired;
        }

        public static void StampPaintStart()
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksPaintStart = Stopwatch.GetTimestamp();
        }

        public static void StampPaintEnd()
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksPaintEnd = Stopwatch.GetTimestamp();
        }

        public static void EndFrame()
        {
            if (!IsEnabled) return;
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksFrameEnd = Stopwatch.GetTimestamp();
        }

        /// <summary>
        /// Returns true if any timestamp in the given ring buffer falls
        /// within the half-open interval [ticksStart, ticksEnd).
        /// </summary>
        private static bool HasEventInInterval(long[] timestamps, long writeIndex, long ticksStart, long ticksEnd)
        {
            if (writeIndex < 0) return false;

            int count = (int)Math.Min(writeIndex + 1, MaxExclusionEvents);
            long firstIdx = writeIndex >= MaxExclusionEvents ? writeIndex - MaxExclusionEvents + 1 : 0;

            for (int i = 0; i < count; i++)
            {
                long ts = timestamps[SafeIndex(firstIdx + i, MaxExclusionEvents)];
                if (ts >= ticksStart && ts < ticksEnd)
                    return true;
            }
            return false;
        }

        /// <summary>
        /// Returns true if any forced-GC delta entry has a timestamp
        /// within the half-open interval [ticksStart, ticksEnd),
        /// indicating a forced GC was performed during that gap.
        /// </summary>
        private static bool HasForcedGcInInterval(long ticksStart, long ticksEnd)
        {
            long writeIndex = Interlocked.Read(ref _forcedGcDeltaWriteIndex);
            if (writeIndex < 0) return false;

            int count = (int)Math.Min(writeIndex + 1, MaxForcedGcEvents);
            long firstIdx = writeIndex >= MaxForcedGcEvents ? writeIndex - MaxForcedGcEvents + 1 : 0;

            for (int i = 0; i < count; i++)
            {
                long ts = _forcedGcDeltas[SafeIndex(firstIdx + i, MaxForcedGcEvents)].Timestamp;
                if (ts >= ticksStart && ts < ticksEnd)
                    return true;
            }
            return false;
        }

        private static bool IsPauseAffected(long ticksStart, long ticksEnd)
        {
            return HasEventInInterval(_pauseTimestamps, Interlocked.Read(ref _pauseWriteIndex), ticksStart, ticksEnd);
        }

        /// <summary>
        /// Returns true if any GC generation's collection count increased
        /// between the previous frame and the current frame, indicating
        /// that a GC (forced or runtime) ran during the inter-frame gap.
        /// </summary>
        private static bool DidGcOccur(FrameTimeSample prev, FrameTimeSample curr)
        {
            return curr.GcCount0 != prev.GcCount0
                || curr.GcCount1 != prev.GcCount1
                || curr.GcCount2 != prev.GcCount2;
        }

        /// <summary>
        /// Returns the highest generation that was collected between
        /// two frames, or -1 if no GC occurred.
        /// </summary>
        private static int MaxGcGen(FrameTimeSample prev, FrameTimeSample curr)
        {
            if (curr.GcCount2 != prev.GcCount2) return 2;
            if (curr.GcCount1 != prev.GcCount1) return 1;
            if (curr.GcCount0 != prev.GcCount0) return 0;
            return -1;
        }

        public static FrameTimeStatistics GetStatistics()
        {
            long currentWriteIndex = Interlocked.Read(ref _writeIndex);
            if (currentWriteIndex < 0)
                return new FrameTimeStatistics();

            int sampleCount = (int)Math.Min(currentWriteIndex + 1, BufferSize);
            long startIndex = currentWriteIndex >= BufferSize ? currentWriteIndex - BufferSize + 1 : 0;

            int interFrameCount = 0;
            double totalInterFrameTime = 0;
            double totalUpdateMs = 0;
            double totalLockWaitMs = 0;
            double totalPaintMs = 0;
            int droppedCount = 0;
            int lockAttemptCount = 0;
            int lockFailCount = 0;
            int renderedCount = 0;

            /* Forced GC frame tracking */
            int gcFrameCount = 0;

            /* Runtime (non-forced) GC frame tracking */
            int runtimeGcFrameCount = 0;

            /* Pause-affected frame tracking */
            int pauseFrameCount = 0;

            /* GC generation breakdown */
            int gcGen0Count = 0, gcGen1Count = 0, gcGen2Count = 0;

            /* Allocation rate tracking */
            long firstAllocBytes = 0;
            long lastAllocBytes = 0;
            long firstTicksStart = 0;
            long lastTicksStart = 0;
            bool hasFirstRendered = false;

            /* Latest heap size */
            long latestHeapSize = 0;

            float targetFrameTimeMs = 1000f / 60f; /* Approx 16.67ms */
            float droppedThresholdMs = targetFrameTimeMs * 1.5f;

            long prevRenderedFrameStart = 0;
            FrameTimeSample prevRenderedSample = default;
            bool hasPrevRendered = false;

            for (int i = 0; i < sampleCount; i++)
            {
                int currIdx = SafeIndex(startIndex + i, BufferSize);
                FrameTimeSample curr = _buffer[currIdx];

                /* Skip frames that did not enter the render path */
                if (curr.TicksUpdateStart == 0)
                    continue;

                renderedCount++;

                /* Track allocation window for rate computation */
                if (!hasFirstRendered)
                {
                    firstAllocBytes = curr.AllocatedBytes;
                    firstTicksStart = curr.TicksFrameStart;
                    hasFirstRendered = true;
                }
                lastAllocBytes = curr.AllocatedBytes;
                lastTicksStart = curr.TicksFrameStart;
                latestHeapSize = curr.HeapSizeBytes;

                if (hasPrevRendered)
                {
                    float interFrameMs = (float)((curr.TicksFrameStart - prevRenderedFrameStart) * _msPerTick);
                    bool forcedGcHit = HasForcedGcInInterval(prevRenderedFrameStart, curr.TicksFrameStart);
                    bool pauseHit = IsPauseAffected(prevRenderedFrameStart, curr.TicksFrameStart);
                    bool gcOccurred = DidGcOccur(prevRenderedSample, curr);
                    bool runtimeGcHit = !forcedGcHit && gcOccurred;

                    if (forcedGcHit)
                    {
                        /* Forced GC happened during this gap — track separately */
                        _gcSortBuffer[gcFrameCount] = interFrameMs;
                        gcFrameCount++;
                        int gen = MaxGcGen(prevRenderedSample, curr);
                        if (gen == 0) gcGen0Count++;
                        else if (gen == 1) gcGen1Count++;
                        else if (gen == 2) gcGen2Count++;
                    }
                    else if (runtimeGcHit)
                    {
                        /* Runtime GC happened during this gap — track separately */
                        _runtimeGcSortBuffer[runtimeGcFrameCount] = interFrameMs;
                        runtimeGcFrameCount++;
                        int gen = MaxGcGen(prevRenderedSample, curr);
                        if (gen == 0) gcGen0Count++;
                        else if (gen == 1) gcGen1Count++;
                        else if (gen == 2) gcGen2Count++;
                    }
                    else if (pauseHit)
                    {
                        /* Canvas was paused during this gap — exclude */
                        pauseFrameCount++;
                    }
                    else
                    {
                        _sortBuffer[interFrameCount] = interFrameMs;
                        interFrameCount++;
                        totalInterFrameTime += interFrameMs;

                        if (interFrameMs > droppedThresholdMs)
                            droppedCount++;
                    }
                }

                if (curr.TicksUpdateStart > 0 && curr.TicksLockResult > 0)
                    totalUpdateMs += (curr.TicksLockResult - curr.TicksUpdateStart) * _msPerTick;

                if (curr.TicksLockAttempt > 0)
                {
                    lockAttemptCount++;
                    if (curr.TicksLockResult > 0)
                        totalLockWaitMs += (curr.TicksLockResult - curr.TicksLockAttempt) * _msPerTick;
                    if (!curr.LockAcquired)
                        lockFailCount++;
                }

                if (curr.TicksPaintStart > 0 && curr.TicksPaintEnd > 0)
                    totalPaintMs += (curr.TicksPaintEnd - curr.TicksPaintStart) * _msPerTick;

                prevRenderedFrameStart = curr.TicksFrameStart;
                prevRenderedSample = curr;
                hasPrevRendered = true;
            }

            if (interFrameCount == 0) return new FrameTimeStatistics();

            float avgInterFrame = (float)(totalInterFrameTime / interFrameCount);

            /* Variance */
            double sumSqDiff = 0;
            for (int i = 0; i < interFrameCount; i++)
            {
                double diff = _sortBuffer[i] - avgInterFrame;
                sumSqDiff += diff * diff;
            }
            float stdDev = (float)Math.Sqrt(sumSqDiff / interFrameCount);

            Array.Sort(_sortBuffer, 0, interFrameCount);

            int p95Idx = (int)(interFrameCount * 0.95);
            int p99Idx = (int)(interFrameCount * 0.99);

            /* Compute forced GC percentiles */
            float gcAvg = 0, gcSd = 0, gcP95 = 0, gcP99 = 0, gcMax = 0;
            if (gcFrameCount > 0)
            {
                Array.Sort(_gcSortBuffer, 0, gcFrameCount);
                double gcTotal = 0;
                for (int i = 0; i < gcFrameCount; i++) gcTotal += _gcSortBuffer[i];
                gcAvg = (float)(gcTotal / gcFrameCount);
                double gcSumSqDiff = 0;
                for (int i = 0; i < gcFrameCount; i++)
                {
                    double d = _gcSortBuffer[i] - gcAvg;
                    gcSumSqDiff += d * d;
                }
                gcSd = (float)Math.Sqrt(gcSumSqDiff / gcFrameCount);
                gcP95 = _gcSortBuffer[Math.Min((int)(gcFrameCount * 0.95), gcFrameCount - 1)];
                gcP99 = _gcSortBuffer[Math.Min((int)(gcFrameCount * 0.99), gcFrameCount - 1)];
                gcMax = _gcSortBuffer[gcFrameCount - 1];
            }

            /* Compute runtime GC percentiles */
            float rtGcAvg = 0, rtGcSd = 0, rtGcP95 = 0, rtGcP99 = 0, rtGcMax = 0;
            if (runtimeGcFrameCount > 0)
            {
                Array.Sort(_runtimeGcSortBuffer, 0, runtimeGcFrameCount);
                double rtTotal = 0;
                for (int i = 0; i < runtimeGcFrameCount; i++) rtTotal += _runtimeGcSortBuffer[i];
                rtGcAvg = (float)(rtTotal / runtimeGcFrameCount);
                double rtSumSqDiff = 0;
                for (int i = 0; i < runtimeGcFrameCount; i++)
                {
                    double d = _runtimeGcSortBuffer[i] - rtGcAvg;
                    rtSumSqDiff += d * d;
                }
                rtGcSd = (float)Math.Sqrt(rtSumSqDiff / runtimeGcFrameCount);
                rtGcP95 = _runtimeGcSortBuffer[Math.Min((int)(runtimeGcFrameCount * 0.95), runtimeGcFrameCount - 1)];
                rtGcP99 = _runtimeGcSortBuffer[Math.Min((int)(runtimeGcFrameCount * 0.99), runtimeGcFrameCount - 1)];
                rtGcMax = _runtimeGcSortBuffer[runtimeGcFrameCount - 1];
            }

            /* Compute allocation rate (MB/s) over the sample window */
            float allocRateMBPerSec = 0;
            if (hasFirstRendered && lastTicksStart > firstTicksStart)
            {
                double elapsedSec = (lastTicksStart - firstTicksStart) * _msPerTick / 1000.0;
                if (elapsedSec > 0)
                {
                    double allocatedMB = (lastAllocBytes - firstAllocBytes) / (1024.0 * 1024.0);
                    allocRateMBPerSec = (float)(allocatedMB / elapsedSec);
                }
            }

            /* Retrieve per-generation heap sizes */
            long gen0Size = 0, gen1Size = 0, gen2Size = 0, lohSize = 0;
#if GNH_MAUI
            try
            {
                GCMemoryInfo gcInfo = GC.GetGCMemoryInfo();
                ReadOnlySpan<GCGenerationInfo> genInfo = gcInfo.GenerationInfo;
                if (genInfo.Length > 0) gen0Size = genInfo[0].SizeAfterBytes;
                if (genInfo.Length > 1) gen1Size = genInfo[1].SizeAfterBytes;
                if (genInfo.Length > 2) gen2Size = genInfo[2].SizeAfterBytes;
                if (genInfo.Length > 3) lohSize = genInfo[3].SizeAfterBytes;
            }
            catch (Exception)
            {
                /* GCMemoryInfo may not be fully supported on all runtimes */
            }
#endif

            return new FrameTimeStatistics
            {
                InterFrameAvgMs = avgInterFrame,
                InterFrameStdDevMs = stdDev,
                InterFrameP95Ms = _sortBuffer[Math.Min(p95Idx, interFrameCount - 1)],
                InterFrameP99Ms = _sortBuffer[Math.Min(p99Idx, interFrameCount - 1)],
                InterFrameMaxMs = _sortBuffer[interFrameCount - 1],
                AvgUpdateMs = renderedCount > 0 ? (float)(totalUpdateMs / renderedCount) : 0,
                AvgLockWaitMs = renderedCount > 0 ? (float)(totalLockWaitMs / renderedCount) : 0,
                AvgPaintMs = renderedCount > 0 ? (float)(totalPaintMs / renderedCount) : 0,
                DroppedFrameCount = droppedCount,
                DroppedFramePct = interFrameCount > 0 ? (float)droppedCount / interFrameCount * 100f : 0,
                LockFailCount = lockFailCount,
                LockFailPct = lockAttemptCount > 0 ? (float)lockFailCount / lockAttemptCount * 100f : 0,
                FPS = avgInterFrame > 0 ? 1000f / avgInterFrame : 0,
                SampleCount = interFrameCount,
                GcFrameCount = gcFrameCount,
                GcAvgMs = gcAvg,
                GcStdDevMs = gcSd,
                GcP95Ms = gcP95,
                GcP99Ms = gcP99,
                GcWorstMs = gcMax,
                RuntimeGcFrameCount = runtimeGcFrameCount,
                RuntimeGcAvgMs = rtGcAvg,
                RuntimeGcStdDevMs = rtGcSd,
                RuntimeGcP95Ms = rtGcP95,
                RuntimeGcP99Ms = rtGcP99,
                RuntimeGcWorstMs = rtGcMax,
                PauseFrameCount = pauseFrameCount,
                GcGen0Count = gcGen0Count,
                GcGen1Count = gcGen1Count,
                GcGen2Count = gcGen2Count,
                AllocationRateMBPerSec = allocRateMBPerSec,
                HeapSizeBytes = latestHeapSize,
                Gen0SizeBytes = gen0Size,
                Gen1SizeBytes = gen1Size,
                Gen2SizeBytes = gen2Size,
                LohSizeBytes = lohSize
            };
        }

        private static FrameTimeStatistics _lastStats;

        public static string GetScreenLogSummary()
        {
            _lastStats = GetStatistics();
            if (_lastStats.SampleCount == 0) return "FT: No data";

            return FormattableString.Invariant($"FT: {_lastStats.FPS:0}fps Avg:{_lastStats.InterFrameAvgMs:0.0} ({_lastStats.InterFrameStdDevMs:0.0}) P95:{_lastStats.InterFrameP95Ms:0.0} P99:{_lastStats.InterFrameP99Ms:0.0} Max:{_lastStats.InterFrameMaxMs:0.0} Drop:{_lastStats.DroppedFramePct:0.0}% Lock:{_lastStats.LockFailPct:0.0}%");
        }

        /// <summary>
        /// Returns the memory summary line showing allocation rate,
        /// heap size, and per-generation sizes. Returns null when
        /// data is insufficient. Must be called after
        /// GetScreenLogSummary() which populates _lastStats.
        /// Format: Mem: Alloc:12.3MB/s Heap:142MB Gen:8/24/96+14
        ///   or:   Mem: Alloc:12.3MB/s Heap:142MB Gen:?
        /// </summary>
        public static string GetScreenLogMemorySummary()
        {
            var s = _lastStats;
            if (s.SampleCount == 0)
                return null;

            float heapMB = s.HeapSizeBytes / (1024f * 1024f);

            string genPart;
            if (s.Gen0SizeBytes > 0 || s.Gen1SizeBytes > 0 || s.Gen2SizeBytes > 0)
            {
                float g0 = s.Gen0SizeBytes / (1024f * 1024f);
                float g1 = s.Gen1SizeBytes / (1024f * 1024f);
                float g2 = s.Gen2SizeBytes / (1024f * 1024f);
                if (s.LohSizeBytes > 0)
                {
                    float loh = s.LohSizeBytes / (1024f * 1024f);
                    genPart = FormattableString.Invariant($"Gen:{g0:0}/{g1:0}/{g2:0}+{loh:0}");
                }
                else
                {
                    genPart = FormattableString.Invariant($"Gen:{g0:0}/{g1:0}/{g2:0}");
                }
            }
            else
            {
                genPart = "Gen:?";
            }

            return FormattableString.Invariant($"Mem: Alloc:{s.AllocationRateMBPerSec:0.0}MB/s Heap:{heapMB:0}MB {genPart}");
        }

        /// <summary>
        /// Returns the forced-GC summary line, or null if no forced GC occurred.
        /// Must be called after GetScreenLogSummary() which populates _lastStats.
        /// </summary>
        public static string GetScreenLogForcedGcSummary()
        {
            var s = _lastStats;
            if (s.GcFrameCount == 0 && s.PauseFrameCount == 0)
                return null;

            string forced = s.GcFrameCount > 0
                ? FormattableString.Invariant($" {s.GcFrameCount}x Avg:{s.GcAvgMs:0.0} ({s.GcStdDevMs:0.0}) P95:{s.GcP95Ms:0.0} P99:{s.GcP99Ms:0.0} Max:{s.GcWorstMs:0.0}")
                : "";

            string pause = s.PauseFrameCount > 0
                ? FormattableString.Invariant($" Pause:{s.PauseFrameCount}")
                : "";

            return $"FoGC:{forced}{pause}";
        }

        /// <summary>
        /// Returns the runtime-GC summary line, or null if no runtime GC occurred.
        /// Must be called after GetScreenLogSummary() which populates _lastStats.
        /// </summary>
        public static string GetScreenLogRuntimeGcSummary()
        {
            var s = _lastStats;
            if (s.RuntimeGcFrameCount == 0
                && s.GcGen0Count == 0 && s.GcGen1Count == 0 && s.GcGen2Count == 0)
                return null;

            string runtime = s.RuntimeGcFrameCount > 0
                ? FormattableString.Invariant($" {s.RuntimeGcFrameCount}x Avg:{s.RuntimeGcAvgMs:0.0} ({s.RuntimeGcStdDevMs:0.0}) P95:{s.RuntimeGcP95Ms:0.0} P99:{s.RuntimeGcP99Ms:0.0} Max:{s.RuntimeGcWorstMs:0.0}")
                : "";

            string gen = (s.GcGen0Count > 0 || s.GcGen1Count > 0 || s.GcGen2Count > 0)
                ? FormattableString.Invariant($" Gen:{s.GcGen0Count}/{s.GcGen1Count}/{s.GcGen2Count}")
                : "";

            return $"RtGC:{runtime}{gen}";
        }

        public static void DumpToCsv(string path)
        {
            long currentWriteIndex = Interlocked.Read(ref _writeIndex);
            if (currentWriteIndex < 0) return;

            int sampleCount = (int)Math.Min(currentWriteIndex + 1, BufferSize);
            long startIndex = currentWriteIndex >= BufferSize ? currentWriteIndex - BufferSize + 1 : 0;

            using (StreamWriter writer = new StreamWriter(path))
            {
                writer.WriteLine("FrameNumber,Rendered,ForcedGc,RuntimeGc,PauseAffected,GcGen,AllocKB,HeapMB,InterFrameMs,UpdateMs,LockWaitMs,LockAcquired,PaintMs,TotalFrameMs");

                FrameTimeSample prev = default;
                FrameTimeSample prevRendered = default;
                bool hasPrev = false;
                bool hasPrevRendered = false;

                for (int i = 0; i < sampleCount; i++)
                {
                    int currIdx = SafeIndex(startIndex + i, BufferSize);
                    FrameTimeSample curr = _buffer[currIdx];

                    bool rendered = curr.TicksUpdateStart > 0;

                    float interFrameMs = 0;
                    bool forcedGc = false;
                    bool runtimeGc = false;
                    bool pauseAffected = false;
                    int gcGen = -1;
                    float allocKB = 0;
                    float heapMB = curr.HeapSizeBytes / (1024f * 1024f);
                    
                    if (rendered)
                    {
                        if (hasPrevRendered)
                        {
                            interFrameMs = (float)((curr.TicksFrameStart - prevRendered.TicksFrameStart) * _msPerTick);
                            forcedGc = HasForcedGcInInterval(prevRendered.TicksFrameStart, curr.TicksFrameStart);
                            pauseAffected = IsPauseAffected(prevRendered.TicksFrameStart, curr.TicksFrameStart);
                            gcGen = MaxGcGen(prevRendered, curr);
                            allocKB = (curr.AllocatedBytes - prevRendered.AllocatedBytes) / 1024f;
                            runtimeGc = !forcedGc && DidGcOccur(prevRendered, curr);
                        }
                        prevRendered = curr;
                        hasPrevRendered = true;
                    }
                    else
                    {
                        if (hasPrev)
                        {
                            interFrameMs = (float)((curr.TicksFrameStart - prev.TicksFrameStart) * _msPerTick);
                            forcedGc = HasForcedGcInInterval(prev.TicksFrameStart, curr.TicksFrameStart);
                            pauseAffected = IsPauseAffected(prev.TicksFrameStart, curr.TicksFrameStart);
                            gcGen = MaxGcGen(prev, curr);
                            allocKB = (curr.AllocatedBytes - prev.AllocatedBytes) / 1024f;
                            runtimeGc = !forcedGc && DidGcOccur(prev, curr);
                        }
                    }

                    float updateMs = curr.TicksUpdateStart > 0 && curr.TicksLockResult > 0 
                        ? (float)((curr.TicksLockResult - curr.TicksUpdateStart) * _msPerTick) : 0;
                    
                    float lockWaitMs = curr.TicksLockAttempt > 0 && curr.TicksLockResult > 0 
                        ? (float)((curr.TicksLockResult - curr.TicksLockAttempt) * _msPerTick) : 0;
                    
                    float paintMs = curr.TicksPaintStart > 0 && curr.TicksPaintEnd > 0 
                        ? (float)((curr.TicksPaintEnd - curr.TicksPaintStart) * _msPerTick) : 0;
                    
                    float totalFrameMs = curr.TicksFrameStart > 0 && curr.TicksFrameEnd > 0 
                        ? (float)((curr.TicksFrameEnd - curr.TicksFrameStart) * _msPerTick) : 0;

                    writer.WriteLine(FormattableString.Invariant($"{curr.FrameNumber},{rendered},{forcedGc},{runtimeGc},{pauseAffected},{gcGen},{allocKB:0.00},{heapMB:0.00},{interFrameMs:0.00},{updateMs:0.00},{lockWaitMs:0.00},{curr.LockAcquired},{paintMs:0.00},{totalFrameMs:0.00}"));

                    prev = curr;
                    hasPrev = true;
                }
            }
        }
    }
}

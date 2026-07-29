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

        /* Snapshot of the forced-GC call counter at frame start */
        public long ForcedGcCount;
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
        public float GcWorstMs;

        /* Runtime (non-forced) GC frame statistics (excluded from base stats) */
        public int RuntimeGcFrameCount;
        public float RuntimeGcWorstMs;

        /* Pause-affected frame statistics (menu/text/command canvas transitions) */
        public int PauseFrameCount;
    }

    public static class FrameTimeProfiler
    {
        private const int BufferSize = 1800;
        private const int MaxExclusionEvents = 64;
        private static readonly FrameTimeSample[] _buffer = new FrameTimeSample[BufferSize];
        private static long _writeIndex = -1;
        private static readonly double _msPerTick = 1000.0 / Stopwatch.Frequency;

        /* Pre-allocated sort buffer to avoid GC pressure on mobile */
        private static readonly float[] _sortBuffer = new float[BufferSize];

        /*
         * Monotonic counter incremented by MarkGcEvent() each time a
         * forced GC is requested.  BeginFrame snapshots the counter so
         * we can detect forced-GC calls between adjacent frames by
         * comparing their snapshots — immune to GC deferral timing.
         */
        private static long _forcedGcCounter = 0;

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
        /// Call immediately before a forced GC.Collect / CollectGarbage /
        /// CollectNursery to mark the current moment as a forced GC event.
        /// </summary>
        public static void MarkGcEvent()
        {
            Interlocked.Increment(ref _forcedGcCounter);
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
                ForcedGcCount = Interlocked.Read(ref _forcedGcCounter)
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
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            Interlocked.Exchange(ref _lastRenderIndex, idx);
            _buffer[SafeIndex(idx, BufferSize)].TicksUpdateStart = Stopwatch.GetTimestamp();
        }

        public static void StampLockAttempt()
        {
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksLockAttempt = Stopwatch.GetTimestamp();
        }

        public static void StampLockResult(bool acquired)
        {
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            int index = SafeIndex(idx, BufferSize);
            _buffer[index].TicksLockResult = Stopwatch.GetTimestamp();
            _buffer[index].LockAcquired = acquired;
        }

        public static void StampPaintStart()
        {
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksPaintStart = Stopwatch.GetTimestamp();
        }

        public static void StampPaintEnd()
        {
            long idx = Interlocked.Read(ref _lastRenderIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx, BufferSize)].TicksPaintEnd = Stopwatch.GetTimestamp();
        }

        public static void EndFrame()
        {
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

        private static bool IsForcedGc(FrameTimeSample prev, FrameTimeSample curr)
        {
            return curr.ForcedGcCount != prev.ForcedGcCount;
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
            float gcWorstMs = 0;

            /* Runtime (non-forced) GC frame tracking */
            int runtimeGcFrameCount = 0;
            float runtimeGcWorstMs = 0;

            /* Pause-affected frame tracking */
            int pauseFrameCount = 0;

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

                if (hasPrevRendered)
                {
                    float interFrameMs = (float)((curr.TicksFrameStart - prevRenderedFrameStart) * _msPerTick);
                    bool forcedGcHit = IsForcedGc(prevRenderedSample, curr);
                    bool pauseHit = IsPauseAffected(prevRenderedFrameStart, curr.TicksFrameStart);
                    bool runtimeGcHit = !forcedGcHit && DidGcOccur(prevRenderedSample, curr);

                    if (forcedGcHit)
                    {
                        /* Forced GC happened during this gap — track separately */
                        gcFrameCount++;
                        if (interFrameMs > gcWorstMs)
                            gcWorstMs = interFrameMs;
                    }
                    else if (runtimeGcHit)
                    {
                        /* Runtime GC happened during this gap — track separately */
                        runtimeGcFrameCount++;
                        if (interFrameMs > runtimeGcWorstMs)
                            runtimeGcWorstMs = interFrameMs;
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
                GcWorstMs = gcWorstMs,
                RuntimeGcFrameCount = runtimeGcFrameCount,
                RuntimeGcWorstMs = runtimeGcWorstMs,
                PauseFrameCount = pauseFrameCount
            };
        }

        public static string GetScreenLogSummary()
        {
            var stats = GetStatistics();
            if (stats.SampleCount == 0) return "FT: No data";

            string gcInfo = stats.GcFrameCount > 0
                ? $" GC:{stats.GcFrameCount}x{stats.GcWorstMs:0.0}"
                : "";

            string runtimeGcInfo = stats.RuntimeGcFrameCount > 0
                ? $" RtGC:{stats.RuntimeGcFrameCount}x{stats.RuntimeGcWorstMs:0.0}"
                : "";

            string pauseInfo = stats.PauseFrameCount > 0
                ? $" Pse:{stats.PauseFrameCount}"
                : "";

            return FormattableString.Invariant($"FT: {stats.FPS:0}fps Avg:{stats.InterFrameAvgMs:0.0} SD:{stats.InterFrameStdDevMs:0.0} P95:{stats.InterFrameP95Ms:0.0} P99:{stats.InterFrameP99Ms:0.0} Max:{stats.InterFrameMaxMs:0.0} Drop:{stats.DroppedFramePct:0.0}% Lock:{stats.LockFailPct:0.0}%{gcInfo}{runtimeGcInfo}{pauseInfo}");
        }

        public static void DumpToCsv(string path)
        {
            long currentWriteIndex = Interlocked.Read(ref _writeIndex);
            if (currentWriteIndex < 0) return;

            int sampleCount = (int)Math.Min(currentWriteIndex + 1, BufferSize);
            long startIndex = currentWriteIndex >= BufferSize ? currentWriteIndex - BufferSize + 1 : 0;

            using (StreamWriter writer = new StreamWriter(path))
            {
                writer.WriteLine("FrameNumber,Rendered,ForcedGc,RuntimeGc,PauseAffected,GcGen,AllocKB,InterFrameMs,UpdateMs,LockWaitMs,LockAcquired,PaintMs,TotalFrameMs");

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
                    
                    if (rendered)
                    {
                        if (hasPrevRendered)
                        {
                            interFrameMs = (float)((curr.TicksFrameStart - prevRendered.TicksFrameStart) * _msPerTick);
                            forcedGc = IsForcedGc(prevRendered, curr);
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
                            forcedGc = IsForcedGc(prev, curr);
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

                    writer.WriteLine(FormattableString.Invariant($"{curr.FrameNumber},{rendered},{forcedGc},{runtimeGc},{pauseAffected},{gcGen},{allocKB:0.00},{interFrameMs:0.00},{updateMs:0.00},{lockWaitMs:0.00},{curr.LockAcquired},{paintMs:0.00},{totalFrameMs:0.00}"));

                    prev = curr;
                    hasPrev = true;
                }
            }
        }
    }
}

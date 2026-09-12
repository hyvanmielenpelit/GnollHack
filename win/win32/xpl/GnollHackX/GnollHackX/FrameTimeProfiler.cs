using System;
using System.Diagnostics;
using System.Diagnostics.Tracing;
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

        /* Measured across the GC.Collect call itself, so unlike a runtime
           collection this is the real pause rather than a frame gap */
        public long DurationTicks;
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
        private const int MaxGcReasonEvents = 64;
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
         * Why the runtime began each collection, stamped when it began so an entry can
         * be matched to the frame gap containing it, the same way a forced collection's
         * delta is. The runtime raises a GC event carrying that reason; the ring is
         * filled from it, and stays empty on a runtime that raises no such event.
         */
        private static readonly GcReasonEvent[] _gcReasons = new GcReasonEvent[MaxGcReasonEvents];
        private static long _gcReasonWriteIndex = -1;
        private static int _gcReasonListenerStarted = 0;
        private static GcEventListener _gcEventListener;

        private struct GcReasonEvent
        {
            public long Timestamp;
            public int Depth;
            public int Reason;
        }

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
        [ThreadStatic]
        private static long _beforeTicks;

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
            _beforeTicks = Stopwatch.GetTimestamp();
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

            long afterTicks = Stopwatch.GetTimestamp();
            long idx = Interlocked.Increment(ref _forcedGcDeltaWriteIndex);
            _forcedGcDeltas[SafeIndex(idx, MaxForcedGcEvents)] = new ForcedGcDelta
            {
                Timestamp = afterTicks,
                DeltaGen0 = deltaGen0,
                DeltaGen1 = deltaGen1,
                DeltaGen2 = deltaGen2,
                DurationTicks = _beforeTicks > 0 ? afterTicks - _beforeTicks : 0
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

            if (idx >= 1 && GHApp.IsDebugScreenLoggingOn)
            {
                EnsureGcReasonListener();
                FrameTimeSample prev = _buffer[SafeIndex(idx - 1, BufferSize)];
                if (DidGcOccur(prev, _buffer[index]))
                    LogGcEvent(prev, _buffer[index]);
            }
        }

        /* One screen-log line per collection, so the log shows when collections landed
           while the dashboard carries the aggregates. Consecutive samples are used
           rather than consecutive rendered frames: the gap is tighter, so a runtime
           collection is attributed more precisely.

           Two durations are reported. "ps" is the collection's own pause: measured
           across the GC.Collect call for a forced collection, and taken from
           GCMemoryInfo for a runtime one, which is discovered only after the fact.
           "gap" is the frame gap that contained the collection -- the observable
           hitch, which also holds whatever else the frame did, so a short pause
           inside a long gap means the hitch was not the collection. "bl" and "bg"
           mark a blocking against a background collection; neither appears when the
           runtime does not report it. */
        private static void LogGcEvent(in FrameTimeSample prev, in FrameTimeSample curr)
        {
            int d0 = curr.GcCount0 - prev.GcCount0;
            int d1 = curr.GcCount1 - prev.GcCount1;
            int d2 = curr.GcCount2 - prev.GcCount2;

            long forcedDurationTicks;
            bool forced = TryGetForcedGcInInterval(prev.TicksFrameStart, curr.TicksFrameStart,
                out forcedDurationTicks);

            double gapMs = (curr.TicksFrameStart - prev.TicksFrameStart) * _msPerTick;

            GcInfoSnapshot info;
            bool haveInfo = TryGetLastGcInfo(out info);

            /* A forced collection may hold several GC.Collect calls, which GCMemoryInfo
               reports only the last of, so the span measured around them wins there */
            double pauseMs = forced && forcedDurationTicks > 0
                ? forcedDurationTicks * _msPerTick
                : (haveInfo ? info.PauseMs : 0);

            string kindFlag = haveInfo ? (info.Concurrent ? " bg" : " bl") : "";
            string psPart = pauseMs > 0
                ? FormattableString.Invariant($" ps:{pauseMs:0.0}ms")
                : "";
            string lohPart = haveInfo && (info.LohBefore > 0 || info.LohAfter > 0)
                ? FormattableString.Invariant(
                    $" loh:{info.LohBefore / (1024f * 1024f):0}>{info.LohAfter / (1024f * 1024f):0}")
                : "";

            float heapBeforeMB = prev.HeapSizeBytes / (1024f * 1024f);
            float heapAfterMB = curr.HeapSizeBytes / (1024f * 1024f);

            /* A canvas transition in the gap makes the elapsed time meaningless, so the
               line says so rather than presenting it as a collection cost */
            string canvasPause = IsPauseAffected(prev.TicksFrameStart, curr.TicksFrameStart)
                ? " [pause]"
                : "";

            GHApp.MaybeWriteScreenLog(FormattableString.Invariant(
                $"GC {(forced ? "forced" : "rt")} {GenerationTag(d0, d1, d2)}{kindFlag}{psPart} gap:{gapMs:0.0}ms {heapBeforeMB:0}>{heapAfterMB:0}MB{lohPart}{canvasPause}"));

            int gcReason;
            bool haveReason = TryGetGcReasonInInterval(prev.TicksFrameStart, curr.TicksFrameStart,
                out gcReason);

            if (haveInfo || haveReason)
                GHApp.MaybeWriteScreenLog(BuildGcDetailLine(info, haveInfo,
                    haveReason ? gcReason : -1));
        }

        /* The continuation line of a collection's report, carrying the counters that say
           why a full collection was needed and why it cost what it did. The pinned object
           heap, like the large object heap, is charged to the gen2 budget, so either can
           force a gen2 while the nursery stays quiet; pinned objects block compaction and
           make a collection dearer than its heap size suggests. "!" marks a memory load at
           or above the threshold where the runtime collects to relieve the system rather
           than because a budget ran out. Sizes are in megabytes, the pinned and
           finalization figures are counts of objects.

           "fin" is how many objects the collection found waiting to be finalized. A
           wrapper around a native resource is only released when it is finalized, so a
           count that grows with the interval between collections means the collection is
           clearing a backlog that built up rather than reclaiming what the frame just
           allocated.

           "why" is the runtime's own reason for starting the collection. It arrives by a
           different route than the rest, so the line carries whichever of the two is
           available: a collection the runtime has not yet published figures for still
           reports why it happened. */
        private static string BuildGcDetailLine(in GcInfoSnapshot info, bool haveInfo, int reason)
        {
            string why = reason >= 0 ? " why:" + GcReasonName(reason) : "";
            if (!haveInfo)
                return "GC .." + why;

            string mem = "";
            if (info.MemoryAvailable > 0)
            {
                float loadPct = (float)info.MemoryLoad / info.MemoryAvailable * 100f;
                string high = info.MemoryLoadThreshold > 0
                    && info.MemoryLoad >= info.MemoryLoadThreshold ? "!" : "";
                mem = FormattableString.Invariant($" mem:{loadPct:0}%{high}");
            }

            float pohBeforeMB = info.PohBefore / (1024f * 1024f);
            float pohAfterMB = info.PohAfter / (1024f * 1024f);
            float promotedMB = info.Promoted / (1024f * 1024f);

            return FormattableString.Invariant(
                $"GC .. poh:{pohBeforeMB:0.0}>{pohAfterMB:0.0} pin:{info.PinnedObjects} fin:{info.FinalizationPending} prom:{promotedMB:0.0}MB{mem}{why}");
        }

#if GNH_MAUI
        /* Index of the collection whose GCMemoryInfo was last reported */
        private static long _lastGcInfoIndex;
#endif

        /* What GCMemoryInfo reports about one collection */
        private struct GcInfoSnapshot
        {
            public bool Concurrent;
            public double PauseMs;
            public long LohBefore;
            public long LohAfter;
            public long PohBefore;
            public long PohAfter;
            public long Promoted;
            public long PinnedObjects;
            public long FinalizationPending;
            public long MemoryLoad;
            public long MemoryLoadThreshold;
            public long MemoryAvailable;
        }

        /* The most recent collection as the runtime describes it. GCMemoryInfo always
           describes the latest collection, so its index is checked against the one last
           reported: an index that has not advanced means the runtime has not published
           the collection just detected and the figures on hand belong to an earlier one.
           A background collection suspends more than once and the sum of its pauses is
           the total suspension. GenerationInfo runs gen0, gen1, gen2, large object heap,
           pinned object heap, and the last two are read by index because both are charged
           to the gen2 budget. */
        private static bool TryGetLastGcInfo(out GcInfoSnapshot snap)
        {
            /* Field by field rather than a default instance, so that the build without
               GCMemoryInfo, where nothing below fills them in, still counts them as
               written */
            snap.Concurrent = false;
            snap.PauseMs = 0;
            snap.LohBefore = 0;
            snap.LohAfter = 0;
            snap.PohBefore = 0;
            snap.PohAfter = 0;
            snap.Promoted = 0;
            snap.PinnedObjects = 0;
            snap.FinalizationPending = 0;
            snap.MemoryLoad = 0;
            snap.MemoryLoadThreshold = 0;
            snap.MemoryAvailable = 0;
#if GNH_MAUI
            try
            {
                GCMemoryInfo info = GC.GetGCMemoryInfo();
                if (info.Index <= Interlocked.Read(ref _lastGcInfoIndex))
                    return false;

                Interlocked.Exchange(ref _lastGcInfoIndex, info.Index);
                snap.Concurrent = info.Concurrent;
                snap.Promoted = info.PromotedBytes;
                snap.PinnedObjects = info.PinnedObjectsCount;
                snap.FinalizationPending = info.FinalizationPendingCount;
                snap.MemoryLoad = info.MemoryLoadBytes;
                snap.MemoryLoadThreshold = info.HighMemoryLoadThresholdBytes;
                snap.MemoryAvailable = info.TotalAvailableMemoryBytes;

                ReadOnlySpan<TimeSpan> pauses = info.PauseDurations;
                for (int i = 0; i < pauses.Length; i++)
                    snap.PauseMs += pauses[i].TotalMilliseconds;

                ReadOnlySpan<GCGenerationInfo> genInfo = info.GenerationInfo;
                if (genInfo.Length > 3)
                {
                    snap.LohBefore = genInfo[3].SizeBeforeBytes;
                    snap.LohAfter = genInfo[3].SizeAfterBytes;
                }
                if (genInfo.Length > 4)
                {
                    snap.PohBefore = genInfo[4].SizeBeforeBytes;
                    snap.PohAfter = genInfo[4].SizeAfterBytes;
                }
                return true;
            }
            catch (Exception)
            {
                /* GCMemoryInfo may not be fully supported on all runtimes */
                return false;
            }
#else
            return false;
#endif
        }

        /* Every generation that topped off a collection in the gap, ascending, each with
           the number of collections that stopped there: "g0x3+g2" is three gen0s and one
           gen2. CollectionCount(n) counts collections of generation n or higher, so the
           per-level counts are differences rather than the raw deltas.

           The three counters are read by separate calls, so a collection landing between
           them leaves one sample internally inconsistent and a difference negative. Those
           are clamped, which is also why no total is printed alongside: after a clamp the
           parts need not sum to the gap's raw collection count. */
        private static string GenerationTag(int d0, int d1, int d2)
        {
            int n2 = d2 > 0 ? d2 : 0;
            int n1 = d1 - d2 > 0 ? d1 - d2 : 0;
            int n0 = d0 - d1 > 0 ? d0 - d1 : 0;

            if (n0 + n1 + n2 == 0)
                return "g?";

            string tag = "";
            string sep = "";
            if (n0 > 0)
            {
                tag = GenerationPart("g0", n0);
                sep = "+";
            }
            if (n1 > 0)
            {
                tag += sep + GenerationPart("g1", n1);
                sep = "+";
            }
            if (n2 > 0)
                tag += sep + GenerationPart("g2", n2);

            return tag;
        }

        private static string GenerationPart(string gen, int count)
        {
            return count > 1 ? gen + "x" + count.ToString(CultureInfo.InvariantCulture) : gen;
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
            long durationTicks;
            return TryGetForcedGcInInterval(ticksStart, ticksEnd, out durationTicks);
        }

        /* Yields the measured duration of the last forced collect that completed in the
           interval, so a forced collection can report its own pause instead of the frame
           gap that contains it. */
        private static bool TryGetForcedGcInInterval(long ticksStart, long ticksEnd, out long durationTicks)
        {
            durationTicks = 0;
            long writeIndex = Interlocked.Read(ref _forcedGcDeltaWriteIndex);
            if (writeIndex < 0) return false;

            int count = (int)Math.Min(writeIndex + 1, MaxForcedGcEvents);
            long firstIdx = writeIndex >= MaxForcedGcEvents ? writeIndex - MaxForcedGcEvents + 1 : 0;

            bool found = false;
            for (int i = 0; i < count; i++)
            {
                ForcedGcDelta delta = _forcedGcDeltas[SafeIndex(firstIdx + i, MaxForcedGcEvents)];
                if (delta.Timestamp >= ticksStart && delta.Timestamp < ticksEnd)
                {
                    durationTicks = delta.DurationTicks;
                    found = true;
                }
            }
            return found;
        }

        /* The reason for the deepest collection that began in the interval, matching the
           generation the line itself reports when a gap holds more than one. */
        private static bool TryGetGcReasonInInterval(long ticksStart, long ticksEnd, out int reason)
        {
            reason = -1;
            long writeIndex = Interlocked.Read(ref _gcReasonWriteIndex);
            if (writeIndex < 0) return false;

            int count = (int)Math.Min(writeIndex + 1, MaxGcReasonEvents);
            long firstIdx = writeIndex >= MaxGcReasonEvents ? writeIndex - MaxGcReasonEvents + 1 : 0;

            int bestDepth = -1;
            bool found = false;
            for (int i = 0; i < count; i++)
            {
                GcReasonEvent ev = _gcReasons[SafeIndex(firstIdx + i, MaxGcReasonEvents)];
                if (ev.Timestamp >= ticksStart && ev.Timestamp < ticksEnd && ev.Depth >= bestDepth)
                {
                    bestDepth = ev.Depth;
                    reason = ev.Reason;
                    found = true;
                }
            }
            return found;
        }

        /* Names for the runtime's collection reasons. A value outside the known set is
           reported as its raw number rather than guessed at. */
        private static string GcReasonName(int reason)
        {
            switch (reason)
            {
            case 0: return "alloc";
            case 1: return "induced";
            case 2: return "lowmem";
            case 3: return "empty";
            case 4: return "allocloh";
            case 5: return "oos_soh";
            case 6: return "oos_loh";
            case 7: return "ind_nf";
            case 8: return "stress";
            case 9: return "lowmem_b";
            case 10: return "ind_comp";
            case 11: return "lowmem_h";
            case 12: return "pm_full";
            case 13: return "lowmem_hb";
            case 14: return "bgc_soh";
            case 15: return "bgc_loh";
            case 16: return "bgc_step";
            case 17: return "ind_aggr";
            default: return "r" + reason.ToString(CultureInfo.InvariantCulture);
            }
        }

        /* Subscribed on the first frame that logs, so a session that never turns debug
           logging on never pays for the subscription. */
        private static void EnsureGcReasonListener()
        {
            if (Volatile.Read(ref _gcEventListener) != null)
                return;
            if (Interlocked.CompareExchange(ref _gcReasonListenerStarted, 1, 0) != 0)
                return;

            try
            {
                Volatile.Write(ref _gcEventListener, new GcEventListener());
            }
            catch (Exception)
            {
                /* Event listening is unavailable on this runtime */
            }
        }

        /* Listens for the runtime's own GC events, whose start event carries the reason
           the collection was begun -- an allocation budget, a Collect call, low memory.
           The callback runs on a runtime thread as a collection starts, so it stamps the
           ring and does nothing else. A runtime that raises no such event never calls it,
           leaving the reason simply absent from the log. */
        private sealed class GcEventListener : EventListener
        {
            private const int GcKeyword = 0x1;
            private const string RuntimeEventSourceName = "Microsoft-Windows-DotNETRuntime";

            protected override void OnEventSourceCreated(EventSource eventSource)
            {
                if (eventSource == null || eventSource.Name != RuntimeEventSourceName)
                    return;

                try
                {
                    EnableEvents(eventSource, EventLevel.Informational, (EventKeywords)GcKeyword);
                }
                catch (Exception)
                {
                    /* Not every runtime raises the GC events */
                }
            }

            protected override void OnEventWritten(EventWrittenEventArgs eventData)
            {
                try
                {
                    if (eventData == null || eventData.EventName == null
                        || !eventData.EventName.StartsWith("GCStart", StringComparison.Ordinal))
                        return;
                    if (eventData.PayloadNames == null || eventData.Payload == null)
                        return;

                    int depth = -1;
                    int reason = -1;
                    int count = Math.Min(eventData.PayloadNames.Count, eventData.Payload.Count);
                    for (int i = 0; i < count; i++)
                    {
                        string name = eventData.PayloadNames[i];
                        if (name == "Depth")
                            depth = Convert.ToInt32(eventData.Payload[i], CultureInfo.InvariantCulture);
                        else if (name == "Reason")
                            reason = Convert.ToInt32(eventData.Payload[i], CultureInfo.InvariantCulture);
                    }

                    if (reason < 0)
                        return;

                    long idx = Interlocked.Increment(ref _gcReasonWriteIndex);
                    _gcReasons[SafeIndex(idx, MaxGcReasonEvents)] = new GcReasonEvent
                    {
                        Timestamp = Stopwatch.GetTimestamp(),
                        Depth = depth,
                        Reason = reason
                    };
                }
                catch (Exception)
                {
                    /* A diagnostic must never throw out of a runtime callback */
                }
            }
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

        /// <summary>
        /// Recomputes the statistics and hands them to the debug dashboard.
        /// Called on the main thread at the screen log's cadence, not per frame:
        /// the dashboard rebuilds every row string when the snapshot changes.
        /// </summary>
        public static void PublishDashboardSnapshot()
        {
            GHDebugDashboard.PublishFrameStats(GetStatistics(), IsEnabled);
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

using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Threading;

namespace GnollHackX.Performance
{
    /* Per-display-callback record of the map frame pipeline: the vsync the platform
       reported, what the pacing logic decided, which content state was requested and which
       was painted, and when each stage ran. Every stage refers to the frame by FrameId, so a
       paint that runs on another thread, or is merged into a later one, is attributed to the
       tick that requested it.

       Threading: BeginTick, the Stamp* tick methods and EndTick run on the UI thread; the
       paint methods run on whichever thread paints the map (the UI thread, or the GL thread
       on Android). A paint never overlaps another paint. Records are read at the end of a
       window; a read racing a write may see one torn record, which the consumers tolerate.

       The ring is allocated when the timeline is first enabled, so a session that never
       enables it pays one boolean check per call. Must compile under C# 7.3. */
    public static class GHFrameTimeline
    {
        /* 60 s at 144 Hz */
        public const int Capacity = 8640;

        private const int PeriodWindow = 15;
        private const long NanosPerSecond = 1000000000L;

        private static int _enabled = 0;
        private static GHFrameRecord[] _ring = null;

        /* Compositor-side frames, written by one platform thread at a time */
        private static GHCompositorFrame[] _compositorRing = null;
        private static long _compositorCount = 0;

        private static long _lastFrameId = 0;          /* id of the tick in progress or last completed */
        private static long _pendingPaintFrameId = 0;  /* invalidated, not yet picked up by a paint */
        private static long _lastPaintedFrameId = 0;
        private static long _currentPaintFrameId = 0;
        private static long _orphanPaintCount = 0;
        private static long _coalescedCount = 0;

        /* Platform frame timing handed over by the platform callback just before the tick */
        private static long _pendingVsyncTicks = 0;
        private static long _pendingExpectedPresentTicks = 0;
        private static long _pendingPlatformFrameTicks = 0;
        private static long _lastPlatformFrameTicks = 0;
        private static long _lastCallbackStartTicks = 0;

        /* UI-thread request work since the last tick, handed to the next one */
        private static int _pendingContentEvents = 0;
        private static long _pendingRequestTicks = 0;
        private static long _lastPaintedMapGeneration = -1;

        /* Platform clock anchor; the first and latest pairs are kept to expose drift */
        private static GHClockAnchor _anchor;
        private static GHClockAnchor _firstAnchor;
        private static int _hasAnchor = 0;

        /* Measured refresh period: median of recent positive frame-time deltas */
        private static readonly long[] _periodDeltas = new long[PeriodWindow];
        private static readonly long[] _periodSortBuffer = new long[PeriodWindow];
        private static int _periodDeltaCount = 0;
        private static int _periodDeltaIndex = 0;
        private static long _measuredPeriodTicks = 0;

        public static long Frequency { get { return Stopwatch.Frequency; } }

        public static bool IsEnabled
        {
            get { return Interlocked.CompareExchange(ref _enabled, 0, 0) != 0; }
            set
            {
                if (value)
                {
                    if (_ring == null)
                        _ring = new GHFrameRecord[Capacity];
                    if (_compositorRing == null)
                        _compositorRing = new GHCompositorFrame[Capacity];
                    Reset();
                    Interlocked.Exchange(ref _enabled, 1);
                }
                else
                {
                    Interlocked.Exchange(ref _enabled, 0);
                }
            }
        }

        public static long LastFrameId { get { return Interlocked.Read(ref _lastFrameId); } }

        /* The frame the paint in progress belongs to, 0 outside a paint or for an orphan paint */
        public static long CurrentPaintFrameId { get { return Interlocked.Read(ref _currentPaintFrameId); } }
        public static long OrphanPaintCount { get { return Interlocked.Read(ref _orphanPaintCount); } }
        public static long CoalescedCount { get { return Interlocked.Read(ref _coalescedCount); } }
        public static long MeasuredRefreshPeriodTicks { get { return Interlocked.Read(ref _measuredPeriodTicks); } }

        public static double MeasuredRefreshPeriodMs
        {
            get
            {
                long p = MeasuredRefreshPeriodTicks;
                return p > 0 ? p * 1000.0 / Stopwatch.Frequency : 0;
            }
        }

        public static bool HasClockAnchor { get { return Interlocked.CompareExchange(ref _hasAnchor, 0, 0) != 0; } }
        public static GHClockAnchor FirstClockAnchor { get { return _firstAnchor; } }
        public static GHClockAnchor LatestClockAnchor { get { return _anchor; } }

        private static void Reset()
        {
            if (_ring != null)
                Array.Clear(_ring, 0, _ring.Length);
            if (_compositorRing != null)
                Array.Clear(_compositorRing, 0, _compositorRing.Length);
            Interlocked.Exchange(ref _compositorCount, 0);
            Interlocked.Exchange(ref _lastFrameId, 0);
            Interlocked.Exchange(ref _pendingPaintFrameId, 0);
            Interlocked.Exchange(ref _lastPaintedFrameId, 0);
            Interlocked.Exchange(ref _currentPaintFrameId, 0);
            Interlocked.Exchange(ref _orphanPaintCount, 0);
            Interlocked.Exchange(ref _coalescedCount, 0);
            _pendingVsyncTicks = 0;
            _pendingExpectedPresentTicks = 0;
            _pendingPlatformFrameTicks = 0;
            _lastPlatformFrameTicks = 0;
            _lastCallbackStartTicks = 0;
            Interlocked.Exchange(ref _pendingContentEvents, 0);
            Interlocked.Exchange(ref _pendingRequestTicks, 0);
            Interlocked.Exchange(ref _lastPaintedMapGeneration, -1);
            _periodDeltaCount = 0;
            _periodDeltaIndex = 0;
            Interlocked.Exchange(ref _measuredPeriodTicks, 0);
            Interlocked.Exchange(ref _hasAnchor, 0);
            GHCadenceMonitor.Reset();
        }

        private static int IndexOf(long frameId)
        {
            return (int)(frameId % Capacity);
        }

        /* ---- Platform clock ---- */

        /* Records a simultaneous reading of the platform's monotonic clock in nanoseconds
           and the Stopwatch. Both clocks are monotonic at the same rate on every platform
           this runs on; the anchor makes the conversion independent of whether they also
           share an epoch. */
        public static void UpdatePlatformClockAnchor(long platformNanos, long stopwatchTicks)
        {
            GHClockAnchor a;
            a.PlatformNanos = platformNanos;
            a.StopwatchTicks = stopwatchTicks;
            _anchor = a;
            if (Interlocked.Exchange(ref _hasAnchor, 1) == 0)
                _firstAnchor = a;
        }

        public static long PlatformNanosToTicks(long platformNanos)
        {
            if (platformNanos == 0 || !HasClockAnchor)
                return 0;
            GHClockAnchor a = _anchor;
            double deltaTicks = (platformNanos - a.PlatformNanos) * (double)Stopwatch.Frequency / NanosPerSecond;
            return a.StopwatchTicks + (long)Math.Round(deltaTicks);
        }

        public static long SecondsToTicks(double seconds)
        {
            return (long)Math.Round(seconds * Stopwatch.Frequency);
        }

        /* TimeSpan ticks (100 ns) to Stopwatch units */
        public static long TimeSpanTicksToTicks(long timeSpanTicks)
        {
            return (long)Math.Round(timeSpanTicks * (double)Stopwatch.Frequency / TimeSpan.TicksPerSecond);
        }

        /* Called by the platform callback immediately before the tick it describes.
           vsyncTicks and expectedPresentTicks are in the Stopwatch domain (0 if unknown);
           platformFrameTicks is in Stopwatch units with the platform's own epoch. */
        public static void SetPendingPlatformFrame(long vsyncTicks, long expectedPresentTicks, long platformFrameTicks)
        {
            if (!IsEnabled)
                return;
            _pendingVsyncTicks = vsyncTicks;
            _pendingExpectedPresentTicks = expectedPresentTicks;
            _pendingPlatformFrameTicks = platformFrameTicks;
        }

        /* ---- Tick (UI thread) ---- */

        public static long BeginTick()
        {
            if (!IsEnabled)
                return 0;
            long now = Stopwatch.GetTimestamp();
            long id = Interlocked.Increment(ref _lastFrameId);
            int idx = IndexOf(id);

            GHFrameRecord r = new GHFrameRecord();
            r.FrameId = id;
            r.CallbackStartTicks = now;
            r.VsyncTicks = _pendingVsyncTicks;
            r.ExpectedPresentTicks = _pendingExpectedPresentTicks;
            r.PlatformFrameTicks = _pendingPlatformFrameTicks;
            r.GcCount0 = GC.CollectionCount(0);
            r.GcCount1 = GC.CollectionCount(1);
            r.GcCount2 = GC.CollectionCount(2);

            /* The platform's own frame time is the better period source; callback start
               times carry the UI thread's scheduling jitter */
            long frameTime = r.PlatformFrameTicks != 0 ? r.PlatformFrameTicks : r.VsyncTicks;
            if (frameTime != 0)
            {
                if (_lastPlatformFrameTicks != 0)
                {
                    long delta = frameTime - _lastPlatformFrameTicks;
                    if (delta == 0)
                        r.Flags |= GHFrameFlags.DuplicateCallback;
                    else if (delta > 0)
                        AddPeriodDelta(delta);
                }
                _lastPlatformFrameTicks = frameTime;
            }
            else if (_lastCallbackStartTicks != 0)
            {
                long delta = now - _lastCallbackStartTicks;
                if (delta > 0)
                    AddPeriodDelta(delta);
            }
            _lastCallbackStartTicks = now;
            r.RefreshPeriodTicks = Interlocked.Read(ref _measuredPeriodTicks);

            _pendingVsyncTicks = 0;
            _pendingExpectedPresentTicks = 0;
            _pendingPlatformFrameTicks = 0;
            r.ContentEvents = (GHContentEvent)Interlocked.Exchange(ref _pendingContentEvents, 0);
            r.RequestTicks = Interlocked.Exchange(ref _pendingRequestTicks, 0);

            _ring[idx] = r;
            return id;
        }

        /* Called by the UI thread after a batch of game requests: what kinds of content they
           brought and how long handling them took. Attached to the next tick, whose callback
           that work may have delayed and whose paint first shows the content. */
        public static void AddRequestWork(GHContentEvent events, long elapsedTicks)
        {
            if (!IsEnabled)
                return;
            if (events != GHContentEvent.None)
            {
                int current, updated;
                do
                {
                    current = Interlocked.CompareExchange(ref _pendingContentEvents, 0, 0);
                    updated = current | (int)events;
                }
                while (Interlocked.CompareExchange(ref _pendingContentEvents, updated, current) != current);
            }
            if (elapsedTicks > 0)
                Interlocked.Add(ref _pendingRequestTicks, elapsedTicks);
        }

        private static void AddPeriodDelta(long delta)
        {
            _periodDeltas[_periodDeltaIndex] = delta;
            _periodDeltaIndex = (_periodDeltaIndex + 1) % PeriodWindow;
            if (_periodDeltaCount < PeriodWindow)
                _periodDeltaCount++;

            int n = _periodDeltaCount;
            Array.Copy(_periodDeltas, _periodSortBuffer, n);
            Array.Sort(_periodSortBuffer, 0, n);
            Interlocked.Exchange(ref _measuredPeriodTicks, _periodSortBuffer[n / 2]);
        }

        private static int _lastTargetFps = 0;
        private static int _lastAssumedRefreshHz = 0;

        /* The target rate of the most recent tick, 0 before the first */
        public static int LastTargetFps { get { return Interlocked.CompareExchange(ref _lastTargetFps, 0, 0); } }

        /* The refresh rate the pacing logic assumed on the most recent tick, 0 before the first */
        public static int LastAssumedRefreshHz { get { return Interlocked.CompareExchange(ref _lastAssumedRefreshHz, 0, 0); } }

        public static void StampTarget(int targetFps, int assumedRefreshHz)
        {
            if (!IsEnabled)
                return;
            Interlocked.Exchange(ref _lastTargetFps, targetFps);
            Interlocked.Exchange(ref _lastAssumedRefreshHz, assumedRefreshHz);
            long id = Interlocked.Read(ref _lastFrameId);
            if (id <= 0)
                return;
            int idx = IndexOf(id);
            _ring[idx].TargetFps = (short)Math.Max(0, Math.Min(short.MaxValue, targetFps));
            _ring[idx].AssumedRefreshHz = (short)Math.Max(0, Math.Min(short.MaxValue, assumedRefreshHz));
        }

        /* Called from UpdateMainCanvas with the content state the tick requested */
        public static void StampUpdate(long mainCounter, long generalCounter)
        {
            if (!IsEnabled)
                return;
            long id = Interlocked.Read(ref _lastFrameId);
            if (id <= 0)
                return;
            int idx = IndexOf(id);
            _ring[idx].MainCounter = mainCounter;
            _ring[idx].GeneralCounter = generalCounter;
        }

        public static void StampInvalidate(GHInvalidateOutcome outcome)
        {
            if (!IsEnabled)
                return;
            long id = Interlocked.Read(ref _lastFrameId);
            if (id <= 0)
                return;
            int idx = IndexOf(id);
            _ring[idx].Invalidate = outcome;
            if (outcome == GHInvalidateOutcome.Invalidated)
            {
                _ring[idx].InvalidateTicks = Stopwatch.GetTimestamp();
                Interlocked.Exchange(ref _pendingPaintFrameId, id);
            }
        }

        public static void EndTick(GHPacingDecision decision)
        {
            if (!IsEnabled)
                return;
            long id = Interlocked.Read(ref _lastFrameId);
            if (id <= 0)
                return;
            int idx = IndexOf(id);
            if (_ring[idx].FrameId != id)
                return;
            _ring[idx].Pacing = decision;
            _ring[idx].CallbackEndTicks = Stopwatch.GetTimestamp();
        }

        /* ---- Paint (paint thread) ---- */

        /* Called first thing in the map's paint handler, before any early return. Takes the
           pending invalidation, marks every earlier invalidation that no paint picked up as
           coalesced, and returns the frame being painted, or 0 for a paint no tick asked for. */
        public static long BeginPaint(bool onUiThread)
        {
            if (!IsEnabled)
                return 0;
            long now = Stopwatch.GetTimestamp();
            long id = Interlocked.Exchange(ref _pendingPaintFrameId, 0);
            if (id <= 0)
            {
                Interlocked.Increment(ref _orphanPaintCount);
                Interlocked.Exchange(ref _currentPaintFrameId, 0);
                return 0;
            }

            long last = Interlocked.Read(ref _lastPaintedFrameId);
            long from = Math.Max(last + 1, id - Capacity + 1);
            for (long k = from; k < id; k++)
            {
                int kidx = IndexOf(k);
                if (_ring[kidx].FrameId == k
                    && _ring[kidx].Invalidate == GHInvalidateOutcome.Invalidated
                    && _ring[kidx].Paint == GHPaintOutcome.None)
                {
                    _ring[kidx].Paint = GHPaintOutcome.Coalesced;
                    Interlocked.Increment(ref _coalescedCount);
                }
            }
            Interlocked.Exchange(ref _lastPaintedFrameId, id);

            int idx = IndexOf(id);
            if (_ring[idx].FrameId != id)
                return 0;
            _ring[idx].PaintStartTicks = now;
            _ring[idx].PaintOnUiThread = onUiThread;
            _ring[idx].Paint = GHPaintOutcome.Painted;
            Interlocked.Exchange(ref _currentPaintFrameId, id);
            return id;
        }

        public static void SetPaintOutcome(long frameId, GHPaintOutcome outcome)
        {
            if (frameId <= 0 || !IsEnabled)
                return;
            int idx = IndexOf(frameId);
            if (_ring[idx].FrameId == frameId)
                _ring[idx].Paint = outcome;
        }

        /* For early returns deep inside the paint, where the frame id is not in scope */
        public static void SetCurrentPaintOutcome(GHPaintOutcome outcome)
        {
            if (!IsEnabled)
                return;
            SetPaintOutcome(Interlocked.Read(ref _currentPaintFrameId), outcome);
        }

        /* Map data lock taken inside the paint of the current frame */
        public static void StampLock(long attemptTicks, long resultTicks, bool acquired)
        {
            if (!IsEnabled)
                return;
            long id = Interlocked.Read(ref _currentPaintFrameId);
            if (id <= 0)
                return;
            int idx = IndexOf(id);
            if (_ring[idx].FrameId != id)
                return;
            _ring[idx].LockAttemptTicks = attemptTicks;
            _ring[idx].LockResultTicks = resultTicks;
            _ring[idx].LockAcquired = acquired;
        }

        public static void StampDrawEnd(long frameId)
        {
            if (frameId <= 0 || !IsEnabled)
                return;
            int idx = IndexOf(frameId);
            if (_ring[idx].FrameId == frameId)
                _ring[idx].DrawEndTicks = Stopwatch.GetTimestamp();
        }

        /* After the canvas flush; the content counters are the ones the paint drew */
        public static void EndPaint(long frameId, long paintedMainCounter, long paintedGeneralCounter, long paintedMapGeneration)
        {
            Interlocked.Exchange(ref _currentPaintFrameId, 0);
            if (frameId <= 0 || !IsEnabled)
                return;
            int idx = IndexOf(frameId);
            if (_ring[idx].FrameId != frameId)
                return;
            _ring[idx].FlushEndTicks = Stopwatch.GetTimestamp();
            _ring[idx].PaintedMainCounter = paintedMainCounter;
            _ring[idx].PaintedGeneralCounter = paintedGeneralCounter;
            _ring[idx].PaintedMapGeneration = paintedMapGeneration;
            long previousGeneration = Interlocked.Exchange(ref _lastPaintedMapGeneration, paintedMapGeneration);
            if (previousGeneration >= 0 && paintedMapGeneration != previousGeneration)
                _ring[idx].ContentEvents |= GHContentEvent.MapUpdate;
            if (_ring[idx].Paint == GHPaintOutcome.Painted)
                GHCadenceMonitor.OnPaintCompleted(_ring[idx].VsyncTicks, _ring[idx].RefreshPeriodTicks,
                    _ring[idx].FlushEndTicks, _ring[idx].TargetFps, paintedMainCounter);
        }

        /* ---- Presentation ---- */

        public static void SetDisplayed(long frameId, long displayedTicks, GHPresentSource source)
        {
            if (frameId <= 0 || !IsEnabled)
                return;
            int idx = IndexOf(frameId);
            if (_ring[idx].FrameId != frameId)
                return;
            _ring[idx].DisplayedAtTicks = displayedTicks;
            _ring[idx].PresentSource = source;
        }

        public static long CompositorFrameCount { get { return Interlocked.Read(ref _compositorCount); } }

        public static void AddCompositorFrame(ref GHCompositorFrame frame)
        {
            if (!IsEnabled)
                return;
            GHCompositorFrame[] ring = _compositorRing;
            if (ring == null)
                return;
            long n = Interlocked.Increment(ref _compositorCount);
            ring[(int)((n - 1) % Capacity)] = frame;
        }

        /* Copies the retained compositor frames, oldest first, and returns the count */
        public static int CopyCompositorFrames(GHCompositorFrame[] destination)
        {
            GHCompositorFrame[] ring = _compositorRing;
            if (destination == null || ring == null)
                return 0;
            long total = Interlocked.Read(ref _compositorCount);
            if (total <= 0)
                return 0;
            long first = Math.Max(0, total - Capacity);
            int n = 0;
            for (long k = first; k < total && n < destination.Length; k++)
                destination[n++] = ring[(int)(k % Capacity)];
            return n;
        }

        /* ---- Reading ---- */

        /* Copies the retained records, oldest first, into destination (which should hold
           Capacity entries) and returns the count copied. */
        public static int CopyRecords(GHFrameRecord[] destination)
        {
            if (destination == null || _ring == null)
                return 0;
            long last = Interlocked.Read(ref _lastFrameId);
            if (last <= 0)
                return 0;
            long first = Math.Max(1, last - Capacity + 1);
            int n = 0;
            for (long id = first; id <= last && n < destination.Length; id++)
            {
                GHFrameRecord r = _ring[IndexOf(id)];
                if (r.FrameId != id)
                    continue;
                destination[n++] = r;
            }
            return n;
        }

        private static string Ms(long ticks, long origin)
        {
            if (ticks == 0)
                return "";
            double ms = (ticks - origin) * 1000.0 / Stopwatch.Frequency;
            return ms.ToString("0.000", CultureInfo.InvariantCulture);
        }

        private static string Ms(long ticks)
        {
            if (ticks == 0)
                return "";
            double ms = ticks * 1000.0 / Stopwatch.Frequency;
            return ms.ToString("0.000", CultureInfo.InvariantCulture);
        }

        /* One row per tick. Times are milliseconds since the first retained tick's callback
           start; PlatformFrame keeps the platform's epoch and is relative to its own first
           value. Lines starting with '#' carry metadata. */
        public static void DumpToCsv(string path)
        {
            GHFrameRecord[] records = new GHFrameRecord[Capacity];
            int n = CopyRecords(records);

            using (StreamWriter w = new StreamWriter(path))
            {
                w.WriteLine("# GHFrameTimeline v1");
                w.WriteLine("# StopwatchFrequency=" + Stopwatch.Frequency.ToString(CultureInfo.InvariantCulture));
                /* The absolute Stopwatch tick every relative time is measured from; on Windows
                   it is a QPC value, which lets external QPC-stamped captures be joined */
                if (n > 0)
                    w.WriteLine("# OriginStopwatchTicks=" + records[0].CallbackStartTicks.ToString(CultureInfo.InvariantCulture));
                if (HasClockAnchor)
                {
                    w.WriteLine("# FirstAnchor platformNanos=" + _firstAnchor.PlatformNanos.ToString(CultureInfo.InvariantCulture)
                        + " stopwatchTicks=" + _firstAnchor.StopwatchTicks.ToString(CultureInfo.InvariantCulture));
                    w.WriteLine("# LatestAnchor platformNanos=" + _anchor.PlatformNanos.ToString(CultureInfo.InvariantCulture)
                        + " stopwatchTicks=" + _anchor.StopwatchTicks.ToString(CultureInfo.InvariantCulture));
                }
                w.WriteLine("# OrphanPaints=" + OrphanPaintCount.ToString(CultureInfo.InvariantCulture)
                    + " Coalesced=" + CoalescedCount.ToString(CultureInfo.InvariantCulture));
                w.WriteLine("FrameId,VsyncMs,ExpectedPresentMs,PlatformFrameMs,RefreshPeriodMs,CallbackStartMs,CallbackEndMs,"
                    + "TargetFps,AssumedRefreshHz,Pacing,MainCounter,GeneralCounter,Invalidate,InvalidateMs,"
                    + "Paint,PaintOnUiThread,PaintStartMs,LockAttemptMs,LockResultMs,LockAcquired,DrawEndMs,FlushEndMs,"
                    + "PaintedMainCounter,PaintedGeneralCounter,PaintedMapGeneration,DisplayedAtMs,PresentSource,Flags,Gc0,Gc1,Gc2,"
                    + "RequestMs,ContentEvents");
                if (n == 0)
                    return;

                long origin = records[0].CallbackStartTicks;
                long platformOrigin = 0;
                for (int i = 0; i < n && platformOrigin == 0; i++)
                    platformOrigin = records[i].PlatformFrameTicks;

                for (int i = 0; i < n; i++)
                {
                    GHFrameRecord r = records[i];
                    w.WriteLine(string.Join(",", new string[]
                    {
                        r.FrameId.ToString(CultureInfo.InvariantCulture),
                        Ms(r.VsyncTicks, origin),
                        Ms(r.ExpectedPresentTicks, origin),
                        r.PlatformFrameTicks == 0 ? "" : Ms(r.PlatformFrameTicks - platformOrigin),
                        Ms(r.RefreshPeriodTicks),
                        Ms(r.CallbackStartTicks, origin),
                        Ms(r.CallbackEndTicks, origin),
                        r.TargetFps.ToString(CultureInfo.InvariantCulture),
                        r.AssumedRefreshHz.ToString(CultureInfo.InvariantCulture),
                        r.Pacing.ToString(),
                        r.MainCounter.ToString(CultureInfo.InvariantCulture),
                        r.GeneralCounter.ToString(CultureInfo.InvariantCulture),
                        r.Invalidate.ToString(),
                        Ms(r.InvalidateTicks, origin),
                        r.Paint.ToString(),
                        r.PaintOnUiThread ? "1" : "0",
                        Ms(r.PaintStartTicks, origin),
                        Ms(r.LockAttemptTicks, origin),
                        Ms(r.LockResultTicks, origin),
                        r.LockAcquired ? "1" : "0",
                        Ms(r.DrawEndTicks, origin),
                        Ms(r.FlushEndTicks, origin),
                        r.PaintedMainCounter.ToString(CultureInfo.InvariantCulture),
                        r.PaintedGeneralCounter.ToString(CultureInfo.InvariantCulture),
                        r.PaintedMapGeneration.ToString(CultureInfo.InvariantCulture),
                        Ms(r.DisplayedAtTicks, origin),
                        r.PresentSource.ToString(),
                        ((int)r.Flags).ToString(CultureInfo.InvariantCulture),
                        r.GcCount0.ToString(CultureInfo.InvariantCulture),
                        r.GcCount1.ToString(CultureInfo.InvariantCulture),
                        r.GcCount2.ToString(CultureInfo.InvariantCulture),
                        r.RequestTicks == 0 ? "" : Ms(r.RequestTicks),
                        ((int)r.ContentEvents).ToString(CultureInfo.InvariantCulture)
                    }));
                }
            }
        }

        /* One row per compositor frame, on the same time origin as DumpToCsv so the two
           files line up. */
        public static void DumpCompositorFramesToCsv(string path)
        {
            GHFrameRecord[] records = new GHFrameRecord[Capacity];
            int nRecords = CopyRecords(records);
            long origin = nRecords > 0 ? records[0].CallbackStartTicks : 0;

            GHCompositorFrame[] frames = new GHCompositorFrame[Capacity];
            int n = CopyCompositorFrames(frames);

            using (StreamWriter w = new StreamWriter(path))
            {
                w.WriteLine("# GHCompositorFrames v1");
                w.WriteLine("# StopwatchFrequency=" + Stopwatch.Frequency.ToString(CultureInfo.InvariantCulture));
                if (nRecords > 0)
                    w.WriteLine("# OriginStopwatchTicks=" + origin.ToString(CultureInfo.InvariantCulture));
                w.WriteLine("Source,IntendedVsyncMs,VsyncMs,SyncStartMs,CompletedMs,GpuDurationMs,DeadlineRaw,"
                    + "RefreshPeriodMs,ComposeMs,RefreshCount,ComposedFrameCount,DroppedSinceLast,FirstDrawFrame");
                for (int i = 0; i < n; i++)
                {
                    GHCompositorFrame f = frames[i];
                    w.WriteLine(string.Join(",", new string[]
                    {
                        f.Source.ToString(),
                        Ms(f.IntendedVsyncTicks, origin),
                        Ms(f.VsyncTicks, origin),
                        Ms(f.SyncStartTicks, origin),
                        Ms(f.CompletedTicks, origin),
                        Ms(f.GpuDurationTicks),
                        f.DeadlineRaw.ToString(CultureInfo.InvariantCulture),
                        Ms(f.RefreshPeriodTicks),
                        Ms(f.ComposeTicks, origin),
                        f.RefreshCount.ToString(CultureInfo.InvariantCulture),
                        f.ComposedFrameCount.ToString(CultureInfo.InvariantCulture),
                        f.DroppedSinceLast.ToString(CultureInfo.InvariantCulture),
                        f.FirstDrawFrame ? "1" : "0"
                    }));
                }
            }
        }
    }
}

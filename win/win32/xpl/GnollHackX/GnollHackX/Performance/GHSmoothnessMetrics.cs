using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace GnollHackX.Performance
{
    /* The stage a late or uneven frame is charged to: the first one, in pipeline order,
       that exceeded its budget */
    public enum GHHitchCause : byte
    {
        None = 0,
        DisplayMode = 1,        /* the measured refresh period changed */
        UiThreadLateGc = 2,     /* a display callback was missed or late, with a collection in the gap */
        UiThreadLate = 3,       /* a display callback was missed or late */
        PacingPolicy = 4,       /* the render loop's skip pattern */
        PaintNotRun = 5,        /* a rendered tick produced no paint */
        DispatchLate = 6,       /* the paint started long after its invalidation */
        GameLock = 7,
        PaintCpu = 8,
        Gpu = 9,
        Compositor = 10,        /* the app was on time; the frame was dropped or shown late downstream */
        Unattributed = 11
    }

    /* One distinct frame that reached the display */
    public struct GHDisplayedFrame
    {
        public long FrameId;
        public int RecordIndex;
        public long DisplayedAtTicks;
        public long GapTicks;           /* since the previous displayed frame; 0 for the first */
        public long ContentStep;        /* main counter advance since the previous displayed frame */
        public long PacingErrorTicks;   /* GapTicks - ContentStep * target period */
        public long TargetPeriodTicks;
        public long DisplayDelayTicks;  /* measured display time minus the vsync the frame was ready for; 0 when estimated */
        public GHPresentSource Source;
        public GHHitchCause Cause;
        public bool IsHitch;            /* the previous frame stayed on screen more than R/2 beyond T */
        public bool IsJudder;           /* abs(PacingErrorTicks) > R/4 */
        public bool IsPausedGap;        /* the gap spans a menu, overlay, suspension or resize */
    }

    public sealed class GHSmoothnessSummary
    {
        public int TickCount;
        public int PaintedCount;
        public int DisplayedCount;
        public int DroppedCount;            /* painted, overwritten before it could be shown */
        public int CoalescedCount;
        public int NotRunCount;             /* rendered ticks whose paint returned early */
        public int PausedGapCount;
        public double WindowMs;             /* first to last displayed frame, pauses excluded */
        public double MeasuredRefreshHz;
        public double AssumedRefreshHz;
        public double TargetFps;
        public bool AssumedRefreshMismatch; /* the pacing logic assumed a rate more than 5 % off */
        public double DisplayedFps;
        public double TileAnimationFps;
        public int HitchCount;
        public double HitchRatioMsPerSec;
        /* Frames whose motion is off by more than a quarter refresh. A target rate that does
           not divide the refresh rate makes this unavoidable: 40 FPS on a 60 Hz panel
           alternates one and two refreshes, an error of exactly R/2 on every frame. */
        public int JudderCount;
        public double JudderPct;
        public double PacingErrorRmsMs;
        public double PacingErrorP99Ms;     /* of the absolute error */
        public double RepeatedRefreshesPerSec;
        public double LatencyP50Ms;
        public double LatencyP99Ms;
        public double CallbackLatenessP99Ms;
        public double PaintP50Ms;
        public double PaintP99Ms;
        public int GcCount;
        public GHPresentSource PresentSource;    /* Measured only when every displayed frame was measured */
        public GHPerformanceStatistics.PacingMetrics OnScreenPacing;
        public readonly int[] CauseCount = new int[GHSmoothnessMetrics.CauseCount];
        public readonly double[] CauseMs = new double[GHSmoothnessMetrics.CauseCount];
        public double UnattributedShare;    /* of hitch time; a quality measure of the instrument */
    }

    /* Reconstructs what reached the display from the frame timeline and computes the
       perceived-smoothness metrics and hitch attribution. Shared with the offline analyzer,
       so a device and a desktop classify the same data identically. Allocates; call it at
       the end of a window, never per frame. Must compile under C# 7.3.

       Displayed time: a frame is shown at the first vsync strictly after it was ready.
       Ready is the flush end, or on Android the RenderThread's completion of the frame
       that carried it: the same doFrame for a UI-thread paint, the first frame whose sync
       began after the flush for a GL-thread paint (sync is when a TextureView latches its
       newest frame). The constant pipeline latency after that vsync is not modelled; it
       cancels out of every gap. When two frames land on the same vsync only the later is
       shown and the earlier counts as dropped. A frame that already carries a measured
       display time keeps it. */
    public static class GHSmoothnessMetrics
    {
        public const int CauseCount = 12;

        private static readonly string[] _causeNames = new string[]
        {
            "None", "DisplayMode", "UiThreadLateGc", "UiThreadLate", "PacingPolicy", "PaintNotRun",
            "DispatchLate", "GameLock", "PaintCpu", "Gpu", "Compositor", "Unattributed"
        };

        public static string CauseName(GHHitchCause cause)
        {
            int i = (int)cause;
            return i >= 0 && i < _causeNames.Length ? _causeNames[i] : "Unknown";
        }

        private static double TicksToMs(double ticks)
        {
            return ticks * 1000.0 / Stopwatch.Frequency;
        }

        private static long MedianPeriod(GHFrameRecord[] records, int n)
        {
            List<long> periods = new List<long>();
            for (int i = 0; i < n; i++)
            {
                if (records[i].RefreshPeriodTicks > 0)
                    periods.Add(records[i].RefreshPeriodTicks);
            }
            if (periods.Count == 0)
                return Stopwatch.Frequency / 60;
            periods.Sort();
            return periods[periods.Count / 2];
        }

        private static int ModeShort(GHFrameRecord[] records, int n, bool target)
        {
            Dictionary<int, int> counts = new Dictionary<int, int>();
            int best = 0, bestCount = 0;
            for (int i = 0; i < n; i++)
            {
                int v = target ? records[i].TargetFps : records[i].AssumedRefreshHz;
                if (v <= 0)
                    continue;
                int c;
                counts.TryGetValue(v, out c);
                c++;
                counts[v] = c;
                if (c > bestCount)
                {
                    bestCount = c;
                    best = v;
                }
            }
            return best;
        }

        private static long TargetPeriodTicks(int targetFps, long fallback)
        {
            return targetFps > 0 ? Stopwatch.Frequency / targetFps : fallback;
        }

        /* The vsync grid: every tick's vsync time, or its callback start when the platform
           gave none, with the refresh period in effect at that tick */
        private sealed class VsyncGrid
        {
            private readonly long[] _times;
            private readonly long[] _periods;
            private readonly int _count;

            public VsyncGrid(GHFrameRecord[] records, int n, long fallbackPeriod)
            {
                bool haveVsync = false;
                for (int i = 0; i < n && !haveVsync; i++)
                    haveVsync = records[i].VsyncTicks != 0;

                _times = new long[n];
                _periods = new long[n];
                int k = 0;
                long last = long.MinValue;
                for (int i = 0; i < n; i++)
                {
                    long t = haveVsync ? records[i].VsyncTicks : records[i].CallbackStartTicks;
                    if (t == 0 || t <= last)
                        continue;
                    _times[k] = t;
                    _periods[k] = records[i].RefreshPeriodTicks > 0 ? records[i].RefreshPeriodTicks : fallbackPeriod;
                    last = t;
                    k++;
                }
                _count = k;
            }

            /* The first vsync strictly after t, extrapolating across missed callbacks */
            public long NextBoundaryAfter(long t)
            {
                if (_count == 0)
                    return t;
                int lo = 0, hi = _count - 1, idx = -1;
                while (lo <= hi)
                {
                    int mid = (lo + hi) / 2;
                    if (_times[mid] <= t)
                    {
                        idx = mid;
                        lo = mid + 1;
                    }
                    else
                    {
                        hi = mid - 1;
                    }
                }
                if (idx < 0)
                    return _times[0];
                long baseTime = _times[idx];
                long period = _periods[idx] > 0 ? _periods[idx] : 1;
                long steps = (t - baseTime) / period + 1;
                long boundary = baseTime + steps * period;
                /* A later real vsync is more accurate than the extrapolation */
                if (idx + 1 < _count && _times[idx + 1] <= boundary)
                    return _times[idx + 1];
                return boundary;
            }
        }

        private struct Candidate
        {
            public int RecordIndex;
            public long DisplayedAt;
            public long ReadyAt;
            public long DisplayDelay;
            public GHPresentSource Source;
        }

        private static int FindCompositorBySyncAfter(GHCompositorFrame[] comp, int m, long t)
        {
            /* Compositor frames arrive in frame order, so sync starts ascend */
            int lo = 0, hi = m - 1, found = -1;
            while (lo <= hi)
            {
                int mid = (lo + hi) / 2;
                if (comp[mid].SyncStartTicks >= t)
                {
                    found = mid;
                    hi = mid - 1;
                }
                else
                {
                    lo = mid + 1;
                }
            }
            return found;
        }

        private static int FindCompositorByVsync(GHCompositorFrame[] comp, int m, long vsync, long tolerance)
        {
            int lo = 0, hi = m - 1;
            while (lo <= hi)
            {
                int mid = (lo + hi) / 2;
                long d = comp[mid].IntendedVsyncTicks - vsync;
                if (Math.Abs(d) <= tolerance)
                    return mid;
                if (d < 0)
                    lo = mid + 1;
                else
                    hi = mid - 1;
            }
            return -1;
        }

        private static bool IsPauseTick(ref GHFrameRecord r)
        {
            switch (r.Pacing)
            {
            case GHPacingDecision.AuxiliaryCanvas:
            case GHPacingDecision.Suspended:
            case GHPacingDecision.PlatformLoopOff:
            case GHPacingDecision.NoGamePage:
            case GHPacingDecision.NoGame:
            case GHPacingDecision.NoResolution:
                return true;
            }
            switch (r.Invalidate)
            {
            case GHInvalidateOutcome.Resizing:
            case GHInvalidateOutcome.RefreshOff:
            case GHInvalidateOutcome.NotVisible:
                return true;
            }
            return r.Paint == GHPaintOutcome.OverlayVisible || r.Paint == GHPaintOutcome.MainCanvasOff;
        }

        private static bool IsRenderedPacing(GHPacingDecision p)
        {
            return p == GHPacingDecision.Rendered || p == GHPacingDecision.RenderedCatchUp;
        }

        /* Fills displayed (which should hold n entries) and returns the summary.
           compositor may be null. records must be in FrameId order, as CopyRecords
           returns them. */
        public static GHSmoothnessSummary Analyze(GHFrameRecord[] records, int n, GHCompositorFrame[] compositor, int m,
                                                  GHDisplayedFrame[] displayed, out int displayedCount)
        {
            GHSmoothnessSummary s = new GHSmoothnessSummary();
            displayedCount = 0;
            s.TickCount = n;
            if (records == null || n <= 0)
                return s;
            if (compositor == null)
                m = 0;

            long period = MedianPeriod(records, n);
            s.MeasuredRefreshHz = period > 0 ? (double)Stopwatch.Frequency / period : 0;
            int targetMode = ModeShort(records, n, true);
            int assumedMode = ModeShort(records, n, false);
            s.TargetFps = targetMode;
            s.AssumedRefreshHz = assumedMode;
            s.AssumedRefreshMismatch = assumedMode > 0 && s.MeasuredRefreshHz > 0
                && Math.Abs(assumedMode - s.MeasuredRefreshHz) > 0.05 * s.MeasuredRefreshHz;
            long targetPeriodMode = TargetPeriodTicks(targetMode, period);

            bool androidCompositor = false;
            for (int i = 0; i < m && !androidCompositor; i++)
                androidCompositor = compositor[i].Source == GHCompositorSource.AndroidFrameMetrics;

            VsyncGrid grid = new VsyncGrid(records, n, period);

            /* 1. Display time of every painted frame */
            List<Candidate> candidates = new List<Candidate>();
            List<double> paintMs = new List<double>();
            List<double> callbackLateMs = new List<double>();
            for (int i = 0; i < n; i++)
            {
                GHFrameRecord r = records[i];
                if (r.Paint == GHPaintOutcome.Coalesced)
                    s.CoalescedCount++;
                else if (IsRenderedPacing(r.Pacing) && r.Invalidate == GHInvalidateOutcome.Invalidated
                         && r.Paint != GHPaintOutcome.Painted && r.Paint != GHPaintOutcome.None)
                    s.NotRunCount++;

                if (r.VsyncTicks != 0 && r.CallbackStartTicks > r.VsyncTicks)
                    callbackLateMs.Add(TicksToMs(r.CallbackStartTicks - r.VsyncTicks));

                if (r.Paint != GHPaintOutcome.Painted || r.FlushEndTicks == 0)
                    continue;
                s.PaintedCount++;
                if (r.PaintStartTicks != 0)
                    paintMs.Add(TicksToMs(r.FlushEndTicks - r.PaintStartTicks));

                Candidate c = new Candidate();
                c.RecordIndex = i;
                c.ReadyAt = r.FlushEndTicks;
                if (r.PresentSource == GHPresentSource.Measured && r.DisplayedAtTicks != 0)
                {
                    c.DisplayedAt = r.DisplayedAtTicks;
                    c.Source = GHPresentSource.Measured;
                    long expected = grid.NextBoundaryAfter(r.FlushEndTicks);
                    c.DisplayDelay = r.DisplayedAtTicks > expected ? r.DisplayedAtTicks - expected : 0;
                }
                else
                {
                    long ready = r.FlushEndTicks;
                    if (androidCompositor)
                    {
                        long recPeriod = r.RefreshPeriodTicks > 0 ? r.RefreshPeriodTicks : period;
                        int ci = r.PaintOnUiThread
                            ? FindCompositorByVsync(compositor, m, r.VsyncTicks, recPeriod / 4)
                            : FindCompositorBySyncAfter(compositor, m, r.FlushEndTicks);
                        if (ci >= 0 && compositor[ci].CompletedTicks > ready)
                            ready = compositor[ci].CompletedTicks;
                    }
                    c.ReadyAt = ready;
                    c.DisplayedAt = grid.NextBoundaryAfter(ready);
                    c.Source = GHPresentSource.Estimated;
                }
                candidates.Add(c);
            }

            candidates.Sort(delegate (Candidate a, Candidate b)
            {
                int cmp = a.DisplayedAt.CompareTo(b.DisplayedAt);
                return cmp != 0 ? cmp : a.ReadyAt.CompareTo(b.ReadyAt);
            });

            /* 2. One displayed frame per vsync: the last one ready wins */
            bool[] isDisplayed = new bool[n];
            int d = 0;
            for (int k = 0; k < candidates.Count; k++)
            {
                bool last = k + 1 >= candidates.Count || candidates[k + 1].DisplayedAt != candidates[k].DisplayedAt;
                if (!last)
                {
                    s.DroppedCount++;
                    continue;
                }
                Candidate c = candidates[k];
                GHDisplayedFrame f = new GHDisplayedFrame();
                f.FrameId = records[c.RecordIndex].FrameId;
                f.RecordIndex = c.RecordIndex;
                f.DisplayedAtTicks = c.DisplayedAt;
                f.DisplayDelayTicks = c.DisplayDelay;
                f.Source = c.Source;
                f.TargetPeriodTicks = TargetPeriodTicks(records[c.RecordIndex].TargetFps, targetPeriodMode);
                if (displayed != null && d < displayed.Length)
                    displayed[d] = f;
                isDisplayed[c.RecordIndex] = true;
                d++;
            }
            if (displayed == null)
                return s;
            if (d > displayed.Length)
                d = displayed.Length;
            displayedCount = d;
            s.DisplayedCount = d;

            /* 3. Gaps, content steps, hitches, judder */
            bool allMeasured = d > 0;
            List<float> gapsMs = new List<float>();
            List<double> absErrMs = new List<double>();
            List<double> latencyMs = new List<double>();
            double errSq = 0;
            double hitchMs = 0;
            double activeMs = 0;
            double repeats = 0;
            HashSet<long> tileStates = new HashSet<long>();

            for (int j = 0; j < d; j++)
            {
                GHFrameRecord r = records[displayed[j].RecordIndex];
                if (displayed[j].Source != GHPresentSource.Measured)
                    allMeasured = false;
                tileStates.Add(r.PaintedGeneralCounter);
                long producedAt = r.VsyncTicks != 0 ? r.VsyncTicks : r.CallbackStartTicks;
                if (producedAt != 0 && displayed[j].DisplayedAtTicks > producedAt)
                    latencyMs.Add(TicksToMs(displayed[j].DisplayedAtTicks - producedAt));

                if (j == 0)
                    continue;

                int prevIdx = displayed[j - 1].RecordIndex;
                int curIdx = displayed[j].RecordIndex;
                long gap = displayed[j].DisplayedAtTicks - displayed[j - 1].DisplayedAtTicks;
                long prevCounter = records[prevIdx].PaintedMainCounter != 0 ? records[prevIdx].PaintedMainCounter : records[prevIdx].MainCounter;
                long curCounter = r.PaintedMainCounter != 0 ? r.PaintedMainCounter : r.MainCounter;
                long step = curCounter - prevCounter;
                if (step < 0)
                    step = 0;
                long target = displayed[j].TargetPeriodTicks;
                long refresh = r.RefreshPeriodTicks > 0 ? r.RefreshPeriodTicks : period;

                displayed[j].GapTicks = gap;
                displayed[j].ContentStep = step;
                displayed[j].PacingErrorTicks = gap - step * target;

                bool paused = false;
                for (int i = prevIdx + 1; i <= curIdx && !paused; i++)
                    paused = IsPauseTick(ref records[i]);
                displayed[j].IsPausedGap = paused;
                if (paused)
                {
                    s.PausedGapCount++;
                    continue;
                }

                double gapMs = TicksToMs(gap);
                activeMs += gapMs;
                gapsMs.Add((float)gapMs);

                double errMs = TicksToMs(displayed[j].PacingErrorTicks);
                errSq += errMs * errMs;
                absErrMs.Add(Math.Abs(errMs));

                long holdRefreshes = (long)Math.Round((double)gap / refresh);
                long intendedRefreshes = (long)Math.Round((double)target / refresh);
                if (holdRefreshes > intendedRefreshes)
                    repeats += holdRefreshes - intendedRefreshes;

                displayed[j].IsHitch = gap > target + refresh / 2;
                displayed[j].IsJudder = Math.Abs(displayed[j].PacingErrorTicks) > refresh / 4;
                if (displayed[j].IsHitch)
                {
                    s.HitchCount++;
                    hitchMs += TicksToMs(gap - target);
                }
                if (displayed[j].IsJudder)
                    s.JudderCount++;

                if (displayed[j].IsHitch || displayed[j].IsJudder)
                {
                    GHHitchCause cause = Attribute(records, prevIdx, curIdx, compositor, m, isDisplayed, target, refresh,
                                                   gap, displayed[j].DisplayDelayTicks);
                    displayed[j].Cause = cause;
                    s.CauseCount[(int)cause]++;
                    if (displayed[j].IsHitch)
                        s.CauseMs[(int)cause] += TicksToMs(gap - target);
                }
            }

            int judged = absErrMs.Count;
            s.WindowMs = activeMs;
            double seconds = activeMs / 1000.0;
            s.DisplayedFps = seconds > 0 ? gapsMs.Count / seconds : 0;
            s.TileAnimationFps = seconds > 0 ? Math.Max(0, tileStates.Count - 1) / seconds : 0;
            s.HitchRatioMsPerSec = seconds > 0 ? hitchMs / seconds : 0;
            s.JudderPct = judged > 0 ? 100.0 * s.JudderCount / judged : 0;
            s.PacingErrorRmsMs = judged > 0 ? Math.Sqrt(errSq / judged) : 0;
            s.PacingErrorP99Ms = Percentile(absErrMs, 99);
            s.RepeatedRefreshesPerSec = seconds > 0 ? repeats / seconds : 0;
            s.LatencyP50Ms = Percentile(latencyMs, 50);
            s.LatencyP99Ms = Percentile(latencyMs, 99);
            s.CallbackLatenessP99Ms = Percentile(callbackLateMs, 99);
            s.PaintP50Ms = Percentile(paintMs, 50);
            s.PaintP99Ms = Percentile(paintMs, 99);
            s.GcCount = (records[n - 1].GcCount0 - records[0].GcCount0);
            s.PresentSource = d == 0 ? GHPresentSource.None : (allMeasured ? GHPresentSource.Measured : GHPresentSource.Estimated);
            s.OnScreenPacing = GHPerformanceStatistics.ComputePacing(gapsMs.ToArray(), TicksToMs(targetPeriodMode), TicksToMs(period));
            s.UnattributedShare = hitchMs > 0 ? s.CauseMs[(int)GHHitchCause.Unattributed] / hitchMs : 0;
            return s;
        }

        private static double Percentile(List<double> values, double p)
        {
            if (values.Count == 0)
                return 0;
            float[] a = new float[values.Count];
            for (int i = 0; i < a.Length; i++)
                a[i] = (float)values[i];
            return GHPerformanceStatistics.Percentile(a, p);
        }

        /* Charges the gap before a late or uneven frame to the first stage, in pipeline
           order, that exceeded its budget. prevIdx and curIdx are the records of the
           displayed frames on either side of the gap. */
        public static GHHitchCause Attribute(GHFrameRecord[] records, int prevIdx, int curIdx,
                                             GHCompositorFrame[] compositor, int m, bool[] isDisplayed,
                                             long targetPeriod, long refreshPeriod, long gapTicks, long displayDelayTicks)
        {
            long halfRefresh = refreshPeriod / 2;

            /* 1. Display mode: the measured period moved by more than 5 % across the gap, or
               the pacing logic is dividing a refresh rate the panel is not running at */
            long p0 = records[prevIdx].RefreshPeriodTicks;
            for (int i = prevIdx + 1; i <= curIdx; i++)
            {
                long p = records[i].RefreshPeriodTicks;
                if (p0 > 0 && p > 0 && Math.Abs(p - p0) > p0 / 20)
                    return GHHitchCause.DisplayMode;
            }
            int assumedHz = records[curIdx].AssumedRefreshHz;
            if (assumedHz > 0 && refreshPeriod > 0)
            {
                double measuredHz = (double)Stopwatch.Frequency / refreshPeriod;
                if (Math.Abs(assumedHz - measuredHz) > 0.05 * measuredHz)
                    return GHHitchCause.DisplayMode;
            }

            /* 2. UI thread: a missed callback or a callback well after its vsync. When the
               UI thread was busy painting the map across that vsync, the paint is the
               cause, not the thread. */
            bool uiLate = false, gc = false;
            GHHitchCause ownPaint = GHHitchCause.None;
            for (int i = prevIdx + 1; i <= curIdx; i++)
            {
                GHFrameRecord r = records[i];
                GHFrameRecord q = records[i - 1];
                long cur = r.VsyncTicks != 0 ? r.VsyncTicks : r.CallbackStartTicks;
                long prev = q.VsyncTicks != 0 ? q.VsyncTicks : q.CallbackStartTicks;
                bool late = (cur != 0 && prev != 0 && cur - prev > refreshPeriod + halfRefresh)
                    || (r.VsyncTicks != 0 && r.CallbackStartTicks - r.VsyncTicks > halfRefresh);
                if (r.GcCount0 != q.GcCount0 || r.GcCount1 != q.GcCount1 || r.GcCount2 != q.GcCount2)
                    gc = true;
                if (!late)
                    continue;
                uiLate = true;
                long deadline = (cur != 0 ? cur : r.CallbackStartTicks) + halfRefresh;
                long missedVsync = prev != 0 ? prev + refreshPeriod : deadline - halfRefresh;
                for (int k = prevIdx; k < i && ownPaint == GHHitchCause.None; k++)
                {
                    GHFrameRecord pr = records[k];
                    if (!pr.PaintOnUiThread || pr.PaintStartTicks == 0 || pr.FlushEndTicks == 0)
                        continue;
                    if (pr.PaintStartTicks < deadline && pr.FlushEndTicks > missedVsync)
                    {
                        long draw = pr.DrawEndTicks != 0 ? pr.DrawEndTicks - pr.PaintStartTicks : 0;
                        long flush = pr.DrawEndTicks != 0 ? pr.FlushEndTicks - pr.DrawEndTicks : 0;
                        ownPaint = flush > draw ? GHHitchCause.Gpu : GHHitchCause.PaintCpu;
                    }
                }
            }
            if (ownPaint != GHHitchCause.None)
                return ownPaint;
            if (uiLate)
                return gc ? GHHitchCause.UiThreadLateGc : GHHitchCause.UiThreadLate;

            /* 3. Pacing policy: the loop's own irregular skip or catch-up render in the gap,
               or a ratio of refresh to target rate the divisor pattern cannot pace evenly.
               Such a pattern holds frames for at most two divisor steps; a longer gap has
               another cause. */
            for (int i = prevIdx + 1; i <= curIdx; i++)
            {
                GHPacingDecision p = records[i].Pacing;
                if (p == GHPacingDecision.SkippedModulo || p == GHPacingDecision.RenderedCatchUp)
                    return GHHitchCause.PacingPolicy;
            }
            int targetFps = records[curIdx].TargetFps;
            if (assumedHz > targetFps && targetFps > 0 && assumedHz % targetFps != 0)
            {
                long divisor = Math.Max(1, assumedHz / targetFps);
                if (gapTicks <= 2 * divisor * refreshPeriod + refreshPeriod / 4)
                    return GHHitchCause.PacingPolicy;
            }

            /* 4. A rendered tick in the gap that produced no paint */
            for (int i = prevIdx + 1; i < curIdx; i++)
            {
                GHFrameRecord r = records[i];
                if (!IsRenderedPacing(r.Pacing))
                    continue;
                if (r.Invalidate != GHInvalidateOutcome.Invalidated)
                    return GHHitchCause.PaintNotRun;
                if (r.Paint != GHPaintOutcome.Painted && r.Paint != GHPaintOutcome.None)
                    return GHHitchCause.PaintNotRun;
            }

            /* 5-8. The stages of every frame painted in the gap, oldest first: a frame that
               ran late can miss its vsync and be overwritten by the next one */
            for (int i = prevIdx + 1; i <= curIdx; i++)
            {
                GHFrameRecord r = records[i];
                if (r.Paint != GHPaintOutcome.Painted)
                    continue;
                if (r.InvalidateTicks != 0 && r.PaintStartTicks - r.InvalidateTicks > halfRefresh)
                    return GHHitchCause.DispatchLate;
                if (r.LockAttemptTicks != 0 && r.LockResultTicks - r.LockAttemptTicks > targetPeriod / 4)
                    return GHHitchCause.GameLock;
                if (r.PaintStartTicks != 0 && r.DrawEndTicks != 0 && r.DrawEndTicks - r.PaintStartTicks > targetPeriod * 3 / 4)
                    return GHHitchCause.PaintCpu;
                if (r.DrawEndTicks != 0 && r.FlushEndTicks - r.DrawEndTicks > targetPeriod / 2)
                    return GHHitchCause.Gpu;
            }
            GHFrameRecord cr = records[curIdx];

            /* 9. Downstream: the frame was measured on screen later than the vsync it was
               ready for, a painted frame in the gap was never shown, or the compositor ran
               long on a frame in the gap */
            if (displayDelayTicks > halfRefresh)
                return GHHitchCause.Compositor;
            for (int i = prevIdx + 1; i < curIdx; i++)
            {
                if (records[i].Paint == GHPaintOutcome.Painted && !isDisplayed[i])
                    return GHHitchCause.Compositor;
            }
            if (compositor != null && m > 0)
            {
                long from = records[prevIdx].VsyncTicks != 0 ? records[prevIdx].VsyncTicks : records[prevIdx].CallbackStartTicks;
                long to = cr.FlushEndTicks != 0 ? cr.FlushEndTicks : cr.CallbackStartTicks;
                for (int k = 0; k < m; k++)
                {
                    GHCompositorFrame f = compositor[k];
                    if (f.IntendedVsyncTicks < from || f.IntendedVsyncTicks > to)
                        continue;
                    if (f.GpuDurationTicks > refreshPeriod)
                        return GHHitchCause.Gpu;
                    if (f.DroppedSinceLast > 0)
                        return GHHitchCause.Compositor;
                    if (f.Source == GHCompositorSource.AndroidFrameMetrics && f.CompletedTicks != 0
                        && f.CompletedTicks - f.IntendedVsyncTicks > refreshPeriod + halfRefresh)
                        return GHHitchCause.Compositor;
                }
            }

            return GHHitchCause.Unattributed;
        }
    }

    /* Online estimate of what the display shows, fed from each completed paint, for the
       dashboard and for cadence-change events. It uses the same display-time rule as
       GHSmoothnessMetrics without the compositor refinement, over 250 ms buckets. A change
       is reported when the displayed rate of the last second differs from that of the three
       seconds before by more than 10 % for a full second, or when the measured refresh
       period moves by more than 5 % for a full second. Runs on the paint thread. */
    public static class GHCadenceMonitor
    {
        private const int Buckets = 16;
        private const int RecentBuckets = 4;
        private const int PersistBuckets = 4;

        private static readonly double[] _fpsRing = new double[Buckets];
        private static readonly double[] _sortScratch = new double[Buckets];
        private static int _ringCount = 0;
        private static int _ringIndex = 0;

        private static long _bucketStart = 0;
        private static int _bucketFrames = 0;
        private static double _bucketHitchMs = 0;
        private static double _bucketErrSq = 0;
        private static int _bucketErrCount = 0;

        private static long _lastBoundary = 0;
        private static long _lastCounter = 0;
        private static int _changeStreak = 0;
        private static int _refreshStreak = 0;
        private static long _reportedPeriod = 0;

        private static double _displayedFps = 0;
        private static double _hitchRatio = 0;
        private static double _pacingErrorRmsMs = 0;
        private static string _lastChange = "";

        /* Receives one line per detected change, e.g. for the screen log */
        public static Action<string> ChangeLog = null;

        public static double DisplayedFps { get { return _displayedFps; } }
        public static double HitchRatioMsPerSec { get { return _hitchRatio; } }
        public static double PacingErrorRmsMs { get { return _pacingErrorRmsMs; } }
        public static string LastChange { get { return _lastChange; } }

        public static void Reset()
        {
            Array.Clear(_fpsRing, 0, Buckets);
            _ringCount = 0;
            _ringIndex = 0;
            _bucketStart = 0;
            _bucketFrames = 0;
            _bucketHitchMs = 0;
            _bucketErrSq = 0;
            _bucketErrCount = 0;
            _lastBoundary = 0;
            _lastCounter = 0;
            _changeStreak = 0;
            _refreshStreak = 0;
            _reportedPeriod = 0;
            _displayedFps = 0;
            _hitchRatio = 0;
            _pacingErrorRmsMs = 0;
            _lastChange = "";
        }

        public static void OnPaintCompleted(long vsyncTicks, long refreshPeriodTicks, long flushEndTicks,
                                            int targetFps, long paintedMainCounter)
        {
            if (flushEndTicks == 0 || refreshPeriodTicks <= 0)
                return;
            long freq = Stopwatch.Frequency;
            long baseTime = vsyncTicks != 0 && vsyncTicks <= flushEndTicks ? vsyncTicks : flushEndTicks - refreshPeriodTicks;
            long boundary = baseTime + ((flushEndTicks - baseTime) / refreshPeriodTicks + 1) * refreshPeriodTicks;
            long target = targetFps > 0 ? freq / targetFps : refreshPeriodTicks;

            if (_bucketStart == 0)
                _bucketStart = flushEndTicks;

            if (boundary > _lastBoundary)
            {
                if (_lastBoundary != 0)
                {
                    long gap = boundary - _lastBoundary;
                    /* A gap of a second or more is a pause, not a frame */
                    if (gap < freq)
                    {
                        _bucketFrames++;
                        if (gap > target + refreshPeriodTicks / 2)
                            _bucketHitchMs += (gap - target) * 1000.0 / freq;
                        long step = paintedMainCounter - _lastCounter;
                        if (step > 0 && step < 1000)
                        {
                            double errMs = (gap - step * target) * 1000.0 / freq;
                            _bucketErrSq += errMs * errMs;
                            _bucketErrCount++;
                        }
                    }
                }
                _lastBoundary = boundary;
                _lastCounter = paintedMainCounter;
            }

            long elapsed = flushEndTicks - _bucketStart;
            if (elapsed >= freq / 4)
            {
                double seconds = (double)elapsed / freq;
                PushBucket(_bucketFrames / seconds, _bucketHitchMs / seconds,
                           _bucketErrCount > 0 ? Math.Sqrt(_bucketErrSq / _bucketErrCount) : 0,
                           refreshPeriodTicks);
                _bucketStart = flushEndTicks;
                _bucketFrames = 0;
                _bucketHitchMs = 0;
                _bucketErrSq = 0;
                _bucketErrCount = 0;
            }
        }

        private static double Median(int newestCount, int skipNewest)
        {
            int n = 0;
            for (int k = skipNewest; k < skipNewest + newestCount && k < _ringCount; k++)
            {
                int idx = (_ringIndex - 1 - k + Buckets) % Buckets;
                _sortScratch[n++] = _fpsRing[idx];
            }
            if (n == 0)
                return 0;
            Array.Sort(_sortScratch, 0, n);
            return _sortScratch[n / 2];
        }

        private static void PushBucket(double fps, double hitchRatio, double errRms, long refreshPeriodTicks)
        {
            _fpsRing[_ringIndex] = fps;
            _ringIndex = (_ringIndex + 1) % Buckets;
            if (_ringCount < Buckets)
                _ringCount++;

            _displayedFps = Median(RecentBuckets, 0);
            _hitchRatio = hitchRatio;
            _pacingErrorRmsMs = errRms;

            if (_ringCount >= Buckets)
            {
                double recent = Median(RecentBuckets, 0);
                double baseline = Median(Buckets - RecentBuckets, RecentBuckets);
                if (baseline > 0 && Math.Abs(recent - baseline) > 0.10 * baseline)
                {
                    _changeStreak++;
                    if (_changeStreak >= PersistBuckets)
                    {
                        Report(GHTraceEvent.CadenceChange, (long)Math.Round(recent),
                            "CADENCE " + Math.Round(baseline).ToString(System.Globalization.CultureInfo.InvariantCulture)
                            + "->" + Math.Round(recent).ToString(System.Globalization.CultureInfo.InvariantCulture) + " fps");
                        /* The new rate becomes the baseline */
                        for (int k = 0; k < Buckets; k++)
                            _fpsRing[k] = recent;
                        _changeStreak = 0;
                    }
                }
                else
                {
                    _changeStreak = 0;
                }
            }

            if (_reportedPeriod == 0)
            {
                _reportedPeriod = refreshPeriodTicks;
            }
            else if (Math.Abs(refreshPeriodTicks - _reportedPeriod) > _reportedPeriod / 20)
            {
                _refreshStreak++;
                if (_refreshStreak >= PersistBuckets)
                {
                    double oldMs = _reportedPeriod * 1000.0 / Stopwatch.Frequency;
                    double newMs = refreshPeriodTicks * 1000.0 / Stopwatch.Frequency;
                    Report(GHTraceEvent.RefreshChange, (long)Math.Round(newMs * 1000.0),
                        "REFRESH " + oldMs.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture)
                        + "->" + newMs.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture) + " ms");
                    _reportedPeriod = refreshPeriodTicks;
                    _refreshStreak = 0;
                }
            }
            else
            {
                _refreshStreak = 0;
            }
        }

        private static void Report(GHTraceEvent traceEvent, long value, string text)
        {
            _lastChange = text;
            GHPresentFeedback.Event(traceEvent, value);
            Action<string> log = ChangeLog;
            if (log != null)
            {
                try
                {
                    log(text);
                }
                catch (Exception)
                {
                    /* Logging must never break the paint */
                }
            }
        }
    }
}

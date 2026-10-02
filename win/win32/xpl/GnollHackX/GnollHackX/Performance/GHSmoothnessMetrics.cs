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
        DisplayMode = 1,        /* the panel's refresh period changed */
        UiThreadLateGc = 2,     /* a display callback was missed or late, and GC paused long enough to explain it */
        UiThreadLate = 3,       /* a display callback was missed or late */
        PacingPolicy = 4,       /* the render loop's skip pattern */
        PaintNotRun = 5,        /* a rendered tick produced no paint */
        DispatchLate = 6,       /* the paint started long after its invalidation */
        GameLock = 7,
        PaintCpu = 8,
        Gpu = 9,
        Compositor = 10,        /* the app was on time; the frame was dropped or shown late downstream */
        Unattributed = 11,
        UiThreadRequests = 12,  /* a display callback was late after the UI thread handled game requests */
        FrameworkCadence = 13   /* the UI framework delivered display callbacks below the panel's rate */
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
        public GHContentEvent ContentEvents;    /* content that appeared during the gap */
        public bool IsHitch;            /* the previous frame stayed on screen more than R/2 beyond its longest on-time hold */
        public bool IsJudder;           /* abs(PacingErrorTicks) > R/4 */
        public bool IsPausedGap;        /* the gap spans a menu, overlay, suspension or resize, or an excluded long stall */
        public bool IsLongStall;        /* display callbacks stopped for LongStallSeconds or more in the gap, without a lifecycle break */
        public bool OverlapsForcedGc;   /* an unpaused gap overlapping the callback interval of an app-forced collection */
    }

    public sealed class GHSmoothnessSummary
    {
        public int MetricsVersion;          /* GHSmoothnessMetrics.MetricsVersion of the analysis */
        public int TickCount;
        public int PaintedCount;
        public int DisplayedCount;
        public int DroppedCount;            /* painted, overwritten before it could be shown */
        public int CoalescedCount;
        public int NotRunCount;             /* rendered ticks whose paint returned early */
        public int PausedGapCount;
        public int CompositorReportsLost;   /* FrameMetrics reports the listener missed over the records' span */
        public double CompositorCoverage = -1;  /* share of GL-thread paints a report carried; -1 when not applicable */
        public double SyncOffsetP50Ms;      /* median FrameMetrics sync offset, ms; 0 when none */
        public double VsyncCorrectedShare = -1; /* share of ticks whose stale vsync was corrected; -1 n/a */
        public int LongStallCount;          /* gaps with display callbacks stopped for LongStallSeconds or more */
        public double LongStallMs;          /* total length of those gaps */
        public bool LongStallsExcluded;     /* long stalls were paused gaps, not hitches */
        public double WindowMs;             /* first to last displayed frame, pauses excluded */
        public double MeasuredRefreshHz;
        public double CallbackRefreshHz;    /* the display callback rate; below MeasuredRefreshHz when callbacks are skipped */
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
        public int ForcedGcCount;           /* ticks after an app-forced collection */
        public int ForcedGcHitchCount;      /* of HitchCount, the hitches with OverlapsForcedGc */
        public double ForcedGcHitchMs;      /* their time beyond target, part of the hitch time */
        public double GcPauseMs;            /* total GC pause over the window */
        public bool GcPauseDataAvailable;   /* the runtime reported pause time; else GC is judged by counts */
        public GHPresentSource PresentSource;    /* Measured only when every displayed frame was measured */
        public GHPerformanceStatistics.PacingMetrics OnScreenPacing;
        public readonly int[] CauseCount = new int[GHSmoothnessMetrics.CauseCount];
        public readonly int[] CauseHitchCount = new int[GHSmoothnessMetrics.CauseCount];   /* of CauseCount, the hitches alone */
        public readonly double[] CauseMs = new double[GHSmoothnessMetrics.CauseCount];
        public double UnattributedShare;    /* of hitch time; a quality measure of the instrument */

        /* Content events against hitches, over the unpaused gaps: per event kind (indexed by
           bit position of GHContentEvent), the gaps in which it happened and how many of them
           were hitches; and the same for gaps with no event at all. A hitch rate well above
           the quiet rate ties the hitches to that kind of content. */
        public readonly int[] EventGapCount = new int[GHSmoothnessMetrics.ContentEventKinds];
        public readonly int[] EventHitchCount = new int[GHSmoothnessMetrics.ContentEventKinds];
        public int QuietGapCount;
        public int QuietHitchCount;
    }

    /* Reconstructs what reached the display from the frame timeline and computes the
       perceived-smoothness metrics and hitch attribution. Shared with the offline analyzer,
       so a device and a desktop classify the same data identically. Allocates; call it at
       the end of a window, never per frame. Must compile under C# 7.3.

       Displayed time: a frame is shown at the first vsync strictly after it was ready.
       Ready is the flush end, or on Android the RenderThread's completion of the frame
       that carried it: the same doFrame for a UI-thread paint, the first frame whose sync
       began after the flush for a GL-thread paint (sync is when a TextureView latches its
       newest frame), or the vsync of the frame expected to carry it when no report
       matches. The constant pipeline latency after that vsync is not modelled; it
       cancels out of every gap. When two frames land on the same vsync only the later is
       shown and the earlier counts as dropped. A frame that already carries a measured
       display time keeps it. The summary records the share of GL-thread paints a report
       carried, and the median sync offset of the reports. On Android, a tick vsync one
       refresh behind HWUI's frame time for the same frame is corrected to it, and the
       summary records the share of ticks corrected. */
    public static class GHSmoothnessMetrics
    {
        public const int CauseCount = 14;
        public const int ContentEventKinds = 10;

        /* The version of the metric definitions; results of different versions do not compare */
        public const int MetricsVersion = 4;

        /* A stop in the display callbacks at least this long is a long stall */
        public const double LongStallSeconds = 1.0;

        /* How many refreshes after its flush a GL-thread paint's frame may start syncing
           when no report was lost before it */
        private const int MaxSyncWaitRefreshes = 2;

        private static readonly string[] _causeNames = new string[]
        {
            "None", "DisplayMode", "UiThreadLateGc", "UiThreadLate", "PacingPolicy", "PaintNotRun",
            "DispatchLate", "GameLock", "PaintCpu", "Gpu", "Compositor", "Unattributed", "UiThreadRequests",
            "FrameworkCadence"
        };

        private static readonly string[] _contentEventNames = new string[]
        {
            "FloatingText", "ScreenText", "ConditionText", "GuiEffect", "ScreenFilter",
            "Message", "ViewChange", "Window", "OtherRequest", "MapUpdate"
        };

        public static string CauseName(GHHitchCause cause)
        {
            int i = (int)cause;
            return i >= 0 && i < _causeNames.Length ? _causeNames[i] : "Unknown";
        }

        /* Name of the content event kind at a bit position of GHContentEvent */
        public static string ContentEventName(int kind)
        {
            return kind >= 0 && kind < _contentEventNames.Length ? _contentEventNames[kind] : "Unknown";
        }

        /* Names of every event in a mask, joined with '+', or "" for none */
        public static string ContentEventNames(GHContentEvent events)
        {
            string text = "";
            for (int k = 0; k < ContentEventKinds; k++)
            {
                if (((int)events & (1 << k)) == 0)
                    continue;
                text = text.Length == 0 ? _contentEventNames[k] : text + "+" + _contentEventNames[k];
            }
            return text;
        }

        private static double TicksToMs(double ticks)
        {
            return ticks * 1000.0 / Stopwatch.Frequency;
        }

        /* The median refresh period (or, with callbackPeriod, the median callback period),
           0 when no record has one */
        private static long MedianPeriod(GHFrameRecord[] records, int n, bool callbackPeriod = false)
        {
            List<long> periods = new List<long>();
            for (int i = 0; i < n; i++)
            {
                long p = callbackPeriod ? records[i].CallbackPeriodTicks : records[i].RefreshPeriodTicks;
                if (p > 0)
                    periods.Add(p);
            }
            if (periods.Count == 0)
                return 0;
            periods.Sort();
            return periods[periods.Count / 2];
        }

        /* A gap longer than this is a hitch: half a refresh beyond the longest on-time hold */
        public static long HitchThresholdTicks(long targetPeriodTicks, long refreshPeriodTicks)
        {
            return (long)GHPerformanceStatistics.OnTimeHold(targetPeriodTicks, refreshPeriodTicks) + refreshPeriodTicks / 2;
        }

        /* The most common target or assumed rate of the ticks that show the map, or of all
           ticks when none does */
        private static int ModeShort(GHFrameRecord[] records, int n, bool target)
        {
            int best = ModeShort(records, n, target, true);
            return best > 0 ? best : ModeShort(records, n, target, false);
        }

        private static int ModeShort(GHFrameRecord[] records, int n, bool target, bool skipPauses)
        {
            Dictionary<int, int> counts = new Dictionary<int, int>();
            int best = 0, bestCount = 0;
            for (int i = 0; i < n; i++)
            {
                if (skipPauses && IsPauseTick(ref records[i]))
                    continue;
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

        /* The FrameMetrics frame with the latest intended vsync at or before t, or -1 */
        private static int LatestCompositorAtOrBefore(GHCompositorFrame[] comp, int m, long t)
        {
            int lo = 0, hi = m - 1, found = -1;
            while (lo <= hi)
            {
                int mid = (lo + hi) / 2;
                if (comp[mid].IntendedVsyncTicks <= t)
                {
                    found = mid;
                    lo = mid + 1;
                }
                else
                {
                    hi = mid - 1;
                }
            }
            while (found >= 0 && comp[found].Source != GHCompositorSource.AndroidFrameMetrics)
                found--;
            return found;
        }

        /* Android can hand the frame callback a frameTimeNanos one refresh older than the one
           HWUI records for the same doFrame. Returns records with each tick's vsync replaced by
           the HWUI frame time where they disagree by one refresh: a copy when anything changed,
           else records itself. corrected is the number of ticks changed, eligible those with a
           vsync. */
        private static GHFrameRecord[] CorrectStaleVsync(GHFrameRecord[] records, int n,
            GHCompositorFrame[] compositor, int m, long fallbackPeriod, out int corrected, out int eligible)
        {
            const sbyte NotEligible = -2, Unclassified = -1, Current = 0, Stale = 1;
            corrected = 0;
            eligible = 0;
            if (records == null || compositor == null)
                return records;
            if (n > records.Length)
                n = records.Length;
            if (m > compositor.Length)
                m = compositor.Length;
            bool frameMetrics = false;
            for (int i = 0; i < m && !frameMetrics; i++)
                frameMetrics = compositor[i].Source == GHCompositorSource.AndroidFrameMetrics;
            if (!frameMetrics || n <= 0)
                return records;

            /* A tick is stale when the HWUI frame of its doFrame, the latest to begin by its
               callback, lies one refresh after its vsync, and current when they agree */
            long tolerance = Stopwatch.Frequency / 1000;
            sbyte[] state = new sbyte[n];
            long[] shift = new long[n];
            List<long> staleShifts = new List<long>();
            for (int i = 0; i < n; i++)
            {
                if (records[i].VsyncTicks == 0 || records[i].CallbackStartTicks == 0)
                {
                    state[i] = NotEligible;
                    continue;
                }
                eligible++;
                state[i] = Unclassified;
                int ci = LatestCompositorAtOrBefore(compositor, m, records[i].CallbackStartTicks);
                if (ci < 0)
                    continue;
                long d = compositor[ci].VsyncTicks - records[i].VsyncTicks;
                long p = records[i].RefreshPeriodTicks > 0 ? records[i].RefreshPeriodTicks : fallbackPeriod;
                if (d - p <= tolerance && d - p >= -tolerance)
                {
                    state[i] = Stale;
                    shift[i] = d;
                    staleShifts.Add(d);
                }
                else if (d <= tolerance && d >= -tolerance)
                {
                    state[i] = Current;
                }
            }
            /* Without a stale tick, which includes no tick classified at all, the records stand */
            if (staleShifts.Count == 0)
                return records;
            staleShifts.Sort();
            long medianShift = staleShifts[staleShifts.Count / 2];

            /* A tick without a matching frame takes the state of the nearest classified tick
               before it, or failing that after it */
            sbyte last = Unclassified;
            for (int i = 0; i < n; i++)
            {
                if (state[i] == NotEligible)
                    continue;
                if (state[i] != Unclassified)
                {
                    last = state[i];
                    continue;
                }
                state[i] = last;
                if (last == Stale)
                    shift[i] = medianShift;
            }
            last = Unclassified;
            for (int i = n - 1; i >= 0; i--)
            {
                if (state[i] == NotEligible)
                    continue;
                if (state[i] != Unclassified)
                {
                    last = state[i];
                    continue;
                }
                state[i] = last;
                if (last == Stale)
                    shift[i] = medianShift;
            }

            GHFrameRecord[] copy = new GHFrameRecord[n];
            Array.Copy(records, copy, n);
            for (int i = 0; i < n; i++)
            {
                if (state[i] != Stale)
                    continue;
                copy[i].VsyncTicks += shift[i];
                if (copy[i].ExpectedPresentTicks != 0)
                    copy[i].ExpectedPresentTicks += shift[i];
                corrected++;
            }
            return copy;
        }

        /* How long after its intended vsync compositor frame i started syncing, when that
           lies within a refresh; else 0 */
        private static long SyncOffset(GHCompositorFrame[] comp, int i, long refreshPeriod)
        {
            if (i < 0 || comp[i].IntendedVsyncTicks == 0 || comp[i].SyncStartTicks == 0)
                return 0;
            long offset = comp[i].SyncStartTicks - comp[i].IntendedVsyncTicks;
            return offset > 0 && offset < refreshPeriod ? offset : 0;
        }

        /* A tick during which the map is not being shown: a menu, overlay, suspension,
           resize or anything else that stops the map render */
        public static bool IsPauseTick(ref GHFrameRecord r)
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
           returns them. Long stalls count as hitches. */
        public static GHSmoothnessSummary Analyze(GHFrameRecord[] records, int n, GHCompositorFrame[] compositor, int m,
                                                  GHDisplayedFrame[] displayed, out int displayedCount)
        {
            return Analyze(records, n, compositor, m, displayed, out displayedCount, false);
        }

        /* The same; excludeLongStalls makes a gap with a long stall a paused gap, as a
           retrospective report needs. A gap holding a lifecycle break is paused either way. */
        public static GHSmoothnessSummary Analyze(GHFrameRecord[] records, int n, GHCompositorFrame[] compositor, int m,
                                                  GHDisplayedFrame[] displayed, out int displayedCount, bool excludeLongStalls)
        {
            GHSmoothnessSummary s = new GHSmoothnessSummary();
            displayedCount = 0;
            s.MetricsVersion = MetricsVersion;
            s.LongStallsExcluded = excludeLongStalls;
            s.TickCount = n;
            if (records == null || n <= 0)
                return s;
            if (compositor == null)
                m = 0;

            long measuredPeriod = MedianPeriod(records, n);
            long period = measuredPeriod > 0 ? measuredPeriod : Stopwatch.Frequency / 60;
            s.MeasuredRefreshHz = measuredPeriod > 0 ? (double)Stopwatch.Frequency / measuredPeriod : 0;
            long callbackPeriod = MedianPeriod(records, n, true);
            s.CallbackRefreshHz = callbackPeriod > 0 ? (double)Stopwatch.Frequency / callbackPeriod : 0;

            /* Everything below reads the corrected vsyncs */
            int vsyncCorrected, vsyncEligible;
            records = CorrectStaleVsync(records, n, compositor, m, period, out vsyncCorrected, out vsyncEligible);
            s.VsyncCorrectedShare = vsyncEligible > 0 ? (double)vsyncCorrected / vsyncEligible : -1;

            /* GC pause time is judged only when the runtime reported some; a runtime that
               does not leaves every record at 0 */
            bool pauseAvailable = false;
            for (int i = 0; i < n && !pauseAvailable; i++)
                pauseAvailable = records[i].GcPauseTicks != 0;
            s.GcPauseDataAvailable = pauseAvailable;
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
            /* DWM frames mean the vsync times are the latest vblank at each callback, also in
               recordings made before the records carried the flag */
            bool dwmCompositor = false;
            for (int i = 0; i < m && !dwmCompositor; i++)
                dwmCompositor = compositor[i].Source == GHCompositorSource.WindowsDwm;

            /* Sync offsets of the FrameMetrics reports within a refresh, a diagnostic only */
            List<double> syncOffsetMs = new List<double>();
            for (int i = 0; i < m; i++)
            {
                if (compositor[i].Source != GHCompositorSource.AndroidFrameMetrics)
                    continue;
                long offset = compositor[i].SyncStartTicks - compositor[i].IntendedVsyncTicks;
                if (offset > 0 && offset < period)
                    syncOffsetMs.Add(TicksToMs(offset));
            }
            s.SyncOffsetP50Ms = syncOffsetMs.Count > 0 ? Percentile(syncOffsetMs, 50) : 0;

            /* Reports the FrameMetrics listener missed: data loss, not display drops */
            long spanStart = 0, spanEnd = 0;
            for (int i = 0; i < n; i++)
            {
                long t = records[i].VsyncTicks != 0 ? records[i].VsyncTicks : records[i].CallbackStartTicks;
                if (t == 0)
                    continue;
                if (spanStart == 0)
                    spanStart = t;
                spanEnd = t;
            }
            for (int k = 0; k < m && spanStart != 0; k++)
            {
                if (compositor[k].IntendedVsyncTicks >= spanStart && compositor[k].IntendedVsyncTicks <= spanEnd)
                    s.CompositorReportsLost += compositor[k].DroppedSinceLast;
            }

            VsyncGrid grid = new VsyncGrid(records, n, period);

            /* 1. Display time of every painted frame */
            List<Candidate> candidates = new List<Candidate>();
            List<double> paintMs = new List<double>();
            List<double> callbackLateMs = new List<double>();
            int glPaints = 0, coveredPaints = 0;
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

                /* A frame painted inside the callback is ready when its buffer swap returns */
                long flushed = r.FlushEndTicks + SwapWaitTicks(r);

                Candidate c = new Candidate();
                c.RecordIndex = i;
                c.ReadyAt = flushed;
                if (r.PresentSource == GHPresentSource.Measured && r.DisplayedAtTicks != 0)
                {
                    c.DisplayedAt = r.DisplayedAtTicks;
                    c.Source = GHPresentSource.Measured;
                    long expected = grid.NextBoundaryAfter(flushed);
                    c.DisplayDelay = r.DisplayedAtTicks > expected ? r.DisplayedAtTicks - expected : 0;
                }
                else
                {
                    long ready = flushed;
                    if (!r.PaintOnUiThread)
                        glPaints++;
                    if (androidCompositor)
                    {
                        long recPeriod = r.RefreshPeriodTicks > 0 ? r.RefreshPeriodTicks : period;
                        int ci;
                        if (r.PaintOnUiThread)
                        {
                            ci = FindCompositorByVsync(compositor, m, r.VsyncTicks, recPeriod / 4);
                        }
                        else
                        {
                            /* The HWUI frame that carries a flushed frame is the first to sync
                               after the flush: that of the refresh the flush fell in when the
                               flush beat its sync, as the nearest report's sync offset tells,
                               else the next vsync's. It starts syncing within the refresh that
                               follows its vsync. A later frame is accepted only when no report
                               was lost before it */
                            long nextVsync = grid.NextBoundaryAfter(r.FlushEndTicks);
                            ci = FindCompositorBySyncAfter(compositor, m, r.FlushEndTicks);
                            long carrierVsync = nextVsync;
                            long syncOffset = SyncOffset(compositor, ci >= 0 ? ci : m - 1, recPeriod);
                            if (r.FlushEndTicks < nextVsync - recPeriod + syncOffset)
                                carrierVsync = nextVsync - recPeriod;
                            if (ci >= 0)
                            {
                                long sync = compositor[ci].SyncStartTicks;
                                bool ofCarrierVsync = sync < carrierVsync + recPeriod;
                                bool lateButContiguous = compositor[ci].DroppedSinceLast == 0
                                    && sync - r.FlushEndTicks <= MaxSyncWaitRefreshes * recPeriod;
                                if (!ofCarrierVsync && !lateButContiguous)
                                    ci = -1;
                            }
                            /* No report for this paint: the carrying vsync's frame showed it */
                            if (ci < 0 && carrierVsync > ready)
                                ready = carrierVsync;
                            if (ci >= 0)
                                coveredPaints++;
                        }
                        if (ci >= 0 && compositor[ci].CompletedTicks > ready)
                            ready = compositor[ci].CompletedTicks;
                    }
                    c.ReadyAt = ready;
                    c.DisplayedAt = grid.NextBoundaryAfter(ready);
                    c.Source = GHPresentSource.Estimated;
                }
                candidates.Add(c);
            }
            /* An Android run whose listener received nothing reads 0; compositor frames of
               another source, or no GL-thread paints, make it not applicable */
            s.CompositorCoverage = glPaints > 0 && (androidCompositor || m == 0) ? (double)coveredPaints / glPaints : -1;

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

            /* An app-forced collection ran between the callback before its flagged tick and the
               flagged tick's own; it stops every thread, so it can delay the frame in flight
               before that tick as much as the tick itself. The intervals ascend, as the gaps do. */
            List<long> forcedFrom = new List<long>();
            List<long> forcedTo = new List<long>();
            for (int i = 1; i < n; i++)
            {
                if ((records[i].Flags & GHFrameFlags.ForcedCollection) == 0)
                    continue;
                if (records[i - 1].CallbackStartTicks == 0 || records[i].CallbackStartTicks == 0)
                    continue;
                forcedFrom.Add(records[i - 1].CallbackStartTicks);
                forcedTo.Add(records[i].CallbackStartTicks);
            }
            int forcedIdx = 0;

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

                /* A pause tick or a lifecycle break pauses the gap. Otherwise a stop in the
                   display callbacks of LongStallSeconds or more is a long stall, paused only
                   when excluded. */
                bool paused = false;
                for (int i = prevIdx + 1; i <= curIdx && !paused; i++)
                    paused = IsPauseTick(ref records[i]) || (records[i].Flags & GHFrameFlags.LifecycleBreak) != 0;
                if (!paused)
                {
                    long stallTicks = (long)(LongStallSeconds * Stopwatch.Frequency);
                    bool longStall = false;
                    for (int i = prevIdx + 1; i <= curIdx && !longStall; i++)
                    {
                        long cbPrev = records[i - 1].CallbackStartTicks;
                        long cbCur = records[i].CallbackStartTicks;
                        longStall = cbPrev != 0 && cbCur != 0 && cbCur - cbPrev >= stallTicks;
                    }
                    if (longStall)
                    {
                        displayed[j].IsLongStall = true;
                        s.LongStallCount++;
                        s.LongStallMs += TicksToMs(gap);
                        paused = excludeLongStalls;
                    }
                }
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

                long holdRefreshes = (long)Math.Round((double)gap / refresh, MidpointRounding.AwayFromZero);
                long intendedRefreshes = (long)Math.Round(GHPerformanceStatistics.OnTimeHold(target, refresh) / refresh);
                if (holdRefreshes > intendedRefreshes)
                    repeats += holdRefreshes - intendedRefreshes;

                displayed[j].IsHitch = gap > HitchThresholdTicks(target, refresh);
                displayed[j].IsJudder = Math.Abs(displayed[j].PacingErrorTicks) > refresh / 4;

                GHContentEvent gapEvents = GHContentEvent.None;
                for (int i = prevIdx + 1; i <= curIdx; i++)
                    gapEvents |= records[i].ContentEvents;
                displayed[j].ContentEvents = gapEvents;
                long gapFrom = displayed[j - 1].DisplayedAtTicks;
                while (forcedIdx < forcedTo.Count && forcedTo[forcedIdx] <= gapFrom)
                    forcedIdx++;
                bool forcedGc = forcedIdx < forcedTo.Count && forcedFrom[forcedIdx] < displayed[j].DisplayedAtTicks;
                displayed[j].OverlapsForcedGc = forcedGc;
                if (gapEvents == GHContentEvent.None)
                {
                    s.QuietGapCount++;
                    if (displayed[j].IsHitch)
                        s.QuietHitchCount++;
                }
                else
                {
                    for (int k = 0; k < ContentEventKinds; k++)
                    {
                        if (((int)gapEvents & (1 << k)) == 0)
                            continue;
                        s.EventGapCount[k]++;
                        if (displayed[j].IsHitch)
                            s.EventHitchCount[k]++;
                    }
                }
                if (displayed[j].IsHitch)
                {
                    s.HitchCount++;
                    hitchMs += TicksToMs(gap - target);
                    if (forcedGc)
                    {
                        s.ForcedGcHitchCount++;
                        s.ForcedGcHitchMs += TicksToMs(gap - target);
                    }
                }
                if (displayed[j].IsJudder)
                    s.JudderCount++;

                if (displayed[j].IsHitch || displayed[j].IsJudder)
                {
                    GHHitchCause cause = Attribute(records, n, prevIdx, curIdx, compositor, m, isDisplayed, target, refresh,
                                                   gap, displayed[j].DisplayDelayTicks, pauseAvailable, dwmCompositor);
                    displayed[j].Cause = cause;
                    s.CauseCount[(int)cause]++;
                    if (displayed[j].IsHitch)
                    {
                        s.CauseHitchCount[(int)cause]++;
                        s.CauseMs[(int)cause] += TicksToMs(gap - target);
                    }
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
            /* CoreCLR's gen0 count includes every higher-generation collection; Mono's counts minor
               collections only, and a full collection moves gen1 and gen2 alone. The larger delta of
               each tick counts its collections on both */
            int gcCount = 0;
            for (int i = 1; i < n; i++)
            {
                int d0 = records[i].GcCount0 - records[i - 1].GcCount0;
                int d2 = records[i].GcCount2 - records[i - 1].GcCount2;
                gcCount += Math.Max(0, Math.Max(d0, d2));
            }
            s.GcCount = gcCount;
            /* From the second tick, as GcCount: the first tick's flag is a collection before the window */
            int forcedGcCount = 0;
            for (int i = 1; i < n; i++)
            {
                if ((records[i].Flags & GHFrameFlags.ForcedCollection) != 0)
                    forcedGcCount++;
            }
            s.ForcedGcCount = forcedGcCount;
            s.GcPauseMs = pauseAvailable ? TicksToMs(Math.Max(0, records[n - 1].GcPauseTicks - records[0].GcPauseTicks)) : 0;
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

        /* Time the UI thread spent in the display callback after the flush, i.e. in the
           platform's buffer swap, when the paint ran inside that callback (Windows GL);
           0 when the paint ran outside it */
        public static long SwapWaitTicks(GHFrameRecord r)
        {
            if (!r.PaintOnUiThread || r.CallbackStartTicks == 0 || r.CallbackEndTicks == 0
                || r.PaintStartTicks == 0 || r.FlushEndTicks == 0)
                return 0;
            if (r.PaintStartTicks < r.CallbackStartTicks || r.FlushEndTicks > r.CallbackEndTicks)
                return 0;
            return r.CallbackEndTicks - r.FlushEndTicks;
        }

        /* Charges the gap before a late or uneven frame to the first stage, in pipeline
           order, that exceeded its budget. prevIdx and curIdx are the records of the
           displayed frames on either side of the gap; n is the number of records.
           pauseAvailable says the records carry GC pause time; without it a collection in
           the gap is judged by the GC counts alone. vsyncIsLatestVblank says every record's
           vsync time is the latest vblank at its callback, as a record's
           GHFrameFlags.VsyncIsLatestVblank does for that record. */
        public static GHHitchCause Attribute(GHFrameRecord[] records, int n, int prevIdx, int curIdx,
                                             GHCompositorFrame[] compositor, int m, bool[] isDisplayed,
                                             long targetPeriod, long refreshPeriod, long gapTicks, long displayDelayTicks,
                                             bool pauseAvailable, bool vsyncIsLatestVblank = false)
        {
            long halfRefresh = refreshPeriod / 2;

            /* 1. Display mode: the measured period moved by more than 5 % across the gap, or
               the pacing logic is dividing a refresh rate the panel is not running at. The
               measured period is a running median, so a change shows in the records up to
               PeriodMedianLag ticks after the gap it caused. */
            long p0 = records[prevIdx].RefreshPeriodTicks;
            int lastPeriodIdx = Math.Min(n - 1, curIdx + GHFrameTimeline.PeriodMedianLag);
            for (int i = prevIdx + 1; i <= lastPeriodIdx; i++)
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

            /* 2. UI thread: a missed callback or a callback well after its vsync; where the
               vsync time is the latest vblank, the callback's lateness after it is only a
               phase and a missed callback alone counts. When the
               UI thread was busy painting the map across that vsync, including a buffer
               swap inside the callback, the paint is the cause, not the thread: the draw
               is charged to the CPU, the flush and swap to the GPU. When it spent more
               than half a refresh handling game requests just before the late callback,
               the requests are, even with a collection in the gap, when pause data exists;
               without it a collection takes precedence. A collection is the cause when it paused the process
               for at least half a refresh in the gap (with pause data), or ran at all
               (without). With the thread not otherwise explained, a late callback while the
               callback period ran at 1.5 refreshes or more is the UI framework's cadence:
               the median shows it delivered callbacks below the panel's rate for a while.
               Like the refresh period, the median lags a real change by up to
               PeriodMedianLag ticks, so the records after the gap are consulted too. */
            bool uiLate = false, countsMoved = false, requests = false;
            int firstLate = -1;
            long pauseTicks = 0;
            GHHitchCause ownPaint = GHHitchCause.None;
            for (int i = prevIdx + 1; i <= curIdx; i++)
            {
                GHFrameRecord r = records[i];
                GHFrameRecord q = records[i - 1];
                long cur = r.VsyncTicks != 0 ? r.VsyncTicks : r.CallbackStartTicks;
                long prev = q.VsyncTicks != 0 ? q.VsyncTicks : q.CallbackStartTicks;
                bool phaseOnly = vsyncIsLatestVblank || (r.Flags & GHFrameFlags.VsyncIsLatestVblank) != 0;
                bool late = (cur != 0 && prev != 0 && cur - prev > refreshPeriod + halfRefresh)
                    || (!phaseOnly && r.VsyncTicks != 0 && r.CallbackStartTicks - r.VsyncTicks > halfRefresh);
                if (r.GcCount0 != q.GcCount0 || r.GcCount1 != q.GcCount1 || r.GcCount2 != q.GcCount2)
                    countsMoved = true;
                if (r.GcPauseTicks > q.GcPauseTicks)
                    pauseTicks += r.GcPauseTicks - q.GcPauseTicks;
                if (!late)
                    continue;
                uiLate = true;
                if (firstLate < 0)
                    firstLate = i;
                if (r.RequestTicks > halfRefresh)
                    requests = true;
                long deadline = (cur != 0 ? cur : r.CallbackStartTicks) + halfRefresh;
                long missedVsync = prev != 0 ? prev + refreshPeriod : deadline - halfRefresh;
                for (int k = prevIdx; k < i && ownPaint == GHHitchCause.None; k++)
                {
                    GHFrameRecord pr = records[k];
                    if (!pr.PaintOnUiThread || pr.PaintStartTicks == 0 || pr.FlushEndTicks == 0)
                        continue;
                    long swap = SwapWaitTicks(pr);
                    if (pr.PaintStartTicks < deadline && pr.FlushEndTicks + swap > missedVsync)
                    {
                        long draw = pr.DrawEndTicks != 0 ? pr.DrawEndTicks - pr.PaintStartTicks : 0;
                        long flush = pr.DrawEndTicks != 0 ? pr.FlushEndTicks - pr.DrawEndTicks : 0;
                        ownPaint = Math.Max(flush, swap) > draw ? GHHitchCause.Gpu : GHHitchCause.PaintCpu;
                    }
                }
            }
            if (ownPaint != GHHitchCause.None)
                return ownPaint;
            /* Without pause data a collection's pause shows up as request time too: the collection is the cause */
            if (uiLate && !pauseAvailable && countsMoved)
                return GHHitchCause.UiThreadLateGc;
            if (requests)
                return GHHitchCause.UiThreadRequests;
            if (uiLate)
            {
                bool gc = pauseAvailable ? pauseTicks >= halfRefresh : countsMoved;
                if (gc)
                    return GHHitchCause.UiThreadLateGc;
                int lastCadenceIdx = Math.Min(n - 1, curIdx + GHFrameTimeline.PeriodMedianLag);
                for (int k = firstLate; k <= lastCadenceIdx; k++)
                {
                    long cb = records[k].CallbackPeriodTicks;
                    if (cb > 0 && cb * 2 >= refreshPeriod * 3)
                        return GHHitchCause.FrameworkCadence;
                }
                return GHHitchCause.UiThreadLate;
            }

            /* 3. Pacing policy: the loop's own irregular skip in the gap, a catch-up render
               that bypassed a skip pattern, or a ratio of refresh to target rate the divisor
               pattern cannot pace evenly. Such a pattern holds frames for at most two divisor
               steps; a longer gap has another cause. */
            int targetFps = records[curIdx].TargetFps;
            long divisor = assumedHz > targetFps && targetFps > 0 ? assumedHz / targetFps : 1;
            bool withinPattern = gapTicks <= 2 * divisor * refreshPeriod + refreshPeriod / 4;
            if (withinPattern)
            {
                for (int i = prevIdx + 1; i <= curIdx; i++)
                {
                    GHFrameRecord q = records[i];
                    if (q.Pacing == GHPacingDecision.SkippedModulo)
                        return GHHitchCause.PacingPolicy;
                    /* At or below the target rate the catch-up path renders exactly as a regular tick */
                    if (q.Pacing == GHPacingDecision.RenderedCatchUp && q.TargetFps > 0 && q.AssumedRefreshHz > q.TargetFps)
                        return GHHitchCause.PacingPolicy;
                }
                if (assumedHz > targetFps && targetFps > 0 && assumedHz % targetFps != 0)
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
                if (SwapWaitTicks(r) > targetPeriod / 2)
                    return GHHitchCause.Gpu;
            }
            GHFrameRecord cr = records[curIdx];
            bool haveCompositor = compositor != null && m > 0;
            long from = records[prevIdx].VsyncTicks != 0 ? records[prevIdx].VsyncTicks : records[prevIdx].CallbackStartTicks;
            long to = cr.FlushEndTicks != 0 ? cr.FlushEndTicks + SwapWaitTicks(cr) : cr.CallbackStartTicks;

            /* 8, compositor side: the GPU ran longer than a refresh on a frame in the gap */
            for (int k = 0; haveCompositor && k < m; k++)
            {
                GHCompositorFrame f = compositor[k];
                if (f.IntendedVsyncTicks >= from && f.IntendedVsyncTicks <= to && f.GpuDurationTicks > refreshPeriod)
                    return GHHitchCause.Gpu;
            }

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
            if (haveCompositor)
            {
                for (int k = 0; k < m; k++)
                {
                    GHCompositorFrame f = compositor[k];
                    if (f.IntendedVsyncTicks < from || f.IntendedVsyncTicks > to)
                        continue;
                    if (f.Source == GHCompositorSource.AndroidFrameMetrics && f.CompletedTicks != 0
                        && f.CompletedTicks - f.IntendedVsyncTicks > refreshPeriod + halfRefresh)
                        return GHHitchCause.Compositor;
                }
            }

            return GHHitchCause.Unattributed;
        }
    }

    /* Online estimate of what the display shows, fed with each completed paint's ready
       time (its flush end, or for a paint inside the display callback, the callback end
       after its buffer swap), for the dashboard and for cadence-change events. It uses the
       same display-time rule as GHSmoothnessMetrics without the compositor refinement, over
       250 ms buckets. A change
       is reported when the displayed rate of the last second differs from that of the three
       seconds before by more than 10 % for a full second, when the refresh period moves by
       more than 5 % for a full second (REFRESH), or when the display callbacks arrive more
       than 5 % slower or faster than the panel refreshes, or return to its rate, for a full
       second (CALLBACKS). Where the platform reports no panel period of its own, the two
       periods are one measurement and only REFRESH fires. A vsync's frame is judged when the
       next vsync's first paint arrives, since a later paint on the same vsync replaces it.
       Runs on the paint thread. */
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

        /* The newest vsync a paint landed on and the counter of its latest paint, and the
           displayed frame before it */
        private static long _lastBoundary = 0;
        private static long _lastCounter = 0;
        private static long _shownBoundary = 0;
        private static long _shownCounter = 0;
        private static int _changeStreak = 0;
        private static int _refreshStreak = 0;
        private static long _reportedPeriod = 0;
        private static int _callbackStreak = 0;
        private static bool _reportedCallbacksOff = false;     /* last report: callbacks off the panel's rate */
        private static long _reportedCallbackPeriod = 0;       /* the period reported with it */

        private static double _displayedFps = 0;
        private static double _hitchRatio = 0;
        private static double _pacingErrorRmsMs = 0;
        private static string _lastChange = "";
        private static long _lastReadyTicks = 0;

        /* Receives one line per detected change, e.g. for the screen log */
        public static Action<string> ChangeLog = null;

        public static double DisplayedFps { get { return _displayedFps; } }
        public static double HitchRatioMsPerSec { get { return _hitchRatio; } }
        public static double PacingErrorRmsMs { get { return _pacingErrorRmsMs; } }
        public static string LastChange { get { return _lastChange; } }
        public static long LastReadyTicks { get { return _lastReadyTicks; } }   /* ready time of the latest paint fed in */

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
            _shownBoundary = 0;
            _shownCounter = 0;
            _changeStreak = 0;
            _refreshStreak = 0;
            _reportedPeriod = 0;
            _callbackStreak = 0;
            _reportedCallbacksOff = false;
            _reportedCallbackPeriod = 0;
            _displayedFps = 0;
            _hitchRatio = 0;
            _pacingErrorRmsMs = 0;
            _lastChange = "";
            _lastReadyTicks = 0;
        }

        public static void OnPaintCompleted(long vsyncTicks, long refreshPeriodTicks, long callbackPeriodTicks,
                                            long readyTicks, int targetFps, long paintedMainCounter)
        {
            if (readyTicks == 0 || refreshPeriodTicks <= 0)
                return;
            _lastReadyTicks = readyTicks;
            long freq = Stopwatch.Frequency;
            long stallTicks = (long)(GHSmoothnessMetrics.LongStallSeconds * freq);
            long baseTime = vsyncTicks != 0 && vsyncTicks <= readyTicks ? vsyncTicks : readyTicks - refreshPeriodTicks;
            long boundary = baseTime + ((readyTicks - baseTime) / refreshPeriodTicks + 1) * refreshPeriodTicks;
            long target = targetFps > 0 ? freq / targetFps : refreshPeriodTicks;

            if (_bucketStart == 0)
                _bucketStart = readyTicks;

            if (boundary > _lastBoundary)
            {
                /* The previous vsync is final: its latest paint is the frame shown there */
                if (_lastBoundary != 0 && _shownBoundary != 0)
                {
                    long gap = _lastBoundary - _shownBoundary;
                    /* A gap of LongStallSeconds or more is a pause, not a frame */
                    if (gap < stallTicks)
                    {
                        _bucketFrames++;
                        if (gap > GHSmoothnessMetrics.HitchThresholdTicks(target, refreshPeriodTicks))
                            _bucketHitchMs += (gap - target) * 1000.0 / freq;
                        long step = _lastCounter - _shownCounter;
                        if (step > 0 && step < 1000)
                        {
                            double errMs = (gap - step * target) * 1000.0 / freq;
                            _bucketErrSq += errMs * errMs;
                            _bucketErrCount++;
                        }
                    }
                }
                if (_lastBoundary != 0)
                {
                    _shownBoundary = _lastBoundary;
                    _shownCounter = _lastCounter;
                }
                _lastBoundary = boundary;
                _lastCounter = paintedMainCounter;
            }
            else if (boundary == _lastBoundary)
            {
                /* Two paints on one vsync: the later one is shown */
                _lastCounter = paintedMainCounter;
            }

            long elapsed = readyTicks - _bucketStart;
            if (elapsed >= freq / 4)
            {
                double seconds = (double)elapsed / freq;
                PushBucket(_bucketFrames / seconds, _bucketHitchMs / seconds,
                           _bucketErrCount > 0 ? Math.Sqrt(_bucketErrSq / _bucketErrCount) : 0,
                           refreshPeriodTicks, callbackPeriodTicks);
                _bucketStart = readyTicks;
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

        private static void PushBucket(double fps, double hitchRatio, double errRms, long refreshPeriodTicks,
                                       long callbackPeriodTicks)
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

            /* Callbacks leaving the panel's rate, moving while off it, or returning to it; a
               panel change the callbacks follow is REFRESH's alone */
            if (callbackPeriodTicks > 0 && refreshPeriodTicks > 0)
            {
                bool off = Math.Abs(callbackPeriodTicks - refreshPeriodTicks) > refreshPeriodTicks / 20;
                bool changed = off != _reportedCallbacksOff
                    || (off && Math.Abs(callbackPeriodTicks - _reportedCallbackPeriod) > _reportedCallbackPeriod / 20);
                if (changed)
                {
                    _callbackStreak++;
                    if (_callbackStreak >= PersistBuckets)
                    {
                        long fromTicks = _reportedCallbacksOff ? _reportedCallbackPeriod : refreshPeriodTicks;
                        double oldMs = fromTicks * 1000.0 / Stopwatch.Frequency;
                        double newMs = callbackPeriodTicks * 1000.0 / Stopwatch.Frequency;
                        double panelMs = refreshPeriodTicks * 1000.0 / Stopwatch.Frequency;
                        Report(GHTraceEvent.CallbackCadenceChange, (long)Math.Round(newMs * 1000.0),
                            "CALLBACKS " + oldMs.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture)
                            + "->" + newMs.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture) + " ms (panel "
                            + panelMs.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture) + " ms)");
                        _reportedCallbacksOff = off;
                        _reportedCallbackPeriod = callbackPeriodTicks;
                        _callbackStreak = 0;
                    }
                }
                else
                {
                    _callbackStreak = 0;
                }
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

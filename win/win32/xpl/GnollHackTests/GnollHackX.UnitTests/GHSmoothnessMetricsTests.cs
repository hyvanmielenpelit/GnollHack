using System;
using System.Collections.Generic;
using System.Diagnostics;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* One synthetic frame timeline per hypothesis of the smoothness plan, each checked for the
       metric signature and attribution class it must produce. The pacing decisions come from
       the same divisor and modulo rule the render loop uses. */
    [Collection(GHFrameTimelineTests.StaticStateCollection)]
    public class GHSmoothnessMetricsTests
    {
        private static readonly long F = Stopwatch.Frequency;

        private static long Ms(double ms)
        {
            return (long)Math.Round(ms * F / 1000.0);
        }

        /* The render loop's decision for callback number counter (GHApp.CompositionTarget_Rendering) */
        private static GHPacingDecision Decide(long counter, int screenHz, int targetFps)
        {
            if (screenHz <= targetFps)
                return GHPacingDecision.Rendered;
            int divisor = Math.Max(1, screenHz / targetFps);
            if (divisor == 1 || counter % divisor == 0)
            {
                int mod = screenHz % targetFps;
                if (mod > 0)
                {
                    int num = screenHz / mod;
                    if ((counter / divisor) % num == 0)
                        return GHPacingDecision.SkippedModulo;
                }
                return GHPacingDecision.Rendered;
            }
            return GHPacingDecision.SkippedDivisor;
        }

        private sealed class Timeline
        {
            public readonly List<GHFrameRecord> Records = new List<GHFrameRecord>();
            public long MainCounter = 1000;
            public long NextVsync = 1000 * F;
            public long FrameId = 0;

            /* One display callback at the next vsync of a panel with the given period */
            public int Tick(long periodTicks, int assumedHz, int targetFps, GHPacingDecision pacing, bool paintOnUi,
                            double paintMs = 3.0, double flushMs = 1.0, double callbackLateMs = 0.2)
            {
                GHFrameRecord r = new GHFrameRecord();
                r.FrameId = ++FrameId;
                r.VsyncTicks = NextVsync;
                r.RefreshPeriodTicks = periodTicks;
                r.CallbackStartTicks = NextVsync + Ms(callbackLateMs);
                r.CallbackEndTicks = r.CallbackStartTicks + Ms(0.3);
                r.TargetFps = (short)targetFps;
                r.AssumedRefreshHz = (short)assumedHz;
                r.Pacing = pacing;
                if (pacing == GHPacingDecision.Rendered || pacing == GHPacingDecision.RenderedCatchUp)
                {
                    MainCounter++;
                    r.MainCounter = MainCounter;
                    r.GeneralCounter = MainCounter / 2;
                    r.Invalidate = GHInvalidateOutcome.Invalidated;
                    r.InvalidateTicks = r.CallbackStartTicks + Ms(0.1);
                    r.Paint = GHPaintOutcome.Painted;
                    r.PaintOnUiThread = paintOnUi;
                    r.PaintStartTicks = r.InvalidateTicks + Ms(0.2);
                    r.LockAttemptTicks = r.PaintStartTicks + Ms(0.1);
                    r.LockResultTicks = r.LockAttemptTicks + Ms(0.01);
                    r.LockAcquired = true;
                    r.DrawEndTicks = r.PaintStartTicks + Ms(paintMs);
                    r.FlushEndTicks = r.DrawEndTicks + Ms(flushMs);
                    r.PaintedMainCounter = MainCounter;
                    r.PaintedGeneralCounter = MainCounter / 2;
                }
                Records.Add(r);
                NextVsync += periodTicks;
                return Records.Count - 1;
            }

            /* Callbacks for seconds of steady pacing on a panel at screenHz */
            public void Run(double seconds, int screenHz, int assumedHz, int targetFps, bool paintOnUi = true)
            {
                long period = F / screenHz;
                int count = (int)Math.Round(seconds * screenHz);
                for (int i = 0; i < count; i++)
                {
                    long counter = FrameId + 1;
                    Tick(period, assumedHz, targetFps, Decide(counter, assumedHz, targetFps), paintOnUi);
                }
            }

            public GHSmoothnessSummary Analyze(out GHDisplayedFrame[] displayed, out int displayedCount,
                                               GHCompositorFrame[] compositor = null, int m = 0)
            {
                GHFrameRecord[] arr = Records.ToArray();
                displayed = new GHDisplayedFrame[arr.Length];
                return GHSmoothnessMetrics.Analyze(arr, arr.Length, compositor, m, displayed, out displayedCount);
            }
        }

        /* Attribution covers hitches and judder alike */
        private static int TotalAttributed(GHSmoothnessSummary s)
        {
            int total = 0;
            for (int i = 0; i < GHSmoothnessMetrics.CauseCount; i++)
                total += s.CauseCount[i];
            return total;
        }

        private static GHHitchCause DominantCause(GHSmoothnessSummary s)
        {
            int best = 0;
            for (int i = 1; i < GHSmoothnessMetrics.CauseCount; i++)
            {
                if (s.CauseCount[i] > s.CauseCount[best])
                    best = i;
            }
            return (GHHitchCause)best;
        }

        [Fact]
        public void Baseline_60On60_IsSmooth()
        {
            Timeline t = new Timeline();
            t.Run(2.0, 60, 60, 60);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(60.0, s.DisplayedFps, 0);
            Assert.Equal(0, s.HitchCount);
            Assert.Equal(0, s.JudderCount);
            Assert.True(s.PacingErrorRmsMs < 0.01, "pacing error " + s.PacingErrorRmsMs);
            Assert.Equal(GHPresentSource.Estimated, s.PresentSource);
            Assert.False(s.AssumedRefreshMismatch);
        }

        [Fact]
        public void Baseline_60On120_HeldTwoRefreshesIsOnTime()
        {
            Timeline t = new Timeline();
            t.Run(2.0, 120, 120, 60);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(60.0, s.DisplayedFps, 0);
            Assert.Equal(0, s.HitchCount);
            Assert.Equal(0, s.JudderCount);
            Assert.Equal(0, s.OnScreenPacing.JankPct, 6);
        }

        /* H1 induced on a 60 Hz device: map 40 FPS alternates one and two refreshes */
        [Fact]
        public void H1_40On60_AlternatesAndIsAttributedToPacing()
        {
            Timeline t = new Timeline();
            t.Run(3.0, 60, 60, 40);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(40.0, s.DisplayedFps, 0);
            /* Every gap is 16.7 or 33.3 ms against a 25 ms target: 8.3 ms off */
            Assert.Equal(1000.0 / 120.0, s.PacingErrorRmsMs, 1);
            Assert.True(s.JudderPct > 95, "judder " + s.JudderPct);
            Assert.Equal(0, s.HitchCount);
            Assert.Equal(GHHitchCause.PacingPolicy, DominantCause(s));
        }

        /* H1 on the Windows PC at 144 Hz with map 60: four 13.9 ms frames, then 27.8 ms */
        [Fact]
        public void H1_60On144_PeriodicLongFrame()
        {
            Timeline t = new Timeline();
            t.Run(3.0, 144, 144, 60);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(60.0, s.DisplayedFps, 0);
            /* One gap in five is a hitch: 27.8 ms > 16.7 + 3.5 */
            double hitchShare = (double)s.HitchCount / (n - 1);
            Assert.InRange(hitchShare, 0.17, 0.23);
            Assert.True(s.JudderPct > 95, "judder " + s.JudderPct);
            Assert.Equal(GHHitchCause.PacingPolicy, DominantCause(s));
            Assert.Equal(TotalAttributed(s), s.CauseCount[(int)GHHitchCause.PacingPolicy]);
        }

        /* H2: the panel drops from 120 Hz to 60 Hz while the pacing logic keeps dividing by 120 */
        [Fact]
        public void H2_RefreshDropWithStaleAssumption_IsDisplayMode()
        {
            Timeline t = new Timeline();
            t.Run(2.0, 120, 120, 60);
            t.Run(2.0, 60, 120, 60);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount > 50, "hitches " + s.HitchCount);
            Assert.Equal(GHHitchCause.DisplayMode, DominantCause(s));
            Assert.Equal(TotalAttributed(s), s.CauseCount[(int)GHHitchCause.DisplayMode]);

            /* After the drop the display shows 30 FPS */
            int afterDrop = 0;
            for (int j = 1; j < n; j++)
            {
                if (d[j].GapTicks > Ms(30))
                    afterDrop++;
            }
            Assert.True(afterDrop > 50, "33 ms gaps " + afterDrop);
        }

        /* H3: the GL thread paints every other invalidation; the rest are coalesced */
        [Fact]
        public void H3_CoalescedPaints_ArePaintNotRun()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 180; i++)
            {
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, false);
                if (i % 2 == 1)
                {
                    GHFrameRecord r = t.Records[idx];
                    r.Paint = GHPaintOutcome.Coalesced;
                    r.PaintStartTicks = 0;
                    r.DrawEndTicks = 0;
                    r.FlushEndTicks = 0;
                    r.PaintedMainCounter = 0;
                    t.Records[idx] = r;
                }
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(90, s.CoalescedCount);
            Assert.Equal(30.0, s.DisplayedFps, 0);
            Assert.True(s.HitchCount > 80);
            Assert.Equal(GHHitchCause.PaintNotRun, DominantCause(s));
        }

        private static void ShiftPaint(Timeline t, int idx, long delay)
        {
            GHFrameRecord r = t.Records[idx];
            r.PaintStartTicks += delay;
            r.LockAttemptTicks += delay;
            r.LockResultTicks += delay;
            r.DrawEndTicks += delay;
            r.FlushEndTicks += delay;
            t.Records[idx] = r;
        }

        /* H4: the paint starts long after its invalidation, as when a dispatcher hop misses the
           frame. Paints on one thread are sequential, so the next frame's paint waits for it. */
        [Fact]
        public void H4_LateDispatch_IsDispatchLate()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 120; i++)
            {
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true);
                if (i % 10 == 5)
                {
                    ShiftPaint(t, idx, Ms(20));
                }
                else if (idx > 0 && t.Records[idx - 1].FlushEndTicks > t.Records[idx].PaintStartTicks)
                {
                    ShiftPaint(t, idx, t.Records[idx - 1].FlushEndTicks - t.Records[idx].PaintStartTicks + Ms(0.1));
                }
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount >= 10, "hitches " + s.HitchCount);
            Assert.Equal(GHHitchCause.DispatchLate, DominantCause(s));
        }

        /* H6: a collection stalls the UI thread across a vsync */
        [Fact]
        public void H6_MissedCallbackWithCollection_IsUiThreadLateGc()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            int gc = 0;
            for (int i = 0; i < 120; i++)
            {
                bool stall = i % 30 == 15;
                if (stall)
                {
                    /* The callback for this vsync never ran; the next one is late */
                    t.NextVsync += period;
                    gc++;
                }
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true, 3.0, 1.0, stall ? 12.0 : 0.2);
                GHFrameRecord r = t.Records[idx];
                r.GcCount0 = gc;
                t.Records[idx] = r;
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount >= 3, "hitches " + s.HitchCount);
            Assert.Equal(GHHitchCause.UiThreadLateGc, DominantCause(s));
        }

        /* A floating text arrives with a request batch long enough to delay the next callback:
           the requests are charged, and the event table ties the hitches to floating texts */
        [Fact]
        public void RequestBatchBeforeLateCallback_IsUiThreadRequests_AndCorrelatesWithItsEvent()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 120; i++)
            {
                bool text = i % 12 == 6;
                if (text)
                    t.NextVsync += period;
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true, 3.0, 1.0, text ? 12.0 : 0.2);
                if (text)
                {
                    GHFrameRecord r = t.Records[idx];
                    r.ContentEvents = GHContentEvent.FloatingText;
                    r.RequestTicks = Ms(14);
                    t.Records[idx] = r;
                }
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(10, s.HitchCount);
            Assert.Equal(GHHitchCause.UiThreadRequests, DominantCause(s));
            Assert.Equal(TotalAttributed(s), s.CauseCount[(int)GHHitchCause.UiThreadRequests]);
            Assert.Equal(10, s.EventGapCount[0]);
            Assert.Equal(10, s.EventHitchCount[0]);
            Assert.Equal(0, s.QuietHitchCount);
            Assert.True(s.QuietGapCount > 90, "quiet gaps " + s.QuietGapCount);
        }

        /* Floating texts that arrive without slowing anything are not hitches */
        [Fact]
        public void HarmlessEvents_DoNotCorrelate()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 120; i++)
            {
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true);
                if (i % 12 == 6)
                {
                    GHFrameRecord r = t.Records[idx];
                    r.ContentEvents = GHContentEvent.FloatingText;
                    r.RequestTicks = Ms(0.5);
                    t.Records[idx] = r;
                }
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(0, s.HitchCount);
            Assert.Equal(10, s.EventGapCount[0]);
            Assert.Equal(0, s.EventHitchCount[0]);
            Assert.Equal("FloatingText+Message", GHSmoothnessMetrics.ContentEventNames(GHContentEvent.FloatingText | GHContentEvent.Message));
        }

        /* A long map paint on the UI thread delays the next callback: the paint is charged, not the thread */
        [Fact]
        public void LongUiThreadPaint_IsPaintCpuNotUiThread()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 120; i++)
            {
                bool longPaint = i % 20 == 10;
                bool afterLong = i % 20 == 11;
                if (afterLong)
                    t.NextVsync += period;
                t.Tick(period, 60, 60, GHPacingDecision.Rendered, true, longPaint ? 25.0 : 3.0, 1.0, afterLong ? 11.0 : 0.2);
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount >= 5, "hitches " + s.HitchCount);
            Assert.Equal(GHHitchCause.PaintCpu, DominantCause(s));
            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.UiThreadLate]);
        }

        /* A frame measured on screen a refresh later than it was ready for is the compositor's */
        [Fact]
        public void MeasuredLateDisplay_IsCompositor()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 120; i++)
            {
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true);
                GHFrameRecord r = t.Records[idx];
                long display = r.VsyncTicks + period;
                if (i % 12 == 6)
                    display += period;
                r.DisplayedAtTicks = display;
                r.PresentSource = GHPresentSource.Measured;
                t.Records[idx] = r;
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(GHPresentSource.Measured, s.PresentSource);
            Assert.True(s.HitchCount >= 9, "hitches " + s.HitchCount);
            Assert.Equal(GHHitchCause.Compositor, DominantCause(s));
        }

        /* Two GL-thread paints ready before the same vsync: only the later one reaches the display */
        [Fact]
        public void TwoPaintsBeforeOneVsync_DropTheEarlier()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            for (int i = 0; i < 60; i++)
            {
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, false);
                if (i % 10 == 5)
                {
                    /* This paint finishes after the next vsync, landing on the same vsync as the next frame */
                    GHFrameRecord r = t.Records[idx];
                    long delay = period;
                    r.DrawEndTicks += delay;
                    r.FlushEndTicks += delay;
                    t.Records[idx] = r;
                }
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(6, s.DroppedCount);
            Assert.Equal(54, n);
        }

        /* A menu in the middle of a run is a pause, not a hitch */
        [Fact]
        public void MenuPause_IsExcludedFromHitches()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            t.Run(1.0, 60, 60, 60);
            for (int i = 0; i < 60; i++)
                t.Tick(period, 60, 60, GHPacingDecision.AuxiliaryCanvas, true);
            t.Run(1.0, 60, 60, 60);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(0, s.HitchCount);
            Assert.Equal(1, s.PausedGapCount);
            Assert.Equal(60.0, s.DisplayedFps, 0);
        }

        [Fact]
        public void CadenceMonitor_ReportsARateDrop()
        {
            GHCadenceMonitor.Reset();
            List<string> changes = new List<string>();
            GHCadenceMonitor.ChangeLog = delegate (string text) { changes.Add(text); };
            try
            {
                long t = 1000 * F;
                long counter = 1;
                long period60 = F / 60;
                /* 5 s at 60 FPS, then 5 s at 30 FPS */
                for (int i = 0; i < 300; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period60, t + Ms(4), 60, counter++);
                    t += period60;
                }
                for (int i = 0; i < 150; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period60, t + Ms(4), 60, counter++);
                    t += 2 * period60;
                }
                Assert.Single(changes);
                Assert.StartsWith("CADENCE 60->30", changes[0]);
                Assert.Equal(30.0, GHCadenceMonitor.DisplayedFps, 0);
            }
            finally
            {
                GHCadenceMonitor.ChangeLog = null;
                GHCadenceMonitor.Reset();
            }
        }
    }
}

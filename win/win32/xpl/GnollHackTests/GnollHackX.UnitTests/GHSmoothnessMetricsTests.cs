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
                RunPeriod(seconds, F / screenHz, assumedHz, targetFps, paintOnUi);
            }

            /* The same on a panel with an exact period, e.g. 59.94 Hz */
            public void RunPeriod(double seconds, long period, int assumedHz, int targetFps, bool paintOnUi = true)
            {
                int count = (int)Math.Round(seconds * F / period);
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

        /* H1 induced on a 60 Hz device: map 40 FPS alternates one and two refreshes. The panel
           runs at 59.94 Hz, so the two-refresh hold (33.37 ms) is longer than T + R/2 would
           allow; it is the longest on-time hold and must not be a hitch. */
        [Fact]
        public void H1_40On60_AlternatesAndIsAttributedToPacing()
        {
            Timeline t = new Timeline();
            t.RunPeriod(3.0, Ms(1000.0 / 59.94), 60, 40);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(40.0, s.DisplayedFps, 0);
            /* Every gap is 16.7 or 33.4 ms against a 25 ms target: 8.3 ms off */
            Assert.Equal(1000.0 / 120.0, s.PacingErrorRmsMs, 1);
            Assert.True(s.JudderPct > 95, "judder " + s.JudderPct);
            Assert.Equal(0, s.HitchCount);
            Assert.Equal(0.0, s.OnScreenPacing.JankPct, 6);
            Assert.Equal(GHHitchCause.PacingPolicy, DominantCause(s));
        }

        /* The same cadence with one rendered tick lost: a three-refresh hold is a hitch */
        [Fact]
        public void H1_40On60_ThreeRefreshHold_IsAHitch()
        {
            Timeline t = new Timeline();
            t.Run(3.0, 60, 60, 40);
            /* A rendered tick right after a modulo skip, well inside the run */
            int lost = -1;
            for (int i = 90; i < t.Records.Count && lost < 0; i++)
            {
                if (t.Records[i].Pacing == GHPacingDecision.Rendered && t.Records[i - 1].Pacing == GHPacingDecision.SkippedModulo)
                    lost = i;
            }
            GHFrameRecord r = t.Records[lost];
            GHFrameRecord blank = new GHFrameRecord();
            blank.FrameId = r.FrameId;
            blank.VsyncTicks = r.VsyncTicks;
            blank.RefreshPeriodTicks = r.RefreshPeriodTicks;
            blank.CallbackStartTicks = r.CallbackStartTicks;
            blank.CallbackEndTicks = r.CallbackEndTicks;
            blank.TargetFps = r.TargetFps;
            blank.AssumedRefreshHz = r.AssumedRefreshHz;
            blank.Pacing = GHPacingDecision.SkippedDivisor;
            t.Records[lost] = blank;

            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(1, s.HitchCount);
            for (int j = 1; j < n; j++)
            {
                if (d[j].IsHitch)
                    Assert.Equal(Ms(50.0), d[j].GapTicks, (double)Ms(0.1));
            }
        }

        /* Cadences of panels this hardware lacks: each alternates holds within the pattern */
        [Theory]
        [InlineData(90, 60)]
        [InlineData(120, 80)]
        public void NonDivisorCadence_JuddersWithoutHitches(int screenHz, int targetFps)
        {
            Timeline t = new Timeline();
            t.Run(3.0, screenHz, screenHz, targetFps);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(targetFps, s.DisplayedFps, 0);
            Assert.Equal(0, s.HitchCount);
            Assert.True(s.JudderPct > 60, "judder " + s.JudderPct);
            Assert.Equal(TotalAttributed(s), s.CauseCount[(int)GHHitchCause.PacingPolicy]);
        }

        /* 60 on 165 Hz: the modulo skip holds a frame four refreshes, past the three that
           T = 2.75 R rounds up to, so the pattern itself produces hitches, all its own */
        [Fact]
        public void Cadence_60On165_PatternHitchesAreThePolicys()
        {
            Timeline t = new Timeline();
            t.Run(3.0, 165, 165, 60);
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount > 0);
            Assert.True(s.JudderPct > 60, "judder " + s.JudderPct);
            Assert.Equal(GHHitchCause.PacingPolicy, DominantCause(s));
            Assert.Equal(TotalAttributed(s), s.CauseCount[(int)GHHitchCause.PacingPolicy]);
        }

        /* An adaptive panel drops from 120 to 60 Hz. The measured period is a running
           median, as in the app, so it lags the change by several ticks, and the pacing
           logic's assumed rate follows the median. The hitches while both lag are the
           display mode's, not the UI thread's. */
        [Fact]
        public void H2_RefreshDropWithLaggingMedian_IsDisplayMode()
        {
            Timeline t = new Timeline();
            List<long> deltas = new List<long>();
            long lastVsync = 0;
            for (int i = 0; i < 360; i++)
            {
                long period = i < 240 ? F / 120 : F / 60;
                if (lastVsync != 0)
                    deltas.Add(t.NextVsync - lastVsync);
                lastVsync = t.NextVsync;
                long measured = MedianOfLast(deltas, 15);
                int assumed = measured > 0 ? (int)Math.Round((double)F / measured) : 120;
                long counter = t.FrameId + 1;
                int idx = t.Tick(period, assumed, 60, Decide(counter, assumed, 60), true);
                GHFrameRecord r = t.Records[idx];
                r.RefreshPeriodTicks = measured;
                t.Records[idx] = r;
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount > 0, "hitches " + s.HitchCount);
            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.UiThreadLate]);
            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.UiThreadLateGc]);
            Assert.Equal(GHHitchCause.DisplayMode, DominantCause(s));
        }

        private static long MedianOfLast(List<long> values, int window)
        {
            if (values.Count == 0)
                return 0;
            List<long> last = values.GetRange(Math.Max(0, values.Count - window), Math.Min(window, values.Count));
            last.Sort();
            return last[last.Count / 2];
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

        /* With pause data, a collection explains a late callback only if it paused long
           enough to make it late: 1 ms does not, 15 ms does */
        [Theory]
        [InlineData(1.0, GHHitchCause.UiThreadLate)]
        [InlineData(15.0, GHHitchCause.UiThreadLateGc)]
        public void GcPause_DecidesWhetherALateCallbackIsGc(double pauseMs, GHHitchCause expected)
        {
            Timeline t = new Timeline();
            long period = F / 60;
            int gc = 0;
            long pause = Ms(100);
            List<int> lateTicks = new List<int>();
            for (int i = 0; i < 120; i++)
            {
                bool stall = i % 30 == 15;
                if (stall)
                {
                    t.NextVsync += period;
                    gc++;
                    pause += Ms(pauseMs);
                }
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true, 3.0, 1.0, stall ? 12.0 : 0.2);
                GHFrameRecord r = t.Records[idx];
                r.GcCount0 = gc;
                r.GcPauseTicks = pause;
                t.Records[idx] = r;
                if (stall)
                    lateTicks.Add(idx);
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.GcPauseDataAvailable);
            Assert.Equal(4 * pauseMs, s.GcPauseMs, 1);
            int judged = 0;
            for (int j = 1; j < n; j++)
            {
                if (!lateTicks.Contains(d[j].RecordIndex))
                    continue;
                Assert.Equal(expected, d[j].Cause);
                judged++;
            }
            Assert.Equal(lateTicks.Count, judged);
        }

        /* The compositor calls the render loop on every other vblank for two seconds while
           the panel keeps its rate and the UI thread is idle: the hitches are the UI
           framework's cadence, from the first one on, not the display mode or the thread */
        [Fact]
        public void CallbacksAtHalfThePanelRate_AreFrameworkCadence()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            List<long> deltas = new List<long>();
            long lastVsync = 0;
            for (int i = 0; i < 360; i++)
            {
                bool throttled = i >= 120 && i < 180;
                long step = throttled ? 2 * period : period;
                if (lastVsync != 0)
                    deltas.Add(t.NextVsync - lastVsync);
                lastVsync = t.NextVsync;
                int idx = t.Tick(step, 60, 60, GHPacingDecision.Rendered, true);
                GHFrameRecord r = t.Records[idx];
                r.RefreshPeriodTicks = period;
                r.CallbackPeriodTicks = deltas.Count > 0 ? MedianOfLast(deltas, 15) : period;
                t.Records[idx] = r;
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.True(s.HitchCount > 50, "hitches " + s.HitchCount);
            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.DisplayMode]);
            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.UiThreadLate]);
            Assert.Equal(TotalAttributed(s), s.CauseCount[(int)GHHitchCause.FrameworkCadence]);
            Assert.Equal(60.0, s.MeasuredRefreshHz, 0);
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

        /* Request work on an on-time tick does not explain a later tick that a collection
           made late: requests are charged only on the late tick itself */
        [Fact]
        public void RequestsOnAnEarlierTick_DoNotTakeTheBlameForAGcLateTick()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            int gc = 0;
            List<int> lateTicks = new List<int>();
            for (int i = 0; i < 120; i++)
            {
                if (i % 20 == 9)
                {
                    /* Skipped, on time, with 9 ms of requests */
                    int skipped = t.Tick(period, 60, 60, GHPacingDecision.SkippedDivisor, true);
                    GHFrameRecord q = t.Records[skipped];
                    q.RequestTicks = Ms(9);
                    q.GcCount0 = gc;
                    t.Records[skipped] = q;
                    continue;
                }
                bool late = i % 20 == 10;
                if (late)
                {
                    t.NextVsync += period;
                    gc++;
                }
                int idx = t.Tick(period, 60, 60, GHPacingDecision.Rendered, true, 3.0, 1.0, late ? 12.0 : 0.2);
                GHFrameRecord r = t.Records[idx];
                r.GcCount0 = gc;
                t.Records[idx] = r;
                if (late)
                    lateTicks.Add(idx);
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.UiThreadRequests]);
            int checkedFrames = 0;
            for (int j = 1; j < n; j++)
            {
                if (!lateTicks.Contains(d[j].RecordIndex))
                    continue;
                Assert.True(d[j].IsHitch);
                Assert.Equal(GHHitchCause.UiThreadLateGc, d[j].Cause);
                checkedFrames++;
            }
            Assert.Equal(lateTicks.Count, checkedFrames);
        }

        /* A long map-lock stall in a gap that also holds a modulo skip is the lock's: the
           skip explains a gap only as long as the pattern's own holds */
        [Fact]
        public void LongStallInAModuloGap_IsNotPacingPolicy()
        {
            Timeline t = new Timeline();
            t.Run(3.0, 60, 60, 40, false);
            int stalled = -1;
            for (int i = 90; i < t.Records.Count && stalled < 0; i++)
            {
                if (t.Records[i].Pacing == GHPacingDecision.Rendered && t.Records[i - 1].Pacing == GHPacingDecision.SkippedModulo)
                    stalled = i;
            }
            long stall = Ms(60);
            GHFrameRecord r = t.Records[stalled];
            r.LockResultTicks += stall;
            r.DrawEndTicks += stall;
            r.FlushEndTicks += stall;
            t.Records[stalled] = r;
            /* The GL thread is busy meanwhile, so the invalidations it misses are coalesced */
            for (int i = stalled + 1; i < t.Records.Count && t.Records[i].PaintStartTicks < r.FlushEndTicks; i++)
            {
                GHFrameRecord c = t.Records[i];
                if (c.Paint != GHPaintOutcome.Painted)
                    continue;
                c.Paint = GHPaintOutcome.Coalesced;
                c.PaintStartTicks = 0;
                c.LockAttemptTicks = 0;
                c.LockResultTicks = 0;
                c.DrawEndTicks = 0;
                c.FlushEndTicks = 0;
                c.PaintedMainCounter = 0;
                t.Records[i] = c;
            }
            GHDisplayedFrame[] d;
            int n;
            t.Analyze(out d, out n);

            GHDisplayedFrame hit = default(GHDisplayedFrame);
            for (int j = 0; j < n; j++)
            {
                if (d[j].RecordIndex == stalled)
                    hit = d[j];
            }
            Assert.True(hit.IsHitch);
            Assert.Equal(GHHitchCause.GameLock, hit.Cause);
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

        /* A long UI-thread paint that stays under the draw and flush budgets can still make
           the next callback late at a half-rate target: the paint is charged, not the thread */
        [Fact]
        public void LongUiThreadPaintAtHalfRate_DelaysTheCallback_IsPaintCpu()
        {
            Timeline t = new Timeline();
            long period = F / 60;
            bool delayNext = false;
            for (int i = 0; i < 180; i++)
            {
                long counter = t.FrameId + 1;
                GHPacingDecision pacing = Decide(counter, 60, 30);
                bool longPaint = pacing == GHPacingDecision.Rendered && i % 30 == 11;
                /* 20 ms draw and 6 ms flush: under 3T/4 and T/2 at T = 33.3 ms, but past the
                   next (skipped) tick's vsync by 10 ms, which delays its callback */
                t.Tick(period, 60, 30, pacing, true, longPaint ? 20.0 : 3.0, longPaint ? 6.0 : 1.0,
                       delayNext ? 10.0 : 0.2);
                delayNext = longPaint;
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(0, s.CauseCount[(int)GHHitchCause.UiThreadLate]);
            int judged = 0;
            for (int j = 1; j < n; j++)
            {
                GHFrameRecord prev = t.Records[d[j - 1].RecordIndex];
                if (prev.DrawEndTicks - prev.PaintStartTicks < Ms(19))
                    continue;
                /* The frame after the long one: its gap holds the late callback */
                Assert.True(d[j].IsJudder || d[j].IsHitch);
                Assert.Equal(GHHitchCause.PaintCpu, d[j].Cause);
                judged++;
            }
            Assert.True(judged >= 5, "judged " + judged);
        }

        /* A frame measured on screen later than the vsync it was ready for is the
           compositor's. It is shown off the grid, so no frame is dropped and the late
           display itself decides. */
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
                    display += period * 3 / 4;
                r.DisplayedAtTicks = display;
                r.PresentSource = GHPresentSource.Measured;
                t.Records[idx] = r;
            }
            GHDisplayedFrame[] d;
            int n;
            GHSmoothnessSummary s = t.Analyze(out d, out n);

            Assert.Equal(GHPresentSource.Measured, s.PresentSource);
            Assert.Equal(0, s.DroppedCount);
            Assert.True(s.HitchCount >= 9, "hitches " + s.HitchCount);
            for (int j = 1; j < n; j++)
            {
                if (d[j].IsHitch)
                    Assert.Equal(GHHitchCause.Compositor, d[j].Cause);
            }
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
                    GHCadenceMonitor.OnPaintCompleted(t, period60, period60, t + Ms(4), 60, counter++);
                    t += period60;
                }
                for (int i = 0; i < 150; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period60, period60, t + Ms(4), 60, counter++);
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

        /* Callbacks at half the panel's rate for two seconds: CALLBACKS reports the drop and
           the return, and REFRESH stays silent because the panel did not change */
        [Fact]
        public void CadenceMonitor_CallbackDropIsCallbacksNotRefresh()
        {
            GHCadenceMonitor.Reset();
            List<string> changes = new List<string>();
            GHCadenceMonitor.ChangeLog = delegate (string text) { changes.Add(text); };
            try
            {
                long t = 1000 * F;
                long counter = 1;
                long period = F / 144;
                for (int i = 0; i < 720; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period, period, t + Ms(2), 144, counter++);
                    t += period;
                }
                for (int i = 0; i < 144; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period, 2 * period, t + Ms(2), 144, counter++);
                    t += 2 * period;
                }
                for (int i = 0; i < 720; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period, period, t + Ms(2), 144, counter++);
                    t += period;
                }
                Assert.DoesNotContain(changes, c => c.StartsWith("REFRESH"));
                List<string> callbacks = changes.FindAll(c => c.StartsWith("CALLBACKS"));
                Assert.Equal(2, callbacks.Count);
                Assert.StartsWith("CALLBACKS 6.9->13.9 ms", callbacks[0]);
                Assert.StartsWith("CALLBACKS 13.9->6.9 ms", callbacks[1]);
            }
            finally
            {
                GHCadenceMonitor.ChangeLog = null;
                GHCadenceMonitor.Reset();
            }
        }

        /* A panel change the callbacks follow, as on a platform without a separate panel
           period: REFRESH reports it and CALLBACKS stays silent */
        [Fact]
        public void CadenceMonitor_PanelChangeIsRefreshNotCallbacks()
        {
            GHCadenceMonitor.Reset();
            List<string> changes = new List<string>();
            GHCadenceMonitor.ChangeLog = delegate (string text) { changes.Add(text); };
            try
            {
                long t = 1000 * F;
                long counter = 1;
                long p120 = F / 120, p60 = F / 60;
                for (int i = 0; i < 600; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, p120, p120, t + Ms(2), 60, counter++);
                    t += p120;
                }
                for (int i = 0; i < 300; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, p60, p60, t + Ms(2), 60, counter++);
                    t += p60;
                }
                Assert.Single(changes, c => c.StartsWith("REFRESH 8.3->16.7"));
                Assert.DoesNotContain(changes, c => c.StartsWith("CALLBACKS"));
            }
            finally
            {
                GHCadenceMonitor.ChangeLog = null;
                GHCadenceMonitor.Reset();
            }
        }

        /* Three paints land on one vsync, then two refreshes later one paint, then one
           refresh later three again. Only the last paint of a vsync is shown, as in Analyze:
           the two-refresh gap advances one step (+R) and the one-refresh gap three (-2R), an
           RMS of R * sqrt(2.5), 26 ms. Judging each vsync by its first paint would give
           R / sqrt(2), 12 ms. A 250 ms bucket holds an uneven count of the two gaps, so the
           reading lands near, not on, 26 ms. */
        [Fact]
        public void CadenceMonitor_SeveralPaintsOnOneVsync_JudgesTheLastOne()
        {
            GHCadenceMonitor.Reset();
            try
            {
                long t = 1000 * F;
                long counter = 1;
                long period60 = F / 60;
                for (int i = 0; i < 40; i++)
                {
                    GHCadenceMonitor.OnPaintCompleted(t, period60, period60, t + Ms(2), 60, counter++);
                    GHCadenceMonitor.OnPaintCompleted(t, period60, period60, t + Ms(3), 60, counter++);
                    GHCadenceMonitor.OnPaintCompleted(t, period60, period60, t + Ms(4), 60, counter++);
                    t += 2 * period60;
                    GHCadenceMonitor.OnPaintCompleted(t, period60, period60, t + Ms(2), 60, counter++);
                    t += period60;
                }
                Assert.InRange(GHCadenceMonitor.PacingErrorRmsMs, 24.0, 29.0);
            }
            finally
            {
                GHCadenceMonitor.Reset();
            }
        }
    }
}

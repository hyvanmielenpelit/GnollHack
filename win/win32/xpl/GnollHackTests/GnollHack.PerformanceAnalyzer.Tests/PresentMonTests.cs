using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class PresentMonTests
    {
        /* false: PresentMon 1.x fixture (presentmon_v1.csv); true: 2.x (presentmon_v2.csv).
           Both are generated from the same underlying presents, so a join against either
           produces the same result; only the column names differ. */
        private static string PathFor(bool v2)
        {
            return v2 ? TestPaths.PresentMonV2 : TestPaths.PresentMonV1;
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void ReadPresents_KeepsTheBusiestSwapChainOfTheProcess(bool v2)
        {
            PresentMonCapture cap = PresentMonCsv.ReadPresents(PathFor(v2), "GnollHackM");

            Assert.True(cap.HasQpc);
            Assert.Equal(v2 ? "CPUStartQPC" : "QPCTime", cap.TimeColumn);
            Assert.False(cap.TimesInMs);
            Assert.Equal(v2 ? "DisplayLatency" : "MsUntilDisplayed", cap.DisplayColumn);
            Assert.Equal(45, cap.RowsAfterProcessFilter);
            Assert.Equal(TestPaths.SwapChainA, cap.KeptSwapChain);
            Assert.Equal(40, cap.Presents.Count);
            Assert.All(cap.Presents, p => Assert.Equal(TestPaths.SwapChainA, p.SwapChain));
            KeyValuePair<string, int> other = Assert.Single(cap.OtherSwapChains);
            Assert.Equal(TestPaths.SwapChainB, other.Key);
            Assert.Equal(5, other.Value);
            Assert.Equal(1, cap.Presents.Count(p => !p.Displayed));
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void ReadPresents_WithoutProcessFilter_ReportsEveryOtherSwapChain(bool v2)
        {
            PresentMonCapture cap = PresentMonCsv.ReadPresents(PathFor(v2), null);
            Assert.Equal(48, cap.RowsAfterProcessFilter);
            Assert.Equal(TestPaths.SwapChainA, cap.KeptSwapChain);
            Assert.Equal(40, cap.Presents.Count);
            Assert.Equal(2, cap.OtherSwapChains.Count);
            Assert.Equal(8, cap.OtherSwapChains.Sum(kv => kv.Value));
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void Read_SeriesComesFromTheKeptSwapChainOnly(bool v2)
        {
            Series s = PresentMonCsv.Read(PathFor(v2), "GnollHackM");
            Assert.Equal(v2 ? "DisplayedTime" : "MsBetweenDisplayChange", s.Column);
            Assert.Equal(TestPaths.SwapChainA, s.Info["swapChain"]);
            Assert.Equal("5", s.Info["otherSwapChainPresents"]);
            /* 40 rows: the first is skipped and the undisplayed one has no interval. The two
               long ones are the slow paint of frame 21 and the dropped frame 31. */
            Assert.Equal(38, s.IntervalsMs.Length);
            Assert.Equal(2, s.IntervalsMs.Count(v => v > 30));
            Assert.All(s.IntervalsMs, v => Assert.InRange(v, 16.0f, 34.0f));
            /* v1 has an explicit Dropped column; v2 has none, so the unparsable (NA) display
               field on the same row is what counts it as dropped */
            Assert.Equal(1, s.DroppedCount);
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void Join_MeasuredPresentsReplaceTheEstimate(bool v2)
        {
            SmoothnessResult r = SmoothnessCommand.Analyze(TestPaths.Run1Json, PathFor(v2), "GnollHackM", null);
            JoinReport j = Assert.Single(r.Joins);
            Assert.True(j.Available);
            Assert.Equal(40, j.PaintedFrames);
            Assert.Equal(40, j.Matched);
            Assert.Equal(0, j.Unmatched);
            Assert.Equal(1, j.DroppedPresents);
            Assert.Equal(0, j.DisplayedBeforeReady);

            GHSmoothnessSummary s = r.Summary;
            Assert.Equal(GHPresentSource.Measured, s.PresentSource);
            Assert.Equal(39, r.DisplayedCount);
            Assert.Equal(1, s.DroppedCount);
            Assert.Equal(2, s.HitchCount);
            Assert.Equal(1, s.CauseCount[(int)GHHitchCause.PaintCpu]);
            Assert.Equal(1, s.CauseCount[(int)GHHitchCause.Compositor]);

            /* Frame 1: flush at 4.3 ms, estimated on the vsync at 16.467 ms, measured 6 ms later */
            Assert.Equal(22.467, r.Timeline.Clock.TicksToMs(r.Timeline.Records[0].DisplayedAtTicks), 3);
            Assert.Equal(GHPresentSource.Measured, r.Timeline.Records[0].PresentSource);
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public void Join_WithoutOrigin_IsUnavailable(bool v2)
        {
            CapturedTimeline t = FrameTimelineCsv.Read(TestPaths.Run1Timeline);
            t.Clock = new TimelineClock(t.Clock.DeviceFrequency, null);
            JoinReport j = FrameJoin.ApplyPresentMon(t, PresentMonCsv.ReadPresents(PathFor(v2), "GnollHackM"));
            Assert.False(j.Available);
            Assert.Contains("OriginStopwatchTicks", j.UnavailableReason);
            Assert.All(t.Records.Take(t.Count), rec => Assert.Equal(GHPresentSource.None, rec.PresentSource));
        }
    }
}

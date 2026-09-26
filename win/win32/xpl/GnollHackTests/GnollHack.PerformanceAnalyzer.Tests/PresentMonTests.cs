using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class PresentMonTests
    {
        [Fact]
        public void ReadPresents_KeepsTheBusiestSwapChainOfTheProcess()
        {
            PresentMonCapture cap = PresentMonCsv.ReadPresents(TestPaths.PresentMon, "GnollHackM");

            Assert.True(cap.HasQpc);
            Assert.Equal("CPUStartQPC", cap.TimeColumn);
            Assert.False(cap.TimesInMs);
            Assert.Equal("MsUntilDisplayed", cap.DisplayColumn);
            Assert.Equal(45, cap.RowsAfterProcessFilter);
            Assert.Equal(TestPaths.SwapChainA, cap.KeptSwapChain);
            Assert.Equal(40, cap.Presents.Count);
            Assert.All(cap.Presents, p => Assert.Equal(TestPaths.SwapChainA, p.SwapChain));
            KeyValuePair<string, int> other = Assert.Single(cap.OtherSwapChains);
            Assert.Equal(TestPaths.SwapChainB, other.Key);
            Assert.Equal(5, other.Value);
            Assert.Equal(1, cap.Presents.Count(p => !p.Displayed));
        }

        [Fact]
        public void ReadPresents_WithoutProcessFilter_ReportsEveryOtherSwapChain()
        {
            PresentMonCapture cap = PresentMonCsv.ReadPresents(TestPaths.PresentMon, null);
            Assert.Equal(48, cap.RowsAfterProcessFilter);
            Assert.Equal(TestPaths.SwapChainA, cap.KeptSwapChain);
            Assert.Equal(40, cap.Presents.Count);
            Assert.Equal(2, cap.OtherSwapChains.Count);
            Assert.Equal(8, cap.OtherSwapChains.Sum(kv => kv.Value));
        }

        [Fact]
        public void Read_SeriesComesFromTheKeptSwapChainOnly()
        {
            Series s = PresentMonCsv.Read(TestPaths.PresentMon, "GnollHackM");
            Assert.Equal("MsBetweenDisplayChange", s.Column);
            Assert.Equal(TestPaths.SwapChainA, s.Info["swapChain"]);
            Assert.Equal("5", s.Info["otherSwapChainPresents"]);
            /* 40 rows: the first is skipped and the undisplayed one has no interval. The two
               long ones are the slow paint of frame 21 and the dropped frame 31. */
            Assert.Equal(38, s.IntervalsMs.Length);
            Assert.Equal(2, s.IntervalsMs.Count(v => v > 30));
            Assert.All(s.IntervalsMs, v => Assert.InRange(v, 16.0f, 34.0f));
        }

        [Fact]
        public void Join_MeasuredPresentsReplaceTheEstimate()
        {
            SmoothnessResult r = SmoothnessCommand.Analyze(TestPaths.Run1Json, TestPaths.PresentMon, "GnollHackM", null);
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

        [Fact]
        public void Join_WithoutOrigin_IsUnavailable()
        {
            CapturedTimeline t = FrameTimelineCsv.Read(TestPaths.Run1Timeline);
            t.Clock = new TimelineClock(t.Clock.DeviceFrequency, null);
            JoinReport j = FrameJoin.ApplyPresentMon(t, PresentMonCsv.ReadPresents(TestPaths.PresentMon, "GnollHackM"));
            Assert.False(j.Available);
            Assert.Contains("OriginStopwatchTicks", j.UnavailableReason);
            Assert.All(t.Records.Take(t.Count), rec => Assert.Equal(GHPresentSource.None, rec.PresentSource));
        }
    }
}

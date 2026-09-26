using System.Diagnostics;
using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class FrameTimelineTests
    {
        private static double LocalMs(long ticks)
        {
            return ticks * 1000.0 / Stopwatch.Frequency;
        }

        [Fact]
        public void Read_ParsesRecordsOnTheLocalClock()
        {
            CapturedTimeline t = FrameTimelineCsv.Read(TestPaths.Run1Timeline);

            Assert.Equal(40, t.Count);
            Assert.Equal(10000000, t.Clock.DeviceFrequency);
            Assert.Equal(123456780000L, t.Clock.OriginDeviceTicks);
            for (int i = 0; i < t.Count; i++)
                Assert.Equal(i + 1, t.Records[i].FrameId);

            GHFrameRecord first = t.Records[0];
            Assert.NotEqual(0, first.VsyncTicks);
            Assert.NotEqual(0, first.CallbackStartTicks);
            Assert.Equal(0.0, t.Clock.TicksToMs(first.CallbackStartTicks), 6);
            Assert.Equal(-0.2, t.Clock.TicksToMs(first.VsyncTicks), 6);
            Assert.Equal(16.667, LocalMs(first.RefreshPeriodTicks), 6);
            Assert.Equal(GHPacingDecision.Rendered, first.Pacing);
            Assert.Equal(GHInvalidateOutcome.Invalidated, first.Invalidate);
            Assert.Equal(GHPaintOutcome.Painted, first.Paint);
            Assert.Equal(GHPresentSource.None, first.PresentSource);
            Assert.Equal(0, first.DisplayedAtTicks);
            Assert.True(first.PaintOnUiThread);
            Assert.True(first.LockAcquired);
            Assert.Equal(60, first.TargetFps);
            Assert.Equal(60, first.AssumedRefreshHz);

            GHFrameRecord slow = t.Records[20];
            Assert.Equal(28.0, LocalMs(slow.DrawEndTicks - slow.PaintStartTicks), 6);
            Assert.False(t.Records[36].PaintOnUiThread);
            Assert.Equal(6, t.Records[30].GcCount0);
        }

        [Fact]
        public void Analyze_Run1_OneHitchChargedToPaintCpu()
        {
            SmoothnessResult r = SmoothnessCommand.Analyze(TestPaths.Run1Json, null, null, null);
            GHSmoothnessSummary s = r.Summary;

            Assert.Equal(42, r.Timeline.CompositorCount);
            Assert.Equal(40, s.TickCount);
            Assert.Equal(40, s.PaintedCount);
            Assert.Equal(40, r.DisplayedCount);
            Assert.Equal(0, s.DroppedCount);
            Assert.Equal(1, s.HitchCount);
            Assert.Equal(1, s.JudderCount);
            Assert.Equal(1, s.CauseCount[(int)GHHitchCause.PaintCpu]);
            Assert.Equal(1, s.CauseCount.Sum());
            Assert.Equal(GHPresentSource.Estimated, s.PresentSource);
            Assert.Equal(58.5, s.DisplayedFps, 1);
            Assert.Equal(25.0, s.HitchRatioMsPerSec, 1);
            Assert.False(s.AssumedRefreshMismatch);
            Assert.Equal(0.0, s.UnattributedShare, 6);

            GHDisplayedFrame hitch = r.Displayed.Take(r.DisplayedCount).Single(d => d.IsHitch);
            Assert.Equal(21, hitch.FrameId);
            Assert.Equal(GHHitchCause.PaintCpu, hitch.Cause);
            Assert.Equal(33.334, LocalMs(hitch.GapTicks), 2);
        }

        [Fact]
        public void WriteThenRead_RoundTripsEveryField()
        {
            CapturedTimeline a = FrameTimelineCsv.Read(TestPaths.Run1Timeline);
            string tmp = TestPaths.TempFile(".csv");
            try
            {
                FrameTimelineCsv.Write(tmp, a);
                CapturedTimeline b = FrameTimelineCsv.Read(tmp);
                Assert.Equal(a.Count, b.Count);
                Assert.Equal(a.Clock.DeviceFrequency, b.Clock.DeviceFrequency);
                Assert.Equal(a.Clock.OriginDeviceTicks, b.Clock.OriginDeviceTicks);
                for (int i = 0; i < a.Count; i++)
                    Assert.Equal(a.Records[i], b.Records[i]);
                Assert.Equal(File.ReadAllText(TestPaths.Run1Timeline).Replace("\r\n", "\n"), File.ReadAllText(tmp).Replace("\r\n", "\n"));
            }
            finally
            {
                File.Delete(tmp);
            }
        }

        [Fact]
        public void Read_WithoutFrequencyLine_UsesTheFallback()
        {
            string[] lines = File.ReadAllLines(TestPaths.Run1Timeline)
                .Where(l => !l.StartsWith("# StopwatchFrequency") && !l.StartsWith("# OriginStopwatchTicks")).ToArray();
            string tmp = TestPaths.TempFile(".csv");
            try
            {
                File.WriteAllLines(tmp, lines);
                CapturedTimeline t = FrameTimelineCsv.Read(tmp, 24000000);
                Assert.Equal(24000000, t.Clock.DeviceFrequency);
                Assert.Null(t.Clock.OriginDeviceTicks);
                Assert.Equal(40, t.Count);
            }
            finally
            {
                File.Delete(tmp);
            }
        }
    }
}

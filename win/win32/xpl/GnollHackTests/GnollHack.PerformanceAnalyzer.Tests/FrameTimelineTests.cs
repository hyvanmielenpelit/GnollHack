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

        /* A capture from before GcPauseMs and CallbackPeriodMs: both read as 0, so the
           analysis has no pause data and judges GC by the counts */
        [Fact]
        public void Read_WithoutPauseAndCallbackColumns_ReadsZeros()
        {
            string[] lines = File.ReadAllLines(TestPaths.Run1Timeline).Select(l =>
                l.StartsWith("#") ? l : l.Substring(0, l.LastIndexOf(',', l.LastIndexOf(',') - 1))).ToArray();
            Assert.EndsWith("RequestMs,ContentEvents", lines.First(l => l.StartsWith("FrameId")));
            string tmp = TestPaths.TempFile(".csv");
            try
            {
                File.WriteAllLines(tmp, lines);
                CapturedTimeline t = FrameTimelineCsv.Read(tmp);
                Assert.Equal(40, t.Count);
                Assert.All(t.Records.Take(t.Count), r => Assert.Equal(0, r.GcPauseTicks));
                Assert.All(t.Records.Take(t.Count), r => Assert.Equal(0, r.CallbackPeriodTicks));
                GHDisplayedFrame[] displayed = new GHDisplayedFrame[t.Count];
                GHSmoothnessSummary s = GHSmoothnessMetrics.Analyze(t.Records, t.Count, null, 0, displayed, out int n);
                Assert.False(s.GcPauseDataAvailable);
                Assert.Equal(0.0, s.CallbackRefreshHz);
            }
            finally
            {
                File.Delete(tmp);
            }
        }

        [Fact]
        public void WriteThenRead_KeepsPauseAndCallbackPeriod()
        {
            CapturedTimeline a = FrameTimelineCsv.Read(TestPaths.Run1Timeline);
            for (int i = 0; i < a.Count; i++)
            {
                a.Records[i].GcPauseTicks = a.Clock.MsToTicks(100.0 + i) - a.Clock.MsToTicks(0);
                a.Records[i].CallbackPeriodTicks = a.Records[i].RefreshPeriodTicks * 2;
            }
            string tmp = TestPaths.TempFile(".csv");
            try
            {
                FrameTimelineCsv.Write(tmp, a);
                CapturedTimeline b = FrameTimelineCsv.Read(tmp);
                for (int i = 0; i < a.Count; i++)
                {
                    Assert.Equal(LocalMs(a.Records[i].GcPauseTicks), LocalMs(b.Records[i].GcPauseTicks), 3);
                    Assert.Equal(LocalMs(a.Records[i].CallbackPeriodTicks), LocalMs(b.Records[i].CallbackPeriodTicks), 3);
                }
            }
            finally
            {
                File.Delete(tmp);
            }
        }

        [Fact]
        public void Read_OriginUtcLine_RoundTrips()
        {
            const string utcText = "2026-09-26T12:34:56.1234567Z";
            const string originLine = "# OriginUtc utc=" + utcText + " stopwatchTicks=123456780000";
            DateTime expected = new DateTime(2026, 9, 26, 12, 34, 56, DateTimeKind.Utc).AddTicks(1234567);
            List<string> lines = File.ReadAllLines(TestPaths.Run1Timeline).ToList();
            int at = lines.FindIndex(l => l.StartsWith("# OriginStopwatchTicks=")) + 1;
            lines.Insert(at, originLine);
            lines.Insert(at + 1, "# SomeFutureLine alpha=1 beta");
            string tmp = TestPaths.TempFile(".csv");
            string tmp2 = TestPaths.TempFile(".csv");
            try
            {
                File.WriteAllLines(tmp, lines);
                CapturedTimeline a = FrameTimelineCsv.Read(tmp);
                Assert.Equal(40, a.Count);
                Assert.Equal(expected, FrameTimelineCsv.OriginUtc(a));
                Assert.Equal(DateTimeKind.Utc, FrameTimelineCsv.OriginUtc(a).Value.Kind);
                Assert.Equal(123456780000L, FrameTimelineCsv.OriginUtcStopwatchTicks(a));
                Assert.Equal(expected.AddMilliseconds(500), FrameTimelineCsv.UtcAtMs(a, 500));
                Assert.Equal(500.0, FrameTimelineCsv.MsAtUtc(a, expected.AddMilliseconds(500)), 6);

                FrameTimelineCsv.Write(tmp2, a);
                Assert.Equal(File.ReadAllText(tmp).Replace("\r\n", "\n"), File.ReadAllText(tmp2).Replace("\r\n", "\n"));

                /* a timeline with no source lines writes the line from its metadata */
                a.MetadataLines.Clear();
                FrameTimelineCsv.Write(tmp2, a);
                Assert.Contains(originLine, File.ReadAllLines(tmp2));
                CapturedTimeline b = FrameTimelineCsv.Read(tmp2);
                Assert.Equal(expected, FrameTimelineCsv.OriginUtc(b));
                Assert.Equal(123456780000L, FrameTimelineCsv.OriginUtcStopwatchTicks(b));
            }
            finally
            {
                File.Delete(tmp);
                File.Delete(tmp2);
            }
        }

        /* OriginUtc read one second after capture time 0 sits at 1000 ms */
        [Fact]
        public void UtcAtMs_PlacesOriginUtcAtItsStopwatchTick()
        {
            DateTime utc = new DateTime(2026, 9, 26, 12, 0, 0, DateTimeKind.Utc);
            List<string> lines = File.ReadAllLines(TestPaths.Run1Timeline).ToList();
            lines.Insert(lines.FindIndex(l => l.StartsWith("# OriginStopwatchTicks=")) + 1,
                "# OriginUtc utc=" + utc.ToString("o") + " stopwatchTicks=123466780000");
            string tmp = TestPaths.TempFile(".csv");
            try
            {
                File.WriteAllLines(tmp, lines);
                CapturedTimeline t = FrameTimelineCsv.Read(tmp);
                Assert.Equal(utc, FrameTimelineCsv.UtcAtMs(t, 1000));
                Assert.Equal(utc.AddSeconds(-1), FrameTimelineCsv.UtcAtMs(t, 0));
            }
            finally
            {
                File.Delete(tmp);
            }
        }

        [Fact]
        public void Read_WithoutOriginUtcLine_HasNoWallClock()
        {
            CapturedTimeline t = FrameTimelineCsv.Read(TestPaths.Run1Timeline);
            Assert.Equal(40, t.Count);
            Assert.Null(FrameTimelineCsv.OriginUtc(t));
            Assert.Null(FrameTimelineCsv.OriginUtcStopwatchTicks(t));
            Assert.Null(FrameTimelineCsv.UtcAtMs(t, 0));
            Assert.True(double.IsNaN(FrameTimelineCsv.MsAtUtc(t, DateTime.UtcNow)));
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

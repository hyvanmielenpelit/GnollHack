using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class PerfettoTests
    {
        [Fact]
        public void Read_ParsesQuotedValuesAndNulls()
        {
            PerfettoCapture p = PerfettoFrames.ReadDirectory(TestPaths.PerfettoDir);
            Assert.Equal(40, p.Frames.Count);
            Assert.Equal(81, p.Slices.Count);
            Assert.Equal(0, p.SkippedFrameRows);

            PerfettoFrame f = p.Frames[0];
            Assert.Equal(1000, f.Token);
            Assert.Equal("On-time Present", f.PresentType);
            Assert.Equal("None", f.JankType);
            Assert.Equal("TX - com.hyvanmielenpelit.gnollhack/crc64.MainActivity#0", f.LayerName);
            Assert.Equal(16666667L, f.ExpectedDur);

            PerfettoSlice paint = p.Slices.First(s => s.Name == "GH.Paint");
            Assert.Equal(1L, paint.FrameId);
            Assert.Null(paint.VsyncToken);
            PerfettoSlice orphan = p.Slices.Last();
            Assert.Equal("GH.Tick", orphan.Name);
            Assert.Null(orphan.FrameId);
        }

        [Fact]
        public void Join_UiThreadFramesTakeSurfaceFlingerPresentTimes()
        {
            SmoothnessResult r = SmoothnessCommand.Analyze(TestPaths.Run1Json, null, null, TestPaths.PerfettoDir);
            JoinReport j = Assert.Single(r.Joins);
            Assert.True(j.Available);
            Assert.Equal(40, j.OffsetSamples);
            Assert.Equal(-5000000.0, j.OffsetMs, 3);
            Assert.Equal(40, j.PaintedFrames);
            Assert.Equal(36, j.Matched);
            Assert.Equal(4, j.GlThreadLeftEstimated);
            Assert.Equal(4, j.Unmatched);
            Assert.Equal(0, j.DroppedPresents);
            Assert.Equal("App Deadline Missed", j.JankTypeByFrameId[21]);
            Assert.Equal("Late Present", j.PresentTypeByFrameId[21]);

            CapturedTimeline t = r.Timeline;
            Assert.Equal(21.467, t.Clock.TicksToMs(t.Records[0].DisplayedAtTicks), 3);
            Assert.Equal(GHPresentSource.Measured, t.Records[35].PresentSource);
            Assert.Equal(GHPresentSource.None, t.Records[36].PresentSource);

            Assert.Equal(GHPresentSource.Estimated, r.Summary.PresentSource);
            Assert.Equal(1, r.Summary.HitchCount);
            GHDisplayedFrame hitch = r.Displayed.Take(r.DisplayedCount).Single(d => d.IsHitch);
            Assert.Equal(21, hitch.FrameId);
            Assert.Equal(GHHitchCause.PaintCpu, hitch.Cause);
            Assert.Equal(GHPresentSource.Measured, hitch.Source);
        }
    }
}

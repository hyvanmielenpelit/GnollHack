using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class CommandTests
    {
        [Fact]
        public void RunRecordLoad_ConvertsAnInAppRecord()
        {
            RunRecord r = RunRecord.Load(TestPaths.Run1Json);
            Assert.Equal(1, r.SchemaVersion);
            Assert.Equal("run1-sample", r.Id);
            Assert.Equal("W1", r.Scenario);
            Assert.Equal("Windows", r.Platform);
            Assert.Equal("Release", r.BuildConfiguration);
            Assert.Equal("Sample Laptop", r.Device.Model);
            Assert.Equal("charging", r.Thermal.PowerState);
            Assert.False(r.Thermal.Throttled);
            Assert.Equal(59.998, r.Display.RefreshHz, 3);

            Series s = r.FindSeries(MetricNames.SmoothnessSeries);
            Assert.NotNull(s);
            Assert.Equal(58.5, s.Metrics[MetricNames.DisplayedFps], 6);
            Assert.Equal(25.0, s.Metrics[MetricNames.HitchRatioMsPerSec], 6);
            Assert.Equal(2.669, s.Metrics[MetricNames.PacingErrorRmsMs], 6);
            Assert.Equal(39, s.IntervalsMs.Length);
            Assert.Equal(39, (int)s.Metrics[MetricNames.FrameCount]);
            Assert.Equal("Estimated", s.Info["presentSource"]);
        }

        [Fact]
        public void Compare_SmoothnessSeries_DecidesOnTheInAppMetrics()
        {
            string outPath = TestPaths.TempFile(".md");
            try
            {
                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", TestPaths.Run1Json, "--b", TestPaths.Run2Json,
                    "--series", "smoothness", "--resamples", "200", "--out", outPath
                }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                Assert.Contains("**hitchRatioMsPerSec** (ms/s)", md);
                Assert.Contains("**pacingErrorRmsMs** (ms)", md);
                Assert.Contains("**displayedFps** (fps)", md);
                Assert.Contains("| Run | Displayed | Window s | Displayed FPS |", md);
                Assert.Contains("Interval level", md);
            }
            finally
            {
                File.Delete(outPath);
            }
        }

        [Fact]
        public void Compare_ExternalSeriesOfInAppRecords_HasNoUsableRuns()
        {
            int code = CompareCommand.Run(new Args(new[] { "compare", "--a", TestPaths.Run1Json, "--b", TestPaths.Run2Json }));
            Assert.Equal(1, code);
        }

        [Fact]
        public void Args_CollectsPositionalValuesBeforeTheFirstOption()
        {
            Args a = new Args(new[] { "smoothness", "run.json", "--out", "r.md" });
            Assert.Equal("smoothness", a.Command);
            Assert.Equal(new[] { "run.json" }, a.Positional);
            Assert.Equal("r.md", a.Get("out"));
        }

        [Fact]
        public void Smoothness_WritesTheReport()
        {
            string outPath = TestPaths.TempFile(".md");
            try
            {
                int code = SmoothnessCommand.Run(new Args(new[]
                {
                    "smoothness", TestPaths.Run1Json, "--presentmon", TestPaths.PresentMon, "--process", "GnollHackM", "--out", outPath
                }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                Assert.Contains("## Headline", md);
                Assert.Contains("critical (> 10)", md);
                Assert.Contains("## Hitch causes", md);
                Assert.Contains("| PaintCpu | 1 |", md);
                Assert.Contains("| Compositor | 1 |", md);
                Assert.Contains("## Worst hitches", md);
                Assert.Contains("### 1. Frame", md);
                Assert.Contains("dropped", md);
                Assert.Contains("## Change points", md);
                Assert.Contains("too short", md);
                Assert.Contains("Thermal state: Nominal before, Nominal after", md);
                Assert.DoesNotContain(((char)10).ToString(), md.Replace(Csv.Crlf, ""));
            }
            finally
            {
                File.Delete(outPath);
            }
        }
    }
}

using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class CommandTests
    {
        /* A hand-built v1 run record with just enough of a "smoothness" series to pass
           the compare command's decision-metric gate: HitchRatioMsPerSec, PacingErrorRmsMs
           and DisplayedFps, plus whatever reported metrics the caller adds. */
        private static RunRecord SyntheticSmoothnessRun(string arm, double hitchRatio, double pacingRms, double displayedFps,
            double targetPeriodMs = 16.667, string buildConfiguration = "Release")
        {
            RunRecord r = new RunRecord
            {
                Id = Guid.NewGuid().ToString("N"),
                TimestampUtc = DateTime.UtcNow.ToString("o"),
                Scenario = "W1",
                ScenarioKind = "gameplay",
                Arm = arm,
                BuildConfiguration = buildConfiguration,
                Platform = "Windows"
            };
            r.Display.RefreshHz = 60;
            r.Display.VsyncMs = 1000.0 / 60.0;
            r.Display.TargetFps = 60;
            r.Display.TargetPeriodMs = targetPeriodMs;
            Series s = new Series { Kind = MetricNames.SmoothnessSeries, Source = "in-app", IntervalsMs = new float[0] };
            s.Metrics[MetricNames.HitchRatioMsPerSec] = hitchRatio;
            s.Metrics[MetricNames.PacingErrorRmsMs] = pacingRms;
            s.Metrics[MetricNames.DisplayedFps] = displayedFps;
            r.Series.Add(s);
            return r;
        }

        private static string SaveTemp(RunRecord r)
        {
            string path = TestPaths.TempFile(".json");
            r.Save(path);
            return path;
        }

        private static void DeleteAll(IEnumerable<string> paths)
        {
            foreach (string p in paths)
            {
                try { File.Delete(p); } catch (IOException) { }
            }
        }

        /* One-sided Fisher exact test on [a b; c d]: hand-checkable hypergeometric tails */
        [Theory]
        [InlineData(3, 0, 0, 3, 0.05)]          /* 1 / C(6,3) */
        [InlineData(0, 3, 3, 0, 1.0)]           /* the whole distribution */
        [InlineData(2, 1, 1, 2, 0.5)]           /* (9 + 1) / 20 */
        public void FisherGreater_MatchesTheHypergeometricTail(int a, int b, int c, int d, double expected)
        {
            Assert.Equal(expected, SmoothnessCommand.FisherGreater(a, b, c, d), 9);
        }

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
                    "--series", "smoothness", "--resamples", "200", "--out", outPath,
                    /* the fixture's 39 on-screen intervals are fewer than ToRunRecord's
                       100-interval floor, so both runs are excluded by design; this test
                       is about the smoothness decision, not the exclusion rule */
                    "--include-excluded"
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
                    "smoothness", TestPaths.Run1Json, "--presentmon", TestPaths.PresentMonV1, "--process", "GnollHackM", "--out", outPath
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

        [Fact]
        public void Compare_ThreeRunsPerArm_RegressesOnHitchRatio()
        {
            List<string> runPaths = new List<string>();
            string outPath = TestPaths.TempFile(".md");
            try
            {
                for (int i = 0; i < 3; i++)
                    runPaths.Add(SaveTemp(SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0)));
                for (int i = 0; i < 3; i++)
                    runPaths.Add(SaveTemp(SyntheticSmoothnessRun("B", 10.0, 0.5, 60.0)));

                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare",
                    "--a", runPaths[0], runPaths[1], runPaths[2],
                    "--b", runPaths[3], runPaths[4], runPaths[5],
                    "--series", "smoothness", "--resamples", "200", "--out", outPath
                }));
                Assert.Equal(0, code);
                string[] lines = File.ReadAllText(outPath).Replace("\r\n", "\n").Split('\n');
                string hitchRow = Assert.Single(lines, l => l.Contains("hitchRatioMsPerSec"));
                Assert.Contains("REGRESSION", hitchRow);
            }
            finally
            {
                DeleteAll(runPaths);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void Compare_OneRunPerArm_SaysTooFewRuns()
        {
            List<string> runPaths = new List<string>
            {
                SaveTemp(SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0)),
                SaveTemp(SyntheticSmoothnessRun("B", 1.5, 0.5, 60.0))
            };
            string outPath = TestPaths.TempFile(".md");
            try
            {
                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", runPaths[0], "--b", runPaths[1],
                    "--series", "smoothness", "--resamples", "200", "--out", outPath
                }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                Assert.Contains("too few runs (need 3)", md);
                Assert.Contains("no decision was made", md);
            }
            finally
            {
                DeleteAll(runPaths);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void Compare_MismatchedCadence_IsRefusedUnlessAllowed()
        {
            List<string> runPaths = new List<string>
            {
                SaveTemp(SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0, targetPeriodMs: 16.667)),
                SaveTemp(SyntheticSmoothnessRun("B", 1.0, 0.5, 60.0, targetPeriodMs: 33.333))
            };
            string outPath = TestPaths.TempFile(".md");
            try
            {
                int refused = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", runPaths[0], "--b", runPaths[1], "--series", "smoothness", "--out", outPath
                }));
                Assert.Equal(1, refused);

                int allowed = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", runPaths[0], "--b", runPaths[1], "--series", "smoothness",
                    "--out", outPath, "--allow-mixed-cadence"
                }));
                Assert.Equal(0, allowed);
            }
            finally
            {
                DeleteAll(runPaths);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void Compare_ReportedMetricMissingFromARun_IsNotTreatedAsZero()
        {
            RunRecord a1 = SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0);
            a1.FindSeries(MetricNames.SmoothnessSeries).Metrics[MetricNames.JudderPct] = 10.0;
            RunRecord a2 = SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0);
            a2.FindSeries(MetricNames.SmoothnessSeries).Metrics[MetricNames.JudderPct] = 10.0;
            RunRecord b1 = SyntheticSmoothnessRun("B", 1.0, 0.5, 60.0);
            b1.FindSeries(MetricNames.SmoothnessSeries).Metrics[MetricNames.JudderPct] = 100.0;
            RunRecord b2 = SyntheticSmoothnessRun("B", 1.0, 0.5, 60.0);
            /* b2 carries no judderPct at all: treating that as zero would pull the median
               of {100, 0} down to 50 instead of the true median of the runs that have it */

            List<string> runPaths = new List<string> { SaveTemp(a1), SaveTemp(a2), SaveTemp(b1), SaveTemp(b2) };
            string outPath = TestPaths.TempFile(".md");
            try
            {
                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", runPaths[0], runPaths[1], "--b", runPaths[2], runPaths[3],
                    "--series", "smoothness", "--resamples", "200", "--out", outPath
                }));
                Assert.Equal(0, code);
                string[] lines = File.ReadAllText(outPath).Replace("\r\n", "\n").Split('\n');
                string row = Assert.Single(lines, l => l.TrimStart().StartsWith("| " + MetricNames.JudderPct + " "));
                Assert.Contains("| 10.00 | 100.00 |", row);
            }
            finally
            {
                DeleteAll(runPaths);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void Compare_ArmDirectoryWithAnEnvFile_LoadsOnlyTheRunRecord()
        {
            string dir = Path.Combine(Path.GetTempPath(), "ghpa_arm_" + Guid.NewGuid().ToString("N"));
            Directory.CreateDirectory(dir);
            string runInDir = Path.Combine(dir, "run.json");
            string otherRun = TestPaths.TempFile(".json");
            string outPath = TestPaths.TempFile(".md");
            try
            {
                SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0).Save(runInDir);
                /* A Get-ThermalState-shaped reading, as sits next to a run in a captured
                   arm directory: an object with no "series" array */
                File.WriteAllText(Path.Combine(dir, "env_before.json"),
                    "{\"status\":\"Nominal\",\"cpuPerformancePct\":95.0}", new System.Text.UTF8Encoding(false));
                SyntheticSmoothnessRun("B", 1.0, 0.5, 60.0).Save(otherRun);

                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", dir, "--b", otherRun, "--series", "smoothness", "--out", outPath
                }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                Assert.Contains("| Runs loaded / used | 1 / 1 | 1 / 1 |", md);
            }
            finally
            {
                Directory.Delete(dir, true);
                File.Delete(otherRun);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void History_DuplicateId_IsSkipped()
        {
            string historyPath = TestPaths.TempFile(".jsonl");
            string runPath = SaveTemp(SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0));
            try
            {
                File.Delete(historyPath);
                int first = HistoryCommand.Run(new Args(new[] { "history", "--file", historyPath, "--append", runPath }));
                int second = HistoryCommand.Run(new Args(new[] { "history", "--file", historyPath, "--append", runPath }));
                Assert.Equal(0, first);
                Assert.Equal(0, second);
                Assert.Single(File.ReadAllLines(historyPath).Where(l => l.Trim().Length > 0));
            }
            finally
            {
                File.Delete(runPath);
                if (File.Exists(historyPath))
                    File.Delete(historyPath);
            }
        }

        [Fact]
        public void History_NonReleaseRun_IsRefused()
        {
            string historyPath = TestPaths.TempFile(".jsonl");
            string runPath = SaveTemp(SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0, buildConfiguration: "Debug"));
            try
            {
                File.Delete(historyPath);
                HistoryCommand.Run(new Args(new[] { "history", "--file", historyPath, "--append", runPath }));
                Assert.False(File.Exists(historyPath) && File.ReadAllLines(historyPath).Any(l => l.Trim().Length > 0));
            }
            finally
            {
                File.Delete(runPath);
                if (File.Exists(historyPath))
                    File.Delete(historyPath);
            }
        }

        [Fact]
        public void Ingest_PresentMonWithRunJsonAndWindow_ProducesBothSeriesAndTrimsTheWindow()
        {
            string outPath = TestPaths.TempFile(".json");
            try
            {
                int code = IngestCommand.Run(new Args(new[]
                {
                    "ingest", "--presentmon", TestPaths.PresentMonV1, "--process", "GnollHackM",
                    "--out", outPath, "--run-json", TestPaths.Run1Json, "--warmup", "0", "--window", "0.3"
                }));
                Assert.Equal(0, code);
                RunRecord r = RunRecord.Load(outPath);
                Assert.NotNull(r);
                Series external = r.FindSeries("external");
                Series smoothness = r.FindSeries(MetricNames.SmoothnessSeries);
                Assert.NotNull(external);
                Assert.NotNull(smoothness);
                Assert.True(external.Info.TryGetValue("windowTrimmedFrames", out string trimmed));
                Assert.True(int.Parse(trimmed) > 0);
            }
            finally
            {
                File.Delete(outPath);
            }
        }
    }
}

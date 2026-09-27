using System.Text.Json.Nodes;
using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class BackgroundAndDriftTests
    {
        private static RunRecord SyntheticSmoothnessRun(string arm, double hitchRatio, double pacingRms, double displayedFps)
        {
            RunRecord r = new RunRecord
            {
                Id = Guid.NewGuid().ToString("N"),
                TimestampUtc = DateTime.UtcNow.ToString("o"),
                Scenario = "W1",
                ScenarioKind = "gameplay",
                Arm = arm,
                BuildConfiguration = "Release",
                Platform = "Windows"
            };
            r.Display.RefreshHz = 60;
            r.Display.VsyncMs = 1000.0 / 60.0;
            r.Display.TargetFps = 60;
            r.Display.TargetPeriodMs = 1000.0 / 60.0;
            Series s = new Series { Kind = MetricNames.SmoothnessSeries, Source = "in-app", IntervalsMs = new float[0] };
            s.Metrics[MetricNames.HitchRatioMsPerSec] = hitchRatio;
            s.Metrics[MetricNames.PacingErrorRmsMs] = pacingRms;
            s.Metrics[MetricNames.DisplayedFps] = displayedFps;
            r.Series.Add(s);
            return r;
        }

        private static BackgroundInfo Bg(string verdict, double otherCpuP90)
        {
            return new BackgroundInfo
            {
                Source = BackgroundInfo.SourceExternal,
                Verdict = verdict,
                Window = new BackgroundWindow { Samples = 10, OtherCpuP90Pct = otherCpuP90 }
            };
        }

        private static Dictionary<string, string> Fp(string commit, string skia, params (string Key, string Value)[] extra)
        {
            Dictionary<string, string> d = new Dictionary<string, string>
            {
                [GHEnvironmentFingerprint.MetaFingerprintVersionKey] = "1",
                [GHEnvironmentFingerprint.CodeGitCommitKey] = commit,
                [GHEnvironmentFingerprint.ComponentSkiaSharpKey] = skia,
                ["os.version"] = "10.0.26200"
            };
            foreach ((string key, string value) in extra)
                d[key] = value;
            return d;
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

        private static string Lf { get { return ((char)10).ToString(); } }

        [Fact]
        public void Typeperf_RealCapture_ParsesBlanksBiasAndHost()
        {
            TypeperfTable t = TypeperfCsv.Read(TestPaths.TypeperfReal);
            Assert.Equal(-180, t.BiasMinutes);
            Assert.Equal(@"\Processor(_Total)\% Processor Time", t.Paths[0]);
            Assert.Equal(2, t.Rows.Count);
            Assert.Equal(new DateTime(2026, 9, 27, 10, 56, 9, 876).Ticks, t.TimestampTicks[0]);

            GHSystemLoadSample[] s = TypeperfCsv.ToSystemSamples(t, "NoSuchProcessXyz", 16, 32768);
            Assert.True(float.IsNaN(s[0].SystemCpuPct));
            Assert.True(float.IsNaN(s[0].DiskBusyPct));
            Assert.Equal(12923, s[0].AvailableMemoryMB);
            Assert.Equal(100.0 * 12923 / 32768, s[0].AvailableMemoryPct, 3);
            Assert.Equal(15.501, s[1].SystemCpuPct, 3);
            Assert.True(float.IsNaN(s[1].OwnCpuPct));
            Assert.True(float.IsNaN(s[1].OtherCpuPct));
            Assert.Equal(4.685, s[1].DiskBusyPct, 3);
            Assert.Equal(3.933, s[1].HardFaultsPerSec, 3);
        }

        [Fact]
        public void Typeperf_PartialLastLine_IsIgnoredAndOwnCpuIsPerLogicalProcessor()
        {
            TypeperfTable t = TypeperfCsv.Read(TestPaths.TypeperfBusy);
            Assert.Equal(12, t.Rows.Count);
            GHSystemLoadSample[] s = TypeperfCsv.ToSystemSamples(t, "GnollHackM.exe", 8, 32768);
            Assert.Equal(10f, s[5].OwnCpuPct, 3);
            Assert.Equal(50f, s[5].OtherCpuPct, 3);
            Assert.Equal(10f, s[5].DiskBusyPct, 3);
            Assert.Equal(100.0 * 12000 / 32768, s[5].AvailableMemoryPct, 3);
        }

        [Fact]
        public void Typeperf_Processes_SumInstancesDropOwnTotalIdleAndReadGpu()
        {
            TypeperfTable t = TypeperfCsv.Read(TestPaths.TypeperfProcesses);
            List<GHProcessLoad> p = TypeperfCsv.ToProcessLoads(t, "GnollHackM", 8, out float otherGpu);
            Assert.Equal(new[] { "devenv", "vmmemwsl", "msmpeng", "typeperf" }, p.Select(x => x.Name).ToArray());
            GHProcessLoad devenv = p.Single(x => x.Name == "devenv");
            Assert.Equal(8f, devenv.CpuPct, 3);
            Assert.Equal(GHBackgroundLoad.CategoryBuildTools, devenv.Category);
            GHProcessLoad wsl = p.Single(x => x.Name == "vmmemwsl");
            Assert.Equal(22f, wsl.CpuPct, 3);
            Assert.Equal(GHBackgroundLoad.CategoryWslVm, wsl.Category);
            Assert.True(float.IsNaN(wsl.GpuPct));
            Assert.Equal(17.5f, otherGpu, 3);
        }

        [Fact]
        public void AndroidLog_ProcStat_GivesSystemOwnAndMemory()
        {
            AndroidLoad load = AndroidLoadLog.Read(TestPaths.AndroidProcStat);
            Assert.False(load.Fallback);
            Assert.Equal(3, load.Samples.Length);
            Assert.Contains("systemCpu", load.Signals);
            Assert.Contains("memory", load.Signals);
            GHSystemLoadSample[] s = load.Samples;
            Assert.Equal(TimeSpan.TicksPerSecond, s[1].TimestampTicks - s[0].TimestampTicks);
            Assert.True(float.IsNaN(s[0].SystemCpuPct));
            Assert.Equal(2048, s[0].AvailableMemoryMB);
            Assert.Equal(25f, s[0].AvailableMemoryPct, 3);
            Assert.Equal(40f, s[1].SystemCpuPct, 3);
            Assert.Equal(12f, s[1].OwnCpuPct, 3);
            Assert.Equal(28f, s[1].OtherCpuPct, 3);
            Assert.Equal(1024, s[1].AvailableMemoryMB);
            Assert.Equal(12.5f, s[1].AvailableMemoryPct, 3);
            Assert.Equal(25f, s[2].SystemCpuPct, 3);
            Assert.True(float.IsNaN(s[2].OwnCpuPct));
        }

        [Fact]
        public void AndroidLog_TopFallback_GivesSystemCpuOnly()
        {
            AndroidLoad load = AndroidLoadLog.Read(TestPaths.AndroidTop);
            Assert.True(load.Fallback);
            Assert.Equal(2, load.Samples.Length);
            Assert.Equal(20f, load.Samples[0].SystemCpuPct, 3);
            Assert.Equal(30f, load.Samples[1].SystemCpuPct, 3);
            Assert.True(float.IsNaN(load.Samples[0].OwnCpuPct));
            Assert.Equal(TimeSpan.TicksPerSecond, load.Samples[1].TimestampTicks - load.Samples[0].TimestampTicks);
        }

        [Fact]
        public void ExternalBackground_TrimsTheWarmUpAndClassifiesBusy()
        {
            Dictionary<string, string> fp = EnvJson.ReadFingerprint(TestPaths.FingerprintJson);
            BackgroundInfo b = IngestCommand.BuildExternalBackground(TestPaths.TypeperfBusy, TestPaths.TypeperfProcesses, null,
                "GnollHackM", fp, 2, 5);
            Assert.NotNull(b);
            Assert.Equal(BackgroundInfo.SourceExternal, b.Source);
            Assert.Equal(8, b.LogicalProcessors);
            Assert.Equal(5, b.Window.Samples);
            Assert.Equal(2, b.PreWindow.Samples);
            Assert.Equal(7, b.Samples.Count);
            Assert.Equal(-2.0, b.Samples[0].T, 3);
            Assert.Equal(50.0, b.Window.OtherCpuP90Pct.Value, 3);
            Assert.Equal(1.0, b.Coverage.Value, 3);
            Assert.Equal(GHBackgroundLoad.VerdictBusyName, b.Verdict);
            Assert.StartsWith("background load: other CPU P90 50 %", b.Reason);
            Assert.Equal(17.5, b.OtherGpuPct.Value, 3);
            Assert.Contains("informational", b.Note);
            Assert.Equal("vmmemwsl", b.Processes[0].Name);
            Assert.Equal(GHBackgroundLoad.CategoryWslVm, b.Processes[0].Category);
            Assert.Contains(b.Activities, x => x.Category == GHBackgroundLoad.CategoryWslVm);
            foreach (string signal in new[] { "systemCpu", "disk", "memory", "hardFaults", "processes", "gpu" })
                Assert.Contains(signal, b.Signals);
        }

        [Fact]
        public void Ingest_BusyExternalLoad_IsExcludedWithTheReason()
        {
            string outPath = TestPaths.TempFile(".json");
            try
            {
                int code = IngestCommand.Run(new Args(new[]
                {
                    "ingest", "--presentmon", TestPaths.PresentMonV1, "--process", "GnollHackM",
                    "--out", outPath, "--run-json", TestPaths.Run1Json, "--warmup", "0",
                    "--batch", "B1", "--fingerprint", TestPaths.FingerprintJson,
                    "--env-during-system", TestPaths.TypeperfBusy, "--env-during-processes", TestPaths.TypeperfProcesses
                }));
                Assert.Equal(0, code);
                RunRecord r = RunRecord.Load(outPath);
                Assert.True(r.Excluded);
                Assert.StartsWith("background load: other CPU P90 50 %", r.ExclusionReason);
                Assert.Equal(GHBackgroundVerdict.Busy, r.BackgroundVerdict);
                Assert.Equal("B1", r.Batch);
                Assert.Equal("4.1.3", r.Versions["app"]);
                BackgroundInfo b = Assert.Single(r.Background);
                Assert.Equal(BackgroundInfo.SourceExternal, b.Source);
                Assert.Equal("8", r.Fingerprint["hardware.logicalProcessors"]);
                Assert.Equal("26200.6584", r.Fingerprint["os.build"]);
                /* the in-app record's own value wins over the file's */
                Assert.Equal("Sample Laptop", r.Fingerprint[GHEnvironmentFingerprint.HardwareDeviceModelKey]);
                Assert.Equal("4.1.3", r.Fingerprint[GHEnvironmentFingerprint.CodeAppVersionKey]);
                /* the in-app record has no versioned fingerprint, so the file's meta keys stay out */
                Assert.False(r.Fingerprint.ContainsKey(GHEnvironmentFingerprint.MetaFingerprintVersionKey));
            }
            finally
            {
                File.Delete(outPath);
            }
        }

        private const string InAppBackgroundJson = """
            {
              "samplerVersion": 1,
              "source": "in-app",
              "intervalMs": 1000,
              "logicalProcessors": 16,
              "coverage": 0.98,
              "samplerBusyMs": 41.5,
              "signals": ["systemCpu", "disk", "memory", "hardFaults", "processes", "gpu"],
              "window": { "samples": 60, "otherCpuMeanPct": 20.0, "otherCpuP90Pct": 31.0, "otherCpuMaxPct": 40.0,
                          "otherCpuSpikeShare": 0.0, "diskBusyMeanPct": 4.0, "diskBusyP90Pct": 9.0,
                          "availableMemoryMinPct": 41.0, "availableMemoryMinMB": 13210,
                          "hardFaultsP90PerSec": 0.0, "memoryPressureEvents": 0, "lowMemory": false },
              "preWindow": { "samples": 10, "otherCpuMeanPct": 2.0, "otherCpuP90Pct": 3.0, "otherCpuMaxPct": 4.0,
                             "otherCpuSpikeShare": 0.0, "diskBusyMeanPct": null, "diskBusyP90Pct": null,
                             "availableMemoryMinPct": 42.0, "availableMemoryMinMB": 13300,
                             "hardFaultsP90PerSec": null, "memoryPressureEvents": 0, "lowMemory": false },
              "otherGpuPct": 1.2,
              "processes": [ { "name": "vmmemWSL", "cpuPct": 22.0, "gpuPct": 0.0, "category": "wsl-vm" } ],
              "activities": [ { "category": "wsl-vm", "processes": ["vmmemWSL"], "cpuPct": 22.0 } ],
              "samples": [ { "t": -1.0, "sys": 9.2, "own": 6.8, "other": 2.4, "disk": 3.0, "availPct": 42.0, "faults": 0.0 },
                           { "t": 0.0, "sys": 40.0, "own": 7.0, "other": 33.0, "disk": 85.0, "availPct": 41.0, "faults": null } ],
              "verdict": "busy",
              "reason": "background load: other CPU P90 31 %; wsl-vm (vmmemWSL 22 %)"
            }
            """;

        [Fact]
        public void InAppRun_BackgroundFingerprintBatchAndNotes_RoundTrip()
        {
            string dir = Path.Combine(Path.GetTempPath(), "ghpa_bg_" + Guid.NewGuid().ToString("N"));
            string saved = TestPaths.TempFile(".json");
            try
            {
                Directory.CreateDirectory(dir);
                foreach (string f in Directory.GetFiles(Path.GetDirectoryName(TestPaths.Run1Json)))
                    File.Copy(f, Path.Combine(dir, Path.GetFileName(f)));
                string runJson = Path.Combine(dir, "run.json");
                JsonNode root = JsonNode.Parse(File.ReadAllText(runJson));
                root["environment"]["fingerprint"] = new JsonObject
                {
                    ["meta.fingerprintVersion"] = "1",
                    ["code.gitCommit"] = "abc123",
                    ["settings.useTileBatching"] = "true"
                };
                root["background"] = JsonNode.Parse(InAppBackgroundJson);
                root["suite"] = new JsonObject { ["suiteId"] = "S1", ["notes"] = "quiet gate timed out (other CPU 12 %)" };
                File.WriteAllText(runJson, root.ToJsonString(), new System.Text.UTF8Encoding(false));

                RunRecord r = RunRecord.Load(runJson);
                Assert.Equal("S1", r.Batch);
                Assert.Equal("quiet gate timed out (other CPU 12 %)", r.Notes);
                Assert.Equal("1", r.Fingerprint[GHEnvironmentFingerprint.MetaFingerprintVersionKey]);
                Assert.Equal("true", r.Fingerprint["settings.useTileBatching"]);
                BackgroundInfo b = Assert.Single(r.Background);
                Assert.Equal(BackgroundInfo.SourceInApp, b.Source);
                Assert.Equal(GHBackgroundVerdict.Busy, r.BackgroundVerdict);
                Assert.True(r.Excluded);
                Assert.Equal("background load: other CPU P90 31 %; wsl-vm (vmmemWSL 22 %)", r.ExclusionReason);
                Assert.Null(b.PreWindow.DiskBusyP90Pct);
                Assert.Null(b.Samples[1].Faults);

                r.Save(saved);
                RunRecord again = RunRecord.Load(saved);
                BackgroundInfo b2 = Assert.Single(again.Background);
                Assert.Equal(31.0, b2.Window.OtherCpuP90Pct.Value, 6);
                Assert.Equal(13210L, b2.Window.AvailableMemoryMinMB.Value);
                Assert.Equal(2, b2.Samples.Count);
                Assert.Equal(-1.0, b2.Samples[0].T, 6);
                Assert.Equal(33.0, b2.Samples[1].Other.Value, 6);
                Assert.Equal("S1", again.Batch);
                Assert.Equal("abc123", again.Fingerprint[GHEnvironmentFingerprint.CodeGitCommitKey]);

                GHBackgroundSummary summary = b2.Window.ToSummary(b2.Coverage);
                Assert.Equal(0.98f, summary.Coverage, 3);
                List<GHProcessLoad> processes = b2.Processes.Select(p => new GHProcessLoad(p.Name, (float)(p.CpuPct ?? 0), (float)(p.GpuPct ?? 0))).ToList();
                Assert.Equal(GHBackgroundVerdict.Busy, GHBackgroundLoad.Classify(summary, processes, float.NaN, out string reason));
                Assert.StartsWith(GHBackgroundLoad.ReasonPrefix, reason);
            }
            finally
            {
                Directory.Delete(dir, true);
                File.Delete(saved);
            }
        }

        [Fact]
        public void LegacyRecords_GetAFingerprintFromTheirFields()
        {
            RunRecord inApp = RunRecord.Load(TestPaths.Run1Json);
            Assert.Equal("4.1.3", inApp.Fingerprint[GHEnvironmentFingerprint.CodeAppVersionKey]);
            Assert.Equal("Windows", inApp.Fingerprint[GHEnvironmentFingerprint.OsPlatformKey]);
            Assert.Equal("Windows 11", inApp.Fingerprint[GHEnvironmentFingerprint.OsVersionKey]);
            Assert.Equal("Sample Laptop", inApp.Fingerprint[GHEnvironmentFingerprint.HardwareDeviceModelKey]);
            Assert.Equal("Release", inApp.Fingerprint[GHEnvironmentFingerprint.CodeBuildConfigurationKey]);
            Assert.False(inApp.Fingerprint.ContainsKey(GHEnvironmentFingerprint.MetaFingerprintVersionKey));

            RunRecord v1 = SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0);
            v1.Versions["skiaSharp"] = "2.88.8";
            v1.Git.Commit = "abc";
            v1.Configuration["environment"] = System.Text.Json.JsonSerializer.SerializeToElement(new Dictionary<string, object>
            {
                ["mapRefreshRateSetting"] = "Hz60",
                ["configuration"] = new Dictionary<string, object> { ["useTileBatching"] = true, ["primaryGPUCacheLimit"] = 256 }
            });
            string path = SaveTemp(v1);
            try
            {
                RunRecord r = RunRecord.Load(path);
                Assert.Equal("2.88.8", r.Fingerprint[GHEnvironmentFingerprint.ComponentSkiaSharpKey]);
                Assert.Equal("abc", r.Fingerprint[GHEnvironmentFingerprint.CodeGitCommitKey]);
                Assert.Equal("Hz60", r.Fingerprint[GHEnvironmentFingerprint.SettingsMapRefreshRateKey]);
                Assert.Equal("true", r.Fingerprint["settings.useTileBatching"]);
                Assert.Equal("256", r.Fingerprint["settings.primaryGPUCacheLimit"]);
            }
            finally
            {
                File.Delete(path);
            }
        }

        [Fact]
        public void BackgroundVerdict_IsTheWorstKnownOne()
        {
            RunRecord r = SyntheticSmoothnessRun("A", 1, 1, 60);
            Assert.Equal(GHBackgroundVerdict.Unknown, r.BackgroundVerdict);
            r.Background = new List<BackgroundInfo> { Bg("unknown", 50), Bg("quiet", 2), Bg("elevated", 12) };
            Assert.Equal(GHBackgroundVerdict.Elevated, r.BackgroundVerdict);
            Assert.Equal(12.0, r.DecisiveBackground.Window.OtherCpuP90Pct.Value, 6);
            Assert.Equal("e 12", HistoryCommand.BackgroundCell(r));
        }

        [Fact]
        public void ExclusionOrder_PresetThenThrottledThenPowerThenBusyThenTooFew()
        {
            RunRecord r = SyntheticSmoothnessRun("A", 1, 1, 60);
            r.Background = new List<BackgroundInfo> { Bg("busy", 40) };
            r.Background[0].Reason = "background load: other CPU P90 40 %";
            Assert.Equal("warm-up run", InAppRun.ExclusionReasonFor(r, "warm-up run", false, true, "few"));
            r.Thermal.Throttled = true;
            r.Thermal.ThrottleReason = "hot";
            Assert.Equal("throttled: hot", InAppRun.ExclusionReasonFor(r, null, false, true, "few"));
            Assert.Equal("background load: other CPU P90 40 %", InAppRun.ExclusionReasonFor(r, null, true, true, "few"));
            r.Thermal.PowerState = "changed";
            Assert.StartsWith("power state changed", InAppRun.ExclusionReasonFor(r, null, true, true, "few"));
            r.Thermal.PowerState = "charging";
            r.Background[0].Verdict = "elevated";
            Assert.Equal("few", InAppRun.ExclusionReasonFor(r, null, true, true, "few"));
            Assert.Null(InAppRun.ExclusionReasonFor(r, null, true, false, "few"));
        }

        [Fact]
        public void Compare_PrintsEnvironmentDifferencesAndTheSensitivityLine()
        {
            List<string> runPaths = new List<string>();
            string outPath = TestPaths.TempFile(".md");
            try
            {
                for (int i = 0; i < 3; i++)
                {
                    RunRecord a = SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0);
                    a.Fingerprint = Fp("c1", "2.88.8");
                    runPaths.Add(SaveTemp(a));
                }
                for (int i = 0; i < 4; i++)
                {
                    RunRecord b = SyntheticSmoothnessRun("B", 10.0, 0.5, 60.0);
                    b.Fingerprint = Fp("c1", "3.116.1");
                    if (i == 0)
                        b.Background = new List<BackgroundInfo> { Bg("elevated", 12) };
                    runPaths.Add(SaveTemp(b));
                }
                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", runPaths[0], runPaths[1], runPaths[2],
                    "--b", runPaths[3], runPaths[4], runPaths[5], runPaths[6],
                    "--series", "smoothness", "--resamples", "200", "--out", outPath
                }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                string[] lines = md.Replace("\r\n", Lf).Split((char)10);
                Assert.Contains("## Environment differences", lines);
                Assert.Contains("  component.SkiaSharp: 2.88.8 -> 3.116.1", lines);
                Assert.Contains("Attribution: environment: component", lines);
                Assert.Contains("**Sensitivity:** Without the 1 elevated run: decisions unchanged.", lines);
                Assert.Contains(lines, l => l.Contains("| e 12 |"));
                Assert.Contains(lines, l => l.EndsWith("| Gate | Throttled | Other CPU P90 | Top suspect |"));
                Assert.Single(lines, l => l.Contains("hitchRatioMsPerSec") && l.Contains("REGRESSION"));
                Assert.DoesNotContain(Lf, md.Replace(Csv.Crlf, ""));
            }
            finally
            {
                DeleteAll(runPaths);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void Compare_TwoSettingsDiffer_WarnsOfTheOneVariableRule()
        {
            List<string> runPaths = new List<string>();
            string outPath = TestPaths.TempFile(".md");
            try
            {
                RunRecord a = SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0);
                a.Fingerprint = Fp("c1", "2.88.8", ("settings.useTileBatching", "true"), ("settings.runtimeEffects", "true"));
                RunRecord b = SyntheticSmoothnessRun("B", 1.0, 0.5, 60.0);
                b.Fingerprint = Fp("c1", "2.88.8", ("settings.useTileBatching", "false"), ("settings.runtimeEffects", "false"));
                runPaths.Add(SaveTemp(a));
                runPaths.Add(SaveTemp(b));
                int code = CompareCommand.Run(new Args(new[]
                {
                    "compare", "--a", runPaths[0], "--b", runPaths[1], "--series", "smoothness", "--resamples", "200", "--out", outPath
                }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                Assert.Contains("Attribution: environment: settings", md);
                Assert.Contains("more than one setting differs", md);
                Assert.Contains("**Sensitivity:** no used run was measured under elevated background load.", md);
            }
            finally
            {
                DeleteAll(runPaths);
                File.Delete(outPath);
            }
        }

        [Fact]
        public void History_Append_DropsBackgroundSamplesButKeepsSummaries()
        {
            string historyPath = TestPaths.TempFile(".jsonl");
            RunRecord r = SyntheticSmoothnessRun("A", 1.0, 0.5, 60.0);
            BackgroundInfo b = Bg("quiet", 3);
            b.Samples = new List<BackgroundSample> { new BackgroundSample { T = 0, Sys = 10, Own = 7, Other = 3 } };
            r.Background = new List<BackgroundInfo> { b };
            r.Batch = "B7";
            string runPath = SaveTemp(r);
            try
            {
                File.Delete(historyPath);
                Assert.Equal(0, HistoryCommand.Run(new Args(new[] { "history", "--file", historyPath, "--append", runPath })));
                string line = Assert.Single(File.ReadAllLines(historyPath).Where(l => l.Trim().Length > 0));
                JsonNode root = JsonNode.Parse(line);
                JsonObject bg = root["background"][0].AsObject();
                Assert.False(bg.ContainsKey("samples"));
                Assert.Equal(3.0, (double)bg["window"]["otherCpuP90Pct"], 6);
                Assert.Equal("B7", (string)root["batch"]);
                Assert.NotNull(root["fingerprint"]);
            }
            finally
            {
                File.Delete(runPath);
                if (File.Exists(historyPath))
                    File.Delete(historyPath);
            }
        }

        [Fact]
        public void ChangePoints_BackgroundEvents_FireOnRisesAndBusyDisk()
        {
            List<BackgroundSample> samples = new List<BackgroundSample>
            {
                new BackgroundSample { T = 0, Sys = 10, Own = 8 },
                new BackgroundSample { T = 1, Sys = 30, Own = 8 },
                new BackgroundSample { T = 2, Other = 25, Disk = 85 },
                new BackgroundSample { T = 3, Other = 30, Disk = 10 }
            };
            List<ChangePoints.TimelineEvent> ev = ChangePoints.BackgroundEvents(samples);
            Assert.Equal(2, ev.Count);
            Assert.All(ev, e => Assert.Equal(ChangePoints.BackgroundEventKind, e.Kind));
            Assert.Equal(1000.0, ev[0].AtMs, 6);
            Assert.Equal("other CPU 2 -> 22 %", ev[0].Text);
            Assert.Equal(2000.0, ev[1].AtMs, 6);
            Assert.Equal("disk busy 85 %", ev[1].Text);
        }

        /* A synthetic history run of a drift scenario: batch, day of September 2026,
           hitch ratio, fingerprint, and optionally a background verdict */
        private static RunRecord DriftRun(string batch, int day, double hitch, Dictionary<string, string> fp, string verdict = null)
        {
            RunRecord r = SyntheticSmoothnessRun("A", hitch, 0.5, 60.0);
            r.TimestampUtc = new DateTime(2026, 9, day, 12, 0, 0, DateTimeKind.Utc).ToString("o");
            r.Batch = batch;
            r.Fingerprint = new Dictionary<string, string>(fp);
            if (verdict != null)
                r.Background = new List<BackgroundInfo> { Bg(verdict, 15) };
            return r;
        }

        private static List<RunRecord> Step(Dictionary<string, string> before, Dictionary<string, string> after, double hitchAfter, string verdictAfter = null)
        {
            List<RunRecord> runs = new List<RunRecord>();
            for (int i = 0; i < 3; i++)
                runs.Add(DriftRun("b1", 1, 1.0, before));
            for (int i = 0; i < 3; i++)
                runs.Add(DriftRun("b2", 2, hitchAfter, after, verdictAfter));
            return runs;
        }

        private static DriftCommand.Shift SingleShift(List<RunRecord> runs)
        {
            DriftCommand.Result res = DriftCommand.Analyze(runs, MetricNames.SmoothnessSeries, 200, 1);
            Assert.Single(res.Lines);
            DriftCommand.Shift s = Assert.Single(res.Shifts);
            Assert.Equal("b2", s.After.Key);
            Assert.Contains(s.Decisions, d => d.Metric.Name == MetricNames.HitchRatioMsPerSec && d.IsRegression);
            return s;
        }

        [Fact]
        public void Drift_ComponentOnly_IsEnvironmentComponent()
        {
            DriftCommand.Shift s = SingleShift(Step(Fp("c1", "2.88.8"), Fp("c1", "3.116.1"), 10.0));
            Assert.Equal("environment: component", s.Label);
        }

        [Fact]
        public void Drift_CommitOnly_IsCode()
        {
            DriftCommand.Shift s = SingleShift(Step(Fp("c1", "2.88.8"), Fp("c2", "2.88.8"), 10.0));
            Assert.Equal("code", s.Label);
        }

        [Fact]
        public void Drift_CommitAndComponent_IsConfounded()
        {
            DriftCommand.Shift s = SingleShift(Step(Fp("c1", "2.88.8"), Fp("c2", "3.116.1"), 10.0));
            Assert.Equal("confounded: code, component", s.Label);
        }

        [Fact]
        public void Drift_NoDiffInALoadedBatch_IsBackground()
        {
            DriftCommand.Shift s = SingleShift(Step(Fp("c1", "2.88.8"), Fp("c1", "2.88.8"), 10.0, "elevated"));
            Assert.Equal(DriftCommand.LabelBackground, s.Label);
        }

        [Fact]
        public void Drift_NothingRecorded_IsUnexplained()
        {
            DriftCommand.Shift s = SingleShift(Step(Fp("c1", "2.88.8"), Fp("c1", "2.88.8"), 10.0));
            Assert.Equal(DriftCommand.LabelUnexplained, s.Label);
        }

        [Fact]
        public void Drift_NoStep_FlagsNoShift()
        {
            DriftCommand.Result res = DriftCommand.Analyze(Step(Fp("c1", "2.88.8"), Fp("c2", "3.116.1"), 1.0), MetricNames.SmoothnessSeries, 200, 1);
            Assert.Single(res.Lines);
            Assert.Equal(2, res.Lines[0].Batches.Count);
            Assert.Empty(res.Shifts);
        }

        [Fact]
        public void Drift_Run_WritesTheReportFromAHistoryFile()
        {
            string historyPath = TestPaths.TempFile(".jsonl");
            string outPath = TestPaths.TempFile(".md");
            try
            {
                System.Text.Json.JsonSerializerOptions compact = new System.Text.Json.JsonSerializerOptions(RunRecord.JsonOptions) { WriteIndented = false };
                string text = string.Concat(Step(Fp("c1", "2.88.8"), Fp("c1", "3.116.1"), 10.0)
                    .Select(r => System.Text.Json.JsonSerializer.Serialize(r, compact) + Csv.Crlf));
                File.WriteAllText(historyPath, text, new System.Text.UTF8Encoding(false));
                int code = DriftCommand.Run(new Args(new[] { "drift", "--file", historyPath, "--resamples", "200", "--out", outPath }));
                Assert.Equal(0, code);
                string md = File.ReadAllText(outPath);
                Assert.Contains("## Line 1: Windows, ", md);
                Assert.Contains("## Shifts", md);
                Assert.Contains("Attribution: environment: component", md);
                Assert.Contains("  - component.SkiaSharp: 2.88.8 -> 3.116.1", md);
                Assert.DoesNotContain(Lf, md.Replace(Csv.Crlf, ""));
            }
            finally
            {
                File.Delete(historyPath);
                File.Delete(outPath);
            }
        }
    }
}

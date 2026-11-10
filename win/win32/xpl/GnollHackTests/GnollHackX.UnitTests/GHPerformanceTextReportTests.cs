using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers the plain-text renderer in GHPerformanceTextReport: the fixed line width,
       identity/label truncation, the run table with its background column, the
       environment, background and previous-suite sections, the medians and cause/event
       summaries, the A/B comparison report built on top of GHPerformanceComparison with
       its environment attribution and sensitivity line, and the recent hitches report
       over a synthetic frame timeline. */
    public class GHPerformanceTextReportTests
    {
        private const int Resamples = GHPerformanceComparison.DefaultResamples;
        private const ulong Seed = GHPerformanceComparison.DefaultSeed;
        private const double TargetPeriodMs = 16.667;

        private static void AssertNoLineExceedsMaxWidth(string report)
        {
            string[] lines = report.Split('\n');
            foreach (string line in lines)
                Assert.True(line.Length <= GHPerformanceTextReport.MaxLineWidth,
                    "line exceeds MaxLineWidth (" + line.Length + "): " + line);
        }

        private static GHSmoothnessSummary BuildSummary(double fps, double hitch, double pace, double judder,
            int dropped, int gcCount, double gcPauseMs, bool gcPauseDataAvailable)
        {
            GHSmoothnessSummary s = new GHSmoothnessSummary
            {
                DisplayedFps = fps,
                HitchRatioMsPerSec = hitch,
                PacingErrorRmsMs = pace,
                JudderPct = judder,
                DroppedCount = dropped,
                GcCount = gcCount,
                GcPauseMs = gcPauseMs,
                GcPauseDataAvailable = gcPauseDataAvailable,
                QuietGapCount = 40,
                QuietHitchCount = 2
            };
            s.CauseCount[(int)GHHitchCause.UiThreadLate] = 3;
            s.CauseMs[(int)GHHitchCause.UiThreadLate] = 12.5;
            s.CauseCount[(int)GHHitchCause.PaintCpu] = 1;
            s.CauseMs[(int)GHHitchCause.PaintCpu] = 4.0;
            s.EventGapCount[0] = 6;
            s.EventHitchCount[0] = 1;
            s.EventGapCount[3] = 4;
            s.EventHitchCount[3] = 0;
            return s;
        }

        private static GHReportRun BuildRun(int index, bool isWarmUp, GHSmoothnessSummary summary,
            string excludedReason, int turnReached)
        {
            return new GHReportRun
            {
                Index = index,
                IsWarmUp = isWarmUp,
                RunFileName = "run_" + index + ".ghreplay",
                Summary = summary,
                TurnReached = turnReached,
                ExcludedReason = excludedReason
            };
        }

        private static GHReportSuite BuildSuite()
        {
            string longReplayName = new string('r', 116) + ".zip"; /* 120 chars */
            string longLabel = new string('L', 70);
            string longAbortReason = "Thermal throttling detected during window 3, sustained CPU "
                + "performance drop below the platform floor for over ten seconds of capture";

            GHReportSuite suite = new GHReportSuite
            {
                SuiteId = "suite-2026-09-26-0001",
                Scenario = "TownWalk",
                ArmLabel = longLabel,
                PageMode = "Map",
                WarmUpRun = true,
                RunsRequested = 4,
                WarmUpSeconds = 10,
                WindowSeconds = 30,
                CooldownSeconds = 5,
                ThermalGate = "light",
                ThermalWaitSeconds = 120,
                ReplayFileName = longReplayName,
                ReplayBytes = 4_194_304,
                ReplaySha256 = "0123456789abcdef0123456789abcdef0123456789abcdef",
                StartTurn = 100,
                Status = "Aborted",
                AbortReason = longAbortReason,
                StartedUtc = new DateTime(2026, 9, 26, 12, 0, 0, DateTimeKind.Utc),
                Origin = "InApp",
                Platform = "Android",
                DeviceModel = "Pixel 8",
                DeviceOs = "Android 15",
                AppVersion = "1.2.3",
                GitCommit = "abcdef0123456789",
                BuildConfiguration = "Release",
                RuntimeVersion = "10.0.0",
                FrameworkVersion = "10.0.0",
                UiFrameworkVersion = "10.0.0",
                SkiaSharpVersion = "3.116.1",
                FmodVersion = "2.02.20",
                MapFpsSetting = 60,
                MeasuredRefreshHz = 60.0
            };

            /* Warm-up: excluded from medians by its own exclusion reason, as a real warm-up run is */
            suite.Runs.Add(BuildRun(0, true, BuildSummary(60.0, 1.0, 0.5, 2.0, 0, 1, 5.0, true), "warm-up", 5));
            /* Three used runs, one without GC pause data */
            suite.Runs.Add(BuildRun(1, false, BuildSummary(59.8, 1.2, 0.6, 2.5, 1, 2, 6.0, true), null, 40));
            suite.Runs.Add(BuildRun(2, false, BuildSummary(60.1, 0.9, 0.4, 1.8, 0, 1, 4.5, true), null, 41));
            suite.Runs.Add(BuildRun(3, false, BuildSummary(59.9, 1.1, 0.55, 2.1, 1, 0, 0.0, false), null, 42));
            /* An excluded run with a long reason */
            suite.Runs.Add(BuildRun(4, false, BuildSummary(45.0, 20.0, 5.0, 30.0, 10, 5, 40.0, true),
                "device screen was locked mid-window, invalidating the capture for this run", 30));
            /* A run with no smoothness record at all */
            suite.Runs.Add(BuildRun(5, false, null, null, -1));

            return suite;
        }

        [Fact]
        public void SuiteReport_EveryLineFitsMaxWidth()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);
            AssertNoLineExceedsMaxWidth(report);
        }

        [Fact]
        public void SuiteReport_ContainsIdentityAndWarmUpRow()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains(suite.SuiteId, report);
            Assert.Contains(suite.Scenario, report);
            /* The 70-char label is truncated to fit MaxLineWidth; a modest prefix survives */
            Assert.Contains(suite.ArmLabel.Substring(0, 20), report);
            /* The warm-up row is labelled "W" rather than a run index */
            Assert.Contains("W ", report);
        }

        [Fact]
        public void SuiteReport_ContainsThermalSettings()
        {
            string report = GHPerformanceTextReport.SuiteReport(BuildSuite());

            Assert.Contains("Thermal gate: Light or better  Thermal wait 120s", report);
        }

        [Fact]
        public void SuiteReport_TruncatesLongReplayNameAndAbortReason()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains(suite.ReplayFileName.Substring(0, 50), report);
            Assert.Contains(suite.AbortReason.Substring(0, 30), report);
        }

        [Fact]
        public void SuiteReport_MediansReflectOnlyTheThreeUsedRuns()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains("Medians (3 used run(s))", report);
        }

        [Fact]
        public void SuiteReport_WarmUpRunWithoutReasonIsNotUsed()
        {
            GHReportSuite suite = BuildSuite();
            suite.Runs[0].ExcludedReason = null;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains("Medians (3 used run(s))", report);
            Assert.Contains("warm-up run", report);
        }

        [Fact]
        public void SuiteReport_ShowsNAWhereGcPauseDataIsUnavailable()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains("n/a", report);
        }

        [Fact]
        public void SuiteReport_HasNoCarriageReturns()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.DoesNotContain("\r", report);
        }

        [Fact]
        public void SuiteReport_IsDeterministic()
        {
            GHReportSuite suite = BuildSuite();
            string first = GHPerformanceTextReport.SuiteReport(suite);
            string second = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Equal(first, second);
        }

        private static List<GHSmoothnessSummary> HitchSeries(double baseValue)
        {
            List<GHSmoothnessSummary> list = new List<GHSmoothnessSummary>();
            for (int i = 0; i < 5; i++)
                list.Add(BuildSummary(60.0 + 0.01 * i, baseValue + 0.01 * i, 0.5, 2.0, 0, 1, 5.0, true));
            return list;
        }

        private static GHReportSuite BuildSuiteWithSkiaVersion(string skiaVersion)
        {
            GHReportSuite suite = BuildSuite();
            suite.SkiaSharpVersion = skiaVersion;
            suite.Runs.Clear();
            return suite;
        }

        [Fact]
        public void ComparisonReport_RegressionAndVersionDifferenceAreReported()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0);
            List<GHSmoothnessSummary> b = HitchSeries(6.0); /* clearly worse hitch ratio */
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.118.0") };

            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains(GHPerformanceComparison.VerdictRegression, report);
            /* Suites without a fingerprint are diffed on their per-field environment */
            Assert.Contains("component.SkiaSharp: 3.116.1 -> 3.118.0", report);
            Assert.Contains("Attribution: environment: component", report);
        }

        [Fact]
        public void ComparisonReport_TwoRunsPerArm_ShowsNoDecision()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0).GetRange(0, 2);
            List<GHSmoothnessSummary> b = HitchSeries(6.0).GetRange(0, 2);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };

            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);

            Assert.Contains("No decision:", report);
        }

        [Fact]
        public void ComparisonReport_FourRunsPerArm_ShowsProvisionalNote()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0).GetRange(0, 4);
            List<GHSmoothnessSummary> b = HitchSeries(6.0).GetRange(0, 4);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };

            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);

            Assert.DoesNotContain("No decision:", report);
            Assert.Contains("Fewer than " + GHPerformanceComparison.ProvisionalBelowRuns
                + " runs per arm: provisional", report);
        }

        [Fact]
        public void ComparisonReport_ThreeRunsPerArm_NamesTheMinimum()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0).GetRange(0, 3);
            List<GHSmoothnessSummary> b = HitchSeries(6.0).GetRange(0, 3);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };

            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);

            Assert.Contains("No decision: fewer than " + GHPerformanceComparison.MinRunsForVerdict
                + " used runs per arm", report);
            Assert.DoesNotContain("provisional", report);
        }

        private static Dictionary<string, string> BuildFingerprint(string gitCommit, string osBuild)
        {
            return new Dictionary<string, string>
            {
                { GHEnvironmentFingerprint.MetaFingerprintVersionKey, GHEnvironmentFingerprint.FingerprintVersion },
                { GHEnvironmentFingerprint.MetaCapturedUtcKey, "2026-09-26T12:00:00.0000000Z" },
                { GHEnvironmentFingerprint.CodeAppVersionKey, "1.2.3" },
                { GHEnvironmentFingerprint.CodeGitCommitKey, gitCommit },
                { GHEnvironmentFingerprint.OsPlatformKey, "Android" },
                { "os.build", osBuild },
                { GHEnvironmentFingerprint.HardwareDeviceModelKey, "Pixel 8" },
                { "settings.useTileBatching", "true" },
                { "settings.backgroundSampler", "true" }
            };
        }

        /* Runs 1, 2 and 4 of BuildSuite with quiet, elevated and busy background verdicts */
        private static GHReportSuite BuildSuiteWithBackground()
        {
            GHReportSuite suite = BuildSuite();
            suite.Runs[1].BackgroundVerdict = GHBackgroundLoad.VerdictQuietName;
            suite.Runs[1].OtherCpuP90Pct = 3.2;
            suite.Runs[2].BackgroundVerdict = GHBackgroundLoad.VerdictElevatedName;
            suite.Runs[2].OtherCpuP90Pct = 12.0;
            suite.Runs[2].BackgroundReason = "background load: other CPU P90 12 %; build-tools (devenv 5 %)";
            suite.Runs[4].BackgroundVerdict = GHBackgroundLoad.VerdictBusyName;
            suite.Runs[4].OtherCpuP90Pct = 31.0;
            suite.Runs[4].BackgroundReason = "background load: other CPU P90 31 %, wsl-vm 22 %; wsl-vm (vmmemWSL 22 %)";
            suite.Runs[4].ExcludedReason = suite.Runs[4].BackgroundReason;
            return suite;
        }

        [Fact]
        public void SuiteReport_BackgroundColumnShowsVerdictLetterAndOtherCpu()
        {
            GHReportSuite suite = BuildSuiteWithBackground();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            string table = Section(report, "Runs:", "Background load");
            Assert.Contains(" Bg ", table);
            Assert.Contains(" q3 ", table);
            Assert.Contains(" e12 ", table);
            Assert.Contains(" B31 ", table);
        }

        [Fact]
        public void SuiteReport_BackgroundSummaryCountsVerdictsAndSuspects()
        {
            GHReportSuite suite = BuildSuiteWithBackground();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            string background = Section(report, "Background load:", "Medians");
            Assert.Contains("1 quiet, 1 elevated, 1 busy, 3 not recorded", background);
            /* wsl-vm is named twice in the busy run's reason but counts once per run */
            Assert.Contains("Suspects: build-tools (1 run), wsl-vm (1 run)", background);
            /* Elevated runs stay in the medians */
            Assert.Contains("Medians (3 used run(s))", report);
        }

        [Fact]
        public void SuiteReport_WithoutBackgroundData_SaysNotRecorded()
        {
            GHReportSuite suite = BuildSuite();
            suite.BackgroundSamplerEnabled = false;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains("Background load: not recorded (sampler off)", report);
        }

        [Fact]
        public void SuiteReport_NotesArePrintedUnderTheirRun()
        {
            GHReportSuite suite = BuildSuite();
            suite.Runs[2].Notes = "quiet gate timed out (other CPU 14 %)";
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("\n    note: quiet gate timed out (other CPU 14 %)\n", report);
        }

        [Fact]
        public void SuiteReport_EnvironmentSection_HashesPerCategoryAndChangesDuringTheSuite()
        {
            GHReportSuite suite = BuildSuite();
            suite.Fingerprint = BuildFingerprint("abcdef0", "26200.1");
            suite.FingerprintAtEnd = BuildFingerprint("abcdef0", "26200.2");
            suite.EnvironmentChanged = true;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            string environment = Section(report, "Environment:", "Runs:");
            Assert.Contains(GHEnvironmentFingerprint.ShortHash(suite.Fingerprint, "code"), environment);
            Assert.Contains(GHEnvironmentFingerprint.ShortHash(suite.Fingerprint, "settings"), environment);
            Assert.DoesNotContain("meta", environment);
            Assert.Contains("Environment changed during the suite:", environment);
            Assert.Contains("  os.build: 26200.1 -> 26200.2", environment);
        }

        [Fact]
        public void SuiteReport_WithoutFingerprint_EnvironmentIsNotRecorded()
        {
            GHReportSuite suite = BuildSuite();
            string report = GHPerformanceTextReport.SuiteReport(suite);

            Assert.Contains("Environment:\n  not recorded\n", report);
            Assert.Contains("Previous comparable suite: none", report);
        }

        [Fact]
        public void SuiteReport_PreviousSuiteSection_ShowsBothMediansAndTheChanges()
        {
            GHReportSuite suite = BuildSuite();
            suite.Fingerprint = BuildFingerprint("abcdef0", "26200.2");
            GHReportPreviousSuite previous = new GHReportPreviousSuite
            {
                SuiteId = "suite-2026-09-20-0001",
                ArmLabel = "1.2.2 1234567",
                StartedUtc = new DateTime(2026, 9, 20, 8, 30, 0, DateTimeKind.Utc),
                RunsUsed = 5,
                MedianHitchRatioMsPerSec = 0.75
            };
            previous.Changes.AddRange(GHEnvironmentFingerprint.Diff(BuildFingerprint("1234567", "26200.1"), suite.Fingerprint));
            suite.Previous = previous;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            string section = Section(report, "Changes since the previous comparable suite:", "Hitch causes");
            Assert.Contains("1.2.2 1234567, started 2026-09-20 08:30 UTC", section);
            /* Used runs 1-3 have hitch ratios 1.2, 0.9 and 1.1 */
            Assert.Contains("0.75 ms/s then, 1.10 ms/s now", section);
            Assert.Contains("  code.gitCommit: 1234567 -> abcdef0", section);
            Assert.Contains("  os.build: 26200.1 -> 26200.2", section);
            Assert.Contains("the baseline comparison is the decision", section);
        }

        [Fact]
        public void ComparisonReport_ConfoundedAndSettingsViolation_AreWarned()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0);
            List<GHSmoothnessSummary> b = HitchSeries(6.0);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            GHReportSuite suiteA = BuildSuiteWithSkiaVersion("3.116.1");
            suiteA.Fingerprint = BuildFingerprint("1111111", "26200.1");
            GHReportSuite suiteB = BuildSuiteWithSkiaVersion("3.116.1");
            suiteB.Fingerprint = BuildFingerprint("2222222", "26200.2");
            suiteB.Fingerprint["settings.useTileBatching"] = "false";
            suiteB.Fingerprint["settings.backgroundSampler"] = "false";

            string report = GHPerformanceTextReport.ComparisonReport(result,
                new List<GHReportSuite> { suiteA }, new List<GHReportSuite> { suiteB });

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("  code.gitCommit: 1111111 -> 2222222", report);
            Assert.Contains("Attribution: confounded: code, os, settings", report);
            Assert.Contains("Warning: code and environment both differ", report);
            Assert.Contains("Warning: " + GHEnvironmentFingerprint.SettingsViolationText, report);
        }

        [Fact]
        public void ComparisonReport_PoolsMixedValuesWithinAnArm()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0);
            List<GHSmoothnessSummary> b = HitchSeries(1.0);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            GHReportSuite a1 = BuildSuiteWithSkiaVersion("3.116.1");
            a1.Fingerprint = BuildFingerprint("1111111", "26200.1");
            GHReportSuite a2 = BuildSuiteWithSkiaVersion("3.116.1");
            a2.Fingerprint = BuildFingerprint("1111111", "26200.2");
            GHReportSuite b1 = BuildSuiteWithSkiaVersion("3.116.1");
            b1.Fingerprint = BuildFingerprint("1111111", "26200.2");

            string report = GHPerformanceTextReport.ComparisonReport(result,
                new List<GHReportSuite> { a1, a2 }, new List<GHReportSuite> { b1 });

            Assert.Contains("  os.build: " + GHEnvironmentFingerprint.MixedValue + " -> 26200.2", report);
            Assert.Contains("Attribution: environment: os", report);
            /* The arm order warnings (arm B has one suite) are not about the environment */
            Assert.DoesNotContain("Warning: code and environment", report);
            Assert.DoesNotContain("Warning: " + GHEnvironmentFingerprint.SettingsViolationText, report);
            Assert.DoesNotContain("more than one build", report);
        }

        /* A suite of BuildSuiteWithSkiaVersion started at hour:00 UTC on 2026-09-26 */
        private static GHReportSuite SuiteStartedAt(int hour, string suiteId)
        {
            GHReportSuite suite = BuildSuiteWithSkiaVersion("3.116.1");
            suite.StartedUtc = new DateTime(2026, 9, 26, hour, 0, 0, DateTimeKind.Utc);
            suite.SuiteId = suiteId;
            return suite;
        }

        private static string ReportFor(List<GHReportSuite> suitesA, List<GHReportSuite> suitesB)
        {
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", HitchSeries(1.0), "B",
                HitchSeries(1.0), TargetPeriodMs, Resamples, Seed);
            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);
            AssertNoLineExceedsMaxWidth(report);
            return report;
        }

        private static Dictionary<string, string> FingerprintWithId(string mvid)
        {
            Dictionary<string, string> fp = BuildFingerprint("1111111", "26200.1");
            fp[GHPerformanceSuiteLogic.CodeAssemblyMvidKey] = mvid;
            return fp;
        }

        [Fact]
        public void ComparisonReport_ArmPoolsTwoBuilds_Warns()
        {
            GHReportSuite a1 = SuiteStartedAt(10, "a1");
            a1.Fingerprint = FingerprintWithId("aaaaaaaaaaaa");
            GHReportSuite a2 = SuiteStartedAt(13, "a2");
            a2.Fingerprint = FingerprintWithId("bbbbbbbbbbbb");
            GHReportSuite b1 = SuiteStartedAt(11, "b1");
            b1.Fingerprint = FingerprintWithId("cccccccccccc");
            GHReportSuite b2 = SuiteStartedAt(12, "b2");
            b2.Fingerprint = FingerprintWithId("cccccccccccc");

            string report = ReportFor(new List<GHReportSuite> { a1, a2 }, new List<GHReportSuite> { b1, b2 });

            Assert.Contains("\nWarning: arm A pools suites of more than one build; keep one build per arm.\n"
                + "  Differs: " + GHPerformanceSuiteLogic.CodeAssemblyMvidKey + "\n", report);
            Assert.DoesNotContain("arm B pools", report);
            Assert.Contains("  id aaaaaaaaaaaa  ", report);
            Assert.Contains("  id bbbbbbbbbbbb  ", report);
        }

        [Fact]
        public void ComparisonReport_OneBuild_NoWarning()
        {
            GHReportSuite a1 = SuiteStartedAt(10, "a1");
            a1.Fingerprint = FingerprintWithId("aaaaaaaaaaaa");
            GHReportSuite a2 = SuiteStartedAt(13, "a2");
            a2.Fingerprint = FingerprintWithId("aaaaaaaaaaaa");
            /* A suite recorded without an id is not compared on it */
            GHReportSuite a3 = SuiteStartedAt(14, "a3");
            a3.Fingerprint = BuildFingerprint("1111111", "26200.1");
            GHReportSuite b1 = SuiteStartedAt(11, "b1");
            GHReportSuite b2 = SuiteStartedAt(12, "b2");

            string report = ReportFor(new List<GHReportSuite> { a1, a2, a3 }, new List<GHReportSuite> { b1, b2 });

            Assert.DoesNotContain("more than one build", report);
        }

        [Fact]
        public void ComparisonReport_AllABeforeB_Warns()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { SuiteStartedAt(10, "a1"), SuiteStartedAt(11, "a2") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { SuiteStartedAt(12, "b1"), SuiteStartedAt(13, "b2") };

            string report = ReportFor(suitesA, suitesB);

            Assert.Contains("\nOrder: A A B B\n", report);
            Assert.Contains("\nWarning: every A suite ran before every B suite (or vice versa);\n"
                + "  drift is confounded with the change. Interleave A B B A.\n", report);
            Assert.DoesNotContain("single suite", report);
        }

        [Fact]
        public void ComparisonReport_OneSuitePerArm_Warns()
        {
            string report = ReportFor(new List<GHReportSuite> { SuiteStartedAt(10, "a1") },
                new List<GHReportSuite> { SuiteStartedAt(11, "b1") });

            Assert.Contains("Warning: every A suite ran before every B suite", report);
            Assert.Contains("\nWarning: arm A has a single suite; run each arm at least twice (A B B A).\n", report);
            Assert.Contains("\nWarning: arm B has a single suite; run each arm at least twice (A B B A).\n", report);
        }

        [Fact]
        public void ComparisonReport_Abba_NoWarning()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { SuiteStartedAt(13, "a2"), SuiteStartedAt(10, "a1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { SuiteStartedAt(11, "b1"), SuiteStartedAt(12, "b2") };

            string report = ReportFor(suitesA, suitesB);

            Assert.Contains("\nOrder: A B B A\n", report);
            Assert.DoesNotContain("Warning:", report);
        }

        [Fact]
        public void ComparisonReport_ListsSuitesInStartOrder()
        {
            GHReportSuite a1 = BuildSuite();
            a1.StartedUtc = new DateTime(2026, 9, 26, 10, 0, 0, DateTimeKind.Utc);
            a1.SuiteId = "a1";
            List<GHReportSuite> suitesA = new List<GHReportSuite> { SuiteStartedAt(13, "a2"), a1 };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { SuiteStartedAt(12, "b2"), SuiteStartedAt(11, "b1") };

            string report = ReportFor(suitesA, suitesB);

            string section = Section(report, "Suites (UTC start, arm, used runs):", "Order:");
            int first = section.IndexOf("  2026-09-26 10:00:00  A  3  a1\n", StringComparison.Ordinal);
            int second = section.IndexOf("  2026-09-26 11:00:00  B  0  b1\n", StringComparison.Ordinal);
            int third = section.IndexOf("  2026-09-26 12:00:00  B  0  b2\n", StringComparison.Ordinal);
            int fourth = section.IndexOf("  2026-09-26 13:00:00  A  0  a2\n", StringComparison.Ordinal);
            Assert.True(first >= 0 && second > first && third > second && fourth > third, section);
        }

        /* A playback suite whose used runs played the given input records in their
           windows; a negative count leaves the run's input records unrecorded */
        private static GHReportSuite PlaybackSuite(params long[] inputRecords)
        {
            GHReportSuite suite = BuildSuiteWithSkiaVersion("3.116.1");
            suite.Scenario = "playback";
            for (int i = 0; i < inputRecords.Length; i++)
            {
                GHReportRun run = BuildRun(i + 1, false, BuildSummary(60.0, 1.0, 0.5, 2.0, 0, 1, 5.0, true), null, 200);
                run.TurnAtWindowStart = 100;
                run.InputRecordsInWindow = inputRecords[i];
                suite.Runs.Add(run);
            }
            return suite;
        }

        [Fact]
        public void Coverage_PlaybackSpreadDiffers_Warns()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { PlaybackSuite(100, 102, 101, 100) };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { PlaybackSuite(50, 70, 90, 130) };

            string warning = GHPerformanceTextReport.ContentCoverageWarning(suitesA, suitesB, "playback");

            Assert.Equal("Warning: the arms played different content (input records in the window):\n"
                + "  A median 100 (100-102), B median 70 (50-130)", warning);
            string report = ReportFor(suitesA, suitesB);
            Assert.Contains("\n" + warning + "\n", report);
        }

        [Fact]
        public void Coverage_PlaybackSameContent_NoWarning()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { PlaybackSuite(100, 102, 101, 100) };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { PlaybackSuite(101, 99, 103, 100) };

            Assert.Null(GHPerformanceTextReport.ContentCoverageWarning(suitesA, suitesB, "playback"));
        }

        [Fact]
        public void Coverage_RangesOneApart_NoWarning()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { PlaybackSuite(100, 100, 100, 100) };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { PlaybackSuite(101, 101, 101, 101) };

            Assert.Null(GHPerformanceTextReport.ContentCoverageWarning(suitesA, suitesB, "playback"));
        }

        [Fact]
        public void Coverage_RangesTwoApartWithinTolerance_Warns()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { PlaybackSuite(100, 101, 100, 101) };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { PlaybackSuite(103, 104, 103, 104) };

            Assert.NotNull(GHPerformanceTextReport.ContentCoverageWarning(suitesA, suitesB, "playback"));
        }

        [Fact]
        public void Coverage_Idle_NoWarning()
        {
            List<GHReportSuite> suitesA = new List<GHReportSuite> { PlaybackSuite(100, 102, 101, 100) };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { PlaybackSuite(10, 12, 11, 10) };

            Assert.Null(GHPerformanceTextReport.ContentCoverageWarning(suitesA, suitesB, "idle"));
            Assert.NotNull(GHPerformanceTextReport.ContentCoverageWarning(suitesA, suitesB, "playback"));
        }

        [Fact]
        public void Coverage_LegacyTurnOnly()
        {
            GHReportSuite a = PlaybackSuite(-1, -1, -1);
            GHReportSuite b = PlaybackSuite(-1, -1, -1);
            for (int i = 0; i < a.Runs.Count; i++)
            {
                a.Runs[i].TurnAtWindowStart = -1;
                a.Runs[i].TurnReached = 150 + i % 2;
                b.Runs[i].TurnAtWindowStart = -1;
                b.Runs[i].TurnReached = 120 + i % 2;
            }

            string warning = GHPerformanceTextReport.ContentCoverageWarning(new List<GHReportSuite> { a },
                new List<GHReportSuite> { b }, "playback");

            Assert.Equal("Warning: the arms played different content (turn reached):\n"
                + "  A median 150 (150-151), B median 120 (120-121)", warning);
        }

        [Fact]
        public void ComparisonReport_SensitivityLine_ReportsAChangedDecision()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0);
            List<GHSmoothnessSummary> b = HitchSeries(6.0);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);
            GHComparisonResult without = GHPerformanceComparison.CompareSmoothness("A", a.GetRange(0, 2), "B",
                b.GetRange(0, 2), TargetPeriodMs, Resamples, Seed);
            GHReportComparisonContext context = new GHReportComparisonContext { ElevatedRuns = 6, WithoutElevated = without };

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB, context);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("Without the 6 elevated runs:\n", report);
            Assert.Contains("  " + GHPerformanceComparison.HitchRatioMsPerSec + " changes to "
                + GHPerformanceComparison.VerdictTooFewRuns, report);
            Assert.DoesNotContain("decision unchanged", report);
        }

        [Fact]
        public void ComparisonReport_SensitivityLine_ReportsAnUnchangedDecision()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0);
            List<GHSmoothnessSummary> b = HitchSeries(6.0);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);
            GHReportComparisonContext context = new GHReportComparisonContext { ElevatedRuns = 1, WithoutElevated = result };

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB, context);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("Without the 1 elevated run: decision unchanged", report);
        }

        [Fact]
        public void ComparisonReport_NoElevatedRuns_PrintsNoSensitivityLine()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0);
            List<GHSmoothnessSummary> b = HitchSeries(6.0);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);
            GHReportComparisonContext context = new GHReportComparisonContext { ElevatedRuns = 0 };

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB, context);

            Assert.DoesNotContain("Without the", report);
            Assert.Contains("Attribution: none", report);
        }

        private static long Ms(double ms)
        {
            return (long)Math.Round(ms * Stopwatch.Frequency / 1000.0);
        }

        /* count ticks of 60 FPS on a 60 Hz panel, painted on the UI thread, in which the
           display callback before stallIndex is missed and the one at stallIndex runs late
           across a collection, as in GHSmoothnessMetricsTests */
        private static GHFrameRecord[] StallTimeline(int count, int stallIndex)
        {
            long period = Stopwatch.Frequency / 60;
            long vsync = 1000 * Stopwatch.Frequency;
            long counter = 1000;
            int gc = 0;
            List<GHFrameRecord> records = new List<GHFrameRecord>();
            for (int i = 0; i < count; i++)
            {
                bool stall = i == stallIndex;
                if (stall)
                {
                    vsync += period;
                    gc++;
                }
                counter++;
                GHFrameRecord r = new GHFrameRecord();
                r.FrameId = i + 1;
                r.VsyncTicks = vsync;
                r.RefreshPeriodTicks = period;
                r.CallbackStartTicks = vsync + Ms(stall ? 12.0 : 0.2);
                r.CallbackEndTicks = r.CallbackStartTicks + Ms(0.3);
                r.TargetFps = 60;
                r.AssumedRefreshHz = 60;
                r.Pacing = GHPacingDecision.Rendered;
                r.MainCounter = counter;
                r.GeneralCounter = counter / 2;
                r.Invalidate = GHInvalidateOutcome.Invalidated;
                r.InvalidateTicks = r.CallbackStartTicks + Ms(0.1);
                r.Paint = GHPaintOutcome.Painted;
                r.PaintOnUiThread = true;
                r.PaintStartTicks = r.InvalidateTicks + Ms(0.2);
                r.LockAttemptTicks = r.PaintStartTicks + Ms(0.1);
                r.LockResultTicks = r.LockAttemptTicks + Ms(0.01);
                r.LockAcquired = true;
                r.DrawEndTicks = r.PaintStartTicks + Ms(3.0);
                r.FlushEndTicks = r.DrawEndTicks + Ms(1.0);
                r.PaintedMainCounter = counter;
                r.PaintedGeneralCounter = counter / 2;
                r.GcCount0 = gc;
                if (stall)
                    r.ContentEvents = GHContentEvent.FloatingText | GHContentEvent.Message;
                records.Add(r);
                vsync += period;
            }
            return records.ToArray();
        }

        private static string Section(string report, string startsWith, string endsBefore)
        {
            int start = report.IndexOf(startsWith, StringComparison.Ordinal);
            Assert.True(start >= 0, "missing section " + startsWith);
            int end = report.IndexOf(endsBefore, start, StringComparison.Ordinal);
            Assert.True(end > start, "missing section " + endsBefore);
            return report.Substring(start, end - start);
        }

        [Fact]
        public void RecentHitchesReport_ListsTheHitchNearAMark_AndItsCause()
        {
            int stallIndex = 60;
            GHFrameRecord[] records = StallTimeline(120, stallIndex);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[records.Length];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, records.Length, null, 0,
                displayed, out displayedCount);
            Assert.True(summary.HitchCount >= 1, "hitches " + summary.HitchCount);

            int hitch = -1;
            for (int j = 1; j < displayedCount && hitch < 0; j++)
            {
                if (displayed[j].IsHitch)
                    hitch = j;
            }
            double gapMs = displayed[hitch].GapTicks * 1000.0 / Stopwatch.Frequency;
            string gapText = "gap " + gapMs.ToString("0.00", CultureInfo.InvariantCulture) + " ms";
            string causeName = GHSmoothnessMetrics.CauseName(displayed[hitch].Cause);

            long markUtcTicks = new DateTime(2026, 9, 26, 12, 34, 56, 789, DateTimeKind.Utc).Ticks;
            string markClock = new DateTime(markUtcTicks, DateTimeKind.Utc).ToLocalTime()
                .ToString("HH:mm:ss.fff", CultureInfo.InvariantCulture);
            long[] markFrameIds = { records[stallIndex].FrameId };
            long[] markTicks = { markUtcTicks };

            string report = GHPerformanceTextReport.RecentHitchesReport(records, records.Length, displayed,
                displayedCount, summary, markFrameIds, markTicks, 1);

            AssertNoLineExceedsMaxWidth(report);
            Assert.DoesNotContain("\r", report);
            Assert.StartsWith("Recent hitches\n", report);

            string marked = Section(report, "Marked moments:", "Worst hitches");
            Assert.Contains(markClock, marked);
            Assert.Contains(gapText, marked);
            Assert.Contains(causeName, marked);
            Assert.DoesNotContain("no hitch within", marked);

            string worst = Section(report, "Worst hitches", "Hitch causes");
            Assert.Contains(gapText, worst);
            Assert.Contains(causeName, worst);
            Assert.Contains("FloatingText", worst);

            string causes = report.Substring(report.IndexOf("Hitch causes", StringComparison.Ordinal));
            Assert.Contains(causeName, causes);
        }

        [Fact]
        public void RecentHitchesReport_MarkFarFromAnyHitch_SaysSo()
        {
            /* Ten seconds with the stall at 9 s, marked at 0.5 s */
            GHFrameRecord[] records = StallTimeline(600, 540);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[records.Length];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, records.Length, null, 0,
                displayed, out displayedCount);
            Assert.True(summary.HitchCount >= 1, "hitches " + summary.HitchCount);
            long[] markFrameIds = { records[30].FrameId };
            long[] markTicks = { new DateTime(2026, 9, 26, 12, 0, 0, DateTimeKind.Utc).Ticks };

            string report = GHPerformanceTextReport.RecentHitchesReport(records, records.Length, displayed,
                displayedCount, summary, markFrameIds, markTicks, 1);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("no hitch from 5 s before to 0.5 s after", Section(report, "Marked moments:", "Worst hitches"));
        }

        [Fact]
        public void RecentHitchesReport_HitchSecondsBeforeTheMark_IsListed()
        {
            /* Ten seconds with the stall at 4 s, marked at 8 s */
            GHFrameRecord[] records = StallTimeline(600, 240);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[records.Length];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, records.Length, null, 0,
                displayed, out displayedCount);
            int hitch = -1;
            for (int j = 1; j < displayedCount && hitch < 0; j++)
            {
                if (displayed[j].IsHitch)
                    hitch = j;
            }
            Assert.True(hitch > 0, "no hitch");
            double gapMs = displayed[hitch].GapTicks * 1000.0 / Stopwatch.Frequency;
            string gapText = "gap " + gapMs.ToString("0.00", CultureInfo.InvariantCulture) + " ms";
            long[] markFrameIds = { records[480].FrameId };
            long[] markTicks = { new DateTime(2026, 9, 26, 12, 0, 0, DateTimeKind.Utc).Ticks };

            string report = GHPerformanceTextReport.RecentHitchesReport(records, records.Length, displayed,
                displayedCount, summary, markFrameIds, markTicks, 1);

            string marked = Section(report, "Marked moments:", "Worst hitches");
            Assert.Contains(gapText, marked);
            Assert.DoesNotContain("no hitch from", marked);
        }

        [Fact]
        public void RecentHitchesReport_HitchSecondsAfterTheMark_IsNotListed()
        {
            /* Ten seconds with the stall at 6 s, marked at 4 s */
            GHFrameRecord[] records = StallTimeline(600, 360);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[records.Length];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, records.Length, null, 0,
                displayed, out displayedCount);
            Assert.True(summary.HitchCount >= 1, "hitches " + summary.HitchCount);
            long[] markFrameIds = { records[240].FrameId };
            long[] markTicks = { new DateTime(2026, 9, 26, 12, 0, 0, DateTimeKind.Utc).Ticks };

            string report = GHPerformanceTextReport.RecentHitchesReport(records, records.Length, displayed,
                displayedCount, summary, markFrameIds, markTicks, 1);

            Assert.Contains("no hitch from 5 s before to 0.5 s after", Section(report, "Marked moments:", "Worst hitches"));
        }

        /* The recent hitches report over a two-second stall timeline, with the summary
           adjusted before rendering */
        private static string RecentReportWith(Action<GHSmoothnessSummary> adjust)
        {
            GHFrameRecord[] records = StallTimeline(120, 60);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[records.Length];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, records.Length, null, 0,
                displayed, out displayedCount);
            if (adjust != null)
                adjust(summary);
            string report = GHPerformanceTextReport.RecentHitchesReport(records, records.Length, displayed,
                displayedCount, summary, new long[0], new long[0], 0);
            AssertNoLineExceedsMaxWidth(report);
            return report;
        }

        [Fact]
        public void RecentHitchesReport_Header_SaysDisplayTimesAreEstimated()
        {
            string report = RecentReportWith(null);

            string header = Section(report, "Recent hitches", "Marked moments:");
            Assert.Contains("\nDisplay times: estimated (first vsync after ready)\n", header);
            Assert.DoesNotContain("FrameMetrics reports lost", header);
            Assert.DoesNotContain("Stalls over", header);
        }

        [Fact]
        public void RecentHitchesReport_Header_MeasuredDisplayTimes()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s)
            {
                s.PresentSource = GHPresentSource.Measured;
            });

            string header = Section(report, "Recent hitches", "Marked moments:");
            Assert.Contains("\nDisplay times: measured\n", header);
            Assert.DoesNotContain("estimated", header);
        }

        [Fact]
        public void RecentHitchesReport_Header_ReportsLostFrameMetricsReports()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s) { s.CompositorReportsLost = 3; });

            Assert.Contains("\nFrameMetrics reports lost: 3 (compositor data incomplete)\n",
                Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_FrameMetricsCoverage()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s)
            {
                s.CompositorCoverage = 0.25;
                s.SyncOffsetP50Ms = 4.5;
            });

            Assert.Contains("\nFrameMetrics coverage: 25 % of GL-thread paints (median sync offset 4.50 ms)\n",
                Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_NoFrameMetricsCoverage_WhenNotApplicable()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s) { s.CompositorCoverage = -1; });

            Assert.DoesNotContain("FrameMetrics coverage", Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_VsyncCorrectedShare()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s) { s.VsyncCorrectedShare = 0.5; });

            Assert.Contains("\nVsync times corrected: 50 % of ticks (stale platform frame time)\n",
                Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_NoVsyncCorrectedShare_WhenZeroOrNotApplicable()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s) { s.VsyncCorrectedShare = 0; });
            Assert.DoesNotContain("Vsync times corrected", Section(report, "Recent hitches", "Marked moments:"));

            report = RecentReportWith(delegate (GHSmoothnessSummary s) { s.VsyncCorrectedShare = -1; });
            Assert.DoesNotContain("Vsync times corrected", Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_ExcludedLongStalls()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s)
            {
                s.LongStallCount = 2;
                s.LongStallMs = 3500;
                s.LongStallsExcluded = true;
            });

            Assert.Contains("\nStalls over 1 s: 2 (3500.00 ms), excluded from hitch time\n",
                Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_CountedLongStalls()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s)
            {
                s.LongStallCount = 1;
                s.LongStallMs = 1250;
                s.LongStallsExcluded = false;
            });

            Assert.Contains("\nStalls over 1 s: 1 (1250.00 ms), counted as hitches\n",
                Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_GcLine_CountsForcedCollections()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s)
            {
                s.GcCount = 5;
                s.ForcedGcCount = 2;
                s.GcPauseDataAvailable = false;
            });

            Assert.Contains("\nGC: 5 collection(s), 2 forced by the app, pause n/a\n",
                Section(report, "Recent hitches", "Marked moments:"));
        }

        [Fact]
        public void RecentHitchesReport_Header_GcLine_NoForcedCollections()
        {
            string report = RecentReportWith(delegate (GHSmoothnessSummary s)
            {
                s.GcCount = 5;
                s.ForcedGcCount = 0;
                s.GcPauseDataAvailable = false;
            });

            string header = Section(report, "Recent hitches", "Marked moments:");
            Assert.Contains("\nGC: 5 collection(s), pause n/a\n", header);
            Assert.DoesNotContain("forced", header);
        }

        /* The stall tick follows an app-forced collection: its hitch is tagged in the worst
           hitches, and the span's cause table counts it under its cause and again below */
        [Fact]
        public void RecentHitchesReport_ForcedGcHitch_IsTaggedAndCounted()
        {
            int stallIndex = 60;
            GHFrameRecord[] records = StallTimeline(120, stallIndex);
            records[stallIndex].Flags |= GHFrameFlags.ForcedCollection;
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[records.Length];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, records.Length, null, 0,
                displayed, out displayedCount);

            int tagged = 0;
            double taggedMs = 0;
            string causeName = null;
            for (int j = 1; j < displayedCount; j++)
            {
                if (!displayed[j].IsHitch || !displayed[j].OverlapsForcedGc)
                    continue;
                tagged++;
                taggedMs += (displayed[j].GapTicks - displayed[j].TargetPeriodTicks) * 1000.0 / Stopwatch.Frequency;
                causeName = GHSmoothnessMetrics.CauseName(displayed[j].Cause);
            }
            Assert.Equal(1, tagged);

            string report = GHPerformanceTextReport.RecentHitchesReport(records, records.Length, displayed,
                displayedCount, summary, new long[0], new long[0], 0);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("  " + causeName + ", forced GC\n", Section(report, "Worst hitches", "Hitch causes"));
            string causes = report.Substring(report.IndexOf("Hitch causes (span):", StringComparison.Ordinal));
            Assert.Contains("\n  after app-forced GC: 1 hitch, "
                + taggedMs.ToString("0.00", CultureInfo.InvariantCulture) + " ms\n", causes);
        }

        [Fact]
        public void RecentHitchesReport_NoForcedGc_NoTagNorLine()
        {
            string report = RecentReportWith(null);

            Assert.DoesNotContain("forced GC", report);
            Assert.DoesNotContain("after app-forced GC", report);
        }

        [Fact]
        public void SuiteReport_CauseTotals_SumForcedGcHitchesOfUsedRuns()
        {
            GHReportSuite suite = BuildSuite();
            string causes = Section(GHPerformanceTextReport.SuiteReport(suite), "Hitch causes", "Content events");
            Assert.DoesNotContain("after app-forced GC", causes);

            suite.Runs[1].Summary.ForcedGcHitchCount = 2;
            suite.Runs[1].Summary.ForcedGcHitchMs = 100.0;
            suite.Runs[3].Summary.ForcedGcHitchCount = 1;
            suite.Runs[3].Summary.ForcedGcHitchMs = 50.25;
            /* The excluded run's hitches are not summed */
            suite.Runs[4].Summary.ForcedGcHitchCount = 5;
            suite.Runs[4].Summary.ForcedGcHitchMs = 999.0;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("\n  after app-forced GC: 3 hitches, 150.25 ms\n",
                Section(report, "Hitch causes", "Content events"));
        }

        [Fact]
        public void SuiteReport_Medians_SayDisplayTimesAreEstimated()
        {
            string report = GHPerformanceTextReport.SuiteReport(BuildSuite());

            string medians = Section(report, "Medians", "Previous comparable suite");
            Assert.Contains("  Display times are estimated in-app; Compositor and Dropped are inferred.\n", medians);
            Assert.DoesNotContain("Stalls over", medians);
        }

        [Fact]
        public void SuiteReport_Medians_SumLongStallsOfUsedRuns()
        {
            GHReportSuite suite = BuildSuite();
            suite.Runs[1].Summary.LongStallCount = 1;
            suite.Runs[1].Summary.LongStallMs = 1200;
            suite.Runs[3].Summary.LongStallCount = 2;
            suite.Runs[3].Summary.LongStallMs = 3000;
            /* The excluded run's stall is not summed */
            suite.Runs[4].Summary.LongStallCount = 5;
            suite.Runs[4].Summary.LongStallMs = 9000;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("\n  Stalls over 1 s: 3 (4200.00 ms), counted as hitches\n",
                Section(report, "Medians", "Previous comparable suite"));
        }

        [Fact]
        public void SuiteReport_Medians_FrameMetricsCoverageOfUsedRuns()
        {
            GHReportSuite suite = BuildSuite();
            string medians = Section(GHPerformanceTextReport.SuiteReport(suite), "Medians", "Previous comparable suite");
            Assert.DoesNotContain("FrameMetrics coverage", medians);

            suite.Runs[1].Summary.CompositorCoverage = 0.9;
            suite.Runs[2].Summary.CompositorCoverage = 0.2;
            suite.Runs[3].Summary.CompositorCoverage = 0.3;
            /* The excluded run's coverage is not in the median */
            suite.Runs[4].Summary.CompositorCoverage = 0.0;
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            Assert.Contains("\n  FrameMetrics coverage: median 30 % of GL-thread paints\n",
                Section(report, "Medians", "Previous comparable suite"));
        }

        /* A cause row as AppendCauseRows prints it, up to the ms column */
        private static string CauseRowPrefix(GHHitchCause cause, int count)
        {
            string name = GHSmoothnessMetrics.CauseName(cause);
            if (name.Length > 18)
                name = name.Substring(0, 18);
            return "  " + name.PadRight(18) + count.ToString(CultureInfo.InvariantCulture).PadLeft(5) + "  ";
        }

        [Fact]
        public void SuiteReport_CauseTotals_Version2_ShowsHitchCounts()
        {
            GHReportSuite suite = BuildSuite();
            for (int i = 0; i < suite.Runs.Count; i++)
            {
                GHSmoothnessSummary s = suite.Runs[i].Summary;
                if (s == null)
                    continue;
                s.MetricsVersion = GHSmoothnessMetrics.MetricsVersion;
                /* Of the three UiThreadLate frames, one is judder */
                s.CauseHitchCount[(int)GHHitchCause.UiThreadLate] = 2;
                s.CauseHitchCount[(int)GHHitchCause.PaintCpu] = 1;
            }
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            string causes = Section(report, "Hitch causes", "Content events");
            Assert.Contains(CauseRowPrefix(GHHitchCause.UiThreadLate, 6), causes);
            Assert.Contains(CauseRowPrefix(GHHitchCause.PaintCpu, 3), causes);
            Assert.DoesNotContain("judder frames", causes);
        }

        [Fact]
        public void SuiteReport_CauseTotals_Version1_ShowsCauseCountAndNote()
        {
            GHReportSuite suite = BuildSuite();
            for (int i = 0; i < suite.Runs.Count; i++)
            {
                GHSmoothnessSummary s = suite.Runs[i].Summary;
                if (s != null)
                    s.MetricsVersion = 1;
            }
            string report = GHPerformanceTextReport.SuiteReport(suite);

            AssertNoLineExceedsMaxWidth(report);
            string causes = Section(report, "Hitch causes", "Content events");
            Assert.Contains(CauseRowPrefix(GHHitchCause.UiThreadLate, 9), causes);
            Assert.Contains(CauseRowPrefix(GHHitchCause.PaintCpu, 3), causes);
            Assert.Contains("\n  counts include judder frames (recorded before metrics version 2)\n", causes);
        }
    }
}

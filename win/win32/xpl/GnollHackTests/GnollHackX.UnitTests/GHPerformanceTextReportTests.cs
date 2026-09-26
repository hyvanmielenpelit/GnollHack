using System;
using System.Collections.Generic;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers the plain-text renderer in GHPerformanceTextReport: the fixed line width,
       identity/label truncation, the run table, the medians and cause/event summaries,
       and the A/B comparison report built on top of GHPerformanceComparison. */
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
            Assert.Contains("SkiaSharp 3.116.1 -> 3.118.0", report);
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
        public void ComparisonReport_ThreeRunsPerArm_ShowsProvisionalNote()
        {
            List<GHSmoothnessSummary> a = HitchSeries(1.0).GetRange(0, 3);
            List<GHSmoothnessSummary> b = HitchSeries(6.0).GetRange(0, 3);
            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            List<GHReportSuite> suitesA = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };
            List<GHReportSuite> suitesB = new List<GHReportSuite> { BuildSuiteWithSkiaVersion("3.116.1") };

            string report = GHPerformanceTextReport.ComparisonReport(result, suitesA, suitesB);

            Assert.Contains("provisional", report);
        }
    }
}

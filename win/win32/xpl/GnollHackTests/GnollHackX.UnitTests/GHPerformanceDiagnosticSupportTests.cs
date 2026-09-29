using System;
using System.Collections.Generic;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHPerformanceDiagnosticSupport: the report retention choice, the window's
       allocation rate, the smoothness summary's mapping to facts, and the non-default
       settings list. */
    public class GHPerformanceDiagnosticSupportTests
    {
        private static string Report(string stamp)
        {
            return "perftest_" + stamp + ".txt";
        }

        private static void Select(List<string> files, List<string> folders, int maxKept, string protectedStamp,
            out List<string> filesToDelete, out List<string> foldersToDelete)
        {
            filesToDelete = new List<string>();
            foldersToDelete = new List<string>();
            GHPerformanceDiagnosticSupport.SelectRetentionDeletions(files, folders, maxKept, protectedStamp,
                filesToDelete, foldersToDelete);
        }

        [Fact]
        public void Retention_KeepsNewestN()
        {
            /* Listed out of order: the choice goes by stamp, not by listing order */
            List<string> stamps = new List<string>
            {
                "20260903_100000", "20260901_100000", "20260905_100000", "20260902_100000", "20260904_100000"
            };
            List<string> files = new List<string>();
            foreach (string stamp in stamps)
                files.Add(Report(stamp));

            List<string> filesToDelete;
            List<string> foldersToDelete;
            Select(files, stamps, 3, null, out filesToDelete, out foldersToDelete);

            filesToDelete.Sort(StringComparer.Ordinal);
            foldersToDelete.Sort(StringComparer.Ordinal);
            Assert.Equal(new List<string> { Report("20260901_100000"), Report("20260902_100000") }, filesToDelete);
            Assert.Equal(new List<string> { "20260901_100000", "20260902_100000" }, foldersToDelete);

            /* Fewer reports than the limit: nothing goes */
            Select(files, stamps, 30, null, out filesToDelete, out foldersToDelete);
            Assert.Empty(filesToDelete);
            Assert.Empty(foldersToDelete);
        }

        [Fact]
        public void Retention_DeletesOrphanFolders()
        {
            List<string> files = new List<string> { Report("20260901_100000"), Report("20260902_100000") };
            List<string> folders = new List<string> { "20260901_100000", "20260902_100000", "20260903_100000" };

            List<string> filesToDelete;
            List<string> foldersToDelete;
            Select(files, folders, 30, null, out filesToDelete, out foldersToDelete);

            Assert.Empty(filesToDelete);
            Assert.Equal(new List<string> { "20260903_100000" }, foldersToDelete);
        }

        [Fact]
        public void Retention_ProtectedStampFolderKeptWithoutReport()
        {
            /* The report of 20260903_100000 could not be written */
            List<string> files = new List<string> { Report("20260901_100000"), Report("20260902_100000") };
            List<string> folders = new List<string> { "20260901_100000", "20260902_100000", "20260903_100000" };

            List<string> filesToDelete;
            List<string> foldersToDelete;
            Select(files, folders, 1, "20260903_100000", out filesToDelete, out foldersToDelete);

            Assert.Equal(new List<string> { Report("20260901_100000") }, filesToDelete);
            Assert.Equal(new List<string> { "20260901_100000" }, foldersToDelete);

            /* Without the protection, as after an abort, the folder is an orphan */
            Select(files, folders, 1, null, out filesToDelete, out foldersToDelete);
            foldersToDelete.Sort(StringComparer.Ordinal);
            Assert.Equal(new List<string> { "20260901_100000", "20260903_100000" }, foldersToDelete);
        }

        [Fact]
        public void Retention_IgnoresNonMatchingNames()
        {
            List<string> files = new List<string>
            {
                "notes.txt", "perftest_20260901.txt", "perftest_20260901_100000.txt.bak",
                "perftest_20260901_100000.log", "Perftest_20260901_100000.txt", "perftest_2026090a_100000.txt"
            };
            List<string> folders = new List<string>
            {
                "old", "20260901_1000", "20260901_100000_x", "x20260901_100000", "suites"
            };

            List<string> filesToDelete;
            List<string> foldersToDelete;
            Select(files, folders, 0, null, out filesToDelete, out foldersToDelete);

            Assert.Empty(filesToDelete);
            Assert.Empty(foldersToDelete);
        }

        [Fact]
        public void AllocationRate_KnownAndUnknown()
        {
            Assert.Equal(2.0, GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(0, 60L * 1024 * 1024, 30.0), 9);
            Assert.Equal(0.0, GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(1000, 1000, 30.0), 9);

            Assert.True(double.IsNaN(GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(-1, 1000, 30.0)));
            Assert.True(double.IsNaN(GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(1000, -1, 30.0)));
            Assert.True(double.IsNaN(GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(2000, 1000, 30.0)));
            Assert.True(double.IsNaN(GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(0, 1000, 0.0)));
            Assert.True(double.IsNaN(GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(0, 1000, double.NaN)));
            Assert.True(double.IsNaN(GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(0, 1000,
                double.PositiveInfinity)));
        }

        private static GHSmoothnessSummary FullSummary()
        {
            GHSmoothnessSummary s = new GHSmoothnessSummary();
            s.MetricsVersion = 2;
            s.WindowMs = 29500.0;
            s.DisplayedCount = 1771;
            s.PaintedCount = 1800;
            s.TargetFps = 60.0;
            s.MeasuredRefreshHz = 120.0;
            s.AssumedRefreshMismatch = true;
            s.DisplayedFps = 59.5;
            s.HitchRatioMsPerSec = 12.5;
            s.HitchCount = 7;
            s.PacingErrorRmsMs = 1.25;
            s.PaintP50Ms = 3.5;
            s.PaintP99Ms = 9.75;
            s.GcCount = 4;
            s.GcPauseMs = 22.5;
            s.GcPauseDataAvailable = true;
            s.CauseMs[(int)GHHitchCause.Gpu] = 40.0;
            s.CauseMs[(int)GHHitchCause.PaintCpu] = 10.0;
            s.PausedGapCount = 20;
            s.QuietGapCount = 1700;
            s.QuietHitchCount = 3;
            s.EventGapCount[0] = 30;
            s.EventHitchCount[0] = 2;
            s.LongStallCount = 1;
            s.LongStallMs = 1250.0;
            s.CompositorReportsLost = 5;
            s.PresentSource = GHPresentSource.Measured;
            return s;
        }

        [Fact]
        public void FillSummaryFacts_MapsFieldsAndUnknowns()
        {
            GHSmoothnessSummary s = FullSummary();
            GHDiagnosisFacts f = new GHDiagnosisFacts();
            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, s, 31.0, 1760);

            Assert.Equal(1760, f.OnScreenIntervalCount);
            Assert.Equal(29.5f, f.WindowSeconds);
            Assert.Equal(60f, f.TargetFps);
            Assert.Equal(120f, f.MeasuredRefreshHz);
            Assert.True(f.AssumedRefreshMismatch);
            Assert.Equal(59.5f, f.DisplayedFps);
            Assert.Equal(12.5f, f.HitchRatioMsPerSec);
            Assert.Equal(7, f.HitchCount);
            Assert.Equal(1.25f, f.PacingErrorRmsMs);
            Assert.Equal(3.5f, f.PaintP50Ms);
            Assert.Equal(9.75f, f.PaintP99Ms);
            Assert.Equal(4, f.GcCount);
            Assert.Equal(22.5f, f.GcPauseMs);
            Assert.Equal(40.0, f.CauseMs[(int)GHHitchCause.Gpu]);
            Assert.Equal(10.0, f.CauseMs[(int)GHHitchCause.PaintCpu]);
            Assert.NotSame(s.CauseMs, f.CauseMs);
            Assert.Equal(20, f.PausedGapCount);
            Assert.Equal(1700, f.QuietGapCount);
            Assert.Equal(3, f.QuietHitchCount);
            Assert.Equal(30, f.EventGapCounts[0]);
            Assert.Equal(2, f.EventHitchCounts[0]);
            Assert.NotSame(s.EventGapCount, f.EventGapCounts);
            Assert.NotSame(s.EventHitchCount, f.EventHitchCounts);
            Assert.Equal(2, f.MetricsVersion);
            Assert.Equal(1, f.LongStallCount);
            Assert.Equal(1250f, f.LongStallMs);
            Assert.Equal(5, f.CompositorReportsLost);
            Assert.Equal(GHPerformanceDiagnosis.PresentSourceMeasured, f.PresentSource);

            /* No span, no paints, no pause data, no rates: unknown, and the window falls
               back to the elapsed time */
            GHSmoothnessSummary empty = new GHSmoothnessSummary();
            empty.DisplayedFps = 50.0;
            empty.HitchRatioMsPerSec = 5.0;
            empty.PacingErrorRmsMs = 1.0;
            empty.PaintP50Ms = 3.0;
            empty.GcPauseMs = 10.0;
            empty.MeasuredRefreshHz = double.PositiveInfinity;
            empty.LongStallMs = double.NaN;
            empty.PresentSource = GHPresentSource.Estimated;
            f = new GHDiagnosisFacts();
            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, empty, 31.0, 0);
            Assert.Equal(31f, f.WindowSeconds);
            Assert.True(float.IsNaN(f.TargetFps));
            Assert.True(float.IsNaN(f.MeasuredRefreshHz));
            Assert.True(float.IsNaN(f.DisplayedFps));
            Assert.True(float.IsNaN(f.HitchRatioMsPerSec));
            Assert.True(float.IsNaN(f.PacingErrorRmsMs));
            Assert.True(float.IsNaN(f.PaintP50Ms));
            Assert.True(float.IsNaN(f.PaintP99Ms));
            Assert.True(float.IsNaN(f.GcPauseMs));
            Assert.True(float.IsNaN(f.LongStallMs));
            Assert.Equal(GHPerformanceDiagnosis.PresentSourceEstimated, f.PresentSource);

            /* A span with a single displayed frame: the window is known, the rates are not */
            GHSmoothnessSummary single = FullSummary();
            single.DisplayedCount = 1;
            f = new GHDiagnosisFacts();
            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, single, 31.0, 0);
            Assert.Equal(29.5f, f.WindowSeconds);
            Assert.True(float.IsNaN(f.DisplayedFps));
            Assert.True(float.IsNaN(f.HitchRatioMsPerSec));
            Assert.True(float.IsNaN(f.PacingErrorRmsMs));
            Assert.Equal(0, f.ContentEventCount);

            /* No summary: only the window length and the interval count */
            f = new GHDiagnosisFacts();
            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, null, 31.0, 12);
            Assert.Equal(31f, f.WindowSeconds);
            Assert.Equal(12, f.OnScreenIntervalCount);
            Assert.Equal(-1, f.MetricsVersion);
            Assert.Null(f.PresentSource);
            f = new GHDiagnosisFacts();
            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, null, double.NaN, 0);
            Assert.True(float.IsNaN(f.WindowSeconds));
        }

        [Fact]
        public void FillSummaryFacts_EventGapsCountedOnce()
        {
            /* 100 intervals: 10 paused, 60 quiet, so 30 with content events, although the
               per-kind counts, which count a gap once per kind, add up to 55 */
            GHSmoothnessSummary s = FullSummary();
            s.DisplayedCount = 101;
            s.PausedGapCount = 10;
            s.QuietGapCount = 60;
            s.EventGapCount[0] = 30;
            s.EventGapCount[1] = 20;
            s.EventGapCount[2] = 5;
            GHDiagnosisFacts f = new GHDiagnosisFacts();
            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, s, 30.0, 100);

            Assert.Equal(30, f.ContentEventCount);
            Assert.Equal(new[] { 30, 20, 5 }, new[] { f.EventGapCounts[0], f.EventGapCounts[1], f.EventGapCounts[2] });
        }

        [Fact]
        public void NonDefaultSettings_OnlyDefaultOnFeaturesTurnedOff()
        {
            /* All defaults kept */
            Assert.Empty(GHPerformanceDiagnosticSupport.NonDefaultPerformanceSettings(true, true, true, true, true));

            /* Default-on features turned off */
            Assert.Equal(new List<string> { "tile batching off", "text blob caching off" },
                GHPerformanceDiagnosticSupport.NonDefaultPerformanceSettings(true, true, false, true, false));

            /* Tile batching unavailable, so forced off: not listed */
            Assert.Equal(new List<string> { "text blob caching off" },
                GHPerformanceDiagnosticSupport.NonDefaultPerformanceSettings(false, true, false, true, false));

            /* Default-off features: neither off nor on is listed */
            Assert.Empty(GHPerformanceDiagnosticSupport.NonDefaultPerformanceSettings(true, false, false, false, false));
            Assert.Empty(GHPerformanceDiagnosticSupport.NonDefaultPerformanceSettings(true, false, true, false, true));
        }
    }
}

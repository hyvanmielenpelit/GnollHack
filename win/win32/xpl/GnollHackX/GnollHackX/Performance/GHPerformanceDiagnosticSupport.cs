using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace GnollHackX.Performance
{
    /* The pure parts of the in-game performance test (GHPerformanceDiagnosticRunner): the
       report retention choice, the window's allocation rate, the smoothness summary's
       facts and the non-default settings list. No app or platform state, so the unit
       tests compile it as is.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHPerformanceDiagnosticSupport
    {
        private const double BytesPerMB = 1024.0 * 1024.0;

        private static readonly Regex _reportFileRegex = new Regex(@"^perftest_([0-9]{8}_[0-9]{6})\.txt\z",
            RegexOptions.CultureInvariant);
        private static readonly Regex _testFolderRegex = new Regex(@"^[0-9]{8}_[0-9]{6}\z", RegexOptions.CultureInvariant);

        /* Of the reports directory's file and folder names, appends to filesToDelete the
           perftest_<stamp>.txt reports older than the newest maxKept, and to
           foldersToDelete every <stamp> folder whose report is not kept, such as an
           aborted test's. The report and folder of protectedStamp (the test just saved,
           or null) are always kept, so a folder whose report could not be written
           survives. Names matching neither pattern are never selected. */
        public static void SelectRetentionDeletions(IList<string> fileNames, IList<string> folderNames, int maxKept,
            string protectedStamp, List<string> filesToDelete, List<string> foldersToDelete)
        {
            List<string> stamps = new List<string>();
            if (fileNames != null)
            {
                for (int i = 0; i < fileNames.Count; i++)
                {
                    Match m = _reportFileRegex.Match(fileNames[i] ?? "");
                    if (m.Success)
                        stamps.Add(m.Groups[1].Value);
                }
            }
            stamps.Sort(StringComparer.Ordinal);
            HashSet<string> kept = new HashSet<string>(StringComparer.Ordinal);
            for (int i = Math.Max(0, stamps.Count - Math.Max(0, maxKept)); i < stamps.Count; i++)
                kept.Add(stamps[i]);
            if (!string.IsNullOrEmpty(protectedStamp))
                kept.Add(protectedStamp);

            if (fileNames != null)
            {
                for (int i = 0; i < fileNames.Count; i++)
                {
                    Match m = _reportFileRegex.Match(fileNames[i] ?? "");
                    if (m.Success && !kept.Contains(m.Groups[1].Value))
                        filesToDelete.Add(fileNames[i]);
                }
            }
            if (folderNames != null)
            {
                for (int i = 0; i < folderNames.Count; i++)
                {
                    string name = folderNames[i] ?? "";
                    if (_testFolderRegex.IsMatch(name) && !kept.Contains(name))
                        foldersToDelete.Add(name);
                }
            }
        }

        /* MB (2^20 bytes) per second between two GC.GetTotalAllocatedBytes readings
           seconds apart; NaN when a reading is unknown (negative), the counter went
           back, or the span is not positive */
        public static double AllocationRateMBPerSec(long start, long end, double seconds)
        {
            if (start < 0 || end < start || double.IsNaN(seconds) || double.IsInfinity(seconds) || seconds <= 0)
                return double.NaN;
            return (end - start) / BytesPerMB / seconds;
        }

        /* The saved window's smoothness summary; elapsedSeconds is the window's wall
           clock length, used when the summary is null or has no span */
        public static void FillSummaryFacts(GHDiagnosisFacts f, GHSmoothnessSummary s, double elapsedSeconds,
            int onScreenIntervals)
        {
            f.OnScreenIntervalCount = onScreenIntervals;
            if (s == null)
            {
                f.WindowSeconds = ToFloat(elapsedSeconds);
                return;
            }

            bool haveWindow = s.WindowMs > 0 && s.DisplayedCount >= 2;
            f.WindowSeconds = s.WindowMs > 0 ? (float)(s.WindowMs / 1000.0) : ToFloat(elapsedSeconds);
            f.TargetFps = Positive(s.TargetFps);
            f.MeasuredRefreshHz = Positive(s.MeasuredRefreshHz);
            f.AssumedRefreshMismatch = s.AssumedRefreshMismatch;
            f.DisplayedFps = haveWindow ? (float)s.DisplayedFps : float.NaN;
            f.HitchRatioMsPerSec = haveWindow ? (float)s.HitchRatioMsPerSec : float.NaN;
            f.HitchCount = s.HitchCount;
            f.PacingErrorRmsMs = haveWindow ? (float)s.PacingErrorRmsMs : float.NaN;
            f.PaintP50Ms = s.PaintedCount > 0 ? (float)s.PaintP50Ms : float.NaN;
            f.PaintP99Ms = s.PaintedCount > 0 ? (float)s.PaintP99Ms : float.NaN;
            f.GcCount = s.GcCount;
            f.GcPauseMs = s.GcPauseDataAvailable ? (float)s.GcPauseMs : float.NaN;
            f.CauseMs = (double[])s.CauseMs.Clone();
            /* The unpaused gaps that are not quiet: a gap with several event kinds
               counts once */
            f.ContentEventCount = Math.Max(0, s.DisplayedCount - 1 - s.PausedGapCount - s.QuietGapCount);
            f.PausedGapCount = s.PausedGapCount;
            f.QuietGapCount = s.QuietGapCount;
            f.QuietHitchCount = s.QuietHitchCount;
            f.EventGapCounts = (int[])s.EventGapCount.Clone();
            f.EventHitchCounts = (int[])s.EventHitchCount.Clone();
            f.MetricsVersion = s.MetricsVersion;
            f.LongStallCount = s.LongStallCount;
            f.LongStallMs = ToFloat(s.LongStallMs);
            f.CompositorReportsLost = s.CompositorReportsLost;
            f.PresentSource = PresentSourceName(s.PresentSource);
        }

        /* The performance features switched off although their default is on; turning a
           feature on is never listed. Tile batching counts only where it is available
           (it is forced off elsewhere). */
        public static List<string> NonDefaultPerformanceSettings(bool tileAvail, bool tileDefault, bool tileOn,
            bool blobDefault, bool blobOn)
        {
            List<string> list = new List<string>();
            if (tileAvail && tileDefault && !tileOn)
                list.Add("tile batching off");
            if (blobDefault && !blobOn)
                list.Add("text blob caching off");
            return list;
        }

        /* NaN for NaN and infinities */
        internal static float ToFloat(double value)
        {
            return double.IsNaN(value) || double.IsInfinity(value) ? float.NaN : (float)value;
        }

        private static float Positive(double value)
        {
            return value > 0 && !double.IsInfinity(value) ? (float)value : float.NaN;
        }

        private static string PresentSourceName(GHPresentSource source)
        {
            switch (source)
            {
            case GHPresentSource.Measured:
                return GHPerformanceDiagnosis.PresentSourceMeasured;
            case GHPresentSource.Estimated:
                return GHPerformanceDiagnosis.PresentSourceEstimated;
            default:
                return GHPerformanceDiagnosis.PresentSourceNone;
            }
        }
    }
}

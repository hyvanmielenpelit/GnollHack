using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Text;

namespace GnollHackX.Performance
{
    /* One run within a recorded performance suite, as shown in a text report. The
       summary is null when the run produced no smoothness record; ExcludedReason is
       null when the run counts toward the suite's medians and totals. */
    public sealed class GHReportRun
    {
        public int Index;
        public bool IsWarmUp;
        public string RunFileName;
        public GHSmoothnessSummary Summary;
        public int TurnReached = -1;
        public string ExcludedReason;
    }

    /* One recorded performance suite: its identity, the device and build it ran on,
       and its per-run results, as shown in a text report or compared against a
       baseline suite. */
    public sealed class GHReportSuite
    {
        public string SuiteId;
        public string Scenario;
        public string ArmLabel;
        public string PageMode;
        public bool WarmUpRun;
        public int RunsRequested;
        public int WarmUpSeconds;
        public int WindowSeconds;
        public int CooldownSeconds;
        public string ReplayFileName;
        public long ReplayBytes;
        public string ReplaySha256;
        public int StartTurn;
        public string Status;
        public string AbortReason;
        public DateTime StartedUtc;
        public string Origin;
        public string Platform;
        public string DeviceModel;
        public string DeviceOs;
        public string AppVersion;
        public string GitCommit;
        public string BuildConfiguration;
        public string RuntimeVersion;
        public string FrameworkVersion;
        public string UiFrameworkVersion;
        public string SkiaSharpVersion;
        public string FmodVersion;
        public double MapFpsSetting;
        public double MeasuredRefreshHz;
        public readonly List<GHReportRun> Runs = new List<GHReportRun>();
    }

    /* Renders a GHReportSuite, a GHComparisonResult, or the hitches of a stretch of
       the frame timeline as plain, fixed-width text for the phone report viewer and
       the share zip. Every line is at most MaxLineWidth characters and lines are
       joined with "\n" only. Deterministic: nothing here reads the current time, so
       the same inputs always render the same text, apart from RecentHitchesReport
       showing mark times in the local time zone. Must compile under C# 7.3. */
    public static class GHPerformanceTextReport
    {
        public const int MaxLineWidth = 78;

        /* Run table column widths; StatusColW is whatever is left of MaxLineWidth. */
        private const int RunColW = 4;
        private const int FpsColW = 7;
        private const int HitchColW = 8;
        private const int PaceColW = 8;
        private const int JudderColW = 8;
        private const int DropColW = 5;
        private const int GcColW = 4;
        private const int GcMsColW = 8;
        private const int TurnColW = 6;
        private const int StatusColW = MaxLineWidth
            - (RunColW + FpsColW + HitchColW + PaceColW + JudderColW + DropColW
               + GcColW + GcMsColW + TurnColW);

        public static string SuiteReport(GHReportSuite suite)
        {
            StringBuilder sb = new StringBuilder();
            if (suite == null)
            {
                Line(sb, "No suite data.");
                return sb.ToString();
            }

            AppendIdentity(sb, suite);
            Line(sb, "");
            AppendRunTable(sb, suite);
            Line(sb, "");
            List<GHReportRun> used = UsedRuns(suite);
            AppendMedians(sb, used);
            Line(sb, "");
            AppendCauseTotals(sb, used);
            Line(sb, "");
            AppendContentEvents(sb, used);
            return sb.ToString();
        }

        public static string ComparisonReport(GHComparisonResult result,
            IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB)
        {
            StringBuilder sb = new StringBuilder();
            if (result == null)
            {
                Line(sb, "No comparison data.");
                return sb.ToString();
            }

            Line(sb, Truncate("Comparison: " + OrNA(result.LabelB) + " vs "
                + OrNA(result.LabelA), MaxLineWidth));
            Line(sb, "Runs used: A=" + result.RunsA.ToString(CultureInfo.InvariantCulture)
                + "  B=" + result.RunsB.ToString(CultureInfo.InvariantCulture));
            Line(sb, "");
            AppendVersionLines(sb, "A", suitesA);
            AppendVersionLines(sb, "B", suitesB);
            AppendVersionDifferences(sb, suitesA, suitesB);
            Line(sb, "");
            AppendVerdictTable(sb, result);
            Line(sb, "");
            AppendOverallLine(sb, result);
            Line(sb, "");
            List<GHReportRun> usedA = UsedRunsOfSuites(suitesA);
            List<GHReportRun> usedB = UsedRunsOfSuites(suitesB);
            Line(sb, "Cause totals, arm A:");
            AppendCauseTotals(sb, usedA);
            Line(sb, "Cause totals, arm B:");
            AppendCauseTotals(sb, usedB);
            return sb.ToString();
        }

        /* The hitches of a stretch of the frame timeline and those around the moments the
           user marked. displayed and summary are what GHSmoothnessMetrics.Analyze returned
           for records[0..recordCount); the marks are frame ids with the UTC time of each
           mark in DateTime ticks, oldest first. Times are in seconds relative to the last
           record's callback start, the end of the span. */
        public static string RecentHitchesReport(GHFrameRecord[] records, int recordCount, GHDisplayedFrame[] displayed,
            int displayedCount, GHSmoothnessSummary summary, long[] markFrameIds, long[] markUtcTicks, int markCount)
        {
            StringBuilder sb = new StringBuilder();
            Line(sb, "Recent hitches");
            if (records == null || summary == null)
                recordCount = 0;
            else
                recordCount = Math.Min(recordCount, records.Length);
            if (recordCount <= 0)
            {
                Line(sb, "No frame data.");
                return sb.ToString();
            }
            displayedCount = displayed == null ? 0 : Math.Max(0, Math.Min(displayedCount, displayed.Length));
            markCount = markFrameIds == null || markUtcTicks == null
                ? 0 : Math.Max(0, Math.Min(markCount, Math.Min(markFrameIds.Length, markUtcTicks.Length)));

            long spanEnd = records[recordCount - 1].CallbackStartTicks;
            double spanSeconds = (double)(spanEnd - records[0].CallbackStartTicks) / Stopwatch.Frequency;
            Line(sb, Truncate("Span: " + Fmt(spanSeconds) + " s  Ticks: "
                + recordCount.ToString(CultureInfo.InvariantCulture)
                + "  Displayed frames: " + displayedCount.ToString(CultureInfo.InvariantCulture), MaxLineWidth));
            Line(sb, Truncate("Displayed FPS: " + Fmt(summary.DisplayedFps)
                + "  Hitch ratio: " + Fmt(summary.HitchRatioMsPerSec) + " ms/s"
                + "  Hitches: " + summary.HitchCount.ToString(CultureInfo.InvariantCulture), MaxLineWidth));
            Line(sb, Truncate("GC: " + summary.GcCount.ToString(CultureInfo.InvariantCulture) + " collection(s), pause "
                + (summary.GcPauseDataAvailable ? Fmt(summary.GcPauseMs) + " ms" : "n/a"), MaxLineWidth));
            Line(sb, "");
            AppendMarkedMoments(sb, records, recordCount, displayed, displayedCount, spanEnd,
                markFrameIds, markUtcTicks, markCount);
            Line(sb, "");
            AppendWorstHitches(sb, records, recordCount, displayed, displayedCount, spanEnd,
                summary.GcPauseDataAvailable);
            Line(sb, "");
            AppendRecentCauseTotals(sb, displayed, displayedCount);
            return sb.ToString();
        }

        /* ---- suite identity and run table ---- */

        private static void AppendIdentity(StringBuilder sb, GHReportSuite suite)
        {
            Line(sb, "Performance Suite Report");
            Line(sb, Truncate("Suite: " + OrNA(suite.SuiteId), MaxLineWidth));

            string status = OrNA(suite.Status);
            if (!string.IsNullOrEmpty(suite.AbortReason))
                status = status + " (" + suite.AbortReason + ")";
            Line(sb, Truncate("Status: " + status, MaxLineWidth));

            Line(sb, Truncate("Scenario: " + OrNA(suite.Scenario)
                + "  Label: " + OrNA(suite.ArmLabel), MaxLineWidth));
            Line(sb, Truncate("Page mode: " + OrNA(suite.PageMode)
                + "  Warm-up run: " + (suite.WarmUpRun ? "on" : "off"), MaxLineWidth));
            Line(sb, Truncate("Runs requested: "
                + suite.RunsRequested.ToString(CultureInfo.InvariantCulture)
                + "  Warm-up " + suite.WarmUpSeconds.ToString(CultureInfo.InvariantCulture) + "s"
                + "  Window " + suite.WindowSeconds.ToString(CultureInfo.InvariantCulture) + "s"
                + "  Cool-down " + suite.CooldownSeconds.ToString(CultureInfo.InvariantCulture) + "s",
                MaxLineWidth));

            string replayLabel = "Replay: ";
            Line(sb, replayLabel + Truncate(OrNA(suite.ReplayFileName),
                MaxLineWidth - replayLabel.Length));
            Line(sb, Truncate("Bytes: " + suite.ReplayBytes.ToString(CultureInfo.InvariantCulture)
                + "  SHA-256: " + ShortHash(suite.ReplaySha256, 12)
                + "  Start turn: " + suite.StartTurn.ToString(CultureInfo.InvariantCulture),
                MaxLineWidth));
            Line(sb, Truncate("Map FPS setting: " + Fmt(suite.MapFpsSetting)
                + "  Measured refresh: " + Fmt(suite.MeasuredRefreshHz) + "Hz", MaxLineWidth));

            Line(sb, Truncate("Platform: " + OrNA(suite.Platform)
                + "  Device: " + OrNA(suite.DeviceModel)
                + "  OS: " + OrNA(suite.DeviceOs), MaxLineWidth));
            Line(sb, Truncate("App version: " + OrNA(suite.AppVersion)
                + "  Commit: " + ShortHash(suite.GitCommit, 9)
                + "  Build: " + OrNA(suite.BuildConfiguration), MaxLineWidth));
            Line(sb, Truncate("Runtime: " + OrNA(suite.RuntimeVersion)
                + "  Framework: " + OrNA(suite.FrameworkVersion)
                + "  UI framework: " + OrNA(suite.UiFrameworkVersion), MaxLineWidth));
            Line(sb, Truncate("SkiaSharp: " + OrNA(suite.SkiaSharpVersion)
                + "  FMOD: " + OrNA(suite.FmodVersion)
                + "  Origin: " + OrNA(suite.Origin), MaxLineWidth));
            Line(sb, "Started (UTC): "
                + suite.StartedUtc.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture));
        }

        private static void AppendRunTable(StringBuilder sb, GHReportSuite suite)
        {
            Line(sb, "Runs:");
            Line(sb, BuildRunHeader());
            for (int i = 0; i < suite.Runs.Count; i++)
                AppendRunRow(sb, suite.Runs[i]);
        }

        private static string BuildRunHeader()
        {
            return PadR("Run", RunColW) + PadR("FPS", FpsColW) + PadR("Hitch", HitchColW)
                + PadR("PaceRMS", PaceColW) + PadR("Judder", JudderColW)
                + PadR("Drop", DropColW) + PadR("GC#", GcColW) + PadR("GCms", GcMsColW)
                + PadR("Turn", TurnColW) + PadR("Status", StatusColW);
        }

        private static void AppendRunRow(StringBuilder sb, GHReportRun run)
        {
            string runLabel = run.IsWarmUp ? "W" : run.Index.ToString(CultureInfo.InvariantCulture);
            string turnLabel = run.TurnReached >= 0
                ? run.TurnReached.ToString(CultureInfo.InvariantCulture) : "?";
            GHSmoothnessSummary s = run.Summary;
            string mainPart;
            string status;

            if (s == null)
            {
                mainPart = PadR(runLabel, RunColW) + PadR("n/a", FpsColW)
                    + PadR("n/a", HitchColW) + PadR("n/a", PaceColW) + PadR("n/a", JudderColW)
                    + PadR("n/a", DropColW) + PadR("n/a", GcColW) + PadR("n/a", GcMsColW)
                    + PadR(turnLabel, TurnColW);
                status = "no record";
            }
            else
            {
                string gcMs = s.GcPauseDataAvailable ? Fmt(s.GcPauseMs) : "n/a";
                mainPart = PadR(runLabel, RunColW) + PadR(Fmt(s.DisplayedFps), FpsColW)
                    + PadR(Fmt(s.HitchRatioMsPerSec), HitchColW)
                    + PadR(Fmt(s.PacingErrorRmsMs), PaceColW)
                    + PadR(Fmt(s.JudderPct) + "%", JudderColW)
                    + PadR(s.DroppedCount.ToString(CultureInfo.InvariantCulture), DropColW)
                    + PadR(s.GcCount.ToString(CultureInfo.InvariantCulture), GcColW)
                    + PadR(gcMs, GcMsColW)
                    + PadR(turnLabel, TurnColW);
                if (run.ExcludedReason != null)
                    status = run.ExcludedReason;
                else
                    status = run.IsWarmUp ? "warm-up run" : "used";
            }

            if (status.Length <= StatusColW)
            {
                Line(sb, mainPart + PadR(status, StatusColW));
            }
            else
            {
                Line(sb, mainPart);
                Line(sb, "    " + Truncate(status, MaxLineWidth - 4));
            }
        }

        /* ---- medians, hitch causes, content events (shared with the comparison) ---- */

        private static void AppendMedians(StringBuilder sb, List<GHReportRun> used)
        {
            Line(sb, "Medians (" + used.Count.ToString(CultureInfo.InvariantCulture)
                + " used run(s)):");
            if (used.Count == 0)
            {
                Line(sb, "  No used runs.");
                return;
            }

            List<double> fps = new List<double>();
            List<double> hitch = new List<double>();
            List<double> pace = new List<double>();
            List<double> judder = new List<double>();
            List<double> gc = new List<double>();
            for (int i = 0; i < used.Count; i++)
            {
                GHSmoothnessSummary s = used[i].Summary;
                fps.Add(s.DisplayedFps);
                hitch.Add(s.HitchRatioMsPerSec);
                pace.Add(s.PacingErrorRmsMs);
                judder.Add(s.JudderPct);
                gc.Add(s.GcCount);
            }

            Line(sb, Truncate("  FPS " + Fmt(Median(fps))
                + "  Hitch " + Fmt(Median(hitch)) + "ms/s"
                + "  PaceRMS " + Fmt(Median(pace)) + "ms"
                + "  Judder " + Fmt(Median(judder)) + "%"
                + "  GC " + Fmt(Median(gc)), MaxLineWidth));
        }

        private static void AppendCauseTotals(StringBuilder sb, List<GHReportRun> used)
        {
            Line(sb, "Hitch causes (used runs, summed):");
            int[] counts = new int[GHSmoothnessMetrics.CauseCount];
            double[] ms = new double[GHSmoothnessMetrics.CauseCount];
            for (int i = 0; i < used.Count; i++)
            {
                GHSmoothnessSummary s = used[i].Summary;
                for (int c = 0; c < GHSmoothnessMetrics.CauseCount; c++)
                {
                    counts[c] += s.CauseCount[c];
                    ms[c] += s.CauseMs[c];
                }
            }
            AppendCauseRows(sb, counts, ms);
        }

        /* One row per cause with a nonzero count, or "none"; counts and ms are indexed
           by GHHitchCause */
        private static void AppendCauseRows(StringBuilder sb, int[] counts, double[] ms)
        {
            double totalMs = 0;
            for (int c = 0; c < GHSmoothnessMetrics.CauseCount; c++)
                totalMs += ms[c];

            bool any = false;
            for (int c = 1; c < GHSmoothnessMetrics.CauseCount; c++)
            {
                if (counts[c] == 0)
                    continue;
                any = true;
                double share = totalMs > 0 ? 100.0 * ms[c] / totalMs : 0;
                string name = GHSmoothnessMetrics.CauseName((GHHitchCause)c);
                Line(sb, Truncate("  " + PadR(name, 18)
                    + PadL(counts[c].ToString(CultureInfo.InvariantCulture), 5)
                    + "  " + Fmt(ms[c]) + "ms  " + Fmt(share) + "%", MaxLineWidth));
            }
            if (!any)
                Line(sb, "  none");
        }

        private static void AppendContentEvents(StringBuilder sb, List<GHReportRun> used)
        {
            Line(sb, "Content events vs hitches (used runs, summed):");
            int[] gaps = new int[GHSmoothnessMetrics.ContentEventKinds];
            int[] hitches = new int[GHSmoothnessMetrics.ContentEventKinds];
            int quietGaps = 0, quietHitches = 0;
            for (int i = 0; i < used.Count; i++)
            {
                GHSmoothnessSummary s = used[i].Summary;
                for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
                {
                    gaps[k] += s.EventGapCount[k];
                    hitches[k] += s.EventHitchCount[k];
                }
                quietGaps += s.QuietGapCount;
                quietHitches += s.QuietHitchCount;
            }

            bool any = false;
            for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
            {
                if (gaps[k] == 0)
                    continue;
                any = true;
                double rate = 100.0 * hitches[k] / gaps[k];
                string name = GHSmoothnessMetrics.ContentEventName(k);
                Line(sb, Truncate("  " + PadR(name, 14)
                    + PadL(gaps[k].ToString(CultureInfo.InvariantCulture), 6)
                    + PadL(hitches[k].ToString(CultureInfo.InvariantCulture), 5)
                    + "  " + Fmt(rate) + "%", MaxLineWidth));
            }
            if (!any)
                Line(sb, "  none");

            double quietRate = quietGaps > 0 ? 100.0 * quietHitches / quietGaps : 0;
            Line(sb, Truncate("  " + PadR("Quiet", 14)
                + PadL(quietGaps.ToString(CultureInfo.InvariantCulture), 6)
                + PadL(quietHitches.ToString(CultureInfo.InvariantCulture), 5)
                + "  " + Fmt(quietRate) + "%", MaxLineWidth));
        }

        /* ---- comparison ---- */

        private static void AppendVersionLines(StringBuilder sb, string arm,
            IList<GHReportSuite> suites)
        {
            List<string> combos = DistinctBuilds(suites);
            if (combos.Count == 0)
            {
                Line(sb, "Arm " + arm + ": n/a");
                return;
            }
            for (int i = 0; i < combos.Count; i++)
            {
                string[] parts = combos[i].Split('|');
                string text = "Arm " + arm + ": app " + parts[0] + "  commit " + parts[1]
                    + "  Skia " + parts[2] + "  Framework " + parts[3] + "  Runtime " + parts[4];
                Line(sb, Truncate(text, MaxLineWidth));
            }
        }

        private static void AppendVersionDifferences(StringBuilder sb,
            IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB)
        {
            List<string> a = DistinctBuilds(suitesA);
            List<string> b = DistinctBuilds(suitesB);
            if (a.Count != 1 || b.Count != 1)
            {
                Line(sb, "Differences: cannot compare (multiple builds within an arm).");
                return;
            }

            string[] pa = a[0].Split('|');
            string[] pb = b[0].Split('|');
            string[] names = { "App version", "Commit", "SkiaSharp", "Framework", "Runtime" };
            List<string> diffs = new List<string>();
            for (int i = 0; i < names.Length; i++)
            {
                if (pa[i] != pb[i])
                    diffs.Add(names[i] + " " + pa[i] + " -> " + pb[i]);
            }

            if (diffs.Count == 0)
            {
                Line(sb, "Differences: none");
                return;
            }
            Line(sb, "Differences:");
            for (int i = 0; i < diffs.Count; i++)
                Line(sb, Truncate("  " + diffs[i], MaxLineWidth));
        }

        /* One '|'-joined combo per distinct (AppVersion, short commit, SkiaSharp,
           Framework, Runtime) tuple found across a suite list, in first-seen order. */
        private static List<string> DistinctBuilds(IList<GHReportSuite> suites)
        {
            List<string> list = new List<string>();
            if (suites == null)
                return list;
            for (int i = 0; i < suites.Count; i++)
            {
                GHReportSuite s = suites[i];
                string combo = OrNA(s.AppVersion) + "|" + ShortHash(s.GitCommit, 9) + "|"
                    + OrNA(s.SkiaSharpVersion) + "|" + OrNA(s.FrameworkVersion) + "|"
                    + OrNA(s.RuntimeVersion);
                if (!list.Contains(combo))
                    list.Add(combo);
            }
            return list;
        }

        private static void AppendVerdictTable(StringBuilder sb, GHComparisonResult result)
        {
            Line(sb, "Verdicts:");
            if (result.Decisions == null || result.Decisions.Count == 0)
            {
                Line(sb, "  none");
                return;
            }
            for (int i = 0; i < result.Decisions.Count; i++)
            {
                GHMetricDecision d = result.Decisions[i];
                string name = d.Metric != null ? d.Metric.Name : "?";
                string unit = d.Metric != null ? d.Metric.Unit : "";
                Line(sb, Truncate("  " + Truncate(name, 24) + " (" + OrNA(unit) + ")",
                    MaxLineWidth));
                Line(sb, Truncate("    A " + Fmt(d.MedianA) + "  B " + Fmt(d.MedianB)
                    + "  Diff " + SignedFmt(d.Diff)
                    + "  CI [" + Fmt(d.Ci.Low) + ", " + Fmt(d.Ci.High) + "]"
                    + "  MDE +/-" + Fmt(d.Mde), MaxLineWidth));
                Line(sb, Truncate("    Verdict: " + OrNA(d.Verdict), MaxLineWidth));
            }
        }

        private static void AppendOverallLine(StringBuilder sb, GHComparisonResult result)
        {
            if (result.TooFewRuns)
            {
                Line(sb, "No decision: fewer than 3 used runs per arm (a and b).");
                return;
            }

            string text = result.Regressions > 0
                ? result.Regressions.ToString(CultureInfo.InvariantCulture)
                    + " decision metric(s) regressed."
                : "No decision metric regressed.";
            text += "  " + result.Improvements.ToString(CultureInfo.InvariantCulture)
                + " improved.";
            Line(sb, Truncate(text, MaxLineWidth));
            if (result.Provisional)
                Line(sb, "Fewer than 5 runs per arm: provisional; read the MDE.");
        }

        /* ---- recent hitches ---- */

        private const int WorstHitchCount = 10;
        private const int MarkHitchLimit = 10;
        private const int MarkWindowSeconds = 3;
        private const string DetailIndent = "     ";

        private static double TicksToMs(long ticks)
        {
            return ticks * 1000.0 / Stopwatch.Frequency;
        }

        private static string RelativeSeconds(long ticks, long spanEnd)
        {
            return SignedFmt((double)(ticks - spanEnd) / Stopwatch.Frequency) + " s";
        }

        private static string LocalClock(long utcTicks)
        {
            if (utcTicks <= 0 || utcTicks > DateTime.MaxValue.Ticks)
                return "??:??:??.???";
            return new DateTime(utcTicks, DateTimeKind.Utc).ToLocalTime()
                .ToString("HH:mm:ss.fff", CultureInfo.InvariantCulture);
        }

        /* Index of the record with frameId, -1 when absent; records are in FrameId order */
        private static int FindRecord(GHFrameRecord[] records, int count, long frameId)
        {
            int lo = 0, hi = count - 1;
            while (lo <= hi)
            {
                int mid = (lo + hi) / 2;
                long id = records[mid].FrameId;
                if (id == frameId)
                    return mid;
                if (id < frameId)
                    lo = mid + 1;
                else
                    hi = mid - 1;
            }
            return -1;
        }

        /* A displayed frame that ended a hitch, with the records on both sides of its gap
           among the first recordCount */
        private static bool IsReportableHitch(GHDisplayedFrame[] displayed, int j, int recordCount)
        {
            return j > 0 && displayed[j].IsHitch
                && displayed[j].RecordIndex >= 0 && displayed[j].RecordIndex < recordCount
                && displayed[j - 1].RecordIndex >= 0 && displayed[j - 1].RecordIndex < recordCount;
        }

        /* Larger gap first, then earlier first */
        private static void SortByGapDescending(List<int> indexes, GHDisplayedFrame[] displayed)
        {
            indexes.Sort(delegate (int a, int b)
            {
                int cmp = displayed[b].GapTicks.CompareTo(displayed[a].GapTicks);
                return cmp != 0 ? cmp : a.CompareTo(b);
            });
        }

        /* "<time>  gap <ms> ms (+<ms> over target)  <cause>" */
        private static string HitchText(GHDisplayedFrame f, long spanEnd)
        {
            return RelativeSeconds(f.DisplayedAtTicks, spanEnd)
                + "  gap " + Fmt(TicksToMs(f.GapTicks)) + " ms"
                + " (" + SignedFmt(TicksToMs(f.GapTicks - f.TargetPeriodTicks)) + " over target)"
                + "  " + GHSmoothnessMetrics.CauseName(f.Cause);
        }

        /* For each mark, oldest first: its local time, its time in the span, and the
           hitches displayed within MarkWindowSeconds of the marked tick's callback start;
           past MarkHitchLimit of them, the largest are listed in time order */
        private static void AppendMarkedMoments(StringBuilder sb, GHFrameRecord[] records, int recordCount,
            GHDisplayedFrame[] displayed, int displayedCount, long spanEnd,
            long[] markFrameIds, long[] markUtcTicks, int markCount)
        {
            Line(sb, "Marked moments:");
            if (markCount == 0)
            {
                Line(sb, "  none");
                return;
            }
            long window = MarkWindowSeconds * Stopwatch.Frequency;
            string windowText = MarkWindowSeconds.ToString(CultureInfo.InvariantCulture) + " s";
            List<int> near = new List<int>();
            for (int i = 0; i < markCount; i++)
            {
                string when = LocalClock(markUtcTicks[i]);
                string frame = "frame " + markFrameIds[i].ToString(CultureInfo.InvariantCulture);
                int idx = FindRecord(records, recordCount, markFrameIds[i]);
                if (idx < 0)
                {
                    Line(sb, Truncate("  " + when + "  " + frame + " is not in the span", MaxLineWidth));
                    continue;
                }
                long markTicks = records[idx].CallbackStartTicks;
                Line(sb, Truncate("  " + when + "  " + RelativeSeconds(markTicks, spanEnd) + "  " + frame,
                    MaxLineWidth));

                near.Clear();
                for (int j = 1; j < displayedCount; j++)
                {
                    if (IsReportableHitch(displayed, j, recordCount)
                        && Math.Abs(displayed[j].DisplayedAtTicks - markTicks) <= window)
                        near.Add(j);
                }
                if (near.Count == 0)
                {
                    Line(sb, "    no hitch within " + windowText);
                    continue;
                }
                int more = 0;
                if (near.Count > MarkHitchLimit)
                {
                    more = near.Count - MarkHitchLimit;
                    SortByGapDescending(near, displayed);
                    near.RemoveRange(MarkHitchLimit, more);
                    near.Sort();
                }
                for (int k = 0; k < near.Count; k++)
                    Line(sb, Truncate("    " + HitchText(displayed[near[k]], spanEnd), MaxLineWidth));
                if (more > 0)
                    Line(sb, "    ... and " + more.ToString(CultureInfo.InvariantCulture)
                        + " smaller hitch(es) within " + windowText);
            }
        }

        /* The WorstHitchCount largest hitches: the gap, the paint stages of the tick that
           ended it, the request work and collections during it, and its content events */
        private static void AppendWorstHitches(StringBuilder sb, GHFrameRecord[] records, int recordCount,
            GHDisplayedFrame[] displayed, int displayedCount, long spanEnd, bool pauseAvailable)
        {
            Line(sb, "Worst hitches (largest gap first):");
            List<int> hitches = new List<int>();
            for (int j = 1; j < displayedCount; j++)
            {
                if (IsReportableHitch(displayed, j, recordCount))
                    hitches.Add(j);
            }
            if (hitches.Count == 0)
            {
                Line(sb, "  none");
                return;
            }
            SortByGapDescending(hitches, displayed);
            int shown = Math.Min(WorstHitchCount, hitches.Count);
            for (int k = 0; k < shown; k++)
            {
                int j = hitches[k];
                string rank = PadL((k + 1).ToString(CultureInfo.InvariantCulture), 3) + ". ";
                Line(sb, Truncate(rank + HitchText(displayed[j], spanEnd), MaxLineWidth));
                Line(sb, Truncate(DetailIndent
                    + StageText(records, displayed[j - 1].RecordIndex, displayed[j].RecordIndex, pauseAvailable),
                    MaxLineWidth));
                AppendEventNames(sb, displayed[j].ContentEvents);
            }
        }

        /* Draw and flush time of the tick that ended a gap, the request work handled
           during the gap, and whether a collection ran in it, with its pause time when
           the runtime reports one; the same derivations GHSmoothnessMetrics.Attribute uses */
        private static string StageText(GHFrameRecord[] records, int prevIdx, int curIdx, bool pauseAvailable)
        {
            GHFrameRecord r = records[curIdx];
            GHFrameRecord p = records[prevIdx];
            string draw = r.PaintStartTicks != 0 && r.DrawEndTicks != 0
                ? Fmt(TicksToMs(r.DrawEndTicks - r.PaintStartTicks)) + " ms" : "n/a";
            string flush = r.DrawEndTicks != 0 && r.FlushEndTicks != 0
                ? Fmt(TicksToMs(r.FlushEndTicks - r.DrawEndTicks)) + " ms" : "n/a";
            long requestTicks = 0;
            for (int i = prevIdx + 1; i <= curIdx; i++)
                requestTicks += records[i].RequestTicks;
            bool countsMoved = r.GcCount0 != p.GcCount0 || r.GcCount1 != p.GcCount1 || r.GcCount2 != p.GcCount2;
            long pauseTicks = pauseAvailable ? Math.Max(0, r.GcPauseTicks - p.GcPauseTicks) : 0;
            string gc;
            if (!countsMoved && pauseTicks == 0)
                gc = "no";
            else if (pauseAvailable)
                gc = "yes (" + Fmt(TicksToMs(pauseTicks)) + " ms)";
            else
                gc = "yes";
            return "draw " + draw + "  flush " + flush + "  requests " + Fmt(TicksToMs(requestTicks)) + " ms"
                + "  GC " + gc;
        }

        /* "events:" and the content events of a gap, wrapped at MaxLineWidth */
        private static void AppendEventNames(StringBuilder sb, GHContentEvent events)
        {
            string line = DetailIndent + "events:";
            bool any = false;
            for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
            {
                if (((int)events & (1 << k)) == 0)
                    continue;
                string name = GHSmoothnessMetrics.ContentEventName(k);
                string piece = (any ? ", " : " ") + name;
                if (any && line.Length + piece.Length + 1 > MaxLineWidth)
                {
                    Line(sb, line + ",");
                    line = DetailIndent + "  " + name;
                }
                else
                {
                    line += piece;
                }
                any = true;
            }
            if (!any)
                line += " none";
            Line(sb, Truncate(line, MaxLineWidth));
        }

        /* Hitch counts and time beyond target per cause, from the displayed frames */
        private static void AppendRecentCauseTotals(StringBuilder sb, GHDisplayedFrame[] displayed, int displayedCount)
        {
            Line(sb, "Hitch causes (span):");
            int[] counts = new int[GHSmoothnessMetrics.CauseCount];
            double[] ms = new double[GHSmoothnessMetrics.CauseCount];
            for (int j = 1; j < displayedCount; j++)
            {
                if (!displayed[j].IsHitch)
                    continue;
                int c = (int)displayed[j].Cause;
                if (c >= GHSmoothnessMetrics.CauseCount)
                    continue;
                counts[c]++;
                ms[c] += TicksToMs(displayed[j].GapTicks - displayed[j].TargetPeriodTicks);
            }
            AppendCauseRows(sb, counts, ms);
        }

        /* ---- run selection ---- */

        /* Runs counted toward medians and totals: not the warm-up run, no exclusion
           reason, and a summary. */
        private static List<GHReportRun> UsedRuns(GHReportSuite suite)
        {
            List<GHReportRun> list = new List<GHReportRun>();
            if (suite == null)
                return list;
            for (int i = 0; i < suite.Runs.Count; i++)
            {
                GHReportRun r = suite.Runs[i];
                if (IsUsed(r))
                    list.Add(r);
            }
            return list;
        }

        private static bool IsUsed(GHReportRun r)
        {
            return r != null && !r.IsWarmUp && r.ExcludedReason == null && r.Summary != null;
        }

        private static List<GHReportRun> UsedRunsOfSuites(IList<GHReportSuite> suites)
        {
            List<GHReportRun> list = new List<GHReportRun>();
            if (suites == null)
                return list;
            for (int i = 0; i < suites.Count; i++)
                list.AddRange(UsedRuns(suites[i]));
            return list;
        }

        /* ---- formatting helpers ---- */

        private static void Line(StringBuilder sb, string text)
        {
            sb.Append(text);
            sb.Append("\n");
        }

        private static string OrNA(string s)
        {
            return string.IsNullOrEmpty(s) ? "n/a" : s;
        }

        private static string ShortHash(string s, int len)
        {
            if (string.IsNullOrEmpty(s))
                return "n/a";
            return s.Length <= len ? s : s.Substring(0, len);
        }

        private static string Fmt(double v)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
                return "n/a";
            return v.ToString("0.00", CultureInfo.InvariantCulture);
        }

        private static string SignedFmt(double v)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
                return "n/a";
            string sign = v >= 0 ? "+" : "";
            return sign + v.ToString("0.00", CultureInfo.InvariantCulture);
        }

        /* Median of a small, unsorted list; NaN for an empty list. Sorts a copy. */
        private static double Median(List<double> values)
        {
            if (values == null || values.Count == 0)
                return double.NaN;
            double[] a = values.ToArray();
            Array.Sort(a);
            int n = a.Length;
            if (n % 2 == 1)
                return a[n / 2];
            return (a[n / 2 - 1] + a[n / 2]) / 2.0;
        }

        private static string Truncate(string s, int maxLen)
        {
            if (s == null)
                return "";
            if (maxLen <= 0)
                return "";
            if (s.Length <= maxLen)
                return s;
            if (maxLen <= 3)
                return s.Substring(0, maxLen);
            return s.Substring(0, maxLen - 3) + "...";
        }

        private static string PadR(string s, int width)
        {
            if (s == null)
                s = "";
            if (s.Length >= width)
                return s.Substring(0, width);
            return s + new string(' ', width - s.Length);
        }

        private static string PadL(string s, int width)
        {
            if (s == null)
                s = "";
            if (s.Length >= width)
                return s.Substring(0, width);
            return new string(' ', width - s.Length) + s;
        }
    }
}

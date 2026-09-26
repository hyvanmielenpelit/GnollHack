using System;
using System.Collections.Generic;
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

    /* Renders a GHReportSuite or a GHComparisonResult as plain, fixed-width text for
       the phone report viewer and the share zip. Every line is at most MaxLineWidth
       characters and lines are joined with "\n" only. Deterministic: nothing here
       reads the current time, so the same inputs always render the same text. Must
       compile under C# 7.3. */
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

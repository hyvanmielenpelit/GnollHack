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
        public int TurnAtWindowStart = -1;          /* -1 when not recorded */
        public long InputRecordsInWindow = -1;      /* -1 when not recorded */
        public string ExcludedReason;
        public string BackgroundVerdict;            /* a GHBackgroundLoad verdict name; null when not recorded */
        public double OtherCpuP90Pct = double.NaN;  /* NaN when not recorded */
        public string BackgroundReason;             /* set for elevated and busy runs */
        public string Notes;                        /* e.g. a quiet gate timeout before the run */
    }

    /* The most recent earlier finished suite with the same comparability key on the same
       hardware, as the suite report describes it. Changes is the fingerprint diff from
       that suite to this one. */
    public sealed class GHReportPreviousSuite
    {
        public string SuiteId;
        public string ArmLabel;
        public DateTime StartedUtc;
        public int RunsUsed;
        public double MedianHitchRatioMsPerSec = double.NaN;
        public readonly List<GHFingerprintChange> Changes = new List<GHFingerprintChange>();
    }

    /* What a comparison report adds to the comparison itself: each arm's pooled
       fingerprint (null computes it from the arm's suites), the number of used runs of
       both arms with an elevated background verdict, and the comparison recomputed
       without them (null when it could not be computed). */
    public sealed class GHReportComparisonContext
    {
        public Dictionary<string, string> FingerprintA;
        public Dictionary<string, string> FingerprintB;
        public int ElevatedRuns;
        public GHComparisonResult WithoutElevated;
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
        public string ThermalGate;
        public int ThermalWaitSeconds;
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
        public Dictionary<string, string> Fingerprint;       /* at the suite start; null when not recorded */
        public Dictionary<string, string> FingerprintAtEnd;  /* null when not recorded */
        public bool EnvironmentChanged;
        public GHReportPreviousSuite Previous;                /* null when there is none */
        public bool? BackgroundSamplerEnabled;                /* null when not recorded */
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
        private const int BgColW = 5;
        private const int TurnColW = 5;
        private const int StatusColW = MaxLineWidth
            - (RunColW + FpsColW + HitchColW + PaceColW + JudderColW + DropColW
               + GcColW + GcMsColW + BgColW + TurnColW);

        /* Width of the category name in the environment section */
        private const int CategoryColW = 11;
        private const int MaxSuspects = 3;

        /* The first metrics version whose summaries carry CauseHitchCount */
        private const int FirstHitchCountVersion = 2;

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
            AppendEnvironment(sb, suite);
            Line(sb, "");
            AppendRunTable(sb, suite);
            Line(sb, "");
            AppendBackgroundSummary(sb, suite);
            Line(sb, "");
            List<GHReportRun> used = UsedRuns(suite);
            AppendMedians(sb, used);
            Line(sb, "");
            AppendPreviousSuite(sb, suite, used);
            Line(sb, "");
            AppendCauseTotals(sb, used);
            Line(sb, "");
            AppendContentEvents(sb, used);
            return sb.ToString();
        }

        public static string ComparisonReport(GHComparisonResult result,
            IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB)
        {
            return ComparisonReport(result, suitesA, suitesB, null);
        }

        /* The comparison report with each arm's builds and a warning when an arm pools
           more than one (BuildIdentityWarnings), the environment differences and the
           attribution, the suites in start order with warnings about the arm order, a
           warning when the arms played different playback content
           (ContentCoverageWarning, for the scenario of the arms' suites) and, when
           context counts elevated runs, the sensitivity line; a null context pools each
           arm's fingerprint from its suites and prints no sensitivity line. */
        public static string ComparisonReport(GHComparisonResult result,
            IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB, GHReportComparisonContext context)
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
            AppendLines(sb, BuildIdentityWarnings(suitesA, "A"));
            AppendLines(sb, BuildIdentityWarnings(suitesB, "B"));
            AppendEnvironmentDifferences(sb, suitesA, suitesB, context);
            Line(sb, "");
            AppendSuiteOrder(sb, suitesA, suitesB);
            string coverage = ContentCoverageWarning(suitesA, suitesB, ScenarioOf(suitesA, suitesB));
            if (coverage != null)
                AppendLines(sb, new List<string>(coverage.Split('\n')));
            Line(sb, "");
            AppendVerdictTable(sb, result);
            Line(sb, "");
            AppendOverallLine(sb, result);
            AppendSensitivity(sb, result, context);
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
            Line(sb, summary.PresentSource == GHPresentSource.Measured
                ? "Display times: measured" : "Display times: estimated (first vsync after ready)");
            if (summary.CompositorReportsLost > 0)
                Line(sb, Truncate("FrameMetrics reports lost: "
                    + summary.CompositorReportsLost.ToString(CultureInfo.InvariantCulture)
                    + " (compositor data incomplete)", MaxLineWidth));
            if (summary.LongStallCount > 0)
                Line(sb, Truncate(LongStallText(summary.LongStallCount, summary.LongStallMs,
                    summary.LongStallsExcluded), MaxLineWidth));
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
            Line(sb, Truncate("Thermal gate: " + GHPerformanceSuiteLogic.ThermalGateDisplayName(suite.ThermalGate)
                + "  Thermal wait " + suite.ThermalWaitSeconds.ToString(CultureInfo.InvariantCulture) + "s",
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

        /* One line per fingerprint category with its short hash and key count, then the
           keys that changed between the suite's start and end when they did. */
        private static void AppendEnvironment(StringBuilder sb, GHReportSuite suite)
        {
            Line(sb, "Environment:");
            Dictionary<string, string> fp = suite.Fingerprint;
            if (fp == null || fp.Count == 0)
            {
                Line(sb, "  not recorded");
                return;
            }
            List<string> categories = FingerprintCategories(fp);
            for (int i = 0; i < categories.Count; i++)
            {
                int count = CountCategoryKeys(fp, categories[i]);
                Line(sb, Truncate("  " + PadR(categories[i], CategoryColW)
                    + GHEnvironmentFingerprint.ShortHash(fp, categories[i])
                    + "  " + count.ToString(CultureInfo.InvariantCulture) + (count == 1 ? " key" : " keys"),
                    MaxLineWidth));
            }
            if (!fp.ContainsKey(GHEnvironmentFingerprint.MetaFingerprintVersionKey))
                Line(sb, "  (legacy record: per-field environment values only)");
            if (suite.EnvironmentChanged)
            {
                Line(sb, "Environment changed during the suite:");
                GHEnvironmentFingerprint.AppendReportLines(sb,
                    CountedChanges(GHEnvironmentFingerprint.Diff(fp, suite.FingerprintAtEnd)), MaxLineWidth);
            }
        }

        /* The categories of fp other than meta, in attribution order, then ordinally */
        private static List<string> FingerprintCategories(Dictionary<string, string> fp)
        {
            List<string> categories = new List<string>();
            foreach (KeyValuePair<string, string> kv in fp)
            {
                string category = GHEnvironmentFingerprint.CategoryOf(kv.Key);
                if (category != GHEnvironmentFingerprint.CategoryMeta && !categories.Contains(category))
                    categories.Add(category);
            }
            categories.Sort(delegate (string x, string y)
            {
                int c = GHEnvironmentFingerprint.CategoryRank(x).CompareTo(GHEnvironmentFingerprint.CategoryRank(y));
                return c != 0 ? c : string.CompareOrdinal(x, y);
            });
            return categories;
        }

        private static int CountCategoryKeys(Dictionary<string, string> fp, string category)
        {
            int n = 0;
            foreach (KeyValuePair<string, string> kv in fp)
            {
                if (GHEnvironmentFingerprint.CategoryOf(kv.Key) == category)
                    n++;
            }
            return n;
        }

        /* The Changed, Added and Removed entries of changes */
        private static List<GHFingerprintChange> CountedChanges(List<GHFingerprintChange> changes)
        {
            List<GHFingerprintChange> list = new List<GHFingerprintChange>();
            for (int i = 0; i < changes.Count; i++)
            {
                GHFingerprintChangeKind kind = changes[i].Kind;
                if (kind == GHFingerprintChangeKind.Changed || kind == GHFingerprintChangeKind.Added
                    || kind == GHFingerprintChangeKind.Removed)
                    list.Add(changes[i]);
            }
            return list;
        }

        /* The suite's own fingerprint, or one mapped from its per-field environment when
           it has none */
        public static Dictionary<string, string> EffectiveFingerprint(GHReportSuite suite)
        {
            if (suite == null)
                return null;
            if (suite.Fingerprint != null && suite.Fingerprint.Count > 0)
                return suite.Fingerprint;
            return GHEnvironmentFingerprint.FromLegacyFields(suite.AppVersion, suite.GitCommit, suite.BuildConfiguration,
                suite.RuntimeVersion, suite.FrameworkVersion, suite.UiFrameworkVersion, suite.SkiaSharpVersion,
                suite.FmodVersion, suite.Platform, suite.DeviceOs, suite.DeviceModel, null, null, null);
        }

        /* The common values (GHEnvironmentFingerprint.CommonValues) of the suites'
           effective fingerprints */
        public static Dictionary<string, string> PooledFingerprint(IList<GHReportSuite> suites)
        {
            List<IDictionary<string, string>> list = new List<IDictionary<string, string>>();
            if (suites != null)
            {
                for (int i = 0; i < suites.Count; i++)
                    list.Add(EffectiveFingerprint(suites[i]));
            }
            return GHEnvironmentFingerprint.CommonValues(list);
        }

        /* Counts of background verdicts over every run and the suspects the elevated and
           busy runs name most often */
        private static void AppendBackgroundSummary(StringBuilder sb, GHReportSuite suite)
        {
            int quiet = 0, elevated = 0, busy = 0, unknown = 0, none = 0;
            Dictionary<string, int> suspects = new Dictionary<string, int>(StringComparer.Ordinal);
            for (int i = 0; i < suite.Runs.Count; i++)
            {
                GHReportRun run = suite.Runs[i];
                string verdict = run.BackgroundVerdict;
                if (string.IsNullOrEmpty(verdict))
                    none++;
                else if (verdict == GHBackgroundLoad.VerdictQuietName)
                    quiet++;
                else if (verdict == GHBackgroundLoad.VerdictElevatedName)
                    elevated++;
                else if (verdict == GHBackgroundLoad.VerdictBusyName)
                    busy++;
                else
                    unknown++;

                string reason = run.BackgroundReason;
                if (string.IsNullOrEmpty(reason) && run.ExcludedReason != null
                    && run.ExcludedReason.StartsWith(GHBackgroundLoad.ReasonPrefix, StringComparison.Ordinal))
                    reason = run.ExcludedReason;
                List<string> named = SuspectsOf(reason);
                for (int j = 0; j < named.Count; j++)
                {
                    int n;
                    suspects.TryGetValue(named[j], out n);
                    suspects[named[j]] = n + 1;
                }
            }

            if (quiet + elevated + busy + unknown == 0)
            {
                Line(sb, "Background load: not recorded"
                    + (suite.BackgroundSamplerEnabled == false ? " (sampler off)" : ""));
                return;
            }
            Line(sb, "Background load:");
            List<string> parts = new List<string>();
            AddCount(parts, quiet, GHBackgroundLoad.VerdictQuietName);
            AddCount(parts, elevated, GHBackgroundLoad.VerdictElevatedName);
            AddCount(parts, busy, GHBackgroundLoad.VerdictBusyName);
            AddCount(parts, unknown, GHBackgroundLoad.VerdictUnknownName);
            AddCount(parts, none, "not recorded");
            Line(sb, Truncate("  " + string.Join(", ", parts.ToArray()), MaxLineWidth));

            if (suspects.Count == 0)
                return;
            List<KeyValuePair<string, int>> ranked = new List<KeyValuePair<string, int>>(suspects);
            ranked.Sort(delegate (KeyValuePair<string, int> x, KeyValuePair<string, int> y)
            {
                int c = y.Value.CompareTo(x.Value);
                return c != 0 ? c : string.CompareOrdinal(x.Key, y.Key);
            });
            StringBuilder text = new StringBuilder("  Suspects: ");
            for (int i = 0; i < ranked.Count && i < MaxSuspects; i++)
            {
                if (i > 0)
                    text.Append(", ");
                text.Append(ranked[i].Key);
                text.Append(" (");
                text.Append(ranked[i].Value.ToString(CultureInfo.InvariantCulture));
                text.Append(ranked[i].Value == 1 ? " run)" : " runs)");
            }
            Line(sb, Truncate(text.ToString(), MaxLineWidth));
        }

        private static void AddCount(List<string> parts, int count, string name)
        {
            if (count > 0)
                parts.Add(count.ToString(CultureInfo.InvariantCulture) + " " + name);
        }

        /* The distinct suspects a background reason names: the known activities among
           its facts ("wsl-vm 22 %") and the categories of its named processes
           ("wsl-vm (vmmemWSL 22 %)"), a process of the "other" category by its name. */
        private static List<string> SuspectsOf(string reason)
        {
            List<string> found = new List<string>();
            if (string.IsNullOrEmpty(reason) || !reason.StartsWith(GHBackgroundLoad.ReasonPrefix, StringComparison.Ordinal))
                return found;
            string body = reason.Substring(GHBackgroundLoad.ReasonPrefix.Length);
            if (body.EndsWith("...", StringComparison.Ordinal))
                body = body.Substring(0, body.Length - 3);
            string[] groups = body.Split(new string[] { "; " }, StringSplitOptions.None);
            for (int g = 0; g < groups.Length; g++)
            {
                string[] pieces = groups[g].Split(new string[] { ", " }, StringSplitOptions.None);
                for (int i = 0; i < pieces.Length; i++)
                {
                    string piece = pieces[i].Trim();
                    string suspect = null;
                    if (g == 0)
                    {
                        int space = piece.IndexOf(' ');
                        if (space > 0 && space + 1 < piece.Length && char.IsDigit(piece[space + 1])
                            && IsActivityCategory(piece.Substring(0, space)))
                            suspect = piece.Substring(0, space);
                    }
                    else
                    {
                        int paren = piece.IndexOf(" (", StringComparison.Ordinal);
                        if (paren > 0)
                        {
                            suspect = piece.Substring(0, paren);
                            if (suspect == GHBackgroundLoad.CategoryOther)
                            {
                                string rest = piece.Substring(paren + 2);
                                int space = rest.IndexOf(' ');
                                suspect = space > 0 ? rest.Substring(0, space) : null;
                            }
                        }
                    }
                    if (!string.IsNullOrEmpty(suspect) && !found.Contains(suspect))
                        found.Add(suspect);
                }
            }
            return found;
        }

        private static bool IsActivityCategory(string name)
        {
            return name == GHBackgroundLoad.CategoryWindowsUpdate || name == GHBackgroundLoad.CategoryAntivirus
                || name == GHBackgroundLoad.CategoryIndexer || name == GHBackgroundLoad.CategoryWslVm
                || name == GHBackgroundLoad.CategoryBuildTools || name == GHBackgroundLoad.CategorySync
                || name == GHBackgroundLoad.CategoryTelemetry;
        }

        /* The previous comparable suite's median against this one's and the fingerprint
           changes since it; descriptive only */
        private static void AppendPreviousSuite(StringBuilder sb, GHReportSuite suite, List<GHReportRun> used)
        {
            GHReportPreviousSuite p = suite.Previous;
            if (p == null)
            {
                Line(sb, "Previous comparable suite: none");
                return;
            }
            List<double> hitch = new List<double>();
            for (int i = 0; i < used.Count; i++)
                hitch.Add(used[i].Summary.HitchRatioMsPerSec);

            Line(sb, "Changes since the previous comparable suite:");
            Line(sb, Truncate("  " + OrNA(p.ArmLabel) + ", started "
                + p.StartedUtc.ToString("yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture) + " UTC, "
                + p.RunsUsed.ToString(CultureInfo.InvariantCulture) + " used run(s)", MaxLineWidth));
            Line(sb, Truncate("  Median hitch: " + Fmt(p.MedianHitchRatioMsPerSec) + " ms/s then, "
                + Fmt(GHPerformanceStatistics.MedianNearestRank(hitch)) + " ms/s now", MaxLineWidth));
            GHEnvironmentFingerprint.AppendReportLines(sb, p.Changes, MaxLineWidth);
            Line(sb, "  Descriptive only: the baseline comparison is the decision.");
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
                + PadR("Bg", BgColW) + PadR("Turn", TurnColW) + PadR("Status", StatusColW);
        }

        /* The background verdict's letter (q quiet, e elevated, B busy), followed by the
           other CPU P90 as an integer when known; "?" when unknown or not recorded */
        private static string BackgroundLabel(GHReportRun run)
        {
            string letter;
            if (run.BackgroundVerdict == GHBackgroundLoad.VerdictQuietName)
                letter = "q";
            else if (run.BackgroundVerdict == GHBackgroundLoad.VerdictElevatedName)
                letter = "e";
            else if (run.BackgroundVerdict == GHBackgroundLoad.VerdictBusyName)
                letter = "B";
            else
                return "?";
            if (double.IsNaN(run.OtherCpuP90Pct) || double.IsInfinity(run.OtherCpuP90Pct))
                return letter;
            return letter + Math.Max(0, run.OtherCpuP90Pct).ToString("F0", CultureInfo.InvariantCulture);
        }

        private static void AppendRunRow(StringBuilder sb, GHReportRun run)
        {
            string runLabel = run.IsWarmUp ? "W" : run.Index.ToString(CultureInfo.InvariantCulture);
            string turnLabel = run.TurnReached >= 0
                ? run.TurnReached.ToString(CultureInfo.InvariantCulture) : "?";
            string bgLabel = BackgroundLabel(run);
            GHSmoothnessSummary s = run.Summary;
            string mainPart;
            string status;

            if (s == null)
            {
                mainPart = PadR(runLabel, RunColW) + PadR("n/a", FpsColW)
                    + PadR("n/a", HitchColW) + PadR("n/a", PaceColW) + PadR("n/a", JudderColW)
                    + PadR("n/a", DropColW) + PadR("n/a", GcColW) + PadR("n/a", GcMsColW)
                    + PadR(bgLabel, BgColW) + PadR(turnLabel, TurnColW);
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
                    + PadR(bgLabel, BgColW)
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
            if (!string.IsNullOrEmpty(run.Notes))
                Line(sb, "    note: " + Truncate(run.Notes, MaxLineWidth - 10));
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
            int stallCount = 0;
            double stallMs = 0;
            for (int i = 0; i < used.Count; i++)
            {
                GHSmoothnessSummary s = used[i].Summary;
                fps.Add(s.DisplayedFps);
                hitch.Add(s.HitchRatioMsPerSec);
                pace.Add(s.PacingErrorRmsMs);
                judder.Add(s.JudderPct);
                gc.Add(s.GcCount);
                stallCount += s.LongStallCount;
                stallMs += s.LongStallMs;
            }

            Line(sb, Truncate("  FPS " + Fmt(GHPerformanceStatistics.MedianNearestRank(fps))
                + "  Hitch " + Fmt(GHPerformanceStatistics.MedianNearestRank(hitch)) + "ms/s"
                + "  PaceRMS " + Fmt(GHPerformanceStatistics.MedianNearestRank(pace)) + "ms"
                + "  Judder " + Fmt(GHPerformanceStatistics.MedianNearestRank(judder)) + "%"
                + "  GC " + Fmt(GHPerformanceStatistics.MedianNearestRank(gc)), MaxLineWidth));
            Line(sb, "  Display times are estimated in-app; Compositor and Dropped are inferred.");
            /* A measurement window counts its long stalls as hitches */
            if (stallCount > 0)
                Line(sb, Truncate("  " + LongStallText(stallCount, stallMs, false), MaxLineWidth));
        }

        /* "Stalls over 1 s: N (X ms), " and how the hitch metrics treated them */
        private static string LongStallText(int count, double ms, bool excluded)
        {
            return "Stalls over " + GHSmoothnessMetrics.LongStallSeconds.ToString("0.##", CultureInfo.InvariantCulture)
                + " s: " + count.ToString(CultureInfo.InvariantCulture) + " (" + Fmt(ms) + " ms), "
                + (excluded ? "excluded from hitch time" : "counted as hitches");
        }

        /* Hitch counts per cause when every used run was analyzed at metrics version 2 or
           later; otherwise CauseCount, which also counts judder frames, with a note */
        private static void AppendCauseTotals(StringBuilder sb, List<GHReportRun> used)
        {
            Line(sb, "Hitch causes (used runs, summed):");
            bool hitchCounts = true;
            for (int i = 0; i < used.Count; i++)
            {
                GHSmoothnessSummary s = used[i].Summary;
                if (s.MetricsVersion < FirstHitchCountVersion || s.CauseHitchCount == null)
                    hitchCounts = false;
            }
            int[] counts = new int[GHSmoothnessMetrics.CauseCount];
            double[] ms = new double[GHSmoothnessMetrics.CauseCount];
            for (int i = 0; i < used.Count; i++)
            {
                GHSmoothnessSummary s = used[i].Summary;
                int[] source = hitchCounts ? s.CauseHitchCount : s.CauseCount;
                for (int c = 0; c < GHSmoothnessMetrics.CauseCount; c++)
                {
                    counts[c] += source[c];
                    ms[c] += s.CauseMs[c];
                }
            }
            AppendCauseRows(sb, counts, ms);
            if (!hitchCounts)
                Line(sb, "  counts include judder frames (recorded before metrics version 2)");
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
                    + (parts[5] != "n/a" ? "  id " + parts[5] : "")
                    + "  Skia " + parts[2] + "  Framework " + parts[3] + "  Runtime " + parts[4];
                Line(sb, Truncate(text, MaxLineWidth));
            }
        }

        /* The code identity keys BuildIdentityWarnings compares */
        private static readonly string[] BuildIdentityKeys =
        {
            GHEnvironmentFingerprint.CodeAppVersionKey,
            GHEnvironmentFingerprint.CodeGitCommitKey,
            GHPerformanceSuiteLogic.CodeAssemblyMvidKey
        };

        /* A warning, as report lines, when the suites of one arm were measured on more
           than one build: some code.appVersion, code.gitCommit or code.assemblyMvid
           differs between two suites whose effective fingerprints both have it. Empty
           when the arm is one build. */
        public static List<string> BuildIdentityWarnings(IList<GHReportSuite> suites, string arm)
        {
            List<string> lines = new List<string>();
            if (suites == null || suites.Count < 2)
                return lines;
            List<string> differing = new List<string>();
            for (int k = 0; k < BuildIdentityKeys.Length; k++)
            {
                string first = null;
                for (int i = 0; i < suites.Count; i++)
                {
                    string value = FingerprintValue(EffectiveFingerprint(suites[i]), BuildIdentityKeys[k]);
                    if (string.IsNullOrEmpty(value))
                        continue;
                    if (first == null)
                    {
                        first = value;
                    }
                    else if (!string.Equals(first, value, StringComparison.Ordinal))
                    {
                        differing.Add(BuildIdentityKeys[k]);
                        break;
                    }
                }
            }
            if (differing.Count == 0)
                return lines;
            lines.Add(Truncate("Warning: arm " + OrNA(arm) + " pools suites of more than one build; keep one build per arm.",
                MaxLineWidth));
            lines.Add(Truncate("  Differs: " + string.Join(", ", differing.ToArray()), MaxLineWidth));
            return lines;
        }

        /* One line per suite of both arms, earliest start first, with its used runs,
           then the order of the arms, and warnings when every suite of one arm ran
           before every suite of the other, and when an arm has a single suite */
        private static void AppendSuiteOrder(StringBuilder sb, IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB)
        {
            List<KeyValuePair<string, GHReportSuite>> entries = new List<KeyValuePair<string, GHReportSuite>>();
            if (suitesA != null)
            {
                for (int i = 0; i < suitesA.Count; i++)
                    entries.Add(new KeyValuePair<string, GHReportSuite>("A", suitesA[i]));
            }
            if (suitesB != null)
            {
                for (int i = 0; i < suitesB.Count; i++)
                    entries.Add(new KeyValuePair<string, GHReportSuite>("B", suitesB[i]));
            }
            /* Earliest first; a tie keeps A before B, then the suite id's order */
            List<int> order = new List<int>();
            for (int i = 0; i < entries.Count; i++)
                order.Add(i);
            order.Sort(delegate (int x, int y)
            {
                GHReportSuite sx = entries[x].Value;
                GHReportSuite sy = entries[y].Value;
                DateTime tx = sx != null ? sx.StartedUtc : DateTime.MinValue;
                DateTime ty = sy != null ? sy.StartedUtc : DateTime.MinValue;
                int c = tx.CompareTo(ty);
                if (c != 0)
                    return c;
                c = string.CompareOrdinal(entries[x].Key, entries[y].Key);
                if (c != 0)
                    return c;
                c = string.CompareOrdinal(sx != null ? sx.SuiteId : null, sy != null ? sy.SuiteId : null);
                return c != 0 ? c : x.CompareTo(y);
            });

            Line(sb, "Suites (UTC start, arm, used runs):");
            if (entries.Count == 0)
            {
                Line(sb, "  none");
                return;
            }
            StringBuilder arms = new StringBuilder("Order:");
            int changes = 0;
            string previous = null;
            for (int i = 0; i < order.Count; i++)
            {
                KeyValuePair<string, GHReportSuite> entry = entries[order[i]];
                GHReportSuite suite = entry.Value;
                string start = suite != null
                    ? suite.StartedUtc.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture) : "n/a";
                int used = UsedRuns(suite).Count;
                Line(sb, Truncate("  " + start + "  " + entry.Key + "  "
                    + PadR(used.ToString(CultureInfo.InvariantCulture), 3) + OrNA(suite != null ? suite.SuiteId : null),
                    MaxLineWidth));
                arms.Append(' ');
                arms.Append(entry.Key);
                if (previous != null && previous != entry.Key)
                    changes++;
                previous = entry.Key;
            }
            Line(sb, Truncate(arms.ToString(), MaxLineWidth));

            int countA = suitesA != null ? suitesA.Count : 0;
            int countB = suitesB != null ? suitesB.Count : 0;
            if (countA > 0 && countB > 0 && changes == 1)
            {
                Line(sb, "Warning: every A suite ran before every B suite (or vice versa);");
                Line(sb, "  drift is confounded with the change. Interleave A B B A.");
            }
            if (countA == 1)
                Line(sb, "Warning: arm A has a single suite; run each arm at least twice (A B B A).");
            if (countB == 1)
                Line(sb, "Warning: arm B has a single suite; run each arm at least twice (A B B A).");
        }

        /* Measures of a run's playback content, best first */
        private const int ContentInputRecords = 0;
        private const int ContentTurnsInWindow = 1;
        private const int ContentTurnReached = 2;

        /* A warning, as "\n"-joined report lines, when the arms' used runs played
           different content; null when they did not, or when scenario is not
           "playback". A run's content is its input records in the window, failing that
           the turns in the window, and failing that the turn reached; both arms are
           measured by the best of these every used run has. The content differs when the
           arms' medians differ by more than max(1, 10 % of the larger) or their ranges are
           more than 1 apart. */
        public static string ContentCoverageWarning(IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB,
            string scenario)
        {
            if (!string.Equals(scenario, "playback", StringComparison.OrdinalIgnoreCase))
                return null;
            List<GHReportRun> runsA = UsedRunsOfSuites(suitesA);
            List<GHReportRun> runsB = UsedRunsOfSuites(suitesB);
            int measure = ContentTurnReached;
            if (AllHaveContent(runsA, ContentInputRecords) && AllHaveContent(runsB, ContentInputRecords))
                measure = ContentInputRecords;
            else if (AllHaveContent(runsA, ContentTurnsInWindow) && AllHaveContent(runsB, ContentTurnsInWindow))
                measure = ContentTurnsInWindow;
            List<double> a = ContentValues(runsA, measure);
            List<double> b = ContentValues(runsB, measure);
            if (a.Count == 0 || b.Count == 0)
                return null;
            a.Sort();
            b.Sort();
            double medianA = GHPerformanceStatistics.MedianNearestRank(a);
            double medianB = GHPerformanceStatistics.MedianNearestRank(b);
            double tolerance = Math.Max(1.0, 0.1 * Math.Max(Math.Abs(medianA), Math.Abs(medianB)));
            bool mediansDiffer = Math.Abs(medianA - medianB) > tolerance;
            bool disjoint = a[a.Count - 1] + 1 < b[0] || b[b.Count - 1] + 1 < a[0];
            if (!mediansDiffer && !disjoint)
                return null;
            string name = measure == ContentInputRecords ? "input records in the window"
                : measure == ContentTurnsInWindow ? "turns in the window" : "turn reached";
            return Truncate("Warning: the arms played different content (" + name + "):", MaxLineWidth) + "\n"
                + Truncate("  A median " + FmtCount(medianA) + " (" + FmtCount(a[0]) + "-" + FmtCount(a[a.Count - 1])
                    + "), B median " + FmtCount(medianB) + " (" + FmtCount(b[0]) + "-" + FmtCount(b[b.Count - 1]) + ")",
                    MaxLineWidth);
        }

        private static bool AllHaveContent(List<GHReportRun> runs, int measure)
        {
            for (int i = 0; i < runs.Count; i++)
            {
                if (double.IsNaN(ContentOf(runs[i], measure)))
                    return false;
            }
            return true;
        }

        /* The runs' content by measure, leaving out the runs without it */
        private static List<double> ContentValues(List<GHReportRun> runs, int measure)
        {
            List<double> values = new List<double>();
            for (int i = 0; i < runs.Count; i++)
            {
                double value = ContentOf(runs[i], measure);
                if (!double.IsNaN(value))
                    values.Add(value);
            }
            return values;
        }

        /* NaN when the run did not record it */
        private static double ContentOf(GHReportRun run, int measure)
        {
            if (measure == ContentInputRecords)
                return run.InputRecordsInWindow >= 0 ? run.InputRecordsInWindow : double.NaN;
            if (measure == ContentTurnsInWindow)
                return run.TurnAtWindowStart >= 0 && run.TurnReached >= run.TurnAtWindowStart
                    ? run.TurnReached - run.TurnAtWindowStart : double.NaN;
            return run.TurnReached >= 0 ? run.TurnReached : double.NaN;
        }

        /* The first scenario named by the suites of arm A, then of arm B */
        private static string ScenarioOf(IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB)
        {
            IList<GHReportSuite>[] arms = { suitesA, suitesB };
            for (int a = 0; a < arms.Length; a++)
            {
                if (arms[a] == null)
                    continue;
                for (int i = 0; i < arms[a].Count; i++)
                {
                    if (arms[a][i] != null && !string.IsNullOrEmpty(arms[a][i].Scenario))
                        return arms[a][i].Scenario;
                }
            }
            return null;
        }

        private static string FingerprintValue(Dictionary<string, string> fp, string key)
        {
            string value;
            return fp != null && fp.TryGetValue(key, out value) ? value : null;
        }

        private static void AppendLines(StringBuilder sb, List<string> lines)
        {
            for (int i = 0; i < lines.Count; i++)
                Line(sb, Truncate(lines[i], MaxLineWidth));
        }

        /* The categorized fingerprint diff from arm A's pooled fingerprint to arm B's (a
           key whose suites disagree within an arm reads "mixed"), its attribution label,
           and a warning when the label is confounded or more than one setting differs */
        private static void AppendEnvironmentDifferences(StringBuilder sb,
            IList<GHReportSuite> suitesA, IList<GHReportSuite> suitesB, GHReportComparisonContext context)
        {
            Dictionary<string, string> a = context != null && context.FingerprintA != null
                ? context.FingerprintA : PooledFingerprint(suitesA);
            Dictionary<string, string> b = context != null && context.FingerprintB != null
                ? context.FingerprintB : PooledFingerprint(suitesB);
            List<GHFingerprintChange> changes = GHEnvironmentFingerprint.Diff(a, b);

            Line(sb, "Environment differences (A -> B):");
            GHEnvironmentFingerprint.AppendReportLines(sb, changes, MaxLineWidth);
            string label = GHEnvironmentFingerprint.AttributionLabel(changes);
            Line(sb, Truncate("Attribution: " + label, MaxLineWidth));
            if (label.StartsWith(GHEnvironmentFingerprint.AttributionConfoundedPrefix, StringComparison.Ordinal))
                Line(sb, "Warning: code and environment both differ; the result is confounded.");
            if (GHEnvironmentFingerprint.SettingsViolation(changes))
                Line(sb, "Warning: " + GHEnvironmentFingerprint.SettingsViolationText
                    + "; compare one change at a time.");
        }

        /* Whether leaving out the elevated runs changes any metric's verdict: "decision
           unchanged" on the same line, else one indented line per changed metric;
           nothing when no used run was elevated */
        private static void AppendSensitivity(StringBuilder sb, GHComparisonResult result,
            GHReportComparisonContext context)
        {
            if (context == null || context.ElevatedRuns <= 0)
                return;
            string head = "Without the " + context.ElevatedRuns.ToString(CultureInfo.InvariantCulture)
                + (context.ElevatedRuns == 1 ? " elevated run:" : " elevated runs:");
            GHComparisonResult without = context.WithoutElevated;
            if (without == null)
            {
                Line(sb, Truncate(head + " an arm has no used runs left", MaxLineWidth));
                return;
            }
            List<string> changes = new List<string>();
            for (int i = 0; i < result.Decisions.Count; i++)
            {
                GHMetricDecision d = result.Decisions[i];
                string name = d.Metric != null ? d.Metric.Name : null;
                for (int j = 0; j < without.Decisions.Count; j++)
                {
                    GHMetricDecision w = without.Decisions[j];
                    string wName = w.Metric != null ? w.Metric.Name : null;
                    if (wName != name)
                        continue;
                    if (!string.Equals(w.Verdict, d.Verdict, StringComparison.Ordinal))
                        changes.Add("  " + OrNA(name) + " changes to " + OrNA(w.Verdict));
                    break;
                }
            }
            if (changes.Count == 0)
            {
                Line(sb, Truncate(head + " decision unchanged", MaxLineWidth));
                return;
            }
            Line(sb, Truncate(head, MaxLineWidth));
            for (int i = 0; i < changes.Count; i++)
                Line(sb, Truncate(changes[i], MaxLineWidth));
        }

        /* One '|'-joined combo per distinct (AppVersion, short commit, SkiaSharp,
           Framework, Runtime, assembly id) tuple found across a suite list, in
           first-seen order; the id is code.assemblyMvid of the suite's fingerprint. */
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
                    + OrNA(s.RuntimeVersion) + "|"
                    + ShortHash(FingerprintValue(s.Fingerprint, GHPerformanceSuiteLogic.CodeAssemblyMvidKey), 12);
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
                Line(sb, "No decision: fewer than "
                    + GHPerformanceComparison.MinRunsForVerdict.ToString(CultureInfo.InvariantCulture)
                    + " used runs per arm (a and b).");
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
                Line(sb, "Fewer than "
                    + GHPerformanceComparison.ProvisionalBelowRuns.ToString(CultureInfo.InvariantCulture)
                    + " runs per arm: provisional; read the MDE.");
        }

        /* ---- recent hitches ---- */

        private const int WorstHitchCount = 10;
        private const int MarkHitchLimit = 10;
        private const double MarkLookbackSeconds = 5.0;
        private const double MarkLookaheadSeconds = 0.5;
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
           hitches displayed from MarkLookbackSeconds before to MarkLookaheadSeconds after
           the marked tick's callback start; past MarkHitchLimit of them, the largest are
           listed in time order */
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
            long lookback = (long)(MarkLookbackSeconds * Stopwatch.Frequency);
            long lookahead = (long)(MarkLookaheadSeconds * Stopwatch.Frequency);
            string windowText = MarkLookbackSeconds.ToString(CultureInfo.InvariantCulture) + " s before to "
                + MarkLookaheadSeconds.ToString(CultureInfo.InvariantCulture) + " s after";
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
                    long fromMark = displayed[j].DisplayedAtTicks - markTicks;
                    if (IsReportableHitch(displayed, j, recordCount)
                        && fromMark >= -lookback && fromMark <= lookahead)
                        near.Add(j);
                }
                if (near.Count == 0)
                {
                    Line(sb, "    no hitch from " + windowText);
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
                        + " smaller hitch(es) from " + windowText);
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

        /* Draw, flush and in-callback swap time of the tick that ended a gap, the request work handled
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
            long swapTicks = GHSmoothnessMetrics.SwapWaitTicks(r);
            string swap = swapTicks > 0 ? Fmt(TicksToMs(swapTicks)) + " ms" : "n/a";
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
            return "draw " + draw + "  flush " + flush + "  swap " + swap + "  requests " + Fmt(TicksToMs(requestTicks)) + " ms"
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

        /* A count or a median of counts, with up to two decimals */
        private static string FmtCount(double v)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
                return "n/a";
            return v.ToString("0.##", CultureInfo.InvariantCulture);
        }

        private static string SignedFmt(double v)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
                return "n/a";
            string sign = v >= 0 ? "+" : "";
            return sign + v.ToString("0.00", CultureInfo.InvariantCulture);
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

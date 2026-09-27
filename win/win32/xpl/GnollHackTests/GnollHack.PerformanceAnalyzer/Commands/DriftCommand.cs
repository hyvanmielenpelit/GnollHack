using System.Globalization;
using System.Text;
using System.Text.Json;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Commands
{
    /* Shifts over time in DEVEL/performance/history.jsonl, and what changed with them:

         drift --file <history.jsonl> [--scenario S] [--platform P] [--device D]
               [--series smoothness|external] [--resamples 2000] [--seed 1] [--out report.md]

       Runs are grouped into lines of like-for-like measurements: platform, device
       model, scenario, scenario kind, series kind, refresh and target periods (rounded
       to 0.1 ms) and the short hash of the settings fingerprint, since settings are
       deliberate variables. Without --series every run joins one line per series
       (external, smoothness) that carries all of that series' decision metrics.

       Within a line runs are grouped into batches: the record's batch (the suite
       invocation), or for older records the date, arm and commit. A batch's used runs
       are the ones not excluded. Each batch with at least MinRunsForVerdict used runs
       is compared with the previous batch, and, when that shows no shift, with the
       pooled used runs of the previous up to PooledBatches batches, which catches slow
       drift. The comparison is GHPerformanceComparison.Decide, the one compare uses: a
       shift is a metric whose bootstrap CI of the difference of medians excludes zero
       and whose difference exceeds the same pre-registered threshold.

       Each shift is attributed from the diff of the two sides' common fingerprints
       (GHEnvironmentFingerprint.AttributionLabel). A "none" label becomes "background"
       when at least half the later batch's runs were measured under elevated or busy
       background load, and "unexplained -- rerun under quiet conditions" otherwise.
       Differences in throttling and power state are appended as context.

       The report is Markdown, one table per line, then the list of shifts with the
       changed keys; written with CRLF line endings, UTF-8 without a BOM. */
    public static class DriftCommand
    {
        public const int PooledBatches = 3;
        public const string LabelBackground = "background";
        public const string LabelUnexplained = "unexplained -- rerun under quiet conditions";

        private static readonly string[] DefaultSeries = { "external", MetricNames.SmoothnessSeries };

        public sealed class Batch
        {
            public string Key;
            public DateTime FirstUtc = DateTime.MaxValue;
            public List<RunRecord> Runs = new List<RunRecord>();
            public List<RunRecord> Used = new List<RunRecord>();
            public Dictionary<string, float[]> Values = new Dictionary<string, float[]>();
            public Dictionary<string, double> Medians = new Dictionary<string, double>();
            public Dictionary<string, string> Common = new Dictionary<string, string>();
            public int Quiet, Elevated, Busy, Unknown;
            public double MeanOtherCpuP90 = double.NaN;
            public int Throttled;
            public List<string> PowerStates = new List<string>();
            public string ShiftText;
            public string Attribution;

            public int ElevatedOrBusy { get { return Elevated + Busy; } }
        }

        public sealed class Line
        {
            public string Key;
            public string Title;
            public string SeriesKind;
            public double TargetPeriodMs;
            public List<Batch> Batches = new List<Batch>();
        }

        public sealed class Shift
        {
            public Line Line;
            public Batch After;
            public List<Batch> Before = new List<Batch>();
            public string Against;
            public List<GHMetricDecision> Decisions = new List<GHMetricDecision>();
            public List<GHFingerprintChange> Changes = new List<GHFingerprintChange>();
            public string Label;
            public string Context;
        }

        public sealed class Result
        {
            public int RunCount;
            public List<Line> Lines = new List<Line>();
            public List<Shift> Shifts = new List<Shift>();
        }

        public static int Run(Args a)
        {
            string file = a.Require("file");
            if (!File.Exists(file))
                throw new FileNotFoundException("history file not found: " + file);
            List<RunRecord> runs = ReadHistory(file);
            runs = Filter(runs, a.Get("scenario"), a.Get("platform"), a.Get("device"));
            int resamples = a.GetInt("resamples", GHPerformanceComparison.DefaultResamples);
            ulong seed = (ulong)a.GetInt("seed", (int)GHPerformanceComparison.DefaultSeed);
            Result res = Analyze(runs, a.Get("series"), resamples, seed);
            string text = BuildReport(res, Path.GetFileName(file), resamples, seed);
            string outPath = a.Get("out");
            if (outPath != null)
            {
                File.WriteAllText(outPath, Csv.ToCrlf(text), new UTF8Encoding(false));
                Console.WriteLine("drift: wrote " + outPath);
            }
            else
            {
                Console.Write(text);
            }
            return 0;
        }

        public static List<RunRecord> ReadHistory(string file)
        {
            List<RunRecord> runs = new List<RunRecord>();
            foreach (string line in File.ReadLines(file))
            {
                if (line.Trim().Length == 0)
                    continue;
                RunRecord r;
                try { r = JsonSerializer.Deserialize<RunRecord>(line, RunRecord.JsonOptions); }
                catch (JsonException) { continue; }
                if (r == null)
                    continue;
                r.NormalizeLegacy();
                runs.Add(r);
            }
            return runs;
        }

        public static List<RunRecord> Filter(List<RunRecord> runs, string scenario, string platform, string device)
        {
            return runs.Where(r =>
                (scenario == null || string.Equals(r.Scenario, scenario, StringComparison.OrdinalIgnoreCase))
                && (platform == null || string.Equals(r.Platform, platform, StringComparison.OrdinalIgnoreCase))
                && (device == null || string.Equals(r.Device?.Model, device, StringComparison.OrdinalIgnoreCase)
                    || string.Equals(r.Device?.Id, device, StringComparison.OrdinalIgnoreCase))).ToList();
        }

        public static Result Analyze(IEnumerable<RunRecord> runs, string seriesKind, int resamples, ulong seed)
        {
            Result res = new Result();
            string[] kinds = seriesKind != null ? new[] { seriesKind } : DefaultSeries;
            Dictionary<string, Line> lines = new Dictionary<string, Line>(StringComparer.Ordinal);
            Dictionary<Line, Dictionary<string, Batch>> batches = new Dictionary<Line, Dictionary<string, Batch>>();
            foreach (RunRecord r in runs)
            {
                res.RunCount++;
                r.NormalizeLegacy();
                foreach (string kind in kinds)
                {
                    Series s = r.FindSeries(kind);
                    if (s == null || MetricNames.DecisionFor(kind).Any(dm => !s.Metrics.ContainsKey(dm.Name)))
                        continue;
                    string key = LineKey(r, kind, out string title, out double targetMs);
                    if (!lines.TryGetValue(key, out Line line))
                    {
                        line = new Line { Key = key, Title = title, SeriesKind = kind, TargetPeriodMs = targetMs };
                        lines[key] = line;
                        batches[line] = new Dictionary<string, Batch>(StringComparer.Ordinal);
                    }
                    string bk = BatchKey(r);
                    if (!batches[line].TryGetValue(bk, out Batch b))
                    {
                        b = new Batch { Key = bk };
                        batches[line][bk] = b;
                        line.Batches.Add(b);
                    }
                    b.Runs.Add(r);
                    DateTime? t = ParseUtc(r.TimestampUtc);
                    if (t.HasValue && t.Value < b.FirstUtc)
                        b.FirstUtc = t.Value;
                }
            }

            foreach (Line line in lines.Values.OrderBy(l => l.Title, StringComparer.Ordinal))
            {
                line.Batches = line.Batches.OrderBy(b => b.FirstUtc).ThenBy(b => b.Key, StringComparer.Ordinal).ToList();
                foreach (Batch b in line.Batches)
                    Summarize(b, line.SeriesKind);
                res.Lines.Add(line);
                DetectShifts(line, res.Shifts, resamples, seed);
            }
            return res;
        }

        private static string LineKey(RunRecord r, string kind, out string title, out double targetMs)
        {
            double refreshMs = Math.Round(r.Display.VsyncMs, 1);
            targetMs = r.Display.TargetPeriodMs > 0 ? r.Display.TargetPeriodMs : r.Display.VsyncMs;
            double targetRounded = Math.Round(targetMs, 1);
            string settings = GHEnvironmentFingerprint.ShortHash(r.Fingerprint, GHEnvironmentFingerprint.CategorySettings);
            string device = r.Device?.Model ?? r.Device?.Id ?? "?";
            title = (r.Platform ?? "?") + ", " + device + ", " + (r.Scenario ?? "?") + " (" + (r.ScenarioKind ?? "?") + "), "
                + kind + ", refresh " + F(refreshMs, 1) + " ms, target " + F(targetRounded, 1) + " ms, settings " + settings;
            return string.Join("|", r.Platform ?? "", device, r.Scenario ?? "", r.ScenarioKind ?? "", kind,
                F(refreshMs, 1), F(targetRounded, 1), settings);
        }

        /* The record's batch, or date, arm and commit for records written before batches */
        public static string BatchKey(RunRecord r)
        {
            if (!string.IsNullOrEmpty(r.Batch))
                return r.Batch;
            DateTime? t = ParseUtc(r.TimestampUtc);
            string date = t.HasValue ? t.Value.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture)
                : (r.TimestampUtc != null && r.TimestampUtc.Length >= 10 ? r.TimestampUtc.Substring(0, 10) : "?");
            return date + "/" + (r.Arm ?? "?") + "/" + Short(r.Git?.Commit);
        }

        private static void Summarize(Batch b, string kind)
        {
            b.Used = b.Runs.Where(r => !r.Excluded).ToList();
            foreach (GHDecisionMetric dm in MetricNames.DecisionFor(kind))
            {
                float[] v = b.Used.Select(r => (float)r.FindSeries(kind).Metrics[dm.Name]).ToArray();
                b.Values[dm.Name] = v;
                b.Medians[dm.Name] = v.Length > 0 ? GHPerformanceStatistics.Median(v) : double.NaN;
            }
            b.Common = CompareCommand.CommonFingerprint(b.Used.Count > 0 ? b.Used : b.Runs);
            List<double> p90 = new List<double>();
            foreach (RunRecord r in b.Runs)
            {
                switch (r.BackgroundVerdict)
                {
                case GHBackgroundVerdict.Quiet:
                    b.Quiet++;
                    break;
                case GHBackgroundVerdict.Elevated:
                    b.Elevated++;
                    break;
                case GHBackgroundVerdict.Busy:
                    b.Busy++;
                    break;
                default:
                    b.Unknown++;
                    break;
                }
                double? x = r.DecisiveBackground?.Window?.OtherCpuP90Pct;
                if (x.HasValue)
                    p90.Add(x.Value);
                if (r.Thermal.Throttled)
                    b.Throttled++;
                string power = r.Thermal.PowerState ?? ThermalGate.PowerState(r.Thermal);
                if (!b.PowerStates.Contains(power))
                    b.PowerStates.Add(power);
            }
            if (p90.Count > 0)
                b.MeanOtherCpuP90 = p90.Average();
        }

        private static void DetectShifts(Line line, List<Shift> shifts, int resamples, ulong seed)
        {
            GHDecisionMetric[] metrics = MetricNames.DecisionFor(line.SeriesKind);
            int minRuns = GHPerformanceComparison.MinRunsForVerdict;
            /* The pool never reaches back across a detected shift, so that the level
               before a step is not compared again with the batches after it */
            int poolFloor = 0;
            for (int i = 1; i < line.Batches.Count; i++)
            {
                Batch after = line.Batches[i];
                if (after.Used.Count < minRuns)
                    continue;
                Batch previous = line.Batches[i - 1];
                Shift shift = null;
                if (previous.Used.Count >= minRuns)
                    shift = Compare(line, metrics, new List<Batch> { previous }, after, "the previous batch", resamples, seed);
                if (shift == null)
                {
                    int from = Math.Max(poolFloor, i - PooledBatches);
                    List<Batch> pool = line.Batches.Skip(from).Take(i - from).ToList();
                    if (pool.Count >= 2 && pool.Sum(p => p.Used.Count) >= minRuns)
                        shift = Compare(line, metrics, pool, after, "the pooled previous " + pool.Count + " batches", resamples, seed);
                }
                if (shift == null)
                    continue;
                poolFloor = i;
                after.ShiftText = string.Join(", ", shift.Decisions.Select(d => d.Metric.Name + " " + Signed(d.Diff) + " " + d.Verdict));
                after.Attribution = shift.Label;
                shifts.Add(shift);
            }
        }

        private static Shift Compare(Line line, GHDecisionMetric[] metrics, List<Batch> before, Batch after, string against, int resamples, ulong seed)
        {
            List<GHMetricDecision> flagged = new List<GHMetricDecision>();
            foreach (GHDecisionMetric dm in metrics)
            {
                float[] va = before.SelectMany(b => b.Values[dm.Name]).ToArray();
                float[] vb = after.Values[dm.Name];
                GHMetricDecision d = GHPerformanceComparison.Decide(dm, va, vb, line.TargetPeriodMs, false, resamples, seed);
                if (d.IsRegression || d.IsImprovement)
                    flagged.Add(d);
            }
            if (flagged.Count == 0)
                return null;

            Shift s = new Shift { Line = line, After = after, Against = against, Decisions = flagged };
            s.Before.AddRange(before);
            Dictionary<string, string> common = before.Count == 1 ? before[0].Common
                : CompareCommand.CommonFingerprint(before.SelectMany(b => b.Used.Count > 0 ? b.Used : b.Runs));
            bool sourcesDiffer;
            s.Changes = RunRecord.DiffFingerprints(common, after.Common, out sourcesDiffer);
            s.Label = GHEnvironmentFingerprint.AttributionLabel(s.Changes);
            if (s.Label == GHEnvironmentFingerprint.AttributionNone)
                s.Label = after.Runs.Count > 0 && 2 * after.ElevatedOrBusy >= after.Runs.Count ? LabelBackground : LabelUnexplained;

            List<string> context = new List<string>();
            if (sourcesDiffer)
                context.Add("fingerprints from different sources, shared keys compared");
            int throttledBefore = before.Sum(b => b.Throttled);
            if (throttledBefore != after.Throttled)
                context.Add("throttled runs " + throttledBefore + " -> " + after.Throttled);
            string powerBefore = string.Join("/", before.SelectMany(b => b.PowerStates).Distinct().OrderBy(p => p, StringComparer.Ordinal));
            string powerAfter = string.Join("/", after.PowerStates.OrderBy(p => p, StringComparer.Ordinal));
            if (powerBefore != powerAfter)
                context.Add("power " + powerBefore + " -> " + powerAfter);
            s.Context = context.Count > 0 ? string.Join(", ", context) : null;
            return s;
        }

        public static string BuildReport(Result res, string fileName, int resamples, ulong seed)
        {
            StringBuilder md = new StringBuilder();
            md.AppendLine("# Performance drift: " + fileName);
            md.AppendLine();
            md.AppendLine("Generated " + DateTime.UtcNow.ToString("u", CultureInfo.InvariantCulture) + " by GnollHack.PerformanceAnalyzer from "
                + res.RunCount + " runs on " + res.Lines.Count + " line(s). Bootstrap: " + resamples + " resamples, seed " + seed
                + ", 95 percent percentile intervals. A batch with at least " + GHPerformanceComparison.MinRunsForVerdict
                + " used runs is compared with the previous batch and, when that shows no shift, with the pooled previous up to "
                + PooledBatches + " batches; a shift is a metric whose CI excludes zero and whose difference exceeds compare's pre-registered threshold.");
            md.AppendLine();

            int n = 0;
            foreach (Line line in res.Lines)
            {
                n++;
                GHDecisionMetric[] metrics = MetricNames.DecisionFor(line.SeriesKind);
                md.AppendLine("## Line " + n + ": " + line.Title);
                md.AppendLine();
                md.AppendLine("| Batch | Date | Commit | n | " + string.Join(" | ", metrics.Select(m => m.Name + " (" + m.Unit + ")")) + " | Delta | bg | Attribution |");
                md.AppendLine("|---|---|---|---|" + string.Concat(metrics.Select(m => "---|")) + "---|---|---|");
                foreach (Batch b in line.Batches)
                {
                    string date = b.FirstUtc != DateTime.MaxValue ? b.FirstUtc.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture) : "?";
                    string commits = string.Join(", ", b.Runs.Select(r => Short(r.Git?.Commit)).Distinct());
                    md.AppendLine("| " + b.Key + " | " + date + " | " + commits + " | " + b.Used.Count + "/" + b.Runs.Count
                        + " | " + string.Join(" | ", metrics.Select(m => F(b.Medians[m.Name], 2)))
                        + " | " + (b.ShiftText ?? "") + " | " + BackgroundText(b) + " | " + (b.Attribution ?? "") + " |");
                }
                md.AppendLine();
            }
            if (res.Lines.Count == 0)
            {
                md.AppendLine("No run carries a series with every decision metric.");
                md.AppendLine();
            }

            md.AppendLine("## Shifts");
            md.AppendLine();
            if (res.Shifts.Count == 0)
            {
                md.AppendLine("No shift detected.");
                md.AppendLine();
                return md.ToString();
            }
            foreach (Shift s in res.Shifts)
            {
                md.AppendLine("- **" + s.Line.Title + "**: batch `" + s.After.Key + "` against " + s.Against
                    + " (" + string.Join(", ", s.Before.Select(b => "`" + b.Key + "`")) + "): "
                    + string.Join("; ", s.Decisions.Select(d => d.Metric.Name + " " + F(d.MedianA, 2) + " -> " + F(d.MedianB, 2)
                        + " (" + Signed(d.Diff) + " [" + Signed(d.Ci.Low) + ", " + Signed(d.Ci.High) + "]) " + d.Verdict))
                    + ". Attribution: " + s.Label + (s.Context != null ? "; context: " + s.Context : "") + ".");
                StringBuilder lines = new StringBuilder();
                GHEnvironmentFingerprint.AppendReportLines(lines, s.Changes, 0);
                foreach (string l in lines.ToString().Split('\n'))
                {
                    if (l.Trim().Length > 0)
                        md.AppendLine("  - " + l.Trim());
                }
            }
            md.AppendLine();
            return md.ToString();
        }

        private static string BackgroundText(Batch b)
        {
            return b.Quiet + "q " + b.Elevated + "e " + b.Busy + "B " + b.Unknown + "?"
                + (double.IsNaN(b.MeanOtherCpuP90) ? "" : ", P90 " + F(b.MeanOtherCpuP90, 1));
        }

        private static DateTime? ParseUtc(string text)
        {
            if (string.IsNullOrEmpty(text))
                return null;
            return DateTime.TryParse(text, CultureInfo.InvariantCulture, DateTimeStyles.AdjustToUniversal | DateTimeStyles.AssumeUniversal, out DateTime t)
                ? t : null;
        }

        private static string F(double v, int decimals) { return double.IsNaN(v) ? "n/a" : v.ToString("F" + decimals, CultureInfo.InvariantCulture); }
        private static string Signed(double v) { return (v > 0 ? "+" : "") + F(v, 2); }
        private static string Short(string commit) { return string.IsNullOrEmpty(commit) ? "?" : (commit.Length > 9 ? commit.Substring(0, 9) : commit); }
    }
}

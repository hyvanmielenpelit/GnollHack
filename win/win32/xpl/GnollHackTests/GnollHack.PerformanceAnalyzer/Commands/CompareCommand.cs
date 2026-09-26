using System.Globalization;
using System.Text;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Commands
{
    /* Two-arm comparison following DEVEL/performance/README.md's Statistics section:

         compare --a <dir|run.json ...> --b <dir|run.json ...> [--label-a X --label-b Y]
                 [--series external|internal|smoothness] [--resamples 2000] [--seed 1]
                 [--include-throttled] [--include-excluded] [--allow-mixed-cadence]
                 [--out report.md]

       Two levels are reported and kept apart:

       Run level (the decision level). Each run contributes one value per metric; the
       arms are compared on those values. With four or five runs per arm the
       Mann-Whitney test has little power (its smallest attainable two-sided p at 5 v 5
       is 0.0079), so the decision rests on the bootstrap confidence interval of the
       difference of arm medians, resampling runs, and on the pre-registered
       thresholds. The minimum detectable effect at the observed spread is printed so
       that "no difference" reads as "no difference larger than X".

       Interval level (the shape level). All intervals of an arm pooled. Mann-Whitney
       and Cliff's delta describe whether one distribution tends to sit above the other,
       the Hodges-Lehmann shift says by how many milliseconds, and a bootstrap CI on the
       P99 difference locates the tail. Pooling treats intervals from different runs as
       exchangeable, which they are not quite, so this level informs and never decides.

       A comparison is only meaningful between runs paced against the same target and
       refresh periods: --allow-mixed-cadence permits comparing runs whose cadence
       differs by more than the 1 percent tolerance, at the cost of a warning, since the
       thresholds below are themselves expressed in target-period units.

       --series smoothness compares in-app (schema v2) run records, or ingested records
       that carry their smoothness series, on the metrics the app reported: hitch time
       ratio (worse above an absolute 2 ms/s), pacing error RMS (absolute 1 ms), and
       displayed FPS (relative 5 percent). The interval level then pools the on-screen
       intervals. */
    public static class CompareCommand
    {
        private const double CadenceToleranceFraction = 0.01;

        /* Below this many used runs in either arm, a decision would rest on too little
           data to trust the bootstrap CI, so every decision row is refused instead */
        private const int MinRunsForVerdict = 3;

        private sealed class Arm
        {
            public string Label;
            public List<RunRecord> Runs = new List<RunRecord>();
            public List<RunRecord> Used = new List<RunRecord>();
            public Dictionary<string, float[]> PerRun = new Dictionary<string, float[]>();
            public float[] Pooled;
        }

        public static int Run(Args a)
        {
            string seriesKind = a.Get("series") ?? "external";
            int resamples = a.GetInt("resamples", 2000);
            ulong seed = (ulong)a.GetInt("seed", 1);
            bool includeThrottled = a.Has("include-throttled");
            bool includeExcluded = a.Has("include-excluded");
            bool allowMixedCadence = a.Has("allow-mixed-cadence");

            Arm armA = LoadArm(a.GetAll("a"), a.Get("label-a") ?? "A", seriesKind, includeThrottled, includeExcluded);
            Arm armB = LoadArm(a.GetAll("b"), a.Get("label-b") ?? "B", seriesKind, includeThrottled, includeExcluded);
            if (armA.Used.Count == 0 || armB.Used.Count == 0)
            {
                Console.Error.WriteLine("compare: an arm has no usable runs (A=" + armA.Used.Count + ", B=" + armB.Used.Count + ")");
                return 1;
            }

            List<RunRecord> allUsed = armA.Used.Concat(armB.Used).ToList();
            double refreshMs = allUsed.Select(r => r.Display.VsyncMs).Where(v => v > 0).DefaultIfEmpty(16.667).First();
            double targetMs = allUsed.Select(TargetPeriodOf).Where(v => v > 0).DefaultIfEmpty(refreshMs).First();

            if (CadenceMismatch(allUsed, CadenceToleranceFraction, out string cadenceDetail))
            {
                if (!allowMixedCadence)
                {
                    Console.Error.WriteLine("compare: cadence differs between runs by more than "
                        + (CadenceToleranceFraction * 100).ToString("0.#", CultureInfo.InvariantCulture)
                        + " percent (" + cadenceDetail + "); pass --allow-mixed-cadence to compare anyway");
                    return 1;
                }
                Console.Error.WriteLine("compare: warning: cadence differs between runs (" + cadenceDetail
                    + "); continuing because --allow-mixed-cadence was given");
            }

            StringBuilder md = new StringBuilder();
            WriteHeader(md, armA, armB, seriesKind, targetMs, refreshMs, resamples, seed);
            WriteRunTable(md, armA, seriesKind);
            WriteRunTable(md, armB, seriesKind);
            WriteRunLevel(md, armA, armB, seriesKind, targetMs, resamples, seed);
            WriteIntervalLevel(md, armA, armB, resamples, seed);
            WriteThermal(md, armA);
            WriteThermal(md, armB);

            string text = md.ToString();
            string outPath = a.Get("out");
            if (outPath != null)
            {
                File.WriteAllText(outPath, text.Replace("\r\n", "\n").Replace("\n", "\r\n"), new UTF8Encoding(false));
                Console.WriteLine("compare: wrote " + outPath);
            }
            else
                Console.Write(text);
            return 0;
        }

        private static double TargetPeriodOf(RunRecord r)
        {
            return r.Display.TargetPeriodMs > 0 ? r.Display.TargetPeriodMs : r.Display.VsyncMs;
        }

        /* True when the refresh period or the target period varies across the given runs
           by more than the tolerance fraction of the smallest value. */
        private static bool CadenceMismatch(List<RunRecord> runs, double tolerance, out string detail)
        {
            List<double> refresh = runs.Select(r => r.Display.VsyncMs).Where(v => v > 0).ToList();
            List<double> target = runs.Select(TargetPeriodOf).Where(v => v > 0).ToList();
            if (!WithinTolerance(refresh, tolerance, out double refMin, out double refMax))
            {
                detail = "refresh period from " + F(refMin) + " to " + F(refMax) + " ms";
                return true;
            }
            if (!WithinTolerance(target, tolerance, out double tMin, out double tMax))
            {
                detail = "target period from " + F(tMin) + " to " + F(tMax) + " ms";
                return true;
            }
            detail = null;
            return false;
        }

        private static bool WithinTolerance(List<double> values, double tolerance, out double min, out double max)
        {
            min = values.Count > 0 ? values.Min() : 0;
            max = values.Count > 0 ? values.Max() : 0;
            if (min <= 0)
                return true;
            return (max - min) / min <= tolerance;
        }

        private static Arm LoadArm(List<string> inputs, string label, string seriesKind, bool includeThrottled, bool includeExcluded)
        {
            Arm arm = new Arm { Label = label };
            foreach (string input in inputs)
            {
                IEnumerable<string> files = Directory.Exists(input)
                    ? Directory.GetFiles(input, "*.json").OrderBy(f => f, StringComparer.Ordinal)
                    : new[] { input };
                foreach (string f in files)
                {
                    RunRecord r;
                    try { r = RunRecord.Load(f); }
                    catch (Exception ex) { Console.Error.WriteLine("skip " + f + ": " + ex.Message); continue; }
                    if (r == null || r.SchemaVersion < 1)
                        continue;
                    r.Notes = r.Notes ?? "";
                    r.Configuration["_file"] = System.Text.Json.JsonSerializer.SerializeToElement(Path.GetFileName(f));
                    arm.Runs.Add(r);
                }
            }
            foreach (RunRecord r in arm.Runs)
            {
                Series s = r.FindSeries(seriesKind);
                if (s == null)
                    continue;
                /* The smoothness decision metrics come from the record itself, not from
                   the intervals, so such a run needs those metrics rather than intervals */
                if (MetricNames.IsSmoothness(seriesKind))
                {
                    if (MetricNames.SmoothnessDecision.Any(dm => !s.Metrics.ContainsKey(dm.Name)))
                        continue;
                    s.IntervalsMs = s.IntervalsMs ?? new float[0];
                }
                else if (s.IntervalsMs == null || s.IntervalsMs.Length == 0)
                    continue;
                if (r.Excluded && !includeExcluded && !(includeThrottled && r.Thermal.Throttled && r.ExclusionReason != null && r.ExclusionReason.StartsWith("throttled")))
                    continue;
                if (r.Thermal.Throttled && !includeThrottled)
                    continue;
                arm.Used.Add(r);
            }
            foreach (DecisionMetric dm in MetricNames.DecisionFor(seriesKind))
                arm.PerRun[dm.Name] = arm.Used.Select(r => (float)r.FindSeries(seriesKind).Metrics[dm.Name]).ToArray();
            /* A run missing a reported (non-decision) metric is left out of that metric's
               array rather than counted as zero, which would read as a false regression or
               improvement */
            foreach (string name in MetricNames.ReportedFor(seriesKind))
                arm.PerRun[name] = arm.Used.Select(r => r.FindSeries(seriesKind).Metrics)
                    .Where(m => m.ContainsKey(name))
                    .Select(m => (float)m[name])
                    .ToArray();
            arm.Pooled = arm.Used.SelectMany(r => r.FindSeries(seriesKind).IntervalsMs).ToArray();
            return arm;
        }

        private static void WriteHeader(StringBuilder md, Arm a, Arm b, string kind, double targetMs, double refreshMs, int resamples, ulong seed)
        {
            md.AppendLine("# Performance comparison: " + a.Label + " versus " + b.Label);
            md.AppendLine();
            md.AppendLine("Generated " + DateTime.UtcNow.ToString("u", CultureInfo.InvariantCulture)
                + " by GnollHack.PerformanceAnalyzer. Series: `" + kind + "`. Target period "
                + F(targetMs) + " ms, refresh period " + F(refreshMs) + " ms. Bootstrap: " + resamples + " resamples, seed " + seed + ", 95 percent percentile intervals.");
            md.AppendLine();
            RunRecord any = a.Used[0];
            md.AppendLine("| | " + a.Label + " | " + b.Label + " |");
            md.AppendLine("|---|---|---|");
            md.AppendLine("| Runs loaded / used | " + a.Runs.Count + " / " + a.Used.Count + " | " + b.Runs.Count + " / " + b.Used.Count + " |");
            md.AppendLine("| Scenario | " + Join(a.Used.Select(r => r.Scenario)) + " | " + Join(b.Used.Select(r => r.Scenario)) + " |");
            md.AppendLine("| Build | " + Join(a.Used.Select(r => (r.Git.Tag ?? Short(r.Git.Commit)) + " " + r.BuildConfiguration)) + " | " + Join(b.Used.Select(r => (r.Git.Tag ?? Short(r.Git.Commit)) + " " + r.BuildConfiguration)) + " |");
            md.AppendLine("| Device | " + Join(a.Used.Select(r => r.Device.Model ?? r.Device.Id)) + " | " + Join(b.Used.Select(r => r.Device.Model ?? r.Device.Id)) + " |");
            md.AppendLine("| Platform | " + Join(a.Used.Select(r => r.Platform)) + " | " + Join(b.Used.Select(r => r.Platform)) + " |");
            md.AppendLine("| Power | " + Join(a.Used.Select(PowerOf)) + " | " + Join(b.Used.Select(PowerOf)) + " |");
            md.AppendLine();
            HashSet<string> powers = new HashSet<string>(a.Used.Concat(b.Used).Select(PowerOf).Where(p => p != "unknown"));
            if (powers.Count > 1)
                md.AppendLine("> **Warning:** power state differs between runs (" + string.Join(", ", powers) + "). A charging device and one on battery are different experiments; hold the state constant and rerun.");
            if (a.Used.Concat(b.Used).Any(r => PowerOf(r) == "unknown"))
                md.AppendLine("> **Note:** at least one run has no power-state reading.");
            if (a.Used.Any(r => r.BuildConfiguration != "Release") || b.Used.Any(r => r.BuildConfiguration != "Release"))
                md.AppendLine("> **Warning:** at least one run is not a Release build. See DEVEL/performance/README.md's Protocol section: such runs are not comparable to shipped builds.");
            if (a.Used.Select(r => r.Scenario).Distinct().Count() > 1 || b.Used.Select(r => r.Scenario).Distinct().Count() > 1
                || a.Used[0].Scenario != b.Used[0].Scenario)
                md.AppendLine("> **Warning:** scenarios differ between or within arms; the comparison is not like for like.");
            if (a.Used.Any(r => r.ScenarioKind != any.ScenarioKind) || b.Used.Any(r => r.ScenarioKind != any.ScenarioKind))
                md.AppendLine("> **Warning:** replay and gameplay runs are mixed; DEVEL/performance/README.md's Protocol section forbids comparing them directly.");
            md.AppendLine();
        }

        private static void WriteSmoothnessRunTable(StringBuilder md, Arm arm, string kind)
        {
            md.AppendLine("## Runs: " + arm.Label);
            md.AppendLine();
            md.AppendLine("| Run | Displayed | Window s | Displayed FPS | Hitch ms/s | Pacing RMS | Pacing P99 | Judder % | Dropped | Present source | Thermal | Used |");
            md.AppendLine("|---|---|---|---|---|---|---|---|---|---|---|---|");
            foreach (RunRecord r in arm.Runs)
            {
                Series s = r.FindSeries(kind);
                bool used = arm.Used.Contains(r);
                string thermal = (r.Thermal.Before?.Status ?? "?") + " -> " + (r.Thermal.After?.Status ?? "?")
                    + (r.Thermal.Throttled ? " THROTTLED" : "");
                string name = Path.GetFileNameWithoutExtension(r.Configuration["_file"].GetString());
                if (s == null || s.Metrics.Count == 0)
                {
                    md.AppendLine("| " + name + " | no `" + kind + "` series | | | | | | | | | " + thermal + " | no |");
                    continue;
                }
                Dictionary<string, double> m = s.Metrics;
                md.AppendLine("| " + name
                    + " | " + Iv(m, MetricNames.DisplayedCount)
                    + " | " + F(r.WindowSeconds > 0 ? r.WindowSeconds : Mv(m, "windowMs") / 1000.0, 0)
                    + " | " + F(Mv(m, MetricNames.DisplayedFps), 1)
                    + " | " + F(Mv(m, MetricNames.HitchRatioMsPerSec))
                    + " | " + F(Mv(m, MetricNames.PacingErrorRmsMs))
                    + " | " + F(Mv(m, MetricNames.PacingErrorP99Ms))
                    + " | " + F(Mv(m, MetricNames.JudderPct), 1)
                    + " | " + Iv(m, MetricNames.DroppedCount)
                    + " | " + (s.Info.GetValueOrDefault("presentSource") ?? "?")
                    + " | " + thermal + " | " + (used ? "yes" : "no" + (r.Excluded ? " (" + r.ExclusionReason + ")" : "")) + " |");
            }
            md.AppendLine();
        }

        private static void WriteRunTable(StringBuilder md, Arm arm, string kind)
        {
            if (MetricNames.IsSmoothness(kind))
            {
                WriteSmoothnessRunTable(md, arm, kind);
                return;
            }
            md.AppendLine("## Runs: " + arm.Label);
            md.AppendLine();
            md.AppendLine("| Run | Frames | Window s | P50 | P95 | P99 | Max | Mean FPS | 1% low | Hitch ms/s | Jank % | Thermal | Used |");
            md.AppendLine("|---|---|---|---|---|---|---|---|---|---|---|---|---|");
            foreach (RunRecord r in arm.Runs)
            {
                Series s = r.FindSeries(kind);
                bool used = arm.Used.Contains(r);
                string thermal = (r.Thermal.Before?.Status ?? "?") + " -> " + (r.Thermal.After?.Status ?? "?")
                    + (r.Thermal.Throttled ? " THROTTLED" : "");
                if (s == null || s.Metrics.Count == 0)
                {
                    md.AppendLine("| " + Path.GetFileNameWithoutExtension(r.Configuration["_file"].GetString()) + " | no `" + kind + "` series | | | | | | | | | | " + thermal + " | no |");
                    continue;
                }
                Dictionary<string, double> m = s.Metrics;
                md.AppendLine("| " + Path.GetFileNameWithoutExtension(r.Configuration["_file"].GetString())
                    + " | " + Iv(m, MetricNames.FrameCount)
                    + " | " + F(Mv(m, MetricNames.WindowSeconds), 0)
                    + " | " + F(Mv(m, MetricNames.FrameDurationP50)) + " | " + F(Mv(m, MetricNames.FrameDurationP95)) + " | " + F(Mv(m, MetricNames.FrameDurationP99))
                    + " | " + F(Mv(m, MetricNames.FrameDurationMax), 1) + " | " + F(Mv(m, MetricNames.FpsMean), 1) + " | " + F(Mv(m, MetricNames.Fps1PctLow), 1)
                    + " | " + F(Mv(m, MetricNames.HitchRatio)) + " | " + F(Mv(m, MetricNames.JankPct), 1)
                    + " | " + thermal + " | " + (used ? "yes" : "no" + (r.Excluded ? " (" + r.ExclusionReason + ")" : "")) + " |");
            }
            md.AppendLine();
        }

        private static void WriteRunLevel(StringBuilder md, Arm a, Arm b, string kind, double targetMs, int resamples, ulong seed)
        {
            md.AppendLine("## Run level (decision)");
            md.AppendLine();
            md.AppendLine("Each run contributes one value. Difference is " + b.Label + " minus " + a.Label
                + "; the interval is a bootstrap CI of the difference of arm medians, resampling runs.");
            md.AppendLine();
            md.AppendLine("| Metric | " + a.Label + " median | " + b.Label + " median | Diff | 95% CI | Rel. | MDE | MWU p | Verdict |");
            md.AppendLine("|---|---|---|---|---|---|---|---|---|");
            bool tooFewRuns = Math.Min(a.Used.Count, b.Used.Count) < MinRunsForVerdict;
            List<string> verdicts = new List<string>();
            foreach (DecisionMetric dm in MetricNames.DecisionFor(kind))
                verdicts.Add(WriteDecisionRow(md, dm, a, b, targetMs, resamples, seed, tooFewRuns));
            foreach (string name in MetricNames.ReportedFor(kind))
            {
                if (name == MetricNames.FrameCount || name == MetricNames.WindowSeconds)
                    continue;
                WriteInfoRow(md, name, a, b, resamples, seed);
            }
            md.AppendLine();
            if (tooFewRuns)
            {
                md.AppendLine("**Overall:** no decision was made: fewer than " + MinRunsForVerdict
                    + " used runs per arm (" + a.Used.Count + " and " + b.Used.Count + ").");
            }
            else
            {
                int regressions = verdicts.Count(v => v == "REGRESSION");
                int improvements = verdicts.Count(v => v == "improvement");
                md.AppendLine("**Overall:** " + (regressions > 0 ? regressions + " decision metric(s) regressed. " : "no decision metric regressed. ")
                    + (improvements > 0 ? improvements + " improved. " : "")
                    + "Runs per arm: " + a.Used.Count + " and " + b.Used.Count + "."
                    + (Math.Min(a.Used.Count, b.Used.Count) < 5 ? " Fewer than 5 runs per arm: treat the verdict as provisional and read the MDE column." : ""));
            }
            md.AppendLine();
        }

        private static string WriteDecisionRow(StringBuilder md, DecisionMetric dm, Arm a, Arm b, double targetMs, int resamples, ulong seed, bool tooFewRuns)
        {
            float[] va = a.PerRun[dm.Name], vb = b.PerRun[dm.Name];
            double medA = GHPerformanceStatistics.Median(va), medB = GHPerformanceStatistics.Median(vb);
            GHPerformanceStatistics.Interval ci = GHPerformanceStatistics.BootstrapDifferenceCi(va, vb, GHPerformanceStatistics.StatMedian, resamples, 0.95, seed);
            double diff = medB - medA;
            double rel = medA != 0 ? diff / Math.Abs(medA) : 0;
            double sdPooled = PooledSd(va, vb);
            double mde = GHPerformanceStatistics.MinimumDetectableEffect(sdPooled, Math.Min(va.Length, vb.Length), 0.05);
            GHPerformanceStatistics.MannWhitneyResult mw = GHPerformanceStatistics.MannWhitneyU(va, vb);

            string verdict;
            if (tooFewRuns)
            {
                /* The CI, MDE and p-value are still informative, but too few runs were used
                   to trust a verdict built on them */
                verdict = "too few runs (need " + MinRunsForVerdict + ")";
            }
            else
            {
                bool ciExcludesZero = ci.Low > 0 || ci.High < 0;
                double absThreshold = dm.AbsoluteThreshold > 0 ? dm.AbsoluteThreshold : dm.AbsoluteThresholdTargetPeriod * targetMs;
                bool beyondThreshold = (dm.RelativeThreshold > 0 && Math.Abs(rel) > dm.RelativeThreshold)
                                    || (absThreshold > 0 && Math.Abs(diff) > absThreshold);
                bool worse = dm.HigherIsWorse ? diff > 0 : diff < 0;
                if (ciExcludesZero && beyondThreshold)
                    verdict = worse ? "REGRESSION" : "improvement";
                else if (ciExcludesZero)
                    verdict = worse ? "worse, under threshold" : "better, under threshold";
                else
                    verdict = "no evidence";
            }
            md.AppendLine("| **" + dm.Name + "** (" + dm.Unit + ") | " + F(medA) + " | " + F(medB) + " | " + Signed(diff)
                + " | [" + Signed(ci.Low) + ", " + Signed(ci.High) + "] | " + Rel(medA, rel) + " | +/-" + F(mde)
                + " | " + P(mw) + " | " + verdict + " |");
            return verdict;
        }

        private static void WriteInfoRow(StringBuilder md, string name, Arm a, Arm b, int resamples, ulong seed)
        {
            float[] va = a.PerRun[name], vb = b.PerRun[name];
            if (va.Length == 0 || vb.Length == 0)
                return;
            double medA = GHPerformanceStatistics.Median(va), medB = GHPerformanceStatistics.Median(vb);
            GHPerformanceStatistics.Interval ci = GHPerformanceStatistics.BootstrapDifferenceCi(va, vb, GHPerformanceStatistics.StatMedian, resamples, 0.95, seed);
            double diff = medB - medA;
            double rel = medA != 0 ? diff / Math.Abs(medA) : 0;
            double mde = GHPerformanceStatistics.MinimumDetectableEffect(PooledSd(va, vb), Math.Min(va.Length, vb.Length), 0.05);
            GHPerformanceStatistics.MannWhitneyResult mw = GHPerformanceStatistics.MannWhitneyU(va, vb);
            md.AppendLine("| " + name + " | " + F(medA) + " | " + F(medB) + " | " + Signed(diff)
                + " | [" + Signed(ci.Low) + ", " + Signed(ci.High) + "] | " + Rel(medA, rel) + " | +/-" + F(mde)
                + " | " + P(mw) + " | |");
        }

        private static void WriteIntervalLevel(StringBuilder md, Arm a, Arm b, int resamples, ulong seed)
        {
            md.AppendLine("## Interval level (shape)");
            md.AppendLine();
            float[] pa = a.Pooled, pb = b.Pooled;
            if (pa.Length == 0 || pb.Length == 0)
            {
                md.AppendLine("An arm has no pooled intervals, so there is no interval level.");
                md.AppendLine();
                return;
            }
            GHPerformanceStatistics.MannWhitneyResult mw = GHPerformanceStatistics.MannWhitneyU(pa, pb);
            float[] sa = GHPerformanceStatistics.Subsample(pa, 4000), sb = GHPerformanceStatistics.Subsample(pb, 4000);
            double hl = GHPerformanceStatistics.HodgesLehmannShift(sa, sb);
            GHPerformanceStatistics.Interval ci99 = GHPerformanceStatistics.BootstrapDifferenceCi(pa, pb, GHPerformanceStatistics.StatP99, Math.Min(resamples, 500), 0.95, seed);
            GHPerformanceStatistics.Interval ci50 = GHPerformanceStatistics.BootstrapDifferenceCi(pa, pb, GHPerformanceStatistics.StatMedian, Math.Min(resamples, 500), 0.95, seed);
            md.AppendLine("| | Value |");
            md.AppendLine("|---|---|");
            md.AppendLine("| Intervals pooled | " + pa.Length + " (" + a.Label + "), " + pb.Length + " (" + b.Label + ") |");
            md.AppendLine("| Mann-Whitney U, two-sided p | " + P(mw) + " |");
            md.AppendLine("| Cliff's delta (" + a.Label + " above " + b.Label + ") | " + Signed(mw.CliffsDelta, 3) + " (" + mw.EffectBand + ") |");
            md.AppendLine("| Hodges-Lehmann shift, " + b.Label + " minus " + a.Label + " | " + Signed(hl, 3) + " ms (subsampled to " + sa.Length + " and " + sb.Length + ") |");
            md.AppendLine("| Median difference, 95% CI | " + Signed(ci50.Point) + " ms [" + Signed(ci50.Low) + ", " + Signed(ci50.High) + "] |");
            md.AppendLine("| P99 difference, 95% CI | " + Signed(ci99.Point) + " ms [" + Signed(ci99.Low) + ", " + Signed(ci99.High) + "] |");
            md.AppendLine();
            md.AppendLine("Pooled intervals are not independent across runs, so this section describes the distributions and does not decide.");
            md.AppendLine();
        }

        private static void WriteThermal(StringBuilder md, Arm arm)
        {
            md.AppendLine("## Thermal and environment: " + arm.Label);
            md.AppendLine();
            md.AppendLine("| Run | Before | After | CPU perf % | CPU temp | GPU temp | Battery temp | Power | Power plan | Gate | Throttled |");
            md.AppendLine("|---|---|---|---|---|---|---|---|---|---|---|");
            foreach (RunRecord r in arm.Runs)
            {
                ThermalReading b = r.Thermal.Before, af = r.Thermal.After;
                md.AppendLine("| " + Path.GetFileNameWithoutExtension(r.Configuration["_file"].GetString())
                    + " | " + (b?.Status ?? "?") + " | " + (af?.Status ?? "?")
                    + " | " + Fo(b?.CpuPerformancePct) + " -> " + Fo(af?.CpuPerformancePct)
                    + " | " + Fo(b?.CpuPackageTempC) + " -> " + Fo(af?.CpuPackageTempC)
                    + " | " + Fo(b?.GpuTempC) + " -> " + Fo(af?.GpuTempC)
                    + " | " + Fo(b?.BatteryTempC) + " -> " + Fo(af?.BatteryTempC)
                    + " | " + PowerOf(r)
                    + " | " + (b?.PowerPlan ?? "?")
                    + " | " + (r.Thermal.GateSignal ?? "none")
                    + " | " + (r.Thermal.Throttled ? "**yes**: " + r.Thermal.ThrottleReason : "no") + " |");
            }
            md.AppendLine();
        }

        private static double PooledSd(float[] a, float[] b)
        {
            double sa = GHPerformanceStatistics.StdDev(a), sb = GHPerformanceStatistics.StdDev(b);
            int na = a.Length, nb = b.Length;
            if (na + nb <= 2)
                return 0;
            return Math.Sqrt(((na - 1) * sa * sa + (nb - 1) * sb * sb) / (na + nb - 2));
        }

        /* A metric absent from the run's Metrics dictionary, as NaN rather than a thrown
           KeyNotFoundException or a misleading zero */
        private static double Mv(Dictionary<string, double> m, string name) { return m.TryGetValue(name, out double v) ? v : double.NaN; }
        private static string Iv(Dictionary<string, double> m, string name) { return m.TryGetValue(name, out double v) ? ((int)v).ToString(CultureInfo.InvariantCulture) : "n/a"; }
        private static string F(double v, int decimals = 2) { return double.IsNaN(v) ? "n/a" : v.ToString("F" + decimals, CultureInfo.InvariantCulture); }
        private static string Fo(double? v) { return v.HasValue ? F(v.Value, 1) : "?"; }
        private static string Signed(double v, int decimals = 2) { return (v > 0 ? "+" : "") + F(v, decimals); }
        private static string P(GHPerformanceStatistics.MannWhitneyResult mw)
        {
            string p = mw.PTwoSided < 0.001 ? "<0.001" : F(mw.PTwoSided, 3);
            return p + (mw.Exact ? " (exact)" : " (asymptotic)");
        }
        private static string Rel(double baseline, double rel) { return baseline == 0 ? "n/a" : Signed(rel * 100, 1) + "%"; }
        private static string PowerOf(RunRecord r) { return r.Thermal?.PowerState ?? ThermalGate.PowerState(r.Thermal); }
        private static string Short(string commit) { return string.IsNullOrEmpty(commit) ? "?" : (commit.Length > 9 ? commit.Substring(0, 9) : commit); }
        private static string Join(IEnumerable<string> s) { return string.Join(", ", s.Where(x => x != null).Distinct()); }
    }
}

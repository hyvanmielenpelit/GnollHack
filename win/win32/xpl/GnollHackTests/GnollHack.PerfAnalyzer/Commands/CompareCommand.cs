using System.Globalization;
using System.Text;
using GnollHack.PerfAnalyzer.Model;
using GnollHackX.Perf;

namespace GnollHack.PerfAnalyzer.Commands
{
    /* Two-arm comparison following the plan's section 2.2:

         compare --a <dir|run.json ...> --b <dir|run.json ...> [--label-a X --label-b Y]
                 [--series external|internal] [--resamples 2000] [--seed 1]
                 [--include-throttled] [--include-excluded] [--out report.md]

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
       exchangeable, which they are not quite, so this level informs and never decides. */
    public static class CompareCommand
    {
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

            Arm armA = LoadArm(a.GetAll("a"), a.Get("label-a") ?? "A", seriesKind, includeThrottled, includeExcluded);
            Arm armB = LoadArm(a.GetAll("b"), a.Get("label-b") ?? "B", seriesKind, includeThrottled, includeExcluded);
            if (armA.Used.Count == 0 || armB.Used.Count == 0)
            {
                Console.Error.WriteLine("compare: an arm has no usable runs (A=" + armA.Used.Count + ", B=" + armB.Used.Count + ")");
                return 1;
            }

            double vsync = armA.Used.Concat(armB.Used).Select(r => r.Display.VsyncMs).Where(v => v > 0).DefaultIfEmpty(16.667).First();
            StringBuilder md = new StringBuilder();
            WriteHeader(md, armA, armB, seriesKind, vsync, resamples, seed);
            WriteRunTable(md, armA, seriesKind);
            WriteRunTable(md, armB, seriesKind);
            WriteRunLevel(md, armA, armB, vsync, resamples, seed);
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
                if (s == null || s.IntervalsMs == null || s.IntervalsMs.Length == 0)
                    continue;
                if (r.Excluded && !includeExcluded && !(includeThrottled && r.Thermal.Throttled && r.ExclusionReason != null && r.ExclusionReason.StartsWith("throttled")))
                    continue;
                if (r.Thermal.Throttled && !includeThrottled)
                    continue;
                arm.Used.Add(r);
            }
            foreach (DecisionMetric dm in MetricNames.Decision)
                arm.PerRun[dm.Name] = arm.Used.Select(r => (float)r.FindSeries(seriesKind).Metrics[dm.Name]).ToArray();
            foreach (string name in MetricNames.Reported)
                arm.PerRun[name] = arm.Used.Select(r => (float)r.FindSeries(seriesKind).Metrics.GetValueOrDefault(name)).ToArray();
            arm.Pooled = arm.Used.SelectMany(r => r.FindSeries(seriesKind).IntervalsMs).ToArray();
            return arm;
        }

        private static void WriteHeader(StringBuilder md, Arm a, Arm b, string kind, double vsync, int resamples, ulong seed)
        {
            md.AppendLine("# Performance comparison: " + a.Label + " versus " + b.Label);
            md.AppendLine();
            md.AppendLine("Generated " + DateTime.UtcNow.ToString("u", CultureInfo.InvariantCulture)
                + " by GnollHack.PerfAnalyzer. Series: `" + kind + "`. Vsync period "
                + F(vsync) + " ms. Bootstrap: " + resamples + " resamples, seed " + seed + ", 95 percent percentile intervals.");
            md.AppendLine();
            RunRecord any = a.Used[0];
            md.AppendLine("| | " + a.Label + " | " + b.Label + " |");
            md.AppendLine("|---|---|---|");
            md.AppendLine("| Runs loaded / used | " + a.Runs.Count + " / " + a.Used.Count + " | " + b.Runs.Count + " / " + b.Used.Count + " |");
            md.AppendLine("| Scenario | " + Join(a.Used.Select(r => r.Scenario)) + " | " + Join(b.Used.Select(r => r.Scenario)) + " |");
            md.AppendLine("| Build | " + Join(a.Used.Select(r => (r.Git.Tag ?? Short(r.Git.Commit)) + " " + r.BuildConfiguration)) + " | " + Join(b.Used.Select(r => (r.Git.Tag ?? Short(r.Git.Commit)) + " " + r.BuildConfiguration)) + " |");
            md.AppendLine("| Device | " + Join(a.Used.Select(r => r.Device.Model ?? r.Device.Id)) + " | " + Join(b.Used.Select(r => r.Device.Model ?? r.Device.Id)) + " |");
            md.AppendLine("| Platform | " + Join(a.Used.Select(r => r.Platform)) + " | " + Join(b.Used.Select(r => r.Platform)) + " |");
            md.AppendLine();
            if (a.Used.Any(r => r.BuildConfiguration != "Release") || b.Used.Any(r => r.BuildConfiguration != "Release"))
                md.AppendLine("> **Warning:** at least one run is not a Release build. Section 2.5 of the plan says such runs are not comparable to shipped builds.");
            if (a.Used.Select(r => r.Scenario).Distinct().Count() > 1 || b.Used.Select(r => r.Scenario).Distinct().Count() > 1
                || a.Used[0].Scenario != b.Used[0].Scenario)
                md.AppendLine("> **Warning:** scenarios differ between or within arms; the comparison is not like for like.");
            if (a.Used.Any(r => r.ScenarioKind != any.ScenarioKind) || b.Used.Any(r => r.ScenarioKind != any.ScenarioKind))
                md.AppendLine("> **Warning:** replay and gameplay runs are mixed; the plan forbids comparing them directly.");
            md.AppendLine();
        }

        private static void WriteRunTable(StringBuilder md, Arm arm, string kind)
        {
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
                    + " | " + (int)m[MetricNames.FrameCount]
                    + " | " + F(m[MetricNames.WindowSeconds], 0)
                    + " | " + F(m[MetricNames.FrameDurationP50]) + " | " + F(m[MetricNames.FrameDurationP95]) + " | " + F(m[MetricNames.FrameDurationP99])
                    + " | " + F(m[MetricNames.FrameDurationMax], 1) + " | " + F(m[MetricNames.FpsMean], 1) + " | " + F(m[MetricNames.Fps1PctLow], 1)
                    + " | " + F(m[MetricNames.HitchRatio]) + " | " + F(m[MetricNames.JankPct], 1)
                    + " | " + thermal + " | " + (used ? "yes" : "no" + (r.Excluded ? " (" + r.ExclusionReason + ")" : "")) + " |");
            }
            md.AppendLine();
        }

        private static void WriteRunLevel(StringBuilder md, Arm a, Arm b, double vsync, int resamples, ulong seed)
        {
            md.AppendLine("## Run level (decision)");
            md.AppendLine();
            md.AppendLine("Each run contributes one value. Difference is " + b.Label + " minus " + a.Label
                + "; the interval is a bootstrap CI of the difference of arm medians, resampling runs.");
            md.AppendLine();
            md.AppendLine("| Metric | " + a.Label + " median | " + b.Label + " median | Diff | 95% CI | Rel. | MDE | MWU p | Verdict |");
            md.AppendLine("|---|---|---|---|---|---|---|---|---|");
            List<string> verdicts = new List<string>();
            foreach (DecisionMetric dm in MetricNames.Decision)
                verdicts.Add(WriteDecisionRow(md, dm, a, b, vsync, resamples, seed));
            foreach (string name in MetricNames.Reported)
            {
                if (name == MetricNames.FrameCount || name == MetricNames.WindowSeconds)
                    continue;
                WriteInfoRow(md, name, a, b, resamples, seed);
            }
            md.AppendLine();
            int regressions = verdicts.Count(v => v == "REGRESSION");
            int improvements = verdicts.Count(v => v == "improvement");
            md.AppendLine("**Overall:** " + (regressions > 0 ? regressions + " decision metric(s) regressed. " : "no decision metric regressed. ")
                + (improvements > 0 ? improvements + " improved. " : "")
                + "Runs per arm: " + a.Used.Count + " and " + b.Used.Count + "."
                + (Math.Min(a.Used.Count, b.Used.Count) < 5 ? " Fewer than 5 runs per arm: treat the verdict as provisional and read the MDE column." : ""));
            md.AppendLine();
        }

        private static string WriteDecisionRow(StringBuilder md, DecisionMetric dm, Arm a, Arm b, double vsync, int resamples, ulong seed)
        {
            float[] va = a.PerRun[dm.Name], vb = b.PerRun[dm.Name];
            double medA = GHPerfStats.Median(va), medB = GHPerfStats.Median(vb);
            GHPerfStats.Interval ci = GHPerfStats.BootstrapDifferenceCi(va, vb, GHPerfStats.StatMedian, resamples, 0.95, seed);
            double diff = medB - medA;
            double rel = medA != 0 ? diff / Math.Abs(medA) : 0;
            double sdPooled = PooledSd(va, vb);
            double mde = GHPerfStats.MinimumDetectableEffect(sdPooled, Math.Min(va.Length, vb.Length), 0.05);
            GHPerfStats.MannWhitneyResult mw = GHPerfStats.MannWhitneyU(va, vb);

            bool ciExcludesZero = ci.Low > 0 || ci.High < 0;
            double absThreshold = dm.AbsoluteThreshold > 0 ? dm.AbsoluteThreshold : dm.AbsoluteThresholdVsync * vsync;
            bool beyondThreshold = (dm.RelativeThreshold > 0 && Math.Abs(rel) > dm.RelativeThreshold)
                                || (absThreshold > 0 && Math.Abs(diff) > absThreshold);
            bool worse = dm.HigherIsWorse ? diff > 0 : diff < 0;
            string verdict;
            if (ciExcludesZero && beyondThreshold)
                verdict = worse ? "REGRESSION" : "improvement";
            else if (ciExcludesZero)
                verdict = worse ? "worse, under threshold" : "better, under threshold";
            else
                verdict = "no evidence";
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
            double medA = GHPerfStats.Median(va), medB = GHPerfStats.Median(vb);
            GHPerfStats.Interval ci = GHPerfStats.BootstrapDifferenceCi(va, vb, GHPerfStats.StatMedian, resamples, 0.95, seed);
            double diff = medB - medA;
            double rel = medA != 0 ? diff / Math.Abs(medA) : 0;
            double mde = GHPerfStats.MinimumDetectableEffect(PooledSd(va, vb), Math.Min(va.Length, vb.Length), 0.05);
            GHPerfStats.MannWhitneyResult mw = GHPerfStats.MannWhitneyU(va, vb);
            md.AppendLine("| " + name + " | " + F(medA) + " | " + F(medB) + " | " + Signed(diff)
                + " | [" + Signed(ci.Low) + ", " + Signed(ci.High) + "] | " + Rel(medA, rel) + " | +/-" + F(mde)
                + " | " + P(mw) + " | |");
        }

        private static void WriteIntervalLevel(StringBuilder md, Arm a, Arm b, int resamples, ulong seed)
        {
            md.AppendLine("## Interval level (shape)");
            md.AppendLine();
            float[] pa = a.Pooled, pb = b.Pooled;
            GHPerfStats.MannWhitneyResult mw = GHPerfStats.MannWhitneyU(pa, pb);
            float[] sa = GHPerfStats.Subsample(pa, 4000), sb = GHPerfStats.Subsample(pb, 4000);
            double hl = GHPerfStats.HodgesLehmannShift(sa, sb);
            GHPerfStats.Interval ci99 = GHPerfStats.BootstrapDifferenceCi(pa, pb, GHPerfStats.StatP99, Math.Min(resamples, 500), 0.95, seed);
            GHPerfStats.Interval ci50 = GHPerfStats.BootstrapDifferenceCi(pa, pb, GHPerfStats.StatMedian, Math.Min(resamples, 500), 0.95, seed);
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
            md.AppendLine("| Run | Before | After | CPU perf % | CPU temp | GPU temp | Battery temp | Charging | Power plan | Gate | Throttled |");
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
                    + " | " + (b?.IsCharging?.ToString() ?? "?")
                    + " | " + (b?.PowerPlan ?? "?")
                    + " | " + (r.Thermal.GateSignal ?? "none")
                    + " | " + (r.Thermal.Throttled ? "**yes**: " + r.Thermal.ThrottleReason : "no") + " |");
            }
            md.AppendLine();
        }

        private static double PooledSd(float[] a, float[] b)
        {
            double sa = GHPerfStats.StdDev(a), sb = GHPerfStats.StdDev(b);
            int na = a.Length, nb = b.Length;
            if (na + nb <= 2)
                return 0;
            return Math.Sqrt(((na - 1) * sa * sa + (nb - 1) * sb * sb) / (na + nb - 2));
        }

        private static string F(double v, int decimals = 2) { return v.ToString("F" + decimals, CultureInfo.InvariantCulture); }
        private static string Fo(double? v) { return v.HasValue ? F(v.Value, 1) : "?"; }
        private static string Signed(double v, int decimals = 2) { return (v > 0 ? "+" : "") + F(v, decimals); }
        private static string P(GHPerfStats.MannWhitneyResult mw)
        {
            string p = mw.PTwoSided < 0.001 ? "<0.001" : F(mw.PTwoSided, 3);
            return p + (mw.Exact ? " (exact)" : " (asymptotic)");
        }
        private static string Rel(double baseline, double rel) { return baseline == 0 ? "n/a" : Signed(rel * 100, 1) + "%"; }
        private static string Short(string commit) { return string.IsNullOrEmpty(commit) ? "?" : (commit.Length > 9 ? commit.Substring(0, 9) : commit); }
        private static string Join(IEnumerable<string> s) { return string.Join(", ", s.Where(x => x != null).Distinct()); }
    }
}

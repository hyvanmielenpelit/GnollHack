using System;
using System.Collections.Generic;
using System.Globalization;

namespace GnollHackX.Performance
{
    /* The A/B decision logic shared by the in-app harness and the offline analyzer, so
       that a verdict computed on the device and a verdict computed from the same data on
       a desktop agree to the bit. See DEVEL/performance/README.md's Statistics section
       for the run-level and interval-level methodology this implements.

       The file must compile under C# 7.3 (the legacy netstandard2.0 project) and must not
       depend on LINQ, JSON, GHApp, GHConstants or any MAUI type. */

    /* One metric the comparison decides on: the direction in which "worse" lies and the
       pre-registered regression thresholds. A relative threshold of 0 disables the
       relative check; likewise for the two absolute thresholds. */
    public sealed class GHDecisionMetric
    {
        public string Name { get; }
        public string Unit { get; }
        public bool HigherIsWorse { get; }
        public double RelativeThreshold { get; }
        public double AbsoluteThreshold { get; }
        public double AbsoluteThresholdTargetPeriod { get; }

        public GHDecisionMetric(string name, string unit, bool higherIsWorse, double relativeThreshold,
                                double absoluteThreshold = 0, double absoluteThresholdTargetPeriod = 0)
        {
            Name = name;
            Unit = unit;
            HigherIsWorse = higherIsWorse;
            RelativeThreshold = relativeThreshold;
            AbsoluteThreshold = absoluteThreshold;
            AbsoluteThresholdTargetPeriod = absoluteThresholdTargetPeriod;
        }
    }

    /* The run-level decision for one metric: medians, the bootstrap CI of their
       difference, the minimum detectable effect, the Mann-Whitney result and the verdict
       drawn from them. */
    public struct GHMetricDecision
    {
        public GHDecisionMetric Metric;
        public double MedianA;
        public double MedianB;
        public double Diff;          /* MedianB - MedianA */
        public double Rel;           /* Diff / |MedianA|, 0 when MedianA == 0 */
        public double Mde;
        public GHPerformanceStatistics.Interval Ci;
        public GHPerformanceStatistics.MannWhitneyResult MannWhitney;
        public string Verdict;
        public bool IsRegression;
        public bool IsImprovement;
    }

    /* Which sensor spoke for a throttle classification. */
    public enum GHThrottleSignal
    {
        None = 0,
        Status = 1,
        CpuPerformancePct = 2
    }

    /* Which rule decided a throttle classification. */
    public enum GHThrottleRule
    {
        None = 0,
        StatusAboveModerate = 1,
        CpuPerformanceDrop = 2,
        StatusRoseTwoClasses = 3
    }

    public struct GHThrottleVerdict
    {
        public bool Throttled;
        public GHThrottleSignal Signal;
        public GHThrottleRule Rule;
    }

    /* The result of comparing two arms of runs: how many runs were usable, whether the
       comparison rests on too little data, and one decision per decision metric. */
    public sealed class GHComparisonResult
    {
        public string LabelA;
        public string LabelB;
        public int RunsA;            /* used runs */
        public int RunsB;
        public bool TooFewRuns;      /* fewer than MinRunsForVerdict in either arm */
        public bool Provisional;     /* !TooFewRuns and fewer than ProvisionalBelowRuns in either arm */
        public double TargetPeriodMs;
        public readonly List<GHMetricDecision> Decisions = new List<GHMetricDecision>();
        public int Regressions;
        public int Improvements;
    }

    public static class GHPerformanceComparison
    {
        public const double CadenceToleranceFraction = 0.01;

        /* Below this many used runs in either arm, a decision would rest on too little
           data to trust the bootstrap CI, so every decision row is refused instead */
        public const int MinRunsForVerdict = 3;

        /* Below this many used runs in either arm, a decision is made but flagged
           provisional: the MDE column is the more honest read at this sample size */
        public const int ProvisionalBelowRuns = 5;

        public const int DefaultResamples = 2000;
        public const ulong DefaultSeed = 1;
        public const double Confidence = 0.95;
        public const double MdeAlpha = 0.05;

        /* Rank of ThermalGate.Order's "Moderate" entry */
        public const int ModerateRank = 3;
        public const double WindowsCpuPerformanceFloorPct = 90.0;
        public const double CpuPerformanceDropPoints = 10.0;

        public const string VerdictRegression = "REGRESSION";
        public const string VerdictImprovement = "improvement";
        public const string VerdictWorseUnderThreshold = "worse, under threshold";
        public const string VerdictBetterUnderThreshold = "better, under threshold";
        public const string VerdictNoEvidence = "no evidence";
        public static readonly string VerdictTooFewRuns = "too few runs (need " + MinRunsForVerdict + ")";

        public const string FrameDurationP99 = "frameDurationP99";
        public const string HitchRatio = "hitchRatio";
        public const string Fps1PctLow = "fps1PctLow";
        public const string DisplayedFps = "displayedFps";
        public const string HitchRatioMsPerSec = "hitchRatioMsPerSec";
        public const string PacingErrorRmsMs = "pacingErrorRmsMs";

        /* The metrics an external (frameDuration-series) comparison decides on. Absolute
           thresholds expressed against the display are in target-period units, since the
           target content rate can differ from the refresh rate. */
        public static readonly GHDecisionMetric[] ExternalDecision =
        {
            new GHDecisionMetric(FrameDurationP99, "ms", higherIsWorse: true, relativeThreshold: 0.20, absoluteThresholdTargetPeriod: 1.0),
            new GHDecisionMetric(HitchRatio, "ms/s", higherIsWorse: true, relativeThreshold: 0, absoluteThreshold: 2.0),
            new GHDecisionMetric(Fps1PctLow, "fps", higherIsWorse: false, relativeThreshold: 0.10),
        };

        /* The metrics a smoothness (in-app) comparison decides on, read directly from the
           run records rather than recomputed from intervals. */
        public static readonly GHDecisionMetric[] SmoothnessDecision =
        {
            new GHDecisionMetric(HitchRatioMsPerSec, "ms/s", higherIsWorse: true, relativeThreshold: 0, absoluteThreshold: 2.0),
            new GHDecisionMetric(PacingErrorRmsMs, "ms", higherIsWorse: true, relativeThreshold: 0, absoluteThreshold: 1.0),
            new GHDecisionMetric(DisplayedFps, "fps", higherIsWorse: false, relativeThreshold: 0.05),
        };

        /* Pooled standard deviation of two independent samples, 0 when there are fewer
           than three values between them. */
        public static double PooledSd(float[] a, float[] b)
        {
            double sa = GHPerformanceStatistics.StdDev(a), sb = GHPerformanceStatistics.StdDev(b);
            int na = a.Length, nb = b.Length;
            if (na + nb <= 2)
                return 0;
            return Math.Sqrt(((na - 1) * sa * sa + (nb - 1) * sb * sb) / (na + nb - 2));
        }

        /* The run-level decision for one metric: medians, the bootstrap CI of their
           difference, the minimum detectable effect at the observed spread, the
           Mann-Whitney result and the verdict drawn from the CI and the pre-registered
           thresholds. tooFewRuns refuses a verdict regardless of what the numbers show. */
        public static GHMetricDecision Decide(GHDecisionMetric metric, float[] a, float[] b, double targetPeriodMs,
                                              bool tooFewRuns, int resamples, ulong seed)
        {
            GHMetricDecision d = new GHMetricDecision();
            d.Metric = metric;
            d.MedianA = GHPerformanceStatistics.Median(a);
            d.MedianB = GHPerformanceStatistics.Median(b);
            d.Ci = GHPerformanceStatistics.BootstrapDifferenceCi(a, b, GHPerformanceStatistics.StatMedian, resamples, Confidence, seed);
            d.Diff = d.MedianB - d.MedianA;
            d.Rel = d.MedianA != 0 ? d.Diff / Math.Abs(d.MedianA) : 0;
            double sdPooled = PooledSd(a, b);
            d.Mde = GHPerformanceStatistics.MinimumDetectableEffect(sdPooled, Math.Min(a.Length, b.Length), MdeAlpha);
            d.MannWhitney = GHPerformanceStatistics.MannWhitneyU(a, b);

            if (tooFewRuns)
            {
                /* The CI, MDE and p-value are still informative, but too few runs were used
                   to trust a verdict built on them */
                d.Verdict = VerdictTooFewRuns;
                return d;
            }

            bool ciExcludesZero = d.Ci.Low > 0 || d.Ci.High < 0;
            double absThreshold = metric.AbsoluteThreshold > 0 ? metric.AbsoluteThreshold : metric.AbsoluteThresholdTargetPeriod * targetPeriodMs;
            bool beyondThreshold = (metric.RelativeThreshold > 0 && Math.Abs(d.Rel) > metric.RelativeThreshold)
                                || (absThreshold > 0 && Math.Abs(d.Diff) > absThreshold);
            bool worse = metric.HigherIsWorse ? d.Diff > 0 : d.Diff < 0;
            if (ciExcludesZero && beyondThreshold)
            {
                d.Verdict = worse ? VerdictRegression : VerdictImprovement;
                d.IsRegression = worse;
                d.IsImprovement = !worse;
            }
            else if (ciExcludesZero)
            {
                d.Verdict = worse ? VerdictWorseUnderThreshold : VerdictBetterUnderThreshold;
            }
            else
            {
                d.Verdict = VerdictNoEvidence;
            }
            return d;
        }

        /* Counts of VerdictRegression and VerdictImprovement among a set of decision
           verdicts. */
        public static void Tally(IList<string> verdicts, out int regressions, out int improvements)
        {
            regressions = 0;
            improvements = 0;
            for (int i = 0; i < verdicts.Count; i++)
            {
                if (verdicts[i] == VerdictRegression)
                    regressions++;
                else if (verdicts[i] == VerdictImprovement)
                    improvements++;
            }
        }

        /* True when every value in the list sits within tolerance of the smallest one, or
           the list has no positive value at all (min <= 0 is treated as "no constraint"). */
        public static bool WithinTolerance(IList<double> values, double tolerance, out double min, out double max)
        {
            min = 0;
            max = 0;
            if (values.Count > 0)
            {
                min = values[0];
                max = values[0];
                for (int i = 1; i < values.Count; i++)
                {
                    double v = values[i];
                    if (v < min)
                        min = v;
                    if (v > max)
                        max = v;
                }
            }
            if (min <= 0)
                return true;
            return (max - min) / min <= tolerance;
        }

        private static List<double> FilterPositive(IList<double> values)
        {
            List<double> result = new List<double>();
            for (int i = 0; i < values.Count; i++)
            {
                if (values[i] > 0)
                    result.Add(values[i]);
            }
            return result;
        }

        /* True when the refresh period or the target period varies, among the positive
           values of each list, by more than the tolerance fraction of the smallest value. */
        public static bool CadenceMismatch(IList<double> refreshPeriodsMs, IList<double> targetPeriodsMs,
                                           double tolerance, out string detail)
        {
            List<double> refresh = FilterPositive(refreshPeriodsMs);
            double refMin, refMax;
            if (!WithinTolerance(refresh, tolerance, out refMin, out refMax))
            {
                detail = "refresh period from " + refMin.ToString("F2", CultureInfo.InvariantCulture)
                    + " to " + refMax.ToString("F2", CultureInfo.InvariantCulture) + " ms";
                return true;
            }
            List<double> target = FilterPositive(targetPeriodsMs);
            double tMin, tMax;
            if (!WithinTolerance(target, tolerance, out tMin, out tMax))
            {
                detail = "target period from " + tMin.ToString("F2", CultureInfo.InvariantCulture)
                    + " to " + tMax.ToString("F2", CultureInfo.InvariantCulture) + " ms";
                return true;
            }
            detail = null;
            return false;
        }

        /* The thermal exclusion rule (see DEVEL/performance/README.md's Protocol section). A
           run is flagged throttled when the platform reports a status above Moderate, or
           when the Windows processor performance counter's after reading is below the floor
           and either there is no before reading or the after reading is at least
           CpuPerformanceDropPoints below it, or, with no better signal, when a thermal
           reading rose by two classes across the run. A before reading alone never gates.
           cpuPerformancePctBefore/After are NaN when the platform reported no reading. */
        public static GHThrottleVerdict ClassifyThrottle(int statusRankBefore, int statusRankAfter,
                                                         double cpuPerformancePctBefore, double cpuPerformancePctAfter)
        {
            GHThrottleVerdict v = new GHThrottleVerdict();
            int rb = statusRankBefore, ra = statusRankAfter;
            if (rb > ModerateRank || ra > ModerateRank)
            {
                v.Throttled = true;
                v.Signal = GHThrottleSignal.Status;
                v.Rule = GHThrottleRule.StatusAboveModerate;
                return v;
            }

            double perfB = cpuPerformancePctBefore, perfA = cpuPerformancePctAfter;
            bool perfBHasValue = !double.IsNaN(perfB);
            bool perfAHasValue = !double.IsNaN(perfA);
            if (perfAHasValue && perfA < WindowsCpuPerformanceFloorPct
                && (!perfBHasValue || perfA < perfB - CpuPerformanceDropPoints))
            {
                v.Throttled = true;
                v.Signal = GHThrottleSignal.CpuPerformancePct;
                v.Rule = GHThrottleRule.CpuPerformanceDrop;
                return v;
            }

            if (rb > 0 && ra > 0)
            {
                v.Signal = GHThrottleSignal.Status;
                if (ra - rb >= 2)
                {
                    v.Throttled = true;
                    v.Rule = GHThrottleRule.StatusRoseTwoClasses;
                }
                return v;
            }

            if (perfBHasValue || perfAHasValue)
            {
                v.Signal = GHThrottleSignal.CpuPerformancePct;
                return v;
            }

            v.Signal = GHThrottleSignal.None;
            return v;
        }

        private static List<GHSmoothnessSummary> FilterNotNull(IList<GHSmoothnessSummary> summaries)
        {
            List<GHSmoothnessSummary> result = new List<GHSmoothnessSummary>();
            for (int i = 0; i < summaries.Count; i++)
            {
                if (summaries[i] != null)
                    result.Add(summaries[i]);
            }
            return result;
        }

        private static double SmoothnessMetricValue(GHSmoothnessSummary s, string name)
        {
            if (name == HitchRatioMsPerSec)
                return s.HitchRatioMsPerSec;
            if (name == PacingErrorRmsMs)
                return s.PacingErrorRmsMs;
            if (name == DisplayedFps)
                return s.DisplayedFps;
            return 0;
        }

        private static float[] ExtractSmoothnessMetric(List<GHSmoothnessSummary> summaries, string name)
        {
            float[] values = new float[summaries.Count];
            for (int i = 0; i < summaries.Count; i++)
                values[i] = (float)SmoothnessMetricValue(summaries[i], name);
            return values;
        }

        /* Compares two arms of in-app (schema v2) run records on the smoothness decision
           metrics, at the run level: one value per run, medians, bootstrap CI and verdict
           per SmoothnessDecision entry. Null summaries (a run with no smoothness series)
           are skipped rather than counted. */
        public static GHComparisonResult CompareSmoothness(string labelA, IList<GHSmoothnessSummary> a,
                                                           string labelB, IList<GHSmoothnessSummary> b,
                                                           double targetPeriodMs, int resamples, ulong seed)
        {
            GHComparisonResult result = new GHComparisonResult();
            result.LabelA = labelA;
            result.LabelB = labelB;
            result.TargetPeriodMs = targetPeriodMs;

            List<GHSmoothnessSummary> usedA = FilterNotNull(a);
            List<GHSmoothnessSummary> usedB = FilterNotNull(b);
            result.RunsA = usedA.Count;
            result.RunsB = usedB.Count;
            int minRuns = Math.Min(result.RunsA, result.RunsB);
            result.TooFewRuns = minRuns < MinRunsForVerdict;
            result.Provisional = !result.TooFewRuns && minRuns < ProvisionalBelowRuns;

            List<string> verdicts = new List<string>();
            for (int i = 0; i < SmoothnessDecision.Length; i++)
            {
                GHDecisionMetric metric = SmoothnessDecision[i];
                float[] va = ExtractSmoothnessMetric(usedA, metric.Name);
                float[] vb = ExtractSmoothnessMetric(usedB, metric.Name);
                GHMetricDecision decision = Decide(metric, va, vb, targetPeriodMs, result.TooFewRuns, resamples, seed);
                result.Decisions.Add(decision);
                verdicts.Add(decision.Verdict);
            }

            int regressions, improvements;
            Tally(verdicts, out regressions, out improvements);
            result.Regressions = regressions;
            result.Improvements = improvements;
            return result;
        }
    }
}

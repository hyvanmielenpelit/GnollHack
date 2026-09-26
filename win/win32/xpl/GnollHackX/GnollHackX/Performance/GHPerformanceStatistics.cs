using System;
using System.Collections.Generic;

namespace GnollHackX.Performance
{
    /* Statistics shared by the in-app harness and the offline analyzer, so that a number
       computed on the device and a number computed from the same data on a desktop agree
       to the bit. Frame-interval data is heavy-tailed and never normal: everything here
       is rank-based or resampling-based, and nothing assumes a distribution.

       The file must compile under C# 7.3 (the legacy netstandard2.0 project) and must not
       allocate in StreamingHistogram.Add or in the summary getters, which run on the UI
       thread once per frame. The batch functions (percentiles, bootstrap, tests) allocate
       freely and belong at the end of a measurement window, never inside it. */
    public static class GHPerformanceStatistics
    {
        /* Cliff's delta bands, Romano et al. 2006 */
        public const double CliffNegligible = 0.147;
        public const double CliffSmall = 0.33;
        public const double CliffMedium = 0.474;

        /* Nearest-rank percentile on an ascending array. p in [0, 100]. The rank is
           ceil(p / 100 * n), clamped to [1, n]; this is the definition GPU reviewers and
           the Android Macrobenchmark library use for frame times, and it always returns an
           observed value rather than an interpolated one. */
        public static float PercentileSorted(float[] sorted, int count, double p)
        {
            if (sorted == null || count <= 0)
                return 0f;
            if (p <= 0)
                return sorted[0];
            if (p >= 100)
                return sorted[count - 1];
            int rank = (int)Math.Ceiling(p / 100.0 * count);
            if (rank < 1)
                rank = 1;
            if (rank > count)
                rank = count;
            return sorted[rank - 1];
        }

        public static float PercentileSorted(float[] sorted, double p)
        {
            return PercentileSorted(sorted, sorted == null ? 0 : sorted.Length, p);
        }

        /* Sorts a copy; the caller's array is untouched. */
        public static float[] SortedCopy(float[] values)
        {
            float[] copy = new float[values.Length];
            Array.Copy(values, copy, values.Length);
            Array.Sort(copy);
            return copy;
        }

        public static float Percentile(float[] values, double p)
        {
            return PercentileSorted(SortedCopy(values), p);
        }

        public static double Mean(float[] values)
        {
            if (values == null || values.Length == 0)
                return 0;
            double sum = 0;
            for (int i = 0; i < values.Length; i++)
                sum += values[i];
            return sum / values.Length;
        }

        /* Sample standard deviation (n - 1). */
        public static double StdDev(float[] values)
        {
            if (values == null || values.Length < 2)
                return 0;
            double mean = Mean(values);
            double ss = 0;
            for (int i = 0; i < values.Length; i++)
            {
                double d = values[i] - mean;
                ss += d * d;
            }
            return Math.Sqrt(ss / (values.Length - 1));
        }

        public static float Median(float[] values)
        {
            return Percentile(values, 50);
        }

        /* Mean of the slowest fraction of intervals, expressed as a rate. "1 percent low
           FPS" is 1000 / mean of the largest 1 percent of frame intervals in ms. At least
           one interval is always included. */
        public static double LowFps(float[] sortedIntervalsMs, int count, double fraction)
        {
            if (sortedIntervalsMs == null || count <= 0)
                return 0;
            int n = (int)Math.Ceiling(count * fraction);
            if (n < 1)
                n = 1;
            if (n > count)
                n = count;
            double sum = 0;
            for (int i = count - n; i < count; i++)
                sum += sortedIntervalsMs[i];
            double mean = sum / n;
            return mean > 0 ? 1000.0 / mean : 0;
        }

        /* Frame pacing metrics of a series of on-screen durations, judged against the target
           content period T (1 / map FPS in effect) and the display refresh period R. A frame
           deliberately held for several refreshes to reach T is on time; a frame held at
           least half a refresh longer than T is late. Definitions:
             jankPct        share of intervals exceeding T + R/2
             hitchRatio     sum over those intervals of (interval - T), divided by the
                            window length: ms of hitch per second, the Apple hitch time
                            ratio measured against the intended cadence
             hitch2xCount   intervals exceeding 2 T
             hitch4xCount   intervals exceeding 4 T
             stutterIndex   coefficient of variation of the intervals
             overrunPxx     Pxx - T
           The window length is the sum of the intervals, so every rate here shares one
           denominator. A refresh period of zero or less is taken to equal T. */
        public struct PacingMetrics
        {
            public int Count;
            public double WindowMs;
            public double TargetPeriodMs;
            public double RefreshPeriodMs;
            public double FpsMean;
            public double Fps1PctLow;
            public double Fps01PctLow;
            public float P50;
            public float P90;
            public float P95;
            public float P99;
            public float Max;
            public float Min;
            public double MeanMs;
            public double StdDevMs;
            public double StutterIndex;
            public double JankPct;
            public double HitchRatioMsPerSec;
            public int Hitch2xCount;
            public int Hitch4xCount;
            public double Hitch2xPerMin;
            public double Hitch4xPerMin;
            public float OverrunP50;
            public float OverrunP95;
            public float OverrunP99;
        }

        public static PacingMetrics ComputePacing(float[] intervalsMs, double targetPeriodMs, double refreshPeriodMs)
        {
            PacingMetrics m = new PacingMetrics();
            if (intervalsMs == null || intervalsMs.Length == 0)
                return m;
            float[] sorted = SortedCopy(intervalsMs);
            int n = sorted.Length;
            m.Count = n;
            m.TargetPeriodMs = targetPeriodMs;
            m.RefreshPeriodMs = refreshPeriodMs > 0 ? refreshPeriodMs : targetPeriodMs;
            double window = 0;
            for (int i = 0; i < n; i++)
                window += intervalsMs[i];
            m.WindowMs = window;
            m.FpsMean = window > 0 ? n * 1000.0 / window : 0;
            m.Fps1PctLow = LowFps(sorted, n, 0.01);
            m.Fps01PctLow = LowFps(sorted, n, 0.001);
            m.P50 = PercentileSorted(sorted, n, 50);
            m.P90 = PercentileSorted(sorted, n, 90);
            m.P95 = PercentileSorted(sorted, n, 95);
            m.P99 = PercentileSorted(sorted, n, 99);
            m.Max = sorted[n - 1];
            m.Min = sorted[0];
            m.MeanMs = Mean(intervalsMs);
            m.StdDevMs = StdDev(intervalsMs);
            m.StutterIndex = m.MeanMs > 0 ? m.StdDevMs / m.MeanMs : 0;

            if (targetPeriodMs > 0)
            {
                double jank = 0, hitchSum = 0;
                int h2 = 0, h4 = 0;
                double tLate = targetPeriodMs + m.RefreshPeriodMs / 2.0;
                double t2 = 2.0 * targetPeriodMs, t4 = 4.0 * targetPeriodMs;
                for (int i = 0; i < n; i++)
                {
                    float v = intervalsMs[i];
                    if (v > tLate)
                    {
                        jank++;
                        hitchSum += v - targetPeriodMs;
                    }
                    if (v > t2)
                        h2++;
                    if (v > t4)
                        h4++;
                }
                m.JankPct = 100.0 * jank / n;
                m.HitchRatioMsPerSec = window > 0 ? hitchSum / (window / 1000.0) : 0;
                m.Hitch2xCount = h2;
                m.Hitch4xCount = h4;
                double minutes = window / 60000.0;
                m.Hitch2xPerMin = minutes > 0 ? h2 / minutes : 0;
                m.Hitch4xPerMin = minutes > 0 ? h4 / minutes : 0;
                m.OverrunP50 = (float)(m.P50 - targetPeriodMs);
                m.OverrunP95 = (float)(m.P95 - targetPeriodMs);
                m.OverrunP99 = (float)(m.P99 - targetPeriodMs);
            }
            return m;
        }

        /* Deterministic xorshift64* generator, so a bootstrap run is reproducible from
           its seed and two machines produce identical intervals from identical data. */
        public sealed class SeededRandom
        {
            private ulong _state;

            public SeededRandom(ulong seed)
            {
                _state = seed == 0 ? 0x9E3779B97F4A7C15UL : seed;
            }

            public ulong NextUInt64()
            {
                ulong x = _state;
                x ^= x >> 12;
                x ^= x << 25;
                x ^= x >> 27;
                _state = x;
                return x * 0x2545F4914F6CDD1DUL;
            }

            /* Uniform in [0, n). */
            public int Next(int n)
            {
                return (int)(NextUInt64() % (ulong)n);
            }
        }

        public delegate double Statistic(float[] sample, int count);

        public struct Interval
        {
            public double Point;
            public double Low;
            public double High;
            public int Resamples;
        }

        /* Percentile bootstrap confidence interval of a statistic of one sample. The
           interval is the (alpha/2, 1 - alpha/2) quantiles of the resampled statistics.
           Percentile bootstrap is biased for extreme quantiles of small samples; with the
           thousands of intervals a 60 s window produces it is adequate, and its
           simplicity means the device and the analyzer cannot disagree. */
        public static Interval BootstrapCi(float[] values, Statistic statistic,
                                           int resamples, double confidence, ulong seed)
        {
            Interval ci = new Interval();
            if (values == null || values.Length == 0)
                return ci;
            int n = values.Length;
            ci.Point = statistic(values, n);
            ci.Resamples = resamples;
            if (resamples <= 0)
            {
                ci.Low = ci.High = ci.Point;
                return ci;
            }
            SeededRandom rng = new SeededRandom(seed);
            float[] sample = new float[n];
            float[] stats = new float[resamples];
            for (int r = 0; r < resamples; r++)
            {
                for (int i = 0; i < n; i++)
                    sample[i] = values[rng.Next(n)];
                stats[r] = (float)statistic(sample, n);
            }
            Array.Sort(stats);
            double alpha = 1.0 - confidence;
            ci.Low = PercentileSorted(stats, resamples, 100.0 * alpha / 2.0);
            ci.High = PercentileSorted(stats, resamples, 100.0 * (1.0 - alpha / 2.0));
            return ci;
        }

        /* Bootstrap CI of statistic(b) - statistic(a), resampling each arm independently.
           A difference whose interval excludes zero is the criterion for "changed". */
        public static Interval BootstrapDifferenceCi(float[] a, float[] b, Statistic statistic,
                                                     int resamples, double confidence, ulong seed)
        {
            Interval ci = new Interval();
            if (a == null || b == null || a.Length == 0 || b.Length == 0)
                return ci;
            int na = a.Length, nb = b.Length;
            ci.Point = statistic(b, nb) - statistic(a, na);
            ci.Resamples = resamples;
            if (resamples <= 0)
            {
                ci.Low = ci.High = ci.Point;
                return ci;
            }
            SeededRandom rng = new SeededRandom(seed);
            float[] sa = new float[na];
            float[] sb = new float[nb];
            float[] stats = new float[resamples];
            for (int r = 0; r < resamples; r++)
            {
                for (int i = 0; i < na; i++)
                    sa[i] = a[rng.Next(na)];
                for (int i = 0; i < nb; i++)
                    sb[i] = b[rng.Next(nb)];
                stats[r] = (float)(statistic(sb, nb) - statistic(sa, na));
            }
            Array.Sort(stats);
            double alpha = 1.0 - confidence;
            ci.Low = PercentileSorted(stats, resamples, 100.0 * alpha / 2.0);
            ci.High = PercentileSorted(stats, resamples, 100.0 * (1.0 - alpha / 2.0));
            return ci;
        }

        /* Ready-made statistics for the bootstrap delegates. */
        public static double StatMedian(float[] s, int n) { return PercentileSorted(SortedCopy(Truncate(s, n)), n, 50); }
        public static double StatP95(float[] s, int n) { return PercentileSorted(SortedCopy(Truncate(s, n)), n, 95); }
        public static double StatP99(float[] s, int n) { return PercentileSorted(SortedCopy(Truncate(s, n)), n, 99); }
        public static double StatMean(float[] s, int n) { return Mean(Truncate(s, n)); }

        private static float[] Truncate(float[] s, int n)
        {
            if (s.Length == n)
                return s;
            float[] t = new float[n];
            Array.Copy(s, t, n);
            return t;
        }

        public struct MannWhitneyResult
        {
            public double U;            /* U statistic for sample A (ties count one half) */
            public double Z;            /* normal approximation with tie and continuity corrections */
            public double PTwoSided;
            public bool Exact;          /* true when PTwoSided came from the exact null distribution */
            public bool HasTies;
            public double CliffsDelta;  /* P(a > b) - P(a < b), in [-1, 1]; positive means A tends larger */
            public string EffectBand;   /* negligible, small, medium, large */
            public int NA;
            public int NB;
        }

        /* Below this product of sample sizes, and with no ties, the p-value is computed
           from the exact null distribution of U rather than the normal approximation.
           At 5 runs per arm the approximation is poor, and the exact test is what R's
           wilcox.test and SciPy's method="exact" give. */
        public const int ExactMannWhitneyMaxProduct = 400;

        /* Mann-Whitney U with average ranks for ties, the tie-corrected variance, and a
           continuity correction, evaluated by the normal approximation, or exactly for
           small tie-free samples. Cliff's delta follows from U directly. */
        public static MannWhitneyResult MannWhitneyU(float[] a, float[] b)
        {
            MannWhitneyResult res = new MannWhitneyResult();
            if (a == null || b == null || a.Length == 0 || b.Length == 0)
            {
                res.EffectBand = "undefined";
                return res;
            }
            int na = a.Length, nb = b.Length, n = na + nb;
            res.NA = na;
            res.NB = nb;

            /* Combined sort with group tags */
            float[] vals = new float[n];
            byte[] group = new byte[n];
            int[] order = new int[n];
            for (int i = 0; i < na; i++) { vals[i] = a[i]; group[i] = 0; }
            for (int i = 0; i < nb; i++) { vals[na + i] = b[i]; group[na + i] = 1; }
            for (int i = 0; i < n; i++)
                order[i] = i;
            Array.Sort(vals, order);

            /* Average ranks across ties; accumulate the tie correction term */
            double rankSumA = 0;
            double tieTerm = 0;
            int i0 = 0;
            while (i0 < n)
            {
                int i1 = i0;
                while (i1 + 1 < n && vals[i1 + 1] == vals[i0])
                    i1++;
                int t = i1 - i0 + 1;
                double avgRank = (i0 + 1 + i1 + 1) / 2.0;
                for (int k = i0; k <= i1; k++)
                {
                    if (group[order[k]] == 0)
                        rankSumA += avgRank;
                }
                if (t > 1)
                    tieTerm += (double)t * t * t - t;
                i0 = i1 + 1;
            }

            double uA = rankSumA - na * (na + 1) / 2.0;
            double uB = (double)na * nb - uA;
            res.U = uA;
            res.HasTies = tieTerm > 0;

            double meanU = na * (double)nb / 2.0;
            double varU = (na * (double)nb / 12.0) * ((n + 1) - tieTerm / ((double)n * (n - 1)));
            if (varU <= 0)
            {
                res.Z = 0;
                res.PTwoSided = 1;
            }
            else
            {
                double diff = uA - meanU;
                double cc = diff > 0 ? -0.5 : (diff < 0 ? 0.5 : 0);
                res.Z = (diff + cc) / Math.Sqrt(varU);
                res.PTwoSided = 2.0 * (1.0 - NormalCdf(Math.Abs(res.Z)));
                if (res.PTwoSided > 1)
                    res.PTwoSided = 1;
            }

            if (!res.HasTies && (long)na * nb <= ExactMannWhitneyMaxProduct)
            {
                res.PTwoSided = MannWhitneyExactTwoSidedP(na, nb, (int)Math.Round(Math.Min(uA, uB)));
                res.Exact = true;
            }

            res.CliffsDelta = (uA - uB) / ((double)na * nb);
            double ad = Math.Abs(res.CliffsDelta);
            res.EffectBand = ad < CliffNegligible ? "negligible"
                : ad < CliffSmall ? "small"
                : ad < CliffMedium ? "medium"
                : "large";
            return res;
        }

        /* Exact two-sided p-value for the smaller U statistic under the null hypothesis,
           by the classical recurrence on the number of arrangements: the count of
           (m, n) arrangements with statistic u is f(m-1, n, u-n) + f(m, n-1, u). The
           distribution is symmetric, so the two-sided p is twice the lower tail, capped
           at one. Counts are held as doubles; they stay exact well beyond the 400-pair
           limit above. */
        public static double MannWhitneyExactTwoSidedP(int m, int n, int uMin)
        {
            int uMax = m * n;
            if (uMin < 0)
                uMin = 0;
            if (uMin > uMax)
                uMin = uMax;
            /* Rolling arrays over j (size of B); f[i][u] for the current j */
            double[][] prev = new double[m + 1][];
            double[][] curr = new double[m + 1][];
            for (int i = 0; i <= m; i++)
            {
                prev[i] = new double[uMax + 1];
                curr[i] = new double[uMax + 1];
            }
            /* j = 0: only u = 0 is possible, one arrangement, for every i */
            for (int i = 0; i <= m; i++)
                prev[i][0] = 1;
            for (int j = 1; j <= n; j++)
            {
                for (int i = 0; i <= m; i++)
                {
                    Array.Clear(curr[i], 0, uMax + 1);
                    for (int u = 0; u <= uMax; u++)
                    {
                        double v = 0;
                        if (i > 0 && u - j >= 0)
                            v += curr[i - 1][u - j];
                        v += prev[i][u];
                        curr[i][u] = v;
                    }
                }
                double[][] t = prev;
                prev = curr;
                curr = t;
            }
            double total = 0, tail = 0;
            for (int u = 0; u <= uMax; u++)
            {
                total += prev[m][u];
                if (u <= uMin)
                    tail += prev[m][u];
            }
            double p = 2.0 * tail / total;
            return p > 1 ? 1 : p;
        }

        /* Standard normal CDF via the complementary error function (Abramowitz and
           Stegun 7.1.26, absolute error below 1.5e-7). */
        public static double NormalCdf(double x)
        {
            return 0.5 * Erfc(-x / Math.Sqrt(2.0));
        }

        private static double Erfc(double x)
        {
            double z = Math.Abs(x);
            double t = 1.0 / (1.0 + 0.5 * z);
            double r = t * Math.Exp(-z * z - 1.26551223 + t * (1.00002368 + t * (0.37409196
                + t * (0.09678418 + t * (-0.18628806 + t * (0.27886807 + t * (-1.13520398
                + t * (1.48851587 + t * (-0.82215223 + t * 0.17087277)))))))));
            return x >= 0 ? r : 2.0 - r;
        }

        /* Hodges-Lehmann shift: the median of all pairwise differences b - a. It is the
           interpretable companion to Cliff's delta ("B is 0.8 ms slower"), and its
           breakdown point is high. O(na * nb) memory and time; the caller guards size. */
        public static double HodgesLehmannShift(float[] a, float[] b)
        {
            if (a == null || b == null || a.Length == 0 || b.Length == 0)
                return 0;
            long total = (long)a.Length * b.Length;
            if (total > 50_000_000L)
                throw new ArgumentException("Hodges-Lehmann shift: sample product too large; subsample first");
            float[] diffs = new float[total];
            long k = 0;
            for (int i = 0; i < a.Length; i++)
                for (int j = 0; j < b.Length; j++)
                    diffs[k++] = b[j] - a[i];
            Array.Sort(diffs);
            return PercentileSorted(diffs, diffs.Length, 50);
        }

        /* Takes every k-th element so that an O(n*m) statistic stays affordable. */
        public static float[] Subsample(float[] values, int maxCount)
        {
            if (values == null || values.Length <= maxCount)
                return values;
            int step = (int)Math.Ceiling(values.Length / (double)maxCount);
            List<float> outList = new List<float>(maxCount + 1);
            for (int i = 0; i < values.Length; i += step)
                outList.Add(values[i]);
            return outList.ToArray();
        }

        /* Minimum detectable effect for a two-arm comparison of per-run summaries at the
           given per-arm run count, expressed in the summary's unit: the shift that a
           two-sided test at alpha with power 0.8 would detect, from the pooled standard
           deviation of the per-run values. Uses the normal approximation, which is
           optimistic at n = 5; it is reported so that a "no difference" result can be
           read as "no difference larger than X", never as "equal". */
        public static double MinimumDetectableEffect(double pooledSd, int runsPerArm, double alpha)
        {
            if (runsPerArm <= 0 || pooledSd <= 0)
                return 0;
            double zAlpha = NormalQuantile(1.0 - alpha / 2.0);
            double zPower = NormalQuantile(0.8);
            return (zAlpha + zPower) * pooledSd * Math.Sqrt(2.0 / runsPerArm);
        }

        /* Inverse normal CDF, Acklam's rational approximation (relative error 1.15e-9). */
        public static double NormalQuantile(double p)
        {
            if (p <= 0)
                return double.NegativeInfinity;
            if (p >= 1)
                return double.PositiveInfinity;
            double[] a = { -3.969683028665376e+01, 2.209460984245205e+02, -2.759285104469687e+02,
                           1.383577518672690e+02, -3.066479806614716e+01, 2.506628277459239e+00 };
            double[] b = { -5.447609879822406e+01, 1.615858368580409e+02, -1.556989798598866e+02,
                           6.680131188771972e+01, -1.328068155288572e+01 };
            double[] c = { -7.784894002430293e-03, -3.223964580411365e-01, -2.400758277161838e+00,
                           -2.549732539343734e+00, 4.374664141464968e+00, 2.938163982698783e+00 };
            double[] d = { 7.784695709041462e-03, 3.224671290700398e-01, 2.445134137142996e+00,
                           3.754408661907416e+00 };
            const double plow = 0.02425, phigh = 1 - plow;
            double q, r;
            if (p < plow)
            {
                q = Math.Sqrt(-2 * Math.Log(p));
                return (((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5])
                     / ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1);
            }
            if (p > phigh)
            {
                q = Math.Sqrt(-2 * Math.Log(1 - p));
                return -(((((c[0] * q + c[1]) * q + c[2]) * q + c[3]) * q + c[4]) * q + c[5])
                      / ((((d[0] * q + d[1]) * q + d[2]) * q + d[3]) * q + 1);
            }
            q = p - 0.5;
            r = q * q;
            return (((((a[0] * r + a[1]) * r + a[2]) * r + a[3]) * r + a[4]) * r + a[5]) * q
                 / (((((b[0] * r + b[1]) * r + b[2]) * r + b[3]) * r + b[4]) * r + 1);
        }

        /* Fixed log-spaced histogram for per-frame accumulation on the UI thread. Add and
           the getters never allocate. Bins are geometric from MinMs to MaxMs with Bins
           steps, so resolution is about 4 percent of the value everywhere, which is finer
           than a vsync period at every frame rate. Values outside the range land in the
           end bins; the exact minimum and maximum are kept separately so Max is never a
           bin edge. Percentiles interpolate linearly inside the bin, which errs by at most
           the bin width. */
        public sealed class StreamingHistogram
        {
            public const float MinMs = 0.25f;
            public const float MaxMs = 4000f;
            public const int Bins = 240;

            private readonly long[] _counts = new long[Bins];
            private readonly double _logMin;
            private readonly double _logStep;
            private long _count;
            private double _sum;
            private double _m2;       /* Welford second moment */
            private double _mean;
            private float _min = float.MaxValue;
            private float _max = float.MinValue;

            public StreamingHistogram()
            {
                _logMin = Math.Log(MinMs);
                _logStep = (Math.Log(MaxMs) - _logMin) / Bins;
            }

            public long Count { get { return _count; } }
            public double Sum { get { return _sum; } }
            public double Mean { get { return _count > 0 ? _mean : 0; } }
            public double StdDev { get { return _count > 1 ? Math.Sqrt(_m2 / (_count - 1)) : 0; } }
            public float Min { get { return _count > 0 ? _min : 0; } }
            public float Max { get { return _count > 0 ? _max : 0; } }

            public void Reset()
            {
                Array.Clear(_counts, 0, Bins);
                _count = 0;
                _sum = 0;
                _m2 = 0;
                _mean = 0;
                _min = float.MaxValue;
                _max = float.MinValue;
            }

            public void Add(float valueMs)
            {
                if (float.IsNaN(valueMs) || float.IsInfinity(valueMs))
                    return;
                _counts[BinIndex(valueMs)]++;
                _count++;
                _sum += valueMs;
                double delta = valueMs - _mean;
                _mean += delta / _count;
                _m2 += delta * (valueMs - _mean);
                if (valueMs < _min)
                    _min = valueMs;
                if (valueMs > _max)
                    _max = valueMs;
            }

            private int BinIndex(float v)
            {
                if (v <= MinMs)
                    return 0;
                if (v >= MaxMs)
                    return Bins - 1;
                int idx = (int)((Math.Log(v) - _logMin) / _logStep);
                if (idx < 0)
                    idx = 0;
                if (idx >= Bins)
                    idx = Bins - 1;
                return idx;
            }

            private float BinLow(int idx)
            {
                return (float)Math.Exp(_logMin + idx * _logStep);
            }

            private float BinHigh(int idx)
            {
                return (float)Math.Exp(_logMin + (idx + 1) * _logStep);
            }

            /* Nearest-rank percentile located by bin, interpolated within the bin. */
            public float Percentile(double p)
            {
                if (_count == 0)
                    return 0;
                if (p <= 0)
                    return _min;
                if (p >= 100)
                    return _max;
                long rank = (long)Math.Ceiling(p / 100.0 * _count);
                if (rank < 1)
                    rank = 1;
                long cum = 0;
                for (int i = 0; i < Bins; i++)
                {
                    long c = _counts[i];
                    if (cum + c >= rank)
                    {
                        double frac = c > 0 ? (rank - cum) / (double)c : 0;
                        float lo = BinLow(i), hi = BinHigh(i);
                        float v = (float)(lo + (hi - lo) * frac);
                        if (v < _min)
                            v = _min;
                        if (v > _max)
                            v = _max;
                        return v;
                    }
                    cum += c;
                }
                return _max;
            }

            /* Number of values strictly above a threshold, by bin, exact for thresholds
               that are bin edges and within one bin otherwise. */
            public long CountAbove(float thresholdMs)
            {
                if (_count == 0)
                    return 0;
                int idx = BinIndex(thresholdMs);
                long c = 0;
                for (int i = idx + 1; i < Bins; i++)
                    c += _counts[i];
                return c;
            }

            public void CopyCounts(long[] destination)
            {
                Array.Copy(_counts, destination, Bins);
            }
        }
    }
}

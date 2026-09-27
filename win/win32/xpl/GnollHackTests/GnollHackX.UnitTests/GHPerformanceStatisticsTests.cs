using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    public class GHPerformanceStatisticsTests
    {
        private static readonly float[] Sorted5 = { 15f, 20f, 35f, 40f, 50f };

        /* Frame-interval-like sample: 8..18 ms with a 5 percent tail of up to 200 ms
           extra, from the library's own deterministic generator. */
        private static float[] HeavyTailedSample(int count, ulong seed)
        {
            GHPerformanceStatistics.SeededRandom rng = new GHPerformanceStatistics.SeededRandom(seed);
            float[] values = new float[count];
            int i;
            for (i = 0; i < count; i++)
            {
                float ms = 8f + rng.Next(10000) / 1000f;
                if (rng.Next(100) < 5)
                    ms += rng.Next(200000) / 1000f;
                values[i] = ms;
            }
            return values;
        }

        [Theory]
        [InlineData(40, 20f)]
        [InlineData(100, 50f)]
        [InlineData(5, 15f)]
        [InlineData(30, 20f)]
        [InlineData(50, 35f)]
        public void PercentileSorted_UsesNearestRank(double p, float expected)
        {
            Assert.Equal(expected, GHPerformanceStatistics.PercentileSorted(Sorted5, p));
            Assert.Equal(expected, GHPerformanceStatistics.PercentileSorted(Sorted5, Sorted5.Length, p));
        }

        [Fact]
        public void Percentile_SortsACopyAndLeavesInputUntouched()
        {
            float[] unsorted = { 50f, 15f, 40f, 20f, 35f };
            Assert.Equal(35f, GHPerformanceStatistics.Percentile(unsorted, 50));
            Assert.Equal(new float[] { 50f, 15f, 40f, 20f, 35f }, unsorted);
        }

        /* T and R in ms: a divisor cadence holds T; a non-divisor one holds the next whole
           refresh count above T; a measured period a hair off still rounds as intended */
        [Theory]
        [InlineData(16.6667, 16.6667, 16.6667)]
        [InlineData(25.0, 16.6667, 33.3333)]
        [InlineData(16.6667, 6.9444, 20.8333)]
        [InlineData(33.3333, 16.6700, 33.3400)]
        [InlineData(33.3333, 16.6600, 33.3333)]
        [InlineData(10.0, 16.6667, 16.6667)]
        public void OnTimeHold_IsTheNextWholeRefreshCount(double targetMs, double refreshMs, double expected)
        {
            Assert.Equal(expected, GHPerformanceStatistics.OnTimeHold(targetMs, refreshMs), 3);
        }

        [Fact]
        public void NormalCdf_MatchesTable()
        {
            Assert.Equal(0.9750021, GHPerformanceStatistics.NormalCdf(1.96), 2e-6);
            Assert.Equal(0.5, GHPerformanceStatistics.NormalCdf(0), 2e-6);
        }

        [Fact]
        public void NormalQuantile_MatchesTable()
        {
            Assert.Equal(1.959964, GHPerformanceStatistics.NormalQuantile(0.975), 1e-5);
            Assert.Equal(-1.959964, GHPerformanceStatistics.NormalQuantile(0.025), 1e-5);
            Assert.Equal(0.0, GHPerformanceStatistics.NormalQuantile(0.5), 1e-9);
        }

        [Fact]
        public void MannWhitney_FullySeparated_IsExact()
        {
            float[] a = { 1f, 2f, 3f, 4f, 5f };
            float[] b = { 6f, 7f, 8f, 9f, 10f };
            GHPerformanceStatistics.MannWhitneyResult r = GHPerformanceStatistics.MannWhitneyU(a, b);
            Assert.Equal(0.0, r.U);
            Assert.True(r.Exact);
            Assert.False(r.HasTies);
            Assert.Equal(2.0 / 252.0, r.PTwoSided, 1e-6);
            Assert.Equal(-1.0, r.CliffsDelta, 1e-12);
            Assert.Equal("large", r.EffectBand);
            Assert.Equal(5, r.NA);
            Assert.Equal(5, r.NB);
        }

        [Fact]
        public void MannWhitney_WithTies_UsesAsymptoticP()
        {
            float[] a = { 1f, 2f, 2f, 3f };
            float[] b = { 2f, 3f, 3f, 4f };
            GHPerformanceStatistics.MannWhitneyResult r = GHPerformanceStatistics.MannWhitneyU(a, b);
            Assert.Equal(3.0, r.U);
            Assert.True(r.HasTies);
            Assert.False(r.Exact);
            Assert.Equal(0.17205, r.PTwoSided, 1e-3);
        }

        [Fact]
        public void MannWhitney_PartialOverlap_IsExact()
        {
            float[] a = { 1f, 2f, 3f, 4f, 5f };
            float[] b = { 3.5f, 6f, 7f, 8f, 9f };
            GHPerformanceStatistics.MannWhitneyResult r = GHPerformanceStatistics.MannWhitneyU(a, b);
            Assert.Equal(2.0, r.U);
            Assert.True(r.Exact);
            Assert.Equal(8.0 / 252.0, r.PTwoSided, 1e-6);
        }

        [Fact]
        public void MannWhitney_EmptyInput_IsUndefined()
        {
            GHPerformanceStatistics.MannWhitneyResult r = GHPerformanceStatistics.MannWhitneyU(new float[0], new float[] { 1f });
            Assert.Equal("undefined", r.EffectBand);
        }

        [Fact]
        public void HodgesLehmannShift_IsMedianOfPairwiseDifferences()
        {
            float[] a = { 1f, 2f, 3f };
            float[] b = { 4f, 5f, 6f };
            Assert.Equal(3.0, GHPerformanceStatistics.HodgesLehmannShift(a, b));
        }

        /* Target period 16.667 ms equal to the refresh period, matching a display that
           can only present whole vsyncs: a frame held to 16.667 ms is on time, and two
           frames stretched to 50 ms each are late by more than half a refresh and clear
           the 2T hitch threshold without reaching 4T. */
        [Fact]
        public void ComputePacing_CountsHitchesAgainstTargetPeriod()
        {
            float[] intervals = new float[102];
            int i;
            for (i = 0; i < 100; i++)
                intervals[i] = 16.667f;
            intervals[100] = 50f;
            intervals[101] = 50f;

            GHPerformanceStatistics.PacingMetrics m = GHPerformanceStatistics.ComputePacing(intervals, 16.667, 16.667);
            Assert.Equal(102, m.Count);
            Assert.Equal(2, m.Hitch2xCount);
            Assert.Equal(0, m.Hitch4xCount);
            Assert.Equal(200.0 / 102.0, m.JankPct, 1e-9);
            Assert.Equal(50f, m.P99);
            Assert.Equal(50f, m.Max);
            Assert.Equal(16.667f, m.Min);
            Assert.Equal(20.0, m.Fps1PctLow, 1e-9);
            Assert.Equal(16.667, m.TargetPeriodMs);
            Assert.Equal(16.667, m.RefreshPeriodMs);
            /* hitchSum = 2 * (50 - 16.667) = 66.666; window = 100*16.667 + 100 = 1766.7 ms */
            Assert.Equal(66.666 / 1.7667, m.HitchRatioMsPerSec, 1e-3);
        }

        [Fact]
        public void ComputePacing_EmptyInput_IsZero()
        {
            GHPerformanceStatistics.PacingMetrics m = GHPerformanceStatistics.ComputePacing(new float[0], 16.667, 16.667);
            Assert.Equal(0, m.Count);
        }

        /* A series held exactly at the target period, with the refresh period at half
           the target (the display presents twice per intended frame): T + R/2 = 20.8335,
           and 16.667 never exceeds it, so nothing is jank and there is no hitch time. */
        [Fact]
        public void ComputePacing_HeldAtTargetWithHalfRefresh_HasNoJankOrHitch()
        {
            float[] intervals = new float[50];
            int i;
            for (i = 0; i < intervals.Length; i++)
                intervals[i] = 16.667f;

            GHPerformanceStatistics.PacingMetrics m = GHPerformanceStatistics.ComputePacing(intervals, 16.667, 8.333);
            Assert.Equal(0.0, m.JankPct, 1e-9);
            Assert.Equal(0.0, m.HitchRatioMsPerSec, 1e-9);
        }

        /* At T = 16.667, R = 11.111 (60 FPS on 90 Hz) a frame is held one or two refreshes
           on time, so the late threshold is 2R + R/2 = 27.78 ms. Alternating two-refresh
           (22.2 ms) and three-refresh (33.3 ms) holds: exactly half the intervals are jank. */
        [Fact]
        public void ComputePacing_AlternatingIntervals_HalfExceedLateThreshold()
        {
            float[] intervals = new float[100];
            int i;
            for (i = 0; i < intervals.Length; i++)
                intervals[i] = (i % 2 == 0) ? 22.2f : 33.3f;

            GHPerformanceStatistics.PacingMetrics m = GHPerformanceStatistics.ComputePacing(intervals, 16.667, 11.111);
            Assert.Equal(50.0, m.JankPct, 1e-9);
        }

        /* The two-refresh hold of a non-divisor cadence is on time, not jank */
        [Fact]
        public void ComputePacing_TwoRefreshHoldOfANonDivisorCadence_IsNotJank()
        {
            float[] intervals = new float[100];
            int i;
            for (i = 0; i < intervals.Length; i++)
                intervals[i] = (i % 2 == 0) ? 16.683f : 33.367f;

            GHPerformanceStatistics.PacingMetrics m = GHPerformanceStatistics.ComputePacing(intervals, 25.0, 16.683);
            Assert.Equal(0.0, m.JankPct, 1e-9);
        }

        [Fact]
        public void StreamingHistogram_TracksExactSampleClosely()
        {
            float[] values = HeavyTailedSample(20000, 42);
            GHPerformanceStatistics.StreamingHistogram h = new GHPerformanceStatistics.StreamingHistogram();
            int i;
            for (i = 0; i < values.Length; i++)
                h.Add(values[i]);

            float[] sorted = GHPerformanceStatistics.SortedCopy(values);
            Assert.Equal((long)values.Length, h.Count);
            Assert.Equal(sorted[sorted.Length - 1], h.Max);
            Assert.Equal(sorted[0], h.Min);
            Assert.Equal(GHPerformanceStatistics.Mean(values), h.Mean, 1e-3);

            double[] ps = { 50, 90, 95, 99, 99.9 };
            for (i = 0; i < ps.Length; i++)
            {
                float exact = GHPerformanceStatistics.PercentileSorted(sorted, ps[i]);
                float approx = h.Percentile(ps[i]);
                Assert.InRange(approx, exact * 0.95f, exact * 1.05f);
            }
            Assert.Equal(h.Min, h.Percentile(0));
            Assert.Equal(h.Max, h.Percentile(100));
        }

        [Fact]
        public void StreamingHistogram_ResetClearsEverything()
        {
            GHPerformanceStatistics.StreamingHistogram h = new GHPerformanceStatistics.StreamingHistogram();
            h.Add(10f);
            h.Add(20f);
            h.Reset();
            Assert.Equal(0L, h.Count);
            Assert.Equal(0f, h.Max);
            Assert.Equal(0f, h.Percentile(50));
        }

        [Fact]
        public void BootstrapCi_IsReproducibleAndContainsPoint()
        {
            float[] values = HeavyTailedSample(300, 1);
            GHPerformanceStatistics.Interval first = GHPerformanceStatistics.BootstrapCi(values, GHPerformanceStatistics.StatMedian, 200, 0.95, 99);
            GHPerformanceStatistics.Interval second = GHPerformanceStatistics.BootstrapCi(values, GHPerformanceStatistics.StatMedian, 200, 0.95, 99);
            Assert.Equal(first.Point, second.Point);
            Assert.Equal(first.Low, second.Low);
            Assert.Equal(first.High, second.High);
            Assert.Equal(200, first.Resamples);
            Assert.True(first.Low <= first.Point && first.Point <= first.High);
        }

        [Fact]
        public void BootstrapDifferenceCi_ShiftedSampleExcludesZero()
        {
            float[] a = HeavyTailedSample(300, 3);
            float[] b = new float[a.Length];
            int i;
            for (i = 0; i < a.Length; i++)
                b[i] = a[i] + 5f;
            GHPerformanceStatistics.Interval ci = GHPerformanceStatistics.BootstrapDifferenceCi(a, b, GHPerformanceStatistics.StatMedian, 200, 0.95, 5);
            Assert.True(ci.Low > 0);
            Assert.True(ci.Low <= ci.Point && ci.Point <= ci.High);
        }

        [Fact]
        public void MinimumDetectableEffect_MatchesNormalApproximation()
        {
            Assert.Equal(1.772, GHPerformanceStatistics.MinimumDetectableEffect(1.0, 5, 0.05), 1e-3);
            Assert.Equal(0.0, GHPerformanceStatistics.MinimumDetectableEffect(0.0, 5, 0.05));
        }

        [Fact]
        public void StreamingHistogram_AddAndPercentile_DoNotAllocate()
        {
            GHPerformanceStatistics.StreamingHistogram h = new GHPerformanceStatistics.StreamingHistogram();
            GHPerformanceStatistics.SeededRandom rng = new GHPerformanceStatistics.SeededRandom(7);
            float sink = 0f;

            /* Warm-up so that JIT work and first-call setup are not counted */
            sink += FillAndQuery(h, rng);

            h.Reset();
            long before = GC.GetAllocatedBytesForCurrentThread();
            sink += FillAndQuery(h, rng);
            long after = GC.GetAllocatedBytesForCurrentThread();

            Assert.Equal(before, after);
            Assert.True(sink > 0f);
        }

        private static float FillAndQuery(GHPerformanceStatistics.StreamingHistogram h, GHPerformanceStatistics.SeededRandom rng)
        {
            int i;
            for (i = 0; i < 100000; i++)
                h.Add(8f + rng.Next(10000) / 1000f);
            return h.Percentile(99);
        }
    }
}

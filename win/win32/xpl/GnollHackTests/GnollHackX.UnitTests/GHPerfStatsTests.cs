using GnollHackX.Perf;
using Xunit;

namespace GnollHackX.UnitTests
{
    public class GHPerfStatsTests
    {
        private static readonly float[] Sorted5 = { 15f, 20f, 35f, 40f, 50f };

        /* Frame-interval-like sample: 8..18 ms with a 5 percent tail of up to 200 ms
           extra, from the library's own deterministic generator. */
        private static float[] HeavyTailedSample(int count, ulong seed)
        {
            GHPerfStats.SeededRandom rng = new GHPerfStats.SeededRandom(seed);
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
            Assert.Equal(expected, GHPerfStats.PercentileSorted(Sorted5, p));
            Assert.Equal(expected, GHPerfStats.PercentileSorted(Sorted5, Sorted5.Length, p));
        }

        [Fact]
        public void Percentile_SortsACopyAndLeavesInputUntouched()
        {
            float[] unsorted = { 50f, 15f, 40f, 20f, 35f };
            Assert.Equal(35f, GHPerfStats.Percentile(unsorted, 50));
            Assert.Equal(new float[] { 50f, 15f, 40f, 20f, 35f }, unsorted);
        }

        [Fact]
        public void NormalCdf_MatchesTable()
        {
            Assert.Equal(0.9750021, GHPerfStats.NormalCdf(1.96), 2e-6);
            Assert.Equal(0.5, GHPerfStats.NormalCdf(0), 2e-6);
        }

        [Fact]
        public void NormalQuantile_MatchesTable()
        {
            Assert.Equal(1.959964, GHPerfStats.NormalQuantile(0.975), 1e-5);
            Assert.Equal(-1.959964, GHPerfStats.NormalQuantile(0.025), 1e-5);
            Assert.Equal(0.0, GHPerfStats.NormalQuantile(0.5), 1e-9);
        }

        [Fact]
        public void MannWhitney_FullySeparated_IsExact()
        {
            float[] a = { 1f, 2f, 3f, 4f, 5f };
            float[] b = { 6f, 7f, 8f, 9f, 10f };
            GHPerfStats.MannWhitneyResult r = GHPerfStats.MannWhitneyU(a, b);
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
            GHPerfStats.MannWhitneyResult r = GHPerfStats.MannWhitneyU(a, b);
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
            GHPerfStats.MannWhitneyResult r = GHPerfStats.MannWhitneyU(a, b);
            Assert.Equal(2.0, r.U);
            Assert.True(r.Exact);
            Assert.Equal(8.0 / 252.0, r.PTwoSided, 1e-6);
        }

        [Fact]
        public void MannWhitney_EmptyInput_IsUndefined()
        {
            GHPerfStats.MannWhitneyResult r = GHPerfStats.MannWhitneyU(new float[0], new float[] { 1f });
            Assert.Equal("undefined", r.EffectBand);
        }

        [Fact]
        public void HodgesLehmannShift_IsMedianOfPairwiseDifferences()
        {
            float[] a = { 1f, 2f, 3f };
            float[] b = { 4f, 5f, 6f };
            Assert.Equal(3.0, GHPerfStats.HodgesLehmannShift(a, b));
        }

        [Fact]
        public void ComputePacing_CountsHitchesAgainstVsync()
        {
            float[] intervals = new float[102];
            int i;
            for (i = 0; i < 100; i++)
                intervals[i] = 16.667f;
            intervals[100] = 50f;
            intervals[101] = 50f;

            GHPerfStats.PacingMetrics m = GHPerfStats.ComputePacing(intervals, 16.667);
            Assert.Equal(102, m.Count);
            Assert.Equal(2, m.Hitch2xCount);
            Assert.Equal(0, m.Hitch4xCount);
            Assert.Equal(200.0 / 102.0, m.JankPct, 1e-9);
            Assert.Equal(50f, m.P99);
            Assert.Equal(50f, m.Max);
            Assert.Equal(16.667f, m.Min);
            Assert.Equal(20.0, m.Fps1PctLow, 1e-9);
            Assert.Equal(16.667, m.VsyncMs);
        }

        [Fact]
        public void ComputePacing_EmptyInput_IsZero()
        {
            GHPerfStats.PacingMetrics m = GHPerfStats.ComputePacing(new float[0], 16.667);
            Assert.Equal(0, m.Count);
        }

        [Fact]
        public void StreamingHistogram_TracksExactSampleClosely()
        {
            float[] values = HeavyTailedSample(20000, 42);
            GHPerfStats.StreamingHistogram h = new GHPerfStats.StreamingHistogram();
            int i;
            for (i = 0; i < values.Length; i++)
                h.Add(values[i]);

            float[] sorted = GHPerfStats.SortedCopy(values);
            Assert.Equal((long)values.Length, h.Count);
            Assert.Equal(sorted[sorted.Length - 1], h.Max);
            Assert.Equal(sorted[0], h.Min);
            Assert.Equal(GHPerfStats.Mean(values), h.Mean, 1e-3);

            double[] ps = { 50, 90, 95, 99, 99.9 };
            for (i = 0; i < ps.Length; i++)
            {
                float exact = GHPerfStats.PercentileSorted(sorted, ps[i]);
                float approx = h.Percentile(ps[i]);
                Assert.InRange(approx, exact * 0.95f, exact * 1.05f);
            }
            Assert.Equal(h.Min, h.Percentile(0));
            Assert.Equal(h.Max, h.Percentile(100));
        }

        [Fact]
        public void StreamingHistogram_ResetClearsEverything()
        {
            GHPerfStats.StreamingHistogram h = new GHPerfStats.StreamingHistogram();
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
            GHPerfStats.Interval first = GHPerfStats.BootstrapCi(values, GHPerfStats.StatMedian, 200, 0.95, 99);
            GHPerfStats.Interval second = GHPerfStats.BootstrapCi(values, GHPerfStats.StatMedian, 200, 0.95, 99);
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
            GHPerfStats.Interval ci = GHPerfStats.BootstrapDifferenceCi(a, b, GHPerfStats.StatMedian, 200, 0.95, 5);
            Assert.True(ci.Low > 0);
            Assert.True(ci.Low <= ci.Point && ci.Point <= ci.High);
        }

        [Fact]
        public void MinimumDetectableEffect_MatchesNormalApproximation()
        {
            Assert.Equal(1.772, GHPerfStats.MinimumDetectableEffect(1.0, 5, 0.05), 1e-3);
            Assert.Equal(0.0, GHPerfStats.MinimumDetectableEffect(0.0, 5, 0.05));
        }

        [Fact]
        public void StreamingHistogram_AddAndPercentile_DoNotAllocate()
        {
            GHPerfStats.StreamingHistogram h = new GHPerfStats.StreamingHistogram();
            GHPerfStats.SeededRandom rng = new GHPerfStats.SeededRandom(7);
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

        private static float FillAndQuery(GHPerfStats.StreamingHistogram h, GHPerfStats.SeededRandom rng)
        {
            int i;
            for (i = 0; i < 100000; i++)
                h.Add(8f + rng.Next(10000) / 1000f);
            return h.Percentile(99);
        }
    }
}

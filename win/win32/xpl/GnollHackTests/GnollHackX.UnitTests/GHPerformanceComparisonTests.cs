using System.Collections.Generic;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers the A/B decision logic in GHPerformanceComparison: per-metric verdicts,
       the run-count gates on CompareSmoothness, the pooled standard deviation, the
       cadence-mismatch detector and the thermal throttle classifier. */
    public class GHPerformanceComparisonTests
    {
        private const int Resamples = GHPerformanceComparison.DefaultResamples;
        private const ulong Seed = GHPerformanceComparison.DefaultSeed;
        private const double TargetPeriodMs = 16.667;

        [Fact]
        public void Decide_HigherIsWorseAbsoluteMetric_ClearOffset_IsRegression()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[0]; /* hitchRatioMsPerSec */
            float[] a = { 1f, 1.1f, 0.9f, 1f, 1.05f };
            float[] b = { 6f, 6.1f, 5.9f, 6f, 6.05f }; /* A + 5, well past the 2.0 ms/s threshold */

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictRegression, d.Verdict);
            Assert.True(d.IsRegression);
            Assert.False(d.IsImprovement);
        }

        [Fact]
        public void Decide_HigherIsWorseAbsoluteMetric_ClearNegativeOffset_IsImprovement()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[0];
            float[] a = { 6f, 6.1f, 5.9f, 6f, 6.05f };
            float[] b = { 1f, 1.1f, 0.9f, 1f, 1.05f }; /* A - 5 */

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictImprovement, d.Verdict);
            Assert.True(d.IsImprovement);
            Assert.False(d.IsRegression);
        }

        [Fact]
        public void Decide_CiExcludesZeroButDiffBelowAbsoluteThreshold_IsWorseUnderThreshold()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[0]; /* absolute threshold 2.0 ms/s */
            float[] a = { 1.000f, 1.001f, 0.999f, 1.000f, 1.002f };
            float[] b = { 1.500f, 1.501f, 1.499f, 1.500f, 1.502f }; /* +0.5, no overlap, under the 2.0 threshold */

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictWorseUnderThreshold, d.Verdict);
            Assert.False(d.IsRegression);
            Assert.False(d.IsImprovement);
        }

        [Fact]
        public void Decide_OverlappingNoisyArms_IsNoEvidence()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[0];
            float[] a = { 1f, 3f, 0.5f, 2f, 1.5f };
            float[] b = { 1.4f, 2.6f, 0.7f, 1.9f, 1.6f }; /* close medians, wide and overlapping spread */

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictNoEvidence, d.Verdict);
            Assert.False(d.IsRegression);
            Assert.False(d.IsImprovement);
        }

        [Fact]
        public void Decide_RelativeThresholdMetric_TenPercentLower_IsRegression()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[2]; /* displayedFps, lower is worse, 5% */
            float[] a = { 60f, 60.1f, 59.9f, 60f, 60.05f };
            float[] b = { 54f, 54.1f, 53.9f, 54f, 54.05f }; /* -10% */

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictRegression, d.Verdict);
            Assert.True(d.IsRegression);
        }

        [Fact]
        public void Decide_RelativeThresholdMetric_TwoPercentLowerButConsistent_IsWorseUnderThreshold()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[2];
            float[] a = { 60f, 60.1f, 59.9f, 60f, 60.05f };
            float[] b = { 58.8f, 58.9f, 58.7f, 58.8f, 58.85f }; /* -2%, no overlap with A */

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictWorseUnderThreshold, d.Verdict);
            Assert.False(d.IsRegression);
        }

        [Fact]
        public void Decide_AbsoluteThresholdTargetPeriod_DecidesOverRelativeThreshold()
        {
            /* frameDurationP99: relative threshold 20%, absolute-in-target-period threshold
               1.0 * targetPeriodMs. At TargetPeriodMs = 16.667 the absolute threshold
               (16.667 ms) is crossed by an 18 ms diff whose relative size (18%) alone would
               not cross the 20% relative threshold. */
            GHDecisionMetric metric = GHPerformanceComparison.ExternalDecision[0];
            float[] a = { 100f, 101f, 99f, 100f, 100.5f };
            float[] b = { 118f, 119f, 117f, 118f, 118.5f };

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, false, Resamples, Seed);

            Assert.True(System.Math.Abs(d.Rel) <= 0.20, "rel " + d.Rel + " should be within the relative threshold");
            Assert.Equal(GHPerformanceComparison.VerdictRegression, d.Verdict);
            Assert.True(d.IsRegression);
        }

        [Fact]
        public void Decide_TooFewRuns_UsesTooFewRunsVerdict()
        {
            GHDecisionMetric metric = GHPerformanceComparison.SmoothnessDecision[0];
            float[] a = { 1f, 1.1f };
            float[] b = { 6f, 6.1f };

            GHMetricDecision d = GHPerformanceComparison.Decide(metric, a, b, TargetPeriodMs, true, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.VerdictTooFewRuns, d.Verdict);
            Assert.False(d.IsRegression);
            Assert.False(d.IsImprovement);
        }

        [Fact]
        public void VerdictTooFewRuns_TextNamesTheMinimum()
        {
            Assert.Equal("too few runs (need 3)", GHPerformanceComparison.VerdictTooFewRuns);
        }

        private static GHSmoothnessSummary Summary(double hitch, double pace, double fps)
        {
            return new GHSmoothnessSummary
            {
                HitchRatioMsPerSec = hitch,
                PacingErrorRmsMs = pace,
                DisplayedFps = fps
            };
        }

        private static List<GHSmoothnessSummary> Repeat(int count, double hitch, double pace, double fps)
        {
            List<GHSmoothnessSummary> list = new List<GHSmoothnessSummary>();
            for (int i = 0; i < count; i++)
                list.Add(Summary(hitch + 0.01 * i, pace + 0.01 * i, fps + 0.01 * i));
            return list;
        }

        [Fact]
        public void CompareSmoothness_TwoRunsPerArm_IsTooFewRunsAndNotProvisional()
        {
            List<GHSmoothnessSummary> a = Repeat(2, 1.0, 0.5, 60.0);
            List<GHSmoothnessSummary> b = Repeat(2, 6.0, 0.5, 60.0);

            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            Assert.Equal(2, result.RunsA);
            Assert.Equal(2, result.RunsB);
            Assert.True(result.TooFewRuns);
            Assert.False(result.Provisional);
        }

        [Theory]
        [InlineData(3)]
        [InlineData(4)]
        public void CompareSmoothness_ThreeOrFourRunsPerArm_IsProvisional(int runsPerArm)
        {
            List<GHSmoothnessSummary> a = Repeat(runsPerArm, 1.0, 0.5, 60.0);
            List<GHSmoothnessSummary> b = Repeat(runsPerArm, 6.0, 0.5, 60.0);

            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            Assert.False(result.TooFewRuns);
            Assert.True(result.Provisional);
        }

        [Fact]
        public void CompareSmoothness_FiveRunsPerArm_IsNeitherTooFewNorProvisional()
        {
            List<GHSmoothnessSummary> a = Repeat(5, 1.0, 0.5, 60.0);
            List<GHSmoothnessSummary> b = Repeat(5, 6.0, 0.5, 60.0);

            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            Assert.False(result.TooFewRuns);
            Assert.False(result.Provisional);
        }

        [Fact]
        public void CompareSmoothness_NullEntries_AreSkippedFromTheRunCount()
        {
            List<GHSmoothnessSummary> a = Repeat(5, 1.0, 0.5, 60.0);
            a.Add(null);
            a.Add(null);
            List<GHSmoothnessSummary> b = Repeat(5, 6.0, 0.5, 60.0);
            b.Add(null);

            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            Assert.Equal(5, result.RunsA);
            Assert.Equal(5, result.RunsB);
        }

        [Fact]
        public void CompareSmoothness_DecisionsFollowSmoothnessDecisionOrder_AndTallyMatchesVerdicts()
        {
            /* Hitch ratio: clearly separated (regression). Pacing and FPS: overlapping
               spreads so their bootstrap CIs include zero (no evidence). */
            List<GHSmoothnessSummary> a = new List<GHSmoothnessSummary>
            {
                Summary(1.0, 0.50, 60.0),
                Summary(1.05, 0.55, 60.1),
                Summary(0.95, 0.45, 59.9),
                Summary(1.0, 0.52, 60.05),
                Summary(1.02, 0.48, 59.95)
            };
            List<GHSmoothnessSummary> b = new List<GHSmoothnessSummary>
            {
                Summary(6.0, 0.60, 59.0),
                Summary(6.05, 0.40, 60.5),
                Summary(5.95, 0.65, 58.5),
                Summary(6.0, 0.35, 60.0),
                Summary(6.02, 0.55, 59.8)
            };

            GHComparisonResult result = GHPerformanceComparison.CompareSmoothness("A", a, "B", b, TargetPeriodMs, Resamples, Seed);

            Assert.Equal(GHPerformanceComparison.SmoothnessDecision.Length, result.Decisions.Count);
            for (int i = 0; i < GHPerformanceComparison.SmoothnessDecision.Length; i++)
                Assert.Equal(GHPerformanceComparison.SmoothnessDecision[i].Name, result.Decisions[i].Metric.Name);

            Assert.Equal(GHPerformanceComparison.HitchRatioMsPerSec, result.Decisions[0].Metric.Name);
            Assert.Equal(GHPerformanceComparison.VerdictRegression, result.Decisions[0].Verdict);

            List<string> verdicts = new List<string>();
            foreach (GHMetricDecision d in result.Decisions)
                verdicts.Add(d.Verdict);
            int regressions, improvements;
            GHPerformanceComparison.Tally(verdicts, out regressions, out improvements);
            Assert.Equal(regressions, result.Regressions);
            Assert.Equal(improvements, result.Improvements);
            Assert.Equal(1, result.Regressions);
            Assert.Equal(0, result.Improvements);
        }

        [Fact]
        public void PooledSd_EqualSpreadArms_ReturnsThatSpread()
        {
            float[] a = { 1f, 2f, 3f, 4f, 5f };
            float[] b = { 11f, 12f, 13f, 14f, 15f }; /* shifted, same standard deviation */
            double expected = GHPerformanceStatistics.StdDev(a);

            Assert.Equal(expected, GHPerformanceComparison.PooledSd(a, b), 9);
        }

        [Fact]
        public void PooledSd_TwoValuesTotal_IsZero()
        {
            Assert.Equal(0.0, GHPerformanceComparison.PooledSd(new float[] { 5f }, new float[] { 7f }));
            Assert.Equal(0.0, GHPerformanceComparison.PooledSd(new float[0], new float[] { 1f, 2f }));
        }

        [Fact]
        public void CadenceMismatch_EqualPeriods_IsFalse()
        {
            IList<double> refresh = new List<double> { 16.667, 16.667 };
            IList<double> target = new List<double> { 16.667, 16.667 };

            string detail;
            bool mismatch = GHPerformanceComparison.CadenceMismatch(refresh, target,
                GHPerformanceComparison.CadenceToleranceFraction, out detail);

            Assert.False(mismatch);
            Assert.Null(detail);
        }

        [Fact]
        public void CadenceMismatch_RefreshDiffersByTwoPercent_IsTrueWithRefreshDetail()
        {
            IList<double> refresh = new List<double> { 16.667, 17.0 };
            IList<double> target = new List<double> { 16.667, 16.667 };

            string detail;
            bool mismatch = GHPerformanceComparison.CadenceMismatch(refresh, target,
                GHPerformanceComparison.CadenceToleranceFraction, out detail);

            Assert.True(mismatch);
            Assert.StartsWith("refresh period from", detail);
        }

        [Fact]
        public void CadenceMismatch_RefreshEqualTargetDiffers_IsTrueWithTargetDetail()
        {
            IList<double> refresh = new List<double> { 16.667, 16.667 };
            IList<double> target = new List<double> { 16.667, 17.0 };

            string detail;
            bool mismatch = GHPerformanceComparison.CadenceMismatch(refresh, target,
                GHPerformanceComparison.CadenceToleranceFraction, out detail);

            Assert.True(mismatch);
            Assert.StartsWith("target period from", detail);
        }

        [Fact]
        public void CadenceMismatch_NonPositiveValues_AreIgnored()
        {
            IList<double> refresh = new List<double> { 16.667, -1.0, 0.0, 16.667 };
            IList<double> target = new List<double> { 16.667, 0.0 };

            string detail;
            bool mismatch = GHPerformanceComparison.CadenceMismatch(refresh, target,
                GHPerformanceComparison.CadenceToleranceFraction, out detail);

            Assert.False(mismatch);
        }

        [Fact]
        public void CadenceMismatch_HalfPercentDifference_IsFalse()
        {
            IList<double> refresh = new List<double> { 16.667, 16.75 };
            IList<double> target = new List<double> { 16.667, 16.667 };

            string detail;
            bool mismatch = GHPerformanceComparison.CadenceMismatch(refresh, target,
                GHPerformanceComparison.CadenceToleranceFraction, out detail);

            Assert.False(mismatch);
        }

        [Theory]
        [InlineData(4, 1)]
        [InlineData(1, 5)]
        public void ClassifyThrottle_StatusAboveModerate_IsThrottled(int statusBefore, int statusAfter)
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(statusBefore, statusAfter, double.NaN, double.NaN);

            Assert.True(v.Throttled);
            Assert.Equal(GHThrottleSignal.Status, v.Signal);
            Assert.Equal(GHThrottleRule.StatusAboveModerate, v.Rule);
        }

        [Fact]
        public void ClassifyThrottle_CpuPerformanceDropFromBefore_IsThrottled()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(0, 0, 95.0, 60.0);

            Assert.True(v.Throttled);
            Assert.Equal(GHThrottleSignal.CpuPerformancePct, v.Signal);
            Assert.Equal(GHThrottleRule.CpuPerformanceDrop, v.Rule);
        }

        [Fact]
        public void ClassifyThrottle_CpuPerformanceBelowFloorWithNoBefore_IsThrottled()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(0, 0, double.NaN, 85.0);

            Assert.True(v.Throttled);
            Assert.Equal(GHThrottleSignal.CpuPerformancePct, v.Signal);
            Assert.Equal(GHThrottleRule.CpuPerformanceDrop, v.Rule);
        }

        [Fact]
        public void ClassifyThrottle_CpuPerformanceSmallDrop_IsNotThrottled()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(0, 0, 62.0, 60.0);

            Assert.False(v.Throttled);
            Assert.Equal(GHThrottleSignal.CpuPerformancePct, v.Signal);
        }

        [Fact]
        public void ClassifyThrottle_OnlyBeforeReading_IsNotThrottled()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(0, 0, 60.0, double.NaN);

            Assert.False(v.Throttled);
            Assert.Equal(GHThrottleSignal.CpuPerformancePct, v.Signal);
        }

        [Fact]
        public void ClassifyThrottle_StatusRoseTwoClasses_IsThrottled()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(1, 3, double.NaN, double.NaN);

            Assert.True(v.Throttled);
            Assert.Equal(GHThrottleSignal.Status, v.Signal);
            Assert.Equal(GHThrottleRule.StatusRoseTwoClasses, v.Rule);
        }

        [Fact]
        public void ClassifyThrottle_StatusRoseOneClass_IsNotThrottled()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(1, 2, double.NaN, double.NaN);

            Assert.False(v.Throttled);
            Assert.Equal(GHThrottleSignal.Status, v.Signal);
        }

        [Fact]
        public void ClassifyThrottle_EverythingUnknown_SignalIsNone()
        {
            GHThrottleVerdict v = GHPerformanceComparison.ClassifyThrottle(0, 0, double.NaN, double.NaN);

            Assert.False(v.Throttled);
            Assert.Equal(GHThrottleSignal.None, v.Signal);
            Assert.Equal(GHThrottleRule.None, v.Rule);
        }

        [Fact]
        public void Tally_CountsRegressionsAndImprovements()
        {
            List<string> verdicts = new List<string>
            {
                GHPerformanceComparison.VerdictRegression,
                GHPerformanceComparison.VerdictImprovement,
                GHPerformanceComparison.VerdictNoEvidence,
                GHPerformanceComparison.VerdictRegression,
                GHPerformanceComparison.VerdictWorseUnderThreshold,
                GHPerformanceComparison.VerdictBetterUnderThreshold,
                GHPerformanceComparison.VerdictTooFewRuns
            };

            int regressions, improvements;
            GHPerformanceComparison.Tally(verdicts, out regressions, out improvements);

            Assert.Equal(2, regressions);
            Assert.Equal(1, improvements);
        }
    }
}

using System;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHRefreshRateEstimator: the minimum count, the trimmed mean against hitches, the snap to the
       monitor's rate and the 1 Hz publish rule without one, irregular callbacks off the monitor rate, and
       the median for vsync-paced frames. */
    public class GHRefreshRateEstimatorTests
    {
        private const double Ms60 = 1000.0 / 60.0;

        /* Returns the Published value after each delta that changed it, or NaN when none did */
        private static float Feed(GHRefreshRateEstimator estimator, double deltaMs, int count)
        {
            float last = float.NaN;
            int i;
            for (i = 0; i < count; i++)
            {
                if (estimator.AddDelta(deltaMs))
                    last = estimator.Published;
            }
            return last;
        }

        [Fact]
        public void FewerThanMinimumCount_PublishesNothing()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();

            float published = Feed(estimator, Ms60, GHRefreshRateEstimator.MinimumCount - 1);

            Assert.True(float.IsNaN(published));
            Assert.Equal(0.0f, estimator.Published);
        }

        [Fact]
        public void SteadyFrames_NoMonitorRate_PublishesTheirRate()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();

            Feed(estimator, Ms60, GHRefreshRateEstimator.WindowSize);

            Assert.InRange(estimator.Published, 59.99f, 60.01f);
        }

        [Fact]
        public void SingleHitch_MonitorRate_StaysAtMonitorRate()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            estimator.MonitorRefreshRate = 60.0f;
            int i;
            for (i = 0; i < GHRefreshRateEstimator.WindowSize - 1; i++)
            {
                if (estimator.AddDelta(Ms60))
                    Assert.Equal(60.0f, estimator.Published);
            }

            bool changed = estimator.AddDelta(100.0);

            Assert.False(changed);
            Assert.Equal(60.0f, estimator.Published);
        }

        [Fact]
        public void PublishedJustBelowMonitorRate_SnapsBackWhenMonitorRateKnown()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            Feed(estimator, 1000.0 / 59.1, GHRefreshRateEstimator.WindowSize);
            Assert.InRange(estimator.Published, 59.09f, 59.11f);

            estimator.MonitorRefreshRate = 60.0f;
            bool changed = estimator.AddDelta(Ms60);

            Assert.True(changed);
            Assert.Equal(60.0f, estimator.Published);
        }

        [Fact]
        public void PublishedJustBelow_NoMonitorRate_KeepsOneHertzRule()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            Feed(estimator, 1000.0 / 59.1, GHRefreshRateEstimator.WindowSize);

            float published = Feed(estimator, Ms60, GHRefreshRateEstimator.WindowSize);

            Assert.True(float.IsNaN(published));
            Assert.InRange(estimator.Published, 59.09f, 59.11f);
        }

        [Fact]
        public void IrregularCallbacksOffMonitorRate_PublishTheirOwnRate()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            estimator.MonitorRefreshRate = 60.0f;
            int i;
            for (i = 0; i < GHRefreshRateEstimator.WindowSize; i++)
                estimator.AddDelta(i % 4 == 3 ? 25.0 : 8.3);

            /* 90 x 8.3 ms and 30 x 25 ms, the 6 largest dropped: 1347 ms / 114 = 11.82 ms */
            Assert.InRange(estimator.Published, 84.0f, 85.0f);
        }

        [Fact]
        public void HighRefreshMonitor_SnapsToItsRate()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            estimator.MonitorRefreshRate = 144.0f;

            Feed(estimator, 1000.0 / 143.2, GHRefreshRateEstimator.WindowSize);

            Assert.Equal(144.0f, estimator.Published);
        }

        [Fact]
        public void VsyncPaced_UsesMedian()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            estimator.VsyncPaced = true;

            Feed(estimator, Ms60, GHRefreshRateEstimator.WindowSize - 1);
            estimator.AddDelta(500.0);

            Assert.InRange(estimator.Published, 59.99f, 60.01f);
        }

        [Fact]
        public void HitchesBeyondTrim_DropBelowSnap_ThenRecoverToMonitorRate()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            estimator.MonitorRefreshRate = 60.0f;
            int i;
            for (i = 0; i < GHRefreshRateEstimator.WindowSize; i++)
                estimator.AddDelta(i < 8 ? 100.0 : Ms60);

            /* 112 x 16.67 ms and 2 x 100 ms kept: 18.13 ms, about 55 Hz, outside the snap */
            Assert.True(estimator.Published < 57.0f);

            Feed(estimator, Ms60, GHRefreshRateEstimator.WindowSize);

            Assert.Equal(60.0f, estimator.Published);
        }

        [Fact]
        public void Reset_ClearsPublishedValue()
        {
            GHRefreshRateEstimator estimator = new GHRefreshRateEstimator();
            Feed(estimator, Ms60, GHRefreshRateEstimator.WindowSize);

            estimator.Reset();

            Assert.Equal(0.0f, estimator.Published);
            Assert.True(float.IsNaN(Feed(estimator, Ms60, GHRefreshRateEstimator.MinimumCount - 1)));
        }
    }
}

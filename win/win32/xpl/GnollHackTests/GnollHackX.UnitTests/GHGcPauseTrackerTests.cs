using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGcPauseTracker: per-interval figures from cumulative counters, unknown and
       re-baselined series, the bridge-pass count, the window and the ring. */
    public class GHGcPauseTrackerTests
    {
        private const long T0 = 638700000000000000L;

        private static long At(long seconds)
        {
            return T0 + seconds * TimeSpan.TicksPerSecond;
        }

        private static void Window(GHGcPauseTracker tracker, long nowSeconds, out long net, out long java, out long javaCount, out long bridgeCount)
        {
            tracker.GetWindow(At(nowSeconds), out net, out java, out javaCount, out bridgeCount);
        }

        [Fact]
        public void FirstSample_IsBaselineOnly()
        {
            long net, java, javaCount, bridgeCount;
            GHGcPauseTracker unknown = new GHGcPauseTracker();
            unknown.AddSample(At(0), -1, -1, -1, -1, -1, -1);
            Assert.Equal(-1, unknown.LastIntervalSeconds);
            Window(unknown, 0, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(new long[] { -1, -1, -1, -1 }, new long[] { net, java, javaCount, bridgeCount });

            GHGcPauseTracker known = new GHGcPauseTracker();
            known.AddSample(At(0), 5, 100, 20, 30, 900, 4);
            Assert.Equal(-1, known.LastIntervalSeconds);
            Assert.Equal(-1, known.LastIntervalNetLongestMs);
            Window(known, 0, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(new long[] { -1, -1, 0, 0 }, new long[] { net, java, javaCount, bridgeCount });
        }

        [Fact]
        public void OneCollectionPerInterval_IsExact()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), 10, 1000, -1, 50, 5000, 7);
            tracker.AddSample(At(30), 11, 1250, -1, 51, 5900, 7);

            Assert.Equal(30, tracker.LastIntervalSeconds);
            Assert.Equal(250, tracker.LastIntervalNetLongestMs);
            Assert.Equal(900, tracker.LastIntervalJavaLongestMs);
            Assert.Equal(1, tracker.LastIntervalJavaCount);
            long net, java, javaCount, bridgeCount;
            Window(tracker, 30, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(250, net);
            Assert.Equal(900, java);
            Assert.Equal(1, javaCount);
        }

        [Fact]
        public void SeveralCollections_GiveTheAverage()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 50, 5000, -1);
            tracker.AddSample(At(60), -1, -1, -1, 54, 7000, -1);

            Assert.Equal(500, tracker.LastIntervalJavaLongestMs);
            Assert.Equal(4, tracker.LastIntervalJavaCount);
        }

        [Fact]
        public void NewestPause_BeatsTheAverage()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), 10, 1000, -1, -1, -1, -1);
            tracker.AddSample(At(60), 13, 3100, 1800, -1, -1, -1);

            Assert.Equal(1800, tracker.LastIntervalNetLongestMs);
        }

        [Fact]
        public void UnknownSeries_StayUnknown()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 50, 5000, -1);
            tracker.AddSample(At(60), -1, -1, -1, 52, 5600, -1);

            Assert.Equal(-1, tracker.LastIntervalNetLongestMs);
            Assert.Equal(300, tracker.LastIntervalJavaLongestMs);
            Assert.Equal(2, tracker.LastIntervalJavaCount);
        }

        [Fact]
        public void CounterGoingDown_Rebaselines()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 50, 5000, -1);
            tracker.AddSample(At(60), -1, -1, -1, 10, 1000, -1);
            Assert.Equal(-1, tracker.LastIntervalJavaCount);
            Assert.Equal(-1, tracker.LastIntervalJavaLongestMs);

            tracker.AddSample(At(120), -1, -1, -1, 12, 1400, -1);
            Assert.Equal(2, tracker.LastIntervalJavaCount);
            Assert.Equal(200, tracker.LastIntervalJavaLongestMs);
        }

        [Fact]
        public void BridgeGeneration_IsCounted()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), 10, 1000, -1, 50, 5000, 10);
            tracker.AddSample(At(60), 10, 1000, -1, 50, 5000, 13);

            Assert.Equal(3, tracker.LastIntervalBridgeCount);
            Assert.Equal(-1, tracker.LastIntervalNetLongestMs);
            Assert.Equal(0, tracker.LastIntervalJavaCount);
            long net, java, javaCount, bridgeCount;
            Window(tracker, 60, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(3, bridgeCount);
            Assert.Equal(0, javaCount);
        }

        [Fact]
        public void BridgeUnknown_StaysUnknown()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 50, 5000, -1);
            tracker.AddSample(At(60), -1, -1, -1, 51, 5400, -1);

            Assert.Equal(-1, tracker.LastIntervalBridgeCount);
            long net, java, javaCount, bridgeCount;
            Window(tracker, 60, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(-1, bridgeCount);
            Assert.Equal(400, java);
            Assert.Equal(1, javaCount);
        }

        [Fact]
        public void Window_DropsOldIntervals()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 50, 5000, -1);
            tracker.AddSample(At(100), -1, -1, -1, 51, 5700, -1);
            tracker.AddSample(At(200), -1, -1, -1, 52, 5900, -1);

            long net, java, javaCount, bridgeCount;
            Window(tracker, 701, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(200, java);
            Assert.Equal(1, javaCount);

            Window(tracker, 699, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(700, java);
            Assert.Equal(2, javaCount);
        }

        [Fact]
        public void Ring_DropsTheOldest()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 0, 0, -1);
            tracker.AddSample(At(1), -1, -1, -1, 1, 5000, -1);
            int i;
            for (i = 0; i < GHGcPauseTracker.MaxIntervals; i++)
                tracker.AddSample(At(2 + i), -1, -1, -1, 2 + i, 5000 + 10 * (i + 1), -1);

            long net, java, javaCount, bridgeCount;
            Window(tracker, 2 + GHGcPauseTracker.MaxIntervals, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(10, java);
            Assert.Equal(GHGcPauseTracker.MaxIntervals, javaCount);
        }

        [Fact]
        public void QuietInterval_IsNotStored()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), 10, 1000, -1, 50, 5000, 7);
            tracker.AddSample(At(60), 10, 1000, -1, 50, 5000, 7);

            Assert.Equal(0, tracker.LastIntervalJavaCount);
            Assert.Equal(0, tracker.LastIntervalBridgeCount);
            Assert.Equal(-1, tracker.LastIntervalNetLongestMs);
            long net, java, javaCount, bridgeCount;
            Window(tracker, 60, out net, out java, out javaCount, out bridgeCount);
            Assert.Equal(new long[] { -1, -1, 0, 0 }, new long[] { net, java, javaCount, bridgeCount });
        }

        [Fact]
        public void LongInterval_IsFlagged()
        {
            GHGcPauseTracker tracker = new GHGcPauseTracker();
            tracker.AddSample(At(0), -1, -1, -1, 50, 5000, -1);
            tracker.AddSample(At(60), -1, -1, -1, 51, 6000, -1);
            Assert.True(tracker.LastIntervalIsLong);

            tracker.AddSample(At(120), -1, -1, -1, 52, 6999, -1);
            Assert.False(tracker.LastIntervalIsLong);
        }
    }
}

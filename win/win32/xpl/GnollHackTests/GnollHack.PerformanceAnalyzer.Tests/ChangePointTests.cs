using GnollHack.PerformanceAnalyzer.Commands;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHackX.Performance;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class ChangePointTests
    {
        /* A displayed-frame stream at the given capture times, steady pacing (no pacing
           error), for feeding ChangePoints.Detect directly without a frame timeline CSV */
        private static GHDisplayedFrame[] DisplayedAt(TimelineClock clock, IEnumerable<double> atMs)
        {
            List<GHDisplayedFrame> list = new List<GHDisplayedFrame>();
            long prevTicks = 0;
            bool first = true;
            long frameId = 1;
            foreach (double ms in atMs)
            {
                long ticks = clock.MsToTicks(ms);
                list.Add(new GHDisplayedFrame
                {
                    FrameId = frameId++,
                    DisplayedAtTicks = ticks,
                    GapTicks = first ? 0 : ticks - prevTicks,
                    PacingErrorTicks = 0,
                    Source = GHPresentSource.Estimated
                });
                prevTicks = ticks;
                first = false;
            }
            return list.ToArray();
        }

        private static CapturedTimeline TimelineOf(TimelineClock clock, List<GHFrameRecord> records)
        {
            return new CapturedTimeline { Clock = clock, Records = records.ToArray(), Count = records.Count };
        }

        private static double Gaussian(Random rng)
        {
            double u1 = 1.0 - rng.NextDouble();
            double u2 = rng.NextDouble();
            return Math.Sqrt(-2.0 * Math.Log(u1)) * Math.Cos(2.0 * Math.PI * u2);
        }

        private static double[] Steps(int seed, double sigma, params (int Length, double Level)[] segments)
        {
            Random rng = new Random(seed);
            List<double> y = new List<double>();
            foreach ((int length, double level) in segments)
            {
                for (int i = 0; i < length; i++)
                    y.Add(level + sigma * Gaussian(rng));
            }
            return y.ToArray();
        }

        private static double FpsPenalty(double[] y)
        {
            double floor = Math.Max(ChangePoints.FpsSigmaFloorQuantization,
                ChangePoints.FpsSigmaFloorRelative * Math.Abs(ChangePoints.Median(y)));
            return ChangePoints.DefaultPenalty(y, floor);
        }

        /* Optimal partitioning without pruning, the reference PELT must reproduce */
        private static int[] Exhaustive(double[] y, double penalty, int len)
        {
            int n = y.Length;
            if (n < 2 * len)
                return new int[0];
            double Cost(int a, int b)
            {
                double mean = 0;
                for (int i = a; i < b; i++)
                    mean += y[i];
                mean /= b - a;
                double c = 0;
                for (int i = a; i < b; i++)
                    c += (y[i] - mean) * (y[i] - mean);
                return c;
            }
            double[] f = new double[n + 1];
            int[] last = new int[n + 1];
            f[0] = -penalty;
            for (int t = 1; t <= n; t++)
                f[t] = double.PositiveInfinity;
            for (int t = len; t <= n; t++)
            {
                for (int tau = 0; tau <= t - len; tau++)
                {
                    if (tau != 0 && tau < len)
                        continue;
                    double v = f[tau] + Cost(tau, t) + penalty;
                    if (v < f[t])
                    {
                        f[t] = v;
                        last[t] = tau;
                    }
                }
            }
            List<int> cps = new List<int>();
            for (int k = n; k > 0; k = last[k])
            {
                if (last[k] > 0)
                    cps.Add(last[k]);
            }
            cps.Reverse();
            return cps.ToArray();
        }

        [Fact]
        public void Pelt_StepFrom60To30_FindsOneChangeNearBucket20()
        {
            double[] y = Steps(12345, 1.0, (20, 60.0), (20, 30.0));
            int[] cps = ChangePoints.Pelt(y, FpsPenalty(y), ChangePoints.MinSegmentBuckets);
            int cp = Assert.Single(cps);
            Assert.InRange(cp, 19, 21);
        }

        [Fact]
        public void Pelt_SteadySeries_FindsNothing()
        {
            double[] y = Steps(777, 1.0, (60, 60.0));
            Assert.Empty(ChangePoints.Pelt(y, FpsPenalty(y), ChangePoints.MinSegmentBuckets));
        }

        [Fact]
        public void Pelt_RespectsTheMinimumSegmentLength()
        {
            /* A three-bucket dip is shorter than the minimum segment */
            double[] y = Steps(5, 0.0, (20, 60.0), (3, 0.0), (20, 60.0));
            int[] cps = ChangePoints.Pelt(y, 1.0, ChangePoints.MinSegmentBuckets);
            int[] bounds = new[] { 0 }.Concat(cps).Concat(new[] { y.Length }).ToArray();
            for (int i = 1; i < bounds.Length; i++)
                Assert.True(bounds[i] - bounds[i - 1] >= ChangePoints.MinSegmentBuckets);
        }

        [Theory]
        [InlineData(1)]
        [InlineData(2)]
        [InlineData(3)]
        [InlineData(4)]
        public void Pelt_EqualsExhaustiveOptimalPartitioning(int seed)
        {
            double[] y = Steps(seed, 2.0, (13, 60.0), (9, 45.0), (17, 58.0), (6, 30.0), (15, 60.0));
            double penalty = ChangePoints.DefaultPenalty(y, 2.0);
            Assert.Equal(Exhaustive(y, penalty, ChangePoints.MinSegmentBuckets), ChangePoints.Pelt(y, penalty, ChangePoints.MinSegmentBuckets));
        }

        [Fact]
        public void RobustSigma_IgnoresALevelShift()
        {
            double[] y = Steps(99, 1.0, (200, 60.0), (200, 30.0));
            Assert.InRange(ChangePoints.RobustSigma(y), 0.8, 1.2);
        }

        [Fact]
        public void Detect_Run1_IsTooShortForChangePoints()
        {
            SmoothnessResult r = SmoothnessCommand.Analyze(TestPaths.Run1Json, null, null, null);
            Assert.Equal(2, r.Changes.Series.Count);
            Assert.Empty(r.Changes.Boundaries);
            ChangePoints.TimelineEvent gc = Assert.Single(r.Changes.Events);
            Assert.Equal("gc", gc.Kind);
            Assert.Equal(31.0 * 1000.0 / 60.0, gc.AtMs, 2);
        }

        [Fact]
        public void Detect_FpsStepWithATargetChangeEvent_FindsABoundaryThatListsTheEvent()
        {
            TimelineClock clock = new TimelineClock(10_000_000, null);
            List<double> atMs = new List<double>();
            for (int i = 0; i < 120; i++)
                atMs.Add(i * 25.0);                    /* 40 fps for 3000 ms: 12 buckets */
            for (int i = 0; i < 120; i++)
                atMs.Add(3000.0 + i * 50.0);            /* 20 fps for 6000 ms */
            GHDisplayedFrame[] displayed = DisplayedAt(clock, atMs);

            List<GHFrameRecord> records = new List<GHFrameRecord>
            {
                new GHFrameRecord { CallbackStartTicks = clock.MsToTicks(2990.0), TargetFps = 40, GcCount0 = 0 },
                new GHFrameRecord { CallbackStartTicks = clock.MsToTicks(3000.0), TargetFps = 20, GcCount0 = 1 }
            };
            CapturedTimeline t = TimelineOf(clock, records);

            ChangePoints.Result r = ChangePoints.Detect(t, displayed, displayed.Length);

            ChangePoints.Boundary b = Assert.Single(r.Boundaries);
            Assert.InRange(b.Bucket, 8, 16);
            Assert.True(b.InFps);
            Assert.Contains(b.Events, e => e.Kind == "target");
            Assert.Contains(b.Events, e => e.Kind == "gc");
        }

        [Fact]
        public void Detect_PauseInTheMiddle_IsAMissingBucketNotAChangePoint()
        {
            TimelineClock clock = new TimelineClock(10_000_000, null);
            List<double> atMs = new List<double>();
            for (int i = 0; i < 80; i++)
                atMs.Add(i * 25.0);                     /* 40 fps, 0-2000 ms: 8 buckets */
            /* no frames displayed 2000-3000 ms: the game is not rendering the map */
            for (int i = 0; i < 80; i++)
                atMs.Add(3000.0 + i * 25.0);             /* 40 fps resumes, 3000-5000 ms */
            GHDisplayedFrame[] displayed = DisplayedAt(clock, atMs);

            List<GHFrameRecord> records = new List<GHFrameRecord>();
            for (double ms = 2000.0; ms < 3000.0; ms += 25.0)
            {
                records.Add(new GHFrameRecord
                {
                    CallbackStartTicks = clock.MsToTicks(ms),
                    Pacing = GHPacingDecision.AuxiliaryCanvas
                });
            }
            CapturedTimeline t = TimelineOf(clock, records);

            ChangePoints.Result r = ChangePoints.Detect(t, displayed, displayed.Length);

            Assert.True(r.Series.Missing[8]);
            Assert.True(r.Series.Missing[11]);
            Assert.False(r.Series.Missing[0]);
            Assert.False(r.Series.Missing[12]);
            Assert.Empty(r.Boundaries);
        }
    }
}

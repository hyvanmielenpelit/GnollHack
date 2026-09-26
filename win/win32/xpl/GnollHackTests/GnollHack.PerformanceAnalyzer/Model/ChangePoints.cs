using System.Globalization;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* Offline change-point detection over the displayed-frame stream, and the timeline
       events that explain a change.

       The stream is cut into 250 ms buckets from capture time 0; only whole buckets up to
       the last displayed frame are kept. Two series are built per bucket: displayed FPS
       (displayed frames in the bucket times 4) and the RMS pacing error of the displayed
       frames in the bucket whose gap is judged (not the first frame, not a paused gap), 0
       when there is none. Each series is segmented by PELT under a Gaussian mean-change
       cost, and the boundaries of both are merged. */
    public static class ChangePoints
    {
        public const double BucketMs = 250.0;
        public const int MinSegmentBuckets = 4;
        public const double EventWindowMs = 500.0;

        /* The smallest noise levels the penalty assumes. A steady stream makes the MAD of
           the first differences zero, and a zero penalty would split on every wobble: FPS
           is quantized to steps of 4 (one frame per bucket), whose rounding noise has a
           standard deviation of 4 / sqrt(12), and a single late frame in an otherwise
           steady run should not open a segment of its own, so the FPS floor is also 5 %
           of the median level; the pacing floor is 0.5 ms. */
        public const double FpsSigmaFloorRelative = 0.05;
        public static readonly double FpsSigmaFloorQuantization = 4.0 / Math.Sqrt(12.0);
        public const double PacingSigmaFloorMs = 0.5;

        /* PELT, Pruned Exact Linear Time (Killick, Fearnhead and Eckley 2012), with a
           minimum segment length L. For y[0..n) with prefix sums, the optimal cost of
           y[0..t) is

             F(0) = -beta
             F(t) = min over tau in R(t) with t - tau >= L of  F(tau) + C(tau, t) + beta

           where C(a, b) is the cost of the segment y[a..b) and beta the penalty per
           change point. The admissible last change points are tau = 0 and L <= tau <= t - L;
           tau = t - L joins the candidate set R when step t reaches it. Pruning: the
           Gaussian cost satisfies C(a, b) >= C(a, s) + C(s, b), so once
           F(tau) + C(tau, t) > F(t) for some t, tau can never beat t as the last change
           point of any T >= t + L (for which t is admissible). Such tau is removed from R
           at step t + L, not at t, which keeps the search exact under the minimum length.
           The segmentation is recovered by following the argmin back from n.

           Returns the start indices of every segment after the first, ascending. */
        public static int[] Pelt(double[] y, double penalty, int minSegment)
        {
            int n = y.Length;
            int len = Math.Max(1, minSegment);
            if (n < 2 * len)
                return new int[0];
            double[] s1 = new double[n + 1];
            double[] s2 = new double[n + 1];
            for (int i = 0; i < n; i++)
            {
                s1[i + 1] = s1[i] + y[i];
                s2[i + 1] = s2[i] + y[i] * y[i];
            }
            double Cost(int a, int b)
            {
                double m = b - a;
                double sum = s1[b] - s1[a];
                double c = s2[b] - s2[a] - sum * sum / m;
                return c > 0 ? c : 0;
            }

            double[] f = new double[n + 1];
            int[] last = new int[n + 1];
            for (int t = 1; t <= n; t++)
                f[t] = double.PositiveInfinity;
            f[0] = -penalty;

            List<int> candidates = new List<int> { 0 };
            /* candidate -> step from which it is pruned */
            Dictionary<int, int> pruneAt = new Dictionary<int, int>();
            for (int t = len; t <= n; t++)
            {
                int fresh = t - len;
                if (fresh >= len)
                    candidates.Add(fresh);
                candidates.RemoveAll(tau => pruneAt.TryGetValue(tau, out int at) && at <= t);

                double best = double.PositiveInfinity;
                int bestTau = 0;
                foreach (int tau in candidates)
                {
                    if (double.IsPositiveInfinity(f[tau]))
                        continue;
                    double v = f[tau] + Cost(tau, t) + penalty;
                    if (v < best)
                    {
                        best = v;
                        bestTau = tau;
                    }
                }
                f[t] = best;
                last[t] = bestTau;

                foreach (int tau in candidates)
                {
                    if (!pruneAt.ContainsKey(tau) && !double.IsPositiveInfinity(f[tau])
                        && f[tau] + Cost(tau, t) > f[t])
                        pruneAt[tau] = t + len;
                }
            }

            List<int> cps = new List<int>();
            int k = n;
            while (k > 0)
            {
                int tau = last[k];
                if (tau > 0)
                    cps.Add(tau);
                k = tau;
            }
            cps.Reverse();
            return cps.ToArray();
        }

        /* sigma = MAD(first differences) / 0.6745 / sqrt(2): the MAD scaled to a Gaussian
           standard deviation, and the differences of independent noise have twice the
           variance of the noise, so a level shift barely moves the estimate */
        public static double RobustSigma(double[] y)
        {
            if (y.Length < 3)
                return 0;
            double[] d = new double[y.Length - 1];
            for (int i = 0; i < d.Length; i++)
                d[i] = y[i + 1] - y[i];
            double med = Median(d);
            double[] dev = d.Select(v => Math.Abs(v - med)).ToArray();
            return Median(dev) / 0.6745 / Math.Sqrt(2.0);
        }

        /* 2 sigma^2 ln(n), sigma being the robust estimate or the floor, whichever is larger */
        public static double DefaultPenalty(double[] y, double sigmaFloor)
        {
            if (y.Length < 2)
                return 0;
            double sigma = Math.Max(RobustSigma(y), sigmaFloor);
            return 2.0 * sigma * sigma * Math.Log(y.Length);
        }

        public static double Median(double[] v)
        {
            if (v.Length == 0)
                return 0;
            double[] s = (double[])v.Clone();
            Array.Sort(s);
            int m = s.Length / 2;
            return s.Length % 2 == 1 ? s[m] : (s[m - 1] + s[m]) / 2.0;
        }

        public sealed class Buckets
        {
            public double[] DisplayedFps = new double[0];
            public double[] PacingErrorRmsMs = new double[0];
            public int Count { get { return DisplayedFps.Length; } }
        }

        public static Buckets Bucket(CapturedTimeline t, GHDisplayedFrame[] displayed, int displayedCount)
        {
            Buckets b = new Buckets();
            if (displayedCount <= 0)
                return b;
            double lastMs = t.Clock.TicksToMs(displayed[displayedCount - 1].DisplayedAtTicks);
            int n = (int)Math.Floor(lastMs / BucketMs);
            if (n <= 0)
                return b;
            int[] frames = new int[n];
            double[] errSq = new double[n];
            int[] judged = new int[n];
            for (int j = 0; j < displayedCount; j++)
            {
                double ms = t.Clock.TicksToMs(displayed[j].DisplayedAtTicks);
                int k = (int)Math.Floor(ms / BucketMs);
                if (k < 0 || k >= n)
                    continue;
                frames[k]++;
                if (j > 0 && !displayed[j].IsPausedGap)
                {
                    double e = t.Clock.DurationTicksToMs(displayed[j].PacingErrorTicks);
                    errSq[k] += e * e;
                    judged[k]++;
                }
            }
            b.DisplayedFps = new double[n];
            b.PacingErrorRmsMs = new double[n];
            for (int k = 0; k < n; k++)
            {
                b.DisplayedFps[k] = frames[k] * (1000.0 / BucketMs);
                b.PacingErrorRmsMs[k] = judged[k] > 0 ? Math.Sqrt(errSq[k] / judged[k]) : 0;
            }
            return b;
        }

        public sealed class TimelineEvent
        {
            public double AtMs;
            public string Kind;
            public string Text;
        }

        public sealed class Segment
        {
            public int StartBucket;
            public int EndBucket;               /* exclusive */
            public double StartS { get { return StartBucket * BucketMs / 1000.0; } }
            public double EndS { get { return EndBucket * BucketMs / 1000.0; } }
            public double MeanDisplayedFps;
            public double MeanPacingErrorMs;
        }

        public sealed class Boundary
        {
            public int Bucket;
            public double AtS { get { return Bucket * BucketMs / 1000.0; } }
            public bool InFps;
            public bool InPacing;
            public List<TimelineEvent> Events = new List<TimelineEvent>();
        }

        public sealed class Result
        {
            public Buckets Series;
            public double FpsPenalty;
            public double PacingPenalty;
            public List<Segment> Segments = new List<Segment>();
            public List<Boundary> Boundaries = new List<Boundary>();
            public List<TimelineEvent> Events = new List<TimelineEvent>();
        }

        public static Result Detect(CapturedTimeline t, GHDisplayedFrame[] displayed, int displayedCount)
        {
            Result res = new Result();
            res.Series = Bucket(t, displayed, displayedCount);
            res.Events = Events(t);
            int n = res.Series.Count;
            if (n == 0)
                return res;

            double fpsFloor = Math.Max(FpsSigmaFloorQuantization, FpsSigmaFloorRelative * Math.Abs(Median(res.Series.DisplayedFps)));
            res.FpsPenalty = DefaultPenalty(res.Series.DisplayedFps, fpsFloor);
            res.PacingPenalty = DefaultPenalty(res.Series.PacingErrorRmsMs, PacingSigmaFloorMs);
            int[] fpsCps = Pelt(res.Series.DisplayedFps, res.FpsPenalty, MinSegmentBuckets);
            int[] pacingCps = Pelt(res.Series.PacingErrorRmsMs, res.PacingPenalty, MinSegmentBuckets);

            /* Boundaries of the two series within a bucket of each other are one change */
            SortedDictionary<int, Boundary> merged = new SortedDictionary<int, Boundary>();
            foreach (int cp in fpsCps)
                merged[cp] = new Boundary { Bucket = cp, InFps = true };
            foreach (int cp in pacingCps)
            {
                Boundary near = merged.Values.FirstOrDefault(x => Math.Abs(x.Bucket - cp) <= 1);
                if (near != null)
                    near.InPacing = true;
                else
                    merged[cp] = new Boundary { Bucket = cp, InPacing = true };
            }
            res.Boundaries = merged.Values.ToList();
            foreach (Boundary b in res.Boundaries)
            {
                double at = b.Bucket * BucketMs;
                b.Events = res.Events.Where(e => Math.Abs(e.AtMs - at) <= EventWindowMs).ToList();
            }

            int start = 0;
            foreach (int end in res.Boundaries.Select(x => x.Bucket).Concat(new[] { n }))
            {
                if (end <= start)
                    continue;
                Segment s = new Segment { StartBucket = start, EndBucket = end };
                s.MeanDisplayedFps = res.Series.DisplayedFps.Skip(start).Take(end - start).Average();
                s.MeanPacingErrorMs = res.Series.PacingErrorRmsMs.Skip(start).Take(end - start).Average();
                res.Segments.Add(s);
                start = end;
            }
            return res;
        }

        private static bool IsCanvasPause(ref GHFrameRecord r)
        {
            return r.Pacing == GHPacingDecision.AuxiliaryCanvas || r.Paint == GHPaintOutcome.OverlayVisible
                || r.Pacing == GHPacingDecision.Suspended;
        }

        /* Timeline events that can explain a change: a measured refresh period moving by
           more than 5 % from the last reported one, a change of the map's target FPS, the
           start and end of a canvas pause (an auxiliary canvas, an overlay covering the
           map, or suspension), and a collection (the GC counters advancing between two
           ticks). Stamped at the tick's callback start. */
        public static List<TimelineEvent> Events(CapturedTimeline t)
        {
            List<TimelineEvent> ev = new List<TimelineEvent>();
            long reportedPeriod = 0;
            int lastTarget = 0;
            bool inPause = false;
            string pauseKind = null;
            for (int i = 0; i < t.Count; i++)
            {
                GHFrameRecord r = t.Records[i];
                if (r.CallbackStartTicks == 0)
                    continue;
                double at = t.Clock.TicksToMs(r.CallbackStartTicks);
                long p = r.RefreshPeriodTicks;
                if (p > 0)
                {
                    if (reportedPeriod == 0)
                    {
                        reportedPeriod = p;
                    }
                    else if (Math.Abs(p - reportedPeriod) > reportedPeriod / 20)
                    {
                        ev.Add(new TimelineEvent
                        {
                            AtMs = at,
                            Kind = "refresh",
                            Text = "refresh period " + F(t.Clock.DurationTicksToMs(reportedPeriod)) + " -> " + F(t.Clock.DurationTicksToMs(p)) + " ms"
                        });
                        reportedPeriod = p;
                    }
                }
                if (r.TargetFps > 0)
                {
                    if (lastTarget > 0 && r.TargetFps != lastTarget)
                        ev.Add(new TimelineEvent { AtMs = at, Kind = "target", Text = "target FPS " + lastTarget + " -> " + r.TargetFps });
                    lastTarget = r.TargetFps;
                }
                bool pause = IsCanvasPause(ref r);
                if (pause != inPause)
                {
                    string kind = pause
                        ? (r.Pacing == GHPacingDecision.AuxiliaryCanvas ? "auxiliary canvas" : r.Pacing == GHPacingDecision.Suspended ? "suspended" : "overlay visible")
                        : pauseKind;
                    ev.Add(new TimelineEvent { AtMs = at, Kind = "pause", Text = "canvas pause " + (pause ? "begins" : "ends") + " (" + kind + ")" });
                    inPause = pause;
                    pauseKind = kind;
                }
                if (i > 0)
                {
                    GHFrameRecord q = t.Records[i - 1];
                    string gen = r.GcCount2 != q.GcCount2 ? "2" : r.GcCount1 != q.GcCount1 ? "1" : r.GcCount0 != q.GcCount0 ? "0" : null;
                    if (gen != null)
                        ev.Add(new TimelineEvent { AtMs = at, Kind = "gc", Text = "GC (gen " + gen + ")" });
                }
            }
            return ev;
        }

        private static string F(double v)
        {
            return v.ToString("0.00", CultureInfo.InvariantCulture);
        }
    }
}

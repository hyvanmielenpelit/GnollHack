using BenchmarkDotNet.Attributes;
using GnollHackX.Perf;

namespace GnollHackX.Benchmarks
{
    /* Percentile summaries of one 30 s window at 60 Hz, the size the dashboard timer
       summarised every 0.5 s: a sorted copy with three nearest-rank lookups against
       three lookups on a pre-filled streaming histogram. */
    [MemoryDiagnoser]
    public class StatisticsBenchmarks
    {
        private const int SampleSize = 1800;

        private float[] _intervals;
        private GHPerfStats.StreamingHistogram _histogram;

        [GlobalSetup]
        public void Setup()
        {
            GHPerfStats.SeededRandom rng = new GHPerfStats.SeededRandom(42);
            _intervals = new float[SampleSize];
            _histogram = new GHPerfStats.StreamingHistogram();
            int i;
            for (i = 0; i < SampleSize; i++)
            {
                /* 16.667 ms with 0..2 ms of jitter and a 3 percent tail of up to 120 ms extra */
                float ms = 16.667f + rng.Next(2000) / 1000f;
                if (rng.Next(100) < 3)
                    ms += rng.Next(120000) / 1000f;
                _intervals[i] = ms;
                _histogram.Add(ms);
            }
        }

        [Benchmark(Baseline = true)]
        public float SortedPercentiles()
        {
            float[] sorted = GHPerfStats.SortedCopy(_intervals);
            int n = sorted.Length;
            return GHPerfStats.PercentileSorted(sorted, n, 50)
                 + GHPerfStats.PercentileSorted(sorted, n, 95)
                 + GHPerfStats.PercentileSorted(sorted, n, 99);
        }

        [Benchmark]
        public float StreamingPercentiles()
        {
            return _histogram.Percentile(50)
                 + _histogram.Percentile(95)
                 + _histogram.Percentile(99);
        }
    }
}

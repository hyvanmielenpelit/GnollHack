using GnollHackX.Perf;

namespace GnollHack.PerfAnalyzer.Model
{
    /* Names are the record's metric keys; they follow the Android Macrobenchmark and
       Apple hitch conventions so that the numbers are recognisable outside this project. */
    public static class MetricNames
    {
        public const string FrameDurationP50 = "frameDurationP50";
        public const string FrameDurationP90 = "frameDurationP90";
        public const string FrameDurationP95 = "frameDurationP95";
        public const string FrameDurationP99 = "frameDurationP99";
        public const string FrameDurationMax = "frameDurationMax";
        public const string FrameDurationMean = "frameDurationMean";
        public const string FrameDurationStdDev = "frameDurationStdDev";
        public const string FpsMean = "fpsMean";
        public const string Fps1PctLow = "fps1PctLow";
        public const string Fps01PctLow = "fps01PctLow";
        public const string FrameOverrunP50 = "frameOverrunP50";
        public const string FrameOverrunP95 = "frameOverrunP95";
        public const string FrameOverrunP99 = "frameOverrunP99";
        public const string JankPct = "jankPct";
        public const string HitchRatio = "hitchRatio";
        public const string Hitch2xPerMin = "hitch2xPerMin";
        public const string Hitch4xPerMin = "hitch4xPerMin";
        public const string StutterIndex = "stutterIndex";
        public const string FrameCount = "frameCount";
        public const string WindowSeconds = "windowSeconds";

        /* The metrics the comparison decides on, with the direction in which "worse" lies
           and the plan's pre-registered regression thresholds (section 2.2). */
        public static readonly DecisionMetric[] Decision =
        {
            new DecisionMetric(FrameDurationP99, "ms", higherIsWorse: true, relativeThreshold: 0.20, absoluteThresholdVsync: 1.0),
            new DecisionMetric(HitchRatio, "ms/s", higherIsWorse: true, relativeThreshold: 0, absoluteThreshold: 2.0),
            new DecisionMetric(Fps1PctLow, "fps", higherIsWorse: false, relativeThreshold: 0.10),
        };

        /* Reported alongside, not decided on. */
        public static readonly string[] Reported =
        {
            FrameDurationP50, FrameDurationP95, FrameDurationMax, FpsMean, Fps01PctLow,
            JankPct, Hitch2xPerMin, Hitch4xPerMin, StutterIndex, FrameOverrunP99, FrameCount, WindowSeconds
        };
    }

    public sealed class DecisionMetric
    {
        public string Name { get; }
        public string Unit { get; }
        public bool HigherIsWorse { get; }
        public double RelativeThreshold { get; }
        public double AbsoluteThreshold { get; }
        public double AbsoluteThresholdVsync { get; }

        public DecisionMetric(string name, string unit, bool higherIsWorse, double relativeThreshold,
                              double absoluteThreshold = 0, double absoluteThresholdVsync = 0)
        {
            Name = name;
            Unit = unit;
            HigherIsWorse = higherIsWorse;
            RelativeThreshold = relativeThreshold;
            AbsoluteThreshold = absoluteThreshold;
            AbsoluteThresholdVsync = absoluteThresholdVsync;
        }
    }

    public static class MetricsComputer
    {
        public static void Fill(Series series, double vsyncMs)
        {
            if (series.IntervalsMs == null || series.IntervalsMs.Length == 0)
                return;
            GHPerfStats.PacingMetrics m = GHPerfStats.ComputePacing(series.IntervalsMs, vsyncMs);
            Dictionary<string, double> d = series.Metrics;
            d[MetricNames.FrameDurationP50] = m.P50;
            d[MetricNames.FrameDurationP90] = m.P90;
            d[MetricNames.FrameDurationP95] = m.P95;
            d[MetricNames.FrameDurationP99] = m.P99;
            d[MetricNames.FrameDurationMax] = m.Max;
            d[MetricNames.FrameDurationMean] = m.MeanMs;
            d[MetricNames.FrameDurationStdDev] = m.StdDevMs;
            d[MetricNames.FpsMean] = m.FpsMean;
            d[MetricNames.Fps1PctLow] = m.Fps1PctLow;
            d[MetricNames.Fps01PctLow] = m.Fps01PctLow;
            d[MetricNames.FrameOverrunP50] = m.OverrunP50;
            d[MetricNames.FrameOverrunP95] = m.OverrunP95;
            d[MetricNames.FrameOverrunP99] = m.OverrunP99;
            d[MetricNames.JankPct] = m.JankPct;
            d[MetricNames.HitchRatio] = m.HitchRatioMsPerSec;
            d[MetricNames.Hitch2xPerMin] = m.Hitch2xPerMin;
            d[MetricNames.Hitch4xPerMin] = m.Hitch4xPerMin;
            d[MetricNames.StutterIndex] = m.StutterIndex;
            d[MetricNames.FrameCount] = m.Count;
            d[MetricNames.WindowSeconds] = m.WindowMs / 1000.0;
        }
    }
}

using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
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
           and the pre-registered regression thresholds (see DEVEL/performance/README.md's
           Statistics section). Absolute thresholds expressed against the display are now
           in target-period units, since the target content rate can differ from the
           refresh rate. Defined in GHPerformanceComparison, shared with the in-app
           harness. */
        public static readonly GHDecisionMetric[] Decision = GHPerformanceComparison.ExternalDecision;

        /* Reported alongside, not decided on. */
        public static readonly string[] Reported =
        {
            FrameDurationP50, FrameDurationP95, FrameDurationMax, FpsMean, Fps01PctLow,
            JankPct, Hitch2xPerMin, Hitch4xPerMin, StutterIndex, FrameOverrunP99, FrameCount, WindowSeconds
        };

        /* The series an in-app (schema v2) run record carries; its metric keys are the
           app's own smoothness names */
        public const string SmoothnessSeries = "smoothness";
        public const string DisplayedFps = "displayedFps";
        public const string HitchRatioMsPerSec = "hitchRatioMsPerSec";
        public const string PacingErrorRmsMs = "pacingErrorRmsMs";
        public const string PacingErrorP99Ms = "pacingErrorP99Ms";
        public const string JudderPct = "judderPct";
        public const string RepeatedRefreshesPerSec = "repeatedRefreshesPerSec";
        public const string LatencyP50Ms = "latencyP50Ms";
        public const string LatencyP99Ms = "latencyP99Ms";
        public const string PaintP99Ms = "paintP99Ms";
        public const string TileAnimationFps = "tileAnimationFps";
        public const string HitchCount = "hitchCount";
        public const string DroppedCount = "droppedCount";
        public const string DisplayedCount = "displayedCount";

        /* Decision metrics of the smoothness series, read directly from the run records.
           Defined in GHPerformanceComparison, shared with the in-app harness. */
        public static readonly GHDecisionMetric[] SmoothnessDecision = GHPerformanceComparison.SmoothnessDecision;

        public static readonly string[] SmoothnessReported =
        {
            PacingErrorP99Ms, JudderPct, RepeatedRefreshesPerSec, LatencyP50Ms, LatencyP99Ms, PaintP99Ms,
            TileAnimationFps, HitchCount, DroppedCount, DisplayedCount
        };

        public static GHDecisionMetric[] DecisionFor(string seriesKind)
        {
            return IsSmoothness(seriesKind) ? SmoothnessDecision : Decision;
        }

        public static string[] ReportedFor(string seriesKind)
        {
            return IsSmoothness(seriesKind) ? SmoothnessReported : Reported;
        }

        public static bool IsSmoothness(string seriesKind)
        {
            return string.Equals(seriesKind, SmoothnessSeries, StringComparison.OrdinalIgnoreCase);
        }
    }

    public static class MetricsComputer
    {
        public static void Fill(Series series, double targetPeriodMs, double refreshPeriodMs)
        {
            if (series.IntervalsMs == null || series.IntervalsMs.Length == 0)
                return;
            GHPerformanceStatistics.PacingMetrics m = GHPerformanceStatistics.ComputePacing(series.IntervalsMs, targetPeriodMs, refreshPeriodMs);
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

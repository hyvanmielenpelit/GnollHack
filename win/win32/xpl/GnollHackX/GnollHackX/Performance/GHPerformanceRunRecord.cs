using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;
using Newtonsoft.Json;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX.Performance
{
    /* One measurement window, in the shape DEVEL/performance/schema/run-record.schema.json
       (schema v2) describes and the offline analyzer reads: a scenario and arm label, the
       environment the run was measured in, thermal readings taken before and after, the
       smoothness summary and on-screen pacing metrics GHSmoothnessMetrics computes, the
       raw on-screen interval and pacing-error series, and the UI thread latency probe.

       BeginWindow and EndWindowAndSave bracket a measurement: BeginWindow remembers
       whether the frame timeline was already running, enables it (which resets its ring),
       and takes the first thermal reading; EndWindowAndSave takes the second reading,
       copies the timeline, computes the summary, writes the JSON and the two CSV dumps,
       and restores the timeline to whatever state it was in before BeginWindow.
       SaveRecent writes the same document from what the timeline already holds, with no
       window. None of the three throws.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHPerformanceRunRecord
    {
        public const int CurrentSchemaVersion = 2;

        private static int _windowOpen = 0;
        private static string _scenario = "";
        private static string _arm = "";
        private static bool _wasTimelineEnabledBeforeWindow;
        private static GHThermalReading _thermalBefore;
        private static DateTime _startedUtc;

        private static readonly JsonSerializerSettings _jsonSettings = new JsonSerializerSettings
        {
            Formatting = Formatting.Indented,
            Culture = CultureInfo.InvariantCulture,
            NullValueHandling = NullValueHandling.Include
        };

        public static bool IsWindowOpen { get { return Interlocked.CompareExchange(ref _windowOpen, 0, 0) != 0; } }

        public static string CurrentScenario { get { return _scenario; } }

        /* The archive directory's performance subdirectory, created when missing */
        public static string DefaultDirectory
        {
            get
            {
                string ghdir = GHApp.GnollHackService.GetGnollHackPath();
                string archive = Path.Combine(ghdir, GHConstants.ArchiveDirectory);
                if (!Directory.Exists(archive))
                    GHApp.CheckCreateDirectory(archive);
                string performance = Path.Combine(archive, "performance");
                if (!Directory.Exists(performance))
                    GHApp.CheckCreateDirectory(performance);
                return performance;
            }
        }

        /* Starts a measurement window: remembers whether the frame timeline was already
           enabled, enables it (which resets it), reads the thermal state, and notes the
           start time. A second call while a window is open replaces it. */
        public static void BeginWindow(string scenario, string arm)
        {
            try
            {
                _scenario = scenario ?? "";
                _arm = arm ?? "";
                _wasTimelineEnabledBeforeWindow = GHFrameTimeline.IsEnabled;
                FrameTimeProfiler.IsEnabled = true;
                _thermalBefore = GHThermalProbe.Read();
                _startedUtc = DateTime.UtcNow;
                Interlocked.Exchange(ref _windowOpen, 1);
            }
            catch
            {
                Interlocked.Exchange(ref _windowOpen, 0);
            }
        }

        /* Ends the window, writes run_<stamp>_<scenario>_<arm>.json plus the frame
           timeline and compositor frame CSVs into directory (created if missing), restores
           the timeline's previous enabled state, and returns the JSON path, or null on any
           failure. */
        public static string EndWindowAndSave(string directory)
        {
            if (Interlocked.CompareExchange(ref _windowOpen, 0, 1) != 1)
                return null;
            try
            {
                return Save(directory, _scenario, _arm, _startedUtc, _thermalBefore, GHThermalProbe.Read());
            }
            catch
            {
                return null;
            }
            finally
            {
                try
                {
                    if (!_wasTimelineEnabledBeforeWindow)
                        FrameTimeProfiler.IsEnabled = false;
                }
                catch { }
            }
        }

        /* Saves what the timeline already holds, up to its last Capacity ticks, without
           opening a window: for capturing a stutter just after it was seen. The window start
           is derived from the retained ticks, and with no earlier thermal reading available
           the current one stands for both. Returns the JSON path, or null when the timeline
           is off or holds nothing; never throws. */
        public static string SaveRecent(string directory, string scenario, string arm)
        {
            try
            {
                if (!GHFrameTimeline.IsEnabled)
                    return null;
                GHFrameRecord[] probe = new GHFrameRecord[GHFrameTimeline.Capacity];
                int n = GHFrameTimeline.CopyRecords(probe);
                if (n == 0)
                    return null;
                double spanSeconds = (double)(probe[n - 1].CallbackStartTicks - probe[0].CallbackStartTicks) / Stopwatch.Frequency;
                DateTime startedUtc = DateTime.UtcNow - TimeSpan.FromSeconds(Math.Max(0, spanSeconds));
                GHThermalReading now = GHThermalProbe.Read();
                return Save(directory, scenario ?? "", arm ?? "", startedUtc, now, now);
            }
            catch
            {
                return null;
            }
        }

        private static string Save(string directory, string scenario, string arm, DateTime startedUtc,
            GHThermalReading thermalBefore, GHThermalReading thermalAfter)
        {
            DateTime endedUtc = DateTime.UtcNow;

            GHFrameRecord[] recordBuffer = new GHFrameRecord[GHFrameTimeline.Capacity];
            int n = GHFrameTimeline.CopyRecords(recordBuffer);
            GHCompositorFrame[] compositorBuffer = new GHCompositorFrame[GHFrameTimeline.Capacity];
            int m = GHFrameTimeline.CopyCompositorFrames(compositorBuffer);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[n];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(recordBuffer, n, compositorBuffer, m,
                displayed, out displayedCount);

            if (!Directory.Exists(directory))
                Directory.CreateDirectory(directory);

            string stamp = endedUtc.ToString("yyyyMMdd_HHmmss", CultureInfo.InvariantCulture);
            string stem = "run_" + stamp + "_" + SafeFileToken(scenario) + "_" + SafeFileToken(arm);
            string timelineCsvName = "frametimeline_" + stem + ".csv";
            string compositorCsvName = "compositorframes_" + stem + ".csv";
            string jsonPath = Path.Combine(directory, stem + ".json");

            GHFrameTimeline.DumpToCsv(Path.Combine(directory, timelineCsvName));
            GHFrameTimeline.DumpCompositorFramesToCsv(Path.Combine(directory, compositorCsvName));

            object doc = BuildDocument(stem, scenario, arm, startedUtc, endedUtc, thermalBefore, thermalAfter,
                summary, displayed, displayedCount, timelineCsvName, compositorCsvName);

            string json = JsonConvert.SerializeObject(doc, _jsonSettings);
            File.WriteAllText(jsonPath, json + Environment.NewLine, new UTF8Encoding(false));
            return jsonPath;
        }

        private static object BuildDocument(string runId, string scenario, string arm, DateTime startedUtc, DateTime endedUtc,
            GHThermalReading before, GHThermalReading after, GHSmoothnessSummary summary,
            GHDisplayedFrame[] displayed, int displayedCount, string timelineCsvName, string compositorCsvName)
        {
            GHPerformanceEnvironmentFacts facts = GHPerformanceEnvironment.Capture();

            DocRoot doc = new DocRoot();
            doc.SchemaVersion = CurrentSchemaVersion;
            doc.RunId = runId;
            doc.Scenario = scenario;
            doc.Arm = arm;
            doc.StartedUtc = startedUtc.ToString("o", CultureInfo.InvariantCulture);
            doc.EndedUtc = endedUtc.ToString("o", CultureInfo.InvariantCulture);
            doc.WindowSeconds = R((endedUtc - startedUtc).TotalSeconds);

            doc.Environment = BuildEnvironment(facts);
            doc.Thermal = new ThermalJson { Before = ReadingJson.From(before), After = ReadingJson.From(after) };
            doc.Clock = BuildClock();
            doc.Display = BuildDisplay(summary);
            doc.Smoothness = BuildSmoothness(summary);
            doc.OnScreenPacing = BuildPacing(summary.OnScreenPacing);
            doc.Series = BuildSeries(displayed, displayedCount);
            doc.UiThread = BuildUiThread();
            doc.Files = new FilesJson { FrameTimeline = timelineCsvName, CompositorFrames = compositorCsvName };
            return doc;
        }

        private static EnvironmentJson BuildEnvironment(GHPerformanceEnvironmentFacts f)
        {
            EnvironmentJson e = new EnvironmentJson();
            e.Platform = f.Platform;
            e.DeviceId = f.DeviceId;
            e.DeviceModel = f.DeviceModel;
            e.DeviceOs = f.DeviceOs;
            e.AppVersion = f.AppVersion;
            e.RuntimeVersion = f.RuntimeVersion;
            e.FrameworkVersion = f.FrameworkVersion;
            e.UiFrameworkVersion = f.UiFrameworkVersion;
            e.SkiaSharpVersion = f.SkiaSharpVersion;
            e.FmodVersion = f.FmodVersion;
            e.GpuBackend = f.GpuBackend;
            e.GpuCacheSize = f.GpuCacheSize;
            e.MainCanvasUsesGpu = f.MainCanvasUsesGpu;
            e.MapRefreshRateSetting = f.MapRefreshRateSetting;
            e.ReportedRefreshHz = R(f.ReportedRefreshHz);
            e.BuildConfiguration = f.BuildConfiguration;
            e.GitCommit = f.GitCommit;
            e.Configuration = f.Configuration;
            return e;
        }

        private static ClockJson BuildClock()
        {
            ClockJson c = new ClockJson();
            c.StopwatchFrequency = Stopwatch.Frequency;
            if (GHFrameTimeline.HasClockAnchor)
            {
                GHClockAnchor first = GHFrameTimeline.FirstClockAnchor;
                GHClockAnchor latest = GHFrameTimeline.LatestClockAnchor;
                c.FirstAnchor = new AnchorJson { PlatformNanos = first.PlatformNanos, StopwatchTicks = first.StopwatchTicks };
                c.LatestAnchor = new AnchorJson { PlatformNanos = latest.PlatformNanos, StopwatchTicks = latest.StopwatchTicks };
            }
            return c;
        }

        private static DisplayJson BuildDisplay(GHSmoothnessSummary s)
        {
            DisplayJson d = new DisplayJson();
            d.MeasuredRefreshHz = R(s.MeasuredRefreshHz);
            d.AssumedRefreshHz = R(s.AssumedRefreshHz);
            d.TargetFps = R(s.TargetFps);
            d.AssumedRefreshMismatch = s.AssumedRefreshMismatch;
            return d;
        }

        private static SmoothnessJson BuildSmoothness(GHSmoothnessSummary s)
        {
            SmoothnessJson j = new SmoothnessJson();
            j.TickCount = s.TickCount;
            j.PaintedCount = s.PaintedCount;
            j.DisplayedCount = s.DisplayedCount;
            j.DroppedCount = s.DroppedCount;
            j.CoalescedCount = s.CoalescedCount;
            j.NotRunCount = s.NotRunCount;
            j.PausedGapCount = s.PausedGapCount;
            j.WindowMs = R(s.WindowMs);
            j.MeasuredRefreshHz = R(s.MeasuredRefreshHz);
            j.AssumedRefreshHz = R(s.AssumedRefreshHz);
            j.TargetFps = R(s.TargetFps);
            j.AssumedRefreshMismatch = s.AssumedRefreshMismatch;
            j.DisplayedFps = R(s.DisplayedFps);
            j.TileAnimationFps = R(s.TileAnimationFps);
            j.HitchCount = s.HitchCount;
            j.HitchRatioMsPerSec = R(s.HitchRatioMsPerSec);
            j.JudderCount = s.JudderCount;
            j.JudderPct = R(s.JudderPct);
            j.PacingErrorRmsMs = R(s.PacingErrorRmsMs);
            j.PacingErrorP99Ms = R(s.PacingErrorP99Ms);
            j.RepeatedRefreshesPerSec = R(s.RepeatedRefreshesPerSec);
            j.LatencyP50Ms = R(s.LatencyP50Ms);
            j.LatencyP99Ms = R(s.LatencyP99Ms);
            j.CallbackLatenessP99Ms = R(s.CallbackLatenessP99Ms);
            j.PaintP50Ms = R(s.PaintP50Ms);
            j.PaintP99Ms = R(s.PaintP99Ms);
            j.GcCount = s.GcCount;
            j.PresentSource = s.PresentSource.ToString();
            j.UnattributedShare = R(s.UnattributedShare);

            j.Causes = new Dictionary<string, CauseJson>();
            for (int i = 1; i < GHSmoothnessMetrics.CauseCount; i++)
            {
                GHHitchCause cause = (GHHitchCause)i;
                CauseJson cj = new CauseJson();
                cj.Count = s.CauseCount[i];
                cj.HitchMs = R(s.CauseMs[i]);
                j.Causes[GHSmoothnessMetrics.CauseName(cause)] = cj;
            }
            return j;
        }

        private static PacingJson BuildPacing(GHPerformanceStatistics.PacingMetrics p)
        {
            PacingJson j = new PacingJson();
            j.Count = p.Count;
            j.WindowMs = R(p.WindowMs);
            j.TargetPeriodMs = R(p.TargetPeriodMs);
            j.RefreshPeriodMs = R(p.RefreshPeriodMs);
            j.FpsMean = R(p.FpsMean);
            j.Fps1PctLow = R(p.Fps1PctLow);
            j.Fps01PctLow = R(p.Fps01PctLow);
            j.P50 = R(p.P50);
            j.P90 = R(p.P90);
            j.P95 = R(p.P95);
            j.P99 = R(p.P99);
            j.Max = R(p.Max);
            j.Min = R(p.Min);
            j.MeanMs = R(p.MeanMs);
            j.StdDevMs = R(p.StdDevMs);
            j.StutterIndex = R(p.StutterIndex);
            j.JankPct = R(p.JankPct);
            j.HitchRatioMsPerSec = R(p.HitchRatioMsPerSec);
            j.Hitch2xCount = p.Hitch2xCount;
            j.Hitch4xCount = p.Hitch4xCount;
            j.Hitch2xPerMin = R(p.Hitch2xPerMin);
            j.Hitch4xPerMin = R(p.Hitch4xPerMin);
            j.OverrunP50 = R(p.OverrunP50);
            j.OverrunP95 = R(p.OverrunP95);
            j.OverrunP99 = R(p.OverrunP99);
            return j;
        }

        /* Gaps and pacing errors of displayed frames after the first, excluding paused
           gaps, in the ms domain, in display order. */
        private static SeriesJson BuildSeries(GHDisplayedFrame[] displayed, int displayedCount)
        {
            SeriesJson j = new SeriesJson();
            List<double> intervals = new List<double>();
            List<double> pacingErrors = new List<double>();
            double msPerTick = 1000.0 / Stopwatch.Frequency;
            for (int i = 1; i < displayedCount; i++)
            {
                if (displayed[i].IsPausedGap)
                    continue;
                intervals.Add(Math.Round(displayed[i].GapTicks * msPerTick, 3));
                pacingErrors.Add(Math.Round(displayed[i].PacingErrorTicks * msPerTick, 3));
            }
            j.OnScreenIntervalsMs = intervals.ToArray();
            j.PacingErrorMs = pacingErrors.ToArray();
            return j;
        }

        private static UiThreadJson BuildUiThread()
        {
            UiThreadJson j = new UiThreadJson();
            try
            {
                j.LatencyP50Ms = R(GHUiThreadProbe.LatencyP50Ms);
                j.LatencyP95Ms = R(GHUiThreadProbe.LatencyP95Ms);
                j.LatencyP99Ms = R(GHUiThreadProbe.LatencyP99Ms);
                j.LatencyMaxMs = R(GHUiThreadProbe.LatencyMaxMs);
                j.TickCount = GHUiThreadProbe.TickCount;
                j.LateTickCount = GHUiThreadProbe.LateTickCount;
            }
            catch { }
            return j;
        }

        private static double R(double value)
        {
            if (double.IsNaN(value) || double.IsInfinity(value))
                return 0;
            return Math.Round(value, 3);
        }

        private static string SafeFileToken(string value)
        {
            if (string.IsNullOrEmpty(value))
                return "none";
            char[] invalid = Path.GetInvalidFileNameChars();
            StringBuilder sb = new StringBuilder(value.Length);
            for (int i = 0; i < value.Length; i++)
            {
                char c = value[i];
                bool bad = c <= ' ' || c == '.' || Array.IndexOf(invalid, c) >= 0;
                sb.Append(bad ? '_' : c);
            }
            return sb.ToString();
        }

        /* ---- JSON document shape (schema v2) ---- */

        private sealed class DocRoot
        {
            [JsonProperty("schemaVersion")]
            public int SchemaVersion;

            [JsonProperty("runId")]
            public string RunId;

            [JsonProperty("scenario")]
            public string Scenario;

            [JsonProperty("arm")]
            public string Arm;

            [JsonProperty("startedUtc")]
            public string StartedUtc;

            [JsonProperty("endedUtc")]
            public string EndedUtc;

            [JsonProperty("windowSeconds")]
            public double WindowSeconds;

            [JsonProperty("environment")]
            public EnvironmentJson Environment;

            [JsonProperty("thermal")]
            public ThermalJson Thermal;

            [JsonProperty("clock")]
            public ClockJson Clock;

            [JsonProperty("display")]
            public DisplayJson Display;

            [JsonProperty("smoothness")]
            public SmoothnessJson Smoothness;

            [JsonProperty("onScreenPacing")]
            public PacingJson OnScreenPacing;

            [JsonProperty("series")]
            public SeriesJson Series;

            [JsonProperty("uiThread")]
            public UiThreadJson UiThread;

            [JsonProperty("files")]
            public FilesJson Files;
        }

        private sealed class EnvironmentJson
        {
            [JsonProperty("platform")]
            public string Platform;

            [JsonProperty("deviceId")]
            public string DeviceId;

            [JsonProperty("deviceModel")]
            public string DeviceModel;

            [JsonProperty("deviceOs")]
            public string DeviceOs;

            [JsonProperty("appVersion")]
            public string AppVersion;

            [JsonProperty("runtimeVersion")]
            public string RuntimeVersion;

            [JsonProperty("frameworkVersion")]
            public string FrameworkVersion;

            [JsonProperty("uiFrameworkVersion")]
            public string UiFrameworkVersion;

            [JsonProperty("skiaSharpVersion")]
            public string SkiaSharpVersion;

            [JsonProperty("fmodVersion")]
            public string FmodVersion;

            [JsonProperty("gpuBackend")]
            public string GpuBackend;

            [JsonProperty("gpuCacheSize")]
            public long GpuCacheSize;

            [JsonProperty("mainCanvasUsesGpu")]
            public bool MainCanvasUsesGpu;

            [JsonProperty("mapRefreshRateSetting")]
            public string MapRefreshRateSetting;

            [JsonProperty("reportedRefreshHz")]
            public double ReportedRefreshHz;

            [JsonProperty("buildConfiguration")]
            public string BuildConfiguration;

            [JsonProperty("gitCommit")]
            public string GitCommit;

            [JsonProperty("configuration")]
            public Dictionary<string, object> Configuration;
        }

        private sealed class ThermalJson
        {
            [JsonProperty("before")]
            public ReadingJson Before;

            [JsonProperty("after")]
            public ReadingJson After;
        }

        /* A GHThermalReading in the schema's shape: status by name, NaN fields as null */
        private sealed class ReadingJson
        {
            [JsonProperty("status")]
            public string Status;

            [JsonProperty("headroomFraction")]
            public double? HeadroomFraction;

            [JsonProperty("batteryTempC")]
            public double? BatteryTempC;

            [JsonProperty("cpuPerformancePct")]
            public double? CpuPerformancePct;

            [JsonProperty("isCharging")]
            public bool IsCharging;

            [JsonProperty("isLowPower")]
            public bool IsLowPower;

            public static ReadingJson From(GHThermalReading r)
            {
                ReadingJson j = new ReadingJson();
                j.Status = GHThermalProbe.StatusName(r.Status);
                j.HeadroomFraction = NullIfNaN(r.HeadroomFraction);
                j.BatteryTempC = NullIfNaN(r.BatteryTempC);
                j.CpuPerformancePct = NullIfNaN(r.CpuPerformancePct);
                j.IsCharging = r.IsCharging;
                j.IsLowPower = r.IsLowPower;
                return j;
            }

            private static double? NullIfNaN(float value)
            {
                if (float.IsNaN(value) || float.IsInfinity(value))
                    return null;
                return Math.Round((double)value, 3);
            }
        }

        private sealed class ClockJson
        {
            [JsonProperty("stopwatchFrequency")]
            public long StopwatchFrequency;

            [JsonProperty("firstAnchor")]
            public AnchorJson FirstAnchor;

            [JsonProperty("latestAnchor")]
            public AnchorJson LatestAnchor;
        }

        private sealed class AnchorJson
        {
            [JsonProperty("platformNanos")]
            public long PlatformNanos;

            [JsonProperty("stopwatchTicks")]
            public long StopwatchTicks;
        }

        private sealed class DisplayJson
        {
            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            [JsonProperty("assumedRefreshHz")]
            public double AssumedRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("assumedRefreshMismatch")]
            public bool AssumedRefreshMismatch;
        }

        private sealed class SmoothnessJson
        {
            [JsonProperty("tickCount")]
            public int TickCount;

            [JsonProperty("paintedCount")]
            public int PaintedCount;

            [JsonProperty("displayedCount")]
            public int DisplayedCount;

            [JsonProperty("droppedCount")]
            public int DroppedCount;

            [JsonProperty("coalescedCount")]
            public int CoalescedCount;

            [JsonProperty("notRunCount")]
            public int NotRunCount;

            [JsonProperty("pausedGapCount")]
            public int PausedGapCount;

            [JsonProperty("windowMs")]
            public double WindowMs;

            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            [JsonProperty("assumedRefreshHz")]
            public double AssumedRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("assumedRefreshMismatch")]
            public bool AssumedRefreshMismatch;

            [JsonProperty("displayedFps")]
            public double DisplayedFps;

            [JsonProperty("tileAnimationFps")]
            public double TileAnimationFps;

            [JsonProperty("hitchCount")]
            public int HitchCount;

            [JsonProperty("hitchRatioMsPerSec")]
            public double HitchRatioMsPerSec;

            [JsonProperty("judderCount")]
            public int JudderCount;

            [JsonProperty("judderPct")]
            public double JudderPct;

            [JsonProperty("pacingErrorRmsMs")]
            public double PacingErrorRmsMs;

            [JsonProperty("pacingErrorP99Ms")]
            public double PacingErrorP99Ms;

            [JsonProperty("repeatedRefreshesPerSec")]
            public double RepeatedRefreshesPerSec;

            [JsonProperty("latencyP50Ms")]
            public double LatencyP50Ms;

            [JsonProperty("latencyP99Ms")]
            public double LatencyP99Ms;

            [JsonProperty("callbackLatenessP99Ms")]
            public double CallbackLatenessP99Ms;

            [JsonProperty("paintP50Ms")]
            public double PaintP50Ms;

            [JsonProperty("paintP99Ms")]
            public double PaintP99Ms;

            [JsonProperty("gcCount")]
            public int GcCount;

            [JsonProperty("presentSource")]
            public string PresentSource;

            [JsonProperty("unattributedShare")]
            public double UnattributedShare;

            [JsonProperty("causes")]
            public Dictionary<string, CauseJson> Causes;
        }

        private sealed class CauseJson
        {
            [JsonProperty("count")]
            public int Count;

            [JsonProperty("hitchMs")]
            public double HitchMs;
        }

        private sealed class PacingJson
        {
            [JsonProperty("count")]
            public int Count;

            [JsonProperty("windowMs")]
            public double WindowMs;

            [JsonProperty("targetPeriodMs")]
            public double TargetPeriodMs;

            [JsonProperty("refreshPeriodMs")]
            public double RefreshPeriodMs;

            [JsonProperty("fpsMean")]
            public double FpsMean;

            [JsonProperty("fps1PctLow")]
            public double Fps1PctLow;

            [JsonProperty("fps01PctLow")]
            public double Fps01PctLow;

            [JsonProperty("p50")]
            public double P50;

            [JsonProperty("p90")]
            public double P90;

            [JsonProperty("p95")]
            public double P95;

            [JsonProperty("p99")]
            public double P99;

            [JsonProperty("max")]
            public double Max;

            [JsonProperty("min")]
            public double Min;

            [JsonProperty("meanMs")]
            public double MeanMs;

            [JsonProperty("stdDevMs")]
            public double StdDevMs;

            [JsonProperty("stutterIndex")]
            public double StutterIndex;

            [JsonProperty("jankPct")]
            public double JankPct;

            [JsonProperty("hitchRatioMsPerSec")]
            public double HitchRatioMsPerSec;

            [JsonProperty("hitch2xCount")]
            public int Hitch2xCount;

            [JsonProperty("hitch4xCount")]
            public int Hitch4xCount;

            [JsonProperty("hitch2xPerMin")]
            public double Hitch2xPerMin;

            [JsonProperty("hitch4xPerMin")]
            public double Hitch4xPerMin;

            [JsonProperty("overrunP50")]
            public double OverrunP50;

            [JsonProperty("overrunP95")]
            public double OverrunP95;

            [JsonProperty("overrunP99")]
            public double OverrunP99;
        }

        private sealed class SeriesJson
        {
            [JsonProperty("onScreenIntervalsMs")]
            public double[] OnScreenIntervalsMs;

            [JsonProperty("pacingErrorMs")]
            public double[] PacingErrorMs;
        }

        private sealed class UiThreadJson
        {
            [JsonProperty("latencyP50Ms")]
            public double LatencyP50Ms;

            [JsonProperty("latencyP95Ms")]
            public double LatencyP95Ms;

            [JsonProperty("latencyP99Ms")]
            public double LatencyP99Ms;

            [JsonProperty("latencyMaxMs")]
            public double LatencyMaxMs;

            [JsonProperty("tickCount")]
            public long TickCount;

            [JsonProperty("lateTickCount")]
            public long LateTickCount;
        }

        private sealed class FilesJson
        {
            [JsonProperty("frameTimeline")]
            public string FrameTimeline;

            [JsonProperty("compositorFrames")]
            public string CompositorFrames;
        }
    }
}

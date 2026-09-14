using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using Newtonsoft.Json;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX.Perf
{
    /* One measurement run, in the shape DEVEL/perf/schema/run-record.schema.json
       describes and the offline analyzer's RunRecord model reads: camelCase property
       names, one internal FrameTimeProfiler series with the exact inter-frame
       intervals of the window, the histogram figures alongside for cross-checking,
       and the thermal readings taken before and after the window.

       Capture reads every probe once and computes the pacing metrics from the copied
       intervals with GHPerfStats.ComputePacing, so the device and the analyzer agree
       on the numbers. Serialization uses Newtonsoft.Json, which both builds reference.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public sealed class GHPerfRunRecord
    {
        public const int CurrentSchemaVersion = 1;

        /* Runs with fewer exact intervals than this are excluded from comparisons */
        public const int MinimumIntervalCount = 100;

        /* CPU performance percent at or below which a run counts as throttled */
        public const double ThrottledCpuPerformancePct = 90.0;

        [JsonProperty("schemaVersion")]
        public int SchemaVersion = CurrentSchemaVersion;

        [JsonProperty("id")]
        public string Id;

        [JsonProperty("timestampUtc")]
        public string TimestampUtc;

        [JsonProperty("scenario")]
        public string Scenario;

        [JsonProperty("scenarioKind")]
        public string ScenarioKind = "gameplay";

        [JsonProperty("arm")]
        public string Arm;

        [JsonProperty("buildConfiguration")]
        public string BuildConfiguration;

        [JsonProperty("git")]
        public GHPerfGitInfo Git = new GHPerfGitInfo();

        [JsonProperty("platform")]
        public string Platform;

        [JsonProperty("device")]
        public GHPerfDeviceInfo Device = new GHPerfDeviceInfo();

        [JsonProperty("versions")]
        public Dictionary<string, string> Versions = new Dictionary<string, string>();

        [JsonProperty("display")]
        public GHPerfDisplayInfo Display = new GHPerfDisplayInfo();

        [JsonProperty("configuration")]
        public Dictionary<string, object> Configuration = new Dictionary<string, object>();

        [JsonProperty("thermal")]
        public GHPerfThermalInfo Thermal = new GHPerfThermalInfo();

        [JsonProperty("series")]
        public List<GHPerfSeries> Series = new List<GHPerfSeries>();

        [JsonProperty("markers")]
        public List<GHPerfMarker> Markers = new List<GHPerfMarker>();

        [JsonProperty("notes")]
        public string Notes;

        [JsonProperty("excluded")]
        public bool Excluded;

        [JsonProperty("exclusionReason")]
        public string ExclusionReason;

        [JsonProperty("warmupSeconds")]
        public double WarmupSeconds;

        [JsonProperty("windowSeconds")]
        public double WindowSeconds;

        private static readonly JsonSerializerSettings _jsonSettings = new JsonSerializerSettings
        {
            Formatting = Formatting.Indented,
            Culture = CultureInfo.InvariantCulture,
            FloatFormatHandling = FloatFormatHandling.DefaultValue,
            NullValueHandling = NullValueHandling.Include
        };

        /* Snapshots every probe and the environment into a record. The intervals are
           the exact inter-frame gaps still in the profiler's ring buffer, so a window
           longer than the ring yields only its tail; the histogram figures cover the
           whole window. */
        public static GHPerfRunRecord Capture(string scenario, string scenarioKind, string arm,
            GHThermalReading before, GHThermalReading after,
            double warmupSeconds, double windowSeconds, string notes)
        {
            GHPerfRunRecord rec = new GHPerfRunRecord();
            DateTime now = DateTime.UtcNow;
            rec.Id = Guid.NewGuid().ToString("N");
            rec.TimestampUtc = now.ToString("o", CultureInfo.InvariantCulture);
            rec.Scenario = scenario ?? "";
            rec.ScenarioKind = string.IsNullOrEmpty(scenarioKind) ? "gameplay" : scenarioKind;
            rec.Arm = arm ?? "";
            rec.Notes = notes;
            rec.WarmupSeconds = warmupSeconds;
            rec.WindowSeconds = windowSeconds;

            GHPerfEnvironmentFacts facts = GHApp.GetPerfEnvironmentFacts();
            rec.Platform = facts.Platform;
            rec.BuildConfiguration = facts.BuildConfiguration;
            rec.Git.Commit = facts.GitCommit;
            rec.Device.Id = facts.DeviceId;
            rec.Device.Model = facts.DeviceModel;
            rec.Device.Os = facts.DeviceOs;
            AddVersion(rec.Versions, "app", facts.AppVersion);
            AddVersion(rec.Versions, "dotnet", facts.FrameworkVersion);
            AddVersion(rec.Versions, "maui", facts.UiFrameworkVersion);
            AddVersion(rec.Versions, "runtime", facts.RuntimeVersion);
            AddVersion(rec.Versions, "skiasharp", facts.SkiaSharpVersion);
            AddVersion(rec.Versions, "fmod", facts.FmodVersion);
            if (facts.Configuration != null)
                rec.Configuration = facts.Configuration;

            float vsyncMs = FrameTimeProfiler.VsyncPeriodMs;
            rec.Display.RefreshHz = facts.RefreshHz > 0 ? facts.RefreshHz : (vsyncMs > 0 ? 1000.0 / vsyncMs : 0);
            rec.Display.VsyncMs = vsyncMs;
            rec.Display.GpuBackend = facts.GpuBackend;

            /* Exact intervals and their pacing metrics */
            float[] buffer = new float[1800];
            int count = FrameTimeProfiler.CopyWindowIntervals(buffer);
            float[] intervals = new float[count];
            Array.Copy(buffer, intervals, count);
            GHPerfStats.PacingMetrics pacing = GHPerfStats.ComputePacing(intervals, vsyncMs);

            FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();

            GHPerfSeries series = new GHPerfSeries();
            series.Kind = "internal";
            series.Source = "FrameTimeProfiler";
            series.Column = "interFrame";
            series.IntervalsMs = intervals;
            series.FrameDurationsMs = null;
            series.DroppedCount = (int)s.PauseExcludedCount;
            FillMetrics(series.Metrics, pacing, s);
            FillInfo(series.Info, facts, s, before, after);
            rec.Series.Add(series);

            rec.Markers = CaptureMarkers();

            rec.Thermal.Before = GHPerfThermalReadingJson.From(before);
            rec.Thermal.After = GHPerfThermalReadingJson.From(after);
            EvaluateThrottling(rec.Thermal, before, after);

            if (rec.Thermal.Throttled)
            {
                rec.Excluded = true;
                rec.ExclusionReason = "throttled: " + rec.Thermal.ThrottleReason;
            }
            else if (count < MinimumIntervalCount)
            {
                rec.Excluded = true;
                rec.ExclusionReason = "too few intervals: " + count.ToString(CultureInfo.InvariantCulture)
                    + " < " + MinimumIntervalCount.ToString(CultureInfo.InvariantCulture);
            }
            else if (rec.Thermal.PowerState == "changed")
            {
                rec.Excluded = true;
                rec.ExclusionReason = "power state changed during the run (plugged or unplugged)";
            }
            return rec;
        }

        private static void AddVersion(Dictionary<string, string> versions, string key, string value)
        {
            if (!string.IsNullOrEmpty(value))
                versions[key] = value;
        }

        private static void FillMetrics(Dictionary<string, double> m, GHPerfStats.PacingMetrics p,
            FrameTimeStreamingStatistics s)
        {
            /* Exact figures from the copied intervals */
            m["frameDurationP50"] = p.P50;
            m["frameDurationP90"] = p.P90;
            m["frameDurationP95"] = p.P95;
            m["frameDurationP99"] = p.P99;
            m["frameDurationMax"] = p.Max;
            m["frameDurationMean"] = p.MeanMs;
            m["frameDurationStdDev"] = p.StdDevMs;
            m["fpsMean"] = p.FpsMean;
            m["fps1PctLow"] = p.Fps1PctLow;
            m["fps01PctLow"] = p.Fps01PctLow;
            m["frameOverrunP50"] = p.OverrunP50;
            m["frameOverrunP95"] = p.OverrunP95;
            m["frameOverrunP99"] = p.OverrunP99;
            m["jankPct"] = p.JankPct;
            m["hitchRatio"] = p.HitchRatioMsPerSec;
            m["hitch2xPerMin"] = p.Hitch2xPerMin;
            m["hitch4xPerMin"] = p.Hitch4xPerMin;
            m["stutterIndex"] = p.StutterIndex;
            m["frameCount"] = p.Count;
            m["windowSeconds"] = p.WindowMs / 1000.0;

            /* Frame phases from the streaming histograms */
            m["updateP50Ms"] = s.UpdateP50Ms;
            m["updateP95Ms"] = s.UpdateP95Ms;
            m["updateP99Ms"] = s.UpdateP99Ms;
            m["updateMaxMs"] = s.UpdateMaxMs;
            m["lockWaitP95Ms"] = s.LockWaitP95Ms;
            m["lockWaitP99Ms"] = s.LockWaitP99Ms;
            m["lockWaitMaxMs"] = s.LockWaitMaxMs;
            m["paintP50Ms"] = s.PaintP50Ms;
            m["paintP95Ms"] = s.PaintP95Ms;
            m["paintP99Ms"] = s.PaintP99Ms;
            m["paintMaxMs"] = s.PaintMaxMs;
            m["flushP50Ms"] = s.FlushP50Ms;
            m["flushP95Ms"] = s.FlushP95Ms;
            m["flushP99Ms"] = s.FlushP99Ms;
            m["flushMaxMs"] = s.FlushMaxMs;
            m["totalFrameP95Ms"] = s.TotalFrameP95Ms;
            m["totalFrameMaxMs"] = s.TotalFrameMaxMs;
            m["lockFailPct"] = s.RenderedFrameCount > 0 ? 100.0 * s.LockFailCount / s.RenderedFrameCount : 0;
            m["pauseExcludedCount"] = s.PauseExcludedCount;
            m["tickCount"] = s.TickCount;
            m["renderedFrameCount"] = s.RenderedFrameCount;

            /* Histogram-based counterparts of the exact figures */
            m["streamHitchRatio"] = s.HitchRatioMsPerSec;
            m["streamFpsMean"] = s.FpsMean;
            m["streamInterFrameP99Ms"] = s.InterFrameP99Ms;

            /* Garbage collector */
            m["gcGen0Count"] = s.GcGen0Count;
            m["gcGen1Count"] = s.GcGen1Count;
            m["gcGen2Count"] = s.GcGen2Count;
            m["gcPauseGen0P99Ms"] = s.GcPauseGen0P99Ms;
            m["gcPauseGen0MaxMs"] = s.GcPauseGen0MaxMs;
            m["gcPauseGen1MaxMs"] = s.GcPauseGen1MaxMs;
            m["gcPauseGen2MaxMs"] = s.GcPauseGen2MaxMs;
            m["allocatedMB"] = s.AllocatedMB;
            m["allocationRateMBPerSec"] = s.AllocationRateMBPerSec;

            /* UI thread latency probe */
            m["uiLatencyP50Ms"] = GHUiThreadProbe.LatencyP50Ms;
            m["uiLatencyP95Ms"] = GHUiThreadProbe.LatencyP95Ms;
            m["uiLatencyP99Ms"] = GHUiThreadProbe.LatencyP99Ms;
            m["uiLatencyMaxMs"] = GHUiThreadProbe.LatencyMaxMs;
            m["uiLateTickCount"] = GHUiThreadProbe.LateTickCount;
            m["uiTickCount"] = GHUiThreadProbe.TickCount;

            /* Game thread processing between input requests */
            m["gameTurnP50Ms"] = GHGameTurnTimer.ProcessingP50Ms;
            m["gameTurnP95Ms"] = GHGameTurnTimer.ProcessingP95Ms;
            m["gameTurnP99Ms"] = GHGameTurnTimer.ProcessingP99Ms;
            m["gameTurnMaxMs"] = GHGameTurnTimer.ProcessingMaxMs;
            m["gameTurnCount"] = GHGameTurnTimer.ProcessingCount;
            m["gameTurnTotalMs"] = GHGameTurnTimer.TotalProcessingMs;
        }

        private static void FillInfo(Dictionary<string, string> info, GHPerfEnvironmentFacts facts,
            FrameTimeStreamingStatistics s, GHThermalReading before, GHThermalReading after)
        {
            info["windowElapsedMs"] = s.WindowElapsedMs.ToString("0.###", CultureInfo.InvariantCulture);
            info["windowOpen"] = s.WindowOpen ? "true" : "false";
            info["intervalRingSize"] = "1800";
            info["gpuCacheSizeBytes"] = facts.GpuCacheSize.ToString(CultureInfo.InvariantCulture);
            info["markerCount"] = s.MarkerCount.ToString(CultureInfo.InvariantCulture);
            if (!string.IsNullOrEmpty(before.Detail))
                info["thermalBeforeDetail"] = before.Detail;
            if (!string.IsNullOrEmpty(after.Detail))
                info["thermalAfterDetail"] = after.Detail;
        }

        /* Markers relative to the WindowBegin marker, or to the earliest marker when
           the window marker is not in the ring. */
        private static List<GHPerfMarker> CaptureMarkers()
        {
            List<GHPerfMarker> list = new List<GHPerfMarker>();
            FrameTimeMarker[] buffer = new FrameTimeMarker[256];
            int count = FrameTimeProfiler.CopyMarkers(buffer);
            if (count <= 0)
                return list;

            long origin = 0;
            bool haveOrigin = false;
            for (int i = 0; i < count; i++)
            {
                if (buffer[i].Code == FrameTimeProfiler.MarkerCodes.WindowBegin)
                {
                    origin = buffer[i].Timestamp;
                    haveOrigin = true;
                    break;
                }
            }
            if (!haveOrigin)
            {
                origin = buffer[0].Timestamp;
                for (int i = 1; i < count; i++)
                {
                    if (buffer[i].Timestamp < origin)
                        origin = buffer[i].Timestamp;
                }
            }

            double msPerTick = 1000.0 / Stopwatch.Frequency;
            for (int i = 0; i < count; i++)
            {
                GHPerfMarker mk = new GHPerfMarker();
                mk.AtMs = (buffer[i].Timestamp - origin) * msPerTick;
                mk.Code = buffer[i].Code;
                mk.Label = MarkerLabel(buffer[i].Code);
                list.Add(mk);
            }
            return list;
        }

        private static string MarkerLabel(int code)
        {
            switch (code)
            {
            case FrameTimeProfiler.MarkerCodes.FeltHitch:
                return "FeltHitch";
            case FrameTimeProfiler.MarkerCodes.PhaseStart:
                return "PhaseStart";
            case FrameTimeProfiler.MarkerCodes.PhaseEnd:
                return "PhaseEnd";
            case FrameTimeProfiler.MarkerCodes.WindowBegin:
                return "WindowBegin";
            case FrameTimeProfiler.MarkerCodes.WindowEnd:
                return "WindowEnd";
            default:
                return null;
            }
        }

        /* A run is throttled when either reading's status is above Moderate, or either
           reading's CPU performance is below the threshold. The status is checked first
           and names the gate signal. */
        private static void EvaluateThrottling(GHPerfThermalInfo t, GHThermalReading before, GHThermalReading after)
        {
            t.Throttled = false;
            t.GateSignal = "none";
            t.ThrottleReason = null;
            bool beforeKnown = before.Status != GHThermalStatus.Unknown || !float.IsNaN(before.CpuPerformancePct) || !float.IsNaN(before.BatteryTempC);
            bool afterKnown = after.Status != GHThermalStatus.Unknown || !float.IsNaN(after.CpuPerformancePct) || !float.IsNaN(after.BatteryTempC);
            if (!beforeKnown && !afterKnown)
                t.PowerState = "unknown";
            else if (beforeKnown && afterKnown && before.IsCharging != after.IsCharging)
                t.PowerState = "changed";
            else
                t.PowerState = (beforeKnown ? before.IsCharging : after.IsCharging) ? "charging" : "battery";

            if (before.Status > GHThermalStatus.Moderate || after.Status > GHThermalStatus.Moderate)
            {
                t.Throttled = true;
                t.GateSignal = "status";
                t.ThrottleReason = "thermal status " + GHThermalProbe.StatusName(before.Status)
                    + " before, " + GHThermalProbe.StatusName(after.Status) + " after";
                return;
            }

            bool beforeLow = !float.IsNaN(before.CpuPerformancePct) && before.CpuPerformancePct < ThrottledCpuPerformancePct;
            bool afterLow = !float.IsNaN(after.CpuPerformancePct) && after.CpuPerformancePct < ThrottledCpuPerformancePct;
            if (beforeLow || afterLow)
            {
                t.Throttled = true;
                t.GateSignal = "cpuPerformancePct";
                t.ThrottleReason = "CPU performance "
                    + FormatPct(before.CpuPerformancePct) + " before, "
                    + FormatPct(after.CpuPerformancePct) + " after (threshold "
                    + ThrottledCpuPerformancePct.ToString("0", CultureInfo.InvariantCulture) + ")";
            }
        }

        private static string FormatPct(float value)
        {
            return float.IsNaN(value) ? "n/a" : value.ToString("0.#", CultureInfo.InvariantCulture) + "%";
        }

        public string ToJson()
        {
            return JsonConvert.SerializeObject(this, _jsonSettings);
        }

        public void Save(string path)
        {
            File.WriteAllText(path, ToJson() + Environment.NewLine, new UTF8Encoding(false));
        }

        /* The archive directory's perf subdirectory, created when missing */
        public static string DefaultDirectory()
        {
            string ghdir = GHApp.GnollHackService.GetGnollHackPath();
            string archive = Path.Combine(ghdir, GHConstants.ArchiveDirectory);
            if (!Directory.Exists(archive))
                GHApp.CheckCreateDirectory(archive);
            string perf = Path.Combine(archive, "perf");
            if (!Directory.Exists(perf))
                GHApp.CheckCreateDirectory(perf);
            return perf;
        }

        /* Writes run_<timestamp>_<scenario>_<arm>.json into DefaultDirectory and returns
           the path. */
        public string SaveToDefaultDirectory()
        {
            DateTime stamp;
            if (!DateTime.TryParseExact(TimestampUtc, "o", CultureInfo.InvariantCulture,
                DateTimeStyles.RoundtripKind, out stamp))
                stamp = DateTime.UtcNow;
            string name = "run_" + stamp.ToUniversalTime().ToString("yyyyMMdd_HHmmss", CultureInfo.InvariantCulture)
                + "_" + SafeFileToken(Scenario) + "_" + SafeFileToken(Arm) + ".json";
            string path = Path.Combine(DefaultDirectory(), name);
            Save(path);
            return path;
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
    }

    public sealed class GHPerfGitInfo
    {
        [JsonProperty("commit")]
        public string Commit;

        [JsonProperty("dirty")]
        public bool Dirty;

        [JsonProperty("tag")]
        public string Tag;

        [JsonProperty("branch")]
        public string Branch;
    }

    public sealed class GHPerfDeviceInfo
    {
        [JsonProperty("id")]
        public string Id;

        [JsonProperty("model")]
        public string Model;

        [JsonProperty("os")]
        public string Os;
    }

    public sealed class GHPerfDisplayInfo
    {
        [JsonProperty("refreshHz")]
        public double RefreshHz;

        [JsonProperty("vsyncMs")]
        public double VsyncMs;

        [JsonProperty("gpuBackend")]
        public string GpuBackend;
    }

    /* A GHThermalReading in the schema's shape: status by name, NaN fields as null */
    public sealed class GHPerfThermalReadingJson
    {
        [JsonProperty("status")]
        public string Status;

        [JsonProperty("headroomFraction")]
        public double? HeadroomFraction;

        [JsonProperty("batteryTempC")]
        public double? BatteryTempC;

        [JsonProperty("cpuPackageTempC")]
        public double? CpuPackageTempC;

        [JsonProperty("gpuTempC")]
        public double? GpuTempC;

        [JsonProperty("cpuPerformancePct")]
        public double? CpuPerformancePct;

        [JsonProperty("cpuFrequencyMHz")]
        public double? CpuFrequencyMHz;

        [JsonProperty("isCharging")]
        public bool? IsCharging;

        [JsonProperty("isLowPower")]
        public bool? IsLowPower;

        [JsonProperty("powerPlan")]
        public string PowerPlan;

        [JsonProperty("timestampUtc")]
        public string TimestampUtc;

        public static GHPerfThermalReadingJson From(GHThermalReading r)
        {
            GHPerfThermalReadingJson j = new GHPerfThermalReadingJson();
            j.Status = GHThermalProbe.StatusName(r.Status);
            j.HeadroomFraction = NullIfNaN(r.HeadroomFraction);
            j.BatteryTempC = NullIfNaN(r.BatteryTempC);
            j.CpuPackageTempC = null;
            j.GpuTempC = null;
            j.CpuPerformancePct = NullIfNaN(r.CpuPerformancePct);
            j.CpuFrequencyMHz = null;
            j.IsCharging = r.IsCharging;
            j.IsLowPower = r.IsLowPower;
            j.PowerPlan = null;
            j.TimestampUtc = r.TimestampTicks > 0
                ? new DateTime(r.TimestampTicks, DateTimeKind.Utc).ToString("o", CultureInfo.InvariantCulture)
                : null;
            return j;
        }

        private static double? NullIfNaN(float value)
        {
            if (float.IsNaN(value) || float.IsInfinity(value))
                return null;
            return value;
        }
    }

    public sealed class GHPerfThermalInfo
    {
        [JsonProperty("before")]
        public GHPerfThermalReadingJson Before;

        [JsonProperty("after")]
        public GHPerfThermalReadingJson After;

        [JsonProperty("throttled")]
        public bool Throttled;

        [JsonProperty("gateSignal")]
        public string GateSignal = "none";

        [JsonProperty("throttleReason")]
        public string ThrottleReason;

        /* charging, battery, changed (differs between the readings), unknown */
        [JsonProperty("powerState")]
        public string PowerState = "unknown";
    }

    public sealed class GHPerfSeries
    {
        [JsonProperty("kind")]
        public string Kind;

        [JsonProperty("source")]
        public string Source;

        [JsonProperty("column")]
        public string Column;

        [JsonProperty("intervalsMs")]
        public float[] IntervalsMs;

        [JsonProperty("frameDurationsMs")]
        public float[] FrameDurationsMs;

        [JsonProperty("droppedCount")]
        public int DroppedCount;

        [JsonProperty("metrics")]
        public Dictionary<string, double> Metrics = new Dictionary<string, double>();

        [JsonProperty("info")]
        public Dictionary<string, string> Info = new Dictionary<string, string>();
    }

    public sealed class GHPerfMarker
    {
        [JsonProperty("atMs")]
        public double AtMs;

        [JsonProperty("code")]
        public int Code;

        [JsonProperty("label")]
        public string Label;
    }
}

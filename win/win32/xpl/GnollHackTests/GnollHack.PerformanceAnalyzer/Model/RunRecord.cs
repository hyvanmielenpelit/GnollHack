using System.Text.Json;
using System.Text.Json.Serialization;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* One measurement run. This v1 shape is the analyzer's own and has no schema file;
       the in-app (schema v2) record that Load converts from is described by
       DEVEL/performance/schema/run-record.schema.json. A record may carry several
       series: the app's internal FrameTimeProfiler series and an external one from
       PresentMon or gfxinfo, and the in-app smoothness series of a schema v2 record,
       each with its own metrics. */
    public sealed class RunRecord
    {
        public int SchemaVersion { get; set; } = 1;
        public string Id { get; set; }
        public string TimestampUtc { get; set; }
        public string Scenario { get; set; }
        public string ScenarioKind { get; set; } = "gameplay";
        public string Arm { get; set; }
        public string BuildConfiguration { get; set; }
        public GitInfo Git { get; set; } = new GitInfo();
        public string Platform { get; set; }
        public DeviceInfo Device { get; set; } = new DeviceInfo();
        public Dictionary<string, string> Versions { get; set; } = new Dictionary<string, string>();
        public DisplayInfo Display { get; set; } = new DisplayInfo();
        public Dictionary<string, JsonElement> Configuration { get; set; } = new Dictionary<string, JsonElement>();
        public ThermalInfo Thermal { get; set; } = new ThermalInfo();
        public List<Series> Series { get; set; } = new List<Series>();
        public List<Marker> Markers { get; set; } = new List<Marker>();
        public string Notes { get; set; }
        public bool Excluded { get; set; }
        public string ExclusionReason { get; set; }
        public double WarmupSeconds { get; set; }
        public double WindowSeconds { get; set; }

        public Series FindSeries(string kind)
        {
            return Series.FirstOrDefault(s => string.Equals(s.Kind, kind, StringComparison.OrdinalIgnoreCase));
        }

        public static readonly JsonSerializerOptions JsonOptions = new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            PropertyNameCaseInsensitive = true,
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull,
            WriteIndented = true,
            NumberHandling = JsonNumberHandling.AllowNamedFloatingPointLiterals
        };

        /* Reads a run record, or converts an in-app (schema v2) record to one; returns
           null for JSON that is neither, such as a bare env_before.json/env_after.json
           thermal reading sitting alongside the runs in an arm directory */
        public static RunRecord Load(string path)
        {
            string text = File.ReadAllText(path);
            using (JsonDocument doc = JsonDocument.Parse(text))
            {
                JsonElement root = doc.RootElement;
                if (InAppRun.IsInAppRun(root))
                    return InAppRun.FromJson(root, path).ToRunRecord();
                if (root.ValueKind != JsonValueKind.Object
                    || !root.TryGetProperty("series", out JsonElement series)
                    || series.ValueKind != JsonValueKind.Array)
                    return null;
            }
            return JsonSerializer.Deserialize<RunRecord>(text, JsonOptions);
        }

        public void Save(string path)
        {
            string json = JsonSerializer.Serialize(this, JsonOptions);
            File.WriteAllText(path, json + "\r\n", new System.Text.UTF8Encoding(false));
        }
    }

    public sealed class GitInfo
    {
        public string Commit { get; set; }
        public bool Dirty { get; set; }
        public string Tag { get; set; }
        public string Branch { get; set; }
    }

    public sealed class DeviceInfo
    {
        public string Id { get; set; }
        public string Model { get; set; }
        public string Os { get; set; }
    }

    public sealed class DisplayInfo
    {
        public double RefreshHz { get; set; }
        public double VsyncMs { get; set; }
        /* The intended content rate, which may be lower than the refresh rate when the
           game paces itself to fewer frames than the display can show. Defaults to the
           refresh rate when nothing else is specified. */
        public double TargetFps { get; set; }
        public double TargetPeriodMs { get; set; }
        public string GpuBackend { get; set; }
    }

    public sealed class ThermalReading
    {
        public string Status { get; set; }            /* Unknown, Nominal, Light, Moderate, Severe, Critical */
        public double? HeadroomFraction { get; set; }
        public double? BatteryTempC { get; set; }
        public double? CpuPackageTempC { get; set; }
        public double? GpuTempC { get; set; }
        public double? CpuPerformancePct { get; set; }
        public double? CpuFrequencyMHz { get; set; }
        public bool? IsCharging { get; set; }
        public bool? IsLowPower { get; set; }
        public string PowerPlan { get; set; }
        public string TimestampUtc { get; set; }
        public Dictionary<string, JsonElement> Raw { get; set; }
    }

    public sealed class ThermalInfo
    {
        public ThermalReading Before { get; set; }
        public ThermalReading After { get; set; }
        public bool Throttled { get; set; }
        public string GateSignal { get; set; }       /* which signal decided Throttled */
        public string ThrottleReason { get; set; }
        public string PowerState { get; set; }       /* charging, battery, changed, unknown */
    }

    public sealed class Series
    {
        public string Kind { get; set; }              /* "external", "internal" or "smoothness" */
        public string Source { get; set; }            /* "presentmon", "gfxinfo", "FrameTimeProfiler", "in-app" */
        public string Column { get; set; }            /* which column or field produced the intervals */
        public float[] IntervalsMs { get; set; }
        public float[] FrameDurationsMs { get; set; } /* gfxinfo: FrameCompleted - IntendedVsync */
        public int DroppedCount { get; set; }
        public Dictionary<string, double> Metrics { get; set; } = new Dictionary<string, double>();
        public Dictionary<string, string> Info { get; set; } = new Dictionary<string, string>();
    }

    public sealed class Marker
    {
        public double AtMs { get; set; }
        public int Code { get; set; }
        public string Label { get; set; }
    }
}

using System.Text.Json;
using System.Text.Json.Serialization;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* One measurement run. This v1 shape is the analyzer's own and has no schema file;
       the in-app (schema v2) record that Load converts from is described by
       DEVEL/performance/schema/run-record.schema.json. A record may carry several
       series: the app's internal FrameTimeProfiler series and an external one from
       PresentMon or gfxinfo, and the in-app smoothness series of a schema v2 record,
       each with its own metrics.

       Batch names the suite invocation the run belongs to. Fingerprint is the flat
       environment fingerprint (GHEnvironmentFingerprint); a record written before
       fingerprints existed gets one built from its per-field environment on Load.
       Background holds one background-load block per source ("in-app", "external"),
       each in the shape of the in-app record's "background" object. */
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
        public string Batch { get; set; }
        public Dictionary<string, string> Fingerprint { get; set; }
        public List<BackgroundInfo> Background { get; set; }

        /* The worst verdict among the background blocks that is not unknown; Unknown
           when there is none */
        [JsonIgnore]
        public GHBackgroundVerdict BackgroundVerdict
        {
            get
            {
                GHBackgroundVerdict worst = GHBackgroundVerdict.Unknown;
                if (Background != null)
                {
                    foreach (BackgroundInfo b in Background)
                    {
                        if (b != null && b.ParsedVerdict > worst)
                            worst = b.ParsedVerdict;
                    }
                }
                return worst;
            }
        }

        /* The block that decided BackgroundVerdict, preferring the in-app one on a tie;
           otherwise the first block with a CPU signal; null when there is none */
        [JsonIgnore]
        public BackgroundInfo DecisiveBackground
        {
            get
            {
                if (Background == null || Background.Count == 0)
                    return null;
                GHBackgroundVerdict worst = BackgroundVerdict;
                if (worst != GHBackgroundVerdict.Unknown)
                    return Background.Where(b => b != null && b.ParsedVerdict == worst)
                        .OrderBy(b => b.Source == BackgroundInfo.SourceInApp ? 0 : 1).First();
                return Background.FirstOrDefault(b => b?.Window?.OtherCpuP90Pct != null) ?? Background.FirstOrDefault(b => b != null);
            }
        }

        public Series FindSeries(string kind)
        {
            return Series.FirstOrDefault(s => string.Equals(s.Kind, kind, StringComparison.OrdinalIgnoreCase));
        }

        /* Which side recorded the fingerprint: the app ("in-app") or only the host
           script ("script"). The two record some facts under different keys. */
        public const string MetaSourceKey = "meta.source";
        public const string SourceInAppValue = "in-app";
        public const string SourceScriptValue = "script";

        /* GHEnvironmentFingerprint.Diff, except that when the two fingerprints come from
           different sources (or either is a mix), keys only one side holds are left out
           instead of being reported as added or removed */
        public static List<GHFingerprintChange> DiffFingerprints(IDictionary<string, string> a, IDictionary<string, string> b, out bool sourcesDiffer)
        {
            string sa = null, sb = null;
            if (a != null)
                a.TryGetValue(MetaSourceKey, out sa);
            if (b != null)
                b.TryGetValue(MetaSourceKey, out sb);
            sourcesDiffer = sa != null && sb != null
                && (sa != sb || sa == GHEnvironmentFingerprint.MixedValue || sb == GHEnvironmentFingerprint.MixedValue);
            if (!sourcesDiffer || a == null || b == null)
                return GHEnvironmentFingerprint.Diff(a, b);
            Dictionary<string, string> sharedA = new Dictionary<string, string>();
            Dictionary<string, string> sharedB = new Dictionary<string, string>();
            foreach (KeyValuePair<string, string> kv in a)
            {
                string other;
                if (b.TryGetValue(kv.Key, out other))
                {
                    sharedA[kv.Key] = kv.Value;
                    sharedB[kv.Key] = other;
                }
            }
            return GHEnvironmentFingerprint.Diff(sharedA, sharedB);
        }

        /* Gives a record without a fingerprint the one its per-field environment maps to */
        public void NormalizeLegacy()
        {
            if (Fingerprint != null && Fingerprint.Count > 0)
                return;
            Fingerprint = LegacyFingerprint();
        }

        /* The fingerprint GHEnvironmentFingerprint.FromLegacyFields builds from Versions,
           the record's scalar fields and Configuration["environment"] (the in-app
           record's environment object, whose "configuration" object is the toggle
           vector); record fields take precedence over the environment object. */
        public Dictionary<string, string> LegacyFingerprint()
        {
            JsonElement env = default;
            bool hasEnv = Configuration != null && Configuration.TryGetValue("environment", out env) && env.ValueKind == JsonValueKind.Object;
            string E(params string[] names)
            {
                if (!hasEnv)
                    return null;
                foreach (string n in names)
                {
                    foreach (JsonProperty p in env.EnumerateObject())
                    {
                        if (!p.Name.Equals(n, StringComparison.OrdinalIgnoreCase))
                            continue;
                        if (p.Value.ValueKind == JsonValueKind.String)
                            return p.Value.GetString();
                        if (p.Value.ValueKind == JsonValueKind.Number || p.Value.ValueKind == JsonValueKind.True || p.Value.ValueKind == JsonValueKind.False)
                            return p.Value.ToString();
                    }
                }
                return null;
            }
            string V(string key) { return Versions != null && Versions.TryGetValue(key, out string v) && !string.IsNullOrEmpty(v) ? v : null; }

            Dictionary<string, object> toggles = null;
            if (hasEnv && env.TryGetProperty("configuration", out JsonElement cfg) && cfg.ValueKind == JsonValueKind.Object)
            {
                toggles = new Dictionary<string, object>();
                foreach (JsonProperty p in cfg.EnumerateObject())
                    toggles[p.Name] = ToggleValue(p.Value);
            }
            string build = BuildConfiguration != null && BuildConfiguration != "Unknown" ? BuildConfiguration : E("buildConfiguration");
            return GHEnvironmentFingerprint.FromLegacyFields(
                V("app") ?? E("appVersion", "version"),
                Git?.Commit ?? E("gitCommit", "commit"),
                build,
                V("runtime") ?? E("runtimeVersion"),
                V("framework") ?? E("frameworkVersion"),
                V("uiFramework") ?? E("uiFrameworkVersion"),
                V("skiaSharp") ?? E("skiaSharpVersion"),
                V("fmod") ?? E("fmodVersion"),
                Platform ?? E("platform"),
                Device?.Os ?? E("deviceOs", "osVersion"),
                Device?.Model ?? E("deviceModel", "model"),
                E("mapRefreshRateSetting"),
                Display?.GpuBackend ?? E("gpuBackend"),
                toggles);
        }

        private static object ToggleValue(JsonElement v)
        {
            switch (v.ValueKind)
            {
            case JsonValueKind.True:
                return true;
            case JsonValueKind.False:
                return false;
            case JsonValueKind.String:
                return v.GetString();
            case JsonValueKind.Number:
                if (v.TryGetInt64(out long l))
                    return l;
                return v.GetDouble();
            default:
                return null;
            }
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
            RunRecord r = JsonSerializer.Deserialize<RunRecord>(text, JsonOptions);
            r?.NormalizeLegacy();
            return r;
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
        public double? CpuUtilizationPct { get; set; }
        public List<EnvProcess> TopProcesses { get; set; }
        public List<BackgroundActivityInfo> Activities { get; set; }
        public bool? PendingReboot { get; set; }
        public Dictionary<string, JsonElement> Raw { get; set; }
    }

    /* One entry of Get-ThermalState.ps1's topProcesses: cpuPct over the reading, or
       lifetime cpuSeconds when the per-process counter was unavailable */
    public sealed class EnvProcess
    {
        public string Name { get; set; }
        public double? CpuPct { get; set; }
        public double? CpuSeconds { get; set; }
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

    /* One background-load block, the in-app record's "background" object plus its
       source. Unsupported signals are null. Note is the analyzer's own remark on how
       to read the block (the external path's otherGpuPct includes the app itself). */
    public sealed class BackgroundInfo
    {
        public const string SourceInApp = "in-app";
        public const string SourceExternal = "external";

        /* Pre-window: the up to PreWindowSeconds before the window start */
        public const int PreWindowSeconds = 10;
        public const int MaxCpuProcesses = 8;
        public const int MaxGpuProcesses = 3;

        public string Source { get; set; }
        public int SamplerVersion { get; set; }
        public int IntervalMs { get; set; }
        public int LogicalProcessors { get; set; }
        public double? Coverage { get; set; }
        public double? SamplerBusyMs { get; set; }
        public List<string> Signals { get; set; } = new List<string>();
        public BackgroundWindow Window { get; set; }
        public BackgroundWindow PreWindow { get; set; }
        public double? OtherGpuPct { get; set; }
        public List<BackgroundProcess> Processes { get; set; } = new List<BackgroundProcess>();
        public List<BackgroundActivityInfo> Activities { get; set; } = new List<BackgroundActivityInfo>();
        public List<BackgroundSample> Samples { get; set; }
        public string Verdict { get; set; }
        public string Reason { get; set; }
        public string Note { get; set; }

        [JsonIgnore]
        public GHBackgroundVerdict ParsedVerdict { get { return GHBackgroundLoad.ParseVerdict(Verdict); } }

        /* Single-letter verdict: q quiet, e elevated, B busy, ? unknown */
        public static string VerdictLetter(GHBackgroundVerdict v)
        {
            switch (v)
            {
            case GHBackgroundVerdict.Quiet:
                return "q";
            case GHBackgroundVerdict.Elevated:
                return "e";
            case GHBackgroundVerdict.Busy:
                return "B";
            default:
                return "?";
            }
        }

        /* The highest-CPU (else highest-GPU) listed process outside the measurement
           category, or null */
        public BackgroundProcess TopSuspect()
        {
            if (Processes == null)
                return null;
            return Processes
                .Where(p => p != null && GHBackgroundLoad.CategoryOf(p.Name) != GHBackgroundLoad.CategoryMeasurement
                    && p.Category != GHBackgroundLoad.CategoryMeasurement)
                .OrderByDescending(p => Math.Max(p.CpuPct ?? 0, p.GpuPct ?? 0))
                .ThenBy(p => p.Name, StringComparer.Ordinal)
                .FirstOrDefault(p => Math.Max(p.CpuPct ?? 0, p.GpuPct ?? 0) > 0);
        }

        /* A block from a sample series: the window [windowStartTicks, windowEndTicks)
           and the pre-window before it summarized with GHBackgroundLoad.Summarize, the
           verdict drawn by GHBackgroundLoad.Classify from the window, the processes and
           classifyGpuPct (NaN keeps the GPU rules from firing), the samples of both
           ranges with t in seconds from the window start, the top MaxCpuProcesses by CPU
           and then the top MaxGpuProcesses by GPU not already listed. otherGpuPct is
           stored as reported. */
        public static BackgroundInfo FromSamples(string source, GHSystemLoadSample[] samples, long windowStartTicks, long windowEndTicks,
            IList<GHProcessLoad> processes, float otherGpuPct, float classifyGpuPct, int logicalProcessors, IEnumerable<string> signals)
        {
            samples = samples ?? new GHSystemLoadSample[0];
            processes = processes ?? new List<GHProcessLoad>();
            long preStart = windowStartTicks - PreWindowSeconds * TimeSpan.TicksPerSecond;
            GHBackgroundSummary window = GHBackgroundLoad.Summarize(samples, samples.Length, windowStartTicks, windowEndTicks, GHBackgroundLoad.SampleIntervalMs);
            GHBackgroundSummary pre = GHBackgroundLoad.Summarize(samples, samples.Length, preStart, windowStartTicks, GHBackgroundLoad.SampleIntervalMs);
            GHBackgroundVerdict verdict = GHBackgroundLoad.Classify(window, processes, classifyGpuPct, out string reason);

            BackgroundInfo b = new BackgroundInfo
            {
                Source = source,
                SamplerVersion = GHBackgroundLoad.SamplerVersion,
                IntervalMs = GHBackgroundLoad.SampleIntervalMs,
                LogicalProcessors = logicalProcessors,
                Coverage = Round(window.Coverage),
                Window = BackgroundWindow.FromSummary(window),
                PreWindow = BackgroundWindow.FromSummary(pre),
                OtherGpuPct = Round(otherGpuPct),
                Verdict = GHBackgroundLoad.VerdictName(verdict),
                Reason = reason
            };
            if (signals != null)
                b.Signals.AddRange(signals);

            List<GHProcessLoad> byCpu = processes.OrderByDescending(p => float.IsNaN(p.CpuPct) ? -1 : p.CpuPct).ThenBy(p => p.Name, StringComparer.Ordinal).Take(MaxCpuProcesses).ToList();
            List<GHProcessLoad> byGpu = processes.Where(p => !float.IsNaN(p.GpuPct) && p.GpuPct > 0 && !byCpu.Any(c => c.Name == p.Name))
                .OrderByDescending(p => p.GpuPct).ThenBy(p => p.Name, StringComparer.Ordinal).Take(MaxGpuProcesses).ToList();
            foreach (GHProcessLoad p in byCpu.Concat(byGpu))
                b.Processes.Add(new BackgroundProcess { Name = p.Name, CpuPct = Round(p.CpuPct), GpuPct = Round(p.GpuPct), Category = p.Category ?? GHBackgroundLoad.CategoryOf(p.Name) });
            foreach (GHBackgroundActivity act in GHBackgroundLoad.BuildActivities(processes))
                b.Activities.Add(new BackgroundActivityInfo { Category = act.Category, Processes = new List<string>(act.Processes), CpuPct = Round(act.CpuPct) });

            b.Samples = new List<BackgroundSample>();
            foreach (GHSystemLoadSample s in samples.Where(x => x.TimestampTicks >= preStart && x.TimestampTicks < windowEndTicks).OrderBy(x => x.TimestampTicks))
            {
                b.Samples.Add(new BackgroundSample
                {
                    T = Math.Round((s.TimestampTicks - windowStartTicks) / (double)TimeSpan.TicksPerSecond, 3),
                    Sys = Round(s.SystemCpuPct),
                    Own = Round(s.OwnCpuPct),
                    Other = Round(s.OtherCpuPct),
                    Disk = Round(s.DiskBusyPct),
                    AvailPct = Round(s.AvailableMemoryPct),
                    Faults = Round(s.HardFaultsPerSec)
                });
            }
            return b;
        }

        /* A float as a JSON number rounded to 3 decimals, null for NaN or infinity */
        public static double? Round(float v)
        {
            if (float.IsNaN(v) || float.IsInfinity(v))
                return null;
            return Math.Round((double)v, 3);
        }
    }

    /* The "window" and "preWindow" objects of a background block */
    public sealed class BackgroundWindow
    {
        public int Samples { get; set; }
        public double? OtherCpuMeanPct { get; set; }
        public double? OtherCpuP90Pct { get; set; }
        public double? OtherCpuMaxPct { get; set; }
        public double? OtherCpuSpikeShare { get; set; }
        public double? DiskBusyMeanPct { get; set; }
        public double? DiskBusyP90Pct { get; set; }
        public double? AvailableMemoryMinPct { get; set; }
        public long? AvailableMemoryMinMB { get; set; }
        public double? HardFaultsP90PerSec { get; set; }
        public int MemoryPressureEvents { get; set; }
        public bool LowMemory { get; set; }

        public static BackgroundWindow FromSummary(GHBackgroundSummary s)
        {
            if (s == null)
                return null;
            return new BackgroundWindow
            {
                Samples = s.Samples,
                OtherCpuMeanPct = BackgroundInfo.Round(s.OtherCpuMeanPct),
                OtherCpuP90Pct = BackgroundInfo.Round(s.OtherCpuP90Pct),
                OtherCpuMaxPct = BackgroundInfo.Round(s.OtherCpuMaxPct),
                OtherCpuSpikeShare = BackgroundInfo.Round(s.OtherCpuSpikeShare),
                DiskBusyMeanPct = BackgroundInfo.Round(s.DiskBusyMeanPct),
                DiskBusyP90Pct = BackgroundInfo.Round(s.DiskBusyP90Pct),
                AvailableMemoryMinPct = BackgroundInfo.Round(s.AvailableMemoryMinPct),
                AvailableMemoryMinMB = s.AvailableMemoryMinMB >= 0 ? s.AvailableMemoryMinMB : null,
                HardFaultsP90PerSec = BackgroundInfo.Round(s.HardFaultsP90PerSec),
                MemoryPressureEvents = s.MemoryPressureEvents,
                LowMemory = s.LowMemory
            };
        }

        /* The summary this object describes, with the block's top-level coverage */
        public GHBackgroundSummary ToSummary(double? coverage)
        {
            GHBackgroundSummary s = new GHBackgroundSummary
            {
                Samples = Samples,
                OtherCpuMeanPct = F(OtherCpuMeanPct),
                OtherCpuP90Pct = F(OtherCpuP90Pct),
                OtherCpuMaxPct = F(OtherCpuMaxPct),
                OtherCpuSpikeShare = F(OtherCpuSpikeShare),
                DiskBusyMeanPct = F(DiskBusyMeanPct),
                DiskBusyP90Pct = F(DiskBusyP90Pct),
                AvailableMemoryMinPct = F(AvailableMemoryMinPct),
                AvailableMemoryMinMB = AvailableMemoryMinMB ?? -1,
                HardFaultsP90PerSec = F(HardFaultsP90PerSec),
                MemoryPressureEvents = MemoryPressureEvents,
                LowMemory = LowMemory
            };
            if (coverage.HasValue)
                s.Coverage = (float)coverage.Value;
            return s;
        }

        private static float F(double? v) { return v.HasValue ? (float)v.Value : float.NaN; }
    }

    public sealed class BackgroundProcess
    {
        public string Name { get; set; }
        public double? CpuPct { get; set; }
        public double? GpuPct { get; set; }
        public string Category { get; set; }
    }

    /* A known activity: its category, process names and summed CPU. Also the shape of
       Get-ThermalState.ps1's "activities" entries. */
    public sealed class BackgroundActivityInfo
    {
        public string Category { get; set; }
        public List<string> Processes { get; set; } = new List<string>();
        public double? CpuPct { get; set; }
    }

    /* One sample of a background block; t is seconds from the window start */
    public sealed class BackgroundSample
    {
        [JsonPropertyName("t")]
        public double T { get; set; }
        [JsonPropertyName("sys")]
        public double? Sys { get; set; }
        [JsonPropertyName("own")]
        public double? Own { get; set; }
        [JsonPropertyName("other")]
        public double? Other { get; set; }
        [JsonPropertyName("disk")]
        public double? Disk { get; set; }
        [JsonPropertyName("availPct")]
        public double? AvailPct { get; set; }
        [JsonPropertyName("faults")]
        public double? Faults { get; set; }

        /* Other, or system minus own when the sample carries no other value */
        [JsonIgnore]
        public double? OtherOrDerived
        {
            get
            {
                if (Other.HasValue)
                    return Other;
                if (Sys.HasValue && Own.HasValue)
                    return Math.Max(0, Sys.Value - Own.Value);
                return null;
            }
        }
    }
}

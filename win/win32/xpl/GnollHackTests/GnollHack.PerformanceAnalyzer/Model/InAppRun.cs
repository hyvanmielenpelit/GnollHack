using System.Globalization;
using System.Text.Json;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* A frame the user marked as a felt stutter, from the run record's "marks" array */
    public sealed class RunMark
    {
        public long FrameId;
        public string UtcText;
        public DateTime? Utc;               /* null when "utc" is absent or does not parse */
        public double MsFromWindowStart;    /* NaN when absent */
    }

    /* An in-app run record, schema version 2, as the app writes it at the end of a
       measurement window:

         { schemaVersion, runId, scenario, arm, startedUtc, endedUtc, windowSeconds,
           environment {...}, thermal { before, after },
           clock { stopwatchFrequency, firstAnchor, latestAnchor },
           display { measuredRefreshHz, assumedRefreshHz, targetFps, assumedRefreshMismatch },
           smoothness { displayedFps, hitchRatioMsPerSec, ..., causes { <CauseName>: { count, hitchMs } } },
           onScreenPacing {...}, series { onScreenIntervalsMs, pacingErrorMs },
           uiThread {...}, files { frameTimeline, compositorFrames },
           suite { suiteId, runIndex, pageMode, isWarmUp, replayFileName, replayBytes,
                   replaySha256, startTurn, turnReached, excludedReason },
           marks [ { frameId, utc, msFromWindowStart } ] }

       The files named under "files" sit next to the JSON. The "suite" block is written
       only for runs taken by the in-app Performance Suite; its optional excludedReason
       (e.g. "warm-up run", "replay ended") overrides the analyzer's own exclusion rules.
       "marks" lists the frames the user marked as felt stutters and is absent when there
       are none. Parsing is tolerant: a missing member reads as zero or null. */
    public sealed class InAppRun
    {
        public int SchemaVersion;
        public string SourcePath;
        public string RunId;
        public string Scenario;
        public string Arm;
        public string StartedUtc;
        public string EndedUtc;
        public double WindowSeconds;
        public JsonElement Environment;
        public ThermalReading ThermalBefore;
        public ThermalReading ThermalAfter;
        public long StopwatchFrequency;
        public GHClockAnchor? FirstAnchor;
        public GHClockAnchor? LatestAnchor;
        public double MeasuredRefreshHz;
        public double AssumedRefreshHz;
        public double TargetFps;
        public bool AssumedRefreshMismatch;
        public Dictionary<string, double> Smoothness = new Dictionary<string, double>(StringComparer.OrdinalIgnoreCase);
        public string PresentSource;
        public Dictionary<string, (int Count, double HitchMs)> Causes = new Dictionary<string, (int, double)>(StringComparer.OrdinalIgnoreCase);
        public Dictionary<string, double> OnScreenPacing = new Dictionary<string, double>(StringComparer.OrdinalIgnoreCase);
        public float[] OnScreenIntervalsMs = new float[0];
        public float[] PacingErrorMs = new float[0];
        public string FrameTimelineFile;
        public string CompositorFramesFile;
        public JsonElement Suite;
        public string SuiteExcludedReason;
        public List<RunMark> Marks = new List<RunMark>();

        /* A v2 in-app record, as opposed to a run record in the analyzer's own schema */
        public static bool IsInAppRun(JsonElement root)
        {
            return root.ValueKind == JsonValueKind.Object
                && root.TryGetProperty("schemaVersion", out JsonElement v) && v.ValueKind == JsonValueKind.Number
                && v.TryGetInt32(out int n) && n >= 2
                && root.TryGetProperty("smoothness", out JsonElement s) && s.ValueKind == JsonValueKind.Object;
        }

        public static InAppRun Load(string path)
        {
            using JsonDocument doc = JsonDocument.Parse(File.ReadAllText(path));
            if (!IsInAppRun(doc.RootElement))
                throw new InvalidDataException("not an in-app run record (schemaVersion 2 with a smoothness block): " + path);
            return FromJson(doc.RootElement, path);
        }

        public static InAppRun FromJson(JsonElement root, string path)
        {
            InAppRun r = new InAppRun { SourcePath = path };
            r.SchemaVersion = (int)Num(root, "schemaVersion");
            r.RunId = Str(root, "runId");
            r.Scenario = Str(root, "scenario");
            r.Arm = Str(root, "arm");
            r.StartedUtc = Str(root, "startedUtc");
            r.EndedUtc = Str(root, "endedUtc");
            r.WindowSeconds = Num(root, "windowSeconds");
            if (root.TryGetProperty("environment", out JsonElement env) && env.ValueKind == JsonValueKind.Object)
                r.Environment = env.Clone();
            if (root.TryGetProperty("thermal", out JsonElement th) && th.ValueKind == JsonValueKind.Object)
            {
                if (th.TryGetProperty("before", out JsonElement b) && b.ValueKind == JsonValueKind.Object)
                    r.ThermalBefore = EnvJson.FromElement(b);
                if (th.TryGetProperty("after", out JsonElement a) && a.ValueKind == JsonValueKind.Object)
                    r.ThermalAfter = EnvJson.FromElement(a);
            }
            if (root.TryGetProperty("clock", out JsonElement clock) && clock.ValueKind == JsonValueKind.Object)
            {
                r.StopwatchFrequency = (long)Num(clock, "stopwatchFrequency");
                r.FirstAnchor = Anchor(clock, "firstAnchor");
                r.LatestAnchor = Anchor(clock, "latestAnchor");
            }
            if (root.TryGetProperty("display", out JsonElement display) && display.ValueKind == JsonValueKind.Object)
            {
                r.MeasuredRefreshHz = Num(display, "measuredRefreshHz");
                r.AssumedRefreshHz = Num(display, "assumedRefreshHz");
                r.TargetFps = Num(display, "targetFps");
                r.AssumedRefreshMismatch = Bool(display, "assumedRefreshMismatch");
            }
            if (root.TryGetProperty("smoothness", out JsonElement sm) && sm.ValueKind == JsonValueKind.Object)
            {
                foreach (JsonProperty p in sm.EnumerateObject())
                {
                    if (p.NameEquals("causes") && p.Value.ValueKind == JsonValueKind.Object)
                    {
                        foreach (JsonProperty c in p.Value.EnumerateObject())
                            r.Causes[c.Name] = ((int)Num(c.Value, "count"), Num(c.Value, "hitchMs"));
                    }
                    else if (p.NameEquals("presentSource"))
                    {
                        r.PresentSource = p.Value.ValueKind == JsonValueKind.String ? p.Value.GetString() : p.Value.ToString();
                    }
                    else if (TryNumber(p.Value, out double d))
                    {
                        r.Smoothness[p.Name] = d;
                    }
                }
                if (r.MeasuredRefreshHz <= 0)
                    r.MeasuredRefreshHz = r.Smoothness.GetValueOrDefault("measuredRefreshHz");
                if (r.AssumedRefreshHz <= 0)
                    r.AssumedRefreshHz = r.Smoothness.GetValueOrDefault("assumedRefreshHz");
                if (r.TargetFps <= 0)
                    r.TargetFps = r.Smoothness.GetValueOrDefault("targetFps");
            }
            if (root.TryGetProperty("onScreenPacing", out JsonElement osp) && osp.ValueKind == JsonValueKind.Object)
            {
                foreach (JsonProperty p in osp.EnumerateObject())
                {
                    if (TryNumber(p.Value, out double d))
                        r.OnScreenPacing[p.Name] = d;
                }
            }
            if (root.TryGetProperty("series", out JsonElement series) && series.ValueKind == JsonValueKind.Object)
            {
                r.OnScreenIntervalsMs = Floats(series, "onScreenIntervalsMs");
                r.PacingErrorMs = Floats(series, "pacingErrorMs");
            }
            if (root.TryGetProperty("files", out JsonElement files) && files.ValueKind == JsonValueKind.Object)
            {
                r.FrameTimelineFile = Str(files, "frameTimeline");
                r.CompositorFramesFile = Str(files, "compositorFrames");
            }
            if (root.TryGetProperty("suite", out JsonElement suite) && suite.ValueKind == JsonValueKind.Object)
            {
                r.Suite = suite.Clone();
                r.SuiteExcludedReason = Str(suite, "excludedReason");
            }
            if (root.TryGetProperty("marks", out JsonElement marks) && marks.ValueKind == JsonValueKind.Array)
            {
                foreach (JsonElement m in marks.EnumerateArray())
                {
                    if (m.ValueKind != JsonValueKind.Object || !m.TryGetProperty("frameId", out JsonElement fid) || !TryNumber(fid, out double frameId))
                        continue;
                    RunMark mark = new RunMark { FrameId = (long)frameId, UtcText = Str(m, "utc") };
                    mark.Utc = FrameTimelineCsv.ParseUtc(mark.UtcText);
                    mark.MsFromWindowStart = m.TryGetProperty("msFromWindowStart", out JsonElement w) && TryNumber(w, out double wms) ? wms : double.NaN;
                    r.Marks.Add(mark);
                }
            }
            return r;
        }

        public string Directory
        {
            get { return System.IO.Path.GetDirectoryName(System.IO.Path.GetFullPath(SourcePath)) ?? "."; }
        }

        /* The named file next to the JSON, or null when the record names none */
        public string ResolveFile(string name)
        {
            return string.IsNullOrEmpty(name) ? null : System.IO.Path.Combine(Directory, name);
        }

        public string EnvString(params string[] names)
        {
            if (Environment.ValueKind != JsonValueKind.Object)
                return null;
            foreach (string n in names)
            {
                foreach (JsonProperty p in Environment.EnumerateObject())
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

        /* The run in the analyzer's run-record schema, with the in-app metrics as a
           "smoothness" series: the decision metrics are read as the app reported them,
           and the classic pacing metrics are computed from onScreenIntervalsMs. The
           environment keys read are assumptions about the app's naming and fall back to
           null when absent. */
        public RunRecord ToRunRecord()
        {
            RunRecord r = new RunRecord
            {
                SchemaVersion = 1,
                Id = RunId ?? System.IO.Path.GetFileNameWithoutExtension(SourcePath),
                TimestampUtc = StartedUtc,
                Scenario = Scenario ?? "unspecified",
                ScenarioKind = EnvString("scenarioKind") ?? "gameplay",
                Arm = Arm ?? "A",
                BuildConfiguration = EnvString("buildConfiguration", "configuration", "build") ?? "Unknown",
                Platform = EnvString("platform", "os"),
                WindowSeconds = WindowSeconds
            };
            r.Git.Commit = EnvString("commit", "gitCommit");
            r.Git.Tag = EnvString("tag", "gitTag");
            r.Git.Branch = EnvString("branch", "gitBranch");
            r.Device.Id = EnvString("deviceId");
            r.Device.Model = EnvString("deviceModel", "model");
            r.Device.Os = EnvString("osVersion", "deviceOs");
            string version = EnvString("appVersion", "version");
            if (version != null)
                r.Versions["app"] = version;
            AddVersion(r, "runtime", EnvString("runtimeVersion"));
            AddVersion(r, "framework", EnvString("frameworkVersion"));
            AddVersion(r, "uiFramework", EnvString("uiFrameworkVersion"));
            AddVersion(r, "skiaSharp", EnvString("skiaSharpVersion"));
            AddVersion(r, "fmod", EnvString("fmodVersion"));
            r.Display.RefreshHz = MeasuredRefreshHz > 0 ? MeasuredRefreshHz : AssumedRefreshHz;
            r.Display.VsyncMs = r.Display.RefreshHz > 0 ? 1000.0 / r.Display.RefreshHz : 0;
            r.Display.TargetFps = TargetFps > 0 ? TargetFps : r.Display.RefreshHz;
            r.Display.TargetPeriodMs = r.Display.TargetFps > 0 ? 1000.0 / r.Display.TargetFps : r.Display.VsyncMs;
            if (Environment.ValueKind == JsonValueKind.Object)
                r.Configuration["environment"] = Environment.Clone();
            r.Thermal.Before = ThermalBefore;
            r.Thermal.After = ThermalAfter;
            ThermalGate.Apply(r);

            int onScreenCount = OnScreenIntervalsMs?.Length ?? 0;
            if (!r.Excluded && onScreenCount < 100)
            {
                r.Excluded = true;
                r.ExclusionReason = "fewer than 100 on-screen intervals (" + onScreenCount + ")";
            }
            if (!string.IsNullOrEmpty(SuiteExcludedReason))
            {
                r.Excluded = true;
                r.ExclusionReason = SuiteExcludedReason;
            }

            Series s = new Series
            {
                Kind = MetricNames.SmoothnessSeries,
                Source = "in-app",
                Column = "onScreenIntervalsMs",
                IntervalsMs = OnScreenIntervalsMs ?? new float[0]
            };
            MetricsComputer.Fill(s, r.Display.TargetPeriodMs, r.Display.VsyncMs);
            foreach (KeyValuePair<string, double> kv in Smoothness)
                s.Metrics[kv.Key] = kv.Value;
            s.DroppedCount = (int)Smoothness.GetValueOrDefault(MetricNames.DroppedCount);
            s.Info["sourceSchemaVersion"] = SchemaVersion.ToString(CultureInfo.InvariantCulture);
            if (PresentSource != null)
                s.Info["presentSource"] = PresentSource;
            r.Series.Add(s);
            return r;
        }

        private static void AddVersion(RunRecord r, string key, string value)
        {
            if (!string.IsNullOrEmpty(value))
                r.Versions[key] = value;
        }

        private static GHClockAnchor? Anchor(JsonElement e, string name)
        {
            if (!e.TryGetProperty(name, out JsonElement a) || a.ValueKind != JsonValueKind.Object)
                return null;
            GHClockAnchor x;
            x.PlatformNanos = (long)Num(a, "platformNanos");
            x.StopwatchTicks = (long)Num(a, "stopwatchTicks");
            return x;
        }

        private static bool TryNumber(JsonElement v, out double d)
        {
            d = 0;
            switch (v.ValueKind)
            {
            case JsonValueKind.Number:
                return v.TryGetDouble(out d);
            case JsonValueKind.True:
                d = 1;
                return true;
            case JsonValueKind.False:
                d = 0;
                return true;
            case JsonValueKind.String:
                return double.TryParse(v.GetString(), NumberStyles.Float, CultureInfo.InvariantCulture, out d);
            default:
                return false;
            }
        }

        private static double Num(JsonElement e, string name)
        {
            return e.TryGetProperty(name, out JsonElement v) && TryNumber(v, out double d) ? d : 0;
        }

        private static bool Bool(JsonElement e, string name)
        {
            return Num(e, name) != 0;
        }

        private static string Str(JsonElement e, string name)
        {
            return e.TryGetProperty(name, out JsonElement v) && v.ValueKind == JsonValueKind.String ? v.GetString() : null;
        }

        private static float[] Floats(JsonElement e, string name)
        {
            if (!e.TryGetProperty(name, out JsonElement v) || v.ValueKind != JsonValueKind.Array)
                return new float[0];
            List<float> list = new List<float>(v.GetArrayLength());
            foreach (JsonElement x in v.EnumerateArray())
            {
                if (TryNumber(x, out double d))
                    list.Add((float)d);
            }
            return list.ToArray();
        }
    }
}

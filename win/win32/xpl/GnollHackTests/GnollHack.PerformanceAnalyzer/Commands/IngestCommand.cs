using System.Globalization;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Commands
{
    /* Turns an external capture plus its environment readings into a run record:

         ingest --presentmon <csv> [--process <exe name>] ... --out <run.json>
         ingest --gfxinfo <dir with framestats_*.txt> ... --out <run.json>

       Common options: --arm, --scenario, --scenario-kind, --platform, --device-id,
       --device-model, --device-os, --build-config, --refresh-hz, --target-fps,
       --env-before <json>, --env-after <json>, --commit, --tag, --dirty, --warmup <s>,
       --window <s>, --notes, --batch <id>, --fingerprint <json> (repeatable),
       --env-during-system <load_system.csv>, --env-during-processes <load_processes.csv>,
       --env-during-android <load_android.txt>. Warm-up trimming is done here: the first
       --warmup seconds of the series are dropped so that the record holds only the
       measurement window. --target-fps is the intended content rate, which may be lower
       than the refresh rate when the game paces itself; it defaults to --refresh-hz.

       --run-json <in-app run record, schema v2> takes the run's identity, scenario, arm,
       environment, versions, fingerprint, batch (suite.suiteId), notes, background
       block, thermal readings and display rates from the app's own record where the
       options above do not give them, and adds its smoothness series next to the
       external one, so that one record carries both.

       --fingerprint files (Get-EnvironmentFingerprint.ps1) fill the keys the in-app
       fingerprint lacks; their meta.* keys are not taken into an in-app fingerprint
       that has no meta.fingerprintVersion, so that a legacy in-app fingerprint keeps
       diffing as Unknown against versioned ones. Keys the record's own fields map to
       (GHEnvironmentFingerprint.FromLegacyFields) fill what is still missing.

       The --env-during-* files give the "external" background block. The sampler starts
       with the warm-up, so the window is taken as [first sample + --warmup, + --window)
       (to the last sample when --window is 0); the start of the first sample
       approximates the start of the capture. On Windows own CPU is --process's counter
       over the logical processors (hardware.logicalProcessors of the fingerprint, else
       this machine's count), and available memory percent needs hardware.memoryGB.
       load_processes.csv averages cover the whole capture, warm-up included. Its GPU
       figure includes the app itself, so it is stored as otherGpuPct with a note and
       never fires a GPU rule.

       Exclusion, first match wins: the in-app suite's excludedReason, throttled (unless
       --include-throttled), power state changed, a busy background verdict of any
       block, fewer than 100 intervals after the warm-up trim. */
    public static class IngestCommand
    {
        public const string ExternalGpuNote = "otherGpuPct is the busiest adapter's 3D engine total including the app itself; informational only, no GPU rule applied";

        public static int Run(Args a)
        {
            string outPath = a.Require("out");
            InAppRun inApp = a.Has("run-json") ? InAppRun.Load(a.Require("run-json")) : null;
            RunRecord fromApp = inApp?.ToRunRecord();
            RunRecord r = new RunRecord
            {
                Id = a.Get("id") ?? fromApp?.Id ?? Guid.NewGuid().ToString("N"),
                TimestampUtc = a.Get("timestamp") ?? fromApp?.TimestampUtc ?? DateTime.UtcNow.ToString("o", CultureInfo.InvariantCulture),
                Scenario = a.Get("scenario") ?? fromApp?.Scenario ?? "unspecified",
                ScenarioKind = a.Get("scenario-kind") ?? fromApp?.ScenarioKind ?? "gameplay",
                Arm = a.Get("arm") ?? fromApp?.Arm ?? "A",
                BuildConfiguration = a.Get("build-config") ?? (fromApp?.BuildConfiguration != "Unknown" ? fromApp?.BuildConfiguration : null) ?? "Unknown",
                Platform = a.Get("platform") ?? fromApp?.Platform ?? (a.Has("gfxinfo") ? "Android" : "Windows"),
                Notes = JoinNotes(a.Get("notes"), fromApp?.Notes),
                WarmupSeconds = a.GetDouble("warmup", 0),
                WindowSeconds = a.GetDouble("window", 0),
                Batch = a.Get("batch") ?? fromApp?.Batch
            };
            r.Git.Commit = a.Get("commit") ?? fromApp?.Git.Commit;
            r.Git.Tag = a.Get("tag") ?? fromApp?.Git.Tag;
            r.Git.Branch = a.Get("branch") ?? fromApp?.Git.Branch;
            r.Git.Dirty = a.Has("dirty");
            r.Device.Id = a.Get("device-id") ?? fromApp?.Device.Id;
            r.Device.Model = a.Get("device-model") ?? fromApp?.Device.Model;
            r.Device.Os = a.Get("device-os") ?? fromApp?.Device.Os;
            r.Display.RefreshHz = a.GetDouble("refresh-hz", fromApp != null && fromApp.Display.RefreshHz > 0 ? fromApp.Display.RefreshHz : 60);
            r.Display.VsyncMs = r.Display.RefreshHz > 0 ? 1000.0 / r.Display.RefreshHz : 0;
            r.Display.TargetFps = a.GetDouble("target-fps", fromApp != null && fromApp.Display.TargetFps > 0 ? fromApp.Display.TargetFps : r.Display.RefreshHz);
            r.Display.TargetPeriodMs = r.Display.TargetFps > 0 ? 1000.0 / r.Display.TargetFps : r.Display.VsyncMs;
            if (fromApp != null)
            {
                foreach (KeyValuePair<string, string> kv in fromApp.Versions)
                    r.Versions[kv.Key] = kv.Value;
            }
            foreach (KeyValuePair<string, string> kv in a.Prefixed("version-"))
                r.Versions[kv.Key] = kv.Value;
            foreach (KeyValuePair<string, string> kv in a.Prefixed("config-"))
                r.Configuration[kv.Key] = System.Text.Json.JsonSerializer.SerializeToElement(kv.Value);

            r.Thermal.Before = EnvJson.Read(a.Get("env-before")) ?? fromApp?.Thermal.Before;
            r.Thermal.After = EnvJson.Read(a.Get("env-after")) ?? fromApp?.Thermal.After;
            ThermalGate.Apply(r);

            Series s;
            if (a.Has("presentmon"))
                s = PresentMonCsv.Read(a.Get("presentmon"), a.Get("process"));
            else if (a.Has("gfxinfo"))
            {
                string p = a.Get("gfxinfo");
                s = Directory.Exists(p) ? GfxinfoFramestats.ReadDirectory(p) : GfxinfoFramestats.ReadFile(p);
            }
            else
                throw new ArgumentException("ingest needs --presentmon <csv> or --gfxinfo <dir|file>");

            if (r.WarmupSeconds > 0)
                s.IntervalsMs = TrimWarmup(s.IntervalsMs, r.WarmupSeconds, out int trimmed, ref s);
            if (r.WindowSeconds > 0)
                s.IntervalsMs = TrimToWindow(s.IntervalsMs, r.WindowSeconds, ref s);
            MetricsComputer.Fill(s, r.Display.TargetPeriodMs, r.Display.VsyncMs);
            r.Series.Add(s);
            if (fromApp != null)
            {
                r.Series.Add(fromApp.FindSeries(MetricNames.SmoothnessSeries));
                if (fromApp.Configuration.TryGetValue("environment", out System.Text.Json.JsonElement env))
                    r.Configuration["environment"] = env;
            }

            r.Fingerprint = BuildFingerprint(inApp, fromApp, a.GetAll("fingerprint"), r);

            List<BackgroundInfo> background = new List<BackgroundInfo>();
            if (fromApp?.Background != null)
                background.AddRange(fromApp.Background);
            BackgroundInfo external = BuildExternalBackground(a.Get("env-during-system"), a.Get("env-during-processes"),
                a.Get("env-during-android"), a.Get("process"), r.Fingerprint, r.WarmupSeconds, r.WindowSeconds);
            if (external != null)
                background.Add(external);
            if (background.Count > 0)
                r.Background = background;

            string reason = InAppRun.ExclusionReasonFor(r, inApp?.SuiteExcludedReason, a.Has("include-throttled"), s.IntervalsMs.Length < 100,
                "fewer than 100 intervals after warm-up trim (" + s.IntervalsMs.Length + ")");
            if (reason != null)
            {
                r.Excluded = true;
                r.ExclusionReason = reason;
            }

            r.Save(outPath);
            Console.WriteLine("ingest: " + outPath + "  frames=" + s.IntervalsMs.Length
                + "  P99=" + s.Metrics[MetricNames.FrameDurationP99].ToString("0.00", CultureInfo.InvariantCulture) + " ms"
                + "  hitch=" + s.Metrics[MetricNames.HitchRatio].ToString("0.00", CultureInfo.InvariantCulture) + " ms/s"
                + "  power=" + (r.Thermal.PowerState ?? "unknown")
                + "  background=" + GHBackgroundLoad.VerdictName(r.BackgroundVerdict)
                + (r.Excluded ? "  EXCLUDED: " + r.ExclusionReason : ""));
            return 0;
        }

        private static string JoinNotes(string a, string b)
        {
            if (string.IsNullOrEmpty(a))
                return string.IsNullOrEmpty(b) ? null : b;
            return string.IsNullOrEmpty(b) ? a : a + "; " + b;
        }

        /* The in-app record's fingerprint (its own, or the one mapped from its fields),
           then the --fingerprint files, then the ingested record's own fields, each
           filling only the keys still missing */
        public static Dictionary<string, string> BuildFingerprint(InAppRun inApp, RunRecord fromApp, IList<string> files, RunRecord r)
        {
            Dictionary<string, string> fp = new Dictionary<string, string>();
            if (fromApp?.Fingerprint != null)
                GHEnvironmentFingerprint.MergeMissing(fp, fromApp.Fingerprint);
            bool legacyInApp = inApp != null && !fp.ContainsKey(GHEnvironmentFingerprint.MetaFingerprintVersionKey);
            if (files != null)
            {
                foreach (string f in files)
                {
                    Dictionary<string, string> file = EnvJson.ReadFingerprint(f);
                    if (file == null)
                    {
                        Console.Error.WriteLine("ingest: warning: fingerprint file not found: " + f);
                        continue;
                    }
                    if (legacyInApp)
                    {
                        foreach (string key in file.Keys.Where(k => GHEnvironmentFingerprint.CategoryOf(k) == GHEnvironmentFingerprint.CategoryMeta).ToList())
                            file.Remove(key);
                    }
                    GHEnvironmentFingerprint.MergeMissing(fp, file);
                }
            }
            GHEnvironmentFingerprint.MergeMissing(fp, r.LegacyFingerprint());
            if (!fp.ContainsKey(RunRecord.MetaSourceKey))
                fp[RunRecord.MetaSourceKey] = inApp != null ? RunRecord.SourceInAppValue : RunRecord.SourceScriptValue;
            return fp;
        }

        /* The "external" background block from the --env-during-* files; null when none
           is given or none holds a sample */
        public static BackgroundInfo BuildExternalBackground(string systemCsv, string processesCsv, string androidLog, string processName,
            IDictionary<string, string> fingerprint, double warmupSeconds, double windowSeconds)
        {
            GHSystemLoadSample[] samples = null;
            List<string> signals = new List<string>();
            List<GHProcessLoad> processes = new List<GHProcessLoad>();
            float otherGpuPct = float.NaN;
            int logical = FingerprintInt(fingerprint, "hardware.logicalProcessors");
            string note = null;

            if (!string.IsNullOrEmpty(androidLog) && File.Exists(androidLog))
            {
                AndroidLoad load = AndroidLoadLog.Read(androidLog);
                samples = load.Samples;
                signals.AddRange(load.Signals);
                if (load.Fallback)
                    note = "from top: own CPU unknown, so other CPU is not measured";
            }
            else if (!string.IsNullOrEmpty(systemCsv) && File.Exists(systemCsv))
            {
                if (logical <= 0)
                    logical = Environment.ProcessorCount;
                double memoryGb = FingerprintDouble(fingerprint, "hardware.memoryGB");
                TypeperfTable t = TypeperfCsv.Read(systemCsv);
                samples = TypeperfCsv.ToSystemSamples(t, processName, logical, memoryGb > 0 ? memoryGb * 1024.0 : 0);
                if (samples.Any(x => !float.IsNaN(x.SystemCpuPct)))
                    signals.Add("systemCpu");
                if (samples.Any(x => !float.IsNaN(x.DiskBusyPct)))
                    signals.Add("disk");
                if (samples.Any(x => x.AvailableMemoryMB >= 0))
                    signals.Add("memory");
                if (samples.Any(x => !float.IsNaN(x.HardFaultsPerSec)))
                    signals.Add("hardFaults");
            }
            if (!string.IsNullOrEmpty(processesCsv) && File.Exists(processesCsv))
            {
                if (logical <= 0)
                    logical = Environment.ProcessorCount;
                TypeperfTable t = TypeperfCsv.Read(processesCsv);
                processes = TypeperfCsv.ToProcessLoads(t, processName, logical, out otherGpuPct);
                if (processes.Count > 0)
                    signals.Add("processes");
                if (!float.IsNaN(otherGpuPct))
                {
                    signals.Add("gpu");
                    note = note == null ? ExternalGpuNote : note + "; " + ExternalGpuNote;
                }
            }
            if (samples == null || samples.Length == 0)
            {
                if (samples != null || processes.Count > 0)
                    Console.Error.WriteLine("ingest: warning: the background load files hold no samples; no external background block");
                return null;
            }

            long captureStart = samples.Min(x => x.TimestampTicks);
            long captureEnd = samples.Max(x => x.TimestampTicks);
            long windowStart = captureStart + (long)(Math.Max(0, warmupSeconds) * TimeSpan.TicksPerSecond);
            long windowEnd = windowSeconds > 0
                ? windowStart + (long)(windowSeconds * TimeSpan.TicksPerSecond)
                : captureEnd + GHBackgroundLoad.SampleIntervalMs * TimeSpan.TicksPerMillisecond;
            BackgroundInfo b = BackgroundInfo.FromSamples(BackgroundInfo.SourceExternal, samples, windowStart, windowEnd,
                processes, otherGpuPct, float.NaN, Math.Max(0, logical), signals);
            b.Note = note;
            return b;
        }

        private static int FingerprintInt(IDictionary<string, string> fp, string key)
        {
            return fp != null && fp.TryGetValue(key, out string v)
                && int.TryParse(v, NumberStyles.Integer, CultureInfo.InvariantCulture, out int n) ? n : 0;
        }

        private static double FingerprintDouble(IDictionary<string, string> fp, string key)
        {
            return fp != null && fp.TryGetValue(key, out string v)
                && double.TryParse(v, NumberStyles.Float, CultureInfo.InvariantCulture, out double d) ? d : 0;
        }

        private static float[] TrimWarmup(float[] intervals, double warmupSeconds, out int trimmed, ref Series s)
        {
            double acc = 0;
            int i = 0;
            double limit = warmupSeconds * 1000.0;
            while (i < intervals.Length && acc < limit)
            {
                acc += intervals[i];
                i++;
            }
            trimmed = i;
            s.Info["warmupTrimmedFrames"] = i.ToString(CultureInfo.InvariantCulture);
            float[] rest = new float[intervals.Length - i];
            Array.Copy(intervals, i, rest, 0, rest.Length);
            if (s.FrameDurationsMs != null && s.FrameDurationsMs.Length > i)
            {
                float[] d = new float[s.FrameDurationsMs.Length - i];
                Array.Copy(s.FrameDurationsMs, i, d, 0, d.Length);
                s.FrameDurationsMs = d;
            }
            return rest;
        }

        /* Keeps the run's intervals to --window seconds of playtime, measured from the
           start of the (already warm-up-trimmed) series: the tail past the window is
           dropped so that runs of different lengths compare over the same span. */
        private static float[] TrimToWindow(float[] intervals, double windowSeconds, ref Series s)
        {
            double limit = windowSeconds * 1000.0;
            double acc = 0;
            int kept = 0;
            while (kept < intervals.Length && acc + intervals[kept] <= limit)
            {
                acc += intervals[kept];
                kept++;
            }
            s.Info["windowTrimmedFrames"] = (intervals.Length - kept).ToString(CultureInfo.InvariantCulture);
            float[] rest = new float[kept];
            Array.Copy(intervals, 0, rest, 0, kept);
            if (s.FrameDurationsMs != null && s.FrameDurationsMs.Length > kept)
            {
                float[] d = new float[kept];
                Array.Copy(s.FrameDurationsMs, 0, d, 0, kept);
                s.FrameDurationsMs = d;
            }
            return rest;
        }
    }
}

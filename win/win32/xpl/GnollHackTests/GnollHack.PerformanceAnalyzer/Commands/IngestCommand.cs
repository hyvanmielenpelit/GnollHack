using System.Globalization;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;

namespace GnollHack.PerformanceAnalyzer.Commands
{
    /* Turns an external capture plus its environment readings into a run record:

         ingest --presentmon <csv> [--process <exe name>] ... --out <run.json>
         ingest --gfxinfo <dir with framestats_*.txt> ... --out <run.json>

       Common options: --arm, --scenario, --scenario-kind, --platform, --device-id,
       --device-model, --device-os, --build-config, --refresh-hz, --target-fps,
       --env-before <json>, --env-after <json>, --commit, --tag, --dirty, --warmup <s>,
       --window <s>, --notes. Warm-up trimming is done here: the first --warmup seconds
       of the series are dropped so that the record holds only the measurement window.
       --target-fps is the intended content rate, which may be lower than the refresh
       rate when the game paces itself; it defaults to --refresh-hz.

       --run-json <in-app run record, schema v2> takes the run's identity, scenario, arm,
       environment, thermal readings and display rates from the app's own record where
       the options above do not give them, and adds its smoothness series next to the
       external one, so that one record carries both. */
    public static class IngestCommand
    {
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
                Notes = a.Get("notes"),
                WarmupSeconds = a.GetDouble("warmup", 0),
                WindowSeconds = a.GetDouble("window", 0)
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
            MetricsComputer.Fill(s, r.Display.TargetPeriodMs, r.Display.VsyncMs);
            r.Series.Add(s);
            if (fromApp != null)
            {
                r.Series.Add(fromApp.FindSeries(MetricNames.SmoothnessSeries));
                if (fromApp.Configuration.TryGetValue("environment", out System.Text.Json.JsonElement env))
                    r.Configuration["environment"] = env;
            }

            if (s.IntervalsMs.Length < 100)
            {
                r.Excluded = true;
                r.ExclusionReason = "fewer than 100 intervals after warm-up trim (" + s.IntervalsMs.Length + ")";
            }
            else if (r.Thermal.Throttled && !a.Has("include-throttled"))
            {
                r.Excluded = true;
                r.ExclusionReason = "throttled: " + r.Thermal.ThrottleReason;
            }
            else if (r.Thermal.PowerState == "changed")
            {
                r.Excluded = true;
                r.ExclusionReason = "power state changed during the run (plugged or unplugged)";
            }

            r.Save(outPath);
            Console.WriteLine("ingest: " + outPath + "  frames=" + s.IntervalsMs.Length
                + "  P99=" + s.Metrics[MetricNames.FrameDurationP99].ToString("0.00", CultureInfo.InvariantCulture) + " ms"
                + "  hitch=" + s.Metrics[MetricNames.HitchRatio].ToString("0.00", CultureInfo.InvariantCulture) + " ms/s"
                + "  power=" + (r.Thermal.PowerState ?? "unknown")
                + (r.Excluded ? "  EXCLUDED: " + r.ExclusionReason : ""));
            return 0;
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
    }
}

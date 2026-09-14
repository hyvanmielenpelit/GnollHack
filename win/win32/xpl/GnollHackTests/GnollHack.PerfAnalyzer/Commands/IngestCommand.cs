using System.Globalization;
using GnollHack.PerfAnalyzer.Model;
using GnollHack.PerfAnalyzer.Readers;

namespace GnollHack.PerfAnalyzer.Commands
{
    /* Turns an external capture plus its environment readings into a run record:

         ingest --presentmon <csv> [--process <exe name>] ... --out <run.json>
         ingest --gfxinfo <dir with framestats_*.txt> ... --out <run.json>

       Common options: --arm, --scenario, --scenario-kind, --platform, --device-id,
       --device-model, --device-os, --build-config, --refresh-hz, --env-before <json>,
       --env-after <json>, --commit, --tag, --dirty, --warmup <s>, --window <s>,
       --notes. Warm-up trimming is done here: the first --warmup seconds of the series
       are dropped so that the record holds only the measurement window. */
    public static class IngestCommand
    {
        public static int Run(Args a)
        {
            string outPath = a.Require("out");
            RunRecord r = new RunRecord
            {
                Id = a.Get("id") ?? Guid.NewGuid().ToString("N"),
                TimestampUtc = a.Get("timestamp") ?? DateTime.UtcNow.ToString("o", CultureInfo.InvariantCulture),
                Scenario = a.Get("scenario") ?? "unspecified",
                ScenarioKind = a.Get("scenario-kind") ?? "gameplay",
                Arm = a.Get("arm") ?? "A",
                BuildConfiguration = a.Get("build-config") ?? "Unknown",
                Platform = a.Get("platform") ?? (a.Has("gfxinfo") ? "Android" : "Windows"),
                Notes = a.Get("notes"),
                WarmupSeconds = a.GetDouble("warmup", 0),
                WindowSeconds = a.GetDouble("window", 0)
            };
            r.Git.Commit = a.Get("commit");
            r.Git.Tag = a.Get("tag");
            r.Git.Branch = a.Get("branch");
            r.Git.Dirty = a.Has("dirty");
            r.Device.Id = a.Get("device-id");
            r.Device.Model = a.Get("device-model");
            r.Device.Os = a.Get("device-os");
            r.Display.RefreshHz = a.GetDouble("refresh-hz", 60);
            r.Display.VsyncMs = r.Display.RefreshHz > 0 ? 1000.0 / r.Display.RefreshHz : 0;
            foreach (KeyValuePair<string, string> kv in a.Prefixed("version-"))
                r.Versions[kv.Key] = kv.Value;
            foreach (KeyValuePair<string, string> kv in a.Prefixed("config-"))
                r.Configuration[kv.Key] = System.Text.Json.JsonSerializer.SerializeToElement(kv.Value);

            r.Thermal.Before = EnvJson.Read(a.Get("env-before"));
            r.Thermal.After = EnvJson.Read(a.Get("env-after"));
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
            MetricsComputer.Fill(s, r.Display.VsyncMs);
            r.Series.Add(s);

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

            r.Save(outPath);
            Console.WriteLine("ingest: " + outPath + "  frames=" + s.IntervalsMs.Length
                + "  P99=" + s.Metrics[MetricNames.FrameDurationP99].ToString("0.00", CultureInfo.InvariantCulture) + " ms"
                + "  hitch=" + s.Metrics[MetricNames.HitchRatio].ToString("0.00", CultureInfo.InvariantCulture) + " ms/s"
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

using System.Text;
using System.Text.Json;
using GnollHack.PerfAnalyzer.Model;

namespace GnollHack.PerfAnalyzer.Commands
{
    /* Appends compact lines to DEVEL/perf/history.jsonl:

         history --file <history.jsonl> --append <run.json ...>
         history --file <history.jsonl> --list [--scenario W1] [--platform Windows]

       A history line is the run record without its raw interval arrays, so the file
       stays small and diffable. The file is append-only: this command never rewrites
       it, and refuses a record whose id is already present. Debug-configuration runs are
       refused too; see plan section 2.5. */
    public static class HistoryCommand
    {
        public static int Run(Args a)
        {
            string file = a.Require("file");
            if (a.Has("append"))
                return Append(file, a.GetAll("append"), a.Has("allow-debug"));
            if (a.Has("list"))
                return List(file, a.Get("scenario"), a.Get("platform"));
            Console.Error.WriteLine("history: use --append <run.json ...> or --list");
            return 2;
        }

        private static int Append(string file, List<string> inputs, bool allowDebug)
        {
            HashSet<string> existing = new HashSet<string>(StringComparer.Ordinal);
            if (File.Exists(file))
            {
                foreach (string line in File.ReadLines(file))
                {
                    if (line.Trim().Length == 0)
                        continue;
                    try
                    {
                        using JsonDocument d = JsonDocument.Parse(line);
                        if (d.RootElement.TryGetProperty("id", out JsonElement id))
                            existing.Add(id.GetString());
                    }
                    catch (JsonException) { }
                }
            }
            int written = 0;
            StringBuilder sb = new StringBuilder();
            foreach (string input in inputs)
            {
                IEnumerable<string> files = Directory.Exists(input) ? Directory.GetFiles(input, "*.json") : new[] { input };
                foreach (string f in files)
                {
                    RunRecord r = RunRecord.Load(f);
                    if (r == null)
                        continue;
                    if (existing.Contains(r.Id))
                    {
                        Console.WriteLine("history: already present, skipped " + r.Id);
                        continue;
                    }
                    if (!string.Equals(r.BuildConfiguration, "Release", StringComparison.OrdinalIgnoreCase) && !allowDebug)
                    {
                        Console.WriteLine("history: refused non-Release run " + r.Id + " (" + r.BuildConfiguration + "); pass --allow-debug to override");
                        continue;
                    }
                    foreach (Series s in r.Series)
                    {
                        s.IntervalsMs = null;
                        s.FrameDurationsMs = null;
                    }
                    r.Configuration.Remove("_file");
                    JsonSerializerOptions compact = new JsonSerializerOptions(RunRecord.JsonOptions) { WriteIndented = false };
                    sb.Append(JsonSerializer.Serialize(r, compact)).Append("\r\n");
                    existing.Add(r.Id);
                    written++;
                }
            }
            if (written > 0)
                File.AppendAllText(file, sb.ToString(), new UTF8Encoding(false));
            Console.WriteLine("history: appended " + written + " record(s) to " + file);
            return 0;
        }

        private static int List(string file, string scenario, string platform)
        {
            if (!File.Exists(file))
            {
                Console.WriteLine("history: no file at " + file);
                return 0;
            }
            Console.WriteLine("timestamp             | platform | scenario | arm | build            | P50   | P99   | 1%low | hitch | thr | power");
            foreach (string line in File.ReadLines(file))
            {
                if (line.Trim().Length == 0)
                    continue;
                RunRecord r;
                try { r = JsonSerializer.Deserialize<RunRecord>(line, RunRecord.JsonOptions); }
                catch (JsonException) { continue; }
                if (r == null)
                    continue;
                if (scenario != null && !string.Equals(r.Scenario, scenario, StringComparison.OrdinalIgnoreCase))
                    continue;
                if (platform != null && !string.Equals(r.Platform, platform, StringComparison.OrdinalIgnoreCase))
                    continue;
                Series s = r.FindSeries("external") ?? r.FindSeries("internal") ?? r.Series.FirstOrDefault();
                Dictionary<string, double> m = s?.Metrics ?? new Dictionary<string, double>();
                Console.WriteLine((r.TimestampUtc ?? "").PadRight(21).Substring(0, 21)
                    + " | " + (r.Platform ?? "").PadRight(8)
                    + " | " + (r.Scenario ?? "").PadRight(8)
                    + " | " + (r.Arm ?? "").PadRight(3)
                    + " | " + ((r.Git.Tag ?? r.Git.Commit ?? "?") + " " + r.BuildConfiguration).PadRight(16)
                    + " | " + m.GetValueOrDefault(MetricNames.FrameDurationP50).ToString("0.00").PadLeft(5)
                    + " | " + m.GetValueOrDefault(MetricNames.FrameDurationP99).ToString("0.00").PadLeft(5)
                    + " | " + m.GetValueOrDefault(MetricNames.Fps1PctLow).ToString("0.0").PadLeft(5)
                    + " | " + m.GetValueOrDefault(MetricNames.HitchRatio).ToString("0.00").PadLeft(5)
                    + " | " + (r.Thermal.Throttled ? "yes" : "no ")
                    + " | " + (r.Thermal.PowerState ?? ThermalGate.PowerState(r.Thermal)));
            }
            return 0;
        }
    }
}

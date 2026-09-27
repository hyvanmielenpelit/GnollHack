using System.Text.Json;
using GnollHack.PerformanceAnalyzer.Model;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* Reads the JSON that Get-ThermalState.ps1 emits into a ThermalReading. The script's
       property names are stable. The thermal and power fields, cpuUtilizationPct,
       topProcesses ({ name, cpuPct } or { name, cpuSeconds }), activities
       ({ category, processes, cpuPct }) and pendingReboot are typed; every property,
       these included, is also kept in Raw so that a later analysis can still see what
       the script added (diskBusyPct, availableMemoryMB, loadAverage, cpuinfoError, ...).

       Also reads the flat fingerprint JSON that Get-EnvironmentFingerprint.ps1 emits. */
    public static class EnvJson
    {
        public static ThermalReading Read(string path)
        {
            if (string.IsNullOrEmpty(path) || !File.Exists(path))
                return null;
            using JsonDocument doc = JsonDocument.Parse(File.ReadAllText(path));
            return FromElement(doc.RootElement);
        }

        /* The same reading from an object embedded elsewhere, e.g. an in-app run record's
           thermal.before and thermal.after */
        public static ThermalReading FromElement(JsonElement root)
        {
            ThermalReading t = new ThermalReading();
            t.Status = GetString(root, "status") ?? "Unknown";
            t.HeadroomFraction = GetDouble(root, "headroomFraction");
            t.BatteryTempC = GetDouble(root, "batteryTempC");
            t.CpuPackageTempC = GetDouble(root, "cpuPackageTempC");
            t.GpuTempC = GetDouble(root, "gpuTempC");
            t.CpuPerformancePct = GetDouble(root, "cpuPerformancePct");
            t.CpuFrequencyMHz = GetDouble(root, "cpuFrequencyMHz");
            t.IsCharging = GetBool(root, "isCharging");
            t.IsLowPower = GetBool(root, "isLowPower");
            t.PowerPlan = GetString(root, "powerPlan");
            t.TimestampUtc = GetString(root, "timestampUtc");
            t.CpuUtilizationPct = GetDouble(root, "cpuUtilizationPct");
            t.PendingReboot = GetBool(root, "pendingReboot");
            t.TopProcesses = GetTopProcesses(root);
            t.Activities = GetActivities(root);
            t.Raw = new Dictionary<string, JsonElement>();
            foreach (JsonProperty p in root.EnumerateObject())
                t.Raw[p.Name] = p.Value.Clone();
            return t;
        }

        /* A fingerprint file: one flat object whose values are strings; numbers and
           booleans are taken in their fingerprint form, null and nested values are
           skipped. Null when the path is empty or the file is missing. */
        public static Dictionary<string, string> ReadFingerprint(string path)
        {
            if (string.IsNullOrEmpty(path) || !File.Exists(path))
                return null;
            using JsonDocument doc = JsonDocument.Parse(File.ReadAllText(path));
            return FingerprintFromElement(doc.RootElement);
        }

        public static Dictionary<string, string> FingerprintFromElement(JsonElement root)
        {
            Dictionary<string, string> d = new Dictionary<string, string>();
            if (root.ValueKind != JsonValueKind.Object)
                return d;
            foreach (JsonProperty p in root.EnumerateObject())
            {
                switch (p.Value.ValueKind)
                {
                case JsonValueKind.String:
                    d[p.Name] = p.Value.GetString();
                    break;
                case JsonValueKind.True:
                case JsonValueKind.False:
                    d[p.Name] = GHEnvironmentFingerprint.FormatValue(p.Value.ValueKind == JsonValueKind.True);
                    break;
                case JsonValueKind.Number:
                    d[p.Name] = p.Value.GetRawText();
                    break;
                }
            }
            return d;
        }

        private static List<EnvProcess> GetTopProcesses(JsonElement e)
        {
            if (!e.TryGetProperty("topProcesses", out JsonElement v))
                return null;
            List<EnvProcess> list = new List<EnvProcess>();
            foreach (JsonElement x in Items(v))
            {
                if (x.ValueKind != JsonValueKind.Object)
                    continue;
                list.Add(new EnvProcess
                {
                    Name = GetString(x, "name"),
                    CpuPct = GetDouble(x, "cpuPct"),
                    CpuSeconds = GetDouble(x, "cpuSeconds")
                });
            }
            return list;
        }

        private static List<BackgroundActivityInfo> GetActivities(JsonElement e)
        {
            if (!e.TryGetProperty("activities", out JsonElement v))
                return null;
            List<BackgroundActivityInfo> list = new List<BackgroundActivityInfo>();
            foreach (JsonElement x in Items(v))
            {
                if (x.ValueKind != JsonValueKind.Object)
                    continue;
                BackgroundActivityInfo a = new BackgroundActivityInfo { Category = GetString(x, "category"), CpuPct = GetDouble(x, "cpuPct") };
                if (x.TryGetProperty("processes", out JsonElement procs))
                {
                    foreach (JsonElement p in Items(procs))
                    {
                        if (p.ValueKind == JsonValueKind.String)
                            a.Processes.Add(p.GetString());
                    }
                }
                list.Add(a);
            }
            return list;
        }

        /* An array's items, or a lone value as one item: PowerShell's ConvertTo-Json can
           write a one-element list as its element */
        private static IEnumerable<JsonElement> Items(JsonElement v)
        {
            if (v.ValueKind == JsonValueKind.Array)
            {
                foreach (JsonElement x in v.EnumerateArray())
                    yield return x;
            }
            else if (v.ValueKind != JsonValueKind.Null && v.ValueKind != JsonValueKind.Undefined)
            {
                yield return v;
            }
        }

        private static string GetString(JsonElement e, string name)
        {
            return e.TryGetProperty(name, out JsonElement v) && v.ValueKind == JsonValueKind.String ? v.GetString() : null;
        }

        private static double? GetDouble(JsonElement e, string name)
        {
            if (!e.TryGetProperty(name, out JsonElement v))
                return null;
            if (v.ValueKind == JsonValueKind.Number && v.TryGetDouble(out double d))
                return d;
            return null;
        }

        private static bool? GetBool(JsonElement e, string name)
        {
            if (!e.TryGetProperty(name, out JsonElement v))
                return null;
            if (v.ValueKind == JsonValueKind.True) return true;
            if (v.ValueKind == JsonValueKind.False) return false;
            return null;
        }
    }
}

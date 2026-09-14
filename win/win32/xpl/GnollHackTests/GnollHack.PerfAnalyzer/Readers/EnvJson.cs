using System.Text.Json;
using GnollHack.PerfAnalyzer.Model;

namespace GnollHack.PerfAnalyzer.Readers
{
    /* Reads the JSON that Get-ThermalState.ps1 emits into a ThermalReading. The script's
       property names are stable; anything it adds beyond them is kept in Raw so that a
       later analysis can still see it. */
    public static class EnvJson
    {
        public static ThermalReading Read(string path)
        {
            if (string.IsNullOrEmpty(path) || !File.Exists(path))
                return null;
            using JsonDocument doc = JsonDocument.Parse(File.ReadAllText(path));
            JsonElement root = doc.RootElement;
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
            t.Raw = new Dictionary<string, JsonElement>();
            foreach (JsonProperty p in root.EnumerateObject())
                t.Raw[p.Name] = p.Value.Clone();
            return t;
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

namespace GnollHack.PerfAnalyzer.Model
{
    /* The plan's thermal exclusion rule (section 2.3). A run is flagged throttled when the
       platform reports a status above Moderate on Android or Fair on iOS (both map to
       "Moderate" here), or when the Windows processor performance counter fell below 90
       percent, or, with no better signal, when a thermal reading rose by two classes
       across the run. The gate signal that decided is recorded so that a reader knows
       which sensor spoke. */
    public static class ThermalGate
    {
        public const double WindowsCpuPerformanceFloorPct = 90.0;

        private static readonly string[] Order = { "Unknown", "Nominal", "Light", "Moderate", "Severe", "Critical" };

        public static int Rank(string status)
        {
            if (string.IsNullOrEmpty(status))
                return 0;
            int i = Array.FindIndex(Order, s => s.Equals(status, StringComparison.OrdinalIgnoreCase));
            return i < 0 ? 0 : i;
        }

        /* "charging", "battery", "changed" (state differed between the before and after
           readings) or "unknown". A run whose power state changed mid-window is not
           comparable to anything and is excluded; arms whose power states differ are
           flagged by the comparison, since a charging phone runs hotter and a laptop on
           battery runs a different power plan. */
        public static string PowerState(ThermalInfo t)
        {
            bool? b = t?.Before?.IsCharging, a = t?.After?.IsCharging;
            if (!b.HasValue && !a.HasValue)
                return "unknown";
            if (b.HasValue && a.HasValue && b.Value != a.Value)
                return "changed";
            bool v = b ?? a.Value;
            return v ? "charging" : "battery";
        }

        public static void Apply(RunRecord r)
        {
            ThermalInfo t = r.Thermal;
            if (t == null)
                return;
            t.Throttled = false;
            t.GateSignal = "none";
            t.ThrottleReason = null;
            t.PowerState = PowerState(t);

            ThermalReading b = t.Before, a = t.After;
            int rb = b == null ? 0 : Rank(b.Status);
            int ra = a == null ? 0 : Rank(a.Status);
            if (rb > Rank("Moderate") || ra > Rank("Moderate"))
            {
                t.Throttled = true;
                t.GateSignal = "status";
                t.ThrottleReason = "thermal status " + (b?.Status ?? "?") + " -> " + (a?.Status ?? "?");
                return;
            }
            double? perfB = b?.CpuPerformancePct, perfA = a?.CpuPerformancePct;
            if ((perfB.HasValue && perfB.Value < WindowsCpuPerformanceFloorPct)
                || (perfA.HasValue && perfA.Value < WindowsCpuPerformanceFloorPct))
            {
                t.Throttled = true;
                t.GateSignal = "cpuPerformancePct";
                t.ThrottleReason = "processor performance " + Fmt(perfB) + "% -> " + Fmt(perfA) + "% (floor "
                    + WindowsCpuPerformanceFloorPct + "%)";
                return;
            }
            if (rb > 0 && ra > 0)
            {
                t.GateSignal = "status";
                if (ra - rb >= 2)
                {
                    t.Throttled = true;
                    t.ThrottleReason = "status rose two classes: " + b.Status + " -> " + a.Status;
                }
            }
            else if (perfB.HasValue || perfA.HasValue)
            {
                t.GateSignal = "cpuPerformancePct";
            }
        }

        private static string Fmt(double? v)
        {
            return v.HasValue ? v.Value.ToString("0.#", System.Globalization.CultureInfo.InvariantCulture) : "?";
        }
    }
}

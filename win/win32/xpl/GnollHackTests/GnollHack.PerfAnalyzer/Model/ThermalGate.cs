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

        public static void Apply(RunRecord r)
        {
            ThermalInfo t = r.Thermal;
            if (t == null)
                return;
            t.Throttled = false;
            t.GateSignal = "none";
            t.ThrottleReason = null;

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

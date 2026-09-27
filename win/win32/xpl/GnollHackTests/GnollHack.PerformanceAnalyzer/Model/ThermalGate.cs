using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Model
{
    /* The thermal exclusion rule (see DEVEL/performance/README.md's Protocol section). A
       run is flagged throttled when the platform reports a status above Moderate on
       Android or Fair on iOS (both map to "Moderate" here), or when the Windows
       processor performance counter's after reading is below 90 percent and either
       there is no before reading or the after reading is at least 10 points below it, or,
       with no better signal, when a thermal reading rose by two classes across the run.
       A before reading alone never gates: an idle machine that happened to sample under
       90 percent before the run started is not evidence the run itself throttled. The
       gate signal that decided is recorded so that a reader knows which sensor spoke. */
    public static class ThermalGate
    {
        public const double WindowsCpuPerformanceFloorPct = GHPerformanceComparison.WindowsCpuPerformanceFloorPct;

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
            double? perfB = b?.CpuPerformancePct, perfA = a?.CpuPerformancePct;
            GHThrottleVerdict verdict = GHPerformanceComparison.ClassifyThrottle(rb, ra,
                perfB ?? double.NaN, perfA ?? double.NaN);
            t.Throttled = verdict.Throttled;

            switch (verdict.Signal)
            {
            case GHThrottleSignal.Status:
                t.GateSignal = "status";
                break;
            case GHThrottleSignal.CpuPerformancePct:
                t.GateSignal = "cpuPerformancePct";
                break;
            default:
                t.GateSignal = "none";
                break;
            }

            switch (verdict.Rule)
            {
            case GHThrottleRule.StatusAboveModerate:
                t.ThrottleReason = "thermal status " + (b?.Status ?? "?") + " -> " + (a?.Status ?? "?");
                break;
            case GHThrottleRule.CpuPerformanceDrop:
                string plan = b?.PowerPlan ?? a?.PowerPlan ?? "unknown plan";
                t.ThrottleReason = "processor performance " + Fmt(perfB) + "% -> " + Fmt(perfA) + "% (floor "
                    + WindowsCpuPerformanceFloorPct + "%, " + plan + ")";
                break;
            case GHThrottleRule.StatusRoseTwoClasses:
                t.ThrottleReason = "status rose two classes: " + b.Status + " -> " + a.Status;
                break;
            default:
                t.ThrottleReason = null;
                break;
            }
        }

        private static string Fmt(double? v)
        {
            return v.HasValue ? v.Value.ToString("0.#", System.Globalization.CultureInfo.InvariantCulture) : "?";
        }
    }
}

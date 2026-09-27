using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace GnollHackX.Performance
{
    /* The in-game performance test's diagnosis: the facts gathered around one measurement
       window, the rule catalog that turns them into ranked findings with advice, the
       health class, where the most likely cause lies (outside the game, in device or game
       configuration, or inside the game), and the plain-text report.

       The file must compile under C# 7.3 (the legacy netstandard2.0 project) and must not
       depend on LINQ, JSON, GHApp, GHConstants or any MAUI type. */

    public enum GHDiagnosisHealth
    {
        Inconclusive = 0,
        Healthy = 1,
        Degraded = 2,
        Poor = 3
    }

    public enum GHFindingSeverity
    {
        Info = 0,
        Suspect = 1,
        Likely = 2
    }

    /* Declaration order is the tie-break priority among findings of equal severity. */
    public enum GHFindingArea
    {
        Heat = 0,
        Power = 1,
        Background = 2,
        Memory = 3,
        GpuPath = 4,
        Display = 5,
        Settings = 6,
        AppInternal = 7,
        Instrument = 8
    }

    public enum GHCauseLocation
    {
        None = 0,
        External = 1,       /* heat, power, background load, memory */
        Configuration = 2,  /* GPU path, display, game settings */
        InsideGame = 3,
        Unclear = 4
    }

    public sealed class GHDiagnosisFinding
    {
        public string Code;
        public GHFindingArea Area;
        public GHFindingSeverity Severity;
        public string Title;
        public string Evidence;     /* the measured numbers */
        public string Advice;       /* fixed text per rule */
    }

    /* One row of the Checks table. Status is one of GHPerformanceDiagnosis.StatusOk,
       StatusWarn, StatusBad or StatusNotAvailable. */
    public sealed class GHCheckRow
    {
        public GHFindingArea Area;
        public string Status;
        public string Detail;
    }

    /* The inputs of one diagnosis, as plain fields. A float of NaN, a long of -1, a bool?
       or a reference of null means "unknown": an unknown signal never produces a finding
       and shows as n/a. Thermal ranks are GHThermalStatus values, 0 Unknown to
       5 Critical. */
    public sealed class GHDiagnosisFacts
    {
        /* Window */
        public float WindowSeconds = float.NaN;
        public float TargetFps = float.NaN;
        public float MeasuredRefreshHz = float.NaN;
        public float DisplayMaxRefreshHz = float.NaN;
        public bool AssumedRefreshMismatch;
        public float DisplayedFps = float.NaN;
        public float HitchRatioMsPerSec = float.NaN;
        public int HitchCount;
        public float PacingErrorRmsMs = float.NaN;
        public float PaintP50Ms = float.NaN;
        public float PaintP99Ms = float.NaN;
        public int GcCount;
        public float GcPauseMs = float.NaN;
        public double[] CauseMs;                    /* indexed by GHHitchCause */
        public int ContentEventCount;
        public string ExcludedReason;

        /* Thermal and power */
        public int ThermalRankBefore;
        public int ThermalRankAfter;
        public float HeadroomAfter = float.NaN;
        public float CpuPerformancePctBefore = float.NaN;
        public float CpuPerformancePctAfter = float.NaN;
        public bool PowerStateKnown;                /* IsCharging and IsLowPower were read */
        public bool IsCharging;
        public bool IsLowPower;

        /* Background */
        public GHBackgroundVerdict BackgroundVerdict = GHBackgroundVerdict.Unknown;
        public string BackgroundReason;
        public float OtherCpuP90Pct = float.NaN;
        public float DiskBusyP90Pct = float.NaN;
        public float AvailableMemoryMinPct = float.NaN;
        public long AvailableMemoryMinMB = -1;
        public float HardFaultsP90PerSec = float.NaN;
        public bool LowMemory;
        public int MemoryPressureEvents;
        public float OtherGpuPct = float.NaN;
        public List<GHProcessLoad> Processes;
        public bool BackgroundSamplerDisabled;

        /* Memory */
        public long OwnMemoryBytes = -1;
        public long DeviceMemoryBytes = -1;
        public float AllocationRateMBPerSec = float.NaN;

        /* GPU path */
        public bool MainCanvasGlRequested;
        public bool? MainCanvasGpuContextLive;
        public string GpuBackend;
        public string RenderAdapterName;
        public bool? RenderAdapterIsIntegrated;
        public bool? RenderAdapterIsSoftware;
        public bool? DiscreteAdapterPresent;
        public string GpuPreference;                /* the hybrid-graphics preference text */
        public List<string> Adapters;               /* display lines */

        /* Settings, e.g. "tile batching off" */
        public List<string> NonDefaultPerformanceSettings;

        /* Instrument */
        public bool IsDebugBuild;
        public bool DebuggerAttached;
        public bool VerboseLoggingOn;
        public bool PlatformRenderLoopOn;
        public bool CountdownShown;

        /* Environment fingerprint, "category.name" keys */
        public Dictionary<string, string> Fingerprint;

        /* Appendix: the recent-hitches report, appended to the report verbatim */
        public string FrameDetailText;
    }

    public sealed class GHDiagnosisResult
    {
        public GHDiagnosisHealth Health;
        public GHCauseLocation Location;
        public List<GHDiagnosisFinding> Findings = new List<GHDiagnosisFinding>();   /* sorted */
        public GHDiagnosisFinding Primary;                                          /* null when none */
        public List<GHCheckRow> Checks = new List<GHCheckRow>();                    /* Heat through AppInternal */
        public string InconclusiveReason;   /* null unless Health is Inconclusive */
        public string Conclusion;           /* the stated result when Primary is null */
    }

    public static class GHPerformanceDiagnosis
    {
        public const string ReportFormatVersion = "1";
        public const string ReportTitle = "GnollHack performance test report v" + ReportFormatVersion;

        /* Health, against TargetFps. Poor and the healthy hitch bound are strict below or
           inclusive at as written: Poor when fps < PoorFpsFraction x target or hitches >=
           PoorHitchRatioMsPerSec; Healthy when fps >= HealthyFpsFraction x target and
           hitches < HealthyHitchRatioMsPerSec. */
        public const double HealthyFpsFraction = 0.95;
        public const double PoorFpsFraction = 0.75;
        public const float HealthyHitchRatioMsPerSec = 5f;
        public const float PoorHitchRatioMsPerSec = 25f;

        /* A hitch-cause share counts only when the summed cause time is at least this
           many ms per second of window: the healthy hitch-ratio bound, so that the few
           hitches of a healthy run name no cause. */
        public const double MinCauseMsPerSec = 5.0;

        /* Heat */
        public const int ThermalThrottledRank = 3;          /* Moderate */
        public const float ThermalRisingHeadroom = 0.85f;
        public const float CpuClockCappedPct = 60f;

        /* Background */
        public const float KnownActivityMinCpuPct = GHBackgroundLoad.KnownActivityMinCpuPct;
        public const float OtherProcessMinCpuPct = 10f;
        public const float OtherGpuSuspectPct = GHBackgroundLoad.ElevatedOtherGpuPct;
        public const float OtherGpuLikelyPct = GHBackgroundLoad.BusyOtherGpuPct;

        /* Memory. Available-memory bounds are strict (below), the others inclusive. */
        public const float LowMemoryAvailableMinPct = GHBackgroundLoad.ElevatedAvailableMemoryMinPct;
        public const float LowMemoryLikelyAvailableMinPct = GHBackgroundLoad.BusyAvailableMemoryMinPct;
        public const float LowMemoryHardFaultsP90PerSec = GHBackgroundLoad.ElevatedHardFaultsP90PerSec;
        public const float LowMemoryLikelyHardFaultsP90PerSec = GHBackgroundLoad.BusyHardFaultsP90PerSec;
        public const int LowMemoryPressureEvents = GHBackgroundLoad.ElevatedMemoryPressureEvents;

        /* GPU path, display, settings and in-game shares of hitch time */
        public const double CpuRenderingPaintShare = 0.30;
        public const double GpuBoundShare = 0.30;
        public const double CompositorShare = 0.30;
        public const double RefreshBelowMaxFraction = 0.9;
        public const double MapFpsCapFraction = 0.9;
        public const double GcPressureShare = 0.20;
        public const float GcPressureAllocationMBPerSec = 50f;
        public const double UiThreadBusyShare = 0.30;
        public const double PaintHeavyShare = 0.30;

        /* Report limits */
        public const int MaxTopProcesses = 5;
        public const int MaxFactsProcesses = 20;

        /* Fingerprint keys and values the rules read */
        public const string PowerModeKey = "settings.powerMode";
        public const string PowerPlanKey = "settings.powerPlan";
        public const string PendingRebootKey = "os.pendingReboot";
        public const string PowerModeEfficiencyValue = "Best power efficiency";
        public const string PowerPlanSaverValue = "Power saver";
        public const string GpuPreferenceDedicatedValue = "Dedicated";

        /* Checks table status values */
        public const string StatusOk = "ok";
        public const string StatusWarn = "warn";
        public const string StatusBad = "BAD";
        public const string StatusNotAvailable = "n/a";

        /* Finding codes. A known background activity's code is "BG_" and its category in
           upper case with '-' as '_', for example BG_WINDOWS_UPDATE. */
        public const string CodeThermalThrottled = "THERMAL_THROTTLED";
        public const string CodeThermalRising = "THERMAL_RISING";
        public const string CodeCpuClockCapped = "CPU_CLOCK_CAPPED";
        public const string CodeLowPowerMode = "LOW_POWER_MODE";
        public const string CodePowerEfficiencyMode = "POWER_EFFICIENCY_MODE";
        public const string CodeOnBattery = "ON_BATTERY";
        public const string CodeBackgroundBusy = "BACKGROUND_BUSY";
        public const string CodeBackgroundElevated = "BACKGROUND_ELEVATED";
        public const string CodeActivityPrefix = "BG_";
        public const string CodeOtherProcess = "BG_OTHER_PROCESS";
        public const string CodeOtherGpuLoad = "OTHER_GPU_LOAD";
        public const string CodeLowMemory = "LOW_MEMORY";
        public const string CodeCpuRendering = "CPU_RENDERING";
        public const string CodeGpuContextMissing = "GPU_CONTEXT_MISSING";
        public const string CodeSoftwareAdapter = "SOFTWARE_ADAPTER";
        public const string CodeWrongGpu = "WRONG_GPU";
        public const string CodeWrongGpuPossible = "WRONG_GPU_POSSIBLE";
        public const string CodeGpuBound = "GPU_BOUND";
        public const string CodeCompositor = "COMPOSITOR";
        public const string CodeRefreshBelowMax = "REFRESH_BELOW_MAX";
        public const string CodePacingMismatch = "PACING_MISMATCH";
        public const string CodeMapFpsCap = "MAP_FPS_CAP";
        public const string CodeSettingsNonDefault = "SETTINGS_NONDEFAULT";
        public const string CodeGcPressure = "GC_PRESSURE";
        public const string CodeUiThreadBusy = "UI_THREAD_BUSY";
        public const string CodePaintHeavy = "PAINT_HEAVY";
        public const string CodeDebugOverhead = "DEBUG_OVERHEAD";
        public const string CodePlayerInput = "PLAYER_INPUT";
        public const string CodeSamplerOff = "SAMPLER_OFF";

        /* Fixed report texts */
        public const string HealthyConclusion = "No problem measured now.";
        public const string IntermittentAdvice = "If the stutter is intermittent, use Mark Stutter when it happens, then "
            + "Analyze Recent, or run this test again while it is happening.";
        public const string EliminationConclusion = "No external or configuration cause found: the slowdown is most likely "
            + "inside the game (this build, this level's content, or a game setting). Compare with a report from an "
            + "earlier build.";
        public const string CaveatLine = "One 30 s test of the current map: indicative, not statistical.";

        private const string NA = "n/a";
        private const string Ellipsis = "...";
        private const int MinWrapRoom = 20;

        private static readonly string[] _thermalRankNames = new string[]
        {
            "Unknown", "Nominal", "Light", "Moderate", "Severe", "Critical"
        };

        /* ---------------------------------------------------------------------------
           Names
           --------------------------------------------------------------------------- */

        public static string HealthName(GHDiagnosisHealth health)
        {
            switch (health)
            {
            case GHDiagnosisHealth.Healthy:
                return "HEALTHY";
            case GHDiagnosisHealth.Degraded:
                return "DEGRADED";
            case GHDiagnosisHealth.Poor:
                return "POOR";
            default:
                return "INCONCLUSIVE";
            }
        }

        public static string LocationName(GHCauseLocation location)
        {
            switch (location)
            {
            case GHCauseLocation.External:
                return "external";
            case GHCauseLocation.Configuration:
                return "configuration";
            case GHCauseLocation.InsideGame:
                return "inside the game";
            case GHCauseLocation.Unclear:
                return "unclear";
            default:
                return "none";
            }
        }

        public static string SeverityName(GHFindingSeverity severity)
        {
            switch (severity)
            {
            case GHFindingSeverity.Likely:
                return "likely";
            case GHFindingSeverity.Suspect:
                return "suspect";
            default:
                return "info";
            }
        }

        /* The Checks table label of an area */
        public static string AreaName(GHFindingArea area)
        {
            switch (area)
            {
            case GHFindingArea.Heat:
                return "Heat";
            case GHFindingArea.Power:
                return "Power";
            case GHFindingArea.Background:
                return "Background";
            case GHFindingArea.Memory:
                return "Memory";
            case GHFindingArea.GpuPath:
                return "GPU";
            case GHFindingArea.Display:
                return "Display";
            case GHFindingArea.Settings:
                return "Settings";
            case GHFindingArea.AppInternal:
                return "Game";
            default:
                return "Instrument";
            }
        }

        /* Where a cause in an area lies. Instrument findings never set the location and
           map to None. */
        public static GHCauseLocation LocationOf(GHFindingArea area)
        {
            switch (area)
            {
            case GHFindingArea.Heat:
            case GHFindingArea.Power:
            case GHFindingArea.Background:
            case GHFindingArea.Memory:
                return GHCauseLocation.External;
            case GHFindingArea.GpuPath:
            case GHFindingArea.Display:
            case GHFindingArea.Settings:
                return GHCauseLocation.Configuration;
            case GHFindingArea.AppInternal:
                return GHCauseLocation.InsideGame;
            default:
                return GHCauseLocation.None;
            }
        }

        public static string ThermalRankName(int rank)
        {
            if (rank < 0 || rank >= _thermalRankNames.Length)
                return _thermalRankNames[0];
            return _thermalRankNames[rank];
        }

        /* The finding code of a known background activity category */
        public static string ActivityCode(string category)
        {
            if (string.IsNullOrEmpty(category))
                return CodeActivityPrefix + "UNKNOWN";
            return CodeActivityPrefix + category.ToUpperInvariant().Replace('-', '_');
        }

        /* ---------------------------------------------------------------------------
           Diagnosis
           --------------------------------------------------------------------------- */

        /* The health class. inconclusiveReason is null unless the result is Inconclusive:
           the platform render loop is off, the measurement was excluded, or the displayed
           frame rate, the target frame rate or the hitch ratio is unknown. */
        public static GHDiagnosisHealth ClassifyHealth(GHDiagnosisFacts f, out string inconclusiveReason)
        {
            inconclusiveReason = null;
            if (f == null)
            {
                inconclusiveReason = "no measurement";
                return GHDiagnosisHealth.Inconclusive;
            }
            if (!f.PlatformRenderLoopOn)
            {
                inconclusiveReason = "the platform render loop is off, so frame timing was not measured";
                return GHDiagnosisHealth.Inconclusive;
            }
            if (!string.IsNullOrEmpty(f.ExcludedReason))
            {
                inconclusiveReason = "the measurement was excluded (" + Clean(f.ExcludedReason) + ")";
                return GHDiagnosisHealth.Inconclusive;
            }
            if (!Known(f.DisplayedFps))
            {
                inconclusiveReason = "no displayed frame rate was measured";
                return GHDiagnosisHealth.Inconclusive;
            }
            if (!Known(f.TargetFps) || f.TargetFps <= 0f)
            {
                inconclusiveReason = "the target frame rate is unknown";
                return GHDiagnosisHealth.Inconclusive;
            }
            if (!Known(f.HitchRatioMsPerSec))
            {
                inconclusiveReason = "the hitch ratio was not measured";
                return GHDiagnosisHealth.Inconclusive;
            }

            double fps = f.DisplayedFps;
            double target = f.TargetFps;
            if (fps < PoorFpsFraction * target || f.HitchRatioMsPerSec >= PoorHitchRatioMsPerSec)
                return GHDiagnosisHealth.Poor;
            if (fps >= HealthyFpsFraction * target && f.HitchRatioMsPerSec < HealthyHitchRatioMsPerSec)
                return GHDiagnosisHealth.Healthy;
            return GHDiagnosisHealth.Degraded;
        }

        /* Runs the rule catalog over f. Findings are sorted by severity descending, then
           area priority, then code ordinally. Primary is the first finding at Suspect or
           stronger outside Instrument when health is Degraded or Poor; without one the
           location is InsideGame by elimination. Healthy has no primary and location
           None; Inconclusive has no primary and location Unclear. A null f is a facts
           object with every signal unknown. */
        public static GHDiagnosisResult Diagnose(GHDiagnosisFacts f)
        {
            if (f == null)
                f = new GHDiagnosisFacts();
            GHDiagnosisResult r = new GHDiagnosisResult();
            string reason;
            r.Health = ClassifyHealth(f, out reason);
            r.InconclusiveReason = reason;

            List<GHDiagnosisFinding> list = r.Findings;
            AddHeatFindings(f, list);
            AddPowerFindings(f, list);
            AddBackgroundFindings(f, list);
            AddMemoryFindings(f, list);
            AddGpuPathFindings(f, r.Health, list);
            AddDisplayFindings(f, list);
            AddSettingsFindings(f, list);
            AddAppInternalFindings(f, list);
            AddInstrumentFindings(f, list);
            list.Sort(CompareFindings);

            switch (r.Health)
            {
            case GHDiagnosisHealth.Healthy:
                r.Location = GHCauseLocation.None;
                r.Conclusion = HealthyConclusion;
                break;
            case GHDiagnosisHealth.Degraded:
            case GHDiagnosisHealth.Poor:
                for (int i = 0; i < list.Count; i++)
                {
                    if (list[i].Severity >= GHFindingSeverity.Suspect && list[i].Area != GHFindingArea.Instrument)
                    {
                        r.Primary = list[i];
                        break;
                    }
                }
                if (r.Primary != null)
                {
                    r.Location = LocationOf(r.Primary.Area);
                }
                else
                {
                    r.Location = GHCauseLocation.InsideGame;
                    r.Conclusion = EliminationConclusion;
                }
                break;
            default:
                r.Location = GHCauseLocation.Unclear;
                r.Conclusion = "Inconclusive: " + reason + ". Run the test again.";
                break;
            }

            r.Checks = BuildChecks(f, list);
            return r;
        }

        private static int CompareFindings(GHDiagnosisFinding x, GHDiagnosisFinding y)
        {
            int c = ((int)y.Severity).CompareTo((int)x.Severity);
            if (c != 0)
                return c;
            c = ((int)x.Area).CompareTo((int)y.Area);
            if (c != 0)
                return c;
            return string.CompareOrdinal(x.Code, y.Code);
        }

        private static void Add(List<GHDiagnosisFinding> list, string code, GHFindingArea area, GHFindingSeverity severity,
                                string title, string evidence, string advice)
        {
            GHDiagnosisFinding finding = new GHDiagnosisFinding();
            finding.Code = code;
            finding.Area = area;
            finding.Severity = severity;
            finding.Title = title;
            finding.Evidence = evidence;
            finding.Advice = advice;
            list.Add(finding);
        }

        /* The share of the summed hitch-cause time spent in the given causes; NaN when
           the cause times or the window length are unknown, or the summed time is below
           MinCauseMsPerSec per second of window. */
        private static double Share(GHDiagnosisFacts f, GHHitchCause a, GHHitchCause b)
        {
            double total = CauseTotalMs(f);
            if (double.IsNaN(total) || !Known(f.WindowSeconds) || f.WindowSeconds <= 0f)
                return double.NaN;
            if (total <= 0 || total < MinCauseMsPerSec * f.WindowSeconds)
                return double.NaN;
            double part = CauseMsOf(f, a);
            if (b != a)
                part += CauseMsOf(f, b);
            return part / total;
        }

        private static double Share(GHDiagnosisFacts f, GHHitchCause cause)
        {
            return Share(f, cause, cause);
        }

        /* The summed hitch-cause time, NaN when unknown; NaN and negative entries count
           as zero. */
        private static double CauseTotalMs(GHDiagnosisFacts f)
        {
            if (f.CauseMs == null)
                return double.NaN;
            double total = 0;
            for (int i = 0; i < f.CauseMs.Length; i++)
                total += PositiveOrZero(f.CauseMs[i]);
            return total;
        }

        private static double CauseMsOf(GHDiagnosisFacts f, GHHitchCause cause)
        {
            int i = (int)cause;
            if (f.CauseMs == null || i < 0 || i >= f.CauseMs.Length)
                return 0;
            return PositiveOrZero(f.CauseMs[i]);
        }

        private static string ShareText(string causes, double share)
        {
            return causes + " " + F0(share * 100.0) + " % of hitch time";
        }

        private static void AddHeatFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            /* Only a reported thermal status counts as heat here; a processor clock drop can as well be
               a power limit, so it goes to CPU_CLOCK_CAPPED */
            GHThrottleVerdict throttle = GHPerformanceComparison.ClassifyThrottle(f.ThermalRankBefore, f.ThermalRankAfter,
                f.CpuPerformancePctBefore, f.CpuPerformancePctAfter);
            bool statusThrottled = throttle.Throttled && throttle.Signal == GHThrottleSignal.Status;
            bool clockThrottled = throttle.Throttled && throttle.Signal == GHThrottleSignal.CpuPerformancePct;
            if (statusThrottled || f.ThermalRankAfter >= ThermalThrottledRank)
            {
                Add(list, CodeThermalThrottled, GHFindingArea.Heat, GHFindingSeverity.Likely,
                    "The device is thermally throttled",
                    ThermalEvidence(f),
                    "Let the device cool down, remove a thick case, avoid playing while fast-charging, and lower "
                    + "the Map FPS setting.");
            }

            bool rose = f.ThermalRankBefore > 0 && f.ThermalRankAfter > f.ThermalRankBefore;
            bool lowHeadroom = Known(f.HeadroomAfter) && f.HeadroomAfter >= ThermalRisingHeadroom;
            if (rose || lowHeadroom)
            {
                Add(list, CodeThermalRising, GHFindingArea.Heat, GHFindingSeverity.Suspect,
                    "The device is heating up",
                    ThermalEvidence(f),
                    "The device is heating up and throttling is imminent: let it cool down, or lower the Map FPS "
                    + "setting.");
            }

            bool lowClock = Known(f.CpuPerformancePctAfter) && f.CpuPerformancePctAfter < CpuClockCappedPct;
            if (lowClock || clockThrottled)
            {
                string evidence = Known(f.CpuPerformancePctBefore)
                    ? "CPU clock " + F0(f.CpuPerformancePctBefore) + " % -> " + F0(f.CpuPerformancePctAfter)
                      + " % of base during the test"
                    : "CPU clock " + F0(f.CpuPerformancePctAfter) + " % of base after the test";
                Add(list, CodeCpuClockCapped, GHFindingArea.Heat, GHFindingSeverity.Suspect,
                    "The CPU clock is capped (heat or power limit)",
                    evidence,
                    "Check the cooling and the vents, and the Windows power mode; a laptop may also cap its clock "
                    + "on battery.");
            }
        }

        private static string ThermalEvidence(GHDiagnosisFacts f)
        {
            List<string> parts = new List<string>();
            if (f.ThermalRankAfter > 0)
            {
                string before = f.ThermalRankBefore > 0 ? ThermalRankName(f.ThermalRankBefore) + " -> " : "";
                parts.Add("thermal status " + before + ThermalRankName(f.ThermalRankAfter));
            }
            if (Known(f.HeadroomAfter))
                parts.Add("thermal headroom " + F2(f.HeadroomAfter) + " (1.00 = throttling)");
            if (Known(f.CpuPerformancePctAfter))
            {
                string before = Known(f.CpuPerformancePctBefore) ? F0(f.CpuPerformancePctBefore) + " % -> " : "";
                parts.Add("CPU clock " + before + F0(f.CpuPerformancePctAfter) + " % of base");
            }
            return parts.Count > 0 ? string.Join(", ", parts.ToArray()) : "thermal state reported as throttled";
        }

        private static void AddPowerFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            if (f.PowerStateKnown && f.IsLowPower)
            {
                Add(list, CodeLowPowerMode, GHFindingArea.Power, GHFindingSeverity.Likely,
                    "Battery Saver or Low Power Mode is on",
                    "the platform reports low power mode",
                    "Turn off Battery Saver (Windows, Android) or Low Power Mode (iOS).");
            }

            string mode = FingerprintValue(f, PowerModeKey);
            string plan = FingerprintValue(f, PowerPlanKey);
            bool efficiencyMode = mode == PowerModeEfficiencyValue;
            bool saverPlan = plan == PowerPlanSaverValue;
            bool efficiency = efficiencyMode || saverPlan;
            if (efficiency)
            {
                List<string> parts = new List<string>();
                if (efficiencyMode)
                    parts.Add("power mode " + mode);
                if (saverPlan)
                    parts.Add("power plan " + plan);
                Add(list, CodePowerEfficiencyMode, GHFindingArea.Power, GHFindingSeverity.Suspect,
                    "Windows is set to save power",
                    string.Join(", ", parts.ToArray()),
                    "Windows Settings > System > Power: choose Balanced or Best performance.");
            }

            if (f.PowerStateKnown && !f.IsCharging)
            {
                Add(list, CodeOnBattery, GHFindingArea.Power,
                    efficiency ? GHFindingSeverity.Suspect : GHFindingSeverity.Info,
                    "Running on battery",
                    "not on external power",
                    "Laptops cap CPU and GPU clocks, and sometimes the refresh rate, on battery: plug in the "
                    + "charger and compare.");
            }
        }

        private static void AddBackgroundFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            bool busy = f.BackgroundVerdict == GHBackgroundVerdict.Busy;
            const string backgroundAdvice = "Close or pause the programs named here, or wait for them to finish, then "
                + "run the test again.";
            if (busy)
            {
                Add(list, CodeBackgroundBusy, GHFindingArea.Background, GHFindingSeverity.Likely,
                    "Other programs keep the machine busy",
                    string.IsNullOrEmpty(f.BackgroundReason) ? "background verdict busy" : f.BackgroundReason,
                    backgroundAdvice);
            }
            else if (f.BackgroundVerdict == GHBackgroundVerdict.Elevated)
            {
                Add(list, CodeBackgroundElevated, GHFindingArea.Background, GHFindingSeverity.Suspect,
                    "Background load is elevated",
                    string.IsNullOrEmpty(f.BackgroundReason) ? "background verdict elevated" : f.BackgroundReason,
                    backgroundAdvice);
            }

            bool pendingReboot = FingerprintValue(f, PendingRebootKey) == "true";
            List<GHBackgroundActivity> activities = GHBackgroundLoad.BuildActivities(f.Processes);
            for (int i = 0; i < activities.Count; i++)
            {
                GHBackgroundActivity a = activities[i];
                if (float.IsNaN(a.CpuPct) || a.CpuPct < KnownActivityMinCpuPct)
                    continue;
                string title;
                string advice;
                ActivityText(a.Category, pendingReboot, out title, out advice);
                Add(list, ActivityCode(a.Category), GHFindingArea.Background,
                    busy ? GHFindingSeverity.Likely : GHFindingSeverity.Suspect,
                    title,
                    a.Category + " " + F1(a.CpuPct) + " % CPU (" + string.Join(", ", a.Processes.ToArray()) + ")",
                    advice);
            }

            if (f.Processes != null)
            {
                bool found = false;
                GHProcessLoad top = new GHProcessLoad();
                for (int i = 0; i < f.Processes.Count; i++)
                {
                    GHProcessLoad p = f.Processes[i];
                    if (float.IsNaN(p.CpuPct) || ProcessCategory(p) != GHBackgroundLoad.CategoryOther)
                        continue;
                    if (!found || p.CpuPct > top.CpuPct
                        || (p.CpuPct == top.CpuPct && string.CompareOrdinal(p.Name, top.Name) < 0))
                    {
                        top = p;
                        found = true;
                    }
                }
                if (found && top.CpuPct >= OtherProcessMinCpuPct)
                {
                    Add(list, CodeOtherProcess, GHFindingArea.Background, GHFindingSeverity.Suspect,
                        "Another program is using the CPU",
                        OrNA(top.Name) + " " + F1(top.CpuPct) + " % CPU",
                        "Close that program if it is not needed, then run the test again.");
                }
            }

            float appGpuPct = GHBackgroundLoad.OtherGpuWithoutCompositor(f.OtherGpuPct, f.Processes);
            if (Known(appGpuPct) && appGpuPct >= OtherGpuSuspectPct)
            {
                Add(list, CodeOtherGpuLoad, GHFindingArea.Background,
                    appGpuPct >= OtherGpuLikelyPct ? GHFindingSeverity.Likely : GHFindingSeverity.Suspect,
                    "Another app is using the GPU",
                    "other processes' 3D GPU use " + F1(appGpuPct) + " %, compositor excluded",
                    "Close video players, browsers playing video, screen recorders and other games.");
            }
        }

        private static string ProcessCategory(GHProcessLoad p)
        {
            return string.IsNullOrEmpty(p.Category) ? GHBackgroundLoad.CategoryOf(p.Name) : p.Category;
        }

        private static void ActivityText(string category, bool pendingReboot, out string title, out string advice)
        {
            switch (category)
            {
            case GHBackgroundLoad.CategoryWindowsUpdate:
                title = "Windows Update is working";
                advice = pendingReboot
                    ? "Windows Update is installing and a restart is pending: let it finish, then restart the device."
                    : "Windows Update is installing: let it finish.";
                break;
            case GHBackgroundLoad.CategoryAntivirus:
                title = "Antivirus is scanning";
                advice = "Defender or another antivirus is scanning: wait for it to finish, or schedule scans "
                    + "outside play time.";
                break;
            case GHBackgroundLoad.CategoryIndexer:
                title = "Search indexing is running";
                advice = "Windows Search is indexing files: wait for it to finish, or exclude large folders from "
                    + "indexing.";
                break;
            case GHBackgroundLoad.CategorySync:
                title = "File sync is running";
                advice = "A cloud sync client (OneDrive, Dropbox, Google Drive) is syncing: pause syncing while "
                    + "playing.";
                break;
            case GHBackgroundLoad.CategoryBuildTools:
                title = "Build tools are running";
                advice = "A compiler, an IDE or a dotnet process is busy: let the build finish, or close the IDE.";
                break;
            case GHBackgroundLoad.CategoryWslVm:
                title = "WSL or a virtual machine is busy";
                advice = "WSL, Hyper-V or an Android subsystem VM is using the CPU: shut it down when not needed "
                    + "(wsl --shutdown).";
                break;
            case GHBackgroundLoad.CategoryTelemetry:
                title = "Windows telemetry is running";
                advice = "Windows compatibility telemetry is running: it usually finishes within minutes.";
                break;
            default:
                title = "Background activity: " + OrNA(category);
                advice = "Wait for this activity to finish, or stop it, then run the test again.";
                break;
            }
        }

        private static void AddMemoryFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            List<string> parts = new List<string>();
            bool likely = false;
            if (f.LowMemory)
            {
                parts.Add("the platform reports low memory");
                likely = true;
            }
            if (Known(f.AvailableMemoryMinPct) && f.AvailableMemoryMinPct < LowMemoryAvailableMinPct)
            {
                string mb = f.AvailableMemoryMinMB >= 0 ? " (" + Int(f.AvailableMemoryMinMB) + " MB)" : "";
                parts.Add("available memory min " + F1(f.AvailableMemoryMinPct) + " %" + mb);
                if (f.AvailableMemoryMinPct < LowMemoryLikelyAvailableMinPct)
                    likely = true;
            }
            if (Known(f.HardFaultsP90PerSec) && f.HardFaultsP90PerSec >= LowMemoryHardFaultsP90PerSec)
            {
                parts.Add("hard faults P90 " + F0(f.HardFaultsP90PerSec) + "/s");
                if (f.HardFaultsP90PerSec >= LowMemoryLikelyHardFaultsP90PerSec)
                    likely = true;
            }
            if (f.MemoryPressureEvents >= LowMemoryPressureEvents)
                parts.Add("memory pressure events " + Int(f.MemoryPressureEvents));

            if (parts.Count > 0)
            {
                Add(list, CodeLowMemory, GHFindingArea.Memory, likely ? GHFindingSeverity.Likely : GHFindingSeverity.Suspect,
                    "Memory is running low",
                    string.Join(", ", parts.ToArray()),
                    "Close other apps; if it persists, restart the device.");
            }
        }

        private static void AddGpuPathFindings(GHDiagnosisFacts f, GHDiagnosisHealth health, List<GHDiagnosisFinding> list)
        {
            if (!f.MainCanvasGlRequested)
            {
                double paintShare = Share(f, GHHitchCause.PaintCpu);
                bool heavy = !double.IsNaN(paintShare) && paintShare >= CpuRenderingPaintShare;
                string evidence = "GPU acceleration is off for the map";
                if (!double.IsNaN(paintShare))
                    evidence += ", " + ShareText("PaintCpu", paintShare);
                Add(list, CodeCpuRendering, GHFindingArea.GpuPath,
                    heavy || health != GHDiagnosisHealth.Healthy ? GHFindingSeverity.Likely : GHFindingSeverity.Info,
                    "The map is drawn on the CPU",
                    evidence,
                    "Settings > GPU Acceleration: turn it on.");
            }

            if (f.MainCanvasGlRequested && f.MainCanvasGpuContextLive == false)
            {
                Add(list, CodeGpuContextMissing, GHFindingArea.GpuPath, GHFindingSeverity.Likely,
                    "GPU requested, but the map has no GPU context",
                    "the main canvas has no live GPU context" + (string.IsNullOrEmpty(f.GpuBackend) ? "" : " (" + f.GpuBackend + ")"),
                    "The graphics driver may be missing or faulty: update the graphics driver.");
            }

            if (f.RenderAdapterIsSoftware == true)
            {
                Add(list, CodeSoftwareAdapter, GHFindingArea.GpuPath, GHFindingSeverity.Likely,
                    "Rendering on a software adapter",
                    "render adapter " + OrNA(f.RenderAdapterName),
                    "The app renders on Microsoft Basic Render Driver: install or repair the GPU driver.");
            }

            const string gpuSettingAdvice = "Windows Settings > System > Display > Graphics > GnollHack > High "
                + "performance, then restart the app.";
            if (f.RenderAdapterIsIntegrated == true && f.DiscreteAdapterPresent == true)
            {
                Add(list, CodeWrongGpu, GHFindingArea.GpuPath, GHFindingSeverity.Likely,
                    "Rendering on the integrated GPU",
                    "render adapter " + OrNA(f.RenderAdapterName) + " is integrated; a discrete GPU is present",
                    gpuSettingAdvice);
            }

            bool adapterUnknown = f.RenderAdapterIsIntegrated == null && f.RenderAdapterIsSoftware != true;
            bool severalAdapters = f.Adapters == null || f.Adapters.Count > 1;
            if (adapterUnknown && f.DiscreteAdapterPresent == true && severalAdapters
                && !string.IsNullOrEmpty(f.GpuPreference)
                && !string.Equals(f.GpuPreference.Trim(), GpuPreferenceDedicatedValue, StringComparison.OrdinalIgnoreCase))
            {
                Add(list, CodeWrongGpuPossible, GHFindingArea.GpuPath, GHFindingSeverity.Suspect,
                    "The map may be drawn on the integrated GPU",
                    "render adapter not identified; a discrete GPU is present; GPU preference " + f.GpuPreference.Trim(),
                    gpuSettingAdvice);
            }

            double gpuShare = Share(f, GHHitchCause.Gpu);
            if (!double.IsNaN(gpuShare) && gpuShare >= GpuBoundShare)
            {
                Add(list, CodeGpuBound, GHFindingArea.GpuPath, GHFindingSeverity.Suspect,
                    "The GPU is the bottleneck",
                    ShareText("Gpu", gpuShare),
                    "Lower the Map FPS setting, make sure no other app uses the GPU, and that the app runs on the "
                    + "discrete GPU.");
            }
        }

        private static void AddDisplayFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            double compositorShare = Share(f, GHHitchCause.Compositor);
            if (!double.IsNaN(compositorShare) && compositorShare >= CompositorShare)
            {
                Add(list, CodeCompositor, GHFindingArea.Display, GHFindingSeverity.Suspect,
                    "Frames are dropped after the app, in the compositor",
                    ShareText("Compositor", compositorShare),
                    "Close overlays and screen recorders, avoid monitors with mixed refresh rates, or try "
                    + "fullscreen.");
            }

            /* Only when the refresh rate is what caps the frame rate, i.e. MAP_FPS_CAP does not
               apply: below a lower Map FPS target the display rate changes nothing */
            bool displayCaps = Known(f.TargetFps) && f.TargetFps > 0f && Known(f.MeasuredRefreshHz)
                && (double)f.TargetFps >= MapFpsCapFraction * f.MeasuredRefreshHz;
            if (displayCaps && Known(f.DisplayMaxRefreshHz) && f.MeasuredRefreshHz > 0f
                && f.DisplayMaxRefreshHz > 0f
                && (double)f.MeasuredRefreshHz <= RefreshBelowMaxFraction * f.DisplayMaxRefreshHz)
            {
                Add(list, CodeRefreshBelowMax, GHFindingArea.Display, GHFindingSeverity.Suspect,
                    "The display runs below its maximum refresh rate",
                    "measured " + F1(f.MeasuredRefreshHz) + " Hz, display maximum " + F1(f.DisplayMaxRefreshHz) + " Hz",
                    "Battery dynamic refresh, power settings or display settings lower the refresh rate: check "
                    + "them.");
            }

            if (f.AssumedRefreshMismatch)
            {
                Add(list, CodePacingMismatch, GHFindingArea.Display, GHFindingSeverity.Suspect,
                    "Frame pacing assumes the wrong refresh rate",
                    "the pacing logic assumed a rate more than 5 % off the measured "
                    + (Known(f.MeasuredRefreshHz) ? F1(f.MeasuredRefreshHz) + " Hz" : "rate"),
                    "The display rate changed or is misreported: restart the app, or check the display settings.");
            }
        }

        private static void AddSettingsFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            if (Known(f.TargetFps) && Known(f.MeasuredRefreshHz) && f.TargetFps > 0f && f.MeasuredRefreshHz > 0f
                && (double)f.TargetFps < MapFpsCapFraction * f.MeasuredRefreshHz)
            {
                Add(list, CodeMapFpsCap, GHFindingArea.Settings, GHFindingSeverity.Info,
                    "The Map FPS setting caps the frame rate",
                    "target " + Fps(f.TargetFps) + " fps on a " + F1(f.MeasuredRefreshHz) + " Hz display",
                    "The Map FPS setting caps the frame rate at " + Fps(f.TargetFps) + ": raise it in Settings "
                    + "for smoother motion.");
            }

            if (f.NonDefaultPerformanceSettings != null && f.NonDefaultPerformanceSettings.Count > 0)
            {
                Add(list, CodeSettingsNonDefault, GHFindingArea.Settings, GHFindingSeverity.Suspect,
                    "Performance settings differ from the defaults",
                    string.Join(", ", f.NonDefaultPerformanceSettings.ToArray()),
                    "Restore the defaults of these settings in Settings, then run the test again.");
            }
        }

        private static void AddAppInternalFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            double gcShare = Share(f, GHHitchCause.UiThreadLateGc);
            bool gcByShare = !double.IsNaN(gcShare) && gcShare >= GcPressureShare;
            bool gcByRate = Known(f.AllocationRateMBPerSec) && f.AllocationRateMBPerSec >= GcPressureAllocationMBPerSec;
            if (gcByShare || gcByRate)
            {
                List<string> parts = new List<string>();
                if (gcByShare)
                    parts.Add(ShareText("UiThreadLateGc", gcShare));
                if (Known(f.AllocationRateMBPerSec))
                    parts.Add("allocation " + F1(f.AllocationRateMBPerSec) + " MB/s");
                parts.Add(Int(f.GcCount) + " GCs");
                Add(list, CodeGcPressure, GHFindingArea.AppInternal, GHFindingSeverity.Suspect,
                    "Garbage collection inside the game",
                    string.Join(", ", parts.ToArray()),
                    "The game allocates enough memory to cause GC pauses: compare with a report from an earlier "
                    + "build.");
            }

            double uiShare = Share(f, GHHitchCause.UiThreadLate, GHHitchCause.UiThreadRequests);
            if (!double.IsNaN(uiShare) && uiShare >= UiThreadBusyShare)
            {
                Add(list, CodeUiThreadBusy, GHFindingArea.AppInternal, GHFindingSeverity.Suspect,
                    "Game work on the UI thread",
                    ShareText("UiThreadLate + UiThreadRequests", uiShare),
                    "The UI thread is late for frames because game requests or UI work delay drawing: compare "
                    + "with a report from an earlier build.");
            }

            double paintShare = Share(f, GHHitchCause.PaintCpu);
            bool gpuOn = f.MainCanvasGlRequested && f.MainCanvasGpuContextLive != false;
            if (gpuOn && !double.IsNaN(paintShare) && paintShare >= PaintHeavyShare)
            {
                string evidence = ShareText("PaintCpu", paintShare);
                if (Known(f.PaintP99Ms))
                    evidence += ", paint P99 " + F1(f.PaintP99Ms) + " ms";
                Add(list, CodePaintHeavy, GHFindingArea.AppInternal, GHFindingSeverity.Suspect,
                    "Map drawing is CPU-heavy",
                    evidence,
                    "Drawing this map costs much CPU time (level content or build): compare in another level, or "
                    + "with a report from an earlier build.");
            }
        }

        private static void AddInstrumentFindings(GHDiagnosisFacts f, List<GHDiagnosisFinding> list)
        {
            if (f.IsDebugBuild || f.DebuggerAttached || f.VerboseLoggingOn)
            {
                List<string> parts = new List<string>();
                if (f.IsDebugBuild)
                    parts.Add("debug build");
                if (f.DebuggerAttached)
                    parts.Add("debugger attached");
                if (f.VerboseLoggingOn)
                    parts.Add("verbose logging on");
                Add(list, CodeDebugOverhead, GHFindingArea.Instrument,
                    f.DebuggerAttached ? GHFindingSeverity.Suspect : GHFindingSeverity.Info,
                    "Debug overhead",
                    string.Join(", ", parts.ToArray()),
                    "Debug builds, an attached debugger and logging slow rendering: compare with a Release build.");
            }

            if (f.ContentEventCount > 0)
            {
                Add(list, CodePlayerInput, GHFindingArea.Instrument, GHFindingSeverity.Info,
                    "Game content changed during the test",
                    Int(f.ContentEventCount) + " frame gaps with content events",
                    "Input during the test mixes workloads: do not touch the game while the test runs.");
            }

            if (f.BackgroundSamplerDisabled)
            {
                Add(list, CodeSamplerOff, GHFindingArea.Instrument, GHFindingSeverity.Info,
                    "Background load was not checked",
                    "the background sampler is disabled",
                    "Enable the background sampler to check the background load.");
            }
        }

        /* ---------------------------------------------------------------------------
           Checks
           --------------------------------------------------------------------------- */

        private static List<GHCheckRow> BuildChecks(GHDiagnosisFacts f, List<GHDiagnosisFinding> sorted)
        {
            List<GHCheckRow> rows = new List<GHCheckRow>();
            for (int a = (int)GHFindingArea.Heat; a <= (int)GHFindingArea.AppInternal; a++)
            {
                GHFindingArea area = (GHFindingArea)a;
                GHCheckRow row = new GHCheckRow();
                row.Area = area;
                GHDiagnosisFinding strongest = null;
                for (int i = 0; i < sorted.Count; i++)
                {
                    if (sorted[i].Area == area && sorted[i].Severity >= GHFindingSeverity.Suspect)
                    {
                        strongest = sorted[i];
                        break;
                    }
                }
                if (strongest != null)
                {
                    row.Status = strongest.Severity == GHFindingSeverity.Likely ? StatusBad : StatusWarn;
                    row.Detail = strongest.Title + " (" + strongest.Code + ")";
                }
                else
                {
                    string detail = CheckDetail(f, area);
                    if (detail != null)
                    {
                        row.Status = StatusOk;
                        row.Detail = detail;
                    }
                    else
                    {
                        row.Status = StatusNotAvailable;
                        row.Detail = area == GHFindingArea.Background && f.BackgroundSamplerDisabled
                            ? "background sampler disabled" : "not measured on this device";
                    }
                }
                rows.Add(row);
            }
            return rows;
        }

        /* What an area's check covered, or null when none of its signals is known */
        private static string CheckDetail(GHDiagnosisFacts f, GHFindingArea area)
        {
            List<string> parts = new List<string>();
            switch (area)
            {
            case GHFindingArea.Heat:
                if (f.ThermalRankAfter > 0)
                    parts.Add("thermal " + ThermalRankName(f.ThermalRankAfter));
                if (Known(f.HeadroomAfter))
                    parts.Add("headroom " + F2(f.HeadroomAfter));
                if (Known(f.CpuPerformancePctAfter))
                    parts.Add("CPU clock " + F0(f.CpuPerformancePctAfter) + " %");
                break;
            case GHFindingArea.Power:
                if (f.PowerStateKnown)
                {
                    parts.Add(f.IsCharging ? "on external power" : "on battery");
                    parts.Add("low power " + OnOff(f.IsLowPower));
                }
                if (FingerprintValue(f, PowerModeKey) != null)
                    parts.Add("power mode " + FingerprintValue(f, PowerModeKey));
                if (FingerprintValue(f, PowerPlanKey) != null)
                    parts.Add("power plan " + FingerprintValue(f, PowerPlanKey));
                break;
            case GHFindingArea.Background:
                if (f.BackgroundSamplerDisabled)
                    return null;
                if (f.BackgroundVerdict != GHBackgroundVerdict.Unknown)
                {
                    parts.Add(GHBackgroundLoad.VerdictName(f.BackgroundVerdict));
                    if (Known(f.OtherCpuP90Pct))
                        parts.Add("other CPU P90 " + F0(f.OtherCpuP90Pct) + " %");
                    if (Known(f.OtherGpuPct))
                        parts.Add("other GPU " + F0(f.OtherGpuPct) + " %");
                }
                break;
            case GHFindingArea.Memory:
                if (Known(f.AvailableMemoryMinPct))
                    parts.Add("available min " + F0(f.AvailableMemoryMinPct) + " %");
                else if (f.AvailableMemoryMinMB >= 0)
                    parts.Add("available min " + Int(f.AvailableMemoryMinMB) + " MB");
                if (Known(f.HardFaultsP90PerSec))
                    parts.Add("hard faults P90 " + F0(f.HardFaultsP90PerSec) + "/s");
                if (f.OwnMemoryBytes >= 0)
                    parts.Add("app " + MB(f.OwnMemoryBytes) + " MB");
                break;
            case GHFindingArea.GpuPath:
                if (!f.MainCanvasGlRequested)
                {
                    parts.Add("CPU rendering (GPU acceleration off)");
                }
                else if (!string.IsNullOrEmpty(f.RenderAdapterName))
                {
                    parts.Add("rendering on " + Clean(f.RenderAdapterName) + " (" + AdapterKind(f) + ")");
                }
                else
                {
                    string context = f.MainCanvasGpuContextLive == true ? "context live"
                        : f.MainCanvasGpuContextLive == false ? "no context" : "context not checked";
                    parts.Add("GPU " + (string.IsNullOrEmpty(f.GpuBackend) ? "on" : Clean(f.GpuBackend)) + ", " + context
                        + ", adapter not identified");
                }
                break;
            case GHFindingArea.Display:
                if (Known(f.MeasuredRefreshHz))
                {
                    string max = Known(f.DisplayMaxRefreshHz) ? " of max " + F1(f.DisplayMaxRefreshHz) + " Hz" : "";
                    parts.Add("refresh " + F1(f.MeasuredRefreshHz) + " Hz" + max);
                }
                break;
            case GHFindingArea.Settings:
                if (Known(f.TargetFps))
                    parts.Add("Map FPS " + Fps(f.TargetFps));
                if (f.NonDefaultPerformanceSettings != null)
                    parts.Add("performance settings at defaults");
                break;
            case GHFindingArea.AppInternal:
                double total = CauseTotalMs(f);
                if (!double.IsNaN(total) && Known(f.WindowSeconds) && f.WindowSeconds > 0f)
                {
                    parts.Add("hitch cause time " + F1(total / f.WindowSeconds) + " ms/s");
                    parts.Add("GC " + Int(f.GcCount));
                }
                if (Known(f.AllocationRateMBPerSec))
                    parts.Add("allocation " + F1(f.AllocationRateMBPerSec) + " MB/s");
                break;
            }
            return parts.Count > 0 ? string.Join(", ", parts.ToArray()) : null;
        }

        private static string AdapterKind(GHDiagnosisFacts f)
        {
            if (f.RenderAdapterIsSoftware == true)
                return "software";
            if (f.RenderAdapterIsIntegrated == true)
                return "integrated";
            if (f.RenderAdapterIsIntegrated == false)
                return "discrete";
            return "type unknown";
        }

        /* ---------------------------------------------------------------------------
           Report
           --------------------------------------------------------------------------- */

        /* The report text: at most GHPerformanceTextReport.MaxLineWidth characters per
           line except in the verbatim Frame detail appendix, "\n" line endings,
           invariant culture, and the same text for the same inputs. A null r is
           Diagnose(f). */
        public static string BuildReport(GHDiagnosisFacts f, GHDiagnosisResult r, DateTime localNow)
        {
            if (f == null)
                f = new GHDiagnosisFacts();
            if (r == null)
                r = Diagnose(f);
            List<GHDiagnosisFinding> findings = r.Findings ?? new List<GHDiagnosisFinding>();

            StringBuilder sb = new StringBuilder(8192);
            Line(sb, ReportTitle + " " + localNow.ToString("yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture));
            Line(sb, "");
            Line(sb, ResultLine(f, r));
            Line(sb, "Location: " + LocationName(r.Location));
            Line(sb, "");

            Line(sb, "Most likely cause:");
            if (r.Primary != null)
            {
                AppendFinding(sb, r.Primary);
            }
            else
            {
                AppendWrapped(sb, "  ", "  ", r.Conclusion);
                if (r.Health == GHDiagnosisHealth.Healthy)
                    AppendWrapped(sb, "  ", "  ", IntermittentAdvice);
            }
            Line(sb, "");

            Line(sb, "Also found:");
            int also = 0;
            for (int i = 0; i < findings.Count; i++)
            {
                GHDiagnosisFinding x = findings[i];
                if (x == r.Primary || x.Severity < GHFindingSeverity.Suspect)
                    continue;
                AppendFinding(sb, x);
                also++;
            }
            if (also == 0)
                Line(sb, "  none");
            Line(sb, "");

            Line(sb, "Checks:");
            List<GHCheckRow> checks = r.Checks ?? new List<GHCheckRow>();
            for (int i = 0; i < checks.Count; i++)
            {
                GHCheckRow row = checks[i];
                Line(sb, "  " + PadR(AreaName(row.Area), 12) + PadR(row.Status ?? StatusNotAvailable, 6) + Clean(row.Detail));
            }
            Line(sb, "");

            AppendMeasurements(sb, f);
            Line(sb, "");

            AppendHitchCauses(sb, f);
            Line(sb, "");

            Line(sb, "Notes:");
            for (int i = 0; i < findings.Count; i++)
            {
                if (findings[i].Severity == GHFindingSeverity.Info)
                    AppendFinding(sb, findings[i]);
            }
            Line(sb, "  " + CaveatLine);
            Line(sb, "");

            AppendEnvironment(sb, f);
            Line(sb, "");

            AppendFacts(sb, f, r);
            Line(sb, "");

            Line(sb, "Frame detail:");
            if (string.IsNullOrEmpty(f.FrameDetailText))
            {
                Line(sb, "  " + NA);
            }
            else
            {
                string detail = f.FrameDetailText.Replace("\r\n", "\n").Replace('\r', '\n');
                sb.Append(detail);
                if (!detail.EndsWith("\n", StringComparison.Ordinal))
                    sb.Append("\n");
            }
            return sb.ToString();
        }

        /* The report's Result and Location lines, trimmed and joined by " | ", or null
           when the text is null or lacks either line. */
        public static string HeadlineOf(string reportText)
        {
            if (string.IsNullOrEmpty(reportText))
                return null;
            string result = null;
            string location = null;
            string[] lines = reportText.Split('\n');
            for (int i = 0; i < lines.Length && (result == null || location == null); i++)
            {
                string line = lines[i].TrimEnd('\r');
                if (result == null && line.StartsWith("Result:", StringComparison.Ordinal))
                    result = line.Trim();
                else if (location == null && line.StartsWith("Location:", StringComparison.Ordinal))
                    location = line.Trim();
            }
            if (result == null || location == null)
                return null;
            return result + " | " + location;
        }

        private static string ResultLine(GHDiagnosisFacts f, GHDiagnosisResult r)
        {
            return "Result: " + HealthName(r.Health) + "  " + F1(f.DisplayedFps) + " of " + Fps(f.TargetFps)
                + " target fps, hitches " + F1(f.HitchRatioMsPerSec) + " ms/s";
        }

        private static void AppendFinding(StringBuilder sb, GHDiagnosisFinding x)
        {
            AppendWrapped(sb, "  ", "    ", x.Title + " (" + x.Code + ", " + SeverityName(x.Severity) + ")");
            if (!string.IsNullOrEmpty(x.Evidence))
                AppendWrapped(sb, "    ", "    ", x.Evidence);
            if (!string.IsNullOrEmpty(x.Advice))
                AppendWrapped(sb, "    -> ", "       ", x.Advice);
        }

        private static void AppendMeasurements(StringBuilder sb, GHDiagnosisFacts f)
        {
            Line(sb, "Measurements:");
            Line(sb, "  Window: " + F1(f.WindowSeconds) + " s, target " + Fps(f.TargetFps) + " fps, displayed "
                + F1(f.DisplayedFps) + " fps");
            Line(sb, "  Refresh: measured " + F1(f.MeasuredRefreshHz) + " Hz, display max " + F1(f.DisplayMaxRefreshHz)
                + " Hz, pacing rate " + (f.AssumedRefreshMismatch ? "mismatch" : "ok"));
            Line(sb, "  Hitches: " + Int(f.HitchCount) + ", " + F1(f.HitchRatioMsPerSec) + " ms/s, pacing error RMS "
                + F2(f.PacingErrorRmsMs) + " ms");
            Line(sb, "  Paint: P50 " + F2(f.PaintP50Ms) + " ms, P99 " + F2(f.PaintP99Ms) + " ms");
            Line(sb, "  GC: " + Int(f.GcCount) + " collections, pause " + F1(f.GcPauseMs) + " ms, allocation "
                + F1(f.AllocationRateMBPerSec) + " MB/s");
            Line(sb, "  Thermal: " + ThermalRankName(f.ThermalRankBefore) + " -> " + ThermalRankName(f.ThermalRankAfter)
                + ", headroom " + F2(f.HeadroomAfter) + ", CPU clock " + F0(f.CpuPerformancePctBefore) + " -> "
                + F0(f.CpuPerformancePctAfter) + " %");

            string power = f.PowerStateKnown
                ? (f.IsCharging ? "external power" : "battery") + ", low power " + OnOff(f.IsLowPower)
                : "state unknown";
            string mode = FingerprintValue(f, PowerModeKey);
            string plan = FingerprintValue(f, PowerPlanKey);
            if (mode != null)
                power += ", mode " + mode;
            if (plan != null)
                power += ", plan " + plan;
            AppendWrapped(sb, "  Power: ", "    ", power);

            string background = f.BackgroundSamplerDisabled ? "sampler disabled"
                : GHBackgroundLoad.VerdictName(f.BackgroundVerdict) + ", other CPU P90 " + F0(f.OtherCpuP90Pct)
                + " %, disk busy P90 " + F0(f.DiskBusyP90Pct) + " %, other GPU " + F0(f.OtherGpuPct) + " %";
            AppendWrapped(sb, "  Background: ", "    ", background);
            if (!string.IsNullOrEmpty(f.BackgroundReason))
                AppendWrapped(sb, "  Background reason: ", "    ", f.BackgroundReason);

            List<GHProcessLoad> top = SortedProcesses(f.Processes);
            if (top != null)
            {
                List<string> names = new List<string>();
                for (int i = 0; i < top.Count && i < MaxTopProcesses; i++)
                    names.Add(OrNA(top[i].Name) + " " + F1(top[i].CpuPct) + " % (" + ProcessCategory(top[i]) + ")");
                AppendWrapped(sb, "  Top processes: ", "    ", names.Count > 0 ? string.Join(", ", names.ToArray()) : "none");
            }

            Line(sb, "  Memory: app " + MB(f.OwnMemoryBytes) + " MB, device " + MB(f.DeviceMemoryBytes)
                + " MB, available min " + F1(f.AvailableMemoryMinPct) + " % ("
                + (f.AvailableMemoryMinMB >= 0 ? Int(f.AvailableMemoryMinMB) : NA) + " MB)");
            Line(sb, "  Memory pressure: hard faults P90 " + F0(f.HardFaultsP90PerSec) + "/s, events "
                + Int(f.MemoryPressureEvents) + ", low memory " + YesNo(f.LowMemory));

            string gpu = "requested " + YesNo(f.MainCanvasGlRequested) + ", context " + Tri(f.MainCanvasGpuContextLive)
                + ", backend " + OrNA(f.GpuBackend) + ", preference " + OrNA(f.GpuPreference);
            AppendWrapped(sb, "  GPU: ", "    ", gpu);
            string adapter = string.IsNullOrEmpty(f.RenderAdapterName) ? "not identified"
                : f.RenderAdapterName + " (" + AdapterKind(f) + ")";
            AppendWrapped(sb, "  Render adapter: ", "    ", adapter + ", discrete present " + Tri(f.DiscreteAdapterPresent));
            if (f.Adapters != null)
            {
                for (int i = 0; i < f.Adapters.Count; i++)
                    AppendWrapped(sb, "  Adapter " + Int(i) + ": ", "    ", OrNA(f.Adapters[i]));
            }

            string settings = "Map FPS " + Fps(f.TargetFps) + ", non-default: ";
            if (f.NonDefaultPerformanceSettings == null)
                settings += NA;
            else if (f.NonDefaultPerformanceSettings.Count == 0)
                settings += "none";
            else
                settings += string.Join(", ", f.NonDefaultPerformanceSettings.ToArray());
            AppendWrapped(sb, "  Settings: ", "    ", settings);

            Line(sb, "  Instrument: debug build " + YesNo(f.IsDebugBuild) + ", debugger " + YesNo(f.DebuggerAttached)
                + ", verbose logging " + YesNo(f.VerboseLoggingOn) + ", render loop " + OnOff(f.PlatformRenderLoopOn));
            Line(sb, "  Content events: " + Int(f.ContentEventCount) + ", countdown shown " + YesNo(f.CountdownShown)
                + (string.IsNullOrEmpty(f.ExcludedReason) ? "" : ", excluded"));
            if (!string.IsNullOrEmpty(f.ExcludedReason))
                AppendWrapped(sb, "  Excluded: ", "    ", f.ExcludedReason);
        }

        private static void AppendHitchCauses(StringBuilder sb, GHDiagnosisFacts f)
        {
            Line(sb, "Hitch causes:");
            double total = CauseTotalMs(f);
            if (double.IsNaN(total))
            {
                Line(sb, "  " + NA);
                return;
            }
            if (total <= 0)
            {
                Line(sb, "  none");
                return;
            }
            List<int> order = new List<int>();
            for (int i = 0; i < f.CauseMs.Length; i++)
            {
                if (PositiveOrZero(f.CauseMs[i]) > 0)
                    order.Add(i);
            }
            double[] ms = f.CauseMs;
            order.Sort(delegate (int x, int y)
            {
                int c = PositiveOrZero(ms[y]).CompareTo(PositiveOrZero(ms[x]));
                return c != 0 ? c : x.CompareTo(y);
            });
            for (int k = 0; k < order.Count; k++)
            {
                int i = order[k];
                double v = PositiveOrZero(ms[i]);
                Line(sb, "  " + PadR(GHSmoothnessMetrics.CauseName((GHHitchCause)i), 18) + PadL(F1(v), 9) + " ms"
                    + PadL(F0(100.0 * v / total), 6) + " %");
            }
            string perSecond = Known(f.WindowSeconds) && f.WindowSeconds > 0f ? F1(total / f.WindowSeconds) : NA;
            Line(sb, "  " + PadR("total", 18) + PadL(F1(total), 9) + " ms  (" + perSecond + " ms/s)");
        }

        private static void AppendEnvironment(StringBuilder sb, GHDiagnosisFacts f)
        {
            Line(sb, "Environment:");
            List<string> keys = new List<string>();
            if (f.Fingerprint != null)
            {
                foreach (KeyValuePair<string, string> kv in f.Fingerprint)
                {
                    if (IncludeInEnvironment(kv.Key))
                        keys.Add(kv.Key);
                }
            }
            keys.Sort(string.CompareOrdinal);
            if (keys.Count == 0)
            {
                Line(sb, "  " + NA);
                return;
            }
            for (int i = 0; i < keys.Count; i++)
            {
                string prefix = "  " + Clean(keys[i]) + ": ";
                string value = f.Fingerprint[keys[i]];
                value = value == null ? NA : Clean(value);
                Line(sb, prefix + ShortenMiddle(value, GHPerformanceTextReport.MaxLineWidth - prefix.Length));
            }
        }

        private static bool IncludeInEnvironment(string key)
        {
            if (string.IsNullOrEmpty(key))
                return false;
            string category = GHEnvironmentFingerprint.CategoryOf(key);
            if (category == GHEnvironmentFingerprint.CategoryMeta)
                return false;
            if (category == GHEnvironmentFingerprint.CategoryComponent)
            {
                return key == GHEnvironmentFingerprint.ComponentSkiaSharpKey
                    || key == GHEnvironmentFingerprint.ComponentMauiControlsKey
                    || key == GHEnvironmentFingerprint.ComponentFmodKey;
            }
            return true;
        }

        /* The Facts block: "  key=value" per GHDiagnosisFacts field in declaration order,
           n/a for unknown values. Lists give their count under their own key, then one
           "key.<i>=" line per entry; a process entry is "name;cpuPct;gpuPct;category",
           for at most MaxFactsProcesses processes by CPU descending. The fingerprint
           gives its key count; its values are in the Environment block. Then health,
           location and primary, and last the finding codes in report order; a list too
           long for one line continues on further findings= lines. */
        private static void AppendFacts(StringBuilder sb, GHDiagnosisFacts f, GHDiagnosisResult r)
        {
            Line(sb, "Facts:");
            Fact(sb, "formatVersion", ReportFormatVersion);

            Fact(sb, "windowSeconds", Num(f.WindowSeconds));
            Fact(sb, "targetFps", Num(f.TargetFps));
            Fact(sb, "measuredRefreshHz", Num(f.MeasuredRefreshHz));
            Fact(sb, "displayMaxRefreshHz", Num(f.DisplayMaxRefreshHz));
            Fact(sb, "assumedRefreshMismatch", Bool(f.AssumedRefreshMismatch));
            Fact(sb, "displayedFps", Num(f.DisplayedFps));
            Fact(sb, "hitchRatioMsPerSec", Num(f.HitchRatioMsPerSec));
            Fact(sb, "hitchCount", Int(f.HitchCount));
            Fact(sb, "pacingErrorRmsMs", Num(f.PacingErrorRmsMs));
            Fact(sb, "paintP50Ms", Num(f.PaintP50Ms));
            Fact(sb, "paintP99Ms", Num(f.PaintP99Ms));
            Fact(sb, "gcCount", Int(f.GcCount));
            Fact(sb, "gcPauseMs", Num(f.GcPauseMs));
            if (f.CauseMs == null)
            {
                Fact(sb, "causeMs", NA);
            }
            else
            {
                for (int i = 0; i < f.CauseMs.Length; i++)
                    Fact(sb, "causeMs." + GHSmoothnessMetrics.CauseName((GHHitchCause)i), Num(f.CauseMs[i]));
            }
            Fact(sb, "contentEventCount", Int(f.ContentEventCount));
            Fact(sb, "excludedReason", Str(f.ExcludedReason));

            Fact(sb, "thermalRankBefore", Int(f.ThermalRankBefore));
            Fact(sb, "thermalRankAfter", Int(f.ThermalRankAfter));
            Fact(sb, "headroomAfter", Num(f.HeadroomAfter));
            Fact(sb, "cpuPerformancePctBefore", Num(f.CpuPerformancePctBefore));
            Fact(sb, "cpuPerformancePctAfter", Num(f.CpuPerformancePctAfter));
            Fact(sb, "powerStateKnown", Bool(f.PowerStateKnown));
            Fact(sb, "isCharging", Bool(f.IsCharging));
            Fact(sb, "isLowPower", Bool(f.IsLowPower));

            Fact(sb, "backgroundVerdict", GHBackgroundLoad.VerdictName(f.BackgroundVerdict));
            Fact(sb, "backgroundReason", Str(f.BackgroundReason));
            Fact(sb, "otherCpuP90Pct", Num(f.OtherCpuP90Pct));
            Fact(sb, "diskBusyP90Pct", Num(f.DiskBusyP90Pct));
            Fact(sb, "availableMemoryMinPct", Num(f.AvailableMemoryMinPct));
            Fact(sb, "availableMemoryMinMB", f.AvailableMemoryMinMB >= 0 ? Int(f.AvailableMemoryMinMB) : NA);
            Fact(sb, "hardFaultsP90PerSec", Num(f.HardFaultsP90PerSec));
            Fact(sb, "lowMemory", Bool(f.LowMemory));
            Fact(sb, "memoryPressureEvents", Int(f.MemoryPressureEvents));
            Fact(sb, "otherGpuPct", Num(f.OtherGpuPct));
            List<GHProcessLoad> processes = SortedProcesses(f.Processes);
            if (processes == null)
            {
                Fact(sb, "processes", NA);
            }
            else
            {
                Fact(sb, "processes", Int(processes.Count));
                for (int i = 0; i < processes.Count && i < MaxFactsProcesses; i++)
                {
                    GHProcessLoad p = processes[i];
                    Fact(sb, "processes." + Int(i), Clean(p.Name).Replace(';', ',') + ";" + Num(p.CpuPct) + ";"
                        + Num(p.GpuPct) + ";" + ProcessCategory(p));
                }
            }
            Fact(sb, "backgroundSamplerDisabled", Bool(f.BackgroundSamplerDisabled));

            Fact(sb, "ownMemoryBytes", f.OwnMemoryBytes >= 0 ? Int(f.OwnMemoryBytes) : NA);
            Fact(sb, "deviceMemoryBytes", f.DeviceMemoryBytes >= 0 ? Int(f.DeviceMemoryBytes) : NA);
            Fact(sb, "allocationRateMBPerSec", Num(f.AllocationRateMBPerSec));

            Fact(sb, "mainCanvasGlRequested", Bool(f.MainCanvasGlRequested));
            Fact(sb, "mainCanvasGpuContextLive", Tri(f.MainCanvasGpuContextLive));
            Fact(sb, "gpuBackend", Str(f.GpuBackend));
            Fact(sb, "renderAdapterName", Str(f.RenderAdapterName));
            Fact(sb, "renderAdapterIsIntegrated", Tri(f.RenderAdapterIsIntegrated));
            Fact(sb, "renderAdapterIsSoftware", Tri(f.RenderAdapterIsSoftware));
            Fact(sb, "discreteAdapterPresent", Tri(f.DiscreteAdapterPresent));
            Fact(sb, "gpuPreference", Str(f.GpuPreference));
            FactList(sb, "adapters", f.Adapters);

            FactList(sb, "nonDefaultPerformanceSettings", f.NonDefaultPerformanceSettings);

            Fact(sb, "isDebugBuild", Bool(f.IsDebugBuild));
            Fact(sb, "debuggerAttached", Bool(f.DebuggerAttached));
            Fact(sb, "verboseLoggingOn", Bool(f.VerboseLoggingOn));
            Fact(sb, "platformRenderLoopOn", Bool(f.PlatformRenderLoopOn));
            Fact(sb, "countdownShown", Bool(f.CountdownShown));

            Fact(sb, "fingerprint", f.Fingerprint == null ? NA : Int(f.Fingerprint.Count));
            Fact(sb, "frameDetailText", string.IsNullOrEmpty(f.FrameDetailText) ? NA : "present");

            Fact(sb, "health", HealthName(r.Health).ToLowerInvariant());
            Fact(sb, "location", LocationName(r.Location));
            Fact(sb, "primary", r.Primary == null ? "none" : r.Primary.Code);

            const string findingsPrefix = "  findings=";
            List<GHDiagnosisFinding> findings = r.Findings ?? new List<GHDiagnosisFinding>();
            StringBuilder codes = new StringBuilder();
            for (int i = 0; i < findings.Count; i++)
            {
                string code = findings[i].Code ?? "";
                if (codes.Length > 0 && findingsPrefix.Length + codes.Length + 1 + code.Length > GHPerformanceTextReport.MaxLineWidth)
                {
                    Line(sb, findingsPrefix + codes.ToString());
                    codes.Length = 0;
                }
                if (codes.Length > 0)
                    codes.Append(',');
                codes.Append(code);
            }
            Line(sb, findingsPrefix + codes.ToString());
        }

        private static void Fact(StringBuilder sb, string key, string value)
        {
            string prefix = "  " + key + "=";
            Line(sb, prefix + ShortenMiddle(value ?? NA, GHPerformanceTextReport.MaxLineWidth - prefix.Length));
        }

        private static void FactList(StringBuilder sb, string key, List<string> values)
        {
            if (values == null)
            {
                Fact(sb, key, NA);
                return;
            }
            Fact(sb, key, Int(values.Count));
            for (int i = 0; i < values.Count; i++)
                Fact(sb, key + "." + Int(i), Str(values[i]));
        }

        /* A copy of processes ordered by CPU descending, then name ordinally; null for
           null. */
        private static List<GHProcessLoad> SortedProcesses(List<GHProcessLoad> processes)
        {
            if (processes == null)
                return null;
            List<GHProcessLoad> copy = new List<GHProcessLoad>(processes);
            copy.Sort(CompareProcesses);
            return copy;
        }

        private static int CompareProcesses(GHProcessLoad x, GHProcessLoad y)
        {
            float cx = float.IsNaN(x.CpuPct) ? -1f : x.CpuPct;
            float cy = float.IsNaN(y.CpuPct) ? -1f : y.CpuPct;
            int c = cy.CompareTo(cx);
            if (c != 0)
                return c;
            return string.CompareOrdinal(x.Name, y.Name);
        }

        /* ---------------------------------------------------------------------------
           Text helpers
           --------------------------------------------------------------------------- */

        /* Appends text word-wrapped at MaxLineWidth: the first line after firstPrefix,
           the others after restPrefix. A word longer than a line is cut. */
        private static void AppendWrapped(StringBuilder sb, string firstPrefix, string restPrefix, string text)
        {
            text = Clean(text);
            if (text.Length == 0)
                return;
            string prefix = firstPrefix;
            int pos = 0;
            while (pos < text.Length)
            {
                while (pos < text.Length && text[pos] == ' ')
                    pos++;
                if (pos >= text.Length)
                    break;
                int room = Math.Max(MinWrapRoom, GHPerformanceTextReport.MaxLineWidth - prefix.Length);
                if (text.Length - pos <= room)
                {
                    Line(sb, prefix + text.Substring(pos));
                    break;
                }
                int space = text.LastIndexOf(' ', pos + room, room);
                int end;
                int next;
                if (space > pos)
                {
                    end = space;
                    next = space + 1;
                }
                else
                {
                    end = pos + room;
                    next = end;
                }
                Line(sb, prefix + text.Substring(pos, end - pos).TrimEnd());
                pos = next;
                prefix = restPrefix;
            }
        }

        /* Appends one "\n"-terminated line, cut with an ellipsis at MaxLineWidth. */
        private static void Line(StringBuilder sb, string text)
        {
            if (text == null)
                text = "";
            int max = GHPerformanceTextReport.MaxLineWidth;
            if (text.Length > max)
                text = text.Substring(0, max - Ellipsis.Length) + Ellipsis;
            sb.Append(text);
            sb.Append("\n");
        }

        /* s cut to at most maxLen characters, keeping its head and tail around an
           ellipsis. */
        private static string ShortenMiddle(string s, int maxLen)
        {
            if (s == null)
                return "";
            if (s.Length <= maxLen)
                return s;
            if (maxLen <= Ellipsis.Length)
                return maxLen <= 0 ? "" : s.Substring(0, maxLen);
            int keep = maxLen - Ellipsis.Length;
            int head = (keep + 1) / 2;
            int tail = keep - head;
            return s.Substring(0, head) + Ellipsis + s.Substring(s.Length - tail);
        }

        /* s on one line: null as "", line breaks and tabs as spaces */
        private static string Clean(string s)
        {
            if (string.IsNullOrEmpty(s))
                return "";
            return s.Replace("\r\n", " ").Replace('\r', ' ').Replace('\n', ' ').Replace('\t', ' ');
        }

        private static string PadR(string s, int width)
        {
            s = s ?? "";
            return s.Length >= width ? s + " " : s.PadRight(width);
        }

        private static string PadL(string s, int width)
        {
            s = s ?? "";
            return s.Length >= width ? s : s.PadLeft(width);
        }

        private static string FingerprintValue(GHDiagnosisFacts f, string key)
        {
            if (f.Fingerprint == null)
                return null;
            string value;
            if (!f.Fingerprint.TryGetValue(key, out value) || string.IsNullOrEmpty(value))
                return null;
            return value;
        }

        private static bool Known(float v)
        {
            return !float.IsNaN(v) && !float.IsInfinity(v);
        }

        private static double PositiveOrZero(double v)
        {
            return double.IsNaN(v) || double.IsInfinity(v) || v < 0 ? 0 : v;
        }

        private static string Format(double v, string format)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
                return NA;
            return v.ToString(format, CultureInfo.InvariantCulture);
        }

        private static string F0(double v)
        {
            return Format(v, "0");
        }

        private static string F1(double v)
        {
            return Format(v, "0.0");
        }

        private static string F2(double v)
        {
            return Format(v, "0.00");
        }

        private static string Fps(double v)
        {
            return Format(v, "0.#");
        }

        private static string Num(double v)
        {
            return Format(v, "0.###");
        }

        private static string Int(long v)
        {
            return v.ToString(CultureInfo.InvariantCulture);
        }

        private static string MB(long bytes)
        {
            if (bytes < 0)
                return NA;
            return (bytes / (1024L * 1024L)).ToString(CultureInfo.InvariantCulture);
        }

        private static string Bool(bool v)
        {
            return v ? "true" : "false";
        }

        private static string Tri(bool? v)
        {
            return v.HasValue ? Bool(v.Value) : NA;
        }

        private static string YesNo(bool v)
        {
            return v ? "yes" : "no";
        }

        private static string OnOff(bool v)
        {
            return v ? "on" : "off";
        }

        private static string OrNA(string s)
        {
            return string.IsNullOrEmpty(s) ? NA : Clean(s);
        }

        private static string Str(string s)
        {
            return s == null ? NA : Clean(s);
        }
    }
}

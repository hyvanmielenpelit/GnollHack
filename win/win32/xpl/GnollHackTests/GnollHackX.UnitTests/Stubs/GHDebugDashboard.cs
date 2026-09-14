using GnollHackX.Perf;

namespace GnollHackX
{
    /* Test-side stand-in for the app's GHDebugDashboard. The real one rebuilds UI
       row strings; this one records the last PublishFrameStats call so a test can
       assert that FrameTimeProfiler.PublishDashboardSnapshot reached it with the
       expected arguments. */
    public static class GHDebugDashboard
    {
        public static int PublishCount;
        public static FrameTimeStreamingStatistics LastStats;
        public static FrameTimeMemorySnapshot LastMemory;
        public static bool LastProfilerEnabled;
        public static float LastUiLatencyP99Ms;
        public static float LastUiLatencyMaxMs;
        public static long LastUiLateTicks;
        public static long LastUiTicks;
        public static float LastGameTurnP95Ms;
        public static float LastGameTurnMaxMs;
        public static GHThermalStatus LastThermalStatus;
        public static float LastCpuPerformancePct;
        public static float LastBatteryTempC;

        public static void PublishFrameStats(in FrameTimeStreamingStatistics stats,
            in FrameTimeMemorySnapshot memory, bool profilerEnabled,
            float uiLatencyP99Ms, float uiLatencyMaxMs, long uiLateTicks, long uiTicks,
            float gameTurnP95Ms, float gameTurnMaxMs,
            GHThermalStatus thermalStatus, float cpuPerformancePct, float batteryTempC)
        {
            PublishCount++;
            LastStats = stats;
            LastMemory = memory;
            LastProfilerEnabled = profilerEnabled;
            LastUiLatencyP99Ms = uiLatencyP99Ms;
            LastUiLatencyMaxMs = uiLatencyMaxMs;
            LastUiLateTicks = uiLateTicks;
            LastUiTicks = uiTicks;
            LastGameTurnP95Ms = gameTurnP95Ms;
            LastGameTurnMaxMs = gameTurnMaxMs;
            LastThermalStatus = thermalStatus;
            LastCpuPerformancePct = cpuPerformancePct;
            LastBatteryTempC = batteryTempC;
        }

        public static void ResetRecording()
        {
            PublishCount = 0;
            LastStats = new FrameTimeStreamingStatistics();
            LastMemory = new FrameTimeMemorySnapshot();
            LastProfilerEnabled = false;
            LastUiLatencyP99Ms = 0;
            LastUiLatencyMaxMs = 0;
            LastUiLateTicks = 0;
            LastUiTicks = 0;
            LastGameTurnP95Ms = 0;
            LastGameTurnMaxMs = 0;
            LastThermalStatus = GHThermalStatus.Unknown;
            LastCpuPerformancePct = 0;
            LastBatteryTempC = 0;
        }
    }
}

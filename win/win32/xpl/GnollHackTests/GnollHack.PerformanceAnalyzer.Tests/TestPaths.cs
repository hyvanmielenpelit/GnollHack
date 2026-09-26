namespace GnollHack.PerformanceAnalyzer.Tests
{
    /* The sample captures under TestData, copied next to the test assembly.

       run1: 40 ticks of a 60 Hz panel paced at 60 FPS, every tick painted on the UI thread
       except the last four (GL thread). Frame 21 (record 20) draws for 28 ms, so the
       vsync after it has no callback and its content reaches the screen one refresh
       late: one hitch, charged to PaintCpu. The Gen 0 GC count advances at frame 31.

       presentmon_two_swapchains.csv: one present per run1 frame on swap chain A, 0.5 ms
       after the flush, shown 6 ms after the estimated vsync; frame 31's present is NA
       (never displayed). Five presents on swap chain B and three of another process.

       perfetto: SurfaceFlinger frames for every run1 tick token, presented 5 ms after the
       estimated vsync, trace clock = capture ms + 5,000,000 ms; frame 21 is "App Deadline
       Missed".

       gfxinfo_gap: three framestats polls of a 60 Hz stream, frames 0-9, 20-29 and
       25-34; frames 10-19 were never seen. */
    internal static class TestPaths
    {
        public static string Data(params string[] parts)
        {
            return Path.Combine(new[] { AppContext.BaseDirectory, "TestData" }.Concat(parts).ToArray());
        }

        public static string Run1Json { get { return Data("run1", "run.json"); } }
        public static string Run2Json { get { return Data("run2", "run.json"); } }
        public static string Run1Timeline { get { return Data("run1", "frametimeline_run1.csv"); } }
        public static string PresentMon { get { return Data("presentmon_two_swapchains.csv"); } }
        public static string PerfettoDir { get { return Data("perfetto"); } }
        public static string GfxinfoDir { get { return Data("gfxinfo_gap"); } }

        public const string SwapChainA = "0x0000021F4A3B2C40";
        public const string SwapChainB = "0x0000021F4A3B9E80";

        public static string TempFile(string extension)
        {
            return Path.Combine(Path.GetTempPath(), "ghpa_" + Guid.NewGuid().ToString("N") + extension);
        }
    }
}

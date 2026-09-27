namespace GnollHack.PerformanceAnalyzer.Tests
{
    /* The sample captures under TestData, copied next to the test assembly.

       run1: 40 ticks of a 60 Hz panel paced at 60 FPS, every tick painted on the UI thread
       except the last four (GL thread). Frame 21 (record 20) draws for 28 ms, so the
       vsync after it has no callback and its content reaches the screen one refresh
       late: one hitch, charged to PaintCpu. The Gen 0 GC count advances at frame 31.

       presentmon_v1.csv / presentmon_v2.csv: one present per run1 frame on swap chain A,
       0.5 ms after the flush, shown 6 ms after the estimated vsync; frame 31's present is
       never displayed (v1: Dropped=1; v2: DisplayLatency/DisplayedTime=NA). Five presents
       on swap chain B and three of another process. Both are generated from the same
       underlying presents, one under PresentMon 1.x column names and one under 2.x, so a
       join against either produces the same result.

       perfetto: SurfaceFlinger frames for every run1 tick token, presented 5 ms after the
       estimated vsync, trace clock = capture ms + 5,000,000 ms; frame 21 is "App Deadline
       Missed".

       gfxinfo_gap: three framestats polls of a 60 Hz stream, frames 0-9, 20-29 and
       25-34; frames 10-19 were never seen.

       typeperf/load_system_real.csv: a real typeperf capture (UTC+3, bias -180) of a
       process that was not running: the first row blank but for Available MBytes, the
       process column blank throughout.
       typeperf/load_system_busy.csv: twelve 1 s rows of GnollHackM from 13:56:09.876
       local, the first blank; then machine CPU 60 %, the app 80 % per core (10 % of 8
       logical processors), disk idle 90 %, 12000 MB available, 5 page reads/s; the
       file ends in a partial line.
       typeperf/load_processes.csv: one Get-Counter row with lower-case paths: _total,
       idle, devenv 40 + devenv#1 24, vmmemwsl 176, msmpeng 16, typeperf 1.6,
       gnollhackm 50 + gnollhackm#1 10 (per-core percent); 3D engines of luid ...e353
       12.5 + 5.0 and of luid ...f000 3.0, and a video-decode engine at 50.
       typeperf/fingerprint.json: a Get-EnvironmentFingerprint.ps1 file with 8 logical
       processors, 32 GB and device model "MSI Test".
       android/load_android.txt: three /proc blocks 1 s apart: system busy 40 % then
       25 %, the app 12 % in the first delta and unknown ("P ") in the second,
       MemAvailable 2 GB then 1 GB of 8 GB.
       android/load_android_top.txt: the "FALLBACK top" form, three iterations of
       800 %cpu with 800, 600 (+40 iow) and 560 idle. */
    internal static class TestPaths
    {
        public static string Data(params string[] parts)
        {
            return Path.Combine(new[] { AppContext.BaseDirectory, "TestData" }.Concat(parts).ToArray());
        }

        public static string Run1Json { get { return Data("run1", "run.json"); } }
        public static string Run2Json { get { return Data("run2", "run.json"); } }
        public static string Run1Timeline { get { return Data("run1", "frametimeline_run1.csv"); } }
        public static string PresentMonV1 { get { return Data("presentmon_v1.csv"); } }
        public static string PresentMonV2 { get { return Data("presentmon_v2.csv"); } }
        public static string PerfettoDir { get { return Data("perfetto"); } }
        public static string GfxinfoDir { get { return Data("gfxinfo_gap"); } }
        public static string TypeperfReal { get { return Data("typeperf", "load_system_real.csv"); } }
        public static string TypeperfBusy { get { return Data("typeperf", "load_system_busy.csv"); } }
        public static string TypeperfProcesses { get { return Data("typeperf", "load_processes.csv"); } }
        public static string FingerprintJson { get { return Data("typeperf", "fingerprint.json"); } }
        public static string AndroidProcStat { get { return Data("android", "load_android.txt"); } }
        public static string AndroidTop { get { return Data("android", "load_android_top.txt"); } }

        public const string SwapChainA = "0x0000021F4A3B2C40";
        public const string SwapChainB = "0x0000021F4A3B9E80";

        public static string TempFile(string extension)
        {
            return Path.Combine(Path.GetTempPath(), "ghpa_" + Guid.NewGuid().ToString("N") + extension);
        }
    }
}

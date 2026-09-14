namespace GnollHackX.Perf
{
    /* Test-side stand-in for the app's GHUiThreadProbe, which needs a MAUI
       dispatcher. FrameTimeProfiler only reads these four values when it publishes
       a dashboard snapshot; a test may set them to check they are passed through. */
    public static class GHUiThreadProbe
    {
        public static float LatencyP99Ms;
        public static float LatencyMaxMs;
        public static long LateTickCount;
        public static long TickCount;
    }
}

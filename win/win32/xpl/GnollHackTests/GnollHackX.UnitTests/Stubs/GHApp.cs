namespace GnollHackX
{
    /* Test-side stand-in for the app's GHApp. FrameTimeProfiler only asks whether
       screen logging is on and writes log lines through it; the stub keeps logging
       off so the GC event listener and the log formatting never run in tests. */
    public static class GHApp
    {
        public static bool IsDebugScreenLoggingOn
        {
            get { return false; }
        }

        public static void MaybeWriteScreenLog(string loggedText)
        {
        }
    }
}

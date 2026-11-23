namespace GnollHackX
{
    /* The newest process exit the platform recorded for this app */
    public sealed class GHPreviousProcessExit
    {
        public int Reason;              /* ApplicationExitInfo.REASON_* on Android */
        public int Importance;          /* ActivityManager.RunningAppProcessInfo.IMPORTANCE_* when the process died */
        public long TimestampUnixMs;
        public string StateSummary;     /* The process state summary the process left behind, or null */
    }

    public enum LowMemoryExitNoticeKind
    {
        None = 0,
        InBackground,
        InGame,
        OutsideGame,
    }

    /* Whether, and how, the player is told at startup that Android ended the previous session for lack of memory */
    public static class GHLowMemoryExitNotice
    {
        public const int ReasonLowMemory = 3;           /* ApplicationExitInfo.REASON_LOW_MEMORY */
        public const int ImportanceVisible = 200;       /* RunningAppProcessInfo.IMPORTANCE_VISIBLE */
        public const long MaxAgeMilliseconds = 7L * 24L * 60L * 60L * 1000L;

        public const string Title = "Out of Memory Previously";
        public const string BackgroundTerminationText = "Android closed it because your device ran low on memory.";
        private const string Advice = " Close the apps you are not using before you play. If it keeps happening, restarting your device can also free further memory.";

        /* firstLaunchOfThisBuild: the first launch after an update or a fresh install, when the exit may predate this build */
        public static LowMemoryExitNoticeKind Classify(GHPreviousProcessExit exit, bool previousSessionSuspended, bool firstLaunchOfThisBuild, long lastShownUnixMs, long nowUnixMs)
        {
            if (exit == null || exit.Reason != ReasonLowMemory || firstLaunchOfThisBuild)
                return LowMemoryExitNoticeKind.None;
            if (exit.TimestampUnixMs <= lastShownUnixMs || nowUnixMs - exit.TimestampUnixMs > MaxAgeMilliseconds)
                return LowMemoryExitNoticeKind.None;
            if (previousSessionSuspended || exit.Importance > ImportanceVisible)
                return LowMemoryExitNoticeKind.InBackground;
            GHExitStateSummary summary;
            if (GHExitStateSummary.TryParse(exit.StateSummary, out summary) && (summary.GameState == 1 || summary.GameState == 3))
                return LowMemoryExitNoticeKind.InGame;
            return LowMemoryExitNoticeKind.OutsideGame;
        }

        /* Null for the kinds that have no notice of their own */
        public static string Message(LowMemoryExitNoticeKind kind)
        {
            switch (kind)
            {
                case LowMemoryExitNoticeKind.InGame:
                    return "Android closed GnollHack while you were playing because your device ran out of memory. Your game may be recoverable from the crash." + Advice;
                case LowMemoryExitNoticeKind.OutsideGame:
                    return "Android closed GnollHack because your device ran out of memory." + Advice;
                default:
                    return null;
            }
        }
    }
}

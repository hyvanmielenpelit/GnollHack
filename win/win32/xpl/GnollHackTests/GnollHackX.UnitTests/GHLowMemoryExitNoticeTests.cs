using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHLowMemoryExitNotice: which previous exits produce a startup notice, and its text */
    public class GHLowMemoryExitNoticeTests
    {
        private const long Now = 1795431404000L;
        private const long ExitTime = Now - 60000L;
        private const string InGameSummary = "gs1 u=1795431404 g=1 t=1792 d=1789";
        private const string LoadingSummary = "gs1 u=1795431404 g=3 t=10";
        private const string MenuSummary = "gs1 u=1795420603 g=0 a=18";
        private const string ReplaySummary = "gs1 u=1795420603 g=2 t=50";
        private const int Foreground = 100;
        private const int Cached = 400;

        private static GHPreviousProcessExit Exit(int reason, int importance, long timestamp, string summary)
        {
            GHPreviousProcessExit exit = new GHPreviousProcessExit();
            exit.Reason = reason;
            exit.Importance = importance;
            exit.TimestampUnixMs = timestamp;
            exit.StateSummary = summary;
            return exit;
        }

        private static GHPreviousProcessExit LowMemoryExit(string summary)
        {
            return Exit(GHLowMemoryExitNotice.ReasonLowMemory, Foreground, ExitTime, summary);
        }

        [Fact]
        public void NoExit_GivesNone()
        {
            Assert.Equal(LowMemoryExitNoticeKind.None, GHLowMemoryExitNotice.Classify(null, false, false, 0L, Now));
        }

        [Fact]
        public void OtherReason_GivesNone()
        {
            GHPreviousProcessExit exit = Exit(2, Foreground, ExitTime, InGameSummary);
            Assert.Equal(LowMemoryExitNoticeKind.None, GHLowMemoryExitNotice.Classify(exit, false, false, 0L, Now));
        }

        [Fact]
        public void AlreadyShown_GivesNone()
        {
            Assert.Equal(LowMemoryExitNoticeKind.None, GHLowMemoryExitNotice.Classify(LowMemoryExit(InGameSummary), false, false, ExitTime, Now));
        }

        [Fact]
        public void OlderThanMaxAge_GivesNone()
        {
            GHPreviousProcessExit exit = Exit(GHLowMemoryExitNotice.ReasonLowMemory, Foreground, Now - GHLowMemoryExitNotice.MaxAgeMilliseconds - 1L, InGameSummary);
            Assert.Equal(LowMemoryExitNoticeKind.None, GHLowMemoryExitNotice.Classify(exit, false, false, 0L, Now));
        }

        [Fact]
        public void PreviousSessionSuspended_GivesInBackground()
        {
            Assert.Equal(LowMemoryExitNoticeKind.InBackground, GHLowMemoryExitNotice.Classify(LowMemoryExit(InGameSummary), true, false, 0L, Now));
        }

        [Fact]
        public void CachedImportance_GivesInBackground()
        {
            GHPreviousProcessExit exit = Exit(GHLowMemoryExitNotice.ReasonLowMemory, Cached, ExitTime, InGameSummary);
            Assert.Equal(LowMemoryExitNoticeKind.InBackground, GHLowMemoryExitNotice.Classify(exit, false, false, 0L, Now));
        }

        [Fact]
        public void FirstLaunchOfThisBuild_GivesNone()
        {
            Assert.Equal(LowMemoryExitNoticeKind.None, GHLowMemoryExitNotice.Classify(LowMemoryExit(InGameSummary), false, true, 0L, Now));
        }

        [Theory]
        [InlineData(InGameSummary)]
        [InlineData(LoadingSummary)]
        public void GameOrGameLoading_GivesInGame(string summary)
        {
            Assert.Equal(LowMemoryExitNoticeKind.InGame, GHLowMemoryExitNotice.Classify(LowMemoryExit(summary), false, false, 0L, Now));
        }

        [Theory]
        [InlineData(MenuSummary)]
        [InlineData(ReplaySummary)]
        [InlineData(null)]
        [InlineData("gnh-close-application")]
        public void NoGameOrNoSnapshot_GivesOutsideGame(string summary)
        {
            Assert.Equal(LowMemoryExitNoticeKind.OutsideGame, GHLowMemoryExitNotice.Classify(LowMemoryExit(summary), false, false, 0L, Now));
        }

        [Fact]
        public void Message_ExistsOnlyForForegroundKinds()
        {
            Assert.Contains("while you were playing", GHLowMemoryExitNotice.Message(LowMemoryExitNoticeKind.InGame));
            Assert.Contains("recoverable", GHLowMemoryExitNotice.Message(LowMemoryExitNoticeKind.InGame));
            Assert.DoesNotContain("recoverable", GHLowMemoryExitNotice.Message(LowMemoryExitNoticeKind.OutsideGame));
            Assert.Contains("apps you are not using", GHLowMemoryExitNotice.Message(LowMemoryExitNoticeKind.OutsideGame));
            Assert.Null(GHLowMemoryExitNotice.Message(LowMemoryExitNoticeKind.None));
            Assert.Null(GHLowMemoryExitNotice.Message(LowMemoryExitNoticeKind.InBackground));
        }
    }
}

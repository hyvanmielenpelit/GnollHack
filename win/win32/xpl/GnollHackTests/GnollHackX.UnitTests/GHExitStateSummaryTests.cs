using System;
using System.Text;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHExitStateSummary: the wire format and its 128-byte cap, tolerant parsing,
       the readable description, content comparison and the rated-memory buckets. */
    public class GHExitStateSummaryTests
    {
        private const ulong GiB = 1024UL * 1024UL * 1024UL;

        private static GHExitStateSummary Full()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = 1794567890;
            s.GameState = 1;
            s.SecondsInGame = 245;
            s.ProcessAgeSeconds = 300;
            s.StartupMilestone = (int)GHStartupMilestone.MainMenu;
            s.StartupMilestoneAgeSeconds = 290;
            s.StartupMilestoneMask = 2046;
            s.MemoryWarningLevel = 3;
            s.MemoryWarningAgeSeconds = 12;
            s.HighestMemoryWarningLevel = 3;
            s.RssMB = 1530;
            s.SwapMB = 610;
            s.PssMB = 2100;
            s.GraphicsMB = 320;
            s.NativeHeapMB = 900;
            s.PssSampleAgeSeconds = 40;
            s.GpuCacheUsedMB = 380;
            s.GpuCacheLimitMB = 384;
            s.BitmapMB = 570;
            s.GcNetLongestMs = 45;
            s.GcJavaLongestMs = 180;
            s.GcJavaCount = 12;
            s.SoundMB = 120;
            s.ManagedHeapMB = 40;
            return s;
        }

        /* What a compact startup field parses back to */
        private static void DoneStartup(GHExitStateSummary s)
        {
            s.StartupMilestone = (int)GHStartupMilestone.MainMenu;
            s.StartupMilestoneAgeSeconds = -1;
            s.StartupMilestoneMask = 1 << (int)GHStartupMilestone.MainMenu;
        }

        [Fact]
        public void Format_AllFields_RoundTrips()
        {
            GHExitStateSummary s = Full();
            string text = s.Format();

            Assert.Equal("gs1 u=1794567890 g=1 t=245 a=300 w=3/12/3 r=1530 s=610 p=2100/320/900/40 c=380/384 b=570 x=45/180/12 h=40 f=120", text);
            Assert.Equal(111, Encoding.ASCII.GetByteCount(text));
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));
            DoneStartup(s);
            Assert.True(s.ContentEquals(parsed));
            Assert.Equal(s.UnixSeconds, parsed.UnixSeconds);
        }

        [Fact]
        public void Format_HugeValues_StaysWithinMaxBytesAndKeepsLeadingFields()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = long.MaxValue;
            s.GameState = int.MaxValue;
            s.SecondsInGame = long.MaxValue;
            s.SecondsSinceLoaded = long.MaxValue;
            s.ProcessAgeSeconds = long.MaxValue;
            s.StartupMilestone = int.MaxValue;
            s.StartupMilestoneAgeSeconds = long.MaxValue;
            s.StartupMilestoneMask = int.MaxValue;
            s.MemoryWarningLevel = int.MaxValue;
            s.MemoryWarningAgeSeconds = long.MaxValue;
            s.HighestMemoryWarningLevel = int.MaxValue;
            s.RssMB = long.MaxValue;
            s.SwapMB = long.MaxValue;
            s.PssMB = long.MaxValue;
            s.GraphicsMB = long.MaxValue;
            s.NativeHeapMB = long.MaxValue;
            s.PssSampleAgeSeconds = long.MaxValue;
            s.GpuCacheUsedMB = long.MaxValue;
            s.GpuCacheLimitMB = long.MaxValue;
            s.GpuCacheSampleAgeSeconds = long.MaxValue;
            s.BitmapMB = long.MaxValue;
            s.TileSheetMB = long.MaxValue;
            s.SoundMB = long.MaxValue;
            s.ManagedHeapMB = long.MaxValue;

            string text = s.Format();
            byte[] bytes = Encoding.ASCII.GetBytes(text);

            Assert.True(bytes.Length <= GHExitStateSummary.MaxBytes);
            Assert.Equal(text, Encoding.ASCII.GetString(bytes));
            Assert.DoesNotContain("\0", text);
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));
            Assert.Equal(long.MaxValue, parsed.UnixSeconds);
            Assert.Equal(int.MaxValue, parsed.GameState);
            Assert.Equal(long.MaxValue, parsed.SecondsInGame);
        }

        private static GHExitStateSummary FullWithLoadAndSheets()
        {
            GHExitStateSummary s = Full();
            s.SecondsSinceLoaded = 70;
            s.GpuCacheSampleAgeSeconds = 9;
            s.TileSheetMB = 570;
            return s;
        }

        [Fact]
        public void Format_LoadAndSheetFields_RoundTrip()
        {
            GHExitStateSummary s = FullWithLoadAndSheets();
            string text = s.Format();

            Assert.Equal("gs1 u=1794567890 g=1 t=245 d=70 a=300 w=3/12/3 r=1530 s=610 p=2100/320/900/40 c=380/384/9 b=570/570 x=45/180/12 h=40 f=120", text);
            Assert.Equal(122, Encoding.ASCII.GetByteCount(text));
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));
            DoneStartup(s);
            Assert.True(s.ContentEquals(parsed));
            Assert.Equal(70, parsed.SecondsSinceLoaded);
            Assert.Equal(9, parsed.GpuCacheSampleAgeSeconds);
            Assert.Equal(570, parsed.TileSheetMB);
        }

        [Fact]
        public void Format_RealisticLongGame_KeepsEveryField()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = 1794567890;
            s.GameState = 1;
            s.SecondsInGame = 3600;
            s.SecondsSinceLoaded = 3590;
            s.ProcessAgeSeconds = 3700;
            s.StartupMilestone = (int)GHStartupMilestone.MainMenu;
            s.StartupMilestoneAgeSeconds = 3650;
            s.StartupMilestoneMask = 2046;
            s.MemoryWarningLevel = 3;
            s.MemoryWarningAgeSeconds = 120;
            s.HighestMemoryWarningLevel = 3;
            s.RssMB = 1500;
            s.SwapMB = 20;
            s.PssMB = 1600;
            s.GraphicsMB = 400;
            s.NativeHeapMB = 840;
            s.PssSampleAgeSeconds = 0;
            s.GpuCacheUsedMB = 180;
            s.GpuCacheLimitMB = 256;
            s.GpuCacheSampleAgeSeconds = 9;
            s.BitmapMB = 705;
            s.TileSheetMB = 570;
            s.GcNetLongestMs = 300;
            s.GcJavaLongestMs = 2500;
            s.GcJavaCount = 45;
            s.SoundMB = 11;
            s.ManagedHeapMB = 80;

            string text = s.Format();

            Assert.Equal(126, Encoding.ASCII.GetByteCount(text));
            Assert.True(Encoding.ASCII.GetByteCount(text) <= GHExitStateSummary.MaxBytes);
            Assert.EndsWith(" f=11", text);
        }

        [Fact]
        public void Format_HeavyLongGame_KeepsGcFieldBeforeHeapAndSound()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = 1794567890;
            s.GameState = 1;
            s.SecondsInGame = 36000;
            s.SecondsSinceLoaded = 35990;
            s.ProcessAgeSeconds = 36100;
            s.StartupMilestone = (int)GHStartupMilestone.MainMenu;
            s.StartupMilestoneAgeSeconds = 36050;
            s.StartupMilestoneMask = 2046;
            s.MemoryWarningLevel = 4;
            s.MemoryWarningAgeSeconds = 0;
            s.HighestMemoryWarningLevel = 5;
            s.RssMB = 1500;
            s.SwapMB = 1165;
            s.PssMB = 2100;
            s.GraphicsMB = 643;
            s.NativeHeapMB = 835;
            s.PssSampleAgeSeconds = 300;
            s.GpuCacheUsedMB = 596;
            s.GpuCacheLimitMB = 768;
            s.GpuCacheSampleAgeSeconds = 120;
            s.BitmapMB = 708;
            s.TileSheetMB = 600;
            s.GcNetLongestMs = 21000;
            s.GcJavaLongestMs = 30000;
            s.GcJavaCount = 240;
            s.ManagedHeapMB = 120;
            s.SoundMB = 21;

            string text = s.Format();

            Assert.Equal(127, Encoding.ASCII.GetByteCount(text));
            Assert.EndsWith(" x=21000/30000/240", text);
            Assert.DoesNotContain(" h=", text);
            Assert.DoesNotContain(" f=", text);
        }

        [Fact]
        public void TryParse_OldFieldOrder_StillParses()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=5 g=1 b=570 f=120 h=40", out parsed));

            Assert.Equal(120, parsed.SoundMB);
            Assert.Equal(40, parsed.ManagedHeapMB);
        }

        [Fact]
        public void TryParse_OnePartStartupField_IsDone()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=5 g=1 a=3700", out parsed));

            Assert.Equal(3700, parsed.ProcessAgeSeconds);
            Assert.Equal("done", parsed.StartupTagValue());
            Assert.Contains("process 3700 s; startup done", parsed.Describe(0));
        }

        [Fact]
        public void TryParse_OldFourPartStartupField_StillParses()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=5 g=1 a=3700/10/3650/2046", out parsed));

            Assert.Equal(3700, parsed.ProcessAgeSeconds);
            Assert.Equal(10, parsed.StartupMilestone);
            Assert.Equal(3650, parsed.StartupMilestoneAgeSeconds);
            Assert.Equal(2046, parsed.StartupMilestoneMask);
        }

        [Fact]
        public void Describe_GcNoneSeenAndUnknownParts()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.GcJavaCount = 0;
            Assert.Equal("GC in last 10 min: none seen", s.Describe(0));

            s.GcJavaLongestMs = 700;
            s.GcJavaCount = 3;
            Assert.Equal("GC in last 10 min: longest .NET pause ? ms, longest Java GC 700 ms, 3 Java GCs", s.Describe(0));
        }

        [Theory]
        [InlineData(-1L, -1L, "unknown")]
        [InlineData(50L, -1L, "under 100 ms")]
        [InlineData(-1L, 100L, "100-999 ms")]
        [InlineData(999L, 1000L, "1-4.9 s")]
        [InlineData(5000L, 10L, "5 s or more")]
        public void GcLongestBucket_Buckets(long netLongestMs, long javaLongestMs, string expected)
        {
            Assert.Equal(expected, GHExitStateSummary.GcLongestBucket(netLongestMs, javaLongestMs));
        }

        [Fact]
        public void TryParse_OldGpuCacheAndBitmapFields_LeavesNewPartsAbsent()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=5 g=1 c=8/512 b=705", out parsed));

            Assert.Equal(8, parsed.GpuCacheUsedMB);
            Assert.Equal(512, parsed.GpuCacheLimitMB);
            Assert.Equal(-1, parsed.GpuCacheSampleAgeSeconds);
            Assert.Equal(705, parsed.BitmapMB);
            Assert.Equal(-1, parsed.TileSheetMB);
            Assert.Equal(-1, parsed.SecondsSinceLoaded);
            string text = parsed.Describe(0);
            Assert.DoesNotContain("sampled", text);
            Assert.DoesNotContain("tile sheets", text);
            Assert.DoesNotContain("loaded", text);
        }

        [Theory]
        [InlineData(0, "none")]
        [InlineData(1, "game")]
        [InlineData(2, "replay")]
        [InlineData(3, "game-loading")]
        [InlineData(4, "replay-loading")]
        [InlineData(5, "unknown")]
        public void GameStateName_NamesStates(int state, string expected)
        {
            Assert.Equal(expected, GHExitStateSummary.GameStateName(state));
        }

        [Fact]
        public void Describe_LoadAndSheetFields()
        {
            GHExitStateSummary s = FullWithLoadAndSheets();

            string text = s.Describe(s.UnixSeconds + 8);

            Assert.StartsWith("game, 245 s in, loaded 70 s before snapshot; process 300 s;", text);
            Assert.Contains("GPU cache 380 / 384 MB sampled 9 s before snapshot; bitmaps 570 MB (tile sheets 570); "
                + "GC in last 10 min: longest .NET pause 45 ms, longest Java GC 180 ms, 12 Java GCs; sound", text);
        }

        [Fact]
        public void ContentEquals_SecondsSinceLoadedDiffers_IsFalse()
        {
            GHExitStateSummary a = FullWithLoadAndSheets();
            GHExitStateSummary b = FullWithLoadAndSheets();
            b.SecondsSinceLoaded = a.SecondsSinceLoaded + 1;

            Assert.False(a.ContentEquals(b));
        }

        [Fact]
        public void Format_OnlySomeFields_OmitsTheRest()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = 100;
            s.GameState = 0;

            string text = s.Format();

            Assert.Equal("gs1 u=100 g=0", text);
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));
            Assert.Equal(-1, parsed.SecondsInGame);
            Assert.Equal(-1, parsed.MemoryWarningLevel);
            Assert.Equal(-1, parsed.RssMB);
            Assert.Equal(-1, parsed.PssMB);
            Assert.Equal(-1, parsed.ManagedHeapMB);
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("gnh-close-application")]
        [InlineData("gs2 u=1")]
        [InlineData("gs1")]
        public void TryParse_NotASnapshot_ReturnsFalse(string text)
        {
            GHExitStateSummary parsed;
            Assert.False(GHExitStateSummary.TryParse(text, out parsed));
            Assert.Null(parsed);
        }

        [Fact]
        public void TryParse_UnknownKeysMalformedNumbersAndAnyOrder_AreTolerated()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=5 zz=9 r=abc g=1", out parsed));
            Assert.Equal(5, parsed.UnixSeconds);
            Assert.Equal(1, parsed.GameState);
            Assert.Equal(-1, parsed.RssMB);
        }

        [Fact]
        public void Describe_FullSnapshot_ReadsAsPlanned()
        {
            GHExitStateSummary s = Full();

            string text = s.Describe(s.UnixSeconds + 8);

            Assert.Equal("game, 245 s in; process 300 s; startup done; warning Critical 12 s before snapshot, highest Critical; rss 1530 MB, swap 610 MB; "
                + "pss 2100 MB (graphics 320, native 900) 40 s before snapshot; GPU cache 380 / 384 MB; bitmaps 570 MB; "
                + "GC in last 10 min: longest .NET pause 45 ms, longest Java GC 180 ms, 12 Java GCs; "
                + "sound 120 MB; managed heap 40 MB; snapshot 8 s before exit", text);
        }

        [Fact]
        public void Describe_NoWarningReplayAndNoExitTime()
        {
            GHExitStateSummary s = Full();
            s.GameState = 2;
            s.MemoryWarningLevel = 0;

            string text = s.Describe(0);

            Assert.StartsWith("replay", text);
            Assert.Contains("no memory warning", text);
            Assert.DoesNotContain("before exit", text);
        }

        [Fact]
        public void Describe_NegativeAge_PrintsZero()
        {
            GHExitStateSummary s = Full();

            Assert.Contains("snapshot 0 s before exit", s.Describe(s.UnixSeconds - 30));
        }

        [Fact]
        public void TryParse_SnapshotWithoutStartupField_LeavesItAbsent()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=100 g=1 t=30 r=1362", out parsed));

            Assert.Equal(-1, parsed.ProcessAgeSeconds);
            Assert.Equal(-1, parsed.StartupMilestone);
            Assert.Equal(-1, parsed.StartupMilestoneAgeSeconds);
            Assert.Equal(-1, parsed.StartupMilestoneMask);
            Assert.Null(parsed.StartupTagValue());
            Assert.DoesNotContain("process", parsed.Describe(0));
            Assert.DoesNotContain("startup", parsed.Describe(0));
        }

        [Fact]
        public void StartupField_InProgress_RoundTripsAndNamesWhatIsMissing()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = 100;
            s.GameState = 0;
            s.ProcessAgeSeconds = 12;
            s.StartupMilestone = (int)GHStartupMilestone.AppReady;
            s.StartupMilestoneAgeSeconds = 3;
            s.StartupMilestoneMask = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);

            string text = s.Format();
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));

            Assert.Equal("gs1 u=100 g=0 a=12/5/3/62", text);
            Assert.True(s.ContentEquals(parsed));
            Assert.Equal("app-ready", parsed.StartupTagValue());
            Assert.Equal("none; process 12 s; startup at app-ready 3 s before snapshot, "
                + "not reached: activity, focused, main-page, startup-tasks, main-menu", parsed.Describe(0));
        }

        [Fact]
        public void StartupField_MainPageBeforeActivity_ListsFromTheMask()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.ProcessAgeSeconds = 9;
            s.StartupMilestone = (int)GHStartupMilestone.MainPage;
            s.StartupMilestoneAgeSeconds = 6;
            s.StartupMilestoneMask = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 8);

            Assert.Equal("main-page", s.StartupTagValue());
            Assert.Equal("process 9 s; startup at main-page 6 s before snapshot, "
                + "not reached: activity, focused, startup-tasks, main-menu", s.Describe(0));
        }

        [Fact]
        public void StartupField_WithWriterMaxCode_RoundTripsAndListsNewMilestones()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.UnixSeconds = 100;
            s.GameState = 0;
            s.ProcessAgeSeconds = 12;
            s.StartupMilestone = (int)GHStartupMilestone.AppReady;
            s.StartupMilestoneAgeSeconds = 3;
            s.StartupMilestoneMask = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)
                | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 14);
            s.StartupMilestoneMaxCode = GHStartupTimeline.MaxMilestone;

            string text = s.Format();
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));

            Assert.Equal("gs1 u=100 g=0 a=12/5/3/30782/15", text);
            Assert.True(s.ContentEquals(parsed));
            Assert.Equal("none; process 12 s; startup at app-ready 3 s before snapshot, "
                + "not reached: activity, first-draw, focused, main-page, startup-tasks, main-menu", parsed.Describe(0));
        }

        [Fact]
        public void StartupField_LegacySnapshot_ListsOnlyItsOwnMilestones()
        {
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse("gs1 u=100 g=0 a=12/7/0/254", out parsed));

            string description = parsed.Describe(0);
            Assert.Equal(-1, parsed.StartupMilestoneMaxCode);
            Assert.EndsWith("not reached: main-page, startup-tasks, main-menu", description);
            Assert.DoesNotContain("first-draw", description);
            Assert.DoesNotContain("maui-app", description);
            Assert.DoesNotContain("base-assets", description);
        }

        [Fact]
        public void StartupTagValue_MainMenuReached_IsDone()
        {
            GHExitStateSummary s = new GHExitStateSummary();
            s.ProcessAgeSeconds = 40;
            s.StartupMilestone = (int)GHStartupMilestone.Focused;
            s.StartupMilestoneAgeSeconds = 30;
            s.StartupMilestoneMask = 1 << (int)GHStartupMilestone.MainMenu;

            Assert.Equal("done", s.StartupTagValue());
            Assert.Equal("process 40 s; startup done", s.Describe(0));
        }

        [Fact]
        public void ContentEquals_IgnoresOnlyTheTime()
        {
            GHExitStateSummary a = Full();
            GHExitStateSummary b = Full();
            b.UnixSeconds = a.UnixSeconds + 15;
            Assert.True(a.ContentEquals(b));

            b.RssMB = a.RssMB + 1;
            Assert.False(a.ContentEquals(b));
            Assert.False(a.ContentEquals(null));

            GHExitStateSummary c = Full();
            c.GcJavaCount = a.GcJavaCount + 1;
            Assert.False(a.ContentEquals(c));
        }

        [Theory]
        [InlineData(0.0, "unknown")]
        [InlineData(1.8, "2 GB or less")]
        [InlineData(2.7, "3 GB")]
        [InlineData(3.0, "4 GB")]
        [InlineData(3.44, "4 GB")]
        [InlineData(5.3, "6 GB")]
        [InlineData(7.3, "8 GB")]
        [InlineData(11.0, "12 GB")]
        [InlineData(15.0, "16 GB+")]
        public void MemoryBucket_MapsTotalMemToRatedSize(double gib, string expected)
        {
            Assert.Equal(expected, GHExitStateSummary.MemoryBucket((ulong)(gib * GiB)));
        }

        [Theory]
        [InlineData(0, "none")]
        [InlineData(1, "Low")]
        [InlineData(3, "Critical")]
        [InlineData(5, "Complete")]
        [InlineData(6, "unknown")]
        [InlineData(-1, "unknown")]
        public void MemoryWarningName_CodesLevels(int level, string expected)
        {
            Assert.Equal(expected, GHExitStateSummary.MemoryWarningName(level));
        }
    }
}

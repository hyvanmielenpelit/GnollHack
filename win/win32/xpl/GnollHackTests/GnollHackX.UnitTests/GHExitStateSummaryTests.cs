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
            s.SoundMB = 120;
            s.ManagedHeapMB = 40;
            return s;
        }

        [Fact]
        public void Format_AllFields_RoundTrips()
        {
            GHExitStateSummary s = Full();
            string text = s.Format();

            Assert.Equal("gs1 u=1794567890 g=1 t=245 w=3/12/3 r=1530 s=610 p=2100/320/900/40 c=380/384 b=570 f=120 h=40", text);
            GHExitStateSummary parsed;
            Assert.True(GHExitStateSummary.TryParse(text, out parsed));
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
            s.BitmapMB = long.MaxValue;
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

            Assert.Equal("game, 245 s in; warning Critical 12 s before snapshot, highest Critical; rss 1530 MB, swap 610 MB; "
                + "pss 2100 MB (graphics 320, native 900) 40 s before snapshot; GPU cache 380 / 384 MB; bitmaps 570 MB; "
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
        public void ContentEquals_IgnoresOnlyTheTime()
        {
            GHExitStateSummary a = Full();
            GHExitStateSummary b = Full();
            b.UnixSeconds = a.UnixSeconds + 15;
            Assert.True(a.ContentEquals(b));

            b.RssMB = a.RssMB + 1;
            Assert.False(a.ContentEquals(b));
            Assert.False(a.ContentEquals(null));
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

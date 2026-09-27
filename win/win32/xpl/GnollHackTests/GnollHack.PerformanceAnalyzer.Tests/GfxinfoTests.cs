using GnollHack.PerformanceAnalyzer.Model;
using GnollHack.PerformanceAnalyzer.Readers;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class GfxinfoTests
    {
        [Fact]
        public void ReadDirectory_DoesNotComputeAnIntervalAcrossLostFrames()
        {
            Series s = GfxinfoFramestats.ReadDirectory(TestPaths.GfxinfoDir);

            Assert.Equal("3", s.Info["polls"]);
            Assert.Equal("25", s.Info["uniqueFrames"]);
            Assert.Equal("1", s.Info["lostGapBoundaries"]);
            /* 9 intervals in frames 0-9, 14 in frames 20-34; none across 9 -> 20 */
            Assert.Equal(23, s.IntervalsMs.Length);
            Assert.All(s.IntervalsMs, v => Assert.InRange(v, 16.6f, 16.7f));
            Assert.Equal(25, s.FrameDurationsMs.Length);
        }

        [Fact]
        public void ReadFile_SinglePollHasNoLostGap()
        {
            Series s = GfxinfoFramestats.ReadFile(TestPaths.Data("gfxinfo_gap", "framestats_0002.txt"));
            Assert.Equal("0", s.Info["lostGapBoundaries"]);
            Assert.Equal(9, s.IntervalsMs.Length);
        }
    }
}

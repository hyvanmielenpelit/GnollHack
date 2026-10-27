using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHMapFps: refresh-rate snapping, the offered Map FPS values, the default divisor
       and default, resolving a stored setting, and the animation factors, which must equal the
       per-rate values the game used before the Map FPS setting became an integer. */
    public class GHMapFpsTests
    {
        [Theory]
        [InlineData(30, new int[] { 30 })]
        [InlineData(48, new int[] { 24, 48 })]
        [InlineData(50, new int[] { 25, 50 })]
        [InlineData(60, new int[] { 20, 30, 60 })]
        [InlineData(72, new int[] { 24, 36, 72 })]
        [InlineData(75, new int[] { 25, 75 })]
        [InlineData(80, new int[] { 20, 40, 80 })]
        [InlineData(90, new int[] { 30, 45, 90 })]
        [InlineData(100, new int[] { 20, 25, 50, 100 })]
        [InlineData(120, new int[] { 20, 24, 30, 40, 60, 120 })]
        [InlineData(144, new int[] { 24, 36, 48, 72, 144 })]
        [InlineData(150, new int[] { 25, 30, 50, 75, 150 })]
        [InlineData(165, new int[] { 33, 55, 165 })]
        [InlineData(180, new int[] { 20, 30, 36, 45, 60, 90, 180 })]
        [InlineData(200, new int[] { 20, 25, 40, 50, 100, 200 })]
        [InlineData(240, new int[] { 20, 24, 30, 40, 48, 60, 80, 120, 240 })]
        [InlineData(250, new int[] { 25, 50, 125, 250 })]
        [InlineData(280, new int[] { 20, 28, 35, 40, 56, 70, 140, 280 })]
        [InlineData(300, new int[] { 20, 25, 30, 50, 60, 75, 100, 150, 300 })]
        [InlineData(360, new int[] { 20, 24, 30, 36, 40, 45, 60, 72, 90, 120, 180, 360 })]
        [InlineData(15, new int[] { 15 })]
        [InlineData(0, new int[] { })]
        public void GetOfferedFps_ListsTheRateAndItsDivisions(int refreshRate, int[] expected)
        {
            Assert.Equal(new List<int>(expected), GHMapFps.GetOfferedFps(refreshRate));
        }

        [Fact]
        public void IsOffered_AgreesWithGetOfferedFps()
        {
            int rate, fps;
            for (rate = 1; rate <= 500; rate++)
            {
                List<int> offered = GHMapFps.GetOfferedFps(rate);
                for (fps = 1; fps <= 500; fps++)
                    Assert.True(offered.Contains(fps) == GHMapFps.IsOffered(fps, rate), "rate " + rate + ", fps " + fps);
            }
        }

        [Theory]
        [InlineData(59, 60)]
        [InlineData(61, 60)]
        [InlineData(119, 120)]
        [InlineData(143, 144)]
        [InlineData(89, 90)]
        [InlineData(359, 360)]
        [InlineData(95, 95)]
        [InlineData(85, 85)]
        [InlineData(0, 0)]
        public void SnapRefreshRate_SnapsNearStandardRates(int rate, int expected)
        {
            Assert.Equal(expected, GHMapFps.SnapRefreshRate(rate));
        }

        [Theory]
        [InlineData(60, 1)]
        [InlineData(99, 1)]
        [InlineData(100, 2)]
        [InlineData(149, 2)]
        [InlineData(150, 3)]
        [InlineData(199, 3)]
        [InlineData(200, 4)]
        [InlineData(249, 4)]
        [InlineData(250, 5)]
        [InlineData(299, 5)]
        [InlineData(300, 6)]
        [InlineData(480, 6)]
        public void GetDefaultDivisor_StepsEvery50HzFrom100(int rate, int expected)
        {
            Assert.Equal(expected, GHMapFps.GetDefaultDivisor(rate));
        }

        [Theory]
        [InlineData(30, 30, 30)]
        [InlineData(48, 48, 48)]
        [InlineData(60, 60, 60)]
        [InlineData(72, 72, 36)]
        [InlineData(75, 75, 25)]
        [InlineData(80, 80, 40)]
        [InlineData(90, 90, 45)]
        [InlineData(99, 99, 33)]
        [InlineData(100, 50, 50)]
        [InlineData(101, 101, 101)]
        [InlineData(120, 60, 60)]
        [InlineData(144, 72, 48)]
        [InlineData(150, 50, 50)]
        [InlineData(165, 55, 55)]
        [InlineData(180, 60, 60)]
        [InlineData(200, 50, 50)]
        [InlineData(240, 60, 60)]
        [InlineData(250, 50, 50)]
        [InlineData(280, 56, 56)]
        [InlineData(300, 50, 50)]
        [InlineData(360, 60, 60)]
        [InlineData(0, 60, 60)]
        public void GetDefaultFps_FollowsTheDivisorAndTheTimerLoopCap(int rate, int withPlatformLoop, int withoutPlatformLoop)
        {
            Assert.Equal(withPlatformLoop, GHMapFps.GetDefaultFps(rate, true));
            Assert.Equal(withoutPlatformLoop, GHMapFps.GetDefaultFps(rate, false));
        }

        [Fact]
        public void GetDefaultFps_IsAlwaysOffered()
        {
            int rate;
            for (rate = 1; rate <= 500; rate++)
            {
                Assert.True(GHMapFps.IsOffered(GHMapFps.GetDefaultFps(rate, true), rate), "platform loop, rate " + rate);
                Assert.True(GHMapFps.IsOffered(GHMapFps.GetDefaultFps(rate, false), rate), "timer loop, rate " + rate);
            }
        }

        [Theory]
        [InlineData(45, 90, 45)]
        [InlineData(60, 144, 72)]
        [InlineData(72, 144, 72)]
        [InlineData(-1, 144, 72)]
        [InlineData(-1, 90, 90)]
        [InlineData(1000, 60, 60)]
        [InlineData(90, 0, 60)]
        [InlineData(30, 0, 30)]
        [InlineData(-1, 0, 60)]
        public void GetEffectiveFps_KeepsOfferedSettingsOnly(int setting, int rate, int expected)
        {
            Assert.Equal(expected, GHMapFps.GetEffectiveFps(setting, rate, true));
        }

        /* The values of the per-rate switch statements that the formulas replace */
        [Theory]
        [InlineData(20, 2.0, 3.0)]
        [InlineData(30, 1.0, 2.0)]
        [InlineData(40, 1.0, 1.5)]
        [InlineData(60, 0.5, 1.0)]
        [InlineData(72, 0.5, 5.0 / 6.0)]
        [InlineData(80, 0.5, 0.75)]
        [InlineData(90, 1.0 / 3.0, 2.0 / 3.0)]
        [InlineData(120, 1.0 / 3.0, 0.5)]
        [InlineData(144, 1.0 / 4.0, 5.0 / 12.0)]
        [InlineData(45, 0.5, 4.0 / 3.0)]
        public void AnimationFactors_MatchThePerRateValues(int fps, double tileFrameSpeed, double guiMultiplier)
        {
            Assert.Equal(tileFrameSpeed, GHMapFps.GetTileAnimationFrameSpeed(fps), 10);
            Assert.Equal(guiMultiplier, GHMapFps.GetGuiAnimationMultiplier(fps), 10);
        }

        /* The retired UIUtils.GetDefaultMapFPS rule */
        [Theory]
        [InlineData(60, true, false, 60)]
        [InlineData(60, false, false, 60)]
        [InlineData(72, true, false, 72)]
        [InlineData(72, false, false, 60)]
        [InlineData(144, true, false, 72)]
        [InlineData(144, false, false, 60)]
        [InlineData(160, true, false, 80)]
        [InlineData(240, true, false, 80)]
        [InlineData(240, true, true, 80)]
        [InlineData(90, true, true, 90)]
        [InlineData(90, true, false, 60)]
        [InlineData(180, true, true, 90)]
        [InlineData(95, true, true, 60)]
        [InlineData(120, true, false, 60)]
        [InlineData(50, true, false, 40)]
        [InlineData(30, true, false, 30)]
        public void GetLegacyDefaultFps_ReproducesTheRetiredRule(int rate, bool platformLoop, bool isAvalonia, int expected)
        {
            Assert.Equal(expected, GHMapFps.GetLegacyDefaultFps(rate, platformLoop, isAvalonia));
        }

        [Theory]
        /* Known refresh rate: only the retired default level at that rate becomes -1 */
        [InlineData(-1, 60, true, false, -1)]
        [InlineData(3, 60, true, false, -1)]
        [InlineData(2, 60, true, false, 40)]
        [InlineData(1, 60, true, false, 30)]
        [InlineData(4, 144, true, false, -1)]
        [InlineData(3, 144, true, false, 60)]
        [InlineData(4, 144, false, false, 72)]
        [InlineData(3, 144, false, false, -1)]
        [InlineData(6, 90, true, true, -1)]
        [InlineData(6, 95, true, true, 90)]
        [InlineData(3, 95, true, true, -1)]
        [InlineData(2, 50, true, false, -1)]
        [InlineData(7, 120, true, false, 120)]
        [InlineData(8, 144, true, false, 144)]
        /* Unknown refresh rate: every retired default level at 60 Hz or more becomes -1 */
        [InlineData(3, 0, false, false, -1)]
        [InlineData(4, 0, true, false, -1)]
        [InlineData(4, 0, false, false, 72)]
        [InlineData(5, 0, true, false, -1)]
        [InlineData(6, 0, true, true, -1)]
        [InlineData(6, 0, true, false, 90)]
        [InlineData(2, 0, true, true, 40)]
        /* Out of range */
        [InlineData(9, 60, true, true, -1)]
        [InlineData(-5, 60, true, true, -1)]
        public void MigrateLegacyIndex_MapsTheOldDefaultLevelToDefault(int legacyIndex, int rate, bool platformLoop, bool isAvalonia, int expected)
        {
            Assert.Equal(expected, GHMapFps.MigrateLegacyIndex(legacyIndex, rate, platformLoop, isAvalonia));
        }

        [Theory]
        [InlineData(60, 16U)]
        [InlineData(90, 11U)]
        [InlineData(45, 22U)]
        [InlineData(120, 8U)]
        [InlineData(20, 50U)]
        [InlineData(0, 100U)]
        public void GetIntervalMs_IsOneFrameRoundedDown(int fps, uint expected)
        {
            Assert.Equal(expected, GHMapFps.GetIntervalMs(fps));
        }
    }
}

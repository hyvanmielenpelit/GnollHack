using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHRadialGeometry: the radii and font sizes of Design C's table for the Steam Deck and
       1080p, the device-independent cap, slot centers and boundaries at N = 6, 8 and 12, the
       compass angle convention with +Y down (R1), hysteresis, the hub/ring/outside hit test, the
       parallel-edge inset, page dots and the L1/R1 chips. */
    public class GHRadialGeometryTests
    {
        private static GHRadialGeometry Make(float width, float height, float dipScale)
        {
            GHRadialGeometry g = new GHRadialGeometry();
            g.Update(width, height, dipScale);
            return g;
        }

        [Fact]
        public void SteamDeck_MatchesDesignTable()
        {
            GHRadialGeometry g = Make(1280, 800, 1);
            Assert.Equal(320f, g.OuterRadius, 2);
            Assert.InRange(g.HubRadius, 114.5f, 115.5f);
            Assert.InRange(g.InnerRadius, 130.5f, 131.5f);
            Assert.InRange(g.SliceGap, 3.5f, 4.5f);
            Assert.InRange(g.CornerRadius, 5.5f, 6.5f);
            Assert.InRange(g.GetIconSize(8), 78.5f, 79.5f);
            Assert.InRange(g.GetIconSize(6), 78.5f, 79.5f);
            Assert.InRange(g.GetIconSize(12), 64.5f, 65.5f);
            Assert.InRange(g.LabelFontSize, 14.5f, 15.5f);
            Assert.InRange(g.HubTitleFontSize, 26.5f, 27.5f);
            Assert.InRange(g.HubDescriptionFontSize, 16.5f, 17.5f);
            Assert.InRange(g.PageTitleFontSize, 21.5f, 22.5f);
            Assert.InRange(g.HintFontSize, 15.5f, 16.5f);
            Assert.Equal(640f, g.CenterX, 2);
            Assert.Equal(400f, g.CenterY, 2);
            /* The hint bar leaves about 40 px below it */
            Assert.InRange(800f - g.HintCenterY, 30f, 40f);
        }

        [Fact]
        public void FullHd_MatchesDesignTable()
        {
            GHRadialGeometry g = Make(1920, 1080, 1);
            Assert.Equal(430f, g.OuterRadius, 2);
            Assert.InRange(g.HubRadius, 154.5f, 155.5f);
            Assert.InRange(g.InnerRadius, 175.5f, 176.5f);
            Assert.InRange(g.SliceGap, 4.5f, 5.5f);
            Assert.InRange(g.CornerRadius, 8.5f, 9.5f);
            Assert.InRange(g.GetIconSize(8), 105.5f, 107f);
            Assert.InRange(g.GetIconSize(12), 86.5f, 88.5f);
            Assert.InRange(g.LabelFontSize, 19.5f, 20.5f);
            Assert.InRange(g.HubTitleFontSize, 36f, 37.5f);
            Assert.InRange(g.HubDescriptionFontSize, 21.5f, 22.5f);
            Assert.InRange(g.PageTitleFontSize, 29.5f, 30.5f);
            Assert.InRange(g.HintFontSize, 21f, 22f);
        }

        [Fact]
        public void OuterRadius_IsCappedInDeviceIndependentPixels()
        {
            Assert.Equal(430f, Make(3840, 2160, 1).OuterRadius, 2);
            Assert.Equal(860f, Make(3840, 2160, 2).OuterRadius, 2);
        }

        [Fact]
        public void LabelFont_HasDeviceIndependentFloor()
        {
            GHRadialGeometry g = Make(400, 300, 1);
            Assert.Equal(14f, g.LabelFontSize, 3);
            Assert.Equal(28f, Make(400, 300, 2).LabelFontSize, 3);
        }

        [Fact]
        public void Update_ReportsChangesAndAdvancesVersion()
        {
            GHRadialGeometry g = new GHRadialGeometry();
            Assert.False(g.IsValid);
            Assert.True(g.Update(1280, 800, 1));
            int v = g.Version;
            Assert.False(g.Update(1280, 800, 1));
            Assert.Equal(v, g.Version);
            Assert.True(g.Update(800, 1280, 1));
            Assert.NotEqual(v, g.Version);
            Assert.True(g.IsValid);
        }

        [Theory]
        [InlineData(0.0, -1.0, 0.0)]     /* up is north */
        [InlineData(1.0, 0.0, 90.0)]     /* right is east */
        [InlineData(0.0, 1.0, 180.0)]    /* down is south */
        [InlineData(-1.0, 0.0, 270.0)]   /* left is west */
        [InlineData(1.0, -1.0, 45.0)]
        [InlineData(-1.0, 1.0, 225.0)]
        public void CompassAngle_IsClockwiseFromNorthWithYDown(double x, double y, double expected)
        {
            Assert.Equal(expected, GHRadialGeometry.CompassAngleDegrees(x, y), 6);
        }

        [Theory]
        [InlineData(-90.0, 270.0)]
        [InlineData(720.0, 0.0)]
        [InlineData(359.5, 359.5)]
        [InlineData(-360.0, 0.0)]
        public void NormalizeDegrees_WrapsIntoRange(double input, double expected)
        {
            Assert.Equal(expected, GHRadialGeometry.NormalizeDegrees(input), 9);
        }

        [Fact]
        public void AngleDelta_IsShortestSignedTurn()
        {
            Assert.Equal(20.0, GHRadialGeometry.AngleDelta(10, 350), 9);
            Assert.Equal(-20.0, GHRadialGeometry.AngleDelta(350, 10), 9);
            Assert.Equal(180.0, GHRadialGeometry.AngleDelta(180, 0), 9);
        }

        [Theory]
        [InlineData(6, 29.99, 0)]
        [InlineData(6, 30.0, 1)]
        [InlineData(6, 330.0, 0)]
        [InlineData(6, 329.99, 5)]
        [InlineData(8, 22.49, 0)]
        [InlineData(8, 22.5, 1)]
        [InlineData(8, 337.5, 0)]
        [InlineData(8, 337.49, 7)]
        [InlineData(8, 90.0, 2)]
        [InlineData(12, 14.99, 0)]
        [InlineData(12, 15.0, 1)]
        [InlineData(12, 345.0, 0)]
        [InlineData(12, 344.99, 11)]
        public void SlotAt_SplitsTheCircleAtSlotBoundaries(int n, double angle, int expected)
        {
            Assert.Equal(expected, GHRadialGeometry.SlotAt(angle, n));
        }

        [Fact]
        public void SlotCenters_StartNorthAndRunClockwise()
        {
            Assert.Equal(0.0, GHRadialGeometry.SlotCenterDegrees(0, 8), 9);
            Assert.Equal(90.0, GHRadialGeometry.SlotCenterDegrees(2, 8), 9);
            Assert.Equal(300.0, GHRadialGeometry.SlotCenterDegrees(5, 6), 9);
            Assert.Equal(330.0, GHRadialGeometry.SlotCenterDegrees(11, 12), 9);
        }

        [Theory]
        [InlineData(6, 6.0)]
        [InlineData(8, 5.625)]
        [InlineData(12, 3.75)]
        public void Hysteresis_IsAQuarterOfHalfASlotClampedTo3To6(int n, double expected)
        {
            Assert.Equal(expected, GHRadialGeometry.HysteresisDegrees(n), 9);
        }

        [Fact]
        public void NearestSlot_BreaksTiesTowardTheBias()
        {
            Assert.Equal(10, GHRadialGeometry.NearestSlot(315, 12, 270));
            Assert.Equal(11, GHRadialGeometry.NearestSlot(315, 12, 0));
            Assert.Equal(1, GHRadialGeometry.NearestSlot(60, 8, 90));
            Assert.Equal(7, GHRadialGeometry.NearestSlot(300, 8, 270));
        }

        [Fact]
        public void HitTest_ClassifiesHubRingAndOutside()
        {
            GHRadialGeometry g = Make(1280, 800, 1);
            float r;
            double a;
            Assert.Equal(GHRadialHitZone.Hub, g.HitTest(g.CenterX, g.CenterY, out r, out a));
            Assert.Equal(GHRadialHitZone.Hub, g.HitTest(g.CenterX, g.CenterY - g.InnerRadius + 1, out r, out a));

            Assert.Equal(GHRadialHitZone.Ring, g.HitTest(g.CenterX, g.CenterY - g.MidRadius, out r, out a));
            Assert.Equal(0.0, a, 6);
            Assert.Equal(g.MidRadius, r, 3);

            Assert.Equal(GHRadialHitZone.Ring, g.HitTest(g.CenterX + g.OuterRadius * 1.05f, g.CenterY, out r, out a));
            Assert.Equal(90.0, a, 6);
            Assert.Equal(GHRadialHitZone.Outside, g.HitTest(g.CenterX + g.OuterRadius * 1.2f, g.CenterY, out r, out a));
            Assert.Equal(GHRadialHitZone.Ring, g.HitTest(g.CenterX, g.CenterY + g.MidRadius, out r, out a));
            Assert.Equal(180.0, a, 6);
        }

        [Fact]
        public void GetPoint_UsesCompassAngles()
        {
            GHRadialGeometry g = Make(1280, 800, 1);
            float x, y;
            g.GetPoint(90, 100, out x, out y);
            Assert.Equal(740f, x, 3);
            Assert.Equal(400f, y, 3);
            g.GetPoint(0, 100, out x, out y);
            Assert.Equal(640f, x, 3);
            Assert.Equal(300f, y, 3);
        }

        [Fact]
        public void InsetDegrees_KeepsGapEdgesParallel()
        {
            GHRadialGeometry g = Make(1280, 800, 1);
            double outer = GHRadialGeometry.InsetDegrees(g.SliceGap, g.OuterRadius);
            double inner = GHRadialGeometry.InsetDegrees(g.SliceGap, g.InnerRadius);
            Assert.True(inner > outer);
            /* Half the gap, measured perpendicular to the slot boundary, at both radii */
            Assert.Equal(g.SliceGap / 2, g.OuterRadius * Math.Sin(outer * Math.PI / 180.0), 4);
            Assert.Equal(g.SliceGap / 2, g.InnerRadius * Math.Sin(inner * Math.PI / 180.0), 4);
            Assert.Equal(0.0, GHRadialGeometry.InsetDegrees(0, 100), 9);
        }

        [Fact]
        public void Dots_AreCenteredAndHitTestable()
        {
            GHRadialGeometry g = Make(1280, 800, 1);
            float x0, y0, x2, y2;
            g.GetDotCenter(0, 3, out x0, out y0);
            g.GetDotCenter(2, 3, out x2, out y2);
            Assert.Equal(g.CenterX, (x0 + x2) / 2, 3);
            Assert.True(y0 < g.CenterY - g.OuterRadius);
            Assert.Equal(0, g.HitTestDot(x0, y0, 3));
            Assert.Equal(2, g.HitTestDot(x2, y2, 3));
            Assert.Equal(-1, g.HitTestDot(g.CenterX, g.CenterY, 3));
            Assert.Equal(-1, g.HitTestDot(x0, y0, 1));
        }

        [Fact]
        public void Chips_SitBesideTheRing()
        {
            GHRadialGeometry g = Make(1280, 800, 1);
            Assert.Equal(-1, g.HitTestChip(g.CenterX - g.ChipOffsetX, g.CenterY));
            Assert.Equal(1, g.HitTestChip(g.CenterX + g.ChipOffsetX, g.CenterY));
            Assert.Equal(0, g.HitTestChip(g.CenterX, g.CenterY));
            Assert.True(g.CenterX - g.ChipOffsetX > 0);
        }
    }
}

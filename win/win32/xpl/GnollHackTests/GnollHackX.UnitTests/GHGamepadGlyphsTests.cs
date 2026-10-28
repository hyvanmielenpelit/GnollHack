using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadGlyphs: a label for every button in every style, distinct within a style, and the
       Steam Deck, Xbox and PlayStation names */
    public class GHGamepadGlyphsTests
    {
        [Fact]
        public void EveryButtonInEveryStyle_HasADistinctLabel()
        {
            foreach (GHGamepadStyle style in Enum.GetValues(typeof(GHGamepadStyle)))
            {
                HashSet<string> seen = new HashSet<string>();
                int b;
                for (b = 0; b < (int)GHGamepadButton.Count; b++)
                {
                    string label = GHGamepadGlyphs.GetLabel((GHGamepadButton)b, style);
                    Assert.False(string.IsNullOrWhiteSpace(label), style + " " + (GHGamepadButton)b);
                    Assert.True(seen.Add(label), style + " repeats " + label);
                }
            }
        }

        [Theory]
        [InlineData(GHGamepadButton.A, "A")]
        [InlineData(GHGamepadButton.B, "B")]
        [InlineData(GHGamepadButton.X, "X")]
        [InlineData(GHGamepadButton.Y, "Y")]
        [InlineData(GHGamepadButton.LB, "L1")]
        [InlineData(GHGamepadButton.RB, "R1")]
        [InlineData(GHGamepadButton.LT, "L2")]
        [InlineData(GHGamepadButton.RT, "R2")]
        [InlineData(GHGamepadButton.L3, "L3")]
        [InlineData(GHGamepadButton.R3, "R3")]
        [InlineData(GHGamepadButton.L4, "L4")]
        [InlineData(GHGamepadButton.L5, "L5")]
        [InlineData(GHGamepadButton.R4, "R4")]
        [InlineData(GHGamepadButton.R5, "R5")]
        [InlineData(GHGamepadButton.View, "View")]
        [InlineData(GHGamepadButton.Menu, "Menu")]
        public void SteamDeck_Labels(GHGamepadButton button, string expected)
        {
            Assert.Equal(expected, GHGamepadGlyphs.GetLabel(button, GHGamepadStyle.SteamDeck));
        }

        [Fact]
        public void Xbox_AndGenericUseXboxLabels()
        {
            Assert.Equal("LB", GHGamepadGlyphs.GetLabel(GHGamepadButton.LB, GHGamepadStyle.Xbox));
            Assert.Equal("RT", GHGamepadGlyphs.GetLabel(GHGamepadButton.RT, GHGamepadStyle.Xbox));
            Assert.Equal("LT", GHGamepadGlyphs.GetLabel(GHGamepadButton.LT, GHGamepadStyle.Generic));
        }

        [Fact]
        public void PlayStation_AndNintendoLabels()
        {
            Assert.Equal("Cross", GHGamepadGlyphs.GetLabel(GHGamepadButton.A, GHGamepadStyle.PlayStation));
            Assert.Equal("Triangle", GHGamepadGlyphs.GetLabel(GHGamepadButton.Y, GHGamepadStyle.PlayStation));
            Assert.Equal("Options", GHGamepadGlyphs.GetLabel(GHGamepadButton.Menu, GHGamepadStyle.PlayStation));
            /* Positional south is labeled B on Nintendo pads */
            Assert.Equal("B", GHGamepadGlyphs.GetLabel(GHGamepadButton.A, GHGamepadStyle.Nintendo));
            Assert.Equal("A", GHGamepadGlyphs.GetLabel(GHGamepadButton.B, GHGamepadStyle.Nintendo));
        }

        [Fact]
        public void Sticks_UseLAndRStick()
        {
            Assert.Equal("L-stick", GHGamepadGlyphs.GetStickLabel(0, GHGamepadStyle.SteamDeck));
            Assert.Equal("R-stick", GHGamepadGlyphs.GetStickLabel(1, GHGamepadStyle.Xbox));
            Assert.Equal("L-stick Up", GHGamepadGlyphs.GetLabel(GHGamepadButton.LeftStickUp, GHGamepadStyle.SteamDeck));
        }
    }
}

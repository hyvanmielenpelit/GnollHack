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

        [Theory]
        [InlineData(GHGamepadButton.A, GHGamepadBadgeKind.FaceButton, "A")]
        [InlineData(GHGamepadButton.Y, GHGamepadBadgeKind.FaceButton, "Y")]
        [InlineData(GHGamepadButton.LB, GHGamepadBadgeKind.Bumper, "L1")]
        [InlineData(GHGamepadButton.RB, GHGamepadBadgeKind.Bumper, "R1")]
        [InlineData(GHGamepadButton.LT, GHGamepadBadgeKind.Trigger, "L2")]
        [InlineData(GHGamepadButton.RT, GHGamepadBadgeKind.Trigger, "R2")]
        [InlineData(GHGamepadButton.L3, GHGamepadBadgeKind.StickClick, "L3")]
        [InlineData(GHGamepadButton.R3, GHGamepadBadgeKind.StickClick, "R3")]
        [InlineData(GHGamepadButton.L4, GHGamepadBadgeKind.Grip, "L4")]
        [InlineData(GHGamepadButton.R5, GHGamepadBadgeKind.Grip, "R5")]
        [InlineData(GHGamepadButton.Menu, GHGamepadBadgeKind.Menu, "")]
        [InlineData(GHGamepadButton.View, GHGamepadBadgeKind.View, "")]
        [InlineData(GHGamepadButton.DPadLeft, GHGamepadBadgeKind.DPad, "")]
        [InlineData(GHGamepadButton.LeftStickUp, GHGamepadBadgeKind.Stick, "L")]
        [InlineData(GHGamepadButton.RightStickRight, GHGamepadBadgeKind.Stick, "R")]
        public void SteamDeck_Badges(GHGamepadButton button, GHGamepadBadgeKind kind, string text)
        {
            Assert.Equal(kind, GHGamepadGlyphs.GetBadgeKind(button, GHGamepadStyle.SteamDeck));
            Assert.Equal(text, GHGamepadGlyphs.GetBadgeText(button, GHGamepadStyle.SteamDeck));
        }

        [Fact]
        public void OtherStyles_Badges()
        {
            Assert.Equal(GHGamepadBadgeKind.PlayStationSymbol, GHGamepadGlyphs.GetBadgeKind(GHGamepadButton.A, GHGamepadStyle.PlayStation));
            Assert.Equal("", GHGamepadGlyphs.GetBadgeText(GHGamepadButton.A, GHGamepadStyle.PlayStation));
            Assert.Equal("B", GHGamepadGlyphs.GetBadgeText(GHGamepadButton.A, GHGamepadStyle.Nintendo));
            Assert.Equal("LT", GHGamepadGlyphs.GetBadgeText(GHGamepadButton.LT, GHGamepadStyle.Xbox));
            Assert.Equal("LB", GHGamepadGlyphs.GetBadgeText(GHGamepadButton.LB, GHGamepadStyle.Xbox));
            Assert.Equal("R", GHGamepadGlyphs.GetBadgeText(GHGamepadButton.RB, GHGamepadStyle.Nintendo));
            Assert.Equal("RS", GHGamepadGlyphs.GetBadgeText(GHGamepadButton.R3, GHGamepadStyle.Generic));
        }

        [Fact]
        public void EveryButtonInEveryStyle_HasABadge()
        {
            foreach (GHGamepadStyle style in Enum.GetValues(typeof(GHGamepadStyle)))
            {
                int b;
                for (b = 0; b < (int)GHGamepadButton.Count; b++)
                {
                    GHGamepadBadgeKind kind = GHGamepadGlyphs.GetBadgeKind((GHGamepadButton)b, style);
                    string text = GHGamepadGlyphs.GetBadgeText((GHGamepadButton)b, style);
                    Assert.NotNull(text);
                    bool symbolOnly = kind == GHGamepadBadgeKind.Menu || kind == GHGamepadBadgeKind.View
                        || kind == GHGamepadBadgeKind.DPad || kind == GHGamepadBadgeKind.PlayStationSymbol;
                    Assert.Equal(symbolOnly, text.Length == 0);
                }
            }
        }

        [Fact]
        public void HintSymbols_Badges()
        {
            Assert.Equal(GHGamepadBadgeKind.DPad, GHGamepadGlyphs.GetSymbolBadgeKind(GHGamepadHintSymbol.DPad));
            Assert.Equal(GHGamepadBadgeKind.Stick, GHGamepadGlyphs.GetSymbolBadgeKind(GHGamepadHintSymbol.LeftStick));
            Assert.Equal("L", GHGamepadGlyphs.GetSymbolBadgeText(GHGamepadHintSymbol.LeftStick));
            Assert.Equal("R", GHGamepadGlyphs.GetSymbolBadgeText(GHGamepadHintSymbol.RightStick));
            Assert.Equal("", GHGamepadGlyphs.GetSymbolBadgeText(GHGamepadHintSymbol.DPad));
        }

        [Fact]
        public void HintPair_AndSymbol()
        {
            GHGamepadHint pair = GHGamepadHint.Pair(GHGamepadButton.LB, GHGamepadButton.RB, "Speed");
            Assert.True(pair.HasButton2);
            Assert.Equal(GHGamepadButton.RB, pair.Button2);
            Assert.Equal(GHGamepadHintSymbol.None, pair.Symbol);
            GHGamepadHint dpad = GHGamepadHint.ForSymbol(GHGamepadHintSymbol.DPad, "Zoom");
            Assert.Equal(GHGamepadHintSymbol.DPad, dpad.Symbol);
            Assert.False(dpad.HasButton2);
            Assert.False(pair.LineBreakBefore);
            Assert.False(new GHGamepadHint(GHGamepadButton.A, "Act", true).LineBreakBefore);
        }
    }
}

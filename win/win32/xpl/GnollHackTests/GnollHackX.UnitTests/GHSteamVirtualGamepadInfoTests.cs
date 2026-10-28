using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHSteamVirtualGamepadInfo: the info file Steam names in SteamVirtualGamepadInfo, the slot in the
       virtual pad's device name, and the glyph style from the real controller. The first fixture is the file
       read on a Steam Deck. */
    public class GHSteamVirtualGamepadInfoTests
    {
        private const string DeckInfoFile =
            "[slot 0]\n"
            + "name=Steam Deck Controller\n"
            + "VID=0x28de\n"
            + "PID=0x1205\n"
            + "handle=0x000000ff62101c65\n"
            + "type=steam\n";

        private const string TwoSlotInfoFile =
            "[slot 0]\r\n"
            + "name=Steam Deck Controller\r\n"
            + "VID=0x28de\r\n"
            + "PID=0x1205\r\n"
            + "handle=0x000000ff62101c65\r\n"
            + "type=steam\r\n"
            + "\r\n"
            + "[slot 1]\r\n"
            + "name=DualSense Wireless Controller\r\n"
            + "VID=0x054c\r\n"
            + "PID=0x0ce6\r\n"
            + "handle=0x000000ff62101c99\r\n"
            + "type=ps5\r\n";

        [Fact]
        public void Parse_TheDeckFile()
        {
            GHSteamVirtualGamepadInfo info = GHSteamVirtualGamepadInfo.Parse(DeckInfoFile);

            Assert.Single(info.Slots);
            GHSteamVirtualGamepadSlot slot = info.GetSlot(0);
            Assert.NotNull(slot);
            Assert.Equal(0, slot.Slot);
            Assert.Equal("Steam Deck Controller", slot.Name);
            Assert.Equal(0x28de, slot.VendorId);
            Assert.Equal(0x1205, slot.ProductId);
            Assert.Equal(0x000000ff62101c65UL, slot.Handle);
            Assert.Equal("steam", slot.Type);
            Assert.Null(info.GetSlot(1));
        }

        [Fact]
        public void Parse_TwoSlotsWithCrLf()
        {
            GHSteamVirtualGamepadInfo info = GHSteamVirtualGamepadInfo.Parse(TwoSlotInfoFile);

            Assert.Equal(2, info.Slots.Count);
            GHSteamVirtualGamepadSlot slot1 = info.GetSlot(1);
            Assert.NotNull(slot1);
            Assert.Equal("DualSense Wireless Controller", slot1.Name);
            Assert.Equal(0x054c, slot1.VendorId);
            Assert.Equal(0x0ce6, slot1.ProductId);
            Assert.Equal("ps5", slot1.Type);
            Assert.Equal(GHGamepadStyle.PlayStation, GHSteamVirtualGamepadInfo.ResolveStyle(slot1, 1, true));
            Assert.Equal(GHGamepadStyle.SteamDeck, GHSteamVirtualGamepadInfo.ResolveStyle(info.GetSlot(0), 0, true));
        }

        [Fact]
        public void Parse_IgnoresLinesOutsideSlotsAndMalformedSections()
        {
            GHSteamVirtualGamepadInfo info = GHSteamVirtualGamepadInfo.Parse(
                "name=orphan\n[slot x]\nVID=0x1234\n[other]\nPID=0x5678\n[slot 2]\nVID=1118\nPID=0x0b13\nnoequals\n");

            Assert.Single(info.Slots);
            GHSteamVirtualGamepadSlot slot = info.GetSlot(2);
            Assert.Equal(0x045e, slot.VendorId);
            Assert.Equal(0x0b13, slot.ProductId);
            Assert.Null(slot.Name);
        }

        [Fact]
        public void Parse_EmptyAndNull()
        {
            Assert.Empty(GHSteamVirtualGamepadInfo.Parse("").Slots);
            Assert.Empty(GHSteamVirtualGamepadInfo.Parse(null).Slots);
        }

        [Theory]
        [InlineData("Microsoft X-Box 360 pad 0", 0)]
        [InlineData("Microsoft X-Box 360 pad 1", 1)]
        [InlineData("Microsoft X-Box 360 pad 12", 12)]
        [InlineData("Microsoft X-Box 360 pad", -1)]
        [InlineData("Microsoft X-Box 360 pad x", -1)]
        [InlineData("Steam Deck", -1)]
        [InlineData("", -1)]
        [InlineData(null, -1)]
        public void SlotFromDeviceName(string name, int expected)
        {
            Assert.Equal(expected, GHSteamVirtualGamepadInfo.GetSlotFromDeviceName(name));
        }

        [Theory]
        [InlineData(0x28de, 0x1205, GHGamepadStyle.SteamDeck)]
        [InlineData(0x054c, 0x0ce6, GHGamepadStyle.PlayStation)]
        [InlineData(0x054c, 0x09cc, GHGamepadStyle.PlayStation)]
        [InlineData(0x057e, 0x2009, GHGamepadStyle.Nintendo)]
        [InlineData(0x045e, 0x0b13, GHGamepadStyle.Xbox)]
        [InlineData(0x28de, 0x1142, GHGamepadStyle.Xbox)]
        [InlineData(0x0000, 0x0000, GHGamepadStyle.Xbox)]
        public void StyleFromTheRealVidPid(int vendor, int product, GHGamepadStyle expected)
        {
            Assert.Equal(expected, GHSteamVirtualGamepadInfo.GetStyle((ushort)vendor, (ushort)product));
        }

        [Fact]
        public void Style_WithoutAnInfoFile()
        {
            Assert.Equal(GHGamepadStyle.SteamDeck, GHSteamVirtualGamepadInfo.ResolveStyle(null, 0, true));
            Assert.Equal(GHGamepadStyle.Xbox, GHSteamVirtualGamepadInfo.ResolveStyle(null, 1, true));
            Assert.Equal(GHGamepadStyle.Xbox, GHSteamVirtualGamepadInfo.ResolveStyle(null, 0, false));
            Assert.Equal(GHGamepadStyle.Xbox, GHSteamVirtualGamepadInfo.ResolveStyle(null, -1, true));
        }

        [Fact]
        public void Describe_ForTheLog()
        {
            GHSteamVirtualGamepadInfo info = GHSteamVirtualGamepadInfo.Parse(DeckInfoFile);

            Assert.Equal("real 28de:1205 \"Steam Deck Controller\" slot 0", GHSteamVirtualGamepadInfo.Describe(info.GetSlot(0), 0));
            Assert.Equal("real unknown slot 3", GHSteamVirtualGamepadInfo.Describe(null, 3));
        }

        [Theory]
        [InlineData("0x28de", true, 0x28deUL)]
        [InlineData("0X1205", true, 0x1205UL)]
        [InlineData("10462", true, 10462UL)]
        [InlineData("017", true, 15UL)]
        [InlineData("0", true, 0UL)]
        [InlineData("0x", false, 0UL)]
        [InlineData("09", false, 0UL)]
        [InlineData("junk", false, 0UL)]
        [InlineData("", false, 0UL)]
        public void CInteger_ParsesLikeStrtoulBaseZero(string text, bool ok, ulong expected)
        {
            ulong value;
            Assert.Equal(ok, GHSteamVirtualGamepadInfo.TryParseCInteger(text, out value));
            if (ok)
                Assert.Equal(expected, value);
        }
    }
}

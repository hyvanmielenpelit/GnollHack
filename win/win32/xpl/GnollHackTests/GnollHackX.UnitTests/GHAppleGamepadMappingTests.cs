using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHAppleGamepadMapping: the style rules and the inner deadzone */
    public class GHAppleGamepadMappingTests
    {
        [Theory]
        [InlineData(GHAppleGamepadProfile.Xbox, "MFi", "Controller", GHGamepadStyle.Xbox)]
        [InlineData(GHAppleGamepadProfile.DualShock, null, null, GHGamepadStyle.PlayStation)]
        [InlineData(GHAppleGamepadProfile.DualSense, "Xbox One", "Xbox Wireless Controller", GHGamepadStyle.PlayStation)]
        public void GetStyle_ProfileDecidesFirst(GHAppleGamepadProfile profile, string category, string vendorName, GHGamepadStyle expected)
        {
            Assert.Equal(expected, GHAppleGamepadMapping.GetStyle(profile, category, vendorName));
        }

        [Theory]
        [InlineData("Xbox One", "Controller", GHGamepadStyle.Xbox)]
        [InlineData("DualShock 4", "Controller", GHGamepadStyle.PlayStation)]
        [InlineData("DualSense", "Controller", GHGamepadStyle.PlayStation)]
        [InlineData("Switch Pro Controller", "Pro Controller", GHGamepadStyle.Nintendo)]
        [InlineData("Nintendo Switch Joy-Con (L/R)", "Joy-Con (L/R)", GHGamepadStyle.Nintendo)]
        [InlineData("xbox one", null, GHGamepadStyle.Xbox)]
        public void GetStyle_ByProductCategory(string category, string vendorName, GHGamepadStyle expected)
        {
            Assert.Equal(expected, GHAppleGamepadMapping.GetStyle(GHAppleGamepadProfile.Extended, category, vendorName));
        }

        [Theory]
        [InlineData("MFi", "Backbone One PlayStation Edition", GHGamepadStyle.PlayStation)]
        [InlineData("MFi", "Xbox Wireless Controller", GHGamepadStyle.Xbox)]
        [InlineData("MFi", "Joy-Con (L)", GHGamepadStyle.Nintendo)]
        [InlineData(null, "DUALSHOCK 4 Wireless Controller", GHGamepadStyle.PlayStation)]
        public void GetStyle_ByVendorNameWhenTheCategoryIsGeneric(string category, string vendorName, GHGamepadStyle expected)
        {
            Assert.Equal(expected, GHAppleGamepadMapping.GetStyle(GHAppleGamepadProfile.Extended, category, vendorName));
        }

        [Theory]
        [InlineData("MFi", "Backbone One")]
        [InlineData("MFi", null)]
        [InlineData(null, null)]
        [InlineData("", "")]
        [InlineData("HID", "Controller")]
        public void GetStyle_IsGenericOtherwise(string category, string vendorName)
        {
            Assert.Equal(GHGamepadStyle.Generic, GHAppleGamepadMapping.GetStyle(GHAppleGamepadProfile.Extended, category, vendorName));
        }

        [Fact]
        public void GetStyle_PrefersTheCategoryOverTheVendorName()
        {
            Assert.Equal(GHGamepadStyle.Xbox,
                GHAppleGamepadMapping.GetStyle(GHAppleGamepadProfile.Extended, "Xbox One", "PlayStation look-alike"));
        }

        [Fact]
        public void InnerDeadzone_MatchesTheOtherBackendsFloor()
        {
            Assert.Equal(0.24, GHAppleGamepadMapping.InnerDeadzone, 4);
        }
    }
}

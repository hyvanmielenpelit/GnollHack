using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHAndroidGamepadMapping: the key map, the style rules, the inner deadzone, the axis layout choice
       and the trigger rescale */
    public class GHAndroidGamepadMappingTests
    {
        [Theory]
        [InlineData(96, GHGamepadButton.A)]
        [InlineData(97, GHGamepadButton.B)]
        [InlineData(99, GHGamepadButton.X)]
        [InlineData(100, GHGamepadButton.Y)]
        [InlineData(102, GHGamepadButton.LB)]
        [InlineData(103, GHGamepadButton.RB)]
        [InlineData(104, GHGamepadButton.LT)]
        [InlineData(105, GHGamepadButton.RT)]
        [InlineData(106, GHGamepadButton.L3)]
        [InlineData(107, GHGamepadButton.R3)]
        [InlineData(108, GHGamepadButton.Menu)]
        [InlineData(109, GHGamepadButton.View)]
        [InlineData(19, GHGamepadButton.DPadUp)]
        [InlineData(20, GHGamepadButton.DPadDown)]
        [InlineData(21, GHGamepadButton.DPadLeft)]
        [InlineData(22, GHGamepadButton.DPadRight)]
        [InlineData(4, GHGamepadButton.View)]
        [InlineData(82, GHGamepadButton.Menu)]
        public void TryMapKey_MapsGamepadKeys(int keyCode, GHGamepadButton expected)
        {
            GHGamepadButton button;
            Assert.True(GHAndroidGamepadMapping.TryMapKey(keyCode, out button));
            Assert.Equal(expected, button);
        }

        [Theory]
        [InlineData(110)]   /* BUTTON_MODE: the Guide button stays Android's */
        [InlineData(98)]    /* BUTTON_C */
        [InlineData(66)]    /* ENTER */
        [InlineData(0)]
        public void TryMapKey_LeavesOtherKeysUnmapped(int keyCode)
        {
            GHGamepadButton button;
            Assert.False(GHAndroidGamepadMapping.TryMapKey(keyCode, out button));
        }

        [Theory]
        [InlineData(0x045E, 0x0B13, "", GHGamepadStyle.Xbox)]
        [InlineData(0x054C, 0x0CE6, "Wireless Controller", GHGamepadStyle.PlayStation)]
        [InlineData(0x057E, 0x2009, "", GHGamepadStyle.Nintendo)]
        [InlineData(0x358A, 0x0104, "Backbone One", GHGamepadStyle.PlayStation)]
        [InlineData(0x358A, 0x0304, "Backbone One", GHGamepadStyle.PlayStation)]
        [InlineData(0x358A, 0x0105, "Backbone One", GHGamepadStyle.Xbox)]
        [InlineData(0x1234, 0x0001, "Generic XBOX pad", GHGamepadStyle.Xbox)]
        [InlineData(0x1234, 0x0001, "DualSense Wireless", GHGamepadStyle.PlayStation)]
        [InlineData(0x1234, 0x0001, "dualshock 4", GHGamepadStyle.PlayStation)]
        [InlineData(0x1234, 0x0001, "PlayStation pad", GHGamepadStyle.PlayStation)]
        [InlineData(0x1234, 0x0001, "Pro Controller", GHGamepadStyle.Nintendo)]
        [InlineData(0x1234, 0x0001, "Joy-Con (L/R)", GHGamepadStyle.Nintendo)]
        [InlineData(0x1234, 0x0001, "NINTENDO compatible", GHGamepadStyle.Nintendo)]
        [InlineData(0x1234, 0x0001, "Gamepad", GHGamepadStyle.Generic)]
        [InlineData(0x1234, 0x0001, "", GHGamepadStyle.Generic)]
        [InlineData(0x1234, 0x0001, null, GHGamepadStyle.Generic)]
        public void GetStyle_ByVendorThenName(int vendorId, int productId, string name, GHGamepadStyle expected)
        {
            Assert.Equal(expected, GHAndroidGamepadMapping.GetStyle(vendorId, productId, name));
        }

        [Fact]
        public void GetInnerDeadzone_HasAFloor()
        {
            Assert.Equal(0.24, GHAndroidGamepadMapping.GetInnerDeadzone(0f, 0f), 4);
            Assert.Equal(0.24, GHAndroidGamepadMapping.GetInnerDeadzone(0.1f, 0.05f), 4);
        }

        [Fact]
        public void GetInnerDeadzone_ScalesTheLargerFlat()
        {
            Assert.Equal(0.30, GHAndroidGamepadMapping.GetInnerDeadzone(0.1f, 0.25f), 4);
            Assert.Equal(0.30, GHAndroidGamepadMapping.GetInnerDeadzone(0.25f, 0.1f), 4);
        }

        [Fact]
        public void GetInnerDeadzone_IsClamped()
        {
            Assert.Equal(0.35, GHAndroidGamepadMapping.GetInnerDeadzone(0.5f, 0f), 4);
            Assert.Equal(0.35, GHAndroidGamepadMapping.GetInnerDeadzone(0f, 1f), 4);
        }

        [Theory]
        [InlineData(true, true, true, true, false, GHAndroidAxisLayout.LinuxGamepad)]
        [InlineData(true, true, true, true, true, GHAndroidAxisLayout.Standard)]
        [InlineData(false, false, true, true, true, GHAndroidAxisLayout.RxRyStick)]
        [InlineData(true, false, true, true, true, GHAndroidAxisLayout.RxRyStick)]
        [InlineData(false, true, true, true, false, GHAndroidAxisLayout.RxRyStick)]
        [InlineData(true, true, false, false, true, GHAndroidAxisLayout.Standard)]
        [InlineData(true, true, false, false, false, GHAndroidAxisLayout.Standard)]
        [InlineData(false, false, false, false, false, GHAndroidAxisLayout.Standard)]
        public void GetAxisLayout_PicksTheLayout(bool hasZ, bool hasRz, bool hasRx, bool hasRy, bool hasTriggerAxis,
            GHAndroidAxisLayout expected)
        {
            Assert.Equal(expected, GHAndroidGamepadMapping.GetAxisLayout(hasZ, hasRz, hasRx, hasRy, hasTriggerAxis));
        }

        [Fact]
        public void RescaleTrigger_CenteredRange()
        {
            Assert.Equal(0.0, GHAndroidGamepadMapping.RescaleTrigger(-1f, -1f, 1f), 4);
            Assert.Equal(0.5, GHAndroidGamepadMapping.RescaleTrigger(0f, -1f, 1f), 4);
            Assert.Equal(1.0, GHAndroidGamepadMapping.RescaleTrigger(1f, -1f, 1f), 4);
        }

        [Fact]
        public void RescaleTrigger_ZeroToOneRange()
        {
            Assert.Equal(0.0, GHAndroidGamepadMapping.RescaleTrigger(0f, 0f, 1f), 4);
            Assert.Equal(0.25, GHAndroidGamepadMapping.RescaleTrigger(0.25f, 0f, 1f), 4);
            Assert.Equal(1.0, GHAndroidGamepadMapping.RescaleTrigger(1f, 0f, 1f), 4);
            Assert.Equal(1.0, GHAndroidGamepadMapping.RescaleTrigger(1.5f, 0f, 1f), 4);
            Assert.Equal(0.0, GHAndroidGamepadMapping.RescaleTrigger(-0.5f, 0f, 1f), 4);
        }

        [Fact]
        public void RescaleTrigger_EmptyRangeIsZero()
        {
            Assert.Equal(0.0, GHAndroidGamepadMapping.RescaleTrigger(0.5f, 1f, 1f), 4);
            Assert.Equal(0.0, GHAndroidGamepadMapping.RescaleTrigger(0.5f, 1f, 0f), 4);
        }
    }
}

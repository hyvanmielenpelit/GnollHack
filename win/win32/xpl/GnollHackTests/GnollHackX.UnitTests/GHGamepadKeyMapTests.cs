using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadKeyMap: exactly the four grips on unique keys, a disjoint fallback list, and no key
       GnollHack, Steam or text entry uses. Key names are Avalonia PhysicalKey names. */
    public class GHGamepadKeyMapTests
    {
        private static readonly string[] DeniedKeys =
        {
            "F1", "F8", "F10", "F12", "Escape", "Enter", "Space", "Tab", "Backspace", "Delete",
            "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", "CapsLock", "NumLock", "ScrollLock",
            "ShiftLeft", "ShiftRight", "ControlLeft", "ControlRight", "AltLeft", "AltRight", "MetaLeft", "MetaRight"
        };

        private static bool IsDenied(string key)
        {
            if (Array.IndexOf(DeniedKeys, key) >= 0)
                return true;
            if (key.Length == 1)
                return true;
            if (key.StartsWith("Digit", StringComparison.Ordinal) || key.StartsWith("NumPad", StringComparison.Ordinal)
                || key.StartsWith("Key", StringComparison.Ordinal))
                return true;
            return false;
        }

        [Fact]
        public void Primary_IsExactlyTheFourGripsOnF2ToF5()
        {
            Assert.Equal(4, GHGamepadKeyMap.Primary.Length);
            Assert.Equal(GHGamepadButton.L4, GHGamepadKeyMap.Primary[0].Button);
            Assert.Equal("F2", GHGamepadKeyMap.Primary[0].KeyName);
            Assert.Equal(GHGamepadButton.L5, GHGamepadKeyMap.Primary[1].Button);
            Assert.Equal("F3", GHGamepadKeyMap.Primary[1].KeyName);
            Assert.Equal(GHGamepadButton.R4, GHGamepadKeyMap.Primary[2].Button);
            Assert.Equal("F4", GHGamepadKeyMap.Primary[2].KeyName);
            Assert.Equal(GHGamepadButton.R5, GHGamepadKeyMap.Primary[3].Button);
            Assert.Equal("F5", GHGamepadKeyMap.Primary[3].KeyName);
        }

        [Fact]
        public void Primary_KeysAndButtonsAreUnique()
        {
            HashSet<string> keys = new HashSet<string>();
            HashSet<GHGamepadButton> buttons = new HashSet<GHGamepadButton>();
            foreach (GHGamepadKeyBinding binding in GHGamepadKeyMap.Primary)
            {
                Assert.True(keys.Add(binding.KeyName));
                Assert.True(buttons.Add(binding.Button));
            }
        }

        [Fact]
        public void Fallback_IsDisjointFromPrimaryAndOrdered()
        {
            Assert.Equal(new[] { "F6", "F7", "F9", "Insert", "Pause" }, GHGamepadKeyMap.Fallback);
            foreach (GHGamepadKeyBinding binding in GHGamepadKeyMap.Primary)
                Assert.DoesNotContain(binding.KeyName, GHGamepadKeyMap.Fallback);
        }

        [Fact]
        public void NoKeyIsDenied()
        {
            foreach (GHGamepadKeyBinding binding in GHGamepadKeyMap.Primary)
                Assert.False(IsDenied(binding.KeyName), binding.KeyName);
            foreach (string key in GHGamepadKeyMap.Fallback)
                Assert.False(IsDenied(key), key);
        }

        [Fact]
        public void TryGetButton_FindsPrimaryKeysOnly()
        {
            GHGamepadButton button;
            Assert.True(GHGamepadKeyMap.TryGetButton("F4", out button));
            Assert.Equal(GHGamepadButton.R4, button);
            Assert.False(GHGamepadKeyMap.TryGetButton("F6", out button));
            Assert.False(GHGamepadKeyMap.TryGetButton("f2", out button));
            Assert.False(GHGamepadKeyMap.TryGetButton(null, out button));
        }
    }
}

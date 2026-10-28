using System;

namespace GnollHackX
{
    /* One grip and the key the Steam Input layout sends for it, by Avalonia PhysicalKey name */
    public struct GHGamepadKeyBinding
    {
        public GHGamepadButton Button;
        public string KeyName;

        public GHGamepadKeyBinding(GHGamepadButton button, string keyName)
        {
            Button = button;
            KeyName = keyName;
        }
    }

    /* The keys GnollHack's Steam Input layout binds to the back grips. GnollHack, Steam, KWin and gamescope
       ignore F2-F5 without modifiers. Fallback lists replacement keys in order of preference. */
    public static class GHGamepadKeyMap
    {
        // WORKAROUND(WA-STEAM-0001): Steam Input's virtual gamepad has no back grips
        public static readonly GHGamepadKeyBinding[] Primary =
        {
            new GHGamepadKeyBinding(GHGamepadButton.L4, "F2"),
            new GHGamepadKeyBinding(GHGamepadButton.L5, "F3"),
            new GHGamepadKeyBinding(GHGamepadButton.R4, "F4"),
            new GHGamepadKeyBinding(GHGamepadButton.R5, "F5"),
        };

        public static readonly string[] Fallback = { "F6", "F7", "F9", "Insert", "Pause" };

        public static bool TryGetButton(string keyName, out GHGamepadButton button)
        {
            foreach (GHGamepadKeyBinding binding in Primary)
            {
                if (string.Equals(binding.KeyName, keyName, StringComparison.Ordinal))
                {
                    button = binding.Button;
                    return true;
                }
            }
            button = GHGamepadButton.Count;
            return false;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Globalization;

namespace GnollHackX
{
    /* One [slot N] section of Steam's virtual gamepad info file: the real controller behind virtual pad N */
    public sealed class GHSteamVirtualGamepadSlot
    {
        public int Slot;
        public string Name;
        public ushort VendorId;
        public ushort ProductId;
        public ulong Handle;
        public string Type;
    }

    /* The file named by SteamVirtualGamepadInfo (SDL_steam_virtual_gamepad.c): [slot N] sections of
       name=, VID=, PID=, type= and handle= lines */
    public sealed class GHSteamVirtualGamepadInfo
    {
        private const ushort SonyVendorId = 0x054c;
        private const ushort NintendoVendorId = 0x057e;

        private readonly List<GHSteamVirtualGamepadSlot> _slots = new List<GHSteamVirtualGamepadSlot>();

        public IList<GHSteamVirtualGamepadSlot> Slots { get { return _slots; } }

        public static GHSteamVirtualGamepadInfo Parse(string text)
        {
            GHSteamVirtualGamepadInfo info = new GHSteamVirtualGamepadInfo();
            if (string.IsNullOrEmpty(text))
                return info;

            GHSteamVirtualGamepadSlot current = null;
            string[] lines = text.Split(new char[] { '\n' });
            foreach (string rawLine in lines)
            {
                string line = rawLine.Trim();
                if (line.Length == 0)
                    continue;

                if (line[0] == '[')
                {
                    current = null;
                    const string prefix = "[slot ";
                    if (line.StartsWith(prefix, StringComparison.OrdinalIgnoreCase) && line[line.Length - 1] == ']')
                    {
                        int slot;
                        string number = line.Substring(prefix.Length, line.Length - prefix.Length - 1).Trim();
                        if (int.TryParse(number, NumberStyles.None, CultureInfo.InvariantCulture, out slot))
                        {
                            current = new GHSteamVirtualGamepadSlot();
                            current.Slot = slot;
                            info._slots.Add(current);
                        }
                    }
                    continue;
                }

                if (current == null)
                    continue;
                int equals = line.IndexOf('=');
                if (equals <= 0)
                    continue;
                string key = line.Substring(0, equals).Trim();
                string value = line.Substring(equals + 1).Trim();
                ulong parsed;
                if (string.Equals(key, "name", StringComparison.OrdinalIgnoreCase))
                    current.Name = value;
                else if (string.Equals(key, "type", StringComparison.OrdinalIgnoreCase))
                    current.Type = value;
                else if (string.Equals(key, "VID", StringComparison.OrdinalIgnoreCase) && TryParseCInteger(value, out parsed))
                    current.VendorId = (ushort)parsed;
                else if (string.Equals(key, "PID", StringComparison.OrdinalIgnoreCase) && TryParseCInteger(value, out parsed))
                    current.ProductId = (ushort)parsed;
                else if (string.Equals(key, "handle", StringComparison.OrdinalIgnoreCase) && TryParseCInteger(value, out parsed))
                    current.Handle = parsed;
            }
            return info;
        }

        /* null when the file has no such slot */
        public GHSteamVirtualGamepadSlot GetSlot(int slot)
        {
            foreach (GHSteamVirtualGamepadSlot s in _slots)
            {
                if (s.Slot == slot)
                    return s;
            }
            return null;
        }

        /* The digits after "pad " in the device name ("Microsoft X-Box 360 pad 0"); -1 without them
           (SDL's GetSteamVirtualGamepadSlot) */
        public static int GetSlotFromDeviceName(string deviceName)
        {
            if (deviceName == null)
                return -1;
            int index = deviceName.IndexOf("pad ", StringComparison.Ordinal);
            if (index < 0)
                return -1;
            int start = index + 4;
            int end = start;
            while (end < deviceName.Length && deviceName[end] >= '0' && deviceName[end] <= '9')
                end++;
            if (end == start)
                return -1;
            int slot;
            if (!int.TryParse(deviceName.Substring(start, end - start), NumberStyles.None, CultureInfo.InvariantCulture, out slot))
                return -1;
            return slot;
        }

        /* Glyph style from the real controller's VID:PID */
        public static GHGamepadStyle GetStyle(ushort vendorId, ushort productId)
        {
            if (vendorId == GHEvdevDeviceFilter.SteamVendorId && productId == GHEvdevDeviceFilter.SteamDeckProductId)
                return GHGamepadStyle.SteamDeck;
            if (vendorId == SonyVendorId)
                return GHGamepadStyle.PlayStation;
            if (vendorId == NintendoVendorId)
                return GHGamepadStyle.Nintendo;
            return GHGamepadStyle.Xbox;
        }

        /* From the slot's entry when there is one; otherwise SteamDeck for slot 0 on a Steam Deck, else Xbox */
        public static GHGamepadStyle ResolveStyle(GHSteamVirtualGamepadSlot slotInfo, int slot, bool isSteamDeck)
        {
            if (slotInfo != null)
                return GetStyle(slotInfo.VendorId, slotInfo.ProductId);
            if (isSteamDeck && slot == 0)
                return GHGamepadStyle.SteamDeck;
            return GHGamepadStyle.Xbox;
        }

        /* For logs and GHGamepadControllerInfo.Details: real 28de:1205 "Steam Deck Controller" slot 0 */
        public static string Describe(GHSteamVirtualGamepadSlot slotInfo, int slot)
        {
            string slotText = "slot " + slot.ToString(CultureInfo.InvariantCulture);
            if (slotInfo == null)
                return "real unknown " + slotText;
            return "real " + slotInfo.VendorId.ToString("x4", CultureInfo.InvariantCulture) + ":"
                + slotInfo.ProductId.ToString("x4", CultureInfo.InvariantCulture)
                + " \"" + (slotInfo.Name ?? "") + "\" " + slotText;
        }

        /* strtoul with base 0: 0x for hex, a leading 0 for octal, else decimal */
        public static bool TryParseCInteger(string text, out ulong value)
        {
            value = 0;
            if (text == null)
                return false;
            string t = text.Trim();
            if (t.Length == 0)
                return false;
            if (t.Length > 2 && t[0] == '0' && (t[1] == 'x' || t[1] == 'X'))
                return ulong.TryParse(t.Substring(2), NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out value);
            if (t.Length > 1 && t[0] == '0')
            {
                ulong result = 0;
                int i;
                for (i = 1; i < t.Length; i++)
                {
                    char c = t[i];
                    if (c < '0' || c > '7' || result > (ulong.MaxValue >> 3))
                        return false;
                    result = (result << 3) | (uint)(c - '0');
                }
                value = result;
                return true;
            }
            return ulong.TryParse(t, NumberStyles.None, CultureInfo.InvariantCulture, out value);
        }
    }
}

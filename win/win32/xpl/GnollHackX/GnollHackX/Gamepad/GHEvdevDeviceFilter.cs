using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace GnollHackX
{
    /* A VID:PID list in the format of SDL's hints (SDL_LoadVIDPIDListFromHint): each "0x" starts a vendor
       number, the next "0x" its product number, and anything else is a separator. A leading '@' names a
       file that holds the list. */
    public sealed class GHVidPidList
    {
        public static readonly GHVidPidList Empty = new GHVidPidList();

        private readonly List<uint> _entries = new List<uint>();

        public int Count { get { return _entries.Count; } }

        /* The file named by an '@' hint; null otherwise */
        public string FilePath { get; private set; }

        public ushort GetVendor(int index)
        {
            return (ushort)(_entries[index] >> 16);
        }

        public ushort GetProduct(int index)
        {
            return (ushort)(_entries[index] & 0xFFFF);
        }

        public bool Contains(ushort vendor, ushort product)
        {
            return _entries.Contains(((uint)vendor << 16) | product);
        }

        /* readFile returns a file's text, or null; it is called only for an '@' hint */
        public static GHVidPidList Parse(string hint, Func<string, string> readFile)
        {
            if (string.IsNullOrEmpty(hint))
                return Empty;

            GHVidPidList list = new GHVidPidList();
            string text = hint;
            if (hint[0] == '@')
            {
                list.FilePath = hint.Substring(1);
                text = null;
                if (readFile != null)
                {
                    try
                    {
                        text = readFile(list.FilePath);
                    }
                    catch (Exception)
                    {
                        text = null;
                    }
                }
                if (string.IsNullOrEmpty(text))
                    return list;
            }

            int position = 0;
            while (true)
            {
                int start = text.IndexOf("0x", position, StringComparison.Ordinal);
                if (start < 0)
                    break;
                ushort vendor = (ushort)ParseHexAt(text, start, out position);
                start = text.IndexOf("0x", position, StringComparison.Ordinal);
                if (start < 0)
                    break;
                ushort product = (ushort)ParseHexAt(text, start, out position);
                list._entries.Add(((uint)vendor << 16) | product);
            }
            return list;
        }

        /* strtol(text + start, &end, 0) where text[start] starts "0x": the hex digits that follow, saturating
           at LONG_MAX; with no digit, only the "0" is consumed */
        private static long ParseHexAt(string text, int start, out int end)
        {
            long value = 0;
            int i = start + 2;
            bool any = false;
            while (i < text.Length)
            {
                int digit = HexDigit(text[i]);
                if (digit < 0)
                    break;
                any = true;
                if (value > (long.MaxValue - digit) / 16)
                    value = long.MaxValue;
                else
                    value = value * 16 + digit;
                i++;
            }
            end = any ? i : start + 1;
            return value;
        }

        private static int HexDigit(char c)
        {
            if (c >= '0' && c <= '9')
                return c - '0';
            if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
            if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;
            return -1;
        }

        /* For logs: "-" when empty, else the count and up to maxEntries entries */
        public string Describe(int maxEntries)
        {
            StringBuilder sb = new StringBuilder();
            if (FilePath != null)
                sb.Append('@').Append(FilePath).Append(':');
            if (_entries.Count == 0)
            {
                sb.Append('-');
                return sb.ToString();
            }
            sb.Append(_entries.Count.ToString(CultureInfo.InvariantCulture)).Append('[');
            int i;
            for (i = 0; i < _entries.Count && i < maxEntries; i++)
            {
                if (i > 0)
                    sb.Append(',');
                sb.Append("0x").Append(GetVendor(i).ToString("x4", CultureInfo.InvariantCulture));
                sb.Append("/0x").Append(GetProduct(i).ToString("x4", CultureInfo.InvariantCulture));
            }
            if (_entries.Count > maxEntries)
                sb.Append(",...");
            sb.Append(']');
            return sb.ToString();
        }
    }

    /* GNOLLHACK_GAMEPAD_EVDEV */
    public enum GHEvdevMode
    {
        Auto,   /* Steam's virtual pads, on a Steam Deck launched by Steam */
        Off,
        Any     /* also physical pads, without the Steam Deck and Steam launch requirements */
    }

    /* The launch environment as the evdev backend sees it, read once */
    public sealed class GHGamepadLaunchEnvironment
    {
        public const string ModeVariable = "GNOLLHACK_GAMEPAD_EVDEV";
        public const string AllowSteamVirtualVariable = "SDL_GAMECONTROLLER_ALLOW_STEAM_VIRTUAL_GAMEPAD";
        public const string IgnoreVariable = "SDL_GAMECONTROLLER_IGNORE_DEVICES";
        public const string AllowOnlyVariable = "SDL_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT";
        public const string VirtualGamepadInfoVariable = "SteamVirtualGamepadInfo";

        /* SteamGameId, SteamAppId (non-empty and not "0") or SteamClientLaunch=1 */
        public bool SteamLaunch { get; private set; }

        /* The variable that made SteamLaunch true, as NAME=value; "-" when false */
        public string SteamLaunchSource { get; private set; }

        /* SDL_GAMECONTROLLER_ALLOW_STEAM_VIRTUAL_GAMEPAD as an SDL boolean */
        public bool AllowSteamVirtualHint { get; private set; }

        /* AllowSteamVirtualHint or SteamLaunch */
        public bool AllowSteamVirtual { get; private set; }

        public GHVidPidList Ignore { get; private set; }
        public GHVidPidList AllowOnly { get; private set; }
        public GHEvdevMode Mode { get; private set; }
        public string ModeText { get; private set; }
        public string VirtualGamepadInfoPath { get; private set; }
        public bool IsSteamDeck { get; private set; }

        private GHGamepadLaunchEnvironment()
        {
        }

        /* getVariable returns an environment variable or null; readFile returns a file's text or null */
        public static GHGamepadLaunchEnvironment Create(Func<string, string> getVariable, Func<string, string> readFile, bool isSteamDeck)
        {
            GHGamepadLaunchEnvironment env = new GHGamepadLaunchEnvironment();
            env.IsSteamDeck = isSteamDeck;

            string gameId = Get(getVariable, "SteamGameId");
            string appId = Get(getVariable, "SteamAppId");
            string clientLaunch = Get(getVariable, "SteamClientLaunch");
            if (IsSteamIdSet(gameId))
                env.SteamLaunchSource = "SteamGameId=" + gameId.Trim();
            else if (IsSteamIdSet(appId))
                env.SteamLaunchSource = "SteamAppId=" + appId.Trim();
            else if (clientLaunch != null && clientLaunch.Trim() == "1")
                env.SteamLaunchSource = "SteamClientLaunch=1";
            env.SteamLaunch = env.SteamLaunchSource != null;
            if (!env.SteamLaunch)
                env.SteamLaunchSource = "-";

            env.AllowSteamVirtualHint = IsSdlTrue(Get(getVariable, AllowSteamVirtualVariable));
            env.AllowSteamVirtual = env.AllowSteamVirtualHint || env.SteamLaunch;
            env.Ignore = GHVidPidList.Parse(Get(getVariable, IgnoreVariable), readFile);
            env.AllowOnly = GHVidPidList.Parse(Get(getVariable, AllowOnlyVariable), readFile);

            string modeText = Get(getVariable, ModeVariable);
            env.ModeText = string.IsNullOrEmpty(modeText) ? "auto" : modeText.Trim();
            env.Mode = ParseMode(modeText);

            string info = Get(getVariable, VirtualGamepadInfoVariable);
            env.VirtualGamepadInfoPath = string.IsNullOrEmpty(info) ? null : info;
            return env;
        }

        private static string Get(Func<string, string> getVariable, string name)
        {
            return getVariable == null ? null : getVariable(name);
        }

        public static bool IsSteamIdSet(string value)
        {
            if (value == null)
                return false;
            string trimmed = value.Trim();
            return trimmed.Length > 0 && trimmed != "0";
        }

        /* 1, true, yes or on, in any case */
        public static bool IsSdlTrue(string value)
        {
            if (value == null)
                return false;
            string v = value.Trim();
            return v == "1"
                || string.Equals(v, "true", StringComparison.OrdinalIgnoreCase)
                || string.Equals(v, "yes", StringComparison.OrdinalIgnoreCase)
                || string.Equals(v, "on", StringComparison.OrdinalIgnoreCase);
        }

        /* auto when unset or unknown */
        public static GHEvdevMode ParseMode(string value)
        {
            if (value == null)
                return GHEvdevMode.Auto;
            string v = value.Trim();
            if (string.Equals(v, "off", StringComparison.OrdinalIgnoreCase))
                return GHEvdevMode.Off;
            if (string.Equals(v, "any", StringComparison.OrdinalIgnoreCase))
                return GHEvdevMode.Any;
            return GHEvdevMode.Auto;
        }

        /* auto: a Steam launch on a Steam Deck; any: always; off: never */
        public bool IsEvdevEligible(out string reason)
        {
            switch (Mode)
            {
            case GHEvdevMode.Off:
                reason = "mode=off";
                return false;
            case GHEvdevMode.Any:
                reason = "mode=any";
                return true;
            default:
                if (!AllowSteamVirtual)
                {
                    reason = "no Steam launch";
                    return false;
                }
                if (!IsSteamDeck)
                {
                    reason = "not a Steam Deck";
                    return false;
                }
                reason = "Steam launch on a Steam Deck";
                return true;
            }
        }
    }

    public enum GHEvdevVerdict
    {
        Disabled,
        NotGamepad,
        MotionSensors,
        SteamVirtualNotOurs,
        NotAllowed,
        IgnoredBySteam,
        PhysicalUnderSteam,
        ValvePhysical,
        AcceptSteamVirtual,
        AcceptPhysical
    }

    /* Which evdev devices the backend reads, in SDL_ShouldIgnoreGamepad's order */
    public static class GHEvdevDeviceFilter
    {
        public const ushort SteamVendorId = 0x28de;
        public const ushort SteamVirtualGamepadProductId = 0x11ff;
        public const ushort SteamDeckProductId = 0x1205;

        /* BTN_SOUTH, ABS_X and ABS_Y; keyboards, mice, touchscreens and hid-steam's lizard-mode nodes have not */
        public static bool IsGamepadLike(byte[] keyBits, byte[] absBits)
        {
            return GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_SOUTH)
                && GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_X)
                && GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_Y);
        }

        public static bool IsSteamVirtualGamepad(ushort vendor, ushort product)
        {
            return vendor == SteamVendorId && product == SteamVirtualGamepadProductId;
        }

        /* propBits may be null when the properties are unknown */
        public static GHEvdevVerdict Classify(ushort vendor, ushort product, byte[] keyBits, byte[] absBits, byte[] propBits,
            GHGamepadLaunchEnvironment env)
        {
            if (env == null || env.Mode == GHEvdevMode.Off)
                return GHEvdevVerdict.Disabled;
            if (!IsGamepadLike(keyBits, absBits))
                return GHEvdevVerdict.NotGamepad;
            if (GHEvdevProtocol.TestBit(propBits, GHEvdevProtocol.INPUT_PROP_ACCELEROMETER))
                return GHEvdevVerdict.MotionSensors;
            if (IsSteamVirtualGamepad(vendor, product))
            {
                if (env.AllowSteamVirtual || env.Mode == GHEvdevMode.Any)
                    return GHEvdevVerdict.AcceptSteamVirtual;
                return GHEvdevVerdict.SteamVirtualNotOurs;
            }
            if (env.AllowOnly.Count > 0 && !env.AllowOnly.Contains(vendor, product))
                return GHEvdevVerdict.NotAllowed;
            if (env.Ignore.Contains(vendor, product))
                return GHEvdevVerdict.IgnoredBySteam;
            if (env.Mode == GHEvdevMode.Auto)
                return GHEvdevVerdict.PhysicalUnderSteam;
            /* hid-steam's Steam Deck layout puts the triggers on ABS_HAT2X and ABS_HAT2Y */
            if (vendor == SteamVendorId)
                return GHEvdevVerdict.ValvePhysical;
            return GHEvdevVerdict.AcceptPhysical;
        }

        public static bool IsAccepted(GHEvdevVerdict verdict)
        {
            return verdict == GHEvdevVerdict.AcceptSteamVirtual || verdict == GHEvdevVerdict.AcceptPhysical;
        }

        public static string GetVerdictName(GHEvdevVerdict verdict)
        {
            switch (verdict)
            {
            case GHEvdevVerdict.Disabled:
                return "disabled";
            case GHEvdevVerdict.NotGamepad:
                return "not-gamepad";
            case GHEvdevVerdict.MotionSensors:
                return "motion-sensors";
            case GHEvdevVerdict.SteamVirtualNotOurs:
                return "steam-virtual-not-ours";
            case GHEvdevVerdict.NotAllowed:
                return "not-allowed";
            case GHEvdevVerdict.IgnoredBySteam:
                return "ignored-by-steam";
            case GHEvdevVerdict.PhysicalUnderSteam:
                return "physical-under-steam";
            case GHEvdevVerdict.ValvePhysical:
                return "valve-physical";
            case GHEvdevVerdict.AcceptSteamVirtual:
                return "accept:steam-virtual";
            case GHEvdevVerdict.AcceptPhysical:
                return "accept:physical";
            default:
                return verdict.ToString();
            }
        }

        /* From the capability bitmasks: analog sticks with X, Y, RX and RY; analog triggers with Z and RZ;
           stick clicks with both thumb buttons */
        public static GHGamepadCapabilities GetCapabilities(byte[] keyBits, byte[] absBits)
        {
            GHGamepadCapabilities caps = new GHGamepadCapabilities();
            caps.HasAnalogSticks = GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_X)
                && GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_Y)
                && GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_RX)
                && GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_RY);
            caps.HasAnalogTriggers = GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_Z)
                && GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_RZ);
            caps.HasStickClicks = GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_THUMBL)
                && GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_THUMBR);
            caps.HasGrips = false;
            caps.HasRightStickFullPush = false;
            return caps;
        }
    }
}

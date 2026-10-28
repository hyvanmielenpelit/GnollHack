using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHVidPidList, GHGamepadLaunchEnvironment and GHEvdevDeviceFilter: SDL's VID:PID hint format,
       the Steam launch and SDL boolean rules, evdev eligibility, the ordered verdict and the capabilities.
       The environment is the one measured in GnollHack's Steam launch on a Steam Deck; the ignore list is
       abbreviated, in Steam's format. */
    public class GHEvdevDeviceFilterTests
    {
        private const string DeckIgnoreDevices =
            "0x0079/0x0006,0x044f/0xb326,0x045e/0x028e,0x045e/0x0b13,0x046d/0xc21d,0x054c/0x0ce6,"
            + "0x057e/0x2009,0x28de/0x1102,0x28de/0x1142,0x28de/0x1205";

        private const string DeckInfoPath = "/home/deck/.local/share/Steam/config/virtualgamepadinfo.txt";

        private static readonly int[] XpadKeys =
        {
            0x130, 0x131, 0x133, 0x134, 0x136, 0x137, 0x13a, 0x13b, 0x13c, 0x13d, 0x13e
        };

        private static readonly int[] XpadAxes =
        {
            GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y, GHEvdevProtocol.ABS_Z, GHEvdevProtocol.ABS_RX,
            GHEvdevProtocol.ABS_RY, GHEvdevProtocol.ABS_RZ, GHEvdevProtocol.ABS_HAT0X, GHEvdevProtocol.ABS_HAT0Y
        };

        private static Dictionary<string, string> DeckSteamLaunch()
        {
            Dictionary<string, string> env = new Dictionary<string, string>();
            env["SteamGameId"] = "3558190";
            env["SteamAppId"] = "3558190";
            env["SteamClientLaunch"] = "1";
            env["SteamDeck"] = "1";
            env["SDL_GAMECONTROLLER_ALLOW_STEAM_VIRTUAL_GAMEPAD"] = "1";
            env["SDL_GAMECONTROLLER_IGNORE_DEVICES"] = DeckIgnoreDevices;
            env["SteamVirtualGamepadInfo"] = DeckInfoPath;
            return env;
        }

        private static GHGamepadLaunchEnvironment LaunchEnvironment(Dictionary<string, string> variables, bool isSteamDeck)
        {
            return GHGamepadLaunchEnvironment.Create(name =>
            {
                string value;
                return variables.TryGetValue(name, out value) ? value : null;
            }, null, isSteamDeck);
        }

        private static GHEvdevVerdict ClassifyXpad(ushort vendor, ushort product, GHGamepadLaunchEnvironment env)
        {
            return GHEvdevDeviceFilter.Classify(vendor, product, XpadKeyBits(), XpadAbsBits(), new byte[GHEvdevProtocol.PropBitsBytes], env);
        }

        private static byte[] XpadKeyBits()
        {
            return GHEvdevProtocolTests.Bits(GHEvdevProtocol.KeyBitsBytes, XpadKeys);
        }

        private static byte[] XpadAbsBits()
        {
            return GHEvdevProtocolTests.Bits(GHEvdevProtocol.AbsBitsBytes, XpadAxes);
        }

        /* The virtual pad: accepted and eligible in GnollHack's Steam launch on a Steam Deck */
        [Fact]
        public void VirtualPad_AcceptedUnderSteamOnADeck()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);
            string reason;

            Assert.True(env.IsEvdevEligible(out reason));
            Assert.Equal(GHEvdevVerdict.AcceptSteamVirtual, ClassifyXpad(0x28de, 0x11ff, env));
            Assert.Equal("accept:steam-virtual", GHEvdevDeviceFilter.GetVerdictName(GHEvdevVerdict.AcceptSteamVirtual));
        }

        [Fact]
        public void VirtualPad_AcceptedFromTheSysfsCapabilityFiles()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);
            byte[] keyBits = GHEvdevProtocol.ParseSysfsBitmap("7cdb000000000000 0 0 0 0\n", GHEvdevProtocol.KEY_MAX);
            byte[] absBits = GHEvdevProtocol.ParseSysfsBitmap("3003f\n", GHEvdevProtocol.ABS_MAX);
            byte[] propBits = GHEvdevProtocol.ParseSysfsBitmap("0\n", GHEvdevProtocol.INPUT_PROP_MAX);

            Assert.Equal(GHEvdevVerdict.AcceptSteamVirtual, GHEvdevDeviceFilter.Classify(0x28de, 0x11ff, keyBits, absBits, propBits, env));
        }

        [Fact]
        public void VirtualPad_RejectedWithoutASteamLaunch()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(new Dictionary<string, string>(), true);
            string reason;

            Assert.False(env.IsEvdevEligible(out reason));
            Assert.Equal("no Steam launch", reason);
            Assert.Equal(GHEvdevVerdict.SteamVirtualNotOurs, ClassifyXpad(0x28de, 0x11ff, env));
        }

        [Fact]
        public void VirtualPad_IneligibleInAutoWhenNotADeck()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), false);
            string reason;

            Assert.False(env.IsEvdevEligible(out reason));
            Assert.Equal("not a Steam Deck", reason);
        }

        [Fact]
        public void VirtualPad_AcceptedInAnyWithoutSteamOrDeck()
        {
            Dictionary<string, string> variables = new Dictionary<string, string>();
            variables["GNOLLHACK_GAMEPAD_EVDEV"] = "any";
            GHGamepadLaunchEnvironment env = LaunchEnvironment(variables, false);
            string reason;

            Assert.True(env.IsEvdevEligible(out reason));
            Assert.Equal(GHEvdevVerdict.AcceptSteamVirtual, ClassifyXpad(0x28de, 0x11ff, env));
        }

        [Fact]
        public void Off_DisablesEverything()
        {
            Dictionary<string, string> variables = DeckSteamLaunch();
            variables["GNOLLHACK_GAMEPAD_EVDEV"] = "off";
            GHGamepadLaunchEnvironment env = LaunchEnvironment(variables, true);
            string reason;

            Assert.False(env.IsEvdevEligible(out reason));
            Assert.Equal(GHEvdevVerdict.Disabled, ClassifyXpad(0x28de, 0x11ff, env));
        }

        /* hid-steam's gamepad node for the built-in controller, present when no client holds hidraw */
        [Fact]
        public void DeckController_Rejected()
        {
            byte[] keyBits = XpadKeyBits();
            byte[] absBits = GHEvdevProtocolTests.Bits(GHEvdevProtocol.AbsBitsBytes, GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y,
                GHEvdevProtocol.ABS_RX, GHEvdevProtocol.ABS_RY, GHEvdevProtocol.ABS_HAT0X, GHEvdevProtocol.ABS_HAT0Y,
                GHEvdevProtocol.ABS_HAT2X, GHEvdevProtocol.ABS_HAT2Y);

            GHGamepadLaunchEnvironment steam = LaunchEnvironment(DeckSteamLaunch(), true);
            Assert.Equal(GHEvdevVerdict.IgnoredBySteam, GHEvdevDeviceFilter.Classify(0x28de, 0x1205, keyBits, absBits, null, steam));

            Dictionary<string, string> anyVariables = new Dictionary<string, string>();
            anyVariables["GNOLLHACK_GAMEPAD_EVDEV"] = "any";
            GHGamepadLaunchEnvironment any = LaunchEnvironment(anyVariables, true);
            Assert.Equal(GHEvdevVerdict.ValvePhysical, GHEvdevDeviceFilter.Classify(0x28de, 0x1205, keyBits, absBits, null, any));

            Dictionary<string, string> noList = DeckSteamLaunch();
            noList.Remove("SDL_GAMECONTROLLER_IGNORE_DEVICES");
            GHGamepadLaunchEnvironment auto = LaunchEnvironment(noList, true);
            Assert.Equal(GHEvdevVerdict.PhysicalUnderSteam, GHEvdevDeviceFilter.Classify(0x28de, 0x1205, keyBits, absBits, null, auto));

            GHGamepadCapabilities caps = GHEvdevDeviceFilter.GetCapabilities(keyBits, absBits);
            Assert.True(caps.HasAnalogSticks);
            Assert.False(caps.HasAnalogTriggers);
        }

        /* hid-steam's lizard-mode mouse and keyboard nodes, "Valve Software Steam Controller" */
        [Fact]
        public void DeckLizardNodes_NotGamepads()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);
            byte[] mouseKeys = GHEvdevProtocolTests.Bits(GHEvdevProtocol.KeyBitsBytes, 0x110, 0x111, 0x112);
            byte[] keyboardKeys = GHEvdevProtocol.ParseSysfsBitmap("10000000000000 0 0 0 0 1c0000000000 0 0", GHEvdevProtocol.KEY_MAX);
            byte[] noAxes = new byte[GHEvdevProtocol.AbsBitsBytes];

            Assert.Equal(GHEvdevVerdict.NotGamepad, GHEvdevDeviceFilter.Classify(0x28de, 0x1205, mouseKeys, noAxes, null, env));
            Assert.Equal(GHEvdevVerdict.NotGamepad, GHEvdevDeviceFilter.Classify(0x28de, 0x1205, keyboardKeys, noAxes, null, env));
        }

        [Fact]
        public void BluetoothXboxPad_RejectedInAutoAndAcceptedInAnyUnlessIgnored()
        {
            Dictionary<string, string> steam = DeckSteamLaunch();
            steam.Remove("SDL_GAMECONTROLLER_IGNORE_DEVICES");
            Assert.Equal(GHEvdevVerdict.PhysicalUnderSteam, ClassifyXpad(0x045e, 0x0b13, LaunchEnvironment(steam, true)));

            Dictionary<string, string> any = new Dictionary<string, string>();
            any["GNOLLHACK_GAMEPAD_EVDEV"] = "any";
            Assert.Equal(GHEvdevVerdict.AcceptPhysical, ClassifyXpad(0x045e, 0x0b13, LaunchEnvironment(any, false)));

            any["SDL_GAMECONTROLLER_IGNORE_DEVICES"] = DeckIgnoreDevices;
            Assert.Equal(GHEvdevVerdict.IgnoredBySteam, ClassifyXpad(0x045e, 0x0b13, LaunchEnvironment(any, false)));
            Assert.Equal(GHEvdevVerdict.AcceptPhysical, ClassifyXpad(0x0e6f, 0x02a1, LaunchEnvironment(any, false)));
        }

        [Fact]
        public void Keyboard_NotAGamepad()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);
            byte[] keyBits = GHEvdevProtocol.ParseSysfsBitmap("402000000 3803078f800d001 feffffdfffefffff fffffffffffffffe",
                GHEvdevProtocol.KEY_MAX);
            byte[] absBits = GHEvdevProtocol.ParseSysfsBitmap("0", GHEvdevProtocol.ABS_MAX);

            Assert.Equal(GHEvdevVerdict.NotGamepad, GHEvdevDeviceFilter.Classify(0x0001, 0x0001, keyBits, absBits, null, env));
        }

        [Fact]
        public void Touchscreen_NotAGamepad()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);
            byte[] keyBits = GHEvdevProtocol.ParseSysfsBitmap("400 0 0 0 0 0", GHEvdevProtocol.KEY_MAX);
            byte[] absBits = GHEvdevProtocol.ParseSysfsBitmap("260800000000003", GHEvdevProtocol.ABS_MAX);

            Assert.True(GHEvdevProtocol.TestBit(keyBits, GHEvdevProtocol.BTN_TOUCH));
            Assert.True(GHEvdevProtocol.TestBit(absBits, GHEvdevProtocol.ABS_X));
            Assert.Equal(GHEvdevVerdict.NotGamepad, GHEvdevDeviceFilter.Classify(0x2808, 0x1015, keyBits, absBits, null, env));
        }

        [Fact]
        public void MotionSensors_Rejected()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);
            byte[] accelerometer = GHEvdevProtocolTests.Bits(GHEvdevProtocol.PropBitsBytes, GHEvdevProtocol.INPUT_PROP_ACCELEROMETER);
            byte[] sensorAxes = GHEvdevProtocolTests.Bits(GHEvdevProtocol.AbsBitsBytes, GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y,
                GHEvdevProtocol.ABS_Z, GHEvdevProtocol.ABS_RX, GHEvdevProtocol.ABS_RY, GHEvdevProtocol.ABS_RZ);

            /* "Steam Deck Motion Sensors" has no keys */
            Assert.Equal(GHEvdevVerdict.NotGamepad,
                GHEvdevDeviceFilter.Classify(0x28de, 0x1205, new byte[GHEvdevProtocol.KeyBitsBytes], sensorAxes, accelerometer, env));
            /* A motion node that also reports buttons */
            Assert.Equal(GHEvdevVerdict.MotionSensors,
                GHEvdevDeviceFilter.Classify(0x054c, 0x0ce6, XpadKeyBits(), sensorAxes, accelerometer, env));
            Assert.Equal(GHEvdevVerdict.MotionSensors,
                GHEvdevDeviceFilter.Classify(0x28de, 0x11ff, XpadKeyBits(), XpadAbsBits(), accelerometer, env));
        }

        [Fact]
        public void AllowList_LimitsPhysicalPadsButNotTheVirtualPad()
        {
            Dictionary<string, string> variables = new Dictionary<string, string>();
            variables["GNOLLHACK_GAMEPAD_EVDEV"] = "any";
            variables["SDL_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT"] = "0x045e/0x0b13";
            GHGamepadLaunchEnvironment env = LaunchEnvironment(variables, false);

            Assert.Equal(GHEvdevVerdict.AcceptPhysical, ClassifyXpad(0x045e, 0x0b13, env));
            Assert.Equal(GHEvdevVerdict.NotAllowed, ClassifyXpad(0x046d, 0xc21d, env));
            Assert.Equal(GHEvdevVerdict.AcceptSteamVirtual, ClassifyXpad(0x28de, 0x11ff, env));
        }

        [Fact]
        public void Capabilities_FromTheVirtualPadBitmasks()
        {
            GHGamepadCapabilities caps = GHEvdevDeviceFilter.GetCapabilities(XpadKeyBits(), XpadAbsBits());

            Assert.True(caps.HasAnalogSticks);
            Assert.True(caps.HasAnalogTriggers);
            Assert.True(caps.HasStickClicks);
            Assert.False(caps.HasGrips);
            Assert.False(caps.HasRightStickFullPush);
        }

        [Fact]
        public void VidPidList_ParsesSdlHints()
        {
            GHVidPidList list = GHVidPidList.Parse("0x28de/0x1205,0x28de/0x1142", null);

            Assert.Equal(2, list.Count);
            Assert.True(list.Contains(0x28de, 0x1205));
            Assert.True(list.Contains(0x28de, 0x1142));
            Assert.False(list.Contains(0x28de, 0x11ff));
            Assert.False(list.Contains(0x1205, 0x28de));
        }

        [Fact]
        public void VidPidList_TheDeckIgnoreList()
        {
            GHVidPidList list = GHVidPidList.Parse(DeckIgnoreDevices, null);

            Assert.Equal(10, list.Count);
            Assert.True(list.Contains(0x28de, 0x1205));
            Assert.False(list.Contains(0x28de, 0x11ff));
            Assert.Equal("10[0x0079/0x0006,0x044f/0xb326,...]", list.Describe(2));
        }

        [Fact]
        public void VidPidList_IgnoresJunkAndSpaces()
        {
            GHVidPidList list = GHVidPidList.Parse(" foo 0x28DE bar  0x1205 ;; 0x45e,\t0x28e  0xBEEF", null);

            Assert.Equal(2, list.Count);
            Assert.True(list.Contains(0x28de, 0x1205));
            Assert.True(list.Contains(0x045e, 0x028e));
            Assert.Equal(0x045e, list.GetVendor(1));
            Assert.Equal(0x028e, list.GetProduct(1));
        }

        [Fact]
        public void VidPidList_FollowsStrtol()
        {
            /* "0x" without digits reads as 0; values wider than 16 bits are truncated */
            GHVidPidList list = GHVidPidList.Parse("0x/0x1205,0x123456789/0x1", null);

            Assert.Equal(2, list.Count);
            Assert.True(list.Contains(0x0000, 0x1205));
            Assert.True(list.Contains(0x6789, 0x0001));
        }

        [Fact]
        public void VidPidList_EmptyAndNull()
        {
            Assert.Equal(0, GHVidPidList.Parse(null, null).Count);
            Assert.Equal(0, GHVidPidList.Parse("", null).Count);
            Assert.Equal(0, GHVidPidList.Parse("no numbers here", null).Count);
            Assert.Equal("-", GHVidPidList.Parse("", null).Describe(4));
        }

        [Fact]
        public void VidPidList_ReadsAnAtFile()
        {
            Func<string, string> reader = path => path == "/tmp/ignore.txt" ? "0x28de/0x1205\n0x045e/0x0b13\n" : null;

            GHVidPidList list = GHVidPidList.Parse("@/tmp/ignore.txt", reader);
            Assert.Equal(2, list.Count);
            Assert.True(list.Contains(0x045e, 0x0b13));
            Assert.Equal("/tmp/ignore.txt", list.FilePath);

            Assert.Equal(0, GHVidPidList.Parse("@/tmp/missing.txt", reader).Count);
            Assert.Equal(0, GHVidPidList.Parse("@/tmp/ignore.txt", null).Count);
            Assert.Equal(0, GHVidPidList.Parse("@/tmp/ignore.txt", path => { throw new System.IO.IOException("denied"); }).Count);
        }

        [Theory]
        [InlineData("1", true)]
        [InlineData("true", true)]
        [InlineData("TRUE", true)]
        [InlineData("yes", true)]
        [InlineData(" on ", true)]
        [InlineData("0", false)]
        [InlineData("false", false)]
        [InlineData("off", false)]
        [InlineData("no", false)]
        [InlineData("", false)]
        [InlineData(null, false)]
        public void SdlBoolean(string value, bool expected)
        {
            Assert.Equal(expected, GHGamepadLaunchEnvironment.IsSdlTrue(value));
        }

        [Theory]
        [InlineData("3558190", null, null, true, "SteamGameId=3558190")]
        [InlineData(null, "3558190", null, true, "SteamAppId=3558190")]
        [InlineData(null, null, "1", true, "SteamClientLaunch=1")]
        [InlineData("0", "0", null, false, "-")]
        [InlineData("", "", "0", false, "-")]
        [InlineData(null, null, null, false, "-")]
        public void SteamLaunch_Matrix(string gameId, string appId, string clientLaunch, bool expected, string source)
        {
            Dictionary<string, string> variables = new Dictionary<string, string>();
            if (gameId != null)
                variables["SteamGameId"] = gameId;
            if (appId != null)
                variables["SteamAppId"] = appId;
            if (clientLaunch != null)
                variables["SteamClientLaunch"] = clientLaunch;

            GHGamepadLaunchEnvironment env = LaunchEnvironment(variables, true);

            Assert.Equal(expected, env.SteamLaunch);
            Assert.Equal(source, env.SteamLaunchSource);
            Assert.Equal(expected, env.AllowSteamVirtual);
        }

        /* A Non-Steam shortcut has SteamAppId=0; Steam's SDL hint alone still allows the virtual pad */
        [Fact]
        public void AllowSteamVirtualHint_AloneMakesTheBackendEligible()
        {
            Dictionary<string, string> variables = new Dictionary<string, string>();
            variables["SteamAppId"] = "0";
            variables["SDL_GAMECONTROLLER_ALLOW_STEAM_VIRTUAL_GAMEPAD"] = "1";
            GHGamepadLaunchEnvironment env = LaunchEnvironment(variables, true);
            string reason;

            Assert.False(env.SteamLaunch);
            Assert.True(env.AllowSteamVirtual);
            Assert.True(env.IsEvdevEligible(out reason));
            Assert.Equal(GHEvdevVerdict.AcceptSteamVirtual, ClassifyXpad(0x28de, 0x11ff, env));
        }

        [Fact]
        public void Environment_ReadsTheDeckLaunch()
        {
            GHGamepadLaunchEnvironment env = LaunchEnvironment(DeckSteamLaunch(), true);

            Assert.True(env.SteamLaunch);
            Assert.True(env.AllowSteamVirtualHint);
            Assert.Equal(GHEvdevMode.Auto, env.Mode);
            Assert.Equal("auto", env.ModeText);
            Assert.Equal(DeckInfoPath, env.VirtualGamepadInfoPath);
            Assert.True(env.Ignore.Contains(0x28de, 0x1205));
            Assert.Equal(0, env.AllowOnly.Count);
            Assert.True(env.IsSteamDeck);
        }

        [Theory]
        [InlineData(null, GHEvdevMode.Auto)]
        [InlineData("", GHEvdevMode.Auto)]
        [InlineData("auto", GHEvdevMode.Auto)]
        [InlineData("bogus", GHEvdevMode.Auto)]
        [InlineData("OFF", GHEvdevMode.Off)]
        [InlineData(" Any ", GHEvdevMode.Any)]
        public void Mode_Parses(string value, GHEvdevMode expected)
        {
            Assert.Equal(expected, GHGamepadLaunchEnvironment.ParseMode(value));
        }
    }
}

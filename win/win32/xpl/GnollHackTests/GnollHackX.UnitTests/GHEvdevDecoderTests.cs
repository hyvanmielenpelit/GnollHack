using System;
using System.Collections.Generic;
using System.Globalization;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHEvdevLayout, GHEvdevDecoder and GHEvdevClock with the Steam virtual gamepad measured on a Steam
       Deck: sticks -32767..32767 (fuzz 16, flat 128), triggers 0..255, hat -1..1. The raw lines come from the
       Step 1b raw dump on the Deck. */
    public class GHEvdevDecoderTests
    {
        private const int Cid = 1;

        private static readonly int[] XpadKeys =
        {
            0x130, 0x131, 0x133, 0x134, 0x136, 0x137, 0x13a, 0x13b, 0x13c, 0x13d, 0x13e
        };

        private static readonly int[] XpadAxes =
        {
            GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y, GHEvdevProtocol.ABS_Z, GHEvdevProtocol.ABS_RX,
            GHEvdevProtocol.ABS_RY, GHEvdevProtocol.ABS_RZ, GHEvdevProtocol.ABS_HAT0X, GHEvdevProtocol.ABS_HAT0Y
        };

        internal static GHEvdevLayout DeckLayout(ushort vendor = 0x28de)
        {
            GHEvdevAbsInfo[] info = new GHEvdevAbsInfo[GHEvdevProtocol.ABS_MAX + 1];
            foreach (int axis in new[] { GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y, GHEvdevProtocol.ABS_RX, GHEvdevProtocol.ABS_RY })
                info[axis] = Abs(-32767, 32767, 16, 128);
            info[GHEvdevProtocol.ABS_Z] = Abs(0, 255, 0, 0);
            info[GHEvdevProtocol.ABS_RZ] = Abs(0, 255, 0, 0);
            info[GHEvdevProtocol.ABS_HAT0X] = Abs(-1, 1, 0, 0);
            info[GHEvdevProtocol.ABS_HAT0Y] = Abs(-1, 1, 0, 0);
            return GHEvdevLayout.Create(GHEvdevProtocolTests.Bits(GHEvdevProtocol.KeyBitsBytes, XpadKeys),
                GHEvdevProtocolTests.Bits(GHEvdevProtocol.AbsBitsBytes, XpadAxes), info, vendor);
        }

        private static GHEvdevAbsInfo Abs(int min, int max, int fuzz, int flat)
        {
            GHEvdevAbsInfo a = new GHEvdevAbsInfo();
            a.Minimum = min;
            a.Maximum = max;
            a.Fuzz = fuzz;
            a.Flat = flat;
            return a;
        }

        /* "GAMEPAD raw cid=1 t=34765.398714 type=1 code=0x130 value=1" */
        internal static GHEvdevInputEvent ParseRawLine(string line)
        {
            GHEvdevInputEvent e = new GHEvdevInputEvent();
            foreach (string part in line.Split(' '))
            {
                if (part.StartsWith("t=", StringComparison.Ordinal))
                {
                    string[] time = part.Substring(2).Split('.');
                    e.Seconds = long.Parse(time[0], CultureInfo.InvariantCulture);
                    e.Microseconds = long.Parse(time[1], CultureInfo.InvariantCulture);
                }
                else if (part.StartsWith("type=", StringComparison.Ordinal))
                {
                    e.Type = ushort.Parse(part.Substring(5), CultureInfo.InvariantCulture);
                }
                else if (part.StartsWith("code=0x", StringComparison.Ordinal))
                {
                    e.Code = ushort.Parse(part.Substring(7), NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture);
                }
                else if (part.StartsWith("value=", StringComparison.Ordinal))
                {
                    e.Value = int.Parse(part.Substring(6), CultureInfo.InvariantCulture);
                }
            }
            return e;
        }

        private static void Feed(GHEvdevDecoder decoder, GHRecordingSink sink, params string[] lines)
        {
            foreach (string line in lines)
                decoder.Feed(ParseRawLine(line), 0, sink);
        }

        private static GHEvdevInputEvent Ev(ushort type, int code, int value)
        {
            GHEvdevInputEvent e = new GHEvdevInputEvent();
            e.Type = type;
            e.Code = (ushort)code;
            e.Value = value;
            return e;
        }

        private static void Feed(GHEvdevDecoder decoder, GHRecordingSink sink, params GHEvdevInputEvent[] events)
        {
            foreach (GHEvdevInputEvent e in events)
                decoder.Feed(e, 0, sink);
        }

        private static GHEvdevInputEvent Syn()
        {
            return Ev(GHEvdevProtocol.EV_SYN, GHEvdevProtocol.SYN_REPORT, 0);
        }

        [Fact]
        public void DeckDump_APressWithRestJitter()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink,
                "GAMEPAD raw cid=1 t=34765.398714 type=1 code=0x130 value=1",
                "GAMEPAD raw cid=1 t=34765.398714 type=3 code=0x004 value=1155",
                "GAMEPAD raw cid=1 t=34765.398714 type=0 code=0x000 value=0");

            Assert.Equal(2, sink.Calls.Count);
            Assert.Equal("button 1 A down", sink.Calls[0]);
            Assert.StartsWith("stick 1 1 ", sink.Calls[1]);
            Assert.Equal(1155f / 32767f, sink.LastStick[3], 4);

            sink.Calls.Clear();
            Feed(decoder, sink,
                "GAMEPAD raw cid=1 t=34765.566228 type=1 code=0x130 value=0",
                "GAMEPAD raw cid=1 t=34765.566228 type=0 code=0x000 value=0");
            Assert.Equal(new[] { "button 1 A up" }, sink.Calls);
        }

        [Fact]
        public void DeckDump_HatLeftPressAndRelease()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink,
                "GAMEPAD raw cid=1 t=34767.110607 type=3 code=0x010 value=-1",
                "GAMEPAD raw cid=1 t=34767.110607 type=0 code=0x000 value=0");
            Assert.Equal(new[] { "button 1 DPadLeft down" }, sink.Calls);

            sink.Calls.Clear();
            Feed(decoder, sink,
                "GAMEPAD raw cid=1 t=34767.222653 type=3 code=0x010 value=0",
                "GAMEPAD raw cid=1 t=34767.222653 type=3 code=0x000 value=383",
                "GAMEPAD raw cid=1 t=34767.222653 type=3 code=0x001 value=876",
                "GAMEPAD raw cid=1 t=34767.222653 type=0 code=0x000 value=0");
            Assert.Equal(2, sink.Calls.Count);
            Assert.Equal("button 1 DPadLeft up", sink.Calls[0]);
            Assert.StartsWith("stick 1 0 ", sink.Calls[1]);
        }

        [Fact]
        public void Hat_CardinalsAndDiagonals()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_HAT0Y, -1), Syn());
            Assert.Equal(new[] { "button 1 DPadUp down" }, sink.Calls);
            sink.Calls.Clear();

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_HAT0X, 1), Syn());
            Assert.Equal(new[] { "button 1 DPadRight down" }, sink.Calls);
            sink.Calls.Clear();

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_HAT0Y, 1), Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_HAT0X, 0), Syn());
            Assert.Equal(new[] { "button 1 DPadDown down", "button 1 DPadRight up", "button 1 DPadUp up" }, Sorted(sink.Calls));
        }

        [Fact]
        public void Sticks_ExtremesAndCenterWithUpNegative()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_Y, -32767), Syn());
            Assert.Equal(-1f, sink.LastStick[1], 5);

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_Y, 32767), Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_X, -32768), Syn());
            Assert.Equal(1f, sink.LastStick[1], 5);
            Assert.Equal(-1f, sink.LastStick[0], 5);

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_RX, 32767), Syn());
            Assert.Equal(1f, sink.LastStick[2], 5);

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_X, 0), Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_Y, 0), Syn());
            Assert.Equal(0f, sink.LastStick[0], 5);
            Assert.Equal(0f, sink.LastStick[1], 5);
        }

        [Fact]
        public void Triggers_ScaleToZeroOne()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_Z, 255), Syn());
            Assert.Equal(new[] { "trigger 1 L 1.000" }, sink.Calls);
            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_RZ, 51), Syn());
            Assert.Equal(0.2f, sink.LastTrigger[1], 4);
            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_Z, 0), Syn());
            Assert.Equal(0f, sink.LastTrigger[0], 5);
        }

        [Fact]
        public void SplitFrame_CommitsOnlyAtSynReport()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_KEY, GHEvdevProtocol.BTN_EAST, 1), Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_X, 20000));
            Assert.Empty(sink.Calls);
            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_ABS, GHEvdevProtocol.ABS_Y, 100));
            Assert.Empty(sink.Calls);
            Feed(decoder, sink, Syn());
            Assert.Equal(2, sink.Calls.Count);
            Assert.Equal("button 1 B down", sink.Calls[0]);
        }

        [Fact]
        public void SynDropped_DiscardsUntilSynReportAndNeedsResync()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink,
                Ev(GHEvdevProtocol.EV_KEY, GHEvdevProtocol.BTN_SOUTH, 1),
                Ev(GHEvdevProtocol.EV_SYN, GHEvdevProtocol.SYN_DROPPED, 0),
                Ev(GHEvdevProtocol.EV_KEY, GHEvdevProtocol.BTN_TL, 1),
                Syn(),
                Ev(GHEvdevProtocol.EV_KEY, GHEvdevProtocol.BTN_TR, 1));
            Assert.Empty(sink.Calls);
            Assert.True(decoder.NeedsResync);

            /* The device reports A held and RB held */
            byte[] keyState = GHEvdevProtocolTests.Bits(GHEvdevProtocol.KeyBitsBytes, GHEvdevProtocol.BTN_SOUTH, GHEvdevProtocol.BTN_TR);
            decoder.ApplySnapshot(keyState, new int[GHEvdevProtocol.ABS_MAX + 1], 0, sink, false);

            Assert.False(decoder.NeedsResync);
            Assert.Contains("button 1 A down", sink.Calls);
            Assert.Contains("button 1 RB down", sink.Calls);
            Assert.DoesNotContain("button 1 LB down", sink.Calls);
        }

        [Fact]
        public void SilentSnapshot_NoButtonEdgesButCurrentAxes()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();
            byte[] keyState = GHEvdevProtocolTests.Bits(GHEvdevProtocol.KeyBitsBytes, GHEvdevProtocol.BTN_SOUTH);
            int[] abs = new int[GHEvdevProtocol.ABS_MAX + 1];
            abs[GHEvdevProtocol.ABS_RZ] = 255;
            abs[GHEvdevProtocol.ABS_Y] = -32767;

            decoder.ApplySnapshot(keyState, abs, 0, sink, true);

            Assert.DoesNotContain(sink.Calls, c => c.StartsWith("button", StringComparison.Ordinal));
            Assert.Equal(-1f, sink.LastStick[1], 5);
            Assert.Equal(1f, sink.LastTrigger[1], 5);

            /* Releasing the held A is an ordinary up edge */
            sink.Calls.Clear();
            Feed(decoder, sink, Ev(GHEvdevProtocol.EV_KEY, GHEvdevProtocol.BTN_SOUTH, 0), Syn());
            Assert.Equal(new[] { "button 1 A up" }, sink.Calls);
        }

        [Fact]
        public void ModeMiscAndUnknownCodes_AreIgnored()
        {
            GHEvdevDecoder decoder = new GHEvdevDecoder(DeckLayout(), Cid);
            GHRecordingSink sink = new GHRecordingSink();

            Feed(decoder, sink,
                Ev(GHEvdevProtocol.EV_KEY, GHEvdevProtocol.BTN_MODE, 1),
                Ev(GHEvdevProtocol.EV_MSC, 4, 123),
                Ev(GHEvdevProtocol.EV_KEY, 0x2c0, 1),
                Ev(GHEvdevProtocol.EV_ABS, 0x28, 5),
                Syn());

            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void FaceButtons_NonSonyAndSony()
        {
            GHEvdevLayout xpad = DeckLayout();
            GHGamepadButton b;
            Assert.True(xpad.TryMapKey(GHEvdevProtocol.BTN_X, out b));
            Assert.Equal(GHGamepadButton.X, b);
            Assert.True(xpad.TryMapKey(GHEvdevProtocol.BTN_Y, out b));
            Assert.Equal(GHGamepadButton.Y, b);

            GHEvdevLayout sony = DeckLayout(0x054c);
            Assert.True(sony.TryMapKey(GHEvdevProtocol.BTN_X, out b));
            Assert.Equal(GHGamepadButton.Y, b);
            Assert.True(sony.TryMapKey(GHEvdevProtocol.BTN_Y, out b));
            Assert.Equal(GHGamepadButton.X, b);
        }

        [Fact]
        public void Layout_MapsTheXpadButtons()
        {
            GHEvdevLayout layout = DeckLayout();
            GHGamepadButton b;
            Assert.True(layout.TryMapKey(GHEvdevProtocol.BTN_SELECT, out b));
            Assert.Equal(GHGamepadButton.View, b);
            Assert.True(layout.TryMapKey(GHEvdevProtocol.BTN_START, out b));
            Assert.Equal(GHGamepadButton.Menu, b);
            Assert.True(layout.TryMapKey(GHEvdevProtocol.BTN_THUMBR, out b));
            Assert.Equal(GHGamepadButton.R3, b);
            /* Analog triggers exist, so the trigger buttons and the hat-covered D-pad keys are not used */
            Assert.False(layout.TryMapKey(GHEvdevProtocol.BTN_TL2, out b));
            Assert.False(layout.TryMapKey(GHEvdevProtocol.BTN_DPAD_UP, out b));
            Assert.False(layout.TryMapKey(GHEvdevProtocol.BTN_MODE, out b));
            Assert.True(layout.HasAnalogTriggers);
            Assert.True(layout.Capabilities.HasAnalogSticks);
            Assert.True(layout.Capabilities.HasStickClicks);
        }

        [Fact]
        public void Layout_TriggerButtonsAndDPadKeysWithoutAxes()
        {
            byte[] keys = GHEvdevProtocolTests.Bits(GHEvdevProtocol.KeyBitsBytes, GHEvdevProtocol.BTN_SOUTH,
                GHEvdevProtocol.BTN_TL2, GHEvdevProtocol.BTN_TR2, GHEvdevProtocol.BTN_DPAD_UP);
            byte[] axes = GHEvdevProtocolTests.Bits(GHEvdevProtocol.AbsBitsBytes, GHEvdevProtocol.ABS_X, GHEvdevProtocol.ABS_Y);
            GHEvdevLayout layout = GHEvdevLayout.Create(keys, axes, null, 0x045e);
            GHGamepadButton b;

            Assert.True(layout.TryMapKey(GHEvdevProtocol.BTN_TL2, out b));
            Assert.Equal(GHGamepadButton.LT, b);
            Assert.True(layout.TryMapKey(GHEvdevProtocol.BTN_DPAD_UP, out b));
            Assert.Equal(GHGamepadButton.DPadUp, b);
            Assert.False(layout.HasAnalogTriggers);
        }

        [Fact]
        public void RestFilter_StickAxesInsideTheDeadzone()
        {
            GHEvdevLayout layout = DeckLayout();

            /* Rest jitter measured on the Deck stays below 4.7 % of full scale */
            Assert.True(layout.IsStickAxisAtRest(GHEvdevProtocol.ABS_RY, 1155, 0.15f));
            Assert.True(layout.IsStickAxisAtRest(GHEvdevProtocol.ABS_RX, -606, 0.15f));
            Assert.False(layout.IsStickAxisAtRest(GHEvdevProtocol.ABS_Y, -20000, 0.15f));
            Assert.False(layout.IsStickAxisAtRest(GHEvdevProtocol.ABS_Z, 0, 0.15f));
            Assert.False(layout.IsStickAxisAtRest(GHEvdevProtocol.ABS_HAT0X, 0, 0.15f));
        }

        [Fact]
        public void Clock_MonotonicNanosecondsAndFallbacks()
        {
            long frequency = 1000000000L;
            long now = 34765500000000L;
            bool fellBack;

            Assert.Equal(34765398714000L, GHEvdevClock.ToTicks(34765, 398714, true, now, frequency, out fellBack));
            Assert.False(fellBack);

            Assert.Equal(now, GHEvdevClock.ToTicks(34765, 600000, true, now, frequency, out fellBack));
            Assert.True(fellBack);
            Assert.Equal(now, GHEvdevClock.ToTicks(34764, 400000, true, now, frequency, out fellBack));
            Assert.True(fellBack);
            Assert.Equal(now, GHEvdevClock.ToTicks(34765, 398714, false, now, frequency, out fellBack));
            Assert.True(fellBack);

            /* Another tick rate */
            Assert.Equal(34765398714L, GHEvdevClock.ToTicks(34765, 398714, true, 34765500000L, 1000000L, out fellBack));
            Assert.False(fellBack);
        }

        private static List<string> Sorted(List<string> calls)
        {
            List<string> sorted = new List<string>(calls);
            sorted.Sort(StringComparer.Ordinal);
            return sorted;
        }
    }
}

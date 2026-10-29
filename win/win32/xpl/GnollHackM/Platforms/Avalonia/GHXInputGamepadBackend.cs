#if GNH_AVALONIA && !LINUX
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using GnollHackX;

namespace GnollHackM
{
    /* Xbox controllers on the Avalonia Windows flavor through xinput1_4.dll, a part of Windows. Connected
       slots are polled every Pump; empty slots are probed one at a time every 2 s, because polling an empty
       slot is slow. Y is inverted (XInput has +Y up). Controller ids 101-104. UI thread only. */
    internal sealed class GHXInputGamepadBackend : IGHGamepadBackend
    {
        private const int SlotCount = 4;
        private const int FirstControllerId = 101;
        private const uint ErrorSuccess = 0;
        private const double EmptySlotProbeSeconds = 2.0;
        private const float InnerDeadzone = 0.24f;

        [StructLayout(LayoutKind.Sequential)]
        private struct XInputGamepad
        {
            public ushort Buttons;
            public byte LeftTrigger;
            public byte RightTrigger;
            public short ThumbLX;
            public short ThumbLY;
            public short ThumbRX;
            public short ThumbRY;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct XInputState
        {
            public uint PacketNumber;
            public XInputGamepad Gamepad;
        }

        [DllImport("xinput1_4.dll", EntryPoint = "XInputGetState")]
        private static extern uint XInputGetState(uint userIndex, out XInputState state);

        /* XINPUT_GAMEPAD wButtons bits, in the order of Map */
        private static readonly ushort[] ButtonBits =
        {
            0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x1000, 0x2000, 0x4000, 0x8000
        };

        private static readonly GHGamepadButton[] Map =
        {
            GHGamepadButton.DPadUp, GHGamepadButton.DPadDown, GHGamepadButton.DPadLeft, GHGamepadButton.DPadRight,
            GHGamepadButton.Menu, GHGamepadButton.View, GHGamepadButton.L3, GHGamepadButton.R3,
            GHGamepadButton.LB, GHGamepadButton.RB, GHGamepadButton.A, GHGamepadButton.B, GHGamepadButton.X, GHGamepadButton.Y
        };

        private readonly bool[] _connected = new bool[SlotCount];
        private readonly uint[] _packet = new uint[SlotCount];
        private readonly GHGamepadStateDiffer[] _differs = new GHGamepadStateDiffer[SlotCount];
        private IGHGamepadInputSink _sink = null;
        private int _nextProbeSlot = 0;
        private long _lastProbeTicks = 0;

        public string Name { get { return "xinput"; } }

        public bool Start(IGHGamepadInputSink sink, out string error)
        {
            error = null;
            if (!OperatingSystem.IsWindows())
            {
                error = "not Windows";
                return false;
            }
            if (sink == null)
            {
                error = "no sink";
                return false;
            }
            _sink = sink;
            try
            {
                long now = Stopwatch.GetTimestamp();
                int slot;
                for (slot = 0; slot < SlotCount; slot++)
                    Probe(slot, now);
                _lastProbeTicks = now;
            }
            catch (Exception ex) when (ex is DllNotFoundException || ex is EntryPointNotFoundException)
            {
                _sink = null;
                error = ex.Message;
                return false;
            }
            return true;
        }

        public void Stop()
        {
            int slot;
            for (slot = 0; slot < SlotCount; slot++)
            {
                if (_connected[slot])
                {
                    _connected[slot] = false;
                    _sink?.ControllerRemoved(FirstControllerId + slot);
                }
            }
            _sink = null;
        }

        public void Pump(long nowTicks)
        {
            if (_sink == null)
                return;
            int slot;
            for (slot = 0; slot < SlotCount; slot++)
            {
                if (!_connected[slot])
                    continue;
                XInputState state;
                if (XInputGetState((uint)slot, out state) != ErrorSuccess)
                {
                    _connected[slot] = false;
                    _sink.ControllerRemoved(FirstControllerId + slot);
                    continue;
                }
                if (state.PacketNumber == _packet[slot])
                    continue;
                _packet[slot] = state.PacketNumber;
                SetState(slot, state);
                _differs[slot].Commit(FirstControllerId + slot, nowTicks, _sink);
            }

            if (nowTicks - _lastProbeTicks >= (long)(EmptySlotProbeSeconds * Stopwatch.Frequency))
            {
                _lastProbeTicks = nowTicks;
                int i;
                for (i = 0; i < SlotCount; i++)
                {
                    int candidate = (_nextProbeSlot + i) % SlotCount;
                    if (_connected[candidate])
                        continue;
                    _nextProbeSlot = (candidate + 1) % SlotCount;
                    Probe(candidate, nowTicks);
                    break;
                }
            }
        }

        /* Reads every connected slot into a silent snapshot */
        public void Resync(long nowTicks)
        {
            if (_sink == null)
                return;
            int slot;
            for (slot = 0; slot < SlotCount; slot++)
            {
                if (!_connected[slot])
                    continue;
                XInputState state;
                if (XInputGetState((uint)slot, out state) != ErrorSuccess)
                    continue;
                _packet[slot] = state.PacketNumber;
                SetState(slot, state);
                _differs[slot].CommitSnapshot(FirstControllerId + slot, nowTicks, _sink, true);
            }
        }

        private void Probe(int slot, long nowTicks)
        {
            XInputState state;
            if (XInputGetState((uint)slot, out state) != ErrorSuccess)
                return;

            _connected[slot] = true;
            _packet[slot] = state.PacketNumber;
            _differs[slot] = new GHGamepadStateDiffer(true);

            GHGamepadControllerInfo info = new GHGamepadControllerInfo();
            info.Id = FirstControllerId + slot;
            info.Name = "XInput controller " + (slot + 1);
            info.BackendName = Name;
            info.Details = "XInput slot " + slot;
            info.Style = GHGamepadStyle.Xbox;
            GHGamepadCapabilities caps = new GHGamepadCapabilities();
            caps.HasAnalogSticks = true;
            caps.HasAnalogTriggers = true;
            caps.HasStickClicks = true;
            info.Capabilities = caps;
            info.InnerDeadzone = InnerDeadzone;
            _sink.ControllerAdded(info);

            SetState(slot, state);
            _differs[slot].CommitSnapshot(info.Id, nowTicks, _sink, true);
        }

        private void SetState(int slot, XInputState state)
        {
            GHGamepadStateDiffer differ = _differs[slot];
            int i;
            for (i = 0; i < Map.Length; i++)
                differ.SetButton(Map[i], (state.Gamepad.Buttons & ButtonBits[i]) != 0);
            differ.SetStick(0, GHGamepadMath.NormalizeInt16(state.Gamepad.ThumbLX), -GHGamepadMath.NormalizeInt16(state.Gamepad.ThumbLY));
            differ.SetStick(1, GHGamepadMath.NormalizeInt16(state.Gamepad.ThumbRX), -GHGamepadMath.NormalizeInt16(state.Gamepad.ThumbRY));
            differ.SetTrigger(false, state.Gamepad.LeftTrigger / 255f);
            differ.SetTrigger(true, state.Gamepad.RightTrigger / 255f);
        }
    }
}
#endif

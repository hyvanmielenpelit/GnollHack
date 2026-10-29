using System;

namespace GnollHackX
{
    /* How an evdev gamepad's codes map to GHGamepadButton and the sticks, from its capabilities and absinfo.
       BTN_X (0x133) is X and BTN_Y (0x134) is Y, as SDL maps non-Sony devices; Sony swaps them. BTN_TL2 and
       BTN_TR2 are LT and RT only without ABS_Z and ABS_RZ. The hat is the D-pad when present, else the
       BTN_DPAD keys are. BTN_MODE is ignored. */
    public sealed class GHEvdevLayout
    {
        public const ushort SonyVendorId = 0x054c;

        private readonly GHEvdevAbsInfo[] _absInfo = new GHEvdevAbsInfo[GHEvdevProtocol.ABS_MAX + 1];
        private readonly bool[] _hasAxis = new bool[GHEvdevProtocol.ABS_MAX + 1];
        private bool _sony;

        public GHGamepadCapabilities Capabilities { get; private set; }
        public bool HasAnalogTriggers { get; private set; }
        public bool UsesHatForDPad { get; private set; }

        private GHEvdevLayout()
        {
        }

        /* absInfo is indexed by axis and may be null or shorter than ABS_MAX + 1 */
        public static GHEvdevLayout Create(byte[] keyBits, byte[] absBits, GHEvdevAbsInfo[] absInfo, ushort vendor)
        {
            GHEvdevLayout layout = new GHEvdevLayout();
            int axis;
            for (axis = 0; axis <= GHEvdevProtocol.ABS_MAX; axis++)
            {
                layout._hasAxis[axis] = GHEvdevProtocol.TestBit(absBits, axis);
                if (absInfo != null && axis < absInfo.Length)
                    layout._absInfo[axis] = absInfo[axis];
            }
            layout._sony = vendor == SonyVendorId;
            layout.Capabilities = GHEvdevDeviceFilter.GetCapabilities(keyBits, absBits);
            layout.HasAnalogTriggers = layout._hasAxis[GHEvdevProtocol.ABS_Z] && layout._hasAxis[GHEvdevProtocol.ABS_RZ];
            layout.UsesHatForDPad = layout._hasAxis[GHEvdevProtocol.ABS_HAT0X] || layout._hasAxis[GHEvdevProtocol.ABS_HAT0Y];
            return layout;
        }

        public bool HasAxis(int axis)
        {
            return axis >= 0 && axis <= GHEvdevProtocol.ABS_MAX && _hasAxis[axis];
        }

        public GHEvdevAbsInfo GetAbsInfo(int axis)
        {
            return HasAxis(axis) ? _absInfo[axis] : new GHEvdevAbsInfo();
        }

        /* The key codes TryMapKey can map, for snapshots */
        public static readonly int[] MappedKeyCodes =
        {
            GHEvdevProtocol.BTN_SOUTH, GHEvdevProtocol.BTN_EAST, GHEvdevProtocol.BTN_X, GHEvdevProtocol.BTN_Y,
            GHEvdevProtocol.BTN_TL, GHEvdevProtocol.BTN_TR, GHEvdevProtocol.BTN_TL2, GHEvdevProtocol.BTN_TR2,
            GHEvdevProtocol.BTN_SELECT, GHEvdevProtocol.BTN_START, GHEvdevProtocol.BTN_THUMBL, GHEvdevProtocol.BTN_THUMBR,
            GHEvdevProtocol.BTN_DPAD_UP, GHEvdevProtocol.BTN_DPAD_DOWN, GHEvdevProtocol.BTN_DPAD_LEFT, GHEvdevProtocol.BTN_DPAD_RIGHT
        };

        public bool TryMapKey(int code, out GHGamepadButton button)
        {
            button = GHGamepadButton.Count;
            switch (code)
            {
            case GHEvdevProtocol.BTN_SOUTH:
                button = GHGamepadButton.A;
                break;
            case GHEvdevProtocol.BTN_EAST:
                button = GHGamepadButton.B;
                break;
            case GHEvdevProtocol.BTN_X:
                button = _sony ? GHGamepadButton.Y : GHGamepadButton.X;
                break;
            case GHEvdevProtocol.BTN_Y:
                button = _sony ? GHGamepadButton.X : GHGamepadButton.Y;
                break;
            case GHEvdevProtocol.BTN_TL:
                button = GHGamepadButton.LB;
                break;
            case GHEvdevProtocol.BTN_TR:
                button = GHGamepadButton.RB;
                break;
            case GHEvdevProtocol.BTN_TL2:
                if (HasAnalogTriggers)
                    return false;
                button = GHGamepadButton.LT;
                break;
            case GHEvdevProtocol.BTN_TR2:
                if (HasAnalogTriggers)
                    return false;
                button = GHGamepadButton.RT;
                break;
            case GHEvdevProtocol.BTN_SELECT:
                button = GHGamepadButton.View;
                break;
            case GHEvdevProtocol.BTN_START:
                button = GHGamepadButton.Menu;
                break;
            case GHEvdevProtocol.BTN_THUMBL:
                button = GHGamepadButton.L3;
                break;
            case GHEvdevProtocol.BTN_THUMBR:
                button = GHGamepadButton.R3;
                break;
            case GHEvdevProtocol.BTN_DPAD_UP:
                button = GHGamepadButton.DPadUp;
                break;
            case GHEvdevProtocol.BTN_DPAD_DOWN:
                button = GHGamepadButton.DPadDown;
                break;
            case GHEvdevProtocol.BTN_DPAD_LEFT:
                button = GHGamepadButton.DPadLeft;
                break;
            case GHEvdevProtocol.BTN_DPAD_RIGHT:
                button = GHGamepadButton.DPadRight;
                break;
            default:
                return false;
            }
            if (button >= GHGamepadButton.DPadUp && button <= GHGamepadButton.DPadRight && UsesHatForDPad)
                return false;
            return true;
        }

        /* -1..1 over the axis range */
        public float NormalizeStickAxis(int axis, int value)
        {
            GHEvdevAbsInfo a = GetAbsInfo(axis);
            return GHGamepadMath.NormalizeAxis(value, a.Minimum, a.Maximum);
        }

        /* 0..1 over the axis range */
        public float NormalizeTriggerAxis(int axis, int value)
        {
            GHEvdevAbsInfo a = GetAbsInfo(axis);
            return GHGamepadMath.NormalizeTrigger(value, a.Minimum, a.Maximum);
        }

        /* -1, 0 or 1: beyond half of the half-range from the middle */
        public int GetHatDirection(int axis, int value)
        {
            GHEvdevAbsInfo a = GetAbsInfo(axis);
            if (a.Maximum <= a.Minimum)
                return 0;
            double center = (a.Minimum + (double)a.Maximum) / 2.0;
            double threshold = (a.Maximum - (double)a.Minimum) / 4.0;
            if (value < center - threshold)
                return -1;
            if (value > center + threshold)
                return 1;
            return 0;
        }

        public static bool IsStickAxis(int axis)
        {
            return axis == GHEvdevProtocol.ABS_X || axis == GHEvdevProtocol.ABS_Y
                || axis == GHEvdevProtocol.ABS_RX || axis == GHEvdevProtocol.ABS_RY;
        }

        /* A stick axis whose value is within deadzone of the middle */
        public bool IsStickAxisAtRest(int axis, int value, float deadzone)
        {
            if (!IsStickAxis(axis) || !HasAxis(axis))
                return false;
            return Math.Abs(NormalizeStickAxis(axis, value)) <= deadzone;
        }
    }

    /* Kernel event timestamps to Stopwatch ticks */
    public static class GHEvdevClock
    {
        /* With a monotonic event clock, the kernel time in ticks; otherwise, or when that falls outside
           [now - 1 s, now], nowTicks with fellBack set */
        public static long ToTicks(long seconds, long microseconds, bool monotonic, long nowTicks, long ticksPerSecond, out bool fellBack)
        {
            fellBack = true;
            if (!monotonic || ticksPerSecond <= 0)
                return nowTicks;
            long ticks;
            if (ticksPerSecond == 1000000000L)
                ticks = seconds * 1000000000L + microseconds * 1000L;
            else
                ticks = seconds * ticksPerSecond + (long)(microseconds * (ticksPerSecond / 1000000.0));
            if (ticks > nowTicks || ticks < nowTicks - ticksPerSecond)
                return nowTicks;
            fellBack = false;
            return ticks;
        }
    }

    /* Event batches of one evdev gamepad to sink calls through GHGamepadStateDiffer. A frame commits at its
       SYN_REPORT. SYN_DROPPED discards the frame and every event up to and including the next SYN_REPORT and
       sets NeedsResync; ApplySnapshot then recovers the state. EV_MSC and unknown codes are ignored. */
    public sealed class GHEvdevDecoder
    {
        private readonly GHEvdevLayout _layout;
        private readonly int _controllerId;
        private readonly GHGamepadStateDiffer _differ;
        private readonly int[] _pendingAbs = new int[GHEvdevProtocol.ABS_MAX + 1];
        private readonly int[] _committedAbs = new int[GHEvdevProtocol.ABS_MAX + 1];
        private bool _dropping = false;

        public bool NeedsResync { get; private set; }

        public GHEvdevLayout Layout { get { return _layout; } }

        public GHEvdevDecoder(GHEvdevLayout layout, int controllerId)
        {
            _layout = layout;
            _controllerId = controllerId;
            _differ = new GHGamepadStateDiffer(layout.HasAnalogTriggers);
            int axis;
            for (axis = 0; axis <= GHEvdevProtocol.ABS_MAX; axis++)
            {
                if (layout.HasAxis(axis))
                    _pendingAbs[axis] = _committedAbs[axis] = layout.GetAbsInfo(axis).Value;
            }
        }

        /* timestamp: the event's time in Stopwatch ticks */
        public void Feed(GHEvdevInputEvent e, long timestamp, IGHGamepadInputSink sink)
        {
            if (_dropping)
            {
                if (e.Type == GHEvdevProtocol.EV_SYN && e.Code == GHEvdevProtocol.SYN_REPORT)
                    _dropping = false;
                return;
            }

            switch (e.Type)
            {
            case GHEvdevProtocol.EV_SYN:
                if (e.Code == GHEvdevProtocol.SYN_REPORT)
                {
                    ApplyAbsToDiffer();
                    _differ.Commit(_controllerId, timestamp, sink);
                    Array.Copy(_pendingAbs, _committedAbs, _pendingAbs.Length);
                }
                else if (e.Code == GHEvdevProtocol.SYN_DROPPED)
                {
                    DiscardPending();
                    _dropping = true;
                    NeedsResync = true;
                }
                break;
            case GHEvdevProtocol.EV_KEY:
                GHGamepadButton button;
                if (_layout.TryMapKey(e.Code, out button))
                    _differ.SetButton(button, e.Value != 0);
                break;
            case GHEvdevProtocol.EV_ABS:
                if (e.Code <= GHEvdevProtocol.ABS_MAX)
                    _pendingAbs[e.Code] = e.Value;
                break;
            default:
                break;
            }
        }

        /* Drops a partly read frame */
        public void DiscardPending()
        {
            _differ.RevertPending();
            Array.Copy(_committedAbs, _pendingAbs, _pendingAbs.Length);
        }

        /* keyState from EVIOCGKEY; absValues indexed by axis, the current values from EVIOCGABS. Silent: held
           buttons give no edges, and every stick and trigger value is emitted. Otherwise the differences from
           the last committed state are emitted as edges. */
        public void ApplySnapshot(byte[] keyState, int[] absValues, long timestamp, IGHGamepadInputSink sink, bool silent)
        {
            _dropping = false;
            NeedsResync = false;
            _differ.RevertPending();
            foreach (int code in GHEvdevLayout.MappedKeyCodes)
            {
                GHGamepadButton button;
                if (_layout.TryMapKey(code, out button))
                    _differ.SetButton(button, GHEvdevProtocol.TestBit(keyState, code));
            }
            int axis;
            for (axis = 0; axis <= GHEvdevProtocol.ABS_MAX; axis++)
            {
                if (_layout.HasAxis(axis) && absValues != null && axis < absValues.Length)
                    _pendingAbs[axis] = absValues[axis];
            }
            ApplyAbsToDiffer();
            _differ.CommitSnapshot(_controllerId, timestamp, sink, silent);
            Array.Copy(_pendingAbs, _committedAbs, _pendingAbs.Length);
        }

        private void ApplyAbsToDiffer()
        {
            if (_layout.HasAxis(GHEvdevProtocol.ABS_X) || _layout.HasAxis(GHEvdevProtocol.ABS_Y))
                _differ.SetStick(0, StickValue(GHEvdevProtocol.ABS_X), StickValue(GHEvdevProtocol.ABS_Y));
            if (_layout.HasAxis(GHEvdevProtocol.ABS_RX) || _layout.HasAxis(GHEvdevProtocol.ABS_RY))
                _differ.SetStick(1, StickValue(GHEvdevProtocol.ABS_RX), StickValue(GHEvdevProtocol.ABS_RY));
            if (_layout.HasAnalogTriggers)
            {
                _differ.SetTrigger(false, _layout.NormalizeTriggerAxis(GHEvdevProtocol.ABS_Z, _pendingAbs[GHEvdevProtocol.ABS_Z]));
                _differ.SetTrigger(true, _layout.NormalizeTriggerAxis(GHEvdevProtocol.ABS_RZ, _pendingAbs[GHEvdevProtocol.ABS_RZ]));
            }
            if (_layout.HasAxis(GHEvdevProtocol.ABS_HAT0X))
            {
                int x = _layout.GetHatDirection(GHEvdevProtocol.ABS_HAT0X, _pendingAbs[GHEvdevProtocol.ABS_HAT0X]);
                _differ.SetButton(GHGamepadButton.DPadLeft, x < 0);
                _differ.SetButton(GHGamepadButton.DPadRight, x > 0);
            }
            if (_layout.HasAxis(GHEvdevProtocol.ABS_HAT0Y))
            {
                int y = _layout.GetHatDirection(GHEvdevProtocol.ABS_HAT0Y, _pendingAbs[GHEvdevProtocol.ABS_HAT0Y]);
                _differ.SetButton(GHGamepadButton.DPadUp, y < 0);
                _differ.SetButton(GHGamepadButton.DPadDown, y > 0);
            }
        }

        private float StickValue(int axis)
        {
            return _layout.HasAxis(axis) ? _layout.NormalizeStickAxis(axis, _pendingAbs[axis]) : 0f;
        }
    }
}

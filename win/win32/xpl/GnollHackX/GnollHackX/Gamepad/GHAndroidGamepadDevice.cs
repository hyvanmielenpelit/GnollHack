using System;
using System.Collections.Generic;

namespace GnollHackX
{
    /* How one Android input device reports its sticks and triggers */
    public sealed class GHAndroidDeviceLayout
    {
        public GHAndroidAxisLayout Axes = GHAndroidAxisLayout.Standard;
        public bool AnalogTriggers;     /* LT and RT come from axes; the L2 and R2 keys are ignored */
        public float ZMin = -1f;        /* the Z and Rz motion ranges, used only by LinuxGamepad */
        public float ZMax = 1f;
        public float RzMin = -1f;
        public float RzMax = 1f;
    }

    /* Every result other than NotMapped means the event belongs to the gamepad */
    public enum GHAndroidKeyResult
    {
        NotMapped,
        Edge,           /* queued for the next Pump, unless the hat already holds that D-pad direction */
        Repeat,         /* a down with a repeat count, or for a button already down; ignored */
        Suppressed,     /* the up of a button held across a Resync; silent */
        UnmatchedUp,    /* an up without a down; ignored */
        Ignored         /* L2/R2 keys of a device with analog triggers */
    }

    /* One joystick MotionEvent's axis values; an axis the device does not have is 0 */
    public struct GHAndroidAxisSample
    {
        public float X;
        public float Y;
        public float Z;
        public float Rz;
        public float Rx;
        public float Ry;
        public float HatX;
        public float HatY;
        public float LTrigger;
        public float RTrigger;
        public float Brake;
        public float Gas;
    }

    /* One Android gamepad: key and motion events arrive between frames and are replayed in Pump. A D-pad
       direction is down when its key or its hat side is down, and an edge is queued only when that merged state
       changes. Resync keeps held buttons silent until released and emits the stored sticks and triggers;
       InputLost reports the sticks and triggers neutral, keeps one that was deflected neutral until it comes
       back inside its release threshold, and releases the hat. UI thread only. */
    public sealed class GHAndroidGamepadDevice
    {
        private const int ButtonCount = (int)GHGamepadButton.LeftStickUp;
        private const int HatSideCount = 4;     /* DPadUp, DPadDown, DPadLeft, DPadRight */

        /* Channels: 0 left stick, 1 right stick, 2 left trigger, 3 right trigger */
        private const int ChannelCount = 4;
        private const int LeftTriggerChannel = 2;
        private const int RightTriggerChannel = 3;

        private struct Edge
        {
            public GHGamepadButton Button;
            public bool Down;
            public long Timestamp;
        }

        private readonly int _controllerId;
        private readonly GHAndroidDeviceLayout _layout;
        private readonly bool[] _keyDown = new bool[ButtonCount];
        private readonly bool[] _hatDown = new bool[HatSideCount];
        private readonly bool[] _suppressed = new bool[ButtonCount];
        private readonly List<Edge> _queue = new List<Edge>();

        /* x and y per channel; a trigger uses x only */
        private readonly float[] _valueX = new float[ChannelCount];
        private readonly float[] _valueY = new float[ChannelCount];
        private readonly float[] _emittedX = new float[ChannelCount];
        private readonly float[] _emittedY = new float[ChannelCount];
        private readonly bool[] _forceEmit = new bool[ChannelCount];
        private readonly bool[] _latched = new bool[ChannelCount];
        private long _axisTimestamp;

        public GHAndroidGamepadDevice(int controllerId, GHAndroidDeviceLayout layout)
        {
            if (layout == null)
                throw new ArgumentNullException("layout");
            _controllerId = controllerId;
            _layout = layout;
        }

        public int ControllerId { get { return _controllerId; } }

        /* A canceled up is passed as an ordinary up */
        public GHAndroidKeyResult OnKey(int keyCode, bool down, int repeatCount, long timestamp)
        {
            GHGamepadButton button;
            if (!GHAndroidGamepadMapping.TryMapKey(keyCode, out button))
                return GHAndroidKeyResult.NotMapped;
            if (_layout.AnalogTriggers && (button == GHGamepadButton.LT || button == GHGamepadButton.RT))
                return GHAndroidKeyResult.Ignored;

            int i = (int)button;
            bool wasMerged = IsMergedDown(i);
            if (down)
            {
                if (repeatCount > 0 || _keyDown[i])
                    return GHAndroidKeyResult.Repeat;
                _keyDown[i] = true;
                if (!wasMerged)
                    Enqueue(button, true, timestamp);
                return GHAndroidKeyResult.Edge;
            }

            if (!_keyDown[i])
                return GHAndroidKeyResult.UnmatchedUp;
            _keyDown[i] = false;
            if (IsMergedDown(i))
                return GHAndroidKeyResult.Edge;
            if (_suppressed[i])
            {
                _suppressed[i] = false;
                return GHAndroidKeyResult.Suppressed;
            }
            Enqueue(button, false, timestamp);
            return GHAndroidKeyResult.Edge;
        }

        public void OnAxes(GHAndroidAxisSample sample, long timestamp)
        {
            _axisTimestamp = timestamp;
            StoreStick(0, sample.X, sample.Y);
            if (_layout.Axes == GHAndroidAxisLayout.Standard)
                StoreStick(1, sample.Z, sample.Rz);
            else
                StoreStick(1, sample.Rx, sample.Ry);

            if (_layout.AnalogTriggers)
            {
                float left;
                float right;
                if (_layout.Axes == GHAndroidAxisLayout.LinuxGamepad)
                {
                    left = GHAndroidGamepadMapping.RescaleTrigger(sample.Z, _layout.ZMin, _layout.ZMax);
                    right = GHAndroidGamepadMapping.RescaleTrigger(sample.Rz, _layout.RzMin, _layout.RzMax);
                }
                else
                {
                    left = GHGamepadMath.Clamp(Math.Max(sample.LTrigger, sample.Brake), 0f, 1f);
                    right = GHGamepadMath.Clamp(Math.Max(sample.RTrigger, sample.Gas), 0f, 1f);
                }
                StoreTrigger(LeftTriggerChannel, left);
                StoreTrigger(RightTriggerChannel, right);
            }

            UpdateHatSide(GHGamepadButton.DPadUp, -sample.HatY, timestamp);
            UpdateHatSide(GHGamepadButton.DPadDown, sample.HatY, timestamp);
            UpdateHatSide(GHGamepadButton.DPadLeft, -sample.HatX, timestamp);
            UpdateHatSide(GHGamepadButton.DPadRight, sample.HatX, timestamp);
        }

        /* The queued edges in order, then each stick and analog trigger whose value changed since it was last
           emitted */
        public void Pump(IGHGamepadInputSink sink)
        {
            if (sink == null)
            {
                _queue.Clear();
                return;
            }
            int i;
            for (i = 0; i < _queue.Count; i++)
                sink.Button(_controllerId, _queue[i].Button, _queue[i].Down, _queue[i].Timestamp);
            _queue.Clear();

            int channel;
            for (channel = 0; channel < ChannelCount; channel++)
            {
                if (IsTrigger(channel) && !_layout.AnalogTriggers)
                    continue;
                if (_forceEmit[channel] || _valueX[channel] != _emittedX[channel] || _valueY[channel] != _emittedY[channel])
                    Emit(sink, channel, _axisTimestamp);
            }
        }

        /* After a stall or a gate change: the stored values are exact. Drops the queue, keeps every held button
           (a D-pad direction held by its key or its hat side included) silent until its up, and emits the
           sticks and triggers as stored (0 for one latched by InputLost). */
        public void Resync(IGHGamepadInputSink sink, long timestamp)
        {
            _queue.Clear();
            int i;
            for (i = 0; i < ButtonCount; i++)
            {
                if (IsMergedDown(i))
                    _suppressed[i] = true;
            }
            if (sink == null)
                return;
            int channel;
            for (channel = 0; channel < ChannelCount; channel++)
            {
                if (IsTrigger(channel) && !_layout.AnalogTriggers)
                    continue;
                Emit(sink, channel, timestamp);
            }
        }

        /* After a pause or a joystick ACTION_CANCEL: the stored values are no longer current. The sticks and
           triggers become 0 and are emitted by the next Pump; one outside its release threshold stays 0 until a
           sample inside it. The hat sides are released, queuing an up for each D-pad direction that goes up,
           except one a Resync left suppressed, whose suppression ends silently. Keys are unchanged. */
        public void InputLost(long timestamp)
        {
            GHGamepadTiming timing = GHGamepadTiming.Default;
            int channel;
            for (channel = 0; channel < ChannelCount; channel++)
            {
                float threshold = IsTrigger(channel) ? timing.TriggerRelease : timing.DirectionRelease;
                if (Magnitude(channel) >= threshold)
                    _latched[channel] = true;
                _valueX[channel] = 0f;
                _valueY[channel] = 0f;
                _forceEmit[channel] = true;
            }
            _axisTimestamp = timestamp;

            int side;
            for (side = 0; side < HatSideCount; side++)
            {
                if (_hatDown[side])
                    SetHatSide((GHGamepadButton)((int)GHGamepadButton.DPadUp + side), false, timestamp);
            }
        }

        /* Forgets all state */
        public void Clear()
        {
            Array.Clear(_keyDown, 0, _keyDown.Length);
            Array.Clear(_hatDown, 0, _hatDown.Length);
            Array.Clear(_suppressed, 0, _suppressed.Length);
            _queue.Clear();
            Array.Clear(_valueX, 0, ChannelCount);
            Array.Clear(_valueY, 0, ChannelCount);
            Array.Clear(_emittedX, 0, ChannelCount);
            Array.Clear(_emittedY, 0, ChannelCount);
            Array.Clear(_forceEmit, 0, ChannelCount);
            Array.Clear(_latched, 0, ChannelCount);
            _axisTimestamp = 0;
        }

        private void StoreStick(int channel, float x, float y)
        {
            if (_latched[channel])
            {
                if (Math.Sqrt((double)x * x + (double)y * y) >= GHGamepadTiming.Default.DirectionRelease)
                    return;
                _latched[channel] = false;
            }
            _valueX[channel] = x;
            _valueY[channel] = y;
        }

        private void StoreTrigger(int channel, float value)
        {
            if (_latched[channel])
            {
                if (value >= GHGamepadTiming.Default.TriggerRelease)
                    return;
                _latched[channel] = false;
            }
            _valueX[channel] = value;
            _valueY[channel] = 0f;
        }

        /* component: the hat axis value along the side's direction */
        private void UpdateHatSide(GHGamepadButton button, float component, long timestamp)
        {
            int side = (int)button - (int)GHGamepadButton.DPadUp;
            bool down = _hatDown[side]
                ? component >= GHAndroidGamepadMapping.HatReleaseThreshold
                : component >= GHAndroidGamepadMapping.HatPressThreshold;
            if (down != _hatDown[side])
                SetHatSide(button, down, timestamp);
        }

        private void SetHatSide(GHGamepadButton button, bool down, long timestamp)
        {
            int i = (int)button;
            bool wasMerged = IsMergedDown(i);
            _hatDown[i - (int)GHGamepadButton.DPadUp] = down;
            bool isMerged = IsMergedDown(i);
            if (isMerged == wasMerged)
                return;
            if (!isMerged && _suppressed[i])
            {
                _suppressed[i] = false;
                return;
            }
            Enqueue(button, isMerged, timestamp);
        }

        private bool IsMergedDown(int button)
        {
            if (_keyDown[button])
                return true;
            int side = button - (int)GHGamepadButton.DPadUp;
            return side >= 0 && side < HatSideCount && _hatDown[side];
        }

        private void Emit(IGHGamepadInputSink sink, int channel, long timestamp)
        {
            if (channel < LeftTriggerChannel)
                sink.Stick(_controllerId, channel, _valueX[channel], _valueY[channel], timestamp);
            else
                sink.Trigger(_controllerId, channel == RightTriggerChannel, _valueX[channel], timestamp);
            _emittedX[channel] = _valueX[channel];
            _emittedY[channel] = _valueY[channel];
            _forceEmit[channel] = false;
        }

        private float Magnitude(int channel)
        {
            if (IsTrigger(channel))
                return _valueX[channel];
            float x = _valueX[channel];
            float y = _valueY[channel];
            return (float)Math.Sqrt((double)x * x + (double)y * y);
        }

        private static bool IsTrigger(int channel)
        {
            return channel >= LeftTriggerChannel;
        }

        private void Enqueue(GHGamepadButton button, bool down, long timestamp)
        {
            Edge e;
            e.Button = button;
            e.Down = down;
            e.Timestamp = timestamp;
            _queue.Add(e);
        }
    }
}

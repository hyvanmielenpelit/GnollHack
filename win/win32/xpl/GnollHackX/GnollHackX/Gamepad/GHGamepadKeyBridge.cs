using System;
using System.Collections.Generic;

namespace GnollHackX
{
    public enum GHGamepadKeyBridgeResult
    {
        NotOwned,
        Edge,           /* queued for the next Pump */
        Repeat,         /* autorepeat KeyDown, ignored */
        Suppressed,     /* held across a Resync; silent until released */
        UnmatchedUp     /* KeyUp without a KeyDown, ignored */
    }

    /* The back grips as keys: an auxiliary backend fed by the window's key handlers. A KeyDown of a key that is
       already down is autorepeat; a KeyUp of a key that is not down is ignored. Resync drops the queued edges
       and keeps every held key silent until its KeyUp. UI thread only. */
    public sealed class GHGamepadKeyBridge : IGHGamepadBackend
    {
        public const int ControllerId = 1000;

        private struct Edge
        {
            public GHGamepadButton Button;
            public bool Down;
            public long Timestamp;
        }

        private readonly int[] _codes;
        private readonly GHGamepadButton[] _buttons;
        private readonly bool[] _down;
        private readonly bool[] _suppressed;
        private readonly List<Edge> _queue = new List<Edge>();
        private IGHGamepadInputSink _sink = null;

        public string Name { get { return "keys"; } }

        /* codes[i] is the key code (the glue uses (int)PhysicalKey) that stands for buttons[i] */
        public GHGamepadKeyBridge(int[] codes, GHGamepadButton[] buttons)
        {
            if (codes == null || buttons == null || codes.Length != buttons.Length)
                throw new ArgumentException("codes and buttons must have the same length");
            _codes = (int[])codes.Clone();
            _buttons = (GHGamepadButton[])buttons.Clone();
            _down = new bool[codes.Length];
            _suppressed = new bool[codes.Length];
        }

        public bool Owns(int code)
        {
            return IndexOf(code) >= 0;
        }

        public GHGamepadKeyBridgeResult OnKey(int code, bool down, long timestamp)
        {
            int i = IndexOf(code);
            if (i < 0)
                return GHGamepadKeyBridgeResult.NotOwned;

            if (down)
            {
                if (_down[i])
                    return GHGamepadKeyBridgeResult.Repeat;
                _down[i] = true;
                Enqueue(i, true, timestamp);
                return GHGamepadKeyBridgeResult.Edge;
            }

            if (!_down[i])
                return GHGamepadKeyBridgeResult.UnmatchedUp;
            _down[i] = false;
            if (_suppressed[i])
            {
                _suppressed[i] = false;
                return GHGamepadKeyBridgeResult.Suppressed;
            }
            Enqueue(i, false, timestamp);
            return GHGamepadKeyBridgeResult.Edge;
        }

        public bool Start(IGHGamepadInputSink sink, out string error)
        {
            error = null;
            if (sink == null)
            {
                error = "no sink";
                return false;
            }
            _sink = sink;
            GHGamepadControllerInfo info = new GHGamepadControllerInfo();
            info.Id = ControllerId;
            info.Name = "Steam Input back grips (keys)";
            info.BackendName = Name;
            info.Details = "grips as keys";
            info.Style = GHGamepadStyle.Generic;
            info.IsAuxiliary = true;
            GHGamepadCapabilities caps = new GHGamepadCapabilities();
            caps.HasGrips = true;
            info.Capabilities = caps;
            sink.ControllerAdded(info);
            return true;
        }

        public void Stop()
        {
            _queue.Clear();
            if (_sink != null)
                _sink.ControllerRemoved(ControllerId);
            _sink = null;
        }

        public void Pump(long nowTicks)
        {
            if (_sink == null)
            {
                _queue.Clear();
                return;
            }
            int i;
            for (i = 0; i < _queue.Count; i++)
                _sink.Button(ControllerId, _queue[i].Button, _queue[i].Down, _queue[i].Timestamp);
            _queue.Clear();
        }

        public void Resync(long nowTicks)
        {
            _queue.Clear();
            int i;
            for (i = 0; i < _down.Length; i++)
            {
                if (_down[i])
                    _suppressed[i] = true;
            }
        }

        private void Enqueue(int index, bool down, long timestamp)
        {
            Edge e;
            e.Button = _buttons[index];
            e.Down = down;
            e.Timestamp = timestamp;
            _queue.Add(e);
        }

        private int IndexOf(int code)
        {
            int i;
            for (i = 0; i < _codes.Length; i++)
            {
                if (_codes[i] == code)
                    return i;
            }
            return -1;
        }
    }
}

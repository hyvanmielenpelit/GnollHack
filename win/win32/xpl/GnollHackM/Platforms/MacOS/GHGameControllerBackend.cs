#if GNH_AVALONIA && MACOS
using System;
using System.Collections.Generic;
using System.Diagnostics;
using GnollHackX;
using N = GnollHackM.GHGameControllerNative;

namespace GnollHackM
{
    /* Controllers with an extended gamepad profile on macOS, through Apple's GameController framework.
       +[GCController controllers] is enumerated at the start of every Pump, so connects and disconnects take effect
       within a frame; each controller is retained while tracked. Every entry point runs in its own autorelease pool.
       Y is inverted (GameController has +Y up). Controller ids 201-299. UI thread only. */
    internal sealed class GHGameControllerBackend : IGHGamepadBackend
    {
        private const int FirstControllerId = 201;
        private const int LastControllerId = 299;
        private const float InnerDeadzone = 0.24f;

        private sealed class Tracked
        {
            public IntPtr Controller;
            public int Id;
            public GHGamepadStateDiffer Differ;
            public bool AnalogTriggers;
        }

        private readonly List<Tracked> _tracked = new List<Tracked>();
        private readonly HashSet<IntPtr> _skipped = new HashSet<IntPtr>();
        private readonly HashSet<IntPtr> _listed = new HashSet<IntPtr>();
        private readonly List<IntPtr> _current = new List<IntPtr>();
        private IGHGamepadInputSink _sink = null;
        private IntPtr _controllerClass = IntPtr.Zero;
        private IntPtr _xboxClass = IntPtr.Zero;
        private IntPtr _dualShockClass = IntPtr.Zero;
        private IntPtr _dualSenseClass = IntPtr.Zero;
        private int _nextId = FirstControllerId;

        public string Name { get { return "gamecontroller"; } }

        public bool Start(IGHGamepadInputSink sink, out string error)
        {
            error = null;
            if (!OperatingSystem.IsMacOS())
            {
                error = "not macOS";
                return false;
            }
            if (sink == null)
            {
                error = "no sink";
                return false;
            }
            try
            {
                if (!N.TryLoadFramework(out error))
                    return false;
                string missing = N.FindMissingSelector();
                if (missing != null)
                {
                    error = "missing selector " + missing;
                    return false;
                }
                _controllerClass = N.objc_getClass("GCController");
                _xboxClass = N.objc_getClass("GCXboxGamepad");
                _dualShockClass = N.objc_getClass("GCDualShockGamepad");
                _dualSenseClass = N.objc_getClass("GCDualSenseGamepad");
                _sink = sink;

                IntPtr pool = N.objc_autoreleasePoolPush();
                try
                {
                    Enumerate(Stopwatch.GetTimestamp());
                }
                finally
                {
                    N.objc_autoreleasePoolPop(pool);
                }
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
            IntPtr pool = N.objc_autoreleasePoolPush();
            try
            {
                foreach (Tracked t in _tracked)
                {
                    _sink?.ControllerRemoved(t.Id);
                    N.objc_release(t.Controller);
                }
                _tracked.Clear();
                _skipped.Clear();
                _sink = null;
            }
            finally
            {
                N.objc_autoreleasePoolPop(pool);
            }
        }

        public void Pump(long nowTicks)
        {
            if (_sink == null)
                return;
            IntPtr pool = N.objc_autoreleasePoolPush();
            try
            {
                Enumerate(nowTicks);
                foreach (Tracked t in _tracked)
                {
                    IntPtr pad = N.SendIntPtr(t.Controller, N.Sel.ExtendedGamepad);
                    if (pad == IntPtr.Zero)
                        continue;
                    ReadState(t, pad);
                    t.Differ.Commit(t.Id, nowTicks, _sink);
                }
            }
            finally
            {
                N.objc_autoreleasePoolPop(pool);
            }
        }

        /* Reads every tracked controller into a silent snapshot */
        public void Resync(long nowTicks)
        {
            if (_sink == null)
                return;
            IntPtr pool = N.objc_autoreleasePoolPush();
            try
            {
                foreach (Tracked t in _tracked)
                {
                    IntPtr pad = N.SendIntPtr(t.Controller, N.Sel.ExtendedGamepad);
                    if (pad == IntPtr.Zero)
                        continue;
                    ReadState(t, pad);
                    t.Differ.CommitSnapshot(t.Id, nowTicks, _sink, true);
                }
            }
            finally
            {
                N.objc_autoreleasePoolPop(pool);
            }
        }

        /* Inside an autorelease pool */
        private void Enumerate(long nowTicks)
        {
            _current.Clear();
            _listed.Clear();
            IntPtr array = N.SendIntPtr(_controllerClass, N.Sel.Controllers);
            nuint count = array != IntPtr.Zero ? N.SendNUInt(array, N.Sel.Count) : 0;
            nuint index;
            for (index = 0; index < count; index++)
            {
                IntPtr controller = N.SendIntPtrNUInt(array, N.Sel.ObjectAtIndex, index);
                if (controller == IntPtr.Zero)
                    continue;
                _current.Add(controller);
                _listed.Add(controller);
            }

            int i;
            for (i = _tracked.Count - 1; i >= 0; i--)
            {
                Tracked t = _tracked[i];
                if (_listed.Contains(t.Controller))
                    continue;
                _tracked.RemoveAt(i);
                _sink.ControllerRemoved(t.Id);
                N.objc_release(t.Controller);
                WriteLog("GAMEPAD gc -disconnect id=" + t.Id);
            }

            foreach (IntPtr controller in _current)
            {
                if (IsTracked(controller))
                    continue;
                IntPtr pad = N.SendIntPtr(controller, N.Sel.ExtendedGamepad);
                if (pad == IntPtr.Zero)
                {
                    if (_skipped.Add(controller))
                    {
                        WriteLog("GAMEPAD gc skip \"" + N.NSStringToString(N.SendIntPtr(controller, N.Sel.VendorName)) + "\" category=\""
                            + N.NSStringToString(N.SendIntPtr(controller, N.Sel.ProductCategory)) + "\": no extended gamepad profile");
                    }
                    continue;
                }
                _skipped.Remove(controller);
                Add(controller, pad, nowTicks);
            }

            _skipped.RemoveWhere(p => !_listed.Contains(p));
        }

        private void Add(IntPtr controller, IntPtr pad, long nowTicks)
        {
            string vendorName = N.NSStringToString(N.SendIntPtr(controller, N.Sel.VendorName));
            string category = N.NSStringToString(N.SendIntPtr(controller, N.Sel.ProductCategory));
            string profileClass = N.GetClassName(pad);
            bool analogTriggers = IsAnalog(pad, N.Sel.LeftTrigger) && IsAnalog(pad, N.Sel.RightTrigger);
            bool stickClicks = N.SendIntPtr(pad, N.Sel.LeftThumbstickButton) != IntPtr.Zero
                && N.SendIntPtr(pad, N.Sel.RightThumbstickButton) != IntPtr.Zero;

            Tracked t = new Tracked();
            t.Controller = N.objc_retain(controller);
            t.Id = AllocateControllerId();
            t.AnalogTriggers = analogTriggers;
            t.Differ = new GHGamepadStateDiffer(analogTriggers);
            _tracked.Add(t);

            GHGamepadControllerInfo info = new GHGamepadControllerInfo();
            info.Id = t.Id;
            info.Name = string.IsNullOrEmpty(vendorName) ? "Game controller" : vendorName;
            info.BackendName = Name;
            info.Details = "GameController " + category + " " + profileClass;
            info.Style = GetStyle(pad, category);
            GHGamepadCapabilities caps = new GHGamepadCapabilities();
            caps.HasAnalogSticks = true;
            caps.HasAnalogTriggers = analogTriggers;
            caps.HasStickClicks = stickClicks;
            info.Capabilities = caps;
            info.IsAuxiliary = false;
            info.InnerDeadzone = InnerDeadzone;
            _sink.ControllerAdded(info);

            ReadState(t, pad);
            t.Differ.CommitSnapshot(t.Id, nowTicks, _sink, true);

            WriteLog("GAMEPAD gc +connect id=" + t.Id + " name=\"" + info.Name + "\" category=\"" + category + "\" profile="
                + profileClass + " style=" + info.Style + " analogTriggers=" + (analogTriggers ? 1 : 0)
                + " stickClicks=" + (stickClicks ? 1 : 0));
        }

        private bool IsTracked(IntPtr controller)
        {
            foreach (Tracked t in _tracked)
            {
                if (t.Controller == controller)
                    return true;
            }
            return false;
        }

        private int AllocateControllerId()
        {
            int span = LastControllerId - FirstControllerId + 1;
            int attempt;
            for (attempt = 0; attempt < span; attempt++)
            {
                int id = _nextId;
                _nextId = id >= LastControllerId ? FirstControllerId : id + 1;
                if (!IsIdInUse(id))
                    return id;
            }
            return _nextId;
        }

        private bool IsIdInUse(int id)
        {
            foreach (Tracked t in _tracked)
            {
                if (t.Id == id)
                    return true;
            }
            return false;
        }

        private GHGamepadStyle GetStyle(IntPtr pad, string category)
        {
            if (N.IsKindOf(pad, _dualSenseClass) || N.IsKindOf(pad, _dualShockClass))
                return GHGamepadStyle.PlayStation;
            if (N.IsKindOf(pad, _xboxClass))
                return GHGamepadStyle.Xbox;
            if (string.IsNullOrEmpty(category))
                return GHGamepadStyle.Generic;
            if (Contains(category, "Xbox"))
                return GHGamepadStyle.Xbox;
            if (Contains(category, "DualShock") || Contains(category, "DualSense") || Contains(category, "PlayStation"))
                return GHGamepadStyle.PlayStation;
            if (Contains(category, "Switch") || Contains(category, "Joy-Con") || Contains(category, "Nintendo"))
                return GHGamepadStyle.Nintendo;
            return GHGamepadStyle.Generic;
        }

        private static bool Contains(string text, string part)
        {
            return text.IndexOf(part, StringComparison.OrdinalIgnoreCase) >= 0;
        }

        private static void ReadState(Tracked t, IntPtr pad)
        {
            GHGamepadStateDiffer differ = t.Differ;
            differ.SetButton(GHGamepadButton.A, IsPressed(pad, N.Sel.ButtonA));
            differ.SetButton(GHGamepadButton.B, IsPressed(pad, N.Sel.ButtonB));
            differ.SetButton(GHGamepadButton.X, IsPressed(pad, N.Sel.ButtonX));
            differ.SetButton(GHGamepadButton.Y, IsPressed(pad, N.Sel.ButtonY));
            differ.SetButton(GHGamepadButton.LB, IsPressed(pad, N.Sel.LeftShoulder));
            differ.SetButton(GHGamepadButton.RB, IsPressed(pad, N.Sel.RightShoulder));
            differ.SetButton(GHGamepadButton.View, IsPressed(pad, N.Sel.ButtonOptions));
            differ.SetButton(GHGamepadButton.Menu, IsPressed(pad, N.Sel.ButtonMenu));
            differ.SetButton(GHGamepadButton.L3, IsPressed(pad, N.Sel.LeftThumbstickButton));
            differ.SetButton(GHGamepadButton.R3, IsPressed(pad, N.Sel.RightThumbstickButton));

            IntPtr dpad = N.SendIntPtr(pad, N.Sel.Dpad);
            differ.SetButton(GHGamepadButton.DPadUp, IsPressed(dpad, N.Sel.Up));
            differ.SetButton(GHGamepadButton.DPadDown, IsPressed(dpad, N.Sel.Down));
            differ.SetButton(GHGamepadButton.DPadLeft, IsPressed(dpad, N.Sel.Left));
            differ.SetButton(GHGamepadButton.DPadRight, IsPressed(dpad, N.Sel.Right));

            IntPtr leftStick = N.SendIntPtr(pad, N.Sel.LeftThumbstick);
            differ.SetStick(0, AxisValue(leftStick, N.Sel.XAxis), -AxisValue(leftStick, N.Sel.YAxis));
            IntPtr rightStick = N.SendIntPtr(pad, N.Sel.RightThumbstick);
            differ.SetStick(1, AxisValue(rightStick, N.Sel.XAxis), -AxisValue(rightStick, N.Sel.YAxis));

            if (t.AnalogTriggers)
            {
                differ.SetTrigger(false, ButtonValue(pad, N.Sel.LeftTrigger));
                differ.SetTrigger(true, ButtonValue(pad, N.Sel.RightTrigger));
            }
            else
            {
                differ.SetButton(GHGamepadButton.LT, IsPressed(pad, N.Sel.LeftTrigger));
                differ.SetButton(GHGamepadButton.RT, IsPressed(pad, N.Sel.RightTrigger));
            }
        }

        /* A missing optional element reads as released */
        private static bool IsPressed(IntPtr parent, IntPtr elementSelector)
        {
            if (parent == IntPtr.Zero)
                return false;
            IntPtr element = N.SendIntPtr(parent, elementSelector);
            return element != IntPtr.Zero && N.SendByte(element, N.Sel.IsPressed) != 0;
        }

        private static float ButtonValue(IntPtr parent, IntPtr elementSelector)
        {
            IntPtr element = N.SendIntPtr(parent, elementSelector);
            return element != IntPtr.Zero ? N.SendFloat(element, N.Sel.Value) : 0f;
        }

        private static float AxisValue(IntPtr directionPad, IntPtr axisSelector)
        {
            if (directionPad == IntPtr.Zero)
                return 0f;
            IntPtr axis = N.SendIntPtr(directionPad, axisSelector);
            return axis != IntPtr.Zero ? N.SendFloat(axis, N.Sel.Value) : 0f;
        }

        private static bool IsAnalog(IntPtr pad, IntPtr elementSelector)
        {
            IntPtr element = N.SendIntPtr(pad, elementSelector);
            return element != IntPtr.Zero && N.SendByte(element, N.Sel.IsAnalog) != 0;
        }

        private static void WriteLog(string line)
        {
            try
            {
                Console.Error.WriteLine(line);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            GHApp.MaybeWriteGHLog(line);
        }
    }
}
#endif

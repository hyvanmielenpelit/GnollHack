using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using Foundation;
using GameController;
using GnollHackX;
using ObjCRuntime;

namespace GnollHackM;

/* Controllers with an extended gamepad profile on iOS, through Apple's GameController framework. The connect and
   disconnect notifications only queue the controller; Pump applies the queue first, so ControllerAdded and
   ControllerRemoved reach the sink at the start of a Poll as on the other backends. A tracked controller's elements
   are resolved once and read on every Pump. Y is inverted (GameController has +Y up). While Use Controller is on,
   the system gestures of the mapped Menu and Options buttons are disabled. The Home button is not read. Controller
   ids 401-499. UI thread only, except the notification queue. */
internal sealed class GHiOSGamepadBackend : IGHGamepadBackend
{
    private const int FirstControllerId = 401;
    private const int LastControllerId = 499;

    private struct Change
    {
        public GCController Controller;
        public bool Added;
    }

    private sealed class Tracked
    {
        public GCController Controller;
        public int Id;
        public GHGamepadStateDiffer Differ;
        public bool AnalogTriggers;
        public GCControllerButtonInput A, B, X, Y;
        public GCControllerButtonInput LeftShoulder, RightShoulder, LeftTrigger, RightTrigger;
        public GCControllerButtonInput Menu, Options, LeftStickButton, RightStickButton;
        public GCControllerButtonInput Up, Down, Left, Right;
        public GCControllerAxisInput LeftX, LeftY, RightX, RightY;
    }

    private readonly List<Tracked> _tracked = new List<Tracked>();
    private readonly List<Change> _changes = new List<Change>();
    private readonly HashSet<IntPtr> _skipped = new HashSet<IntPtr>();
    private IGHGamepadInputSink _sink = null;
    private NSObject _connectObserver = null;
    private NSObject _disconnectObserver = null;
    private bool _gesturesDisabled = false;
    private int _nextId = FirstControllerId;

    public string Name { get { return "ios"; } }

    public bool HasControllers { get { return _tracked.Count > 0; } }

    public bool Start(IGHGamepadInputSink sink, out string error)
    {
        error = null;
        if (sink == null)
        {
            error = "no sink";
            return false;
        }
        try
        {
            _sink = sink;
            NSNotificationCenter center = NSNotificationCenter.DefaultCenter;
            _connectObserver = center.AddObserver(GCController.DidConnectNotification, OnDidConnect);
            _disconnectObserver = center.AddObserver(GCController.DidDisconnectNotification, OnDidDisconnect);

            GCController[] controllers = GCController.Controllers;
            if (controllers != null)
            {
                foreach (GCController controller in controllers)
                    QueueChange(controller, true);
            }
        }
        catch (Exception ex)
        {
            Unregister();
            _sink = null;
            lock (_changes)
            {
                _changes.Clear();
            }
            error = ex.GetType().Name + ": " + ex.Message;
            return false;
        }
        int present;
        lock (_changes)
        {
            present = _changes.Count;
        }
        WriteLog("GAMEPAD ios started controllers=" + present);
        return true;
    }

    public void Stop()
    {
        Unregister();
        foreach (Tracked t in _tracked)
        {
            SetSystemGestures(t, false);
            _sink?.ControllerRemoved(t.Id);
        }
        _tracked.Clear();
        _skipped.Clear();
        lock (_changes)
        {
            _changes.Clear();
        }
        _gesturesDisabled = false;
        _sink = null;
    }

    public void Pump(long nowTicks)
    {
        if (_sink == null)
            return;
        ApplyChanges(nowTicks);

        bool disableGestures = GHGamepadService.IsEnabled;
        if (disableGestures != _gesturesDisabled)
        {
            _gesturesDisabled = disableGestures;
            foreach (Tracked t in _tracked)
                SetSystemGestures(t, disableGestures);
        }

        foreach (Tracked t in _tracked)
        {
            ReadState(t);
            t.Differ.Commit(t.Id, nowTicks, _sink);
        }
    }

    /* Reads every tracked controller into a silent snapshot */
    public void Resync(long nowTicks)
    {
        if (_sink == null)
            return;
        foreach (Tracked t in _tracked)
        {
            ReadState(t);
            t.Differ.CommitSnapshot(t.Id, nowTicks, _sink, true);
        }
    }

    private void OnDidConnect(NSNotification notification)
    {
        QueueChange(notification?.Object as GCController, true);
    }

    private void OnDidDisconnect(NSNotification notification)
    {
        QueueChange(notification?.Object as GCController, false);
    }

    /* Called from native code: nothing may escape */
    private void QueueChange(GCController controller, bool added)
    {
        try
        {
            if (controller == null)
                return;
            Change change;
            change.Controller = controller;
            change.Added = added;
            lock (_changes)
            {
                _changes.Add(change);
            }
        }
        catch (Exception ex)
        {
            Debug.WriteLine(ex.Message);
        }
    }

    private void Unregister()
    {
        try
        {
            NSNotificationCenter center = NSNotificationCenter.DefaultCenter;
            if (_connectObserver != null)
                center.RemoveObserver(_connectObserver);
            if (_disconnectObserver != null)
                center.RemoveObserver(_disconnectObserver);
        }
        catch (Exception ex)
        {
            WriteLog("GAMEPAD ios removing the controller observers failed: " + ex.Message);
        }
        _connectObserver = null;
        _disconnectObserver = null;
    }

    private void ApplyChanges(long nowTicks)
    {
        Change[] changes;
        lock (_changes)
        {
            if (_changes.Count == 0)
                return;
            changes = _changes.ToArray();
            _changes.Clear();
        }
        foreach (Change change in changes)
        {
            if (change.Added)
                Add(change.Controller, nowTicks);
            else
                Remove(change.Controller);
        }
    }

    private void Add(GCController controller, long nowTicks)
    {
        IntPtr handle = controller.Handle;
        if (Find(handle) != null)
            return;

        string vendorName = controller.VendorName;
        string category = controller.ProductCategory;
        GCExtendedGamepad pad = controller.ExtendedGamepad;
        if (pad == null)
        {
            if (_skipped.Add(handle))
                WriteLog("GAMEPAD ios skip \"" + vendorName + "\" category=\"" + category + "\": no extended gamepad profile");
            return;
        }
        _skipped.Remove(handle);

        Tracked t = new Tracked();
        t.Controller = controller;
        t.Id = AllocateControllerId();
        t.A = pad.ButtonA;
        t.B = pad.ButtonB;
        t.X = pad.ButtonX;
        t.Y = pad.ButtonY;
        t.LeftShoulder = pad.LeftShoulder;
        t.RightShoulder = pad.RightShoulder;
        t.LeftTrigger = pad.LeftTrigger;
        t.RightTrigger = pad.RightTrigger;
        t.Menu = pad.ButtonMenu;
        t.Options = pad.ButtonOptions;
        t.LeftStickButton = pad.LeftThumbstickButton;
        t.RightStickButton = pad.RightThumbstickButton;
        GCControllerDirectionPad dpad = pad.DPad;
        if (dpad != null)
        {
            t.Up = dpad.Up;
            t.Down = dpad.Down;
            t.Left = dpad.Left;
            t.Right = dpad.Right;
        }
        GCControllerDirectionPad leftStick = pad.LeftThumbstick;
        if (leftStick != null)
        {
            t.LeftX = leftStick.XAxis;
            t.LeftY = leftStick.YAxis;
        }
        GCControllerDirectionPad rightStick = pad.RightThumbstick;
        if (rightStick != null)
        {
            t.RightX = rightStick.XAxis;
            t.RightY = rightStick.YAxis;
        }
        t.AnalogTriggers = t.LeftTrigger != null && t.RightTrigger != null && t.LeftTrigger.IsAnalog && t.RightTrigger.IsAnalog;
        t.Differ = new GHGamepadStateDiffer(t.AnalogTriggers);
        _tracked.Add(t);

        bool stickClicks = t.LeftStickButton != null && t.RightStickButton != null;
        string profileClass = pad.Class != null ? pad.Class.Name : null;

        GHGamepadControllerInfo info = new GHGamepadControllerInfo();
        info.Id = t.Id;
        info.Name = string.IsNullOrEmpty(vendorName) ? "Game controller" : vendorName;
        info.BackendName = Name;
        info.Details = "GameController " + category + " " + profileClass;
        info.Style = GHAppleGamepadMapping.GetStyle(GetProfile(pad), category, vendorName);
        GHGamepadCapabilities caps = new GHGamepadCapabilities();
        caps.HasAnalogSticks = true;
        caps.HasAnalogTriggers = t.AnalogTriggers;
        caps.HasStickClicks = stickClicks;
        caps.HasGrips = false;
        info.Capabilities = caps;
        info.IsAuxiliary = false;
        info.InnerDeadzone = GHAppleGamepadMapping.InnerDeadzone;
        _sink.ControllerAdded(info);

        if (_gesturesDisabled)
            SetSystemGestures(t, true);
        ReadState(t);
        t.Differ.CommitSnapshot(t.Id, nowTicks, _sink, true);

        GCControllerButtonInput home = pad.ButtonHome;
        StringBuilder sb = new StringBuilder();
        sb.Append("GAMEPAD ios +connect id=").Append(t.Id).Append(" name=\"").Append(info.Name).Append("\" category=\"")
            .Append(category).Append("\" profile=").Append(profileClass).Append(" style=").Append(info.Style)
            .Append(" analogTriggers=").Append(t.AnalogTriggers ? 1 : 0).Append(" stickClicks=").Append(stickClicks ? 1 : 0)
            .Append(" options=").Append(t.Options != null ? 1 : 0).Append(" home=").Append(home != null ? 1 : 0)
            .Append(" gestures=menu:").Append(IsBound(t.Menu) ? 1 : 0).Append(",options:").Append(IsBound(t.Options) ? 1 : 0)
            .Append(",home:").Append(IsBound(home) ? 1 : 0)
            .Append(" a=\"").Append(t.A != null ? t.A.SfSymbolsName : null).Append('"');
        WriteLog(sb.ToString());
    }

    private void Remove(GCController controller)
    {
        IntPtr handle = controller.Handle;
        _skipped.Remove(handle);
        int i;
        for (i = _tracked.Count - 1; i >= 0; i--)
        {
            Tracked t = _tracked[i];
            if ((IntPtr)t.Controller.Handle != handle)
                continue;
            _tracked.RemoveAt(i);
            SetSystemGestures(t, false);
            _sink?.ControllerRemoved(t.Id);
            WriteLog("GAMEPAD ios -disconnect id=" + t.Id);
        }
    }

    private Tracked Find(IntPtr handle)
    {
        foreach (Tracked t in _tracked)
        {
            if ((IntPtr)t.Controller.Handle == handle)
                return t;
        }
        return null;
    }

    private static GHAppleGamepadProfile GetProfile(GCExtendedGamepad pad)
    {
        if (IsKind(pad, typeof(GCDualSenseGamepad)))
            return GHAppleGamepadProfile.DualSense;
        if (IsKind(pad, typeof(GCDualShockGamepad)))
            return GHAppleGamepadProfile.DualShock;
        if (IsKind(pad, typeof(GCXboxGamepad)))
            return GHAppleGamepadProfile.Xbox;
        return GHAppleGamepadProfile.Extended;
    }

    private static bool IsKind(NSObject obj, Type type)
    {
        try
        {
            return obj.IsKindOfClass(new Class(type));
        }
        catch (Exception ex)
        {
            Debug.WriteLine(ex.Message);
            return false;
        }
    }

    private static void ReadState(Tracked t)
    {
        GHGamepadStateDiffer differ = t.Differ;
        differ.SetButton(GHGamepadButton.A, IsPressed(t.A));
        differ.SetButton(GHGamepadButton.B, IsPressed(t.B));
        differ.SetButton(GHGamepadButton.X, IsPressed(t.X));
        differ.SetButton(GHGamepadButton.Y, IsPressed(t.Y));
        differ.SetButton(GHGamepadButton.LB, IsPressed(t.LeftShoulder));
        differ.SetButton(GHGamepadButton.RB, IsPressed(t.RightShoulder));
        differ.SetButton(GHGamepadButton.View, IsPressed(t.Options));
        differ.SetButton(GHGamepadButton.Menu, IsPressed(t.Menu));
        differ.SetButton(GHGamepadButton.L3, IsPressed(t.LeftStickButton));
        differ.SetButton(GHGamepadButton.R3, IsPressed(t.RightStickButton));
        differ.SetButton(GHGamepadButton.DPadUp, IsPressed(t.Up));
        differ.SetButton(GHGamepadButton.DPadDown, IsPressed(t.Down));
        differ.SetButton(GHGamepadButton.DPadLeft, IsPressed(t.Left));
        differ.SetButton(GHGamepadButton.DPadRight, IsPressed(t.Right));

        differ.SetStick(0, AxisValue(t.LeftX), -AxisValue(t.LeftY));
        differ.SetStick(1, AxisValue(t.RightX), -AxisValue(t.RightY));

        if (t.AnalogTriggers)
        {
            differ.SetTrigger(false, t.LeftTrigger.Value);
            differ.SetTrigger(true, t.RightTrigger.Value);
        }
        else
        {
            differ.SetButton(GHGamepadButton.LT, IsPressed(t.LeftTrigger));
            differ.SetButton(GHGamepadButton.RT, IsPressed(t.RightTrigger));
        }
    }

    /* A missing optional element reads as released */
    private static bool IsPressed(GCControllerButtonInput button)
    {
        return button != null && button.IsPressed;
    }

    private static float AxisValue(GCControllerAxisInput axis)
    {
        return axis != null ? axis.Value : 0f;
    }

    private static bool IsBound(GCControllerElement element)
    {
        return element != null && element.IsBoundToSystemGesture;
    }

    private static void SetSystemGestures(Tracked t, bool disabled)
    {
        SetSystemGesture(t.Menu, disabled);
        SetSystemGesture(t.Options, disabled);
    }

    private static void SetSystemGesture(GCControllerElement element, bool disabled)
    {
        if (element == null)
            return;
        try
        {
            GCSystemGestureState state = disabled ? GCSystemGestureState.Disabled : GCSystemGestureState.Enabled;
            if (element.PreferredSystemGestureState != state)
                element.PreferredSystemGestureState = state;
        }
        catch (Exception ex)
        {
            Debug.WriteLine(ex.Message);
        }
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

    public static void WriteLog(string line)
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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Text;
using Android.Hardware.Input;
using Android.Views;
using GnollHackX;

namespace GnollHackM;

/* Game controllers on Android, from the key and joystick motion events that GHAndroidGamepad passes on and from
   InputManager's device callbacks. The callbacks only queue the device id; Pump applies the queue first, so
   ControllerAdded and ControllerRemoved reach the sink at the start of a Poll as on the other backends. Events
   are stored by each device and replayed by its Pump. Fallback keys, which Android sends for controller keys the
   app did not consume, are never mapped. Controller ids 301-399. UI thread only. */
internal sealed class GHAndroidGamepadBackend : Java.Lang.Object, IGHGamepadBackend, InputManager.IInputDeviceListener
{
    private const int FirstControllerId = 301;
    private const int LastControllerId = 399;

    /* The axes of GHAndroidAxisSample */
    private static readonly Axis[] SampleAxes =
    {
        Axis.X, Axis.Y, Axis.Z, Axis.Rz, Axis.Rx, Axis.Ry, Axis.HatX, Axis.HatY, Axis.Ltrigger, Axis.Rtrigger, Axis.Brake, Axis.Gas
    };

    private enum ChangeKind
    {
        Added,
        Changed,
        Removed
    }

    private struct DeviceChange
    {
        public int DeviceId;
        public ChangeKind Kind;
    }

    /* What an eligible device reports, read once when it is added or changed */
    private sealed class Description
    {
        public GHAndroidDeviceLayout Layout;
        public Axis[] Axes;                     /* the sample axes that have a motion range */
        public InputDevice.MotionRange[] Ranges;
        public bool HasSticks;
        public bool HasStickClicks;
        public float InnerDeadzone;
    }

    private sealed class Tracked
    {
        public int DeviceId;
        public int Id;
        public GHAndroidGamepadDevice Device;
        public Description Description;
        public readonly HashSet<int> UnmappedKeys = new HashSet<int>();
    }

    private readonly List<Tracked> _tracked = new List<Tracked>();
    private readonly List<DeviceChange> _changes = new List<DeviceChange>();
    private readonly HashSet<int> _skipped = new HashSet<int>();
    private IGHGamepadInputSink _sink = null;
    private InputManager _inputManager = null;
    private int _nextId = FirstControllerId;

    public string Name { get { return "android"; } }

    public bool Start(IGHGamepadInputSink sink, out string error)
    {
        error = null;
        if (sink == null)
        {
            error = "no sink";
            return false;
        }
        error = FindConstantMismatch();
        if (error != null)
            return false;
        try
        {
            InputManager manager = global::Android.App.Application.Context.GetSystemService(global::Android.Content.Context.InputService) as InputManager;
            if (manager == null)
            {
                error = "no InputManager";
                return false;
            }
            manager.RegisterInputDeviceListener(this, new global::Android.OS.Handler(global::Android.OS.Looper.MainLooper));
            _inputManager = manager;
            _sink = sink;

            int[] ids = InputDevice.GetDeviceIds();
            if (ids != null)
            {
                foreach (int id in ids)
                    QueueChange(id, ChangeKind.Added);
            }
        }
        catch (Exception ex)
        {
            Unregister();
            _sink = null;
            _changes.Clear();
            error = ex.GetType().Name + ": " + ex.Message;
            return false;
        }
        return true;
    }

    public void Stop()
    {
        Unregister();
        foreach (Tracked t in _tracked)
            _sink?.ControllerRemoved(t.Id);
        _tracked.Clear();
        _changes.Clear();
        _skipped.Clear();
        _sink = null;
    }

    public void Pump(long nowTicks)
    {
        if (_sink == null)
            return;
        ApplyChanges();
        foreach (Tracked t in _tracked)
            t.Device.Pump(_sink);
    }

    public void Resync(long nowTicks)
    {
        if (_sink == null)
            return;
        foreach (Tracked t in _tracked)
            t.Device.Resync(_sink, nowTicks);
    }

    public bool IsTrackedDevice(int deviceId)
    {
        return Find(deviceId) != null;
    }

    /* True when the event belongs to a controller and Use Controller is on */
    public bool HandleKey(KeyEvent e)
    {
        if (e == null)
            return false;
        Tracked t = Find(e.DeviceId);
        if (t == null)
            return false;
        if ((e.Flags & KeyEventFlags.Fallback) != 0)
            return false;
        KeyEventActions action = e.Action;
        if (action != KeyEventActions.Down && action != KeyEventActions.Up)
            return false;

        int keyCode = (int)e.KeyCode;
        GHAndroidKeyResult result = t.Device.OnKey(keyCode, action == KeyEventActions.Down, e.RepeatCount, Stopwatch.GetTimestamp());
        if (result == GHAndroidKeyResult.NotMapped)
        {
            if (t.UnmappedKeys.Add(keyCode))
                WriteLog("GAMEPAD android unmapped key=" + keyCode + " device=" + t.DeviceId);
            return false;
        }
        return GHGamepadService.IsEnabled;
    }

    /* True when the event is a controller's joystick motion and Use Controller is on */
    public bool HandleMotion(MotionEvent e)
    {
        if (e == null || !e.IsFromSource(InputSourceType.Joystick))
            return false;
        Tracked t = Find(e.DeviceId);
        if (t == null)
            return false;

        MotionEventActions action = e.ActionMasked;
        long now = Stopwatch.GetTimestamp();
        if (action == MotionEventActions.Cancel)
        {
            t.Device.InputLost(now);
            return false;
        }
        if (action != MotionEventActions.Move)
            return false;

        Axis[] axes = t.Description.Axes;
        int historySize = e.HistorySize;
        int i;
        for (i = 0; i < historySize; i++)
            t.Device.OnAxes(ReadSample(e, axes, i), now);
        t.Device.OnAxes(ReadSample(e, axes, -1), now);
        return GHGamepadService.IsEnabled;
    }

    /* The window lost its input: the stored sticks, triggers and hat are no longer current */
    public void InputLost(long timestamp)
    {
        foreach (Tracked t in _tracked)
            t.Device.InputLost(timestamp);
    }

    public void OnInputDeviceAdded(int deviceId)
    {
        QueueChange(deviceId, ChangeKind.Added);
    }

    public void OnInputDeviceChanged(int deviceId)
    {
        QueueChange(deviceId, ChangeKind.Changed);
    }

    public void OnInputDeviceRemoved(int deviceId)
    {
        QueueChange(deviceId, ChangeKind.Removed);
    }

    /* Called from Java: nothing may escape */
    private void QueueChange(int deviceId, ChangeKind kind)
    {
        try
        {
            DeviceChange change;
            change.DeviceId = deviceId;
            change.Kind = kind;
            _changes.Add(change);
        }
        catch (Exception ex)
        {
            Debug.WriteLine(ex.Message);
        }
    }

    private void Unregister()
    {
        InputManager manager = _inputManager;
        _inputManager = null;
        if (manager == null)
            return;
        try
        {
            manager.UnregisterInputDeviceListener(this);
        }
        catch (Exception ex)
        {
            WriteLog("GAMEPAD android unregistering the device listener failed: " + ex.Message);
        }
    }

    private void ApplyChanges()
    {
        if (_changes.Count == 0)
            return;
        DeviceChange[] changes = _changes.ToArray();
        _changes.Clear();
        foreach (DeviceChange change in changes)
        {
            if (change.Kind == ChangeKind.Removed)
            {
                _skipped.Remove(change.DeviceId);
                Remove(change.DeviceId);
            }
            else
            {
                Reconcile(change.DeviceId);
            }
        }
    }

    /* Adds, removes or re-adds the device to match its current eligibility and layout */
    private void Reconcile(int deviceId)
    {
        InputDevice device = InputDevice.GetDevice(deviceId);
        Tracked t = Find(deviceId);
        if (!IsEligible(device))
        {
            if (t != null)
                Remove(deviceId);
            if (device != null && _skipped.Add(deviceId))
            {
                WriteLog("GAMEPAD android skip \"" + device.Name + "\" vid:pid=" + FormatVidPid(device) + " sources=0x"
                    + ((int)device.Sources).ToString("x", CultureInfo.InvariantCulture));
            }
            return;
        }
        _skipped.Remove(deviceId);

        Description description = Describe(device);
        if (t != null)
        {
            if (IsSameLayout(t.Description, description))
                return;
            Remove(deviceId);
        }
        Add(device, description);
    }

    private static bool IsEligible(InputDevice device)
    {
        if (device == null || device.IsVirtual)
            return false;
        if (device.SupportsSource(InputSourceType.Gamepad))
            return true;
        return device.SupportsSource(InputSourceType.Joystick) && device.GetMotionRange(Axis.X, InputSourceType.Joystick) != null;
    }

    private static Description Describe(InputDevice device)
    {
        List<Axis> axes = new List<Axis>();
        List<InputDevice.MotionRange> ranges = new List<InputDevice.MotionRange>();
        foreach (Axis axis in SampleAxes)
        {
            InputDevice.MotionRange range = device.GetMotionRange(axis, InputSourceType.Joystick);
            if (range == null)
                continue;
            axes.Add(axis);
            ranges.Add(range);
        }

        InputDevice.MotionRange zRange = FindRange(axes, ranges, Axis.Z);
        InputDevice.MotionRange rzRange = FindRange(axes, ranges, Axis.Rz);
        bool hasTriggerAxis = axes.Contains(Axis.Ltrigger) || axes.Contains(Axis.Rtrigger) || axes.Contains(Axis.Brake)
            || axes.Contains(Axis.Gas);

        GHAndroidDeviceLayout layout = new GHAndroidDeviceLayout();
        layout.Axes = GHAndroidGamepadMapping.GetAxisLayout(zRange != null, rzRange != null, axes.Contains(Axis.Rx),
            axes.Contains(Axis.Ry), hasTriggerAxis);
        layout.AnalogTriggers = hasTriggerAxis || layout.Axes == GHAndroidAxisLayout.LinuxGamepad;
        if (zRange != null)
        {
            layout.ZMin = zRange.Min;
            layout.ZMax = zRange.Max;
        }
        if (rzRange != null)
        {
            layout.RzMin = rzRange.Min;
            layout.RzMax = rzRange.Max;
        }

        InputDevice.MotionRange leftRange = FindRange(axes, ranges, Axis.X);
        InputDevice.MotionRange rightRange = FindRange(axes, ranges, layout.Axes == GHAndroidAxisLayout.Standard ? Axis.Z : Axis.Rx);
        bool[] stickClicks = device.HasKeys(new int[] { (int)Keycode.ButtonThumbl, (int)Keycode.ButtonThumbr });

        Description description = new Description();
        description.Layout = layout;
        description.Axes = axes.ToArray();
        description.Ranges = ranges.ToArray();
        description.HasSticks = leftRange != null;
        description.HasStickClicks = stickClicks != null && stickClicks.Length >= 2 && stickClicks[0] && stickClicks[1];
        description.InnerDeadzone = GHAndroidGamepadMapping.GetInnerDeadzone(leftRange != null ? leftRange.Flat : 0f,
            rightRange != null ? rightRange.Flat : 0f);
        return description;
    }

    private static InputDevice.MotionRange FindRange(List<Axis> axes, List<InputDevice.MotionRange> ranges, Axis axis)
    {
        int i = axes.IndexOf(axis);
        return i >= 0 ? ranges[i] : null;
    }

    private static bool IsSameLayout(Description a, Description b)
    {
        GHAndroidDeviceLayout la = a.Layout;
        GHAndroidDeviceLayout lb = b.Layout;
        if (la.Axes != lb.Axes || la.AnalogTriggers != lb.AnalogTriggers || la.ZMin != lb.ZMin || la.ZMax != lb.ZMax
            || la.RzMin != lb.RzMin || la.RzMax != lb.RzMax)
            return false;
        if (a.Axes.Length != b.Axes.Length)
            return false;
        int i;
        for (i = 0; i < a.Axes.Length; i++)
        {
            if (a.Axes[i] != b.Axes[i])
                return false;
        }
        return true;
    }

    private void Add(InputDevice device, Description description)
    {
        Tracked t = new Tracked();
        t.DeviceId = device.Id;
        t.Id = AllocateControllerId();
        t.Description = description;
        t.Device = new GHAndroidGamepadDevice(t.Id, description.Layout);
        _tracked.Add(t);

        string deviceName = device.Name;
        GHGamepadControllerInfo info = new GHGamepadControllerInfo();
        info.Id = t.Id;
        info.Name = string.IsNullOrEmpty(deviceName) ? "Game controller" : deviceName;
        info.BackendName = Name;
        info.Details = "Android dev=" + device.Id + " vid:pid=" + FormatVidPid(device) + " sources=0x"
            + ((int)device.Sources).ToString("x", CultureInfo.InvariantCulture) + " layout=" + description.Layout.Axes;
        info.Style = GHAndroidGamepadMapping.GetStyle(device.VendorId, device.ProductId, deviceName);
        GHGamepadCapabilities caps = new GHGamepadCapabilities();
        caps.HasAnalogSticks = description.HasSticks;
        caps.HasAnalogTriggers = description.Layout.AnalogTriggers;
        caps.HasStickClicks = description.HasStickClicks;
        caps.HasGrips = false;
        info.Capabilities = caps;
        info.IsAuxiliary = false;
        info.InnerDeadzone = description.InnerDeadzone;
        _sink.ControllerAdded(info);

        StringBuilder sb = new StringBuilder();
        sb.Append("GAMEPAD android +connect id=").Append(t.Id).Append(" name=\"").Append(info.Name).Append("\" style=")
            .Append(info.Style).Append(" analogTriggers=").Append(caps.HasAnalogTriggers ? 1 : 0)
            .Append(" stickClicks=").Append(caps.HasStickClicks ? 1 : 0).Append(' ').Append(info.Details).Append(" axes=");
        int i;
        for (i = 0; i < description.Axes.Length; i++)
        {
            InputDevice.MotionRange range = description.Ranges[i];
            if (i > 0)
                sb.Append(',');
            sb.Append(description.Axes[i]).Append(':')
                .Append(range.Min.ToString("0.00", CultureInfo.InvariantCulture)).Append("..")
                .Append(range.Max.ToString("0.00", CultureInfo.InvariantCulture)).Append('/')
                .Append(range.Flat.ToString("0.00", CultureInfo.InvariantCulture));
        }
        WriteLog(sb.ToString());
    }

    private void Remove(int deviceId)
    {
        int i;
        for (i = _tracked.Count - 1; i >= 0; i--)
        {
            Tracked t = _tracked[i];
            if (t.DeviceId != deviceId)
                continue;
            _tracked.RemoveAt(i);
            t.Device.Clear();
            _sink?.ControllerRemoved(t.Id);
            WriteLog("GAMEPAD android -disconnect id=" + t.Id + " dev=" + deviceId);
        }
    }

    private Tracked Find(int deviceId)
    {
        foreach (Tracked t in _tracked)
        {
            if (t.DeviceId == deviceId)
                return t;
        }
        return null;
    }

    /* historyIndex -1 reads the current values */
    private static GHAndroidAxisSample ReadSample(MotionEvent e, Axis[] axes, int historyIndex)
    {
        GHAndroidAxisSample sample = new GHAndroidAxisSample();
        foreach (Axis axis in axes)
        {
            float value = historyIndex < 0 ? e.GetAxisValue(axis) : e.GetHistoricalAxisValue(axis, historyIndex);
            switch (axis)
            {
            case Axis.X:
                sample.X = value;
                break;
            case Axis.Y:
                sample.Y = value;
                break;
            case Axis.Z:
                sample.Z = value;
                break;
            case Axis.Rz:
                sample.Rz = value;
                break;
            case Axis.Rx:
                sample.Rx = value;
                break;
            case Axis.Ry:
                sample.Ry = value;
                break;
            case Axis.HatX:
                sample.HatX = value;
                break;
            case Axis.HatY:
                sample.HatY = value;
                break;
            case Axis.Ltrigger:
                sample.LTrigger = value;
                break;
            case Axis.Rtrigger:
                sample.RTrigger = value;
                break;
            case Axis.Brake:
                sample.Brake = value;
                break;
            case Axis.Gas:
                sample.Gas = value;
                break;
            }
        }
        return sample;
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

    /* The binding's values against the constants GHAndroidGamepadMapping carries; null when they agree */
    private static string FindConstantMismatch()
    {
        return CheckConstant("Keycode.ButtonA", (int)Keycode.ButtonA, GHAndroidGamepadMapping.KeycodeButtonA)
            ?? CheckConstant("Keycode.ButtonMode", (int)Keycode.ButtonMode, GHAndroidGamepadMapping.KeycodeButtonMode)
            ?? CheckConstant("Keycode.DpadUp", (int)Keycode.DpadUp, GHAndroidGamepadMapping.KeycodeDpadUp)
            ?? CheckConstant("Axis.HatX", (int)Axis.HatX, GHAndroidGamepadMapping.AxisHatX)
            ?? CheckConstant("Axis.Brake", (int)Axis.Brake, GHAndroidGamepadMapping.AxisBrake)
            ?? CheckConstant("Axis.Rz", (int)Axis.Rz, GHAndroidGamepadMapping.AxisRz);
    }

    private static string CheckConstant(string name, int actual, int expected)
    {
        if (actual == expected)
            return null;
        return name + " is " + actual + ", GHAndroidGamepadMapping has " + expected;
    }

    private static string FormatVidPid(InputDevice device)
    {
        return device.VendorId.ToString("x4", CultureInfo.InvariantCulture) + ":"
            + device.ProductId.ToString("x4", CultureInfo.InvariantCulture);
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

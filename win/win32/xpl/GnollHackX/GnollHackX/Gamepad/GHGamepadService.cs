using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
#if GNH_MAUI
using GnollHackM;
#else
using GnollHackX.Pages.Game;
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    /* The gamepad hub, on the UI thread. Poll pumps every backend, gates input on the window being active,
       the app not being suspended, the keyboard hook being enabled and the Use Controller setting, resyncs
       after a stall, tracks presses, derives LT/RT and the virtual stick directions from the active
       controller, and routes button events and then the stick snapshot to the top page. Routing: no game
       page drops everything; an open message popup gets the event (IGHGamepadPopupPage, else B Pressed as
       Escape and A ShortPressed as Enter); else an IGHGamepadHandlingPage; else the keyboard adapter.
       Presses are captured per (top page, popup open). Every Poll ends with the game page's frame tick. */
    public static class GHGamepadService
    {
        private const double ErrorLogIntervalSeconds = 10.0;

        private struct Edge
        {
            public int ControllerId;
            public GHGamepadButton Button;
            public bool Down;
            public long Timestamp;
        }

        /* Raw stick and trigger values per controller, as the backend reported them */
        private sealed class ControllerValues
        {
            public float LeftX, LeftY, RightX, RightY;
            public float LeftTrigger, RightTrigger;
            public bool SuppressDerived = true;
        }

        private sealed class InputSink : IGHGamepadInputSink
        {
            public void ControllerAdded(GHGamepadControllerInfo info)
            {
                OnControllerAdded(info);
            }

            public void ControllerRemoved(int controllerId)
            {
                OnControllerRemoved(controllerId);
            }

            public void Button(int controllerId, GHGamepadButton button, bool down, long timestamp)
            {
                Edge e;
                e.ControllerId = controllerId;
                e.Button = button;
                e.Down = down;
                e.Timestamp = timestamp;
                _edges.Add(e);
            }

            public void Stick(int controllerId, int stick, float x, float y, long timestamp)
            {
                ControllerValues v;
                if (!_values.TryGetValue(controllerId, out v))
                    return;
                if (stick == 0)
                {
                    v.LeftX = x;
                    v.LeftY = y;
                }
                else if (stick == 1)
                {
                    v.RightX = x;
                    v.RightY = y;
                }
            }

            public void Trigger(int controllerId, bool right, float value, long timestamp)
            {
                ControllerValues v;
                if (!_values.TryGetValue(controllerId, out v))
                    return;
                if (right)
                    v.RightTrigger = value;
                else
                    v.LeftTrigger = value;
            }
        }

        private static readonly GHGamepadTiming _timing = GHGamepadTiming.Default;
        private static readonly InputSink _sink = new InputSink();
        private static readonly List<IGHGamepadBackend> _backends = new List<IGHGamepadBackend>();
        private static readonly GHGamepadControllerSelector _selector = new GHGamepadControllerSelector();
        private static readonly GHGamepadButtonTracker _tracker = new GHGamepadButtonTracker(_timing, Stopwatch.Frequency);
        private static readonly GHGamepadPressRouter _router = new GHGamepadPressRouter();
        private static readonly Dictionary<int, ControllerValues> _values = new Dictionary<int, ControllerValues>();
        private static readonly List<Edge> _edges = new List<Edge>();
        private static readonly List<GHGamepadButtonEvent> _events = new List<GHGamepadButtonEvent>();

        private static int _isActive = 0;
        private static int _isEnabled = 1;
        private static int _isWindowActive = 1;
        private static int _isAvailable = 0;

        /* UI thread */
        private static bool _wasGated = false;
        private static bool _enabledSeen = true;
        private static bool _activeControllerRemoved = false;
        private static long _lastPollTicks = 0;
        private static long _lastStickTicks = 0;
        private static long _lastErrorLogTicks = 0;
        private static int _leftDirections = 0;
        private static int _rightDirections = 0;
        private static bool _leftTriggerDown = false;
        private static bool _rightTriggerDown = false;
        private static object _stickTarget = null;
        private static GHGamepadStyle _style = GHGamepadStyle.Generic;
        private static GHGamepadCapabilities _capabilities;

        /* Raised on the UI thread when IsActive, Style, Capabilities or IsAvailable changes */
        public static event Action StateChanged;

        /* The last input came from a gamepad; any thread */
        public static bool IsActive { get { return Interlocked.CompareExchange(ref _isActive, 0, 0) != 0; } }

        /* A backend started; independent of the Use Controller setting; any thread */
        public static bool IsAvailable { get { return Interlocked.CompareExchange(ref _isAvailable, 0, 0) != 0; } }

        /* The Use Controller setting; any thread */
        public static bool IsEnabled { get { return Interlocked.CompareExchange(ref _isEnabled, 0, 0) != 0; } }

        /* Active from startup until the first Deactivated; any thread */
        public static bool IsWindowActive { get { return Interlocked.CompareExchange(ref _isWindowActive, 0, 0) != 0; } }

        /* The active controller's style; UI thread */
        public static GHGamepadStyle Style { get { return _style; } }

        /* The active controller's capabilities, with HasGrips also from the auxiliary controllers; UI thread */
        public static GHGamepadCapabilities Capabilities { get { return _capabilities; } }

        /* Any thread; the next Poll applies the change */
        public static void SetEnabled(bool enabled)
        {
            Interlocked.Exchange(ref _isEnabled, enabled ? 1 : 0);
        }

        /* Any thread; the next Poll applies the change */
        public static void SetWindowActive(bool active)
        {
            Interlocked.Exchange(ref _isWindowActive, active ? 1 : 0);
        }

        /* Mouse movement, a pointer press or a non-grip key; UI thread */
        public static void NoteNonGamepadInput()
        {
            if (Interlocked.Exchange(ref _isActive, 0) != 0)
                RaiseStateChanged();
        }

        /* Starts the backend on the UI thread; a backend that fails to start is not kept */
        public static void RegisterBackend(IGHGamepadBackend backend)
        {
            string error;
            RegisterBackend(backend, out error);
        }

        public static bool RegisterBackend(IGHGamepadBackend backend, out string error)
        {
            error = null;
            if (backend == null)
            {
                error = "no backend";
                return false;
            }
            if (_backends.Contains(backend))
                return true;
            if (!backend.Start(_sink, out error))
                return false;
            _backends.Add(backend);
            if (Interlocked.Exchange(ref _isAvailable, 1) == 0)
                RaiseStateChanged();
            return true;
        }

        /* Stops every backend and forgets the controllers; UI thread */
        public static void StopAllBackends()
        {
            long now = Stopwatch.GetTimestamp();
            foreach (IGHGamepadBackend backend in _backends.ToArray())
            {
                try
                {
                    backend.Stop();
                }
                catch (Exception ex)
                {
                    LogError("GHGamepadService: stopping " + backend.Name + " failed: " + ex.Message, now);
                }
            }
            _backends.Clear();
            _events.Clear();
            _tracker.CancelAll(now, _events);
            RouteEvents(false);
            _tracker.Reset();
            _router.Reset();
            _selector.Clear();
            _values.Clear();
            _edges.Clear();
            ResetDerived();
            SendNeutralSticks();
            Interlocked.Exchange(ref _isAvailable, 0);
            Interlocked.Exchange(ref _isActive, 0);
            UpdateControllerState(true);
        }

        /* Once per UI frame */
        public static void Poll(long nowTicks)
        {
            try
            {
                PollCore(nowTicks);
            }
            finally
            {
                try
                {
                    IGHGamepadFrameTickPage tickPage = GHApp.CurrentGamePage as IGHGamepadFrameTickPage;
                    if (tickPage != null)
                        tickPage.GamepadFrameTick(nowTicks);
                }
                catch (Exception ex)
                {
                    LogError("GHGamepadService: frame tick failed: " + ex.Message, nowTicks);
                }
            }
        }

        private static void PollCore(long now)
        {
            _events.Clear();
            _edges.Clear();
            bool stalled = _lastPollTicks != 0 && now - _lastPollTicks > (long)(_timing.StallSeconds * Stopwatch.Frequency);
            _lastPollTicks = now;
            if (_backends.Count == 0)
                return;

            foreach (IGHGamepadBackend backend in _backends)
                backend.Pump(now);

            if (_activeControllerRemoved)
            {
                _activeControllerRemoved = false;
                _tracker.CancelAll(now, _events);
            }

            bool enabled = IsEnabled;
            if (enabled != _enabledSeen)
            {
                _enabledSeen = enabled;
                if (!enabled && Interlocked.Exchange(ref _isActive, 0) != 0)
                    RaiseStateChanged();
            }

            /* Gated frames still pump, so the backends stay drained, but their edges are discarded; entering or
               leaving the gate and a stall resync the backends, and held presses end cancelled */
            bool gated = !enabled || !IsWindowActive || GHApp.IsSuspended || !GHApp.IsKeyboardHookEnabled;
            if (gated || stalled || _wasGated)
            {
                _edges.Clear();
                _tracker.CancelAll(now, _events);
                if (gated != _wasGated || stalled)
                {
                    foreach (IGHGamepadBackend backend in _backends)
                        backend.Resync(now);
                    _edges.Clear();
                }
                foreach (ControllerValues v in _values.Values)
                    v.SuppressDerived = true;
            }

            if (gated)
            {
                RouteEvents(false);
                if (!_wasGated)
                    SendNeutralSticks();
                _wasGated = true;
                _lastStickTicks = 0;
                UpdateControllerState(false);
                return;
            }
            if (_wasGated)
            {
                _wasGated = false;
                _lastStickTicks = 0;
            }

            ProcessEdges(now);
            NoteInactiveControllerActivity(now);
            DeriveButtons(now);
            _tracker.Update(now, _events);

            bool revealsUi = false;
            foreach (GHGamepadButtonEvent e in _events)
            {
                if (e.Kind == GHGamepadButtonEventKind.Pressed)
                {
                    if (Interlocked.Exchange(ref _isActive, 1) == 0)
                    {
                        revealsUi = true;
                        RaiseStateChanged();
                    }
                    break;
                }
            }
            UpdateControllerState(false);

            RouteEvents(revealsUi);
            DeliverSticks(now);
        }

        private static void ProcessEdges(long now)
        {
            foreach (Edge edge in _edges)
            {
                if (edge.Down && !_selector.Delivers(edge.ControllerId))
                {
                    if (_selector.NoteActivity(edge.ControllerId))
                        SwitchedController(now);
                }
                if (!_selector.Delivers(edge.ControllerId))
                    continue;
                if (edge.Down)
                    _tracker.Down(edge.Button, edge.Timestamp, _events);
                else
                    _tracker.Up(edge.Button, edge.Timestamp, _events);
            }
            _edges.Clear();
        }

        /* A controller that is not active becomes active on a trigger press or a virtual-direction push */
        private static void NoteInactiveControllerActivity(long now)
        {
            if (_values.Count < 2)
                return;
            int activeId = _selector.ActiveId;
            foreach (KeyValuePair<int, ControllerValues> pair in _values)
            {
                if (pair.Key == activeId || _selector.IsAuxiliary(pair.Key))
                    continue;
                ControllerValues v = pair.Value;
                if (v.SuppressDerived)
                    continue;
                float press = _timing.DirectionPress;
                bool active = Math.Abs(v.LeftX) >= press || Math.Abs(v.LeftY) >= press
                    || Math.Abs(v.RightX) >= press || Math.Abs(v.RightY) >= press
                    || v.LeftTrigger >= _timing.TriggerPress || v.RightTrigger >= _timing.TriggerPress;
                if (active && _selector.NoteActivity(pair.Key))
                {
                    SwitchedController(now);
                    return;
                }
            }
        }

        private static void SwitchedController(long now)
        {
            _tracker.CancelAll(now, _events);
            foreach (ControllerValues v in _values.Values)
                v.SuppressDerived = true;
        }

        /* LT/RT and the virtual stick directions of the active controller. Right after the controller was added
           or resynchronized, a derived button that is already past its threshold starts suppressed. */
        private static void DeriveButtons(long now)
        {
            GHGamepadControllerInfo active = _selector.Active;
            ControllerValues v = null;
            if (active != null)
                _values.TryGetValue(active.Id, out v);

            bool suppress = v != null && v.SuppressDerived;
            float leftX = 0f, leftY = 0f, rightX = 0f, rightY = 0f, leftMagnitude = 0f, rightMagnitude = 0f;
            bool leftTrigger = false, rightTrigger = false;
            if (v != null)
            {
                float inner = active.InnerDeadzone;
                leftMagnitude = GHGamepadMath.ApplyRadialDeadzone(v.LeftX, v.LeftY, inner, _timing.OuterDeadzone, out leftX, out leftY);
                rightMagnitude = GHGamepadMath.ApplyRadialDeadzone(v.RightX, v.RightY, inner, _timing.OuterDeadzone, out rightX, out rightY);
                if (active.Capabilities.HasAnalogTriggers)
                {
                    leftTrigger = GHGamepadMath.UpdateTriggerButton(_leftTriggerDown,
                        GHGamepadMath.ApplyTriggerDeadzone(v.LeftTrigger, _timing.TriggerDeadzone), _timing);
                    rightTrigger = GHGamepadMath.UpdateTriggerButton(_rightTriggerDown,
                        GHGamepadMath.ApplyTriggerDeadzone(v.RightTrigger, _timing.TriggerDeadzone), _timing);
                }
                v.SuppressDerived = false;
            }

            if (active == null || active.Capabilities.HasAnalogTriggers || _leftTriggerDown || _rightTriggerDown)
            {
                SetDerived(GHGamepadButton.LT, ref _leftTriggerDown, leftTrigger, now, suppress);
                SetDerived(GHGamepadButton.RT, ref _rightTriggerDown, rightTrigger, now, suppress);
            }

            int left = GHGamepadMath.UpdateDirectionMask(_leftDirections, leftX, leftY, leftMagnitude, _timing);
            int right = GHGamepadMath.UpdateDirectionMask(_rightDirections, rightX, rightY, rightMagnitude, _timing);
            ApplyDirections(ref _leftDirections, left, GHGamepadButton.LeftStickUp, now, suppress);
            ApplyDirections(ref _rightDirections, right, GHGamepadButton.RightStickUp, now, suppress);
        }

        /* firstButton is the stick's Up button; Down, Left and Right follow it in the enum */
        private static void ApplyDirections(ref int current, int next, GHGamepadButton firstButton, long now, bool suppress)
        {
            int bit;
            for (bit = 0; bit < 4; bit++)
            {
                bool was = (current & (1 << bit)) != 0;
                bool isDown = (next & (1 << bit)) != 0;
                SetDerived(firstButton + bit, ref was, isDown, now, suppress);
            }
            current = next;
        }

        private static void SetDerived(GHGamepadButton button, ref bool state, bool down, long now, bool suppress)
        {
            if (down == state)
                return;
            state = down;
            if (down)
            {
                if (suppress)
                    _tracker.SuppressHeld(button, now);
                else
                    _tracker.Down(button, now, _events);
            }
            else
            {
                _tracker.Up(button, now, _events);
            }
        }

        private static void ResetDerived()
        {
            _leftDirections = 0;
            _rightDirections = 0;
            _leftTriggerDown = false;
            _rightTriggerDown = false;
        }

        private static void OnControllerAdded(GHGamepadControllerInfo info)
        {
            if (info == null)
                return;
            _selector.Add(info);
            if (!info.IsAuxiliary)
                _values[info.Id] = new ControllerValues();
        }

        private static void OnControllerRemoved(int controllerId)
        {
            if (_selector.Remove(controllerId))
                _activeControllerRemoved = true;
            _values.Remove(controllerId);
        }

        private static void UpdateControllerState(bool forceRaise)
        {
            GHGamepadStyle style = _selector.Style;
            GHGamepadCapabilities caps = _selector.Capabilities;
            bool changed = style != _style
                || caps.HasAnalogSticks != _capabilities.HasAnalogSticks
                || caps.HasAnalogTriggers != _capabilities.HasAnalogTriggers
                || caps.HasGrips != _capabilities.HasGrips
                || caps.HasStickClicks != _capabilities.HasStickClicks
                || caps.HasRightStickFullPush != _capabilities.HasRightStickFullPush;
            _style = style;
            _capabilities = caps;
            if (changed || forceRaise)
                RaiseStateChanged();
        }

        /* The top page and whether its message popup is open; null outside a game */
        private static object GetTarget(out bool popupOpen)
        {
            popupOpen = false;
            GamePage gamePage = GHApp.CurrentGamePage;
            if (gamePage == null)
                return null;
            Page top = GHApp.PageFromTopOfModalNavigationStack();
            if (top == null)
                top = gamePage;
            IMessagePopupPage popupPage = top as IMessagePopupPage;
            popupOpen = popupPage != null && popupPage.IsPopupOpen;
            return top;
        }

        private static void RouteEvents(bool revealsUi)
        {
            if (_events.Count == 0)
            {
                bool popup;
                _router.ObserveTarget(GetTarget(out popup), popup);
                return;
            }

            bool popupOpen;
            object target = GetTarget(out popupOpen);
            _router.ObserveTarget(target, popupOpen);
            int i;
            for (i = 0; i < _events.Count; i++)
            {
                GHGamepadButtonEvent e = _events[i];
                e.RevealsUi = revealsUi;
                object capturedTarget;
                bool capturedPopupOpen;
                GHGamepadRouteAction action = _router.Route(e, target, popupOpen, out capturedTarget, out capturedPopupOpen);
                if (action == GHGamepadRouteAction.Deliver)
                {
                    Deliver(target, popupOpen, e);
                }
                else if (action == GHGamepadRouteAction.DeliverCancelled)
                {
                    e.IsCancelled = true;
                    Deliver(capturedTarget, capturedPopupOpen, e);
                }
            }
            _events.Clear();
        }

        /* Handlers must not block or await page changes (R13) */
        private static void Deliver(object target, bool popupOpen, GHGamepadButtonEvent e)
        {
            if (target == null)
                return;
            try
            {
                if (popupOpen)
                {
                    IGHGamepadPopupPage gamepadPopup = target as IGHGamepadPopupPage;
                    if (gamepadPopup != null)
                    {
                        gamepadPopup.SendGamepadButtonToPopup(e);
                        return;
                    }
                    IMessagePopupPage popupPage = target as IMessagePopupPage;
                    if (popupPage == null || !popupPage.IsPopupOpen || e.IsCancelled)
                        return;
                    if (e.Button == GHGamepadButton.B && e.Kind == GHGamepadButtonEventKind.Pressed)
                        popupPage.SendSpecialKeyToPopup(GHSpecialKey.Escape, false, false, false);
                    else if (e.Button == GHGamepadButton.A && e.Kind == GHGamepadButtonEventKind.ShortPressed)
                        popupPage.SendSpecialKeyToPopup(GHSpecialKey.Enter, false, false, false);
                    return;
                }

                IGHGamepadHandlingPage handlingPage = target as IGHGamepadHandlingPage;
                if (handlingPage != null)
                {
                    handlingPage.HandleGamepadButton(e);
                    return;
                }
                DeliverToKeyboardAdapter(e);
            }
            catch (Exception ex)
            {
                LogError("GHGamepadService: handling " + e.Button + " " + e.Kind + " failed: " + ex.Message, e.Timestamp);
            }
        }

        /* Pages without gamepad handling: A Enter, B Escape, D-pad and left stick arrows, LB/RB page keys, on
           Pressed and Repeat */
        private static void DeliverToKeyboardAdapter(GHGamepadButtonEvent e)
        {
            if (e.IsCancelled || (e.Kind != GHGamepadButtonEventKind.Pressed && e.Kind != GHGamepadButtonEventKind.Repeat))
                return;
            GHSpecialKey key;
            switch (e.Button)
            {
            case GHGamepadButton.A:
                key = GHSpecialKey.Enter;
                break;
            case GHGamepadButton.B:
                key = GHSpecialKey.Escape;
                break;
            case GHGamepadButton.DPadUp:
            case GHGamepadButton.LeftStickUp:
                key = GHSpecialKey.Up;
                break;
            case GHGamepadButton.DPadDown:
            case GHGamepadButton.LeftStickDown:
                key = GHSpecialKey.Down;
                break;
            case GHGamepadButton.DPadLeft:
            case GHGamepadButton.LeftStickLeft:
                key = GHSpecialKey.Left;
                break;
            case GHGamepadButton.DPadRight:
            case GHGamepadButton.LeftStickRight:
                key = GHSpecialKey.Right;
                break;
            case GHGamepadButton.LB:
                key = GHSpecialKey.PageUp;
                break;
            case GHGamepadButton.RB:
                key = GHSpecialKey.PageDown;
                break;
            default:
                return;
            }
            if ((e.Button == GHGamepadButton.A || e.Button == GHGamepadButton.B) && e.Kind == GHGamepadButtonEventKind.Repeat)
                return;
            GHApp.SendSpecialKeyPress(key, false, false, false);
        }

        /* The active controller's sticks after the radial deadzone, to the top page when it handles gamepad
           input and no popup is open; the previous target gets one neutral snapshot when the target changes */
        private static void DeliverSticks(long now)
        {
            bool popupOpen;
            object top = GetTarget(out popupOpen);
            object target = !popupOpen && top is IGHGamepadHandlingPage ? top : null;
            if (!ReferenceEquals(target, _stickTarget))
            {
                SendNeutralSticks();
                _stickTarget = target;
                _lastStickTicks = 0;
            }
            if (target == null)
                return;

            double dt = _lastStickTicks == 0 ? 0.0 : (now - _lastStickTicks) / (double)Stopwatch.Frequency;
            dt = GHGamepadMath.Clamp(dt, 0.0, _timing.MaxStickDeltaSeconds);
            _lastStickTicks = now;

            GHGamepadSticks sticks = new GHGamepadSticks();
            GHGamepadControllerInfo active = _selector.Active;
            ControllerValues v;
            if (active != null && _values.TryGetValue(active.Id, out v))
            {
                float inner = active.InnerDeadzone;
                sticks.LeftMagnitude = GHGamepadMath.ApplyRadialDeadzone(v.LeftX, v.LeftY, inner, _timing.OuterDeadzone, out sticks.LeftX, out sticks.LeftY);
                sticks.RightMagnitude = GHGamepadMath.ApplyRadialDeadzone(v.RightX, v.RightY, inner, _timing.OuterDeadzone, out sticks.RightX, out sticks.RightY);
                sticks.LeftTrigger = GHGamepadMath.ApplyTriggerDeadzone(v.LeftTrigger, _timing.TriggerDeadzone);
                sticks.RightTrigger = GHGamepadMath.ApplyTriggerDeadzone(v.RightTrigger, _timing.TriggerDeadzone);
            }
            sticks.IsNeutral = sticks.LeftMagnitude == 0f && sticks.RightMagnitude == 0f
                && sticks.LeftTrigger == 0f && sticks.RightTrigger == 0f;
            try
            {
                ((IGHGamepadHandlingPage)target).UpdateGamepadSticks(sticks, dt);
            }
            catch (Exception ex)
            {
                LogError("GHGamepadService: stick update failed: " + ex.Message, now);
            }
        }

        private static void SendNeutralSticks()
        {
            IGHGamepadHandlingPage page = _stickTarget as IGHGamepadHandlingPage;
            _stickTarget = null;
            if (page == null)
                return;
            GHGamepadSticks neutral = new GHGamepadSticks();
            neutral.IsNeutral = true;
            try
            {
                page.UpdateGamepadSticks(neutral, 0.0);
            }
            catch (Exception ex)
            {
                LogError("GHGamepadService: stick update failed: " + ex.Message, Stopwatch.GetTimestamp());
            }
        }

        private static void RaiseStateChanged()
        {
            Action handler = StateChanged;
            if (handler == null)
                return;
            try
            {
                handler();
            }
            catch (Exception ex)
            {
                LogError("GHGamepadService: StateChanged handler failed: " + ex.Message, Stopwatch.GetTimestamp());
            }
        }

        /* At most one entry per ErrorLogIntervalSeconds */
        private static void LogError(string message, long nowTicks)
        {
            Debug.WriteLine(message);
            if (_lastErrorLogTicks != 0 && nowTicks - _lastErrorLogTicks < (long)(ErrorLogIntervalSeconds * Stopwatch.Frequency))
                return;
            _lastErrorLogTicks = nowTicks;
            GHApp.WriteGHLog(message);
        }
    }
}

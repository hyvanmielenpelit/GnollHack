using System;
using System.Diagnostics;
using GnollHackX;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Input;

namespace GnollHackM
{
    /* Gamepad glue for the WinUI 3 build. At the first frame at least a second after the first OnFrame it registers
       the XInput backend, which waits for a controller. OnFrame runs first in every Rendering callback and polls the
       service. XInput reads the pad whatever has focus, so window activation is read every frame from WindowFocused and
       the foreground check. Root pointer handlers, which clear the service's gamepad-active state, exist only while the
       pad is the active input, because each pointer event costs CsWinRT wrappers. A key press arrives from the keyboard
       hook as a flag. A frame that throws is logged at most once per 10 s, and 30 such frames in a row stop the backend
       for the session. Nothing may escape OnFrame: an exception in a Rendering callback fails fast. UI thread only. */
    internal static class GHWinUIGamepad
    {
        private const double StartDelaySeconds = 1.0;
        private const double PointerMoveThreshold = 4.0;
        private const double ErrorLogIntervalSeconds = 10.0;
        private const int MaxConsecutiveFailures = 30;

        private static long _firstFrameTicks = 0;
        private static bool _started = false;
        private static bool _stoppedForSession = false;
        private static int _consecutiveFailures = 0;
        private static long _lastErrorLogTicks = 0;
        private static bool _errorLogged = false;
        private static bool _keyDownPending = false;
        private static UIElement _pointerRoot = null;
        private static PointerEventHandler _movedHandler = null;
        private static PointerEventHandler _pressedHandler = null;
        private static bool _hasPointerBase = false;
        private static Windows.Foundation.Point _pointerBase;

        /* From the keyboard hook, which must return quickly; the next OnFrame applies it */
        public static void NoteKeyDown()
        {
            _keyDownPending = true;
        }

        public static void OnFrame(long nowTicks)
        {
            if (_stoppedForSession)
            {
                /* Without backends Poll only runs the game page's frame tick */
                try
                {
                    GHGamepadService.Poll(nowTicks);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                return;
            }

            try
            {
                if (_firstFrameTicks == 0)
                    _firstFrameTicks = nowTicks;
                if (!_started && nowTicks - _firstFrameTicks >= (long)(StartDelaySeconds * Stopwatch.Frequency))
                {
                    _started = true;
                    string error;
                    if (!GHGamepadService.RegisterBackend(new GHXInputGamepadBackend(), true, out error))
                        GHApp.MaybeWriteGHLog("GAMEPAD XInput backend not started: " + error);
                    GHGamepadService.StateChanged += UpdatePointerHooks;
                }
                if (_keyDownPending)
                {
                    _keyDownPending = false;
                    GHGamepadService.NoteNonGamepadInput();
                }
                if (GHGamepadService.IsAvailable)
                    GHGamepadService.SetWindowActive(GHApp.WindowFocused && WindowFocusHelper.IsOwnProcessForeground());
                GHGamepadService.Poll(nowTicks);
                _consecutiveFailures = 0;
            }
            catch (Exception ex)
            {
                _consecutiveFailures++;
                if (!_errorLogged || nowTicks - _lastErrorLogTicks >= (long)(ErrorLogIntervalSeconds * Stopwatch.Frequency))
                {
                    _errorLogged = true;
                    _lastErrorLogTicks = nowTicks;
                    GHApp.MaybeWriteGHLog("GAMEPAD frame failed: " + ex.GetType().Name + ": " + ex.Message);
                }
                if (_consecutiveFailures >= MaxConsecutiveFailures)
                {
                    _stoppedForSession = true;
                    try
                    {
                        GHGamepadService.StopAllBackends();
                    }
                    catch (Exception stopEx)
                    {
                        GHApp.MaybeWriteGHLog("GAMEPAD stopping the backends failed: " + stopEx.Message);
                    }
                }
            }
        }

        private static void UpdatePointerHooks()
        {
            bool want = GHGamepadService.IsActive;
            if (want == (_pointerRoot != null))
                return;
            if (want)
            {
                UIElement root = GHApp.WindowsXamlWindow?.Content;
                if (root == null)
                    return;
                if (_movedHandler == null)
                {
                    _movedHandler = new PointerEventHandler(OnPointerMoved);
                    _pressedHandler = new PointerEventHandler(OnPointerPressed);
                }
                _hasPointerBase = false;
                root.AddHandler(UIElement.PointerMovedEvent, _movedHandler, true);
                root.AddHandler(UIElement.PointerPressedEvent, _pressedHandler, true);
                _pointerRoot = root;
            }
            else
            {
                _pointerRoot.RemoveHandler(UIElement.PointerMovedEvent, _movedHandler);
                _pointerRoot.RemoveHandler(UIElement.PointerPressedEvent, _pressedHandler);
                _pointerRoot = null;
            }
        }

        /* A move of more than PointerMoveThreshold from where the pointer first moved switches back to the mouse */
        private static void OnPointerMoved(object sender, PointerRoutedEventArgs e)
        {
            UIElement root = _pointerRoot;
            if (root == null)
                return;
            Windows.Foundation.Point position = e.GetCurrentPoint(root).Position;
            if (!_hasPointerBase)
            {
                _pointerBase = position;
                _hasPointerBase = true;
                return;
            }
            double dx = position.X - _pointerBase.X;
            double dy = position.Y - _pointerBase.Y;
            if (dx * dx + dy * dy > PointerMoveThreshold * PointerMoveThreshold)
                GHGamepadService.NoteNonGamepadInput();
        }

        private static void OnPointerPressed(object sender, PointerRoutedEventArgs e)
        {
            GHGamepadService.NoteNonGamepadInput();
        }
    }
}

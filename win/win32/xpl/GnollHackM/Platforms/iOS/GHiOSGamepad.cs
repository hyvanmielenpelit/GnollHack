using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.Versioning;
using Foundation;
using GameController;
using GnollHackX;
using UIKit;

namespace GnollHackM;

/* Gamepad glue for the iOS build. At the first frame at least a second after the first OnFrame it registers the
   GameController backend, which waits for a controller, and the on-screen keyboard provider. OnFrame runs first in
   every display link callback and polls the service. Once a controller has been seen, every frame sets the window
   activation from the application state and from whether a view controller that is not the game's own, such as an
   alert, a share sheet or the system browser, is presented on top. On iOS 18 and later, while Use Controller is on and
   a controller is connected, a GCEventInteraction on the window keeps the controller's events out of UIKit.
   GHUIApplication passes its touch, key and press events here: a touch in the game's window or a hardware key clears
   the service's gamepad-active state. A frame that throws is logged at most once per 10 s, and 30 such frames in a row
   stop the backend for the session. Nothing may escape an entry point. UI thread only. */
internal static class GHiOSGamepad
{
    private const double StartDelaySeconds = 1.0;
    private const double ErrorLogIntervalSeconds = 10.0;
    private const int MaxConsecutiveFailures = 30;
    private const int MaxPresentationDepth = 16;
    private const int MaxLoggedPressTypes = 16;

    private static readonly Assembly MauiControlsAssembly = typeof(Microsoft.Maui.Controls.Page).Assembly;
    private static readonly Assembly MauiCoreAssembly = typeof(Microsoft.Maui.Platform.ContainerViewController).Assembly;
    private static readonly Assembly OwnAssembly = typeof(GHiOSGamepad).Assembly;

    private static long _firstFrameTicks = 0;
    private static bool _started = false;
    private static bool _stoppedForSession = false;
    private static int _consecutiveFailures = 0;
    private static long _lastErrorLogTicks = 0;
    private static bool _errorLogged = false;
    private static long _lastEventErrorLogTicks = 0;
    private static bool _eventErrorLogged = false;
    private static GHiOSGamepadBackend _backend = null;
    private static WeakReference<UIWindow> _window = null;
    private static IUIInteraction _eventInteraction = null;
    private static bool _eventInteractionInstalled = false;
    private static readonly HashSet<long> _loggedPressTypes = new HashSet<long>();

    /* The game's window, from the scene delegate */
    public static void SetWindow(UIWindow window)
    {
        RemoveEventInteraction();
        _window = window != null ? new WeakReference<UIWindow>(window) : null;
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
                GHiOSGamepadBackend backend = new GHiOSGamepadBackend();
                string error;
                if (GHGamepadService.RegisterBackend(backend, true, out error))
                    _backend = backend;
                else
                    GHApp.MaybeWriteGHLog("GAMEPAD iOS backend not started: " + error);
                GHScreenKeyboard.Provider = new GHiOSScreenKeyboard();
            }
            if (GHGamepadService.IsAvailable)
            {
                if (OperatingSystem.IsIOSVersionAtLeast(18))
                    UpdateEventInteraction();
                GHGamepadService.SetWindowActive(IsInputActive());
            }
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
                _backend = null;
                try
                {
                    GHGamepadService.StopAllBackends();
                }
                catch (Exception stopEx)
                {
                    GHApp.MaybeWriteGHLog("GAMEPAD stopping the backends failed: " + stopEx.Message);
                }
                RemoveEventInteraction();
            }
        }
    }

    /* From GHUIApplication.SendEvent, for a touch event while the controller is the active input. Never consumes the
       event. A touch in another window, such as the software keyboard's, keeps controller mode. */
    public static void NoteTouches(UIEvent uievent)
    {
        try
        {
            if (!GHGamepadService.IsActive || uievent == null || uievent.Type != UIEventType.Touches)
                return;
            NSSet allTouches = uievent.AllTouches;
            if (allTouches == null)
                return;
            UIWindow window = GetWindow();
            foreach (UITouch touch in allTouches.ToArray<UITouch>())
            {
                if (touch == null || touch.Phase != UITouchPhase.Began)
                    continue;
                UIWindow touchWindow = touch.Window;
                if (window != null && (touchWindow == null || touchWindow.Handle != window.Handle))
                    continue;
                GHGamepadService.NoteNonGamepadInput("touch " + touch.Type);
                return;
            }
        }
        catch (Exception ex)
        {
            LogEventError("touch event", ex);
        }
    }

    /* From GHUIApplication's key handlers; a key typed while the on-screen keyboard shows keeps controller mode */
    public static void NoteKey(string cause)
    {
        try
        {
            if (GHGamepadService.IsActive && !GHScreenKeyboard.IsShown)
                GHGamepadService.NoteNonGamepadInput(cause);
        }
        catch (Exception ex)
        {
            LogEventError("key event", ex);
        }
    }

    /* From GHUIApplication.PressesBegan. A press with a key is a hardware key; a modifier or Caps Lock alone keeps
       controller mode. While a controller is connected, a press without a key is logged once per press type: it shows
       which controller buttons UIKit still receives. */
    public static void NotePresses(NSSet<UIPress> presses)
    {
        try
        {
            if (presses == null)
                return;
            GHiOSGamepadBackend backend = _backend;
            bool logTypes = backend != null && backend.HasControllers && _loggedPressTypes.Count < MaxLoggedPressTypes;
            if (!logTypes && !GHGamepadService.IsActive)
                return;
            foreach (UIPress press in presses)
            {
                if (press == null)
                    continue;
                UIKey key = press.Key;
                if (key != null)
                {
                    if (!IsModifierKey(key))
                        NoteKey("key press");
                    continue;
                }
                long type = (long)press.Type;
                if (logTypes && _loggedPressTypes.Count < MaxLoggedPressTypes && _loggedPressTypes.Add(type))
                    GHiOSGamepadBackend.WriteLog("GAMEPAD ios uikit press type=" + type);
            }
        }
        catch (Exception ex)
        {
            LogEventError("press event", ex);
        }
    }

    public static UIWindow GetWindow()
    {
        UIWindow window = null;
        if (_window != null)
            _window.TryGetTarget(out window);
        return window != null && window.Handle != IntPtr.Zero ? window : null;
    }

    private static bool IsInputActive()
    {
        if (UIApplication.SharedApplication.ApplicationState != UIApplicationState.Active)
            return false;
        return !IsForeignViewControllerPresented();
    }

    /* True when the topmost presented view controller is not one of MAUI's or the game's own */
    private static bool IsForeignViewControllerPresented()
    {
        UIWindow window = GetWindow();
        UIViewController root = window != null ? window.RootViewController : null;
        UIViewController top = root != null ? root.PresentedViewController : null;
        if (top == null)
            return false;
        int depth;
        for (depth = 0; depth < MaxPresentationDepth; depth++)
        {
            UIViewController next = top.PresentedViewController;
            if (next == null)
                break;
            top = next;
        }
        Assembly assembly = top.GetType().Assembly;
        return !ReferenceEquals(assembly, MauiControlsAssembly) && !ReferenceEquals(assembly, MauiCoreAssembly)
            && !ReferenceEquals(assembly, OwnAssembly);
    }

    private static bool IsModifierKey(UIKey key)
    {
        UIKeyboardHidUsage code = key.KeyCode;
        return code == UIKeyboardHidUsage.KeyboardCapsLock
            || (code >= UIKeyboardHidUsage.KeyboardLeftControl && code <= UIKeyboardHidUsage.KeyboardRightGui);
    }

    [SupportedOSPlatform("ios18.0")]
    private static void UpdateEventInteraction()
    {
        GHiOSGamepadBackend backend = _backend;
        bool want = GHGamepadService.IsEnabled && backend != null && backend.HasControllers;
        if (want == _eventInteractionInstalled)
            return;
        if (!want)
        {
            RemoveEventInteraction();
            return;
        }
        UIWindow window = GetWindow();
        if (window == null)
            return;
        if (_eventInteraction == null)
        {
            GCEventInteraction interaction = new GCEventInteraction();
            interaction.HandledEventTypes = GCUIEventTypes.Gamepad;
            _eventInteraction = interaction;
        }
        window.AddInteraction(_eventInteraction);
        _eventInteractionInstalled = true;
    }

    /* Takes the interaction off the window it was added to */
    private static void RemoveEventInteraction()
    {
        if (!_eventInteractionInstalled)
            return;
        _eventInteractionInstalled = false;
        try
        {
            UIWindow window = GetWindow();
            if (window != null && _eventInteraction != null)
                window.RemoveInteraction(_eventInteraction);
        }
        catch (Exception ex)
        {
            Debug.WriteLine(ex.Message);
        }
    }

    private static void LogEventError(string what, Exception ex)
    {
        try
        {
            long now = Stopwatch.GetTimestamp();
            if (_eventErrorLogged && now - _lastEventErrorLogTicks < (long)(ErrorLogIntervalSeconds * Stopwatch.Frequency))
                return;
            _eventErrorLogged = true;
            _lastEventErrorLogTicks = now;
            GHApp.MaybeWriteGHLog("GAMEPAD " + what + " failed: " + ex.GetType().Name + ": " + ex.Message);
        }
        catch (Exception logEx)
        {
            Debug.WriteLine(logEx.Message);
        }
    }
}

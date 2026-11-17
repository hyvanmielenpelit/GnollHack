using System;
using System.Diagnostics;
using Android.Views;
using GnollHackX;

namespace GnollHackM;

/* Gamepad glue for the Android build. At the first frame at least a second after the first OnFrame it registers
   the Android backend, which waits for a controller, and the on-screen keyboard provider. OnFrame runs first in
   every Rendering callback and polls the service. MainActivity and GHAndroidWindowCallback, on each modal page's
   window, pass their key, generic motion and touch events here. A key from another device or a touch clears the
   service's gamepad-active state; soft keyboard, modifier and system keys do not. The activity's pause and resume
   set the window activation. A frame that throws is logged at most once per 10 s, and 30 such frames in a row stop
   the backend for the session. Nothing may escape an entry point: an exception escaping a Java callback ends the
   app. UI thread only. */
internal static class GHAndroidGamepad
{
    private const double StartDelaySeconds = 1.0;
    private const double ErrorLogIntervalSeconds = 10.0;
    private const int MaxConsecutiveFailures = 30;

    private static long _firstFrameTicks = 0;
    private static bool _started = false;
    private static bool _stoppedForSession = false;
    private static int _consecutiveFailures = 0;
    private static long _lastErrorLogTicks = 0;
    private static bool _errorLogged = false;
    private static long _lastEventErrorLogTicks = 0;
    private static bool _eventErrorLogged = false;
    private static GHAndroidGamepadBackend _backend = null;

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
                GHAndroidGamepadBackend backend = new GHAndroidGamepadBackend();
                string error;
                if (GHGamepadService.RegisterBackend(backend, true, out error))
                    _backend = backend;
                else
                    GHApp.MaybeWriteGHLog("GAMEPAD Android backend not started: " + error);
                GHScreenKeyboard.Provider = new GHAndroidScreenKeyboard();
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
            }
        }
    }

    /* True when the event is consumed */
    public static bool DispatchKeyEvent(KeyEvent e)
    {
        try
        {
            if (e == null)
                return false;
            GHAndroidGamepadBackend backend = _backend;
            if (backend != null && backend.HandleKey(e))
                return true;
            if (GHGamepadService.IsActive && e.Action == KeyEventActions.Down
                && (backend == null || !backend.IsTrackedDevice(e.DeviceId)) && !KeepsGamepadActive(e))
                GHGamepadService.NoteNonGamepadInput("key");
        }
        catch (Exception ex)
        {
            LogEventError("key event", ex);
        }
        return false;
    }

    /* True when the event is consumed */
    public static bool DispatchGenericMotionEvent(MotionEvent e)
    {
        try
        {
            GHAndroidGamepadBackend backend = _backend;
            return backend != null && backend.HandleMotion(e);
        }
        catch (Exception ex)
        {
            LogEventError("motion event", ex);
            return false;
        }
    }

    /* Never consumes the event */
    public static void NoteTouch(MotionEvent e)
    {
        try
        {
            /* Checked before the event is read: every touch of every player comes here */
            if (!GHGamepadService.IsActive)
                return;
            if (e != null && e.ActionMasked == MotionEventActions.Down)
                GHGamepadService.NoteNonGamepadInput("touch");
        }
        catch (Exception ex)
        {
            LogEventError("touch event", ex);
        }
    }

    /* From the activity's resume and pause */
    public static void SetActive(bool active)
    {
        try
        {
            GHAndroidGamepadBackend backend = _backend;
            if (!active && backend != null)
                backend.InputLost(Stopwatch.GetTimestamp());
            GHGamepadService.SetWindowActive(active);
        }
        catch (Exception ex)
        {
            LogEventError("activation", ex);
        }
    }

    /* Keys that leave controller mode on, as on the Steam Deck: anything typed while the on-screen keyboard shows,
       the soft keyboard, modifiers, and system, volume, media, headset, camera and power keys */
    private static bool KeepsGamepadActive(KeyEvent e)
    {
        if (GHScreenKeyboard.IsShown)
            return true;
        if (e.DeviceId == (int)KeyboardType.VirtualKeyboard || (e.Flags & KeyEventFlags.SoftKeyboard) != 0)
            return true;
        Keycode keyCode = e.KeyCode;
        if (KeyEvent.IsModifierKey(keyCode) || e.IsSystem)
            return true;
        switch (keyCode)
        {
        case Keycode.VolumeUp:
        case Keycode.VolumeDown:
        case Keycode.VolumeMute:
        case Keycode.Mute:
        case Keycode.MediaPlay:
        case Keycode.MediaPause:
        case Keycode.MediaPlayPause:
        case Keycode.MediaStop:
        case Keycode.MediaNext:
        case Keycode.MediaPrevious:
        case Keycode.MediaRewind:
        case Keycode.MediaFastForward:
        case Keycode.MediaRecord:
        case Keycode.MediaClose:
        case Keycode.MediaEject:
        case Keycode.MediaAudioTrack:
        case Keycode.Headsethook:
        case Keycode.Camera:
        case Keycode.Focus:
        case Keycode.Power:
        case Keycode.Sleep:
        case Keycode.SoftSleep:
        case Keycode.Wakeup:
            return true;
        default:
            return false;
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

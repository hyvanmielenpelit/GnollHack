using System;
using Android.Runtime;
using Android.Views;
using GnollHackX;

namespace GnollHackM;

// WORKAROUND(WA-MAUI-0013): Modal pages on Android lose the activity's immersive mode and key handling
/* Offers a window's key, generic motion and touch events to GHAndroidGamepad before its own callback. MAUI shows
   each modal page in a DialogFragment with its own window, whose events never reach MainActivity. */
/* WindowCallbackWrapper is restricted AndroidX API */
#pragma warning disable XAOBS001
internal sealed class GHAndroidWindowCallback : AndroidX.AppCompat.View.WindowCallbackWrapper
{
    public GHAndroidWindowCallback(Android.Views.Window.ICallback wrapped)
        : base(wrapped)
    {
    }

    private GHAndroidWindowCallback(IntPtr handle, JniHandleOwnership transfer)
        : base(handle, transfer)
    {
    }

    /* Wraps the window's current callback, once; called from Java, so nothing may escape */
    public static void Install(Android.Views.Window window)
    {
        try
        {
            if (window == null)
                return;
            Android.Views.Window.ICallback current = window.Callback;
            if (current == null || current is GHAndroidWindowCallback)
                return;
            window.Callback = new GHAndroidWindowCallback(current);
        }
        catch (Exception ex)
        {
            GHApp.MaybeWriteGHLog("GAMEPAD installing the window callback failed: " + ex.GetType().Name + ": " + ex.Message);
        }
    }

    public override bool DispatchKeyEvent(KeyEvent e)
    {
        if (GHAndroidGamepad.DispatchKeyEvent(e))
            return true;
        return base.DispatchKeyEvent(e);
    }

    public override bool DispatchGenericMotionEvent(MotionEvent e)
    {
        if (GHAndroidGamepad.DispatchGenericMotionEvent(e))
            return true;
        return base.DispatchGenericMotionEvent(e);
    }

    public override bool DispatchTouchEvent(MotionEvent e)
    {
        GHAndroidGamepad.NoteTouch(e);
        return base.DispatchTouchEvent(e);
    }
}
#pragma warning restore XAOBS001

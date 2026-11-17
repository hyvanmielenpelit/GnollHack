using System;
using System.Diagnostics;
using Android.App;
using Android.Content;
using Android.OS;
using Android.Views.InputMethods;
using GnollHackX;
using AndroidView = Android.Views.View;
using MauiView = Microsoft.Maui.Controls.View;

namespace GnollHackM
{
    /* Android's soft keyboard for controller text entry: focuses the field's platform view and asks the input method
       to show itself for it. Available while no hardware keyboard is connected. UI thread only. */
    internal sealed class GHAndroidScreenKeyboard : IGHScreenKeyboardProvider
    {
        /* The view the keyboard was last shown for; it may be in a dialog's window */
        private WeakReference<AndroidView> _shownView;

        public bool IsAvailable
        {
            get { return !MainActivity.IsHardKeyboardConnected; }
        }

        /* True when the field has a platform view; the mode is not used */
        public bool Show(MauiView field, GHScreenKeyboardMode mode)
        {
            return ShowFor(field);
        }

        /* As Show, for the host's platform view (the WebView); the area is not used */
        public bool ShowForArea(MauiView host, double x, double y, double width, double height,
            GHScreenKeyboardMode mode)
        {
            return ShowFor(host);
        }

        /* Hides the keyboard from the window of the view it was shown for, else of the activity's focused view */
        public void Hide()
        {
            AndroidView view = null;
            if (_shownView != null)
                _shownView.TryGetTarget(out view);
            _shownView = null;
            try
            {
                if (view != null && view.Handle == IntPtr.Zero)
                    view = null;
                Android.App.Activity activity = Microsoft.Maui.ApplicationModel.Platform.CurrentActivity ?? MainActivity.CurrentMainActivity;
                if (view == null)
                    view = activity?.CurrentFocus;
                IBinder token = view?.WindowToken;
                if (token == null)
                    return;
                InputMethodManager inputMethodManager = GetInputMethodManager(view.Context ?? activity);
                inputMethodManager?.HideSoftInputFromWindow(token, HideSoftInputFlags.None);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAndroidScreenKeyboard: cannot hide the keyboard: " + ex.Message);
            }
        }

        /* Focuses element's platform view and posts an explicit show request to it; false without a platform view */
        private bool ShowFor(MauiView element)
        {
            AndroidView view = GetPlatformView(element);
            if (view == null)
                return false;
            try
            {
                view.RequestFocus();
                if (!view.Post(() => ShowSoftInput(view)))
                    return false;
                _shownView = new WeakReference<AndroidView>(view);
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAndroidScreenKeyboard: cannot show the keyboard: " + ex.Message);
                return false;
            }
        }

        /* An implicit show is refused when the input method would run in fullscreen extract mode, so the flags are 0 */
        private static void ShowSoftInput(AndroidView view)
        {
            try
            {
                if (view.Handle == IntPtr.Zero)
                    return;
                InputMethodManager inputMethodManager = GetInputMethodManager(view.Context);
                if (inputMethodManager != null && !inputMethodManager.ShowSoftInput(view, (ShowFlags)0))
                    GHApp.MaybeWriteGHLog("GHAndroidScreenKeyboard: the input method did not show the keyboard");
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAndroidScreenKeyboard: cannot show the keyboard: " + ex.Message);
            }
        }

        /* The element's current platform view; handler getters can throw instead of returning null */
        private static AndroidView GetPlatformView(MauiView element)
        {
            if (element == null)
                return null;
            try
            {
                AndroidView view = element.Handler?.PlatformView as AndroidView;
                return view != null && view.Handle != IntPtr.Zero ? view : null;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
                return null;
            }
        }

        private static InputMethodManager GetInputMethodManager(Context context)
        {
            return context?.GetSystemService(Context.InputMethodService) as InputMethodManager;
        }
    }
}

using System;
using System.Diagnostics;
using Foundation;
using GameController;
using GnollHackX;
using UIKit;
using MauiView = Microsoft.Maui.Controls.View;

namespace GnollHackM
{
    /* The iOS software keyboard for controller text entry: makes the field's platform view the first responder, which
       shows the keyboard. Available while no hardware keyboard is connected. UI thread only. */
    internal sealed class GHiOSScreenKeyboard : IGHScreenKeyboardProvider
    {
        private WeakReference<UIView> _shownView;
        private bool _hardKeyboardConnected;
        private NSObject _keyboardConnectObserver;
        private NSObject _keyboardDisconnectObserver;

        public GHiOSScreenKeyboard()
        {
            try
            {
                NSNotificationCenter center = NSNotificationCenter.DefaultCenter;
                _keyboardConnectObserver = center.AddObserver(GCKeyboard.DidConnectNotification, OnKeyboardChanged);
                _keyboardDisconnectObserver = center.AddObserver(GCKeyboard.DidDisconnectNotification, OnKeyboardChanged);
                ReadHardKeyboard();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHiOSScreenKeyboard: cannot watch the hardware keyboard: " + ex.Message);
            }
        }

        public bool IsAvailable
        {
            get { return !_hardKeyboardConnected; }
        }

        /* True when the field's platform view is the first responder; the mode is not used */
        public bool Show(MauiView field, GHScreenKeyboardMode mode)
        {
            return ShowFor(field);
        }

        /* As Show, for the host's platform view (the web view); the area is not used */
        public bool ShowForArea(MauiView host, double x, double y, double width, double height,
            GHScreenKeyboardMode mode)
        {
            return ShowFor(host);
        }

        /* Ends editing in the view the keyboard was shown for, else in the game's window */
        public void Hide()
        {
            UIView view = null;
            if (_shownView != null)
                _shownView.TryGetTarget(out view);
            _shownView = null;
            try
            {
                if (view != null && view.Handle == IntPtr.Zero)
                    view = null;
                if (view == null)
                    view = GHiOSGamepad.GetWindow();
                if (view != null)
                    view.EndEditing(true);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHiOSScreenKeyboard: cannot hide the keyboard: " + ex.Message);
            }
        }

        private bool ShowFor(MauiView element)
        {
            UIView view = GetPlatformView(element);
            if (view == null)
                return false;
            try
            {
                if (!view.IsFirstResponder && !view.BecomeFirstResponder())
                    return false;
                _shownView = new WeakReference<UIView>(view);
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHiOSScreenKeyboard: cannot show the keyboard: " + ex.Message);
                return false;
            }
        }

        /* Called from native code: nothing may escape */
        private void OnKeyboardChanged(NSNotification notification)
        {
            try
            {
                ReadHardKeyboard();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private void ReadHardKeyboard()
        {
            _hardKeyboardConnected = GCKeyboard.CoalescedKeyboard != null;
        }

        /* The element's current platform view; handler getters can throw instead of returning null */
        private static UIView GetPlatformView(MauiView element)
        {
            if (element == null)
                return null;
            try
            {
                UIView view = element.Handler?.PlatformView as UIView;
                return view != null && view.Handle != IntPtr.Zero ? view : null;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                return null;
            }
        }
    }
}

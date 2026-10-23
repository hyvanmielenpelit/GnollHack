using System;
using System.Collections.Generic;
using GnollHackX;
using UIKit;

namespace GnollHackM.Platforms.iOS
{
    /// <summary>
    /// Disables the window-level flex pan gesture recognizers while active. Each Liquid Glass UISwitch and
    /// UISlider (iOS 26+) registers a private _UIFlexInteractionPanGestureRecognizer on its UIWindow, so with
    /// many such controls in one window every touch is arbitrated among all of them, touch handling falls
    /// behind, and quick taps on switches are lost. Recognizers are disabled at each touch-down, before
    /// UIKit dispatches it, and re-enabled on deactivation. UI thread only.
    /// </summary>
    public static class FlexGestureSuppressor
    {
        private const string FlexPanClassName = "_UIFlexInteractionPanGestureRecognizer";

        private static bool _active;
        private static bool _loggedDisable;
        private static readonly List<WeakReference<UIGestureRecognizer>> _disabled = new List<WeakReference<UIGestureRecognizer>>();

        public static bool IsActive => _active;

        public static int DisabledCount
        {
            get
            {
                int count = 0;
                foreach (WeakReference<UIGestureRecognizer> weakRecognizer in _disabled)
                {
                    if (weakRecognizer.TryGetTarget(out UIGestureRecognizer _))
                        count++;
                }
                return count;
            }
        }

        public static void SetActive(bool active)
        {
            try
            {
                if (active == _active)
                    return;

                _active = active;
                if (active)
                {
                    _loggedDisable = false;
                    GHApp.WriteGHLog("FlexGestureSuppressor: active");
                }
                else
                {
                    int reenabled = 0;
                    foreach (WeakReference<UIGestureRecognizer> weakRecognizer in _disabled)
                    {
                        if (weakRecognizer.TryGetTarget(out UIGestureRecognizer recognizer))
                        {
                            recognizer.Enabled = true;
                            reenabled++;
                        }
                    }
                    _disabled.Clear();
                    GHApp.WriteGHLog("FlexGestureSuppressor: inactive, re-enabled " + reenabled);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("FlexGestureSuppressor: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        public static void BeforeEvent(UIEvent evt)
        {
            try
            {
                if (!_active || evt == null || evt.Type != UIEventType.Touches)
                    return;

                Foundation.NSSet allTouches = evt.AllTouches;
                if (allTouches == null)
                    return;

                UIWindow lastWindow = null;
                foreach (UITouch touch in allTouches.ToArray<UITouch>())
                {
                    if (touch.Phase != UITouchPhase.Began)
                        continue;
                    UIWindow window = touch.Window;
                    if (window == null || window == lastWindow)
                        continue;
                    lastWindow = window;
                    DisableOn(window);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("FlexGestureSuppressor: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void DisableOn(UIWindow window)
        {
            UIGestureRecognizer[] recognizers = window.GestureRecognizers;
            if (recognizers == null)
                return;

            int disabledNow = 0;
            foreach (UIGestureRecognizer recognizer in recognizers)
            {
                if (recognizer != null && recognizer.Enabled && recognizer.Class.Name == FlexPanClassName)
                {
                    recognizer.Enabled = false;
                    _disabled.Add(new WeakReference<UIGestureRecognizer>(recognizer));
                    disabledNow++;
                }
            }

            if (disabledNow > 0 && !_loggedDisable)
            {
                _loggedDisable = true;
                GHApp.WriteGHLog("FlexGestureSuppressor: disabled " + disabledNow + " on touch-down");
            }
        }
    }
}

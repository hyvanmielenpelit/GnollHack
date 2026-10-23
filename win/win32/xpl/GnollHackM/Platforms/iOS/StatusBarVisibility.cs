using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using GnollHackX;
using Microsoft.Maui.ApplicationModel;
using Microsoft.Maui.Controls;
using ObjCRuntime;
using UIKit;
using iOSPage = Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific.Page;
using StatusBarHiddenMode = Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific.StatusBarHiddenMode;

namespace GnollHackM.Platforms.iOS
{
    /// <summary>
    /// Shows or hides the status bar through view-controller-based appearance
    /// (UIViewControllerBasedStatusBarAppearance = true); UIApplication.StatusBarHidden has no effect
    /// on iOS 27. Each page gets the iOS-specific PrefersStatusBarHidden value when its handler
    /// connects, and every live page is updated on change. MAUI's ControlsModalWrapper forwards only
    /// the status bar style to its page, so childViewControllerForStatusBarHidden is added to it at
    /// runtime. UI thread only.
    /// </summary>
    public static class StatusBarVisibility
    {
        private const string ModalWrapperTypeName = "Microsoft.Maui.Controls.Platform.ControlsModalWrapper, Microsoft.Maui.Controls";

        private static bool _hidden = true; /* Matches UIStatusBarHidden in Info.plist */
        private static bool _registered;
        private static readonly List<WeakReference<Page>> _pages = new List<WeakReference<Page>>();

        [DllImport(Constants.ObjectiveCLibrary)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool class_addMethod(IntPtr cls, IntPtr name, IntPtr imp, string types);

        public static void Register()
        {
            if (_registered)
                return;
            _registered = true;
            Microsoft.Maui.Handlers.PageHandler.Mapper.AppendToMapping("GHStatusBarHidden", (handler, view) =>
            {
                if (view is Page page)
                {
                    TrackPage(page);
                    ApplyToPage(page);
                }
            });
            AddModalWrapperForwarding();
        }

        public static void SetHidden(bool hidden)
        {
            if (!MainThread.IsMainThread)
            {
                MainThread.BeginInvokeOnMainThread(() => SetHidden(hidden));
                return;
            }
            _hidden = hidden;
            for (int i = _pages.Count - 1; i >= 0; i--)
            {
                if (_pages[i].TryGetTarget(out Page page))
                    ApplyToPage(page);
                else
                    _pages.RemoveAt(i);
            }

            /* The Shell root and each presented wrapper decide, not the pages that changed */
            Application app = Application.Current;
            if (app == null)
                return;
            foreach (Window window in app.Windows)
            {
                if (window.Handler?.PlatformView is UIWindow uiWindow)
                {
                    UIViewController controller = uiWindow.RootViewController;
                    while (controller != null)
                    {
                        controller.SetNeedsStatusBarAppearanceUpdate();
                        controller = controller.PresentedViewController;
                    }
                }
            }
        }

        public static bool IsHidden()
        {
            foreach (UIScene scene in UIApplication.SharedApplication.ConnectedScenes)
            {
                if (scene is UIWindowScene windowScene && windowScene.StatusBarManager != null)
                    return windowScene.StatusBarManager.StatusBarHidden;
            }
            return _hidden;
        }

        private static void TrackPage(Page page)
        {
            for (int i = _pages.Count - 1; i >= 0; i--)
            {
                if (!_pages[i].TryGetTarget(out Page tracked))
                    _pages.RemoveAt(i);
                else if (ReferenceEquals(tracked, page))
                    return;
            }
            _pages.Add(new WeakReference<Page>(page));
        }

        private static void ApplyToPage(Page page)
        {
            iOSPage.SetPrefersStatusBarHidden(page, _hidden ? StatusBarHiddenMode.True : StatusBarHiddenMode.False);
        }

        private static unsafe void AddModalWrapperForwarding()
        {
            try
            {
                Type wrapperType = Type.GetType(ModalWrapperTypeName);
                IntPtr wrapperClass = wrapperType != null ? (IntPtr)Class.GetHandle(wrapperType) : IntPtr.Zero;
                if (wrapperClass == IntPtr.Zero)
                {
                    GHApp.MaybeWriteGHLog("StatusBarVisibility: modal wrapper class not found");
                    return;
                }
                delegate* unmanaged<IntPtr, IntPtr, IntPtr> imp = &ChildViewControllerForStatusBarHidden;
                /* Fails, and changes nothing, when the class implements the selector itself */
                bool added = class_addMethod(wrapperClass, Selector.GetHandle("childViewControllerForStatusBarHidden"), (IntPtr)imp, "@@:");
                GHApp.MaybeWriteGHLog("StatusBarVisibility: modal wrapper forwarding " + (added ? "added" : "already implemented"));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("StatusBarVisibility: modal wrapper forwarding failed: " + ex.Message);
            }
        }

        [UnmanagedCallersOnly]
        private static IntPtr ChildViewControllerForStatusBarHidden(IntPtr self, IntPtr selector)
        {
            try
            {
                UIViewController wrapper = Runtime.GetNSObject<UIViewController>(self);
                UIViewController[] children = wrapper?.ChildViewControllers;
                return children != null && children.Length > 0 ? (IntPtr)children[children.Length - 1].Handle : IntPtr.Zero;
            }
            catch
            {
                return IntPtr.Zero;
            }
        }
    }
}

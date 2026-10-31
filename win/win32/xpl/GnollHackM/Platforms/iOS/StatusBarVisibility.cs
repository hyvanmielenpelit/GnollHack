using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using Foundation;
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

        [DllImport(Constants.ObjectiveCLibrary, EntryPoint = "objc_msgSend")]
        private static extern IntPtr IntPtr_objc_msgSend(IntPtr receiver, IntPtr selector);

        [DllImport(Constants.ObjectiveCLibrary, EntryPoint = "objc_msgSend")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool bool_objc_msgSend(IntPtr receiver, IntPtr selector);

        [DllImport(Constants.ObjectiveCLibrary)]
        private static extern IntPtr object_getClassName(IntPtr obj);

        /// <summary>
        /// The visibility the app asks for, set before the first window exists. IsHidden gives the
        /// state on screen instead.
        /// </summary>
        public static bool RequestedHidden => _hidden;

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
            NSTimer.CreateScheduledTimer(1.0, false, timer => LogState("set " + hidden));

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

        /// <summary>
        /// Logs the Info.plist keys, the scene's status bar state, and, for every window, the view
        /// controllers UIKit consults: each presented controller and its childViewControllerForStatusBarHidden
        /// chain, with the prefersStatusBarHidden answer each gives. Queried through objc_msgSend, so
        /// methods added at runtime are included.
        /// </summary>
        public static void LogState(string reason)
        {
            try
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("StatusBarVisibility state (").Append(reason).Append("): iOS ").Append(UIDevice.CurrentDevice.SystemVersion);
                sb.Append(", idiom=").Append(UIDevice.CurrentDevice.UserInterfaceIdiom);
                sb.Append(", hidden=").Append(_hidden).Append(", pages=").Append(_pages.Count);
                NSBundle bundle = NSBundle.MainBundle;
                sb.Append(", plist VCBased=").Append(bundle.ObjectForInfoDictionary("UIViewControllerBasedStatusBarAppearance")?.ToString() ?? "null");
                sb.Append(", plist Hidden=").Append(bundle.ObjectForInfoDictionary("UIStatusBarHidden")?.ToString() ?? "null");
                Page mainPage = GHApp.CurrentMainPage;
                if (mainPage != null)
                    sb.Append(", MainPage mode=").Append(iOSPage.GetPrefersStatusBarHidden(mainPage));
                foreach (UIScene scene in UIApplication.SharedApplication.ConnectedScenes)
                {
                    if (!(scene is UIWindowScene windowScene))
                        continue;
                    sb.Append(" | scene");
                    UIStatusBarManager manager = windowScene.StatusBarManager;
                    if (manager != null)
                        sb.Append(" manager hidden=").Append(manager.StatusBarHidden).Append(", frame=").Append(manager.StatusBarFrame);
                    foreach (UIWindow uiWindow in windowScene.Windows)
                    {
                        sb.Append(" | window ").Append(ClassName(uiWindow.Handle)).Append(" key=").Append(uiWindow.IsKeyWindow);
                        sb.Append(" level=").Append(uiWindow.WindowLevel).Append(" hidden=").Append(uiWindow.Hidden);
                        sb.Append(" bounds=").Append(uiWindow.Bounds.Size).Append(" vsize=").Append(uiWindow.TraitCollection.VerticalSizeClass);
                        sb.Append(" safetop=").Append(uiWindow.SafeAreaInsets.Top);
                        AppendControllerChain(sb, uiWindow.RootViewController);
                    }
                }
                GHApp.MaybeWriteGHLog(sb.ToString());
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("StatusBarVisibility state (" + reason + ") failed: " + ex.Message);
            }
        }

        private static void AppendControllerChain(StringBuilder sb, UIViewController root)
        {
            IntPtr prefersSelector = Selector.GetHandle("prefersStatusBarHidden");
            IntPtr childSelector = Selector.GetHandle("childViewControllerForStatusBarHidden");
            UIViewController presented = root;
            int presentedDepth = 0;
            while (presented != null && presentedDepth < 16)
            {
                sb.Append(" => ");
                IntPtr controller = presented.Handle;
                int childDepth = 0;
                while (controller != IntPtr.Zero && childDepth < 8)
                {
                    sb.Append(ClassName(controller)).Append("[prefers=").Append(bool_objc_msgSend(controller, prefersSelector)).Append("]");
                    IntPtr child = IntPtr_objc_msgSend(controller, childSelector);
                    if (child == IntPtr.Zero || child == controller)
                        break;
                    sb.Append(" child ");
                    controller = child;
                    childDepth++;
                }
                sb.Append(" style=").Append(presented.ModalPresentationStyle);
                sb.Append(" captures=").Append(presented.ModalPresentationCapturesStatusBarAppearance);
                presented = presented.PresentedViewController;
                presentedDepth++;
            }
        }

        private static string ClassName(IntPtr obj)
        {
            return Marshal.PtrToStringAnsi(object_getClassName(obj)) ?? "?";
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

        // WORKAROUND(WA-MAUI-0004): ControlsModalWrapper does not forward status bar visibility to its page
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

        // WORKAROUND(WA-MAUI-0004): ControlsModalWrapper does not forward status bar visibility to its page
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

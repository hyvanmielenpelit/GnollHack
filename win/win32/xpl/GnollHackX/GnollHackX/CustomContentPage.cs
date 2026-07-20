using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

#if GNH_MAUI
#if WINDOWS
using System.Reflection;
using Microsoft.Maui.Platform;
#endif
using GnollHackX;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace GnollHackX
#endif
{
    public partial class CustomContentPage : ContentPage
    {
        public CustomContentPage() : base()
        {
            UpdateEdge2Edge();
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            /* Content has not been set yet, as InitializeComponent has not run */
        }

        protected override void OnParentSet()
        {
            base.OnParentSet();
            if (Content != null)
            {
                UIUtils.SetViewCursorOnHandler(Content, GameCursorType.Normal);
                UpdateEdge2Edge();
            }
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();
#if GNH_MAUI
#if IOS
            /* Re-apply on every appearance: the Handler is guaranteed to exist
               by this point, and safe area insets may have changed (rotation). */
            ApplyiOSEdge2Edge(GHApp.Edge2Edge);
#endif
#endif
        }

        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
#if GNH_MAUI
#if IOS
            /* After rotation the system safe area insets change,
               so re-apply the negative override to stay edge-to-edge. */
            ApplyiOSEdge2Edge(GHApp.Edge2Edge);
#endif
#endif
        }

        public void UpdateEdge2Edge()
        {
            SetEdge2Edge(GHApp.Edge2Edge);
        }

        public void SetEdge2Edge(bool newEdge2Edge)
        {
#if GNH_MAUI
            /* Keep the MAUI property as belt-and-suspenders in case a future
               MAUI update fixes SafeAreaEdges propagation on iOS. */
            SafeAreaEdges = newEdge2Edge ? SafeAreaEdges.None : SafeAreaEdges.All;
#if IOS
            ApplyiOSEdge2Edge(newEdge2Edge);
#endif
#else
            On<iOS>().SetUseSafeArea(!newEdge2Edge);
#endif
        }

#if GNH_MAUI
#if IOS
        /// <summary>
        /// Applies or removes negative AdditionalSafeAreaInsets on the
        /// UIViewController hierarchy, forcing content to extend
        /// behind the notch and home indicator.
        /// Works for both main pages (under RootViewController) and
        /// modal pages (presented with their own VC hierarchy).
        /// </summary>
        private void ApplyiOSEdge2Edge(bool edge2Edge)
        {
            UIKit.UIWindow window = null;
            foreach (var scene in UIKit.UIApplication.SharedApplication.ConnectedScenes)
            {
                if (scene is UIKit.UIWindowScene windowScene)
                {
                    foreach (var w in windowScene.Windows)
                    {
                        if (w.IsKeyWindow)
                        {
                            window = w;
                            break;
                        }
                    }
                    if (window != null)
                        break;
                }
            }
            if (window == null)
                return;

            var safeArea = window.SafeAreaInsets;
            var insets = edge2Edge
                ? new UIKit.UIEdgeInsets(-safeArea.Top, -safeArea.Left, -safeArea.Bottom, -safeArea.Right)
                : UIKit.UIEdgeInsets.Zero;

            /* Apply to the window's RootViewController and its children
               (covers the main non-modal page hierarchy). */
            var rootVC = window.RootViewController;
            if (rootVC != null)
            {
                ApplyInsetsToVCTree(rootVC, insets);

                /* Also walk any modally-presented VC chain from the root.
                   Each modal presentation creates a new VC that is separate
                   from the root's child hierarchy. */
                var presented = rootVC.PresentedViewController;
                while (presented != null)
                {
                    ApplyInsetsToVCTree(presented, insets);
                    presented = presented.PresentedViewController;
                }
            }

            /* Additionally, apply to this page's own VC via its Handler,
               in case it wasn't reached by the tree walk above. */
            if (this.Handler is Microsoft.Maui.IPlatformViewHandler pvh
                && pvh.ViewController != null)
            {
                var pageVC = pvh.ViewController;
                pageVC.AdditionalSafeAreaInsets = insets;
                /* Walk up the page VC's parent chain within its modal context */
                var parent = pageVC.ParentViewController;
                while (parent != null)
                {
                    parent.AdditionalSafeAreaInsets = insets;
                    parent = parent.ParentViewController;
                }
            }
        }

        /// <summary>
        /// Applies AdditionalSafeAreaInsets to a VC and all its direct children.
        /// </summary>
        private static void ApplyInsetsToVCTree(UIKit.UIViewController vc, UIKit.UIEdgeInsets insets)
        {
            vc.AdditionalSafeAreaInsets = insets;
            if (vc.ChildViewControllers != null)
            {
                foreach (var child in vc.ChildViewControllers)
                    child.AdditionalSafeAreaInsets = insets;
            }
        }
#endif
#endif

        public void UpdateDarkMode()
        {
            SetDarkMode(GHApp.DarkMode);
        }

        public void SetDarkMode(bool newDarkMode)
        {
            UIUtils.SetPageThemeAndTransitions(this, newDarkMode);
        }
    }
}

using CoreGraphics;
using GnollHackX;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;
using UIKit;

namespace GnollHackM.Platforms.iOS
{
    public class FocusKeepingScrollViewHandler : ScrollViewHandler
    {
        // WORKAROUND(WA-MAUI-0007): Focused input is not kept visible when the page shrinks above the keyboard
        /// <summary>
        /// Keeps the focused input visible when the scroll view gets shorter, as Android's ScrollView does.
        /// MAUI shrinks SafeAreaEdges.All pages above the keyboard but does not scroll on iOS, and UIKit only
        /// scrolls a field into view when it gains focus, before the shrink.
        /// </summary>
        private class FocusKeepingScrollView : MauiScrollView
        {
            private const double BottomSpacing = 8;
            private nfloat _lastHeight;

            public override void LayoutSubviews()
            {
                base.LayoutSubviews();

                nfloat height = Bounds.Height;
                bool shrank = _lastHeight > 0 && height < _lastHeight;
                _lastHeight = height;
                if (!shrank)
                    return;

                UIView focused = FindFirstResponder(this);
                if (focused != null)
                    ScrollToVisible(ConvertRectFromView(focused.Bounds, focused), true);
            }

            public override void ScrollRectToVisible(CGRect rect, bool animated)
            {
                /* In edge-to-edge mode KeyboardAutoManagerScroll owns keyboard scrolling */
                if (GHApp.Edge2Edge)
                    base.ScrollRectToVisible(rect, animated);
                else
                    ScrollToVisible(rect, animated);
            }

            /* MauiScrollView ignores ScrollRectToVisible while the keyboard is up, so the offset is set directly */
            private void ScrollToVisible(CGRect rect, bool animated)
            {
                UIEdgeInsets insets = AdjustedContentInset;
                nfloat visibleTop = ContentOffset.Y + insets.Top;
                nfloat visibleBottom = ContentOffset.Y + Bounds.Height - insets.Bottom;

                /* The floating close button hides what lies under it in its column */
                CGRect button = KeyboardDoneOverlay.ButtonFrame;
                if (!button.IsEmpty && Window != null)
                {
                    CGRect buttonRect = ConvertRectFromView(button, null);
                    if (rect.Right > buttonRect.X && rect.X < buttonRect.Right)
                        visibleBottom = nfloat.Min(visibleBottom, buttonRect.Y);
                }

                nfloat delta;
                if (rect.Bottom + BottomSpacing > visibleBottom)
                    delta = rect.Bottom + (nfloat)BottomSpacing - visibleBottom;
                else if (rect.Y < visibleTop)
                    delta = rect.Y - visibleTop;
                else
                    return;

                nfloat minY = -insets.Top;
                nfloat maxY = nfloat.Max(minY, ContentSize.Height + insets.Bottom - Bounds.Height);
                nfloat offsetY = nfloat.Min(nfloat.Max(ContentOffset.Y + delta, minY), maxY);
                if (offsetY != ContentOffset.Y)
                    SetContentOffset(new CGPoint(ContentOffset.X, offsetY), animated);
            }

            private static UIView FindFirstResponder(UIView view)
            {
                if (view.IsFirstResponder)
                    return view;

                foreach (UIView subview in view.Subviews)
                {
                    UIView found = FindFirstResponder(subview);
                    if (found != null)
                        return found;
                }
                return null;
            }
        }

        protected override UIScrollView CreatePlatformView()
        {
            return new FocusKeepingScrollView();
        }
    }
}

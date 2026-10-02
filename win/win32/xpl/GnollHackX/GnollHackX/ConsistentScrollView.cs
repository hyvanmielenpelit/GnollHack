using System;
#if GNH_MAUI
using Microsoft.Maui.Handlers;
#if ANDROID
using Android.Views;
using Microsoft.Maui.Platform;
using Microsoft.Maui.Primitives;
#endif
namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    /* Sizes to its content, up to the available space, on every platform */
    public class ConsistentScrollView : ScrollView
    {
    }

#if GNH_MAUI && ANDROID
    /* The stock Android handler measures a Fill-aligned ScrollView at the full constraint; this one
       measures to content as iOS and Windows do, and measures at the final frame when arranged so
       that FillViewport still stretches short content */
    internal class ConsistentScrollViewHandler : ScrollViewHandler
    {
        public override Size GetDesiredSize(double widthConstraint, double heightConstraint)
        {
            var context = MauiContext?.Context;
            var platformView = PlatformView;
            var virtualView = VirtualView;
            if (context == null || platformView == null || virtualView == null)
                return Size.Zero;

            int widthSpec = CreateMeasureSpec(context, widthConstraint, virtualView.Width, virtualView.MinimumWidth, virtualView.MaximumWidth);
            int heightSpec = CreateMeasureSpec(context, heightConstraint, virtualView.Height, virtualView.MinimumHeight, virtualView.MaximumHeight);
            platformView.Measure(widthSpec, heightSpec);
            return context.FromPixels(platformView.MeasuredWidth, platformView.MeasuredHeight);
        }

        public override void PlatformArrange(Rect frame)
        {
            var context = MauiContext?.Context;
            var platformView = PlatformView;
            if (context != null && platformView != null && frame.Width >= 0 && frame.Height >= 0)
            {
                /* Same pixel rounding as MAUI's PlatformArrangeHandler */
                var (left, top, right, bottom) = context.ToPixels(frame);
                int width = right - left;
                int height = bottom - top;
                if (platformView.MeasuredWidth != width || platformView.MeasuredHeight != height)
                {
                    platformView.Measure(
                        global::Android.Views.View.MeasureSpec.MakeMeasureSpec(width, MeasureSpecMode.Exactly),
                        global::Android.Views.View.MeasureSpec.MakeMeasureSpec(height, MeasureSpecMode.Exactly));
                }
            }
            base.PlatformArrange(frame);
        }

        /* MAUI's internal ContextExtensions.CreateMeasureSpec */
        private static int CreateMeasureSpec(global::Android.Content.Context context, double constraint, double explicitSize, double minimumSize, double maximumSize)
        {
            MeasureSpecMode mode = MeasureSpecMode.AtMost;
            if (Dimension.IsExplicitSet(explicitSize))
            {
                mode = MeasureSpecMode.Exactly;
                constraint = Math.Max(explicitSize, Dimension.ResolveMinimum(minimumSize));
                if (Dimension.IsMaximumSet(maximumSize))
                    constraint = Math.Min(constraint, maximumSize);
            }
            else if (Dimension.IsMaximumSet(maximumSize) && maximumSize < constraint)
            {
                constraint = maximumSize;
            }
            else if (double.IsInfinity(constraint))
            {
                mode = MeasureSpecMode.Unspecified;
                constraint = 0;
            }
            return global::Android.Views.View.MeasureSpec.MakeMeasureSpec((int)context.ToPixels(constraint), mode);
        }
    }
#endif
}

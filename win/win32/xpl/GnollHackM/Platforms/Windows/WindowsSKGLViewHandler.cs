#if WINDOWS
#nullable enable

using Microsoft.Maui;
using SkiaSharp;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Handlers;
using SkiaSharp.Views.Windows;

namespace GnollHackM
{
    /* SkiaSharp's SKGLViewHandler with WindowsTouchHandler in place of its own touch handler */
    public class WindowsSKGLViewHandler : SKGLViewHandler
    {
        public static PropertyMapper<ISKGLView, SKGLViewHandler> WindowsSKGLViewMapper =
            new PropertyMapper<ISKGLView, SKGLViewHandler>(SKGLViewMapper)
            {
                [nameof(ISKGLView.EnableTouchEvents)] = MapWindowsEnableTouchEvents,
            };

        private WindowsTouchHandler? _touchHandler;

        public WindowsSKGLViewHandler() : base(WindowsSKGLViewMapper, SKGLViewCommandMapper)
        {
        }

        protected override void DisconnectHandler(SKSwapChainPanel platformView)
        {
            _touchHandler?.Detach(platformView);
            _touchHandler = null;
            base.DisconnectHandler(platformView);
        }

        public static void MapWindowsEnableTouchEvents(SKGLViewHandler handler, ISKGLView view)
        {
            if (handler is not WindowsSKGLViewHandler h || h.PlatformView == null)
                return;

            if (h._touchHandler == null)
            {
                if (!view.EnableTouchEvents)
                    return;

                h._touchHandler = new WindowsTouchHandler(
                    args => h.VirtualView?.OnTouch(args),
                    (x, y) => h.GetScaledCoord(x, y));
            }
            h._touchHandler.SetEnabled(h.PlatformView, view.EnableTouchEvents);
        }

        private SKPoint GetScaledCoord(double x, double y)
        {
            if (VirtualView != null && !VirtualView.IgnorePixelScaling && PlatformView != null)
            {
                double scale = PlatformView.ContentsScale;
                x *= scale;
                y *= scale;
            }
            return new SKPoint((float)x, (float)y);
        }
    }
}
#endif

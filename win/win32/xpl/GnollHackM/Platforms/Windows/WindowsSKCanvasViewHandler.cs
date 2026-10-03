#if WINDOWS
#nullable enable

using Microsoft.Maui;
using SkiaSharp;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Handlers;
using SkiaSharp.Views.Windows;

namespace GnollHackM
{
    /* SkiaSharp's SKCanvasViewHandler with WindowsTouchHandler in place of its own touch handler */
    public class WindowsSKCanvasViewHandler : SKCanvasViewHandler
    {
        public static PropertyMapper<ISKCanvasView, SKCanvasViewHandler> WindowsSKCanvasViewMapper =
            new PropertyMapper<ISKCanvasView, SKCanvasViewHandler>(SKCanvasViewMapper)
            {
                [nameof(ISKCanvasView.EnableTouchEvents)] = MapWindowsEnableTouchEvents,
            };

        private WindowsTouchHandler? _touchHandler;

        public WindowsSKCanvasViewHandler() : base(WindowsSKCanvasViewMapper, SKCanvasViewCommandMapper)
        {
        }

        protected override void DisconnectHandler(SKXamlCanvas platformView)
        {
            _touchHandler?.Detach(platformView);
            _touchHandler = null;
            base.DisconnectHandler(platformView);
        }

        public static void MapWindowsEnableTouchEvents(SKCanvasViewHandler handler, ISKCanvasView view)
        {
            if (handler is not WindowsSKCanvasViewHandler h || h.PlatformView == null)
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
                double dpi = PlatformView.Dpi;
                x *= dpi;
                y *= dpi;
            }
            return new SKPoint((float)x, (float)y);
        }
    }
}
#endif

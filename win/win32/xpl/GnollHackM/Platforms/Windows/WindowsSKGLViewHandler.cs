#if WINDOWS
#nullable enable

using GnollHackX;
using Microsoft.Maui;
using SkiaSharp;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Handlers;
using SkiaSharp.Views.Windows;

namespace GnollHackM
{
    // WORKAROUND(WA-SKIASHARP-0007): SkiaSharp's Windows touch handler is internal and reads every WinRT object
    /* SkiaSharp's SKGLViewHandler with WindowsTouchHandler in place of its own touch handler */
    public class WindowsSKGLViewHandler : SKGLViewHandler
    {
        public static PropertyMapper<ISKGLView, SKGLViewHandler> WindowsSKGLViewMapper =
            new PropertyMapper<ISKGLView, SKGLViewHandler>(SKGLViewMapper)
            {
                [nameof(ISKGLView.EnableTouchEvents)] = MapWindowsEnableTouchEvents,
            };

        private WindowsTouchHandler? _touchHandler;

        /* True once a panel of this handler type has painted and VerifyCurrentDisplay has run; UI thread */
        private static bool _displayChecked = false;
        private bool _displayCheckSubscribed = false;

        public WindowsSKGLViewHandler() : base(WindowsSKGLViewMapper, SKGLViewCommandMapper)
        {
        }

        // WORKAROUND(WA-SKIASHARP-0012): SKSwapChainPanel cannot choose its Direct3D adapter
        /* The adapter is chosen before the panel exists; SkiaSharp creates its display later, in the panel's Loaded */
        protected override SKSwapChainPanel CreatePlatformView()
        {
            if (GHApp.UseGPU)
                GHAngleAdapterSelection.EnsureSelected();
            return base.CreatePlatformView();
        }

        protected override void ConnectHandler(SKSwapChainPanel platformView)
        {
            base.ConnectHandler(platformView);
            if (!_displayChecked && !_displayCheckSubscribed)
            {
                platformView.PaintSurface += OnFirstPaintSurface;
                _displayCheckSubscribed = true;
            }
        }

        protected override void DisconnectHandler(SKSwapChainPanel platformView)
        {
            if (_displayCheckSubscribed)
            {
                platformView.PaintSurface -= OnFirstPaintSurface;
                _displayCheckSubscribed = false;
            }
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

        /* The first panel to paint is the map: the menu and text panels stay hidden on Windows */
        private void OnFirstPaintSurface(object? sender, SkiaSharp.Views.Windows.SKPaintGLSurfaceEventArgs e)
        {
            if (sender is SKSwapChainPanel panel)
                panel.PaintSurface -= OnFirstPaintSurface;
            _displayCheckSubscribed = false;
            if (_displayChecked)
                return;
            _displayChecked = true;
            GHAngleAdapterSelection.VerifyCurrentDisplay();
        }
    }
}
#endif

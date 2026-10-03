#if WINDOWS
#nullable enable

using System;
using Microsoft.Maui;
using Microsoft.Maui.Handlers;
using SkiaSharp;
using SkiaSharp.Views.Maui;

namespace GnollHackM
{
    /* An ISKGLView handler drawing into an SKCompositionGLPanel, whose content blends with the
       XAML around it. ViewHandler's VirtualView and PlatformView getters throw when unset, so
       the IElementHandler views are read where the handler may be disconnected. */
    public class WindowsSKGLCompositionViewHandler : ViewHandler<ISKGLView, SKCompositionGLPanel>
    {
        public static PropertyMapper<ISKGLView, WindowsSKGLCompositionViewHandler> WindowsSKGLCompositionViewMapper =
            new PropertyMapper<ISKGLView, WindowsSKGLCompositionViewHandler>(ViewHandler.ViewMapper)
            {
                [nameof(ISKGLView.EnableTouchEvents)] = MapEnableTouchEvents,
                [nameof(ISKGLView.IgnorePixelScaling)] = MapIgnorePixelScaling,
                [nameof(ISKGLView.HasRenderLoop)] = MapHasRenderLoop,
                [nameof(IView.Background)] = MapBackground,
            };

        public static CommandMapper<ISKGLView, WindowsSKGLCompositionViewHandler> WindowsSKGLCompositionViewCommandMapper =
            new CommandMapper<ISKGLView, WindowsSKGLCompositionViewHandler>(ViewHandler.ViewCommandMapper)
            {
                [nameof(ISKGLView.InvalidateSurface)] = OnInvalidateSurface,
            };

        private WindowsTouchHandler? _touchHandler;
        private SKSizeI lastCanvasSize;
        private GRContext? lastGRContext;

        public WindowsSKGLCompositionViewHandler()
            : base(WindowsSKGLCompositionViewMapper, WindowsSKGLCompositionViewCommandMapper)
        {
        }

        private ISKGLView? CurrentVirtualView
        {
            get { return ((IElementHandler)this).VirtualView as ISKGLView; }
        }

        private SKCompositionGLPanel? CurrentPlatformView
        {
            get { return ((IElementHandler)this).PlatformView as SKCompositionGLPanel; }
        }

        protected override SKCompositionGLPanel CreatePlatformView()
        {
            return new SKCompositionGLPanel();
        }

        protected override void ConnectHandler(SKCompositionGLPanel platformView)
        {
            platformView.PaintSurface += OnPaintSurface;
            platformView.GRContextChanged += OnGRContextChanged;
            base.ConnectHandler(platformView);
        }

        protected override void DisconnectHandler(SKCompositionGLPanel platformView)
        {
            _touchHandler?.Detach(platformView);
            _touchHandler = null;
            platformView.PaintSurface -= OnPaintSurface;
            platformView.GRContextChanged -= OnGRContextChanged;
            platformView.DisposeResources();
            CurrentVirtualView?.OnGRContextChanged(null);
            lastGRContext = null;
            lastCanvasSize = default;
            base.DisconnectHandler(platformView);
        }

        private void OnPaintSurface(object? sender, SKPaintGLSurfaceEventArgs e)
        {
            ISKGLView? virtualView = CurrentVirtualView;
            if (virtualView == null)
                return;

            SKSizeI newCanvasSize = e.Info.Size;
            if (lastCanvasSize != newCanvasSize)
            {
                lastCanvasSize = newCanvasSize;
                virtualView.OnCanvasSizeChanged(newCanvasSize);
            }
            if (sender is SKCompositionGLPanel platformView)
            {
                GRContext? newGRContext = platformView.GRContext;
                if (lastGRContext != newGRContext)
                {
                    lastGRContext = newGRContext;
                    virtualView.OnGRContextChanged(newGRContext);
                }
            }

            virtualView.OnPaintSurface(e);
        }

        /* Device loss reports null; the rebuild reports the new context, and the panel
           invalidates itself */
        private void OnGRContextChanged(object? sender, EventArgs e)
        {
            ISKGLView? virtualView = CurrentVirtualView;
            if (virtualView == null || sender is not SKCompositionGLPanel platformView)
                return;

            GRContext? newGRContext = platformView.GRContext;
            if (lastGRContext != newGRContext)
            {
                lastGRContext = newGRContext;
                virtualView.OnGRContextChanged(newGRContext);
            }
        }

        public static void OnInvalidateSurface(WindowsSKGLCompositionViewHandler handler, ISKGLView view, object? args)
        {
            handler?.CurrentPlatformView?.Invalidate();
        }

        public static void MapIgnorePixelScaling(WindowsSKGLCompositionViewHandler handler, ISKGLView view)
        {
            SKCompositionGLPanel? platformView = handler?.CurrentPlatformView;
            if (platformView == null)
                return;

            platformView.IgnorePixelScaling = view.IgnorePixelScaling;
            platformView.Invalidate();
        }

        public static void MapHasRenderLoop(WindowsSKGLCompositionViewHandler handler, ISKGLView view)
        {
        }

        /* The default background mapping would replace the panel's transparent brush, which
           the panel needs for hit testing */
        public static void MapBackground(WindowsSKGLCompositionViewHandler handler, ISKGLView view)
        {
        }

        public static void MapEnableTouchEvents(WindowsSKGLCompositionViewHandler handler, ISKGLView view)
        {
            SKCompositionGLPanel? platformView = handler?.CurrentPlatformView;
            if (handler == null || platformView == null)
                return;

            if (handler._touchHandler == null)
            {
                if (!view.EnableTouchEvents)
                    return;

                handler._touchHandler = new WindowsTouchHandler(
                    args => handler.CurrentVirtualView?.OnTouch(args),
                    (x, y) => handler.GetScaledCoord(x, y));
            }
            handler._touchHandler.SetEnabled(platformView, view.EnableTouchEvents);
        }

        private SKPoint GetScaledCoord(double x, double y)
        {
            ISKGLView? virtualView = CurrentVirtualView;
            SKCompositionGLPanel? platformView = CurrentPlatformView;
            if (virtualView != null && !virtualView.IgnorePixelScaling && platformView != null)
            {
                double scale = platformView.ContentsScale;
                x *= scale;
                y *= scale;
            }
            return new SKPoint((float)x, (float)y);
        }
    }
}
#endif

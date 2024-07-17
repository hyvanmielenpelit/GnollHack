#if IOS
using PlatformView = SkiaSharp.Views.iOS.SKMetalView;
using SkiaSharp.Views.iOS;
using UIKit;
#elif MACCATALYST
using PlatformView = SkiaSharp.Views.iOS.SKCanvasView;
using SkiaSharp.Views.iOS;
using UIKit;
#elif ANDROID
using PlatformView = SkiaSharp.Views.Android.SKSurfaceView;
using SkiaSharp.Views.Android;
#elif WINDOWS
using PlatformView = SkiaSharp.Views.Windows.SKXamlCanvas;
using SkiaSharp.Views.Windows;
#elif (NETSTANDARD || !PLATFORM) || (NET6_0_OR_GREATER && !IOS && !ANDROID)
using PlatformView = System.Object;
#endif

using System;
using Microsoft.Maui.Handlers;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Platform;
using Microsoft.Maui.Controls.PlatformConfiguration;
using SkiaSharp.Views.Maui.Handlers;
using SkiaSharp.Views.Maui.Controls;
using SkiaSharp;
using Microsoft.Maui;

namespace GnollHackM.Platforms.iOS
{
    public partial class SKAcceleratedCanvasViewHandler : ViewHandler<ISKGLView, PlatformView>
    {
        private SKSizeI lastCanvasSize;
        private GRContext? lastGRContext;
#if IOS
        private GnollHackM.Platforms.iOS.SKTouchHandler? touchHandler;
#endif
        protected override PlatformView CreatePlatformView() =>
#if IOS
            new MauiSKMetalView
            {
                BackgroundColor = UIColor.Clear,
                Opaque = false,
            };
#elif MACCATALYST
            new SkiaSharp.Views.iOS.SKCanvasView();
#elif ANDROID
            new SKSurfaceView(Context);
#elif WINDOWS
            new SKXamlCanvas();
#else
            new object();
#endif

        protected override void ConnectHandler(PlatformView platformView)
        {
#if IOS
            platformView.PaintSurface += OnPaintSurface;
#endif
            base.ConnectHandler(platformView);
        }

        protected override void DisconnectHandler(PlatformView platformView)
        {
#if IOS
            touchHandler?.Detach(platformView);
            touchHandler = null;

            platformView.PaintSurface -= OnPaintSurface;

            base.DisconnectHandler(platformView);
#endif
        }

        // Mapper actions / properties

        public static void OnInvalidateSurface(SKAcceleratedCanvasViewHandler handler, ISKGLView view, object? args)
        {
#if IOS
            //if (handler.PlatformView.Paused && handler.PlatformView.EnableSetNeedsDisplay)
            //    handler.PlatformView.SetNeedsDisplay();
#endif
        }

        public static void MapIgnorePixelScaling(SKAcceleratedCanvasViewHandler handler, ISKGLView view)
        {
#if IOS
            if (handler.PlatformView is MauiSKMetalView pv)
            {
                pv.IgnorePixelScaling = view.IgnorePixelScaling;
                handler.PlatformView.SetNeedsDisplay();
            }
#endif
        }

        public static void MapHasRenderLoop(SKAcceleratedCanvasViewHandler handler, ISKGLView view)
        {
#if IOS
            //handler.PlatformView.Paused = !view.HasRenderLoop;
            handler.PlatformView.EnableSetNeedsDisplay = !view.HasRenderLoop;
#endif
        }

        public static void MapEnableTouchEvents(SKAcceleratedCanvasViewHandler handler, ISKGLView view)
        {
#if IOS
            handler.touchHandler ??= new GnollHackM.Platforms.iOS.SKTouchHandler(
                args => view.OnTouch(args),
                (x, y) => handler.OnGetScaledCoord(x, y));

            handler.touchHandler?.SetEnabled(handler.PlatformView, view.EnableTouchEvents);
#endif
        }

        // helper methods

#if IOS
        private void OnPaintSurface(object? sender, SkiaSharp.Views.iOS.SKPaintMetalSurfaceEventArgs e)
        {
            var newCanvasSize = e.Info.Size;
            if (lastCanvasSize != newCanvasSize)
            {
                lastCanvasSize = newCanvasSize;
                VirtualView?.OnCanvasSizeChanged(newCanvasSize);
            }
            if (sender is SKMetalView platformView)
            {
                var newGRContext = platformView.GRContext;
                if (lastGRContext != newGRContext)
                {
                    lastGRContext = newGRContext;
                    VirtualView?.OnGRContextChanged(newGRContext);
                }
            }

            VirtualView?.OnPaintSurface(new SkiaSharp.Views.Maui.SKPaintGLSurfaceEventArgs(e.Surface, e.BackendRenderTarget, e.Origin, e.Info, e.RawInfo));
        }
#endif

        private SKPoint OnGetScaledCoord(double x, double y)
        {
#if IOS
            if (VirtualView?.IgnorePixelScaling == false && PlatformView != null)
            {
                var scale = PlatformView.ContentScaleFactor;

                x *= scale;
                y *= scale;
            }
#endif
            return new SKPoint((float)x, (float)y);
        }

#if IOS
        private class MauiSKMetalView : PlatformView
        {
            public bool IgnorePixelScaling { get; set; }

            protected override void OnPaintSurface(SkiaSharp.Views.iOS.SKPaintMetalSurfaceEventArgs e)
            {
                if (IgnorePixelScaling)
                {
                    var userVisibleSize = new SKSizeI((int)Bounds.Width, (int)Bounds.Height);
                    var canvas = e.Surface.Canvas;
                    canvas.Scale((float)ContentScaleFactor);
                    canvas.Save();

                    e = new SkiaSharp.Views.iOS.SKPaintMetalSurfaceEventArgs(e.Surface, e.BackendRenderTarget, e.Origin, e.Info.WithSize(userVisibleSize), e.Info);
                }

                base.OnPaintSurface(e);
            }
        }
#endif

        public static IPropertyMapper<ISKGLView, SKAcceleratedCanvasViewHandler> PropertyMapper = 
            new PropertyMapper<ISKGLView, SKAcceleratedCanvasViewHandler>(ViewHandler.ViewMapper)
        {
            [nameof(ISKGLView.EnableTouchEvents)] = MapEnableTouchEvents,
            [nameof(ISKGLView.IgnorePixelScaling)] = MapIgnorePixelScaling,
            [nameof(ISKGLView.HasRenderLoop)] = MapHasRenderLoop,
        };

        public static CommandMapper<ISKGLView, SKAcceleratedCanvasViewHandler> CommandMapper =
            new CommandMapper<ISKGLView, SKAcceleratedCanvasViewHandler>(ViewHandler.ViewCommandMapper)
            {
                [nameof(ISKGLView.InvalidateSurface)] = OnInvalidateSurface,
            };

        public SKAcceleratedCanvasViewHandler() : base(PropertyMapper, CommandMapper)
        {

        }

        public SKAcceleratedCanvasViewHandler(PropertyMapper mapper, CommandMapper commands)
            : base(mapper, commands ?? CommandMapper)
        {

        }
    }
}

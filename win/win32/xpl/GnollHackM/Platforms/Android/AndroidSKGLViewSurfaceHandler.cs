#nullable enable

using System;
using Android.Content;
using Android.Opengl;
using Android.Views;
using GnollHackX;
using Microsoft.Maui;
using Microsoft.Maui.Handlers;
using Microsoft.Maui.Platform;
using SkiaSharp;
using SkiaSharp.Views.Android;
using SkiaSharp.Views.Maui;
using AView = Android.Views.View;

namespace GnollHackM
{
    /* SKGLView on an SKGLSurfaceView: the map is its own SurfaceFlinger layer instead of a
       TextureView that HWUI composites into the page's window. The layer sits behind the
       window, so views above it draw normally; the view's own opacity, transforms and
       clipping do not apply to the layer. */
    public class AndroidSKGLViewSurfaceHandler : ViewHandler<ISKGLView, SKGLSurfaceView>
    {
        private SKSizeI _lastCanvasSize;
        private GRContext? _lastGRContext;
        private SurfaceTouchHandler? _touchHandler;

        public AndroidSKGLViewSurfaceHandler() : base(SKGLViewMapper, SKGLViewCommandMapper)
        {
        }

        public static PropertyMapper<ISKGLView, AndroidSKGLViewSurfaceHandler> SKGLViewMapper =
            new PropertyMapper<ISKGLView, AndroidSKGLViewSurfaceHandler>(ViewHandler.ViewMapper)
            {
                [nameof(ISKGLView.IgnorePixelScaling)] = MapIgnorePixelScaling,
                [nameof(ISKGLView.HasRenderLoop)] = MapHasRenderLoop,
                [nameof(ISKGLView.EnableTouchEvents)] = MapEnableTouchEvents,
            };

        public static CommandMapper<ISKGLView, AndroidSKGLViewSurfaceHandler> SKGLViewCommandMapper =
            new CommandMapper<ISKGLView, AndroidSKGLViewSurfaceHandler>(ViewHandler.ViewCommandMapper)
            {
                [nameof(ISKGLView.InvalidateSurface)] = OnInvalidateSurface,
            };

        protected override SKGLSurfaceView CreatePlatformView()
        {
            var view = new MauiSKGLSurfaceView(Context);
            /* Read only by GLSurfaceView.OnPause, which nothing calls: the EGL context and its
               GPU resources outlive a stopped window either way */
            view.PreserveEGLContextOnPause = true;
            return view;
        }

        protected override void ConnectHandler(SKGLSurfaceView platformView)
        {
            platformView.PaintSurface += OnPaintSurface;
            base.ConnectHandler(platformView);
        }

        protected override void DisconnectHandler(SKGLSurfaceView platformView)
        {
            _touchHandler?.Detach(platformView);
            _touchHandler = null;
            platformView.PaintSurface -= OnPaintSurface;
            _lastGRContext = null;
            _lastCanvasSize = default;
            base.DisconnectHandler(platformView);
        }

        /* GLSurfaceView.requestRender is thread-safe, so no hop to the UI thread */
        public static void OnInvalidateSurface(AndroidSKGLViewSurfaceHandler handler, ISKGLView view, object? args)
        {
            /* The typed PlatformView property throws while the handler is disconnecting */
            var platformView = ((IElementHandler?)handler)?.PlatformView as SKGLSurfaceView;
            if (platformView == null || platformView.Handle == IntPtr.Zero)
                return;

            if (platformView.RenderMode == Rendermode.WhenDirty)
                platformView.RequestRender();
        }

        public static void MapIgnorePixelScaling(AndroidSKGLViewSurfaceHandler handler, ISKGLView view)
        {
            if (handler?.PlatformView is MauiSKGLSurfaceView pv)
            {
                pv.IgnorePixelScaling = view.IgnorePixelScaling;
                pv.RequestRender();
            }
        }

        public static void MapHasRenderLoop(AndroidSKGLViewSurfaceHandler handler, ISKGLView view)
        {
            var platformView = handler?.PlatformView;
            if (platformView == null)
                return;

            platformView.RenderMode = view.HasRenderLoop ? Rendermode.Continuously : Rendermode.WhenDirty;
        }

        public static void MapEnableTouchEvents(AndroidSKGLViewSurfaceHandler handler, ISKGLView view)
        {
            var platformView = handler?.PlatformView;
            if (handler == null || platformView == null)
                return;

            if (handler._touchHandler == null)
            {
                handler._touchHandler = new SurfaceTouchHandler(
                    args => handler.VirtualView?.OnTouch(args),
                    (x, y) => handler.GetScaledCoord(x, y));
            }
            handler._touchHandler.SetEnabled(platformView, view.EnableTouchEvents);
        }

        /* GL thread */
        private void OnPaintSurface(object? sender, SkiaSharp.Views.Android.SKPaintGLSurfaceEventArgs e)
        {
            /* The typed VirtualView property throws once the handler is disconnected */
            var virtualView = ((IElementHandler)this).VirtualView as ISKGLView;
            if (virtualView == null)
                return;

            var newCanvasSize = e.Info.Size;
            if (_lastCanvasSize != newCanvasSize)
            {
                _lastCanvasSize = newCanvasSize;
                virtualView.OnCanvasSizeChanged(newCanvasSize);
            }
            if (sender is SKGLSurfaceView platformView)
            {
                var newGRContext = platformView.GRContext;
                if (_lastGRContext != newGRContext)
                {
                    _lastGRContext = newGRContext;
                    virtualView.OnGRContextChanged(newGRContext);
                }
            }

            virtualView.OnPaintSurface(new SkiaSharp.Views.Maui.SKPaintGLSurfaceEventArgs(e.Surface, e.BackendRenderTarget, e.Origin, e.Info, e.RawInfo));
        }

        private SKPoint GetScaledCoord(double x, double y)
        {
            if (VirtualView?.IgnorePixelScaling == true && Context != null)
            {
                x = Context.FromPixels(x);
                y = Context.FromPixels(y);
            }
            return new SKPoint((float)x, (float)y);
        }

        private class MauiSKGLSurfaceView : SKGLSurfaceView
        {
            private readonly float _density;

            public MauiSKGLSurfaceView(Context context) : base(context)
            {
                _density = Resources?.DisplayMetrics?.Density ?? 1;
            }

            public bool IgnorePixelScaling { get; set; }

            private bool _wasDetached;

            /* GLSurfaceView gives a re-attached view a new GL thread and EGL context, while
               SKGLSurfaceView's renderer keeps the GRContext it made in the old one */
            protected override void OnAttachedToWindow()
            {
                base.OnAttachedToWindow();
                if (_wasDetached && GRContext != null)
                    GHApp.MaybeWriteGHLog("Map surface re-attached: its GRContext belongs to a destroyed EGL context",
                        true, GHConstants.SentryGnollHackGeneralCategoryName);
            }

            protected override void OnDetachedFromWindow()
            {
                _wasDetached = true;
                base.OnDetachedFromWindow();
            }

            protected override void OnPaintSurface(SkiaSharp.Views.Android.SKPaintGLSurfaceEventArgs e)
            {
                if (IgnorePixelScaling)
                {
                    var userVisibleSize = new SKSizeI((int)(e.Info.Width / _density), (int)(e.Info.Height / _density));
                    var canvas = e.Surface.Canvas;
                    canvas.Scale(_density);
                    canvas.Save();
                    e = new SkiaSharp.Views.Android.SKPaintGLSurfaceEventArgs(e.Surface, e.BackendRenderTarget, e.Origin, e.Info.WithSize(userVisibleSize), e.Info);
                }
                base.OnPaintSurface(e);
            }
        }

        /* Adapted from SkiaSharp's internal Android SKTouchHandler (MIT) */
        private class SurfaceTouchHandler
        {
            private Action<SKTouchEventArgs>? _onTouchAction;
            private Func<double, double, SKPoint>? _scalePixels;

            public SurfaceTouchHandler(Action<SKTouchEventArgs> onTouchAction, Func<double, double, SKPoint> scalePixels)
            {
                _onTouchAction = onTouchAction;
                _scalePixels = scalePixels;
            }

            public void SetEnabled(AView view, bool enableTouchEvents)
            {
                if (view != null && view.Handle != IntPtr.Zero)
                {
                    view.Touch -= OnTouch;
                    if (enableTouchEvents)
                        view.Touch += OnTouch;
                }
            }

            public void Detach(AView view)
            {
                SetEnabled(view, false);
                _onTouchAction = null;
                _scalePixels = null;
            }

            private void OnTouch(object? sender, AView.TouchEventArgs e)
            {
                var evt = e.Event;
                if (_onTouchAction == null || _scalePixels == null || evt == null)
                    return;

                int pointer = evt.ActionIndex;
                int id = evt.GetPointerId(pointer);
                SKPoint coords = _scalePixels(evt.GetX(pointer), evt.GetY(pointer));
                MotionEventToolType toolType = evt.GetToolType(pointer);
                SKTouchDeviceType deviceType = GetDeviceType(toolType);
                float pressure = evt.GetPressure(pointer);
                SKMouseButton button = GetButton(evt, toolType);

                switch (evt.ActionMasked)
                {
                case MotionEventActions.Down:
                case MotionEventActions.PointerDown:
                    {
                        var args = new SKTouchEventArgs(id, SKTouchAction.Pressed, button, deviceType, coords, true, 0, pressure);
                        _onTouchAction(args);
                        e.Handled = args.Handled;
                        break;
                    }
                case MotionEventActions.Move:
                    {
                        int count = evt.PointerCount;
                        for (pointer = 0; pointer < count; pointer++)
                        {
                            id = evt.GetPointerId(pointer);
                            coords = _scalePixels(evt.GetX(pointer), evt.GetY(pointer));
                            var args = new SKTouchEventArgs(id, SKTouchAction.Moved, button, deviceType, coords, true, 0, pressure);
                            _onTouchAction(args);
                            e.Handled = e.Handled || args.Handled;
                        }
                        break;
                    }
                case MotionEventActions.Up:
                case MotionEventActions.PointerUp:
                    {
                        var args = new SKTouchEventArgs(id, SKTouchAction.Released, button, deviceType, coords, false, 0, pressure);
                        _onTouchAction(args);
                        e.Handled = args.Handled;
                        break;
                    }
                case MotionEventActions.Cancel:
                    {
                        var args = new SKTouchEventArgs(id, SKTouchAction.Cancelled, button, deviceType, coords, false, 0, pressure);
                        _onTouchAction(args);
                        e.Handled = args.Handled;
                        break;
                    }
                }
            }

            private static SKMouseButton GetButton(MotionEvent evt, MotionEventToolType toolType)
            {
                if (toolType == MotionEventToolType.Eraser)
                    return SKMouseButton.Middle;
                if (evt.ButtonState.HasFlag(MotionEventButtonState.StylusSecondary))
                    return SKMouseButton.Right;
                return SKMouseButton.Left;
            }

            private static SKTouchDeviceType GetDeviceType(MotionEventToolType toolType)
            {
                switch (toolType)
                {
                case MotionEventToolType.Stylus:
                case MotionEventToolType.Eraser:
                    return SKTouchDeviceType.Pen;
                case MotionEventToolType.Mouse:
                    return SKTouchDeviceType.Mouse;
                default:
                    return SKTouchDeviceType.Touch;
                }
            }
        }
    }
}

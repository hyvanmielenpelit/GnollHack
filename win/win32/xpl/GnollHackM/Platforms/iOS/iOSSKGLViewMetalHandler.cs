#nullable enable

using System;
using Microsoft.Maui;
using Microsoft.Maui.Handlers;
using SkiaSharp;
using SkiaSharp.Views.iOS;
using SkiaSharp.Views.Maui;
using UIKit;
using Foundation;
using System.Linq;

namespace GnollHackM.Platforms.iOS
{
    public class iOSSKGLViewMetalHandler : ViewHandler<ISKGLView, SKMetalView>
    {
        private CustomSKTouchHandler? _touchHandler;
        private SKSizeI lastCanvasSize;
        private GRContext? lastGRContext;

        public iOSSKGLViewMetalHandler() : base(SKGLViewMapper, SKGLViewCommandMapper)
        {
        }

        public static PropertyMapper<ISKGLView, iOSSKGLViewMetalHandler> SKGLViewMapper =
            new PropertyMapper<ISKGLView, iOSSKGLViewMetalHandler>(ViewHandler.ViewMapper)
            {
                [nameof(ISKGLView.IgnorePixelScaling)] = MapIgnorePixelScaling,
                [nameof(ISKGLView.HasRenderLoop)] = MapHasRenderLoop,
                [nameof(ISKGLView.EnableTouchEvents)] = MapEnableTouchEvents,
            };

        public static CommandMapper<ISKGLView, iOSSKGLViewMetalHandler> SKGLViewCommandMapper =
            new CommandMapper<ISKGLView, iOSSKGLViewMetalHandler>(ViewHandler.ViewCommandMapper)
            {
                [nameof(ISKGLView.InvalidateSurface)] = OnInvalidateSurface,
            };

        protected override SKMetalView CreatePlatformView()
        {
            return new MauiSKMetalView
            {
                BackgroundColor = UIColor.Clear,
                Opaque = false,
            };
        }

        protected override void ConnectHandler(SKMetalView platformView)
        {
            platformView.PaintSurface += OnPaintSurface;
            base.ConnectHandler(platformView);
        }

        protected override void DisconnectHandler(SKMetalView platformView)
        {
            platformView.PaintSurface -= OnPaintSurface;
            if (_touchHandler != null)
            {
                _touchHandler.Detach(platformView);
                _touchHandler = null;
            }
            base.DisconnectHandler(platformView);
        }

        private void OnPaintSurface(object? sender, SKPaintMetalSurfaceEventArgs e)
        {
            var virtualView = VirtualView;
            if (virtualView == null)
                return;

            var newCanvasSize = e.Info.Size;
            if (lastCanvasSize != newCanvasSize)
            {
                lastCanvasSize = newCanvasSize;
                virtualView.OnCanvasSizeChanged(newCanvasSize);
            }
            if (sender is SKMetalView platformView)
            {
                var newGRContext = platformView.GRContext;
                if (lastGRContext != newGRContext)
                {
                    lastGRContext = newGRContext;
                    virtualView.OnGRContextChanged(newGRContext);
                }
            }

            var args = new SkiaSharp.Views.Maui.SKPaintGLSurfaceEventArgs(e.Surface, e.BackendRenderTarget, e.Origin, e.Info, e.RawInfo);
            virtualView.OnPaintSurface(args);
        }

        public static void OnInvalidateSurface(iOSSKGLViewMetalHandler handler, ISKGLView view, object? args)
        {
            var platformView = handler?.PlatformView;
            if (platformView == null)
                return;

            if (platformView.Paused && platformView.EnableSetNeedsDisplay)
            {
                platformView.BeginInvokeOnMainThread(() =>
                {
                    if (platformView.Handle != IntPtr.Zero)
                    {
                        platformView.SetNeedsDisplay();
                    }
                });
            }
        }

        public static void MapIgnorePixelScaling(iOSSKGLViewMetalHandler handler, ISKGLView view)
        {
            if (handler?.PlatformView is MauiSKMetalView pv)
            {
                pv.IgnorePixelScaling = view.IgnorePixelScaling;
                OnInvalidateSurface(handler, view, null);
            }
        }

        public static void MapHasRenderLoop(iOSSKGLViewMetalHandler handler, ISKGLView view)
        {
            var platformView = handler?.PlatformView;
            if (platformView == null)
                return;

            platformView.Paused = !view.HasRenderLoop;
            platformView.EnableSetNeedsDisplay = !view.HasRenderLoop;
        }

        public static void MapEnableTouchEvents(iOSSKGLViewMetalHandler handler, ISKGLView view)
        {
            var platformView = handler?.PlatformView;
            if (handler == null || platformView == null)
                return;

            if (view.EnableTouchEvents)
            {
                if (handler._touchHandler == null)
                {
                    handler._touchHandler = new CustomSKTouchHandler(
                        args => handler.VirtualView?.OnTouch(args),
                        (x, y) =>
                        {
                            var scale = platformView.ContentScaleFactor;
                            bool ignore = handler.VirtualView?.IgnorePixelScaling ?? false;
                            if (!ignore)
                            {
                                x *= scale;
                                y *= scale;
                            }
                            return new SKPoint((float)x, (float)y);
                        }
                    );
                }
                handler._touchHandler?.SetEnabled(platformView, true);
            }
            else
            {
                handler._touchHandler?.SetEnabled(platformView, false);
            }
        }

        private class MauiSKMetalView : SKMetalView
        {
            public bool IgnorePixelScaling { get; set; }

            protected override void OnPaintSurface(SKPaintMetalSurfaceEventArgs e)
            {
                if (IgnorePixelScaling)
                {
                    var userVisibleSize = new SKSizeI((int)Bounds.Width, (int)Bounds.Height);
                    var canvas = e.Surface.Canvas;
                    canvas.Scale((float)ContentScaleFactor);
                    canvas.Save();

                    e = new SKPaintMetalSurfaceEventArgs(e.Surface, e.BackendRenderTarget, e.Origin, e.Info.WithSize(userVisibleSize), e.Info);
                }

                base.OnPaintSurface(e);
            }
        }

        private class CustomSKTouchHandler : UIGestureRecognizer
        {
            private Action<SKTouchEventArgs>? onTouchAction;
            private Func<double, double, SKPoint>? scalePixels;

            public CustomSKTouchHandler(Action<SKTouchEventArgs> onTouchAction, Func<double, double, SKPoint> scalePixels)
            {
                this.onTouchAction = onTouchAction;
                this.scalePixels = scalePixels;
                DisablesUserInteraction = false;
            }

            public bool DisablesUserInteraction { get; set; }

            public void SetEnabled(UIView view, bool enableTouchEvents)
            {
                if (view != null)
                {
                    if (!view.UserInteractionEnabled || DisablesUserInteraction)
                    {
                        view.UserInteractionEnabled = enableTouchEvents;
                    }
                    if (enableTouchEvents && view.GestureRecognizers?.Contains(this) != true)
                    {
                        view.AddGestureRecognizer(this);
                    }
                    else if (!enableTouchEvents && view.GestureRecognizers?.Contains(this) == true)
                    {
                        view.RemoveGestureRecognizer(this);
                    }
                }
            }

            public void Detach(UIView view)
            {
                SetEnabled(view, false);
                onTouchAction = null;
                scalePixels = null;
            }

            public override void TouchesBegan(NSSet touches, UIEvent evt)
            {
                base.TouchesBegan(touches, evt);
                foreach (UITouch touch in touches.Cast<UITouch>())
                {
                    if (!FireEvent(SKTouchAction.Pressed, touch, true))
                    {
                        IgnoreTouch(touch, evt);
                    }
                }
            }

            public override void TouchesMoved(NSSet touches, UIEvent evt)
            {
                base.TouchesMoved(touches, evt);
                foreach (UITouch touch in touches.Cast<UITouch>())
                {
                    FireEvent(SKTouchAction.Moved, touch, true);
                }
            }

            public override void TouchesEnded(NSSet touches, UIEvent evt)
            {
                base.TouchesEnded(touches, evt);
                foreach (UITouch touch in touches.Cast<UITouch>())
                {
                    FireEvent(SKTouchAction.Released, touch, false);
                }
            }

            public override void TouchesCancelled(NSSet touches, UIEvent evt)
            {
                base.TouchesCancelled(touches, evt);
                foreach (UITouch touch in touches.Cast<UITouch>())
                {
                    FireEvent(SKTouchAction.Cancelled, touch, false);
                }
            }

            private bool FireEvent(SKTouchAction actionType, UITouch touch, bool inContact)
            {
                if (onTouchAction == null || scalePixels == null)
                    return false;

                var id = ((IntPtr)touch.Handle).ToInt64();
                var cgPoint = touch.LocationInView(View);
                var point = scalePixels(cgPoint.X, cgPoint.Y);

                var args = new SKTouchEventArgs(id, actionType, point, inContact);
                onTouchAction(args);
                return args.Handled;
            }
        }
    }
}

#nullable enable

using System;
using CoreGraphics;
using Metal;
using MetalKit;
using Microsoft.Maui;
using Microsoft.Maui.Handlers;
using SkiaSharp;
using SkiaSharp.Views.Maui;
using UIKit;
using Foundation;
using System.Linq;

namespace GnollHackM.Platforms.iOS
{
    public class iOSSKGLViewMetalHandler : ViewHandler<ISKGLView, iOSSKGLViewMetalHandler.MauiMetalView>
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

        protected override MauiMetalView CreatePlatformView()
        {
            return new MauiMetalView
            {
                BackgroundColor = UIColor.Clear,
                Opaque = false,
            };
        }

        protected override void ConnectHandler(MauiMetalView platformView)
        {
            platformView.PaintSurface += OnPaintSurface;
            base.ConnectHandler(platformView);
        }

        protected override void DisconnectHandler(MauiMetalView platformView)
        {
            platformView.PaintSurface -= OnPaintSurface;

            /* The virtual view must drop its GRContext before the context is disposed */
            if (lastGRContext != null && ((IElementHandler)this).VirtualView is ISKGLView virtualView)
                virtualView.OnGRContextChanged(null);
            lastGRContext = null;
            lastCanvasSize = default;
            /* MapHasRenderLoop resumes drawing on reconnect */
            platformView.Paused = true;
            platformView.EnableSetNeedsDisplay = false;
            platformView.ReleaseGpuResources();

            if (_touchHandler != null)
            {
                _touchHandler.Detach(platformView);
                _touchHandler = null;
            }
            base.DisconnectHandler(platformView);
        }

        private void OnPaintSurface(object? sender, SKPaintGLSurfaceEventArgs e)
        {
            var virtualView = ((IElementHandler)this).VirtualView as ISKGLView;
            if (virtualView == null)
                return;

            var newCanvasSize = e.Info.Size;
            if (lastCanvasSize != newCanvasSize)
            {
                lastCanvasSize = newCanvasSize;
                virtualView.OnCanvasSizeChanged(newCanvasSize);
            }
            if (sender is MauiMetalView platformView)
            {
                var newGRContext = platformView.GRContext;
                if (lastGRContext != newGRContext)
                {
                    lastGRContext = newGRContext;
                    virtualView.OnGRContextChanged(newGRContext);
                }
            }

            virtualView.OnPaintSurface(e);
        }

        public static void OnInvalidateSurface(iOSSKGLViewMetalHandler handler, ISKGLView view, object? args)
        {
            /* The typed PlatformView property throws while the handler is disconnecting */
            var platformView = ((IElementHandler?)handler)?.PlatformView as MauiMetalView;
            if (platformView == null)
                return;

            /* NSObject.BeginInvokeOnMainThread posts to the run loop even when already on
               the main thread, and allocates a dispatcher per call; this runs per frame */
            if (NSThread.Current.IsMainThread)
            {
                SetNeedsDisplayIfPaused(platformView);
            }
            else
            {
                platformView.BeginInvokeOnMainThread(() => SetNeedsDisplayIfPaused(platformView));
            }
        }

        private static void SetNeedsDisplayIfPaused(MauiMetalView platformView)
        {
            if (platformView.Handle != IntPtr.Zero && platformView.Paused && platformView.EnableSetNeedsDisplay)
            {
                platformView.SetNeedsDisplay();
            }
        }

        public static void MapIgnorePixelScaling(iOSSKGLViewMetalHandler handler, ISKGLView view)
        {
            if (handler?.PlatformView is MauiMetalView pv)
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

        /* An MTKView that renders with Skia. Each view owns its own command queue and
           GRContext. Drawing happens on the main thread. */
        public class MauiMetalView : MTKView, IMTKViewDelegate
        {
            private const SKColorType colorType = SKColorType.Bgra8888;
            private const GRSurfaceOrigin surfaceOrigin = GRSurfaceOrigin.TopLeft;

            private IMTLDevice? _device;
            private IMTLCommandQueue? _queue;
            private GRContext? _context;

            public MauiMetalView() : base(CGRect.Empty, MTLDevice.SystemDefault)
            {
                _device = Device;
                if (_device == null)
                {
                    Console.WriteLine("Metal is not supported on this device.");
                    return;
                }

                ColorPixelFormat = MTLPixelFormat.BGRA8Unorm;
                /* Skia renders straight into the drawable texture and allocates its own
                   stencil buffers, so the view needs no depth, stencil, or MSAA textures */
                DepthStencilPixelFormat = MTLPixelFormat.Invalid;
                SampleCount = 1;
                /* Skia may read back from the render target */
                FramebufferOnly = false;
                _queue = _device.CreateCommandQueue();
                Delegate = this;
            }

            public bool IgnorePixelScaling { get; set; }

            public GRContext? GRContext => _context;

            public event EventHandler<SKPaintGLSurfaceEventArgs>? PaintSurface;

            /* Frees the GRContext and its resource cache; the next draw creates a new one */
            public void ReleaseGpuResources()
            {
                if (_context != null)
                {
                    _context.AbandonContext(true);
                    _context.Dispose();
                    _context = null;
                }
                ReleaseDrawables();
            }

            public override void MovedToWindow()
            {
                base.MovedToWindow();
                if (Window == null)
                    ReleaseDrawables();
            }

            void IMTKViewDelegate.DrawableSizeWillChange(MTKView view, CGSize size)
            {
                if (Paused && EnableSetNeedsDisplay)
                    SetNeedsDisplay();
            }

            void IMTKViewDelegate.Draw(MTKView view)
            {
                var queue = _queue;
                if (_device == null || queue == null)
                    return;

                /* Every wrapper is disposed at the end of the frame: the layer has only a
                   few drawables and cannot reuse one that a GC-pending wrapper retains */
                using var drawable = CurrentDrawable;
                if (drawable == null)
                    return;
                using var texture = drawable.Texture;
                if (texture == null)
                    return;

                int width = (int)texture.Width;
                int height = (int)texture.Height;
                if (width <= 0 || height <= 0)
                    return;

                if (_context == null)
                {
                    using var backendContext = new GRMtlBackendContext
                    {
                        Device = _device,
                        Queue = queue,
                    };
                    _context = GRContext.CreateMetal(backendContext);
                    if (_context == null)
                        return;
                }

                using var renderTarget = new GRBackendRenderTarget(width, height, new GRMtlTextureInfo(texture));
                using var surface = SKSurface.Create(_context, renderTarget, surfaceOrigin, colorType);
                if (surface == null)
                    return;

                var rawInfo = new SKImageInfo(width, height, colorType);
                var info = rawInfo;
                if (IgnorePixelScaling)
                {
                    surface.Canvas.Scale((float)ContentScaleFactor);
                    info = rawInfo.WithSize(new SKSizeI((int)Bounds.Width, (int)Bounds.Height));
                }

                PaintSurface?.Invoke(this, new SKPaintGLSurfaceEventArgs(surface, renderTarget, surfaceOrigin, info, rawInfo));

                surface.Flush();

                using var commandBuffer = queue.CommandBuffer();
                if (commandBuffer == null)
                    return;
                commandBuffer.PresentDrawable(drawable);
                commandBuffer.Commit();
            }

            protected override void Dispose(bool disposing)
            {
                if (disposing)
                {
                    ReleaseGpuResources();
                    /* The device wrapper is shared with MTLDevice.SystemDefault callers */
                    _device = null;
                    _queue?.Dispose();
                    _queue = null;
                }
                base.Dispose(disposing);
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

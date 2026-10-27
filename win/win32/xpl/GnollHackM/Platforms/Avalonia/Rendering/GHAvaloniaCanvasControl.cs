using System;
using System.Diagnostics;
using System.Threading;
using GnollHackX;
using SkiaSharp;
using SkiaSharp.Views.Maui;
using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvDispatcherPriority = Avalonia.Threading.DispatcherPriority;
using AvPoint = Avalonia.Point;
using AvPointerCaptureLostEventArgs = Avalonia.Input.PointerCaptureLostEventArgs;
using AvPointerEventArgs = Avalonia.Input.PointerEventArgs;
using AvPointerPressedEventArgs = Avalonia.Input.PointerPressedEventArgs;
using AvPointerReleasedEventArgs = Avalonia.Input.PointerReleasedEventArgs;
using AvPointerType = Avalonia.Input.PointerType;
using AvRect = Avalonia.Rect;

namespace GnollHackM
{
    // WORKAROUND(WA-AVALONIA-0010): ACM canvas view paints inside the render pass
    /* ACM's SKCanvasViewControl, painting on the UI thread outside Avalonia's render pass, because
       GnollHack's raster views read and write MAUI properties while painting. Paint requests are
       coalesced into one posted paint below layout priority, except that a request made inside the
       platform frame callback paints at once; Render only draws the last painted image.
       Touch handling is ACM's; mouse hover, and Cancelled for a touch ACM drops, are raised here. */
    public class GHAvaloniaCanvasControl : Avalonia.Controls.Maui.SkiaSharp.Views.Controls.SKCanvasViewControl
    {
        /* Normal requests run after layout; requests made by a paint itself run below input, so a view
           that invalidates from its own paint cannot starve input */
        private static readonly AvDispatcherPriority PaintPriority = AvDispatcherPriority.Loaded;
        private static readonly AvDispatcherPriority RepaintPriority = AvDispatcherPriority.Background;

        private int _paintRequested = 0;
        private int _paintPosted = 0;
        private volatile bool _isPainting = false;

        /* UI thread only */
        private SKSurface _surface = null;
        private SKImageInfo _surfaceInfo;
        private GHRefCountedImage _currentImage = null;
        private double _currentImageScaling = 1.0;
        private Avalonia.Controls.TopLevel _topLevel = null;

        /* Stopwatch timestamp of the last raised hover move */
        private long _lastHoverMoveTicks = 0;

        /* Touch ids ACM has raised Pressed for; UI thread only */
        private readonly GHAvaloniaTouchTracker _touchTracker = new GHAvaloniaTouchTracker();

        /* Marks the view dirty and posts one paint unless one is already posted; any thread */
        public void RequestPaint()
        {
            Interlocked.Exchange(ref _paintRequested, 1);
            // WORKAROUND(WA-AVALONIA-0021): An animation frame that commits nothing waits for a 16 ms timer
            /* Inside the frame callback the paint runs at once, before the render pass, so that this
               frame commits it */
            if (GHAvaloniaFrameLoop.IsInFrameCallback && !_isPainting && AvDispatcher.UIThread.CheckAccess())
            {
                PaintIfRequested();
                return;
            }
            if (Interlocked.CompareExchange(ref _paintPosted, 1, 0) == 0)
                AvDispatcher.UIThread.Post(PaintNow, _isPainting ? RepaintPriority : PaintPriority);
        }

        // WORKAROUND(WA-AVALONIA-0021): An animation frame that commits nothing waits for a 16 ms timer
        /* Shows the last painted image again without painting, so that the frame commits and the next
           animation frame follows the render loop. UI thread; false when nothing was asked for */
        public bool RequestPresent()
        {
            if (!AvDispatcher.UIThread.CheckAccess())
                return false;
            if (_topLevel == null || !IsEffectivelyVisible || _currentImage == null)
                return false;

            /* Render draws the last painted image */
            InvalidateVisual();
            return true;
        }

        private void PaintNow()
        {
            Interlocked.Exchange(ref _paintPosted, 0);
            PaintIfRequested();
        }

        /* UI thread */
        private void PaintIfRequested()
        {
            if (_isPainting)
            {
                /* Re-entered from inside a paint; the outer paint posts again on finishing */
                return;
            }
            if (Interlocked.Exchange(ref _paintRequested, 0) == 0)
                return;

            _isPainting = true;
            try
            {
                PaintCore();
            }
            catch (Exception ex)
            {
                /* Logged and dropped, as Avalonia does for an exception in a render-thread draw operation */
                GHApp.MaybeWriteGHLog("GHAvaloniaCanvasControl: paint failed: " + ex.ToString());
            }
            finally
            {
                _isPainting = false;
                if (Volatile.Read(ref _paintRequested) != 0 && Interlocked.CompareExchange(ref _paintPosted, 1, 0) == 0)
                    AvDispatcher.UIThread.Post(PaintNow, RepaintPriority);
            }
        }

        private void PaintCore()
        {
            Action<SKPaintSurfaceEventArgs> paintAction = PaintSurfaceAction;
            if (paintAction == null)
                return;

            AvRect bounds = Bounds;
            if (bounds.Width <= 0 || bounds.Height <= 0)
                return;

            double scaling = Avalonia.Controls.TopLevel.GetTopLevel(this)?.RenderScaling ?? 1.0;
            int pixelWidth = (int)(bounds.Width * scaling);
            int pixelHeight = (int)(bounds.Height * scaling);
            if (pixelWidth <= 0 || pixelHeight <= 0)
                return;

            SKImageInfo rawInfo = new SKImageInfo(pixelWidth, pixelHeight, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
            if (_surface == null || _surfaceInfo.Width != pixelWidth || _surfaceInfo.Height != pixelHeight)
            {
                _surface?.Dispose();
                _surface = SKSurface.Create(rawInfo);
                _surfaceInfo = rawInfo;
                if (_surface == null)
                    return;
            }

            SKCanvas canvas = _surface.Canvas;
            int saveCount = canvas.Save();
            try
            {
                canvas.Clear(SKColors.Transparent);
                SKImageInfo info = rawInfo;
                if (IgnorePixelScaling)
                {
                    info = new SKImageInfo((int)bounds.Width, (int)bounds.Height, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
                    canvas.Scale((float)scaling);
                }
                paintAction(new SKPaintSurfaceEventArgs(_surface, info, rawInfo));
            }
            finally
            {
                canvas.RestoreToCount(saveCount);
            }

            SKImage snapshot = _surface.Snapshot();
            if (snapshot == null)
                return;

            SetCurrentImage(new GHRefCountedImage(snapshot), scaling);
            InvalidateVisual();
        }

        private void SetCurrentImage(GHRefCountedImage image, double scaling)
        {
            GHRefCountedImage previous = _currentImage;
            _currentImage = image;
            _currentImageScaling = scaling;
            previous?.Release();
        }

        /* Zero unless MAUI requests a size, as with SkiaSharp's views on the other platforms */
        protected override Avalonia.Size MeasureOverride(Avalonia.Size availableSize)
        {
            return default(Avalonia.Size);
        }
        public override void Render(Avalonia.Media.DrawingContext context)
        {
            GHRefCountedImage image = _currentImage;
            if (image == null)
                return;

            AvRect bounds = Bounds;
            if (bounds.Width <= 0 || bounds.Height <= 0)
                return;

            if (!image.AddRef())
                return;

            GHAvaloniaFrameLoop.NoteFrameSubmitted();
            context.Custom(new ImageDrawOperation(new AvRect(bounds.Size), image, _currentImageScaling));
        }

        protected override void OnAttachedToVisualTree(Avalonia.VisualTreeAttachmentEventArgs e)
        {
            base.OnAttachedToVisualTree(e);
            _topLevel = Avalonia.Controls.TopLevel.GetTopLevel(this);
            if (_topLevel != null)
                _topLevel.ScalingChanged += TopLevel_ScalingChanged;
            RequestPaint();
        }

        protected override void OnDetachedFromVisualTree(Avalonia.VisualTreeAttachmentEventArgs e)
        {
            base.OnDetachedFromVisualTree(e);
            if (_topLevel != null)
            {
                _topLevel.ScalingChanged -= TopLevel_ScalingChanged;
                _topLevel = null;
            }

            /* Draw operations still holding the image keep it until they are disposed */
            SetCurrentImage(null, 1.0);
            _surface?.Dispose();
            _surface = null;
        }

        protected override void OnSizeChanged(Avalonia.Controls.SizeChangedEventArgs e)
        {
            base.OnSizeChanged(e);
            RequestPaint();
        }

        private void TopLevel_ScalingChanged(object sender, EventArgs e)
        {
            RequestPaint();
        }

        // WORKAROUND(WA-AVALONIA-0009): ACM Skia controls raise no hover touch actions
        protected override void OnPointerEntered(AvPointerEventArgs e)
        {
            base.OnPointerEntered(e);
            RaiseHover(e, SKTouchAction.Entered);
        }

        protected override void OnPointerExited(AvPointerEventArgs e)
        {
            base.OnPointerExited(e);
            RaiseHover(e, SKTouchAction.Exited);
        }

        /* ACM's base handles in-contact moves */
        protected override void OnPointerMoved(AvPointerEventArgs e)
        {
            base.OnPointerMoved(e);
            if (GHAvaloniaPointerHover.IsInContact(e.GetCurrentPoint(this).Properties))
                return;

            long now = Stopwatch.GetTimestamp();
            if (now - _lastHoverMoveTicks < GHAvaloniaPointerHover.HoverMoveIntervalTicks)
                return;
            _lastHoverMoveTicks = now;
            RaiseHover(e, SKTouchAction.Moved);
        }

        /* Mouse only: touch screens and pens produce no hover */
        private void RaiseHover(AvPointerEventArgs e, SKTouchAction action)
        {
            Func<SKTouchEventArgs, bool> touchAction = TouchAction;
            if (!EnableTouchEvents || touchAction == null || e.Pointer.Type != AvPointerType.Mouse)
                return;
            if (GHAvaloniaPointerHover.IsInContact(e.GetCurrentPoint(this).Properties))
                return;

            GHAvaloniaPointerHover.Raise(this, e, action, IgnorePixelScaling, touchAction);
        }

        // WORKAROUND(WA-AVALONIA-0019): ACM Skia controls drop touches without Released or Cancelled
        protected override void OnPointerPressed(AvPointerPressedEventArgs e)
        {
            Func<SKTouchEventArgs, bool> touchAction = TouchAction;
            if (!EnableTouchEvents || touchAction == null)
            {
                base.OnPointerPressed(e);
                return;
            }

            /* ACM gives a pointer pressed again a new id without releasing the old one */
            Avalonia.Input.IPointer pointer = e.Pointer;
            _touchTracker.Cancel(pointer, touchAction);

            Func<SKTouchEventArgs, bool> trackingAction = args =>
            {
                if (args.ActionType == SKTouchAction.Pressed)
                    _touchTracker.Track(pointer, args);
                return touchAction(args);
            };
            TouchAction = trackingAction;
            try
            {
                base.OnPointerPressed(e);
            }
            finally
            {
                if (ReferenceEquals(TouchAction, trackingAction))
                    TouchAction = touchAction;
            }
        }

        protected override void OnPointerReleased(AvPointerReleasedEventArgs e)
        {
            base.OnPointerReleased(e);
            _touchTracker.Forget(e.Pointer);
        }

        /* ACM forgets the pointer here without raising an action; after a release there is nothing to cancel */
        protected override void OnPointerCaptureLost(AvPointerCaptureLostEventArgs e)
        {
            base.OnPointerCaptureLost(e);
            _touchTracker.Cancel(e.Pointer, EnableTouchEvents ? TouchAction : null);
        }

        /* Draws one painted image on the render thread; holds its own reference until disposed */
        private sealed class ImageDrawOperation : Avalonia.Rendering.SceneGraph.ICustomDrawOperation
        {
            private GHRefCountedImage _image;
            private readonly double _scaling;

            public ImageDrawOperation(AvRect bounds, GHRefCountedImage image, double scaling)
            {
                Bounds = bounds;
                _image = image;
                _scaling = scaling;
            }

            public AvRect Bounds { get; }

            public void Dispose()
            {
                GHRefCountedImage image = Interlocked.Exchange(ref _image, null);
                image?.Release();
            }

            public bool Equals(Avalonia.Rendering.SceneGraph.ICustomDrawOperation other) => ReferenceEquals(this, other);

            public bool HitTest(AvPoint p) => Bounds.Contains(p);

            public void Render(Avalonia.Media.ImmediateDrawingContext context)
            {
                SKImage image = Volatile.Read(ref _image)?.Image;
                if (image == null)
                    return;

#pragma warning disable CS0618
                Avalonia.Skia.ISkiaSharpApiLeaseFeature leaseFeature =
                    context.TryGetFeature(typeof(Avalonia.Skia.ISkiaSharpApiLeaseFeature)) as Avalonia.Skia.ISkiaSharpApiLeaseFeature;
                if (leaseFeature == null)
                    return;

                using (Avalonia.Skia.ISkiaSharpApiLease lease = leaseFeature.Lease())
                {
                    SKCanvas canvas = lease?.SkCanvas;
                    if (canvas == null)
                        return;

                    /* A GPU pass has its context current, so buried GL resources can go now */
                    GRContext grContext = lease.GrContext;
                    if (grContext != null)
                        GHGLResourceGraveyard.Drain(grContext);

                    canvas.Save();
                    canvas.Scale(1.0f / (float)_scaling);
                    canvas.DrawImage(image, 0, 0, SKSamplingOptions.Default, null);
                    canvas.Restore();
                }
#pragma warning restore CS0618
            }
        }
    }
}

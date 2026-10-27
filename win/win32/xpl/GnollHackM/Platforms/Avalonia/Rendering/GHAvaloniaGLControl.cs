using System;
using System.Diagnostics;
using System.Threading;
using GnollHackX;
using GnollHackX.Performance;
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
    /* ACM's SKGLViewControl with one cached GPU surface per pixel size and GRContext, an explicit GLX
       swap interval per drawable (Linux), present-timing samples, and an invalidation that is
       immediate on the UI thread. The paint runs on Avalonia's render thread with the context current. */
    public sealed class GHAvaloniaGLControl : Avalonia.Controls.Maui.SkiaSharp.Views.Controls.SKGLViewControl
    {
        private const uint GL_RGBA8 = 0x8058;

        /* The cached GPU surface and the render target describing it */
        private sealed class SurfaceResources
        {
            public SKSurface Surface;
            public GRBackendRenderTarget RenderTarget;
            public GRContext Context;
            public SKImageInfo Info;
            public long PaintedSequence = -1;

            public static SurfaceResources Create(GRContext context, int width, int height)
            {
                SKImageInfo info = new SKImageInfo(width, height, SKColorType.Rgba8888, SKAlphaType.Premul);
                SKSurface surface = SKSurface.Create(context, false, info);
                if (surface == null)
                    return null;

                SurfaceResources resources = new SurfaceResources();
                resources.Surface = surface;
                resources.RenderTarget = new GRBackendRenderTarget(width, height, 0, 8, new GRGlFramebufferInfo(0, GL_RGBA8));
                resources.Context = context;
                resources.Info = info;
                return resources;
            }

            public bool Matches(GRContext context, int width, int height)
            {
                return ReferenceEquals(Context, context) && Info.Width == width && Info.Height == height;
            }

            public void Bury()
            {
                GHGLResourceGraveyard.Bury(Surface, Context);
                GHGLResourceGraveyard.Bury(RenderTarget, Context);
                Surface = null;
                RenderTarget = null;
            }
        }

        /* Taken by the render thread for the duration of a frame; null while in use or after release */
        private SurfaceResources _resources = null;
        private int _isAttached = 0;
        private int _invalidatePosted = 0;
        private Avalonia.Controls.TopLevel _topLevel = null;

        /* Stopwatch timestamp of the last raised hover move */
        private long _lastHoverMoveTicks = 0;

        /* Touch ids ACM has raised Pressed for; UI thread only */
        private readonly GHAvaloniaTouchTracker _touchTracker = new GHAvaloniaTouchTracker();

        /* UI thread: one number per Render call that asks for a paint, so a replay of the same render data,
           or a re-present, does not repaint */
        private long _renderRequestSequence = 0;

        /* Set by RequestRender and RequestPresent, taken by Render; any thread */
        private int _renderRequested = 0;
        private int _presentRequested = 0;
        /* Frame id claimed from GHFrameTimeline by the last RequestRender that claimed one, taken by Render */
        private long _claimedFrameId = 0;
        /* This control paints the map, whose paints the frame timeline tracks */
        private volatile bool _carriesFrameTimeline = false;
        /* UI thread: the frame id of the last draw operation that asked for a paint */
        private long _lastOperationFrameId = 0;
        /* The paint state handed over by the last request that gave one, taken by Render; any thread */
        private object _claimedPaintState = null;
        /* UI thread: the paint state of the last draw operation that asked for a paint */
        private object _lastOperationPaintState = null;
        /* Paint thread: the paint state of the request whose paint runs on this thread */
        [ThreadStatic] private static object _currentPaintState;
        /* Render thread: the last sequence whose frame id was handed to a paint */
        private long _ticketedSequence = -1;
        /* Render thread writes, UI thread reads: 1 when the last draw had a GPU context */
        private int _lastDrawUsedGpu = 0;

        /* Render thread only */
        private GRContext _reportedContext = null;
        private Action<GRContext> _reportedContextAction = null;

        /* Render thread only, shared by all GL controls */
        private static int _swapIntervalReported = 0;
#if LINUX
        private static IntPtr _swapIntervalDrawable = IntPtr.Zero;
        private static GHAvaloniaGLControl _presentSampler = null;
        private static long _presentSamplerArea = 0;
        private static int _presentSamplerFrames = 0;
#endif

        public void RequestRender()
        {
            RequestRender(false);
        }

        /* Asks for a paint and invalidates the visual. claimTimelineFrame takes the frame timeline's pending
           invalidation, UI thread, for the paint that follows; ACM's software rendering paints without a
           ticket, so it leaves the invalidation pending. */
        public void RequestRender(bool claimTimelineFrame)
        {
            RequestRender(claimTimelineFrame, null);
        }

        /* The same, with the state the paint should draw, as of this request; null keeps an earlier
           state no Render has taken yet. The paint reads it through CurrentPaintState. */
        public void RequestRender(bool claimTimelineFrame, object paintState)
        {
            if (paintState != null)
                Interlocked.Exchange(ref _claimedPaintState, paintState);
            Interlocked.Exchange(ref _renderRequested, 1);
            if (claimTimelineFrame && !ForceSoftwareRendering)
            {
                _carriesFrameTimeline = true;
                long frameId = GHFrameTimeline.ClaimPendingPaint();
                if (frameId > 0)
                    Interlocked.Exchange(ref _claimedFrameId, frameId);
            }
            InvalidateNowOrPosted();
        }

        // WORKAROUND(WA-AVALONIA-0021): An animation frame that commits nothing waits for a 16 ms timer
        /* Shows the last painted frame again without painting, so that the frame commits and the next
           animation frame follows the render loop. UI thread; false when nothing was asked for */
        public bool RequestPresent()
        {
            if (!AvDispatcher.UIThread.CheckAccess())
                return false;
            if (Volatile.Read(ref _isAttached) == 0 || !IsEffectivelyVisible)
                return false;
            /* base.Render repaints in full; a render loop renders continuously anyway; without a GPU
               context every draw repaints in full */
            if (ForceSoftwareRendering || HasRenderLoop || Volatile.Read(ref _lastDrawUsedGpu) == 0)
                return false;

            Interlocked.Exchange(ref _presentRequested, 1);
            InvalidateNowOrPosted();
            return true;
        }

        /* Invalidates the visual: at once on the UI thread, otherwise through one posted operation */
        private void InvalidateNowOrPosted()
        {
            if (AvDispatcher.UIThread.CheckAccess())
            {
                try
                {
                    InvalidateVisual();
                    return;
                }
                catch (InvalidOperationException)
                {
                    /* Raised inside Avalonia's render pass; the posted invalidation below runs after it */
                }
            }

            if (Interlocked.CompareExchange(ref _invalidatePosted, 1, 0) == 0)
                AvDispatcher.UIThread.Post(InvalidateFromPost, AvDispatcherPriority.Normal);
        }

        private void InvalidateFromPost()
        {
            Interlocked.Exchange(ref _invalidatePosted, 0);
            InvalidateVisual();
        }

        /* Zero unless MAUI requests a size, as with SkiaSharp's views on the other platforms */
        protected override Avalonia.Size MeasureOverride(Avalonia.Size availableSize)
        {
            return default(Avalonia.Size);
        }

        public override void Render(Avalonia.Media.DrawingContext context)
        {
            if (ForceSoftwareRendering)
            {
                GHAvaloniaFrameLoop.NoteFrameSubmitted();
                base.Render(context);
                return;
            }

            if (PaintSurfaceAction == null)
                return;

            AvRect bounds = Bounds;
            if (bounds.Width <= 0 || bounds.Height <= 0)
                return;

            double scaling = Avalonia.Controls.TopLevel.GetTopLevel(this)?.RenderScaling ?? 1.0;
            /* A re-present repeats the last operation's sequence, frame id and paint state; any other Render paints */
            bool renderRequested = Interlocked.Exchange(ref _renderRequested, 0) != 0;
            bool presentOnly = Interlocked.Exchange(ref _presentRequested, 0) != 0 && !renderRequested;
            if (!presentOnly)
            {
                _renderRequestSequence++;
                _lastOperationFrameId = Interlocked.Exchange(ref _claimedFrameId, 0);
                _lastOperationPaintState = Interlocked.Exchange(ref _claimedPaintState, null);
            }
            GHAvaloniaFrameLoop.NoteFrameSubmitted();
            context.Custom(new GLDrawOperation(this, new AvRect(bounds.Size), IgnorePixelScaling, scaling, _renderRequestSequence,
                _lastOperationFrameId, _carriesFrameTimeline, _lastOperationPaintState));
        }

        protected override void OnAttachedToVisualTree(Avalonia.VisualTreeAttachmentEventArgs e)
        {
            Interlocked.Exchange(ref _isAttached, 1);
            base.OnAttachedToVisualTree(e);
            _topLevel = Avalonia.Controls.TopLevel.GetTopLevel(this);
            if (_topLevel != null)
                _topLevel.ScalingChanged += TopLevel_ScalingChanged;
        }

        /* A detached control may never render again, so its surface goes to the graveyard now */
        protected override void OnDetachedFromVisualTree(Avalonia.VisualTreeAttachmentEventArgs e)
        {
            base.OnDetachedFromVisualTree(e);
            if (_topLevel != null)
            {
                _topLevel.ScalingChanged -= TopLevel_ScalingChanged;
                _topLevel = null;
            }
            Interlocked.Exchange(ref _isAttached, 0);
            ReleaseResources();
#if LINUX
            Interlocked.CompareExchange(ref _presentSampler, null, this);
#endif
        }

        private void TopLevel_ScalingChanged(object sender, EventArgs e)
        {
            InvalidateVisual();
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

        private void ReleaseResources()
        {
            SurfaceResources resources = Interlocked.Exchange(ref _resources, null);
            if (resources != null)
                resources.Bury();
        }

        /* Returns the resources after a frame, unless the control was detached meanwhile */
        private void KeepResources(SurfaceResources resources)
        {
            SurfaceResources previous = Interlocked.Exchange(ref _resources, resources);
            if (previous != null && !ReferenceEquals(previous, resources))
                previous.Bury();
            if (Volatile.Read(ref _isAttached) == 0)
                ReleaseResources();
        }

        /* Render thread. frameId is the frame timeline's id for the sequence's paint, 0 when none was claimed;
           carriesFrameTimeline hands it to the paint as a ticket. paintState is the request's state for the
           paint, or null. */
        private void RenderFrame(Avalonia.Media.ImmediateDrawingContext context, AvRect bounds, bool ignorePixelScaling, double scaling, long sequence,
            long frameId, bool carriesFrameTimeline, object paintState)
        {
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
                GRContext grContext = lease.GrContext;
                Volatile.Write(ref _lastDrawUsedGpu, grContext != null ? 1 : 0);

                int pixelWidth = (int)(bounds.Width * scaling);
                int pixelHeight = (int)(bounds.Height * scaling);

                if (grContext == null)
                {
                    /* No GPU context in this pass (software rendering or a raster render target):
                       paint into a raster surface for this frame, as ACM's control does */
                    ReportSwapIntervalOnce(GHSwapIntervalState.NotGlx, "no GPU context");
                    if (pixelWidth > 0 && pixelHeight > 0)
                        RenderRaster(canvas, bounds, ignorePixelScaling, scaling, pixelWidth, pixelHeight, sequence, frameId, carriesFrameTimeline, paintState);
                    return;
                }

                GHGLResourceGraveyard.Drain(grContext);
                ReportContext(grContext);
                UpdateSwapInterval();

                if (pixelWidth <= 0 || pixelHeight <= 0)
                    return;

                long drawStart = Stopwatch.GetTimestamp();
                double paintMs = -1.0;
                SurfaceResources resources = Interlocked.Exchange(ref _resources, null);
                try
                {
                    if (resources != null && !resources.Matches(grContext, pixelWidth, pixelHeight))
                    {
                        resources.Bury();
                        resources = null;
                    }
                    if (resources == null)
                    {
                        resources = SurfaceResources.Create(grContext, pixelWidth, pixelHeight);
                        if (resources == null)
                            return;
                    }

                    if (resources.PaintedSequence != sequence)
                    {
                        long paintStart = Stopwatch.GetTimestamp();
                        if (PaintGpu(resources, bounds, ignorePixelScaling, scaling, sequence, frameId, carriesFrameTimeline, paintState))
                            resources.PaintedSequence = sequence;
                        paintMs = (Stopwatch.GetTimestamp() - paintStart) * 1000.0 / Stopwatch.Frequency;
                    }

                    /* No flush: the lease canvas is on the same GRContext, which orders the two */
                    using (SKImage snapshot = resources.Surface.Snapshot())
                    {
                        DrawSnapshot(canvas, snapshot, scaling);
                    }
                }
                finally
                {
                    if (resources != null)
                        KeepResources(resources);
                }

                GHAvaloniaFrameLoop.RecordGlDraw(drawStart, paintMs);
                SamplePresentTiming((long)pixelWidth * pixelHeight);
            }
        }
#pragma warning restore CS0618

        /* The state the request of the paint running on this thread handed over, or null; read by paint
           handlers so that they draw the state as of their request rather than the live one */
        public static object CurrentPaintState { get { return _currentPaintState; } }

        /* Render thread: runs the paint with its request's paint state, and with the sequence's frame id as
           its ticket when the control carries the frame timeline. Only the first paint of a sequence gets the
           id; a repeat gets ticket 0. Every paint of a sequence gets its state. */
        private void InvokePaint(Action<SKPaintGLSurfaceEventArgs> paintAction, SKPaintGLSurfaceEventArgs args, long sequence,
            long frameId, bool carriesFrameTimeline, object paintState)
        {
            long ticket = sequence != _ticketedSequence ? frameId : 0;
            _ticketedSequence = sequence;
            _currentPaintState = paintState;
            try
            {
                if (!carriesFrameTimeline)
                {
                    paintAction(args);
                    return;
                }

                GHFrameTimeline.BeginTicketedPaint(ticket);
                try
                {
                    paintAction(args);
                }
                finally
                {
                    GHFrameTimeline.EndTicketedPaint();
                }
            }
            finally
            {
                _currentPaintState = null;
            }
        }

        private bool PaintGpu(SurfaceResources resources, AvRect bounds, bool ignorePixelScaling, double scaling, long sequence,
            long frameId, bool carriesFrameTimeline, object paintState)
        {
            Action<SKPaintGLSurfaceEventArgs> paintAction = PaintSurfaceAction;
            if (paintAction == null)
                return false;

            SKSurface surface = resources.Surface;
            SKCanvas surfaceCanvas = surface.Canvas;
            int saveCount = surfaceCanvas.Save();
            try
            {
                surfaceCanvas.Clear(SKColors.Transparent);
                SKImageInfo rawInfo = resources.Info;
                SKImageInfo info = rawInfo;
                if (ignorePixelScaling)
                {
                    info = new SKImageInfo((int)bounds.Width, (int)bounds.Height, rawInfo.ColorType, SKAlphaType.Premul);
                    surfaceCanvas.Scale((float)scaling);
                }
                InvokePaint(paintAction, new SKPaintGLSurfaceEventArgs(surface, resources.RenderTarget, GRSurfaceOrigin.BottomLeft, info, rawInfo),
                    sequence, frameId, carriesFrameTimeline, paintState);
            }
            finally
            {
                surfaceCanvas.RestoreToCount(saveCount);
            }
            return true;
        }

        private void RenderRaster(SKCanvas canvas, AvRect bounds, bool ignorePixelScaling, double scaling, int pixelWidth, int pixelHeight,
            long sequence, long frameId, bool carriesFrameTimeline, object paintState)
        {
            Action<SKPaintGLSurfaceEventArgs> paintAction = PaintSurfaceAction;
            if (paintAction == null)
                return;

            SKImageInfo rawInfo = new SKImageInfo(pixelWidth, pixelHeight, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
            using (SKSurface surface = SKSurface.Create(rawInfo))
            {
                if (surface == null)
                    return;

                SKImageInfo info = rawInfo;
                if (ignorePixelScaling)
                {
                    info = new SKImageInfo((int)bounds.Width, (int)bounds.Height, rawInfo.ColorType, SKAlphaType.Premul);
                    surface.Canvas.Scale((float)scaling);
                }

                using (GRBackendRenderTarget renderTarget = new GRBackendRenderTarget(pixelWidth, pixelHeight, 0, 8, new GRGlFramebufferInfo(0, GL_RGBA8)))
                {
                    InvokePaint(paintAction, new SKPaintGLSurfaceEventArgs(surface, renderTarget, GRSurfaceOrigin.BottomLeft, info, rawInfo),
                        sequence, frameId, carriesFrameTimeline, paintState);
                }

                using (SKImage snapshot = surface.Snapshot())
                {
                    DrawSnapshot(canvas, snapshot, scaling);
                }
            }
        }

        private static void DrawSnapshot(SKCanvas canvas, SKImage snapshot, double scaling)
        {
            if (snapshot == null)
                return;

            canvas.Save();
            canvas.Scale(1.0f / (float)scaling);
            canvas.DrawImage(snapshot, 0, 0, SKSamplingOptions.Default, null);
            canvas.Restore();
        }

        /* Passes a changed context, or a newly connected handler, to GRContextChangedAction */
        private void ReportContext(GRContext grContext)
        {
            Action<GRContext> action = GRContextChangedAction;
            if (ReferenceEquals(_reportedContext, grContext) && ReferenceEquals(_reportedContextAction, action))
                return;

            _reportedContext = grContext;
            _reportedContextAction = action;
            action?.Invoke(grContext);
        }

        private static void ReportSwapIntervalOnce(GHSwapIntervalState state, string detail)
        {
            if (Interlocked.CompareExchange(ref _swapIntervalReported, 1, 0) != 0)
                return;

            GHAvaloniaFrameLoop.SwapIntervalState = state;
            GHApp.MaybeWriteGHLog("Avalonia GL: SwapIntervalState " + state.ToString() + " (" + detail + ")");
        }

        /* Applies swap interval 1 whenever the current drawable differs from the one last handled */
        private static void UpdateSwapInterval()
        {
#if LINUX
            IntPtr display, drawable;
            if (!GHGlx.TryGetCurrent(out display, out drawable))
            {
                ReportSwapIntervalOnce(GHSwapIntervalState.NotGlx, "no current GLX context");
                return;
            }
            if (drawable == _swapIntervalDrawable)
                return;

            _swapIntervalDrawable = drawable;
            IntPtr appliedDrawable;
            string detail;
            GHSwapIntervalState state = GHGlx.ApplySwapInterval(out appliedDrawable, out detail);
            GHAvaloniaFrameLoop.SwapIntervalState = state;
            Interlocked.Exchange(ref _swapIntervalReported, 1);
            GHApp.MaybeWriteGHLog("Avalonia GL: SwapIntervalState " + state.ToString() + " for drawable 0x"
                + drawable.ToInt64().ToString("X") + " (" + detail + ")");
#else
            ReportSwapIntervalOnce(GHSwapIntervalState.NotGlx, "not a Linux build");
#endif
        }

        /* Linux: every GHAvaloniaFrameLoop.PresentSampleFrameInterval frames of the largest attached GL
           control (the map), one GLX_OML_sync_control sample while the swap interval is applied */
        private void SamplePresentTiming(long pixelArea)
        {
#if LINUX
            if (GHAvaloniaFrameLoop.SwapIntervalState != GHSwapIntervalState.Applied)
                return;

            GHAvaloniaGLControl sampler = Volatile.Read(ref _presentSampler);
            if (!ReferenceEquals(sampler, this))
            {
                if (sampler != null && pixelArea <= _presentSamplerArea)
                    return;
                Interlocked.Exchange(ref _presentSampler, this);
                _presentSamplerFrames = 0;
            }
            _presentSamplerArea = pixelArea;

            if (++_presentSamplerFrames < GHAvaloniaFrameLoop.PresentSampleFrameInterval)
                return;
            _presentSamplerFrames = 0;

            long ust, msc, sbc;
            if (GHGlx.TryGetSyncValues(out ust, out msc, out sbc))
                GHAvaloniaFrameLoop.RecordPresentSample(ust, msc, sbc);
#endif
        }

        private sealed class GLDrawOperation : Avalonia.Rendering.SceneGraph.ICustomDrawOperation
        {
            private readonly GHAvaloniaGLControl _owner;
            private readonly bool _ignorePixelScaling;
            private readonly double _scaling;
            private readonly long _sequence;
            private readonly long _frameId;
            private readonly bool _carriesFrameTimeline;
            private readonly object _paintState;

            public GLDrawOperation(GHAvaloniaGLControl owner, AvRect bounds, bool ignorePixelScaling, double scaling, long sequence,
                long frameId, bool carriesFrameTimeline, object paintState)
            {
                _owner = owner;
                Bounds = bounds;
                _ignorePixelScaling = ignorePixelScaling;
                _scaling = scaling;
                _sequence = sequence;
                _frameId = frameId;
                _carriesFrameTimeline = carriesFrameTimeline;
                _paintState = paintState;
            }

            public AvRect Bounds { get; }

            public void Dispose()
            {
            }

            public bool Equals(Avalonia.Rendering.SceneGraph.ICustomDrawOperation other) => ReferenceEquals(this, other);

            public bool HitTest(AvPoint p) => Bounds.Contains(p);

            public void Render(Avalonia.Media.ImmediateDrawingContext context)
            {
                _owner.RenderFrame(context, Bounds, _ignorePixelScaling, _scaling, _sequence, _frameId, _carriesFrameTimeline, _paintState);
            }
        }
    }
}

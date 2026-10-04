#if WINDOWS
#nullable enable

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Numerics;
using System.Threading;
using Microsoft.Graphics.DirectX;
using Microsoft.UI.Composition;
using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml.Hosting;
using SkiaSharp;
using Windows.Graphics;
using WinRT;
using static GnollHackM.AngleCompositionInterop;

namespace GnollHackM
{
    /* A Skia GL view that draws through ANGLE into a CompositionDrawingSurface shown by a child
       SpriteVisual. The visual is compositor content and blends with the XAML around it, which
       a SwapChainPanel cannot. Rendering is synchronous on the UI thread, as in
       SKSwapChainPanel.Invalidate. The visual and drawing surface are created on the first
       render, so hidden views cost nothing. */
    public sealed class GnhSKCompositionGLPanel : Microsoft.UI.Xaml.Controls.Grid
    {
        /* These two set the vertical orientation of the drawing; if the content appears upside
           down, change one of them */
        private static readonly GRSurfaceOrigin Origin = GRSurfaceOrigin.TopLeft;
        private static readonly bool InvertPbufferY = false;

        private const int MaxCachedTargets = 4;

        /* Set when ANGLE refuses the offset and size attributes; pbuffers then cover the whole
           texture and the paint is translated and clipped to the update rectangle */
        private static bool s_offsetAttributesUnsupported;
        private static bool s_offsetPathLogged;
        private static bool s_renderInProgress;
        private static bool s_beginDrawFailureLogged;
        private static bool s_endDrawFailureLogged;
        private static bool s_pbufferFailureLogged;
        private static bool s_skSurfaceFailureLogged;
        private static bool s_compositionFailureLogged;
        private static bool s_queuedRenderFailureLogged;

        /* A pbuffer over one update rectangle of one drawing surface texture. The texture is
           referenced so that its address cannot be reused while the entry exists. */
        private sealed class RenderTarget
        {
            public IntPtr Texture;
            public int OffsetX;
            public int OffsetY;
            public int KeyWidth;
            public int KeyHeight;
            public bool WholeTexture;
            public int Width;
            public int Height;
            public IntPtr Pbuffer;
            public GRBackendRenderTarget? BackendRenderTarget;
            public SKSurface? Surface;
            public long LastUse;
        }

        private readonly List<RenderTarget> _targets = new List<RenderTarget>(MaxCachedTargets);
        private int _targetsGeneration;
        private long _useCounter;

        private readonly DispatcherQueue _dispatcherQueue;
        private readonly DispatcherQueueHandler _queuedRenderHandler;
        private int _renderQueued;

        private CompositionDrawingSurface? _drawingSurface;
        private IntPtr _drawingSurfaceInterop;
        private CompositionSurfaceBrush? _brush;
        private SpriteVisual? _sprite;

        /* Render timing per panel, summarized in the log every two seconds while it renders */
        private static readonly bool s_logRenderTiming = true;
        private static readonly long s_timingWindowTicks = 2 * Stopwatch.Frequency;
        private static readonly double s_ticksToMs = 1000.0 / Stopwatch.Frequency;
        private static int s_panelCounter;

        private sealed class RenderTiming
        {
            public long WindowStart;
            public int Invalidates;
            public int QueuedInvalidates;
            public int Renders;
            public int CacheHits;
            public int CacheMisses;
            public int TextureChanges;
            public int NonZeroOffsets;
            public IntPtr LastTexture;
            public long BeginDrawTicks;
            public long TargetTicks;
            public long PaintTicks;
            public long FlushTicks;
            public long EndDrawTicks;
            public long TotalTicks;
            public long MaxTotalTicks;
            public long MaxBeginDrawTicks;
            public long MaxEndDrawTicks;
            public long LastRenderStart;
            public long MaxGapTicks;
        }

        private readonly int _panelId = Interlocked.Increment(ref s_panelCounter);
        private readonly RenderTiming _timing = new RenderTiming();

        private Microsoft.UI.Xaml.XamlRoot? _xamlRoot;
        private bool _isLoaded;
        private bool _isVisible;
        private bool _deviceEventsSubscribed;
        private int _pixelWidth;
        private int _pixelHeight;
        private double _scale = 1.0;

        public event EventHandler<SkiaSharp.Views.Maui.SKPaintGLSurfaceEventArgs>? PaintSurface;

        /* Raised when the shared GRContext is lost or replaced */
        public event EventHandler? GRContextChanged;

        public GnhSKCompositionGLPanel()
        {
            /* A child visual does not hit-test; the transparent background does */
            Background = new Microsoft.UI.Xaml.Media.SolidColorBrush(Microsoft.UI.Colors.Transparent);
            _dispatcherQueue = DispatcherQueue;
            _queuedRenderHandler = new DispatcherQueueHandler(OnQueuedRender);
            _isVisible = Visibility == Microsoft.UI.Xaml.Visibility.Visible;
            Loaded += OnLoaded;
            Unloaded += OnUnloaded;
            SizeChanged += OnSizeChanged;
            RegisterPropertyChangedCallback(Microsoft.UI.Xaml.UIElement.VisibilityProperty, OnVisibilityChanged);
        }

        public bool IgnorePixelScaling { get; set; }

        public double ContentsScale { get { return _scale; } }

        public GRContext? GRContext
        {
            get
            {
                AngleCompositionDevice? device = AngleCompositionDevice.Instance;
                return device != null && device.IsAvailable ? device.GRContext : null;
            }
        }

        public void Invalidate()
        {
            bool onUiThread = _dispatcherQueue.HasThreadAccess;
            if (s_logRenderTiming && onUiThread)
                _timing.Invalidates++;

            if (onUiThread && !s_renderInProgress)
            {
                Render();
                return;
            }

            if (s_logRenderTiming && onUiThread)
                _timing.QueuedInvalidates++;
            if (Interlocked.Exchange(ref _renderQueued, 1) == 0)
            {
                if (!_dispatcherQueue.TryEnqueue(_queuedRenderHandler))
                    Interlocked.Exchange(ref _renderQueued, 0);
            }
        }

        /* For the handler's disconnect; the shared GRContext is never disposed here */
        public void DisposeResources()
        {
            UnsubscribeDeviceEvents();
            UnsubscribeXamlRoot();
            ClearTargets();
            if (_sprite != null)
            {
                ElementCompositionPreview.SetElementChildVisual(this, null);
                _sprite.Dispose();
                _sprite = null;
            }
            if (_brush != null)
            {
                _brush.Dispose();
                _brush = null;
            }
            Release(_drawingSurfaceInterop);
            _drawingSurfaceInterop = IntPtr.Zero;
            if (_drawingSurface != null)
            {
                _drawingSurface.Dispose();
                _drawingSurface = null;
            }
        }

        private void OnLoaded(object sender, Microsoft.UI.Xaml.RoutedEventArgs e)
        {
            _isLoaded = true;
            Microsoft.UI.Xaml.XamlRoot? root = XamlRoot;
            if (root != _xamlRoot)
            {
                UnsubscribeXamlRoot();
                _xamlRoot = root;
                if (_xamlRoot != null)
                    _xamlRoot.Changed += OnXamlRootChanged;
            }
            SubscribeDeviceEvents();
            UpdateSize();
            Invalidate();
        }

        /* Unloaded can arrive after the Loaded of a re-parenting */
        private void OnUnloaded(object sender, Microsoft.UI.Xaml.RoutedEventArgs e)
        {
            if (IsLoaded)
                return;

            _isLoaded = false;
            UnsubscribeXamlRoot();
            UnsubscribeDeviceEvents();
        }

        private void OnSizeChanged(object sender, Microsoft.UI.Xaml.SizeChangedEventArgs e)
        {
            UpdateSize();
            Invalidate();
        }

        private void OnXamlRootChanged(Microsoft.UI.Xaml.XamlRoot sender, Microsoft.UI.Xaml.XamlRootChangedEventArgs args)
        {
            if (sender.RasterizationScale == _scale)
                return;

            UpdateSize();
            Invalidate();
        }

        private void OnVisibilityChanged(Microsoft.UI.Xaml.DependencyObject sender, Microsoft.UI.Xaml.DependencyProperty dp)
        {
            _isVisible = Visibility == Microsoft.UI.Xaml.Visibility.Visible;
            if (_isVisible)
                Invalidate();
        }

        private void UnsubscribeXamlRoot()
        {
            if (_xamlRoot != null)
            {
                _xamlRoot.Changed -= OnXamlRootChanged;
                _xamlRoot = null;
            }
        }

        private void SubscribeDeviceEvents()
        {
            if (_deviceEventsSubscribed)
                return;
            AngleCompositionDevice.DeviceLost += OnDeviceLost;
            AngleCompositionDevice.DeviceRecreated += OnDeviceRecreated;
            _deviceEventsSubscribed = true;
        }

        private void UnsubscribeDeviceEvents()
        {
            if (!_deviceEventsSubscribed)
                return;
            AngleCompositionDevice.DeviceLost -= OnDeviceLost;
            AngleCompositionDevice.DeviceRecreated -= OnDeviceRecreated;
            _deviceEventsSubscribed = false;
        }

        private void OnDeviceLost()
        {
            ClearTargets();
            GRContextChanged?.Invoke(this, EventArgs.Empty);
        }

        private void OnDeviceRecreated()
        {
            GRContextChanged?.Invoke(this, EventArgs.Empty);
            Invalidate();
        }

        /* Pixel size is the DIP size times the rasterization scale, rounded up; the sprite keeps
           the DIP size and the brush stretches the surface onto it */
        private void UpdateSize()
        {
            Microsoft.UI.Xaml.XamlRoot? root = XamlRoot;
            double scale = root != null ? root.RasterizationScale : 1.0;
            if (!(scale > 0))
                scale = 1.0;
            double width = ActualWidth;
            double height = ActualHeight;
            int pixelWidth = width > 0 ? (int)Math.Ceiling(width * scale) : 0;
            int pixelHeight = height > 0 ? (int)Math.Ceiling(height * scale) : 0;

            _scale = scale;
            if (_sprite != null)
                _sprite.Size = new Vector2((float)Math.Max(0, width), (float)Math.Max(0, height));

            if (pixelWidth == _pixelWidth && pixelHeight == _pixelHeight)
                return;

            _pixelWidth = pixelWidth;
            _pixelHeight = pixelHeight;
            ClearTargets();
            if (_drawingSurface != null && pixelWidth > 0 && pixelHeight > 0)
                _drawingSurface.Resize(new SizeInt32 { Width = pixelWidth, Height = pixelHeight });
        }

        /* A queued render has no caller to report to, so its exception is logged */
        private void OnQueuedRender()
        {
            Interlocked.Exchange(ref _renderQueued, 0);
            try
            {
                if (!s_renderInProgress)
                    Render();
                else
                    Invalidate();
            }
            catch (Exception ex)
            {
                if (!s_queuedRenderFailureLogged)
                {
                    s_queuedRenderFailureLogged = true;
                    GnollHackX.GHApp.MaybeWriteGHLog(
                        "GnhSKCompositionGLPanel: render failed: " + ex.GetType().Name + ": " + ex.Message,
                        true, GHConstants.SentryGnollHackGeneralCategoryName);
                }
            }
        }

        /* Without a paint handler, a BeginDraw would show undefined content */
        private void Render()
        {
            if (!_isLoaded || !_isVisible || _pixelWidth <= 0 || _pixelHeight <= 0 || PaintSurface == null)
                return;
            if (!AngleCompositionDevice.TryInitialize())
                return;

            AngleCompositionDevice? device = AngleCompositionDevice.Instance;
            if (device == null || !device.IsAvailable)
                return;
            if (_targetsGeneration != device.Generation)
                ClearTargets();
            if (!EnsureComposition(device))
                return;

            s_renderInProgress = true;
            try
            {
                RenderFrame(device);
            }
            finally
            {
                s_renderInProgress = false;
            }
        }

        private bool EnsureComposition(AngleCompositionDevice device)
        {
            if (_drawingSurface != null)
                return true;

            Compositor compositor = ElementCompositionPreview.GetElementVisual(this).Compositor;
            CompositionGraphicsDevice? graphicsDevice = device.GetGraphicsDevice(compositor);
            if (graphicsDevice == null)
                return false;

            CompositionDrawingSurface drawingSurface = graphicsDevice.CreateDrawingSurface2(
                new SizeInt32 { Width = _pixelWidth, Height = _pixelHeight },
                DirectXPixelFormat.B8G8R8A8UIntNormalized, DirectXAlphaMode.Premultiplied);
            IntPtr drawingSurfaceInterop;
            int hr = QueryInterface(((IWinRTObject)drawingSurface).NativeObject.ThisPtr,
                IID_ICompositionDrawingSurfaceInterop, out drawingSurfaceInterop);
            if (hr < 0)
            {
                drawingSurface.Dispose();
                LogOnce(ref s_compositionFailureLogged,
                    "GnhSKCompositionGLPanel: ICompositionDrawingSurfaceInterop query failed: 0x" + hr.ToString("X8"));
                return false;
            }

            CompositionSurfaceBrush brush = compositor.CreateSurfaceBrush(drawingSurface);
            brush.Stretch = CompositionStretch.Fill;
            brush.BitmapInterpolationMode = CompositionBitmapInterpolationMode.NearestNeighbor;
            SpriteVisual sprite = compositor.CreateSpriteVisual();
            sprite.Brush = brush;
            sprite.Size = new Vector2((float)Math.Max(0, ActualWidth), (float)Math.Max(0, ActualHeight));
            ElementCompositionPreview.SetElementChildVisual(this, sprite);

            _drawingSurface = drawingSurface;
            _drawingSurfaceInterop = drawingSurfaceInterop;
            _brush = brush;
            _sprite = sprite;
            return true;
        }

        /* EndDraw runs even when the paint handler throws; the exception then propagates to the
           caller of Invalidate, as from SKSwapChainPanel */
        private void RenderFrame(AngleCompositionDevice device)
        {
            long startTicks = Stopwatch.GetTimestamp();
            IntPtr texture;
            POINT offset;
            int hr = BeginDraw(_drawingSurfaceInterop, out texture, out offset);
            long beginDrawEndTicks = Stopwatch.GetTimestamp();
            if (s_logRenderTiming && hr >= 0 && texture != IntPtr.Zero)
            {
                RenderTiming timing = _timing;
                /* An idle period closes the window, so pauses do not count as gaps */
                if (timing.LastRenderStart != 0 && startTicks - timing.LastRenderStart > s_timingWindowTicks)
                {
                    if (timing.Renders > 0)
                        LogRenderTiming(timing.LastRenderStart);
                    timing.LastRenderStart = 0;
                    timing.WindowStart = 0;
                }
                if (timing.WindowStart == 0)
                    timing.WindowStart = startTicks;
                if (timing.LastRenderStart != 0 && startTicks - timing.LastRenderStart > timing.MaxGapTicks)
                    timing.MaxGapTicks = startTicks - timing.LastRenderStart;
                timing.LastRenderStart = startTicks;
                timing.Renders++;
                long beginDrawTicks = beginDrawEndTicks - startTicks;
                timing.BeginDrawTicks += beginDrawTicks;
                if (beginDrawTicks > timing.MaxBeginDrawTicks)
                    timing.MaxBeginDrawTicks = beginDrawTicks;
                if (texture != timing.LastTexture)
                {
                    if (timing.LastTexture != IntPtr.Zero)
                        timing.TextureChanges++;
                    timing.LastTexture = texture;
                }
                if (offset.X != 0 || offset.Y != 0)
                    timing.NonZeroOffsets++;
            }
            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
            {
                device.HandleDeviceLost();
                return;
            }
            if (hr < 0 || texture == IntPtr.Zero)
            {
                if (hr >= 0)
                    EndDraw(_drawingSurfaceInterop);
                LogOnce(ref s_beginDrawFailureLogged, "GnhSKCompositionGLPanel: BeginDraw failed: 0x" + hr.ToString("X8"));
                return;
            }

            device.EnterDraw();
            try
            {
                long targetStartTicks = Stopwatch.GetTimestamp();
                RenderTarget? target = GetOrCreateTarget(device, texture, offset.X, offset.Y);
                bool ready = target != null && device.MakeCurrent(target.Pbuffer) && EnsureSkiaSurface(device, target);
                if (s_logRenderTiming)
                    _timing.TargetTicks += Stopwatch.GetTimestamp() - targetStartTicks;
                if (ready)
                    Paint(device, target!, offset.X, offset.Y);
            }
            finally
            {
                long endDrawStartTicks = Stopwatch.GetTimestamp();
                hr = EndDraw(_drawingSurfaceInterop);
                long endTicks = Stopwatch.GetTimestamp();
                Release(texture);
                if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
                    device.HandleDeviceLost();
                else if (hr < 0)
                    LogOnce(ref s_endDrawFailureLogged, "GnhSKCompositionGLPanel: EndDraw failed: 0x" + hr.ToString("X8"));
                device.ExitDraw();

                if (s_logRenderTiming)
                {
                    RenderTiming timing = _timing;
                    long endDrawTicks = endTicks - endDrawStartTicks;
                    timing.EndDrawTicks += endDrawTicks;
                    if (endDrawTicks > timing.MaxEndDrawTicks)
                        timing.MaxEndDrawTicks = endDrawTicks;
                    long totalTicks = endTicks - startTicks;
                    timing.TotalTicks += totalTicks;
                    if (totalTicks > timing.MaxTotalTicks)
                        timing.MaxTotalTicks = totalTicks;
                    if (endTicks - timing.WindowStart >= s_timingWindowTicks)
                        LogRenderTiming(endTicks);
                }
            }
        }

        private void LogRenderTiming(long nowTicks)
        {
            RenderTiming t = _timing;
            int n = t.Renders > 0 ? t.Renders : 1;
            string message = string.Format(System.Globalization.CultureInfo.InvariantCulture,
                "GnhSKCompositionGLPanel #{0} {1}x{2}: {3:0.0} s, invalidates {4} (queued {5}), renders {6}, "
                + "cache hits {7} misses {8}, texture changes {9}, non-zero offsets {10}; "
                + "avg ms begin {11:0.00} target {12:0.00} paint {13:0.00} flush {14:0.00} end {15:0.00} total {16:0.00}; "
                + "max ms begin {17:0.00} end {18:0.00} total {19:0.00} gap {20:0.0}",
                _panelId, _pixelWidth, _pixelHeight, (nowTicks - t.WindowStart) * s_ticksToMs / 1000.0,
                t.Invalidates, t.QueuedInvalidates, t.Renders,
                t.CacheHits, t.CacheMisses, t.TextureChanges, t.NonZeroOffsets,
                t.BeginDrawTicks * s_ticksToMs / n, t.TargetTicks * s_ticksToMs / n, t.PaintTicks * s_ticksToMs / n,
                t.FlushTicks * s_ticksToMs / n, t.EndDrawTicks * s_ticksToMs / n, t.TotalTicks * s_ticksToMs / n,
                t.MaxBeginDrawTicks * s_ticksToMs, t.MaxEndDrawTicks * s_ticksToMs, t.MaxTotalTicks * s_ticksToMs,
                t.MaxGapTicks * s_ticksToMs);
            GnollHackX.GHApp.MaybeWriteGHLog(message);

            /* LastTexture and LastRenderStart carry over into the next window */
            t.WindowStart = nowTicks;
            t.Invalidates = 0;
            t.QueuedInvalidates = 0;
            t.Renders = 0;
            t.CacheHits = 0;
            t.CacheMisses = 0;
            t.TextureChanges = 0;
            t.NonZeroOffsets = 0;
            t.BeginDrawTicks = 0;
            t.TargetTicks = 0;
            t.PaintTicks = 0;
            t.FlushTicks = 0;
            t.EndDrawTicks = 0;
            t.TotalTicks = 0;
            t.MaxTotalTicks = 0;
            t.MaxBeginDrawTicks = 0;
            t.MaxEndDrawTicks = 0;
            t.MaxGapTicks = 0;
        }

        private void Paint(AngleCompositionDevice device, RenderTarget target, int offsetX, int offsetY)
        {
            SKSurface surface = target.Surface!;
            SKCanvas canvas = surface.Canvas;
            var rawInfo = new SKImageInfo(_pixelWidth, _pixelHeight, SKColorType.Rgba8888, SKAlphaType.Premul);
            SKImageInfo info = rawInfo;

            using (new SKAutoCanvasRestore(canvas, true))
            {
                if (target.WholeTexture)
                {
                    canvas.Translate(offsetX, offsetY);
                    canvas.ClipRect(new SKRect(0, 0, _pixelWidth, _pixelHeight));
                }
                if (IgnorePixelScaling)
                {
                    float scale = (float)_scale;
                    info = rawInfo.WithSize(new SKSizeI((int)(_pixelWidth / scale), (int)(_pixelHeight / scale)));
                    canvas.Scale(scale);
                    canvas.Save();
                }

                long paintStartTicks = Stopwatch.GetTimestamp();
                PaintSurface?.Invoke(this, new SkiaSharp.Views.Maui.SKPaintGLSurfaceEventArgs(
                    surface, target.BackendRenderTarget!, Origin, info, rawInfo));
                if (s_logRenderTiming)
                    _timing.PaintTicks += Stopwatch.GetTimestamp() - paintStartTicks;
            }

            /* The paint handler may have made another GL view's context current */
            if (!device.MakeCurrent(target.Pbuffer))
                return;

            long flushStartTicks = Stopwatch.GetTimestamp();
            GRContext? grContext = device.GRContext;
            if (grContext != null)
                grContext.Flush(true);
            glFlush();
            if (s_logRenderTiming)
                _timing.FlushTicks += Stopwatch.GetTimestamp() - flushStartTicks;
        }

        private RenderTarget? GetOrCreateTarget(AngleCompositionDevice device, IntPtr texture, int offsetX, int offsetY)
        {
            bool wholeTexture = s_offsetAttributesUnsupported;
            int keyX = wholeTexture ? 0 : offsetX;
            int keyY = wholeTexture ? 0 : offsetY;
            int keyWidth = wholeTexture ? 0 : _pixelWidth;
            int keyHeight = wholeTexture ? 0 : _pixelHeight;

            int i;
            for (i = 0; i < _targets.Count; i++)
            {
                RenderTarget cached = _targets[i];
                if (cached.Texture == texture && cached.WholeTexture == wholeTexture && cached.OffsetX == keyX
                    && cached.OffsetY == keyY && cached.KeyWidth == keyWidth && cached.KeyHeight == keyHeight)
                {
                    cached.LastUse = ++_useCounter;
                    if (s_logRenderTiming)
                        _timing.CacheHits++;
                    return cached;
                }
            }

            if (s_logRenderTiming)
                _timing.CacheMisses++;

            IntPtr pbuffer = CreatePbuffer(device, texture, offsetX, offsetY, _pixelWidth, _pixelHeight, wholeTexture);
            if (pbuffer == EGL_NO_SURFACE)
            {
                int error = eglGetError();
                if (wholeTexture || error != EGL_BAD_ATTRIBUTE)
                {
                    LogOnce(ref s_pbufferFailureLogged,
                        "GnhSKCompositionGLPanel: eglCreatePbufferFromClientBuffer failed: 0x" + error.ToString("X4"));
                    return null;
                }

                s_offsetAttributesUnsupported = true;
                wholeTexture = true;
                keyX = 0;
                keyY = 0;
                keyWidth = 0;
                keyHeight = 0;
                GnollHackX.GHApp.MaybeWriteGHLog(
                    "GnhSKCompositionGLPanel: texture offset attributes refused; pbuffers cover the whole texture",
                    true, GHConstants.SentryGnollHackGeneralCategoryName);
                pbuffer = CreatePbuffer(device, texture, offsetX, offsetY, _pixelWidth, _pixelHeight, true);
                if (pbuffer == EGL_NO_SURFACE)
                {
                    LogOnce(ref s_pbufferFailureLogged,
                        "GnhSKCompositionGLPanel: eglCreatePbufferFromClientBuffer failed: 0x" + eglGetError().ToString("X4"));
                    return null;
                }
            }
            else if (!wholeTexture && !s_offsetPathLogged)
            {
                s_offsetPathLogged = true;
                GnollHackX.GHApp.MaybeWriteGHLog("GnhSKCompositionGLPanel: pbuffers use texture offset attributes");
            }

            int width = _pixelWidth;
            int height = _pixelHeight;
            if (wholeTexture)
                QueryPbufferSize(device, pbuffer, ref width, ref height);

            if (_targets.Count >= MaxCachedTargets)
            {
                int oldest = 0;
                for (i = 1; i < _targets.Count; i++)
                {
                    if (_targets[i].LastUse < _targets[oldest].LastUse)
                        oldest = i;
                }
                ReleaseTarget(_targets[oldest], device);
                _targets.RemoveAt(oldest);
            }

            AddRef(texture);
            var target = new RenderTarget
            {
                Texture = texture,
                OffsetX = keyX,
                OffsetY = keyY,
                KeyWidth = keyWidth,
                KeyHeight = keyHeight,
                WholeTexture = wholeTexture,
                Width = width,
                Height = height,
                Pbuffer = pbuffer,
                LastUse = ++_useCounter,
            };
            _targets.Add(target);
            _targetsGeneration = device.Generation;
            return target;
        }

        private static unsafe IntPtr CreatePbuffer(AngleCompositionDevice device, IntPtr texture, int offsetX, int offsetY,
            int width, int height, bool wholeTexture)
        {
            int* attributes = stackalloc int[11];
            int n = 0;
            if (!wholeTexture)
            {
                attributes[n++] = EGL_TEXTURE_OFFSET_X_ANGLE;
                attributes[n++] = offsetX;
                attributes[n++] = EGL_TEXTURE_OFFSET_Y_ANGLE;
                attributes[n++] = offsetY;
                attributes[n++] = EGL_WIDTH;
                attributes[n++] = width;
                attributes[n++] = EGL_HEIGHT;
                attributes[n++] = height;
            }
            if (InvertPbufferY)
            {
                attributes[n++] = EGL_SURFACE_ORIENTATION_ANGLE;
                attributes[n++] = EGL_SURFACE_ORIENTATION_INVERT_Y_ANGLE;
            }
            attributes[n] = EGL_NONE;
            return eglCreatePbufferFromClientBuffer(device.Display, EGL_D3D_TEXTURE_ANGLE, texture, device.Config, attributes);
        }

        private static unsafe void QueryPbufferSize(AngleCompositionDevice device, IntPtr pbuffer, ref int width, ref int height)
        {
            int value = 0;
            if (eglQuerySurface(device.Display, pbuffer, EGL_WIDTH, &value) != EGL_FALSE && value > 0)
                width = value;
            value = 0;
            if (eglQuerySurface(device.Display, pbuffer, EGL_HEIGHT, &value) != EGL_FALSE && value > 0)
                height = value;
        }

        private static unsafe bool EnsureSkiaSurface(AngleCompositionDevice device, RenderTarget target)
        {
            if (target.Surface != null)
                return true;

            GRContext? grContext = device.GRContext;
            if (grContext == null)
                return false;

            int stencilBits = 0;
            glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
            target.BackendRenderTarget = new GRBackendRenderTarget(target.Width, target.Height, 0, stencilBits,
                new GRGlFramebufferInfo(0, GL_RGBA8));
            target.Surface = SKSurface.Create(grContext, target.BackendRenderTarget, Origin, SKColorType.Rgba8888);
            if (target.Surface == null)
            {
                target.BackendRenderTarget.Dispose();
                target.BackendRenderTarget = null;
                LogOnce(ref s_skSurfaceFailureLogged, "GnhSKCompositionGLPanel: SKSurface.Create failed");
                return false;
            }
            return true;
        }

        /* Pbuffers of an older device generation died with their display; only the managed
           objects and texture references are released for those */
        private void ClearTargets()
        {
            if (_targets.Count == 0)
                return;

            AngleCompositionDevice? device = AngleCompositionDevice.Instance;
            bool live = device != null && device.Display != IntPtr.Zero && device.Generation == _targetsGeneration;
            int i;
            for (i = 0; i < _targets.Count; i++)
                ReleaseTarget(_targets[i], live ? device : null);
            _targets.Clear();
        }

        private static void ReleaseTarget(RenderTarget target, AngleCompositionDevice? liveDevice)
        {
            target.Surface?.Dispose();
            target.Surface = null;
            target.BackendRenderTarget?.Dispose();
            target.BackendRenderTarget = null;
            if (liveDevice != null && target.Pbuffer != IntPtr.Zero)
            {
                liveDevice.ForgetSurface(target.Pbuffer);
                eglDestroySurface(liveDevice.Display, target.Pbuffer);
            }
            target.Pbuffer = IntPtr.Zero;
            Release(target.Texture);
            target.Texture = IntPtr.Zero;
        }

        private static void LogOnce(ref bool logged, string message)
        {
            if (logged)
                return;
            logged = true;
            GnollHackX.GHApp.MaybeWriteGHLog(message, true, GHConstants.SentryGnollHackGeneralCategoryName);
        }
    }
}
#endif

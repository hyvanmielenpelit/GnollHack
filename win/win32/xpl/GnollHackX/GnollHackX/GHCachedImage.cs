using SkiaSharp;
#if GNH_MAUI
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;
using GnollHackX;
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;
using GnollHackX.Controls;
using Xamarin.Essentials;
#endif
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public enum GHAspect
    {
        AspectFit = 0,
        AspectFill = 1,
        Fill = 2
    }

    public class GHCachedImage : SKCanvasView, IThreadSafeView
    {
        public GHCachedImage() : base()
        {
            SizeChanged += (object sender, EventArgs args) =>
            {
                InvalidateSurface();
            };
            PaintSurface += CustomCanvasView_PaintSurface;
            SizeChanged += GHCachedImage_SizeChanged;
            PropertyChanged += GHCachedImage_PropertyChanged;
            lock (_propertyLock)
            {
                _threadSafeWidth = Width;
                _threadSafeHeight = Height;
                _threadSafeX = X;
                _threadSafeY = Y;
                _threadSafeIsVisible = IsVisible;
                _threadSafeMargin = Margin;
                if (Parent == null || !(Parent is IThreadSafeView))
                    _threadSafeParent = null;
                else
                    _threadSafeParent = new WeakReference<IThreadSafeView>((IThreadSafeView)Parent);
            }
        }

        private readonly object _propertyLock = new object();
        private double _threadSafeWidth = 0;
        private double _threadSafeHeight = 0;
        private double _threadSafeX = 0;
        private double _threadSafeY = 0;
        private bool _threadSafeIsVisible = true;
        private Thickness _threadSafeMargin = new Thickness();
        WeakReference<IThreadSafeView> _threadSafeParent = null;

        public double ThreadSafeWidth { get {  /* lock (_propertyLock) */  { return _threadSafeWidth; } } /* private set { lock (_propertyLock) { _threadSafeWidth = value; } } */ }
        public double ThreadSafeHeight { get {  /* lock (_propertyLock) */ { return _threadSafeHeight; } } /* private set { lock (_propertyLock) { _threadSafeHeight = value; } } */ }
        public double ThreadSafeX { get { /* lock (_propertyLock) */ { return _threadSafeX; } } /* private set { lock (_propertyLock) { _threadSafeX = value; } } */ }
        public double ThreadSafeY { get { /* lock (_propertyLock) */  { return _threadSafeY; } } /* private set { lock (_propertyLock) { _threadSafeY = value; } } */ }
        public bool ThreadSafeIsVisible { get { lock (_propertyLock) { return _threadSafeIsVisible; } } private set { lock (_propertyLock) { _threadSafeIsVisible = value; } } }
        public Thickness ThreadSafeMargin { get { lock (_propertyLock) { return _threadSafeMargin; } } private set { lock (_propertyLock) { _threadSafeMargin = value; } } }
        public WeakReference<IThreadSafeView> ThreadSafeParent { get { lock (_propertyLock) { return _threadSafeParent; } } private set { lock (_propertyLock) { _threadSafeParent = value; } } }

        private void GHCachedImage_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(IsVisible))
            {
                ThreadSafeIsVisible = IsVisible;
            }
            else if (e.PropertyName == nameof(Width))
            {
                //ThreadSafeWidth = Width;
                Interlocked.Exchange(ref _threadSafeWidth, Width);
            }
            else if (e.PropertyName == nameof(Height))
            {
                //ThreadSafeWidth = Height;
                Interlocked.Exchange(ref _threadSafeHeight, Height);
            }
            else if (e.PropertyName == nameof(X))
            {
                //ThreadSafeX = X;
                Interlocked.Exchange(ref _threadSafeX, X);
            }
            else if (e.PropertyName == nameof(Y))
            {
                //ThreadSafeY = Y;
                Interlocked.Exchange(ref _threadSafeY, Y);
            }
            else if (e.PropertyName == nameof(Margin))
            {
                ThreadSafeMargin = Margin;
            }
            else if (e.PropertyName == nameof(Parent))
            {
                if (Parent == null || !(Parent is IThreadSafeView))
                    ThreadSafeParent = null;
                else
                    ThreadSafeParent = new WeakReference<IThreadSafeView>((IThreadSafeView)Parent);
            }
        }

        private void GHCachedImage_SizeChanged(object sender, EventArgs e)
        {
            lock (_propertyLock)
            {
                _threadSafeWidth = Width;
                _threadSafeHeight = Height;
            }
        }

        public bool CacheImage { get; set; } = true;

        public static readonly BindableProperty SourceProperty = BindableProperty.Create(nameof(Source), typeof(string), typeof(GHCachedImage), null, propertyChanged: OnSourceChanged);
        public string Source
        {
            get => (string)GetValue(SourceProperty);
            set => SetValue(SourceProperty, value);
        }
        private static void OnSourceChanged(BindableObject bindable, object oldValue, object newValue)
        {
            GHCachedImage img = bindable as GHCachedImage;
            string sourcePath = newValue as string;
            if (img != null)
            {
                lock (img._sourceBitmapLock)
                {
                    if (sourcePath != null && sourcePath != "")
                        img._sourceBitmap = GHApp.GetCachedImageSourceBitmap(sourcePath, img.CacheImage);
                    else
                        img._sourceBitmap = null;
                }
                img.InvalidateSurface();
            }
        }

        public static readonly BindableProperty ActiveGlyphImageSourceProperty = BindableProperty.Create(nameof(ActiveGlyphImageSource), typeof(GlyphImageSource), typeof(GHCachedImage), null, propertyChanged: OnImageSourceChanged);
        public GlyphImageSource ActiveGlyphImageSource
        {
            get => (GlyphImageSource)GetValue(ActiveGlyphImageSourceProperty);
            set { SetValue(ActiveGlyphImageSourceProperty, value); InvalidateSurface(); CheckStartAnimation(); }
        }

        private static void OnImageSourceChanged(BindableObject bindable, object oldValue, object newValue)
        {
            GHCachedImage img = bindable as GHCachedImage;
            if (img != null)
            {
                ImageSource oldImgSource = oldValue as GlyphImageSource;
                ImageSource newImgSource = newValue as GlyphImageSource;
                if(oldImgSource != newImgSource)
                {
                    if (newImgSource != null)
                        img.Source = null;
                    img.InvalidateSurface();
                }
            }
        }

        public void StopAnimation()
        {
            _StopAnimation = true;
        }

        private readonly object _stopAnimationLock = new object();
        private bool _stopAnimation = false;
        private bool _StopAnimation { get { lock (_stopAnimationLock) { return _stopAnimation; } } set { lock (_stopAnimationLock) { _stopAnimation = value; } } }

        private readonly object _timerOnLock = new object();
        private bool _timerOn = false;
        private bool _TimerOn { get { lock (_timerOnLock) { return _timerOn; } } set { lock (_timerOnLock) { _timerOn = value; } } }

        public void CheckStartAnimation()
        {
            if (ActiveGlyphImageSource != null)
            {
                int glyph = ActiveGlyphImageSource.Glyph;
                int absglyph = Math.Abs(glyph);
                int tile = 0;
                short anim = 0;
                //lock (GHApp.Glyph2TileLock)
                //{
                    if (GHApp.Glyph2Tile == null || GHApp.Tile2Animation == null)
                    {
                        if (_TimerOn)
                            _StopAnimation = true;
                        return;
                    }
                    tile = absglyph < GHApp.Glyph2Tile.Length ? GHApp.Glyph2Tile[absglyph] : 0;
                    anim = tile < GHApp.Tile2Animation.Length ? GHApp.Tile2Animation[tile] : (short)0;
                //}

                long _refreshFrequency = (long)Math.Min(60, UIUtils.GetGeneralAnimationFrequency());
                if(anim > 0 &&  !_TimerOn)
                {
                    _StopAnimation = false;
                    _TimerOn = true;
#if GNH_MAUI
                    var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                    timer.Interval = TimeSpan.FromSeconds(1.0 / _refreshFrequency);
                    timer.IsRepeating = true;
                    timer.Tick += (s, e) => { if(!DoStartCheck()) timer.Stop(); };
                    timer.Start();
#else
                    Device.StartTimer(TimeSpan.FromSeconds(1.0 / _refreshFrequency), () =>
                    {
                        return DoStartCheck();
                    });
#endif
                }
                else if (anim == 0 && _TimerOn)
                {
                    _StopAnimation = true;
                }
            } 
            else if (_TimerOn)
            {
                _StopAnimation = true;
            }
        }

        private bool DoStartCheck()
        {
            if (ActiveGlyphImageSource == null || _StopAnimation)
            {
                _TimerOn = false;
                return false;
            }
            int glyph = ActiveGlyphImageSource.Glyph;
            int absglyph = Math.Abs(glyph);
            int tile = 0;
            short anim = 0;
            //lock(GHApp.Glyph2TileLock)
            //{
                if (GHApp.Glyph2Tile == null || GHApp.Tile2Animation == null)
                {
                    _TimerOn = false;
                    return false;
                }
                tile = absglyph < GHApp.Glyph2Tile.Length ? GHApp.Glyph2Tile[absglyph] : 0;
                anim = tile < GHApp.Tile2Animation.Length ? GHApp.Tile2Animation[tile] : (short)0;
            //}
            _TimerOn = anim > 0;
            if (anim > 0)
            {
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    InvalidateSurface();
                });
            }
            return anim > 0;
        }

        public static readonly BindableProperty AspectProperty = BindableProperty.Create(nameof(Aspect), typeof(GHAspect), typeof(GHCachedImage), GHAspect.AspectFit, propertyChanged: OnAspectChanged);
        public GHAspect Aspect
        {
            get => (GHAspect)GetValue(AspectProperty);
            set => SetValue(AspectProperty, value);
        }
        private static void OnAspectChanged(BindableObject bindable, object oldValue, object newValue)
        {
            GHCachedImage img = bindable as GHCachedImage;
            if (img != null)
            {
                if (oldValue != null && newValue != null)
                {
                    try
                    {
                        GHAspect oldaspect = (GHAspect)oldValue;
                        GHAspect newaspect = (GHAspect)newValue;
                        if (newaspect != oldaspect)
                            img.InvalidateSurface();
                    }
                    catch
                    {

                    }
                }
                else
                    img.InvalidateSurface();
            }
        }

        public static readonly BindableProperty IsHighlightedProperty = BindableProperty.Create(nameof(IsHighlighted), typeof(bool), typeof(GHCachedImage), false, propertyChanged: OnIsHighlightedChanged);

        public bool IsHighlighted
        {
            get => (bool)GetValue(IsHighlightedProperty);
            set => SetValue(IsHighlightedProperty, value);
        }

        private static void OnIsHighlightedChanged(BindableObject bindable, object oldValue, object newValue)
        {
            GHCachedImage img = bindable as GHCachedImage;
            if (img != null)
                img.InvalidateSurface();
        }

        public static readonly BindableProperty HighFilterQualityProperty = BindableProperty.Create(nameof(HighFilterQuality), typeof(bool), typeof(GHCachedImage), false, propertyChanged: OnHighFilterQualityChanged);

        public bool HighFilterQuality
        {
            get => (bool)GetValue(HighFilterQualityProperty);
            set => SetValue(HighFilterQualityProperty, value);
        }

        private static void OnHighFilterQualityChanged(BindableObject bindable, object oldValue, object newValue)
        {
            GHCachedImage img = bindable as GHCachedImage;
            if (img != null && oldValue is bool && newValue is bool && (bool)oldValue != (bool)newValue)
                img.InvalidateSurface();
        }


        private readonly object _sourceBitmapLock = new object();
        private SKImage _sourceBitmap = null;

        private void CustomCanvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            SKImageInfo info = e.Info;
            canvas.Clear();
            float canvaswidth = info.Width;
            float canvasheight = info.Height;
            if (canvaswidth <= 0 || canvasheight <= 0)
                return;

            if(ActiveGlyphImageSource != null)
            {
                ActiveGlyphImageSource.Width = (int)canvaswidth;
                ActiveGlyphImageSource.Height = (int)canvasheight;
                ActiveGlyphImageSource.DrawOnCanvas(canvas, false, false, false, GHApp.FixRects);
            }
            else
            {
                SKImage targetBitmap = null;
                lock (_sourceBitmapLock)
                {
                    targetBitmap = _sourceBitmap;
                }
                if (targetBitmap == null)
                    return;

                SKRect sourcerect = new SKRect(0, 0, targetBitmap.Width, targetBitmap.Height);
                if (targetBitmap.Width <= 0 || targetBitmap.Height <= 0)
                    return;

                SKRect targetrect = new SKRect(0, 0, canvaswidth, canvasheight);
                bool widthsmaller = canvaswidth < canvasheight;
                float sourceWHRatio = (float)targetBitmap.Width / (float)targetBitmap.Height;
                switch (Aspect)
                {
                    case GHAspect.AspectFit:
                        {
                            float width = widthsmaller ? canvaswidth : canvasheight * sourceWHRatio;
                            float height = widthsmaller ? canvaswidth / sourceWHRatio : canvasheight;
                            float hpadding = (width - canvaswidth) / 2;
                            float vpadding = (height - canvasheight) / 2;
                            targetrect = new SKRect(-hpadding, -vpadding, -hpadding + width, -vpadding + height);
                            break;
                        }
                    case GHAspect.AspectFill:
                        {
                            float width = widthsmaller ? canvasheight * sourceWHRatio : canvaswidth;
                            float height = widthsmaller ? canvasheight : canvaswidth / sourceWHRatio;
                            float hpadding = (width - canvaswidth) / 2;
                            float vpadding = (height - canvasheight) / 2;
                            targetrect = new SKRect(-hpadding, -vpadding, -hpadding + width, -vpadding + height);
                            break;
                        }
                    default:
                    case GHAspect.Fill:
                        targetrect = new SKRect(0, 0, canvaswidth, canvasheight);
                        break;
                }
                using(SKPaint paint = new SKPaint())
                {
                    if (IsHighlighted)
                        paint.ColorFilter = UIUtils.HighlightColorFilter;
#if GNH_MAUI
                    canvas.DrawImage(targetBitmap, sourcerect, targetrect, new SKSamplingOptions(HighFilterQuality ? SKFilterMode.Linear : SKFilterMode.Nearest), paint);
#else
                    if (HighFilterQuality)
                        paint.FilterQuality = SKFilterQuality.High;
                    canvas.DrawImage(targetBitmap, sourcerect, targetrect, paint);
#endif
                }
            }
        }

        private double _currentWidth = 0;
        private double _currentHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentWidth || height != _currentHeight)
            {
                _currentWidth = width;
                _currentHeight = height;
                InvalidateSurface();
            }
        }

    }
}

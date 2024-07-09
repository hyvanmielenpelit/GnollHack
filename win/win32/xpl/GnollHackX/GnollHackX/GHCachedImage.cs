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

    public class GHCachedImage : SKCanvasView
    {
        public GHCachedImage() : base()
        {
            SizeChanged += (object sender, EventArgs args) =>
            {
                InvalidateSurface();
            };
            PaintSurface += CustomCanvasView_PaintSurface;
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
            if (ActiveGlyphImageSource != null && GHApp.Glyph2Tile != null && GHApp.Tile2Animation != null)
            {
                int glyph = ActiveGlyphImageSource.Glyph;
                int absglyph = Math.Abs(glyph);
                int tile = absglyph < GHApp.Glyph2Tile.Length ? GHApp.Glyph2Tile[absglyph] : 0;
                short anim = tile < GHApp.Tile2Animation.Length ? GHApp.Tile2Animation[tile] : (short)0;
                long _refreshFrequency = (long)Math.Min(60, UIUtils.GetAuxiliaryCanvasAnimationFrequency());
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
            if (ActiveGlyphImageSource == null || GHApp.Glyph2Tile == null || GHApp.Tile2Animation == null || _StopAnimation)
            {
                _TimerOn = false;
                return false;
            }
            int glyph = ActiveGlyphImageSource.Glyph;
            int absglyph = Math.Abs(glyph);
            int tile = absglyph < GHApp.Glyph2Tile.Length ? GHApp.Glyph2Tile[absglyph] : 0;
            short anim = tile < GHApp.Tile2Animation.Length ? GHApp.Tile2Animation[tile] : (short)0;
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



        private readonly object _sourceBitmapLock = new object();
        private SKImage _sourceBitmap = null;

        private void CustomCanvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            canvas.Clear();
            float canvaswidth = CanvasSize.Width;
            float canvasheight = CanvasSize.Height;
            if (canvaswidth <= 0 || canvasheight <= 0)
                return;

            if(ActiveGlyphImageSource != null)
            {
                ActiveGlyphImageSource.Width = (int)canvaswidth;
                ActiveGlyphImageSource.Height = (int)canvasheight;
                ActiveGlyphImageSource.DrawOnCanvas(canvas, false);
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
                    canvas.DrawImage(targetBitmap, sourcerect, targetrect, paint);
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

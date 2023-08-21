using SkiaSharp;
#if GNH_MAUI
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;
using GnollHackX;
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;
using GnollHackX.Controls;
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
            _stopAnimation = true;
        }

        private bool _stopAnimation = false;
        private bool _timerOn = false;
        public void CheckStartAnimation()
        {
            if (ActiveGlyphImageSource != null && GHApp.Glyph2Tile != null && GHApp.Tile2Animation != null)
            {
                int glyph = ActiveGlyphImageSource.Glyph;
                int absglyph = Math.Abs(glyph);
                int tile = absglyph < GHApp.Glyph2Tile.Length ? GHApp.Glyph2Tile[absglyph] : 0;
                int anim = tile < GHApp.Tile2Animation.Length ? GHApp.Tile2Animation[tile] : 0;
                long _refreshFrequency = (long)Math.Min(60, UIUtils.GetAuxiliaryCanvasAnimationFrequency());
                if(anim > 0 &&  !_timerOn)
                {
                    _stopAnimation = false;
                    _timerOn = true;
                    Device.StartTimer(TimeSpan.FromSeconds(1.0 / _refreshFrequency), () =>
                    {
                        if (ActiveGlyphImageSource == null ||  GHApp.Glyph2Tile == null || GHApp.Tile2Animation == null || _stopAnimation)
                        {
                            _timerOn = false;
                            return false;
                        }
                        glyph = ActiveGlyphImageSource.Glyph;
                        absglyph = Math.Abs(glyph);
                        tile = absglyph < GHApp.Glyph2Tile.Length ? GHApp.Glyph2Tile[absglyph] : 0;
                        anim = tile < GHApp.Tile2Animation.Length ? GHApp.Tile2Animation[tile] : 0;
                        if(anim > 0)
                            InvalidateSurface();
                        _timerOn = anim > 0;
                        return anim > 0;
                    });
                }
                else if (anim == 0 && _timerOn)
                {
                    _stopAnimation = true;
                }
            } 
            else if (_timerOn)
            {
                _stopAnimation = true;
            }
        }

        //private GHAspect _aspect = GHAspect.AspectFit;
        //public GHAspect Aspect
        //{
        //    get { return _aspect;  }
        //    set 
        //    { 
        //        if (_aspect != value) 
        //        { 
        //            _aspect = value; 
        //            InvalidateSurface(); 
        //        } 
        //    }
        //}


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

        private readonly object _sourceBitmapLock = new object();
        private SKBitmap _sourceBitmap = null;

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
                ActiveGlyphImageSource.DrawOnCanvas(canvas);
            }
            else
            {
                SKBitmap targetBitmap = null;
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
                canvas.DrawBitmap(targetBitmap, sourcerect, targetrect);
            }
        }
    }
}

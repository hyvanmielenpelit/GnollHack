using GnollHackX.Controls;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackX
{
    public enum GHAspect
    {
        AspectFit,
        AspectFill,
        Fill
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
                        img._sourceBitmap = App.GetCachedImageSourceBitmap(sourcePath);
                    else
                        img._sourceBitmap = null;
                }
                img.InvalidateSurface();
            }
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
                if(oldValue != null && newValue != null)
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
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            canvas.Clear();
            float canvaswidth = CanvasSize.Width;
            float canvasheight = CanvasSize.Height;
            if (canvaswidth <= 0 || canvasheight <= 0)
                return;

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
                    targetrect = new SKRect(0, 0, widthsmaller ? canvasheight * sourceWHRatio : canvaswidth, widthsmaller ? canvasheight : canvaswidth / sourceWHRatio);
                    break;
                case GHAspect.AspectFill:
                    float width = widthsmaller ? canvaswidth : canvasheight * sourceWHRatio;
                    float height = widthsmaller ? canvaswidth / sourceWHRatio : canvasheight;
                    float hpadding = (width - canvaswidth) / 2;
                    float vpadding = (height - canvasheight) / 2;
                    targetrect = new SKRect(-hpadding, -vpadding, -hpadding + width, -vpadding + height);
                    break;
                default:
                case GHAspect.Fill:
                    targetrect = new SKRect(0, 0, canvaswidth, canvasheight);
                    break;
            }
            canvas.DrawBitmap(targetBitmap, sourcerect, targetrect);
        }
    }
}

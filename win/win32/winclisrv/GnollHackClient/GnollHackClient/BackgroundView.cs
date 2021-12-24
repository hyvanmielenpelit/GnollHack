using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    class BackgroundView : SKCanvasView
    {
        public BackgroundView() : base()
        {
            PaintSurface += Base_PaintSurface;
        }
        public BackgroundView(SKBitmap bitmap) : base()
        {
            PaintSurface += Base_PaintSurface;
            SeamlessBitmap = bitmap;
        }
        public SKBitmap SeamlessBitmap { get; set; }

        private void Base_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = this.CanvasSize.Width;
            float canvasheight = this.CanvasSize.Height;
            float scale = canvaswidth / (float)this.Width;

            canvas.Clear();
            SKBitmap bmp = SeamlessBitmap != null ? SeamlessBitmap : App.DarkMarbleBackgroundBitmap;
            float bmpwidth = (float)bmp.Width;
            float bmpheight = (float)bmp.Height;
            if(bmpwidth > 0 && bmpheight > 0)
            {
                int xtiles = (int)Math.Ceiling(canvaswidth / bmpwidth);
                int ytiles = (int)Math.Ceiling(canvasheight / bmpheight);
                SKRect source_rect = new SKRect();
                SKRect target_rect = new SKRect();
                for (int x = 0; x < xtiles; x++)
                {
                    for(int y = 0; y < ytiles; y++)
                    {
                        target_rect.Left = x * bmpwidth;
                        target_rect.Top = y * bmpheight;
                        target_rect.Right = Math.Min(canvaswidth, (x + 1) * bmpwidth);
                        target_rect.Bottom = Math.Min(canvasheight, (y + 1) * bmpheight);
                        source_rect.Left = 0;
                        source_rect.Top = 0;
                        source_rect.Right = target_rect.Width;
                        source_rect.Bottom = target_rect.Height;
                        canvas.DrawBitmap(bmp, source_rect, target_rect);
                    }
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
                if(IsVisible)
                    InvalidateSurface();
            }
        }

    }
}

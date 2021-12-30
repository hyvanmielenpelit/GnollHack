using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackClient
{
    public enum BackgroundStyles
    {
        SeamlessBitmap = 0,
        StretchedBitmap
    }
    public enum BackgroundBitmaps
    {
        DarkMarble = 0,
        OldPaper,
        Scroll,
        SmallDarkMenu,
        Custom
    }
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

        public static readonly BindableProperty BackgroundStyleProperty = BindableProperty.Create(
            "BackgroundStyle", typeof(BackgroundStyles), typeof(BackgroundView), BackgroundStyles.SeamlessBitmap);

        public BackgroundStyles BackgroundStyle
        {
            get => (BackgroundStyles)GetValue(BackgroundStyleProperty);
            set => SetValue(BackgroundStyleProperty, value);
        }

        public static readonly BindableProperty BackgroundBitmapProperty = BindableProperty.Create(
            "BackgroundBitmap", typeof(BackgroundBitmaps), typeof(BackgroundView), BackgroundBitmaps.DarkMarble);

        public BackgroundBitmaps BackgroundBitmap
        {
            get => (BackgroundBitmaps)GetValue(BackgroundBitmapProperty);
            set => SetValue(BackgroundBitmapProperty, value);
        }

        private void Base_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = this.CanvasSize.Width;
            float canvasheight = this.CanvasSize.Height;
            float scale = canvaswidth / (float)this.Width;

            canvas.Clear();

            SKBitmap bmp = null;
            switch (BackgroundBitmap)
            {
                case BackgroundBitmaps.DarkMarble:
                    bmp = App.DarkMarbleBackgroundBitmap;
                    break;
                case BackgroundBitmaps.OldPaper:
                    bmp = App.OldPaperBackgroundBitmap;
                    break;
                case BackgroundBitmaps.Scroll:
                    bmp = App.ScrollBackgroundBitmap;
                    break;
                case BackgroundBitmaps.SmallDarkMenu:
                    bmp = App.MenuBackgroundBitmap;
                    break;
                case BackgroundBitmaps.Custom:
                    bmp = SeamlessBitmap != null ? SeamlessBitmap : App.DarkMarbleBackgroundBitmap;
                    break;
            }

            switch(BackgroundStyle)
            {
                case BackgroundStyles.SeamlessBitmap:
                    if (bmp != null)
                    {
                        float bmpwidth = (float)bmp.Width;
                        float bmpheight = (float)bmp.Height;
                        if (bmpwidth > 0 && bmpheight > 0)
                        {
                            int xtiles = (int)Math.Ceiling(canvaswidth / bmpwidth);
                            int ytiles = (int)Math.Ceiling(canvasheight / bmpheight);
                            SKRect source_rect = new SKRect();
                            SKRect target_rect = new SKRect();
                            for (int x = 0; x < xtiles; x++)
                            {
                                for (int y = 0; y < ytiles; y++)
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
                    break;
                case BackgroundStyles.StretchedBitmap:
                    if (bmp != null)
                    {
                        SKRect target_rect = new SKRect();
                        target_rect.Left = 0;
                        target_rect.Top = 0;
                        target_rect.Right = canvaswidth;
                        target_rect.Bottom = canvasheight;
                        canvas.DrawBitmap(bmp, target_rect);
                    }
                    break;
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

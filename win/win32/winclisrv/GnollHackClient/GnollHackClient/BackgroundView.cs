using GnollHackCommon;
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
        StretchedBitmap,
        FitToScreen
    }
    public enum BackgroundBitmaps
    {
        OldPaper = 0,
        LoadingScreen,
        SmallDarkMenu,
        Custom
    }
    public enum BorderStyles
    {
        None = 0,
        Simple,
        Small,
        Custom
    }
    class BackgroundView : SKCanvasView
    {
        public BackgroundView() : base()
        {
            PaintSurface += Base_PaintSurface;
        }
        public SKBitmap CustomBackgroundBitmap { get; set; }
        public SKBitmap CustomBorderTopLeftCorner { get; set; }
        public SKBitmap CustomBorderTopHorizontal { get; set; }
        public SKBitmap CustomBorderLeftVertical { get; set; }

        public static readonly BindableProperty BackgroundStyleProperty = BindableProperty.Create(
            "BackgroundStyle", typeof(BackgroundStyles), typeof(BackgroundView), BackgroundStyles.SeamlessBitmap);

        public BackgroundStyles BackgroundStyle
        {
            get { return (BackgroundStyles)GetValue(BackgroundStyleProperty); }
            set { SetValue(BackgroundStyleProperty, value); InvalidateSurface(); }
        }

        public static readonly BindableProperty BackgroundBitmapProperty = BindableProperty.Create(
            "BackgroundBitmap", typeof(BackgroundBitmaps), typeof(BackgroundView), BackgroundBitmaps.OldPaper);

        public BackgroundBitmaps BackgroundBitmap
        {
            get { return (BackgroundBitmaps)GetValue(BackgroundBitmapProperty); }
            set { SetValue(BackgroundBitmapProperty, value); InvalidateSurface(); }
        }

        public static readonly BindableProperty BorderStyleProperty = BindableProperty.Create(
            "BorderStyle", typeof(BorderStyles), typeof(BackgroundView), BorderStyles.None);

        public BorderStyles BorderStyle
        {
            get { return (BorderStyles)GetValue(BorderStyleProperty); }
            set { SetValue(BorderStyleProperty, value); InvalidateSurface(); }
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
                case BackgroundBitmaps.OldPaper:
                    bmp = App.OldPaperBackgroundBitmap;
                    break;
                case BackgroundBitmaps.LoadingScreen:
                    bmp = App.LoadingScreenBackgroundBitmap;
                    break;
                case BackgroundBitmaps.SmallDarkMenu:
                    bmp = App.MenuBackgroundBitmap;
                    break;
                case BackgroundBitmaps.Custom:
                    bmp = CustomBackgroundBitmap != null ? CustomBackgroundBitmap : App.OldPaperBackgroundBitmap;
                    break;
            }

            SKBitmap bordertl = null;
            SKBitmap borderhorizontal = null;
            SKBitmap bordervertical = null;
            switch (BorderStyle)
            {
                case BorderStyles.None:
                    break;
                case BorderStyles.Simple:
                    bordertl = App.SimpleFrameTopLeftCornerBitmap; ;
                    borderhorizontal = App.SimpleFrameTopHorizontalBitmap;
                    bordervertical = App.SimpleFrameLeftVerticalBitmap; ;
                    break;
                case BorderStyles.Small:
                    bordertl = App.SimpleFrameSmallTopLeftCornerBitmap; ;
                    borderhorizontal = App.SimpleFrameTopHorizontalBitmap;
                    bordervertical = App.SimpleFrameLeftVerticalBitmap; ;
                    break;
                case BorderStyles.Custom:
                    bordertl = CustomBorderTopLeftCorner;
                    borderhorizontal = CustomBorderTopHorizontal;
                    bordervertical = CustomBorderLeftVertical;
                    break;
            }


            switch (BackgroundStyle)
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
                case BackgroundStyles.FitToScreen:
                    if (bmp != null)
                    {
                        float bmpwidth = (float)bmp.Width;
                        float bmpheight = (float)bmp.Height;
                        if (bmpwidth > 0 && bmpheight > 0 && canvaswidth > 0 && canvasheight > 0)
                        {
                            float bmpaspectratio = bmpwidth / bmpheight;
                            float canvasaspectratio = canvaswidth / canvasheight;
                            bool fitheight = true;
                            if(canvasaspectratio > bmpaspectratio)
                            {
                                fitheight = false;
                            }
                            SKRect target_rect = new SKRect();
                            if(fitheight)
                            {
                                target_rect.Top = 0;
                                target_rect.Bottom = canvasheight;

                                float target_bmpwidth = canvasheight * bmpaspectratio;
                                float padding = (canvaswidth - target_bmpwidth) / 2;
                                target_rect.Left = padding;
                                target_rect.Right = target_rect.Left + target_bmpwidth;
                            }
                            else
                            {
                                target_rect.Left = 0;
                                target_rect.Right = canvaswidth;

                                float target_bmpheight = canvaswidth / bmpaspectratio;
                                float padding = (canvasheight - target_bmpheight) / 2;
                                target_rect.Top = padding;
                                target_rect.Bottom = target_rect.Top + target_bmpheight;
                            }
                            canvas.DrawBitmap(bmp, target_rect);
                        }
                    }
                    break;
            }

            if (BorderStyle > BorderStyles.None && bordertl != null && borderhorizontal != null && bordervertical != null)
            {
                float borderscalex = (canvaswidth / GHConstants.BackgroundBorderDivisor) / bordertl.Width;
                float borderscaley = (canvasheight / GHConstants.BackgroundBorderDivisor) / bordertl.Height;
                float borderscale = Math.Max(0.10f, Math.Min(1.0f, Math.Min(borderscalex, borderscaley)));
                for (int i = 0; i < 4; i++)
                {
                    float tx = 0, ty = 0;
                    bool hflip = false, vflip = false;
                    using (SKAutoCanvasRestore res = new SKAutoCanvasRestore(canvas, true))
                    {
                        switch (i)
                        {
                            case 0:
                                tx = 0;
                                ty = 0;
                                break;
                            case 1:
                                tx = canvaswidth - bordertl.Width * borderscale;
                                ty = 0;
                                hflip = true;
                                break;
                            case 2:
                                tx = canvaswidth - bordertl.Width * borderscale;
                                ty = canvasheight - bordertl.Height * borderscale;
                                hflip = true;
                                vflip = true;
                                break;
                            case 3:
                                tx = 0;
                                ty = canvasheight - bordertl.Height * borderscale;
                                vflip = true;
                                break;
                            default:
                                break;
                        }
                        SKRect target_rect = new SKRect();
                        target_rect.Left = 0;
                        target_rect.Top = 0;
                        target_rect.Right = bordertl.Width * borderscale;
                        target_rect.Bottom = bordertl.Height * borderscale;
                        canvas.Translate(tx + (hflip ? target_rect.Width : 0), ty + (vflip ? target_rect.Height : 0));
                        canvas.Scale(hflip ? -1 : 1, vflip ? -1 : 1, 0, 0);
                        canvas.DrawBitmap(bordertl, target_rect);
                    }
                }

                for (int i = 0; i < 2; i++)
                {
                    float tx = bordertl.Width * borderscale, ty = 0;
                    bool hflip = false, vflip = false;
                    using (SKAutoCanvasRestore res = new SKAutoCanvasRestore(canvas, true))
                    {
                        if(i == 1)
                        {
                            ty = canvasheight - borderhorizontal.Height * borderscale;
                            vflip = true;
                        }
                        SKRect target_rect = new SKRect();
                        target_rect.Left = 0;
                        target_rect.Top = 0;
                        target_rect.Right = canvaswidth - bordertl.Width * borderscale - tx;
                        target_rect.Bottom = borderhorizontal.Height * borderscale;
                        canvas.Translate(tx + (hflip ? target_rect.Width : 0), ty + (vflip ? target_rect.Height : 0));
                        canvas.Scale(hflip ? -1 : 1, vflip ? -1 : 1, 0, 0);
                        canvas.DrawBitmap(borderhorizontal, target_rect);
                    }
                }

                for (int i = 0; i < 2; i++)
                {
                    float tx = 0, ty = bordertl.Height * borderscale;
                    bool hflip = false, vflip = false;
                    using (SKAutoCanvasRestore res = new SKAutoCanvasRestore(canvas, true))
                    {
                        if (i == 1)
                        {
                            tx = canvaswidth - bordervertical.Width * borderscale;
                            hflip = true;
                        }
                        SKRect target_rect = new SKRect();
                        target_rect.Left = 0;
                        target_rect.Top = 0;
                        target_rect.Right = bordervertical.Width * borderscale;
                        target_rect.Bottom = canvasheight - bordertl.Height * borderscale - ty;
                        canvas.Translate(tx + (hflip ? target_rect.Width : 0), ty + (vflip ? target_rect.Height : 0));
                        canvas.Scale(hflip ? -1 : 1, vflip ? -1 : 1, 0, 0);
                        canvas.DrawBitmap(bordervertical, target_rect);
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

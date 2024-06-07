using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;
using GnollHackX;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    public enum BackgroundStyles
    {
        SeamlessBitmap = 0,
        StretchedBitmap,
        FitToScreen,
        Automatic
    }
    public enum BackgroundBitmaps
    {
        OldPaper = 0,
        LoadingScreen,
        DarkMarble,
        Custom,
        AutoMenuBackground,
    }
    class BackgroundView : SKCanvasView
    {
        public BackgroundView() : base()
        {
            PaintSurface += Base_PaintSurface;
        }
        public SKImage CustomBackgroundBitmap { get; set; }
        public SKImage CustomBorderTopLeftCorner { get; set; }
        public SKImage CustomBorderTopHorizontal { get; set; }
        public SKImage CustomBorderLeftVertical { get; set; }

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

            SKImage bmp = null;
            BackgroundBitmaps bitmapStyle = BackgroundBitmap;
            switch (bitmapStyle)
            {
                case BackgroundBitmaps.OldPaper:
                    bmp = GHApp.OldPaperBackgroundBitmap;
                    break;
                case BackgroundBitmaps.LoadingScreen:
                    bmp = GHApp.LoadingScreenBackgroundBitmap;
                    break;
                case BackgroundBitmaps.DarkMarble:
                    bmp = GHApp.DarkMarbleBackgroundBitmap;
                    break;
                case BackgroundBitmaps.Custom:
                    bmp = CustomBackgroundBitmap != null ? CustomBackgroundBitmap :  GHApp.OldPaperBackgroundBitmap;
                    break;
                case BackgroundBitmaps.AutoMenuBackground:
                    bmp = GHApp.DarkMode ? GHApp.DarkMarbleBackgroundBitmap : GHApp.OldPaperBackgroundBitmap;
                    break;
            }

            SKImage bordertl = null;
            SKImage borderhorizontal = null;
            SKImage bordervertical = null;
            if(GHApp.DarkMode)
            {
                switch (BorderStyle)
                {
                    case BorderStyles.None:
                        break;
                    case BorderStyles.Simple:
                        bordertl = GHApp.DarkModeSimpleFrameTopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.DarkModeSimpleFrameTopHorizontalBitmap;
                        bordervertical = GHApp.DarkModeSimpleFrameLeftVerticalBitmap; ;
                        break;
                    case BorderStyles.Small:
                        bordertl = GHApp.DarkModeSimpleFrameSmallTopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.DarkModeSimpleFrameTopHorizontalBitmap;
                        bordervertical = GHApp.DarkModeSimpleFrameLeftVerticalBitmap; ;
                        break;
                    case BorderStyles.SimpleAlternative:
                        bordertl = GHApp.DarkModeSimpleFrame2TopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.DarkModeSimpleFrame2TopHorizontalBitmap;
                        bordervertical = GHApp.DarkModeSimpleFrame2LeftVerticalBitmap; ;
                        break;
                    case BorderStyles.SmallAlternative:
                        bordertl = GHApp.DarkModeSimpleFrame2SmallTopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.DarkModeSimpleFrame2TopHorizontalBitmap;
                        bordervertical = GHApp.DarkModeSimpleFrame2LeftVerticalBitmap; ;
                        break;
                    case BorderStyles.Custom:
                        bordertl = CustomBorderTopLeftCorner;
                        borderhorizontal = CustomBorderTopHorizontal;
                        bordervertical = CustomBorderLeftVertical;
                        break;
                }
            }
            else
            {
                switch (BorderStyle)
                {
                    case BorderStyles.None:
                        break;
                    case BorderStyles.Simple:
                        bordertl = GHApp.SimpleFrameTopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.SimpleFrameTopHorizontalBitmap;
                        bordervertical = GHApp.SimpleFrameLeftVerticalBitmap; ;
                        break;
                    case BorderStyles.Small:
                        bordertl = GHApp.SimpleFrameSmallTopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.SimpleFrameTopHorizontalBitmap;
                        bordervertical = GHApp.SimpleFrameLeftVerticalBitmap; ;
                        break;
                    case BorderStyles.SimpleAlternative:
                        bordertl = GHApp.SimpleFrame2TopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.SimpleFrame2TopHorizontalBitmap;
                        bordervertical = GHApp.SimpleFrame2LeftVerticalBitmap; ;
                        break;
                    case BorderStyles.SmallAlternative:
                        bordertl = GHApp.SimpleFrame2SmallTopLeftCornerBitmap; ;
                        borderhorizontal = GHApp.SimpleFrame2TopHorizontalBitmap;
                        bordervertical = GHApp.SimpleFrame2LeftVerticalBitmap; ;
                        break;
                    case BorderStyles.Custom:
                        bordertl = CustomBorderTopLeftCorner;
                        borderhorizontal = CustomBorderTopHorizontal;
                        bordervertical = CustomBorderLeftVertical;
                        break;
                }
            }

            BackgroundStyles bkgStyle = BackgroundStyle;
            if(bkgStyle == BackgroundStyles.Automatic)
            {
                if (bitmapStyle == BackgroundBitmaps.AutoMenuBackground)
                    bkgStyle = GHApp.DarkMode ? BackgroundStyles.SeamlessBitmap : BackgroundStyles.StretchedBitmap;
                else
                    bkgStyle = BackgroundStyles.StretchedBitmap;
            }

            switch (bkgStyle)
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
                                    canvas.DrawImage(bmp, source_rect, target_rect);
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
                        canvas.DrawImage(bmp, target_rect);
                    }
                    break;
                default:
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
                            canvas.DrawImage(bmp, target_rect);
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
                        canvas.DrawImage(bordertl, target_rect);
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
                        canvas.DrawImage(borderhorizontal, target_rect);
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
                        canvas.DrawImage(bordervertical, target_rect);
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

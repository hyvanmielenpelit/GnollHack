using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Essentials;
using Xamarin.Forms;

namespace GnollHackClient
{
    public enum CustomLabelFonts
    {
        Diablo,
        Immortal
    }

    public class CustomLabel : SKCanvasView
    {
        public CustomLabel() : base()
        {
            PaintSurface += Base_PaintSurface;
        }

        public static readonly BindableProperty TextProperty = BindableProperty.Create(
            "Text", typeof(string), typeof(CustomLabel), "");

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty FontFamilyProperty = BindableProperty.Create(
            "FontFamily", typeof(CustomLabelFonts), typeof(CustomLabel), CustomLabelFonts.Diablo);

        public CustomLabelFonts FontFamily
        {
            get { return (CustomLabelFonts)GetValue(FontFamilyProperty); }
            set { SetValue(FontFamilyProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty FontSizeProperty = BindableProperty.Create(
            "FontSize", typeof(double), typeof(CustomLabel), 12.0);

        public double FontSize
        {
            get { return (double)GetValue(FontSizeProperty); }
            set { SetValue(FontSizeProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty OutlineWidthProperty = BindableProperty.Create(
            "OutlineWidth", typeof(double), typeof(CustomLabel), 0.0);

        public double OutlineWidth
        {
            get { return (double)GetValue(OutlineWidthProperty); }
            set { SetValue(OutlineWidthProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty TextColorProperty = BindableProperty.Create(
            "TextColor", typeof(Color), typeof(CustomLabel), Color.White);

        public Color TextColor
        {
            get { return (Color)GetValue(TextColorProperty); }
            set { SetValue(TextColorProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty OutlineColorProperty = BindableProperty.Create(
            "OutlineColor", typeof(Color), typeof(CustomLabel), Color.Black);

        public Color OutlineColor
        {
            get { return (Color)GetValue(OutlineColorProperty); }
            set { SetValue(OutlineColorProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty HorizontalTextAlignmentProperty = BindableProperty.Create(
            "HorizontalTextAlignment", typeof(TextAlignment), typeof(CustomLabel), TextAlignment.Start);

        public TextAlignment HorizontalTextAlignment
        {
            get { return (TextAlignment)GetValue(HorizontalTextAlignmentProperty); }
            set { SetValue(HorizontalTextAlignmentProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty VerticalTextAlignmentProperty = BindableProperty.Create(
            "VerticalTextAlignment", typeof(TextAlignment), typeof(CustomLabel), TextAlignment.Center);

        public TextAlignment VerticalTextAlignment
        {
            get { return (TextAlignment)GetValue(VerticalTextAlignmentProperty); }
            set { SetValue(VerticalTextAlignmentProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty UseSpecialSymbolsProperty = BindableProperty.Create(
            "UseSpecialSymbols", typeof(bool), typeof(CustomLabel), false);

        public bool UseSpecialSymbols
        {
            get { return (bool)GetValue(UseSpecialSymbolsProperty); }
            set { SetValue(UseSpecialSymbolsProperty, value); }
        }


        private void UpdateLabel()
        {
            InvalidateSurface();
        }

        private SizeRequest CalculateLabelSize(double widthConstraint, double heightConstraint)
        {
            double wr, hr;
            if (WidthRequest > 0 && HeightRequest > 0)
            {
                wr = Math.Min(widthConstraint, WidthRequest);
                hr = Math.Min(heightConstraint, HeightRequest);
            }
            else
            {
                using (SKPaint textPaint = new SKPaint())
                {
                    float scale = App.DisplayScale;
                    textPaint.TextSize = (float)FontSize * scale;
                    textPaint.Typeface = GetFontTypeface();
                    if (OutlineWidth > 0)
                    {
                        textPaint.Style = SKPaintStyle.Stroke;
                        textPaint.StrokeWidth = (float)OutlineWidth * scale;
                    }
                    SKRect bounds = new SKRect();
                    float width = textPaint.MeasureText(Text, ref bounds);
                    if (WidthRequest > 0)
                        wr = Math.Min(widthConstraint, WidthRequest);
                    else
                        wr = Math.Min(widthConstraint, (double)(width / scale));

                    if (HeightRequest > 0)
                        hr = Math.Min(heightConstraint, HeightRequest);
                    else
                        hr = Math.Min(heightConstraint, (double)(bounds.Height / scale));
                }
            }
            return new SizeRequest(new Size(wr, hr));
        }

        private SKTypeface GetFontTypeface()
        {
            SKTypeface tf = App.DiabloTypeface;
            switch (FontFamily)
            {
                default:
                case CustomLabelFonts.Diablo:
                    tf = App.DiabloTypeface;
                    break;
                case CustomLabelFonts.Immortal:
                    tf = App.ImmortalTypeface;
                    break;
            }
            return tf;
        }

        private void Base_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = this.CanvasSize.Width;
            float canvasheight = this.CanvasSize.Height;
            float scale = this.Width == 0 ? 1.0f : canvaswidth / (float)this.Width;

            canvas.Clear();

            using(SKPaint textPaint = new SKPaint())
            {
                float x = 0, y = 0;
                textPaint.Typeface = GetFontTypeface();
                textPaint.TextSize = (float)FontSize * scale;
                float textwidth = textPaint.MeasureText(Text);
                float maxtextwidth = 0.98f * canvaswidth;
                if (textwidth > maxtextwidth)
                    textPaint.TextSize = textPaint.TextSize * maxtextwidth / textwidth;

                switch (HorizontalTextAlignment)
                {
                    default:
                    case TextAlignment.Start:
                        x = 0;
                        textPaint.TextAlign = SKTextAlign.Left;
                        break;
                    case TextAlignment.Center:
                        x = canvaswidth / 2;
                        textPaint.TextAlign = SKTextAlign.Center;
                        break;
                    case TextAlignment.End:
                        x = canvaswidth;
                        textPaint.TextAlign = SKTextAlign.Right;
                        break;
                }

                switch (VerticalTextAlignment)
                {
                    case TextAlignment.Start:
                        y = -textPaint.FontMetrics.Ascent;
                        break;
                    default:
                    case TextAlignment.Center:
                        y = canvasheight / 2 - (textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent) / 2 - textPaint.FontMetrics.Ascent;
                        break;
                    case TextAlignment.End:
                        y = canvasheight - textPaint.FontMetrics.Descent;
                        break;
                }

                if(OutlineWidth > 0)
                {
                    textPaint.Style = SKPaintStyle.Stroke;
                    textPaint.StrokeWidth = (float)OutlineWidth * scale;
                    SKColor outlinecolor = new SKColor((byte)(255 * OutlineColor.R), (byte)(255 * OutlineColor.G), (byte)(255 * OutlineColor.B), (byte)(255 * OutlineColor.A));
                    textPaint.Color = outlinecolor;
                    canvas.DrawText(Text, x, y, textPaint);
                }

                textPaint.Style = SKPaintStyle.Fill;
                SKColor fillcolor = new SKColor((byte)(255 * TextColor.R), (byte)(255 * TextColor.G), (byte)(255 * TextColor.B), (byte)(255 * TextColor.A));
                textPaint.Color = fillcolor;
                canvas.DrawText(Text, x, y, textPaint);
            }

        }

        protected override SizeRequest OnMeasure(double widthConstraint, double heightConstraint)
        {
            return CalculateLabelSize(widthConstraint, heightConstraint);
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
                if (IsVisible)
                    UpdateLabel();
            }
        }

    }
}

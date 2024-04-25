using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using static System.Net.Mime.MediaTypeNames;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public class GHSkiaFontPaint : IDisposable
    {
        SKPaint _paint = new SKPaint();
#if GNH_MAUI
        SKFont _font = new SKFont();
#endif

        public GHSkiaFontPaint()
        {
            _paint.IsAntialias = true;
        }

        public void Dispose()
        {
            _paint.Dispose();
#if GNH_MAUI
            _font.Dispose();
#endif
        }

        public SKPaint Paint { get { return _paint; } }

        public SKTypeface Typeface
        {
            get
            {
#if GNH_MAUI
                return _font.Typeface;
#else
                return _paint.Typeface;
#endif
            }
            set
            {
#if GNH_MAUI
                _font.Typeface = value;
#else
                _paint.Typeface = value;
#endif
            }
        }

        public float TextSize
        {
            get
            {
#if GNH_MAUI
                return _font.Size;
#else
                return _paint.TextSize;
#endif
            }
            set
            {
#if GNH_MAUI
                _font.Size = value;
#else
                _paint.TextSize = value;
#endif
            }
        }

        public SKPaintStyle Style
        {
            get
            {
                return _paint.Style;
            }
            set
            {
                _paint.Style = value;
            }
        }

        public float StrokeWidth
        {
            get
            {
                return _paint.StrokeWidth;
            }
            set
            {
                _paint.StrokeWidth = value;
            }
        }

        public SKPathEffect PathEffect
        {
            get
            {
                return _paint.PathEffect;
            }
            set
            {
                _paint.PathEffect = value;
            }
        }

        public SKMaskFilter MaskFilter
        {
            get
            {
                return _paint.MaskFilter;
            }
            set
            {
                _paint.MaskFilter = value;
            }
        }

        public SKColor Color
        {
            get
            {
                return _paint.Color;
            }
            set
            {
                _paint.Color = value;
            }
        }

        public SKFontMetrics FontMetrics
        {
            get
            {
#if GNH_MAUI
                return _font.Metrics;
#else
                return _paint.FontMetrics;
#endif
            }
        }

        public float FontSpacing
        {
            get
            {
#if GNH_MAUI
                return _font.Spacing;
#else
                return _paint.FontSpacing;
#endif
            }
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, float x, float y, SKTextAlign textAlign)
        {
#if GNH_MAUI
            canvas.DrawText(text, x, y, textAlign, _font, _paint);
#else
            SKTextAlign oldAlign = _paint.TextAlign;
            _paint.TextAlign = textAlign;
            canvas.DrawText(text, x, y, _paint);
            _paint.TextAlign = oldAlign;
#endif
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, SKPoint p, SKTextAlign textAlign)
        {
            DrawTextOnCanvas(canvas, text, p.X, p.Y, textAlign);
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, float x, float y)
        {
#if GNH_MAUI
            canvas.DrawText(text, x, y, _font, _paint);
#else
            canvas.DrawText(text, x, y, _paint);
#endif
        }

        public void DrawTextOnCanvas(SKCanvas canvas, string text, SKPoint p)
        {
            DrawTextOnCanvas(canvas, text, p.X, p.Y);
        }

        public float MeasureText(string text)
        {
#if GNH_MAUI
            return _font.MeasureText(text, _paint);
#else
            return _paint.MeasureText(text);
#endif
        }

        public float MeasureText(string text, ref SKRect bounds)
        {
#if GNH_MAUI
            return _font.MeasureText(text, out bounds, _paint);
#else
            return _paint.MeasureText(text, ref bounds);
#endif
        }
    }
}

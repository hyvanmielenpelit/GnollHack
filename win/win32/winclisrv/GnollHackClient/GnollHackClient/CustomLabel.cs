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
        Immortal,
        Endor,
        Xizor,
        Underwood,
        DejaVuSansMono,
        DejaVuSansMonoBold,
        LatoRegular,
        LatoBold,
        ARChristy
    }

    public struct TextAreaSize
    {
        public float Width;
        public float Height;
        private List<float> _rowWidths;
        public TextAreaSize(float width, float height, List<float> rowWidths)
        {
            Width = width;
            Height = height;
            _rowWidths = rowWidths;
        }

        public float GetRowWidth(int rowidx)
        {
            if (_rowWidths == null || rowidx < 0 || rowidx >= _rowWidths.Count)
                return Width;
            return _rowWidths[rowidx];
        }
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
            set { SetValue(UseSpecialSymbolsProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty WordWrapSeparatorProperty = BindableProperty.Create(
            "WordWrapSeparator", typeof(char), typeof(CustomLabel), ' ');

        public char WordWrapSeparator
        {
            get { return (char)GetValue(WordWrapSeparatorProperty); }
            set { SetValue(WordWrapSeparatorProperty, value); UpdateLabel(); }
        }


        private void UpdateLabel()
        {
            InvalidateSurface();
            InvalidateMeasure();
        }

        float CalculateTextPartWidth(string textPart, SKPaint textPaint)
        {
            SKBitmap symbolbitmap;
            if (UseSpecialSymbols && (symbolbitmap = App.GetSpecialSymbol(textPart)) != null)
            {
                float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                float bmpwidth = bmpheight * (float)symbolbitmap.Width / (float)Math.Max(1, symbolbitmap.Height);
                float bmpmargin = bmpheight / 8;
                return bmpwidth + bmpmargin;
            }
            else
            {
                float width = textPaint.MeasureText(textPart);
                return width;
            }
        }

        public string[] SplitTextWithConstraint(string text, char separator, float widthConstraint, SKPaint textPaint)
        {
            List<string> result = new List<string>();
            int rowstartidx = 0;
            int wordstartidx = 0;
            float totalWidth = 0;
            string separatorstr = separator.ToString();
            float separatorwidth = textPaint.MeasureText(separatorstr);
            float width = 0;
            string word;

            for (int i = 0; i < text.Length; i++)
            {
                char c = text[i];
                if(c == separator || c == '\n')
                {
                    word = text.Substring(wordstartidx, i - wordstartidx);
                    width = CalculateTextPartWidth(word, textPaint);
                    if (width + totalWidth > widthConstraint && wordstartidx > rowstartidx)
                    {
                        string row = text.Substring(rowstartidx, wordstartidx - 1 - rowstartidx);
                        result.Add(row);
                        rowstartidx = wordstartidx;
                        while (rowstartidx < text.Length - 1 && text[rowstartidx] == ' ')
                            rowstartidx++;
                        totalWidth = 0;
                    }
                    totalWidth += width + separatorwidth;
                    if (c == '\n')
                    {
                        string row = text.Substring(rowstartidx, i - rowstartidx);
                        result.Add(row);
                        rowstartidx = i + 1;
                        while (rowstartidx < text.Length - 1 && text[rowstartidx] == ' ')
                            rowstartidx++;
                        totalWidth = 0;
                    }
                    wordstartidx = i + 1;
                }
                else
                {
                    if (i == text.Length - 1)
                    {
                        word = text.Substring(wordstartidx, i - wordstartidx + 1);
                        width = CalculateTextPartWidth(word, textPaint);
                        if (width + totalWidth > widthConstraint && wordstartidx > rowstartidx)
                        {
                            result.Add(text.Substring(rowstartidx, wordstartidx - 1 - rowstartidx));
                            rowstartidx = wordstartidx;
                            while (rowstartidx < text.Length - 1 && text[rowstartidx] == ' ')
                                rowstartidx++;
                        }
                        result.Add(text.Substring(rowstartidx));
                    }
                }
            }
            return result.ToArray();
        }

        private TextAreaSize CalculateTextAreaSize(float widthConstraint)
        {
            float longestwidth = 0;
            float totalheight = 0;
            List<float> rowWidths = new List<float>();
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
                string[] textRows = SplitTextWithConstraint(Text, WordWrapSeparator, widthConstraint, textPaint);
                foreach (string textRow in textRows)
                {
                    totalheight += textPaint.FontSpacing;
                    string[] textParts;
                    if (UseSpecialSymbols)
                    {
                        textParts = textRow.Split(' ');
                    }
                    else
                    {
                        textParts = new string[1] { textRow };
                    }
                    int cnt = 0;
                    float totalwidth = 0;
                    foreach (string textPart in textParts)
                    {
                        SKBitmap symbolbitmap;
                        if (UseSpecialSymbols && (symbolbitmap = App.GetSpecialSymbol(textPart)) != null)
                        {
                            float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                            float bmpwidth = bmpheight * (float)symbolbitmap.Width / (float)Math.Max(1, symbolbitmap.Height);
                            float bmpmargin = bmpheight / 8;
                            totalwidth += bmpwidth + bmpmargin;
                        }
                        else
                        {
                            string measuredString = cnt == textParts.Length - 1 ? textPart : textPart + " ";
                            float width = textPaint.MeasureText(measuredString, ref bounds);
                            totalwidth += width;
                        }
                        cnt++;
                    }
                    rowWidths.Add(totalwidth);
                    if (totalwidth > longestwidth)
                        longestwidth = totalwidth;
                }
                return new TextAreaSize(longestwidth, totalheight, rowWidths);
            }
        }

        private SizeRequest CalculateLabelSize(double widthConstraint, double heightConstraint)
        {
            double usedMaximumValue = 1000000.0;
            double adjWidthConstraint = double.IsInfinity(widthConstraint) ?
               usedMaximumValue : widthConstraint;
            double adjHeightConstraint = double.IsInfinity(heightConstraint) ?
               usedMaximumValue : heightConstraint;

            double wr = 0, hr = 0;
            if (WidthRequest > 0 && HeightRequest > 0)
            {
                wr = Math.Min(adjWidthConstraint, WidthRequest);
                hr = Math.Min(adjHeightConstraint, HeightRequest);
            }
            //else if(HorizontalOptions.Alignment == LayoutAlignment.Fill && VerticalOptions.Alignment == LayoutAlignment.Fill)
            //{
            //    wr = Math.Min(widthConstraint, App.DisplayWidth);
            //    hr = Math.Min(adjHeightConstraint, App.DisplayHeight);
            //}
            else
            {
                float scale = App.DisplayScale;
                float scaledwidthconstraint = scale * (float)(WidthRequest > 0 ? Math.Min(adjWidthConstraint, WidthRequest) : adjWidthConstraint);

                TextAreaSize textAreaSize = CalculateTextAreaSize(scaledwidthconstraint);
                if (WidthRequest > 0)
                    wr = Math.Min(adjWidthConstraint, WidthRequest);
                //else if (HorizontalOptions.Alignment == LayoutAlignment.Fill)
                //    wr = Math.Min(adjWidthConstraint, App.DisplayWidth);
                else
                    wr = Math.Min(adjWidthConstraint, (double)(textAreaSize.Width / scale));

                if (HeightRequest > 0)
                    hr = HeightRequest; // Math.Min(adjHeightConstraint, HeightRequest);
                //else if (VerticalOptions.Alignment == LayoutAlignment.Fill)
                //    hr = Math.Min(adjHeightConstraint, App.DisplayHeight);
                else
                    hr = (double)(textAreaSize.Height / scale); // Math.Min(adjHeightConstraint, (double)(textAreaSize.Height / scale));
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
                case CustomLabelFonts.Endor:
                    tf = App.EndorTypeface;
                    break;
                case CustomLabelFonts.Xizor:
                    tf = App.XizorTypeface;
                    break;
                case CustomLabelFonts.Underwood:
                    tf = App.UnderwoodTypeface;
                    break;
                case CustomLabelFonts.DejaVuSansMono:
                    tf = App.DejaVuSansMonoTypeface;
                    break;
                case CustomLabelFonts.DejaVuSansMonoBold:
                    tf = App.DejaVuSansMonoBoldTypeface;
                    break;
                case CustomLabelFonts.LatoRegular:
                    tf = App.LatoRegular;
                    break;
                case CustomLabelFonts.LatoBold:
                    tf = App.LatoBold;
                    break;
                case CustomLabelFonts.ARChristy:
                    tf = App.ARChristyTypeface;
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
            float scale = App.DisplayScale;
            float scale2 = this.Width == 0 ? 1.0f : canvaswidth / (float)this.Width;

            canvas.Clear();

            TextAreaSize textAreaSize = CalculateTextAreaSize((float)Width * scale);

            using (SKPaint textPaint = new SKPaint())
            {
                float x = 0, y = 0;
                textPaint.Typeface = GetFontTypeface();
                textPaint.TextSize = (float)FontSize * scale;
                //float textwidth = textPaint.MeasureText(Text);
                //float maxtextwidth = 0.98f * canvaswidth;
                //if (textwidth > maxtextwidth)
                //    textPaint.TextSize = textPaint.TextSize * maxtextwidth / textwidth;

                //string[] textRows = Text.Split('\n');
                string[] textRows = SplitTextWithConstraint(Text, WordWrapSeparator, (float)Width * scale, textPaint);

                switch (VerticalTextAlignment)
                {
                    case TextAlignment.Start:
                        y = 0;
                        break;
                    default:
                    case TextAlignment.Center:
                        y = (canvasheight - textPaint.FontSpacing * textRows.Length) / 2;
                        break;
                    case TextAlignment.End:
                        y = canvasheight - textPaint.FontSpacing * textRows.Length;
                        break;
                }
                y += -textPaint.FontMetrics.Ascent;

                int rowidx = 0;
                textPaint.TextAlign = SKTextAlign.Left;
                foreach (string textRow in textRows)
                {
                    string[] textParts = textRow.Split(' ');
                    float textWidth = textAreaSize.GetRowWidth(rowidx);

                    switch (HorizontalTextAlignment)
                    {
                        default:
                        case TextAlignment.Start:
                            x = 0;
                            break;
                        case TextAlignment.Center:
                            x = canvaswidth / 2 - textWidth / 2;
                            break;
                        case TextAlignment.End:
                            x = canvaswidth - textWidth;
                            break;
                    }

                    int cnt = 0;
                    foreach(string textPart in textParts)
                    {
                        SKBitmap symbolbitmap;
                        if (UseSpecialSymbols && (symbolbitmap = App.GetSpecialSymbol(textPart)) != null)
                        {
                            float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                            float bmpwidth = bmpheight * (float)symbolbitmap.Width / (float)Math.Max(1, symbolbitmap.Height);
                            float bmpmargin = bmpheight / 8;

                            float bmpx = x;
                            float bmpy = y + textPaint.FontMetrics.Ascent;
                            SKRect bmptargetrect = new SKRect(bmpx, bmpy, bmpx + bmpwidth, bmpy + bmpheight);
                            canvas.DrawBitmap(symbolbitmap, bmptargetrect, textPaint);
                            x += bmpwidth + bmpmargin;
                        }
                        else
                        {
                            string printedString = cnt == textParts.Length - 1 ? textPart : textPart + " ";
                            if (OutlineWidth > 0)
                            {
                                textPaint.Style = SKPaintStyle.Stroke;
                                textPaint.StrokeWidth = (float)OutlineWidth * scale;
                                SKColor outlinecolor = new SKColor((byte)(255 * OutlineColor.R), (byte)(255 * OutlineColor.G), (byte)(255 * OutlineColor.B), (byte)(255 * OutlineColor.A));
                                textPaint.Color = outlinecolor;
                                canvas.DrawText(printedString, x, y, textPaint);
                            }

                            textPaint.Style = SKPaintStyle.Fill;
                            SKColor fillcolor = new SKColor((byte)(255 * TextColor.R), (byte)(255 * TextColor.G), (byte)(255 * TextColor.B), (byte)(255 * TextColor.A));
                            textPaint.Color = fillcolor;
                            canvas.DrawText(printedString, x, y, textPaint);
                            x += textPaint.MeasureText(printedString);
                        }
                        cnt++;
                    }
                    rowidx++;
                    y += textPaint.FontSpacing;
                }
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

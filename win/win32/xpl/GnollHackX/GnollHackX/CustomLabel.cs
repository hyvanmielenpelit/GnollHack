using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.Concurrent;

#if GNH_MAUI
using GnollHackX;
using SkiaSharp.Views.Maui;
using SkiaSharp.Views.Maui.Controls;
using SkiaSharp.Views.Maui.Handlers;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Essentials;
using Xamarin.Forms;

namespace GnollHackX
#endif
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
#pragma warning disable 67
        public event EventHandler<GHMouseWheelEventArgs> MouseWheel;
#pragma warning restore 67

        public CustomLabel() : base()
        {
            PaintSurface += Base_PaintSurface;
            Touch += Base_Touch;
            MouseWheel += Base_MouseWheel;
        }

        public static readonly BindableProperty TextProperty = BindableProperty.Create(
            "Text", typeof(string), typeof(CustomLabel), "");

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); SplitRows(); UpdateLabel(); UpdateRolledDown(); }
        }

        public static readonly BindableProperty FontFamilyProperty = BindableProperty.Create(
            "FontFamily", typeof(string), typeof(CustomLabel), "Diablo");

        public string FontFamily
        {
            get { return (string)GetValue(FontFamilyProperty); }
            set { SetValue(FontFamilyProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty FontAttributesProperty = BindableProperty.Create(
            "FontAttributes", typeof(FontAttributes), typeof(CustomLabel), FontAttributes.None);

        public FontAttributes FontAttributes
        {
            get { return (FontAttributes)GetValue(FontAttributesProperty); }
            set { SetValue(FontAttributesProperty, value); UpdateLabel(); }
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
            "TextColor", typeof(Color), typeof(CustomLabel), GHColors.White);

        public Color TextColor
        {
            get { return (Color)GetValue(TextColorProperty); }
            set { SetValue(TextColorProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty OutlineColorProperty = BindableProperty.Create(
            "OutlineColor", typeof(Color), typeof(CustomLabel), GHColors.Black);

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

        public static readonly BindableProperty IsScrollableProperty = BindableProperty.Create(
            "IsScrollable", typeof(bool), typeof(CustomLabel), false);

        public bool IsScrollable
        {
            get { return (bool)GetValue(IsScrollableProperty); }
            set { UpdateScrollable(value, value && !IsScrollable); SetValue(IsScrollableProperty, value); UpdateRolledDown(); }
        }

        public static readonly BindableProperty InitiallyRolledDownProperty = BindableProperty.Create(
            "InitiallyRolledDown", typeof(bool), typeof(CustomLabel), false);

        public bool InitiallyRolledDown
        {
            get { return (bool)GetValue(InitiallyRolledDownProperty); }
            set { SetValue(InitiallyRolledDownProperty, value); UpdateRolledDown(); }
        }

        public static readonly BindableProperty WordWrapSeparatorProperty = BindableProperty.Create(
            "WordWrapSeparator", typeof(char), typeof(CustomLabel), ' ');

        public char WordWrapSeparator
        {
            get { return (char)GetValue(WordWrapSeparatorProperty); }
            set { SetValue(WordWrapSeparatorProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty DisplayWrapSeparatorProperty = BindableProperty.Create(
            "DisplayWrapSeparatorSymbols", typeof(bool), typeof(CustomLabel), true);

        public bool DisplayWrapSeparator
        {
            get { return (bool)GetValue(DisplayWrapSeparatorProperty); }
            set { SetValue(DisplayWrapSeparatorProperty, value); UpdateLabel(); }
        }

        public static readonly BindableProperty GeneralAnimationCounterProperty = BindableProperty.Create(
            "GeneralAnimationCounter", typeof(long), typeof(CustomLabel), 0L);

        public long GeneralAnimationCounter
        {
            get { return (long)GetValue(GeneralAnimationCounterProperty); }
            set { SetValue(GeneralAnimationCounterProperty, value); UpdateLabelScroll(); }
        }



        private object _textRowLock = new object();
        private string[] _textRows = null;
        private string[] TextRows
        {
            get { lock (_textRowLock) { return _textRows == null ? new string[1] {""} : _textRows; } }
        }

        private void SplitRows()
        {
            using (GHSkiaFontPaint textPaint = new GHSkiaFontPaint())
            {
                float scale = GHApp.DisplayDensity;
                textPaint.TextSize = (float)FontSize * scale;
                textPaint.Typeface = GetFontTypeface();
                lock (_textRowLock)
                {
                    if (Text != null)
                        _textRows = SplitTextWithConstraint(Text.Replace("\r", ""), (float)Width * scale, textPaint);
                    else
                        _textRows = null;
                }
            }
        }

        private void UpdateLabel()
        {
            InvalidateSurface();
            InvalidateMeasure();
        }

        private readonly object _initialYPosLock = new object();
        private bool _calculateInitialYPos = false;
        public bool CalculateInitialYPos { get { lock (_initialYPosLock) { return _calculateInitialYPos; } } set { lock (_initialYPosLock) { _calculateInitialYPos = value; } } }

        private void UpdateRolledDown()
        {
            lock (_textScrollLock)
            {
                _textScrollSpeed = 0;
                _textScrollSpeedRecordOn = false;
                _textScrollSpeedRecords.Clear();
                _textScrollOffset = 0;
            }
            if (InitiallyRolledDown)
                CalculateInitialYPos = true;
            InvalidateSurface();
        }

        private void UpdateScrollable(bool newval, bool updaterows)
        {
            EnableTouchEvents = newval;
            if (updaterows)
            {
                IsFirst = true;
                if(IsVisible)
                    InvalidateSurface();
            }
        }

        float CalculateTextPartWidth(string textPart, GHSkiaFontPaint textPaint)
        {
            if(textPart == null || textPart == "")
                return 0;

            SKImage symbolbitmap;
            SKRect source_rect = new SKRect();
            if (UseSpecialSymbols && (symbolbitmap = GHApp.GetSpecialSymbol(textPart, out source_rect)) != null)
            {
                float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                float bmpwidth = bmpheight * source_rect.Width / Math.Max(1f, source_rect.Height);
                float bmpmargin = bmpheight / 8;
                return bmpwidth + bmpmargin;
            }
            else
            {
                float width = textPaint.MeasureText(textPart);
                return width;
            }
        }

        public string[] SplitTextWithConstraint(string text, float widthConstraint, GHSkiaFontPaint textPaint)
        {
            if (text == null)
                return null;

            char separator = WordWrapSeparator;
            bool displayseparator = DisplayWrapSeparator;
            List<string> result = new List<string>();
            int wordstartidx = 0;
            float totalWidth = 0;
            string separatorstr = separator.ToString();
            float separatorwidth = textPaint.MeasureText(separatorstr);
            float width = 0;
            string word;
            string ending = displayseparator && separator != ' ' ? separator.ToString() : "";
            float ending_width = ending != "" ? textPaint.MeasureText(ending) : 0;
            string currentrowstr = "";

            for (int i = 0; i < text.Length; i++)
            {
                if (i  < wordstartidx)
                    continue;

                char c = text[i];
                if(c == separator || c == '\n')
                {
                    word = text.Substring(wordstartidx, i - wordstartidx);
                    width = CalculateTextPartWidth(word, textPaint);
                    if (width + (c == '\n' ? 0 : ending_width) + totalWidth > widthConstraint && currentrowstr != "")
                    {
                        string row = currentrowstr;
                        result.Add(row);
                        totalWidth = 0;
                        currentrowstr = "";
                    }

                    //Clear out separators and spaces from the start of the row with automatic word wrap
                    if (currentrowstr == "")
                    {
                        int startIdx = 0;
                        while (startIdx < word.Length && (word[startIdx] == ' ' || word[startIdx] == separator))
                            startIdx++;

                        word = word.Substring(startIdx);
                        width = CalculateTextPartWidth(word, textPaint);
                    }

                    currentrowstr += word;
                    totalWidth += width;
                    if(displayseparator && c == separator)
                    {
                        currentrowstr += separator.ToString();
                        totalWidth += separatorwidth;
                    }
                    if (c == '\n' || i == text.Length - 1)
                    {
                        string row = currentrowstr;
                        result.Add(row);
                        totalWidth = 0;
                        currentrowstr = "";
                    }
                    wordstartidx = i + 1;
                }
                else
                {
                    if (i == text.Length - 1)
                    {
                        string row;
                        word = text.Substring(wordstartidx, i - wordstartidx + 1);
                        width = CalculateTextPartWidth(word, textPaint);
                        if (width + totalWidth > widthConstraint && currentrowstr != "")
                        {
                            row = currentrowstr;
                            result.Add(row);
                            totalWidth = 0;
                            currentrowstr = "";
                        }
                        //Clear out separators and spaces from the start of the row with automatic word wrap
                        if (currentrowstr == "")
                        {
                            int startIdx = 0;
                            while (startIdx < word.Length && (word[startIdx] == ' ' || word[startIdx] == separator))
                                startIdx++;

                            word = word.Substring(startIdx);
                            //width = CalculateTextPartWidth(word, textPaint);
                        }
                        row = currentrowstr + word;
                        result.Add(row);
                        totalWidth = 0;
                        currentrowstr = "";
                    }
                }
            }
            return result.ToArray();
        }

        private TextAreaSize CalculateTextAreaSize(float widthConstraint)
        {
            if (Text == null)
                return new TextAreaSize(0, 0, new List<float>());

            float longestwidth = 0;
            float totalheight = 0;
            List<float> rowWidths = new List<float>();
            using (GHSkiaFontPaint textPaint = new GHSkiaFontPaint())
            {
                float scale = GHApp.DisplayDensity;
                textPaint.TextSize = (float)FontSize * scale;
                textPaint.Typeface = GetFontTypeface();
                if (OutlineWidth > 0)
                {
                    textPaint.Style = SKPaintStyle.Stroke;
                    textPaint.StrokeWidth = (float)OutlineWidth * scale;
                }
                string[] textRows = SplitTextWithConstraint(Text, widthConstraint, textPaint);
                for (int i = 0, n = textRows.Length; i < n; i++)
                {
                    string textRow = textRows[i];
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
                    for (int j = 0, m = textParts.Length; j < m; j++)
                    {
                        string textPart = textParts[j];
                        SKImage symbolbitmap;
                        SKRect source_rect = new SKRect();
                        if (UseSpecialSymbols && (symbolbitmap = GHApp.GetSpecialSymbol(textPart, out source_rect)) != null)
                        {
                            float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                            float bmpwidth = bmpheight * source_rect.Width / Math.Max(1f, source_rect.Height);
                            float bmpmargin = bmpheight / 8;
                            totalwidth += bmpwidth + bmpmargin;
                        }
                        else
                        {
                            string measuredString = cnt == textParts.Length - 1 ? textPart : textPart + " ";
                            float width = textPaint.MeasureText(measuredString);
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

        private Size CalculateLabelSize(double widthConstraint, double heightConstraint)
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
            else
            {
                float scale = GHApp.DisplayDensity;
                float scaledwidthconstraint = scale * (float)(WidthRequest > 0 ? Math.Min(adjWidthConstraint, WidthRequest) : adjWidthConstraint);

                TextAreaSize textAreaSize = CalculateTextAreaSize(scaledwidthconstraint);
                if (WidthRequest > 0)
                    wr = Math.Min(adjWidthConstraint, WidthRequest);
                else
                    wr = Math.Min(adjWidthConstraint, (double)(textAreaSize.Width / scale));

                if (HeightRequest > 0)
                    hr = HeightRequest; // Math.Min(adjHeightConstraint, HeightRequest);
                else
                    hr = (double)(textAreaSize.Height / scale); // Math.Min(adjHeightConstraint, (double)(textAreaSize.Height / scale));
            }
            return new Size(wr, hr);
        }

        Dictionary<string, CustomLabelFonts> _fontDictionary = new Dictionary<string, CustomLabelFonts>
        {
            {"Diablo", CustomLabelFonts.Diablo },
            {"Immortal", CustomLabelFonts.Immortal },
            {"Endor", CustomLabelFonts.Endor },
            {"Xizor", CustomLabelFonts.Xizor },
            {"Underwood", CustomLabelFonts.Underwood },
            {"DejaVuSansMono", CustomLabelFonts.DejaVuSansMono },
            {"DejaVuSansMonoBold", CustomLabelFonts.DejaVuSansMonoBold },
            {"LatoRegular", CustomLabelFonts.LatoRegular },
            {"LatoBold", CustomLabelFonts.LatoBold },
            {"ARChristy", CustomLabelFonts.ARChristy },
        };

        private SKTypeface GetFontTypeface()
        {
            SKTypeface tf = GHApp.DiabloTypeface;
            CustomLabelFonts clf;
            bool success = _fontDictionary.TryGetValue(FontFamily, out clf);
            if (!success)
                clf = CustomLabelFonts.Diablo;

            switch (clf)
            {
                default:
                case CustomLabelFonts.Diablo:
                    tf = GHApp.DiabloTypeface;
                    break;
                case CustomLabelFonts.Immortal:
                    tf = GHApp.ImmortalTypeface;
                    break;
                case CustomLabelFonts.Endor:
                    tf = GHApp.EndorTypeface;
                    break;
                case CustomLabelFonts.Xizor:
                    tf = GHApp.XizorTypeface;
                    break;
                case CustomLabelFonts.Underwood:
                    tf = GHApp.UnderwoodTypeface;
                    break;
                case CustomLabelFonts.DejaVuSansMono:
                    tf = FontAttributes == FontAttributes.Bold ? GHApp.DejaVuSansMonoBoldTypeface : GHApp.DejaVuSansMonoTypeface;
                    break;
                case CustomLabelFonts.DejaVuSansMonoBold:
                    tf = GHApp.DejaVuSansMonoBoldTypeface;
                    break;
                case CustomLabelFonts.LatoRegular:
                    tf = FontAttributes == FontAttributes.Bold ? GHApp.LatoBold : GHApp.LatoRegular;
                    break;
                case CustomLabelFonts.LatoBold:
                    tf = GHApp.LatoBold;
                    break;
                case CustomLabelFonts.ARChristy:
                    tf = GHApp.ARChristyTypeface;
                    break;
            }
            return tf;
        }

        private object _textAreaSizeLock = new object();
        private TextAreaSize _textAreaSize;
        private TextAreaSize TextAreaSize { get { lock (_textAreaSizeLock) { return _textAreaSize; } } set { lock (_textAreaSizeLock) { _textAreaSize = value; } } }

        private void Base_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;
            float canvaswidth = this.CanvasSize.Width;
            float canvasheight = this.CanvasSize.Height;
            float scale = GHApp.DisplayDensity;
            float scale2 = this.Width == 0 ? 1.0f : canvaswidth / (float)this.Width;

            canvas.Clear();

            lock (_isFirstLock)
            {
                TextAreaSize textAreaSize;
                if (!(IsScrollable && TouchMoved) || _isFirst)
                    TextAreaSize = CalculateTextAreaSize((float)Width * scale);

                textAreaSize = TextAreaSize;

                using (GHSkiaFontPaint textPaint = new GHSkiaFontPaint())
                {
#if !GNH_MAUI
                    SKFilterQuality oldFilterQuality = textPaint.Paint.FilterQuality;
#endif
                    float x = 0, y = 0;
                    textPaint.Typeface = GetFontTypeface();
                    textPaint.TextSize = (float)FontSize * scale;
                    if (!(IsScrollable && TouchMoved) || _isFirst)
                        SplitRows();
                    string[] textRows = TextRows;
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

                    float usedTextOffset = 0;
                    if (CalculateInitialYPos)
                    {
                        CalculateInitialYPos = false;
                        float textHeight = textRows.Length * textPaint.FontSpacing;
                        float bottomScrollLimit = Math.Min(0, CanvasSize.Height - textHeight);
                        lock (_textScrollLock)
                        {
                            usedTextOffset = _textScrollOffset = bottomScrollLimit;
                        }
                    }
                    else
                    {
                        lock (_textScrollLock)
                        {
                            usedTextOffset = _textScrollOffset;
                        }
                    }

                    y += usedTextOffset;

                    float generalpadding = (textPaint.FontSpacing - (textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent)) / 2;
                    //textPaint.TextAlign = SKTextAlign.Left;
                    for (int i = 0; i < textRows.Length; i++)
                    {
                        string textRow = textRows[i];
                        y += generalpadding;
                        y += -textPaint.FontMetrics.Ascent;

                        if (y + textPaint.FontMetrics.Ascent >= canvasheight || y + textPaint.FontMetrics.Descent < 0)
                        {
                            // Nothing, just calculate the height
                        }
                        else
                        {
                            string[] textParts = textRow.Split(' ');
                            float textWidth = textAreaSize.GetRowWidth(i);
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
                            for (int j = 0, m = textParts.Length; j < m; j++)
                            {
                                string textPart = textParts[j];
                                SKImage symbolbitmap;
                                SKRect source_rect = new SKRect();
                                if (UseSpecialSymbols && (symbolbitmap = GHApp.GetSpecialSymbol(textPart, out source_rect)) != null)
                                {
                                    float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                                    float bmpwidth = bmpheight * source_rect.Width / Math.Max(1f, source_rect.Height);
                                    float bmpmargin = bmpheight / 8;

                                    float bmpx = x;
                                    float bmpy = y + textPaint.FontMetrics.Ascent;
                                    SKRect bmptargetrect = new SKRect(bmpx, bmpy, bmpx + bmpwidth, bmpy + bmpheight);
#if !GNH_MAUI
                                    textPaint.Paint.FilterQuality = SKFilterQuality.High;
#endif
                                    canvas.DrawImage(symbolbitmap, source_rect, bmptargetrect,
#if GNH_MAUI
                                        new SKSamplingOptions(SKFilterMode.Linear));
#else
                                        textPaint.Paint);
                                    textPaint.Paint.FilterQuality = oldFilterQuality;
#endif
                                    x += bmpwidth + bmpmargin;
                                }
                                else
                                {
                                    string printedString = cnt == textParts.Length - 1 ? textPart : textPart + " ";
                                    if (OutlineWidth > 0)
                                    {
                                        textPaint.Style = SKPaintStyle.Stroke;
                                        textPaint.StrokeWidth = (float)OutlineWidth * scale;
#if GNH_MAUI
                                        SKColor outlinecolor = new SKColor((byte)(255 * OutlineColor.Red), (byte)(255 * OutlineColor.Green), (byte)(255 * OutlineColor.Blue), (byte)(255 * OutlineColor.Alpha));
#else
                                        SKColor outlinecolor = new SKColor((byte)(255 * OutlineColor.R), (byte)(255 * OutlineColor.G), (byte)(255 * OutlineColor.B), (byte)(255 * OutlineColor.A));
#endif
                                        textPaint.Color = outlinecolor;
                                        //canvas.DrawText(printedString, x, y, textPaint);
                                        textPaint.DrawTextOnCanvas(canvas, printedString, x, y, SKTextAlign.Left);
                                    }

                                    textPaint.Style = SKPaintStyle.Fill;
#if GNH_MAUI
                                    SKColor fillcolor = new SKColor((byte)(255 * TextColor.Red), (byte)(255 * TextColor.Green), (byte)(255 * TextColor.Blue), (byte)(255 * TextColor.Alpha));
#else
                                    SKColor fillcolor = new SKColor((byte)(255 * TextColor.R), (byte)(255 * TextColor.G), (byte)(255 * TextColor.B), (byte)(255 * TextColor.A));
#endif
                                    textPaint.Color = fillcolor;
                                    //canvas.DrawText(printedString, x, y, textPaint);
                                    textPaint.DrawTextOnCanvas(canvas, printedString, x, y, SKTextAlign.Left);
                                    x += textPaint.MeasureText(printedString);
                                }
                                cnt++;
                            }
                        }
                        y += textPaint.FontMetrics.Descent;
                        y += generalpadding;
                    }
                    TextHeight = y - usedTextOffset;
                }
                _isFirst = false;
            }
        }


        public double MeasureWidth(string str)
        {
            float scale = GHApp.DisplayDensity;
            if (scale == 0)
                return 0;
            float skwidth = 0;
            using (GHSkiaFontPaint textPaint = new GHSkiaFontPaint())
            {
                textPaint.Typeface = GetFontTypeface();
                textPaint.TextSize = (float)FontSize * scale;
                if(OutlineWidth > 0)
                {
                    textPaint.StrokeWidth = (float)OutlineWidth * scale;
                    textPaint.Style = SKPaintStyle.Stroke;
                }
                skwidth = textPaint.MeasureText(str);
            }
            return (double)skwidth / scale;
        }

        private ConcurrentDictionary<long, TouchEntry> TouchDictionary = new ConcurrentDictionary<long, TouchEntry>();
        private readonly object _textScrollLock = new object();
        private float _textScrollOffset = 0;
        private float _textScrollSpeed = 0; /* pixels per second */
        private bool _textScrollSpeedRecordOn = false;
        private DateTime _textScrollSpeedStamp;
        List<TouchSpeedRecord> _textScrollSpeedRecords = new List<TouchSpeedRecord>();
        private uint _auxAnimationLength = GHConstants.AuxiliaryCanvasAnimationTime / UIUtils.GetAuxiliaryCanvasAnimationInterval();
        private bool _textScrollSpeedOn = false;
        private bool TextScrollSpeedOn 
        { 
            get 
            { 
                return _textScrollSpeedOn; 
            } 
            set
            {
                _textScrollSpeedOn = value;
                if(value)
                {
                    Animation commandAnimation = new Animation(v => GeneralAnimationCounter = (long)v, 1, _auxAnimationLength);
                    commandAnimation.Commit(this, "GeneralAnimationCounter", length: GHConstants.AuxiliaryCanvasAnimationTime,
                        rate: UIUtils.GetAuxiliaryCanvasAnimationInterval(), repeat: () => true);
                }
                else
                {
                    if (this.AnimationIsRunning("GeneralAnimationCounter"))
                        this.AbortAnimation("GeneralAnimationCounter");
                }
            }
        }

        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);
            if (!IsVisible)
                return;
            if (propertyName == nameof(GeneralAnimationCounter))
                UpdateLabelScroll();
            else if (propertyName == nameof(IsVisible))
                UpdateRolledDown();
        }

        private DateTime _textScrollSpeedReleaseStamp;

        private object _textHeightLock = new object();
        private float _textHeight = 0;
        private float TextHeight { get { lock(_textHeightLock) { return _textHeight; } } set { lock (_textHeightLock) { _textHeight = value; } } }
        private readonly object _touchMovedLock = new object();
        private bool _touchMoved = false;
        private bool TouchMoved { get { lock (_touchMovedLock) { return _touchMoved; } } set { lock (_touchMovedLock) { _touchMoved = value; } } }
        private DateTime _savedTimeStamp;

        private readonly object _isFirstLock = new object();
        private bool _isFirst = false;
        private bool IsFirst { get { lock (_isFirstLock) { return _isFirst; } } set { lock (_isFirstLock) { _isFirst = value; } } }

        private void Base_Touch(object sender, SKTouchEventArgs e)
        {
            float textHeight = TextHeight;
            float bottomScrollLimit = Math.Min(0, CanvasSize.Height - textHeight);
            switch (e?.ActionType)
            {
                case SKTouchAction.Entered:
                    break;
                case SKTouchAction.Pressed:
                    if (TouchDictionary.ContainsKey(e.Id))
                        TouchDictionary[e.Id] = new TouchEntry(e.Location, DateTime.Now);
                    else
                        TouchDictionary.TryAdd(e.Id, new TouchEntry(e.Location, DateTime.Now));

                    if (TouchDictionary.Count > 1)
                        TouchMoved = true;

                    TextScrollSpeedOn = false;
                    _textScrollSpeed = 0;

                    e.Handled = true;
                    break;
                case SKTouchAction.Moved:
                    {
                        TouchEntry entry;
                        bool res = TouchDictionary.TryGetValue(e.Id, out entry);
                        if (res)
                        {
                            SKPoint anchor = entry.Location;

                            float diffX = e.Location.X - anchor.X;
                            float diffY = e.Location.Y - anchor.Y;
                            float dist = (float)Math.Sqrt((Math.Pow(diffX, 2) + Math.Pow(diffY, 2)));

                            if (TouchDictionary.Count == 1)
                            {
                                if ((dist > GHConstants.MoveDistanceThreshold ||
                                    (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond > GHConstants.MoveOrPressTimeThreshold
                                       ))
                                {
                                    if (diffX != 0 || diffY != 0)
                                    {
                                        lock (_textScrollLock)
                                        {
                                            float oldoffset = _textScrollOffset;
                                            //_textScrollOffset += diffY;
                                            //if (_textScrollOffset < -(textHeight - CanvasSize.Height))
                                            //{
                                            //    _textScrollOffset = -(textHeight - CanvasSize.Height);
                                            //}
                                            //if (_textScrollOffset > 0)
                                            //{
                                            //    _textScrollOffset = 0;
                                            //}

                                            DateTime now = DateTime.Now;
                                            /* Do not scroll within button press time threshold, unless large move */
                                            long millisecs_elapsed = (now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond;

                                            lock (_textScrollLock)
                                            {
                                                float stretchLimit = GHConstants.ScrollStretchLimit * CanvasSize.Height;
                                                float stretchConstant = GHConstants.ScrollConstantStretch * CanvasSize.Height;
                                                float adj_factor = 1.0f;
                                                if (_textScrollOffset > 0)
                                                    adj_factor = _textScrollOffset >= stretchLimit ? 0 : (1 - ((_textScrollOffset + stretchConstant) / (stretchLimit + stretchConstant)));
                                                else if (_textScrollOffset < bottomScrollLimit)
                                                    adj_factor = _textScrollOffset < bottomScrollLimit - stretchLimit ? 0 : (1 - ((bottomScrollLimit - (_textScrollOffset - stretchConstant)) / (stretchLimit + stretchConstant)));

                                                float adj_diffY = diffY * adj_factor;
                                                _textScrollOffset += adj_diffY;

                                                if (_textScrollOffset > stretchLimit)
                                                    _textScrollOffset = stretchLimit;
                                                else if (_textScrollOffset < bottomScrollLimit - stretchLimit)
                                                    _textScrollOffset = bottomScrollLimit - stretchLimit;
                                                else
                                                {
                                                    /* Calculate duration since last touch move */
                                                    float duration = 0;
                                                    if (!_textScrollSpeedRecordOn)
                                                    {
                                                        duration = (float)millisecs_elapsed / 1000f;
                                                        _textScrollSpeedRecordOn = true;
                                                    }
                                                    else
                                                    {
                                                        duration = ((float)(now.Ticks - _textScrollSpeedStamp.Ticks) / TimeSpan.TicksPerMillisecond) / 1000f;
                                                    }
                                                    _textScrollSpeedStamp = now;

                                                    /* Discard speed records to the opposite direction */
                                                    if (_textScrollSpeedRecords.Count > 0)
                                                    {
                                                        int prevsgn = Math.Sign(_textScrollSpeedRecords[0].Distance);
                                                        if (diffY != 0 && prevsgn != 0 && Math.Sign(diffY) != prevsgn)
                                                            _textScrollSpeedRecords.Clear();
                                                    }

                                                    /* Add a new speed record */
                                                    _textScrollSpeedRecords.Insert(0, new TouchSpeedRecord(diffY, duration, now));

                                                    /* Discard too old records */
                                                    while (_textScrollSpeedRecords.Count > 0)
                                                    {
                                                        long lastrecord_ms = (now.Ticks - _textScrollSpeedRecords[_textScrollSpeedRecords.Count - 1].TimeStamp.Ticks) / TimeSpan.TicksPerMillisecond;
                                                        if (lastrecord_ms > GHConstants.ScrollRecordThreshold)
                                                            _textScrollSpeedRecords.RemoveAt(_textScrollSpeedRecords.Count - 1);
                                                        else
                                                            break;
                                                    }

                                                    /* Sum up the distances and durations of current records to get an average */
                                                    float totaldistance = 0;
                                                    float totalsecs = 0;
                                                    foreach (TouchSpeedRecord r in _textScrollSpeedRecords)
                                                    {
                                                        totaldistance += r.Distance;
                                                        totalsecs += r.Duration;
                                                    }
                                                    _textScrollSpeed = totaldistance / Math.Max(0.001f, totalsecs);
                                                    TextScrollSpeedOn = false;
                                                }
                                            }
                                            TouchDictionary[e.Id].Location = e.Location;
                                            _touchMoved = true;
                                            _savedTimeStamp = DateTime.Now;

                                            if (_textScrollOffset != oldoffset)
                                                InvalidateSurface();
                                        }
                                        TouchDictionary[e.Id].Location = e.Location;
                                        TouchMoved = true;
                                    }
                                }

                            }
                            else if (TouchDictionary.Count == 2)
                            {
                                TouchDictionary[e.Id].Location = e.Location;
                                TouchMoved = true;
                            }
                        }
                        e.Handled = true;
                    }
                    break;
                case SKTouchAction.Released:
                    {
                        TouchEntry entry;
                        bool res = TouchDictionary.TryGetValue(e.Id, out entry);
                        if (res)
                        {
                            long elapsedms = (DateTime.Now.Ticks - entry.PressTime.Ticks) / TimeSpan.TicksPerMillisecond;
                            if (elapsedms <= GHConstants.MoveOrPressTimeThreshold && !_touchMoved)
                            {
                                //Touching does not do anything currently
                            }
                        }

                        if (TouchDictionary.ContainsKey(e.Id))
                        {
                            TouchEntry removedEntry;
                            TouchDictionary.TryRemove(e.Id, out removedEntry);
                        }
                        else
                            TouchDictionary.Clear(); /* Something's wrong; reset the touch dictionary */

                        if (TouchDictionary.Count == 0)
                        {
                            lock (_textScrollLock)
                            {
                                long lastrecord_ms = 0;
                                if (_textScrollSpeedRecords.Count > 0)
                                {
                                    lastrecord_ms = (DateTime.Now.Ticks - _textScrollSpeedRecords[_textScrollSpeedRecords.Count - 1].TimeStamp.Ticks) / TimeSpan.TicksPerMillisecond;
                                }

                                if (_textScrollOffset > 0 || _textScrollOffset < bottomScrollLimit)
                                {
                                    if (lastrecord_ms > GHConstants.ScrollRecordThreshold
                                        || Math.Abs(_textScrollSpeed) < GHConstants.ScrollSpeedThreshold * CanvasSize.Height)
                                        _textScrollSpeed = 0;

                                    TextScrollSpeedOn = true;
                                    _textScrollSpeedReleaseStamp = DateTime.Now;
                                }
                                else if (lastrecord_ms > GHConstants.ScrollRecordThreshold)
                                {
                                    TextScrollSpeedOn = false;
                                    _textScrollSpeed = 0;
                                }
                                else if (Math.Abs(_textScrollSpeed) >= GHConstants.ScrollSpeedThreshold * CanvasSize.Height)
                                {
                                    TextScrollSpeedOn = true;
                                    _textScrollSpeedReleaseStamp = DateTime.Now;
                                }
                                else
                                {
                                    TextScrollSpeedOn = false;
                                    _textScrollSpeed = 0;
                                }
                                _textScrollSpeedRecordOn = false;
                                _textScrollSpeedRecords.Clear();
                            }
                            TouchMoved = false;
                        }
                        e.Handled = true;
                    }
                    break;
                case SKTouchAction.Cancelled:
                    if (TouchDictionary.ContainsKey(e.Id))
                    {
                        TouchEntry removedEntry;
                        TouchDictionary.TryRemove(e.Id, out removedEntry);
                    }
                    else
                        TouchDictionary.Clear(); /* Something's wrong; reset the touch dictionary */

                    lock (_textScrollLock)
                    {
                        if (_textScrollOffset > 0 || _textScrollOffset < bottomScrollLimit)
                        {
                            long lastrecord_ms = 0;
                            if (_textScrollSpeedRecords.Count > 0)
                            {
                                lastrecord_ms = (DateTime.Now.Ticks - _textScrollSpeedRecords[_textScrollSpeedRecords.Count - 1].TimeStamp.Ticks) / TimeSpan.TicksPerMillisecond;
                            }

                            if (lastrecord_ms > GHConstants.ScrollRecordThreshold
                                || Math.Abs(_textScrollSpeed) < GHConstants.ScrollSpeedThreshold * CanvasSize.Height)
                                _textScrollSpeed = 0;

                            TextScrollSpeedOn = true;
                            _textScrollSpeedReleaseStamp = DateTime.Now;
                        }
                    }
                    e.Handled = true;
                    break;
                case SKTouchAction.Exited:
                    break;
                case SKTouchAction.WheelChanged:
                    break;
                default:
                    break;
            }
        }

        private void UpdateLabelScroll()
        {
            InvalidateSurface();
            lock (_textScrollLock)
            {
                float speed = _textScrollSpeed; /* pixels per second */
                float bottomScrollLimit = 0;
                bottomScrollLimit = Math.Min(0, CanvasSize.Height - TextHeight);
                int sgn = Math.Sign(_textScrollSpeed);
                float delta = speed / UIUtils.GetAuxiliaryCanvasAnimationFrequency(); /* pixels */
                _textScrollOffset += delta;
                if (_textScrollOffset < 0 && _textScrollOffset - delta > 0)
                {
                    _textScrollOffset = 0;
                    _textScrollSpeed = 0;
                    TextScrollSpeedOn = false;
                }
                else if (_textScrollOffset > bottomScrollLimit && _textScrollOffset - delta < bottomScrollLimit)
                {
                    _textScrollOffset = bottomScrollLimit;
                    _textScrollSpeed = 0;
                    TextScrollSpeedOn = false;
                }
                else if (_textScrollOffset > 0 || _textScrollOffset < bottomScrollLimit)
                {
                    float deceleration1 = CanvasSize.Height * GHConstants.ScrollConstantDeceleration * GHConstants.ScrollConstantDecelerationOverEdgeMultiplier;
                    float deceleration2 = Math.Abs(_textScrollSpeed) * GHConstants.ScrollSpeedDeceleration * GHConstants.ScrollSpeedDecelerationOverEdgeMultiplier;
                    float deceleration_per_second = deceleration1 + deceleration2;
                    float distance_from_edge = _textScrollOffset > 0 ? _textScrollOffset : _textScrollOffset - bottomScrollLimit;
                    float deceleration3 = (distance_from_edge + (float)Math.Sign(distance_from_edge) * GHConstants.ScrollDistanceEdgeConstant * CanvasSize.Height) * GHConstants.ScrollOverEdgeDeceleration;
                    float distance_anchor_distance = CanvasSize.Height * GHConstants.ScrollDistanceAnchorFactor;
                    float close_anchor_distance = CanvasSize.Height * GHConstants.ScrollCloseAnchorFactor;
                    float target_speed_at_distance = GHConstants.ScrollTargetSpeedAtDistanceAnchor;
                    float target_speed_at_close = GHConstants.ScrollTargetSpeedAtCloseAnchor;
                    float target_speed_at_edge = GHConstants.ScrollTargetSpeedAtEdge;
                    float dist_factor = (Math.Abs(distance_from_edge) - close_anchor_distance) / (distance_anchor_distance - close_anchor_distance);
                    float close_factor = Math.Abs(distance_from_edge) / close_anchor_distance;
                    float target_speed = -1.0f * (float)Math.Sign(distance_from_edge)
                        * (
                        Math.Max(0f, dist_factor) * (target_speed_at_distance - target_speed_at_close)
                        + Math.Min(1f, close_factor) * (target_speed_at_close - target_speed_at_edge)
                        + target_speed_at_edge
                        )
                        * CanvasSize.Height;
                    if (_textScrollOffset > 0 ? _textScrollSpeed <= 0 : _textScrollSpeed >= 0)
                    {
                        float target_factor = Math.Abs(distance_from_edge) / distance_anchor_distance;
                        _textScrollSpeed += (-1.0f * deceleration3) * (float)UIUtils.GetAuxiliaryCanvasAnimationInterval() / 1000;
                        if (target_factor < 1.0f)
                        {
                            _textScrollSpeed = _textScrollSpeed * target_factor + target_speed * (1.0f - target_factor);
                        }
                    }
                    else
                        _textScrollSpeed += (-1.0f * (float)sgn * deceleration_per_second - deceleration3) * (float)UIUtils.GetAuxiliaryCanvasAnimationInterval() / 1000;
                }
                else
                {
                    //if (_textScrollSpeedReleaseStamp != null)
                    {
                        long millisecs_elapsed = (DateTime.Now.Ticks - _textScrollSpeedReleaseStamp.Ticks) / TimeSpan.TicksPerMillisecond;
                        if (millisecs_elapsed > GHConstants.FreeScrollingTime)
                        {
                            float deceleration1 = (float)CanvasSize.Height * GHConstants.ScrollConstantDeceleration;
                            float deceleration2 = Math.Abs(_textScrollSpeed) * GHConstants.ScrollSpeedDeceleration;
                            float deceleration_per_second = deceleration1 + deceleration2;
                            _textScrollSpeed += -1.0f * (float)sgn * ((deceleration_per_second * (float)UIUtils.GetAuxiliaryCanvasAnimationInterval()) / 1000);
                            if (sgn == 0 || (sgn > 0 && _textScrollSpeed < 0) || (sgn < 0 && _textScrollSpeed > 0))
                                _textScrollSpeed = 0;
                        }
                    }
                }
            }
        }

#if GNH_MAUI
        public Size CustomMeasureSize(double widthConstraint, double heightConstraint)
        {
            return CalculateLabelSize(widthConstraint, heightConstraint);
        }
#else
        protected override SizeRequest OnMeasure(double widthConstraint, double heightConstraint)
        {
            return new SizeRequest(CalculateLabelSize(widthConstraint, heightConstraint));
        }
#endif

        private double _currentWidth = 0;
        private double _currentHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentWidth || height != _currentHeight)
            {
                _currentWidth = width;
                _currentHeight = height;
                UpdateRolledDown();
                UpdateScrollable(IsScrollable, true);
            }
        }

        private void Base_MouseWheel(object sender, GHMouseWheelEventArgs e)
        {
            if (IsScrollable && e.MouseWheelDelta != 0)
            {
                lock (_textScrollLock)
                {
                    float bottomScrollLimit =  Math.Min(0, CanvasSize.Height - TextHeight);
                    _textScrollOffset += (CanvasSize.Height * e.MouseWheelDelta) / (10 * 120);
                    if (_textScrollOffset > 0)
                    {
                        _textScrollOffset = 0;
                        _textScrollSpeed = 0;
                        TextScrollSpeedOn = false;
                    }
                    else if (_textScrollOffset < bottomScrollLimit)
                    {
                        _textScrollOffset = bottomScrollLimit;
                        _textScrollSpeed = 0;
                        TextScrollSpeedOn = false;
                    }
                }
                InvalidateSurface();
            }
        }
#if GNH_MAUI
        protected override void OnHandlerChanged()
        {
            base.OnHandlerChanged();
#if WINDOWS
            SkiaSharp.Views.Windows.SKXamlCanvas view = Handler?.PlatformView as SkiaSharp.Views.Windows.SKXamlCanvas;
            if(view != null)
                view.PointerWheelChanged += View_PointerWheelChanged;
#endif
        }

#if WINDOWS
        private void View_PointerWheelChanged(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if (sender is Microsoft.UI.Xaml.UIElement)
            {
                var delta = e.GetCurrentPoint((Microsoft.UI.Xaml.UIElement)sender).Properties.MouseWheelDelta;
                if (delta != 0)
                {
                    MouseWheel?.Invoke(sender, new GHMouseWheelEventArgs(delta));
                }
            }
        }

#endif
#endif
    }

#if GNH_MAUI
    internal class AutoSizeSKCanvasViewHandler : SKCanvasViewHandler
    {
        public override Size GetDesiredSize(double widthConstraint, double heightConstraint)
        {
            var custom = (this.VirtualView as CustomLabel).CustomMeasureSize(widthConstraint, heightConstraint);
            if (custom == Size.Zero)
                return base.GetDesiredSize(widthConstraint, heightConstraint);
            else
                return custom;
        }
    }
#endif
}

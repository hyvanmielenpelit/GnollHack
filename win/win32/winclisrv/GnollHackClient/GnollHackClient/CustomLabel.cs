using GnollHackCommon;
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
            Touch += Base_Touch;
        }

        public static readonly BindableProperty TextProperty = BindableProperty.Create(
            "Text", typeof(string), typeof(CustomLabel), "");

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); SplitRows(); UpdateLabel(); }
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

        public static readonly BindableProperty IsScrollableProperty = BindableProperty.Create(
            "IsScrollable", typeof(bool), typeof(CustomLabel), false);

        public bool IsScrollable
        {
            get { return (bool)GetValue(IsScrollableProperty); }
            set { SetValue(IsScrollableProperty, value); UpdateScrollable(value); }
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

        private object _textRowLock = new object();
        private string[] _textRows = null;
        private string[] TextRows
        {
            get { lock (_textRowLock) { return _textRows == null ? new string[1] {""} : _textRows; } }
        }

        private void SplitRows()
        {
            using (SKPaint textPaint = new SKPaint())
            {
                float scale = App.DisplayScale;
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
            UpdateScrollable(IsScrollable);
        }

        private void UpdateScrollable(bool val)
        {
            this.EnableTouchEvents = val;
        }

        float CalculateTextPartWidth(string textPart, SKPaint textPaint)
        {
            if(textPart == null || textPart == "")
                return 0;

            SKBitmap symbolbitmap;
            SKRect source_rect = new SKRect();
            if (UseSpecialSymbols && (symbolbitmap = App.GetSpecialSymbol(textPart, out source_rect)) != null)
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

        public string[] SplitTextWithConstraint(string text, float widthConstraint, SKPaint textPaint)
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
                string[] textRows = SplitTextWithConstraint(Text, widthConstraint, textPaint);
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
                        SKRect source_rect = new SKRect();
                        if (UseSpecialSymbols && (symbolbitmap = App.GetSpecialSymbol(textPart, out source_rect)) != null)
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
            else
            {
                float scale = App.DisplayScale;
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
            return new SizeRequest(new Size(wr, hr));
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
            SKTypeface tf = App.DiabloTypeface;
            CustomLabelFonts clf;
            bool success = _fontDictionary.TryGetValue(FontFamily, out clf);
            if (!success)
                clf = CustomLabelFonts.Diablo;

            switch (clf)
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
                    tf = FontAttributes == FontAttributes.Bold ? App.DejaVuSansMonoBoldTypeface : App.DejaVuSansMonoTypeface;
                    break;
                case CustomLabelFonts.DejaVuSansMonoBold:
                    tf = App.DejaVuSansMonoBoldTypeface;
                    break;
                case CustomLabelFonts.LatoRegular:
                    tf = FontAttributes == FontAttributes.Bold ? App.LatoBold : App.LatoRegular;
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
            float scale = App.DisplayScale;
            float scale2 = this.Width == 0 ? 1.0f : canvaswidth / (float)this.Width;

            canvas.Clear();
            TextAreaSize textAreaSize = TextAreaSize;

            using (SKPaint textPaint = new SKPaint())
            {
                float x = 0, y = 0;
                textPaint.Typeface = GetFontTypeface();
                textPaint.TextSize = (float)FontSize * scale;
                string[] textRows = TextRows; // SplitTextWithConstraint(Text, (float)Width * scale, textPaint);
                float usedTextOffset = 0;
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

                lock(_textOffsetLock)
                {
                    y += _textOffsetY;
                    usedTextOffset = _textOffsetY;
                }
                float generalpadding = (textPaint.FontSpacing - (textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent)) / 2;
                textPaint.TextAlign = SKTextAlign.Left;
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
                        foreach (string textPart in textParts)
                        {
                            SKBitmap symbolbitmap;
                            SKRect source_rect = new SKRect();
                            if (UseSpecialSymbols && (symbolbitmap = App.GetSpecialSymbol(textPart, out source_rect)) != null)
                            {
                                float bmpheight = textPaint.FontMetrics.Descent / 2 - textPaint.FontMetrics.Ascent;
                                float bmpwidth = bmpheight * source_rect.Width / Math.Max(1f, source_rect.Height);
                                float bmpmargin = bmpheight / 8;

                                float bmpx = x;
                                float bmpy = y + textPaint.FontMetrics.Ascent;
                                SKRect bmptargetrect = new SKRect(bmpx, bmpy, bmpx + bmpwidth, bmpy + bmpheight);
                                canvas.DrawBitmap(symbolbitmap, source_rect, bmptargetrect, textPaint);
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
                    }
                    y += textPaint.FontMetrics.Descent;
                    y += generalpadding;
                }
                TextHeight = y - usedTextOffset;
            }
        }



        private Dictionary<long, TouchEntry> TouchDictionary = new Dictionary<long, TouchEntry>();
        private readonly object _textOffsetLock = new object();
        private float _textOffsetY = 0;
        private object _textHeightLock = new object();
        private float _textHeight = 0;
        private float TextHeight { get { lock(_textHeightLock) { return _textHeight; } } set { lock (_textHeightLock) { _textHeight = value; } } }
        private bool _touchMoved = false;
        private object _savedSender = null;
        private SKTouchEventArgs _savedEventArgs = null;

        private void Base_Touch(object sender, SKTouchEventArgs e)
        {
            float textHeight = TextHeight;
            switch (e?.ActionType)
            {
                case SKTouchAction.Entered:
                    break;
                case SKTouchAction.Pressed:
                    _savedSender = null;
                    _savedEventArgs = null;

                    if (TouchDictionary.ContainsKey(e.Id))
                        TouchDictionary[e.Id] = new TouchEntry(e.Location, DateTime.Now);
                    else
                        TouchDictionary.Add(e.Id, new TouchEntry(e.Location, DateTime.Now));

                    if (TouchDictionary.Count > 1)
                        _touchMoved = true;

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
                                        lock (_textOffsetLock)
                                        {
                                            float oldoffset = _textOffsetY;
                                            _textOffsetY += diffY;
                                            if (_textOffsetY < -(textHeight - CanvasSize.Height))
                                            {
                                                _textOffsetY = -(textHeight - CanvasSize.Height);
                                            }
                                            if (_textOffsetY > 0)
                                            {
                                                _textOffsetY = 0;
                                            }
                                            if (_textOffsetY != oldoffset)
                                                InvalidateSurface();
                                        }
                                        TouchDictionary[e.Id].Location = e.Location;
                                        _touchMoved = true;
                                    }
                                }

                            }
                            else if (TouchDictionary.Count == 2)
                            {
                                _savedSender = null;
                                _savedEventArgs = null;

                                TouchDictionary[e.Id].Location = e.Location;
                                _touchMoved = true;
                            }
                        }
                        e.Handled = true;
                    }
                    break;
                case SKTouchAction.Released:
                    {
                        _savedSender = null;
                        _savedEventArgs = null;

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
                            TouchDictionary.Remove(e.Id);
                        else
                            TouchDictionary.Clear(); /* Something's wrong; reset the touch dictionary */

                        if (TouchDictionary.Count == 0)
                            _touchMoved = false;

                        e.Handled = true;
                    }
                    break;
                case SKTouchAction.Cancelled:
                    if (TouchDictionary.ContainsKey(e.Id))
                        TouchDictionary.Remove(e.Id);
                    else
                        TouchDictionary.Clear(); /* Something's wrong; reset the touch dictionary */
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
                SplitRows();
                TextAreaSize = CalculateTextAreaSize((float)Width * App.DisplayScale);
                UpdateScrollable(IsScrollable);
                lock(_textOffsetLock)
                {
                    _textOffsetY = 0;
                }
                if (IsVisible)
                    InvalidateSurface();
                //if (IsVisible)
                //    UpdateLabel();
            }
        }

    }
}

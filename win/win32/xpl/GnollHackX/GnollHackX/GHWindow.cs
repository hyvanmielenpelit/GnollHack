#if GNH_MAUI
using GnollHackM;
using GnollHackX;
#else
using GnollHackX.Pages.Game;
#endif
using SkiaSharp;
using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace GnollHackX
{
    public enum GHWinType
    {
        None,
        Message,
        Status,
        Map,
        Menu,
        Text,
        Base,
        Here,
        Inventory,
        Reserved_1,
        Reserved_2,
        Reserved_3,
        RIP,
        Keypad,
        Overview,
        Worn
    }

    public sealed class GHPadding
    {
        public GHPadding(float left, float top, float right, float bottom)
        {
            Left = left;
            Top = top;
            Right = right;
            Bottom = bottom;
        }
        public float Left { get; set; }
        public float Top { get; set; }
        public float Right { get; set; }
        public float Bottom { get; set; }
    }

    public sealed class GHWindow : IDisposable
    {
        private GHWinType _winType = 0;
        private ghwindow_styles _winStyle = 0;
        private int _glyph;
        private ObjectDataItem _objdata = null;
        public ObjectDataItem ObjData { get { return _objdata; } set { _objdata = value; } }
        private GHGame _currentGame;
        private int _winId;
        private bool _useUpperSide;
        private bool _useSpecialSymbols;
        private bool _ascension;

        /* Cached GHSkiaFontPaint for MeasureText in PutStrEx/PutStrEx2 */
        private GHSkiaFontPaint _measurePaint = null;
        private SKTypeface _measureTypeface = null;
        private float _measureTextSize = 0;

        public SKTypeface Typeface { get; set; }
        public SKColor TextColor { get; set; }
        public float TextSize { get; set; }
        public float StrokeWidth { get; set; }
        public bool HasShadow { get; set; }
        public SKColor BackgroundColor { get; set; }
        public int CursX { get; set; }
        public int CursY { get; set; }
        public bool CenterHorizontally { get; set; }
        public GHWinType WindowType { get { return _winType; } }
        public ghwindow_styles WindowStyle { get { return _winStyle; } }
        public int Glyph { get { return _glyph; } }
        public bool UseUpperSide { get { return _useUpperSide; } }
        public bool UseSpecialSymbols { get { return _useSpecialSymbols; } }
        public bool Ascension { get { return _ascension; } }
        //public GamePage ActiveGamePage { get { return _gamePage; } }
        public bool AutoPlacement { get; set; }
        private bool ShouldPublishClone
        {
            get
            {
                switch (_winType)
                {
                case GHWinType.Status:
                    return _currentGame.ActiveGamePage?.ClassicStatusBar ?? true;
                case GHWinType.Text:
                case GHWinType.Menu:
                    return false;
                default:
                    return true;
                }
            }
        }
        public bool AutoCarriageReturn
        {
            get
            {
                bool res = (WindowPrintStyle == GHWindowPrintLocations.PrintToWindow);
                switch (_winType)
                {
                    case GHWinType.None:
                        break;
                    case GHWinType.Message:
                        break;
                    case GHWinType.Status:
                        break;
                    case GHWinType.Map:
                        break;
                    case GHWinType.Menu:
                        res = true;
                        break;
                    case GHWinType.Text:
                        res = true;
                        break;
                    case GHWinType.Base:
                        break;
                    case GHWinType.Here:
                        res = true;
                        break;
                    case GHWinType.Inventory:
                        break;
                    case GHWinType.Reserved_1:
                    case GHWinType.Reserved_2:
                    case GHWinType.Reserved_3:
                        break;
                    case GHWinType.RIP:
                        break;
                    case GHWinType.Keypad:
                        break;
                    case GHWinType.Overview:
                        break;
                    case GHWinType.Worn:
                        break;
                }

                return res;
            }
        }
        public int WindowID { get { return _winId; } }

        private List<GHWindowRow> _putStrs = new List<GHWindowRow>();
        public List<GHWindowRow> PutStrs { get { return _putStrs; } set { _putStrs = value; } }

        public bool Visible { get; set; }
        private int _width = 0;
        private int _height = 0;

        public GHPadding Padding = new GHPadding(10, 10, 10 , 10);

        public int WidthInChars { get { return _width; } }
        public int HeightInChars { get { return _height; } }

        public void SetWidthHeight(int width, int height, float pixelWidth, float pixelHeight)
        {
            _width = width;
            _height = height;
            _pixelWidth = pixelWidth;
            _pixelHeight = pixelHeight;
        }

        public GHWindowPrintLocations WindowPrintStyle 
        { get {
                GHWindowPrintLocations res = GHWindowPrintLocations.PrintToWindow;
                switch (_winType)
                {
                    case GHWinType.None:
                        break;
                    case GHWinType.Message:
                        res = GHWindowPrintLocations.RawPrint;
                        break;
                    case GHWinType.Status:
                        res = GHWindowPrintLocations.PrintToMap;
                        break;
                    case GHWinType.Map:
                        res = GHWindowPrintLocations.PrintToMap;
                        break;
                    case GHWinType.Menu:
                        break;
                    case GHWinType.Text:
                        break;
                    case GHWinType.Base:
                        break;
                    case GHWinType.Here:
                        res = GHWindowPrintLocations.PrintToMap;
                        break;
                    case GHWinType.Inventory:
                        break;
                    case GHWinType.Reserved_1:
                    case GHWinType.Reserved_2:
                    case GHWinType.Reserved_3:
                        break;
                    case GHWinType.RIP:
                        break;
                    case GHWinType.Keypad:
                        break;
                    case GHWinType.Overview:
                        break;
                    case GHWinType.Worn:
                        break;
                }

                return res;
            }
        }
        public float Left { get; set; }
        public float Top { get; set; }
        public float UnscaledWidth { get { return _pixelHeight; } }
        public float UnscaledHeight { get { return _pixelHeight; } }

        private float _pixelWidth = 0;
        private float _pixelHeight = 0;

        public GHMenuInfo MenuInfo { get; set; }

        private List<GHMenuItem> _selectedMenuItems = null;
        //private readonly object _selectedMenuItemsLock = new object();
        public List<GHMenuItem> SelectedMenuItems
        {
            //get { lock (_selectedMenuItemsLock) { return _selectedMenuItems; } }
            //set { lock (_selectedMenuItemsLock) { _selectedMenuItems = value; } }
            get { return Interlocked.CompareExchange(ref _selectedMenuItems, null, null); }
            set { Interlocked.Exchange(ref _selectedMenuItems, value); }
        }

        public bool WasCancelled { get; set; } = false;

        public GHWindow(GHWinType winType, ghwindow_styles winStyle, int glyph, bool useUpperSide, bool useSpecialSymbols, bool ascension, ObjectDataItem objdata, int winid)
        {
            _winType = winType;
            _winStyle = winStyle;
            _glyph = glyph;
            _useUpperSide = useUpperSide;
            _useSpecialSymbols = useSpecialSymbols;
            _ascension = ascension;
            _objdata = objdata;
            _currentGame = GHApp.CurrentGHGame;
            _winId = winid;
        }

        public GHPublishedWindow Publish()
        {
            List<GHPublishedWindowRow> publishedRows = new List<GHPublishedWindowRow>(_putStrs.Count);
            
            GHPublishedWindow clone = new GHPublishedWindow(
                _winType, _winStyle, _glyph, _useUpperSide, _useSpecialSymbols, _ascension, _objdata, _winId,
                Typeface, TextColor, TextSize, StrokeWidth, HasShadow, BackgroundColor, CursX, CursY,
                CenterHorizontally, AutoPlacement, AutoCarriageReturn, WindowPrintStyle, publishedRows,
                Visible, _width, _height, _pixelWidth, _pixelHeight, Padding, Left, Top,
                MenuInfo != null ? MenuInfo.Clone() : null,
                null, false, new WeakReference<GHWindow>(this)
            );

            foreach (var row in _putStrs)
            {
                publishedRows.Add(CreatePublishedRow(clone, row));
            }

            return clone;
        }

        private GHPublishedWindowRow CreatePublishedRow(GHPublishedWindow clone, GHWindowRow row)
        {
            List<GHPutStrInstructions> instructions = new List<GHPutStrInstructions>();
            string text = row.TextStringBuilder.ToString();
            
            int prevattr = 0, prevclr = 0;
            int curattr = 0, curclr = 0;
            int cnt = 0;

            for (int i = 0; i < text.Length; i++)
            {
                prevattr = curattr;
                prevclr = curclr;
                cnt++;
                curattr = i < row.AttributeList.Count ? row.AttributeList[i] : 0;
                curclr = i < row.ColorList.Count ? row.ColorList[i] : (int)NhColor.CLR_WHITE;

                if (cnt > 1 && (curattr != prevattr || curclr != prevclr))
                {
                    instructions.Add(new GHPutStrInstructions(prevattr, prevclr, cnt - 1));
                    cnt = 1;
                }
                
                if (i == text.Length - 1)
                {
                    instructions.Add(new GHPutStrInstructions(curattr, curclr, cnt));
                }
            }

            return new GHPublishedWindowRow(clone, text, instructions);
        }

        private GHSkiaFontPaint GetMeasurePaint()
        {
            float size = TextSize * UIUtils.CalculateTextScale();
            if (_measurePaint != null && _measureTypeface == Typeface && _measureTextSize == size)
                return _measurePaint;

            _measurePaint?.Dispose();
            _measurePaint = new GHSkiaFontPaint()
            {
                Typeface = Typeface,
                TextSize = size
            };
            _measureTypeface = Typeface;
            _measureTextSize = size;
            return _measurePaint;
        }

        public void PutStrEx(int attributes, string str, int append, int color)
        {
            GHSkiaFontPaint textPaint = GetMeasurePaint();

            if (CursY >= PutStrs.Count)
            {
                for (int i = 0; i < CursY - PutStrs.Count + 1; i++)
                {
                    PutStrs.Add(new GHWindowRow(this));
                }
            }

            if (CursY >= 0)
            {
                var row = PutStrs[CursY];
                if (row == null)
                {
                    row = new GHWindowRow(this);
                    PutStrs[CursY] = row;
                }

                int len = str.Length;
                int curlen = row.TextStringBuilder.Length;
                List<int> curattrs = row.AttributeList;
                List<int> curclrs = row.ColorList;

                int origCursX = CursX;

                if (CursX > curlen)
                {
                    row.TextStringBuilder.Append(' ', CursX - curlen);
                }
                else if (CursX < curlen)
                {
                    row.TextStringBuilder.Length = CursX;
                }

                row.TextStringBuilder.Append(str);

                CursX += str.Length;

                float textWidth = textPaint.MeasureText(row.TextStringBuilder.ToString());
                textWidth += Padding.Left + Padding.Right;
                if (textWidth > _pixelWidth)
                    _pixelWidth = textWidth;

                if (row.TextStringBuilder.Length > _width)
                    _width = row.TextStringBuilder.Length;

                if (CursY + 1 > _height)
                    _height = CursY + 1;

                int i;
                for (i = origCursX; i < CursX; i++)
                {
                    while (i > curattrs.Count)
                        curattrs.Add(0);

                    while (i > curclrs.Count)
                        curclrs.Add((int)NhColor.CLR_WHITE);

                    if (i == curattrs.Count)
                        curattrs.Add(attributes);
                    else if (i < curattrs.Count)
                        curattrs[i] = attributes;

                    if (i == curclrs.Count)
                        curclrs.Add(color);
                    else if (i < curattrs.Count)
                        curclrs[i] = color;
                }

                if (AutoCarriageReturn && append == 0)
                {
                    CursY++;
                    CursX = 0;
                }
            }

            float textHeight = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
            _pixelHeight = _height * textHeight + Padding.Top + Padding.Bottom;
            
            if(append == 0 && ShouldPublishClone)
            {
                _currentGame.RequestQueue.Enqueue(new GHRequest(_currentGame, GHRequestType.UpdateGHWindow, _winId, Publish()));
            }
        }

        public void PutStrEx2(string str, byte[] attributes, byte[] colors, int attribute, int color, int append)
        {
            if (str == null)
                return;
            if(attributes == null || colors == null)
                PutStrEx(attribute, str, append, color);
            else
            {
                GHSkiaFontPaint textPaint = GetMeasurePaint();

                if (CursY >= PutStrs.Count)
                {
                    for (int i = 0; i < CursY - PutStrs.Count + 1; i++)
                    {
                        PutStrs.Add(new GHWindowRow(this));
                    }
                }

                if (CursY >= 0)
                {
                    var row = PutStrs[CursY];
                    if (row == null)
                    {
                        row = new GHWindowRow(this);
                        PutStrs[CursY] = row;
                    }

                    int len = str.Length;
                    int curlen = row.TextStringBuilder.Length;
                    List<int> curattrs = row.AttributeList;
                    List<int> curclrs = row.ColorList;

                    int origCursX = CursX;

                    if (CursX > curlen)
                    {
                        row.TextStringBuilder.Append(' ', CursX - curlen);
                    }
                    else if (CursX < curlen)
                    {
                        row.TextStringBuilder.Length = CursX;
                    }

                    row.TextStringBuilder.Append(str);

                    CursX += str.Length;

                    float textWidth = textPaint.MeasureText(row.TextStringBuilder.ToString());
                    textWidth += Padding.Left + Padding.Right;
                    if (textWidth > _pixelWidth)
                        _pixelWidth = textWidth;

                    if (row.TextStringBuilder.Length > _width)
                        _width = row.TextStringBuilder.Length;

                    if (CursY + 1 > _height)
                        _height = CursY + 1;

                    int i, idx;
                    for (i = origCursX; i < CursX; i++)
                    {
                        idx = i - origCursX;
                        while (i > curattrs.Count)
                            curattrs.Add(attribute);

                        while (i > curclrs.Count)
                            curclrs.Add(color);

                        if (i == curattrs.Count)
                            curattrs.Add(idx < attributes.Length && attributes[idx] != (int)MenuItemAttributes.None ? attributes[idx] : attribute);
                        else if (i < curattrs.Count)
                            curattrs[i] = attribute;

                        if (i == curclrs.Count)
                            curclrs.Add(idx < colors.Length && colors[idx] != (int)NhColor.NO_COLOR ? colors[idx] : color);
                        else if (i < curattrs.Count)
                            curclrs[i] = color;
                    }

                    if (AutoCarriageReturn && append == 0)
                    {
                        CursY++;
                        CursX = 0;
                    }
                }

                float textHeight = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
                _pixelHeight = _height * textHeight + Padding.Top + Padding.Bottom;
            }
            if (append == 0 && ShouldPublishClone)
            {
                _currentGame.RequestQueue.Enqueue(new GHRequest(_currentGame, GHRequestType.UpdateGHWindow, _winId, Publish()));
            }
        }

        public SKRect GetWindowRect(float textScalingFactor)
        {
            return new SKRect(Left, Top, Left + UnscaledWidth * textScalingFactor, Top + UnscaledHeight * textScalingFactor);
        }

        public double TextWindowMaximumWidth
        {
            get
            {
                switch(_winStyle)
                {
                    case ghwindow_styles.GHWINDOW_STYLE_ENLIGHTENMENT_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_SEMI_WIDE_LIST:
                    case ghwindow_styles.GHWINDOW_STYLE_MONSTER_DESCRIPTION_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_OBJECT_DESCRIPTION_SCREEN:
                        return 520.0;
                    case ghwindow_styles.GHWINDOW_STYLE_CHARACTER_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_SPELL_DESCRIPTION_SCREEN:
                    case ghwindow_styles.GHWINDOW_STYLE_MEDIUM_WIDE_LIST:
                        return 440.0;
                    case ghwindow_styles.GHWINDOW_STYLE_NARROW_LIST:
                    case ghwindow_styles.GHWINDOW_STYLE_SKILL_DESCRIPTION_SCREEN:
                        return 360.0;
                    case ghwindow_styles.GHWINDOW_STYLE_DISPLAY_FILE:
                    case ghwindow_styles.GHWINDOW_STYLE_DISPLAY_FILE_WITH_INDENTED_TEXT:
                        return 800.0;
                    default:
                        return 600.0;
                }
            }
        }
        public bool WrapGlyph
        {
            get
            {
                switch (_winStyle)
                {
                    case ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER:
                    case ghwindow_styles.GHWINDOW_STYLE_OUTRIP:
                    case ghwindow_styles.GHWINDOW_STYLE_MONSTER_COMMAND_MENU:
                    case ghwindow_styles.GHWINDOW_STYLE_ENLIGHTENMENT_SCREEN:                        
                        return true;
                    default:
                        return false;
                }
            }
        }

        private int _disposed = 0;

        public void Dispose()
        {
            try
            {
                if (Interlocked.Exchange(ref _disposed, 1) == 0)
                {
                    _measurePaint?.Dispose();
                    _measurePaint = null;
                    _putStrs.Clear();
                    /* Do not dispose MenuInfo's GHMenuItems here;
                     * the render thread may still hold references to
                     * the same GHMenuItem objects and their text blobs.
                     * They will be GC'd when all references are released. */
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("GHWindow.Dispose error: " + ex.Message);
            }
        }
    }
}
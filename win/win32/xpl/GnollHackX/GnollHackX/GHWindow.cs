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

    public class GHPadding
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

    public class GHWindow
    {
        private GHWinType _winType = 0;
        private ghwindow_styles _winStyle = 0;
        private int _glyph;
        private ObjectDataItem _objdata = null;
        public ObjectDataItem ObjData { get { return _objdata; } set { _objdata = value; } }
        private GamePage _gamePage;
        private GHGame _currentGame;
        private int _winId;
        private bool _useUpperSide;
        private bool _useSpecialSymbols;
        private bool _ascension;

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

        private List<GHPutStrItem> _putStrs = new List<GHPutStrItem>();
        public List<GHPutStrItem> PutStrs { get { return _putStrs; } set { _putStrs = value; } }

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
        private readonly object _selectedMenuItemsLock = new object();
        public List<GHMenuItem> SelectedMenuItems
        {
            get { lock (_selectedMenuItemsLock) { return _selectedMenuItems; } }
            set { lock (_selectedMenuItemsLock) { _selectedMenuItems = value; } }
        }

        public bool WasCancelled { get; set; } = false;

        public GHWindow(GHWinType winType, ghwindow_styles winStyle, int glyph, bool useUpperSide, bool useSpecialSymbols, bool ascension, ObjectDataItem objdata, GamePage gamePage, int winid)
        {
            _winType = winType;
            _winStyle = winStyle;
            _glyph = glyph;
            _useUpperSide = useUpperSide;
            _useSpecialSymbols = useSpecialSymbols;
            _ascension = ascension;
            _objdata = objdata;
            _gamePage = gamePage;
            _currentGame = gamePage.CurrentGame;
            _winId = winid;
        }

        //private SKColor TransparentBlack = new SKColor(0, 0, 0, 128);
        //public void Create()
        //{
        //    Typeface = GHApp.LatoRegular;
        //    TextColor = SKColors.White;
        //    TextSize = GHConstants.WindowBaseFontSize;
        //    BackgroundColor = SKColors.Transparent;
        //    switch (_winType)
        //    {
        //        case GHWinType.None:
        //            break;
        //        case GHWinType.Message:
        //            TextSize = GHConstants.WindowMessageFontSize;
        //            Typeface = GHApp.DejaVuSansMonoTypeface;
        //            StrokeWidth = TextSize / 4.0f;
        //            AutoPlacement = true;
        //            break;
        //        case GHWinType.Status:
        //            TextSize = GHConstants.WindowStatusBarFontSize;
        //            Typeface = GHApp.LatoRegular;
        //            StrokeWidth = TextSize / 4.0f;
        //            HasShadow = true;
        //            Left = 0;
        //            Top = 0;
        //            break;
        //        case GHWinType.Map:
        //            TextSize = GHConstants.WindowStatusBarFontSize;
        //            Typeface = GHApp.LatoRegular;
        //            Left = 0;
        //            Top = 120;
        //            break;
        //        case GHWinType.Menu:
        //            TextSize = GHConstants.WindowMenuFontSize;
        //            Typeface = GHApp.UnderwoodTypeface;
        //            Left = 0;
        //            Top = 150;
        //            CenterHorizontally = true;
        //            break;
        //        case GHWinType.Text:
        //            TextSize = GHConstants.WindowMenuFontSize;
        //            Typeface = GHApp.UnderwoodTypeface;
        //            Left = 0;
        //            Top = 150;
        //            CenterHorizontally = true;
        //            break;
        //        case GHWinType.Base:
        //            break;
        //        case GHWinType.Here:
        //            TextSize = GHConstants.WindowMessageFontSize;
        //            Typeface = GHApp.DejaVuSansMonoTypeface;
        //            StrokeWidth = TextSize / 4.0f;
        //            AutoPlacement = true;
        //            break;
        //        case GHWinType.Inventory:
        //            break;
        //        case GHWinType.Reserved_1:
        //        case GHWinType.Reserved_2:
        //        case GHWinType.Reserved_3:
        //            break;
        //        case GHWinType.RIP:
        //            break;
        //        case GHWinType.Keypad:
        //            break;
        //        case GHWinType.Overview:
        //            break;
        //        case GHWinType.Worn:
        //            break;
        //    }

        //    //if (_winType == GHWinType.Menu || _winType == GHWinType.Text)
        //    //{
        //    //    ConcurrentQueue<GHRequest> queue;
        //    //    if (GHGame.RequestDictionary.TryGetValue(_currentGame, out queue))
        //    //    {
        //    //        queue.Enqueue(new GHRequest(_currentGame, GHRequestType.CreateWindowView, _winId, Clone()));
        //    //    }
        //    //}
        //}
        public WeakReference<GHWindow> ClonedFrom = null;
        public GHWindow Clone()
        {
            GHWindow clone = new GHWindow(_winType, _winStyle, _glyph, _useUpperSide, _useSpecialSymbols, _ascension, _objdata, _gamePage, _winId);
            List<GHPutStrItem> clonestrs = new List<GHPutStrItem>();
            foreach (GHPutStrItem item in PutStrs)
            {
                clonestrs.Add(item.Clone(clone));
            }
            clone.PutStrs = clonestrs;
            clone.TextColor = TextColor;
            clone.TextSize = TextSize;
            clone.Typeface = Typeface;
            clone.StrokeWidth = StrokeWidth;
            clone.AutoPlacement = AutoPlacement;
            clone.CursX = CursX;
            clone.CursY = CursY;
            clone.Visible = Visible;
            clone.BackgroundColor = BackgroundColor;
            clone.CenterHorizontally = CenterHorizontally;
            clone.HasShadow = HasShadow;
            clone.SetWidthHeight(_width, _height, _pixelWidth, _pixelHeight);
            clone.Padding = Padding;
            clone.MenuInfo = MenuInfo != null ? MenuInfo.Clone() : null;
            clone.Left = Left;
            clone.Top = Top;
            clone.SelectedMenuItems = SelectedMenuItems;
            clone.WasCancelled = WasCancelled;
            clone.ClonedFrom = new WeakReference<GHWindow>(this);
            return clone;
        }

        //public void Destroy()
        //{
        //    Visible = false;
        //    ConcurrentQueue<GHRequest> queue;
        //    if (GHGame.RequestDictionary.TryGetValue(_currentGame, out queue))
        //    {
        //        queue.Enqueue(new GHRequest(_currentGame, GHRequestType.DestroyWindowView, _winId));
        //        if (_winType == GHWinType.Menu && MenuInfo != null && MenuInfo.MenuCloseUponDestroy)
        //        {
        //            queue.Enqueue(new GHRequest(_currentGame, GHRequestType.HideMenuPage, _winId));
        //        }
        //    }
        //}

        //public void Clear()
        //{
        //    switch(WindowType)
        //    {
        //        case GHWinType.Map:
        //            ActiveGamePage.ClearMap();
        //            break;
        //    }

        //    PutStrs.Clear();

        //    _height = 0;
        //    _width = 0;
        //    _pixelWidth = 0;
        //    _pixelHeight = 0;
        //    CursX = 0;
        //    CursY = 0;

        //    ConcurrentQueue<GHRequest> queue;
        //    if (GHGame.RequestDictionary.TryGetValue(_currentGame, out queue))
        //    {
        //        if (_winType == GHWinType.Menu || _winType == GHWinType.Text)
        //            queue.Enqueue(new GHRequest(_currentGame, GHRequestType.ClearWindowView, _winId));
        //        queue.Enqueue(new GHRequest(_currentGame, GHRequestType.UpdateGHWindow, _winId, Clone()));
        //    }
        //}

        //public void Display(bool blocking)
        //{
        //    Visible = true;
        //    if(_winType == GHWinType.Menu || _winType == GHWinType.Text)
        //    {
        //        ConcurrentQueue<GHRequest> queue;
        //        if (GHGame.RequestDictionary.TryGetValue(_currentGame, out queue))
        //        {
        //            List<GHPutStrItem> clonestrs = new List<GHPutStrItem>();
        //            //lock (PutStrsLock) //Probably not needed since only reading from PutStrs (UI thread is not writing)
        //            {
        //                foreach (GHPutStrItem item in PutStrs)
        //                {
        //                    clonestrs.Add(item.Clone());
        //                }
        //            }
        //            queue.Enqueue(new GHRequest(_currentGame, GHRequestType.DisplayWindowView, _winId, clonestrs));
        //        }
        //    }
        //}
        //public void Curs(int x, int y)
        //{
        //    CursX = x;
        //    CursY = y;
        //    if(WindowType == GHWinType.Map)
        //        ActiveGamePage.SetMapCursor(x, y);
        //}
        //public void PrintGlyph(int x, int y, int glyph, int bkglyph, int symbol, int color, uint special, ref LayerInfo layers)
        //{
        //    ActiveGamePage.SetMapSymbol(x, y, glyph, bkglyph, symbol, color, special, ref layers);
        //}

        public void PutStrEx(int attributes, string str, int append, int color)
        {
            //lock (PutStrsLock)
            {
                using (GHSkiaFontPaint textPaint = new GHSkiaFontPaint()
                {
                    Typeface = Typeface,
                    TextSize = TextSize * UIUtils.CalculateTextScale()
                })
                {
                    if (CursY >= PutStrs.Count)
                    {
                        for (int i = 0; i < CursY - PutStrs.Count + 1; i++)
                        {
                            PutStrs.Add(new GHPutStrItem(_gamePage, this, ""));
                        }
                    }

                    if (CursY >= 0)
                    {
                        if (PutStrs[CursY] == null)
                            PutStrs[CursY] = new GHPutStrItem(_gamePage, this, "");
                        else if (PutStrs[CursY].Text == null)
                            PutStrs[CursY].Text = "";

                        int len = str.Length;
                        string curstr = PutStrs[CursY].Text;
                        int curlen = curstr.Length;
                        List<int> curattrs = PutStrs[CursY].AttributeList;
                        List<int> curclrs = PutStrs[CursY].ColorList;

                        //if (append != 0)
                        //    CursX = PutStrs[CursY].Text.Length;

                        int origCursX = CursX;

                        if (CursX > curlen)
                        {
                            int n = CursX - curlen;
                            string spaces = new String(' ', n);
                            curstr = curstr + spaces;
                            curlen = curstr.Length;
                        }

                        string leftstr = CursX <= 0 ? "" : curstr.Substring(0, CursX);
                        //string rightstr = curstr.Length <= CursX + len ? "" : curstr.Substring(CursX + len, curlen - (CursX + len));
                        PutStrs[CursY].Text = leftstr + str; // + rightstr;

                        CursX += str.Length;

                        // Adjust TextSize property so text is 90% of screen width
                        float textWidth = textPaint.MeasureText(PutStrs[CursY].Text); //.Replace(' ', '_'));
                        textWidth += Padding.Left + Padding.Right;
                        if (textWidth > _pixelWidth)
                            _pixelWidth = textWidth;

                        if (PutStrs[CursY].Text.Length > _width)
                            _width = PutStrs[CursY].Text.Length;

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

                        PutStrs[CursY].ConvertListFromArrays();

                        if (AutoCarriageReturn && append == 0)
                        {
                            CursY++;
                            CursX = 0;
                        }
                    }

                    float textHeight = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
                    _pixelHeight = _height * textHeight + Padding.Top + Padding.Bottom;
                }
            }
            if(append == 0)
            {
                _currentGame.RequestQueue.Enqueue(new GHRequest(_currentGame, GHRequestType.UpdateGHWindow, _winId, Clone()));
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
                //int len1 = str.Length;
                //int len2 = attributes.Length;
                //int len3 = colors.Length;
                //int minlen = Math.Min(Math.Min(len1, len2), len3);

                //for (int i = 0; i < minlen; i++)
                //    PutStrEx(attributes[i] == (int)MenuItemAttributes.None ? attribute : attributes[i], 
                //        str.Substring(i, 1), 
                //        i == minlen - 1 ? append : 1, 
                //        colors[i] == (int)NhColor.NO_COLOR ? color : colors[i]);

                //lock (PutStrsLock)
                {
                    using (GHSkiaFontPaint textPaint = new GHSkiaFontPaint()
                    {
                        Typeface = Typeface,
                        TextSize = TextSize * UIUtils.CalculateTextScale()
                    })
                    {
                        if (CursY >= PutStrs.Count)
                        {
                            for (int i = 0; i < CursY - PutStrs.Count + 1; i++)
                            {
                                PutStrs.Add(new GHPutStrItem(_gamePage, this, ""));
                            }
                        }

                        if (CursY >= 0)
                        {
                            if (PutStrs[CursY] == null)
                                PutStrs[CursY] = new GHPutStrItem(_gamePage, this, "");
                            else if (PutStrs[CursY].Text == null)
                                PutStrs[CursY].Text = "";

                            int len = str.Length;
                            string curstr = PutStrs[CursY].Text;
                            int curlen = curstr.Length;
                            List<int> curattrs = PutStrs[CursY].AttributeList;
                            List<int> curclrs = PutStrs[CursY].ColorList;

                            //if (append != 0)
                            //    CursX = PutStrs[CursY].Text.Length;

                            int origCursX = CursX;

                            if (CursX > curlen)
                            {
                                int n = CursX - curlen;
                                string spaces = new String(' ', n);
                                curstr = curstr + spaces;
                                curlen = curstr.Length;
                            }

                            string leftstr = CursX <= 0 ? "" : curstr.Substring(0, CursX);
                            //string rightstr = curstr.Length <= CursX + len ? "" : curstr.Substring(CursX + len, curlen - (CursX + len));
                            PutStrs[CursY].Text = leftstr + str; // + rightstr;

                            CursX += str.Length;

                            // Adjust TextSize property so text is 90% of screen width
                            float textWidth = textPaint.MeasureText(PutStrs[CursY].Text); //.Replace(' ', '_'));
                            textWidth += Padding.Left + Padding.Right;
                            if (textWidth > _pixelWidth)
                                _pixelWidth = textWidth;

                            if (PutStrs[CursY].Text.Length > _width)
                                _width = PutStrs[CursY].Text.Length;

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

                            PutStrs[CursY].ConvertListFromArrays();

                            if (AutoCarriageReturn && append == 0)
                            {
                                CursY++;
                                CursX = 0;
                            }
                        }

                        float textHeight = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
                        _pixelHeight = _height * textHeight + Padding.Top + Padding.Bottom;
                    }
                }
            }
            if (append == 0)
            {
                _currentGame.RequestQueue.Enqueue(new GHRequest(_currentGame, GHRequestType.UpdateGHWindow, _winId, Clone()));
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
    }
}
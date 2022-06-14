using GnollHackClient.Pages.Game;
using GnollHackCommon;
using SkiaSharp;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;

namespace GnollHackClient
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
        private ClientGame _clientGame;
        private int _winId;
        private bool _useUpperSide;

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
        public GamePage ClientGamePage { get { return _gamePage; } }
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
        public object PutStrsLock = new object();
        public List<GHPutStrItem> PutStrs { get { return _putStrs; } }

        public bool Visible { get; set; }
        private int _width = 0;
        private int _height = 0;

        public GHPadding Padding = new GHPadding(10, 10, 10 , 10);

        public int WidthInChars { get { return _width; } }
        public int HeightInChars { get { return _height; } }
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
        public float Right { get { return Left + _pixelWidth; } }
        public float Bottom { get { return Top + _pixelHeight; } }
        public float Width { get { return _pixelHeight; } }
        public float Height { get { return _pixelHeight; } }

        private float _pixelWidth = 0;
        private float _pixelHeight = 0;

        public GHMenuInfo MenuInfo { get; set; }

        private List<GHMenuItem> _selectedMenuItems = null;
        private object _selectedMenuItemsLock = new object();
        public List<GHMenuItem> SelectedMenuItems
        {
            get { lock (_selectedMenuItemsLock) { return _selectedMenuItems; } }
            set { lock (_selectedMenuItemsLock) { _selectedMenuItems = value; } }
        }

        public GHWindow(GHWinType winType, ghwindow_styles winStyle, int glyph, bool useUpperSide, ObjectDataItem objdata, GamePage gamePage, int winid)
        {
            _winType = winType;
            _winStyle = winStyle;
            _glyph = glyph;
            _useUpperSide = useUpperSide;
            ObjData = objdata;
            _gamePage = gamePage;
            _clientGame = gamePage.ClientGame;
            _winId = winid;
        }

        private SKColor TransparentBlack = new SKColor(0, 0, 0, 128);
        public void Create()
        {
            Typeface = App.LatoRegular;
            TextColor = SKColors.White;
            TextSize = 14.0f;
            BackgroundColor = SKColors.Transparent;
            switch (_winType)
            {
                case GHWinType.None:
                    break;
                case GHWinType.Message:
                    TextSize = TextSize * 32.0f / 42f;
                    Typeface = App.DejaVuSansMonoTypeface;
                    StrokeWidth = TextSize / 4.0f;
                    AutoPlacement = true;
                    break;
                case GHWinType.Status:
                    //BackgroundColor = TransparentBlack;
                    TextSize = TextSize * 30.0f / 42.0f;
                    Typeface = App.LatoRegular;
                    StrokeWidth = TextSize / 4.0f;
                    HasShadow = true;
                    Left = 0;
                    Top = 0;
                    break;
                case GHWinType.Map:
                    TextSize = TextSize * 30.0f / 42.0f;
                    Typeface = App.LatoRegular;
                    Left = 0;
                    Top = 120;
                    break;
                case GHWinType.Menu:
                    Typeface = App.UnderwoodTypeface;
                    TextSize = TextSize * 42.0f / 42.0f;
                    Left = 0;
                    Top = 150;
                    CenterHorizontally = true;
                    break;
                case GHWinType.Text:
                    Typeface = App.EndorTypeface;
                    TextSize = TextSize * 42.0f / 42.0f;
                    Left = 0;
                    Top = 150;
                    CenterHorizontally = true;
                    break;
                case GHWinType.Base:
                    break;
                case GHWinType.Here:
                    TextSize = TextSize * 32.0f / 42.0f;
                    Typeface = App.DejaVuSansMonoTypeface;
                    StrokeWidth = TextSize / 4.0f;
                    AutoPlacement = true;
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

            if (_winType == GHWinType.Menu || _winType == GHWinType.Text)
            {
                ConcurrentQueue<GHRequest> queue;
                if (ClientGame.RequestDictionary.TryGetValue(_clientGame, out queue))
                {
                    queue.Enqueue(new GHRequest(_clientGame, GHRequestType.CreateWindowView, _winId));
                }
            }
        }

        public void Destroy()
        {
            Visible = false;
            if (_winType == GHWinType.Menu)
            {
                ConcurrentQueue<GHRequest> queue;
                if (ClientGame.RequestDictionary.TryGetValue(_clientGame, out queue))
                {
                    queue.Enqueue(new GHRequest(_clientGame, GHRequestType.DestroyWindowView, _winId));
                    if (MenuInfo != null && MenuInfo.MenuCloseUponDestroy)
                    {
                        queue.Enqueue(new GHRequest(_clientGame, GHRequestType.HideMenuPage, _winId));
                    }
                }
            }
        }
        public void Clear()
        {
            switch(WindowType)
            {
                case GHWinType.Map:
                    ClientGamePage.ClearMap();
                    break;
            }

            lock(PutStrsLock)
            {
                PutStrs.Clear();
            }

            lock(_clientGame.WindowsLock)
            {
                _height = 0;
                _width = 0;
                _pixelWidth = 0;
                _pixelHeight = 0;
                CursX = 0;
                CursY = 0;

            }

            if (_winType == GHWinType.Menu || _winType == GHWinType.Text)
            {
                ConcurrentQueue<GHRequest> queue;
                if (ClientGame.RequestDictionary.TryGetValue(_clientGame, out queue))
                {
                    queue.Enqueue(new GHRequest(_clientGame, GHRequestType.ClearWindowView, _winId));
                }
            }
        }
        public void Display(bool blocking)
        {
            Visible = true;
            if(_winType == GHWinType.Menu || _winType == GHWinType.Text)
            {
                ConcurrentQueue<GHRequest> queue;
                if (ClientGame.RequestDictionary.TryGetValue(_clientGame, out queue))
                {
                    List<GHPutStrItem> clonestrs = new List<GHPutStrItem>();
                    lock (PutStrsLock)
                    {
                        clonestrs.AddRange(PutStrs);
                    }
                    queue.Enqueue(new GHRequest(_clientGame, GHRequestType.DisplayWindowView, _winId, clonestrs));
                }
            }
        }
        public void Curs(int x, int y)
        {
            CursX = x;
            CursY = y;
            if(WindowType == GHWinType.Map)
                ClientGamePage.SetMapCursor(x, y);
        }
        public void PrintGlyph(int x, int y, int glyph, int bkglyph, int symbol, int color, uint special, LayerInfo layers)
        {
            ClientGamePage.SetMapSymbol(x, y, glyph, bkglyph, symbol, color, special, layers);
        }

        public void PutStrEx(int attributes, string str, int append, int color)
        {
            lock (PutStrsLock)
            {
                SKPaint textPaint = new SKPaint()
                {
                    Typeface = Typeface,
                    TextSize = TextSize * _gamePage.GetTextScale()
                };

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
                    List<int> curattrs;
                    List<int> curclrs;
                    PutStrs[CursY].ConvertCurrentListToArrays(out curattrs, out curclrs);

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
                            curclrs.Add((int)nhcolor.CLR_WHITE);

                        if (i == curattrs.Count)
                            curattrs.Add(attributes);
                        else if (i < curattrs.Count)
                            curattrs[i] = attributes;

                        if (i == curclrs.Count)
                            curclrs.Add(color);
                        else if (i < curattrs.Count)
                            curclrs[i] = color;
                    }

                    PutStrs[CursY].ConvertCurrentListFromArrays(curattrs, curclrs);

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
    }
}
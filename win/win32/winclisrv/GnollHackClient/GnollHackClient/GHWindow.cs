using System;
using System.Collections.Generic;
using System.Text;
using GnollHackClient.Pages.Game;
using GnollHackCommon;
using SkiaSharp;

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
        private GamePage _gamePage;
        private int _winId;

        public SKTypeface Typeface { get; set; }
        public SKColor TextColor { get; set; }
        public float TextSize { get; set; }
        public SKColor BackgroundColor { get; set; }
        public int CursX { get; set; }
        public int CursY { get; set; }
        public bool CenterHorizontally { get; set; }
        public GHWinType WindowType { get { return _winType; } }
        public GamePage ClientGamePage { get { return _gamePage; } }
        public int WindowID { get { return _winId; } }

        public string[] PutStrs = new string[GHConstants.MaxPutStrHeight];
        public bool Visible { get; set; }
        private int _width = 0;
        private int _height = 0;

        public GHPadding Padding = new GHPadding(10, 10, 10 , 10);

        public int WidthInChars { get { return _width; } }
        public int HeightInChars { get { return _height; } }
        public float Left { get; set; }
        public float Top { get; set; }
        public float Right { get { return Left + _pixelWidth; } }
        public float Bottom { get { return Top + _pixelHeight; } }
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

        public GHWindow(GHWinType winType, GamePage gamePage, int winid)
        {
            _winType = winType;
            _gamePage = gamePage;
            _winId = winid;
        }

        public void Create()
        {
            Typeface = App.LatoRegular;
            TextColor = SKColors.White;
            TextSize = 42;
            BackgroundColor = new SKColor(25, 25, 25);
            switch (_winType)
            {
                case GHWinType.None:
                    break;
                case GHWinType.Message:
                    Typeface = App.DiabloTypeface;
                    Left = 0;
                    Top = 0;
                    break;
                case GHWinType.Status: /* Does not exist */
                    TextSize = 36;
                    Typeface = App.LatoRegular;
                    Left = 0;
                    Top = 0;
                    break;
                case GHWinType.Map:
                    TextSize = 36;
                    Typeface = App.LatoRegular;
                    Left = 0;
                    Top = 0;
                    break;
                case GHWinType.Menu:
                    Typeface = App.UnderwoodTypeface;
                    TextSize = 42;
                    Left = 0;
                    Top = 150;
                    CenterHorizontally = true;
                    break;
                case GHWinType.Text:
                    Typeface = App.EndorTypeface;
                    TextSize = 42;
                    Left = 0;
                    Top = 150;
                    CenterHorizontally = true;
                    break;
            }
        }

        public void Destroy()
        {
            Visible = false;
        }
        public void Clear()
        {
            switch(WindowType)
            {
                case GHWinType.Map:
                    ClientGamePage.ClearMap();
                    break;
            }
            for (int i = 0; i < GHConstants.MaxPutStrHeight; i++)
                PutStrs[i] = "";

            _height = 0;
            _width = 0;
            _pixelWidth = 0;
            _pixelHeight = 0;
            CursX = 0;
            CursY = 0;
        }
        public void Display(bool blocking)
        {
            Visible = true;
        }
        public void Curs(int x, int y)
        {
            CursX = x;
            CursY = y;
        }
        public void PrintGlyph(int x, int y, int symbol, int color, uint special)
        {
            ClientGamePage.SetMapSymbol(x, y, symbol, color, special);
        }

        public void PutStrEx(int attributes, string str, int append)
        {
            SKPaint textPaint = new SKPaint()
            {
                Typeface = Typeface,
                TextSize = TextSize
            };

            if (CursY >= 0 && CursY < GHConstants.MaxPutStrHeight)
            {
                if(append != 0)
                {
                    PutStrs[CursY] = PutStrs[CursY] + str;
                    CursX += str.Length;
                    if (PutStrs[CursY].Length > _width)
                        _width = PutStrs[CursY].Length;

                    float textWidth = textPaint.MeasureText(PutStrs[CursY]);
                    textWidth += Padding.Left + Padding.Right;
                    if (textWidth > _pixelWidth)
                        _pixelWidth = textWidth;
                }
                else
                {
                    if (PutStrs[CursY] == null || PutStrs[CursY] == "")
                    {
                        PutStrs[CursY] = str;
                    }
                    else if (CursX <= PutStrs[CursY].Length + 1)
                    {

                        PutStrs[CursY] = PutStrs[CursY].Substring(0, CursX) + str;
                    }
                    else
                    {
                        int n = CursX - 1 - PutStrs[CursY].Length;
                        string spaces = new String(' ', n);
                        PutStrs[CursY] = PutStrs[CursY].Substring(0, CursX - 1) + spaces + str;
                    }

                    // Adjust TextSize property so text is 90% of screen width
                    float textWidth = textPaint.MeasureText(PutStrs[CursY]);
                    textWidth += Padding.Left + Padding.Right;
                    if (textWidth > _pixelWidth)
                        _pixelWidth = textWidth;

                    if (PutStrs[CursY].Length > _width)
                        _width = PutStrs[CursY].Length;

                    if (CursY + 1 > _height)
                        _height = CursY + 1;

                    if ( CursY < GHConstants.MaxPutStrHeight - 1)
                        CursY++;

                    CursX = 0;
                }

            }

            float textHeight = textPaint.FontMetrics.Descent - textPaint.FontMetrics.Ascent;
            _pixelHeight = _height * textHeight + Padding.Top + Padding.Bottom;
        }
    }
}
using System;
using System.Collections.Generic;
using System.Text;
using GnollHackClient.Pages.Game;
using GnollHackCommon;

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

    public class GHWindow
    {
        private GHWinType _winType = 0;
        private GamePage _gamePage;
        public float WinX { get; set; }
        public float WinY { get; set; }
        public int CursX { get; set; }
        public int CursY { get; set; }
        public GHWinType WindowType { get { return _winType; } }
        public GamePage ClientGamePage { get { return _gamePage; } }
        public string[] PutStrs = new string[GHConstants.MaxPutStrHeight];

        public GHWindow(GHWinType winType, GamePage gamePage)
        {
            _winType = winType;
            _gamePage = gamePage;
        }

        public void Create()
        {
            switch (_winType)
            {
                case GHWinType.None:
                    break;
                case GHWinType.Message:
                    WinX = 0;
                    WinY = 100;
                    break;
                case GHWinType.Status:
                    WinX = 0;
                    WinY = 0;
                    break;
                case GHWinType.Map:
                    WinX = 0;
                    WinY = 200;
                    break;
                case GHWinType.Menu:
                    break;
                case GHWinType.Text:
                    break;
            }
        }

        public void Destroy()
        {

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
        }
        public void Display(bool blocking)
        {

        }
        public void Curs(int x, int y)
        {
            CursX = x;
            CursY = y;
        }
        public void PrintGlyph(int x, int y, string symbol, int color, uint special)
        {
            ClientGamePage.SetMapSymbol(x, y, symbol, color, special);
        }

        public void PutStrEx(int attributes, string str, int append)
        {
            if(CursY >= 0 && CursY < GHConstants.MaxPutStrHeight)
            {
                if(append != 0)
                {
                    PutStrs[CursY] = PutStrs[CursY] + str;
                }
                else
                {
                    if (PutStrs[CursY] == null || PutStrs[CursY] == "")
                        PutStrs[CursY] = str;
                    else if (CursX <= PutStrs[CursY].Length + 1)
                        PutStrs[CursY] = PutStrs[CursY].Substring(0, CursX) + str;
                    else
                    {
                        int n = CursX - 1 - PutStrs[CursY].Length;
                        string spaces = new String(' ', n);
                        PutStrs[CursY] = PutStrs[CursY].Substring(0, CursX - 1) + spaces + str;
                    }
                }
            }
        }
    }
}
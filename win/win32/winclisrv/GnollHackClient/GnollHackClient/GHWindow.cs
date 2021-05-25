using System;
using System.Collections.Generic;
using System.Text;
using GnollHackClient.Pages.Game;

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
        public int CursX { get; set; }
        public int CursY { get; set; }
        public GHWinType WindowType { get { return _winType; } }
        public GamePage ClientGamePage { get { return _gamePage; } }

        public GHWindow(GHWinType winType, GamePage gamePage)
        {
            _winType = winType;
            _gamePage = gamePage;
        }

        public void Create()
        {
        
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
            ClientGamePage.SetMapSymbol(x, y, symbol);
        }
    }
}

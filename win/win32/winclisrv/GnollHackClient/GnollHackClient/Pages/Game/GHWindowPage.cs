using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackClient.Pages.Game
{
    class GHWindowPage : ContentPage
    {
        private int _winid;
        public int WinId { get { return _winid; } }
        public GHWindowPage(int winid)
        {
            _winid = winid;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackClient
{
    public class GHMenuInfo
    {
        public List<GHMenuItem> MenuItems = new List<GHMenuItem>();
        public string Header { get; set; }
        public SelectionMode SelectionHow { get; set; }
        public GHMenuInfo()
        {

        }
    }
}

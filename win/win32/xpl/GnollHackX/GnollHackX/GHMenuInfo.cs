using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#else
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    public class GHMenuInfo
    {
        public ghmenu_styles Style { get; set; }
        public List<GHMenuItem> MenuItems = new List<GHMenuItem>();
        public string Header { get; set; }
        public string Subtitle { get; set; }
        public SelectionMode SelectionHow { get; set; }
        private char _autoAccelerator = 'a';
        public char AutoAccelerator { get { char res = _autoAccelerator; IncrementAutoAccelerator(); return res; } }
        private void IncrementAutoAccelerator()
        {
            _autoAccelerator++;
            if (_autoAccelerator == 'z' + 1)
                _autoAccelerator = 'A';
            if (_autoAccelerator == 'Z' + 1)
                _autoAccelerator = 'a';
        }
        public GHMenuInfo(ghmenu_styles style)
        {
            Style = style;
        }

        public bool MenuCloseUponDestroy
        {
            get
            {
                return UIUtils.StyleClosesMenuUponDestroy(Style);
            }
        }

    }
}

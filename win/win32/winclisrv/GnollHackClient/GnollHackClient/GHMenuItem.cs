using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHMenuItem
    {
        public char Accelerator { get; set; }
        public char GroupAccelerator { get; set; }
        public string Text { get; set; }
        public int Attributes { get; set; }
        public int Glyph { get; set; }
        public bool Preselected { get; set; }

        public GHMenuItem()
        {

        }
    }
}

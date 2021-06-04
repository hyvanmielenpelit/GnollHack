using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHMenuItem
    {
        public Int64 Identifier { get; set; }
        public int Count { get; set; }
        public char Accelerator { get; set; }
        public char GroupAccelerator { get; set; }
        public string Text { get; set; }
        public int Attributes { get; set; }
        public int Glyph { get; set; }
        public bool Selected { get; set; }
        public int NHColor { get; set; }
        public bool Is_Heading { get; set; }
        public char HeadingGroupAccelerator { get; set; }
        public UInt32 MenuFlags { get; set; }
        public UInt64 Oid { get; set; }
        public UInt64 Mid { get; set; }

        public GHMenuItem()
        {

        }
    }
}

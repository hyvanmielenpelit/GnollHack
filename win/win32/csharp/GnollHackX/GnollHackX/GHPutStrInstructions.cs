using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHPutStrInstructions
    {
        public int Attributes { get; set; }
        public int Color { get; set; }
        public int PrintLength { get; set; }

        public GHPutStrInstructions(int attr, int color, int len)
        {
            Attributes = attr;
            Color = color;
            PrintLength = len;
        }
    }
}

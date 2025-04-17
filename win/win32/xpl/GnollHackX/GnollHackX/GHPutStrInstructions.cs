using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHPutStrInstructions
    {
        public readonly int Attributes;
        public readonly int Color;
        public readonly int PrintLength;
        public GHPutStrInstructions(int attr, int color, int len)
        {
            Attributes = attr;
            Color = color;
            PrintLength = len;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackCommon
{
    public class GHUtils
    {
        public static int Ctrl(int c)
        {
            return (0x1f & (c));
        }
        public static int Meta(int c)
        {
            return (0x80 | (c));
        }
    }
}

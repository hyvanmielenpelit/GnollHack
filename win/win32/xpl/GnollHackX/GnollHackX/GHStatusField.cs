using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public struct GHStatusField
    {
        /* Base Data */
        public string Name;
        public string Format;
        public bool IsEnabled;

        /* Values */
        public string Text;
        public long Bits;
        public int Color;
        public int Change;
        public int Percent;

        public GHStatusField()
        {

        }
    }
}

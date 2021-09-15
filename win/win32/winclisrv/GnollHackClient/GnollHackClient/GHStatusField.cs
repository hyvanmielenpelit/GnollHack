using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHStatusField
    {
        /* Base Data */
        public string Name { get; set; }
        public string Format { get; set; }
        public bool IsEnabled { get; set; }

        /* Values */
        public string Text { get; set; }
        public long Bits { get; set; }
        public int Color { get; set; }
        public int Change { get; set; }
        public int Percent { get; set; }

        public GHStatusField()
        {

        }
    }
}

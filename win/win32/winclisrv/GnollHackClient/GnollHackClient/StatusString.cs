using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class StatusString
    {
        public string Text { get; set; } /* ascii string to be displayed */
        public bool SpaceInFront { get; set; } /* render with a space in front of string */
        public int Color { get; set; } /* string text color index */
        public int Attribute { get; set; } /* string text attributes */
        public bool DrawBar { get; set; } /* draw a percentage bar  */
        public int BarPercent { get; set; } /* a percentage to indicate */
        public int BarColor { get; set; } /* color index of percentage bar */
        public int BarAttribute { get; set; } /* attributes of percentage bar */

        public StatusString()
        {

        }
    }
}

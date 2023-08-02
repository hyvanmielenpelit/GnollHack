using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHOutRipInfo
    {
        public string PlayerName { get; set; }
        public int Points { get; set; }
        public string Killer { get; set; }
        public string Time { get; set; }
        public GHOutRipInfo(string playername, int points, string killer, string time)
        {
            PlayerName = playername;
            Points = points;
            Killer = killer;
            Time = time;
        }
    }
}

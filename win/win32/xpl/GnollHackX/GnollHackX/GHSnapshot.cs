using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHSnapshot
    {
        public int version;
        public ulong gnh_version;
        public ulong gnh_compatibility;

        public string name;
        public string dumplog;
        public string dumphtml;

        public int glyph;
        public int gui_glyph;

        public int rolenum;
        public int racenum;
        public sbyte gender;
        public sbyte alignment;
        public int ulevel;
        public long moves;
        public int dnum;
        public int dlvel;
        public sbyte depth;
        public sbyte difficulty;
        public byte wizard;
        public byte explore;
        public byte modern;
        public byte casual;
        public byte tournament;
        public byte nonscoring;
        public ulong collapses;
        public long timestamp;

        public string character;
        public string location;
        public string gamemode;
        public string timing;

        public GHSnapshot() 
        {

        }
    }
}

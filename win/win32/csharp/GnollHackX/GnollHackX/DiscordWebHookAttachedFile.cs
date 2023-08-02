using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class DiscordWebHookAttachedFile
    {
        public int id;
        public string description;
        public string filename;

        public DiscordWebHookAttachedFile(int init_id, string init_desc, string init_filename)
        {
            id = init_id;
            description = init_desc;
            filename = init_filename;
        }
    }
}

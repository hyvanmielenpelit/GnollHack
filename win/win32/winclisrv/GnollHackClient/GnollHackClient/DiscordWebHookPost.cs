using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class DiscordWebHookPost
    {
        public string content = "";

        public DiscordWebHookPost() 
        {

        }
        public DiscordWebHookPost(string initial_content)
        {
            content = initial_content;
        }
    }
}

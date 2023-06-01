using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class DiscordWebHookPostWithAttachment : DiscordWebHookPost
    {
        public DiscordWebHookAttachedFile[] attachments = new DiscordWebHookAttachedFile[1];

        public DiscordWebHookPostWithAttachment(string message) : base(message) 
        {
            
        }
        public DiscordWebHookPostWithAttachment(string message, string description, string attachment) : base(message)
        {
            attachments[0] = new DiscordWebHookAttachedFile(0, description, attachment);
        }
    }
}

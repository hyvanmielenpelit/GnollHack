using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class DiscordWebHookPostWithAttachment : DiscordWebHookPost
    {
        public DiscordWebHookAttachedFile[] attachments = null; // new DiscordWebHookAttachedFile[1];

        public DiscordWebHookPostWithAttachment(string message) : base(message) 
        {
            
        }

        public void AddAttachment(string description, string attachment)
        {
            List<DiscordWebHookAttachedFile> templist = new List<DiscordWebHookAttachedFile>();
            if(attachments != null)
                templist.AddRange(attachments);
            templist.Add(new DiscordWebHookAttachedFile(templist.Count, description, attachment));
            attachments = templist.ToArray(); 
        }
    }
}

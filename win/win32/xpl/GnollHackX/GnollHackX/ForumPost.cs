using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class ForumPost
    {
        public bool is_game_status;
        public int status_type;
        public int status_datatype;
        public string status_string;
        public List<ForumPostAttachment> attachments;

        public ForumPost() 
        {
            
        }

        public ForumPost(bool is_game_status, int status_type, int status_datatype, string status_string, List<ForumPostAttachment> attachments)
        {
            this.is_game_status = is_game_status;
            this.status_type = status_type;
            this.status_datatype = status_datatype;
            this.status_string = status_string;
            this.attachments = attachments;
        }   
    }
}

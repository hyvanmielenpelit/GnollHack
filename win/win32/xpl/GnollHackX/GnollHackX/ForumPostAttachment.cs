using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class ForumPostAttachment
    {
        public string FullPath { get; set; }
        public string ContentType { get; set; }
        public string Description { get; set; }
        public bool IsDiagnostic { get; set; }
        public int AttachmentType { get; set; }
        public bool IsTemporary { get; set; }


        public ForumPostAttachment()
        {

        }
        public ForumPostAttachment(string fullPath, string contentType, string description, bool isDiagnostic, int attachmentType, bool isTemporary)
        {
            FullPath = fullPath;
            ContentType = contentType;
            Description = description;
            IsDiagnostic = isDiagnostic;
            AttachmentType = attachmentType;
            IsTemporary = isTemporary;
        }
    }
}

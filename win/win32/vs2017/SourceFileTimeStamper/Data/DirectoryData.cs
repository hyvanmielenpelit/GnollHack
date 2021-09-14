using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace SourceFileTimeStamper.Data
{
    public class DirectoryData
    {
        public DirectoryData()
        {
            Extensions = new List<string>();
        }

        public string RelativePath { get; set; }
        public List<string> Extensions { get; set; }
        public SearchOption DirectorySearchOption { get; set; }
    }
}

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
            SearchPatterns = new List<string>();
            ExcludedFiles = new List<string>();
        }

        public string RelativePath { get; set; }
        public List<string> SearchPatterns { get; set; }
        public List<string> ExcludedFiles { get; set; }
        public SearchOption DirectorySearchOption { get; set; }
    }
}

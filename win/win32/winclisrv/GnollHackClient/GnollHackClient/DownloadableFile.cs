using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class DownloadableFile
    {
        public string name;
        public string description;
        public string version;
        public string download_url;
        public int length;
        public string sha256;

        public DownloadableFile()
        {

        }
    }
}

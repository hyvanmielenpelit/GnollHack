using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackCommon
{
    public class SecretsFile
    {
        public string id;
        public string name;
        public string type;
        public string description;
        public string version;
        public string source;
        public string source_path;
        public int length;
        public string sha256;
        public string target_directory;
        public int flags;

        public SecretsFile()
        {

        }
    }
}

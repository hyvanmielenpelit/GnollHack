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
        public int subtype_id;
        public string description;
        public string version;
        public string source_directory;
        public string source_url;
        public int length;
        public string sha256;
        public string target_directory;
        public int on_demand;
        public int streaming_asset;
        public int flags;

        public SecretsFile()
        {

        }
    }
}

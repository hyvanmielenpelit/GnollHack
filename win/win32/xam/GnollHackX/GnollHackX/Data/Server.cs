using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX.Data
{
    public class Server
    {
        public string Name { get; set; }
        public string Url { get; set; }

        public Server()
        {

        }

        public Server(string name, string url)
        {
            Name = name;
            Url = url;
        }
    }
}

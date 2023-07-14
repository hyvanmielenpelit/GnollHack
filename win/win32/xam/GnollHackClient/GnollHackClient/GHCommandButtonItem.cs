using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHCommandButtonItem
    {
        public string Text;
        public string ImageSourcePath;
        public int Command;

        public GHCommandButtonItem()
        {

        }
        public GHCommandButtonItem(string text, string imgsrcpath, int command)
        {
            Text = text;
            ImageSourcePath = imgsrcpath;
            Command = command;
        }
    }
}

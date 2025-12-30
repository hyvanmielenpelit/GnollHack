using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public sealed class GHCommandButtonItem
    {
        public string Text;
        public string ImageSourcePath;
        public int Command;

        public string CommandChar;
        public bool IsCtrl;
        public bool IsMeta;

        public GHCommandButtonItem()
        {

        }
        public GHCommandButtonItem(string text, string imgsrcpath, int command)
        {
            Text = text;
            ImageSourcePath = imgsrcpath;
            Command = command;

            int c = GHUtils.Unctrl(GHUtils.Unmeta(command));
            CommandChar = c >= 32 && c <= 123 ? ((char)c).ToString() : "";
            IsCtrl = GHUtils.IsCtrl(command);
            IsMeta = GHUtils.IsMeta(command);
        }
    }
}

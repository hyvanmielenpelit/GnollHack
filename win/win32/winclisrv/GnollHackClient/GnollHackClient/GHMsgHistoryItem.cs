using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHMsgHistoryItem
    {
        public string Text { get; set; }
        public uint Attributes { get; set; }
        public bool IsLast { get; set; }

        public GHMsgHistoryItem()
        {
            IsLast = false;
        }
        public GHMsgHistoryItem(string text)
        {
            Text = text;
            IsLast = false;
        }
        public GHMsgHistoryItem(string text, uint attr)
        {
            Text = text;
            Attributes = attr;
            IsLast = false;
        }
    }
}

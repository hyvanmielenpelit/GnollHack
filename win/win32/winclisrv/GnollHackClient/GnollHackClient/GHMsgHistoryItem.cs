using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;
using GnollHackClient;
using GnollHackCommon;

namespace GnollHackClient
{
    public class GHMsgHistoryItem
    {
        private string _text = "";
        public string Text { get { return _text; } set { if (value != null) { _text = value; _textSplit = _text.Split(' '); } } }

        private string[] _textSplit = null;
        public string[] TextSplit 
        { get
            {
                if (_textSplit == null)
                    return new string[1] { "" };
                else
                    return _textSplit;
            } 
        }
        public List<string> WrappedTextRows = new List<string>();
        public int Attribute { get; set; }
        public int NHColor { get; set; }

        public byte[] Attributes { get; set; } = null;
        public byte[] Colors { get; set; } = null;

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
        public GHMsgHistoryItem(string text, int attr, int color)
        {
            Text = text;
            Attribute = attr;
            NHColor = color;
            IsLast = false;
        }
        public GHMsgHistoryItem(string text, byte[] attrs, byte[] colors, int attr, int color)
        {
            Text = text;
            Attributes = attrs;
            Colors = colors;
            Attribute = attr;
            NHColor = color;
            IsLast = false;
        }
    }
}

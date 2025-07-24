﻿using System;
using System.Collections.Generic;
using System.Text;
using GnollHackX;

namespace GnollHackX
{
    public sealed class GHMsgHistoryItem
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

        private string _filter = null;
        public string Filter { get { return _filter; } set { _filter = value; _isMatchCalculated = false; } }
        private bool _isMatchCalculated = false;
        private bool _calculatedMatch = false;
        public bool MatchFilter
        {
            get
            {
                if (_isMatchCalculated)
                {
                    return _calculatedMatch; 
                }
                else
                {
                    if (Filter == null || Filter == "")
                    {
                        _calculatedMatch = true;
                        _isMatchCalculated = true;
                    }
                    else
                    {
                        _calculatedMatch = Text.IndexOf(Filter, StringComparison.OrdinalIgnoreCase) >= 0; // Text.Contains(Filter);
                        _isMatchCalculated = true;
                    }
                    return _calculatedMatch;
                }
            }
        }

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

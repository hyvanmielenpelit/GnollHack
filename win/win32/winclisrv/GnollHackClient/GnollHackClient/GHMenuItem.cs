using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Xamarin.Forms;
using SkiaSharp;
using System.IO;
using GnollHackClient.Pages.Game;
using GnollHackCommon;

namespace GnollHackClient
{
    public class GHMenuItem : IEquatable<GHMenuItem>, INotifyPropertyChanged
    {
        private GamePage _gamePage;
        private int _noGlyph;
        public Int64 Identifier { get; set; }
        private int _count;
        public int Count
        {
            get
            {
                return _count;
            }
            set
            {
                _count = value;
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs("Count"));
                    PropertyChanged(this, new PropertyChangedEventArgs("ConvertedCount"));
                    PropertyChanged(this, new PropertyChangedEventArgs("IsLabelVisible"));
                    PropertyChanged(this, new PropertyChangedEventArgs("IsSwipeVisible"));
                    PropertyChanged(this, new PropertyChangedEventArgs("LabelString"));
                }
            }
        }
        public bool IsLabelVisible { get { return (Count > 0 && MaxCount > 0); } }
        public string LabelString { get { return (MaxCount <= 0 ? "N/A" : Count == -1 ? "All" : Count.ToString()); } }
        public int ConvertedCount { get { return (Count + 1); } set { Count = value - 1; } }
        public bool IsSwipeVisible { get { return (Count != 0 && MaxCount > 1); } }
        public double MinimumRowHeight { get { return Identifier != 0 || HeadingGroupAccelerator != 0 ? 30.0 : 12.0; } }
        public string EntryString { get; set; }
        public Color EntryTextColor { get; set; }
        public int MaxCount { get; set; }
        public char Accelerator { get; set; }
        public char GroupAccelerator { get; set; }
        private string _text;
        private string _mainText;
        private string _suffixText;
        public string Text
        {
            get { return _text; }
            set
            {
                _text = value;
                int first_parenthesis_open = value.IndexOf('(');
                if(first_parenthesis_open < 0)
                    _mainText = value;
                else if (first_parenthesis_open > 0)
                    _mainText = value.Substring(0, first_parenthesis_open);
                else
                    _mainText = "";

                if (first_parenthesis_open >=0)
                    _suffixText = ParseSuffixText(value);
                else
                    _suffixText = "";
            }
        }
        public string MainText { get { return _mainText; } }
        public string SuffixText { get { return _suffixText; } }
        public bool IsSuffixTextVisible { get { return (SuffixText != null && SuffixText != ""); } }
        private string ParseSuffixText(string text)
        {
            string str = text;
            string res = "";
            while(!string.IsNullOrWhiteSpace(str))
            {
                int poidx = str.IndexOf('(');
                if (poidx < 0 || poidx >= str.Length - 1)
                    break;
                string searchstr = str.Substring(poidx + 1);
                int pcidx = searchstr.IndexOf(')');
                if (pcidx < 0)
                    break;

                if (pcidx > 0)
                {
                    string sufstr = searchstr.Substring(0, pcidx);
                    if(!string.IsNullOrWhiteSpace(sufstr))
                    {
                        if (res != "")
                            res = res + ", ";

                        res = res + sufstr;
                    }
                }

                if (pcidx >= searchstr.Length - 1)
                    break;
                str = searchstr.Substring(pcidx + 1);
            }
            
            return res;
        }
        public LayoutOptions MainTextVerticalOptions { get { return IsSuffixTextVisible ? LayoutOptions.End : LayoutOptions.Center; } }
        public TextAlignment MainTextVerticalTextAlign { get { return IsSuffixTextVisible ? TextAlignment.End : TextAlignment.Center; } }
        public bool IsGlyphVisible { get { return (Glyph != _noGlyph); } }

        public int Attributes { get; set; }

        private int _glyph;
        public int Glyph { get { return _glyph; } set { _glyph = value; _glyphImageSource.Glyph = value; } }
        public int NHColor { get; set; }
        public bool Is_Heading { get; set; }
        public char HeadingGroupAccelerator { get; set; }
        public ulong MenuFlags { get; set; }
        public UInt64 Oid { get; set; }
        public UInt64 Mid { get; set; }

        public bool Equals(GHMenuItem other)
        {
            if (other == null) 
                return false;
            if(this.Identifier != 0)
                return (this.Identifier.Equals(other.Identifier));
            else
                return (this.Text.Equals(other.Text));
        }

        public GHMenuItem(int noGlyph, GamePage gamePage)
        {
            _noGlyph = noGlyph;
            _gamePage = gamePage;
            _glyphImageSource.GamePage = gamePage;
            _glyphImageSource.X = 0;
            _glyphImageSource.Y = GHConstants.TileHeight / 2;
            _glyphImageSource.Width = GHConstants.TileWidth;
            _glyphImageSource.Height = GHConstants.TileHeight / 2;
            EntryTextColor = Color.White;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private GlyphImageSource _glyphImageSource = new GlyphImageSource();

        public ImageSource GlyphImage
        {
            get {
                return _glyphImageSource;
            }
        }
    }
}

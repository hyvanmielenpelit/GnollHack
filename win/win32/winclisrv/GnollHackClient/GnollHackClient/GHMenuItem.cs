using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Xamarin.Forms;
using SkiaSharp;
using System.IO;
using GnollHackClient.Pages.Game;
using GnollHackCommon;
using System.Linq;

namespace GnollHackClient
{
    public class GHMenuItem : IEquatable<GHMenuItem>, INotifyPropertyChanged
    {
        GHMenuInfo _menuInfo;
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
        public char SpecialMark { get; set; }
        public string FormattedAccelerator
        { 
            get
            { string res = Accelerator.ToString(); 
                if (res == "" || res == "\0")
                    return "";
                else
                    return "   " + res;
            }
        }
        public string FormattedSpecialMark
        {
            get
            {
                string res = SpecialMark.ToString();
                if (res == "" || res == "\0")
                    return "";
                else
                    return "   " + res;
            }
        }
        private string _text;
        private string _mainText;
        private string _suffixText;
        public string Text
        {
            get { return _text; }
            set
            {
                _text = value;
                int first_parenthesis_open = -1;
                if (SuffixParseStyle == 0 || (SuffixParseStyle == 2 && ((int)Attributes & (int)MenuItemAttributes.Bold) == 0))
                    first_parenthesis_open = -1;
                else
                    first_parenthesis_open = value.IndexOf('(');

                if (first_parenthesis_open <= 0) /* Ignore cases where the entire row is in parentheses */
                    _mainText = value;
                else if (first_parenthesis_open > 1)
                    _mainText = value.Substring(0, first_parenthesis_open);
                else
                    _mainText = "";

                if (first_parenthesis_open > 0 && !(_menuInfo.Style == ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND))  /* Ignore cases where the entire row is in parentheses */
                    _suffixText = ParseSuffixText(value);
                else
                    _suffixText = "";
            }
        }
        public int SuffixParseStyle
        {
            get
            {
                int res = 0;
                switch (_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_GENERAL:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                    case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                        res = 2;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_HELP:
                        break;
                    case ghmenu_styles.MAX_GHMENU_STYLES:
                        break;
                }
                return res;
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
        public LayoutOptions MainTextVerticalOptions { get { return IsSuffixTextVisible ? LayoutOptions.EndAndExpand : LayoutOptions.CenterAndExpand; } }
        public TextAlignment MainTextVerticalTextAlign { get { return IsSuffixTextVisible ? TextAlignment.End : TextAlignment.Center; } }
        public bool IsGlyphVisible { get { return (Glyph != _noGlyph); } }

        public int Attributes { get; set; }

        private int _glyph;
        public int Glyph { get { return _glyph; } set { _glyph = value; _glyphImageSource.Glyph = value; } }
        private ObjectDataItem _objData = null;
        public ObjectDataItem ObjData { get { return _objData; } set { _objData = value; _glyphImageSource.ObjData = value; } }
        public int NHColor { get; set; }
        public bool Is_Heading { get; set; }
        public char HeadingGroupAccelerator { get; set; }
        public bool HeadingUnselectGroup { get; set; }
        public ulong MenuFlags { get; set; }
        public UInt64 Oid { get; set; }
        public UInt64 Mid { get; set; }

        public string FontFamily {
            get 
            {
                if (((int)Attributes & ((int)MenuItemAttributes.Bold | (int)MenuItemAttributes.Heading | (int)MenuItemAttributes.Title)) != 0)
                {
                    string res = "Diablo";
                    switch (_menuInfo.Style)
                    {
                        case ghmenu_styles.GHMENU_STYLE_GENERAL:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                        case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHAT:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SKILLS:
                            res = "DejaVuSansMono-Bold";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SPELLS:
                            res = "DejaVuSansMono-Bold";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                            res = "DejaVuSansMono-Bold";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_HELP:
                            res = "DejaVuSansMono-Bold";
                            break;
                        case ghmenu_styles.MAX_GHMENU_STYLES:
                            break;
                    }
                    return res;
                }
                else 
                {
                    string res = "Underwood";
                    switch (_menuInfo.Style)
                    {
                        case ghmenu_styles.GHMENU_STYLE_GENERAL:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                        case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHAT:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SKILLS:
                            res = "DejaVuSansMono";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SPELLS:
                            res = "DejaVuSansMono";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                            res = "DejaVuSansMono";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_HELP:
                            res = "DejaVuSansMono";
                            break;
                        case ghmenu_styles.MAX_GHMENU_STYLES:
                            break;
                    }
                    return res;
                }
            }
        }

        private double _fontSize = 0;
        public double FontSize
        {
            get
            {
                double res = 15;
                if (((int)Attributes & ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title)) == ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title))
                {
                    res = 18.5;
                }
                else if (((int)Attributes & ((int)MenuItemAttributes.Title)) == ((int)MenuItemAttributes.Title))
                {
                    res = 20;
                }
                else if (((int)Attributes & ((int)MenuItemAttributes.Bold | (int)MenuItemAttributes.Heading)) != 0)
                {
                    res = 18;
                }
                switch (_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_GENERAL:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                        res = 17;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                        res = 18;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        res = 20;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS:
                        res = Math.Min(18, Math.Max(11.75, 11.9 * _gamePage.CurrentPageWidth / 600));
                        break;
                    case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                        res = Math.Min(18, Math.Max(11.75, 14 * _gamePage.CurrentPageWidth / 600));
                        break;
                    case ghmenu_styles.GHMENU_STYLE_HELP:
                        break;
                    case ghmenu_styles.MAX_GHMENU_STYLES:
                        break;
                }

                if(res != _fontSize)
                {
                    _fontSize = res;
                    OnPropertyChanged("FontSize");
                }
                return _fontSize; 
            }
        }

        public bool Equals(GHMenuItem other)
        {
            if (other == null) 
                return false;
            if(this.Identifier != 0)
                return (this.Identifier.Equals(other.Identifier));
            else
                return (this.Text.Equals(other.Text));
        }

        public GHMenuItem(GHMenuInfo info, int noGlyph, GamePage gamePage)
        {
            _menuInfo = info;
            _noGlyph = noGlyph;
            _gamePage = gamePage;
            _glyphImageSource.ReferenceGamePage = gamePage;
            _glyphImageSource.Width = GHConstants.TileWidth;
            _glyphImageSource.Height = GHConstants.TileHeight / 2;
            EntryTextColor = Color.White;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private GlyphImageSource _glyphImageSource = new GlyphImageSource();

        public GlyphImageSource GlyphImageSource
        {
            get
            {
                return _glyphImageSource;
            }
        }

        public ImageSource GlyphImage
        {
            get {
                return _glyphImageSource;
            }
        }

        private void OnPropertyChanged(string propertyname)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyname));
        }

        public SKRect DrawBounds = new SKRect();

        public bool IsVisibleOnCanvas(float canvaswidth, float canvasheight)
        {
            if (DrawBounds.Bottom <= 0 || DrawBounds.Top >= canvasheight)
                return false;

            return true;
        }

        public bool Selected { get; set; }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#else
using Xamarin.Forms;
using GnollHackX.Pages.Game;
using Xamarin.Forms.Internals;
#endif
using SkiaSharp;
using System.IO;
using System.Linq;
using System.Threading;

namespace GnollHackX
{
    public sealed class GHMenuItem : IEquatable<GHMenuItem> //, INotifyPropertyChanged
    {
        public GHMenuItem(GHMenuInfo info, int noGlyph, GamePage gamePage, Int64 identifier, char accelerator, char groupAccelerator, char specialMark,
            int attr, int color, byte[] attrs, byte[] colors, int glyph, bool useUpperSide, bool useColorForSuffixes, bool useSpecialSymbols,
            ObjectDataItem objData, string text, int count, bool useNumItems, int numItems, bool isButtonStyle, bool isAutoClickOk, bool selected,
            int maxCount, ulong oid, ulong mid, char headingGroupAccelerator, ulong flags)
        {
            _menuInfo = info;
            _noGlyph = noGlyph;
            _gamePage = gamePage;
            GlyphImageSource.ReferenceGamePage = gamePage;
            GlyphImageSource.UseUpperSide = useUpperSide;
            GlyphImageSource.Width = GHConstants.TileWidth;
            GlyphImageSource.Height = GHConstants.TileHeight / 2;
            Identifier = identifier;    
            Accelerator = accelerator;
            FormattedAccelerator = accelerator == '\0' ? "" : accelerator.ToString();
            GroupAccelerator = groupAccelerator;
            SpecialMark = specialMark;
            FormattedSpecialMark = specialMark == '\0' ? "" : specialMark.ToString();

            NHAttribute = attr;
            NHColor = color;
            NHAttributes = attrs;
            NHColors = colors;
            Glyph = glyph;
            GlyphImageSource.Glyph = glyph;

            //UseUpperSide = useUpperSide;
            UseColorForSuffixes = useColorForSuffixes;
            UseSpecialSymbols = useSpecialSymbols;
            ObjData = objData;
            GlyphImageSource.ObjData = objData;
            Text = text;
            Count = count;
            UseNumItems = useNumItems;
            NumItems = numItems;
            IsButtonStyle = isButtonStyle;
            IsAutoClickOk = isAutoClickOk;
            Selected = selected;
            MaxCount = maxCount;
            Oid = oid;
            Mid = mid;
            HeadingGroupAccelerator = headingGroupAccelerator;
            Flags = flags;

            /* Process text */
            int first_parenthesis_open = -1;
            bool altdivisors = ((int)NHAttribute & (int)MenuItemAttributes.AltDivisors) != 0;
            if (SuffixParseStyle == 0 || (SuffixParseStyle == 2 && ((int)NHAttribute & (int)MenuItemAttributes.Bold) == 0))
                first_parenthesis_open = -1;
            else
                first_parenthesis_open = text.IndexOf(altdivisors ? '|' : '(');

            if (first_parenthesis_open <= 0) /* Ignore cases where the entire row is in parentheses */
                _mainText = text;
            else if (first_parenthesis_open > 1)
                _mainText = text.Substring(0, first_parenthesis_open);
            else
                _mainText = "";

            _trimmedMainText = _mainText.Trim();
            MainTextSplit = _trimmedMainText.Split(' ');

            if (first_parenthesis_open > 0 && !(_menuInfo.Style == ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND || _menuInfo.Style == ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND))  /* Ignore cases where the entire row is in parentheses */
            {
                _suffixText = ParseSuffixText(text, false, altdivisors);
                _suffix2Text = ParseSuffixText(text, true, altdivisors);
            }
            else
            {
                _suffixText = "";
                _suffix2Text = "";
            }
            _trimmedSuffixText = _suffixText.Trim();
            SuffixTextSplit = _trimmedSuffixText.Split(' ');
            _trimmedSuffix2Text = _suffix2Text.Trim();
            Suffix2TextSplit = _trimmedSuffix2Text.Split(' ');

            ProcessSplitAttrsColors();
        }

        /* Private readonly */
        private readonly GHMenuInfo _menuInfo;
        private readonly GamePage _gamePage;
        private readonly int _noGlyph;

        /* Public readonly */
        public readonly Int64 Identifier;
        public readonly int MaxCount = 0;
        public readonly bool UseNumItems = false;
        public readonly int NumItems = 0;
        public readonly char Accelerator;
        public readonly char GroupAccelerator;
        public readonly char SpecialMark;
        public readonly string FormattedAccelerator = "";
        public readonly string FormattedSpecialMark = "";
        public readonly string Text = "";
        public readonly bool IsButtonStyle;
        public readonly bool IsAutoClickOk;
        public readonly int NHAttribute;
        public readonly int NHColor;
        public readonly byte[] NHAttributes = null;
        public readonly byte[] NHColors = null;
        public readonly int Glyph;
        public readonly bool UseSpecialSymbols = false;
        public readonly bool UseColorForSuffixes = false;
        public readonly ObjectDataItem ObjData = null;
        public readonly char HeadingGroupAccelerator;
        public readonly ulong Flags;
        public readonly UInt64 Oid;
        public readonly UInt64 Mid;
        public readonly GlyphImageSource GlyphImageSource = new GlyphImageSource();


        /* Interlocked properties */
        private int _selected = 0;
        private int _highlighted = 0;
        private int _count = 0;
        private int _headingUnselectGroup = 0;
        private int _textRowCountsSet = 0;
        private int _mainTextRows = 1;
        private int _suffixTextRows = 0;
        private int _suffix2TextRows = 0;

        public bool Selected { get { return Interlocked.CompareExchange(ref _selected, 0, 0) != 0; } set { Interlocked.Exchange(ref _selected, value ? 1 : 0); } }
        public bool Highlighted { get { return Interlocked.CompareExchange(ref _highlighted, 0, 0) != 0; } set { Interlocked.Exchange(ref _highlighted, value ? 1 : 0); } }
        public int Count { get { return Interlocked.CompareExchange(ref _count, 0, 0); } set { Interlocked.Exchange(ref _count, value); } }
        public bool HeadingUnselectGroup { get { return Interlocked.CompareExchange(ref _headingUnselectGroup, 0, 0) != 0; } set { Interlocked.Exchange(ref _headingUnselectGroup, value ? 1 : 0); } }
        public bool TextRowCountsSet { get { return Interlocked.CompareExchange(ref _textRowCountsSet, 0, 0) != 0; } set { Interlocked.Exchange(ref _textRowCountsSet, value ? 1 : 0); } }
        public int MainTextRows { get { return Interlocked.CompareExchange(ref _mainTextRows, 0, 0); } set { Interlocked.Exchange(ref _mainTextRows, value); } }
        public int SuffixTextRows { get { return Interlocked.CompareExchange(ref _suffixTextRows, 0, 0); } set { Interlocked.Exchange(ref _suffixTextRows, value); } }
        public int Suffix2TextRows { get { return Interlocked.CompareExchange(ref _suffix2TextRows, 0, 0); } set { Interlocked.Exchange(ref _suffix2TextRows, value); } }


        /* Get only properties */
        public bool IsGlyphVisible { get { return (Glyph != _noGlyph); } }
        public bool UseUpperSide { get { return GlyphImageSource.UseUpperSide; } }
        public bool IsObjWorn { get { return ObjData?.IsWorn ?? false; } }
        public long ObjWornBits { get { return ObjData?.WornBits ?? 0L; } }
        public bool IsObjArmor { get { return ObjData?.IsArmor ?? false; } }
        public bool IsObjShield { get { return ObjData?.IsShield ?? false; } }
        public bool IsBimanual { get { return ObjData?.IsBimanual ?? false; } }
        public float MinimumTouchableTextSize { get { return GHConstants.MenuDefaultRowHeight; } }
        public bool HasMultiColors { get { return NHAttributes != null && NHColors != null && Text != null; } }


        /* Readonly text splits */
        private readonly string _mainText;
        private readonly string _suffixText;
        private readonly string _suffix2Text;
        private readonly string _trimmedMainText;
        private readonly string _trimmedSuffixText;
        private readonly string _trimmedSuffix2Text;
        private readonly List<byte[]> _mainSplitAttrs = new List<byte[]>(1);
        private readonly List<byte[]> _mainSplitColors = new List<byte[]>(1);
        private readonly List<byte[]> _suffixSplitAttrs = new List<byte[]>(1);
        private readonly List<byte[]> _suffixSplitColors = new List<byte[]>(1);
        private readonly List<byte[]> _suffix2SplitAttrs = new List<byte[]>(1);
        private readonly List<byte[]> _suffix2SplitColors = new List<byte[]>(1);

        public string MainText { get { return _mainText; } }
        public string TrimmedMainText { get { return _trimmedMainText; } }
        public string SuffixText { get { return _suffixText; } }
        public string TrimmedSuffixText { get { return _trimmedSuffixText; } }
        public bool IsSuffixTextVisible { get { return (SuffixText != null && SuffixText != ""); } }
        public string Suffix2Text { get { return _suffix2Text; } }
        public string TrimmedSuffix2Text { get { return _trimmedSuffix2Text; } }
        public bool IsSuffix2TextVisible { get { return (Suffix2Text != null && Suffix2Text != ""); } }
        public List<byte[]> MainSplitAttrs { get { return HasMultiColors ? _mainSplitAttrs : null; } }
        public List<byte[]> MainSplitColors { get { return HasMultiColors ? _mainSplitColors : null; } }
        public List<byte[]> SuffixSplitAttrs { get { return HasMultiColors ? _suffixSplitAttrs : null; } }
        public List<byte[]> SuffixSplitColors { get { return HasMultiColors ? _suffixSplitColors : null; } }
        public List<byte[]> Suffix2SplitAttrs { get { return HasMultiColors ? _suffix2SplitAttrs : null; } }
        public List<byte[]> Suffix2SplitColors { get { return HasMultiColors ? _suffix2SplitColors : null; } }


        /* Interlocked text splits and row widths */
        private List<float> _mainTextRowWidths = new List<float>() { 0.0f };
        private List<float> _suffixTextRowWidths = new List<float>();
        private List<float> _suffix2TextRowWidths = new List<float>();
        public List<float> MainTextRowWidths { get { return Interlocked.CompareExchange(ref _mainTextRowWidths, null, null); } set { Interlocked.Exchange(ref _mainTextRowWidths, value); } }
        public List<float> SuffixTextRowWidths { get { return Interlocked.CompareExchange(ref _suffixTextRowWidths, null, null); } set { Interlocked.Exchange(ref _suffixTextRowWidths, value); } }
        public List<float> Suffix2TextRowWidths { get { return Interlocked.CompareExchange(ref _suffix2TextRowWidths, null, null); } set { Interlocked.Exchange(ref _suffix2TextRowWidths, value); } }

        private string[] _mainTextSplit;
        public string[] MainTextSplit
        {
            get
            {
                var textSplit = Interlocked.CompareExchange(ref _mainTextSplit, null, null);
                if (textSplit == null)
                    return new string[1] { "" };
                else
                    return textSplit;
            }
            set
            {
                Interlocked.Exchange(ref _mainTextSplit, value);
            }
        }
        private string[] _suffixTextSplit;
        public string[] SuffixTextSplit
        {
            get
            {
                var textSplit = Interlocked.CompareExchange(ref _suffixTextSplit, null, null);
                if (textSplit == null)
                    return new string[1] { "" };
                else
                    return textSplit;
            }
            set
            {
                Interlocked.Exchange(ref _suffixTextSplit, value);
            }
        }
        private string[] _suffix2TextSplit;
        public string[] Suffix2TextSplit
        {
            get
            {
                var textSplit = Interlocked.CompareExchange(ref _suffix2TextSplit, null, null);
                if (textSplit == null)
                    return new string[1] { "" };
                else
                    return textSplit;
            }
            set
            {
                Interlocked.Exchange(ref _suffix2TextSplit, value);
            }
        }


        /* METHODS */
        public void ProcessSplitAttrsColors()
        {
            if (!HasMultiColors)
                return;

            int idx = -1;
            List<string[]> textSplits = new List<string[]>() { MainTextSplit, SuffixTextSplit, Suffix2TextSplit };
            List<List<byte[]>> attrSplits = new List<List<byte[]>>() { _mainSplitAttrs, _suffixSplitAttrs, _suffix2SplitAttrs };
            List<List<byte[]>> colorSplits = new List<List<byte[]>>() { _mainSplitColors, _suffixSplitColors, _suffix2SplitColors };
            for (int splitIdx = 0; splitIdx < 3; splitIdx++)
            {
                string[] usedTextSplit = textSplits[splitIdx];
                List<byte[]> usedAttrSplit = attrSplits[splitIdx];
                List<byte[]> usedColorSplit = colorSplits[splitIdx];
                usedAttrSplit.Clear();
                usedColorSplit.Clear();
                if (usedTextSplit != null)
                {
                    for (int i = 0; i < usedTextSplit.Length; i++)
                    {
                        string splitText = usedTextSplit[i];
                        int len = splitText.Length;
                        byte[] splitAttrs = new byte[len + 1];
                        byte[] splitColors = new byte[len + 1];
                        for (int j = 0; j < len; j++)
                        {
                            splitAttrs[j] = 0;
                            splitColors[j] = (int)NhColor.NO_COLOR;
                        }
                        if (idx + 1 < Text.Length)
                        {
                            int startIdx = idx = Text.IndexOf(splitText, idx + 1);
                            if (idx >= 0)
                            {
                                for (; idx - startIdx < splitText.Length && idx < Text.Length; idx++)
                                {
                                    splitAttrs[idx - startIdx] = NHAttributes[idx];
                                    splitColors[idx - startIdx] = NHColors[idx];
                                }
                            }
                        }
                        splitAttrs[len] = 0;
                        splitColors[len] = 0;
                        usedAttrSplit.Add(splitAttrs);
                        usedColorSplit.Add(splitColors);
                    }
                }
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
                    case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                    case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                    case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL_ALTERNATE:
                    case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                    case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                    case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                    case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    case ghmenu_styles.GHMENU_STYLE_ACCEPT_PLAYER:
                    case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                        res = 1;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                    case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL:
                    case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                        res = 1; // 2;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_HELP:
                        break;
                    case ghmenu_styles.MAX_GHMENU_STYLES:
                        break;
                }
                return res;
            }
        }

        private string ParseSuffixText(string text, bool issuffix2, bool altdivisors)
        {
            string str = text;
            string res = "";
            char startchar = altdivisors ? (issuffix2 ? '^' : '|') : issuffix2 ? '{' : '(';
            char endchar = altdivisors ? (issuffix2 ? '^' : '|') : issuffix2 ? '}' : ')';
            while (!string.IsNullOrWhiteSpace(str))
            {
                int poidx = str.IndexOf(startchar);
                if (poidx < 0 || poidx >= str.Length - 1)
                    break;
                string searchstr = str.Substring(poidx + 1);
                int pcidx = searchstr.IndexOf(endchar);
                if (pcidx < 0)
                    break;

                if (pcidx > 0)
                {
                    string sufstr = searchstr.Substring(0, pcidx);
                    if (!string.IsNullOrWhiteSpace(sufstr))
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

        public string FontFamily {
            get 
            {
                if (((int)NHAttribute & ((int)MenuItemAttributes.Bold | (int)MenuItemAttributes.Heading | (int)MenuItemAttributes.Title)) != 0)
                {
                    string res = "Diablo";
                    switch (_menuInfo.Style)
                    {
                        case ghmenu_styles.GHMENU_STYLE_GENERAL:
                        case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                        case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                        case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                        case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                        case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL_ALTERNATE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                        case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
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
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ACCEPT_PLAYER:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SKILLS:
                            res = "DejaVuSansMonoBold";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SPELLS:
                        case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL:
                            res = "DejaVuSansMonoBold";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                            res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                            res = "DejaVuSansMonoBold";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_HELP:
                            res = "DejaVuSansMonoBold";
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
                        case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                        case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                        case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                        case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                        case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL_ALTERNATE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                        case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                        case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHAT:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        case ghmenu_styles.GHMENU_STYLE_ACCEPT_PLAYER:
                            if(Identifier != 0)
                                res = "Immortal";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SKILLS:
                            res = "DejaVuSansMono";
                            break;
                        case ghmenu_styles.GHMENU_STYLE_SPELLS:
                        case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL:
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

        //private double _fontSize = 0;
        public double FontSize
        {
            get
            {
                double res = 15.0;
                if (((int)NHAttribute & ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title)) == ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title))
                {
                    res = 18.5;
                }
                else if (((int)NHAttribute & ((int)MenuItemAttributes.Title)) == ((int)MenuItemAttributes.Title))
                {
                    res = 20.0;
                }
                else if (((int)NHAttribute & ((int)MenuItemAttributes.Bold | (int)MenuItemAttributes.Heading)) != 0)
                {
                    res = 18.0;
                }
                switch (_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_GENERAL:
                    case ghmenu_styles.GHMENU_STYLE_PREVIOUS_MESSAGES:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_FILE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_INVENTORY:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                        res = res * 17.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL_ALTERNATE:
                        res = res * 17.5 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_MONSTER_ABILITY:
                        res = res * 17.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PERMANENT_INVENTORY:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST:
                    case ghmenu_styles.GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_PICK_CATEGORY_LIST:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ITEM_COMMAND:
                    case ghmenu_styles.GHMENU_STYLE_GENERAL_COMMAND:
                    case ghmenu_styles.GHMENU_STYLE_SKILL_COMMAND:
                        res = res * 16.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_SPELL_COMMAND:
                        res = res * 17.5 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHAT_CHOOSE_ITEM:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SIMPLE:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_COMMAND:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        res = res * 15.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                        res = res * 15.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ACCEPT_PLAYER:
                        if(Identifier != 0)
                            res = res * 18.0 / 15.0;
                        else
                            res = res * 14.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                        res = res * 18.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        res = res * 20.0 / 15.0;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHARACTER:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_ATTRIBUTES:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_SPELLS:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL:
                    case ghmenu_styles.GHMENU_STYLE_SKILLS:
                        res = _gamePage == null ? 18 : Math.Min(18, Math.Max(11.75, 11.9 * _gamePage.CurrentPageWidth / 600));
                        break;
                    case ghmenu_styles.GHMENU_STYLE_DUNGEON_OVERVIEW:
                        break;
                    case ghmenu_styles.GHMENU_STYLE_OPTIONS:
                        res = _gamePage == null ? 18 : Math.Min(18, Math.Max(11.75, 14 * _gamePage.CurrentPageWidth / 600));
                        break;
                    case ghmenu_styles.GHMENU_STYLE_HELP:
                        break;
                    case ghmenu_styles.MAX_GHMENU_STYLES:
                        break;
                }

                if((NHAttribute & (int)MenuItemAttributes.HalfSize) != 0)
                {
                    res /= 2;
                }

                //if(res != Interlocked.CompareExchange(ref _fontSize, 0.0, 0.0))
                //{
                //    Interlocked.Exchange(ref _fontSize, res);
                //    //_fontSize = res;
                //}
                return res; 
            }
        }

        //private double _relativeSuffixFontSize = 0;
        public double RelativeSuffixFontSize
        {
            get
            {
                double res = 0.8;
                switch(_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_SKILLS_ALTERNATE:
                        res = 0.8;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_SPELLS_ALTERNATE:
                    case ghmenu_styles.GHMENU_STYLE_VIEW_SPELL_ALTERNATE:
                        res = 0.775;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        res = 0.675;
                        break;
                    default:
                        break;
                }
                //if (res != Interlocked.CompareExchange(ref _relativeSuffixFontSize, 0.0, 0.0))
                //{
                //    Interlocked.Exchange(ref _fontSize, res);
                //    //_relativeSuffixFontSize = res;
                //    //OnPropertyChanged("RelativeSuffixFontSize");
                //}
                return res;
            }
        }

        public double HeadingTopMargin
        {
            get
            {
                double res;
                switch (_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                        res = 18;
                        break;
                    default:
                        res = 12;
                        break;
                }
                return res;
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

        //public event PropertyChangedEventHandler PropertyChanged;

        //private void OnPropertyChanged(string propertyname)
        //{
        //    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyname));
        //}

        //public SKRect DrawBounds = new SKRect();
        //public SKRect EquipmentDrawBounds = new SKRect();

        //public bool IsVisibleOnCanvas(float canvaswidth, float canvasheight)
        //{
        //    if (DrawBounds.Bottom <= 0 || DrawBounds.Top >= canvasheight)
        //        return false;

        //    return true;
        //}

        public bool IsButton
        {
            get
            {
                return (IsButtonStyle
                    || _menuInfo.Style == ghmenu_styles.GHMENU_STYLE_START_GAME_MENU 
                    || _menuInfo.Style == ghmenu_styles.GHMENU_STYLE_ACCEPT_PLAYER 
                    || _menuInfo.Style == ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY) 
                    && Identifier != 0;
            }
        }

        public float BottomPadding
        {
            get
            {
                float bottomPadding = 0;
                if (!IsButton
                    && (((ulong)Flags & (ulong)MenuFlags.MENU_FLAGS_IS_HEADING) != 0 
                    || (NHAttribute & ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Heading)) == (int)MenuItemAttributes.Heading
                    || (NHAttribute & ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title)) == (int)MenuItemAttributes.Title))
                {
                    bottomPadding = 3;
                }
                else
                {
                    switch (_menuInfo.Style)
                    {
                        case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                            bottomPadding = 3;
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        default:
                            if (IsSuffixTextVisible && IsSuffix2TextVisible)
                                bottomPadding = 6;
                            else
                                bottomPadding = 3;
                            break;
                    }
                }
                return bottomPadding;
            }
        }

        public float TopPadding
        {
            get
            {
                float topPadding = 0;  
                if (!IsButton
                    && (((ulong)Flags & (ulong)MenuFlags.MENU_FLAGS_IS_HEADING) != 0
                    || (((NHAttribute & ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Heading)) == (int)MenuItemAttributes.Heading
                    || (NHAttribute & ((int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title)) == (int)MenuItemAttributes.Title) && Identifier == 0)))
                {
                    topPadding = (float) HeadingTopMargin;
                }
                else
                {
                    switch (_menuInfo.Style)
                    {
                        case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                            topPadding = 0;
                            break;
                        case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                        case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                        default:
                            if (IsSuffixTextVisible && IsSuffix2TextVisible)
                                topPadding = 6;
                            else
                                topPadding = 3;
                            break;
                    }
                }
                return topPadding;
            }
        }

        public bool UsesMinRowHeight
        {
            get
            {
                bool res = true;
                switch(_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_ACCEPT_PLAYER:
                    case ghmenu_styles.GHMENU_STYLE_DELETE_SAVED_GAME:
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_SAVED_GAME:
                        if (Identifier == 0 && HeadingGroupAccelerator == 0)
                            res = false;
                        break;
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_DIFFICULTY:
                    case ghmenu_styles.GHMENU_STYLE_START_GAME_MENU:
                    default:
                        break;
                }
                return res;
            }
        }

        public float MinimumRowHeight(float fontspacing, float bottompadding, float toppadding, float canvaswidth, float canvasheight)
        {
            float res = 0;

            if (UsesMinRowHeight)
            {
                res = fontspacing;
                switch (_menuInfo.Style)
                {
                    case ghmenu_styles.GHMENU_STYLE_CHOOSE_PLAYER:
                        float altsize = Math.Min((float)GHConstants.TileHeight, (Math.Min(canvaswidth, canvasheight) / 15.25f - bottompadding - toppadding));
                        if (altsize > fontspacing)
                            res = altsize;
                        break;
                    default:
                        break;
                }
            }
            return res;
        }
    }
}

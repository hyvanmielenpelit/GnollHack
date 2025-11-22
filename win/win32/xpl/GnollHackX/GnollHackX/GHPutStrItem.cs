#if GNH_MAUI
using GnollHackM;
#else
using Xamarin.Forms;
using GnollHackX.Pages.Game;
#endif
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GnollHackX
{
    public sealed class GHPutStrItem
    {
        private GHWindow _window;
        private string _text;

        public int PaddingAmount { get; set; }

        public GHWindow Window { get { return _window; } }

        public string Text { 
            get 
            { 
                return _text;
            } 
            set 
            { 
                _text = value; 
            }
        }
        private List<GHPutStrInstructions> _instructionList;

        public List<GHPutStrInstructions> InstructionList { get { return _instructionList; } }

        private List<int> _attributeList;
        private List<int> _colorList;
        public List<int> AttributeList { get { return _attributeList; } }
        public List<int> ColorList { get { return _colorList; } }

        public GHPutStrItem(GHWindow window, string str)
        {
            _instructionList = new List<GHPutStrInstructions>();
            _attributeList = new List<int>();
            _colorList = new List<int>();
            _text = str;
            _window = window;
        }

        public void ConvertListFromArrays()
        {
            int prevattr = 0, prevclr = 0;
            int curattr = 0, curclr = 0;
            int cnt = 0;

            _instructionList.Clear();
            for (int i = 0; i < Text.Length; i++)
            {
                prevattr = curattr;
                prevclr = curclr;
                cnt++;
                curattr = i < _attributeList.Count ? _attributeList[i] : 0;
                curclr = i < _colorList.Count ? _colorList[i] : (int)NhColor.CLR_WHITE;

                if(cnt > 1 && (curattr != prevattr || curclr != prevclr))
                {
                    _instructionList.Add(new GHPutStrInstructions(prevattr, prevclr, cnt - 1));
                    cnt = 1;
                }
                
                if(i == Text.Length - 1)
                {
                    _instructionList.Add(new GHPutStrInstructions(curattr, curclr, cnt));
                }
            }
        }

        public void ConvertListToArrays()
        {
            _attributeList.Clear();
            _colorList.Clear();
            for(int j = 0, m = _instructionList.Count; j < m; j++)
            {
                GHPutStrInstructions instr = _instructionList[j];
                for (int i = 0, n = instr.PrintLength; i < n; i++)
                {
                    _attributeList.Add(instr.Attributes);
                    _colorList.Add(instr.Color);
                }
            }
        }

        public GHPutStrItem Clone(GHWindow clonedWindow)
        {
            GHPutStrItem clone = new GHPutStrItem(clonedWindow, _text);
            clone.AttributeList.AddRange(_attributeList);
            clone.ColorList.AddRange(_colorList);
            clone.InstructionList.AddRange(_instructionList);
            return clone;
        }

        public string TextWindowFontFamily
        {
            get
            {
                if (_instructionList != null && _instructionList.Count > 0)
                {
                    if ((_instructionList[0].Attributes & ((int)MenuItemAttributes.Bold | (int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title | (int)MenuItemAttributes.Heading)) != 0)
                        return "Immortal";
                }

                if (_window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_GENERAL || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_KEEP_LINE_BREAKS
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER)
                    return "Immortal";

                return "Underwood";
            }
        }

        public double TextWindowFontSize
        {
            get
            {
                if (_window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_GENERAL || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_KEEP_LINE_BREAKS
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER)
                {
                    GamePage gamePage = GHApp.CurrentGamePage;
                    double basesize = gamePage == null ? 22 : Math.Min(22, Math.Min(15.5 * gamePage.CurrentPageWidth / 300, Math.Max(15.5, 15.5 * gamePage.CurrentPageWidth * gamePage.CurrentPageHeight / (600 * 360))));
                    if (_instructionList != null && _instructionList.Count > 0)
                    {
                        if ((_instructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title))
                            return basesize / 1.1;
                        if ((_instructionList[0].Attributes & (int)MenuItemAttributes.Title) == (int)MenuItemAttributes.Title)
                            return basesize * 1.2;
                    }
                    return basesize;
                }

                if (_instructionList != null && _instructionList.Count > 0)
                {
                    if ((_instructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title))
                        return 18;
                    if ((_instructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading))
                        return 16;
                    if ((_instructionList[0].Attributes & (int)MenuItemAttributes.Title) == (int)MenuItemAttributes.Title)
                        return 21;
                    if ((_instructionList[0].Attributes & (int)MenuItemAttributes.Heading) == (int)MenuItemAttributes.Heading)
                        return 17.5;
                }

                if (_window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_OBJECT_DESCRIPTION_SCREEN
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_MONSTER_DESCRIPTION_SCREEN
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_CHARACTER_SCREEN
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_ENLIGHTENMENT_SCREEN)
                        return 14.5;


                return 15.5;
            }
        }

        public double LeftPaddingWidth
        {
            get
            {
                return (double)(PaddingAmount * 5);
            }
        }

        public double RightPaddingWidth
        {
            get
            {
                return (double)(PaddingAmount * 5);
            }
        }

        public bool LeftVisible
        {
            get
            {
                return (PaddingAmount > 0);
            }
        }

        public bool RightVisible
        {
            get
            {
                return (PaddingAmount > 0);
            }
        }

        public Thickness TextMargin
        {
            get
            {
                Thickness res = new Thickness(0);
                if (_instructionList != null && _instructionList.Count > 0)
                {
                    if ((_instructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title))
                        res.Top = 0;
                    else if ((_instructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading))
                        res.Top = 10;
                    else if ((_instructionList[0].Attributes & (int)MenuItemAttributes.Title) == (int)MenuItemAttributes.Title)
                        res.Top = 0;
                    else if ((_instructionList[0].Attributes & (int)MenuItemAttributes.Heading) == (int)MenuItemAttributes.Heading)
                        res.Top = 8;
                }

                return res;
            }
        }

        //public string GetIndentationString()
        //{
        //    if (string.IsNullOrEmpty(Text))
        //        return "";

        //    if (_instructionList != null && _instructionList.Count > 0)
        //    {
        //        return GHUtils.GetIndentationString(Text, _instructionList[0].Attributes);
        //    }
        //    return "";
        //}

        public void GetIndentationSpan(out ReadOnlySpan<char> outSpan)
        {
            if (string.IsNullOrEmpty(Text))
            {
                outSpan = ReadOnlySpan<char>.Empty;
                return;
            }

            if (_instructionList != null && _instructionList.Count > 0)
            {
                GHUtils.GetIndentationSpan(Text, _instructionList[0].Attributes, out outSpan);
                return;
            }
            outSpan = ReadOnlySpan<char>.Empty;
        }

        public int StartingSpaces
        {
            get 
            {
                string txt = Text;
                if (string.IsNullOrEmpty(txt)) 
                    return 0; 
                else 
                {
                    int cnt = 0;
                    for (int i = 0, len = txt.Length; i < len; i++)
                    {
                        if (txt[i] == ' ')
                            cnt++;
                        else
                            break;
                    }
                    return cnt;
                } 
            }
        }
    }
}

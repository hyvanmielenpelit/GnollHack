using GnollHackClient.Pages.Game;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xamarin.Forms;

namespace GnollHackClient
{
    public class GHPutStrItem
    {
        private GamePage _gamePage;
        private GHWindow _window;
        private string _text;

        public int PaddingAmount { get; set; }

        public GamePage ReferenceGamePage { get { return _gamePage; } }
        public GHWindow Window { get { return _window; } }

        public string Text { get { return _text; } 
            set 
            { 
                _text = value; 
                //if (_instructionList.Count < _text.Length)
                //{
                //    _instructionList.Add(new GHPutStrInstructions(0, (int)nhcolor.CLR_WHITE, _text.Length - _instructionList.Count));
                //}
            }
        }
        private List<GHPutStrInstructions> _instructionList;
        public List<GHPutStrInstructions> InstructionList { get { return _instructionList; } set { _instructionList = value; } }

        public GHPutStrItem(GamePage gamePage, GHWindow window, string str)
        {
            _instructionList = new List<GHPutStrInstructions>();
            Text = str;
            _gamePage = gamePage;
            _window = window;
        }
        public GHPutStrItem(GamePage gamePage, GHWindow window, string str, List<GHPutStrInstructions> instrlist)
        {
            InstructionList = instrlist;
            Text = str;
            _gamePage = gamePage;
            _window = window;
        }
        public GHPutStrItem(Pages.Game.GamePage gamePage, GHWindow window, string str, List<int> attrlist, List<int> colorlist)
        {
            InstructionList = ConvertListFromArrays(attrlist, colorlist);
            Text = str;
            _gamePage = gamePage;
            _window = window;
        }
        public List<GHPutStrInstructions> ConvertListFromArrays(List<int> attrlist, List<int> colorlist)
        {
            List <GHPutStrInstructions> res = new List<GHPutStrInstructions>();
            int prevattr = 0, prevclr = 0;
            int curattr = 0, curclr = 0;
            int cnt = 0;

            for (int i = 0; i < Text.Length; i++)
            {
                prevattr = curattr;
                prevclr = curclr;
                cnt++;
                curattr = i < attrlist.Count ? attrlist[i] : 0;
                curclr = i < colorlist.Count ? colorlist[i] : (int)nhcolor.CLR_WHITE;

                if(cnt > 1 && (curattr != prevattr || curclr != prevclr))
                {
                    res.Add(new GHPutStrInstructions(prevattr, prevclr, cnt - 1));
                    cnt = 1;
                }
                
                if(i == Text.Length - 1)
                {
                    res.Add(new GHPutStrInstructions(curattr, curclr, cnt));
                }
            }
            return res;
        }

        public void ConvertListToArrays(List<GHPutStrInstructions> instrlist, out List<int> attrlist, out List<int> colorlist)
        {
            attrlist = new List<int>();
            colorlist = new List<int>();
            foreach(GHPutStrInstructions instr in instrlist)
            {
                for(int i = 0; i < instr.PrintLength; i++)
                {
                    attrlist.Add(instr.Attributes);
                    colorlist.Add(instr.Color);
                }
            }
        }
        public void ConvertCurrentListToArrays(out List<int> attrlist, out List<int> colorlist)
        {
            ConvertListToArrays(InstructionList, out attrlist, out colorlist);
        }
        public void ConvertCurrentListFromArrays(List<int> attrlist, List<int> colorlist)
        {
            InstructionList = ConvertListFromArrays(attrlist, colorlist);
        }

        public string TextWindowFontFamily
        {
            get
            {
                if (InstructionList != null && InstructionList.Count > 0)
                {
                    if ((InstructionList[0].Attributes & ((int)MenuItemAttributes.Bold | (int)MenuItemAttributes.Sub | (int)MenuItemAttributes.Title | (int)MenuItemAttributes.Heading)) != 0)
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
                    double basesize = Math.Min(22, Math.Min(15.5 * _gamePage.CurrentPageWidth / 300, Math.Max(15.5, 15.5 * _gamePage.CurrentPageWidth * _gamePage.CurrentPageHeight / (600 * 360))));
                    if (InstructionList != null && InstructionList.Count > 0)
                    {
                        if ((InstructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title))
                            return basesize / 1.1;
                        if ((InstructionList[0].Attributes & (int)MenuItemAttributes.Title) == (int)MenuItemAttributes.Title)
                            return basesize * 1.2;
                    }
                    return basesize;
                }

                if (InstructionList != null && InstructionList.Count > 0)
                {
                    if ((InstructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title))
                        return 18;
                    if ((InstructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading))
                        return 16;
                    if ((InstructionList[0].Attributes & (int)MenuItemAttributes.Title) == (int)MenuItemAttributes.Title)
                        return 21;
                    if ((InstructionList[0].Attributes & (int)MenuItemAttributes.Heading) == (int)MenuItemAttributes.Heading)
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
                if (InstructionList != null && InstructionList.Count > 0)
                {
                    if ((InstructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Title))
                        res.Top = 0;
                    else if ((InstructionList[0].Attributes & (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading)) == (((int)MenuItemAttributes.Sub) | (int)MenuItemAttributes.Heading))
                        res.Top = 10;
                    else if ((InstructionList[0].Attributes & (int)MenuItemAttributes.Title) == (int)MenuItemAttributes.Title)
                        res.Top = 0;
                    else if ((InstructionList[0].Attributes & (int)MenuItemAttributes.Heading) == (int)MenuItemAttributes.Heading)
                        res.Top = 8;
                }

                return res;
            }
        }

        public string GetIndentationString()
        {
            if (Text == null || Text == "")
                return "";

            if (InstructionList != null && InstructionList.Count > 0)
            {
                return GHUtils.GetIndentationString(Text, InstructionList[0].Attributes);
            }
            return "";
        }

        public int StartingSpaces
        {
            get {
                if (Text == null || Text == "") 
                    return 0; 
                else 
                {
                    int cnt = 0;
                    for (int i = 0; i < Text.Length; i++)
                    {
                        if (Text[i] == ' ')
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

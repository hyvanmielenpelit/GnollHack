using GnollHackClient.Pages.Game;
using GnollHackCommon;
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
                if (_instructionList.Count < _text.Length)
                {
                    _instructionList.Add(new GHPutStrInstructions(0, (int)nhcolor.CLR_WHITE, _text.Length - _instructionList.Count));
                }
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
                    if (InstructionList[0].Attributes == (int)MenuItemAttributes.Bold)
                        return "Immortal";
                    if (InstructionList[0].Attributes == (int)MenuItemAttributes.Dim)
                        return "Immortal";
                }

                if (_window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_GENERAL
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER)
                    return "Immortal";

                return "Underwood";
            }
        }

        public double TextWindowFontSize
        {
            get
            {
                if (_window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_GENERAL
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER)
                {
                    double basesize = Math.Min(22, Math.Max(17, 17 * _gamePage.CurrentPageWidth * _gamePage.CurrentPageHeight / (600 * 360)));
                    if (InstructionList != null && InstructionList.Count > 0)
                    {
                        if (InstructionList[0].Attributes == (int)MenuItemAttributes.Bold)
                            return basesize * 1.2;
                        if (InstructionList[0].Attributes == (int)MenuItemAttributes.Dim)
                            return basesize / 1.2;
                    }
                    return basesize;
                }

                if (InstructionList != null && InstructionList.Count > 0)
                {
                    if (InstructionList[0].Attributes == (int)MenuItemAttributes.Bold)
                        return 21;
                    if (InstructionList[0].Attributes == (int)MenuItemAttributes.Dim)
                        return 16;
                }

                if (_window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_OBJECT_DESCRIPTION_SCREEN
                    || _window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_MONSTER_DESCRIPTION_SCREEN)
                        return 15;


                return 15.5;
            }
        }
        public Color TextWindowTextColor
        {
            get
            {
                if (InstructionList != null && InstructionList.Count > 0 && InstructionList[0].Color != (int)nhcolor.NO_COLOR)
                {
                    return ClientUtils.NHColor2XColor((nhcolor)InstructionList[0].Color);
                }
                return Color.White;
            }
        }

        public double LeftPaddingWidth
        {
            get
            {
                return (double)(PaddingAmount * 5);
            }
        }

        public double RightPaddingText
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
    }
}

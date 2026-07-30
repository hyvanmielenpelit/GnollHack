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
    /* GHPublishedWindowRow is part of the published (read-only) window snapshot.
     * However, Text, PaddingAmount, and InstructionList are left mutable because
     * UIUtils.ProcessAdjustedItems creates NEW GHPublishedWindowRow instances and
     * builds them up incrementally (appending text, setting padding, adding
     * instructions). The mutability applies only to freshly-created rows during
     * that process, not to rows already published to the UI. */
    public sealed class GHPublishedWindowRow
    {
        private GHPublishedWindow _window;
        private string _text;

        public int PaddingAmount { get; set; }

        public GHPublishedWindow Window { get { return _window; } }

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

        public GHPublishedWindowRow(GHPublishedWindow window, string str, List<GHPutStrInstructions> instructions = null)
        {
            _instructionList = instructions ?? new List<GHPutStrInstructions>();
            _text = str;
            _window = window;
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

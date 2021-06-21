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
        public string Text { get; set; }
        public int Attributes { get; set; }
        public nhcolor NHColor { get; set; }
        public Color Color 
        { get 
            { 
                if(IsLast)
                    return Pages.Game.GamePage.NHColor2Color(NHColor);
                else
                    return Pages.Game.GamePage.NHColor2Color(NHColor); //NHColor2GrayedColor(NHColor); 
            } 
        }
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
            Attributes = attr;
            NHColor = (nhcolor)color;
            IsLast = false;
        }
    }
}

using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackClient
{
    public class GHButton : Button
    {
        private int _ghcommand;
        
        public int GHCommand 
        { 
            get 
            { 
                if(ApplyMeta) 
                    return GHUtils.Meta(_ghcommand);
                if (ApplyCtrl)
                    return GHUtils.Ctrl(_ghcommand);
                else
                    return _ghcommand;
            }
        }
        public char Letter { get { return (char)_ghcommand; } set { _ghcommand = (int)value; } }
        public bool ApplyMeta { get; set; }
        public bool ApplyCtrl { get; set; }

        public GHButton()
        {

        }

        public GHButton(int ghcommand)
        {
            _ghcommand = ghcommand;
        }
    }
}

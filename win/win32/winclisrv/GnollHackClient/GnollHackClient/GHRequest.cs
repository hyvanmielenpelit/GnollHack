using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public enum GHRequestType
    {
        None,
        GetChar,
        AskName,
        ReturnToMainMenu,
        ShowMenuPage,
    }

    public class GHRequest
    {
        public ClientGame RequestingClientGame { get; }
        public GHRequestType RequestType { get; set; }
        public GHWindow RequestingGHWindow { get; set; }
        public GHMenuInfo RequestMenuInfo { get; set; }
        public GHRequest()
        {

        }

        public GHRequest(ClientGame clientgame, GHRequestType requesttype)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, GHWindow requestingGHWindow, GHMenuInfo menuinfo)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestingGHWindow = requestingGHWindow;
            RequestMenuInfo = menuinfo;
        }
    }
}

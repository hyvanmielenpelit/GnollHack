using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public enum GHRequestType
    {
        None,
        GetChar,
        GetLine,
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
        public string RequestString { get; set; }
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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
        }
    }
}

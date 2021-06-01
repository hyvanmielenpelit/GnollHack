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
        PrintTopLine,
        PrintHistory,
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
        public uint RequestStringAttributes { get; set; }
        public int RequestInt { get; set; }
        public List<GHMsgHistoryItem> MessageHistory { get; set; }
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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring, uint attributes)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestStringAttributes = attributes;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, List<GHMsgHistoryItem> msgHistory)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            MessageHistory = msgHistory;
        }
    }
}

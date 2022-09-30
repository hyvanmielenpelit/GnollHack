using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHResponse
    {
        public GHRequestType RequestType { get; set; }
        public string ResponseStringValue { get; set; }
        public int ResponseIntValue { get; set; }
        public uint ResponseUIntValue { get; set; }
        public bool ResponseBoolValue { get; set; }
        public List<GHMenuItem> SelectedMenuItems { get; set; }
        public ClientGame RequestingClientGame { get; }
        public GHWindow RequestingGHWindow { get; }
        public int X { get; set; }
        public int Y { get; set; }
        public int Mod { get; set; }
        public GHResponse()
        {

        }

        public GHResponse(ClientGame clientgame, GHRequestType requesttype)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
        }

        public GHResponse(ClientGame clientgame, GHRequestType requesttype, string responsestringvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseStringValue = responsestringvalue;
        }
        public GHResponse(ClientGame clientgame, GHRequestType requesttype, int responseintvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseIntValue = responseintvalue;
        }
        public GHResponse(ClientGame clientgame, GHRequestType requesttype, uint responseuintvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseUIntValue = responseuintvalue;
        }
        public GHResponse(ClientGame clientgame, GHRequestType requesttype, GHWindow window, List<GHMenuItem> responselist, bool wasCancelled)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestingGHWindow = window;
            SelectedMenuItems = responselist;
            ResponseBoolValue = wasCancelled;
        }
        public GHResponse(ClientGame clientgame, GHRequestType requesttype, int x, int y, int mod)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            X = x;
            Y = y;
            Mod = mod;
        }
    }
}

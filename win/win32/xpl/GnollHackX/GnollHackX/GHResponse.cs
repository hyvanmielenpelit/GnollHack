using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHResponse
    {
        public GHRequestType RequestType { get; set; }
        public string ResponseStringValue { get; set; }
        public int ResponseIntValue { get; set; }
        public uint ResponseUIntValue { get; set; }
        public bool ResponseBoolValue { get; set; }
        public List<GHMenuItem> SelectedMenuItems { get; set; }
        public GHGame RequestingClientGame { get; }
        public GHWindow RequestingGHWindow { get; }
        public int X { get; set; }
        public int Y { get; set; }
        public int Mod { get; set; }
        public GHResponse()
        {

        }

        public GHResponse(GHGame clientgame, GHRequestType requesttype)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
        }

        public GHResponse(GHGame clientgame, GHRequestType requesttype, string responsestringvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseStringValue = responsestringvalue;
        }
        public GHResponse(GHGame clientgame, GHRequestType requesttype, int responseintvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseIntValue = responseintvalue;
        }
        public GHResponse(GHGame clientgame, GHRequestType requesttype, uint responseuintvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseUIntValue = responseuintvalue;
        }
        public GHResponse(GHGame clientgame, GHRequestType requesttype, bool responseboolvalue)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ResponseBoolValue = responseboolvalue;
        }
        public GHResponse(GHGame clientgame, GHRequestType requesttype, GHWindow window, List<GHMenuItem> responselist, bool wasCancelled)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestingGHWindow = window;
            SelectedMenuItems = responselist;
            ResponseBoolValue = wasCancelled;
        }
        public GHResponse(GHGame clientgame, GHRequestType requesttype, int x, int y, int mod)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            X = x;
            Y = y;
            Mod = mod;
        }
    }
}

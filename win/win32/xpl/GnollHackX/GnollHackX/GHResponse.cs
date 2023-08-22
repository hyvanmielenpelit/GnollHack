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
        public GHGame RequestingGame { get; }
        public GHWindow RequestingGHWindow { get; }
        public int X { get; set; }
        public int Y { get; set; }
        public int Mod { get; set; }
        public GHResponse()
        {

        }

        public GHResponse(GHGame ghGame, GHRequestType requesttype)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
        }

        public GHResponse(GHGame ghGame, GHRequestType requesttype, string responsestringvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseStringValue = responsestringvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, int responseintvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseIntValue = responseintvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, uint responseuintvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseUIntValue = responseuintvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, bool responseboolvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseBoolValue = responseboolvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, GHWindow window, List<GHMenuItem> responselist, bool wasCancelled)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestingGHWindow = window;
            SelectedMenuItems = responselist;
            ResponseBoolValue = wasCancelled;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, int x, int y, int mod)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            X = x;
            Y = y;
            Mod = mod;
        }
    }
}

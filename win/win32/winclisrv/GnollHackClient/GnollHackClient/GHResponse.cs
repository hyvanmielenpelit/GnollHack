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
        public List<GHMenuItem> SelectedMenuItems { get; set; }
        public ClientGame RequestingClientGame { get; }
        public GHWindow RequestingGHWindow { get; }

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
        public GHResponse(ClientGame clientgame, GHRequestType requesttype, GHWindow window, List<GHMenuItem> responselist)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestingGHWindow = window;
            SelectedMenuItems = responselist;
        }
    }
}

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
        public ClientGame RequestingClientGame { get; }

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
    }
}

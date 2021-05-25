using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public enum GHRequestType
    {
        None,
        AskName
    }

    public class GHRequest
    {
        public ClientGame RequestingClientGame { get; }
        public GHRequestType RequestType { get; set; }
        public GHRequest()
        {

        }

        public GHRequest(ClientGame clientgame, GHRequestType requesttype)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
        }
    }
}

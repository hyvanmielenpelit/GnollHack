using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public enum GHRequestType
    {
        None,
        GetChar,
        Location,
        PosKey,
        GetLine,
        PrintTopLine,
        PrintHistory,
        AskName,
        ReturnToMainMenu,
        ShowMenuPage,
        ShowYnResponses,
        HideYnResponses,
        ShowDirections,
        HideDirections,
        CreateWindowView,
        ClearWindowView,
        DestroyWindowView,
        DisplayWindowView,
        HideLoadingScreen,
        ClearContextMenu,
        AddContextMenu,
        DisplayFloatingText,
        DisplayScreenText,
        DisplayPopupText,
        ShowSkillButton,
        HideSkillButton,
        ShowOutRipPage,
    }

    public struct AddContextMenuData
    {
        public int cmd_def_char;
        public int cmd_cur_char;
        public int dir;
        public int glyph;
        public string cmd_text;
        public string target_text;
        public int attr;
        public int color;
    }

    public struct DisplayFloatingTextData
    {
        public int x;
        public int y;
        public string text;
        public int style;
        public int attr;
        public int color;
        public ulong tflags;
    }

    public struct DisplayScreenTextData
    {
        public string text;
        public string subtext;
        public int style;
        public int attr;
        public int color;
        public ulong tflags;
    }

    public class GHRequest
    {
        public ClientGame RequestingClientGame { get; }
        public GHRequestType RequestType { get; set; }
        public GHWindow RequestingGHWindow { get; set; }
        public GHMenuInfo RequestMenuInfo { get; set; }
        public GHOutRipInfo RequestOutRipInfo { get; set; }
        public string RequestString { get; set; }
        public uint RequestStringAttributes { get; set; }
        public int RequestInt { get; set; }
        public List<GHMsgHistoryItem> MessageHistory { get; set; }
        public string Responses { get; set; }
        public List<GHPutStrItem> RequestPutStrItems { get; set; }

        public AddContextMenuData ContextMenuData { get; set; }
        public DisplayFloatingTextData FloatingTextData { get; set; }
        public DisplayScreenTextData ScreenTextData { get; set; }

        public GHRequest()
        {

        }

        public GHRequest(ClientGame clientgame, GHRequestType requesttype)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, int requestint)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestInt = requestint;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, int requestint, List<GHPutStrItem> strs)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestPutStrItems = strs;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, GHWindow requestingGHWindow, GHMenuInfo menuinfo)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestingGHWindow = requestingGHWindow;
            RequestMenuInfo = menuinfo;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, GHWindow requestingGHWindow, GHOutRipInfo outripinfo)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestingGHWindow = requestingGHWindow;
            RequestOutRipInfo = outripinfo;
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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring, string responses)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
            Responses = responses;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, AddContextMenuData data)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ContextMenuData = data;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, DisplayFloatingTextData data)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            FloatingTextData = data;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, DisplayScreenTextData data)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ScreenTextData = data;
        }
    }
}

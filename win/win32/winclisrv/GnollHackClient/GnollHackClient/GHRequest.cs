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
        HideMenuPage,
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
        HidePopupText,
        DisplayGUIEffect,
        ShowSkillButton,
        HideSkillButton,
        ShowOutRipPage,
        FadeToBlack,
        FadeFromBlack,
        ShowGUITips,
        CrashReport,
        Panic,
        Message,
        DisplayConditionText,
        DisplayScreenFilter,
        SetPetMID,
        SaveAndDisableTravelMode,
        RestoreTravelMode,
    }

    public struct AddContextMenuData
    {
        public int cmd_def_char;
        public int cmd_cur_char;
        public int style;
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

    public struct DisplayGUIEffectData
    {
        public int x;
        public int y;
        public int style;
        public ulong tflags;
    }

    public struct DisplayScreenTextData
    {
        public string text;
        public string supertext;
        public string subtext;
        public int style;
        public int attr;
        public int color;
        public int glyph;
        public ulong tflags;
    }

    public struct DisplayConditionTextData
    {
        public string text;
        public int style;
        public int attr;
        public int color;
        public ulong tflags;
    }

    public struct DisplayScreenFilterData
    {
        public int style;
        public int attr;
        public int color;
        public ulong fflags;
    }

    public class GHRequest
    {
        public ClientGame RequestingClientGame { get; }
        public GHRequestType RequestType { get; set; }
        public GHWindow RequestingGHWindow { get; set; }
        public GHMenuInfo RequestMenuInfo { get; set; }
        public GHOutRipInfo RequestOutRipInfo { get; set; }
        public string RequestString { get; set; }
        public string RequestString2 { get; set; }
        public string PlaceHolderString { get; set; }
        public string DefValueString { get; set; }
        public string TitleString { get; set; }
        public uint RequestStringAttributes { get; set; }
        public int RequestInt { get; set; }
        public int RequestInt2 { get; set; }
        public int RequestAttr { get; set; }
        public int RequestNhColor { get; set; }
        public int RequestGlyph { get; set; }
        public ulong RequestFlags { get; set; }
        public List<GHMsgHistoryItem> MessageHistory { get; set; }
        public string Responses { get; set; }
        public string ResponseDescriptions { get; set; }
        public List<GHPutStrItem> RequestPutStrItems { get; set; }

        public AddContextMenuData ContextMenuData { get; set; }
        public DisplayFloatingTextData FloatingTextData { get; set; }
        public DisplayScreenTextData ScreenTextData { get; set; }
        public DisplayConditionTextData ConditionTextData { get; set; }
        public DisplayScreenFilterData ScreenFilterData { get; set; }
        public DisplayGUIEffectData GUIEffectData { get; set; }

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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, int requestint, int requestint2)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestInt2 = requestint2;
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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring, string requeststring2)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestString2 = requeststring2;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring, int requestint, int requestint2)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestInt = requestint;
            RequestInt2 = requestint2;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring, int style, int attr, int color)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestInt = style;
            RequestAttr = attr;
            RequestNhColor = color;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, string requeststring, string placeholderstring, string defvaluestring, int style, int attr, int color)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            RequestString = requeststring;
            PlaceHolderString = placeholderstring;
            DefValueString = defvaluestring;
            RequestInt = style;
            RequestAttr = attr;
            RequestNhColor = color;
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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, int style, int attr, int color, int glyph, string titlestring, string requeststring, string responses, string descriptions, ulong ynflags)
        {
            RequestingClientGame = clientgame;
            RequestInt = style;
            RequestAttr = attr;
            RequestNhColor = color;
            RequestGlyph = glyph;
            TitleString = titlestring;
            RequestType = requesttype;
            RequestString = requeststring;
            Responses = responses;
            ResponseDescriptions = descriptions;
            RequestFlags = ynflags;
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
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, DisplayConditionTextData data)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ConditionTextData = data;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, DisplayScreenFilterData data)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            ScreenFilterData = data;
        }
        public GHRequest(ClientGame clientgame, GHRequestType requesttype, DisplayGUIEffectData data)
        {
            RequestingClientGame = clientgame;
            RequestType = requesttype;
            GUIEffectData = data;
        }
    }
}

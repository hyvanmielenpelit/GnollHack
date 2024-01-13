using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
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
        SaveAndDisableTravelModeOnLevel,
        RestoreTravelModeOnLevel,
        SaveGameAndWaitForResume,
        StopWaitAndRestoreSavedGame,
        TallyRealTime,
        YnConfirmation,
        PostGameStatus,
        PostDiagnosticData,
        PostXlogEntry,
        PostBonesFile,
        RestartGame,
        DebugLog,
        CloseAllDialogs,
        UseLongerMessageHistory,
        SaveInsuranceCheckPoint,
        RedrawScreen,
        EndReplayFile,
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
        public int style;
        public int subtype;
        public int x;
        public int y;
        public int x2;
        public int y2;
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
        public GHGame RequestingGame { get; }
        public GHRequestType RequestType { get; set; }
        public GHWindow RequestingGHWindow { get; set; }
        public GHMenuInfo RequestMenuInfo { get; set; }
        public GHOutRipInfo RequestOutRipInfo { get; set; }
        public string RequestString { get; set; }
        public string RequestString2 { get; set; }
        public string PlaceHolderString { get; set; }
        public string DefValueString { get; set; }
        public string IntroLineString { get; set; }
        public string TitleString { get; set; }
        public uint RequestStringAttributes { get; set; }
        public bool RequestBool { get; set; }
        public int RequestInt { get; set; }
        public int RequestInt2 { get; set; }
        public int RequestAttr { get; set; }
        public int RequestNhColor { get; set; }
        public int RequestGlyph { get; set; }
        public ulong RequestFlags { get; set; }
        public GHMsgHistoryItem[] MessageHistory { get; set; } = null;
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

        public GHRequest(GHGame ghGame, GHRequestType requesttype)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, int requestint)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestInt = requestint;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, bool requestbool)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestBool = requestbool;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, int requestint, int requestint2)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestInt2 = requestint2;
        }

        public GHRequest(GHGame ghGame, GHRequestType requesttype, int requestint, string requeststring)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestString = requeststring;
        }

        public GHRequest(GHGame ghGame, GHRequestType requesttype, int requestint, int requestint2, string requeststring)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestInt2 = requestint2;
            RequestString = requeststring;
        }

        public GHRequest(GHGame ghGame, GHRequestType requesttype, int requestint, string requeststring, string requeststring2)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestString = requeststring;
            RequestString2 = requeststring2;
        }

        public GHRequest(GHGame ghGame, GHRequestType requesttype, int requestint, List<GHPutStrItem> strs)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestInt = requestint;
            RequestPutStrItems = strs;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, GHWindow requestingGHWindow, GHMenuInfo menuinfo)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestingGHWindow = requestingGHWindow;
            RequestMenuInfo = menuinfo;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, GHWindow requestingGHWindow, GHOutRipInfo outripinfo)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestingGHWindow = requestingGHWindow;
            RequestOutRipInfo = outripinfo;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, string requeststring)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestString = requeststring;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, string requeststring, string requeststring2)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestString2 = requeststring2;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, string titlestring, string requeststring, string acceptstring, string cancelstring)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            TitleString = titlestring;
            RequestString = requeststring;
            RequestString2 = acceptstring;
            DefValueString = cancelstring;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, string requeststring, int requestint, int requestint2)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestInt = requestint;
            RequestInt2 = requestint2;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, string requeststring, int style, int attr, int color)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestInt = style;
            RequestAttr = attr;
            RequestNhColor = color;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, string requeststring, string placeholderstring, string defvaluestring, string introlinestring, int style, int attr, int color)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestString = requeststring;
            PlaceHolderString = placeholderstring;
            DefValueString = defvaluestring;
            IntroLineString = introlinestring;
            RequestInt = style;
            RequestAttr = attr;
            RequestNhColor = color;
        }

        public GHRequest(GHGame ghGame, GHRequestType requesttype, string requeststring, uint attributes)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestString = requeststring;
            RequestStringAttributes = attributes;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, GHMsgHistoryItem[] msgHistory)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            MessageHistory = msgHistory;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, int style, int attr, int color, int glyph, string titlestring, string requeststring, string responses, string descriptions, string introline, ulong ynflags)
        {
            RequestingGame = ghGame;
            RequestInt = style;
            RequestAttr = attr;
            RequestNhColor = color;
            RequestGlyph = glyph;
            TitleString = titlestring;
            RequestType = requesttype;
            RequestString = requeststring;
            Responses = responses;
            ResponseDescriptions = descriptions;
            IntroLineString = introline;
            RequestFlags = ynflags;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, AddContextMenuData data)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ContextMenuData = data;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, DisplayFloatingTextData data)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            FloatingTextData = data;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, DisplayScreenTextData data)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ScreenTextData = data;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, DisplayConditionTextData data)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ConditionTextData = data;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, DisplayScreenFilterData data)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ScreenFilterData = data;
        }
        public GHRequest(GHGame ghGame, GHRequestType requesttype, DisplayGUIEffectData data)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            GUIEffectData = data;
        }
    }
}

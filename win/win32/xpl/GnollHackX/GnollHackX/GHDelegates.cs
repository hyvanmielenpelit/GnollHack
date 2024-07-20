using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    /* General */
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidConstCharCallback([MarshalAs(GHConstants.GHStringMarshallingType)] string value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntCallback(int value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanDoubleCallback(double value1);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanDoubleVoidPtrCallback(double value1, IntPtr value2);

    /* Specific */
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte CanSuspendYesCallback();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayFileCallback([MarshalAs(GHConstants.GHStringMarshallingType)] string value1, byte value2);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayWindowCallback(int value1, byte value2);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void CursCallback(int value1, int value2, int value3);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void ClipAroundCallback(int value1, int value2, byte value3);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void UpdatePositionBarCallback([MarshalAs(GHConstants.GHStringMarshallingType)] string value);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PutStrExColorCallback(int value1, [MarshalAs(GHConstants.GHStringMarshallingType)] string value2, int value3, int value4, int value5);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PutStrEx2ColorCallback(int value1, [MarshalAs(GHConstants.GHStringMarshallingType)] string value2, IntPtr value3, IntPtr value4, int value5, int value6, int value7);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int GetMsgHistoryCallback(IntPtr text_ptr, IntPtr attributes_ptr, IntPtr colors_ptr, byte value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PutMsgHistoryCallback([MarshalAs(GHConstants.GHStringMarshallingType)] string value1, IntPtr attributes_ptr, IntPtr colors_ptr, byte value2);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int CreateGHWindowCallback(int winid, int style, int glyph, byte dataflags, IntPtr objdata_ptr, IntPtr otypdata_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PrintGlyphCallback(int value1, int value2, int value3, int value4, int value5, int value6, int value7, uint value8, IntPtr layers_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void IssueGuiCommandCallback(int cmd_id, int cmd_param, int cmd_param2, [MarshalAs(GHConstants.GHStringMarshallingType)] string cmd_str);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void StartMenuCallback(int winid, int style);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddMenuCallback(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, int color, [MarshalAs(GHConstants.GHStringMarshallingType)]  string text, byte presel);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddExtendedMenuCallback(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, int color, [MarshalAs(GHConstants.GHStringMarshallingType)] string text, byte presel, int maxcount,
        UInt64 oid, UInt64 mid, char heading_groupaccel, char special_mark, ulong menuflags, byte dataflags, int style, IntPtr otmpdata_ptr, IntPtr otypdata_ptr, IntPtr attrs_ptr, IntPtr colors_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void EndMenuCallback(int value1, [MarshalAs(GHConstants.GHStringMarshallingType)] string value2, [MarshalAs(GHConstants.GHStringMarshallingType)] string value3);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void OutRipCallback(int winid, [MarshalAs(GHConstants.GHStringMarshallingType)] string plname, int points, [MarshalAs(GHConstants.GHStringMarshallingType)] string killer, [MarshalAs(GHConstants.GHStringMarshallingType)] string time);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int UIHasInputCallback();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayImmediateSoundCallback(
        int ghsound, 
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath, 
        int bankid, double eventVolume,
        double soundVolume,
        [MarshalAs(UnmanagedType.LPArray, SizeConst = GHConstants.MaxSoundParameters)][In] string[] parameterNames, 
        [MarshalAs(UnmanagedType.LPArray, SizeConst = GHConstants.MaxSoundParameters)][In] float[] parameterValues, 
        int arraysize, 
        int sound_type, 
        int play_group, 
        uint dialogue_mid,
        uint play_flags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayMusicCallback(
        int ghsound,
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayLevelAmbientCallback(
        int ghsound,
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayOccupationAmbientCallback(
        int ghsound,
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayEffectAmbientCallback(
        int ghsound,
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int SetEffectAmbientVolumeCallback(
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayEnvironmentAmbientCallback(
        int ghsound,
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int StopAllSoundsCallback(
        uint flags,
        uint dialogue_mid);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int AddAmbientSoundCallback(
        int ghsound,
        [MarshalAs(GHConstants.GHStringMarshallingType)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume, out UInt64 soundSourceId);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int DeleteAmbientSoundCallback(UInt64 soundSourceId);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int SetAmbientSoundVolumeCallback(UInt64 soundSourceId, double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte AdjustGeneralVolumesCallback(double value1, double value2, double value3, double value4, double value5, double value6);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int AskNameCallback([MarshalAs(GHConstants.GHStringMarshallingType)] string modeName, [MarshalAs(GHConstants.GHStringMarshallingType)] string modeDescription, IntPtr out_string_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(GHConstants.GHStringMarshallingType)]
    public delegate string CharVoidCallback();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int YnFunctionCallback(int style, int attr, int color, int glyph, [MarshalAs(GHConstants.GHStringMarshallingType)] string title, [MarshalAs(GHConstants.GHStringMarshallingType)] string query, [MarshalAs(GHConstants.GHStringMarshallingType)] string responses, [MarshalAs(GHConstants.GHStringMarshallingType)] string default_answer, [MarshalAs(GHConstants.GHStringMarshallingType)] string response_descriptions, [MarshalAs(GHConstants.GHStringMarshallingType)] string introline, ulong ynflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int GetLineCallback(int style, int attr, int color, [MarshalAs(GHConstants.GHStringMarshallingType)] string value1, [MarshalAs(GHConstants.GHStringMarshallingType)] string value2, [MarshalAs(GHConstants.GHStringMarshallingType)] string value3, [MarshalAs(GHConstants.GHStringMarshallingType)] string value4, IntPtr out_string_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntCharCharUintCallback([MarshalAs(GHConstants.GHStringMarshallingType)] string value1, [MarshalAs(GHConstants.GHStringMarshallingType)] string value2, uint value3);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntIntIntOutIntCallback(int value1, int value2, /* [Out, MarshalAs(UnmanagedType.LPArray)] out int[] */ out IntPtr value3, out int value4);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int NhPosKeyCallback(out int x, out int y, out int mod);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void StatusUpdateCallback(int value1, string value2, long value3, int value4, int value5, int value6, IntPtr value7);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidUlongCallback(ulong value);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void StatusEnableFieldCallback(int value1, string value2, string value3, byte value4);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void FreeMemoryCallback(ref IntPtr ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void ReportPlayerNameCallback(string used_player_name);
 
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void ReportPlayTimeCallback(long timePassed, long currentPlayTime);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddContextMenuCallback(int value1, int value2, int value3, int value4, string value5, string value6, int value7, int value8);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void UpdateStatusButtonCallback(int value1, int value2, int value3, ulong value4);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void ToggleAnimationTimerCallback(int value1, int value2, int value3, int value4, int value5, int value6, ulong value7);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayFloatingTextCallback(int x, int y, string text, int style, int attr, int color, ulong tflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayScreenTextCallback(string text, string supertext, string subtext, int style, int attr, int color, ulong tflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayPopupTextCallback(string text, string title, int style, int attr, int color, int glyph, ulong tflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayGUIEffectCallback(int style, int subtype, int x, int y, int x2, int y2, ulong tflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void UpdateCursorCallback(int style, int force_paint, int show_on_u);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int OpenSpecialViewCallback(int viewtype, [MarshalAs(GHConstants.GHStringMarshallingType)] string text, [MarshalAs(GHConstants.GHStringMarshallingType)] string title, int attr, int color);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void SendObjectDataCallback(int x, int y, IntPtr otmp_ptr, int cmdtype, int where, IntPtr otypdata_ptr, ulong oflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void SendMonsterDataCallback(int cmdtype, int x, int y, IntPtr monster_data_ptr, ulong oflags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void SendEngravingDataCallback(int cmdtype, int x, int y, string engraving_text, int etype, ulong eflags, ulong gflags);
}

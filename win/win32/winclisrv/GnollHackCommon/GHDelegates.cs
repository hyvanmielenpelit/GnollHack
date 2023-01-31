using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace GnollHackCommon
{
    /* General */
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidCharCallback([MarshalAs(UnmanagedType.LPStr)] string value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidConstCharCallback([MarshalAs(UnmanagedType.LPStr)] string value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntIntCallback(int value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntCallback(int value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntBooleanCallback(int value1, int value2, byte value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntIntCallback(int value1, int value2, int value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntBooleanCallback(int value1, byte value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntConstCharCallback(int value1, int value2, [MarshalAs(UnmanagedType.LPStr)] string value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PutStrExColorCallback(int value1, int value2, [MarshalAs(UnmanagedType.LPStr)] string value3, int value4, int value5);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PutStrEx2ColorCallback(int value1, [MarshalAs(UnmanagedType.LPStr)] string value2, IntPtr value3, IntPtr value4, int value5, int value6, int value7);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidConstCharIntCallback([MarshalAs(UnmanagedType.LPStr)] string value1, int value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidConstCharBooleanCallback([MarshalAs(UnmanagedType.LPStr)] string value1, byte value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanIntDoubleCallback(int value1, double value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanDoubleCallback(double value1);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanIntDoubleVoidPtrCallback(int value1, double value2, IntPtr value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanDoubleVoidPtrCallback(double value1, IntPtr value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanVoidPtrCallback(IntPtr value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string GetMsgHistoryCallback(IntPtr attributes_ptr, IntPtr colors_ptr, byte value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PutMsgHistoryCallback([MarshalAs(UnmanagedType.LPStr)] string value1, IntPtr attributes_ptr, IntPtr colors_ptr, byte value2);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int CreateGHWindowCallback(int winid, int style, int glyph, byte dataflags, IntPtr objdata_ptr, IntPtr otypdata_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PrintGlyphCallback(int value1, int value2, int value3, int value4, int value5, int value6, int value7, uint value8, IntPtr layers_ptr);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void StartMenuCallback(int winid, int style);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddMenuCallback(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, [MarshalAs(UnmanagedType.LPStr)]  string text, byte presel, int color);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddExtendedMenuCallback(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, [MarshalAs(UnmanagedType.LPStr)] string text, byte presel, int color, int maxcount,
        UInt64 oid, UInt64 mid, char heading_groupaccel, char special_mark, ulong menuflags, byte dataflags, int style, IntPtr otmpdata_ptr, IntPtr otypdata_ptr);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void EndMenuCallback(int value1, [MarshalAs(UnmanagedType.LPStr)] string value2, [MarshalAs(UnmanagedType.LPStr)] string value3);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void OutRipCallback(int winid, [MarshalAs(UnmanagedType.LPStr)] string plname, int points, [MarshalAs(UnmanagedType.LPStr)] string killer, [MarshalAs(UnmanagedType.LPStr)] string time);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int UIHasInputCallback();

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayImmediateSoundCallback(
        int ghsound, 
        [MarshalAs(UnmanagedType.LPStr)] string eventPath, 
        int bankid, double eventVolume,
        double soundVolume,
        [MarshalAs(UnmanagedType.LPArray, SizeConst = GHConstants.MaxSoundParameters)][In] string[] parameterNames, 
        [MarshalAs(UnmanagedType.LPArray, SizeConst = GHConstants.MaxSoundParameters)][In] float[] parameterValues, 
        int arraysize, 
        int sound_type, 
        int play_group, 
        uint dialogue_mid,
        ulong play_flags);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayMusicCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayLevelAmbientCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayOccupationAmbientCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayEffectAmbientCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int SetEffectAmbientVolumeCallback(
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayEnvironmentAmbientCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int StopAllSoundsCallback(
        uint flags,
        uint dialogue_mid);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int AddAmbientSoundCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, double eventVolume,
        double soundVolume, out UInt64 soundSourceId);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int DeleteAmbientSoundCallback(UInt64 soundSourceId);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int SetAmbientSoundVolumeCallback(UInt64 soundSourceId, double soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte AdjustGeneralVolumesCallback(double value1, double value2, double value3, double value4, double value5, double value6);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string AskNameCallback([MarshalAs(UnmanagedType.LPStr)] string modeName, [MarshalAs(UnmanagedType.LPStr)] string modeDescription);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string CharVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string CharPtrBooleanCallback(byte value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int YnFunctionCallback(int style, int attr, int color, int glyph, [MarshalAs(UnmanagedType.LPStr)] string title, [MarshalAs(UnmanagedType.LPStr)] string query, [MarshalAs(UnmanagedType.LPStr)] string responses, [MarshalAs(UnmanagedType.LPStr)] string default_answer, [MarshalAs(UnmanagedType.LPStr)] string response_descriptions, [MarshalAs(UnmanagedType.LPStr)] string introline, ulong ynflags);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string GetLineCallback(int style, int attr, int color, [MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, [MarshalAs(UnmanagedType.LPStr)] string value3, [MarshalAs(UnmanagedType.LPStr)] string value4);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntCharCharUintCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, uint value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntIntIntOutIntCallback(int value1, int value2, /* [Out, MarshalAs(UnmanagedType.LPArray)] out int[] */ out IntPtr value3, out int value4);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int NhPosKeyCallback(out int x, out int y, out int mod);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void StatusUpdateCallback(int value1, string value2, long value3, int value4, int value5, int value6, IntPtr value7);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidUlongCallback(ulong value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntConstCharPtrConstCharPtrBooleanCallback(int value1, string value2, string value3, byte value4);

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
    public delegate void DisplayGUIEffectCallback(int x, int y, int style, ulong tflags);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void UpdateCursorCallback(int style, int force_paint, int show_on_u);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int OpenSpecialViewCallback(int viewtype, [MarshalAs(UnmanagedType.LPStr)] string text, [MarshalAs(UnmanagedType.LPStr)] string title, int attr, int color);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void SendObjectDataCallback(int x, int y, IntPtr otmp_ptr, int cmdtype, int where, IntPtr otypdata_ptr, ulong oflags);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void SendMonsterDataCallback(int cmdtype, int x, int y, IntPtr monster_data_ptr, ulong oflags);
}

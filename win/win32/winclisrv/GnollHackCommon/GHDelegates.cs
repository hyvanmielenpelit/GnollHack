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
    public delegate byte BooleanDoubleDoubleDoubleDoubleDoubleCallback(double value1, double value2, double value3, double value4, double value5);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanIntDoubleVoidPtrCallback(int value1, double value2, IntPtr value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanDoubleVoidPtrCallback(double value1, IntPtr value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate byte BooleanVoidPtrCallback(IntPtr value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int CreateGHWindowCallback(int winid, int style, int glyph);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PrintGlyphCallback(int value1, int value2, int value3, int value4, int value5, int value6, int value7, uint value8, LayerInfo layers);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void StartMenuCallback(int winid, int style);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddMenuCallback(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, [MarshalAs(UnmanagedType.LPStr)]  string text, byte presel, int color);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void AddExtendedMenuCallback(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, [MarshalAs(UnmanagedType.LPStr)] string text, byte presel, int color, int maxcount,
        UInt64 oid, UInt64 mid, char heading_groupaccel, ulong menuflags);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void EndMenuCallback(int value1, [MarshalAs(UnmanagedType.LPStr)] string value2, [MarshalAs(UnmanagedType.LPStr)] string value3);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayImmediateSoundCallback(
        int ghsound, 
        [MarshalAs(UnmanagedType.LPStr)] string eventPath, 
        int bankid, float eventVolume, 
        float soundVolume,
        [MarshalAs(UnmanagedType.LPArray, SizeConst = GHConstants.MaxSoundParameters)][In] string[] parameterNames, 
        [MarshalAs(UnmanagedType.LPArray, SizeConst = GHConstants.MaxSoundParameters)][In] float[] parameterValues, 
        int arraysize, 
        int sound_type, 
        int play_group, 
        uint dialogue_mid);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int PlayMusicCallback(
        int ghsound,
        [MarshalAs(UnmanagedType.LPStr)] string eventPath,
        int bankid, float eventVolume,
        float soundVolume);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int StopAllSoundsCallback(
        uint flags,
        uint dialogue_mid);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string CharVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string CharPtrBooleanCallback(byte value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntIntIntConstCharPtrConstCharPtrCharPtrCallback(int attr, int color, [MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, [MarshalAs(UnmanagedType.LPStr)] string value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string GetLineCallback(int attr, int color, [MarshalAs(UnmanagedType.LPStr)] string value1);

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
    public delegate void AddContextMenuCallback(int value1, int value2, int value3, int value4, string value5, string value6, int value7, int value8);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void ToggleAnimationTimerCallback(int value1, int value2, int value3, int value4, int value5, int value6, ulong value7);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayFloatingTextCallback(int x, int y, string text, int style, int attr, int color, ulong tflags);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void DisplayScreenTextCallback(string text, string subtext, int style, int attr, int color, ulong tflags);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void UpdateCursorCallback(int style, int force_paint, int show_on_u);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void SendObjectDataCallback(int x, int y, obj otmp, int cmdtype, int where, int tile_height, ulong oflags);
}

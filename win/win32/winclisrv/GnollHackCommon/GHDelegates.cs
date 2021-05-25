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
    public delegate void VoidIntIntCallback(int value1, int value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntIntCallback(int value1, int value2, int value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntBooleanCallback(int value1, byte value2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntConstCharCallback(int value1, int value2, [MarshalAs(UnmanagedType.LPStr)] string value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntConstCharIntCallback(int value1, int value2, [MarshalAs(UnmanagedType.LPStr)] string value3, int value4);
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
    public delegate void VoidIntIntIntIntIntCharPtrIntUlongCallback(int value1, int value2, int value3, int value4, int value5, [MarshalAs(UnmanagedType.LPStr)] string value6, int value7, uint value8);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string CharVoidCallback();
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public delegate string CharPtrBooleanCallback(byte value1);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate sbyte CharConstCharPtrConstCharPtrCharCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, sbyte value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidConstCharPtrCharPtrCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntCharCharUintCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, uint value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntIntIntVoidPtrCallback(int value1, int value2, IntPtr value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate int IntIntPtrIntPtrIntPtrCallback(ref int value1, ref int value2, ref int value3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntIntPtrIntIntIntUlongPtrCallback(int value1, ref int value2, int value3, int value4, int value5, ref UInt32 value6);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidUlongCallback(UInt32 value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void VoidIntConstCharPtrConstCharPtrBooleanCallback(int value1, string value2, string value3, byte value4);

}

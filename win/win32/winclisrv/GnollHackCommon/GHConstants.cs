using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace GnollHackCommon
{
    /* Colors */
    public enum nhcolor
    {
        CLR_BLACK = 0,
        CLR_RED,
        CLR_GREEN,
        CLR_BROWN, /* on IBM, low-intensity yellow is brown */
        CLR_BLUE,
        CLR_MAGENTA,
        CLR_CYAN,
        CLR_GRAY, /* low-intensity white */
        NO_COLOR,
        CLR_ORANGE,
        CLR_BRIGHT_GREEN,
        CLR_YELLOW,
        CLR_BRIGHT_BLUE,
        CLR_BRIGHT_MAGENTA,
        CLR_BRIGHT_CYAN,
        CLR_WHITE,
        CLR_MAX
    }

    [Flags]
    public enum MenuItemAttributes : int
    {
        None =      0x00000000,
        Bold =      0x00000001,
        Dim =       0x00000002,
        Underline = 0x00000004,
        Blink =     0x00000005,
        Inverse =   0x00000007,
        Urgent =    0x00000010,
        NoHistory = 0x00000020,
        StayOnLine =0x00000040,
        NoTabs =    0x00000080,
    }

    [Flags]
    public enum MapSpecial : uint
    {
        None =              0x00000000,
        Corpse =            0x00000001,
        Invisible =         0x00000002,
        Detected =          0x00000004,
        Pet =               0x00000008,
        Ridden =            0x00000010,
        Statue =            0x00000020,
        ObjectPile =        0x00000040,
        BlackWhiteLava =    0x00000080,
        Peaceful =          0x00000100,
        Saddled =           0x00000200,
        Female =            0x00000400,
        HorizontalFlip =    0x00000800,
        StoneInverse =      0x00001000,
    }

    [Flags]
    public enum MenuFlags : UInt32
    {
        None =              0x00000000,
        IsHeading =         0x00000001,
        IsGroupHeading =    0x00000002,
    }
    public enum TTYCursorStyle
    {
        BlinkingUnderline = 0,
        GreenBlock
    }
    public enum GHMapMode
    {
        Normal = 0,
        Travel,
        Look,
        NumMapModes
    }
    public enum NhGetPosMods
    {
        None = 0,
        Click1,
        Click2
    }

    public class GHConstants
    {
        public const int InputBufferLength = 32;
        public const int MaxGHWindows = 32;
        public const int PollingInterval = 25;
        public const int MapCols = 80;
        public const int MapRows = 21;
        public const int DefaultAnimationInterval = 25;
        public const int MaxMessageHistoryLength = 256;
        public const long MoveOrPressTimeThreshold = 250; /* Milliseconds */
        public const int BlCondMaskBits = 23;
        public const int MaxStatusLines = 8;
        public const int MaxStatusFieldsPerLine = 24;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct GHSelectedItem
    {
        public IntPtr Identifier;
        public int Count;
    }
}

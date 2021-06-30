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

    public enum GHWindowPrintLocations
    {
        PrintToMap = 0,
        PrintToWindow,
        RawPrint
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
    public enum GHGraphicsStyle
    {
        ASCII = 0,
        Tiles
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
        Click2,
        Click3
    }

    [Flags]
    public enum RunGnollHackFlags: UInt32
    {
        None =          0x00000000,
        SetWinCaps =    0x00000001,
        WizardMode =    0x00000002
    }

    [Flags]
    public enum StopSoundFlags : UInt32
    {
        None =                  0x00000000,
        Music =                 0x00000001,
        LevelAmbient =          0x00000002,
        EnvironmentAmbient =    0x00000004,
        OccupationAmbient =     0x00000008,
        EffectAmbient =         0x00000010,
        ImmediateNormal =       0x00000020,
        ImmediateLong =         0x00000040,
        SoundSources =          0x00000080,
        All =                   0x000000FF,
        ImmediateAll =          0x00000060,
        OnlyDialogueMid =       0x00000100,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct AnimationDefinition
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public string animation_name;
        public int animation_type;
        public sbyte number_of_tiles;
        public sbyte number_of_frames;
        public sbyte number_of_tile_animations;
        public sbyte intervals_between_frames;
        public int play_type;
        public int main_tile_use_style;
        public int main_tile_autodraw;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxFramesPerAnimation)]
        public sbyte[] frame2tile;
        public sbyte sound_play_frame;
        public sbyte action_execution_frame;
        public short tile_enlargement;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxFramesPerAnimation)]
        public int[] frame_autodraw;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct EnlargementDefinition
    {
        public string enlargement_name;
        public sbyte number_of_animation_tiles;
        public sbyte number_of_animation_frames;
        public sbyte number_of_enlargement_tiles;
        public sbyte width_in_tiles;
        public sbyte height_in_tiles;
        public sbyte main_tile_x_coordinate; /* Always 0 or 1 on the last row */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NumPositionsInEnlargement)]
        public sbyte[] position2tile;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NumPositionsInEnlargement)]
        public byte[] position_flags;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NumPositionsInEnlargement)]
        public int[] position_autodraw;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ReplacementDefinition
    {
        public string replacement_name;
        public sbyte number_of_tiles;
        public ulong replacement_events;
        public int replacement_action; /* hard-coded - defines which tile to use and when */
        public int general_autodraw; /* For zero-tile replacements */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public IntPtr[] tile_names; /* Not needed */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public short[] tile_animation;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public short[] tile_enlargement;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public int[] tile_autodraw;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxTilesPerReplacement)]
        public ulong[] tile_flags;

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct AutoDrawDefinition
    {
        public string autodraw_name;
        public int draw_type;
        public byte flags;
        public int source_glyph;
        public int source_glyph2;
        public int source_glyph3;
        public int source_glyph4;
        public UInt32 parameter1;
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
        public const int TileWidth = 64;
        public const int TileHeight = 96;
        public const int MaxTileSheets = 4;
        public const int NumberOfTilesPerSheet = 16224;
        public const int MaxSoundParameters = 8;
        public const int MaxFramesPerAnimation = 32;
        public const int NumPositionsInEnlargement = 5;
        public const int MaxTilesPerReplacement = 32;

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct GHSelectedItem
    {
        public IntPtr Identifier;
        public int Count;
    }
}

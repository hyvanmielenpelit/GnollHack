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
    public enum MenuFlags : ulong
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
    public enum RunGnollHackFlags: ulong
    {
        None =          0x00000000,
        SetWinCaps =    0x00000001,
        WizardMode =    0x00000002
    }

    [Flags]
    public enum StopSoundFlags : ulong
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

    [Flags]
    public enum LayerFlags : ulong
    {
         None =                     0x00000000UL,
         LFLAGS_M_PET =             0x00000001UL,
         LFLAGS_M_PEACEFUL =        0x00000002UL,
         LFLAGS_M_DETECTED =        0x00000004UL,
         LFLAGS_M_RIDDEN =          0x00000008UL,
         LFLAGS_M_SADDLED =         0x00000010UL,
         LFLAGS_M_YOU =             0x00000020UL,
         LFLAGS_M_MIMIC_OBJECT =    0x00000040UL,
         LFLAGS_M_MIMIC_FURNITURE = 0x00000080UL,
         LFLAGS_M_BEING_HIT =       0x00000100UL,
         LFLAGS_U_TETHERED =        0x00000200UL, /* You are tethered */
         LFLAGS_M_TETHERED =        0x00000400UL, /* Monster is tethered */
         LFLAGS_M_DROPPING_PIERCER =0x00000800UL,
         LFLAGS_M_WORM_TAIL =       0x00001000UL,
         LFLAGS_M_WORM_SEEN =       0x00002000UL,
         LFLAGS_M_KILLED =          0x00004000UL,
         LFLAGS_M_MASK =            0x0000FFFFUL,
         LFLAGS_O_PILE =            0x00010000UL,
         LFLAGS_O_IN_PIT =          0x00020000UL,
         LFLAGS_O_CHAIN =           0x00040000UL,
         LFLAGS_O_MASK =            0x00070000UL,
         LFLAGS_CMAP_MASK =         0x00080000UL,
         LFLAGS_ZAP_LEADING_EDGE =  0x00100000UL, /* First (leading) tile in a zap */
         LFLAGS_ZAP_TRAILING_EDGE = 0x00200000UL, /* Last (trailing) tile in a zap */
         LFLAGS_ZAP_MASK = (0x00100000UL | 0x00200000UL),
         LFLAGS_T_TRAPPED =         0x00400000UL,
         /* free */
         LFLAGS_SHOWING_MEMORY =    0x01000000UL, /* also implies that you cannot see the location (and hence showing memory) */
         LFLAGS_SHOWING_DETECTION = 0x02000000UL, /* Do not darken */
         LFLAGS_ASCENSION_RADIANCE =0x04000000UL, /* Lighten up */
         LFLAGS_CAN_SEE =           0x08000000UL, /* cansee(x, y) is true (if not, then darken etc.) */
         LFLAGS_UXUY =              0x10000000UL, /* x == u.ux && y == u.uy is true */
         LFLAGS_APPEARS_UNLIT =     0x20000000UL, /* looks unlit */
    }

    [Flags]
    public enum glyph_tile_flags : byte
    {
        None = 0x00,
        GLYPH_TILE_FLAG_FLIP_HORIZONTALLY = 0x01,
        GLYPH_TILE_FLAG_FLIP_VERTICALLY = 0x02,
        GLYPH_TILE_FLAG_FULL_SIZED_ITEM = 0x04,
        GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE = 0x08,
        GLYPH_TILE_FLAG_HALF_SIZED_NONITEM = 0x10,
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
        public ulong parameter1;
    }

    public enum layer_types
    {
        LAYER_FLOOR = 0,
        LAYER_FLOOR_DOODAD,   /* Doodads underneath features and traps */
        LAYER_FEATURE,
        LAYER_TRAP,
        LAYER_FEATURE_DOODAD, /* Doodads above features and traps */
        LAYER_BACKGROUND_EFFECT,
        LAYER_CHAIN,
        LAYER_OBJECT,
        LAYER_MONSTER,
        LAYER_MISSILE,
        LAYER_COVER_TRAP,
        LAYER_COVER_OBJECT,
        LAYER_COVER_FEATURE,
        LAYER_LEASH,
        LAYER_ENVIRONMENT,
        LAYER_ZAP,
        LAYER_GENERAL_EFFECT,
        LAYER_MONSTER_EFFECT,
        LAYER_GENERAL_UI,
        MAX_LAYERS
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct LayerInfo
    {
        public int glyph; /* For ascii compatibility */
        public int bkglyph; /* For ascii compatibility */

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)layer_types.MAX_LAYERS)]
        public int[] layer_glyphs;

        public ulong layer_flags;
        public uint m_id;  /* check that the monster found at the square is the one that is supposed to be drawn by comparing their m_ids */

        public IntPtr memory_objchn;     /* Pointer to the location's first memory object*/

        public short damage_displayed;
        public short hit_tile;
        public short special_monster_layer_height;

        public byte missile_poisoned;
        public byte missile_elemental_enchantment;
        public byte missile_exceptionality;
        public byte missile_mythic_prefix;
        public byte missile_mythic_suffix;
        public byte missile_eroded;
        public byte missile_eroded2;
        public ulong missile_flags;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxLeashed + 1)]
        public sbyte[] leash_mon_x; /* the last coordinate is the other end of the leash, i.e., u.ux at the time */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxLeashed + 1)]
        public sbyte[] leash_mon_y; /* the last coordinate is the other end of the leash, i.e., u.uy at the time */
    }

    public enum animation_play_types
    {
        ANIMATION_PLAY_TYPE_ALWAYS = 0,
        ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY
    }

    public enum animation_timer_types
    {
        ANIMATION_TIMER_GENERAL = 0,
        ANIMATION_TIMER_YOU,
        ANIMATION_TIMER_MONSTER,
        ANIMATION_TIMER_EXPLOSION,
        ANIMATION_TIMER_ZAP,
        ANIMATION_TIMER_SPECIAL_EFFECT,
    }

    public enum immediate_sound_types
    {
        IMMEDIATE_SOUND_SFX = 0,
        IMMEDIATE_SOUND_UI,
        IMMEDIATE_SOUND_DIALOGUE
    }

    public enum sound_play_groups
    {
        SOUND_PLAY_GROUP_NORMAL = 0,
        SOUND_PLAY_GROUP_LONG,
        MAX_SOUND_PLAY_GROUPS
    }


    public enum hit_tile_types
    {
        HIT_GENERAL = 0,
        HIT_POISONED,
        HIT_DISINTEGRATED,
        HIT_CRUSHED,
        HIT_SPLASHED_ACID,
        HIT_ON_FIRE,
        HIT_FROZEN,
        HIT_ELECTROCUTED,
        HIT_DEATH,
        HIT_SLEEP,
        HIT_PETRIFIED,
        HIT_CRITICAL,
        HIT_FLASHED_LIGHT,
        HIT_PARALYZED,
        HIT_SLIMED,
        HIT_SICK,
        HIT_FAMINE,
        HIT_HEAL,
        HIT_WERE,
        HIT_DRAIN_LEVEL,
        HIT_SLOW,
        HIT_HALLUCINATED,
        MAX_HIT_TILES
    }

    public enum general_tile_types
    {
        GENERAL_TILE_DEATH = 0,
        GENERAL_TILE_CHAIN_IS_UP,
        GENERAL_TILE_CHAIN_IS_RIGHT,
        GENERAL_TILE_CHAIN_IS_DOWN,
        GENERAL_TILE_CHAIN_IS_LEFT,
        GENERAL_TILE_CHAIN_MAIN,
        MAX_GENERAL_TILES
    }

    public enum statusfields
    {
        BL_CHARACTERISTICS = -3, /* alias for BL_STR..BL_CH */
        BL_RESET = -2,           /* Force everything to redisplay */
        BL_FLUSH = -1,           /* Finished cycling through bot fields */
        BL_TITLE = 0,
        BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH, BL_GOLD, BL_ALIGN,
        BL_SCORE, BL_CAP, BL_ENE, BL_ENEMAX,
        BL_XP, BL_2WEP, BL_SKILL, BL_AC, BL_MC_LVL, BL_MC_PCT, BL_MOVE, BL_UWEP, BL_UWEP2, BL_HD, BL_TIME, BL_HUNGER, BL_HP,
        BL_HPMAX, BL_LEVELDESC, BL_EXP, BL_CONDITION,
        BL_PARTYSTATS, BL_PARTYSTATS2, BL_PARTYSTATS3, BL_PARTYSTATS4, BL_PARTYSTATS5,
        MAXBLSTATS
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
        public const double MoveByHoldingDownThreshold = 0.25; /* Seconds */
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
        public const int MaxLeashed = 2;
        public const int MaxPlayedSpecialEffects = 12;
        public const int MaxPlayedZapAnimations = 16;
        public const int MaxNormalImmediateSoundInstances = 64;
        public const int MaxLongImmediateSoundInstances = 64;

    }

    [StructLayout(LayoutKind.Sequential)]
    public struct GHSelectedItem
    {
        public IntPtr Identifier;
        public int Count;
    }
}

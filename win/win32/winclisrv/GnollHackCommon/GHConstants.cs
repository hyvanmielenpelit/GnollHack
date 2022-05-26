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

    public enum nhaltcolor
    {
        CLR_ALT_BLACK = 0,
        CLR_ALT_RED,
        CLR_ALT_GREEN,
        CLR_ALT_GOLD,   /* Gold instead of brown */
        CLR_ALT_BLUE,
        CLR_ALT_MAGENTA,
        CLR_ALT_CYAN,
        CLR_ALT_GRAY,
        NO_ALT_COLOR,
        CLR_ALT_ORANGE,
        CLR_ALT_BRIGHT_GREEN,
        CLR_ALT_YELLOW,
        CLR_ALT_BRIGHT_BLUE,
        CLR_ALT_BRIGHT_MAGENTA,
        CLR_ALT_BRIGHT_CYAN,
        CLR_ALT_WHITE,
        CLR_ALT_MAX
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
        Title =     0x00000100,
        Heading =   0x00000200,
        Sub =       0x00000400,
        IndentDash =    0x00000800,
        IndentColon =   0x00001000,
        IndentAstr =    0x00002000,
        IndentSpace =   0x00004000,
        IndentBracket = 0x00008000,
        IndentPeriod =  0x00001800,
        IndentDoubleSpace = 0x00004800,
        IndentBracketOrDoubleSpace = 0x0000C800,
        AlignCenter = 0x00010000,
        AlignRight = 0x00020000,
        Inactive = 0x00040000,
        HalfSize = 0x00080000,
        AltColors = 0x00100000,
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
        MENU_FLAGS_NONE =                       0x00000000,
        MENU_FLAGS_IS_HEADING =                 0x00000001,
        MENU_FLAGS_IS_GROUP_HEADING =           0x00000002,
        MENU_FLAGS_MONOSPACE_FONT_REQUESTED =   0x00000004,
        MENU_FLAGS_COUNT_DISALLOWED =           0x00000008,
        MENU_FLAGS_USE_SKILL_MENU_DATA =        0x00000010,
        MENU_FLAGS_USE_SPELL_MENU_DATA =        0x00000020,
        MENU_FLAGS_ACTIVE =                     0x00000040,
        MENU_FLAGS_USE_COLOR_FOR_SUFFIXES =     0x00000080,
        MENU_FLAGS_USE_SPECIAL_SYMBOLS =        0x00000100,
    }

    [Flags]
    public enum MenuDataFlags : byte
    {
        None = 0x00,
        MENU_DATAFLAGS_HAS_OBJECT_DATA = 0x01,
        MENU_DATAFLAGS_HAS_MONSTER_DATA = 0x02,
        MENU_DATAFLAGS_HALLUCINATED = 0x04,
        MENU_DATAFLAGS_FEMALE = 0x08,
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
    public enum MapRefreshRateStyle
    {
        MapFPS20 = 0,
        MapFPS30,
        MapFPS40,
        MapFPS60,
        MapFPS80,
        MapFPS90,
        MapFPS120,
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
    public enum ChmodPermissions : uint
    {
        // user permissions
        S_IRUSR = 0x100U,
        S_IWUSR = 0x80U,
        S_IXUSR = 0x40U,

        // group permission
        S_IRGRP = 0x20U,
        S_IWGRP = 0x10U,
        S_IXGRP = 0x8U,

        // other permissions
        S_IROTH = 0x4U,
        S_IWOTH = 0x2U,
        S_IXOTH = 0x1U,

        S_IALL = 0x1FFU,
    }

    [Flags]
    public enum RunGnollHackFlags: ulong
    {
        None =          0x00000000,
        SetWinCaps =    0x00000001,
        WizardMode =    0x00000002,
        FullVersion =   0x00000004,
        ModernMode =    0x00000008, /* Upon death, the character teleports back to starting altar */
        CasualMode =    0x00000010, /* Save games are preserved */
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
         LFLAGS_M_RIDDEN =          0x00000004UL,
         LFLAGS_M_DETECTED =        0x00000008UL,
         LFLAGS_M_CANSPOTMON =      0x00000010UL,
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
         LFLAGS_O_MASK =            0x000F0000UL,
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
         LFLAGS_NO_WALL_END_AUTODRAW=0x40000000UL,/* NO_WALL_END_AUTODRAW(x, y) is true */
    }

    [Flags]
    public enum LayerMonsterFlags : ulong
    {
        None =                          0x00000000UL,
        LMFLAGS_WORM_HEAD =             0x00000001UL,
        LMFLAGS_WORM_TAILEND =          0x00000002UL,
        LMFLAGS_INVISIBLE_TRANSPARENT = 0x00000004UL,
        LMFLAGS_SEMI_TRANSPARENT =      0x00000008UL,
        LMFLAGS_RADIAL_TRANSPARENCY =   0x00000010UL,
        LMFLAGS_GLASS_TRANSPARENCY =    0x00000020UL,
        LMFLAGS_CAN_SPOT_SELF =         0x00000040UL,
        LMFLAGS_LONG_WORM_WITH_TAIL =   0x00000080UL,
        LMFLAGS_LONG_WORM_TAIL =        0x00000100UL,
        LMFLAGS_BOSS_MONSTER_FIGHT =    0x00000200UL,
    }

    [Flags]
    public enum LayerMissileFlags : ulong
    {
        MISSILE_FLAGS_CORRODEABLE =  0x00000001UL,
        MISSILE_FLAGS_ROTTABLE =     0x00000002UL,
        MISSILE_FLAGS_FLAMMABLE =    0x00000004UL,
        MISSILE_FLAGS_RUSTPRONE =    0x00000008UL,
        MISSILE_FLAGS_ERODEPROOF =   0x00000010UL,
        MISSILE_FLAGS_POISONABLE =   0x00000020UL,
        MISSILE_FLAGS_TETHERED =     0x00000040UL, /* Missile is tethered */
    }

 
    [Flags]
    public enum glyph_tile_flags : byte
    {
        None = 0x00,
        GLYPH_TILE_FLAG_FLIP_HORIZONTALLY = 0x01,
        GLYPH_TILE_FLAG_FLIP_VERTICALLY = 0x02,
        GLYPH_TILE_FLAG_FULL_SIZED_ITEM = 0x04,
        GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE = 0x08,
        GLYPH_TILE_FLAG_HALF_SIZED_TILE = 0x10,
        GLYPH_TILE_FLAG_HAS_FLOOR_TILE = 0x20,
        GLYPH_TILE_FLAG_TWO_WIDE_CENTERED = 0x40,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct AnimationDefinition
    {
        public IntPtr animation_name;
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
        public IntPtr enlargement_name;
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
        public IntPtr replacement_name;
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
        public IntPtr autodraw_name;
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
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)layer_types.MAX_LAYERS)]
        public int[] layer_gui_glyphs;

        public ulong layer_flags;
        public uint m_id;  /* check that the monster found at the square is the one that is supposed to be drawn by comparing their m_ids */
        public uint o_id;  /* this is the o_id of the possibly moving boulder */

        public IntPtr memory_objchn;     /* Pointer to the location's first memory object*/

        public short damage_displayed;
        public short hit_tile;

        public short special_monster_layer_height;
        public sbyte monster_origin_x;
        public sbyte monster_origin_y;
        public int monster_hp;
        public int monster_maxhp;
        public int rider_glyph;
        public int rider_gui_glyph;
        public ulong status_bits;
        public ulong condition_bits;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NUM_BUFF_BIT_ULONGS)]
        public ulong[] buff_bits;

        public sbyte wsegdir;
        public sbyte reverse_prev_wsegdir;
        public ulong monster_flags;

        public short object_height;

        public byte missile_poisoned;
        public byte missile_elemental_enchantment;
        public byte missile_exceptionality;
        public byte missile_mythic_prefix;
        public byte missile_mythic_suffix;
        public byte missile_eroded;
        public byte missile_eroded2;
        public ulong missile_flags;
        public short missile_height;
        public sbyte missile_origin_x;
        public sbyte missile_origin_y;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxLeashed + 1)]
        public sbyte[] leash_mon_x; /* the last coordinate is the other end of the leash, i.e., u.ux at the time */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.MaxLeashed + 1)]
        public sbyte[] leash_mon_y; /* the last coordinate is the other end of the leash, i.e., u.uy at the time */
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct simple_layer_info
    {
        public int glyph; /* For ascii compatibility */
        public int bkglyph; /* For ascii compatibility */
        public ulong layer_flags;
        public uint m_id;  /* check that the monster found at the square is the one that is supposed to be drawn by comparing their m_ids */
        public uint o_id;  /* this is the o_id of the possibly moving boulder */
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



    /* GHWindow styles */
    public enum ghwindow_styles
    {
        GHWINDOW_STYLE_GENERAL = 0,
        GHWINDOW_STYLE_PAGER_GENERAL,
        GHWINDOW_STYLE_PAGER_KEEP_LINE_BREAKS,
        GHWINDOW_STYLE_PAGER_SPEAKER,
        GHWINDOW_STYLE_OBJECT_DESCRIPTION_SCREEN,
        GHWINDOW_STYLE_MONSTER_DESCRIPTION_SCREEN,
        GHWINDOW_STYLE_OBJECT_COMMAND_MENU,
        GHWINDOW_STYLE_MONSTER_COMMAND_MENU,
        GHWINDOW_STYLE_CHAT_MENU,
        GHWINDOW_STYLE_CHAT_ITEM_MENU,
        GHWINDOW_STYLE_CHARACTER_MENU_SCREEN,
        GHWINDOW_STYLE_CHARACTER_SCREEN,
        GHWINDOW_STYLE_ENLIGHTENMENT_SCREEN,
        GHWINDOW_STYLE_OUTRIP,
        GHWINDOW_STYLE_PLAYER_SELECTION,
        GHWINDOW_STYLE_SAVED_GAME_SELECTION,
        GHWINDOW_STYLE_SKILL_DESCRIPTION_SCREEN,
        GHWINDOW_STYLE_SPELL_DESCRIPTION_SCREEN,
        GHWINDOW_STYLE_SKILL_COMMAND_MENU,
        GHWINDOW_STYLE_SPELL_COMMAND_MENU,
        GHWINDOW_STYLE_NARROW_LIST,
        GHWINDOW_STYLE_MEDIUM_WIDE_LIST,
        GHWINDOW_STYLE_SEMI_WIDE_LIST,
        GHWINDOW_STYLE_DISPLAY_FILE,
        MAX_GHWINDOW_STYLES
    }

    /* GHMenu styles */
    public enum ghmenu_styles
    {
        GHMENU_STYLE_GENERAL = 0,
        GHMENU_STYLE_INVENTORY,
        GHMENU_STYLE_PERMANENT_INVENTORY,
        GHMENU_STYLE_OTHERS_INVENTORY,
        GHMENU_STYLE_PICK_ITEM_LIST,
        GHMENU_STYLE_PICK_CATEGORY_LIST,
        GHMENU_STYLE_ITEM_COMMAND,
        GHMENU_STYLE_CHAT,
        GHMENU_STYLE_CHAT_CHOOSE_ITEM,
        GHMENU_STYLE_CHOOSE_SIMPLE,
        GHMENU_STYLE_CHOOSE_COMMAND,
        GHMENU_STYLE_CHOOSE_SAVED_GAME,
        GHMENU_STYLE_CHOOSE_PLAYER,
        GHMENU_STYLE_CHOOSE_DIFFICULTY,
        GHMENU_STYLE_CHARACTER,
        GHMENU_STYLE_ATTRIBUTES,
        GHMENU_STYLE_SKILLS,
        GHMENU_STYLE_SPELLS,
        GHMENU_STYLE_SKILLS_ALTERNATE,
        GHMENU_STYLE_SPELLS_ALTERNATE,
        GHMENU_STYLE_DUNGEON_OVERVIEW,
        GHMENU_STYLE_OPTIONS,
        GHMENU_STYLE_HELP,
        GHMENU_STYLE_SKILL_COMMAND,
        GHMENU_STYLE_SPELL_COMMAND,
        GHMENU_STYLE_GENERAL_COMMAND,
        GHMENU_STYLE_MONSTER_ABILITY,
        MAX_GHMENU_STYLES
    }

    public enum floating_text_types
    {
        FLOATING_TEXT_GENERAL = 0,
        FLOATING_TEXT_HEALING,
        FLOATING_TEXT_DAMAGE,
        FLOATING_TEXT_GOLD_ACQUIRED,
        FLOATING_TEXT_GOLD_STOLEN,
        FLOATING_TEXT_MANA_GAIN,
        FLOATING_TEXT_MANA_LOSS,
        FLOATING_TEXT_NUTRITION_GAIN,
        FLOATING_TEXT_NUTRITION_LOSS,
        FLOATING_TEXT_ITEM_ACQUIRED,
        FLOATING_TEXT_ITEM_STOLEN,
        FLOATING_TEXT_ATTRIBUTE_GAIN,
        FLOATING_TEXT_ATTRIBUTE_LOSS,
    }

    public enum screen_text_types
    {
        SCREEN_TEXT_GENERAL = 0,
        SCREEN_TEXT_ENTER_DUNGEON_LEVEL,
        SCREEN_TEXT_GAIN_LEVEL,
        SCREEN_TEXT_SAVING,
        SCREEN_TEXT_DEAD,
        SCREEN_TEXT_ESCAPED,
        SCREEN_TEXT_ASCENDED,
        SCREEN_TEXT_QUIT,
        SCREEN_TEXT_SPECIAL_END,
        SCREEN_TEXT_BOSS_FIGHT,
        SCREEN_TEXT_EXTRA_LIFE_SPENT,
    }

    public enum game_cursor_types
    {
        CURSOR_STYLE_GENERIC_CURSOR = 0,
        CURSOR_STYLE_LOOK_CURSOR,
        CURSOR_STYLE_TRAVEL_CURSOR,
        CURSOR_STYLE_NAME_CURSOR,
        CURSOR_STYLE_TELEPORT_CURSOR,
        CURSOR_STYLE_JUMP_CURSOR,
        CURSOR_STYLE_POLEARM_CURSOR,
        CURSOR_STYLE_GRAPPLE_CURSOR,
        CURSOR_STYLE_SPELL_CURSOR,
        CURSOR_STYLE_PAY_CURSOR,
        CURSOR_STYLE_INVISIBLE,
        MAX_CURSORS
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

    public enum game_ui_tile_types
    {
        GENERAL_UI_ELEMENTS = 0,        /* Check box etc. */
        STATUS_MARKS,               /* Pet mark, detection mark, ridden mark, pile mark, etc. */
        CONDITION_MARKS,            /* One tile for each specified condition (the same as on status line) */
        MAIN_TILE_MARK,
        U_TILE_MARK,
        ITEM_AUTODRAW_GRAPHICS,
        ITEM2_AUTODRAW_GRAPHICS,
        ITEM_PROPERTY_MARKS,
        JAR_GRAPHICS,
        JAR_FOREGROUND,
        MAX_UI_TILES
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
        BL_HPMAX, BL_MODE, BL_LEVELDESC, BL_EXP, BL_CONDITION,
        BL_PARTYSTATS, BL_PARTYSTATS2, BL_PARTYSTATS3, BL_PARTYSTATS4, BL_PARTYSTATS5,
        MAXBLSTATS
    }

    public enum bl_conditions
    {
        BL_COND_STONE = 0,
        BL_COND_SLIME,
        BL_COND_STRNGL,
        BL_COND_SUFFOC,
        BL_COND_FOODPOIS,
        BL_COND_TERMILL,
        BL_COND_BLIND,
        BL_COND_DEAF,
        BL_COND_STUN,
        BL_COND_CONF,
        BL_COND_HALLU,
        BL_COND_LEV,
        BL_COND_FLY,
        BL_COND_RIDE,
        BL_COND_SLOWED,
        BL_COND_PARALYZED,
        BL_COND_FEARFUL,
        BL_COND_SLEEPING,
        BL_COND_CANCELLED,
        BL_COND_SILENCED,
        BL_COND_GRAB,
        BL_COND_ROT,
        BL_COND_LYCANTHROPY,
        NUM_BL_CONDITIONS
    }

    public enum game_ui_status_mark_types
    {
        STATUS_MARK_PET = 0,
        STATUS_MARK_PEACEFUL,
        STATUS_MARK_DETECTED,
        STATUS_MARK_PILE,
        STATUS_MARK_SATIATED,
        STATUS_MARK_HUNGRY,
        STATUS_MARK_WEAK,
        STATUS_MARK_FAINTING,
        STATUS_MARK_BURDENED,
        STATUS_MARK_STRESSED,
        STATUS_MARK_STRAINED,
        STATUS_MARK_OVERTAXED,
        STATUS_MARK_OVERLOADED,
        STATUS_MARK_2WEP,
        STATUS_MARK_SKILL,
        STATUS_MARK_SADDLED,
        STATUS_MARK_LOW_HP,
        STATUS_MARK_CRITICAL_HP,
        STATUS_MARK_SPEC_USED,
        STATUS_MARK_TRAPPED,
        STATUS_MARK_USTUCK,
        STATUS_MARK_INVENTORY,
        STATUS_MARK_TOWNGUARD_PEACEFUL,
        STATUS_MARK_TOWNGUARD_HOSTILE,
        MAX_STATUS_MARKS
    }

    public enum item_property_mark_types
    {
        ITEM_PROPERTY_MARK_POISONED = 0,
        ITEM_PROPERTY_MARK_DEATH_MAGICAL,
        ITEM_PROPERTY_MARK_FLAMING,
        ITEM_PROPERTY_MARK_FREEZING,
        ITEM_PROPERTY_MARK_ELECTRIFIED,
        ITEM_PROPERTY_MARK_EXCEPTIONAL,
        ITEM_PROPERTY_MARK_ELITE,
        ITEM_PROPERTY_MARK_CELESTIAL,
        ITEM_PROPERTY_MARK_PRIMORDIAL,
        ITEM_PROPERTY_MARK_INFERNAL,
        ITEM_PROPERTY_MARK_MYTHIC,
        ITEM_PROPERTY_MARK_LEGENDARY,
        ITEM_PROPERTY_MARK_CORRODED,
        ITEM_PROPERTY_MARK_ROTTED,
        ITEM_PROPERTY_MARK_BURNT,
        ITEM_PROPERTY_MARK_RUSTY,
        ITEM_PROPERTY_MARK_VERY_CORRODED,
        ITEM_PROPERTY_MARK_VERY_ROTTED,
        ITEM_PROPERTY_MARK_VERY_BURNT,
        ITEM_PROPERTY_MARK_VERY_RUSTY,
        ITEM_PROPERTY_MARK_THOROUGHLY_CORRODED,
        ITEM_PROPERTY_MARK_THOROUGHLY_ROTTED,
        ITEM_PROPERTY_MARK_THOROUGHLY_BURNT,
        ITEM_PROPERTY_MARK_THOROUGHLY_RUSTY,
        MAX_ITEM_PROPERTY_MARKS
    }

    public enum autodraw_drawing_types
    {
        AUTODRAW_DRAW_GENERAL = 0,
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DRAW_BOOKSHELF_CONTENTS,
        AUTODRAW_DRAW_WEAPON_RACK_CONTENTS,
        AUTODRAW_DRAW_COFFIN_CONTENTS,
        AUTODRAW_DRAW_CANDELABRUM_CANDLES,
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        AUTODRAW_DRAW_JAR_CONTENTS,
        AUTODRAW_DRAW_FIGURINE,
        AUTODRAW_DRAW_CHAIN,
        AUTODRAW_DRAW_BALL,
        AUTODRAW_DRAW_LONG_WORM,
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct obj
    {
        public IntPtr nobj;
        public IntPtr v;
        public IntPtr cobj; /* contents list for containers */
        public uint o_id;
        public sbyte ox, oy;
        public sbyte ox0, oy0;
        public short otyp; /* object class number */
        public uint owt;
        public long quan; /* number of items */

        public short enchantment; /* Always set to zero by cancellation */
        public short charges; /* number of charges for wand or charged tool ( >= -1 ), always set to -1/0 by cancellation */
        public short special_quality; /* item-specific special quality, e.g., the amount of wetness of a towel, number of candles attached to candelabrum, not affected by cancellation */
        public ulong speflags; /* anything else that might be going on with an item, not affected by cancellation */
        public sbyte oclass;    /* object class */
        public sbyte invlet;    /* designation in inventory */
        public short oartifact; /* artifact array index */
        public byte mythic_prefix; /* magical quality for a weapon or armor giving additional powers */
        public byte mythic_suffix;  /* magical quality for a weapon or armor giving additional powers */
        public byte exceptionality; /* exceptional, elite, etc. weapon, multiplies base damage */
        public byte elemental_enchantment; /* cold, fire, lightning, or deathly */

        public sbyte recharged; /* number of times it's been recharged */
        public sbyte where;        /* where the object thinks it is */
        public sbyte timed; /* # of fuses (timers) attached to this obj */

        public uint bitfields;

        public int corpsenm;         /* type of corpse is mons[corpsenm] */
        public int usecount;           /* overloaded for various things that tally */
        public uint oeaten;        /* nutrition left in food, if partly eaten */
        public long age;               /* creation date */
        public long owornmask;
        public short cooldownleft;       /* item cooldown left before it can be used again*/
        public short repowerleft;       /* artifact cooldown left before its invoke ability can be used again*/
        public short detectioncount;    /* monsters detected for WARN_ORC and other similar properties */
        public byte invokeon;      /* the object's / artifact's invoked ability is on */
        public uint o_id_memory;  /* This is a memory object of this o_id */
        public uint m_id_memory;  /* This is a memory object of this mimic m_id */

        public byte special_tileset;
        public int glyph;
        public int gui_glyph;
        public IntPtr oextra; /* pointer to oextra struct */
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct objclassdata
    {
        public int tile_height;
        public int special_quality;

        public byte lamplit;
        public byte poisoned;
        public byte eroded;
        public byte eroded2;

        public byte corrodeable;
        public byte rottable;
        public byte flammable;
        public byte rustprone;
        public byte poisonable;
    }

    public enum obj_class_types
    {
        RANDOM_CLASS = 0, /* used for generating random objects */
        ILLOBJ_CLASS = 1,
        WEAPON_CLASS = 2,
        ARMOR_CLASS = 3,
        RING_CLASS = 4,
        AMULET_CLASS = 5,
        TOOL_CLASS = 6,
        FOOD_CLASS = 7,
        POTION_CLASS = 8,
        SCROLL_CLASS = 9,
        SPBOOK_CLASS = 10, /* actually SPELL-book */
        WAND_CLASS = 11,
        COIN_CLASS = 12,
        GEM_CLASS = 13,
        ROCK_CLASS = 14,
        BALL_CLASS = 15,
        CHAIN_CLASS = 16,
        VENOM_CLASS = 17,
        REAGENT_CLASS = 18,
        MISCELLANEOUS_CLASS = 19,

        MAX_OBJECT_CLASSES = 20
    }

    public enum obj_where_types
    {
        OBJ_FREE = 0,      /* object not attached to anything */
        OBJ_FLOOR = 1  ,   /* object on floor */
        OBJ_CONTAINED = 2, /* object in a container */
        OBJ_INVENT = 3,    /* object in the hero's inventory */
        OBJ_MINVENT = 4,   /* object in a monster inventory */
        OBJ_MIGRATING = 5, /* object sent off to another level */
        OBJ_BURIED = 6,    /* object buried */
        OBJ_ONBILL = 7,    /* object on shk bill */
        OBJ_HEROMEMORY = 8,/* object remembered by hero */
        NOBJ_STATES = 9
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct monst_info
    {
        public int glyph;
        public int gui_glyph;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = GHConstants.BUFSZ)]
        public string name;
        public uint m_id;
        
        public int mhp;
        public int mhpmax;
        
        public ulong status_bits;
        public ulong condition_bits;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = GHConstants.NUM_BUFF_BIT_ULONGS)]
        public ulong[] buff_bits;

        public ulong monster_flags;
    }

    [Flags]
    public enum speflag_types : ulong
    {
        SPEFLAGS_YOURS                         = 0x00000001UL,
        SPEFLAGS_FEMALE                        = 0x00000002UL,
        SPEFLAGS_MALE                          = 0x00000004UL,
        SPEFLAGS_FACING_RIGHT                  = 0x00000008UL,
        SPEFLAGS_SCHROEDINGERS_BOX             = 0x00000010UL,
        SPEFLAGS_STATUE_HISTORIC               = 0x00000020UL,
        SPEFLAGS_CORPSE_ON_ICE                 = 0x00000040UL,
        SPEFLAGS_DETECTED                      = 0x00000080UL,
        SPEFLAGS_SERVICED_SPELL                = 0x00000100UL,
        SPEFLAGS_MINES_PRIZE                   = 0x00000200UL,
        SPEFLAGS_SOKO_PRIZE1                   = 0x00000400UL,
        SPEFLAGS_SOKO_PRIZE2                   = 0x00000800UL,
        SPEFLAGS_FROM_SINK                     = 0x00001000UL,
        SPEFLAGS_INDESTRUCTIBLE                = 0x00002000UL,
        SPEFLAGS_USES_UP_KEY                   = 0x00004000UL,
        SPEFLAGS_NO_PICKUP                     = 0x00008000UL,  /* Monsters will not pick up this item */
        SPEFLAGS_ROYAL_COFFER                  = 0x00010000UL,
        SPEFLAGS_LID_OPENED                    = 0x00020000UL,
        SPEFLAGS_GRABBED_FROM_YOU              = 0x00040000UL,
        SPEFLAGS_ADDED_TO_YOUR_BILL            = 0x00080000UL,
        SPEFLAGS_WILL_TURN_TO_DUST_ON_PICKUP   = 0x00100000UL,
        SPEFLAGS_CAUGHT_IN_LEAVES              = 0x00200000UL,
        SPEFLAGS_CLONED_ITEM                   = 0x00400000UL,
        SPEFLAGS_INSCRIPTION_REVEALED          = 0x00800000UL
    }

    [Flags]
    public enum obj_bitfield_types : uint
    {
        cursed =        0x00000001,
        blessed =       0x00000002,
        unpaid =        0x00000004,
        no_charge =     0x00000008,
        known =         0x00000010,
        dknown =        0x00000020,
        bknown =        0x00000040,
        rknown =        0x00000080,
        oeroded =       0x00000100 | 0x00000200,
        oeroded2 =      0x00000400 | 0x00000800,
        oerodeproof =   0x00001000,
        olocked =       0x00002000,
        degraded_horn = 0x00004000,
        otrapped =      0x00008000,  /* Monsters will not pick up this item */
        lamplit =       0x00010000,
        makingsound =   0x00020000,
        globby =        0x00040000,
        greased =       0x00080000,
        nomerge =       0x00100000,
        was_thrown =    0x00200000,
        has_special_tileset = 0x00400000,
        in_use =        0x00800000,
        bypass =        0x01000000,
        cknown =        0x02000000,
        lknown =        0x04000000,
        tknown =        0x08000000,
        nknown =        0x10000000,
        aknown =        0x20000000,
        mknown =        0x40000000
        /* 1 free bit to 32-bit integer */
    }

    [Flags]
    public enum objdata_flags : ulong
    {
        None = 0x00000000UL,
        OBJDATA_FLAGS_DRAWN_IN_FRONT =  0x00000001UL,
        OBJDATA_FLAGS_HALLUCINATION =   0x00000002UL,
        OBJDATA_FLAGS_UCHAIN =          0x00000004UL,
        OBJDATA_FLAGS_UBALL =           0x00000008UL,
    }

    [Flags]
    public enum sound_play_flags : ulong
    {
        None = 0x00000000UL,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING = 0x00000001UL,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_QUEUED =  0x00000002UL,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED = 0x00000003UL,
    }

    public enum elemental_enchantment_types
    {
        None = 0,
        COLD_ENCHANTMENT = 1,
        FIRE_ENCHANTMENT = 2,
        LIGHTNING_ENCHANTMENT = 3,
        DEATH_ENCHANTMENT = 4
    }

    public enum exceptionality_types
    {
        EXCEPTIONALITY_NORMAL = 0,
        EXCEPTIONALITY_EXCEPTIONAL,
        EXCEPTIONALITY_ELITE,
        EXCEPTIONALITY_CELESTIAL,
        EXCEPTIONALITY_PRIMORDIAL,
        EXCEPTIONALITY_INFERNAL,
        MAX_EXCEPTIONALITY_TYPES
    }

    public enum gui_command_types
    {
        GUI_CMD_PROGRAM_START = 0,
        GUI_CMD_PREFERENCE_SET,
        GUI_CMD_LOAD_GLYPHS,
        GUI_CMD_BEFORE_COLLECT,
        GUI_CMD_COLLECT_GARBAGE,
        GUI_CMD_AFTER_COLLECT,
        GUI_CMD_FORCE_ASCII,
        GUI_CMD_UNFORCE_ASCII,
        GUI_CMD_START_FLUSH,
        GUI_CMD_FINISH_FLUSH,
        GUI_CMD_LOAD_VIDEOS,
        GUI_CMD_MUTE_SOUNDS,
        GUI_CMD_UNMUTE_SOUNDS,
        GUI_CMD_ACTIVATE_QUIETER_MODE,
        GUI_CMD_DEACTIVATE_QUIETER_MODE,
        GUI_CMD_ENABLE_WIZARD_MODE,
        GUI_CMD_ENABLE_CASUAL_MODE,
        GUI_CMD_PETS,
        GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE,
        GUI_CMD_RESTORE_TRAVEL_MODE,
        GUI_CMD_GAME_START,
        GUI_CMD_PROGRAM_FINISH,
    }

    public enum popup_text_types
    {
        POPUP_TEXT_GENERAL = 0,
        POPUP_TEXT_IDENTIFY,
        POPUP_TEXT_NO_MONSTERS_IN_LIST,
        POPUP_TEXT_DIALOGUE,
        POPUP_TEXT_ADVICE,
        POPUP_TEXT_MESSAGE,
        POPUP_TEXT_REVIVAL
    }

    public enum gui_effect_types
    {
        GUI_EFFECT_SEARCH = 0,
        GUI_EFFECT_WAIT,
    }

    public enum yn_function_styles
    {
        YN_STYLE_GENERAL = 0,
        YN_STYLE_ITEM_EXCHANGE,
        YN_STYLE_KNAPSACK_FULL,
        YN_STYLE_MONSTER_QUESTION,
    }

    public enum special_view_types
    {
        SPECIAL_VIEW_NONE = 0,
        SPECIAL_VIEW_CHAT_MESSAGE,
        SPECIAL_VIEW_SHOW_SPECIAL_EFFECT,
        SPECIAL_VIEW_GUI_TIPS,
        SPECIAL_VIEW_CRASH_DETECTED,
        SPECIAL_VIEW_PANIC,
        SPECIAL_VIEW_DEBUGLOG,
        SPECIAL_VIEW_MESSAGE,
        MAX_SPECIAL_VIEW_TYPES
    }

    public enum context_menu_styles
    {
        CONTEXT_MENU_STYLE_GENERAL = 0,
        CONTEXT_MENU_STYLE_GETDIR,
        CONTEXT_MENU_STYLE_GETPOS,
        CONTEXT_MENU_STYLE_CLOSE_DISPLAY,
    }

    public enum getline_types
    {
        GETLINE_GENERAL = 0,
        GETLINE_EXTENDED_COMMAND,
        GETLINE_ASK_NAME,
        GETLINE_MENU_SEARCH,
        GETLINE_PARANOID,
        GETLINE_LEVELPORT,
        GETLINE_WIZ_LEVELPORT,
        GETLINE_LEVEL_CHANGE,
        GETLINE_NUMBERS_ONLY,
        GETLINE_WISHING,
        GETLINE_GENESIS,
        GETLINE_POLYMORPH,
        GETLINE_GENOCIDE,
        GETLINE_MONSTER,
        GETLINE_MONSTER_CLASS,
        GETLINE_TUNE,
        GETLINE_QUESTION,
    }



    public enum downloaded_file_check_results
    {
        OK = 0,
        NoSecretsFile,
        FileNotInSecretsList,
        VerificationFailed,
        FileDoesNotExist,
        FileAlreadyExists,
        Cancelled,
    }

    [Flags]
    public enum secrets_flags : int
    {
        None = 0x00
    }


    public class GHConstants
    {
        public const int InputBufferLength = 32;
        public const int BUFSZ = 256;
        public const int MaxGHWindows = 32;
        public const int MapCols = 80;
        public const int MapRows = 21;
        public const int DefaultAnimationInterval = 25;
        public const int MaxMessageHistoryLength = 256;
        public const float MoveDistanceThreshold = 25.0f;
        public const long MoveOrPressTimeThreshold = 200; /* Milliseconds */
        public const long SwipeTimeThreshold = 300; /* Milliseconds */
        public const double MoveByHoldingDownThreshold = 0.20; /* Seconds */
        public const double LongMenuTapThreshold = 0.75; /* Seconds */
        public const int TileWidth = 64;
        public const int TileHeight = 96;
        public const int StatusMarkWidth = 16;
        public const int StatusMarkHeight = 16;
        public const int MaxTileSheets = 4;
        public const int NumberOfTilesPerSheet = 16224;
        public const int MaxSoundParameters = 10;
        public const int MaxFramesPerAnimation = 32;
        public const int NumPositionsInEnlargement = 5;
        public const int MaxTilesPerReplacement = 32;
        public const int MaxLeashed = 2;
        public const int MaxPlayedSpecialEffects = 12;
        public const int MaxPlayedZapAnimations = 16;
        public const int MaxNormalImmediateSoundInstances = 64;
        public const int MaxLongImmediateSoundInstances = 64;
        //public const int DefaultPanTime = 5;
        public const int PIT_BOTTOM_BORDER = 2;
        public const int SPECIAL_HEIGHT_IN_PIT = -32;
        public const int SPECIAL_HEIGHT_LEVITATION = 32;
        public const float OBJECT_PIT_SCALING_FACTOR = 0.75f;
        //public const long MoveIntervals = 3;
        public const int NUM_BUFF_BIT_ULONGS = 7;
        public const int BUFFS_PER_TILE = 24;
        public const int MAX_PROPS = 167;
        public const int LAST_PROP = (MAX_PROPS - 1);
        public const int MaxObjectsDrawn = 12;
        public const int OBJECT_PILE_START_HEIGHT = 2;
        public const int OBJECT_PILE_HEIGHT_DIFFERENCE = 2;
        public const float MinimumMapMiniRelativeFontSize = 0.5f;
        public const float MaximumMapMiniRelativeFontSize = 2.0f;
        public const float MinimumMapFontSize = 4.0f;
        public const float MaximumMapFontSize = 500.0f;
        public const float MapFontDefaultSize = 72.0f;
        public const float MapFontRelativeAlternateSize = 7.0f / 16.0f;
        public const int DefaultMessageRows = 5;
        public const int AllMessageRows = 100;
        public const int DefaultPetRows = 2;
        public const int IntroGHSound = 1;
        public const string IntroEventPath = "event:/Music/Start/Splash";
        public const int IntroBankId = 0;
        public const int ButtonClickGHSound = 28;
        public const string ButtonClickEventPath = "event:/UI/Button Down";
        public const int ButtonClickBankId = 0;
        public const float ButtonClickVolume = 0.15f;
        public const int MenuSelectGHSound = 29;
        public const string MenuSelectEventPath = "event:/UI/Menu Select";
        public const int MenuSelectBankId = 0;
        public const float MenuSelectVolume = 0.15f;
#if DEBUG
        public const bool DefaultDeveloperMode = true;
#else
        public const bool DefaultDeveloperMode = false;
#endif
        public const int NUM_ZAP_SOURCE_BASE_DIRS = 8;
        public const int NUM_ZAP_SOURCE_DIRS = (GHConstants.NUM_ZAP_SOURCE_BASE_DIRS * 2);
        public const int NUM_CHAIN_SOURCE_DIRS = 4;
        public const int NUM_WORM_SOURCE_DIRS = 4;
        public const float ArrowButtonSize = 0.25f;
        public const float NumberButtonSize = 0.3f;
        public const float MoveArrowButtonSize = 0.35f;
        public const int MoreButtonPages = 4;
        public const int MoreButtonsPerRow = 4;
        public const int MoreButtonsPerColumn = 6;
        public const float BackgroundBorderDivisor = 8.0f;
        public const float BackgroundTopBorderExtraDivisor = 4.0f;
        public const bool DownloadFromWebInDebugMode = true;
        public const bool IsDefaultStatusBarClassic = false;
        public const string GnollHackGitHubPage = "https://github.com/hyvanmielenpelit/GnollHack";
        public const string GnollHackWebPage = "https://gnollhack.com";
        public const string GnollHackWikiPage = "https://github.com/hyvanmielenpelit/GnollHack/wiki";
        public const string GnollHackSponsorPage = "https://hyvanmielenpelit.fi/tule-mukaan/pienkannatusjaseneksi/in-english";
        public const string GHSettingsResourcePath = "GnollHackClient.Assets.ghsettings.json";
        public const int MainCanvasAnimationFrequency = 40;
        public const uint MainCanvasAnimationInterval = 25;
        public const int GameAnimationRefreshRate = 40;
        public const int MaxRefreshRate = 120;
        public const int PollingFrequency = 60;
        public const int PollingInterval = 15;
        public const bool IsGPUDefault = true;
        public const double DefaultTextWindowMaxWidth = 600.0;
        public const double WindowHideIntervals = 3.0;
        public const uint MainCanvasAnimationTime = 250;
        public const uint AuxiliaryCanvasAnimationTime = 240;
        public const float MenuDefaultRowHeight = 30.0f;
        public const float DefaultGeneralVolume = 1.0f;
        public const float DefaultMusicVolume = 0.5f;
        public const float DefaultAmbientVolume = 0.5f;
        public const float DefaultDialogueVolume = 0.5f;
        public const float DefaultEffectsVolume = 0.5f;
        public const float DefaultUIVolume = 0.5f;
        public const bool DefaultMapNoClipMode = false;
        public const bool DefaultMapAlternateNoClipMode = false;
        public const bool DefaultZoomChangeCenterMode = false;
        public const bool DefaultHideNavigation = true;
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct GHSelectedItem
    {
        public IntPtr Identifier;
        public int Count;
    }

    public enum CanvasTypes
    {
        General = 0,
        MainCanvas,
        CommandCanvas,
        MenuCanvas,
        TextCanvas,
        TipCanvas,
    }


}

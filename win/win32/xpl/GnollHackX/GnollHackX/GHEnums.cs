using System;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    [Flags]
    public enum GHSaveFlags : uint
    {
        None = 0x00,
        NonScoring = 0x01,
        TournamentMode = 0x02,
        FileTrackValid = 0x04,
        DebugMode = 0x100,
        ExploreMode = 0x200,
        ModernMode = 0x400,
        CasualMode = 0x800,
        NonTrackingMask = (NonScoring | DebugMode | ExploreMode | CasualMode)
    }

    [Flags]
    public enum ReplayVersionFlags : ulong
    {
        None = 0UL,
        HasOffsetData = 1UL,
    }

    /* Colors */
    public enum NhColor
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

    public enum NhAltColor
    {
        CLR_ALT_BLACK = 0,
        CLR_ALT_RED,
        CLR_ALT_GREEN,
        CLR_ALT_GOLD,   /* Gold instead of brown */
        CLR_ALT_BLUE,
        CLR_ALT_MAGENTA,
        CLR_ALT_CYAN,
        CLR_ALT_GRAYED_OUT,
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
        None =          0x00000000,
        Bold =          0x00000001,
        Dim =           0x00000002,
        Underline =     0x00000004,
        Blink =         0x00000005,
        Inverse =       0x00000007,
        AttrMask =      0x0000000F,
        Urgent =        0x00000010,
        NoHistory =     0x00000020,
        StayOnLine =    0x00000040,
        NoTabs =        0x00000080,
        Title =         0x00000100,
        Heading =       0x00000200,
        Sub =           0x00000400,
        IndentDash =    0x00000800,
        IndentColon =   0x00001000,
        IndentAstr =    0x00002000,
        IndentSpace =   0x00004000,
        IndentBracket = 0x00008000,
        IndentPeriod =  0x00001800,
        IndentPlus =    0x00002800,
        IndentDoubleSpace = 0x00004800,
        IndentBracketOrDoubleSpace = 0x0000C800,
        IndentMask =    0x0000F800,
        AlignCenter =   0x00010000,
        AlignRight =    0x00020000,
        Inactive =      0x00040000,
        HalfSize =      0x00080000,
        AltColors =     0x00100000,
        AltDivisors =   0x00200000,
        OrderedList =   0x00400000,
        StartTable =    0x00800000,
        EndTable =      0x01000000,
        TableRow =      0x02000000,
        TableHeader =   0x04000000,
        StartTableBody =0x08000000,
        EndTableBody =  0x10000000,
        ParagraphLine = 0x20000000,
        LineAttrMask =  0x7FFFFFF0,
    }

    [Flags]
    public enum MapSpecial : UInt64
    {
        None =              0x00000000UL,
        Corpse =            0x00000001UL,
        Invisible =         0x00000002UL,
        Detected =          0x00000004UL,
        Pet =               0x00000008UL,
        Ridden =            0x00000010UL,
        Statue =            0x00000020UL,
        ObjectPile =        0x00000040UL,
        BlackWhiteLava =    0x00000080UL,
        Peaceful =          0x00000100UL,
        Saddled =           0x00000200UL,
        Female =            0x00000400UL,
        HorizontalFlip =    0x00000800UL,
        StoneInverse =      0x00001000UL,
        AltColors =         0x00002000UL,
        Decoration =        0x00004000UL,
        Carpet =            0x00008000UL,

        Inverse =           0x10000000UL,
        Underline =         0x20000000UL,
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
        MENU_FLAGS_USE_NUM_ITEMS =              0x00000200,
        MENU_FLAGS_BUTTON_STYLE =               0x00000400,
        MENU_FLAGS_AUTO_CLICK_OK =              0x00000800,
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
        MapFPS72,
        MapFPS80,
        MapFPS90,
        MapFPS120,
        MapFPS144,
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
        Default = 0,
        DefClickRole = 1,

        ClickPrimary = 1,
        ClickSecondary,
        ClickTertiary,

        ClickOff = 3,
        ClickLook,
        ClickMove,
        ClickCast,
        ClickFire,
        ClickZap,
        ClickPole,
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
        None =                  0x00000000UL,
        SetWinCaps =            0x00000001UL,
        WizardMode =            0x00000002UL,
        FullVersion =           0x00000004UL,
        ModernMode =            0x00000008UL, /* Upon death, the character teleports back to starting altar */
        CasualMode =            0x00000010UL, /* Save games are preserved */
        DisableBones =          0x00000020UL, /* Force flags.bones to off */
        ForceLastPlayerName =   0x00000040UL, /* Use LastUsedPlayerName as preset player name */
        PlayingReplay =         0x00000080UL, /* Game is a replay */
        TournamentMode =        0x00000100UL, /* Playing with server-like settings */
        GUIDebugMode =          0x00000200UL, /* GUI has been built in debug mode (not a release mode game?) */
        CharacterClickAction =  0x00000400UL, /* Set character-click action to true by default */
        NoPet =                 0x00000800UL, /* Same as pettype:none in options file */
        DiceAsRanges =          0x00001000UL, /* Set show_dice_as_ranges to true by default */
        GetPositionArrows =     0x00002000UL, /* Show move arrows by default in getpos (i.e., turn travel mode off) */
        SaveFileTrackingSupported = 0x00004000UL, /* Save file tracking is supported (always on modern version) */
        SaveFileTrackingNeeded = 0x00008000UL, /* Save file tracking is needed (= is desktop) */
        SaveFileTrackingOn =    0x00010000UL, /* Save file tracking is switched on */
        AutoDig =               0x00020000UL, /* Set autodig to true by default */
        IgnoreStopping =        0x00040000UL, /* Set ignore_stopping to true by default */
        DefaultVIKeys =         0x00080000UL, /* VI Keys are default instead of number pad movement */
        WornShowsEquipment =    0x00100000UL, /* Show the equipment screen of inventory instead of only worn items in normal menu */
        MetricSystem =          0x00200000UL, /* Set metric_system to true by default */
        DamageFormula =         0x00400000UL, /* Set show_damage_formula to true by default */
    }

    [Flags]
    public enum StopSoundFlags : uint
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
         LFLAGS_L_LEGAL =           0x00000001UL,
         LFLAGS_L_ILLEGAL =         0x00000002UL,
         LFLAGS_L_ENGRAVING =       0x00000004UL,

         LFLAGS_V_XRAY_VISION =     0x00000010UL,
         /* Free bits */

         /* Reserved */
         LFLAGS_O_CHAIN =           0x00002000UL,
         LFLAGS_O_PILE =            0x00004000UL,
         LFLAGS_O_IN_PIT =          0x00008000UL,
         LFLAGS_O_MASK =            0x0000F000UL,
         
         /* Reserved */
         LFLAGS_E_GEN_FADE_OUT =    0x00020000UL,
         LFLAGS_E_BKG_FADE_IN =     0x00040000UL,
         LFLAGS_E_BKG_FADE_OUT =    0x00080000UL,

         LFLAGS_ZAP_LEADING_EDGE =  0x00100000UL, /* First (leading) tile in a zap */
         LFLAGS_ZAP_TRAILING_EDGE = 0x00200000UL, /* Last (trailing) tile in a zap */
         LFLAGS_ZAP_MASK = (LFLAGS_ZAP_LEADING_EDGE | LFLAGS_ZAP_TRAILING_EDGE),

         LFLAGS_T_TRAPPED =         0x00400000UL,

         LFLAGS_C_DECORATION =      0x00800000UL,
         LFLAGS_C_CARPET =          0x01000000UL,

         LFLAGS_SHOWING_MEMORY =    0x02000000UL, /* also implies that you cannot see the location (and hence showing memory) */
         LFLAGS_SHOWING_DETECTION = 0x04000000UL, /* Do not darken */
         LFLAGS_ASCENSION_RADIANCE =0x08000000UL, /* Lighten up */
         LFLAGS_CAN_SEE =           0x10000000UL, /* cansee(x, y) is true (if not, then darken etc.) */
         LFLAGS_UXUY =              0x20000000UL, /* x == u.ux && y == u.uy is true */
         LFLAGS_APPEARS_UNLIT =     0x40000000UL, /* looks unlit */
         LFLAGS_NO_WALL_END_AUTODRAW=0x80000000UL,/* NO_WALL_END_AUTODRAW(x, y) is true */
    }

    [Flags]
    public enum LayerMonsterFlags : ulong
    {
        None =                              0x00000000UL,
        LMFLAGS_PET =                       0x00000001UL,
        LMFLAGS_PEACEFUL =                  0x00000002UL,
        LMFLAGS_RIDDEN =                    0x00000004UL,
        LMFLAGS_DETECTED =                  0x00000008UL,
        LMFLAGS_CANSPOTMON =                0x00000010UL,
        LMFLAGS_YOU =                       0x00000020UL,
        LMFLAGS_BEING_HIT =                 0x00000040UL,
        LMFLAGS_KILLED =                    0x00000080UL,
        LMFLAGS_U_TETHERED =                0x00000100UL, /* You are tethered */
        LMFLAGS_TETHERED =                  0x00000200UL, /* Monster is tethered */
        LMFLAGS_DROPPING_PIERCER =          0x00000400UL,
        LMFLAGS_WORM_TAIL =                 0x00000800UL,
        LMFLAGS_WORM_SEEN =                 0x00001000UL,
        LMFLAGS_WORM_HEAD =                 0x00002000UL,
        LMFLAGS_WORM_TAILEND =              0x00004000UL,
        LMFLAGS_INVISIBLE_TRANSPARENT =     0x00008000UL,
        LMFLAGS_SEMI_TRANSPARENT =          0x00010000UL,
        LMFLAGS_RADIAL_TRANSPARENCY =       0x00020000UL,
        LMFLAGS_GLASS_TRANSPARENCY =        0x00040000UL,
        LMFLAGS_CAN_SPOT_SELF =             0x00080000UL,
        LMFLAGS_LONG_WORM_WITH_TAIL =       0x00100000UL,
        LMFLAGS_LONG_WORM_TAIL =            0x00200000UL,
        LMFLAGS_SLIGHT_TRANSPARENCY =       0x00400000UL,
        LMFLAGS_FADES_UPON_DEATH =          0x00800000UL,
        LMFLAGS_FLYING =                    0x01000000UL,
        LMFLAGS_LEVITATING =                0x02000000UL,
        LMFLAGS_BLOBBY_ANIMATION =          0x04000000UL,
        LMFLAGS_SWIM_ANIMATION =            0x08000000UL,
        LMFLAGS_SPECIAL_ANIMATION =         0x10000000UL,
        LMFLAGS_HUMAN_BREATHE_ANIMATION =   0x20000000UL,
        LMFLAGS_ANIMAL_BREATHE_ANIMATION =  0x40000000UL,
        LMFLAGS_STONED =                    0x80000000UL,
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
        MISSILE_FLAGS_LIT =          0x00000080UL,
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
        GLYPH_TILE_FLAG_HEIGHT_IS_CLIPPING = 0x80,
    }

    public enum layer_types
    {
        LAYER_FLOOR = 0,
        LAYER_CARPET,
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
        GHWINDOW_STYLE_HAS_INDENTED_TEXT,
        GHWINDOW_STYLE_DISPLAY_FILE_WITH_INDENTED_TEXT,
        MAX_GHWINDOW_STYLES
    }

    /* GHMenu styles */
    public enum ghmenu_styles : int
    {
        GHMENU_STYLE_GENERAL = 0,
        GHMENU_STYLE_INVENTORY,
        GHMENU_STYLE_PERMANENT_INVENTORY,
        GHMENU_STYLE_OTHERS_INVENTORY,
        GHMENU_STYLE_PICK_ITEM_LIST,
        GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK,
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
        GHMENU_STYLE_DELETE_SAVED_GAME,
        GHMENU_STYLE_START_GAME_MENU,
        GHMENU_STYLE_PREVIOUS_MESSAGES,
        GHMENU_STYLE_VIEW_FILE,
        GHMENU_STYLE_ACCEPT_PLAYER,
        GHMENU_STYLE_VIEW_SPELL,
        GHMENU_STYLE_VIEW_SPELL_ALTERNATE,
        GHMENU_STYLE_INVENTORY_EQUIPMENT,
        MAX_GHMENU_STYLES
    }

    public enum floating_text_types
    {
        FLOATING_TEXT_GENERAL = 0,
        FLOATING_TEXT_HEALING,
        FLOATING_TEXT_DAMAGE,
        FLOATING_TEXT_GOLD_ACQUIRED,
        FLOATING_TEXT_GOLD_REDUCED,
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
        SCREEN_TEXT_SNAPSHOT,
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
        CURSOR_STYLE_RESERVED_1,
        CURSOR_STYLE_RESERVED_2,
        CURSOR_STYLE_RESERVED_3,
        CURSOR_STYLE_RESERVED_4,
        CURSOR_STYLE_RESERVED_5,
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
        HIT_RESERVED_1,
        HIT_RESERVED_2,
        HIT_RESERVED_3,
        HIT_RESERVED_4,
        HIT_RESERVED_5,
        HIT_RESERVED_6,
        HIT_RESERVED_7,
        HIT_RESERVED_8,
        HIT_RESERVED_9,
        HIT_RESERVED_10,
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
        GENERAL_TILE_RESERVED_1,
        GENERAL_TILE_RESERVED_2,
        MAX_GENERAL_TILES
    }

    public enum game_ui_tile_types
    {
        GENERAL_UI_ELEMENTS = 0,        /* Check box etc. */
        STATUS_MARKS,                   /* Pet mark, detection mark, ridden mark, pile mark, etc. */
        STATUS2_MARKS,                  /* More status marks */
        CONDITION_MARKS,                /* One tile for each specified condition (the same as on status line) */
        CONDITION2_MARKS,               /* More condition marks */
        MAIN_TILE_MARK,
        U_TILE_MARK,
        ITEM_AUTODRAW_GRAPHICS,
        ITEM2_AUTODRAW_GRAPHICS,
        ITEM_PROPERTY_MARKS,
        ITEM2_PROPERTY_MARKS,
        JAR_GRAPHICS,
        JAR_FOREGROUND,
        JAR2_GRAPHICS,
        JAR3_GRAPHICS,
        RESERVED_UI_TILE_1,
        MAX_UI_TILES
    }

    public enum NhStatusFields
    {
        BL_CHARACTERISTICS = -3, /* alias for BL_STR..BL_CH */
        BL_RESET = -2,           /* Force everything to redisplay */
        BL_FLUSH = -1,           /* Finished cycling through bot fields */
        BL_TITLE = 0,
        BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH, BL_GOLD, BL_ALIGN,
        BL_SCORE, BL_CAP, BL_ENE, BL_ENEMAX,
        BL_XP, BL_2WEP, BL_SKILL, BL_AC, BL_MC_LVL, BL_MC_PCT, BL_MOVE, BL_UWEP, BL_UWEP2, BL_UQUIVER, BL_HD, BL_TIME, BL_REALTIME, BL_HUNGER, BL_HP,
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
        BL_COND_WOUNDED_LEGS,
        NUM_BL_CONDITIONS
    }

    public enum game_ui_status_mark_types
    {
        STATUS_MARK_PET = 0,
        STATUS_MARK_PEACEFUL,
        STATUS_MARK_DETECTED,
        STATUS_MARK_BOUNTY,
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
        STATUS_MARK_EATING,
        STATUS_MARK_FROZEN,
        STATUS_MARK_HIGH_LEVEL,
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

    public enum ocdata_flag_types : byte
    {
        OCDATA_BIMANUAL = 0x01,
        OCDATA_WIELDED_WEAPON = 0x02,
        OCDATA_QUIVERABLE = 0x04,
        OCDATA_BLINDFOLD = 0x08,
        OCDATA_APPLIABLE_POLEARM = 0x10,
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
        ART_CLASS = 20,

        MAX_OBJECT_CLASSES = 21
    }
    public enum obj_armor_types : sbyte
    {
        ARM_SUIT = 0,
        ARM_SHIELD = 1,        /* needed for special wear function */
        ARM_HELM = 2,
        ARM_GLOVES = 3,
        ARM_BOOTS = 4,
        ARM_CLOAK = 5,
        ARM_SHIRT = 6,
        ARM_ROBE = 7,
        ARM_BRACERS = 8,
        MAX_ARMOR_TYPES
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
        OBJ_MEMORY = 8,    /* object remembered by hero */
        OBJ_MAGIC = 9,     /* object in a magic chest */
        NOBJ_STATES = 10
    }

    [Flags]
    public enum obj_worn_flags : ulong
    {
        W_ARM  = 0x00000001UL,  /* Body armor */
        W_ARMC = 0x00000002UL,  /* Cloak */
        W_ARMH = 0x00000004UL,  /* Helmet/hat */
        W_ARMS = 0x00000008UL,  /* Shield */
        W_ARMG = 0x00000010UL,  /* Gloves/gauntlets */
        W_ARMF = 0x00000020UL,  /* Footwear */
        W_ARMU = 0x00000040UL,  /* Undershirt */
        W_ARMO = 0x00000080UL,  /* Robe or overcoat */
        W_ARMB = 0x00000100UL,  /* Bracers */

        W_ARMOR = (W_ARM | W_ARMC | W_ARMH | W_ARMS | W_ARMG | W_ARMF | W_ARMU | W_ARMO | W_ARMB),


        /* Weapons */
        W_WEP         = 0x00000200UL,        /* Wielded weapon in the primary hand */
        W_WEP2        = W_ARMS,              /* Wielded weapon in the secondary hand (uses the same hand as W_ARMS) */
        W_AUTOSTASH   = 0x00000400UL,        /* Autostash for putting items in automatically; NOT USED currently */
        W_QUIVER      = 0x00000800UL,        /* Quiver for (f)iring ammo */
        W_SWAPWEP     = 0x00001000UL,        /* Alternate weapon in the primary hand */
        W_SWAPWEP2    = 0x00002000UL,        /* Alternate weapon/shield in the secondary hand */

        W_ENVIRONMENT = 0x00004000UL,        /* Caused by environment, e.g., suffocation due to no air */
        W_STUCK       = 0x00008000UL,        /* A monster in u.ustuck had grabbed you and is e.g. constricting you */

        W_WIELDED_WEAPON = (W_WEP | W_WEP2),
        W_SWAP_WEAPON = (W_SWAPWEP | W_SWAPWEP2),
        W_WEAPON = (W_WIELDED_WEAPON | W_SWAP_WEAPON | W_QUIVER),

        /* Amulets, rings, tools, and other items */
        W_AMUL  = 0x00010000UL,      /* Amulet */
        W_RINGL = 0x00020000UL,      /* Left ring */
        W_RINGR = 0x00040000UL,      /* Right ring */
        W_RING = (W_RINGL | W_RINGR),
        W_BLINDFOLD = 0x00080000UL,  /* Eyewear */

        /* historical note: originally in slash'em, 'worn' saddle stayed in
           hero's inventory; in GnollHack, it's kept in the steed's inventory */
        W_SADDLE = 0x00100000UL,     /* KMH -- For riding monsters */
        W_BALL   = 0x00200000UL,     /* Punishment ball */
        W_CHAIN  = 0x00400000UL,     /* Punishment chain */

        /* new accessories*/
        W_MISC  = 0x00800000UL,      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
        W_MISC2 = 0x01000000UL,      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
        W_MISC3 = 0x02000000UL,      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
        W_MISC4 = 0x04000000UL,      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
        W_MISC5 = 0x08000000UL,      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */

        W_MISCITEMS = (W_MISC  | W_MISC2 | W_MISC3 | W_MISC4 | W_MISC5),
        W_ACCESSORY = (W_RING | W_AMUL | W_MISCITEMS | W_BLINDFOLD),
        W_WORN_NOT_WIELDED = ((W_ARMOR & ~W_ARMS) | W_ACCESSORY),
        W_WORN_AND_WIELDED = (W_WORN_NOT_WIELDED | W_WIELDED_WEAPON),
    }

    public enum InventorySlotPictureIndices : int
    {
        General = 0,
        WeaponRight,
        WeaponLeft,
        Shield,
        SwapWeaponRight,
        SwapWeaponLeft,
        Quiver,
        Reserved,
        Suit,
        Cloak,
        Robe,
        Shirt,
        Helmet,
        Gloves,
        Bracers,
        Boots,
        Amulet,
        RingRight,
        RingLeft,
        Blindfold,
        Miscellaneous1,
        Miscellaneous2,
        Miscellaneous3,
        Miscellaneous4,
        Miscellaneous5,
        NumPictureIndices
    }

    [Flags]
    public enum speflag_types : ulong
    {
        SPEFLAGS_YOURS                         = 0x00000001UL,
        SPEFLAGS_FEMALE                        = 0x00000002UL,
        SPEFLAGS_MALE                          = 0x00000004UL,
        SPEFLAGS_FACING_RIGHT                  = 0x00000008UL,
        SPEFLAGS_SCHROEDINGERS_BOX             = 0x00000010UL,
        SPEFLAGS_INTENDED_FOR_SALE             = 0x00000020UL,
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
        SPEFLAGS_CERTAIN_WISH                  = 0x00100000UL,
        SPEFLAGS_CAUGHT_IN_LEAVES              = 0x00200000UL,
        SPEFLAGS_CLONED_ITEM                   = 0x00400000UL,
        SPEFLAGS_NO_PREVIOUS_WEAPON            = 0x00800000UL,
        SPEFLAGS_ALTERNATIVE_APPEARANCE        = 0x01000000UL, /* Alternative glyph is used for the object */
        SPEFLAGS_PREVIOUSLY_WIELDED            = 0x02000000UL,
        SPEFLAGS_FAVORITE                      = 0x04000000UL,
        SPEFLAGS_EMPTY_NOTICED                 = 0x08000000UL,
        SPEFLAGS_BEING_BROKEN                  = 0x10000000UL,
        SPEFLAGS_GIVEN_OUT_BLUE_SMOKE          = 0x20000000UL,
        SPEFLAGS_FOUND_THIS_TURN               = 0x40000000UL,
        SPEFLAGS_HAS_BEEN_PICKED_UP_BY_HERO    = 0x80000000UL,
    }

    [Flags]
    public enum obj_bitfield_types : ulong
    {
        OBJFLAG_CURSED                   = 0x00000001UL,
        OBJFLAG_BLESSED                  = 0x00000002UL,
        OBJFLAG_UNPAID                   = 0x00000004UL,
        OBJFLAG_NO_CHARGE                = 0x00000008UL,
        OBJFLAG_KNOWN                    = 0x00000010UL,
        OBJFLAG_DKNOWN                   = 0x00000020UL,
        OBJFLAG_BKNOWN                   = 0x00000040UL,
        OBJFLAG_RKNOWN                   = 0x00000080UL,
        OBJFLAG_OERODEPROOF              = 0x00000100UL,
        OBJFLAG_OLOCKED                  = 0x00000200UL,
        OBJFLAG_OBROKEN                  = 0x00000400UL,
        OBJFLAG_OTRAPPED                 = 0x00000800UL,
        OBJFLAG_LAMPLIT                  = 0x00001000UL,
        OBJFLAG_MAKINGSOUND              = 0x00002000UL,
        OBJFLAG_GLOBBY                   = 0x00004000UL,
        OBJFLAG_GREASED                  = 0x00008000UL,
        OBJFLAG_NOMERGE                  = 0x00010000UL,
        OBJFLAG_WAS_THROWN               = 0x00020000UL,
        OBJFLAG_HAS_SPECIAL_TILESET      = 0x00040000UL,
        OBJFLAG_IN_USE                   = 0x00080000UL,
        OBJFLAG_BYPASS                   = 0x00100000UL,
        OBJFLAG_CKNOWN                   = 0x00200000UL,
        OBJFLAG_LKNOWN                   = 0x00400000UL,
        OBJFLAG_TKNOWN                   = 0x00800000UL,
        OBJFLAG_NKNOWN                   = 0x01000000UL,
        OBJFLAG_AKNOWN                   = 0x02000000UL,
        OBJFLAG_MKNOWN                   = 0x04000000UL,
        OBJFLAG_ROTKNOWN                 = 0x08000000UL
    }

    [Flags]
    public enum objdata_flags : ulong
    {
        None = 0x00000000UL,
        OBJDATA_FLAGS_DRAWN_IN_FRONT =  0x00000001UL,
        OBJDATA_FLAGS_HALLUCINATION =   0x00000002UL,
        OBJDATA_FLAGS_UCHAIN =          0x00000004UL,
        OBJDATA_FLAGS_UBALL =           0x00000008UL,
        OBJDATA_FLAGS_UWEP =            0x00000010UL,
        OBJDATA_FLAGS_UWEP2 =           0x00000020UL,
        OBJDATA_FLAGS_UQUIVER =         0x00000040UL,
        OBJDATA_FLAGS_OUT_OF_AMMO1 =    0x00000080UL,
        OBJDATA_FLAGS_WRONG_AMMO_TYPE1 =0x00000100UL,
        OBJDATA_FLAGS_NOT_BEING_USED1 = 0x00000200UL,
        OBJDATA_FLAGS_NOT_WEAPON1 =     0x00000400UL,
        OBJDATA_FLAGS_OUT_OF_AMMO2 =    0x00000800UL,
        OBJDATA_FLAGS_WRONG_AMMO_TYPE2 =0x00001000UL,
        OBJDATA_FLAGS_NOT_BEING_USED2 = 0x00002000UL,
        OBJDATA_FLAGS_NOT_WEAPON2 =     0x00004000UL,
        OBJDATA_FLAGS_FOUND_THIS_TURN = 0x00008000UL,
        OBJDATA_FLAGS_IS_AMMO =         0x00010000UL, /* is_ammo is TRUE */
        OBJDATA_FLAGS_THROWING_WEAPON = 0x00020000UL, /* throwing_weapon is TRUE */
        OBJDATA_FLAGS_PREV_WEP_FOUND =  0x00040000UL,
        OBJDATA_FLAGS_PREV_UNWIELD =    0x00080000UL,
    }

    [Flags]
    public enum sound_play_flags : uint
    {
        None = 0x00000000U,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING = 0x00000001U,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_QUEUED =  0x00000002U,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED = 0x00000003U,
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
        GUI_CMD_FADE_TO_BLACK,
        GUI_CMD_COLLECT_GARBAGE,
        GUI_CMD_FADE_FROM_BLACK,
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
        GUI_CMD_DISABLE_WIZARD_MODE,
        GUI_CMD_ENABLE_CASUAL_MODE,
        GUI_CMD_DISABLE_CASUAL_MODE,
        GUI_CMD_CLEAR_PET_DATA,
        GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE,
        GUI_CMD_RESTORE_TRAVEL_MODE,
        GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE_ON_LEVEL,
        GUI_CMD_RESTORE_TRAVEL_MODE_ON_LEVEL,
        GUI_CMD_CLEAR_CONDITION_TEXTS,
        GUI_CMD_CLEAR_FLOATING_TEXTS,
        GUI_CMD_CLEAR_GUI_EFFECTS,
        GUI_CMD_CLEAR_MESSAGE_HISTORY,
        GUI_CMD_LOAD_INTRO_SOUND_BANK,
        GUI_CMD_UNLOAD_INTRO_SOUND_BANK,
        GUI_CMD_WAIT_FOR_RESUME,
        GUI_CMD_REPORT_PLAY_TIME,
        GUI_CMD_POST_GAME_STATUS,
        GUI_CMD_POST_DIAGNOSTIC_DATA,
        GUI_CMD_POST_XLOG_ENTRY,
        GUI_CMD_POST_BONES_FILE,
        GUI_CMD_LIBRARY_MANUAL,
        GUI_CMD_DEBUGLOG,
        GUI_CMD_GAME_START,
        GUI_CMD_GAME_ENDED,
        GUI_CMD_ENABLE_TOURNAMENT_MODE,
        GUI_CMD_DISABLE_TOURNAMENT_MODE,
        GUI_CMD_TOGGLE_MENU_POSITION_SAVING,
        GUI_CMD_TOGGLE_CHARACTER_CLICK_ACTION,
        GUI_CMD_SET_TO_BLACK,
        GUI_CMD_FADE_FROM_BLACK_SLOWLY_NONBLOCKING,
        GUI_CMD_REPORT_MOUSE_COMMAND,
        GUI_CMD_TOGGLE_QUICK_ZAP_WAND,
        GUI_CMD_TOGGLE_QUICK_CAST_SPELL,
        GUI_CMD_TOGGLE_DICE_AS_RANGES,
        GUI_CMD_ZOOM_NORMAL,
        GUI_CMD_ZOOM_IN,
        GUI_CMD_ZOOM_OUT,
        GUI_CMD_ZOOM_MINI,
        GUI_CMD_ZOOM_HALF,
        GUI_CMD_ZOOM_TO_SCALE,
        GUI_CMD_SAVE_ZOOM,
        GUI_CMD_RESTORE_ZOOM,
        GUI_CMD_TOGGLE_GETPOS_ARROWS,
        GUI_CMD_DELETE_TRACKING_FILE,
        GUI_CMD_KEYBOARD_FOCUS,
        GUI_CMD_ORACLE_MAJOR_CONSULTATION,
        GUI_CMD_TOGGLE_AUTODIG,
        GUI_CMD_TOGGLE_IGNORE_STOPPING,
        GUI_CMD_EXIT_APP_ON_MAIN_SCREEN,
        GUI_CMD_BREADCRUMB,
        GUI_CMD_BREADCRUMB2,
        GUI_CMD_BREADCRUMB3,
        GUI_CMD_TOGGLE_WORN_SHOWS_EQUIPMENT,
        GUI_CMD_TOGGLE_NO_PET,
        GUI_CMD_REPORT_COMMANDS,
        GUI_CMD_GAME_ENTERED_MOVELOOP,
        GUI_CMD_ACHIEVEMENT,
        GUI_CMD_REPORT_ENGRAVE_QUICK_TEXT,
        GUI_CMD_REPORT_ENGRAVE_QUICK_STYLE,
        GUI_CMD_TOGGLE_METRIC_SYSTEM,
        GUI_CMD_TOGGLE_DAMAGE_FORMULA,
    }

    public enum gui_achievement_categories
    {
        Gameplay = 0,
        Combat,
        Exploration,
        Playthrough,
        Ascension,

        NumberOfCategories
    }

    public enum gui_achievement_types
    {
        /* Gameplay: Base */
        GUI_ACHIEVEMENT_PLAYED_GAME_IN_CLASSIC_MODE = 1,

        /* Gameplay: Strategies */
        GUI_ACHIEVEMENT_IDENTIFIED_AN_ITEM = 32,
        GUI_ACHIEVEMENT_USED_WAND_OF_PROBING,
        GUI_ACHIEVEMENT_ENGRAVED_ELBERETH,
        GUI_ACHIEVEMENT_USED_SCARE_MONSTER,
        GUI_ACHIEVEMENT_CONSULTED_ORACLE,
        GUI_ACHIEVEMENT_MADE_A_WISH,
        GUI_ACHIEVEMENT_RESERVED_1,
        GUI_ACHIEVEMENT_BROKE_CHEST_LOCK_BY_KICKING,
        GUI_ACHIEVEMENT_DISARMED_TRAP,
        GUI_ACHIEVEMENT_FOUND_SECRET_DOOR_OR_PASSAGE,
        GUI_ACHIEVEMENT_POLYMORPHED_FORM,
        GUI_ACHIEVEMENT_GENOCIDED_MONSTERS,

        /* Gameplay: Religious strategies */
        GUI_ACHIEVEMENT_IDENTIFIED_BLESSEDNESS_ON_ALTAR = 64,
        GUI_ACHIEVEMENT_PRAYED,
        GUI_ACHIEVEMENT_SACRIFICED,
        GUI_ACHIEVEMENT_WAS_GIFTED_ARTIFACT,
        GUI_ACHIEVEMENT_WAS_CROWNED,
        GUI_ACHIEVEMENT_CREATED_HOLY_WATER_ON_ALTAR,
        GUI_ACHIEVEMENT_CONVERTED_ALTAR,
        GUI_ACHIEVEMENT_DETERMINED_CURSED_STATUS_WITH_PET,

        /* Gameplay: Experience level and stats */
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_5 = 128,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_10,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_15,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_20,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_25,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_30,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_35,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_40,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_45,
        GUI_ACHIEVEMENT_REACHED_EXPERIENCE_LEVEL_50,
        GUI_ACHIEVEMENT_AC_0_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_10_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_20_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_30_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_40_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_50_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_75_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_100_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_125_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_150_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_175_OR_BELOW,
        GUI_ACHIEVEMENT_AC_MINUS_200_OR_BELOW,
        GUI_ACHIEVEMENT_MC_10_OR_MORE,
        GUI_ACHIEVEMENT_MC_15_OR_MORE,
        GUI_ACHIEVEMENT_MC_20_OR_MORE,
        GUI_ACHIEVEMENT_MC_25_OR_MORE,
        GUI_ACHIEVEMENT_HP_100_OR_MORE,
        GUI_ACHIEVEMENT_HP_200_OR_MORE,
        GUI_ACHIEVEMENT_HP_500_OR_MORE,
        GUI_ACHIEVEMENT_HP_1000_OR_MORE,
        GUI_ACHIEVEMENT_HP_1500_OR_MORE,
        GUI_ACHIEVEMENT_HP_2000_OR_MORE,
        GUI_ACHIEVEMENT_STR_20_OR_MORE,
        GUI_ACHIEVEMENT_DEX_20_OR_MORE,
        GUI_ACHIEVEMENT_CON_20_OR_MORE,
        GUI_ACHIEVEMENT_INT_20_OR_MORE,
        GUI_ACHIEVEMENT_WIS_20_OR_MORE,
        GUI_ACHIEVEMENT_CHA_20_OR_MORE,
        GUI_ACHIEVEMENT_20_OR_MORE_IN_ALL_STATS,
        GUI_ACHIEVEMENT_STR_25,
        GUI_ACHIEVEMENT_DEX_25,
        GUI_ACHIEVEMENT_CON_25,
        GUI_ACHIEVEMENT_INT_25,
        GUI_ACHIEVEMENT_WIS_25,
        GUI_ACHIEVEMENT_CHA_25,
        GUI_ACHIEVEMENT_25_IN_ALL_STATS,

        /* Gameplay: Props */
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_VERY_FAST_SPEED = 192,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_ULTRA_FAST_SPEED,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_SUPER_FAST_SPEED,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_LIGHTNING_FAST_SPEED,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_HEROISM,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_SUPER_HEROISM,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_CONFLICT,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_REFLECTION,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_MAGIC_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_POISON_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_DEATH_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_DISINTEGRATION_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_SLEEP_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_PARALYSIS_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_SICKNESS_RESISTANCE,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_OILSKIN,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_CEREBRAL_SAFEGUARDING,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_FLYING,
        GUI_ACHIEVEMENT_PROPS_ACQUIRED_LIFE_SAVING,

        /* Combat: Defeated a monster */
        GUI_ACHIEVEMENT_DEFEATED_MEDUSA = 256,
        GUI_ACHIEVEMENT_DEFEATED_QUEST_NEMESIS,
        GUI_ACHIEVEMENT_DEFEATED_VLAD_THE_IMPALER,
        GUI_ACHIEVEMENT_DEFEATED_WIZARD_OF_YENDOR,
        GUI_ACHIEVEMENT_DEFEATED_YACC,
        GUI_ACHIEVEMENT_DEFEATED_MODRON_PRIMUS,
        GUI_ACHIEVEMENT_DEFEATED_HIGH_PRIEST_OF_MOLOCH,
        GUI_ACHIEVEMENT_DEFEATED_DEMOGORGON,
        GUI_ACHIEVEMENT_DEFEATED_ASMODEUS,
        GUI_ACHIEVEMENT_DEFEATED_DISPATER,
        GUI_ACHIEVEMENT_DEFEATED_BAALZEBUB,
        GUI_ACHIEVEMENT_DEFEATED_ORCUS,
        GUI_ACHIEVEMENT_DEFEATED_YEENAGHU,
        GUI_ACHIEVEMENT_DEFEATED_JUBILEX,
        GUI_ACHIEVEMENT_DEFEATED_GERYON,
        GUI_ACHIEVEMENT_DEFEATED_BAPHOMET,
        GUI_ACHIEVEMENT_DEFEATED_ALL_DEMON_PRINCES,
        GUI_ACHIEVEMENT_DEFEATED_TARRASQUE,
        GUI_ACHIEVEMENT_DEFEATED_DEATH,
        GUI_ACHIEVEMENT_DEFEATED_PESTILENCE,
        GUI_ACHIEVEMENT_DEFEATED_FAMINE,
        GUI_ACHIEVEMENT_DEFEATED_ALL_RIDERS,
        GUI_ACHIEVEMENT_DEFEATED_CERBERUS,
        GUI_ACHIEVEMENT_DEFEATED_AMONKET,
        GUI_ACHIEVEMENT_DEFEATED_CROESUS,
        GUI_ACHIEVEMENT_DEFEATED_GARGANTUAN_MIMIC,
        GUI_ACHIEVEMENT_DEFEATED_XAN,
        GUI_ACHIEVEMENT_DEFEATED_ORC_AND_A_PIE,
        GUI_ACHIEVEMENT_DEFEATED_ORC_CAPTAIN_AND_A_PIE,
        GUI_ACHIEVEMENT_DEFEATED_LICH,
        GUI_ACHIEVEMENT_DEFEATED_MASTER_LICH,
        GUI_ACHIEVEMENT_DEFEATED_ARCH_LICH,
        GUI_ACHIEVEMENT_DEFEATED_TENTACLED_ONE,
        GUI_ACHIEVEMENT_DEFEATED_ELDER_TENTACLED_ONE,

        /* Combat: Extinctinist */
        GUI_ACHIEVEMENT_KILLED_MORE_THAN_1000_MONSTERS = 320,
        GUI_ACHIEVEMENT_KILLED_MORE_THAN_2000_MONSTERS,
        GUI_ACHIEVEMENT_KILLED_MORE_THAN_3000_MONSTERS,
        GUI_ACHIEVEMENT_KILLED_MORE_THAN_4000_MONSTERS,
        GUI_ACHIEVEMENT_KILLED_MORE_THAN_5000_MONSTERS,
        GUI_ACHIEVEMENT_KILLED_MORE_THAN_10000_MONSTERS,
        GUI_ACHIEVEMENT_2_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_3_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_4_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_5_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_10_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_20_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_30_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_40_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_50_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_100_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_200_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_300_OR_MORE_SPECIES_EXTINCT,
        GUI_ACHIEVEMENT_400_OR_MORE_SPECIES_EXTINCT,

        /* Exploration: Reached depth in the main dungeon or gehennom */
        GUI_ACHIEVEMENT_REACHED_DUNGEON_LEVEL_5 = 384,
        GUI_ACHIEVEMENT_REACHED_DUNGEON_LEVEL_10,
        GUI_ACHIEVEMENT_REACHED_DUNGEON_LEVEL_15,
        GUI_ACHIEVEMENT_REACHED_DUNGEON_LEVEL_20,
        GUI_ACHIEVEMENT_REACHED_MEDUSA_ISLAND,
        GUI_ACHIEVEMENT_REACHED_CASTLE,
        GUI_ACHIEVEMENT_REACHED_VALLEY_OF_THE_DEAD,
        GUI_ACHIEVEMENT_REACHED_WIZARD_TOWER,
        GUI_ACHIEVEMENT_REACHED_MINE_TOWN,

        /* Exploration: Found a branch (32) */
        GUI_ACHIEVEMENT_ENTERED_GEHENNOM = 448,
        GUI_ACHIEVEMENT_ENTERED_GNOMISH_MINES,
        GUI_ACHIEVEMENT_ENTERED_SOKOBAN,
        GUI_ACHIEVEMENT_ENTERED_FORT_LUDIOUS,
        GUI_ACHIEVEMENT_ENTERED_LARGE_CIRCULAR_DUNGEON,
        GUI_ACHIEVEMENT_ENTERED_VLAD_TOWER,
        GUI_ACHIEVEMENT_ENTERED_HELLISH_PASTURES,
        GUI_ACHIEVEMENT_ENTERED_PLANE_OF_THE_MODRON,
        GUI_ACHIEVEMENT_ENTERED_THE_QUEST,

        /* Exploration: Reached the end of a branch (32) */
        GUI_ACHIEVEMENT_REACHED_BOTTOM_OF_GEHENNOM = 480,
        GUI_ACHIEVEMENT_REACHED_MINES_END,
        GUI_ACHIEVEMENT_REACHED_TOP_OF_SOKOBAN,
        GUI_ACHIEVEMENT_REACHED_QUANTUM_CORE,
        GUI_ACHIEVEMENT_REACHED_TOP_OF_VLAD_TOWER,
        GUI_ACHIEVEMENT_REACHED_PROTONUS,
        GUI_ACHIEVEMENT_REACHED_FINAL_QUEST_LEVEL,

        /* Playthrough: Minor playthrough achievement */
        GUI_ACHIEVEMENT_COMPLETED_THE_QUEST = 512,
        GUI_ACHIEVEMENT_COMPLETED_OPTIONAL_QUEST,
        GUI_ACHIEVEMENT_FOUND_GLADSTONE,
        GUI_ACHIEVEMENT_SOLVED_SOKOBAN,
        GUI_ACHIEVEMENT_CONQUERED_FORT_LUDIOUS,
        GUI_ACHIEVEMENT_SOLVED_CASTLE_TUNE,
        GUI_ACHIEVEMENT_FOUND_PRIME_CODEX,
        GUI_ACHIEVEMENT_REACHED_SANCTUM,

        /* Playthrough: Major playthrough achievement */
        GUI_ACHIEVEMENT_FOUND_BELL_OF_OPENING = 576,
        GUI_ACHIEVEMENT_FOUND_CANDELABRUM_OF_INVOCATION,
        GUI_ACHIEVEMENT_FOUND_BOOK_OF_THE_DEAD,
        GUI_ACHIEVEMENT_FOUND_VIBRATING_SQUARE,
        GUI_ACHIEVEMENT_PERFORMED_THE_RITUAL,
        GUI_ACHIEVEMENT_FOUND_AMULET_OF_YENDOR,
        GUI_ACHIEVEMENT_ENTERED_ELEMENTAL_PLANES,
        GUI_ACHIEVEMENT_ENTERED_ASTRAL_PLANE,

        /* 640 - 960 RESERVED */

        /* Ascension: Ascended */
        GUI_ACHIEVEMENT_ASCENDED = 1024,
        GUI_ACHIEVEMENT_ASCENDED_IN_CLASSIC_MODE,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_RESERVED_1,
        GUI_ACHIEVEMENT_ASCENDED_RESERVED_2,
        GUI_ACHIEVEMENT_ASCENDED_RESERVED_3,
        GUI_ACHIEVEMENT_ASCENDED_RESERVED_4,

        GUI_ACHIEVEMENT_ASCENDED_WITH_FOODLESS_CONDUCT,
        GUI_ACHIEVEMENT_ASCENDED_WITH_NUDIST_CONDUCT,
        GUI_ACHIEVEMENT_ASCENDED_WITH_PACIFIST_CONDUCT,
        GUI_ACHIEVEMENT_ASCENDED_WITH_ZEN_CONDUCT,
        GUI_ACHIEVEMENT_ASCENDED_WITH_CONDUCT_RESERVED_1,
        GUI_ACHIEVEMENT_ASCENDED_WITH_CONDUCT_RESERVED_2,
        GUI_ACHIEVEMENT_ASCENDED_WITH_CONDUCT_RESERVED_3,
        GUI_ACHIEVEMENT_ASCENDED_WITH_CONDUCT_RESERVED_4,

        GUI_ACHIEVEMENT_ASCENDED_WITH_AT_LEAST_ONE_MILLION_POINTS,
        GUI_ACHIEVEMENT_ASCENDED_WITH_AT_LEAST_FIVE_MILLION_POINTS,
        GUI_ACHIEVEMENT_ASCENDED_WITH_AT_LEAST_TEN_MILLION_POINTS,
        GUI_ACHIEVEMENT_ASCENDED_WITH_AT_LEAST_FIFTY_MILLION_POINTS,
        GUI_ACHIEVEMENT_ASCENDED_WITH_AT_LEAST_HUNDRED_MILLION_POINTS,

        /* Ascension: Ascended at expert for roles */
        GUI_ACHIEVEMENT_ASCENDED_ARCHAEOLOGIST_AT_EXPERT_DIFFICULTY = 1088,
        GUI_ACHIEVEMENT_ASCENDED_BARBARIAN_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_CAVEMAN_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_HEALER_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_KNIGHT_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_MONK_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_PRIEST_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_RANGER_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_ROGUE_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_SAMURAI_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_VALKYRIE_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_TOURIST_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_WIZARD_AT_EXPERT_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_ALL_ROLES_AT_EXPERT_DIFFICULTY,

        /* Ascension: Ascended at expert with conducts */
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_0 = 1120,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_1,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_2,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_3,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_4,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_5,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_6,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_7,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_8,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITH_CONDUCTS_9,

        /* Ascension: Ascended at expert, speedrun */
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITHIN_40000_TURNS = 1152,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITHIN_35000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITHIN_30000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITHIN_25000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITHIN_20000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_EXPERT_DIFFICULTY_WITHIN_15000_TURNS,

        /* Ascension: Ascended at master for roles */
        GUI_ACHIEVEMENT_ASCENDED_ARCHAEOLOGIST_AT_MASTER_DIFFICULTY = 1216,
        GUI_ACHIEVEMENT_ASCENDED_BARBARIAN_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_CAVEMAN_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_HEALER_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_KNIGHT_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_MONK_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_PRIEST_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_RANGER_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_ROGUE_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_SAMURAI_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_VALKYRIE_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_TOURIST_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_WIZARD_AT_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_ALL_ROLES_AT_MASTER_DIFFICULTY,

        /* Ascension: Ascended at master with conducts */
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_0 = 1248,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_1,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_2,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_3,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_4,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_5,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_6,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_7,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_8,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITH_CONDUCTS_9,

        /* Ascension: Ascended at master, speedrun */
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITHIN_40000_TURNS = 1280,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITHIN_35000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITHIN_30000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITHIN_25000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITHIN_20000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_MASTER_DIFFICULTY_WITHIN_15000_TURNS,

        /* Ascension: Ascended at master for roles */
        GUI_ACHIEVEMENT_ASCENDED_ARCHAEOLOGIST_AT_GRAND_MASTER_DIFFICULTY = 1344,
        GUI_ACHIEVEMENT_ASCENDED_BARBARIAN_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_CAVEMAN_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_HEALER_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_KNIGHT_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_MONK_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_PRIEST_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_RANGER_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_ROGUE_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_SAMURAI_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_VALKYRIE_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_TOURIST_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_WIZARD_AT_GRAND_MASTER_DIFFICULTY,
        GUI_ACHIEVEMENT_ASCENDED_ALL_ROLES_AT_GRAND_MASTER_DIFFICULTY,

        /* Ascension: Ascended at grand master with conducts */
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_0 = 1376,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_1,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_2,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_3,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_4,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_5,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_6,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_7,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_8,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITH_CONDUCTS_9,

        /* Ascension: Ascended at grand master, speedrun */
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITHIN_40000_TURNS = 1408,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITHIN_35000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITHIN_30000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITHIN_25000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITHIN_20000_TURNS,
        GUI_ACHIEVEMENT_ASCENDED_AT_GRAND_MASTER_DIFFICULTY_WITHIN_15000_TURNS,

        NUM_GUI_ACHIEVEMENTS
    }

    public enum game_status_types
    {
        GAME_STATUS_GENERAL_POST = 0,
        GAME_STATUS_START,
        GAME_STATUS_POST_IF_CLOUD_REPLAY_ON,
        GAME_STATUS_RESULT,
        GAME_STATUS_RESULT_ATTACHMENT,
    }

    public enum game_status_data_types
    {
        GAME_STATUS_ATTACHMENT_GENERIC = 0,
        GAME_STATUS_ATTACHMENT_DUMPLOG_TEXT,
        GAME_STATUS_ATTACHMENT_DUMPLOG_HTML,
    }

    public enum diagnostic_data_types
    {
        DIAGNOSTIC_DATA_GENERAL = 0,
        DIAGNOSTIC_DATA_IMPOSSIBLE,
        DIAGNOSTIC_DATA_PANIC,
        DIAGNOSTIC_DATA_CRITICAL,
        DIAGNOSTIC_DATA_CRASH_REPORT,
        DIAGNOSTIC_DATA_ATTACHMENT,
        DIAGNOSTIC_DATA_CREATE_ATTACHMENT_FROM_TEXT,
    }

    public enum diagnostic_data_attachment_types
    {
        DIAGNOSTIC_DATA_ATTACHMENT_GENERIC = 0,
        DIAGNOSTIC_DATA_ATTACHMENT_FILE_DESCRIPTOR_LIST,
    }


    public enum debug_log_types
    {
        DEBUGLOG_GENERAL = 0,
        DEBUGLOG_PRIORITY,
        DEBUGLOG_DEBUG_ONLY,
        DEBUGLOG_FILE_DESCRIPTOR,
        DEBUGLOG_PANIC,
        DEBUGLOG_IMPOSSIBLE,
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

    [Flags]
    public enum popup_text_flags : ulong
    {
        POPUP_FLAGS_NONE =          0x00000000U,
        POPUP_FLAGS_ADD_QUOTES =    0x00000001U,
        POPUP_FLAGS_COLOR_TEXT =    0x00000002U,
        POPUP_FLAGS_UPPER_SIDE =    0x00000004U,
    }

    public enum gui_effect_types
    {
        GUI_EFFECT_SEARCH = 0,
        GUI_EFFECT_WAIT,
        GUI_EFFECT_POLEARM,
        GUI_EFFECT_LIGHTNING,
        GUI_EFFECT_FIRE,
        GUI_EFFECT_FREEZE,
        GUI_EFFECT_MAGIC_HIT,
        GUI_EFFECT_STUN_HIT,
        GUI_EFFECT_DEATH_MAGIC,
    }

    public enum gui_polearm_types
    {
        GUI_POLEARM_SPEAR = 0,
        GUI_POLEARM_LANCE,
        GUI_POLEARM_THRUSTED,
        GUI_POLEARM_POLEAXE,
    }

    public enum yn_function_styles
    {
        YN_STYLE_GENERAL = 0,
        YN_STYLE_ITEM_EXCHANGE,
        YN_STYLE_KNAPSACK_FULL,
        YN_STYLE_MONSTER_QUESTION,
        YN_STYLE_END,
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
        SPECIAL_VIEW_SELFIE,
        SPECIAL_VIEW_HELP_DIR,
        SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N,
        SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y,
        SPECIAL_VIEW_SAVE_FILE_TRACKING_SAVE,
        SPECIAL_VIEW_SAVE_FILE_TRACKING_LOAD,
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

    public enum sound_bank_loading_type : int
    {
        Master = 0,
        Preliminary,
        Intro,
        Music,
        Game,
    }

    [Flags]
    public enum playsound_play_flags : uint
    {
        PLAY_FLAGS_NONE =                           0x00000000U,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING =     0x00000001U,
        PLAY_FLAGS_NO_PLAY_IF_ALREADY_QUEUED =      0x00000002U,
        PLAY_FLAGS_SONG =                           0x00000004U,
    }

    /* special key functions */
    public enum nh_keyfunc
    {
        NHKF_ESC = 0,
        NHKF_DOAGAIN,
        NHKF_DOAGAIN2,

        NHKF_REQMENU,

        /* run ... clicklook need to be in a continuous block */
        NHKF_RUN,
        NHKF_RUN2,
        NHKF_RUSH,
        NHKF_FIGHT,
        NHKF_FIGHT2,
        NHKF_NOPICKUP,
        NHKF_RUN_NOPICKUP,
        NHKF_DOINV,
        NHKF_DOSEEWORN,
        NHKF_TRAVEL,
        NHKF_TRAVEL_ATTACK,
        NHKF_TRAVEL_WALK,
        NHKF_CLICKFIRE,
        NHKF_CLICKLOOK,
        NHKF_CLICKCAST,
        NHKF_CLICKZAP,
        NHKF_CLICKPOLE,

        NHKF_REDRAW,
        NHKF_REDRAW2,
        NHKF_GETDIR_SELF,
        NHKF_GETDIR_SELF2,
        NHKF_GETDIR_HELP,
        NHKF_COUNT,
        NHKF_COUNT2,
        NHKF_GETPOS_SELF,
        NHKF_GETPOS_PICK,
        NHKF_GETPOS_PICK_Q,  /* quick */
        NHKF_GETPOS_PICK_O,  /* once */
        NHKF_GETPOS_PICK_V,  /* verbose */
        NHKF_GETPOS_SHOWVALID,
        NHKF_GETPOS_AUTODESC,
        NHKF_GETPOS_MON_NEXT,
        NHKF_GETPOS_MON_PREV,
        NHKF_GETPOS_OBJ_NEXT,
        NHKF_GETPOS_OBJ_PREV,
        NHKF_GETPOS_DOOR_NEXT,
        NHKF_GETPOS_DOOR_PREV,
        NHKF_GETPOS_UNEX_NEXT,
        NHKF_GETPOS_UNEX_PREV,
        NHKF_GETPOS_INTERESTING_NEXT,
        NHKF_GETPOS_INTERESTING_PREV,
        NHKF_GETPOS_VALID_NEXT,
        NHKF_GETPOS_VALID_PREV,
        NHKF_GETPOS_HELP,
        NHKF_GETPOS_MENU,
        NHKF_GETPOS_LIMITVIEW,
        NHKF_GETPOS_MOVESKIP,

        NUM_NHKF
    }

    public enum exit_hack_types
    {
        EXITHACK_NORMAL = 0,
        EXITHACK_RESTART_EXISTING,
        EXITHACK_RECOVER_NEW,
        EXITHACK_EXITTHREAD,
    }

    public enum CanvasTypes : int
    {
        General = 0,
        MainCanvas,
        CommandCanvas,
        MenuCanvas,
        TextCanvas,
        TipCanvas,
    }

    public enum BorderStyles
    {
        None = 0,
        Simple,
        Small,
        SimpleAlternative,
        SmallAlternative,
        SimpleTransformTopLeft,
        SimpleAlternativeTransformTopLeft,
        SimpleTransformTopRight,
        SimpleAlternativeTransformTopRight,
        Custom
    }

    public enum EngravingType
    {
        DUST = 1,
        ENGRAVE,
        BURN,
        MARK,
        ENGR_BLOOD,
        ENGR_HEADSTONE,
        ENGR_SIGNPOST
    }

    public enum RecordedFunctionID
    {
        Undefined = 0,
        EndOfFile,
        ContinueToNextFile,
        Reserved1,
        Reserved2,
        Reserved3,
        Reserved4,
        Reserved5,
        Reserved6,
        Reserved7,

        InitializeWindows,
        CreateWindow,
        DestroyWindow,
        ClearWindow,
        DisplayWindow,
        ExitWindows,
        PlayerSelection,
        Curs,
        PrintGlyph,
        AskName,
        GetEvent,
        GetChar,
        PosKey,
        YnFunction,
        ClipAround,
        RawPrint,
        RawPrintBold,
        PutStrEx,
        PutStrEx2,
        DelayOutput,
        DelayOutputMilliseconds,
        DelayOutputIntervals,
        PreferenceUpdate,
        StatusInit,
        StatusFinish,
        StatusEnable,
        StatusUpdate,
        GetMsgHistory,
        PutMsgHistory,
        StartMenu,
        AddExtendedMenu,
        EndMenu,
        SelectMenu,
        FreeMemory,
        ReportPlayerName,
        ReportPlayTime,
        SendObjectData,
        SendMonsterData,
        SendEngravingData,
        GetLine,
        ClearContextMenu,
        AddContextMenu,
        UpdateStatusButton,
        ToggleAnimationTimer,
        DisplayFloatingText,
        DisplayScreenText,
        DisplayPopupText,
        DisplayGUIEffect,
        UpdateCursor,
        PlayImmediateSound,
        PlayMusic,
        PlayLevelAmbient,
        PlayEnvironmentAmbient,
        PlayOccupationAmbient,
        PlayEffectAmbient,
        SetEffectAmbientVolume,
        AddAmbientSound,
        DeleteAmbientSound,
        SetAmbientSoundVolume,
        StopAllSounds,
        IssueGuiCommand,
        OutRip,
        UIHasInput,
        OpenSpecialView,
        ExitHack,
        NumberOfFunctionCalls
    }

    public enum GHPlatform
    {
        Unknown = 0,
        Android = 1,
        iOS = 2,
        Windows = 3,
        MacOS = 4,
        Unix = 5,
    }

    public enum PlayReplayResult
    {
        Success = 0,
        Error = 1,
        GameIsNull = 2,
        FilePathIsNullOrEmpty = 3,
        FileDoesNotExist = 4,
        InvalidVersion = 5,
        Restarting = 6,
    }

    public enum GameCursorType
    {
        Normal = 0,
        Info = 1
    }

    public enum GHSpecialKey
    {
        None = 0,
        Left,
        Right,
        Up,
        Down,
        UpLeft,
        UpRight,
        DownLeft,
        DownRight,
        PageDown,
        PageUp,
        Home,
        End,
        Insert,
        Delete,
        Escape,
        Enter,
        Space,
        Tab,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        Add,
        Subtract,
        Multiply,
        Divide,
        Decimal,
        Period,
        Comma,
        Plus,
        Minus,
        Number0,
        Number1,
        Number2,
        Number3,
        Number4,
        Number5,
        Number6,
        Number7,
        Number8,
        Number9,
        NumberPad0,
        NumberPad1,
        NumberPad2,
        NumberPad3,
        NumberPad4,
        NumberPad5,
        NumberPad6,
        NumberPad7,
        NumberPad8,
        NumberPad9,
        A,
        Z = A + 25,
    }
}

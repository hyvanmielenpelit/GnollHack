/*
 * general.h
 * Copyright (c) Janne Gustafsson, 2020
 */

#ifndef GENERAL_H
#define GENERAL_H

/*
 * This is a general header file for enums and structs used in arguments of global functions and
 * for general constants, enums, and structs with no better place
 *
 */

#define SHIELD_COUNT 21  /* Total number of cmap indices in the shield_static[] array. */
#define GNH_DEF_SHIELD_COUNT 10
#define TALK_COUNT 10  
#define GNH_DEF_TALK_COUNT 6
#define MAX_LAST_ITEM_SHOW_DURATION 99  
#define DEF_LAST_ITEM_SHOW_DURATION 3  
#define NATTK 8             /* Maximum number of attacks per monster */
#define MAXNASTIES 10    /* more than this can be created */
#define WARNCOUNT 7         /* number of different warning levels */

/* types of calls to bhit() */
enum bhit_call_types {
    ZAPPED_WAND = 0,
    THROWN_WEAPON = 1,
    THROWN_TETHERED_WEAPON = 2,
    KICKED_WEAPON = 3,
    FLASHED_LIGHT = 4,
    INVIS_BEAM = 5,
    GOLF_SWING = 6
};

enum zap_types {
    ZAP_RAY_MAGIC_MISSILE = 0,
    ZAP_RAY_FIRE,
    ZAP_RAY_COLD,
    ZAP_RAY_SLEEP,
    ZAP_RAY_DISINTEGRATION,
    ZAP_RAY_LIGHTNING,
    ZAP_RAY_POISON_GAS,
    ZAP_RAY_ACID,
    ZAP_RAY_DEATH,
    ZAP_RAY_PETRIFICATION,

    ZAP_DRAGON_BREATH_MAGIC_MISSILE,
    ZAP_DRAGON_BREATH_FIRE,
    ZAP_DRAGON_BREATH_COLD,
    ZAP_DRAGON_BREATH_SLEEP,
    ZAP_DRAGON_BREATH_DISINTEGRATION,
    ZAP_DRAGON_BREATH_LIGHTNING,
    ZAP_DRAGON_BREATH_POISON_GAS,
    ZAP_DRAGON_BREATH_ACID,
    ZAP_DRAGON_BREATH_DEATH,
    ZAP_DRAGON_BREATH_PETRIFICATION,

    ZAP_SPECIAL_DIGGING,
    ZAP_SPECIAL_EVAPORATION,
    ZAP_SPECIAL_FLASHED_LIGHT,

    MAX_ZAP_TYPES
};

#define NUM_ZAP 10       /* Number of basic zap beam types */


enum explosion_types {
    EXPL_DARK = 0,
    EXPL_NOXIOUS,
    EXPL_MUDD,
    EXPL_WET,
    EXPL_MAGICAL,
    EXPL_FIERY,
    EXPL_FROSTY,
    /* Additional real explosions here (to be used with explode) */
    EXPL_SCROLL_OF_FIRE,
    EXPL_METEOR_SWARM,
    /* Various large effects here (to be used with play_explosion_animation) */
    EXPL_CIRCLE_OF_FIRE,
    EXPL_CIRCLE_OF_FROST,
    EXPL_CIRCLE_OF_LIGHTNING,
    EXPL_CIRCLE_OF_MAGIC,
    EXPL_CIRCLE_OF_RADIANCE,
    EXPL_SCROLL_OF_EARTH,
    EXPL_FLAMING_SPHERE,
    EXPL_FREEZING_SPHERE,
    EXPL_SHOCKING_SPHERE,
    MAX_EXPLOSIONS
};


enum tailed_long_worm_types {
    LONG_WORM_TYPE_ADULT = 0,
    LONG_WORM_TYPE_ELDER,
    MAX_LONG_WORM_TYPES
};

enum worm_tile_types
{
    WORM_TILE_WORM_IS_RIGHT_GOING_UP_LEFT = 0,
    WORM_TILE_WORM_IS_DOWN_GOING_UP_LEFT,
    WORM_TILE_WORM_IS_LEFT_GOING_DOWN_RIGHT,
    WORM_TILE_WORM_IS_UP_GOING_DOWN_RIGHT,
    WORM_TILE_WORM_SEGMENT_GOING_UP,
    WORM_TILE_WORM_SEGMENT_GOING_LEFT,
    WORM_TILE_WORM_SEGMENT_GOING_UP_LEFT,
    WORM_TILE_WORM_SEGMENT_GOING_DOWN_RIGHT,
    WORM_TILE_WORM_SEGMENT_COMING_FROM_UP,
    WORM_TILE_WORM_SEGMENT_COMING_FROM_LEFT,
    WORM_TILE_WORM_SEGMENT_COMING_FROM_UP_LEFT,
    WORM_TILE_WORM_SEGMENT_COMING_FROM_DOWN_RIGHT,
    WORM_TILE_WORM_SEGMENT_BOTTOM,
    WORM_TILE_WORM_SEGMENT_MIDDLE,
    WORM_TILE_WORM_SEGMENT_TOP,
    WORM_TILE_WORM_RESERVED,
    WORM_TILE_WORM_TAIL_GOING_UP,
    WORM_TILE_WORM_TAIL_GOING_LEFT,
    WORM_TILE_WORM_TAIL_GOING_UP_LEFT,
    WORM_TILE_WORM_TAIL_GOING_DOWN_RIGHT,
    WORM_TILE_WORM_HEAD_COMING_FROM_UP,
    WORM_TILE_WORM_HEAD_COMING_FROM_LEFT,
    WORM_TILE_WORM_HEAD_COMING_FROM_UP_LEFT,
    WORM_TILE_WORM_HEAD_COMING_FROM_DOWN_RIGHT,
    MAX_WORM_TILES
};



struct replacement_info {
    int signed_glyph;
    int layer;
    struct obj* object;
    struct monst* monster;
    unsigned long layer_flags;
};

struct skill_menu_info
{
    char skillname[64];
    unsigned char skill_id;
    unsigned char skilltype;
    unsigned char current_level;
    unsigned char advance_flags;
    unsigned char skill_points_needed_to_advance;
    unsigned char max_level;
    const char* curlevelname;
    const char* nextlevelname;
    const char* maxlevelname;
    unsigned char bonus1_type;
    float curbonus1;
    float nextbonus1;
    unsigned char bonus2_type;
    float curbonus2;
    float nextbonus2;
    unsigned char bonus3_type;
    float curbonus3;
    float nextbonus3;
    unsigned char bonus4_type;
    float curbonus4;
    float nextbonus4;
};

struct spell_menu_info
{
    char spellname[64];
    char schoolname[64];
    short spell_id;
    unsigned char school_id;
    short level;
    float mana_cost;
    char stats[8];
    short fail_percentage;
    short cooldown_rounds;
    short casts;
    short adds;
    char description[64];
    char hotkey;
};

struct extended_menu_info {
    struct obj* object;
    struct monst* monster;
    char heading_for_group_accelerator;
    int color;
    int style; /* Generic style or subtype; used in menu data */
    char special_mark;
    unsigned long menu_flags;
};

struct extended_create_window_info {
    struct obj* object;
    struct monst* monster;
    unsigned long create_flags;
};

#define WINDOW_CREATE_FLAGS_NONE                0x00000000
#define WINDOW_CREATE_FLAGS_ACTIVE              0x00000001
#define WINDOW_CREATE_FLAGS_USE_SPECIAL_SYMBOLS 0x00000002 //For text window only; use menu flags for menu window

#define MENU_FLAGS_NONE                         0x00000000
#define MENU_FLAGS_IS_HEADING                   0x00000001
#define MENU_FLAGS_IS_GROUP_HEADING             0x00000002
#define MENU_FLAGS_MONOSPACE_FONT_REQUESTED     0x00000004
#define MENU_FLAGS_COUNT_DISALLOWED             0x00000008
#define MENU_FLAGS_USE_SKILL_MENU_DATA          0x00000010
#define MENU_FLAGS_USE_SPELL_MENU_DATA          0x00000020
#define MENU_FLAGS_ACTIVE                       0x00000040
#define MENU_FLAGS_USE_COLOR_FOR_SUFFIXES       0x00000080
#define MENU_FLAGS_USE_SPECIAL_SYMBOLS          0x00000100

#define MENU_DATAFLAGS_HAS_OBJECT_DATA          0x01
#define MENU_DATAFLAGS_HAS_MONSTER_DATA         0x02
#define MENU_DATAFLAGS_HALLUCINATED             0x04
#define MENU_DATAFLAGS_FEMALE                   0x08

enum obj_material_types {
    MAT_NONE = 0,
    MAT_LIQUID = 1, /* currently only for venom */  /* Organics start here */
    MAT_OIL = 2, /* flammable liquid */
    MAT_WAX = 3,
    MAT_VEGGY = 4, /* foodstuffs */
    MAT_FLESH = 5, /*   ditto    */
    MAT_ORGANIC = 6, /* non-veggy, non-flesh organic material, e.g. bat guano, feathers */
    MAT_PAPER = 7,
    MAT_CLOTH = 8,
    MAT_SILK = 9,
    MAT_LEATHER = 10, /* Flimsy materials stop here */
    MAT_WOOD = 11, /* Organics stop here */
    MAT_BONE = 12,
    MAT_CHITIN = 13, /* Chitin in insects' exoskeleton, or keratin in nails and horns */
    MAT_IVORY = 14, /* Worm tooth, ivory, etc. Hard bone-like materials stop here */
    MAT_DRAGON_HIDE = 15, /* not leather! */
    MAT_IRON = 16, /* Fe - includes steel */
    MAT_METAL = 17, /* Sn, &c. */
    MAT_COPPER = 18, /* Cu - includes brass */
    MAT_SILVER = 19, /* Ag */
    MAT_GOLD = 20, /* Au */
    MAT_PLATINUM = 21, /* Pt */
    MAT_ORICHALCUM = 22,
    MAT_ADAMANTIUM = 23,
    MAT_MITHRIL = 24,
    MAT_PLASTIC = 25,
    MAT_GLASS = 26,
    MAT_CRYSTAL = 27, //Brittle like glass
    MAT_HARD_CRYSTAL = 28, //Strong like gemstone
    MAT_GEMSTONE = 29,
    MAT_MINERAL = 30,
    MAT_MODRONITE = 31,
    MAT_PLANARRIFT = 32,
    MAT_FORCEFIELD = 33,
    MAT_AIR = 34,
    MAT_FIRE = 35,
    MAT_ENERGY = 36,
    MAT_INCORPOREAL = 37,
    MAT_ICE = 38,
    MAT_SOIL = 39,
    MAX_MATERIAL_TYPES
};

enum material_phase {
    PHASE_SOLID = 0,
    PHASE_LIQUID,
    PHASE_GAS,
    PHASE_ENERGY,
    PHASE_VOID
};

/* Other definitions */
enum hit_surface_source_types {
    HIT_SURFACE_SOURCE_NONE = 0,
    HIT_SURFACE_SOURCE_MONSTER,
    HIT_SURFACE_SOURCE_OBJECT,
    HIT_SURFACE_SOURCE_LOCATION,
    HIT_SURFACE_SOURCE_TRAP
};

enum hit_surface_types {
    HIT_SURFACE_NONE = 0,
    HIT_SURFACE_FLESH,
    HIT_SURFACE_ORGANIC, /* Jellies */
    HIT_SURFACE_BONE,  /* Also chitin etc. */
    HIT_SURFACE_LEATHER,
    HIT_SURFACE_METAL,
    HIT_SURFACE_WOOD,
    HIT_SURFACE_STONE,
    HIT_SURFACE_GLASS,
    HIT_SURFACE_LIQUID,
    HIT_SURFACE_IMMATERIAL,
    MAX_HIT_SURFACE_TYPES
};


/* Movement paramters (+weight) */
enum floor_surface_types {
    FLOOR_SURFACE_NONE = 0,
    FLOOR_SURFACE_STONE,
    FLOOR_SURFACE_WOOD,
    FLOOR_SURFACE_GROUND,
    FLOOR_SURFACE_GRASS,
    FLOOR_SURFACE_METAL,
    FLOOR_SURFACE_CARPET,
    FLOOR_SURFACE_LIQUID,
    MAX_FLOOR_SURFACE_TYPES
};

enum floor_treading_types {
    FLOOR_TREADING_TYPE_NORMAL = 0,
    FLOOR_TREADING_TYPE_STEALTH,
    FLOOR_TREADING_TYPE_RUN,
    MAX_FLOOR_TREADING_TYPES
};

enum climbing_types {
    CLIMBING_TYPE_NONE = 0,
    CLIMBING_TYPE_STAIRS_UP,
    CLIMBING_TYPE_STAIRS_DOWN,
    CLIMBING_TYPE_LADDER_UP,
    CLIMBING_TYPE_LADDER_DOWN,
    MAX_CLIMBING_TYPES
};

enum location_passing_types {
    LOCATION_PASSING_TYPE_NORMAL = 0,
    LOCATION_PASSING_TYPE_WATERWALKING,            /* Has waterwalking on water or lava, so does not strictly splash the water or lava */
    LOCATION_PASSING_TYPE_THROUGH_SOLID_WALLS,  /* Like a ghost, xorn or earth elemental; sound inside solid wall, maybe normal while not inside? */
    LOCATION_PASSING_TYPE_SUBMERGED,            /* Maybe an amulet of magical breathing and no swimming */
    MAX_LOCATION_PASSING_TYPES
};


enum special_level_naming_types {
    SPECIAL_LEVEL_NAMING_REVEALED_NEVER = 0,
    SPECIAL_LEVEL_NAMING_REVEALED_ON_ARRIVAL,
    SPECIAL_LEVEL_NAMING_REVEALED_ON_SEEING_MONSTER,
    MAX_SPECIAL_LEVEL_NAMING_TYPES
};

enum simple_doodad_tile_types
{
    DOODAD_STALAGMITE = 0,
    MAX_SIMPLE_DOODAD_TILES
};

enum mirrorable_doodad_tile_types
{
    DOODAD_COBWEB_NORMAL = 0,
    DOODAD_COBWEB_DOUBLE,
    DOODAD_COBWEB_CORNER_ELONGATED,
    DOODAD_COBWEB_CORNER_SMALL_DECORATED,
    DOODAD_COBWEB_CORNER_SMALL_PLAIN,
    DOODAD_COBWEB_CORNER_MEDIUM_PLAIN,
    DOODAD_COBWEB_CORNER_LARGE,
    MAX_MIRRORABLE_DOODAD_TILES
};
#define NUM_DOODAD_MIRRORINGS 4

enum hit_tile_types
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
};

enum general_tile_types
{
    GENERAL_TILE_DEATH = 0,
    GENERAL_TILE_CHAIN_IS_UP,
    GENERAL_TILE_CHAIN_IS_RIGHT,
    GENERAL_TILE_CHAIN_IS_DOWN,
    GENERAL_TILE_CHAIN_IS_LEFT,
    GENERAL_TILE_CHAIN_MAIN,
    MAX_GENERAL_TILES
};


/* Game cursor types */
enum game_cursor_types
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
};


/* GHWindow styles */
enum ghwindow_styles
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
};

/* GHMenu styles */
enum ghmenu_styles
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
    GHMENU_STYLE_DELETE_SAVED_GAME,
    GHMENU_STYLE_START_GAME_MENU,
    MAX_GHMENU_STYLES
};


/* Special view */
enum special_view_types
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
    SPECIAL_VIEW_GUI_YN_CONFIRMATION,
    MAX_SPECIAL_VIEW_TYPES
};


struct special_view_info {
    enum special_view_types viewtype;
    const char* text;
    const char* title;
    int attr;
    int color;
};

/* Fountain types */
enum fountain_subtypes
{
    FOUNTAIN_NATURAL = 0,
    FOUNTAIN_MAGIC,
    FOUNTAIN_HEALING,
    FOUNTAIN_MANA,
    FOUNTAIN_POWER,
    FOUNTAIN_POISON,
    FOUNTAIN_WATER,
    MAX_FOUNTAIN_SUBTYPES
};

#define LAST_SHUFFLED_FOUNTAIN FOUNTAIN_POISON


/* alteration types--keep in synch with costly_alteration(mkobj.c) */
enum cost_alteration_types {
    COST_CANCEL = 0, /* standard cancellation */
    COST_DRAIN = 1, /* drain life upon an object */
    COST_UNCHRG = 2, /* cursed charging */
    COST_UNBLSS = 3, /* unbless (devalues holy water) */
    COST_UNCURS = 4, /* uncurse (devalues unholy water) */
    COST_DECHNT = 5, /* disenchant weapons or armor */
    COST_DEGRD = 6, /* removal of rustproofing, dulling via engraving */
    COST_DILUTE = 7, /* potion dilution */
    COST_ERASE = 8, /* scroll or spellbook blanking */
    COST_BURN = 9, /* dipped into flaming oil */
    COST_NUTRLZ = 10, /* neutralized via unicorn horn */
    COST_DSTROY = 11, /* wand breaking (bill first, useup later) */
    COST_SPLAT = 12, /* cream pie to own face (ditto) */
    COST_BITE = 13, /* start eating food */
    COST_OPEN = 14, /* open tin */
    COST_BRKLCK = 15, /* break box/chest's lock */
    COST_RUST = 16, /* rust damage */
    COST_ROT = 17, /* rotting attack */
    COST_CORRODE = 18 /* acid damage */
};

enum animation_timer_types {
    ANIMATION_TIMER_GENERAL = 0,
    ANIMATION_TIMER_YOU,
    ANIMATION_TIMER_MONSTER,
    ANIMATION_TIMER_EXPLOSION,
    ANIMATION_TIMER_ZAP,
    ANIMATION_TIMER_SPECIAL_EFFECT,
};

enum floating_text_types {
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
};

enum screen_text_types {
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
};

enum popup_text_types {
    POPUP_TEXT_GENERAL = 0,
    POPUP_TEXT_IDENTIFY,
    POPUP_TEXT_NO_MONSTERS_IN_LIST,
    POPUP_TEXT_DIALOGUE,
    POPUP_TEXT_ADVICE,
    POPUP_TEXT_MESSAGE,
    POPUP_TEXT_REVIVAL,
};

enum gui_effect_types {
    GUI_EFFECT_SEARCH = 0,
    GUI_EFFECT_WAIT,
};

enum getline_types {
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
};

#define POPUP_FLAGS_NONE        0x00000000UL
#define POPUP_FLAGS_ADD_QUOTES  0x00000001UL

#define PICKLOCK_LEARNED_SOMETHING (-1) /* time passes */
#define PICKLOCK_DID_NOTHING 0          /* no time passes */
#define PICKLOCK_DID_SOMETHING 1

#define NUM_ZAP_SOURCE_BASE_DIRS 8
#define NUM_ZAP_SOURCE_DIRS (NUM_ZAP_SOURCE_BASE_DIRS * 2)

#define NUM_CHAIN_SOURCE_DIRS 4
#define NUM_WORM_SOURCE_DIRS 4

#define GH_LONG_MIN    (-2147483647L - 1)    // minimum (signed) long value
#define GH_LONG_MAX      2147483647L        // maximum (signed) long value

#define GH_UNREFERENCED_PARAMETER(P)        (P)
#define NUM_UNSIGNED_LONG_BITS 32

#define MKARTIFACT_FLAGS_NONE                 0x00
#define MKARTIFACT_FLAGS_NONWEAPONS_ONLY      0x01
#define MKARTIFACT_FLAGS_NO_VORPAL_WEAPONS    0x02
#define MKARTIFACT_FLAGS_WEAPONS_ONLY         0x04

#define ZHITM_FLAGS_CRITICAL_STRIKE           0x01
#define ZHITM_FLAGS_PETRIFICATION             0x02
#define ZHITM_FLAGS_DISINTEGRATION            0x04
#define ZHITM_FLAGS_DEATH                     0x08
#define ZHITM_FLAGS_SLEEP                     0x10
#define ZHITM_FLAGS_DO_NOT_SHOW_HIT_TILE      0x20
#define ZHITM_FLAGS_HIDE_DAMAGE               0x40

#define DEATH_MAX_HP_FROM_DEATH_RAY 2500
#define NUM_METEOR_SWARM_METEORS 4

enum main_menu_command {
    MAIN_MENU_NONE = 0,
    MAIN_MENU_START_GAME,
    MAIN_MENU_EXIT_GAME
};

enum create_context_menu_types {
    CREATE_CONTEXT_MENU_NORMAL = 0,
    CREATE_CONTEXT_MENU_IN_GETPOS_GENERAL,
    CREATE_CONTEXT_MENU_IN_GETPOS_MONSTER,
    CREATE_CONTEXT_MENU_IN_GETPOS_TRAVEL_MODE,
    CREATE_CONTEXT_MENU_IN_GETDIR,
    CREATE_CONTEXT_MENU_BLOCKING_WINDOW,
};

enum context_menu_styles {
    CONTEXT_MENU_STYLE_GENERAL = 0,
    CONTEXT_MENU_STYLE_GETDIR,
    CONTEXT_MENU_STYLE_GETPOS,
    CONTEXT_MENU_STYLE_CLOSE_DISPLAY,
};


#define MIN_DIFFICULTY_LEVEL -4
#define MAX_DIFFICULTY_LEVEL 2
#define WAND_OF_PROBING_DIFFICULTY_THRESHOLD -1
#define SCROLL_OF_REMOVE_CURSE_DIFFICULTY_THRESHOLD -3

/* Moved from spell.c */
/* spell retention period, in turns; at 10% of this value, player becomes
   eligible to reread the spellbook and regain 100% retention (the threshold
   used to be 1000 turns, which was 10% of the original 10000 turn retention
   period but didn't get adjusted when that period got doubled to 20000) */
#define SPELL_IS_KEEN 20000
   /* x: need to add 1 when used for reading a spellbook rather than for hero
      initialization; spell memory is decremented at the end of each turn,
      including the turn on which the spellbook is read; without the extra
      increment, the hero used to get cheated out of 1 turn of retention */
#define incr_spell_nknow(spell, x) (spl_book[spell].sp_know = SPELL_IS_KEEN + (x))

#define SUMMONCREATURE_FLAGS_CAPITALIZE                0x00000001
#define SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED          0x00000002
#define SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH       0x00000004
#define SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH         0x00000008
#define SUMMONCREATURE_FLAGS_BLOODLUST                 0x00000010
#define SUMMONCREATURE_FLAGS_PACIFIST                  0x00000020
#define SUMMONCREATURE_FLAGS_FAITHFUL                  0x00000040
#define SUMMONCREATURE_FLAGS_PROTECTOR                 0x00000080

#define TAMEDOG_NO_FORCED_TAMING    0
#define TAMEDOG_FORCE_NON_UNIQUE    1
#define TAMEDOG_FORCE_ALL           2


#define MKOBJ_FLAGS_OPEN_COFFIN                        0x00000001
#define MKOBJ_FLAGS_MONSTER_SPECIFIED                  0x00000002
#define MKOBJ_FLAGS_ALSO_RARE                          0x00000004
#define MKOBJ_FLAGS_SHOP_ITEM                          0x00000008
#define MKOBJ_FLAGS_PARAM_IS_TITLE                     0x00000010
#define MKOBJ_FLAGS_NORMAL_SPELLBOOK                   0x00000020
#define MKOBJ_FLAGS_FORCE_MYTHIC_OR_LEGENDARY          0x00000040
#define MKOBJ_FLAGS_FORCE_LEGENDARY                    0x00000080
#define MKOBJ_FLAGS_PARAM_IS_EXCEPTIONALITY            0x00000100
#define MKOBJ_FLAGS_OWNER_IS_LAWFUL                    0x00000200
#define MKOBJ_FLAGS_OWNER_IS_NEUTRAL                   0x00000400
#define MKOBJ_FLAGS_OWNER_IS_CHAOTIC                   0x00000800
#define MKOBJ_FLAGS_OWNER_IS_NONALIGNED                0x00001000
#define MKOBJ_FLAGS_PARAM_IS_EXCLUDED_INDEX_BITS       0x00002000

#define MONDIED_FLAGS_NONE                             0x00000000
#define MONDIED_FLAGS_NO_DEATH_ACTION                  0x00000001

/* Moved from shk.c */
#define NOTANGRY(mon) ((mon)->mpeaceful)
#define ANGRY(mon) (!NOTANGRY(mon))
#define IS_SHOP(x) (rooms[x].rtype >= SHOPBASE)


/* Turn on score on botl everywhere */
#define SCORE_ON_BOTL

/* Demo version */
#define GUI_SPECIAL_EFFECT_LEVEL_DEPTH_THRESHOLD 10

/* Basic tile information */
#define NUM_TILES_PER_SHEET 16224
#define MAX_TILE_SHEETS 2
#define MAX_TILES (MAX_TILE_SHEETS * NUM_TILES_PER_SHEET)

/* mode values for findtravelpath() and m_findtravelpath() */
#define TRAVP_TRAVEL 0
#define TRAVP_GUESS  1
#define TRAVP_VALID  2

/* gui_command_types */
enum gui_command_types {
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
    GUI_CMD_GAME_START,
    GUI_CMD_GAME_ENDED,
};

enum takeoff_wear_commands {
    TAKEOFF_WEAR_CMD_TAKEOFF = 0,
    TAKEOFF_WEAR_CMD_TAKEOFF_AND_WEAR,
    TAKEOFF_WEAR_CMD_EXCHANGE,
};

enum yn_function_styles {
    YN_STYLE_GENERAL = 0,
    YN_STYLE_ITEM_EXCHANGE,
    YN_STYLE_KNAPSACK_FULL,
    YN_STYLE_MONSTER_QUESTION,
};

/* Other */
#define DISREGARD_HEALTH_LEVEL_MULTIPLIER 2
#define DISREGARD_HEALTH_LEVEL_DIFFERENCE_THRESHOLD 5
#define ARMOR_SPELL_CASTING_PENALTY_MULTIPLIER 30L
#define MAX_BURN_IN_CANDELABRUM 6000L
#define MAX_OIL_IN_LAMP 1500L

/* Artifact */
#define ARTDMG_125PCT_DAMAGE -5
#define ARTDMG_150PCT_DAMAGE -10
#define ARTDMG_175PCT_DAMAGE -15
#define ARTDMG_DOUBLE_DAMAGE -20
#define ARTDMG_250PCT_DAMAGE -30
#define ARTDMG_TRIPLE_DAMAGE -40
#define ARTDMG_350PCT_DAMAGE -50
#define ARTDMG_QUADRUPLE_DAMAGE -60

/* Object data */
#define OBJDATA_FLAGS_DRAWN_IN_FRONT    0x00000001UL
#define OBJDATA_FLAGS_HALLUCINATION     0x00000002UL
#define OBJDATA_FLAGS_UCHAIN            0x00000004UL
#define OBJDATA_FLAGS_UBALL             0x00000008UL

/* Monster abilities */
#define BREATH_WEAPON_MANA_COST 15
#define GAZE_MANA_COST 15
#define CNCL_GAZE_MANA_COST 5
#define EYE_STALK_MANA_COST 5
#define WERE_SUMMON_MANA_COST 10
#define MIND_BLAST_MANA_COST 10
#define UNICORN_HORN_MANA_COST 25

/* Minimum dungeon levels for traps */
#define MINIMUM_DGN_LEVEL_MEDIUM_LEVEL  2
#define MINIMUM_DGN_LEVEL_LEVELTELE     5
#define MINIMUM_DGN_LEVEL_SPIKED_PIT    5
#define MINIMUM_DGN_LEVEL_LANDMINE      6
#define MINIMUM_DGN_LEVEL_WEB           7
#define MINIMUM_DGN_LEVEL_POLY_TRAP     8

/*
 * type nhsym: loadable symbols go into this type
 */
typedef long nhsym;

#define DEF_HERE_WINDOW_MAX_SIZE 10

#define MAX_QUEST_REQUIREMENTS 12
struct gh_quest_status_data {
    unsigned char quest_state;
    unsigned char requirement_state[MAX_QUEST_REQUIREMENTS];
};

enum quest_states {
    QUEST_STATE_UNDISCOVERED = 0,
    QUEST_STATE_DISCOVERED,
    QUEST_STATE_COMPLETED,
    QUEST_STATE_FAILED,
};

enum spl_sort_types {
    SORTBY_NONE = 0,
    SORTBY_LETTER = 0, //This is the same as none in fact
    SORTBY_ALPHA,
    SORTBY_LVL_LO,
    SORTBY_LVL_HI,
    SORTBY_SKL_AL,
    SORTBY_SKL_LO,
    SORTBY_SKL_HI,
    SORTBY_CURRENT,
    SORTRETAINORDER,

    NUM_SPELL_SORTBY
};

struct multishot_result {
    int wielder_attacks; /* Multishot due to the wielder skill */
    int weapon_attacks; /* Multishot due to the weapon */
    double average;
};


#define AD_MODULO 4


#endif /* GENERAL_H */


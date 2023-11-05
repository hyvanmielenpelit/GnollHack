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
    unsigned long monster_flags;
    unsigned long missile_flags;
    unsigned char missile_material;
    short missile_special_quality;
};

struct extended_menu_info {
    struct obj* object;
    struct monst* monster;
    char heading_for_group_accelerator;
    const char* attrs;
    const char* colors;
    int style; /* Generic style or subtype; used in menu data */
    char special_mark;
    int num_items; /* Number of items in e.g. category */
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
#define WINDOW_CREATE_FLAGS_ASCENDED            0x00000004

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
#define MENU_FLAGS_USE_NUM_ITEMS                0x00000200
#define MENU_FLAGS_BUTTON_STYLE                 0x00000400
#define MENU_FLAGS_AUTO_CLICK_OK                0x00000800

#define MENU_DATAFLAGS_HAS_OBJECT_DATA          0x01
#define MENU_DATAFLAGS_HAS_MONSTER_DATA         0x02
#define MENU_DATAFLAGS_HALLUCINATED             0x04
#define MENU_DATAFLAGS_FEMALE                   0x08

enum obj_material_types {
    MAT_NONE = 0,
    MAT_LIQUID, /* currently only for venom */  /* Organics start here */
    MAT_OIL, /* flammable liquid */
    MAT_WAX,
    MAT_VEGGY, /* foodstuffs */
    MAT_FLESH, /*   ditto    */
    MAT_ORGANIC, /* non-veggy, non-flesh organic material, e.g. bat guano, feathers */
    MAT_PAPER,
    MAT_CLOTH,
    MAT_COTTON,
    MAT_SILK,
    MAT_LEATHER, /* Flimsy materials stop here */
    MAT_WOOD, /* Organics stop here */
    MAT_BONE,
    MAT_CHITIN, /* Chitin in insects' exoskeleton, or keratin in nails and horns */
    MAT_TOOTH, /* Worm tooth, ivory, etc. Hard bone-like materials stop here */
    MAT_DRAGON_HIDE, /* not leather! */
    MAT_IRON, /* Fe */
    MAT_STEEL, /* Alloy of Fe and C (carbon)  */
    MAT_METAL, /* Sn, &c. */
    MAT_LEAD, /* Pb */
    MAT_COPPER, /* Cu */
    MAT_BRASS, /* Alloy of Cu and Zn (zinc) */
    MAT_BRONZE, /* Alloy of Cu and Sn (tin) */
    MAT_SILVER, /* Ag */
    MAT_GOLD, /* Au */
    MAT_PLATINUM, /* Pt */
    MAT_ORICHALCUM,
    MAT_ADAMANTIUM,
    MAT_MITHRIL,
    MAT_PLASTIC,
    MAT_GLASS,
    MAT_CRYSTAL, /* Brittle like glass */
    MAT_HARD_CRYSTAL, /* Strong like gemstone */
    MAT_GEMSTONE,
    MAT_MINERAL, /* Stone */
    MAT_MODRONITE,
    MAT_PLANARRIFT,
    MAT_FORCEFIELD,
    MAT_AIR,
    MAT_FIRE,
    MAT_ENERGY,
    MAT_INCORPOREAL,
    MAT_ICE,
    MAT_SOIL,
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
    SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N,
    SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y,
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
    GUI_EFFECT_POLEARM,
};

enum gui_polearm_types {
    GUI_POLEARM_SPEAR = 0,
    GUI_POLEARM_LANCE,
    GUI_POLEARM_THRUSTED,
    GUI_POLEARM_POLEAXE,
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
#define POPUP_FLAGS_COLOR_TEXT  0x00000002UL
#define POPUP_FLAGS_UPPER_SIDE  0x00000004UL

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
#define NO_SECRET_DOORS_DIFFICULTY_THRESHOLD -1
#define DEFAULT_MAX_HINT_DIFFICULTY -1

#define NO_SECRET_DOORS_DUNGEON_LEVEL_THRESHOLD 2
#define DEFAULT_RUN_SPOT_DISTANCE 10
#define RUN_SPOT_NEARBY_DISTANCE 6

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
#define MKOBJ_FLAGS_FORCE_BASE_MATERIAL                0x00004000
#define MKOBJ_FLAGS_PARAM_IS_SPECIAL_QUALITY           0x00008000
#define MKOBJ_FLAGS_PARAM_IS_MNUM                      0x00010000
#define MKOBJ_FLAGS_FOUND_THIS_TURN                    0x00020000

#define MONDEAD_FLAGS_NONE                             0x00000000
#define MONDEAD_FLAGS_NO_DEATH_ACTION                  0x00000001
#define MONDEAD_FLAGS_DISINTEGRATED                    0x00000002
#define MONDEAD_FLAGS_STONED                           0x00000004
#define MONDEAD_FLAGS_DIGESTED                         0x00000008
#define MONDEAD_FLAGS_GENERIC_NO_CORPSE                0x00000010

#define DONAME_WITH_PRICE                   0x0001
#define DONAME_VAGUE_QUAN                   0x0002
#define DONAME_WITH_WEIGHT_FIRST            0x0004
#define DONAME_WITH_WEIGHT_LAST             0x0008
#define DONAME_LOADSTONE_CORRECTLY          0x0010
#define DONAME_LIT_IN_FRONT                 0x0020
#define DONAME_HIDE_REMAINING_LIT_TURNS     0x0040
#define DONAME_COMPARISON                   0x0080

/* Moved from shk.c */
#define NOTANGRY(mon) ((mon)->mpeaceful)
#define ANGRY(mon) (!NOTANGRY(mon))
#define IS_SHOP(x) (rooms[x].rtype >= SHOPBASE)

/* Special effect in GUI upon reaching a certain level */
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
    GUI_CMD_POST_GAME_STATUS,
    GUI_CMD_POST_DIAGNOSTIC_DATA,
    GUI_CMD_LIBRARY_MANUAL,
    GUI_CMD_DEBUGLOG,
    GUI_CMD_GAME_START,
    GUI_CMD_GAME_ENDED,
};

enum game_status_types
{
    GAME_STATUS_START = 0,
    GAME_STATUS_EVENT,
    GAME_STATUS_ACHIEVEMENT,
    GAME_STATUS_RESULT,
    GAME_STATUS_RESULT_ATTACHMENT,
};

enum game_status_data_types
{
    GAME_STATUS_ATTACHMENT_GENERIC = 0,
    GAME_STATUS_ATTACHMENT_DUMPLOG_TEXT,
    GAME_STATUS_ATTACHMENT_DUMPLOG_HTML,
};

enum diagnostic_data_types
{
    DIAGNOSTIC_DATA_GENERAL = 0,
    DIAGNOSTIC_DATA_IMPOSSIBLE,
    DIAGNOSTIC_DATA_PANIC,
    DIAGNOSTIC_DATA_CRITICAL,
    DIAGNOSTIC_DATA_CRASH_REPORT,
    DIAGNOSTIC_DATA_ATTACHMENT,
    DIAGNOSTIC_DATA_CREATE_ATTACHMENT_FROM_TEXT,
};

enum diagnostic_data_attachment_types
{
    DIAGNOSTIC_DATA_ATTACHMENT_GENERIC = 0,
    DIAGNOSTIC_DATA_ATTACHMENT_FILE_DESCRIPTOR_LIST,
};

enum debug_log_types
{
    DEBUGLOG_GENERAL = 0,
    DEBUGLOG_DEBUG_ONLY,
    DEBUGLOG_FILE_DESCRIPTOR,
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
    YN_STYLE_END,
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
#define OBJDATA_FLAGS_UWEP              0x00000010UL
#define OBJDATA_FLAGS_UWEP2             0x00000020UL
#define OBJDATA_FLAGS_UQUIVER           0x00000040UL
#define OBJDATA_FLAGS_OUT_OF_AMMO1      0x00000080UL
#define OBJDATA_FLAGS_WRONG_AMMO_TYPE1  0x00000100UL
#define OBJDATA_FLAGS_NOT_BEING_USED1   0x00000200UL
#define OBJDATA_FLAGS_NOT_WEAPON1       0x00000400UL
#define OBJDATA_FLAGS_OUT_OF_AMMO2      0x00000800UL
#define OBJDATA_FLAGS_WRONG_AMMO_TYPE2  0x00001000UL
#define OBJDATA_FLAGS_NOT_BEING_USED2   0x00002000UL
#define OBJDATA_FLAGS_NOT_WEAPON2       0x00004000UL
#define OBJDATA_FLAGS_FOUND_THIS_TURN   0x00008000UL
#define OBJDATA_FLAGS_IS_AMMO           0x00010000UL /* is_ammo is TRUE */
#define OBJDATA_FLAGS_THROWING_WEAPON   0x00020000UL /* throwing_weapon is TRUE */

/* Monster abilities */
#define BREATH_WEAPON_MANA_COST 15
#define GAZE_MANA_COST 15
#define CNCL_GAZE_MANA_COST 5
#define EYE_STALK_MANA_COST 5
#define WERE_SUMMON_MANA_COST 10
#define MIND_BLAST_MANA_COST 10
#define UNICORN_HORN_MANA_COST 25
#define TELEPORT_AT_WILL_MANA_COST 15

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

struct item_description_stats {
    unsigned char stats_set;
    unsigned char weapon_stats_printed;
    unsigned char armor_stats_printed;
    unsigned char wep_ac50pct_set;
    double avg_damage;
    double extra_damage;
    int ac50pct;
    int ac_bonus;
    int mc_bonus;
};

enum dog_breeds {
    DOG_BREED_GENERIC = 0,
    DOG_BREED_LABRADOR_BLACK,
    DOG_BREED_LABRADOR_BROWN,
    DOG_BREED_LABRADOR_YELLOW,
    DOG_BREED_DALMATIAN,
    DOG_BREED_GERMAN_SHEPHERD,
    DOG_BREED_BLACK_SCHNAUZER,
    DOG_BREED_SALTPEPPER_SCHNAUZER,
    DOG_BREED_SCOTTISH_TERRIER,
    DOG_BREED_PEMBROKE_CORGI,
    DOG_BREED_CARDIGAN_CORGI,
    DOG_BREED_COCKER_SPANIEL,
    DOG_BREED_BLOODHOUND,
    DOG_BREED_BLUE_ROUGH_COLLIE,
    DOG_BREED_SABLE_ROUGH_COLLIE,
    DOG_BREED_TRICOLOR_ROUGH_COLLIE,
    DOG_BREED_FINNISH_LAPPHUND,
    DOG_BREED_AKITA,
    DOG_BREED_SHIBA,
    DOG_BREED_BLACK_BORDER_COLLIE,
    DOG_BREED_BROWN_BORDER_COLLIE,
    DOG_BREED_GREY_BORDER_COLLIE,
    DOG_BREED_ENGLISH_SPRINGER_SPANIEL,
    DOG_BREED_WELSH_SPRINGER_SPANIEL,
    DOG_BREED_BULLDOG,
    DOG_BREED_ROTTWEILER,
    DOG_BREED_DACHSHUND,
    DOG_BREED_BEAGLE,
    DOG_BREED_ST_BERNARD,
    NUM_DOG_BREEDS
};
#define NUM_SPECIAL_DOG_BREEDS (NUM_DOG_BREEDS - 1)

enum cat_breeds {
    CAT_BREED_GENERIC = 0,
    CAT_BREED_BLACK,
    CAT_BREED_SIAMESE,
    CAT_BREED_PERSIAN_GREY,
    CAT_BREED_PERSIAN_WHITE,
    CAT_BREED_RAGDOLL_BROWN,
    CAT_BREED_RAGDOLL_GREY,
    CAT_BREED_RAGDOLL_WHITE,
    NUM_CAT_BREEDS
};
#define NUM_SPECIAL_CAT_BREEDS (NUM_CAT_BREEDS - 1)

#define AD_MODULO 4
#define ZAP_TEMPLATE_WIDTH 5
#define ZAP_TEMPLATE_HEIGHT 4
#define NUM_ZAP_BASE_TILES 18

#define UTOFLAGS_AT_STAIRS          0x0001
#define UTOFLAGS_FALLING            0x0002
#define UTOFLAGS_PORTAL_1           0x0004
#define UTOFLAGS_PORTAL_2           0x0008
#define UTOFLAGS_PORTAL_3           0x0010
#define UTOFLAGS_PORTAL_4           0x0020
#define UTOFLAGS_DEFERRED_GOTO      0x0040
#define UTOFLAGS_REMOVE_PORTAL      0x0080
#define UTOFLAGS_TELEPORT_EFFECT    0x0100
#define UTOFLAGS_AT_ALTAR           0x0200
#define UTOFLAGS_INSIDE_TOWER       0x0400


#endif /* GENERAL_H */


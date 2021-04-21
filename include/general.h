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
#define TALK_COUNT 10  
#define NATTK 8	         /* Maximum number of attacks per monster */
#define MAXNASTIES 10    /* more than this can be created */
#define WARNCOUNT 7		 /* number of different warning levels */

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
	/* Various large effects here */
	EXPL_CIRCLE_OF_FIRE,
	EXPL_CIRCLE_OF_FROST,
	EXPL_CIRCLE_OF_LIGHTNING,
	EXPL_CIRCLE_OF_MAGIC,
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

struct extended_menu_info {
    struct obj* object;
    int color;
    unsigned long menu_flags;
};

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
	MAT_CHITIN = 13,
	MAT_TOOTH = 14, /* Hard bone-like materials stop here */
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
	MAT_GEMSTONE = 27,
	MAT_MINERAL = 28,
	MAT_MODRONITE = 29,
	MAT_PLANARRIFT = 30,
	MAT_FORCEFIELD = 31,
	MAT_AIR = 32,
	MAT_FIRE = 33,
	MAT_ENERGY = 34,
	MAT_INCORPOREAL = 35,
	MAT_ICE = 36,
	MAT_SOIL = 37,
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
	LOCATION_PASSING_TYPE_WATERWALKING,			/* Has waterwalking on water or lava, so does not strictly splash the water or lava */
	LOCATION_PASSING_TYPE_THROUGH_SOLID_WALLS,  /* Like a ghost, xorn or earth elemental; sound inside solid wall, maybe normal while not inside? */
	LOCATION_PASSING_TYPE_SUBMERGED,			/* Maybe an amulet of magical breathing and no swimming */
	MAX_LOCATION_PASSING_TYPES
};


enum special_level_naming_types {
	SPECIAL_LEVEL_NAMING_REVEALED_NEVER = 0,
	SPECIAL_LEVEL_NAMING_REVEALED_ON_ARRIVAL,
	SPECIAL_LEVEL_NAMING_REVEALED_ON_SEEING_MONSTER,
	MAX_SPECIAL_LEVEL_NAMING_TYPES
};

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



/* Special view */
enum special_view_types
{
	SPECIAL_VIEW_NONE = 0,
	SPECIAL_VIEW_CHAT_MESSAGE,
	MAX_SPECIAL_VIEW_TYPES
};

struct special_view_info {
	enum special_view_types viewtype;
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

#define PICKLOCK_LEARNED_SOMETHING (-1) /* time passes */
#define PICKLOCK_DID_NOTHING 0          /* no time passes */
#define PICKLOCK_DID_SOMETHING 1

#define NUM_ZAP_SOURCE_BASE_DIRS 8
#define NUM_ZAP_SOURCE_DIRS (NUM_ZAP_SOURCE_BASE_DIRS * 2)

#define NUM_CHAIN_SOURCE_DIRS 4
#define NUM_WORM_SOURCE_DIRS 4

#define GH_LONG_MIN    (-2147483647L - 1)	// minimum (signed) long value
#define GH_LONG_MAX      2147483647L		// maximum (signed) long value

#define GH_UNREFERENCED_PARAMETER(P)        (P)
#define NUM_UNSIGNED_LONG_BITS 32

#define MKARTIFACT_FLAGS_NONE				0x00
#define MKARTIFACT_FLAGS_NONWEAPONS_ONLY	0x01
#define MKARTIFACT_FLAGS_NO_VORPAL_WEAPONS	0x02
#define MKARTIFACT_FLAGS_WEAPONS_ONLY		0x04

#define ZHITM_FLAGS_CRITICAL_STRIKE			0x01
#define ZHITM_FLAGS_PETRIFICATION			0x02
#define ZHITM_FLAGS_DISINTEGRATION			0x04
#define ZHITM_FLAGS_DEATH					0x08
#define ZHITM_FLAGS_SLEEP					0x10
#define ZHITM_FLAGS_DO_NOT_SHOW_HIT_TILE	0x20
#define ZHITM_FLAGS_HIDE_DAMAGE				0x40

#define DEATH_MAX_HP_FROM_DEATH_RAY 2500
#define NUM_METEOR_SWARM_METEORS 4

enum main_menu_command {
	MAIN_MENU_NONE = 0,
	MAIN_MENU_START_GAME,
	MAIN_MENU_EXIT_GAME
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

#define SUMMONCREATURE_FLAGS_CAPITALIZE				0x00000001
#define SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED		0x00000002
#define SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH	0x00000004
#define SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH		0x00000008
#define SUMMONCREATURE_FLAGS_BLOODLUST				0x00000010
#define SUMMONCREATURE_FLAGS_PACIFIST				0x00000020
#define SUMMONCREATURE_FLAGS_FAITHFUL				0x00000040
#define SUMMONCREATURE_FLAGS_PROTECTOR				0x00000080

#define TAMEDOG_NO_FORCED_TAMING	0
#define TAMEDOG_FORCE_NON_UNIQUE	1
#define TAMEDOG_FORCE_ALL			2


#define MKOBJ_FLAGS_OPEN_COFFIN						0x00000001
#define MKOBJ_FLAGS_MONSTER_SPECIFIED				0x00000002


/* Moved from shk.c */
#define NOTANGRY(mon) ((mon)->mpeaceful)
#define ANGRY(mon) (!NOTANGRY(mon))
#define IS_SHOP(x) (rooms[x].rtype >= SHOPBASE)


/* Turn on score on botl everywhere */
#define SCORE_ON_BOTL

/* Basic tile information */
#define NUM_TILES_PER_SHEET 16224
#define MAX_TILE_SHEETS 2
#define MAX_TILES (MAX_TILE_SHEETS * NUM_TILES_PER_SHEET)

/* mode values for findtravelpath() and m_findtravelpath() */
#define TRAVP_TRAVEL 0
#define TRAVP_GUESS  1
#define TRAVP_VALID  2

/* Other */
#define DISREGARD_HEALTH_LEVEL_MULTIPLIER 2
#define DISREGARD_HEALTH_LEVEL_DIFFERENCE_THRESHOLD 5

#endif /* GENERAL_H */


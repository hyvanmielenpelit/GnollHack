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


struct replacement_info {
	int signed_glyph;
	int layer;
	struct obj* object;
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


enum exceptionality_types {
	EXCEPTIONALITY_NORMAL = 0,
	EXCEPTIONALITY_EXCEPTIONAL,
	EXCEPTIONALITY_ELITE,
	EXCEPTIONALITY_CELESTIAL,
	EXCEPTIONALITY_PRIMORDIAL,
	EXCEPTIONALITY_INFERNAL,
	MAX_EXCEPTIONALITY_TYPES
};

enum special_level_naming_types {
	SPECIAL_LEVEL_NAMING_REVEALED_NEVER = 0,
	SPECIAL_LEVEL_NAMING_REVEALED_ON_ARRIVAL,
	SPECIAL_LEVEL_NAMING_REVEALED_ON_SEEING_MONSTER,
	MAX_SPECIAL_LEVEL_NAMING_TYPES
};

enum hit_tile_types
{
	HIT_TILE = 0,
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
	MAX_HIT_TILES
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

#define PICKLOCK_LEARNED_SOMETHING (-1) /* time passes */
#define PICKLOCK_DID_NOTHING 0          /* no time passes */
#define PICKLOCK_DID_SOMETHING 1

#endif /* GENERAL_H */


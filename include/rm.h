/* GnollHack 4.0	rm.h	$NHDT-Date: 1547255911 2019/01/12 01:18:31 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.60 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef RM_H
#define RM_H

#include "layer.h"
#include "soundset.h"


 /*
 * The screen symbols may be the default or defined at game startup time.
 * See drawing.c for defaults.
 * Note: {ibm|dec|curses}_graphics[] arrays (also in drawing.c) must be kept in
 * synch.
 */

/* begin dungeon characters */
enum screen_symbols {
    S_unexplored = 0,
    S_stone     =  1,
    S_vwall     =  2,
    S_hwall     =  3,
    S_tlcorn    =  4,
    S_trcorn    =  5,
    S_blcorn    =  6,
    S_brcorn    =  7,
    S_crwall    =  8,
    S_tuwall    =  9,
    S_tdwall    = 10,
    S_tlwall    = 11,
    S_trwall    = 12,
    S_ndoor     = 13,
    S_vodoor    = 14,
    S_hodoor    = 15,
    S_vcdoor    = 16, /* closed door, vertical wall */
    S_hcdoor    = 17, /* closed door, horizontal wall */
    S_vbdoor    = 18, /* broken door, vertical wall */
    S_hbdoor    = 19, /* broken door, horizontal wall */
    S_voportcullis = 20, /* open portcullis, vertical wall */
    S_hoportcullis = 21, /* open portcullis, horizontal wall */
    S_bars      = 22, /* KMH -- iron bars */
    S_tree      = 23, /* KMH */
    S_room      = 24,
	S_darkroom  = 25,
    S_grass     = 26,
    S_darkgrass = 27,
    S_ground    = 28,
    S_darkground = 29,
    S_corr      = 30,
    S_litcorr   = 31,
    S_upstair   = 32,
    S_dnstair   = 33,
    S_upladder  = 34,
    S_dnladder  = 35,
    S_anvil     = 36,
    S_altar     = 37,
    S_extra_boulder = 38,
    S_grave     = 28+11,
    S_throne    = 29+11,
    S_sink      = 30+11,
    S_fountain  = 31+11,
    S_pool      = 32+11,
    S_ice       = 33+11,
    S_lava      = 34+11,
    S_vodbridge = 35+11,
    S_hodbridge = 36+11,
    S_vcdbridge = 37+11, /* closed drawbridge+11, vertical wall */
    S_hcdbridge = 38+11, /* closed drawbridge+11, horizontal wall */
    S_air       = 39+11,
    S_cloud     = 40+11,
    S_water     = 41+11,

/* end dungeon characters+11, begin traps */

    S_arrow_trap           = 42+11,
    S_dart_trap            = 43+11,
    S_falling_rock_trap    = 44+11,
    S_squeaky_board        = 45+11,
    S_bear_trap            = 46+11,
    S_land_mine            = 47+11,
    S_rolling_boulder_trap = 48+11,
    S_sleeping_gas_trap    = 49+11,
    S_rust_trap            = 50+11,
    S_fire_trap            = 51+11,
    S_pit                  = 52+11,
    S_spiked_pit           = 53+11,
    S_hole                 = 54+11,
    S_trap_door            = 55+11,
    S_teleportation_trap   = 56+11,
    S_level_teleporter     = 57+11,
    S_magic_portal         = 58+11,
    S_web                  = 59+11,
    S_statue_trap          = 60+11,
    S_magic_trap           = 61+11,
    S_anti_magic_trap      = 62+11,
    S_polymorph_trap       = 63+11,
	S_modron_portal	       = 64+11,
    S_vibrating_square     = 65+11, /* for display rather than any trap effect */

/* end traps+11, begin special effects */

    S_vbeam     = 65+12, /* The 4 zap beam symbols.  Do NOT separate. */
    S_hbeam     = 66+12, /* To change order or add+12, see function      */
    S_lslant    = 67+12, /* zapdir_to_glyph() in display.c.           */
    S_rslant    = 68+12,
    S_digbeam   = 69+12, /* dig beam symbol */
    S_flashbeam = 70+12, /* camera flash symbol */
    S_boomleft  = 71+12, /* thrown boomerang+12, open left+12, e.g ')'    */
    S_boomright = 72+12, /* thrown boomerang+12, open right+12, e.g. '('  */
    S_ss1       = 73+12, /* 4 magic shield ("resistance sparkle") glyphs */
    S_ss2       = 74+12,
    S_ss3       = 75+12,
    S_ss4       = 76+12,
    S_talkeffect = 77+12,
    S_poisoncloud = 78+12,
    S_goodpos   = 79+12, /* valid position for targeting via getpos() */

/* The 8 swallow symbols.  Do NOT separate.  To change order or add+12, */
/* see the function swallow_to_glyph() in display.c.                 */
    S_sw_tl     = 80+12, /* swallow top left [1]             */
    S_sw_tc     = 81+12, /* swallow top center [2]    Order: */
    S_sw_tr     = 82+12, /* swallow top right [3]            */
    S_sw_ml     = 83+12, /* swallow middle left [4]          */
    S_sw_mc     = 84+12, /* swallow middle center [5] 1 2 3  */
    S_sw_mr     = 85+12, /* swallow middle right [6]  4 5 6  */
    S_sw_bl     = 86+12, /* swallow bottom left [7]   7 8 9  */
    S_sw_bc     = 87+12, /* swallow bottom center [8]        */
    S_sw_br     = 88+12, /* swallow bottom right [9]         */

    S_explode1  = 89+12, /* explosion top left               */
    S_explode2  = 90+12, /* explosion top center             */
    S_explode3  = 91+12, /* explosion top right        Ex.   */
    S_explode4  = 92+12, /* explosion middle left            */
    S_explode5  = 93+12, /* explosion middle center    /-\   */
    S_explode6  = 94+12, /* explosion middle right     |@|   */
    S_explode7  = 95+12, /* explosion bottom left      \-/   */
    S_explode8  = 96+12, /* explosion bottom center          */
    S_explode9  = 97+12, /* explosion bottom right           */

/* end effects */

    MAX_CMAPPED_CHARS   = 110  /* maximum number of mapped characters */
};

#define MAX_DUNGEON_CHARS (S_water - S_unexplored + 1) /* mapped dungeon characters */
#define MAX_TRAP_CHARS (S_vibrating_square - S_arrow_trap + 1) /* trap chars */
#define MAX_EFFECT_CHARS (S_explode9 - S_vbeam + 1) /* mapped effects characters */
#define MAX_EXPLOSION_CHARS 9 /* number of explosion characters */
#define MAX_SWALLOW_CHARS 9 /* number of swallow characters */
#define MAX_ZAP_CHARS 4 /* number of zap characters */

#define DARKROOMSYM (Is_rogue_level(&u.uz) ? S_unexplored : S_darkroom)
#define DARKGRASSSYM (Is_rogue_level(&u.uz) ? S_unexplored : S_darkgrass)
#define DARKGROUNDSYM (Is_rogue_level(&u.uz) ? S_unexplored : S_darkground)

#define is_cmap_trap(i) ((i) >= S_arrow_trap && (i) <= S_modron_portal)
#define is_cmap_drawbridge(i) ((i) >= S_vodbridge && (i) <= S_hcdbridge)
#define is_cmap_door(i) ((i) >= S_vodoor && (i) <= S_hoportcullis)
#define is_cmap_wall(i) ((i) >= S_stone && (i) <= S_trwall)
#define is_cmap_room(i) ((i) >= S_room && (i) <= S_darkroom)
#define is_cmap_grass(i) ((i) >= S_grass && (i) <= S_darkgrass)
#define is_cmap_ground(i) ((i) >= S_ground && (i) <= S_darkground)
#define is_cmap_corr(i) ((i) >= S_corr && (i) <= S_litcorr)
#define is_cmap_furniture(i) ((i) >= S_upstair && (i) <= S_fountain)
#define is_cmap_water(i) ((i) == S_pool || (i) == S_water)
#define is_cmap_lava(i) ((i) == S_lava)

/*
 * The dungeon presentation graphics code and data structures were rewritten
 * and generalized for GnollHack's release 2 by Eric S. Raymond (eric@snark)
 * building on Don G. Kneller's MS-DOS implementation.	See drawing.c for
 * the code that permits the user to set the contents of the symbol structure.
 *
 * The door representation was changed by Ari
 * Huttunen(ahuttune@niksula.hut.fi)
 */

 /*
  * TLCORNER     TDWALL          TRCORNER
  * +-           -+-             -+
  * |             |               |
  *
  * TRWALL       CROSSWALL       TLWALL          HWALL
  * |             |               |
  * +-           -+-             -+              ---
  * |             |               |
  *
  * BLCORNER     TUWALL          BRCORNER        VWALL
  * |             |               |              |
  * +-           -+-             -+              |
  */

  /* Level location types.  [Some debugging code in src/display.c
     defines array type_names[] which contains an entry for each of
     these, so needs to be kept in sync if any new types are added
     or existing ones renumbered.] */
enum levl_typ_types {
    STONE = 0,
    VWALL = 1,
    HWALL = 2,
    TLCORNER = 3,
    TRCORNER = 4,
    BLCORNER = 5,
    BRCORNER = 6,
    CROSSWALL = 7, /* For pretty mazes and special levels */
    TUWALL = 8,
    TDWALL = 9,
    TLWALL = 10,
    TRWALL = 11,
    DBWALL = 12,
    TREE = 13, /* KMH */
    SDOOR = 14,
    SCORR = 15,
    POOL = 16,
    MOAT = 17, /* pool that doesn't boil, adjust messages */
    WATER = 18,
    DRAWBRIDGE_UP = 19,
    LAVAPOOL = 20,
    IRONBARS = 21, /* KMH */
    DOOR = 22,
    CORR = 23,
    ROOM = 24,
    STAIRS = 25,
    LADDER = 26,
    FOUNTAIN = 27,
    THRONE = 28,
    SINK = 29,
    GRAVE = 30,
    ANVIL = 31,
    ALTAR = 32,
    ICE = 33,
    DRAWBRIDGE_DOWN = 34,
    AIR = 35,
    CLOUD = 36,
    GRASS = 37,
    GROUND = 38,

    UNEXPLORED = 39, /* Keep this last */
    MAX_TYPE = 40,
    INVALID_TYPE = 127
};

struct location_type_definition {
    const char* name;
    enum screen_symbols base_screen_symbol;
    enum levl_typ_types initial_floor_type;
    enum obj_material_types material;
    enum location_soundset_types soundset;
};

extern struct location_type_definition location_type_definitions[MAX_TYPE];

struct category_definition {
    int first_subtype;
    int number_of_subtypes;
};

enum corridor_categories {
    CORRIDOR_CATEGORY_NORMAL = 0,
    MAX_CORRIDOR_CATEGORIES
};

extern struct category_definition corridor_category_definitions[MAX_CORRIDOR_CATEGORIES];


enum grass_categories {
    GRASS_CATEGORY_NORMAL = 0,
    GRASS_CATEGORY_SWAMPY,
    MAX_GRASS_CATEGORIES
};

extern struct category_definition grass_category_definitions[MAX_GRASS_CATEGORIES];

enum grass_subtypes {
    GRASS_SUBTYPE_NORMAL = 0,
    GRASS_SUBTYPE_NORMAL_VARIATION_1,
    GRASS_SUBTYPE_NORMAL_VARIATION_2,
    GRASS_SUBTYPE_SWAMPY,
    MAX_GRASS_SUBTYPES
};

enum ground_categories {
    GROUND_CATEGORY_NORMAL = 0,
    GROUND_CATEGORY_SWAMPY,
    MAX_GROUND_CATEGORIES
};

extern struct category_definition ground_category_definitions[MAX_GROUND_CATEGORIES];

enum ground_subtypes {
    GROUND_SUBTYPE_NORMAL = 0,
    GROUND_SUBTYPE_NORMAL_VARIATION_1,
    GROUND_SUBTYPE_NORMAL_VARIATION_2,
    GROUND_SUBTYPE_SWAMPY,
    MAX_GROUND_SUBTYPES
};

enum floor_categories {
    FLOOR_CATEGORY_NORMAL = 0,
    FLOOR_CATEGORY_MARBLE,
    FLOOR_CATEGORY_PARQUET,
    MAX_FLOOR_CATEGORIES
};

extern struct category_definition floor_category_definitions[MAX_FLOOR_CATEGORIES];

enum floor_subtypes {
    FLOOR_SUBTYPE_NORMAL = 0,
    FLOOR_SUBTYPE_NORMAL_VARIATION_1,
    FLOOR_SUBTYPE_NORMAL_VARIATION_2,
    FLOOR_SUBTYPE_NORMAL_VARIATION_3,
    FLOOR_SUBTYPE_MARBLE,
    FLOOR_SUBTYPE_PARQUET,
    MAX_FLOOR_SUBTYPES
};

enum corridor_subtypes {
    CORRIDOR_SUBTYPE_NORMAL = 0,
    MAX_CORRIDOR_SUBTYPES
};

enum modron_portal_subtypes {
    MODRON_PORTAL_SUBTYPE_BASE = 0, /* Indicates use of base cmap */
    MODRON_PORTAL_SUBTYPE_SPHERICAL,
    MODRON_PORTAL_SUBTYPE_CYLINDRICAL,
    MODRON_PORTAL_SUBTYPE_TETRAHEDRAL,
    MODRON_PORTAL_SUBTYPE_CUBICAL,
    MODRON_PORTAL_SUBTYPE_OCTAHEDRAL,
    MODRON_PORTAL_SUBTYPE_DODECAHEDRAL,
    MODRON_PORTAL_SUBTYPE_ICOSAHEDRAL,
    MAX_MODRON_PORTAL_SUBTYPES
};

enum arrow_trap_subtypes_types
{
    ARROW_TRAP_NORMAL = 0,
    CROSSBOW_BOLT_TRAP,
    MAX_ARROW_TRAP_SUBTYPES
};

/*
 * Avoid using the level types in inequalities:
 * these types are subject to change.
 * Instead, use one of the macros below.
 */
#define IS_WALL(typ) ((typ) && (typ) <= DBWALL && (typ) >= (STONE))
#define IS_STWALL(typ) ((typ) <= DBWALL && (typ) >= (STONE)) /* STONE <= (typ) <= DBWALL */
#define IS_ROCK(typ) ((typ) < POOL)      /* absolutely nonaccessible */
#define IS_DOOR(typ) ((typ) == DOOR)
#define IS_DOORJOIN(typ) (IS_ROCK(typ) || (typ) == IRONBARS)
#define IS_TREE(typ)                                            \
    ((typ) == TREE || (level.flags.arboreal && (typ) == UNEXPLORED))
#define ACCESSIBLE(typ) ((typ) >= DOOR) /* good position */
#define IS_ROOM(typ) ((typ) >= ROOM)    /* ROOM, STAIRS, furniture.. */
#define ZAP_POS(typ) ((typ) >= POOL)
#define SPACE_POS(typ) ((typ) > DOOR)
#define IS_POOL(typ) ((typ) >= POOL && (typ) <= DRAWBRIDGE_UP)
#define IS_THRONE(typ) ((typ) == THRONE)
#define IS_FOUNTAIN(typ) ((typ) == FOUNTAIN)
#define IS_SINK(typ) ((typ) == SINK)
#define IS_GRAVE(typ) ((typ) == GRAVE)
#define IS_ALTAR(typ) ((typ) == ALTAR)
#define IS_ANVIL(typ) ((typ) == ANVIL)
#define IS_DRAWBRIDGE(typ) \
    ((typ) == DRAWBRIDGE_UP || (typ) == DRAWBRIDGE_DOWN)
#define IS_FURNITURE(typ) ((typ) >= STAIRS && (typ) <= ALTAR)
#define IS_AIR(typ) ((typ) == AIR || (typ) == CLOUD)
#define IS_SOFT(typ) ((typ) == AIR || (typ) == CLOUD || IS_POOL(typ))

 /* Location types for which floortyp is zero */
#define IS_FLOOR(typ) ((typ) == ROOM || (typ) == CORR || (typ) == GRASS || (typ) == GROUND  || (typ) == UNEXPLORED || (typ) == AIR )

/* Character maps for various dungeons */
enum cmap_types {
    CMAP_NORMAL = 0,
    CMAP_GNOMISH_MINES = 1,
    CMAP_GEHENNOM = 2,
    CMAP_FORT_LUDIOUS = 3,
    CMAP_SOKOBAN = 4,
    CMAP_SWAMP_STYLE = 5,
    CMAP_GEHENNOM_ALTERNATIVE = 6,
    CMAP_MODRON = 7,
    CMAP_BOVINE = 8,
    CMAP_UNDEAD_STYLE = 9,
    CMAP_CITYSCAPE = 10,
    CMAP_SEWERS = 11,
    CMAP_MAZE = 12,
    CMAP_TOWER = 13,
    CMAP_CAVERNS = 14,
    CMAP_ASTRAL = 15,
    MAX_CMAP_TYPES = 16
};

extern const char* cmap_type_names[MAX_CMAP_TYPES];

struct symdef {
    uchar sym;
    const char* explanation;
#ifdef TEXTCOLOR
    uchar color;
#endif
};

struct symdef_cmap {
    uchar sym;
    const char *explanation;
    const char* broken_explanation;
    uchar layer;
    boolean has_broken_tile;

    int variations;
    int variation_offset;
#ifdef TEXTCOLOR
    uchar color[MAX_CMAP_TYPES];
#endif
    short stand_animation[MAX_CMAP_TYPES];
    short enlargement[MAX_CMAP_TYPES];
    short replacement[MAX_CMAP_TYPES];

    short broken_animation[MAX_CMAP_TYPES];
    short broken_enlargement[MAX_CMAP_TYPES];
    short broken_replacement[MAX_CMAP_TYPES];

    boolean included_in_default_tileset_cmap[MAX_CMAP_TYPES];    /* Note that these go according to the numbering of cmaps in default_tileset, not internal cmaps */
    uchar base_default_tileset_cmap[MAX_CMAP_TYPES];             /* Note that these go according to the numbering of cmaps in default_tileset, not internal cmaps */
};


struct symdef_cmap_variation {
    int base_screen_symbol;
    const char* variation_name;
    const char* explanation;
    const char* broken_explanation;
    boolean has_broken_tile;

#ifdef TEXTCOLOR
    uchar color[MAX_CMAP_TYPES];
#endif
    short stand_animation[MAX_CMAP_TYPES];
    short enlargement[MAX_CMAP_TYPES];
    short replacement[MAX_CMAP_TYPES];

    short broken_animation[MAX_CMAP_TYPES];
    short broken_enlargement[MAX_CMAP_TYPES];
    short broken_replacement[MAX_CMAP_TYPES];

    boolean included_in_default_tileset_cmap[MAX_CMAP_TYPES];    /* Note that these go according to the numbering of cmaps in default_tileset, not internal cmaps */
    uchar base_default_tileset_cmap[MAX_CMAP_TYPES];             /* Note that these go according to the numbering of cmaps in default_tileset, not internal cmaps */
};

/* Variation definitions */
#define CRWALL_VARIATION_OFFSET (0)
#define CRWALL_VARIATIONS 1

#define FLOOR_VARIATION_OFFSET (CRWALL_VARIATIONS + CRWALL_VARIATION_OFFSET)
#define FLOOR_VARIATIONS (MAX_FLOOR_SUBTYPES - 1)

#define GRASS_VARIATION_OFFSET (FLOOR_VARIATIONS + FLOOR_VARIATION_OFFSET)
#define GRASS_VARIATIONS (MAX_GRASS_SUBTYPES - 1)

#define GROUND_VARIATION_OFFSET (GRASS_VARIATIONS + GRASS_VARIATION_OFFSET)
#define GROUND_VARIATIONS (MAX_GROUND_SUBTYPES - 1)

#define STAIRCASE_UP_VARIATION_OFFSET (GROUND_VARIATIONS + GROUND_VARIATION_OFFSET)
enum staircase_variation_types
{
    BRANCH_STAIRCASE = 0,
    SPECIAL_BRANCH_STAIRCASE,
    STAIRCASE_VARIATIONS
};

#define STAIRCASE_DOWN_VARIATION_OFFSET (STAIRCASE_VARIATIONS + STAIRCASE_UP_VARIATION_OFFSET)

#define ALTAR_VARIATION_OFFSET (STAIRCASE_VARIATIONS + STAIRCASE_DOWN_VARIATION_OFFSET)
enum altar_variation_types
{
    ALTAR_VARIATION_LAWFUL = 0,
    ALTAR_VARIATION_NEUTRAL,
    ALTAR_VARIATION_CHAOTIC,
    ALTAR_VARIATION_MOLOCH,
    ALTAR_VARIATION_HIGH,
    ALTAR_VARIATIONS
};

#define FOUNTAIN_VARIATION_OFFSET (ALTAR_VARIATIONS + ALTAR_VARIATION_OFFSET)
#define FOUNTAIN_VARIATIONS (MAX_FOUNTAIN_SUBTYPES) /* Slightly unusual: also subtype 0 has a variation for historical reasons */
#define ARROW_TRAP_VARIATION_OFFSET (FOUNTAIN_VARIATIONS + FOUNTAIN_VARIATION_OFFSET)
#define ARROW_TRAP_VARIATIONS (MAX_ARROW_TRAP_SUBTYPES - 1)
#define MODRON_PORTAL_VARIATION_OFFSET (ARROW_TRAP_VARIATIONS + ARROW_TRAP_VARIATION_OFFSET)
#define MODRON_PORTAL_VARIATIONS (MAX_MODRON_PORTAL_SUBTYPES - 1)
#define MAX_VARIATIONS (MODRON_PORTAL_VARIATIONS + MODRON_PORTAL_VARIATION_OFFSET)

#define is_wall_variation(idx) (defsym_variations[(idx)].base_screen_symbol >= S_vwall && defsym_variations[(idx)].base_screen_symbol <= S_trwall) // ((idx) >= HWALL_VARIATION_OFFSET && (idx) < FLOOR_VARIATION_OFFSET)
#define is_base_cmap_variation(idx) (defsym_variations[(idx)].base_screen_symbol >= S_stone && defsym_variations[(idx)].base_screen_symbol <= S_dnladder) // ((idx) >= HWALL_VARIATION_OFFSET && (idx) < ALTAR_VARIATION_OFFSET)

struct symparse {
    unsigned range;
#define SYM_CONTROL 1 /* start/finish markers */
#define SYM_PCHAR 2   /* index into showsyms  */
#define SYM_OC 3      /* index into oc_syms   */
#define SYM_MON 4     /* index into monsyms   */
#define SYM_OTH 5     /* misc                 */
    int idx;
    const char *name;
};

/* misc symbol definitions */
#define SYM_BOULDER 0
#define SYM_INVISIBLE 1
#define MAXOTHER 2

/* linked list of symsets and their characteristics */
struct symsetentry {
    struct symsetentry *next; /* next in list                         */
    char *name;               /* ptr to symset name                   */
    char *desc;               /* ptr to description                   */
    int idx;                  /* an index value                       */
    int handling;             /* known handlers value                 */
    Bitfield(nocolor, 1);     /* don't use color if set               */
    Bitfield(primary, 1);     /* restricted for use as primary set    */
    Bitfield(rogue, 1);       /* restricted for use as rogue lev set  */
                              /* 5 free bits */
};

/*
 * Graphics sets for display symbols
 */
#define DEFAULT_GRAPHICS 0 /* regular characters: '-', '+', &c */
#define PRIMARY 0          /* primary graphics set        */
#define ROGUESET 1         /* rogue graphics set          */
#define NUM_GRAPHICS 2

/*
 * special symbol set handling types ( for invoking callbacks, etc.)
 * Must match the order of the known_handlers strings
 * in drawing.c
 */
#define H_UNK     0
#define H_IBM     1
#define H_DEC     2
#define H_CURS    3

extern const struct symdef_cmap defsyms[MAX_CMAPPED_CHARS]; /* defaults */
extern const struct symdef_cmap_variation defsym_variations[MAX_VARIATIONS];
extern const struct symdef def_warnsyms[WARNCOUNT];
extern int currentgraphics; /* from drawing.c */
extern nhsym showsyms[];
extern nhsym l_syms[];
extern nhsym r_syms[];

extern struct symsetentry symset[NUM_GRAPHICS]; /* from drawing.c */
#define SYMHANDLING(ht) (symset[currentgraphics].handling == (ht))

/*
 * The 5 possible states of doors
 */

#define D_NODOOR 0
#define D_BROKEN 1
#define D_ISOPEN 2
#define D_CLOSED 4
#define D_LOCKED 8
#define D_TRAPPED 16
#define D_SECRET 32 /* only used by sp_lev.c, NOT in rm-struct */
#define D_PORTCULLIS 64 /* this is an open portcullis, similar to D_NODOOR */

/*
 * Some altars are considered as shrines, so we need a flag.
 */
#define AM_SHRINE 8

/*
 * Thrones should only be looted once.
 */
#define T_LOOTED 1

/*
 * Trees have more than one kick result.
 */
#define TREE_LOOTED 1
#define TREE_SWARM 2

/*
 * Fountains have limits, and special warnings.
 */
#define F_LOOTED 16
#define F_WARNED 32
#define F_KNOWN 64
#define FOUNTAIN_IS_WARNED(x, y) (levl[x][y].looted & F_WARNED)
#define FOUNTAIN_IS_LOOTED(x, y) (levl[x][y].looted & F_LOOTED)
#define FOUNTAIN_IS_KNOWN(x, y) (levl[x][y].looted & F_KNOWN)
#define SET_FOUNTAIN_WARNED(x, y) levl[x][y].looted |= F_WARNED;
#define SET_FOUNTAIN_LOOTED(x, y) levl[x][y].looted |= F_LOOTED;
#define SET_FOUNTAIN_KNOWN(x, y) levl[x][y].looted |= F_KNOWN;
#define CLEAR_FOUNTAIN_WARNED(x, y) levl[x][y].looted &= ~F_WARNED;
#define CLEAR_FOUNTAIN_LOOTED(x, y) levl[x][y].looted &= ~F_LOOTED;
#define CLEAR_FOUNTAIN_KNOWN(x, y) levl[x][y].looted &= ~F_KNOWN;

/*
 * Doors are even worse :-) The special warning has a side effect
 * of instantly trapping the door, and if it was defined as trapped,
 * the guards consider that you have already been warned!
 */
#define D_WARNED 16

/*
 * Sinks have 3 different types of loot that shouldn't be abused
 */
#define S_LPUDDING 1
#define S_LDWASHER 2
#define S_LRING 4

/*
 * The four directions for a DrawBridge.
 */
#define DB_NORTH 0
#define DB_SOUTH 1
#define DB_EAST 2
#define DB_WEST 3
#define DB_DIR 3 /* mask for direction */

/*
 * What's under a drawbridge.
 */
#define DB_MOAT 0
#define DB_LAVA 4
#define DB_ICE 8
#define DB_FLOOR 16
#define DB_UNDER 28 /* mask for underneath */

/*
 * Wall information.  Nondiggable also applies to iron bars.
 */
#define WM_MASK 0x07 /* wall mode (bottom three bits) */
#define W_NONDIGGABLE 0x08
#define W_NONPASSWALL 0x10

/*
 * Ladders (in Vlad's tower) may be up or down.
 */
#define LA_UP 1
#define LA_DOWN 2

/*
 * Room areas may be iced pools
 */
#define ICED_POOL 8
#define ICED_MOAT 16

/*
 * The structure describing a coordinate position.
 * Before adding fields, remember that this will significantly affect
 * the size of temporary files and save files.
 *
 * Also remember that the run-length encoding for some ports in save.c
 * must be updated to consider the field.
 */
struct rm {
    struct layer_info hero_memory_layers; // glyph;               /* what the hero thinks is there */
    schar typ;               /* what is really there */
    schar subtyp;            /* grass containing flowers, etc., which may influence tile behavior, drawing, and description */
    schar floortyp;          /* for feature layer typs, what is the floor typ underneath, which may influence GUI drawing */
    schar floorsubtyp;       /* subtyp for floortyp */

    int floor_doodad;        /* a floor doodad layer glyph: previously broken typ, etc., which appears on the broken feature layer above the floor layer */
    int feature_doodad;      /* a feature doodad layer glyph, which appears on the doodad layer above the dungeon feature */

    uchar seenv;             /* seen vector */
    uchar flags;			 /* extra information for typ */
    Bitfield(horizontal, 1); /* wall/door/etc is horiz. (more typ info) */
    Bitfield(lit, 1);        /* speed hack for lit rooms */
    Bitfield(waslit, 1);     /* remember if a location was lit */

    Bitfield(facing_right, 1);      /* flip picture horizontally to "face right" */
    Bitfield(lamplit, 1);           /* if the location is a light source, is it on? */
    Bitfield(makingsound, 1);       /* if the location is a sound source, is it on? */

    Bitfield(roomno, 6); /* room # for special rooms */
    Bitfield(edge, 1);   /* marks boundaries for special rooms*/
    Bitfield(candig, 1); /* Exception to Can_dig_down; was a trapdoor */
};


#define SET_TYPLIT(x, y, ttyp, llit)                              \
    {                                                             \
        if ((x) >= 0 && (y) >= 0 && (x) < COLNO && (y) < ROWNO) { \
            if ((ttyp) < MAX_TYPE)  {                             \
                if(IS_FLOOR(levl[(x)][(y)].typ)) {                \
                     levl[(x)][(y)].floortyp = levl[(x)][(y)].typ; \
                     levl[(x)][(y)].floorsubtyp = levl[(x)][(y)].subtyp; \
                }                                                 \
                levl[(x)][(y)].typ = (ttyp);                      \
                levl[(x)][(y)].subtyp = get_initial_location_subtype(ttyp); \
            }                                                     \
            if ((ttyp) == LAVAPOOL)                               \
                levl[(x)][(y)].lit = 1;                           \
            else if ((schar)(llit) != -2) {                       \
                if ((schar)(llit) == -1)                          \
                    levl[(x)][(y)].lit = rn2(2);                  \
                else                                              \
                    levl[(x)][(y)].lit = (llit);                  \
            }                                                     \
        }                                                         \
    }

/*
 * Add wall angle viewing by defining "modes" for each wall type.  Each
 * mode describes which parts of a wall are finished (seen as as wall)
 * and which are unfinished (seen as rock).
 *
 * We use the bottom 3 bits of the flags field for the mode.  This comes
 * in conflict with secret doors, but we avoid problems because until
 * a secret door becomes discovered, we know what sdoor's bottom three
 * bits are.
 *
 * The following should cover all of the cases.
 *
 *      type    mode                            Examples: R=rock, F=finished
 *      -----   ----                            ----------------------------
 *      WALL:   0 none                          hwall, mode 1
 *              1 left/top (1/2 rock)                   RRR
 *              2 right/bottom (1/2 rock)               ---
 *                                                      FFF
 *
 *      CORNER: 0 none                          trcorn, mode 2
 *              1 outer (3/4 rock)                      FFF
 *              2 inner (1/4 rock)                      F+-
 *                                                      F|R
 *
 *      TWALL:  0 none                          tlwall, mode 3
 *              1 long edge (1/2 rock)                  F|F
 *              2 bottom left (on a tdwall)             -+F
 *              3 bottom right (on a tdwall)            R|F
 *
 *      CRWALL: 0 none                          crwall, mode 5
 *              1 top left (1/4 rock)                   R|F
 *              2 top right (1/4 rock)                  -+-
 *              3 bottom left (1/4 rock)                F|R
 *              4 bottom right (1/4 rock)
 *              5 top left & bottom right (1/2 rock)
 *              6 bottom left & top right (1/2 rock)
 */

#define WM_W_LEFT 1 /* vertical or horizontal wall */
#define WM_W_RIGHT 2
#define WM_W_TOP WM_W_LEFT
#define WM_W_BOTTOM WM_W_RIGHT

#define WM_C_OUTER 1 /* corner wall */
#define WM_C_INNER 2

#define WM_T_LONG 1 /* T wall */
#define WM_T_BL 2
#define WM_T_BR 3

#define WM_X_TL 1 /* cross wall */
#define WM_X_TR 2
#define WM_X_BL 3
#define WM_X_BR 4
#define WM_X_TLBR 5
#define WM_X_BLTR 6

/*
 * Seen vector values.  The seen vector is an array of 8 bits, one for each
 * octant around a given center x:
 *
 *              0 1 2
 *              7 x 3
 *              6 5 4
 *
 * In the case of walls, a single wall square can be viewed from 8 possible
 * directions.  If we know the type of wall and the directions from which
 * it has been seen, then we can determine what it looks like to the hero.
 */
#define SV0   0x01
#define SV1   0x02
#define SV2   0x04
#define SV3   0x08
#define SV4   0x10
#define SV5   0x20
#define SV6   0x40
#define SV7   0x80
#define SVALL 0xFF

#define doormask flags
#define altarmask flags
#define fountainmask flags

#define wall_info flags
#define ladder flags
#define drawbridgemask flags
#define looted flags
#define icedpool flags

#define blessedftn horizontal /* a fountain that grants attribs */
#define disturbed horizontal  /* a grave that has been disturbed */

struct damage {
    struct damage *next;
    long when, cost;
    coord place;
    schar typ;
};

/* for bones levels:  identify the dead character, who might have died on
   an existing bones level; if so, most recent victim will be first in list */
struct cemetery {
    struct cemetery *next; /* next struct is previous dead character... */
    /* "plname" + "-ROLe" + "-RACe" + "-GENder" + "-ALIgnment" + \0 */
    char who[PL_NSIZ + 4 * (1 + 3) + 1];
    /* death reason, same as in score/log file */
    char how[100 + 1]; /* [DTHSZ+1] */
    /* date+time in string of digits rather than binary */
    char when[4 + 2 + 2 + 2 + 2 + 2 + 1]; /* "YYYYMMDDhhmmss\0" */
    /* final resting place spot */
    schar frpx, frpy;
    boolean bonesknown;
};

struct levelflags {
    uchar tileset;
    uchar nfountains; /* number of fountains on level */
    uchar nsinks;     /* number of sinks on the level */
    /* Several flags that give hints about what's on the level */
    Bitfield(has_tileset, 1);

    Bitfield(has_shop, 1);
    Bitfield(has_vault, 1);
    Bitfield(has_zoo, 1);
    Bitfield(has_court, 1);
    Bitfield(has_morgue, 1);
    Bitfield(has_beehive, 1);
    Bitfield(has_barracks, 1);
    Bitfield(has_armory, 1);
    Bitfield(has_temple, 1);
    Bitfield(has_smithy, 1);
    Bitfield(has_npc_room, 1);
    Bitfield(has_library, 1);
	Bitfield(has_dragonlair, 1);
	Bitfield(has_garden, 1);
	Bitfield(has_desertedshop, 1);

    Bitfield(has_swamp, 1);
    Bitfield(noteleport, 1);
    Bitfield(hardfloor, 1);
    Bitfield(nommap, 1);
    Bitfield(hero_memory, 1);   /* hero has memory */
    Bitfield(shortsighted, 1);  /* monsters are shortsighted */
    Bitfield(graveyard, 1);     /* has_morgue, but remains set */
    Bitfield(sokoban_rules, 1); /* fill pits and holes w/ boulders */

    Bitfield(is_maze_lev, 1);
    Bitfield(is_cavernous_lev, 1);
    Bitfield(arboreal, 1);     /* Trees replace rock */
    Bitfield(swampy, 1);       /* Swampy outdoor level with swampy grass */
    Bitfield(wizard_bones, 1); /* set if level came from a bones file
                                  which was created in wizard mode (or
                                  normal mode descendant of such) */
    Bitfield(corrmaze, 1);     /* Whether corridors are used for the maze
                                  rather than ROOM */
    Bitfield(throne_on_ground, 1); /* Throne symbols on special level maps are on ground  */
    Bitfield(fountain_on_grass, 1); /* Fountain symbols on special level maps are on grass */
    Bitfield(fountain_on_ground, 1); /* Fountain and tree symbols on special level maps are on ground */
};

typedef struct {
    struct rm locations[COLNO][ROWNO];
#ifndef MICROPORT_BUG
    struct obj *objects[COLNO][ROWNO];
    struct monst *monsters[COLNO][ROWNO];
#else
    struct obj *objects[1][ROWNO];
    char *yuk1[COLNO - 1][ROWNO];
    struct monst *monsters[1][ROWNO];
    char *yuk2[COLNO - 1][ROWNO];
#endif
    struct obj *objlist;
    struct obj *buriedobjlist;
    struct monst *monlist;
    struct damage *damagelist;
    struct cemetery *bonesinfo;
    struct levelflags flags;
} dlevel_t;

extern schar lastseentyp[COLNO][ROWNO]; /* last seen/touched dungeon typ */

extern dlevel_t level; /* structure describing the current level */

/*
 * Macros for compatibility with old code. Someday these will go away.
 */
#define levl level.locations
#define fobj level.objlist
#define fmon level.monlist

/*
 * Covert a trap number into the defsym graphics array.
 * Convert a defsym number into a trap number.
 * Assumes that arrow trap will always be the first trap.
 */
#define trap_to_defsym(t) (S_arrow_trap + (t) -1)
#define defsym_to_trap(d) ((d) -S_arrow_trap + 1)

#define OBJ_AT(x, y) (level.objects[x][y] != (struct obj *) 0)
/*
 * Macros for encapsulation of level.monsters references.
 */
#define MON_AT(x, y)                            \
    (level.monsters[x][y] != (struct monst *) 0 \
     && !(level.monsters[x][y])->mburied)
#define MON_BURIED_AT(x, y)                     \
    (level.monsters[x][y] != (struct monst *) 0 \
     && (level.monsters[x][y])->mburied)
#ifdef EXTRA_SANITY_CHECKS
#define place_worm_seg(m, x, y) \
    do {                                                        \
        if (level.monsters[x][y] && level.monsters[x][y] != m)  \
            impossible("place_worm_seg over mon");              \
        level.monsters[x][y] = m;                               \
    } while(0)
#define remove_monster(x, y) \
    do {                                                \
        if (!level.monsters[x][y])                      \
            impossible("no monster to remove");         \
        level.monsters[x][y] = (struct monst *) 0;      \
    } while(0)
#else
#define place_worm_seg(m, x, y) level.monsters[x][y] = m
#define remove_monster(x, y) level.monsters[x][y] = (struct monst *) 0
#endif
#define m_at(x, y) (MON_AT(x, y) ? level.monsters[x][y] : (struct monst *) 0)
#define m_buried_at(x, y) \
    (MON_BURIED_AT(x, y) ? level.monsters[x][y] : (struct monst *) 0)

/* restricted movement, potential luck penalties */
#define Sokoban level.flags.sokoban_rules

#endif /* RM_H */

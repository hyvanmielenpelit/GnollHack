/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    rm.h    $NHDT-Date: 1547255911 2019/01/12 01:18:31 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.60 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef RM_H
#define RM_H

#include "layer.h"
#include "soundset.h"
#include "general.h"


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
    S_grave     = 39,
    S_brazier   = 40,
    S_signpost  = 41,
    S_throne    = 29+13,
    S_sink      = 30+13,
    S_fountain  = 31+13,
    S_pool      = 32+13,
    S_ice       = 33+13,
    S_lava      = 34+13,
    S_vodbridge = 35+13,
    S_hodbridge = 36+13,
    S_vcdbridge = 37+13, /* closed drawbridge+13, vertical wall */
    S_hcdbridge = 38+13, /* closed drawbridge+13, horizontal wall */
    S_air       = 39+13,
    S_cloud     = 40+13,
    S_water     = 41+13,

/* end dungeon characters+13, begin traps */

    S_arrow_trap           = 42+13,
    S_dart_trap            = 43+13,
    S_falling_rock_trap    = 44+13,
    S_squeaky_board        = 45+13,
    S_bear_trap            = 46+13,
    S_land_mine            = 47+13,
    S_rolling_boulder_trap = 48+13,
    S_sleeping_gas_trap    = 49+13,
    S_rust_trap            = 50+13,
    S_fire_trap            = 51+13,
    S_pit                  = 52+13,
    S_spiked_pit           = 53+13,
    S_hole                 = 54+13,
    S_trap_door            = 55+13,
    S_teleportation_trap   = 56+13,
    S_level_teleporter     = 57+13,
    S_magic_portal         = 58+13,
    S_web                  = 59+13,
    S_statue_trap          = 60+13,
    S_magic_trap           = 61+13,
    S_anti_magic_trap      = 62+13,
    S_polymorph_trap       = 63+13,
    S_modron_portal           = 64+13,
    S_lever                = 65+13,
    S_vibrating_square     = 66+13, /* for display rather than any trap effect */

/* end traps+13, begin special effects */

    S_vbeam     = 65+15, /* The 4 zap beam symbols.  Do NOT separate. */
    S_hbeam     = 66+15, /* To change order or add+15, see function      */
    S_lslant    = 67+15, /* zapdir_to_glyph() in display.c.           */
    S_rslant    = 68+15,
    S_digbeam  = 69+15, /* dig beam symbol */
    S_flashbeam = 70+15, /* camera flash symbol */
    S_boom_tl = 71 +15, /* thrown boomerang flying towards top left [1]             */
    S_boom_tc = 72 +15, /* thrown boomerang flying towards top center [2]    Order: */
    S_boom_tr = 73 +15, /* thrown boomerang flying towards top right [3]            */
    S_boom_ml = 74 +15, /* thrown boomerang flying towards middle left [4]   1 2 3  */
    S_boom_mr = 75 +15, /* thrown boomerang flying towards middle right [5]  4   5  */
    S_boom_bl = 76 +15, /* thrown boomerang flying towards bottom left [6]   6 7 8  */
    S_boom_bc = 77 +15, /* thrown boomerang flying towards bottom center [7]        */
    S_boom_br = 78 +15, /* thrown boomerang flying towards bottom right [8]         */
    S_ss1       = 79+15, /* 4 magic shield ("resistance sparkle") glyphs */
    S_ss2       = 80+15,
    S_ss3       = 81+15,
    S_ss4       = 82+15,
    S_talkeffect = 83+15,
    S_poisoncloud = 84+15,
    S_goodpos   = 85+15, /* valid position for targeting via getpos() */

/* The 8 swallow symbols.  Do NOT separate.  To change order or add+21, */
/* see the function swallow_to_glyph() in display.c.                 */
    S_sw_tl     = 80+21, /* swallow top left [1]             */
    S_sw_tc     = 81+21, /* swallow top center [2]    Order: */
    S_sw_tr     = 82+21, /* swallow top right [3]            */
    S_sw_ml     = 83+21, /* swallow middle left [4]          */
    S_sw_mc     = 84+21, /* swallow middle center [5] 1 2 3  */
    S_sw_mr     = 85+21, /* swallow middle right [6]  4 5 6  */
    S_sw_bl     = 86+21, /* swallow bottom left [7]   7 8 9  */
    S_sw_bc     = 87+21, /* swallow bottom center [8]        */
    S_sw_br     = 88+21, /* swallow bottom right [9]         */

    S_explode1  = 89+21, /* explosion top left               */
    S_explode2  = 90+21, /* explosion top center             */
    S_explode3  = 91+21, /* explosion top right        Ex.   */
    S_explode4  = 92+21, /* explosion middle left            */
    S_explode5  = 93+21, /* explosion middle center    /-\   */
    S_explode6  = 94+21, /* explosion middle right     |@|   */
    S_explode7  = 95+21, /* explosion bottom left      \-/   */
    S_explode8  = 96+21, /* explosion bottom center          */
    S_explode9  = 97+21, /* explosion bottom right           */

/* end effects */

    MAX_CMAPPED_CHARS   = 119  /* maximum number of mapped characters */
};

#define MAX_DUNGEON_CHARS (S_water - S_unexplored + 1) /* mapped dungeon characters */
#define MAX_TRAP_CHARS (S_vibrating_square - S_arrow_trap + 1) /* trap chars */
#define MAX_EFFECT_CHARS (S_explode9 - S_vbeam + 1) /* mapped effects characters */
#define MAX_EXPLOSION_CHARS 9 /* number of explosion characters */
#define MAX_SWALLOW_CHARS 9 /* number of swallow characters */
#define NUM_ZAP_CHARS 56 /* number of zap characters */
#define NUM_ASCII_ZAP_CHARS 4 /* number of zap symbols in ascii */

#define DARKROOMSYM (Is_really_rogue_level(&u.uz) ? S_unexplored : S_darkroom)
#define DARKGRASSSYM (Is_really_rogue_level(&u.uz) ? S_unexplored : S_darkgrass)
#define DARKGROUNDSYM (Is_really_rogue_level(&u.uz) ? S_unexplored : S_darkground)

#define is_cmap_trap(i) ((i) >= S_arrow_trap && (i) <= S_vibrating_square)
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
#define is_cmap_brazier(i) ((i) == S_brazier)

/*
 * The dungeon presentation graphics code and data structures were rewritten
 * and generalized for GnollHack's release 2 by Eric S. Raymond (eric@snark)
 * building on Don G. Kneller's MS-DOS implementation.    See drawing.c for
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
    BRAZIER = 31,
    SIGNPOST = 32,
    ANVIL = 33,
    ALTAR = 34,
    ICE = 35,
    DRAWBRIDGE_DOWN = 36,
    AIR = 37,
    CLOUD = 38,
    GRASS = 39,
    GROUND = 40,

    UNDEFINED_LOCATION = 41, /* Keep this last */
    MAX_TYPE = 42,
    INVALID_TYPE = 127
};

enum decoration_types {
    DECORATION_NONE = 0,
    DECORATION_COBWEB,
    DECORATION_COBWEB_CORNER,
    DECORATION_TORCH,
    DECORATION_LANTERN,
    MAX_DECORATIONS
};

struct decoration_type_definition {
    const char* name;
    const char* description;
    const char* description_filled;
    int first_doodad;
    int num_subtypes;
    int lootable_item;
    int color;
    enum location_soundset_types soundset;
    short animation;
    short enlargement;
    short replacement;
    unsigned short dflags;
};

extern struct decoration_type_definition decoration_type_definitions[MAX_DECORATIONS];

#define DECORATION_TYPE_FLAGS_NONE               0x0000
#define DECORATION_TYPE_FLAGS_LOOTABLE           0x0001
#define DECORATION_TYPE_FLAGS_LIGHTABLE          0x0002
#define DECORATION_TYPE_FLAGS_UNDERSCORE         0x0004
#define DECORATION_TYPE_FLAGS_ALL_SIDES          0x0008
#define DECORATION_TYPE_FLAGS_CORNERS            0x0010
#define DECORATION_TYPE_FLAGS_MIRRORABLE         0x0020

struct location_type_definition {
    const char* name;
    enum screen_symbols base_screen_symbol;
    enum levl_typ_types initial_floor_type;
    enum obj_material_types material;
    enum location_soundset_types soundset;
};

extern struct location_type_definition location_type_definitions[MAX_TYPE];

struct location_subtype_definition {
    const char* subtype_name;
    int number_of_vartypes;
    int variation_offset;
};

/* Floor subtypes and categories */

#define CORRIDOR_SUBTYPE_NORMAL_VARIATIONS 4
#define TOTAL_CORRIDOR_SUBTYPE_VARIATIONS (CORRIDOR_SUBTYPE_NORMAL_VARIATIONS)
enum corridor_subtypes {
    CORRIDOR_SUBTYPE_NORMAL = 0,
    MAX_CORRIDOR_SUBTYPES
};

extern struct location_subtype_definition corridor_subtype_definitions[MAX_CORRIDOR_SUBTYPES];

#define GRASS_SUBTYPE_NORMAL_VARIATIONS 3
#define GRASS_SUBTYPE_SWAMPY_VARIATIONS 1
#define TOTAL_GRASS_SUBTYPE_VARIATIONS (GRASS_SUBTYPE_NORMAL_VARIATIONS + GRASS_SUBTYPE_SWAMPY_VARIATIONS)
enum grass_subtypes {
    GRASS_SUBTYPE_NORMAL = 0,
    GRASS_SUBTYPE_SWAMPY,
    MAX_GRASS_SUBTYPES
};

extern struct location_subtype_definition grass_subtype_definitions[MAX_GRASS_SUBTYPES];

#define GROUND_SUBTYPE_NORMAL_VARIATIONS 4
#define GROUND_SUBTYPE_SWAMPY_VARIATIONS 1
#define GROUND_SUBTYPE_DESERT_SAND_VARIATIONS 1
#define TOTAL_GROUND_SUBTYPE_VARIATIONS (GROUND_SUBTYPE_NORMAL_VARIATIONS + GROUND_SUBTYPE_SWAMPY_VARIATIONS + GROUND_SUBTYPE_DESERT_SAND_VARIATIONS)
enum ground_subtypes {
    GROUND_SUBTYPE_NORMAL = 0,
    GROUND_SUBTYPE_SWAMPY,
    GROUND_SUBTYPE_DESERT_SAND,
    MAX_GROUND_SUBTYPES
};

extern struct location_subtype_definition ground_subtype_definitions[MAX_GROUND_SUBTYPES];

#define FLOOR_SUBTYPE_NORMAL_VARIATIONS 4
#define FLOOR_SUBTYPE_MARBLE_VARIATIONS 4
#define FLOOR_SUBTYPE_PARQUET_VARIATIONS 4
#define FLOOR_SUBTYPE_COBBLESTONE_VARIATIONS 4
#define TOTAL_FLOOR_SUBTYPE_VARIATIONS (FLOOR_SUBTYPE_NORMAL_VARIATIONS + FLOOR_SUBTYPE_MARBLE_VARIATIONS + FLOOR_SUBTYPE_PARQUET_VARIATIONS + FLOOR_SUBTYPE_COBBLESTONE_VARIATIONS)
enum floor_subtypes {
    FLOOR_SUBTYPE_NORMAL = 0,
    FLOOR_SUBTYPE_MARBLE,
    FLOOR_SUBTYPE_PARQUET,
    FLOOR_SUBTYPE_COBBLESTONE,
    MAX_FLOOR_SUBTYPES
};

extern struct location_subtype_definition floor_subtype_definitions[MAX_FLOOR_SUBTYPES];

/* Stone and wall subtypes and categories */
#define NUM_NORMAL_STONE_VARTYPES 4

#define STONE_SUBTYPE_NORMAL_VARIATIONS NUM_NORMAL_STONE_VARTYPES
#define TOTAL_STONE_SUBTYPE_VARIATIONS (STONE_SUBTYPE_NORMAL_VARIATIONS)
enum stone_subtypes {
    STONE_SUBTYPE_NORMAL = 0,
    MAX_STONE_SUBTYPES
};

extern struct location_subtype_definition stone_subtype_definitions[MAX_STONE_SUBTYPES];

#define WALL_SUBTYPE_NORMAL_VARIATIONS NUM_NORMAL_STONE_VARTYPES
#define TOTAL_WALL_SUBTYPE_VARIATIONS (WALL_SUBTYPE_NORMAL_VARIATIONS)

enum wall_subtypes {
    WALL_SUBTYPE_STONE = 0,
    MAX_WALL_SUBTYPES
};

extern struct location_subtype_definition wall_subtype_definitions[MAX_WALL_SUBTYPES];

/* Other subtypes */
#define ALTAR_SUBTYPE_NORMAL_VARIATIONS 4
#define ALTAR_SUBTYPE_HIGH_VARIATIONS 1
#define ALTAR_SUBTYPE_MOLOCH_VARIATIONS 1
#define TOTAL_ALTAR_SUBTYPE_VARIATIONS (ALTAR_SUBTYPE_NORMAL_VARIATIONS + ALTAR_SUBTYPE_HIGH_VARIATIONS + ALTAR_SUBTYPE_MOLOCH_VARIATIONS)
enum altar_subtypes
{
    ALTAR_SUBTYPE_NORMAL = 0,
    ALTAR_SUBTYPE_HIGH,
    ALTAR_SUBTYPE_MOLOCH,
    MAX_ALTAR_SUBTYPES
};

extern struct location_subtype_definition altar_subtype_definitions[MAX_ALTAR_SUBTYPES];


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

enum door_subtypes_types
{
    DOOR_SUBTYPE_WOODEN = 0,
    DOOR_SUBTYPE_WOODEN_WINDOWED,
    DOOR_SUBTYPE_IRON,
    DOOR_SUBTYPE_IRON_WINDOWED,
    DOOR_SUBTYPE_IRON_BARS,
    DOOR_SUBTYPE_MAGIC,
    DOOR_SUBTYPE_MAGIC_WINDOWED,
    DOOR_SUBTYPE_MODRON,
    DOOR_SUBTYPE_REINFORCED,
    DOOR_SUBTYPE_STONE,
    DOOR_SUBTYPE_OBSIDIAN,
    DOOR_SUBTYPE_BLACK_GATE,
    MAX_DOOR_SUBTYPES
};

struct door_subtype_definition {
    const char* description;
    const char* short_description;
    enum obj_material_types material;
    enum location_soundset_types soundset;
    unsigned long flags;
};

#define DSTFLAGS_NONE                                       0x00000000
#define DSTFLAGS_BLOCKS_VISION_AND_SOUND                    0x00000001
#define DSTFLAGS_BLOCKS_PROJECTILES                         0x00000002
#define DSTFLAGS_BROKEN_BY_KICKING                          0x00000004 /* Cannot be kicked */
#define DSTFLAGS_BROKEN_BY_DIGGING                          0x00000008 /* Cannot be destroyed by digging */
#define DSTFLAGS_BROKEN_BY_STRONG_MONSTERS                  0x00000010 /* Cannot be destroyed by large monsters */
#define DSTFLAGS_BROKEN_BY_STRIKING                         0x00000020 /* Cannot be destroyed by large monsters */
#define DSTFLAGS_BROKEN_BY_BEING_BOOBY_TRAPPED              0x00000040 /* Booby traps destroy the door */
#define DSTFLAGS_INDESTRUCTIBLE                             0x00000080 /* Not destroyed by anything, not even by being booby trapped */
#define DSTFLAGS_NO_LOCK_DESCRIPTION_IS_DEFAULT             0x00000100 /* If no lock, nothing is printed, with normal lock, tells that has a metal lock */
#define DSTFLAGS_NONPASSABLE                                0x00000200 /* Cannot be walked through with wallwalk */

extern struct door_subtype_definition door_subtype_definitions[MAX_DOOR_SUBTYPES];

enum forest_types
{
    FOREST_TYPE_NORMAL = 0,
    FOREST_TYPE_DECIDUOUS,
    FOREST_TYPE_CONIFEROUS,
    FOREST_TYPE_TROPICAL,
    FOREST_TYPE_DESERT,
    FOREST_TYPE_GARDEN,
    FOREST_TYPE_SWAMP,
    FOREST_TYPE_ISLANDS,
    FOREST_TYPE_UNDEAD,
    FOREST_TYPE_GEHENNOM,
    FOREST_TYPE_SNOWY,
    MAX_FOREST_TYPES
};

enum tree_subtypes
{
    TREE_SUBTYPE_NORMAL = 0, /* Uses base_cmap */
    TREE_SUBTYPE_OAK,
    TREE_SUBTYPE_SPRUCE,
    TREE_SUBTYPE_FIR,
    TREE_SUBTYPE_DATE_PALM,
    TREE_SUBTYPE_APPLE,
    TREE_SUBTYPE_BANANA,
    TREE_SUBTYPE_FIG,
    TREE_SUBTYPE_ORANGE,
    TREE_SUBTYPE_PITAYA,
    TREE_SUBTYPE_SMALL,
    TREE_SUBTYPE_FIR_SNOWY,
    TREE_SUBTYPE_WIZENED,
    TREE_SUBTYPE_EUCALYPTUS,
    MAX_TREE_SUBTYPES
};

#define TREE_SUBTYPE_NORMAL_VARIATIONS 1
#define TREE_SUBTYPE_OAK_VARIATIONS 1
#define TREE_SUBTYPE_SPRUCE_VARIATIONS 1
#define TREE_SUBTYPE_FIR_VARIATIONS 1
#define TREE_SUBTYPE_DATE_PALM_VARIATIONS 3
#define TREE_SUBTYPE_APPLE_VARIATIONS 1
#define TREE_SUBTYPE_BANANA_VARIATIONS 1
#define TREE_SUBTYPE_FIG_VARIATIONS 1
#define TREE_SUBTYPE_ORANGE_VARIATIONS 1
#define TREE_SUBTYPE_PITAYA_VARIATIONS 1
#define TREE_SUBTYPE_SMALL_VARIATIONS 1
#define TREE_SUBTYPE_SNOWY_FIR_VARIATIONS 1
#define TREE_SUBTYPE_WIZENED_VARIATIONS 1
#define TREE_SUBTYPE_EUCALYPTUS_VARIATIONS 1
#define TOTAL_TREE_SUBTYPE_VARIATIONS (TREE_SUBTYPE_NORMAL_VARIATIONS + TREE_SUBTYPE_OAK_VARIATIONS + TREE_SUBTYPE_SPRUCE_VARIATIONS + TREE_SUBTYPE_FIR_VARIATIONS + TREE_SUBTYPE_DATE_PALM_VARIATIONS + \
  TREE_SUBTYPE_APPLE_VARIATIONS + TREE_SUBTYPE_BANANA_VARIATIONS + TREE_SUBTYPE_FIG_VARIATIONS + TREE_SUBTYPE_ORANGE_VARIATIONS + TREE_SUBTYPE_PITAYA_VARIATIONS + \
  TREE_SUBTYPE_SMALL_VARIATIONS + TREE_SUBTYPE_SNOWY_FIR_VARIATIONS + TREE_SUBTYPE_WIZENED_VARIATIONS + TREE_SUBTYPE_EUCALYPTUS_VARIATIONS)

struct tree_subtype_definition {
    const char* description;
    const char* short_description;
    int number_of_vartypes;
    int variation_offset;
    uchar tree_type;
    short probability[MAX_FOREST_TYPES];
    int fruit_type;
    short fruit_d;
    short fruit_n;
    short fruit_p;
    uchar fruit_kick_drop_chance;
    short fruit_drop_d;
    short fruit_drop_n;
    short fruit_drop_p;
    short burning_subtype;
    short burnt_subtype;
    unsigned long tree_flags;
};

#define TREE_FLAGS_NONE                     0x00000000
#define TREE_FLAGS_MAY_HAVE_KILLER_BEES     0x00000001

enum tree_classes
{
    TREE_CLASS_GENERAL = 0,
    TREE_CLASS_DECIDUOUS,
    TREE_CLASS_CONIFEROUS,
    TREE_CLASS_TROPICAL,
    TREE_CLASS_WIZENED,
    MAX_TREE_CLASSES
};

extern struct tree_subtype_definition tree_subtype_definitions[MAX_TREE_SUBTYPES];

/*
 * Avoid using the level types in inequalities:
 * these types are subject to change.
 * Instead, use one of the macros below.
 */
#define IS_NON_STONE_WALL(typ) ((typ) && (((typ) < DBWALL && (typ) > (STONE)) || (typ) == SDOOR))
#define IS_WALL(typ) ((typ) && (typ) <= DBWALL) /* && (typ) >= (STONE) */
#define IS_WALL_OR_SDOOR(typ) ((typ) && (IS_WALL(typ) || (typ) == SDOOR))
#define IS_STWALL(typ) ((typ) <= DBWALL) /* && (typ) >= (STONE) */ /* STONE <= (typ) <= DBWALL */
#define IS_ROCK(typ) ((typ) < POOL)      /* absolutely nonaccessible */
#define IS_DOOR(typ) ((typ) == DOOR)
#define IS_DOOR_OR_SDOOR(typ) ((typ) == DOOR || (typ) == SDOOR)
#define IS_DOORJOIN(typ) (IS_ROCK(typ) || (typ) == IRONBARS)
#define IS_TREE(typ)                                            \
    ((typ) == TREE || (level.flags.arboreal && (typ) == UNDEFINED_LOCATION))
#define ACCESSIBLE(typ) ((typ) >= DOOR) /* good position */
#define IS_ROOM(typ) ((typ) >= ROOM)    /* ROOM, STAIRS, furniture.. */
#define ZAP_POS(typ) ((typ) >= POOL)
#define SPACE_POS(typ) ((typ) > DOOR)
#define IS_POOL(typ) ((typ) >= POOL && (typ) <= DRAWBRIDGE_UP)
#define IS_THRONE(typ) ((typ) == THRONE)
#define IS_FOUNTAIN(typ) ((typ) == FOUNTAIN)
#define IS_SINK(typ) ((typ) == SINK)
#define IS_GRAVE(typ) ((typ) == GRAVE)
#define IS_BRAZIER(typ) ((typ) == BRAZIER)
#define IS_SIGNPOST(typ) ((typ) == SIGNPOST)
#define IS_ALTAR(typ) ((typ) == ALTAR)
#define IS_ANVIL(typ) ((typ) == ANVIL)
#define IS_DRAWBRIDGE(typ) \
    ((typ) == DRAWBRIDGE_UP || (typ) == DRAWBRIDGE_DOWN)
#define IS_FURNITURE(typ) ((typ) >= STAIRS && (typ) <= ALTAR)
#define IS_AIR(typ) ((typ) == AIR || (typ) == CLOUD)
#define IS_SOFT(typ) ((typ) == AIR || (typ) == CLOUD || IS_POOL(typ))

#define IS_SOLID_FLOOR(typ) ((typ) == ROOM || (typ) == CORR || (typ) == GRASS || (typ) == GROUND)
 /* Location types for which floortyp is zero */
#define IS_FLOOR(typ) (IS_SOLID_FLOOR(typ) || IS_AIR(typ) || (typ) == UNDEFINED_LOCATION)

/* Character maps for various dungeons */
enum cmap_types {
    CMAP_NORMAL = 0,
    CMAP_GNOMISH_MINES = 1,
    CMAP_GEHENNOM = 2,
    CMAP_UNDEAD_STYLE = 3,
    CMAP_SOKOBAN = 4,
    CMAP_ELEMENTAL_PLANES = 5,
    CMAP_ASTRAL = 6,
    CMAP_GEHENNOM_CAVERNOUS = 7,
    CMAP_GARDEN = 8,
    CMAP_MODRON = 9,
    CMAP_CITYSCAPE = 10,
    CMAP_SEWERS = 11,
    CMAP_MAZE = 12,
    CMAP_TOWER = 13,
    CMAP_CAVERNS = 14,
    CMAP_EXTRA = 15,
    MAX_CMAP_TYPES = 16
};

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

    boolean included_in_cmap[MAX_CMAP_TYPES];
    uchar base_cmap[MAX_CMAP_TYPES]; 
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

    boolean included_in_cmap[MAX_CMAP_TYPES];
    uchar base_cmap[MAX_CMAP_TYPES];
};

/* Variation definitions */
#define CRWALL_VARIATION_OFFSET (0)
#define CRWALL_VARIATIONS 1

#define STONE_VARIATION_OFFSET (CRWALL_VARIATIONS + CRWALL_VARIATION_OFFSET)
#define STONE_VARIATIONS (TOTAL_STONE_SUBTYPE_VARIATIONS - 1)

#define VWALL_VARIATION_OFFSET (STONE_VARIATIONS + STONE_VARIATION_OFFSET)
#define VWALL_VARIATIONS (TOTAL_WALL_SUBTYPE_VARIATIONS - 1)

#define HWALL_VARIATION_OFFSET (VWALL_VARIATIONS + VWALL_VARIATION_OFFSET)
#define HWALL_VARIATIONS (TOTAL_WALL_SUBTYPE_VARIATIONS - 1)

#define FLOOR_VARIATION_OFFSET (HWALL_VARIATIONS + HWALL_VARIATION_OFFSET)
#define FLOOR_VARIATIONS (TOTAL_FLOOR_SUBTYPE_VARIATIONS - 1)

#define GRASS_VARIATION_OFFSET (FLOOR_VARIATIONS + FLOOR_VARIATION_OFFSET)
#define GRASS_VARIATIONS (TOTAL_GRASS_SUBTYPE_VARIATIONS - 1)

#define GROUND_VARIATION_OFFSET (GRASS_VARIATIONS + GRASS_VARIATION_OFFSET)
#define GROUND_VARIATIONS (TOTAL_GROUND_SUBTYPE_VARIATIONS - 1)

#define CORRIDOR_VARIATION_OFFSET (GROUND_VARIATIONS + GROUND_VARIATION_OFFSET)
#define CORRIDOR_VARIATIONS (TOTAL_CORRIDOR_SUBTYPE_VARIATIONS - 1)

#define STAIRCASE_UP_VARIATION_OFFSET (CORRIDOR_VARIATIONS + CORRIDOR_VARIATION_OFFSET)
enum staircase_subtypes
{
    STAIRCASE_NORMAL = 0,
    STAIRCASE_BRANCH,             /* Mines down, sokoban up */
    STAIRCASE_TO_DEEPER,          /* Down to Gehennom or Sanktum, up out of the dungeon branch */
    MAX_STAIRCASE_SUBTYPES
};
#define STAIRCASE_VARIATIONS (MAX_STAIRCASE_SUBTYPES - 1)
#define STAIRCASE_DOWN_VARIATION_OFFSET (STAIRCASE_VARIATIONS + STAIRCASE_UP_VARIATION_OFFSET)

#define ALTAR_VARIATION_OFFSET (STAIRCASE_VARIATIONS + STAIRCASE_DOWN_VARIATION_OFFSET)
#define ALTAR_VARIATIONS (TOTAL_ALTAR_SUBTYPE_VARIATIONS - 1)

#define FOUNTAIN_VARIATION_OFFSET (ALTAR_VARIATIONS + ALTAR_VARIATION_OFFSET)
#define FOUNTAIN_VARIATIONS (MAX_FOUNTAIN_SUBTYPES - 1)
#define ARROW_TRAP_VARIATION_OFFSET (FOUNTAIN_VARIATIONS + FOUNTAIN_VARIATION_OFFSET)
#define ARROW_TRAP_VARIATIONS (MAX_ARROW_TRAP_SUBTYPES - 1)
#define MODRON_PORTAL_VARIATION_OFFSET (ARROW_TRAP_VARIATIONS + ARROW_TRAP_VARIATION_OFFSET)
#define MODRON_PORTAL_VARIATIONS (MAX_MODRON_PORTAL_SUBTYPES - 1)
#define VODOOR_VARIATION_OFFSET (MODRON_PORTAL_VARIATIONS + MODRON_PORTAL_VARIATION_OFFSET)
#define VODOOR_VARIATIONS (MAX_DOOR_SUBTYPES - 1)
#define HODOOR_VARIATION_OFFSET (VODOOR_VARIATIONS + VODOOR_VARIATION_OFFSET)
#define HODOOR_VARIATIONS (MAX_DOOR_SUBTYPES - 1)
#define VCDOOR_VARIATION_OFFSET (HODOOR_VARIATIONS + HODOOR_VARIATION_OFFSET)
#define VCDOOR_VARIATIONS (MAX_DOOR_SUBTYPES - 1)
#define HCDOOR_VARIATION_OFFSET (VCDOOR_VARIATIONS + VCDOOR_VARIATION_OFFSET)
#define HCDOOR_VARIATIONS (MAX_DOOR_SUBTYPES - 1)
#define VBDOOR_VARIATION_OFFSET (HCDOOR_VARIATIONS + HCDOOR_VARIATION_OFFSET)
#define VBDOOR_VARIATIONS (MAX_DOOR_SUBTYPES - 1)
#define HBDOOR_VARIATION_OFFSET (VBDOOR_VARIATIONS + VBDOOR_VARIATION_OFFSET)
#define HBDOOR_VARIATIONS (MAX_DOOR_SUBTYPES - 1)
enum brazier_subtypes
{
    BRAZIER_SUBTYPE_NORMAL = 0, /* Uses base_cmap */
    BRAZIER_SUBTYPE_1,
    BRAZIER_SUBTYPE_2,
    MAX_BRAZIER_SUBTYPES
};
#define BRAZIER_VARIATION_OFFSET (HBDOOR_VARIATIONS + HBDOOR_VARIATION_OFFSET)
#define BRAZIER_VARIATIONS (MAX_BRAZIER_SUBTYPES - 1)
enum signpost_subtypes
{
    SIGNPOST_SUBTYPE_NORMAL = 0, /* Uses base_cmap */
    SIGNPOST_SUBTYPE_1,
    SIGNPOST_SUBTYPE_2,
    MAX_SIGNPOST_SUBTYPES
};
#define SIGNPOST_VARIATION_OFFSET (BRAZIER_VARIATIONS + BRAZIER_VARIATION_OFFSET)
#define SIGNPOST_VARIATIONS (MAX_SIGNPOST_SUBTYPES - 1)
#define TREE_VARIATION_OFFSET (SIGNPOST_VARIATIONS + SIGNPOST_VARIATION_OFFSET)
#define TREE_VARIATIONS (TOTAL_TREE_SUBTYPE_VARIATIONS - 1)

#define MAX_VARIATIONS (TREE_VARIATIONS + TREE_VARIATION_OFFSET)

#define is_wall_variation(idx) (defsym_variations[(idx)].base_screen_symbol >= S_vwall && defsym_variations[(idx)].base_screen_symbol <= S_trwall) // ((idx) >= HWALL_VARIATION_OFFSET && (idx) < FLOOR_VARIATION_OFFSET)
#define is_base_cmap_variation(idx) (defsym_variations[(idx)].base_screen_symbol >= S_stone && defsym_variations[(idx)].base_screen_symbol <= S_dnladder) // ((idx) >= HWALL_VARIATION_OFFSET && (idx) < ALTAR_VARIATION_OFFSET)

struct doodad_definition {
    const char* name;
    const char* description;
    short /*enum replacement_types*/ replacement;
    short /*enum animation_types*/ animation;
    short /*enum enlargement_types*/ enlargement;
};
extern NEARDATA const struct doodad_definition simple_doodads[MAX_SIMPLE_DOODAD_TILES];
extern NEARDATA const struct doodad_definition mirrorable_doodads[MAX_MIRRORABLE_DOODAD_TILES];


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
#define H_UNICODE 4

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
#define D_MASK 0xFF

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
#define TREE_HAS_BEE_HIVE 4

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
#define DB_NORTH    0x0000
#define DB_SOUTH    0x0001
#define DB_EAST     0x0002
#define DB_WEST     0x0003
#define DB_DIR      0x0003 /* mask for direction */

/*
 * What's under a drawbridge.
 */
#define DB_MOAT     0x0000
#define DB_LAVA     0x0004
#define DB_ICE      0x0008
#define DB_GROUND   0x0010
#define DB_UNDER    (DB_MOAT | DB_LAVA | DB_ICE | DB_GROUND) // 28 /* mask for underneath */

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
 * General flags for all locations
 */
#define L_INDESTRUCTIBLE    0x0100  /* Currently used just for doors */
#define L_USES_UP_KEY       0x0200  /* Currently used just for doors */
#define L_NON_PASSDOOR      0x0400  /* Currently used just for doors */
#define L_INITIALLY_UNLIT   0x0800


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
    schar vartyp;            /* variation type for typ / subtyp combination, normally set at random */
    schar floortyp;          /* for feature layer typs, what is the floor typ underneath, which may influence GUI drawing */
    schar floorsubtyp;       /* subtyp for floortyp */
    schar floorvartyp;       /* variation type for floortyp / floorsubtyp combination, normally set at random */

    int floor_doodad;        /* a floor doodad layer glyph: previously broken typ, etc., which appears on the broken feature layer above the floor layer */
    //int feature_doodad;      /* a feature doodad layer glyph, which appears on the doodad layer above the dungeon feature */

    schar decoration_typ;
    schar decoration_subtyp;
    schar decoration_dir;
    uchar decoration_flags;

    short key_otyp;            /* For doors, the otyp that unlocks the door */
    short special_quality;     /* For doors, the special quality of the key that unlocks the door */

    uchar seenv;             /* seen vector */
    unsigned short flags;    /* extra information for typ */
    Bitfield(horizontal, 1); /* wall/door/etc is horiz. (more typ info) */
    Bitfield(lit, 1);        /* speed hack for lit rooms */
    Bitfield(waslit, 1);     /* remember if a location was lit */

    Bitfield(facing_right, 1);      /* flip picture horizontally to "face right" */
    Bitfield(lamplit, 1);           /* if the location is a light source, is it on? */
    Bitfield(makingsound, 1);       /* if the location is a sound source, is it on? */

    Bitfield(roomno, 6); /* room # for special rooms */
    Bitfield(edge, 1);   /* marks boundaries for special rooms*/
    Bitfield(candig, 1); /* Exception to Can_dig_down; was a trapdoor */
    Bitfield(use_special_tileset, 1); /* Use tileset specified below instead standard one */
    Bitfield(special_tileset, 5); /* Specific tileset applicable to this location */

    Bitfield(click_kick_ok, 1); /* No query when clicking to kick  */
};


#define SET_TYPLIT(x, y, ttyp, tsubtyp, llit)                              \
    {                                                             \
        if ((x) >= 0 && (y) >= 0 && (x) < COLNO && (y) < ROWNO) { \
            if ((ttyp) < MAX_TYPE)  {                             \
                if(IS_FLOOR(ttyp)) {                \
                     levl[(x)][(y)].floortyp = 0; \
                     levl[(x)][(y)].floorsubtyp = 0; \
                     levl[(x)][(y)].floorvartyp = 0; \
                }                                                 \
                else if(IS_FLOOR(levl[(x)][(y)].typ)) {                \
                     levl[(x)][(y)].floortyp = levl[(x)][(y)].typ; \
                     levl[(x)][(y)].floorsubtyp = levl[(x)][(y)].subtyp; \
                     levl[(x)][(y)].floorvartyp = levl[(x)][(y)].vartyp; \
                }                                                 \
                else if(!IS_FLOOR(levl[(x)][(y)].floortyp))       \
                {                                                 \
                     levl[(x)][(y)].floortyp = location_type_definitions[ttyp].initial_floor_type; \
                     levl[(x)][(y)].floorsubtyp = get_initial_location_subtype(levl[(x)][(y)].floortyp);         \
                     levl[(x)][(y)].floorvartyp = get_initial_location_vartype(levl[(x)][(y)].floortyp, levl[(x)][(y)].floorsubtyp);         \
                }                                                 \
                levl[(x)][(y)].typ = (ttyp);                      \
                levl[(x)][(y)].subtyp = (tsubtyp) < 0 ? get_initial_location_subtype(ttyp) : (tsubtyp); \
                levl[(x)][(y)].vartyp = get_initial_location_vartype(levl[(x)][(y)].typ, levl[(x)][(y)].subtyp); \
                levl[(x)][(y)].special_quality = 0;                      \
                initialize_location(&levl[(x)][(y)]);             \
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

struct monster_generation_info {
    short mnum;
    char mclass;
    xchar probability;
};

#define MAX_MON_GEN_INFOS 32

struct levelflags {
    char special_description[BUFSZ];
    enum special_level_naming_types special_naming_reveal_type;
    short special_naming_seen_monster_type;
    xchar special_naming_seen_monster_class;

    xchar nmgeninfos;
    struct monster_generation_info mon_gen_infos[MAX_MON_GEN_INFOS];

    schar forest_type;
    schar boundary_type; /* floor style to replace boundary tiles */

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
    Bitfield(desert, 1);       /* Desert map, furniture may have desert ground as floor */
    Bitfield(wizard_bones, 1); /* set if level came from a bones file
                                  which was created in wizard mode (or
                                  normal mode descendant of such) */
    Bitfield(corrmaze, 1);     /* Whether corridors are used for the maze
                                  rather than ROOM */
    Bitfield(mapping_does_not_reveal_special, 1); /* Magic mapping does not reveal the special nature of the level */
    Bitfield(no_special_level_naming_checks, 1);
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
#define o_at(x, y) (OBJ_AT(x, y) ? level.objects[x][y] : (struct obj *) 0)
 
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

 /* From pray.c */
#include "align.h"
#define a_align(x, y) ((aligntyp) Amask2align(levl[x][y].altarmask & AM_MASK))
#define ugod_is_angry() (u.ualign.record < 0)
#define on_altar() IS_ALTAR(levl[u.ux][u.uy].typ)
#define on_shrine() ((levl[u.ux][u.uy].altarmask & AM_SHRINE) != 0)


#endif /* RM_H */

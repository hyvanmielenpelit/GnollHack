/* GnollHack 4.0 rm.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

NEARDATA struct location_type_definition location_type_definitions[MAX_TYPE] = {
    {"STONE",           S_stone,    CORR,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"VWALL",           S_vwall,    ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"HWALL",           S_hwall,    ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"TLCORNER",        S_tlcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"TRCORNER",        S_trcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"BLCORNER",        S_blcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"BRCORNER",        S_blcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"CROSSWALL",       S_crwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"TUWALL",          S_tuwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"TDWALL",          S_tdwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"TLWALL",          S_tlwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"TRWALL",          S_trwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"DBWALL",          S_vcdbridge,ROOM,   MAT_WOOD,    LOCATION_SOUNDSET_NONE},
    {"TREE",            S_tree,     GRASS,  MAT_WOOD,    LOCATION_SOUNDSET_NONE},
    {"SDOOR",           S_vcdoor,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"SCORR",           S_corr,     CORR,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"POOL",            S_pool,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_NONE},
    {"MOAT",            S_pool,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_NONE},
    {"WATER",           S_pool,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_NONE},
    {"DRAWBRIDGE_UP",   S_vcdbridge,GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_DOOR},
    {"LAVAPOOL",        S_lava,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_NONE},
    {"IRON_BARS",       S_bars,     ROOM,   MAT_IRON,    LOCATION_SOUNDSET_NONE},
    {"DOOR",            S_vcdoor,   ROOM,   MAT_WOOD,    LOCATION_SOUNDSET_DOOR},
    {"CORR",            S_litcorr,  0,      MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"ROOM",            S_room,     0,      MAT_MINERAL, LOCATION_SOUNDSET_NONE },
    {"STAIRS",          S_dnstair,  ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"LADDER",          S_dnladder, ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"FOUNTAIN",        S_fountain, ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_FOUNTAIN},
    {"THRONE",          S_throne,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"SINK",            S_sink,     ROOM,   MAT_METAL,   LOCATION_SOUNDSET_NONE },
    {"GRAVE",           S_grave,    GROUND, MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"ANVIL",           S_anvil,    ROOM,   MAT_METAL,   LOCATION_SOUNDSET_NONE},
    {"ALTAR",           S_altar,    ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_ALTAR},
    {"ICE",             S_ice,      GROUND, MAT_ICE,     LOCATION_SOUNDSET_NONE},
    {"DRAWBRIDGE_DOWN", S_vodbridge,GROUND, MAT_WOOD,    LOCATION_SOUNDSET_NONE},
    {"AIR",             S_air,      0,      MAT_AIR,     LOCATION_SOUNDSET_NONE},
    {"CLOUD",           S_cloud,    AIR,    MAT_AIR,     LOCATION_SOUNDSET_NONE},
    {"GRASS",           S_grass,    0,      MAT_VEGGY,   LOCATION_SOUNDSET_NONE},
    {"GROUND",          S_ground,   0,      MAT_SOIL,    LOCATION_SOUNDSET_NONE},
    {"UNEXPLORED",      S_unexplored,0,     MAT_NONE,    LOCATION_SOUNDSET_NONE}
};

/* force linkage */
void
init_rm()
{
    return;
}

enum obj_material_types
get_location_type_material(typ, subtyp)
int typ, subtyp;
{
    if (subtyp <= 0)
        return typ >= 0 && typ < MAX_TYPE ? location_type_definitions[typ].material : MAT_NONE;

    enum obj_material_types material = location_type_definitions[typ].material;

    if (typ == ROOM)
    {
        enum floor_subtypes subtype = subtyp;
        switch (subtype)
        {
        case FLOOR_SUBTYPE_PARQUET:
            material = MAT_WOOD;
            break;
        case FLOOR_SUBTYPE_NORMAL:
        case FLOOR_SUBTYPE_NORMAL_VARIATION_1:
        case FLOOR_SUBTYPE_NORMAL_VARIATION_2:
        case FLOOR_SUBTYPE_NORMAL_VARIATION_3:
        case FLOOR_SUBTYPE_MARBLE:
            /* Base material type */
            break;
        default:
            break;
        }
    }

    return material;
}

/* rm.c */
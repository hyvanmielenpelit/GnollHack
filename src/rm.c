/* GnollHack 4.0 rm.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

NEARDATA struct location_type_definition level_location_types[MAX_TYPE] = {
    {"STONE", S_stone, CORR, LOCATION_SOUNDSET_NONE},
    {"VWALL", S_vwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"HWALL", S_hwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"TLCORNER", S_tlcorn, ROOM, LOCATION_SOUNDSET_NONE},
    {"TRCORNER", S_trcorn, ROOM, LOCATION_SOUNDSET_NONE},
    {"BLCORNER", S_blcorn, ROOM, LOCATION_SOUNDSET_NONE},
    {"BRCORNER", S_blcorn, ROOM, LOCATION_SOUNDSET_NONE},
    {"CROSSWALL", S_crwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"TUWALL", S_tuwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"TDWALL", S_tdwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"TLWALL", S_tlwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"TRWALL", S_trwall, ROOM, LOCATION_SOUNDSET_NONE},
    {"DBWALL", S_vcdbridge, ROOM, LOCATION_SOUNDSET_NONE},
    {"TREE", S_tree, GRASS, LOCATION_SOUNDSET_NONE},
    {"SDOOR", S_vcdoor, ROOM, LOCATION_SOUNDSET_NONE},
    {"SCORR", S_corr, CORR, LOCATION_SOUNDSET_NONE},
    {"POOL", S_pool, GROUND, LOCATION_SOUNDSET_NONE},
    {"MOAT", S_pool, GROUND, LOCATION_SOUNDSET_NONE},
    {"WATER", S_pool, GROUND, LOCATION_SOUNDSET_NONE},
    {"DRAWBRIDGE_UP", S_vcdbridge, GROUND, LOCATION_SOUNDSET_NONE},
    {"LAVAPOOL", S_lava, GROUND, LOCATION_SOUNDSET_NONE},
    {"IRON_BARS", S_bars, ROOM, LOCATION_SOUNDSET_NONE},
    {"DOOR", S_vcdoor, ROOM, LOCATION_SOUNDSET_NONE},
    {"CORR", S_litcorr, 0, LOCATION_SOUNDSET_NONE},
    {"ROOM", S_room, 0, LOCATION_SOUNDSET_NONE },
    {"STAIRS", S_dnstair, ROOM, LOCATION_SOUNDSET_NONE},
    {"LADDER", S_dnladder, ROOM, LOCATION_SOUNDSET_NONE},
    {"FOUNTAIN", S_fountain, ROOM, LOCATION_SOUNDSET_FOUNTAIN},
    {"THRONE", S_throne, ROOM, LOCATION_SOUNDSET_NONE},
    {"SINK", S_sink, ROOM, LOCATION_SOUNDSET_NONE },
    {"GRAVE", S_grave, GROUND, LOCATION_SOUNDSET_NONE},
    {"ALTAR", S_altar, ROOM, LOCATION_SOUNDSET_ALTAR},
    {"ICE", S_ice, POOL, LOCATION_SOUNDSET_NONE},
    {"DRAWBRIDGE_DOWN", S_vodbridge, MOAT, LOCATION_SOUNDSET_NONE},
    {"AIR", S_air, 0, LOCATION_SOUNDSET_NONE},
    {"CLOUD", S_cloud, AIR, LOCATION_SOUNDSET_NONE},
    {"GRASS", S_grass, 0, LOCATION_SOUNDSET_NONE},
    {"GROUND", S_ground, 0, LOCATION_SOUNDSET_NONE},
    {"UNEXPLORED", S_unexplored, 0, LOCATION_SOUNDSET_NONE}
};

/* force linkage */
void
init_rm()
{
    return;
}

/* rm.c */
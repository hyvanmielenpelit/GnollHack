/* GnollHack 4.0 rm.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

NEARDATA struct location_type_definition level_location_types[MAX_TYPE] = {
    {"STONE", S_stone, LOCATION_SOUNDSET_NONE},
    {"VWALL", S_vwall, LOCATION_SOUNDSET_NONE},
    {"HWALL", S_hwall, LOCATION_SOUNDSET_NONE},
    {"TLCORNER", S_tlcorn, LOCATION_SOUNDSET_NONE},
    {"TRCORNER", S_trcorn, LOCATION_SOUNDSET_NONE},
    {"BLCORNER", S_blcorn, LOCATION_SOUNDSET_NONE},
    {"BRCORNER", S_blcorn, LOCATION_SOUNDSET_NONE},
    {"CROSSWALL", S_crwall, LOCATION_SOUNDSET_NONE},
    {"TUWALL", S_tuwall, LOCATION_SOUNDSET_NONE},
    {"TDWALL", S_tdwall, LOCATION_SOUNDSET_NONE},
    {"TLWALL", S_tlwall, LOCATION_SOUNDSET_NONE},
    {"TRWALL", S_trwall, LOCATION_SOUNDSET_NONE},
    {"DBWALL", S_vcdbridge, LOCATION_SOUNDSET_NONE},
    {"TREE", S_tree, LOCATION_SOUNDSET_NONE},
    {"SDOOR", S_vcdoor, LOCATION_SOUNDSET_NONE},
    {"SCORR", S_corr, LOCATION_SOUNDSET_NONE},
    {"POOL", S_pool, LOCATION_SOUNDSET_NONE},
    {"MOAT", S_pool, LOCATION_SOUNDSET_NONE},
    {"WATER", S_pool, LOCATION_SOUNDSET_NONE},
    {"DRAWBRIDGE_UP", S_vcdbridge, LOCATION_SOUNDSET_NONE},
    {"LAVAPOOL", S_lava, LOCATION_SOUNDSET_NONE},
    {"IRON_BARS", S_bars, LOCATION_SOUNDSET_NONE},
    {"DOOR", S_vcdoor, LOCATION_SOUNDSET_NONE},
    {"CORR", S_litcorr, LOCATION_SOUNDSET_NONE},
    {"ROOM", S_room, LOCATION_SOUNDSET_NONE },
    {"STAIRS", S_dnstair, LOCATION_SOUNDSET_NONE},
    {"LADDER", S_dnladder, LOCATION_SOUNDSET_NONE},
    {"FOUNTAIN", S_fountain, LOCATION_SOUNDSET_FOUNTAIN},
    {"THRONE", S_throne, LOCATION_SOUNDSET_NONE},
    {"SINK", S_sink, LOCATION_SOUNDSET_NONE },
    {"GRAVE", S_grave, LOCATION_SOUNDSET_NONE},
    {"ALTAR", LOCATION_SOUNDSET_ALTAR},
    {"ICE", S_ice, LOCATION_SOUNDSET_NONE},
    {"DRAWBRIDGE_DOWN", S_vodbridge, LOCATION_SOUNDSET_NONE},
    {"AIR", S_air, LOCATION_SOUNDSET_NONE},
    {"CLOUD", S_cloud, LOCATION_SOUNDSET_NONE},
    {"GRASS", S_grass, LOCATION_SOUNDSET_NONE},
    {"GROUND", S_ground, LOCATION_SOUNDSET_NONE},
    {"UNEXPLORED", S_unexplored, LOCATION_SOUNDSET_NONE}
};

/* force linkage */
void
init_rm()
{
    return;
}

/* rm.c */
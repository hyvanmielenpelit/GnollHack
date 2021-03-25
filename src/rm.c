/* GnollHack 4.0 rm.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

NEARDATA struct location_type_definition location_type_definitions[MAX_TYPE] = {
    {"STONE",           S_stone,    CORR,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"VWALL",           S_vwall,    ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"HWALL",           S_hwall,    ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"TLCORNER",        S_tlcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"TRCORNER",        S_trcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"BLCORNER",        S_blcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"BRCORNER",        S_blcorn,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"CROSSWALL",       S_crwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"TUWALL",          S_tuwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"TDWALL",          S_tdwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"TLWALL",          S_tlwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"TRWALL",          S_trwall,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"DBWALL",          S_vcdbridge,ROOM,   MAT_WOOD,    LOCATION_SOUNDSET_STONE},
    {"TREE",            S_tree,     GRASS,  MAT_WOOD,    LOCATION_SOUNDSET_TREE},
    {"SDOOR",           S_vcdoor,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"SCORR",           S_corr,     CORR,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"POOL",            S_pool,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_WATER},
    {"MOAT",            S_pool,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_WATER},
    {"WATER",           S_pool,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_WATER},
    {"DRAWBRIDGE_UP",   S_vcdbridge,GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_WATER},
    {"LAVAPOOL",        S_lava,     GROUND, MAT_LIQUID,  LOCATION_SOUNDSET_LAVA},
    {"IRON_BARS",       S_bars,     ROOM,   MAT_IRON,    LOCATION_SOUNDSET_GENERAL},
    {"DOOR",            S_vcdoor,   ROOM,   MAT_WOOD,    LOCATION_SOUNDSET_DOOR},
    {"CORR",            S_litcorr,  0,      MAT_MINERAL, LOCATION_SOUNDSET_NONE},
    {"ROOM",            S_room,     0,      MAT_MINERAL, LOCATION_SOUNDSET_NONE },
    {"STAIRS",          S_dnstair,  ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_STONE},
    {"LADDER",          S_dnladder, ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_GENERAL},
    {"FOUNTAIN",        S_fountain, ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_FOUNTAIN},
    {"THRONE",          S_throne,   ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_GENERAL},
    {"SINK",            S_sink,     ROOM,   MAT_METAL,   LOCATION_SOUNDSET_SINK },
    {"GRAVE",           S_grave,    GROUND, MAT_MINERAL, LOCATION_SOUNDSET_GENERAL},
    {"ANVIL",           S_anvil,    ROOM,   MAT_METAL,   LOCATION_SOUNDSET_GENERAL},
    {"ALTAR",           S_altar,    ROOM,   MAT_MINERAL, LOCATION_SOUNDSET_ALTAR},
    {"ICE",             S_ice,      GROUND, MAT_ICE,     LOCATION_SOUNDSET_ICE},
    {"DRAWBRIDGE_DOWN", S_vodbridge,GROUND, MAT_WOOD,    LOCATION_SOUNDSET_GENERAL},
    {"AIR",             S_air,      0,      MAT_AIR,     LOCATION_SOUNDSET_NONE},
    {"CLOUD",           S_cloud,    0,      MAT_AIR,     LOCATION_SOUNDSET_NONE},
    {"GRASS",           S_grass,    0,      MAT_VEGGY,   LOCATION_SOUNDSET_NONE},
    {"GROUND",          S_ground,   0,      MAT_SOIL,    LOCATION_SOUNDSET_NONE},
    {"UNDEFINED_LOCATION",      S_unexplored,0,     MAT_NONE,    LOCATION_SOUNDSET_NONE}
};


struct category_definition corridor_category_definitions[MAX_CORRIDOR_CATEGORIES] =
{
    { CORRIDOR_SUBTYPE_NORMAL, 4},
};

struct category_definition grass_category_definitions[MAX_GRASS_CATEGORIES] =
{
    { GRASS_SUBTYPE_NORMAL, 3},
    { GRASS_SUBTYPE_SWAMPY, 1},
};

struct category_definition ground_category_definitions[MAX_GROUND_CATEGORIES] =
{
    { GROUND_SUBTYPE_NORMAL, 4},
    { GROUND_SUBTYPE_SWAMPY, 1},
};

struct category_definition floor_category_definitions[MAX_FLOOR_CATEGORIES] =
{
    { FLOOR_SUBTYPE_NORMAL, 4},
    { FLOOR_SUBTYPE_MARBLE, 1},
    { FLOOR_SUBTYPE_PARQUET, 1},
};


struct category_definition stone_category_definitions[MAX_STONE_CATEGORIES] =
{
    { STONE_SUBTYPE_NORMAL, NUM_NORMAL_STONE_SUBTYPES},
};

struct category_definition wall_category_definitions[MAX_WALL_CATEGORIES] =
{
    { WALL_SUBTYPE_STONE, NUM_NORMAL_STONE_SUBTYPES}, /* Must be aligned with stone normal category */
};


struct door_subtype_definition door_subtype_definitions[MAX_DOOR_SUBTYPES] =
{
    {"wooden door",          "door", MAT_WOOD, LOCATION_SOUNDSET_DOOR, DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES | DSTFLAGS_BROKEN_BY_STRONG_MONSTERS | DSTFLAGS_BROKEN_BY_DIGGING | DSTFLAGS_BROKEN_BY_KICKING | DSTFLAGS_BROKEN_BY_STRIKING },
    {"windowed wooden door", "door", MAT_WOOD, LOCATION_SOUNDSET_DOOR, DSTFLAGS_BROKEN_BY_STRONG_MONSTERS | DSTFLAGS_BROKEN_BY_DIGGING | DSTFLAGS_BROKEN_BY_KICKING | DSTFLAGS_BROKEN_BY_STRIKING },
    {"iron door",            "door", MAT_IRON, LOCATION_SOUNDSET_DOOR, DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES},
    {"windowed iron door",   "door", MAT_IRON, LOCATION_SOUNDSET_DOOR, DSTFLAGS_NONE },
    {"iron bar door",        "door", MAT_IRON, LOCATION_SOUNDSET_DOOR, DSTFLAGS_NONE },
    {"magic door",           "door", MAT_MINERAL, LOCATION_SOUNDSET_DOOR, DSTFLAGS_INDESTRUCTIBLE | DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES},
    {"windowed magic door",  "door", MAT_MINERAL, LOCATION_SOUNDSET_DOOR, DSTFLAGS_INDESTRUCTIBLE },
    {"alien door",           "door", MAT_MODRONITE, LOCATION_SOUNDSET_DOOR, DSTFLAGS_NO_LOCK_DESCRIPTION_IS_DEFAULT | DSTFLAGS_INDESTRUCTIBLE | DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES},
    {"reinforced door",      "door", MAT_METAL, LOCATION_SOUNDSET_DOOR, DSTFLAGS_INDESTRUCTIBLE | DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES},
    {"stone door",           "door", MAT_MINERAL, LOCATION_SOUNDSET_DOOR, DSTFLAGS_NO_LOCK_DESCRIPTION_IS_DEFAULT | DSTFLAGS_BROKEN_BY_DIGGING | DSTFLAGS_BROKEN_BY_STRIKING | DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES},
    {"obsidian door",        "door", MAT_MINERAL, LOCATION_SOUNDSET_DOOR, DSTFLAGS_NO_LOCK_DESCRIPTION_IS_DEFAULT | DSTFLAGS_BLOCKS_VISION_AND_SOUND | DSTFLAGS_BLOCKS_PROJECTILES},
    {"black gate",           "gate", MAT_METAL, LOCATION_SOUNDSET_DOOR, DSTFLAGS_BLOCKS_VISION_AND_SOUND},
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

int
get_location_subtype_by_category(ltype, category_id)
int ltype;
int category_id;
{
    struct category_definition* cat_def = 0;
    int cat_size = 0;

    if (ltype == GRASS)
    {
        cat_def = grass_category_definitions;
        cat_size = MAX_GRASS_CATEGORIES;
    }
    else if (ltype == GROUND)
    {
        cat_def = ground_category_definitions;
        cat_size = MAX_GROUND_CATEGORIES;
    }
    else if (ltype == ROOM)
    {
        cat_def = floor_category_definitions;
        cat_size = MAX_FLOOR_CATEGORIES;
    }
    else if (ltype == CORR)
    {
        cat_def = corridor_category_definitions;
        cat_size = MAX_CORRIDOR_CATEGORIES;
    }
    else if (ltype == STONE)
    {
        cat_def = stone_category_definitions;
        cat_size = MAX_STONE_CATEGORIES;
    }
    else if (IS_WALL(ltype))
    {
        cat_def = wall_category_definitions;
        cat_size = MAX_WALL_CATEGORIES;
    }

    if (cat_def == 0)
        return 0;

    if (category_id < 0 || category_id >= cat_size)
        category_id = 0; /* Default to normal if illegal category */

    int first_st = cat_def[category_id].first_subtype;
    int num_st = cat_def[category_id].number_of_subtypes;

    if (num_st <= 0)
        return 0;
    else if (num_st == 1)
        return max(0, first_st);
    else
        return max(0, first_st) + rn2(num_st);

}

int
get_initial_location_subtype(ltype)
int ltype;
{
    if (ltype == FOUNTAIN)
        return FOUNTAIN_MAGIC + rn2(MAX_FOUNTAIN_SUBTYPES - FOUNTAIN_MAGIC);
    else
        return get_location_subtype_by_category(ltype, 
            ltype == GRASS && level.flags.swampy ? GRASS_CATEGORY_SWAMPY : ltype == GROUND && level.flags.swampy ? GROUND_CATEGORY_SWAMPY : 0
    );
}

int
get_location_category(typ, subtyp)
int typ, subtyp;
{
    struct category_definition* cat_def = 0;
    int num_cats = 0;

    if (typ == CORR)
    {
        cat_def = corridor_category_definitions;
        num_cats = MAX_CORRIDOR_CATEGORIES;
    }
    else if (typ == GRASS)
    {
        cat_def = grass_category_definitions;
        num_cats = MAX_GRASS_CATEGORIES;
    }
    else if (typ == GROUND)
    {
        cat_def = ground_category_definitions;
        num_cats = MAX_GROUND_CATEGORIES;
    }
    else if (typ == ROOM)
    {
        cat_def = floor_category_definitions;
        num_cats = MAX_FLOOR_CATEGORIES;
    }
    else if (typ == STONE)
    {
        cat_def = stone_category_definitions;
        num_cats = MAX_STONE_CATEGORIES;
    }
    else if (IS_WALL(typ))
    {
        cat_def = wall_category_definitions;
        num_cats = MAX_WALL_CATEGORIES;
    }

    if (cat_def == 0)
        return 0;

    for (int i = 0; i < num_cats; i++)
    {
        if (subtyp >= cat_def[i].first_subtype && subtyp < cat_def[i].first_subtype + cat_def[i].number_of_subtypes)
        {
            return i;
        }
    }

    return 0;
}

const char*
get_door_name_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return "illegal door location";

    struct rm* door = &levl[x][y];
    return  get_door_name_at_ptr(door);

}

const char*
get_door_name_at_ptr(door)
struct rm* door;
{
    if (!door)
        return "no door pointer";

    if (door->typ == SDOOR)
        return "secret door";
    else if (!IS_DOOR(door->typ))
        return "not a door";

    if (door->subtyp < 0 || door->subtyp >= MAX_DOOR_SUBTYPES)
        return "door";
    else
        return door_subtype_definitions[door->subtyp].description;
}

const char*
get_short_door_name_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return "illegal door location";

    struct rm* door = &levl[x][y];
    return  get_short_door_name_at_ptr(door);

}

const char*
get_short_door_name_at_ptr(door)
struct rm* door;
{
    if (!door)
        return "no door pointer";

    if(door->typ == SDOOR)
        return "secret door";
    else if(!IS_DOOR(door->typ))
        return "not a door";

    if (door->subtyp < 0 || door->subtyp >= MAX_DOOR_SUBTYPES)
        return "door";
    else
        return door_subtype_definitions[door->subtyp].short_description;
}

boolean
m_can_destroy_door(mtmp, door, include_eating)
struct monst* mtmp;
struct rm* door;
boolean include_eating;
{
    if (!mtmp || !door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;
    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE)
        return FALSE;

    if (include_eating && metallivorous(mtmp->data)
        && (door_subtype_definitions[subtyp].material == MAT_METAL || door_subtype_definitions[subtyp].material == MAT_IRON))
        return TRUE;

    if (include_eating && tunnels(mtmp->data) && !needspick(mtmp->data)
        && (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_DIGGING)
        && (door_subtype_definitions[subtyp].material == MAT_MINERAL))
        return TRUE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_STRONG_MONSTERS)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_indestructible_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_indestructible_at_ptr(door);

}


boolean
is_door_indestructible_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >=0  && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;
    if ((door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE) != 0
        || (door->flags & L_INDESTRUCTIBLE) != 0)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_nonpassable_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_nonpassable_at_ptr(door);

}


boolean
is_door_nonpassable_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;
    if ((door_subtype_definitions[subtyp].flags & DSTFLAGS_NONPASSABLE) != 0
        || (door->flags & L_NON_PASSDOOR) != 0)
        return TRUE;
    else
        return FALSE;

}

boolean
has_door_normal_lock_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  has_door_normal_lock_at_ptr(door);

}


boolean
has_door_normal_lock_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    if ((door->key_otyp == STRANGE_OBJECT || door->key_otyp == SKELETON_KEY) && door->special_quality == 0)
        return TRUE;
    else
        return FALSE;

}

boolean
has_box_normal_lock(box)
struct obj* box;
{
    if (!box || !Is_box(box))
        return FALSE;

    if ((box->keyotyp == STRANGE_OBJECT || box->keyotyp == NON_PM || box->keyotyp == SKELETON_KEY) && box->special_quality == 0)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_diggable_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_diggable_at_ptr(door);

}


boolean
is_door_diggable_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE)
        return FALSE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_DIGGING)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_kickable_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_kickable_at_ptr(door);

}


boolean
is_door_kickable_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE)
        return FALSE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_KICKING)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_destroyed_by_booby_trap_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_destroyed_by_booby_trap_at_ptr(door);

}


boolean
is_door_destroyed_by_booby_trap_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE)
        return FALSE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_BEING_BOOBY_TRAPPED)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_destroyed_by_striking_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_destroyed_by_striking_at_ptr(door);

}


boolean
is_door_destroyed_by_striking_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE)
        return FALSE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_STRIKING)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_destroyed_by_monsters_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  is_door_destroyed_by_monsters_at_ptr(door);

}


boolean
is_door_destroyed_by_monsters_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_INDESTRUCTIBLE)
        return FALSE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BROKEN_BY_STRONG_MONSTERS)
        return TRUE;
    else
        return FALSE;

}

boolean
door_blocks_vision_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return FALSE;

    struct rm* door = &levl[x][y];
    return  door_blocks_vision_at_ptr(door);

}


boolean
door_blocks_vision_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return FALSE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;

    /* Secret doors always block vision */
    if (door->typ == SDOOR)
        return TRUE;

    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND)
        return TRUE;
    else
        return FALSE;

}

boolean
is_door_normally_without_lock_at(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return TRUE;

    struct rm* door = &levl[x][y];
    return  is_door_normally_without_lock_at_ptr(door);

}


boolean
is_door_normally_without_lock_at_ptr(door)
struct rm* door;
{
    if (!door || !IS_DOOR_OR_SDOOR(door->typ))
        return TRUE;

    enum door_subtypes_types subtyp = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door->subtyp : 0;
    if (door_subtype_definitions[subtyp].flags & DSTFLAGS_NO_LOCK_DESCRIPTION_IS_DEFAULT)
        return TRUE;
    else
        return FALSE;

}

/* rm.c */
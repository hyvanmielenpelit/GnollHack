/*
 * tiledata.h
 * Copyright 2020 by Janne Gustafsson
*/

#ifndef TILEDATA_H
#define TILEDATA_H

struct tileset_definition {
    uchar female_tile_style;     /* 0 = use base monster, 1 = separate female tile for each monster, 2 = indicated monsters with G_FEMALE_TILE on in mons[].geno */
    boolean has_pet_tiles;       /* 0 = use normal monster variation, 1 = separate pet tile for each monster  */
    boolean has_detect_tiles;    /* 0 = use normal monster variation, 1 = separate detect tile for each monster  */
    uchar body_tile_style;       /* 0 = has one generic corpse tile only (regardless of female style), 1 = separate corpse tile for each monster, 2 = monsters marked with M5_CORPSE_TILE have body tiles */
    boolean ridden_tile_style;   /* 0 = use normal monster variation, 1 = separate ridden tile for each monster, 2 = monsters with M1_STEED have ridden tiles */
    boolean has_statue_tiles;    /* 0 = has one generic statue tile only (regardless of female style), 1 = separate statue tile for each monster  */
   
    uchar attack_tile_style;     /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_ATTACK_TILE have attack tiles */
    uchar throw_tile_style;      /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_THROW_TILE have attack tiles */
    uchar fire_tile_style;       /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_FIRE_TILE have attack tiles */
    uchar cast_tile_style;       /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_CAST_TILE have attack tiles */
    uchar special_attack_tile_style; /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_SPECIAL_ATTACK_TILE have attack tiles */
    uchar kick_tile_style; /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_SPECIAL_ATTACK_TILE have attack tiles */
    uchar item_use_tile_style;     /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_ITEM_USE_TILE have attack tiles */
    uchar door_use_tile_style;     /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_DOOR_USE_TILE have attack tiles */

    uchar inventory_tile_style;  /* 0 = no inventory tiles, 1 = has separate inventory tiles for all objects, 2 = has inventory tiles for each object marked with O4_INVENTORY_TILE  */
    uchar lit_tile_style;        /* 0 = no lit tiles, 1 = has separate lit tiles for all objects, 2 = has lit tiles for each object marked with O4_LIT_TILE  */
    uchar missile_tile_style;    /* 0 = no missile tiles, 1 = has missile inventory tiles for all objects, 2 = has missile tiles for each object marked with O4_MISSILE_TILE  */

    uchar swallow_tile_style;    /*  0 = one set of swallow tiles, 1 = separate set for all monsters, 2 = one set for each monster with swallow attack */
    boolean has_full_cmap_set;   /* 0 = has only number_of_cmaps cmaps, 1 = has CMAP_TYPE_MAX cmaps */
    boolean has_variations;      /* 0 = all variations use the base tile, 1 = separate tile for each variation */

    uchar number_of_cmaps;       /* 0 = 1 = one set ... X = X sets */
    char* cmap_names[CMAP_TYPE_MAX]; /* names of the cmaps of this tileset */
    uchar cmap_mapping[CMAP_TYPE_MAX]; /* mapping from the tilemaps's cmaps to GnollHack's internal cmaps, e.g., 0 means that this tileset's cmap 0 is being used for GnollHack's internal cmap in question */
    uchar cmap_limitation_style[CMAP_TYPE_MAX]; /* (not applicable for cmap 0) 0 = cmap has a full character set, 1 = cmap has only wall tiles, 2 = cmap is a base cmap: it has all tiles between S_stone and S_dnladder */
    uchar cmap_wall_style[CMAP_TYPE_MAX]; /* 0 = cmap has all wall tiles,
                                           * 1 = simple 2-tile wall style: cmap has only horizontal wall (which will also be bottom-end variation for applicable walls) and vertical wall (which will be all other walls)
                                           */
    boolean has_all_explode_tiles; /* 0 = one set of explode tiles, 1 = separate explode tile for each case  */
    boolean has_all_zap_tiles; /* 0 = one set of zap tiles, 1 = separate zap tile for each case  */
    uchar player_tile_style; /* 0 = use base role monster tile,
                              * 1 = one generic icon
                              * 2 = separate player tile for each role/race/gender/alignment/level combination
                              * 3 = separate player tile for each role/race/gender/alignment/level combination for relevant cases only
                              */
};

#define GENERIC_PLAYER_HAS_ATTACK_TILE 1
#define GENERIC_PLAYER_HAS_THROW_TILE 1
#define GENERIC_PLAYER_HAS_FIRE_TILE 1
#define GENERIC_PLAYER_HAS_CAST_TILE 1
#define GENERIC_PLAYER_HAS_SPECIAL_ATTACK_TILE 1
#define GENERIC_PLAYER_HAS_KICK_TILE 1
#define GENERIC_PLAYER_HAS_ITEM_USE_TILE 1
#define GENERIC_PLAYER_HAS_DOOR_USE_TILE 1

extern NEARDATA struct tileset_definition default_tileset_definition;


enum game_cursor_types
{
    GENERIC_CURSOR = 0,
    LOOK_CURSOR,
    TELEPORT_CURSOR,
    JUMP_CURSOR,
    POLEARM_CURSOR,
    SPELL_CURSOR,
    PAY_CURSOR,
    MAX_CURSORS
};


struct ui_component_definition {
    char* name;
    int number;
    uchar width;
    uchar height;
    const char* component_names[24];
};

enum game_ui_tile_types
{
    DEATH_TILE = 0,
    HIT_TILE,
    HIT_TEXT_1,
    HIT_TEXT_2,
    HIT_TEXT_3,
    HIT_TEXT_4,
    HIT_TEXT_5,
    GENERAL_UI_ELEMENTS,    /* Check box etc. */
    STATUS_MARKS,   /* Pet mark, detection mark, ridden mark, pile mark, etc. */
    CONDITION_MARKS,   /* One tile for each specified status (the same as on status line) */
    MAIN_TILE_MARK,
    MAIN_WINDOW_BORDERS,
    MESSAGE_WINDOW_BORDERS,
    STATUS_WINDOW_BORDERS,
    MAP_WINDOW_BORDERS,
    MENU_WINDOW_BORDERS,
    TEXT_WINDOW_BORDERS,
    MAX_UI_TILES
};

extern NEARDATA struct ui_component_definition ui_tile_component_array[MAX_UI_TILES]; /* from tiledata.c */

#endif /* TILEDATA_H */


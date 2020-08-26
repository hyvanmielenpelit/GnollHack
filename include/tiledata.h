/*
 * tiledata.h
 * Copyright (c) Janne Gustafsson, 2020
*/
#ifndef TILEDATA_H
#define TILEDATA_H

struct tileset_definition {
    uchar female_tile_style;     /* 0 = use base monster, 1 = separate female tile for each monster, 2 = indicated monsters with M5_FEMALE_TILE */
   
    uchar action_tile_style[MAX_ACTION_TILES];     /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_XXXX_TILE have attack tiles */
    uchar statue_tile_style;    /* 0 = has one generic statue tile only (regardless of female style), 1 = separate statue tile for each monster, (2 = monsters marked with M5_STATUE_TILE have body tiles)  */
    uchar corpse_tile_style;       /* 0 = has one generic corpse tile only (regardless of female style), 1 = separate corpse tile for each monster, 2 = monsters marked with M5_CORPSE_TILE have body tiles */

    uchar missile_tile_style;    /* 0 = no missile tiles, 1 = has missile inventory tiles for all objects, 2 = has missile tiles for each object marked with O4_MISSILE_TILE  */

    uchar swallow_tile_style;    /*  0 = one set of swallow tiles, 1 = separate set for all monsters, 2 = one set for each monster with swallow attack */

    uchar number_of_cmaps;       /* 0 = 1 = one set ... X = X sets */
    char* cmap_names[CMAP_TYPE_MAX]; /* names of the cmaps of this tileset */
    uchar cmap_mapping[CMAP_TYPE_MAX]; /* mapping from the tilemaps's cmaps to GnollHack's internal cmaps, e.g., 0 means that this tileset's cmap 0 is being used for GnollHack's internal cmap in question */
    uchar cmap_limitation_style[CMAP_TYPE_MAX]; /* (not applicable for cmap 0) 0 = cmap has a full character set, 1 = has tiles marked in defsyms and defsym_variations arrays, base cmap also marked in those arrays (must be lower number than the current cmap), OBSOLETE: 2 = cmap has only wall tiles, base cmap is 0, 3 = cmap is a base cmap: it has all tiles between S_stone and S_dnladder, base cmap is 0 */
    boolean has_all_explode_tiles; /* 0 = one set of explode tiles, 1 = separate explode tile for each case  */
    boolean has_all_zap_tiles; /* 0 = one set of zap tiles, 1 = separate zap tile for each case  */
    uchar player_tile_style; /* 0 = use base role monster tile,
                              * 1 = one generic icon
                              * 2 = separate player tile for each role/race/gender/alignment/level combination
                              * 3 = separate player tile for each role/race/gender/alignment/level combination for relevant cases only
                              */
};

extern NEARDATA struct tileset_definition default_tileset_definition;


struct ui_component_definition {
    char* name;
    enum replacement_types replacement;
    enum animation_types animation;
    enum enlargement_types enlargement;
    int number;
    uchar width;
    uchar height;
    const char* component_names[24];
};

enum game_ui_tile_types
{
    DEATH_TILE = 0,
    HIT_TILE,
    HIT_POISONED,
    HIT_DISINTEGRATED,
    HIT_CRUSHED,
    HIT_STRANGLED,
    HIT_EXTRA_TILE_5,
    HIT_EXTRA_TILE_6,
    HIT_EXTRA_TILE_7,
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
#define MAX_HIT_TILES 8 /* Acually with hit_tile there are 8 */

enum game_ui_status_mark_types
{
    STATUS_MARK_PET = 0,
    STATUS_MARK_PEACEFUL,
    STATUS_MARK_DETECTED,
    STATUS_MARK_PILE,
    STATUS_MARK_HUNGRY,
    STATUS_MARK_WEAK,
    STATUS_MARK_FAINTING,
    STATUS_MARK_BURDENED,
    STATUS_MARK_STRESSED,
    STATUS_MARK_STRAINED,
    STATUS_MARK_OVERTAXED,
    STATUS_MARK_OVERLOADED,
    STATUS_MARK_2WEP,
    STATUS_MARK_SKILL,
    STATUS_MARK_SADDLED,
    STATUS_MARK_LOW_HP,
    STATUS_MARK_CRITICAL_HP,
    STATUS_MARK_SPEC_USED,
    MAX_STATUS_MARKS
};

extern NEARDATA struct ui_component_definition ui_tile_component_array[MAX_UI_TILES]; /* from tiledata.c */

#define GLYPH_TILE_FLAG_FLIP_HORIZONTALLY       0x01
#define GLYPH_TILE_FLAG_FULL_SIZED_ITEM         0x02
#define GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE  0x04


#endif /* TILEDATA_H */


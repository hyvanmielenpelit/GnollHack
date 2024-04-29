/*
 * tiledata.h
 * Copyright (c) Janne Gustafsson, 2020
*/
#ifndef TILEDATA_H
#define TILEDATA_H

#include "rm.h"

#define BUFFS_PER_TILE 24
#define BUFF_WIDTH 16
#define BUFF_HEIGHT 16
#define BUFF_TEXT_WIDTH 6
#define BUFF_TEXT_HEIGHT 8

#ifndef TILE_X
#define TILE_X 64
#endif
#define TILE_Y 96

#ifndef MENU_TILE_X
#define MENU_TILE_X 64
#endif
#define MENU_TILE_Y 48

#define GLYPHLESS_MENU_HEIGHT 20

#define TILE_BK_COLOR_RED 71
#define TILE_BK_COLOR_GREEN 108
#define TILE_BK_COLOR_BLUE 108

struct tileset_definition {
    const char* cmap_names[MAX_CMAP_TYPES]; /* names of the cmaps in GnollHack */
    double you_darkening[MAX_CMAP_TYPES]; /* 0.0 is default */
    double lit_darkening[MAX_CMAP_TYPES]; /* 0.0 is default */
    double nonlit_darkening[MAX_CMAP_TYPES]; /* 0.0 is default */

    uchar female_tile_style;     /* 0 = use base monster, 1 = separate female tile for each monster, 2 = indicated monsters with M5_FEMALE_TILE */
    uchar action_tile_style[MAX_ACTION_TILES];     /* 0 = use normal monster variation, 1 = separate tile for each monster, 2 = monsters with M5_XXXX_TILE have attack tiles */
    uchar statue_tile_style;    /* 0 = has one generic statue tile only (regardless of female style), 1 = separate statue tile for each monster, (2 = monsters marked with M5_STATUE_TILE have body tiles)  */
    uchar corpse_tile_style;       /* 0 = has one generic corpse tile only (regardless of female style), 1 = separate corpse tile for each monster, 2 = monsters marked with M5_CORPSE_TILE have body tiles */
    uchar missile_tile_style;    /* 0 = no missile tiles, 1 = has missile inventory tiles for all objects, 2 = has missile tiles for each object marked with O4_MISSILE_TILE  */
    uchar player_tile_style; /* 0 = use base role monster tile,
                              * 1 = one generic icon
                              * 2 = separate player tile for each role/race/gender/alignment/level combination
                              * 3 = separate player tile for each role/race/gender/alignment/level combination for relevant cases only
                              */
};

extern NEARDATA struct tileset_definition default_tileset_definition;

#define MAX_UI_TILE_COMPONENTS 32
#define MAX_UI_TILE_16_x_16_COMPONENTS 24
#define MAX_UI_TILE_8_x_24_COMPONENTS 32

struct ui_component_definition {
    const char* name;
    short /*enum replacement_types*/ replacement;
    short /*enum animation_types*/ animation;
    short /*enum enlargement_types*/ enlargement;
    int number;
    uchar width;
    uchar height;
    const char* component_names[MAX_UI_TILE_COMPONENTS];
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

struct game_cursor_definition {
    const char* name;
    short /*enum replacement_types*/ replacement;
    short /*enum animation_types*/ animation;
    short /*enum enlargement_types*/ enlargement;
};

extern NEARDATA const struct game_cursor_definition game_cursors[MAX_CURSORS];


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

struct hit_tile_definition {
    const char* name;
    short /*enum replacement_types*/ replacement;
    short /*enum animation_types*/ animation;
    short /*enum enlargement_types*/ enlargement;
};

extern NEARDATA const struct hit_tile_definition hit_tile_definitions[MAX_HIT_TILES];


struct command_tile_definition {
    const char* name;
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

struct general_tile_definition {
    const char* name;
    short /*enum replacement_types*/ replacement;
    short /*enum animation_types*/ animation;
    short /*enum enlargement_types*/ enlargement;
};

extern NEARDATA const struct general_tile_definition general_tile_definitions[MAX_GENERAL_TILES];

enum game_ui_tile_types
{
    GENERAL_UI_ELEMENTS = 0,        /* Check box etc. */
    STATUS_MARKS,               /* Pet mark, detection mark, ridden mark, pile mark, etc. */
    CONDITION_MARKS,            /* One tile for each specified condition (the same as on status line) */
    MAIN_TILE_MARK,
    U_TILE_MARK,
    ITEM_AUTODRAW_GRAPHICS,
    ITEM2_AUTODRAW_GRAPHICS,
    ITEM_PROPERTY_MARKS,
    JAR_GRAPHICS,
    JAR_FOREGROUND,
    JAR2_GRAPHICS,
    JAR3_GRAPHICS,
    MAX_UI_TILES
};

enum game_command_tile_types
{
    NO_COMMAND_TILE,
    VIEW_SPELLS_COMMAND_TILE,
    MIX_COMMAND_TILE,
    YOU_COMMAND_TILE,
    ATTRIBUTES_COMMAND_TILE,
    SKILLS_COMMAND_TILE,
    RIDE_COMMAND_TILE,
    UNTRAP_COMMAND_TILE,
    CONDUCT_COMMAND_TILE,
    OVERVIEW_COMMAND_TILE,
    CHRONICLE_COMMAND_TILE,
    KILLED_COMMAND_TILE,
    GENOCIDED_COMMAND_TILE,
    DISCOVERED_COMMAND_TILE,
    MONSTER_COMMAND_TILE,
    SORT_SPELLS_COMMAND_TILE,
    REORDER_SPELLS_COMMAND_TILE,
    RESERVED_3,
    RESERVED_4,
    RESERVED_5,
    RESERVED_6,
    RESERVED_7,
    RESERVED_8,
    RESERVED_9,
    MAX_COMMAND_TILES
};

enum game_ui_status_mark_types
{
    STATUS_MARK_PET = 0,
    STATUS_MARK_PEACEFUL,
    STATUS_MARK_DETECTED,
    STATUS_MARK_BOUNTY,
    STATUS_MARK_SATIATED,
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
    STATUS_MARK_TRAPPED,
    STATUS_MARK_USTUCK,
    STATUS_MARK_INVENTORY,
    STATUS_MARK_TOWNGUARD_PEACEFUL,
    STATUS_MARK_TOWNGUARD_HOSTILE,
    MAX_STATUS_MARKS
};

enum item_property_mark_types
{
    ITEM_PROPERTY_MARK_POISONED = 0,
    ITEM_PROPERTY_MARK_DEATH_MAGICAL,
    ITEM_PROPERTY_MARK_FLAMING,
    ITEM_PROPERTY_MARK_FREEZING,
    ITEM_PROPERTY_MARK_ELECTRIFIED,
    ITEM_PROPERTY_MARK_EXCEPTIONAL,
    ITEM_PROPERTY_MARK_ELITE,
    ITEM_PROPERTY_MARK_CELESTIAL,
    ITEM_PROPERTY_MARK_PRIMORDIAL,
    ITEM_PROPERTY_MARK_INFERNAL,
    ITEM_PROPERTY_MARK_MYTHIC,
    ITEM_PROPERTY_MARK_LEGENDARY,
    ITEM_PROPERTY_MARK_CORRODED,
    ITEM_PROPERTY_MARK_ROTTED,
    ITEM_PROPERTY_MARK_BURNT,
    ITEM_PROPERTY_MARK_RUSTY,
    ITEM_PROPERTY_MARK_VERY_CORRODED,
    ITEM_PROPERTY_MARK_VERY_ROTTED,
    ITEM_PROPERTY_MARK_VERY_BURNT,
    ITEM_PROPERTY_MARK_VERY_RUSTY,
    ITEM_PROPERTY_MARK_THOROUGHLY_CORRODED,
    ITEM_PROPERTY_MARK_THOROUGHLY_ROTTED,
    ITEM_PROPERTY_MARK_THOROUGHLY_BURNT,
    ITEM_PROPERTY_MARK_THOROUGHLY_RUSTY,
    MAX_ITEM_PROPERTY_MARKS
};

extern NEARDATA struct ui_component_definition ui_tile_component_array[MAX_UI_TILES]; /* from tiledata.c */

#define GLYPH_TILE_FLAG_FLIP_HORIZONTALLY       0x01
#define GLYPH_TILE_FLAG_FLIP_VERTICALLY         0x02
#define GLYPH_TILE_FLAG_FULL_SIZED_ITEM         0x04
#define GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE  0x08
#define GLYPH_TILE_FLAG_HALF_SIZED_TILE         0x10 /* General flag to check whether the tile uses only lower half */
#define GLYPH_TILE_FLAG_HAS_FLOOR_TILE          0x20
#define GLYPH_TILE_FLAG_TWO_WIDE_CENTERED_TILE  0x40 /* General flag to check whether a tile with 3 tiles wide enlargement uses only middle tile and half of each side tile */
#define GLYPH_TILE_FLAG_HEIGHT_IS_CLIPPING      0x80

#define ZAP_INDEX_WITH_FIRST_TILE 48

#endif /* TILEDATA_H */


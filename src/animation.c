/* GnollHack 4.1.0 animation.c */
/* Copyright (c) Janne Gustafsson, 2020. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "animoff.h"

NEARDATA const struct autodraw_definition autodraws[MAX_AUTODRAWS] =
{
    {"", 0, 0, 0, 0, 0, 0, 0UL, 0UL, 0UL},
    {
        "dungeon-normal-wall-autodraw-all",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-up-down",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-up-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-up-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-up-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-down-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-down-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-down-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-down-up-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_UP,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-normal-wall-autodraw-down-up-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },

    {
        "gnomish-mines-wall-autodraw-all",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-up-down",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-up-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-up-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-up-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-down-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-down-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-down-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-down-up-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_UP,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gnomish-mines-wall-autodraw-down-up-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GNOMISH_MINES * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GNOMISH_MINES * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },

    {
        "gehennom-wall-autodraw-all",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-up-down",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-up-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-up-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-up-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-down-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-down-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-down-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-down-up-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_UP,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-wall-autodraw-down-up-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },

    {
        "dungeon-undead-wall-autodraw-all",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-up-down",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-up-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-up-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-up-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-down-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-down-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-down-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-down-up-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_UP,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "dungeon-undead-wall-autodraw-down-up-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_UNDEAD * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_UNDEAD * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },

    {
        "gehennom-cavernous-wall-autodraw-all",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-up-down",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-up-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-up-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-up-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-down-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-down-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-down-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-down-up-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_UP,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },
    {
        "gehennom-cavernous-wall-autodraw-down-up-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + CMAP_GEHENNOM_CAVERNOUS * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        S_hwall + CMAP_GEHENNOM_CAVERNOUS * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF,
        0,
        0UL, 0UL, 0UL
    },


    {
        "bookshelf-autodraw",
        AUTODRAW_DRAW_BOOKSHELF_CONTENTS,
        0,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "weapon-rack-autodraw",
        AUTODRAW_DRAW_WEAPON_RACK_CONTENTS,
        0,
        0,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "candelabrum-autodraw",
        AUTODRAW_DRAW_CANDELABRUM_CANDLES,
        0,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "candelabrum-animation-1-autodraw",
        AUTODRAW_DRAW_CANDELABRUM_CANDLES,
        1,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "large-five-branched-candelabrum-autodraw",
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        0,
        ITEM2_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "large-five-branched-candelabrum-animation-1-autodraw",
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        1,
        ITEM2_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "large-five-branched-candelabrum-animation-2-autodraw",
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        2,
        ITEM2_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "large-five-branched-candelabrum-animation-3-autodraw",
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        3,
        ITEM2_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "large-five-branched-candelabrum-animation-4-autodraw",
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        4,
        ITEM2_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "large-five-branched-candelabrum-animation-5-autodraw",
        AUTODRAW_DRAW_LARGE_FIVE_BRANCHED_CANDELABRUM_CANDLES,
        5,
        ITEM2_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "jar-yellow-autodraw",
        AUTODRAW_DRAW_JAR_CONTENTS,
        0,
        JAR_GRAPHICS + GLYPH_UI_TILE_OFF,
        JAR_FOREGROUND + GLYPH_UI_TILE_OFF,
        0,
        0,
        0xFFFFFF00UL,
        0xFFCCAA00UL,
        1UL
    },
    {
        "jar-purple-autodraw",
        AUTODRAW_DRAW_JAR_CONTENTS,
        0,
        JAR3_GRAPHICS + GLYPH_UI_TILE_OFF,
        JAR_FOREGROUND + GLYPH_UI_TILE_OFF,
        0,
        0,
        0xFF800080UL,
        0xFFCC00AAUL,
        1UL
    },
    {
        "jar-white-autodraw",
        AUTODRAW_DRAW_JAR_CONTENTS,
        0,
        JAR_GRAPHICS + GLYPH_UI_TILE_OFF,
        JAR_FOREGROUND + GLYPH_UI_TILE_OFF,
        0,
        0,
        0xFFFFFFFFUL,
        0xFFDDCC22UL,
        1UL
    },
    {
        "jar-violet-autodraw",
        AUTODRAW_DRAW_JAR_CONTENTS,
        0,
        JAR3_GRAPHICS + GLYPH_UI_TILE_OFF,
        JAR_FOREGROUND + GLYPH_UI_TILE_OFF,
        0,
        0,
        0xFFEE82EEUL,
        0xFF8877EEUL,
        1UL
    },
    {
        "jar-crimson-autodraw",
        AUTODRAW_DRAW_JAR_CONTENTS,
        0,
        JAR2_GRAPHICS + GLYPH_UI_TILE_OFF,
        JAR_FOREGROUND + GLYPH_UI_TILE_OFF,
        0,
        0,
        0xFFDC143CUL,
        0xFFBB1122UL,
        0UL
    },
    {
        "figurine-autodraw",
        AUTODRAW_DRAW_FIGURINE,
        0,
        0,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "chain-autodraw",
        AUTODRAW_DRAW_CHAIN,
        0,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        IRON_CHAIN + GLYPH_OBJ_OFF, /* Backup data for unimplemented autodraw to avoid invisible chain */
        0UL, 0UL, 0UL
    },
    {
        "ball-autodraw",
        AUTODRAW_DRAW_BALL,
        0,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        HEAVY_IRON_BALL + GLYPH_OBJ_OFF, /* Backup data for unimplemented autodraw */
        0UL, 0UL, 0UL
    },
    {
        "chain-autodraw-up",
        AUTODRAW_DRAW_CHAIN,
        1,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "chain-autodraw-right",
        AUTODRAW_DRAW_CHAIN,
        2,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "chain-autodraw-down",
        AUTODRAW_DRAW_CHAIN,
        3,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "chain-autodraw-left",
        AUTODRAW_DRAW_CHAIN,
        4,
        ITEM_AUTODRAW_GRAPHICS + GLYPH_UI_TILE_OFF,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },
    {
        "long-worm-autodraw-head",
        AUTODRAW_DRAW_LONG_WORM,
        0,
        WORM_TILE_WORM_HEAD_COMING_FROM_UP + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_GOING_UP + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_BOTTOM + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_IS_RIGHT_GOING_UP_LEFT + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        0UL, 0UL, 0UL
    },
    {
        "long-worm-autodraw-tail",
        AUTODRAW_DRAW_LONG_WORM,
        1,
        WORM_TILE_WORM_TAIL_GOING_UP + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_GOING_UP + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_BOTTOM + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_IS_RIGHT_GOING_UP_LEFT + LONG_WORM_TYPE_ADULT * MAX_WORM_TILES + GLYPH_WORM_OFF,
        0UL, 0UL, 0UL
    },
    {
        "elder-long-worm-autodraw-head",
        AUTODRAW_DRAW_LONG_WORM,
        0,
        WORM_TILE_WORM_HEAD_COMING_FROM_UP + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_GOING_UP + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_BOTTOM + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_IS_RIGHT_GOING_UP_LEFT + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        0UL, 0UL, 0UL
    },
    {
        "elder-long-worm-autodraw-tail",
        AUTODRAW_DRAW_LONG_WORM,
        1,
        WORM_TILE_WORM_TAIL_GOING_UP + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_GOING_UP + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_SEGMENT_BOTTOM + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        WORM_TILE_WORM_IS_RIGHT_GOING_UP_LEFT + LONG_WORM_TYPE_ELDER * MAX_WORM_TILES + GLYPH_WORM_OFF,
        0UL, 0UL, 0UL
    },
    {
        "coffin-autodraw",
        AUTODRAW_DRAW_COFFIN_CONTENTS,
        0,
        0,
        0,
        0,
        0,
        0UL, 0UL, 0UL
    },

};


/* Special Effects */
NEARDATA const struct special_effect_definition special_effects[MAX_SPECIAL_EFFECTS] =
{
    {"teleport-out",            0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, TELEPORT_OUT_ANIMATION, NO_ENLARGEMENT},
    {"teleport-in",             0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, TELEPORT_IN_ANIMATION, NO_ENLARGEMENT },
    {"level-teleport-out",      0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, LEVEL_TELEPORT_OUT_ANIMATION, NO_ENLARGEMENT},
    {"level-teleport-in",       0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, LEVEL_TELEPORT_IN_ANIMATION, NO_ENLARGEMENT },
    {"generic-spell",           0, 0, 6,        LAYER_MONSTER_EFFECT, NO_REPLACEMENT, GENERIC_SPELL_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"summon-demon",            0, 0, 2,        LAYER_BACKGROUND_EFFECT, NO_REPLACEMENT, SUMMON_DEMON_ANIMATION, NO_ENLARGEMENT },
    {"summon-celestial",        0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, SUMMON_CELESTIAL_ANIMATION, NO_ENLARGEMENT },
    {"summon-nature",           0, 0, 2,        LAYER_BACKGROUND_EFFECT, NO_REPLACEMENT, SUMMON_NATURE_ANIMATION, NO_ENLARGEMENT },
    {"summon-monster",          0, 0, 2,        LAYER_BACKGROUND_EFFECT, NO_REPLACEMENT, SUMMON_DEMON_ANIMATION, NO_ENLARGEMENT },
    {"summon-nasty",            0, 0, 2,        LAYER_BACKGROUND_EFFECT, NO_REPLACEMENT, SUMMON_DEMON_ANIMATION, NO_ENLARGEMENT },
    {"summon-undead",           0, 0, 2,        LAYER_BACKGROUND_EFFECT, NO_REPLACEMENT, SUMMON_DEMON_ANIMATION, NO_ENLARGEMENT },
    {"animate-dead",            0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, SUMMON_DEMON_ANIMATION, NO_ENLARGEMENT },
    {"summon-in-smoke",         0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, PUFF_OF_SMOKE_ANIMATION, NO_ENLARGEMENT },
    {"puff-of-smoke",           0, 0, 2,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, PUFF_OF_SMOKE_ANIMATION, NO_ENLARGEMENT },
    {"disintegrated",           0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT },
    {"sleep-gas-trap-effect",   0, 6, 6,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, SLEEP_GAS_TRAP_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"rust-trap-effect",        0, 0, 3,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, RUST_TRAP_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"fire-trap-effect",        0, 0, 3,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, FIRE_TRAP_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"magic-trap-effect",       0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, MAGIC_TRAP_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"magic-trap-light-flash",  0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, MAGIC_TRAP_LIGHT_FLASH_ANIMATION, NO_ENLARGEMENT },
    {"antimagic-trap-effect",   0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, ANTIMAGIC_TRAP_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"polymorph-trap-effect",   0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, POLYMORPH_TRAP_EFFECT_ANIMATION, NO_ENLARGEMENT },
    {"falling-rock-trap-trap-door",    0, 4, 9, LAYER_GENERAL_EFFECT, NO_REPLACEMENT, FALLING_ROCK_TRAP_TRAP_DOOR_ANIMATION, NO_ENLARGEMENT },
    {"falling-rock-trap-falling-rock", 0, 0, 9, LAYER_MONSTER_EFFECT, NO_REPLACEMENT, FALLING_ROCK_TRAP_FALLING_ROCK_ANIMATION, NO_ENLARGEMENT },
    {"land-mine-explosion",     0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, LAND_MINE_EXPLOSION_ANIMATION, NO_ENLARGEMENT },
    {"bear-trap-close",         0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, BEAR_TRAP_CLOSE_ANIMATION, NO_ENLARGEMENT },
    {"bear-trap-open",          0, 0, 4,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, BEAR_TRAP_OPEN_ANIMATION, NO_ENLARGEMENT },
    {"needle",                  0, 0, 12,       LAYER_GENERAL_EFFECT, NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT },
    {"general-exclamation",     0, 0, 12,       LAYER_GENERAL_EFFECT, NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT },
    {"small-fiery-explosion",   0, 0, 8,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, SMALL_FIERY_EXPLOSION_ANIMATION, NO_ENLARGEMENT },
    {"yellow-light-flash",      0, 0, 3,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, YELLOW_LIGHT_FLASH_ANIMATION, NO_ENLARGEMENT },
    {"black-light-flash",       0, 0, 3,        LAYER_GENERAL_EFFECT, NO_REPLACEMENT, BLACK_LIGHT_FLASH_ANIMATION, NO_ENLARGEMENT },
};

STATIC_DCL int FDECL(get_shore_tile_index, (struct rm*, struct rm*));
STATIC_DCL int FDECL(get_solid_floor_tile_index, (struct rm*));
STATIC_DCL int FDECL(get_shore_and_floor_adjusted_tile_index, (struct rm*, struct rm*, struct rm*, struct rm*));

int
get_player_action_glyph_offset(action)
enum action_tile_types action;
{
    /* relies on aligned ordering of glyph offsets and action_tile_types */
    int res = GLYPH_PLAYER_OFF + action * NUM_PLAYER_CHARACTERS;
    return res;
}

int
get_monster_action_glyph_offset(action, genderidx)
enum action_tile_types action;
int genderidx;
{
    /* relies on aligned ordering of glyph offsets and action_tile_types */
    int res = (genderidx == 0 ? GLYPH_MON_OFF : GLYPH_FEMALE_MON_OFF) + action * NUM_MONSTERS;
    return res;
}

short
get_player_replacement(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right replacement for the combination that has a replacement */
    /* To remove gcc warning */
    if (roleidx == 0 && raceidx == 0 && genderidx == 0 && alignmentidx == 0 && levelidx == 0)
    {
        /* Do nothing */
    }

    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        break;
    case ACTION_TILE_CAST_NODIR:
        break;
    case ACTION_TILE_CAST_DIR:
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        break;
    case ACTION_TILE_KICK:
        break;
    case ACTION_TILE_PASSIVE_DEFENSE:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_2:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_3:
        break;
    case ACTION_TILE_ITEM_USE:
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }
    return 0;
}

short
get_player_animation(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    /* To remove gcc warning */
    if (roleidx == 0 && raceidx == 0 && genderidx == 0 && alignmentidx == 0 && levelidx == 0)
    {
        /* Do nothing */
    }

    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_MALE)
            return ELF_WIZARD_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return ELF_WIZARD_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_WIZARD_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_WIZARD_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_KNIGHT_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_KNIGHT_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_KNIGHT_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return DWARF_KNIGHT_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_HEALER_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_HEALER_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_MONK_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_MONK_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_SAMURAI && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_SAMURAI_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_SAMURAI && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_SAMURAI_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_ROGUE_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_ROGUE_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_ROGUE_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_ROGUE_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_ROGUE_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_ROGUE_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_BARBARIAN_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_BARBARIAN_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_TOURIST_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_TOURIST_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_BARBARIAN_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_BARBARIAN_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_BARBARIAN_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_BARBARIAN_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_VALKYRIE_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_VALKYRIE_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_VALKYRIE_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_HEALER_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_HEALER_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_MALE_LAWFUL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_FEMALE_LAWFUL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_MALE_NEUTRAL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_FEMALE_NEUTRAL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_MALE_CHAOTIC_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_FEMALE_CHAOTIC_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_MALE_LAWFUL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_FEMALE_LAWFUL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_MALE_NEUTRAL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_FEMALE_NEUTRAL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_MALE_CHAOTIC_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_FEMALE_CHAOTIC_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_MALE_LAWFUL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_FEMALE_LAWFUL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_MALE_NEUTRAL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_FEMALE_NEUTRAL_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_MALE_CHAOTIC_ATTACK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_FEMALE_CHAOTIC_ATTACK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_WIZARD_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_WIZARD_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return DWARF_CAVEMAN_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_CAVEMAN_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_CAVEMAN_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_CAVEMAN_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_CAVEMAN_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_CAVEMAN_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return DWARF_ARCHAEOLOGIST_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_ARCHAEOLOGIST_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_ARCHAEOLOGIST_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_ARCHAEOLOGIST_FEMALE_ATTACK_ANIMATION;
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF && genderidx == GENDER_MALE)
            return ELF_RANGER_MALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return ELF_RANGER_FEMALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_RANGER_MALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_RANGER_FEMALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_RANGER_MALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_RANGER_FEMALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_RANGER_MALE_FIRE_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_RANGER_FEMALE_FIRE_ANIMATION;
        break;
    case ACTION_TILE_CAST_NODIR:
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_MALE_LAWFUL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_FEMALE_LAWFUL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_MALE_NEUTRAL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_FEMALE_NEUTRAL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_MALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_FEMALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_MALE_LAWFUL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_FEMALE_LAWFUL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_MALE_NEUTRAL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_FEMALE_NEUTRAL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_FEMALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_MALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_FEMALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_MALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_FEMALE_CHAOTIC_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_MALE_NEUTRAL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_FEMALE_NEUTRAL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_MALE_LAWFUL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_FEMALE_LAWFUL_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return HUMAN_KNIGHT_FEMALE_CAST_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_WIZARD_MALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_WIZARD_FEMALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_MALE)
            return ELF_WIZARD_MALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return ELF_WIZARD_FEMALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_WIZARD_MALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_WIZARD_FEMALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_HEALER_FEMALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_HEALER_MALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_HEALER_FEMALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_HEALER_MALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_MONK_MALE_CAST_INDIR_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_MONK_FEMALE_CAST_INDIR_ANIMATION;
        break;
    case ACTION_TILE_CAST_DIR:
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_MALE_LAWFUL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_FEMALE_LAWFUL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_MALE_NEUTRAL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_FEMALE_NEUTRAL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_MALE_CHAOTIC_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_FEMALE_CHAOTIC_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_MALE_LAWFUL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_FEMALE_LAWFUL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_MALE_NEUTRAL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_FEMALE_NEUTRAL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_MALE_CHAOTIC_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_FEMALE_CHAOTIC_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_MALE_CHAOTIC_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_FEMALE_CHAOTIC_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_MALE_NEUTRAL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_FEMALE_NEUTRAL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_MALE_LAWFUL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_FEMALE_LAWFUL_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_WIZARD_MALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_WIZARD_FEMALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_MALE)
            return ELF_WIZARD_MALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return ELF_WIZARD_FEMALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_WIZARD_MALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_WIZARD_FEMALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_HEALER_FEMALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_HEALER_MALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_HEALER_MALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_HEALER_FEMALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_MONK_MALE_CAST_DIR_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_MONK_FEMALE_CAST_DIR_ANIMATION;
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_TOURIST_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_TOURIST_FEMALE_ATTACK_ANIMATION;
        break;
    case ACTION_TILE_KICK:
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_MALE)
            return ELF_WIZARD_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return ELF_WIZARD_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_WIZARD_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_WIZARD_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_KNIGHT_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_KNIGHT_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_KNIGHT_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return DWARF_KNIGHT_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_HEALER_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_HEALER_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_MONK_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_MONK_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_SAMURAI && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_SAMURAI_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_SAMURAI && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_SAMURAI_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_MALE_LAWFUL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return HUMAN_PRIEST_FEMALE_LAWFUL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_MALE_NEUTRAL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return HUMAN_PRIEST_FEMALE_NEUTRAL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_MALE_CHAOTIC_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return HUMAN_PRIEST_FEMALE_CHAOTIC_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_MALE_LAWFUL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return GNOLL_PRIEST_FEMALE_LAWFUL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_MALE_NEUTRAL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return GNOLL_PRIEST_FEMALE_NEUTRAL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_MALE_CHAOTIC_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return GNOLL_PRIEST_FEMALE_CHAOTIC_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_MALE_LAWFUL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_LAWFUL)
            return ELF_PRIEST_FEMALE_LAWFUL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_MALE_NEUTRAL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_NEUTRAL)
            return ELF_PRIEST_FEMALE_NEUTRAL_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_MALE_CHAOTIC_KICK_ANIMATION;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_FEMALE && alignmentidx == A_CHAOTIC)
            return ELF_PRIEST_FEMALE_CHAOTIC_KICK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_ROGUE_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_ROGUE_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_ROGUE_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_ROGUE_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_ROGUE_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_ROGUE_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_BARBARIAN_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_BARBARIAN_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_TOURIST_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_TOURIST_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_BARBARIAN_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_BARBARIAN_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_BARBARIAN_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_BARBARIAN_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_VALKYRIE_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_VALKYRIE_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_VALKYRIE_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_HEALER_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_HEALER_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF && genderidx == GENDER_MALE)
            return ELF_RANGER_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return ELF_RANGER_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_RANGER_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_RANGER_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return ORC_RANGER_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return ORC_RANGER_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_RANGER_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_RANGER && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_RANGER_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_WIZARD_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_WIZARD_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return DWARF_CAVEMAN_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_CAVEMAN_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_CAVEMAN_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_CAVEMAN_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return GNOLL_CAVEMAN_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_GNOLL && genderidx == GENDER_FEMALE)
            return GNOLL_CAVEMAN_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return DWARF_ARCHAEOLOGIST_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return DWARF_ARCHAEOLOGIST_FEMALE_KICK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_ARCHAEOLOGIST_MALE_KICK_ANIMATION;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_ARCHAEOLOGIST_FEMALE_KICK_ANIMATION;
        break;
    case ACTION_TILE_PASSIVE_DEFENSE:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_2:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_3:
        break;
    case ACTION_TILE_ITEM_USE:
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_TOURIST_MALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_TOURIST_FEMALE_ATTACK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return HUMAN_HEALER_FEMALE_POTION_DRINK_ANIMATION;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return HUMAN_HEALER_MALE_POTION_DRINK_ANIMATION;
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }
    return 0;
}

struct replacement_info
data_to_replacement_info(signed_glyph, layer, otmp, mtmp, layer_flags, monster_flags, missile_flags, missile_material, missile_special_quality)
int signed_glyph, layer;
struct obj* otmp;
struct monst* mtmp;
uint64_t layer_flags, monster_flags, missile_flags;
uchar missile_material;
short missile_special_quality;
{
    struct replacement_info info = { 0 };
    info.signed_glyph = signed_glyph;
    info.layer = layer;
    info.object = otmp;
    info.monster = mtmp;
    info.layer_flags = layer_flags;
    info.monster_flags = monster_flags;
    info.missile_flags = missile_flags;
    info.missile_material = missile_material;
    info.missile_special_quality = missile_special_quality;

    return info;
}

int
maybe_get_replaced_glyph(glyph, x, y, info)
#if defined(USE_TILES)
int glyph;
int x, y;
struct replacement_info info;
{
    int sign = sgn(glyph);
    int absglyph = abs(glyph);
    struct obj* otmp = info.object;
    struct monst* mtmp = info.monster;
    uint64_t layer_flags = info.layer_flags;
    uint64_t monster_flags = info.monster_flags;
    uchar missile_material = info.missile_material;
    short missile_special_quality = info.missile_special_quality;
    boolean is_lit_missile = info.layer == LAYER_MISSILE && (info.missile_flags & MISSILE_FLAGS_LIT) != 0;
    short replacement_idx = glyph2replacement[absglyph];
    if (replacement_idx > 0)
    {
        if (replacements[replacement_idx].number_of_tiles < 1)
            return glyph;

        boolean is_missile_with_dirs = glyph_is_missile(glyph) && replacements[replacement_idx].missile_dir >= 0;
        int glyph_missile_dir = glyph_missile_direction(glyph);

        switch (replacements[replacement_idx].replacement_action)
        {
        case REPLACEMENT_ACTION_BOTTOM_TILE:
        {
            int below_y = y + 1;
            if (!isok(x, below_y)
                || (!couldsee(x, below_y) && glyph_is_specific_cmap_or_its_variation(levl[x][below_y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored))
                || (IS_DOORJOIN(levl[x][below_y].typ) && !IS_TREE(levl[x][below_y].typ))
                || levl[x][below_y].typ == DOOR
                || levl[x][below_y].typ == UNDEFINED_LOCATION
                || (!couldsee(x, y) && glyph_is_specific_cmap_or_its_variation(levl[x][below_y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_stone))
                || (levl[x][y].seenv & (SV3 | SV4 | SV5 | SV6 | SV7)) == 0)
            {
                /* No action */
            }
            else
            {
                /* Return the tile based on stone's vartyp (i.e., take the right variation) */
                int repl_idx = (int)min(replacements[replacement_idx].number_of_tiles - 1, levl[x][y].vartyp);
                return sign * (repl_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_SHORE_TILE:
        {
            int above_y = y - 1;
            //int below_y = y + 1;
            int floortype = IS_FLOOR(levl[x][y].typ) || IS_POOL(levl[x][y].typ) || levl[x][y].typ == LAVAPOOL || levl[x][y].typ == ICE ? levl[x][y].typ : levl[x][y].floortyp;
            boolean is_water_or_air_level = (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz));

            if (!(is_water_or_air_level && info.layer == LAYER_FLOOR) && (Underwater || !isok(x, above_y)
                || (levl[x][y].typ == levl[x][above_y].typ && levl[x][y].subtyp == levl[x][above_y].subtyp)
                || (level.flags.hero_memory && glyph_is_specific_cmap_or_its_variation(levl[x][above_y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored))
                || levl[x][above_y].typ == UNDEFINED_LOCATION || (IS_SOLID_FLOOR(floortype) && (IS_DOORJOIN(levl[x][above_y].typ)))))
            {
                /* No action */
            }
            else
            {
                int tileidx = -1;
                /* Kludge for water and air levels */
                if (is_water_or_air_level && info.layer == LAYER_FLOOR)
                {
                    struct layer_info this_l = layers_at(x, y);
                    struct layer_info above_l = layers_at(x, above_y);
                    int this_g = this_l.layer_glyphs[info.layer];
                    int above_g = above_l.layer_glyphs[info.layer];
                    int this_cmap = generic_glyph_to_cmap(this_g);
                    int above_cmap = generic_glyph_to_cmap(above_g);
                    if (this_cmap == above_cmap)
                        return glyph;

                    if (above_cmap == S_water)
                        tileidx = MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 6;
                    else if (above_cmap == S_cloud)
                        tileidx = MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 5;
                    else if (above_cmap == S_air)
                        tileidx = MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 4;
                    else
                        tileidx = get_shore_tile_index(&levl[x][y], &levl[x][above_y]);
                }
                else
                    tileidx = get_shore_tile_index(&levl[x][y], &levl[x][above_y]);

                if (tileidx < 0 || tileidx >= MAX_TILES_PER_REPLACEMENT || (replacements[replacement_idx].tile_flags[tileidx] & RTF_SKIP))
                    return glyph;

                return sign * (tileidx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }

            break;
        }
        case REPLACEMENT_ACTION_SHORE_ADJUSTED_TILE:
        {
            int above_y = y - 1;
            if (Underwater || !isok(x, above_y) || glyph_is_specific_cmap_or_its_variation(levl[x][above_y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored)
                || levl[x][above_y].typ == UNDEFINED_LOCATION)
            {
                /* No action */
            }
            else if (levl[x][above_y].typ == DRAWBRIDGE_DOWN || levl[x][above_y].typ == LAVAPOOL || levl[x][above_y].typ == POOL || levl[x][above_y].typ == MOAT || levl[x][above_y].typ == WATER || levl[x][above_y].typ == DRAWBRIDGE_UP)
            {
                int tileidx = 0;
                return sign * (tileidx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }

            break;
        }
        case REPLACEMENT_ACTION_FLOOR_ADJUSTED_TILE:
        {
            if (Underwater || glyph_is_specific_cmap_or_its_variation(levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored)
                || !IS_SOLID_FLOOR(levl[x][y].floortyp))
            {
                /* No action */
            }
            else
            {
                int tileidx = get_solid_floor_tile_index(&levl[x][y]);
                return sign * (tileidx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_SHORE_AND_FLOOR_ADJUSTED_TILE:
        {
            int above_y = y - 1;
            int left_x = x - 1;
            int right_x = x + 1;
            if (Underwater || glyph_is_specific_cmap_or_its_variation(levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored)
                || !IS_SOLID_FLOOR(levl[x][y].floortyp))
            {
                /* No action */
            }
            else
            {
                int tileidx = get_shore_and_floor_adjusted_tile_index(&levl[x][y], isok(x, above_y) ? &levl[x][above_y] : 0, isok(left_x, y) ? &levl[left_x][y] : 0, isok(right_x, y) ? &levl[right_x][y] : 0);
                return sign * (tileidx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_OBJECT_LIT:
        {
            if (!otmp && !is_lit_missile)
                return glyph;

            if (is_lit_missile || is_obj_activated(otmp))
            {
                /* Return the first tile with index 0 */
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_LOCATION_LIT:
        {
            if (isok(x, y) && get_location_light_range(x, y) != 0 && levl[x][y].lamplit == TRUE)
            {
                /* Return the first tile with index 0 */
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_LOCATION_HORIZONTAL:
        {
            if (isok(x, y) && levl[x][y].horizontal == TRUE)
            {
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_LEVER_STATE:
        {
            struct trap* ttmp = 0;
            uint64_t leverstate = 0UL;
            if (isok(x, y) && (ttmp = t_at(x, y)) != 0 && ttmp->ttyp == LEVER && (leverstate = ttmp->tflags & TRAPFLAGS_STATE_MASK) > 0)
            {
                int glyph_idx = max(0, min((int)replacements[replacement_idx].number_of_tiles, (int)leverstate) - 1);
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_COIN_QUANTITY:
        {
            if (!otmp)
                return glyph;

            if (otmp->quan > 1)
            {
                int glyph_idx = (otmp->quan <= 8 ? (int)otmp->quan - 2 : otmp->quan <= 30 ? 7 : otmp->quan <= 100 ? 8 : otmp->quan <= 1000 ? 9 : 10);
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_PEBBLE_QUANTITY:
        {
            if (!otmp)
                return glyph;

            if (replacements[replacement_idx].number_of_tiles < 1)
                return glyph;

            if (otmp->quan > 1)
            {
                int glyph_idx = (otmp->quan <= 8 ? (int)otmp->quan - 2 : otmp->quan <= 20 ? 7 : otmp->quan <= 50 ? 8 : otmp->quan <= 100 ? 9 : 10);
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_CHEST:
        {
            if (!otmp)
                return glyph;

            if (replacements[replacement_idx].number_of_tiles < 1)
                return glyph;

            if (otmp->olocked)
            {
                int glyph_idx = 0;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            else if (otmp->obroken)
            {
                int glyph_idx = 1;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_ALTERNATIVE_APPEARANCE:
        {
            if (!otmp)
                return glyph;

            if (replacements[replacement_idx].number_of_tiles < 1)
                return glyph;

            if (otmp->speflags & SPEFLAGS_ALTERNATIVE_APPEARANCE)
            {
                /* note: appearanceidx is -1 if not set separately */
                int glyph_idx = otmp->appearanceidx > 0 && otmp->appearanceidx < replacements[replacement_idx].number_of_tiles ? otmp->appearanceidx : 0;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_GLOB:
        {
            if (!otmp)
                return glyph;

            if (replacements[replacement_idx].number_of_tiles < 1 || otmp->owt <= GLOB_SMALL_MAXIMUM_WEIGHT) /* Small */
                return glyph;

            if (otmp->globby)
            {
                /* note: appearanceidx is -1 if not set separately */
                int glyph_idx = (otmp->owt > GLOB_LARGE_MAXIMUM_WEIGHT)
                    ? 2 /* Very large */
                    : (otmp->owt > GLOB_MEDIUM_MAXIMUM_WEIGHT)
                    ? 1 /* Large */
                    : 0; /* Medium */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_AUTODRAW:
        {
            break;
        }
        case REPLACEMENT_ACTION_AUTODRAW_AND_OBJECT_LIT:
        {
            if (!otmp && info.layer != LAYER_MISSILE)
                return glyph;

            if ((info.layer == LAYER_MISSILE && is_lit_missile) || (otmp && is_obj_activated(otmp)))
            {
                /* Return the first tile with index 0 */
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_AUTODRAW_AND_OBJECT_UCHAIN:
        {
            if (!otmp)
                return glyph;

            if (otmp == uchain)
            {
                /* Return the first tile with index 0 */
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_AUTODRAW_AND_OBJECT_UBALL:
        {
            if (!otmp)
                return glyph;

            if (otmp == uball)
            {
                /* Return the first tile with index 0 */
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_AUTODRAW_AND_LONG_WORM:
        {
            if (!mtmp)
                return glyph;

            if (count_wsegs(mtmp) > 0)
            {
                /* Return the first tile with index 0 */
                return sign * (0 + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_PIERCER:
        {
            if (!mtmp && !(monster_flags & LMFLAGS_DROPPING_PIERCER))
                return glyph;

            if ((monster_flags & LMFLAGS_DROPPING_PIERCER) || (mtmp && mtmp->mundetected))
            {
                int glyph_idx = (monster_flags & LMFLAGS_DROPPING_PIERCER) ? 1 : 0;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_BEAR_TRAP:
        {
            if (!(layer_flags & LFLAGS_T_TRAPPED))
                return glyph;

            if (layer_flags & LFLAGS_T_TRAPPED)
            {
                int glyph_idx = 0;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_COFFIN:
        {
            if (!otmp)
                return glyph;

            if (otmp->speflags & SPEFLAGS_LID_OPENED)
            {
                int glyph_idx = 0;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_ALIGNED_PRIEST:
        {
            if (!mtmp)
                return glyph;

            int glyph_idx = 0;
            aligntyp algn = mon_aligntyp(mtmp);
            switch (algn)
            {
            case A_NONE:
                glyph_idx = 0;
                break;
            case A_CHAOTIC:
            case A_NEUTRAL:
            case A_LAWFUL:
                glyph_idx = algn + 2;
                break;
            default:
                return glyph;
            }
            return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_ALIGNED_PRIEST_STATUE:
        {
            if (!otmp)
                return glyph;

            struct monst* mtmp2 = get_mtraits(otmp, FALSE);
            if (mtmp2 && has_epri(mtmp2))
            {
                int glyph_idx = 0;
                switch (EPRI(mtmp2)->shralign)
                {
                case A_NONE:
                    glyph_idx = 0;
                    break;
                case A_CHAOTIC:
                case A_NEUTRAL:
                case A_LAWFUL:
                    glyph_idx = EPRI(mtmp2)->shralign + 2;
                    break;
                default:
                    return glyph;
                }
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_ANGEL:
        {
            if (!mtmp)
                return glyph;

            int glyph_idx = 0;
            aligntyp algn = mon_aligntyp(mtmp);
            switch (algn)
            {
            case A_NONE:
                glyph_idx = 0;
                break;
            default:
                return glyph;
            }
            return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_ANGEL_STATUE:
        {
            if (!otmp)
                return glyph;

            struct monst* mtmp2 = get_mtraits(otmp, FALSE);
            if (mtmp2 && has_epri(mtmp2))
            {
                int glyph_idx = 0;
                switch (EPRI(mtmp2)->shralign)
                {
                case A_NONE:
                    glyph_idx = 0;
                    break;
                default:
                    return glyph;
                }
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_ACOLYTE:
        {
            int glyph_idx = 0;
            aligntyp algn = u.ualignbase[A_ORIGINAL];
            switch (algn)
            {
            case A_CHAOTIC:
                glyph_idx = 0;
                break;
            case A_NEUTRAL:
                glyph_idx = 1;
                break;
            default:
                return glyph;
            }
            return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_TORCH_HOLDER:
        {
            if (isok(x, y) && levl[x][y].decoration_typ > 0 && (levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
            {
                int glyph_idx = 0;
                if (get_location_light_range(x, y) != 0 && levl[x][y].lamplit == TRUE)
                    glyph_idx = 1;

                /* Return the first tile with index 0 */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_FIREPLACE:
        {
            if (isok(x, y) && levl[x][y].decoration_typ > 0 && get_location_light_range(x, y) != 0 && levl[x][y].lamplit == TRUE)
            {
                int glyph_idx = 0;

                /* Return the first tile with index 0 */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_PLATE_MAIL_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_BRONZE:
                glyph_idx = 0;
                break;
            case MAT_HARD_CRYSTAL:
                glyph_idx = 1;
                break;
            case MAT_ORICHALCUM:
                glyph_idx = 2;
                break;
            case MAT_MITHRIL:
                glyph_idx = 3;
                break;
            case MAT_ADAMANTIUM:
                glyph_idx = 4;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_SILVER_BONE_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_SILVER:
                glyph_idx = 0;
                break;
            case MAT_BONE:
                glyph_idx = 1;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_SILVER_LEAD_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_SILVER:
                glyph_idx = 0;
                break;
            case MAT_LEAD:
                glyph_idx = 1;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);            break;
        }
        case REPLACEMENT_ACTION_SILVER_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_SILVER:
                glyph_idx = 0;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);            break;
        }
        case REPLACEMENT_ACTION_STEEL_MITHRIL_GEMSTONE_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_MITHRIL:
                glyph_idx = 0;
                break;
            case MAT_STEEL:
                glyph_idx = 1;
                break;
            case MAT_GEMSTONE:
                glyph_idx = 2;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);            break;
        }
        case REPLACEMENT_ACTION_ADAMANTIUM_MITHRIL_SILVER_BONE_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_ADAMANTIUM:
                glyph_idx = 0;
                break;
            case MAT_MITHRIL:
                glyph_idx = 1;
                break;
            case MAT_SILVER:
                glyph_idx = 2;
                break;
            case MAT_BONE:
                glyph_idx = 3;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);            break;
        }
        case REPLACEMENT_ACTION_SILVER_CRYSTAL_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_SILVER:
                glyph_idx = 0;
                break;
            case MAT_GLASS:
            case MAT_CRYSTAL:
            case MAT_HARD_CRYSTAL:
                glyph_idx = 1;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);            break;
        }
        case REPLACEMENT_ACTION_BONE_MATERIAL:
        {
            uchar material = MAT_NONE;
            if (otmp)
                material = otmp->material;
            else if (info.layer == LAYER_MISSILE)
                material = missile_material;
            else
                return glyph;

            int glyph_idx = 0;
            switch (material)
            {
            case MAT_BONE:
                glyph_idx = 0;
                break;
            default:
                return glyph;
            }
            int adj_glyph_idx = is_missile_with_dirs ? glyph_missile_dir + glyph_idx * NUM_MISSILE_DIRS : glyph_idx;
            return sign * (adj_glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);            break;
        }
        case REPLACEMENT_ACTION_PAINTING:
        {
            short special_quality = 0;
            if (otmp)
                special_quality = otmp->special_quality;
            else
                special_quality = missile_special_quality;

            if (special_quality > 0 && special_quality <= replacements[replacement_idx].number_of_tiles)
            {
                int glyph_idx = special_quality - 1;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_PAINTING_HOLDER:
        {
            if (isok(x, y) && levl[x][y].decoration_typ > 0 && (levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
            {
                int glyph_idx = levl[x][y].decoration_subtyp;

                /* Return the first tile with index 0 */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_WALL_SCULPTURE:
        {
            if (isok(x, y) && levl[x][y].decoration_typ > 0 && levl[x][y].decoration_subtyp > 0)
            {
                int glyph_idx = levl[x][y].decoration_subtyp - 1;

                /* Return the first tile with index 0 */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_STATUE_NICHE:
        {
            if (isok(x, y) && levl[x][y].decoration_typ > 0 && (levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
            {
                int glyph_idx = 0;

                /* Return the first tile with index 0 */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_EQUIPMENT_HOLDER:
        {
            if (isok(x, y) && levl[x][y].decoration_typ > 0 && (levl[x][y].decoration_flags & (DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER)) != 0)
            {
                int glyph_idx = 0;
                switch ((levl[x][y].decoration_flags & (DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER)))
                {
                case DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER:
                    glyph_idx = 0;
                    break;
                case DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER:
                    glyph_idx = 1;
                    break;
                case DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER:
                    glyph_idx = 2;
                    break;
                case DECORATION_FLAGS_ITEM_IN_HOLDER:
                    glyph_idx = 3;
                    break;
                case DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER:
                    glyph_idx = 4;
                    break;
                case DECORATION_FLAGS_ITEM2_IN_HOLDER:
                    glyph_idx = 5;
                    break;
                case DECORATION_FLAGS_ITEM3_IN_HOLDER:
                    glyph_idx = 6;
                    break;
                default:
                    return glyph;
                }

                /* Return the first tile with index 0 */
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_BREED:
        {
            if (!mtmp || !mtmp->subtype || mtmp->subtype > MAX_TILES_PER_REPLACEMENT || mtmp->subtype > replacements[replacement_idx].number_of_tiles)
                return glyph;

            int glyph_idx = (int)mtmp->subtype - 1;
            return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_STATUE_BREED:
        {
            if (!otmp)
                return glyph;

            struct monst* mtmp2 = get_mtraits(otmp, FALSE);
            if (mtmp2 && mtmp2->subtype && mtmp2->subtype <= MAX_TILES_PER_REPLACEMENT)
            {
                int glyph_idx = mtmp2->subtype - 1;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        case REPLACEMENT_ACTION_SADDLED:
        {
            if (!mtmp || !which_armor(mtmp, W_SADDLE))
                return glyph;

            int glyph_idx = 0;
            return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            break;
        }
        case REPLACEMENT_ACTION_STATUE_SADDLED:
        {
            if (!otmp)
                return glyph;

            struct monst* mtmp2 = get_mtraits(otmp, FALSE);
            if (mtmp2 && (which_armor(mtmp2, W_SADDLE) || contains_otyp(otmp, SADDLE)))
            {
                int glyph_idx = 0;
                return sign * (glyph_idx + replacement_offsets[replacement_idx] /* replacements[replacement_idx].glyph_offset */ + GLYPH_REPLACEMENT_OFF);
            }
            break;
        }
        default:
            break;
        }
    }
#else
int glyph UNUSED;
int x UNUSED, y UNUSED;
struct replacement_info info UNUSED;
{
#endif
    return glyph;
}



STATIC_OVL int
get_shore_tile_index(mainlev, lev)
struct rm* mainlev, *lev;
{
    if (!mainlev || !lev)
        return -1;

    int mainlev_is_pool_ice_or_lava = (mainlev && (IS_POOL(mainlev->typ) || mainlev->typ == ICE || mainlev->typ == LAVAPOOL));
    if (lev->typ == WATER)
    {
        if (mainlev->typ == WATER)
            return -1;
        else
            return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 6;
    }
    else if (lev->typ == CLOUD)
    {
        if (mainlev->typ == CLOUD)
            return -1;
        else
            return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 5;
    }
    else if (lev->typ == AIR)
    {
        if (mainlev->typ == AIR)
            return -1;
        else
            return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 4;
    }
    else if (lev->typ == DRAWBRIDGE_DOWN)
    {
        return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 3;
    }
    else if (lev->typ == LAVAPOOL || (lev->typ == DRAWBRIDGE_UP && lev->flags & DB_LAVA))
    {
        if(mainlev->typ == LAVAPOOL)
            return -1;
        else
            return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 2;
    }
    else if (lev->typ == ICE || (lev->typ == DRAWBRIDGE_UP && lev->flags & DB_ICE))
    {
        if (mainlev->typ == ICE)
            return -1;
        else
            return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 1;
    }
    else if ((IS_POOL(lev->typ) && lev->typ != DRAWBRIDGE_UP) || (lev->typ == DRAWBRIDGE_UP && (lev->flags & DB_UNDER) == DB_MOAT))
    {
        if (IS_POOL(mainlev->typ))
            return -1;
        else
            return MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES + 0;
    }
    else if (lev->typ == CORR)
    {
        if ((!mainlev_is_pool_ice_or_lava && mainlev->typ == lev->typ && mainlev->subtyp == lev->subtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->typ && mainlev->floorsubtyp == lev->subtyp)
            )
            return -1;
        else
            return lev->subtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES;
    }
    else if (!IS_FLOOR(lev->typ) && lev->floortyp == CORR)
    {
        if ((!mainlev_is_pool_ice_or_lava && IS_FURNITURE(lev->typ) && mainlev->typ == lev->floortyp && mainlev->subtyp == lev->floorsubtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->floortyp && mainlev->floorsubtyp == lev->floorsubtyp)
            )
            return -1;
        else
            return lev->floorsubtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES;
    }
    else if (lev->typ == GROUND || (lev->typ == DRAWBRIDGE_UP && (lev->flags & DB_UNDER) == DB_GROUND))
    {
        if ((!mainlev_is_pool_ice_or_lava &&  mainlev->typ == lev->typ && mainlev->subtyp == lev->subtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->typ && mainlev->floorsubtyp == lev->subtyp)
            )
            return -1;
        else
            return lev->subtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES;
    }
    else if (!IS_FLOOR(lev->typ) && lev->floortyp == GROUND)
    {
        if ((!mainlev_is_pool_ice_or_lava && IS_FURNITURE(lev->typ) && mainlev->typ == lev->floortyp && mainlev->subtyp == lev->floorsubtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->floortyp && mainlev->floorsubtyp == lev->floorsubtyp)
            )
            return -1;
        else
            return lev->floorsubtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES;
    }
    else if (lev->typ == GRASS)
    {
        if ((!mainlev_is_pool_ice_or_lava && mainlev->typ == lev->typ && mainlev->subtyp == lev->subtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->typ && mainlev->floorsubtyp == lev->subtyp)
            )
            return -1;
        else
            return lev->subtyp + MAX_FLOOR_SUBTYPES;
    }
    else if (!IS_FLOOR(lev->typ) && lev->floortyp == GRASS)
    {
        if ((!mainlev_is_pool_ice_or_lava && IS_FURNITURE(lev->typ) && mainlev->typ == lev->floortyp && mainlev->subtyp == lev->floorsubtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->floortyp && mainlev->floorsubtyp == lev->floorsubtyp)
            )
            return -1;
        else
            return lev->floorsubtyp + MAX_FLOOR_SUBTYPES;
    }
    else if (lev->typ == ROOM)
    {
        if ((!mainlev_is_pool_ice_or_lava && mainlev->typ == lev->typ && mainlev->subtyp == lev->subtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->typ && mainlev->floorsubtyp == lev->subtyp)
            )
            return -1;
        else
            return lev->subtyp;
    }
    else if (!IS_FLOOR(lev->typ) && lev->floortyp == ROOM)
    {
        if ((!mainlev_is_pool_ice_or_lava && IS_FURNITURE(lev->typ) && mainlev->typ == lev->floortyp && mainlev->subtyp == lev->floorsubtyp)
            || (!mainlev_is_pool_ice_or_lava && IS_FURNITURE(mainlev->typ) && mainlev->floortyp == lev->floortyp && mainlev->floorsubtyp == lev->floorsubtyp)
            )
            return -1;
        else
            return lev->floorsubtyp;
    }

    return 0;
}


STATIC_OVL int
get_solid_floor_tile_index(lev)
struct rm* lev;
{
    if (lev->typ == CORR)
    {
        return lev->subtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES;
    }
    else if (lev->floortyp == CORR)
    {
        return lev->floorsubtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES;
    }
    else if (lev->typ == GROUND)
    {
        return lev->subtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES;
    }
    else if (lev->floortyp == GROUND)
    {
        return lev->floorsubtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES;
    }
    else if (lev->typ == GRASS)
    {
        return lev->subtyp + MAX_FLOOR_SUBTYPES;
    }
    else if (lev->floortyp == GRASS)
    {
        return lev->floorsubtyp + MAX_FLOOR_SUBTYPES;
    }
    else if (lev->typ == ROOM)
    {
        return lev->subtyp;
    }
    else if (lev->floortyp == ROOM)
    {
        return lev->floorsubtyp;
    }

    return 0;
}

STATIC_OVL int
get_shore_and_floor_adjusted_tile_index(lev, above_lev, left_lev, right_lev)
struct rm *lev, *above_lev, *left_lev, *right_lev;
{
    int typ = 0;


    if (lev->typ == CORR)
    {
        typ = lev->subtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES;
    }
    else if (lev->floortyp == CORR)
    {
        typ = lev->floorsubtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES;
    }
    else if (lev->typ == GROUND)
    {
        typ = lev->subtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES;
    }
    else if (lev->floortyp == GROUND)
    {
        typ = lev->floorsubtyp + MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES;
    }
    else if (lev->typ == GRASS)
    {
        typ = lev->subtyp + MAX_FLOOR_SUBTYPES;
    }
    else if (lev->floortyp == GRASS)
    {
        typ = lev->floorsubtyp + MAX_FLOOR_SUBTYPES;
    }
    else if (lev->typ == ROOM)
    {
        typ = lev->subtyp;
    }
    else if (lev->floortyp == ROOM)
    {
        typ = lev->floorsubtyp;
    }

    if ((above_lev && (IS_POOL(above_lev->typ) || above_lev->typ == LAVAPOOL || above_lev->typ == ICE))
        || (left_lev && (IS_POOL(left_lev->typ) || left_lev->typ == LAVAPOOL || left_lev->typ == ICE))
        || (right_lev && (IS_POOL(right_lev->typ) || right_lev->typ == LAVAPOOL || right_lev->typ == ICE))
        )
    {
        /* Pick smaller version */
        typ += MAX_FLOOR_SUBTYPES + MAX_GRASS_SUBTYPES + MAX_GROUND_SUBTYPES + MAX_CORRIDOR_SUBTYPES;
    }

    return typ;
}

int
maybe_get_animated_tile(ntile, tile_animation_idx, play_type, interval_counter, frame_idx_ptr, main_tile_idx_ptr, mapAnimated, autodraw_ptr)
int ntile;
int tile_animation_idx;
enum animation_play_types play_type;
int64_t interval_counter;
int *frame_idx_ptr, *main_tile_idx_ptr;
char* mapAnimated;
enum autodraw_types* autodraw_ptr;
{
#ifdef USE_TILES
    if (frame_idx_ptr)
        *frame_idx_ptr = -1;
    if (main_tile_idx_ptr)
        *main_tile_idx_ptr = -1;

    short animation_idx = tile2animation[ntile];
    if (animation_idx > 0)
    {
        if (autodraw_ptr)
            *autodraw_ptr = animations[animation_idx].main_tile_autodraw;
        if (animations[animation_idx].number_of_frames < 1)
            return ntile;
        if (animations[animation_idx].play_type != play_type)
            return ntile;

        if (mapAnimated)
        {
            if(!(*mapAnimated) || *mapAnimated > animations[animation_idx].intervals_between_frames)
                *mapAnimated = max(1, animations[animation_idx].intervals_between_frames);
        }

        schar main_tile_frame_position = -1; /* ignore */
        if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_FIRST)
        {
            main_tile_frame_position = 0;
        }
        else if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_LAST)
        {
            main_tile_frame_position = animations[animation_idx].number_of_frames;
        }
        if (main_tile_idx_ptr)
            *main_tile_idx_ptr = main_tile_frame_position;
        schar additional_tile_num = (main_tile_frame_position > -1 ? 1 : 0);
        schar animation_tile_offset = (main_tile_frame_position == 0 ? 1 : 0);
        schar numframes = animations[animation_idx].number_of_frames + additional_tile_num; /* add original tile as the first tile and frame */
        schar current_animation_frame = (char)((interval_counter / (int64_t)animations[animation_idx].intervals_between_frames) % (int64_t)numframes);
        if (current_animation_frame < 0)
            current_animation_frame += numframes;

        if (frame_idx_ptr)
            *frame_idx_ptr = current_animation_frame;
        
        /* Separately played animations are played only once, and once the numframes is exceeded, the animation stops */
        if (play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY
            && (interval_counter / (int64_t)animations[animation_idx].intervals_between_frames) >= (int64_t)numframes)
        {
            if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_IGNORE)
            {
                current_animation_frame = (int)animations[animation_idx].number_of_frames - 1; /* Set to last frame of the animation */
            }
            else
            {
                return ntile;  /* Return the original tile in other cases */
            }
        }

        if (current_animation_frame != main_tile_frame_position) /* 0 is the original picture */
        {
            int animation_frame_index = current_animation_frame - animation_tile_offset;
            if (animation_frame_index >= (int)animations[animation_idx].number_of_frames)
            {
                return ntile; /* original tile is the last if number_of_frames is exceeded by numframes */
            }

            if (autodraw_ptr)
                *autodraw_ptr = animations[animation_idx].frame_autodraw[animation_frame_index];

            int tile_anim_idx = (tile_animation_idx < 0 || tile_animation_idx >= animations[animation_idx].number_of_tile_animations ? 0 : tile_animation_idx);
            int animation_glyph = animation_frame_index + tile_anim_idx * (int)animations[animation_idx].number_of_frames + animation_offsets[animation_idx] /* animations[animation_idx].glyph_offset */ + GLYPH_ANIMATION_OFF;
            int res = glyph2tile[animation_glyph]; /* animated version selected */

            return res;
        }
    }
#endif
    return ntile;
}


int
get_tile_animation_index_from_glyph(glyph)
int glyph;
{
    int absglyph = abs(glyph);
    if (glyph_is_swallow(absglyph))
    {
        int glyph_idx = absglyph - GLYPH_SWALLOW_OFF;
        int char_idx = glyph_idx % MAX_SWALLOW_CHARS;
        return char_idx;
    }
    else if (glyph_is_explosion(absglyph))
    {
        int glyph_idx = absglyph - GLYPH_EXPLODE_OFF;
        int char_idx = glyph_idx % MAX_EXPLOSION_CHARS;
        return char_idx;
    }
    else if (glyph_is_zap(absglyph))
    {
        int glyph_idx = absglyph - GLYPH_ZAP_OFF;
        int char_idx = glyph_idx % NUM_ZAP_CHARS;
        return char_idx;
    }
    else if (glyph_is_worm(absglyph))
    {
        int glyph_idx = absglyph - GLYPH_WORM_OFF;
        int char_idx = glyph_idx % MAX_WORM_TILES;
        return char_idx;
    }
    else
        return 0;

}

short
get_player_enlargement(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    if (roleidx == 0 && raceidx == 0 && genderidx == 0 && alignmentidx == 0 && levelidx == 0)
    {
        /* Do nothing */
    }

    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        break;
    case ACTION_TILE_CAST_NODIR:
        break;
    case ACTION_TILE_CAST_DIR:
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        break;
    case ACTION_TILE_KICK:
        break;
    case ACTION_TILE_PASSIVE_DEFENSE:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_2:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_3:
        break;
    case ACTION_TILE_ITEM_USE:
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }
    return 0;
}


int
get_animation_base_tile(animidx)
short animidx;
{
#ifdef USE_TILES
    int i, j, cmap_idx;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        int action;
        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].animation.actions[action] == animidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 0)];
        }
        if (mons[i].animation.statue == animidx)
            return glyph2tile[i + GLYPH_STATUE_OFF];
        if (mons[i].animation.corpse == animidx)
            return glyph2tile[i + GLYPH_BODY_OFF];

        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].female_animation.actions[action] == animidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 1)];
        }
        if (mons[i].female_animation.corpse == animidx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
        if (mons[i].female_animation.statue == animidx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
    }

    for (i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if(obj_descr[objects[i].oc_descr_idx].stand_animation == animidx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
    }

    for (cmap_idx = 0; cmap_idx < MAX_CMAP_TYPES; cmap_idx++)
    {
        for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].stand_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
            if (defsyms[i].broken_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_BROKEN_CMAP_OFF];
        }

        for (i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].stand_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
            if (defsym_variations[i].broken_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_BROKEN_CMAP_VARIATION_OFF];
        }
    }

    int roleidx, raceidx, genderidx, alignment, glevel;
    for (roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (genderidx = 0; genderidx <= 1; genderidx++)
            {
                for (alignment = A_CHAOTIC; alignment <= A_LAWFUL; alignment++)
                {
                    for (glevel = 0; glevel < NUM_PLAYER_GLYPH_LEVELS; glevel++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, genderidx, alignment, glevel);
                        int action;
                        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                        {
                            if (get_player_animation(action, roleidx, raceidx, genderidx, alignment, glevel) == animidx)
                                return glyph2tile[player_glyph_index + get_player_action_glyph_offset(action)];
                        }
                    }
                }
            }
        }
    }

    /* Explosion */
    for (i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosion_type_definitions[i].animation == animidx)
            return glyph2tile[i * MAX_EXPLOSION_CHARS + GLYPH_EXPLODE_OFF];
    }

    /* Zap */
    for (i = 0; i < MAX_ZAP_TYPES; i++)
    {
        if (zap_type_definitions[i].animation == animidx)
            return glyph2tile[ZAP_INDEX_WITH_FIRST_TILE +  i * NUM_ZAP_CHARS + GLYPH_ZAP_OFF];
    }

    /* Swallow */
    for (i = 0; i < NUM_MONSTERS; i++)
    {
        if (mons[i].animation.swallow == animidx || mons[i].female_animation.swallow == animidx)
            return glyph2tile[i * MAX_SWALLOW_CHARS + GLYPH_SWALLOW_OFF];
    }

    /* Special Effects */
    for (i = SPECIAL_EFFECT_TELEPORT_OUT; i < MAX_SPECIAL_EFFECTS; i++)
    {
        if (special_effects[i].animation == animidx)
            return glyph2tile[i + GLYPH_SPECIAL_EFFECT_OFF];
    }

    /* Cursors */
    for (i = CURSOR_STYLE_GENERIC_CURSOR; i < MAX_CURSORS; i++)
    {
        if (game_cursors[i].animation == animidx)
            return glyph2tile[i + GLYPH_CURSOR_OFF];
    }

    /* Hit tiles */
    for (i = HIT_GENERAL; i < MAX_HIT_TILES; i++)
    {
        if (hit_tile_definitions[i].animation == animidx)
            return glyph2tile[i + GLYPH_HIT_TILE_OFF];
    }

    /* General tiles */
    for (i = GENERAL_TILE_DEATH; i < MAX_GENERAL_TILES; i++)
    {
        if (general_tile_definitions[i].animation == animidx)
            return glyph2tile[i + GLYPH_GENERAL_TILE_OFF];
    }

    /* UI Tiles */
    for (i = GENERAL_UI_ELEMENTS; i < MAX_UI_TILES; i++)
    {
        if (ui_tile_component_array[i].animation == animidx)
            return glyph2tile[i + GLYPH_UI_TILE_OFF];
    }

    for (i = 0; i < MAX_SIMPLE_DOODAD_TILES; i++)
    {
        if (simple_doodads[i].animation == animidx)
            return glyph2tile[i + GLYPH_SIMPLE_DOODAD_OFF];
    }

    for (i = 0; i < MAX_MIRRORABLE_DOODAD_TILES; i++)
    {
        if (mirrorable_doodads[i].animation == animidx)
            return glyph2tile[i + GLYPH_MIRRORABLE_DOODAD_OFF];
    }


#if 0
    /* Spell tiles */
    for (i = 0; i < MAXSPELL; i++)
    {
        /* Currently spell tiles do not have animations */
        //return glyph2tile[i + GLYPH_SPELL_TILE_OFF];
    }

    /* Skill tiles */
    for (i = 0; i < P_NUM_SKILLS; i++)
    {
        /* Currently skill tiles do not have animations */
        //return glyph2tile[i + GLYPH_SKILL_TILE_OFF];
    }

    /* Command tiles */
    for (i = 0; i < MAX_COMMAND_TILES; i++)
    {
        /* Currently command tiles do not have animations */
        //return glyph2tile[i + GLYPH_COMMAND_TILE_OFF];
    }
#endif

    /* Replacement */
    for (i = 1; i < MAX_REPLACEMENTS; i++)
    {
        for (j = 0; j < replacements[i].number_of_tiles; j++)
        {
            if (replacements[i].tile_animation[j] == animidx)
                return glyph2tile[j + replacement_offsets[i] /* replacements[i].glyph_offset */ + GLYPH_REPLACEMENT_OFF];
        }
    }
#endif

    return -1;
}


int
get_enlargement_base_tile(enlidx, enl_anim_tile_idx)
short enlidx, enl_anim_tile_idx;
{
#ifdef USE_TILES
    int i, j, cmap_idx;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        int action;
        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].enlargement.actions[action] == enlidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 0)];
        }
        if (mons[i].enlargement.statue == enlidx)
            return glyph2tile[i + GLYPH_STATUE_OFF];
        if (mons[i].enlargement.corpse == enlidx)
            return glyph2tile[i + GLYPH_BODY_OFF];

        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].female_enlargement.actions[action] == enlidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 1)];
        }
        if (mons[i].female_enlargement.statue == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
        if (mons[i].female_enlargement.corpse == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
    }

    for (i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if (obj_descr[objects[i].oc_descr_idx].enlargement == enlidx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
    }

    for (cmap_idx = 0; cmap_idx < MAX_CMAP_TYPES; cmap_idx++)
    {

        for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + cmap_idx * NUM_CMAP_TYPE_CHARS + GLYPH_CMAP_OFF];
            if (defsyms[i].broken_enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + cmap_idx * NUM_CMAP_TYPE_CHARS + GLYPH_BROKEN_CMAP_OFF];
        }

        for (i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + cmap_idx * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF];
            if (defsym_variations[i].broken_enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + cmap_idx * MAX_VARIATIONS + GLYPH_BROKEN_CMAP_VARIATION_OFF];
        }
    }

    int roleidx, raceidx, genderidx, alignment, glevel;
    for (roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (genderidx = 0; genderidx <= 1; genderidx++)
            {
                for (alignment = A_CHAOTIC; alignment <= A_LAWFUL; alignment++)
                {
                    for (glevel = 0; glevel < NUM_PLAYER_GLYPH_LEVELS; glevel++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, genderidx, alignment, glevel);
                        int action;
                        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                        {
                            if (get_player_enlargement(action, roleidx, raceidx, genderidx, alignment, glevel) == enlidx)
                                return glyph2tile[player_glyph_index + get_player_action_glyph_offset(action)];
                        }
                    }
                }
            }
        }
    }

    for (i = SPECIAL_EFFECT_TELEPORT_OUT; i < MAX_SPECIAL_EFFECTS; i++)
    {
        if (special_effects[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_SPECIAL_EFFECT_OFF];
    }

    for (i = CURSOR_STYLE_GENERIC_CURSOR; i < MAX_CURSORS; i++)
    {
        if (game_cursors[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_CURSOR_OFF];
    }

    for (i = HIT_GENERAL; i < MAX_HIT_TILES; i++)
    {
        if (hit_tile_definitions[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_HIT_TILE_OFF];
    }

    for (i = GENERAL_TILE_DEATH; i < MAX_GENERAL_TILES; i++)
    {
        if (general_tile_definitions[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_GENERAL_TILE_OFF];
    }

    for (i = GENERAL_UI_ELEMENTS; i < MAX_UI_TILES; i++)
    {
        if (ui_tile_component_array[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_UI_TILE_OFF];
    }

    for (i = 0; i < MAX_SIMPLE_DOODAD_TILES; i++)
    {
        if (simple_doodads[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_SIMPLE_DOODAD_OFF];
    }

    for (i = 0; i < MAX_MIRRORABLE_DOODAD_TILES; i++)
    {
        if (mirrorable_doodads[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_MIRRORABLE_DOODAD_OFF];
    }


#if 0
    /* Spell tiles */
    for (i = 0; i < MAXSPELL; i++)
    {
        /* Currently spell tiles do not have enlargements */
        //return glyph2tile[i + GLYPH_SPELL_TILE_OFF];
    }

    /* Skill tiles */
    for (i = 0; i < P_NUM_SKILLS; i++)
    {
        /* Currently skill tiles do not have enlargements */
        //return glyph2tile[i + GLYPH_SKILL_TILE_OFF];
    }

    /* Command tiles */
    for (i = 0; i < MAX_COMMAND_TILES; i++)
    {
        /* Currently command tiles do not have enlargements */
        //return glyph2tile[i + GLYPH_COMMAND_TILE_OFF];
    }
#endif

    /* Replacement */
    for (i = 1; i < MAX_REPLACEMENTS; i++)
    {
        for (j = 0; j < replacements[i].number_of_tiles; j++)
        {
            if (replacements[i].tile_enlargement[j] == enlidx)
                return glyph2tile[j + replacement_offsets[i] /* replacements[i].glyph_offset */ + GLYPH_REPLACEMENT_OFF];
        }
    }

    /* Animation */
    for (i = 1; i < MAX_ANIMATIONS; i++)
    {
        for (j = 0; j < animations[i].number_of_tiles; j++)
        {
            if (animations[i].tile_enlargement == enlidx && (enl_anim_tile_idx < 0 || enl_anim_tile_idx == j))
            {
                int res = glyph2tile[get_animation_frame_with_tile(i, j) + animation_offsets[i] /* animations[i].glyph_offset */ + GLYPH_ANIMATION_OFF];
                return res;
            }
        }
    }
#endif

    return -1;
}

int
get_animation_frame_with_tile(animidx, tileidx)
int animidx, tileidx;
{
    int i;
    for (i = 0; i < animations[animidx].number_of_frames; i++)
    {
        if (animations[animidx].frame2tile[i] == tileidx)
            return i;
    }

    return 0;
}

int
get_replacement_base_tile(replacement_idx)
short replacement_idx;
{
#ifdef USE_TILES
    int i, j, k, cmap_idx;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        int action;
        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].replacement.actions[action] == replacement_idx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 0)];
        }
        if (mons[i].replacement.statue == replacement_idx)
            return glyph2tile[i + GLYPH_STATUE_OFF];
        if (mons[i].replacement.corpse == replacement_idx)
            return glyph2tile[i + GLYPH_BODY_OFF];

        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].female_replacement.actions[action] == replacement_idx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 1)];
        }
        if (mons[i].female_replacement.corpse == replacement_idx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
        if (mons[i].female_replacement.statue == replacement_idx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
    }

    for (i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if (obj_descr[objects[i].oc_descr_idx].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
        if (obj_descr[objects[i].oc_descr_idx].oc_descr_flags & OD_MISSILE_REPLACEMENTS)
        {
            for (j = 0; j < NUM_BASE_TILE_DIRS; j++)
            {
                if (obj_descr[objects[i].oc_descr_idx].replacement + j + 1 == replacement_idx)
                {
                    for (k = 0; k < NUM_MISSILE_DIRS; k++)
                    {
                        boolean hflip = FALSE;
                        boolean vflip = FALSE;
                        boolean isdirfrombasedir = is_dir_from_base_dir(k, j, &hflip, &vflip);
                        if(isdirfrombasedir)
                            return glyph2tile[k + i * NUM_MISSILE_DIRS + GLYPH_OBJ_MISSILE_OFF];
                    }
                }
            }
        }
    }

    for (i = 1; i <= NUM_ARTIFACTS; i++)
    {
        if (artilist[i].replacement == replacement_idx)
            return glyph2tile[i - 1 + GLYPH_ARTIFACT_OFF];
        if (artilist[i].aflags2 & AF2_MISSILE_REPLACEMENTS)
        {
            for (j = 0; j < NUM_BASE_TILE_DIRS; j++)
            {
                if (artilist[i].replacement + j + 1 == replacement_idx)
                {
                    for (k = 0; k < NUM_MISSILE_DIRS; k++)
                    {
                        boolean hflip = FALSE;
                        boolean vflip = FALSE;
                        boolean isdirfrombasedir = is_dir_from_base_dir(k, j, &hflip, &vflip);
                        if (isdirfrombasedir)
                            return glyph2tile[k + (i - 1) * NUM_MISSILE_DIRS + GLYPH_ARTIFACT_MISSILE_OFF];
                    }
                }
            }
        }
    }

    for (cmap_idx = 0; cmap_idx < MAX_CMAP_TYPES; cmap_idx++)
    {
        for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].replacement[cmap_idx] == replacement_idx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
            if (defsyms[i].broken_replacement[cmap_idx] == replacement_idx)
                return glyph2tile[i + GLYPH_BROKEN_CMAP_OFF];
        }

        for (i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].replacement[cmap_idx] == replacement_idx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
            if (defsym_variations[i].broken_replacement[cmap_idx] == replacement_idx)
                return glyph2tile[i + GLYPH_BROKEN_CMAP_VARIATION_OFF];
        }
    }

    int roleidx, raceidx, genderidx, alignment, glevel;
    for (roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (genderidx = 0; genderidx <= 1; genderidx++)
            {
                for (alignment = A_CHAOTIC; alignment <= A_LAWFUL; alignment++)
                {
                    for (glevel = 0; glevel < NUM_PLAYER_GLYPH_LEVELS; glevel++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, genderidx, alignment, glevel);
                        int action;
                        for (action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                        {
                            if (get_player_replacement(action, roleidx, raceidx, genderidx, alignment, glevel) == replacement_idx)
                                return glyph2tile[player_glyph_index + get_player_action_glyph_offset(action)];
                        }
                    }
                }
            }
        }
    }

    for (i = SPECIAL_EFFECT_TELEPORT_OUT; i < MAX_SPECIAL_EFFECTS; i++)
    {
        if (special_effects[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_SPECIAL_EFFECT_OFF];
    }

    for (i = CURSOR_STYLE_GENERIC_CURSOR; i < MAX_CURSORS; i++)
    {
        if (game_cursors[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_CURSOR_OFF];
    }

    for (i = HIT_GENERAL; i < MAX_HIT_TILES; i++)
    {
        if (hit_tile_definitions[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_HIT_TILE_OFF];
    }

    for (i = GENERAL_TILE_DEATH; i < MAX_GENERAL_TILES; i++)
    {
        if (general_tile_definitions[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_GENERAL_TILE_OFF];
    }

    for (i = GENERAL_UI_ELEMENTS; i < MAX_UI_TILES; i++)
    {
        if (ui_tile_component_array[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_UI_TILE_OFF];
    }

    for (i = 0; i < MAX_SIMPLE_DOODAD_TILES; i++)
    {
        if (simple_doodads[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_SIMPLE_DOODAD_OFF];
    }

    for (i = 0; i < MAX_MIRRORABLE_DOODAD_TILES; i++)
    {
        if (mirrorable_doodads[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_MIRRORABLE_DOODAD_OFF];
    }


#if 0
    /* Spell tiles */
    for (i = 0; i < MAXSPELL; i++)
    {
        /* Currently spell tiles do not have replacements */
        //return glyph2tile[i + GLYPH_SPELL_TILE_OFF];
    }

    /* Skill tiles */
    for (i = 0; i < P_NUM_SKILLS; i++)
    {
        /* Currently skill tiles do not have replacements */
        //return glyph2tile[i + GLYPH_SKILL_TILE_OFF];
    }

    /* Command tiles */
    for (i = 0; i < MAX_COMMAND_TILES; i++)
    {
        /* Currently command tiles do not have replacements */
        //return glyph2tile[i + GLYPH_COMMAND_TILE_OFF];
    }
#endif

#endif

    return -1;
}

void
play_special_effect_at(sp_effect, spef_number, x, y, force_visibility)
enum special_effect_types sp_effect;
int spef_number, x, y;
boolean force_visibility;
{
    enum layer_types layer = special_effects[sp_effect].layer;
    short anim = special_effects[sp_effect].animation;
    int frames_to_sound = special_effects[sp_effect].frames_to_sound;
    int frames_from_sound_to_action = special_effects[sp_effect].frames_from_sound_to_action;
    int frames_from_action_to_end = special_effects[sp_effect].frames_from_action_to_end;;
    play_special_effect_with_details_at(spef_number, x, y, sp_effect + GLYPH_SPECIAL_EFFECT_OFF, layer, anim, frames_to_sound, frames_from_sound_to_action, frames_from_action_to_end, force_visibility);
}

void
play_special_effect_with_details_at(spef_number, x, y, glyph, layer, anim, frames_to_sound, frames_from_sound_to_action, frames_from_action_to_end, force_visibility)
enum layer_types layer;
short anim; /* -1 = fade glyph in, -2: fade glyph out */
int glyph, spef_number, frames_to_sound, frames_from_sound_to_action, frames_from_action_to_end, x, y;
boolean force_visibility;
{
    if (iflags.using_gui_tiles && isok(x, y) && spef_number >= 0 && spef_number < MAX_PLAYED_SPECIAL_EFFECTS && (force_visibility || cansee(x, y)))
    {
        context.spef_action_animation_layer[spef_number] = layer;
        context.spef_action_animation_x[spef_number] = x;
        context.spef_action_animation_y[spef_number] = y;
        context.spef_intervals_to_wait_until_action[spef_number] = 0;
        context.spef_intervals_to_wait_until_end[spef_number] = 0;
        context.force_allow_keyboard_commands = TRUE;
        show_glyph_on_layer(x, y, glyph, layer);
        if (anim < 0)
        {
            uint64_t blflag = 0UL;
            switch (layer)
            {
            case LAYER_BACKGROUND_EFFECT:
                blflag = anim == -1 ? LFLAGS_E_BKG_FADE_IN : LFLAGS_E_BKG_FADE_OUT;
                break;
            case LAYER_GENERAL_EFFECT:
                blflag = anim == -1 ? 0UL : LFLAGS_E_GEN_FADE_OUT;
                break;
            default:
                break;

            }
            add_glyph_buffer_layer_flags(x, y, blflag, 0UL);
        }

#ifdef USE_TILES
        if (!anim)
        {
            int atile = glyph2tile[abs(glyph)];
            anim = tile2animation[atile];
        }
#endif
        if (anim > 0 && animations[anim].play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY)
        {
            //context.special_effect_animation_counter[spef_number] = 0L;
            context.special_effect_animation_counter_on[spef_number] = TRUE;
            toggle_animation_timer(ANIMATION_TIMER_SPECIAL_EFFECT, spef_number, TRUE, x, y, layer, 0UL);
            force_redraw_at(x, y);
            flush_screen(1);

            int framenum = animations[anim].number_of_frames + (animations[anim].main_tile_use_style != ANIMATION_MAIN_TILE_IGNORE ? 1 : 0);
            if (animations[anim].sound_play_frame <= -1)
            {
                context.spef_intervals_to_wait_until_action[spef_number] = (uint64_t)((int)animations[anim].intervals_between_frames * framenum);
            }
            else
            {
                if(animations[anim].sound_play_frame > 0)
                    delay_output_intervals((int)animations[anim].intervals_between_frames * (int)animations[anim].sound_play_frame);
  
                //(flags.animation_frame_interval_in_milliseconds > 0 ? flags.animation_frame_interval_in_milliseconds : ANIMATION_FRAME_INTERVAL)

                if (animations[anim].action_execution_frame > animations[anim].sound_play_frame)
                {
                    context.spef_intervals_to_wait_until_action[spef_number] = (uint64_t)((int)animations[anim].intervals_between_frames * (int)(animations[anim].action_execution_frame - animations[anim].sound_play_frame));
                    if (animations[anim].action_execution_frame < framenum)
                        context.spef_intervals_to_wait_until_end[spef_number] = (uint64_t)((int)animations[anim].intervals_between_frames * (int)(framenum - animations[anim].action_execution_frame));
                }
                else
                {
                    context.spef_intervals_to_wait_until_action[spef_number] = (uint64_t)((int)animations[anim].intervals_between_frames * (int)(framenum - animations[anim].sound_play_frame));
                    context.spef_intervals_to_wait_until_end[spef_number] = 0UL;
                }
            }
        }
        else
        {
            force_redraw_at(x, y);
            flush_screen(1);
            if(frames_to_sound > 0)
                delay_output_intervals(frames_to_sound);
            context.spef_intervals_to_wait_until_action[spef_number] = frames_from_sound_to_action;
            context.spef_intervals_to_wait_until_end[spef_number] = frames_from_action_to_end;
        }
    }
}

void
special_effect_wait_until_action(spef_number)
int spef_number;
{
    if (context.spef_intervals_to_wait_until_action[spef_number] > 0UL)
    {
        delay_output_intervals((int)context.spef_intervals_to_wait_until_action[spef_number]);
        context.spef_intervals_to_wait_until_action[spef_number] = 0UL;
    }
}

void
special_effect_wait_until_end(spef_number)
int spef_number;
{
    if (context.spef_intervals_to_wait_until_end[spef_number] > 0)
    {
        delay_output_intervals((int)context.spef_intervals_to_wait_until_end[spef_number]);
        context.spef_intervals_to_wait_until_end[spef_number] = 0UL;
    }

    toggle_animation_timer(ANIMATION_TIMER_SPECIAL_EFFECT, spef_number, 0, 0, 0, 0, 0UL);
    context.special_effect_animation_counter_on[spef_number] = FALSE;
    //context.special_effect_animation_counter[spef_number] = 0L;
    context.spef_intervals_to_wait_until_action[spef_number] = 0L;

    if (isok(context.spef_action_animation_x[spef_number], context.spef_action_animation_y[spef_number]))
    {
        show_glyph_on_layer(context.spef_action_animation_x[spef_number], context.spef_action_animation_y[spef_number], NO_GLYPH, context.spef_action_animation_layer[spef_number]);
        force_redraw_at(context.spef_action_animation_x[spef_number], context.spef_action_animation_y[spef_number]);
        flush_screen(1);
    }
    context.force_allow_keyboard_commands = FALSE;
}

void
reduce_counters(milliseconds)
int milliseconds;
{
    if (milliseconds < 0)
        return;

    int interval_length = (flags.animation_frame_interval_in_milliseconds > 0 ? flags.animation_frame_interval_in_milliseconds : ANIMATION_FRAME_INTERVAL);
    int intervals = milliseconds / interval_length;

    reduce_counters_intervals(intervals);
}


void
reduce_counters_intervals(intervals)
int intervals;
{
    int i;

    /* Reduce animation ending counters */
    if (context.u_action_animation_counter_on && context.u_intervals_to_wait_until_action == 0UL && context.u_intervals_to_wait_until_end > 0UL)
    {
        if (context.u_intervals_to_wait_until_end <= (uint64_t)intervals)
            context.u_intervals_to_wait_until_end = 0UL;
        else
            context.u_intervals_to_wait_until_end -= (uint64_t)intervals;
    }

    if (context.m_action_animation_counter_on && context.m_intervals_to_wait_until_action == 0UL && context.m_intervals_to_wait_until_end > 0UL)
    {
        if (context.m_intervals_to_wait_until_end <= (uint64_t)intervals)
            context.m_intervals_to_wait_until_end = 0UL;
        else
            context.m_intervals_to_wait_until_end -= (uint64_t)intervals;
    }

    if (context.explosion_animation_counter_on && context.expl_intervals_to_wait_until_action == 0UL && context.expl_intervals_to_wait_until_end > 0UL)
    {
        if (context.expl_intervals_to_wait_until_end <= (uint64_t)intervals)
            context.expl_intervals_to_wait_until_end = 0UL;
        else
            context.expl_intervals_to_wait_until_end -= (uint64_t)intervals;
    }

    for (i = 0; i < MAX_PLAYED_SPECIAL_EFFECTS; i++)
    {
        if (context.special_effect_animation_counter_on[i] && context.spef_intervals_to_wait_until_action[i] == 0UL && context.spef_intervals_to_wait_until_end[i] > 0UL)
        {
            if (context.spef_intervals_to_wait_until_end[i] <= (uint64_t)intervals)
                context.spef_intervals_to_wait_until_end[i] = 0UL;
            else
                context.spef_intervals_to_wait_until_end[i] -= (uint64_t)intervals;
        }
    }

    if (context.zap_aggregate_intervals_to_wait_until_action > 0UL)
    {
        if (context.zap_aggregate_intervals_to_wait_until_action <= (uint64_t)intervals)
            context.zap_aggregate_intervals_to_wait_until_action = 0UL;
        else
            context.zap_aggregate_intervals_to_wait_until_action -= (uint64_t)intervals;
    }

    if (context.zap_aggregate_intervals_to_wait_until_end > 0UL)
    {
        if (context.zap_aggregate_intervals_to_wait_until_end <= (uint64_t)intervals)
            context.zap_aggregate_intervals_to_wait_until_end = 0UL;
        else
            context.zap_aggregate_intervals_to_wait_until_end -= (uint64_t)intervals;
    }
}

void
stop_animations()
{
    if (toggle_animation_timer)
    {
        toggle_animation_timer(ANIMATION_TIMER_YOU, 0, 0, 0, 0, 0, 0UL);
        toggle_animation_timer(ANIMATION_TIMER_MONSTER, 0, 0, 0, 0, 0, 0UL);
        toggle_animation_timer(ANIMATION_TIMER_EXPLOSION, 0, 0, 0, 0, 0, 0UL);
        int i;
        for (i = 0; i < MAX_PLAYED_SPECIAL_EFFECTS; i++)
            toggle_animation_timer(ANIMATION_TIMER_SPECIAL_EFFECT, i, 0, 0, 0, 0, 0UL);
        for (i = 0; i < MAX_PLAYED_ZAP_ANIMATIONS; i++)
            toggle_animation_timer(ANIMATION_TIMER_ZAP, i, 0, 0, 0, 0, 0UL);
    }
}


boolean
glyph_is_specific_cmap_or_its_variation(glyph, s_idx)
int glyph, s_idx;
{
    if (s_idx == S_unexplored)
    {
        if (glyph_is_cmap(glyph))
        {
            if (glyph_to_cmap(glyph) == s_idx)
                return TRUE;
        }
        
        if (glyph_is_cmap_variation(glyph))
        {
            if (defsym_variations[glyph_to_cmap_variation(glyph)].base_screen_symbol == s_idx)
                return TRUE;
        }
    }
    else
    {
        if (generic_glyph_to_cmap(glyph) == s_idx)
            return TRUE;
    }

#if 0
    int base_glyph = cmap_to_glyph(s_idx);
    if (glyph == base_glyph)
        return TRUE;

    int variations = defsyms[s_idx].variations;
    int variation_offset = defsyms[s_idx].variation_offset;

    int var_glyph = 0;
    for (int i = 0; i < variations; i++)
    {
        var_glyph = cmap_variation_to_glyph(i + variation_offset);
        if (glyph == var_glyph)
            return TRUE;
    }
#endif


    return FALSE;
}

boolean
no_wall_end_autodraw(x, y)
int x, y;
{
    if(!isok(x, y))
        return TRUE;
    
    if(glyph_is_specific_cmap_or_its_variation(levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR], S_unexplored))
        return TRUE;
        
    if(cansee(x, y))
        return (IS_DOORJOIN(levl[x][y].typ) && !IS_TREE(levl[x][y].typ)) || levl[x][y].typ == DOOR || levl[x][y].typ == UNDEFINED_LOCATION; /*|| (levl[x][y].seenv & (SV4 | SV5 | SV6)) == 0 */
    else
    {
        int relevant_glyph = levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FEATURE] != NO_GLYPH ? levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FEATURE] : levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR];
        int cmaptyp = generic_glyph_to_cmap(relevant_glyph);
        if (cmaptyp == S_unexplored)
            return TRUE;
        else
        {
            int levtyp = defsyms[cmaptyp].implied_levtyp;
            return (IS_DOORJOIN(levtyp) && !IS_TREE(levtyp)) || levtyp == DOOR || levtyp == UNDEFINED_LOCATION;
        }
    }
}

/* animation.c */



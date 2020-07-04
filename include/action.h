/*
 * Actions. Copyright (c) Janne Gustafsson, 2020
 *
 */

#ifndef ACTION_H
#define ACTION_H

/* Action tiles */
enum action_tile_types
{
    ACTION_TILE_NO_ACTION = 0,
    ACTION_TILE_ATTACK = 1,
    ACTION_TILE_THROW,
    ACTION_TILE_FIRE,
    ACTION_TILE_CAST,
    ACTION_TILE_SPECIAL_ATTACK,
    ACTION_TILE_KICK,
    ACTION_TILE_ITEM_USE,
    ACTION_TILE_DOOR_USE,
    ACTION_TILE_DEATH,
    MAX_ACTION_TILES
};

enum innate_movement_styles {
	MOVEMENT_STYLE_ON_GROUND = 0,
	MOVEMENT_STYLE_STEALTH,
	MOVEMENT_STYLE_FLYING,
	MOVEMENT_STYLE_LEVITATING,
	MOVEMENT_STYLE_SWIMMING,
	MOVEMENT_STYLE_PASSING_THROUGH_WALLS,
	MAX_MOVEMENT_STYLES
};

enum floor_surface_types {
	FLOOR_SURFACE_NONE = 0,
	FLOOR_SURFACE_CARPET,
	FLOOR_SURFACE_GRASS,
	FLOOR_SURFACE_WOODEN,
	FLOOR_SURFACE_STONE,
	FLOOR_SURFACE_GROUND,
	FLOOR_SURFACE_SAND,
	FLOOR_SURFACE_WATER,
	MAX_FLOOR_SURFACES
};


/*	Max # of attacks for any given monster. -- Moved here from permonst.h --JG
 */

#define NATTK 8	    /* Maximum number of attacks per monster */
#define NRAYS 10	/* Maximum number of rays */

#endif /* ACTION_H */
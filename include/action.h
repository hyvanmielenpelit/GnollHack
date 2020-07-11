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
    ACTION_TILE_PASSIVE_DEFENSE,
    ACTION_TILE_DEFEND,
    ACTION_TILE_RECEIVE_DAMAGE,
    ACTION_TILE_ITEM_USE,
    ACTION_TILE_DOOR_USE,
    ACTION_TILE_DEATH,
    MAX_ACTION_TILES
};


/*	Max # of attacks for any given monster. -- Moved here from permonst.h --JG
 */

#define NATTK 8	    /* Maximum number of attacks per monster */
#define NRAYS 10	/* Maximum number of rays */

#endif /* ACTION_H */
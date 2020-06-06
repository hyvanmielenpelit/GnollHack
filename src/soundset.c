/* GnollHack 4.0 soundset.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

NEARDATA struct player_soundset_definition player_soundsets[MAX_PLAYER_SOUNDSETS + 1] =
{
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	}
};


NEARDATA struct monster_soundset_definition monster_soundsets[MAX_MONSTER_SOUNDSETS + 1] =
{
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	}
};


void
play_dungeon_music(level_ptr, x, y, music_flags)
d_level* level_ptr;
int x, y;
unsigned long music_flags;
{
	if (!level_ptr)
		return;

	int dnum = level_ptr->dnum;
	struct mkroom* room_ptr;

	struct ghsound_music_info musicinfo = { 0 };
	musicinfo.ghsound = get_dungeon_music(dnum);
	play_ghsound_music(musicinfo);
}

void
play_movement_sound(mtmp, x, y, movement_flags)
struct monst* mtmp;
int x, y;
unsigned long movement_flags;
{
	if (!mtmp)
		return;
	boolean isyou = (mtmp == &youmonst);

	struct ghsound_movement_info movementinfo = { 0 };
	enum ghfloor_types floorid = GHFLOOR_STONE_FLOOR; /* Set the appropriate floor here */

	if (isyou)
	{

	}
	else
	{
		enum monster_soundset_types mss = is_female(mtmp->data) ? mtmp->data->female_soundset : mtmp->data->soundset;
		enum ghsound_types soundid = monster_soundsets[mss].movement_sounds[GHMOVEMENT_ON_GROUND];

		if (is_flying(mtmp))
		{
			soundid = monster_soundsets[mss].movement_sounds[GHMOVEMENT_FLYING];
			floorid = GHFLOOR_NO_FLOOR; /* Flying, slithering? */

		}
		else if (is_levitating(mtmp))
		{
			soundid = monster_soundsets[mss].movement_sounds[GHMOVEMENT_LEVITATING];
			floorid = GHFLOOR_NO_FLOOR; /* Flying, slithering? */
		}
		movementinfo.ghsound = soundid;
		movementinfo.ghfloor = floorid; /* Flying, slithering? */
	}

	play_ghsound_movement(movementinfo);
}

/* soundset.c */
/* GnollHack 4.0 soundset.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

NEARDATA struct player_soundset_definition player_soundsets[MAX_PLAYER_SOUNDSETS + 1] =
{
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"Generic",
		0,
		{GHSOUND_PLAYER_FOOTSTEPS_NORMAL, GHSOUND_PLAYER_FOOTSTEPS_NORMAL, GHSOUND_PLAYER_FOOTSTEPS_NORMAL, GHSOUND_PLAYER_FOOTSTEPS_NORMAL, GHSOUND_PLAYER_FOOTSTEPS_NORMAL, GHSOUND_PLAYER_FOOTSTEPS_NORMAL},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	}
};


NEARDATA struct monster_soundset_definition monster_soundsets[MAX_MONSTER_SOUNDSETS + 1] =
{
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"Generic",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"",
		0,
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		"",
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
	struct mkroom* room_ptr = (struct mkroom*)0;
	room_ptr = room_ptr;

	struct ghsound_music_info musicinfo = { 0 };
	musicinfo.ghsound = get_dungeon_music(dnum);
	musicinfo.volume = BACKGROUND_MUSIC_VOLUME;

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
	enum floor_surface_types floorid = FLOOR_SURFACE_CARPET; /* Set the appropriate floor here */
	enum ghsound_types soundid = GHSOUND_NONE;

	if (isyou)
	{
		enum player_soundset_types pss = get_player_soundset();
		soundid = player_soundsets[pss].movement_sounds[MOVEMENT_STYLE_ON_GROUND];
		if (Flying)
		{
			soundid = player_soundsets[pss].movement_sounds[MOVEMENT_STYLE_FLYING];
			floorid = FLOOR_SURFACE_NONE;

		}
		else if (Levitation)
		{
			soundid = player_soundsets[pss].movement_sounds[MOVEMENT_STYLE_LEVITATING];
			floorid = FLOOR_SURFACE_NONE;
		}
	}
	else
	{
		enum monster_soundset_types mss = is_female(mtmp->data) ? mtmp->data->female_soundset : mtmp->data->soundset;
		soundid = monster_soundsets[mss].movement_sounds[MOVEMENT_STYLE_ON_GROUND];

		if (is_flying(mtmp))
		{
			soundid = monster_soundsets[mss].movement_sounds[MOVEMENT_STYLE_FLYING];
			floorid = FLOOR_SURFACE_NONE; 

		}
		else if (is_levitating(mtmp))
		{
			soundid = monster_soundsets[mss].movement_sounds[MOVEMENT_STYLE_LEVITATING];
			floorid = FLOOR_SURFACE_NONE;
		}
	}

	movementinfo.ghsound = soundid;
	movementinfo.floor = floorid; /* Flying, slithering? */
	movementinfo.volume = 1.0f;

	play_ghsound_movement(movementinfo);
}


enum player_soundset_types
get_player_soundset()
{
	return PLAYER_SOUNDSET_GENERAL;
}

void
dosetsoundvolume()
{
	adjust_ghsound_general_volumes();
}


/* soundset.c */
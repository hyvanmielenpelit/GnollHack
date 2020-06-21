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

STATIC_DCL void FDECL(set_hearing_array, (int, int, int));

char hearing_array[COLNO][ROWNO] = { 0 };


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

void
update_hearing_array()
{
	/* Clear array*/
	memset(hearing_array, 0, sizeof(hearing_array));

	/* Can't hear anything */
	if (Deaf)
		return;

	int hear_distance = get_max_hearing_distance();

	/* Fill the array */
	hearing_array[u.ux][u.uy] = 100;

	for (int r = 1; r <= hear_distance; r++)
	{
		int x_min = u.ux - r;
		int x_max = u.ux + r;
		int x_min_adjusted = max(1, x_min);
		int x_max_adjusted = min(COLNO - 1, x_max);

		int y_min = u.uy - r;
		int y_max = u.uy + r;
		int y_min_adjusted = max(0, y_min);
		int y_max_adjusted = min(COLNO - 1, y_max);

		boolean horizontal_min_done = FALSE;
		boolean horizontal_max_done = FALSE;

		/* 1. Horizontal lines, left to right */
		for (int i = 0; i <= 1; i++)
		{
			if (i == 0)
			{
				if(y_min_adjusted != y_min)
					continue;

				horizontal_min_done = TRUE;
			}
			else if (i == 1)
			{
				if(y_max_adjusted != y_max)
					continue;

				horizontal_max_done = TRUE;
			}

			int y = (i == 0 ? y_min_adjusted : y_max_adjusted);

			
			for (int x = x_min_adjusted; x <= x_max_adjusted; x++)
			{
				int prev_hearing = 0;
				int prev_y = (i == 0) ? y + 1 : y - 1;

				if (r == 1)
				{
					prev_hearing = 100;
				}
				else if (prev_y > y_max_adjusted || prev_y < y_min_adjusted)
				{
					//Nothing
				}
				else
				{
					int maximum = 0;

					/* Take maximum from above or below from the previous round */
					for (int prev_x = max(x_min_adjusted + 1, x - 1); prev_x <= min(x_max_adjusted - 1, x + 1); prev_x++)
					{
						maximum = max(maximum, hearing_array[prev_x][prev_y]);
					}
	
					/* Take also previous from the same line */
					if(x > x_min_adjusted)
						maximum = max(maximum, hearing_array[x - 1][y]);

					prev_hearing = maximum;
				}

				if (prev_hearing == 0)
				{
					continue;
					/* Current hearing is 0, too */
				}
				else
				{
					set_hearing_array(x, y, prev_hearing);
				}
			}
		}

		/* Vertical lines, top to bottom */
		for (int i = 0; i <= 1; i++)
		{
			if (i == 0 && x_min_adjusted != x_min)
				continue;
			if (i == 1 && x_max_adjusted != x_max)
				continue;

			int x = (i == 0 ? x_min_adjusted : x_max_adjusted);
			int upper_y_limit = y_max_adjusted - (horizontal_max_done ? 1 : 0);

			for (int y = y_min_adjusted + (horizontal_min_done ? 1 : 0); y <= upper_y_limit; y++)
			{
				int prev_hearing = 0;
				int prev_x = (i == 0) ? x + 1 : x - 1;

				if (r == 1)
				{
					prev_hearing = 100;
				}
				else if (prev_x > x_max_adjusted || prev_x < x_min_adjusted)
				{
					//Nothing
				}
				else
				{
					int maximum = 0;
					for (int prev_y = max(y_min_adjusted + 1, y - 1); prev_y <= min(y_max_adjusted - 1, y + 1); prev_y++)
					{
						maximum = max(maximum, hearing_array[prev_x][prev_y]);
					}

					/* Take also previous from the same line */
					if (y > y_min_adjusted)
						maximum = max(maximum, hearing_array[x][y - 1]);

					prev_hearing = maximum;
				}

				if (prev_hearing == 0)
				{
					continue;
					/* Current hearing is 0, too */
				}
				else
				{
					set_hearing_array(x, y, prev_hearing);
				}
			}
		}

		/* Vertical lines, bottom to top */
		for (int i = 0; i <= 1; i++)
		{
			if (i == 0 && x_max_adjusted != x_max)
				continue;
			if (i == 1 && x_min_adjusted != x_min)
				continue;

			int x = (i == 0 ? x_max_adjusted : x_min_adjusted);

			for (int y = y_max_adjusted - 1 ; y >= y_min_adjusted; y--)
			{
				if(hearing_array[x][y + 1] > 0)
					set_hearing_array(x, y, hearing_array[x][y + 1]);
			}
		}

		/* Horizontal lines, right to left */
		for (int i = 0; i <= 1; i++)
		{
			if (i == 0 && y_max_adjusted != y_max)
				continue;
			if (i == 1 && y_min_adjusted != y_min)
				continue;

			int y = (i == 0 ? y_max_adjusted : y_min_adjusted);

			for (int x = x_max_adjusted - 1; x >= x_min_adjusted; x--)
			{
				if (hearing_array[x + 1][y] > 0)
					set_hearing_array(x, y, hearing_array[x][y + 1]);
			}
		}
	}

}

STATIC_OVL
void set_hearing_array(x, y, prev_hearing)
int x, y, prev_hearing;
{
	struct monst* mtmp;
	if (IS_ROCK(levl[x][y].typ) && !IS_TREE(levl[x][y].typ))
	{
		/* Nothing */
	}
	else if ((mtmp = m_at(x, y)) && is_lightblocker_mappear(mtmp))
	{
		/* Nothing */
	}
	else if (IS_DOOR(levl[x][y].typ) && (levl[x][y].doormask != 0 && (levl[x][y].doormask & (D_NODOOR | D_ISOPEN | D_BROKEN)) == 0))
	{
		int new_hearing = (char)max(0, min(127, prev_hearing / 3 - 5));
		if(new_hearing > hearing_array[x][y])
			hearing_array[x][y] = new_hearing;
	}
	else
	{
		int new_hearing = (char)max(0, min(127, prev_hearing - 8));
		if (new_hearing > hearing_array[x][y])
			hearing_array[x][y] = new_hearing;
	}

}

void
update_ambient_sounds()
{

}

void
update_hearing_array_and_ambient_sounds()
{
	update_hearing_array();
	update_ambient_sounds();
}

int get_max_hearing_distance()
{
	return (u.uswallow ? 1 : Underwater ? 2 : MAX_HEARING_DISTANCE);
}

void
update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(px, py)
int px, py;
{
	int max_hear_dist = get_max_hearing_distance();
	if (abs(u.ux - px) <= max_hear_dist && abs(u.uy - py) <= max_hear_dist)
		update_hearing_array_and_ambient_sounds();
}

void
unblock_vision_and_hearing_at_point(x, y)
int x, y;
{
	unblock_point(x, y);
	update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(x, y);
}

void
block_vision_and_hearing_at_point(x, y)
int x, y;
{
	block_point(x, y);
	update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(x, y);
}

/* soundset.c */
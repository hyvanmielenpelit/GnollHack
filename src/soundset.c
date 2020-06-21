/* GnollHack 4.0 soundset.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h" /* for checking save modes */

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

//char hearing_array[COLNO][ROWNO] = { 0 };


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




/*
 * Ambient sound sources.
 *
 * This implementation minimizes memory at the expense of extra
 * recalculations.
 *
 * Sound sources are "things" that have a physical position and volume.
 * They have a type, which gives us information about them.  Currently
 * they are only attached to objects, monsters, and locations.  Note:  the
 * polymorphed-player handling assumes that both youmonst.m_id and
 * youmonst.mx will always remain 0.
 *
 * Sound sources, like timers, either follow game play (RANGE_GLOBAL) or
 * stay on a level (RANGE_LEVEL).  Sound sources are unique by their
 * (type, id) pair.  For sound sources attached to objects, this id
 * is a pointer to the object.
 *
 * The structure of the save/restore mechanism is amazingly similar to
 * the timer save/restore.  This is because they both have the same
 * principals of having pointers into objects that must be recalculated
 * across saves and restores.
 */

 /* flags */
#define SSF_SHOW 0x1        /* display the sound source */
#define SSF_NEEDS_FIXUP 0x2 /* need oid fixup */
#define SSF_SILENCE_SOURCE 0x4 /* emits silence rather than sound */

static sound_source* sound_base = 0;

STATIC_DCL void FDECL(write_soundsource, (int, sound_source*));
STATIC_DCL int FDECL(maybe_write_soundsource, (int, int, BOOLEAN_P));


/* Create a new sound source.  */
void
new_sound_source(x, y, volume, type, id)
xchar x, y;
int volume, type;
anything* id;
{
    sound_source* ss;
    int absvolume = abs(volume);

    if (absvolume > 100 || absvolume < 1) 
    {
        impossible("new_sound_source:  illegal volume %d", volume);
        return;
    }

    ss = (sound_source*)alloc(sizeof(sound_source));

    ss->next = sound_base;
    ss->x = x;
    ss->y = y;
    ss->volume = absvolume;
    ss->type = type;
    ss->id = *id;
    ss->flags = volume < 0 ? SSF_SILENCE_SOURCE : 0;
    sound_base = ss;

    hearing_full_recalc = 1; /* make the source show up */
}

/*
 * Delete a sound source. This assumes only one sound source is attached
 * to an object at a time.
 */
void
del_sound_source(type, id)
int type;
anything* id;
{
    sound_source* curr, * prev;
    anything tmp_id;

    tmp_id = zeroany;
    /* need to be prepared for dealing a with sound source which
       has only been partially restored during a level change
       (in particular: chameleon vs prot. from shape changers) */
    switch (type)
    {
    case SOUNDSOURCE_OBJECT:
        tmp_id.a_uint = id->a_obj->o_id;
        break;
    case SOUNDSOURCE_MONSTER:
        tmp_id.a_uint = id->a_monst->m_id;
        break;
    case SOUNDSOURCE_LOCATION:
        tmp_id = zeroany;
        tmp_id.a_coord.x = id->a_coord.x;
        tmp_id.a_coord.y = id->a_coord.y;
        break;
    default:
        tmp_id.a_uint = 0;
        break;
    }

    for (prev = 0, curr = sound_base; curr; prev = curr, curr = curr->next)
    {
        if (curr->type != type)
            continue;

        if ((type == SOUNDSOURCE_LOCATION && curr->id.a_coord.x == tmp_id.a_coord.x && curr->id.a_coord.y == tmp_id.a_coord.y)
            || (type != SOUNDSOURCE_LOCATION && curr->id.a_obj == ((curr->flags & SSF_NEEDS_FIXUP) ? tmp_id.a_obj : id->a_obj))
            )
        {
            if (prev)
                prev->next = curr->next;
            else
                sound_base = curr->next;

            free((genericptr_t)curr);
            hearing_full_recalc = 1;
            return;
        }
    }
    impossible("del_sound_source: not found type=%d, id=%s", type,
        fmt_ptr((genericptr_t)id->a_obj));
}

/* Mark locations that are temporarily lit via mobile sound sources. */
void
do_sound_sources(cs_rows)
char** cs_rows;
{

#if 0
    int x, y, min_x, max_x, max_y, offset;
    char* limits;
    short at_hero_range = 0;
    sound_source* ss;
    char* row;

    for (ss = sound_base; ss; ss = ss->next)
    {
        ss->flags &= ~SSF_SHOW;

        /*
         * Check for moved sound sources.  It may be possible to
         * save some effort if an object has not moved, but not in
         * the current setup -- we need to recalculate for every
         * vision recalc.
         */
        if (ss->type == SOUNDSOURCE_OBJECT)
        {
            if (get_obj_location(ss->id.a_obj, &ss->x, &ss->y, 0))
                ss->flags |= SSF_SHOW;
        }
        else if (ss->type == SOUNDSOURCE_MONSTER)
        {
            if (get_mon_location(ss->id.a_monst, &ss->x, &ss->y, 0))
                ss->flags |= SSF_SHOW;
        }
        else if (ss->type == SOUNDSOURCE_LOCATION)
        {
            ss->x = ss->id.a_coord.x;
            ss->y = ss->id.a_coord.y;
            ss->flags |= SSF_SHOW;
        }

        /* minor optimization: don't bother with duplicate sound sources */
        /* at hero */
        if (ss->x == u.ux && ss->y == u.uy)
        {
            if (at_hero_range >= ss->volume)
                ss->flags &= ~SSF_SHOW;
            else
                at_hero_range = ss->volume;
        }

        if (ss->flags & SSF_SHOW) 
        {
            /*
             * Walk the points in the circle and see if they are
             * visible from the center.  If so, mark'em.
             *
             * Kevin's tests indicated that doing this brute-force
             * method is faster for radius <= 3 (or so).
             */
            limits = circle_ptr(ss->volume);
            if ((max_y = (ss->y + ss->volume)) >= ROWNO)
                max_y = ROWNO - 1;
            if ((y = (ss->y - ss->volume)) < 0)
                y = 0;
            for (; y <= max_y; y++) 
            {
                row = cs_rows[y];
                offset = limits[abs(y - ss->y)];
                if ((min_x = (ss->x - offset)) < 0)
                    min_x = 0;
                if ((max_x = (ss->x + offset)) >= COLNO)
                    max_x = COLNO - 1;

                if (ss->x == u.ux && ss->y == u.uy) 
                {
                    /*
                     * If the sound source is located at the hero, then
                     * we can use the COULD_SEE bits already calculated
                     * by the vision system.  More importantly than
                     * this optimization, is that it allows the vision
                     * system to correct problems with clear_path().
                     * The function clear_path() is a simple LOS
                     * path checker that doesn't go out of its way
                     * make things look "correct".  The vision system
                     * does this.
                     */
                    for (x = min_x; x <= max_x; x++)
                        if (row[x] & COULD_SEE)
                            row[x] |= ((ss->flags & SSF_SILENCE_SOURCE) ? TEMP_MAGICAL_DARKNESS : TEMP_LIT);
                }
                else 
                {
                    for (x = min_x; x <= max_x; x++)
                        if ((ss->x == x && ss->y == y)
                            || clear_path((int)ss->x, (int)ss->y, x, y))
                            row[x] |= ((ss->flags & SSF_SILENCE_SOURCE) ? TEMP_MAGICAL_DARKNESS : TEMP_LIT);
                }
            }
        }
    }
#endif
}


/* Save all sound sources of the given volume. */
void
save_sound_sources(fd, mode, volume)
int fd, mode, volume;
{
    int count, actual, is_global;
    sound_source** prev, * curr;

    if (perform_bwrite(mode))
    {
        count = maybe_write_soundsource(fd, volume, FALSE);
        bwrite(fd, (genericptr_t)&count, sizeof count);
        actual = maybe_write_soundsource(fd, volume, TRUE);
        if (actual != count)
        {
            panic("counted %d sound sources, wrote %d! [volume=%d]", count,
                actual, volume);
            return;
        }
    }

    if (release_data(mode)) 
    {
        for (prev = &sound_base; (curr = *prev) != 0;) 
        {
            if (!curr->id.a_monst)
            {
                impossible("save_sound_sources: no id! [volume=%d]", volume);
                is_global = 0;
            }
            else
                switch (curr->type) 
                {
                case SOUNDSOURCE_OBJECT:
                    is_global = !obj_is_local(curr->id.a_obj);
                    break;
                case SOUNDSOURCE_MONSTER:
                    is_global = !mon_is_local_mx(curr->id.a_monst);
                    break;
                case SOUNDSOURCE_LOCATION:
                    is_global = 0; /* always local by definition */
                    break;
                default:
                    is_global = 0;
                    impossible("save_sound_sources: bad type (%d) [volume=%d]",
                        curr->type, volume);
                    break;
                }
            /* if global and not doing local, or vice versa, remove it */
            if (is_global ^ (volume == RANGE_LEVEL)) 
            {
                *prev = curr->next;
                free((genericptr_t)curr);
            }
            else
            {
                prev = &(*prev)->next;
            }
        }
    }
}

/*
 * Pull in the structures from disk, but don't recalculate the object
 * pointers.
 */
void
restore_sound_sources(fd)
int fd;
{
    int count;
    sound_source* ss;

    /* restore elements */
    mread(fd, (genericptr_t)&count, sizeof count);

    while (count-- > 0) {
        ss = (sound_source*)alloc(sizeof(sound_source));
        mread(fd, (genericptr_t)ss, sizeof(sound_source));
        ss->next = sound_base;
        sound_base = ss;
    }
}

/* to support '#stats' wizard-mode command */
void
sound_stats(hdrfmt, hdrbuf, count, size)
const char* hdrfmt;
char* hdrbuf;
long* count;
size_t* size;
{
    sound_source* ss;

    Sprintf(hdrbuf, hdrfmt, sizeof(sound_source));
    *count = *size = 0L;
    for (ss = sound_base; ss; ss = ss->next) {
        ++* count;
        *size += sizeof * ss;
    }
}

/* Relink all sounds that are so marked. */
void
relink_sound_sources(ghostly)
boolean ghostly;
{
    char which;
    unsigned nid;
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (ss->flags & SSF_NEEDS_FIXUP)
        {
            if (ss->type == SOUNDSOURCE_OBJECT || ss->type == SOUNDSOURCE_MONSTER || ss->type == SOUNDSOURCE_LOCATION)
            {
                if (ghostly && ss->type != SOUNDSOURCE_LOCATION)
                {
                    if (!lookup_id_mapping(ss->id.a_uint, &nid))
                        impossible("relink_sound_sources: no id mapping");
                }
                else
                    nid = ss->id.a_uint;

                if (ss->type == SOUNDSOURCE_OBJECT)
                {
                    which = 'o';
                    ss->id.a_obj = find_oid(nid);
                }
                else if (ss->type == SOUNDSOURCE_MONSTER)
                {
                    which = 'm';
                    ss->id.a_monst = find_mid(nid, FM_EVERYWHERE);
                }
                else if (ss->type == SOUNDSOURCE_LOCATION)
                {
                    which = 'l';
                    ss->id = zeroany;
                    ss->id.a_coord.x = ss->x;
                    ss->id.a_coord.y = ss->y;
                }

                if (!ss->id.a_monst)
                    impossible("relink_sound_sources: cant find %c_id %d", which, nid);
            }
            else
                impossible("relink_sound_sources: bad type (%d)", ss->type);

            ss->flags &= ~SSF_NEEDS_FIXUP;
        }
    }
}

/*
 * Part of the sound source save routine.  Count up the number of sound
 * sources that would be written.  If write_it is true, actually write
 * the sound source out.
 */
STATIC_OVL int
maybe_write_soundsource(fd, volume, write_it)
int fd, volume;
boolean write_it;
{
    int count = 0, is_global;
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (!ss->id.a_monst)
        {
            impossible("maybe_write_soundsource: no id! [volume=%d]", volume);
            continue;
        }

        switch (ss->type)
        {
        case SOUNDSOURCE_OBJECT:
            is_global = !obj_is_local(ss->id.a_obj);
            break;
        case SOUNDSOURCE_MONSTER:
            is_global = !mon_is_local_mx(ss->id.a_monst);
            break;
        case SOUNDSOURCE_LOCATION:
            is_global = 0; /* always local */
            break;
        default:
            is_global = 0;
            impossible("maybe_write_soundsource: bad type (%d) [volume=%d]", ss->type, volume);
            break;
        }

        /* if global and not doing local, or vice versa, count it */
        if (is_global ^ (volume == RANGE_LEVEL))
        {
            count++;
            if (write_it)
                write_soundsource(fd, ss);
        }
    }

    return count;
}

void
sound_sources_sanity_check()
{
    sound_source* ss;
    struct monst* mtmp;
    struct obj* otmp;
    unsigned int auint = 0;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (!ss->id.a_monst)
        {
            panic("insane sound source: no id!");
            return;
        }
        if (ss->type == SOUNDSOURCE_OBJECT)
        {
            otmp = (struct obj*)ss->id.a_obj;
            if (otmp)
                auint = otmp->o_id;

            if (find_oid(auint) != otmp)
            {
                panic("insane sound source: can't find obj #%u!", auint);
                return;
            }
        }
        else if (ss->type == SOUNDSOURCE_MONSTER)
        {
            mtmp = (struct monst*)ss->id.a_monst;
            if (mtmp)
                auint = mtmp->m_id;

            if (find_mid(auint, FM_EVERYWHERE) != mtmp)
            {
                panic("insane sound source: can't find mon #%u!", auint);
                return;
            }
        }
        else if (ss->type == SOUNDSOURCE_LOCATION)
        {
            coord c = ss->id.a_coord;
            if (!isok(c.x, c.y))
            {
                panic("insane sound source: invalid location coordinates (%d, %d)!", c.x, c.y);
                return;
            }
        }
        else
        {
            panic("insane sound source: bad ss type %d", ss->type);
            return;
        }
    }
}

/* Write a sound source structure to disk. */
STATIC_OVL void
write_soundsource(fd, ss)
int fd;
sound_source* ss;
{
    anything arg_save;
    struct obj* otmp;
    struct monst* mtmp;

    if (ss->type == SOUNDSOURCE_OBJECT || ss->type == SOUNDSOURCE_MONSTER || ss->type == SOUNDSOURCE_LOCATION)
    {
        if (ss->flags & SSF_NEEDS_FIXUP)
        {
            bwrite(fd, (genericptr_t)ss, sizeof(sound_source));
        }
        else
        {
            /* replace object pointer with id for write, then put back */
            arg_save = ss->id;
            if (ss->type == SOUNDSOURCE_OBJECT)
            {
                otmp = ss->id.a_obj;
                ss->id = zeroany;
                ss->id.a_uint = otmp->o_id;
                if (find_oid((unsigned)ss->id.a_uint) != otmp)
                    impossible("write_soundsource: can't find obj #%u!",
                        ss->id.a_uint);
            }
            else if (ss->type == SOUNDSOURCE_MONSTER)
            { /* ss->type == SOUNDSOURCE_MONSTER */
                mtmp = (struct monst*)ss->id.a_monst;
                ss->id = zeroany;
                ss->id.a_uint = mtmp->m_id;
                if (find_mid((unsigned)ss->id.a_uint, FM_EVERYWHERE) != mtmp)
                    impossible("write_soundsource: can't find mon #%u!",
                        ss->id.a_uint);
            }
            else if (ss->type == SOUNDSOURCE_LOCATION)
            {
                /* No need to do anything, coord can be written to disk as is */
            }

            ss->flags |= SSF_NEEDS_FIXUP;
            bwrite(fd, (genericptr_t)ss, sizeof(sound_source));
            ss->id = arg_save;
            ss->flags &= ~SSF_NEEDS_FIXUP;
        }
    }
    else
    {
        impossible("write_soundsource: bad type (%d)", ss->type);
    }
}

/* Change sound source's ID from src to dest. */
void
obj_move_sound_source(src, dest)
struct obj* src, * dest;
{
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == src)
            ss->id.a_obj = dest;
    }

    //src->lamplit = 0;
    //dest->lamplit = 1;
}

/* return true if there exist any sound sources */
boolean
any_sound_source()
{
    return (boolean)(sound_base != (sound_source*)0);
}

/*
 * Snuff an object sound source if at (x,y).  This currently works
 * only for burning sound sources.
 */
void
snuff_sound_source(x, y)
int x, y;
{
    sound_source* ss;
    struct obj* obj;

    for (ss = sound_base; ss; ss = ss->next)
        /*
         * Is this position check valid??? Can I assume that the positions
         * will always be correct because the objects would have been
         * updated with the last vision update?  [Is that recent enough???]
         */
        if (ss->type == SOUNDSOURCE_OBJECT && ss->x == x && ss->y == y) {
            obj = ss->id.a_obj;
            if (1) //obj_is_burning(obj)) 
            {
                del_sound_source(LS_OBJECT, obj_to_any(obj));
                //end_burn(obj, obj->otyp != MAGIC_LAMP && obj->otyp != MAGIC_CANDLE);
                /*
                 * The current ss element has just been removed (and
                 * ss->next is now invalid).  Return assuming that there
                 * is only one sound source attached to each object.
                 */
                return;
            }
        }
}


/* copy the sound source(s) attached to src, and attach it/them to dest */
void
obj_split_sound_source(src, dest)
struct obj* src, * dest;
{
    sound_source* ss, * new_ss;

    for (ss = sound_base; ss; ss = ss->next)
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == src) 
        {
            /*
             * Insert the new source at beginning of list.  This will
             * never interfere us walking down the list - we are already
             * past the insertion point.
             */
            new_ss = (sound_source*)alloc(sizeof(sound_source));
            *new_ss = *ss;
            new_ss->id.a_obj = dest;
            new_ss->next = sound_base;
            sound_base = new_ss;
            //dest->lamplit = 1; /* now an active sound source */
        }
}

/* sound source `src' has been folded into sound source `dest';
   used for merging lit candles and adding candle(s) to lit candelabrum */
void
obj_merge_sound_sources(src, dest)
struct obj* src, * dest;
{
    sound_source* ss;

    /* src == dest implies adding to candelabrum */
    if (src != dest)
        del_sound_source(LS_OBJECT, obj_to_any(src));
    //end_burn(src, TRUE); /* extinguish candles */


    for (ss = sound_base; ss; ss = ss->next)
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == dest) 
        {
            //ss->volume = candle_sound_range(dest);
            hearing_full_recalc = 1; /* in case volume changed */
            break;
        }
}

/* sound source `obj' is being made brighter or dimmer */
void
obj_adjust_sound_volume(obj, new_volume)
struct obj* obj;
int new_volume;
{
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == obj) {
            if (new_volume != ss->volume)
                hearing_full_recalc = 1;
            ss->volume = new_volume;
            return;
        }
    impossible("obj_adjust_sound_volume: can't find %s", xname(obj));
}


/* soundset.c */
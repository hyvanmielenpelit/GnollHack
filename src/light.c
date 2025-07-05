/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    light.c    $NHDT-Date: 1446191876 2015/10/30 07:57:56 $  $NHDT-Branch: master $:$NHDT-Revision: 1.28 $ */
/* Copyright (c) Dean Luick, 1994                                       */
/* GnollHack may be freely redistributed.  See license for details.       */

#include "hack.h"
#include "lev.h" /* for checking save modes */

/*
 * Mobile light sources.
 *
 * This implementation minimizes memory at the expense of extra
 * recalculations.
 *
 * Light sources are "things" that have a physical position and range.
 * They have a type, which gives us information about them.  Currently
 * they are only attached to objects and monsters.  Note well:  the
 * polymorphed-player handling assumes that both youmonst.m_id and
 * youmonst.mx will always remain 0.
 *
 * Light sources, like timers, either follow game play (RANGE_GLOBAL) or
 * stay on a level (RANGE_LEVEL).  Light sources are unique by their
 * (type, id) pair.  For light sources attached to objects, this id
 * is a pointer to the object.
 *
 * The major working function is do_light_sources(). It is called
 * when the vision system is recreating its "could see" array.  Here
 * we add a flag (TEMP_LIT) to the array for all locations that are lit
 * via a light source.  The bad part of this is that we have to
 * re-calculate the LOS of each light source every time the vision
 * system runs.  Even if the light sources and any topology (vision blocking
 * positions) have not changed.  The good part is that no extra memory
 * is used, plus we don't have to figure out how far the sources have moved,
 * or if the topology has changed.
 *
 * The structure of the save/restore mechanism is amazingly similar to
 * the timer save/restore.  This is because they both have the same
 * principals of having pointers into objects that must be recalculated
 * across saves and restores.
 */

/* flags */
#define LSF_SHOW 0x1        /* display the light source */
#define LSF_NEEDS_FIXUP 0x2 /* need oid fixup */
#define LSF_DARKNESS_SOURCE 0x4 /* emits darkness rather than light */

STATIC_VAR light_source *light_base = 0;

STATIC_DCL void FDECL(write_ls, (int, light_source *));
STATIC_DCL int FDECL(maybe_write_ls, (int, int, BOOLEAN_P));

/* imported from vision.c, for small circles */
extern char circle_data[];
extern char circle_start[];

/* Create a new light source.  */
void
new_light_source(x, y, range, type, id, sidedness)
xchar x, y;
int range, type, sidedness;
anything *id;
{
    light_source *ls;
    int absrange = abs(range);

    if (absrange > MAX_RADIUS || absrange < 1) 
    {
        impossible("new_light_source:  illegal range %d", range);
        if (absrange > MAX_RADIUS)
            absrange = MAX_RADIUS;
        if (absrange < 1)
            absrange = 1;
        range = sgn(range) * absrange;
        //return;
    }

    ls = (light_source *) alloc(sizeof(light_source));

    ls->next = light_base;
    ls->x = x;
    ls->y = y;
    ls->range = (short)absrange;
    ls->type = (short)type;
    ls->id = *id;
    ls->flags = range < 0 ? LSF_DARKNESS_SOURCE : 0;
    ls->sidedness = (short)sidedness;
    light_base = ls;

    vision_full_recalc = 1; /* make the source show up */
}

/*
 * Delete a light source. This assumes only one light source is attached
 * to an object at a time.
 */
void
del_light_source(type, id)
int type;
anything *id;
{
    light_source *curr, *prev;
    anything tmp_id;

    tmp_id = zeroany;
    /* need to be prepared for dealing a with light source which
       has only been partially restored during a level change
       (in particular: chameleon vs prot. from shape changers) */
    switch (type) {
    case LS_OBJECT:
        tmp_id.a_uint = id->a_obj->o_id;
        break;
    case LS_MONSTER:
        tmp_id.a_uint = id->a_monst->m_id;
        break;
    case LS_LOCATION:
        tmp_id = zeroany;
        tmp_id.a_coord.x = id->a_coord.x;
        tmp_id.a_coord.y = id->a_coord.y;
        break;
    default:
        tmp_id.a_uint = 0;
        break;
    }

    for (prev = 0, curr = light_base; curr; prev = curr, curr = curr->next) 
    {
        if (curr->type != type)
            continue;

        if ((type == LS_LOCATION && curr->id.a_coord.x == tmp_id.a_coord.x && curr->id.a_coord.y == tmp_id.a_coord.y)
            || (type != LS_LOCATION && curr->id.a_obj == ((curr->flags & LSF_NEEDS_FIXUP) ? tmp_id.a_obj : id->a_obj))
           )
        {
            if (prev)
                prev->next = curr->next;
            else
                light_base = curr->next;

            free((genericptr_t) curr);
            vision_full_recalc = 1;
            return;
        }
    }
    impossible("del_light_source: not found type=%d, id=%s, otyp=%d, where=%d, ox=%d, oy=%d, debug3=%s, debug4=%s", type,
               fmt_ptr((genericptr_t) id->a_obj), id->a_obj->otyp, id->a_obj->where, id->a_obj->ox, id->a_obj->oy, debug_buf_3, debug_buf_4);
}

/* Mark locations that are temporarily lit via mobile light sources. */
void
do_light_sources(cs_rows)
char **cs_rows;
{
    int x, y, min_x, max_x, max_y, offset;
    char *limits;
    short at_hero_range = 0;
    light_source *ls;
    char *row;

    for (ls = light_base; ls; ls = ls->next) {
        ls->flags &= ~LSF_SHOW;

        /*
         * Check for moved light sources.  It may be possible to
         * save some effort if an object has not moved, but not in
         * the current setup -- we need to recalculate for every
         * vision recalc.
         */
        if (ls->type == LS_OBJECT)
        {
            if (get_obj_location(ls->id.a_obj, &ls->x, &ls->y, 0))
                ls->flags |= LSF_SHOW;
        }
        else if (ls->type == LS_MONSTER) 
        {
            if (get_mon_location(ls->id.a_monst, &ls->x, &ls->y, 0))
                ls->flags |= LSF_SHOW;
        }
        else if (ls->type == LS_LOCATION)
        {
            ls->x = ls->id.a_coord.x;
            ls->y = ls->id.a_coord.y;
            ls->flags |= LSF_SHOW;
        }

        /* minor optimization: don't bother with duplicate light sources */
        /* at hero */
        if (ls->x == u.ux && ls->y == u.uy) {
            if (at_hero_range >= ls->range)
                ls->flags &= ~LSF_SHOW;
            else
                at_hero_range = ls->range;
        }

        if (ls->flags & LSF_SHOW) {
            /*
             * Walk the points in the circle and see if they are
             * visible from the center.  If so, mark'em.
             *
             * Kevin's tests indicated that doing this brute-force
             * method is faster for radius <= 3 (or so).
             */
            limits = circle_ptr(ls->range);
            if ((max_y = (ls->y + ls->range)) >= ROWNO)
                max_y = ROWNO - 1;
            if ((y = (ls->y - ls->range)) < 0)
                y = 0;
            for (; y <= max_y; y++) {
                row = cs_rows[y];
                offset = limits[abs(y - ls->y)];
                if ((min_x = (ls->x - offset)) < 0)
                    min_x = 0;
                if ((max_x = (ls->x + offset)) >= COLNO)
                    max_x = COLNO - 1;

                if (ls->x == u.ux && ls->y == u.uy) {
                    /*
                     * If the light source is located at the hero, then
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
                            row[x] |= ((ls->flags & LSF_DARKNESS_SOURCE) ? TEMP_MAGICAL_DARKNESS : TEMP_LIT);
                } else {
                    int sidedness_x = ls->sidedness == 2 ? 1 : ls->sidedness == 3 ? -1 : 0;
                    int sidedness_y = ls->sidedness == 1 ? 1 : ls->sidedness == 4 ? -1 : 0;
                    for (x = min_x; x <= max_x; x++)
                        if ((ls->x == x && ls->y == y)
                            || clear_path((int) ls->x + sidedness_x, (int) ls->y + sidedness_y, x, y))
                            row[x] |= ((ls->flags & LSF_DARKNESS_SOURCE) ? TEMP_MAGICAL_DARKNESS : TEMP_LIT);
                }
            }
        }
    }
}


struct monst*
find_mid_ew(nid) //everywhere
unsigned nid;
{
    struct monst* mtmp;

    if (!nid)
        return &youmonst;

    mtmp = find_mid(nid, FM_FMON);
    if (mtmp)
        return mtmp;
    else
        mtmp = find_mid(nid, FM_MIGRATE);

    if (mtmp)
        return mtmp;
    else
        mtmp = find_mid(nid, FM_MYDOGS);

    if (mtmp)
        return mtmp;
    else
        return (struct monst*) 0;
}


struct monst *
find_mid(nid, fmflags)
unsigned nid;
unsigned fmflags;
{
    struct monst *mtmp;

    if (!nid)
        return &youmonst;
    if (fmflags & FM_FMON)
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
            if (!DEADMONSTER(mtmp) && mtmp->m_id == nid)
                return mtmp;
    if (fmflags & FM_MIGRATE)
        for (mtmp = migrating_mons; mtmp; mtmp = mtmp->nmon)
            if (mtmp->m_id == nid)
                return mtmp;
    if (fmflags & FM_MYDOGS)
        for (mtmp = mydogs; mtmp; mtmp = mtmp->nmon)
            if (mtmp->m_id == nid)
                return mtmp;
    return (struct monst *) 0;
}

/* Save all light sources of the given range. */
void
save_light_sources(fd, mode, range)
int fd, mode, range;
{
    int count, actual, is_global;
    light_source **prev, *curr;

    if (perform_bwrite(mode)) {
        count = maybe_write_ls(fd, range, FALSE);
        bwrite(fd, (genericptr_t) &count, sizeof count);
        actual = maybe_write_ls(fd, range, TRUE);
        if (actual != count)
        {
            panic("counted %d light sources, wrote %d! [range=%d]", count,
                actual, range);
            return;
        }
    }

    if (release_data(mode)) {
        for (prev = &light_base; (curr = *prev) != 0;) {
            if (!curr->id.a_monst) {
                impossible("save_light_sources: no id! [range=%d]", range);
                is_global = 0;
            } else
                switch (curr->type) {
                case LS_OBJECT:
                    is_global = !obj_is_local(curr->id.a_obj);
                    break;
                case LS_MONSTER:
                    is_global = !mon_is_local_mx(curr->id.a_monst);
                    break;
                case LS_LOCATION:
                    is_global = 0; /* always local by definition */
                    break;
                default:
                    is_global = 0;
                    impossible("save_light_sources: bad type (%d) [range=%d]",
                               curr->type, range);
                    break;
                }
            /* if global and not doing local, or vice versa, remove it */
            if (is_global ^ (range == RANGE_LEVEL)) {
                *prev = curr->next;
                free((genericptr_t) curr);
            } else {
                prev = &(*prev)->next;
            }
        }
    }
}

void
reset_light_sources(VOID_ARGS)
{
    light_source* curr, *next_source;

    for (curr = light_base; curr; curr = next_source)
    {
        next_source = curr->next;
        free((genericptr_t)curr);
    }

    light_base = 0;
}

/*
 * Pull in the structures from disk, but don't recalculate the object
 * pointers.
 */
void
restore_light_sources(fd)
int fd;
{
    int count;
    light_source *ls;
    Strcpy(debug_buf_4, "restore_light_sources");

    /* restore elements */
    mread(fd, (genericptr_t) &count, sizeof count);

    while (count-- > 0) {
        ls = (light_source *) alloc(sizeof(light_source));
        mread(fd, (genericptr_t) ls, sizeof(light_source));
        ls->next = light_base;
        light_base = ls;
    }
}

/* to support '#stats' wizard-mode command */
void
light_stats(hdrfmt, hdrbuf, count, size)
const char *hdrfmt;
char *hdrbuf;
int64_t* count;
size_t *size;
{
    light_source *ls;

    Sprintf(hdrbuf, hdrfmt, sizeof (light_source));
    *count = *size = 0L;
    for (ls = light_base; ls; ls = ls->next) {
        ++*count;
        *size += sizeof *ls;
    }
}

/* Relink all lights that are so marked. */
void
relink_light_sources(ghostly)
boolean ghostly;
{
    char which;
    unsigned nid;
    light_source *ls;

    for (ls = light_base; ls; ls = ls->next)
    {
        if (ls->flags & LSF_NEEDS_FIXUP)
        {
            if (ls->type == LS_OBJECT || ls->type == LS_MONSTER || ls->type == LS_LOCATION)
            {
                if (ghostly && ls->type != LS_LOCATION)
                {
                    if (!lookup_id_mapping(ls->id.a_uint, &nid))
                        impossible("relink_light_sources: no id mapping");
                }
                else
                    nid = ls->id.a_uint;

                if (ls->type == LS_OBJECT)
                {
                    which = 'o';
                    ls->id.a_obj = find_oid(nid);
                } 
                else if (ls->type == LS_MONSTER)
                {
                    which = 'm';
                    ls->id.a_monst = find_mid(nid, FM_EVERYWHERE);
                }
                else if (ls->type == LS_LOCATION)
                {
                    which = 'l';
                    ls->id = zeroany;
                    ls->id.a_coord.x = ls->x;
                    ls->id.a_coord.y = ls->y;
                }

                if (!ls->id.a_monst)
                    impossible("relink_light_sources: cant find %c_id %d", which, nid);
            }
            else
                impossible("relink_light_sources: bad type (%d)", ls->type);

            ls->flags &= ~LSF_NEEDS_FIXUP;
        }
    }
}

/*
 * Part of the light source save routine.  Count up the number of light
 * sources that would be written.  If write_it is true, actually write
 * the light source out.
 */
STATIC_OVL int
maybe_write_ls(fd, range, write_it)
int fd, range;
boolean write_it;
{
    int count = 0, is_global;
    light_source *ls;

    for (ls = light_base; ls; ls = ls->next) 
    {
        if (!ls->id.a_monst) 
        {
            impossible("maybe_write_ls: no id! [range=%d]", range);
            continue;
        }

        switch (ls->type)
        {
        case LS_OBJECT:
            is_global = !obj_is_local(ls->id.a_obj);
            break;
        case LS_MONSTER:
            is_global = !mon_is_local_mx(ls->id.a_monst);
            break;
        case LS_LOCATION:
            is_global = 0; /* always local */
            break;
        default:
            is_global = 0;
            impossible("maybe_write_ls: bad type (%d) [range=%d]", ls->type, range);
            break;
        }
        
        /* if global and not doing local, or vice versa, count it */
        if (is_global ^ (range == RANGE_LEVEL)) 
        {
            count++;
            if (write_it)
                write_ls(fd, ls);
        }
    }

    return count;
}

void
light_sources_sanity_check()
{
    light_source *ls;
    struct monst *mtmp;
    struct obj *otmp;
    unsigned int auint = 0;

    for (ls = light_base; ls; ls = ls->next)
    {
        if (!ls->id.a_monst)
        {
            panic("insane light source: no id!");
            return;
        }
        if (ls->type == LS_OBJECT)
        {
            otmp = (struct obj *) ls->id.a_obj;
            if(otmp)
                auint = otmp->o_id;

            if (find_oid(auint) != otmp)
            {
                panic("insane light source: can't find obj #%u!", auint);
                return;
            }
        }
        else if (ls->type == LS_MONSTER) 
        {
            mtmp = (struct monst *) ls->id.a_monst;
            if(mtmp)
                auint = mtmp->m_id;

            if (find_mid(auint, FM_EVERYWHERE) != mtmp)
            {
                panic("insane light source: can't find mon #%u!", auint);
                return;
            }
        }
        else if (ls->type == LS_LOCATION)
        {
            coord c = ls->id.a_coord;
            if (!isok(c.x, c.y))
            {
                panic("insane light source: invalid location coordinates (%d, %d)!", c.x, c.y);
                return;
            }
        }
        else
        {
            panic("insane light source: bad ls type %d", ls->type);
            return;
        }
    }
}

/* Write a light source structure to disk. */
STATIC_OVL void
write_ls(fd, ls)
int fd;
light_source *ls;
{
    anything arg_save;
    struct obj *otmp;
    struct monst *mtmp;

    if (ls->type == LS_OBJECT || ls->type == LS_MONSTER || ls->type == LS_LOCATION)
    {
        if (ls->flags & LSF_NEEDS_FIXUP) 
        {
            bwrite(fd, (genericptr_t) ls, sizeof(light_source));
        } 
        else 
        {
            /* replace object pointer with id for write, then put back */
            arg_save = ls->id;
            if (ls->type == LS_OBJECT) 
            {
                otmp = ls->id.a_obj;
                ls->id = zeroany;
                ls->id.a_uint = otmp->o_id;
                if (find_oid((unsigned) ls->id.a_uint) != otmp)
                    impossible("write_ls: can't find obj #%u!",
                               ls->id.a_uint);
            } 
            else if(ls->type == LS_MONSTER)
            { /* ls->type == LS_MONSTER */
                mtmp = (struct monst *) ls->id.a_monst;
                ls->id = zeroany;
                ls->id.a_uint = mtmp->m_id;
                if (find_mid((unsigned) ls->id.a_uint, FM_EVERYWHERE) != mtmp)
                    impossible("write_ls: can't find mon #%u!",
                               ls->id.a_uint);
            }
            else if (ls->type == LS_LOCATION)
            {
                /* No need to do anything, coord can be written to disk as is */
            }

            ls->flags |= LSF_NEEDS_FIXUP;
            bwrite(fd, (genericptr_t) ls, sizeof(light_source));
            ls->id = arg_save;
            ls->flags &= ~LSF_NEEDS_FIXUP;
        }
    } 
    else 
    {
        impossible("write_ls: bad type (%d)", ls->type);
    }
}

/* return true if there exist any light sources */
boolean
any_light_source()
{
    return (boolean) (light_base != (light_source *) 0);
}

/*
 * Snuff an object light source if at (x,y).  This currently works
 * only for burning light sources.
 */
void
snuff_light_source(x, y)
int x, y;
{
    light_source* ls;
    struct obj* obj;

    for (ls = light_base; ls; ls = ls->next)
        /*
         * Is this position check valid??? Can I assume that the positions
         * will always be correct because the objects would have been
         * updated with the last vision update?  [Is that recent enough???]
         */
        if (ls->type == LS_OBJECT && ls->x == x && ls->y == y)
        {
            obj = ls->id.a_obj;
            if (obj_is_burning(obj)) {
                /* The only way to snuff Sunsword is to unwield it.  Darkness
                 * scrolls won't affect it.  (If we got here because it was
                 * dropped or thrown inside a monster, this won't matter
                 * anyway because it will go out when dropped.)
                 */
                if (artifact_light(obj))
                    continue;
                Strcpy(debug_buf_3, "snuff_light_source");
                end_burn(obj, obj->otyp != MAGIC_LAMP && obj->otyp != MAGIC_CANDLE);
                /*
                 * The current ls element has just been removed (and
                 * ls->next is now invalid).  Return assuming that there
                 * is only one light source attached to each object.
                 */
                return;
            }
        }
        else if (ls->type == LS_LOCATION && ls->x == x && ls->y == y)
        {
            if (levl[x][y].lamplit)
            {
                Strcpy(debug_buf_4, "snuff_light_source");
                levl[x][y].lamplit = 0;
                del_light_source(LS_LOCATION, xy_to_any(x, y));
                newsym(x, y);
                return;
            }
        }
}

/* Return TRUE if object sheds any light at all. */
boolean
obj_sheds_light(obj)
struct obj *obj;
{
    /* so far, only burning objects shed light */
    /* objects in memory do not shed light */
    return obj_is_burning(obj);
}

/* Return TRUE if sheds light AND will be snuffed by end_burn(). */
boolean
obj_is_burning(obj)
struct obj *obj;
{
    return (boolean) (obj->lamplit && (is_obj_ignitable(obj)
                                    || artifact_light(obj)
                                    || obj_shines_magical_light(obj)
                                    || has_obj_mythic_magical_light(obj)
        ));
}

/* copy the light source(s) attached to src, and attach it/them to dest */
void
obj_split_light_source(src, dest)
struct obj *src, *dest;
{
    light_source *ls, *new_ls;

    for (ls = light_base; ls; ls = ls->next)
        if (ls->type == LS_OBJECT && ls->id.a_obj == src) {
            /*
             * Insert the new source at beginning of list.  This will
             * never interfere us walking down the list - we are already
             * past the insertion point.
             */
            new_ls = (light_source *) alloc(sizeof(light_source));
            *new_ls = *ls;
            if (is_candle(src)) {
                /* split candles may emit less light than original group */
                ls->range = candle_light_range(src);
                new_ls->range = candle_light_range(dest);
                vision_full_recalc = 1; /* in case range changed */
            }
            new_ls->id.a_obj = dest;
            new_ls->next = light_base;
            light_base = new_ls;
            dest->lamplit = 1; /* now an active light source */
        }
}

/* light source `src' has been folded into light source `dest';
   used for merging lit candles and adding candle(s) to lit candelabrum */
void
obj_merge_light_sources(src, dest)
struct obj *src, *dest;
{
    light_source *ls;

    Strcpy(debug_buf_3, "obj_merge_light_sources");
    /* src == dest implies adding to candelabrum */
    if (src != dest)
        end_burn(src, TRUE); /* extinguish candles */

    for (ls = light_base; ls; ls = ls->next)
        if (ls->type == LS_OBJECT && ls->id.a_obj == dest) {
            ls->range = candle_light_range(dest);
            vision_full_recalc = 1; /* in case range changed */
            break;
        }
}

/* light source `obj' is being made brighter or dimmer */
void
obj_adjust_light_radius(obj, new_radius)
struct obj *obj;
int new_radius;
{
    light_source *ls;

    for (ls = light_base; ls; ls = ls->next)
        if (ls->type == LS_OBJECT && ls->id.a_obj == obj) {
            if (new_radius != ls->range)
                vision_full_recalc = 1;
            ls->range = new_radius;
            return;
        }
    impossible("obj_adjust_light_radius: can't find %s", xname(obj));
}

/* Candlelight is proportional to the number of candles;
   minimum range is 2 rather than 1 for playability. */
int
candle_light_range(obj)
struct obj *obj;
{
    int radius;

    if (obj->otyp == CANDELABRUM_OF_INVOCATION) 
    {
        /*
         *      The special candelabrum emits more light than the
         *      corresponding number of candles would.
         *       1..3 candles, range 2 (minimum range);
         *       4..6 candles, range 3 (normal lamp range);
         *          7 candles, range 4 (bright).
         */
        radius = (obj->special_quality < 4) ? 2 : (obj->special_quality < 7) ? 3 : 4;
    } 
    else if (is_obj_candelabrum(obj))
    {
        radius = (obj->special_quality < 5) ? 2 : (obj->special_quality < 9) ? 3 : 4;
    }
    else if (is_candle(obj))
    {
        /*
         *      Range is incremented by powers of 7 so that it will take
         *      wizard mode quantities of candles to get more light than
         *      from a lamp, without imposing an arbitrary limit.
         *       1..6   candles, range 2;
         *       7..48  candles, range 3;
         *      49..342 candles, range 4; &c.
         */
        int64_t n = obj->quan;

        radius = 1; /* always incremented at least once */
        do {
            radius++;
            n /= 7L;
        } while (n > 0L);
    }
    else
    {
        /* we're only called for lit candelabrum or candles */
        /* impossible("candlelight for %d?", obj->otyp); */
        radius = 3; /* lamp's value */
    }
    return radius;
}

/* light emitting artifact's range depends upon its curse/bless state */
int
artifact_light_range(obj)
struct obj* obj;
{
    return (obj->blessed ? 3 : !obj->cursed ? 2 : 1);
}

/* light emitting artifact's range depends upon its curse/bless state */
int
current_arti_light_radius(obj)
struct obj *obj;
{
    /*
     * Used by begin_burn() when setting up a new light source
     * (obj->lamplit will already be set by this point) and
     * also by bless()/unbless()/uncurse()/curse() to decide
     * whether to call obj_adjust_light_radius().
     */

    /* sanity check [simplifies usage by bless()/curse()/&c] */
    if (!obj->lamplit || !(artifact_light(obj) || obj_shines_magical_light(obj) || has_obj_mythic_magical_light(obj)))
        return 0;

    /* cursed radius of 1 is not noticeable for an item that's
       carried by the hero but is if it's carried by a monster
       or left lit on the floor (not applicable for Sunsword) */
    return artifact_light_range(obj);
}


int64_t
obj_light_maximum_burn_time(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    if (!is_obj_light_source(obj))
        return 0;

    int64_t maxburntime = 0; //Normal lamps
    if (objects[obj->otyp].oc_flags5 & O5_BURNS_INFINITELY)
        maxburntime = -1;
    else if (artifact_light(obj) || obj_shines_magical_light(obj) || has_obj_mythic_magical_light(obj))
        maxburntime = -1;
    else if (is_candle(obj))
        maxburntime = candle_maximum_burn_time(obj);
    else if (is_torch(obj))
        maxburntime = torch_maximum_burn_time(obj);
    else if (is_obj_candelabrum(obj))
        maxburntime = candlelabrum_maximum_burn_time(obj);
    else if (obj->otyp == POTION_CLASS) //Potion of oil
        maxburntime = potion_maximum_burn_time(obj);
    else
        maxburntime = lamp_maximum_burn_time(obj);

    return maxburntime;
}


int
obj_light_radius(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    //if (!is_obj_light_source(obj))
    //    return 0;

    int radius = 3; //Normal lamps and torches
    if (is_candle(obj) || is_obj_candelabrum(obj))
        radius = candle_light_range(obj);
    else if (obj->otyp == POTION_CLASS) //Potion of oil
        radius = 1;
    else if (artifact_light(obj) || (obj_shines_magical_light(obj) || has_obj_mythic_magical_light(obj)))
        radius = artifact_light_range(obj);

    return radius;
}

int64_t
burn_time_left_from_timer(obj)
struct obj* obj;
{
    if (!obj)
        return 0L;

    int64_t timeout = peek_timer(BURN_OBJECT, obj_to_any(obj));

    if (!timeout)
        return 0L;

    int64_t howlong = monstermoves - timeout;
    int64_t age = obj->age - howlong;
    return age;
}

int64_t
obj_light_burn_time_left(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    if (!is_obj_light_source(obj))
        return 0;

    int64_t burntimeleft = 0;
    if (objects[obj->otyp].oc_flags5 & O5_BURNS_INFINITELY)
        burntimeleft = -1;
    else if (artifact_light(obj) || (obj_shines_magical_light(obj) || has_obj_mythic_magical_light(obj)))
        burntimeleft = -1;
    else if (!obj->lamplit)
        burntimeleft = obj->age;
    else
        burntimeleft = burn_time_left_from_timer(obj);

    return burntimeleft;
}



/* adverb describing lit artifact's light; depends on curse/bless state */
const char *
arti_light_description(obj)
struct obj *obj;
{
    switch (current_arti_light_radius(obj)) {
    case 3:
        return "brilliantly"; /* blessed */
    case 2:
        return "brightly"; /* uncursed */
    case 1:
        return "dimly"; /* cursed */
    default:
        break;
    }
    return "strangely";
}

int
wiz_light_sources()
{
    winid win;
    char buf[BUFSZ];
    light_source *ls;

    win = create_nhwindow(NHW_MENU); /* corner text window */
    if (win == WIN_ERR)
        return 0;

    Sprintf(buf, "Mobile light sources: hero @ (%2d,%2d)", u.ux, u.uy);
    putstr(win, 0, buf);
    putstr(win, 0, "");

    if (light_base) 
    {
        putstr(win, 0, "location range flags  type    id");
        putstr(win, 0, "-------- ----- ------ ----  -------");
        for (ls = light_base; ls; ls = ls->next) 
        {
            Sprintf(buf, "  %2d,%2d   %2d   0x%04x  %s  %s", ls->x, ls->y,
                    ls->range, ls->flags,
                    (ls->type == LS_OBJECT
                       ? "obj" :
                        ls->type == LS_LOCATION
                        ? "loc" : 
                        ls->type == LS_MONSTER
                          ? (mon_is_local_mx(ls->id.a_monst)
                             ? "mon"
                             : (ls->id.a_monst == &youmonst)
                                ? "you"
                                /* migrating monster */
                                : "<m>")
                          : "???"),
                    fmt_ptr(ls->id.a_void));
            putstr(win, 0, buf);
        }
    } 
    else
        putstr(win, 0, "<none>");

    display_nhwindow(win, FALSE);
    destroy_nhwindow(win);

    return 0;
}

/*light.c*/

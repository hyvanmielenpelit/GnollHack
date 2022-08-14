/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    region.c    $NHDT-Date: 1543455828 2018/11/29 01:43:48 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.43 $ */
/* Copyright (c) 1996 by Jean-Christophe Collet  */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"

/*
 * This should really go into the level structure, but
 * I'll start here for ease. It *WILL* move into the level
 * structure eventually.
 */

STATIC_VAR NhRegion **regions;
STATIC_VAR int n_regions = 0;
STATIC_VAR int max_regions = 0;

boolean FDECL(enter_special_level_seen, (genericptr, genericptr));
boolean FDECL(enter_special_level_true_nature_revealed, (genericptr, genericptr));
boolean FDECL(inside_gas_cloud, (genericptr, genericptr));
boolean FDECL(expire_gas_cloud, (genericptr, genericptr));
boolean FDECL(inside_rect, (NhRect *, int, int));
boolean FDECL(inside_region, (NhRegion *, int, int));
NhRegion *FDECL(create_region, (enum region_types, NhRect *, int));
void FDECL(add_rect_to_reg, (NhRegion *, NhRect *));
void FDECL(add_mon_to_reg, (NhRegion *, struct monst *));
void FDECL(remove_mon_from_reg, (NhRegion *, struct monst *));
boolean FDECL(mon_in_region, (NhRegion *, struct monst *));

#if 0
NhRegion *FDECL(clone_region, (NhRegion *));
#endif
void FDECL(free_region, (NhRegion *));
void FDECL(add_region, (NhRegion *));
void FDECL(remove_region, (NhRegion *));

#if 0
void FDECL(replace_mon_regions, (struct monst *,struct monst *));
void FDECL(remove_mon_from_regions, (struct monst *));
NhRegion *FDECL(create_msg_region, (XCHAR_P,XCHAR_P,XCHAR_P,XCHAR_P,
                                    const char *,const char *));
boolean FDECL(enter_force_field, (genericptr,genericptr));
NhRegion *FDECL(create_force_field, (XCHAR_P,XCHAR_P,int,long));
#endif

STATIC_DCL void FDECL(reset_region_mids, (NhRegion *));

struct region_type_definition region_type_definitions[MAX_REGION_TYPES] =
{
    {"general",                 REGION_BASETYPE_GENERAL, 0, 0, 0, 0, 0, 0, FALSE, S_unexplored, 0, FALSE,  FALSE,  FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"message",                 REGION_BASETYPE_GENERAL, 0, 0, 0, 0, 0, 0, FALSE, S_unexplored, 0, FALSE,  FALSE,  FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"special level seen",      REGION_BASETYPE_GENERAL, 0, 0, 0, enter_special_level_seen, 0, 0, FALSE, S_unexplored, 0, FALSE,  FALSE,  FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"sp.lvl true nature rev.", REGION_BASETYPE_GENERAL, 0, 0, 0, enter_special_level_true_nature_revealed, 0, 0, FALSE, S_unexplored, 0, FALSE,  FALSE,  FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"poison gas",              REGION_BASETYPE_GAS, inside_gas_cloud, expire_gas_cloud, 0, 0, 0, 0, TRUE, S_poisoncloud, 0, TRUE,   FALSE, TRUE,   0, REGION_SOUNDSET_POISON_GAS, 0UL },
    {"smoke",                   REGION_BASETYPE_GAS, 0, 0, 0, 0, 0, 0, TRUE, S_cloud, 0, TRUE,   FALSE, TRUE,   0, REGION_SOUNDSET_NONE, 0UL },
    {"cloud",                   REGION_BASETYPE_GAS, 0, 0, 0, 0, 0, 0, TRUE, S_cloud, 0, TRUE,   FALSE, TRUE,   0, REGION_SOUNDSET_NONE, 0UL },
    {"fire",                    REGION_BASETYPE_ELEMENTAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,  FALSE,  1, REGION_SOUNDSET_NONE, 0UL },
    {"lightning",               REGION_BASETYPE_ELEMENTAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,  FALSE,  1, REGION_SOUNDSET_NONE, 0UL },
    {"frost",                   REGION_BASETYPE_ELEMENTAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,  FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"death",                   REGION_BASETYPE_ELEMENTAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,  FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"annihilation",            REGION_BASETYPE_ELEMENTAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,  FALSE, -1, REGION_SOUNDSET_NONE, 0UL },
    {"magical darkness",        REGION_BASETYPE_MAGICAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, FALSE,  FALSE,   FALSE, -1, REGION_SOUNDSET_NONE, 0UL },
    {"magical silence",         REGION_BASETYPE_MAGICAL_EFFECT, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, FALSE,  FALSE,   FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"transparent force field", REGION_BASETYPE_FORCEFIELD, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,   FALSE,  0, REGION_SOUNDSET_NONE, 0UL },
    {"opaque force field",      REGION_BASETYPE_FORCEFIELD, 0, 0, 0, 0, 0, 0, TRUE, S_unexplored, 0, TRUE,   TRUE,   TRUE,   0, REGION_SOUNDSET_NONE, 0UL }
};



/* Should be inlined. */
boolean
inside_rect(r, x, y)
NhRect *r;
int x, y;
{
    return (boolean) (x >= r->lx && x <= r->hx && y >= r->ly && y <= r->hy);
}

/*
 * Check if a point is inside a region.
 */
boolean
inside_region(reg, x, y)
NhRegion *reg;
int x, y;
{
    int i;

    if (reg == (NhRegion *) 0 || !inside_rect(&(reg->bounding_box), x, y))
        return FALSE;
    for (i = 0; i < reg->nrects; i++)
        if (inside_rect(&(reg->rects[i]), x, y))
            return TRUE;
    return FALSE;
}

/*
 * Create a region. It does not activate it.
 */
NhRegion*
create_region(typ, rects, nrect)
enum region_types typ;
NhRect *rects;
int nrect;
{
    int i;
    NhRegion *reg;

    reg = (NhRegion *) alloc(sizeof(NhRegion));
    (void) memset((genericptr_t)reg, 0, sizeof(NhRegion));
    /* Determines bounding box */
    if (nrect > 0) {
        reg->bounding_box = rects[0];
    } else {
        reg->bounding_box.lx = COLNO;
        reg->bounding_box.ly = ROWNO;
        reg->bounding_box.hx = 0; /* 1 */
        reg->bounding_box.hy = 0;
    }
    reg->nrects = nrect;
    reg->rects = (nrect > 0) ? (NhRect *) alloc((size_t)nrect * sizeof (NhRect)) : 0;
    for (i = 0; i < nrect; i++) {
        if (rects[i].lx < reg->bounding_box.lx)
            reg->bounding_box.lx = rects[i].lx;
        if (rects[i].ly < reg->bounding_box.ly)
            reg->bounding_box.ly = rects[i].ly;
        if (rects[i].hx > reg->bounding_box.hx)
            reg->bounding_box.hx = rects[i].hx;
        if (rects[i].hy > reg->bounding_box.hy)
            reg->bounding_box.hy = rects[i].hy;
        reg->rects[i] = rects[i];
    }
    reg->time_to_live = -1L; /* Defaults */
    reg->attach_2_u = FALSE;
    reg->attach_2_m = 0;
    /* reg->attach_2_o = NULL; */
    reg->enter_msg = (const char *) 0;
    reg->leave_msg = (const char *) 0;
    reg->enter_msg_is_on = FALSE;
    reg->leave_msg_is_on = FALSE;
    reg->inside_proc_is_on = (region_type_definitions[typ].inside_proc != 0);
    reg->expire_proc_is_on = (region_type_definitions[typ].expire_proc != 0);
    reg->can_enter_proc_is_on = (region_type_definitions[typ].can_enter_proc != 0);
    reg->enter_proc_is_on = (region_type_definitions[typ].enter_proc != 0);
    reg->can_leave_proc_is_on = (region_type_definitions[typ].can_leave_proc != 0);
    reg->leave_proc_is_on = (region_type_definitions[typ].leave_proc != 0);
    clear_hero_inside(reg);
    clear_heros_fault(reg);
    reg->n_monst = 0;
    reg->max_monst = 0;
    reg->monsters = (unsigned int *) 0;
    reg->arg = zeroany;
    reg->typ = typ;
    reg->visible = region_type_definitions[typ].visible;
    reg->glyph = region_type_definitions[typ].variation > 0 && defsyms[region_type_definitions[typ].screen_symbol].variations <= region_type_definitions[typ].variation
        ? cmap_variation_to_glyph(defsyms[region_type_definitions[typ].screen_symbol].variation_offset + region_type_definitions[typ].variation - 1) 
        : cmap_to_glyph(region_type_definitions[typ].screen_symbol);
    if (region_type_definitions[typ].is_light_source != 0)
        reg->lamplit = TRUE;

    return reg;
}

/*
 * Add rectangle to region.
 */
void
add_rect_to_reg(reg, rect)
NhRegion *reg;
NhRect *rect;
{
    NhRect *tmp_rect;

    tmp_rect = (NhRect *) alloc(((size_t)reg->nrects + 1) * sizeof (NhRect));
    if (reg->nrects > 0) {
        (void) memcpy((genericptr_t) tmp_rect, (genericptr_t) reg->rects,
                      reg->nrects * sizeof (NhRect));
        free((genericptr_t) reg->rects);
    }
    tmp_rect[reg->nrects] = *rect;
    reg->nrects++;
    reg->rects = tmp_rect;
    /* Update bounding box if needed */
    if (reg->bounding_box.lx > rect->lx)
        reg->bounding_box.lx = rect->lx;
    if (reg->bounding_box.ly > rect->ly)
        reg->bounding_box.ly = rect->ly;
    if (reg->bounding_box.hx < rect->hx)
        reg->bounding_box.hx = rect->hx;
    if (reg->bounding_box.hy < rect->hy)
        reg->bounding_box.hy = rect->hy;
}

/*
 * Add a monster to the region
 */
void
add_mon_to_reg(reg, mon)
NhRegion *reg;
struct monst *mon;
{
    int i;
    unsigned *tmp_m;

    if (reg->max_monst <= reg->n_monst) {
        tmp_m = (unsigned *) alloc(sizeof (unsigned) * ((size_t)reg->max_monst + MONST_INC));
        if (reg->max_monst > 0) {
            for (i = 0; i < reg->max_monst; i++)
                tmp_m[i] = reg->monsters[i];
            free((genericptr_t) reg->monsters);
        }
        reg->monsters = tmp_m;
        reg->max_monst += MONST_INC;
    }
    reg->monsters[reg->n_monst++] = mon->m_id;
}

/*
 * Remove a monster from the region list (it left or died...)
 */
void
remove_mon_from_reg(reg, mon)
NhRegion *reg;
struct monst *mon;
{
    register int i;

    for (i = 0; i < reg->n_monst; i++)
        if (reg->monsters[i] == mon->m_id) {
            reg->n_monst--;
            reg->monsters[i] = reg->monsters[reg->n_monst];
            return;
        }
}

/*
 * Check if a monster is inside the region.
 * It's probably quicker to check with the region internal list
 * than to check for coordinates.
 */
boolean
mon_in_region(reg, mon)
NhRegion *reg;
struct monst *mon;
{
    int i;

    for (i = 0; i < reg->n_monst; i++)
        if (reg->monsters[i] == mon->m_id)
            return TRUE;
    return FALSE;
}

#if 0
/* not yet used */

/*
 * Clone (make a standalone copy) the region.
 */
NhRegion *
clone_region(reg)
NhRegion *reg;
{
    NhRegion *ret_reg;

    ret_reg = create_region(reg->typ, reg->rects, reg->nrects);
    ret_reg->time_to_live = reg->time_to_live;
    ret_reg->attach_2_u = reg->attach_2_u;
    ret_reg->attach_2_m = reg->attach_2_m;
 /* ret_reg->attach_2_o = reg->attach_2_o; */
    ret_reg->player_flags = reg->player_flags; /* set/clear_hero_inside,&c*/
    ret_reg->n_monst = reg->n_monst;
    if (reg->n_monst > 0) {
        ret_reg->monsters = (unsigned int *)
                                    alloc((sizeof (unsigned)) * (size_t)reg->n_monst);
        (void) memcpy((genericptr_t) ret_reg->monsters,
                      (genericptr_t) reg->monsters,
                      sizeof (unsigned) * reg->n_monst);
    } else
        ret_reg->monsters = (unsigned int *) 0;
    return ret_reg;
}

#endif /*0*/

/*
 * Free mem from region.
 */
void
free_region(reg)
NhRegion *reg;
{
    if (reg) {
        if (reg->rects)
            free((genericptr_t) reg->rects);
        if (reg->monsters)
            free((genericptr_t) reg->monsters);
        if (reg->enter_msg)
            free((genericptr_t) reg->enter_msg);
        if (reg->leave_msg)
            free((genericptr_t) reg->leave_msg);
        free((genericptr_t) reg);
    }
}

/*
 * Add a region to the list.
 * This actually activates the region.
 */
void
add_region(reg)
NhRegion *reg;
{
    NhRegion **tmp_reg;
    int i, j;

    if (max_regions <= n_regions) {
        tmp_reg = regions;
        regions =
            (NhRegion **) alloc(((size_t)max_regions + 10) * sizeof (NhRegion *));
        if (max_regions > 0) {
            (void) memcpy((genericptr_t) regions, (genericptr_t) tmp_reg,
                          max_regions * sizeof (NhRegion *));
            free((genericptr_t) tmp_reg);
        }
        max_regions += 10;
    }
    regions[n_regions] = reg;
    n_regions++;

    /* Start making sound */
    if (region_type_definitions[reg->typ].soundset > REGION_SOUNDSET_NONE)
    {
        int x, y;
        x = (reg->bounding_box.lx + reg->bounding_box.hx) / 2;
        y = (reg->bounding_box.ly + reg->bounding_box.hy) / 2;
        if (!isok(x, y))
            x = 1, y = 1; /* These are actually not being used for a region sound source */

        new_sound_source(x, y,
            region_soundsets[region_type_definitions[reg->typ].soundset].sounds[REGION_SOUND_TYPE_AMBIENT].ghsound,
            region_soundsets[region_type_definitions[reg->typ].soundset].sounds[REGION_SOUND_TYPE_AMBIENT].volume,
            SOUNDSOURCE_REGION,
            region_soundsets[region_type_definitions[reg->typ].soundset].ambient_subtype, region_to_any(reg));
        reg->makingsound = TRUE;
    }

    /* Check for monsters inside the region */
    for (i = reg->bounding_box.lx; i <= reg->bounding_box.hx; i++)
        for (j = reg->bounding_box.ly; j <= reg->bounding_box.hy; j++)
        {
            /* Some regions can cross the level boundaries */
            if (!isok(i, j))
                continue;
            if (MON_AT(i, j) && inside_region(reg, i, j))
                add_mon_to_reg(reg, level.monsters[i][j]);
            if (reg->visible && cansee(i, j))
                newsym(i, j);
        }

    /* Check for player now... */
    if (!in_mklev && inside_region(reg, u.ux, u.uy))
        set_hero_inside(reg);
    else
        clear_hero_inside(reg);
}

/*
 * Remove a region from the list & free it.
 */
void
remove_region(reg)
NhRegion *reg;
{
    register int i, x, y;

    for (i = 0; i < n_regions; i++)
        if (regions[i] == reg)
            break;
    if (i == n_regions)
        return;

    /* Update screen if necessary */
    reg->time_to_live = -2L; /* for visible_region_at */
    if (reg->visible)
        for (x = reg->bounding_box.lx; x <= reg->bounding_box.hx; x++)
            for (y = reg->bounding_box.ly; y <= reg->bounding_box.hy; y++)
                if (isok(x, y) && inside_region(reg, x, y) && cansee(x, y))
                    newsym(x, y);

    if (reg->makingsound)
    {
        del_sound_source(SOUNDSOURCE_REGION, region_to_any(reg));
        reg->makingsound = FALSE;
    }

    free_region(reg);
    regions[i] = regions[n_regions - 1];
    regions[n_regions - 1] = (NhRegion *) 0;
    n_regions--;
}

/*
 * Remove all regions and clear all related data (This must be down
 * when changing level, for instance).
 */
void
clear_regions()
{
    register int i;

    for (i = 0; i < n_regions; i++)
        free_region(regions[i]);
    n_regions = 0;
    if (max_regions > 0)
        free((genericptr_t) regions);
    max_regions = 0;
    regions = (NhRegion **) 0;
}

/*
 * This function is called every turn.
 * It makes the regions age, if necessary and calls the appropriate
 * callbacks when needed.
 */
void
run_regions()
{
    register int i, j, k;
//    int f_indx;

    /* End of life ? */
    /* Do it backward because the array will be modified */
    for (i = n_regions - 1; i >= 0; i--) {
        if (regions[i]->time_to_live == 0L) {
            if (!region_type_definitions[regions[i]->typ].expire_proc
                || (*region_type_definitions[regions[i]->typ].expire_proc)(regions[i], (genericptr_t)0))
                remove_region(regions[i]);
        }
    }

    /* Process remaining regions */
    for (i = 0; i < n_regions; i++) {
        /* Make the region age */
        if (regions[i]->time_to_live > 0L)
            regions[i]->time_to_live--;
        /* Check if player is inside region */
        //f_indx = regions[i]->inside_f;
        if (region_type_definitions[regions[i]->typ].inside_proc && hero_inside(regions[i]))
            (void) (*region_type_definitions[regions[i]->typ].inside_proc)(regions[i], (genericptr_t) 0);
        /* Check if any monster is inside region */
        if (region_type_definitions[regions[i]->typ].inside_proc) {
            for (j = 0; j < regions[i]->n_monst; j++) {
                struct monst *mtmp =
                    find_mid(regions[i]->monsters[j], FM_FMON);

                if (!mtmp || DEADMONSTER(mtmp)
                    || (*region_type_definitions[regions[i]->typ].inside_proc)(regions[i], mtmp)) {
                    /* The monster died, remove it from list */
                    k = (regions[i]->n_monst -= 1);
                    regions[i]->monsters[j] = regions[i]->monsters[k];
                    regions[i]->monsters[k] = 0;
                    --j; /* current slot has been reused; recheck it next */
                }
            }
        }
    }
}

/*
 * check whether player enters/leaves one or more regions.
 */
boolean
in_out_region(x, y)
xchar x, y;
{
    int i; // , f_indx;

    /* First check if we can do the move */
    for (i = 0; i < n_regions; i++) {
        if (inside_region(regions[i], x, y) && !hero_inside(regions[i])
            && !regions[i]->attach_2_u) {
            if (regions[i]->can_enter_proc_is_on && region_type_definitions[regions[i]->typ].can_enter_proc)
                if (!(*region_type_definitions[regions[i]->typ].can_enter_proc)(regions[i], (genericptr_t) 0))
                    return FALSE;
        } else if (hero_inside(regions[i]) && !inside_region(regions[i], x, y)
                   && !regions[i]->attach_2_u) {
            if (regions[i]->can_leave_proc_is_on && region_type_definitions[regions[i]->typ].can_leave_proc)
                if (!(*region_type_definitions[regions[i]->typ].can_leave_proc)(regions[i], (genericptr_t) 0))
                    return FALSE;
        }
    }

    /* Callbacks for the regions we do leave */
    for (i = 0; i < n_regions; i++)
        if (hero_inside(regions[i]) && !regions[i]->attach_2_u
            && !inside_region(regions[i], x, y)) {
            clear_hero_inside(regions[i]);
            if (regions[i]->leave_msg_is_on && regions[i]->leave_msg != (const char *) 0)
                pline1(regions[i]->leave_msg);
            if (regions[i]->leave_proc_is_on && region_type_definitions[regions[i]->typ].leave_proc)
                (void) (*region_type_definitions[regions[i]->typ].leave_proc)(regions[i], (genericptr_t) 0);
        }

    /* Callbacks for the regions we do enter */
    for (i = 0; i < n_regions; i++)
        if (!hero_inside(regions[i]) && !regions[i]->attach_2_u
            && inside_region(regions[i], x, y)) {
            set_hero_inside(regions[i]);
            if (regions[i]->enter_msg_is_on && regions[i]->enter_msg != (const char *) 0)
                pline1(regions[i]->enter_msg);
            if (regions[i]->enter_proc_is_on && region_type_definitions[regions[i]->typ].enter_proc)
                (void) (*region_type_definitions[regions[i]->typ].enter_proc)(regions[i], (genericptr_t) 0);
        }
    return TRUE;
}

/*
 * check whether a monster enters/leaves one or more region.
*/
boolean
m_in_out_region(mon, x, y)
struct monst *mon;
xchar x, y;
{
    int i; // , f_indx;

    /* First check if we can do the move */
    for (i = 0; i < n_regions; i++) {
        if (inside_region(regions[i], x, y) && !mon_in_region(regions[i], mon)
            && regions[i]->attach_2_m != mon->m_id) {
            if (regions[i]->can_enter_proc_is_on && region_type_definitions[regions[i]->typ].can_enter_proc)
                if (!(*region_type_definitions[regions[i]->typ].can_enter_proc)(regions[i], mon))
                    return FALSE;
        } else if (mon_in_region(regions[i], mon)
                   && !inside_region(regions[i], x, y)
                   && regions[i]->attach_2_m != mon->m_id) {
            if (regions[i]->can_leave_proc_is_on && region_type_definitions[regions[i]->typ].can_leave_proc)
                if (!(*region_type_definitions[regions[i]->typ].can_leave_proc)(regions[i], mon))
                    return FALSE;
        }
    }

    /* Callbacks for the regions we do leave */
    for (i = 0; i < n_regions; i++)
        if (mon_in_region(regions[i], mon)
            && regions[i]->attach_2_m != mon->m_id
            && !inside_region(regions[i], x, y)) {
            remove_mon_from_reg(regions[i], mon);
            if (regions[i]->leave_proc_is_on && region_type_definitions[regions[i]->typ].leave_proc)
                (void) (*region_type_definitions[regions[i]->typ].leave_proc)(regions[i], mon);
        }

    /* Callbacks for the regions we do enter */
    for (i = 0; i < n_regions; i++)
        if (!mon_in_region(regions[i], mon) 
            && regions[i]->attach_2_m != mon->m_id
            && inside_region(regions[i], x, y)) {
            add_mon_to_reg(regions[i], mon);
            if (regions[i]->enter_proc_is_on && region_type_definitions[regions[i]->typ].enter_proc)
                (void) (*region_type_definitions[regions[i]->typ].enter_proc)(regions[i], mon);
        }
    return TRUE;
}

/*
 * Checks player's regions after a teleport for instance.
 */
void
update_player_regions()
{
    register int i;

    for (i = 0; i < n_regions; i++)
        if (!regions[i]->attach_2_u && inside_region(regions[i], u.ux, u.uy))
            set_hero_inside(regions[i]);
        else
            clear_hero_inside(regions[i]);
}

/*
 * Ditto for a specified monster.
 */
void
update_monster_region(mon)
struct monst *mon;
{
    register int i;

    for (i = 0; i < n_regions; i++) {
        if (inside_region(regions[i], mon->mx, mon->my)) {
            if (!mon_in_region(regions[i], mon))
                add_mon_to_reg(regions[i], mon);
        } else {
            if (mon_in_region(regions[i], mon))
                remove_mon_from_reg(regions[i], mon);
        }
    }
}

#if 0
/* not yet used */

/*
 * Change monster pointer in regions
 * This happens, for instance, when a monster grows and
 * need a new structure (internally that is).
 */
void
replace_mon_regions(monold, monnew)
struct monst *monold, *monnew;
{
    register int i;

    for (i = 0; i < n_regions; i++)
        if (mon_in_region(regions[i], monold)) {
            remove_mon_from_reg(regions[i], monold);
            add_mon_to_reg(regions[i], monnew);
        }
}

/*
 * Remove monster from all regions it was in (ie monster just died)
 */
void
remove_mon_from_regions(mon)
struct monst *mon;
{
    register int i;

    for (i = 0; i < n_regions; i++)
        if (mon_in_region(regions[i], mon))
            remove_mon_from_reg(regions[i], mon);
}

#endif /*0*/

/*
 * Check if a spot is under a visible region (eg: gas cloud).
 * Returns NULL if not, otherwise returns region.
 */
NhRegion *
visible_region_at(x, y)
xchar x, y;
{
    register int i;

    for (i = 0; i < n_regions; i++)
        if (inside_region(regions[i], x, y) && regions[i]->visible
            && regions[i]->time_to_live != -2L)
            return regions[i];
    return (NhRegion *) 0;
}

void
show_region(reg, x, y)
NhRegion *reg;
xchar x, y;
{
    show_glyph_ascii(x, y, reg->glyph);
    show_glyph_on_layer(x, y, reg->glyph, LAYER_ENVIRONMENT);
}

/**
 * save_regions :
 */
void
save_regions(fd, mode)
int fd;
int mode;
{
    int i, j;
    size_t n;

    if (!perform_bwrite(mode))
        goto skip_lots;

    bwrite(fd, (genericptr_t) &moves, sizeof(moves)); /* timestamp */
    bwrite(fd, (genericptr_t) &n_regions, sizeof(n_regions));
    for (i = 0; i < n_regions; i++) {
        bwrite(fd, (genericptr_t) &regions[i]->bounding_box, sizeof(NhRect));
        bwrite(fd, (genericptr_t) &regions[i]->nrects, sizeof(short));
        for (j = 0; j < regions[i]->nrects; j++)
            bwrite(fd, (genericptr_t) &regions[i]->rects[j], sizeof(NhRect));
        bwrite(fd, (genericptr_t) &regions[i]->attach_2_u, sizeof(boolean));
        n = 0;
        bwrite(fd, (genericptr_t) &regions[i]->attach_2_m, sizeof(unsigned));
        n = regions[i]->enter_msg != (const char *) 0
                ? strlen(regions[i]->enter_msg)
                : 0;
        bwrite(fd, (genericptr_t) &n, sizeof n);
        if (n > 0)
            bwrite(fd, (genericptr_t) regions[i]->enter_msg, n);
        n = regions[i]->leave_msg != (const char *) 0
                ? strlen(regions[i]->leave_msg)
                : 0;
        bwrite(fd, (genericptr_t) &n, sizeof n);
        if (n > 0)
            bwrite(fd, (genericptr_t) regions[i]->leave_msg, n);
        bwrite(fd, (genericptr_t)&regions[i]->enter_msg_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->leave_msg_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->inside_proc_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->expire_proc_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->can_enter_proc_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->enter_proc_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->can_leave_proc_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t)&regions[i]->leave_proc_is_on, sizeof(boolean));
        bwrite(fd, (genericptr_t) &regions[i]->time_to_live, sizeof(long));
        bwrite(fd, (genericptr_t) &regions[i]->player_flags,
               sizeof(unsigned int));
        bwrite(fd, (genericptr_t) &regions[i]->n_monst, sizeof(short));
        for (j = 0; j < regions[i]->n_monst; j++)
            bwrite(fd, (genericptr_t) &regions[i]->monsters[j],
                   sizeof(unsigned));
        bwrite(fd, (genericptr_t) &regions[i]->visible, sizeof(boolean));
        bwrite(fd, (genericptr_t) &regions[i]->glyph, sizeof(int));
        bwrite(fd, (genericptr_t) &regions[i]->arg, sizeof(anything));
        bwrite(fd, (genericptr_t) &regions[i]->typ, sizeof(enum region_types));
        bwrite(fd, (genericptr_t) &regions[i]->dmg_dice, sizeof(int));
        bwrite(fd, (genericptr_t) &regions[i]->dmg_diesize, sizeof(int));
        bwrite(fd, (genericptr_t) &regions[i]->dmg_plus, sizeof(int));
        bwrite(fd, (genericptr_t) &regions[i]->dmg_adjustment, sizeof(double));
        bwrite(fd, (genericptr_t) &regions[i]->region_flags, sizeof(unsigned long));
        bwrite(fd, (genericptr_t) &regions[i]->lamplit, sizeof(boolean));
        bwrite(fd, (genericptr_t) &regions[i]->makingsound, sizeof(boolean));
    }

skip_lots:
    if (release_data(mode))
        clear_regions();
}

void
rest_regions(fd, ghostly)
int fd;
boolean ghostly; /* If a bones file restore */
{
    int i, j;
    size_t n;
    long tmstamp;
    char *msg_buf;

    clear_regions(); /* Just for security */
    mread(fd, (genericptr_t) &tmstamp, sizeof(tmstamp));
    if (ghostly)
        tmstamp = 0;
    else
        tmstamp = (moves - tmstamp);
    mread(fd, (genericptr_t) &n_regions, sizeof(n_regions));
    max_regions = n_regions;
    if (n_regions > 0)
        regions = (NhRegion **) alloc(sizeof(NhRegion *) * (size_t)n_regions);
    for (i = 0; i < n_regions; i++) {
        regions[i] = (NhRegion *) alloc(sizeof(NhRegion));
        mread(fd, (genericptr_t) &regions[i]->bounding_box, sizeof(NhRect));
        mread(fd, (genericptr_t) &regions[i]->nrects, sizeof(short));

        if (regions[i]->nrects > 0)
            regions[i]->rects =
                (NhRect *) alloc(sizeof(NhRect) * (size_t)regions[i]->nrects);
        for (j = 0; j < regions[i]->nrects; j++)
            mread(fd, (genericptr_t) &regions[i]->rects[j], sizeof(NhRect));
        mread(fd, (genericptr_t) &regions[i]->attach_2_u, sizeof(boolean));
        mread(fd, (genericptr_t) &regions[i]->attach_2_m, sizeof(unsigned));

        mread(fd, (genericptr_t) &n, sizeof n);
        if (n > 0) {
            msg_buf = (char *) alloc(n + 1);
            mread(fd, (genericptr_t) msg_buf, n);
            msg_buf[n] = '\0';
            regions[i]->enter_msg = (const char *) msg_buf;
        } else
            regions[i]->enter_msg = (const char *) 0;

        mread(fd, (genericptr_t) &n, sizeof n);
        if (n > 0) {
            msg_buf = (char *) alloc(n + 1);
            mread(fd, (genericptr_t) msg_buf, n);
            msg_buf[n] = '\0';
            regions[i]->leave_msg = (const char *) msg_buf;
        } else
            regions[i]->leave_msg = (const char *) 0;

        mread(fd, (genericptr_t)&regions[i]->enter_msg_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->leave_msg_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->inside_proc_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->expire_proc_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->can_enter_proc_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->enter_proc_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->can_leave_proc_is_on, sizeof(boolean));
        mread(fd, (genericptr_t)&regions[i]->leave_proc_is_on, sizeof(boolean));

        mread(fd, (genericptr_t) &regions[i]->time_to_live, sizeof(long));
        /* check for expired region */
        if (regions[i]->time_to_live >= 0L)
            regions[i]->time_to_live =
                (regions[i]->time_to_live > tmstamp) ? regions[i]->time_to_live - tmstamp : 0L;
        mread(fd, (genericptr_t) &regions[i]->player_flags,
              sizeof(unsigned int));
        if (ghostly) { /* settings pertained to old player */
            clear_hero_inside(regions[i]);
            clear_heros_fault(regions[i]);
        }
        mread(fd, (genericptr_t) &regions[i]->n_monst, sizeof(short));
        if (regions[i]->n_monst > 0)
            regions[i]->monsters =
                (unsigned *) alloc(sizeof(unsigned) * (size_t)regions[i]->n_monst);
        else
            regions[i]->monsters = (unsigned int *) 0;
        regions[i]->max_monst = regions[i]->n_monst;
        for (j = 0; j < regions[i]->n_monst; j++)
            mread(fd, (genericptr_t) &regions[i]->monsters[j],
                  sizeof(unsigned));
        mread(fd, (genericptr_t) &regions[i]->visible, sizeof(boolean));
        mread(fd, (genericptr_t) &regions[i]->glyph, sizeof(int));
        mread(fd, (genericptr_t) &regions[i]->arg, sizeof(anything));
        mread(fd, (genericptr_t) &regions[i]->typ, sizeof(enum region_types));
        mread(fd, (genericptr_t) &regions[i]->dmg_dice, sizeof(int));
        mread(fd, (genericptr_t) &regions[i]->dmg_diesize, sizeof(int));
        mread(fd, (genericptr_t) &regions[i]->dmg_plus, sizeof(int));
        mread(fd, (genericptr_t) &regions[i]->dmg_adjustment, sizeof(double));
        mread(fd, (genericptr_t) &regions[i]->region_flags, sizeof(unsigned long));
        mread(fd, (genericptr_t) &regions[i]->lamplit, sizeof(boolean));
        mread(fd, (genericptr_t) &regions[i]->makingsound, sizeof(boolean));
    }
    /* remove expired regions, do not trigger the expire_f callback (yet!);
       also update monster lists if this data is coming from a bones file */
    for (i = n_regions - 1; i >= 0; i--)
        if (regions[i]->time_to_live == 0L)
            remove_region(regions[i]);
        else if (ghostly && regions[i]->n_monst > 0)
            reset_region_mids(regions[i]);
}

/* to support '#stats' wizard-mode command */
void
region_stats(hdrfmt, hdrbuf, count, size)
const char *hdrfmt;
char *hdrbuf;
long* count;
size_t *size;
{
    NhRegion *rg;
    int i;

    /* other stats formats take one parameter; this takes two */
    Sprintf(hdrbuf, hdrfmt, (long) sizeof (NhRegion), sizeof (NhRect));
    *count = (long) n_regions; /* might be 0 even though max_regions isn't */
    *size = (size_t) max_regions * sizeof (NhRegion);
    for (i = 0; i < n_regions; ++i) {
        rg = regions[i];
        *size += (size_t) rg->nrects * sizeof (NhRect);
        if (rg->enter_msg)
            *size += strlen(rg->enter_msg) + 1;
        if (rg->leave_msg)
            *size += strlen(rg->leave_msg) + 1;
        *size += (size_t) rg->max_monst * sizeof *rg->monsters;
    }
    /* ? */
}

/* update monster IDs for region being loaded from bones; `ghostly' implied */
STATIC_OVL void
reset_region_mids(reg)
NhRegion *reg;
{
    int i = 0, n = reg->n_monst;
    unsigned *mid_list = reg->monsters;

    while (i < n)
        if (!lookup_id_mapping(mid_list[i], &mid_list[i])) {
            /* shrink list to remove missing monster; order doesn't matter */
            mid_list[i] = mid_list[--n];
        } else {
            /* move on to next monster */
            ++i;
        }
    reg->n_monst = n;
    return;
}

#if 0
/* not yet used */

/*--------------------------------------------------------------*
 *                                                              *
 *                      Create Region with just a message       *
 *                                                              *
 *--------------------------------------------------------------*/

NhRegion *
create_msg_region(x, y, w, h, msg_enter, msg_leave)
xchar x, y;
xchar w, h;
const char *msg_enter;
const char *msg_leave;
{
    NhRect tmprect;
    NhRegion *reg = create_region(REGION_GENERAL, (NhRect *) 0, 0);

    if (msg_enter)
    {
        reg->enter_msg = dupstr(msg_enter);
        reg->enter_msg_is_on = TRUE;
    }
    
    if (msg_leave)
    {
        reg->leave_msg = dupstr(msg_leave);
        reg->leave_msg_is_on = TRUE;

    }

    tmprect.lx = x;
    tmprect.ly = y;
    tmprect.hx = x + w;
    tmprect.hy = y + h;
    add_rect_to_reg(reg, &tmprect);
    reg->time_to_live = -1L;
    return reg;
}


/*--------------------------------------------------------------*
 *                                                              *
 *                      Force Field Related Cod                 *
 *                      (unused yet)                            *
 *--------------------------------------------------------------*/

boolean
enter_force_field(p1, p2)
genericptr_t p1;
genericptr_t p2;
{
    struct monst *mtmp;

    if (p2 == (genericptr_t) 0) { /* That means the player */
        play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
        if (!Blind)
            You("bump into %s.  Ouch!",
                Hallucination ? "an invisible tree"
                              : "some kind of invisible wall");
        else
            pline("Ouch!");
    } else {
        mtmp = (struct monst *) p2;
        if (canseemon(mtmp))
            pline("%s bumps into %s!", Monnam(mtmp), something);
    }
    return FALSE;
}

NhRegion *
create_force_field(x, y, radius, time_to_live)
xchar x, y;
int radius;
long time_to_live;
{
    int i;
    NhRegion *ff;
    int nrect;
    NhRect tmprect;

    ff = create_region(REGION_FORCE_FIELD_TRANSPARENT, (NhRect *) 0, 0);
    nrect = radius;
    tmprect.lx = x;
    tmprect.hx = x;
    tmprect.ly = y - (radius - 1);
    tmprect.hy = y + (radius - 1);
    for (i = 0; i < nrect; i++) {
        add_rect_to_reg(ff, &tmprect);
        tmprect.lx--;
        tmprect.hx++;
        tmprect.ly++;
        tmprect.hy--;
    }
    ff->time_to_live = time_to_live;
    if (!in_mklev && !context.mon_moving)
        set_heros_fault(ff); /* assume player has created it */
    add_region(ff);
    return ff;
}

#endif /*0*/

/*--------------------------------------------------------------*
 *                                                              *
 *                      Gas cloud related code                  *
 *                                                              *
 *--------------------------------------------------------------*/

/*
 * Here is an example of an expire function that may prolong
 * region life after some mods...
 */
/*ARGSUSED*/
boolean
expire_gas_cloud(p1, p2)
genericptr_t p1;
genericptr_t p2 UNUSED;
{
    NhRegion *reg;

    reg = (NhRegion *) p1;

    /* If it was a thick cloud, it dissipates a little first */
    if (reg->dmg_adjustment >= 0.5)
    { /* Do it twice */
        reg->dmg_adjustment /= 2.0;
        reg->time_to_live = 2L; /* Here's the trick : reset time_to_live */
        return FALSE;  /* THEN return FALSE, means "still there" */
    }
    return TRUE; /* OK, it's gone, you can free it! */
}

boolean
inside_gas_cloud(p1, p2)
genericptr_t p1;
genericptr_t p2;
{
    NhRegion *reg;
    struct monst *mtmp;
    int dam, dmg_n, dmg_d, dmg_p;
    double expiration_adjustment;

    /*
     * Gas clouds can't be targetted at water locations, but they can
     * start next to water and spread over it.
     */

    reg = (NhRegion *) p1;
    expiration_adjustment = reg->dmg_adjustment;
    dmg_n = reg->dmg_dice;
    dmg_d = reg->dmg_diesize;
    dmg_p = reg->dmg_plus;
    dam = dd(dmg_n, dmg_d, dmg_p);
    dam = (int)((double)dam * expiration_adjustment);

    if (p2 == (genericptr_t) 0) 
    { /* This means *YOU* Bozo! */
        if (u.uinvulnerable || is_not_living(youmonst.data) || Breathless
            || Underwater)
            return FALSE;
        
        if (!Blind) 
        {
            Your("%s sting.", makeplural(body_part(EYE)));
            make_blinded(1L, FALSE);
        }

        if (!Poison_resistance && dam > 0) 
        {
            pline("%s is burning your %s!", Something,
                  makeplural(body_part(LUNG)));
            You("cough and spit blood!");
            losehp(adjust_damage(dam, (struct monst*)0, &youmonst, AD_DRST, ADFLAGS_NONE), "gas cloud", KILLED_BY_AN);
            return FALSE;
        } 
        else 
        {
            You("cough!");
            return FALSE;
        }
    } 
    else 
    { /* A monster is inside the cloud */
        mtmp = (struct monst *) p2;

        /* Non living and non breathing monsters are not concerned;
           adult green dragon is not affected by gas cloud, baby one is */
        if (!(is_not_living(mtmp->data) || is_vampshifter(mtmp))
            && !has_innate_breathless(mtmp->data)
            /* not is_swimmer(); assume that non-fish are swimming on
               the surface and breathing the air above it periodically
               unless located at water spot on plane of water */
            && !((mtmp->data->mlet == S_EEL || Is_waterlevel(&u.uz))
                 && is_pool(mtmp->mx, mtmp->my))
            /* exclude monsters with poison gas breath attack:
               adult green dragon and Chromatic Dragon (and iron golem,
               but is_not_living() and has_innate_breathless() tests also catch that) */
            && !(attacktype_fordmg(mtmp->data, AT_BREA, AD_DRST)
                 || attacktype_fordmg(mtmp->data, AT_BREA, AD_RBRE) || attacktype_fordmg(mtmp->data, AT_BREA, AD_RBGD)))
        {
            if (cansee(mtmp->mx, mtmp->my))
                pline("%s coughs!", Monnam(mtmp));
            if (heros_fault(reg))
                setmangry(mtmp, TRUE);

            if (haseyes(mtmp->data) && !is_blinded(mtmp))
            {
                nonadditive_increase_mon_property(mtmp, BLINDED, 5 + rnd(5));
            }

            if (resists_poison(mtmp))
                return FALSE;

            deduct_monster_hp(mtmp, adjust_damage(dam, (struct monst*)0, mtmp, AD_DRST, ADFLAGS_NONE));

            if (DEADMONSTER(mtmp)) 
            {
                if (heros_fault(reg))
                    killed(mtmp);
                else
                    monkilled(mtmp, "gas cloud", AD_DRST);

                if (DEADMONSTER(mtmp)) 
                { /* not lifesaved */
                    return TRUE;
                }
            }
        }
    }
    return FALSE; /* Monster is still alive */
}

NhRegion *
create_gas_cloud(x, y, type, radius, dmg_dice, dmg_diesize, dmg_plus)
xchar x, y;
enum region_types type;
int radius;
int dmg_dice, dmg_diesize, dmg_plus;
{
    NhRegion *cloud;
    int i, nrect;
    NhRect tmprect;

    cloud = create_region(type, (NhRect *) 0, 0);
    nrect = radius;
    tmprect.lx = x;
    tmprect.hx = x;
    tmprect.ly = y - (radius - 1);
    tmprect.hy = y + (radius - 1);
    for (i = 0; i < nrect; i++) {
        add_rect_to_reg(cloud, &tmprect);
        tmprect.lx--;
        tmprect.hx++;
        tmprect.ly++;
        tmprect.hy--;
    }
    cloud->time_to_live = rn1(3, 4);
    if (!in_mklev && !context.mon_moving)
        set_heros_fault(cloud); /* assume player has created it */
    cloud->arg = zeroany; /* Unused */
    cloud->dmg_dice = dmg_dice;
    cloud->dmg_diesize = dmg_diesize;
    cloud->dmg_plus = dmg_plus;
    cloud->dmg_adjustment = 1.0; /* Might go down towards dissipation */
    add_region(cloud);
    return cloud;
}

NhRegion*
create_simple_permanent_region(x1, y1, x2, y2, type)
xchar x1, y1, x2, y2;
enum region_types type;
{
    NhRegion* reg;
    NhRect tmprect;

    reg = create_region(type, (NhRect*)0, 0);
    tmprect.lx = x1;
    tmprect.hx = x2;
    tmprect.ly = y1;
    tmprect.hy = y2;
    add_rect_to_reg(reg, &tmprect);
    reg->arg = zeroany; /* Unused */
    add_region(reg);
    return reg;
}

/* for checking troubles during prayer; is hero at risk? */
boolean
region_danger()
{
    int i, n = 0; //f_indx, 

    for (i = 0; i < n_regions; i++) {
        /* only care about regions that hero is in */
        if (!hero_inside(regions[i]))
            continue;
        /* the only type of region we understand is gas_cloud */
        if (regions[i]->typ == REGION_POISON_GAS) {
            /* completely harmless if you don't need to breathe */
            if (is_not_living(youmonst.data) || Breathless)
                continue;
            /* minor inconvenience if you're poison resistant;
               not harmful enough to be a prayer-level trouble */
            if (Poison_resistance)
                continue;
            ++n;
        }
    }
    return n ? TRUE : FALSE;
}

/* for fixing trouble at end of prayer;
   danger detected at start of prayer might have expired by now */
void
region_safety()
{
    NhRegion *r = 0;
    int i, n = 0; //f_indx, 

    for (i = 0; i < n_regions; i++) {
        /* only care about regions that hero is in */
        if (!hero_inside(regions[i]))
            continue;

        /* the only type of region we understand is gas_cloud */
        if (regions[i]->typ == REGION_POISON_GAS) {
            if (!n++ && regions[i]->time_to_live >= 0)
                r = regions[i];
        }
    }

    if (n > 1 || (n == 1 && !r)) {
        /* multiple overlapping cloud regions or non-expiring one */
        safe_teleds(FALSE, FALSE);
    } else if (r) {
        remove_region(r);
        pline_The("gas cloud enveloping you dissipates.");
    } else {
        /* cloud dissipated on its own, so nothing needs to be done */
        pline_The("gas cloud has dissipated.");
    }
    /* maybe cure blindness too */
    if ((Blinded & TIMEOUT) == 1L)
        make_blinded(0L, TRUE);
}

struct nhregion* 
find_rid(nid)
int nid;
{
    if (nid < 0 || nid >= n_regions)
        return (struct nhregion*)0;
    
    return regions[nid];
}

int
get_rid(reg)
struct nhregion* reg;
{
    if (!reg)
        return -1;

    for (int i = 0; i < n_regions; i++)
    {
        if (regions[i] == reg)
            return i;
    }

    return -1;
}


void
do_light_regions(cs_rows)
char** cs_rows;
{
    char* row;
    for (int i = 0; i < n_regions; i++)
    {
        struct nhregion* reg = regions[i];
        if (!reg)
            break;
        if (!reg->lamplit)
            continue;

        for (int j = 0; j < reg->nrects; j++)
        {
            for (int y = reg->rects[j].ly; y <= reg->rects[j].hy; y++)
            {
                row = cs_rows[y];
                for (int x = reg->rects[j].lx; x <= reg->rects[j].hx; x++)
                {
                    if (row[x] & COULD_SEE)
                        row[x] |= ((region_type_definitions[reg->typ].is_light_source < 0) ? TEMP_MAGICAL_DARKNESS : TEMP_LIT);
                }
            }
        }
    }
}

boolean
enter_special_level_seen(p1, p2)
genericptr_t p1;
genericptr_t p2;
{
    NhRegion* reg;
    reg = (NhRegion*)p1;

    if (p2 == (genericptr_t)0) /* You */
    { 
        set_special_level_seen(&u.uz, FALSE);
        reg->enter_proc_is_on = FALSE;
        return TRUE;
    }
    return FALSE;
}


boolean
enter_special_level_true_nature_revealed(p1, p2)
genericptr_t p1;
genericptr_t p2;
{
    NhRegion* reg;
    reg = (NhRegion*)p1;

    if (p2 == (genericptr_t)0) /* You */
    {
        set_special_level_seen(&u.uz, TRUE);
        reg->enter_proc_is_on = FALSE;
        return TRUE;
    }
    return FALSE;
}

/*region.c*/

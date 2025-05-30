/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    detect.c    $NHDT-Date: 1544437284 2018/12/10 10:21:24 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.91 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * Detection routines, including crystal ball, magic mapping, and search
 * command.
 */

#include "hack.h"

extern boolean known; /* from read.c */

STATIC_DCL boolean NDECL(unconstrain_map);
STATIC_DCL void NDECL(reconstrain_map);
STATIC_DCL void FDECL(browse_map, (int, const char *));
STATIC_DCL void FDECL(map_monst, (struct monst *, BOOLEAN_P));
STATIC_DCL void FDECL(do_dknown_of, (struct obj *));
STATIC_DCL boolean FDECL(check_map_spot, (int, int, CHAR_P, unsigned));
STATIC_DCL boolean FDECL(clear_stale_map, (CHAR_P, unsigned));
STATIC_DCL void FDECL(sense_trap, (struct trap *, XCHAR_P, XCHAR_P, int));
STATIC_DCL int FDECL(detect_obj_traps, (struct obj *, BOOLEAN_P, int));
STATIC_DCL void FDECL(show_map_spot, (int, int));
STATIC_PTR void FDECL(findone, (int, int, genericptr_t));
STATIC_PTR void FDECL(openone, (int, int, genericptr_t));
STATIC_DCL int FDECL(mfind0, (struct monst *, BOOLEAN_P));
STATIC_DCL int FDECL(reveal_terrain_getglyph, (int, int, int,
                                               BOOLEAN_P, int, int));

/* bring hero out from underwater or underground or being engulfed;
   return True iff any change occurred */
STATIC_OVL boolean
unconstrain_map()
{
    boolean res = u.uinwater || u.uburied || u.uswallow;

    /* bring Underwater, buried, or swallowed hero to normal map */
    iflags.save_uinwater = u.uinwater, u.uinwater = 0;
    iflags.save_uburied  = u.uburied,  u.uburied  = 0;
    iflags.save_uswallow = u.uswallow, u.uswallow = 0;

    return res;
}

/* put hero back underwater or underground or engulfed */
STATIC_OVL void
reconstrain_map()
{
    u.uinwater = iflags.save_uinwater, iflags.save_uinwater = 0;
    u.uburied  = iflags.save_uburied,  iflags.save_uburied  = 0;
    u.uswallow = iflags.save_uswallow, iflags.save_uswallow = 0;
}

/* use getpos()'s 'autodescribe' to view whatever is currently shown on map */
STATIC_DCL void
browse_map(ter_typ, ter_explain)
int ter_typ;
const char *ter_explain;
{
    coord dummy_pos; /* don't care whether player actually picks a spot */
    boolean save_autodescribe;

    dummy_pos.x = u.ux, dummy_pos.y = u.uy; /* starting spot for getpos() */
    save_autodescribe = iflags.autodescribe;
    iflags.autodescribe = TRUE;
    iflags.terrainmode = ter_typ;
    getpos(&dummy_pos, FALSE, ter_explain, CURSOR_STYLE_LOOK_CURSOR);
    iflags.terrainmode = 0;
    iflags.autodescribe = save_autodescribe;
}

/* extracted from monster_detection() so can be shared by do_vicinity_map() */
STATIC_DCL void
map_monst(mtmp, showtail)
struct monst *mtmp;
boolean showtail;
{
    show_monster_glyph_with_extra_info(mtmp->mx, mtmp->my, any_mon_to_glyph(mtmp, newsym_rn2), mtmp, 0UL, 0UL, 0, 0);
    check_special_level_naming_by_mon(mtmp);
    if (showtail && is_long_worm_with_tail(mtmp->data))
        detect_wsegs(mtmp, 0);
}

/* this is checking whether a trap symbol represents a trapped chest,
   not whether a trapped chest is actually present */
boolean
trapped_chest_at(ttyp, x, y)
int ttyp;
int x, y;
{
    struct monst *mtmp;
    struct obj *otmp;

    if (!glyph_is_trap(glyph_at(x, y)))
        return FALSE;
    if (ttyp != BEAR_TRAP || (Hallucination && rn2(20)))
        return FALSE;

    /*
     * TODO?  We should check containers recursively like the trap
     * detecting routine does.  Chests and large boxes do not nest in
     * themselves or each other, but could be contained inside statues.
     *
     * For farlook, we should also check for buried containers, but
     * for '^' command to examine adjacent trap glyph, we shouldn't.
     */

    /* on map, presence of any trappable container will do */
    if (any_obj_at(CHEST, x, y) || any_obj_at(LARGE_BOX, x, y))
        return TRUE;

    /* in inventory, we need to find one which is actually trapped */
    if (x == u.ux && y == u.uy) {
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (Is_box(otmp) && otmp->otrapped)
                return TRUE;
        if (u.usteed) { /* steed isn't on map so won't be found by m_at() */
            for (otmp = u.usteed->minvent; otmp; otmp = otmp->nobj)
                if (Is_box(otmp) && otmp->otrapped)
                    return TRUE;
        }
    }
    if ((mtmp = m_at(x, y)) != 0)
        for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
            if (Is_box(otmp) && otmp->otrapped)
                return TRUE;
    return FALSE;
}

/* this is checking whether a trap symbol represents a trapped door,
   not whether the door here is actually trapped */
boolean
trapped_door_at(ttyp, x, y)
int ttyp;
int x, y;
{
    struct rm *lev;

    if (!glyph_is_trap(glyph_at(x, y)))
        return FALSE;
    if (ttyp != BEAR_TRAP || (Hallucination && rn2(20)))
        return FALSE;
    lev = &levl[x][y];
    if (!IS_DOOR(lev->typ))
        return FALSE;
    if ((lev->doormask & (D_NODOOR | D_BROKEN | D_ISOPEN | D_PORTCULLIS)) != 0
         && trapped_chest_at(ttyp, x, y))
        return FALSE;
    return TRUE;
}

/* recursively search obj for an object in class oclass, return 1st found */
struct obj *
o_in(obj, oclass)
struct obj *obj;
char oclass;
{
    register struct obj *otmp;
    struct obj *temp;

    if (obj->oclass == oclass)
        return obj;
    /*
     * Note:  we exclude SchroedingersBox because the corpse it contains
     * isn't necessarily a corpse yet.  Resolving the status would lead
     * to complications if it turns out to be a live cat.  We know that
     * that Box can't contain anything else because putting something in
     * would resolve the cat/corpse situation and convert to ordinary box.
     */
    if (Has_contents(obj) && !SchroedingersBox(obj)) {
        for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
            if (otmp->oclass == oclass)
                return otmp;
            else if (Has_contents(otmp) && (temp = o_in(otmp, oclass)) != 0)
                return temp;
    }
    return (struct obj *) 0;
}

/* Recursively search obj for an object made of specified material.
 * Return first found.
 */
struct obj *
o_material(obj, material)
struct obj *obj;
unsigned material;
{
    register struct obj *otmp;
    struct obj *temp;

    if (obj->material == material)
        return obj;

    if (Has_contents(obj)) {
        for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
            if (otmp->material == material)
                return otmp;
            else if (Has_contents(otmp)
                     && (temp = o_material(otmp, material)) != 0)
                return temp;
    }
    return (struct obj *) 0;
}

STATIC_OVL void
do_dknown_of(obj)
struct obj *obj;
{
    struct obj *otmp;

    obj->dknown = 1;
    if (Has_contents(obj)) {
        for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
            do_dknown_of(otmp);
    }
}

/* Check whether the location has an outdated object displayed on it. */
STATIC_OVL boolean
check_map_spot(x, y, oclass, material)
int x, y;
char oclass;
unsigned material;
{
    int glyph;
    register struct obj *otmp;
    register struct monst *mtmp;

    glyph = glyph_at(x, y);
    if (glyph_is_object(glyph)) {
        /* there's some object shown here */
        if (oclass == ALL_CLASSES) {
            return (boolean) !(level.objects[x][y] /* stale if nothing here */
                               || ((mtmp = m_at(x, y)) != 0 && mtmp->minvent));
        } else {
            if (material
                && objects[glyph_to_obj(glyph)].oc_material == material) {
                /* object shown here is of interest because material matches */
                for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
                    if (o_material(otmp, MAT_GOLD))
                        return FALSE;
                /* didn't find it; perhaps a monster is carrying it */
                if ((mtmp = m_at(x, y)) != 0) {
                    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
                        if (o_material(otmp, MAT_GOLD))
                            return FALSE;
                }
                /* detection indicates removal of this object from the map */
                return TRUE;
            }
            if (oclass && objects[glyph_to_obj(glyph)].oc_class == oclass) {
                /* obj shown here is of interest because its class matches */
                for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
                    if (o_in(otmp, oclass))
                        return FALSE;
                /* didn't find it; perhaps a monster is carrying it */
                if ((mtmp = m_at(x, y)) != 0) {
                    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
                        if (o_in(otmp, oclass))
                            return FALSE;
                }
                /* detection indicates removal of this object from the map */
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*
 * When doing detection, remove stale data from the map display (corpses
 * rotted away, objects carried away by monsters, etc) so that it won't
 * reappear after the detection has completed.  Return true if noticeable
 * change occurs.
 */
STATIC_OVL boolean
clear_stale_map(oclass, material)
char oclass;
unsigned material;
{
    register int zx, zy;
    boolean change_made = FALSE;

    for (zx = 1; zx < COLNO; zx++)
        for (zy = 0; zy < ROWNO; zy++)
            if (check_map_spot(zx, zy, oclass, material)) 
            {
                unmap_object(zx, zy);
                change_made = TRUE;
            }

    return change_made;
}

/* look for gold, on the floor or in monsters' possession */
int
gold_detect(sobj)
register struct obj *sobj;
{
    register struct obj *obj;
    register struct monst *mtmp;
    struct obj gold, *temp = 0;
    boolean stale, ugold = FALSE, steedgold = FALSE;
    int ter_typ = TER_DETECT | TER_OBJ;

    known = stale = clear_stale_map(COIN_CLASS,
                                    (unsigned) (sobj->blessed ? MAT_GOLD : 0));

    /* look for gold carried by monsters (might be in a container) */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        if (DEADMONSTER(mtmp))
            continue; /* probably not needed in this case but... */

        if (findgold(mtmp->minvent) || mtmp->mnum == PM_GOLD_GOLEM) 
        {
            if (mtmp == u.usteed) 
            {
                steedgold = TRUE;
            } 
            else
            {
                known = TRUE;
                goto outgoldmap; /* skip further searching */
            }
        }
        else 
        {
            for (obj = mtmp->minvent; obj; obj = obj->nobj)
                if ((sobj->blessed && o_material(obj, MAT_GOLD))
                    || o_in(obj, COIN_CLASS)) 
                {
                    if (mtmp == u.usteed)
                    {
                        steedgold = TRUE;
                    } 
                    else 
                    {
                        known = TRUE;
                        goto outgoldmap; /* skip further searching */
                    }
                }
        }
    }

    /* look for gold objects */
    for (obj = fobj; obj; obj = obj->nobj)
    {
        if (sobj->blessed && o_material(obj, MAT_GOLD)) 
        {
            known = TRUE;
            if (obj->ox != u.ux || obj->oy != u.uy)
                goto outgoldmap;
        }
        else if (o_in(obj, COIN_CLASS)) 
        {
            known = TRUE;
            if (obj->ox != u.ux || obj->oy != u.uy)
                goto outgoldmap;
        }
    }

    if (!known) 
    {
        /* no gold found on floor or monster's inventory.
           adjust message if you have gold in your inventory */
        if (sobj)
        {
            char buf[BUFSZ];

            if (youmonst.data == &mons[PM_GOLD_GOLEM])
                Sprintf(buf, "You feel like a million %s!", currency(2L));
            else if (money_cnt(invent) || hidden_gold())
                Strcpy(buf,
                   "You feel worried about your future financial situation.");
            else if (steedgold)
                Sprintf(buf, "You feel interested in %s financial situation.",
                        s_suffix(x_monnam(u.usteed, is_tame(u.usteed) ? ARTICLE_YOUR : ARTICLE_THE,
                                          (char *) 0, SUPPRESS_SADDLE, FALSE)));
            else
                Strcpy(buf, "You feel materially poor.");

            strange_feeling(sobj, buf, FALSE);
        }
        return 1;
    }
    /* only under me - no separate display required */
    if (stale)
        docrt();
    You("notice some gold between your %s.", makeplural(body_part(FOOT)));
    return 0;

outgoldmap:
    cls();
    //double scale_before = flags.screen_scale_adjustment;
    issue_simple_gui_command(GUI_CMD_SAVE_ZOOM);
    dozoommini();
    (void) unconstrain_map();

    /* Discover gold locations. */
    for (obj = fobj; obj; obj = obj->nobj) 
    {
        if (sobj->blessed && (temp = o_material(obj, MAT_GOLD)) != 0)
        {
            if (temp != obj)
            {
                temp->ox = obj->ox;
                temp->oy = obj->oy;
            }
            map_object_for_detection(temp, 1);
        }
        else if ((temp = o_in(obj, COIN_CLASS)) != 0) 
        {
            if (temp != obj)
            {
                temp->ox = obj->ox;
                temp->oy = obj->oy;
            }
            map_object_for_detection(temp, 1);
        }
        if (temp && temp->ox == u.ux && temp->oy == u.uy)
            ugold = TRUE;
    }

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue; /* probably overkill here */
        temp = 0;

        if (findgold(mtmp->minvent) || mtmp->mnum == PM_GOLD_GOLEM) 
        {
            gold = zeroobj; /* ensure oextra is cleared too */
            gold.otyp = GOLD_PIECE;
            gold.quan = (int64_t) rnd(10); /* usually more than 1 */
            gold.ox = mtmp->mx;
            gold.oy = mtmp->my;
            map_object_for_detection(&gold, 1);
            temp = &gold;
        } 
        else
        {
            for (obj = mtmp->minvent; obj; obj = obj->nobj)
                if (sobj->blessed && (temp = o_material(obj, MAT_GOLD)) != 0) 
                {
                    temp->ox = mtmp->mx;
                    temp->oy = mtmp->my;
                    map_object_for_detection(temp, 1);
                    break;
                }
                else if ((temp = o_in(obj, COIN_CLASS)) != 0) 
                {
                    temp->ox = mtmp->mx;
                    temp->oy = mtmp->my;
                    map_object_for_detection(temp, 1);
                    break;
                }
        }

        if (temp && temp->ox == u.ux && temp->oy == u.uy)
            ugold = TRUE;
    }

    if (!ugold) 
    {
        newsym(u.ux, u.uy);
        ter_typ |= TER_MON; /* so autodescribe will recognize hero */
    }
    show_memory_and_detection_everywhere();
    if (sobj)
        play_simple_object_sound(sobj, OBJECT_SOUND_TYPE_GENERAL_EFFECT);

    You_feel("very greedy, and sense gold!");
    exercise(A_WIS, TRUE);

    browse_map(ter_typ, "gold");

    reconstrain_map();
    issue_simple_gui_command(GUI_CMD_RESTORE_ZOOM);
    //zoomtoscale(scale_before);
    docrt();
    if (Underwater)
        under_water(2);
    if (u.uburied)
        under_ground(2);
    return 0;
}

/* returns 1 if nothing was detected, 0 if something was detected */
int
food_detect(sobj)
register struct obj *sobj;
{
    register struct obj *obj;
    register struct monst *mtmp;
    register int ct = 0, ctu = 0;
    boolean confused = (Confusion || (sobj && sobj->cursed)), stale;
    char oclass = confused ? POTION_CLASS : FOOD_CLASS;
    const char *what = confused ? something : "food";

    stale = clear_stale_map(oclass, 0);
    if (u.usteed) /* some situations leave steed with stale coordinates */
        u.usteed->mx = u.ux, u.usteed->my = u.uy;

    for (obj = fobj; obj; obj = obj->nobj)
        if (o_in(obj, oclass)) {
            if (obj->ox == u.ux && obj->oy == u.uy)
                ctu++;
            else
                ct++;
        }
    for (mtmp = fmon; mtmp && (!ct || !ctu); mtmp = mtmp->nmon) {
        /* no DEADMONSTER(mtmp) check needed -- dmons never have inventory */
        for (obj = mtmp->minvent; obj; obj = obj->nobj)
            if (o_in(obj, oclass)) {
                if (mtmp->mx == u.ux && mtmp->my == u.uy)
                    ctu++; /* steed or an engulfer with inventory */
                else
                    ct++;
                break;
            }
    }

    if (!ct && !ctu) {
        known = stale && !confused;
        if (stale) {
            docrt();
            You("sense a lack of %s nearby.", what);
            if (sobj && sobj->blessed) {
                if (!u.uedibility)
                    Your("%s starts to tingle.", body_part(NOSE));
                u.uedibility = 1;
            }
        } else if (sobj) {
            char buf[BUFSZ];

            Sprintf(buf, "Your %s twitches%s.", body_part(NOSE),
                    (sobj->blessed && !u.uedibility)
                        ? " then starts to tingle"
                        : "");
            if (sobj->blessed && !u.uedibility) {
                boolean savebeginner = flags.beginner;

                flags.beginner = FALSE; /* prevent non-delivery of message */
                strange_feeling(sobj, buf, FALSE);
                flags.beginner = savebeginner;
                u.uedibility = 1;
            } else
                strange_feeling(sobj, buf, FALSE);
        }
        return !stale;
    } else if (!ct) {
        known = TRUE;
        You("%s %s nearby.", sobj ? "smell" : "sense", what);
        if (sobj && sobj->blessed) {
            if (!u.uedibility)
                pline("Your %s starts to tingle.", body_part(NOSE));
            u.uedibility = 1;
        }
    } else {
        struct obj *temp;
        int ter_typ = TER_DETECT | TER_OBJ;

        known = TRUE;
        cls();
        //double scale_before = flags.screen_scale_adjustment;
        issue_simple_gui_command(GUI_CMD_SAVE_ZOOM);
        dozoommini();

        (void) unconstrain_map();
        for (obj = fobj; obj; obj = obj->nobj)
            if ((temp = o_in(obj, oclass)) != 0) {
                if (temp != obj) {
                    temp->ox = obj->ox;
                    temp->oy = obj->oy;
                }
                map_object_for_detection(temp, 1);
            }
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
            /* no DEADMONSTER() check needed -- dmons never have inventory */
            for (obj = mtmp->minvent; obj; obj = obj->nobj)
                if ((temp = o_in(obj, oclass)) != 0) {
                    temp->ox = mtmp->mx;
                    temp->oy = mtmp->my;
                    map_object_for_detection(temp, 1);
                    break; /* skip rest of this monster's inventory */
                }
        if (!ctu) {
            newsym(u.ux, u.uy);
            ter_typ |= TER_MON; /* for autodescribe of self */
        }
        show_memory_and_detection_everywhere();

        if (sobj) {
            if (sobj->blessed) {
                Your("%s %s to tingle and you smell %s.", body_part(NOSE),
                     u.uedibility ? "continues" : "starts", what);
                u.uedibility = 1;
            } else
                Your("%s tingles and you smell %s.", body_part(NOSE), what);
        } else
            You("sense %s.", what);
        exercise(A_WIS, TRUE);
        if (sobj)
            play_simple_object_sound(sobj, OBJECT_SOUND_TYPE_GENERAL_EFFECT);

        browse_map(ter_typ, "food");

        reconstrain_map();
        issue_simple_gui_command(GUI_CMD_RESTORE_ZOOM);
        //zoomtoscale(scale_before);
        docrt();
        if (Underwater)
            under_water(2);
        if (u.uburied)
            under_ground(2);
    }
    return 0;
}

/*
 * Used for scrolls, potions, spells, and crystal balls.  Returns:
 *
 *      1 - nothing was detected
 *      0 - something was detected
 */
int
object_detect(detector, class)
struct obj *detector; /* object doing the detecting */
int class;            /* an object class, 0 for all */
{
    register int x, y;
    char stuff[BUFSZ];
    int is_cursed = (detector && detector->cursed);
    int do_dknown = (detector && (detector->oclass == POTION_CLASS
                                  || detector->oclass == SPBOOK_CLASS)
                     && detector->blessed);
    int ct = 0, ctu = 0;
    register struct obj *obj, *otmp = (struct obj *) 0;
    register struct monst *mtmp;
    nhsym sym;
    char boulder = 0;
    int ter_typ = TER_DETECT | TER_OBJ;

    if (class < 0 || class >= MAX_OBJECT_CLASSES) {
        impossible("object_detect:  illegal class %d", class);
        class = 0;
    }

    /* Special boulder symbol check - does the class symbol happen
     * to match iflags.bouldersym which is a user-defined?
     * If so, that means we aren't sure what they really wanted to
     * detect. Rather than trump anything, show both possibilities.
     * We can exclude checking the buried obj chain for boulders below.
     */
    sym = (nhsym)(class ? def_oc_syms[class].sym : 0);
    if (sym && iflags.bouldersym && sym == iflags.bouldersym)
        boulder = ROCK_CLASS;

    if (Hallucination || (Confusion && class == SCROLL_CLASS))
        Strcpy(stuff, something);
    else
        Strcpy(stuff, class ? def_oc_syms[class].name : "objects");
    if (boulder && class != ROCK_CLASS)
        Strcat(stuff, " and/or large stones");

    if (do_dknown)
        for (obj = invent; obj; obj = obj->nobj)
            do_dknown_of(obj);

    for (obj = fobj; obj; obj = obj->nobj) {
        if ((!class && !boulder) || o_in(obj, class) || o_in(obj, boulder)) {
            if (obj->ox == u.ux && obj->oy == u.uy)
                ctu++;
            else
                ct++;
        }
        if (do_dknown)
            do_dknown_of(obj);
    }

    for (obj = level.buriedobjlist; obj; obj = obj->nobj) {
        if (!class || o_in(obj, class)) {
            if (obj->ox == u.ux && obj->oy == u.uy)
                ctu++;
            else
                ct++;
        }
        if (do_dknown)
            do_dknown_of(obj);
    }

    if (u.usteed)
        u.usteed->mx = u.ux, u.usteed->my = u.uy;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        for (obj = mtmp->minvent; obj; obj = obj->nobj) {
            if ((!class && !boulder) || o_in(obj, class)
                || o_in(obj, boulder))
                ct++;
            if (do_dknown)
                do_dknown_of(obj);
        }
        if ((is_cursed && M_AP_TYPE(mtmp) == M_AP_OBJECT
             && (!class || class == objects[mtmp->mappearance].oc_class))
            || (findgold(mtmp->minvent) && (!class || class == COIN_CLASS))) {
            ct++;
            break;
        }
    }

    if (!clear_stale_map(!class ? ALL_CLASSES : class, 0) && !ct) 
    {
        if (!ctu) {
            if (detector)
                strange_feeling(detector, "You feel a lack of something.", FALSE);
            return 1;
        }

        You("sense %s nearby.", stuff);
        return 0;
    }

    cls();
    //double scale_before = flags.screen_scale_adjustment;
    issue_simple_gui_command(GUI_CMD_SAVE_ZOOM);
    dozoommini();
    (void) unconstrain_map();
    /*
     *  Map all buried objects first.
     */
    for (obj = level.buriedobjlist; obj; obj = obj->nobj)
        if (!class || (otmp = o_in(obj, class)) != 0) {
            if (class) {
                if (otmp != obj) {
                    otmp->ox = obj->ox;
                    otmp->oy = obj->oy;
                }
                map_object_for_detection(otmp, 1);
            } else
                map_object_for_detection(obj, 1);
        }
    /*
     * If we are mapping all objects, map only the top object of a pile or
     * the first object in a monster's inventory.  Otherwise, go looking
     * for a matching object class and display the first one encountered
     * at each location.
     *
     * Objects on the floor override buried objects.
     */
    for (x = 1; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            for (obj = level.objects[x][y]; obj; obj = obj->nexthere)
                if ((!class && !boulder) || (otmp = o_in(obj, class)) != 0
                    || (otmp = o_in(obj, boulder)) != 0) {
                    if (class || boulder) {
                        if (otmp != obj) {
                            otmp->ox = obj->ox;
                            otmp->oy = obj->oy;
                        }
                        map_object_for_detection(otmp, 1);
                    } else
                        map_object_for_detection(obj, 1);
                    break;
                }

    /* Objects in the monster's inventory override floor objects. */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        for (obj = mtmp->minvent; obj; obj = obj->nobj)
            if ((!class && !boulder) || (otmp = o_in(obj, class)) != 0
                || (otmp = o_in(obj, boulder)) != 0) {
                if (!class && !boulder)
                    otmp = obj;
                otmp->ox = mtmp->mx; /* at monster location */
                otmp->oy = mtmp->my;
                map_object_for_detection(otmp, 1);
                break;
            }
        /* Allow a mimic to override the detected objects it is carrying. */
        if (is_cursed && M_AP_TYPE(mtmp) == M_AP_OBJECT
            && (!class || class == objects[mtmp->mappearance].oc_class)) {
            struct obj temp;
            if (has_mobj(mtmp))
            {
                temp = *MOBJ(mtmp);
            }
            else
            {
                temp = zeroobj;
                temp.otyp = mtmp->mappearance; /* needed for obj_to_glyph() */
                temp.corpsenm = PM_TENGU; /* if mimicing a corpse */
                temp.quan = 1L;
            }
            temp.ox = mtmp->mx;
            temp.oy = mtmp->my;
            map_object_for_detection(&temp, 1);
        } else if (findgold(mtmp->minvent)
                   && (!class || class == COIN_CLASS)) {
            struct obj gold;

            gold = zeroobj; /* ensure oextra is cleared too */
            gold.otyp = GOLD_PIECE;
            gold.quan = (int64_t) rnd(10); /* usually more than 1 */
            gold.ox = mtmp->mx;
            gold.oy = mtmp->my;
            map_object_for_detection(&gold, 1);
        }
    }
    if (!glyph_is_object(glyph_at(u.ux, u.uy))) {
        newsym(u.ux, u.uy);
        ter_typ |= TER_MON;
    }
    show_memory_and_detection_everywhere();
    You("detect the %s of %s.", ct ? "presence" : "absence", stuff);
    if (detector)
        play_simple_object_sound(detector, OBJECT_SOUND_TYPE_GENERAL_EFFECT);

    if (!ct)
    {
        create_context_menu(CREATE_CONTEXT_MENU_BLOCKING_WINDOW);
        display_nhwindow(WIN_MAP, TRUE);
        create_context_menu(CREATE_CONTEXT_MENU_NORMAL);
    }
    else
        browse_map(ter_typ, "object");

    reconstrain_map();
    issue_simple_gui_command(GUI_CMD_RESTORE_ZOOM);
    //zoomtoscale(scale_before);
    docrt(); /* this will correctly reset vision */
    if (Underwater)
        under_water(2);
    if (u.uburied)
        under_ground(2);
    return 0;
}

/*
 * Used by: crystal balls, potions, fountains
 *
 * Returns 1 if nothing was detected.
 * Returns 0 if something was detected.
 */
int
monster_detect(otmp, mclass)
register struct obj *otmp; /* detecting object (if any) */
int mclass;                /* monster class, 0 for all */
{
    register struct monst *mtmp;
    int mcnt = 0;

    /* Note: This used to just check fmon for a non-zero value
     * but in versions since 3.3.0 fmon can test TRUE due to the
     * presence of dmons, so we have to find at least one
     * with positive hit-points to know for sure.
     */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        if (!DEADMONSTER(mtmp)) {
            mcnt++;
            break;
        }

    if (!mcnt)
    {
        if (otmp)
            strange_feeling(otmp, Hallucination
                                      ? "You get the heebie jeebies."
                                      : "You feel threatened.", FALSE);
        return 1;
    } 
    else 
    {
        boolean unconstrained, woken = FALSE;
        unsigned swallowed = u.uswallow; /* before unconstrain_map() */

        cls();
        //double scale_before = flags.screen_scale_adjustment;
        issue_simple_gui_command(GUI_CMD_SAVE_ZOOM);
        dozoommini();

        unconstrained = unconstrain_map();
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if (!mclass || mtmp->data->mlet == mclass
                || (is_long_worm_with_tail(mtmp->data)
                    && mclass == S_WORM_TAIL))
                map_monst(mtmp, TRUE);

            if (otmp && otmp->cursed
                && !mon_can_move(mtmp))
            {
                mtmp->msleeping = 0;
                mtmp->mfrozen = mtmp->mstaying = 0;
                mtmp->mcanmove = 1;
                mtmp->mwantstomove = 1;
                mtmp->mprops[SLEEPING] = 0;
                mtmp->mprops[PARALYZED] = 0;
                woken = TRUE;
                refresh_m_tile_gui_info(mtmp, FALSE);
            }
        }
        show_detection_everywhere();

        if (!swallowed)
            display_self();
        You("sense the presence of monsters.");
        if (woken)
            pline("Monsters sense the presence of you.");
        if (otmp)
            play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_GENERAL_EFFECT);

        if ((otmp && otmp->blessed) && !unconstrained) {
            /* persistent detection--just show updated map */
            create_context_menu(CREATE_CONTEXT_MENU_BLOCKING_WINDOW);
            display_nhwindow(WIN_MAP, TRUE);
            create_context_menu(CREATE_CONTEXT_MENU_NORMAL);
        } else {
            /* one-shot detection--allow player to move cursor around and
               get autodescribe feedback */
            EDetect_monsters |= I_SPECIAL;
            browse_map(TER_DETECT | TER_MON, "monster of interest");
            EDetect_monsters &= ~I_SPECIAL;
        }

        reconstrain_map();
        issue_simple_gui_command(GUI_CMD_RESTORE_ZOOM);
        //zoomtoscale(scale_before);
        docrt(); /* redraw the screen to remove unseen monsters from map */
        if (Underwater)
            under_water(2);
        if (u.uburied)
            under_ground(2);
    }
    return 0;
}

STATIC_OVL void
sense_trap(trap, x, y, src_cursed)
struct trap *trap;
xchar x, y;
int src_cursed;
{
    if (Hallucination || src_cursed) {
        struct obj obj; /* fake object */

        obj = zeroobj;
        if (trap) {
            obj.ox = trap->tx;
            obj.oy = trap->ty;
        } else {
            obj.ox = x;
            obj.oy = y;
        }
        obj.otyp = !Hallucination ? GOLD_PIECE : random_object(rn2);
        obj.quan = (int64_t) ((obj.otyp == GOLD_PIECE) ? rnd(10)
                           : objects[obj.otyp].oc_merge ? rnd(2) : 1);
        obj.corpsenm = random_monster(rn2); /* if otyp == CORPSE */
        map_object_for_detection(&obj, 1);
    } else if (trap) {
        map_trap(trap, 1);
        trap->tseen = 1;
    } else { /* trapped door or trapped chest */
        struct trap temp_trap; /* fake trap */

        (void) memset((genericptr_t) &temp_trap, 0, sizeof temp_trap);
        temp_trap.tx = x;
        temp_trap.ty = y;
        temp_trap.ttyp = BEAR_TRAP; /* some kind of trap */
        map_trap(&temp_trap, 1);
    }
}

#define OTRAP_NONE 0  /* nothing found */
#define OTRAP_HERE 1  /* found at hero's location */
#define OTRAP_THERE 2 /* found at any other location */

/* check a list of objects for chest traps; return 1 if found at <ux,uy>,
   2 if found at some other spot, 3 if both, 0 otherwise; optionally
   update the map to show where such traps were found */
STATIC_OVL int
detect_obj_traps(objlist, show_them, how)
struct obj *objlist;
boolean show_them;
int how; /* 1 for misleading map feedback */
{
    struct obj *otmp;
    xchar x, y;
    int result = OTRAP_NONE;

    /*
     * TODO?  Display locations of unarmed land mine and beartrap objects.
     * If so, should they be displayed as objects or as traps?
     */

    for (otmp = objlist; otmp; otmp = otmp->nobj) {
        if (Is_box(otmp) && otmp->otrapped
            && get_obj_location(otmp, &x, &y, BURIED_TOO | CONTAINED_TOO)) {
            result |= (x == u.ux && y == u.uy) ? OTRAP_HERE : OTRAP_THERE;
            if (show_them)
                sense_trap((struct trap *) 0, x, y, how);
        }
        if (Has_contents(otmp))
            result |= detect_obj_traps(otmp->cobj, show_them, how);
    }
    return result;
}

/* the detections are pulled out so they can
 * also be used in the crystal ball routine
 * returns 1 if nothing was detected
 * returns 0 if something was detected
 */
int
trap_detect(sobj)
struct obj *sobj; /* detecting object */
{
    register struct trap *ttmp;
    struct monst *mon;
    int door, glyph, tr, ter_typ = TER_DETECT | TER_TRP | TER_OBJ;
    int cursed_src = sobj && sobj->oclass == SCROLL_CLASS && sobj->cursed; /* Other items are not affected by curse, since it would be too obvious */
    boolean found = FALSE;
    coord cc;

    if (u.usteed)
        u.usteed->mx = u.ux, u.usteed->my = u.uy;

    /* floor/ceiling traps */
    for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap) {
        if (ttmp->tx != u.ux || ttmp->ty != u.uy)
            goto outtrapmap;
        else
            found = TRUE;
    }
    /* chest traps (might be buried or carried) */
    if ((tr = detect_obj_traps(fobj, FALSE, 0)) != OTRAP_NONE) {
        if (tr & OTRAP_THERE)
            goto outtrapmap;
        else
            found = TRUE;
    }
    if ((tr = detect_obj_traps(level.buriedobjlist, FALSE, 0)) != OTRAP_NONE) {
        if (tr & OTRAP_THERE)
            goto outtrapmap;
        else
            found = TRUE;
    }
    for (mon = fmon; mon; mon = mon->nmon) {
        if (DEADMONSTER(mon))
            continue;
        if ((tr = detect_obj_traps(mon->minvent, FALSE, 0)) != OTRAP_NONE) {
            if (tr & OTRAP_THERE)
                goto outtrapmap;
            else
                found = TRUE;
        }
    }
    if (detect_obj_traps(invent, FALSE, 0) != OTRAP_NONE)
        found = TRUE;
    /* door traps */
    for (door = 0; door < doorindex; door++) {
        cc = doors[door];
        if (levl[cc.x][cc.y].doormask & D_TRAPPED) {
            if (cc.x != u.ux || cc.y != u.uy)
                goto outtrapmap;
            else
                found = TRUE;
        }
    }
    if (!found) {
        char buf[BUFSZ];

        Sprintf(buf, "Your %s stop itching.", makeplural(body_part(TOE)));
        strange_feeling(sobj, buf, FALSE);
        return 1;
    }
    /* traps exist, but only under me - no separate display required */
    Your("%s itch.", makeplural(body_part(TOE)));
    clear_memory_object_detection_marks();
    return 0;

outtrapmap:
    cls();
    //double scale_before = flags.screen_scale_adjustment;
    issue_simple_gui_command(GUI_CMD_SAVE_ZOOM);
    dozoommini();

    (void) unconstrain_map();
    /* show chest traps first, so that subsequent floor trap display
       will override if both types are present at the same location */
    (void) detect_obj_traps(fobj, TRUE, cursed_src);
    (void) detect_obj_traps(level.buriedobjlist, TRUE, cursed_src);
    for (mon = fmon; mon; mon = mon->nmon) {
        if (DEADMONSTER(mon))
            continue;
        (void) detect_obj_traps(mon->minvent, TRUE, cursed_src);
    }
    (void) detect_obj_traps(invent, TRUE, cursed_src);

    for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap)
        sense_trap(ttmp, 0, 0, cursed_src);

    for (door = 0; door < doorindex; door++) {
        cc = doors[door];
        if (levl[cc.x][cc.y].doormask & D_TRAPPED)
            sense_trap((struct trap *) 0, cc.x, cc.y, cursed_src);
    }

    /* redisplay hero unless sense_trap() revealed something at <ux,uy> */
    glyph = glyph_at(u.ux, u.uy);
    if (!(glyph_is_trap(glyph) || glyph_is_object(glyph))) {
        newsym(u.ux, u.uy);
        ter_typ |= TER_MON; /* for autodescribe at <u.ux,u.uy> */
    }
    show_detection_everywhere();
    You_feel("%s.", cursed_src ? "very greedy" : "entrapped");
    if (sobj)
        play_simple_object_sound(sobj, OBJECT_SOUND_TYPE_GENERAL_EFFECT);

    browse_map(ter_typ, "trap of interest");

    reconstrain_map();
    issue_simple_gui_command(GUI_CMD_RESTORE_ZOOM);
    //zoomtoscale(scale_before);
    docrt(); /* redraw the screen to remove unseen traps from the map */
    if (Underwater)
        under_water(2);
    if (u.uburied)
        under_ground(2);
    return 0;
}

const char *
level_distance(where)
d_level *where;
{
    register schar ll = depth(&u.uz) - depth(where);
    register boolean indun = (u.uz.dnum == where->dnum);

    STATIC_VAR const char far_away[] = "far away";
    STATIC_VAR const char far_below[] = "far below";
    STATIC_VAR const char away_below_you[] = "away below you";
    STATIC_VAR const char below_you[] = "below you";
    STATIC_VAR const char in_the_distance[] = "in the distance";
    STATIC_VAR const char just_below[] = "just below";
    STATIC_VAR const char far_above[] = "far above";
    STATIC_VAR const char away_above_you[] = "away above you";
    STATIC_VAR const char above_you[] = "above you";
    STATIC_VAR const char just_above[] = "just above";
    STATIC_VAR const char near_you[] = "near you";

    if (ll < 0) {
        if (ll < (-8 - rn2(3)))
            if (!indun)
                return far_away;
            else
                return far_below;
        else if (ll < -1)
            if (!indun)
                return away_below_you;
            else
                return below_you;
        else if (!indun)
            return in_the_distance;
        else
            return just_below;
    } else if (ll > 0) {
        if (ll > (8 + rn2(3)))
            if (!indun)
                return far_away;
            else
                return far_above;
        else if (ll > 1)
            if (!indun)
                return away_above_you;
            else
                return above_you;
        else if (!indun)
            return in_the_distance;
        else
            return just_above;
    } else if (!indun)
        return in_the_distance;
    else
        return near_you;
}

STATIC_VAR const struct {
    const char *what;
    d_level *where;
} level_detects[] = {
    { "Delphi", &oracle_level },
    { "Medusa's lair", &medusa_level },
    { "a castle", &stronghold_level },
    { "the Wizard of Yendor's tower", &wiz1_level },
};

void
use_crystal_ball(optr)
struct obj **optr;
{
    if (!optr)
        return;

    char ch;
    int oops;
    struct obj *obj = *optr;

    if (!obj)
        return;

    if (Blind)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Too bad you can't see %s.", the(xname(obj)));
        return;
    }

    obj->cooldownleft = objects[obj->otyp].oc_item_cooldown;

    oops = (rnd(20) > ACURR(A_INT) || obj->cursed);
    if (oops && (obj->charges > 0)) 
    {
        switch (rnd(4)) //obj->oartifact ? 4 : 5))
        {
        case 1:
            play_sfx_sound(SFX_TOO_MUCH_TO_COMPREHEND);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s too much to comprehend!", Tobjnam(obj, "are"));
            break;
        case 2:
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s you!", Tobjnam(obj, "confuse"));
            if (!Confusion)
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, rnd(100)), FALSE);
            break;
        case 3:
            if (!resists_blnd(&youmonst)) 
            {
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s your vision!", Tobjnam(obj, "damage"));
                if(!Blinded)
                    play_sfx_sound(SFX_ACQUIRE_BLINDNESS);

                make_blinded((Blinded & TIMEOUT) + (int64_t) rnd(100), FALSE);
                if (!Blind)
                    Your1(vision_clears);
            }
            else
            {
                play_sfx_sound(SFX_GENERAL_UNAFFECTED);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s your vision.", Tobjnam(obj, "assault"));
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "are unaffected!");
            }
            break;
        case 4:
            if (!Hallucination)
                play_sfx_sound(SFX_ACQUIRE_HALLUCINATION);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s your mind!", Tobjnam(obj, "zap"));
            (void) make_hallucinated(
                (HHallucination & TIMEOUT) + (int64_t) rnd(100), FALSE, 0L);
            break;
        case 5:
            play_sfx_sound(SFX_EXPLOSION_FIERY);
            play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_action(0);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s!", Tobjnam(obj, "explode"));
            Sprintf(priority_debug_buf_2, "use_crystal_ball: %d", obj->otyp);
            useup(obj);
            *optr = obj = 0; /* it's gone */
            /* physical damage cause by the shards and force */
            losehp(adjust_damage(rnd(30), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "exploding crystal ball",
                   KILLED_BY_AN);
            special_effect_wait_until_end(0);
            break;
        }
        if (obj)
            consume_obj_charge(obj, TRUE);
        return;
    }

    if (Hallucination) 
    {
        if (!obj->charges)
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY2);
            pline_multi_ex(ATR_NONE, CLR_MSG_HALLUCINATED, no_multiattrs, multicolor_buffer, "All you see is funky %s haze.", hcolor_multi_buf0((char *) 0));
        } 
        else 
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
            switch (rnd(6))
            {
            case 1:
                You_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "grok some groovy globs of incandescent lava.");
                break;
            case 2:
                pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "Whoa!  Psychedelic colors, %s!",
                      poly_gender() == 1 ? "babe" : "dude");
                break;
            case 3:
                pline_The_multi_ex(ATR_NONE, CLR_MSG_HALLUCINATED, no_multiattrs, multicolor_buffer, "crystal pulses with sinister %s light!",
                          hcolor_multi_buf0((char *) 0));
                break;
            case 4:
                You_see_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "goldfish swimming above fluorescent rocks.");
                break;
            case 5:
                You_see_ex(ATR_NONE, CLR_MSG_HALLUCINATED,
                    "tiny snowflakes spinning around a miniature farmhouse.");
                break;
            default:
                pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "Oh wow... like a kaleidoscope!");
                break;
            }
            consume_obj_charge(obj, TRUE);
        }
        return;
    }

    /* read a single character */
    if (flags.verbose)
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_ZAP);
        You("may look for an object or monster symbol.");
    }
    if (iflags.using_gui_tiles)
    {
        winid tmpwin;
        menu_item* selected;
        anything any;
        int n, choice;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_SIMPLE);
        any = zeroany; /* zero out all bits */

        int k;
        for (k = 0; k < MAX_MONSTER_CLASSES; k++)
        {
            if (strcmp(def_monsyms[k].explain, ""))
            {
                any.a_int = k + 1;
                add_menu(tmpwin, NO_GLYPH, &any, def_monsyms[k].sym == ' ' ? ',' : def_monsyms[k].sym, 0, ATR_NONE, NO_COLOR, def_monsyms[k].explain, MENU_UNSELECTED);
            }
        }
        for (k = 0; k < MAX_OBJECT_CLASSES; k++)
        {
            if (strcmp(def_oc_syms[k].explain, "") && def_oc_syms[k].sym != ILLOBJ_SYM && def_oc_syms[k].sym != '\0')
            {
                any.a_int = -(k + 1);
                add_menu(tmpwin, NO_GLYPH, &any, def_oc_syms[k].sym, 0, ATR_NONE, NO_COLOR, def_oc_syms[k].explain, MENU_UNSELECTED);
            }
        }

        /* Cmap, which sometimes do not work */
        any.a_int = MAX_MONSTER_CLASSES + S_arrow_trap + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_arrow_trap].sym, 0, ATR_NONE, NO_COLOR, "traps and portals", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_throne + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_throne].sym, 0, ATR_NONE, NO_COLOR, "thrones", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_altar + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_altar].sym, 0, ATR_NONE, NO_COLOR, "altars and anvils", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_dnstair + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_dnstair].sym, 0, ATR_NONE, NO_COLOR, "stairs and ladders down", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_upstair + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_upstair].sym, 0, ATR_NONE, NO_COLOR, "stairs and ladders up", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_tree + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_tree].sym, 0, ATR_NONE, NO_COLOR, "trees and sinks", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_fountain + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_fountain].sym, 0, ATR_NONE, NO_COLOR, "fountains", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_pool + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_pool].sym, 0, ATR_NONE, NO_COLOR, "water or lava", MENU_UNSELECTED);
        any.a_int = MAX_MONSTER_CLASSES + S_grave + 1;
        add_menu(tmpwin, NO_GLYPH, &any, defsyms[S_grave].sym, 0, ATR_NONE, NO_COLOR, "graves, braziers, and signposts", MENU_UNSELECTED);


        end_menu(tmpwin, "What do you look for?");
        n = select_menu(tmpwin, PICK_ONE, &selected);
        destroy_nhwindow(tmpwin);
        if (n > 0) {
            if (selected[0].item.a_int > MAX_MONSTER_CLASSES)
            {
                choice = selected[0].item.a_int - MAX_MONSTER_CLASSES - 1;
                ch = defsyms[choice].sym;
            }
            else if (selected[0].item.a_int > 0)
            {
                choice = selected[0].item.a_int - 1;
                ch = def_monsyms[choice].sym;
            }
            else if (selected[0].item.a_int < 0)
            {
                choice = -selected[0].item.a_int - 1;
                ch = def_oc_syms[choice].sym;
            }
            else
                ch = '\033';
            /* skip preselected entry if we have more than one item chosen */
            free((genericptr_t)selected);
        }
        else
            ch = '\033';
    }
    else
    {
        ch = yn_function("What do you look for?", (char*)0, '\0', (char*)0);
    }

    /* Don't filter out ' ' here; it has a use */
    if ((ch != def_monsyms[S_GHOST].sym) && index(quitchars, ch)) {
        if (flags.verbose)
            pline1(Never_mind);
        return;
    }
    You("peer into %s...", the(xname(obj)));
    //nomul(-rnd(10));
    //multi_reason = "gazing into a crystal ball";
    //nomovemsg = "";

    if (obj->charges <= 0)
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
        pline_The("vision is unclear.");
    } 
    else
    {
        int class, i;
        int ret = 0;

        makeknown(CRYSTAL_BALL);
        consume_obj_charge(obj, TRUE);

        /* special case: accept ']' as synonym for mimic
         * we have to do this before the def_char_to_objclass check
         */
        if (ch == DEF_MIMIC_DEF)
            ch = DEF_MIMIC;

        if ((class = def_char_to_objclass(ch)) != MAX_OBJECT_CLASSES)
            ret = object_detect(/* (struct obj *) 0*/ obj, class);
        else if ((class = def_char_to_monclass(ch)) != MAX_MONSTER_CLASSES)
            ret = monster_detect(/* (struct obj *) 0*/ obj, class);
        else if (iflags.bouldersym && iflags.bouldersym < 128 && (ch == (char)iflags.bouldersym))
            ret = object_detect(/* (struct obj *) 0*/ obj, ROCK_CLASS);
        else
            switch (ch) 
            {
            case '^':
                ret = trap_detect(/* (struct obj *) 0*/ obj);
                break;
            default:
                i = rn2(SIZE(level_detects));
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT);
                You_see("%s, %s.", level_detects[i].what,
                        level_distance(level_detects[i].where));
                ret = 0;
                break;
            }

        if (ret)
        {
            if (!rn2(100)) /* make them nervous */
            {
                play_sfx_sound(SFX_SURPRISE_ATTACK);
                You_see_ex(ATR_NONE, CLR_MSG_WARNING, "the Wizard of Yendor gazing out at you.");
            }
            else
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
                pline_The("vision is unclear.");
            }
        }
    }
    return;
}

STATIC_OVL void
show_map_spot(x, y)
register int x, y;
{
    struct rm *lev;
    struct trap *t;
    int oldglyph;

    if (Confusion && rn2(7))
        return;
    lev = &levl[x][y];

    lev->seenv = SVALL;

    /* Secret corridors are found, but not secret doors. */
    if (lev->typ == SCORR) 
    {
        transform_location_type(x, y, CORR, 0);
        unblock_vision_and_hearing_at_point(x, y);
    }

    /*
     * Force the real background, then if it's not furniture and there's
     * a known trap there, display the trap, else if there was an object
     * shown there, redisplay the object.  So during mapping, furniture
     * takes precedence over traps, which take precedence over objects,
     * opposite to how normal vision behaves.
     */
    oldglyph = glyph_at(x, y);
    //uint64_t oldlayerflags = lev->hero_memory_layers.layer_flags;

    if (level.flags.hero_memory)
    {
        magic_map_background(x, y, 0);
        newsym(x, y); /* show it, if not blocked */
    }
    else 
    {
        magic_map_background(x, y, 1); /* display it */
    }

    if (!IS_FURNITURE(lev->typ))
    {
        if ((t = t_at(x, y)) != 0 && t->tseen) 
        {
            map_trap(t, 1);
        } 
        else if (glyph_is_trap(oldglyph) || glyph_is_object(oldglyph)) 
        {
            show_glyph_ascii(x, y, oldglyph);
            if (glyph_is_trap(oldglyph))
                show_glyph_on_layer(x, y, oldglyph, LAYER_TRAP);
            /* Objects are not drawn on layers; they are in memory_objchn */

            if (level.flags.hero_memory)
            {
                lev->hero_memory_layers.glyph = oldglyph;
                if (glyph_is_trap(oldglyph))
                {
                    lev->hero_memory_layers.layer_glyphs[LAYER_TRAP] = oldglyph;
                    lev->hero_memory_layers.layer_gui_glyphs[LAYER_TRAP] = oldglyph;
                }
            }
        }
    }
}

void
do_mapping()
{
    register int zx, zy;
    boolean unconstrained;

    if(!level.flags.mapping_does_not_reveal_special)
        set_special_level_seen(&u.uz, FALSE);

    unconstrained = unconstrain_map();
    for (zx = 1; zx < COLNO; zx++)
    {
        for (zy = 0; zy < ROWNO; zy++)
            show_map_spot(zx, zy);
        for (zy = ROWNO - 1; zy >= 0; zy--)
            show_map_spot(zx, zy);
    }
    if (!level.flags.hero_memory || unconstrained) {
        flush_screen(1);                 /* flush temp screen */
        /* browse_map() instead of display_nhwindow(WIN_MAP, TRUE) */
        browse_map(TER_DETECT | TER_MAP | TER_TRP | TER_OBJ,
                   "anything of interest");
        docrt();
    }
    reconstrain_map();
    exercise(A_WIS, TRUE);
}

/* clairvoyance */
void
do_vicinity_map(sobj)
struct obj *sobj; /* scroll--actually fake spellbook--object */
{
    register int zx, zy;
    struct monst *mtmp;
    struct obj *otmp;
    int64_t save_EDetect_mons;
    char save_viz_uyux;
    boolean unconstrained, refresh = FALSE,
            mdetected = FALSE, odetected = FALSE,
            /* fake spellbook 'sobj' implies hero has cast the spell;
               when book is blessed, casting is skilled or expert level;
               if already clairvoyant, non-skilled spell acts like skilled */
            extended = (sobj && (sobj->blessed || Clairvoyant));
    int newglyph, oldglyph,
        lo_y = ((u.uy - 5 < 0) ? 0 : u.uy - 5),
        hi_y = ((u.uy + 6 >= ROWNO) ? ROWNO - 1 : u.uy + 6),
        lo_x = ((u.ux - 9 < 1) ? 1 : u.ux - 9), /* avoid column 0 */
        hi_x = ((u.ux + 10 >= COLNO) ? COLNO - 1 : u.ux + 10),
        ter_typ = TER_DETECT | TER_MAP | TER_TRP | TER_OBJ;

    /*
     * 3.6.0 attempted to emphasize terrain over transient map
     * properties (monsters and objects) but that led to problems.
     * Notably, known trap would be displayed instead of a monster
     * on or in it and then the display remained that way after the
     * clairvoyant snapshot finished.  That could have been fixed by
     * issuing --More-- and then regular vision update, but we want
     * to avoid that when having a clairvoyant episode every N turns
     * (from donating to a temple priest or by carrying the Amulet).
     * Unlike when casting the spell, it is much too intrustive when
     * in the midst of walking around or combatting monsters.
     *
     * For 3.6.2, show terrain, then object, then monster like regular
     * map updating, except in this case the map locations get marked
     * as seen from every direction rather than just from direction of
     * hero.  Skilled spell marks revealed objects as 'seen up close'
     * (but for piles, only the top item) and shows monsters as if
     * detected.  Non-skilled and timed clairvoyance reveals non-visible
     * monsters as 'remembered, unseen'.
     */

    /* if hero is engulfed, show engulfer at <u.ux,u.uy> */
    save_viz_uyux = viz_array[u.uy][u.ux];
    if (u.uswallow)
        viz_array[u.uy][u.ux] |= IN_SIGHT; /* <x,y> are reversed to [y][x] */
    save_EDetect_mons = EDetect_monsters;
    /* for skilled spell, getpos() scanning of the map will display all
       monsters within range; otherwise, "unseen creature" will be shown */
    EDetect_monsters |= I_SPECIAL;
    unconstrained = unconstrain_map();
    for (zx = lo_x; zx <= hi_x; zx++)
        for (zy = lo_y; zy <= hi_y; zy++) {
            oldglyph = glyph_at(zx, zy);
            /* this will remove 'remembered, unseen mon' (and objects) */
            show_map_spot(zx, zy);
            /* if there are any objects here, see the top one */
            if (OBJ_AT(zx, zy)) {
                /* not vobj_at(); this is not vision-based access;
                   unlike object detection, we don't notice buried items */
                otmp = level.objects[zx][zy];
                if (extended)
                    otmp->dknown = 1;
                map_object(otmp, TRUE);
                newglyph = glyph_at(zx, zy);
                /* if otmp is underwater, we'll need to redisplay the water */
                if (newglyph != oldglyph && covers_objects(zx, zy))
                    odetected = TRUE;
            }
            /* if there is a monster here, see or detect it,
               possibly as "remembered, unseen monster" */
            if ((mtmp = m_at(zx, zy)) != 0
                && mtmp->mx == zx && mtmp->my == zy) { /* skip worm tails */
                /* if we're going to offer browse_map()/getpos() scanning of
                   the map and we're not doing extended/blessed clairvoyance
                   (hence must be swallowed or underwater), show "unseen
                   creature" unless map already displayed a monster here */
                if ((unconstrained || !level.flags.hero_memory)
                    && !extended && (zx != u.ux || zy != u.uy)
                    && !glyph_is_monster(oldglyph))
                    map_invisible(zx, zy);
                else
                    map_monst(mtmp, FALSE);
                newglyph = glyph_at(zx, zy);
                if (extended && newglyph != oldglyph
                    && !glyph_is_invisible(newglyph))
                    mdetected = TRUE;
            }
        }

    if (sobj && (!level.flags.hero_memory || unconstrained || mdetected || odetected))
    {
        flush_screen(1);                 /* flush temp screen */
        /* the getpos() prompt from browse_map() is only shown when
           flags.verbose is set, but make this unconditional so that
           not-verbose users become aware of the prompting situation */
        You("sense your surroundings.");
        if (extended || glyph_is_monster(glyph_at(u.ux, u.uy)))
            ter_typ |= TER_MON;
        browse_map(ter_typ, "anything of interest");
        refresh = TRUE;
    }
    reconstrain_map();
    EDetect_monsters = save_EDetect_mons;
    viz_array[u.uy][u.ux] = save_viz_uyux;

    /* replace monsters with remembered,unseen monster, then run
       see_monsters() to update visible ones and warned-of ones */
    for (zx = lo_x; zx <= hi_x; zx++)
        for (zy = lo_y; zy <= hi_y; zy++) {
            if (zx == u.ux && zy == u.uy)
                continue;
            newglyph = glyph_at(zx, zy);
            if (glyph_is_monster(newglyph)
                && glyph_to_mon(newglyph) != PM_LONG_WORM_TAIL && glyph_to_mon(newglyph) != PM_ELDER_LONG_WORM_TAIL)
                map_invisible(zx, zy);
        }
    see_monsters();

    if (refresh)
        docrt();
}

void
cvt_scorr_to_corr_with_animation(x, y)
int x, y;
{
    if (!isok(x, y))
        return;

    struct layer_info layers = layers_at(x, y);
    int glyph = layers.layer_gui_glyphs[LAYER_FLOOR];
    create_basic_floor_location(x, y, levl[x][y].floortyp ? levl[x][y].floortyp : CORR, 0, 0, FALSE);
    unblock_vision_and_hearing_at_point(x, y); /* vision */
    feel_newsym(x, y);
    if (windowprocs.wincap2 & WC2_FADING_ANIMATIONS)
    {
        play_special_effect_with_details_at(0, x, y, glyph, LAYER_BACKGROUND_EFFECT, -2, 20, 0, 0, FALSE);
        special_effect_wait_until_action(0);
        special_effect_wait_until_end(0);
    }
}

void
cvt_sdoor_to_door_with_animation(x, y)
int x, y;
{
    if (!isok(x, y))
        return;

    if (windowprocs.wincap2 & WC2_FADING_ANIMATIONS)
    {
        play_special_effect_with_details_at(0, x, y, levl[x][y].horizontal ? cmap_to_glyph(S_hcdoor) : cmap_to_glyph(S_vcdoor), LAYER_BACKGROUND_EFFECT, -1, 20, 0, 0, FALSE);
        special_effect_wait_until_action(0);
        special_effect_wait_until_end(0);
    }
    cvt_sdoor_to_door(x, y);
    flush_screen(1);
}

/* convert a secret door into a normal door */
void
cvt_sdoor_to_door(x, y)
int x, y;
{
    struct rm* lev = &levl[x][y];
    int newmask = lev->doormask & ~WM_MASK;
    int subtype = lev->subtyp;
    if (Is_really_rogue_level(&u.uz))
    {
        /* rogue didn't have doors, only doorways */
        newmask = D_NODOOR;
        subtype = 0;
    }
    else
    {
        /* newly exposed door is closed */
        if (!(newmask & D_LOCKED))
            newmask |= D_CLOSED;
    }
    /* Add other flags than door mask */
    newmask |= (lev->doormask & ~D_MASK);
    transform_location_type_and_flags(x, y, DOOR, subtype, newmask);
}

STATIC_PTR void
findone(zx, zy, num)
int zx, zy;
genericptr_t num;
{
    register struct trap *ttmp;
    register struct monst *mtmp;

    if (levl[zx][zy].typ == SDOOR) 
    {
        cvt_sdoor_to_door(zx, zy); /* .typ = DOOR */
        magic_map_background(zx, zy, 0);
        newsym(zx, zy);
        (*(int *) num)++;
    } 
    else if (levl[zx][zy].typ == SCORR)
    {
        levl[zx][zy].typ = CORR;
        unblock_vision_and_hearing_at_point(zx, zy);
        magic_map_background(zx, zy, 0);
        newsym(zx, zy);
        (*(int *) num)++;
    } 
    else if ((ttmp = t_at(zx, zy)) != 0)
    {
        if (!ttmp->tseen && ttmp->ttyp != STATUE_TRAP)
        {
            ttmp->tseen = 1;
            newsym(zx, zy);
            (*(int *) num)++;
        }
    }
    else if ((mtmp = m_at(zx, zy)) != 0) 
    {
        if (M_AP_TYPE(mtmp))
        {
            seemimic(mtmp);
            (*(int *) num)++;
        }

        if (mtmp->mundetected
            && (is_hider(mtmp->data) || mtmp->data->mlet == S_EEL))
        {
            mtmp->mundetected = 0;
            newsym(zx, zy);
            (*(int *) num)++;
        }

        if (!canspotmon(mtmp) && !glyph_is_invisible(levl[zx][zy].hero_memory_layers.glyph))
            map_invisible(zx, zy);
    }
    else if (unmap_invisible(zx, zy))
    {
        (*(int *) num)++;
    }
}

STATIC_PTR void
openone(zx, zy, num)
int zx, zy;
genericptr_t num;
{
    register struct trap *ttmp;
    register struct obj *otmp;
    int *num_p = (int *) num;

    if (OBJ_AT(zx, zy)) {
        for (otmp = level.objects[zx][zy]; otmp; otmp = otmp->nexthere) {
            if (Is_box(otmp) && otmp->olocked) {
                otmp->olocked = 0;
                (*num_p)++;
            }
        }
        /* let it fall to the next cases. could be on trap. */
    }
    if (levl[zx][zy].typ == SDOOR
        || (levl[zx][zy].typ == DOOR
            && (levl[zx][zy].doormask & (D_CLOSED | D_LOCKED)))) {
        if (levl[zx][zy].typ == SDOOR)
            cvt_sdoor_to_door(zx, zy); /* .typ = DOOR */
        if (levl[zx][zy].doormask & D_TRAPPED) {
            if (distu(zx, zy) < 3)
            {
                b_trapped(get_door_name_at(zx, zy), 0, zx, zy);
                levl[zx][zy].doormask &= ~D_TRAPPED;
            }
            else
                Norep("You %s an explosion!",
                      cansee(zx, zy) ? "see" : (!Deaf ? "hear"
                                                      : "feel the shock of"));
            wake_nearto(zx, zy, 11 * 11);
            if (is_door_destroyed_by_booby_trap_at(zx, zy))
            {
                levl[zx][zy].doormask &= ~D_MASK;
                levl[zx][zy].doormask |= D_NODOOR;
                levl[zx][zy].subtyp = 0;
            }
        }
        else
        {
            levl[zx][zy].doormask &= ~D_MASK;
            levl[zx][zy].doormask |= D_ISOPEN;
        }
        unblock_vision_and_hearing_at_point(zx, zy);
        newsym(zx, zy);
        (*num_p)++;
    } else if (levl[zx][zy].typ == SCORR) {
        levl[zx][zy].typ = CORR;
        unblock_vision_and_hearing_at_point(zx, zy);
        newsym(zx, zy);
        (*num_p)++;
    } else if ((ttmp = t_at(zx, zy)) != 0) {
        struct monst *mon;
        boolean dummy; /* unneeded "you notice it arg" */

        if (!ttmp->tseen && ttmp->ttyp != STATUE_TRAP) {
            ttmp->tseen = 1;
            newsym(zx, zy);
            (*num_p)++;
        }
        mon = (zx == u.ux && zy == u.uy) ? &youmonst : m_at(zx, zy);
        if (openholdingtrap(mon, &dummy)
            || openfallingtrap(mon, TRUE, &dummy))
            (*num_p)++;
    } else if (find_drawbridge(&zx, &zy)) {
        /* make sure it isn't an open drawbridge */
        open_drawbridge(zx, zy, FALSE);
        (*num_p)++;
    }
}

/* returns number of things found */
int
findit()
{
    int num = 0;

    if (u.uswallow)
        return 0;
    do_clear_area(u.ux, u.uy, FIND_RANGE, findone, (genericptr_t) &num);
    return num;
}

/* returns number of things found and opened */
int
openit()
{
    int num = 0;

    if (u.uswallow) {
        if (is_animal(u.ustuck->data)) {
            if (Blind)
                pline("Its mouth opens!");
            else
                pline("%s opens its mouth!", Monnam(u.ustuck));
        }
        expels(u.ustuck, u.ustuck->data, TRUE);
        return -1;
    }

    do_clear_area(u.ux, u.uy, OPEN_RANGE, openone, (genericptr_t) &num);
    return num;
}

/* callback hack for overriding vision in do_clear_area() */
boolean
detecting(func)
void FDECL((*func), (int, int, genericptr_t));
{
    return (func == findone || func == openone);
}

void
find_trap(trap)
struct trap *trap;
{
    //int tt = what_trap(trap->ttyp, rn2);
    boolean cleared = FALSE;

    trap->tseen = 1;
    exercise(A_WIS, TRUE);
    feel_newsym(trap->tx, trap->ty);

    /* The "Hallucination ||" is to preserve 3.6.1 behaviour, but this
       behaviour might need a rework in the hallucination case
       (e.g. to not prompt if any trap glyph appears on the
       square). */
    if (Hallucination ||
        levl[trap->tx][trap->ty].hero_memory_layers.glyph !=
        trap_to_glyph(trap, rn2_on_display_rng)) {
        /* There's too much clutter to see your find otherwise */
        cls();
        map_trap(trap, 1);
        display_self();
        cleared = TRUE;
    }

    play_sfx_sound(SFX_TRAP_FOUND);
    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "find %s.", an(get_trap_explanation(trap)));

    if (cleared) {
        create_context_menu(CREATE_CONTEXT_MENU_BLOCKING_WINDOW);
        display_nhwindow(WIN_MAP, TRUE); /* wait */
        create_context_menu(CREATE_CONTEXT_MENU_NORMAL);
        docrt();
    }
}

STATIC_OVL int
mfind0(mtmp, via_warning)
struct monst *mtmp;
boolean via_warning;
{
    int x = mtmp->mx, y = mtmp->my;
    boolean found_something = FALSE;

    if (via_warning && !warning_of(mtmp))
        return -1;

    if (M_AP_TYPE(mtmp))
    {
        seemimic(mtmp);
        found_something = TRUE;
    } 
    else if (!canspotmon(mtmp))
    {
        if (mtmp->mundetected
            && (is_hider(mtmp->data) || mtmp->data->mlet == S_EEL))
        {
            if (via_warning)
            {
                Your("warning senses cause you to take a second %s.",
                     Blind ? "to check nearby" : "look close by");
                display_nhwindow(WIN_MESSAGE, FALSE); /* flush messages */
            }
            mtmp->mundetected = 0;
        }
        newsym(x, y);
        found_something = TRUE;
    }

    if (found_something)
    {
        if (!canspotmon(mtmp) && glyph_is_invisible(levl[x][y].hero_memory_layers.glyph))
            return -1; /* Found invisible monster in square which already has
                        * 'I' in it.  Logically, this should still take time
                        * and lead to `return 1', but if we did that the hero
                        * would keep finding the same monster every turn. */
        exercise(A_WIS, TRUE);
        if (!canspotmon(mtmp)) 
        {
            map_invisible(x, y);
            You_feel("an unseen monster!");
        }
        else if (!sensemon(mtmp))
        {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "find %s.", is_tame(mtmp) ? y_monnam(mtmp) : a_monnam(mtmp));
        }
        return 1;
    }
    return 0;
}

int
dosearch0(aflag)
register int aflag; /* intrinsic autosearch vs explicit searching */
{
#ifdef GCC_BUG
    /* Some old versions of gcc seriously muck up nested loops.  If you get
     * strange crashes while searching in a version compiled with gcc, try
     * putting #define GCC_BUG in *conf.h (or adding -DGCC_BUG to CFLAGS in
     * the makefile).
     */
    volatile xchar x, y;
#else
    register xchar x, y;
#endif
    register struct trap *trap;
    register struct monst *mtmp;

    if (u.uswallow) 
    {
        if (!aflag)
        {
            play_sfx_sound(SFX_GENERAL_THATS_SILLY);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "What are you looking for?  The exit?");
        }
    }
    else 
    {
        if (flags.search_box_traps && !aflag && check_all_box_traps(FALSE) == 1)
            return 1;

        int fund = 0;
        if (Enhanced_vision && !Blind)
            fund += 2; /* JDS: lenses help searching */
        if (Searching)
            fund += 1; /* JG: searching helps a little in finding hidden doors */
        fund += (Luck + 1) / 2; /* Luck moved here to be more consistent */

        if (fund > 5)
            fund = 5;
        else if (fund < -10)
            fund = -10;

        int itemsfound = unearth_objs(&youmonst, u.ux, u.uy, TRUE, TRUE);
        if (itemsfound)
        {
            (void)pickup(1, FALSE);
        }
        else
        {
            if (!aflag && (context.first_time_cmd || !occupation))
            {
                play_simple_player_sound_with_flags(MONSTER_SOUND_TYPE_SEARCH, PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING);
                display_gui_effect(GUI_EFFECT_SEARCH, 0, u.ux, u.uy, 0, 0, 0UL);
                if (iflags.using_gui_sounds)
                    delay_output_milliseconds(10 * ANIMATION_FRAME_INTERVAL);
            }

            for (x = u.ux - 1; x <= u.ux + 1; x++)
            {
                for (y = u.uy - 1; y <= u.uy + 1; y++) 
                {
                    if (!isok(x, y))
                        continue;
                    if (x == u.ux && y == u.uy)
                        continue;

                    if (Blind && !aflag)
                        feel_location(x, y);

                    if (levl[x][y].typ == SDOOR)
                    {
                        if (rn2(7 - fund))
                            continue;
                        play_sfx_sound(SFX_HIDDEN_DOOR_FOUND);
                        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "find a hidden door.");
                        cvt_sdoor_to_door_with_animation(x, y); /* .typ = DOOR */
                        exercise(A_WIS, TRUE);
                        nomul(0);
                        feel_location(x, y); /* make sure it shows up */
                        flush_screen(1);
                    } 
                    else if (levl[x][y].typ == SCORR) 
                    {
                        if (rn2(7 - fund))
                            continue;
                        play_sfx_sound(SFX_HIDDEN_DOOR_FOUND);
                        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "find a hidden passage.");
                        cvt_scorr_to_corr_with_animation(x, y);
                        //levl[x][y].typ = CORR;
                        //unblock_vision_and_hearing_at_point(x, y); /* vision */
                        //feel_newsym(x, y); /* make sure it shows up */
                        exercise(A_WIS, TRUE);
                        nomul(0);
                        flush_screen(1);
                    }
                    else
                    {
                        /* Be careful not to find anything in an SCORR or SDOOR */
                        if ((mtmp = m_at(x, y)) != 0 && !aflag) 
                        {
                            int mfres = mfind0(mtmp, 0);

                            if (mfres == -1)
                                continue;
                            else if (mfres > 0)
                                return mfres;
                        }

                        /* see if an invisible monster has moved--if Blind,
                         * feel_location() already did it
                         */
                        if (!aflag && !mtmp && !Blind)
                            (void) unmap_invisible(x, y);

                        if ((trap = t_at(x, y)) && !trap->tseen && !rnl(8)) {
                            nomul(0);
                            if (trap->ttyp == STATUE_TRAP) {
                                if (activate_statue_trap(trap, x, y, FALSE))
                                    exercise(A_WIS, TRUE);
                                return 1;
                            } else {
                                find_trap(trap);
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

/* the 's' command -- explicit searching */
int
dosearch()
{
    return dosearch0(0);
}

void
warnreveal()
{
    int x, y;
    struct monst *mtmp;

    for (x = u.ux - 1; x <= u.ux + 1; x++)
        for (y = u.uy - 1; y <= u.uy + 1; y++) {
            if (!isok(x, y) || (x == u.ux && y == u.uy))
                continue;
            if ((mtmp = m_at(x, y)) != 0
                && warning_of(mtmp) && mtmp->mundetected)
                (void) mfind0(mtmp, 1); /* via_warning */
        }
}

/* Pre-map the sokoban levels */
void
sokoban_detect()
{
    register int x, y;
    register struct trap *ttmp;
    register struct obj *obj;

    /* Map the background and boulders */
    for (x = 1; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++) {
            levl[x][y].seenv = SVALL;
            levl[x][y].waslit = TRUE;
            map_background(x, y, 1);
            if ((obj = sobj_at(BOULDER, x, y)) != 0)
                map_object(obj, 1);
        }

    /* Map the traps */
    for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap) {
        ttmp->tseen = 1;
        map_trap(ttmp, 1);
        /* set sokoban_rules when there is at least one pit or hole */
        if (ttmp->ttyp == PIT || ttmp->ttyp == HOLE)
            Sokoban = 1;
    }
}

STATIC_DCL int
reveal_terrain_getglyph(x, y, full, swallowed, default_glyph, which_subset)
int x, y, full;
boolean swallowed;
int default_glyph, which_subset;
{
    int glyph, levl_glyph;
    uchar seenv;
    boolean keep_traps = (which_subset & TER_TRP) !=0,
            keep_objs = (which_subset & TER_OBJ) != 0,
            keep_mons = (which_subset & TER_MON) != 0;
    struct monst *mtmp;
    struct trap *t;

    /* for 'full', show the actual terrain for the entire level,
       otherwise what the hero remembers for seen locations with
       monsters, objects, and/or traps removed as caller dictates */
    seenv = (full || level.flags.hero_memory)
              ? levl[x][y].seenv : cansee(x, y) ? SVALL : 0;
    if (full)
    {
        levl[x][y].seenv = SVALL;
        glyph = back_to_glyph(x, y);
        levl[x][y].seenv = seenv;
    }
    else
    {
        levl_glyph = level.flags.hero_memory
              ? levl[x][y].hero_memory_layers.glyph
              : seenv ? back_to_glyph(x, y): default_glyph;

        /* glyph_at() returns the displayed glyph, which might
           be a monster.  levl[][].hero_memory_layers.glyph contains the remembered
           glyph, which will never be a monster (unless it is
           the invisible monster glyph, which is handled like
           an object, replacing any object or trap at its spot) */
        glyph = !swallowed ? glyph_at(x, y) : levl_glyph;
        if (keep_mons && x == u.ux && y == u.uy && swallowed)
            glyph = any_mon_to_glyph(u.ustuck, rn2_on_display_rng);
        else if (((glyph_is_monster(glyph)
                   || glyph_is_warning(glyph)) && !keep_mons)
                 || glyph_is_swallow(glyph))
            glyph = levl_glyph;

        if (((glyph_is_object(glyph) && !keep_objs)
             || glyph_is_invisible(glyph))
            && keep_traps && !covers_traps(x, y)) 
        {
            if ((t = t_at(x, y)) != 0 && t->tseen)
                glyph = trap_to_glyph(t, rn2_on_display_rng);
        }

        if ((glyph_is_object(glyph) && !keep_objs)
            || (glyph_is_trap(glyph) && !keep_traps)
            || glyph_is_invisible(glyph))
        {
            if (!seenv) 
            {
                glyph = default_glyph;
            }
            else if (lastseentyp[x][y] == levl[x][y].typ)
            {
                glyph = back_to_glyph(x, y);
            }
            else
            {
                /* look for a mimic here posing as furniture;
                   if we don't find one, we'll have to fake it */
                if ((mtmp = m_at(x, y)) != 0
                    && M_AP_TYPE(mtmp) == M_AP_FURNITURE)
                {
                    glyph = cmap_to_glyph(mtmp->mappearance);
                }
                else
                {
                    /* we have a topology type but we want a screen
                       symbol in order to derive a glyph; some screen
                       symbols need the flags field of levl[][] in
                       addition to the type (to disambiguate STAIRS to
                       S_upstair or S_dnstair, for example; current
                       flags might not be intended for remembered type,
                       but we've got no other choice) */
                    schar save_typ = levl[x][y].typ;

                    levl[x][y].typ = lastseentyp[x][y];
                    glyph = back_to_glyph(x, y);
                    levl[x][y].typ = save_typ;
                }
            }
        }
    }

    return glyph;
}

#if defined(DUMPLOG) || defined(DUMPHTML)
void
dump_map()
{
    int x, y, glyph, skippedrows, lastnonblank;
    int subset = TER_MAP | TER_TRP | TER_OBJ | TER_MON;
    int default_glyph = base_cmap_to_glyph(level.flags.arboreal ? S_tree : S_unexplored);
    char buf[BUFSZ * 2];
    char* bp;
    boolean blankrow, toprow;

    /*
     * Squeeze out excess vertial space when dumping the map.
     * If there are any blank map rows at the top, suppress them
     * (our caller has already printed a separator).  If there is
     * more than one blank map row at the bottom, keep just one.
     * Any blank rows within the middle of the map are kept.
     * Note: putstr() with winid==0 is for dumplog.
     */
    skippedrows = 0;
    toprow = TRUE;
    for (y = 0; y < ROWNO; y++) 
    {
        bp = buf;
        blankrow = TRUE; /* assume blank until we discover otherwise */
        lastnonblank = -1; /* buf[] index rather than map's x */
        for (x = 1; x < COLNO; x++) 
        {
            nhsym ch;
            int color, sym;
            uint64_t special;

            glyph = reveal_terrain_getglyph(x, y, FALSE, u.uswallow,
                                            default_glyph, subset);
            struct layer_info layers = nul_layerinfo;
            layers.glyph = glyph;

            sym = mapglyph(layers, &ch, &color, &special, x, y);
            if (SYMHANDLING(H_IBM) || SYMHANDLING(H_UNICODE))
            {
                write_nhsym_utf8(&bp, ch, !!SYMHANDLING(H_IBM)); //buf[x - 1] = ch;
            }
            else
            {
                /* Revert to basic symbols; no better looking processing available */
                if(sym >= 0 && sym < MAX_CMAPPED_CHARS)
                    ch = (nhsym)defsyms[sym].sym;
                write_nhsym_utf8(&bp, ch, FALSE);
            }

#if defined (DUMPHTML)
            /* HTML map prints in a defined rectangle, so
               just render every glyph - no skipping. */
            html_dump_glyph(x, y, sym, ch, color, special);
#endif

            /* UTF-8 must be handled here, because you cannot write nhsym (int64_t) to the buf (char) and convert it later */
            /* Note: write_nhsym_utf8 moves bp forward the right amount (1-4 bytes) */

            if (ch != ' ') 
            {
                blankrow = FALSE;
                lastnonblank = (int)(bp - buf - 1); /* Since write_nhsym_utf8 moved the pointer one further than last nonblank */ //x - 1;
            }
        }

        if (!blankrow) 
        {
            buf[lastnonblank + 1] = '\0';
            if (toprow) 
            {
                skippedrows = 0;
                toprow = FALSE;
            }

            for (x = 0; x < skippedrows; x++)
                dump_putstr_no_utf8(0, 0, "");

            dump_putstr_no_utf8(0, 0, buf); /* map row #y, utf-8 already handled */
            skippedrows = 0;
        }
        else 
        {
            ++skippedrows;
        }
    }
    if (skippedrows)
        dump_putstr_no_utf8(0, 0, "");
}
#endif /* DUMPLOG */

/* idea from crawl; show known portion of map without any monsters,
   objects, or traps occluding the view of the underlying terrain */
void
reveal_terrain(full, which_subset)
int full; /* wizard|explore modes allow player to request full map */
int which_subset; /* when not full, whether to suppress objs and/or traps */
{
    if ((Hallucination || Stunned || Confusion) && !full) 
    {
        play_sfx_sound(SFX_GENERAL_NOT_IN_THE_RIGHT_CONDITION);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are too disoriented for this.");
    } 
    else 
    {
        int x, y, glyph, default_glyph;
        char buf[BUFSZ];
        /* there is a TER_MAP bit too; we always show map regardless of it */
        boolean keep_traps = (which_subset & TER_TRP) !=0,
                keep_objs = (which_subset & TER_OBJ) != 0,
                keep_mons = (which_subset & TER_MON) != 0; /* not used */
        boolean swallowed = u.uswallow; /* before unconstrain_map() */

        if (unconstrain_map())
            docrt();
        default_glyph = cmap_to_glyph(level.flags.arboreal ? S_tree : S_unexplored);

        for (x = 1; x < COLNO; x++)
            for (y = 0; y < ROWNO; y++) {
                glyph = reveal_terrain_getglyph(x,y, full, swallowed,
                                                default_glyph, which_subset);
                show_glyph(x, y, glyph);
            }

        /* hero's location is not highlighted, but getpos() starts with
           cursor there, and after moving it anywhere '@' moves it back */
        flush_screen(1);
        if (full) {
            Strcpy(buf, "underlying terrain");
        } else {
            Strcpy(buf, "known terrain");
            if (keep_traps)
                Sprintf(eos(buf), "%s traps",
                        (keep_objs || keep_mons) ? "," : " and");
            if (keep_objs)
                Sprintf(eos(buf), "%s%s objects",
                        (keep_traps || keep_mons) ? "," : "",
                        keep_mons ? "" : " and");
            if (keep_mons)
                Sprintf(eos(buf), "%s and monsters",
                        (keep_traps || keep_objs) ? "," : "");
        }
        pline("Showing %s only...", buf);

        /* allow player to move cursor around and get autodescribe feedback
           based on what is visible now rather than what is on 'real' map */
        which_subset |= TER_MAP; /* guarantee non-zero */
        browse_map(which_subset, "anything of interest");

        reconstrain_map();
        docrt(); /* redraw the screen, restoring regular map */
        if (Underwater)
            under_water(2);
        if (u.uburied)
            under_ground(2);
    }
    return;
}


/*detect.c*/

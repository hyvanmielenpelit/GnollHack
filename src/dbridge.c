/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    dbridge.c    $NHDT-Date: 1503355815 2017/08/21 22:50:15 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.39 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet              */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * This file contains the drawbridge manipulation (create, open, close,
 * destroy).
 *
 * Added comprehensive monster-handling, and the "entity" structure to
 * deal with players as well. - 11/89
 *
 * Any traps and/or engravings at either the portcullis or span location
 * are destroyed whenever the bridge is lowered, raised, or destroyed.
 * (Engraving handling could be extended to flag whether an engraving on
 * the DB_UNDER surface is hidden by the lowered bridge, or one on the
 * bridge itself is hidden because the bridge has been raised, but that
 * seems like an awful lot of effort for very little gain.)
 */

#include "hack.h"

STATIC_DCL void FDECL(get_wall_for_db, (int *, int *));
STATIC_DCL struct entity *FDECL(e_at, (int, int));
STATIC_DCL void FDECL(m_to_e, (struct monst *, int, int, struct entity *));
STATIC_DCL void FDECL(u_to_e, (struct entity *));
STATIC_DCL void FDECL(set_entity, (int, int, struct entity *));
STATIC_DCL const char *FDECL(e_nam, (struct entity *));
STATIC_DCL const char *FDECL(E_phrase, (struct entity *, const char *));
STATIC_DCL boolean FDECL(e_survives_at, (struct entity *, int, int));
STATIC_DCL void FDECL(e_died, (struct entity *, int, int));
STATIC_OVL void FDECL(e_damage, (struct entity*, int, int, unsigned long, const char*, int));
STATIC_DCL boolean FDECL(automiss, (struct entity *));
STATIC_DCL boolean FDECL(e_missed, (struct entity *, BOOLEAN_P));
STATIC_DCL boolean FDECL(e_jumps, (struct entity *));
STATIC_DCL void FDECL(do_entity, (struct entity *));

boolean
is_pool(x, y)
int x, y;
{
    schar ltyp;

    if (!isok(x, y))
        return FALSE;
    ltyp = levl[x][y].typ;
    /* The ltyp == MOAT is not redundant with is_moat, because the
     * Jubilex level does not have moats, although it has MOATs. There
     * is probably a better way to express this. */
    if (ltyp == POOL || ltyp == MOAT || ltyp == WATER || is_moat(x, y))
        return TRUE;
    return FALSE;
}

boolean
is_lava(x, y)
int x, y;
{
    schar ltyp;

    if (!isok(x, y))
        return FALSE;
    ltyp = levl[x][y].typ;
    if (ltyp == LAVAPOOL
        || (ltyp == DRAWBRIDGE_UP
            && (levl[x][y].drawbridgemask & DB_UNDER) == DB_LAVA))
        return TRUE;
    return FALSE;
}

boolean
is_pool_or_lava(x, y)
int x, y;
{
    if (is_pool(x, y) || is_lava(x, y))
        return TRUE;
    else
        return FALSE;
}

boolean
is_ice(x, y)
int x, y;
{
    schar ltyp;

    if (!isok(x, y))
        return FALSE;
    ltyp = levl[x][y].typ;
    if (ltyp == ICE || (ltyp == DRAWBRIDGE_UP
                        && (levl[x][y].drawbridgemask & DB_UNDER) == DB_ICE))
        return TRUE;
    return FALSE;
}

boolean
is_moat(x, y)
int x, y;
{
    schar ltyp;

    if (!isok(x, y))
        return FALSE;
    ltyp = levl[x][y].typ;
    if (!Is_jubilex_level(&u.uz)
        && (ltyp == MOAT
            || (ltyp == DRAWBRIDGE_UP
                && (levl[x][y].drawbridgemask & DB_UNDER) == DB_MOAT)))
        return TRUE;
    return FALSE;
}

schar
db_under_typ(mask)
int mask;
{
    switch (mask & DB_UNDER) {
    case DB_ICE:
        return ICE;
    case DB_LAVA:
        return LAVAPOOL;
    case DB_MOAT:
        return MOAT;
    default:
        return STONE;
    }
}

/*
 * We want to know whether a wall (or a door) is the portcullis (passageway)
 * of an eventual drawbridge.
 *
 * Return value:  the direction of the drawbridge.
 */

int
is_drawbridge_wall(x, y)
int x, y;
{
    struct rm *lev;

    lev = &levl[x][y];
    if (lev->typ != DOOR && lev->typ != DBWALL)
        return -1;

    if (IS_DRAWBRIDGE(levl[x + 1][y].typ)
        && (levl[x + 1][y].drawbridgemask & DB_DIR) == DB_WEST)
        return DB_WEST;
    if (IS_DRAWBRIDGE(levl[x - 1][y].typ)
        && (levl[x - 1][y].drawbridgemask & DB_DIR) == DB_EAST)
        return DB_EAST;
    if (IS_DRAWBRIDGE(levl[x][y - 1].typ)
        && (levl[x][y - 1].drawbridgemask & DB_DIR) == DB_SOUTH)
        return DB_SOUTH;
    if (IS_DRAWBRIDGE(levl[x][y + 1].typ)
        && (levl[x][y + 1].drawbridgemask & DB_DIR) == DB_NORTH)
        return DB_NORTH;

    return -1;
}

/*
 * Use is_db_wall where you want to verify that a
 * drawbridge "wall" is UP in the location x, y
 * (instead of UP or DOWN, as with is_drawbridge_wall).
 */
boolean
is_db_wall(x, y)
int x, y;
{
    return (boolean) (levl[x][y].typ == DBWALL);
}

/*
 * Return true with x,y pointing to the drawbridge if x,y initially indicate
 * a drawbridge or drawbridge wall.
 */
boolean
find_drawbridge(x, y)
int *x, *y;
{
    int dir;

    if (IS_DRAWBRIDGE(levl[*x][*y].typ))
        return TRUE;
    dir = is_drawbridge_wall(*x, *y);
    if (dir >= 0) {
        switch (dir) {
        case DB_NORTH:
            (*y)++;
            break;
        case DB_SOUTH:
            (*y)--;
            break;
        case DB_EAST:
            (*x)--;
            break;
        case DB_WEST:
            (*x)++;
            break;
        }
        return TRUE;
    }
    return FALSE;
}

/*
 * Find the drawbridge wall associated with a drawbridge.
 */
STATIC_OVL void
get_wall_for_db(x, y)
int *x, *y;
{
    switch (levl[*x][*y].drawbridgemask & DB_DIR) {
    case DB_NORTH:
        (*y)--;
        break;
    case DB_SOUTH:
        (*y)++;
        break;
    case DB_EAST:
        (*x)++;
        break;
    case DB_WEST:
        (*x)--;
        break;
    }
}

/*
 * Creation of a drawbridge at pos x,y.
 *     dir is the direction.
 *     flag must be put to TRUE if we want the drawbridge to be opened.
 */
boolean
create_drawbridge(x, y, dir, flag)
int x, y, dir;
boolean flag;
{
    int x2, y2;
    boolean horiz;
    boolean lava = levl[x][y].typ == LAVAPOOL; /* assume initialized map */

    x2 = x;
    y2 = y;
    switch (dir) {
    case DB_NORTH:
        horiz = TRUE;
        y2--;
        break;
    case DB_SOUTH:
        horiz = TRUE;
        y2++;
        break;
    case DB_EAST:
        horiz = FALSE;
        x2++;
        break;
    default:
        impossible("bad direction in create_drawbridge");
        /*FALLTHRU*/
    case DB_WEST:
        horiz = FALSE;
        x2--;
        break;
    }
    if (!IS_WALL(levl[x2][y2].typ))
        return FALSE;
    
    int lev1typ = 0;
    int lev2typ = 0;
    uchar lev1flags = 0;
    uchar lev2flags = 0;
    boolean lev1horizontal = 0;
    boolean lev2horizontal = 0;

    if (flag)
    { /* We want the bridge open */
        lev1typ = DRAWBRIDGE_DOWN;
        lev2typ = DOOR;
        lev2flags = D_PORTCULLIS;
    } 
    else 
    {
        lev1typ = DRAWBRIDGE_UP;
        lev2typ = DBWALL;
        /* Drawbridges are non-diggable. */
        lev2flags = W_NONDIGGABLE;
    }
    lev1horizontal = !horiz;
    lev2horizontal = horiz;
    lev1flags = dir;
    if (lava)
        lev1flags |= DB_LAVA;

    full_location_transform(x, y, lev1typ, 0, 0, lev1flags, 0, 0, 0, 0, 0, 0, 0, 0, levl[x][y].floortyp, levl[x][y].floorsubtyp, levl[x][y].floorvartyp, FALSE, lev1horizontal, 0, 0, FALSE);
    full_initial_location_transform(x2, y2, lev2typ, lev2flags, 0, 0, 0, 0, 0, 0, 0, 0, ROOM, FALSE, lev2horizontal, 0, 0, FALSE);

    return  TRUE;
}

struct entity {
    struct monst *emon;     /* youmonst for the player */
    struct permonst *edata; /* must be non-zero for record to be valid */
    int ex, ey;
};

#define ENTITIES 2

STATIC_VAR NEARDATA struct entity occupants[ENTITIES];

STATIC_OVL
struct entity *
e_at(x, y)
int x, y;
{
    int entitycnt;

    for (entitycnt = 0; entitycnt < ENTITIES; entitycnt++)
        if ((occupants[entitycnt].edata) && (occupants[entitycnt].ex == x)
            && (occupants[entitycnt].ey == y))
            break;
    debugpline1("entitycnt = %d", entitycnt);
#ifdef D_DEBUG
    wait_synch();
#endif
    return (entitycnt == ENTITIES) ? (struct entity *) 0
                                   : &(occupants[entitycnt]);
}

STATIC_OVL void
m_to_e(mtmp, x, y, etmp)
struct monst *mtmp;
int x, y;
struct entity *etmp;
{
    etmp->emon = mtmp;
    if (mtmp) {
        etmp->ex = x;
        etmp->ey = y;
        if (mtmp->wormno && (x != mtmp->mx || y != mtmp->my))
            etmp->edata = &mons[get_worm_tail_mnum(mtmp->data)];
        else
            etmp->edata = mtmp->data;
    } else
        etmp->edata = (struct permonst *) 0;
}

STATIC_OVL void
u_to_e(etmp)
struct entity *etmp;
{
    etmp->emon = &youmonst;
    etmp->ex = u.ux;
    etmp->ey = u.uy;
    etmp->edata = youmonst.data;
}

STATIC_OVL void
set_entity(x, y, etmp)
int x, y;
struct entity *etmp;
{
    if ((x == u.ux) && (y == u.uy))
        u_to_e(etmp);
    else if (MON_AT(x, y))
        m_to_e(m_at(x, y), x, y, etmp);
    else
        etmp->edata = (struct permonst *) 0;
}

#define is_u(etmp) (etmp->emon == &youmonst)
#define e_canseemon(etmp) \
    (is_u(etmp) ? (boolean) TRUE : canseemon(etmp->emon))

/*
 * e_strg is a utility routine which is not actually in use anywhere, since
 * the specialized routines below suffice for all current purposes.
 */

/* #define e_strg(etmp, func) (is_u(etmp)? (char *)0 : func(etmp->emon)) */

STATIC_OVL const char *
e_nam(etmp)
struct entity *etmp;
{
    return is_u(etmp) ? "you" : mon_nam(etmp->emon);
}

/*
 * Generates capitalized entity name, makes 2nd -> 3rd person conversion on
 * verb, where necessary.
 */
STATIC_OVL const char *
E_phrase(etmp, verb)
struct entity *etmp;
const char *verb;
{
    static char wholebuf[80];

    Strcpy(wholebuf, is_u(etmp) ? "You" : Monnam(etmp->emon));
    if (!verb || !*verb)
        return wholebuf;
    Strcat(wholebuf, " ");
    if (is_u(etmp))
        Strcat(wholebuf, verb);
    else
        Strcat(wholebuf, vtense((char *) 0, verb));
    return wholebuf;
}

/*
 * Simple-minded "can it be here?" routine
 */
STATIC_OVL boolean
e_survives_at(etmp, x, y)
struct entity *etmp;
int x, y;
{
    if (is_incorporeal(etmp->edata))
        return TRUE;
    if (is_pool(x, y))
        return (boolean) ((is_u(etmp) && (Walks_on_water || Amphibious || Swimming
                                          || Flying || Levitation))
                          || is_swimmer(etmp->edata)
                          || is_flyer(etmp->edata)
                          || is_floater(etmp->edata));
    /* must force call to lava_effects in e_died if is_u */
    if (is_lava(x, y))
        return (boolean) ((is_u(etmp) && Moves_above_ground)
                          || likes_lava(etmp->edata)
                          || is_flyer(etmp->edata));
    if (is_db_wall(x, y))
        return (boolean) (is_u(etmp) ? Passes_walls
                          : passes_walls(etmp->edata));
    return TRUE;
}

STATIC_OVL void
e_died(etmp, xkill_flags, how)
struct entity *etmp;
int xkill_flags, how;
{
    if (is_u(etmp)) {
        if (how == DROWNING) {
            killer.name[0] = 0; /* drown() sets its own killer */
            killer.hint_idx = 0;
            (void) drown();
        } else if (how == BURNING) {
            killer.name[0] = 0; /* lava_effects() sets own killer */
            killer.hint_idx = 0;
            (void) lava_effects();
        } else {
            coord xy;

            /* use more specific killer if specified */
            if (!killer.name[0]) {
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "falling drawbridge");
            }
            done(how);
            /* So, you didn't die */
            if (!e_survives_at(etmp, etmp->ex, etmp->ey)) {
                if (enexto(&xy, etmp->ex, etmp->ey, etmp->edata)) {
                    pline("A %s force teleports you away...",
                          Hallucination ? "normal" : "strange");
                    teleds_with_effects(xy.x, xy.y, FALSE, FALSE);
                }
                /* otherwise on top of the drawbridge is the
                 * only viable spot in the dungeon, so stay there
                 */
            }
        }
        /* we might have crawled out of the moat to survive */
        etmp->ex = u.ux, etmp->ey = u.uy;
    } else {
        int entitycnt;

        killer.name[0] = 0;
        killer.hint_idx = 0;
        /* fake "digested to death" damage-type suppresses corpse */
        /* if monsters are moving, one of them caused the destruction */
        if (context.mon_moving)
            monkilled(etmp->emon, ((xkill_flags & XKILL_NOMSG) != 0) ? (char*)0 : "", AD_PHYS, xkill_flags);
        else /* you caused it */
            xkilled(etmp->emon, xkill_flags);
        etmp->edata = (struct permonst *) 0;

        /* dead long worm handling */
        for (entitycnt = 0; entitycnt < ENTITIES; entitycnt++) {
            if (etmp != &(occupants[entitycnt])
                && etmp->emon == occupants[entitycnt].emon)
                occupants[entitycnt].edata = (struct permonst *) 0;
        }
    }
}

STATIC_OVL void
e_damage(etmp, basedmg, adtyp, ad_flags, knam, k_format)
struct entity* etmp;
int basedmg, adtyp;
unsigned long ad_flags;
const char* knam;
int k_format;
{
    if (!etmp || !etmp->emon)
        return;

    if (etmp->emon == &youmonst)
        losehp(adjust_damage(basedmg, (struct monst*)0, &youmonst, adtyp, ad_flags), knam, k_format);
    else
    {
        deduct_monster_hp(etmp->emon, adjust_damage(basedmg, (struct monst*)0, etmp->emon, adtyp, ad_flags));
        if (DEADMONSTER(etmp->emon))
        {
            if (canspotmon(etmp->emon))
                pline("%s is %s!", Monnam(etmp->emon),
                    (is_not_living(etmp->emon->data) || is_vampshifter(etmp->emon))
                    ? "destroyed" : "killed");

            monkilled(etmp->emon, canspotmon(etmp->emon) ? knam : 0, adtyp, 0);
        }
    }
}

/*
 * These are never directly affected by a bridge or portcullis.
 */
STATIC_OVL boolean
automiss(etmp)
struct entity *etmp;
{
    return (boolean) ((is_u(etmp) ? Passes_walls : passes_walls(etmp->edata))
                      || is_incorporeal(etmp->edata));
}

/*
 * Does falling drawbridge or portcullis miss etmp?
 */
STATIC_OVL boolean
e_missed(etmp, chunks)
struct entity *etmp;
boolean chunks;
{
    int misses;

    if (chunks) {
        debugpline0("Do chunks miss?");
    }
    if (automiss(etmp))
        return TRUE;

    if (is_flyer(etmp->edata)
        && (is_u(etmp) ? !Unaware
                       : mon_can_move(etmp->emon)))
        /* flying requires mobility */
        misses = 5; /* out of 8 */
    else if (is_floater(etmp->edata)
             || (is_u(etmp) && Levitation)) /* doesn't require mobility */
        misses = 3;
    else if (chunks && is_pool(etmp->ex, etmp->ey))
        misses = 2; /* sitting ducks */
    else
        misses = 0;

    if (is_db_wall(etmp->ex, etmp->ey))
        misses -= 3; /* less airspace */

    debugpline1("Miss chance = %d (out of 8)", misses);

    return (misses >= rnd(8)) ? TRUE : FALSE;
}

/*
 * Can etmp jump from death?
 */
STATIC_OVL boolean
e_jumps(etmp)
struct entity *etmp;
{
    int tmp = 4; /* out of 10 */

    if (is_u(etmp) ? (Unaware || Fumbling)
                   : (!mon_can_move(etmp->emon)
                      || !etmp->edata->mmove || etmp->emon->wormno))
        return FALSE;

    if (is_u(etmp) ? Confusion : etmp->emon->mprops[CONFUSION])
        tmp -= 2;

    if (is_u(etmp) ? Stunned : etmp->emon->mprops[STUNNED])
        tmp -= 3;

    if (is_db_wall(etmp->ex, etmp->ey))
        tmp -= 2; /* less room to maneuver */

    debugpline2("%s to jump (%d chances in 10)", E_phrase(etmp, "try"), tmp);
    return (tmp >= rnd(10)) ? TRUE : FALSE;
}

STATIC_OVL void
do_entity(etmp)
struct entity *etmp;
{
    int newx, newy, at_portcullis, oldx, oldy;
    boolean must_jump = FALSE, relocates = FALSE, e_inview;
    struct rm *crm;

    if (!etmp->edata)
        return;

    e_inview = e_canseemon(etmp);
    oldx = etmp->ex;
    oldy = etmp->ey;
    at_portcullis = is_db_wall(oldx, oldy);
    crm = &levl[oldx][oldy];

    if (automiss(etmp) && e_survives_at(etmp, oldx, oldy)) 
    {
        if (e_inview && (at_portcullis || IS_DRAWBRIDGE(crm->typ)))
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s passes through %s!",
                      at_portcullis ? "portcullis" : "drawbridge",
                      e_nam(etmp));
        if (is_u(etmp))
            spoteffects(FALSE);
        return;
    }

    if (e_missed(etmp, FALSE)) 
    {
        if (at_portcullis) 
        {
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "portcullis misses %s!", e_nam(etmp));
        } 
        else 
        {
            debugpline1("The drawbridge misses %s!", e_nam(etmp));
        }
        if (e_survives_at(etmp, oldx, oldy)) 
        {
            return;
        }
        else
        {
            debugpline0("Mon can't survive here");
            if (at_portcullis)
                must_jump = TRUE;
            else
                relocates = TRUE; /* just ride drawbridge in */
        }
    }
    else 
    {
        if (crm->typ == DRAWBRIDGE_DOWN) 
        {
            if (is_u(etmp)) 
            {
                killer.format = NO_KILLER_PREFIX;
                Strcpy(killer.name,
                       "crushed to death underneath a drawbridge");
            }
            pline_ex(ATR_NONE, is_u(etmp) ? CLR_MSG_NEGATIVE : CLR_MSG_WARNING, "%s crushed underneath the drawbridge.",
                  E_phrase(etmp, "are"));             /* no jump */
            e_died(etmp,
                   XKILL_NOCORPSE | (e_inview ? XKILL_GIVEMSG : XKILL_NOMSG),
                   CRUSHING); /* no corpse */
            return;       /* Note: Beyond this point, we know we're  */
        }                 /* not at an opened drawbridge, since all  */
        must_jump = TRUE; /* *missable* creatures survive on the     */
    }                     /* square, and all the unmissed ones die.  */

    if (must_jump) 
    {
        if (at_portcullis) 
        {
            if (e_jumps(etmp)) 
            {
                relocates = TRUE;
                debugpline0("Jump succeeds!");
            } 
            else 
            {
                if (e_inview)
                    pline_ex(ATR_NONE, is_u(etmp) ? CLR_MSG_NEGATIVE : CLR_MSG_WARNING, "%s crushed by the falling portcullis!",
                          E_phrase(etmp, "are"));
                else if (!Deaf)
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a crushing sound.");

                e_died(etmp,
                       XKILL_NOCORPSE | (e_inview ? XKILL_GIVEMSG
                                                  : XKILL_NOMSG),
                       CRUSHING);
                /* no corpse */
                return;
            }
        }
        else 
        { /* tries to jump off bridge to original square */
            relocates = !e_jumps(etmp);
            debugpline1("Jump %s!", (relocates) ? "fails" : "succeeds");
        }
    }

    /*
     * Here's where we try to do relocation.  Assumes that etmp is not
     * arriving at the portcullis square while the drawbridge is
     * falling, since this square would be inaccessible (i.e. etmp
     * started on drawbridge square) or unnecessary (i.e. etmp started
     * here) in such a situation.
     */
    debugpline0("Doing relocation.");
    newx = oldx;
    newy = oldy;
    (void) find_drawbridge(&newx, &newy);
    if ((newx == oldx) && (newy == oldy))
        get_wall_for_db(&newx, &newy);
    debugpline0("Checking new square for occupancy.");
    if (relocates && (e_at(newx, newy)))
    {
        /*
         * Standoff problem: one or both entities must die, and/or
         * both switch places.  Avoid infinite recursion by checking
         * first whether the other entity is staying put.  Clean up if
         * we happen to move/die in recursion.
         */
        struct entity *other;

        other = e_at(newx, newy);
        debugpline1("New square is occupied by %s", e_nam(other));
        if (e_survives_at(other, newx, newy) && automiss(other))
        {
            relocates = FALSE; /* "other" won't budge */
            debugpline1("%s suicide.", E_phrase(etmp, "commit"));
        }
        else
        {
            debugpline1("Handling %s", e_nam(other));
            while ((e_at(newx, newy) != 0) && (e_at(newx, newy) != etmp))
                do_entity(other);
            debugpline1("Checking existence of %s", e_nam(etmp));
#ifdef D_DEBUG
            wait_synch();
#endif
            if (e_at(oldx, oldy) != etmp) 
            {
                debugpline1("%s moved or died in recursion somewhere",
                            E_phrase(etmp, "have"));
#ifdef D_DEBUG
                wait_synch();
#endif
                return;
            }
        }
    }

    if (relocates && !e_at(newx, newy)) 
    { /* if e_at() entity = worm tail */
        debugpline1("Moving %s", e_nam(etmp));
        if (!is_u(etmp)) 
        {
            remove_monster(etmp->ex, etmp->ey);
            place_monster(etmp->emon, newx, newy);
            update_monster_region(etmp->emon);
        }
        else 
        {
            u.ux = newx;
            u.uy = newy;
        }
        etmp->ex = newx;
        etmp->ey = newy;
        e_inview = e_canseemon(etmp);
    }
    debugpline1("Final disposition of %s", e_nam(etmp));
#ifdef D_DEBUG
    wait_synch();
#endif

    if (is_db_wall(etmp->ex, etmp->ey)) 
    {
        debugpline1("%s in portcullis chamber", E_phrase(etmp, "are"));
#ifdef D_DEBUG
        wait_synch();
#endif
        if (e_inview) 
        {
            if (is_u(etmp)) 
            {
                You_ex(ATR_NONE, CLR_MSG_WARNING, "tumble towards the closed portcullis!");
                if (automiss(etmp))
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "pass through it!");
                else
                    pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "drawbridge closes in...");
            } 
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s behind the drawbridge.",
                      E_phrase(etmp, "disappear"));
        }
        if (!e_survives_at(etmp, etmp->ex, etmp->ey)) 
        {
            killer.format = KILLED_BY_AN;
            Strcpy(killer.name, "closing drawbridge");
            e_died(etmp, XKILL_NOMSG, CRUSHING);
            return;
        }
        debugpline1("%s in here", E_phrase(etmp, "survive"));
    } 
    else
    {
        debugpline1("%s on drawbridge square", E_phrase(etmp, "are"));
        if (is_pool(etmp->ex, etmp->ey) && !e_inview)
            if (!Deaf)
                You_hear_ex(ATR_NONE, is_u(etmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "a splash.");
        if (e_survives_at(etmp, etmp->ex, etmp->ey))
        {
            if (e_inview && !is_flyer(etmp->edata)
                && !is_floater(etmp->edata))
                pline_ex(ATR_NONE, is_u(etmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s from the bridge.", E_phrase(etmp, "fall"));
            return;
        }
        debugpline1("%s cannot survive on the drawbridge square",
                    E_phrase(etmp, NULL));
        if (is_pool(etmp->ex, etmp->ey) || is_lava(etmp->ex, etmp->ey))
            if (e_inview && !is_u(etmp))
            {
                /* drown() will supply msgs if nec. */
                boolean lava = is_lava(etmp->ex, etmp->ey);

                if (Hallucination)
                    pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "%s the %s and disappears.",
                          E_phrase(etmp, "drink"), lava ? "lava" : "moat");
                else
                    pline_ex(ATR_NONE, is_u(etmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s into the %s.", E_phrase(etmp, "fall"),
                          lava ? hliquid("lava") : "moat");
            }

        killer.format = NO_KILLER_PREFIX;
        Strcpy(killer.name, "fell from a drawbridge");
        e_died(etmp, /* CRUSHING is arbitrary */
               XKILL_NOCORPSE | (e_inview ? XKILL_GIVEMSG : XKILL_NOMSG),
               is_pool(etmp->ex, etmp->ey) ? DROWNING
                 : is_lava(etmp->ex, etmp->ey) ? BURNING
                   : CRUSHING); /*no corpse*/
        return;
    }
}

/* clear stale reason for death before returning */
#define nokiller() (killer.name[0] = '\0', killer.format = 0)

void
maybe_close_drawbridge(x, y, by_u_intentionally)
int x, y;
boolean by_u_intentionally;
{
    register struct rm* lev1;
    int x2, y2;

    lev1 = &levl[x][y];
    if (lev1->typ != DRAWBRIDGE_DOWN)
        return;
    x2 = x;
    y2 = y;
    get_wall_for_db(&x2, &y2);

    if (MON_AT(x, y))
    {
        struct monst* mtmp = m_at(x, y);
        if (mtmp && !is_flying(mtmp) && !is_levitating(mtmp) && !is_incorporeal(mtmp->data) && !passes_walls(mtmp->data) && mtmp->data->cwt >= WT_ELF)
        {
            play_sfx_sound_at_location(SFX_DRAWBRIDGE_STARTS_TO_RISE, x2, y2);
            if(cansee(x, y) || cansee(x2, y2))
                pline_ex(ATR_NONE, CLR_MSG_FAIL, "The drawbridge starts to rise, but the weight of %s keeps it down.",
                    canseemon(mtmp) ? mon_nam(mtmp) : "something");
            return;
        }
    }
    else if (u.ux == x && u.uy == y)
    {
        if (!Flying && !Levitation && !is_incorporeal(youmonst.data) && !passes_walls(youmonst.data) && youmonst.data->cwt >= WT_ELF)
        {
            play_sfx_sound_at_location(SFX_DRAWBRIDGE_STARTS_TO_RISE, x2, y2);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "The drawbridge starts to rise, but your weight keeps it down.");
            return;
        }
    }


    if (MON_AT(x2, y2))
    {
        struct monst* mtmp = m_at(x2, y2);
        if (mtmp && !is_incorporeal(mtmp->data) && !passes_walls(mtmp->data) && mtmp->data->msize >= MZ_MEDIUM)
        {
            play_sfx_sound_at_location(SFX_DRAWBRIDGE_GETS_BLOCKED, x2, y2);
            if (cansee(x, y) || cansee(x2, y2))
                pline_ex(ATR_NONE, CLR_MSG_FAIL, "The drawbridge starts to close, but it gets blocked by %s and falls back down.",
                canseemon(mtmp) ? mon_nam(mtmp) : "something");
            return;
        }
    }
    else if (u.ux == x2 && u.uy == y2)
    {
        if (!is_incorporeal(youmonst.data) && !passes_walls(youmonst.data) && youmonst.data->msize >= MZ_MEDIUM)
        {
            play_sfx_sound_at_location(SFX_DRAWBRIDGE_GETS_BLOCKED, x2, y2);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "The drawbridge starts to close, but it gets blocked by you and falls back down.");
            return;
        }
    }

    close_drawbridge(x, y, by_u_intentionally);
}


/*
 * Close the drawbridge located at x,y
 */
void
close_drawbridge(x, y, by_u_intentionally)
int x, y;
boolean by_u_intentionally;
{
    register struct rm *lev1, *lev2;
    struct trap *t;
    int x2, y2;

    lev1 = &levl[x][y];
    if (lev1->typ != DRAWBRIDGE_DOWN)
        return;
    x2 = x;
    y2 = y;
    get_wall_for_db(&x2, &y2);
    play_sfx_sound_at_location(SFX_CLOSE_DRAWBRIDGE, x2, y2);
    if (cansee(x, y) || cansee(x2, y2))
        You_see_ex(ATR_NONE, by_u_intentionally ? CLR_MSG_SUCCESS : CLR_MSG_ATTENTION, "a drawbridge %s up!",
                (((u.ux == x || u.uy == y) && !Underwater)
                 || distu(x2, y2) < distu(x, y))
                    ? "coming"
                    : "going");
    else /* "5 gears turn" for castle drawbridge tune */
        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "chains rattling and gears turning.");

    full_location_transform(x, y, DRAWBRIDGE_UP, lev1->subtyp, lev1->vartyp, lev1->flags, 0, 0, 0, 0, 0, 0, 0, 0, lev1->floortyp, lev1->floorsubtyp, lev1->floorvartyp, lev1->facing_right, lev1->horizontal, 0, 0, FALSE);

    lev2 = &levl[x2][y2];
    boolean lhoriz = FALSE;
    switch (lev1->drawbridgemask & DB_DIR) {
    case DB_NORTH:
    case DB_SOUTH:
        lhoriz = TRUE;
        break;
    case DB_WEST:
    case DB_EAST:
        lhoriz = FALSE;
        break;
    }
    full_location_transform(x2, y2, DBWALL, 0, 0, W_NONDIGGABLE, 0, 0, 0, 0, 0, 0, 0, 0, lev2->floortyp, lev2->floorsubtyp, lev2->floorvartyp, lev2->facing_right, lhoriz, 0, 0, FALSE);

    set_entity(x, y, &(occupants[0]));
    set_entity(x2, y2, &(occupants[1]));
    do_entity(&(occupants[0]));          /* Do set_entity after first */
    set_entity(x2, y2, &(occupants[1])); /* do_entity for worm tail */
    do_entity(&(occupants[1]));
    if (OBJ_AT(x, y) && !Deaf)
        You_hear("smashing and crushing.");
    (void) revive_nasty(x, y, (char *) 0);
    (void) revive_nasty(x2, y2, (char *) 0);
    delallobj(x, y);
    delallobj(x2, y2);
    if ((t = t_at(x, y)) != 0)
        deltrap(t);
    if ((t = t_at(x2, y2)) != 0)
        deltrap(t);
    del_engr_at(x, y);
    del_engr_at(x2, y2);
    newsym(x, y);
    newsym(x2, y2);
    block_vision_and_hearing_at_point(x2, y2); /* vision */
    nokiller();
}

/*
 * Open the drawbridge located at x,y
 */
void
open_drawbridge(x, y, by_u_intentionally)
int x, y;
boolean by_u_intentionally;
{
    register struct rm *lev1, *lev2;
    struct trap *t;
    int x2, y2;

    lev1 = &levl[x][y];
    if (lev1->typ != DRAWBRIDGE_UP)
        return;
    x2 = x;
    y2 = y;
    get_wall_for_db(&x2, &y2);

    if (cansee(x, y) || cansee(x2, y2))
        You_see_ex(ATR_NONE, by_u_intentionally ? CLR_MSG_SUCCESS : CLR_MSG_ATTENTION, "a drawbridge %s down!",
                (distu(x2, y2) < distu(x, y)) ? "going" : "coming");
    else /* "5 gears turn" for castle drawbridge tune */
        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "gears turning and chains rattling.");

    full_location_transform(x, y, DRAWBRIDGE_DOWN, lev1->subtyp, lev1->vartyp, lev1->flags, 0, 0, 0, 0, 0, 0, 0, 0, lev1->floortyp, lev1->floorsubtyp, lev1->floorvartyp, lev1->facing_right, lev1->horizontal, 0, 0, FALSE);
    lev2 = &levl[x2][y2];
    full_location_transform(x2, y2, DOOR, 0, 0, D_PORTCULLIS, 0, 0, 0, 0, 0, 0, 0, 0, lev2->floortyp, lev2->floorsubtyp, lev2->floorvartyp, lev2->facing_right, lev2->horizontal, 0, 0, FALSE);

    set_entity(x, y, &(occupants[0]));
    set_entity(x2, y2, &(occupants[1]));
    do_entity(&(occupants[0]));          /* do set_entity after first */
    set_entity(x2, y2, &(occupants[1])); /* do_entity for worm tails */
    do_entity(&(occupants[1]));
    (void) revive_nasty(x, y, (char *) 0);
    delallobj(x, y);
    if ((t = t_at(x, y)) != 0)
        deltrap(t);
    if ((t = t_at(x2, y2)) != 0)
        deltrap(t);
    del_engr_at(x, y);
    del_engr_at(x2, y2);
    newsym(x, y);
    newsym(x2, y2);
    unblock_vision_and_hearing_at_point(x2, y2); /* vision */
    play_sfx_sound_at_location(SFX_OPEN_DRAWBRIDGE, x, y);

    if (Is_stronghold(&u.uz))
        u.uevent.uopened_dbridge = TRUE;
    nokiller();
}

/*
 * Let's destroy the drawbridge located at x,y
 */
void
destroy_drawbridge(x, y, is_disintegrated)
int x, y;
boolean is_disintegrated;
{
    register struct rm *lev1, *lev2;
    struct trap *t;
    struct obj *otmp;
    int x2, y2, i;
    boolean e_inview;
    struct entity *etmp1 = &(occupants[0]), *etmp2 = &(occupants[1]);

    lev1 = &levl[x][y];
    if (!IS_DRAWBRIDGE(lev1->typ))
        return;
    x2 = x;
    y2 = y;
    get_wall_for_db(&x2, &y2);
    lev2 = &levl[x2][y2];
    if ((lev1->drawbridgemask & DB_UNDER) == DB_MOAT
        || (lev1->drawbridgemask & DB_UNDER) == DB_LAVA) 
    {
        struct obj *otmp2;
        boolean lava = (lev1->drawbridgemask & DB_UNDER) == DB_LAVA;

        if (is_disintegrated)
        {
            play_sfx_sound_at_location(SFX_DISINTEGRATE, x2, y2);
            if (cansee(x, y) || cansee(x2, y2))
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "drawbridge disintegrates!");
        }
        else
        {
            play_sfx_sound_at_location(SFX_DRAWBRIDGE_LOUD_SPLASH, x, y);
            if (lev1->typ == DRAWBRIDGE_UP) {
                if (cansee(x2, y2))
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "portcullis of the drawbridge falls into the %s!",
                        lava ? hliquid("lava") : "moat");
                else if (!Deaf)
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a loud *SPLASH*!");
            }
            else 
            {
                if (cansee(x, y))
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "drawbridge collapses into the %s!",
                        lava ? hliquid("lava") : "moat");
                else if (!Deaf)
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a loud *SPLASH*!");
            }
        }
        full_location_transform(x, y, lava ? LAVAPOOL : MOAT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, lev1->floortyp, lev1->floorsubtyp, lev1->floorvartyp, FALSE, FALSE, 0, 0, FALSE);
        if ((otmp2 = sobj_at(BOULDER, x, y)) != 0)
        {
            Strcpy(debug_buf_2, "destroy_drawbridge");
            obj_extract_self(otmp2);
            (void) flooreffects(otmp2, x, y, "fall");
        }
    }
    else
    {
        play_sfx_sound_at_location(SFX_DRAWBRIDGE_LOUD_CRASH, x, y);
        if (cansee(x, y))
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "drawbridge shatters!");
        else
            You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a loud *CRASH*!");

        int trtyp = ((lev1->drawbridgemask & DB_ICE) ? ICE : GROUND);
        int trsubtyp = get_initial_location_subtype(trtyp);
        int trvartyp = get_initial_location_vartype(trtyp, trsubtyp);
        full_location_transform(x, y, trtyp, trsubtyp, trvartyp, ((lev1->drawbridgemask & DB_ICE) ? ICED_MOAT : 0), 0, 0, 0, 0, 0, 0, 0, 0, lev1->floortyp, lev1->floorsubtyp, lev1->floorvartyp, FALSE, FALSE, 0, 0, FALSE);
    }
    wake_nearto(x, y, 500);
    full_location_transform(x2, y2, DOOR, 0, 0, D_PORTCULLIS, 0, 0, 0, 0, 0, 0, 0, 0, lev2->floortyp, lev2->floorsubtyp, lev2->floorvartyp, lev2->facing_right, lev2->horizontal, 0, 0, FALSE);
    if ((t = t_at(x, y)) != 0)
        deltrap(t);
    if ((t = t_at(x2, y2)) != 0)
        deltrap(t);
    del_engr_at(x, y);
    del_engr_at(x2, y2);
    if (!is_disintegrated)
    {
        for (i = rn2(6); i > 0; --i) { /* scatter some debris */
            /* doesn't matter if we happen to pick <x,y2> or <x2,y>;
               since drawbridges are never placed diagonally, those
               pairings will always match one of <x,y> or <x2,y2> */
            otmp = mksobj_at(IRON_CHAIN, rn2(2) ? x : x2, rn2(2) ? y : y2, TRUE,
                             FALSE);
            /* a force of 5 here would yield a radius of 2 for
               iron chain; anything less produces a radius of 1 */
            (void) scatter(otmp->ox, otmp->oy, 1, MAY_HIT, otmp);
        }
    }

    newsym(x, y);
    newsym(x2, y2);
    if (!does_block(x2, y2, lev2))
        unblock_vision_and_hearing_at_point(x2, y2); /* vision */
    if (Is_stronghold(&u.uz))
        u.uevent.uopened_dbridge = TRUE;

    set_entity(x2, y2, etmp2); /* currently only automissers can be here */
    if (etmp2->edata)
    {
        e_inview = e_canseemon(etmp2);
        if (!automiss(etmp2)) 
        {
            if (e_inview)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s hit by flying debris.",
                      E_phrase(etmp2, "are"));

            e_damage(etmp2, d(10, 8), AD_PHYS, ADFLAGS_NONE, "flying debris from a collapsing drawbridge", KILLED_BY);
            //killer.format = KILLED_BY_AN;
            //Strcpy(killer.name, "exploding drawbridge");
            //e_died(etmp2,
            //       XKILL_NOCORPSE | (e_inview ? XKILL_GIVEMSG : XKILL_NOMSG),
            //       CRUSHING); /*no corpse*/
        } /* nothing which is vulnerable can survive this */
    }
    set_entity(x, y, etmp1);
    if (etmp1->edata)
    {
        e_inview = e_canseemon(etmp1);
        if (e_missed(etmp1, TRUE))
        {
            debugpline1("%s spared!", E_phrase(etmp1, "are"));
            /* if there is water or lava here, fall in now */
            if (is_u(etmp1))
                spoteffects(FALSE);
            else
                (void) minliquid(etmp1->emon);
        }
        else 
        {
            if (e_inview) 
            {
                if (!is_u(etmp1) && Hallucination)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s into some heavy metal!",
                          E_phrase(etmp1, "get"));
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s hit by a huge chunk of metal!",
                          E_phrase(etmp1, "are"));
            } 
            else
            {
                if (!Deaf && !is_u(etmp1) && !is_pool(x, y)) 
                {
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a crushing sound.");
                }
                else 
                {
                    debugpline1("%s from shrapnel", E_phrase(etmp1, "die"));
                }
            }
            e_damage(etmp2, d(10, 8), AD_PHYS, ADFLAGS_NONE, "flying debris from a collapsing drawbridge", KILLED_BY);
            //killer.format = KILLED_BY_AN;
            //Strcpy(killer.name, "collapsing drawbridge");
            //e_died(etmp1,
            //       XKILL_NOCORPSE | (e_inview ? XKILL_GIVEMSG : XKILL_NOMSG),
            //       CRUSHING); /*no corpse*/
            //if (levl[etmp1->ex][etmp1->ey].typ == MOAT)
            //    do_entity(etmp1);
        }
    }
    nokiller();
}


void
reset_drawbridge(VOID_ARGS)
{
    memset((genericptr_t)&occupants, 0, sizeof(occupants));
}

/*dbridge.c*/

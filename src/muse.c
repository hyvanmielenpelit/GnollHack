/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    muse.c    $NHDT-Date: 1547025167 2019/01/09 09:12:47 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.92 $ */
/*      Copyright (C) 1990 by Ken Arromdee                         */
/* GnollHack may be freely redistributed.  See license for details.  */

/*
 * Monster item usage routines.
 */

#include "hack.h"

boolean m_using = FALSE;

/* Let monsters use magic items.  Arbitrary assumptions: Monsters only use
 * scrolls when they can see, monsters know when wands have 0 charges,
 * monsters cannot recognize if items are cursed are not, monsters which
 * are confused don't know not to read scrolls, etc....
 */

STATIC_DCL struct permonst *FDECL(muse_newcham_mon, (struct monst *));
STATIC_DCL int FDECL(precheck, (struct monst *, struct obj *));
STATIC_DCL void FDECL(mzapmsg, (struct monst *, struct obj *, BOOLEAN_P));
STATIC_DCL void FDECL(minvokemsg, (struct monst*, struct obj*, BOOLEAN_P));
STATIC_DCL void FDECL(mreadmsg, (struct monst *, struct obj *));
STATIC_DCL void FDECL(mquaffmsg, (struct monst *, struct obj *));
STATIC_DCL boolean FDECL(m_use_healing, (struct monst *));
STATIC_PTR int FDECL(mbhitm, (struct monst *, struct obj *, struct monst*));
STATIC_DCL void FDECL(mbhit, (struct monst *, int,
                              int FDECL((*), (MONST_P, OBJ_P, MONST_P)),
                              int FDECL((*), (OBJ_P, OBJ_P, MONST_P)), struct obj *));
STATIC_DCL void FDECL(you_aggravate, (struct monst *));
STATIC_DCL void FDECL(mon_consume_unstone, (struct monst *, struct obj *,
                                            BOOLEAN_P, BOOLEAN_P));
STATIC_DCL boolean FDECL(cures_stoning, (struct monst *, struct obj *,
                                         BOOLEAN_P));
STATIC_DCL boolean FDECL(mcould_eat_tin, (struct monst *));
STATIC_DCL boolean FDECL(muse_unslime, (struct monst *, struct obj *,
                                        struct trap *, BOOLEAN_P));
STATIC_DCL int FDECL(cures_sliming, (struct monst *, struct obj *));
STATIC_DCL boolean FDECL(green_mon, (struct monst *));

STATIC_VAR struct musable {
    struct obj *offensive;
    struct obj *defensive;
    struct obj *misc;
    int has_offense, has_defense, has_misc;
    /* =0, no capability; otherwise, different numbers.
     * If it's an object, the object is also set (it's 0 otherwise).
     */
} m;
STATIC_VAR int trapx, trapy;
STATIC_VAR boolean zap_oseen; /* for wands which use mbhitm and are zapped at
                           * players.  We usually want an oseen local to
                           * the function, but this is impossible since the
                           * function mbhitm has to be compatible with the
                           * normal zap routines, and those routines don't
                           * remember who zapped the wand. */

/* Any preliminary checks which may result in the monster being unable to use
 * the item.  Returns 0 if nothing happened, 2 if the monster can't do
 * anything (i.e. it teleported) and 1 if it's dead.
 */
STATIC_OVL int
precheck(mon, obj)
struct monst *mon;
struct obj *obj;
{
    boolean vis;

    if (!obj || !mon)
        return 0;
    vis = cansee(mon->mx, mon->my);

    if (obj->oclass == POTION_CLASS) {
        coord cc;
        static const char *empty = "The potion turns out to be empty.";
        const char *potion_descr;
        struct monst *mtmp;

        potion_descr = OBJ_DESCR(objects[obj->otyp]);
        if (potion_descr && !strcmp(potion_descr, "milky")) {
            if (!(mvitals[PM_GHOST].mvflags & MV_GONE)
                && !rn2(POTION_OCCUPANT_CHANCE(mvitals[PM_GHOST].born))) {
                if (!enexto(&cc, mon->mx, mon->my, &mons[PM_GHOST]))
                    return 0;
                mquaffmsg(mon, obj);
                m_useup(mon, obj);
                mtmp = makemon(&mons[PM_GHOST], cc.x, cc.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
                if (!mtmp) {
                    if (vis)
                        pline1(empty);
                } else {
                    boolean frightened = !(is_not_living(mon->data) || is_demon(mon->data) || is_angel(mon->data) || mindless(mon->data) || is_fearless(mon->data));
                    if (vis) 
                    {
                        play_sfx_sound(SFX_SURPRISE_ATTACK);
                        pline_ex(ATR_NONE, CLR_MSG_WARNING,
                            "As %s opens the bottle, an enormous %s emerges!",
                              mon_nam(mon),
                              Hallucination ? rndmonnam(NULL)
                                            : (const char *) "ghost");
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s %s.",
                              Monnam(mon), !frightened ? "looks surprised" : "is frightened to death, and unable to move");
                    }
                    if(frightened)
                        paralyze_monst(mon, 3, FALSE);
                }
                return 2;
            }
        }
        if (potion_descr && !strcmp(potion_descr, "smoky")
            && !(mvitals[PM_DJINNI].mvflags & MV_GONE)
            && !rn2(POTION_OCCUPANT_CHANCE(mvitals[PM_DJINNI].born))) {
            if (!enexto(&cc, mon->mx, mon->my, &mons[PM_DJINNI]))
                return 0;
            mquaffmsg(mon, obj);
            m_useup(mon, obj);
            mtmp = makemon(&mons[PM_DJINNI], cc.x, cc.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
            if (!mtmp) {
                if (vis)
                    pline1(empty);
            } else {
                if (vis)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "In a cloud of smoke, %s emerges!", a_monnam(mtmp));
                pline("%s speaks.", vis ? Monnam(mtmp) : Something);
                if (rn2(2)) {
                    play_monster_special_dialogue_line(mtmp, DJINN_LINE_THANK_YOU_FOR_FREEING);
                    verbalize_happy1("Thank you for freeing me!");
                    play_monster_special_dialogue_line(mtmp, DJINN_LINE_GRANT_ONE_WISH);
                    verbalize_happy1("I am in your debt.  I will grant one wish!");
                    mmake_wish(mon);
                    if (vis)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s vanishes.", Monnam(mtmp));
                    mongone(mtmp);
                } else if (rn2(2)) {
                    play_monster_special_dialogue_line(mtmp, DJINN_LINE_YOU_FREED_ME);
                    verbalize_talk1("You freed me!");
                    mtmp->mpeaceful = 1;
                    set_mhostility(mtmp);
                    newsym(mtmp->mx, mtmp->my);
                } else {
                    play_monster_special_dialogue_line(mtmp, DJINN_LINE_IT_IS_ABOUT_TIME);
                    verbalize_talk1("It is about time.");
                    play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
                    if (vis)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s vanishes.", Monnam(mtmp));
                    mongone(mtmp);
                }
            }
            return 2;
        }
    }
    if (obj->oclass == WAND_CLASS && obj->cursed && !is_cancelled(mon)
        && !rn2(WAND_BACKFIRE_CHANCE))
    {
        int dam = d(obj->charges + 2, 6);

        context.global_minimum_volume = 0.15f;
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_ZAP);
        context.global_minimum_volume = 0.0f;

        play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_MAGICAL, mon->mx, mon->my, 0.15);
        play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, mon->mx, mon->my, FALSE);
        special_effect_wait_until_action(0);

        /* 3.6.1: no Deaf filter; 'if' message doesn't warrant it, 'else'
           message doesn't need it since You_hear() has one of its own */
        if (vis) {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s zaps %s, which suddenly explodes!", Monnam(mon),
                  an(xname(obj)));
        } else {
            /* same near/far threshold as mzapmsg() */
            int range = couldsee(mon->mx, mon->my) /* 9 or 5 */
                           ? NEARBY_CUTOFF_RANGE_CAN_SEE : NEARBY_CUTOFF_RANGE_CANNOT_SEE;

            You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a zap and an explosion %s.",
                     (distu(mon->mx, mon->my) <= range * range)
                        ? "nearby" : "in the distance");
        }
        m_useup(mon, obj);
        special_effect_wait_until_end(0);

        deduct_monster_hp(mon, adjust_damage(dam, (struct monst*)0, mon, AD_MAGM, ADFLAGS_NONE));
        //mon->mhp -= dam;
        if (DEADMONSTER(mon))
        {
            monkilled(mon, "", AD_MAGM, 0);
            return 1;
        }
        m.has_defense = m.has_offense = m.has_misc = 0;
        /* Only one needed to be set to 0 but the others are harmless */
    }
    return 0;
}

STATIC_OVL void
mzapmsg(mtmp, otmp, self)
struct monst *mtmp;
struct obj *otmp;
boolean self;
{
    if (!mtmp || !otmp)
        return;

    context.global_minimum_volume = 0.15f;
    play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_ZAP);
    context.global_minimum_volume = 0.0f;

    if (!canseemon(mtmp)) {
        int range = couldsee(mtmp->mx, mtmp->my) /* 9 or 5 */
                       ? NEARBY_CUTOFF_RANGE_CAN_SEE : NEARBY_CUTOFF_RANGE_CANNOT_SEE;

        You_hear("a %s zap.", (distu(mtmp->mx, mtmp->my) <= range * range)
                                 ? "nearby" : "distant");
    } else if (self) {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s zaps %sself with %s!", Monnam(mtmp), mhim(mtmp),
              doname(otmp));
    } else {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s zaps %s!", Monnam(mtmp), an(xname(otmp)));
        stop_occupation();
    }
}

STATIC_OVL void
minvokemsg(mtmp, otmp, self)
struct monst* mtmp;
struct obj* otmp;
boolean self;
{
    if (!mtmp || !otmp)
        return;

    if (!otmp->oartifact)
    {
        mzapmsg(mtmp, otmp, self);
        return;
    }


    context.global_minimum_volume = 0.15f;
    play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_INVOKE);
    context.global_minimum_volume = 0.0f;

    const char* powertxt = "";
    switch (artilist[otmp->oartifact].inv_prop)
    {
    case ARTINVOKE_WAND_OF_DEATH:
        powertxt = " a death ray from";
        break;
    case ARTINVOKE_RUBY_ROD:
        powertxt = otmp->special_quality == 0 ? " a cone of cold from" : otmp->special_quality == 1 ? " a lightning bolt from" : " a bolt of fire from";
        break;
    default:
        break;
    }

    if (!canseemon(mtmp))
    {
        int range = couldsee(mtmp->mx, mtmp->my) /* 9 or 5 */
            ? NEARBY_CUTOFF_RANGE_CAN_SEE : NEARBY_CUTOFF_RANGE_CANNOT_SEE;

        You_hear("someone invoking%s an artifact %s.", powertxt, (distu(mtmp->mx, mtmp->my) <= range * range)
            ? "nearby" : "at a distance");
    }
    else if (self) 
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s invokes%s %s at %sself!", Monnam(mtmp), powertxt, the(cxname(otmp)), mhim(mtmp));
    }
    else 
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s invokes%s %s!", Monnam(mtmp), powertxt, acxname(otmp));
        stop_occupation();
    }
}

STATIC_OVL void
mreadmsg(mtmp, otmp)
struct monst *mtmp;
struct obj *otmp;
{
    if (!mtmp || !otmp)
        return;

    boolean vismon = canseemon(mtmp);
    char onambuf[BUFSZ];
    short saverole;
    unsigned savebknown;

    if (!vismon && Deaf)
        return; /* no feedback */

    otmp->dknown = 1; /* seeing or hearing it read reveals its label */
    /* shouldn't be able to hear curse/bless status of unseen scrolls;
       for priest characters, bknown will always be set during naming */
    savebknown = otmp->bknown;
    saverole = Role_switch;
    if (!vismon) {
        otmp->bknown = 0;
        if (Role_if(PM_PRIEST))
            Role_switch = 0;
    }
    Strcpy(onambuf, singular(otmp, doname));
    Role_switch = saverole;
    otmp->bknown = savebknown;
    play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_READ);
    if (vismon)
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s reads %s!", Monnam(mtmp), onambuf);
    else
        You_hear("%s reading %s.",
                 x_monnam(mtmp, ARTICLE_A, (char *) 0,
                          (SUPPRESS_IT | SUPPRESS_INVISIBLE | SUPPRESS_SADDLE),
                          FALSE),
                 onambuf);

    if (is_confused(mtmp))
        pline("Being confused, %s mispronounces the magic words...",
              vismon ? mon_nam(mtmp) : mhe(mtmp));
}

STATIC_OVL void
mquaffmsg(mtmp, otmp)
struct monst *mtmp;
struct obj *otmp;
{
    play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_QUAFF);

    if (canseemon(mtmp)) {
        otmp->dknown = 1;
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s drinks %s!", Monnam(mtmp), singular(otmp, doname));
    } else if (!Deaf)
        You_hear("a chugging sound.");
}

/* Defines for various types of stuff.  The order in which monsters prefer
 * to use them is determined by the order of the code logic, not the
 * numerical order in which they are defined.
 */
#define MUSE_SCR_TELEPORTATION 1
#define MUSE_WAN_TELEPORTATION_SELF 2
#define MUSE_POT_HEALING 3
#define MUSE_POT_EXTRA_HEALING 4
#define MUSE_WAN_DIGGING 5
#define MUSE_TRAPDOOR 6
#define MUSE_TELEPORT_TRAP 7
#define MUSE_UPSTAIRS 8
#define MUSE_DOWNSTAIRS 9
#define MUSE_WAN_CREATE_MONSTER 10
#define MUSE_SCR_CREATE_MONSTER 11
#define MUSE_UP_LADDER 12
#define MUSE_DN_LADDER 13
#define MUSE_SSTAIRS 14
#define MUSE_WAN_TELEPORTATION 15
#define MUSE_BUGLE 16
#define MUSE_UNICORN_HORN 17
#define MUSE_POT_GREATER_HEALING 18
#define MUSE_POT_FULL_HEALING 19
#define MUSE_LIZARD_CORPSE 20
#define MUSE_DRAGON_FRUIT 21
#define MUSE_POT_SICKNESS 22

 /*
#define MUSE_INNATE_TPT 9999
 * We cannot use this.  Since monsters get unlimited teleportation, if they
 * were allowed to teleport at will you could never catch them.  Instead,
 * assume they only teleport at random times, despite the inconsistency
 * that if you polymorph into one you teleport at will.
 */

boolean
set_defensive_potion(mtmp, obj)
struct monst* mtmp;
struct obj* obj;
{
    m.defensive = (struct obj*)0;
    m.has_defense = 0;
    if (!mtmp || !obj)
        return FALSE;

    int otyp = obj->otyp;
    switch (otyp)
    {
    case POT_FULL_HEALING:
        m.has_defense = MUSE_POT_FULL_HEALING;
        break;
    case POT_GREATER_HEALING:
        m.has_defense = MUSE_POT_GREATER_HEALING;
        break;
    case POT_EXTRA_HEALING:
        m.has_defense = MUSE_POT_EXTRA_HEALING;
        break;
    case POT_HEALING:
        m.has_defense = MUSE_POT_HEALING;
        break;
    default:
        break;
    }
    if (m.has_defense != 0)
        m.defensive = obj;
    return (m.has_defense != 0);
}



STATIC_OVL boolean
m_use_healing(mtmp)
struct monst *mtmp;
{
    struct obj *obj = 0;
    if ((obj = m_carrying(mtmp, POT_FULL_HEALING)) != 0) {
        m.defensive = obj;
        m.has_defense = MUSE_POT_FULL_HEALING;
        return TRUE;
    }
    if ((obj = m_carrying(mtmp, POT_GREATER_HEALING)) != 0) {
        m.defensive = obj;
        m.has_defense = MUSE_POT_GREATER_HEALING;
        return TRUE;
    }
    if ((obj = m_carrying(mtmp, POT_EXTRA_HEALING)) != 0) {
        m.defensive = obj;
        m.has_defense = MUSE_POT_EXTRA_HEALING;
        return TRUE;
    }
    if ((obj = m_carrying(mtmp, POT_HEALING)) != 0) {
        m.defensive = obj;
        m.has_defense = MUSE_POT_HEALING;
        return TRUE;
    }
    return FALSE;
}

/* Select a defensive item/action for a monster.  Returns TRUE iff one is
   found. */
boolean
find_defensive(mtmp)
struct monst *mtmp;
{
    register struct obj *obj = 0;
    struct trap *t;
    int x = mtmp->mx, y = mtmp->my;
    boolean stuck = (mtmp == u.ustuck);
    boolean immobile = (mtmp->data->mmove == 0);
    int fraction;

    if (is_animal(mtmp->data) || mindless(mtmp->data))
        return FALSE;
    if (dist2(x, y, mtmp->mux, mtmp->muy) > 25)
        return FALSE;
    if (u.uswallow && stuck)
        return FALSE;

    m.defensive = (struct obj *) 0;
    m.has_defense = 0;

    /* since unicorn horns don't get used up, the monster would look
     * silly trying to use the same cursed horn round after round
     */
    if (is_confused(mtmp) || is_stunned(mtmp) || is_blinded(mtmp))
    {
        if (!is_unicorn(mtmp->data) && can_operate_objects(mtmp->data)) 
        {
            for (obj = mtmp->minvent; obj; obj = obj->nobj)
                if (obj->otyp == UNICORN_HORN && !obj->cursed)
                    break;
        }
        if (obj || is_unicorn(mtmp->data)) 
        {
            m.defensive = obj;
            m.has_defense = MUSE_UNICORN_HORN;
            return TRUE;
        }
    }

    if (has_stoned(mtmp)) 
    {
        struct obj *liztin = 0;

        for (obj = mtmp->minvent; obj; obj = obj->nobj)
        {
            if (obj->otyp == CORPSE && obj->corpsenm == PM_LIZARD)
            {
                m.defensive = obj;
                m.has_defense = MUSE_LIZARD_CORPSE;
                return TRUE;
            } 
            else if (obj->otyp == TIN && obj->corpsenm == PM_LIZARD) 
            {
                liztin = obj;
            }
        }

        for (obj = mtmp->minvent; obj; obj = obj->nobj)
        {
            if (obj->otyp == DRAGON_FRUIT)
            {
                m.defensive = obj;
                m.has_defense = MUSE_DRAGON_FRUIT;
                return TRUE;
            }
        }
        /* confused or stunned monster might not be able to open tin */
        if (liztin && mcould_eat_tin(mtmp) && rn2(3))
        {
            m.defensive = liztin;
            /* tin and corpse ultimately end up being handled the same */
            m.has_defense = MUSE_LIZARD_CORPSE;
            return TRUE;
        }
    }

    /* It so happens there are two unrelated cases when we might want to
     * check specifically for healing alone.  The first is when the monster
     * is blind (healing cures blindness).  The second is when the monster
     * is peaceful; then we don't want to flee the player, and by
     * coincidence healing is all there is that doesn't involve fleeing.
     * These would be hard to combine because of the control flow.
     * Pestilence won't use healing even when blind.
     */
    if (is_blinded(mtmp) && can_operate_objects(mtmp->data)
        && mtmp->data != &mons[PM_PESTILENCE]) 
    {
        if (m_use_healing(mtmp))
            return TRUE;
    }

    fraction = u.ulevel < 10 ? 5 : u.ulevel < 14 ? 4 : 3;
    if (mtmp->mhp >= mtmp->mhpmax
        || (mtmp->mhp >= 10 && mtmp->mhp * fraction >= mtmp->mhpmax))
        return FALSE;

    if (is_peaceful(mtmp))
    {
        if (can_operate_objects(mtmp->data)) 
        {
            if (m_use_healing(mtmp))
                return TRUE;
        }
        return FALSE;
    }

    if (stuck || immobile) 
    {
        ; /* fleeing by stairs or traps is not possible */
    }
    else if (levl[x][y].typ == STAIRS) 
    {
        if (x == xdnstair && y == ydnstair)
        {
            if (!is_floater(mtmp->data))
                m.has_defense = MUSE_DOWNSTAIRS;
        } else if (x == xupstair && y == yupstair) 
        {
            m.has_defense = MUSE_UPSTAIRS;
        } 
        else if (sstairs.sx && x == sstairs.sx && y == sstairs.sy) 
        {
            if (sstairs.up || !is_floater(mtmp->data))
                m.has_defense = MUSE_SSTAIRS;
        }
    }
    else if (levl[x][y].typ == LADDER) 
    {
        if (x == xupladder && y == yupladder)
        {
            m.has_defense = MUSE_UP_LADDER;
        }
        else if (x == xdnladder && y == ydnladder) 
        {
            if (!is_floater(mtmp->data))
                m.has_defense = MUSE_DN_LADDER;
        } 
        else if (sstairs.sx && x == sstairs.sx && y == sstairs.sy)
        {
            if (sstairs.up || !is_floater(mtmp->data))
                m.has_defense = MUSE_SSTAIRS;
        }
    } 
    else 
    {
        /* Note: trap doors take precedence over teleport traps. */
        int xx, yy, i, locs[10][2];
        boolean ignore_boulders = (verysmall(mtmp->data)
                                   || throws_rocks(mtmp->data)
                                   || passes_walls(mtmp->data)),
            diag_ok = !NODIAG(mtmp->mnum);

        for (i = 0; i < 10; ++i) /* 10: 9 spots plus sentinel */
            locs[i][0] = locs[i][1] = 0;
        /* collect viable spots; monster's <mx,my> comes first */
        locs[0][0] = x, locs[0][1] = y;
        i = 1;
        for (xx = x - 1; xx <= x + 1; xx++)
            for (yy = y - 1; yy <= y + 1; yy++)
                if (isok(xx, yy) && (xx != x || yy != y)) 
                {
                    locs[i][0] = xx, locs[i][1] = yy;
                    ++i;
                }
        /* look for a suitable trap among the viable spots */
        for (i = 0; i < 10; ++i)
        {
            xx = locs[i][0], yy = locs[i][1];
            if (!xx)
                break; /* we've run out of spots */
            /* skip if it's hero's location
               or a diagonal spot and monster can't move diagonally
               or some other monster is there */
            if ((xx == u.ux && yy == u.uy)
                || (xx != x && yy != y && !diag_ok)
                || (level.monsters[xx][yy] && !(xx == x && yy == y)))
                continue;
            /* skip if there's no trap or can't/won't move onto trap */
            if ((t = t_at(xx, yy)) == 0
                || (!ignore_boulders && sobj_at(BOULDER, xx, yy))
                || onscary(xx, yy, mtmp))
                continue;
            /* use trap if it's the correct type */
            if (is_hole(t->ttyp)
                && !is_floater(mtmp->data)
                && !mtmp->isshk && !mtmp->isgd && !mtmp->ispriest && !mtmp->issmith && !mtmp->isnpc
                && Can_fall_thru(&u.uz)) {
                trapx = xx;
                trapy = yy;
                m.has_defense = MUSE_TRAPDOOR;
                break; /* no need to look at any other spots */
            }
            else if (t->ttyp == TELEP_TRAP) 
            {
                trapx = xx;
                trapy = yy;
                m.has_defense = MUSE_TELEPORT_TRAP;
            }
        }
    }

    if (!can_operate_objects(mtmp->data)) /* can't use objects */
        goto botm;

    if (is_mercenary(mtmp->data) && (obj = m_carrying(mtmp, BUGLE)) != 0)
    {
        int xx, yy;
        struct monst *mon;

        /* Distance is arbitrary.  What we really want to do is
         * have the soldier play the bugle when it sees or
         * remembers soldiers nearby...
         */
        for (xx = x - 3; xx <= x + 3; xx++) 
        {
            for (yy = y - 3; yy <= y + 3; yy++)
            {
                if (!isok(xx, yy) || (xx == x && yy == y))
                    continue;
                if ((mon = m_at(xx, yy)) != 0 && is_mercenary(mon->data)
                    && mon->data != &mons[PM_GUARD]
                    && !mon_can_move(mon))
                {
                    m.defensive = obj;
                    m.has_defense = MUSE_BUGLE;
                    goto toot; /* double break */
                }
            }
        }
 toot:
        ;
    }

    /* use immediate physical escape prior to attempting magic */
    if (m.has_defense) /* stairs, trap door or tele-trap, bugle alert */
        goto botm;

    /* kludge to cut down on trap destruction (particularly portals) */
    t = t_at(x, y);
    if (t && (is_pit(t->ttyp) || t->ttyp == WEB
              || t->ttyp == BEAR_TRAP))
        t = 0; /* ok for monster to dig here */

#define nomore(x)       if (m.has_defense == x) continue;
    /* selection could be improved by collecting all possibilities
       into an array and then picking one at random */
    for (obj = mtmp->minvent; obj; obj = obj->nobj) 
    {
        /* don't always use the same selection pattern */
        if (m.has_defense && !rn2(3))
            break;

        /* nomore(MUSE_WAN_DIGGING); */
        if (m.has_defense == MUSE_WAN_DIGGING)
            break;
        if (obj->otyp == WAN_DIGGING && obj->charges > 0 && !is_cancelled(mtmp) && !stuck && !t
            && !mtmp->isshk && !mtmp->isgd && !mtmp->ispriest && !mtmp->issmith && !mtmp->isnpc
            && !is_floater(mtmp->data)
            /* monsters digging in Sokoban can ruin things */
            && !Sokoban
            /* digging wouldn't be effective; assume they know that */
            && !(levl[x][y].wall_info & W_NONDIGGABLE)
            && !(Is_botlevel(&u.uz) || In_endgame(&u.uz))
            && !(is_ice(x, y) || is_pool(x, y) || is_lava(x, y) || IS_AIR(levl[x][y].typ))
            && !(mtmp->data == &mons[PM_VLAD_THE_IMPALER]
                 && In_V_tower(&u.uz)))
        {
            m.defensive = obj;
            m.has_defense = MUSE_WAN_DIGGING;
        }
        nomore(MUSE_WAN_TELEPORTATION_SELF);
        nomore(MUSE_WAN_TELEPORTATION);
        if (obj->otyp == WAN_TELEPORTATION && obj->charges > 0 && !is_cancelled(mtmp) && !level.flags.noteleport)
        {
            /* use the TELEP_TRAP bit to determine if they know
             * about noteleport on this level or not.  Avoids
             * ineffective re-use of teleportation.  This does
             * mean if the monster leaves the level, they'll know
             * about teleport traps.
             */
            if (!level.flags.noteleport
                || !(mtmp->mtrapseen & (1 << (TELEP_TRAP - 1)))) 
            {
                m.defensive = obj;
                m.has_defense = (mon_has_amulet(mtmp))
                                    ? MUSE_WAN_TELEPORTATION
                                    : MUSE_WAN_TELEPORTATION_SELF;
            }
        }
        nomore(MUSE_SCR_TELEPORTATION);
        if (obj->otyp == SCR_TELEPORTATION && !is_blinded(mtmp)
            && haseyes(mtmp->data) && !level.flags.noteleport
            && (!obj->cursed || (!(mtmp->isshk && inhishop(mtmp))
                                 && !mtmp->isgd && !mtmp->ispriest && !mtmp->issmith && !mtmp->isnpc)))
        {
            /* see WAN_TELEPORTATION case above */
            if (!level.flags.noteleport
                || !(mtmp->mtrapseen & (1 << (TELEP_TRAP - 1)))) 
            {
                m.defensive = obj;
                m.has_defense = MUSE_SCR_TELEPORTATION;
            }
        }

        if (mtmp->data != &mons[PM_PESTILENCE])
        {
            nomore(MUSE_POT_FULL_HEALING);
            if (obj->otyp == POT_FULL_HEALING) 
            {
                m.defensive = obj;
                m.has_defense = MUSE_POT_FULL_HEALING;
            }
            nomore(MUSE_POT_GREATER_HEALING);
            if (obj->otyp == POT_GREATER_HEALING) 
            {
                m.defensive = obj;
                m.has_defense = MUSE_POT_GREATER_HEALING;
            }
            nomore(MUSE_POT_EXTRA_HEALING);
            if (obj->otyp == POT_EXTRA_HEALING) 
            {
                m.defensive = obj;
                m.has_defense = MUSE_POT_EXTRA_HEALING;
            }
            nomore(MUSE_WAN_CREATE_MONSTER);
            if (obj->otyp == WAN_CREATE_MONSTER && obj->charges > 0 && !is_cancelled(mtmp))
            {
                m.defensive = obj;
                m.has_defense = MUSE_WAN_CREATE_MONSTER;
            }
            nomore(MUSE_POT_HEALING);
            if (obj->otyp == POT_HEALING) 
            {
                m.defensive = obj;
                m.has_defense = MUSE_POT_HEALING;
            }
        }
        else
        { /* Pestilence */
            nomore(MUSE_POT_SICKNESS);
            if (obj->otyp == POT_SICKNESS) 
            {
                m.defensive = obj;
                m.has_defense = MUSE_POT_SICKNESS;
            }
            nomore(MUSE_WAN_CREATE_MONSTER);
            if (obj->otyp == WAN_CREATE_MONSTER && obj->charges > 0 && !is_cancelled(mtmp))
            {
                m.defensive = obj;
                m.has_defense = MUSE_WAN_CREATE_MONSTER;
            }
        }
        nomore(MUSE_SCR_CREATE_MONSTER);
        if (obj->otyp == SCR_CREATE_MONSTER)
        {
            m.defensive = obj;
            m.has_defense = MUSE_SCR_CREATE_MONSTER;
        }
    }
 botm:
    return (boolean) !!m.has_defense;
#undef nomore
}

/* Perform a defensive action for a monster.  Must be called immediately
 * after find_defensive().  Return values are 0: did something, 1: died,
 * 2: did something and can't attack again (i.e. teleported).
 */
int
use_defensive(mtmp)
struct monst *mtmp;
{
    if (!mtmp)
        return 0;

    int i, fleetim, how = 0;
    struct obj *otmp = m.defensive;
    boolean vis, vismon, oseen;
    const char *Mnam;

    if ((i = precheck(mtmp, otmp)) != 0)
        return i;

    int duration = 0, dicebuc = 0, extra_data1 = 0;
    boolean cures_sick = FALSE;
    boolean cures_blind = FALSE;
    boolean cures_hallucination = FALSE;
    boolean cures_stun = FALSE;
    boolean cures_confusion = FALSE;
    if (otmp)
    {
        int otyp = otmp->otyp;
        if (mtmp->mnum == PM_PESTILENCE && otyp == POT_SICKNESS)
            otyp = POT_FULL_HEALING;
        dicebuc = (int)(otmp->oclass == POTION_CLASS ? objects[otyp].oc_potion_normal_dice_buc_multiplier : 0);
        duration =
            (int)max(0,
                otmp->oclass == POTION_CLASS ? (objects[otyp].oc_potion_normal_diesize == 0 ? 0 : d(max(0, objects[otyp].oc_potion_normal_dice + dicebuc * bcsign(otmp)), max(1, objects[otyp].oc_potion_normal_diesize))) + objects[otyp].oc_potion_normal_plus + bcsign(otmp) * objects[otyp].oc_potion_normal_buc_multiplier :
                d(objects[otyp].oc_spell_dur_dice, objects[otyp].oc_spell_dur_diesize) + objects[otyp].oc_spell_dur_plus
            );
        extra_data1 = otmp->oclass == POTION_CLASS ? (int)objects[otyp].oc_potion_extra_data1 : 0;
        /* Adjustment for dilution */
        boolean isdiluted = otmp->oclass == POTION_CLASS && otmp->odiluted;
        if (isdiluted)
        {
            duration /= 2;
            extra_data1 /= 2;
        }
        if (objects[otmp->otyp].oc_flags5 & O5_EFFECT_FLAGS_ARE_HEALING)
        {
            cures_sick = otmp->blessed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_SICKNESS) :
                otmp->cursed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_SICKNESS) :
                !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_SICKNESS);
            cures_blind = otmp->blessed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_BLINDNESS) :
                otmp->cursed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_BLINDNESS) :
                !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_BLINDNESS);
            cures_hallucination = otmp->blessed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_HALLUCINATION) :
                otmp->cursed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_HALLUCINATION) :
                !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_HALLUCINATION);
            cures_stun = otmp->blessed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_STUN) :
                otmp->cursed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_STUN) :
                !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_STUN);
            cures_confusion = otmp->blessed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_CONFUSION) :
                otmp->cursed ? !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_CONFUSION) :
                !!(objects[otmp->otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_CONFUSION);
        }
    }
    vis = cansee(mtmp->mx, mtmp->my);
    vismon = canseemon(mtmp);
    oseen = otmp && vismon;

    /* when using defensive choice to run away, we want monster to avoid
       rushing right straight back; don't override if already scared */
    fleetim = !is_fleeing(mtmp) ? (33 - (30 * mtmp->mhp / mtmp->mhpmax)) : 0;
#define m_flee(m)                          \
    if (fleetim && !m->iswiz) {            \
        monflee(m, fleetim, FALSE, FALSE); \
    }

    switch (m.has_defense) {
    case MUSE_UNICORN_HORN:
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_APPLY);
        if (vismon) {
            if (otmp)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s uses a unicorn horn!", Monnam(mtmp));
            else
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "tip of %s's horn glows!", mon_nam(mtmp));
        }
        if (is_blinded(mtmp)) {
            mcureblindness(mtmp, vismon);
        } else if (is_confused(mtmp) || is_stunned(mtmp)) {
            mtmp->mprops[CONFUSION] = 0;
            mtmp->mprops[STUNNED] = 0;
            if (vismon)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems steadier now.", Monnam(mtmp));
        } else
            impossible("No need for unicorn horn?");
        return 2;
    case MUSE_BUGLE:
        if (!otmp)
            return 2;

        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_APPLY);
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s plays %s!", Monnam(mtmp), doname(otmp));
        else if (!Deaf)
            You_hear("a bugle playing reveille!");

        boolean is_scary = FALSE;
        if (otmp->charges > 0)
        {
            otmp->charges--;
            is_scary = TRUE;
        }
        awaken_soldiers(mtmp, is_scary);
        return 2;
    case MUSE_WAN_TELEPORTATION_SELF:
        if ((mtmp->isshk && inhishop(mtmp)) || mtmp->isgd || mtmp->ispriest || mtmp->issmith || mtmp->isnpc)
            return 2;
        m_flee(mtmp);
        if (!otmp)
            return 2;
        mzapmsg(mtmp, otmp, TRUE);
        otmp->charges--;
        how = WAN_TELEPORTATION;
 mon_tele:
        if (tele_restrict(mtmp)) { /* mysterious force... */
            if (vismon && how)     /* mentions 'teleport' */
                makeknown(how);
            /* monster learns that teleportation isn't useful here */
            if (level.flags.noteleport)
                mtmp->mtrapseen |= (1 << (TELEP_TRAP - 1));
            return 2;
        }
        if ((mon_has_amulet(mtmp) || On_W_tower_level(&u.uz)) && !rn2(3)) {
            if (vismon)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems disoriented for a moment.", Monnam(mtmp));
            return 2;
        }
        if (oseen && how)
            makeknown(how);

        (void)rloc2(mtmp, TRUE, TRUE);
        return 2;
    case MUSE_WAN_TELEPORTATION:
        zap_oseen = oseen;
        if (!otmp)
            return 2;
        mzapmsg(mtmp, otmp, FALSE);
        otmp->charges--;
        m_using = TRUE;
        mbhit(mtmp, rn1(8, 6), mbhitm, bhito, otmp);
        /* monster learns that teleportation isn't useful here */
        if (level.flags.noteleport)
            mtmp->mtrapseen |= (1 << (TELEP_TRAP - 1));
        m_using = FALSE;
        return 2;
    case MUSE_SCR_TELEPORTATION: {
        if (!otmp)
            return 2;

        int obj_is_cursed = otmp->cursed;

        if (mtmp->isshk || mtmp->isgd || mtmp->ispriest || mtmp->issmith || mtmp->isnpc)
            return 2;
        m_flee(mtmp);
        mreadmsg(mtmp, otmp);
        m_useup(mtmp, otmp); /* otmp might be free'ed */
        how = SCR_TELEPORTATION;
        if (obj_is_cursed || is_confused(mtmp)) {
            int nlev;
            d_level flev;

            if (mon_has_amulet(mtmp) || In_endgame(&u.uz)) {
                if (vismon)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems very disoriented for a moment.",
                          Monnam(mtmp));
                return 2;
            }
            nlev = random_teleport_level();
            if (nlev == depth(&u.uz)) {
                if (vismon)
                    pline("%s shudders for a moment.", Monnam(mtmp));
                return 2;
            }
            get_level(&flev, nlev);
            level_teleport_effect_out(mtmp->mx, mtmp->my);
            migrate_to_level(mtmp, ledger_no(&flev), MIGR_RANDOM,
                             (coord *) 0);
            if (oseen)
                makeknown(SCR_TELEPORTATION);
        } else
            goto mon_tele;
        return 2;
    }
    case MUSE_WAN_DIGGING: {
        struct trap *ttmp;
        m_flee(mtmp);
        if (!otmp)
            return 2;
        mzapmsg(mtmp, otmp, FALSE);
        otmp->charges--;
        if (oseen)
            makeknown(WAN_DIGGING);
        if (IS_FURNITURE(levl[mtmp->mx][mtmp->my].typ)
            || IS_DRAWBRIDGE(levl[mtmp->mx][mtmp->my].typ)
            || (is_drawbridge_wall(mtmp->mx, mtmp->my) >= 0)
            || (sstairs.sx && sstairs.sx == mtmp->mx
                && sstairs.sy == mtmp->my)) {
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "digging ray is ineffective.");
            return 2;
        }
        if (!Can_dig_down(&u.uz) && !levl[mtmp->mx][mtmp->my].candig) {
            if (canseemon(mtmp))
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s here is too hard to dig in.",
                          surface(mtmp->mx, mtmp->my));
            return 2;
        }
        ttmp = maketrap(mtmp->mx, mtmp->my, HOLE, NON_PM, MKTRAPFLAG_MADE_BY_MON);
        if (!ttmp)
            return 2;
        seetrap(ttmp);
        if (vis) {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s has made a hole in the %s.", Monnam(mtmp),
                  surface(mtmp->mx, mtmp->my));
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s through...", Monnam(mtmp),
                  is_flying(mtmp) ? "dives" : "falls");
        } else if (!Deaf)
            You_hear("%s crash through the %s.", something,
                     surface(mtmp->mx, mtmp->my));
        /* we made sure that there is a level for mtmp to go to */
        play_sfx_sound_at_location(SFX_TRAP_DOOR_OPENS, mtmp->mx, mtmp->my);
        migrate_to_level(mtmp, ledger_no(&u.uz) + 1, MIGR_RANDOM,
                         (coord *) 0);
        return 2;
    }
    case MUSE_WAN_CREATE_MONSTER: {
        if (!otmp)
            return 2;
        coord cc;
        /* pm: 0 => random, eel => aquatic, croc => amphibious */
        struct permonst *pm =
            !is_pool(mtmp->mx, mtmp->my)
                ? 0
                : &mons[u.uinwater ? PM_GIANT_EEL : PM_CROCODILE];
        struct monst *mon;

        if (!enexto(&cc, mtmp->mx, mtmp->my, pm))
            return 0;
        mzapmsg(mtmp, otmp, FALSE);
        otmp->charges--;
        mon = makemon2((struct permonst *) 0, cc.x, cc.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END, MM2_RANDOMIZE_SUBTYPE);
        if (mon && canspotmon(mon) && oseen)
            makeknown(WAN_CREATE_MONSTER);
        return 2;
    }
    case MUSE_SCR_CREATE_MONSTER: {
        if (!otmp)
            return 2;
        coord cc;
        struct permonst *pm = 0, *fish = 0;
        int cnt = 1;
        struct monst *mon;
        boolean known = FALSE;

        if (!rn2(73))
            cnt += rnd(4);
        if (is_confused(mtmp) || otmp->cursed)
            cnt += 12;
        if (is_confused(mtmp))
            pm = fish = &mons[PM_ACID_BLOB];
        else if (is_pool(mtmp->mx, mtmp->my))
            fish = &mons[u.uinwater ? PM_GIANT_EEL : PM_CROCODILE];
        mreadmsg(mtmp, otmp);

        context.makemon_spef_idx = 0;
        while (cnt--) {
            /* `fish' potentially gives bias towards water locations;
               `pm' is what to actually create (0 => random) */
            if (!enexto(&cc, mtmp->mx, mtmp->my, fish))
                break;
            mon = makemon(pm, cc.x, cc.y, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
            if (mon)
            {
                context.makemon_spef_idx++;
                if(canspotmon(mon))
                    known = TRUE;
            }
        }
        makemon_animation_wait_until_end();

        /* The only case where we don't use oseen.  For wands, you
         * have to be able to see the monster zap the wand to know
         * what type it is.  For teleport scrolls, you have to see
         * the monster to know it teleported.
         */
        if (known)
            makeknown(SCR_CREATE_MONSTER);
        else if (!objects[SCR_CREATE_MONSTER].oc_name_known
                 && !objects[SCR_CREATE_MONSTER].oc_uname)
            docall(otmp, (char*)0);
        m_useup(mtmp, otmp);
        return 2;
    }
    case MUSE_TRAPDOOR:
        /* trap doors on "bottom" levels of dungeons are rock-drop
         * trap doors, not holes in the floor.  We check here for
         * safety.
         */
        if (Is_botlevel(&u.uz))
            return 0;
        m_flee(mtmp);
        if (vis) {
            struct trap *t = t_at(trapx, trapy);

            Mnam = Monnam(mtmp);
            play_sfx_sound_at_location((t->ttyp == TRAPDOOR) ? SFX_TRAP_DOOR_OPENS : SFX_HOLE_OPENS, trapx, trapy);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s into %s!", Mnam,
                  vtense(Mnam, locomotion(mtmp->data, "jump")), an(trap_type_definitions[t->ttyp].type_name));
            if (levl[trapx][trapy].typ == SCORR) {
                levl[trapx][trapy].typ = CORR;
                unblock_vision_and_hearing_at_point(trapx, trapy);
            }
            seetrap(t_at(trapx, trapy));
        }

        /*  don't use rloc_to() because worm tails must "move" */
        remove_monster(mtmp->mx, mtmp->my);
        newsym(mtmp->mx, mtmp->my); /* update old location */
        place_monster(mtmp, trapx, trapy);
        if (mtmp->wormno)
            worm_move(mtmp);
        newsym(trapx, trapy);
        play_sfx_sound_at_location(SFX_TRAP_DOOR_OPENS, mtmp->mx, mtmp->my);
        migrate_to_level(mtmp, ledger_no(&u.uz) + 1, MIGR_RANDOM,
                         (coord *) 0);
        return 2;
    case MUSE_UPSTAIRS:
        m_flee(mtmp);
        if (ledger_no(&u.uz) == 1)
            goto escape; /* impossible; level 1 upstairs are SSTAIRS */
#if 0
        if (Inhell && mon_has_amulet(mtmp) && !rn2(4)
            && (dunlev(&u.uz) < dunlevs_in_dungeon(&u.uz) - 3)) {
            if (vismon)
                pline(
    "As %s climbs the stairs, a mysterious force momentarily surrounds %s...",
                      mon_nam(mtmp), mhim(mtmp));
            /* simpler than for the player; this will usually be
               the Wizard and he'll immediately go right to the
               upstairs, so there's not much point in having any
               chance for a random position on the current level */
            play_movement_sound(mtmp, CLIMBING_TYPE_STAIRS_UP);
            migrate_to_level(mtmp, ledger_no(&u.uz) + 1, MIGR_RANDOM,
                             (coord *) 0);
        } else {
            if (vismon)
                pline("%s escapes upstairs!", Monnam(mtmp));
            play_movement_sound(mtmp, CLIMBING_TYPE_STAIRS_UP);
            migrate_to_level(mtmp, ledger_no(&u.uz) - 1, MIGR_STAIRS_DOWN,
                             (coord *) 0);
        }
#endif
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s escapes upstairs!", Monnam(mtmp));
        play_movement_sound(mtmp, CLIMBING_TYPE_STAIRS_UP);
        migrate_to_level(mtmp, ledger_no(&u.uz) - 1, MIGR_STAIRS_DOWN,
            (coord*)0);

        return 2;
    case MUSE_DOWNSTAIRS:
        m_flee(mtmp);
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s escapes downstairs!", Monnam(mtmp));
        play_movement_sound(mtmp, CLIMBING_TYPE_STAIRS_DOWN);
        migrate_to_level(mtmp, ledger_no(&u.uz) + 1, MIGR_STAIRS_UP,
                         (coord *) 0);
        return 2;
    case MUSE_UP_LADDER:
        m_flee(mtmp);
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s escapes up the ladder!", Monnam(mtmp));
        play_movement_sound(mtmp, CLIMBING_TYPE_LADDER_UP);
        migrate_to_level(mtmp, ledger_no(&u.uz) - 1, MIGR_LADDER_DOWN,
                         (coord *) 0);
        return 2;
    case MUSE_DN_LADDER:
        m_flee(mtmp);
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s escapes down the ladder!", Monnam(mtmp));
        play_movement_sound(mtmp, CLIMBING_TYPE_LADDER_DOWN);
        migrate_to_level(mtmp, ledger_no(&u.uz) + 1, MIGR_LADDER_UP,
                         (coord *) 0);
        return 2;
    case MUSE_SSTAIRS:
        m_flee(mtmp);
        if (ledger_no(&u.uz) == 1) {
 escape:
            /* Monsters without the Amulet escape the dungeon and
             * are gone for good when they leave up the up stairs.
             * A monster with the Amulet would leave it behind
             * (mongone -> mdrop_special_objs) but we force any
             * monster who manages to acquire it or the invocation
             * tools to stick around instead of letting it escape.
             */
            if (mon_has_special(mtmp))
                return 0;
            if (vismon)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s escapes the dungeon!", Monnam(mtmp));
            play_movement_sound(mtmp, sstairs.up ? CLIMBING_TYPE_STAIRS_UP : CLIMBING_TYPE_STAIRS_DOWN);
            mongone(mtmp);
            return 2;
        }
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s escapes %sstairs!", Monnam(mtmp),
                  sstairs.up ? "up" : "down");
        /* going from the Valley to Castle (Stronghold) has no sstairs
           to target, but having sstairs.<sx,sy> == <0,0> will work the
           same as specifying MIGR_RANDOM when mon_arrive() eventually
           places the monster, so we can use MIGR_SSTAIRS unconditionally */
        play_movement_sound(mtmp, sstairs.up ? CLIMBING_TYPE_STAIRS_UP : CLIMBING_TYPE_STAIRS_DOWN);
        migrate_to_level(mtmp, ledger_no(&sstairs.tolev), MIGR_SSTAIRS,
                         (coord *) 0);
        return 2;
    case MUSE_TELEPORT_TRAP:
        m_flee(mtmp);
        if (vis)
        {
            Mnam = Monnam(mtmp);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s onto a teleport trap!", Mnam,
                  vtense(Mnam, locomotion(mtmp->data, "jump")));
            seetrap(t_at(trapx, trapy));
        }
        /*  don't use rloc_to() because worm tails must "move" */
        remove_monster(mtmp->mx, mtmp->my);
        newsym(mtmp->mx, mtmp->my); /* update old location */
        place_monster(mtmp, trapx, trapy);
        if (mtmp->wormno)
            worm_move(mtmp);
        newsym(trapx, trapy);

        goto mon_tele;
    case MUSE_POT_SICKNESS: /* Pestilence */
        if (!otmp)
            return 2;
        if (mtmp->mnum != PM_PESTILENCE)
        {
            /* Should not happen */
            mquaffmsg(mtmp, otmp);
            m_useup(mtmp, otmp);
            break;
        }
        /* FALLTHRU */
    case MUSE_POT_HEALING:
    case MUSE_POT_EXTRA_HEALING:
    case MUSE_POT_GREATER_HEALING:
    case MUSE_POT_FULL_HEALING:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_HEALING, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);
        deduct_monster_hp(mtmp, (double)(-duration));
        if (mtmp->mhp > mtmp->mhpmax)
            mtmp->mhp = (mtmp->mhpmax += (otmp->blessed ? extra_data1 : 0));
        if (cures_blind && is_blinded(mtmp))
            mcureblindness(mtmp, vismon);
        if (cures_sick)
            mcuresickness(mtmp, vismon);
        if (cures_stun)
            mcurestun(mtmp, vismon);
        if (cures_hallucination)
            mcurehallucination(mtmp, vismon);
        if (cures_confusion)
            mcureconfusion(mtmp, vismon);
        if (vismon)
        {
            const char* fmt = "%s looks better.";
            switch(otmp->otyp)
            {
            case POT_HEALING:
                fmt = "%s looks better.";
                break;
            case POT_EXTRA_HEALING:
                fmt = "%s looks much better.";
                break;
            case POT_GREATER_HEALING:
                fmt = "%s looks much, much better.";
                break;
            case POT_SICKNESS: /* Pestilence */
            case POT_FULL_HEALING:
                fmt = "%s looks completely healed.";
                break;
            }
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : NO_COLOR, fmt, Monnam(mtmp));
        }
        if (oseen)
            makeknown(otmp->otyp);
        m_useup(mtmp, otmp);
        special_effect_wait_until_end(0);
        return 2;
    case MUSE_DRAGON_FRUIT:
    case MUSE_LIZARD_CORPSE:
        if (!otmp)
            return 2;
        /* not actually called for its unstoning effect */
        mon_consume_unstone(mtmp, otmp, FALSE, FALSE);
        return 2;
    case 0:
        return 0; /* i.e. an exploded wand */
    default:
        impossible("%s wanted to perform action %d?", Monnam(mtmp),
                   m.has_defense);
        break;
    }
    return 0;
#undef m_flee
}

int
rnd_defensive_item(mtmp)
struct monst *mtmp;
{
    struct permonst *pm = mtmp->data;
    int difficulty = mons[mtmp->mnum].difficulty;
    int trycnt = 0;
    int roll = 0;

    if (is_animal(pm) || attacktype(pm, AT_EXPL) || mindless(mtmp->data)
        || pm->mlet == S_GHOST || pm->mlet == S_KOP)
        return 0;
try_again:
    roll = rn2(8 + (difficulty > 3) + (difficulty > 6) + (difficulty > 8));
    switch (roll) {
    case 6:
    case 9:
        if (level.flags.noteleport && ++trycnt < 2)
            goto try_again;
        if (roll >= 9 ? 1 : !rn2(3))
            return WAN_TELEPORTATION;
        /*FALLTHRU*/
    case 0:
        return SCR_TELEPORTATION;
    case 1:
        return POT_HEALING; /* One scroll of teleportation changed to potion of healing */
    case 8:
    case 10:
        if (!rn2(3))
            return WAN_CREATE_MONSTER;
        /*FALLTHRU*/
    case 2:
        return SCR_CREATE_MONSTER;
    case 3:
        return POT_HEALING;
    case 4:
        return POT_EXTRA_HEALING;
    case 5:
        return (mtmp->data != &mons[PM_PESTILENCE]) ? (rn2(3) ? POT_GREATER_HEALING : POT_FULL_HEALING) : POT_SICKNESS;
    case 7:
        if (level.flags.hardfloor && ++trycnt < 4)
            goto try_again;
        if (is_floater(pm) || mtmp->isshk || mtmp->isgd || mtmp->ispriest || mtmp->issmith || mtmp->isnpc)
            return 0;
        else
            return WAN_DIGGING;
    }
    /*NOTREACHED*/
    return 0;
}

#define MUSE_WAN_DEATH 1
#define MUSE_WAN_SLEEP 2
#define MUSE_WAN_FIRE 3
#define MUSE_WAN_COLD 4
#define MUSE_WAN_LIGHTNING 5
#define MUSE_WAN_MAGIC_MISSILE 6
#define MUSE_WAN_STRIKING 7
#define MUSE_SCR_FIRE 8
#define MUSE_POT_PARALYSIS 9
#define MUSE_POT_BLINDNESS 10
#define MUSE_POT_CONFUSION 11
#define MUSE_FROST_HORN 12
#define MUSE_FIRE_HORN 13
#define MUSE_POT_ACID 14
/*#define MUSE_WAN_TELEPORTATION 15*/
#define MUSE_POT_SLEEPING 16
#define MUSE_SCR_EARTH 17
#define MUSE_WAN_DISINTEGRATION 18
#define MUSE_WAN_PETRIFICATION 19
#define MUSE_WAN_ORCUS 20
#define MUSE_RUBY_ROD 21

/* Select an offensive item/action for a monster.  Returns TRUE iff one is
 * found.
 */
boolean
find_offensive(mtmp)
struct monst *mtmp;
{
    register struct obj *obj;
    boolean extremely_high_level_skip = (mtmp->data->difficulty > 60);
    boolean very_high_level_skip = (mtmp->data->difficulty > 30 && mtmp->data->difficulty <= 60);
    boolean high_level_skip = (mtmp->data->difficulty > 20 && mtmp->data->difficulty <= 30);
    boolean medium_level_skip = (mtmp->data->difficulty > 15 && mtmp->data->difficulty <= 20);
    boolean low_level_skip = (mtmp->data->difficulty > 10 && mtmp->data->difficulty <= 15);
    boolean very_low_level_skip = (mtmp->data->difficulty > 5 && mtmp->data->difficulty <= 10);
    boolean reflection_skip = (Reflecting && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 15 || (mtmp->data->intl > 10 && rn2(4)) || rn2(2)));
    boolean antimagic_skip = (Antimagic && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 15 || (mtmp->data->intl > 10 && rn2(4)) || rn2(2)));
    boolean magic_missile_resistant_skip = (Magic_missile_immunity && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 15 || (mtmp->data->intl > 10 && rn2(4)) || rn2(2)));
    boolean sleep_resistant_skip = (Sleep_resistance && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));
    boolean death_resistant_skip = (Death_resistance && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));
    boolean disintegration_resistant_skip = (Disint_resistance && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));
    boolean petrification_resistant_skip = Stoned || (Stone_resistance && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));
    boolean fire_resistant_skip = (Fire_immunity && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));
    boolean cold_resistant_skip = (Cold_immunity && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));
    boolean shock_resistant_skip = (Shock_immunity && (is_lord(mtmp->data) || is_prince(mtmp->data) || mtmp->data->intl > 16 || (mtmp->data->intl > 12 && rn2(4)) || rn2(2)));

    boolean level_skip_weak_wand = (very_low_level_skip && rn2(2)) || (low_level_skip && rn2(4)) || (medium_level_skip && rn2(8)) || high_level_skip || very_high_level_skip || extremely_high_level_skip;
    boolean level_skip_normal_wand = (very_low_level_skip && !rn2(3)) || (low_level_skip && rn2(2)) || (medium_level_skip && rn2(4)) || (high_level_skip && rn2(8)) || very_high_level_skip || extremely_high_level_skip;
    boolean level_skip_powerful_wand = (very_low_level_skip && !rn2(9)) || (low_level_skip && !rn2(6)) || (medium_level_skip && !rn2(3)) || (high_level_skip && rn2(2)) || (very_high_level_skip && rn2(3)) || (extremely_high_level_skip && rn2(4));

    struct obj *helmet = which_armor(mtmp, W_ARMH);

    m.offensive = (struct obj *) 0;
    m.has_offense = 0;
    if (is_peaceful(mtmp) || is_animal(mtmp->data) || mindless(mtmp->data)
        || !can_operate_objects(mtmp->data))
        return FALSE;
    if (u.uswallow)
        return FALSE;
    if (in_your_sanctuary(mtmp, 0, 0))
        return FALSE;
    if (dmgtype(mtmp->data, AD_HEAL)
        && !uwep && !uarmu && !uarm && !uarmh && !uarmo && !uarmb
        && !uarms && !uarmg && !uarmc && !uarmf)
        return FALSE;
    /* all offensive items require orthogonal or diagonal targetting */
    if (!lined_up(mtmp, FALSE, 0, FALSE, M_LINEUP_RANGE))
        return FALSE;

#define nomore(x)       if (m.has_offense == x) continue;
    /* this picks the last viable item rather than prioritizing choices */
    for (obj = mtmp->minvent; obj; obj = obj->nobj) {
        if (!reflection_skip) 
        {
            nomore(MUSE_WAN_DEATH);
            if (obj->otyp == WAN_DEATH && obj->charges > 0 && !is_cancelled(mtmp) && !death_resistant_skip && !level_skip_powerful_wand
                && lined_up(mtmp, TRUE, AD_DRAY, TRUE, M_RAY_RANGE)) 
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_DEATH;
            }
            nomore(MUSE_WAN_ORCUS);
            if (obj->oartifact == ART_WAND_OF_ORCUS && obj->charges > 0 && obj->repowerleft == 0 && !death_resistant_skip && !level_skip_powerful_wand
                && lined_up(mtmp, TRUE, AD_DRAY, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_ORCUS;
            }
            nomore(MUSE_RUBY_ROD);
            if (obj->oartifact == ART_RUBY_ROD_OF_ASMODEUS && obj->repowerleft == 0)
            {
                if (!cold_resistant_skip
                    && lined_up(mtmp, TRUE, AD_COLD, TRUE, M_RAY_RANGE))
                {
                    obj->special_quality = 0;
                        m.offensive = obj;
                        m.has_offense = MUSE_RUBY_ROD;
                }
                else if (!shock_resistant_skip
                    && lined_up(mtmp, TRUE, AD_ELEC, TRUE, M_RAY_RANGE))
                {
                    obj->special_quality = 1;
                    m.offensive = obj;
                    m.has_offense = MUSE_RUBY_ROD;
                }
                else if (!fire_resistant_skip
                    && lined_up(mtmp, TRUE, AD_FIRE, TRUE, M_RAY_RANGE))
                {
                    obj->special_quality = 2;
                    m.offensive = obj;
                    m.has_offense = MUSE_RUBY_ROD;
                }
            }
            nomore(MUSE_WAN_DISINTEGRATION);
            if (obj->otyp == WAN_DISINTEGRATION && obj->charges > 0 && !is_cancelled(mtmp) && !disintegration_resistant_skip && ((!uarm && !uarms && !level_skip_powerful_wand) || ((uarm || uarms) && !level_skip_normal_wand))
                && lined_up(mtmp, TRUE, AD_DISN, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_DISINTEGRATION;
            }
            nomore(MUSE_WAN_PETRIFICATION);
            if (obj->otyp == WAN_PETRIFICATION && obj->charges > 0 && !is_cancelled(mtmp) && !petrification_resistant_skip && !level_skip_powerful_wand
                && lined_up(mtmp, TRUE, AD_STON, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_PETRIFICATION;
            }
            nomore(MUSE_WAN_SLEEP);
            if (obj->otyp == WAN_SLEEP && obj->charges > 0 && !is_cancelled(mtmp) && multi >= 0 && !Sleeping && !Paralyzed_or_immobile && !sleep_resistant_skip && !antimagic_skip && !level_skip_normal_wand &&
                lined_up(mtmp, TRUE, AD_SLEE, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_SLEEP;
            }
            nomore(MUSE_WAN_FIRE);
            if (obj->otyp == WAN_FIRE && obj->charges > 0 && !is_cancelled(mtmp) && !fire_resistant_skip && !level_skip_normal_wand && lined_up(mtmp, TRUE, AD_FIRE, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_FIRE;
            }
            nomore(MUSE_FIRE_HORN);
            if (obj->otyp == FIRE_HORN && obj->charges > 0 && !is_cancelled(mtmp) && can_blow(mtmp) && !fire_resistant_skip && !level_skip_normal_wand && lined_up(mtmp, TRUE, AD_FIRE, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_FIRE_HORN;
            }
            nomore(MUSE_WAN_COLD);
            if (obj->otyp == WAN_COLD && obj->charges > 0 && !is_cancelled(mtmp) && !cold_resistant_skip && !level_skip_normal_wand && lined_up(mtmp, TRUE, AD_COLD, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_COLD;
            }
            nomore(MUSE_FROST_HORN);
            if (obj->otyp == FROST_HORN && obj->charges > 0 && !is_cancelled(mtmp) && can_blow(mtmp) && !cold_resistant_skip && !level_skip_normal_wand && lined_up(mtmp, TRUE, AD_COLD, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_FROST_HORN;
            }
            nomore(MUSE_WAN_LIGHTNING);
            if (obj->otyp == WAN_LIGHTNING && obj->charges > 0 && !is_cancelled(mtmp) && !shock_resistant_skip && !level_skip_normal_wand && lined_up(mtmp, TRUE, AD_ELEC, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_LIGHTNING;
            }
            nomore(MUSE_WAN_MAGIC_MISSILE);
            if (obj->otyp == WAN_MAGIC_MISSILE && obj->charges > 0 && !is_cancelled(mtmp) && !magic_missile_resistant_skip && !antimagic_skip && !level_skip_weak_wand && lined_up(mtmp, TRUE, AD_MAGM, TRUE, M_RAY_RANGE))
            {
                m.offensive = obj;
                m.has_offense = MUSE_WAN_MAGIC_MISSILE;
            }
        }
        nomore(MUSE_WAN_STRIKING);
        if (obj->otyp == WAN_STRIKING && obj->charges > 0 && !is_cancelled(mtmp) && !magic_missile_resistant_skip && !level_skip_weak_wand && lined_up(mtmp, TRUE, AD_MAGM, FALSE, M_RAY_RANGE))
        {
            m.offensive = obj;
            m.has_offense = MUSE_WAN_STRIKING;
        }
#if 0   /* use_offensive() has had some code to support wand of teleportation
         * for a long time, but find_offensive() never selected one;
         * so for the time being, this is still disabled */
        nomore(MUSE_WAN_TELEPORTATION);
        if (obj->otyp == WAN_TELEPORTATION && obj->charges > 0
            /* don't give controlled hero a free teleport */
            && !Teleport_control
            /* do try to move hero to a more vulnerable spot */
            && (onscary(u.ux, u.uy, mtmp)
                || (u.ux == xupstair && u.uy == yupstair)
                || (u.ux == xdnstair && u.uy == ydnstair)
                || (u.ux == sstairs.sx && u.uy == sstairs.sy)
                || (u.ux == xupladder && u.uy == yupladder)
                || (u.ux == xdnladder && u.uy == ydnladder))) {
            m.offensive = obj;
            m.has_offense = MUSE_WAN_TELEPORTATION;
        }
#endif
        if (!level_skip_weak_wand && lined_up(mtmp, TRUE, AD_PHYS, FALSE, M_GENERIC_RANGED_ATTACK_RANGE))
        {
            nomore(MUSE_POT_PARALYSIS);
            if (obj->otyp == POT_PARALYSIS && multi >= 0 && !Sleeping && !Paralyzed_or_immobile) {
                m.offensive = obj;
                m.has_offense = MUSE_POT_PARALYSIS;
            }
            nomore(MUSE_POT_BLINDNESS);
            if (obj->otyp == POT_BLINDNESS && !attacktype(mtmp->data, AT_GAZE)) {
                m.offensive = obj;
                m.has_offense = MUSE_POT_BLINDNESS;
            }
            nomore(MUSE_POT_CONFUSION);
            if (obj->otyp == POT_CONFUSION) {
                m.offensive = obj;
                m.has_offense = MUSE_POT_CONFUSION;
            }
            nomore(MUSE_POT_SLEEPING);
            if (obj->otyp == POT_SLEEPING) {
                m.offensive = obj;
                m.has_offense = MUSE_POT_SLEEPING;
            }
            nomore(MUSE_POT_ACID);
            if (obj->otyp == POT_ACID) {
                m.offensive = obj;
                m.has_offense = MUSE_POT_ACID;
            }
            /* we can safely put this scroll here since the locations that
             * are in a 1 square radius are a subset of the locations that
             * are in wand or throwing range (in other words, always lined_up())
             */
            nomore(MUSE_SCR_EARTH);
            if (obj->otyp == SCR_EARTH
                && ((helmet && is_metallic(helmet)) || is_confused(mtmp)
                    || amorphous(mtmp->data) || passes_walls(mtmp->data)
                    || is_incorporeal(mtmp->data) || unsolid(mtmp->data)
                    || !rn2(10))
                && dist2(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy) <= 2
                && !is_blinded(mtmp) && haseyes(mtmp->data)
                && !Is_really_rogue_level(&u.uz)
                && (!In_endgame(&u.uz) || Is_earthlevel(&u.uz))) {
                m.offensive = obj;
                m.has_offense = MUSE_SCR_EARTH;
            }
        }
#if 0
        nomore(MUSE_SCR_FIRE);
        if (obj->otyp == SCR_FIRE && is_mon_immune_to_fire(mtmp)
            && dist2(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy) <= 2
            && !is_blinded(mtmp) && haseyes(mtmp->data)) {
            m.offensive = obj;
            m.has_offense = MUSE_SCR_FIRE;
        }
#endif /* 0 */
    }
    return (boolean) !!m.has_offense;
#undef nomore
}

STATIC_PTR
int
mbhitm(mtmp, otmp, origmonst)
register struct monst *mtmp;
register struct obj *otmp;
register struct monst* origmonst;
{
    int tmp;
    boolean reveal_invis = FALSE;

    if (origmonst)
    {
        /* Do nothing */
    }

    if (mtmp != &youmonst)
    {
        if (mtmp->msleeping)
        {
            mtmp->msleeping = 0;
            refresh_m_tile_gui_info(mtmp, TRUE);
        }
        if (mtmp->m_ap_type)
            seemimic(mtmp);
    }

    int duration = get_obj_spell_duration(otmp);
    switch (otmp->otyp) 
    {
    case WAN_STRIKING:
        reveal_invis = TRUE;
        if (mtmp == &youmonst)
        {
            if (zap_oseen)
                makeknown(WAN_STRIKING);
            if (Magic_missile_immunity || Invulnerable) 
            {
                u_shieldeff();
                pline("Boing!");
            } 
            else if (rnd(20) < 10 + u.uac)
            {
                double damage = adjust_damage(d(2, 12), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_SPELL_DAMAGE);
                int damagedealt = calculate_damage_dealt_to_player(damage);
                if (damagedealt > 0)
                    pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_red1, "The wand hits you for %d damage!", damagedealt);
                else
                    pline_The("The wand hits you!");

                char kbuf[BUFSZ * 2], knam[BUFSZ] = "";
                int kprefix = KILLED_BY;
                if (origmonst)
                {
                    int mkprefix = KILLED_BY_AN;
                    get_killer_name_and_format(origmonst, knam, &mkprefix);
                    Sprintf(kbuf, "%s zapped by %s", killer_xname(otmp), mkprefix == KILLED_BY_AN ? an(knam) : knam);
                }
                else
                {
                    Sprintf(kbuf, "%s", killer_xname(otmp));
                }
                losehp(damage, kbuf, kprefix);
            }
            else
                pline_The("wand misses you.");

            stop_occupation();
            nomul(0);
        } 
        else if (is_mon_immune_to_magic_missile(mtmp) || has_invulnerable(mtmp))
        {
            m_shieldeff(mtmp);
            pline("Boing!");
        } 
        else if (rnd(20) < 10 + find_mac(mtmp)) 
        {
            tmp = d(2, 12);
            hit("wand", mtmp, exclam(tmp), 0, ""); //Do not tell the damage here, since resist will
            (void) inflict_spell_damage(mtmp, otmp, origmonst, tmp, AD_MAGM, TELL);
            if (cansee(mtmp->mx, mtmp->my) && zap_oseen)
                makeknown(WAN_STRIKING);
        } 
        else 
        {
            miss("wand", mtmp);
            if (cansee(mtmp->mx, mtmp->my) && zap_oseen)
                makeknown(WAN_STRIKING);
        }
        break;
#if 0   /* disabled because find_offensive() never picks WAN_TELEPORTATION */
    case WAN_TELEPORTATION:
        if (mtmp == &youmonst) {
            if (zap_oseen)
                makeknown(WAN_TELEPORTATION);
            tele();
        } else {
            /* for consistency with zap.c, don't identify */
            if (mtmp->ispriest && *in_rooms(mtmp->mx, mtmp->my, TEMPLE)) {
                play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
                if (cansee(mtmp->mx, mtmp->my))
                    pline("%s resists the magic!", Monnam(mtmp));
            } else if (!tele_restrict(mtmp))
                (void)rloc2(mtmp, TRUE, TRUE);
        }
        break;
#endif
    case WAN_CANCELLATION:
    case SPE_CANCELLATION:
    case WAN_DISJUNCTION:
    case SPE_DISJUNCTION:
        (void) cancel_monst(mtmp, otmp, FALSE, TRUE, FALSE, duration);
        break;
    case SPE_LOWER_MAGIC_RESISTANCE:
    case SPE_DIMINISH_MAGIC_RESISTANCE:
    case SPE_ABOLISH_MAGIC_RESISTANCE:
    case SPE_NEGATE_MAGIC_RESISTANCE:
    case SPE_FORBID_SUMMONING:
        (void)add_temporary_property(mtmp, otmp, FALSE, TRUE, FALSE, duration);
        break;
    }
    if (reveal_invis) {
        if (!DEADMONSTER(mtmp) && cansee(bhitpos.x, bhitpos.y)
            && !canspotmon(mtmp))
            map_invisible(bhitpos.x, bhitpos.y);
    }
    return 0;
}

/* A modified bhit() for monsters.  Based on bhit() in zap.c.  Unlike
 * buzz(), bhit() doesn't take into account the possibility of a monster
 * zapping you, so we need a special function for it.  (Unless someone wants
 * to merge the two functions...)
 */
STATIC_OVL void
mbhit(mon, range, fhitm, fhito, obj)
struct monst *mon;  /* monster shooting the wand */
register int range; /* direction and range */
int FDECL((*fhitm), (MONST_P, OBJ_P, MONST_P));
int FDECL((*fhito), (OBJ_P, OBJ_P, MONST_P)); /* fns called when mon/obj hit */
struct obj *obj;                     /* 2nd arg to fhitm/fhito */
{
    register struct monst *mtmp;
    register struct obj *otmp;
    register uchar typ;
    int ddx, ddy;

    bhitpos.x = mon->mx;
    bhitpos.y = mon->my;
    ddx = sgn(mon->mux - mon->mx);
    ddy = sgn(mon->muy - mon->my);

    while (range-- > 0) {
        int x, y;

        bhitpos.x += ddx;
        bhitpos.y += ddy;
        x = bhitpos.x;
        y = bhitpos.y;

        if (!isok(x, y)) {
            bhitpos.x -= ddx;
            bhitpos.y -= ddy;
            break;
        }
        if (find_drawbridge(&x, &y))
            switch (obj->otyp) {
            case WAN_STRIKING:
                destroy_drawbridge(x, y, FALSE);
            }
        if (bhitpos.x == u.ux && bhitpos.y == u.uy) {
            (*fhitm)(&youmonst, obj, mon);
            range -= 3;
        } else if ((mtmp = m_at(bhitpos.x, bhitpos.y)) != 0) {
            if (cansee(bhitpos.x, bhitpos.y) && !canspotmon(mtmp))
                map_invisible(bhitpos.x, bhitpos.y);
            (*fhitm)(mtmp, obj, mon);
            range -= 3;
        }
        /* modified by GAN to hit all objects */
        if (fhito) {
            int hitanything = 0;
            register struct obj *next_obj;

            for (otmp = level.objects[bhitpos.x][bhitpos.y]; otmp;
                 otmp = next_obj) {
                /* Fix for polymorph bug, Tim Wright */
                next_obj = otmp->nexthere;
                hitanything += (*fhito)(otmp, obj, mon);
            }
            if (hitanything)
                range--;
        }
        typ = levl[bhitpos.x][bhitpos.y].typ;
        if (IS_DOOR(typ) || typ == SDOOR) {
            switch (obj->otyp) {
            /* note: monsters don't use opening or locking magic
               at present, but keep these as placeholders */
            case WAN_OPENING:
            case WAN_LOCKING:
            case WAN_STRIKING:
                if (doorlock(obj, bhitpos.x, bhitpos.y)) {
                    if (zap_oseen)
                        makeknown(obj->otyp);
                    /* if a shop door gets broken, add it to
                       the shk's fix list (no cost to player) */
                    if ((levl[bhitpos.x][bhitpos.y].doormask & D_MASK) == D_BROKEN
                        && *in_rooms(bhitpos.x, bhitpos.y, SHOPBASE))
                        add_damage(bhitpos.x, bhitpos.y, 0L);
                }
                break;
            }
        }
        if (!ZAP_POS(typ)
            || (IS_DOOR(typ) && (levl[bhitpos.x][bhitpos.y].doormask
                                 & (D_LOCKED | D_CLOSED)))) {
            bhitpos.x -= ddx;
            bhitpos.y -= ddy;
            break;
        }
    }
}

/* Perform an offensive action for a monster.  Must be called immediately
 * after find_offensive().  Return values are same as use_defensive().
 */
int
use_offensive(mtmp)
struct monst *mtmp;
{
    if (!mtmp)
        return 0;

    int i;
    struct obj *otmp = m.offensive;
    boolean oseen;
    int raytype = 0;

    /* offensive potions are not drunk, they're thrown */
    if (otmp->oclass != POTION_CLASS && (i = precheck(mtmp, otmp)) != 0)
        return i;
    oseen = otmp && canseemon(mtmp);

    switch (m.has_offense) {
    case MUSE_WAN_DEATH:
    case MUSE_WAN_DISINTEGRATION:
    case MUSE_WAN_PETRIFICATION:
    case MUSE_WAN_SLEEP:
    case MUSE_WAN_FIRE:
    case MUSE_WAN_COLD:
    case MUSE_WAN_LIGHTNING:
    case MUSE_WAN_MAGIC_MISSILE:
        if (!otmp)
            return 2;

        mzapmsg(mtmp, otmp, FALSE);
        otmp->charges--;
        if (oseen)
            makeknown(otmp->otyp);
        m_using = TRUE;

        raytype = -40 - objects[otmp->otyp].oc_dir_subtype; //-40...-48;

        buzz(raytype, otmp, mtmp, 0, 0, 0, mtmp->mx, mtmp->my,
             sgn(mtmp->mux - mtmp->mx), sgn(mtmp->muy - mtmp->my));
        m_using = FALSE;
        return (DEADMONSTER(mtmp)) ? 1 : 2;
    case MUSE_WAN_ORCUS:
    case MUSE_RUBY_ROD:
        if (!otmp)
            return 2;

        minvokemsg(mtmp, otmp, FALSE);
        if (otmp->oartifact > 0 && artilist[otmp->oartifact].repower_time > 0) /* Override below if effect failed */
            otmp->repowerleft = artilist[otmp->oartifact].repower_time;

        if (artilist[otmp->oartifact].aflags & AF_INVOKE_EXPENDS_CHARGE)
            otmp->charges--;

        if (oseen && otmp->oartifact)
        {
            makeknown(otmp->otyp);
            otmp->aknown = otmp->nknown = 1;
        }
        m_using = TRUE;

        struct obj pseudo = { 0 };
        pseudo.otyp = m.has_offense == MUSE_WAN_ORCUS ? WAN_DEATH : otmp->special_quality == 0 ? WAN_COLD : otmp->special_quality == 1 ? WAN_LIGHTNING : WAN_FIRE;
        pseudo.quan = 1L;
        raytype = -40 - objects[pseudo.otyp].oc_dir_subtype; //-40...-48;
        buzz(raytype, &pseudo, mtmp, 0, 0, 0, mtmp->mx, mtmp->my,
            sgn(mtmp->mux - mtmp->mx), sgn(mtmp->muy - mtmp->my));
        m_using = FALSE;
        return (DEADMONSTER(mtmp)) ? 1 : 2;
    case MUSE_FIRE_HORN:
    case MUSE_FROST_HORN:
        if (!otmp)
            return 2;

        if (oseen) {
            makeknown(otmp->otyp);
            pline("%s plays a %s!", Monnam(mtmp), xname(otmp));
        } else
            You_hear("a horn being played.");
        otmp->charges--;
        m_using = TRUE;
        buzz(-40 - objects[otmp->otyp].oc_dir_subtype, otmp, mtmp, 0, 0, 0,
             mtmp->mx, mtmp->my, sgn(mtmp->mux - mtmp->mx),
             sgn(mtmp->muy - mtmp->my));
        m_using = FALSE;
        return (DEADMONSTER(mtmp)) ? 1 : 2;
    case MUSE_WAN_TELEPORTATION:
    case MUSE_WAN_STRIKING:
        zap_oseen = oseen;
        if (!otmp)
            return 2;
        mzapmsg(mtmp, otmp, FALSE);
        otmp->charges--;
        m_using = TRUE;
        mbhit(mtmp, rn1(8, 6), mbhitm, bhito, otmp);
        m_using = FALSE;
        return 2;
    case MUSE_SCR_EARTH: {
        if (!otmp)
            return 2;
        /* TODO: handle steeds */
        register int x, y;
        /* don't use monster fields after killing it */
        boolean confused = (is_confused(mtmp) ? TRUE : FALSE);
        int mmx = mtmp->mx, mmy = mtmp->my;
        boolean is_cursed = otmp->cursed;

        mreadmsg(mtmp, otmp);
        /* Identify the scroll */
        if (canspotmon(mtmp)) {
            pline_The("%s rumbles %s %s!", ceiling(mtmp->mx, mtmp->my),
                      otmp->blessed ? "around" : "above", mon_nam(mtmp));
            if (oseen)
                makeknown(otmp->otyp);
        } else if (cansee(mtmp->mx, mtmp->my)) {
            pline_The("%s rumbles in the middle of nowhere!",
                      ceiling(mtmp->mx, mtmp->my));
            if (is_invisible(mtmp))
                map_invisible(mtmp->mx, mtmp->my);
            if (oseen)
                makeknown(otmp->otyp);
        }

        /* Loop through the surrounding squares */
        for (x = mmx - 1; x <= mmx + 1; x++) {
            for (y = mmy - 1; y <= mmy + 1; y++) {
                /* Is this a suitable spot? */
                if (isok(x, y) && !closed_door(x, y)
                    && !IS_ROCK(levl[x][y].typ) && !IS_AIR(levl[x][y].typ)
                    && (((x == mmx) && (y == mmy)) ? !otmp->blessed
                                                   : !otmp->cursed)
                    && (x != u.ux || y != u.uy)) {
                    (void) drop_boulder_on_monster(x, y, confused, FALSE);
                }
            }
        }
        m_useup(mtmp, otmp);
        /* Attack the player */
        if (distmin(mmx, mmy, u.ux, u.uy) == 1 && !is_cursed) {
            drop_boulder_on_player(confused, !is_cursed, FALSE, TRUE);
        }

        return (DEADMONSTER(mtmp)) ? 1 : 2;
    }
#if 0
    case MUSE_SCR_FIRE: {
        boolean vis = cansee(mtmp->mx, mtmp->my);

        mreadmsg(mtmp, otmp);
        if (is_confused(mtmp)) {
            if (vis)
                pline("Oh, what a pretty fire!");
        } else {
            struct monst *mtmp2;
            int num;

            if (vis)
                pline_The("scroll erupts in a tower of flame!");
            m_shieldeff(mtmp);
            pline("%s is uninjured.", Monnam(mtmp));
            (void) destroy_mitem(mtmp, SCROLL_CLASS, AD_FIRE);
            (void) destroy_mitem(mtmp, SPBOOK_CLASS, AD_FIRE);
            (void) destroy_mitem(mtmp, POTION_CLASS, AD_FIRE);
            num = (2 * (rn1(3, 3) + 2 * bcsign(otmp)) + 1) / 3;
            if (Fire_immunity)
                You("are not harmed.");
            burn_away_slime();
            if (Half_spell_damage)
                num = (num + 1) / 2;
            else
                losehp(num, "scroll of fire", KILLED_BY_AN);
            for (mtmp2 = fmon; mtmp2; mtmp2 = mtmp2->nmon) {
                if (DEADMONSTER(mtmp2))
                    continue;
                if (mtmp == mtmp2)
                    continue;
                if (dist2(mtmp2->mx, mtmp2->my, mtmp->mx, mtmp->my) < 3) {
                    if (is_mon_immune_to_fire(mtmp2))
                        continue;
                    mtmp2->mhp -= num;
                    if (is_mon_immune_to_cold(mtmp2))
                        mtmp2->mhp -= 3 * num;
                    if (DEADMONSTER(mtmp2)) {
                        mondied(mtmp2);
                        break;
                    }
                }
            }
        }
        return 2;
    }
#endif /* 0 */
    case MUSE_POT_PARALYSIS:
    case MUSE_POT_BLINDNESS:
    case MUSE_POT_CONFUSION:
    case MUSE_POT_SLEEPING:
    case MUSE_POT_ACID:
        if (!otmp)
            return 2;
        /* Note: this setting of dknown doesn't suffice.  A monster
         * which is out of sight might throw and it hits something _in_
         * sight, a problem not existing with wands because wand rays
         * are not objects.  Also set dknown in mthrowu.c.
         */
        if (cansee(mtmp->mx, mtmp->my)) 
        {
            otmp->dknown = 1;
            int multicolors[2] = { NO_COLOR, CLR_MSG_HINT };
            pline_multi_ex(ATR_NONE, CLR_MSG_WARNING, no_multiattrs, multicolors, "%s hurls %s!", Monnam(mtmp), singular(otmp, doname));
        }
        m_throw(mtmp, mtmp->mx, mtmp->my, sgn(mtmp->mux - mtmp->mx),
                sgn(mtmp->muy - mtmp->my),
                distmin(mtmp->mx, mtmp->my, mtmp->mux, mtmp->muy), otmp);
        return 2;
    case 0:
        return 0; /* i.e. an exploded wand */
    default:
        impossible("%s wanted to perform action %d?", Monnam(mtmp),
                   m.has_offense);
        break;
    }
    return 0;
}

int
rnd_offensive_item(mtmp)
struct monst *mtmp;
{
    struct permonst *pm = mtmp->data;
    int difficulty = mons[mtmp->mnum].difficulty;

    if (is_animal(pm) || attacktype(pm, AT_EXPL) || mindless(mtmp->data) || is_modron(mtmp->data)
        || pm->mlet == S_GHOST || pm->mlet == S_KOP)
        return 0;

#if 0
    if (difficulty > 7 && !rn2(35))
        return WAN_DEATH;
#endif

    switch (rn2(9 - (difficulty < 4) + 4 * (difficulty > 6))) 
    {
    case 0: 
    {
        struct obj *helmet = which_armor(mtmp, W_ARMH);

        if ((helmet && is_metallic(helmet)) || amorphous(pm)
            || passes_walls(pm) || is_incorporeal(pm) || unsolid(pm))
            return SCR_EARTH;
    } /* fall through */
    case 1:
        return WAN_STRIKING;
    case 2:
        return POT_ACID;
    case 3:
        return POT_CONFUSION;
    case 4:
        return POT_BLINDNESS;
    case 5:
        return POT_SLEEPING;
    case 6:
        return POT_PARALYSIS;
    case 7:
    case 8:
        return WAN_MAGIC_MISSILE;
    case 9:
        return WAN_SLEEP;
    case 10:
        return WAN_FIRE;
    case 11:
        return WAN_COLD;
    case 12:
        return WAN_LIGHTNING;
    }
    /*NOTREACHED*/
    return 0;
}

#define MUSE_POT_GAIN_LEVEL 1
#define MUSE_WAN_MAKE_INVISIBLE 2
#define MUSE_POT_INVISIBILITY 3
#define MUSE_POLY_TRAP 4
#define MUSE_WAN_POLYMORPH 5
#define MUSE_POT_SPEED 6
#define MUSE_POT_GREATER_SPEED 7
#define MUSE_POT_LIGHTNING_SPEED 8
#define MUSE_WAN_SPEED_MONSTER 9
#define MUSE_BULLWHIP 10
#define MUSE_POT_POLYMORPH 11
#define MUSE_POT_GAIN_ENERGY 12
#define MUSE_POT_EXTRA_ENERGY 13
#define MUSE_POT_GREATER_ENERGY 14
#define MUSE_POT_FULL_ENERGY 15
#define MUSE_POT_HEROISM 16
#define MUSE_POT_SUPER_HEROISM 17
#define MUSE_POT_TITAN_STRENGTH 18
#define MUSE_POT_LESSER_REJUVENATION 19
#define MUSE_POT_REJUVENATION 20
#define MUSE_POT_GREATER_REJUVENATION 21
#define MUSE_POT_LESSER_REGENERATION 22
#define MUSE_POT_REGENERATION 23
#define MUSE_POT_GREATER_REGENERATION 24
#define MUSE_POT_WATER 25
#define MUSE_POT_SEE_INVISIBLE 26
#define MUSE_POT_FRUIT_JUICE 27
#define MUSE_POT_LEVITATION 28
#define MUSE_POT_FIRE_IMMUNITY 29
#define MUSE_POT_COLD_IMMUNITY 30
#define MUSE_POT_SHOCK_IMMUNITY 31
#define MUSE_POT_MAGIC_RESISTANCE 32
#define MUSE_WAN_WISHING 33

boolean
set_misc_potion(mtmp, obj)
struct monst* mtmp;
struct obj* obj;
{
    m.misc = (struct obj*)0;
    m.has_misc = 0;
    if (!mtmp || !obj)
        return FALSE;

    int otyp = obj->otyp;
    switch (otyp)
    {
    case POT_GAIN_LEVEL:
        m.has_misc = MUSE_POT_GAIN_LEVEL;
        break;
    case WAN_MAKE_INVISIBLE:
        m.has_misc = MUSE_WAN_MAKE_INVISIBLE;
        break;
    case POT_INVISIBILITY:
        m.has_misc = MUSE_POT_INVISIBILITY;
        break;
    case WAN_POLYMORPH:
        m.has_misc = MUSE_WAN_POLYMORPH;
        break;
    case POT_SPEED:
        m.has_misc = MUSE_POT_SPEED;
        break;
    case POT_GREATER_SPEED:
        m.has_misc = MUSE_POT_GREATER_SPEED;
        break;
    case POT_LIGHTNING_SPEED:
        m.has_misc = MUSE_POT_LIGHTNING_SPEED;
        break;
    case WAN_SPEED_MONSTER:
        m.has_misc = MUSE_WAN_SPEED_MONSTER;
        break;
    case POT_POLYMORPH:
        m.has_misc = MUSE_POT_POLYMORPH;
        break;
    case POT_GAIN_ENERGY:
        m.has_misc = MUSE_POT_GAIN_ENERGY;
        break;
    case POT_EXTRA_ENERGY:
        m.has_misc = MUSE_POT_EXTRA_ENERGY;
        break;
    case POT_GREATER_ENERGY:
        m.has_misc = MUSE_POT_GREATER_ENERGY;
        break;
    case POT_FULL_ENERGY:
        m.has_misc = MUSE_POT_FULL_ENERGY;
        break;
    case POT_HEROISM:
        m.has_misc = MUSE_POT_HEROISM;
        break;
    case POT_SUPER_HEROISM:
        m.has_misc = MUSE_POT_SUPER_HEROISM;
        break;
    case POT_TITAN_STRENGTH:
        m.has_misc = MUSE_POT_TITAN_STRENGTH;
        break;
    case POT_LESSER_REJUVENATION:
        m.has_misc = MUSE_POT_LESSER_REJUVENATION;
        break;
    case POT_REJUVENATION:
        m.has_misc = MUSE_POT_REJUVENATION;
        break;
    case POT_GREATER_REJUVENATION:
        m.has_misc = MUSE_POT_GREATER_REJUVENATION;
        break;
    case POT_LESSER_REGENERATION:
        m.has_misc = MUSE_POT_LESSER_REGENERATION;
        break;
    case POT_REGENERATION:
        m.has_misc = MUSE_POT_REGENERATION;
        break;
    case POT_GREATER_REGENERATION:
        m.has_misc = MUSE_POT_GREATER_REGENERATION;
        break;
    case POT_WATER:
        m.has_misc = MUSE_POT_WATER;
        break;
    case POT_SEE_INVISIBLE:
        m.has_misc = MUSE_POT_SEE_INVISIBLE;
        break;
    case POT_FRUIT_JUICE:
        m.has_misc = MUSE_POT_FRUIT_JUICE;
        break;
    case POT_LEVITATION:
        m.has_misc = MUSE_POT_LEVITATION;
        break;
    case POT_FIRE_IMMUNITY:
        m.has_misc = MUSE_POT_FIRE_IMMUNITY;
        break;
    case POT_SHOCK_IMMUNITY:
        m.has_misc = MUSE_POT_SHOCK_IMMUNITY;
        break;
    case POT_COLD_IMMUNITY:
        m.has_misc = MUSE_POT_COLD_IMMUNITY;
        break;
    case POT_MAGIC_RESISTANCE:
        m.has_misc = MUSE_POT_MAGIC_RESISTANCE;
        break;
    default:
        break;
    }
    if (m.has_misc != 0)
        m.misc = obj;
    return (m.has_misc != 0);
}

boolean
find_misc(mtmp)
struct monst *mtmp;
{
    register struct obj *obj;
    struct permonst *mdat = mtmp->data;
    int x = mtmp->mx, y = mtmp->my;
    struct trap *t;
    int xx, yy, pmidx = NON_PM;
    boolean immobile = (mdat->mmove == 0);
    boolean stuck = (mtmp == u.ustuck);

    m.misc = (struct obj *) 0;
    m.has_misc = 0;
    if (is_animal(mdat) || mindless(mdat))
        return 0;
    if (u.uswallow && stuck)
        return FALSE;

    /* We arbitrarily limit to times when a player is nearby for the
     * same reason as Junior Pac-Man doesn't have energizers eaten until
     * you can see them...
     */
    if (dist2(x, y, mtmp->mux, mtmp->muy) > 36)
        return FALSE;

    if (!stuck && !immobile && (mtmp->cham == NON_PM)
        && mons[(pmidx = monsndx(mdat))].difficulty < 6) {
        boolean ignore_boulders = (verysmall(mdat) || throws_rocks(mdat)
                                   || passes_walls(mdat)),
            diag_ok = !NODIAG(pmidx);

        for (xx = x - 1; xx <= x + 1; xx++)
            for (yy = y - 1; yy <= y + 1; yy++)
                if (isok(xx, yy) && (xx != u.ux || yy != u.uy)
                    && (diag_ok || xx == x || yy == y)
                    && ((xx == x && yy == y) || !level.monsters[xx][yy]))
                    if ((t = t_at(xx, yy)) != 0
                        && (ignore_boulders || !sobj_at(BOULDER, xx, yy))
                        && !onscary(xx, yy, mtmp)) {
                        /* use trap if it's the correct type */
                        if (t->ttyp == POLY_TRAP) {
                            trapx = xx;
                            trapy = yy;
                            m.has_misc = MUSE_POLY_TRAP;
                            return TRUE;
                        }
                    }
    }
    if (!can_operate_objects(mdat))
        return 0;

#define nomore(x)       if (m.has_misc == x) continue
    /*
     * [bug?]  Choice of item is not prioritized; the last viable one
     * in the monster's inventory will be chosen.
     * 'nomore()' is nearly worthless because it only screens checking
     * of duplicates when there is no alternate type in between them.
     */
    for (obj = mtmp->minvent; obj; obj = obj->nobj) {
        /* Monsters shouldn't recognize cursed items; this kludge is
           necessary to prevent serious problems though... */
        if (obj->otyp == POT_GAIN_LEVEL
            && (!obj->cursed
                || (!mtmp->isgd && !mtmp->isshk && !mtmp->ispriest && !mtmp->issmith && !mtmp->isnpc))) {
            m.misc = obj;
            m.has_misc = MUSE_POT_GAIN_LEVEL;
        }
        nomore(MUSE_WAN_WISHING);
        if (obj->otyp == WAN_WISHING && obj->charges > 0) {
            m.misc = obj;
            m.has_misc = MUSE_WAN_WISHING;
        }
        nomore(MUSE_BULLWHIP);
        if (obj->otyp == BULLWHIP && !is_peaceful(mtmp)
            /* the random test prevents whip-wielding
               monster from attempting disarm every turn */
            && uwep && !rn2(5) && obj == MON_WEP(mtmp)
            /* hero's location must be known and adjacent */
            && mtmp->mux == u.ux && mtmp->muy == u.uy
            && distu(mtmp->mx, mtmp->my) <= 2
            /* don't bother if it can't work (this doesn't
               prevent cursed weapons from being targetted) */
            && (canletgo(uwep, "")
                || (u.twoweap && uarms && canletgo(uarms, "")))) {
            m.misc = obj;
            m.has_misc = MUSE_BULLWHIP;
        }
        /* Note: peaceful/tame monsters won't make themselves
         * invisible unless you can see them.  Not really right, but...
         */
        nomore(MUSE_WAN_MAKE_INVISIBLE);
        if (obj->otyp == WAN_MAKE_INVISIBLE && obj->charges > 0 && !is_cancelled(mtmp) && !has_invisibility(mtmp)
            && !has_blocks_invisibility(mtmp) && (!is_peaceful(mtmp) || See_invisible)
            && (!attacktype(mtmp->data, AT_GAZE) || is_cancelled(mtmp))) {
            m.misc = obj;
            m.has_misc = MUSE_WAN_MAKE_INVISIBLE;
        }
        nomore(MUSE_POT_INVISIBILITY);
        if (obj->otyp == POT_INVISIBILITY && !has_invisibility(mtmp)
            && !has_blocks_invisibility(mtmp) && (!is_peaceful(mtmp) || See_invisible)
            && (!attacktype(mtmp->data, AT_GAZE) || is_cancelled(mtmp))) {
            m.misc = obj;
            m.has_misc = MUSE_POT_INVISIBILITY;
        }
        nomore(MUSE_WAN_SPEED_MONSTER);
        if (obj->otyp == WAN_SPEED_MONSTER && obj->charges > 0 && !is_cancelled(mtmp)
            && !has_very_fast(mtmp) && !has_ultra_fast(mtmp) && !has_super_fast(mtmp) && !has_lightning_fast(mtmp) && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_WAN_SPEED_MONSTER;
        }
        nomore(MUSE_POT_SPEED);
        if (obj->otyp == POT_SPEED && !has_ultra_fast(mtmp) && !has_super_fast(mtmp) && !has_lightning_fast(mtmp) && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_POT_SPEED;
        }
        nomore(MUSE_POT_GREATER_SPEED);
        if (obj->otyp == POT_GREATER_SPEED && !has_super_fast(mtmp) && !has_lightning_fast(mtmp) && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_POT_GREATER_SPEED;
        }
        nomore(MUSE_POT_LIGHTNING_SPEED);
        if (obj->otyp == POT_LIGHTNING_SPEED && !has_lightning_fast(mtmp) && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_POT_LIGHTNING_SPEED;
        }
        nomore(MUSE_POT_GAIN_ENERGY);
        if (obj->otyp == POT_GAIN_ENERGY
            && !has_property(mtmp, RAPIDEST_ENERGY_REGENERATION)
            && !has_property(mtmp, RAPIDER_ENERGY_REGENERATION)
            && !has_property(mtmp, RAPID_ENERGY_REGENERATION)
            && (mtmp->mspec_used > 0 || mtmp->mmagespell_used || mtmp->mmageintermediate_used || mtmp->mmageultimate_used
                || mtmp->mclericspell_used || mtmp->mclericintermediate_used || mtmp->mclericultimate_used))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_GAIN_ENERGY;
        }
        nomore(MUSE_POT_EXTRA_ENERGY);
        if (obj->otyp == POT_EXTRA_ENERGY
            && !has_property(mtmp, RAPIDEST_ENERGY_REGENERATION)
            && !has_property(mtmp, RAPIDER_ENERGY_REGENERATION)
            && !has_property(mtmp, RAPID_ENERGY_REGENERATION)
            && (mtmp->mspec_used > 0 || ((mtmp->mmagespell_used || mtmp->mclericspell_used) && !rn2(2))
                || ((mtmp->mmageintermediate_used || mtmp->mclericintermediate_used))
                || mtmp->mmageultimate_used || mtmp->mclericultimate_used))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_EXTRA_ENERGY;
        }
        nomore(MUSE_POT_GREATER_ENERGY);
        if (obj->otyp == POT_GREATER_ENERGY
            && !has_property(mtmp, RAPIDEST_ENERGY_REGENERATION)
            && !has_property(mtmp, RAPIDER_ENERGY_REGENERATION)
            && (mtmp->mspec_used > 0 || ((mtmp->mmagespell_used || mtmp->mclericspell_used) && !rn2(4))
                || ((mtmp->mmageintermediate_used || mtmp->mclericintermediate_used) && !rn2(2))
                || mtmp->mmageultimate_used || mtmp->mclericultimate_used))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_GREATER_ENERGY;
        }
        nomore(MUSE_POT_FULL_ENERGY);
        if (obj->otyp == POT_FULL_ENERGY
            && !has_property(mtmp, RAPIDEST_ENERGY_REGENERATION)
            && (mtmp->mspec_used > 0 || ((mtmp->mmagespell_used || mtmp->mclericspell_used) && !rn2(6)) 
                || ((mtmp->mmageintermediate_used || mtmp->mclericintermediate_used) && !rn2(3)) 
                || mtmp->mmageultimate_used || mtmp->mclericultimate_used))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_FULL_ENERGY;
        }
        nomore(MUSE_POT_GREATER_REJUVENATION);
        if (obj->otyp == POT_GREATER_REJUVENATION 
            && (mtmp->mspec_used > 0 || ((mtmp->mmagespell_used || mtmp->mclericspell_used) && !rn2(2))
                || ((mtmp->mmageintermediate_used || mtmp->mclericintermediate_used))
                || mtmp->mmageultimate_used || mtmp->mclericultimate_used
                || (mtmp->mhp < (3 * mtmp->mhpmax) / 4 && !has_property(mtmp, DIVINE_REGENERATION) && !has_property(mtmp, RAPIDEST_REGENERATION) && (!has_property(mtmp, RAPIDER_REGENERATION) || !rn2(2)))
                ))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_GREATER_REJUVENATION;
        }
        nomore(MUSE_POT_REJUVENATION);
        if (obj->otyp == POT_REJUVENATION
            && (mtmp->mspec_used > 0 || ((mtmp->mmagespell_used || mtmp->mclericspell_used))
                || ((mtmp->mmageintermediate_used || mtmp->mclericintermediate_used))
                || mtmp->mmageultimate_used || mtmp->mclericultimate_used
                || (mtmp->mhp < (3 * mtmp->mhpmax) / 4 && !has_property(mtmp, DIVINE_REGENERATION) && !has_property(mtmp, RAPIDEST_REGENERATION) && !has_property(mtmp, RAPIDER_REGENERATION) && (!has_property(mtmp, RAPID_REGENERATION) || !rn2(2)))
                ))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_REJUVENATION;
        }
        nomore(MUSE_POT_LESSER_REJUVENATION);
        if (obj->otyp == POT_LESSER_REJUVENATION
            && (mtmp->mspec_used > 0 || ((mtmp->mmagespell_used || mtmp->mclericspell_used))
                || ((mtmp->mmageintermediate_used || mtmp->mclericintermediate_used))
                || mtmp->mmageultimate_used || mtmp->mclericultimate_used
                || (mtmp->mhp < (3 * mtmp->mhpmax) / 4 && !has_property(mtmp, DIVINE_REGENERATION) && !has_property(mtmp, RAPIDEST_REGENERATION) && !has_property(mtmp, RAPIDER_REGENERATION) && !has_property(mtmp, RAPID_REGENERATION))
                ))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_LESSER_REJUVENATION;
        }
        nomore(MUSE_POT_GREATER_REGENERATION);
        if (obj->otyp == POT_GREATER_REGENERATION
            && ((mtmp->mhp < (3 * mtmp->mhpmax) / 4 && !has_property(mtmp, DIVINE_REGENERATION) && (!has_property(mtmp, RAPIDEST_REGENERATION) || !rn2(2)))
                ))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_GREATER_REGENERATION;
        }
        nomore(MUSE_POT_REGENERATION);
        if (obj->otyp == POT_REGENERATION
            && ((mtmp->mhp < (3 * mtmp->mhpmax) / 4 && !has_property(mtmp, DIVINE_REGENERATION) && !has_property(mtmp, RAPIDEST_REGENERATION) && (!has_property(mtmp, RAPIDER_REGENERATION) || !rn2(2)))
                ))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_REGENERATION;
        }
        nomore(MUSE_POT_LESSER_REGENERATION);
        if (obj->otyp == POT_LESSER_REGENERATION
            && ((mtmp->mhp < (3 * mtmp->mhpmax) / 4 && !has_property(mtmp, DIVINE_REGENERATION) && !has_property(mtmp, RAPIDEST_REGENERATION) && !has_property(mtmp, RAPIDER_REGENERATION) && (!has_property(mtmp, RAPID_REGENERATION) || !rn2(2)))
                ))
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_LESSER_REGENERATION;
        }
        nomore(MUSE_POT_TITAN_STRENGTH);
        if (obj->otyp == POT_TITAN_STRENGTH && !has_titan_strength(mtmp) && M_ACURRSTR(mtmp) < 25 && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_POT_TITAN_STRENGTH;
        }
        nomore(MUSE_POT_SUPER_HEROISM);
        if (obj->otyp == POT_SUPER_HEROISM && !has_super_heroism(mtmp) && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_POT_SUPER_HEROISM;
        }
        nomore(MUSE_POT_HEROISM);
        if (obj->otyp == POT_HEROISM && !has_super_heroism(mtmp) && !has_heroism(mtmp) && !mtmp->isgd) {
            m.misc = obj;
            m.has_misc = MUSE_POT_HEROISM;
        }
        nomore(MUSE_WAN_POLYMORPH);
        if (obj->otyp == WAN_POLYMORPH && obj->charges > 0 && !is_cancelled(mtmp)
            && (mtmp->cham == NON_PM) && mons[mtmp->mnum].difficulty < 6) {
            m.misc = obj;
            m.has_misc = MUSE_WAN_POLYMORPH;
        }
        nomore(MUSE_POT_POLYMORPH);
        if (obj->otyp == POT_POLYMORPH && (mtmp->cham == NON_PM)
            && mons[mtmp->mnum].difficulty < 6) {
            m.misc = obj;
            m.has_misc = MUSE_POT_POLYMORPH;
        }
        nomore(MUSE_POT_WATER);
        if (obj->otyp == POT_WATER)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_WATER;
        }
        nomore(MUSE_POT_FRUIT_JUICE);
        if (obj->otyp == POT_FRUIT_JUICE)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_FRUIT_JUICE;
        }
        nomore(MUSE_POT_LEVITATION);
        if (obj->otyp == POT_LEVITATION)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_LEVITATION;
        }
        nomore(MUSE_POT_FIRE_IMMUNITY);
        if (obj->otyp == POT_FIRE_IMMUNITY)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_FIRE_IMMUNITY;
        }
        nomore(MUSE_POT_SHOCK_IMMUNITY);
        if (obj->otyp == POT_SHOCK_IMMUNITY)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_SHOCK_IMMUNITY;
        }
        nomore(MUSE_POT_COLD_IMMUNITY);
        if (obj->otyp == POT_COLD_IMMUNITY)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_COLD_IMMUNITY;
        }
        nomore(MUSE_POT_MAGIC_RESISTANCE);
        if (obj->otyp == POT_MAGIC_RESISTANCE)
        {
            //m.misc = obj;
            //m.has_misc = MUSE_POT_MAGIC_RESISTANCE;
        }
        nomore(MUSE_POT_SEE_INVISIBLE);
        if (obj->otyp == POT_SEE_INVISIBLE && !is_peaceful(mtmp) && Invis && !has_see_invisible(mtmp) 
            && isok(mtmp->mux, mtmp->muy) && m_cansee(mtmp, mtmp->mux, mtmp->muy)) 
        {
            m.misc = obj;
            m.has_misc = MUSE_POT_SEE_INVISIBLE;
        }
    }
    return (boolean) !!m.has_misc;
#undef nomore
}

/* type of monster to polymorph into; defaults to one suitable for the
   current level rather than the totally arbitrary choice of newcham() */
STATIC_OVL struct permonst *
muse_newcham_mon(mon)
struct monst *mon;
{
    struct obj *m_armr;

    if ((m_armr = which_armor(mon, W_ARM)) != 0) {
        if (is_dragon_scales(m_armr))
            return Dragon_scales_to_pm(m_armr);
        else if (is_dragon_mail(m_armr))
            return Dragon_mail_to_pm(m_armr);
    }
    return rndmonst();
}

int
use_misc(mtmp)
struct monst *mtmp;
{
    if (!mtmp)
        return 0;

    int i;
    struct obj *otmp = m.misc;
    boolean vis, vismon, oseen;
    char nambuf[BUFSZ];

    if ((i = precheck(mtmp, otmp)) != 0)
        return i;

    int duration = 0, dicebuc = 0;
    int sfx = 0;
    boolean isdiluted = FALSE;

    if (otmp)
    {
        dicebuc = (int)(otmp->oclass == POTION_CLASS ? objects[otmp->otyp].oc_potion_normal_dice_buc_multiplier : 0);
        duration =
            (int)max(0,
                otmp->oclass == POTION_CLASS ? (objects[otmp->otyp].oc_potion_normal_diesize == 0 ? 0 : d(max(0, objects[otmp->otyp].oc_potion_normal_dice + dicebuc * bcsign(otmp)), max(1, objects[otmp->otyp].oc_potion_normal_diesize))) + objects[otmp->otyp].oc_potion_normal_plus + bcsign(otmp) * objects[otmp->otyp].oc_potion_normal_buc_multiplier :
                d(objects[otmp->otyp].oc_spell_dur_dice, objects[otmp->otyp].oc_spell_dur_diesize) + objects[otmp->otyp].oc_spell_dur_plus
            );
        /* Adjustment for dilution */
        isdiluted = otmp->oclass == POTION_CLASS && otmp->odiluted;
        if (isdiluted)
        {
            duration /= 2;
        }
    }

    vis = cansee(mtmp->mx, mtmp->my);
    vismon = canseemon(mtmp);
    oseen = otmp && vismon;

    switch (m.has_misc) {
    case MUSE_POT_GAIN_LEVEL:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        if (otmp->cursed) {
            if (Can_rise_up(mtmp->mx, mtmp->my, &u.uz) && (!isdiluted || !rn2(2))) {
                register int tolev = depth(&u.uz) - 1;
                d_level tolevel;

                get_level(&tolevel, tolev);
                /* insurance against future changes... */
                if (on_level(&tolevel, &u.uz))
                    goto skipmsg;
                if (vismon) {
                    char dcbuf[BUFSZ] = "";
                    play_sfx_sound_at_location(SFX_CURSED_GAIN_LEVEL, mtmp->mx, mtmp->my);
                    Sprintf(dcbuf, "%s rises up, through the %s!", Monnam(mtmp),
                          ceiling(mtmp->mx, mtmp->my));
                    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                    if (!objects[POT_GAIN_LEVEL].oc_name_known
                        && !objects[POT_GAIN_LEVEL].oc_uname)
                        docall(otmp, dcbuf);
                }
                m_useup(mtmp, otmp);
                migrate_to_level(mtmp, ledger_no(&tolevel), MIGR_RANDOM,
                                 (coord *) 0);
                return 2;
            } else {
 skipmsg:
                if (vismon) {
                    char dcbuf[BUFSZ] = "";
                    Sprintf(dcbuf, "%s looks uneasy.", Monnam(mtmp));
                    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                    if (!objects[POT_GAIN_LEVEL].oc_name_known
                        && !objects[POT_GAIN_LEVEL].oc_uname)
                        docall(otmp, dcbuf);
                }
                m_useup(mtmp, otmp);
                return 2;
            }
        }

        if (!isdiluted || !rn2(2))
        {
            if (vismon)
            {
                pline("%s seems more experienced.", Monnam(mtmp));
            }
            if (oseen)
                makeknown(POT_GAIN_LEVEL);
            m_useup(mtmp, otmp);
            if (!grow_up(mtmp, (struct monst*)0))
                return 1;
            /* grew into genocided monster */
        }
        else
        {
            if (vismon)
            {
                pline("%s looks peculiarly elevated.", Monnam(mtmp));
            }
            if (oseen)
                makeknown(POT_GAIN_LEVEL);
            m_useup(mtmp, otmp);
        }
        return 2;
    case MUSE_WAN_MAKE_INVISIBLE:
    case MUSE_POT_INVISIBILITY:
        if (!otmp)
            return 2;
        if (otmp->otyp == WAN_MAKE_INVISIBLE) {
            mzapmsg(mtmp, otmp, TRUE);
            otmp->charges--;
        } else
            mquaffmsg(mtmp, otmp);
        /* format monster's name before altering its visibility */
        Strcpy(nambuf, mon_nam(mtmp));
        increase_mon_property(mtmp, INVISIBILITY, duration);
        if (vismon && is_invisible(mtmp)) { /* was seen, now invisible */
            if (canspotmon(mtmp)) {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s body takes on a %s transparency.",
                      upstart(s_suffix(nambuf)),
                      Hallucination ? "normal" : "strange");
            } else {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly you cannot see %s.", nambuf);
                if (vis)
                    map_invisible(mtmp->mx, mtmp->my);
            }
            if (oseen)
                makeknown(otmp->otyp);
        }
        if (otmp->otyp == POT_INVISIBILITY) {
            if (otmp->cursed)
                you_aggravate(mtmp);
            m_useup(mtmp, otmp);
        }
        return 2;
    case MUSE_WAN_SPEED_MONSTER:
        if (!otmp)
            return 2;
        mzapmsg(mtmp, otmp, TRUE);
        otmp->charges--;
        (void)increase_mon_property_verbosely(mtmp, VERY_FAST, duration);
        return 2;
    case MUSE_POT_SPEED:
    case MUSE_POT_GREATER_SPEED:
    case MUSE_POT_LIGHTNING_SPEED:
    case MUSE_POT_HEROISM:
    case MUSE_POT_SUPER_HEROISM:
    case MUSE_POT_TITAN_STRENGTH:
    case MUSE_POT_GREATER_REGENERATION:
    case MUSE_POT_REGENERATION:
    case MUSE_POT_LESSER_REGENERATION:
    case MUSE_POT_GREATER_REJUVENATION:
    case MUSE_POT_REJUVENATION:
    case MUSE_POT_LESSER_REJUVENATION:
    case MUSE_POT_SEE_INVISIBLE:
    case MUSE_POT_LEVITATION:
    case MUSE_POT_FIRE_IMMUNITY:
    case MUSE_POT_COLD_IMMUNITY:
    case MUSE_POT_SHOCK_IMMUNITY:
    case MUSE_POT_MAGIC_RESISTANCE:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        if (objects[otmp->otyp].oc_oprop > 0)
            (void)increase_mon_property_verbosely(mtmp, objects[otmp->otyp].oc_oprop, duration);
        if (objects[otmp->otyp].oc_oprop2 > 0)
            (void)increase_mon_property_verbosely(mtmp, objects[otmp->otyp].oc_oprop2, duration);
        if (objects[otmp->otyp].oc_oprop3 > 0)
            (void)increase_mon_property_verbosely(mtmp, objects[otmp->otyp].oc_oprop3, duration);

        switch (objects[otmp->otyp].oc_oprop)
        {
        case VERY_FAST:
        case ULTRA_FAST:
        case SUPER_FAST:
        case LIGHTNING_FAST:
            sfx = SFX_ACQUIRE_HASTE;
            break;
        }
        if(sfx > 0)
        play_sfx_sound_at_location(sfx, mtmp->mx, mtmp->my);
        m_useup(mtmp, otmp);
        return 2;
    case MUSE_POT_WATER:
    case MUSE_POT_FRUIT_JUICE:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        if (has_edog(mtmp))
        {
            Strcpy(debug_buf_2, "use_misc");
            obj_extract_self(otmp);
            place_object(otmp, mtmp->mx, mtmp->my);
            dog_eat(mtmp, otmp, mtmp->mx, mtmp->my, FALSE);
            /* It's gone now */
            otmp = 0;
        }
        break;
    case MUSE_POT_GAIN_ENERGY:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        mtmp->mspec_used = 0;
        mtmp->mmagespell_used = 0;
        mtmp->mclericspell_used = 0;
        mtmp->mmageintermediate_used /= (!isdiluted ? 4 : 2);
        mtmp->mclericintermediate_used /= (!isdiluted ? 4 : 2);
        mtmp->mmageultimate_used /= (!isdiluted ? 2 : 1);
        mtmp->mclericultimate_used /= (!isdiluted ? 2 : 1);
        m_useup(mtmp, otmp);
        return 2;
    case MUSE_POT_EXTRA_ENERGY:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        mtmp->mspec_used = 0;
        mtmp->mmagespell_used = 0;
        mtmp->mclericspell_used = 0;
        mtmp->mmageintermediate_used /= (!isdiluted ? 8 : 4);
        mtmp->mclericintermediate_used /= (!isdiluted ? 8 : 4);
        mtmp->mmageultimate_used /= (!isdiluted ? 4 : 2);
        mtmp->mclericultimate_used /= (!isdiluted ? 4 : 2);
        m_useup(mtmp, otmp);
        return 2;
    case MUSE_POT_GREATER_ENERGY:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        mtmp->mspec_used = 0;
        mtmp->mmagespell_used = 0;
        mtmp->mclericspell_used = 0;
        mtmp->mmageintermediate_used = 0;
        mtmp->mclericintermediate_used = 0;
        mtmp->mmageultimate_used /= (!isdiluted ? 8 : 4);
        mtmp->mclericultimate_used /= (!isdiluted ? 8 : 4);
        m_useup(mtmp, otmp);
        return 2;
    case MUSE_POT_FULL_ENERGY:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        mtmp->mspec_used = 0;
        mtmp->mmagespell_used = 0;
        mtmp->mclericspell_used = 0;
        mtmp->mmageintermediate_used = 0;
        mtmp->mclericintermediate_used = 0;
        mtmp->mmageultimate_used = 0;
        mtmp->mclericultimate_used = 0;
        m_useup(mtmp, otmp);
        return 2;
    case MUSE_WAN_POLYMORPH:
        if (!otmp)
            return 2;
        mzapmsg(mtmp, otmp, TRUE);
        otmp->charges--;
        if(newcham(mtmp, muse_newcham_mon(mtmp), 0, TRUE, FALSE))
            play_sfx_sound_at_location(SFX_POLYMORPH_SUCCESS, mtmp->mx, mtmp->my);

        if (oseen)
            makeknown(WAN_POLYMORPH);
        return 2;
    case MUSE_POT_POLYMORPH:
        if (!otmp)
            return 2;
        mquaffmsg(mtmp, otmp);
        if (vismon)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly mutates!", Monnam(mtmp));
        if (newcham(mtmp, muse_newcham_mon(mtmp), 0, FALSE, FALSE))
        {
            play_sfx_sound_at_location(SFX_POLYMORPH_SUCCESS, mtmp->mx, mtmp->my);
        }
        if (oseen)
            makeknown(POT_POLYMORPH);
        m_useup(mtmp, otmp);
        return 2;
    case MUSE_POLY_TRAP:
        if (vismon) {
            const char *Mnam = Monnam(mtmp);

            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s deliberately %s onto a polymorph trap!", Mnam,
                  vtense(Mnam, locomotion(mtmp->data, "jump")));
        }
        if (vis)
            seetrap(t_at(trapx, trapy));

        /*  don't use rloc() due to worms */
        remove_monster(mtmp->mx, mtmp->my);
        newsym(mtmp->mx, mtmp->my);
        place_monster(mtmp, trapx, trapy);
        if (mtmp->wormno)
            worm_move(mtmp);
        newsym(trapx, trapy);

        if(newcham(mtmp, (struct permonst *) 0, 0, FALSE, FALSE))
            play_sfx_sound_at_location(SFX_POLYMORPH_SUCCESS, mtmp->mx, mtmp->my);

        return 2;
    case MUSE_WAN_WISHING: {
        if (!otmp)
            return 2;
        /* wear any armor items previously wished for before
         * using another wish */
        m_dowear(mtmp, FALSE, FALSE);
        mzapmsg(mtmp, otmp, FALSE);
        otmp->charges--;
        if (!vismon && !Deaf)
            You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "something making a wish!");
        if (oseen)
            makeknown(WAN_WISHING);
        mmake_wish(mtmp);
        return 2;
    }
    case MUSE_BULLWHIP:
        /* attempt to disarm hero */
        {
            const char *The_whip = vismon ? "The bullwhip" : "A whip";
            int where_to = rn2(4);
            struct obj *obj = uwep;
            const char *hand;
            char the_weapon[BUFSZ];

            if (!obj || !canletgo(obj, "")
                || (u.twoweap && uarms && canletgo(uarms, "") && rn2(2)))
                obj = uarms;
            if (!obj)
                break; /* shouldn't happen after find_misc() */

            Strcpy(the_weapon, the(xname(obj)));
            hand = body_part(HAND);
            if (bimanual(obj))
                hand = makeplural(hand);

            if (vismon)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s flicks a bullwhip towards your %s!", Monnam(mtmp),
                      hand);
            if (obj->otyp == HEAVY_IRON_BALL) {
                pline("%s fails to wrap around %s.", The_whip, the_weapon);
                return 1;
            }
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s wraps around %s you're wielding!", The_whip,
                  the_weapon);
            if (welded(obj, &youmonst)) {
                pline("%s welded to your %s%c",
                      !is_plural(obj) ? "It is" : "They are", hand,
                      !obj->bknown ? '!' : '.');
                /* obj->bknown = 1; */ /* welded() takes care of this */
                where_to = 0;
            }
            if (!where_to) {
                pline_The("whip slips free."); /* not `The_whip' */
                return 1;
            } else if (where_to == 3 && mon_hates_silver(mtmp)
                       && obj_counts_as_silver(obj)) {
                /* this monster won't want to catch a silver
                   weapon; drop it at hero's feet instead */
                where_to = 2;
            }
            remove_worn_item(obj, FALSE);
            freeinv(obj);
            switch (where_to) {
            case 1: /* onto floor beneath mon */
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s yanks %s from your %s!", Monnam(mtmp), the_weapon,
                      hand);
                place_object(obj, mtmp->mx, mtmp->my);
                break;
            case 2: /* onto floor beneath you */
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s yanks %s to the %s!", Monnam(mtmp), the_weapon,
                      surface(u.ux, u.uy));
                dropy(obj);
                break;
            case 3: /* into mon's inventory */
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s snatches %s!", Monnam(mtmp), the_weapon);
                (void) mpickobj(mtmp, obj);
                break;
            }
            return 1;
        }
        return 0;
    case 0:
        return 0; /* i.e. an exploded wand */
    default:
        impossible("%s wanted to perform action %d?", Monnam(mtmp),
                   m.has_misc);
        break;
    }
    return 0;
}

STATIC_OVL void
you_aggravate(mtmp)
struct monst *mtmp;
{
    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "For some reason, %s presence is known to you.",
          s_suffix(noit_mon_nam(mtmp)));
    cls();
#ifdef CLIPPING
    cliparound(mtmp->mx, mtmp->my, FALSE);
#endif
    show_monster_glyph_with_extra_info(mtmp->mx, mtmp->my, any_mon_to_glyph(mtmp, rn2), mtmp, 0UL, 0UL, 0, 0);
    display_self();
    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "aggravated at %s.", noit_mon_nam(mtmp));
    create_context_menu(CREATE_CONTEXT_MENU_BLOCKING_WINDOW);
    display_nhwindow(WIN_MAP, TRUE);
    create_context_menu(CREATE_CONTEXT_MENU_NORMAL);
    docrt();
    if (unconscious()) {
        multi = -1;
        nomovemsg = "Aggravated, you are jolted into full consciousness.";
        nomovemsg_attr = ATR_NONE;
        nomovemsg_color = CLR_MSG_ATTENTION;
    }
    newsym(mtmp->mx, mtmp->my);
    if (!canspotmon(mtmp))
        map_invisible(mtmp->mx, mtmp->my);
}

int
rnd_misc_item(mtmp)
struct monst *mtmp;
{
    struct permonst *pm = mtmp->data;
    int difficulty = mons[mtmp->mnum].difficulty;

    if (is_animal(pm) || attacktype(pm, AT_EXPL) || mindless(mtmp->data)
        || pm->mlet == S_GHOST || pm->mlet == S_KOP)
        return 0;
    /* Unlike other rnd_item functions, we only allow _weak_ monsters
     * to have this item; after all, the item will be used to strengthen
     * the monster and strong monsters won't use it at all...
     */
    if (difficulty < 6 && !rn2(30))
        return rn2(6) ? POT_POLYMORPH : WAN_POLYMORPH;

    if (!rn2(40) && !is_not_living(pm) && !is_vampshifter(mtmp))
        return AMULET_OF_LIFE_SAVING;

    switch (rn2(3)) {
    case 0:
        if (mtmp->isgd)
            return 0;
        return rn2(6) ? POT_SPEED : WAN_SPEED_MONSTER;
    case 1:
        if (is_peaceful(mtmp) && !See_invisible)
            return 0;
        return rn2(6) ? POT_INVISIBILITY : WAN_MAKE_INVISIBLE;
    case 2:
        return POT_GAIN_LEVEL;
    }
    /*NOTREACHED*/
    return 0;
}

boolean
searches_for_item(mon, obj)
struct monst *mon;
struct obj *obj;
{
    int typ = obj->otyp;

    if (is_obj_no_pickup(obj))
        return FALSE;

    if (is_animal(mon->data) || mindless(mon->data)
        || mon->data == &mons[PM_GHOST]) /* don't loot bones piles */
        return FALSE;

    if (typ == WAN_MAKE_INVISIBLE || typ == POT_INVISIBILITY)
        return (boolean) (!has_invisibility(mon) && !has_blocks_invisibility(mon)
                          && !attacktype(mon->data, AT_GAZE));
    if (typ == WAN_SPEED_MONSTER || typ == POT_SPEED)
        return !has_very_fast(mon);

    switch (obj->oclass) 
    {
    case WAND_CLASS:
        if (obj->charges <= 0)
            return FALSE;
        if (typ == WAN_DIGGING)
            return (boolean) !is_floater(mon->data);
        if (typ == WAN_POLYMORPH)
            return (boolean) (mons[mon->mnum].difficulty < 6);
        if (objects[typ].oc_dir == RAY || typ == WAN_STRIKING
            || typ == WAN_TELEPORTATION || typ == WAN_CREATE_MONSTER)
            return TRUE;
        break;
    case POTION_CLASS:
        if (typ == POT_HEALING || typ == POT_EXTRA_HEALING
            || typ == POT_FULL_HEALING || typ == POT_POLYMORPH
            || typ == POT_GAIN_LEVEL || typ == POT_PARALYSIS
            || typ == POT_SLEEPING || typ == POT_ACID || typ == POT_CONFUSION)
            return TRUE;
        if (typ == POT_BLINDNESS && !attacktype(mon->data, AT_GAZE))
            return TRUE;
        break;
    case SCROLL_CLASS:
        if (typ == SCR_TELEPORTATION || typ == SCR_CREATE_MONSTER
            || typ == SCR_EARTH || typ == SCR_FIRE)
            return TRUE;
        break;
    case AMULET_CLASS:
        if (typ == AMULET_OF_LIFE_SAVING)
            return (boolean) !(is_not_living(mon->data) || is_vampshifter(mon));
        if (typ == AMULET_OF_REFLECTION)
            return !is_reflecting(mon);
        if (typ == AMULET_OF_UNCHANGING)
            return TRUE;
        if (typ == AMULET_OF_ESP)
            return !has_telepathy(mon);
        if (typ == AMULET_VERSUS_PETRIFICATION)
            return !resists_ston(mon);
        if (typ == AMULET_VERSUS_UNDEATH)
            return !resists_death(mon) && !resists_drain(mon);
        if (typ == AMULET_VERSUS_POISON)
            return !resists_poison(mon);
        break;
    case RING_CLASS:
        if ((mon->worn_item_flags & W_RING) == W_RING)
            return FALSE;
        if (!is_cursed_magic_item(obj))
            return TRUE;
        break;
    case MISCELLANEOUS_CLASS:
        break;
    case TOOL_CLASS:
        if (typ == PICK_AXE)
            return (boolean) needspick(mon->data);
        if (typ == UNICORN_HORN)
            return (boolean) (!obj->cursed && !is_unicorn(mon->data));
        if (typ == FROST_HORN || typ == FIRE_HORN)
            return (obj->charges > 0 && can_blow(mon));
        break;
    case FOOD_CLASS:
        if (typ == CORPSE)
            return (boolean) (((mon->worn_item_flags & W_ARMG) != 0L
                               && obj->corpsenm >= LOW_PM && touch_petrifies(&mons[obj->corpsenm]))
                              || (!resists_ston(mon)
                                  && cures_stoning(mon, obj, FALSE)));
        if (typ == TIN)
            return (boolean) (mcould_eat_tin(mon)
                              && (!resists_ston(mon)
                                  && cures_stoning(mon, obj, TRUE)));
        if (typ == EGG)
            return (boolean)(obj->corpsenm >= LOW_PM && touch_petrifies(&mons[obj->corpsenm]));
        break;
    default:
        break;
    }

    return FALSE;
}

boolean
mon_reflects(mon, str)
struct monst *mon;
const char *str;
{
    if (!mon)
        return FALSE;

    if (!is_reflecting(mon))
        return FALSE;

    if (!(mon->mprops[REFLECTING] & M_EXTRINSIC))
    {
        if (str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, str, s_suffix(mon_nam(mon)), mon->data->mlet == S_DRAGON ? "scales" : mon->data->mlet == S_TREANT ? "bark" : "body");

        return TRUE;
    }

    struct obj* orefl = what_gives_monster(mon, REFLECTING);
    if(orefl)
    {
        if (is_shield(orefl)) 
        {
            if (str) 
            {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, str, s_suffix(mon_nam(mon)), "shield");
                makeknown(orefl->otyp);
            }
            return TRUE;
        }
        else if (is_wieldable_weapon(orefl))
        {
            if (str)
            {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, str, s_suffix(mon_nam(mon)), "weapon");
                makeknown(orefl->otyp);
            }
            return TRUE;
        } 
        else if (is_amulet(orefl)) 
        {
            if (str) 
            {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, str, s_suffix(mon_nam(mon)), "amulet");
                makeknown(orefl->otyp);
            }
            return TRUE;
        }
        else if (is_suit(orefl)) 
        {
            if (str)
            {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, str, s_suffix(mon_nam(mon)), "armor");
                makeknown(orefl->otyp);
            }
            return TRUE;
        }
        else
        {
            if (str)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, str, s_suffix(mon_nam(mon)), "item");

            return TRUE;
        }
    }
    return FALSE;
}

struct obj*
what_gives_monster(mon, prop_index)
struct monst* mon;
int prop_index;
{
    if (!mon)
        return (struct obj*)0;

    for (struct obj* otmp = (mon == &youmonst ? invent : mon->minvent); otmp; otmp = otmp->nobj)
    {
        if(item_is_giving_monster_power(mon, otmp, prop_index, FALSE))
            return otmp;
    }

    return (struct obj*)0;
}

boolean
ureflects(fmt, str)
const char *fmt, *str;
{
    /* Check from outermost to innermost objects */
    if (EReflecting & W_ARMS) 
    {
        if (fmt && str)
        {
            if (uarms)
            {
                if(is_shield(uarms))
                {
                    if (u.twoweap)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "left hand shield");
                    else
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "shield");
                    makeknown(uarms->otyp);
                }
                else if(is_wieldable_weapon(uarms))
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "left hand weapon");
                }
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "left hand item");
                }
            }
        }
        return TRUE;
    }
    else if (EReflecting & W_WEP) 
    {
        /* Due to wielded artifact weapon */
        if (fmt && str)
        {
            if (uwep)
            {
                if (is_shield(uwep))
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "right hand shield");
                    makeknown(uwep->otyp);
                }
                else if (is_wieldable_weapon(uwep))
                {
                    if(u.twoweap)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "right hand weapon");
                    else
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "weapon");
                }
                else
                {
                    if (u.twoweap)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "right hand item");
                    else
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "wielded item");
                }
            }

        }
        return TRUE;
    }
    else if (EReflecting & W_AMUL)
    {
        if (fmt && str)
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "medallion");
            if(uamul)
                makeknown(uamul->otyp);
        }
        return TRUE;
    }
    else if (EReflecting & W_ARMC)
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "cloak");
        if (uarmc)
            makeknown(uarmc->otyp);
        return TRUE;
    }
    else if (EReflecting & W_ARMO)
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "robe");
        if (uarmo)
            makeknown(uarmo->otyp);
        return TRUE;
    }
    else if (EReflecting & W_ARM) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, uskin ? "luster" : "armor");
        return TRUE;
    }
    else if (EReflecting & W_ARMB) 
    {
        if (fmt && str)
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "bracers");
            if(uarmb)
                makeknown(uarmb->otyp);
        }
        return TRUE;
    }
    else if (is_reflecting(&youmonst))
    {  //youmonst.data == &mons[PM_SILVER_DRAGON]) {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "scales");
        return TRUE;
    }
    else if (HReflecting & TIMEOUT)
    { //Spell
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "protective spell");
        return TRUE;
    }
    else if ((EReflecting & W_MISC) && umisc)
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, xname(umisc));
        return TRUE;
    }
    else if ((EReflecting & W_MISC2) && umisc2) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, xname(umisc2));
        return TRUE;
    }
    else if (EReflecting & W_MISC3 && umisc3) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, xname(umisc3));
        return TRUE;
    }
    else if (EReflecting & W_MISC4 && umisc4) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, xname(umisc4));
        return TRUE;
    }
    else if (EReflecting & W_MISC5 && umisc5) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, xname(umisc5));
        return TRUE;
    }
    else if (EReflecting & W_CARRIED)
    {
        struct obj* selobj = (struct obj*)0;
        for (struct obj* otmp = invent; otmp; otmp = otmp->nobj)
        {
            if (carried_base_item_is_giving_monster_power(&youmonst, otmp, REFLECTING, FALSE))
            {
                selobj = otmp;
                break;
            }
        }
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, selobj ? xname(selobj) :"item-induced force field");
        return TRUE;
    }
    else if (EReflecting & W_ARTIFACT_CARRIED)
    {
        struct obj* selobj = (struct obj*)0;
        for (struct obj* otmp = invent; otmp; otmp = otmp->nobj)
        {
            if (carried_artifact_is_giving_monster_power(&youmonst, otmp, REFLECTING))
            {
                selobj = otmp;
                break;
            }
        }
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, selobj ? xname(selobj) : "artifact");
        return TRUE;
    }
    else if (EReflecting & W_ARTIFACT_INVOKED)
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "artifact-invoked force field");
        return TRUE;
    }
    else if (EReflecting) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "item");
        return TRUE;
    }
    else if (HReflecting) 
    {
        if (fmt && str)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, str, "body");
        return TRUE;
    }
    return FALSE;
}

/* cure mon's blindness (use_defensive, dog_eat, meatobj) */
void
mcureblindness(mon, verbos)
struct monst *mon;
boolean verbos;
{
    if (is_blinded(mon)) 
    {
        mon->mprops[BLINDED] = 0;
        if (verbos && haseyes(mon->data))
            pline("%s can see again.", Monnam(mon));
    }
}

void
mcuresickness(mtmp, verbos)
struct monst* mtmp;
boolean verbos;
{
    if (!mtmp)
        return;

    if (has_sick(mtmp))
    {
        (void)set_mon_property_b(mtmp, SICK, 0, verbos && canseemon(mtmp));
    }
    if (has_food_poisoned(mtmp))
    {
        (void)set_mon_property_b(mtmp, FOOD_POISONED, 0, verbos && canseemon(mtmp));
    }
    if (has_mummy_rot(mtmp))
    {
        (void)set_mon_property_b(mtmp, MUMMY_ROT, -3, verbos && canseemon(mtmp));
    }
    if (has_vomiting(mtmp))
    {
        (void)set_mon_property_b(mtmp, VOMITING, 0, verbos && canseemon(mtmp));
    }
}

void
mcurehallucination(mtmp, verbos)
struct monst* mtmp;
boolean verbos;
{
    if (!mtmp)
        return;

    if (has_hallucination(mtmp))
    {
        (void)set_mon_property_b(mtmp, HALLUC, 0, verbos && canseemon(mtmp));
    }
}

void
mcureconfusion(mtmp, verbos)
struct monst* mtmp;
boolean verbos;
{
    if (!mtmp)
        return;

    if (has_confused(mtmp))
    {
        (void)set_mon_property_b(mtmp, CONFUSION, 0, verbos && canseemon(mtmp));
    }
}

void
mcurestun(mtmp, verbos)
struct monst* mtmp;
boolean verbos;
{
    if (!mtmp)
        return;

    if (has_stunned(mtmp))
    {
        (void)set_mon_property_b(mtmp, STUNNED, 0, verbos && canseemon(mtmp));
    }
}

/* TRUE if the monster ate something */
boolean
munstone(mon, by_you)
struct monst *mon;
boolean by_you;
{
    struct obj *obj;
    boolean tinok;

    if (resists_ston(mon))
        return FALSE;
    if (mon->meating || !mon_can_move(mon))
        return FALSE;
    mon->mstrategy &= ~STRAT_WAITFORU;

    tinok = mcould_eat_tin(mon);
    for (obj = mon->minvent; obj; obj = obj->nobj) {
        if (cures_stoning(mon, obj, tinok)) {
            mon_consume_unstone(mon, obj, by_you, TRUE);
            return TRUE;
        }
    }
    return FALSE;
}

STATIC_OVL void
mon_consume_unstone(mon, obj, by_you, stoning)
struct monst *mon;
struct obj *obj;
boolean by_you;
boolean stoning; /* True: stop petrification, False: cure stun && confusion */
{
    boolean vis = canseemon(mon), tinned = obj->otyp == TIN,
            food = obj->otyp == CORPSE || tinned,
            acid = obj->otyp == POT_ACID
                   || (food && obj->corpsenm >= LOW_PM && has_acidic_corpse(&mons[obj->corpsenm])),
            lizard = food && obj->corpsenm == PM_LIZARD;
    boolean dragonfruit = (obj->otyp == DRAGON_FRUIT);
    int nutrit = food || dragonfruit ? dog_nutrition(mon, obj) : 0; /* also sets meating */

    if (vis) {
        int64_t save_quan = obj->quan;

        obj->quan = 1L;
        pline("%s %s %s.", Monnam(mon),
              (obj->oclass == POTION_CLASS)
                  ? "quaffs"
                  : (obj->otyp == TIN) ? "opens and eats the contents of"
                                       : "eats",
              distant_name(obj, doname));
        obj->quan = save_quan;
    } else if (!Deaf)
        You_hear("%s.",
                 (obj->oclass == POTION_CLASS) ? "drinking" : "chewing");

    m_useup(mon, obj);
    /* obj is now gone */

    if (acid && !tinned && !is_mon_immune_to_acid(mon)) 
    {
        deduct_monster_hp(mon, adjust_damage(rnd(15), (struct monst*)0, mon, AD_ACID, ADFLAGS_NONE));
        if (vis)
            pline("%s has a very bad case of stomach acid.", Monnam(mon));
        if (DEADMONSTER(mon)) {
            pline("%s dies!", Monnam(mon));
            if (by_you)
                /* hero gets credit (experience) and blame (possible loss
                   of alignment and/or luck and/or telepathy depending on
                   mon) for the kill but does not break pacifism conduct */
                xkilled(mon, XKILL_NOMSG | XKILL_NOCONDUCT);
            else
                mondead(mon);
            return;
        }
    }
    if (stoning && vis) 
    {
        if (Hallucination)
            pline("What a pity - %s just ruined a future piece of art!",
                  mon_nam(mon));
        else
            pline("%s seems limber!", Monnam(mon));
    }
    if (lizard && (is_confused(mon) || is_stunned(mon))) 
    {
        mon->mprops[CONFUSION]  = 0;
        mon->mprops[STUNNED] = 0;
        if (vis && !is_bat(mon->data) && mon->data != &mons[PM_STALKER])
            pline("%s seems steadier now.", Monnam(mon));
    }
    if (mon->mtame && !mon->isminion && nutrit > 0 & has_edog(mon))
    {
        struct edog *edog = EDOG(mon);

        if (edog->hungrytime < monstermoves)
            edog->hungrytime = monstermoves;
        edog->hungrytime += nutrit;
        mon->mprops[CONFUSION] &= ~M_INTRINSIC_ACQUIRED;
    }
    /* use up monster's next move */
    mon->mprops[STONED] &= ~M_TIMEOUT;
    increase_mon_property(mon, STONE_RESISTANCE, 13);
    mon->movement -= NORMAL_SPEED;
    mon->mlstmv = monstermoves;
}

/* decide whether obj can cure petrification; also used when picking up */
STATIC_OVL boolean
cures_stoning(mon, obj, tinok)
struct monst *mon;
struct obj *obj;
boolean tinok;
{
    if (obj->otyp == POT_ACID)
        return TRUE;
    if (obj->otyp == DRAGON_FRUIT)
        return TRUE;
    if (obj->otyp != CORPSE && (obj->otyp != TIN || !tinok))
        return FALSE;
    /* corpse, or tin that mon can open */
    return (boolean) (obj->corpsenm == PM_LIZARD
                      || (obj->corpsenm >= LOW_PM && has_acidic_corpse(&mons[obj->corpsenm])
                          && (obj->corpsenm != PM_GREEN_SLIME
                              || resists_slime(mon))));
}

STATIC_OVL boolean
mcould_eat_tin(mon)
struct monst *mon;
{
    struct obj *obj, *mwep;
    boolean welded_wep;

    /* monkeys who manage to steal tins can't open and eat them
       even if they happen to also have the appropriate tool */
    if (is_animal(mon->data))
        return FALSE;

    mwep = MON_WEP(mon);
    welded_wep = mwep && mwelded(mwep, mon);
    /* this is different from the player; tin opener or dagger doesn't
       have to be wielded, and knife can be used instead of dagger */
    for (obj = mon->minvent; obj; obj = obj->nobj) {
        /* if stuck with a cursed weapon, don't check rest of inventory */
        if (welded_wep && obj != mwep)
            continue;

        if (obj->otyp == TIN_OPENER
            || (obj->oclass == WEAPON_CLASS
                && (objects[obj->otyp].oc_skill == P_DAGGER)))
            return TRUE;
    }
    return FALSE;
}

/* TRUE if monster does something to avoid turning into green slime */
boolean
munslime(mon, by_you)
struct monst *mon;
boolean by_you;
{
    struct obj *obj, odummy;
    struct permonst *mptr = mon->data;

    /*
     * muse_unslime() gives "mon starts turning green", "mon zaps
     * itself with a wand of fire", and "mon's slime burns away"
     * messages.  Monsters who don't get any chance at that just have
     * (via our caller) newcham()'s "mon turns into slime" feedback.
     */

    if (resists_slime(mon))
        return FALSE;
    if (mon->meating || !mon_can_move(mon))
        return FALSE;
    mon->mstrategy &= ~STRAT_WAITFORU;

    /* if monster can breathe fire, do so upon self; a monster who deals
       fire damage by biting, clawing, gazing, and especially exploding
       isn't able to cure itself of green slime with its own attack
       [possible extension: monst capable of casting high level clerical
       spells could toss pillar of fire at self--probably too suicidal] */
    if (!is_cancelled(mon) && !mon->mspec_used
        && attacktype_fordmg(mptr, AT_BREA, AD_FIRE)) {
        odummy = zeroobj; /* otyp == STRANGE_OBJECT */
        return muse_unslime(mon, &odummy, (struct trap *) 0, by_you);
    }

    /* same MUSE criteria as use_defensive() */
    if (!is_animal(mptr) && !mindless(mptr)) {
        struct trap *t;

        for (obj = mon->minvent; obj; obj = obj->nobj)
            if (cures_sliming(mon, obj))
                return muse_unslime(mon, obj, (struct trap *) 0, by_you);

        if (((t = t_at(mon->mx, mon->my)) == 0 || t->ttyp != FIRE_TRAP)
            && mptr->mmove && !mon->mtrapped) {
            int xy[2][8], x, y, idx, ridx, nxy = 0;

            for (x = mon->mx - 1; x <= mon->mx + 1; ++x)
                for (y = mon->my - 1; y <= mon->my + 1; ++y)
                    if (isok(x, y) && accessible(x, y)
                        && !m_at(x, y) && (x != u.ux || y != u.uy)) {
                        xy[0][nxy] = x, xy[1][nxy] = y;
                        ++nxy;
                    }
            for (idx = 0; idx < nxy; ++idx) {
                ridx = rn1(nxy - idx, idx);
                if (ridx != idx) {
                    x = xy[0][idx];
                    xy[0][idx] = xy[0][ridx];
                    xy[0][ridx] = x;
                    y = xy[1][idx];
                    xy[1][idx] = xy[1][ridx];
                    xy[1][ridx] = y;
                }
                if ((t = t_at(xy[0][idx], xy[1][idx])) != 0
                    && t->ttyp == FIRE_TRAP)
                    break;
            }
        }
        if (t && t->ttyp == FIRE_TRAP)
            return muse_unslime(mon, (struct obj *) &zeroobj, t, by_you);

    } /* MUSE */

    return FALSE;
}

/* mon uses an item--selected by caller--to burn away incipient slime */
STATIC_OVL boolean
muse_unslime(mon, obj, trap, by_you)
struct monst *mon;
struct obj *obj;
struct trap *trap;
boolean by_you; /* true: if mon kills itself, hero gets credit/blame */
{               /* [by_you not honored if 'mon' triggers fire trap]. */
    struct obj *odummyp;
    uchar zhitm_out_flags = 0;
    int otyp = obj->otyp;
    double damage = 0;
    boolean vis = canseemon(mon), res = TRUE;

    if (!mon)
        return FALSE;
    
    if (vis)
    {
        (void)get_colorless_multicolor_buffer();
        pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s starts turning %s.", Monnam(mon),
            green_mon(mon) ? "into ooze" : hcolor_multi_buf1(NH_GREEN));
    }
    /* -4 => sliming, causes quiet loss of enhanced speed */
    (void)increase_mon_property_verbosely(mon, SLIMED, 10);

    if (trap)
    {
        const char *Mnam = vis ? Monnam(mon) : 0;

        if (mon->mx == trap->tx && mon->my == trap->ty)
        {
            if (vis)
                pline("%s triggers %s fire trap!", Mnam,
                      trap->tseen ? "the" : "a");
        } 
        else 
        {
            remove_monster(mon->mx, mon->my);
            newsym(mon->mx, mon->my);
            place_monster(mon, trap->tx, trap->ty);
            if (mon->wormno) /* won't happen; worms don't MUSE to unslime */
                worm_move(mon);
            newsym(mon->mx, mon->my);
            if (vis)
                pline("%s %s %s %s fire trap!", Mnam,
                      vtense(Mnam, locomotion(mon->data, "move")),
                      is_floater(mon->data) ? "over" : "onto",
                      trap->tseen ? "the" : "a");
        }
        /* hack to avoid mintrap()'s chance of avoiding known trap */
        mon->mtrapseen &= ~(1 << (FIRE_TRAP - 1));
        mintrap(mon);
    } 
    else if (otyp == STRANGE_OBJECT) 
    {
        /* monster is using fire breath on self */
        if (vis)
            pline("%s breathes fire on %sself.", Monnam(mon), mhim(mon));
        if (!rn2(3))
            mon->mspec_used = rn1(10, 5);
        /* -21 => monster's fire breath; 1 => # of damage dice */
        damage = zhitm(mon, by_you ? 21 : -21, (struct obj*)0, mon, 1, 8, 0, &odummyp, &zhitm_out_flags);
    }
    else if (otyp == SCR_FIRE) 
    {
        mreadmsg(mon, obj);
        if (is_confused(mon))
        {
            if (cansee(mon->mx, mon->my))
                pline("Oh, what a pretty fire!");
            if (vis && !objects[otyp].oc_name_known
                && !objects[otyp].oc_uname)
                docall(obj, (char*)0);
            m_useup(mon, obj); /* after docall() */
            vis = FALSE;       /* skip makeknown() below */
            res = FALSE;       /* failed to cure sliming */
        } 
        else 
        {
            m_useup(mon, obj); /* before explode() */
            //int dmg = (4 * bcsign(obj) + 2) / 3 + 1;
            /* -11 => monster's fireball */
            explode(mon->mx, mon->my, -11, mon, 1, 2, (4 * bcsign(obj) + 2) / 3, otyp, SCROLL_CLASS,
                    /* by_you: override -11 for mon but not others */
                    by_you ? -EXPL_FIERY : EXPL_FIERY);
            damage = 0; /* damage has been applied by explode() */
        }
    }
    else
    { /* wand/horn of fire w/ positive charge count */
        mzapmsg(mon, obj, TRUE);
        obj->charges--;
        /* -1 => monster's wand of fire; 2 => # of damage dice */
        damage = zhitm(mon, by_you ? 1 : -1, (struct obj*)0, mon, 2, 8, 0, &odummyp, &zhitm_out_flags);
    }

    if (damage > 0) 
    {
        /* zhitm() applies damage but doesn't kill creature off;
           for fire breath, dmg is going to be 0 (fire breathers are
           immune to fire damage) but for wand of fire or fire horn,
           'mon' could have taken damage so might die */
        if (DEADMONSTER(mon))
        {
            if (by_you)
            {
                /* mon killed self but hero gets credit and blame (except
                   for pacifist conduct); xkilled()'s message would say
                   "You killed/destroyed <mon>" so give our own message */
                if (vis)
                    pline("%s is %s by the fire!", Monnam(mon),
                          is_not_living(mon->data) ? "destroyed" : "killed");
                xkilled(mon, XKILL_NOMSG | XKILL_NOCONDUCT);
            } else
                monkilled(mon, "fire", AD_FIRE, 0);
        } 
        else 
        {
            /* non-fatal damage occurred */
            if (vis)
                pline("%s is burned%s", Monnam(mon), exclam((int)damage));
        }
    }
    if (vis) 
    {
        if (res && !DEADMONSTER(mon))
            pline("%s slime is burned away!", s_suffix(Monnam(mon)));
        if (otyp != STRANGE_OBJECT)
            makeknown(otyp);
    }
    
    mon->mprops[SLIMED] &= ~M_TIMEOUT;
    /* use up monster's next move */
    mon->movement -= NORMAL_SPEED;
    mon->mlstmv = monstermoves;
    return res;
}

/* decide whether obj can be used to cure green slime */
STATIC_OVL int
cures_sliming(mon, obj)
struct monst *mon;
struct obj *obj;
{
    /* scroll of fire, non-empty wand or horn of fire */
    if (obj->otyp == SCR_FIRE)
        return (haseyes(mon->data) && !is_blinded(mon));
    /* hero doesn't need hands or even limbs to zap, so mon doesn't either */
    return ((obj->otyp == WAN_FIRE
             || (obj->otyp == FIRE_HORN && can_blow(mon)))
            && obj->charges > 0);
}

/* TRUE if monster appears to be green; for active TEXTCOLOR, we go by
   the display color, otherwise we just pick things that seem plausibly
   green (which doesn't necessarily match the TEXTCOLOR categorization) */
STATIC_OVL boolean
green_mon(mon)
struct monst *mon;
{
    struct permonst *ptr = mon->data;

    if (Hallucination)
        return FALSE;
#ifdef TEXTCOLOR
    if (iflags.use_color)
        return (ptr->mcolor == CLR_GREEN || ptr->mcolor == CLR_BRIGHT_GREEN);
#endif
    /* approximation */
    if (strstri(ptr->mname, "green"))
        return TRUE;
    switch (mon->mnum) {
    case PM_FOREST_CENTAUR:
    case PM_GARTER_SNAKE:
    case PM_GECKO:
    case PM_GREMLIN:
    case PM_HOMUNCULUS:
    case PM_JUBILEX:
    case PM_LEPRECHAUN:
    case PM_LICHEN:
    case PM_TREANT:
    case PM_LIZARD:
    case PM_WOOD_NYMPH:
        return TRUE;
    default:
        if (is_elf(ptr) && !is_prince(ptr) && !is_lord(ptr)
            && ptr != &mons[PM_GREY_ELF])
            return TRUE;
        break;
    }
    return FALSE;
}

void
mmake_wish(mon)
struct monst* mon;
{
    struct obj* otmp;
    char* oname;
    boolean wearable = FALSE;
    otmp = NULL;

    switch (rnd(18)) {
    case 1:
        otmp = mksobj(POT_GAIN_LEVEL, FALSE, FALSE, MKOBJ_TYPE_WISHING);
        if (otmp)
        {
            bless(otmp);
            otmp->quan = rnd(3);
            otmp->owt = weight(otmp);
        }
        break;
    case 2:
        otmp = mksobj(WAN_CREATE_MONSTER, TRUE, FALSE, MKOBJ_TYPE_WISHING);
        break;
    case 3:
        otmp = mksobj(AMULET_OF_LIFE_SAVING, FALSE, FALSE, MKOBJ_TYPE_WISHING);
        if (otmp)
        {
            bless(otmp);
            otmp->oerodeproof = 1;
            wearable = TRUE;
        }
        break;
    case 4:
        if (!mon->mprops[DISPLACED] && !cantweararm(mon->data)) {
            otmp = mksobj(CLOAK_OF_DISPLACEMENT, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(WAN_DIGGING, TRUE, FALSE, MKOBJ_TYPE_WISHING);
        }
        break;
    case 5:
        if (!nohands(mon->data)) {
            otmp = mksobj(GAUNTLETS_OF_DEXTERITY, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(POT_FULL_HEALING, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if(otmp)
                bless(otmp);
        }
        break;
    case 6:
        if (!resists_magic(mon)) {
            if (!cantweararm(mon->data))
            {
                otmp = mksobj(CLOAK_OF_MAGIC_RESISTANCE, FALSE, FALSE, MKOBJ_TYPE_WISHING);
                if (otmp)
                {
                    otmp->oerodeproof = 1;
                    otmp->enchantment = rn2(3) + 1;
                }
            }
            else
                otmp = mksobj(IOUN_STONE_OF_MAGIC_RESISTANCE, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(WAN_CREATE_MONSTER, TRUE, FALSE, MKOBJ_TYPE_WISHING);
        }
        break;
    case 7:
        if (!cantweararm(mon->data)) {
            otmp = mksobj(PLATE_MAIL, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->material = MAT_CRYSTAL;
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                otmp->owt = weight(otmp);
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(BAG_OF_HOLDING, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
            }
        }
        break;
    case 8:
        otmp = mksobj(POT_PARALYSIS, FALSE, FALSE, MKOBJ_TYPE_WISHING);
        if (otmp)
        {
            bless(otmp);
            otmp->quan = rnd(3);
            otmp->owt = weight(otmp);
        }
        break;
    case 9:
        if (!mon->mprops[WATER_WALKING] && !sliparm(mon->data)) {
            otmp = mksobj(WATER_WALKING_BOOTS, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(RIN_SLOW_DIGESTION, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if(otmp)
                bless(otmp);
        }
        break;
    case 10:
        if (!mon_reflects(mon, (char*)0)) {
            if (humanoid(mon->data))
            {
                otmp = mksobj(SHIELD_OF_REFLECTION, FALSE, FALSE, MKOBJ_TYPE_WISHING);
                if (otmp)
                {
                    otmp->oerodeproof = 1;
                    otmp->enchantment = rn2(3) + 1;
                }
            }
            else
                otmp = mksobj(AMULET_OF_REFLECTION, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(POT_EXTRA_HEALING, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if(otmp)
                bless(otmp);
        }
        break;
    case 11:
        if (!nohands(mon->data) && !strongmonst(mon->data)) {
            otmp = mksobj(GAUNTLETS_OF_OGRE_POWER, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(POT_GAIN_LEVEL, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if(otmp)
                curse(otmp);
        }
        break;
    case 12:
        if (!is_fast(mon) && !is_very_fast(mon) && !is_ultra_fast(mon) && !is_super_fast(mon) && !is_lightning_fast(mon) && mon_can_wear_boots_itself(mon)) {
            otmp = mksobj(SPEED_BOOTS, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(POT_FULL_HEALING, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if(otmp)
                bless(otmp);
        }
        break;
    case 13:
        if (!has_telepathy(mon)
            && !sliparm(mon->data) && !has_horns(mon->data) && has_head(mon->data)) {
            otmp = mksobj(HELM_OF_TELEPATHY, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(POT_EXTRA_HEALING, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if(otmp)
                bless(otmp);
        }
        break;
    case 14:
        if (!sliparm(mon->data) && !has_horns(mon->data) && has_head(mon->data)) {
            otmp = mksobj(HELM_OF_BRILLIANCE, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(UNICORN_HORN, TRUE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
            }
        }
        break;
    case 15:
        if (!cantweararm(mon->data)) {
            otmp = mksobj(CLOAK_OF_PROTECTION, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
        }
        else {
            otmp = mksobj(RIN_PROTECTION, FALSE, FALSE, MKOBJ_TYPE_WISHING);
            if (otmp)
            {
                bless(otmp);
                otmp->enchantment = rn2(3) + 1;
            }
        }
        break;
    case 16: /* Monsters can wish for certain artifacts */
        otmp = mk_artifact((struct obj*)0, mon_aligntyp(mon), MKARTIFACT_FLAGS_NONE);
        if (otmp) {
            bless(otmp);
            if (is_weapon(otmp) || is_armor(otmp))
            {
                otmp->oerodeproof = 1;
                otmp->enchantment = rn2(3) + 1;
                wearable = TRUE;
            }
            else if (otmp->oclass == RING_CLASS)
                wearable = TRUE;
        }
        break;
    case 17:
        switch (rnd(3)) {
        case 1:
            otmp = mksobj(WAN_PETRIFICATION, TRUE, FALSE, MKOBJ_TYPE_WISHING);
            break;
        case 2:
            otmp = mksobj(WAN_DISINTEGRATION, TRUE, FALSE, MKOBJ_TYPE_WISHING);
            break;
        case 3:
            otmp = mksobj(WAN_DEATH, TRUE, FALSE, MKOBJ_TYPE_WISHING);
            break;
        }
        break;
    case 18:
        otmp = mksobj(EGG, FALSE, FALSE, MKOBJ_TYPE_WISHING);
        if (otmp)
        {
            otmp->corpsenm = PM_COCKATRICE;
            otmp->quan = rnd(3);
            otmp->owt = weight(otmp);
        }
        break;
    default:
        otmp = mksobj(POT_GAIN_LEVEL, FALSE, FALSE, MKOBJ_TYPE_WISHING);
        if(otmp)
            curse(otmp);
    }

    if (otmp == NULL) {
        if (canseemon(mon))
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "For a moment, %s had %s in its %s, but it disappears.",
                mon_nam(mon), something, makeplural(mbodypart(mon, HAND)));
        return;
    }

    oname = ansimpleoname(otmp);
    if (canseemon(mon))
    {
        play_sfx_sound_at_location(SFX_WISH_FOR_OBJECT, mon->mx, mon->my);
        int multicolors[3] = { NO_COLOR, CLR_BRIGHT_MAGENTA, CLR_BRIGHT_MAGENTA };
        pline_multi_ex(ATR_NONE, CLR_MSG_SPELL, no_multiattrs, multicolors, "%s makes a wish for%s %s!",
            Monnam(mon), (otmp->quan > 1) ? " some" : "", oname);
    }

    if (mpickobj(mon, otmp))
        otmp = NULL;

    if (wearable)
        check_mon_wearable_items_next_turn(mon);
}

/*muse.c*/

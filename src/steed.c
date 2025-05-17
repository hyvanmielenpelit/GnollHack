/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    steed.c    $NHDT-Date: 1545441042 2018/12/22 01:10:42 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.62 $ */
/* Copyright (c) Kevin Hugo, 1998-1999. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL boolean FDECL(landing_spot, (coord *, int, int));
STATIC_DCL void FDECL(maybewakesteed, (struct monst *));

/* caller has decided that hero can't reach something while mounted */
void
rider_cant_reach()
{
    play_sfx_sound(SFX_GENERAL_NOT_SKILLED_ENOUGH);
    You_ex(ATR_NONE, CLR_MSG_FAIL, "aren't skilled enough to reach from %s.", y_monnam(u.usteed));
}

/*** Putting the saddle on ***/

/* Can this monster wear a saddle? */
boolean
can_saddle(mtmp)
struct monst *mtmp;
{
    if (!mtmp)
        return FALSE;

    return can_wear_saddle(mtmp->data);
}

int
use_saddle(otmp)
struct obj *otmp;
{
    struct monst *mtmp;
    struct permonst *ptr;
    int chance;
    const char *s;

    if (!u_handsy() && !is_telekinetic_operator(youmonst.data))
        return 0;

    /* Select an animal */
    if (u.uswallow || Underwater || !getdir((char *) 0)) {
        pline1(Never_mind);
        return 0;
    }
    if (!u.dx && !u.dy) {
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        pline_ex(ATR_NONE, NO_COLOR, "Saddle yourself?  Very funny...");
        return 0;
    }

    update_u_facing(TRUE);

    if (!isok(u.ux + u.dx, u.uy + u.dy)
        || !(mtmp = m_at(u.ux + u.dx, u.uy + u.dy)) || !canspotmon(mtmp)) {
        play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "I see nobody there.");
        return 1;
    }

    /* Is this a valid monster? */
    if (mtmp->worn_item_flags & W_SADDLE || which_armor(mtmp, W_SADDLE)) {
        play_sfx_sound(SFX_GENERAL_ALREADY_DONE);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s doesn't need another one.", Monnam(mtmp));
        return 1;
    }
    ptr = mtmp->data;
    if (touch_petrifies(ptr) && !uarmg && !Stone_resistance) {
        char kbuf[BUFSZ];

        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "touch %s.", mon_nam(mtmp));
        if (!(poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM))) {
            Sprintf(kbuf, "attempting to saddle %s", an(mon_monster_name(mtmp)));
            killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
            instapetrify(kbuf);
        }
    }
    if (ptr == &mons[PM_INCUBUS]) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "Shame on you!");
        exercise(A_WIS, FALSE);
        return 1;
    }
    if (mtmp->isminion || mtmp->isshk || mtmp->ispriest || mtmp->isgd
        || mtmp->iswiz) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "I think %s would mind.", mon_nam(mtmp));
        return 1;
    }
    if (!can_saddle(mtmp)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "saddle such a creature.");
        return 1;
    }

    /* Calculate your chance */
    chance = ACURR(A_DEX) + ACURR(A_CHA) / 2 + 2 * mtmp->mtame;
    chance += u.ulevel * (is_tame(mtmp) ? 20 : 5);
    if (!is_tame(mtmp))
        chance -= 10 * mtmp->m_lev;
    chance += riding_skill_saddling_bonus(P_SKILL_LEVEL(P_RIDING));

    if (Confusion || Fumbling || Glib)
        chance -= 20;
    else if (uarmg && (s = OBJ_DESCR(objects[uarmg->otyp])) != (char *) 0
             && !strncmp(s, "riding ", 7))
        /* Bonus for wearing "riding" (but not fumbling) gloves */
        chance += 10;
    else if (uarmf && (s = OBJ_DESCR(objects[uarmf->otyp])) != (char *) 0
             && !strncmp(s, "riding ", 7))
        /* ... or for "riding boots" */
        chance += 10;
    if (otmp->cursed)
        chance -= 50;

    /* [intended] steed becomes alert if possible */
    maybewakesteed(mtmp);

    /* Make the attempt */
    if (rn2(100) < chance) {
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_APPLY);
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "put the saddle on %s.", mon_nam(mtmp));
        if (otmp->owornmask)
            remove_worn_item(otmp, FALSE);
        freeinv(otmp);
        put_saddle_on_mon(otmp, mtmp);
    }
    else
    {
        play_sfx_sound_at_location(SFX_STEED_REFUSES, mtmp->mx, mtmp->my);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s resists!", Monnam(mtmp));
    }
    return 1;
}

void
put_saddle_on_mon(saddle, mtmp)
struct obj *saddle;
struct monst *mtmp;
{
    if (!can_saddle(mtmp) || which_armor(mtmp, W_SADDLE))
        return;
    if (mpickobj(mtmp, saddle))
        panic("merged saddle?");
    mtmp->worn_item_flags |= W_SADDLE;
    saddle->owornmask = W_SADDLE;
    saddle->leashmon = mtmp->m_id;
    update_all_mon_statistics(mtmp, FALSE);
}

/*** Riding the monster ***/

/* Can we ride this monster?  Caller should also check can_saddle() */
boolean
can_ride(mtmp)
struct monst *mtmp;
{
    return (is_tame(mtmp) && humanoid(youmonst.data)
            && !verysmall(youmonst.data) && !bigmonst(youmonst.data)
            && (!Underwater || is_swimmer(mtmp->data)));
}

int
doride(VOID_ARGS)
{
    if (u.usteed) 
    {
        dismount_steed(DISMOUNT_BYCHOICE);
    } 
    else
    {
        if (getdir((char*)0) && isok(u.ux + u.dx, u.uy + u.dy))
        {
            update_u_facing(TRUE);
            return (int)mount_steed(m_at(u.ux + u.dx, u.uy + u.dy));
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int
doridenearby(VOID_ARGS)
{
    if (u.usteed)
    {
        dismount_steed(DISMOUNT_BYCHOICE);
    }
    else
    {
        int x, y;
        int numsteeds = 0;
        struct monst* steedmtmp = 0;
        if (!Hallucination)
        {
            struct monst* mtmp = 0;
            for (x = u.ux - 1; x <= u.ux + 1; x++)
            {
                for (y = u.uy - 1; y <= u.uy + 1; y++)
                    if (!(x == u.ux && y == u.uy) && isok(x, y))
                    {
                        mtmp = m_at(x, y);
                        if (mtmp && !DEADMONSTER(mtmp) && is_tame(mtmp) && is_steed(mtmp->data) && canspotmon(mtmp) && which_armor(mtmp, W_SADDLE))
                        {
                            steedmtmp = mtmp;
                            numsteeds++;
                        }
                    }
            }
        }

        if (!steedmtmp || numsteeds != 1)
            return doride();
        else if (steedmtmp)
        {
            u.dx = steedmtmp->mx - u.ux;
            u.dy = steedmtmp->my - u.uy;
            update_u_facing(TRUE);
            return mount_steed(steedmtmp);
        }
        else
            return 0;
    }
    return 1;
}

/* Start riding, with the given monster */
boolean
mount_steed(mtmp)
struct monst *mtmp; /* The animal */
{
    struct obj *otmp;
    char buf[BUFSZ];
    struct permonst *ptr;

    /* Sanity checks */
    if (u.usteed)
    {
        play_sfx_sound(SFX_GENERAL_ALREADY_DONE);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are already riding %s.", mon_nam(u.usteed));
        return (FALSE);
    }

    /* Can the player reach and see the monster? */
    boolean force = FALSE;      /* Quietly force this animal */
    boolean forceasked = FALSE;      /* Quietly force this animal */
    boolean steedunseen = mtmp && ((Blind && !(Blind_telepat || Unblind_telepat || Detect_monsters)) || mtmp->mundetected
        || M_AP_TYPE(mtmp) == M_AP_FURNITURE
        || M_AP_TYPE(mtmp) == M_AP_OBJECT);

    /* We have two questions to avoid having two plines for not seeing anything */
    if (wizard && steedunseen)
    {
        forceasked = TRUE;
        if (yn_query("Force the mount to succeed?") == 'y')
            force = TRUE;
    }

    if (!mtmp || steedunseen)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT_SEE_SPOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "I see nobody there.");
        return (FALSE);
    }

    if (wizard && !forceasked)
    {
        forceasked = TRUE;
        if (yn_query("Force the mount to succeed?") == 'y')
            force = TRUE;
    }

    /* Is the player in the right form? */
    if (Hallucination && !force) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Maybe you should find a designated driver.");
        return (FALSE);
    }
    /* While riding Wounded_legs refers to the steed's,
     * not the hero's legs.
     * That opens up a potential abuse where the player
     * can mount a steed, then dismount immediately to
     * heal leg damage, because leg damage is always
     * healed upon dismount (Wounded_legs context switch).
     * By preventing a hero with Wounded_legs from
     * mounting a steed, the potential for abuse is
     * reduced.  However, dismounting still immediately
     * heals the steed's wounded legs.  [In 3.4.3 and
     * earlier, that unintentionally made the hero's
     * temporary 1 point Dex loss become permanent.]
     */
    if (Wounded_legs)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s are in no shape for riding.", makeplural(body_part(LEG)));
        if (force && wizard && yn_query("Heal your legs?") == 'y')
            HWounded_legs = EWounded_legs = 0L;
        else
            return (FALSE);
    }

    if (!force && (near_capacity() > SLT_ENCUMBER))
    {
        play_sfx_sound(SFX_GENERAL_TOO_MUCH_ENCUMBRANCE);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "do that while carrying so much stuff.");
        return (FALSE);
    }

    if (is_long_worm_with_tail(mtmp->data)
        && (u.ux + u.dx != mtmp->mx || u.uy + u.dy != mtmp->my))
    {
        /* 3.6.2:  test_move(below) is used to check for trying to mount
           diagonally into or out of a doorway or through a tight squeeze;
           attempting to mount a tail segment when hero was not adjacent
           to worm's head could trigger an impossible() in worm_cross()
           called from test_move(), so handle not-on-head before that */
        play_sfx_sound(SFX_GENERAL_NOT_A_GOOD_IDEA);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "couldn't ride %s, let alone its tail.", a_monnam(mtmp));
        return FALSE;
    }

    if (u.uswallow || u.ustuck || u.utrap || Punished
        || !test_move(u.ux, u.uy, mtmp->mx - u.ux, mtmp->my - u.uy,
                      TEST_MOVE)) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        if (Punished || !(u.uswallow || u.ustuck || u.utrap))
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are unable to swing your %s over.", body_part(LEG));
        else
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are stuck here for now.");
        return (FALSE);
    }

    /* Is this a valid monster? */
    otmp = which_armor(mtmp, W_SADDLE);
    if (!otmp)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is not saddled.", Monnam(mtmp));
        return (FALSE);
    }

    /* Steed has saddle, check if you fit */
    if (Upolyd && (!humanoid(youmonst.data) || verysmall(youmonst.data)
        || bigmonst(youmonst.data) || slithy(youmonst.data)))
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "won't fit on a saddle.");
        return (FALSE);
    }

    ptr = mtmp->data;
    if (touch_petrifies(ptr) && !Stone_resistance)
    {
        char kbuf[BUFSZ];

        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "touch %s.", mon_nam(mtmp));
        Sprintf(kbuf, "attempting to ride %s", an(mon_monster_name(mtmp)));
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
        instapetrify(kbuf);
    }
    if (!is_tame(mtmp) || mtmp->isminion)
    {
        play_sfx_sound(SFX_MONSTER_DOES_NOT_ALLOW);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "I think %s would mind.", mon_nam(mtmp));
        return (FALSE);
    }
    if (mtmp->mtrapped)
    {
        struct trap *t = t_at(mtmp->mx, mtmp->my);

        if (t)
        {
            play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "mount %s while %s's trapped in %s.", mon_nam(mtmp),
                mhe(mtmp), an(get_trap_explanation(t)));
        }

        return (FALSE);
    }

    /* Reduce tameness */
    if (!force && mtmp->mtame > 0 && !mindless(mtmp->data) && !mtmp->isfaithful &&
        P_SKILL_LEVEL(P_RIDING) < P_EXPERT &&
        !rn2(
            P_SKILL_LEVEL(P_RIDING) < P_BASIC ? 10 : 
            P_SKILL_LEVEL(P_RIDING) == P_BASIC ? 20 : 50 /* P_SKILLED */
        )) /* must be tame at this point*/
        mtmp->mtame--; /* reduce tameness if not knight */

    if (!force && !is_tame(mtmp)) 
    {
        /* no longer tame */
        newsym(mtmp->mx, mtmp->my);
        play_sfx_sound_at_location(SFX_STEED_REFUSES, mtmp->mx, mtmp->my);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s resists%s!", Monnam(mtmp),
              mtmp->mleashed ? " and its leash comes off" : "");
        if (mtmp->mleashed)
            m_unleash(mtmp, FALSE);
        return (FALSE);
    }
    if (!force && Underwater && !is_swimmer(ptr))
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "ride that creature while under %s.",
                 hliquid("water"));
        return (FALSE);
    }
    if (!can_saddle(mtmp) || !can_ride(mtmp))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "ride such a creature.");
        return FALSE;
    }

    /* Is the player impaired? */
    if (!force && !(is_flying(mtmp) || is_levitating(mtmp)) && Levitation
        && !Lev_at_will)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot reach %s.", mon_nam(mtmp));
        return (FALSE);
    }
    if (!force && uarm && is_metallic(uarm) && greatest_erosion(uarm)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s armor is too stiff to be able to mount %s.",
             uarm->oeroded ? "rusty" : "corroded", mon_nam(mtmp));
        return (FALSE);
    }

    int chance = riding_skill_mount_bonus(P_SKILL_LEVEL(P_RIDING)) + mtmp->mtame * 5;
    if (otmp->cursed)
        chance -= 25;
    if(Wounded_legs)
        chance -= 50;
    if (Glib)
        chance -= 50;
    if (Confusion)
        chance -= 50;
    if (Fumbling)
        chance -= 100;

    if (!force && rn2(100) >= chance)
    {
        if (Levitation) {
            play_sfx_sound(SFX_MOUNT_FAIL_SLIP_AWAY);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s slips away from you.", Monnam(mtmp));
            return FALSE;
        }
        play_sfx_sound(SFX_MOUNT_FAIL_AND_SLIP);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "slip while trying to get on %s.", mon_nam(mtmp));
        if (iflags.using_gui_sounds)
        {
            delay_output_milliseconds(300);
            play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
        }

        if (!rn2(2) || ((Upolyd ? u.mh : u.uhp) <= 3))
        {
            if (has_head(youmonst.data))
            {
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "hit your %s.", body_part(HEAD));
                if (!Stun_resistance)
                    make_stunned(rnd(3) + 1, TRUE);
            }
            else
            {
                nomul(-rnd(3));
            }
        }
        else
        {
            if (has_head(youmonst.data))
            {
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "hit your %s badly.", body_part(HEAD));
                if(!Stun_resistance)
                    make_stunned(rnd(5) + 1, TRUE);
            }
            else
            {
                nomul(-rnd(5));
            }

            Sprintf(buf, "slipped while mounting %s",
                /* "a saddled mumak" or "a saddled pony called Dobbin" */
                x_monnam(mtmp, ARTICLE_A, (char*)0,
                    SUPPRESS_IT | SUPPRESS_INVISIBLE
                    | SUPPRESS_HALLUCINATION,
                    TRUE));

            int dmg = rnd(3);
            losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), buf, NO_KILLER_PREFIX);
        }
        return (FALSE);
    }

    /* Success */
    maybewakesteed(mtmp);
    if (!force)
    {
        if (Levitation && !(is_flying(mtmp) || is_levitating(mtmp)))
            /* Must have Lev_at_will at this point */
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s magically floats up!", Monnam(mtmp));
        play_sfx_sound(SFX_MOUNT_SUCCESS);
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "mount %s.", mon_nam(mtmp));
        if (Flying)
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "and %s take flight together.", mon_nam(mtmp));
    }
    else
    {
        play_sfx_sound(SFX_MOUNT_SUCCESS);
    }
    /* setuwep handles polearms differently when you're mounted */
    update_unweapon();
    /*
    if (uwep && is_pole(uwep))
        unweapon = FALSE;
    */
    u.usteed = mtmp;
    remove_monster(mtmp->mx, mtmp->my);
    teleds(mtmp->mx, mtmp->my, TRUE, FALSE);
    context.botl = context.botlx = TRUE;
    return TRUE;
}

/* You and your steed have moved */
void
exercise_steed()
{
    if (!u.usteed)
        return;

    /* It takes many turns of riding to exercise skill */
    if (++u.urideturns >= RIDING_TURNS_TO_EXERCISE_SKILL) {
        u.urideturns = 0;
        use_skill(P_RIDING, 1);
    }
    return;
}

/* The player kicks or whips the steed */
void
kick_steed()
{
    char He[4];
    if (!u.usteed)
        return;

    /* [ALI] Various effects of kicking sleeping/paralyzed steeds */
    if (!mon_can_move(u.usteed)) {
        /* We assume a message has just been output of the form
         * "You kick <steed>."
         */
        Strcpy(He, mhe(u.usteed));
        *He = highc(*He);
        if ((u.usteed->mcanmove || u.usteed->mfrozen) && !rn2(2)) {
            if (u.usteed->mcanmove)
                u.usteed->msleeping = 0;
            else if (u.usteed->mfrozen > 2)
                u.usteed->mfrozen -= 2;
            else {
                u.usteed->mfrozen = 0;
                u.usteed->mcanmove = 1;
            }
            if (!mon_can_move(u.usteed))
                pline("%s stirs.", He);
            else
                pline("%s rouses %sself!", He, mhim(u.usteed));

            refresh_m_tile_gui_info(u.usteed, TRUE);
        } else
            pline("%s does not respond.", He);
        return;
    }

    /* Make the steed less tame and check if it resists */
    if (u.usteed->mtame && !mindless(u.usteed->data))
        u.usteed->mtame--;
    if (!u.usteed->mtame && u.usteed->mleashed)
        m_unleash(u.usteed, TRUE);
    if (!u.usteed->mtame
        || (u.ulevel + u.usteed->mtame < rnd(MAXULEV / 2 + 5))) {
        newsym(u.usteed->mx, u.usteed->my);
        dismount_steed(DISMOUNT_THROWN);
        return;
    }

    pline("%s gallops!", Monnam(u.usteed));
    u.ugallop += rn1(20, 30);
    return;
}

/*
 * Try to find a dismount point adjacent to the steed's location.
 * If all else fails, try enexto().  Use enexto() as a last resort because
 * enexto() chooses its point randomly, possibly even outside the
 * room's walls, which is not what we want.
 * Adapted from mail daemon code.
 */
STATIC_OVL boolean
landing_spot(spot, reason, forceit)
coord *spot; /* landing position (we fill it in) */
int reason;
int forceit;
{
    int i = 0, x, y, distance, min_distance = -1;
    boolean found = FALSE;
    struct trap *t;

    /* avoid known traps (i == 0) and boulders, but allow them as a backup */
    if (reason != DISMOUNT_BYCHOICE || Stunned || Confusion || Fumbling)
        i = 1;
    for (; !found && i < 2; ++i) {
        for (x = u.ux - 1; x <= u.ux + 1; x++)
            for (y = u.uy - 1; y <= u.uy + 1; y++) {
                if (!isok(x, y) || (x == u.ux && y == u.uy))
                    continue;

                if (accessible(x, y) && !MON_AT(x, y)) {
                    distance = distu(x, y);
                    if (min_distance < 0 || distance < min_distance
                        || (distance == min_distance && rn2(2))) {
                        if (i > 0 || (((t = t_at(x, y)) == 0 || !t->tseen)
                                      && (!sobj_at(BOULDER, x, y)
                                          || throws_rocks(youmonst.data)))) {
                            spot->x = x;
                            spot->y = y;
                            min_distance = distance;
                            found = TRUE;
                        }
                    }
                }
            }
    }

    /* If we didn't find a good spot and forceit is on, try enexto(). */
    if (forceit && min_distance < 0
        && !enexto(spot, u.ux, u.uy, youmonst.data))
        return FALSE;

    return found;
}

/* Stop riding the current steed */
void
dismount_steed(reason)
int reason; /* Player was thrown off etc. */
{
    struct monst *mtmp;
    struct obj *otmp;
    coord cc, steedcc;
    const char *verb = "fall";
    boolean repair_leg_damage = (Wounded_legs != 0L);
    uint64_t save_utrap = u.utrap;
    boolean have_spot = landing_spot(&cc, reason, 0);

    reset_monster_origin_coordinates(&youmonst);

    mtmp = u.usteed; /* make a copy of steed pointer */
    /* Sanity check */
    if (!mtmp) /* Just return silently */
        return;

    /* Check the reason for dismounting */
    otmp = which_armor(mtmp, W_SADDLE);
    switch (reason) {
    case DISMOUNT_THROWN:
        verb = "are thrown";
        /*FALLTHRU*/
    case DISMOUNT_FELL:
        You("%s off of %s!", verb, mon_nam(mtmp));
        if (!have_spot)
            have_spot = landing_spot(&cc, reason, 1);
        losehp(adjust_damage(rn1(10, 10), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "riding accident", KILLED_BY_AN);
        set_wounded_legs(BOTH_SIDES, (int) HWounded_legs + rn1(5, 5));
        repair_leg_damage = FALSE;
        break;
    case DISMOUNT_POLY:
        You("can no longer ride %s.", mon_nam(u.usteed));
        if (!have_spot)
            have_spot = landing_spot(&cc, reason, 1);
        break;
    case DISMOUNT_ENGULFED:
        /* caller displays message */
        break;
    case DISMOUNT_BONES:
        /* hero has just died... */
        break;
    case DISMOUNT_GENERIC:
        /* no messages, just make it so */
        break;
    case DISMOUNT_BYCHOICE:
    default:
        if (otmp && otmp->cursed) {
            play_sfx_sound(SFX_GENERAL_WELDED);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "can't.  The saddle %s cursed.",
                otmp->bknown ? "is" : "seems to be");
            otmp->bknown = TRUE;
            return;
        }
        if (!have_spot) {
            play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "can't.  There isn't anywhere for you to stand.");
            return;
        }
        if (!has_mname(mtmp)) {
//            pline("You've been through the dungeon on %s with no name.",
//                  an(mtmp->data->mname));
            You("dismount %s.", mon_nam(mtmp));
            if (Hallucination)
                pline("It felt good to get out of the rain.");
        } else
            You("dismount %s.", mon_nam(mtmp));
    }
    /* While riding, Wounded_legs refers to the steed's legs;
       after dismounting, it reverts to the hero's legs. */
    if (repair_leg_damage)
        heal_legs(1);

    /* Release the steed and saddle */
    u.usteed = 0;
    u.ugallop = 0L;
    /*
     * rloc(), rloc_to(), and monkilled()->mondead()->m_detach() all
     * expect mtmp to be on the map or else have mtmp->mx be 0, but
     * setting the latter to 0 here would interfere with dropping
     * the saddle.  Prior to 3.6.2, being off the map didn't matter.
     *
     * place_monster() expects mtmp to be alive and not be u.usteed.
     *
     * Unfortunately, <u.ux,u.uy> (former steed's implicit location)
     * might now be occupied by an engulfer, so we can't just put mtmp
     * at that spot.  An engulfer's previous spot will be unoccupied
     * but we don't know where that was and even if we did, it might
     * be hostile terrain.
     */
    steedcc.x = u.ux, steedcc.y = u.uy;
    if (m_at(u.ux, u.uy)) {
        /* hero's spot has a monster in it; hero must have been plucked
           from saddle as engulfer moved into his spot--other dismounts
           shouldn't run into this situation; find nearest viable spot */
        if (!enexto(&steedcc, u.ux, u.uy, mtmp->data)
            /* no spot? must have been engulfed by a lurker-above over
               water or lava; try requesting a location for a flyer */
            && !enexto(&steedcc, u.ux, u.uy, &mons[PM_BAT]))
            /* still no spot; last resort is any spot within bounds */
            (void) enexto(&steedcc, u.ux, u.uy, &mons[PM_GHOST]);
    }
    if (!m_at(steedcc.x, steedcc.y)) {
        if (mtmp->mhp < 1)
            mtmp->mhp = 0; /* make sure it isn't negative */
        mtmp->mhp++; /* force at least one hit point, possibly resurrecting */
        place_monster(mtmp, steedcc.x, steedcc.y);
        mtmp->mhp--; /* take the extra hit point away: cancel resurrection */
    } else {
        impossible("Dismounting: can't place former steed on map.");
    }

    if (!DEADMONSTER(mtmp)) {
        /* if for bones, there's no reason to place the hero;
           we want to make room for potential ghost, so move steed */
        if (reason == DISMOUNT_BONES) {
            /* move the steed to an adjacent square */
            if (enexto(&cc, u.ux, u.uy, mtmp->data))
                rloc_to(mtmp, cc.x, cc.y);
            else /* evidently no room nearby; move steed elsewhere */
                (void) rloc(mtmp, FALSE);
            return;
        }

        /* Set hero's and/or steed's positions.  Try moving the hero first. */
        if (!u.uswallow && !u.ustuck && have_spot) {
            struct permonst *mdat = mtmp->data;

            /* The steed may drop into water/lava */
            if (!(is_flying(mtmp) || is_levitating(mtmp)) && !is_clinger(mdat)) {
                if (is_pool(u.ux, u.uy)) {
                    if (!Underwater)
                        pline("%s falls into the %s!", Monnam(mtmp),
                              surface(u.ux, u.uy));
                    if (!is_swimmer(mdat) && !amphibious(mdat)) {
                        killed(mtmp);
                        adjalign(-1);
                    }
                } else if (is_lava(u.ux, u.uy)) {
                    pline("%s is pulled into the %s!", Monnam(mtmp),
                          hliquid("lava"));
                    if (!likes_lava(mdat)) {
                        killed(mtmp);
                        adjalign(-1);
                    }
                }
            }
            /* Steed dismounting consists of two steps: being moved to another
             * square, and descending to the floor.  We have functions to do
             * each of these activities, but they're normally called
             * individually and include an attempt to look at or pick up the
             * objects on the floor:
             * teleds() --> spoteffects() --> pickup()
             * float_down() --> pickup()
             * We use this kludge to make sure there is only one such attempt.
             *
             * Clearly this is not the best way to do it.  A full fix would
             * involve having these functions not call pickup() at all,
             * instead
             * calling them first and calling pickup() afterwards.  But it
             * would take a lot of work to keep this change from having any
             * unforeseen side effects (for instance, you would no longer be
             * able to walk onto a square with a hole, and autopickup before
             * falling into the hole).
             */
            /* [ALI] No need to move the player if the steed died. */
            if (!DEADMONSTER(mtmp)) {
                /* Keep steed here, move the player to cc;
                 * teleds() clears u.utrap
                 */
                in_steed_dismounting = TRUE;
                teleds(cc.x, cc.y, TRUE, FALSE);
                in_steed_dismounting = FALSE;

                /* Put your steed in your trap */
                if (save_utrap)
                    (void) mintrap(mtmp);
            }

        /* Couldn't move hero... try moving the steed. */
        } else if (enexto(&cc, u.ux, u.uy, mtmp->data)) {
            /* Keep player here, move the steed to cc */
            rloc_to(mtmp, cc.x, cc.y);
            /* Player stays put */

        /* Otherwise, kill the steed. */
        } else {
            if (reason == DISMOUNT_BYCHOICE) {
                /* [un]#ride: hero gets credit/blame for killing steed */
                killed(mtmp);
                adjalign(-1);
            } else {
                /* other dismount: kill former steed with no penalty */
                monkilled(mtmp, "", AD_PHYS, 0);
            }
        }
    } /* !DEADMONST(mtmp) */

    /* usually return the hero to the surface */
    if (reason != DISMOUNT_ENGULFED && reason != DISMOUNT_BONES) {
        in_steed_dismounting = TRUE;
        (void) float_down(0L, W_SADDLE);
        in_steed_dismounting = FALSE;
        (void) encumber_msg();
        vision_full_recalc = 1;
    } 
    /* polearms behave differently when not mounted */
    update_unweapon();
    context.botl = context.botlx = TRUE;
    /*
    if (uwep && is_pole(uwep))
        unweapon = TRUE;
    */
    return;
}

/* when attempting to saddle or mount a sleeping steed, try to wake it up
   (for the saddling case, it won't be u.usteed yet) */
STATIC_OVL void
maybewakesteed(steed)
struct monst *steed;
{
    int frozen = (int) steed->mfrozen;
    boolean wasimmobile = !mon_can_move(steed);

    steed->msleeping = 0;
    if (frozen) {
        frozen = (frozen + 1) / 2; /* half */
        /* might break out of timed sleep or paralysis */
        if (!rn2(frozen)) {
            steed->mfrozen = 0;
            steed->mcanmove = 1;
        } else {
            /* didn't awake, but remaining duration is halved */
            steed->mfrozen = frozen;
        }
    }
    if (wasimmobile && mon_can_move(steed))
    {
        pline("%s wakes up.", Monnam(steed));
        refresh_m_tile_gui_info(steed, TRUE);
    }
    /* regardless of waking, terminate any meal in progress */
    finish_meating(steed);
}

/* decide whether hero's steed is able to move;
   doesn't check for holding traps--those affect the hero directly */
boolean
stucksteed(checkfeeding)
boolean checkfeeding;
{
    struct monst *steed = u.usteed;

    if (steed) {
        /* check whether steed can move */
        if (!mon_can_move(steed)) {
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s won't move!", upstart(y_monnam(steed)));
            return TRUE;
        }
        /* optionally check whether steed is in the midst of a meal */
        if (checkfeeding && steed->meating) {
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is still eating.", upstart(y_monnam(steed)));
            return TRUE;
        }
    }
    return FALSE;
}

void
place_monster(mon, x, y)
struct monst *mon;
int x, y;
{
    /* normal map bounds are <1..COLNO-1,0..ROWNO-1> but sometimes
       vault guards (either living or dead) are parked at <0,0> */
    if (!isok(x, y) && (x != 0 || y != 0 || !mon->isgd)) {
        impossible("trying to place monster at <%d,%d>", x, y);
        x = y = 0;
    }
    if (mon == u.usteed
        /* special case is for convoluted vault guard handling */
        || (DEADMONSTER(mon) && !(mon->isgd && x == 0 && y == 0))) {
        impossible("placing %s onto map?",
                   (mon == u.usteed) ? "steed" : "defunct monster");
        return;
    }
    if (level.monsters[x][y])
        impossible("placing monster over another at <%d,%d>?", x, y);
    mon->mx = x, mon->my = y;
    level.monsters[x][y] = mon;
}

/*steed.c*/

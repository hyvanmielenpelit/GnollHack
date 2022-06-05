/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    mhitm.c    $NHDT-Date: 1555720096 2019/04/20 00:28:16 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.113 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

extern boolean notonhead;

static NEARDATA boolean vis, far_noise;
static NEARDATA long noisetime;
static NEARDATA struct obj *otmp;

static const char brief_feeling[] =
    "have a %s feeling for a moment, then it passes.";

STATIC_DCL char *FDECL(mon_nam_too, (char *, struct monst *, struct monst *));
STATIC_DCL int FDECL(hitmm, (struct monst *, struct monst *, struct attack *, struct obj*));
STATIC_DCL int FDECL(gazemm, (struct monst *, struct monst *,
                              struct attack *));
STATIC_DCL int FDECL(gulpmm, (struct monst *, struct monst *,
                              struct attack *));
STATIC_DCL int FDECL(explmm, (struct monst *, struct monst *,
                              struct attack *));
STATIC_DCL int FDECL(mdamagem, (struct monst *, struct monst *, struct attack *, struct obj*));
STATIC_DCL void FDECL(mswingsm, (struct monst *, struct monst *,
                                 struct obj *));
STATIC_DCL void FDECL(noises, (struct monst *, struct attack *));
STATIC_DCL void FDECL(missmm, (struct monst *, struct monst *,
                               struct attack *));
STATIC_DCL int FDECL(passivemm, (struct monst *, struct monst *,
                                 BOOLEAN_P, int));

/* Needed for the special case of monsters wielding vorpal blades (rare).
 * If we use this a lot it should probably be a parameter to mdamagem()
 * instead of a global variable.
 */
static int dieroll;

/* returns mon_nam(mon) relative to other_mon; normal name unless they're
   the same, in which case the reference is to {him|her|it} self */
STATIC_OVL char *
mon_nam_too(outbuf, mon, other_mon)
char *outbuf;
struct monst *mon, *other_mon;
{
    if (mon != other_mon)
        Strcpy(outbuf, mon_nam(mon));
    else
        switch (pronoun_gender(mon, FALSE)) 
        {
        case 0:
            Strcpy(outbuf, "himself");
            break;
        case 1:
            Strcpy(outbuf, "herself");
            break;
        default:
            Strcpy(outbuf, "itself");
            break;
        }
    return outbuf;
}

STATIC_OVL void
noises(magr, mattk)
register struct monst *magr;
register struct attack *mattk;
{
    boolean farq = (distu(magr->mx, magr->my) > 15);

    if (!Deaf && (farq != far_noise || moves - noisetime > 10))
    {
        far_noise = farq;
        noisetime = moves;
        You_hear("%s%s.",
                 (mattk->aatyp == AT_EXPL) ? "an explosion" : "some noises",
                 farq ? " in the distance" : "");
    }
}

STATIC_OVL
void
missmm(magr, mdef, mattk)
register struct monst *magr, *mdef;
struct attack *mattk;
{
    const char *fmt;
    char buf[BUFSZ], mdef_name[BUFSZ];

    if (vis) 
    {
        if (!canspotmon(magr))
            map_invisible(magr->mx, magr->my);
        if (!canspotmon(mdef))
            map_invisible(mdef->mx, mdef->my);
        if (M_AP_TYPE(mdef))
            seemimic(mdef);
        if (M_AP_TYPE(magr))
            seemimic(magr);
        fmt = (could_seduce(magr, mdef, mattk) && !is_cancelled(magr))
                  ? "%s pretends to be friendly to"
                  : "%s misses";
        Sprintf(buf, fmt, Monnam(magr));
        pline("%s %s.", buf, mon_nam_too(mdef_name, mdef, magr));
    } 
    else
        noises(magr, mattk);
}

/*
 *  fightm()  -- fight some other monster
 *
 *  Returns:
 *      0 - Monster did nothing.
 *      1 - If the monster made an attack.  The monster might have died.
 *
 *  There is an exception to the above.  If mtmp has the hero swallowed,
 *  then we report that the monster did nothing so it will continue to
 *  digest the hero.
 */
 /* have monsters fight each other */
int
fightm(mtmp, attack_only_hostiles)
register struct monst *mtmp;
boolean attack_only_hostiles;
{
    register struct monst *mon, *nmon;
    int result, has_u_swallowed;
#ifdef LINT
    nmon = 0;
#endif
    /* perhaps the monster will resist Conflict */
    if (check_ability_resistance_success(mtmp, A_WIS, 0))
        return 0;

    if (u.ustuck == mtmp) 
    {
        /* perhaps we're holding it... */
        if (itsstuck(mtmp))
            return 0;
    }

    has_u_swallowed = (u.uswallow && (mtmp == u.ustuck));

    for (mon = fmon; mon; mon = nmon)
    {
        nmon = mon->nmon;
        if (nmon == mtmp)
            nmon = mtmp->nmon;

        if (attack_only_hostiles && (is_peaceful(mtmp) || is_tame(mtmp)))
            continue;

        /* Be careful to ignore monsters that are already dead, since we
         * might be calling this before we've cleaned them up.  This can
         * happen if the monster attacked a cockatrice bare-handedly, for
         * instance.
         */
        if (mon != mtmp && !DEADMONSTER(mon))
        {
            if (monnear(mtmp, mon->mx, mon->my)) 
            {
                if (!u.uswallow && (mtmp == u.ustuck)) 
                {
                    if (!rn2(4)) 
                    {
                        pline("%s releases you!", Monnam(mtmp));
                        u.ustuck = 0;
                    } 
                    else
                        break;
                }

                /* mtmp can be killed */
                bhitpos.x = mon->mx;
                bhitpos.y = mon->my;
                notonhead = 0;
                result = mattackm(mtmp, mon);

                if (result & MM_AGR_DIED)
                    return 1; /* mtmp died */
                /*
                 * If mtmp has the hero swallowed, lie and say there
                 * was no attack (this allows mtmp to digest the hero).
                 */
                if (has_u_swallowed)
                    return 0;

                /* Allow attacked monsters a chance to hit back. Primarily
                 * to allow monsters that resist conflict to respond.
                 */
                if ((result & MM_HIT) && !(result & MM_DEF_DIED) && rn2(4)
                    && mon->movement >= NORMAL_SPEED) 
                {
                    mon->movement -= NORMAL_SPEED;
                    bhitpos.x = mtmp->mx;
                    bhitpos.y = mtmp->my;
                    notonhead = 0;
                    (void) mattackm(mon, mtmp); /* return attack */
                }

                return (result & MM_HIT) ? 1 : 0;
            }
        }
    }
    return 0;
}

/*
 * mdisplacem() -- attacker moves defender out of the way;
 *                 returns same results as mattackm().
 */
int
mdisplacem(magr, mdef, quietly)
register struct monst *magr, *mdef;
boolean quietly;
{
    struct permonst *pa, *pd;
    int tx, ty, fx, fy;

    /* sanity checks; could matter if we unexpectedly get a long worm */
    if (!magr || !mdef || magr == mdef)
        return MM_MISS;

    pa = magr->data, pd = mdef->data;
    tx = mdef->mx, ty = mdef->my; /* destination */
    fx = magr->mx, fy = magr->my; /* current location */
    if (m_at(fx, fy) != magr || m_at(tx, ty) != mdef)
        return MM_MISS;

    /* The 1 in 7 failure below matches the chance in attack()
     * for pet displacement.
     */
    if (!rn2(7))
        return MM_MISS;

    /* Grid bugs cannot displace at an angle. */
    if (pa == &mons[PM_GRID_BUG] && magr->mx != mdef->mx
        && magr->my != mdef->my)
        return MM_MISS;

    update_m_facing(magr, mdef->mx - magr->mx, FALSE);

    /* undetected monster becomes un-hidden if it is displaced */
    if (mdef->mundetected)
        mdef->mundetected = 0;
    if (M_AP_TYPE(mdef) && M_AP_TYPE(mdef) != M_AP_MONSTER)
        seemimic(mdef);
    /* wake up the displaced defender */
    mdef->msleeping = 0;
    mdef->mstrategy &= ~STRAT_WAITMASK;
    finish_meating(mdef);

    /*
     * Set up the visibility of action.
     * You can observe monster displacement if you can see both of
     * the monsters involved.
     */
    vis = (canspotmon(magr) && canspotmon(mdef));

    if (touch_petrifies(pd) && !resists_ston(magr)) 
    {
        start_delayed_petrification(magr, FALSE);
#if 0
        if (which_armor(magr, W_ARMG) != 0) {
            if (poly_when_stoned(pa)) {
                mon_to_stone(magr);
                return MM_HIT; /* no damage during the polymorph */
            }
            if (!quietly && canspotmon(magr))
                pline("%s turns to stone!", Monnam(magr));
            monstone(magr);
            if (!DEADMONSTER(magr))
                return MM_HIT; /* lifesaved */
            else if (is_tame(magr) && !vis)
                You(brief_feeling, "peculiarly sad");
            return MM_AGR_DIED;
        }
#endif
    }

    remove_monster(fx, fy); /* pick up from orig position */
    remove_monster(tx, ty);
    place_monster(magr, tx, ty); /* put down at target spot */
    place_monster(mdef, fx, fy);
    if (vis && !quietly)
        pline("%s moves %s out of %s way!", Monnam(magr), mon_nam(mdef),
              is_rider(pa) ? "the" : mhis(magr));
    newsym(fx, fy);  /* see it */
    newsym(tx, ty);  /*   all happen */
    flush_screen(1); /* make sure it shows up */

    return MM_HIT;
}

/*
 * mattackm() -- a monster attacks another monster.
 *
 *          --------- aggressor died
 *         /  ------- defender died
 *        /  /  ----- defender was hit
 *       /  /  /
 *      x  x  x
 *
 *      0x4     MM_AGR_DIED
 *      0x2     MM_DEF_DIED
 *      0x1     MM_HIT
 *      0x0     MM_MISS
 *
 * Each successive attack has a lower probability of hitting.  Some rely on
 * success of previous attacks.  ** this doen't seem to be implemented -dl **
 *
 * In the case of exploding monsters, the monster dies as well.
 */
int
mattackm(magr, mdef)
register struct monst *magr, *mdef;
{
    int i,          /* loop counter */
        tmp,        /* amour class difference */
        strike = 0, /* hit this attack */
        attk,       /* attack attempted this time */
        struck = 0, /* hit at least once */
        res[NATTK]; /* results of all attacks */
    struct attack *mattk, alt_attk;
    struct permonst *pa, *pd;

    if (!magr || !mdef)
        return MM_MISS; /* mike@genat */
    if (!mon_can_move(magr))
        return MM_MISS;
    pa = magr->data;
    pd = mdef->data;

    /* Grid bugs cannot attack at an angle. */
    if (pa == &mons[PM_GRID_BUG] && magr->mx != mdef->mx
        && magr->my != mdef->my)
        return MM_MISS;

    reset_monster_origin_coordinates(&youmonst);

    /* Update facing */
    update_m_facing(magr, mdef->mx - magr->mx, TRUE);

    /* Calculate the armour class differential. */
    tmp = find_mac(mdef) + magr->m_lev + magr->mhitinc;
    if (is_confused(mdef) || !mon_can_move(mdef))
    {
        tmp += 4;
        mdef->msleeping = 0;
        refresh_m_tile_gui_info(mdef, FALSE);
    }

    /* undetect monsters become un-hidden if they are attacked */
    if (mdef->mundetected) 
    {
        mdef->mundetected = 0;
        newsym(mdef->mx, mdef->my);
        if (canseemon(mdef) && !sensemon(mdef))
        {
            if (Unaware) 
            {
                boolean justone = (mdef->data->geno & G_UNIQ) != 0L;
                const char *montype;

                montype = noname_monnam(mdef, justone ? ARTICLE_THE
                                                      : ARTICLE_NONE);
                if (!justone)
                    montype = makeplural(montype);
                You_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "dream of %s.", montype);
            } 
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly, you notice %s.", a_monnam(mdef));
        }
    }

    /* Elves hate orcs. */
    if (is_elf(pa) && is_orc(pd))
        tmp++;

    /* Set up the visibility of action */
    vis = (cansee(magr->mx, magr->my) && cansee(mdef->mx, mdef->my)
           && (canspotmon(magr) || canspotmon(mdef)));

    /* Set flag indicating monster has moved this turn.  Necessary since a
     * monster might get an attack out of sequence (i.e. before its move) in
     * some cases, in which case this still counts as its move for the round
     * and it shouldn't move again.
     */
    magr->mlstmv = monstermoves;

    int weaponattackcount = 0;

    int tmp2 = tmp;
    int bite_butt_count = 0;
    boolean first_attack = TRUE;
    enum action_tile_types action = 0;

    /* Now perform all attacks for the monster. */
    for (i = 0; i < NATTK; i++) 
    {
        tmp = tmp2; //Reset hitv to original start value
        res[i] = MM_MISS;
        mattk = getmattk(magr, mdef, i, res, &alt_attk);

        if (mattk->aatyp == AT_BITE || mattk->aatyp == AT_BUTT)
            bite_butt_count++;
        if (magr->data->heads > 1 && magr->heads_left < bite_butt_count)
            continue;

        otmp = (struct obj *) 0;
        attk = 1;
        int multistrike = 1;

        switch (mattk->aatyp)
        {
        case AT_WEAP: /* "hand to hand" attacks */
            if (distmin(magr->mx, magr->my, mdef->mx, mdef->my) > 1) 
            {
                /* D: Do a ranged attack here! */
                strike = thrwmm(magr, mdef);
                if (DEADMONSTER(mdef))
                    res[i] = MM_DEF_DIED;
                if (DEADMONSTER(magr))
                    res[i] |= MM_AGR_DIED;
                break;
            }
            if (magr->weapon_strategy == NEED_WEAPON || !MON_WEP(magr) || (is_launcher(MON_WEP(magr)) && !mwelded(MON_WEP(magr), magr)))
            {
                magr->weapon_strategy = NEED_HTH_WEAPON;
                if (mon_wield_item(magr, FALSE) != 0)
                    return 0;
            }
            possibly_unwield(magr, FALSE);
//            otmp = MON_WEP(magr);

            weaponattackcount++;
            if (is_multiweaponmonster(magr->data))
                otmp = select_multiweapon_nth_hwep(magr, weaponattackcount);
            else
                otmp = MON_WEP(magr);

            if (otmp) 
            {
                if (vis)
                    mswingsm(magr, mdef, otmp);
                tmp += weapon_to_hit_value(otmp, mdef, magr, 0);

            }

            multistrike = get_multishot_stats(magr, otmp, otmp, FALSE, (double*)0);

            /*FALLTHRU*/
        case AT_CLAW:
        case AT_HORN:
        case AT_TUSK:
        case AT_KICK:
        case AT_BITE:
        case AT_RAMS:
        case AT_TAIL:
        case AT_STNG:
        case AT_TUCH:
        case AT_BUTT:
        case AT_TENT:
            /* Nymph that teleported away on first attack? */
            if (distmin(magr->mx, magr->my, mdef->mx, mdef->my) > 1)
                /* Continue because the monster may have a ranged attack. */
                continue;

            /* Monsters won't attack cockatrices physically if they
             * have a weapon instead.  This instinct doesn't work for
             * players, or under conflict or confusion.
             */
            if (!is_confused(magr) && !Conflict && !is_crazed(magr) && otmp && mattk->aatyp != AT_WEAP
                && touch_petrifies(mdef->data)) 
            {
                strike = 0;
                break;
            }

            //Give strength and dexerity bonus to hit
            tmp += m_strdex_to_hit_bonus(magr);

            int mdef_x = mdef->mx;
            int mdef_y = mdef->my;

            if (first_attack)
            {
                play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                first_attack = FALSE;
            }

            action = mattk->action_tile ? mattk->action_tile : mattk->aatyp == AT_KICK ? ACTION_TILE_KICK : ACTION_TILE_ATTACK;
            update_m_action(magr, action);
            play_monster_simple_weapon_sound(magr, i, otmp, OBJECT_SOUND_TYPE_SWING_MELEE);
            m_wait_until_action();
            for (int strikeindex = 0; strikeindex < multistrike; strikeindex++)
            {
                boolean endforloop = FALSE;

                if (otmp)
                {
                    if (strikeindex == 0)
                        ; //Swinging message is already done above
                    else
                    {
                        if (flags.verbose && !Blind && vis)
                            pline("%s %s %s!", 
                                s_suffix(Monnam(magr)), 
                                aobjnam(otmp, "strike"), 
                                strikeindex == 1 ? "a second time" : strikeindex == 2 ? "a third time" : "once more");

                    }
                }

                //TO-HIT IS DETERMINED HERE
                dieroll = rnd(20 + i);
                strike = (tmp > dieroll);

                if (strike)
                {
                    struct obj* omonwep = otmp;
                    res[i] = hitmm(magr, mdef, mattk, otmp);
                    boolean uses_spell_flags = omonwep ? object_uses_spellbook_wand_flags_and_properties(omonwep) : FALSE;

                    /* Check if the weapon shatters */
                    /* Check if the object should shatter */

                    if (omonwep && omonwep->where == OBJ_MINVENT && is_fragile(omonwep)
                        && !is_obj_indestructible(omonwep)
                        && !is_quest_artifact(omonwep)
                        && !omonwep->oartifact
                        )
                    {
                        /* Shattering is done below, here just the message*/
                        boolean set_to_zero = FALSE;
                        if (omonwep->quan == 1)
                        {
                            set_to_zero = TRUE;
                            if(canseemon(magr))
                                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s shatters from the blow!", s_suffix(Monnam(magr)), xname(omonwep));
                        }
                        else
                        {
                            if (canseemon(magr))
                                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "One of %s %s shatters from the blow!", s_suffix(mon_nam(magr)), xname(omonwep));
                        }
                        m_useup(magr, omonwep);
                        if (set_to_zero)
                        {
                            omonwep = 0;
                            otmp = 0;
                            MON_WEP(magr) = 0;
                        }
                        endforloop = TRUE;
                    }
                    else if (omonwep && !uses_spell_flags && (objects[omonwep->otyp].oc_aflags & A1_ITEM_VANISHES_ON_HIT)
                        && (
                            !(objects[omonwep->otyp].oc_aflags & A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET)
                            || ((objects[omonwep->otyp].oc_aflags & A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET) && eligible_for_extra_damage(omonwep, mdef, magr))
                            )
                        )
                    {
                        if (omonwep->where == OBJ_MINVENT)
                            m_useup(magr, omonwep);
                        else if (omonwep->where == OBJ_FREE)
                            obfree(omonwep, (struct obj*)0);

                        omonwep = 0;
                        otmp = 0;
                        MON_WEP(magr) = 0;

                        endforloop = TRUE;
                    }
                    if (does_split_upon_hit(mdef->data)
                        && (otmp && (objects[otmp->otyp].oc_material == MAT_IRON
                            || objects[otmp->otyp].oc_material == MAT_METAL))
                        && mdef->mhp > 1
                        && !is_cancelled(mdef)) 
                    {
                        struct monst* mclone;
                        if ((mclone = clone_mon(mdef, 0, 0, TRUE)) != 0) 
                        {
                            if (vis && canspotmon(mdef)) 
                            {
                                char buf[BUFSZ];

                                Strcpy(buf, Monnam(mdef));
                                if (canseemon(mdef))
                                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s divides as %s hits it!", buf, mon_nam(magr));
                            }
                            mintrap(mclone);
                        }
                    }
                }
                else
                    missmm(magr, mdef, mattk);



                if(endforloop || DEADMONSTER(mdef) || DEADMONSTER(magr) || m_at(mdef_x, mdef_y) != mdef)
                    break;
            }
            update_m_action_revert(magr, ACTION_TILE_NO_ACTION);

            break;

        case AT_HUGS: /* automatic if prev two attacks succeed */
            if (first_attack)
            {
                play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                first_attack = FALSE;
            }

            action = mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK;
            update_m_action(magr, action);
            play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            m_wait_until_action();
            strike = (i >= 2 && res[i - 1] == MM_HIT && res[i - 2] == MM_HIT);
            if (strike)
                res[i] = hitmm(magr, mdef, mattk, (struct obj*)0);
            update_m_action_revert(magr, ACTION_TILE_NO_ACTION);

            break;

        case AT_GAZE:
            strike = 0;
            if (first_attack)
            {
                play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                first_attack = FALSE;
            }

            update_m_action(magr, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
            play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            m_wait_until_action();
            res[i] = gazemm(magr, mdef, mattk);
            update_m_action_revert(magr, ACTION_TILE_NO_ACTION);
            break;

        case AT_EXPL:
            /* D: Prevent explosions from a distance */
            if (distmin(magr->mx,magr->my,mdef->mx,mdef->my) > 1)
                continue;

            if (first_attack)
            {
                play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                first_attack = FALSE;
            }

            res[i] = explmm(magr, mdef, mattk);
            if (res[i] == MM_MISS) 
            { /* cancelled--no attack */
                strike = 0;
                attk = 0;
            } 
            else
                strike = 1; /* automatic hit */

            break;

        case AT_ENGL:
            if (u.usteed && mdef == u.usteed) 
            {
                strike = 0;
                break;
            }

            /* D: Prevent engulf from a distance */
            if (distmin(magr->mx, magr->my, mdef->mx, mdef->my) > 1)
                continue;
            /* Engulfing attacks are directed at the hero if possible. -dlc */
            if (first_attack)
            {
                play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                first_attack = FALSE;
            }

            update_m_action(magr, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
            play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            m_wait_until_action();
            if (u.uswallow && magr == u.ustuck)
                strike = 0;
            else if ((strike = (tmp > rnd(20 + i))) != 0)
                res[i] = gulpmm(magr, mdef, mattk);
            else
                missmm(magr, mdef, mattk);
            update_m_action_revert(magr, ACTION_TILE_NO_ACTION);
            break;

        case AT_BREA:
            if (!monnear(magr, mdef->mx, mdef->my))
            {
                if (first_attack)
                {
                    play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(magr, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                strike = breamm(magr, mattk, mdef);
                update_m_action_revert(magr, ACTION_TILE_NO_ACTION);

                /* We don't really know if we hit or not; pretend we did. */
                if (strike)
                    res[i] |= MM_HIT;
                if (DEADMONSTER(mdef))
                    res[i] = MM_DEF_DIED;
                if (DEADMONSTER(magr))
                    res[i] |= MM_AGR_DIED;
            }
            else
                strike = 0;
            break;

        case AT_EYES:
            strike = 0;
            if ((monnear(magr, mdef->mx, mdef->my) || rn2(6)) && !is_reflecting(mdef) && !is_blinded(magr))
            {
                if (first_attack)
                {
                    play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(magr, mattk->action_tile ? mattk->action_tile : ACTION_TILE_SPECIAL_ATTACK);
                play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                strike = eyesmm(magr, mattk, mdef);
                update_m_action_revert(magr, ACTION_TILE_NO_ACTION);
            }

            /* We don't really know if we hit or not; pretend we did. */
            if (strike)
                res[i] |= MM_HIT;
            if (DEADMONSTER(mdef))
                res[i] = MM_DEF_DIED;
            if (DEADMONSTER(magr))
                res[i] |= MM_AGR_DIED;
            break;

        case AT_MAGC:
            if (!monnear(magr, mdef->mx, mdef->my))
            {
                if (first_attack)
                {
                    play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(magr, ACTION_TILE_CAST_DIR);
                play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                strike = buzzmm(magr, mattk, mdef);
                update_m_action_revert(magr, ACTION_TILE_NO_ACTION);

                /* We don't really know if we hit or not; pretend we did. */
                if (strike)
                    res[i] |= MM_HIT;
                if (DEADMONSTER(mdef))
                    res[i] = MM_DEF_DIED;
                if (DEADMONSTER(magr))
                    res[i] |= MM_AGR_DIED;
            }
            else
                strike = 0;
            break;

        case AT_SPIT:
            if (!monnear(magr, mdef->mx, mdef->my)) 
            {
                if (first_attack)
                {
                    play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_START_ATTACK);
                    first_attack = FALSE;
                }

                update_m_action(magr, mattk->action_tile ? mattk->action_tile : ACTION_TILE_FIRE);
                play_monster_simple_weapon_sound(magr, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
                m_wait_until_action();
                strike = spitmm(magr, mattk, mdef);
                update_m_action_revert(magr, ACTION_TILE_NO_ACTION);

                /* We don't really know if we hit or not; pretend we did. */
                if (strike)
                    res[i] |= MM_HIT;
                if (DEADMONSTER(mdef))
                    res[i] = MM_DEF_DIED;
                if (DEADMONSTER(magr))
                    res[i] |= MM_AGR_DIED;
            }
            break;

        default: /* no attack */
            strike = 0;
            attk = 0;
            break;
        }

        if (attk && !(res[i] & MM_AGR_DIED)
            && distmin(magr->mx, magr->my, mdef->mx, mdef->my) <= 1)
            res[i] = passivemm(magr, mdef, strike, res[i] & MM_DEF_DIED);

        if (res[i] & MM_DEF_DIED)
            return res[i];
        if (res[i] & MM_AGR_DIED)
            return res[i];
        /* return if aggressor can no longer attack */
        if (!mon_can_move(magr))
            return res[i];
        if (res[i] & MM_HIT)
            struck = 1; /* at least one hit */
    }

    return (struck ? MM_HIT : MM_MISS);
}

/* Returns the result of mdamagem(). */
STATIC_OVL int
hitmm(magr, mdef, mattk, omonwep)
register struct monst *magr, *mdef;
struct attack *mattk;
struct obj* omonwep;
{
    if (vis) 
    {
        int compat;
        char buf[BUFSZ], mdef_name[BUFSZ];

        if (!canspotmon(magr))
            map_invisible(magr->mx, magr->my);
        if (!canspotmon(mdef))
            map_invisible(mdef->mx, mdef->my);
        if (M_AP_TYPE(mdef))
            seemimic(mdef);
        if (M_AP_TYPE(magr))
            seemimic(magr);

        if ((compat = could_seduce(magr, mdef, mattk)) && !is_cancelled(magr) && !is_silenced(magr))
        {
            Sprintf(buf, "%s %s", Monnam(magr),
                    !is_blinded(mdef) ? "smiles at" : "talks to");
            pline("%s %s %s.", buf, mon_nam(mdef),
                  compat == 2 ? "engagingly" : "seductively");
        } 
        else
        {
            char magr_name[BUFSZ];

            Strcpy(magr_name, Monnam(magr));
            switch (mattk->aatyp) 
            {
            case AT_BITE:
                Sprintf(buf, "%s bites", magr_name);
                break;
            case AT_STNG:
                Sprintf(buf, "%s stings", magr_name);
                break;
            case AT_BUTT:
                Sprintf(buf, "%s butts", magr_name);
                break;
            case AT_TUSK:
            case AT_HORN:
                Sprintf(buf, "%s gores", magr_name);
                break;
            case AT_TUCH:
                Sprintf(buf, "%s touches", magr_name);
                break;
            case AT_TENT:
                Sprintf(buf, "%s tentacles suck", s_suffix(magr_name));
                break;
            case AT_TAIL:
                Sprintf(buf, "%s lashes %s tail at", magr_name, mhis(magr));
                break;
            case AT_RAMS:
                Sprintf(buf, "%s rams into", magr_name);
                break;
            case AT_HUGS:
                if (magr != u.ustuck) 
                {
                    Sprintf(buf, "%s squeezes", magr_name);
                    break;
                }
                /*FALLTHRU*/
            default:
                Sprintf(buf, "%s hits", magr_name);
            }
            pline("%s %s.", buf, mon_nam_too(mdef_name, mdef, magr));
        }
    } 
    else
        noises(magr, mattk);

    return mdamagem(magr, mdef, mattk, omonwep);
}

/* Returns the same values as mdamagem(). */
STATIC_OVL int
gazemm(magr, mdef, mattk)
register struct monst *magr, *mdef;
struct attack *mattk;
{
    if (!magr || !mdef || !mattk)
        return 0;

    int range = mattk->range ? mattk->range : M_GENERIC_RANGED_ATTACK_RANGE;
    if (dist2(magr->mx, magr->my, mdef->mx, mdef->my) > range * range)
        return 0; /* Out of range */

    if ((mattk->aflags & ATTKFLAG_MUST_SEE_TARGET) && !m_cansee_m(magr, mdef))
        return 0;

    char buf[BUFSZ];

    if (vis) 
    {
        if (is_mimic(mdef->data)
            && M_AP_TYPE(mdef) != M_AP_NOTHING)
            seemimic(mdef);
        Sprintf(buf, "%s gazes at", Monnam(magr));
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s...", buf,
              canspotmon(mdef) ? mon_nam(mdef) : "something");
    }

    if (is_cancelled(magr)|| is_blinded(magr) || is_blinded(mdef)
        || (is_invisible(magr) && !has_see_invisible(mdef)) || is_sleeping(mdef))
    {
        if (vis && canspotmon(mdef))
            pline("but nothing happens.");
        return MM_MISS;
    }
    /* call mon_reflects 2x, first test, then, if visible, print message */
    if (magr->data == &mons[PM_MEDUSA] && mon_reflects(mdef, (char *) 0))
    {
        if (canseemon(mdef))
        {
            play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mdef->mx, mdef->my);
            (void)mon_reflects(mdef, "The gaze is reflected away by %s %s.");
        }
        if (!is_blinded(mdef)) 
        {
            if (mon_reflects(magr, (char *) 0)) 
            {
                if (canseemon(magr))
                {
                    play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, magr->mx, magr->my);
                    (void)mon_reflects(magr,
                        "The gaze is reflected away by %s %s.");
                }
                return MM_MISS;
            }
            if (is_invisible(mdef) && !has_see_invisible(magr))
            {
                if (canseemon(magr)) 
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION,
                      "%s doesn't seem to notice that %s gaze was reflected.",
                          Monnam(magr), mhis(magr));
                }
                return MM_MISS;
            }
            play_sfx_sound_at_location(SFX_PETRIFY, magr->mx, magr->my);
            if (canseemon(magr))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is turned to stone!", Monnam(magr));
            monstone(magr);
            if (!DEADMONSTER(magr))
                return MM_MISS;
            return MM_AGR_DIED;
        }
    }

    return mdamagem(magr, mdef, mattk, (struct obj*)0);
}

/* return True if magr is allowed to swallow mdef, False otherwise */
boolean
engulf_target(magr, mdef)
struct monst *magr, *mdef;
{
    struct rm *lev;
    int dx, dy;

    /* can't swallow something that's too big */
    if (mdef->data->msize >= MZ_HUGE)
        return FALSE;

    /* (hypothetical) engulfers who can pass through walls aren't
     limited by rock|trees|bars */
    if ((magr == &youmonst) ? Passes_walls : passes_walls(magr->data))
        return TRUE;

    /* don't swallow something in a spot where attacker wouldn't
       otherwise be able to move onto; we don't want to engulf
       a wall-phaser and end up with a non-phaser inside a wall */
    dx = mdef->mx, dy = mdef->my;
    if (mdef == &youmonst)
        dx = u.ux, dy = u.uy;
    lev = &levl[dx][dy];
    if (IS_ROCK(lev->typ) || closed_door(dx, dy) || IS_TREE(lev->typ)
        /* not passes_bars(); engulfer isn't squeezing through */
        || (lev->typ == IRONBARS && !is_whirly(magr->data)))
        return FALSE;

    return TRUE;
}

/* Returns the same values as mattackm(). */
STATIC_OVL int
gulpmm(magr, mdef, mattk)
register struct monst *magr, *mdef;
register struct attack *mattk;
{
    xchar ax, ay, dx, dy;
    int status;
    char buf[BUFSZ];
    struct obj *obj;

    if (!engulf_target(magr, mdef))
        return MM_MISS;

    if (vis) 
    {
        /* [this two-part formatting dates back to when only one x_monnam
           result could be included in an expression because the next one
           would overwrite first's result -- that's no longer the case] */
        Sprintf(buf, "%s swallows", Monnam(magr));
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s.", buf, mon_nam(mdef));
    }

    for (obj = mdef->minvent; obj; obj = obj->nobj)
        (void) snuff_lit(obj);

    if (mdef->cham >= LOW_PM && is_vampshifter(mdef)
        && newcham(mdef, &mons[mdef->cham], FALSE, FALSE))
    {
        if (vis)
        {
            /* 'it' -- previous form is no longer available and
               using that would be excessively verbose */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s expels %s.", Monnam(magr),
                  canspotmon(mdef) ? "it" : something);
            if (canspotmon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It turns into %s.", a_monnam(mdef));
        }
        return MM_HIT; /* bypass mdamagem() */
    }

    /*
     *  All of this manipulation is needed to keep the display correct.
     *  There is a flush at the next pline().
     */
    ax = magr->mx;
    ay = magr->my;
    dx = mdef->mx;
    dy = mdef->my;
    /*
     *  Leave the defender in the monster chain at it's current position,
     *  but don't leave it on the screen.  Move the aggressor to the
     *  defender's position.
     */
    remove_monster(dx, dy);
    remove_monster(ax, ay);
    place_monster(magr, dx, dy);
    newsym(ax, ay); /* erase old position */
    newsym(dx, dy); /* update new position */

    status = mdamagem(magr, mdef, mattk, (struct obj*)0);

    if ((status & (MM_AGR_DIED | MM_DEF_DIED))
        == (MM_AGR_DIED | MM_DEF_DIED))
    {
        ;                              /* both died -- do nothing  */
    } 
    else if (status & MM_DEF_DIED) 
    { /* defender died */
        /*
         *  Note:  remove_monster() was called in relmon(), wiping out
         *  magr from level.monsters[mdef->mx][mdef->my].  We need to
         *  put it back and display it.  -kd
         */
        place_monster(magr, dx, dy);
        newsym(dx, dy);
        /* aggressor moves to <dx,dy> and might encounter trouble there */
        if (minliquid(magr) || (t_at(dx, dy) && mintrap(magr) == 2))
            status |= MM_AGR_DIED;
    } 
    else if (status & MM_AGR_DIED) 
    { /* aggressor died */
        place_monster(mdef, dx, dy);
        newsym(dx, dy);
    }
    else 
    {                           /* both alive, put them back */
        if (cansee(dx, dy))
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is regurgitated!", Monnam(mdef));

        remove_monster(dx,dy);
        place_monster(magr, ax, ay);
        place_monster(mdef, dx, dy);
        newsym(ax, ay);
        newsym(dx, dy);
    }

    return status;
}

STATIC_OVL int
explmm(magr, mdef, mattk)
struct monst *magr, *mdef;
struct attack *mattk;
{
    int result;

    if (is_cancelled(magr))
        return MM_MISS;

    enum sfx_sound_types sfx_sound = SFX_ILLEGAL;
    enum special_effect_types spef_idx = MAX_SPECIAL_EFFECTS;
    enum explosion_types expl_idx = MAX_EXPLOSIONS;

    switch (mattk->adtyp)
    {
    case AD_COLD:
        expl_idx = EXPL_FREEZING_SPHERE;
        sfx_sound = explosion_type_definitions[expl_idx].sfx;
        break;
    case AD_FIRE:
        expl_idx = EXPL_FLAMING_SPHERE;
        sfx_sound = explosion_type_definitions[expl_idx].sfx;
        break;
    case AD_ELEC:
        expl_idx = EXPL_SHOCKING_SPHERE;
        sfx_sound = explosion_type_definitions[expl_idx].sfx;
        break;
    case AD_BLND:
        sfx_sound = SFX_BLINDING_FLASH;
        spef_idx = SPECIAL_EFFECT_YELLOW_LIGHT_FLASH;
        break;
    case AD_HALU:
        sfx_sound = SFX_HALLUCINATING_FLASH;
        spef_idx = SPECIAL_EFFECT_BLACK_LIGHT_FLASH;
        break;
    default:
        break;
    }
    if (spef_idx < MAX_SPECIAL_EFFECTS)
        play_special_effect_at(spef_idx, 0, magr->mx, magr->my, FALSE);
    else if (expl_idx < MAX_EXPLOSIONS)
        play_explosion_animation_at(magr->mx, magr->my, expl_idx);

    if (sfx_sound != SFX_ILLEGAL)
        play_sfx_sound_at_location_with_minimum_volume(sfx_sound, magr->mx, magr->my, 0.15);

    if (spef_idx < MAX_SPECIAL_EFFECTS)
    {
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        special_effect_wait_until_action(0);
        show_glyph_on_layer(magr->mx, magr->my, NO_GLYPH, LAYER_MONSTER);
        flush_screen(1);
    }
    else if (expl_idx < MAX_EXPLOSIONS)
    {
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        explosion_wait_until_action();
        show_glyph_on_layer(magr->mx, magr->my, NO_GLYPH, LAYER_MONSTER);
        flush_screen(1);
    }

    if (cansee(magr->mx, magr->my))
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s explodes!", Monnam(magr));
    else
        noises(magr, mattk);

    result = mdamagem(magr, mdef, mattk, (struct obj*)0);

    /* Kill off aggressor if it didn't die. */
    if (!(result & MM_AGR_DIED))
    {
        boolean was_leashed = (magr->mleashed != 0);

        mondead_with_flags(magr, MONDIED_FLAGS_NO_DEATH_ACTION);
        if (!DEADMONSTER(magr))
            return result; /* life saved */
        result |= MM_AGR_DIED;

        /* mondead() -> m_detach() -> m_unleash() always suppresses
           the m_unleash() slack message, so deliver it here instead */
        if (was_leashed)
        {
            play_sfx_sound(SFX_LEASH_GOES_SLACK);
            Your("leash falls slack.");
        }
    }

    if (is_tame(magr)) /* give this one even if it was visible */
        You_ex(ATR_NONE, CLR_MSG_WARNING, brief_feeling, "melancholy");

    if (spef_idx < MAX_SPECIAL_EFFECTS)
    {
        special_effect_wait_until_end(0);
        context.global_newsym_flags = 0UL;
    }
    else if (expl_idx < MAX_EXPLOSIONS)
    {
        explosion_wait_until_end();
        context.global_newsym_flags = 0UL;
    }

    return result;
}

/*
 *  See comment at top of mattackm(), for return values.
 */
STATIC_OVL int
mdamagem(magr, mdef, mattk, omonwep)
register struct monst *magr, *mdef;
register struct attack *mattk;
register struct obj* omonwep;
{
    struct obj *obj;
    char buf[BUFSZ];
    struct permonst *pa = magr->data, *pd = mdef->data;
    int num,res = MM_MISS;
    boolean cancelled;
    double poisondamage = 0;
    boolean isdisintegrated = FALSE;
    boolean hittxtalreadydisplayed = FALSE;
    //boolean objectshatters = FALSE;
    int critstrikeroll = rn2(100);
    enum hit_tile_types hit_tile = HIT_GENERAL;

    int extratmp = 0;
    double damage = 0;
    int increase_damage_adtyp = mattk->adtyp;

    struct obj* mweapon = omonwep; // MON_WEP(magr);
    boolean uses_spell_flags = omonwep ? object_uses_spellbook_wand_flags_and_properties(omonwep) : FALSE;
    
    /* cancellation factor is the same as when attacking the hero */
    cancelled = is_cancelled(magr) || check_magic_cancellation_success(mdef, mattk->mcadj);


    if (mweapon)
    {
        if (is_launcher(mweapon))
            damage += adjust_damage(d(1, 2), magr, mdef, objects[mweapon->otyp].oc_damagetype, ADFLAGS_NONE);
        else
        {
            int basedmg = weapon_dmg_value(mweapon, mdef, magr, 0);
            damage += adjust_damage(basedmg, magr, mdef, objects[mweapon->otyp].oc_damagetype, ADFLAGS_NONE);
            extratmp = weapon_extra_dmg_value(mweapon, mdef, magr, basedmg);
            damage += adjust_damage(extratmp, magr, mdef, objects[mweapon->otyp].oc_extra_damagetype, ADFLAGS_NONE);
        }
        increase_damage_adtyp = objects[mweapon->otyp].oc_damagetype;
    }
    else
    {
        int basedmg = 0;
        if (mattk->damn > 0 && mattk->damd > 0)
            basedmg += d((int)mattk->damn, (int)mattk->damd);
        basedmg += (int)mattk->damp;
        damage += adjust_damage(basedmg, magr, mdef, mattk->adtyp, ADFLAGS_NONE);
    }

    //Damage bonus is obtained in any case
    if (mattk->adtyp == AD_PHYS || mattk->adtyp == AD_DRIN)
    {
        if (omonwep || mattk->aatyp == AT_WEAP || mattk->aatyp == AT_HUGS)
            damage += adjust_damage(m_str_dmg_bonus(magr), magr, mdef, mattk->adtyp, ADFLAGS_NONE);
        else
            damage += adjust_damage(m_str_dmg_bonus(magr) / 2, magr, mdef, mattk->adtyp, ADFLAGS_NONE);
    }

    damage += adjust_damage(magr->mdaminc, magr, mdef, increase_damage_adtyp, ADFLAGS_NONE);

    if ((touch_petrifies(pd) /* or flesh_petrifies() */
         || (mattk->adtyp == AD_DGST && pd == &mons[PM_MEDUSA]))
        && !resists_ston(magr)) 
    {
        /* Note: no cancellation applies because the mon touches the petrifying creature by attacking bare handed */
        long protector = attk_protection((int) mattk->aatyp),
             wornitems = magr->worn_item_flags;

        /* wielded weapon gives same protection as gloves here */
        if (otmp != 0)
            wornitems |= W_ARMG;

        if (protector == 0L
            || (protector != ~0L && (wornitems & protector) != protector)) 
        {
            start_delayed_petrification(magr, FALSE);
#if 0
            if (poly_when_stoned(pa)) {
                mon_to_stone(magr);
                return MM_HIT; /* no damage during the polymorph */
            }
            if (vis && canspotmon(magr))
                pline("%s turns to stone!", Monnam(magr));
            monstone(magr);
            if (!DEADMONSTER(magr))
                return MM_HIT; /* lifesaved */
            else if (is_tame(magr) && !vis)
                You(brief_feeling, "peculiarly sad");
            return MM_AGR_DIED;
#endif
        }
    }

    switch (mattk->adtyp) 
    {
    case AD_DGST:
        /* eating a Rider or its corpse is fatal */
        if (is_rider(pd))
        {
            if (vis && canseemon(magr))
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s %s!", Monnam(magr),
                      (pd == &mons[PM_FAMINE])
                          ? "belches feebly, shrivels up and dies"
                          : (pd == &mons[PM_PESTILENCE])
                                ? "coughs spasmodically and collapses"
                                : "vomits violently and drops dead");
            mondied(magr);
            if (!DEADMONSTER(magr))
                return 0; /* lifesaved */
            else if (is_tame(magr) && !vis)
                You_ex(ATR_NONE, CLR_MSG_WARNING, brief_feeling, "queasy");
            return MM_AGR_DIED;
        }
        if (flags.verbose && !Deaf)
        {
            play_sfx_sound_at_location(SFX_BURP, magr->mx, magr->my);
            verbalize("Burrrrp!");
        }
        // mdef->mhp;
        break;
    case AD_STUN:
        if (is_cancelled(magr) || resists_stun(mdef))
            break;
        if (canseemon(mdef))
        { 
            play_sfx_sound_at_location(SFX_ACQUIRE_STUN, mdef->mx, mdef->my);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s %s for a moment.", Monnam(mdef),
                makeplural(stagger(pd, "stagger")));
        }
        nonadditive_increase_mon_property(mdef, STUNNED, 5 + rnd(5));
        goto physical;
    case AD_LEGS:
        if (is_cancelled(magr)) 
        {
            damage = 0;
            break;
        }
        goto physical;
    case AD_WERE:
    case AD_HEAL:
    case AD_PHYS:
 physical:
        if (mattk->aatyp == AT_KICK && thick_skinned(pd)) 
        {
            damage = 0;
        } 
        else if (mattk->aatyp == AT_WEAP) 
        {
            if (otmp) 
            {
                if (otmp->otyp == CORPSE
                    && touch_petrifies(&mons[otmp->corpsenm]))
                    goto do_stone;
                //damage += weapon_dmg_value(otmp, mdef,magr);
                //damage += m_str_dmg_bonus(magr);

                if (damage < 1) /* is this necessary?  mhitu.c has it... */
                    damage = 1;

                if (otmp->oartifact) 
                {
                    (void) artifact_hit(magr, mdef, otmp, &damage, dieroll);
                    if (DEADMONSTER(mdef))
                        return (MM_DEF_DIED
                                | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
                }
                short spec_adtyp = 0;
                int special_hit_dmg = pseudo_artifact_hit(magr, mdef, otmp, extratmp, dieroll, critstrikeroll, &spec_adtyp);
                if (special_hit_dmg < 0)
                {
                    hittxtalreadydisplayed = TRUE;
                    damage += 2 * (double)mdef->mhp + 200;
                    if (special_hit_dmg == -2)
                        isdisintegrated = TRUE;
                }
                else if (special_hit_dmg > 0)
                {
                    damage += adjust_damage(special_hit_dmg, magr, mdef, spec_adtyp, ADFLAGS_NONE);
                }

                /* Check if the object shatters */
                if (damage > 0)
                    rustm(mdef, otmp);
            }
        }
        else if (pa == &mons[PM_PURPLE_WORM] && pd == &mons[PM_SHRIEKER])
        {
            /* hack to enhance mm_aggression(); we don't want purple
               worm's bite attack to kill a shrieker because then it
               won't swallow the corpse; but if the target survives,
               the subsequent engulf attack should accomplish that */
            if (damage >= (double)mdef->mhp && mdef->mhp > 1)
                damage = (double)mdef->mhp - 1;
        }
        break;
    case AD_FIRE:
#if 0
        if (cancelled)
        {
            damage = 0;
            break;
        }
#endif
        hit_tile = HIT_ON_FIRE;
        play_sfx_sound_at_location(SFX_MONSTER_ON_FIRE, mdef->mx, mdef->my);
        if (vis && canseemon(mdef))
            pline("%s is %s!", Monnam(mdef), on_fire(pd, mattk));
        if (completelyburns(pd)) { /* paper golem or straw golem */
            if (vis && canseemon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s burns completely!", Monnam(mdef));
            mondead(mdef); /* was mondied() but that dropped paper scrolls */
            if (!DEADMONSTER(mdef))
                return 0;
            else if (is_tame(mdef) && !vis)
                pline("May %s roast in peace.", mon_nam(mdef));
            return (MM_DEF_DIED | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
        }
        damage += adjust_damage(destroy_mitem(mdef, SCROLL_CLASS, AD_FIRE), magr, mdef, AD_FIRE, ADFLAGS_NONE);
        damage += adjust_damage(destroy_mitem(mdef, SPBOOK_CLASS, AD_FIRE), magr, mdef, AD_FIRE, ADFLAGS_NONE);
        if (is_mon_immune_to_fire(mdef))
        {
            if (vis && canseemon(mdef))
            {
                pline_The("fire doesn't seem to burn %s!", mon_nam(mdef));
                play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, mdef->mx, mdef->my);
                m_shieldeff(mdef);
            }
            golemeffects(mdef, AD_FIRE, damage);
            damage = 0;
        }
        /* only potions damage resistant players in destroy_item */
        damage += adjust_damage(destroy_mitem(mdef, POTION_CLASS, AD_FIRE), magr, mdef, AD_FIRE, ADFLAGS_NONE);
        break;
    case AD_COLD:
#if 0
        if (cancelled)
        {
            damage = 0;
            break;
        }
#endif
        hit_tile = HIT_FROZEN;
        play_sfx_sound_at_location(SFX_MONSTER_COVERED_IN_FROST, mdef->mx, mdef->my);
        if (vis && canseemon(mdef))
            pline("%s is covered in frost!", Monnam(mdef));
        if (is_mon_immune_to_cold(mdef))
        {
            if (vis && canseemon(mdef))
            {
                pline_The("frost doesn't seem to chill %s!", mon_nam(mdef));
                play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, mdef->mx, mdef->my);
                m_shieldeff(mdef);
            }
            golemeffects(mdef, AD_COLD, damage);
            damage = 0;
        }
        damage += adjust_damage(destroy_mitem(mdef, POTION_CLASS, AD_COLD), magr, mdef, AD_COLD, ADFLAGS_NONE);
        break;
    case AD_ELEC:
#if 0
        if (cancelled)
        {
            damage = 0;
            break;
        }
#endif

        hit_tile = HIT_ELECTROCUTED;
        play_sfx_sound_at_location(SFX_MONSTER_GETS_ZAPPED, mdef->mx, mdef->my);
        if (vis && canseemon(mdef))
            pline("%s gets zapped!", Monnam(mdef));
        
        damage += adjust_damage(destroy_mitem(mdef, WAND_CLASS, AD_ELEC), magr, mdef, AD_COLD, ADFLAGS_NONE);
        
        if (is_mon_immune_to_elec(mdef)) 
        {
            if (vis && canseemon(mdef))
            {
                pline_The("zap doesn't shock %s!", mon_nam(mdef));
                play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, mdef->mx, mdef->my);
                m_shieldeff(mdef);
            }
            golemeffects(mdef, AD_ELEC, damage);
            damage = 0;
        }
        /* only rings damage resistant players in destroy_item */
        damage += adjust_damage(destroy_mitem(mdef, RING_CLASS, AD_ELEC), magr, mdef, AD_ELEC, ADFLAGS_NONE);
        break;
    case AD_ACID:
        if (is_cancelled(magr))
        {
            damage = 0;
            break;
        }
        hit_tile = HIT_SPLASHED_ACID;
        play_sfx_sound_at_location(SFX_MONSTER_GETS_SPLASHED_BY_ACID, mdef->mx, mdef->my);
        if (is_mon_immune_to_acid(mdef))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, mdef->mx, mdef->my);
            if (vis && canseemon(mdef))
                pline("%s is covered in %s, but it seems harmless.",
                      Monnam(mdef), hliquid("acid"));
            damage = 0;
        } 
        else if (vis && canseemon(mdef)) 
        {
            pline("%s is covered in %s!", Monnam(mdef), hliquid("acid"));
            pline("It burns %s!", mon_nam(mdef));
        }
        if (!rn2(30))
            erode_armor(mdef, ERODE_CORRODE);
        if (!rn2(6))
            acid_damage(MON_WEP(mdef));
        break;
    case AD_RUST:
        if (is_cancelled(magr))
            break;
        if (is_iron(pd)) 
        {
            if (vis && canseemon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s falls to pieces!", Monnam(mdef));
            mondied(mdef);
            if (!DEADMONSTER(mdef))
                return 0;
            else if (is_tame(mdef) && !vis)
                pline("May %s rust in peace.", mon_nam(mdef));
            return (MM_DEF_DIED | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
        }
        erode_armor(mdef, ERODE_RUST);
        mdef->mstrategy &= ~STRAT_WAITFORU;
        damage = 0;
        break;
    case AD_CORR:
        if (is_cancelled(magr))
            break;
        erode_armor(mdef, ERODE_CORRODE);
        mdef->mstrategy &= ~STRAT_WAITFORU;
        damage = 0;
        break;
    case AD_DCAY:
        if (is_cancelled(magr))
            break;
        if (pd == &mons[PM_WOOD_GOLEM] || pd == &mons[PM_LEATHER_GOLEM])
        {
            if (vis && canseemon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s falls to pieces!", Monnam(mdef));
            mondied(mdef);
            if (!DEADMONSTER(mdef))
                return 0;
            else if (is_tame(mdef) && !vis)
                pline("May %s rot in peace.", mon_nam(mdef));
            return (MM_DEF_DIED | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
        }
        erode_armor(mdef, ERODE_CORRODE);
        mdef->mstrategy &= ~STRAT_WAITFORU;
        damage = 0;
        break;
    case AD_STON:
        if (cancelled) //changed to respect MC
            break;
 do_stone:
        if (!resists_ston(mdef))
        {
            display_m_being_hit(mdef, HIT_PETRIFIED, 0, 0UL, TRUE);
            /* Medusa's gaze is instapetrify */
            if (mattk->aatyp == AT_GAZE)
            {
                if(vis)
                    play_sfx_sound_at_location(SFX_PETRIFY, magr->mx, magr->my);
                if (vis && canseemon(mdef))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s turns to stone!", Monnam(mdef));
                monstone(mdef);

                if (!DEADMONSTER(mdef))
                    return 0;
                else if (is_tame(mdef) && !vis)
                    You_ex(ATR_NONE, CLR_MSG_WARNING, brief_feeling, "peculiarly sad");
                return (MM_DEF_DIED | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
            }
            else
                start_delayed_petrification(mdef, FALSE);
        }
        damage = (mattk->adtyp == AD_STON ? 0 : 1);
        break;
    case AD_TLPT:
        if (!cancelled && damage < mdef->mhp && !tele_restrict(mdef)) 
        {
            char mdef_Monnam[BUFSZ];
            boolean wasseen = canspotmon(mdef);

            /* save the name before monster teleports, otherwise
               we'll get "it" in the suddenly disappears message */
            if (vis && wasseen)
                Strcpy(mdef_Monnam, Monnam(mdef));
            mdef->mstrategy &= ~STRAT_WAITFORU;
            (void) rloc2(mdef, TRUE, TRUE);

            if (vis && wasseen && !canspotmon(mdef) && mdef != u.usteed)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly disappears!", mdef_Monnam);

            if (damage >= (double)mdef->mhp) 
            { /* see hitmu(mhitu.c) */
                if (mdef->mhp == 1)
                    ++mdef->mhp;
                damage = (double)mdef->mhp - 1;
            }
        }
        break;
    case AD_SLEE:
        if (!cancelled && !is_sleeping(mdef)
            && sleep_monst(mdef, (struct obj *)0, magr, rn1(3,8), mattk->mcadj, FALSE)) 
        {
            hit_tile = HIT_SLEEP;
            if (vis && canspotmon(mdef))
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_SLEEP, mdef->mx, mdef->my);
                Strcpy(buf, Monnam(mdef));
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s is put to sleep by %s.", buf, mon_nam(magr));
            }
            mdef->mstrategy &= ~STRAT_WAITFORU;
            slept_monst(mdef);
        }
        break;
    case AD_PLYS:
        if (!cancelled && !resists_paralysis(mdef))
        {
            hit_tile = HIT_PARALYZED;
            if (vis && canspotmon(mdef) && !is_paralyzed(mdef))
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mdef->mx, mdef->my);
                Strcpy(buf, Monnam(mdef));
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s is frozen by %s.", buf, mon_nam(magr));
            }
            paralyze_monst(mdef, 2 + rnd(8), FALSE);
        }
        break;
    case AD_SHRP:
        if (rn2(100) < SHARPNESS_PERCENTAGE_CHANCE)
        {
            hit_tile = HIT_CRITICAL;
            if (vis && canspotmon(mdef) && !is_paralyzed(mdef))
            {
                play_sfx_sound_at_location(SFX_SHARPNESS_SLICE, mdef->mx, mdef->my);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s strike slices a part of %s off!", s_suffix(Monnam(magr)), mon_nam(mdef));
            }
            damage += adjust_damage((mdef->mhpmax * SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE) / 100, magr, mdef, AD_PHYS, ADFLAGS_NONE);
        }
        break;
    case AD_SLOW:
        if (!cancelled && (mdef->mprops[SLOWED] & M_TIMEOUT) < 300)
        {
            play_sfx_sound_at_location(SFX_ACQUIRE_SLOW, mdef->mx, mdef->my);
            (void)increase_mon_property_verbosely(mdef, SLOWED, 20 + rnd(10));
            mdef->mstrategy &= ~STRAT_WAITFORU;
        }
        break;
    case AD_CONF:
        /* Since confusing another monster doesn't have a real time
         * limit, setting spec_used would not really be right (though
         * we still should check for it).
         */
        if (!is_cancelled(magr)&& !is_confused(mdef) && !magr->mspec_used)
        {
            if (vis && canseemon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks confused.", Monnam(mdef));
            play_sfx_sound_at_location(SFX_ACQUIRE_CONFUSION, mdef->mx, mdef->my);
            nonadditive_increase_mon_property(mdef, CONFUSION, 20 + rnd(10));
            mdef->mstrategy &= ~STRAT_WAITFORU;
        }
        break;
    case AD_BLND:
        if (can_blnd(magr, mdef, mattk->aatyp, (struct obj *) 0)) 
        {
            register unsigned rnd_tmp;

            if (vis && !is_blinded(mdef) && canspotmon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is blinded.", Monnam(mdef));
            rnd_tmp = d((int) mattk->damn, (int) mattk->damd);
            play_sfx_sound_at_location(SFX_ACQUIRE_BLINDNESS, mdef->mx, mdef->my);
            nonadditive_increase_mon_property(mdef, BLINDED, 1 + rnd_tmp);
            mdef->mstrategy &= ~STRAT_WAITFORU;
        }
        damage = 0;
        break;
    case AD_CNCL:
        if (cancelled)
        {
            if (canseemon(magr))
                pline("%s gazes at %s but without effect.", Monnam(magr), mon_nam(mdef));
            break;
        }
        else if (is_blinded(magr) || (is_invisible(mdef) && !has_see_invisible(magr)))
        {
            if (canseemon(magr))
                pline("%s stares blindly at %s general direction.", Monnam(magr), s_suffix(mon_nam(mdef)));
            break;
        }
#if 0
        else if (is_reflecting(mdef))
        {
            if (canseemon(magr))
            {
                pline("%s gazes at %s.", Monnam(magr), mon_nam(mdef));
                play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mdef->mx, mdef->my);
                (void)mon_reflects(mdef, "The gaze is reflected away by %s %s!");
            }
            break;
        }
#endif
        else
        {
            if (canseemon(magr))
            {
                if (is_cancelled(mdef))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s gazes at %s. %s is hit by an invisible anti-magic ray!", Monnam(magr), mon_nam(mdef), Monnam(mdef));
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s focuses %s anti-magic gaze on %s.", Monnam(magr), mhis(magr), mon_nam(mdef));
            }
            if (has_cancellation_resistance(mdef))
            {
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mdef->mx, mdef->my);
                pline("However, %s is unaffected!", mon_nam(mdef));
                m_shieldeff(mdef);
            }
            else
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_CANCELLATION, mdef->mx, mdef->my);
                (void)nonadditive_increase_mon_property_verbosely(mdef, CANCELLED, d(2, 4));
                (void)nonadditive_increase_mon_property_verbosely(mdef, CANCELLATION_RESISTANCE, 10);
            }
        }
        break;    
    case AD_HALU:
        if (!is_cancelled(magr)&& haseyes(pd) && !is_blinded(mdef)) 
        {
            if (vis && canseemon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks %sconfused.", Monnam(mdef),
                      is_confused(mdef) ? "more " : "");

            play_sfx_sound_at_location(SFX_ACQUIRE_HALLUCINATION, mdef->mx, mdef->my);
            nonadditive_increase_mon_property(mdef, HALLUC, 100 + rnd(50));
            mdef->mstrategy &= ~STRAT_WAITFORU;
        }
        damage = 0;
        break;
    case AD_CURS:
        if (!night() && (pa == &mons[PM_GREMLIN]))
            break;
        if (!is_cancelled(magr) && !has_cancellation_resistance(mdef) && !rn2(10)) 
        {
            increase_mon_property(mdef, CANCELLED, 100 + rnd(50)); /* cancelled regardless of lifesave */
            mdef->mstrategy &= ~STRAT_WAITFORU;
            refresh_m_tile_gui_info(mdef, FALSE);

            if (is_were(pd) && pd->mlet != S_HUMAN)
                were_change(mdef);

            if (pd == &mons[PM_CLAY_GOLEM]) 
            {
                if (vis && canseemon(mdef))
                {
                    play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mdef->mx, mdef->my);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Some writing vanishes from %s head!",
                          s_suffix(mon_nam(mdef)));
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is destroyed!", Monnam(mdef));
                }
                mondied(mdef);
                if (!DEADMONSTER(mdef))
                    return 0;
                else if (is_tame(mdef) && !vis)
                    You_ex(ATR_NONE, CLR_MSG_WARNING, brief_feeling, "strangely sad");
                return (MM_DEF_DIED
                        | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
            }

            if (!Deaf && !is_silenced(magr)) 
            {
                play_simple_monster_sound(magr, MONSTER_SOUND_TYPE_LAUGHTER);
                if (!vis)
                    You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "laughter.");
                else if (canseemon(magr))
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s chuckles.", Monnam(magr));
            }
        }
        break;
    case AD_SGLD:
        damage = 0;
        if (is_cancelled(magr))
            break;
        /* technically incorrect; no check for stealing gold from
         * between mdef's feet...
         */
        {
            struct obj *gold = findgold(mdef->minvent);

            if (!gold)
                break;
            obj_extract_self(gold);
            add_to_minv(magr, gold);
        }
        mdef->mstrategy &= ~STRAT_WAITFORU;
        play_sfx_sound_at_location(SFX_STEAL_GOLD, mdef->mx, mdef->my);
        if (vis && canseemon(mdef))
        {
            Strcpy(buf, Monnam(magr));
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s steals some gold from %s.", buf, mon_nam(mdef));
        }
        if (!tele_restrict(magr)) 
        {
            boolean couldspot = canspotmon(magr);
            (void) rloc2(magr, TRUE, TRUE);
            if (vis && couldspot && !canspotmon(magr))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly disappears!", buf);
        }
        break;
    case AD_DRLI:
        if (!cancelled && !resists_drli(mdef)) //!rn2(3) && 
        {
            hit_tile = HIT_DRAIN_LEVEL;
            int basehpdrain = d(2, 6);
            play_sfx_sound_at_location(SFX_DRAIN_LIFE, mdef->mx, mdef->my);
            if (vis && canspotmon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly seems weaker!", Monnam(mdef));
            mdef->mbasehpmax -= basehpdrain;
            if (mdef->m_lev == 0)
                damage = (double)mdef->mhp + 1;
            else
                mdef->m_lev--;

            /* Automatic kill if drained past level 0 */
        }
        break;
    case AD_SSEX:
    case AD_SITM: /* for now these are the same */
    case AD_SEDU:
        if (cancelled || resists_charm(mdef))
            break;
        /* find an object to steal, non-cursed if magr is tame */
        for (obj = mdef->minvent; obj; obj = obj->nobj)
            if (!is_tame(magr) || !obj->cursed)
                break;

        if (obj)
        {
            char onambuf[BUFSZ], mdefnambuf[BUFSZ];

            /* make a special x_monnam() call that never omits
               the saddle, and save it for later messages */
            Strcpy(mdefnambuf,
                   x_monnam(mdef, ARTICLE_THE, (char *) 0, 0, FALSE));

            otmp = obj;
            if (u.usteed == mdef && otmp == which_armor(mdef, W_SADDLE))
                /* "You can no longer ride <steed>." */
                dismount_steed(DISMOUNT_POLY);
            obj_extract_self(otmp);
            if (otmp->owornmask) {
                mdef->worn_item_flags &= ~otmp->owornmask;
                if (otmp->owornmask & W_WEP)
                    mwepgone(mdef);
                otmp->owornmask = 0L;
                update_all_mon_statistics(mdef, FALSE);
                /* give monster a chance to wear other equipment on its next
                   move instead of waiting until it picks something up */
                mdef->worn_item_flags |= I_SPECIAL;
            }
            /* add_to_minv() might free otmp [if it merges] */
            if (vis)
                Strcpy(onambuf, doname(otmp));
            (void) add_to_minv(magr, otmp);
            play_sfx_sound_at_location(SFX_STEAL_ITEM, mdef->mx, mdef->my);
            if (vis && canseemon(mdef))
            {
                Strcpy(buf, Monnam(magr));
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s steals %s from %s!", buf, onambuf, mdefnambuf);
            }
            possibly_unwield(mdef, FALSE);
            mdef->mstrategy &= ~STRAT_WAITFORU;
            mselftouch(mdef, (const char *) 0, FALSE);
            refresh_m_tile_gui_info(mdef, FALSE);
            if (DEADMONSTER(mdef))
                return (MM_DEF_DIED
                        | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));

            if (pa->mlet == S_NYMPH && !tele_restrict(magr)) 
            {
                boolean couldspot = canspotmon(magr);

                (void) rloc2(magr, TRUE, TRUE);
                if (vis && couldspot && !canspotmon(magr))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly disappears!", buf);
            }
        }
        damage = 0;
        break;
    case AD_DREN:
        if (!cancelled && !rn2(4))
        {
            play_sfx_sound_at_location(SFX_DRAIN_ENERGY, mdef->mx, mdef->my);
            xdrainenergym(mdef, (boolean)(vis&& canspotmon(mdef)
                && mattk->aatyp != AT_ENGL));
        }
        damage = 0;
        break;
    case AD_DRST:
    case AD_DRDX:
    case AD_DRCO:
        if (!cancelled && !rn2(2)) 
        {
            hit_tile = HIT_POISONED;
            if (vis && canspotmon(magr))
            {
                play_sfx_sound_at_location(SFX_MONSTER_IS_POISONED, mdef->mx, mdef->my);
                pline("%s %s was poisoned!", s_suffix(Monnam(magr)),
                    mpoisons_subj(magr, mattk));

            }
            if (resists_poison(mdef)) 
            {
                //play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, mdef->mx, mdef->my);
                if (vis && canspotmon(mdef) && canspotmon(magr))
                    pline_The("poison doesn't seem to affect %s.",
                              mon_nam(mdef));
            }
            else
            {
                if (magr->m_lev <= 2)
                {
                    poisondamage = adjust_damage(d(1, 6), magr, mdef, AD_DRCO, ADFLAGS_NONE);
                }
                else if (magr->m_lev <= 5)
                {
                    poisondamage = adjust_damage(d(2, 6), magr, mdef, AD_DRCO, ADFLAGS_NONE);
                }
                else
                {
                    if (rn2(10) || magr->m_lev <= 8)
                        poisondamage = adjust_damage(d(3, 6), magr, mdef, AD_DRCO, ADFLAGS_NONE);
                    else 
                        poisondamage = adjust_damage(d(6, 6) + 10, magr, mdef, AD_DRCO, ADFLAGS_NONE); // mdef->mhp;
                }
                damage += poisondamage;
            }
        }
        break;
    case AD_DRIN:
        if (notonhead || !has_head(pd)) 
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, mdef->mx, mdef->my);
            if (vis && canspotmon(mdef))
                pline("%s doesn't seem harmed.", Monnam(mdef));
            /* Not clear what to do for green slimes */
            damage = 0;
            break;
        }
        if ((mdef->worn_item_flags & W_ARMH) && rn2(8)) 
        {
            if (vis && canspotmon(magr) && canseemon(mdef)) 
            {
                play_sfx_sound_at_location(SFX_HELMET_BLOCKS_ATTACK, mdef->mx, mdef->my);
                Strcpy(buf, s_suffix(Monnam(mdef)));
                pline("%s helmet blocks %s attack to %s head.", buf,
                      s_suffix(mon_nam(magr)), mhis(mdef));
            }
            break;
        }
        res = eat_brains(magr, mdef, vis, &damage);
        break;
    case AD_SLIM:
        if (cancelled)
            break; /* physical damage only */
        if (!slimeproof(pd)) 
        {
            hit_tile = HIT_SLIMED;
            play_sfx_sound_at_location(SFX_START_SLIMING, mdef->mx, mdef->my);
            start_delayed_sliming(mdef, FALSE);
#if 0
            if (!munslime(mdef, FALSE) && !DEADMONSTER(mdef)) {
                if (newcham(mdef, &mons[PM_GREEN_SLIME], FALSE,
                            (boolean) (vis && canseemon(mdef))))
                    pd = mdef->data;
                mdef->mstrategy &= ~STRAT_WAITFORU;
                res = MM_HIT;
            }
            /* munslime attempt could have been fatal,
               potentially to multiple monsters (SCR_FIRE) */
            if (DEADMONSTER(magr))
                res |= MM_AGR_DIED;
            if (DEADMONSTER(mdef))
                res |= MM_DEF_DIED;
            damage = 0;
#endif
        }
        break;
    case AD_STCK:
        if (cancelled)
            damage = 0;
        break;
    case AD_WRAP: /* monsters cannot grab one another, it's too hard */
        if (is_cancelled(magr))
            damage = 0;
        break;
    case AD_ENCH:
        /* there's no msomearmor() function, so just do damage */
        /* if (cancelled) break; */
        break;
    case AD_DISE:
        if (!resists_sickness(mdef) && !cancelled)
        {
            hit_tile = HIT_SICK;
            play_sfx_sound_at_location(SFX_CATCH_TERMINAL_ILLNESS, mdef->mx, mdef->my);
            set_mon_property_verbosely(mdef, SICK,
                is_sick(mdef) ? max(1, (get_mon_property(mdef, SICK) + 1) / 3) : rn1(M_ACURR(mdef, A_CON), 20));
        }
        break;
    case AD_ROTS:
        if (!resists_sickness(mdef) && !cancelled)
        {
            hit_tile = HIT_SICK;
            play_sfx_sound_at_location(SFX_CATCH_MUMMY_ROT, mdef->mx, mdef->my);
            set_mon_property_verbosely(mdef, MUMMY_ROT, -1L);
        }
        break;
    default:
        damage = 0;
        break;
    }
    if (!damage)
    {
        refresh_m_tile_gui_info(mdef, FALSE);
        return res;
    }

    int crit_strike_probability = get_critical_strike_percentage_chance(mweapon, mdef, magr);
    int crit_strike_die_roll_threshold = crit_strike_probability / 5;

    /* Wounding */
    if (mweapon && !uses_spell_flags && !isdisintegrated && !is_rider(mdef->data))
    {
        int extradmg = 0;
        if(
            (
                (objects[mweapon->otyp].oc_aflags & A1_WOUNDING) &&
                eligible_for_extra_damage(mweapon, mdef, magr) 
                && (
                ((objects[mweapon->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                    && (
                    ((objects[mweapon->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                        && dieroll <= crit_strike_die_roll_threshold)
                        ||
                        (!(objects[mweapon->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && critstrikeroll < crit_strike_probability))
                    )
                    ||
                    (!(objects[mweapon->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && 1)
                    )
               )
            )
        {
            if (objects[mweapon->otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
                extradmg += (int)ceil(damage);
            else
                extradmg += extratmp;
        }

        if (has_obj_mythic_wounding(mweapon))
            extradmg += mythic_wounding_amount(mweapon);

        if (extradmg > 0)
        {
            mdef->mbasehpmax -= extradmg;

            if (extradmg > 0)
            {
                if (canspotmon(mdef) && canspotmon(magr))
                {
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s's %s %s deeply into %s!", Monnam(magr), cxname(mweapon), otense(mweapon, "cut"), mon_nam(mdef));
                }
            }
        }
    }

    /* Life leech */
    if (mweapon && !isdisintegrated && !resists_life_leech(mdef))
    {
        int extradmg = 0;
        if (!uses_spell_flags && (objects[mweapon->otyp].oc_aflags & A1_LIFE_LEECH) && eligible_for_extra_damage(mweapon, mdef, magr)

            && (
                ((objects[mweapon->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                    && (
                        ((objects[mweapon->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && dieroll <= crit_strike_die_roll_threshold)
                        ||
                        (!(objects[mweapon->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && critstrikeroll < crit_strike_probability))
                    )
                ||
                (!(objects[mweapon->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                    && 1)
                )
            )
        {
            if (objects[mweapon->otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
                extradmg += (int)ceil(damage);
            else
                extradmg += extratmp;
        }

        if (has_obj_mythic_life_draining(mweapon))
            extradmg += mythic_life_draining_amount(mweapon);

        if (extradmg > 0)
        {
            int hpbefore = mdef->mhp;
            deduct_monster_hp(mdef, (double)-extradmg);

            if (mdef->mhp > hpbefore)
            {
                if (canspotmon(mdef) && canspotmon(magr))
                {
                    play_sfx_sound_at_location(SFX_LIFE_LEECH, mdef->mx, mdef->my);
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s's %s %s life energy from %s!", Monnam(magr), cxname(mweapon), otense(mweapon, "leech"), mon_nam(mdef));
                }
            }
        }
    }


    //Reduce HP
    int hp_before = mdef->mhp;
    deduct_monster_hp(mdef, damage);
    int hp_after = mdef->mhp;
    int damagedealt = hp_before - hp_after;
    update_mon_maxhp(mdef);
    if (is_long_worm_with_tail(mdef->data) && isok(bhitpos.x, bhitpos.y) && !is_wseg_head(mdef, bhitpos.x, bhitpos.y))
        play_monster_weapon_hit_sound_at_location(magr, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mdef), get_pm_attack_index(magr->data, mattk), omonwep, damage, 0, bhitpos.x, bhitpos.y); 
    else
        play_monster_weapon_hit_sound(magr, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mdef), get_pm_attack_index(magr->data, mattk), omonwep, damage, 0);

    //Adjust further if mhpmax is smaller
    if (mdef->mhp > mdef->mhpmax)
        mdef->mhp = mdef->mhpmax;

    if (vis && ((is_tame(mdef) && canspotmon(mdef)) || (is_tame(magr) && canspotmon(magr))))
    {
        display_m_being_hit(mdef, hit_tile, damagedealt, 0UL, TRUE);

        if (!hittxtalreadydisplayed && damagedealt > 0)
        {
            pline("%s sustains %d damage!", Monnam(mdef), damagedealt);
        }
    }

    if (iflags.wc2_statuslines > 3 && is_tame(mdef))
        context.botl = 1;

    refresh_m_tile_gui_info(mdef, FALSE);

    if (DEADMONSTER(mdef)) 
    {
        if (poisondamage > 0 && ((double)mdef->mhp + ((double)mdef->mhp_fraction)/10000 - 1) > -poisondamage && vis && canspotmon(mdef) && !isdisintegrated)
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "poison was deadly...");

        if (m_at(mdef->mx, mdef->my) == magr) { /* see gulpmm() */
            remove_monster(mdef->mx, mdef->my);
            mdef->mhp = 1; /* otherwise place_monster will complain */
            place_monster(mdef, mdef->mx, mdef->my);
            reset_monster_origin_coordinates(mdef);
            mdef->mhp = 0;
        }

        if (isdisintegrated)
            disintegrate_mon(mdef, -1, mweapon ? xname(mweapon) : mon_nam(magr));
        else
            monkilled(mdef, "", (int) mattk->adtyp);

        if (!DEADMONSTER(mdef))
            return res; /* mdef lifesaved */
        else if (res == MM_AGR_DIED)
            return (MM_DEF_DIED | MM_AGR_DIED);

        if (mattk->adtyp == AD_DGST) 
        {
            /* Is a corpse for nutrition possible?  It may kill magr */
            //if (!corpse_chance(mdef, magr, TRUE) || DEADMONSTER(magr))
            //    break;

            /* Pets get nutrition from swallowing monster whole.
             * No nutrition from G_NOCORPSE monster, eg, undead.
             * DGST monsters don't die from undead corpses
             */
            num = mdef->mnum;
            if (is_tame(magr) && !magr->isminion
                && !(mvitals[num].mvflags & MV_NOCORPSE)) 
            {
                struct obj* virtualcorpse = mksobj(CORPSE, FALSE, FALSE, FALSE);
                int nutrit;

                set_corpsenm(virtualcorpse, num);
                nutrit = dog_nutrition(magr, virtualcorpse);
                dealloc_obj(virtualcorpse);

                /* only 50% nutrition, 25% of normal eating time */
                if (magr->meating > 1)
                    magr->meating = (magr->meating + 3) / 4;
                if (nutrit > 1)
                    nutrit /= 2;
                EDOG(magr)->hungrytime += nutrit;
            }

            /* various checks similar to dog_eat and meatobj.
             * after monkilled() to provide better message ordering */
            if (mdef->cham >= LOW_PM)
            {
                (void) newcham(magr, (struct permonst *) 0, FALSE, TRUE);
            }
            else if (pd == &mons[PM_GREEN_SLIME] && !slimeproof(pa)) 
            {
                (void) newcham(magr, &mons[PM_GREEN_SLIME], FALSE, TRUE);
            }
            else if (pd == &mons[PM_WRAITH] || pd == &mons[PM_SPECTRE] || pd == &mons[PM_KING_WRAITH])
            {
                (void) grow_up(magr, (struct monst *) 0);
                /* don't grow up twice */
                return (MM_DEF_DIED | (!DEADMONSTER(magr) ? 0 : MM_AGR_DIED));
            }
            else if (pd == &mons[PM_NURSE])
            {
                magr->mhp = magr->mhpmax;
            }
        }
        /* caveat: above digestion handling doesn't keep `pa' up to date */

        return (MM_DEF_DIED | (grow_up(magr, mdef) ? 0 : MM_AGR_DIED));
    }

    return (res == MM_AGR_DIED) ? MM_AGR_DIED : MM_HIT;
}

void
paralyze_monst(mon, amt, verbosely)
struct monst *mon;
int amt;
boolean verbosely;
{
    if (verbosely)
    {
        (void)nonadditive_increase_mon_property_verbosely(mon, PARALYZED, amt);
        play_sfx_sound_at_location(SFX_CAUSE_PARALYSIS, mon->mx, mon->my);
    }
    else
        nonadditive_increase_mon_property(mon, PARALYZED, amt);

#if 0
    if (amt > 127)
        amt = 127;

    mon->mcanmove = 0;
    mon->mfrozen = amt;
#endif

    mon->meating = 0; /* terminate any meal-in-progress */
    mon->mstrategy &= ~STRAT_WAITFORU;
    refresh_m_tile_gui_info(mon, FALSE);
}

/* `mon' is hit by a sleep attack; return 1 if it's affected, 0 otherwise */
/* lvl needs to specified only if otmp == 0, level -1 == cannot be resisted */
int
sleep_monst(mon, origobj, origmonst, amt, saving_throw_adjustment, tellstyle)
struct monst *mon, *origmonst;
struct obj* origobj;
int amt, saving_throw_adjustment, tellstyle;
{
    if (!mon)
        return 0;

    if (!origobj)
    {
        /* Do nothing, since not needed */
    }

    int total_save_adj = saving_throw_adjustment;
    if (origobj && origmonst)
    {
        int otyp = origobj->otyp;
        int skill_level = get_spell_skill_level(otyp, origmonst, mon);
        if (skill_level > P_UNSKILLED)
            total_save_adj -= 2 * (skill_level - P_UNSKILLED);
    }

    if (resists_sleep(mon))
    {
        if (tellstyle != NOTELL)
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
            pline("%s is unaffected!", Monnam(mon));
        }
        m_shieldeff(mon);
    }
    else if(saving_throw_adjustment > -100 && check_ability_resistance_success(mon, A_WIS, total_save_adj)) // check_magic_resistance_and_inflict_damage(mon, otmp, FALSE, 0, 0, tellstyle))
    {
        if (tellstyle != NOTELL)
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
            pline("%s is unaffected!", Monnam(mon));
        }

        m_shieldeff(mon);
    }
    else
    {
        finish_meating(mon); /* terminate any meal-in-progress */
        //amt += (int) mon->mfrozen;
        if (amt > 0) 
        { /* sleep for N turns */
            if(tellstyle == NOTELL)
                nonadditive_increase_mon_property(mon, SLEEPING, amt);
            else
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_SLEEP, mon->mx, mon->my);
                (void)nonadditive_increase_mon_property_verbosely(mon, SLEEPING, amt);
            }
        }
        else 
        { /* sleep until awakened */
            if (tellstyle == NOTELL)
                mon->mprops[SLEEPING] |= M_INTRINSIC_ACQUIRED;
            else
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_SLEEP, mon->mx, mon->my);
                (void)set_mon_property_verbosely(mon, SLEEPING, -1);
            }
        }
        return 1;
    }
    return 0;
}

/* sleeping grabber releases, engulfer doesn't; don't use for paralysis! */
void
slept_monst(mon)
struct monst *mon;
{
    if (!mon)
        return;

    if (!mon_can_move(mon) && mon == u.ustuck
        && !sticks(youmonst.data) && !u.uswallow)
    {
        pline("%s grip relaxes.", s_suffix(Monnam(mon)));
        unstuck(mon);
    }
    newsym(mon->mx, mon->my);
}

void
rustm(mdef, obj)
struct monst *mdef;
struct obj *obj;
{
    int dmgtyp = -1, chance = 1;

    if (!mdef || !obj)
        return; /* just in case */
    /* AD_ACID and AD_ENCH are handled in passivemm() and passiveum() */
    if (dmgtype(mdef->data, AD_CORR)) 
    {
        dmgtyp = ERODE_CORRODE;
    } 
    else if (dmgtype(mdef->data, AD_RUST))
    {
        dmgtyp = ERODE_RUST;
    } 
    else if (dmgtype(mdef->data, AD_FIRE)
               /* steam vortex: fire resist applies, fire damage doesn't */
               && mdef->data != &mons[PM_STEAM_VORTEX])
    {
        dmgtyp = ERODE_BURN;
        chance = 6;
    }

    if (dmgtyp >= 0 && !rn2(chance))
        (void) erode_obj(obj, (char *) 0, dmgtyp, EF_GREASE | EF_VERBOSE);
}

STATIC_OVL void
mswingsm(magr, mdef, otemp)
struct monst *magr, *mdef;
struct obj *otemp;
{
    if (flags.verbose && !Blind && mon_visible(magr)) 
    {
        pline("%s %s %s%s %s at %s.", Monnam(magr),
              (objects[otemp->otyp].oc_dir & PIERCE) ? "thrusts" : "swings",
              (otemp->quan > 1L) ? "one of " : "", mhis(magr), xname(otemp),
              mon_nam(mdef));
    }
}

/*
 * Passive responses by defenders.  Does not replicate responses already
 * handled above.  Returns same values as mattackm.
 */
STATIC_OVL int
passivemm(magr, mdef, mhit, mdead)
register struct monst *magr, *mdef;
boolean mhit;
int mdead;
{
    if (!magr || !mdef)
        return 0;

    register struct permonst *mddat = mdef->data;
    register struct permonst *madat = magr->data;
    char buf[BUFSZ];
    int i;
    double damage = 0;
    int basedmg = 0;
    enum hit_tile_types hit_tile = HIT_GENERAL;

    for (i = 0;; i++) 
    {
        if (i >= NATTK)
            return (mdead | mhit); /* no passive attacks */
        if (mddat->mattk[i].aatyp == AT_PASV)
            break;
    }

    if (mddat->mattk[i].damn > 0 || mddat->mattk[i].damd > 0)
        basedmg = max(0, d(mddat->mattk[i].damn > 0 ? mddat->mattk[i].damn : mddat->mlevel / 2 + 2, mddat->mattk[i].damd > 0 ? mddat->mattk[i].damd : 6) + (int)mddat->mattk[i].damp);
    else
        basedmg = max(0, mddat->mattk[i].damp);

    damage += adjust_damage(basedmg, mdef, magr, mddat->mattk[i].adtyp, ADFLAGS_NONE); /* Defender deals damage in passive defense */
    
    enum action_tile_types action_before = mdef->action;
    update_m_action(mdef, mddat->mattk[i].action_tile ? mddat->mattk[i].action_tile : ACTION_TILE_PASSIVE_DEFENSE);
    play_monster_simple_weapon_sound(mdef, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
    m_wait_until_action();

    /* These affect the enemy even if defender killed */
    switch (mddat->mattk[i].adtyp) 
    {
    case AD_ACID:
        hit_tile = HIT_SPLASHED_ACID;
        if (mhit && !rn2(2))
        {
            play_sfx_sound_at_location(SFX_MONSTER_GETS_SPLASHED_BY_ACID, mdef->mx, mdef->my);
            Strcpy(buf, Monnam(magr));
            if (canseemon(magr))
                pline("%s is splashed by %s %s!", buf,
                      s_suffix(mon_nam(mdef)), hliquid("acid"));
            if (is_mon_immune_to_acid(magr)) 
            {
                if (canseemon(magr))
                {
                    play_sfx_sound_at_location(SFX_GENERAL_UNHARMED, magr->mx, magr->my);
                    pline("%s is not affected.", Monnam(magr));
                }
                damage = 0;
            }
        }
        else
            damage = 0;

        if (!rn2(30))
            erode_armor(magr, ERODE_CORRODE);
        if (!rn2(6))
            acid_damage(MON_WEP(magr));
        goto assess_dmg;
    case AD_ENCH: /* KMH -- remove enchantment (disenchanter) */
        if (mhit && !is_cancelled(mdef)&& otmp) 
        {
            (void) drain_item(otmp, FALSE);
            /* No message */
        }
        break;
    default:
        break;
    }
    if (mdead || is_cancelled(mdef))
    {
        update_m_action_core(mdef, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
        return (mdead | mhit);
    }

    /* These affect the enemy only if defender is still alive */
    if (rn2(3))
        switch (mddat->mattk[i].adtyp) 
        {
        case AD_PLYS: /* Floating eye */
            if (basedmg > 127)
                basedmg = 127;
            if (mddat == &mons[PM_FLOATING_EYE]) 
            {
                if (!rn2(20))
                    basedmg = 48;
                if (!is_blinded(magr) && haseyes(madat) && !is_blinded(mdef)
                    && (is_invisible(magr) || !is_invisible(mdef))) 
                {
                    /* construct format string; guard against '%' in Monnam */
                    Strcpy(buf, s_suffix(Monnam(mdef)));
                    (void) strNsubst(buf, "%", "%%", 0);
                    Strcat(buf, " gaze is reflected by %s %s.");

                    if (mon_reflects(magr, canseemon(magr) ? buf : (char*)0))
                    {
                        play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mdef->mx, mdef->my);
                        update_m_action_core(mdef, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                        return (mdead | mhit);
                    }
                    Strcpy(buf, Monnam(magr));

                    if(!resists_paralysis(mdef))
                    {
                        if (canseemon(magr))
                            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is frozen by %s gaze!", buf,
                                  s_suffix(mon_nam(mdef)));
                        play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mdef->mx, mdef->my);
                        paralyze_monst(magr, basedmg, FALSE);
                    }
                    update_m_action_core(mdef, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                    return (mdead | mhit);
                }
            }
            else 
            { /* gelatinous cube */
                Strcpy(buf, Monnam(magr));
                if (canseemon(magr))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is frozen by %s.", buf, mon_nam(mdef));
                play_sfx_sound_at_location(SFX_ACQUIRE_PARALYSIS, mdef->mx, mdef->my);
                paralyze_monst(magr, basedmg, FALSE);
                update_m_action_core(mdef, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                return (mdead | mhit);
            }
            return 1;
        case AD_COLD:
            hit_tile = HIT_FROZEN;
            if (is_mon_immune_to_cold(magr))
            {
                if (canseemon(magr)) 
                {
                    pline("%s is mildly chilly.", Monnam(magr));
                    golemeffects(magr, AD_COLD, damage);
                }
                damage = 0;
                break;
            }
            if (canseemon(magr))
            {
                play_sfx_sound_at_location(SFX_MONSTER_COVERED_IN_FROST, mdef->mx, mdef->my);
                pline("%s is suddenly very cold!", Monnam(magr));
            }
            mdef->mhp += (int)ceil(damage) / 2;
            if (mdef->mhpmax < mdef->mhp)
                mdef->mbasehpmax += (mdef->mhp - mdef->mhpmax);
            update_mon_maxhp(mdef);
            if (mdef->mhp > mdef->mhpmax)
                mdef->mhp = mdef->mhpmax;
            if (mdef->mhpmax > ((int) (mdef->m_lev + 1) * 8))
                (void) split_mon(mdef, magr);
            break;
        case AD_STUN:
            if (!is_stunned(magr) || !resists_stun(magr)) 
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_STUN, mdef->mx, mdef->my);
                nonadditive_increase_mon_property(magr, STUNNED, 5 + rnd(5));
                if (canseemon(magr))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s...", Monnam(magr),
                          makeplural(stagger(magr->data, "stagger")));
            }
            damage = 0;
            break;
        case AD_FIRE:
            hit_tile = HIT_ON_FIRE;
            if (is_mon_immune_to_fire(magr))
            {
                if (canseemon(magr)) 
                {
                    if (flaming(mdef->data))
                        pline("%s is engulfed in %s flames, but they do not burn %s.", Monnam(magr), s_suffix(mon_nam(mdef)), mon_nam(magr));
                    else
                        pline("%s is mildly warmed.", Monnam(magr));
                    golemeffects(magr, AD_FIRE, damage);
                }
                damage = 0;
                break;
            }
            if (canseemon(magr))
            {
                play_sfx_sound_at_location(SFX_MONSTER_ON_FIRE, mdef->mx, mdef->my);
                if (flaming(mdef->data))
                    pline("%s is engulfed in %s flames!", Monnam(magr), s_suffix(mon_nam(mdef)));
                else
                    pline("%s is suddenly very hot!", Monnam(magr));
            }
            break;
        case AD_ELEC:
            hit_tile = HIT_ELECTROCUTED;
            if (is_mon_immune_to_elec(magr))
            {
                if (canseemon(magr)) {
                    pline("%s is mildly tingled.", Monnam(magr));
                    golemeffects(magr, AD_ELEC, damage);
                }
                damage = 0;
                break;
            }
            if (canseemon(magr))
            {
                play_sfx_sound_at_location(SFX_MONSTER_GETS_ZAPPED, mdef->mx, mdef->my);
                pline("%s is jolted with electricity!", Monnam(magr));
            }
            break;
        default:
            damage = 0;
            break;
        }
    else
    damage = 0;

assess_dmg:
    {
        int mhp_before = magr->mhp;
        deduct_monster_hp(magr, damage); //(magr->mhp -= tmp);
        int mhp_after = magr->mhp;
        int damagedealt = mhp_before - mhp_after;

        if (canseemon(magr) && damagedealt > 0)
        {
            pline("%s sustains %d damage!", Monnam(magr), damagedealt);
            display_m_being_hit(magr, hit_tile, damagedealt, 0UL, FALSE);
        }

        if (magr->mhp <= 0)
        {
            monkilled(magr, "", (int) mddat->mattk[i].adtyp);
            update_m_action_core(mdef, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
            return (mdead | mhit | MM_AGR_DIED);
        }
    }
    update_m_action_core(mdef, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    return (mdead | mhit);
}

/* hero or monster has successfully hit target mon with drain energy attack */
void
xdrainenergym(mon, givemsg)
struct monst *mon;
boolean givemsg;
{
    if (mon->mspec_used < 20 /* limit draining */
        && (attacktype(mon->data, AT_MAGC)
            || attacktype(mon->data, AT_BREA))) 
    {
        mon->mspec_used += d(2, 2);
        if (givemsg)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems lethargic.", Monnam(mon));
    }
}

/* "aggressive defense"; what type of armor prevents specified attack
   from touching its target? */
long
attk_protection(aatyp)
int aatyp;
{
    long w_mask = 0L;

    switch (aatyp) 
    {
    case AT_NONE:
    case AT_PASV:
    case AT_SPIT:
    case AT_EXPL:
    case AT_BOOM:
    case AT_GAZE:
    case AT_BREA:
    case AT_EYES:
    case AT_SMMN:
    case AT_MAGC:
        w_mask = ~0L; /* special case; no defense needed */
        break;
    case AT_CLAW:
    case AT_TUCH:
    case AT_WEAP:
        w_mask = W_ARMG; /* caller needs to check for weapon */
        break;
    case AT_KICK:
        w_mask = W_ARMF;
        break;
    case AT_BUTT:
    case AT_HORN:
    case AT_TUSK:
        w_mask = W_ARMH;
        break;
    case AT_HUGS:
        w_mask = (W_ARMC | W_ARMG); /* attacker needs both to be protected */
        break;
    case AT_BITE:
    case AT_RAMS:
    case AT_TAIL:
    case AT_STNG:
    case AT_ENGL:
    case AT_TENT:
    default:
        w_mask = 0L; /* no defense available */
        break;
    }
    return w_mask;
}

/*mhitm.c*/

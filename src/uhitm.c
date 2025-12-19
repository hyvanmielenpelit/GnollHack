/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    uhitm.c    $NHDT-Date: 1555720104 2019/04/20 00:28:24 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.207 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

STATIC_DCL boolean FDECL(known_hitum, (struct monst *, struct obj *, int *,
                                       int, int, struct attack *, int));
STATIC_DCL boolean FDECL(theft_petrifies, (struct obj *));
STATIC_DCL void FDECL(steal_it, (struct monst *, struct attack *));
STATIC_DCL boolean FDECL(hitum_cleave, (struct monst *, struct attack *, struct obj*));
STATIC_DCL boolean FDECL(hitum, (struct monst *, struct attack *));
STATIC_DCL boolean FDECL(hmon_hitmon, (struct monst *, struct obj *, int,
                                       int, boolean *));
STATIC_DCL int FDECL(joust, (struct monst *, struct obj *));
STATIC_DCL void NDECL(demonpet);
STATIC_DCL int FDECL(explum, (struct monst *, struct attack *));
STATIC_DCL void FDECL(start_engulf, (struct monst *));
STATIC_DCL void NDECL(end_engulf);
STATIC_DCL int FDECL(gulpum, (struct monst *, struct attack *));
STATIC_DCL boolean FDECL(hmonas, (struct monst *));
STATIC_DCL void FDECL(nohandglow, (struct monst *));
STATIC_DCL boolean FDECL(shade_aware, (struct obj *));

extern boolean notonhead; /* for long worms */

/* Used to flag attacks caused by Stormbringer's maliciousness. */
static boolean override_confirmation = FALSE;

#define PROJECTILE(obj) ((obj) && is_ammo(obj))

void
erode_armor(mdef, hurt)
struct monst *mdef;
int hurt;
{
    struct obj *target;

    /* What the following code does: it keeps looping until it
     * finds a target for the rust monster.
     * Head, feet, etc... not covered by metal, or covered by
     * rusty metal, are not targets.  However, your body always
     * is, no matter what covers it.
     */
    while (1) {
        switch (rn2(5)) {
        case 0:
            target = which_armor(mdef, W_ARMH);
            if (!target
                || erode_obj(target, xname(target), hurt, EF_GREASE)
                       == ER_NOTHING)
                continue;
            break;
        case 1:
            target = which_armor(mdef, W_ARMC);
            if (target) {
                (void) erode_obj(target, xname(target), hurt,
                                 EF_GREASE | EF_VERBOSE);
                break;
            }
            if ((target = which_armor(mdef, W_ARM)) != (struct obj *) 0) {
                (void) erode_obj(target, xname(target), hurt,
                                 EF_GREASE | EF_VERBOSE);
            } else if ((target = which_armor(mdef, W_ARMU))
                       != (struct obj *) 0) {
                (void) erode_obj(target, xname(target), hurt,
                                 EF_GREASE | EF_VERBOSE);
            }
            break;
        case 2:
            target = which_armor(mdef, W_ARMS);
            if (!target
                || erode_obj(target, xname(target), hurt, EF_GREASE)
                       == ER_NOTHING)
                continue;
            break;
        case 3:
            target = which_armor(mdef, W_ARMG);
            if (!target
                || erode_obj(target, xname(target), hurt, EF_GREASE)
                       == ER_NOTHING)
                continue;
            break;
        case 4:
            target = which_armor(mdef, W_ARMF);
            if (!target
                || erode_obj(target, xname(target), hurt, EF_GREASE)
                       == ER_NOTHING)
                continue;
            break;
        }
        break; /* Out of while loop */
    }
}

/* FALSE means it's OK to attack */
boolean
attack_checks(mtmp, wep)
register struct monst *mtmp;
struct obj *wep; /* uwep for attack(), null for kick_monster() */
{
    int glyph;

    /* if you're close enough to attack, alert any waiting monster */
    mtmp->mstrategy &= ~STRAT_WAITMASK;

    if (u.uswallow && mtmp == u.ustuck)
        return FALSE;

    if (context.forcefight) {
        /* Do this in the caller, after we checked that the monster
         * didn't die from the blow.  Reason: putting the 'I' there
         * causes the hero to forget the square's contents since
         * both 'I' and remembered contents are stored in .glyph.
         * If the monster dies immediately from the blow, the 'I' will
         * not stay there, so the player will have suddenly forgotten
         * the square's contents for no apparent reason.
        if (!canspotmon(mtmp)
            && !glyph_is_invisible(levl[bhitpos.x][bhitpos.y].hero_memory_layers.glyph))
            map_invisible(bhitpos.x, bhitpos.y);
         */
        return FALSE;
    }

    /* cache the shown glyph;
       cases which might change it (by placing or removing
       'rembered, unseen monster' glyph or revealing a mimic)
       always return without further reference to this */
    glyph = glyph_at(bhitpos.x, bhitpos.y);

    /* Put up an invisible monster marker, but with exceptions for
     * monsters that hide and monsters you've been warned about.
     * The former already prints a warning message and
     * prevents you from hitting the monster just via the hidden monster
     * code below; if we also did that here, similar behavior would be
     * happening two turns in a row.  The latter shows a glyph on
     * the screen, so you know something is there.
     */
    if (!canspotmon(mtmp)
        && !glyph_is_warning(glyph) && !glyph_is_invisible(glyph)
        && !(!Blind && mtmp->mundetected && hides_under(mtmp->data))) {
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "Wait!  There's %s there you can't see!", something);
        map_invisible(bhitpos.x, bhitpos.y);
        /* if it was an invisible mimic, treat it as if we stumbled
         * onto a visible mimic
         */
        if (M_AP_TYPE(mtmp) && !Protection_from_shape_changers
            /* applied pole-arm attack is too far to get stuck */
            && distu(mtmp->mx, mtmp->my) <= 2) {
            if (!u.ustuck && !is_fleeing(mtmp) && check_stuck_and_slip(mtmp))
            {
                play_sfx_sound(SFX_ACQUIRE_GRAB);
                u.ustuck = mtmp;
                refresh_m_tile_gui_info(mtmp, FALSE);
            }
        }
        /* #H7329 - if hero is on engraved "Elbereth", this will end up
         * assessing an alignment penalty and removing the engraving
         * even though no attack actually occurs.  Since it also angers
         * peacefuls, we're operating as if an attack attempt did occur
         * and the Elbereth behavior is consistent.
         */
        wakeup(mtmp, TRUE); /* always necessary; also un-mimics mimics */
        return TRUE;
    }

    if (M_AP_TYPE(mtmp) && !Protection_from_shape_changers && !sensemon(mtmp)
        && !glyph_is_warning(glyph)) {
        /* If a hidden mimic was in a square where a player remembers
         * some (probably different) unseen monster, the player is in
         * luck--he attacks it even though it's hidden.
         */
        if (glyph_is_invisible(glyph)) {
            seemimic(mtmp);
            return FALSE;
        }
        stumble_onto_mimic(mtmp);
        return TRUE;
    }

    if (mtmp->mundetected && !canseemon(mtmp)
        && !glyph_is_warning(glyph)
        && (hides_under(mtmp->data) || mtmp->data->mlet == S_EEL)) {
        mtmp->mundetected = mtmp->msleeping = 0;
        newsym(mtmp->mx, mtmp->my);
        if (glyph_is_invisible(glyph)) {
            seemimic(mtmp);
            return FALSE;
        }
        if (!((Blind ? (Blind_telepat || Unblind_telepat) : Unblind_telepat) || Detect_monsters)) {
            struct obj *obj;

            play_sfx_sound(SFX_WAS_HIDING);
            if (!Blind && Hallucination)
                pline("A %s %s appeared!",
                      is_tame(mtmp) ? (call_mon_tame(mtmp) ? "allied" : "tame") : "wild", l_monnam(mtmp));
            else if (Blind || (is_pool(mtmp->mx, mtmp->my) && !Underwater))
                pline("Wait!  There's a hidden monster there!");
            else if ((obj = level.objects[mtmp->mx][mtmp->my]) != 0)
                pline("Wait!  There's %s hiding under %s!",
                      an(l_monnam(mtmp)), doname(obj));
            return TRUE;
        }
    }

    /*
     * make sure to wake up a monster from the above cases if the
     * hero can sense that the monster is there.
     */
    if ((mtmp->mundetected || M_AP_TYPE(mtmp)) && sensemon(mtmp)) {
        mtmp->mundetected = 0;
        wakeup(mtmp, TRUE);
    }

    if (flags.confirm && is_peaceful(mtmp)
        && !Confusion && !Hallucination && !Stunned) {
        /* Intelligent chaotic weapons (Stormbringer) want blood */
        if (wep && wep->oartifact && artifact_has_flag(wep, AF_BLOODTHIRSTY) && !is_tame(mtmp)) 
        {
            override_confirmation = TRUE;
            return FALSE;
        }
        if (canspotmon(mtmp)) {
            char qbuf[QBUFSZ];

            Sprintf(qbuf, "Really attack %s?", mon_nam(mtmp));
            if (!paranoid_query(ParanoidHit, qbuf)) {
                context.move = 0;
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*
 * It is unchivalrous for a knight to attack the defenseless or from behind.
 */
void
check_caitiff(mtmp)
struct monst *mtmp;
{
    if (u.ualign.record <= -10)
        return;

    if (Role_if(PM_KNIGHT) && u.ualign.type == A_LAWFUL
        && (!mon_can_move(mtmp)
            || (is_fleeing(mtmp) && !mtmp->mavenge && mtmp != u.ustuck))) 
    {
        play_sfx_sound(SFX_CAITIFF);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "caitiff!");
        adjalign(-1);
    } 
    else if (Role_if(PM_SAMURAI) && is_peaceful(mtmp)) 
    {
        /* attacking peaceful creatures is bad for the samurai's giri */
        play_sfx_sound(SFX_CAITIFF);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "dishonorably attack the innocent!");
        adjalign(-1);
    }
}

int
find_roll_to_hit(mtmp, aatyp, weapon, attk_count, role_roll_penalty)
register struct monst *mtmp;
uchar aatyp;        /* usually AT_WEAP or AT_KICK */
struct obj *weapon; /* uwep or uarms or NULL */
int *attk_count, *role_roll_penalty;
{
    int tmp, tmp2;
    int polytmp = 0, nonpolytmp = 0;
    int weaponskill = weapon_skill_type(weapon);
    if (weaponskill == P_MARTIAL_ARTS)
        weaponskill = P_BARE_HANDED_COMBAT;
    //int skill_level = P_SKILL_LEVEL(weaponskill);
    double skill_multiplier = 1.0;

    if (Upolyd && aatyp != AT_WEAP && !weapon)
    {
        skill_multiplier = 1.0;
    }
    else if (weaponskill == P_MARTIAL_ARTS && !uarm && !uarms)
    {
        skill_multiplier = 1.0;
    }
    else if(weaponskill != P_NONE)
    {
        skill_multiplier = TO_HIT_LEVEL_MULTIPLIER;
    }
    *role_roll_penalty = 0; /* default is `none' */

    polytmp = youmonst.data->mlevel;
    nonpolytmp = (int)(skill_multiplier * (double)u.ulevel);
    tmp = 1 + Luck + u_strdex_to_hit_bonus() + find_mac(mtmp) + u.ubasehitinc + u.uhitinc;

    if (mtmp != &youmonst)
    {
        if (weapon && nonmelee_throwing_weapon(weapon))
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "It is complicated to hit with %s in melee combat.", the(cxname(weapon)));
            tmp -= THROWING_WEAPON_TO_HIT_MELEE_PENALTY_WHEN_USED_AS_MELEE_WEAPON;
        }

        /* some actions should occur only once during multiple attacks */
        if (!(*attk_count)++)
        {
            /* knight's chivalry or samurai's giri */
            check_caitiff(mtmp);
        }

        /* adjust vs. (and possibly modify) monster state */
        if (is_stunned(mtmp))
            tmp += 2;
        if (is_fleeing(mtmp))
            tmp += 2;

        boolean refresh = FALSE;
        if (mtmp->msleeping)
        {
            mtmp->msleeping = 0;
            refresh = TRUE;
            tmp += 2;
        }

        if (!mtmp->mcanmove)
        {
            tmp += 4;
            if (!rn2(10))
            {
                mtmp->mcanmove = 1;
                mtmp->mfrozen = 0;
                refresh = TRUE;
            }
        }

        if(refresh)
            refresh_m_tile_gui_info(mtmp, TRUE);
    }
    /* role/race adjustments */
    if (Role_if(PM_MONK) && !Upolyd)
    {
        if (uarm)
            tmp -= (*role_roll_penalty = urole.spelarmr);
        else if (!uwep && !uarms)
            tmp += (u.ulevel / 3) + 2;
    }

    if (is_orc(mtmp->data)
        && maybe_polyd(is_elf(youmonst.data), Race_if(PM_ELF)))
        tmp++;

    /* encumbrance: with a lot of luggage, your agility diminishes */
    if ((tmp2 = near_capacity()) != 0)
        tmp -= (tmp2 * 2) - 1;
    if (u.utrap)
        tmp -= 3;

    /*
     * weapon_to_hit_value applies if making a weapon attack while wielding a weapon;
     * weapon_skill_hit_bonus applies if doing a weapon attack even bare-handed
     * or if kicking as martial artist
     */
    if (aatyp == AT_WEAP || aatyp == AT_CLAW)
    {
        if (weapon)
            tmp += weapon_to_hit_value(weapon, mtmp, &youmonst, 0);
        else if(uarmg)
            tmp += weapon_to_hit_value(uarmg, mtmp, &youmonst, 0);
        nonpolytmp += weapon_skill_hit_bonus(weapon, P_NONE, FALSE, TRUE, TRUE, 0, TRUE, FALSE);
    } 
    else if (aatyp == AT_KICK && martial_bonus()) 
    {
        if (weapon)
            tmp += weapon_to_hit_value(weapon, mtmp, &youmonst, 0);
        else if (uarmf)
            tmp += weapon_to_hit_value(uarmf, mtmp, &youmonst, 0);
        nonpolytmp += weapon_skill_hit_bonus((struct obj *) 0, P_NONE, FALSE, TRUE, TRUE, 0, TRUE, FALSE);
    }

    tmp += maybe_polyd(max(polytmp, nonpolytmp), nonpolytmp);

    return tmp;
}

/* try to attack; return False if monster evaded;
   u.dx and u.dy must be set */
boolean
attack(mtmp)
register struct monst *mtmp;
{
    if (!mtmp)
        return FALSE;

    update_u_facing(TRUE);

    register struct permonst *mdat = mtmp->data;

    /* This section of code provides protection against accidentally
     * hitting peaceful (like '@') and tame (like 'd') monsters.
     * Protection is provided as long as player is not: blind, confused,
     * hallucinating or stunned.
     * changes by wwp 5/16/85
     * More changes 12/90, -dkh-. if its tame and safepet, (and protected
     * 07/92) then we assume that you're not trying to attack. Instead,
     * you'll usually just swap places if this is a movement command
     */
    /* Intelligent chaotic weapons (Stormbringer) want blood */
    if ((is_safepet(mtmp) || is_displaceable_peaceful(mtmp) || is_stopping_peaceful(mtmp)) && !context.forcefight)
    {
        //if (1) //!uwep || !(uwep->oartifact && artifact_has_flag(uwep, AF_BLOODTHIRSTY)))  // Always true
        {
            /* There are some additional considerations: this won't work
             * if in a shop or Punished or you miss a random roll or
             * if you can walk thru walls and your pet cannot (KAA) or
             * if your pet is a long worm with a tail.
             * There's also a chance of displacing a "frozen" monster:
             * sleeping monsters might magically walk in their sleep.
             */
            boolean foo = (Punished || !rn2(7) || (is_long_worm(mtmp->data) && mtmp->wormno));
            boolean inshop = FALSE;
            boolean peacefulstop = is_stopping_peaceful(mtmp);
            char *p;

            /* only check for in-shop if don't already have reason to stop */
            if (!foo) 
            {
                for (p = in_rooms(mtmp->mx, mtmp->my, SHOPBASE); *p; p++)
                    if (tended_shop(&rooms[*p - ROOMOFFSET])) 
                    {
                        inshop = TRUE;
                        break;
                    }
            }

            if (inshop || foo || peacefulstop || (IS_ROCK(levl[u.ux][u.uy].typ) && !passes_walls(mtmp->data)))
            {
                char buf[BUFSZ];

                monflee(mtmp, rnd(6), FALSE, FALSE);
                Strcpy(buf, y_monnam(mtmp));
                buf[0] = highc(buf[0]);
                play_sfx_sound(SFX_SOMETHING_IN_WAY);
                You("stop.  %s is in the way!", buf);
                clear_run_and_travel();
                return TRUE;
            } 
            else if ((!mon_can_move(mtmp) || (mtmp->data->mmove == 0)) && rn2(6)) 
            {
                play_sfx_sound(SFX_PET_DOES_NOT_MOVE);
                pline("%s doesn't seem to move!", Monnam(mtmp));
                clear_run_and_travel();
                return TRUE;
            }
            else
                return FALSE;
        }
    }

    /* possibly set in attack_checks;
       examined in known_hitum, called via hitum or hmonas below */
    override_confirmation = FALSE;
    /* attack_checks() used to use <u.ux+u.dx,u.uy+u.dy> directly, now
       it uses bhitpos instead; it might map an invisible monster there */
    bhitpos.x = u.ux + u.dx;
    bhitpos.y = u.uy + u.dy;
    notonhead = (bhitpos.x != mtmp->mx || bhitpos.y != mtmp->my);
    if (attack_checks(mtmp, uwep))
        return TRUE;

    if (Upolyd && noattacks(youmonst.data)) {
        /* certain "pacifist" monsters don't attack */
        You("have no way to attack monsters physically.");
        mtmp->mstrategy &= ~STRAT_WAITMASK;
        goto atk_done;
    }

    if (check_capacity("You cannot fight while so heavily loaded.")
        /* consume extra nutrition during combat; maybe pass out */
        || overexertion())
        goto atk_done;

    if (iflags.autoswap_launchers && unweapon1 && uwep && is_launcher(uwep) && ((uswapwep && !is_unweapon(uswapwep)) || (!uswapwep && Role_if(PM_MONK))))
    {
        boolean cursed_weapon_blocks_swap = (uwep && objects[uwep->otyp].oc_bimanual) || (uwep && uarms && !flags.swap_rhand_only) ? ((uwep && welded(uwep, &youmonst)) || (uarms && welded(uarms, &youmonst))) : (uwep && welded(uwep, &youmonst));
        if (!cursed_weapon_blocks_swap)
        {
            if (uwep && objects[uwep->otyp].oc_bimanual)
                (void)doswapweapon();
            else
                (void)doswapweapon_right_or_both();
            update_unweapon();
        }
    }

    char qbuf[BUFSZ * 2];
    Strcpy(qbuf, "");
    if (unweapon1 && unweapon2)
    {
        if (flags.verbose) 
        {
            if (uwep && uarms)
                Sprintf(qbuf, "You begin bashing monsters with %s and %s. Continue?", yname(uwep), cxname(uarms));
            else if (uwep)
                Sprintf(qbuf, "You begin bashing monsters with %s and %s left %s. Continue?", yname(uwep), uarmg ? "gloved" : "bare", body_part(HAND));
            else if (uarms)
                Sprintf(qbuf, "You begin bashing monsters with %s and %s right %s. Continue?", yname(uarms), uarmg ? "gloved" : "bare", body_part(HAND));
            else if (!cantwield(youmonst.data))
                Sprintf(qbuf, "You begin %s monsters with your %s %s. Continue?",
                    ing_suffix(Role_if(PM_MONK) ? "strike" : "bash"),
                    uarmg ? "gloved" : "bare", /* Del Lamb */
                    makeplural(body_part(HAND)));
        }
    }
    else if (unweapon1)
    {
        if (flags.verbose)
        {
            if (uwep)
                Sprintf(qbuf, "You begin bashing monsters with %s. Continue?", yname(uwep));
            else if (!cantwield(youmonst.data))
                Sprintf(qbuf, "You begin %s monsters with your %s %s%s. Continue?",
                    ing_suffix(Role_if(PM_MONK) ? "strike" : "bash"),
                    uarmg ? "gloved" : "bare", /* Del Lamb */
                    u.twoweap ? "right " : "",
                    u.twoweap ? body_part(HAND) : makeplural(body_part(HAND)));

        }
    }
    else if (unweapon2)
    {
        if (flags.verbose)
        {
            if (uarms)
                Sprintf(qbuf, "You begin bashing monsters with %s. Continue?", yname(uarms));
            else if (!cantwield(youmonst.data))
                Sprintf(qbuf, "You begin %s monsters with your %s left %s. Continue?",
                    ing_suffix(Role_if(PM_MONK) ? "strike" : "bash"),
                    uarmg ? "gloved" : "bare", /* Del Lamb */
                    body_part(HAND));
        }
    }

    boolean do_bash_prompt = strcmp(qbuf, "") != 0;
    if (do_bash_prompt)
    {
        char ans = yn_query(qbuf);
        if (ans == 'n')
        {
            nomul(0);
            context.move = FALSE;
            return TRUE;
        }
        else
            unweapon1 = unweapon2 = FALSE;
    }

    boolean has_right_weapon = (uwep && is_weapon(uwep));
    boolean has_left_weapon = (u.twoweap && uwep2 && is_weapon(uwep2));
    if (/* !do_bash_prompt && */ ParanoidWieldedWeapon && u.uconduct.weaphit == 0 && (has_right_weapon || has_left_weapon))
    {
        if(has_right_weapon && !has_left_weapon)
            Sprintf(qbuf, "You begin attacking with %s. Continue?", yname(uwep));
        else if (!has_right_weapon && has_left_weapon)
            Sprintf(qbuf, "You begin attacking with %s. Continue?", yname(uwep2));
        else if (has_right_weapon && has_left_weapon)
            Sprintf(qbuf, "You begin attacking with %s and %s. Continue?", yname(uwep), cxname(uwep2));
        else
            Sprintf(qbuf, "You begin attacking with %s. Continue?", "weapons");
        char ans = yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Attacking with Weapon", qbuf);
        if (ans != 'y')
        {
            nomul(0);
            context.move = FALSE;
            return TRUE;
        }
    }

    exercise(A_STR, TRUE); /* you're exercising muscles */
    /* andrew@orca: prevent unlimited pick-axe attacks */
    u_wipe_engr(3);

    /* Is the "it died" check actually correct? */
    if (mdat->mlet == S_LEPRECHAUN && mon_can_move(mtmp)
        && !is_confused(mtmp) && !is_blinded(mtmp) && !rn2(7)
        && (m_move(mtmp, 0) == 2 /* it died */
            || mtmp->mx != u.ux + u.dx
            || mtmp->my != u.uy + u.dy)) { /* it moved */
        You("miss wildly and stumble forwards.");
        return FALSE;
    }

    if (Upolyd)
        (void) hmonas(mtmp);
    else
        (void) hitum(mtmp, youmonst.data->mattk);
    mtmp->mstrategy &= ~STRAT_WAITMASK;

 atk_done:
    /* see comment in attack_checks() */
    /* we only need to check for this if we did an attack_checks()
     * and it returned 0 (it's okay to attack), and the monster didn't
     * evade.
     */
    if (context.forcefight && !DEADMONSTER(mtmp) && !canspotmon(mtmp)
        && !glyph_is_invisible(levl[u.ux + u.dx][u.uy + u.dy].hero_memory_layers.glyph)
        && !(u.uswallow && mtmp == u.ustuck))
    {
        map_invisible(u.ux + u.dx, u.uy + u.dy);
    }

    return TRUE;
}

/* really hit target monster; returns TRUE if it still lives */
STATIC_OVL boolean
known_hitum(mon, weapon, mhit, rollneeded, armorpenalty, uattk, dieroll)
register struct monst *mon;
struct obj *weapon;
int *mhit;
int rollneeded, armorpenalty; /* for monks */
struct attack *uattk;
int dieroll;
{
    boolean malive = TRUE,
            /* hmon() might destroy weapon; remember aspect for cutworm */
            slice_or_chop = (weapon && (is_blade(weapon) || is_axe(weapon)));

    if (override_confirmation) 
    {
        if (flags.verbose && weapon)
            Your("bloodthirsty %s attacks!", is_blade(weapon) ? "blade" : weapon->oclass == WEAPON_CLASS ? weapon_type_names[objects[weapon->otyp].oc_subtyp] : "weapon");
    }

    if (!*mhit)
    {
        missum(mon, uattk, (rollneeded + armorpenalty > dieroll));
    }
    else
    {
        int oldhp = mon->mhp;
        int64_t oldweaphit = u.uconduct.weaphit;

        /* KMH, conduct */
        if (weapon && is_wieldable_weapon(weapon))
            u.uconduct.weaphit++; /* Livelog is below */

        /* we hit the monster; be careful: it might die or
           be knocked into a different location */
        notonhead = (mon->mx != bhitpos.x || mon->my != bhitpos.y);
        boolean obj_destroyed = FALSE;
        malive = hmon(mon, weapon, HMON_MELEE, dieroll, &obj_destroyed);
        if (obj_destroyed)
            weapon = 0;

        if (malive) 
        {
            /* monster still alive */
            if (!rn2(25) && mon->mhp < mon->mhpmax / 2 && !mindless(mon->data)
                && !(u.uswallow && mon == u.ustuck)) 
            {
                /* maybe should regurgitate if swallowed? */
                monflee(mon, !rn2(3) ? rnd(100) : 0, FALSE, TRUE);

                if (u.ustuck == mon && !u.uswallow && !sticks(youmonst.data))
                    u.ustuck = 0;
            }
            /* Vorpal Blade hit converted to miss */
            /* could be headless monster or worm tail */
            if (mon->mhp == oldhp) 
            {
                *mhit = 0;
                /* a miss does not break conduct */
                u.uconduct.weaphit = oldweaphit;
            }
            if (mon->wormno && *mhit)
                cutworm(mon, bhitpos.x, bhitpos.y, slice_or_chop);
        }
        if (u.uconduct.weaphit && !oldweaphit)
            livelog_printf(LL_CONDUCT, "%s",
                "hit with a wielded weapon for the first time");
    }
    return malive;
}

/* hit the monster next to you and the monsters to the left and right of it;
   return False if the primary target is killed, True otherwise */
STATIC_OVL boolean
hitum_cleave(target, uattk, weapon)
struct monst *target; /* non-Null; forcefight at nothing doesn't cleave... */
struct attack *uattk; /* ... but we don't enforce that here; Null works ok */
struct obj* weapon;
{
    /* swings will be delivered in alternate directions; with consecutive
       attacks it will simulate normal swing and backswing; when swings
       are non-consecutive, hero will sometimes start a series of attacks
       with a backswing--that doesn't impact actual play, just spoils the
       simulation attempt a bit */
    static boolean clockwise = FALSE;
    unsigned i;
    coord save_bhitpos;
    int count, umort, x = u.ux, y = u.uy;

    /* find the direction toward primary target */
    for (i = 0; i < 8; ++i)
        if (xdir[i] == u.dx && ydir[i] == u.dy)
            break;
    if (i == 8) {
        impossible("hitum_cleave: unknown target direction [%d,%d,%d]?",
                   u.dx, u.dy, u.dz);
        return TRUE; /* target hasn't been killed */
    }
    /* adjust direction by two so that loop's increment (for clockwise)
       or decrement (for counter-clockwise) will point at the spot next
       to primary target */
    i = (i + (clockwise ? 6 : 2)) % 8;
    umort = u.umortality; /* used to detect life-saving */
    save_bhitpos = bhitpos;

    /*
     * Three attacks:  adjacent to primary, primary, adjacent on other
     * side.  Primary target must be present or we wouldn't have gotten
     * here (forcefight at thin air won't 'cleave').  However, the
     * first attack might kill it (gas spore explosion, weak long worm
     * occupying both spots) so we don't assume that it's still present
     * on the second attack.
     */
    for (count = 3; count > 0; --count) {
        struct monst *mtmp;
        int tx, ty, tmp, dieroll, mhit, attknum, armorpenalty;

        /* ++i, wrap 8 to i=0 /or/ --i, wrap -1 to i=7 */
        i = (i + (clockwise ? 1 : 7)) % 8;

        tx = x + xdir[i], ty = y + ydir[i]; /* current target location */
        if (!isok(tx, ty))
            continue;
        mtmp = m_at(tx, ty);
        if (!mtmp) {
            if (glyph_is_invisible(levl[tx][ty].hero_memory_layers.glyph))
                (void) unmap_invisible(tx, ty);
            continue;
        }

        /* Hit only hostile monsters to make it better --JG */
        if (mtmp && (!is_peaceful(mtmp) || is_crazed(mtmp) || Conflict || mtmp == target))
        {
            tmp = find_roll_to_hit(mtmp, uattk->aatyp, weapon,
                &attknum, &armorpenalty);
            dieroll = rnd(20);
            mhit = (tmp > dieroll);
            bhitpos.x = tx, bhitpos.y = ty; /* normally set up by attack() */
            (void)known_hitum(mtmp, weapon, &mhit, tmp, armorpenalty,
                uattk, dieroll);
            (void)passive(mtmp, weapon, mhit, !DEADMONSTER(mtmp), AT_WEAP, !weapon);
        }
        /* stop attacking if weapon is gone or hero got killed and
           life-saved after passive counter-attack */
        if (!weapon || u.umortality > umort)
            break;
    }
    /* set up for next time */
    clockwise = !clockwise; /* alternate */
    bhitpos = save_bhitpos; /* in case somebody relies on bhitpos
                             * designating the primary target */

    /* return False if primary target died, True otherwise; note: if 'target'
       was nonNull upon entry then it's still nonNull even if *target died */
    return (target && DEADMONSTER(target)) ? FALSE : TRUE;
}

/* hit target monster; returns TRUE if it still lives */
STATIC_OVL boolean
hitum(mon, uattk)
struct monst *mon;
struct attack *uattk;
{
    boolean malive = TRUE, wep_was_destroyed = FALSE;
    int armorpenalty, attknum = 0, x = u.ux + u.dx, y = u.uy + u.dy;
    struct obj* wep = (uwep ? uwep : uarmg ? uarmg : (struct obj*)0);
    struct obj* wepbefore = wep;

    struct multishot_result msres = get_multishot_stats(&youmonst, wep, wep, FALSE);
    int wieldermultistrike = msres.wielder_attacks;
    int multistrike = msres.weapon_attacks;
    int wielderstrikeindex;
    int strikeindex;

    for (wielderstrikeindex = 0; wielderstrikeindex < wieldermultistrike; wielderstrikeindex++)
    {
        boolean breakloop = FALSE;
        if (wielderstrikeindex > 0)
        {
            char wielderstrikebuf[BUFSZ * 2] = "";
            if (uwep)
                Sprintf(wielderstrikebuf, "You strike with %s", yname(uwep));
            else if (u.twoweap)
                Sprintf(wielderstrikebuf, "You strike with your right %s%s", uarmg ? "gloved " : "", body_part(HAND));
            else
                Sprintf(wielderstrikebuf, "You strike");

            pline("%s %s!", wielderstrikebuf, wielderstrikeindex == 1 ? "a second time" : wielderstrikeindex == 2 ? "a third time" : "once more");
        }

        for (strikeindex = 0; strikeindex < multistrike; strikeindex++)
        {
            int mx = mon->mx, my = mon->my;
            boolean was_invis_glyph = isok(mx, my) && glyph_is_invisible(levl[mx][my].hero_memory_layers.glyph);

            update_u_action(ACTION_TILE_ATTACK);
            play_monster_simple_weapon_sound(&youmonst, 0, wep, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();

            if (strikeindex > 0 && uwep)
            {
                pline("%s %s!", Yobjnam2(uwep, "strike"), strikeindex == 1 ? "a second time" : strikeindex == 2 ? "a third time" : "once more");
            }

            //DETERMINE IF YOU HIT THE MONSTER
            int tmp = find_roll_to_hit(mon, uattk->aatyp, wep, &attknum, &armorpenalty);
            int dieroll = rnd(20);
            int mhit = (tmp > dieroll || u.uswallow);
            boolean uses_spell_flags = wep ? object_uses_spellbook_wand_flags_and_properties(wep) : FALSE;

            if (wep && !uses_spell_flags && ((objects[wep->otyp].oc_aflags2 & A2_HITS_ADJACENT_SQUARES) || (wep->oartifact && artifact_has_flag(wep, AF_HITS_ADJACENT_SQUARES)))
                && !u.uswallow && !u.ustuck && !NODIAG(u.umonnum))
            {
                malive = hitum_cleave(mon, uattk, wep);
                if (wepbefore && !wep)
                    wep_was_destroyed = TRUE;
            }
            else
            {
                if (tmp > dieroll)
                    exercise(A_DEX, TRUE);
                /* bhitpos is set up by caller */
                malive = known_hitum(mon, wep, &mhit, tmp, armorpenalty, uattk, dieroll);
                if (wepbefore && !wep)
                    wep_was_destroyed = TRUE;
                (void)passive(mon, wep, mhit, malive, AT_WEAP, wep_was_destroyed);
            }

            boolean play_fade_animation = (windowprocs.wincap2 & WC2_FADING_ANIMATIONS) != 0 && was_invis_glyph && !malive && !glyph_is_invisible(levl[mx][my].hero_memory_layers.glyph);
            if (play_fade_animation)
            {
                play_special_effect_with_details_at(0, mx, my, GLYPH_INVISIBLE, LAYER_GENERAL_EFFECT, -2, 0, 0, 20, FALSE);
            }
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            if (play_fade_animation)
            {
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }

            if (!malive || m_at(x, y) != mon || wep_was_destroyed)
            {
                breakloop = TRUE;
                break;
            }
        }
        if (breakloop)
            break;
    }

    /* second attack for two-weapon combat */
    wep_was_destroyed = FALSE;

    if (u.twoweap && !(uwep && bimanual(uwep)) && malive && m_at(x, y) == mon) //&& !override_confirmation 
    {
        if (uarms && is_wieldable_weapon(uarms))
            You("strike with your left-hand %s.", is_shield(uarms) ? "shield" : "weapon");
        else
            You("strike with your left %s%s.", uarmg ? "gloved " : "", body_part(HAND));

        wep = (uarms ? uarms : uarmg ? uarmg : (struct obj*)0);
        wepbefore = wep;

        struct multishot_result msres2 = get_multishot_stats(&youmonst, wep, wep, FALSE);
        int wieldermultistrike2 = msres2.wielder_attacks;
        int multistrike2 = msres2.weapon_attacks;
        int wielderstrike2index;
        for (wielderstrike2index = 0; wielderstrike2index < wieldermultistrike2; wielderstrike2index++)
        {
            boolean breakloop = FALSE;
            if (wielderstrike2index > 0)
            {
                char wielderstrikebuf[BUFSZ * 2] = "";
                if (uarms)
                    Sprintf(wielderstrikebuf, "You strike with %s", yname(uarms));
                else
                    Sprintf(wielderstrikebuf, "You strike with your left %s%s", uarmg ? "gloved " : "", body_part(HAND));

                pline("%s %s!", wielderstrikebuf, wielderstrike2index == 1 ? "a second time" : wielderstrike2index == 2 ? "a third time" : "once more");
            }

            int strike2index;
            for (strike2index = 0; strike2index < multistrike2; strike2index++)
            {
                update_u_action(ACTION_TILE_ATTACK);
                play_monster_simple_weapon_sound(&youmonst, 0, wep, OBJECT_SOUND_TYPE_SWING_MELEE);
                u_wait_until_action();

                if (strike2index > 0 && uarms)
                {
                    pline("%s %s!", Yobjnam2(uarms, "strike"), strike2index == 1 ? "a second time" : strike2index == 2 ? "a third time" : "once more");
                }

                int tmp = find_roll_to_hit(mon, uattk->aatyp, wep, &attknum,
                    &armorpenalty);

                int dieroll = rnd(20);
                int mhit = (tmp > dieroll || u.uswallow);
                boolean uses_spell_flags = wep ? object_uses_spellbook_wand_flags_and_properties(wep) : FALSE;

                if (wep && !uses_spell_flags && ((objects[wep->otyp].oc_aflags2 & A2_HITS_ADJACENT_SQUARES) || (wep->oartifact && artifact_has_flag(wep, AF_HITS_ADJACENT_SQUARES)))
                    && !u.uswallow && !u.ustuck && !NODIAG(u.umonnum))
                {
                    malive = hitum_cleave(mon, uattk, wep);
                    if (wepbefore && !wep)
                        wep_was_destroyed = TRUE;
                }
                else
                {

                    malive = known_hitum(mon, wep, &mhit, tmp, armorpenalty, uattk,
                        dieroll);
                    /* second passive counter-attack only occurs if second attack hits */
                    if (wepbefore && !wep)
                        wep_was_destroyed = TRUE;
                    if (mhit)
                        (void)passive(mon, wep, mhit, malive, AT_WEAP, wep_was_destroyed);
                }

                update_u_action_revert(ACTION_TILE_NO_ACTION);

                if (!malive || m_at(x, y) != mon || wep_was_destroyed)
                {
                    breakloop = TRUE;
                    break;
                }
            }
            if (breakloop)
                break;
        }
    }
    return malive;
}

/* general "damage monster" routine; return True if mon still alive */
boolean
hmon(mon, obj, thrown, dieroll, obj_destroyed)
struct monst *mon;
struct obj *obj;
int thrown; /* HMON_xxx (0 => hand-to-hand, other => ranged) */
int dieroll;
boolean* obj_destroyed;
{
    boolean result, anger_guards;

    anger_guards = (is_peaceful(mon)
                    && (mon->ispriest || mon->isshk || mon->issmith || mon->isnpc || is_watch(mon->data)));
    result = hmon_hitmon(mon, obj, thrown, dieroll, obj_destroyed);
    if (mon->ispriest && !rn2(2))
        ghod_hitsu(mon);
    if (anger_guards)
        (void) angry_guards(!!Deaf);
    return result;
}

/* guts of hmon() */
STATIC_OVL boolean
hmon_hitmon(mon, obj, thrown, dieroll, obj_destroyed)
struct monst *mon;
struct obj *obj;
int thrown; /* HMON_xxx (0 => hand-to-hand, other => ranged) */
int dieroll;
boolean* obj_destroyed;
{
    int extratmp = 0;
    double damage = 0;
    struct permonst* mdat = mon->data;
    int barehand_silver_rings = 0;
    boolean barehand_silver_gauntlets = 0;

    /* The basic reason we need all these booleans is that we don't want
     * a "hit" message when a monster dies, so we have to know how much
     * damage it did _before_ outputting a hit message, but any messages
     * associated with the damage don't come out until _after_ outputting
     * a hit message.
     */
    boolean hittxt = FALSE, displaysustain = FALSE, destroyed = FALSE, already_killed = FALSE;
    boolean strikefrombehind = FALSE;
    boolean get_dmg_bonus = TRUE;
    boolean ispoisoned = FALSE, needpoismsg = FALSE, poiskilled = FALSE,
        unpoisonmsg = FALSE;
    int needenchantmsg = 0;
    double poisondamage = 0;
    boolean enchantkilled = FALSE, unenchantmsg = FALSE;
    boolean silvermsg = FALSE, silverobj = FALSE, reallysilverobj = FALSE;
    boolean lightobj = FALSE;
    boolean valid_weapon_attack = FALSE;
    boolean unarmed = !uwep && !uarm && !uarms;
    boolean hand_to_hand = (thrown == HMON_MELEE
        /* not grapnels; applied implies uwep */
        || (thrown == HMON_APPLIED && is_appliable_pole_type_weapon(uwep)));
    boolean ordinary_thrown = (thrown == HMON_THROWN && !ammo_and_launcher(obj, uwep));
    boolean hide_damage_amount = FALSE;
    boolean isinstakilled = FALSE;
    boolean isdisintegrated = FALSE;
    int damage_increase_adtyp = AD_MAGM; /* base assumption if nothing else is set */
    boolean incorrect_weapon_use = FALSE;

    int critstrikeroll = rn2(100);
    boolean is_golf_swing_with_stone = (thrown == HMON_GOLF && obj && uwep && (obj->oclass == GEM_CLASS || objects[obj->otyp].oc_skill == -P_SLING) && uwep->otyp == GOLF_CLUB);
    int mx = mon->mx;
    int my = mon->my;

    int jousting = 0;
    int64_t silverhit = 0L;
    enum p_skills wtype;
    struct obj* monwep;
    char unconventional[BUFSZ]; /* substituted for word "attack" in msg */
    char saved_oname[BUFSZ];

    unconventional[0] = '\0';
    saved_oname[0] = '\0';

    wakeup(mon, TRUE);
    if (!obj || (uarmg && obj == uarmg)) 
    { 
        /* attack with bare hands */
        /* All gloves give bonuses when fighting 'bare-handed'. -- JG
           So do silver rings.  Note:  rings are worn under gloves, so you don't
           get both bonuses, and two silver rings don't give double bonus. */
        if (is_shade(mdat) && !(obj && shade_glare(obj)))
            damage = 0;
        else
        {
            boolean martial_arts_applies = martial_bonus(); // && !(uarmg&& is_metallic(uarmg));
            int tmp = 0;
            if (martial_arts_applies)
            {
                /* bonus for martial arts */
                switch (adjusted_skill_level(P_MARTIAL_ARTS))
                {
                case P_BASIC:
                    tmp = rnd(4);
                    break;
                case P_SKILLED:
                    tmp = rnd(6);
                    break;
                case P_EXPERT:
                    tmp = rnd(8);
                    break;
                case P_MASTER:
                    tmp = rnd(10);
                    break;
                case P_GRAND_MASTER:
                    tmp = rnd(12);
                    break;
                default:
                    break;
                }
            }
            else
            {
                /* normal bare-handed damage */
                tmp = rnd(2);
            }

            damage_increase_adtyp = AD_PHYS;
            damage += adjust_damage(tmp, &youmonst, mon, AD_PHYS, ADFLAGS_NONE);

            if (martial_bonus() && !martial_arts_applies)
            {
                if (!context.glovehitmsgdisplayed)
                {
                    You_feel("your gloves hinder the elegance of your strikes.");
                    context.glovehitmsgdisplayed = TRUE;
                }
            }
            else
            {
                if (context.glovehitmsgdisplayed)
                {
                    Your("strikes feel effortless.");
                    context.glovehitmsgdisplayed = FALSE;
                }
            }

            if (uarmg)
            {
                /* gauntlets, no rings exposed */
                damage_increase_adtyp = objects[uarmg->otyp].oc_damagetype;
                int basedmg = weapon_dmg_value(uarmg, mon, &youmonst, 0);
                damage += adjust_damage(basedmg, &youmonst, mon, objects[uarmg->otyp].oc_damagetype, ADFLAGS_NONE);
                extratmp = weapon_extra_dmg_value(uarmg, mon, &youmonst, basedmg);
                damage += adjust_damage(extratmp, &youmonst, mon, objects[uarmg->otyp].oc_extra_damagetype, ADFLAGS_NONE);
                
                /* artifact gauntlets*/
                int ahres = 0;
                if (uarmg->oartifact
                    && (ahres = artifact_hit(&youmonst, mon, uarmg, &damage, &isinstakilled, dieroll)))
                {
                    if (DEADMONSTER(mon)) /* artifact killed monster */
                        return FALSE;

                    if (damage == 0)
                        return TRUE;

                    hittxt = TRUE;
                    if (ahres == 1)
                        displaysustain = TRUE;
                }

                short spec_adtyp = 0;
                int special_hit_dmg = pseudo_artifact_hit(&youmonst, mon, uarmg, extratmp, dieroll, critstrikeroll, &spec_adtyp);
                if (special_hit_dmg < 0)
                {
                    //damage += 2 * (double)mon->mhp + 200;
                    //mon->mhp = 0;
                    isinstakilled = TRUE;
                    if (special_hit_dmg == -2)
                        isdisintegrated = TRUE;
                    hide_damage_amount = TRUE;
                    hittxt = TRUE; /* This means that hit text is already given */
                }
                else if (special_hit_dmg > 0)
                {
                    damage += adjust_damage(special_hit_dmg, &youmonst, mon, spec_adtyp, ADFLAGS_NONE);
                }
                
                /* silver gauntelts? */
                damage += adjust_damage(special_dmgval(&youmonst, mon, W_ARMG, &silverhit), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
                barehand_silver_gauntlets = !!(silverhit & W_ARMG);
                reallysilverobj = uarmg && uarmg->material == MAT_SILVER;
                if (barehand_silver_gauntlets)
                    silvermsg = TRUE;
            }
            else
            {
                /* no gauntlets, rings exposed */
                damage_increase_adtyp = AD_PHYS;
                damage += adjust_damage(special_dmgval(&youmonst, mon, W_RING, &silverhit), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
                barehand_silver_rings += (((silverhit & W_RINGL) ? 1 : 0) + ((silverhit & W_RINGR) ? 1 : 0));
                if (barehand_silver_rings > 0)
                    silvermsg = TRUE;
            }

            /* Play hit sound; note that uarmg == 0 indicates bare-handed */
            if (is_long_worm_with_tail(mon->data) && isok(bhitpos.x, bhitpos.y) && !is_wseg_head(mon, bhitpos.x, bhitpos.y))
                play_monster_weapon_hit_sound_at_location(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), 0, uarmg, damage, thrown, bhitpos.x, bhitpos.y);
            else
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), 0, uarmg, damage, thrown);
        }
        valid_weapon_attack = (damage > 0);
    }
    else 
    {
        if (!((artifact_light(obj) || obj_shines_magical_light(obj) || has_obj_mythic_magical_light(obj)) && obj->lamplit))
            Strcpy(saved_oname, cxname(obj));
        else
            Strcpy(saved_oname, bare_artifactname(obj));

        /* Play hit sound */
        if (is_long_worm_with_tail(mon->data) && isok(bhitpos.x, bhitpos.y) && !is_wseg_head(mon, bhitpos.x, bhitpos.y))
        {
            if (thrown == HMON_MELEE)
                play_monster_weapon_hit_sound_at_location(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), 0, obj, damage, thrown, bhitpos.x, bhitpos.y);
            else
                play_object_hit_sound_at_location(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), damage, thrown, bhitpos.x, bhitpos.y);
        }
        else
        {
            if (thrown == HMON_MELEE)
                play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), 0, obj, damage, thrown);
            else
                play_object_hit_sound(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), damage, thrown);
        }

        if (is_weapon(obj) || obj->oclass == GEM_CLASS) 
        {
            /* is it not a melee weapon? */
            if (/* if you strike with a bow... */
                is_launcher(obj)
                /* or strike with non-worn weapon gloves */
                || ((objects[(obj)->otyp].oc_flags5 & O5_IS_WEAPON_WHEN_WORN) != 0 && !is_obj_worn(obj))
                /* or strike with a missile in your hand... */
                || (!thrown && (is_missile(obj) || is_ammo(obj)))
                /* or use a pole at short range and not mounted... */
                || (!thrown && !u.usteed && is_appliable_pole_type_weapon(obj) && !is_spear(obj) && !is_trident(obj))
                /* or throw a missile without the proper bow... */
                || (is_ammo(obj) && !is_golf_swing_with_stone && (thrown != HMON_THROWN
                    || !ammo_and_launcher(obj, uwep)))) 
            {
                incorrect_weapon_use = TRUE;

                /* then do only 1-2 points of damage */
                damage_increase_adtyp = objects[obj->otyp].oc_damagetype;
                if (is_shade(mdat) && !shade_glare(obj))
                    damage = 0;
                else
                    damage = adjust_damage(rnd(2), &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);

                if (obj_counts_as_silver(obj)
                    && mon_hates_silver(mon)) 
                {
                    silvermsg = TRUE;
                    silverobj = TRUE;
                    reallysilverobj = obj->material == MAT_SILVER;

                    /* if it will already inflict dmg, make it worse */
                    damage += adjust_damage(rnd((min(1, (int)damage)) ? 20 : 10), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
                }
                if (!thrown && (obj == uwep || obj == uarms) && obj->otyp == BOOMERANG
                    && rnl(4) == 4 - 1) 
                {
                    boolean more_than_1 = (obj->quan > 1L);

                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);
                    pline("As you hit %s, %s%s breaks into splinters.",
                        mon_nam(mon), more_than_1 ? "one of " : "",
                        yname(obj));
                    if (!more_than_1)
                    {
                        if(obj == uwep)
                            uwepgone(); /* set unweapon */
                        else if (obj == uarms)
                            uwep2gone(); /* set unweapon */
                    }
                    debugprint("hmon_hitmon: %d", obj->otyp);
                    useup(obj);
                    if (!more_than_1)
                        obj = (struct obj*) 0;
                    hittxt = TRUE;
                    if (!is_shade(mdat))
                        damage++;
                }
            }
            else
            {
                int basedmg = 0;
                if (is_launcher(obj))
                    basedmg = d(1, 2);
                else
                    basedmg = weapon_dmg_value(obj, mon, &youmonst, 0);

                damage_increase_adtyp = objects[obj->otyp].oc_damagetype;
                damage = adjust_damage(basedmg, &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);
                extratmp = weapon_extra_dmg_value(obj, mon, &youmonst, basedmg);
                damage += adjust_damage(extratmp, &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);

                /* a minimal hit doesn't exercise proficiency */
                valid_weapon_attack = (damage > 0);
                if (!valid_weapon_attack || mon == u.ustuck || u.twoweap
                    /* Cleaver can hit up to three targets at once so don't
                       let it also hit from behind or shatter foes' weapons */
                    || (hand_to_hand && obj->oartifact && artifact_has_flag(obj, AF_HITS_ADJACENT_SQUARES))
                    || (hand_to_hand && (objects[obj->otyp].oc_aflags2 & A2_HITS_ADJACENT_SQUARES))
                    )
                {
                    ; /* no special bonuses */
                }
                else if (is_fleeing(mon) && Role_if(PM_ROGUE) && !Upolyd
                    /* multi-shot throwing is too powerful here */
                    && hand_to_hand) 
                {
                    //You("strike %s from behind!", mon_nam(mon));
                    damage += adjust_damage(rnd(u.ulevel), &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);
                    strikefrombehind = TRUE;
                }
                else if (dieroll == 2 && obj == uwep
                    && obj->oclass == WEAPON_CLASS
                    && (bimanual(obj)
                        || (Role_if(PM_SAMURAI) && obj->otyp == KATANA
                            && !uarms))
                    && ((wtype = uwep_skill_type()) != P_NONE
                        && P_SKILL_LEVEL(wtype) >= P_SKILLED)
                    && ((monwep = MON_WEP(mon)) != 0
                        && !is_flimsy(monwep)
                        && !obj_resists(monwep,
                            50 + 15 * (greatest_erosion(obj)
                                - greatest_erosion(monwep)),
                            100))) 
                {
                    /*
                     * 2.5% chance of shattering defender's weapon when
                     * using a two-handed weapon; less if uwep is rusted.
                     * [dieroll == 2 is most successful non-beheading or
                     * -bisecting hit, in case of special artifact damage;
                     * the percentage chance is (1/20)*(50/100).]
                     * If attacker's weapon is rustier than defender's,
                     * the obj_resists chance is increased so the shatter
                     * chance is decreased; if less rusty, then vice versa.
                     */
                    setmnotwielded(mon, monwep);
                    mon->weapon_strategy = NEED_WEAPON;
                    play_simple_object_sound(monwep, OBJECT_SOUND_TYPE_BREAK);
                    pline("%s from the force of your blow!",
                        Yobjnam2(monwep, "shatter"));
                    m_useupall(mon, monwep);
                    /* If someone just shattered MY weapon, I'd flee! */
                    if (rn2(4)) {
                        monflee(mon, d(2, 3), TRUE, TRUE);
                    }
                    hittxt = TRUE;
                }

                int ahres = 0;
                if (obj->oartifact
                    && (ahres = artifact_hit(&youmonst, mon, obj, &damage, &isinstakilled, dieroll))) 
                {
                    if (DEADMONSTER(mon)) /* artifact killed monster */
                        return FALSE;
                    
                    if (damage == 0)
                        return TRUE;

                    hittxt = TRUE;
                    if(ahres == 1)
                        displaysustain = TRUE;
                }

                short spec_adtyp = 0;
                int special_hit_dmg = pseudo_artifact_hit(&youmonst, mon, obj, extratmp, dieroll, critstrikeroll, &spec_adtyp);
                if (special_hit_dmg < 0)
                {
                    //damage += 2 * (double)mon->mhp + 200;
                    //mon->mhp = 0;
                    isinstakilled = TRUE;
                    if (special_hit_dmg == -2)
                        isdisintegrated = TRUE;
                    hide_damage_amount = TRUE;
                    hittxt = TRUE; /* This means that hit text is already given */
                }
                else if(special_hit_dmg > 0)
                {
                    damage += adjust_damage(special_hit_dmg, &youmonst, mon, spec_adtyp, ADFLAGS_NONE);
                }
                if (obj_counts_as_silver(obj)
                    && mon_hates_silver(mon)) 
                {
                    silvermsg = TRUE;
                    silverobj = TRUE;
                    reallysilverobj = obj->material == MAT_SILVER;
                }

                if ((artifact_light(obj) || obj_shines_magical_light(obj) || has_obj_mythic_magical_light(obj)) && obj->lamplit
                    && mon_hates_light(mon))
                    lightobj = TRUE;

                if (u.usteed && !thrown && damage > 0
                    && obj && can_obj_joust(obj) && mon != u.ustuck)
                {
                    jousting = joust(mon, obj);
                    /* exercise skill even for minimal damage hits */
                    if (jousting)
                        valid_weapon_attack = TRUE;
                }

                if (obj->opoisoned && is_poisonable(obj))
                    ispoisoned = TRUE;
            }
        }
        else if (obj->oclass == POTION_CLASS) 
        {
            if (obj->quan > 1L)
                obj = splitobj(obj, 1L);
            else
            {
                if(obj == uwep)
                    setuwep((struct obj*) 0, W_WEP);
                else if (obj == uarms)
                    setuwep((struct obj*) 0, W_ARMS);
            }
            freeinv(obj);
            potionhit(mon, &obj,
                hand_to_hand ? POTHIT_HERO_BASH : POTHIT_HERO_THROW);
            obj = (struct obj*)0;
            *obj_destroyed = TRUE;
            if (DEADMONSTER(mon))
                return FALSE; /* killed */
            hittxt = TRUE;
            /* in case potion effect causes transformation */
            mdat = mon->data;
            damage = is_shade(mdat) ? 0 : 1;
        }
        else
        {
            if (is_shade(mdat) && !shade_aware(obj))
            {
                damage = 0;
                Strcpy(unconventional, cxname(obj));
            }
            else 
            {
                switch (obj->otyp) 
                {
                case BOULDER:         /* 1d20 */
                case HEAVY_IRON_BALL: /* 1d25 */
                case IRON_CHAIN:      /* 1d4+1 */
                {
                    damage_increase_adtyp = objects[obj->otyp].oc_damagetype;
                    int basedmg = weapon_dmg_value(obj, mon, &youmonst, 0);
                    damage = adjust_damage(basedmg, &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);
                    extratmp = weapon_extra_dmg_value(obj, mon, &youmonst, basedmg);
                    damage += adjust_damage(extratmp, &youmonst, mon, objects[obj->otyp].oc_extra_damagetype, ADFLAGS_NONE);
                    break;
                }
                case MIRROR:
                case MAGIC_MIRROR:
                    if (breaktest(obj)) 
                    {
                        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);
                        You_ex(ATR_NONE, CLR_MSG_WARNING, "break %s.  That's bad luck!", ysimple_name(obj));
                        change_luck(-2, TRUE);
                        debugprint("hmon_hitmon2: %d", obj->otyp);
                        useup(obj);
                        obj = (struct obj*) 0;
                        unarmed = FALSE; /* avoid obj==0 confusion */
                        get_dmg_bonus = FALSE;
                        hittxt = TRUE;
                    }
                    damage = 1;
                    break;
                case EXPENSIVE_CAMERA:
                    /* Play hit sound now here */
                    if (thrown == HMON_MELEE)
                        play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), 0, obj, damage, thrown);
                    else
                        play_object_hit_sound(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), damage, thrown);
                    
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);

                    You_ex(ATR_NONE, CLR_MSG_WARNING, "succeed in destroying %s.  Congratulations!",
                        ysimple_name(obj));
                    release_camera_demon(obj, u.ux, u.uy);
                    debugprint("hmon_hitmon3: %d", obj->otyp);
                    useup(obj);
                    return TRUE;
                case CORPSE: /* fixed by polder@cs.vu.nl */
                    if (obj->corpsenm >= LOW_PM && touch_petrifies(&mons[obj->corpsenm]))
                    {
                        damage = 1;
                        hittxt = TRUE;

                        /* Play hit sound now here */
                        if (thrown == HMON_MELEE)
                            play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), 0, obj, damage, thrown);
                        else
                            play_object_hit_sound(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), damage, thrown);

                        You("hit %s with %s.", mon_nam(mon),
                            corpse_xname(obj, (const char*)0,
                                obj->dknown ? CXN_PFX_THE
                                : CXN_ARTICLE));
                        obj->dknown = 1;
                        if (check_magic_cancellation_success(mon, 0) || resists_ston(mon))
                        {
                            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mon->mx, mon->my);
                            m_shieldeff(mon);
                            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists!", Monnam(mon));
                        }
                        else
                        {
                            start_delayed_petrification(mon, TRUE);
                        }
                        /* note: hp may be <= 0 even if munstoned==TRUE */
                        return (boolean)!DEADMONSTER(mon);
#if 0
                    }
                    else if (touch_petrifies(mdat)) {
                        ; /* maybe turn the corpse into a statue? */
#endif
                    }
                    damage_increase_adtyp = AD_PHYS;
                    damage = adjust_damage((obj->corpsenm >= LOW_PM ? mons[obj->corpsenm].msize
                        : 0) + 1, &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
                    break;

#define useup_eggs(o)                    \
    do { \
            if (thrown)                      \
            { \
                    debugprint("useup_eggs: %d", (o)->otyp); \
                    obfree(o, (struct obj*)0); \
            } \
            else                             \
            {\
                debugprint("useup_eggs: %d", (o)->otyp); \
                useupall(o);                 \
            }\
            o = (struct obj*) 0;            \
    } while (0) /* now gone */
                case EGG: 
                {
                    int64_t cnt = obj->quan;
                    int luck_change = 0;

                    damage = 1; /* nominal physical damage */
                    get_dmg_bonus = FALSE;
                    hittxt = TRUE; /* message always given */
                    /* egg is always either used up or transformed, so next
                       hand-to-hand attack should yield a "bashing" mesg */
                    update_unweapon();
                    /*
                    if (obj == uwep)
                        unweapon = TRUE;
                    */
                    if ((obj->speflags & SPEFLAGS_YOURS) && obj->corpsenm >= LOW_PM) 
                    {
                        if (obj->quan < 5L)
                            luck_change += (schar)-(obj->quan);
                        else
                            luck_change += -5;
                    }

                    if (obj->corpsenm >= LOW_PM && touch_petrifies(&mons[obj->corpsenm]))
                    {
                        /*learn_egg_type(obj->corpsenm);*/
                        pline1("Splat!");
                        You("hit %s with %s %s egg%s!",
                            mon_nam(mon),
                            obj->known ? "the" : cnt > 1L ? "some" : "a",
                            obj->known ? mons[obj->corpsenm].mname
                            : "petrifying",
                            plur(cnt));
                        obj->known = 1; /* (not much point...) */
                        useup_eggs(obj);

                        change_luck(luck_change, TRUE);

                        if (check_magic_cancellation_success(mon, 0) || resists_ston(mon))
                        {
                            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mon->mx, mon->my);
                            m_shieldeff(mon);
                            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists!", Monnam(mon));
                        }
                        else
                        {
                            start_delayed_petrification(mon, TRUE);
                        }
                        return (boolean)(!DEADMONSTER(mon));
                    }
                    else 
                    { /* ordinary egg(s) */
                        const char* eggp = (obj->corpsenm != NON_PM
                            && obj->known)
                            ? the(mons[obj->corpsenm].mname)
                            : (cnt > 1L) ? "some" : "an";

                        You("hit %s with %s egg%s.", mon_nam(mon), eggp,
                            plur(cnt));

                        if (touch_petrifies(mdat) && !stale_egg(obj))
                        {
                            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "egg%s %s alive any more...", plur(cnt),
                                (cnt == 1L) ? "isn't" : "aren't");
                            if (obj->timed)
                                obj_stop_timers(obj);
                            obj->otyp = ROCK;
                            obj->material = objects[obj->otyp].oc_material;
                            obj->oclass = GEM_CLASS;
                            obj->oartifact = 0;
                            obj->speflags = 0;
                            obj->known = obj->dknown = obj->bknown = 0;
                            obj->owt = weight(obj);
                            if (thrown)
                                place_object(obj, mon->mx, mon->my);
                        }
                        else 
                        {
                            pline("Splat!");
                            useup_eggs(obj);
                            exercise(A_WIS, FALSE);
                        }
                        change_luck(luck_change, TRUE);
                    }
                    break;
#undef useup_eggs
                }
                case CLOVE_OF_GARLIC: /* no effect against demons */
                    if (is_undead(mdat) || is_vampshifter(mon)) 
                    {
                        play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mon->mx, mon->my);
                        monflee(mon, d(2, 4), FALSE, TRUE);
                    }
                    damage = 1;
                    break;
                case CREAM_PIE:
                case BLINDING_VENOM:
                    mon->msleeping = 0;
                    if (can_blnd(&youmonst, mon,
                        (uchar)((obj->otyp == BLINDING_VENOM)
                            ? AT_SPIT
                            : AT_WEAP),
                        obj)) {
                        if (Blind) {
                            pline(obj->otyp == CREAM_PIE ? "Splat!"
                                : "Splash!");
                        }
                        else if (obj->otyp == BLINDING_VENOM) 
                        {
                            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "venom blinds %s%s!", mon_nam(mon),
                                !is_blinded(mon) ? "" : " further");
                        }
                        else 
                        {
                            char* whom = mon_nam(mon);
                            char* what = The(xname(obj));

                            if (!thrown && obj->quan > 1L)
                                what = An(singular(obj, xname));
                            /* note: s_suffix returns a modifiable buffer */
                            if (haseyes(mdat)
                                && mdat != &mons[PM_FLOATING_EYE])
                                whom = strcat(strcat(s_suffix(whom), " "),
                                    mbodypart(mon, FACE));
                            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s over %s!", what,
                                vtense(what, "splash"), whom);
                        }
                        setmangry(mon, TRUE);
                        increase_mon_property(mon, BLINDED, rn1(25, 21));
                    }
                    else 
                    {
                        pline(obj->otyp == CREAM_PIE ? "Splat!" : "Splash!");
                        setmangry(mon, TRUE);
                    }
                    if (thrown)
                    {
                        debugprint("hmon_hitmon: %d", obj->otyp);
                        obfree(obj, (struct obj*)0);
                    }
                    else
                    {
                        debugprint("hmon_hitmon4: %d", obj->otyp);
                        useup(obj);
                    }
                    hittxt = TRUE;
                    get_dmg_bonus = FALSE;
                    damage = 0;
                    break;
                case ACID_VENOM: /* thrown (or spit) */
                    if (is_mon_immune_to_acid(mon)) 
                    {
                        Your("venom hits %s harmlessly.", mon_nam(mon));
                        damage = 0;
                    }
                    else 
                    {
                        Your("venom burns %s!", mon_nam(mon));
                        damage_increase_adtyp = objects[obj->otyp].oc_damagetype;
                        int basedmg = weapon_dmg_value(obj, mon, &youmonst, 1);
                        damage = adjust_damage(basedmg, &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);
                        extratmp = weapon_extra_dmg_value(obj, mon, &youmonst, basedmg);
                        damage += adjust_damage(extratmp, &youmonst, mon, objects[obj->otyp].oc_extra_damagetype, ADFLAGS_NONE);
                    }
                    debugprint("hmon_hitmon2: %d", obj->otyp);
                    if (thrown)
                        obfree(obj, (struct obj*) 0);
                    else
                        useup(obj);
                    hittxt = TRUE;
                    get_dmg_bonus = FALSE;
                    break;
                default:
                    /* non-weapons can damage because of their weight */
                    /* (but not too much) */
                    damage_increase_adtyp = AD_PHYS;
                    damage = adjust_damage(obj->owt / 100, &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
                    if (is_wet_towel(obj)) 
                    {
                        /* wielded wet towel should probably use whip skill
                           (but not by setting objects[TOWEL].oc_skill==P_WHIP
                           because that would turn towel into a weptool) */
                        damage += obj->special_quality;
                        if (rn2(obj->special_quality + 1)) /* usually lose some wetness */
                            dry_a_towel(obj, -1, TRUE);
                    }
                    if (damage < 1)
                        damage = 1;
                    else
                        damage = rnd((int)ceil(damage));
                    if (damage > 6)
                        damage = 6;
                    /*
                     * Things like silver wands can arrive here so
                     * so we need another silver check.
                     */
                    if (obj_counts_as_silver(obj)
                        && mon_hates_silver(mon)) 
                    {
                        damage += adjust_damage(rnd(20), &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);
                        silvermsg = TRUE;
                        silverobj = TRUE;
                        reallysilverobj = obj->material == MAT_SILVER;
                    }
                }
            }
        }
    }

    /****** NOTE: perhaps obj is undefined!! (if !thrown && BOOMERANG)
     *      *OR* if attacking bare-handed!! */

    if (get_dmg_bonus && damage > 0)
    {
        damage += adjust_damage(u.ubasedaminc, &youmonst, mon, damage_increase_adtyp, ADFLAGS_NONE);
        damage += adjust_damage(u.udaminc, &youmonst, mon, damage_increase_adtyp, ADFLAGS_NONE);
        
        /* If you throw using a propellor, you don't get a strength
         * bonus but you do get an increase-damage bonus.
         */
        if (!is_golf_swing_with_stone && (thrown != HMON_THROWN || !obj || !uwep
            || !ammo_and_launcher(obj, uwep))) 
        {
            if (thrown == HMON_THROWN)
                damage += adjust_damage(u_thrown_str_dmg_bonus(), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
            else if (*obj_destroyed)
                damage += 0;
            else if (!obj || obj == uarmg)
            {
                if(!martial_bonus()) // || (uarmg && is_metallic(uarmg))
                    damage += adjust_damage(u_str_dmg_bonus() / 2, &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
                else if(adjusted_skill_level(P_MARTIAL_ARTS) >= P_BASIC)
                    damage += adjust_damage(u_str_dmg_bonus(), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
            }
            else if(obj && objects[obj->otyp].oc_skill == P_NONE)
                damage += adjust_damage(u_str_dmg_bonus() / 2, &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
            else
                damage += adjust_damage(u_str_dmg_bonus(), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
        }
        else if ((obj && uwep && ammo_and_launcher(obj, uwep)) || is_golf_swing_with_stone)
        {
            if (objects[uwep->otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH)
                damage += adjust_damage(objects[uwep->otyp].oc_fixed_damage_bonus, &youmonst, mon, objects[uwep->otyp].oc_damagetype, ADFLAGS_NONE);
            else
                damage += adjust_damage(u_str_dmg_bonus(), &youmonst, mon, objects[uwep->otyp].oc_damagetype, ADFLAGS_NONE);

            /*
            if (uwep->otyp == CROSSBOW)
                tmp += 3; // Light crossbows get +3 bonus, (heavy) crossbows get 18/00 strength bonus
            else if (uwep->otyp == HEAVY_CROSSBOW)
                tmp += 6;
            else if (uwep->otyp == HAND_CROSSBOW)
                tmp += 0;
            else if (uwep->otyp == REPEATING_CROSSBOW)
                tmp += 0;
            else
                tmp += u_str_dmg_bonus(); //Normal bows get full strength bonus
            */

            //All bows get bow's enchantment bonus and damage
            int basedmg = weapon_dmg_value(uwep, mon, &youmonst, 2);
            damage += adjust_damage(basedmg, & youmonst, mon, objects[uwep->otyp].oc_damagetype, ADFLAGS_NONE);
            extratmp = weapon_extra_dmg_value(uwep, mon, &youmonst, basedmg);
            damage += adjust_damage(extratmp, & youmonst, mon, objects[uwep->otyp].oc_extra_damagetype, ADFLAGS_NONE);

            if (u.uarcherybonus != 0)
                damage += adjust_damage(u.uarcherybonus, &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE);

        }
        else if (thrown == HMON_THROWN && obj && !is_ammo(obj))
        {
            //Thrown weapons get also damage bonus, but specific for thrown weapons
            damage += adjust_damage(u_thrown_str_dmg_bonus(), &youmonst, mon, AD_PHYS, ADFLAGS_NONE);
        }
    }

    //Bonus from weapon skills
    struct obj* wep;
    wep = (is_golf_swing_with_stone || PROJECTILE(obj)) ? uwep : obj;
    wtype = is_golf_swing_with_stone ? P_THROWN_WEAPON :
        (!obj || !wep) ? (adjusted_skill_level(P_BARE_HANDED_COMBAT) < P_GRAND_MASTER ? P_BARE_HANDED_COMBAT : P_MARTIAL_ARTS) :
        weapon_skill_type(wep); //: uwep_skill_type();

    if (valid_weapon_attack)
    {
        /* to be valid a projectile must have had the correct projector */
        damage += adjust_damage(
            weapon_skill_dmg_bonus(wep, is_golf_swing_with_stone ? P_THROWN_WEAPON : P_NONE, FALSE, !is_golf_swing_with_stone, TRUE, 0, TRUE, ordinary_thrown),
            &youmonst, mon, wep ? objects[wep->otyp].oc_damagetype : AD_PHYS, ADFLAGS_NONE);
        /* [this assumes that `!thrown' implies wielded...] */
        use_skill(wtype, 1);

        if(u.twoweap && obj && uarms && obj == uarms) /* Dual weapon combat skill is increased when you successfully hit with a weapon in your left hand */
            use_skill(P_DUAL_WEAPON_COMBAT, 1);

        if (thrown == HMON_MELEE && obj && uwep && obj == uwep && two_handed_bonus_applies(obj))
            use_skill(P_TWO_HANDED_WEAPON, 1);

        if (thrown == HMON_THROWN && wtype != P_THROWN_WEAPON && ordinary_thrown)
            use_skill(P_THROWN_WEAPON, 1);
    }
    else if (ordinary_thrown) //Thrown weapon skill bonus to thrown objects
    {
        damage += adjust_damage(weapon_skill_dmg_bonus((struct obj*)0, P_NONE, FALSE, FALSE, 2, 0, TRUE, TRUE),
            &youmonst, mon, wep ? objects[wep->otyp].oc_damagetype : AD_PHYS, ADFLAGS_NONE);

        if (thrown == HMON_THROWN && wtype != P_THROWN_WEAPON && damage > 0)
            use_skill(P_THROWN_WEAPON, 1);
    }

    if (ispoisoned && !isdisintegrated) 
    {
        int nopoison = (10 - (obj->owt / 10));

        if (nopoison < 2)
            nopoison = 2;
        if (Role_if(PM_SAMURAI))
        {
            play_sfx_sound(SFX_CAITIFF);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "dishonorably use a poisoned weapon!");
            adjalign(-sgn(u.ualign.type));
        }
        else if (u.ualign.type == A_LAWFUL && u.ualign.record > -10)
        {
            play_sfx_sound(SFX_CAITIFF);
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "like an evil coward for using a poisoned weapon.");
            adjalign(-1);
        }
        if (obj && !rn2(nopoison)) 
        {
            /* remove poison now in case obj ends up in a bones file */
            obj->opoisoned = FALSE;
            /* defer "obj is no longer poisoned" until after hit message */
            unpoisonmsg = TRUE;
        }

        if (resists_poison(mon))
            needpoismsg = TRUE;
        else
        {
            poisondamage = adjust_damage(d(2, 6), &youmonst, mon, AD_DRST, ADFLAGS_NONE);
            damage += poisondamage;
        }
    }

    if (obj && obj->elemental_enchantment > 0 && !isdisintegrated)
    {
        switch (obj->elemental_enchantment)
        {
        case COLD_ENCHANTMENT:
            if (is_mon_immune_to_cold(mon))
                needenchantmsg = -obj->elemental_enchantment;
            else
            {
                needenchantmsg = obj->elemental_enchantment;
                damage += adjust_damage(d(12, 6), &youmonst, mon, AD_COLD, ADFLAGS_NONE);
            }
            if (is_ammo(obj) || throwing_weapon(obj) || objects[obj->otyp].oc_merge ? 1 : !rn2(5)) 
            {
                obj->elemental_enchantment = 0;
                /* defer "obj is no longer enchanted" until after hit message */
                unenchantmsg = TRUE;
                if(obj->where == OBJ_INVENT)
                    update_inventory();
            }
            break;
        case FIRE_ENCHANTMENT:
            if (is_mon_immune_to_fire(mon))
                needenchantmsg = -1 * ((int)obj->elemental_enchantment);
            else
            {
                needenchantmsg = obj->elemental_enchantment;
                damage += adjust_damage(d(4, 6), &youmonst, mon, AD_FIRE, ADFLAGS_NONE);
            }
            if (is_ammo(obj) || throwing_weapon(obj) || objects[obj->otyp].oc_merge ? 1 : !rn2(5)) {
                obj->elemental_enchantment = 0;
                /* defer "obj is no longer enchanted" until after hit message */
                unenchantmsg = TRUE;
                if (obj->where == OBJ_INVENT)
                    update_inventory();
            }
            break;
        case LIGHTNING_ENCHANTMENT:
            if (is_mon_immune_to_elec(mon))
                needenchantmsg = -1 * ((int)obj->elemental_enchantment);
            else
            {
                needenchantmsg = obj->elemental_enchantment;
                damage += adjust_damage(d(6, 6), &youmonst, mon, AD_ELEC, ADFLAGS_NONE);
            }
            if (is_ammo(obj) || throwing_weapon(obj) || objects[obj->otyp].oc_merge ? 1 : !rn2(5))
            {
                obj->elemental_enchantment = 0;
                /* defer "obj is no longer enchanted" until after hit message */
                unenchantmsg = TRUE;
                if (obj->where == OBJ_INVENT)
                    update_inventory();
            }
            break;
        case DEATH_ENCHANTMENT:
            if (resists_death(mon))
            {
                needenchantmsg = -1 * ((int)obj->elemental_enchantment);
            }
            else
            {
                needenchantmsg = 0; //Since gets killed message
                damage = (double)mon->mhp + 1;
                hide_damage_amount = TRUE;
                enchantkilled = TRUE;
            }
            obj->elemental_enchantment = 0;
            /* defer "obj is no longer enchanted" until after hit message */
            unenchantmsg = TRUE;
            if (obj->where == OBJ_INVENT)
                update_inventory();
            break;
        default:
            break;
        }

        if (obj->elemental_enchantment == DEATH_ENCHANTMENT)
        {
            if (Role_if(PM_SAMURAI))
            {
                play_sfx_sound(SFX_CAITIFF);
                You_ex(ATR_NONE, CLR_MSG_WARNING, "dishonorably use a weapon imbued with death magic!");
                adjalign(-sgn(u.ualign.type));
            }
            else if (u.ualign.type == A_LAWFUL && u.ualign.record > -10) 
            {
                play_sfx_sound(SFX_CAITIFF);
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "like an evil coward for using a weapon imbued with death magic.");
                adjalign(-1);
            }
        }
    }

    if (damage <= 0)
    {
        /* make sure that negative damage adjustment can't result
           in inadvertently boosting the victim's hit points */
        damage = 0;
        if (!isinstakilled)
        {
            if (is_shade(mdat))
            {
                if (!hittxt)
                {
                    const char* what = *unconventional ? unconventional : "attack";

                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s harmlessly through %s.", what,
                        vtense(what, "pass"), mon_nam(mon));
                    hittxt = TRUE;
                }
            }
            else
            {
                if (get_dmg_bonus)
                    damage = 1;
            }
        }
    }

    if (jousting && !isdisintegrated) 
    {
        damage += adjust_damage(d(2, (obj == uwep) ? 10 : 2), &youmonst, mon, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE); /* [was in weapon_dmg_value()] */
        You("joust %s%s", mon_nam(mon), canseemon(mon) ? exclam((int)ceil(damage)) : ".");
        if (jousting < 0) 
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s shatters on impact!", Yname2(obj));
            /* (must be either primary or secondary weapon to get here) */
            //u.twoweap = FALSE; /* untwoweapon() is too verbose here */
            if (obj == uwep)
                uwepgone(); /* set unweapon */
            if (obj == uarms)
                uwep2gone(); /* set unweapon */
            /* minor side-effect: broken lance won't split puddings */
            debugprint("hmon_hitmon6: %d", obj->otyp);
            useup(obj);
            obj = 0;
        }
        /* avoid migrating a dead monster */
        if (mon->mhp > (int)ceil(damage)) 
        {
            mhurtle(mon, u.dx, u.dy, 1);
            mdat = mon->data; /* in case of a polymorph trap */
            if (DEADMONSTER(mon))
                already_killed = TRUE;
        }
        hittxt = TRUE;

        //Exercise also riding in addtion to thrusting weapon already exercised
        use_skill(P_RIDING, 1);
    }
    else if (unarmed && damage > 1 && !thrown && (!obj || obj == uarmg) && !Upolyd) 
    {
        /* VERY small chance of stunning opponent if unarmed. */
        if (rnd(100) < 2 * adjusted_skill_level(P_MARTIAL_ARTS) && !bigmonst(mdat)
            && !thick_skinned(mdat)) 
        {
            if (canspotmon(mon))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s from your powerful strike!", Monnam(mon),
                    makeplural(stagger(mon->data, "stagger")));
            /* avoid migrating a dead monster */
            if (mon->mhp >(int)ceil(damage)) 
            {
                mhurtle(mon, u.dx, u.dy, 1);
                mdat = mon->data; /* in case of a polymorph trap */
                if (DEADMONSTER(mon))
                    already_killed = TRUE;
            }
            hittxt = TRUE;
        }
    }

    /* Skill-based critical strike */
    boolean skill_critical_success = FALSE;
    if (damage > 0 && !incorrect_weapon_use)
    {
        int skill_crit_chance = get_skill_critical_strike_chance(wtype, FALSE, TRUE, 0, TRUE, ordinary_thrown);
        if (skill_crit_chance > 0 && rn2(100) < skill_crit_chance)
        {
            skill_critical_success = TRUE;
            damage *= 2;
        }
    }

    if (isinstakilled)
    {
        mon->mhp = 0;
    }

    int mon_hp_before = mon->mhp;

    if (!already_killed)
    {
        deduct_monster_hp(mon, damage); //    mon->mhp -= tmp;
        remove_monster_and_nearby_waitforu(mon);
    }
    int mon_hp_after = mon->mhp;

    int damagedealt = mon_hp_before - mon_hp_after;

    /* adjustments might have made damage become less than what
       a level draining artifact has already done to max HP */
    if (mon->mhp > mon->mhpmax)
        mon->mhp = mon->mhpmax;

    if (DEADMONSTER(mon))
    {
        destroyed = TRUE;
        if (poisondamage > 0 && ((double)mon->mhp + ((double)mon->mhp_fraction) / 10000 - 1) > -poisondamage)
        {
            poiskilled = TRUE;
            hide_damage_amount = TRUE;
        }
    }
    if (mon->mtame && damage > 0)
    {
        /* do this even if the pet is being killed (affects revival) */
        abuse_dog(mon); /* reduces tameness */
        /* flee if still alive and still tame; if already suffering from
           untimed fleeing, no effect, otherwise increases timed fleeing */
        if (is_tame(mon) && !destroyed)
            monflee(mon, 10 * rnd((int)ceil(damage)), FALSE, FALSE);
    }
    if ((does_split_upon_hit(mdat))
        /* pudding is alive and healthy enough to split */
        && mon->mhp > 1 && !is_cancelled(mon)
        /* iron weapon using melee or polearm hit [3.6.1: metal weapon too;
           also allow either or both weapons to cause split when twoweap] */
        && obj && (obj == uwep || obj == uarms)
        && (is_metallic(obj) /* allow scalpel and tsurugi to split puddings */
            /* but not bashing with darts, arrows or ya */
            && !(is_ammo(obj) || is_missile(obj)))
        && hand_to_hand) {
        struct monst* mclone;
        if ((mclone = clone_mon(mon, 0, 0, TRUE)) != 0) 
        {
            char withwhat[BUFSZ];

            withwhat[0] = '\0';
            if (u.twoweap && flags.verbose)
                Sprintf(withwhat, " with %s", yname(obj));
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s divides as you hit it%s!", Monnam(mon), withwhat);
            hittxt = TRUE;
            mintrap(mclone);
        }
    }

    enum hit_tile_types hit_tile = skill_critical_success || strikefrombehind ? HIT_CRITICAL : poisondamage > 0 ? HIT_POISONED : HIT_GENERAL;
    if (!hittxt) /* && (1 = 1 || (thrown && m_shot.n > 1 && m_shot.o == obj->otyp)) */
    {
        const char* striketext = strikefrombehind ? "strike" : "hit";
        const char* frombehindtext = strikefrombehind ? " from behind" : "";
        const char* strikemark = strikefrombehind ? "!" : ".";
        const char* critical_text = skill_critical_success ? " critically" : "";

        if (thrown)
            hit_with_hit_tile(mshot_xname(obj), mon, exclam(destroyed ? 100 : damagedealt), hide_damage_amount ? -1 : damagedealt, critical_text, hit_tile, FALSE);
        else if (!hide_damage_amount && damagedealt > 0) 
        {

            if (!flags.verbose)
            {
                int multicolors[5] = { NO_COLOR, CLR_ORANGE, CLR_ORANGE, NO_COLOR, NO_COLOR };
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s%s it for %d damage%s%s", striketext, critical_text, damagedealt, frombehindtext, strikemark);
            }
            else
            {
                int multicolors[6] = { NO_COLOR, NO_COLOR, CLR_ORANGE, CLR_ORANGE, CLR_ORANGE, NO_COLOR };
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s %s%s for %d damage%s%s",
                    (obj && (is_shield(obj) || obj->otyp == HEAVY_IRON_BALL))
                    ? "bash" : Role_if(PM_BARBARIAN) ? "smite" : striketext,
                    mon_nam(mon), critical_text, damagedealt, frombehindtext,
                    canseemon(mon) && !strikefrombehind ? exclam(damagedealt) : strikemark);
            }

            display_m_being_hit(mon, hit_tile, damagedealt, 0UL, TRUE);
        }
        else 
        {
            if (!flags.verbose)
            {
                int multicolors[4] = { NO_COLOR, CLR_ORANGE, CLR_ORANGE, NO_COLOR };
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s it%s%s%s", striketext, critical_text, frombehindtext, strikemark);
            }
            else
            {
                int multicolors[4] = { NO_COLOR, NO_COLOR, CLR_ORANGE, NO_COLOR };
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s %s%s%s",
                    (obj && (is_shield(obj) || obj->otyp == HEAVY_IRON_BALL))
                    ? "bash" : Role_if(PM_BARBARIAN) ? "smite" : striketext,
                    mon_nam(mon), critical_text,
                    canseemon(mon) && !strikefrombehind ? exclam(damagedealt) : strikemark);
            }

            display_m_being_hit(mon, hit_tile, damagedealt, 0UL, TRUE);
        }
    }
    else if (hittxt && displaysustain && damagedealt > 0)
    {
        pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_orange2, "%s sustains %d damage!", Monnam(mon), damagedealt);
        display_m_being_hit(mon, hit_tile, damagedealt, 0UL, TRUE);
    }

    if (silvermsg) 
    {
        const char* fmt;
        char* whom = mon_nam(mon);
        char silverobjbuf[BUFSZ];

        if (canspotmon(mon)) 
        {
            if (barehand_silver_rings == 1)
                fmt = "Your silver ring sears %s!";
            else if (barehand_silver_rings == 2)
                fmt = "Your silver rings sear %s!";
            else if (barehand_silver_gauntlets)
                fmt = reallysilverobj ? "Your silver gauntlets sear %s!" : "Your gauntlets sear %s!";
            else if (silverobj && saved_oname[0]) {
                /* guard constructed format string against '%' in
                   saved_oname[] from xname(via cxname()) */
                char* oname_ptr = saved_oname;
                if (strlen(oname_ptr) > 4 && !strncmpi(oname_ptr, "the ", 4))
                    oname_ptr += 4;
                Sprintf(silverobjbuf, "Your %s%s %s",
                    strstri(oname_ptr, "silver") || !reallysilverobj ? "" : "silver ",
                    oname_ptr, vtense(oname_ptr, "sear"));
                (void)strNsubst(silverobjbuf, "%", "%%", 0);
                Strcat(silverobjbuf, " %s!");
                fmt = silverobjbuf;
            }
            else if (reallysilverobj)
                fmt = "The silver sears %s!";
            else 
            {
                *whom = highc(*whom);
                fmt = "%s is seared!";
            }
        }
        else {
            *whom = highc(*whom); /* "it" -> "It" */
            fmt = "%s is seared!";
        }
        /* note: s_suffix returns a modifiable buffer */
        if (!is_incorporeal(mdat) && !amorphous(mdat))
            whom = strcat(s_suffix(whom), " flesh");
        pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, fmt, whom);
    }
    if (lightobj)
    {
        const char* fmt;
        char* whom = mon_nam(mon);
        char emitlightobjbuf[BUFSZ];

        if (canspotmon(mon)) {
            if (saved_oname[0]) {
                Sprintf(emitlightobjbuf,
                    "%s radiance penetrates deep into",
                    s_suffix(saved_oname));
                Strcat(emitlightobjbuf, " %s!");
                fmt = emitlightobjbuf;
            }
            else
                fmt = "The light sears %s!";
        }
        else {
            *whom = highc(*whom); /* "it" -> "It" */
            fmt = "%s is seared!";
        }
        /* note: s_suffix returns a modifiable buffer */
        if (!is_incorporeal(mdat) && !amorphous(mdat))
            whom = strcat(s_suffix(whom), " flesh");
        pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, fmt, whom);
    }

    boolean uses_spell_flags = obj ? object_uses_spellbook_wand_flags_and_properties(obj) : FALSE;

    int crit_strike_probability = get_critical_strike_percentage_chance(obj, mon, &youmonst);
    int crit_strike_die_roll_threshold = crit_strike_probability / 5;

    /* Wounding */
    if (obj && !uses_spell_flags)
    {
        int extradmg = 0;
        if (
            (
                (objects[obj->otyp].oc_aflags & A1_WOUNDING) && eligible_for_extra_damage(obj, mon, &youmonst)
                && (
                    ((objects[obj->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && (
                            ((objects[obj->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && dieroll <= crit_strike_die_roll_threshold)
                            ||
                            (!(objects[obj->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && critstrikeroll < crit_strike_probability))
                        )
                    ||
                    (!(objects[obj->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && 1)
                    )
              )
           )
        {
            if (objects[obj->otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
                extradmg += (int)ceil(damage);
            else
                extradmg += (int)ceil(adjust_damage(extratmp, &youmonst, mon, objects[obj->otyp].oc_extra_damagetype, ADFLAGS_NONE));
        }

        if (has_obj_mythic_wounding(obj))
            extradmg += mythic_wounding_amount(obj);

        if (extradmg > 0)
        {
            if (!resists_wounding(mon))
            {
                mon->mbasehpdrain -= extradmg;
                update_mon_maxhp(mon);

                if (mon->mhp > mon->mhpmax)
                    mon->mhp = mon->mhpmax;

                char* whom = mon_nam(mon);
                if (canspotmon(mon))
                {
                    pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s deeply into %s!", Yobjnam2(obj, "cut"), whom);
                }
            }
            if (DEADMONSTER(mon))
            {
                destroyed = TRUE;
            }
        }
    }

    /* Life leech */
    if (obj && !resists_life_leech(mon))
    {
        int extradmg = 0;
        if (
            (!uses_spell_flags && (objects[obj->otyp].oc_aflags & A1_LIFE_LEECH) && eligible_for_extra_damage(obj, mon, &youmonst)
                && (
                    ((objects[obj->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && (
                            ((objects[obj->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && dieroll <= crit_strike_die_roll_threshold)
                            ||
                            (!(objects[obj->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && critstrikeroll < crit_strike_probability))
                        )
                    ||
                    (!(objects[obj->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && 1)
                    )
                )
            )
        {
            if (objects[obj->otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
                extradmg += (int)ceil(damage);
            else
                extradmg += (int)ceil(adjust_damage(extratmp, &youmonst, mon, objects[obj->otyp].oc_extra_damagetype, ADFLAGS_NONE));
        }

        if (has_obj_mythic_life_draining(obj))
            extradmg += mythic_life_draining_amount(obj);

        if (extradmg > 0)
        {
            boolean didleech = FALSE;
            if (Upolyd)
            {
                int hpbefore = u.mh;
                deduct_player_hp((double)-extradmg);
                if (u.mh > hpbefore)
                    didleech = TRUE;
            }
            else
            {
                int hpbefore = u.uhp;
                deduct_player_hp((double)-extradmg);
                if (u.uhp > hpbefore)
                    didleech = TRUE;
            }

            if (didleech)
            {
                char* whom = mon_nam(mon);
                if (canspotmon(mon))
                {
                    pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s the life energy from %s to you!", Yobjnam2(obj, "leech"), whom);
                }
            }
        }
    }


    boolean objectshatters = FALSE;
    /* message for shattering blades and objects here */
    if (obj && is_fragile(obj) 
        && !is_obj_indestructible(obj)
        && !is_quest_artifact(obj)
        && !obj->oartifact
        )
    {
        /* Shattering is done below, here just the message*/
        objectshatters = TRUE;
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);
        char dcbuf[BUFSZ] = "";
        if(obj->quan == 1)
            Sprintf(dcbuf, "%s from the blow!", Yobjnam2(obj, "shatter"));
        else
            Sprintf(dcbuf, "One of %s shatters from the blow!", yname(obj));

        pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
        if (obj->oclass == GEM_CLASS)
        {
            if (obj->dknown && !objects[obj->otyp].oc_name_known
                && !objects[obj->otyp].oc_uname)
                docall(obj, dcbuf);
        }
    }



    /* if a "no longer poisoned" message is coming, it will be last;
       obj->opoisoned was cleared above and any message referring to
       "poisoned <obj>" has now been given; we want just "<obj>" for
       last message, so reformat while obj is still accessible */
    if (unpoisonmsg || unenchantmsg)
        Strcpy(saved_oname, cxname(obj));

    /* [note: thrown obj might go away during killed()/xkilled() call
       (via 'thrownobj'; if swallowed, it gets added to engulfer's
       minvent and might merge with a stack that's already there)] */
    
    if (mon)
    {

        if (needpoismsg)
        {
            //play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "poison doesn't seem to affect %s.", mon_nam(mon));
        }

        if (needenchantmsg && !destroyed)    
        {
            if (iflags.using_gui_sounds)
                delay_output_milliseconds(100);

            switch (needenchantmsg)
            {
            case -COLD_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "cold doesn't seem to affect %s.", mon_nam(mon));
                break;
            case -FIRE_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "fire doesn't seem to affect %s.", mon_nam(mon));
                break;
            case -LIGHTNING_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "electricity doesn't seem to affect %s.", mon_nam(mon));
                break;
            case -DEATH_ENCHANTMENT:
                if (!check_rider_death_absorption(mon, (const char*)0))
                {
                    play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "death magic doesn't seem to affect %s.", mon_nam(mon));
                }
                break;
            case COLD_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_MONSTER_COVERED_IN_FROST, mon->mx, mon->my);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "cold sears %s.", mon_nam(mon));
                break;
            case FIRE_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_MONSTER_ON_FIRE, mon->mx, mon->my);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "fire burns %s.", mon_nam(mon));
                break;
            case LIGHTNING_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_MONSTER_GETS_ZAPPED, mon->mx, mon->my);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is jolted by lightning.", Monnam(mon));
                break;
            case DEATH_ENCHANTMENT:
                play_sfx_sound_at_location(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC, mon->mx, mon->my);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s feels its life energy draining away.", Monnam(mon));
                break;
            default:
                //play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mon->mx, mon->my);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "enchantment doesn't seem to affect %s.", mon_nam(mon));
                break;
            }
        }

        if (poiskilled) 
        {
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "poison was deadly...");
            if (!already_killed)
                killed(mon); //xkilled(mon, XKILL_NOMSG);
            destroyed = TRUE; /* return FALSE; */
        } 
        else if (enchantkilled) 
        {
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(100);
                play_sfx_sound_at_location(SFX_MONSTER_IS_HIT_WITH_DEATH_MAGIC, mon->mx, mon->my);
                delay_output_milliseconds(200);
            }
            pline_The("magic was deadly...");
            if (!already_killed)
                killed(mon);  //xkilled(mon, XKILL_NOMSG);
            destroyed = TRUE; /* return FALSE; */
        } 
        else if (destroyed) 
        {
            if (!already_killed)
            {
                if (isdisintegrated)
                    disintegrate_mon(mon, 1, xname(obj));
                else
                    killed(mon); /* takes care of most messages */
            }
        }
        else if (u.umconf && hand_to_hand) 
        {
            nohandglow(mon);
            if (!is_confused(mon) && !check_ability_resistance_success(mon, A_WIS, 0))
            {
                int duration = get_otyp_spell_duration(SCR_CONFUSE_MONSTER);
                increase_mon_property(mon, CONFUSION, duration);
                if (!is_stunned(mon) && mon_can_move(mon)
                    && canseemon(mon))
                {
                    play_sfx_sound_at_location(SFX_ACQUIRE_CONFUSION, mon->mx, mon->my);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s appears confused.", Monnam(mon));
                }
            }
        }

    }

    if (obj && (objectshatters || (!uses_spell_flags
        && (objects[obj->otyp].oc_aflags & A1_ITEM_VANISHES_ON_HIT)
        && (
            !(objects[obj->otyp].oc_aflags & A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET)
            || ((objects[obj->otyp].oc_aflags & A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET) && eligible_for_extra_damage(obj, mon, &youmonst))
           )
        )
     ))
    {
        *obj_destroyed = TRUE;
        if (*u.ushops || obj->unpaid)
            check_shop_obj(obj, mx, my, TRUE, FALSE);

        if (obj->where == OBJ_INVENT)
        {
            debugprint("hmon_hitmon7: %d", obj->otyp);
            if (obj->quan > 1)
                useup(obj);
            else
            {
                if (obj == uwep)
                    uwepgone(); /* set unweapon */
                else if (obj == uarms)
                    uwep2gone(); /* set unweapon */
                debugprint("hmon_hitmon: %d", obj->otyp);
                useupall(obj);
                obj = 0;
            }
        }
        else if (obj->where == OBJ_FLOOR)
        {
            /* in case MON_AT+enexto for invisible mon */
            int x = obj->ox, y = obj->oy;
            /* not useupf(), which charges */
            debugprint("hmon_hitmon-delobj1: %d", obj->otyp);
            delobj(obj);
            newsym(x, y);
            obj = (struct obj*)0;
        }
        else if (obj->where == OBJ_FREE)
        {
            debugprint("hmon_hitmon3: %d", obj->otyp);
            obfree(obj, (struct obj*)0);
            obj = (struct obj*)0;
        }
        update_inventory();
    }

    if(obj)
    {
        //No longer messages
        if (unpoisonmsg)
        {
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(150);
                play_sfx_sound(SFX_WEAPON_NO_LONGER_POISONED);
            }
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s no longer poisoned.", saved_oname,
                vtense(saved_oname, "are"));
        }

        if (unenchantmsg)
        {
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(150);
                play_sfx_sound(SFX_WEAPON_NO_LONGER_ENCHANTED);
            }
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s no longer enchanted.", saved_oname,
                vtense(saved_oname, "are"));
        }

    }

    return destroyed ? FALSE : TRUE;
}

STATIC_OVL boolean
shade_aware(obj)
struct obj *obj;
{
    if (!obj)
        return FALSE;
    /*
     * The things in this list either
     * 1) affect shades.
     *  OR
     * 2) are dealt with properly by other routines
     *    when it comes to shades.
     */
    if (obj->otyp == BOULDER
        || obj->otyp == HEAVY_IRON_BALL
        || obj->otyp == IRON_CHAIN      /* weapon_dmg_value handles those first three */
        || obj->otyp == MIRROR          /* silver in the reflective surface */
        || obj->otyp == MAGIC_MIRROR          /* silver in the reflective surface */
        || obj->otyp == CLOVE_OF_GARLIC /* causes shades to flee */
        || obj_counts_as_silver(obj))
        return TRUE;
    return FALSE;
}

/* check whether slippery clothing protects from hug or wrap attack */
/* [currently assumes that you are the attacker] */
boolean
m_slips_free(mdef, mattk)
struct monst *mdef;
struct attack *mattk;
{
    struct obj *obj;

    if (mattk->adtyp == AD_DRIN) {
        /* intelligence drain attacks the head */
        obj = which_armor(mdef, W_ARMH);
    } else {
        /* grabbing attacks the body */
        obj = which_armor(mdef, W_ARMC); /* cloak */
        if (!obj)
            obj = which_armor(mdef, W_ARM); /* suit */
        if (!obj)
            obj = which_armor(mdef, W_ARMU); /* shirt */
    }

    /* if monster's cloak/armor is greased, your grab slips off; this
       protection might fail (33% chance) when the armor is cursed */
    if (obj && (obj->greased || (objects[obj->otyp].oc_flags5 & O5_PERMANENTLY_GREASED) != 0 || obj->otyp == OILSKIN_CLOAK)
        && (!obj->cursed || rn2(3))) {
        You("%s %s %s %s!",
            mattk->adtyp == AD_WRAP ? "slip off of"
                                    : "grab, but cannot hold onto",
            s_suffix(mon_nam(mdef)), obj->greased ? "greased" : "slippery",
            /* avoid "slippery slippery cloak"
               for undiscovered oilskin cloak */
            (obj->greased || objects[obj->otyp].oc_name_known)
                ? xname(obj)
                : cloak_simple_name(obj));

        if (obj->greased && !rn2(2)) {
            pline_The("grease wears off.");
            obj->greased = 0;
        }
        return TRUE;
    }
    return FALSE;
}

/* used when hitting a monster with a lance while mounted;
   1: joust hit; 0: ordinary hit; -1: joust but break lance */
STATIC_OVL int
joust(mon, obj)
struct monst *mon; /* target */
struct obj *obj;   /* weapon */
{
    int skill_chance = 0, joust_dieroll;

    if (Fumbling || Stunned)
        return 0;
    /* sanity check; lance must be wielded in order to joust */
    if (obj != uwep && obj != uarms)
        return 0;

    int skill_level = P_SKILL_LEVEL(weapon_skill_type(obj));
    if (u.twoweap && skill_level > P_SKILL_LEVEL(P_DUAL_WEAPON_COMBAT))
        skill_level = P_SKILL_LEVEL(P_DUAL_WEAPON_COMBAT);

    skill_chance += spear_skill_jousting_bonus(skill_level); /* thrusting weapon skill */
    skill_chance += spear_skill_jousting_bonus(P_SKILL_LEVEL(P_RIDING)); /* riding skill */

    if ((joust_dieroll = rn2(100)) < skill_chance)
    {
        int roll_threshold = 5 - min(obj->exceptionality, EXCEPTIONALITY_CELESTIAL) + 10 * greatest_erosion(obj);        
        if (joust_dieroll < roll_threshold && rnl(50) == (50 - 1) 
            && !unsolid(mon->data) && !is_incorporeal(mon->data)
            && !obj_resists(obj, 0, 100))
            return -1; /* hit that breaks lance */
        return 1;      /* successful joust */
    }
    return 0; /* no joust bonus; revert to ordinary attack */
}

/*
 * Send in a demon pet for the hero.  Exercise wisdom.
 *
 */
STATIC_OVL void
demonpet()
{
    int i;
    struct permonst *pm;
    struct monst *dtmp;

    i = !rn2(6) ? ndemon(u.ualign.type, FALSE, TRUE) : NON_PM;
    pm = i != NON_PM ? &mons[i] : youmonst.data;
    if ((dtmp = makemon(pm, u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END)) != 0)
    {
        (void)tamedog(dtmp, (struct obj*) 0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);

        dtmp->issummoned = TRUE;
        dtmp->disregards_enemy_strength = TRUE;
        dtmp->disregards_own_health = FALSE;
        dtmp->hasbloodlust = TRUE;
        if ((objects[SPE_SUMMON_DEMON].oc_spell_dur_dice > 0 && objects[SPE_SUMMON_DEMON].oc_spell_dur_diesize > 0) || objects[SPE_SUMMON_DEMON].oc_spell_dur_plus > 0)
        {
            dtmp->summonduration = d(objects[SPE_SUMMON_DEMON].oc_spell_dur_dice, objects[SPE_SUMMON_DEMON].oc_spell_dur_diesize) + objects[SPE_SUMMON_DEMON].oc_spell_dur_plus;
            begin_summontimer(dtmp);
        }

        play_sfx_sound_at_location(SFX_SUMMON_DEMON, dtmp->mx, dtmp->my);
        if (canseemon(dtmp))
        {
            if (is_demon(dtmp->data))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s appears in a cloud of smoke!", Amonnam(dtmp));
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s appears!", Amonnam(dtmp));
        }
    }
    exercise(A_WIS, TRUE);
}

STATIC_OVL boolean
theft_petrifies(otmp)
struct obj *otmp;
{
    if (uarmg || otmp->otyp != CORPSE || otmp->corpsenm < LOW_PM
        || !touch_petrifies(&mons[otmp->corpsenm]) || Stone_resistance)
        return FALSE;

#if 0   /* no poly_when_stoned() critter has theft capability */
    if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM)) {
        display_nhwindow(WIN_MESSAGE, FALSE);   /* --More-- */
        return TRUE;
    }
#endif

    /* stealing this corpse is fatal... */
    killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
    instapetrify(corpse_xname(otmp, "stolen", CXN_ARTICLE));
    /* apparently wasn't fatal after all... */
    return TRUE;
}

/*
 * Player uses theft attack against monster.
 *
 * If the target is wearing body armor, take all of its possessions;
 * otherwise, take one object.  [Is this really the behavior we want?]
 */
STATIC_OVL void
steal_it(mdef, mattk)
struct monst *mdef;
struct attack *mattk;
{
    struct obj *otmp, *stealoid, **minvent_ptr;
    int64_t unwornmask;

    if (!mdef->minvent)
        return; /* nothing to take */

    /* look for worn body armor */
    stealoid = (struct obj *) 0;
    if (could_seduce(&youmonst, mdef, mattk)) {
        /* find armor, and move it to end of inventory in the process */
        minvent_ptr = &mdef->minvent;
        while ((otmp = *minvent_ptr) != 0)
            if (otmp->owornmask & W_ARM) {
                if (stealoid)
                {
                    panic("steal_it: multiple worn suits");
                    return;
                }
                *minvent_ptr = otmp->nobj; /* take armor out of minvent */
                stealoid = otmp;
                stealoid->nobj = (struct obj *) 0;
            } else {
                minvent_ptr = &otmp->nobj;
            }
        *minvent_ptr = stealoid; /* put armor back into minvent */
    }

    if (stealoid) { /* we will be taking everything */
        if (gender(mdef) == (int) u.mfemale && youmonst.data->mlet == S_NYMPH)
            You("charm %s.  She gladly hands over her possessions.",
                mon_nam(mdef));
        else
            You("seduce %s and %s starts to take off %s clothes.",
                mon_nam(mdef), mhe(mdef), mhis(mdef));
    }

    while ((otmp = mdef->minvent) != 0) {
        if (!Upolyd)
            break; /* no longer have ability to steal */
        /* take the object away from the monster */
        debugprint("steal_it");
        obj_extract_self(otmp);
        if ((unwornmask = otmp->owornmask) != 0L) {
            mdef->worn_item_flags &= ~unwornmask;
            if (otmp->owornmask & W_WEP)
                setmnotwielded(mdef, otmp);
            otmp->owornmask = 0L;
            update_all_mon_statistics(mdef, FALSE);
            if (mdef == u.usteed && otmp->otyp == SADDLE)
                dismount_steed(DISMOUNT_FELL);

            /* give monster a chance to wear other equipment on its next
               move instead of waiting until it picks something up */
            check_mon_wearable_items_next_turn(mdef);

            if (otmp == stealoid) /* special message for final item */
                pline("%s finishes taking off %s suit.", Monnam(mdef),
                      mhis(mdef));
        }
        /* give the object to the character */
        otmp = hold_another_object(otmp, "You snatched but dropped %s.",
                                   doname(otmp), "You steal: ", TRUE);
        /* might have dropped otmp, and it might have broken or left level */
        if (!otmp || otmp->where != OBJ_INVENT)
            continue;
        if (theft_petrifies(otmp))
            break; /* stop thieving even though hero survived */
        /* more take-away handling, after theft message */
        if (unwornmask & W_WEP) { /* stole wielded weapon */
            possibly_unwield(mdef, FALSE);
        } else if (unwornmask & W_ARMG) { /* stole worn gloves */
            mselftouch(mdef, (const char *) 0, TRUE);
            if (DEADMONSTER(mdef)) /* it's now a statue */
                return;         /* can't continue stealing */
        }

        if (!stealoid)
            break; /* only taking one item */
    }
}

int
damageum(mdef, mattk, omonwep, specialdmg)
register struct monst *mdef;
register struct attack *mattk;
struct obj* omonwep;
int specialdmg; /* blessed and/or silver bonus against various things */
{
    register struct permonst *pd = mdef->data;
    boolean negated;
    struct obj *mongold;
    int chance = 0;
    double poisondamage = 0;

    int extratmp = 0;
    double damage = 0;
    enum p_skills wtype = P_BARE_HANDED_COMBAT;
    boolean incorrect_weapon_use = FALSE;
    enum hit_tile_types hit_tile = HIT_GENERAL;

    int mx = mdef->mx, my = mdef->my;
    boolean was_invis_glyph = isok(mx, my) && glyph_is_invisible(levl[mx][my].hero_memory_layers.glyph);

    /*  First determine the base damage done */
    struct obj* mweapon = omonwep;
    if (mweapon && mattk->aatyp == AT_WEAP)
    {
        wtype = weapon_skill_type(mweapon);

        int basedmg = 0;
        //Use weapon damage
        if (is_launcher(mweapon) || is_ammo(mweapon) || is_missile(mweapon))
            basedmg = d(1, 2), incorrect_weapon_use = TRUE;
        else
            basedmg = weapon_dmg_value(mweapon, mdef, &youmonst, 0);

        damage += adjust_damage(basedmg, &youmonst, mdef, objects[mweapon->otyp].oc_damagetype, ADFLAGS_NONE);
        extratmp = weapon_extra_dmg_value(mweapon, mdef, &youmonst, basedmg);
        damage += adjust_damage(extratmp, &youmonst, mdef, objects[mweapon->otyp].oc_damagetype, ADFLAGS_NONE);
    }
    else
    {
        int tmp = 0;
        //Use stats from ATTK
        if (mattk->damn > 0 && mattk->damd > 0)
            tmp += d((int)mattk->damn, (int)mattk->damd);
        tmp += (int)mattk->damp;
        damage += adjust_damage(tmp, &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
    }

    if (mattk->adtyp == AD_PHYS || mattk->adtyp == AD_DRIN)
    {
        if (mattk->aatyp == AT_WEAP || mattk->aatyp == AT_HUGS)
            damage += adjust_damage(u_str_dmg_bonus(), & youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
        else
            damage += adjust_damage(u_str_dmg_bonus() / 2, & youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
    }

    negated = Cancelled || check_magic_cancellation_success(mdef, mattk->mcadj);

    switch (mattk->adtyp) {
    case AD_STUN:
        if (!resists_stun(mdef))
        {
            if (!Blind)
                pline("%s %s for a moment.", Monnam(mdef), makeplural(stagger(pd, "stagger")));

            nonadditive_increase_mon_property(mdef, STUNNED, 5 + rnd(5));
        }
        goto physical;
    case AD_LEGS:
#if 0
        if (u.ucancelled) {
            damage = 0;
            break;
        }
#endif
        goto physical;
    case AD_WERE: /* no special effect on monsters */
    case AD_HEAL: /* likewise */
    case AD_PHYS:
 physical:
        if (is_shade(pd)) 
        {
            damage = 0;
            //if (!specialdmg)
            //    impossible("bad shade attack function flow?");
        }
        damage += adjust_damage(specialdmg, &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);

        if (mattk->aatyp == AT_WEAP)
        {
            /* hmonas() uses known_hitum() to deal physical damage,
               then also damageum() for non-AD_PHYS; don't inflict
               extra physical damage for unusual damage types */
            damage = 0;
        } 
        else if (mattk->aatyp == AT_KICK
                   || mattk->aatyp == AT_CLAW
                   || mattk->aatyp == AT_TUCH
                   || mattk->aatyp == AT_HUGS)
        {
            if (thick_skinned(pd))
                damage = (mattk->aatyp == AT_KICK) ? 0 : damage / 2;

            /* add ring(s) of increase damage */
            if (u.ubasedaminc + u.udaminc > 0)
            {
                /* applies even if damage was 0 */
                damage += adjust_damage(u.ubasedaminc + u.udaminc, & youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
            }
            else if (damage > 0) 
            {
                /* ring(s) might be negative; avoid converting
                   0 to non-0 or positive to non-positive */
                damage += adjust_damage(u.udaminc, &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
                if (damage < 1)
                    damage = 1;
            }
        }
        break;
    case AD_FIRE:
#if 0
        if (negated)
        {
            damage = 0;
            break;
        }
#endif
        hit_tile = HIT_ON_FIRE;
        if (!Blind)
            pline_ex(ATR_NONE, HI_FIRE, "%s is %s!", Monnam(mdef), on_fire(pd, mattk));
        if (completelyburns(pd)) { /* paper golem or straw golem */
            if (!Blind)
                pline_ex(ATR_NONE, HI_FIRE, "%s burns completely!", Monnam(mdef));
            else
                You_ex(ATR_NONE, HI_FIRE, "smell burning%s.",
                    (pd == &mons[PM_PAPER_GOLEM]) ? " paper"
                      : (pd == &mons[PM_STRAW_GOLEM]) ? " straw" : " material");
            xkilled(mdef, XKILL_NOMSG | XKILL_NOCORPSE);
            damage = 0;
            break;
            /* Don't return yet; keep hp<1 and damage=0 for pet msg */
        }
        damage += adjust_damage(destroy_mitem(mdef, SCROLL_CLASS, AD_FIRE), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
        damage += adjust_damage(destroy_mitem(mdef, SPBOOK_CLASS, AD_FIRE), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);

        if (is_mon_immune_to_fire(mdef))
        {
            if (!Blind)
                pline_The("fire doesn't heat %s!", mon_nam(mdef));
            golemeffects(mdef, AD_FIRE, damage);
            m_shieldeff(mdef);
            damage = 0;
        }

        /* only potions damage resistant players in destroy_item */
        damage += adjust_damage(destroy_mitem(mdef, POTION_CLASS, AD_FIRE), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);

        break;
    case AD_COLD:
#if 0

        if (*negated)
        {
            damage = 0;
            break;
        }
#endif
        hit_tile = HIT_FROZEN;
        if (!Blind)
            pline_ex(ATR_NONE, HI_ICE, "%s is covered in frost!", Monnam(mdef));

        if (is_mon_immune_to_cold(mdef)) 
        {
            m_shieldeff(mdef);
            if (!Blind)
                pline_The("frost doesn't chill %s!", mon_nam(mdef));
            golemeffects(mdef, AD_COLD, damage);
            damage = 0;
        }

        damage += adjust_damage(destroy_mitem(mdef, POTION_CLASS, AD_COLD), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
        break;
    case AD_ELEC:
#if 0
        if (negated) 
        {
            damage = 0;
            break;
        }
#endif

        hit_tile = HIT_ELECTROCUTED;
        if (!Blind)
            pline_ex(ATR_NONE, HI_ZAP, "%s is zapped!", Monnam(mdef));

        damage += adjust_damage(destroy_mitem(mdef, WAND_CLASS, AD_ELEC), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
        
        if (is_mon_immune_to_elec(mdef)) 
        {
            if (!Blind)
                pline_The("zap doesn't shock %s!", mon_nam(mdef));
            golemeffects(mdef, AD_ELEC, damage);
            m_shieldeff(mdef);
            damage = 0;
        }
        /* only rings damage resistant players in destroy_item */
        damage += adjust_damage(destroy_mitem(mdef, RING_CLASS, AD_ELEC), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
        break;
    case AD_ACID:
        if (is_mon_immune_to_acid(mdef))
            damage = 0;
        hit_tile = HIT_SPLASHED_ACID;
        break;
    case AD_STON:
        if (!negated && !resists_ston(mdef))
        {
            display_m_being_hit(mdef, HIT_PETRIFIED, 0, 0UL, FALSE);
            if (mattk->aatyp == AT_GAZE)
                minstapetrify(mdef, TRUE);
            else
                start_delayed_petrification(mdef, TRUE);
        }
        damage = 0;
        break;
    case AD_SSEX:
    case AD_SEDU:
    case AD_SITM:
        if(!resists_charm(mdef) && !check_magic_cancellation_success(mdef, mattk->mcadj))
            steal_it(mdef, mattk);
        damage = 0;
        break;
    case AD_SGLD:
        /* This you as a leprechaun, so steal
           real gold only, no lesser coins */
        mongold = findgold(mdef->minvent);
        if (mongold) {
            debugprint("damageum");
            obj_extract_self(mongold);
            if (merge_choice(invent, mongold) || inv_cnt(FALSE) < 52) {
                addinv(mongold);
                Your_ex(ATR_NONE, CLR_MSG_SUCCESS, "purse feels heavier.");
            } else {
                play_ui_sound(UI_SOUND_KNAPSACK_FULL);
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "grab %s's gold, but find no room in your inventory.",
                    mon_nam(mdef));
                dropyf(mongold);
            }
        }
        exercise(A_DEX, TRUE);
        damage = 0;
        break;
    case AD_TLPT:
        if (damage <= 0)
            damage = 1;

        if (!negated) 
        {
            char nambuf[BUFSZ];
            boolean u_saw_mon = (canseemon(mdef)
                                 || (u.uswallow && u.ustuck == mdef));

            /* record the name before losing sight of monster */
            Strcpy(nambuf, Monnam(mdef));
            if (u_teleport_mon(mdef, FALSE) && u_saw_mon
                && !(canseemon(mdef) || (u.uswallow && u.ustuck == mdef)))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly disappears!", nambuf);
            
            if (damage >= (double)mdef->mhp) 
            { /* see hitmu(mhitu.c) */
                if (mdef->mhp == 1)
                    ++mdef->mhp;
                damage = (double)mdef->mhp - 1;
            }
        }
        break;
    case AD_BLND:
        if (can_blnd(&youmonst, mdef, mattk->aatyp, (struct obj *) 0)) 
        {
            if (!Blind && !is_blinded(mdef))
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is blinded.", Monnam(mdef));
            nonadditive_increase_mon_property(mdef, BLINDED, 1 + (int)ceil(damage));
        }
        damage = 0;
        break;
    case AD_CURS:
        if (night() && !rn2(10) && !is_cancelled(mdef)) {
            if (pd == &mons[PM_CLAY_GOLEM]) {
                if (!Blind)
                    pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "Some writing vanishes from %s head!",
                          s_suffix(mon_nam(mdef)));
                xkilled(mdef, XKILL_NOMSG);
                /* Don't return yet; keep hp<1 and damage=0 for pet msg */
            } else {
                mdef->mprops[CANCELLED] = 100 + rnd(50);
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "chuckle.");
            }
        }
        damage = 0;
        break;
    case AD_DRLI:
        if (!negated && !resists_drli(mdef)) //!rn2(3) && 
        {
            hit_tile = HIT_DRAIN_LEVEL;
            int xtmp = d(2, 6);

            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s suddenly seems weaker!", Monnam(mdef));
            mdef->mbasehpdrain -= xtmp;
            update_mon_maxhp(mdef);
            deduct_monster_hp(mdef, adjust_damage(xtmp, &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE));
            /* !m_lev: level 0 monster is killed regardless of hit points
               rather than drop to level -1 */
            if (DEADMONSTER(mdef) || !mdef->m_lev) 
            {
                pline("%s dies!", Monnam(mdef));
                xkilled(mdef, XKILL_NOMSG);
            } 
            else
                mdef->m_lev--;

            damage = 0;
        }
        break;
    case AD_RUST:
        if (is_iron(pd)) 
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s falls to pieces!", Monnam(mdef));
            xkilled(mdef, XKILL_NOMSG);
        }
        erode_armor(mdef, ERODE_RUST);
        damage = 0;
        break;
    case AD_CORR:
        erode_armor(mdef, ERODE_CORRODE);
        damage = 0;
        break;
    case AD_DCAY:
        if (pd == &mons[PM_WOOD_GOLEM] || pd == &mons[PM_LEATHER_GOLEM]) {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s falls to pieces!", Monnam(mdef));
            xkilled(mdef, XKILL_NOMSG);
        }
        erode_armor(mdef, ERODE_ROT);
        damage = 0;
        break;
    case AD_DREN:
        if (!negated && !rn2(4))
            xdrainenergym(mdef, TRUE);
        damage = 0;
        break;
    case AD_DRST:
    case AD_DRDX:
    case AD_DRCO:
        if (!negated && !rn2(2)) 
        {
            Your("%s was poisoned!", mpoisons_subj(&youmonst, mattk));
            if (resists_poison(mdef)) 
            {
                pline_The("poison doesn't seem to affect %s.", mon_nam(mdef));
            } 
            else 
            {
                play_sfx_sound_at_location(SFX_MONSTER_IS_POISONED, mdef->mx, mdef->my);
                hit_tile = HIT_POISONED;
                if (!rn2(10))
                {
                    poisondamage = adjust_damage(d(6, 6) + 10, &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE); // mdef->mhp;
                } else
                    poisondamage = adjust_damage(rn1(10, 6), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
                damage += poisondamage;
            }
        }
        break;
    case AD_DRIN: {
        struct obj *helmet;

        if (notonhead || !has_head(pd)) {
            pline("%s doesn't seem harmed.", Monnam(mdef));
            damage = 0;
            if (!Unchanging && pd == &mons[PM_GREEN_SLIME]) {
                if (!Slimed) {
                    play_sfx_sound(SFX_START_SLIMING);
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "suck in some slime and don't feel very well.");
                    make_slimed(10L, (char *) 0, KILLED_BY, "sucking in some green slime", HINT_KILLED_ATE_GREEN_SLIME);
                }
            }
            break;
        }
        if (m_slips_free(mdef, mattk))
            break;

        if ((helmet = which_armor(mdef, W_ARMH)) != 0 && rn2(8)) {
            pline("%s %s blocks your attack to %s head.",
                  s_suffix(Monnam(mdef)), helm_simple_name(helmet),
                  mhis(mdef));
            break;
        }

        (void) eat_brains(&youmonst, mdef, TRUE, &damage);
        break;
    }
    case AD_STCK:
        if (!negated && !sticks(pd))
            u.ustuck = mdef; /* it's now stuck to you */
        break;
    case AD_WRAP:
        if (!sticks(pd)) {
            if (!u.ustuck && !rn2(10)) {
                if (m_slips_free(mdef, mattk)) {
                    damage = 0;
                } else {
                    play_sfx_sound(SFX_ACQUIRE_GRAB);
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "swing yourself around %s!", mon_nam(mdef));
                    u.ustuck = mdef;
                }
            } else if (u.ustuck == mdef) {
                /* Monsters don't wear amulets of magical breathing */
                if (is_pool(u.ux, u.uy) && !is_swimmer(pd)
                    && !amphibious(pd)) {
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "drown %s...", mon_nam(mdef));
                    damage = mdef->mhp;
                } else if (mattk->aatyp == AT_HUGS)
                    pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is being crushed.", Monnam(mdef));
            } else {
                damage = 0;
                if (flags.verbose)
                    You("brush against %s %s.", s_suffix(mon_nam(mdef)),
                        mbodypart(mdef, LEG));
            }
        } else
            damage = 0;
        break;
    case AD_PLYS:
        if (!negated && !resists_paralysis(mdef) && damage < ((double)mdef->mhp + ((double)mdef->mhp_fraction)/10000))
        {
            hit_tile = HIT_PARALYZED;
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is frozen by you!", Monnam(mdef));
            paralyze_monst(mdef, 2 + rnd(8), FALSE);
        }
        break;
    case AD_SHRP:
        if (rn2(100) < SHARPNESS_PERCENTAGE_CHANCE)
        {
            hit_tile = HIT_CRITICAL;
            damage += adjust_damage((mdef->mhpmax * SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE) / 100, &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
        }
        break;
    case AD_SLEE:
        if (!negated && !mdef->msleeping && sleep_monst(mdef, (struct obj*)0, &youmonst, rn1(3,8), mattk->mcadj, FALSE)) 
        {
            hit_tile = HIT_SLEEP;
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is put to sleep by you!", Monnam(mdef));
            slept_monst(mdef);
        }
        break;
    case AD_SLIM:
        if (negated)
            break; /* physical damage only */
        if (!rn2(4) && !resists_slime(mdef))
        {
            hit_tile = HIT_SLIMED;
            if (!munslime(mdef, TRUE) && !DEADMONSTER(mdef)) {
                /* this assumes newcham() won't fail; since hero has
                   a slime attack, green slimes haven't been geno'd */
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "turn %s into slime.", mon_nam(mdef));
                if (newcham(mdef, &mons[PM_GREEN_SLIME], 0, FALSE, FALSE))
                    pd = mdef->data;
            }
            /* munslime attempt could have been fatal */
            if (DEADMONSTER(mdef))
                return 2; /* skip death message */
            damage = 0;
        }
        break;
    case AD_ENCH: /* KMH -- remove enchantment (disenchanter) */
        /* there's no msomearmor() function, so just do damage */
        /* if (negated) break; */
        break;
    case AD_SLOW:
        if (!negated && ((mdef->mprops[SLOWED] & M_TIMEOUT) < 300))
        {
            hit_tile = HIT_SLOW;
            (void)increase_mon_property_verbosely(mdef, SLOWED, 20 + rnd(10));
        }
        break;
    case AD_CONF:
        if (!is_confused(mdef)) 
        {
            increase_mon_property_verbosely(mdef, CONFUSION, 10 + rnd(10));
        }
        break;
    case AD_DMNS:
        damage = 0;
        chance = mattk->damp;
        if(chance > 0 && !Summon_forbidden)
        {
            if (rn2(100) < chance) {
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "gate in some help!");
                demonpet();
                return 0;
            }
            else
            {
                You("attempt to gate in some help, but nothing happens.");
            }
        }
        break;
    case AD_UNDO: //Not implemented, demon prince ability
    case AD_GHUL: //Not implemented, demon prince ability
    case AD_GNOL: //Not implemented, demon prince ability
        damage = 0;
        break;
    case AD_LYCA: //Not implemented
        damage = 0;
        break;
    default:
        damage = 0;
        break;
    }

    /* Skill-based critical strike */
    boolean skill_critical_success = FALSE;
    if (damage > 0 && !incorrect_weapon_use)
    {
        int skill_crit_chance = get_skill_critical_strike_chance(wtype, FALSE, TRUE, 0, TRUE, FALSE);
        if (skill_crit_chance > 0 && rn2(100) < skill_crit_chance)
        {
            skill_critical_success = TRUE;
            damage *= 2;
        }
    }

    int hp_before = mdef->mhp;
    deduct_monster_hp(mdef, damage);
    int hp_after = mdef->mhp;
    int damagedealt = hp_before - hp_after;
    play_monster_weapon_hit_sound(&youmonst, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mdef), get_pm_attack_index(youmonst.data, mattk), mweapon, damage, HMON_MELEE);
    remove_monster_and_nearby_waitforu(mdef);

    if (DEADMONSTER(mdef)) 
    {
        if(poisondamage > 0 && ((double)mdef->mhp + ((double)mdef->mhp_fraction)/10000 - 1) > -poisondamage)
            Your_ex(ATR_NONE, CLR_MSG_SUCCESS, "poison was deadly...");
        
        if (is_tame(mdef) && !cansee(mdef->mx, mdef->my)) 
        {
            You_feel("embarrassed for a moment.");
            if (damage > 0)
                xkilled(mdef, XKILL_NOMSG); /* !tmp but hp<1: already killed */
        } 
        else if (!flags.verbose) 
        {
            You("destroy it!");
            if (damage > 0)
                xkilled(mdef, XKILL_NOMSG);
        } 
        else if (damage > 0)
            killed(mdef);

        if ((windowprocs.wincap2 & WC2_FADING_ANIMATIONS) != 0 && was_invis_glyph && !glyph_is_invisible(levl[mx][my].hero_memory_layers.glyph))
        {
            play_special_effect_with_details_at(0, mx, my, GLYPH_INVISIBLE, LAYER_GENERAL_EFFECT, -2, 0, 0, 20, FALSE);
            special_effect_wait_until_action(0);
            special_effect_wait_until_end(0);
        }
        return 2;
    } 
    else if (flags.verbose && damagedealt > 0)
    {
        if (skill_critical_success)
        {
            if (hit_tile == HIT_GENERAL)
                hit_tile = HIT_CRITICAL;
            Your_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_orange1, "critical strike inflicts %d damage!", damagedealt);
        }
        else
        {
            You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_orange1, "inflict %d damage.", damagedealt);
        }
        display_m_being_hit(mdef, hit_tile, damagedealt, 0UL, FALSE);
    }
    return 1;
}

void
remove_monster_and_nearby_waitforu(mdef)
struct monst* mdef;
{
    if (!mdef)
        return;

    mdef->mstrategy &= ~STRAT_WAITFORU; /* in case player is very fast */
    struct monst* mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        if ((mtmp->mstrategy & STRAT_WAITFORU) != 0 && !mtmp->iswiz && m_cansee(mtmp, mdef->mx, mdef->my))
        {
            mtmp->mstrategy &= ~STRAT_WAITFORU;
        }
    }
}

STATIC_OVL int
explum(mdef, mattk)
register struct monst *mdef;
register struct attack *mattk;
{
    boolean resistance; /* only for cold/fire/elec */
    double damage = adjust_damage(d((int) mattk->damn, (int) mattk->damd), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);

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
        play_special_effect_at(spef_idx, 0, u.ux, u.uy, FALSE);
    else if (expl_idx < MAX_EXPLOSIONS)
        play_explosion_animation_at(u.ux, u.uy, expl_idx);

    if (sfx_sound != SFX_ILLEGAL)
        play_sfx_sound(sfx_sound);
    
    if (spef_idx < MAX_SPECIAL_EFFECTS)
    {
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        special_effect_wait_until_action(0);
        show_glyph_on_layer(u.ux, u.uy, NO_GLYPH, LAYER_MONSTER);
        flush_screen(1);
    }
    else if (expl_idx < MAX_EXPLOSIONS)
    {
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        explosion_wait_until_action();
        show_glyph_on_layer(u.ux, u.uy, NO_GLYPH, LAYER_MONSTER);
        flush_screen(1);
    }

    You_ex(ATR_NONE, CLR_MSG_WARNING, "explode!");
    switch (mattk->adtyp) 
    {
    case AD_BLND:
        if (!resists_blnd(mdef)) 
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is blinded by your flash of light!", Monnam(mdef));
            increase_mon_property(mdef, BLINDED, 1 + (int)ceil(damage));
        }
        break;
    case AD_HALU:
        if (haseyes(mdef->data) && !is_blinded(mdef)) 
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s is affected by your flash of light!", Monnam(mdef));
            increase_mon_property_verbosely(mdef, HALLUC, 100 + rnd(100));
        }
        break;
    case AD_COLD:
        resistance = is_mon_immune_to_cold(mdef);
        goto common;
    case AD_FIRE:
        resistance = is_mon_immune_to_fire(mdef);
        goto common;
    case AD_ELEC:
        resistance = is_mon_immune_to_elec(mdef);
 common:
        if (!resistance) 
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s gets blasted!", Monnam(mdef));
            deduct_monster_hp(mdef, damage);
            if (DEADMONSTER(mdef)) 
            {
                killed(mdef);

                if (spef_idx < MAX_SPECIAL_EFFECTS)
                {
                    special_effect_wait_until_end(0);
                    context.global_newsym_flags = 0UL;
                }

                return 2;
            }
        } 
        else
        {
            m_shieldeff(mdef);
            if (is_golem(mdef->data))
                golemeffects(mdef, (int) mattk->adtyp, damage);
            else
                pline_The("blast doesn't seem to affect %s.", mon_nam(mdef));
        }
        break;
    default:
        break;
    }

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

    return 1;
}

STATIC_OVL void
start_engulf(mdef)
struct monst *mdef;
{
    if (!Invisib) 
    {
        map_location(u.ux, u.uy, TRUE);
        tmp_at(DISP_ALWAYS, any_mon_to_glyph(&youmonst, rn2_on_display_rng));
        tmp_at(mdef->mx, mdef->my);
    }
    You("engulf %s!", mon_nam(mdef));
    adjusted_delay_output();
    adjusted_delay_output();
}

STATIC_OVL void
end_engulf()
{
    if (!Invisib) {
        tmp_at(DISP_END, 0);
        newsym(u.ux, u.uy);
    }
}

STATIC_OVL int
gulpum(mdef, mattk)
register struct monst *mdef;
register struct attack *mattk;
{
#ifdef LINT /* static char msgbuf[BUFSZ]; */
    char msgbuf[BUFSZ];
#else
    static char msgbuf[BUFSZ]; /* for nomovemsg */
#endif
    register int tmp;
    //register int dam = d((int) mattk->damn, (int) mattk->damd);
    double damage = adjust_damage(d((int)mattk->damn, (int)mattk->damd), &youmonst, mdef, mattk->adtyp, ADFLAGS_NONE);
    boolean fatal_gulp;
    struct obj *otmp;
    struct permonst *pd = mdef->data;

    /* Not totally the same as for real monsters.  Specifically, these
     * don't take multiple moves.  (It's just too hard, for too little
     * result, to program monsters which attack from inside you, which
     * would be necessary if done accurately.)  Instead, we arbitrarily
     * kill the monster immediately for AD_DGST and we regurgitate them
     * after exactly 1 round of attack otherwise.  -KAA
     */

    if (!engulf_target(&youmonst, mdef))
        return 0;

    if (u.uhunger < NUTRITION_FULL_AMOUNT && !u.uswallow)
    {
        for (otmp = mdef->minvent; otmp; otmp = otmp->nobj)
            (void) snuff_lit(otmp);

        /* force vampire in bat, cloud, or wolf form to revert back to
           vampire form now instead of dealing with that when it dies */
        if (is_vampshifter(mdef)
            && newcham(mdef, &mons[mdef->cham], mdef->cham_subtype, FALSE, FALSE))
        {
            You("engulf it, then expel it.");
            if (canspotmon(mdef))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "It turns into %s.", a_monnam(mdef));
            else
                map_invisible(mdef->mx, mdef->my);
            return 1;
        }

        /* engulfing a cockatrice or digesting a Rider or Medusa */
        fatal_gulp = (touch_petrifies(pd) && !Stone_resistance)
                     || (mattk->adtyp == AD_DGST && (is_rider(pd) || (is_medusa(pd) && !Stone_resistance)));

        if ((mattk->adtyp == AD_DGST && !Slow_digestion) || fatal_gulp)
            eating_conducts(pd);

        if (fatal_gulp && !is_rider(pd)) 
        { /* petrification */
            char kbuf[BUFSZ];
            const char *mname = mon_monster_name(mdef);

            if (!is_mname_proper_name(pd))
                mname = an(mname);
            You("englut %s.", mon_nam(mdef));
            Sprintf(kbuf, "swallowing %s whole", mname);
            killer.hint_idx = HINT_KILLED_ATE_COCKATRICE_CORPSE;
            instapetrify(kbuf);
        } 
        else
        {
            start_engulf(mdef);
            switch (mattk->adtyp) 
            {
            case AD_DGST:
                /* eating a Rider or its corpse is fatal */
                if (is_rider(pd)) 
                {
                    pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, "Unfortunately, digesting any of it is fatal.");
                    end_engulf();
                    Sprintf(killer.name, "unwisely tried to eat %s",
                        mon_monster_name(mdef));
                    killer.format = NO_KILLER_PREFIX;
                    done(DIED);
                    return 0; /* lifesaved */
                }

                if (Slow_digestion)
                {
                    damage = 0;
                    break;
                }

                /* Use up amulet of life saving */
                if ((otmp = mlifesaver(mdef)) != 0)
                    m_useup(mdef, otmp);

                update_hunger_status(FALSE);
                /* start_engulf() issues "you engulf <mdef>" above; this
                   used to specify XKILL_NOMSG but we need "you kill <mdef>"
                   in case we're also going to get "welcome to level N+1";
                   "you totally digest <mdef>" will be coming soon (after
                   several turns) but the level-gain message seems out of
                   order if the kill message is left implicit */
                xkilled(mdef, XKILL_GIVEMSG | XKILL_NOCORPSE);
                if (!DEADMONSTER(mdef)) { /* monster lifesaved */
                    You("hurriedly regurgitate the sizzling in your %s.",
                        body_part(STOMACH));
                } else {
                    tmp = 1 + (pd->cwt >> 8);
                    if (corpse_chance(mdef, &youmonst, TRUE)
                        && !(mvitals[mdef->mnum].mvflags & MV_NOCORPSE))
                    {
                        /* nutrition only if there can be a corpse */
                        u.uhunger += (pd->cnutrit + 1) / 2;
                    } 
                    else
                        tmp = 0;

                    Sprintf(msgbuf, "You totally digest %s.", mon_nam(mdef));
                    if (tmp != 0) 
                    {
                        /* setting afternmv = end_engulf is tempting,
                         * but will cause problems if the player is
                         * attacked (which uses his real location) or
                         * if his See_invisible wears off
                         */
                        You("digest %s.", mon_nam(mdef));
                        if (Slow_digestion)
                            tmp *= 2;
                        nomul(-tmp);
                        multi_reason = "digesting something";
                        nomovemsg = msgbuf;
                        nomovemsg_attr = ATR_NONE;
                        nomovemsg_color = NO_COLOR;
                    }
                    else
                        pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, msgbuf);

                    if (pd == &mons[PM_GREEN_SLIME]) 
                    {
                        Sprintf(msgbuf, "%s isn't sitting well with you.",
                                The(mon_monster_name(mdef)));
                        if (!Unchanging)
                        {
                            play_sfx_sound(SFX_START_SLIMING);
                            make_slimed(5L, (char *) 0, KILLED_BY, "trying to digest green slime", HINT_KILLED_ATE_GREEN_SLIME);
                        }
                    } 
                    else
                        exercise(A_CON, TRUE);
                }
                end_engulf();
                return 2;
            case AD_PHYS:
                if (youmonst.data == &mons[PM_FOG_CLOUD])
                {
                    pline("%s is laden with your moisture.", Monnam(mdef));
                    if (amphibious(pd) && !flaming(pd))
                    {
                        damage = 0;
                        pline("%s seems unharmed.", Monnam(mdef));
                    }
                } 
                else
                    pline("%s is pummeled with your debris!", Monnam(mdef));
                break;
            case AD_ACID:
                pline("%s is covered with your goo!", Monnam(mdef));
                if (is_mon_immune_to_acid(mdef)) {
                    pline("It seems harmless to %s.", mon_nam(mdef));
                    damage = 0;
                }
                break;
            case AD_BLND:
                if (can_blnd(&youmonst, mdef, mattk->aatyp,
                             (struct obj *) 0)) 
                {
                    if (!is_blinded(mdef))
                        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s can't see in there!", Monnam(mdef));

                    nonadditive_increase_mon_property(mdef, BLINDED, 1 + (int)ceil(damage));
                }
                damage = 0;
                break;
            case AD_ELEC:
                if (rn2(2))
                {
                    pline_The("air around %s crackles with electricity.",
                              mon_nam(mdef));
                    if (is_mon_immune_to_elec(mdef)) 
                    {
                        pline("%s seems unhurt.", Monnam(mdef));
                        damage = 0;
                    }
                    golemeffects(mdef, (int) mattk->adtyp, damage);
                } else
                    damage = 0;
                break;
            case AD_COLD:
                if (rn2(2)) 
                {
                    if (is_mon_immune_to_cold(mdef))
                    {
                        pline("%s seems mildly chilly.", Monnam(mdef));
                        damage = 0;
                    } else
                        pline("%s is freezing to death!", Monnam(mdef));
                    golemeffects(mdef, (int) mattk->adtyp, damage);
                } else
                    damage = 0;
                break;
            case AD_FIRE:
                if (rn2(2)) 
                {
                    if (is_mon_immune_to_fire(mdef))
                    {
                        pline("%s seems mildly hot.", Monnam(mdef));
                        damage = 0;
                    } else
                        pline("%s is burning to a crisp!", Monnam(mdef));
                    golemeffects(mdef, (int) mattk->adtyp, damage);
                } else
                    damage = 0;
                break;
            case AD_DREN:
                if (!rn2(4))
                    xdrainenergym(mdef, TRUE);
                damage = 0;
                break;
            }
            end_engulf();
            deduct_monster_hp(mdef, damage);
            remove_monster_and_nearby_waitforu(mdef);
            if (DEADMONSTER(mdef))
            {
                killed(mdef);
                if (DEADMONSTER(mdef)) /* not lifesaved */
                    return 2;
            }
            You("%s %s!", is_animal(youmonst.data) ? "regurgitate" : "expel",
                mon_nam(mdef));
            if (Slow_digestion || is_animal(youmonst.data))
            {
                pline("Obviously, you didn't like %s taste.",
                      s_suffix(mon_nam(mdef)));
            }
        }
    }
    return 0;
}

void
missum(mdef, mattk, wouldhavehit)
register struct monst *mdef;
register struct attack *mattk;
boolean wouldhavehit;
{
    if (wouldhavehit) /* monk is missing due to penalty for wearing suit */
        Your("armor is rather cumbersome...");

    if (could_seduce(&youmonst, mdef, mattk))
        You("pretend to be friendly to %s.", mon_nam(mdef));
    else if (canspotmon(mdef) && flags.verbose)
        You("miss %s.", mon_nam(mdef));
    else
        You("miss it.");
    if (mon_can_move(mdef))
        wakeup(mdef, TRUE);
}

/* attack monster as a monster; returns True if mon survives */
STATIC_OVL boolean
hmonas(mon)
register struct monst *mon;
{
    if (!mon)
        return FALSE;

    struct attack *mattk, alt_attk;
    struct obj *weapon, **originalweapon;
    boolean altwep = FALSE, weapon_used = FALSE, weapon2_used = FALSE, odd_claw = TRUE;
    int i, tmp, armorpenalty, sum[NATTK], /* nsum = 0, */ dhit = 0, attknum = 0;
    int dieroll, multi_claw = 0;

    /* with just one touch/claw/weapon attack, both rings matter;
       with more than one, alternate right and left when checking
       whether silver ring causes successful hit */
    for (i = 0; i < NATTK; i++) {
        sum[i] = 0;
        mattk = getmattk(&youmonst, mon, i, sum, &alt_attk);
        if (mattk->aatyp == AT_WEAP
            || mattk->aatyp == AT_CLAW || mattk->aatyp == AT_TUCH)
            ++multi_claw;
    }
    multi_claw = (multi_claw > 1); /* switch from count to yes/no */
    unsigned int bite_butt_count = 0;

    play_simple_monster_sound(&youmonst, MONSTER_SOUND_TYPE_START_ATTACK);

    for (i = 0; i < NATTK; i++) 
    {
        /* sum[i] = 0; -- now done above */
        mattk = getmattk(&youmonst, mon, i, sum, &alt_attk);

        if ((mattk->aatyp == AT_BITE || mattk->aatyp == AT_BUTT) && !(mattk->aflags & ATTKFLAGS_SAME_HEAD))
            bite_butt_count++;
        if (youmonst.data->heads > 1 && youmonst.heads_left < bite_butt_count)
            continue;


        weapon = 0;
        switch (mattk->aatyp) {
        case AT_WEAP:
            /* if (!uwep) goto weaponless; */
 use_weapon:
            odd_claw = !odd_claw; /* see case AT_CLAW,AT_TUCH below */
            /* if we've already hit with a two-handed weapon, we don't
               get to make another weapon attack (note:  monsters who
               use weapons do not have this restriction, but they also
               never have the opportunity to use two weapons) */
            if (weapon_used && sum[i - 1] && uwep && bimanual(uwep))
                continue;
            /* Certain monsters don't use weapons when encountered as enemies,
             * but players who polymorph into them have hands or claws and
             * thus should be able to use weapons.  This shouldn't prohibit
             * the use of most special abilities, either.
             * If monster has multiple claw attacks, only one can use weapon.
             */

            if(weapon_used)
                weapon2_used = TRUE;

            weapon_used = TRUE;
            /* Potential problem: if the monster gets multiple weapon attacks,
             * we currently allow the player to get each of these as a weapon
             * attack.  Is this really desirable?
             */
            /* approximate two-weapon mode; known_hitum() -> hmon() -> &c
               might destroy the weapon argument, but it might also already
               be Null, and we want to track that for passive() */
            originalweapon = (altwep && uarms && u.twoweap) ? &uarms : &uwep;
            if (u.twoweap && uarms /* set up 'altwep' flag for next iteration */
                /* only consider seconary when wielding one-handed primary */
                && !bimanual(uwep)
                && !(obj_counts_as_silver(uarms) && Hate_silver))
                altwep = !altwep; /* toggle for next attack */
            weapon = *originalweapon;
            if (!weapon) /* no need to go beyond no-gloves to rings; not ...*/
                originalweapon = &uarmg; /*... subject to erosion damage */

            struct multishot_result msres = get_multishot_stats(&youmonst, weapon, weapon, FALSE);
            int wieldermultistrike = msres.wielder_attacks;
            int multistrike = msres.weapon_attacks;
            int wielderstrikeindex;
            int strikeindex;
            for (wielderstrikeindex = 0; wielderstrikeindex < wieldermultistrike; wielderstrikeindex++)
            {
                boolean breakloop = FALSE;
                if (wielderstrikeindex > 0 && weapon)
                    You("strike with %s %s!", yname(weapon), strikeindex == 1 ? "a second time" : strikeindex == 2 ? "a third time" : "once more");

                for (strikeindex = 0; strikeindex < multistrike; strikeindex++)
                {
                    update_u_action(mattk->action_tile ? mattk->action_tile : ACTION_TILE_ATTACK);
                    play_monster_simple_weapon_sound(&youmonst, i, weapon, OBJECT_SOUND_TYPE_SWING_MELEE);
                    u_wait_until_action();

                    if (strikeindex > 0 && weapon)
                        pline("%s %s!", Yobjnam2(weapon, "strike"), strikeindex == 1 ? "a second time" : strikeindex == 2 ? "a third time" : "once more");

                    tmp = find_roll_to_hit(mon, AT_WEAP, weapon, &attknum, &armorpenalty);
                    dieroll = rnd(20);
                    dhit = (tmp > dieroll || u.uswallow);
                    /* caller must set bhitpos */
                    //DAMAGE IS DONE HERE FOR WEAPON
                    if (!known_hitum(mon, weapon, &dhit, tmp,
                        armorpenalty, mattk, dieroll)) {
                        /* enemy dead, before any special abilities used */
                        sum[i] = 2;
                        breakloop = TRUE;
                        break; // This used to be switch break, but works still by getting out of for loop
                    }
                    else
                        sum[i] = dhit;
                    /* originalweapon points to an equipment slot which might
                       now be empty if the weapon was destroyed during the hit;
                       passive(,weapon,...) won't call passive_obj() in that case */
                    weapon = *originalweapon; /* might receive passive erosion */
                    /* might be a worm that gets cut in half; if so, early return */
                    if (m_at(u.ux + u.dx, u.uy + u.dy) != mon) {
                        i = NATTK; /* skip additional attacks */
                        /* proceed with uswapwep->cursed check, then exit loop */
                        goto passivedone;
                    }
                    /* Do not print "You hit" message; known_hitum already did it. */
                    if (dhit && mattk->adtyp != AD_SPEL && mattk->adtyp != AD_PHYS)
                        sum[i] = damageum(mon, mattk, weapon, 0); //SPECIAL EFFECTS ARE DONE HERE FOR SPECIALS AFTER HITUM

                    update_u_action_revert(ACTION_TILE_NO_ACTION);
                }
                if (breakloop)
                    break;
            }
            break;
        case AT_CLAW:
            if (uwep && !cantwield(youmonst.data) && !weapon_used)
                goto use_weapon;
            if (uarms && u.twoweap && !cantwield(youmonst.data) && weapon_used && !weapon2_used)
            {
                weapon2_used = TRUE;
                goto use_weapon;
            }
            /*FALLTHRU*/
        case AT_TUCH:
            if (uwep && youmonst.data->mlet == S_LICH && !weapon_used)
                goto use_weapon;
            if (uarms && u.twoweap && youmonst.data->mlet == S_LICH && weapon_used && !weapon2_used)
            {
                weapon2_used = TRUE;
                goto use_weapon;
            }
            /*FALLTHRU*/
        case AT_SMMN:
            update_u_action(mattk->action_tile ? mattk->action_tile : ACTION_TILE_CAST_NODIR);
            play_monster_simple_weapon_sound(&youmonst, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();
            sum[i] = damageum(mon, mattk, (struct obj*)0, 0); //SPECIAL EFFECTS ARE DONE HERE FOR SPECIALS AFTER HITUM
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            break;

        case AT_KICK:
        case AT_BITE:
        case AT_RAMS:
        case AT_STNG:
        case AT_TUSK:
        case AT_HORN:
        case AT_BUTT:
        case AT_TAIL:
        case AT_TENT:
        /*weaponless:*/
            update_u_action(mattk->action_tile ? mattk->action_tile : mattk->aatyp == AT_KICK ? ACTION_TILE_KICK : ACTION_TILE_ATTACK);
            play_monster_simple_weapon_sound(&youmonst, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();
            tmp = find_roll_to_hit(mon, mattk->aatyp, (struct obj *) 0,
                                   &attknum, &armorpenalty);
            dieroll = rnd(20);
            dhit = (tmp > dieroll || u.uswallow);
            if (dhit) {
                int compat, specialdmg;
                int64_t silverhit = 0L;
                const char *verb = 0; /* verb or body part */

                if (!u.uswallow
                    && (compat = could_seduce(&youmonst, mon, mattk)) != 0) {
                    You("%s %s %s.",
                        (!is_blinded(mon) && haseyes(mon->data)) ? "smile at"
                                                             : "talk to",
                        mon_nam(mon),
                        (compat == 2) ? "engagingly" : "seductively");
                    /* doesn't anger it; no wakeup() */
                    sum[i] = damageum(mon, mattk, (struct obj*)0, 0); //SPECIAL EFFECTS ARE DONE HERE FOR SPECIALS WITHOUT HITUM
                    break;
                }
                wakeup(mon, TRUE);

                specialdmg = 0; /* blessed and/or silver bonus */
                switch (mattk->aatyp) {
                case AT_CLAW:
                case AT_TUCH:
                    /* verb=="claws" may be overridden below */
                    verb = (mattk->aatyp == AT_TUCH) ? "touch" : "claws";
                    /* decide if silver-hater will be hit by silver ring(s);
                       for 'multi_claw' where attacks alternate right/left,
                       assume 'even' claw or touch attacks use right hand
                       or paw, 'odd' ones use left for ring interaction;
                       even vs odd is based on actual attacks rather
                       than on index into mon->dat->mattk[] so that {bite,
                       claw,claw} instead of {claw,claw,bite} doesn't
                       make poly'd hero mysteriously become left-handed */
                    odd_claw = !odd_claw;
                    specialdmg = special_dmgval(&youmonst, mon,
                                                W_ARMG
                                                | ((odd_claw || !multi_claw)
                                                   ? W_RINGL : 0L)
                                                | ((!odd_claw || !multi_claw)
                                                   ? W_RINGR : 0L),
                                                &silverhit);
                    break;
                case AT_TENT:
                    /* assumes tentacled one's tentacles-on-head rather
                       than sea monster's tentacle-as-arm */
                    verb = "tentacles";
                    break;
                case AT_KICK:
                    verb = "kick";
                    specialdmg = special_dmgval(&youmonst, mon, W_ARMF,
                                                &silverhit);
                    break;
                case AT_BUTT:
                    verb = "head butt"; /* mbodypart(mon,HEAD)=="head" */
                    /* hypothetical; if any form with a head-butt attack
                       could wear a helmet, it would hit shades when
                       wearing a blessed (or silver) one */
                    specialdmg = special_dmgval(&youmonst, mon, W_ARMH,
                                                &silverhit);
                    break;
                case AT_TUSK:
                case AT_HORN:
                    verb = "gore";
                    specialdmg = special_dmgval(&youmonst, mon, W_ARMH,
                        &silverhit);
                    break;
                case AT_BITE:
                    verb = "bite";
                    break;
                case AT_RAMS:
                    verb = "ram";
                    break;
                case AT_TAIL:
                    verb = "tail-lash";
                    break;
                case AT_STNG:
                    verb = "sting";
                    break;
                default:
                    verb = "hit";
                    break;
                }
                if (is_shade(mon->data) && !specialdmg) {
                    if (!strcmp(verb, "hit")
                        || (mattk->aatyp == AT_CLAW && humanoid(mon->data)))
                        verb = "attack";
                    Your("%s %s harmlessly through %s.",
                         verb, vtense(verb, "pass"), mon_nam(mon));
                } else {
                    if (mattk->aatyp == AT_TENT) {
                        Your("tentacles suck %s.", mon_nam(mon));
                    } else {
                        if (mattk->aatyp == AT_CLAW)
                            verb = "hit"; /* not "claws" */
                        You("%s %s.", verb, mon_nam(mon));
                        if (silverhit && flags.verbose)
                            silver_sears(&youmonst, mon, silverhit);
                    }
                    //SPECIAL EFFECTS ARE DONE HERE FOR SPECIALS WITHOUT HITUM (AND BELOW MORE FOR HUGS)
                    sum[i] = damageum(mon, mattk, (struct obj*)0, specialdmg);
                }
            } else { /* !dhit */
                missum(mon, mattk, (tmp + armorpenalty > dieroll));
            }
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            break;

        case AT_HUGS: 
        {
            update_u_action(mattk->action_tile ? mattk->action_tile : ACTION_TILE_ATTACK);
            play_monster_simple_weapon_sound(&youmonst, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();
            int specialdmg;
            int64_t silverhit = 0L;
            boolean byhand = hug_throttles(&mons[u.umonnum]), /* rope golem */
                    unconcerned = (byhand && !can_be_strangled(mon));

            if (sticks(mon->data) || u.uswallow || notonhead
                || (byhand && (uwep || !has_neck(mon->data))))
            {
                /* can't hold a holder due to subsequent ambiguity over
                   who is holding whom; can't hug engulfer from inside;
                   can't hug a worm tail (would immobilize entire worm!);
                   byhand: can't choke something that lacks a head;
                   not allowed to make a choking hug if wielding a weapon
                   (but might have grabbed w/o weapon, then wielded one,
                   and may even be attacking a different monster now) */
                if (byhand && uwep && u.ustuck
                    && !(sticks(u.ustuck->data) || u.uswallow))
                    uunstick();
                
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                continue; /* not 'break'; bypass passive counter-attack */
            }
            /* automatic if prev two attacks succeed, or if
               already grabbed in a previous attack */
            dhit = 1;
            wakeup(mon, TRUE);
            /* choking hug/throttling grab uses hands (gloves or rings);
               normal hug uses outermost of cloak/suit/shirt */
            specialdmg = special_dmgval(&youmonst, mon,
                                        byhand ? (W_ARMG | W_RINGL | W_RINGR)
                                               : (W_ARMC | W_ARM | W_ARMU),
                                        &silverhit);
            if (unconcerned)
            {
                /* strangling something which can't be strangled */
                if (mattk != &alt_attk) 
                {
                    alt_attk = *mattk;
                    mattk = &alt_attk;
                }
                /* change damage to 1d1; not strangling but still
                   doing [minimal] physical damage to victim's body */
                mattk->damn = mattk->damd = 1;
                /* don't give 'unconcerned' feedback if there is extra damage
                   or if it is nearly destroyed or if creature doesn't have
                   the mental ability to be concerned in the first place */
                if (specialdmg || mindless(mon->data)
                    || mon->mhp <= 1 + max(u.ubasedaminc + u.udaminc, 1))
                    unconcerned = FALSE;
            }
            if (is_shade(mon->data))
            {
                const char *verb = byhand ? "grasp" : "hug";

                /* hugging a shade; successful if blessed outermost armor
                   for normal hug, or blessed gloves or silver ring(s) for
                   choking hug; deals damage but never grabs hold */
                if (specialdmg)
                {
                    You("%s %s%s%s", verb, mon_nam(mon), byhand ? " by the throat" : "", exclam(specialdmg));
                    
                    if (silverhit && flags.verbose)
                        silver_sears(&youmonst, mon, silverhit);
                    sum[i] = damageum(mon, mattk, (struct obj*)0, specialdmg);
                } else {
                    Your("%s passes harmlessly through %s.",
                         verb, mon_nam(mon));
                }
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                break;
            }
            /* hug attack against ordinary foe */
            if (mon == u.ustuck)
            {
                pline("%s is being %s%s.", Monnam(mon),
                      byhand ? "throttled" : "crushed",
                      /* extra feedback for non-breather being choked */
                      unconcerned ? " but doesn't seem concerned" : "");
                if (silverhit && flags.verbose)
                    silver_sears(&youmonst, mon, silverhit);
                sum[i] = damageum(mon, mattk, (struct obj*)0, specialdmg);
            }
            else if (i >= 2 && sum[i - 1] && sum[i - 2]) 
            {
                /* in case we're hugging a new target while already
                   holding something else; yields feedback
                   "<u.ustuck> is no longer in your clutches" */
                if (u.ustuck && u.ustuck != mon)
                    uunstick();
                You("grab %s!", mon_nam(mon));
                play_sfx_sound(SFX_ACQUIRE_GRAB);
                u.ustuck = mon;
                if (silverhit && flags.verbose)
                    silver_sears(&youmonst, mon, silverhit);
                sum[i] = damageum(mon, mattk, (struct obj*)0, specialdmg);
            }
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            break; /* AT_HUGS */
        }

        case AT_EXPL: /* automatic hit if next to */
            update_u_action(mattk->action_tile ? mattk->action_tile : ACTION_TILE_ATTACK);
            play_monster_simple_weapon_sound(&youmonst, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();
            dhit = -1;
            wakeup(mon, TRUE);
            sum[i] = explum(mon, mattk);
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            break;

        case AT_ENGL:
            update_u_action(mattk->action_tile ? mattk->action_tile : ACTION_TILE_ATTACK);
            play_monster_simple_weapon_sound(&youmonst, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();
            tmp = find_roll_to_hit(mon, mattk->aatyp, (struct obj *) 0, &attknum, &armorpenalty);

            if ((dhit = (tmp > rnd(20 + i))))
            {
                wakeup(mon, TRUE);
                if (is_shade(mon->data))
                    Your("attempt to surround %s is harmless.", mon_nam(mon));
                else {
                    sum[i] = gulpum(mon, mattk);
                    if (sum[i] == 2 && (mon->data->mlet == S_GREATER_UNDEAD
                                        || mon->data->mlet == S_LESSER_UNDEAD)
                        && rn2(5) && !Sick_resistance && !Invulnerable) {
                        You_feel("%ssick.", (Sick) ? "very " : "");
                        mdamageu(mon, adjust_damage(rnd(8), mon, &youmonst, mattk->adtyp, ADFLAGS_NONE), TRUE);
                    }
                }
            } else {
                missum(mon, mattk, FALSE);
            }
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            break;

        case AT_MAGC:
            /* No check for uwep; if wielding nothing we want to
             * do the normal 1-2 points bare hand damage...
             */
            if ((youmonst.data->mlet == S_KOBOLD
                 || youmonst.data->mlet == S_ORC
                 || youmonst.data->mlet == S_GNOLL) && !weapon_used)
                goto use_weapon;
            /*FALLTHRU*/

        case AT_NONE:
        case AT_PASV:
        case AT_BOOM:
            continue;
        /* Not break--avoid passive attacks from enemy */

        case AT_EYES:
        case AT_BREA:
        case AT_SPIT:
        case AT_GAZE: /* all done using #monster command */
            dhit = 0;
            break;

        default: /* Strange... */
            impossible("strange attack of yours (%d)", mattk->aatyp);
        }
        if (dhit == -1) {
            u.mh = -1; /* dead in the current form */
            rehumanize();
        }
        if (sum[i] == 2) {
            /* defender dead */
            (void) passive(mon, weapon, 1, 0, mattk->aatyp, FALSE);
            //nsum = 0; /* return value below used to be 'nsum > 0' */
        } else {
            (void) passive(mon, weapon, sum[i], 1, mattk->aatyp, FALSE);
            //nsum |= sum[i];
        }

        /* don't use sum[i] beyond this point;
           'i' will be out of bounds if we get here via 'goto' */
 passivedone:
        /* stop attacking if defender has died;
           needed to defer this until after uswapwep->cursed check */

        if (DEADMONSTER(mon))
            break;
        if (!Upolyd)
            break; /* No extra attacks if no longer a monster */
        if (multi < 0 || Sleeping || Paralyzed_or_immobile)
            break; /* If paralyzed while attacking, i.e. floating eye */
    }
    
    /* return value isn't used, but make it match hitum()'s */
    return !DEADMONSTER(mon);
}

/*      Special (passive) attacks on you by monsters done here.
 */
int
passive(mon, weapon, mhit, malive, aatyp, wep_was_destroyed)
struct monst *mon;
struct obj *weapon; /* uwep or uarms or uarmg or uarmf or Null */
boolean mhit;
int malive;
uchar aatyp;
boolean wep_was_destroyed;
{
    register struct permonst *ptr = mon->data;
    register int i, basedmg = 0;
    double damage = 0;
    enum hit_tile_types hit_tile = HIT_GENERAL;

    for (i = 0;; i++) 
    {
        if (i >= NATTK)
            return (malive | mhit); /* no passive attacks */
        if (ptr->mattk[i].aatyp == AT_PASV)
            break; /* try this one */
    }

    /* Note: tmp not always used */
    if (ptr->mattk[i].damn > 0 || ptr->mattk[i].damd > 0)
        basedmg = max(0, d(ptr->mattk[i].damn > 0 ? ptr->mattk[i].damn : (int) mon->m_lev / 2 + 2, ptr->mattk[i].damd > 0 ? (int) ptr->mattk[i].damd : 6) + (int)ptr->mattk[i].damp);
    else
        basedmg = max(0, ptr->mattk[i].damp);

    damage += adjust_damage(basedmg, mon, &youmonst, ptr->mattk[i].adtyp, ADFLAGS_NONE);

    enum action_tile_types action_before = mon->action;
    if (malive)
    {
        update_m_action(mon, ptr->mattk[i].action_tile ? ptr->mattk[i].action_tile : ACTION_TILE_PASSIVE_DEFENSE);
        play_monster_simple_weapon_sound(mon, i, (struct obj*)0, OBJECT_SOUND_TYPE_SWING_MELEE);
        m_wait_until_action(mon, ptr->mattk[i].action_tile ? ptr->mattk[i].action_tile : ACTION_TILE_PASSIVE_DEFENSE);
    }

    /*  These affect you even if they just died.
     */
    switch (ptr->mattk[i].adtyp)
    {
    case AD_FIRE:
        if(!is_cancelled(mon))
            hit_tile = HIT_ON_FIRE;
        if (mhit && !is_cancelled(mon) && weapon)
        {
            if (aatyp == AT_KICK)
            {
                if (uarmf && !rn2(6))
                    (void) erode_obj(uarmf, xname(uarmf), ERODE_BURN,
                                     EF_GREASE | EF_VERBOSE);
            } else if (aatyp == AT_WEAP || aatyp == AT_CLAW
                       || aatyp == AT_MAGC || aatyp == AT_TUCH)
                passive_obj(mon, weapon, &(ptr->mattk[i]));
        }
        break;
    case AD_ACID:
        hit_tile = HIT_SPLASHED_ACID;
        if (mhit && rn2(2)) 
        {
            play_sfx_sound(SFX_MONSTER_GETS_SPLASHED_BY_ACID);
            if (Blind || !flags.verbose)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are splashed!");
            else
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are splashed by %s %s!", s_suffix(mon_nam(mon)),
                    hliquid("acid"));

            if (!Acid_immunity && !Invulnerable)
                mdamageu_with_hit_tile(mon, damage, TRUE, HIT_SPLASHED_ACID);

            if (!rn2(30))
                erode_armor(&youmonst, ERODE_CORRODE);
        }
        if (mhit && weapon) {
            if (aatyp == AT_KICK) {
                if (uarmf && !rn2(6))
                    (void) erode_obj(uarmf, xname(uarmf), ERODE_CORRODE,
                                     EF_GREASE | EF_VERBOSE);
            } else if (aatyp == AT_WEAP || aatyp == AT_CLAW
                       || aatyp == AT_MAGC || aatyp == AT_TUCH)
                passive_obj(mon, weapon, &(ptr->mattk[i]));
        }
        exercise(A_STR, FALSE);
        break;
    case AD_STON:
        if (mhit) { /* successful attack */
            int64_t protector = attk_protection((int) aatyp);

            /* hero using monsters' AT_MAGC attack is hitting hand to
               hand rather than casting a spell */
            if (aatyp == AT_MAGC)
                protector = W_ARMG;

            if (protector == 0L /* no protection */
                || (protector == W_ARMG && !uarmg
                    && !weapon && !wep_was_destroyed)
                || (protector == W_ARMF && !uarmf)
                || (protector == W_ARMH && !uarmh)
                || (protector == (W_ARMC | W_ARMG) && (!uarmc || !uarmg))) {
                if (!Stone_resistance
                    && !(poly_when_stoned(youmonst.data)
                         && polymon(PM_STONE_GOLEM))) {
                    display_u_being_hit(HIT_PETRIFIED, 0, 0UL);
                    done_in_by(mon, STONING); /* "You turn to stone..." */
                    if(malive)
                        update_m_action_core(mon, action_before, 0, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
                    return 2;
                }
            }
        }
        break;
    case AD_RUST:
        if (mhit && !is_cancelled(mon) && weapon) {
            if (aatyp == AT_KICK) {
                if (uarmf)
                    (void) erode_obj(uarmf, xname(uarmf), ERODE_RUST,
                                     EF_GREASE | EF_VERBOSE);
            } else if (aatyp == AT_WEAP || aatyp == AT_CLAW
                       || aatyp == AT_MAGC || aatyp == AT_TUCH)
                passive_obj(mon, weapon, &(ptr->mattk[i]));
        }
        break;
    case AD_CORR:
        if (mhit && !is_cancelled(mon) && weapon) {
            if (aatyp == AT_KICK) {
                if (uarmf)
                    (void) erode_obj(uarmf, xname(uarmf), ERODE_CORRODE,
                                     EF_GREASE | EF_VERBOSE);
            } else if (aatyp == AT_WEAP || aatyp == AT_CLAW
                       || aatyp == AT_MAGC || aatyp == AT_TUCH)
                passive_obj(mon, weapon, &(ptr->mattk[i]));
        }
        break;
    case AD_MAGM:
        /* wrath of gods for attacking Oracle */
        if (Magic_missile_immunity || Invulnerable) {
            play_sfx_sound(SFX_HAIL_OF_MAGIC_MISSILES_MISSES);
            u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "A hail of magic missiles narrowly misses you!");
        } else {
            play_sfx_sound(SFX_HAIL_OF_MAGIC_MISSILES_HITS);
            You_ex(ATR_NONE, CLR_MSG_SPELL, "are hit by magic missiles appearing from thin air!");
            mdamageu(mon, damage, TRUE);
        }
        break;
    case AD_ENCH: /* KMH -- remove enchantment (disenchanter) */
        if (mhit) {
            if (aatyp == AT_KICK) {
                if (!weapon)
                    break;
            } else if (aatyp == AT_BITE || aatyp == AT_RAMS || aatyp == AT_BUTT || aatyp == AT_HORN || aatyp == AT_TUSK || aatyp == AT_TAIL
                       || (aatyp >= AT_STNG && aatyp < AT_WEAP)) {
                break; /* no object involved */
            }
            passive_obj(mon, weapon, &(ptr->mattk[i]));
        }
        break;
    default:
        break;
    }

    /*  These only affect you if they still live.
     */
    if (malive && !is_cancelled(mon) && rn2(3)) {
        switch (ptr->mattk[i].adtyp) {
        case AD_PLYS:
            if (ptr == &mons[PM_FLOATING_EYE]) {
                if (!canseemon(mon)) {
                    break;
                }
                if (!is_blinded(mon)) {
                    play_sfx_sound(SFX_GENERAL_REFLECTS);
                    if (ureflects("%s gaze is reflected by your %s.",
                                  s_suffix(Monnam(mon)))) {
                        ;
                    } else if (Free_action) {
                        play_sfx_sound(SFX_GENERAL_RESISTS);
                        You_ex(ATR_NONE, CLR_MSG_WARNING, "momentarily stiffen under %s gaze!",
                            s_suffix(mon_nam(mon)));
                    } else if (Hallucination && rn2(4)) {
                        pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "%s looks %s%s.", Monnam(mon),
                              !rn2(2) ? "" : "rather ",
                              !rn2(2) ? "numb" : "stupified");
                    } else {
                        play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are frozen by %s gaze!", s_suffix(mon_nam(mon)));
                        incr_itimeout(&HParalyzed, (ACURR(A_WIS) > 12 || rn2(4)) ? basedmg : 127);
                        context.botl = context.botlx = 1;
                        refresh_u_tile_gui_info(TRUE);
                        standard_hint("Do not hit floating eyes in melee unless you wear a blindfold or a towel. Use ranged weapons against them.", &u.uhint.paralyzed_by_floating_eye);
                    }
                } else {
                    pline("%s cannot defend itself.",
                          Adjmonnam(mon, "blind"));
                    if (!rn2(500))
                        change_luck(-1, TRUE);
                }
            } else if (Free_action) {
                play_sfx_sound(SFX_GENERAL_RESISTS);
                You_ex(ATR_NONE, CLR_MSG_WARNING, "momentarily stiffen.");
            } else { /* gelatinous cube */
                play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are frozen by %s!", mon_nam(mon));
                incr_itimeout(&HParalyzed, basedmg);
                context.botl = context.botlx = 1;
                refresh_u_tile_gui_info(TRUE);
                standard_hint("Get paralysis resistance as early as possible. Use ranged weapons to attack monsters with paralyzing passive defense.", &u.uhint.paralyzed_by_cube);
            }
            break;
        case AD_COLD: /* brown mold or blue jelly */
            if (monnear(mon, u.ux, u.uy)) {
                if (Cold_immunity || Invulnerable) {
                    u_shieldeff();
                    You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "a mild chill.");
                    ugolemeffects(AD_COLD, damage);
                    break;
                }
                hit_tile = HIT_FROZEN;
                play_sfx_sound(SFX_MONSTER_COVERED_IN_FROST);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are suddenly very cold!");
                mdamageu_with_hit_tile(mon, damage, TRUE, hit_tile);
                /* monster gets stronger with your heat! */
                mon->mhp += (int)ceil(damage) / 2;
                if (mon->mhpmax < mon->mhp)
                    mon->mbasehpmax += mon->mhp - mon->mhpmax;
                update_mon_maxhp(mon);

                /* at a certain point, the monster will reproduce! */
                if (mon->mhpmax > ((int) (mon->m_lev + 1) * 8))
                    (void) split_mon(mon, &youmonst);
            }
            break;
        case AD_STUN: /* specifically yellow mold */
            if (!Stunned && !Stun_resistance)
            {
                play_sfx_sound(SFX_ACQUIRE_STUN);
                make_stunned((int64_t)basedmg, TRUE);
            }
            break;
        case AD_FIRE:
            if (monnear(mon, u.ux, u.uy))
            {
                if (Fire_immunity || Invulnerable) 
                {
                    u_shieldeff();
                    if (flaming(mon->data))
                        You_ex(ATR_NONE, CLR_MSG_WARNING, "are engulfed in %s flames, but they do not burn you!", s_suffix(mon_nam(mon)));
                    else
                        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "mildly warm.");
                    ugolemeffects(AD_FIRE, damage);
                    break;
                }
                play_sfx_sound(SFX_MONSTER_ON_FIRE);
                if (flaming(mon->data))
                {
                    hit_tile = HIT_ON_FIRE;
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are engulfed in %s flames!", s_suffix(mon_nam(mon)));
                }
                else
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are suddenly very hot!");
                mdamageu_with_hit_tile(mon, damage, TRUE, hit_tile); /* fire damage */
                standard_hint("Acquire full fire resistance or use ranged weapons to bypass passive fire defense.", &u.uhint.damaged_by_passive_fire);
            }
            break;
        case AD_ELEC:
            if (Shock_immunity || Invulnerable) 
            {
                u_shieldeff();
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "a mild tingle.");
                ugolemeffects(AD_ELEC, damage);
                break;
            }
            hit_tile = HIT_ELECTROCUTED;
            play_sfx_sound(SFX_MONSTER_GETS_ZAPPED);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are jolted with electricity!");
            mdamageu_with_hit_tile(mon, damage, TRUE, hit_tile);
            standard_hint("Acquire full shock resistance or use ranged weapons to bypass passive electricity defense.", &u.uhint.damaged_by_passive_electricity);
            break;
        default:
            break;
        }
    }
    if(malive)
        update_m_action_core(mon, action_before, 1, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    return (malive | mhit);
}

/*
 * Special (passive) attacks on an attacking object by monsters done here.
 * Assumes the attack was successful.
 */
void
passive_obj(mon, obj, mattk)
struct monst *mon;
struct obj *obj;          /* null means pick uwep, uswapwep or uarmg */
struct attack *mattk;     /* null means we find one internally */
{
    struct permonst *ptr = mon->data;
    int i;

    /* [this first bit is obsolete; we're not called with Null anymore] */
    /* if caller hasn't specified an object, use uwep, uswapwep or uarmg */
    if (!obj) {
        obj = (u.twoweap && uarms && !rn2(2)) ? uarms : uwep;
        if (!obj && mattk->adtyp == AD_ENCH)
            obj = uarmg; /* no weapon? then must be gloves */
        if (!obj)
            return; /* no object to affect */
    }

    /* if caller hasn't specified an attack, find one */
    if (!mattk) {
        for (i = 0;; i++) {
            if (i >= NATTK)
                return; /* no passive attacks */
            if (ptr->mattk[i].aatyp == AT_PASV)
                break; /* try this one */
        }
        mattk = &(ptr->mattk[i]);
    }

    switch (mattk->adtyp) {
    case AD_FIRE:
        if (!rn2(6) && !is_cancelled(mon)
            /* steam vortex: fire resist applies, fire damage doesn't */
            && mon->data != &mons[PM_STEAM_VORTEX]) {
            (void) erode_obj(obj, NULL, ERODE_BURN, EF_NONE);
        }
        break;
    case AD_ACID:
        if (!rn2(6)) {
            (void) erode_obj(obj, NULL, ERODE_CORRODE, EF_GREASE);
        }
        break;
    case AD_RUST:
        if (!is_cancelled(mon)) {
            (void) erode_obj(obj, (char *) 0, ERODE_RUST, EF_GREASE);
        }
        break;
    case AD_CORR:
        if (!is_cancelled(mon)) {
            (void) erode_obj(obj, (char *) 0, ERODE_CORRODE, EF_GREASE);
        }
        break;
    case AD_ENCH:
        if (!is_cancelled(mon)) {
            if (drain_item(obj, TRUE) && carried(obj)
                && (obj->known || obj->oclass == ARMOR_CLASS)) {
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s less effective.", Yobjnam2(obj, "seem"));
            }
            break;
        }
    default:
        break;
    }

    if (carried(obj))
        update_inventory();
}

/* Note: caller must ascertain mtmp is mimicking... */
void
stumble_onto_mimic(mtmp)
struct monst *mtmp;
{
    const char *fmt = "Wait!  That's %s!", *generic = "a monster", *what = 0;

    if (Blind) 
    {
        if (!(Blind_telepat || Unblind_telepat || Detect_monsters))
            what = generic; /* with default fmt */
        else if (M_AP_TYPE(mtmp) == M_AP_MONSTER)
            what = a_monnam(mtmp); /* differs from what was sensed */
    } 
    else 
    {
        int glyph = levl[u.ux + u.dx][u.uy + u.dy].hero_memory_layers.glyph;

        if (glyph_is_cmap_or_cmap_variation(glyph) && (generic_glyph_to_cmap(glyph) == S_hcdoor
                                     || generic_glyph_to_cmap(glyph) == S_vcdoor))
            fmt = "The door actually was %s!";
        else if (glyph_is_object(glyph) && glyph_to_otyp(glyph) == GOLD_PIECE)
            fmt = "That gold was %s!";

        /* cloned Wiz starts out mimicking some other monster and
           might make himself invisible before being revealed */
        if (is_invisible(mtmp) && !See_invisible)
            what = generic;
        else
            what = a_monnam(mtmp);
    }

    if (what)
    { 
        update_m_facing(mtmp, u.ux - mtmp->mx, TRUE);
        play_sfx_sound(SFX_STUMBLE_ON_MIMIC);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, fmt, what);
    }

    /* Make mimic grab you */
    if (!u.ustuck && !is_fleeing(mtmp) && check_stuck_and_slip(mtmp))
    {
        play_sfx_sound(SFX_ACQUIRE_GRAB);
        u.ustuck = mtmp;
        refresh_m_tile_gui_info(mtmp, FALSE);
    }

    wakeup(mtmp, FALSE); /* clears mimicking */
    /* if hero is blind, wakeup() won't display the monster even though
       it's no longer concealed */
    if (!canspotmon(mtmp)
        && !glyph_is_invisible(levl[mtmp->mx][mtmp->my].hero_memory_layers.glyph))
        map_invisible(mtmp->mx, mtmp->my);

    flush_screen(1);
}

STATIC_OVL void
nohandglow(mon)
struct monst *mon;
{
    char *hands = makeplural(body_part(HAND));

    if (!u.umconf || is_confused(mon))
        return;
    if (u.umconf == 1) {
        if (Blind)
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stop tingling.", hands);
        else
            Your_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s stop glowing %s.", hands, hcolor_multi_buf1(NH_RED));
    } else {
        if (Blind)
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "tingling in your %s lessens.", hands);
        else
            Your_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s no longer glow so brightly %s.", hands, hcolor_multi_buf1(NH_RED));
    }
    u.umconf--;
}

int
flash_hits_mon(mtmp, otmp)
struct monst *mtmp;
struct obj *otmp; /* source of flash */
{
    if (!mtmp)
        return 0;

    int tmp, amt, res = 0, useeit = canseemon(mtmp);

    if (mtmp->msleeping) {
        mtmp->msleeping = 0;
        if (useeit) {
            refresh_m_tile_gui_info(mtmp, TRUE);
            pline_The("flash awakens %s.", mon_nam(mtmp));
            res = 1;
        }
    } else if (mtmp->data->mlet != S_LIGHT) {
        if (!resists_blnd(mtmp)) {
            tmp = dist2(otmp->ox, otmp->oy, mtmp->mx, mtmp->my);
            if (useeit) {
                if(tmp < 3 && !is_blinded(mtmp))
                    pline("%s is blinded by the flash!", Monnam(mtmp));
                else
                    pline("%s is illuminated in the light of the flash!", Monnam(mtmp));
                res = 1;
            }
            if (mtmp->data == &mons[PM_GREMLIN]) {
                /* Rule #1: Keep them out of the light. */
                amt = otmp->otyp == WAN_LIGHT ? d(1 + otmp->charges, 4)
                                              : rn2(min(mtmp->mhp, 4));
                light_hits_gremlin(mtmp, amt);
            }
            if (!DEADMONSTER(mtmp)) {
                if (!context.mon_moving)
                    setmangry(mtmp, TRUE);
                if (tmp < 9 && !mtmp->isshk && rn2(4))
                {
                    play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mtmp->mx, mtmp->my);
                    monflee(mtmp, rn2(4) ? rnd(100) : 0, FALSE, TRUE);
                }

                if (tmp < 3)
                    increase_mon_property(mtmp, BLINDED, rnd(1 + 50 / tmp));
            }
        }
    }
    return res;
}

void
light_hits_gremlin(mon, dmg)
struct monst *mon;
int dmg;
{
    pline("%s %s!", Monnam(mon),
          (dmg > mon->mhp / 2) ? "wails in agony" : "cries out in pain");
    deduct_monster_hp(mon, adjust_damage(dmg, (struct monst*)0, mon, AD_BLND, ADFLAGS_NONE));
    wake_nearto(mon->mx, mon->my, 30);
    remove_monster_and_nearby_waitforu(mon);
    if (DEADMONSTER(mon)) {
        if (context.mon_moving)
            monkilled(mon, (char *) 0, AD_BLND, 0);
        else
            killed(mon);
    } else if (cansee(mon->mx, mon->my) && !canspotmon(mon)) {
        map_invisible(mon->mx, mon->my);
    }
}

double
adjust_damage(basedamage, magr, mdef, adtyp, ad_flags)
int basedamage;
struct monst* magr;
struct monst* mdef;
short adtyp;
uint64_t ad_flags;
{
    double base_dmg_d = (double)basedamage;
    boolean you_attack = (magr && magr == &youmonst);
    boolean you_defend = (mdef && mdef == &youmonst);
    double monster_damage_multiplier = 1;
    double monster_hp_multiplier = 1;

    if (adtyp == AD_NONE)
        adtyp = AD_PHYS; /* Just make sure it does not bypass all checks */

    get_game_difficulty_multipliers(&monster_damage_multiplier, &monster_hp_multiplier);

    if (mdef)
    {
        if (you_defend ? Invulnerable : has_invulnerable(mdef))
        {
            return 0;
        }

        /* Physical and spell damage adjustments */
        if ((adtyp == AD_PHYS || adtyp == AD_ACID || adtyp == AD_DGST || adtyp == AD_MAGM) &&
             ((you_defend ? Half_physical_damage : has_half_physical_damage(mdef))
              || (magr && (you_defend ? Half_physical_damage_against_undead_and_demons : has_half_physical_damage_against_undead_and_demons(mdef)) && (is_undead(magr->data) || is_demon(magr->data)))
             )
           )
        {
            base_dmg_d /= 2.0;
        }

        if ((adtyp == AD_PHYS || adtyp == AD_ACID || adtyp == AD_DGST || adtyp == AD_MAGM) &&
            ((you_defend ? Double_physical_damage : has_double_physical_damage(mdef)))
            )
        {
            base_dmg_d *= 2.0;
        }

        if ((ad_flags & ADFLAGS_SPELL_DAMAGE) && (you_defend ? Half_spell_damage : has_half_spell_damage(mdef)))
        {
            base_dmg_d /= 2.0;
        }

        if ((ad_flags & ADFLAGS_SPELL_DAMAGE) && (you_defend ? Double_spell_damage : has_double_spell_damage(mdef)))
        {
            base_dmg_d *= 2.0;
        }

        /* Armor-type reductions, half specific damage type reductions, and vulneratbilities here */
        if (adtyp == AD_FIRE && (you_defend ? Fire_vulnerability : has_fire_vulnerability(mdef)))
        {
            base_dmg_d *= 2.0;
        }

        if (adtyp == AD_COLD && (you_defend ? Cold_vulnerability : has_cold_vulnerability(mdef)))
        {
            base_dmg_d *= 2.0;
        }

        if (adtyp == AD_ELEC && (you_defend ? Elec_vulnerability : has_elec_vulnerability(mdef)))
        {
            base_dmg_d *= 2.0;
        }

        if (adtyp == AD_MAGM && (you_defend ? Magic_damage_vulnerability : has_magm_vulnerability(mdef)))
        {
            base_dmg_d *= 2.0;
        }

        /* Resistances (half damage, immunity takes to zero) */
        if (adtyp == AD_FIRE)
        {
            if (you_defend ? Fire_immunity : is_mon_immune_to_fire(mdef))
                base_dmg_d = 0.0;
            else if (you_defend ? Improved_fire_resistance : mon_resists_fire_strongly(mdef))
                base_dmg_d /= 4.0;
            else if(you_defend ? Fire_resistance : mon_resists_fire_weakly(mdef))
                base_dmg_d /= 2.0;
        }

        if (adtyp == AD_COLD)
        {
            if (you_defend ? Cold_immunity : is_mon_immune_to_cold(mdef))
                base_dmg_d = 0.0;
            else if (you_defend ? Improved_cold_resistance : mon_resists_cold_strongly(mdef))
                base_dmg_d /= 4.0;
            else if(you_defend ? Cold_resistance : mon_resists_cold_weakly(mdef))
                base_dmg_d /= 2.0;
        }

        if (adtyp == AD_ELEC)
        {
            if (you_defend ? Shock_immunity : is_mon_immune_to_elec(mdef))
                base_dmg_d = 0.0;
            else if (you_defend ? Improved_shock_resistance : mon_resists_elec_strongly(mdef))
                base_dmg_d /= 4.0;
            else if(you_defend ? Shock_resistance : mon_resists_elec_weakly(mdef))
                base_dmg_d /= 2.0;
        }

        if (adtyp == AD_MAGM)
        {
            if (you_defend ? Magic_missile_immunity : is_mon_immune_to_magic_missile(mdef))
                base_dmg_d = 0.0;
            else if (you_defend ? Improved_magic_missile_resistance : mon_resists_magic_missile_strongly(mdef))
                base_dmg_d /= 4.0;
            else if (you_defend ? Magic_missile_resistance : mon_resists_magic_missile_weakly(mdef))
                base_dmg_d /= 2.0;
        }

        if (adtyp == AD_ACID)
        {
            if (you_defend ? Acid_immunity : is_mon_immune_to_acid(mdef))
                base_dmg_d = 0.0;
            else if (you_defend ? Improved_acid_resistance : mon_resists_acid_strongly(mdef))
                base_dmg_d /= 4.0;
            else if (you_defend ? Acid_resistance : mon_resists_acid_weakly(mdef))
                base_dmg_d /= 2.0;
        }

        /* Game difficulty level adjustments */
        if (you_defend || is_tame(mdef)) /* You or your pet is being hit */
        {
            if(!magr || (magr && !(you_attack || is_tame(magr)))) /* Only hostile and non-tame peaceful attacking monsters get damage bonus */
                base_dmg_d *= monster_damage_multiplier;
        }
        else /* Hostile or non-tame peaceful monster is being hit */
        {
            base_dmg_d /= monster_hp_multiplier; /* The damage gets adjusted by their implicit hp adjustment */
            if (!magr || (magr && !(you_attack || is_tame(magr)))) /* Only hostile and non-tame peaceful attacking monsters get damage bonus */
                base_dmg_d *= monster_damage_multiplier;
        }

    }

    if (magr) // && !(ad_flags & ADFLAGS_SPELL_DAMAGE)
    {
        if(you_attack)
        {
            if (Super_heroism)
            {
                base_dmg_d *= 2.0;
            }
            else if (Heroism)
            {
                base_dmg_d *= 1.5;
            }
        }
        else
        {
            if (has_super_heroism(magr))
            {
                base_dmg_d *= 2.0;
            }
            else if (has_heroism(magr))
            {
                base_dmg_d *= 1.5;

            }
        }
    }

    return base_dmg_d;
}

void
get_game_difficulty_multipliers(monster_damage_multiplier_ptr, monster_hp_multiplier_ptr)
double* monster_damage_multiplier_ptr;
double* monster_hp_multiplier_ptr;
{
    get_game_difficulty_multipliers_by_level(monster_damage_multiplier_ptr, monster_hp_multiplier_ptr, context.game_difficulty);
}

void
get_game_difficulty_multipliers_by_level(monster_damage_multiplier_ptr, monster_hp_multiplier_ptr, difficulty_level)
double *monster_damage_multiplier_ptr;
double *monster_hp_multiplier_ptr;
schar difficulty_level;
{
    if (!monster_damage_multiplier_ptr || !monster_hp_multiplier_ptr)
        return;

    *monster_damage_multiplier_ptr = 1.0;
    *monster_hp_multiplier_ptr = 1.0;

    switch (difficulty_level)
    {
    case -4:
        *monster_damage_multiplier_ptr = 0.3501;
        *monster_hp_multiplier_ptr = 0.2856;
        break;
    case -3:
        *monster_damage_multiplier_ptr = 0.4552;
        *monster_hp_multiplier_ptr = 0.3907;
        break;
    case -2:
        *monster_damage_multiplier_ptr = 0.5917;
        *monster_hp_multiplier_ptr = 0.5344;
        break;
    case -1:
        *monster_damage_multiplier_ptr = 0.7692;
        *monster_hp_multiplier_ptr = 0.7310;
        break;
    case 0:
        *monster_damage_multiplier_ptr = 1.0;
        *monster_hp_multiplier_ptr = 1.0;
        break;
    case 1:
        *monster_damage_multiplier_ptr = 1.3;
        *monster_hp_multiplier_ptr = 1.3679;
        break;
    case 2:
        *monster_damage_multiplier_ptr = 1.69;
        *monster_hp_multiplier_ptr = 1.8712;
        break;
    default:
        break;
    }
}

int
calculate_damage_dealt_to_player(hp_d)
double hp_d;
{
    int integer_hp = (int)hp_d;
    double fracionalpart_hp_d = hp_d - (double)integer_hp;
    int fractional_hp = (int)(10000 * fracionalpart_hp_d);

    int target_integer_part = 0;
    int target_fractional_part = 0;
    int target_max = 0;

    if (Upolyd)
    {
        target_integer_part = u.mh;
        target_fractional_part = u.mh_fraction;
        target_max = u.mhmax;
    }
    else
    {
        target_integer_part = u.uhp;
        target_fractional_part = u.uhp_fraction;
        target_max = u.uhpmax;
    }

    int hp_before = target_integer_part;
    target_integer_part -= integer_hp;
    target_fractional_part -= fractional_hp;

    if (target_fractional_part < 0)
    {
        int multiple = (abs(target_fractional_part) / 10000) + 1;
        target_integer_part -= multiple;
        target_fractional_part += multiple * 10000;
    }
    else if (target_fractional_part >= 10000)
    {
        int multiple = target_fractional_part / 10000;
        target_integer_part += multiple;
        target_fractional_part -= multiple * 10000;
    }

    if (target_integer_part >= target_max)
    {
        target_integer_part = target_max;
        target_fractional_part = 0;
    }

    int hp_after = target_integer_part;
    int damage_dealt = hp_before - hp_after;
    return max(0, damage_dealt);
}

int
deduct_player_hp(hp_d)
double hp_d;
{
    int integer_hp = (int)hp_d;
    double fracionalpart_hp_d = hp_d - (double)integer_hp;
    int fractional_hp = (int)(10000 * fracionalpart_hp_d);

    int* target_integer_part_ptr = (int*)0;
    int* target_fractional_part_ptr = (int*)0;
    int* target_max_ptr = (int*)0;

    if (Upolyd)
    {
        target_integer_part_ptr = &u.mh;
        target_fractional_part_ptr = &u.mh_fraction;
        target_max_ptr = &u.mhmax;
    }
    else
    {
        target_integer_part_ptr = &u.uhp;
        target_fractional_part_ptr = &u.uhp_fraction;
        target_max_ptr = &u.uhpmax;
    }

    int hp_before = *target_integer_part_ptr;
    *target_integer_part_ptr -= integer_hp;
    *target_fractional_part_ptr -= fractional_hp;

    if (*target_fractional_part_ptr < 0)
    {
        int multiple = (abs(*target_fractional_part_ptr) / 10000) + 1;
        *target_integer_part_ptr -= multiple;
        *target_fractional_part_ptr += multiple * 10000;
    }
    else if (*target_fractional_part_ptr >= 10000)
    {
        int multiple = *target_fractional_part_ptr / 10000;
        *target_integer_part_ptr += multiple;
        *target_fractional_part_ptr -= multiple * 10000;
    }

    if (*target_integer_part_ptr >= *target_max_ptr)
    {
        *target_integer_part_ptr = *target_max_ptr;
        *target_fractional_part_ptr = 0;
    }
    context.botl = 1;

    int hp_after = *target_integer_part_ptr;
    int damage_dealt = hp_before - hp_after;

    if (damage_dealt != 0 && isok(u.ux, u.uy))
    {
        char buf[BUFSZ];
        if(damage_dealt > 0)
            Sprintf(buf, "%d", -damage_dealt);
        else
            Sprintf(buf, "+%d", -damage_dealt);

        display_floating_text(u.ux, u.uy, buf, damage_dealt > 0 ? FLOATING_TEXT_DAMAGE : FLOATING_TEXT_HEALING, ATR_NONE, NO_COLOR, 0UL);
    }

    if (damage_dealt > 0)
    {
        context.last_turn_when_took_damage = moves;
    }

    if (iflags.using_gui_tiles)
    {
        if (flags.show_tile_u_hp_bar)
            newsym(u.ux, u.uy);
        else
            refresh_u_tile_gui_info(TRUE);
    }

    if (hp_d > 0 && critically_low_hp(TRUE))
    {
        pray_hint("heal yourself", "drinking a potion of healing or retreating to another level to heal up", &u.uhint.low_hit_points);
    }

    return *target_integer_part_ptr;
}



int
deduct_monster_hp(mtmp, hp_d)
struct monst* mtmp;
double hp_d;
{
    if (!mtmp)
        return 0;

//    mtmp->mhp -= (int)ceil(hp_d);
    int hp_before = mtmp->mhp;
    int integer_hp = (int)hp_d;
    double fracionalpart_hp_d = hp_d - (double)integer_hp;
    int fractional_hp = (int)(10000 * fracionalpart_hp_d);

    mtmp->mhp -= integer_hp;
    mtmp->mhp_fraction -= fractional_hp;

    if (mtmp->mhp_fraction < 0)
    {
        int multiple = (abs(mtmp->mhp_fraction) / 10000) + 1;
        mtmp->mhp -= multiple;
        mtmp->mhp_fraction += multiple * 10000;
    }
    else if (mtmp->mhp_fraction >= 10000)
    {
        int multiple = mtmp->mhp_fraction / 10000;
        mtmp->mhp += multiple;
        mtmp->mhp_fraction -= multiple * 10000;
    }

    if (mtmp->mhp >= mtmp->mhpmax)
    {
        mtmp->mhp = mtmp->mhpmax;
        mtmp->mhp_fraction = 0;
    }

    int hp_after = mtmp->mhp;
    int damage_dealt = hp_before - hp_after;

    if (iflags.using_gui_tiles && canspotmon(mtmp))
    {
        if ((!is_tame(mtmp) && flags.show_tile_mon_hp_bar) || (is_tame(mtmp) && flags.show_tile_pet_hp_bar))
            newsym(mtmp->mx, mtmp->my);
        else
            refresh_m_tile_gui_info(mtmp, TRUE);
    }

    if (iflags.wc2_statuslines > 3 && is_tame(mtmp))
        context.botl = 1;

    if (canspotmon(mtmp) && damage_dealt != 0 && isok(mtmp->mx, mtmp->my))
    {
        char buf[BUFSZ];
        if(damage_dealt > 0)
            Sprintf(buf, "%d", -damage_dealt);
        else
            Sprintf(buf, "+%d", -damage_dealt);

        display_floating_text(mtmp->mx, mtmp->my, buf, damage_dealt > 0 ? FLOATING_TEXT_DAMAGE : FLOATING_TEXT_HEALING, ATR_NONE, NO_COLOR, 0UL);
    }

    return mtmp->mhp;
}

void
update_u_facing(update_symbol)
uchar update_symbol;
{
    if (u.dx != 0)
    {
        boolean facing_before = u.facing_right;
        if (u.dx < 0)
            u.facing_right = FALSE;
        else if (u.dx > 0)
            u.facing_right = TRUE;

        boolean steed_facing_different = FALSE;
        if (u.usteed)
        {
            boolean steed_facing_before = u.usteed->facing_right;
            if (u.dx < 0)
                u.usteed->facing_right = FALSE;
            else if (u.dx > 0)
                u.usteed->facing_right = TRUE;

            steed_facing_different = u.usteed->facing_right != steed_facing_before;
        }

        if (update_symbol && (u.facing_right != facing_before || steed_facing_different))
            newsym(u.ux, u.uy);
        if (update_symbol == 2 && (u.facing_right != facing_before || steed_facing_different))
            flush_screen(1);
    }
}

void
update_u_action(action)
enum action_tile_types action;
{
    update_u_action_core(action, 3, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
}

void
update_u_action_revert(action)
enum action_tile_types action;
{
    update_u_action_core(action, 0, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    u_wait_until_action();
}

void
update_u_action_and_wait(action)
enum action_tile_types action;
{
    update_u_action_core(action, 3, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    u_wait_until_action();
}

void
update_u_action_core(action, simple_wait_multiplier, additional_newsym_flags)
enum action_tile_types action;
uint64_t simple_wait_multiplier;
uint64_t additional_newsym_flags;
{
    enum action_tile_types action_before = u.action;
    if (iflags.using_gui_tiles && action == ACTION_TILE_NO_ACTION)
    {
        if (context.u_intervals_to_wait_until_end > 0)
        {
            delay_output_intervals((int)context.u_intervals_to_wait_until_end);
            context.u_intervals_to_wait_until_end = 0UL;
        }
    }

    context.u_intervals_to_wait_until_action = 0UL;
    context.u_intervals_to_wait_until_end = 0UL;
    //context.u_action_animation_counter = 0L;
    context.u_action_animation_counter_on = FALSE;
    toggle_animation_timer(ANIMATION_TIMER_YOU, 0, FALSE, 0, 0, 0, 0UL);

    u.action = action;

    if (iflags.using_gui_tiles && action_before != u.action)
    {
        context.force_allow_keyboard_commands = TRUE;
        enum animation_types anim = get_player_animation(action, urole.rolenum, urace.racenum, flags.female, u.ualign.type, 0);
        if (u.action != ACTION_TILE_NO_ACTION && anim > 0
            && animations[anim].play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY && !u.usteed)
        {
            //context.u_action_animation_counter = 0L;
            context.u_action_animation_counter_on = TRUE;
            toggle_animation_timer(ANIMATION_TIMER_YOU, 0, TRUE, u.ux, u.uy, 0, 0UL);
            newsym_with_flags(u.ux, u.uy, additional_newsym_flags);
            force_redraw_at(u.ux, u.uy);
            flush_screen(1);
            int framenum = animations[anim].number_of_frames + (animations[anim].main_tile_use_style != ANIMATION_MAIN_TILE_IGNORE ? 1 : 0);
            if (animations[anim].sound_play_frame <= -1)
            {
                context.u_intervals_to_wait_until_action = animations[anim].intervals_between_frames * framenum;
            }
            else
            {
                delay_output_intervals((int)animations[anim].intervals_between_frames * animations[anim].sound_play_frame);
                if (animations[anim].action_execution_frame > animations[anim].sound_play_frame)
                {
                    context.u_intervals_to_wait_until_action = animations[anim].intervals_between_frames * (animations[anim].action_execution_frame - animations[anim].sound_play_frame);
                    if (animations[anim].action_execution_frame < framenum)
                        context.u_intervals_to_wait_until_end = animations[anim].intervals_between_frames * (framenum - animations[anim].action_execution_frame);
                }
                else
                {
                    context.u_intervals_to_wait_until_action = animations[anim].intervals_between_frames * (framenum - animations[anim].sound_play_frame);
                    context.u_intervals_to_wait_until_end = 0UL;
                }
            }
        }
        else
        {
            newsym_with_flags(u.ux, u.uy, additional_newsym_flags);
            flush_screen(1);
            context.u_intervals_to_wait_until_action = DELAY_OUTPUT_INTERVAL_IN_ANIMATION_INTERVALS;
            //adjusted_delay_output();
            if (u.action != ACTION_TILE_NO_ACTION)
            {
                context.u_intervals_to_wait_until_action *= simple_wait_multiplier;
                //adjusted_delay_output();
                //adjusted_delay_output();
            }
        }
        context.force_allow_keyboard_commands = FALSE;
    }
}

void
update_m_action(mtmp, action)
struct monst* mtmp;
enum action_tile_types action;
{
    update_m_action_core(mtmp, action, 2, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
}

void
update_m_action_revert(mtmp, action)
struct monst* mtmp;
enum action_tile_types action;
{
    if (!mtmp)
        return;
    update_m_action_core(mtmp, action, 0, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    if(mtmp == &youmonst)
        u_wait_until_action();
    else if(action == ACTION_TILE_DEATH ? canspotmon(mtmp) : canseemon(mtmp))
        m_wait_until_action((struct monst*)0, action); // Visibility check is above
}

void
update_m_action_and_wait(mtmp, action)
struct monst* mtmp;
enum action_tile_types action;
{
    if (!mtmp)
        return;
    update_m_action_core(mtmp, action, 2, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS | NEWSYM_FLAGS_KEEP_OLD_FLAGS);
    if (mtmp == &youmonst)
        u_wait_until_action();
    else if (action == ACTION_TILE_DEATH ? canspotmon(mtmp) : canseemon(mtmp))
        m_wait_until_action((struct monst*)0, action); // Visibility check is above
}



void
update_m_action_core(mtmp, action, simple_wait_multiplier, additional_newsym_flags)
struct monst* mtmp;
enum action_tile_types action;
uint64_t simple_wait_multiplier;
uint64_t additional_newsym_flags;
{
    if (!mtmp)
        return;

    if (mtmp == &youmonst)
    {
        update_u_action_core(action, simple_wait_multiplier, additional_newsym_flags);
        return;
    }

    if (!should_display_m_action_tile(mtmp, action))
        return;

    enum action_tile_types action_before = mtmp->action;
    if (iflags.using_gui_tiles && action == ACTION_TILE_NO_ACTION)
    {
        if (context.m_intervals_to_wait_until_end > 0)
        {
            if(canseemon(mtmp))
                delay_output_intervals((int)context.m_intervals_to_wait_until_end);
            context.m_intervals_to_wait_until_end = 0UL;
        }
    }

    context.m_intervals_to_wait_until_action = 0UL;
    context.m_intervals_to_wait_until_end = 0UL;
    context.m_action_animation_x = 0;
    context.m_action_animation_y = 0;
    //context.m_action_animation_counter = 0L;
    context.m_action_animation_counter_on = FALSE;
    toggle_animation_timer(ANIMATION_TIMER_MONSTER, 0, FALSE, 0, 0, 0, 0UL);

    mtmp->action = action;

    if (iflags.using_gui_tiles && (action == ACTION_TILE_DEATH ? canspotmon(mtmp) : canseemon(mtmp)) && action_before != mtmp->action)
    {
        enum animation_types anim = mtmp->data->animation.actions[action];
        if (mtmp->action != ACTION_TILE_NO_ACTION && anim > 0
            && animations[anim].play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY)
        {
            context.m_action_animation_x = mtmp->mx;
            context.m_action_animation_y = mtmp->my;
            //context.m_action_animation_counter = 0L;
            context.m_action_animation_counter_on = TRUE;
            toggle_animation_timer(ANIMATION_TIMER_MONSTER, 0, TRUE, mtmp->mx, mtmp->my, 0, 0UL);
            newsym_with_flags(mtmp->mx, mtmp->my, additional_newsym_flags);
            force_redraw_at(mtmp->mx, mtmp->my);
            flush_screen(1);
            int framenum = animations[anim].number_of_frames + (animations[anim].main_tile_use_style != ANIMATION_MAIN_TILE_IGNORE ? 1 : 0);
            if (animations[anim].sound_play_frame <= -1)
            {
                context.m_intervals_to_wait_until_action = animations[anim].intervals_between_frames * framenum;
            }
            else
            {
                delay_output_intervals((int)animations[anim].intervals_between_frames * animations[anim].sound_play_frame);
                if (animations[anim].action_execution_frame > animations[anim].sound_play_frame)
                {
                    context.m_intervals_to_wait_until_action = animations[anim].intervals_between_frames * (animations[anim].action_execution_frame - animations[anim].sound_play_frame);
                    if (animations[anim].action_execution_frame < framenum)
                        context.m_intervals_to_wait_until_end = animations[anim].intervals_between_frames * (framenum - animations[anim].action_execution_frame);
                }
                else
                {
                    context.m_intervals_to_wait_until_action = animations[anim].intervals_between_frames * (framenum - animations[anim].sound_play_frame);
                    context.m_intervals_to_wait_until_end = 0UL;
                }
            }
        }
        else
        {
            newsym_with_flags(mtmp->mx, mtmp->my, additional_newsym_flags);
            flush_screen(1);
            if(action == ACTION_TILE_DEATH ? canspotmon(mtmp) : canseemon(mtmp))
            {
                context.m_intervals_to_wait_until_action = DELAY_OUTPUT_INTERVAL_IN_ANIMATION_INTERVALS;
                if (mtmp->action != ACTION_TILE_NO_ACTION)
                {
                    context.m_intervals_to_wait_until_action *= simple_wait_multiplier;
                }
            }
        }
    }
}

void 
u_wait_until_action()
{
    if (context.u_intervals_to_wait_until_action > 0UL)
    {
        delay_output_intervals((int)context.u_intervals_to_wait_until_action);
        context.u_intervals_to_wait_until_action = 0UL;
    }
}

void
m_wait_until_action(mon, action)
struct monst* mon;
enum action_tile_types action;
{
    if (context.m_intervals_to_wait_until_action > 0UL)
    {
        if (!mon || mon == &youmonst || (action == ACTION_TILE_DEATH ? canspotmon(mon) : canseemon(mon)))
            delay_output_intervals((int)context.m_intervals_to_wait_until_action);
        context.m_intervals_to_wait_until_action = 0UL;
    }
}

void
u_wait_until_end()
{
    if (context.u_intervals_to_wait_until_end > 0UL)
    {
        delay_output_intervals((int)context.u_intervals_to_wait_until_end);
        context.u_intervals_to_wait_until_end = 0UL;
    }
}

void
m_wait_until_end(mon, action)
struct monst* mon;
enum action_tile_types action;
{
    if (context.m_intervals_to_wait_until_end > 0UL)
    {
        if (!mon || mon == &youmonst || (action == ACTION_TILE_DEATH ? canspotmon(mon) : canseemon(mon)))
            delay_output_intervals((int)context.m_intervals_to_wait_until_end);
        context.m_intervals_to_wait_until_end = 0UL;
    }
}

void
display_being_hit(mon, x, y, hit_symbol_shown, damage_shown, extra_mflags)
struct monst* mon;
int x, y;
enum hit_tile_types hit_symbol_shown;
int damage_shown;
uint64_t extra_mflags;
{
    if (!iflags.using_gui_tiles || hit_symbol_shown >= MAX_HIT_TILES || hit_symbol_shown < 0)
        return;

    uint64_t mhflags = (LMFLAGS_BEING_HIT | extra_mflags);
    show_extra_info(x, y, 0UL, mhflags, (short)(hit_symbol_shown - HIT_GENERAL), damage_shown);
    if(mon == &youmonst)
        u_wait_until_action();
    else
        m_wait_until_action((struct monst*)0, ACTION_TILE_NO_ACTION); // Visibility check has been before
    flush_screen(1);
    adjusted_delay_output();
    adjusted_delay_output();
    newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS);
    flush_screen(1);
}

void
display_u_being_hit(hit_symbol_shown, damage_shown, extra_mflags)
enum hit_tile_types hit_symbol_shown;
int damage_shown;
uint64_t extra_mflags;
{
    display_being_hit(&youmonst, u.ux, u.uy, hit_symbol_shown, damage_shown, extra_mflags);
}

void
display_m_being_hit(mon, hit_symbol_shown, damage_shown, extra_mflags, use_bhitpos)
struct monst* mon;
enum hit_tile_types hit_symbol_shown;
int damage_shown;
uint64_t extra_mflags;
boolean use_bhitpos;
{
    if (!mon)
        return;

    int x = use_bhitpos ? bhitpos.x : mon->mx;
    int y = use_bhitpos ? bhitpos.y : mon->my;
    if(!(u.uswallow && mon == u.ustuck) && isok(x, y) && cansee(x, y)) // Show hit to invisible, if you can see the location
        display_being_hit(mon, x, y, hit_symbol_shown, damage_shown, extra_mflags);
}

/*uhitm.c*/

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    polyself.c    $NHDT-Date: 1556497911 2019/04/29 00:31:51 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.132 $ */
/*      Copyright (C) 1987, 1988, 1989 by Ken Arromdee */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * Polymorph self routine.
 *
 * Note:  the light source handling code assumes that both youmonst.m_id
 * and youmonst.mx will always remain 0 when it handles the case of the
 * player polymorphed into a light-emitting monster.
 *
 * Transformation sequences:
 *              /-> polymon                 poly into monster form
 *    polyself =
 *              \-> newman -> polyman       fail to poly, get human form
 *
 *    rehumanize -> polyman                 return to original form
 *
 *    polymon (called directly)             usually golem petrification
 */

#include "hack.h"

STATIC_DCL void FDECL(check_strangling, (BOOLEAN_P));
STATIC_DCL void FDECL(polyman, (const char *, const char *));
STATIC_DCL void NDECL(break_armor);
STATIC_DCL void FDECL(drop_weapon, (int));
STATIC_DCL int FDECL(armor_to_dragon, (int));
STATIC_DCL void NDECL(newman);
STATIC_DCL void NDECL(polysense);

STATIC_VAR const char no_longer_petrify_resistant[] =
    "No longer petrify-resistant, you";

/* controls whether taking on new form or becoming new man can also
   change sex (ought to be an arg to polymon() and newman() instead) */
STATIC_VAR int sex_change_ok = 0;

/* update the youmonst.data structure pointer and intrinsics */
void
set_uasmon()
{
    struct permonst *mdat = &mons[u.umonnum];
    int i;

    set_mon_data(&youmonst, mdat);

    /* Clear out FROM_FORMs*/
    for (i = 1; i <= LAST_PROP; i++)
    {
        u.uprops[i].intrinsic &= ~FROM_FORM;
    }

    unsigned long bit = 1UL;
    for (i = 1; i <= 32; i++)
    {
        int prop = 0;
        if(i > 1)
            bit = bit << 1;

        prop = innate_to_prop(bit);
        if (prop > 0)
        {
            if (has_innate(youmonst.data, bit))
                u.uprops[prop].intrinsic |= FROM_FORM;
        }
        prop = innate2_to_prop(bit);
        if (prop > 0)
        {
            if (has_innate2(youmonst.data, bit))
                u.uprops[prop].intrinsic |= FROM_FORM;
        }
    }

#define PROPSET(PropIndx, condition)                          \
    do {                                               \
        if (condition)                                        \
            u.uprops[PropIndx].intrinsic |= FROM_FORM;  \
    } while (0)

    /* We add these additionally in the case creature type (e.g., undead) has resistances not conferred explicitly by MR_ fields */
    PROPSET(SLEEP_RESISTANCE, resists_sleep(&youmonst));
    PROPSET(DISINTEGRATION_RESISTANCE, resists_disint(&youmonst));
    PROPSET(DEATH_RESISTANCE, resists_death(&youmonst));
    PROPSET(CHARM_RESISTANCE, resists_charm(&youmonst));
    PROPSET(FEAR_RESISTANCE, resists_fear(&youmonst));
    PROPSET(POISON_RESISTANCE, resists_poison(&youmonst));
    PROPSET(STONE_RESISTANCE, resists_ston(&youmonst));
    PROPSET(DRAIN_RESISTANCE, resists_drli(&youmonst));
    PROPSET(STUN_RESISTANCE, resists_stun(&youmonst));
    PROPSET(BISECTION_RESISTANCE, resists_bisection(&youmonst));
    PROPSET(SLIME_RESISTANCE, resists_slime(&youmonst));

    PROPSET(FIRE_IMMUNITY, is_mon_immune_to_fire(&youmonst));
    PROPSET(COLD_IMMUNITY, is_mon_immune_to_cold(&youmonst));
    PROPSET(SHOCK_IMMUNITY, is_mon_immune_to_elec(&youmonst));
    PROPSET(MAGIC_MISSILE_IMMUNITY, is_mon_immune_to_magic_missile(&youmonst));
    PROPSET(ACID_IMMUNITY, is_mon_immune_to_acid(&youmonst));

    PROPSET(FIRE_RESISTANCE, mon_resists_fire_weakly(&youmonst));
    PROPSET(COLD_RESISTANCE, mon_resists_cold_weakly(&youmonst));
    PROPSET(SHOCK_RESISTANCE, mon_resists_elec_weakly(&youmonst));
    PROPSET(MAGIC_MISSILE_RESISTANCE, mon_resists_magic_missile_weakly(&youmonst));
    PROPSET(ACID_RESISTANCE, mon_resists_acid_weakly(&youmonst));

    /* Strong resistances cannot be innate */

#if 0
    {
        /* resists_drli() takes wielded weapon into account; suppress it */
        struct obj *save_uwep = uwep;

        uwep = 0;
        PROPSET(DRAIN_RESISTANCE, resists_drli(&youmonst));
        uwep = save_uwep;
    }
    /* is_magic_resistant() takes wielded, worn, and carried equipment into
       into account; cheat and duplicate its monster-specific part */
    PROPSET(ANTIMAGIC, (dmgtype(mdat, AD_MAGM)
                        || mdat == &mons[PM_GRAY_DRAGON_HATCHLING]
                        || dmgtype(mdat, AD_RBRE)));
//    PROPSET(STUNNED, (mdat == &mons[PM_STALKER] || is_bat(mdat))); /* Not sure what this was about --JG */
    PROPSET(SEE_INVISIBLE, has_innate_see_invisible(mdat));
    PROPSET(BLIND_TELEPATHY, has_innate_blind_telepathy(mdat));
    PROPSET(TELEPAT, has_innate_telepathy(mdat));
    /* note that Infravision uses mons[race] rather than usual mons[role] */
    PROPSET(INVISIBILITY, has_innate_invisibility(mdat));
    PROPSET(TELEPORT, has_innate_teleportation(mdat));
    PROPSET(TELEPORT_CONTROL, has_innate_teleport_control(mdat));
    /* floating eye is the only 'floater'; it is also flagged as a 'flyer';
       suppress flying for it so that enlightenment doesn't confusingly
       show latent flight capability always blocked by levitation */
    PROPSET(LEVITATION_CONTROL, has_innate_levitation_control(mdat));
    /* [don't touch MAGICAL_BREATHING here; both Amphibious and Breathless
       key off of it but include different monster forms...] */
    PROPSET(REGENERATION, has_innate_regeneration(mdat));
    PROPSET(REFLECTING, is_reflecting(&youmonst));
#endif
    PROPSET(SICK_RESISTANCE, (mdat->mlet == S_FUNGUS || mdat == &mons[PM_GHOUL]));
    PROPSET(HALLUC_RES, dmgtype(mdat, AD_HALU));
    PROPSET(INFRAVISION, infravision(Upolyd ? mdat : &mons[urace.monsternum]));
    PROPSET(LEVITATION, is_floater(mdat));
    PROPSET(FLYING, (is_flyer(mdat)));
    PROPSET(SWIMMING, is_swimmer(mdat));
    PROPSET(PASSES_WALLS, passes_walls(mdat));
#undef PROPSET

    float_vs_flight(); /* maybe toggle (BFlying & I_SPECIAL) */
    polysense();
    status_reassess();
}

/* Levitation overrides Flying; set or clear BFlying|I_SPECIAL */
void
float_vs_flight()
{
    boolean stuck_in_floor = (u.utrap && u.utraptype != TT_PIT);

    /* floating overrides flight; so does being trapped in the floor */
    if ((HLevitation || ELevitation)
        || ((HFlying || EFlying) && stuck_in_floor))
        HBlocks_Flying |= I_SPECIAL;
    else
        HBlocks_Flying &= ~I_SPECIAL;
    /* being trapped on the ground (bear trap, web, molten lava survived
       with fire resistance, former lava solidified via cold, tethered
       to a buried iron ball) overrides floating--the floor is reachable */
    if ((HLevitation || ELevitation) && stuck_in_floor)
        HBlocks_Levitation |= I_SPECIAL;
    else
        HBlocks_Levitation &= ~I_SPECIAL;
    context.botl = context.botlx = TRUE;
}

/* for changing into form that's immune to strangulation */
STATIC_OVL void
check_strangling(on)
boolean on;
{
    /* on -- maybe resume strangling */
    if (on) {
        /* when Strangled is already set, polymorphing from one
           vulnerable form into another causes the counter to be reset */
        if (uamul && uamul->otyp == AMULET_OF_STRANGULATION
            && can_be_strangled(&youmonst)) {
            Strangled = 6L;
            context.botl = context.botlx = TRUE;
            Your("%s %s your %s!", simpleonames(uamul),
                 Strangled ? "still constricts" : "begins constricting",
                 body_part(NECK)); /* "throat" */
            makeknown(AMULET_OF_STRANGULATION);
        }

    /* off -- maybe block strangling */
    } else {
        if (Strangled && !can_be_strangled(&youmonst)) {
            Strangled = 0L;
            context.botl = context.botlx = TRUE;
            You("are no longer being strangled.");
        }
    }
}

/* make a (new) human out of the player */
STATIC_OVL void
polyman(fmt, arg)
const char *fmt, *arg;
{
    boolean sticky = (sticks(youmonst.data) && u.ustuck && !u.uswallow),
            was_mimicking = (U_AP_TYPE == M_AP_OBJECT);
    boolean was_blind = !!Blind;

    if (Upolyd) {
        u.acurr = u.macurr; /* restore old attribs */
        u.amin = u.mamin;
        u.amax = u.mamax;
        u.umonnum = u.umonster;
        flags.female = u.mfemale;
    }
    set_uasmon();

    u.mh = u.mhmax = u.basemhmax = 0;
    u.mtimedone = 0;
    skinback(FALSE);
    u.uundetected = 0;

    if (sticky)
        uunstick();
    find_ac();
    find_mc();
    if (was_mimicking) {
        if (multi < 0)
            unmul("");
        youmonst.m_ap_type = M_AP_NOTHING;
    }

    newsym(u.ux, u.uy);

    You_ex(ATR_NONE, CLR_MSG_ATTENTION, fmt, arg);
    /* check whether player foolishly genocided self while poly'd */
    if (ugenocided()) {
        /* intervening activity might have clobbered genocide info */
        struct kinfo *kptr = find_delayed_killer(POLYMORPH);

        if (kptr != (struct kinfo *) 0 && kptr->name[0]) {
            killer.format = kptr->format;
            Strcpy(killer.name, kptr->name);
        } else {
            killer.format = KILLED_BY;
            Strcpy(killer.name, "self-genocide");
        }
        dealloc_killer(kptr);
        done(GENOCIDED);
    }

    //if (u.twoweap && !could_twoweap(youmonst.data))
    //    untwoweapon();

    if (u.utrap && u.utraptype == TT_PIT) {
        set_utrap(rn1(6, 2), TT_PIT); /* time to escape resets */
    }
    if (was_blind && !Blind) { /* reverting from eyeless */
        Blinded = 1L;
        make_blinded(0L, TRUE); /* remove blindness */
    }
    check_strangling(TRUE);

    if (!Levitation && !u.ustuck && is_pool_or_lava(u.ux, u.uy))
        spoteffects(TRUE);

    see_monsters();
}

void
change_sex()
{
    /* setting u.umonster for caveman/cavewoman or priest/priestess
       swap unintentionally makes `Upolyd' appear to be true */
    boolean already_polyd = (boolean) Upolyd;

    /* Some monsters are always of one sex and their sex can't be changed;
     * Succubi/incubi can change, but are handled below.
     *
     * !already_polyd check necessary because is_male() and is_female()
     * are true if the player is a priest/priestess.
     */
    if (!already_polyd
        || (!is_male(youmonst.data) && !is_female(youmonst.data)
            && !is_neuter(youmonst.data)))
        flags.female = !flags.female;
    if (already_polyd) /* poly'd: also change saved sex */
        u.mfemale = !u.mfemale;
    max_rank_sz(); /* [this appears to be superfluous] */
    if ((already_polyd ? u.mfemale : flags.female) && urole.name.f)
        Strcpy(pl_character, urole.name.f);
    else
        Strcpy(pl_character, urole.name.m);
    u.umonster = urole.monsternum;
    if (!already_polyd) {
        u.umonnum = u.umonster;
    }
}

STATIC_OVL void
newman()
{
    int i, oldlvl, newlvl, hpmax, enmax;

    oldlvl = u.ulevel;
    newlvl = oldlvl + rn1(5, -2);     /* new = old + {-2,-1,0,+1,+2} */
    if (newlvl > 127 || newlvl < 1) { /* level went below 0? */
        goto dead; /* old level is still intact (in case of lifesaving) */
    }
    if (newlvl > MAXULEV)
        newlvl = MAXULEV;
    /* If your level goes down, your peak level goes down by
       the same amount so that you can't simply use blessed
       full healing to undo the decrease.  But if your level
       goes up, your peak level does *not* undergo the same
       adjustment; you might end up losing out on the chance
       to regain some levels previously lost to other causes. */
    if (newlvl < oldlvl)
        u.ulevelmax -= (oldlvl - newlvl);
    if (u.ulevelmax < newlvl)
        u.ulevelmax = newlvl;
    u.ulevel = newlvl;

    if (sex_change_ok && !rn2(10))
        change_sex();

    adjabil(oldlvl, (int) u.ulevel);
    reset_rndmonst(NON_PM); /* new monster generation criteria */

    /* random experience points for the new experience level */
    u.uexp = rndexp(FALSE);

    /* set up new attribute points (particularly Con) */
    redist_attr();

    /*
     * New hit points:
     *  remove level-gain based HP from any extra HP accumulated
     *  (the "extra" might actually be negative);
     *  modify the extra, retaining {80%, 90%, 100%, or 110%};
     *  add in newly generated set of level-gain HP.
     *
     * (This used to calculate new HP in direct proportion to old HP,
     * but that was subject to abuse:  accumulate a large amount of
     * extra HP, drain level down to 1, then polyself to level 2 or 3
     * [lifesaving capability needed to handle level 0 and -1 cases]
     * and the extra got multiplied by 2 or 3.  Repeat the level
     * drain and polyself steps until out of lifesaving capability.)
     */
    hpmax = u.ubasehpmax;
    for (i = 0; i < oldlvl; i++)
        hpmax -= (int) u.uhpinc[i];
    /* hpmax * rn1(4,8) / 10; 0.95*hpmax on average */
    hpmax = rounddiv(hpmax * rn1(4, 8), 10);
    for (i = 0; (u.ulevel = i) < newlvl; i++)
        hpmax += newhp();
    if (hpmax < u.ulevel)
        hpmax = u.ulevel; /* min of 1 HP per level */
    /* retain same proportion for current HP; u.uhp * hpmax / u.uhpmax */
    int oldhpmax = u.uhpmax;
    u.ubasehpmax = hpmax;
    updatemaxhp();
    u.uhp = rounddiv(u.uhp * oldhpmax, u.uhpmax);
    /*
     * Do the same for spell power.
     */
    enmax = u.ubaseenmax;
    for (i = 0; i < oldlvl; i++)
        enmax -= (int) u.ueninc[i];
    enmax = rounddiv(enmax * rn1(4, 8), 10);
    for (i = 0; (u.ulevel = i) < newlvl; i++)
        enmax += newpw();
    if (enmax < u.ulevel)
        enmax = u.ulevel;
    u.uen = rounddiv(u.uen * enmax,
                     ((u.uenmax < 1) ? 1 : u.uenmax));
    u.ubaseenmax = enmax;
    updatemaxen();
    /* [should alignment record be tweaked too?] */

    u.uhunger = rn1(500, 500);
    if (Sick)
        make_sick(0L, (char *) 0, FALSE, 0);
    if (FoodPoisoned)
        make_food_poisoned(0L, (char*)0, FALSE, 0);
    if (MummyRot)
        make_mummy_rotted(0L, (char*)0, FALSE, 0);
    if (Stoned)
        make_stoned(0L, (char *) 0, 0, (char *) 0, 0);
    if (u.uhp <= 0) {
        if (Polymorph_control) { /* even when Stunned || Unaware */
            if (u.uhp <= 0)
                u.uhp = 1;
        } else {
        dead: /* we come directly here if their experience level went to 0 or
                 less */
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "new form doesn't seem healthy enough to survive.");
            killer.format = KILLED_BY_AN;
            Strcpy(killer.name, "unsuccessful polymorph");
            done(DIED);
            update_hunger_status(FALSE);
            return; /* lifesaved */
        }
    }
    update_hunger_status(FALSE);
    play_sfx_sound(SFX_POLYMORPH_NEW_MAN);
    polyman("feel like a new %s!",
            /* use saved gender we're about to revert to, not current */
            ((Upolyd ? u.mfemale : flags.female) && urace.individual.f)
                ? urace.individual.f
                : (urace.individual.m)
                   ? urace.individual.m
                   : urace.noun);
    if (Slimed) {
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "body transforms, but there is still slime on you.");
        make_slimed(10L, (const char *) 0, KILLED_BY, "sliming", HINT_KILLED_SLIMED);
    }

    context.botl = context.botlx = TRUE;
    see_monsters();
    (void) encumber_msg();

    retouch_equipment(2);
    if (!uarmg)
        selftouch(no_longer_petrify_resistant);

    newsym(u.ux, u.uy);
}

void
polyself(psflags)
int psflags;
{
    char buf[BUFSZ] = DUMMY;
    int old_light, new_light, old_ambient, new_ambient, mntmp, class, tryct;
    boolean forcecontrol = (psflags == 1), monsterpoly = (psflags == 2),
            draconian = (uarm && is_dragon_scale_armor(uarm)),
            bullheaded =
                ((umisc && umisc->otyp == NOSE_RING_OF_BULLHEADEDNESS)
                    || (umisc2 && umisc2->otyp == NOSE_RING_OF_BULLHEADEDNESS)
                    || (umisc3 && umisc3->otyp == NOSE_RING_OF_BULLHEADEDNESS)
                    || (umisc4 && umisc4->otyp == NOSE_RING_OF_BULLHEADEDNESS)
                    || (umisc5 && umisc5->otyp == NOSE_RING_OF_BULLHEADEDNESS)
                    ),
            iswere = (u.ulycn >= LOW_PM), isvamp = is_vampire(youmonst.data),
            controllable_poly = Polymorph_control && !(Stunned || Unaware);

    if (Unchanging) 
    {
        if (!wizard || (wizard && yn_query("You are unchanging. Force polymorph anyway?") != 'y'))
        {
            play_sfx_sound(SFX_POLYMORPH_FAIL);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "You fail to transform!");
            return;
        }
    }

    /* being Stunned|Unaware doesn't negate this aspect of Poly_control */
    if (!Polymorph_control && !forcecontrol && !draconian && !bullheaded && !iswere
        && !isvamp) {
        if (rn2(20) > ACURR(A_CON)) 
        {
            if (!wizard || (wizard && yn_query("You are about to shudder. Force polymorph instead?") != 'y'))
            {
                play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
                You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
                losehp(adjust_damage(rnd(30), (struct monst*)0, &youmonst, AD_SHOC, ADFLAGS_NONE), "system shock", KILLED_BY_AN);
                exercise(A_CON, FALSE);
                return;
            }
        }
    }
    old_light = emitted_light_range(youmonst.data);
    old_ambient = mon_ambient_sound(youmonst.data);
    mntmp = NON_PM;

    if (monsterpoly && isvamp)
        goto do_vampyr;

    if (controllable_poly || forcecontrol) 
    {
        tryct = 5;
        do {
            mntmp = NON_PM;
            getlin_ex(GETLINE_POLYMORPH, ATR_NONE, NO_COLOR, "Become what kind of monster?", buf, "type the name", (char*)0, (char*)0);
            (void) mungspaces(buf);
            if (*buf == '\033')
            {
                /* user is cancelling controlled poly */
                if (forcecontrol) { /* wizard mode #polyself */
                    pline1(Never_mind);
                    return;
                }
                Strcpy(buf, "*"); /* resort to random */
            }
            if (!strcmp(buf, "*") || !strcmp(buf, "random")) 
            {
                /* explicitly requesting random result */
                tryct = 0; /* will skip thats_enough_tries */
                continue;  /* end do-while(--tryct > 0) loop */
            }
            class = 0;
            mntmp = name_to_mon(buf);

            if (mntmp < LOW_PM) 
            {
            by_class:
                class = name_to_monclass(buf, &mntmp);
                if (class && mntmp == NON_PM)
                    mntmp = mkclass_poly(class);
            }

            if (mntmp < LOW_PM)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                if (!class)
                    pline("I've never heard of such monsters.");
                else
                    You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "polymorph into any of those.");
            } 
            else if (iswere && (were_beastie(mntmp) == u.ulycn
                                  || mntmp == counter_were(u.ulycn)
                                  || (Upolyd && mntmp == PM_HUMAN))) 
            {
                goto do_shift;
                /* Note:  humans are illegal as monsters, but an
                 * illegal monster forces newman(), which is what we
                 * want if they specified a human.... */
            } 
            else if (!polyok(&mons[mntmp])
                       && !(mntmp == PM_HUMAN || your_race(&mons[mntmp])
                            || mntmp == urole.monsternum))
            {
                const char *pm_name;

                /* mkclass_poly() can pick a !polyok()
                   candidate; if so, usually try again */
                if (class) {
                    if (rn2(3) || --tryct > 0)
                        goto by_class;
                    /* no retries left; put one back on counter
                       so that end of loop decrement will yield
                       0 and trigger thats_enough_tries message */
                    ++tryct;
                }
                pm_name = pm_monster_name(&mons[mntmp], flags.female);
                if (the_unique_pm(&mons[mntmp]))
                    pm_name = the(pm_name);
                else if (!is_mname_proper_name(&mons[mntmp]))
                    pm_name = an(pm_name);
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "polymorph into %s.", pm_name);
            }
            else
            {
                if (forcecontrol)
                    break;
                else if (mons[mntmp].difficulty > max(5, u.ulevel * 2))
                {
                    if(wizard && !forcecontrol && yn_query("Enforce polymorph control success?") == 'y')
                    {
                        forcecontrol = TRUE;
                        break;
                    }
                    else if (yn_query("This form may be too difficult for your polymorph control. Continue?") == 'y')
                        break;
                }
                else
                    break;
            }
        } while (--tryct > 0);
        if (!tryct)
            pline1(thats_enough_tries);
        /* allow skin merging, even when polymorph is controlled */
        if (draconian && (tryct <= 0 || mntmp == armor_to_dragon(uarm->otyp)))
            goto do_merge;
        if (bullheaded && (tryct <= 0 || mntmp == PM_MINOTAUR))
            goto do_minotaur;
        if (isvamp && (tryct <= 0 || mntmp == PM_WOLF || mntmp == PM_FOG_CLOUD
                       || is_bat(&mons[mntmp])))
            goto do_vampyr;

        if (!forcecontrol && mntmp >= LOW_PM && (mons[mntmp].difficulty > max(5, u.ulevel * 3) || (!rn2(2) && mons[mntmp].difficulty > max(5, u.ulevel * 2))))
        {
            /* Control fails -- Randomize instead */
            pline("Oops! That form was too difficult for your polymorph control!");
            mntmp = NON_PM;
        }
    } 
    else if (draconian || bullheaded || iswere || isvamp) 
    {
        /* special changes that don't require polyok() */
        if (draconian)
        {
        do_merge:
            mntmp = armor_to_dragon(uarm->otyp);
            if (!(mvitals[mntmp].mvflags & MV_GENOCIDED))
            {
                /* allow MV_EXTINCT */
                if (is_dragon_scales(uarm))
                {
                    /* dragon scales remain intact as uskin */
                    play_sfx_sound(SFX_POLYMORPH_SCALES_MERGE);
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "merge with your scaly armor.");
                } else { /* dragon scale mail */
                    /* d.scale mail first reverts to scales */
                    char *p, *dsmail;

                    /* similar to noarmor(invent.c),
                       shorten to "<color> scale mail" */
                    dsmail = strcpy(buf, simpleonames(uarm));
                    if ((p = strstri(dsmail, " dragon ")) != 0)
                        while ((p[1] = p[8]) != '\0')
                            ++p;
                    /* tricky phrasing; dragon scale mail
                       is singular, dragon scales are plural */
                    play_sfx_sound(SFX_POLYMORPH_SCALES_REVERT);
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s reverts to scales as you merge with them.",
                         dsmail);
                    /* uarm->enchantment enchantment remains unchanged;
                       re-converting scales to mail poses risk
                       of evaporation due to over enchanting */
                    uarm->otyp += GRAY_DRAGON_SCALES - GRAY_DRAGON_SCALE_MAIL;
                    uarm->dknown = 1;
                    context.botl = 1; /* AC is changing */
                }
                uskin = uarm;
                uarm = (struct obj *) 0;
                /* save/restore hack */
                uskin->owornmask |= I_SPECIAL;
                update_inventory();
            }
        } 
        else if (bullheaded)
        {
        do_minotaur:
            mntmp = PM_MINOTAUR;
        }
        else if (iswere)
        {
        do_shift:
            if (Upolyd && were_beastie(mntmp) != u.ulycn)
                mntmp = PM_HUMAN; /* Illegal; force newman() */
            else
                mntmp = u.ulycn;
        }
        else if (isvamp) 
        {
        do_vampyr:
            if (mntmp < LOW_PM || (mons[mntmp].geno & G_UNIQ))
                mntmp = (youmonst.data != &mons[PM_VAMPIRE] && !rn2(10))
                            ? PM_WOLF
                            : !rn2(4) ? PM_FOG_CLOUD : PM_VAMPIRE_BAT;
            if (controllable_poly) 
            {
                Sprintf(buf, "Become %s?", an(pm_monster_name(&mons[mntmp], flags.female)));
                if (yn_query(buf) != 'y')
                    return;
            }
        }
        /* if polymon fails, "you feel" message has been given
           so don't follow up with another polymon or newman;
           sex_change_ok left disabled here */
        if (mntmp == PM_HUMAN)
            newman(); /* werecritter */
        else
            (void) polymon(mntmp);
        goto made_change; /* maybe not, but this is right anyway */
    }

    if (mntmp < LOW_PM)
    {
        tryct = 200;
        struct permonst* pm = 0;
        do
        {
            /* randomly pick an "ordinary" monster */
            pm = rndmonst();
            if (pm)
                mntmp = monsndx(pm);
            else if (tryct < 100)
                mntmp = rn1(SPECIAL_PM - LOW_PM, LOW_PM);
            else
                continue;

            if (mntmp == NON_PM)
                continue;

            if (polyok(&mons[mntmp]) && !is_placeholder(&mons[mntmp]))
                break;
        } 
        while (--tryct > 0);
    }

    /* The below polyok() fails either if everything is genocided, or if
     * we deliberately chose something illegal to force newman().
     */
    sex_change_ok++;
    if (!polyok(&mons[mntmp]) || (!wizard && ((!forcecontrol && !rn2(5))
        || (your_race(&mons[mntmp]) && !is_undead(&mons[mntmp]))))
        )
    {
        newman();
    }
    else
    {
        (void) polymon(mntmp);
    }
    sex_change_ok--; /* reset */

made_change:
    new_light = emitted_light_range(youmonst.data);
    if (old_light != new_light)
    {
        if (old_light)
            del_light_source(LS_MONSTER, monst_to_any(&youmonst));
        if (new_light == 1)
            ++new_light; /* otherwise it's undetectable */
        if (new_light)
            new_light_source(u.ux, u.uy, new_light, LS_MONSTER,
                             monst_to_any(&youmonst));
    }
    new_ambient = mon_ambient_sound(youmonst.data);
    if (old_ambient != new_ambient)
    {
        if (old_ambient)
            del_sound_source(SOUNDSOURCE_MONSTER, monst_to_any(&youmonst));
        if (new_ambient == 1)
            ++new_ambient; /* otherwise it's undetectable */
        if (new_ambient)
            new_sound_source(u.ux, u.uy, new_ambient, (double)mon_ambient_volume(youmonst.data), SOUNDSOURCE_MONSTER, mon_ambient_subtype(youmonst.data),
                monst_to_any(&youmonst));
    }
}

/* (try to) make a mntmp monster out of the player;
   returns 1 if polymorph successful */
int
polymon(mntmp)
int mntmp;
{
    char buf[BUFSZ];
    boolean sticky = sticks(youmonst.data) && u.ustuck && !u.uswallow,
            was_blind = !!Blind, dochange = FALSE;
    int mlvl;

    if (mvitals[mntmp].mvflags & MV_GENOCIDED) { /* allow MV_EXTINCT */
        play_sfx_sound(SFX_POLYMORPH_FAIL);
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "rather %s-ish.", pm_monster_name(&mons[mntmp], flags.female));
        exercise(A_WIS, TRUE);
        return 0;
    }

    /* KMH, conduct */
    u.uconduct.polyselfs++;

    /* exercise used to be at the very end but only Wis was affected
       there since the polymorph was always in effect by then */
    exercise(A_CON, FALSE);
    exercise(A_WIS, TRUE);

    if (!Upolyd) {
        /* Human to monster; save human stats */
        u.macurr = u.acurr;
        u.mamin = u.amin;
        u.mamax = u.amax;
        u.mfemale = flags.female;
    } else {
        /* Monster to monster; restore human stats, to be
         * immediately changed to provide stats for the new monster
         */
        u.acurr = u.macurr;
        u.amin = u.mamin;
        u.amax = u.mamax;
        flags.female = u.mfemale;
    }

    /* if stuck mimicking gold, stop immediately */
    if (multi < 0 && U_AP_TYPE == M_AP_OBJECT
        && !is_mimic(youmonst.data))
        unmul("");
    /* if becoming a non-mimic, stop mimicking anything */
    if (!is_mimic(&mons[mntmp])) {
        /* as in polyman() */
        youmonst.m_ap_type = M_AP_NOTHING;
    }
    if (is_male(&mons[mntmp])) {
        if (flags.female)
            dochange = TRUE;
    } else if (is_female(&mons[mntmp])) {
        if (!flags.female)
            dochange = TRUE;
    } else if (!is_neuter(&mons[mntmp]) && mntmp != u.ulycn) {
        if (sex_change_ok && !rn2(10))
            dochange = TRUE;
    }

    Strcpy(buf, (u.umonnum != mntmp) ? "" : "new ");
    if (dochange) {
        flags.female = !flags.female;
        Strcat(buf, (is_male(&mons[mntmp]) || is_female(&mons[mntmp]))
                       ? "" : flags.female ? "female " : "male ");
    }
    play_sfx_sound(SFX_POLYMORPH_SUCCESS);
    Strcat(buf, pm_monster_name(&mons[mntmp], flags.female));
    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s!", (u.umonnum != mntmp) ? "turn into" : "feel like", an(buf));

    if (Stoned && poly_when_stoned(&mons[mntmp])) {
        /* poly_when_stoned already checked stone golem genocide */
        mntmp = PM_STONE_GOLEM;
        make_stoned(0L, "You turn to stone!", 0, (char *) 0, 0);
    }

    u.mtimedone = rn1(500, 500);
    u.umonnum = mntmp;
    set_uasmon();

    /* New stats for monster, to last only as long as polymorphed.
     * Currently only strength gets changed.
     */
    //if (strongmonst(&mons[mntmp]))
    //    ABASE(A_STR) = AMAX(A_STR) = STR18(100);


    ABASE(A_STR) = AMAX(A_STR) = AMIN(A_STR) = (&mons[mntmp])->str; // STR18(100);
    ABASE(A_DEX) = AMAX(A_DEX) = AMIN(A_DEX) = (&mons[mntmp])->dex;
    ABASE(A_CON) = AMAX(A_CON) = AMIN(A_CON) = (&mons[mntmp])->con;
    ABASE(A_INT) = AMAX(A_INT) = AMIN(A_INT) = (&mons[mntmp])->intl;
//    ABASE(A_WIS) = AMAX(A_WIS) = AMIN(A_WIS) = (&mons[mntmp])->wis; // Wisdom does not change 
    ABASE(A_CHA) = AMAX(A_CHA) = AMIN(A_CHA) = (&mons[mntmp])->cha;

    /* These are incorrect in the sense that they give monster race's maximum, not the current undrained maximum
    AMAX(A_STR) = monster_attribute_maximum(&mons[mntmp], A_STR);
    AMAX(A_DEX) = monster_attribute_maximum(&mons[mntmp], A_DEX);
    AMAX(A_CON) = monster_attribute_maximum(&mons[mntmp], A_CON);
    AMAX(A_INT) = monster_attribute_maximum(&mons[mntmp], A_INT);
    AMAX(A_WIS) = monster_attribute_maximum(&mons[mntmp], A_WIS);
    AMAX(A_CHA) = monster_attribute_maximum(&mons[mntmp], A_CHA);
    */

    if (Stone_resistance && Stoned) { /* parnes@eniac.seas.upenn.edu */
        make_stoned(0L, "You no longer seem to be petrifying.", 0,
                    (char *) 0, 0);
    }
    if (Sick_resistance && (Sick || FoodPoisoned || MummyRot)) {
        if(Sick)
            make_sick(0L, (char *) 0, FALSE, 0);
        if (FoodPoisoned)
            make_food_poisoned(0L, (char*)0, FALSE, 0);
        if (MummyRot)
            make_mummy_rotted(0L, (char*)0, FALSE, 0);
        You_ex(ATR_NONE, CLR_MSG_POSITIVE, "no longer feel sick.");
    }
    if (Slimed) {
        if (flaming(youmonst.data)) {
            make_slimed(0L, "The slime burns away!", 0, (char*)0, 0);
        } else if (mntmp == PM_GREEN_SLIME) {
            /* do it silently */
            make_slimed(0L, (char *) 0, 0, (char*)0, 0);
        }
    }
    check_strangling(FALSE); /* maybe stop strangling */
    if (nohands(youmonst.data))
        Glib = 0;

    /*
    mlvl = adj_lev(&mons[mntmp]);
     * We can't do the above, since there's no such thing as an
     * "experience level of you as a monster" for a polymorphed character.
     */
    mlvl = (int) mons[mntmp].mlevel;
    int hp = 1;
    if (youmonst.data->mlet == S_DRAGON && mntmp >= PM_GRAY_DRAGON) 
    {
        hp = (int)((In_endgame(&u.uz) ? (8.0 * (double)mlvl) : ((double)d(mlvl, 8))) + constitution_hp_bonus(ACURR(A_CON)) * (double)mlvl);
//    } else if (is_golem(youmonst.data)) {
//        hp = golemhp(mntmp);
    } 
    else
    {
        if (!mlvl)
        {
            hp = rnd(4) + (int)(constitution_hp_bonus(ACURR(A_CON)) / 2.0);
        }
        else
            hp = d(mlvl, 8) + (int)(constitution_hp_bonus(ACURR(A_CON)) * (double)mlvl);
        /*
        if (is_home_elemental(&mons[mntmp]))
            hp *= 2;
         */
    }
    if (hp < 1)
        hp = 1;
    u.mhmax = hp;
    u.basemhmax = hp;
    u.mh = u.mhmax;
    updatemaxhp();

    if (u.ulevel < mlvl) {
        /* Low level characters can't become high level monsters for long */
#ifdef DUMB
        /* DRS/NS 2.2.6 messes up -- Peter Kendell */
        int mtd = u.mtimedone, ulv = u.ulevel;

        u.mtimedone = mtd * ulv / mlvl;
#else
        u.mtimedone = u.mtimedone * u.ulevel / mlvl;
#endif
    }

    if (uskin && mntmp != armor_to_dragon(uskin->otyp))
        skinback(FALSE);
    break_armor();
    drop_weapon(1);
    (void) hideunder(&youmonst);

    if (u.utrap && u.utraptype == TT_PIT) {
        set_utrap(rn1(6, 2), TT_PIT); /* time to escape resets */
    }
    if (was_blind && !Blind) { /* previous form was eyeless */
        Blinded = 1L;
        make_blinded(0L, TRUE); /* remove blindness */
    }
    newsym(u.ux, u.uy); /* Change symbol */

    /* [note:  this 'sticky' handling is only sufficient for changing from
       grabber to engulfer or vice versa because engulfing by poly'd hero
       always ends immediately so won't be in effect during a polymorph] */
    if (!sticky && !u.uswallow && u.ustuck && sticks(youmonst.data))
        u.ustuck = 0;
    else if (sticky && !sticks(youmonst.data))
        uunstick();

    if (u.usteed) {
        if (touch_petrifies(u.usteed->data) && !Stone_resistance && rnl(3)) {
            pline("%s touch %s.", no_longer_petrify_resistant,
                  mon_nam(u.usteed));
            Sprintf(buf, "riding %s", an(mon_monster_name(u.usteed)));
            killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
            instapetrify(buf);
        }
        if (!can_ride(u.usteed))
            dismount_steed(DISMOUNT_POLY);
    }

    /*
    if (flags.verbose) {
        static const char use_thec[] = "Use the command #%s to %s.";
        static const char monsterc[] = "monster";

        if (can_breathe(youmonst.data))
            pline(use_thec, monsterc, "use your breath weapon");
        if (attacktype(youmonst.data, AT_SPIT))
            pline(use_thec, monsterc, "spit venom");
        if (youmonst.data->mlet == S_NYMPH)
            pline(use_thec, monsterc, "remove an iron ball");
        if (attacktype(youmonst.data, AT_GAZE))
            pline(use_thec, monsterc, "gaze at monsters");
        if (is_hider(youmonst.data))
            pline(use_thec, monsterc, "hide");
        if (is_were(youmonst.data))
            pline(use_thec, monsterc, "summon help");
        if (webmaker(youmonst.data))
            pline(use_thec, monsterc, "spin a web");
        if (u.umonnum == PM_GREMLIN)
            pline(use_thec, monsterc, "multiply in a fountain");
        if (is_unicorn(youmonst.data))
            pline(use_thec, monsterc, "use your horn");
        if (is_tentacled_one(youmonst.data))
            pline(use_thec, monsterc, "emit a mental blast");
        if (youmonst.data->msound == MS_SHRIEK)
            pline(use_thec, monsterc, "shriek");
        if (is_vampire(youmonst.data))
            pline(use_thec, monsterc, "change shape");

        if (lays_eggs(youmonst.data) && flags.female &&
            !(youmonst.data == &mons[PM_GIANT_EEL]
                || youmonst.data == &mons[PM_ELECTRIC_EEL]))
            pline(use_thec, "sit",
                  eggs_in_water(youmonst.data) ?
                      "spawn in the water" : "lay an egg");
    }
    */
    /* you now know what an egg of your type looks like */
    if (lays_eggs(youmonst.data)) {
        learn_egg_type(u.umonnum);
        /* make queen bees recognize killer bee eggs */
        learn_egg_type(egg_type_from_parent(u.umonnum, TRUE));
    }
    find_ac();
    find_mc();
    if ((!Levitation && !u.ustuck && !Flying && is_pool_or_lava(u.ux, u.uy))
        || (Underwater && !Swimming))
        spoteffects(TRUE);
    if (Passes_walls && u.utrap
        && (u.utraptype == TT_INFLOOR || u.utraptype == TT_BURIEDBALL)) {
        if (u.utraptype == TT_INFLOOR) {
            pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "rock seems to no longer trap you.");
        } else {
            pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "buried ball is no longer bound to you.");
            buried_ball_to_freedom();
        }
        reset_utrap(TRUE);
    } else if (likes_lava(youmonst.data) && u.utrap
               && u.utraptype == TT_LAVA) {
        pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s now feels soothing.", hliquid("lava"));
        reset_utrap(TRUE);
    }
    if (amorphous(youmonst.data) || is_whirly(youmonst.data)
        || unsolid(youmonst.data) || is_incorporeal(youmonst.data)) {
        if (Punished) {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "slip out of the iron chain.");
            unpunish();
        } else if (u.utrap && u.utraptype == TT_BURIEDBALL) {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "slip free of the buried ball and chain.");
            buried_ball_to_freedom();
        }
    }
    if (u.utrap && (u.utraptype == TT_WEB || u.utraptype == TT_BEARTRAP)
        && (amorphous(youmonst.data) || is_whirly(youmonst.data)
            || unsolid(youmonst.data) || is_incorporeal(youmonst.data) || (youmonst.data->msize <= MZ_SMALL
                                          && u.utraptype == TT_BEARTRAP))) {
        You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are no longer stuck in the %s.",
            u.utraptype == TT_WEB ? "web" : "bear trap");
        /* probably should burn webs too if PM_FIRE_ELEMENTAL */
        reset_utrap(TRUE);
    }
    if (webmaker(youmonst.data) && u.utrap && u.utraptype == TT_WEB) {
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "orient yourself on the web.");
        reset_utrap(TRUE);
    }
    check_strangling(TRUE); /* maybe start strangling */

    context.botl = context.botlx = TRUE;
    vision_full_recalc = 1;
    see_monsters();
    (void) encumber_msg();

    retouch_equipment(2);
    /* this might trigger a recursive call to polymon() [stone golem
       wielding cockatrice corpse and hit by stone-to-flesh, becomes
       flesh golem above, now gets transformed back into stone golem] */
    if (!uarmg)
        selftouch(no_longer_petrify_resistant);
    return 1;
}

int
monster_attribute_maximum(monster_class_ptr, attribute_index)
struct permonst *monster_class_ptr;
int attribute_index;
{
    if (!monster_class_ptr)
        return 0;

    if (attribute_index < A_STR || attribute_index > A_CHA)
        return 0;

    int monster_attribute_score = 0;
    switch (attribute_index) {
    case A_STR:
        monster_attribute_score = monster_class_ptr->str;
        break;
    case A_DEX:
        monster_attribute_score = monster_class_ptr->dex;
        break;
    case A_CON:
        monster_attribute_score = monster_class_ptr->con;
        break;
    case A_INT:
        monster_attribute_score = monster_class_ptr->intl;
        break;
    case A_WIS:
        monster_attribute_score = monster_class_ptr->wis;
        break;
    case A_CHA:
        monster_attribute_score = monster_class_ptr->cha;
        break;
    }
    if (attribute_index == A_STR)
    {
        if (monster_attribute_score > STR18(100))
            return STR19(25);
        else if (monster_attribute_score > 18)
            return STR18(100);
        else if (monster_attribute_score >= 3)
            return 18;
        else
            return monster_attribute_score;
    }
    else
    {
        if (monster_attribute_score > 18)
            return 25;
        else if (monster_attribute_score >= 3)
            return 18;
        else
            return monster_attribute_score;
    }

    
}



int
monster_attribute_minimum(monster_class_ptr, attribute_index)
struct permonst* monster_class_ptr;
int attribute_index;
{
    if (!monster_class_ptr)
        return 0;

    if (attribute_index < A_STR || attribute_index > A_CHA)
        return 0;

    int monster_attribute_score = 0;
    switch (attribute_index) {
    case A_STR:
        monster_attribute_score = monster_class_ptr->str;
        break;
    case A_DEX:
        monster_attribute_score = monster_class_ptr->dex;
        break;
    case A_CON:
        monster_attribute_score = monster_class_ptr->con;
        break;
    case A_INT:
        monster_attribute_score = monster_class_ptr->intl;
        break;
    case A_WIS:
        monster_attribute_score = monster_class_ptr->wis;
        break;
    case A_CHA:
        monster_attribute_score = monster_class_ptr->cha;
        break;
    }
    if (monster_attribute_score <= 3)
        return monster_attribute_score;
    else
        return 3;


}




STATIC_OVL void
break_armor()
{
    register struct obj *otmp;

    //Suit, cloak, robe, shirt
    if (breakarm(youmonst.data)) 
    {
        if ((otmp = uarm) != 0)
        {
            if (donning(otmp))
                cancel_don();

            if (otmp->oartifact || is_obj_indestructible(otmp))
            {
                /* Luckily, you do not die, just the armor pops off, so having an indestructible armor is not life-threatening --JG */
                pline("%s falls off!", Yname2(otmp));
                (void)Armor_off();
                dropx(otmp);
            }
            else
            {
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_BREAK);
                You("break out of %s!", yname(otmp));
                exercise(A_STR, FALSE);
                (void)Armor_gone();
                useup(otmp);
            }
        }
        if ((otmp = uarmc) != 0) 
        {
            if (donning(otmp))
                cancel_don();

            if (otmp->oartifact || is_obj_indestructible(otmp))
            {
                Your("%s falls off!", cloak_simple_name(otmp));
                (void) Cloak_off();
                dropx(otmp);
            } 
            else 
            {
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_BREAK);
                Your("%s tears apart!", cloak_simple_name(otmp));
                (void) Cloak_off();
                useup(otmp);
            }
        }
        if ((otmp = uarmo) != 0)
        {
            if (donning(otmp))
                cancel_don();

            if (otmp->oartifact || is_obj_indestructible(otmp))
            {
                /* Not sure how this happens but at least it is not life-threatening */
                Your("%s falls off!", robe_simple_name(otmp));
                (void)Robe_off();
                dropx(otmp);
            }
            else
            {
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_BREAK);
                Your("%s in torn to pieces!", robe_simple_name(otmp));
                (void)Robe_off();
                useup(otmp);
            }
        }
        if ((otmp = uarmu) != 0)
        {
            if (donning(otmp))
                cancel_don();

            if (otmp->oartifact || is_obj_indestructible(otmp))
            {
                /* Not sure how this happens but at least it is not life-threatening */
                Your("shirt falls off!");
                (void)Shirt_off();
                dropx(otmp);
            }
            else
            {
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_BREAK);
                Your("shirt rips to shreds!");
                (void)Shirt_off();
                useup(otmp);
            }
        }
    } 
    else if (sliparm(youmonst.data)) 
    {
        if (((otmp = uarm) != 0) && (racial_exception(&youmonst, otmp) < 1))
        {
            if (donning(otmp))
                cancel_don();
            Your("armor falls around you!");
            (void) Armor_gone();
            dropx(otmp);
        }
        if ((otmp = uarmo) != 0)
        {
            if (is_whirly(youmonst.data))
                Your("%s falls, unsupported!", robe_simple_name(otmp));
            else
                You("shrink out of your %s!", robe_simple_name(otmp));
            (void)Robe_off();
            dropx(otmp);
        }
        if ((otmp = uarmc) != 0) {
            if (is_whirly(youmonst.data))
                Your("%s falls, unsupported!", cloak_simple_name(otmp));
            else
                You("shrink out of your %s!", cloak_simple_name(otmp));
            (void) Cloak_off();
            dropx(otmp);
        }
        if ((otmp = uarmu) != 0) 
        {
            if (is_whirly(youmonst.data))
                You("seep right through your shirt!");
            else
                You("become much too small for your shirt!");
            setworn((struct obj *) 0, otmp->owornmask & W_ARMU);
            dropx(otmp);
        }
    }

    //Helmet
    if (has_horns(youmonst.data) || !has_place_to_put_helmet_on(youmonst.data))
    {
        if ((otmp = uarmh) != 0) 
        {
            if (is_flimsy(otmp) && !donning(otmp) && has_place_to_put_helmet_on(youmonst.data))
            {
                char hornbuf[BUFSZ];

                /* Future possibilities: This could damage/destroy helmet */
                Sprintf(hornbuf, "horn%s", plur(num_horns(youmonst.data)));
                Your("%s %s through %s.", hornbuf, vtense(hornbuf, "pierce"),
                     yname(otmp));
            }
            else 
            {
                if (donning(otmp))
                    cancel_don();
                Your("%s falls to the %s!", helm_simple_name(otmp),
                     surface(u.ux, u.uy));
                (void) Helmet_off();
                dropx(otmp);
            }
        }
    }

    //Gloves and weapons
    if (nohands(youmonst.data) || nolimbs(youmonst.data) || verysmall(youmonst.data))
    {
        if ((otmp = uarmg) != 0) 
        {
            if (donning(otmp))
                cancel_don();
            /* Drop weapon along with gloves */
            You("drop your gloves%s!", uwep ? " and weapon" : "");
            drop_weapon(0);
            (void) Gloves_off();
            dropx(otmp);
        }
        if ((otmp = uarms) != 0)
        {
            if(is_shield(otmp))
            {
                You("can no longer hold your shield!");
                (void) Shield_off();
            }
            else
            {
                You("can no longer hold your %s!", cxname(otmp));
                remove_worn_item(otmp, FALSE);
            }
            dropx(otmp);
        }
        if ((otmp = uarmh) != 0) 
        {
            if (donning(otmp))
                cancel_don();
            Your("%s falls to the %s!", helm_simple_name(otmp),
                 surface(u.ux, u.uy));
            (void) Helmet_off();
            dropx(otmp);
        }
    }

    //Boots
    if (nohands(youmonst.data) || nolimbs(youmonst.data) || verysmall(youmonst.data)
        || slithy(youmonst.data) || youmonst.data->mlet == S_CENTAUR)
    {
        if ((otmp = uarmf) != 0)
        {
            if (donning(otmp))
                cancel_don();
            if (is_whirly(youmonst.data))
                Your("boots fall away!");
            else
                Your("boots %s off your feet!",
                     verysmall(youmonst.data) ? "slide" : "are pushed");
            (void) Boots_off();
            dropx(otmp);
        }
    }

    //Bracers
    if (nohands(youmonst.data) || nolimbs(youmonst.data) || verysmall(youmonst.data))
    {
        if ((otmp = uarmb) != 0) {
            if (donning(otmp))
                cancel_don();
            if (is_whirly(youmonst.data))
                Your("bracers fall away!");
            else
                Your("bracers %s off your arms!",
                    verysmall(youmonst.data) ? "slide" : "are pushed");
            (void)Bracers_off();
            dropx(otmp);
        }
    }

}

STATIC_OVL void
drop_weapon(alone)
int alone;
{
    struct obj *otmp;
    const char *what, *which, *whichtoo;
    boolean candropwep, candropwep2, updateinv = TRUE;

    if (uwep) {
        /* !alone check below is currently superfluous but in the
         * future it might not be so if there are monsters which cannot
         * wear gloves but can wield weapons
         */
        if (!alone || cantwield(youmonst.data)) {
            candropwep = canletgo(uwep, "");
            candropwep2 = uarms && canletgo(uarms, "");
            if (alone) {
                what = (candropwep && candropwep2) ? "drop" : "release";
                which = is_sword(uwep) ? "sword" : weapon_descr(uwep);
                if (u.twoweap && uarms) {
                    whichtoo = is_shield(uarms) ? "shield" :
                        is_sword(uarms) ? "sword" : weapon_descr(uarms);
                    if (strcmp(which, whichtoo))
                        which = "weapon";
                }
                if (uwep->quan != 1L || (u.twoweap && uwep && uarms))
                    which = makeplural(which);

                You("find you must %s %s %s!", what,
                    the_your[!!strncmp(which, "corpse", 6)], which);
            }
            /* if either uwep or wielded uswapwep is flagged as 'in_use'
               then don't drop it or explicitly update inventory; leave
               those actions to caller (or caller's caller, &c) */
            if (u.twoweap) {
                otmp = uarms;
                uwep2gone();
                if (otmp && otmp->in_use)
                    updateinv = FALSE;
                else if (candropwep2)
                    dropx(otmp);
            }
            otmp = uwep;
            uwepgone();
            if (otmp->in_use)
                updateinv = FALSE;
            else if (candropwep)
                dropx(otmp);

            if (updateinv)
                update_inventory();
        } /*else if (!could_twoweap(youmonst.data)) {
            untwoweapon();
        }*/
    }
}

void
rehumanize()
{
    boolean was_flying = (Flying != 0);

    /* You can't revert back while unchanging */
    if (Unchanging) {
        if (u.mh < 1) {
            killer.format = NO_KILLER_PREFIX;
            Strcpy(killer.name, "killed while stuck in creature form");
            done(DIED);
        } else if (uamul && uamul->otyp == AMULET_OF_UNCHANGING) {
            Your("%s %s!", simpleonames(uamul), otense(uamul, "fail"));
            uamul->dknown = 1;
            makeknown(AMULET_OF_UNCHANGING);
        }
    }

    if (emitted_light_range(youmonst.data))
        del_light_source(LS_MONSTER, monst_to_any(&youmonst));
    if (mon_ambient_sound(youmonst.data))
        del_sound_source(SOUNDSOURCE_MONSTER, monst_to_any(&youmonst));

    play_sfx_sound(SFX_POLYMORPH_SUCCESS);
    polyman("return to %s form!", urace.adj);

    if (u.uhp < 1) {
        /* can only happen if some bit of code reduces u.uhp
           instead of u.mh while poly'd */
        Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "old form was not healthy enough to survive.");
        Sprintf(killer.name, "reverting to unhealthy %s form", urace.adj);
        killer.format = KILLED_BY;
        done(DIED);
    }
    nomul(0);

    context.botl = context.botlx = TRUE;
    vision_full_recalc = 1;
    (void) encumber_msg();
    if (was_flying && !Flying && u.usteed)
        You("and %s return gently to the %s.",
            mon_nam(u.usteed), surface(u.ux, u.uy));
    retouch_equipment(2);
    if (!uarmg)
        selftouch(no_longer_petrify_resistant);
}

int
dobreathe()
{
    struct attack *mattk;

    if (Strangled) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "breathe.  Sorry.");
        return 0;
    }
    if (u.uen < BREATH_WEAPON_MANA_COST)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MANA);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough energy to breathe!");
        return 0;
    }
    u.uen -= BREATH_WEAPON_MANA_COST;
    context.botl = 1;

    if (!getdir((char *) 0))
        return 0;

    mattk = attacktype_fordmg(youmonst.data, AT_BREA, AD_ANY);

    if (!mattk)
        impossible("bad breath attack?"); /* mouthwash needed... */
    else if (!u.dx && !u.dy && !u.dz)
        ubreatheu(mattk);
    else
    {
        update_u_facing(TRUE);

        uchar adtyp = mattk->adtyp;
        int typ = get_ray_adtyp(adtyp);

        buzz((20 + typ - 1), (struct obj*)0, &youmonst, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp, u.ux, u.uy,
            u.dx, u.dy);

    }
    return 1;
}

int
get_ray_adtyp(adtyp)
uchar adtyp;
{
    int rbgd_effect_choices[2] = { AD_FIRE, AD_DRST };
    int rbpd_effect_choices[2] = { AD_COLD, AD_DISN };
    int ray1_effect_choices[3] = { AD_DISN, AD_ELEC, AD_COLD }; /* Elemental */
    int ray2_effect_choices[3] = { AD_DRAY, AD_STON, AD_SLEE }; /* Magic */
    int typ = (adtyp == AD_RBRE) ? rnd(AD_ACID) :
        (adtyp == AD_REY1) ? ray1_effect_choices[rn2(3)] :
        (adtyp == AD_REY2) ? ray2_effect_choices[rn2(3)] :
        (adtyp == AD_RBGD) ? rbgd_effect_choices[rn2(2)] :
        (adtyp == AD_RBPD) ? rbpd_effect_choices[rn2(2)] :
        adtyp;

    return typ;
}

int
dosteedbreathe()
{
    struct attack* mattk;

    if (!u.usteed || !can_breathe(u.usteed->data))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "have no steed that use a breath weapon!");
        return 0;
    }

    if (u.usteed->mspec_used > 0)
    {
        play_sfx_sound(SFX_NOT_READY_YET);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s breath weapon is not ready yet.", s_suffix(Monnam(u.usteed)));
        return 0;
    }

    if (!getdir((char*)0))
        return 0;
    
    if (!u.dx && !u.dy && !u.dz)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "steed cannot breathe at you!");
        return 0;
    }

    mattk = attacktype_fordmg(u.usteed->data, AT_BREA, AD_ANY);
    if (!mattk)
        impossible("bad breath attack?"); /* mouthwash needed... */
    else
    {
        update_u_facing(FALSE);
        update_m_facing(u.usteed, u.dx, TRUE);

        int typ = get_ray_adtyp(mattk->adtyp);

        buzz((int)(-(20 + typ - 1)), (struct obj*)0, u.usteed, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp, u.ux, u.uy, u.dx, u.dy);

        u.usteed->mspec_used = (5 + rn2(10)) / mon_spec_cooldown_divisor(u.usteed);
        if (typ == AD_SLEE && !Sleep_resistance)
            u.usteed->mspec_used += rnd(20) / mon_spec_cooldown_divisor(u.usteed);

    }
    return 1;
}


int
dospit()
{
    struct obj *otmp;
    struct attack *mattk;

    if (!getdir((char *) 0))
        return 0;
    mattk = attacktype_fordmg(youmonst.data, AT_SPIT, AD_ANY);
    if (!mattk) {
        impossible("bad spit attack?");
    } else {
        update_u_facing(TRUE);

        switch (mattk->adtyp) {
        case AD_BLND:
        case AD_DRST:
            otmp = mksobj(BLINDING_VENOM, TRUE, FALSE, FALSE);
            break;
        default:
            impossible("bad attack type in dospit");
            /*FALLTHRU*/
        case AD_ACID:
            otmp = mksobj(ACID_VENOM, TRUE, FALSE, FALSE);
            break;
        }
        otmp->speflags |= SPEFLAGS_YOURS; /* to indicate it's yours */
        throwit(otmp, 0L);
    }
    return 1;
}

int
doremove()
{
    if (!Punished) {
        if (u.utrap && u.utraptype == TT_BURIEDBALL) {
            pline_The("ball and chain are buried firmly in the %s.",
                      surface(u.ux, u.uy));
            return 0;
        }
        You("are not chained to anything!");
        return 0;
    }
    unpunish();
    return 1;
}

int
dospinweb()
{
    register struct trap *ttmp = t_at(u.ux, u.uy);

    if (Levitation || Is_airlevel(&u.uz) || Underwater
        || Is_waterlevel(&u.uz)) {
        You("must be on the ground to spin a web.");
        return 0;
    }
    if (u.uswallow) {
        You("release web fluid inside %s.", mon_nam(u.ustuck));
        if (is_animal(u.ustuck->data)) {
            expels(u.ustuck, u.ustuck->data, TRUE);
            return 0;
        }
        if (is_whirly(u.ustuck->data)) {
            int i;

            for (i = 0; i < NATTK; i++)
                if (u.ustuck->data->mattk[i].aatyp == AT_ENGL)
                    break;
            if (i == NATTK)
                impossible("Swallower has no engulfing attack?");
            else {
                char sweep[30];

                sweep[0] = '\0';
                switch (u.ustuck->data->mattk[i].adtyp) {
                case AD_FIRE:
                    Strcpy(sweep, "ignites and ");
                    break;
                case AD_ELEC:
                    Strcpy(sweep, "fries and ");
                    break;
                case AD_COLD:
                    Strcpy(sweep, "freezes, shatters and ");
                    break;
                }
                pline_The("web %sis swept away!", sweep);
            }
            return 0;
        } /* default: a nasty jelly-like creature */
        pline_The("web dissolves into %s.", mon_nam(u.ustuck));
        return 0;
    }
    if (u.utrap) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot spin webs while stuck in a trap.");
        return 0;
    }
    exercise(A_DEX, TRUE);
    if (ttmp) {
        if(trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_NOT_OVERRIDEN)
        {
            Your("webbing vanishes!");
            return 0;
        }
        else
        {
            switch (ttmp->ttyp) {
            case PIT:
            case SPIKED_PIT:
                You("spin a web, covering up the pit.");
                deltrap(ttmp);
                bury_objs(u.ux, u.uy);
                newsym(u.ux, u.uy);
                return 1;
            case SQKY_BOARD:
                pline_The("squeaky board is muffled.");
                deltrap(ttmp);
                newsym(u.ux, u.uy);
                return 1;
            case TELEP_TRAP:
            case LEVEL_TELEP:
                Your("webbing vanishes!");
                return 0;
            case WEB:
                You("make the web thicker.");
                return 1;
            case HOLE:
            case TRAPDOOR:
                You("web over the %s.", trap_type_definitions[ttmp->ttyp].type_name);
                deltrap(ttmp);
                newsym(u.ux, u.uy);
                return 1;
            case ROLLING_BOULDER_TRAP:
                You("spin a web, jamming the trigger.");
                deltrap(ttmp);
                newsym(u.ux, u.uy);
                return 1;
            case ARROW_TRAP:
            case DART_TRAP:
            case BEAR_TRAP:
            case ROCKTRAP:
            case FIRE_TRAP:
            case LANDMINE:
            case SLP_GAS_TRAP:
            case RUST_TRAP:
            case MAGIC_TRAP:
            case ANTI_MAGIC_TRAP:
            case POLY_TRAP:
                You("have triggered a trap!");
                dotrap(ttmp, 0);
                return 1;
            default:
                impossible("Webbing over trap type %d?", ttmp->ttyp);
                return 0;
            }
        }
    } else if (On_stairs(u.ux, u.uy)) {
        /* cop out: don't let them hide the stairs */
        Your("web fails to impede access to the %s.",
             (levl[u.ux][u.uy].typ == STAIRS) ? "stairs" : "ladder");
        return 1;
    }
    ttmp = maketrap(u.ux, u.uy, WEB, NON_PM, MKTRAP_NO_FLAGS);
    if (ttmp) {
        ttmp->madeby_u = 1;
        feeltrap(ttmp);
    }
    return 1;
}

int
dosummon()
{
    int placeholder;
    if (u.uen < WERE_SUMMON_MANA_COST) {
        You_ex(ATR_NONE, CLR_MSG_FAIL, "lack the energy to send forth a call for help!");
        return 0;
    }
    u.uen -= WERE_SUMMON_MANA_COST;
    context.botl = 1;

    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "call upon your brethren for help!");
    exercise(A_WIS, TRUE);
    if (!were_summon(youmonst.data, TRUE, &placeholder, (char *) 0))
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "But none arrive.");
    return 1;
}

int
dogaze()
{
    register struct monst *mtmp;
    int looked = 0;
    char qbuf[QBUFSZ];
    int i;
    uchar adtyp = 0;

    for (i = 0; i < NATTK; i++) {
        if (youmonst.data->mattk[i].aatyp == AT_GAZE) {
            adtyp = youmonst.data->mattk[i].adtyp;
            break;
        }
    }

    if (adtyp != AD_CONF && adtyp != AD_FIRE && adtyp != AD_CNCL) 
    {
        impossible("gaze attack %d?", adtyp);
        return 0;
    }

    int gazemanacost = (adtyp == AD_CNCL ? CNCL_GAZE_MANA_COST : GAZE_MANA_COST);

    if (Blind) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "see anything to gaze at.");
        return 0;
    } else if (Hallucination) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "gaze at anything you can see.");
        return 0;
    }
    if (u.uen < gazemanacost) {
        play_sfx_sound(SFX_NOT_ENOUGH_MANA);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "lack the energy to use your special gaze!");
        return 0;
    }

    if (!getdir((char*)0))
        return 0;

    if (!u.dx && !u.dy && !u.dz)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "gaze at yourself.");
        return 0;
    }
    else if (!u.dx && !u.dy && u.dz > 0)
    {
        You("gaze at the floor. Nothing else happens.");
        return 0;
    }
    else if (!u.dx && !u.dy && u.dz < 0)
    {
        You("gaze at the ceiling. Nothing else happens.");
        return 0;
    }

    u.uen -= gazemanacost;
    context.botl = 1;

    int maxgazerange = 18;
    int cx = u.ux + u.dx, cy = u.uy + u.dy;

    update_u_facing(TRUE);

    while (isok(cx, cy) && couldsee(cx, cy) && dist2(u.ux, u.uy, cx, cy) <= maxgazerange * maxgazerange) 
    {
        if (MON_AT(cx, cy))
        {
            mtmp = m_at(cx, cy);
            if (mtmp && !DEADMONSTER(mtmp) && canseemon(mtmp))
            {
                looked++;
                if (Invis && !has_see_invisible(mtmp))
                {
                    pline("%s seems not to notice your gaze.", Monnam(mtmp));
                }
                else if (is_invisible(mtmp) && !See_invisible)
                {
                    You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "see where to gaze at %s.", Monnam(mtmp));
                }
                else if (M_AP_TYPE(mtmp) == M_AP_FURNITURE
                           || M_AP_TYPE(mtmp) == M_AP_OBJECT)
                {
                    looked--;
                }
/*                else if (flags.safe_dog && is_tame(mtmp) && !Confusion) //Unnecessary now that gazing has been changed to be directional
                {
                    You("avoid gazing at %s.", y_monnam(mtmp));
                } 
*/                else 
                {
                    if (flags.confirm && is_peaceful(mtmp) && !Confusion) 
                    {
                        Sprintf(qbuf, "Really %s %s?",
                                (adtyp == AD_CONF) ? "confuse" : "gaze at",
                                mon_nam(mtmp));
                        if (yn_query(qbuf) != 'y')
                            break;
                    }
                    setmangry(mtmp, TRUE);
                    /* No reflection check for consistency with when a monster
                     * gazes at *you*--only medusa gaze gets reflected then.
                     */
                    switch (adtyp)
                    {
                    case AD_CONF:
                    {
                        if (!mon_can_move(mtmp) || is_stunned(mtmp)
                            || is_blinded(mtmp) || !haseyes(mtmp->data))
                        {
                            Your("gaze has no effect on %s!", mon_nam(mtmp));
                            break;
                        }
                        play_sfx_sound_at_location(SFX_ACQUIRE_CONFUSION, mtmp->mx, mtmp->my);
                        if (!is_confused(mtmp))
                            Your("gaze confuses %s!", mon_nam(mtmp));
                        else
                            pline("%s is getting more and more confused.",
                                Monnam(mtmp));
                        increase_mon_property(mtmp, CONFUSION, 20 + rnd(10));
                        break;
                    }
                    case AD_FIRE:
                    {
                        if (!mon_can_move(mtmp) || is_stunned(mtmp)
                            || is_blinded(mtmp) || !haseyes(mtmp->data))
                        {
                            Your("gaze has no effect on %s!", mon_nam(mtmp));
                            break;
                        }
                        int dmg = d(2, 6), lev = (int)u.ulevel;

                        You("attack %s with a fiery gaze!", mon_nam(mtmp));

                        if (is_mon_immune_to_fire(mtmp)) 
                        {
                            pline_The("fire doesn't burn %s!", mon_nam(mtmp));
                            dmg = 0;
                        }

                        if (lev > rn2(20))
                            (void)destroy_mitem(mtmp, SCROLL_CLASS, AD_FIRE);
                        if (lev > rn2(20))
                            (void)destroy_mitem(mtmp, POTION_CLASS, AD_FIRE);
                        if (lev > rn2(25))
                            (void)destroy_mitem(mtmp, SPBOOK_CLASS, AD_FIRE);

                        int hp_before = mtmp->mhp;
                        if (dmg)
                            deduct_monster_hp(mtmp, adjust_damage(dmg, &youmonst, mtmp, adtyp, ADFLAGS_NONE));
                        int hp_after = mtmp->mhp;
                        int damage_dealt = hp_before - hp_after;
                        pline("%s sustains %d damage!", Monnam(mtmp), damage_dealt);
                        display_m_being_hit(mtmp, HIT_ON_FIRE, damage_dealt, 0UL, FALSE);

                        if (DEADMONSTER(mtmp))
                            killed(mtmp);
                        break;
                    }
                    case AD_CNCL:
                    {
                        if (Cancelled)
                        {
                            You("gaze at %s but without effect.", mon_nam(mtmp));
                            break;
                        }
                        else if (Blind || (is_invisible(mtmp) && !See_invisible))
                        {
                            You("stare blindly at %s general direction.", s_suffix(mon_nam(mtmp)));
                            break;
                        }
#if 0
                        else if (is_reflecting(mtmp))
                        {
                            You("gaze at %s.", mon_nam(mtmp));
                            play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mtmp->mx, mtmp->my);
                            (void)mon_reflects(mtmp, "The gaze is reflected away by %s %s!");
                            break;
                        }
#endif
                        else
                        {
                            if (is_cancelled(mtmp))
                                You("gaze at %s. %s is hit by an invisible anti-magic ray!", mon_nam(mtmp), Monnam(mtmp));
                            else
                                You("focus your anti-magic gaze on %s.", mon_nam(mtmp));
                            if (has_cancellation_resistance(mtmp))
                            {
                                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                                pline("However, %s is unaffected!", mon_nam(mtmp));
                                m_shieldeff(mtmp);

                            }
                            else
                            {
                                play_sfx_sound_at_location(SFX_ACQUIRE_CANCELLATION, mtmp->mx, mtmp->my);
                                nonadditive_increase_mon_property_verbosely(mtmp, CANCELLED, d(2, 4));
                                nonadditive_increase_mon_property_verbosely(mtmp, CANCELLATION_RESISTANCE, 10);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                    }

                    /* For consistency with passive() in uhitm.c, this only
                     * affects you if the monster is still alive.
                     */
                    if (!DEADMONSTER(mtmp))
                    {
                        if (mtmp->data == &mons[PM_FLOATING_EYE] && !is_cancelled(mtmp)) 
                        {
                            if (!Free_action) {
                                play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
                                You("are frozen by %s gaze!",
                                    s_suffix(mon_nam(mtmp)));
                                nomul((u.ulevel > 6 || rn2(4))
                                    ? -d((int)mtmp->m_lev + 1,
                                    (int)mtmp->data->mattk[0].damd)
                                    : -200);
                                multi_reason = "frozen by a monster's gaze";
                                nomovemsg = 0;
                                standard_hint("Do not gaze at floating eyes unless you have free action. Use ranged weapons against them.", &u.uhint.paralyzed_by_floating_eye);
                                return 1;
                            }
                            else
                                You("stiffen momentarily under %s gaze.",
                                    s_suffix(mon_nam(mtmp)));
                        }
                        /* Technically this one shouldn't affect you at all because
                         * the Medusa gaze is an active monster attack that only
                         * works on the monster's turn, but for it to *not* have an
                         * effect would be too weird.
                         */
                        if (mtmp->data == &mons[PM_MEDUSA] && !is_cancelled(mtmp)) 
                        {
                            pline("Gazing at the awake %s is not a very good idea.",
                                l_monnam(mtmp));
                            /* as if gazing at a sleeping anything is fruitful... */
                            play_sfx_sound(SFX_PETRIFY);
                            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "turn to stone...");
                            killer.format = KILLED_BY;
                            Strcpy(killer.name, "deliberately meeting Medusa's gaze");
                            done(STONING);
                        }
                    }
                }
                /* Gaze hits only one monster */
                break;
            }
        }

        /* Add range */
        cx = cx + u.dx;
        cy = cy + u.dy;
    }
    if (!looked)
        You("gaze at no place in particular.");
    return 1;
}

int
doeyestalk()
{
    if (Blind) {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "see anything to gaze at.");
        return 0;
    }
    else if (Cancelled) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are cancelled and cannot use your eyestalks!");
        return 0;
    }
    else if (Hallucination) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "gaze at anything you can see.");
        return 0;
    }

    if (!getdir((char*)0))
        return 0;

    if (!u.dx && !u.dy && !u.dz)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "gaze at yourself.");
        return 0;
    }

    update_u_facing(TRUE);

    int attacksperformed = 0;
    int i;
    for (i = 0; i < NATTK; i++)
    {
        struct attack* mattk = &youmonst.data->mattk[i];

        if (!mattk)
            continue;

        if (mattk->aatyp != AT_EYES)
            continue;

        if (u.uen < EYE_STALK_MANA_COST) {
            play_sfx_sound(SFX_NOT_ENOUGH_MANA);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "lack the energy to use your eyestalks%s!", attacksperformed > 0 ? " any further" : "");
            return (attacksperformed > 0 ? 1 : 0);
        }
        u.uen -= EYE_STALK_MANA_COST;
        context.botl = 1;

        int typ = get_ray_adtyp(mattk->adtyp); 

        if ((typ >= AD_MAGM) && (typ <= AD_STON))
        {
            pline("One of your eyestalks fires %s!", get_eyestalk_ray_name(typ));
            buzz((int)(30 + (typ - 1)), (struct obj*)0, &youmonst, (int)mattk->damn, (int)mattk->damd, (int)mattk->damp,
                u.ux, u.uy, u.dx, u.dy);
            attacksperformed++;
        }
        else
            impossible("Eyestalk %d used", typ - 1);

    }

    return 1;
}

int
dohide()
{
    boolean ismimic = is_mimic(youmonst.data),
            on_ceiling = is_clinger(youmonst.data) || Flying;

    /* can't hide while being held (or holding) or while trapped
       (except for floor hiders [trapper or mimic] in pits) */
    if (u.ustuck || (u.utrap && (u.utraptype != TT_PIT || on_ceiling))) {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "hide while you're %s.",
                 !u.ustuck ? "trapped"
                   : u.uswallow ? (is_animal(u.ustuck->data) ? "swallowed"
                                                             : "engulfed")
                     : !sticks(youmonst.data) ? "being held"
                       : (humanoid(u.ustuck->data) ? "holding someone"
                                                   : "holding that creature"));
        if (u.uundetected
            || (ismimic && U_AP_TYPE != M_AP_NOTHING)) {
            u.uundetected = 0;
            youmonst.m_ap_type = M_AP_NOTHING;
            newsym(u.ux, u.uy);
        }
        return 0;
    }
    /* note: the eel and hides_under cases are hypothetical;
       such critters aren't offered the option of hiding via #monster */
    if (youmonst.data->mlet == S_EEL && !is_pool(u.ux, u.uy)) {
        if (IS_FOUNTAIN(levl[u.ux][u.uy].typ))
            The("fountain is not deep enough to hide in.");
        else
            There("is no %s to hide in here.", hliquid("water"));
        u.uundetected = 0;
        return 0;
    }
    if (hides_under(youmonst.data) && !level.objects[u.ux][u.uy]) {
        There("is nothing to hide under here.");
        u.uundetected = 0;
        return 0;
    }
    /* Planes of Air and Water */
    if (on_ceiling && !has_ceiling(&u.uz)) {
        There("is nowhere to hide above you.");
        u.uundetected = 0;
        return 0;
    }
    if ((is_hider(youmonst.data) && !Flying) /* floor hider */
        && (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz))) {
        There("is nowhere to hide beneath you.");
        u.uundetected = 0;
        return 0;
    }
    /* TODO? inhibit floor hiding at furniture locations, or
     * else make youhiding() give smarter messages at such spots.
     */

    if (u.uundetected || (ismimic && U_AP_TYPE != M_AP_NOTHING)) {
        youhiding(FALSE, 1); /* "you are already hiding" */
        return 0;
    }

    if (ismimic) {
        /* should bring up a dialog "what would you like to imitate?" */
        youmonst.m_ap_type = M_AP_OBJECT;
        youmonst.mappearance = STRANGE_OBJECT;
    } else
        u.uundetected = 1;
    newsym(u.ux, u.uy);
    youhiding(FALSE, 0); /* "you are now hiding" */
    return 1;
}

int
dopoly()
{
    struct permonst *savedat = youmonst.data;

    if (is_vampire(youmonst.data)) {
        polyself(2);
        if (savedat != youmonst.data) {
            You("transform into %s.", an(youmonst.data->mname));
            newsym(u.ux, u.uy);
        }
    }
    return 1;
}

int
dodryfountain()
{
    if (IS_FOUNTAIN(levl[u.ux][u.uy].typ)) {
        if (split_mon(&youmonst, (struct monst*) 0))
            dryup(u.ux, u.uy, TRUE);
        return 1;
    }
    else
        There("is no fountain here.");

    return 0;
}

int
douseunicornhorn()
{
    if (u.uen < UNICORN_HORN_MANA_COST) {
        play_sfx_sound(SFX_NOT_ENOUGH_MANA);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "lack the energy to use your horn!");
        return 1;
    }
    u.uen -= UNICORN_HORN_MANA_COST;
    context.botl = 1;

    struct obj dummyhorn = { 0 };
    dummyhorn.otyp = UNICORN_HORN;
    dummyhorn.oclass = TOOL_CLASS;
    dummyhorn.charges = 10;
    use_unicorn_horn(&dummyhorn);
    return 1;
}

int
doshriek()
{
    play_monster_chat_sound(&youmonst, 0);
    You("shriek.");
    if (u.uburied)
        pline("Unfortunately sound does not carry well through rock.");
    else
        aggravate();
    return 1;
}

int
dolayegg()
{
    struct obj* uegg;

    if (!flags.female)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s can't lay eggs!",
            Hallucination
            ? "You may think you are a platypus, but a male still"
            : "Males");
        return 0;
    }
    else if (u.uhunger < (int)objects[EGG].oc_nutrition)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MANA);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough energy to lay an egg.");
        return 0;
    }
    else if (eggs_in_water(youmonst.data))
    {
        if (!(Underwater || Is_waterlevel(&u.uz)))
        {
            pline("A splash tetra you are not.");
            return 0;
        }
        if (Upolyd &&
            (youmonst.data == &mons[PM_GIANT_EEL]
                || youmonst.data == &mons[PM_ELECTRIC_EEL]))
        {
            You("yearn for the Sargasso Sea.");
            return 0;
        }
    }
    uegg = mksobj(EGG, FALSE, FALSE, FALSE);
    uegg->speflags |= SPEFLAGS_YOURS;
    uegg->quan = 1L;
    uegg->owt = weight(uegg);
    /* this sets hatch timers if appropriate */
    set_corpsenm(uegg, egg_type_from_parent(u.umonnum, FALSE));
    uegg->known = uegg->dknown = 1;
    You("%s an egg.", eggs_in_water(youmonst.data) ? "spawn" : "lay");
    dropy(uegg);
    stackobj(uegg);
    morehungry((int)objects[EGG].oc_nutrition);
    return 1;
}


int
domindblast()
{
    struct monst *mtmp, *nmon;

    if (u.uen < MIND_BLAST_MANA_COST) {
        play_sfx_sound(SFX_NOT_ENOUGH_MANA);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "concentrate but lack the energy to maintain doing so.");
        return 0;
    }
    u.uen -= MIND_BLAST_MANA_COST;
    context.botl = 1;

    You_ex(ATR_NONE, CLR_MSG_SPELL, "concentrate.");
    pline_ex(ATR_NONE, CLR_MSG_SPELL, "A wave of psychic energy pours out.");
    for (mtmp = fmon; mtmp; mtmp = nmon) {
        int u_sen;

        nmon = mtmp->nmon;
        if (DEADMONSTER(mtmp))
            continue;
        if (distu(mtmp->mx, mtmp->my) > TELEPATHY_RANGE * TELEPATHY_RANGE)
            continue;
        if (is_peaceful(mtmp))
            continue;
        u_sen = (has_telepathy(mtmp)) && is_blinded(mtmp);
        if (u_sen || (has_telepathy(mtmp) && rn2(2)) || !rn2(10)) {
            You_ex(ATR_NONE, CLR_MSG_SPELL, "lock in on %s %s.", s_suffix(mon_nam(mtmp)),
                u_sen ? "telepathy"
                      : has_telepathy(mtmp) ? "latent telepathy" : "mind");
            deduct_monster_hp(mtmp, adjust_damage(rnd(15), &youmonst, mtmp, AD_PSIO, ADFLAGS_NONE));
            if (DEADMONSTER(mtmp))
                killed(mtmp);
        }
    }
    return 1;
}

void
uunstick()
{
    if (!u.ustuck) {
        impossible("uunstick: no ustuck?");
        return;
    }
    pline("%s is no longer in your clutches.", Monnam(u.ustuck));
    u.ustuck = 0;
}

void
skinback(silently)
boolean silently;
{
    if (uskin) {
        if (!silently)
            Your("skin returns to its original form.");
        uarm = uskin;
        uskin = (struct obj *) 0;
        /* undo save/restore hack */
        uarm->owornmask &= ~I_SPECIAL;
    }
}

const char *
mbodypart(mon, part)
struct monst *mon;
int part;
{
    STATIC_VAR NEARDATA const char
        *humanoid_parts[] = { "arm",       "eye",  "face",         "finger",
                              "fingertip", "foot", "hand",         "handed",
                              "head",      "leg",  "light headed", "neck",
                              "spine",     "toe",  "hair",         "blood",
                              "lung",      "nose", "stomach" },
        *jelly_parts[] = { "pseudopod", "dark spot", "front",
                           "pseudopod extension", "pseudopod extremity",
                           "pseudopod root", "grasp", "grasped",
                           "cerebral area", "lower pseudopod", "viscous",
                           "middle", "surface", "pseudopod extremity",
                           "ripples", "juices", "surface", "sensor",
                           "stomach" },
        *animal_parts[] = { "forelimb",  "eye",           "face",
                            "foreclaw",  "claw tip",      "rear claw",
                            "foreclaw",  "clawed",        "head",
                            "rear limb", "light headed",  "neck",
                            "spine",     "rear claw tip", "fur",
                            "blood",     "lung",          "nose",
                            "stomach" },
        *bird_parts[] = { "wing",     "eye",  "face",         "wing",
                          "wing tip", "foot", "wing",         "winged",
                          "head",     "leg",  "light headed", "neck",
                          "spine",    "toe",  "feathers",     "blood",
                          "lung",     "bill", "stomach" },
        *horse_parts[] = { "foreleg",  "eye",           "face",
                           "forehoof", "hoof tip",      "rear hoof",
                           "forehoof", "hooved",        "head",
                           "rear leg", "light headed",  "neck",
                           "backbone", "rear hoof tip", "mane",
                           "blood",    "lung",          "nose",
                           "stomach" },
        *sphere_parts[] = { "appendage", "optic nerve", "body", "tentacle",
                            "tentacle tip", "lower appendage", "tentacle",
                            "tentacled", "body", "lower tentacle",
                            "rotational", "equator", "body",
                            "lower tentacle tip", "cilia", "life force",
                            "retina", "olfactory nerve", "interior" },
        *fungus_parts[] = { "mycelium", "visual area", "front",
                            "hypha",    "hypha",       "root",
                            "strand",   "stranded",    "cap area",
                            "rhizome",  "sporulated",  "stalk",
                            "root",     "rhizome tip", "spores",
                            "juices",   "gill",        "gill",
                            "interior" },
        *vortex_parts[] = { "region",        "eye",           "front",
                            "minor current", "minor current", "lower current",
                            "swirl",         "swirled",       "central core",
                            "lower current", "addled",        "center",
                            "currents",      "edge",          "currents",
                            "life force",    "center",        "leading edge",
                            "interior" },
        *snake_parts[] = { "vestigial limb", "eye", "face", "large scale",
                           "large scale tip", "rear region", "scale gap",
                           "scale gapped", "head", "rear region",
                           "light headed", "neck", "length", "rear scale",
                           "scales", "blood", "lung", "forked tongue",
                           "stomach" },
        *worm_parts[] = { "anterior segment", "light sensitive cell",
                          "clitellum", "setae", "setae", "posterior segment",
                          "segment", "segmented", "anterior segment",
                          "posterior", "over stretched", "clitellum",
                          "length", "posterior setae", "setae", "blood",
                          "skin", "prostomium", "stomach" },
        *fish_parts[] = { "fin", "eye", "premaxillary", "pelvic axillary",
                          "pelvic fin", "anal fin", "pectoral fin", "finned",
                          "head", "peduncle", "played out", "gills",
                          "dorsal fin", "caudal fin", "scales", "blood",
                          "gill", "nostril", "stomach" };
    /* claw attacks are overloaded in mons[]; most humanoids with
       such attacks should still reference hands rather than claws */
    static const char not_claws[] = {
        S_HUMAN,     S_LESSER_UNDEAD,   S_GREATER_UNDEAD, S_ANGEL, S_NYMPH, S_LEPRECHAUN,
        S_QUANTMECH, S_VAMPIRE, S_ORC,    S_GIANT, /* quest nemeses */
        '\0' /* string terminator; assert( S_xxx != 0 ); */
    };
    struct permonst *mptr = mon->data;

    /* some special cases */
    if (mptr->mlet == S_DOG || mptr->mlet == S_FELINE
        || mptr->mlet == S_RODENT || (mptr->mflags4 & M4_PAWED))
    {
        switch (part) {
        case HAND:
            return "paw";
        case HANDED:
            return "pawed";
        case FOOT:
            return "rear paw";
        case ARM:
        case LEG:
            return horse_parts[part]; /* "foreleg", "rear leg" */
        default:
            break; /* for other parts, use animal_parts[] below */
        }
    } 
    else if (mptr->mlet == S_YETI) 
    { /* excl. owlbear due to 'if' above and M4_PAWED */
        /* opposable thumbs, hence "hands", "arms", "legs", &c */
        return humanoid_parts[part]; /* yeti/sasquatch, monkey/ape */
    }
    if ((part == HAND || part == HANDED)
        && (humanoid(mptr) && attacktype(mptr, AT_CLAW)
            && !index(not_claws, mptr->mlet) && mptr != &mons[PM_STONE_GOLEM]
            && mptr != &mons[PM_INCUBUS]))
        return (part == HAND) ? "claw" : "clawed";
    if ((mptr == &mons[PM_OLIPHANT] || mptr == &mons[PM_MASTODON])
        && part == NOSE)
        return "trunk";
    if (mptr == &mons[PM_SHARK] && part == HAIR)
        return "skin"; /* sharks don't have scales */
    if ((mptr == &mons[PM_JELLYFISH] || mptr == &mons[PM_KRAKEN])
        && (part == ARM || part == FINGER || part == HAND || part == FOOT
            || part == TOE))
        return "tentacle";
    if (mptr == &mons[PM_FLOATING_EYE] && part == EYE)
        return "cornea";
    if (humanoid(mptr) && (part == ARM || part == FINGER || part == FINGERTIP
                           || part == HAND || part == HANDED))
        return humanoid_parts[part];
    if (mptr == &mons[PM_RAVEN] || mptr == &mons[PM_ROC])
        return bird_parts[part];
    if (mptr->mlet == S_CENTAUR || mptr->mlet == S_UNICORN
        || (mptr == &mons[PM_ROTHE] && part != HAIR))
        return horse_parts[part];
    if (mptr->mlet == S_LIGHT) {
        if (part == HANDED)
            return "rayed";
        else if (part == ARM || part == FINGER || part == FINGERTIP
                 || part == HAND)
            return "ray";
        else
            return "beam";
    }
    if (mptr == &mons[PM_STALKER] && part == HEAD)
        return "head";
    if (mptr->mlet == S_EEL && mptr != &mons[PM_JELLYFISH])
        return fish_parts[part];
    if (mptr->mlet == S_WORM)
        return worm_parts[part];
    if (slithy(mptr) || (mptr->mlet == S_DRAGON && part == HAIR))
        return snake_parts[part];
    if (mptr->mlet == S_EYE)
        return sphere_parts[part];
    if (mptr->mlet == S_JELLY || mptr->mlet == S_PUDDING
        || mptr->mlet == S_BLOB || mptr == &mons[PM_JELLYFISH])
        return jelly_parts[part];
    if (mptr->mlet == S_VORTEX || mptr->mlet == S_ELEMENTAL)
        return vortex_parts[part];
    if (mptr->mlet == S_FUNGUS)
        return fungus_parts[part];
    if (humanoid(mptr))
        return humanoid_parts[part];
    return animal_parts[part];
}

const char *
body_part(part)
int part;
{
    return mbodypart(&youmonst, part);
}

int
poly_gender()
{
    /* Returns gender of polymorphed player;
     * 0/1=same meaning as flags.female, 2=none.
     */
    if (is_neuter(youmonst.data) || !humanoid(youmonst.data))
        return 2;
    return flags.female;
}

void
ugolemeffects(damtype, dam)
int damtype;
double dam;
{
    double heal = 0;

    /* We won't bother with "slow"/"haste" since players do not
     * have a monster-specific slow/haste so there is no way to
     * restore the old velocity once they are back to human.
     */
    if (u.umonnum != PM_FLESH_GOLEM && u.umonnum != PM_IRON_GOLEM)
        return;

    switch (damtype) 
    {
    case AD_ELEC:
        if (u.umonnum == PM_FLESH_GOLEM)
            heal = dam / 6; /* Approx 1 per die */
        break;
    case AD_FIRE:
        if (u.umonnum == PM_IRON_GOLEM)
            heal = dam;
        break;
    }
    if (heal && (u.mh < u.mhmax)) 
    {
        deduct_player_hp(-heal);
        pline("Strangely, you feel better than before.");
        exercise(A_STR, TRUE);
    }
}

STATIC_OVL int
armor_to_dragon(atyp)
int atyp;
{
    switch (atyp) {
    case GRAY_DRAGON_SCALE_MAIL:
    case GRAY_DRAGON_SCALES:
        return PM_GRAY_DRAGON;
    case SILVER_DRAGON_SCALE_MAIL:
    case SILVER_DRAGON_SCALES:
        return PM_SILVER_DRAGON;
    case RED_DRAGON_SCALE_MAIL:
    case RED_DRAGON_SCALES:
        return PM_RED_DRAGON;
    case ORANGE_DRAGON_SCALE_MAIL:
    case ORANGE_DRAGON_SCALES:
        return PM_ORANGE_DRAGON;
    case WHITE_DRAGON_SCALE_MAIL:
    case WHITE_DRAGON_SCALES:
        return PM_WHITE_DRAGON;
    case BLACK_DRAGON_SCALE_MAIL:
    case BLACK_DRAGON_SCALES:
        return PM_BLACK_DRAGON;
    case BLUE_DRAGON_SCALE_MAIL:
    case BLUE_DRAGON_SCALES:
        return PM_BLUE_DRAGON;
    case GREEN_DRAGON_SCALE_MAIL:
    case GREEN_DRAGON_SCALES:
        return PM_GREEN_DRAGON;
    case YELLOW_DRAGON_SCALE_MAIL:
    case YELLOW_DRAGON_SCALES:
        return PM_YELLOW_DRAGON;
    default:
        return -1;
    }
}

/* some species have awareness of other species */
STATIC_OVL void
polysense()
{
    short warnidx = NON_PM;

    context.warntype.speciesidx = NON_PM;
    context.warntype.species = 0;
    context.warntype.polyd = 0;
    HWarn_of_mon &= ~FROM_RACE;

    switch (u.umonnum) {
    case PM_PURPLE_WORM:
        warnidx = PM_SHRIEKER;
        break;
    case PM_VAMPIRE:
    case PM_VAMPIRE_MAGE:
    case PM_VAMPIRE_LORD:
        context.warntype.polyd = M2_HUMAN | M2_ELF;
        HWarn_of_mon |= FROM_RACE;
        return;
    }
    if (warnidx >= LOW_PM) {
        context.warntype.speciesidx = warnidx;
        context.warntype.species = &mons[warnidx];
        HWarn_of_mon |= FROM_RACE;
    }
}

/* True iff hero's role or race has been genocided */
boolean
ugenocided()
{
    return (boolean) ((mvitals[urole.monsternum].mvflags & MV_GENOCIDED)
                      || (mvitals[urace.monsternum].mvflags & MV_GENOCIDED)
                     );
}

/* how hero feels "inside" after self-genocide of role or race */
const char *
udeadinside()
{
    /* self-genocide used to always say "you feel dead inside" but that
       seems silly when you're polymorphed into something undead;
       monkilled() distinguishes between living (killed) and non (destroyed)
       for monster death message; we refine the nonliving aspect a bit */
    return is_living(youmonst.data)
             ? "dead"          /* living, including demons */
             : is_undead(youmonst.data)
                 ? "condemned" /* undead plus manes */
                 : "empty";    /* golems plus vortices */
}

void
reset_polyself(VOID_ARGS)
{
    sex_change_ok = 0;
}

/*polyself.c*/

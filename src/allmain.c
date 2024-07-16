/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    allmain.c    $NHDT-Date: 1555552624 2019/04/18 01:57:04 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.100 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

/* various code that was replicated in *main.c */

#include "hack.h"
#include <ctype.h>

#ifndef NO_SIGNAL
#include <signal.h>
#endif

#ifdef POSITIONBAR
STATIC_DCL void NDECL(do_positionbar);
#endif
STATIC_DCL void NDECL(regenerate_hp);
STATIC_DCL void NDECL(regenerate_mana);
STATIC_DCL void FDECL(interrupt_multi, (const char *, int, int));
STATIC_DCL void FDECL(debug_fields, (const char *));
STATIC_DCL void NDECL(create_monster_or_encounter);
STATIC_DCL int NDECL(select_rwraith);
STATIC_DCL boolean NDECL(maybe_create_rwraith);

#ifdef EXTRAINFO_FN
static int64_t prev_dgl_extrainfo = 0;
#endif

void
moveloop(resuming)
uchar resuming; /* 0 = new game, 1 = loaded a saved game, 2 = continued playing after saving (restart) */
{
#if defined(MICRO) || defined(WIN32)
    char ch;
    int abort_lev;
#endif
    int moveamt = 0, wtcap = 0, change = 0;
    boolean monscanmove = FALSE;
    struct obj* otmp;

    /* Note:  these initializers don't do anything except guarantee that
            we're linked properly.
       Necessary initializers are called just before move loop in windmain.c and newgame -- JG            
    */
    decl_init();
    monst_init();
    objects_init();

    /* Set sound volume */
    adjust_ghsound_general_volumes();
    update_hearing_array_and_ambient_sounds();

    /* if a save file created in normal mode is now being restored in
       explore mode, treat it as normal restore followed by 'X' command
       to use up the save file and require confirmation for explore mode */
    if (resuming && iflags.deferred_X)
        (void) enter_explore_mode();

    if (resuming < 2)
    {
        /* side-effects from the real world */
        flags.moonphase = phase_of_the_moon();
        if (flags.moonphase == FULL_MOON)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are lucky!  Full moon tonight.");
            change_luck(1, FALSE);
        }
        else if (flags.moonphase == NEW_MOON)
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Be careful!  New moon tonight.");
        }

        flags.friday13 = friday_13th();
        if (flags.friday13)
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Watch out!  Bad things can happen on Friday the 13th.");
            change_luck(-1, FALSE);
        }
    }

    if (!resuming)
    { /* new game */
        boolean oldverbose = flags.verbose;
        flags.verbose = FALSE;
        context.rndencode = rnd(9000);
        set_wear((struct obj *) 0); /* for side-effects of starting gear */
        (void) pickup(1, FALSE);      /* autopickup at initial location */
        flags.verbose = oldverbose;
    }

    context.botlx = TRUE; /* for STATUS_HILITES */
    update_inventory(); /* for perm_invent */

    if (resuming == 1) 
    { /* restoring old game */
        read_engr_at(u.ux, u.uy); /* subset of pickup() */
    }

    (void) encumber_msg(); /* in case they auto-picked up something */

    if (defer_see_monsters)
    {
        defer_see_monsters = FALSE;
        see_monsters();
    }
    initrack();

    u.uz0.dlevel = u.uz.dlevel;
    youmonst.movement = NORMAL_SPEED; /* give the hero some movement points */
    context.move = 0;

    /* Main move loop */
    program_state.in_moveloop = 1;

#ifdef WHEREIS_FILE
    touch_whereis();
#endif

    for (;;)
    {
#ifdef SAFERHANGUP
        if (program_state.done_hup)
            end_of_input();
#endif
        get_nh_event();
#ifdef POSITIONBAR
        do_positionbar();
#endif

        if (context.move) 
        {
            /* actual time passed */
            youmonst.movement -= NORMAL_SPEED;

            /* Loop for increasing youmonst.movement towards the action threshold of NORMAL_SPEED */
            do
            { /* hero can't move this turn loop */
                wtcap = encumber_msg();
                context.hungry_message_displayed = FALSE;

                /* Loop for moving monsters*/
                context.mon_moving = TRUE;
                do 
                {
                    monscanmove = movemon();
                    if (youmonst.movement >= NORMAL_SPEED)
                        break; /* it's now your turn */
                } 
                while (monscanmove);
                context.mon_moving = FALSE;

                /* Increase your movement */
                if (!monscanmove && youmonst.movement < NORMAL_SPEED)
                {
                    /* both hero and monsters are out of steam this round */
                    struct monst *mtmp;
                    if (!context.time_stopped)
                    {
                        /* set up for a new turn */
                        update_monster_timeouts(); /* adjust monsters' trap, blind, etc */

                        /* reallocate movement rations to monsters; don't need
                           to skip dead monsters here because they will have
                           been purged at end of their previous round of moving */
                        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
                            mtmp->movement += mcalcmove(mtmp, FALSE);

                        /* occasionally add another monster; since this takes
                           place after movement has been allotted, the new
                           monster effectively loses its first turn */
                        if (!rn2(u.uevent.invoked && !In_endgame(&u.uz) ? 15 /* More monsters after invocation, since there is no mysterious force */
                            : u.uevent.ukilled_wizard ? 25 /* If killed the wizard */
                            : Inhell ? 50
                            : 70))
                        {
                            create_monster_or_encounter();
                        }
                    }

                    moveamt = get_u_move_speed(FALSE);

                    youmonst.movement += moveamt;
                    if (youmonst.movement < 0)
                        youmonst.movement = 0;
                    settrack();

                    monstermoves++;
                    moves++;
                    context.hungry_message_displayed = FALSE;
                    context.mon_talking = FALSE;

                    /********************************/
                    /* once-per-turn things go here */
                    /********************************/

                    if (Glib)
                        glibr();
                    nh_timeout();
                    if(!context.time_stopped)
                        run_regions();

                    if (u.uprayer_timeout)
                    {
                        u.uprayer_timeout--;
                        if (!context.starting_prayer_timeout_expired && !u.uprayer_timeout)
                        {
                            context.starting_prayer_timeout_expired = TRUE;
                        }
                    }
                    if (flags.time && !context.run)
                        iflags.time_botl = TRUE;

#ifdef EXTRAINFO_FN
                    if ((prev_dgl_extrainfo == 0) || (prev_dgl_extrainfo < (moves + 250))) {
                        prev_dgl_extrainfo = moves;
                        mk_dgl_extrainfo();
                    }
#endif

                    /* One possible result of prayer is healing.  Whether or
                     * not you get healed depends on your current hit points.
                     * If you are allowed to regenerate during the prayer,
                     * the end-of-prayer calculation messes up on this.
                     * Another possible result is rehumanization, which
                     * requires that encumbrance and movement rate be
                     * recalculated.
                     */
                    if (u.uinvulnerable)
                    {
                        /* for the moment at least, you're in tiptop shape */
                        wtcap = UNENCUMBERED;
                    } 
                    else if ((!Upolyd ? (u.uhp < u.uhpmax)
                                       : (u.mh < u.mhmax))
                                          || youmonst.data->mlet == S_EEL || MummyRot) 
                    {
                        /* regenerate hit points */
                        regenerate_hp();
                    }

                    /* moving around while encumbered is hard work */
                    if (wtcap > MOD_ENCUMBER && u.umoved) 
                    {
                        if (!(wtcap < EXT_ENCUMBER ? moves % 30
                                                   : moves % 10)) 
                        {
                            if (Upolyd && u.mh > 1)
                            {
                                u.mh--;
                                context.botl = TRUE;
                            }
                            else if (!Upolyd && u.uhp > 1) 
                            {
                                u.uhp--;
                                context.botl = TRUE;
                            } else {
                                You("pass out from exertion!");
                                exercise(A_CON, FALSE);
                                fall_asleep(-10, FALSE);
                            }
                        }
                    }

                    /* regenerate mana */
                    regenerate_mana();


                    if (!u.uinvulnerable) 
                    {
                        if (Teleportation && (!Teleport_control || Stunned || Confusion) && !rn2(85))
                        {
                            xchar old_ux = u.ux, old_uy = u.uy;

                            tele();
                            if (u.ux != old_ux || u.uy != old_uy) {
                                if (!next_to_u()) {
                                    check_leash(old_ux, old_uy);
                                }
                                /* clear doagain keystrokes */
                                pushch(0);
                                savech(0);
                            }
                        }
                        /* delayed change may not be valid anymore */
                        if ((change == 1 && !Polymorph)
                            || (change == 2 && u.ulycn == NON_PM))
                            change = 0;

                        if (Polymorph && !rn2(100))
                            change = 1;
                        else if (u.ulycn >= LOW_PM && !Upolyd
                                 && !rn2(80 - (20 * night())))
                            change = 2;

                        if (change && !Unchanging) 
                        {
                            if (multi >= 0) {
                                stop_occupation();
                                if (change == 1)
                                    polyself(0);
                                else
                                    you_were();
                                change = 0;
                            }
                        }
                    }

                    if (smells_buried_searchable(&mons[Upolyd ? u.umonnum : urace.monsternum]) && multi >= 0)
                    {
                        int buriedsearchablefound = FALSE;
                        struct obj* otmp2;
                        for (otmp = level.buriedobjlist; otmp; otmp = otmp2)
                        {
                            otmp2 = otmp->nobj;
                            if (otmp->ox == u.ux && otmp->oy == u.uy && is_otyp_buried_searchable(otmp->otyp))
                            {
                                buriedsearchablefound = TRUE;
                                break;
                            }
                        }
                        if (buriedsearchablefound)
                        {
                            You("smell something buried underground.");
                            (void)unearth_objs(&youmonst, u.ux, u.uy, TRUE, TRUE);
                            (void)pickup(1, FALSE);
                            nomul(0);
                        }
                    }
                    if (Searching && multi >= 0 && !Sleeping && !Paralyzed_or_immobile)
                        (void) dosearch0(1);
                    if (Warning || Any_warning)
                        warnreveal();
                    mkot_trap_warn();
                    gethungry();
                    if (!context.time_stopped)
                    {
                        dosounds();
                        do_storms();
                        invault();
                    }
                    //age_spells();
                    //exerchk();  /* exercise system has been deactivated -- JG */
                    if (u.uhave.amulet)
                        amulet();
                    if (!rn2(40 + (int) (ACURR(A_DEX) * 3)))
                        u_wipe_engr(rnd(3));
                    if (u.uevent.ukilled_wizard && !u.uinvulnerable && !context.time_stopped)
                    {
                        if (u.uintervene_timer)
                            u.uintervene_timer--;
                        if (!u.uintervene_timer)
                        {
                            intervene();
                            u.uintervene_timer = rn1(200, 50);
                        }
                    }
                    restore_attrib();
                    /* underwater and waterlevel vision are done here */
                    if ((Is_waterlevel(&u.uz) || Is_airlevel(&u.uz)) && !context.time_stopped)
                        movebubbles();
                    else if (Is_firelevel(&u.uz) && !context.time_stopped)
                        fumaroles();
                    else if (Underwater)
                        under_water(0);
                    /* vision while buried done here */
                    else if (u.uburied)
                        under_ground(0);

                    /* when immobile, count is in turns */
                    if (multi < 0 && !Sleeping && !Paralyzed_or_immobile) /* Let Sleeping and Paralyzed expire first, and then multi */
                    {
                        if (++multi == 0)
                        { /* finished yet? */
                            unmul((char *) 0);
                            /* if unmul caused a level change, take it now */
                            if (u.utotype)
                                deferred_goto();
                        }
                    }
                }
            } while (youmonst.movement < NORMAL_SPEED); /* hero can't move */


            /* Now the hero can take finally move or take an action, unless the action is prevented by sleeping, paralysis, or being occupied */
            /* These are equivalent to the next ones below outside the if clause */

            /******************************************/
            /* once-per-hero-took-time things go here */
            /******************************************/

#ifdef STATUS_HILITES
            if (iflags.hilite_delta)
                status_eval_next_unhilite();
#endif
            if (context.bypasses)
                clear_bypasses();
            if ((u.uhave.amulet || Clairvoyant) && !In_endgame(&u.uz)
                && !Blocks_Clairvoyance && !(moves % 15) && !rn2(2))
                do_vicinity_map((struct obj *) 0);
            if (u.utrap && u.utraptype == TT_LAVA)
                sink_into_lava();
            /* when/if hero escapes from lava, he can't just stay there */
            else if (!u.umoved)
                (void) pooleffects(FALSE);

        } /* actual time passed */

        /****************************************/
        /* once-per-player-input things go here */
        /****************************************/

        clear_splitobjs();
        update_all_character_properties((struct obj*)0, TRUE);

        /* Clear context-dependent UI */
        clear_context_menu();
        if (ui_has_input())
            clear_run_and_travel();

        /* Redraw screen */
        if (!context.mv || Blind)
        {
            /* redo monsters if hallu or wearing a helm of telepathy */
            if (Hallucination) { /* update screen randomly */
                see_monsters();
                see_objects();
                see_traps();
                if (u.uswallow)
                    swallowed(0);
            } else if (Blind_telepat || Unblind_telepat) {
                see_monsters();
            } else if (Warning || Warn_of_mon || Any_warning)
                see_monsters();

            if (vision_full_recalc)
                vision_recalc(0); /* vision! */
        }

        /* Update hearing */
        if (!context.mv || Deaf)
        {
            if (hearing_full_recalc)
                update_hearing_array_and_ambient_sounds(); /* hearing! */
        }

        /* Update the statusline */
        context.skip_botl = FALSE; /* Shouldn't happen, but just for insurance */
        if (context.botl || context.botlx) 
        {
            bot();
            curs_on_u();
        }
        else if (iflags.time_botl) 
        {
            timebot();
            curs_on_u();
        }

        context.move = 1;

        if (multi >= 0 && occupation && !Sleeping && !Paralyzed_or_immobile) /* No occupation when sleeping or paralyzed */
        {
#if defined(MICRO) || defined(WIN32)
            abort_lev = 0;

            if (kbhit())
            {
                if ((ch = pgetchar()) == ABORT)
                    abort_lev++;
                else
                    pushch(ch);
            }

            if (!abort_lev && (*occupation)() == 0)
#else
            if ((*occupation)() == 0)
#endif
            {
                stop_occupation_ambient_sound(occsoundset, occtyp);
                play_occupation_immediate_sound(occsoundset, occtyp, OCCUPATION_SOUND_TYPE_FINISH);

                occupation = 0;
                occsoundset = 0;
                occtyp = 0;
            }

            if (
#if defined(MICRO) || defined(WIN32)
                abort_lev ||
#endif
                monster_nearby()) 
            {
                stop_occupation();
                reset_eat();
            }
#if defined(MICRO) || defined(WIN32)
            if (!(++occtime % 7))
                display_nhwindow(WIN_MAP, FALSE);
#endif
            continue;
        }

        if (iflags.sanity_check || iflags.debug_fuzzer)
            sanity_check();

#ifdef CLIPPING
        /* just before rhack */
        cliparound(u.ux, u.uy, FALSE);
#endif

        u.umoved = FALSE;

        if (!Sleeping && !Paralyzed_or_immobile)
        {
            if (multi > 0) 
            {
                lookaround();

                if (!multi) 
                {
                    /* lookaround may clear multi */
                    context.move = 0;
                    if (flags.time)
                        context.botl = TRUE;
                    continue;
                }

                if (context.mv) 
                {
                    if (multi < COLNO && !--multi)
                        clear_run_and_travel();
                    domove();
                }
                else 
                {
                    --multi;
                    rhack(save_cm);
                }
            }
            else if (multi == 0)
            {
#ifdef MAIL
                ckmailstatus();
#endif
                rhack((char*)0);
                reduce_last_item_show_duration();

            }
        }

        if (u.utotype)       /* change dungeon level */
            deferred_goto(); /* after rhack() */
        /* !context.move here: multiple movement command stopped */
        else if (flags.time && (!context.move || !context.mv))
            context.botl = TRUE;

        if (vision_full_recalc)
            vision_recalc(0); /* vision! */

        if (hearing_full_recalc)
            update_hearing_array_and_ambient_sounds(); /* hearing! */

        /* when running in non-tport mode, this gets done through domove() */
        if ((!context.run || flags.runmode == RUN_TPORT)
            && (multi && (!context.travel ? !(multi % 7) : !(moves % 7L))))
        {
            if (flags.time && context.run)
                context.botl = TRUE;
            /* [should this be flush_screen() instead?] */
            display_nhwindow(WIN_MAP, FALSE);
        }
    }
}

void 
reset_monster_origin_coordinates(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return;

    if (mtmp == &youmonst)
    {
        /* Reset u.ux0 and u.uy0 */
        u.ux0 = u.ux, u.uy0 = u.uy;
        //reset_object_origin_coordinates(invent);
    }
    
    mtmp->mx0 = mtmp->mx;
    mtmp->my0 = mtmp->my;

    //reset_object_origin_coordinates(mtmp->minvent);
}

void
reset_all_monster_origin_coordinates(VOID_ARGS)
{
    struct monst* mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        reset_monster_origin_coordinates(mtmp);
    }
}

void
reset_object_origin_coordinates(obj)
struct obj* obj;
{
    if (!obj)
        return;

    obj->ox0 = obj->ox;
    obj->oy0 = obj->oy;

    reset_object_origin_coordinates(obj->cobj);
}

void
reset_all_object_origin_coordinates(VOID_ARGS)
{
    struct obj* obj;
    for (obj = fobj; obj; obj = obj->nobj)
    {
        reset_object_origin_coordinates(obj);
    }
    for (obj = memoryobjs; obj; obj = obj->nobj)
    {
        reset_object_origin_coordinates(obj);
    }
}

void
reduce_last_item_show_duration(VOID_ARGS)
{
    if (context.last_picked_obj_show_duration_left > 0)
    {
        context.last_picked_obj_show_duration_left--;
        if (!context.last_picked_obj_show_duration_left)
            context.last_picked_obj_oid = 0;
    }
}

STATIC_OVL
void
create_monster_or_encounter(VOID_ARGS)
{

    /* Special wraith appearance for the Ruling Ring of Yendor */
    boolean rwraith_appeared = maybe_create_rwraith();

    if (!rwraith_appeared)
    {
        if (level.flags.nmgeninfos > 0)
        {
            /* Monster generation infos are defined */
            (void)make_level_monster_anywhere();
        }
        else if (!(u.uz.dnum == quest_dnum) && !In_endgame(&u.uz) && !Is_really_rogue_level(&u.uz) && !In_sokoban(&u.uz) && !In_V_tower(&u.uz) && (flags.wiz_alwaysenc || !rn2(ENCOUNTER_ONE_IN_CHANCE)))
        {
            randomize_encounter(0, 0);
        }
        else
        {
            (void)makemon2((struct permonst*) 0, 0, 0, NO_MM_FLAGS, MM2_RANDOMIZE_SUBTYPE);
        }
    }
}


STATIC_OVL
int
select_rwraith()
{
    /* Special wraith appearance for the Ruling Ring of Yendor */
    int minlevel = 0, maxlevel = 1;
    int mdx = NON_PM;
    boolean wraithlordok = FALSE;
    boolean kingwraithok = FALSE;
    boolean spectreok = FALSE;
    boolean barrowwightok = FALSE;
    boolean wraithok = FALSE;

    int i;
    for (i = 1; i <= 3; i++)
    {
        /* Difficulty level is one level higher than normal */
        get_generated_monster_minmax_levels(i, &minlevel, &maxlevel, 1);

        wraithlordok = maxlevel >= mons[PM_WRAITHLORD].difficulty && !(mvitals[PM_WRAITHLORD].mvflags & MV_GONE);
        kingwraithok = maxlevel >= mons[PM_KING_WRAITH].difficulty && !(mvitals[PM_KING_WRAITH].mvflags & MV_GONE);
        spectreok = maxlevel >= mons[PM_SPECTRE].difficulty && !(mvitals[PM_SPECTRE].mvflags & MV_GONE);
        barrowwightok = maxlevel >= mons[PM_BARROW_WIGHT].difficulty && !(mvitals[PM_BARROW_WIGHT].mvflags & MV_GONE);
        wraithok = maxlevel >= mons[PM_WRAITH].difficulty && !(mvitals[PM_WRAITH].mvflags & MV_GONE);

        if (wraithlordok || kingwraithok || spectreok || barrowwightok || wraithok)
            break;
    }

    /* Select a wraith */
    if (wraithlordok)
        mdx = PM_WRAITHLORD;
    else if (kingwraithok)
        mdx = PM_KING_WRAITH;
    else if (spectreok && (rn2(3) || !(barrowwightok || wraithok)))
        mdx = PM_SPECTRE;
    else if (barrowwightok && (!rn2(2) || !wraithok))
        mdx = PM_BARROW_WIGHT;
    else if (wraithok)
        mdx = PM_WRAITH;

    return mdx;
}

STATIC_OVL
boolean
maybe_create_rwraith()
{
    struct monst* rwraith = (struct monst*)0;
    boolean rwraith_appeared = FALSE;
    int mdx = NON_PM;

    if (!(u.uz.dnum == quest_dnum) && !In_endgame(&u.uz) && !Is_really_rogue_level(&u.uz) && !(u.uz.dnum == modron_dnum) && !(u.uz.dnum == bovine_dnum) && (mdx = select_rwraith()) >= LOW_PM)
    {
        /* Special wraith appearance if carrying the Ruling Ring of Yendor */
        struct obj* ring = carrying(RIN_SUPREME_POWER);
        if (ring && ring->oartifact == ART_RULING_RING_OF_YENDOR && mdx >= LOW_PM && !rn2(20))
        {
            if (!context.made_witch_king && mdx == PM_WRAITHLORD && !rn2(9))
            {
                rwraith = makemon(&mons[PM_WRAITHLORD], 0, 0, MM_MAX_HP | MM_MALE);
                if (rwraith)
                {
                    rwraith = christen_monst(rwraith, "Witch-King of Yendor");
                    rwraith->u_know_mname = TRUE; /* He's famous -- JG */
                    struct obj* crown = mongets(rwraith, ROYAL_CROWN);
                    if (crown)
                    {
                        crown->mythic_prefix = MYTHIC_PREFIX_WITCH_KINGS;
                        crown->mythic_suffix = MYTHIC_SUFFIX_SORCERY;
                        if (crown->enchantment < 5)
                            crown->enchantment = 5;
                        curse(crown);
                    }
                    context.made_witch_king = TRUE;
                }
            }
            else
            {
                rwraith = makemon(&mons[mdx], 0, 0, NO_MM_FLAGS);
            }
        }
    }

    if (rwraith)
    {
        rwraith->mon_flags |= MON_FLAGS_RWRAITH;
        rwraith->leaves_no_corpse = TRUE;
        rwraith_appeared = TRUE;
    }

    return rwraith_appeared;
}

/* maybe recover some lost health (or lose some when an eel out of water) */
STATIC_OVL void
regenerate_hp(VOID_ARGS)
{
    /* regenerate hp */
    int relevant_hpmax = Upolyd ? u.mhmax : u.uhpmax;
    int roundstofull = 
        Divine_regeneration ? max(1, min(relevant_hpmax / 16, 10)) :
        Rapidest_regeneration ? max(1, min(relevant_hpmax / 8, 20)) :
        Rapider_regeneration ? max(1, min(relevant_hpmax / 4, 40)) :
        Rapid_regeneration ? max(1, min(relevant_hpmax / 2, 80)) :
        Regeneration ? max(1, min(relevant_hpmax, 160)) :
        320;
    int fixedhpperround = relevant_hpmax / roundstofull;
    int fractional_hp = (10000 * (relevant_hpmax % roundstofull)) / roundstofull;
    int added_hp = 0;

    /* Mummy rot here */
    if (MummyRot && !Sick_resistance)
    {
        if (Divine_regeneration)
        {
            fixedhpperround = 4;
            fractional_hp = 0;
        }
        else if (Rapidest_regeneration)
        {
            fixedhpperround = 3;
            fractional_hp = 0;
        }
        else if (Rapider_regeneration)
        {
            fixedhpperround = 2;
            fractional_hp = 0;
        }
        else if (Rapid_regeneration)
        {
            fixedhpperround = 1;
            fractional_hp = 0;
        }
        else if (Regeneration)
        {
            fixedhpperround = 0;
            fractional_hp = 0;
            return;
        }
        else
        {
            roundstofull = 960;
            fixedhpperround = -relevant_hpmax / roundstofull;
            fractional_hp = -(10000 * (relevant_hpmax % roundstofull)) / roundstofull;
        }

    }

    if (Upolyd)
    {
        if (u.mh < 1) { /* shouldn't happen... */
            rehumanize();
        }
        else if (youmonst.data->mlet == S_EEL && !is_pool(u.ux, u.uy) && !Is_waterlevel(&u.uz))
        {
            /* eel out of water loses hp, similar to monster eels;
               as hp gets lower, rate of further loss slows down */
            if (!Regeneration && rn2(u.mh) > rn2(8) && (!Half_physical_damage || (Half_physical_damage && !rn2(2))))
            {
                u.mh--;
                context.botl = TRUE;
                force_redraw_at(u.ux, u.uy);
            }
        }
        else if (relevant_hpmax > 0 && (u.mh < relevant_hpmax || MummyRot))
        {
            int mh_before = u.mh;
            u.mh += fixedhpperround;
            u.mh_fraction += fractional_hp;
            if (u.mh_fraction >= 10000 || u.mh_fraction < 0)
            {
                added_hp = (u.mh_fraction / 10000) + (u.mh_fraction < 0 ? -1 : 0);
                u.mh += added_hp;
                u.mh_fraction -= 10000 * added_hp;
            }

            if (u.mh >= relevant_hpmax)
            {
                u.mh = relevant_hpmax;
                u.mh_fraction = 0;
            }
            context.botl = TRUE;
            if (u.mh != mh_before)
                force_redraw_at(u.ux, u.uy);
            if (u.mh == relevant_hpmax)
                interrupt_multi("You are in full health.", ATR_NONE, CLR_MSG_SUCCESS);
        }
    }
    else
    {
        if (relevant_hpmax > 0 && (u.uhp < relevant_hpmax || MummyRot))
        {
            int uhp_before = u.uhp;
            u.uhp += fixedhpperround;
            u.uhp_fraction += fractional_hp;
            if (u.uhp_fraction >= 10000 || u.uhp_fraction < 0)
            {
                added_hp = (u.uhp_fraction / 10000) + (u.uhp_fraction < 0 ? -1 : 0);
                u.uhp += added_hp;
                u.uhp_fraction -= 10000 * added_hp;
            }

            if (u.uhp >= relevant_hpmax)
            {
                u.uhp = relevant_hpmax;
                u.uhp_fraction = 0;
            }
            context.botl = TRUE;
            if (u.uhp != uhp_before)
                force_redraw_at(u.ux, u.uy);
            if (u.uhp == relevant_hpmax)
                interrupt_multi("You are in full health.", ATR_NONE, CLR_MSG_SUCCESS);
        }
    }

    if (MummyRot && !Sick_resistance && (Upolyd ? u.mh : u.uhp) <= 4 && added_hp <= -1)
    {
        You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "desiccated!");
        nomul(0);
    }
    if (Upolyd)
    {
        if (u.mh < 1) { /* shouldn't happen... */
            rehumanize();
        }
    }
    else
    {
        if (u.uhp <= 0)
        {
            if (MummyRot && !Sick_resistance)
            {
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die from your illness.");

                struct kinfo* kptr = find_delayed_killer(MUMMY_ROT);
                if (kptr && kptr->name[0])
                {
                    killer.format = kptr->format;
                    Strcpy(killer.name, kptr->name);
                    killer.hint_idx = HINT_KILLED_MUMMY_ROT;
                }
                else
                {
                    killer.format = KILLED_BY_AN;
                    killer.name[0] = 0; /* take the default */
                    killer.hint_idx = 0;
                }
                dealloc_killer(kptr);

                int m_idx;
                if ((m_idx = name_to_mon(killer.name)) >= LOW_PM)
                {
                    if (is_mname_proper_name(&mons[m_idx]))
                    {
                        killer.format = KILLED_BY;
                    }
                    else if (mons[m_idx].geno & G_UNIQ)
                    {
                        Strcpy(killer.name, the(killer.name));
                        killer.format = KILLED_BY;
                    }
                }
                done(ROTTED);
            }
            else if(youmonst.data->mlet == S_EEL && !is_pool(u.ux, u.uy) && !Is_waterlevel(&u.uz))
            {
                killer.format = KILLED_BY;
                Strcpy(killer.name, "drying");
                done(DIED);
            }
            else
            {
                /* should not happen */
                killer.format = KILLED_BY;
                Strcpy(killer.name, "loss of hit points");
                done(DIED);
            }
        }
    }

#if 0
    int heal = 0;
    boolean reached_full = FALSE,
        encumbrance_ok = (wtcap < MOD_ENCUMBER || !u.umoved);


    if (Upolyd) {
        if (u.mh < 1) { /* shouldn't happen... */
            rehumanize();
        } else if (youmonst.data->mlet == S_EEL
                   && !is_pool(u.ux, u.uy) && !Is_waterlevel(&u.uz)) {
            /* eel out of water loses hp, similar to monster eels;
               as hp gets lower, rate of further loss slows down */
            if (u.mh > 1 && !Regeneration && rn2(u.mh) > rn2(8)
                && (!Half_physical_damage || !(moves % 2L)))
                heal = -1;
        } else if (u.mh < u.mhmax) {
            if (Regeneration || (encumbrance_ok && !(moves % 20L)))
                heal = 1;
        }
        if (heal) {
            context.botl = TRUE;
            u.mh += heal;
            reached_full = (u.mh == u.mhmax);
        }

    /* !Upolyd */
    } else {
        /* [when this code was in-line within moveloop(), there was
           no !Upolyd check here, so poly'd hero recovered lost u.uhp
           once u.mh reached u.mhmax; that may have been convenient
           for the player, but it didn't make sense for gameplay...] */
        if (u.uhp < u.uhpmax && (encumbrance_ok || Regeneration)) {
            if (u.ulevel > 9) {
                if (!(moves % 3L)) {
                    int Con = (int) ACURR(A_CON);

                    if (Con <= 12) {
                        heal = 1;
                    } else {
                        heal = rnd(Con);
                        if (heal > u.ulevel - 9)
                            heal = u.ulevel - 9;
                    }
                }
            } else { /* u.ulevel <= 9 */
                if (!(moves % (int64_t) ((MAXULEV + 12) / (u.ulevel + 2) + 1)))
                    heal = 1;
            }
            if (Regeneration && !heal)
                heal = 1;

            if (heal) {
                context.botl = TRUE;
                u.uhp += heal;
                if (u.uhp > u.uhpmax)
                    u.uhp = u.uhpmax;
                /* stop voluntary multi-turn activity if now fully healed */
                reached_full = (u.uhp == u.uhpmax);
            }
        }
    }

    if (reached_full)
        interrupt_multi("You are in full health.", ATR_NONE, CLR_MSG_SUCCESS);
#endif
}

STATIC_OVL void
regenerate_mana(VOID_ARGS)
{
    /* regenerate mana */
    int roundstofull =
        Rapidest_energy_regeneration ? max(1, min(u.uenmax / 12, 15)) :
        Rapider_energy_regeneration ? max(1, min(u.uenmax / 6, 30)) :
        Rapid_energy_regeneration ? max(1, min(u.uenmax / 3, 60)) :
        Energy_regeneration ? max(1, min((2 * u.uenmax) / 3, 120)) :
        240;
    int fixedmanaperround = u.uenmax / roundstofull;
    int fractional_mana = (10000 * (u.uenmax % roundstofull)) / roundstofull;

    if (u.uenmax > 0 && u.uen < u.uenmax)
    {
        u.uen += fixedmanaperround;
        u.uen_fraction += fractional_mana;
        if (u.uen_fraction >= 10000)
        {
            int added_mana = (u.uen_fraction / 10000);
            u.uen += added_mana;
            u.uen_fraction -= 10000 * added_mana;
        }

        if (u.uen >= u.uenmax)
        {
            u.uen = u.uenmax;
            u.uen_fraction = 0;
        }
        context.botl = TRUE;
        if (u.uen == u.uenmax)
            interrupt_multi("You feel full of energy.", ATR_NONE, CLR_MSG_SUCCESS);
    }

}


void
stop_occupation()
{
    if (occupation) {
        if (!maybe_finished_meal(TRUE))
            You("stop %s.", occtxt);
        stop_occupation_ambient_sound(occsoundset, occtyp);
        play_occupation_immediate_sound(occsoundset, occtyp, OCCUPATION_SOUND_TYPE_INTERRUPTED);
        occupation = 0;
        occsoundset = 0;
        occtyp = 0;
        context.botl = TRUE; /* in case u.uhs changed */
        nomul(0);
        pushch(0);
    } else if (multi >= 0) {
        nomul(0);
    }
}

void
display_gamewindows()
{
    WIN_MESSAGE = create_nhwindow(NHW_MESSAGE);
    if (VIA_WINDOWPORT()) {
        status_initialize(0);
    } else {
        WIN_STATUS = create_nhwindow(NHW_STATUS);
    }
    WIN_MAP = create_nhwindow(NHW_MAP);
    WIN_INVEN = create_nhwindow(NHW_MENU);
    /* in case of early quit where WIN_INVEN could be destroyed before
       ever having been used, use it here to pacify the Qt interface */
    start_menu_ex(WIN_INVEN, GHMENU_STYLE_PERMANENT_INVENTORY);
    end_menu(WIN_INVEN, (char*)0);

    if (!(windowprocs.wincap2 & WC2_HEREWINDOW))
        iflags.wc2_herewindow = FALSE;

    if(iflags.wc2_herewindow)
        WIN_HERE = create_nhwindow(NHW_HERE);

#ifdef MAC
    /* This _is_ the right place for this - maybe we will
     * have to split display_gamewindows into create_gamewindows
     * and show_gamewindows to get rid of this ifdef...
     */
    if (!strcmp(windowprocs.name, "mac"))
        SanePositions();
#endif

    /*
     * The mac port is not DEPENDENT on the order of these
     * displays, but it looks a lot better this way...
     */
#ifndef STATUS_HILITES
    display_nhwindow(WIN_STATUS, FALSE);
#endif
    display_nhwindow(WIN_MESSAGE, FALSE);
    clear_glyph_buffer();
    display_nhwindow(WIN_MAP, FALSE);

    if (iflags.wc2_herewindow)
        display_nhwindow(WIN_HERE, FALSE);

}

const char*
get_game_difficulty_text(dif_level)
int dif_level;
{
    
    switch (dif_level)
    {
    case -4:
        return "standard";
    case -3:
        return "experienced";
    case -2:
        return "adept";
    case -1:
        return "veteran";
    case 0:
        return "expert";
    case 1:
        return "master";
    case 2:
        return "grand master";
    default:
        return "unknown";
    }

    return "unknown";
}

const char*
get_game_difficulty_symbol(dif_level)
int dif_level;
{

    switch (dif_level)
    {
    case -4:
        return "s";
    case -3:
        return "e";
    case -2:
        return "a";
    case -1:
        return "v";
    case 0:
        return "x";
    case 1:
        return "m";
    case 2:
        return "g";
    default:
        return "?";
    }

    return "?";
}

const char*
get_game_mode_text(display_nonscoring)
boolean display_nonscoring;
{
    return get_game_mode_text_core(wizard, discover, ModernMode, CasualMode, flags.non_scoring, TournamentMode, display_nonscoring);
}

const char*
get_game_mode_text_core(iswizardmode, isexporemode, ismodernmode, iscasualmode, isnonscoring, istournamentmode, display_nonscoring)
boolean display_nonscoring, iswizardmode, isexporemode, ismodernmode, iscasualmode, isnonscoring, istournamentmode;
{
    if (iswizardmode)
    {
        if (ismodernmode)
        {
            if (iscasualmode)
            {
                return "casual wizard";
            }
            else
            {
                return "modern wizard";
            }
        }
        else
        {
            if (iscasualmode)
            {
                return "reloadable wizard";
            }
            else
            {
                return "wizard";
            }
        }
    }
    else if (isexporemode)
    {
        if (ismodernmode)
        {
            if (iscasualmode)
            {
                return display_nonscoring ? "non-scoring casual explore" : "casual explore";
            }
            else
            {
                return display_nonscoring ? "non-scoring modern explore" : "modern explore";
            }
        }
        else
        {
            if (iscasualmode)
            {
                return display_nonscoring ? "non-scoring reloadable explore" : "reloadable explore";
            }
            else
            {
                return display_nonscoring ? "non-scoring explore" : "explore";
            }
        }
       
    }
    else if (iscasualmode)
    {
        if (ismodernmode)
            return display_nonscoring ? "non-scoring casual" : "casual";
        else
            return display_nonscoring ? "non-scoring reloadable" : "reloadable";
    }
    else if (ismodernmode)
        return display_nonscoring && isnonscoring ? "non-scoring modern" : "modern";
    else
        return display_nonscoring && isnonscoring ? "non-scoring classic" : istournamentmode ? "tournament" : "classic";
}

const char*
get_game_mode_description()
{
    return get_game_mode_description_core(wizard, discover, ModernMode, CasualMode);
}

const char*
get_game_mode_description_core(iswizardmode, isexporemode, ismodernmode, iscasualmode)
boolean iswizardmode, isexporemode, ismodernmode, iscasualmode;
{
    if (iswizardmode)
        return "immortal mode with debug commands";
    else if (isexporemode)
        return "non-scoring immortal mode with loadable saved games";
    else if (iscasualmode)
    {
        if (ismodernmode)
            return "revival upon death and loadable saved games";
        else
            return "permanent death but loadable saved games";
    }
    else if (ismodernmode)
        return "revival and score reduction upon death";
    else
        return "traditional mode with permanent death";

}

#define QUIT_DUMMY 100
void 
choose_game_difficulty(VOID_ARGS)
{
    int mindifficulty = TournamentMode ? max(0, sysopt.min_difficulty) : sysopt.min_difficulty;
    int maxdifficulty = sysopt.max_difficulty;
    if (mindifficulty > maxdifficulty)
    {
        /* Assume difficulty levels are disabled; perhaps should throw an error */
        context.game_difficulty = 0;
        return;
    }
    else if (mindifficulty == maxdifficulty)
    {
        /* No need to choose if only one choice */
        context.game_difficulty = mindifficulty;
        return;
    }

    winid menuwin;
    menu_item* selected = (menu_item*)0;
    int n = 0;

    menuwin = create_nhwindow(NHW_MENU);
    start_menu_ex(menuwin, GHMENU_STYLE_CHOOSE_DIFFICULTY);
    anything any = zeroany;

    int i;
    for(i = mindifficulty; i <= maxdifficulty; i++)
    {
        any = zeroany;
        any.a_int = i - MIN_DIFFICULTY_LEVEL + 1;
        char diffchar = i - MIN_DIFFICULTY_LEVEL + 'a';
        char buf[BUFSZ] = "", buf2[BUFSZ] = "";
        const char* leveltext;
        double m_dmg_mult = 0, m_hp_mult = 0;
        get_game_difficulty_multipliers_by_level(&m_dmg_mult, &m_hp_mult, (schar)i);
        int combat_damage_percentage = (int)(m_dmg_mult * m_hp_mult * 100.0);
        
        leveltext = get_game_difficulty_text(i);
        Strcpy(buf, leveltext);
        *buf = highc(*buf);

        if(iflags.menu_tab_sep)
            Sprintf(buf2, "%s\t(%d%% combat)", buf, combat_damage_percentage);
        else
            Sprintf(buf2, "%s (%d%% combat)", buf, combat_damage_percentage);

        add_menu(menuwin, NO_GLYPH, &any, diffchar, 0, ATR_NONE, NO_COLOR,
            buf2, MENU_UNSELECTED);
    }
    if (!(windowprocs.wincap2 & WC2_MENU_SHOWS_OK_CANCEL))
    {
        any = zeroany;
        add_menu(menuwin, NO_GLYPH, &any, 0, 0, ATR_HALF_SIZE, NO_COLOR,
            " ", MENU_UNSELECTED);

        any.a_int = QUIT_DUMMY;
        add_menu(menuwin, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR,
            "Quit", MENU_UNSELECTED);
    }
    end_menu(menuwin, "Pick a level for game difficulty");
    n = select_menu(menuwin, PICK_ONE, &selected);
    destroy_nhwindow(menuwin);
    if (n > 0)
    {
        if (selected->item.a_int == QUIT_DUMMY)
        {
            /* Quit */
            nh_bail(EXIT_SUCCESS, "Until next time, then...", TRUE);
        }
        context.game_difficulty = selected->item.a_int + MIN_DIFFICULTY_LEVEL - 1;
        free((genericptr_t)selected);
    }
    else
    {
        /* Quit upon ESC, too */
        nh_bail(EXIT_SUCCESS, "Until next time, then...", TRUE);
    }
}
#undef QUIT_DUMMY

void
set_mouse_buttons(VOID_ARGS)
{
    if (flags.right_click_command == DEFCLICK_ROLE)
    {
        switch (Role_switch)
        {
        default:
        case PM_ARCHAEOLOGIST:
        case PM_BARBARIAN:
        case PM_CAVEMAN:
        case PM_KNIGHT:
        case PM_RANGER:
        case PM_ROGUE:
        case PM_SAMURAI:
        case PM_TOURIST:
        case PM_VALKYRIE:
            flags.right_click_command = CLICK_FIRE;
            break;
        case PM_MONK:
        case PM_HEALER:
        case PM_PRIEST:
        case PM_WIZARD:
            flags.right_click_command = CLICK_CAST;
            break;
        }
    }
    if (flags.middle_click_command == DEFCLICK_ROLE)
    {
        switch (Role_switch)
        {
        default:
        case PM_ARCHAEOLOGIST:
        case PM_BARBARIAN:
        case PM_CAVEMAN:
        case PM_KNIGHT:
        case PM_RANGER:
        case PM_ROGUE:
        case PM_SAMURAI:
        case PM_TOURIST:
        case PM_VALKYRIE:
            flags.middle_click_command = CLICK_LOOK;
            break;
        case PM_MONK:
        case PM_HEALER:
        case PM_PRIEST:
        case PM_WIZARD:
            flags.middle_click_command = CLICK_ZAP;
            break;
        }
    }
}

void
newgame(VOID_ARGS)
{
    int i;

#ifdef MFLOPPY
    gameDiskPrompt();
#endif

    context.botlx = TRUE;
    context.ident = 1;
    context.stethoscope_move = -1L;
    context.warnlevel = 1;
    context.next_attrib_check = 600L; /* arbitrary first setting */
    context.tribute.enabled = TRUE;   /* turn on 3.6 tributes    */
    context.tribute.tributesz = (uint64_t)sizeof(struct tribute_info);
    Strcpy(context.used_names, "|");

    init_rm();
    init_fountains();
    init_animations();
    init_tiledata();

#ifdef USE_TILES
    flags.screen_scale_adjustment = flags.preferred_screen_scale == 0 ? 0.0
        : max(MIN_SCREEN_SCALE_ADJUSTMENT, min(MAX_SCREEN_SCALE_ADJUSTMENT, ((double)flags.preferred_screen_scale) / 100.0 - 1.0));
#endif

    for (i = LOW_PM; i < NUM_MONSTERS; i++)
        if((mons[i].geno & G_NOCORPSE) != 0)
            mvitals[i].mvflags |= MV_NOCORPSE;

    init_objects(); /* must be before u_init() */

    flags.pantheon = -1; /* role_init() will reset this */
    role_init();         /* must be before init_dungeons(), u_init(),
                          * and init_artifacts() */

    init_dungeons();  /* must be before u_init() to avoid rndmonst()
                       * creating odd monsters for any tins and eggs
                       * in hero's initial inventory */
    init_artifacts(); /* before u_init() in case $WIZKIT specifies
                       * any artifacts */
    u_init();

    encounter_init();        /* initialize encounters and force linkage */

    issue_simple_gui_command(GUI_CMD_LOAD_GLYPHS);

#ifndef NO_SIGNAL
    (void) signal(SIGINT, (SIG_RET_TYPE) done1);
#endif
#ifdef NEWS
    if (iflags.news)
        display_file(NEWS, FALSE);
#endif
    load_qtlist();          /* load up the quest text info */
    /* quest_init();  --  Now part of role_init() */

    mklev();
    u_on_upstairs();
    if (wizard)
        obj_delivery(FALSE); /* finish wizkit */
    vision_reset();          /* set up internals for level (after mklev) */
    
    if (MON_AT(u.ux, u.uy))
        mnexto(m_at(u.ux, u.uy));
    (void) makedog();

    /* Change to intro music */
    update_game_music();

    docrt();
    status_reassess();

    if (flags.legacy) {
        issue_simple_gui_command(GUI_CMD_LOAD_INTRO_SOUND_BANK);
        flush_screen(1);
        com_pager_ex((struct monst*)0, 1, ATR_NONE, CLR_MSG_HINT, FALSE);
        issue_simple_gui_command(GUI_CMD_UNLOAD_INTRO_SOUND_BANK);
    }

    /* Game is starting now */
    context.game_started = TRUE;
    urealtime.realtime = 0L;
    urealtime.start_timing = (int64_t)getnow();

#ifdef INSURANCE
    save_currentstate();
#endif
    program_state.something_worth_saving++; /* useful data now exists */

    /* Delete existing save files and backup save files from previous characters of the same name, if any; this may happen if save file has been corrupted */
    set_savefile_name(TRUE);
    (void)delete_savefile_if_exists();
    (void)delete_tmp_backup_savefile();
    (void)delete_backup_savefile();
    (void)delete_error_savefile();
    delete_excess_levelfiles();

    /* Change to the main music */
    update_game_music();
    play_level_ambient_sounds();
    play_environment_ambient_sounds();

    /* Success! */
    /* Fade from black */
    issue_simple_gui_command(GUI_CMD_FADE_FROM_BLACK_SLOWLY_NONBLOCKING);

    /* Welcome! */
    welcome(TRUE);

    /* GUI tips */
    show_gui_tips();

    /* Check special room */
    check_special_room(FALSE);

    return;
}

void
show_gui_tips(VOID_ARGS)
{
    struct special_view_info info = { 0 };
    info.viewtype = SPECIAL_VIEW_GUI_TIPS;
    open_special_view(info);
}

/* show "welcome [back] to GnollHack" message at program startup */
void
welcome(new_game)
boolean new_game; /* false => restoring an old game */
{
    char buf[BUFSZ];
    boolean currentgend = Ufemale;

    /* skip "welcome back" if restoring a doomed character */
    if (!new_game && Upolyd && ugenocided()) {
        /* death via self-genocide is pending */
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "You're back, but you still feel %s inside.", udeadinside());
        return;
    }

    /*
     * The "welcome back" message always describes your innate form
     * even when polymorphed or wearing a helm of opposite alignment.
     * Alignment is shown unconditionally for new games; for restores
     * it's only shown if it has changed from its original value.
     * Sex is shown for new games except when it is redundant; for
     * restores it's only shown if different from its original value.
     */
    *buf = '\0';
    if (new_game || u.ualignbase[A_ORIGINAL] != u.ualignbase[A_CURRENT])
        Sprintf(eos(buf), " %s", align_str(u.ualignbase[A_ORIGINAL]));
    if (!urole.name.f
        && (new_game
                ? (urole.allow & ROLE_GENDMASK) == (ROLE_MALE | ROLE_FEMALE)
                : currentgend != flags.initgend))
        Sprintf(eos(buf), " %s", genders[currentgend].adj);

    int multicolors[5] = { NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR };
    pline_multi_ex(ATR_NONE, CLR_MSG_HINT, no_multiattrs, multicolors, new_game ? "%s %s, welcome to GnollHack!  You are a%s %s %s."
                   : "%s %s, the%s %s %s, welcome back to GnollHack!",
          Hello((struct monst *) 0), plname, buf, urace.adj,
          (currentgend && urole.name.f) ? urole.name.f : urole.name.m);

    if (new_game)
    {
        char postbuf[BUFSZ * 2];
        Sprintf(postbuf, "%s the%s %s %s has entered the dungeon on %s difficulty in %s mode", plname, buf, urace.adj,
            (currentgend&& urole.name.f) ? urole.name.f : urole.name.m, get_game_difficulty_text(context.game_difficulty), get_game_mode_text(FALSE));
        livelog_printf(LL_DUMP | LL_GAME_START, "%s", postbuf);
    }
}

#ifdef POSITIONBAR
STATIC_DCL void
do_positionbar()
{
    static char pbar[COLNO];
    char *p;

    p = pbar;
    /* up stairway */
    if (upstair.sx
        && (generic_glyph_to_cmap(level.locations[upstair.sx][upstair.sy].hero_memory_layers.glyph)
                == S_upstair
            || generic_glyph_to_cmap(level.locations[upstair.sx][upstair.sy].hero_memory_layers.glyph)
                   == S_upladder)) {
        *p++ = '<';
        *p++ = upstair.sx;
    }
    if (sstairs.sx
        && (generic_glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].hero_memory_layers.glyph)
                == S_upstair
            || generic_glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].hero_memory_layers.glyph)
                   == S_upstair)) {
        *p++ = '<';
        *p++ = sstairs.sx;
    }

    /* down stairway */
    if (dnstair.sx
        && (generic_glyph_to_cmap(level.locations[dnstair.sx][dnstair.sy].hero_memory_layers.glyph)
                == S_dnstair
            || generic_glyph_to_cmap(level.locations[dnstair.sx][dnstair.sy].hero_memory_layers.glyph)
                   == S_dnladder)) {
        *p++ = '>';
        *p++ = dnstair.sx;
    }
    if (sstairs.sx
        && (generic_glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].hero_memory_layers.glyph)
                == S_branch_dnstair
            || generic_glyph_to_cmap(level.locations[sstairs.sx][sstairs.sy].hero_memory_layers.glyph)
                   == S_extra_dnstair)) {
        *p++ = '>';
        *p++ = sstairs.sx;
    }

    /* hero location */
    if (u.ux) {
        *p++ = '@';
        *p++ = u.ux;
    }
    /* fence post */
    *p = 0;

    update_positionbar(pbar);
}
#endif

STATIC_DCL void
interrupt_multi(msg, attr, color)
const char *msg;
int attr, color;
{
    if (multi > 0 && !context.travel && !context.run) {
        nomul(0);
        if (flags.verbose && msg)
            Norep_ex(attr, color, "%s", msg);
    }
}

/*
 * Argument processing helpers - for xxmain() to share
 * and call.
 *
 * These should return TRUE if the argument matched,
 * whether the processing of the argument was
 * successful or not.
 *
 * Most of these do their thing, then after returning
 * to xxmain(), the code exits without starting a game.
 *
 */

static const struct early_opt earlyopts[] = {
    {ARG_DEBUG, "debug", 5, TRUE},
    {ARG_VERSION, "version", 4, TRUE},
#ifdef WIN32
    {ARG_WINDOWS, "windows", 4, TRUE},
#endif
};

#ifdef WIN32
extern int FDECL(windows_early_options, (const char *));
#endif

/*
 * Returns:
 *    0 = no match
 *    1 = found and skip past this argument
 *    2 = found and trigger immediate exit
 */

int
argcheck(argc, argv, e_arg)
int argc;
char *argv[];
enum earlyarg e_arg;
{
    int i, idx;
    boolean match = FALSE;
    char *userea = (char *)0;
    const char *dashdash = "";

    for (idx = 0; idx < SIZE(earlyopts); idx++) {
        if (earlyopts[idx].e == e_arg)
            break;
    }
    if ((idx >= SIZE(earlyopts)) || (argc <= 1))
            return FALSE;

    for (i = 0; i < argc; ++i) {
        if (argv[i][0] != '-')
            continue;
        if (argv[i][1] == '-') {
            userea = &argv[i][2];
            dashdash = "-";
        } else {
            userea = &argv[i][1];
        }
        match = match_optname(userea, earlyopts[idx].name,
                              (size_t)earlyopts[idx].minlength,
                              earlyopts[idx].valallowed);
        if (match) break;
    }

    if (match) {
        const char *extended_opt = index(userea, ':');

        if (!extended_opt)
            extended_opt = index(userea, '=');
        switch(e_arg) {
        case ARG_DEBUG:
            if (extended_opt) {
                extended_opt++;
                debug_fields(extended_opt);
            }
            return 1;
        case ARG_VERSION: {
            boolean insert_into_pastebuf = FALSE;

            if (extended_opt) {
                extended_opt++;
                if (match_optname(extended_opt, "paste", 5, FALSE)) {
                    insert_into_pastebuf = TRUE;
                } else {
                    raw_printf(
                   "-%sversion can only be extended with -%sversion:paste.\n",
                               dashdash, dashdash);
                    return TRUE;
                }
            }
            early_version_info(insert_into_pastebuf);
            return 2;
        }
#ifdef WIN32
        case ARG_WINDOWS: {
            if (extended_opt) {
                extended_opt++;
                return windows_early_options(extended_opt);
            }
        }
#endif
        default:
            break;
        }
    };
    return FALSE;
}

/*
 * These are internal controls to aid developers with
 * testing and debugging particular aspects of the code.
 * They are not player options and the only place they
 * are documented is right here. No gameplay is altered.
 *
 * test             - test whether this parser is working
 * ttystatus        - TTY:
 * immediateflips   - WIN32: turn off display performance
 *                    optimization so that display output
 *                    can be debugged without buffering.
 */
void
debug_fields(opts)
const char *opts;
{
    char *op;
    boolean negated = FALSE;

    while ((op = index(opts, ',')) != 0) {
        *op++ = 0;
        /* recurse */
        debug_fields(op);
    }
    if (strlen(opts) > BUFSZ / 2)
        return;


    /* strip leading and trailing white space */
    while (isspace((uchar) *opts))
        opts++;
    op = eos((char *) opts);
    while (--op >= opts && isspace((uchar) *op))
        *op = '\0';

    if (!*opts) {
        /* empty */
        return;
    }
    while ((*opts == '!') || !strncmpi(opts, "no", 2)) {
        if (*opts == '!')
            opts++;
        else
            opts += 2;
        negated = !negated;
    }
    if (match_optname(opts, "test", 4, FALSE))
        iflags.debug.test = negated ? FALSE : TRUE;
#ifdef TTY_GRAPHICS
    if (match_optname(opts, "ttystatus", 9, FALSE))
        iflags.debug.ttystatus = negated ? FALSE : TRUE;
#endif
#ifdef WIN32
    if (match_optname(opts, "immediateflips", 14, FALSE))
        iflags.debug.immediateflips = negated ? FALSE : TRUE;
#endif
    return;
}


int
get_u_move_speed(return_expected_value)
boolean return_expected_value;
{
    int wtcap = near_capacity();
    int moveamt = 0;

    /* calculate how much time passed. */
    if (u.usteed && (u.umoved || return_expected_value))
    {
        /* your speed doesn't augment steed's speed */
        moveamt = mcalcmove(u.usteed, return_expected_value);
    }
    else
    {
        if (!Slowed)
            moveamt = youmonst.data->mmove;
        else
            moveamt = (youmonst.data->mmove * 2 + 2) / 3;

        if (Lightning_fast)
        {
            moveamt += NORMAL_SPEED;
            if (return_expected_value)
            {
                moveamt += (2 * NORMAL_SPEED) /3;
            }
            else
            {
                if (rn2(3) != 0)
                    moveamt += NORMAL_SPEED;
            }
        }
        else if (Super_fast)
        {
            moveamt += NORMAL_SPEED;
            if (return_expected_value)
            {
                moveamt += (1 * NORMAL_SPEED) / 3;
            }
            else
            {
                if (rn2(3) == 0)
                    moveamt += NORMAL_SPEED;
            }
        }
        else if (Ultra_fast)
        {
            moveamt += NORMAL_SPEED;
        }
        else if (Very_fast)
        {
            /* randomization is here to avoid the player from optimizing the speed difference -- a slower monster has a chance of catching up! */
            /* gain a free action on 2/3 of turns */
            if (return_expected_value)
            {
                moveamt += (2 * NORMAL_SPEED) / 3;
            }
            else
            {
                if (rn2(3) != 0)
                    moveamt += NORMAL_SPEED;
            }
        }
        else if (Fast)
        {
            /* gain a free action on 1/3 of turns */
            if (return_expected_value)
            {
                moveamt += (1 * NORMAL_SPEED) / 3;
            }
            else
            {
                if (rn2(3) == 0)
                    moveamt += NORMAL_SPEED;
            }
        }
    }

    switch (wtcap)
    {
    case UNENCUMBERED:
        break;
    case SLT_ENCUMBER:
        moveamt -= (moveamt / 4);
        break;
    case MOD_ENCUMBER:
        moveamt -= (moveamt / 2);
        break;
    case HVY_ENCUMBER:
        moveamt -= ((moveamt * 3) / 4);
        break;
    case EXT_ENCUMBER:
        moveamt -= ((moveamt * 7) / 8);
        break;
    default:
        break;
    }

    return moveamt;
}

void
lock_thread_lock(VOID_ARGS)
{
#if defined(UNIX) && defined(GNH_MOBILE)
    thread_lock_lock();
#else
    return;
#endif
}

void
unlock_thread_lock(VOID_ARGS)
{
#if defined(UNIX) && defined(GNH_MOBILE)
    thread_lock_unlock();
#else
    return;
#endif
}

void
reset_allmain(VOID_ARGS)
{
#ifdef EXTRAINFO_FN
    prev_dgl_extrainfo = 0;
#endif
}


/*allmain.c*/

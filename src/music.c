/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    music.c    $NHDT-Date: 1544442713 2018/12/10 11:51:53 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.57 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * This file contains the different functions designed to manipulate the
 * musical instruments and their various effects.
 *
 * The list of instruments / effects is :
 *
 * (wooden) flute       may calm snakes if player has enough dexterity
 * magic flute          may put monsters to sleep:  area of effect depends
 *                      on player level.
 * (tooled) horn        Will awaken monsters:  area of effect depends on
 *                      player level.  May also scare monsters.
 * fire horn            Acts like a wand of fire.
 * frost horn           Acts like a wand of cold.
 * bugle                Will awaken soldiers (if any):  area of effect depends
 *                      on player level.
 * (wooden) harp        May calm nymph if player has enough dexterity.
 * magic harp           Charm monsters:  area of effect depends on player
 *                      level.
 * (leather) drum       Will awaken monsters like the horn.
 * drum of earthquake   Will initiate an earthquake whose intensity depends
 *                      on player level.  That is, it creates random pits
 *                      called here chasms.
 */

#include "hack.h"

STATIC_DCL void FDECL(awaken_monsters, (int, BOOLEAN_P));
STATIC_DCL void FDECL(put_monsters_to_sleep, (int, int));
STATIC_DCL void FDECL(charm_snakes, (int));
STATIC_DCL void FDECL(calm_nymphs, (int));
STATIC_DCL void FDECL(charm_monsters, (int, int));
STATIC_DCL void FDECL(do_earthquake, (int));
STATIC_DCL int FDECL(do_improvisation, (struct obj *));

#ifdef UNIX386MUSIC
STATIC_DCL int NDECL(atconsole);
STATIC_DCL void FDECL(speaker, (struct obj *, char *));
#endif
#ifdef VPIX_MUSIC
extern int sco_flag_console; /* will need changing if not _M_UNIX */
STATIC_DCL void NDECL(playinit);
STATIC_DCL void FDECL(playstring, (char *, size_t));
STATIC_DCL void FDECL(speaker, (struct obj *, char *));
#endif
#ifdef PCMUSIC
void FDECL(pc_speaker, (struct obj *, char *));
#endif
#ifdef AMIGA
void FDECL(amii_speaker, (struct obj *, char *, int));
#endif

/*
 * Wake every monster in range...
 */

STATIC_OVL void
awaken_monsters(distance, isscary)
int distance;
boolean isscary;
{
    register struct monst *mtmp;
    register int distm;
    int cnt = 0;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue;
        if ((distm = distu(mtmp->mx, mtmp->my)) < distance) 
        {
            mtmp->msleeping = 0;
            mtmp->mcanmove = 1;
            mtmp->mfrozen = 0;
            /* may scare some monsters -- waiting monsters excluded */
            if (!unique_corpstat(mtmp->data)
                && (mtmp->mstrategy & STRAT_WAITMASK) != 0)
                mtmp->mstrategy &= ~STRAT_WAITMASK;
            else if (isscary && distm < distance / 3
                     && !check_ability_resistance_success(mtmp, A_WIS, 0)
                     && !resists_fear(mtmp)
                     /* some monsters are immune */
                     && onscary(0, 0, mtmp))
                monflee(mtmp, 0, FALSE, TRUE);

            refresh_m_tile_gui_info(mtmp, FALSE);
            cnt++;
        }
    }

    if (cnt > 0)
        flush_screen(1);
}

/*
 * Make monsters fall asleep.  Note that they may resist the spell.
 */

STATIC_OVL void
put_monsters_to_sleep(otyp, distance)
int otyp, distance;
{
    register struct monst *mtmp;
    int duration = get_otyp_spell_duration(otyp);

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (distu(mtmp->mx, mtmp->my) < distance
            && sleep_monst(mtmp, (struct obj*)0, (struct monst*)0, duration, 0, FALSE)) {
            //mtmp->msleeping = 1; /* 10d10 turns + wake_nearby to rouse */
            slept_monst(mtmp);
        }
    }

    flush_screen(1);
}

/*
 * Charm snakes in range.  Note that the snakes are NOT tamed.
 */

STATIC_OVL void
charm_snakes(distance)
int distance;
{
    register struct monst *mtmp;
    int could_see_mon, was_peaceful;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->data->mlet == S_SNAKE && mtmp->mcanmove
            && distu(mtmp->mx, mtmp->my) < distance) {
            was_peaceful = mtmp->mpeaceful;
            mtmp->mpeaceful = 1;
            mtmp->mavenge = 0;
            mtmp->mstrategy &= ~STRAT_WAITMASK;
            could_see_mon = canseemon(mtmp);
            mtmp->mundetected = 0;
            newsym(mtmp->mx, mtmp->my);
            if (canseemon(mtmp)) {
                if (!could_see_mon)
                    You("notice %s, swaying with the music.", a_monnam(mtmp));
                else
                    pline("%s freezes, then sways with the music%s.",
                          Monnam(mtmp),
                          was_peaceful ? "" : ", and now seems quieter");
            }
        }
    }
}

/*
 * Calm nymphs in range.
 */

STATIC_OVL void
calm_nymphs(distance)
int distance;
{
    register struct monst *mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->data->mlet == S_NYMPH && mtmp->mcanmove
            && distu(mtmp->mx, mtmp->my) < distance) {
            mtmp->msleeping = 0;
            mtmp->mpeaceful = 1;
            mtmp->mavenge = 0;
            mtmp->mstrategy &= ~STRAT_WAITMASK;
            newsym(mtmp->mx, mtmp->my);
            if (canseemon(mtmp))
                pline(
                    "%s listens cheerfully to the music, then seems quieter.",
                      Monnam(mtmp));
        }
    }
}

/* Awake soldiers anywhere the level (and any nearby monster). */
void
awaken_soldiers(bugler, isscary)
struct monst *bugler; /* monster that played instrument */
boolean isscary;
{
    register struct monst *mtmp;
    int distance, distm, cnt = 0;

    /* distance of affected non-soldier monsters to bugler */
    distance = ((bugler == &youmonst) ? u.ulevel : bugler->data->mlevel) * 30;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (is_mercenary(mtmp->data) && mtmp->data != &mons[PM_GUARD]) {
            mtmp->mpeaceful = mtmp->msleeping = mtmp->mfrozen = 0;
            mtmp->mcanmove = 1;
            mtmp->mstrategy &= ~STRAT_WAITMASK;
            newsym(mtmp->mx, mtmp->my);
            if (canseemon(mtmp))
                pline("%s is now ready for battle!", Monnam(mtmp));
            else
                Norep("You hear the rattle of battle gear being readied.");
        } else if ((distm = ((bugler == &youmonst)
                                 ? distu(mtmp->mx, mtmp->my)
                                 : dist2(bugler->mx, bugler->my, mtmp->mx,
                                         mtmp->my))) < distance) {
            mtmp->msleeping = 0;
            mtmp->mcanmove = 1;
            mtmp->mfrozen = 0;
            refresh_m_tile_gui_info(mtmp, FALSE);
            cnt++;
            /* may scare some monsters -- waiting monsters excluded */
            if (!unique_corpstat(mtmp->data)
                && (mtmp->mstrategy & STRAT_WAITMASK) != 0)
                mtmp->mstrategy &= ~STRAT_WAITMASK;
            else if (isscary && distm < distance / 3 && !check_ability_resistance_success(mtmp, A_WIS, 0))
                monflee(mtmp, 0, FALSE, TRUE);
        }
    }
    if (cnt > 0)
        flush_screen(1);
}

/* Charm monsters in range.  Note that they may resist the spell.
 * If swallowed, range is reduced to 0.
 */
STATIC_OVL void
charm_monsters(distance, saving_throw_adjustment)
int distance, saving_throw_adjustment;
{
    struct monst *mtmp, *mtmp2;

    if (u.uswallow)
    {
        if (!check_ability_resistance_success(u.ustuck, A_WIS, saving_throw_adjustment))
            (void) tamedog(u.ustuck, (struct obj *) 0, TAMEDOG_NO_FORCED_TAMING, TRUE, 200 + rnd(100), TRUE, FALSE);
    }
    else
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp2) 
        {
            mtmp2 = mtmp->nmon;
            if (DEADMONSTER(mtmp))
                continue;

            if (distu(mtmp->mx, mtmp->my) <= distance) 
            {
                if (!check_ability_resistance_success(mtmp, A_WIS, saving_throw_adjustment))
                    (void) tamedog(mtmp, (struct obj *) 0, TAMEDOG_NO_FORCED_TAMING, TRUE, 200 + rnd(100), TRUE, FALSE);
            }
        }
    }
}

/* Generate earthquake :-) of desired force.
 * That is:  create random chasms (pits).
 */
STATIC_OVL void
do_earthquake(force)
int force;
{
    register int x, y;
    struct monst *mtmp;
    struct obj *otmp;
    struct trap *chasm, *trap_at_u = t_at(u.ux, u.uy);
    int start_x, start_y, end_x, end_y;
    schar filltype;
    unsigned tu_pit = 0;

    if (trap_at_u)
        tu_pit = is_pit(trap_at_u->ttyp);
    start_x = u.ux - (force * 2);
    start_y = u.uy - (force * 2);
    end_x = u.ux + (force * 2);
    end_y = u.uy + (force * 2);
    start_x = max(start_x, 1);
    start_y = max(start_y, 0);
    end_x = min(end_x, COLNO - 1);
    end_y = min(end_y, ROWNO - 1);
    for (x = start_x; x <= end_x; x++)
        for (y = start_y; y <= end_y; y++) {
            if ((mtmp = m_at(x, y)) != 0) {
                wakeup(mtmp, TRUE); /* peaceful monster will become hostile */
                if (mtmp->mundetected && is_hider(mtmp->data)) {
                    mtmp->mundetected = 0;
                    if (cansee(x, y))
                        pline("%s is shaken loose from the ceiling!",
                              Amonnam(mtmp));
                    else
                        You_hear("a thumping sound.");
                    if (x == u.ux && y == u.uy)
                        You("easily dodge the falling %s.", mon_nam(mtmp));
                    newsym(x, y);
                }
            }
            if (!rn2(14 - force))
                switch (levl[x][y].typ) {
                case FOUNTAIN: /* Make the fountain disappear */
                    if (cansee(x, y))
                        pline_The("fountain falls into a chasm.");
                    goto do_pit;
                case SINK:
                    if (cansee(x, y))
                        pline_The("kitchen sink falls into a chasm.");
                    goto do_pit;
                case ALTAR:
                    if (Is_astralevel(&u.uz) || Is_sanctum(&u.uz))
                        break;

                    if (cansee(x, y))
                        pline_The("altar falls into a chasm.");
                    goto do_pit;
                case GRAVE:
                    if (cansee(x, y))
                        pline_The("headstone topples into a chasm.");
                    goto do_pit;
                case BRAZIER:
                    if (cansee(x, y))
                        pline_The("brazier falls into a chasm.");
                    goto do_pit;
                case SIGNPOST:
                    if (cansee(x, y))
                        pline_The("signpost falls into a chasm.");
                    goto do_pit;
                case THRONE:
                    if (cansee(x, y))
                        pline_The("throne falls into a chasm.");
                    /*FALLTHRU*/
                case ROOM:
                case CORR: /* Try to make a pit */
                do_pit:
                    chasm = maketrap(x, y, PIT, NON_PM, MKTRAP_NO_FLAGS);
                    if (!chasm)
                        break; /* no pit if portal at that location */
                    chasm->tseen = 1;

                    /* TODO:
                     * This ought to be split into a separate routine to
                     * reduce indentation and the consequent line-wraps.
                     */

                    levl[x][y].doormask = 0;
                    /*
                     * Let liquid flow into the newly created chasm.
                     * Adjust corresponding code in apply.c for
                     * exploding wand of digging if you alter this sequence.
                     */
                    filltype = fillholetyp(x, y, FALSE);
                    if (!IS_FLOOR(filltype)) {
                        delete_location(x, y);
                        levl[x][y].typ = filltype; /* flags set via doormask */
                        liquid_flow(x, y, filltype, chasm, (char *) 0);
                    }

                    mtmp = m_at(x, y);

                    if ((otmp = sobj_at(BOULDER, x, y)) != 0) {
                        if (cansee(x, y))
                            pline("KADOOM!  The boulder falls into a chasm%s!",
                                  (x == u.ux && y == u.uy) ? " below you"
                                                           : "");
                        if (mtmp)
                            mtmp->mtrapped = 0;
                        obj_extract_self(otmp);
                        (void) flooreffects(otmp, x, y, "");
                        break;
                    }

                    /* We have to check whether monsters or player
                       falls in a chasm... */
                    if (mtmp) {
                        if (!is_flyer(mtmp->data)
                            && !is_clinger(mtmp->data)) {
                            boolean m_already_trapped = mtmp->mtrapped;

                            mtmp->mtrapped = 1;
                            if (!m_already_trapped) { /* suppress messages */
                                if (cansee(x, y))
                                    pline("%s falls into a chasm!",
                                          Monnam(mtmp));
                                else if (humanoid(mtmp->data))
                                    You_hear("a scream!");
                            }
                            /* Falling is okay for falling down
                                within a pit from jostling too */
                            mselftouch(mtmp, "Falling, ", TRUE);
                            if (!DEADMONSTER(mtmp)) 
                            {
                                deduct_monster_hp(mtmp, adjust_damage(rnd(m_already_trapped ? 4 : 6), (struct monst*)0, mtmp, AD_PHYS, ADFLAGS_NONE));
                                if (DEADMONSTER(mtmp)) 
                                {
                                    if (!cansee(x, y)) 
                                    {
                                        pline("It is destroyed!");
                                    }
                                    else 
                                    {
                                        You("destroy %s!",
                                            mtmp->mtame
                                              ? x_monnam(mtmp, ARTICLE_THE,
                                                         "poor",
                                                         has_mname(mtmp)
                                                           ? SUPPRESS_SADDLE
                                                           : 0,
                                                         FALSE)
                                              : mon_nam(mtmp));
                                    }
                                    xkilled(mtmp, XKILL_NOMSG);
                                }
                            }
                        }
                    } else if (x == u.ux && y == u.uy) {
                        if (u.utrap && u.utraptype == TT_BURIEDBALL) {
                            /* Note:  the chain should break if a pit gets
                               created at the buried ball's location, which
                               is not necessarily here.  But if we don't do
                               things this way, entering the new pit below
                               will override current trap anyway, but too
                               late to get Lev and Fly handling. */
                            Your("chain breaks!");
                            reset_utrap(TRUE);
                        }
                        if (Levitation || Flying
                            || is_clinger(youmonst.data)) {
                            if (!tu_pit) { /* no pit here previously */
                                pline("A chasm opens up under you!");
                                You("don't fall in!");
                            }
                        } else if (!tu_pit || !u.utrap
                                   || (u.utrap && u.utraptype != TT_PIT)) {
                            /* no pit here previously, or you were
                               not in it even if there was */
                            You("fall into a chasm!");
                            set_utrap(rn1(6, 2), TT_PIT);
                            losehp(adjust_damage(rnd(6), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "fell into a chasm", NO_KILLER_PREFIX);
                            selftouch("Falling, you");
                        } else if (u.utrap && u.utraptype == TT_PIT) {
                            boolean keepfooting =
                                ((Fumbling && !rn2(5))
                                 || (!rnl(Role_if(PM_ARCHAEOLOGIST) ? 3 : 9))
                                 || ((ACURR(A_DEX) > 7) && rn2(5)));

                            You("are jostled around violently!");
                            set_utrap(rn1(6, 2), TT_PIT);
                            losehp(adjust_damage(rnd(keepfooting ? 2 : 4), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), //Maybe_Half_Phys(rnd(keepfooting ? 2 : 4)),
                                   "hurt in a chasm", NO_KILLER_PREFIX);
                            if (keepfooting)
                                exercise(A_DEX, TRUE);
                            else
                                selftouch(
                                    (Upolyd && (slithy(youmonst.data)
                                                || nolimbs(youmonst.data)))
                                        ? "Shaken, you"
                                        : "Falling down, you");
                        }
                    } else
                        newsym(x, y);
                    break;
                case DOOR: /* Make the door collapse */
                    if ((levl[x][y].doormask & D_MASK) == D_NODOOR)
                        goto do_pit;
                    if ((levl[x][y].doormask & D_MASK) == D_PORTCULLIS)
                        break;
                    if (cansee(x, y))
                        pline_The("door collapses.");
                    if (*in_rooms(x, y, SHOPBASE))
                        add_damage(x, y, 0L);
                    levl[x][y].doormask &= ~D_MASK;
                    levl[x][y].doormask |= D_NODOOR;
                    unblock_vision_and_hearing_at_point(x, y);
                    newsym(x, y);
                    break;
                }
        }
}

const char *
generic_lvl_desc()
{
    if (Is_astralevel(&u.uz))
        return "astral plane";
    else if (In_endgame(&u.uz))
        return "plane";
    else if (Is_sanctum(&u.uz))
        return "sanctum";
    else if (In_sokoban(&u.uz))
        return "puzzle";
    else if (In_V_tower(&u.uz))
        return "tower";
    else
        return "dungeon";
}

const char *beats[] = {
    "stepper", "one drop", "slow two", "triple stroke roll",
    "double shuffle", "half-time shuffle", "second line", "train"
};

/*
 * The player is trying to extract something from his/her instrument.
 */
STATIC_OVL int
do_improvisation(instr)
struct obj *instr;
{
    if (!instr)
        return 0;

    double damage = 0;
    int mode, do_spec = !(Stunned || Confusion);
    struct obj itmp;
    boolean mundane = FALSE;

    itmp = *instr;
    itmp.oextra = (struct oextra *) 0; /* ok on this copy as instr maintains
                                          the ptr to free at some point if
                                          there is one */

    /* if won't yield special effect, make sound of mundane counterpart */
    if (!do_spec || instr->charges <= 0)
        while (objects[itmp.otyp].oc_magic) {
            itmp.otyp -= 1;
            mundane = TRUE;
        }
#ifdef MAC
    mac_speaker(&itmp, "C");
#endif
#ifdef AMIGA
    amii_speaker(&itmp, "Cw", AMII_OKAY_VOLUME);
#endif
#ifdef VPIX_MUSIC
    if (sco_flag_console)
        speaker(&itmp, "C");
#endif
#ifdef PCMUSIC
    pc_speaker(&itmp, "C");
#endif

#define PLAY_NORMAL   0x00
#define PLAY_STUNNED  0x01
#define PLAY_CONFUSED 0x02
#define PLAY_HALLU    0x04
    mode = PLAY_NORMAL;
    if (Stunned)
        mode |= PLAY_STUNNED;
    if (Confusion)
        mode |= PLAY_CONFUSED;
    if (Hallucination)
        mode |= PLAY_HALLU;

    enum object_instrument_soundset_types iss = object_soundsets[objects[instr->otyp].oc_soundset].instrument_soundset;

    switch (mode) {
    case PLAY_NORMAL:
        play_immediate_instrument_sound(iss, INSTRUMENT_SOUND_TYPE_IMPROVISE_NORMAL);
        You("start playing %s.", yname(instr));
        break;
    case PLAY_STUNNED:
        play_immediate_instrument_sound(iss, INSTRUMENT_SOUND_TYPE_IMPROVISE_STUNNED);
        You("produce an obnoxious droning sound.");
        break;
    case PLAY_CONFUSED:
        play_immediate_instrument_sound(iss, INSTRUMENT_SOUND_TYPE_IMPROVISE_CONFUSED);
        You("produce a raucous noise.");
        break;
    case PLAY_HALLU:
        play_immediate_instrument_sound(iss, INSTRUMENT_SOUND_TYPE_IMPROVISE_HALLUCINATED);
        You("produce a kaleidoscopic display of floating butterfiles.");
        break;
    /* TODO? give some or all of these combinations their own feedback;
       hallucination ones should reference senses other than hearing... */
    case PLAY_STUNNED | PLAY_CONFUSED:
    case PLAY_STUNNED | PLAY_HALLU:
    case PLAY_CONFUSED | PLAY_HALLU:
    case PLAY_STUNNED | PLAY_CONFUSED | PLAY_HALLU:
    default:
        play_immediate_instrument_sound(iss, INSTRUMENT_SOUND_TYPE_IMPROVISE_FAR_FROM_MUSIC);
        pline("What you produce is quite far from music...");
        break;
    }
#undef PLAY_NORMAL
#undef PLAY_STUNNED
#undef PLAY_CONFUSED
#undef PLAY_HALLU

    switch (itmp.otyp) { /* note: itmp.otyp might differ from instr->otyp */
    case MAGIC_FLUTE: /* Make monster fall asleep */
        consume_obj_charge(instr, TRUE);

        You("produce %s music.", Hallucination ? "piped" : "soft");
        put_monsters_to_sleep(itmp.otyp, u.ulevel * 5);
        exercise(A_DEX, TRUE);
        break;
    case WOODEN_FLUTE: /* May charm snakes */
        do_spec &= (rn2(ACURR(A_DEX)) + u.ulevel > 25);
        pline("%s.", Tobjnam(instr, do_spec ? "trill" : "toot"));
        if (do_spec)
            charm_snakes(u.ulevel * 3);
        exercise(A_DEX, TRUE);
        break;
    case FIRE_HORN:  /* Idem wand of fire */
    case FROST_HORN: /* Idem wand of cold */
        consume_obj_charge(instr, TRUE);

        if (!getdir((char *) 0)) {
            pline("%s.", Tobjnam(instr, "vibrate"));
            break;
        } else if (!u.dx && !u.dy && !u.dz) {
            play_simple_object_sound(instr, OBJECT_SOUND_TYPE_APPLY);
            if ((damage = zapyourself(instr, TRUE)) != 0) {
                char buf[BUFSZ];

                Sprintf(buf, "using a magical horn on %sself", uhim());
                losehp(damage, buf, KILLED_BY); /* fire or frost damage */
            }
        } else {
            update_u_facing(TRUE);
            buzz(objects[instr->otyp].oc_dir_subtype, instr, &youmonst, 0, 0, 0,
                 u.ux, u.uy, u.dx, u.dy);
        }
        makeknown(instr->otyp);
        break;
    case HORN_OF_CHAOS:
    {
        consume_obj_charge(instr, TRUE);
        You("produce a strange, vibrating sound.");
        wake_nearby();
        int dur_dice = objects[instr->otyp].oc_spell_dur_dice;
        int dur_diesize = objects[instr->otyp].oc_spell_dur_diesize;
        int dur_plus = objects[instr->otyp].oc_spell_dur_plus;
        int radius = 10;
        int affected_cnt = 0;
        struct monst* mtmp;
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (!DEADMONSTER(mtmp) && !is_tame(mtmp) && !is_peaceful(mtmp) && !has_mind_shielding(mtmp) && isok(mtmp->mx, mtmp->my) && dist2(mtmp->mx, mtmp->my, u.ux, u.uy) <= radius * radius && hearing_array[mtmp->mx][mtmp->my] > 0.0f)
            {
                if (!check_ability_resistance_success(mtmp, A_WIS, objects[instr->otyp].oc_spell_saving_throw_adjustment))
                {
                    increase_mon_property_b(mtmp, CRAZED, (dur_dice > 0 && dur_diesize > 0 ? d(dur_dice, dur_diesize) : 0) + dur_plus, canspotmon(mtmp));
                    //incr_itimeout(&u.uprops[CRAZED].intrinsic, (dur_dice > 0 && dur_diesize > 0 ? d(dur_dice, dur_diesize) : 0) + dur_plus);
                    affected_cnt++;
                }
            }
        }
        if (affected_cnt)
            You_feel("like a rabble-rouser.");
        else
            You_feel("tense for a moment.");

        break;
    }
    case BRASS_HORN: /* Just make noise and wake nearby monsters */
        You("produce a loud, deep sound.");
        wake_nearby();
        break;
    case TOOLED_HORN: /* Awaken or scare monsters */
    {
        boolean is_scary = FALSE;
        if (instr->charges > 0)
        {
            consume_obj_charge(instr, TRUE);
            is_scary = TRUE;
        }
        You("produce a %sgrave sound.", is_scary ? "frightful, " : "");
        awaken_monsters(u.ulevel * 30, is_scary);
        exercise(A_WIS, FALSE);
        makeknown(instr->otyp);
        break;
    }
    case BUGLE: /* Awaken & attract soldiers */
    {
        boolean is_scary = FALSE;
        if (instr->charges > 0)
        {
            consume_obj_charge(instr, TRUE);
            is_scary = TRUE;
        }
        You("extract a %sloud noise from %s.", is_scary ? "frightful, " : "", yname(instr));
        awaken_soldiers(&youmonst, is_scary);
        exercise(A_WIS, FALSE);
        break;
    }
    case MAGIC_HARP: /* Charm monsters */
        consume_obj_charge(instr, TRUE);

        pline("%s very attractive music.", Tobjnam(instr, "produce"));
        charm_monsters((u.ulevel - 1) / 3 + 1, objects[instr->otyp].oc_spell_saving_throw_adjustment);
        exercise(A_DEX, TRUE);
        break;
    case WOODEN_HARP: /* May calm Nymph */
        do_spec &= (rn2(ACURR(A_DEX)) + u.ulevel > 25);
        pline("%s %s.", Yname2(instr),
              do_spec ? "produces a lilting melody" : "twangs");
        if (do_spec)
            calm_nymphs(u.ulevel * 3);
        exercise(A_DEX, TRUE);
        break;
    case DRUM_OF_EARTHQUAKE: /* create several pits */
        /* a drum of earthquake does not cause deafness
           while still magically functional, nor afterwards
           when it invokes the LEATHER_DRUM case instead and
           mundane is flagged */
        consume_obj_charge(instr, TRUE);

        You("produce a heavy, thunderous rolling!");
        pline_The("entire %s is shaking around you!", generic_lvl_desc());
        do_earthquake((u.ulevel - 1) / 3 + 1);
        /* shake up monsters in a much larger radius... */
        awaken_monsters(ROWNO * COLNO, TRUE);
        makeknown(DRUM_OF_EARTHQUAKE);
        break;
    case LEATHER_DRUM: /* Awaken monsters */
        if (!mundane) 
        {
            You("beat a deafening row!");
            incr_itimeout(&HDeaf, rn1(20, 30));
            refresh_u_tile_gui_info(TRUE);
            play_environment_ambient_sounds();
            exercise(A_WIS, FALSE);
        } 
        else
            You("%s %s.",
                rn2(2) ? "butcher" : rn2(2) ? "manage" : "pull off",
                an(beats[rn2(SIZE(beats))]));

        awaken_monsters(u.ulevel * (mundane ? 5 : 40), FALSE);
        context.botl = TRUE;
        break;
    default:
        impossible("What a weird instrument (%d)!", instr->otyp);
        return 0;
    }
    return 2; /* That takes time */
}

/*
 * So you want music...
 */
int
do_play_instrument(instr)
struct obj *instr;
{
    if (!instr)
        return 0;

    char buf[BUFSZ] = DUMMY, c = 'y';
    char *s;
    int x, y;
    boolean ok;

    if (Underwater) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "play music underwater!");
        return 0;
    }
    else if ((objects[instr->otyp].oc_subtyp == TOOLTYPE_FLUTE || objects[instr->otyp].oc_subtyp == TOOLTYPE_HORN)
               && !can_blow(&youmonst)) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are incapable of playing %s.", the(distant_name(instr, xname)));
        return 0;
    }

    if (objects[instr->otyp].oc_subtyp != TOOLTYPE_DRUM
        && !(Stunned || Confusion || Hallucination)) 
    {
        c = ynq("Improvise?");
        if (c == 'q')
            goto nevermind;
    }

    if (c == 'n') 
    {
        if (u.uevent.uheard_tune == 2)
            c = ynq("Play the passtune?");
        if (c == 'q') 
        {
            goto nevermind;
        } 
        else if (c == 'y') 
        {
            Strcpy(buf, tune);
        } 
        else 
        {
            getlin_ex(GETLINE_TUNE, ATR_NONE, NO_COLOR, "What tune are you playing? ", buf, (char*)0, "[5 notes, A-G]", (char*)0);
            (void) mungspaces(buf);
            if (*buf == '\033')
                goto nevermind;

            /* convert to uppercase and change any "H" to the expected "B" */
            for (s = buf; *s; s++) 
            {
#ifndef AMIGA
                *s = highc(*s);
#else
                /* The AMIGA supports two octaves of notes */
                if (*s == 'h')
                    *s = 'b';
#endif
                if (*s == 'H')
                    *s = 'B';
            }
        }

        if (!iflags.using_gui_sounds)
            You("extract a strange sound from %s!", the(xname(instr)));

        boolean sound_played = FALSE;

#ifdef UNIX386MUSIC
        /* if user is at the console, play through the console speaker */
        if (atconsole())
        {
            sound_played = TRUE;
            speaker(instr, buf);
        }
#endif
#ifdef VPIX_MUSIC
        if (sco_flag_console)
        {
            sound_played = TRUE;
            speaker(instr, buf);
        }
#endif
#ifdef MAC
        sound_played = TRUE;
        mac_speaker(instr, buf);
#endif
#ifdef PCMUSIC
        sound_played = TRUE;
        pc_speaker(instr, buf);
#endif
#ifdef AMIGA
        {
            char nbuf[20];
            int i;

            for (i = 0; buf[i] && i < 5; ++i) 
            {
                nbuf[i * 2] = buf[i];
                nbuf[(i * 2) + 1] = 'h';
            }
            nbuf[i * 2] = 0;
            amii_speaker(instr, nbuf, AMII_OKAY_VOLUME);
            sound_played = TRUE;
        }
#endif
        enum object_instrument_soundset_types iss = object_soundsets[objects[instr->otyp].oc_soundset].instrument_soundset;
        if (iflags.using_gui_sounds && (sound_played || iss == OBJECT_INSTRUMENT_SOUNDSET_NONE))
            You("extract a strange sound from %s!", the(xname(instr)));
        else if (!sound_played && iflags.using_gui_sounds)
        {
            You("start playing %s...", the(xname(instr)));
            for (s = buf; *s; s++)
            {
                enum instrument_sound_types isound = *s >= 'A' && *s <= 'G' ?  INSTRUMENT_SOUND_TYPE_NOTE_A + *s - 'A' : MAX_INSTRUMENT_SOUND_TYPES;
                if (isound < MAX_INSTRUMENT_SOUND_TYPES)
                {
                    play_immediate_instrument_sound(iss, isound);
                    int notelength = (int)instrument_soundsets[iss].note_length_in_milliseconds;
                    if(notelength > 0)
                        delay_output_milliseconds(notelength);
                }
            }
            You("finish playing %s.", the(xname(instr)));
        }

        /* Check if there was the Stronghold drawbridge near
         * and if the tune conforms to what we're waiting for.
         */
        if (Is_stronghold(&u.uz)) 
        {
            exercise(A_WIS, TRUE); /* just for trying */
            if (!strcmp(buf, tune)) 
            {
                /* Search for the drawbridge */
                for (y = u.uy - 1; y <= u.uy + 1; y++)
                    for (x = u.ux - 1; x <= u.ux + 1; x++)
                        if (isok(x, y))
                            if (find_drawbridge(&x, &y)) 
                            {
                                u.uevent.uheard_tune =
                                    2; /* tune now fully known */
                                if (levl[x][y].typ == DRAWBRIDGE_DOWN)
                                    maybe_close_drawbridge(x, y);
                                else
                                    open_drawbridge(x, y);

                                if (!u.uachieve.learned_castle_tune)
                                {
                                    //achievement_gained("Learned Castle Drawbridge Tune");
                                    u.uachieve.learned_castle_tune = 1;
                                }

                                return 1;
                            }
            } 
            else if (!Deaf)
            {
                if (u.uevent.uheard_tune < 1)
                    u.uevent.uheard_tune = 1;
                /* Okay, it wasn't the right tune, but perhaps
                 * we can give the player some hints like in the
                 * Mastermind game */
                ok = FALSE;
                for (y = u.uy - 1; y <= u.uy + 1 && !ok; y++)
                    for (x = u.ux - 1; x <= u.ux + 1 && !ok; x++)
                        if (isok(x, y))
                            if (IS_DRAWBRIDGE(levl[x][y].typ)
                                || is_drawbridge_wall(x, y) >= 0)
                                ok = TRUE;
                
                if (ok)
                { /* There is a drawbridge near */
                    int tumblers, gears;
                    boolean matched[5];

                    tumblers = gears = 0;
                    for (x = 0; x < 5; x++)
                        matched[x] = FALSE;

                    for (x = 0; x < (int)strlen(buf); x++)
                    {
                        if (x < 5)
                        {
                            if (buf[x] == tune[x])
                            {
                                gears++;
                                matched[x] = TRUE;
                                if (iflags.using_gui_sounds)
                                {
                                    play_sfx_sound(SFX_GEAR_TURN);
                                    delay_output_milliseconds(650);
                                }
                            }
                            else
                            {
                                for (y = 0; y < 5; y++)
                                {
                                    if (!matched[y] && buf[x] == tune[y]
                                        && buf[y] != tune[y]) 
                                    {
                                        tumblers++;
                                        matched[y] = TRUE;
                                        if (iflags.using_gui_sounds)
                                        {
                                            play_sfx_sound(SFX_TUMBLER_CLICK);
                                            delay_output_milliseconds(250);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (tumblers)
                    {
                        if (gears)
                            You_hear_ex(ATR_NONE, CLR_MSG_HINT, "%d tumbler%s click and %d gear%s turn.",
                                tumblers, plur(tumblers), gears,
                                plur(gears));
                        else
                            You_hear_ex(ATR_NONE, CLR_MSG_HINT, "%d tumbler%s click.", tumblers,
                                plur(tumblers));
                    }
                    else if (gears)
                    {
                        You_hear_ex(ATR_NONE, CLR_MSG_HINT, "%d gear%s turn.", gears, plur(gears));
                        /* could only get `gears == 5' by playing five
                           correct notes followed by excess; otherwise,
                           tune would have matched above */
                        if (gears == 5)
                            u.uevent.uheard_tune = 2;
                    }
                }
            }
        }
        return 1;
    } 
    else
    {
        return do_improvisation(instr);
    }


nevermind:
    pline1(Never_mind);
    return 0;
}

#ifdef UNIX386MUSIC
/*
 * Play audible music on the machine's speaker if appropriate.
 */

STATIC_OVL int
atconsole()
{
    /*
     * Kluge alert: This code assumes that your [34]86 has no X terminals
     * attached and that the console tty type is AT386 (this is always true
     * under AT&T UNIX for these boxen). The theory here is that your remote
     * ttys will have terminal type `ansi' or something else other than
     * `AT386' or `xterm'. We'd like to do better than this, but testing
     * to see if we're running on the console physical terminal is quite
     * difficult given the presence of virtual consoles and other modern
     * UNIX impedimenta...
     */
    char *termtype = nh_getenv("TERM");

    return (!strcmp(termtype, "AT386") || !strcmp(termtype, "xterm"));
}

STATIC_OVL void
speaker(instr, buf)
struct obj *instr;
char *buf;
{
    /*
     * For this to work, you need to have installed the PD speaker-control
     * driver for PC-compatible UNIX boxes that I (esr@snark.thyrsus.com)
     * posted to comp.sources.unix in Feb 1990.  A copy should be included
     * with your GnollHack distribution.
     */
    int fd;

    if ((fd = open("/dev/speaker", 1)) != -1) {
        /* send a prefix to modify instrumental `timbre' */
        switch (instr->otyp) {
        case WOODEN_FLUTE:
        case MAGIC_FLUTE:
            (void) write(fd, ">ol", 1); /* up one octave & lock */
            break;
        case BRASS_HORN:
        case TOOLED_HORN:
        case FROST_HORN:
        case FIRE_HORN:
            (void) write(fd, "<<ol", 2); /* drop two octaves & lock */
            break;
        case BUGLE:
            (void) write(fd, "ol", 2); /* octave lock */
            break;
        case WOODEN_HARP:
        case MAGIC_HARP:
            (void) write(fd, "l8mlol", 4); /* fast, legato, octave lock */
            break;
        }
        (void) write(fd, buf, strlen(buf));
        (void) nhclose(fd);
    }
}
#endif /* UNIX386MUSIC */

#ifdef VPIX_MUSIC

#if 0
#include <sys/types.h>
#include <sys/console.h>
#include <sys/vtkd.h>
#else
#define KIOC ('K' << 8)
#define KDMKTONE (KIOC | 8)
#endif

#define noDEBUG

/* emit tone of frequency hz for given number of ticks */
STATIC_OVL void
tone(hz, ticks)
unsigned int hz, ticks;
{
    ioctl(0, KDMKTONE, hz | ((ticks * 10) << 16));
#ifdef DEBUG
    printf("TONE: %6d %6d\n", hz, ticks * 10);
#endif
    nap(ticks * 10);
}

/* rest for given number of ticks */
STATIC_OVL void
rest(ticks)
int ticks;
{
    nap(ticks * 10);
#ifdef DEBUG
    printf("REST:        %6d\n", ticks * 10);
#endif
}

#include "interp.c" /* from snd86unx.shr */

STATIC_OVL void
speaker(instr, buf)
struct obj *instr;
char *buf;
{
    /* emit a prefix to modify instrumental `timbre' */
    playinit();
    switch (instr->otyp) {
    case WOODEN_FLUTE:
    case MAGIC_FLUTE:
        playstring(">ol", 1); /* up one octave & lock */
        break;
    case BRASS_HORN:
    case TOOLED_HORN:
    case FROST_HORN:
    case FIRE_HORN:
        playstring("<<ol", 2); /* drop two octaves & lock */
        break;
    case BUGLE:
        playstring("ol", 2); /* octave lock */
        break;
    case WOODEN_HARP:
    case MAGIC_HARP:
        playstring("l8mlol", 4); /* fast, legato, octave lock */
        break;
    }
    playstring(buf, strlen(buf));
}

#ifdef VPIX_DEBUG
main(argc, argv)
int argc;
char *argv[];
{
    if (argc == 2) {
        playinit();
        playstring(argv[1], strlen(argv[1]));
    }
}
#endif
#endif /* VPIX_MUSIC */

/*music.c*/

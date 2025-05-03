/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    potion.c    $NHDT-Date: 1549074254 2019/02/02 02:24:14 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.160 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2013. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

boolean notonhead = FALSE;

STATIC_VAR NEARDATA int nothing, unkn;
STATIC_VAR NEARDATA const char beverages[] = { POTION_CLASS, TOOL_CLASS, 0 };

STATIC_DCL int64_t FDECL(itimeout, (int64_t));
STATIC_DCL void NDECL(ghost_from_bottle);
STATIC_DCL short FDECL(mixtype, (struct obj *, struct obj *));
STATIC_DCL short FDECL(mixtype_core, (struct obj*, struct obj*));

/* force `val' to be within valid range for intrinsic timeout value */
STATIC_OVL int64_t
itimeout(val)
int64_t val;
{
    if (val >= TIMEOUT)
        val = TIMEOUT;
    else if (val < 1)
        val = 0;

    return val;
}

/* increment `old' by `incr' and force result to be valid intrinsic timeout */
int64_t
itimeout_incr(old, incr)
int64_t old;
int incr;
{
    return itimeout((old & TIMEOUT) + (int64_t) incr);
}

/* set the timeout field of intrinsic `which' */
void
set_itimeout(which, val)
int64_t *which, val;
{
    *which &= ~TIMEOUT;
    *which |= itimeout(val);
}

/* increment the timeout field of intrinsic `which' */
void
incr_itimeout(which, incr)
int64_t *which;
int incr;
{
    set_itimeout(which, itimeout_incr(*which, incr));
}

void
make_confused(xtime, talk)
int64_t xtime;
boolean talk;
{
    int64_t old = HConfusion;

    if (Unaware)
        talk = FALSE;

    if (!xtime && old) {
        if (talk)
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "less %s now.", Hallucination ? "trippy" : "confused");
    }

    set_itimeout(&HConfusion, xtime);

    if ((xtime && !old) || (!xtime && old))
    {
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
    }

}

void
make_stunned(xtime, talk)
int64_t xtime;
boolean talk;
{
    int64_t old = HStun;

    if (Unaware)
        talk = FALSE;

    if (!xtime && old) 
    {
        if (talk)
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s now.",
                     Hallucination ? "less wobbly" : "a bit steadier");
    }

    if (xtime && !old) 
    {
        if (talk)
        {
            if (u.usteed)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "wobble in the saddle.");
            else
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s...", stagger(youmonst.data, "stagger"));
        }

        standard_hint("You can cure stun by using a unicorn horn.", &u.uhint.got_stunned);
    }

    set_itimeout(&HStun, xtime);

    if ((!xtime && old) || (xtime && !old))
    {
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
    }
}


/* Sick is overloaded with both fatal illness and food poisoning (via
   u.usick_type bit mask), but delayed killer can only support one or
   the other at a time.  They should become separate intrinsics.... */
void
make_sick(xtime, cause, talk, killerhintidx)
int64_t xtime;
const char *cause; /* sickness cause */
boolean talk;
int killerhintidx;
{
    struct kinfo *kptr;
    int64_t old = Sick;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        talk = FALSE;
#endif

    if (xtime > 0L) 
    {
        if (Sick_resistance)
            return;
    
        if (!old) 
        {
            /* newly sick */
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "deathly sick.");
            pray_hint("cure terminal illness", "eating an eucalyptus leaf or a fig", &u.uhint.got_terminal_illness);
        }
        else
        {
            /* already sick */
            if (talk)
                You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s worse.", xtime <= Sick / 2L ? "much" : "even");
        }
        set_itimeout(&Sick, xtime);
        context.botl = context.botlx = TRUE;
        if (!old)
            refresh_u_tile_gui_info(TRUE);
    }
    else if (old)
    {
        /* was sick, now not */
        if (talk)
        {
            if (FoodPoisoned || MummyRot)
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "somewhat better.");
            else
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "cured.  What a relief!");
        }
        Sick = 0L; /* set_itimeout(&Sick, 0L) */
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
    }

    kptr = find_delayed_killer(SICK);
    if (Sick)
    {
        exercise(A_CON, FALSE);
        /* setting delayed_killer used to be unconditional, but that's
           not right when make_sick(0) is called to cure food poisoning
           if hero was also fatally ill; this is only approximate */
        if (xtime || !old || !kptr) 
        {
            int kpfx = ((cause && !strcmp(cause, "#wizintrinsic"))
                        ? KILLED_BY : KILLED_BY_AN);

            delayed_killer(SICK, kpfx, cause, killerhintidx);
        }
    } 
    else
        dealloc_killer(kptr);
}


/* Sick is overloaded with both fatal illness and food poisoning (via
   u.usick_type bit mask), but delayed killer can only support one or
   the other at a time.  They should become separate intrinsics.... */
void
make_food_poisoned(xtime, cause, talk, killerhintidx)
int64_t xtime;
const char* cause; /* sickness cause */
boolean talk;
int killerhintidx;
{
    struct kinfo* kptr;
    int64_t old = FoodPoisoned;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        talk = FALSE;
#endif

    if (xtime > 0L)
    {
        if (Sick_resistance)
            return;

        if (!old)
        {
            /* newly sick */
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "terminally ill from food poisoning.");
            pray_hint("cure food poisoning", "eating an eucalyptus leaf or a fig", &u.uhint.got_food_poisoning);
        }
        else
        {
            /* already sick */
            if (talk)
                You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s worse.", xtime <= FoodPoisoned / 2L ? "much" : "even");
        }
        set_itimeout(&FoodPoisoned, xtime);
        context.botl = context.botlx = TRUE;
        if(!old)
            refresh_u_tile_gui_info(TRUE);
    }
    else if (old)
    {
        /* was sick, now not */
        if (talk)
        {
            if (Sick || MummyRot)
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "somewhat better.");
            else
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "cured.  What a relief!");
        }
        FoodPoisoned = 0L;
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
    }

    kptr = find_delayed_killer(FOOD_POISONED);
    if (FoodPoisoned)
    {
        exercise(A_CON, FALSE);
        /* setting delayed_killer used to be unconditional, but that's
           not right when make_sick(0) is called to cure food poisoning
           if hero was also fatally ill; this is only approximate */
        if (xtime || !old || !kptr)
        {
            int kpfx = ((cause && !strcmp(cause, "#wizintrinsic"))
                ? KILLED_BY : KILLED_BY_AN);

            delayed_killer(FOOD_POISONED, kpfx, cause, killerhintidx);
        }
    }
    else
        dealloc_killer(kptr);
}


/* xtime = -1 makes it a permanent intrinsic */

void
make_mummy_rotted(xtime, cause, talk, killerhintidx)
int64_t xtime;
const char* cause; /* sickness cause */
boolean talk;
int killerhintidx;
{
    struct kinfo* kptr;
    int64_t old = MummyRot;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        talk = FALSE;
#endif

    if (xtime != 0L)
    {
        if (Sick_resistance)
            return;

        if (!old)
        {
            /* newly sick */
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "severely ill.");
            context.mummyrot_advancement = 0;
            pray_hint("cure mummy rot", "eating an eucalyptus leaf or a fig", &u.uhint.got_mummy_rot);
        }
        else
        {
            /* already sick */
            if (talk)
                You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "even worse.");
        }

        if (xtime > 0L)
            set_itimeout(&MummyRot, xtime);
        else
            MummyRot |= FROM_ACQUIRED;

        context.botl = context.botlx = TRUE;
        if (!old)
            refresh_u_tile_gui_info(TRUE);
    }
    else if (old)
    {
        /* was sick, now not */
        if (talk)
        {
            if (Sick || FoodPoisoned)
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "somewhat better.");
            else
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "cured.  What a relief!");
        }
        MummyRot = 0L;
        context.mummyrot_advancement = 0;
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
    }

    kptr = find_delayed_killer(MUMMY_ROT);
    if (MummyRot)
    {
        exercise(A_CON, FALSE);
        /* setting delayed_killer used to be unconditional, but that's
           not right when make_sick(0) is called to cure food poisoning
           if hero was also fatally ill; this is only approximate */
        if (xtime || !old || !kptr)
        {
            int kpfx = ((cause && !strcmp(cause, "#wizintrinsic"))
                ? KILLED_BY : KILLED_BY_AN);

            delayed_killer(MUMMY_ROT, kpfx, cause, killerhintidx);
        }
    }
    else
        dealloc_killer(kptr);
}

void
make_slimed(xtime, msg, killedby, killername, killerhintidx)
int64_t xtime;
const char *msg;
int killedby;
const char* killername;
int killerhintidx;
{
    int64_t old = Slimed;
    struct kinfo* kptr;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        msg = 0;
#endif
    set_itimeout(&Slimed, xtime);
    if ((xtime != 0L) ^ (old != 0L)) 
    {
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (msg)
            pline_ex(ATR_NONE, !xtime ? CLR_MSG_POSITIVE : CLR_MSG_NEGATIVE, "%s", msg);
    }

    kptr = find_delayed_killer(SLIMED);
    if (!Slimed)
        dealloc_killer(kptr);
    else
    {
        if (!old || !kptr)
            delayed_killer(SLIMED, killedby, killername, killerhintidx);

        if (!old)
            pray_hint("cure sliming", "burning it with fire", &u.uhint.got_sliming);
    }
}

/* start or stop petrification */
void
make_stoned(xtime, msg, killedby, killername, killhintidx)
int64_t xtime;
const char *msg;
int killedby, killhintidx;
const char *killername;
{
    int64_t old = Stoned;
    struct kinfo* kptr;

#if 0   /* tell player even if hero is unconscious */
    if (Unaware)
        msg = 0;
#endif
    set_itimeout(&Stoned, xtime);
    if ((xtime != 0L) ^ (old != 0L)) 
    {
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (msg)
            pline_ex(ATR_NONE, !xtime ? CLR_MSG_POSITIVE : CLR_MSG_NEGATIVE, "%s", msg);
    }

    kptr = find_delayed_killer(STONED);
    if (!Stoned)
        dealloc_killer(kptr);
    else
    {
        if (!old || !kptr)
            delayed_killer(STONED, killedby, killername, killhintidx);

        if (!old)
            pray_hint("cure stoning", "eating a lizard corpse or a dragon fruit", &u.uhint.got_stoning);
    }
}

void
make_vomiting(xtime, talk)
int64_t xtime;
boolean talk;
{
    int64_t old = Vomiting;

    if (Unaware)
        talk = FALSE;

    set_itimeout(&Vomiting, xtime);
    refresh_u_tile_gui_info(TRUE);
    context.botl = context.botlx = TRUE;
    if (!xtime && old)
        if (talk)
            You_feel_ex(ATR_NONE, !xtime ? CLR_MSG_POSITIVE : CLR_MSG_NEGATIVE, "much less nauseated now.");
}

STATIC_VAR const char vismsg[] = "vision seems to %s for a moment but is %s now.";
STATIC_VAR const char eyemsg[] = "%s momentarily %s.";

void
make_blinded(xtime, talk)
int64_t xtime;
boolean talk;
{
    int64_t old = Blinded;
    boolean u_could_see, can_see_now;
    const char *eyes;

    /* we need to probe ahead in case the Eyes of the Overworld
       are or will be overriding blindness */
    u_could_see = !Blind;
    Blinded = xtime ? 1L : 0L;
    can_see_now = !Blind;
    Blinded = old; /* restore */

    if (Unaware)
        talk = FALSE;

    if (can_see_now && !u_could_see) { /* regaining sight */
        if (talk) 
        {
            if (Hallucination)
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Far out!  Everything is all cosmic again!");
            else
                You_ex(ATR_NONE, CLR_MSG_POSITIVE, "can see again.");
        }
    } else if (old && !xtime) {
        /* clearing temporary blindness without toggling blindness */
        if (talk) {
            if (!haseyes(youmonst.data)) {
                strange_feeling((struct obj *) 0, (char *) 0, FALSE);
            } else if (Blindfolded) {
                eyes = body_part(EYE);
                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Your(eyemsg, eyes, vtense(eyes, "itch"));
            } else { /* Eyes of the Overworld */
                Your(vismsg, "brighten", Hallucination ? "sadder" : "normal");
            }
        }
    }

    if (u_could_see && !can_see_now) { /* losing sight */
        if (talk) {
            if (Hallucination)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "Oh, bummer!  Everything is dark!  Help!");
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "A cloud of darkness falls upon you.");
        }
        /* Before the hero goes blind, set the ball&chain variables. */
        if (Punished)
            set_bc(0);
    } else if (!old && xtime) {
        /* setting temporary blindness without toggling blindness */
        if (talk) {
            if (!haseyes(youmonst.data)) {
                strange_feeling((struct obj *) 0, (char *) 0, FALSE);
            } else if (Blindfolded) {
                eyes = body_part(EYE);
                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Your(eyemsg, eyes, vtense(eyes, "twitch"));
            } else { /* Eyes of the Overworld */
                Your(vismsg, "dim", Hallucination ? "happier" : "normal");
            }
        }
    }

    set_itimeout(&Blinded, xtime);

    if (u_could_see ^ can_see_now) { /* one or the other but not both */
        toggle_blindness();
        refresh_u_tile_gui_info(TRUE);
    }
}

/* blindness has just started or just ended--caller enforces that;
   called by Blindf_on(), Blindf_off(), and make_blinded() */
void
toggle_blindness()
{
    /* Note this is a kludge that works only with wielded weapons; */
    /* blindness has just been toggled */
    context.botl = context.botlx = TRUE;
    /* status conditions need update */
    vision_full_recalc = 1; /* vision has changed */
    /* this vision recalculation used to be deferred until moveloop(),
       but that made it possible for vision irregularities to occur
       (cited case was force bolt hitting an adjacent potion of blindness
       and then a secret door; hero was blinded by vapors but then got the
       message "a door appears in the wall" because wall spot was IN_SIGHT) */
    vision_recalc(0);
    if (Blind_telepat || Unblind_telepat || Infravision || Warn_of_mon || Warning || Any_warning)
        see_monsters(); /* also counts EWarn_of_mon monsters */

    flush_screen(1);

    /*
     * Avoid either of the sequences
     * "Sting starts glowing", [become blind], "Sting stops quivering" or
     * "Sting starts quivering", [regain sight], "Sting stops glowing"
     * by giving "Sting is quivering" when becoming blind or
     * "Sting is glowing" when regaining sight so that the eventual
     * "stops" message matches the most recent "Sting is ..." one.
     */    

    struct obj* uitem;
    for (uitem = invent; uitem; uitem = uitem->nobj)
    {
        if (!object_uses_spellbook_wand_flags_and_properties(uitem))
        {
            int otyp = uitem->otyp;
            boolean inappr = inappropriate_character_type(uitem);
            boolean worn = is_obj_worn(uitem);

            if ((((worn || (!worn && (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_WHEN_CARRIED)))
                && ((!inappr && !(objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    || (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_TO_ALL_CHARACTERS)
                    || (inappr && (objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    )) 
                    && (
                        ((objects[otyp].oc_flags & O1_FLICKERS_WHEN_MONSTERS_DETECTED) || artifact_has_flag(uitem, AF_FLICKERS) || artifact_has_flag(uitem, AF_FLICKERS_WHEN_CARRIED))
                        && item_has_specific_monster_warning(uitem)
                       )
                )
                )
            {
                Sting_effects(uitem, -1);
            }
        }
    }

    if (!Blind)
    {
        /* update dknown flag for inventory picked up while blind */
        learn_unseen_invent();
        /* check if a boss monster has appeared into view */
        check_seen_bosses();
    }
}

boolean
make_hallucinated(xtime, talk, mask)
int64_t xtime; /* nonzero if this is an attempt to turn on hallucination */
boolean talk;
int64_t mask; /* nonzero if resistance status should change by mask */
{
    int64_t old = HHallucination;
    boolean changed = 0;
    const char *message, *verb;

    if (Unaware)
        talk = FALSE;

    message = (!xtime) ? "Everything %s SO boring now."
                       : "Oh wow!  Everything %s so cosmic!";
    verb = (!Blind) ? "looks" : "feels";

    if (mask) {
        if (HHallucination)
            changed = TRUE;

        if (!xtime)
            EHalluc_resistance |= mask;
        else
            EHalluc_resistance &= ~mask;
    } else {
        if (!EHalluc_resistance && (!!HHallucination != !!xtime))
            changed = TRUE;
        set_itimeout(&HHallucination, xtime);

        /* clearing temporary hallucination without toggling vision */
        if (!changed && !HHallucination && old && talk) {
            if (!haseyes(youmonst.data)) {
                strange_feeling((struct obj *) 0, (char *) 0, FALSE);
            } else if (Blind) {
                const char *eyes = body_part(EYE);

                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, eyemsg, eyes, vtense(eyes, "itch"));
            } else { /* Grayswandir */
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, vismsg, "flatten", "normal");
            }
        }
    }

    if (changed) {
        /* in case we're mimicking an orange (hallucinatory form
           of mimicking gold) update the mimicking's-over message */
        if (!Hallucination)
            eatmupdate();

        if (u.uswallow) {
            swallowed(0); /* redraw swallow display */
        } else {
            /* The see_* routines should be called *before* the pline. */
            see_monsters();
            see_objects();
            see_traps();
        }

        /* for perm_inv and anything similar
        (eg. Qt windowport's equipped items display) */
        update_inventory();

        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        if (talk)
            pline_ex(ATR_NONE, !xtime ? CLR_MSG_POSITIVE : CLR_MSG_HALLUCINATED, message, verb);
    }

    return changed;
}

void
make_deaf(xtime, talk)
int64_t xtime;
boolean talk;
{
    int64_t old = HDeaf;

    if (Unaware)
        talk = FALSE;

    set_itimeout(&HDeaf, xtime);

    if ((xtime != 0L) ^ (old != 0L)) 
    {
        context.botl = context.botlx = TRUE;
        update_hearing_array_and_ambient_sounds();
        play_environment_ambient_sounds();
        refresh_u_tile_gui_info(TRUE);
        if (talk)
            You_ex(ATR_NONE, !xtime ? CLR_MSG_POSITIVE : CLR_MSG_WARNING, old ? "can hear again." : "are unable to hear anything.");
    }
}

void
self_invis_message()
{
    pline("%s %s.",
          Hallucination ? "Far out, man!  You"
                        : "Gee!  All of a sudden, you",
          See_invisible ? "can see right through yourself"
                        : "can't see yourself");
}

STATIC_OVL void
ghost_from_bottle()
{
    struct monst *mtmp = makemon(&mons[PM_GHOST], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);

    if (!mtmp) {
        pline("This bottle turns out to be empty.");
        return;
    }
    play_sfx_sound(SFX_SURPRISE_ATTACK);
    if (Blind) {
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "As you open the bottle, %s emerges.", something);
        return;
    }
    pline_ex(ATR_NONE, CLR_MSG_WARNING, "As you open the bottle, an enormous %s emerges!",
          Hallucination ? rndmonnam(NULL) : (const char *) "ghost");
    if (flags.verbose)
        You_ex(ATR_NONE, CLR_MSG_WARNING, "are frightened to death, and unable to move.");
    nomul(-3);
    multi_reason = "being frightened to death";
    nomovemsg = "You regain your composure.";
}

/* "Quaffing is like drinking, except you spill more." - Terry Pratchett */
int
dodrink()
{
    struct obj *otmp;
    const char *potion_descr;
    boolean action_taken = FALSE;

    if (Strangled) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "If you can't breathe air, how can you drink liquid?");
        return 0;
    }
    if (!getobj_autoselect_obj)
    {
        /* Is there a fountain to drink from here? */
        if (IS_FOUNTAIN(levl[u.ux][u.uy].typ)
            /* not as low as floor level but similar restrictions apply */
            && can_reach_floor(FALSE)) 
        {
            char qbuf[BUFSZ];
            Sprintf(qbuf, "Drink from the %s?", get_fountain_name(u.ux, u.uy));

            if (yn_query(qbuf) == 'y') 
            {
                drinkfountain();
                return 1;
            }
        }
        /* Or a kitchen sink? */
        if (IS_SINK(levl[u.ux][u.uy].typ)
            /* not as low as floor level but similar restrictions apply */
            && can_reach_floor(FALSE)) {
            if (yn_query("Drink from the sink?") == 'y') {
                drinksink();
                return 1;
            }
        }
        /* Or are you surrounded by water? */
        if (Underwater && !u.uswallow) {
            if (yn_query("Drink the water around you?") == 'y') {
                pline("Do you know what lives in this water?");
                return 1;
            }
        }
    }

    otmp = getobj(beverages, "drink", 0, "");
    if (!otmp)
        return 0;

    if (otmp->oclass == TOOL_CLASS)
    {
        switch (otmp->otyp)
        {
        case GRAIL_OF_HEALING:
            return use_grail(otmp, TRUE);
        case JAR_OF_BASILISK_BLOOD:
            return use_salve(otmp, TRUE);
        default:
            play_sfx_sound(SFX_GENERAL_THATS_SILLY);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "That's a silly thing to drink!");
            return 0;
        }
    }

    if (Race_if(PM_GNOLL))
    {
        if (otmp->otyp == POT_SICKNESS || otmp->otyp == POT_POISON || otmp->otyp == POT_URINE)
        {
            char qbuf[BUFSZ];
            Sprintf(qbuf, "This potion smells %s. Drink it?",
                otmp->otyp == POT_SICKNESS ? "contaminated" : otmp->otyp == POT_POISON ? "like poison" : otmp->otyp == POT_URINE ? "like urine" : "foul");

            char qres = yn_query_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Warning", qbuf);

            if (qres != 'y')
                return 0;
        }
    }

    /* quan > 1 used to be left to useup(), but we need to force
       the current potion to be unworn, and don't want to do
       that for the entire stack when starting with more than 1.
       [Drinking a wielded potion of polymorph can trigger a shape
       change which causes hero's weapon to be dropped.  In 3.4.x,
       that led to an "object lost" panic since subsequent useup()
       was no longer dealing with an inventory item.  Unwearing
       the current potion is intended to keep it in inventory.] */
    if (otmp->quan > 1L) 
    {
        otmp = splitobj(otmp, 1L);
        otmp->owornmask = 0L; /* rest of original stuck unaffected */
    }
    else if (otmp->owornmask) 
    {
        remove_worn_item(otmp, FALSE);
    }

    otmp->in_use = TRUE; /* you've opened the stopper */

    if (u_item_use_flags() & ACTION_ITEM_USE_FLAGS_POTION)
    {
        action_taken = TRUE;
        update_u_action(ACTION_TILE_ITEM_USE);
        u_wait_until_action();
    }

    struct monst* shkp = 0;
    boolean billable_potion = FALSE;
    boolean gone = FALSE;

    if (otmp->unpaid && costly_spot(u.ux, u.uy))
    {
        char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
        shkp = shop_keeper(*o_shop);
        if (shkp && inhishop(shkp) && is_obj_on_shk_bill(otmp, shkp))
        {
            billable_potion = TRUE;
        }
    }

    potion_descr = OBJ_DESCR(objects[otmp->otyp]);
    if (potion_descr)
    {
        if (!strcmp(potion_descr, "milky")
            && !(mvitals[PM_GHOST].mvflags & MV_GONE)
            && !rn2(POTION_OCCUPANT_CHANCE(mvitals[PM_GHOST].born)))
        {
            ghost_from_bottle();
            useup(otmp);
            gone = TRUE;
            if (action_taken)
                update_u_action_revert(ACTION_TILE_NO_ACTION);
            goto check_add_to_bill_here;

        }
        else if (!strcmp(potion_descr, "smoky")
                   && !(mvitals[PM_DJINNI].mvflags & MV_GONE)
                   && ((otmp->speflags & SPEFLAGS_CERTAIN_WISH) != 0 || !rn2(POTION_OCCUPANT_CHANCE(mvitals[PM_DJINNI].born))))
        {
            djinni_from_bottle(otmp);
            useup(otmp);
            gone = TRUE;
            if (action_taken)
                update_u_action_revert(ACTION_TILE_NO_ACTION);
            goto check_add_to_bill_here;
        }
    }

    int res = dopotion(otmp);
    if (action_taken)
    {
        u_wait_until_end();
        update_u_action_revert(ACTION_TILE_NO_ACTION);
    }
    return res;

check_add_to_bill_here:
    if (gone && billable_potion && shkp && !Deaf && !muteshk(shkp))
        play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_ILL_ADD_THAT_TO_YOUR_BILL);
    return 1;
}

int
dopotion(otmp)
struct obj *otmp;
{
    int retval;
    struct monst* shkp = 0;
    boolean billable_potion = FALSE;
    boolean gone = FALSE;

    if (otmp->unpaid && costly_spot(u.ux, u.uy))
    {
        char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
        shkp = shop_keeper(*o_shop);
        if (shkp && inhishop(shkp) && is_obj_on_shk_bill(otmp, shkp))
        {
            billable_potion = TRUE;
        }
    }

    play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_QUAFF);

    otmp->in_use = TRUE;
    nothing = unkn = 0;
    if ((retval = peffects(otmp)) >= 0)
    {
        if (retval == 1)
        {
            gone = TRUE;
            goto check_billable_potion_here;
        }
        else
            return retval;
    }

    if (nothing)
    {
        unkn++;
        You("have a %s feeling for a moment, then it passes.",
            Hallucination ? "normal" : "peculiar");
    }

    if (otmp->dknown && !objects[otmp->otyp].oc_name_known)
    {
        if (!unkn) 
        {
            makeknown(otmp->otyp);
            //more_experienced(0, 10);
        }
        else if (!objects[otmp->otyp].oc_uname)
            docall(otmp, (char*)0);
    }

    useup(otmp);
    gone = TRUE;

check_billable_potion_here:
    if (gone && billable_potion && shkp && !Deaf && !muteshk(shkp))
        play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_ILL_ADD_THAT_TO_YOUR_BILL);
    return 1;
}

int
peffects(otmp)
struct obj *otmp;
{
    int i, ii, lim;
    
    if (!otmp)
        return 0;

    int dicebuc = (int)(otmp->oclass == POTION_CLASS ? objects[otmp->otyp].oc_potion_normal_dice_buc_multiplier : 0);
    int duration =
        (int)max(0, 
            otmp->oclass == POTION_CLASS ? (objects[otmp->otyp].oc_potion_normal_diesize == 0 ? 0 : d(max(0, objects[otmp->otyp].oc_potion_normal_dice + dicebuc * bcsign(otmp)), max(1, objects[otmp->otyp].oc_potion_normal_diesize))) + objects[otmp->otyp].oc_potion_normal_plus + bcsign(otmp) * objects[otmp->otyp].oc_potion_normal_buc_multiplier :
            d(objects[otmp->otyp].oc_spell_dur_dice, objects[otmp->otyp].oc_spell_dur_diesize) + objects[otmp->otyp].oc_spell_dur_plus
           );
    int nutrdicebuc = (int)(otmp->oclass == POTION_CLASS ? objects[otmp->otyp].oc_potion_nutrition_dice_buc_multiplier : 0);
    int nutrition = (int)(
        otmp->oclass == POTION_CLASS ? d(max(0, objects[otmp->otyp].oc_potion_nutrition_dice + nutrdicebuc * bcsign(otmp)), max(1, objects[otmp->otyp].oc_potion_nutrition_diesize)) + objects[otmp->otyp].oc_potion_nutrition_plus + bcsign(otmp) * objects[otmp->otyp].oc_potion_nutrition_buc_multiplier :
        0);

    int extra_data1 = otmp->oclass == POTION_CLASS ? (int)objects[otmp->otyp].oc_potion_extra_data1 : 0;

    /* Adjustment for dilution */
    boolean isdiluted = otmp->oclass == POTION_CLASS && otmp->odiluted;
    if (isdiluted)
    {
        nutrition /= 2;
        duration /= 2;
        extra_data1 /= 2;
    }

    boolean cures_sick = FALSE;
    boolean cures_blind = FALSE;
    boolean cures_hallucination = FALSE;
    boolean cures_stun = FALSE;
    boolean cures_confusion = FALSE;
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

    switch (otmp->otyp) {
    case POT_RESTORE_ABILITY:
    case SPE_RESTORE_ABILITY:
        unkn++;
        if (otmp->cursed) {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Ulch!  This makes you feel mediocre!");
            break;
        } else {
            /* unlike unicorn horn, overrides Fixed_abil */
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_RESTORE_ABILITY);
            special_effect_wait_until_action(0);
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Wow!  This makes you feel %s!",
                  (otmp->blessed)
                      ? (unfixable_trouble_count(FALSE) ? "better" : "great")
                      : "good");
            i = rn2(A_MAX); /* start at a random point */
            for (ii = 0; ii < A_MAX; ii++) {
                lim = AMAX(i);
                /* this used to adjust 'lim' for A_STR when u.uhs was
                   WEAK or worse, but that's handled via ATEMP(A_STR) now */
                if (ABASE(i) < lim) {
                    ABASE(i) = lim;
                    context.botl = 1;
                    /* only first found if not blessed */
                    if (!otmp->blessed || isdiluted)
                        break;
                }
                if (++i >= A_MAX)
                    i = 0;
            }

            /* when using the potion (not the spell) also restore lost levels,
               to make the potion more worth keeping around for players with
               the spell or with a unihorn; this is better than full healing
               in that it can restore all of them, not just half, and a
               blessed potion restores them all at once */
            if (otmp->otyp == POT_RESTORE_ABILITY)
            {
                if (u.ulevel < u.ulevelmax) {
                    do {
                        pluslvl(FALSE);
                    } while (u.ulevel < u.ulevelmax && otmp->blessed);
                }
                if (u.ubasehpdrain < 0) {
                    if (otmp->blessed)
                        u.ubasehpdrain = 0;
                    else
                        u.ubasehpdrain += min(4, -u.ubasehpdrain);
                    updatemaxhp();
                    context.botl = context.botlx = 1;
                }
                if (u.basemhdrain < 0) {
                    if (otmp->blessed)
                        u.basemhdrain = 0;
                    else
                        u.basemhdrain += min(4, -u.basemhdrain);
                    updatemaxhp();
                    context.botl = context.botlx = 1;
                }
                if (u.ubaseendrain < 0) {
                    if (otmp->blessed)
                        u.ubaseendrain = 0;
                    else
                        u.ubaseendrain += min(4, -u.ubaseendrain);
                    updatemaxen();
                    context.botl = context.botlx = 1;
                }
            }
            special_effect_wait_until_end(0);
        }
        break;
    case POT_HALLUCINATION:
        if (Hallucination || Halluc_resistance)
            nothing++;
        else
            play_sfx_sound(SFX_ACQUIRE_HALLUCINATION);

        if (!Halluc_resistance)
        {
            (void)make_hallucinated(itimeout_incr(HHallucination, duration),
                TRUE, 0L);
        }
        break;
    case POT_WATER:
        if (!otmp->blessed && !otmp->cursed) {
            pline("This tastes like %s.", hliquid("water"));
            u.uhunger += nutrition;
            update_hunger_status(FALSE);
            break;
        }
        unkn++;
        if (is_undead(youmonst.data) || is_demon(youmonst.data)
            || u.ualign.type == A_CHAOTIC)
        {
            if (otmp->blessed) {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "This burns like %s!", hliquid("acid"));
                exercise(A_CON, FALSE);
                if (u.ulycn >= LOW_PM) 
                {
                    Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "affinity to %s disappears!",
                         makeplural(mons[u.ulycn].mname));
                    if (youmonst.data == &mons[u.ulycn])
                        you_unwere(FALSE);
                    set_ulycn(NON_PM); /* cure lycanthropy */
                }
                losehp(adjust_damage(duration, (struct monst*)0, &youmonst, AD_ACID, FALSE), "potion of holy water",
                       KILLED_BY_AN);
            } 
            else if (otmp->cursed) 
            {
                You_feel("quite proud of yourself.");
                healup(duration, 0, 0, 0, 0, 0, 0);
                if (u.ulycn >= LOW_PM && !Upolyd)
                    you_were();
                exercise(A_CON, TRUE);
            }
        }
        else 
        {
            if (otmp->blessed) 
            {
                play_sfx_sound(SFX_CURE_DISEASE);
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "full of awe.");
                make_sick(0L, (char *) 0, TRUE, 0);
                make_food_poisoned(0L, (char*)0, TRUE, 0);
                make_mummy_rotted(0L, (char*)0, TRUE, 0);
                exercise(A_WIS, TRUE);
                exercise(A_CON, TRUE);
                if (u.ulycn >= LOW_PM)
                    you_unwere(TRUE); /* "Purified" */
                /* make_confused(0L, TRUE); */
            } 
            else 
            {
                if (u.ualign.type == A_LAWFUL)
                {
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "This burns like %s!", hliquid("acid"));
                    losehp(adjust_damage(duration, (struct monst*)0, &youmonst, AD_ACID, ADFLAGS_NONE), "potion of unholy water",
                           KILLED_BY_AN);
                } else
                    You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "full of dread.");
                if (u.ulycn >= LOW_PM && !Upolyd)
                    you_were();
                exercise(A_CON, FALSE);
            }
        }
        break;
    case POT_ELVEN_HERBAL_BREW:
        unkn++;
        pline("Ooph!  This tastes like %s%s!",
              otmp->odiluted ? "watered down " : "",
              Hallucination ? "dandelion wine" : "liquid fire");
        if (!otmp->blessed)
        {
            if(!Confusion)
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, duration), FALSE);
        }
        /* the whiskey makes us feel better */
        if (!otmp->odiluted)
            healup(1, 0, FALSE, FALSE, FALSE, FALSE, FALSE);
        u.uhunger += nutrition; // 10 * (2 + bcsign(otmp));
        update_hunger_status(FALSE);
        exercise(A_WIS, FALSE);
        if (otmp->cursed) 
        {
            You("pass out.");
            multi = -rnd(max(1, extra_data1));
            nomovemsg = "You awake with a headache.";
        }
        break;
    case POT_URINE:
        unkn++;
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Ooph!  This tastes like %s%s!",
            otmp->odiluted ? "watered down " : "",
            Hallucination ? "dwarven mushroom brew" : objects[POT_URINE].oc_name_known ? "pee" : "urine sample");
        break;
    case POT_ENLIGHTENMENT:
        if (otmp->cursed)
        {
            unkn++;
            You("have an uneasy feeling...");
            exercise(A_WIS, FALSE);
        }
        else 
        {
            if (otmp->blessed && !isdiluted)
            {
                play_sfx_sound(SFX_GAIN_ABILITY);
                (void) adjattrib(A_INT, 1, FALSE);
                (void) adjattrib(A_WIS, 1, FALSE);
            }
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "self-knowledgeable...");
            display_nhwindow(WIN_MESSAGE, FALSE);
            enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
            pline_The("feeling subsides.");
            exercise(A_WIS, TRUE);
        }
        break;
    case SPE_INVISIBILITY:
        /* spell cannot penetrate mummy wrapping */
        if (uarmo && Blocks_Invisibility && uarmo->otyp == MUMMY_WRAPPING) 
        {
            You_feel("rather itchy under %s.", yname(uarmo));
            break;
        }
        /* FALLTHRU */
    case POT_INVISIBILITY:
        if (Invis || Blind || Blocks_Invisibility) 
        {
            nothing++;
        } 
        else 
        {
            self_invis_message();
        }

        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HInvis, duration);
        newsym(u.ux, u.uy); /* update position */
        refresh_u_tile_gui_info(TRUE);
        special_effect_wait_until_end(0);

        if (otmp->cursed)
        {
            pline("For some reason, you feel your presence is known.");
            play_sfx_sound(SFX_AGGRAVATE_MONSTER);
            aggravate();
        }
        break;
    case POT_SEE_INVISIBLE: /* tastes like fruit juice in Rogue */
    case POT_FRUIT_JUICE: 
    {
        int msg = Invisib && !Blind;

        unkn++;
        if (otmp->cursed)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Yecch!  This tastes %s.",
                  Hallucination ? "overripe" : "rotten");
        else
            pline(
                Hallucination
                    ? "This tastes like 10%% real %s%s all-natural beverage."
                    : "This tastes like %s%s.",
                otmp->odiluted ? "reconstituted " : "", fruitname(TRUE));
        if (otmp->otyp == POT_FRUIT_JUICE) 
        {
            u.uhunger += nutrition / (otmp->odiluted ? 2 : 1); // 5 : 10) *(2 + bcsign(otmp));
            update_hunger_status(FALSE);
            break;
        }
        if (!otmp->cursed) {
            /* Tell them they can see again immediately, which
             * will help them identify the potion...
             */
            make_blinded(0L, TRUE);
        }
        if (otmp->blessed && !isdiluted)
            HSee_invisible |= FROM_ACQUIRED;
        else
            incr_itimeout(&HSee_invisible, duration);
        refresh_u_tile_gui_info(FALSE);
        set_mimic_blocking(); /* do special mimic handling */
        see_monsters();       /* see invisible monsters */
        newsym(u.ux, u.uy);   /* see yourself! */
        flush_screen(1);
        if (msg && !Blind) {  /* Blind possible if polymorphed */
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "can see through yourself, but you are visible!");
            unkn--;
        }
        check_seen_bosses();
        break;
    }
    case POT_DWARVEN_MUSHROOM_BREW:
        unkn++;
        const char* halludrinktypes[6] = { "lemonade", "soda", "tea", "infusion", "drink" };
        const char* halluflavors[6] = { "a rusty", "a strange crunchy", "an odd mineral", "a marked sweet", "a dry" };

        const char* drinktype = Hallucination ? halludrinktypes[rn2(6)] : "stout";
        const char* flavortype = Hallucination ? halluflavors[rn2(6)] : "a nasty metallic";

        if (otmp->cursed)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Yecch!  This tastes like %s%s, but it has %s flavor.", otmp->odiluted ? "particularly light " : "finely brewed ", drinktype, flavortype);
        else
            pline("This tastes like %s%s.", otmp->odiluted ? "particularly light " : "", drinktype);

        if (otmp->cursed)
            pline("Nevertheless, you feel refreshed%s.", !otmp->odiluted ? " although somewhat confused" : "");
        else
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel refreshed.");

        u.uhunger += nutrition;
        update_hunger_status(FALSE);

        if (!otmp->odiluted)
        {
            if(!Confusion)
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, duration), FALSE);
        }
        break;
    case POT_PARALYSIS:
        if (Free_action) {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "stiffen momentarily.");
        } else {
            if (Levitation || Is_airlevel(&u.uz) || Is_waterlevel(&u.uz))
                You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are motionlessly suspended.");
            else if (u.usteed)
                You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are frozen in place!");
            else
                Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s are frozen to the %s!", makeplural(body_part(FOOT)),
                     surface(u.ux, u.uy));
            incr_itimeout(&HParalyzed, duration);
            context.botl = context.botlx = 1;
            refresh_u_tile_gui_info(TRUE);
            standard_hint("Identify potions before drinking.", &u.uhint.drank_potion_of_paralysis_or_sleep);
        }
        break;
    case POT_SLEEPING:
        if (Sleep_resistance) // || Free_action
        {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "yawn.");
        } 
        else
        {
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "suddenly fall asleep!");
            fall_asleep(-duration, TRUE);
            standard_hint("Identify potions before drinking.", &u.uhint.drank_potion_of_paralysis_or_sleep);
        }
        break;
    case POT_MONSTER_DETECTION:
    case SPE_DETECT_MONSTERS:
    case SPE_IMPROVED_MONSTER_DETECTION:
        if (otmp->blessed || otmp->otyp == SPE_IMPROVED_MONSTER_DETECTION)
        {
            int x, y;

            if (Detect_monsters)
                nothing++;
            unkn++;
            /* after a while, repeated uses become less effective */
            if ((HDetect_monsters & TIMEOUT) >= 300L)
                i = 1;
            else
                i = duration;
            incr_itimeout(&HDetect_monsters, i);
            refresh_u_tile_gui_info(TRUE);
            for (x = 1; x < COLNO; x++) {
                for (y = 0; y < ROWNO; y++) 
                {
                    if (glyph_is_invisible(levl[x][y].hero_memory_layers.glyph))
                    {
                        unmap_object(x, y);
                        newsym(x, y);
                    }
                    if (MON_AT(x, y))
                        unkn = 0;
                }
            }
            see_monsters();
            if (unkn)
                You_feel("lonely.");
            break;
        }
        if (monster_detect(otmp, 0))
        {
            if(otmp->otyp == SPE_DETECT_MONSTERS || otmp->otyp == SPE_IMPROVED_MONSTER_DETECTION || objects[POT_MONSTER_DETECTION].oc_name_known)
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
            return 1; /* nothing detected */
        }
        exercise(A_WIS, TRUE);
        break;
    case POT_OBJECT_DETECTION:
    case SPE_DETECT_TREASURE:
        if (object_detect(otmp, 0))
        {
            if(otmp->otyp == SPE_DETECT_TREASURE || objects[POT_OBJECT_DETECTION].oc_name_known)
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
            return 1; /* nothing detected */
        }
        exercise(A_WIS, TRUE);
        break;
    case POT_SICKNESS:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Ulch!  This potion was contaminated!");
        if (otmp->blessed || isdiluted)
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "(But in fact it was mildly stale %s.)", fruitname(TRUE));
            if (!Role_if(PM_HEALER) && !Sick_resistance)
            {
                losehp(adjust_damage(1, (struct monst*)0, &youmonst, AD_DISE, ADFLAGS_NONE), "mildly contaminated potion", KILLED_BY_AN);
            }
        }
        else 
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "(But in fact it was biologically contaminated %s.)",  fruitname(TRUE));

            if (Sick_resistance || Role_if(PM_HEALER))
            {
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "Fortunately, you have been immunized.");
            } 
            else
            {
                char contaminant[BUFSZ];

                Sprintf(contaminant, "%s%s",
                        (Sick_resistance) ? "mildly " : "",
                        (otmp->speflags & SPEFLAGS_FROM_SINK) ? "contaminated tap water"
                                         : "contaminated potion");

                if (!FoodPoisoned)
                    play_sfx_sound(SFX_CATCH_FOOD_POISONING);
                
                make_food_poisoned(FoodPoisoned ? FoodPoisoned / 3L + 1L : (int64_t)rn1(ACURR(max(2, A_CON)), 20),
                    contaminant, TRUE, HINT_KILLED_POTION_OF_SICKNESS);

                exercise(A_CON, FALSE);
            }
            standard_hint("Identify potions to avoid drinking contaminated potions.", &u.uhint.drank_potion_of_sickness);
        }
        break;
    case POT_POISON:
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Yecch!  This stuff tastes like poison.");
        if (Poison_resistance)
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "However, you are unaffected by the poison.");
        }
        else 
        {
            if (!Fixed_abil) 
            {
                int typ = rn2(A_MAX);
                poisontell(typ, FALSE);
                (void)adjattrib(typ, Poison_resistance ? -1 : (otmp->blessed ? -rnd(max(1,extra_data1)) : -d(2, max(1, extra_data1))), 1);
            }
            losehp(adjust_damage(duration, (struct monst*)0, &youmonst, AD_DRST, ADFLAGS_NONE), "drinking poison", KILLED_BY);
            exercise(A_CON, FALSE);
            standard_hint("Identify potions to avoid drinking poisonous potions. Get poison resistance as early as possible.", &u.uhint.drank_potion_of_poison);
        }

        if (Hallucination) 
        {
            You("are shocked back to your senses!");
            (void)make_hallucinated(0L, FALSE, 0L);
        }

        /* Also makes you cured from lycanthropy */
        if (u.ulycn >= LOW_PM || is_were(youmonst.data))
            you_unwere(TRUE);

        break;
    case POT_CONFUSION:
        if (!Confusion) {
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            if (Hallucination) {
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "What a trippy feeling!");
                unkn++;
            } else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "Huh, What?  Where am I?");
        } else
            nothing++;
        make_confused(itimeout_incr(HConfusion, duration), FALSE);
        break;
    case POT_GAIN_ABILITY:
        if (otmp->cursed)
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Ulch!  That potion tasted foul!");
            unkn++;
        } 
        else if (Fixed_abil) 
        {
            nothing++;
        } 
        else 
        {      /* If blessed, increase all; if not, try up to */
            int added_abilities = 0;
            int prev_ability = -1;
            int abilities_to_increase = (otmp->blessed ? 2 : 1);
            i = -1;   /* increment to 0 */
            if (isdiluted && !rn2(2))
            {
                /* Nothing */
            }
            else
            {
                int cnt;
                for (cnt = 0; cnt < 10; cnt++)
                {
                    do
                    {
                        i = rn2(A_MAX);
                    } while (i == prev_ability);

                    /* On tenth try, give STR if that's not maxed out */
                    if (cnt == 9 && ABASE(A_STR) < ATTRMAX(A_STR) && prev_ability != A_STR)
                        i = A_STR;

                    if (adjattrib(i, 1, -1))
                    {
                        prev_ability = i;
                        added_abilities++;
                    }
                    if (added_abilities >= abilities_to_increase)
                        break;
                }
            }

            if (added_abilities == 0)
            {
                You_feel("a bit more competent for a moment, but then the feeling subsides.");
                play_sfx_sound(SFX_GENERAL_THAT_DID_NOTHING);
            }
            else
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                play_sfx_sound(SFX_GAIN_ABILITY);
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        }
        break;
    case POT_SPEED:
    case POT_GREATER_SPEED:
    case POT_LIGHTNING_SPEED:
        /* skip when mounted; heal_legs() would heal steed's legs */
        if (Wounded_legs && !otmp->cursed && !u.usteed) {
            heal_legs(0);
            unkn++;
            break;
        }
        /* FALLTHRU */
    case SPE_HASTE_SELF:
        if (otmp->otyp == POT_LIGHTNING_SPEED ? !Lightning_fast : otmp->otyp == POT_GREATER_SPEED ? !Ultra_fast || !Super_fast : otmp->otyp == POT_SPEED ? !Ultra_fast || !Super_fast || !Lightning_fast : !Very_fast || !Ultra_fast || !Super_fast || !Lightning_fast)
        { /* wwf@doe.carleton.ca */
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are suddenly moving %sfaster.", (otmp->otyp == POT_LIGHTNING_SPEED ? Super_fast : otmp->otyp == POT_GREATER_SPEED ? Ultra_fast : otmp->otyp == POT_SPEED ? Very_fast : Fast) ? "" : "much ");
        }
        else 
        {
            Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s get new energy.", makeplural(body_part(LEG)));
            unkn++;
        }
        exercise(A_DEX, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_ACQUIRE_HASTE);
        special_effect_wait_until_action(0);
        incr_itimeout(otmp->otyp == POT_LIGHTNING_SPEED && !isdiluted ? &HLightning_fast : 
            (otmp->otyp == POT_GREATER_SPEED && !isdiluted) || (otmp->otyp == POT_LIGHTNING_SPEED && isdiluted) ? &HSuper_fast : 
            &HUltra_fast, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_MAGIC_RESISTANCE:
        if (!Antimagic)
        { 
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel secure from magical forces.");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit more secure from magical forces.");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HAntimagic, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_TITAN_STRENGTH:
        if (!Titan_strength)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel as strong as a titan.");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit stronger than before.");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HTitan_strength, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_FIRE_IMMUNITY:
        if (!Fire_immunity)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel fully protected from fire.");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit more protected from fire.");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HFire_immunity, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_COLD_IMMUNITY:
        if (!Cold_immunity)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel fully protected from cold.");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit more protected from cold.");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HCold_immunity, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_SHOCK_IMMUNITY:
        if (!Shock_immunity)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel fully protected from electricity.");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit more protected from electricity.");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HShock_immunity, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_HEROISM:
    case POT_SUPER_HEROISM:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        if (otmp->otyp == POT_HEROISM ? (!Heroism && !Super_heroism) : !Super_heroism)
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel %sheroic.", otmp->otyp == POT_SUPER_HEROISM ? "super " : "");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit more %sheroic than before.", otmp->otyp == POT_SUPER_HEROISM ? "super " : "");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        incr_itimeout(otmp->otyp == POT_HEROISM ? &HHeroism : &HSuper_heroism, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_LESSER_REGENERATION:
    case POT_REGENERATION:
    case POT_GREATER_REGENERATION:
        if (otmp->otyp == POT_GREATER_REGENERATION ? !Divine_regeneration : otmp->otyp == POT_REGENERATION ? !Divine_regeneration && !Rapidest_regeneration : !Divine_regeneration && !Rapidest_regeneration &&!Rapider_regeneration)
        {
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "unusually regenerative.");
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_POSITIVE, "feel a bit more regenerative than before.");
            unkn++;
        }
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(otmp->otyp == POT_GREATER_REGENERATION ? &HDivine_regeneration : otmp->otyp == POT_REGENERATION ? &HRapidest_regeneration : &HRapider_regeneration, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_LESSER_REJUVENATION:
    case POT_REJUVENATION:
    case POT_GREATER_REJUVENATION:
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "rejuvenated.");
        exercise(A_WIS, TRUE);
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(otmp->otyp == POT_GREATER_REJUVENATION ? &HRapidest_regeneration : otmp->otyp == POT_REJUVENATION ? &HRapider_regeneration : &HRapid_regeneration, duration);
        incr_itimeout(otmp->otyp == POT_GREATER_REJUVENATION ? &HRapidest_energy_regeneration : otmp->otyp == POT_REJUVENATION ? &HRapider_energy_regeneration : &HRapid_energy_regeneration, duration);
        special_effect_wait_until_end(0);
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_BLINDNESS:
        if (Blind)
            nothing++;
        else
            play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
        make_blinded(itimeout_incr(Blinded, duration), (boolean) !Blind);
        break;
    case POT_GAIN_LEVEL:
        if (otmp->cursed) {
            unkn++;
            /* they went up a level */
            if (((ledger_no(&u.uz) == 1 && u.uhave.amulet) || Can_rise_up(u.ux, u.uy, &u.uz)) 
                && (!isdiluted || !rn2(2)))
            {
                const char *riseup = "rise up, through the %s!";
                play_sfx_sound(SFX_CURSED_GAIN_LEVEL);

                if (ledger_no(&u.uz) == 1) 
                {
                    You(riseup, ceiling(u.ux, u.uy));
                    goto_level(&earth_level, FALSE, FALSE, FALSE, FALSE);
                } 
                else
                {
                    int newlev = depth(&u.uz) - 1;
                    d_level newlevel;

                    get_level(&newlevel, newlev);
                    if (on_level(&newlevel, &u.uz)) 
                    {
                        pline("It tasted bad.");
                        break;
                    } else
                        You(riseup, ceiling(u.ux, u.uy));
                    goto_level(&newlevel, FALSE, FALSE, FALSE, FALSE);
                }
            } 
            else
            {
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have an uneasy feeling.");
                play_sfx_sound(SFX_GENERAL_THAT_DID_NOTHING);
            }
            break;
        }

        if (!isdiluted || !rn2(2))
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_action(0);

            pluslvl(FALSE);
            /* blessed potions place you at a random spot in the
               middle of the new level instead of the low point */
            if (otmp->blessed)
                u.uexp = rndexp(TRUE);

            special_effect_wait_until_end(0);
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have an elevated feeling for a moment.");
            play_sfx_sound(SFX_GENERAL_THAT_DID_NOTHING);
        }
        break;
    case POT_HEALING:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_HEALING);
        special_effect_wait_until_action(0);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "better.");
        healup(duration, otmp->blessed ? extra_data1 : 0,
               cures_sick, cures_blind, cures_hallucination, cures_stun, cures_confusion);
        exercise(A_CON, TRUE);
        special_effect_wait_until_end(0);
        break;
    case POT_EXTRA_HEALING:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_HEALING);
        special_effect_wait_until_action(0);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much better.");
        healup(duration, otmp->blessed ? extra_data1 : 0,
            cures_sick, cures_blind, cures_hallucination, cures_stun, cures_confusion);
        exercise(A_CON, TRUE);
        exercise(A_STR, TRUE);
        special_effect_wait_until_end(0);
        break;
    case POT_GREATER_HEALING:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_HEALING);
        special_effect_wait_until_action(0);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much, much better.");
        healup(duration, otmp->blessed ? extra_data1 : 0,
            cures_sick, cures_blind, cures_hallucination, cures_stun, cures_confusion);
        exercise(A_CON, TRUE);
        exercise(A_STR, TRUE);
        special_effect_wait_until_end(0);
        break;
    case POT_FULL_HEALING:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_FULL_HEALING);
        special_effect_wait_until_action(0);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "completely healed.");
        healup(duration, otmp->blessed ? extra_data1 : 0,
            cures_sick, cures_blind, cures_hallucination, cures_stun, cures_confusion);
        /* Restore one lost level if blessed */
        if (otmp->blessed && u.ulevel < u.ulevelmax) {
            /* when multiple levels have been lost, drinking
               multiple potions will only get half of them back */
            u.ulevelmax -= 1;
            pluslvl(FALSE);
        }
        exercise(A_STR, TRUE);
        exercise(A_CON, TRUE);
        special_effect_wait_until_end(0);
        break;
    case POT_LEVITATION:
    case SPE_LEVITATION:
        /*
         * BLevitation will be set if levitation is blocked due to being
         * inside rock (currently or formerly in phazing xorn form, perhaps)
         * but it doesn't prevent setting or incrementing Levitation timeout
         * (which will take effect after escaping from the rock if it hasn't
         * expired by then).
         */
        if (!Levitation && !Blocks_Levitation)
        {
            /* kludge to ensure proper operation of float_up() */
            set_itimeout(&HLevitation, 1L);
            float_up();
            /* This used to set timeout back to 0, then increment it below
               for blessed and uncursed effects.  But now we leave it so
               that cursed effect yields "you float down" on next turn.
               Blessed and uncursed get one extra turn duration. */
        } else /* already levitating, or can't levitate */
            nothing++;

        if (otmp->cursed) 
        {
            /* 'already levitating' used to block the cursed effect(s)
               aside from ~I_SPECIAL; it was not clear whether that was
               intentional; either way, it no longer does (as of 3.6.1) */
            HLevitation &= ~I_SPECIAL; /* can't descend upon demand */
            if (Blocks_Levitation)
            {
                ; /* rising via levitation is blocked */
            }
            else if ((u.ux == xupstair && u.uy == yupstair)
                    || (sstairs.up && u.ux == sstairs.sx && u.uy == sstairs.sy)
                    || (xupladder && u.ux == xupladder && u.uy == yupladder))
            {
                (void) doup();
                /* in case we're already Levitating, which would have
                   resulted in incrementing 'nothing' */
                nothing = 0; /* not nothing after all */
            } 
            else if (has_ceiling(&u.uz))
            {
                int dmg = rnd(!uarmh ? 10 : !is_hard_helmet(uarmh) ? 6 : 3);

                You("hit your %s on the %s.", body_part(HEAD),
                    ceiling(u.ux, u.uy));
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "colliding with the ceiling",
                       KILLED_BY);
                nothing = 0; /* not nothing after all */
            }
        } else if (otmp->blessed) 
        {
            /* at this point, timeout is already at least 1 */
            incr_itimeout(&HLevitation, duration + extra_data1);
            /* can descend at will (stop levitating via '>') provided timeout
               is the only factor (ie, not also wearing Lev ring or boots) */
            HLevitation |= I_SPECIAL;
        } 
        else /* timeout is already at least 1 */
            incr_itimeout(&HLevitation, duration);

        if (Levitation && IS_SINK(levl[u.ux][u.uy].typ))
            spoteffects(FALSE);
        /* levitating blocks flying */
        float_vs_flight();
        refresh_u_tile_gui_info(TRUE);
        break;
    case POT_GREATER_ENERGY:
    case POT_FULL_ENERGY:
    case POT_EXTRA_ENERGY:
    case POT_GAIN_ENERGY:
    { /* M. Stephenson */
        int num = 0, numxtra = 0;
        
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);

        if (otmp->cursed)
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "lackluster.");
        else
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Magical energies course through your body.");

        /* old: num = rnd(5) + 5 * otmp->blessed + 1;
         *      blessed:  +7..11 max & current (+9 avg)
         *      uncursed: +2.. 6 max & current (+4 avg)
         *      cursed:   -2.. 6 max & current (-4 avg)
         * new: (3.6.0)
         *      blessed:  +3..18 max (+10.5 avg), +9..54 current (+31.5 avg)
         *      uncursed: +2..12 max (+ 7   avg), +6..36 current (+21   avg)
         *      cursed:   -1.. 6 max (- 3.5 avg), -3..18 current (-10.5 avg)
         */
        num = duration;
        numxtra = extra_data1;
        if (otmp->cursed)
        {
            /* subtract instead of add when cursed */
            num = -num;
            u.ubaseendrain -= numxtra;
            play_sfx_sound(SFX_LOSE_ENERGY);
        }
        else if(otmp->blessed)
        {
            num = num * 2;
            if (u.ubaseendrain < 0)
            {
                u.ubaseendrain += numxtra;
                if (u.ubaseendrain > 0)
                {
                    numxtra = u.ubaseendrain;
                    u.ubaseendrain = 0;
                }
                else
                    numxtra = 0;
            }
            u.ubaseenmax += numxtra;
            play_sfx_sound(SFX_FULL_ENERGY);
        }
        else
        {
            play_sfx_sound(otmp->otyp == POT_FULL_ENERGY ? SFX_FULL_ENERGY : SFX_GAIN_ENERGY);
        }

        special_effect_wait_until_action(0);

        int mana_before = u.uen;
        u.uen += num;
        updatemaxen();
        int mana_after = u.uen;
        int mana_gain = mana_after - mana_before;
        if (mana_gain > 0 && isok(u.ux, u.uy))
        {
            char fbuf[BUFSZ];
            Sprintf(fbuf, "+%d", mana_gain);
            display_floating_text(u.ux, u.uy, fbuf, FLOATING_TEXT_MANA_GAIN, ATR_NONE, NO_COLOR, 0UL);
        }

        context.botl = 1;
        exercise(A_WIS, TRUE);
        special_effect_wait_until_end(0);

        break;
    }
    case POT_OIL: { /* P. Winner */
        boolean good_for_you = FALSE;

        if (otmp->lamplit) 
        {
            if (likes_fire(youmonst.data))
            {
                pline("Ahh, a refreshing drink.");
                good_for_you = TRUE;
            }
            else
            {
                You("burn your %s%s.", body_part(FACE), Fire_immunity ? ", but you just get a nice tan" : "");
                /* fire damage */
                losehp(adjust_damage(duration, (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), "burning potion of oil",
                       KILLED_BY_AN);
            }
        } 
        else if (otmp->cursed)
            pline("This tastes like castor oil.");
        else
            pline("That was smooth!");
        exercise(A_WIS, good_for_you);
        break;
    }
    case POT_ACID:
        if (Acid_immunity) {
            /* Not necessarily a creature who _likes_ acid */
            pline("This tastes %s.", Hallucination ? "tangy" : "sour");
        } else {
            int dmg;

            pline_ex(ATR_NONE, otmp->blessed ? CLR_MSG_WARNING : CLR_MSG_NEGATIVE, "This burns%s!",
                  otmp->blessed ? " a little" : otmp->cursed ? " a lot"
                                                             : " like acid");
            dmg = duration;
            losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_ACID, ADFLAGS_NONE), "potion of acid", KILLED_BY_AN);
            exercise(A_CON, FALSE);
        }
        if (Stoned)
            fix_petrification();
        unkn++; /* holy/unholy water can burn like acid too */
        break;
    case POT_POLYMORPH:
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "a little %s.", Hallucination ? "normal" : "strange");
        if (!Unchanging && !Polymorph_resistance)
        {
            if (isdiluted && !rn2(2))
            {
                You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
            }
            else
            {
                polyself(0);
            }
        }
        special_effect_wait_until_end(0);
        break;
    default:
        impossible("What a funny potion! (%u)", otmp->otyp);
        return 0;
    }
    return -1;
}

/* nhp = healed hit points, nxtra = increase to maximum, nhp and nxtra do not stack */
void
healup(nhp, nxtra, curesick, cureblind, curehallucination, curestun, cureconfusion)
int nhp, nxtra;
register boolean curesick, cureblind, curehallucination, curestun, cureconfusion;
{
    int cur_hp_before = (Upolyd ? u.mh : u.uhp);
    int max_hp_before = (Upolyd ? u.mhmax : u.uhpmax);
    int nxtra_limit = max(0, cur_hp_before + nhp - max_hp_before);
    int gained_nxtra = min(nxtra_limit, nxtra);
    if (gained_nxtra > 0)
    {
        if (Upolyd)
        {
            if (u.basemhdrain < 0)
            {
                u.basemhdrain += gained_nxtra;
                if (u.basemhdrain > 0)
                {
                    gained_nxtra = u.basemhdrain;
                    u.basemhdrain = 0;
                }
                else
                    gained_nxtra = 0;
            }
            u.basemhmax += gained_nxtra;
        }
        else 
        {
            if (u.ubasehpdrain < 0)
            {
                u.ubasehpdrain += gained_nxtra;
                if (u.ubasehpdrain > 0)
                {
                    gained_nxtra = u.ubasehpdrain;
                    u.ubasehpdrain = 0;
                }
                else
                    gained_nxtra = 0;
            }
            u.ubasehpmax += gained_nxtra;
        }
        updatemaxhp();
        int max_hp_after = (Upolyd ? u.mhmax : u.uhpmax);
        int max_hp_gain = max_hp_after - max_hp_before;
        if (max_hp_gain > 0 && isok(u.ux, u.uy))
        {
            char fbuf[BUFSZ];
            Sprintf(fbuf, "+%d max HP", max_hp_gain);
            display_floating_text(u.ux, u.uy, fbuf, FLOATING_TEXT_ATTRIBUTE_GAIN, ATR_NONE, NO_COLOR, 0UL);
        }
    }

    if (nhp > 0)
        deduct_player_hp((double)(-nhp));
    
    if (cureblind)
    {
        /* 3.6.1: it's debatible whether healing magic should clean off
           mundane 'dirt', but if it doesn't, blindness isn't cured */
        u.ucreamed = 0;
        make_blinded(0L, TRUE);
        /* heal deafness too */
        make_deaf(0L, TRUE);
    }

    if (curesick)
    {
        make_vomiting(0L, TRUE);
        make_sick(0L, (char *) 0, TRUE, 0);
        make_food_poisoned(0L, (char*)0, TRUE, 0);
        make_mummy_rotted(0L, (char*)0, TRUE, 0);
    }

    if (curehallucination) 
    {
        (void)make_hallucinated(0L, TRUE, 0L);
    }

    if (curestun)
    {
        make_stunned(0L, TRUE);
    }

    if (cureconfusion)
    {
        make_confused(0L, TRUE);
    }

    context.botl = context.botlx = TRUE;
    newsym_with_flags(u.ux, u.uy, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS_AND_FLAGS);
    flush_screen(1);
    return;
}

void
strange_feeling(obj, txt, dopopup)
struct obj *obj;
const char *txt;
boolean dopopup;
{
    char buf[BUFSZ] = "";
    if (flags.beginner || !txt)
        Sprintf(buf, "You have a %s feeling for a moment, then it passes.",
            Hallucination ? "normal" : "strange");
    else
        Strcpy(buf, txt);

    if(*buf)
        pline_ex1_popup(ATR_NONE, CLR_MSG_ATTENTION, buf, "Strange Feeling", dopopup);

    if (!obj) /* e.g., crystal ball finds no traps */
        return;

    if (obj->oclass != SCROLL_CLASS && obj->oclass != POTION_CLASS) /* Use up only potions and scrolls */
        return;

    if (obj->dknown && !objects[obj->otyp].oc_name_known
        && !objects[obj->otyp].oc_uname)
        docall(obj, buf);

    if (obj->unpaid && costly_spot(u.ux, u.uy))
    {
        char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
        struct monst* shkp = 0;
        shkp = shop_keeper(*o_shop);
        if (shkp && inhishop(shkp) && is_obj_on_shk_bill(obj, shkp) && !Deaf && !muteshk(shkp))
        {
            play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_ILL_ADD_THAT_TO_YOUR_BILL);
        }
    }

    useup(obj);
}

const char *bottlenames[] = { "bottle", "phial", "flagon", "carafe",
                              "flask",  "jar",   "vial" };

const char *
bottlename()
{
    return bottlenames[rn2(SIZE(bottlenames))];
}

/* handle item dipped into water potion or steed saddle splashed by same */
boolean
H2Opotion_dip(potion, targobj, useeit, objphrase)
struct obj *potion, *targobj;
boolean useeit;
const char *objphrase; /* "Your widget glows" or "Steed's saddle glows" */
{
    void FDECL((*func), (OBJ_P)) = 0;
    const char *glowcolortext = 0;
#define COST_alter (-2)
#define COST_none (-1)
    int costchange = COST_none;
    boolean altfmt = FALSE, res = FALSE;
    enum sfx_sound_types sfxsound = 0;
    int textcolor = CLR_MSG_ATTENTION;

    if (!potion || potion->otyp != POT_WATER)
        return FALSE;

    if (potion->blessed) 
    {
        if (targobj->cursed) 
        {
            func = uncurse;
            textcolor = Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_POSITIVE;
            glowcolortext = NH_AMBER;
            costchange = COST_UNCURS;
            sfxsound = SFX_UNCURSE_ITEM_SUCCESS;
        } 
        else if (!targobj->blessed)
        {
            func = bless;
            textcolor = Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_POSITIVE;
            glowcolortext = NH_LIGHT_BLUE;
            costchange = COST_alter;
            altfmt = TRUE; /* "with a <color> aura" */
            sfxsound = SFX_BLESS_ITEM_SUCCESS;
        }
    }
    else if (potion->cursed) 
    {
        if (targobj->blessed)
        {
            func = unbless;
            textcolor = Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_WARNING;
            glowcolortext = NH_BROWN;
            costchange = COST_UNBLSS;
            sfxsound = SFX_UNBLESS_ITEM_SUCCESS;
        } 
        else if (!targobj->cursed) 
        {
            func = curse;
            textcolor = Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_NEGATIVE;
            glowcolortext = NH_BLACK;
            costchange = COST_alter;
            altfmt = TRUE;
            sfxsound = SFX_CURSE_ITEM_SUCCESS;
        }
    } 
    else 
    {
        /* dipping into uncursed water; carried() check skips steed saddle */
        if (carried(targobj)) 
        {
            if (water_damage(targobj, 0, TRUE) != ER_NOTHING)
                res = TRUE;
        }
    }

    if (func) 
    {
        /* give feedback before altering the target object;
           this used to set obj->bknown even when not seeing
           the effect; now hero has to see the glow, and bknown
           is cleared instead of set if perception is distorted */
        if (useeit)
        {
            if(sfxsound > 0)
                play_sfx_sound(sfxsound);

            if (altfmt)
            {
                const char* hclr = hcolor_multi_buf2(glowcolortext);
                pline_multi_ex(ATR_NONE, textcolor, no_multiattrs, multicolor_buffer, "%s with %s%s aura.", objphrase, an_prefix(hclr), hclr);
            }
            else
                pline_multi_ex(ATR_NONE, textcolor, no_multiattrs, multicolor_buffer, "%s %s.", objphrase, hcolor_multi_buf1(glowcolortext));

            iflags.last_msg = PLNMSG_OBJ_GLOWS;
            targobj->bknown = !Hallucination;
        }

        /* potions of water are the only shop goods whose price depends
           on their curse/bless state */
        if (targobj->unpaid && targobj->otyp == POT_WATER) 
        {
            if (costchange == COST_alter)
                /* added blessing or cursing; update shop
                   bill to reflect item's new higher price */
                alter_cost(targobj, 0L);
            else if (costchange != COST_none)
                /* removed blessing or cursing; you
                   degraded it, now you'll have to buy it... */
                costly_alteration(targobj, costchange);
        }

        /* finally, change curse/bless state */
        (*func)(targobj);
        res = TRUE;
    }
    return res;
}

/* potion obj hits monster mon, which might be youmonst; obj always used up */
void
potionhit(mon, obj_ptr, how)
struct monst *mon;
struct obj **obj_ptr;
int how;
{
    if (!mon || !obj_ptr)
        return;

    const char *botlnam = bottlename();
    boolean isyou = (mon == &youmonst);
    int distance, tx, ty;
    struct obj *saddle = (struct obj *) 0;
    boolean hit_saddle = FALSE, your_fault = (how <= POTHIT_HERO_THROW);
    struct obj* obj = *obj_ptr;
    char dcbuf[IBUFSZ] = "";

    if (!obj)
        return;

    if (obj->oclass != POTION_CLASS)
        return;

    int power1 = objects[obj->otyp].oc_oprop;
    int power2 = objects[obj->otyp].oc_oprop2;
    int power3 = objects[obj->otyp].oc_oprop3;
    int dicebuc = (int)objects[obj->otyp].oc_potion_breathe_dice_buc_multiplier;
    int duration = (int)(
        d(max(0, objects[obj->otyp].oc_potion_breathe_dice + dicebuc * bcsign(obj)), max(1, objects[obj->otyp].oc_potion_breathe_diesize))
        + objects[obj->otyp].oc_potion_breathe_plus
        + bcsign(obj) * objects[obj->otyp].oc_potion_breathe_buc_multiplier);

    if (isyou)
    {
        tx = u.ux, ty = u.uy;
        distance = 0;
        play_simple_object_sound_at_location(obj, tx, ty, OBJECT_SOUND_TYPE_BREAK);
        pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "%s crashes on your %s and breaks into shards.", botlnam, body_part(HEAD));
        losehp(adjust_damage(rnd(2), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE),
               (how == POTHIT_OTHER_THROW) ? "propelled potion" /* scatter */
                                           : "thrown potion",
               KILLED_BY_AN);
    } 
    else 
    {
        tx = mon->mx, ty = mon->my;
        /* sometimes it hits the saddle */
        if (((mon->worn_item_flags & W_SADDLE)
             && (saddle = which_armor(mon, W_SADDLE)))
            && (!rn2(10)
                || (obj->otyp == POT_WATER
                    && ((rnl(10) > 7 && obj->cursed)
                        || (rnl(10) < 4 && obj->blessed) || !rn2(3)))))
            hit_saddle = TRUE;
        distance = distu(tx, ty);
        if (!cansee(tx, ty)) 
        {
            play_simple_object_sound_at_location(obj, tx, ty, OBJECT_SOUND_TYPE_BREAK);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Crash!");
        }
        else
        {
            char *mnam = mon_nam(mon);
            char buf[BUFSZ];

            if (hit_saddle && saddle) 
            {
                Sprintf(buf, "%s saddle",
                        s_suffix(x_monnam(mon, ARTICLE_THE, (char *) 0,
                                          (SUPPRESS_IT | SUPPRESS_SADDLE),
                                          FALSE)));
            } 
            else if (has_head(mon->data))
            {
                Sprintf(buf, "%s %s", s_suffix(mnam),
                        (notonhead ? "body" : "head"));
            } 
            else 
            {
                Strcpy(buf, mnam);
            }
            play_simple_object_sound_at_location(obj, tx, ty, OBJECT_SOUND_TYPE_BREAK);
            pline_The_ex(ATR_NONE, is_tame(mon) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s crashes on %s and breaks into shards.", botlnam, buf);
        }
        if (rn2(5) && mon->mhp > 1 && !hit_saddle)
            mon->mhp--;
    }

    /* oil doesn't instantly evaporate; Neither does a saddle hit */
    if (obj->otyp != POT_OIL && !hit_saddle && cansee(tx, ty))
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s.", Tobjnam(obj, "evaporate"));

    if (isyou) 
    {
        switch (obj->otyp) 
        {
        case POT_OIL:
            if (obj->lamplit)
                explode_oil(obj, u.ux, u.uy);
            break;
        case POT_POLYMORPH:
            Sprintf(dcbuf, "You feel a little %s.", Hallucination ? "normal" : "strange");
            pline_ex1(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_MYSTICAL, dcbuf);
            if (!Unchanging && !Polymorph_resistance && !Antimagic_or_resistance)
                polyself(0);
            break;
        case POT_ACID:
            if (!Acid_immunity) 
            {
                int dmg;

                Sprintf(dcbuf, "This burns%s!",
                      obj->blessed ? " a little"
                                   : obj->cursed ? " a lot" : "");
                pline_ex1(ATR_NONE, obj->blessed ? CLR_MSG_WARNING : CLR_MSG_NEGATIVE, dcbuf);
                dmg = duration;
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_ACID, FALSE), "potion of acid", KILLED_BY_AN);
            }
            break;
        }
    } 
    else if (hit_saddle && saddle) 
    {
        char *mnam, buf[BUFSZ], saddle_glows[BUFSZ];
        boolean affected = FALSE;
        boolean useeit = !Blind && canseemon(mon) && cansee(tx, ty);

        mnam = x_monnam(mon, ARTICLE_THE, (char *) 0,
                        (SUPPRESS_IT | SUPPRESS_SADDLE), FALSE);
        Sprintf(buf, "%s", upstart(s_suffix(mnam)));

        switch (obj->otyp) 
        {
        case POT_WATER:
            Sprintf(saddle_glows, "%s %s", buf, aobjnam(saddle, "glow"));
            affected = H2Opotion_dip(obj, saddle, useeit, saddle_glows);
            break;
        case POT_POLYMORPH:
            /* Do we allow the saddle to polymorph? */
            break;
        }
        if (useeit && !affected)
        {
            Sprintf(dcbuf, "%s %s wet.", buf, aobjnam(saddle, "get"));
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        }
    } 
    else 
    {
        boolean angermon = your_fault, cureblind = FALSE;

        switch (obj->otyp) 
        {
        case POT_FULL_HEALING:
            cureblind = TRUE;
            /*FALLTHRU*/
        case POT_GREATER_HEALING:
        case POT_EXTRA_HEALING:
            if (!obj->cursed)
                cureblind = TRUE;
            /*FALLTHRU*/
        case POT_HEALING:
            if (obj->blessed)
                cureblind = TRUE;
            if (mon->data == &mons[PM_PESTILENCE])
                goto do_illness;
            /*FALLTHRU*/
        case POT_RESTORE_ABILITY:
        case POT_GAIN_ABILITY:
 do_healing:
            angermon = FALSE;
            if (mon->mhp < mon->mhpmax) 
            {
                mon->mhp = mon->mhpmax;
                if (canseemon(mon))
                {
                    Sprintf(dcbuf, "%s looks sound and hale again.", Monnam(mon));
                    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                }
            }
            if (cureblind)
                mcureblindness(mon, canseemon(mon));
            break;
        case POT_SICKNESS:
            if (mon->data == &mons[PM_PESTILENCE])
                goto do_healing;

            if (dmgtype(mon->data, AD_DISE)
                /* won't happen, see prior goto */
                || dmgtype(mon->data, AD_PEST)
                /* most common case */
                || resists_sickness(mon))
            {
                if (canseemon(mon))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks unharmed.", Monnam(mon));
                break;
            }

            if (!check_ability_resistance_success(mon, A_CON, objects[obj->otyp].oc_mc_adjustment))
            {
                (void)set_mon_property_verbosely(mon, FOOD_POISONED,
                    is_food_poisoned(mon) ? max(1, (get_mon_property(mon, FOOD_POISONED) + 1) / 3) : rn1(M_ACURR(mon, A_CON), 20));
            }
            else
            {
                if (canseemon(mon))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks unharmed.", Monnam(mon));
            }
            break;
        case POT_POISON:
            if (resists_poison(mon))
            {
                if (canseemon(mon))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks unharmed.", Monnam(mon));
                break;
            }

do_illness: /* Pestilence's potion of healing effect */
            if ((mon->mbasehpmax + mon->mbasehpdrain > 3) && !check_ability_resistance_success(mon, A_CON, objects[obj->otyp].oc_mc_adjustment))
                mon->mbasehpdrain -= mon->mbasehpmax / 2;
            
            update_mon_maxhp(mon);
            
            if ((mon->mhp > 2) && !check_ability_resistance_success(mon, A_CON, objects[obj->otyp].oc_mc_adjustment))
                mon->mhp /= 2;
            if (mon->mhp > mon->mhpmax)
                mon->mhp = mon->mhpmax;
            if (canseemon(mon))
            {
                Sprintf(dcbuf, "%s looks rather ill.", Monnam(mon));
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
            }
            break;
        case POT_CONFUSION:
            (void)increase_mon_property_verbosely(mon, CONFUSION, duration);
            break;
        case POT_URINE:
            if (canseemon(mon) && !is_incorporeal(mon->data))
            {
                Sprintf(dcbuf, "%s looks concerened of %s body odor.", Monnam(mon), mhis(mon));
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
            }
            break;
        case POT_DWARVEN_MUSHROOM_BREW:
            if (canseemon(mon))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks unharmed.", Monnam(mon));
            break;
        case POT_ELVEN_HERBAL_BREW:
            if (!check_ability_resistance_success(mon, A_CON, objects[obj->otyp].oc_mc_adjustment))
                increase_mon_property_verbosely(mon, CONFUSION, duration);
            break;
        case POT_INVISIBILITY: 
        {
            boolean sawit = canspotmon(mon);

            angermon = FALSE;
            increase_mon_property_verbosely(mon, INVISIBILITY, duration);
            if (sawit && !canspotmon(mon) && cansee(mon->mx, mon->my))
                map_invisible(mon->mx, mon->my);
            break;
        }
        case POT_SLEEPING:
            /* wakeup() doesn't rouse victims of temporary sleep */
            if (sleep_monst(mon, obj, (struct monst*)0, duration, 0, FALSE)) 
            {
                Sprintf(dcbuf, "%s falls asleep.", Monnam(mon));
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                slept_monst(mon);
            }
            break;
        case POT_PARALYSIS:
            if (!resists_paralysis(mon))
            {
                /* really should be rnd(5) for consistency with players
                 * breathing potions, but...
                 */
                paralyze_monst(mon, duration, TRUE); //rn1(9, 8));
            }
            break;
        case POT_SPEED:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case POT_HEROISM:
        case POT_SUPER_HEROISM:
        case POT_LESSER_REGENERATION:
        case POT_REGENERATION:
        case POT_GREATER_REGENERATION:
        case POT_LESSER_REJUVENATION:
        case POT_REJUVENATION:
        case POT_GREATER_REJUVENATION:
            angermon = FALSE;
            if(power1)
                increase_mon_property_verbosely(mon, power1, duration);
            if (power2)
                increase_mon_property_verbosely(mon, power2, duration);
            if (power3)
                increase_mon_property_verbosely(mon, power3, duration);
            break;
        case POT_BLINDNESS:
            if (haseyes(mon->data)) 
            {
                int btmp = d(1, 8) + d(3, 8) * !check_ability_resistance_success(mon, A_CON, objects[obj->otyp].oc_mc_adjustment);

                increase_mon_property_verbosely(mon, BLINDED, btmp);
            }
            break;
        case POT_WATER:
            if (is_undead(mon->data) || is_demon(mon->data)
                || is_were(mon->data) || is_vampshifter(mon))
            {
                if (obj->blessed) 
                {
                    Sprintf(dcbuf, "%s %s in pain!", Monnam(mon),
                          is_silent(mon->data) ? "writhes" : "shrieks");
                    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                    if (!is_silent(mon->data))
                        wake_nearto(tx, ty, mon->data->mlevel * 10);
                    deduct_monster_hp(mon, adjust_damage(duration, &youmonst, mon, AD_CLRC, ADFLAGS_NONE));
                    /* should only be by you */
                    if (DEADMONSTER(mon))
                        killed(mon);
                    else if (is_were(mon->data) && !is_human(mon->data))
                        new_were(mon); /* revert to human */
                } 
                else if (obj->cursed)
                {
                    angermon = FALSE;
                    if (canseemon(mon))
                    {
                        Sprintf(dcbuf, "%s looks healthier.", Monnam(mon));
                        pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                    }
                    deduct_monster_hp(mon, adjust_damage(-duration, &youmonst, mon, AD_CLRC, ADFLAGS_NONE));
                    if (is_were(mon->data) && is_human(mon->data)
                        && !Protection_from_shape_changers)
                        new_were(mon); /* transform into beast */
                }
            } 
            else if (mon->data == &mons[PM_GREMLIN]) 
            {
                angermon = FALSE;
                (void) split_mon(mon, (struct monst *) 0);
            }
            else if (is_iron(mon->data)) 
            {
                if (canseemon(mon))
                {
                    Sprintf(dcbuf, "%s rusts.", Monnam(mon));
                    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                }
                deduct_monster_hp(mon, adjust_damage(duration / 2, &youmonst, mon, AD_PHYS, ADFLAGS_NONE));
                /* should only be by you */
                if (DEADMONSTER(mon))
                    killed(mon);
            }
            break;
        case POT_OIL:
            if (obj->lamplit)
                explode_oil(obj, tx, ty);
            break;
        case POT_ACID:
            if (!is_mon_immune_to_acid(mon) && !check_ability_resistance_success(mon, A_DEX, objects[obj->otyp].oc_mc_adjustment)) {
                Sprintf(dcbuf, "%s %s in pain!", Monnam(mon),
                      is_silent(mon->data) ? "writhes" : "shrieks");
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
                if (!is_silent(mon->data))
                    wake_nearto(tx, ty, mon->data->mlevel * 10);
                deduct_monster_hp(mon, adjust_damage(d(obj->cursed ? 4 : 3, obj->blessed ? 6 : 8), &youmonst, mon, AD_ACID, ADFLAGS_NONE));
                if (DEADMONSTER(mon)) {
                    if (your_fault)
                        killed(mon);
                    else
                        monkilled(mon, "", AD_ACID, 0);
                }
            }
            break;
        case POT_POLYMORPH:
            (void) bhitm(mon, obj, (struct monst*)0);
            break;
        /*
        case POT_GAIN_LEVEL:
        case POT_LEVITATION:
        case POT_FRUIT_JUICE:
        case POT_MONSTER_DETECTION:
        case POT_OBJECT_DETECTION:
            break;
        */
        }
        /* target might have been killed */
        if (!DEADMONSTER(mon)) {
            if (angermon)
                wakeup(mon, TRUE);
            else
            {
                if (mon->msleeping)
                {
                    mon->msleeping = 0;
                    refresh_m_tile_gui_info(mon, TRUE);
                }
            }
        }
    }

    /* Note: potionbreathe() does its own docall() */
    if ((distance == 0 || (distance < 3 && rn2(5)))
        && (!has_innate_breathless(youmonst.data) || haseyes(youmonst.data)))
        potionbreathe(obj, dcbuf);
    else if (obj->dknown && !objects[obj->otyp].oc_name_known
             && !objects[obj->otyp].oc_uname && cansee(tx, ty))
        docall(obj, dcbuf);

    if (*u.ushops && obj->unpaid) {
        struct monst *shkp = shop_keeper(*in_rooms(u.ux, u.uy, SHOPBASE));

        /* neither of the first two cases should be able to happen;
           only the hero should ever have an unpaid item, and only
           when inside a tended shop */
        if (!shkp) /* if shkp was killed, unpaid ought to cleared already */
            obj->unpaid = 0;
        else if (context.mon_moving) /* obj thrown by monster */
            subfrombill(obj, shkp);
        else /* obj thrown by hero */
            (void) stolen_value(obj, u.ux, u.uy, is_peaceful(shkp),
                                FALSE);
    }
    obfree(obj, (struct obj *) 0);
    *obj_ptr = (struct obj *)0;

}

/* vapors are inhaled or get in your eyes */
void
potionbreathe(obj, introline)
struct obj *obj;
const char* introline;
{
    if (!obj || obj->oclass != POTION_CLASS)
        return;

    int i, kn = 0;
    boolean cureblind = FALSE;
    char dcbuf[IBUFSZ] = "";

    /* potion of unholy water might be wielded; prevent
       you_were() -> drop_weapon() from dropping it so that it
       remains in inventory where our caller expects it to be */
    int dicebuc = (int)objects[obj->otyp].oc_potion_breathe_dice_buc_multiplier;
    int duration = (int)(
        d(max(0, objects[obj->otyp].oc_potion_breathe_dice + dicebuc * bcsign(obj)), max(1, objects[obj->otyp].oc_potion_breathe_diesize))
        + objects[obj->otyp].oc_potion_breathe_plus
        + bcsign(obj) * objects[obj->otyp].oc_potion_breathe_buc_multiplier);

    /* Adjustment for dilution */
    boolean isdiluted = obj->oclass == POTION_CLASS && obj->odiluted;
    if (isdiluted)
    {
        duration /= 2;
    }

    obj->in_use = 1;

    switch (obj->otyp) 
    {
    case POT_RESTORE_ABILITY:
    case POT_GAIN_ABILITY:
        if (obj->cursed) 
        {
            if (!has_innate_breathless(youmonst.data))
            {
                Strcpy(dcbuf, "Ulch!  That potion smells terrible!");
                pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
            }
            else if (haseyes(youmonst.data)) {
                const char *eyes = body_part(EYE);

                if (eyecount(youmonst.data) != 1)
                    eyes = makeplural(eyes);
                Sprintf(dcbuf, "Your %s %s!", eyes, vtense(eyes, "sting"));
                pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
            }
            break;
        } 
        else 
        {
            if (obj->blessed && !Fixed_abil)
            {
                boolean restored = FALSE;
                int trycnt;
                for (trycnt = 0; trycnt < 10; trycnt++)
                {
                    i = rn2(A_MAX); /* start at a random point */
                    if (ABASE(i) < AMAX(i))
                    {
                        ABASE(i)++;
                        context.botl = 1;
                        restored = TRUE;
                        break;
                    }
                }
                if(restored)
                    play_sfx_sound(SFX_RESTORE_ABILITY);
            }
        }
        break;
    case POT_FULL_HEALING:
        if (!isdiluted || !rn2(2))
        {
            if (Upolyd && u.mh < u.mhmax)
                u.mh++, context.botl = context.botlx = 1;
            if (u.uhp < u.uhpmax)
                u.uhp++, context.botl = context.botlx = 1;
        }
        cureblind = TRUE;
        /*FALLTHRU*/
    case POT_GREATER_HEALING:
    case POT_EXTRA_HEALING:
        if (!isdiluted || !rn2(2))
        {
            if (Upolyd && u.mh < u.mhmax)
                u.mh++, context.botl = context.botlx = 1;
            if (u.uhp < u.uhpmax)
                u.uhp++, context.botl = context.botlx = 1;
        }
        if (!obj->cursed)
            cureblind = TRUE;
        /*FALLTHRU*/
    case POT_HEALING:
        if (!isdiluted || !rn2(2))
        {
            if (Upolyd && u.mh < u.mhmax)
                u.mh++, context.botl = context.botlx = 1;
            if (u.uhp < u.uhpmax)
                u.uhp++, context.botl = context.botlx = 1;
        }
        if (obj->blessed)
            cureblind = TRUE;
        if (cureblind) {
            make_blinded(0L, !u.ucreamed);
            make_deaf(0L, TRUE);
        }
        exercise(A_CON, TRUE);
        break;
    case POT_SICKNESS:
        /* No bad effect, just smell */
        if (!has_innate_breathless(youmonst.data))
        {
            Strcpy(dcbuf, "Ulch!  That potion smells contaminated!");
            pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
        }
        break;
    case POT_POISON:
        if (!Poison_resistance)
        {
            if (Upolyd)
            {
                if (u.mh <= 5)
                    u.mh = 1;
                else
                    u.mh -= 5;
            }
            else
            {
                if (u.uhp <= 5)
                    u.uhp = 1;
                else
                    u.uhp -= 5;
            }
            context.botl = 1;
            exercise(A_CON, FALSE);
        }
        break;
    case POT_HALLUCINATION:
        Strcpy(dcbuf, "You have a momentary vision.");
        pline_ex1(ATR_NONE, CLR_MSG_HALLUCINATED, dcbuf);
        break;
    case POT_ELVEN_HERBAL_BREW:
        if (!Confusion)
        {
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            Strcpy(dcbuf, "You feel somewhat dizzy.");
            pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
        }
        make_confused(itimeout_incr(HConfusion, duration), FALSE);
        break;
    case POT_URINE:
        if (!has_innate_breathless(youmonst.data))
        {
            Strcpy(dcbuf, "Ooph!  That potion smells like urine!");
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        }
        break;
    case POT_DWARVEN_MUSHROOM_BREW:
        if (!has_innate_breathless(youmonst.data))
        {
            Strcpy(dcbuf, "That smells like finely brewed non-alcoholic beverage!");
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        }
        break;
    case POT_CONFUSION:
        if (!Confusion)
        {
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            Strcpy(dcbuf, "You feel somewhat dizzy.");
            pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
        }
        make_confused(itimeout_incr(HConfusion, duration), FALSE);
        break;
    case POT_INVISIBILITY:
        if (!Blind && !Invis) {
            kn++;
            Sprintf(dcbuf, "For an instant you %s!",
                See_invisible ? "could see right through yourself"
                : "couldn't see yourself");
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        }
        break;
    case POT_PARALYSIS:
        kn++;
        if (!Free_action) 
        {
            play_sfx_sound(SFX_ACQUIRE_PARALYSIS);
            Sprintf(dcbuf, "%s seems to be holding you.", Something);
            pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
            incr_itimeout(&HParalyzed, duration);
            refresh_u_tile_gui_info(TRUE);
            context.botl = context.botlx = 1;
            standard_hint("You should acquire paralysis resistance as early as possible. Keep pets around to protect you while paralyzed.", &u.uhint.paralyzed_by_thrown_potion);
#if 0
            nomul(-d(3 - 1 * bcsign(obj), 4)); // rnd(5));
            multi_reason = "frozen by a potion";
            nomovemsg = You_can_move_again;
            nomovemsg_color = CLR_MSG_SUCCESS;
            exercise(A_DEX, FALSE);
#endif
        }
        else
        {
            Strcpy(dcbuf, "You stiffen momentarily.");
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        }
        break;
    case POT_SLEEPING:
        kn++;
        if (!Sleep_resistance) // !Free_action && 
        {
            play_sfx_sound(SFX_ACQUIRE_SLEEP);
            Strcpy(dcbuf, "You feel rather tired.");
            pline_ex1(ATR_NONE, CLR_MSG_WARNING, dcbuf);
            fall_asleep(-duration, FALSE);
            standard_hint("You should acquire sleep resistance as early as possible. Keep pets around to protect you while sleeping.", &u.uhint.paralyzed_by_thrown_potion);
        }
        else
        {
            Strcpy(dcbuf, "You yawn.");
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        }
        break;
    case POT_SPEED:
    case POT_GREATER_SPEED:
    case POT_LIGHTNING_SPEED:
        if (obj->otyp == POT_LIGHTNING_SPEED ? !Lightning_fast : obj->otyp == POT_LIGHTNING_SPEED ? !Lightning_fast && !Super_fast : !Lightning_fast && !Super_fast && !Ultra_fast)
        {
            Strcpy(dcbuf, "Your knees seem more flexible now.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        play_sfx_sound(SFX_ACQUIRE_HASTE);
        incr_itimeout(obj->otyp == POT_LIGHTNING_SPEED ? &HLightning_fast : obj->otyp == POT_GREATER_SPEED ? &HSuper_fast : &HUltra_fast, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_DEX, TRUE);
        break;
    case POT_MAGIC_RESISTANCE:
        if (!Antimagic)
        {
            Strcpy(dcbuf, "You feel a bit more protected now.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(&HAntimagic, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_TITAN_STRENGTH:
        if (!Titan_strength)
        {
            Strcpy(dcbuf, "You feel a bit stronger than before.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(&HTitan_strength, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_FIRE_IMMUNITY:
        if (!Fire_immunity)
        {
            Strcpy(dcbuf, "You feel a bit more fire-protected now.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(&HFire_immunity, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_COLD_IMMUNITY:
        if (!Cold_immunity)
        {
            Strcpy(dcbuf, "You feel a bit more cold-protected now.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(&HCold_immunity, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_SHOCK_IMMUNITY:
        if (!Shock_immunity)
        {
            Strcpy(dcbuf, "You feel a bit more electricity-protected now.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(&HShock_immunity, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_HEROISM:
    case POT_SUPER_HEROISM:
        if (obj->otyp == POT_SUPER_HEROISM ? !Super_heroism : !Super_heroism && !Heroism)
        {
            Strcpy(dcbuf, "You feel a bit more heroic.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(obj->otyp == POT_SUPER_HEROISM ? &HSuper_heroism : &HHeroism, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_LESSER_REGENERATION:
    case POT_REGENERATION:
    case POT_GREATER_REGENERATION:
        if (obj->otyp == POT_GREATER_REGENERATION ? !Divine_regeneration :
            obj->otyp == POT_REGENERATION ? !Divine_regeneration && !Rapidest_regeneration : !Super_heroism && !Heroism)
        {
            Strcpy(dcbuf, "You feel a bit more regenerative.");
            pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, dcbuf);
        }
        incr_itimeout(obj->otyp == POT_SUPER_HEROISM ? &HSuper_heroism : &HHeroism, duration);
        refresh_u_tile_gui_info(TRUE);
        exercise(A_WIS, TRUE);
        break;
    case POT_BLINDNESS:
    {
        boolean was_blind = Blind;
        make_blinded(itimeout_incr(Blinded, duration), FALSE);
        if (Blind && !Unaware && !was_blind) 
        {
            kn++;
            play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
            Strcpy(dcbuf, "It suddenly gets dark.");
            pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, dcbuf);
        }
        //if (!Blind && !Unaware)
        //    Your1(vision_clears);
        break;
    }
    case POT_WATER:
        if (u.umonnum == PM_GREMLIN) {
            (void) split_mon(&youmonst, (struct monst *) 0);
        } else if (u.ulycn >= LOW_PM) {
            /* vapor from [un]holy water will trigger
               transformation but won't cure lycanthropy */
            if (obj->blessed && youmonst.data == &mons[u.ulycn])
                you_unwere(FALSE);
            else if (obj->cursed && !Upolyd)
                you_were();
        }
        break;
    case POT_ACID:
    case POT_POLYMORPH:
        exercise(A_CON, FALSE);
        break;
    /*
    case POT_GAIN_LEVEL:
    case POT_LEVITATION:
    case POT_FRUIT_JUICE:
    case POT_MONSTER_DETECTION:
    case POT_OBJECT_DETECTION:
    case POT_OIL:
        break;
     */
    }

    char dcbuf2[IBUFSZ] = "";
    if (introline && *introline)
    {
        Strcpy(dcbuf2, introline);
    }
    if (*dcbuf)
    {
        if(*dcbuf2)
            Strcat(dcbuf2, " ");
        Strcat(dcbuf2, dcbuf);
    }

    /* note: no obfree() -- that's our caller's responsibility */
    if (obj->dknown) {
        if (kn)
            makeknown(obj->otyp);
        else if (!objects[obj->otyp].oc_name_known
                 && !objects[obj->otyp].oc_uname)
            docall(obj, dcbuf2);
    }
}

/* returns the potion type when o1 is dipped in o2 */
STATIC_OVL short
mixtype(o1, o2)
struct obj* o1, * o2;
{
    short mixtyp = mixtype_core(o1, o2);
    /* try the other way around for potions */
    if (mixtyp == STRANGE_OBJECT && (o1->oclass == POTION_CLASS && o2->oclass == POTION_CLASS))
        mixtyp = mixtype_core(o2, o1);
    return mixtyp;
}

STATIC_OVL short
mixtype_core(o1, o2)
struct obj *o1, *o2;
{
    switch (o1->otyp) {
    case POT_POISON:
    case POT_SICKNESS:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_RESTORE_ABILITY:
            return POT_WATER;
        case JAR_OF_BASILISK_BLOOD:
            return POT_PARALYSIS;
        }
        break;
    case POT_WATER:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_WATER;
        case JAR_OF_EXTRA_HEALING_SALVE:
            return POT_HEALING;
        case JAR_OF_GREATER_HEALING_SALVE:
            return POT_EXTRA_HEALING;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
            return POT_GREATER_HEALING;
        case GRAIL_OF_HEALING:
            return POT_FULL_HEALING;
        default:
            if (o1->oclass == POTION_CLASS && o2->oclass == POTION_CLASS)
                return o2->otyp; /* Becomes diluted version of the potion dipped into */
        }
        break;
    case POT_LESSER_REGENERATION:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case POT_LESSER_REJUVENATION:
        case POT_GAIN_ENERGY:
            return POT_LESSER_REJUVENATION;
        case POT_REJUVENATION:
        case POT_EXTRA_ENERGY:
            return POT_REJUVENATION;
        case POT_GREATER_REJUVENATION:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
            return POT_GREATER_REJUVENATION;
        case POT_HEALING:
            return POT_LESSER_REGENERATION;
        case POT_SPEED:
        case POT_EXTRA_HEALING:
        case JAR_OF_EXTRA_HEALING_SALVE:
            return POT_REGENERATION;
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case POT_GREATER_HEALING:
        case POT_FULL_HEALING:
        case GRAIL_OF_HEALING:
            return POT_GREATER_REGENERATION;
        case POT_GAIN_LEVEL:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_REGENERATION:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case POT_GAIN_ENERGY:
            return POT_LESSER_REJUVENATION;
        case POT_LESSER_REJUVENATION:
        case POT_EXTRA_ENERGY:
            return POT_REJUVENATION;
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_REJUVENATION:
        case POT_GREATER_REJUVENATION:
            return POT_GREATER_REJUVENATION;
        case POT_LESSER_REGENERATION:
        case POT_HEALING:
        case POT_EXTRA_HEALING:
        case JAR_OF_EXTRA_HEALING_SALVE:
            return POT_REGENERATION;
        case POT_SPEED:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case POT_GREATER_HEALING:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_FULL_HEALING:
        case POT_GREATER_REGENERATION:
            return POT_GREATER_REGENERATION;
        case POT_GAIN_LEVEL:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        }
        break;    
    case POT_GREATER_REGENERATION:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case POT_GAIN_ENERGY:
        case POT_LESSER_REJUVENATION:
            return POT_REJUVENATION;
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_REJUVENATION:
        case POT_GREATER_REJUVENATION:
            return POT_GREATER_REJUVENATION;
        case POT_SPEED:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_HEALING:
        case POT_EXTRA_HEALING:
        case POT_GREATER_HEALING:
        case POT_FULL_HEALING:
            return POT_GREATER_REGENERATION;
        case POT_GAIN_LEVEL:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_LESSER_REJUVENATION:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_REJUVENATION:
        case POT_REGENERATION:
        case POT_GREATER_REGENERATION:
            return POT_REJUVENATION;
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GREATER_REJUVENATION:
            return POT_GREATER_REJUVENATION;
        case POT_HEALING:
            return POT_LESSER_REGENERATION;
        case JAR_OF_EXTRA_HEALING_SALVE:
        case POT_EXTRA_HEALING:
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_GREATER_HEALING:
            return POT_REGENERATION;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_FULL_HEALING:
            return POT_GREATER_REGENERATION;
        case POT_SPEED:
            return POT_REJUVENATION;
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
            return POT_GREATER_REJUVENATION;
        case POT_GAIN_LEVEL:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_REJUVENATION:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_REJUVENATION:
        case POT_REGENERATION:
        case POT_GREATER_REGENERATION:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GREATER_REJUVENATION:
            return POT_GREATER_REJUVENATION;
        case POT_HEALING:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case POT_EXTRA_HEALING:
            return POT_REGENERATION;
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_GREATER_HEALING:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_FULL_HEALING:
            return POT_GREATER_REGENERATION;
        case POT_SPEED:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
            return POT_GREATER_REJUVENATION;
        case POT_GAIN_LEVEL:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_GREATER_REJUVENATION:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_REJUVENATION:
        case POT_REGENERATION:
        case POT_GREATER_REGENERATION:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GREATER_REJUVENATION:
            return POT_GREATER_REJUVENATION;
        case POT_HEALING:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case POT_EXTRA_HEALING:
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_GREATER_HEALING:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_FULL_HEALING:
            return POT_GREATER_REGENERATION;
        case POT_SPEED:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
            return POT_GREATER_REJUVENATION;
        case POT_GAIN_LEVEL:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_HEROISM:
        switch (o2->otyp) {
        case POT_GAIN_ENERGY:
        case POT_SPEED:
            return POT_HEROISM;
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case POT_GAIN_LEVEL:
        case POT_SUPER_HEROISM:
            return POT_SUPER_HEROISM;
        }
        break;
    case POT_SUPER_HEROISM:
        switch (o2->otyp) {
        case POT_GAIN_ENERGY:
        case POT_SPEED:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case POT_GAIN_LEVEL:
        case POT_HEROISM:
        case POT_SUPER_HEROISM:
            return POT_SUPER_HEROISM;
        }
        break;
    case POT_SPEED:
        switch (o2->otyp) {
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_SPEED:
            return POT_GREATER_SPEED;
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GAIN_LEVEL:
        case POT_LIGHTNING_SPEED:
            return POT_LIGHTNING_SPEED;
        }
        break;
    case POT_GREATER_SPEED:
        switch (o2->otyp) {
        case POT_SPEED:
            return POT_GREATER_SPEED;
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GAIN_LEVEL:
        case POT_LIGHTNING_SPEED:
            return POT_LIGHTNING_SPEED;
        }
        break;
    case POT_LIGHTNING_SPEED:
        switch (o2->otyp) {
        case POT_SPEED:
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_SPEED:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GAIN_LEVEL:
        case POT_LIGHTNING_SPEED:
            return POT_LIGHTNING_SPEED;
        }
        break;
    case POT_HEALING:
        switch (o2->otyp) {
        case POT_SPEED:
        case POT_GAIN_ENERGY:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case POT_EXTRA_ENERGY:
        case POT_EXTRA_HEALING:
            return POT_EXTRA_HEALING;
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_GREATER_ENERGY:
        case POT_GREATER_SPEED:
        case POT_GREATER_HEALING:
            return POT_GREATER_HEALING;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_FULL_ENERGY:
        case POT_LIGHTNING_SPEED:
        case POT_FULL_HEALING:
            return POT_FULL_HEALING;
        case POT_GAIN_LEVEL:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_EXTRA_HEALING:
        switch (o2->otyp) {
        case JAR_OF_EXTRA_HEALING_SALVE:
        case POT_HEALING:
            return POT_EXTRA_HEALING;
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_SPEED:
        case POT_GREATER_HEALING:
            return POT_GREATER_HEALING;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case POT_FULL_HEALING:
            return POT_FULL_HEALING;
        case POT_GAIN_LEVEL:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_GREATER_HEALING:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_HEALING:
        case POT_EXTRA_HEALING:
            return POT_GREATER_HEALING;
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_FULL_ENERGY:
        case POT_FULL_HEALING:
            return POT_FULL_HEALING;
        case POT_GAIN_LEVEL:
            return POT_GAIN_ABILITY;
        }
        break;
    case POT_FULL_HEALING:
        switch (o2->otyp) {
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_HEALING:
        case POT_EXTRA_HEALING:
        case POT_GREATER_HEALING:
            return POT_FULL_HEALING;
        case POT_GAIN_LEVEL:
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
            return POT_GAIN_ABILITY;
        }
        break;
    case UNICORN_HORN:
        switch (o2->otyp) {
        case POT_SICKNESS:
            return POT_FRUIT_JUICE;
        case POT_HALLUCINATION:
        case POT_BLINDNESS:
        case POT_CONFUSION:
            return POT_WATER;
        }
        break;
    case AMETHYST:
        if (o2->otyp == POT_ELVEN_HERBAL_BREW)
            return POT_FRUIT_JUICE;
        break;
    case POT_GAIN_LEVEL:
        switch (o2->otyp) {
        case POT_CONFUSION:
            return (rn2(3) ? POT_ELVEN_HERBAL_BREW : POT_ENLIGHTENMENT);
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
            return POT_FULL_HEALING;
        case POT_HEALING:
        case POT_EXTRA_HEALING:
        case POT_GREATER_HEALING:
        case POT_FULL_HEALING:
        case POT_RESTORE_ABILITY:
            return POT_GAIN_ABILITY;
        case POT_FRUIT_JUICE:
            return POT_SEE_INVISIBLE;
        case POT_ELVEN_HERBAL_BREW:
            return POT_HALLUCINATION;
        }
        break;
    case POT_GAIN_ENERGY:
    case POT_EXTRA_ENERGY:
        switch (o2->otyp) {
        case POT_CONFUSION:
            return (rn2(3) ? POT_ELVEN_HERBAL_BREW : POT_ENLIGHTENMENT);
        case JAR_OF_MEDICINAL_SALVE:
        case POT_RESTORE_ABILITY:
            return POT_RESTORE_ABILITY;
        case JAR_OF_EXTRA_HEALING_SALVE:
        case POT_HEALING:
            return POT_EXTRA_HEALING;
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_EXTRA_HEALING:
            return POT_GREATER_HEALING;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_GREATER_HEALING:
            return POT_FULL_HEALING;
        case POT_FULL_HEALING:
            return POT_GAIN_ABILITY;
        case POT_FRUIT_JUICE:
            return POT_SEE_INVISIBLE;
        case POT_ELVEN_HERBAL_BREW:
            return POT_HALLUCINATION;
        case JAR_OF_BASILISK_BLOOD:
            return POT_SPEED;
        }
        break;
    case POT_GREATER_ENERGY:
        switch (o2->otyp) {
        case POT_CONFUSION:
            return (rn2(3) ? POT_ELVEN_HERBAL_BREW : POT_ENLIGHTENMENT);
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_EXTRA_HEALING_SALVE:
            return POT_EXTRA_HEALING;
        case JAR_OF_GREATER_HEALING_SALVE:
        case POT_HEALING:
            return POT_GREATER_HEALING;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_EXTRA_HEALING:
            return POT_FULL_HEALING;
        case POT_GREATER_HEALING:
            return POT_FULL_HEALING;
        case POT_RESTORE_ABILITY:
        case POT_FULL_HEALING:
            return POT_GAIN_ABILITY;
        case POT_FRUIT_JUICE:
            return POT_SEE_INVISIBLE;
        case POT_ELVEN_HERBAL_BREW:
            return POT_HALLUCINATION;
        case JAR_OF_BASILISK_BLOOD:
            return POT_GREATER_SPEED;
        }
        break;
    case POT_FULL_ENERGY:
        switch (o2->otyp) {
        case POT_CONFUSION:
            return (rn2(3) ? POT_ELVEN_HERBAL_BREW : POT_ENLIGHTENMENT);
        case JAR_OF_MEDICINAL_SALVE:
            return POT_RESTORE_ABILITY;
        case JAR_OF_EXTRA_HEALING_SALVE:
            return POT_EXTRA_HEALING;
        case JAR_OF_GREATER_HEALING_SALVE:
            return POT_GREATER_HEALING;
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
        case POT_HEALING:
            return POT_FULL_HEALING;
        case POT_EXTRA_HEALING:
            return POT_FULL_HEALING;
        case POT_GREATER_HEALING:
            return POT_FULL_HEALING;
        case POT_RESTORE_ABILITY:
        case POT_FULL_HEALING:
            return POT_GAIN_ABILITY;
        case POT_FRUIT_JUICE:
            return POT_SEE_INVISIBLE;
        case POT_ELVEN_HERBAL_BREW:
            return POT_HALLUCINATION;
        case JAR_OF_BASILISK_BLOOD:
            return POT_LIGHTNING_SPEED;
        }
        break;
    case POT_FRUIT_JUICE:
        switch (o2->otyp) {
        case POT_SICKNESS:
            return POT_SICKNESS;
        case POT_ENLIGHTENMENT:
        case POT_SPEED:
            return POT_ELVEN_HERBAL_BREW;
        case POT_GAIN_LEVEL:
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
            return POT_SEE_INVISIBLE;
        }
        break;
    case POT_RESTORE_ABILITY:
        switch (o2->otyp) {
        case POT_SICKNESS:
        case POT_POISON:
            return POT_WATER;
        case POT_GREATER_SPEED:
        case POT_LIGHTNING_SPEED:
        case POT_GAIN_LEVEL:
        case POT_GREATER_ENERGY:
        case POT_FULL_ENERGY:
            return POT_GAIN_ABILITY;
        case POT_SPEED:
        case POT_GAIN_ENERGY:
        case POT_EXTRA_ENERGY:
        case POT_HEALING:
        case POT_EXTRA_HEALING:
        case POT_GREATER_HEALING:
        case POT_FULL_HEALING:
        case JAR_OF_MEDICINAL_SALVE:
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case GRAIL_OF_HEALING:
            return POT_RESTORE_ABILITY;
        }
        break;
    case POT_ENLIGHTENMENT:
        switch (o2->otyp) {
        case POT_LEVITATION:
            if (rn2(3))
                return POT_GAIN_LEVEL;
            break;
        case POT_FRUIT_JUICE:
            return POT_ELVEN_HERBAL_BREW;
        case POT_ELVEN_HERBAL_BREW:
            return POT_CONFUSION;
        }
        break;
    }

    return STRANGE_OBJECT;
}

STATIC_VAR const char Dip_[] = "Dip ";
/* #dip command */
int
dodip()
{
    struct obj *potion, *obj;
    struct obj *singlepotion;
    uchar here;
    char allowall[2];
    short mixture;
    char qbuf[QBUFSZ], obuf[QBUFSZ];
    const char *shortestname; /* last resort obj name for prompt */

    allowall[0] = ALL_CLASSES;
    allowall[1] = '\0';
    if (!(obj = getobj(allowall, "dip", 0, "")))
        return 0;
    if (inaccessible_equipment(obj, "dip", FALSE, FALSE))
        return 0;

    shortestname = (is_plural(obj) || pair_of(obj)) ? "them" : "it";
    /*
     * Bypass safe_qbuf() since it doesn't handle varying suffix without
     * an awful lot of support work.  Format the object once, even though
     * the fountain and pool prompts offer a lot more room for it.
     * 3.6.0 used thesimpleoname() unconditionally, which posed no risk
     * of buffer overflow but drew bug reports because it omits user-
     * supplied type name.
     * getobj: "What do you want to dip <the object> into? [xyz or ?*] "
     */
    Strcpy(obuf, short_oname(obj, doname, thesimpleoname,
                             /* 128 - (24 + 54 + 1) leaves 49 for <object> */
                             QBUFSZ - sizeof "What do you want to dip \
 into? [abdeghjkmnpqstvwyzBCEFHIKLNOQRTUWXZ#-# or ?*] "));

    here = levl[u.ux][u.uy].typ;
    /* Is there a fountain to dip into here? */
    if (IS_FOUNTAIN(here)) 
    {
        Sprintf(qbuf, "%s%s into the %s?", Dip_, flags.verbose ? obuf : shortestname, get_fountain_name(u.ux, u.uy));
        
        /* "Dip <the object> into the fountain?" */
        if (yn_query(qbuf) == 'y') 
        {
            dipfountain(obj);
            return 1;
        }
    }
    else if (IS_SINK(here))
    {
        Sprintf(qbuf, "Put %s into the sink and open tap?", flags.verbose ? obuf : shortestname);

        /* "Dip <the object> into the sink?" */
        if (yn_query(qbuf) == 'y')
        {
            if (Levitation && !Levitation_control)
            {
                floating_above("sink");
            }
            else
            {
                play_sfx_sound(SFX_SINK_OPEN_TAP);
                You("open the tap and let the water run.");
                if (iflags.using_gui_sounds)
                {
                    delay_output_milliseconds(1000);
                }
                if (obj->otyp == POT_ACID)
                    obj->in_use = 1;
                if (water_damage(obj, 0, TRUE) != ER_DESTROYED && obj->in_use)
                    useup(obj);
                if (!rn2(20))
                {
                    /* The same probability as with drinking from the sink */
                    breaksink(u.ux, u.uy);
                }
            }
            return 1;
        }
    }
    else if (is_pool(u.ux, u.uy))
    {
        const char *pooltype = waterbody_name(u.ux, u.uy);

        Sprintf(qbuf, "%s%s into the %s?", Dip_,
                flags.verbose ? obuf : shortestname, pooltype);

        /* "Dip <the object> into the {pool, moat, &c}?" */
        if (yn_query(qbuf) == 'y')
        {
            if (Levitation && !Levitation_control) 
            {
                floating_above(pooltype);
            }
            else if (u.usteed && !is_swimmer(u.usteed->data) && P_SKILL_LEVEL(P_RIDING) < P_BASIC) 
            {
                rider_cant_reach(); /* not skilled enough to reach */
            } 
            else 
            {
                if (obj->otyp == POT_ACID)
                    obj->in_use = 1;
                if (water_damage(obj, 0, TRUE) != ER_DESTROYED && obj->in_use)
                    useup(obj);
            }
            return 1;
        }
    }

    /* "What do you want to dip <the object> into? [xyz or ?*] " */
    Sprintf(qbuf, "dip %s into", flags.verbose ? obuf : shortestname);
    potion = getobj(beverages, qbuf, 0, "");
    if (!potion)
        return 0;
    if (potion == obj && potion->quan == 1L)
    {
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "That is a potion bottle, not a Klein bottle!");
        return 0;
    }
    potion->in_use = TRUE; /* assume it will be used up */
    if (potion->otyp == POT_WATER)
    {
        boolean useeit = !Blind || (obj == ublindf && Blind_because_of_blindfold_only);
        const char *obj_glows = Yobjnam2(obj, "glow");

        if (H2Opotion_dip(potion, obj, useeit, obj_glows))
            goto poof;
    }
    else if (obj->otyp == POT_POLYMORPH || potion->otyp == POT_POLYMORPH)
    {
        /* some objects can't be polymorphed */
        if (obj->otyp == potion->otyp /* both POT_POLY */
            || obj->otyp == WAN_POLYMORPH || obj->otyp == SPE_POLYMORPH
            || obj == uball || obj == uskin
            || obj_resists(obj->otyp == POT_POLYMORPH ? potion : obj,
                           5, 95))
        {
            pline1(nothing_happens);
        } 
        else 
        {
            short save_otyp = obj->otyp;

            /* KMH, conduct */
            if (!u.uconduct.polypiles++)
                livelog_printf(LL_CONDUCT, "polymorphed %s first item", uhis());

            obj = poly_obj(obj, STRANGE_OBJECT);

            /*
             * obj might be gone:
             *  poly_obj() -> set_wear() -> Amulet_on() -> useup()
             * if obj->otyp is worn amulet and becomes AMULET_OF_CHANGE.
             */
            if (!obj)
            {
                makeknown(POT_POLYMORPH);
                return 1;
            } 
            else if (obj->otyp != save_otyp) 
            {
                makeknown(POT_POLYMORPH);
                useup(potion);
                prinv((char *) 0, obj, 0L);
                return 1;
            } 
            else
            {
                pline("Nothing seems to happen.");
                goto poof;
            }
        }
        potion->in_use = FALSE; /* didn't go poof */
        return 1;
    }
    else if (otyp_expends_charges_when_dipped_into(potion->otyp) && potion->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s empty. You cannot dip %s into %s.", Tobjnam(potion, "are"), acxname(obj), potion->quan > 1 ? "them" : "it");
        if (objects[obj->otyp].oc_uses_known)
            obj->known = 1;
        if (obj->where == OBJ_INVENT)
            update_inventory();
        return 1;
    }
    else if (obj->oclass == POTION_CLASS && potion->oclass != POTION_CLASS
        && !obj_currently_allows_object_to_be_dipped_into_it(potion))
    {
        /* Dipping a potion into an empty Holy Grail, an empty jar, or into a lamp etc. */
        if (obj->otyp == POT_OIL && is_refillable_with_oil(potion))
        {
            /* Switch these around */
            struct obj* obj_temp = obj;
            obj = potion;
            potion = obj_temp;
        }
        else if (Is_proper_container(potion) && !potion->olocked)
        {
            if (!stash_obj_in_container(obj, potion))
            {
                /* couldn't put selected item into container for some
                   reason; might need to undo splitobj() */
                (void)unsplitobj(obj);
            }
            return 1;
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "You cannot dip %s into %s.", acxname(obj), acxname(potion));
            return 1;
        }
    }
    else if (obj->oclass == POTION_CLASS && obj->otyp != potion->otyp)
    {
        int amt = (int) obj->quan;
        boolean magic;
        char dcbuf[BUFSZ] = "";
        boolean useupliquidonly = otyp_expends_charges_when_dipped_into(potion->otyp) && potion->charges > 0;
        boolean neutralizingliquid = !!(objects[potion->otyp].oc_flags5 & O5_MIXTURE_CLEARS);

        mixture = mixtype(obj, potion);
        if (mixture == obj->otyp) /* Can happen with a potion of extra healing and a jar of extra healing salve, for example */
        {
            potion->in_use = FALSE; /* didn't go poof */
            pline("Nothing seems to happen.");
            return 1;
        }
        else
        {
            magic = (mixture != STRANGE_OBJECT) ? objects[mixture].oc_magic
                : (objects[obj->otyp].oc_magic || objects[potion->otyp].oc_magic);
            Strcpy(qbuf, "The"); /* assume full stack */
            if (amt > (magic ? 3 : 7)) {
                /* trying to dip multiple potions will usually affect only a
                   subset; pick an amount between 3 and 8, inclusive, for magic
                   potion result, between 7 and N for non-magic */
                if (magic)
                    amt = rnd(min(amt, 8) - (3 - 1)) + (3 - 1); /* 1..6 + 2 */
                else
                    amt = rnd(amt - (7 - 1)) + (7 - 1); /* 1..(N-6) + 6 */

                if ((int64_t)amt < obj->quan)
                {
                    obj = splitobj(obj, (int64_t)amt);
                    Sprintf(qbuf, "%lld of the", (long long)obj->quan);
                }
            }
            /* [N of] the {obj(s)} mix(es) with [one of] {the potion}... */
            Sprintf(dcbuf, "%s %s %s with %s%s%s...", qbuf, simpleonames(obj),
                otense(obj, "mix"), useupliquidonly ? "the liquid inside " : "",
                (potion->quan > 1L) ? "one of " : "",
                thesimpleoname(potion));
            pline_ex1(ATR_NONE, NO_COLOR, dcbuf);

            if (useupliquidonly)
            {
                if (obj->quan > (int64_t)potion->charges)
                    potion->charges = 0;
                else
                    potion->charges -= (short)obj->quan;
            }

            /* Mixing potions is dangerous...
               KMH, balance patch -- acid is particularly unstable */
            if (obj->cursed || obj->otyp == POT_ACID || !rn2(10))
            {
                if (useupliquidonly)
                    potion->charges = 0;
                if (!potion->oartifact && !is_obj_indestructible(potion))
                    useup(potion); /* now gone */

                char dcbuf2[IBUFSZ] = "";
                char dcbuf3[IBUFSZ] = "";
                /* it would be better to use up the whole stack in advance
                   of the message, but we can't because we need to keep it
                   around for potionbreathe() [and we can't set obj->in_use
                   to 'amt' because that's not implemented] */

                play_sfx_sound(SFX_EXPLOSION_MAGICAL);
                obj->in_use = 1;
                Strcpy(dcbuf2, "BOOM!  They explode!");
                pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, dcbuf2);
                Sprintf(dcbuf3, "%s %s", dcbuf, dcbuf2);
                wake_nearto(u.ux, u.uy, EXPLOSION_SOUND_RADIUS * EXPLOSION_SOUND_RADIUS);
                exercise(A_STR, FALSE);
                if (!has_innate_breathless(youmonst.data) || haseyes(youmonst.data))
                    potionbreathe(obj, dcbuf3);
                useupall(obj);
                losehp(adjust_damage(amt + rnd(9), (struct monst*)0, &youmonst, AD_MAGM, ADFLAGS_NONE), /* not physical damage */
                    "alchemic blast", KILLED_BY_AN);
                return 1;
            }

            /* get rid of 'dippee' before potential perm_invent updates */
            if (!useupliquidonly && !potion->oartifact && !is_obj_indestructible(potion))
                useup(potion); /* now gone */

            obj->blessed = obj->cursed = obj->bknown = 0;
            if (Blind || Hallucination)
                obj->dknown = 0;

            if (mixture != STRANGE_OBJECT)
            {
                obj->otyp = mixture;
            }
            else
            {
                switch (obj->odiluted ? 1 : rnd(8))
                {
                case 1:
                    obj->otyp = POT_WATER;
                    break;
                case 2:
                    obj->otyp = neutralizingliquid ? POT_WATER : POT_POISON;
                    break;
                case 3:
                    obj->otyp = neutralizingliquid ? POT_WATER : POT_SICKNESS;
                    break;
                case 4: {
                    struct obj* otmp = mkobj(POTION_CLASS, FALSE, FALSE);
                    obj->otyp = otmp->otyp;
                    obfree(otmp, (struct obj*)0);
                    if(neutralizingliquid && (obj->otyp == POT_SICKNESS || obj->otyp == POT_POISON))
                        obj->otyp = POT_WATER;
                    break;
                }
                default:
                    useupall(obj);
                    if (!Blind)
                    {
                        play_sfx_sound(SFX_SOME_WATER_EVAPORATES);
                        pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "mixture glows brightly and evaporates.");
                    }
                    return 1;
                }
            }
            obj->odiluted = (obj->otyp != POT_WATER);

            if (obj->otyp == POT_WATER && !Hallucination)
            {
                play_sfx_sound(SFX_STEAMY_BUBBLES);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "mixture bubbles%s.", Blind ? "" : ", then clears");
            }
            else if (!Blind)
            {
                play_sfx_sound(SFX_MIXING_SUCCESS);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "mixture looks %s.",
                    hcolor(OBJ_DESCR(objects[obj->otyp])));
            }

            /* this is required when 'obj' was split off from a bigger stack,
               so that 'obj' will now be assigned its own inventory slot;
               it has a side-effect of merging 'obj' into another compatible
               stack if there is one, so we do it even when no split has
               been made in order to get the merge result for both cases;
               as a consequence, mixing while Fumbling drops the mixture */
            freeinv(obj);
            (void)hold_another_object(obj, "You drop %s!", doname(obj),
                (const char*)0);
            return 1;
        }
    }

    if (potion->otyp == POT_ACID && obj->otyp == CORPSE
        && (obj->corpsenm == PM_LICHEN || obj->corpsenm == PM_WHITE_LICHEN || obj->corpsenm == PM_BLACK_LICHEN) && !Blind) 
    {
        pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s %s %s around the edges.", The(cxname(obj)),
              otense(obj, "turn"),
              hcolor_multi_buf2(potion->odiluted ? NH_ORANGE : NH_RED));
        potion->in_use = FALSE; /* didn't go poof */
        return 1;
    }

    if (potion->otyp == POT_WATER && obj->otyp == TOWEL) 
    {
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "towel soaks it up!");
        /* wetting towel already done via water_damage() in H2Opotion_dip */
        goto poof;
    }

    if (is_poisonable(obj)) 
    {
        if ((potion->otyp == POT_POISON) && !obj->opoisoned)
        {
            char buf[BUFSZ];

            if (potion->quan > 1L)
                Sprintf(buf, "One of %s", the(xname(potion)));
            else
                Strcpy(buf, The(xname(potion)));

            play_sfx_sound(SFX_POISON_COATING);
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s forms a coating on %s.", buf, the(xname(obj)));
            obj->opoisoned = TRUE;
            goto poof;
        } 
        else if (obj->opoisoned && (potion->otyp == POT_HEALING
                                      || potion->otyp == POT_EXTRA_HEALING || potion->otyp == POT_GREATER_HEALING
                                      || potion->otyp == POT_FULL_HEALING  || potion->otyp == JAR_OF_EXTRA_HEALING_SALVE
                                      || potion->otyp == JAR_OF_GREATER_HEALING_SALVE || potion->otyp == JAR_OF_PRODIGIOUS_HEALING_SALVE
                                      || potion->otyp == JAR_OF_MEDICINAL_SALVE
            ))
        {
            play_sfx_sound(SFX_POISON_DISSOLVES);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "A coating wears off %s.", the(xname(obj)));
            obj->opoisoned = 0;
            goto poof;
        }
    }

    if (potion->otyp == POT_ACID)
    {
        if (erode_obj(obj, 0, ERODE_CORRODE, EF_GREASE) != ER_NOTHING)
            goto poof;
    }

    if (potion->otyp == POT_OIL) 
    {
        boolean wisx = FALSE;

        if (potion->lamplit) { /* burning */
            fire_damage(obj, TRUE, u.ux, u.uy);
        } 
        else if (potion->cursed) 
        {
            play_sfx_sound(SFX_ACQUIRE_GLIB);
            pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "potion spills and covers your %s with oil.",
                      makeplural(body_part(FINGER)));
            incr_itimeout(&Glib, d(2, 10));
            refresh_u_tile_gui_info(TRUE);
        }
        else if (obj->oclass != WEAPON_CLASS && !is_weptool(obj)) 
        {
            /* the following cases apply only to weapons */
            goto more_dips;
            /* Oil removes rust and corrosion, but doesn't unburn.
             * Arrows, etc are classed as metallic due to arrowhead
             * material, but dipping in oil shouldn't repair them.
             */
        } 
        else if ((!is_rustprone(obj) && !is_corrodeable(obj))
                   || is_ammo(obj) || (!obj->oeroded && !obj->oeroded2))
        {
            /* uses up potion, doesn't set obj->greased */
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s %s with an oily sheen.", Yname2(obj),
                  otense(obj, "gleam"));
        } 
        else
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s %s less %s.", Yname2(obj), otense(obj, "are"),
                  (obj->oeroded && obj->oeroded2)
                      ? "corroded and rusty"
                      : obj->oeroded ? "rusty" : "corroded");
            if (obj->oeroded > 0)
                obj->oeroded--;
            if (obj->oeroded2 > 0)
                obj->oeroded2--;
            wisx = TRUE;
        }
        exercise(A_WIS, wisx);
        makeknown(potion->otyp);
        useup(potion);
        return 1;
    }
 more_dips:

    /* Allow filling of MAGIC_LAMPs to prevent identification by player */
    if (is_refillable_with_oil(obj) && potion->otyp == POT_OIL) 
    {
        return refill_obj_with_oil(obj, potion);

    }
    else if (potion->otyp == JAR_OF_BASILISK_BLOOD)
    {
        return stone_to_flesh_obj(obj);
    }

    potion->in_use = FALSE; /* didn't go poof */
    if ((obj->otyp == UNICORN_HORN || obj->otyp == AMETHYST)
        && (mixture = mixtype(obj, potion)) != STRANGE_OBJECT) {
        char oldbuf[BUFSZ], newbuf[BUFSZ];
        short old_otyp = potion->otyp;
        boolean old_dknown = FALSE;
        boolean more_than_one = potion->quan > 1L;

        oldbuf[0] = '\0';
        if (potion->dknown) {
            old_dknown = TRUE;
            Sprintf(oldbuf, "%s ", hcolor(OBJ_DESCR(objects[potion->otyp])));
        }
        /* with multiple merged potions, split off one and
           just clear it */
        if (potion->quan > 1L) {
            singlepotion = splitobj(potion, 1L);
        } else
            singlepotion = potion;

        costly_alteration(singlepotion, COST_NUTRLZ);
        singlepotion->otyp = mixture;
        singlepotion->blessed = 0;
        if (mixture == POT_WATER)
            singlepotion->cursed = singlepotion->odiluted = 0;
        else
            singlepotion->cursed = obj->cursed; /* odiluted left as-is */
        singlepotion->bknown = FALSE;
        if (Blind) {
            singlepotion->dknown = FALSE;
        } else {
            singlepotion->dknown = !Hallucination;
            if (mixture == POT_WATER && singlepotion->dknown)
                Sprintf(newbuf, "clears");
            else
                Sprintf(newbuf, "turns %s",
                        hcolor(OBJ_DESCR(objects[mixture])));
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%spotion%s %s.", oldbuf,
                      more_than_one ? " that you dipped into" : "", newbuf);
            if (!objects[old_otyp].oc_uname
                && !objects[old_otyp].oc_name_known && old_dknown) {
                struct obj fakeobj;
                fakeobj = zeroobj;
                fakeobj.dknown = 1;
                fakeobj.otyp = old_otyp;
                fakeobj.oclass = POTION_CLASS;
                docall(&fakeobj, (char*)0);
            }
        }
        Strcpy(debug_buf_2, "dodip");
        obj_extract_self(singlepotion);
        singlepotion = hold_another_object(singlepotion,
                                           "You juggle and drop %s!",
                                           doname(singlepotion),
                                           (const char *) 0);
        nhUse(singlepotion);
        update_inventory();
        return 1;
    }

    play_sfx_sound(SFX_GENERAL_THAT_DID_NOTHING);
    pline("Interesting...");
    return 1;

 poof:
    if (!objects[potion->otyp].oc_name_known
        && !objects[potion->otyp].oc_uname)
        docall(potion, (char*)0);
    useup(potion);
    return 1;
}

int
refill_obj_with_oil(obj, potion)
struct obj* obj, * potion;
{
    if (!obj || !potion || !is_refillable_with_oil(obj))
        return 0;

    /* Turn off engine before fueling, turn off fuel too :-)  */
    if (obj->lamplit || potion->lamplit)
    {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s fire!", Tobjnam(potion, "catch"));
        useup(potion);
        explode(u.ux, u.uy, 11, &youmonst, 6, 6, 0, obj->otyp, 0, EXPL_FIERY);
        exercise(A_WIS, FALSE);
        return 1;
    }
    /* Adding oil to an empty magic lamp renders it into an oil lamp */
    if ((obj->otyp == MAGIC_LAMP) && obj->special_quality == 0)
    {
        obj->otyp = OIL_LAMP;
        obj->age = 0;
    }
    if (obj->age > 1000L)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s full.", Yname2(obj), otense(obj, "are"));
        potion->in_use = FALSE; /* didn't go poof */
    }
    else
    {
        play_sfx_sound(SFX_FILL_OIL_UP);
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "fill %s with oil.", yname(obj));
        check_unpaid(potion);        /* Yendorian Fuel Tax */
        /* burns more efficiently in a lamp than in a bottle;
           diluted potion provides less benefit but we don't attempt
           to track that the lamp now also has some non-oil in it */
        obj->age += (!potion->odiluted ? 4L : 3L) * potion->age / 2L;
        if (obj->age > 1500L)
            obj->age = 1500L;
        useup(potion);
        exercise(A_WIS, TRUE);
    }
    makeknown(POT_OIL);
    update_inventory();
    return 1;
}

/* *monp grants a wish and then leaves the game */
void
mongrantswish(monp)
struct monst **monp;
{
    struct monst *mon = *monp;
    int mx = mon->mx, my = mon->my, glyph = glyph_at(mx, my);

    /* remove the monster first in case wish proves to be fatal
       (blasted by artifact), to keep it out of resulting bones file */
    mongone(mon);
    *monp = 0; /* inform caller that monster is gone */
    /* hide that removal from player--map is visible during wish prompt */
    tmp_at(DISP_ALWAYS, glyph);
    tmp_at(mx, my);
    /* grant the wish */
    makewish(FALSE, FALSE);
    /* clean up */
    play_special_effect_at(SPECIAL_EFFECT_PUFF_OF_SMOKE, 0, mx, my, FALSE);
    play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
    special_effect_wait_until_action(0);
    tmp_at(DISP_END, 0);
    special_effect_wait_until_end(0);
}

void
djinni_from_bottle(obj)
struct obj *obj;
{
    if (!obj)
        return;

    struct monst *mtmp;
    int chance;

    if (!(mtmp = makemon(&mons[PM_DJINNI], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END))) {
        pline("It turns out to be empty.");
        return;
    }

    if (!Blind) {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "In a cloud of smoke, %s emerges!", a_monnam(mtmp));
        pline("%s speaks.", Monnam(mtmp));
    } else {
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "smell acrid fumes.");
        pline("%s speaks.", Something);
    }

    chance = rn2(5);
    if (obj->blessed)
        chance = (chance == 4) ? rnd(4) : 0;
    else if (obj->cursed)
        chance = (chance == 0) ? rn2(4) : 4;
    if (obj->speflags & SPEFLAGS_CERTAIN_WISH)
        chance = 0;
    /* 0,1,2,3,4:  b=80%,5,5,5,5; nc=20%,20,20,20,20; c=5%,5,5,5,80 */

    switch (chance) {
    case 0:
        obj->speflags |= SPEFLAGS_CERTAIN_WISH;
        wish_insurance_check(TRUE);
        convert_magic_lamp_to_oil_lamp(obj);
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_GRANT_ONE_WISH);
        verbalize_happy1("I am in your debt.  I will grant one wish!");
        /* give a wish and discard the monster (mtmp set to null) */
        mongrantswish(&mtmp);
        break;
    case 1:
        convert_magic_lamp_to_oil_lamp(obj);
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_THANK_YOU_FOR_FREEING);
        verbalize_talk1("Thank you for freeing me!");
        (void) tamedog(mtmp, (struct obj *) 0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);
        break;
    case 2:
        convert_magic_lamp_to_oil_lamp(obj);
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_YOU_FREED_ME);
        verbalize_talk1("You freed me!");
        mtmp->mpeaceful = TRUE;
        set_mhostility(mtmp);
        newsym(mtmp->mx, mtmp->my);
        break;
    case 3:
        convert_magic_lamp_to_oil_lamp(obj);
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_IT_IS_ABOUT_TIME);
        verbalize_talk1("It is about time!");
        play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
        play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
        if (canspotmon(mtmp))
            pline("%s vanishes.", Monnam(mtmp));
        special_effect_wait_until_action(0);
        mongone(mtmp);
        special_effect_wait_until_end(0);
        break;
    default:
        convert_magic_lamp_to_oil_lamp(obj);
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_YOU_DISTURBED);
        verbalize_angry1("You disturbed me, fool!");
        mtmp->mpeaceful = FALSE;
        set_mhostility(mtmp);
        newsym(mtmp->mx, mtmp->my);
        break;
    }
}

void
convert_magic_lamp_to_oil_lamp(obj)
struct obj* obj;
{
    if (!obj)
        return;

    if (obj->otyp == MAGIC_LAMP)
    {
        /* bones preparation:  perform the lamp transformation
       before releasing the djinni in case the latter turns out
       to be fatal (a hostile djinni has no chance to attack yet,
       but an indebted one who grants a wish might bestow an
       artifact which blasts the hero with lethal results) */
        obj->otyp = OIL_LAMP;
        obj->special_quality = 0; /* for safety */
        obj->speflags &= ~SPEFLAGS_CERTAIN_WISH;
        obj->age = rn1(500, 1000);
        if (obj->lamplit)
            begin_burn(obj, TRUE);
    }
}

/* clone a gremlin or mold (2nd arg non-null implies heat as the trigger);
   hit points are cut in half (odd HP stays with original) */
struct monst *
split_mon(mon, mtmp)
struct monst *mon,  /* monster being split */
             *mtmp; /* optional attacker whose heat triggered it */
{
    struct monst *mtmp2 = 0;
    char reason[BUFSZ];

    reason[0] = '\0';
    if (mtmp)
        Sprintf(reason, " from %s heat",
                (mtmp == &youmonst) ? the_your[1]
                                    : (const char *) s_suffix(mon_nam(mtmp)));

    if (mon)
    {
        if (mon == &youmonst)
        {
            mtmp2 = cloneu();
            if (mtmp2)
            {
                mtmp2->mbasehpmax = u.basemhmax / 2;
                u.basemhmax -= mtmp2->mbasehpmax;
                mtmp2->mbasehpdrain = u.basemhdrain / 2;
                u.basemhdrain -= mtmp2->mbasehpdrain;
                updatemaxhp();
                update_mon_maxhp(mtmp2);
                context.botl = context.botlx = 1;
                You("multiply%s!", reason);
            }
        }
        else
        {
            mtmp2 = clone_mon(mon, 0, 0, TRUE);
            if (mtmp2)
            {
                /* Gremlins are half strength */
                mtmp2->mbasehpmax = mon->mbasehpmax / 2;
                mon->mbasehpmax -= mtmp2->mbasehpmax;
                mtmp2->mbasehpdrain = mon->mbasehpdrain / 2;
                mon->mbasehpdrain -= mtmp2->mbasehpdrain;
                update_mon_maxhp(mon);
                update_mon_maxhp(mtmp2);
                if (canspotmon(mon))
                    pline("%s multiplies%s!", Monnam(mon), reason);
            }
        }
    }
    return mtmp2;
}

void
reset_potion(VOID_ARGS)
{
    nothing = unkn = 0;
}

/*potion.c*/

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    timeout.c    $NHDT-Date: 1545182148 2018/12/19 01:15:48 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.89 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h" /* for checking save modes */

STATIC_DCL void NDECL(stoned_dialogue);
STATIC_DCL void NDECL(vomiting_dialogue);
STATIC_DCL void NDECL(choke_dialogue);
STATIC_DCL void NDECL(levitation_dialogue);
STATIC_DCL void NDECL(slime_dialogue);
STATIC_DCL void FDECL(slimed_to_death, (struct kinfo *));
STATIC_DCL void NDECL(slip_or_trip);
STATIC_DCL void NDECL(laugh_uncontrollably);
STATIC_DCL void NDECL(get_odd_idea);
STATIC_DCL void FDECL(see_lamp_flicker, (struct obj *, const char *));
STATIC_DCL void FDECL(lantern_message, (struct obj *));
STATIC_DCL void FDECL(cleanup_burn, (ANY_P *, long));
STATIC_DCL void FDECL(cleanup_sound, (ANY_P*, long));
STATIC_DCL void NDECL(sick_dialogue);
STATIC_DCL void NDECL(food_poisoned_dialogue);
STATIC_DCL void NDECL(mummy_rot_dialogue);

STATIC_VAR boolean alternate_sick_text = FALSE;

STATIC_VAR NEARDATA const char* const sick_texts[] = {
    "You are starting to feel badly feverish.",        /* 8 */
    "Your fever is rising very high.",    /* 7 */
    "You are feeling extremely feverish.",          /* 6 */
    "Your condition is starting to deteriorate badly.",       /* 5 */
    "You feel your condition is becoming critical.",      /* 4 */
    "You are feeling really deathly sick.", /* 3 */
    "You feel you are at death's door.",        /* 2 */
    "The sickness is fatal."            /* 1 */
};

STATIC_OVL void
sick_dialogue()
{
    if (!is_living(youmonst.data))
        return;

    register long i = (Sick & TIMEOUT);

    if (i > 0L && i <= SIZE(sick_texts)) {
        char buf[BUFSZ];
        Strcpy(buf, sick_texts[SIZE(sick_texts) - i]);
        pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, buf);
    }
    else if (has_head(youmonst.data) && !rn2(3))
    {
        if(!rn2(2))
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "cough%s.", (alternate_sick_text ? " roughly" : ""));
        else
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "have a %sbout of coughing.", (alternate_sick_text ? "severe " : "terrible "));

        alternate_sick_text = !alternate_sick_text;
        if (multi > 0)
            nomul(0);
    }

    boolean updategui = FALSE;
    if (i <= 8)
    {
        set_itimeout(&HConfusion, max((HConfusion & TIMEOUT), i + 1));
        updategui = TRUE;
    }

    if (i <= 4)
    {
        set_itimeout(&HStun, max((HStun & TIMEOUT), i + 1));
        updategui = TRUE;
    }

    if (updategui)
    {
        refresh_u_tile_gui_info(TRUE);
        context.botl = context.botlx = 1;
    }

}


STATIC_VAR NEARDATA const char* const food_poisoned_texts[] = {
    "You are feeling very feverish.",        /* 8 */
    "Your stomach is hurting terribly.",    /* 7 */
    "You are feeling extremely feverish.",          /* 6 */
    "You are experiencing massive stomach pains.",       /* 5 */
    "You feel your condition is becoming critical.",      /* 4 */
    "You are feeling really deathly sick.", /* 3 */
    "You feel you are at death's door.",        /* 2 */
    "The food poisoning is fatal."            /* 1 */
};

STATIC_OVL void
food_poisoned_dialogue()
{
    if (!is_living(youmonst.data))
        return;

    register long i = (FoodPoisoned & TIMEOUT);

    if (i > 0L && i <= SIZE(food_poisoned_texts)) {
        char buf[BUFSZ];

        Strcpy(buf, food_poisoned_texts[SIZE(food_poisoned_texts) - i]);
        pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, buf);
    }
    else if (!rn2(3))
    {
        if (!rn2(2))
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "stomach hurts%s.", (alternate_sick_text ? " badly" : ""));
        else
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "experience a %sbout of stomach cramps.", (alternate_sick_text ? "severe " : "terrible "));

        alternate_sick_text = !alternate_sick_text;

        if (multi > 0)
            nomul(0);
    }

    boolean updategui = FALSE;
    if (i <= 8)
    {
        set_itimeout(&HConfusion, max((HConfusion & TIMEOUT), i + 1));
        updategui = TRUE;
    }

    if (i <= 4)
    {
        set_itimeout(&HStun, max((HStun & TIMEOUT), i + 1));
        updategui = TRUE;
    }

    if (updategui)
    {
        refresh_u_tile_gui_info(TRUE);
        context.botl = context.botlx = 1;
    }
}


STATIC_OVL void
mummy_rot_dialogue()
{
    if (!is_living(youmonst.data) || Sick_resistance)
        return;

    int i = context.mummyrot_advancement;

    if (i < 20)
    {
        switch (i % 10)
        {
        case 0:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s patches of skin are appearing on your %s!", i >= 10 ? "More black" : "Black", body_part(FACE));
            break;
        case 1:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s is turning black!", i >= 10 ? "Another one" : "One", makeplural(body_part(FINGERTIP)));
            break;
        case 2:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s is turning black!", i >= 10 ? "Another one" : "One", makeplural(body_part(TOE)));
            break;
        case 3:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s patches of your skin are turning black!", i >= 10 ? "Huge" : "Large");
            break;
        case 4:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s is turning black!", i >= 10 ? "Another one" : "One", makeplural(body_part(FINGER)));
            break;
        case 5:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s falls off!", i >= 10 ? "Large patches of your hair" : "One of your teeth");
            break;
        case 6:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s are turning %sblack!", makeplural(body_part(HAND)), i >= 10 ? "pitch " : "");
            break;
        case 7:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s feels loose!", i >= 10 ? "Another one" : "One", makeplural(body_part(FINGER)));
            break;
        case 8:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s are turning %sblack!", makeplural(body_part(FOOT)), i >= 10 ? "pitch " : "");
            break;
        case 9:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s feels loose!", i >= 10 ? "Another one" : "One", makeplural(body_part(TOE)));
            break;
        default:
            break;
        }
    }
    else
    {
        int sel_index = i - 20;
        if (sel_index >= 4 * 6)
            sel_index = !rn2(2) ? 0 : 3;

        switch (sel_index % 6)
        {
        case 0:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s falls off!", sel_index >= 30 ? "Further parts" : "A part", body_part(FACE));
            break;
        case 1:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s falls off!", sel_index >= 30 ? "Another one" : "One", makeplural(body_part(FINGER)));
            break;
        case 2:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s falls off!", sel_index >= 30 ? "Another one" : "One", makeplural(body_part(TOE)));
            break;
        case 3:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s of your %s falls off!", sel_index >= 30 ? "Further parts" : "A part", body_part(NOSE));
            break;
        case 4:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Further patches of your hair falls off!");
            break;
        case 5:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Another one of your teeth falls off!");
            break;
        default:
            break;
        }
    }

    if(context.mummyrot_advancement < 50)
        context.mummyrot_advancement++;

    nomul(0);
}


/* He is being petrified - dialogue by inmet!tower */
STATIC_VAR NEARDATA const char *const stoned_texts[] = {
    "You are slowing down.",            /* 5 */
    "Your limbs are stiffening.",       /* 4 */
    "Your limbs have turned to stone.", /* 3 */
    "You have turned to stone.",        /* 2 */
    "You are a statue."                 /* 1 */
};

STATIC_OVL void
stoned_dialogue()
{
    register long i = (Stoned & TIMEOUT);

    if (i > 0L && i <= SIZE(stoned_texts)) {
        char buf[BUFSZ];

        Strcpy(buf, stoned_texts[SIZE(stoned_texts) - i]);
        if (nolimbs(youmonst.data) && strstri(buf, "limbs"))
            (void) strsubst(buf, "limbs", "extremities");
        pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, buf);
    }
    switch ((int) i) {
    case 5: /* slowing down */
        HFast = 0L;
        if (multi > 0)
            nomul(0);
        break;
    case 4: /* limbs stiffening */
        /* just one move left to save oneself so quit fiddling around;
           don't stop attempt to eat tin--might be lizard or acidic */
        if (!Popeye(STONED))
            stop_occupation();
        if (multi > 0)
            nomul(0);
        break;
    case 3: /* limbs turned to stone */
        stop_occupation();
        nomul(-3); /* can't move anymore */
        multi_reason = "getting stoned";
        nomovemsg = You_can_move_again; /* not unconscious */
        /* "your limbs have turned to stone" so terminate wounded legs */
        if (Wounded_legs && !u.usteed)
            heal_legs(2);
        break;
    case 2: /* turned to stone */
        if ((HDeaf & TIMEOUT) > 0L && (HDeaf & TIMEOUT) < 5L)
        {
            set_itimeout(&HDeaf, 5L); /* avoid Hear_again at tail end */
            play_environment_ambient_sounds();
        }
        /* if also vomiting or turning into slime, stop those (no messages) */
        if (Vomiting)
            make_vomiting(0L, FALSE);
        if (Slimed)
            make_slimed(0L, (char *) 0, 0, (char*)0, 0);
        break;
    default:
        break;
    }
    exercise(A_DEX, FALSE);
}

/* hero is getting sicker and sicker prior to vomiting */
STATIC_VAR NEARDATA const char *const vomiting_texts[] = {
    "are feeling mildly nauseated.", /* 14 */
    "feel slightly confused.",       /* 11 */
    "can't seem to think straight.", /* 8 */
    "feel incredibly sick.",         /* 5 */
    "are about to vomit."            /* 2 */
};

STATIC_OVL void
vomiting_dialogue()
{
    const char *txt = 0;
    long v = (Vomiting & TIMEOUT);

    /* note: nhtimeout() hasn't decremented timed properties for the
       current turn yet, so we use Vomiting-1 here */
    switch ((int) (v - 1L)) {
    case 14:
        txt = vomiting_texts[0];
        break;
    case 11:
        txt = vomiting_texts[1];
        break;
    case 6:
        make_stunned(itimeout_incr(HStun, d(2, 4)), FALSE);
        if (!Popeye(VOMITING))
            stop_occupation();
    /*FALLTHRU*/
    case 9:
        make_confused(itimeout_incr(HConfusion, d(2, 4)), FALSE);
        if (multi > 0)
            nomul(0);
        break;
    case 8:
        txt = vomiting_texts[2];
        break;
    case 5:
        txt = vomiting_texts[3];
        break;
    case 2:
        txt = vomiting_texts[4];
        if (cantvomit(youmonst.data))
            txt = "gag uncontrollably.";
        else if (Hallucination)
            /* "hurl" is short for "hurl chunks" which is slang for
               relatively violent vomiting... */
            txt = "are about to hurl!";
        break;
    case 0:
        stop_occupation();
        if (!cantvomit(youmonst.data)) {
            morehungry(20);
            /* case 2 used to be "You suddenly vomit!" but it wasn't sudden
               since you've just been through the earlier messages of the
               countdown, and it was still possible to move around between
               that message and "You can move again." (from vomit()'s
               nomul(-2)) with no intervening message; give one here to
               have more specific point at which hero became unable to move
               [vomit() issues its own message for the cantvomit() case
               and for the FAINTING-or-worse case where stomach is empty] */
            if (u.uhs < FAINTING)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s!", !Hallucination ? "vomit" : "hurl chunks");
        }
        vomit();
        break;
    default:
        break;
    }
    if (txt)
        You_ex1(ATR_NONE, CLR_MSG_NEGATIVE, txt);
    exercise(A_CON, FALSE);
}

STATIC_VAR NEARDATA const char *const choke_texts[] = {
    "You find it hard to breathe.",
    "You're gasping for air.",
    "You can no longer breathe.",
    "You're turning %s.",
    "You suffocate."
};

STATIC_VAR NEARDATA const char *const choke_texts2[] = {
    "Your %s is becoming constricted.",
    "Your blood is having trouble reaching your brain.",
    "The pressure on your %s increases.",
    "Your consciousness is fading.",
    "You suffocate."
};

STATIC_OVL void
choke_dialogue()
{
    register long i = (Strangled & TIMEOUT);

    if (i > 0 && i <= SIZE(choke_texts)) {
        if (Breathless || !rn2(50))
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, choke_texts2[SIZE(choke_texts2) - i], body_part(NECK));
        else {
            const char *str = choke_texts[SIZE(choke_texts) - i];

            if (index(str, '%'))
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, str, hcolor(NH_BLUE));
            else
                pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, str);
        }
    }
    exercise(A_STR, FALSE);
}

STATIC_VAR NEARDATA const char *const levi_texts[] = {
    "You float slightly lower.",
    "You wobble unsteadily %s the %s."
};

STATIC_OVL void
levitation_dialogue()
{
    /* -1 because the last message comes via float_down() */
    long i = (((HLevitation & TIMEOUT) - 1L) / 2L);

    if (ELevitation)
        return;

    if (!ACCESSIBLE(levl[u.ux][u.uy].typ)
        && !is_pool_or_lava(u.ux,u.uy))
        return;

    if (((HLevitation & TIMEOUT) % 2L) && i > 0L && i <= SIZE(levi_texts)) {
        const char *s = levi_texts[SIZE(levi_texts) - i];

        if (index(s, '%')) {
            boolean danger = (is_pool_or_lava(u.ux, u.uy)
                              && !Is_waterlevel(&u.uz));

            pline(s, danger ? "over" : "in",
                  danger ? surface(u.ux, u.uy) : "air");
        } else
            pline1(s);
    }
}

STATIC_VAR NEARDATA const char *const slime_texts[] = {
    "You are turning a little %s.",   /* 5 */
    "Your limbs are getting oozy.",   /* 4 */
    "Your skin begins to peel away.", /* 3 */
    "You are turning into %s.",       /* 2 */
    "You have become %s."             /* 1 */
};

STATIC_OVL void
slime_dialogue()
{
    register long i = (Slimed & TIMEOUT) / 2L;

    if (i == 1L) {
        /* display as green slime during "You have become green slime."
           but don't worry about not being able to see self; if already
           mimicking something else at the time, implicitly be revealed */
        youmonst.m_ap_type = M_AP_MONSTER;
        youmonst.mappearance = PM_GREEN_SLIME;
    }
    if (((Slimed & TIMEOUT) % 2L) && i >= 0L && i < SIZE(slime_texts)) {
        char buf[BUFSZ];

        Strcpy(buf, slime_texts[SIZE(slime_texts) - i - 1L]);
        if (nolimbs(youmonst.data) && strstri(buf, "limbs"))
            (void) strsubst(buf, "limbs", "extremities");

        if (index(buf, '%')) {
            if (i == 4L) {  /* "you are turning green" */
                if (!Blind) /* [what if you're already green?] */
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, buf, hcolor(NH_GREEN));
            } else
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, buf,
                      an(Hallucination ? rndmonnam(NULL) : "green slime"));
        } else
            pline_ex1(ATR_NONE, CLR_MSG_NEGATIVE, buf);
    }

    switch (i) {
    case 3L:  /* limbs becoming oozy */
        HFast = 0L; /* lose intrinsic speed */
        if (!Popeye(SLIMED))
            stop_occupation();
        if (multi > 0)
            nomul(0);
        break;
    case 2L: /* skin begins to peel */
        if ((HDeaf & TIMEOUT) > 0L && (HDeaf & TIMEOUT) < 5L)
        {
            set_itimeout(&HDeaf, 5L); /* avoid Hear_again at tail end */
            play_environment_ambient_sounds();
        }
        break;
    case 1L: /* turning into slime */
        /* if also turning to stone, stop doing that (no message) */
        if (Stoned)
            make_stoned(0L, (char *) 0, KILLED_BY_AN, (char *) 0, 0);
        break;
    }
    exercise(A_DEX, FALSE);
}

void
burn_away_slime()
{
    if (Slimed) {
        make_slimed(0L, "The slime that covers you is burned away!", 0, (char*)0, 0);
    }
}

/* countdown timer for turning into green slime has run out; kill our hero */
STATIC_OVL void
slimed_to_death(kptr)
struct kinfo *kptr;
{
    uchar save_mvflags;

    /* redundant: polymon() cures sliming when polying into green slime */
    if (Upolyd && youmonst.data == &mons[PM_GREEN_SLIME]) {
        dealloc_killer(kptr);
        return;
    }
    /* more sure killer reason is set up */
    if (kptr && kptr->name[0]) {
        killer.format = kptr->format;
        Strcpy(killer.name, kptr->name);
        killer.hint_idx = kptr->hint_idx;
    } else {
        killer.format = NO_KILLER_PREFIX;
        Strcpy(killer.name, "turned into green slime");
        killer.hint_idx = HINT_KILLED_SLIMED;
    }
    dealloc_killer(kptr);

    /*
     * Polymorph into a green slime, which might destroy some worn armor
     * (potentially affecting bones) and dismount from steed.
     * Can't be Unchanging; wouldn't have turned into slime if we were.
     * Despite lack of Unchanging, neither done() nor savelife() calls
     * rehumanize() if hero dies while polymorphed.
     * polymon() undoes the slime countdown's mimick-green-slime hack
     * but does not perform polyself()'s light source bookkeeping.
     * No longer need to manually increment uconduct.polyselfs to reflect
     * [formerly implicit] change of form; polymon() takes care of that.
     * Temporarily ungenocide if necessary.
     */
    if (emitted_light_range(youmonst.data))
        del_light_source(LS_MONSTER, monst_to_any(&youmonst));
    if (mon_ambient_sound(youmonst.data))
        del_sound_source(SOUNDSOURCE_MONSTER, monst_to_any(&youmonst));
    save_mvflags = mvitals[PM_GREEN_SLIME].mvflags;
    mvitals[PM_GREEN_SLIME].mvflags = save_mvflags & ~MV_GENOCIDED;
    (void) polymon(PM_GREEN_SLIME);
    mvitals[PM_GREEN_SLIME].mvflags = save_mvflags;
    done(TURNED_SLIME);

    /* life-saved; even so, hero still has turned into green slime;
       player may have genocided green slimes after being infected */
    if ((mvitals[PM_GREEN_SLIME].mvflags & MV_GENOCIDED) != 0) {
        killer.format = KILLED_BY;
        Strcpy(killer.name, "slimicide");
        /* immediately follows "OK, so you don't die." */
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Yes, you do.  Green slime has been genocided...");
        done(GENOCIDED);
        /* could be life-saved again (only in explore or wizard mode)
           but green slimes are gone; just stay in current form */
    }
    return;
}

/* Intrinsic Passes_walls is temporary when your god is trying to fix
   all troubles and then TROUBLE_STUCK_IN_WALL calls _telesafe_teleds() but
   it can't find anywhere to place you.  If that happens you get a small
   value for (HPasses_walls & TIMEOUT) to move somewhere yourself.
   Message given is "you feel much slimmer" as a joke hint that you can
   move between things which are closely packed--like the substance of
   solid rock! */
STATIC_VAR NEARDATA const char *const phaze_texts[] = {
    "You start to feel bloated.",
    "You are feeling rather flabby.",
};

STATIC_OVL void
phaze_dialogue()
{
    long i = ((HPasses_walls & TIMEOUT) / 2L);

    if (EPasses_walls || (HPasses_walls & ~TIMEOUT))
        return;

    if (((HPasses_walls & TIMEOUT) % 2L) && i > 0L && i <= SIZE(phaze_texts))
        pline1(phaze_texts[SIZE(phaze_texts) - i]);
}

void
nh_timeout()
{
    register struct prop *upp;
    struct kinfo *kptr;
    int sleeptime;
    int m_idx;
    
    /* Conditions */
    boolean was_flying = !!Flying;
    boolean was_levitating = !!Levitation;
    boolean had_hallucination = !!Hallucination;
    boolean was_blind = !!Blind;
    boolean was_deaf = !!Deaf;
    boolean was_stunned = !!Stunned;
    boolean was_confused = !!Confusion;
    boolean was_terminally_ill = !!Sick;
    boolean was_food_poisoned = !!FoodPoisoned;
    boolean was_stoned = !!Stoned;
    boolean was_strangled = (Strangled && !Breathless);
    boolean was_suffocating = (Airless_environment && !Survives_without_air);
    boolean was_vomiting = !!Vomiting;
    boolean was_slimed = !!Slimed;
    boolean had_mummyrot = !!MummyRot;
    boolean was_silenced = !!Silenced;
    boolean was_cancelled = !!Cancelled;
    boolean was_paralyzed = !!Paralyzed;
    boolean was_sleeping = !!Sleeping;
    boolean was_fearful = !!Fearful;
    int old_weight_cap = near_capacity();
    /* Hunger status cannot change here */
    struct monst* old_ustuck = u.ustuck;
    struct monst* old_usteed = u.usteed;
    int old_ulycn = u.ulycn;
    int old_move = get_u_move_speed(TRUE);
    int oldstr = ACURR(A_STR);
    int olddex = ACURR(A_DEX);
    int oldcon = ACURR(A_CON);
    int oldint = ACURR(A_INT);
    int oldwis = ACURR(A_WIS);
    int oldcha = ACURR(A_CHA);
    
    
    int baseluck = (flags.moonphase == FULL_MOON) ? 1 : 0;

    if (flags.friday13)
        baseluck -= 1;

    if (u.uluck != baseluck
        && moves % ((u.uhave.amulet || u.ugangr) ? 300 : 600) == 0) {
        /* Cursed luckstones stop bad luck from timing out; blessed luckstones
         * stop good luck from timing out; normal luckstones stop both;
         * neither is stopped if you don't have a luckstone.
         * Luck is based at 0 usually, +1 if a full moon and -1 on Friday 13th
         */
        //register int time_luck = stone_luck(FALSE);
        //boolean nostone = !carrying(LUCKSTONE) && !stone_luck(TRUE);

        if (u.uluck > baseluck  && !u.luck_does_not_timeout)//(u.uluck > baseluck && (nostone || time_luck < 0))
            u.uluck--;
        else if (u.uluck < baseluck && !u.unluck_does_not_timeout) //(u.uluck < baseluck && (nostone || time_luck > 0))
            u.uluck++;
    }
    if (u.uinvulnerable)
        return; /* things past this point could kill you */
    if (Stoned)
        stoned_dialogue();
    if (Slimed)
        slime_dialogue();
    if (Vomiting)
        vomiting_dialogue();
    if (Strangled && !Breathless)
        choke_dialogue();
    if (Sick)
        sick_dialogue();
    if (FoodPoisoned)
        food_poisoned_dialogue();
    if (HLevitation & TIMEOUT)
        levitation_dialogue();
    if (HPasses_walls & TIMEOUT)
        phaze_dialogue();
    if (u.mtimedone && !--u.mtimedone) {
        if (Unchanging)
            u.mtimedone = rnd(100 * youmonst.data->mlevel + 1);
        else if (is_were(youmonst.data))
            you_unwere(FALSE); /* if polycontrl, asks whether to rehumanize */
        else
            rehumanize();
    }
    if (u.ucreamed)
        u.ucreamed--;

    if (u.ugallop) {
        if (--u.ugallop == 0L && u.usteed)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stops galloping.", Monnam(u.usteed));
    }

    //Reduce spell cooldown timers
    int sp_no;
    for (sp_no = 0; sp_no < MAXSPELL; ++sp_no)
        if (spl_book[sp_no].sp_id == NO_SPELL)
            break;
        else if (spl_book[sp_no].sp_cooldownleft > 0)
            spl_book[sp_no].sp_cooldownleft--;


    //Reduce item cooldown timers
    struct monst* mon, * mmtmp[3];
    int i;

    /* first go through various obj lists directly */
    reduce_item_cooldown(invent);
    reduce_item_cooldown(fobj);
    reduce_item_cooldown(level.buriedobjlist);
    reduce_item_cooldown(migrating_objs);

    /* second go through monster inventories */
    mmtmp[0] = fmon;
    mmtmp[1] = migrating_mons;
    mmtmp[2] = mydogs; /* for use during level changes */
    for (i = 0; i < 3; i++)
        for (mon = mmtmp[i]; mon; mon = mon->nmon)
            reduce_item_cooldown(mon->minvent);

    for (upp = u.uprops; upp < u.uprops + SIZE(u.uprops); upp++)
    {
        int propnum = (int)(upp - u.uprops);
        int extratime = 0;
        if ((upp->intrinsic & TIMEOUT) && !(--upp->intrinsic & TIMEOUT))
        {
            kptr = find_delayed_killer(propnum);
            switch (propnum) {
            case STONED:
                if (kptr && kptr->name[0]) {
                    killer.format = kptr->format;
                    Strcpy(killer.name, kptr->name);
                    killer.hint_idx = kptr->hint_idx;
                }
                else {
                    killer.format = NO_KILLER_PREFIX;
                    Strcpy(killer.name, "killed by petrification");
                    killer.hint_idx = HINT_KILLED_PETRIFICATION;
                }
                if (kptr)
                    dealloc_killer(kptr);
                /* (unlike sliming, you aren't changing form here) */
                play_sfx_sound(SFX_PETRIFY);
                done(STONING);
                break;
            case SLIMED:
                slimed_to_death(kptr); /* done(TURNED_SLIME) */
                break;
            case VOMITING:
                make_vomiting(0L, TRUE);
                break;
            case SICK:
            case FOOD_POISONED:
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die from your illness.");

                if (kptr && kptr->name[0])
                {
                    killer.format = kptr->format;
                    Strcpy(killer.name, kptr->name);
                    killer.hint_idx = kptr->hint_idx;
                }
                else
                {
                    killer.format = KILLED_BY_AN;
                    killer.name[0] = 0; /* take the default */
                    killer.hint_idx = 0;
                }
    
                if(kptr)
                    dealloc_killer(kptr);

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
                done(ILLNESS);
                break;
            case MUMMY_ROT:
                if (MummyRot)
                    mummy_rot_dialogue();

                if (ABASE(A_CON) <= ATTRMIN(A_CON) + 2)
                    You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "desiccated.");
                else
                    You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "gangrenous.");

                if (ABASE(A_CHA) > ATTRMIN(A_CHA))
                {
                    adjattrib(A_CHA, -1, TRUE);
                }
                if (ABASE(A_CON) > ATTRMIN(A_CON))
                {
                    adjattrib(A_CON, -1, TRUE);
                }
                else
                {
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die from your illness.");
                
                    if (kptr && kptr->name[0]) 
                    {
                        killer.format = kptr->format;
                        Strcpy(killer.name, kptr->name);
                        killer.hint_idx = kptr->hint_idx;
                    }
                    else 
                    {
                        killer.format = KILLED_BY_AN;
                        killer.name[0] = 0; /* take the default */
                        killer.hint_idx = 0;
                    }

                    if (kptr)
                        dealloc_killer(kptr);

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
                    /* Life saved */
                    make_mummy_rotted(0L, (char*)0, FALSE, 0);
                }

                break;
            case SLEEPY:
                if (unconscious() || Sleep_resistance) {
                    //incr_itimeout(&HSleepy, rnd(100));
                }
                else if (Sleepy) {
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "fall asleep.");
                    sleeptime = rnd(20);
                    fall_asleep(-sleeptime, TRUE);
                    extratime = sleeptime;
                    //incr_itimeout(&HSleepy, sleeptime + rnd(100));
                }
                break;
            case STRANGLED:
                killer.format = KILLED_BY_AN;
                if (Breathless)
                {
                    // Nothing
                }
                else if (uamul && uamul->otyp == AMULET_OF_STRANGULATION)
                {
                    Strcpy(killer.name, "amulet of strangulation");
                    killer.hint_idx = HINT_KILLED_ITEM_STRANGULATION;
                    done(STRANGULATION);
                }
                else if (u.ustuck && (is_constrictor(u.ustuck->data) || hug_throttles(u.ustuck->data)))
                {
                    killer.hint_idx = HINT_KILLED_MONSTER_STRANGULATION;
                    done_in_by(u.ustuck, STRANGULATION);
                }
                /* must be declining to die in explore|wizard mode;
                   treat like being cured of strangulation by prayer */
                if (uamul && uamul->otyp == AMULET_OF_STRANGULATION) {
                    play_sfx_sound(SFX_ITEM_VANISHES);
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "amulet vanishes!");
                    useup(uamul);
                }
                break;
            case AIRLESS_ENVIRONMENT:
            {
                boolean drowned_by_monster = u.ustuck && is_pool(u.ustuck->mx, u.ustuck->my) && !Swimming && !Amphibious;
                if (Survives_without_air)
                {
                    //Nothing
                }
                else
                {
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, Underwater || drowned_by_monster ? "drown." : "suffocate.");
                    if (drowned_by_monster)
                    {
                        killer.hint_idx = HINT_KILLED_DROWNED_BY_MONSTER;
                        done_in_by(u.ustuck, DROWNED);
                    }
                    else
                    {
                        killer.format = KILLED_BY;
                        boolean isdrowning = (Underwater || is_pool(u.ux, u.uy));

                        if (isdrowning)
                        {
                            killer.hint_idx = HINT_KILLED_DROWNED;
                            killer.format = KILLED_BY_AN;
                            Sprintf(killer.name, "%s", is_pool(u.ux, u.uy) ? (levl[u.ux][u.uy].typ == MOAT ? "moat" : "pool of water") : "body of water");
                        }
                        else
                        {
                            killer.hint_idx = u.uburied ? HINT_KILLED_SUFFOCATION_BY_BEING_BURIED : HINT_KILLED_SUFFOCATION;
                            Sprintf(killer.name, "%s", u.uburied ? "being buried alive" : "");
                        }

                        done(isdrowning ? DROWNING : SUFFOCATION);
                    }
                }
                break;
            }
            case FUMBLING:
                /* call this only when a move took place.  */
                /* otherwise handle fumbling msgs locally. */
                if (u.umoved && !Levitation) {
                    slip_or_trip();
                    nomul(-2);
                    multi_reason = "fumbling";
                    nomovemsg = "";
                    /* The more you are carrying the more likely you
                     * are to make noise when you fumble.  Adjustments
                     * to this number must be thoroughly play tested.
                     */
                    if ((inv_weight() > -500)) {
                        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "make a lot of noise!");
                        wake_nearby();
                    }
                }
                /* from outside means slippery ice; don't reset
                   counter if that's the only fumble reason */
                HFumbling &= ~FROM_ACQUIRED;
                //if (Fumbling)
                //    incr_itimeout(&HFumbling, rnd(20));
                break;
            case LAUGHING:
                laugh_uncontrollably();
                nomul(-1);
                multi_reason = "laughing";
                wake_nearby();
                //if (Laughing)
                //    incr_itimeout(&HLaughing, rnd(20));
                break;
            case ODD_IDEAS:
                get_odd_idea();
                nomul(0);
                multi_reason = "getting visionary ideas";
                //if (OddIdeas)
                //    incr_itimeout(&HOddIdeas, 150 + rnd(100));
                break;
            case CONFUSION:
                /* So make_confused works properly */
                set_itimeout(&HConfusion, 1L);
                make_confused(0L, TRUE);
                if (!Confusion)
                    stop_occupation();
                break;
            case STUNNED:
                set_itimeout(&HStun, 1L);
                make_stunned(0L, TRUE);
                if (!Stunned)
                    stop_occupation();
                break;
            case BLINDED:
                set_itimeout(&Blinded, 1L);
                make_blinded(0L, TRUE);
                if (!Blind)
                    stop_occupation();
                break;
            case DEAF:
                set_itimeout(&HDeaf, 1L);
                make_deaf(0L, TRUE);
                context.botl = context.botlx = TRUE;
                if (!Deaf)
                    stop_occupation();
                break;
            case WOUNDED_LEGS:
                heal_legs(0);
                stop_occupation();
                break;
            case HALLUC:
                set_itimeout(&HHallucination, 1L);
                (void)make_hallucinated(0L, TRUE, 0L);
                if (!Hallucination)
                    stop_occupation();
                break;
            default:
                property_expiry_message(propnum, was_flying);
            }
            newsym(u.ux, u.uy);
        }
        else if ((upp->intrinsic & TIMEOUT) && ((upp->intrinsic & TIMEOUT) == 3) && !(upp->intrinsic & ~TIMEOUT) && !(upp->extrinsic))
        {
            /* Early warning */
            switch (propnum) {
            case FAST:
                if (!Lightning_fast && !Super_fast && !Ultra_fast && !Very_fast)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less quick than before.");
                break;
            case VERY_FAST:
                if (!Lightning_fast && !Super_fast && !Ultra_fast)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel%s less quick than before.",
                    Fast ? " a bit" : "");
                break;
            case ULTRA_FAST:
                if (!Lightning_fast && !Super_fast)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel%s less quick than before.",
                        Very_fast || Fast ? " a bit" : "");
                break;
            case SUPER_FAST:
                if (!Lightning_fast)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel%s less quick than before.",
                        Ultra_fast || Very_fast || Fast ? " a bit" : "");
                break;
            case LIGHTNING_FAST:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel%s less quick than before.",
                    Super_fast || Ultra_fast || Very_fast || Fast ? " a bit" : "");
                break;
            case SLOWED:
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to speed up.");
                break;
            case HEROISM:
                if (!Super_heroism && !Heroism)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less heroic than before.");
                break;
            case SUPER_HEROISM:
                if (!Super_heroism)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel%s less heroic than before.",
                        Heroism ? " a bit" : "");
                break;
            case RAPID_REGENERATION:
                if (!Divine_regeneration && !Rapidest_regeneration && !Rapider_regeneration)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to heal %sless rapidly than before.",
                        Regeneration ? "" : "much ");
                break;
            case RAPIDER_REGENERATION:
                if (!Divine_regeneration && !Rapidest_regeneration)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to heal %sless rapidly than before.",
                        Rapid_regeneration || Regeneration ? "" : "much ");
                break;
            case RAPIDEST_REGENERATION:
                if (!Divine_regeneration)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to heal %sless rapidly than before.",
                    Rapider_regeneration || Rapid_regeneration || Regeneration ? "" : "much ");
                break;
            case DIVINE_REGENERATION:
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to heal %sless rapidly than before.",
                    Rapidest_regeneration || Rapider_regeneration || Rapid_regeneration || Regeneration ? "" : "much ");
                break;
            case RAPID_ENERGY_REGENERATION:
                if (!Rapidest_energy_regeneration && !Rapider_energy_regeneration)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to gain mana %sless rapidly than before.",
                        Energy_regeneration ? "" : "much ");
                break;
            case RAPIDER_ENERGY_REGENERATION:
                if (!Rapidest_energy_regeneration)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to gain mana %sless rapidly than before.",
                        Rapid_energy_regeneration || Energy_regeneration ? "" : "much ");
                break;
            case RAPIDEST_ENERGY_REGENERATION:
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to gain mana %sless rapidly than before.",
                    Rapider_energy_regeneration || Rapid_energy_regeneration || Energy_regeneration ? "" : "much ");
                break;
            case MELEE_LIFE_LEECH:
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are starting to lose your ability to leech life in melee.");
                break;
            case CRAZED:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel more sane than before.");
                break;
            case SILENCED:
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "your is starting to return.");
                break;
            case INVISIBILITY:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel more visible.");
                break;
            case SEE_INVISIBLE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "vision of invisible monsters is becoming less clear.");
                break;
            case LEVITATION:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less buoyant.");
                break;
            case FLYING:
                /* timed Flying is via #wizintrinsic only */
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less aerial.");
                break;
            case PASSES_WALLS:
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You're starting to get back to your %s self again.",
                    !Upolyd ? "normal" : "unusual");
                break;
            case REFLECTING:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel less reflecting than before.");
                break;
            case FIRE_IMMUNITY:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to burning than before.");
                break;
            case COLD_IMMUNITY:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to frostbites than before.");
                break;
            case SHOCK_IMMUNITY:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to electricity than before.");
                break;
            case IMPROVED_FIRE_RESISTANCE:
                if (!Fire_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to burning than before.");
                break;
            case IMPROVED_COLD_RESISTANCE:
                if (!Cold_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to frostbites than before.");
                break;
            case IMPROVED_SHOCK_RESISTANCE:
                if (!Shock_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to electricity than before.");
                break;
            case FIRE_RESISTANCE:
                if(!Improved_fire_resistance && !Fire_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to burning than before.");
                break;
            case COLD_RESISTANCE:
                if (!Improved_cold_resistance && !Cold_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to frostbites than before.");
                break;
            case SHOCK_RESISTANCE:
                if (!Improved_shock_resistance && !Shock_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to electricity than before.");
                break;
            case DISINTEGRATION_RESISTANCE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "body is starting to feel less firm than before.");
                break;
            case POISON_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less healthy than before.");
                break;
            case ACID_IMMUNITY:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to acid than before.");
                break;
            case IMPROVED_ACID_RESISTANCE:
                if (!Acid_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to acid than before.");
                break;
            case ACID_RESISTANCE:
                if (!Improved_acid_resistance && !Acid_immunity)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to acid than before.");
                break;
            case STONE_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel a bit less limber than before.");
                break;
            case DRAIN_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel more suspectible to draining than before.");
                break;
            case SICK_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel more bothered by bugs.");
                break;
            case INVULNERABLE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel more prone to damage than before.");
                break;
            case ANTIMAGIC:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from magic.");
                break;
            case MAGIC_MISSILE_IMMUNITY:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from magic missiles.");
                break;
            case IMPROVED_MAGIC_MISSILE_RESISTANCE:
                if (!Magic_missile_immunity)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from magic missiles.");
                break;
            case MAGIC_MISSILE_RESISTANCE:
                if (!Improved_magic_missile_resistance && !Magic_missile_immunity)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from magic missiles.");
                break;
            case CANCELLED:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic is about to start to flow more normally.");
                break;
            case CANCELLATION_RESISTANCE:
                /* Nothing intentionally */
                break;
            case THREE_FOURTHS_MAGIC_RESISTANCE:
                if (!Half_magic_resistance && !One_fourth_magic_resistance && !No_magic_resistance)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is starting to work more properly.");
                break;
            case HALVED_MAGIC_RESISTANCE:
                if (!One_fourth_magic_resistance && !No_magic_resistance)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is starting to work more properly.");
                break;
            case ONE_FOURTH_MAGIC_RESISTANCE:
                if (!No_magic_resistance)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is starting to work more properly.");
                break;
            case NO_MAGIC_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is starting to work more properly.");
                break;
            case PARALYZED:
                //if(!(Undead_immobility && is_undead(youmonst.data)))
                //    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "limbs are starting move a bit.");
                break;
            case UNDEAD_IMMOBILITY:
                //if (!(Undead_immobility && is_undead(youmonst.data)))
                //    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "limbs are starting move a bit.");
                break;
            case FEARFUL:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to regain your composure.");
                break;
            case SLEEPING:
                //You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to wake up.");
                break;
            case SUMMON_FORBIDDEN:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel summoning is starting to work a bit more properly again.");
                break;
            case CHARMED:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "own motivations are starting to make a bit more sense to you.");
                break;
            case UNDEAD_CONTROL:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to be more in control of your own actions.");
                break;
            case DEATH_RESISTANCE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "soul's silver cord is starting to feel thinner than before.");
                break;
            case CHARM_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less certain of your own motivations.");
                break;
            case FEAR_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less courageous.");
                break;
            case MIND_SHIELDING:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from mental detection.");
                break;
            case LYCANTHROPY_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from lycanthropy.");
                break;
            case CURSE_RESISTANCE:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected from curses.");
                break;
            case LIFESAVED:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel more mortal than before.");
                break;
            case DETECT_MONSTERS:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less sensitive to the presence of monsters than before.");
                break;
            case BLIND_TELEPATHY:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less telepathic when blind than before.");
                break;
            case TELEPAT:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less telepathic than before.");
                break;
            case XRAY_VISION:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "vision through the walls is starting to get blurred.");
                break;
            case WATER_WALKING:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less able to walk on water than before.");
                break;
            case MAGICAL_BREATHING:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less able to breathe in water than before.");
                break;
            case DISPLACED:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "mirror image is starting to vanish.");
                break;
            case CONFLICT:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "neighborhood is starting to feel less quarrelsome than before.");
                break;
            case MAGICAL_PROTECTION:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less protected than before.");
                break;
            case MAGICAL_SHIELDING:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less shielded than before.");
                break;
            case MAGICAL_BARKSKIN:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel less bark-like than before.");
                break;
            case MAGICAL_STONESKIN:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel less stone-like than before.");
                break;
            case BISECTION_RESISTANCE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel less steel-like than before.");
                break;
            case TITAN_STRENGTH:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less strong than before.");
                break;
            case DIVINE_ENDURANCE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "endurance is starting to feel worse than before.");
                break;
            case DIVINE_DEXTERITY:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less dextrous than before.");
                break;
            case DIVINE_INTELLECT:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less smart than before.");
                break;
            case DIVINE_WISDOM:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "common sense is starting to feel less abundant than before.");
                break;
            case DIVINE_CHARISMA:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are starting to feel less charming than before.");
                break;
            case SLIME_RESISTANCE:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin is starting to feel less fiery than before.");
                break;
            }
        }
        else if ((upp->intrinsic & TIMEOUT) > 0)
        {
            // Continuous warning
            switch (propnum) {
            case STRANGLED:
                if (!Breathless)
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are being strangled!");
                else
                    upp->intrinsic &= ~TIMEOUT; /* You can breathe, so clear the strangulation timeout -- It will be set to the full time value below */
                break;
            case AIRLESS_ENVIRONMENT:
                if (!Survives_without_air)
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "cannot breathe!");
                else
                    upp->intrinsic &= ~TIMEOUT; /* You can breathe, so clear the suffocation timeout -- It will be set to the full time value below */
                break;
            }
        }

        /* Finally, add time to recurring intrinsics */
        if ((upp->intrinsic & TIMEOUT) == 0 && property_definitions[propnum].recurring && (upp->extrinsic || (upp->intrinsic & ~TIMEOUT)))
        {
            incr_itimeout(&upp->intrinsic, extratime + property_definitions[propnum].recurring_constant + (property_definitions[propnum].recurring_random > 0 ? rn2(property_definitions[propnum].recurring_random + 1) : 0));
        }
    }

    /* Needs to be called if e.g. amulet of strangulation vanished */
    update_all_character_properties((struct obj*)0, TRUE);

    run_timers();

    /* Condition change check */
    if ((was_flying != !!Flying)
        || (was_levitating != !!Levitation)
        || (had_hallucination != !!Hallucination)
        || (was_blind != !!Blind)
        || (was_deaf != !!Deaf)
        || (was_stunned != !!Stunned)
        || (was_confused != !!Confusion)
        || (was_terminally_ill != !!Sick)
        || (was_food_poisoned != !!FoodPoisoned)
        || (was_stoned != !!Stoned)
        || (was_strangled != (Strangled && !Breathless))
        || (was_suffocating != (Airless_environment && !Survives_without_air))
        || (was_vomiting != !!Vomiting)
        || (was_slimed != !!Slimed)
        || (had_mummyrot != !!MummyRot)
        || (was_silenced != !!Silenced)
        || (was_cancelled != !!Cancelled)
        || (was_paralyzed != !!Paralyzed)
        || (was_sleeping != !!Sleeping)
        || (was_fearful != !!Fearful)
        || (old_weight_cap != near_capacity())
        || (old_ustuck != u.ustuck)
        || (old_usteed != u.usteed)
        || (old_ulycn != u.ulycn)
        || (ACURR(A_STR) != oldstr)
        || (ACURR(A_DEX) != olddex)
        || (ACURR(A_CON) != oldcon)
        || (ACURR(A_INT) != oldint)
        || (ACURR(A_WIS) != oldwis)
        || (ACURR(A_CHA) != oldcha)
        || (old_move != get_u_move_speed(TRUE))
        )
        context.botl = context.botlx = 1;


}

void
fall_asleep(how_long, wakeup_msg)
int how_long;
boolean wakeup_msg;
{
    stop_occupation();
    incr_itimeout(&HSleeping, abs(how_long));
    context.botl = context.botlx = 1;
    refresh_u_tile_gui_info(TRUE);

    if (wakeup_msg)
    {
        /* Do nothing at the moment */
    }
}

/* Attach an egg hatch timeout to the given egg.
 *      when = Time to hatch, usually only passed if re-creating an
 *             existing hatch timer. Pass 0L for random hatch time.
 */
void
attach_egg_hatch_timeout(egg, when)
struct obj *egg;
long when;
{
    int i;

    /* stop previous timer, if any */
    (void) stop_timer(HATCH_EGG, obj_to_any(egg));

    /*
     * Decide if and when to hatch the egg.  The old hatch_it() code tried
     * once a turn from age 151 to 200 (inclusive), hatching if it rolled
     * a number x, 1<=x<=age, where x>150.  This yields a chance of
     * hatching > 99.9993%.  Mimic that here.
     */
    if (!when) {
        for (i = (MAX_EGG_HATCH_TIME - 50) + 1; i <= MAX_EGG_HATCH_TIME; i++)
            if (rnd(i) > 150) {
                /* egg will hatch */
                when = (long) i;
                break;
            }
    }
    if (when) {
        (void) start_timer(when, TIMER_OBJECT, HATCH_EGG, obj_to_any(egg));
    }
}

/* prevent an egg from ever hatching */
void
kill_egg(egg)
struct obj *egg;
{
    /* stop previous timer, if any */
    (void) stop_timer(HATCH_EGG, obj_to_any(egg));
}

/* timer callback routine: hatch the given egg */
void
hatch_egg(arg, timeout)
anything *arg;
long timeout;
{
    struct obj *egg;
    struct monst *mon, *mon2;
    coord cc;
    xchar x, y;
    boolean yours, tamed, silent, knows_egg = FALSE;
    boolean cansee_hatchspot = FALSE;
    int i, mnum, hatchcount = 0;

    egg = arg->a_obj;
    /* sterilized while waiting */
    if (egg->corpsenm == NON_PM)
        return;

    mon = mon2 = (struct monst *) 0;
    mnum = big_to_little(egg->corpsenm);
    /* The identity of one's father is learned, not innate */
    yours = (egg->speflags & SPEFLAGS_YOURS) != 0;
    tamed = yours || (!flags.female && carried(egg) && !rn2(2));
    silent = (timeout != monstermoves); /* hatched while away */

    /* only can hatch when in INVENT, FLOOR, MINVENT */
    if (get_obj_location(egg, &x, &y, 0)) {
        hatchcount = rnd((int) egg->quan);
        cansee_hatchspot = cansee(x, y) && !silent;
        if (!(mons[mnum].geno & G_UNIQ)
            && !(mvitals[mnum].mvflags & (MV_GENOCIDED | MV_EXTINCT))) {
            for (i = hatchcount; i > 0; i--) {
                if (!enexto(&cc, x, y, &mons[mnum])
                    || !(mon = makemon(&mons[mnum], cc.x, cc.y, MM_NO_MONSTER_INVENTORY)))
                    break;
                /* tame if your own egg hatches while you're on the
                   same dungeon level, or any dragon egg which hatches
                   while it's in your inventory */
                if ((tamed && !silent)
                    || (carried(egg) && mon->data->mlet == S_DRAGON)) {
                    if (tamedog(mon, (struct obj *) 0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, FALSE, FALSE)) {
                        if (carried(egg) && mon->data->mlet != S_DRAGON)
                            mon->mtame = 20;
                    }
                }
                if (mon) /* Hatched monsters do not have rumors obviously */
                {
                    mon->rumorsleft = 0;
                    if (yours)
                        mon->mon_flags |= MON_FLAGS_YOUR_CHILD;
                }
                if (mvitals[mnum].mvflags & MV_EXTINCT)
                    break;  /* just made last one */
                mon2 = mon; /* in case makemon() fails on 2nd egg */
            }
            if (!mon)
                mon = mon2;
            hatchcount -= i;
            egg->quan -= (long) hatchcount;
            play_sfx_sound_at_location(SFX_EGG_HATCH, x, y);
        }
#if 0
    /*
     * We could possibly hatch while migrating, but the code isn't
     * set up for it...
     */
    } else if (obj->where == OBJ_MIGRATING) {
        /*
         * We can do several things.  The first ones that come to
         * mind are:
         * + Create the hatched monster then place it on the migrating
         *   mons list.  This is tough because all makemon() is made
         *   to place the monster as well.  Makemon() also doesn't lend
         *   itself well to splitting off a "not yet placed" subroutine.
         * + Mark the egg as hatched, then place the monster when we
         *   place the migrating objects.
         * + Or just kill any egg which gets sent to another level.
         *   Falling is the usual reason such transportation occurs.
         */
        cansee_hatchspot = FALSE;
        mon = ???;
#endif
    }

    if (mon) {
        char monnambuf[BUFSZ], carriedby[BUFSZ];
        boolean siblings = (hatchcount > 1), redraw = FALSE;

        if (cansee_hatchspot) {
            /* [bug?  m_monnam() yields accurate monster type
               regardless of hallucination] */
            Sprintf(monnambuf, "%s%s", siblings ? "some " : "",
                    siblings ? makeplural(m_monnam(mon)) : an(m_monnam(mon)));
            /* we don't learn the egg type here because learning
               an egg type requires either seeing the egg hatch
               or being familiar with the egg already,
               as well as being able to see the resulting
               monster, checked below
            */
        }

        play_movement_sound(mon, CLIMBING_TYPE_STAIRS_DOWN);

        switch (egg->where) {
        case OBJ_INVENT:
            knows_egg = TRUE; /* true even if you are blind */
            if (!cansee_hatchspot)
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s from your pack!", something,
                         locomotion(mon->data, "drop"));
            else
                You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s out of your pack!", monnambuf,
                        locomotion(mon->data, "drop"));
            if (tamed) {
                pline("%s cries sound like \"%s%s\"",
                      siblings ? "Their" : "Its",
                      flags.female ? "mommy" : "daddy", yours ? "." : "?");
            } else if (mon->data->mlet == S_DRAGON && !Deaf) {
                verbalize("Gleep!"); /* Mything eggs :-) */
            }
            break;

        case OBJ_FLOOR:
            if (cansee_hatchspot) {
                knows_egg = TRUE;
                You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s hatch.", monnambuf);
                redraw = TRUE; /* update egg's map location */
            }
            break;

        case OBJ_MINVENT:
            if (cansee_hatchspot) {
                /* egg carrying monster might be invisible */
                mon2 = egg->ocarry;
                if (canseemon(mon2)
                    && (!mon2->wormno || cansee(mon2->mx, mon2->my))) {
                    Sprintf(carriedby, "%s pack",
                            s_suffix(a_monnam(mon2)));
                    knows_egg = TRUE;
                } else if (is_pool(mon->mx, mon->my)) {
                    Strcpy(carriedby, "empty water");
                } else {
                    Strcpy(carriedby, "thin air");
                }
                You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s out of %s!", monnambuf,
                        locomotion(mon->data, "drop"), carriedby);
            }
            break;
#if 0
        case OBJ_MIGRATING:
            break;
#endif
        default:
            impossible("egg hatched where? (%d)", (int) egg->where);
            break;
        }

        if (cansee_hatchspot && knows_egg)
            learn_egg_type(mnum);

        if (egg->quan > 0) {
            /* still some eggs left */
            /* Instead of ordinary egg timeout use a short one */
            attach_egg_hatch_timeout(egg, (long) rnd(12));
        } else if (carried(egg)) {
            useup(egg);
        } else {
            /* free egg here because we use it above */
            obj_extract_self(egg);
            obfree(egg, (struct obj *) 0);
        }
        if (redraw)
            newsym(x, y);

        if (mon && yours && !has_mname(mon) && is_tame(mon) && cansee_hatchspot)
        {
            char buf[BUFSZ] = "";
            char monnambuf[BUFSZ] = "";
            char qbuf[QBUFSZ] = "";
            /* Give true name to the hatch child */
            Sprintf(qbuf, "Which name do you want to give to %s?",
                distant_monnam(mon, ARTICLE_THE, monnambuf));
            getlin_ex(GETLINE_ASK_NAME, ATR_NONE, NO_COLOR, qbuf, buf, "type the name", (char*)0, "You have gained offspring.");
            if (!*buf || *buf == '\033')
            {
                //Nothing
            }
            else
            {
                /* strip leading and trailing spaces; unnames monster if all spaces */
                (void)mungspaces(buf);
                if ((mon->data->geno & G_UNIQ) || mon->ispriest || mon->isminion || mon->isshk || mon->issmith || mon->isnpc)
                {
                    pline("Unfortunately, %s will not accept the name %s.", monnambuf, buf);
                }
                else
                {
                    (void)christen_monst(mon, buf);
                    mon->u_know_mname = 1;
                    /* Clear out umname */
                    if (has_umname(mon))
                        free_umname(mon);
                }
            }
        }
    }
}

/* Learn to recognize eggs of the given type. */
void
learn_egg_type(mnum)
int mnum;
{
    /* baby monsters hatch from grown-up eggs */
    if (mnum > NON_PM && mnum < NUM_MONSTERS)
    {
        mnum = little_to_big(mnum);
        mvitals[mnum].mvflags |= MV_KNOWS_EGG;
        /* we might have just learned about other eggs being carried */
        update_inventory();
    }
}

/* Learn to recognize eggs of the given type. */
void
learn_corpse_type(mnum)
int mnum;
{
    if(mnum > NON_PM && mnum < NUM_MONSTERS)
        mvitals[mnum].mvflags |= MV_KNOWS_CORPSE;
}

/* Attach a fig_transform timeout to the given figurine. */
void
attach_fig_transform_timeout(figurine)
struct obj *figurine;
{
    int i;

    /* stop previous timer, if any */
    (void) stop_timer(FIG_TRANSFORM, obj_to_any(figurine));

    /*
     * Decide when to transform the figurine.
     */
    i = rnd(9000) + 200;
    /* figurine will transform */
    (void) start_timer((long) i, TIMER_OBJECT, FIG_TRANSFORM,
                       obj_to_any(figurine));
}

/* give a fumble message */
STATIC_OVL void
slip_or_trip()
{
    struct obj *otmp = vobj_at(u.ux, u.uy), *otmp2;
    const char *what;
    char buf[BUFSZ];
    boolean on_foot = TRUE;
    if (u.usteed)
        on_foot = FALSE;

    if (otmp && on_foot && !u.uinwater && is_pool(u.ux, u.uy))
        otmp = 0;

    if (otmp && on_foot) { /* trip over something in particular */
        /*
          If there is only one item, it will have just been named
          during the move, so refer to by via pronoun; otherwise,
          if the top item has been or can be seen, refer to it by
          name; if not, look for rocks to trip over; trip over
          anonymous "something" if there aren't any rocks.
        */
        what = (iflags.last_msg == PLNMSG_ONE_ITEM_HERE)
                ? ((otmp->quan == 1L) ? "it"
                      : Hallucination ? "they" : "them")
                : (otmp->dknown || !Blind)
                      ? doname(otmp)
                      : ((otmp2 = sobj_at(ROCK, u.ux, u.uy)) == 0
                             ? something
                             : (otmp2->quan == 1L ? "a rock" : "some rocks"));

        play_player_flounder_sound(MONSTER_FLOUNDER_SOUND_TRIP);
        if (Hallucination) {
            what = strcpy(buf, what);
            buf[0] = highc(buf[0]);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Egads!  %s bite%s your %s!", what,
                  (!otmp || otmp->quan == 1L) ? "s" : "", body_part(FOOT));
        } else {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION,  "trip over %s.", what);
        }
        if (!uarmf && otmp->otyp == CORPSE
            && touch_petrifies(&mons[otmp->corpsenm]) && !Stone_resistance) {
            Sprintf(killer.name, "tripping over %s corpse",
                    an(corpse_monster_name(otmp)));
            killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
            instapetrify(killer.name);
        }
    } else if (rn2(3) && is_ice(u.ux, u.uy)) {
        play_player_flounder_sound(MONSTER_FLOUNDER_SOUND_SLIP);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s%s on the ice.",
              u.usteed ? upstart(x_monnam(u.usteed,
                                          (has_mname(u.usteed)) ? ARTICLE_NONE
                                                                : ARTICLE_THE,
                                          (char *) 0, SUPPRESS_SADDLE, FALSE))
                       : "You",
              rn2(2) ? "slip" : "slide", on_foot ? "" : "s");
    } else {
        if (on_foot) {
            switch (rn2(4)) {
            case 1:
                play_player_flounder_sound(MONSTER_FLOUNDER_SOUND_TRIP);
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "trip over your own %s.",
                    Hallucination ? "elbow" : makeplural(body_part(FOOT)));
                break;
            case 2:
                play_player_flounder_sound(MONSTER_FLOUNDER_SOUND_SLIP);
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "slip %s.",
                    Hallucination ? "on a banana peel" : "and nearly fall");
                break;
            case 3:
                play_player_flounder_sound(MONSTER_FLOUNDER_SOUND_FLOUNDER);
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "flounder.");
                break;
            default:
                play_player_flounder_sound(MONSTER_FLOUNDER_SOUND_STUMBLE);
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "stumble.");
                break;
            }
        } else {
            switch (rn2(4)) {
            case 1:
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s slip out of the stirrups.",
                     makeplural(body_part(FOOT)));
                break;
            case 2:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "let go of the reins.");
                break;
            case 3:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "bang into the saddle-horn.");
                break;
            default:
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "slide to one side of the saddle.");
                break;
            }
            dismount_steed(DISMOUNT_FELL);
        }
    }
}

/* give a fumble message */
STATIC_OVL void
laugh_uncontrollably()
{
    play_simple_player_sound(MONSTER_SOUND_TYPE_LAUGHTER);
    switch (rn2(4)) {
        case 1:
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "start laughing uncontrollably.");
            break;
        case 2:
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly almost laugh your head off.");
            break;
        case 3:
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "remember something funny and laugh out loudly.");
            break;
        default:
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "giggle at some funny thoughts.");
            break;
        }
}

/* give an odd idea message */
STATIC_OVL void
get_odd_idea()
{
    context.oddideacnt++;
    switch (context.oddideacnt)
    {
    case 1:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are suddenly sure that the government is controlled by...");
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "...squid-faced, brain-sucking aliens from inside the hollow earth.");
        break;
    case 2:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly feel as if strange beings shaped like... ");
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "...platonic solids are trying to contact you telepathically.");
        break;
    case 3:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly feel that the radiation inside the dungeon is getting stronger.");
        break;
    case 4:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are suddenly even more convinced that the government is controlled by...");
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "...squid-faced, brain-sucking aliens from inside the hollow earth.");
        break;
    case 5:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly feel as if strange beings shaped like...");
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "...platonic solids are trying to contact you telepathically again.");
        break;
    case 6:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly feel that the radiation inside the dungeon is getting even stronger.");
        context.oddideacnt = 3;
        break;
    default:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly feel somewhat paranoid.");
        break;
    }
}

/* Print a lamp flicker message with tailer. */
STATIC_OVL void
see_lamp_flicker(obj, tailer)
struct obj *obj;
const char *tailer;
{
    switch (obj->where) {
    case OBJ_INVENT:
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flickers%s.", Yname2(obj), tailer);
        break;
    case OBJ_MINVENT:
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flickers%s.", Yname2(obj), tailer);
        break;
    case OBJ_FLOOR:
        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flicker%s.", an(xname(obj)), tailer);
        break;
    }
}

/* Print a dimming message for brass lanterns. */
STATIC_OVL void
lantern_message(obj)
struct obj *obj;
{
    /* from adventure */
    switch (obj->where) {
    case OBJ_INVENT:
        Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "lantern is getting dim.");
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Batteries have not been invented yet.");
        break;
    case OBJ_FLOOR:
        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "a lantern getting dim.");
        break;
    case OBJ_MINVENT:
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s lantern is getting dim.", s_suffix(Monnam(obj->ocarry)));
        break;
    }
}

/*
 * Timeout callback for for objects that are burning. E.g. lamps, candles.
 * See begin_burn() for meanings of obj->age and obj->enchantment.
 */
void
burn_object(arg, timeout)
anything *arg;
long timeout;
{
    struct obj *obj = arg->a_obj;
    boolean canseeit, many, is_candelabrum, need_newsym, need_invupdate;
    xchar x, y;
    char whose[BUFSZ];

    is_candelabrum = is_obj_candelabrum(obj);
    many = is_candelabrum ? obj->special_quality > 1 : obj->quan > 1L;

    /* timeout while away */
    if (timeout != monstermoves) {
        long how_long = monstermoves - timeout;

        if (how_long >= obj->age) {
            obj->age = 0;
            end_burn(obj, FALSE);

            if (is_candelabrum) {
                obj->special_quality = 0; /* no more candles */
                obj->owt = weight(obj);
            } else if (is_candle(obj) || obj->otyp == POT_OIL) {
                /* get rid of candles and burning oil potions;
                   we know this object isn't carried by hero,
                   nor is it migrating */
                obj_extract_self(obj);
                obfree(obj, (struct obj *) 0);
                obj = (struct obj *) 0;
            }

        } else {
            obj->age -= how_long;
            begin_burn(obj, TRUE);
        }
        return;
    }

    /* only interested in INVENT, FLOOR, and MINVENT */
    if (get_obj_location(obj, &x, &y, 0)) {
        canseeit = !Blind && cansee(x, y);
        /* set `whose[]' to be "Your " or "Fred's " or "The goblin's " */
        (void) Shk_Your(whose, obj);
    } else {
        canseeit = FALSE;
    }
    need_newsym = need_invupdate = FALSE;

    /* obj->age is the age remaining at this point.  */
    switch (obj->otyp) {
    case POT_OIL:
        /* this should only be called when we run out */
        if (canseeit) {
            switch (obj->where) {
            case OBJ_INVENT:
                need_invupdate = TRUE;
                /*FALLTHRU*/
            case OBJ_MINVENT:
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%spotion of oil has burnt away.", whose);
                break;
            case OBJ_FLOOR:
                You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "a burning potion of oil go out.");
                need_newsym = TRUE;
                break;
            }
        }
        end_burn(obj, FALSE); /* turn off light source */
        if (carried(obj)) {
            useupall(obj);
        } else {
            /* clear migrating obj's destination code before obfree
               to avoid false complaint of deleting worn item */
            if (obj->where == OBJ_MIGRATING)
                obj->owornmask = 0L;
            obj_extract_self(obj);
            obfree(obj, (struct obj *) 0);
        }
        obj = (struct obj *) 0;
        break;

    case BRASS_LANTERN:
    case OIL_LAMP:
        switch ((int) obj->age) {
        case 150:
        case 100:
        case 50:
            if (canseeit) {
                if (obj->otyp == BRASS_LANTERN)
                    lantern_message(obj);
                else
                    see_lamp_flicker(obj,
                                     obj->age == 50L ? " considerably" : "");
            }
            break;

        case 25:
            if (canseeit) {
                if (obj->otyp == BRASS_LANTERN)
                    lantern_message(obj);
                else {
                    switch (obj->where) {
                    case OBJ_INVENT:
                    case OBJ_MINVENT:
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems about to go out.", Yname2(obj));
                        break;
                    case OBJ_FLOOR:
                        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s about to go out.", an(xname(obj)));
                        break;
                    }
                }
            }
            break;

        case 0:
            /* even if blind you'll know if holding it */
            if (canseeit || obj->where == OBJ_INVENT) {
                switch (obj->where) {
                case OBJ_INVENT:
                    need_invupdate = TRUE;
                    /*FALLTHRU*/
                case OBJ_MINVENT:
                    if (obj->otyp == BRASS_LANTERN)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%slantern has run out of power.", whose);
                    else
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s has gone out.", Yname2(obj));
                    break;
                case OBJ_FLOOR:
                    if (obj->otyp == BRASS_LANTERN)
                        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "a lantern run out of power.");
                    else
                        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s go out.", an(xname(obj)));
                    break;
                }
            }
            end_burn(obj, FALSE);
            break;

        default:
            /*
             * Someone added fuel to the lamp while it was
             * lit.  Just fall through and let begin burn
             * handle the new age.
             */
            break;
        }

        if (obj->age)
            begin_burn(obj, TRUE);

        break;

    case CANDELABRUM_OF_INVOCATION:
    case LARGE_FIVE_BRANCHED_CANDELABRUM:
    case TALLOW_CANDLE:
    case WAX_CANDLE:
        switch (obj->age) {
        case 75:
            if (canseeit)
                switch (obj->where) {
                case OBJ_INVENT:
                case OBJ_MINVENT:
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%scandle%s getting short.", whose,
                        is_candelabrum ? "candelabrum's " : "",
                          many ? "s are" : " is");
                    break;
                case OBJ_FLOOR:
                    You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%scandle%s getting short.",
                        is_candelabrum ? "a candelabrum's " : many ? "some "
                                                                : "a ",
                            many ? "s" : "");
                    break;
                }
            break;

        case 15:
            if (canseeit)
                switch (obj->where) {
                case OBJ_INVENT:
                case OBJ_MINVENT:
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%scandle%s flame%s flicker%s low!", whose,
                        is_candelabrum ? "candelabrum's " : "", many ? "s'" : "'s",
                          many ? "s" : "", many ? "" : "s");
                    break;
                case OBJ_FLOOR:
                    You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%scandle%s flame%s flicker low!",
                        is_candelabrum ? "a candelabrum's " : many ? "some "
                                                                : "a ",
                            many ? "s'" : "'s", many ? "s" : "");
                    break;
                }
            break;

        case 0:
            /* we know even if blind and in our inventory */
            if (canseeit || obj->where == OBJ_INVENT) {
                if (is_candelabrum) {
                    switch (obj->where) {
                    case OBJ_INVENT:
                        need_invupdate = TRUE;
                        /*FALLTHRU*/
                    case OBJ_MINVENT:
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%scandelabrum's flame%s.", whose,
                              many ? "s die" : " dies");
                        break;
                    case OBJ_FLOOR:
                        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "a candelabrum's flame%s die.",
                                many ? "s" : "");
                        break;
                    }
                } else {
                    switch (obj->where) {
                    case OBJ_INVENT:
                        /* no need_invupdate for update_inventory() necessary;
                           useupall() -> freeinv() handles it */
                        /*FALLTHRU*/
                    case OBJ_MINVENT:
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s consumed!", Yname2(obj),
                              many ? "are" : "is");
                        break;
                    case OBJ_FLOOR:
                        /*
                          You see some wax candles consumed!
                          You see a wax candle consumed!
                         */
                        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%s consumed!", many ? "some " : "",
                                many ? xname(obj) : an(xname(obj)));
                        need_newsym = TRUE;
                        break;
                    }

                    /* post message */
                    pline(Hallucination
                              ? (many ? "They shriek!" : "It shrieks!")
                              : Blind ? "" : (many ? "Their flames die."
                                                   : "Its flame dies."));
                }
            }
            end_burn(obj, FALSE);

            if (is_candelabrum) {
                obj->special_quality = 0;
                obj->owt = weight(obj);
            } else {
                if (carried(obj)) {
                    useupall(obj);
                } else {
                    /* clear migrating obj's destination code
                       so obfree won't think this item is worn */
                    if (obj->where == OBJ_MIGRATING)
                        obj->owornmask = 0L;
                    obj_extract_self(obj);
                    obfree(obj, (struct obj *) 0);
                }
                obj = (struct obj *) 0;
            }
            break; /* case [age ==] 0 */

        default:
            /*
             * Someone added fuel (candles) to the menorah while
             * it was lit.  Just fall through and let begin burn
             * handle the new age.
             */
            break;
        }

        if (obj && obj->age)
            begin_burn(obj, TRUE);
        break; /* case [otyp ==] candelabrum|tallow_candle|wax_candle */

    default:
        impossible("burn_object: unexpeced obj %s", xname(obj));
        break;
    }
    if (need_newsym)
        newsym(x, y);
    if (need_invupdate)
        update_inventory();
}

/*
 * Start a burn timeout on the given object. If not "already lit" then
 * create a light source for the vision system.  There had better not
 * be a burn already running on the object.
 *
 * Magic lamps stay lit as long as there's a genie inside, so don't start
 * a timer.
 *
 * Burn rules:
 *      potions of oil, lamps & candles:
 *              age = # of turns of fuel left
 *              special_quality = <unused>
 *      magic lamps:
 *              age = <unused>
 *              special_quality = 0 not lightable, 1 lightable forever
 *      magic candles:
 *              age = <unused>
 *              special_quality = 0 not lightable, 1 lightable forever, partly used, 2 lightable forever, unused
 *      candelabrum:
 *              age = # of turns of fuel left
 *              special_quality = # of candles
 *
 * Once the burn begins, the age will be set to the amount of fuel
 * remaining _once_the_burn_finishes_.  If the burn is terminated
 * early then fuel is added back.
 *
 * This use of age differs from the use of age for corpses and eggs.
 * For the latter items, age is when the object was created, so we
 * know when it becomes "bad".
 *
 * This is a "silent" routine - it should not print anything out.
 */
void
begin_burn(obj, already_lit)
struct obj *obj;
boolean already_lit;
{
    if (!obj)
        return;

    if (obj->age == 0 && !obj_burns_infinitely(obj))
        return;

    int radius = obj_light_radius(obj);
    long turns = 0;
    boolean do_timer = TRUE;
    if (obj_burns_infinitely(obj))
    {
        /* Infinite burn */
        do_timer = FALSE;
        obj->lamplit = 1;

        if (obj->otyp == MAGIC_CANDLE)
        {
            if (obj->special_quality == 2)
                obj->special_quality = 1;
        }
    }
    else
    {

        switch (obj->otyp) {
        case MAGIC_LAMP:
            //obj->lamplit = 1;
            //do_timer = FALSE;
            break;
        case MAGIC_CANDLE:
            //obj->lamplit = 1;
            if (obj->special_quality == 2)
                obj->special_quality = 1;
            //do_timer = FALSE;
            break;
        case POT_OIL:
            turns = obj->age;
            if (obj->odiluted)
                turns = (3L * turns + 2L) / 4L;
            //radius = 1; /* very dim light */
            break;

        case BRASS_LANTERN:
        case OIL_LAMP:
            /* magic times are 150, 100, 50, 25, and 0 */
            if (obj->age > 150L)
                turns = obj->age - 150L;
            else if (obj->age > 100L)
                turns = obj->age - 100L;
            else if (obj->age > 50L)
                turns = obj->age - 50L;
            else if (obj->age > 25L)
                turns = obj->age - 25L;
            else
                turns = obj->age;
            break;

        case CANDELABRUM_OF_INVOCATION:
        case LARGE_FIVE_BRANCHED_CANDELABRUM:
        case TALLOW_CANDLE:
        case WAX_CANDLE:
            /* magic times are 75, 15, and 0 */
            if (obj->age > 75L)
                turns = obj->age - 75L;
            else if (obj->age > 15L)
                turns = obj->age - 15L;
            else
                turns = obj->age;
            //radius = candle_light_range(obj);
            break;

        default:
            if(!is_obj_light_source(obj))
                impossible("begin burn: unexpected %s", xname(obj));
            turns = obj->age;
            break;
        }
    }

    if (do_timer) {
        if (start_timer(turns, TIMER_OBJECT, BURN_OBJECT, obj_to_any(obj))) {
            obj->lamplit = 1;
            obj->age -= turns;
            if (carried(obj) && !already_lit)
                update_inventory();
        } else {
            obj->lamplit = 0;
        }
    } else {
        if (carried(obj) && !already_lit)
            update_inventory();
    }

    if (obj->lamplit && !already_lit) {
        xchar x, y;

        if (get_obj_location(obj, &x, &y, CONTAINED_TOO | BURIED_TOO))
            new_light_source(x, y, radius, LS_OBJECT, obj_to_any(obj));
        else
            impossible("begin_burn: can't get obj position");
    }
}

/*
 * Stop a burn timeout on the given object if timer attached.  Darken
 * light source.
 */
void
end_burn(obj, timer_attached)
struct obj *obj;
boolean timer_attached;
{
    if (!obj->lamplit) {
        impossible("end_burn: obj %s not lit", xname(obj));
        return;
    }

    if (obj_burns_infinitely(obj))
        timer_attached = FALSE;

    if (!timer_attached) {
        /* [DS] Cleanup explicitly, since timer cleanup won't happen */
        del_light_source(LS_OBJECT, obj_to_any(obj));
        obj->lamplit = 0;
        if (obj->where == OBJ_INVENT)
            update_inventory();
    } else if (!stop_timer(BURN_OBJECT, obj_to_any(obj)))
        impossible("end_burn: obj %s not timed!", xname(obj));
}

/*
 * Cleanup a burning object if timer stopped.
 */
STATIC_OVL void
cleanup_burn(arg, expire_time)
anything *arg;
long expire_time;
{
    struct obj *obj = arg->a_obj;
    if (!obj->lamplit) {
        impossible("cleanup_burn: obj %s not lit", xname(obj));
        return;
    }

    del_light_source(LS_OBJECT, obj_to_any(obj));

    /* restore unused time */
    obj->age += expire_time - monstermoves;

    obj->lamplit = 0;

    if (obj->where == OBJ_INVENT)
        update_inventory();
}

//Black Blade
/*
 * Timeout callback for for Black Blade and other summonable objects, very similar to rot_corpse
 */
void
unsummon_item(arg, timeout)
anything* arg;
long timeout;
{
    xchar x = 0, y = 0;
    struct obj* obj = arg->a_obj;
    boolean on_floor = obj->where == OBJ_FLOOR,
        in_invent = obj->where == OBJ_INVENT,
        canseeunsummon = FALSE,
        iswielded = FALSE;
    char whosebuf[BUFSZ] = "";

    if (!obj)
        return;

    if (timeout)
    {
        /* Do nothing */
    }

    if (on_floor) {
        x = obj->ox;
        y = obj->oy;
        if(cansee(x,y))
        {
            canseeunsummon = TRUE;
            iswielded = FALSE;
            strcpy(whosebuf, "The ");
        }
    }
    else if (in_invent) {
        if (obj->owornmask)
            remove_worn_item(obj, TRUE);

        strcpy(whosebuf, "Your ");
        canseeunsummon = TRUE;
    }
    else if (obj->where == OBJ_MINVENT && obj->owornmask) {
        if (obj == MON_WEP(obj->ocarry))
        {
            setmnotwielded(obj->ocarry, obj);
        }
        else
        {
            
        }
        if (obj->ocarry && canseemon(obj->ocarry))
        {
            canseeunsummon = TRUE;
            strcpy(whosebuf, s_suffix(Monnam(obj->ocarry)));
            strcat(whosebuf, " ");
            iswielded = FALSE; //Do not show this for monsters
        }
    }
    else if (obj->where == OBJ_MIGRATING) {
        /* clear destination flag so that obfree()'s check for
           freeing a worn object doesn't get a false hit */
        obj->owornmask = 0L;
        canseeunsummon = FALSE;
    }

    if (flags.verbose && canseeunsummon) {
        char* bbname = xname(obj);
        boolean animon = FALSE;
        if (isok(x, y))
        {
            animon = TRUE;
            play_special_effect_at(SPECIAL_EFFECT_PUFF_OF_SMOKE, 0, x, y, FALSE);
            play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, x, y);
            special_effect_wait_until_action(0);
        }

        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%s%s %s in a puff of smoke%c", whosebuf, iswielded ? "wielded " : "", bbname,
            otense(obj, "vanish"), iswielded ? '!' : '.');

        if (animon)
        {
            special_effect_wait_until_end(0);
        }
    }


    //Destroy item
    if (carried(obj)) {
        useupall(obj);
    }
    else {
        /* clear migrating obj's destination code
           so obfree won't think this item is worn */
        obj_extract_self(obj);
        obfree(obj, (struct obj*) 0);
    }
    obj = (struct obj*) 0;

    //Additional floor considerations
    if (on_floor) {
        struct monst* mtmp = m_at(x, y);

        /* a hiding monster may be exposed */
        if (mtmp && !OBJ_AT(x, y) && mtmp->mundetected
            && hides_under(mtmp->data)) {
            mtmp->mundetected = 0;
        }
        else if (x == u.ux && y == u.uy && u.uundetected && hides_under(youmonst.data))
            (void)hideunder(&youmonst);
        newsym(x, y);
    }
    else if (in_invent)
        update_inventory();

}

/*
 * Start an existence timeout on the given object. 
 */
void
begin_existence(obj)
struct obj* obj;
{
    if (start_timer(obj->age, TIMER_OBJECT, ITEM_UNSUMMON, obj_to_any(obj))) {
        obj->age = 0; //Not strictly necessary
    }
}

//Monster summons here

/*
 * Timeout callback for for summon monster spells
 */
void
unsummon_monster(arg, timeout)
anything* arg;
long timeout;
{
    struct monst* mon = arg->a_monst;

    if (!mon || (mon && DEADMONSTER(mon)))
        return;

    if (timeout)
    {
        /* Do nothing */
    }

    if (canseemon(mon)) {
        play_special_effect_at(SPECIAL_EFFECT_PUFF_OF_SMOKE, 0, mon->mx, mon->my, FALSE);
        play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mon->mx, mon->my);
        special_effect_wait_until_action(0);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s vanishes in a puff of smoke!", Monnam(mon));
        special_effect_wait_until_end(0);
    }
    else
        play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mon->mx, mon->my);

    if (glyph_is_invisible(levl[mon->mx][mon->my].hero_memory_layers.glyph))
        unmap_object(mon->mx, mon->my);

    release_monster_objects(mon, FALSE, FALSE, FALSE);
    mongone(mon);
}


/*
 * Start a summon timeout on the given monster.
 */
void
begin_summontimer(mon)
struct monst* mon;
{
    if (start_timer(mon->summonduration, TIMER_MONSTER, MONSTER_UNSUMMON, monst_to_any(mon))) {
        mon->summonduration = 0; //Not strictly necessary
    }
}



/*
 * Start a summon timeout on the given monster.
 */
void
begin_timestoptimer(duration)
long duration;
{
    anything any = zeroany;
    if (start_timer(duration, TIMER_GLOBAL, TIME_RESTART, &any))
    {
        //Success
    }
}


void
restart_time(arg, timeout)
anything* arg;
long timeout;
{
    if (!arg && timeout)
    {
        /* Do nothing */
    }

    context.time_stopped = FALSE;
    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "The flow of time seems faster again.");
}


void
do_storms()
{
    int nstrike;
    register int x, y;
    int dirx, diry;
    int count;

    /* no lightning if not the air level or too often, even then */
    if (!Is_airlevel(&u.uz) || rn2(8))
        return;

    /* the number of strikes is 8-log2(nstrike) */
    for (nstrike = rnd(64); nstrike <= 64; nstrike *= 2) {
        count = 0;
        do {
            x = rnd(COLNO - 1);
            y = rn2(ROWNO);
        } while (++count < 100 && levl[x][y].typ != CLOUD);

        if (count < 100) {
            dirx = rn2(3) - 1;
            diry = rn2(3) - 1;
            if (dirx != 0 || diry != 0)
                buzz(-15, /* "monster" LIGHTNING spell */
                     (struct obj*)0, (struct monst*)0, 8, 8, 0, x, y, dirx, diry);
        }
    }

    if (levl[u.ux][u.uy].typ == CLOUD) {
        /* Inside a cloud during a thunder storm is deafening. */
        /* Even if already deaf, we sense the thunder's vibrations. */
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Kaboom!!!  Boom!!  Boom!!");
        incr_itimeout(&HDeaf, rn1(20, 30));
        context.botl = context.botlx = TRUE;
        refresh_u_tile_gui_info(TRUE);
        play_environment_ambient_sounds();
        if (!u.uinvulnerable) {
            stop_occupation();
            nomul(-3);
            multi_reason = "hiding from thunderstorm";
            nomovemsg = 0;
        }
    } else
        You_hear("a rumbling noise.");
}

/* -------------------------------------------------------------------------
 */
/*
 * Generic Timeout Functions.
 *
 * Interface:
 *
 * General:
 *  boolean start_timer(long timeout,short kind,short func_index,
 *                      anything *arg)
 *      Start a timer of kind 'kind' that will expire at time
 *      monstermoves+'timeout'.  Call the function at 'func_index'
 *      in the timeout table using argument 'arg'.  Return TRUE if
 *      a timer was started.  This places the timer on a list ordered
 *      "sooner" to "later".  If an object, increment the object's
 *      timer count.
 *
 *  long stop_timer(short func_index, anything *arg)
 *      Stop a timer specified by the (func_index, arg) pair.  This
 *      assumes that such a pair is unique.  Return the time the
 *      timer would have gone off.  If no timer is found, return 0.
 *      If an object, decrement the object's timer count.
 *
 *  long peek_timer(short func_index, anything *arg)
 *      Return time specified timer will go off (0 if no such timer).
 *
 *  void run_timers(void)
 *      Call timers that have timed out.
 *
 * Save/Restore:
 *  void save_timers(int fd, int mode, int range)
 *      Save all timers of range 'range'.  Range is either global
 *      or local.  Global timers follow game play, local timers
 *      are saved with a level.  Object and monster timers are
 *      saved using their respective id's instead of pointers.
 *
 *  void restore_timers(int fd, int range, boolean ghostly, long adjust)
 *      Restore timers of range 'range'.  If from a ghost pile,
 *      adjust the timeout by 'adjust'.  The object and monster
 *      ids are not restored until later.
 *
 *  void relink_timers(boolean ghostly)
 *      Relink all object and monster timers that had been saved
 *      using their object's or monster's id number.
 *
 * Object Specific:
 *  void obj_move_timers(struct obj *src, struct obj *dest)
 *      Reassign all timers from src to dest.
 *
 *  void obj_split_timers(struct obj *src, struct obj *dest)
 *      Duplicate all timers assigned to src and attach them to dest.
 *
 *  void obj_stop_timers(struct obj *obj)
 *      Stop all timers attached to obj.
 *
 *  boolean obj_has_timer(struct obj *object, short timer_type)
 *      Check whether object has a timer of type timer_type.
 */

STATIC_DCL const char *FDECL(kind_name, (SHORT_P));
STATIC_DCL void FDECL(print_queue, (winid, timer_element *));
STATIC_DCL void FDECL(insert_timer, (timer_element *));
STATIC_DCL timer_element *FDECL(remove_timer,
                                (timer_element **, SHORT_P, ANY_P *));
STATIC_DCL void FDECL(write_timer, (int, timer_element *));
STATIC_DCL boolean FDECL(mon_is_local, (struct monst *));
STATIC_DCL boolean FDECL(timer_is_local, (timer_element *));
STATIC_DCL int FDECL(maybe_write_timer, (int, int, BOOLEAN_P));

/* ordered timer list */
STATIC_VAR timer_element *timer_base; /* "active" */
STATIC_VAR unsigned long timer_id = 1;

/* If defined, then include names when printing out the timer queue */
#define VERBOSE_TIMER

typedef struct {
    timeout_proc f, cleanup;
#ifdef VERBOSE_TIMER
    const char *name;
#define TTAB(a, b, c) \
    {                 \
        a, b, c       \
    }
#else
#define TTAB(a, b, c) \
    {                 \
        a, b          \
    }
#endif
} ttable;

/* table of timeout functions */
STATIC_VAR const ttable timeout_funcs[NUM_TIME_FUNCS] = {
    TTAB(rot_organic, (timeout_proc) 0, "rot_organic"),
    TTAB(rot_corpse, (timeout_proc) 0, "rot_corpse"),
    TTAB(revive_mon, (timeout_proc) 0, "revive_mon"),
    TTAB(burn_object, cleanup_burn, "burn_object"),
    TTAB(make_sound_object, cleanup_sound,  "make_sound_object"),
    TTAB(hatch_egg, (timeout_proc) 0, "hatch_egg"),
    TTAB(fig_transform, (timeout_proc) 0, "fig_transform"),
    TTAB(melt_ice_away, (timeout_proc) 0, "melt_ice_away"),
    TTAB(unsummon_item, (timeout_proc)0, "unsummon_item"),
    TTAB(unsummon_monster, (timeout_proc)0, "unsummon_monster"),
    TTAB(restart_time, (timeout_proc)0, "restart_time")
};
#undef TTAB

STATIC_OVL const char *
kind_name(kind)
short kind;
{
    switch (kind) {
    case TIMER_LEVEL:
        return "level";
    case TIMER_GLOBAL:
        return "global";
    case TIMER_OBJECT:
        return "object";
    case TIMER_MONSTER:
        return "monster";
    }
    return "unknown";
}

STATIC_OVL void
print_queue(win, base)
winid win;
timer_element *base;
{
    timer_element *curr;
    char buf[BUFSZ];

    if (!base) {
        putstr(win, 0, " <empty>");
    } else {
        putstr(win, 0, "timeout  id   kind   call");
        for (curr = base; curr; curr = curr->next) {
#ifdef VERBOSE_TIMER
            Sprintf(buf, " %4ld   %4ld  %-6s %s(%s)", curr->timeout,
                    curr->tid, kind_name(curr->kind),
                    timeout_funcs[curr->func_index].name,
                    fmt_ptr((genericptr_t) curr->arg.a_void));
#else
            Sprintf(buf, " %4ld   %4ld  %-6s #%d(%s)", curr->timeout,
                    curr->tid, kind_name(curr->kind), curr->func_index,
                    fmt_ptr((genericptr_t) curr->arg.a_void));
#endif
            putstr(win, 0, buf);
        }
    }
}

int
wiz_timeout_queue()
{
    winid win;
    char buf[BUFSZ];
    const char *propname;
    long intrinsic;
    int i, p, count, longestlen, ln, specindx = 0;

    win = create_nhwindow(NHW_MENU); /* corner text window */
    if (win == WIN_ERR)
        return 0;

    Sprintf(buf, "Current time = %ld.", monstermoves);
    putstr(win, 0, buf);
    putstr(win, 0, "");
    putstr(win, 0, "Active timeout queue:");
    putstr(win, 0, "");
    print_queue(win, timer_base);

    /* Timed properies:
     * check every one; the majority can't obtain temporary timeouts in
     * normal play but those can be forced via the #wizintrinsic command.
     */
    count = longestlen = 0;
    for (i = 0; (propname = propertynames[i].prop_name) != 0; ++i) {
        p = propertynames[i].prop_num;
        intrinsic = u.uprops[p].intrinsic;
        if (intrinsic & TIMEOUT) {
            ++count;
            if ((ln = (int) strlen(propname)) > longestlen)
                longestlen = ln;
        }
        if (specindx == 0 && p == FIRE_IMMUNITY)
            specindx = i;
    }
    putstr(win, 0, "");
    if (!count) {
        putstr(win, 0, "No timed properties.");
    } else {
        putstr(win, 0, "Timed properties:");
        putstr(win, 0, "");
        for (i = 0; (propname = propertynames[i].prop_name) != 0; ++i) {
            p = propertynames[i].prop_num;
            intrinsic = u.uprops[p].intrinsic;
            if (intrinsic & TIMEOUT) {
                if (specindx > 0 && i >= specindx) {
                    putstr(win, 0, " -- settable via #wizinstrinc only --");
                    specindx = 0;
                }
                /* timeout value can be up to 16777215 (0x00ffffff) but
                   width of 4 digits should result in values lining up
                   almost all the time (if/when they don't, it won't
                   look nice but the information will still be accurate) */
                Sprintf(buf, " %*s %4ld", -longestlen, propname,
                        (intrinsic & TIMEOUT));
                putstr(win, 0, buf);
            }
        }
    }
    display_nhwindow(win, FALSE);
    destroy_nhwindow(win);

    return 0;
}

void
timer_sanity_check()
{
    timer_element *curr;

    /* this should be much more complete */
    for (curr = timer_base; curr; curr = curr->next)
    {
        if (curr->kind == TIMER_OBJECT) {
            struct obj* obj = curr->arg.a_obj;

            if (obj->timed == 0) {
                impossible("timer sanity: untimed obj %s, timer %ld",
                    fmt_ptr((genericptr_t)obj), curr->tid);
            }
        }
        else if (curr->kind == TIMER_MONSTER) {
            struct monst* mon = curr->arg.a_monst;

            if (mon->timed == 0) {
                impossible("timer sanity: untimed mon %s, timer %ld",
                    fmt_ptr((genericptr_t)mon), curr->tid);
            }
        }
    }
}

/*
 * Pick off timeout elements from the global queue and call their functions.
 * Do this until their time is less than or equal to the move count.
 */
void
run_timers()
{
    timer_element *curr;

    /*
     * Always use the first element.  Elements may be added or deleted at
     * any time.  The list is ordered, we are done when the first element
     * is in the future.
     */
    while (timer_base && timer_base->timeout <= monstermoves) {
        curr = timer_base;
        timer_base = curr->next;

        if (curr->kind == TIMER_OBJECT)
        {
            (curr->arg.a_obj)->timed--;
        }
        else if (curr->kind == TIMER_MONSTER)
        {
            (curr->arg.a_monst)->timed--;
        }

        (*timeout_funcs[curr->func_index].f)(&curr->arg, curr->timeout);
        free((genericptr_t) curr);
    }
}

/*
 * Start a timer.  Return TRUE if successful.
 */
boolean
start_timer(when, kind, func_index, arg)
long when;
short kind;
short func_index;
anything *arg;
{
    timer_element* gnu;

    if (func_index < 0 || func_index >= NUM_TIME_FUNCS)
    {
        panic("start_timer");
        return FALSE;
    }

    gnu = (timer_element*)alloc(sizeof(timer_element));
    (void)memset((genericptr_t)gnu, 0, sizeof(timer_element));
    gnu->next = 0;
    gnu->tid = timer_id++;
    gnu->timeout = monstermoves + when;
    gnu->kind = kind;
    gnu->needs_fixup = 0;
    gnu->func_index = func_index;
    gnu->arg = *arg;
    insert_timer(gnu);

    if (kind == TIMER_OBJECT) /* increment object's timed count */
    {
        (arg->a_obj)->timed++;
    }
    else if (kind == TIMER_MONSTER) /* increment monster's timed count */
    {
        (arg->a_monst)->timed++;
    }
    /* should check for duplicates and fail if any */
    return TRUE;
}

/*
 * Remove the timer from the current list and free it up.  Return the time
 * remaining until it would have gone off, 0 if not found.
 */
long
stop_timer(func_index, arg)
short func_index;
anything *arg;
{
    timer_element *doomed;
    long timeout;

    doomed = remove_timer(&timer_base, func_index, arg);

    if (doomed) {
        timeout = doomed->timeout;
        if (doomed->kind == TIMER_OBJECT) {
            (arg->a_obj)->timed--;
        }
        else if (doomed->kind == TIMER_MONSTER) {
            (arg->a_monst)->timed--;
        }
        if (timeout_funcs[doomed->func_index].cleanup)
            (*timeout_funcs[doomed->func_index].cleanup)(arg, timeout);
        free((genericptr_t) doomed);
        return (timeout - monstermoves);
    }
    return 0L;
}

/*
 * Find the timeout of specified timer; return 0 if none.
 */
long
peek_timer(type, arg)
short type;
anything *arg;
{
    timer_element *curr;

    for (curr = timer_base; curr; curr = curr->next) {
        if (curr->func_index == type && curr->arg.a_void == arg->a_void)
            return curr->timeout;
    }
    return 0L;
}

/*
 * Move all object timers from src to dest, leaving src untimed.
 */
void
obj_move_timers(src, dest)
struct obj *src, *dest;
{
    int count;
    timer_element *curr;

    for (count = 0, curr = timer_base; curr; curr = curr->next) {
        if (curr->kind == TIMER_OBJECT && curr->arg.a_obj == src) {
            curr->arg.a_obj = dest;
            dest->timed++;
            count++;
        }
    }
    if (count != src->timed)
        panic("obj_move_timers");
    src->timed = 0;
}

/*
 * Move all object timers from src to dest, leaving src untimed.
 */
void
mon_move_timers(src, dest)
struct monst* src, * dest;
{
    int count;
    timer_element* curr;

    for (count = 0, curr = timer_base; curr; curr = curr->next) {
        if (curr->kind == TIMER_MONSTER && curr->arg.a_monst == src) {
            curr->arg.a_monst = dest;
            dest->timed++;
            count++;
        }
    }
    if (count != src->timed)
        panic("mon_move_timers");
    src->timed = 0;
}

/*
 * Find all object timers and duplicate them for the new object "dest".
 */
void
obj_split_timers(src, dest)
struct obj *src, *dest;
{
    timer_element *curr, *next_timer = 0;

    for (curr = timer_base; curr; curr = next_timer) {
        next_timer = curr->next; /* things may be inserted */
        if (curr->kind == TIMER_OBJECT && curr->arg.a_obj == src) {
            (void) start_timer(curr->timeout - monstermoves, TIMER_OBJECT,
                               curr->func_index, obj_to_any(dest));
        }
    }
}

/*
 * Find all monster timers and duplicate them for the new monster "dest".
 */
void
mon_split_timers(src, dest)
struct monst* src, * dest;
{
    timer_element* curr, * next_timer = 0;

    for (curr = timer_base; curr; curr = next_timer) {
        next_timer = curr->next; /* things may be inserted */
        if (curr->kind == TIMER_MONSTER && curr->arg.a_monst == src) {
            (void)start_timer(curr->timeout - monstermoves, TIMER_MONSTER,
                curr->func_index, monst_to_any(dest));
        }
    }
}

/*
 * Stop all timers attached to this object.  We can get away with this because
 * all object pointers are unique.
 */
void
obj_stop_timers(obj)
struct obj *obj;
{
    timer_element *curr, *prev, *next_timer = 0;

    for (prev = 0, curr = timer_base; curr; curr = next_timer) {
        next_timer = curr->next;
        if (curr->kind == TIMER_OBJECT && curr->arg.a_obj == obj) {
            if (prev)
                prev->next = curr->next;
            else
                timer_base = curr->next;
            if (timeout_funcs[curr->func_index].cleanup)
                (*timeout_funcs[curr->func_index].cleanup)(&curr->arg,
                                                           curr->timeout);
            free((genericptr_t) curr);
        } else {
            prev = curr;
        }
    }
    obj->timed = 0;
}

/*
 * Stop all timers attached to this monster.  We can get away with this because
 * all object pointers are unique.
 */
void
mon_stop_timers(mon)
struct monst* mon;
{
    timer_element* curr, * prev, * next_timer = 0;

    for (prev = 0, curr = timer_base; curr; curr = next_timer) {
        next_timer = curr->next;
        if (curr->kind == TIMER_MONSTER && curr->arg.a_monst == mon) {
            if (prev)
                prev->next = curr->next;
            else
                timer_base = curr->next;
            if (timeout_funcs[curr->func_index].cleanup)
                (*timeout_funcs[curr->func_index].cleanup)(&curr->arg,
                    curr->timeout);
            free((genericptr_t)curr);
        }
        else {
            prev = curr;
        }
    }
    mon->timed = 0;
}


/*
 * Check whether object has a timer of type timer_type.
 */
boolean
obj_has_timer(object, timer_type)
struct obj *object;
short timer_type;
{
    long timeout = peek_timer(timer_type, obj_to_any(object));

    return (boolean) (timeout != 0L);
}

/*
 * Check whether object has a timer of type timer_type.
 */
boolean
mon_has_timer(mon, timer_type)
struct monst* mon;
short timer_type;
{
    long timeout = peek_timer(timer_type, monst_to_any(mon));

    return (boolean)(timeout != 0L);
}

/*
 * Stop all timers of index func_index at this spot.
 *
 */
void
spot_stop_timers(x, y, func_index)
xchar x, y;
short func_index;
{
    timer_element *curr, *prev, *next_timer = 0;
    long where = (((long) x << 16) | ((long) y));

    for (prev = 0, curr = timer_base; curr; curr = next_timer) {
        next_timer = curr->next;
        if (curr->kind == TIMER_LEVEL && curr->func_index == func_index
            && curr->arg.a_long == where) {
            if (prev)
                prev->next = curr->next;
            else
                timer_base = curr->next;
            if (timeout_funcs[curr->func_index].cleanup)
                (*timeout_funcs[curr->func_index].cleanup)(&curr->arg,
                                                           curr->timeout);
            free((genericptr_t) curr);
        } else {
            prev = curr;
        }
    }
}

/*
 * When is the spot timer of type func_index going to expire?
 * Returns 0L if no such timer.
 */
long
spot_time_expires(x, y, func_index)
xchar x, y;
short func_index;
{
    timer_element *curr;
    long where = (((long) x << 16) | ((long) y));

    for (curr = timer_base; curr; curr = curr->next) {
        if (curr->kind == TIMER_LEVEL && curr->func_index == func_index
            && curr->arg.a_long == where)
            return curr->timeout;
    }
    return 0L;
}

long
spot_time_left(x, y, func_index)
xchar x, y;
short func_index;
{
    long expires = spot_time_expires(x, y, func_index);
    return (expires > 0L) ? expires - monstermoves : 0L;
}

/* Insert timer into the global queue */
STATIC_OVL void
insert_timer(gnu)
timer_element *gnu;
{
    timer_element *curr, *prev;

    for (prev = 0, curr = timer_base; curr; prev = curr, curr = curr->next)
        if (curr->timeout >= gnu->timeout)
            break;

    gnu->next = curr;
    if (prev)
        prev->next = gnu;
    else
        timer_base = gnu;
}

STATIC_OVL timer_element *
remove_timer(base, func_index, arg)
timer_element **base;
short func_index;
anything *arg;
{
    timer_element *prev, *curr;

    for (prev = 0, curr = *base; curr; prev = curr, curr = curr->next)
        if (curr->func_index == func_index && curr->arg.a_void == arg->a_void)
            break;

    if (curr) {
        if (prev)
            prev->next = curr->next;
        else
            *base = curr->next;
    }

    return curr;
}

STATIC_OVL void
write_timer(fd, timer)
int fd;
timer_element *timer;
{
    anything arg_save;

    arg_save = zeroany;
    switch (timer->kind) {
    case TIMER_GLOBAL:
    case TIMER_LEVEL:
        /* assume no pointers in arg */
        bwrite(fd, (genericptr_t) timer, sizeof(timer_element));
        break;

    case TIMER_OBJECT:
        if (timer->needs_fixup)
            bwrite(fd, (genericptr_t) timer, sizeof(timer_element));
        else {
            /* replace object pointer with id */
            arg_save.a_obj = timer->arg.a_obj;
            timer->arg = zeroany;
            timer->arg.a_uint = (arg_save.a_obj)->o_id;
            timer->needs_fixup = 1;
            bwrite(fd, (genericptr_t) timer, sizeof(timer_element));
            timer->arg.a_obj = arg_save.a_obj;
            timer->needs_fixup = 0;
        }
        break;

    case TIMER_MONSTER:
        if (timer->needs_fixup)
            bwrite(fd, (genericptr_t) timer, sizeof(timer_element));
        else {
            /* replace monster pointer with id */
            arg_save.a_monst = timer->arg.a_monst;
            timer->arg = zeroany;
            timer->arg.a_uint = (arg_save.a_monst)->m_id;
            timer->needs_fixup = 1;
            bwrite(fd, (genericptr_t) timer, sizeof(timer_element));
            timer->arg.a_monst = arg_save.a_monst;
            timer->needs_fixup = 0;
        }
        break;

    default:
        panic("write_timer");
        break;
    }
}

/*
 * Return TRUE if the object will stay on the level when the level is
 * saved.
 */
boolean
obj_is_local(obj)
struct obj *obj;
{
    switch (obj->where) {
    case OBJ_INVENT:
    case OBJ_MIGRATING:
        return FALSE;
    case OBJ_FLOOR:
    case OBJ_BURIED:
        return TRUE;
    case OBJ_CONTAINED:
        return obj_is_local(obj->ocontainer);
    case OBJ_MINVENT:
        return mon_is_local(obj->ocarry);
    }
    panic("obj_is_local");
    return FALSE;
}

/*
 * Return TRUE if the given monster will stay on the level when the
 * level is saved.
 */
STATIC_OVL boolean
mon_is_local(mon)
struct monst *mon;
{
    struct monst *curr;

    for (curr = migrating_mons; curr; curr = curr->nmon)
        if (curr == mon)
            return FALSE;
    /* `mydogs' is used during level changes, never saved and restored */
    for (curr = mydogs; curr; curr = curr->nmon)
        if (curr == mon)
            return FALSE;
    return TRUE;
}

/*
 * Return TRUE if the timer is attached to something that will stay on the
 * level when the level is saved.
 */
STATIC_OVL boolean
timer_is_local(timer)
timer_element *timer;
{
    switch (timer->kind) {
    case TIMER_LEVEL:
        return TRUE;
    case TIMER_GLOBAL:
        return FALSE;
    case TIMER_OBJECT:
        return obj_is_local(timer->arg.a_obj);
    case TIMER_MONSTER:
        return mon_is_local(timer->arg.a_monst);
    }
    panic("timer_is_local");
    return FALSE;
}

/*
 * Part of the save routine.  Count up the number of timers that would
 * be written.  If write_it is true, actually write the timer.
 */
STATIC_OVL int
maybe_write_timer(fd, range, write_it)
int fd, range;
boolean write_it;
{
    int count = 0;
    timer_element *curr;

    for (curr = timer_base; curr; curr = curr->next) {
        if (range == RANGE_GLOBAL) {
            /* global timers */

            if (!timer_is_local(curr)) {
                count++;
                if (write_it)
                    write_timer(fd, curr);
            }

        } else {
            /* local timers */

            if (timer_is_local(curr)) {
                count++;
                if (write_it)
                    write_timer(fd, curr);
            }
        }
    }

    return count;
}

/*
 * Save part of the timer list.  The parameter 'range' specifies either
 * global or level timers to save.  The timer ID is saved with the global
 * timers.
 *
 * Global range:
 *      + timeouts that follow the hero (global)
 *      + timeouts that follow obj & monst that are migrating
 *
 * Level range:
 *      + timeouts that are level specific (e.g. storms)
 *      + timeouts that stay with the level (obj & monst)
 */
void
save_timers(fd, mode, range)
int fd, mode, range;
{
    timer_element *curr, *prev, *next_timer = 0;
    int count;

    if (perform_bwrite(mode)) {
        if (range == RANGE_GLOBAL)
            bwrite(fd, (genericptr_t) &timer_id, sizeof(timer_id));

        count = maybe_write_timer(fd, range, FALSE);
        bwrite(fd, (genericptr_t) &count, sizeof count);
        (void) maybe_write_timer(fd, range, TRUE);
    }

    if (release_data(mode)) {
        for (prev = 0, curr = timer_base; curr; curr = next_timer) {
            next_timer = curr->next; /* in case curr is removed */

            if (!(!!(range == RANGE_LEVEL) ^ !!timer_is_local(curr))) {
                if (prev)
                    prev->next = curr->next;
                else
                    timer_base = curr->next;
                free((genericptr_t) curr);
                /* prev stays the same */
            } else {
                prev = curr;
            }
        }
    }
}

/*
 * Pull in the structures from disk, but don't recalculate the object and
 * monster pointers.
 */
void
restore_timers(fd, range, ghostly, adjust)
int fd, range;
boolean ghostly; /* restoring from a ghost level */
long adjust;     /* how much to adjust timeout */
{
    int count;
    timer_element *curr;

    if (range == RANGE_GLOBAL)
        mread(fd, (genericptr_t) &timer_id, sizeof timer_id);

    /* restore elements */
    mread(fd, (genericptr_t) &count, sizeof count);
    while (count-- > 0) {
        curr = (timer_element *) alloc(sizeof(timer_element));
        mread(fd, (genericptr_t) curr, sizeof(timer_element));
        if (ghostly)
            curr->timeout += adjust;
        insert_timer(curr);
    }
}

void
reset_timers(VOID_ARGS)
{
    timer_element* curr, * next_timer = 0;

    for (curr = timer_base; curr; curr = next_timer) {
        next_timer = curr->next; /* in case curr is removed */
        free((genericptr_t)curr);
        /* prev stays the same */
    }

    timer_base = 0;
    timer_id = 1;
}

/* to support '#stats' wizard-mode command */
void
timer_stats(hdrfmt, hdrbuf, count, size)
const char *hdrfmt;
char *hdrbuf;
long* count;
size_t *size;
{
    timer_element *te;

    Sprintf(hdrbuf, hdrfmt, (long) sizeof (timer_element));
    *count = *size = 0L;
    for (te = timer_base; te; te = te->next) {
        ++*count;
        *size += sizeof *te;
    }
}

/* reset all timers that are marked for reseting */
void
relink_timers(ghostly)
boolean ghostly;
{
    timer_element *curr;
    unsigned nid;

    for (curr = timer_base; curr; curr = curr->next) {
        if (curr->needs_fixup) {
            if (curr->kind == TIMER_OBJECT) {
                if (ghostly) {
                    if (!lookup_id_mapping(curr->arg.a_uint, &nid))
                    {
                        panic("relink_timers 1");
                        return;
                    }
                } else
                    nid = curr->arg.a_uint;
                curr->arg.a_obj = find_oid(nid);
                if (!curr->arg.a_obj)
                {
                    panic("cant find o_id %d", nid);
                    return;
                }
                curr->needs_fixup = 0;
            } else if (curr->kind == TIMER_MONSTER) {
//                panic("relink_timers: no monster timer implemented");
                if (ghostly) {
                    if (!lookup_id_mapping(curr->arg.a_uint, &nid))
                    {
                        panic("relink_timers 1");
                        return;
                    }
                }
                else
                    nid = curr->arg.a_uint;
                curr->arg.a_monst = find_mid_ew(nid);
                if (!curr->arg.a_monst)
                {
                    panic("cant find m_id %d", nid);
                    return;
                }
                curr->needs_fixup = 0;
            }
            else
            {
                panic("relink_timers 2");
                return;
            }
        }
    }
}

/*
 * Timeout callback for for objects that are making noise.
 */
void
make_sound_object(arg, timeout)
anything* arg;
long timeout;
{
    if (!arg && timeout)
    {
        /* Do nothing */
    }

    return;
}

/*
 * Cleanup an object making sound if timer stopped.
 */
STATIC_OVL void
cleanup_sound(arg, expire_time)
anything* arg;
long expire_time;
{
    struct obj* obj = arg->a_obj;
    if (!obj->makingsound)
    {
        impossible("cleanup_sound: obj %s not making sound", xname(obj));
        return;
    }

    del_sound_source(SOUNDSOURCE_OBJECT, obj_to_any(obj));

    /* restore unused time */
    obj->age += expire_time - monstermoves;

    obj->makingsound = 0;

    if (obj->where == OBJ_INVENT)
        update_inventory();
}

void
property_expiry_message(propidx, was_flying)
int propidx;
boolean was_flying;
{
    switch (propidx)
    {
    case REFLECTING:
        if (!Reflecting)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels less reflecting than before.");
        }
        break;
    case FIRE_IMMUNITY:
        if (!Fire_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to burning than before.");
        }
        break;
    case COLD_IMMUNITY:
        if (!Cold_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to frostbites than before.");
        }
        break;
    case SHOCK_IMMUNITY:
        if (!Shock_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to electricity than before.");
        }
        break;
    case IMPROVED_FIRE_RESISTANCE:
        if (!Improved_fire_resistance && !Fire_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to burning than before.");
        }
        break;
    case IMPROVED_COLD_RESISTANCE:
        if (!Improved_cold_resistance && !Cold_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to frostbites than before.");
        }
        break;
    case IMPROVED_SHOCK_RESISTANCE:
        if (!Improved_shock_resistance && !Shock_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to electricity than before.");
        }
        break;
    case FIRE_RESISTANCE:
        if (!Fire_resistance && !Improved_fire_resistance && !Fire_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to burning than before.");
        }
        break;
    case COLD_RESISTANCE:
        if (!Cold_resistance && !Improved_cold_resistance && !Cold_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to frostbites than before.");
        }
        break;
    case SHOCK_RESISTANCE:
        if (!Shock_resistance && !Improved_shock_resistance && !Shock_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to electricity than before.");
        }
        break;
    case DISINTEGRATION_RESISTANCE:
        if (!Disint_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "body feels less firm than before.");
        }
        break;
    case POISON_RESISTANCE:
        if (!Poison_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less healthy than before.");
        }
        break;
    case ACID_IMMUNITY:
        if (!Acid_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to acid than before.");
        }
        break;
    case IMPROVED_ACID_RESISTANCE:
        if (!Improved_acid_resistance && !Acid_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to acid than before.");
        }
        break;
    case ACID_RESISTANCE:
        if (!Acid_resistance && !Improved_acid_resistance && !Acid_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to acid than before.");
        }
        break;
    case STONE_RESISTANCE:
        if (!Stone_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less limber than before.");
        }
        break;
    case DRAIN_RESISTANCE:
        if (!Drain_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel more suspectible to draining than before.");
        }
        break;
    case SICK_RESISTANCE:
        if (!Sick_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel like you might be catching a cold easier now.");
        }
        break;
    case INVULNERABLE:
        if (!Invulnerable)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels more prone to damage than before.");
        }
        break;
    case ANTIMAGIC:
        if (!Antimagic)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less protected from magic.");
        }
        break;
    case MAGIC_MISSILE_IMMUNITY:
        if (!Magic_missile_immunity)
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less protected from magic missiles.");
        break;
    case IMPROVED_MAGIC_MISSILE_RESISTANCE:
        if (!Improved_magic_missile_resistance && !Magic_missile_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less protected from magic missiles.");
        }
        break;
    case MAGIC_MISSILE_RESISTANCE:
        if (!Magic_missile_resistance && !Improved_magic_missile_resistance && !Magic_missile_immunity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less protected from magic missiles.");
        }
        break;
    case CANCELLED:
        if (!Cancelled)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic is flowing more normally.");
        }
        break;
    case CANCELLATION_RESISTANCE:
        /* Nothing intentionally */
        break;
    case THREE_FOURTHS_MAGIC_RESISTANCE:
        if (!Three_fourths_magic_resistance && !Half_magic_resistance && !One_fourth_magic_resistance && !No_magic_resistance)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is working more properly.");
        }
        break;
    case HALVED_MAGIC_RESISTANCE:
        if (!Half_magic_resistance && !One_fourth_magic_resistance && !No_magic_resistance)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is working more properly.");
        }
        break;
    case ONE_FOURTH_MAGIC_RESISTANCE:
        if (!One_fourth_magic_resistance && !No_magic_resistance)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is working more properly.");
        }
        break;
    case NO_MAGIC_RESISTANCE:
        if (!No_magic_resistance)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel your magic resistance is working more properly.");
        }
        break;
    case FEARFUL:
        if (!Fearful)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "regain your composure.");
        }
        break;
    case SUMMON_FORBIDDEN:
        if (!Summon_forbidden)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel summoning is working properly again.");
        }
        break;
    case CHARMED:
        if (!Charmed_or_controlled)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "own motivations make more sense to you now.");
        }
        break;
    case UNDEAD_CONTROL:
        if (!Charmed_or_controlled)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "are more in control of your own actions.");
        }
        break;
    case DEATH_RESISTANCE:
        if (!Death_resistance)
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "soul's silver cord feels thinner than before.");
        break;
    case CHARM_RESISTANCE:
        if (!Charm_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less certain of your own motivations.");
        }
        break;
    case FEAR_RESISTANCE:
        if (!Fear_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less courageous.");
        }
        break;
    case MIND_SHIELDING:
        if (!Mind_shielding)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "unprotected from mental detection.");
        }
        break;
    case LYCANTHROPY_RESISTANCE:
        if (!Lycanthropy_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel that your immunity to lycanthropy is gone.");
        }
        break;
    case CURSE_RESISTANCE:
        if (!Curse_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less protected from curses.");
        }
        break;
    case LIFESAVED:
        if (!Lifesaved)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel more mortal than before.");
        }
        break;
    case DETECT_MONSTERS:
        if (!Detect_monsters)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less sensitive to the presence of monsters than before.");
        }
        see_monsters();
        break;
    case BLIND_TELEPATHY:
        if (!Blind_telepat)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less telepathic when blind.");
        }
        see_monsters();
        break;
    case TELEPAT:
        if (!Unblind_telepat)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less telepathic.");
        }
        see_monsters();
        break;
    case XRAY_VISION:
        if (!XRay_vision)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "can no longer see through walls.");
        }
        see_monsters();
        break;
    case WATER_WALKING:
        if (!Wwalking)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less able to walk on water.");
        }
        break;
    case MAGICAL_BREATHING:
        if (!EMagical_breathing && !HMagical_breathing)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less able to breathe in water.");
        }
        break;
    case DISPLACED:
        if (!Displaced)
        {
            play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "mirror image vanishes.");
        }
        break;
    case CONFLICT:
        if (!Conflict)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "neighborhood feels less quarrelsome than before.");
        }
        break;
    case MAGICAL_PROTECTION:
        if (!Magical_protection)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less protected than before.");
        }
        break;
    case MAGICAL_SHIELDING:
        if (!Magical_shielding)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less shielded than before.");
        }
        break;
    case MAGICAL_BARKSKIN:
        if (!Magical_barkskin)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels less bark-like than before.");
        }
        break;
    case MAGICAL_STONESKIN:
        if (!Magical_stoneskin)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels less stone-like than before.");
        }
        break;
    case BISECTION_RESISTANCE:
        if (!Bisection_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels less steel-like than before.");
        }
        break;
    case TITAN_STRENGTH:
        if (!Titan_strength)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less strong than before.");
        }
        break;
    case DIVINE_ENDURANCE:
        if (!Divine_endurance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel you have less endurance than before.");
        }
        break;
    case DIVINE_DEXTERITY:
        if (!Divine_dexterity)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less dexterous than before.");
        }
        break;
    case DIVINE_INTELLECT:
        if (!Divine_intellect)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less smart than before.");
        }
        break;
    case DIVINE_WISDOM:
        if (!Divine_wisdom)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel you have less common sense than before.");
        }
        break;
    case DIVINE_CHARISMA:
        if (!Divine_charisma)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "feel less charming than before.");
        }
        break;
    case FAST:
        if (!Lightning_fast && !Super_fast && !Ultra_fast && !Very_fast && !Fast)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less quick than before.");
        }
        break;
    case VERY_FAST:
        if (!Lightning_fast && !Super_fast && !Ultra_fast && !Very_fast)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%sless quick than before.",
                Fast ? "a bit " : "");
        }
        break;
    case ULTRA_FAST:
        if (!Lightning_fast && !Super_fast && !Ultra_fast)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%sless quick than before.",
                Very_fast || Fast ? "a bit " : "");
        }
        break;
    case SUPER_FAST:
        if (!Lightning_fast && !Super_fast)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%sless quick than before.",
                Ultra_fast || Very_fast || Fast ? "a bit " : "");
        }
        break;
    case LIGHTNING_FAST:
        if (!Lightning_fast)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%sless quick than before.",
                Super_fast || Ultra_fast || Very_fast || Fast ? "a bit " : "");
        }
        break;
    case SLOWED:
        if (!Slowed)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "yourself speed up%s.",
                Very_fast || Ultra_fast || Super_fast || Lightning_fast ? " a lot" : Fast ? "" : " a bit");
        }
        break;
    case HEROISM:
        if (!Super_heroism && !Heroism)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "less heroic than before.");
        }
        break;
    case SUPER_HEROISM:
        if (!Super_heroism)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%sless heroic than before.",
                Heroism ? "a bit " : "");
        }
        break;
    case RAPID_REGENERATION:
        if (!Divine_regeneration && !Rapidest_regeneration && !Rapider_regeneration && !Rapid_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are healing %sless rapidly than before.",
                Regeneration ? "" : "much ");
        }
        break;
    case RAPIDER_REGENERATION:
        if (!Divine_regeneration && !Rapidest_regeneration && !Rapider_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are healing %sless rapidly than before.",
                Rapid_regeneration || Regeneration ? "" : "much ");
        }
        break;
    case RAPIDEST_REGENERATION:
        if (!Divine_regeneration && !Rapidest_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are healing %sless rapidly than before.",
                Rapider_regeneration || Rapid_regeneration || Regeneration ? "" : "much ");
        }
        break;
    case DIVINE_REGENERATION:
        if (!Divine_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are healing %sless rapidly than before.",
                Rapidest_regeneration || Rapider_regeneration || Rapid_regeneration || Regeneration ? "" : "much ");
        }
        break;
    case RAPID_ENERGY_REGENERATION:
        if (!Rapidest_energy_regeneration && !Rapider_energy_regeneration && !Rapid_energy_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are gaining mana %sless rapidly than before.",
                Energy_regeneration ? "" : "much ");
        }
        break;
    case RAPIDER_ENERGY_REGENERATION:
        if (!Rapidest_energy_regeneration && !Rapider_energy_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are gaining mana %sless rapidly than before.",
                Rapid_energy_regeneration || Energy_regeneration ? "" : "much ");
        }
        break;
    case RAPIDEST_ENERGY_REGENERATION:
        if (!Rapidest_energy_regeneration)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "you are gaining mana %sless rapidly than before.",
                Rapider_energy_regeneration || Rapid_energy_regeneration || Energy_regeneration ? "" : "much ");
        }
        break;
    case MELEE_LIFE_LEECH:
        if (!Melee_life_leech)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have lost your ability to leech life in melee!");
        }
        break;
    case CRAZED:
        if (!Crazed)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "more sane than before!");
        }
        break;
    case SILENCED:
        if (!Silenced)
        {
            play_sfx_sound(SFX_NEGATIVE_EFFECT_ENDED);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "can speak again!");
        }
        break;

    case INVISIBILITY:
        newsym(u.ux, u.uy);
        if (!Invis && !Blocks_Invisibility && !Blind) {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You(!See_invisible
                ? "are no longer invisible."
                : "can no longer see through yourself.");
            stop_occupation();
        }
        break;
    case SEE_INVISIBLE:
        set_mimic_blocking(); /* do special mimic handling */
        see_monsters();       /* make invis mons appear */
        newsym(u.ux, u.uy);   /* make self appear */
        stop_occupation();
        if (!See_invisible)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are no longer able to see invisibile monsters.");
        }
        break;
    case LEVITATION:
        (void)float_down(I_SPECIAL | TIMEOUT, 0L);
        break;
    case FLYING:
        /* timed Flying is via #wizintrinsic only */
        if (was_flying && !Flying) {
            context.botl = context.botlx = 1;
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "land.");
            spoteffects(TRUE);
        }
        break;
    case WARN_OF_MON:
        /* timed Warn_of_mon is via #wizintrinsic only */
        if (!Warn_of_mon) {
            context.warntype.speciesidx = NON_PM;
            if (context.warntype.species) {
                play_sfx_sound(SFX_PROTECTION_END_WARNING);
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are no longer warned about %s.",
                    makeplural(context.warntype.species->mname));
                context.warntype.species = (struct permonst*)0;
            }
        }
        break;
    case WARN_UNDEAD:
    case WARN_DEMON:
    case WARN_ORC:

    case WARN_TROLL:
    case WARN_GIANT:
    case WARN_DRAGON:

    case WARN_ELF:
    case WARN_DWARF:
    case WARN_GNOLL:

    case WARN_HUMAN:
    case WARN_LYCANTHROPE:
    case WARN_ANGEL:

    case WARN_OGRE:
    case WARN_GNOME:
        break;
    case PASSES_WALLS:
        if (!Passes_walls) {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            if (stuck_in_wall())
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "hemmed in again.");
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You're back to your %s self again.",
                    !Upolyd ? "normal" : "unusual");
        }
        break;
    case SLEEPING:
        if (!Sleeping)
        {
            u.usleep = 0;
            if (Paralyzed_or_immobile)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "wake up but still cannot move!");
            else
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "wake up!");
        }
        break;
    case UNDEAD_IMMOBILITY:
    case PARALYZED:
        if (!Paralyzed_or_immobile)
        {
            if (Sleeping)
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are no longer paralyzed but still asleep!");
            else if (nolimbs(youmonst.data))
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are no longer paralyzed");
            else
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "limbs are moving again!");
        }
        break;
    case SLIME_RESISTANCE:
        if (!Slime_resistance)
        {
            play_sfx_sound(SFX_PROTECTION_END_WARNING);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "skin feels less fiery than before.");
        }
        break;
    default:
        break;
    }
}


/*timeout.c*/

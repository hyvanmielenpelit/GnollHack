/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-05-22 */

/* GnollHack 4.0    pray.c    $NHDT-Date: 1549074257 2019/02/02 02:24:17 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.110 $ */
/* Copyright (c) Benson I. Margulies, Mike Stephenson, Steve Linhart, 1989. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_PTR int NDECL(prayer_done);
STATIC_DCL struct obj *NDECL(worst_cursed_item);
STATIC_DCL int NDECL(in_trouble);
STATIC_DCL void FDECL(fix_worst_trouble, (int));
STATIC_DCL void FDECL(angrygods, (ALIGNTYP_P));
STATIC_DCL void FDECL(at_your_feet, (const char *));
STATIC_DCL void NDECL(gcrownu);
STATIC_DCL void FDECL(pleased, (ALIGNTYP_P));
STATIC_DCL void FDECL(godvoice, (ALIGNTYP_P, const char *));
STATIC_DCL void FDECL(god_zaps_you, (ALIGNTYP_P));
STATIC_DCL void FDECL(fry_by_god, (ALIGNTYP_P, BOOLEAN_P));
STATIC_DCL void FDECL(gods_angry, (ALIGNTYP_P));
STATIC_DCL void FDECL(gods_upset, (ALIGNTYP_P));
STATIC_DCL void FDECL(consume_offering, (struct obj *));
STATIC_DCL boolean FDECL(water_prayer, (BOOLEAN_P));
STATIC_DCL boolean FDECL(symbol_prayer, (BOOLEAN_P));
STATIC_DCL boolean FDECL(altar_prayer, (BOOLEAN_P));
STATIC_DCL boolean FDECL(blocked_boulder, (int, int));

/* simplify a few tests */
#define Cursed_obj(obj, typ) ((obj) && (obj)->otyp == (typ) && (obj)->cursed)

/*
 * Logic behind deities and altars and such:
 * + prayers are made to your god if not on an altar, and to the altar's god
 *   if you are on an altar
 * + If possible, your god answers all prayers, which is why bad things happen
 *   if you try to pray on another god's altar
 * + sacrifices work basically the same way, but the other god may decide to
 *   accept your allegiance, after which they are your god.  If rejected,
 *   your god takes over with your punishment.
 * + if you're in Gehennom, all messages come from Moloch
 */

/*
 *      Moloch, who dwells in Gehennom, is the "renegade" cruel god
 *      responsible for the theft of the Amulet from Marduk, the Creator.
 *      Moloch is unaligned.
 */
STATIC_VAR const char *godvoices[] = {
    "booms out", "thunders", "rings out", "booms",
};

/* values calculated when prayer starts, and used when completed */
STATIC_VAR aligntyp p_aligntyp;
STATIC_VAR int p_trouble;
STATIC_VAR int p_type; /* (-1)-3: (-1)=really naughty, 3=really good */

#define PIOUS 20
#define DEVOUT 14
#define FERVENT 9
#define STRIDENT 4

/*
 * The actual trouble priority is determined by the order of the
 * checks performed in in_trouble() rather than by these numeric
 * values, so keep that code and these values synchronized in
 * order to have the values be meaningful.
 */

#define TROUBLE_MUMMY_ROT 17
#define TROUBLE_FOOD_POISONED 16
#define TROUBLE_CURSED_EYEGLASSES_OF_HALLUCINATION 15
#define TROUBLE_STONED 14
#define TROUBLE_SLIMED 13
#define TROUBLE_STRANGLED 12
#define TROUBLE_LAVA 11
#define TROUBLE_SICK 10
#define TROUBLE_STARVING 9
#define TROUBLE_REGION 8 /* stinking cloud */
#define TROUBLE_HIT 7
#define TROUBLE_LYCANTHROPE 6
#define TROUBLE_COLLAPSING 5
#define TROUBLE_STUCK_IN_WALL 4
#define TROUBLE_CURSED_LEVITATION 3
#define TROUBLE_UNUSEABLE_HANDS 2
#define TROUBLE_CURSED_BLINDFOLD 1

#define TROUBLE_PUNISHED (-1)
#define TROUBLE_FUMBLING (-2)
#define TROUBLE_CURSED_ITEMS (-3)
#define TROUBLE_SADDLE (-4)
#define TROUBLE_BLIND (-5)
#define TROUBLE_POISONED (-6)
#define TROUBLE_WOUNDED_LEGS (-7)
#define TROUBLE_HUNGRY (-8)
#define TROUBLE_STUNNED (-9)
#define TROUBLE_CONFUSED (-10)
#define TROUBLE_HALLUCINATION (-11)
#define TROUBLE_LAUGHING (-12)
#define TROUBLE_TELEPORTITIS (-13)


/* critically low hit points if hp <= 5 or hp <= maxhp/N for some N */
boolean
critically_low_hp(only_if_injured)
boolean only_if_injured; /* determines whether maxhp <= 5 matters */
{
    int divisor, hplim, curhp = Upolyd ? u.mh : u.uhp,
                        maxhp = Upolyd ? u.mhmax : u.uhpmax;

    if (only_if_injured && !(curhp < maxhp))
        return FALSE;
    /* if maxhp is extremely high, use lower threshold for the division test
       (golden glow cuts off at 11+5*lvl, nurse interaction at 25*lvl; this
       ought to use monster hit dice--and a smaller multiplier--rather than
       ulevel when polymorphed, but polyself doesn't maintain that) */
    hplim = 15 * u.ulevel;
    if (maxhp > hplim)
        maxhp = hplim;

    divisor = 3;
#if 0
    /* 7 used to be the unconditional divisor */
    switch (xlev_to_rank(u.ulevel)) { /* maps 1..30 into 0..8 */
    case 0:
    case 1:
        divisor = 5;
        break; /* explvl 1 to 5 */
    case 2:
    case 3:
        divisor = 6;
        break; /* explvl 6 to 13 */
    case 4:
    case 5:
        divisor = 7;
        break; /* explvl 14 to 21 */
    case 6:
    case 7:
        divisor = 8;
        break; /* explvl 22 to 29 */
    default:
        divisor = 9;
        break; /* explvl 30+ */
    }
#endif

    /* 5 is a magic number in TROUBLE_HIT handling below */
    return (boolean) (curhp <= 5 || curhp * divisor <= maxhp);
}

/* return True if surrounded by impassible rock, regardless of the state
   of your own location (for example, inside a doorless closet) */
boolean
stuck_in_wall()
{
    int i, j, x, y, count = 0;

    if (Passes_walls)
        return FALSE;
    for (i = -1; i <= 1; i++) {
        x = u.ux + i;
        for (j = -1; j <= 1; j++) {
            if (!i && !j)
                continue;
            y = u.uy + j;
            if (!isok(x, y)
                || (IS_ROCK(levl[x][y].typ)
                    && (levl[x][y].typ != SDOOR && levl[x][y].typ != SCORR))
                || (blocked_boulder(i, j) && !throws_rocks(youmonst.data)))
                ++count;
        }
    }
    return (count == 8) ? TRUE : FALSE;
}

/*
 * Return 0 if nothing particular seems wrong, positive numbers for
 * serious trouble, and negative numbers for comparative annoyances.
 * This returns the worst problem. There may be others, and the gods
 * may fix more than one.
 *
 * This could get as bizarre as noting surrounding opponents, (or
 * hostile dogs), but that's really hard.
 *
 * We could force rehumanize of polyselfed people, but we can't tell
 * unintentional shape changes from the other kind. Oh well.
 * 3.4.2: make an exception if polymorphed into a form which lacks
 * hands; that's a case where the ramifications override this doubt.
 */
STATIC_OVL int
in_trouble()
{
    struct obj *otmp;
    int i;

    /*
     * major troubles
     */
    if (Stoned)
        return TROUBLE_STONED;
    if (Slimed)
        return TROUBLE_SLIMED;
    if (Strangled)
        return TROUBLE_STRANGLED;
    if (u.utrap && u.utraptype == TT_LAVA)
        return TROUBLE_LAVA;
    if (Sick)
        return TROUBLE_SICK;
    if (FoodPoisoned)
        return TROUBLE_FOOD_POISONED;
    if (MummyRot)
        return TROUBLE_MUMMY_ROT;
    if (u.uhs >= WEAK)
        return TROUBLE_STARVING;
    if (region_danger())
        return TROUBLE_REGION;
    if (critically_low_hp(FALSE))
        return TROUBLE_HIT;
    if (u.ulycn >= LOW_PM)
        return TROUBLE_LYCANTHROPE;
    if (near_capacity() >= EXT_ENCUMBER && AMAX(A_STR) - ABASE(A_STR) > 3)
        return TROUBLE_COLLAPSING;
    if (stuck_in_wall())
        return TROUBLE_STUCK_IN_WALL;

    if (Cursed_obj(umisc, EYEGLASSES_OF_HALLUCINATION)
        || Cursed_obj(umisc2, EYEGLASSES_OF_HALLUCINATION)
        || Cursed_obj(umisc3, EYEGLASSES_OF_HALLUCINATION)
        || Cursed_obj(umisc4, EYEGLASSES_OF_HALLUCINATION)
        || Cursed_obj(umisc5, EYEGLASSES_OF_HALLUCINATION))
        return TROUBLE_CURSED_EYEGLASSES_OF_HALLUCINATION;

    if (Cursed_obj(uarmf, LEVITATION_BOOTS)
        || stuck_ring(uleft, RIN_LEVITATION)
        || stuck_ring(uright, RIN_LEVITATION))
        return TROUBLE_CURSED_LEVITATION;
    if (nohands(youmonst.data) || !freehand()) {
        /* for bag/box access [cf use_container()]...
           make sure it's a case that we know how to handle;
           otherwise "fix all troubles" would get stuck in a loop */
        if (welded(uwep, &youmonst))
            return TROUBLE_UNUSEABLE_HANDS;
        if (Upolyd && nohands(youmonst.data)
            && (!Unchanging || ((otmp = unchanger()) != 0 && otmp->cursed)))
            return TROUBLE_UNUSEABLE_HANDS;
    }
    if (Blindfolded && ublindf->cursed)
        return TROUBLE_CURSED_BLINDFOLD;



    /*
     * minor troubles
     */
    if (Punished || (u.utrap && u.utraptype == TT_BURIEDBALL))
        return TROUBLE_PUNISHED;
    if ((HTeleportation & ~TIMEOUT) && !Teleport_control)
        return TROUBLE_TELEPORTITIS;
    if (Cursed_obj(uarmg, GAUNTLETS_OF_FUMBLING)
        || Cursed_obj(uarmf, FUMBLE_BOOTS))
        return TROUBLE_FUMBLING;
    if (Cursed_obj(uarmu, SHIRT_OF_UNCONTROLLABLE_LAUGHTER))
        return TROUBLE_LAUGHING;
    if (worst_cursed_item())
        return TROUBLE_CURSED_ITEMS;
    if (u.usteed) { /* can't voluntarily dismount from a cursed saddle */
        otmp = which_armor(u.usteed, W_SADDLE);
        if (Cursed_obj(otmp, SADDLE))
            return TROUBLE_SADDLE;
    }

    if (Blinded > 1 && haseyes(youmonst.data)
        && (!u.uswallow
            || !attacktype_fordmg(u.ustuck->data, AT_ENGL, AD_BLND)))
        return TROUBLE_BLIND;
    /* deafness isn't it's own trouble; healing magic cures deafness
       when it cures blindness, so do the same with trouble repair */
    if ((HDeaf & TIMEOUT) > 1L)
        return TROUBLE_BLIND;

    for (i = 0; i < A_MAX; i++)
        if (ABASE(i) < AMAX(i))
            return TROUBLE_POISONED;
    if (Wounded_legs && !u.usteed)
        return TROUBLE_WOUNDED_LEGS;
    if (u.uhs >= HUNGRY)
        return TROUBLE_HUNGRY;
    if (HStun & TIMEOUT)
        return TROUBLE_STUNNED;
    if (HConfusion & TIMEOUT)
        return TROUBLE_CONFUSED;
    if (HHallucination & TIMEOUT)
        return TROUBLE_HALLUCINATION;
    return 0;
}

/* select an item for TROUBLE_CURSED_ITEMS */
STATIC_OVL struct obj *
worst_cursed_item()
{
    register struct obj *otmp;

    /* if strained or worse, check for loadstone first */
    if (near_capacity() >= HVY_ENCUMBER) {
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (Cursed_obj(otmp, LOADSTONE))
                return otmp;
    }
    /* Then jinxstone if carried */
    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (Cursed_obj(otmp, JINXSTONE))
            return otmp;

    /* weapon takes precedence if it is interfering
       with taking off a ring or putting on a shield */
    if (welded(uwep, &youmonst) && (uright || bimanual(uwep))) { /* weapon */
        otmp = uwep;
    /* gloves come next, due to rings */
    } else if (uarmg && uarmg->cursed) { /* gloves */
        otmp = uarmg;
    /* then shield due to two handed weapons and spells */
    } else if (uarms && uarms->cursed) { /* shield */
        otmp = uarms;
    /* then cloak due to body armor */
    } else if (uarmc && uarmc->cursed) { /* cloak */
        otmp = uarmc;
    }
    else if (uarmo && uarmo->cursed) { /* robe */
        otmp = uarmo;
    } else if (uarm && uarm->cursed) { /* suit */
        otmp = uarm;
    /* if worn helmet of opposite alignment is making you an adherent
       of the current god, he/she/it won't uncurse that for you */
    } else if (uarmh && uarmh->cursed /* helmet */
               && uarmh->otyp != HELM_OF_OPPOSITE_ALIGNMENT) {
        otmp = uarmh;
    } else if (uarmf && uarmf->cursed) { /* boots */
        otmp = uarmf;
    }
    else if (uarmb && uarmb->cursed) { /* bracers */
        otmp = uarmb;
    } else if (uarmu && uarmu->cursed) { /* shirt */
        otmp = uarmu;
    } else if (uamul && uamul->cursed) { /* amulet */
        otmp = uamul;
    } else if (umisc && umisc->cursed) { /* miscellaneous */
        otmp = umisc;
    } else if (umisc2 && umisc2->cursed) { /* miscellaneous */
        otmp = umisc2;
    } else if (umisc3 && umisc3->cursed) { /* miscellaneous */
        otmp = umisc3;
    } else if (umisc4 && umisc4->cursed) { /* miscellaneous */
        otmp = umisc4;
    } else if (umisc5 && umisc5->cursed) { /* miscellaneous */
        otmp = umisc5;
    } else if (uleft && uleft->cursed) { /* left ring */
        otmp = uleft;
    } else if (uright && uright->cursed) { /* right ring */
        otmp = uright;
    } else if (ublindf && ublindf->cursed) { /* eyewear */
        otmp = ublindf; /* must be non-blinding lenses */
    /* if weapon wasn't handled above, do it now */
    } else if (welded(uwep, &youmonst)) { /* weapon */
        otmp = uwep;
    }
    else if (welded(uarms, &youmonst)) { /* weapon */
        otmp = uarms;
        /* active secondary weapon even though it isn't welded */
    } else if (uarms && uarms->cursed && u.twoweap) {
        otmp = uarms;
    /* all worn items ought to be handled by now */
    } else {
        for (otmp = invent; otmp; otmp = otmp->nobj) {
            if (!otmp->cursed)
                continue;
            if ((objects[otmp->otyp].oc_flags & O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED)
                || objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED 
                || confers_luck(otmp))
                break;
        }
    }
    return otmp;
}

STATIC_OVL void
fix_worst_trouble(trouble)
int trouble;
{
    int i;
    struct obj *otmp = 0;
    const char *what = (const char *) 0;
    STATIC_VAR NEARDATA const char leftglow[] = "Your left ring softly glows",
                               rightglow[] = "Your right ring softly glows";

    switch (trouble) {
    case TROUBLE_STONED:
        play_sfx_sound(SFX_CURE_AILMENT);
        make_stoned(0L, "You feel more limber.", 0, (char *) 0, 0);
        break;
    case TROUBLE_SLIMED:
        play_sfx_sound(SFX_CURE_AILMENT);
        make_slimed(0L, "The slime disappears.", 0, (char*)0, 0);
        break;
    case TROUBLE_STRANGLED:
        if (uamul && uamul->otyp == AMULET_OF_STRANGULATION)
        {
            play_sfx_sound(SFX_ITEM_VANISHES);
            Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "amulet vanishes!");
            useup(uamul);
        }
        if(Breathless)
            You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "are no longer being strangled.");
        else
            You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "can breathe again.");
        Strangled = 0;
        context.botl = context.botlx = 1;
        break;
    case TROUBLE_LAVA:
        You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are back on solid ground.");
        /* teleport should always succeed, but if not, just untrap them */
        if (!safe_teleds(FALSE, FALSE))
            reset_utrap(TRUE);
        break;
    case TROUBLE_STARVING:
        /* temporarily lost strength recovery now handled by init_uhunger() */
        /*FALLTHRU*/
    case TROUBLE_HUNGRY:
        Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s feels content.", body_part(STOMACH));
        init_uhunger();
        context.botl = context.botlx = 1;
        break;
    case TROUBLE_SICK:
        play_sfx_sound(SFX_CURE_DISEASE);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "better.");
        make_sick(0L, (char *) 0, FALSE, 0);
        break;
    case TROUBLE_FOOD_POISONED:
        play_sfx_sound(SFX_CURE_DISEASE);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "better.");
        make_food_poisoned(0L, (char*)0, FALSE, 0);
        break;
    case TROUBLE_MUMMY_ROT:
        play_sfx_sound(SFX_CURE_DISEASE);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "better.");
        make_mummy_rotted(0L, (char*)0, FALSE, 0);
        break;
    case TROUBLE_REGION:
        /* stinking cloud, with hero vulnerable to HP loss */
        region_safety();
        break;
    case TROUBLE_HIT:
        /* "fix all troubles" will keep trying if hero has
           5 or less hit points, so make sure they're always
           boosted to be more than that */
        play_sfx_sound(SFX_FULL_HEALING);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much better.");
        u.ubasehpdrain = u.basemhdrain = 0;
        if (Upolyd) {
            u.basemhmax += rnd(5);
            if (u.basemhmax <= 5)
                u.basemhmax = 5 + 1;
            updatemaxhp();
            u.mh = u.mhmax;
        }
        if (u.ubasehpmax < u.ulevel * 5 + 11)
            u.ubasehpmax += rnd(5);
        if (u.ubasehpmax <= 5)
            u.ubasehpmax = 5 + 1;
        updatemaxhp();
        u.uhp = u.uhpmax;
        context.botl = 1;
        refresh_u_tile_gui_info(TRUE);
        break;
    case TROUBLE_COLLAPSING:
        play_sfx_sound(SFX_GAIN_ABILITY);
        /* override Fixed_abil; uncurse that if feasible */
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "%sstronger.",
                 (AMAX(A_STR) - ABASE(A_STR) > 6) ? "much " : "");
        ABASE(A_STR) = AMAX(A_STR);
        context.botl = 1;
        if (Fixed_abil) {
            if ((otmp = stuck_ring(uleft, RIN_SUSTAIN_ABILITY)) != 0) {
                if (otmp == uleft)
                    what = leftglow;
            } else if ((otmp = stuck_ring(uright, RIN_SUSTAIN_ABILITY)) != 0) {
                if (otmp == uright)
                    what = rightglow;
            }
            if (otmp)
                goto decurse;
        }
        break;
    case TROUBLE_STUCK_IN_WALL:
        /* no control, but works on no-teleport levels */
        if (safe_teleds(FALSE, FALSE)) {
            Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "surroundings change.");
        } else {
            /* safe_teleds() couldn't find a safe place; perhaps the
               level is completely full.  As a last resort, confer
               intrinsic wall/rock-phazing.  Hero might get stuck
               again fairly soon....
               Without something like this, fix_all_troubles can get
               stuck in an infinite loop trying to fix STUCK_IN_WALL
               and repeatedly failing. */
            set_itimeout(&HPasses_walls, (long) (d(4, 4) + 4)); /* 8..20 */
            refresh_u_tile_gui_info(TRUE);
            /* how else could you move between packed rocks or among
               lattice forming "solid" rock? */
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much slimmer.");
        }
        break;
    case TROUBLE_CURSED_LEVITATION:
        if (Cursed_obj(uarmf, LEVITATION_BOOTS)) {
            otmp = uarmf;
        } else if ((otmp = stuck_ring(uleft, RIN_LEVITATION)) != 0) {
            if (otmp == uleft)
                what = leftglow;
        } else if ((otmp = stuck_ring(uright, RIN_LEVITATION)) != 0) {
            if (otmp == uright)
                what = rightglow;
        }
        goto decurse;
    case TROUBLE_UNUSEABLE_HANDS:
        if (welded(uwep, &youmonst)) {
            otmp = uwep;
            goto decurse;
        }
        if (Upolyd && nohands(youmonst.data)) {
            if (!Unchanging) {
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "shape becomes uncertain.");
                rehumanize(); /* "You return to {normal} form." */
            } else if ((otmp = unchanger()) != 0 && otmp->cursed) {
                /* otmp is an amulet of unchanging */
                goto decurse;
            }
        }
        if (nohands(youmonst.data) || !freehand())
            impossible("fix_worst_trouble: couldn't cure hands.");
        break;
    case TROUBLE_CURSED_BLINDFOLD:
        otmp = ublindf;
        goto decurse;
    case TROUBLE_CURSED_EYEGLASSES_OF_HALLUCINATION:
        otmp = (umisc->otyp == EYEGLASSES_OF_HALLUCINATION ? umisc :
            umisc2->otyp == EYEGLASSES_OF_HALLUCINATION ? umisc2 :
            umisc3->otyp == EYEGLASSES_OF_HALLUCINATION ? umisc3 :
            umisc4->otyp == EYEGLASSES_OF_HALLUCINATION ? umisc4 :
            umisc5->otyp == EYEGLASSES_OF_HALLUCINATION ? umisc5 : (struct obj*)0);
        goto decurse;
    case TROUBLE_LYCANTHROPE:
        play_sfx_sound(SFX_CURE_DISEASE);
        you_unwere(TRUE);
        break;
    /*
     */
    case TROUBLE_PUNISHED:
        play_sfx_sound(SFX_BLESS_ITEM_SUCCESS);
        Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "chain disappears.");
        if (u.utrap && u.utraptype == TT_BURIEDBALL)
            buried_ball_to_freedom();
        else
            unpunish();
        break;
    case TROUBLE_FUMBLING:
        if (Cursed_obj(uarmg, GAUNTLETS_OF_FUMBLING))
            otmp = uarmg;
        else if (Cursed_obj(uarmf, FUMBLE_BOOTS))
            otmp = uarmf;
        goto decurse;
        /*NOTREACHED*/
        break;
    case TROUBLE_LAUGHING:
        if (Cursed_obj(uarmu, SHIRT_OF_UNCONTROLLABLE_LAUGHTER))
            otmp = uarmu;
        goto decurse;
        /*NOTREACHED*/
        break;
    case TROUBLE_TELEPORTITIS:
        play_sfx_sound(SFX_CURE_AILMENT);
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "much more composed.");
        u.uprops[TELEPORT].intrinsic = (u.uprops[TELEPORT].intrinsic & TIMEOUT);
        break;
    case TROUBLE_CURSED_ITEMS:
        otmp = worst_cursed_item();
        if (otmp == uright)
            what = rightglow;
        else if (otmp == uleft)
            what = leftglow;
    decurse:
        if (!otmp) {
            impossible("fix_worst_trouble: nothing to uncurse.");
            return;
        }
        if (!Blind || (otmp == ublindf && Blind_because_of_blindfold_only)) {
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s %s.",
                  what ? what : (const char *) Yobjnam2(otmp, "softly glow"),
                  hcolor(NH_AMBER));
            iflags.last_msg = PLNMSG_OBJ_GLOWS;
            otmp->bknown = !Hallucination;
        }
        play_sfx_sound(SFX_UNCURSE_ITEM_SUCCESS);
        uncurse(otmp);
        update_inventory();
        break;
    case TROUBLE_POISONED:
        /* override Fixed_abil; ignore items which confer that */
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "There's a tiger in your tank.");
        else
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "in good health again.");

        play_sfx_sound(SFX_RESTORE_ABILITY);

        for (i = 0; i < A_MAX; i++) {
            if (ABASE(i) < AMAX(i)) {
                ABASE(i) = AMAX(i);
                context.botl = 1;
            }
        }
        (void) encumber_msg();
        break;
    case TROUBLE_BLIND: { /* handles deafness as well as blindness */
        char msgbuf[BUFSZ];
        const char *eyes = body_part(EYE);
        boolean cure_deaf = (HDeaf & TIMEOUT) ? TRUE : FALSE;

        msgbuf[0] = '\0';
        if (Blinded) {
            if (eyecount(youmonst.data) != 1)
                eyes = makeplural(eyes);
            Sprintf(msgbuf, "Your %s %s better", eyes, vtense(eyes, "feel"));
            u.ucreamed = 0;
            play_sfx_sound(SFX_CURE_AILMENT);
            make_blinded(0L, FALSE);
        }
        if (cure_deaf) {
            make_deaf(0L, FALSE);
            if (!Deaf)
            {
                Sprintf(eos(msgbuf), "%s can hear again",
                        !*msgbuf ? "You" : " and you");
            }
        }
        if (*msgbuf)
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s.", msgbuf);
        break;
    }
    case TROUBLE_WOUNDED_LEGS:
        play_sfx_sound(SFX_CURE_AILMENT);
        heal_legs(0);
        break;
    case TROUBLE_STUNNED:
        play_sfx_sound(SFX_CURE_AILMENT);
        make_stunned(0L, TRUE);
        break;
    case TROUBLE_CONFUSED:
        play_sfx_sound(SFX_CURE_AILMENT);
        make_confused(0L, TRUE);
        break;
    case TROUBLE_HALLUCINATION:
        play_sfx_sound(SFX_CURE_AILMENT);
        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Looks like you are back in Kansas.");
        (void) make_hallucinated(0L, FALSE, 0L);
        break;
    case TROUBLE_SADDLE:
        play_sfx_sound(SFX_UNCURSE_ITEM_SUCCESS);
        otmp = which_armor(u.usteed, W_SADDLE);
        if (!Blind) {
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s %s.", Yobjnam2(otmp, "softly glow"), hcolor(NH_AMBER));
            otmp->bknown = TRUE;
        }
        uncurse(otmp);
        break;
    }
}

/* "I am sometimes shocked by... the nuns who never take a bath without
 * wearing a bathrobe all the time.  When asked why, since no man can see them,
 * they reply 'Oh, but you forget the good God'.  Apparently they conceive of
 * the Deity as a Peeping Tom, whose omnipotence enables Him to see through
 * bathroom walls, but who is foiled by bathrobes." --Bertrand Russell, 1943
 * Divine wrath, dungeon walls, and armor follow the same principle.
 */
STATIC_OVL void
god_zaps_you(resp_god)
aligntyp resp_god;
{
    if (u.uswallow) {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE,
          "Suddenly a bolt of lightning comes down at you from the heavens!");
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "It strikes %s!", mon_nam(u.ustuck));
        if (!is_mon_immune_to_elec(u.ustuck)) {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s fries to a crisp!", Monnam(u.ustuck));
            /* Yup, you get experience.  It takes guts to successfully
             * pull off this trick on your god, anyway.
             * Other credit/blame applies (luck or alignment adjustments),
             * but not direct kill count (pacifist conduct).
             */
            xkilled(u.ustuck, XKILL_NOMSG | XKILL_NOCONDUCT);
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems unaffected.", Monnam(u.ustuck));
        }
    }
    else 
    {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Suddenly, a bolt of lightning strikes you!");
        if (Reflecting) {
            play_sfx_sound(SFX_GENERAL_REFLECTS);
            u_shieldeff();
            if (Blind)
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "For some reason you're unaffected.");
            else
                (void) ureflects("%s reflects from your %s.", "It");
        } else if (Shock_immunity) {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            u_shieldeff();
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "It seems not to affect you.");
        } else
            fry_by_god(resp_god, FALSE);
    }

    pline("%s is not deterred...", align_gname(resp_god));
    if (u.uswallow) {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A wide-angle disintegration beam aimed at you hits %s!",
              mon_nam(u.ustuck));
        if (!resists_disint(u.ustuck)) {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s disintegrates into a pile of dust!", Monnam(u.ustuck));
            xkilled(u.ustuck, XKILL_NOMSG | XKILL_NOCORPSE | XKILL_NOCONDUCT);
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems unaffected.", Monnam(u.ustuck));
        }
    } else {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A wide-angle disintegration beam hits you!");

        /* disintegrate shield and body armor before disintegrating
         * the impudent mortal, like black dragon breath -3.
         */
        if (uarms && uarms->oclass == ARMOR_CLASS && uarms->owornmask & W_ARMS && !(EReflecting & W_ARMS)
            && !(EDisint_resistance & W_ARMS))
            (void) destroy_arm(uarms);
        if (uarmc && !(EReflecting & W_ARMC)
            && !(EDisint_resistance & W_ARMC))
            (void) destroy_arm(uarmc);
        if (uarmo && !(EReflecting & W_ARMO)
            && !(EDisint_resistance & W_ARMO) && !uarmo)
            (void)destroy_arm(uarmo);
        if (uarm && !(EReflecting & W_ARM) && !(EDisint_resistance & W_ARM)
            && !uarmc && !uarmo)
            (void) destroy_arm(uarm);
        if (uarmu && !uarm && !uarmo && !uarmc)
            (void) destroy_arm(uarmu);
        if (!Disint_resistance) {
            fry_by_god(resp_god, TRUE);
        } else {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "bask in its %s glow for a minute...", NH_BLACK);
            play_voice_god_simple_line_by_align(resp_god, GOD_LINE_I_BELIEVE_IT_NOT);
            godvoice(resp_god, "I believe it not!");
        }
        if (Is_astralevel(&u.uz) || Is_sanctum(&u.uz)) {
            /* one more try for high altars */
            play_voice_god_simple_line_by_align(resp_god, GOD_LINE_THOU_CANNOT_ESCAPE_MY_WRATH_MORTAL);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou cannot escape my wrath, mortal!");
            summon_minion(resp_god, FALSE);
            summon_minion(resp_god, FALSE);
            summon_minion(resp_god, FALSE);
            play_voice_god_simple_line_by_align(resp_god, flags.female ? GOD_LINE_DESTROY_HER_MY_SERVANTS : GOD_LINE_DESTROY_HIM_MY_SERVANTS);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Destroy %s, my servants!", uhim());
        }
    }
}

STATIC_OVL void
fry_by_god(resp_god, via_disintegration)
aligntyp resp_god;
boolean via_disintegration;
{
    if (via_disintegration)
        play_sfx_sound(SFX_DISINTEGRATE);

    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s!", !via_disintegration ? "fry to a crisp"
                                   : "disintegrate into a pile of dust");
    killer.format = KILLED_BY;
    Sprintf(killer.name, "the wrath of %s", align_gname(resp_god));
    done(DIED);
}

STATIC_OVL void
angrygods(resp_god)
aligntyp resp_god;
{
    int maxanger;

    if (Inhell)
        resp_god = A_NONE;
    u.ublessed = 0;

    /* changed from tmp = u.ugangr + abs (u.uluck) -- rph */
    /* added test for alignment diff -dlc */
    if (resp_god != u.ualign.type)
        maxanger = u.ualign.record / 2 + (Luck > 0 ? -Luck / 3 : -Luck);
    else
        maxanger = 3 * u.ugangr + ((Luck > 0 || u.ualign.record >= STRIDENT)
                                   ? -Luck / 3
                                   : -Luck);
    if (maxanger < 1)
        maxanger = 1; /* possible if bad align & good luck */
    else if (maxanger > 15)
        maxanger = 15; /* be reasonable */

    switch (rn2(maxanger)) {
    case 0:
    case 1:
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "that %s is %s.", align_gname(resp_god),
                 Hallucination ? "bummed" : "displeased");
        break;
    case 2:
    case 3:
        godvoice(resp_god, (char *) 0);
        play_voice_god_simple_line_by_align(resp_god,
            youmonst.data->mlet == S_HUMAN ? (ugod_is_angry() && resp_god == u.ualign.type ? GOD_LINE_THOU_HAST_STRAYED_FROM_THE_PATH_MORTAL : GOD_LINE_THOU_ART_ARROGANT_MORTAL) : 
            (ugod_is_angry() && resp_god == u.ualign.type ? GOD_LINE_THOU_HAST_STRAYED_FROM_THE_PATH_CREATURE : GOD_LINE_THOU_ART_ARROGANT_CREATURE));
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou %s, %s.",
              (ugod_is_angry() && resp_god == u.ualign.type)
                  ? "hast strayed from the path"
                  : "art arrogant",
              youmonst.data->mlet == S_HUMAN ? "mortal" : "creature");

        play_voice_god_simple_line_by_align(resp_god, GOD_LINE_THOU_MUST_RELEARN_THY_LESSONS);
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou must relearn thy lessons!");
        (void)adjattrib(A_WIS, -1, FALSE);
        losexp((char*)0);
        break;
    case 6:
        if (!Punished) {
            gods_angry(resp_god);
            punish((struct obj *) 0);
            break;
        } /* else fall thru */
    case 4:
    case 5:
        gods_angry(resp_god);
        if (!Blind && !Antimagic)
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s glow surrounds you.", An(hcolor(NH_BLACK)));
        rndcurse();
        break;
    case 9:
    case 7:
        if (resp_god == u.ualignbase[A_ORIGINAL] && u.ualignbase[A_ORIGINAL] != u.ualign.type)
        {
            godvoice(resp_god, (char*)0);
            play_voice_god_simple_line_by_align(resp_god,
                youmonst.data->mlet == S_HUMAN ? GOD_LINE_THOU_HAST_STRAYED_FROM_THE_PATH_MORTAL :
                (GOD_LINE_THOU_HAST_STRAYED_FROM_THE_PATH_CREATURE));

            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou hast strayed from the path, %s.",
                youmonst.data->mlet == S_HUMAN ? "mortal" : "creature");

            summon_minion(resp_god, FALSE);

            play_voice_god_simple_line_by_align(resp_god, GOD_LINE_THOU_SHALT_PAY_FOR_THINE_INDISCRETION);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou shalt pay for thine indiscretion!");
            break;
        }
        /* else fall thru */
    case 8:
        godvoice(resp_god, (char *) 0);
        play_voice_god_simple_line_by_align(resp_god, 
            (on_altar() && (a_align(u.ux, u.uy) != resp_god)) ? GOD_LINE_THOU_DURST_SCORN_ME : GOD_LINE_THOU_DURST_CALL_UPON_ME);

        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou durst %s me?",
                  (on_altar() && (a_align(u.ux, u.uy) != resp_god))
                      ? "scorn"
                      : "call upon");
        play_voice_god_simple_line_by_align(resp_god, youmonst.data->mlet == S_HUMAN ? GOD_LINE_THEN_DIE_MORTAL : GOD_LINE_THEN_DIE_CREATURE);
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Then die, %s!",
            youmonst.data->mlet == S_HUMAN ? "mortal" : "creature");
        summon_minion(resp_god, FALSE);
        break;

    default:
        gods_angry(resp_god);
        god_zaps_you(resp_god);
        break;
    }
    u.uprayer_timeout = Role_if(PM_PRIEST) ? rnz(150) : rnz(300);
    return;
}

/* helper to print "str appears at your feet", or appropriate */
STATIC_OVL void
at_your_feet(str)
const char *str;
{
    if (Blind)
        str = Something;
    if (u.uswallow) {
        /* barrier between you and the floor */
        pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s %s into %s %s.", str, vtense(str, "drop"),
              s_suffix(mon_nam(u.ustuck)), mbodypart(u.ustuck, STOMACH));
    } else {
        pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s %s %s your %s!", str,
              Blind ? "lands" : vtense(str, "appear"),
              Levitation ? "beneath" : "at", makeplural(body_part(FOOT)));
    }
}

STATIC_OVL void
gcrownu()
{
    struct obj *obj, *obj2;
    boolean already_exists, in_hand, in_hand2;
    short class_gift;
    struct monst* steed_gift = 0;
    int sp_no;
#define ok_wep(o) ((o) && ((o)->oclass == WEAPON_CLASS || is_weptool(o)))

    HSee_invisible |= FROM_ACQUIRED;
    HFire_resistance |= FROM_ACQUIRED;
    HCold_resistance |= FROM_ACQUIRED;
    HShock_resistance |= FROM_ACQUIRED;
    HDeath_resistance |= FROM_ACQUIRED;
    HLycanthropy_resistance |= FROM_ACQUIRED;
    HSleep_resistance |= FROM_ACQUIRED;
    HPoison_resistance |= FROM_ACQUIRED;
    HFear_resistance |= FROM_ACQUIRED;

    play_sfx_sound(SFX_PRAY_CROWNING);

    godvoice(u.ualign.type, (char *) 0);

    obj = ok_wep(uwep) ? uwep : 0;
    obj2 = ok_wep(uarms) ? uarms : 0;
    already_exists = in_hand = in_hand2 = FALSE; /* lint suppression */
    boolean katana_already_exists = exist_artifact(KATANA, artiname(ART_KATANA_OF_MASAMUNE));
    boolean lance_already_exists = exist_artifact(LANCE, artiname(ART_RHONGOMYNIAD));
    boolean grail_already_exists = exist_artifact(GRAIL_OF_HEALING, artiname(ART_HOLY_GRAIL));
    boolean gauntlets_already_exists = exist_artifact(GAUNTLETS_OF_BALANCE, artiname(ART_GAUNTLETS_OF_YIN_AND_YANG));
    boolean monkgauntlets = (Role_if(PM_MONK) && !gauntlets_already_exists);
    boolean usegnollchaoticgift = (Race_if(PM_GNOLL) && !exist_artifact(FLAIL, artiname(ART_HOWLING_FLAIL)));
    short chaotic_crowning_gift_oartifact = usegnollchaoticgift ? ART_HOWLING_FLAIL : exist_artifact(RUNESWORD, artiname(ART_STORMBRINGER)) ? ART_MOURNBLADE : ART_STORMBRINGER;
    int chaotic_crowning_gift_baseitem = usegnollchaoticgift ? RUNED_FLAIL : RUNESWORD;
    enum p_skills chaotic_crowning_gift_skill = usegnollchaoticgift ? P_FLAIL : P_SWORD;

    switch (u.ualign.type) {
    case A_LAWFUL:
        u.uevent.uhand_of_elbereth = 1;
        in_hand = (uwep && uwep->oartifact == ART_KATANA_OF_MASAMUNE);
        in_hand2 = (uarms && uarms->oartifact == ART_KATANA_OF_MASAMUNE);
        play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_I_CROWN_THEE_THE_HAND_OF_ELBERETH);
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "I crown thee...  The Hand of Elbereth!");
        livelog_printf(LL_DIVINEGIFT,
            "was crowned \"The Hand of Elbereth\" by %s", u_gname());
        break;
    case A_NEUTRAL:
        u.uevent.uhand_of_elbereth = 2;
        in_hand = (uwep && uwep->oartifact == ART_VORPAL_BLADE);
        in_hand2 = (uarms && uarms->oartifact == ART_VORPAL_BLADE);
        already_exists = exist_artifact(LONG_SWORD, artiname(ART_VORPAL_BLADE));
        play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_THOU_SHALT_BE_MY_ENVOY_OF_BALANCE);
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou shalt be my Envoy of Balance!");
        livelog_printf(LL_DIVINEGIFT, "became %s Envoy of Balance",
            s_suffix(u_gname()));
        break;
    case A_CHAOTIC:
        u.uevent.uhand_of_elbereth = 3;
        in_hand = (uwep && uwep->oartifact == chaotic_crowning_gift_oartifact);
        in_hand2 = (uarms && uarms->oartifact == chaotic_crowning_gift_oartifact);
        already_exists = exist_artifact(chaotic_crowning_gift_baseitem, artiname(chaotic_crowning_gift_oartifact));
        if (Role_if(PM_WIZARD) || Role_if(PM_PRIEST) || Role_if(PM_MONK))
        {
            play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_I_CROWN_THEE_THE_GLORY_OF_ARIOCH);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "I crown thee... The Glory of Arioch!");
            livelog_printf(LL_DIVINEGIFT, "%s", "became The Glory of Arioch");
        }
        else
        {
            boolean takelives = ((already_exists && !in_hand && !in_hand2) || chaotic_crowning_gift_oartifact == ART_HOWLING_FLAIL);
            play_voice_god_simple_line_by_align(u.ualign.type, takelives ? GOD_LINE_THOU_ART_CHOSEN_TO_TAKE_LIVES_FOR_MY_GLORY : GOD_LINE_THOU_ART_CHOSEN_TO_STEAL_SOULS_FOR_MY_GLORY);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou art chosen to %s for My Glory!",
                takelives ? "take lives" : "steal souls");
            livelog_printf(LL_DIVINEGIFT, "was chosen to %s for the Glory of %s", takelives ? "take lives" : "steal souls",
                u_gname());
        }
        break;
    }

    if (monkgauntlets)
    {
        class_gift = GAUNTLETS_OF_BALANCE;

        obj = mksobj(class_gift, FALSE, FALSE, FALSE);
        obj = oname(obj, artiname(ART_GAUNTLETS_OF_YIN_AND_YANG));
        obj->enchantment = 1;
        at_your_feet("A pair of gauntlets");
        dropy(obj);
        u.ugifts++;
        obj->aknown = obj->nknown = 1;
        if (obj && obj->oartifact == ART_GAUNTLETS_OF_YIN_AND_YANG)
        {
            discover_artifact(ART_GAUNTLETS_OF_YIN_AND_YANG);
            livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                "was bestowed with %s",
                artiname(ART_GAUNTLETS_OF_YIN_AND_YANG));
        }
    }
    else if (Role_if(PM_WIZARD))
    {
        class_gift = GOLDEN_CHEST;

        obj = mksobj(class_gift, FALSE, FALSE, FALSE);
        obj->olocked = FALSE;
        obj->otrapped = FALSE;

        /* Contents */
        struct obj* otmp = (struct obj*)0;

        otmp = mksobj(ROBE_OF_THE_ARCHMAGI, FALSE, FALSE, FALSE);
        bless(otmp);
        otmp->enchantment = 1 + rnd(3);
        otmp->oerodeproof = 1;
        (void)add_to_container(obj, otmp);

        otmp = mksobj(STAFF_OF_THE_MAGI, FALSE, FALSE, FALSE);
        bless(otmp);
        otmp->enchantment = 1 + rnd(3);
        otmp->oerodeproof = 1;
        (void)add_to_container(obj, otmp);

        otmp = mksobj(MAGIC_MARKER, TRUE, FALSE, TRUE);
        bless(otmp);
        (void)add_to_container(obj, otmp);

        if (!already_learnt_spell_type(SPE_GREATER_MAGIC_MISSILE))
        {
            otmp = mksobj(SPE_GREATER_MAGIC_MISSILE, TRUE, FALSE, TRUE);
            bless(otmp);
            (void)add_to_container(obj, otmp);
        }

        if (!already_learnt_spell_type(SPE_WISH))
        {
            otmp = mksobj(SPE_WISH, TRUE, FALSE, TRUE);
            bless(otmp);
            (void)add_to_container(obj, otmp);
        }

        int randomspell = 0;
        switch (rnd(3))
        {
        case 1:
            randomspell = SPE_POWER_WORD_KILL;
            if(already_learnt_spell_type(randomspell))
                randomspell = SPE_BLACK_BLADE_OF_DISASTER;
            else
                break;

            if (already_learnt_spell_type(randomspell))
                randomspell = SPE_TIME_STOP;
            else
                break;

            if (already_learnt_spell_type(randomspell))
                randomspell = 0;

            break;
        case 2:
            randomspell = SPE_BLACK_BLADE_OF_DISASTER;
            if (already_learnt_spell_type(randomspell))
                randomspell = SPE_TIME_STOP;
            else
                break;

            if (already_learnt_spell_type(randomspell))
                randomspell = SPE_POWER_WORD_KILL;
            else
                break;

            if (already_learnt_spell_type(randomspell))
                randomspell = 0;

            break;
        case 3:
            randomspell = SPE_TIME_STOP;
            if (already_learnt_spell_type(randomspell))
                randomspell = SPE_BLACK_BLADE_OF_DISASTER;
            else
                break;

            if (already_learnt_spell_type(randomspell))
                randomspell = SPE_POWER_WORD_KILL;
            else
                break;

            if (already_learnt_spell_type(randomspell))
                randomspell = 0;

            break;
        default:
            break;
        }
        if (randomspell && !already_learnt_spell_type(randomspell))
        {
            otmp = mksobj(randomspell, TRUE, FALSE, TRUE);
            bless(otmp);
            (void)add_to_container(obj, otmp);
        }
        livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
            "was bestowed with %s", an(actualoname(obj)));
        at_your_feet("A golden chest");
        dropy(obj);
        u.ugifts++;
    }
    else if (Role_if(PM_PRIEST))
    {
        class_gift = GOLDEN_CHEST;

        obj = mksobj(class_gift, FALSE, FALSE, FALSE);
        obj->olocked = FALSE;
        obj->otrapped = FALSE;

        /* Contents */
        if(u.ualign.type == A_LAWFUL)
        {
            struct obj* otmp = (struct obj*)0;

            otmp = mksobj(GOWN_OF_THE_ARCHBISHOPS, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = 1 + rnd(3);
            otmp->oerodeproof = 1;
            (void)add_to_container(obj, otmp);

            otmp = mksobj(MACE, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = 1 + rnd(3);
            otmp->mythic_suffix = MYTHIC_SUFFIX_BANISHMENT;
            otmp->oerodeproof = 1;
            otmp->owt = weight(otmp);
            (void)add_to_container(obj, otmp);

            if (!already_learnt_spell_type(SPE_OBLITERATE))
            {
                otmp = mksobj(SPE_OBLITERATE, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

            if (!already_learnt_spell_type(SPE_HOLY_WORD))
            {
                otmp = mksobj(SPE_HOLY_WORD, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

            int randomspell = 0;
            switch (rnd(3))
            {
            case 1:
                randomspell = SPE_SUMMON_GOLD_DRAGON;
                if (already_learnt_spell_type(randomspell))
                    randomspell = SPE_DIVINE_MOUNT;
                else
                    break;

                if (already_learnt_spell_type(randomspell))
                    randomspell = SPE_HEAVENLY_ARMY;
                else
                    break;

                if (already_learnt_spell_type(randomspell))
                    randomspell = 0;

                break;
            case 2:
                randomspell = SPE_DIVINE_MOUNT;
                if (already_learnt_spell_type(randomspell))
                    randomspell = SPE_HEAVENLY_ARMY;
                else
                    break;

                if (already_learnt_spell_type(randomspell))
                    randomspell = SPE_SUMMON_GOLD_DRAGON;
                else
                    break;

                if (already_learnt_spell_type(randomspell))
                    randomspell = 0;

                break;
            case 3:
                randomspell = SPE_HEAVENLY_ARMY;
                if (already_learnt_spell_type(randomspell))
                    randomspell = SPE_DIVINE_MOUNT;
                else
                    break;

                if (already_learnt_spell_type(randomspell))
                    randomspell = SPE_SUMMON_GOLD_DRAGON;
                else
                    break;

                if (already_learnt_spell_type(randomspell))
                    randomspell = 0;

                break;
            default:
                break;
            }
            if (randomspell && !already_learnt_spell_type(randomspell))
            {
                otmp = mksobj(randomspell, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

        }
        else if (u.ualign.type == A_CHAOTIC)
        {
            struct obj* otmp = (struct obj*)0;

            otmp = mksobj(ROBE_OF_MAGIC_RESISTANCE, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = 1 + rnd(3);
            otmp->oerodeproof = 1;
            (void)add_to_container(obj, otmp);

            otmp = mksobj(MACE, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = 1 + rnd(3);
            otmp->mythic_prefix = MYTHIC_PREFIX_WITCH_KINGS;
            otmp->oerodeproof = 1;
            (void)add_to_container(obj, otmp);

            otmp = mksobj(RIN_THE_SERPENT_GOD, TRUE, TRUE, FALSE);
            bless(otmp);
            (void)add_to_container(obj, otmp);

            if (!already_learnt_spell_type(SPE_FINGER_OF_DEATH))
            {
                otmp = mksobj(SPE_FINGER_OF_DEATH, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

            if (!already_learnt_spell_type(SPE_CREATE_DRACOLICH))
            {
                otmp = mksobj(SPE_CREATE_DRACOLICH, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }
            else if (!already_learnt_spell_type(SPE_CREATE_ELDER_DRACOLICH))
            {
                otmp = mksobj(SPE_CREATE_ELDER_DRACOLICH, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

            if (!already_learnt_spell_type(SPE_GREATER_UNDEATH_REPLENISHMENT))
            {
                otmp = mksobj(SPE_GREATER_UNDEATH_REPLENISHMENT, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

        }
        else
        {
            struct obj* otmp = (struct obj*)0;

            otmp = mksobj(BRACERS_OF_REFLECTION, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = rnd(3);
            otmp->oerodeproof = 1;
            (void)add_to_container(obj, otmp);

            otmp = mksobj(ROBE_OF_MAGIC_RESISTANCE, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = 1 + rnd(3);
            otmp->oerodeproof = 1;
            (void)add_to_container(obj, otmp);

            otmp = mksobj(STAFF_OF_LIFE, FALSE, FALSE, FALSE);
            bless(otmp);
            otmp->enchantment = 1 + rnd(3);
            otmp->oerodeproof = 1;
            (void)add_to_container(obj, otmp);

            if (!already_learnt_spell_type(SPE_GAZE_OF_PETRIFICATION))
            {
                otmp = mksobj(SPE_GAZE_OF_PETRIFICATION, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

            if (!already_learnt_spell_type(SPE_STICK_TO_BOA))
            {
                otmp = mksobj(SPE_STICK_TO_BOA, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

            if (!already_learnt_spell_type(SPE_SUMMON_ELDER_TREANT))
            {
                otmp = mksobj(SPE_SUMMON_ELDER_TREANT, TRUE, FALSE, TRUE);
                bless(otmp);
                (void)add_to_container(obj, otmp);
            }

        }
        livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
            "was bestowed with %s", an(actualoname(obj)));
        at_your_feet("A golden chest");
        dropy(obj);
        u.ugifts++;
    }
    else
    {
        class_gift = STRANGE_OBJECT;
        /* 3.3.[01] had this in the A_NEUTRAL case below,
           preventing chaotic wizards from receiving a spellbook */
        if (Role_if(PM_WIZARD)
            && (!uwep || (uwep->oartifact != ART_VORPAL_BLADE
                          && uwep->oartifact != chaotic_crowning_gift_oartifact))
            && (!uarms || (uarms->oartifact != ART_VORPAL_BLADE
                && uarms->oartifact != chaotic_crowning_gift_oartifact))
            && !carrying(SPE_POWER_WORD_KILL)
            && !already_learnt_spell_type(SPE_POWER_WORD_KILL)
            ) {
            class_gift = SPE_POWER_WORD_KILL;
        make_splbk:
            obj = mksobj(class_gift, TRUE, FALSE, FALSE);
            bless(obj);
            obj->bknown = TRUE;
            livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT | LL_SPOILER,
                "was bestowed with %s", an(actualoname(obj)));
            at_your_feet("A spellbook");
            dropy(obj);
            u.ugifts++;
            /* when getting a new book for known spell, enhance
               currently wielded weapon rather than the book */
            for (sp_no = 0; sp_no < MAXSPELL; sp_no++)
                if (spl_book[sp_no].sp_id == class_gift) {
                    if (ok_wep(uwep))
                        obj = uwep; /* to be blessed,&c */
                    break;
                }
        } else if (Role_if(PM_MONK) && (!uwep || !uwep->oartifact)
                   && !carrying(BELT_OF_STORM_GIANT_STRENGTH)) {
            /* monks rarely wield a weapon */
            class_gift = BELT_OF_STORM_GIANT_STRENGTH;
            goto make_splbk;
        }

        switch (u.ualign.type) {
        case A_LAWFUL:
            if (class_gift != STRANGE_OBJECT) {
                ; /* already got bonus above */
            }
            else if (Role_if(PM_KNIGHT) && !lance_already_exists)
            {
                class_gift = LANCE;
                obj = mksobj(LANCE, FALSE, FALSE, FALSE);
                obj = oname(obj, artiname(ART_RHONGOMYNIAD));
                obj->enchantment = 1;
                at_your_feet("A lance");
                dropy(obj);
                u.ugifts++;
                if (obj->oartifact == ART_RHONGOMYNIAD)
                {
                    obj->aknown = obj->nknown = TRUE;
                    discover_artifact(ART_RHONGOMYNIAD);
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT | LL_SPOILER,
                        "was bestowed with %s",
                        artiname(ART_RHONGOMYNIAD));
                }
            }
            else if ((Role_if(PM_KNIGHT) || Role_if(PM_ARCHAEOLOGIST)) && !grail_already_exists)
            {
                class_gift = GRAIL_OF_HEALING;
                obj = mksobj(GRAIL_OF_HEALING, FALSE, FALSE, FALSE);
                obj = oname(obj, artiname(ART_HOLY_GRAIL));
                at_your_feet("A grail");
                dropy(obj);
                u.ugifts++;
                if (obj->oartifact == ART_HOLY_GRAIL)
                {
                    obj->aknown = obj->nknown = TRUE;
                    discover_artifact(ART_HOLY_GRAIL);
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                        "was bestowed with %s",
                        artiname(ART_HOLY_GRAIL));
                }
            }
            else if (obj && in_hand)
            {
                Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s shines white for a while!", xname(obj));
                obj->enchantment = max(1, obj->enchantment + rnd(3));
                obj->dknown = obj->aknown = obj->nknown = TRUE;
            }
            else if (obj2 && in_hand2)
            {
                Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s shines white for a while!", xname(obj2));
                obj2->enchantment = max(1, obj2->enchantment + rnd(3));
                obj2->dknown = obj2->aknown = obj2->nknown = TRUE;
            }
            else if (!katana_already_exists)
            {
                class_gift = KATANA;
                obj = mksobj(KATANA, FALSE, FALSE, FALSE);
                obj = oname(obj, artiname(ART_KATANA_OF_MASAMUNE));
                obj->enchantment = 1;
                at_your_feet("A katana");
                dropy(obj);
                u.ugifts++;
                if (obj->oartifact == ART_KATANA_OF_MASAMUNE)
                {
                    obj->aknown = obj->nknown = TRUE;
                    discover_artifact(ART_KATANA_OF_MASAMUNE);
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT | LL_SPOILER,
                        "was bestowed with %s",
                        artiname(ART_KATANA_OF_MASAMUNE));
                }
            }
            else if (obj && objects[obj->otyp].oc_subtyp == WEP_LONG_SWORD && get_object_base_value(obj) < 2000L && !obj->oartifact)
            {
                if (!Blind)
                    Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "sword shines brightly for a moment.");
                obj->otyp = LONG_SWORD;
                obj->material = objects[obj->otyp].oc_material;
                obj = oname(obj, artiname(ART_EXCALIBUR));
                if (obj && obj->oartifact == ART_EXCALIBUR)
                {
                    u.ugifts++;
                    obj->aknown = obj->nknown = TRUE;
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                        "was bestowed with %s",
                        artiname(ART_EXCALIBUR));
                }
            }
            /* acquire Excalibur's skill regardless of weapon or gift */
            else if (obj2 && objects[obj2->otyp].oc_subtyp == WEP_LONG_SWORD && get_object_base_value(obj2) < 2000L && !obj2->oartifact)
            {
                if (!Blind)
                    Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "sword shines brightly for a moment.");
                obj2->otyp = LONG_SWORD;
                obj2->material = objects[obj2->otyp].oc_material;
                obj2 = oname(obj2, artiname(ART_EXCALIBUR));
                if (obj2 && obj2->oartifact == ART_EXCALIBUR)
                {
                    u.ugifts++;
                    obj2->aknown = obj2->nknown = TRUE;
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT | LL_SPOILER,
                        "was bestowed with %s",
                        artiname(ART_EXCALIBUR));
                }
            }
            else
            {
                obj = mksobj(SWORD_OF_HOLY_VENGEANCE, FALSE, FALSE, FALSE);
                obj->enchantment = 1 + rnd(3);
                at_your_feet("A sword");
                dropy(obj);
                u.ugifts++;
                obj->aknown = obj->nknown = TRUE;
                livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT | LL_SPOILER,
                    "was bestowed with %s", an(actualoname(obj)));
            }

            /* acquire Excalibur's skill regardless of weapon or gift */
            unrestrict_weapon_skill(P_SWORD);
            if ((obj && obj->oartifact == ART_EXCALIBUR) || (obj2 && obj2->oartifact == ART_EXCALIBUR))
                discover_artifact(ART_EXCALIBUR);

            if (Role_if(PM_KNIGHT))
            {
                /* acquire Rhongomyniad's skill regardless of weapon or gift */
                unrestrict_weapon_skill(P_THRUSTING_WEAPON);

                /* Summon a ki-rin as a steed, too */
                steed_gift = summoncreature(STRANGE_OBJECT, PM_KI_RIN, "%s appears in a puff of smoke.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_SADDLED,
                    SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
            }
            break;
        case A_NEUTRAL:
            if (class_gift != STRANGE_OBJECT)
            {
                ; /* already got bonus above */
            }
            else if (Role_if(PM_ARCHAEOLOGIST) && !grail_already_exists)
            {
                class_gift = GRAIL_OF_HEALING;
                obj = mksobj(GRAIL_OF_HEALING, FALSE, FALSE, FALSE);
                obj = oname(obj, artiname(ART_HOLY_GRAIL));
                at_your_feet("A grail");
                dropy(obj);
                u.ugifts++;
                if (obj->oartifact == ART_HOLY_GRAIL)
                {
                    obj->aknown = obj->nknown = TRUE;
                    discover_artifact(ART_HOLY_GRAIL);
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                        "was bestowed with %s",
                        artiname(ART_HOLY_GRAIL));
                }
            }
            else if (obj && in_hand)
            {
                Your("%s goes snicker-snack!", xname(obj));
                obj->enchantment = max(1, obj->enchantment + rnd(3));
                obj->dknown = obj->aknown = obj->nknown = TRUE;
            } 
            else if (obj2 && in_hand2)
            {
                Your("%s goes snicker-snack!", xname(obj2));
                obj2->enchantment = max(1, obj2->enchantment + rnd(3));
                obj2->dknown = obj2->aknown = obj2->nknown = TRUE;
            }
            else if (!already_exists)
            {
                obj = mksobj(LONG_SWORD, FALSE, FALSE, FALSE);
                obj = oname(obj, artiname(ART_VORPAL_BLADE));
                obj->enchantment = 1;
                at_your_feet("A sword");
                dropy(obj);
                u.ugifts++;
                obj->aknown = obj->nknown = TRUE;
                if (obj->oartifact == ART_VORPAL_BLADE)
                {
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                        "was bestowed with %s",
                        artiname(ART_VORPAL_BLADE));
                }
            }
            /* acquire Vorpal Blade's skill regardless of weapon or gift */
            unrestrict_weapon_skill(P_SWORD);
            if ((obj && obj->oartifact == ART_VORPAL_BLADE) || (obj2 && obj2->oartifact == ART_VORPAL_BLADE))
                discover_artifact(ART_VORPAL_BLADE);

            if (Role_if(PM_KNIGHT))
            {
                /* Summon a roc as a steed, too */
                steed_gift = summoncreature(STRANGE_OBJECT, PM_ROC, "%s appears in a puff of smoke.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_SADDLED,
                    SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
            }
            break;
        case A_CHAOTIC: {
            char swordbuf[BUFSZ];

            if(chaotic_crowning_gift_oartifact == ART_STORMBRINGER || chaotic_crowning_gift_oartifact == ART_MOURNBLADE)
                Sprintf(swordbuf, "%s sword", hcolor(NH_BLACK));
            else if (chaotic_crowning_gift_oartifact == ART_HOWLING_FLAIL)
                Sprintf(swordbuf, "runed flail");
            else
                Sprintf(swordbuf, "item");

            if (class_gift != STRANGE_OBJECT) 
            {
                ; /* already got bonus above */
            }
            else if (obj && in_hand) 
            {
                Your("%s hums ominously!", swordbuf);
                obj->enchantment = max(1, obj->enchantment + rnd(3));
                obj->dknown = obj->aknown = obj->nknown = TRUE;
            }
            else if (obj2 && in_hand2)
            {
                Your("%s hums ominously!", swordbuf);
                obj2->enchantment = max(1, obj2->enchantment + rnd(3));
                obj2->dknown = obj2->aknown = obj2->nknown = TRUE;
            }
            else if (!already_exists)
            {
                obj = mksobj(chaotic_crowning_gift_baseitem, FALSE, FALSE, FALSE);
                obj = oname(obj, artiname(chaotic_crowning_gift_oartifact));
                obj->enchantment = 1;
                at_your_feet(An(swordbuf));
                dropy(obj);
                u.ugifts++;
                obj->aknown = obj->nknown = TRUE;
                if (obj->oartifact == chaotic_crowning_gift_oartifact)
                {
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                        "was bestowed with %s",
                        artiname(chaotic_crowning_gift_oartifact));
                }
            }
            /* Acquire two-weapon combat for dual-wielding Stormbringer and Mournblade  */
            if (chaotic_crowning_gift_oartifact == ART_STORMBRINGER || chaotic_crowning_gift_oartifact == ART_MOURNBLADE)
                unrestrict_weapon_skill(P_DUAL_WEAPON_COMBAT);
            /* acquire weapon skill regardless of weapon or gift */
            unrestrict_weapon_skill(chaotic_crowning_gift_skill);
            if (obj && obj->oartifact == chaotic_crowning_gift_oartifact)
                discover_artifact(chaotic_crowning_gift_oartifact);

            if (Role_if(PM_KNIGHT))
            {
                /* A chaotic knight gets a gorgon as a steed */
                steed_gift = summoncreature(STRANGE_OBJECT, PM_GORGON, "%s appears in a puff of smoke.", MM_SUMMON_IN_SMOKE_ANIMATION | MM_SADDLED,
                    SUMMONCREATURE_FLAGS_CAPITALIZE | SUMMONCREATURE_FLAGS_MARK_AS_SUMMONED | SUMMONCREATURE_FLAGS_DISREGARDS_STRENGTH | SUMMONCREATURE_FLAGS_DISREGARDS_HEALTH);
            }
            break;
        }
        default:
            obj = 0; /* lint */
            break;
        }
    }

    /* enhance weapon regardless of alignment or artifact status */
    if (ok_wep(obj) || in_hand)
    {
        bless(obj);
        obj->oeroded = obj->oeroded2 = 0;
        obj->oerodeproof = TRUE;
        obj->bknown = obj->rknown = obj->nknown = obj->aknown = TRUE;
        if (obj->enchantment < 1)
            obj->enchantment = 1;
        /* acquire skill in this weapon */
        unrestrict_weapon_skill(weapon_skill_type(obj));
    }
    else if (ok_wep(obj2) || in_hand2)
    {
        bless(obj2);
        obj2->oeroded = obj2->oeroded2 = 0;
        obj2->oerodeproof = TRUE;
        obj2->bknown = obj2->rknown = TRUE;
        if (obj2->enchantment < 1)
            obj2->enchantment = 1;
        /* acquire skill in this weapon */
        unrestrict_weapon_skill(weapon_skill_type(obj2));
    }
    else if (class_gift == STRANGE_OBJECT && steed_gift == 0)
    {
        /* opportunity knocked, but there was nobody home... */
        You_feel_ex(ATR_NONE, CLR_MSG_MYSTICAL, "unworthy.");
    }
    update_inventory();

    /* lastly, confer an extra skill slot/credit beyond the
       up-to-29 you can get from gaining experience levels */
    add_weapon_skill(1);

    if (u.uevent.uhand_of_elbereth > 0)
    {
        const char* hofe = hofe_titles[u.uevent.uhand_of_elbereth - 1];
        if (!strncmpi(hofe, "the ", 4))
            hofe += 4;

        u.uachieve.crowned = 1;
        achievement_gained(hofe);
    }
    return;
}

STATIC_OVL void
pleased(g_align)
aligntyp g_align;
{
    /* don't use p_trouble, worst trouble may get fixed while praying */
    int trouble = in_trouble(); /* what's your worst difficulty? */
    int pat_on_head = 0, kick_on_butt;

    play_sfx_sound(SFX_PRAY_PLEASED);
    You_feel_ex(ATR_NONE, CLR_MSG_SUCCESS, "that %s is %s.", align_gname(g_align),
             (u.ualign.record >= DEVOUT)
                 ? Hallucination ? "pleased as punch" : "well-pleased"
                 : (u.ualign.record >= STRIDENT)
                       ? Hallucination ? "ticklish" : "pleased"
                       : Hallucination ? "full" : "satisfied");

    /* not your deity */
    if (on_altar() && p_aligntyp != u.ualign.type)
    {
        adjalign(-1);
        return;
    } 
    else if (u.ualign.record < 2 && trouble <= 0)
        adjalign(1);
    
    if (on_altar() && p_aligntyp == u.ualign.type)
    {
        struct obj* otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj)
        {
            if (is_obj_special_praying_item(otmp) && !otmp->blessed)
            {
                play_sfx_sound(SFX_AURA_GLOW);
                if(!Blind)
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s with %s aura.", Yobjnam2(otmp, "softly glow"), an(hcolor(NH_LIGHT_BLUE)));
                bless(otmp);
                otmp->bknown = 1;
                update_inventory();
                break;
            }
        }
    }

    /*
     * Depending on your luck & align level, the god you prayed to will:
     *  - fix your worst problem if it's major;
     *  - fix all your major problems;
     *  - fix your worst problem if it's minor;
     *  - fix all of your problems;
     *  - do you a gratuitous favor.
     *
     * If you make it to the last category, you roll randomly again
     * to see what they do for you.
     *
     * If your luck is at least 0, then you are guaranteed rescued from
     * your worst major problem.
     */
    if (!trouble && u.ualign.record >= DEVOUT) 
    {
        /* if hero was in trouble, but got better, no special favor */
        if (p_trouble == 0)
            pat_on_head = 1;
    } 
    else 
    {
        int action, prayer_luck;
        int tryct = 0;

        /* Negative luck is normally impossible here (can_pray() forces
           prayer failure in that situation), but it's possible for
           Luck to drop during the period of prayer occupation and
           become negative by the time we get here.  [Reported case
           was lawful character whose stinking cloud caused a delayed
           killing of a peaceful human, triggering the "murderer"
           penalty while successful prayer was in progress.  It could
           also happen due to inconvenient timing on Friday 13th, but
           the magnitude there (-1) isn't big enough to cause trouble.]
           We don't bother remembering start-of-prayer luck, just make
           sure it's at least -1 so that Luck+2 is big enough to avoid
           a divide by zero crash when generating a random number.  */
        prayer_luck = max(Luck, -1); /* => (prayer_luck + 2 > 0) */
        action = rn1(prayer_luck + (on_altar() ? 3 + on_shrine() : 2), 1);
        if (!on_altar())
            action = min(action, 3);
        if (u.ualign.record < STRIDENT)
            action = (u.ualign.record > 0 || !rnl(2)) ? 1 : 0;

        switch (min(action, 5)) 
        {
        case 5:
            pat_on_head = 1;
            /*FALLTHRU*/
        case 4:
            play_sfx_sound(SFX_PRAY_FIX_ALL_TROUBLES);
            do
                fix_worst_trouble(trouble);
            while ((trouble = in_trouble()) != 0);
            break;
        case 3:
            if (trouble > 0)
                fix_worst_trouble(trouble);
            /* FALLTHRU */
        case 2:
            /* arbitrary number of tries */
            play_sfx_sound(SFX_PRAY_FIX_ALL_TROUBLES);
            while ((trouble = in_trouble()) > 0 && (++tryct < 10))
                fix_worst_trouble(trouble);
            break;

        case 1:
            play_sfx_sound(SFX_PRAY_FIX_ONE_TROUBLE);
            if (trouble > 0)
                fix_worst_trouble(trouble);
        case 0:
            break; /* your god blows you off, too bad */
        }
    }

    /* note: can't get pat_on_head unless all troubles have just been
       fixed or there were no troubles to begin with; hallucination
       won't be in effect so special handling for it is superfluous */
    if (pat_on_head)
        switch (rn2((Luck + 6) >> 1)) 
        {
        case 0:
            break;
        case 1:
            if (uwep && (welded(uwep, &youmonst) || uwep->oclass == WEAPON_CLASS
                         || is_weptool(uwep)))
            {
                char repair_buf[BUFSZ];
                play_sfx_sound(SFX_PRAY_REPAIR);

                *repair_buf = '\0';
                if (uwep->oeroded || uwep->oeroded2)
                    Sprintf(repair_buf, " and %s now as good as new",
                            otense(uwep, "are"));

                if (uwep->cursed) 
                {
                    if (!Blind)
                    {
                        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s %s%s.", Yobjnam2(uwep, "softly glow"), hcolor(NH_AMBER), repair_buf);
                        iflags.last_msg = PLNMSG_OBJ_GLOWS;
                    }
                    else
                        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "the power of %s over %s.", u_gname(), yname(uwep));
                    uncurse(uwep);
                    uwep->bknown = TRUE;
                    *repair_buf = '\0';
                } 
                else if (!uwep->blessed) 
                {
                    if (!Blind)
                    {
                        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s with %s aura%s.", Yobjnam2(uwep, "softly glow"), an(hcolor(NH_LIGHT_BLUE)), repair_buf);
                        iflags.last_msg = PLNMSG_OBJ_GLOWS;
                    }
                    else
                        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "the blessing of %s over %s.", u_gname(), yname(uwep));
                    bless(uwep);
                    uwep->bknown = TRUE;
                    *repair_buf = '\0';
                }

                /* fix any rust/burn/rot damage, but don't protect
                   against future damage */
                if (uwep->oeroded || uwep->oeroded2)
                {
                    uwep->oeroded = uwep->oeroded2 = 0;
                    /* only give this message if we didn't just bless
                       or uncurse (which has already given a message) */
                    if (*repair_buf)
                        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s as good as new!",
                              Yobjnam2(uwep, Blind ? "feel" : "look"));
                }
                update_inventory();
            }
            break;
        case 3:
            /* takes 2 hints to get the music to enter the stronghold;
               skip if you've solved it via mastermind or destroyed the
               drawbridge (both set uopened_dbridge) or if you've already
               travelled past the Valley of the Dead (gehennom_entered) */
            if (!u.uevent.uopened_dbridge && !u.uevent.gehennom_entered) 
            {
                if (u.uevent.uheard_tune < 1) 
                {
                    godvoice(g_align, (char *) 0);
                    play_voice_god_simple_line_by_align(g_align, youmonst.data->mlet == S_HUMAN ? GOD_LINE_HARK_MORTAL : GOD_LINE_HARK_CREATURE);
                    verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Hark, %s!", youmonst.data->mlet == S_HUMAN
                                               ? "mortal"
                                               : "creature");
                    play_voice_god_simple_line_by_align(g_align, GOD_LINE_TO_ENTER_THE_CASTLE_THOU_MUST_PLAY_THE_RIGHT_TUNE);
                    verbalize_ex(ATR_NONE, CLR_MSG_GOD,
                       "To enter the castle, thou must play the right tune!");
                    u.uevent.uheard_tune++;
                    break;
                } 
                else if (u.uevent.uheard_tune < 2)
                {
                    You_hear_ex(ATR_NONE, CLR_MSG_MYSTICAL, "a divine music...");
                    pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "It sounds like:  \"%s\".", tune);
                    u.uevent.uheard_tune++;
                    break;
                }
            }
            /*FALLTHRU*/
        case 2:
            play_sfx_sound(SFX_PRAY_HEALING);
            if (!Blind)
                You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are surrounded by %s glow.", an(hcolor(NH_GOLDEN)));
            /* if any levels have been lost (and not yet regained),
               treat this effect like blessed full healing */
            if (u.ulevel < u.ulevelmax) 
            {
                u.ulevelmax -= 1; /* see potion.c */
                pluslvl(FALSE);
            }
            else 
            {
                u.ubasehpmax += 5;
                if (Upolyd)
                    u.basemhmax += 5;
            }
            updatemaxhp();
            u.uhp = u.uhpmax;
            if (Upolyd)
                u.mh = u.mhmax;
            if (ABASE(A_STR) < AMAX(A_STR)) {
                ABASE(A_STR) = AMAX(A_STR);
                context.botl = 1; /* before potential message */
                (void) encumber_msg();
            }
            if (u.uhunger < 900)
                init_uhunger();
            /* luck couldn't have been negative at start of prayer because
               the prayer would have failed, but might have been decremented
               due to a timed event (delayed death of peaceful monster hit
               by hero-created stinking cloud) during the praying interval */
            if (u.uluck < 0)
                u.uluck = 0;
            /* superfluous; if hero was blinded we'd be handling trouble
               rather than issuing a pat-on-head */
            u.ucreamed = 0;
            make_blinded(0L, TRUE);
            context.botl = context.botlx = 1;
            refresh_u_tile_gui_info(TRUE);
            break;
        case 4: 
        {
            register struct obj *otmp;
            int any = 0;

            play_sfx_sound(SFX_PRAY_UNCURSE);
            if (Blind)
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "the power of %s.", u_gname());
            else
                You_ex(ATR_NONE, CLR_MSG_POSITIVE, "are surrounded by %s aura.", an(hcolor(NH_LIGHT_BLUE)));
            for (otmp = invent; otmp; otmp = otmp->nobj) 
            {
                if (otmp->cursed
                    && (otmp != uarmh /* [see worst_cursed_item()] */
                        || uarmh->otyp != HELM_OF_OPPOSITE_ALIGNMENT)) 
                {
                    if (!Blind) 
                    {
                        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s %s.", Yobjnam2(otmp, "softly glow"),
                              hcolor(NH_AMBER));
                        iflags.last_msg = PLNMSG_OBJ_GLOWS;
                        otmp->bknown = TRUE;
                        ++any;
                    }
                    uncurse(otmp);
                }
            }
            if (any)
                update_inventory();
            break;
        }
        case 5: 
        {
            STATIC_VAR NEARDATA const char msg[] =
                "\"and thus I grant thee the gift of %s!\"";

            play_sfx_sound(SFX_PRAY_GIFT);
            play_voice_god_simple_line_by_align(g_align, GOD_LINE_THOU_HAST_PLEASED_ME_WITH_THY_PROGRESS);
            godvoice(u.ualign.type,
                     "Thou hast pleased me with thy progress,");
            if (!(HTelepat & INTRINSIC)) 
            {
                HTelepat |= FROM_ACQUIRED;
                play_voice_god_simple_line_by_align(g_align, GOD_LINE_AND_THUS_I_GRANT_THEE_THE_GIFT_OF_TELEPATHY);
                pline_ex(ATR_NONE, CLR_MSG_GOD, msg, "Telepathy");
                if (Blind)
                    see_monsters();
            } 
            else if (!(HFast & INTRINSIC)) 
            {
                HFast |= FROM_ACQUIRED;
                play_voice_god_simple_line_by_align(g_align, GOD_LINE_AND_THUS_I_GRANT_THEE_THE_GIFT_OF_SPEED);
                pline_ex(ATR_NONE, CLR_MSG_GOD, msg, "Speed");
            } 
            else if (!(HStealth & INTRINSIC)) 
            {
                HStealth |= FROM_ACQUIRED;
                play_voice_god_simple_line_by_align(g_align, GOD_LINE_AND_THUS_I_GRANT_THEE_THE_GIFT_OF_STEALTH);
                pline_ex(ATR_NONE, CLR_MSG_GOD, msg, "Stealth");
            }
            else if (u.ublessed < 9)
            {
                if (!u.ublessed)
                    u.ublessed = rnd(3);
                else
                    u.ublessed++;
                play_voice_god_simple_line_by_align(g_align, GOD_LINE_AND_THUS_I_GRANT_THEE_THE_GIFT_OF_MY_PROTECTION);
                pline_ex(ATR_NONE, CLR_MSG_GOD, msg, "my protection");
            }
            else
            {
                goto crown_here;
            }
            play_voice_god_simple_line_by_align(g_align, GOD_LINE_USE_IT_WISELY_IN_MY_NAME);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Use it wisely in my name!");
            break;
        }
        case 7:
        case 8:
crown_here:
            if (u.ualign.record >= PIOUS && !u.uevent.uhand_of_elbereth) 
            {
                gcrownu();
                break;
            }
            /*FALLTHRU*/
        case 6: 
        {
            struct obj *otmp;
            int sp_no, trycnt = u.ulevel + 1;

            /* not yet known spells given preference over already known ones
             */
            /* Also, try to grant a spell for which there is a skill slot */
            otmp = mkobj(SPBOOK_CLASS, TRUE, TRUE);
            while (--trycnt > 0) 
            {
                if (otmp->otyp != SPE_BLANK_PAPER) 
                {
                    for (sp_no = 0; sp_no < MAXSPELL; sp_no++)
                        if (spl_book[sp_no].sp_id == otmp->otyp)
                            break;
                    if (sp_no == MAXSPELL
                        && !P_RESTRICTED(spell_skilltype(otmp->otyp)))
                        break; /* usable, but not yet known */
                } 
                else 
                {
                    if (!objects[SPE_BLANK_PAPER].oc_name_known
                        || carrying(MAGIC_MARKER))
                        break;
                }
                otmp->otyp = rnd_class(bases[SPBOOK_CLASS], SPE_BLANK_PAPER);
            }
            bless(otmp);
            play_sfx_sound(SFX_PRAY_SPELLBOOK);
            at_your_feet("A spellbook");
            place_object(otmp, u.ux, u.uy);
            newsym(u.ux, u.uy);
            break;
        }
        default:
            impossible("Confused deity!");
            break;
        }

    u.uprayer_timeout = Role_if(PM_PRIEST) ? rnz(175) : rnz(350);

    kick_on_butt = u.uevent.ukilled_wizard ? 1 : 0;
    if (u.uevent.uhand_of_elbereth)
        kick_on_butt++;
    if (kick_on_butt)
        u.uprayer_timeout += kick_on_butt * (Role_if(PM_PRIEST) ? rnz(500) : rnz(1000));

    find_ac();
    find_mc();
    context.botl = 1;

    return;
}

/* either blesses or curses water on the altar,
 * returns true if it found any water here.
 */
STATIC_OVL boolean
water_prayer(bless_water)
boolean bless_water;
{
    register struct obj *otmp;
    register long changed = 0;
    boolean other = FALSE, bc_known = !(Blind || Hallucination);

    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere) 
    {
        /* turn water into (un)holy water */
        if (otmp->otyp == POT_WATER
            && (bless_water ? !otmp->blessed : !otmp->cursed))
        {
            otmp->blessed = bless_water;
            otmp->cursed = !bless_water;
            otmp->bknown = bc_known;
            changed += otmp->quan;
        } 
        else if (otmp->oclass == POTION_CLASS)
            other = TRUE;
    }

    if (changed)
    {
        play_sfx_sound(bless_water ? SFX_PRAY_BLESS_WATER : SFX_PRAY_CURSE_WATER);

        if (!Blind)
            pline_ex(ATR_NONE, bless_water ? CLR_MSG_POSITIVE : NO_COLOR, "%s potion%s on the altar glow%s %s for a moment.",
              ((other && changed > 1L) ? "Some of the"
                                       : (other ? "One of the" : "The")),
              ((other || changed > 1L) ? "s" : ""), (changed > 1L ? "" : "s"),
              (bless_water ? hcolor(NH_LIGHT_BLUE) : hcolor(NH_BLACK)));
    }
    return (boolean) (changed > 0L);
}

/* either blesses or curses holy symbols and prayer stones on the altar, and either charges a holy symbol or strips charges to zero
 * returns true if it found any holy�symbols here.
 */
STATIC_OVL boolean
symbol_prayer(bless_stuff)
boolean bless_stuff;
{
    register struct obj* otmp;
    register long changed = 0;
    boolean other = FALSE, bc_known = !(Blind || Hallucination);
    char buf[BUFSZ];
    strcpy(buf, "");
    boolean use_items = FALSE;

    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
    {
        /* make holy symbols and prayerstones blessed or cursed first */
        if (is_obj_special_praying_item(otmp))
        {
            boolean something_happened = FALSE;

            if(bless_stuff ? !otmp->blessed : !otmp->cursed)
            {
                something_happened = TRUE;
                otmp->blessed = bless_stuff;
                otmp->cursed = !bless_stuff;
                otmp->bknown = bc_known;
            }

            if (objects[otmp->otyp].oc_charged > 0)
            {
                int lim = get_obj_max_charge(otmp);
                if (!bless_stuff)
                {
                    something_happened = TRUE;
                    strip_charges(otmp, FALSE, FALSE);
                }
                else if (bless_stuff && otmp->charges < lim)
                {
                    something_happened = TRUE;
                    otmp->charges += rn1(10, 10);
                    if (otmp->charges > lim)
                        otmp->charges = lim;
                }
            }

            if(something_happened)
                changed += otmp->quan;
            else
                other = TRUE;

            if (!strcmp(buf, ""))
                strcpy(buf, cxname_singular(otmp));
            else if (strcmp(buf, cxname_singular(otmp)))
                use_items = TRUE;
        }
    }

    if (changed)
    {
        play_sfx_sound(bless_stuff ? SFX_PRAY_BLESS_WATER : SFX_PRAY_CURSE_WATER);

        if(!Blind)
            pline_ex(ATR_NONE, bless_stuff ? CLR_MSG_POSITIVE : NO_COLOR, "%s %s%s on the altar glow%s %s for a moment.",
                ((other && changed > 1L) ? "Some of the"
                    : (other ? "One of the" : "The")),
                use_items || !strcmp(buf, "") ? "religious item" : buf,
                ((other || changed > 1L) ? "s" : ""), (changed > 1L ? "" : "s"),
                (bless_stuff ? hcolor(NH_LIGHT_BLUE) : hcolor(NH_BLACK)));

        update_inventory();
    }

    return (boolean)(changed > 0L);
}

STATIC_OVL boolean
altar_prayer(bless_stuff)
boolean bless_stuff;
{
    boolean water = water_prayer(bless_stuff);
    boolean symbol = symbol_prayer(bless_stuff);
    return (water || symbol);
}

STATIC_OVL void
godvoice(g_align, words)
aligntyp g_align;
const char *words;
{
    const char *quot = "";

    if (words)
        quot = "\"";
    else
        words = "";

    pline_The_ex(ATR_NONE, CLR_MSG_GOD, "voice of %s %s: %s%s%s", align_gname(g_align),
              godvoices[rn2(SIZE(godvoices))], quot, words, quot);
}

STATIC_OVL void
gods_angry(g_align)
aligntyp g_align;
{
    play_voice_god_simple_line_by_align(g_align, GOD_LINE_THOU_HAST_ANGERED_ME);
    godvoice(g_align, "Thou hast angered me.");
}

/* The g_align god is upset with you. */
STATIC_OVL void
gods_upset(g_align)
aligntyp g_align;
{
    if (g_align == u.ualign.type)
        u.ugangr++;
    else if (u.ugangr)
        u.ugangr--;
    angrygods(g_align);
}

STATIC_OVL void
consume_offering(otmp)
register struct obj *otmp;
{
    if (Hallucination)
    {
        switch (rn2(3))
        {
        case 0:
            play_sfx_sound(SFX_ALTAR_OFFERING_VANISH);
            Your_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "sacrifice sprouts wings and a propeller and roars away!");
            break;
        case 1:
            play_sfx_sound(SFX_ALTAR_OFFERING_VANISH);
            Your_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "sacrifice puffs up, swelling bigger and bigger, and pops!");
            break;
        case 2:
            play_sfx_sound(SFX_ALTAR_OFFERING_VANISH);
            Your_ex(ATR_NONE, CLR_MSG_HALLUCINATED,
     "sacrifice collapses into a cloud of dancing particles and fades away!");
            break;
        }
    }
    else if (Blind && u.ualign.type == A_LAWFUL)
    {
        play_sfx_sound(SFX_ALTAR_OFFERING_VANISH);
        Your_ex(ATR_NONE, CLR_MSG_MYSTICAL, "sacrifice disappears!");
    }
    else
    {
        play_sfx_sound(SFX_ALTAR_BURST_OF_FLAME);
        Your_ex(ATR_NONE, CLR_MSG_MYSTICAL, "sacrifice is consumed in a %s!",
            u.ualign.type == A_LAWFUL ? "flash of light" : "burst of flame");
    }
    if (carried(otmp))
        useup(otmp);
    else
        useupf(otmp, 1L);
    exercise(A_WIS, TRUE);
}

int
dosacrifice()
{
    STATIC_VAR NEARDATA const char cloud_of_smoke[] =
        "A cloud of %s smoke surrounds you...";
    register struct obj* otmp;
    int value = 0, mndx;
    boolean highaltar, molochaltar, godlessaltar;
    aligntyp altaralign = a_align(u.ux, u.uy);
    int altarsubtyp = levl[u.ux][u.uy].subtyp;
    int luck_change = 0;

    if (!on_altar() || u.uswallow) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are not standing on an altar.");
        return 0;
    }

    highaltar = ((Is_astralevel(&u.uz) || Is_sanctum(&u.uz))
                 && (levl[u.ux][u.uy].altarmask & AM_SHRINE));

    molochaltar = (altarsubtyp == ALTAR_SUBTYPE_MOLOCH);
    godlessaltar = (altaralign == A_NONE && altarsubtyp == ALTAR_SUBTYPE_NORMAL);

    otmp = floorfood("sacrifice", 1);
    if (!otmp)
        return 0;

    /*
     * Was based on nutritional value and aging behavior (< 50 moves).
     * Sacrificing a food ration got you max luck instantly, making the
     * gods as easy to please as an angry dog!
     *
     * Now only accepts corpses, based on the game's evaluation of their
     * toughness.  Human and pet sacrifice, as well as sacrificing unicorns
     * of your alignment, is strongly discouraged.
     */
#define MAXVALUE 24 /* Highest corpse value (besides Wiz) */

    if (otmp->otyp == CORPSE) 
    {
        register struct permonst *ptr = &mons[otmp->corpsenm];
        struct monst *mtmp;

        /* KMH, conduct */
        if (!u.uconduct.gnostic++)
            livelog_printf(LL_CONDUCT,
                "rejected atheism by offering %s on an altar of %s",
                The(xname(otmp)), a_gname());

        /* you're handling this corpse, even if it was killed upon the altar
         */
        feel_cockatrice(otmp, TRUE);
        if (rider_corpse_revival(otmp, FALSE))
            return 1;

        if (otmp->corpsenm == PM_ACID_BLOB
            || (monstermoves <= peek_at_iced_corpse_age(otmp) + 50)) 
        {
            value = mons[otmp->corpsenm].difficulty + 1;
            if (otmp->oeaten)
                value = eaten_stat(value, otmp);
        }

        if (your_race(ptr)) 
        {
            if (is_demon(youmonst.data)) 
            {
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "find the idea very satisfying.");
                exercise(A_WIS, TRUE);
            }
            else if (u.ualign.type != A_CHAOTIC) 
            {
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "You'll regret this infamous offense!");
                exercise(A_WIS, FALSE);
            }

            if (highaltar && (altaralign != A_CHAOTIC || u.ualign.type != A_CHAOTIC) && !molochaltar)
            {
                goto desecrate_high_altar;
            } 
            else if (altaralign != A_CHAOTIC && !molochaltar) 
            {
                /* curse the lawful/neutral altar */
                play_sfx_sound(SFX_DESECRATE_ALTAR);
                pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "altar is stained with %s blood.", urace.adj);
                levl[u.ux][u.uy].altarmask = AM_CHAOTIC;
                angry_priest();
            } 
            else 
            {
                struct monst *dmon = 0;
                const char *demonless_msg;

                /* Human sacrifice on a chaotic or Moloch altar */
                /* is equivalent to demon summoning */
                play_sfx_sound(SFX_DESECRATE_ALTAR);
                if (altaralign == A_CHAOTIC && u.ualign.type != A_CHAOTIC)
                {
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "The blood floods the altar, which vanishes in %s cloud!",
                          an(hcolor(NH_BLACK)));

                    create_current_floor_location(u.ux, u.uy, 0, NO_GLYPH, TRUE);

                    angry_priest();
                    demonless_msg = "cloud dissipates";
                } 
                else 
                {
                    /* either you're chaotic or altar is Moloch's or both */
                    pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "blood covers the altar!");
                    luck_change += (molochaltar ? -2 : 2);
                    demonless_msg = "blood coagulates";
                }

                if(Inhell)
                    dmon = makemon(&mons[PM_DEMOGORGON], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);

                if (!dmon)
                {
                    mndx = dlord(altaralign);
                    if(mndx >= LOW_PM && mndx < NUM_MONSTERS)
                        dmon = makemon(&mons[mndx], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
                }

                if (dmon)
                {
                    if(context.dlords_summoned_via_altar < 255)
                        context.dlords_summoned_via_altar++;
                    play_sfx_sound(SFX_SUMMON_DEMON);
                    char dbuf[BUFSZ];
                    boolean itdreadful = FALSE;

                    Strcpy(dbuf, a_monnam(dmon));
                    if (!strcmpi(dbuf, "it"))
                    {
                        itdreadful = TRUE;
                        Strcpy(dbuf, "something dreadful");
                    }
                    else
                        dmon->mstrategy &= ~STRAT_APPEARMSG;

                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "have summoned %s!", dbuf);
                    if (sgn(u.ualign.type) == sgn(dmon->data->maligntyp))
                    {
                        int rndval = (u.ualign.record >= PIOUS ? 5 : u.ualign.record >= DEVOUT ? 4 : u.ualign.record >= FERVENT ? 3 : u.ualign.record >= STRIDENT ? 2 : u.ualign.record >= 0 ? 1 : 0) + 2 - (int)context.dlords_summoned_via_altar;
                        if (context.dlords_summoned_via_altar <= 1 || (rndval > 1 && rn2(rndval)) || !rn2(1 + context.dlords_summoned_via_altar * (u.ualign.record < 0 ? 2 : 1)))
                        {
                            dmon->mpeaceful = TRUE;
                            dmon->mon_flags |= MON_FLAGS_SUMMONED_AT_ALTAR;
                            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "Luckily for you, %s appears to be pleased with your sacrifice.", itdreadful ? "that something" : dbuf);
                        }
                        else
                            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "you have overstayed your welcome with the Lords of the Abyss.");
                    }
                    if (!Fear_resistance)
                    {
                        play_sfx_sound(SFX_ACQUIRE_FEAR);
                        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are terrified, and unable to move.");
                        nomul(-3);
                        multi_reason = "being terrified of a demon";
                        nomovemsg = 0;
                    }
                }
                else
                    pline_The("%s.", demonless_msg);
            }

            if (u.ualign.type != A_CHAOTIC) 
            {
                adjalign(-5);
                u.ugangr += 3;
                (void) adjattrib(A_WIS, -1, TRUE);
                if (!Inhell)
                    angrygods(u.ualign.type);
                luck_change += -5;
            } else
                adjalign(5);

            change_luck(luck_change, TRUE);

            if (carried(otmp))
                useup(otmp);
            else
                useupf(otmp, 1L);
            return 1;
        } 
        else if (has_omonst(otmp)
                   && (mtmp = get_mtraits(otmp, FALSE)) != 0
                   && is_tame(mtmp)) 
        {
                /* mtmp is a temporary pointer to a tame monster's attributes,
                 * not a real monster */
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "So this is how you repay loyalty?");
            adjalign(-3);
            value = -1;
            HAggravate_monster |= FROM_ACQUIRED;
            play_sfx_sound(SFX_SACRIFICE_PET);
        }
        else if (is_undead(ptr)) { /* Not demons--no demon corpses */
            if (u.ualign.type != A_CHAOTIC)
                value += 1;
        }
        else if (is_unicorn(ptr)) 
        {
            int unicalign = sgn(ptr->maligntyp);

            if (unicalign == altaralign) 
            {
                /* When same as altar, always a very bad action.
                 */
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Such an action is an insult to %s!",
                      (unicalign == A_CHAOTIC) ? "chaos"
                         : unicalign ? "law" : "balance");
                (void) adjattrib(A_WIS, -1, TRUE);
                value = -5;
                play_sfx_sound(SFX_SACRIFICE_COALIGNED_UNICORN);
            }
            else if (u.ualign.type == altaralign)
            {
                /* When different from altar, and altar is same as yours,
                 * it's a very good action.
                 */
                if (u.ualign.record < ALIGNLIM)
                    You_feel_ex(ATR_NONE, CLR_MSG_SUCCESS, "appropriately %s.", align_str(u.ualign.type));
                else
                    You_feel_ex(ATR_NONE, CLR_MSG_SUCCESS, "you are thoroughly on the right path.");
                adjalign(5);
                value += 3;
                play_sfx_sound(SFX_ALTAR_ADD_ALIGNMENT);
            }
            else if (unicalign == u.ualign.type)
            {
                /* When sacrificing unicorn of your alignment to altar not of
                 * your alignment, your god gets angry and it's a conversion.
                 */
                u.ualign.record = -1;
                value = 1;
            }
            else
            {
                /* Otherwise, unicorn's alignment is different from yours
                 * and different from the altar's.  It's an ordinary (well,
                 * with a bonus) sacrifice on a cross-aligned altar.
                 */
                value += 3;
            }
        }
    } /* corpse */

    if (otmp->otyp == AMULET_OF_YENDOR)
    {
        if (!highaltar)
        {
        too_soon:
            if (molochaltar)
                /* hero has left Moloch's Sanctum so is in the process
                   of getting away with the Amulet (outside of Gehennom,
                   fall through to the "ashamed" feedback) */
                gods_upset(A_NONE);
            else
                You_feel("%s.",
                         Hallucination
                            ? "homesick"
                            /* if on track, give a big hint */
                            : (altaralign == u.ualign.type)
                               ? "an urge to return to the surface"
                               /* else headed towards celestial disgrace */
                               : "ashamed");
            return 1;
        }
        else
        {
            /* The final Test.  Did you win? */
            if (uamul == otmp)
                Amulet_off();
            u.uevent.ascended = 1;
            if (carried(otmp))
                useup(otmp); /* well, it's gone now */
            else
                useupf(otmp, 1L);
            You("offer the Amulet of Yendor to %s...", a_gname());
            play_sfx_sound(SFX_ALTAR_BURST_OF_FLAME);
            if (molochaltar)
            {
                /* Moloch's high altar */
                if (u.ualign.record > -99)
                    u.ualign.record = -99;
                /*[apparently shrug/snarl can be sensed without being seen]*/
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s shrugs and retains dominion over %s,", Moloch,
                      u_gname());
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "then mercilessly snuffs out your life.");
                Sprintf(killer.name, "%s indifference", s_suffix(Moloch));
                killer.format = KILLED_BY;
                done(DIED);
                /* life-saved (or declined to die in wizard/explore mode) */
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s snarls and tries again...", Moloch);
                fry_by_god(A_NONE, TRUE); /* wrath of Moloch */
                /* declined to die in wizard or explore mode */
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, cloud_of_smoke, hcolor(NH_BLACK));
                done(ESCAPED);
            } 
            else if (u.ualign.type != altaralign)
            {
                /* And the opposing team picks you up and
                   carries you off on their shoulders */
                adjalign(-99);
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s accepts your gift, and gains dominion over %s...",
                      a_gname(), u_gname());
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s is enraged...", u_gname());
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "Fortunately, %s permits you to live...", a_gname());
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, cloud_of_smoke, hcolor(NH_ORANGE));
                done(ESCAPED);
            } 
            else
            { /* super big win */
                adjalign(10);
                if (flags.showscore && !u.uachieve.ascended)
                    context.botl = 1;

                u.uachieve.ascended = 1;

                play_sfx_sound(SFX_INVISIBLE_CHOIR_SINGS);
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "An invisible choir sings, and you are bathed in radiance...");
                
                /* Radiance */
                int x, y;
                for (x = 1; x < COLNO; x++)
                    for (y = 0; y < ROWNO; y++)
                        if(cansee(x, y))
                            newsym(x, y);

                flush_screen(1);
                
                if (iflags.using_gui_sounds)
                    delay_output_milliseconds(4500);

                play_voice_god_simple_line_by_align(altaralign, GOD_LINE_MORTAL_THOU_HAST_DONE_WELL);
                godvoice(altaralign, "Mortal, thou hast done well!");
                display_nhwindow(WIN_MESSAGE, FALSE);
                play_voice_god_simple_line_by_align(altaralign, GOD_LINE_IN_RETURN_FOR_THY_SERVICE_I_GRANT_THEE_THE_GIFT_OF_IMMORTALITY);
                verbalize_ex(ATR_NONE, CLR_MSG_GOD,
                      "In return for thy service, I grant thee the gift of Immortality!");
                You_ex(ATR_NONE, CLR_MSG_POSITIVE, "ascend to the status of Demigod%s...",
                    flags.female ? "dess" : "");
                achievement_gained("Ascended");
                done(ASCENDED);
            }
        }
    } /* real Amulet */

    if (otmp->otyp == FAKE_AMULET_OF_YENDOR)
    {
        if (!highaltar && !otmp->known)
            goto too_soon;

        play_sfx_sound(SFX_ALTAR_THUNDERCLAP);
        You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "a nearby thunderclap.");

        if (!otmp->known)
        {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "realize you have made a %s.",
                Hallucination ? "boo-boo" : "mistake");
            otmp->known = TRUE;
            change_luck(-1, TRUE);
            return 1;
        } 
        else 
        {
            /* don't you dare try to fool the gods */
            if (Deaf)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "Oh, no."); /* didn't hear thunderclap */
            luck_change += -3;
            adjalign(-1);
            u.ugangr += 3;
            value = -3;
        }
    } /* fake Amulet */

    if (value == 0) 
    {
        if (luck_change)
            change_luck(luck_change, TRUE);
        else
            pline1(nothing_happens);
        return 1;
    }

    if (altaralign != u.ualign.type && highaltar) 
    {
    desecrate_high_altar:
        /*
         * REAL BAD NEWS!!! High altars cannot be converted.  Even an attempt
         * gets the god who owns it truly pissed off.
         */
        play_sfx_sound(SFX_ALTAR_CHARGED_AIR);
        You_feel_ex(ATR_NONE, CLR_MSG_MYSTICAL, "the air around you grow charged...");
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "Suddenly, you realize that %s has noticed you...", a_gname());
        play_voice_god_simple_line_by_align(altaralign, GOD_LINE_SO_MORTAL_YOU_DARE_DESECRATE_MY_HIGH_TEMPLE);
        godvoice(altaralign,
                 "So, mortal!  You dare desecrate my High Temple!");
        /* Throw everything we have at the player */
        god_zaps_you(altaralign);
    }
    else if (value < 0) 
    { /* I don't think the gods are gonna like this... */
        if(!godlessaltar)
            gods_upset(altaralign);
        else
        {
            pline1(nothing_happens);
            return 1;
        }
    }
    else
    {
        int saved_anger = u.ugangr;
        int saved_cnt = u.uprayer_timeout;
        //int saved_luck = u.uluck;

        /* Sacrificing at an altar of a different alignment */
        if (u.ualign.type != altaralign) 
        {
            /* Is this a conversion ? */
            /* An unaligned altar in Gehennom will always elicit rejection. */
            if (ugod_is_angry() || (molochaltar && Inhell)) 
            {
                if (u.ualignbase[A_CURRENT] == u.ualignbase[A_ORIGINAL]
                    && altaralign != A_NONE) 
                {
                    You_ex(ATR_NONE, CLR_MSG_WARNING, "have a strong feeling that %s is angry...", u_gname());
                    consume_offering(otmp);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s accepts your allegiance.", a_gname());
                    play_sfx_sound(SFX_ALTAR_ANGRY_ACCEPTS_SACRIFICE);

                    uchangealign(altaralign, 0);
                    /* Beware, Conversion is costly */
                    luck_change += -3;
                    u.uprayer_timeout += Role_if(PM_PRIEST) ? 150 : 300;
                } 
                else if (godlessaltar)
                {
                    pline1(nothing_happens);
                    return 1;
                }
                else 
                {
                    u.ugangr += 3;
                    adjalign(-5);
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s rejects your sacrifice!", a_gname());
                    play_sfx_sound(SFX_ALTAR_ANGRY_REJECTS_SACRIFICE);
                    play_voice_god_simple_line_by_align(altaralign, GOD_LINE_SUFFER_INFIDEL);
                    godvoice(altaralign, "Suffer, infidel!");
                    luck_change += -5;
                    (void) adjattrib(A_WIS, -2, TRUE);
                    if (!Inhell)
                        angrygods(u.ualign.type);
                }
                change_luck(luck_change, TRUE);
                return 1;
            }
            else 
            {
                consume_offering(otmp);
                if(!godlessaltar)
                    You_ex(ATR_NONE, CLR_MSG_WARNING, "sense a conflict between %s and %s.", u_gname(), a_gname());
                if (godlessaltar || rn2(8 + u.ulevel) > 5)
                {
                    struct monst *pri;
                    play_sfx_sound(SFX_ALTAR_POWER_INCREASE);
                    You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "the power of %s increase.", u_gname());
                    exercise(A_WIS, TRUE);
                    luck_change += 1;
                    /* Yes, this is supposed to be &=, not |= */
                    levl[u.ux][u.uy].altarmask &= AM_SHRINE;
                    /* the following accommodates stupid compilers */
                    levl[u.ux][u.uy].altarmask = (levl[u.ux][u.uy].altarmask | (Align2amask(u.ualign.type)));
                    levl[u.ux][u.uy].subtyp = ALTAR_SUBTYPE_NORMAL; /* In the case it is Moloch's altar or a high altar */
                    newsym(u.ux, u.uy);
                    if (!Blind)
                        pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "altar glows %s.",
                                  hcolor((u.ualign.type == A_LAWFUL)
                                            ? NH_WHITE
                                            : u.ualign.type
                                               ? NH_BLACK
                                               : (const char *) "gray"));

                    if (rnl(u.ulevel) > 6 && u.ualign.record > 0
                        && rnd(u.ualign.record) > (3 * ALIGNLIM) / 4)
                        summon_minion(altaralign, TRUE);
                    /* anger priest; test handles bones files */
                    if ((pri = findpriest(temple_occupied(u.urooms)))
                        && !p_coaligned(pri))
                        angry_priest();
                }
                else
                {
                    play_sfx_sound(SFX_ALTAR_POWER_DECREASE);
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Unluckily, you feel the power of %s decrease.",
                          u_gname());
                    luck_change += -1;
                    exercise(A_WIS, FALSE);
                    if (rnl(u.ulevel) > 6 && u.ualign.record > 0
                        && rnd(u.ualign.record) > (7 * ALIGNLIM) / 8)
                        summon_minion(altaralign, TRUE);
                }
                change_luck(luck_change, TRUE);
                return 1;
            }
        }

        consume_offering(otmp);

        boolean bless_savestone = FALSE;

        /* OK, you get brownie points. */
        if (u.ugangr) 
        {
            u.ugangr -= ((value * (u.ualign.type == A_CHAOTIC ? 2 : 3))
                         / MAXVALUE);
            if (u.ugangr < 0)
                u.ugangr = 0;
            if (u.ugangr != saved_anger)
            {
                play_sfx_sound(SFX_ALTAR_GOD_MOLLIFIED);
                if (u.ugangr)
                {
                    pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s seems %s.", u_gname(),
                          Hallucination ? "groovy" : "slightly mollified");

                    if ((int) u.uluck < 0)
                        luck_change += 1;
                } 
                else
                {
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s seems %s.", u_gname(),
                          Hallucination ? "cosmic (not a new fact)"
                                        : "mollified");

                    if ((int) u.uluck < 0)
                        u.uluck = 0;
                }
            } 
            else 
            { /* not satisfied yet */
                play_sfx_sound(SFX_ALTAR_INADEQUACY);
                if (Hallucination)
                    pline_The_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "gods seem tall.");
                else
                    You_ex(ATR_NONE, NO_COLOR, "have a feeling of inadequacy.");
            }
        }
        else if (ugod_is_angry()) 
        {
            if (value > MAXVALUE)
                value = MAXVALUE;
            if (value > -u.ualign.record)
                value = -u.ualign.record;
            adjalign(value);
            play_sfx_sound(SFX_ALTAR_ABSOLVED);
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "partially absolved.");
        }
        else if (u.uprayer_timeout > 0)
        {
            bless_savestone = TRUE;

            u.uprayer_timeout -= ((value * (u.ualign.type == A_CHAOTIC ? 500 : 300) / (Role_if(PM_PRIEST) ? 2 : 1))
                            / MAXVALUE);
            if (u.uprayer_timeout < 0)
                u.uprayer_timeout = 0;
            if (u.uprayer_timeout != saved_cnt) 
            {
                if (u.uprayer_timeout) 
                {
                    play_sfx_sound(SFX_ALTAR_HOPEFUL_FEELING);
                    if (Hallucination)
                        You_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "realize that the gods are not like you and I.");
                    else
                        You_ex(ATR_NONE, NO_COLOR, "have a hopeful feeling.");
                    if ((int) u.uluck < 0)
                        luck_change += 1;
                } 
                else 
                {
                    play_sfx_sound(SFX_ALTAR_RECONCILIATION);
                    if (Hallucination)
                        pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "Overall, there is a smell of fried onions.");
                    else
                        You_ex(ATR_NONE, CLR_MSG_POSITIVE, "have a feeling of reconciliation.");
                    if ((int) u.uluck < 0)
                        u.uluck = 0;
                }
            }
        }
        else
        {
            bless_savestone = TRUE;

            int nartifacts = nartifact_exist();

            /* you were already in pretty good standing */
            /* The player can gain an artifact */
            /* The chance goes down as the number of artifacts goes up */
            if (u.ulevel > 2 && u.uluck >= 0
                && !rn2(10 + (2 * u.ugifts * nartifacts))) 
            {
                otmp = mk_artifact((struct obj *) 0, a_align(u.ux, u.uy), urole.rolenum == ROLE_MONK ? MKARTIFACT_FLAGS_NONWEAPONS_ONLY : MKARTIFACT_FLAGS_NONE);
                if (!otmp)
                {
                    otmp = mksobj(get_artifact_replacement_item_otyp(), TRUE, FALSE, 2);
                    if (otmp)
                    {
                        if (otmp->cursed)
                            uncurse(otmp);

                        if (objects[otmp->otyp].oc_charged > CHARGED_NOT_CHARGED)
                            otmp->charges = get_obj_max_charge(otmp);

                        if (objects[otmp->otyp].oc_enchantable > ENCHTYPE_NO_ENCHANTMENT)
                        {
                            int maxench = get_obj_max_enchantment(otmp);
                            if (otmp->enchantment <= 0)
                                otmp->enchantment = 1; 
                            if (otmp->enchantment < maxench)
                                otmp->enchantment++;
                            otmp->enchantment = min(maxench, max(otmp->enchantment, rnd(max(2, maxench))));
                        }
                    }
                }

                if (otmp)
                {
                    play_sfx_sound(SFX_ALTAR_GIFT);
                    if (otmp->enchantment < 0)
                        otmp->enchantment = 0;
                    if (otmp->cursed)
                        uncurse(otmp);
                    if(erosion_matters(otmp))
                        otmp->oerodeproof = TRUE;
                    at_your_feet("An object");
                    dropy(otmp);
                    if (is_launcher(otmp))
                    {
                        struct obj* otmp2 = (struct obj*)0;
                        if (objects[otmp->otyp].oc_skill == P_BOW)
                            otmp2 = mksobj_with_flags(ARROW, FALSE, FALSE, FALSE, (struct monst*)0, MAT_SILVER, 0L, 0L, 0UL);
                        else if(objects[otmp->otyp].oc_skill == P_CROSSBOW)
                            otmp2 = mksobj_with_flags(CROSSBOW_BOLT, FALSE, FALSE, FALSE, (struct monst*)0, MAT_SILVER, 0L, 0L, 0UL);
                        else if (objects[otmp->otyp].oc_skill == P_SLING)
                            otmp2 = mksobj_with_flags(SLING_BULLET, FALSE, FALSE, FALSE, (struct monst*)0, MAT_SILVER, 0L, 0L, 0UL);

                        if (otmp2)
                        {
                            otmp2->quan = 50;
                            otmp2->oerodeproof = TRUE;
                            bless(otmp2);
                            otmp2->enchantment = rn2(4);
                            otmp2->owt = weight(otmp2);
                            dropy(otmp2);
                        }
                    }
                    play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_USE_MY_GIFT_WISELY);
                    godvoice(u.ualign.type, "Use my gift wisely!");
                    u.ugifts++;
                    u.uprayer_timeout = Role_if(PM_PRIEST) ? rnz(150 + (25 * nartifacts)) : rnz(300 + (50 * nartifacts));
                    exercise(A_WIS, TRUE);
                    livelog_printf(LL_DIVINEGIFT | LL_ARTIFACT,
                        "was bestowed with %s by %s",
                        artiname(otmp->oartifact),
                        align_gname(u.ualign.type));

                    /* make sure we can use this weapon */
                    enum p_skills wep_skill_idx = weapon_skill_type(otmp);
                    if (wep_skill_idx > P_NONE)
                    {
                        unrestrict_weapon_skill(wep_skill_idx);
                    }
                    if (!Hallucination && !Blind)
                    {
                        otmp->dknown = 1;
                        makeknown(otmp->otyp);
                        if (otmp->oartifact)
                        {
                            otmp->nknown = 1;
                            discover_artifact(otmp->oartifact);
                        }
                    }
                    return 1;
                }
            }
            luck_change += (value * LUCKMAX) / (MAXVALUE * 2);
            if (luck_change + u.uluck > LUCKMAX)
                luck_change -= (luck_change + u.uluck - LUCKMAX);
            if (luck_change + u.uluck < LUCKMIN)
                luck_change -= (luck_change + u.uluck - LUCKMIN);
            if (luck_change + u.uluck < 0)
                luck_change -= (luck_change + u.uluck - 0);

            if (luck_change) //u.uluck != saved_luck) 
            {
                play_sfx_sound(SFX_ALTAR_FOUR_LEAF_CLOVER);
                if (Blind)
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "think %s brushed your %s.", something,
                        body_part(FOOT));
                else
                    You_ex(ATR_NONE, CLR_MSG_SUCCESS, Hallucination
                    ? "see crabgrass at your %s.  A funny thing in a dungeon."
                            : "glimpse a four-leaf clover at your %s.",
                        makeplural(body_part(FOOT)));
            }
        }

        if (bless_savestone)
        {
            struct obj* otmp2;
            for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
            {
                if (is_obj_special_praying_item(otmp2) && !otmp2->blessed)
                {
                    play_sfx_sound(SFX_AURA_GLOW);
                    if(!Blind)
                        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s with %s aura.", Yobjnam2(otmp2, "softly glow"), an(hcolor(NH_LIGHT_BLUE)));
                    bless(otmp2);
                    otmp2->bknown = 1;
                    update_inventory();
                    break;
                }
            }
        }
    }
    change_luck(luck_change, TRUE);
    return 1;
}

void
removealtarsummons(VOID_ARGS)
{
    int cnt = 0;
    struct monst* mtmp, *mtmp2;
    for (mtmp = fmon; mtmp; mtmp = mtmp2)
    {
        mtmp2 = mtmp->nmon; //Just in case mtmp gets deleted
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->mon_flags & MON_FLAGS_SUMMONED_AT_ALTAR)
        {
            //Returns to whence it came
            mongone(mtmp);
            cnt++;
        }
    }
    if (cnt > 0 && !Deaf)
    {
        play_sfx_sound(SFX_DISTANT_PUFF);
        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a distant puff.");
    }
}

int
get_artifact_replacement_item_otyp()
{
    /* Non-artifact replacement in the case an artifact couldn't be made (most likely a case for monk) */
    int repl_otyp = ACURR(A_STR) < STR19(19) && !rn2(2) && !carrying(BELT_OF_STORM_GIANT_STRENGTH) && !carrying(BELT_OF_STONE_GIANT_STRENGTH) && !carrying(BELT_OF_FIRE_GIANT_STRENGTH) && !carrying(BELT_OF_FROST_GIANT_STRENGTH) && !carrying(BELT_OF_HILL_GIANT_STRENGTH) ? BELT_OF_STORM_GIANT_STRENGTH :
        !EReflecting && !rn2(2) && !carrying(BRACERS_OF_REFLECTION) && !carrying(AMULET_OF_REFLECTION) ? BRACERS_OF_REFLECTION :
        !EAntimagic && !rn2(2) && !carrying(CLOAK_OF_MAGIC_RESISTANCE) ? CLOAK_OF_MAGIC_RESISTANCE :
        !EVery_fast && !rn2(2) && !carrying(GLOVES_OF_HASTE) && !carrying(SPEED_BOOTS) ? (!rn2(2) ? GLOVES_OF_HASTE : SPEED_BOOTS) :
        !ELifesaved && !rn2(2) && !carrying(AMULET_OF_LIFE_SAVING) ? AMULET_OF_LIFE_SAVING :
        !EFlying && !rn2(2) && !carrying(WINGS_OF_FLYING) ? WINGS_OF_FLYING :
        !EDeath_resistance && !rn2(2) && !carrying(RIN_PROTECTION_FROM_UNDEATH) && !carrying(AMULET_VERSUS_UNDEATH) && !carrying(GOWN_OF_THE_ARCHBISHOPS) ? RIN_PROTECTION_FROM_UNDEATH :
        ACURR(A_STR) < STR19(24) && !rn2(2) && !carrying(BELT_OF_STORM_GIANT_STRENGTH) ? BELT_OF_STORM_GIANT_STRENGTH : !rn2(2) ? (!rn2(2) ? WAN_DEATH : WAN_DISINTEGRATION) :
        !rn2(2) ? MAGIC_MARKER :
        !rn2(2) ? POT_GAIN_LEVEL :
        !rn2(2) ? POT_LIGHTNING_SPEED : POT_GREATER_REGENERATION;

    return repl_otyp;
}
/* determine prayer results in advance; also used for enlightenment */
boolean
can_pray(praying)
boolean praying; /* false means no messages should be given */
{
    int alignment;

    p_aligntyp = on_altar() ? a_align(u.ux, u.uy) : u.ualign.type;
    p_trouble = in_trouble();

    if (is_demon(youmonst.data) && (p_aligntyp != A_CHAOTIC)) 
    {
        if (praying)
            pline_The("very idea of praying to a %s god is repugnant to you.",
                      p_aligntyp ? "lawful" : "neutral");
        return FALSE;
    }

    if (praying)
        You("begin praying to %s.", align_gname(p_aligntyp));

    if (u.ualign.type && u.ualign.type == -p_aligntyp)
        alignment = -u.ualign.record; /* Opposite alignment altar */
    else if (u.ualign.type != p_aligntyp)
        alignment = u.ualign.record / 2; /* Different alignment altar */
    else
        alignment = u.ualign.record;

    if ((p_trouble > 0) ? (u.uprayer_timeout > 200)      /* big trouble */
           : (p_trouble < 0) ? (u.uprayer_timeout > 100) /* minor difficulties */
              : (u.uprayer_timeout > 0))                 /* not in trouble */
        p_type = 0;                     /* too soon... */
    else if ((int) Luck < 0 || u.ugangr || alignment < 0)
        p_type = 1; /* too naughty... */
    else /* alignment >= 0 */ 
    {
        if (on_altar() && u.ualign.type != p_aligntyp)
            p_type = 2;
        else
            p_type = 3;
    }

    if (is_undead(youmonst.data) && !Inhell
        && (p_aligntyp == A_LAWFUL || (p_aligntyp == A_NEUTRAL && !rn2(10))))
        p_type = -1;
    /* Note:  when !praying, the random factor for neutrals makes the
       return value a non-deterministic approximation for enlightenment.
       This case should be uncommon enough to live with... */

    return !praying ? (boolean) (p_type == 3 && !Inhell) : TRUE;
}

/* #pray commmand */
int
dopray()
{
    if (!context.spellpray)
    {
        (void)can_pray(FALSE);
        boolean tooquickly = (p_trouble > 0) ? (u.uprayer_timeout > 200)
            : (p_trouble < 0) ? (u.uprayer_timeout > 100)
            : (u.uprayer_timeout > 0);

        if (context.game_difficulty < 0 && tooquickly && !context.starting_prayer_timeout_expired)
        {
            char qbuf[BUFSZ];
            Sprintf(qbuf, "Praying soon after starting the game may anger your %s. Do you still want to pray?", align_gtitle(u.ualign.type));
            if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Praying Too Early", qbuf) != 'y')
                return 0;
        }
        else if (context.game_difficulty == MIN_DIFFICULTY_LEVEL && tooquickly)
        {
            char qbuf[BUFSZ];
            Sprintf(qbuf, "Praying too often in succession may anger your %s. Do you still want to pray?", align_gtitle(u.ualign.type));
            if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Praying Too Often", qbuf) != 'y')
                return 0;
        }
        else if (context.game_difficulty == MIN_DIFFICULTY_LEVEL && Inhell)
        {
            char qbuf[BUFSZ];
            Strcpy(qbuf, "You cannot safely pray in Gehennom. Do you still want to pray?");
            if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Praying in Gehennom", qbuf) != 'y')
                return 0;
        }
        else if (ParanoidPray || context.game_difficulty == MIN_DIFFICULTY_LEVEL)
        {
            /* Confirm accidental slips of Alt-P */
            char qbuf[BUFSZ];
            if(context.game_difficulty == MIN_DIFFICULTY_LEVEL)
                Sprintf(qbuf, "Praying can be dangerous unless your %s views you favorably. Are you sure you want to pray?", align_gtitle(u.ualign.type));
            else
                strcpy(qbuf, "Are you sure you want to pray?");
            if(yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Confirm Prayer", qbuf) != 'y')
                return 0;
        }
    }
    
    if (!u.uconduct.gnostic++)
        /* breaking conduct should probably occur in can_pray() at
         * "You begin praying to %s", as demons who find praying repugnant
         * should not break conduct.  Also we can add more detail to the
         * livelog message as p_aligntyp will be known.
         */
        livelog_printf(LL_CONDUCT, "%s", "rejected atheism with a prayer");

    /* set up p_type and p_alignment */
    if (!can_pray(TRUE))
        return 0;

    if (wizard && p_type >= 0) 
    {
        if (yn_query("Force the gods to be pleased?") == 'y') 
        {
            u.uprayer_timeout = 0;
            if (u.uluck < 0)
                u.uluck = 0;
            if (u.ualign.record <= 0)
                u.ualign.record = 1;
            u.ugangr = 0;
            if (p_type < 2)
                p_type = 3;
        }
    }

    nomul(-3);
    multi_reason = "praying";
    nomovemsg = "You finish your prayer.";
    afternmv = prayer_done;

    if (iflags.using_gui_sounds)
    {
        play_simple_player_voice(PLAYER_VOICE_TYPE_PRAY);
        delay_output_milliseconds(1500);
    }

    if (p_type == 3 && !Inhell) 
    {
        /* if you've been true to your god you can't die while you pray */
        if (!Blind)
            You_ex(ATR_NONE, CLR_MSG_MYSTICAL, "are surrounded by a shimmering light.");
        play_sfx_sound(SFX_PRAY_SHIMMERING_LIGHT);
        u.uinvulnerable = TRUE;
    }

    return 1;
}

STATIC_PTR int
prayer_done() /* M. Stephenson (1.0.3b) */
{
    aligntyp alignment = p_aligntyp;

    u.uinvulnerable = FALSE;

    if (p_type == -1) 
    {
        play_voice_god_simple_line_by_align(alignment, (alignment == A_LAWFUL) ? GOD_LINE_VILE_CREATURE_THOU_DURST_CALL_UPON_ME : GOD_LINE_WALK_NO_MORE_PERVERSION_OF_NATURE);
        godvoice(alignment,
                 (alignment == A_LAWFUL)
                    ? "Vile creature, thou durst call upon me?"
                    : "Walk no more, perversion of nature!");
        You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "like you are falling apart.");
        /* KMH -- Gods have mastery over unchanging */
        rehumanize();
        /* no Half_physical_damage adjustment here */
        losehp(adjust_damage(rnd(20), (struct monst*)0, &youmonst, AD_CLRC, ADFLAGS_NONE), "residual undead turning effect", KILLED_BY_AN);
        exercise(A_CON, FALSE);
        return 1;
    }

    if (Inhell) 
    {
        play_sfx_sound(SFX_PRAY_FAIL);
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Since you are in Gehennom, %s won't help you.",
              align_gname(alignment));
        /* haltingly aligned is least likely to anger */
        if (u.ualign.record <= 0 || rnl(u.ualign.record))
            angrygods(u.ualign.type);
        return 0;
    }

    if (p_type == 0)
    {
        play_sfx_sound(SFX_PRAY_FAIL);
        if (on_altar() && u.ualign.type != alignment)
            (void)altar_prayer(FALSE);

        u.uprayer_timeout += Role_if(PM_PRIEST) ? rnz(125) : rnz(250);
        gods_upset(u.ualign.type);
        change_luck(-3, TRUE);
    } 
    else if (p_type == 1) 
    {
        play_sfx_sound(SFX_PRAY_FAIL);
        if (on_altar() && u.ualign.type != alignment)
            (void) altar_prayer(FALSE);
        angrygods(u.ualign.type); /* naughty */
    } 
    else if (p_type == 2) 
    {
        if (altar_prayer(FALSE))
        {
            play_sfx_sound(SFX_PRAY_FAIL);
            /* attempted water prayer on a non-coaligned altar */
            u.uprayer_timeout += Role_if(PM_PRIEST) ? rnz(125) : rnz(250);
            gods_upset(u.ualign.type);
            change_luck(-3, TRUE);
        }
        else
            pleased(alignment);
    } 
    else
    {
        /* coaligned */
        if (on_altar())
            (void) altar_prayer(TRUE);

        pleased(alignment); /* nice */
    }
    return 1;
}


int
absolution_spell()
{
    /* You are absolved for your sins */
    boolean sins_absolved = FALSE;

    u.uprayer_timeout = 0;
    if (u.uluck < 0)
        u.uluck = 0, sins_absolved = TRUE, u.ucleansed = moves;
    if (u.ualign.record < 0)
        u.ualign.record = 0, sins_absolved = TRUE, u.ucleansed = moves;
    if (u.ugangr)
        u.ugangr = 0, sins_absolved = TRUE, u.ucleansed = moves;

    if (sins_absolved)
    {
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "that your sins have been forgiven.");
        return 1;
    }
    else
    {
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "that %s is already pleased with you.", u_gname());
        return 0;
    }
}


/* OBSOLETE --JG */
/* #turn command */
int
doturn()
{
    /* Knights & Priest(esse)s only please */
    struct monst *mtmp, *mtmp2;
    int once, range, xlev;

    if (!Role_if(PM_PRIEST) && !Role_if(PM_KNIGHT)) {
        /* Try to use the "turn undead" spell.
         *
         * This used to be based on whether hero knows the name of the
         * turn undead spellbook, but it's possible to know--and be able
         * to cast--the spell while having lost the book ID to amnesia.
         * (It also used to tell spelleffects() to cast at self?)
         */
        int sp_no;

        for (sp_no = 0; sp_no < MAXSPELL; ++sp_no) {
            if (spl_book[sp_no].sp_id == NO_SPELL)
                break;
            else if (spl_book[sp_no].sp_id == SPE_TURN_UNDEAD)
                return spelleffects(sp_no, FALSE, &youmonst);
        }
        play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't know how to turn undead!");
        return 0;
    }

    if (!u.uconduct.gnostic++)
        livelog_printf(LL_CONDUCT, "%s", "rejected atheism by turning undead");

    if ((u.ualign.type != A_CHAOTIC
         && (is_demon(youmonst.data) || is_undead(youmonst.data)))
        || u.ugangr > 6) { /* "Die, mortal!" */
        pline("For some reason, %s seems to ignore you.", u_gname());
        aggravate();
        exercise(A_WIS, FALSE);
        return 1;
    }
    if (Inhell) {
        pline("Since you are in Gehennom, %s won't help you.", u_gname());
        aggravate();
        return 1;
    }
    pline("Calling upon %s, you chant an arcane formula.", u_gname());
    exercise(A_WIS, TRUE);

    /* note: does not perform revive_from_inventory() on victims' inventories */
    range = UNDEAD_TURNING_BASE_DISTANCE + (u.ulevel / 5); /* 5 to 11 */
    range *= range;
    once = 0;
    for (mtmp = fmon; mtmp; mtmp = mtmp2) {
        mtmp2 = mtmp->nmon;

        if (DEADMONSTER(mtmp))
            continue;
        if (!cansee(mtmp->mx, mtmp->my) || distu(mtmp->mx, mtmp->my) > range)
            continue;

        if (!is_peaceful(mtmp)
            && (is_undead(mtmp->data) || is_vampshifter(mtmp)
                || (is_demon(mtmp->data) && (u.ulevel > (MAXULEV / 2))))) {
            mtmp->msleeping = 0;
            if (Confusion) {
                if (!once++)
                    pline("Unfortunately, your voice falters.");
                mtmp->mflee = 0;
                mtmp->mfrozen = 0;
                mtmp->mcanmove = 1;
            } else if (!check_magic_resistance_and_inflict_damage(mtmp, (struct obj*)0, (struct monst*)0, u.ulevel, 0, 0, TELL)) {
                xlev = 6;
                switch (mtmp->data->mlet) {
                /* this is intentional, lichs are tougher
                   than zombies. */
                case S_LICH:
                    xlev += 2; /*FALLTHRU*/
                case S_GHOST:
                    xlev += 2; /*FALLTHRU*/
                case S_VAMPIRE:
                    xlev += 2; /*FALLTHRU*/
                case S_WRAITH:
                    xlev += 2; /*FALLTHRU*/
                case S_GREATER_UNDEAD: /* Mummies */
                    xlev += 2; /*FALLTHRU*/
                case S_LESSER_UNDEAD:
                    if (u.ulevel >= xlev && !check_magic_resistance_and_inflict_damage(mtmp, (struct obj*)0, (struct monst*)0, u.ulevel, 0, 0, NOTELL)) {
                        if (u.ualign.type == A_CHAOTIC) {
                            mtmp->mpeaceful = 1;
                            set_mhostility(mtmp);
                            newsym(mtmp->mx, mtmp->my);
                        } else { /* damn them */
                            killed(mtmp);
                        }
                        break;
                    } /* else flee */
                /*FALLTHRU*/
                default:
                    monflee(mtmp, 0, FALSE, TRUE);
                    break;
                }
            }
            refresh_m_tile_gui_info(mtmp, TRUE);
        }
    }
    nomul(-(5 - ((u.ulevel - 1) / 6))); /* -5 .. -1 */
    multi_reason = "trying to turn the monsters";
    nomovemsg = You_can_move_again;
    nomovemsg_color = CLR_MSG_SUCCESS;
    return 1;
}

const char *
a_gname()
{
    return a_gname_at(u.ux, u.uy);
}

/* returns the name of an altar's deity */
const char *
a_gname_at(x, y)
xchar x, y;
{
    if (!IS_ALTAR(levl[x][y].typ))
        return (char *) 0;

    if (levl[x][y].subtyp == ALTAR_SUBTYPE_MOLOCH)
        return Moloch;

    if (a_align(x, y) == A_NONE)
        return "no-one";

    return align_gname(a_align(x, y));
}

/* returns the name of the hero's deity */
const char *
u_gname()
{
    return align_gname(u.ualign.type);
}

boolean
align_gfemale(alignment)
aligntyp alignment;
{
    const char* gnam = 0;

    switch (alignment) {
    case A_NONE:
        gnam = Moloch;
        break;
    case A_LAWFUL:
        gnam = urole.lgod;
        break;
    case A_NEUTRAL:
        gnam = urole.ngod;
        break;
    case A_CHAOTIC:
        gnam = urole.cgod;
        break;
    default:
        impossible("unknown alignment.");
        gnam = "someone";
        break;
    }
    if (gnam && *gnam == '_')
        return TRUE;

    return FALSE;
}

const char *
align_gname(alignment)
aligntyp alignment;
{
    const char *gnam;

    switch (alignment) {
    case A_NONE:
        gnam = Moloch;
        break;
    case A_LAWFUL:
        gnam = urole.lgod;
        break;
    case A_NEUTRAL:
        gnam = urole.ngod;
        break;
    case A_CHAOTIC:
        gnam = urole.cgod;
        break;
    default:
        impossible("unknown alignment.");
        gnam = "someone";
        break;
    }
    if (*gnam == '_')
        ++gnam;
    return gnam;
}

const char*
align_gdesc(alignment)
aligntyp alignment;
{
    const char* result;
    boolean isfemale = align_gfemale(alignment);

    switch (alignment)
    {
    case A_NONE:
        result = isfemale ? "the Lady of Evil" : "the Lord of Evil";
        break;
    case A_LAWFUL:
        result = isfemale ? "the Lady of Law" : "the Lord of Law";
        break;
    case A_NEUTRAL:
        result = isfemale ? "the Lady of Balance" : "the Lord of Balance";
        break;
    case A_CHAOTIC:
        result = isfemale ? "the Lady of Chaos" : "the Lord of Chaos";
        break;
    default:
        result = "the God of Unknown Alignment";
        break;
    }

    return result;
}

const char*
align_ghisher(alignment)
aligntyp alignment;
{
    if (align_gfemale(alignment))
        return "her";
    else
        return "his";
}

const char*
u_ghisher()
{
    return align_ghisher(u.ualign.type);
}


STATIC_VAR const char *hallu_gods[] = {
    "the Flying Spaghetti Monster", /* Church of the FSM */
    "Eris",                         /* Discordianism */
    "the Martians",                 /* every science fiction ever */
    "Xom",                          /* Crawl */
    "AnDoR dRaKoN",                 /* ADOM */
    "the Central Bank of Yendor",   /* economics */
    "Tooth Fairy",                  /* real world(?) */
    "Om",                           /* Discworld */
    "Yawgmoth",                     /* Magic: the Gathering */
    "Morgoth",                      /* LoTR */
    "Cthulhu",                      /* Lovecraft */
    "the Ori",                      /* Stargate */
    "destiny",                      /* why not? */
    "your Friend the Computer",     /* Paranoia */
};

/* hallucination handling for priest/minion names: select a random god
   iff character is hallucinating */
const char *
halu_gname(alignment)
aligntyp alignment;
{
    const char *gnam = NULL;
    int which;

    if (!Hallucination)
        return align_gname(alignment);

    /* Some roles (Priest) don't have a pantheon unless we're playing as
       that role, so keep trying until we get a role which does have one.
       [If playing a Priest, the current pantheon will be twice as likely
       to get picked as any of the others.  That's not significant enough
       to bother dealing with.] */
    do
        which = randrole(TRUE);
    while (!roles[which].lgod);

    switch (rn2_on_display_rng(9)) {
    case 0:
    case 1:
        gnam = roles[which].lgod;
        break;
    case 2:
    case 3:
        gnam = roles[which].ngod;
        break;
    case 4:
    case 5:
        gnam = roles[which].cgod;
        break;
    case 6:
    case 7:
        gnam = hallu_gods[rn2_on_display_rng(SIZE(hallu_gods))];
        break;
    case 8:
        gnam = Moloch;
        break;
    default:
        impossible("rn2 broken in halu_gname?!?");
    }
    if (!gnam) {
        impossible("No random god name?");
        gnam = "your Friend the Computer"; /* Paranoia */
    }
    if (*gnam == '_')
        ++gnam;
    return gnam;
}

/* deity's title */
const char *
align_gtitle(alignment)
aligntyp alignment;
{
    const char *gnam, *result = "god";

    switch (alignment) {
    case A_LAWFUL:
        gnam = urole.lgod;
        break;
    case A_NEUTRAL:
        gnam = urole.ngod;
        break;
    case A_CHAOTIC:
        gnam = urole.cgod;
        break;
    default:
        gnam = 0;
        break;
    }
    if (gnam && *gnam == '_')
        result = "goddess";
    return result;
}

void
altar_wrath(x, y)
register int x, y;
{
    aligntyp altaralign = a_align(x, y);

    if (levl[x][y].subtyp != ALTAR_SUBTYPE_MOLOCH && altaralign == A_NONE) /* Related to no-one */
        return;

    if (!strcmp(align_gname(altaralign), u_gname())) {
        play_voice_god_simple_line_by_align(altaralign, GOD_LINE_HOW_DAREST_THOU_DESECRATE_MY_ALTAR);
        godvoice(altaralign, "How darest thou desecrate my altar!");
        (void) adjattrib(A_WIS, -1, FALSE);
    } else {
        pline("A voice (could it be %s?) whispers:", align_gname(altaralign));
        play_voice_god_simple_line_by_align(altaralign, GOD_LINE_THOU_SHALT_PAY_INFIDEL);
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou shalt pay, infidel!");
        change_luck(-1, TRUE);
    }
}

/* assumes isok() at one space away, but not necessarily at two */
STATIC_OVL boolean
blocked_boulder(dx, dy)
int dx, dy;
{
    register struct obj *otmp;
    int nx, ny;
    long count = 0L;

    for (otmp = level.objects[u.ux + dx][u.uy + dy]; otmp;
         otmp = otmp->nexthere) {
        if (otmp->otyp == BOULDER)
            count += otmp->quan;
    }

    nx = u.ux + 2 * dx, ny = u.uy + 2 * dy; /* next spot beyond boulder(s) */
    switch (count) {
    case 0:
        /* no boulders--not blocked */
        return FALSE;
    case 1:
        /* possibly blocked depending on if it's pushable */
        break;
    case 2:
        /* this is only approximate since multiple boulders might sink */
        if (is_pool_or_lava(nx, ny)) /* does its own isok() check */
            break; /* still need Sokoban check below */
        /*FALLTHRU*/
    default:
        /* more than one boulder--blocked after they push the top one;
           don't force them to push it first to find out */
        return TRUE;
    }

    if (dx && dy && Sokoban) /* can't push boulder diagonally in Sokoban */
        return TRUE;
    if (!isok(nx, ny))
        return TRUE;
    if (IS_ROCK(levl[nx][ny].typ))
        return TRUE;
    if (sobj_at(BOULDER, nx, ny))
        return TRUE;

    return FALSE;
}

int
wiz_crown()
{
    gcrownu();
    return 0;
}

/*pray.c*/

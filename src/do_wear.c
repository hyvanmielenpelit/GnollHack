/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    do_wear.c    $NHDT-Date: 1551138255 2019/02/25 23:44:15 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.108 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_VAR NEARDATA const char c_armor[] = "armor", c_suit[] = "suit",
                           c_shirt[] = "shirt", c_robe[] = "robe",
                           c_bracers[] = "bracers", c_cloak[] = "cloak",
                           c_gloves[] = "gloves", c_boots[] = "boots",
                           c_shield[] = "shield",
                           c_weapon[] = "weapon", c_sword[] = "sword",
                           c_axe[] = "axe", c_that_[] = "that";

STATIC_VAR NEARDATA const long takeoff_order[] = {
    WORN_BLINDF,  W_WEP,      WORN_SHIELD, WORN_GLOVES, LEFT_RING,
    RIGHT_RING,   W_MISC,     W_MISC2,     W_MISC3,     W_MISC4,      W_MISC5,  
    WORN_BRACERS, WORN_CLOAK, WORN_HELMET, WORN_AMUL,   WORN_ROBE,   WORN_ARMOR,
    WORN_SHIRT,   WORN_BOOTS, W_SWAPWEP,   W_SWAPWEP2,   W_QUIVER,    0L
};

STATIC_VAR NEARDATA const long wear_order[] = {
    WORN_SHIRT,   WORN_ARMOR,  WORN_ROBE,    WORN_SHIELD,  WORN_CLOAK,
    WORN_HELMET,  WORN_BOOTS,  WORN_GLOVES,  WORN_BRACERS, LEFT_RING,
    RIGHT_RING,   W_MISC,      W_MISC2,      W_MISC3,      W_MISC4,      W_MISC5,
    WORN_AMUL,    WORN_BLINDF, W_WEP,        W_SWAPWEP,    W_SWAPWEP2,   W_QUIVER,    0L
};

STATIC_DCL void FDECL(on_msg, (struct obj *));
STATIC_DCL void FDECL(toggle_stealth, (struct obj *, long, BOOLEAN_P));
STATIC_DCL void FDECL(toggle_displacement, (struct obj *, long, BOOLEAN_P));
STATIC_PTR int NDECL(Armor_on);
/* int NDECL(Boots_on); -- moved to extern.h */
STATIC_PTR int NDECL(Cloak_on);
STATIC_PTR int NDECL(Helmet_on);
STATIC_PTR int NDECL(Gloves_on);
STATIC_DCL void FDECL(wielding_corpse, (struct obj *, BOOLEAN_P));
STATIC_PTR int NDECL(Shield_on);
STATIC_PTR int NDECL(Shirt_on);
STATIC_PTR int NDECL(Robe_on);
STATIC_PTR int NDECL(Bracers_on);
STATIC_PTR int FDECL(MiscellaneousItem_on, (struct obj*, long));
STATIC_DCL void NDECL(Amulet_on);
STATIC_DCL void FDECL(Ring_off_or_gone, (struct obj *, BOOLEAN_P));
STATIC_PTR int FDECL(select_off, (struct obj *));
STATIC_DCL struct obj *NDECL(do_takeoff);
STATIC_PTR int NDECL(take_off);
STATIC_DCL int FDECL(menu_remarm, (int));
STATIC_DCL void FDECL(count_worn_stuff, (struct obj **, BOOLEAN_P));
STATIC_PTR int FDECL(accessory_or_armor_on, (struct obj *, BOOLEAN_P));
STATIC_DCL void FDECL(already_wearing, (const char *));
STATIC_DCL void FDECL(item_change_sex_and_useup, (struct obj*));
STATIC_DCL int FDECL(already_wearing_with_exchange_prompt, (const char*, struct obj*, struct obj*));
STATIC_DCL int FDECL(exchange_worn_item, (struct obj*, struct obj*, long));
STATIC_DCL int FDECL(take_off_covering_and_wear, (struct obj*, long));
STATIC_DCL int FDECL(add_wear_oid, (struct obj*, long));
STATIC_DCL void NDECL(activate_take_off);
STATIC_DCL void FDECL(print_covering_items, (struct obj*, char*));
STATIC_DCL boolean FDECL(is_armor_covered, (struct obj*));

void
off_msg(otmp)
struct obj *otmp;
{
    if (flags.verbose && otmp)
    {
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_TAKE_OFF);
        You("were wearing %s.", doname(otmp));
    }
}

/* for items that involve no delay */
STATIC_OVL void
on_msg(otmp)
struct obj *otmp;
{
    if (flags.verbose && otmp) {
        char how[BUFSZ];
        /* call xname() before obj_is_pname(); formatting obj's name
           might set obj->dknown and that affects the pname test */
        const char *otmp_name = xname(otmp);

        how[0] = '\0';
        if (otmp->otyp == TOWEL)
            Sprintf(how, " around your %s", body_part(HEAD));

        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_WEAR);
        You("are now wearing %s%s.",
            obj_is_pname(otmp) ? the(otmp_name) : an(otmp_name), how);
    }
}

STATIC_OVL boolean
is_armor_covered(obj)
struct obj* obj;
{
    return ((obj == uarmo) && uarmc)
            || ((obj == uarm) && (uarmc || uarmo))
            || ((obj == uarmu) && (uarmc || uarmo || uarm));
}

STATIC_DCL void
print_covering_items(obj, printbuf)
struct obj* obj;
char* printbuf;
{
    if (!printbuf)
        return;

    char* what = printbuf;
    *what = 0;

    if ((obj == uarmu) && uarm && uarmo && uarmc)
    {
        Strcat(what, cloak_simple_name(uarmc));
        Strcat(what, ", ");
        Strcat(what, robe_simple_name(uarm));
        Strcat(what, ", and ");
        Strcat(what, suit_simple_name(uarm));
    }
    else
    {
        //First it must be a problem with the cloak if there is one
        if (uarmc)
        {
            Strcat(what, cloak_simple_name(uarmc));
            //Check if there is another culprit
            if ((obj == uarmu) && uarm) {
                Strcat(what, " and ");
                Strcat(what, suit_simple_name(uarm));
            }
            else if ((obj == uarmu) && uarmo) {
                Strcat(what, " and ");
                Strcat(what, robe_simple_name(uarmo));
            }
            else if ((obj == uarm) && uarmo) {
                Strcat(what, " and ");
                Strcat(what, robe_simple_name(uarmo));
            }
        }
        else
        {
            //If it wasn't the cloak then check if it is the robe
            if (uarmo)
            {
                Strcat(what, robe_simple_name(uarmo));
                //Check if there is another culprit
                if ((obj == uarmu) && uarm) {
                    Strcat(what, " and ");
                    Strcat(what, suit_simple_name(uarm));
                }
            }
            else
            {
                //If not the cloak or the robe, then it must be a shirt being taken out with an armor on
                Strcat(what, suit_simple_name(uarm));
            }
        }
    }
}

/* starting equipment gets auto-worn at beginning of new game,
   and we don't want stealth or displacement feedback then */
STATIC_VAR boolean initial_don = FALSE; /* manipulated in set_wear() */

/* putting on or taking off an item which confers stealth;
   give feedback and discover it iff stealth state is changing */
STATIC_OVL
void
toggle_stealth(obj, oldprop, on)
struct obj *obj;
long oldprop; /* prop[].extrinsic, with obj->owornmask stripped by caller */
boolean on;
{
    if (on ? initial_don : context.takeoff.cancelled_don)
        return;

    if (!oldprop /* extrinsic stealth from something else */
        && !HStealth) { /* stealth blocked by something */
        if (obj->otyp == RIN_STEALTH)
            learnring(obj, TRUE);
        else
            makeknown(obj->otyp);

        if (on) {
            if (!is_boots(obj))
                You("move very quietly.");
            else if (Levitation || Flying)
                You("float imperceptibly.");
            else
                You("walk very quietly.");
        } else {
            You("sure are noisy.");
        }
    }
}

/* putting on or taking off an item which confers displacement;
   give feedback and discover it iff displacement state is changing *and*
   hero is able to see self (or sense monsters) */
STATIC_OVL
void
toggle_displacement(obj, oldprop, on)
struct obj *obj;
long oldprop; /* prop[].extrinsic, with obj->owornmask stripped by caller */
boolean on;
{
    if (on ? initial_don : context.takeoff.cancelled_don)
        return;

    if (!oldprop /* extrinsic displacement from something else */
        && !(u.uprops[DISPLACED].intrinsic) /* (theoretical) */
        /* we don't use canseeself() here because it augments vision
           with touch, which isn't appropriate for deciding whether
           we'll notice that monsters have trouble spotting the hero */
        && ((!Blind         /* see anything */
             && !u.uswallow /* see surroundings */
             && !Invisib) /* see self */
            /* actively sensing nearby monsters via telepathy or extended
               monster detection overrides vision considerations because
               hero also senses self in this situation */
            || (Unblind_telepat
                || (Blind_telepat && Blind)
                || Detect_monsters))) {
        makeknown(obj->otyp);

        You_feel("that monsters%s have difficulty pinpointing your location.",
                 on ? "" : " no longer");
    }
}

/*
 * The Type_on() functions should be called *after* setworn().
 * The Type_off() functions call setworn() themselves.
 * [Blindf_on() is an exception and calls setworn() itself.]
 */

int
Boots_on(VOID_ARGS)
{
    if (!uarmf)
        return 0;

    if (flags.verbose)
        play_simple_object_sound(uarmf, OBJECT_SOUND_TYPE_WEAR);

    long oldprop =
        u.uprops[objects[uarmf->otyp].oc_oprop].extrinsic & ~WORN_BOOTS;

    switch (uarmf->otyp) {
    case WATER_WALKING_BOOTS:
        if (u.uinwater)
            spoteffects(TRUE);
        /* (we don't need a lava check here since boots can't be
           put on while feet are stuck) */
        break;
    case SPEED_BOOTS:
        makeknown(uarmf->otyp);
        break;
    case ELVEN_BOOTS:
        toggle_stealth(uarmf, oldprop, TRUE);
        break;
    }
    uarmf->known = 1; /* boots' +/- evident because of status line AC */
    return 0;
}

int
Boots_off(VOID_ARGS)
{
    if (!uarmf)
        return 0;

    struct obj *otmp = uarmf;
    int otyp = otmp->otyp;
    long oldprop = u.uprops[objects[otyp].oc_oprop].extrinsic & ~WORN_BOOTS;
    Strcpy(debug_buf_4, "Boots_off");

    if (flags.verbose && otmp)
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMF;
    /* For levitation, float_down() returns if Levitation, so we
     * must do a setworn() _before_ the levitation case.
     */
    setworn((struct obj *) 0, W_ARMF);

    switch (otyp) {
    case WATER_WALKING_BOOTS:
        /* check for lava since fireproofed boots make it viable */
        if ((is_pool(u.ux, u.uy) || is_lava(u.ux, u.uy))
            && !Levitation && !Flying && !is_clinger(youmonst.data)
            && !context.takeoff.cancelled_don
            /* avoid recursive call to lava_effects() */
            && !iflags.in_lava_effects) {
            /* make boots known in case you survive the drowning */
            makeknown(otyp);
            spoteffects(TRUE);
        }
        break;
    case ELVEN_BOOTS:
        toggle_stealth(otmp, oldprop, FALSE);
        break;
    }
    context.takeoff.cancelled_don = FALSE;
    return 0;
}

STATIC_PTR int
Cloak_on(VOID_ARGS)
{
    if (!uarmc)
        return 0;

    if (flags.verbose && uarmc)
        play_simple_object_sound(uarmc, OBJECT_SOUND_TYPE_WEAR);

    long oldprop =
        u.uprops[objects[uarmc->otyp].oc_oprop].extrinsic & ~WORN_CLOAK;

    //boolean wasinvisble = Invis;

    switch (uarmc->otyp) {
    case ELVEN_CLOAK:
        toggle_stealth(uarmc, oldprop, TRUE);
        break;
    case CLOAK_OF_DISPLACEMENT:
        toggle_displacement(uarmc, oldprop, TRUE);
        break;
    case OILSKIN_CLOAK:
        pline("%s very tightly.", Tobjnam(uarmc, "fit"));
        break;
    }
    uarmc->known = 1; /* cloak's +/- evident because of status line AC */
    return 0;
}

int
Cloak_off(VOID_ARGS)
{
    struct obj *otmp = uarmc;
    int otyp = otmp->otyp;
    long oldprop = u.uprops[objects[otyp].oc_oprop].extrinsic & ~WORN_CLOAK;
    Strcpy(debug_buf_4, "Cloak_off");

    if (flags.verbose && otmp)
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMC;
    /* For mummy wrapping, taking it off first resets `Invisible'. */
    setworn((struct obj *) 0, W_ARMC);
    context.takeoff.cancelled_don = FALSE;

    switch (otyp) {
    case ELVEN_CLOAK:
        toggle_stealth(otmp, oldprop, FALSE);
        break;
    case CLOAK_OF_DISPLACEMENT:
        toggle_displacement(otmp, oldprop, FALSE);
        break;
    }
    return 0;
}

STATIC_PTR
int
Helmet_on(VOID_ARGS)
{
    if (flags.verbose && uarmh)
        play_simple_object_sound(uarmh, OBJECT_SOUND_TYPE_WEAR);

    switch (uarmh->otyp) {
    case HELM_OF_OPPOSITE_ALIGNMENT:
        /* changing alignment can toggle off active artifact
           properties, including levitation; uarmh could get
           dropped or destroyed here */
        uchangealign((u.ualign.type != A_NEUTRAL)
                         ? -u.ualign.type
                         : (uarmh->o_id % 2) ? A_CHAOTIC : A_LAWFUL,
                     1);
    /* makeknown(uarmh->otyp);   -- moved below, after xname() */
        if (Hallucination) {
            pline("My brain hurts!"); /* Monty Python's Flying Circus */
        }
        else {
            makeknown(uarmh->otyp);
        }
        update_all_character_properties((struct obj*)0, TRUE);
        for (int i = 0; i <= 1; i++)
        {
            struct obj* wep = (i == 0 ? uwep : uarms);
            if (wep && (artifact_light(wep) || has_obj_mythic_magical_light(wep) || (obj_shines_magical_light(wep) && !inappropriate_monster_character_type(&youmonst, wep))) && !wep->lamplit)
            {
                begin_burn(wep, FALSE);
                if (!Blind)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s to shine %s!", Tobjnam(wep, "begin"),
                        arti_light_description(wep));
            }
        }
        break;
    case DUNCE_CAP:
        /*
        if (uarmh && !uarmh->cursed) {
            if (Blind)
                pline("%s for a moment.", Tobjnam(uarmh, "vibrate"));
            else
                pline("%s %s for a moment.", Tobjnam(uarmh, "glow"),
                      hcolor(NH_BLACK));
            curse(uarmh);
        }
        */
        //context.botl = 1; /* reveal new alignment or INT & WIS */
        if (Hallucination) {
            pline("My brain hurts!"); /* Monty Python's Flying Circus */
        } else {
            You_feel("%s.", /* track INT change; ignore WIS */
                     ACURR(A_INT)
                             <= (ABASE(A_INT) + ABONUS(A_INT) + ATEMP(A_INT))
                         ? "like sitting in a corner"
                         : "giddy");
        }
        break;
    }
    uarmh->known = 1; /* helmet's +/- evident because of status line AC */
    return 0;
}

int
Helmet_off(VOID_ARGS)
{
    if (!uarmh)
        return 0;

    Strcpy(debug_buf_4, "Helmet_off");

    if (flags.verbose && uarmh)
        play_simple_object_sound(uarmh, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMH;

    switch (uarmh->otyp) {
    case HELM_OF_OPPOSITE_ALIGNMENT:
        /* changing alignment can toggle off active artifact
           properties, including levitation; uarmh could get
           dropped or destroyed here */
        uchangealign(u.ualignbase[A_CURRENT], 2);
        break;
    }

    setworn((struct obj *) 0, W_ARMH);

    context.takeoff.cancelled_don = FALSE;
    return 0;
}

STATIC_PTR
int
Gloves_on(VOID_ARGS)
{
    if (!uarmg)
        return 0;

    if (flags.verbose && uarmg)
        play_simple_object_sound(uarmg, OBJECT_SOUND_TYPE_WEAR);

    uarmg->known = 1; /* gloves' +/- evident because of status line AC */
    return 0;
}

STATIC_OVL void
wielding_corpse(obj, voluntary)
struct obj *obj;
boolean voluntary; /* taking gloves off on purpose? */
{
    char kbuf[BUFSZ];

    if (!obj || obj->otyp != CORPSE)
        return;
    if (obj != uwep && obj != uarms)
        return;

    if (touch_petrifies(&mons[obj->corpsenm]) && !Stone_resistance) {
        You("now wield %s in your bare %s.",
            corpse_xname(obj, (const char *) 0, CXN_ARTICLE),
            makeplural(body_part(HAND)));
        Sprintf(kbuf, "%s gloves while wielding %s",
                voluntary ? "removing" : "losing", killer_xname(obj));
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
        instapetrify(kbuf);
        /* life-saved; can't continue wielding cockatrice corpse though */
        remove_worn_item(obj, FALSE);
    }
}

int
Gloves_off(VOID_ARGS)
{
    boolean on_purpose = !context.mon_moving && !uarmg->in_use;
    Strcpy(debug_buf_4, "Gloves_off");

    if (flags.verbose && uarmg)
        play_simple_object_sound(uarmg, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMG;

    setworn((struct obj*) 0, W_ARMG);

    context.takeoff.cancelled_don = FALSE;
    (void) encumber_msg(); /* immediate feedback for GoP */

    /* prevent wielding cockatrice when not wearing gloves */
    if (uwep && uwep->otyp == CORPSE)
        wielding_corpse(uwep, on_purpose);

    /* KMH -- ...or your secondary weapon when you're wielding it
       [This case can't actually happen; twoweapon mode won't
       engage if a corpse has been set up as the alternate weapon.] */
    if (uarms && uarms->otyp == CORPSE)
        wielding_corpse(uarms, on_purpose);

    return 0;
}

STATIC_PTR int
Shield_on(VOID_ARGS)
{
    if (!uarms)
        return 0;

    if (flags.verbose && uarms)
        play_simple_object_sound(uarms, OBJECT_SOUND_TYPE_WEAR);

    uarms->known = 1; /* shield's +/- evident because of status line AC */

    status_reassess();

    return 0;
}

int
Shield_off(VOID_ARGS)
{
    Strcpy(debug_buf_4, "Shield_off");
    if (flags.verbose && uarms)
        play_simple_object_sound(uarms, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMS;

    setworn((struct obj *) 0, W_ARMS);
    context.takeoff.cancelled_don = FALSE;
    status_reassess();

    return 0;
}

STATIC_PTR int
Shirt_on(VOID_ARGS)
{
    if (!uarmu)
        return 0;

    if (flags.verbose && uarmu)
        play_simple_object_sound(uarmu, OBJECT_SOUND_TYPE_WEAR);

    uarmu->known = 1; /* shirt's +/- evident because of status line AC */
    return 0;
}

int
Shirt_off(VOID_ARGS)
{
    Strcpy(debug_buf_4, "Shirt_off");
    if (flags.verbose && uarmu)
        play_simple_object_sound(uarmu, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMU;

    setworn((struct obj*) 0, W_ARMU);

    context.takeoff.cancelled_don = FALSE;
    return 0;
}

STATIC_PTR int
Robe_on(VOID_ARGS)
{
    if (!uarmo)
        return 0;

    if (flags.verbose && uarmo)
        play_simple_object_sound(uarmo, OBJECT_SOUND_TYPE_WEAR);

    uarmo->known = 1; /* shirt's +/- evident because of status line AC */
    return 0;
}

int
Robe_off(VOID_ARGS)
{
    Strcpy(debug_buf_4, "Robe_off");
    if (flags.verbose && uarmo)
        play_simple_object_sound(uarmo, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMO;
    setworn((struct obj*) 0, W_ARMO);
    context.takeoff.cancelled_don = FALSE;

    return 0;
}

STATIC_PTR int
Bracers_on(VOID_ARGS)
{
    if (!uarmb)
        return 0;

    if (flags.verbose && uarmb)
        play_simple_object_sound(uarmb, OBJECT_SOUND_TYPE_WEAR);

    uarmb->known = 1;
    return 0;
}

int
Bracers_off(VOID_ARGS)
{
    Strcpy(debug_buf_4, "Bracers_off");
    if (flags.verbose && uarmb)
        play_simple_object_sound(uarmb, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARMB;

    setworn((struct obj*) 0, W_ARMB);
    context.takeoff.cancelled_don = FALSE;
    return 0;
}

STATIC_PTR int
MiscellaneousItem_on(ud, wearslotmask)
struct obj* ud;
long wearslotmask;
{
    /* Wear slot mask added to find out the situation before wearing */
    if (!ud)
        return 0;

    Strcpy(debug_buf_4, "MiscellaneousItem_on");
    if (flags.verbose && ud)
        play_simple_object_sound(ud, OBJECT_SOUND_TYPE_WEAR);

    if (ud == uwep)
        setuwep((struct obj*) 0, W_WEP);
    else if (ud == uarms)
        setuwep((struct obj*) 0, W_WEP2);
    else if (ud == uswapwep)
        setuswapwep((struct obj*) 0, W_SWAPWEP);
    else if (ud == uswapwep2)
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
    else if (ud == uquiver)
        setuqwep((struct obj*) 0);

    if (objects[ud->otyp].oc_subtyp == MISC_IOUN_STONE)
    {
        pline("%s to the air and starts to orbit around your %s.", Tobjnam(ud, "rise"), body_part(HEAD));
    }

    if (wearslotmask != 0)
        setworn(ud, wearslotmask);

    if(ud->otyp == BELT_OF_CHANGE)
        item_change_sex_and_useup(ud);

    return 0;
}

int
MiscellaneousItem_off(ud)
struct obj* ud;
{
    if (!ud)
        return 0;

    Strcpy(debug_buf_4, "MiscellaneousItem_off");
    if (flags.verbose && ud)
        play_simple_object_sound(ud, OBJECT_SOUND_TYPE_TAKE_OFF);

    long bit = ud->owornmask & W_MISCITEMS;

    context.takeoff.mask &= ~bit;
    setworn((struct obj*) 0, bit);
    context.takeoff.cancelled_don = FALSE;

    if (objects[ud->otyp].oc_subtyp == MISC_IOUN_STONE)
    {
        pline("%s orbiting around your %s.", Tobjnam(ud, "stop"), body_part(HEAD));
    }

    return 0;
}

STATIC_PTR
int
Armor_on(VOID_ARGS)
{
    if (!uarm)
        return 0;

    Strcpy(debug_buf_4, "Armor_on");
    if (flags.verbose && uarm)
        play_simple_object_sound(uarm, OBJECT_SOUND_TYPE_WEAR);
    /*
     * No suits require special handling.  Special properties conferred by
     * suits are set up as intrinsics (actually 'extrinsics') by setworn()
     * which is called by armor_or_accessory_on() before Armor_on().
     */
    uarm->known = 1; /* suit's +/- evident because of status line AC */
    return 0;
}

int
Armor_off(VOID_ARGS)
{
    if (flags.verbose && uarm)
        play_simple_object_sound(uarm, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_ARM;
    setworn((struct obj *) 0, W_ARM);
    context.takeoff.cancelled_don = FALSE;
    return 0;
}

/* The gone functions differ from the off functions in that if you die from
 * taking it off and have life saving, you still die.  [Obsolete reference
 * to lack of fire resistance being fatal in hell (GnollHack 3.0) and life
 * saving putting a removed item back on to prevent that from immediately
 * repeating.]
 */
int
Armor_gone()
{
    context.takeoff.mask &= ~W_ARM;
    setnotworn(uarm);
    context.takeoff.cancelled_don = FALSE;
    return 0;
}

STATIC_OVL void
Amulet_on()
{
    if (!uamul)
        return;

    if (flags.verbose && uamul)
        play_simple_object_sound(uamul, OBJECT_SOUND_TYPE_WEAR);

    /* make sure amulet isn't wielded; can't use remove_worn_item()
       here because it has already been set worn in amulet slot */
    if (uamul == uwep)
        setuwep((struct obj *) 0, W_WEP);
    else if (uamul == uarms)
        setuwep((struct obj*) 0, W_WEP2);
    else if (uamul == uswapwep)
        setuswapwep((struct obj *) 0, W_SWAPWEP);
    else if (uamul == uswapwep2)
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
    else if (uamul == uquiver)
        setuqwep((struct obj *) 0);

    switch (uamul->otyp) {
    case AMULET_OF_UNCHANGING:
        if (Slimed)
            make_slimed(0L, (char *) 0, 0, (char*)0, 0);
        break;
    case AMULET_OF_CHANGE:
        item_change_sex_and_useup(uamul);
        break;
    case AMULET_OF_STRANGULATION:
        if (can_be_strangled(&youmonst)) {
            makeknown(AMULET_OF_STRANGULATION);
            Strangled = 6L;
            context.botl = context.botlx = TRUE;
            pline("It constricts your throat!");
        }
        break;
    }
    return;
}

void
Amulet_off()
{
    if (!uamul)
        return;

    Strcpy(debug_buf_4, "Amulet_off");
    if (flags.verbose && uamul)
        play_simple_object_sound(uamul, OBJECT_SOUND_TYPE_TAKE_OFF);

    context.takeoff.mask &= ~W_AMUL;

    switch (uamul->otyp) {
    case AMULET_OF_STRANGULATION:
        if (Strangled) {
            Strangled = 0L;
            context.botl = context.botlx = TRUE;
            if (Breathless)
                Your("%s is no longer constricted!", body_part(NECK));
            else
                You("can breathe more easily!");
        }
        break;
    }

    setworn((struct obj *) 0, W_AMUL);
    return;
}

STATIC_OVL
void
item_change_sex_and_useup(uitem)
struct obj* uitem;
{
    if (!uitem)
        return;

    char dcbuf[BUFSZ] = "";
    int orig_sex = poly_gender();

    if (Unchanging)
        return;
    change_sex();
    /* Don't use same message as polymorph */
    if (orig_sex != poly_gender()) {
        makeknown(uitem->otyp);
        play_sfx_sound(SFX_SEX_CHANGE);
        Sprintf(dcbuf, "You are suddenly very %s!",
            flags.female ? "feminine" : "masculine");
        pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
        context.botl = 1;
    }
    else
    {
        play_sfx_sound(SFX_UNKNOWN_ODD_EFFECT);
        /* already polymorphed into single-gender monster; only
           changed the character's base sex */
        Strcpy(dcbuf, "You don't feel like yourself.");
        pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);
    }
    play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s disintegrates!", The(cxname(uitem)));
    if (orig_sex == poly_gender() && uitem->dknown
        && !objects[uitem->otyp].oc_name_known
        && !objects[uitem->otyp].oc_uname)
        docall(uitem, dcbuf);
    useup(uitem);
    newsym(u.ux, u.uy);
}

/* handle ring discovery; comparable to learnwand() */
void
learnring(ring, observed)
struct obj *ring;
boolean observed;
{
    int ringtype = ring->otyp;

    /* if effect was observeable then we usually discover the type */
    if (observed) {
        /* if we already know the ring type which accomplishes this
           effect (assumes there is at most one type for each effect),
           mark this ring as having been seen (no need for makeknown);
           otherwise if we have seen this ring, discover its type */
        if (objects[ringtype].oc_name_known)
            ring->dknown = 1;
        else if (ring->dknown)
            makeknown(ringtype);
#if 0 /* see learnwand() */
        else
            ring->eknown = 1;
#endif

        /* make enchantment of charged ring known (might be +0) and update
       perm invent window if we've seen this ring and know its type */
        if (ring->dknown && objects[ringtype].oc_name_known) {
            if (objects[ringtype].oc_enchantable)
                ring->known = 1;
            update_inventory();
        }

    }

}

void
Ring_on(obj)
register struct obj *obj;
{
    if (!obj)
        return;

    if(flags.verbose && obj)
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_WEAR);

    long oldprop = u.uprops[objects[obj->otyp].oc_oprop].extrinsic;

    /* make sure ring isn't wielded; can't use remove_worn_item()
       here because it has already been set worn in a ring slot */
    if (obj == uwep)
        setuwep((struct obj *) 0, W_WEP);
    else if (obj == uarms)
        setuwep((struct obj*) 0, W_WEP2);
    else if (obj == uswapwep)
        setuswapwep((struct obj *) 0, W_SWAPWEP);
    else if (obj == uswapwep2)
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
    else if (obj == uquiver)
        setuqwep((struct obj *) 0);

    /* only mask out W_RING when we don't have both
       left and right rings of the same type */
    if ((oldprop & W_RING) != W_RING)
        oldprop &= ~W_RING;

    switch (obj->otyp) {
    case RIN_STEALTH:
        toggle_stealth(obj, oldprop, TRUE);
        break;

    case RIN_PROTECTION_FROM_SHAPE_CHANGERS:
        rescham();
        break;
    }

    return;
}

STATIC_OVL void
Ring_off_or_gone(obj, gone)
register struct obj *obj;
boolean gone;
{
    if (!obj)
        return;

    Strcpy(debug_buf_4, "Ring_off_or_gone");
    if (flags.verbose && obj && !gone)
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_TAKE_OFF);

    long mask = (obj->owornmask & W_RING);
    context.takeoff.mask &= ~mask;
    //if (!(u.uprops[objects[obj->otyp].oc_oprop].extrinsic & mask))
    //   impossible("Strange... I didn't know you had that ring.");
    if (gone)
        setnotworn(obj);
    else
        setworn((struct obj *) 0, obj->owornmask);

    switch (obj->otyp) {
    case RIN_STEALTH:
        toggle_stealth(obj, (EStealth & ~mask), FALSE);
        break;
    case RIN_PROTECTION_FROM_SHAPE_CHANGERS:
        /* If you're no longer protected, let the chameleons
         * change shape again -dgk
         */
        restartcham();
        break;
    }

    return;
}

void
Ring_off(obj)
struct obj *obj;
{
    Ring_off_or_gone(obj, FALSE);
}

void
Ring_gone(obj)
struct obj *obj;
{
    Ring_off_or_gone(obj, TRUE);
}

void
Blindf_on(otmp)
struct obj *otmp;
{
    Strcpy(debug_buf_4, "Blindf_on");
    /* blindfold might be wielded; release it for wearing */
    if (otmp->owornmask & W_WEAPON)
        remove_worn_item(otmp, FALSE);
    setworn(otmp, W_BLINDFOLD);
    on_msg(otmp);
}

void
Blindf_off(otmp)
struct obj *otmp;
{
    boolean was_blind = Blind;

    if (!otmp) {
        impossible("Blindf_off without otmp");
        return;
    }
    context.takeoff.mask &= ~W_BLINDFOLD;
    setworn((struct obj *) 0, otmp->owornmask);
    off_msg(otmp);

    if (Blind) {
        if (was_blind) {
            /* "still cannot see" makes no sense when removing lenses
               since they can't have been the cause of your blindness */
            You_ex(ATR_NONE, CLR_MSG_WARNING, "still cannot see.");
        }
    }
}

/* called in moveloop()'s prologue to set side-effects of worn start-up items;
   also used by poly_obj() when a worn item gets transformed */
void
set_wear(obj)
struct obj *obj; /* if null, do all worn items; otherwise just obj itself */
{
    initial_don = !obj;

    if (!obj ? ublindf != 0 : (obj == ublindf))
        (void) Blindf_on(ublindf);
    if (!obj ? uright != 0 : (obj == uright))
        (void) Ring_on(uright);
    if (!obj ? uleft != 0 : (obj == uleft))
        (void) Ring_on(uleft);
    if (!obj ? uamul != 0 : (obj == uamul))
        (void) Amulet_on();
    if (!obj ? umisc != 0 : (obj == umisc))
        (void)MiscellaneousItem_on(umisc, 0);
    if (!obj ? umisc2 != 0 : (obj == umisc2))
        (void)MiscellaneousItem_on(umisc2, 0);
    if (!obj ? umisc3 != 0 : (obj == umisc3))
        (void)MiscellaneousItem_on(umisc3, 0);
    if (!obj ? umisc4 != 0 : (obj == umisc4))
        (void)MiscellaneousItem_on(umisc4, 0);
    if (!obj ? umisc5 != 0 : (obj == umisc5))
        (void)MiscellaneousItem_on(umisc5, 0);

    if (!obj ? uarmu != 0 : (obj == uarmu))
        (void) Shirt_on();
    if (!obj ? uarm != 0 : (obj == uarm))
        (void) Armor_on();
    if (!obj ? uarmo != 0 : (obj == uarmo))
        (void) Robe_on();
    if (!obj ? uarmc != 0 : (obj == uarmc))
        (void) Cloak_on();
    if (!obj ? uarmf != 0 : (obj == uarmf))
        (void) Boots_on();
    if (!obj ? uarmb != 0 : (obj == uarmb))
        (void) Bracers_on();
    if (!obj ? uarmg != 0 : (obj == uarmg))
        (void) Gloves_on();
    if (!obj ? uarmh != 0 : (obj == uarmh))
        (void) Helmet_on();
    if (!obj ? uarms != 0 : (obj == uarms && obj->oclass == ARMOR_CLASS && objects[obj->otyp].oc_armor_category == ARM_SHIELD))
        (void) Shield_on();

    initial_don = FALSE;
}

/* check whether the target object is currently being put on (or taken off--
   also checks for doffing--[why?]) */
boolean
donning(otmp)
struct obj *otmp;
{
    boolean result = FALSE;

    /* 'W' (or 'P' used for armor) sets afternmv */
    if (doffing(otmp))
        result = TRUE;
    else if (otmp == uarm)
        result = (afternmv == Armor_on);
    else if (otmp == uarmu)
        result = (afternmv == Shirt_on);
    else if (otmp == uarmo)
        result = (afternmv == Robe_on);
    else if (otmp == uarmc)
        result = (afternmv == Cloak_on);
    else if (otmp == uarmf)
        result = (afternmv == Boots_on);
    else if (otmp == uarmh)
        result = (afternmv == Helmet_on);
    else if (otmp == uarmb)
        result = (afternmv == Bracers_on);
    else if (otmp == uarmg)
        result = (afternmv == Gloves_on);
    else if (otmp == uarms && otmp->oclass == ARMOR_CLASS && objects[otmp->otyp].oc_armor_category == ARM_SHIELD)
        result = (afternmv == Shield_on);

    return result;
}

/* check whether the target object is currently being taken off,
   so that stop_donning() and steal() can vary messages and doname()
   can vary "(being worn)" suffix */
boolean
doffing(otmp)
struct obj *otmp;
{
    long what = context.takeoff.what;
    boolean result = FALSE;

    /* 'T' (or 'R' used for armor) sets afternmv, 'A' sets takeoff.what */
    if (otmp == uarm)
        result = (afternmv == Armor_off || what == WORN_ARMOR);
    else if (otmp == uarmu)
        result = (afternmv == Shirt_off || what == WORN_SHIRT);
    else if (otmp == uarmo)
        result = (afternmv == Robe_off || what == WORN_ROBE);
    else if (otmp == uarmc)
        result = (afternmv == Cloak_off || what == WORN_CLOAK);
    else if (otmp == uarmf)
        result = (afternmv == Boots_off || what == WORN_BOOTS);
    else if (otmp == uarmh)
        result = (afternmv == Helmet_off || what == WORN_HELMET);
    else if (otmp == uarmb)
        result = (afternmv == Bracers_off || what == WORN_BRACERS);
    else if (otmp == uarmg)
        result = (afternmv == Gloves_off || what == WORN_GLOVES);
    else if (otmp == uarms && is_shield(otmp))
        result = (afternmv == Shield_off || what == WORN_SHIELD);
    /* these 1-turn items don't need 'afternmv' checks */
    else if (otmp == uamul)
        result = (what == WORN_AMUL);
    else if (otmp == umisc)
        result = (what == WORN_MISCELLANEOUS);
    else if (otmp == umisc2)
        result = (what == WORN_MISCELLANEOUS2);
    else if (otmp == umisc3)
        result = (what == WORN_MISCELLANEOUS3);
    else if (otmp == umisc4)
        result = (what == WORN_MISCELLANEOUS4);
    else if (otmp == umisc5)
        result = (what == WORN_MISCELLANEOUS5);
    else if (otmp == uleft)
        result = (what == LEFT_RING);
    else if (otmp == uright)
        result = (what == RIGHT_RING);
    else if (otmp == ublindf)
        result = (what == WORN_BLINDF);
    else if (otmp == uwep)
        result = (what == W_WEP);
    else if (otmp == uarms)
        result = (what == W_WEP2);
    else if (otmp == uswapwep)
        result = (what == W_SWAPWEP);
    else if (otmp == uswapwep2)
        result = (what == W_SWAPWEP2);
    else if (otmp == uquiver)
        result = (what == W_QUIVER);

    return result;
}

/* despite their names, cancel_don() and cancel_doff() both apply to both
   donning and doffing... */
void
cancel_doff(obj, slotmask)
struct obj *obj;
long slotmask;
{
    /* Called by setworn() for old item in specified slot or by setnotworn()
     * for specified item.  We don't want to call cancel_don() if we got
     * here via <X>_off() -> setworn((struct obj *)0) -> cancel_doff()
     * because that would stop the 'A' command from continuing with next
     * selected item.  So do_takeoff() sets a flag in takeoff.mask for us.
     * [For taking off an individual item with 'T'/'R'/'w-', it doesn't
     * matter whether cancel_don() gets called here--the item has already
     * been removed by now.]
     */
    if (!(context.takeoff.mask & I_SPECIAL) && donning(obj))
        cancel_don(); /* applies to doffing too */
    context.takeoff.mask &= ~slotmask;
}

/* despite their names, cancel_don() and cancel_doff() both apply to both
   donning and doffing... */
void
cancel_don()
{
    /* the piece of armor we were donning/doffing has vanished, so stop
     * wasting time on it (and don't dereference it when donning would
     * otherwise finish)
     */
    context.takeoff.cancelled_don =
        (afternmv == Boots_on || afternmv == Helmet_on
         || afternmv == Gloves_on || afternmv == Armor_on);
    afternmv = (int NDECL((*))) 0;
    nomovemsg = (char *) 0;
    multi = 0;
    context.takeoff.delay = 0;
    context.takeoff.what = 0L;
    context.wear.delay = 0;
    context.wear.what = 0L;
}

/* called by steal() during theft from hero; interrupt donning/doffing */
int
stop_donning(stolenobj)
struct obj *stolenobj; /* no message if stolenobj is already being doffing */
{
    char buf[BUFSZ];
    struct obj *otmp;
    boolean putting_on;
    int result = 0;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if ((otmp->owornmask & W_ARMOR) && donning(otmp))
            break;
    /* at most one item will pass donning() test at any given time */
    if (!otmp)
        return 0;

    /* donning() returns True when doffing too; doffing() is more specific */
    putting_on = !doffing(otmp);
    /* cancel_don() looks at afternmv; it can also cancel doffing */
    cancel_don();
    /* don't want <armor>_on() or <armor>_off() being called
       by unmul() since the on or off action isn't completing */
    afternmv = (int NDECL((*))) 0;
    if (putting_on || otmp != stolenobj) {
        Sprintf(buf, "You stop %s %s.",
                putting_on ? "putting on" : "taking off",
                thesimpleoname(otmp));
    } else {
        buf[0] = '\0';   /* silently stop doffing stolenobj */
        result = -multi; /* remember this before calling unmul() */
    }
    unmul(buf);
    /* while putting on, item becomes worn immediately but side-effects are
       deferred until the delay expires; when interrupted, make it unworn
       (while taking off, item stays worn until the delay expires; when
       interrupted, leave it worn) */
    if (putting_on)
        remove_worn_item(otmp, FALSE);

    return result;
}

/* both 'clothes' and 'accessories' now include both armor and accessories;
   TOOL_CLASS is for eyewear, FOOD_CLASS is for MEAT_RING */
STATIC_VAR NEARDATA const char clothes[] = {
    ARMOR_CLASS, RING_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, TOOL_CLASS, FOOD_CLASS, 0
};
STATIC_VAR NEARDATA const char accessories[] = {
    RING_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, TOOL_CLASS, FOOD_CLASS, ARMOR_CLASS, 0
};
STATIC_VAR NEARDATA int Narmorpieces, Naccessories;

/* assign values to Narmorpieces and Naccessories */
STATIC_OVL void
count_worn_stuff(which, accessorizing)
struct obj **which; /* caller wants this when count is 1 */
boolean accessorizing;
{
    struct obj *otmp;

    Narmorpieces = Naccessories = 0;

#define MOREWORN(x,wtyp) do { if (x) { wtyp++; otmp = x; } } while (0)
    otmp = 0;
    MOREWORN(uarmh, Narmorpieces);
    MOREWORN(uarms, Narmorpieces);
    MOREWORN(uarmg, Narmorpieces);
    MOREWORN(uarmb, Narmorpieces);
    MOREWORN(umisc, Narmorpieces);
    MOREWORN(uarmf, Narmorpieces);
    /* for cloak/suit/shirt, we only count the outermost item so that it
       can be taken off without confirmation if final count ends up as 1 */
    if (uarmc)
        MOREWORN(uarmc, Narmorpieces);
    else if (uarmo)
        MOREWORN(uarmo, Narmorpieces);
    else if (uarm)
        MOREWORN(uarm, Narmorpieces);
    else if (uarmu)
        MOREWORN(uarmu, Narmorpieces);
    if (!accessorizing)
        *which = otmp; /* default item iff Narmorpieces is 1 */

    otmp = 0;
    MOREWORN(uleft, Naccessories);
    MOREWORN(uright, Naccessories);
    MOREWORN(uamul, Naccessories);
    MOREWORN(umisc, Naccessories);
    MOREWORN(umisc2, Naccessories);
    MOREWORN(umisc3, Naccessories);
    MOREWORN(umisc4, Naccessories);
    MOREWORN(umisc5, Naccessories);
    MOREWORN(ublindf, Naccessories);
    if (accessorizing)
        *which = otmp; /* default item iff Naccessories is 1 */
#undef MOREWORN
}

/* take off one piece or armor or one accessory;
   shared by dotakeoff('T') and doremring('R') */
int
armor_or_accessory_off(obj)
struct obj *obj;
{
    if (!(obj->owornmask & (W_ARMOR | W_ACCESSORY))) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are not wearing that.");
        return 0;
    }
    if (obj == uskin || is_armor_covered(obj))
    {
        char why[QBUFSZ], what[QBUFSZ];

        why[0] = what[0] = '\0';
        if (obj != uskin) 
        {
            print_covering_items(obj, what);
            Sprintf(why, " without taking off your %s first", what);
        } 
        else 
        {
            Strcpy(why, "; it's embedded");
        }
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "take that off%s.", why);
        return 0;
    }

    reset_remarm(); /* clear context.takeoff.mask and context.takeoff.what */
    (void) select_off(obj);
    if (!context.takeoff.mask)
        return 0;
    /* none of armoroff()/Ring_/Amulet/Blindf_off() use context.takeoff.mask */
    reset_remarm();

    if (obj->owornmask & W_ARMS)
    {
        if (is_shield(obj))
            (void)armoroff(obj);
        else
            remove_worn_item(obj, FALSE);
    } 
    else if (obj->owornmask & W_ARMOR) 
    {
        (void) armoroff(obj);
    }
    else if (obj == uright || obj == uleft) {
        /* Sometimes we want to give the off_msg before removing and
         * sometimes after; for instance, "you were wearing a moonstone
         * ring (on right hand)" is desired but "you were wearing a
         * square amulet (being worn)" is not because of the redundant
         * "being worn".
         */
        off_msg(obj);
        Ring_off(obj);
    } else if (obj == uamul) {
        Amulet_off();
        off_msg(obj);
    } else if (obj == umisc || obj == umisc2 || obj == umisc3 || obj == umisc4 || obj == umisc5) {
        MiscellaneousItem_off(obj);
        off_msg(obj);
    } else if (obj == ublindf) {
        Blindf_off(obj); /* does its own off_msg */
    } else {
        impossible("removing strange accessory?");
        if (obj->owornmask)
            remove_worn_item(obj, FALSE);
    }

    return 1;
}


/* the 'T' command */
int
dotakeoff()
{
    struct obj *otmp = (struct obj *) 0;

    count_worn_stuff(&otmp, FALSE);
    if (!Narmorpieces && !Naccessories) {
        /* assert( GRAY_DRAGON_SCALES > YELLOW_DRAGON_SCALE_MAIL ); */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        if (uskin)
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "%s merged with your skin!",
                      uskin->otyp >= GRAY_DRAGON_SCALES
                          ? "dragon scales are"
                          : "dragon scale mail is");
        else
            pline("Not wearing any armor or accessories.");
        return 0;
    }
    if (Narmorpieces != 1 || ParanoidRemove)
        otmp = getobj(clothes, "take off", 0, "");
    if (!otmp)
        return 0;

    if (iflags.takeoff_uses_all)
    {
        if (context.takeoff.command == TAKEOFF_WEAR_CMD_TAKEOFF && context.takeoff.mask && context.takeoff.mask == otmp->owornmask)
            return doddoremarm();

        if (!(otmp->owornmask & (W_ARMOR | W_ACCESSORY))) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex1(ATR_NONE, CLR_MSG_FAIL, "are not wearing that.");
            return 0;
        }
        else if (otmp == uskin)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_cant_ex1(ATR_NONE, CLR_MSG_FAIL, "take that off; it's embedded.");
            return 0;
        }

        if (is_armor_covered(otmp))
        {
            char what[BUFSZ] = "", why[BUFSZ], qbuf[BUFSZ * 2], tbuf[BUFSZ];
            print_covering_items(otmp, what);
            Sprintf(why, " without taking off your %s first", what);
            Sprintf(qbuf, "You cannot take %s off%s. Proceed with disrobing?", the(cxname(otmp)), why);
            Strcpy(tbuf, "Disrobe Covering Items");
            char ans = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, tbuf, qbuf, ynchars, 'n', yndescs, (const char*)0);
            if (ans != 'y')
                return 0;
        }
        reset_remarm();
        (void)select_off(otmp);
        activate_take_off();
        return 0;
    }
    else
        return armor_or_accessory_off(otmp);
}

/* the 'R' command */
int
doremring()
{
    struct obj *otmp = 0;

    count_worn_stuff(&otmp, TRUE);
    if (!Naccessories && !Narmorpieces) {
        pline("Not wearing any accessories or armor.");
        return 0;
    }
    if (Naccessories != 1 || ParanoidRemove)
        otmp = getobj(accessories, "remove", 0, "");
    if (!otmp)
        return 0;

    return armor_or_accessory_off(otmp);
}

/* Check if something worn is cursed _and_ unremovable. */
int
cursed(otmp)
register struct obj *otmp;
{
    if (!otmp) {
        impossible("cursed without otmp");
        return 0;
    }
    /* Curses, like chickens, come home to roost. */
    if ((otmp == uwep) ? welded(otmp, &youmonst) : (int) otmp->cursed && !cursed_items_are_positive(youmonst.data))
    {
        boolean use_plural = (is_boots(otmp) || is_gloves(otmp) || is_bracers(otmp)
                               || otmp->quan > 1L);

        play_sfx_sound(SFX_GENERAL_WELDED);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "can't.  %s cursed.", use_plural ? "They are" : "It is");
        if (!otmp->bknown)
        {
            otmp->bknown = TRUE;
            update_inventory();
        }
        return 1;
    }
    return 0;
}

int
armoroff(otmp)
register struct obj *otmp;
{
    register int delay = -objects[otmp->otyp].oc_delay;
    Strcpy(debug_buf_4, "armoroff");

    if (cursed(otmp))
        return 0;
    if (delay) {
        nomul(delay);
        multi_reason = "disrobing";
        if (is_helmet(otmp)) {
            /* ick... */
            nomovemsg = !strcmp(helm_simple_name(otmp), "hat")
                            ? "You finish taking off your hat."
                            : "You finish taking off your helmet.";
            afternmv = Helmet_off;
        } else if (is_gloves(otmp)) {
            nomovemsg = "You finish taking off your gloves.";
            afternmv = Gloves_off;
        }
        else if (is_bracers(otmp)) {
            nomovemsg = "You finish taking off your bracers.";
            afternmv = Bracers_off;
        }
        else if (is_shirt(otmp)) {
            nomovemsg = "You finish taking off your shirt.";
            afternmv = Shirt_off;
        }
        else if (is_boots(otmp)) {
            nomovemsg = "You finish taking off your boots.";
            afternmv = Boots_off;
        }
        else if (is_cloak(otmp)) { //Apparently supposed to happen without delay, here just in case
            nomovemsg = "You finish taking off your cloak.";
            afternmv = Cloak_off;
        }
        else if (is_robe(otmp)) {
            nomovemsg = "You finish taking off your robe.";
            afternmv = Robe_off;
        }
        else if (is_shirt(otmp)) {
            nomovemsg = "You finish taking off your robe.";
            afternmv = Shirt_off;
        }
        else if (is_shield(otmp)) {
            nomovemsg = "You finish taking off your shield.";
            afternmv = Shield_off;
        } else {
            nomovemsg = "You finish taking off your suit.";
            afternmv = Armor_off;
        }
    } else {
        /* Be warned!  We want off_msg after removing the item to
         * avoid "You were wearing ____ (being worn)."  However, an
         * item which grants fire resistance might cause some trouble
         * if removed in Hell and lifesaving puts it back on; in this
         * case the message will be printed at the wrong time (after
         * the messages saying you died and were lifesaved).  Luckily,
         * no cloak, shield, or fast-removable armor grants fire
         * resistance, so we can safely do the off_msg afterwards.
         * Rings do grant fire resistance, but for rings we want the
         * off_msg before removal anyway so there's no problem.  Take
         * care in adding armors granting fire resistance; this code
         * might need modification.
         * 3.2 (actually 3.1 even): that comment is obsolete since
         * fire resistance is not required for Gehennom so setworn()
         * doesn't force the resistance granting item to be re-worn
         * after being lifesaved anymore.
         */
        if (is_cloak(otmp))
            (void) Cloak_off();
        else if (is_gloves(otmp)) //here just in case is instantaneous
            (void)Gloves_off();
        else if (is_boots(otmp)) //here just in case is instantaneous
            (void)Boots_off();
        else if (is_helmet(otmp)) //here just in case is instantaneous
            (void)Helmet_off();
        else if (is_robe(otmp)) //here just in case is instantaneous
            (void)Robe_off();
        else if (is_bracers(otmp)) //here just in case is instantaneous
            (void) Bracers_off();
        else if (is_shirt(otmp)) //here just in case is instantaneous
            (void)Shirt_off();
        else if (is_shield(otmp))
            (void) Shield_off();
        else if (is_suit(otmp))
            (void) Armor_off();
        else
            setworn((struct obj *) 0, otmp->owornmask & W_ARMOR);
        off_msg(otmp);
    }
    context.takeoff.mask = context.takeoff.what = context.wear.mask = context.wear.what = 0L;
    return 1;
}

STATIC_OVL void
already_wearing(cc)
const char *cc;
{
    (void)already_wearing_with_exchange_prompt(cc, (struct obj*)0, (struct obj*)0);
}

STATIC_OVL int
already_wearing_with_exchange_prompt(cc, obj, curobj)
const char* cc;
struct obj* obj, *curobj;
{
    if (flags.exchange_prompt && obj && curobj && curobj->owornmask && !(obj->owornmask & W_ARMOR))
    {
        char abuf[BUFSZ] = "";
        char classbuf[BUFSZ] = "";
        if(curobj->oclass == MISCELLANEOUS_CLASS)
            (void)strcpy_capitalized_for_title(classbuf, misc_type_names[objects[obj->otyp].oc_subtyp]);
        Sprintf(abuf, "%s", curobj->oclass == MISCELLANEOUS_CLASS ? classbuf : curobj->oclass == AMULET_CLASS ? "amulet" : curobj->oclass == ARMOR_CLASS ? armor_class_simple_name(curobj) : "item");
        *abuf = highc(*abuf);

        char tbuf[BUFSZ] = "";
        Sprintf(tbuf, "Already Wearing %s", abuf);

        char qbuf[BUFSIZ] = "";
        Sprintf(qbuf, "You are currently wearing %s. Exchange it for %s?", acxname(curobj), the(cxname(obj)));
        char ans = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, tbuf, qbuf, ynchars, 'n', yndescs, (const char*)0);
        if (ans == 'y')
            return exchange_worn_item(obj, curobj, curobj->owornmask);
    }
    else
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are already wearing %s%c", cc, (cc == c_that_) ? '!' : '.');
    }

    return 0;
}

STATIC_OVL void
activate_take_off(VOID_ARGS)
{
    if (context.takeoff.mask)
    {
        context.takeoff.command = TAKEOFF_WEAR_CMD_TAKEOFF;

        /* default activity for armor and/or accessories,
           possibly combined with weapons */
        (void)strncpy(context.takeoff.disrobing, "disrobing", CONTEXTVERBSZ);
        /* specific activity when handling weapons only */
        if (!(context.takeoff.mask & ~W_WEAPON))
            (void)strncpy(context.takeoff.disrobing, "disarming",
                CONTEXTVERBSZ);
        (void)take_off();
    }
}

STATIC_OVL int
exchange_worn_item(obj, curobj, mask)
struct obj* obj, *curobj;
long mask;
{
    if (!obj)
        return 0;

    if (curobj)
        mask = curobj->owornmask; /* Override */

    int takeoffcmd = curobj ? TAKEOFF_WEAR_CMD_EXCHANGE : TAKEOFF_WEAR_CMD_TAKEOFF_AND_WEAR;
    boolean contoccupation = FALSE;
    if (context.takeoff.command == takeoffcmd && (context.wear.what || context.wear.mask))
    {
        long combinedmask = context.wear.what | context.wear.mask;
        if (combinedmask & mask) /* Some object is being worn in the same slot */
        {
            int idx = -1, i;
            for (i = 0; i < WEAR_OID_BITS; i++)
                if ((mask & (1L << i)) != 0)
                {
                    idx = i;
                    break;
                }

            if (idx >= 0 && context.wear.oid[idx] == obj->o_id) /* It is the same object */
                contoccupation = TRUE;
        }
    }


    if (contoccupation)
    {
        You("continue %s.", context.takeoff.disrobing);
        set_occupation(take_off, context.takeoff.disrobing, occsoundset, occtyp, OCCUPATION_SOUND_TYPE_RESUME, 0);
        return 0;
    }
    else
    {
        reset_remarm();
    }

    if (curobj && !curobj->owornmask) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s not worn.", Tobjnam(curobj, "are"));
        return 0;
    }
    else if (obj->owornmask & W_ARMOR) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s already worn.", Tobjnam(obj, "are"));
        return 0;
    }

    if (curobj)
    {
        (void)select_off(curobj);
        if (context.takeoff.mask == 0L)
            return 0;

        context.takeoff.command = TAKEOFF_WEAR_CMD_EXCHANGE;
        context.wear.mask = context.takeoff.mask;
        (void)add_wear_oid(obj, context.wear.mask);
    }
    else
    {
        context.takeoff.command = TAKEOFF_WEAR_CMD_TAKEOFF_AND_WEAR;
        context.wear.mask = mask;
        (void)add_wear_oid(obj, mask);
    }

    (void)add_wear_oid(obj, context.wear.mask);
    if ((mask & (W_ARMU | W_ARM | W_ARMO)) && uarmc)
    {
        select_off(uarmc);
        context.wear.mask |= W_ARMC;
        (void)add_wear_oid(uarmc, W_ARMC);
    }
    if ((mask & (W_ARMU | W_ARM)) && uarmo)
    {
        select_off(uarmo);
        context.wear.mask |= W_ARMO;
        (void)add_wear_oid(uarmo, W_ARMO);
    }
    if ((mask & (W_ARMU)) && uarm)
    {
        select_off(uarm);
        context.wear.mask |= W_ARM;
        (void)add_wear_oid(uarm, W_ARM);
    }

    if (context.takeoff.mask && context.wear.mask) {
        /* default activity for armor and/or accessories,
           possibly combined with weapons */
        (void)strncpy(context.takeoff.disrobing, "adjusting your equipment", CONTEXTVERBSZ);
        /* specific activity when handling weapons only */
        if (!(context.takeoff.mask & ~W_WEAPON))
            (void)strncpy(context.takeoff.disrobing, "rearming",
                CONTEXTVERBSZ);
        (void)take_off();
    }
    /* The time to perform the command is already completely accounted for
     * in take_off(); if we return 1, that would add an extra turn to each
     * disrobe.
     */
    return 0;
}

STATIC_OVL int
take_off_covering_and_wear(obj, mask)
struct obj* obj;
long mask;
{
    return exchange_worn_item(obj, (struct obj*)0, mask);
}

STATIC_OVL
int
add_wear_oid(obj, bit)
struct obj* obj;
long bit;
{
    int idx = -1, i;
    for (i = 0; i < WEAR_OID_BITS; i++)
        if ((bit & (1L << i)) != 0) /* Take the first bit; there should be only one */
        {
            idx = i;
            break;
        }

    if (idx < 0)
        return 0;

    context.wear.oid[idx] = obj->o_id;

    return 1;
}


/*
 * canwearobj checks to see whether the player can wear a piece of armor
 *
 * inputs: otmp (the piece of armor)
 *         noisy (if TRUE give error messages, otherwise be quiet about it)
 * output: mask (otmp's armor type)
 */
int
canwearobj(otmp, mask, noisy, constructing_letters)
struct obj *otmp;
long *mask;
boolean noisy;
boolean constructing_letters;
{
    int err = 0;
    const char *which;

    /* this is the same check as for 'W' (dowear), but different message,
       in case we get here via 'P' (doputon) */
    if (!can_operate_objects(youmonst.data))
    {
        if (noisy)
        {
            play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "can't wear any armor in your current form.");
        }
        return 0;
    }

    which = is_cloak(otmp)
                ? c_cloak
                : is_shirt(otmp)
                    ? c_shirt
                    : is_robe(otmp)
                        ? c_robe
                        : is_suit(otmp)
                            ? c_suit
                            : 0;
    if (which && cantweararm(youmonst.data)
        /* same exception for cloaks as used in m_dowear() */
        && (which != c_cloak || youmonst.data->msize != MZ_SMALL)
        && (racial_exception(&youmonst, otmp) < 1)) {
        if (noisy)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s will not fit on your body.", which);
        }
        return 0;
    } else if (otmp->owornmask & W_ARMOR) {
        if (noisy)
            already_wearing(c_that_);
        return 0;
    }

    if (welded(uwep, &youmonst) && bimanual(uwep) && (is_suit(otmp) || is_robe(otmp) || is_shirt(otmp))) {
        if (noisy)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot do that while holding your %s.",
                is_sword(uwep) ? c_sword : c_weapon);
        }
        return 0;
    }

    if (is_helmet(otmp)) 
    {
        if (uarmh && !constructing_letters)
        {
            if (noisy)
            {
                (void)already_wearing_with_exchange_prompt(an(helm_simple_name(uarmh)), otmp, uarmh);
            }

            err++;
        } 
        else if (Upolyd && has_horns(youmonst.data) && !is_flimsy(otmp)) 
        {
            /* (flimsy exception matches polyself handling) */
            if (noisy)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s won't fit over your horn%s.",
                    helm_simple_name(otmp),
                    plur(num_horns(youmonst.data)));
            }
            err++;
        }
        else if (Upolyd && !has_place_to_put_helmet_on(youmonst.data)) 
        {
            if (noisy)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "have no place to put your %s on.",
                    helm_simple_name(otmp));
            }
            err++;
        } else
            *mask = W_ARMH;
    } 
    else if (is_shield(otmp)) 
    {
        if (uarms && !constructing_letters)
        {
            if (noisy)
            {
                if (is_shield(uarms))
                {
                    (void)already_wearing_with_exchange_prompt(an(c_shield), otmp, uarms);
                }
                else
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "are already holding something else in your left %s.", body_part(HAND));
                }
            }

            err++;
        } 
        else if (uwep && bimanual(uwep)) 
        {
            if (noisy)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wear a shield while wielding a two-handed %s.",
                    is_sword(uwep) ? c_sword : (uwep->otyp == BATTLE_AXE)
                    ? c_axe
                    : c_weapon);
            }
            err++;
        } else
            *mask = W_ARMS;
    } 
    else if (is_boots(otmp)) 
    {
        if (uarmf && !constructing_letters)
        {
            if (noisy)
                (void)already_wearing_with_exchange_prompt(c_boots, otmp, uarmf);

            err++;
        } 
        else if (Upolyd && slithy(youmonst.data)) 
        {
            if (noisy)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "have no feet..."); /* not body_part(FOOT) */
            }
            err++;
        } 
        else if (Upolyd && youmonst.data->mlet == S_CENTAUR) 
        {
            /* break_armor() pushes boots off for centaurs,
               so don't let dowear() put them back on... */
            if (noisy)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                pline_ex(ATR_NONE, CLR_MSG_FAIL, "You have too many hooves to wear %s.",
                    c_boots); /* makeplural(body_part(FOOT)) yields
                                 "rear hooves" which sounds odd */
            }
            err++;
        }
        else if (u.utrap
                   && (u.utraptype == TT_BEARTRAP || u.utraptype == TT_INFLOOR
                       || u.utraptype == TT_LAVA
                       || u.utraptype == TT_BURIEDBALL)) 
        {
            if (u.utraptype == TT_BEARTRAP)
            {
                if (noisy)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s is trapped!", body_part(FOOT));
                }
            }
            else if (u.utraptype == TT_INFLOOR || u.utraptype == TT_LAVA) 
            {
                if (noisy)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s are stuck in the %s!",
                        makeplural(body_part(FOOT)), surface(u.ux, u.uy));
                }
            }
            else { /*TT_BURIEDBALL*/
                if (noisy)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s is attached to the buried ball!",
                        body_part(LEG));
                }
            }
            err++;
        } else
            *mask = W_ARMF;
    }
    else if (is_gloves(otmp)) 
    {
        if (uarmg && !constructing_letters)
        {
            if (noisy)
                (void)!already_wearing_with_exchange_prompt(c_gloves, otmp, uarmg);
            err++;
        }
        else if (welded(uwep, &youmonst)) 
        {
            if (noisy)
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wear gloves over your %s.",
                    is_sword(uwep) ? c_sword : c_weapon);
            }
            err++;
        } else
            *mask = W_ARMG;
    }
    else if (is_bracers(otmp)) 
    {
        if (uarmb && !constructing_letters)
        {
            if (noisy)
                (void)already_wearing_with_exchange_prompt(c_bracers, otmp, uarmb);

            err++;
        }
        else
            *mask = W_ARMB;
    } 
    else if (is_shirt(otmp)) 
    {
        if ((uarm || uarmc || uarmu || uarmo) && !constructing_letters)
        {
            if (uarmu)
            {
                if (noisy)
                    (void)already_wearing_with_exchange_prompt(an(c_shirt), otmp, uarmu);
            } 
            else 
            {
                if (noisy)
                {
                    if (flags.exchange_prompt)
                    {
                        char cbuf[BUFSZ] = "your piece of armor";
                        boolean many = FALSE;
                        if (uarm && uarmo && uarmc)
                        {
                            many = TRUE;
                            Sprintf(cbuf, "your armor, %s, and %s", robe_simple_name(uarmo), cloak_simple_name(uarmc));
                        }
                        else if (uarm && uarmc)
                        {
                            many = TRUE;
                            Sprintf(cbuf, "your armor and %s", cloak_simple_name(uarmc));
                        }
                        else if (uarm && uarmo)
                        {
                            many = TRUE;
                            Sprintf(cbuf, "your armor and %s", robe_simple_name(uarmo));
                        }
                        else if (uarmo && uarmc)
                        {
                            many = TRUE;
                            Sprintf(cbuf, "your %s and %s", cloak_simple_name(uarmc), robe_simple_name(uarmo));
                        }
                        else if (uarm)
                            Sprintf(cbuf, "%s", yname(uarm));
                        else if (uarmo)
                            Sprintf(cbuf, "%s", yname(uarmo));
                        else if (uarmc)
                            Sprintf(cbuf, "%s", yname(uarmc));
                        
                        char tbuf[BUFSZ];
                        Sprintf(tbuf, "Cannot Wear Shirt Over %s", many ? "Covering Armor" : uarm ? "Suit" : uarmo ? "Robe" : "Cloak");

                        char qbuf[BUFSIZ];
                        Sprintf(qbuf, "You cannot wear %s over %s. Take %s off and then wear the shirt?", acxname(otmp), cbuf, many ? "them" : "it");
                        char ans = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, tbuf, qbuf, ynchars, 'n', yndescs, (const char*)0);

                        if (ans == 'y')
                        {
                            (void)take_off_covering_and_wear(otmp, W_ARMU);
                        }
                    }
                    else
                    {
                        play_sfx_sound(SFX_GENERAL_CANNOT);
                        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "wear that over your %s.",
                            (uarmc) ? cloak_simple_name(uarmc)
                            : (uarmo) ? robe_simple_name(uarmo) : c_armor);
                    }
                }
            }
            err++;
        } else
            *mask = W_ARMU;
    }
    else if (is_robe(otmp)) 
{
        if ((uarmc || uarmo) && !constructing_letters)
        {
            if (uarmo) 
            {
                if (noisy)
                    (void)already_wearing_with_exchange_prompt(an(c_robe), otmp, uarmo);
            }
            else 
            {
                if (noisy)
                {
                    if (flags.exchange_prompt)
                    {
                        char cbuf[BUFSZ];
                        Sprintf(cbuf, "%s", yname(uarmc));

                        char tbuf[BUFSZ];
                        Sprintf(tbuf, "%s", "Cannot Wear Robe Over Cloak");

                        char qbuf[BUFSIZ];
                        Sprintf(qbuf, "You cannot wear %s over %s. Take it off and then wear the robe?", acxname(otmp), cbuf);
                        char ans = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, tbuf, qbuf, ynchars, 'n', yndescs, (const char*)0);
                        if (ans == 'y')
                        {
                            (void)take_off_covering_and_wear(otmp, W_ARMO);
                        }
                    }
                    else
                    {
                        play_sfx_sound(SFX_GENERAL_CANNOT);
                        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "wear that over your %s.", cloak_simple_name(uarmc));
                    }
                }
            }
            err++;
        }
        else
            *mask = W_ARMO;
    }
    else if (is_cloak(otmp)) 
    {
        if (uarmc && !constructing_letters)
        {
            if (noisy)
                (void)already_wearing_with_exchange_prompt(an(cloak_simple_name(uarmc)), otmp, uarmc);
            err++;
        } 
        else
            *mask = W_ARMC;
    } 
    else if (is_suit(otmp)) 
    {
        if (uarm && !constructing_letters)
        {
            if (noisy)
            {
                (void)already_wearing_with_exchange_prompt("some armor", otmp, uarm);
            }
            err++;
        }
        else if ((uarmc || uarmo) && !constructing_letters)
        {
            if (noisy)
            {
                if (flags.exchange_prompt)
                {
                    char cbuf[BUFSZ] = "your piece of armor";
                    boolean many = FALSE;
                    if (uarmo && uarmc)
                    {
                        many = TRUE;
                        Sprintf(cbuf, "your %s and %s", cloak_simple_name(uarmc), robe_simple_name(uarmo));
                    }
                    else if (uarmo)
                        Sprintf(cbuf, "%s", yname(uarmo));
                    else if (uarmc)
                        Sprintf(cbuf, "%s", yname(uarmc));

                    char tbuf[BUFSZ];
                    Sprintf(tbuf, "Cannot Wear Suit Over %s", many ? "Robe and Cloak" : uarmo ? "Robe" : "Cloak");

                    char qbuf[BUFSIZ];
                    Sprintf(qbuf, "You cannot wear %s over %s. Take %s off and then wear the armor?", acxname(otmp), cbuf, many ? "them" : "it");
                    char ans = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, tbuf, qbuf, ynchars, 'n', yndescs, (const char*)0);
                    if (ans == 'y')
                    {
                        (void)take_off_covering_and_wear(otmp, W_ARM);
                    }
                }
                else if (uarmo)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wear armor over a %s.", robe_simple_name(uarmo));
                }
                else
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wear armor over a %s.", cloak_simple_name(uarmc));
                }
            }
            err++;
        } 
        else
            *mask = W_ARM;
    } 
    else 
    {
        /* getobj can't do this after setting its allow_all flag; that
           happens if you have armor for slots that are covered up or
           extra armor for slots that are filled */
        if (noisy)
            silly_thing("wear", otmp);
        err++;
    }
    /* Unnecessary since now only weapons and special items like pick-axes get
     * welded to your hand, not armor
        if (welded(otmp)) {
            if (!err++) {
                if (noisy) weldmsg(otmp);
            }
        }
     */
    return !err;
}

STATIC_OVL int
accessory_or_armor_on(obj, in_takeoff_wear)
struct obj *obj;
boolean in_takeoff_wear;
{
    long mask = 0L;
    boolean armor, ring, eyewear;
    Strcpy(debug_buf_4, "accessory_or_armor_on");

    if (obj->owornmask & (W_ACCESSORY | W_ARMOR)) 
    {
        already_wearing(c_that_);
        return 0;
    }

    armor = (obj->oclass == ARMOR_CLASS);
    ring = (obj->oclass == RING_CLASS || obj->otyp == MEAT_RING);
    eyewear = (obj->otyp == BLINDFOLD || obj->otyp == TOWEL);
    /* checks which are performed prior to actually touching the item */
    if (armor) 
    {
        if (!canwearobj(obj, &mask, TRUE, FALSE))
            return 0;

        if (obj->otyp == HELM_OF_OPPOSITE_ALIGNMENT
            && qstart_level.dnum == u.uz.dnum)
        { /* in quest */
            play_sfx_sound(SFX_GENERAL_NOT_A_GOOD_IDEA);
            if (u.ualignbase[A_CURRENT] == u.ualignbase[A_ORIGINAL])
                You_ex(ATR_NONE, CLR_MSG_WARNING, "narrowly avoid losing all chance at your goal.");
            else /* converted */
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are suddenly overcome with shame and change your mind.");
            u.ublessed = 0; /* lose your god's protection */
            makeknown(obj->otyp);
            context.botl = 1; /*for AC after zeroing u.ublessed */
            return 1;
        }
    }
    else 
    {
        /* accessory */
        if (ring)
        {
            char answer, qbuf[QBUFSZ];
            int res = 0;

            if (nolimbs(youmonst.data) || nohands(youmonst.data)) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot make the ring stick to your body.");
                return 0;
            }
            if (uleft && uright) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                There_ex(ATR_NONE, CLR_MSG_FAIL, "are no more %s%s to fill.",
                      humanoid(youmonst.data) ? "ring-" : "",
                      makeplural(body_part(FINGER)));

                if (flags.exchange_prompt && !(!cursed_items_are_positive_mon(&youmonst) && uleft->cursed && uleft->bknown && uright->cursed && uright->bknown))
                {
                    struct obj* remove_obj = 0;
                    char tbuf[BUFSZ] = "";
                    Sprintf(tbuf, "No More %s%s to Fill",
                        humanoid(youmonst.data) ? "Ring-" : "",
                        makeplural(body_part(FINGER)));

                    You("have %s on the left %s and %s on the right.", acxname(uleft), body_part(FINGER), acxname(uright));

                    Sprintf(qbuf, "Remove Left or Right ring?");
                    answer = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, tbuf, qbuf, "lrq", '\0', "Left\nRight\nCancel", (const char*)0);
                    switch (answer)
                    {
                    case '\0':
                    case 'q':
                    case 'Q':
                        return 0;
                    case 'l':
                    case 'L':
                        remove_obj = uleft;
                        break;
                    case 'r':
                    case 'R':
                        remove_obj = uright;
                        break;
                    }

                    if (remove_obj)
                        (void)armor_or_accessory_off(remove_obj);
                }
                if (uleft && uright)
                    return 0;
            }

            if (uleft) 
            {
                mask = RIGHT_RING;
            } 
            else if (uright) {
                mask = LEFT_RING;
            }
            else 
            {
                do 
                {
                    Sprintf(qbuf, "Which %s%s, Left or Right?",
                            humanoid(youmonst.data) ? "ring-" : "",
                            body_part(FINGER));
                    answer = yn_function(qbuf, "lr", '\0', "Left\nRight");
                    switch (answer) 
                    {
                    case '\0':
                        return 0;
                    case 'l':
                    case 'L':
                        mask = LEFT_RING;
                        break;
                    case 'r':
                    case 'R':
                        mask = RIGHT_RING;
                        break;
                    }
                } while (!mask);
            }
            if (uarmg && uarmg->cursed) 
            {
                res = !uarmg->bknown;
                if (!uarmg->bknown)
                {
                    uarmg->bknown = TRUE;
                    update_inventory();
                }
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "cannot remove your gloves to put on the ring.");
                return res; /* uses move iff we learned gloves are cursed */
            }

            if (uwep) 
            {
                res = !uwep->bknown; /* check this before calling welded() */
                if ((mask == RIGHT_RING || bimanual(uwep)) && welded(uwep, &youmonst)) 
                {
                    const char *hand = body_part(HAND);

                    /* welded will set bknown */
                    if (bimanual(uwep))
                        hand = makeplural(hand);
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "cannot free your weapon %s to put on the ring.",
                        hand);
                    return res; /* uses move iff we learned weapon is cursed */
                }
            }
        }
        else if (obj->oclass == AMULET_CLASS) 
        {
            if (uamul) 
            {
                already_wearing_with_exchange_prompt("an amulet", obj, uamul);
                return 0;
            }
        }
        else if (obj->oclass == MISCELLANEOUS_CLASS) 
        {
            if (objects[obj->otyp].oc_subtyp != MISC_MULTIPLE_PERMITTED)
            {
                if ((umisc && objects[umisc->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp))
                {
                    already_wearing_with_exchange_prompt(an(misc_type_names[objects[obj->otyp].oc_subtyp]), obj, umisc);
                    return 0;
                }
                if ((umisc2 && objects[umisc2->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp))
                {
                    already_wearing_with_exchange_prompt(an(misc_type_names[objects[obj->otyp].oc_subtyp]), obj, umisc2);
                    return 0;
                }
                if ((umisc3 && objects[umisc3->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp))
                {
                    already_wearing_with_exchange_prompt(an(misc_type_names[objects[obj->otyp].oc_subtyp]), obj, umisc3);
                    return 0;
                }
                if ((umisc4 && objects[umisc4->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp))
                {
                    already_wearing_with_exchange_prompt(an(misc_type_names[objects[obj->otyp].oc_subtyp]), obj, umisc4);
                    return 0;
                }
                if ((umisc5 && objects[umisc5->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp))
                {
                    already_wearing_with_exchange_prompt(an(misc_type_names[objects[obj->otyp].oc_subtyp]), obj, umisc5);
                    return 0;
                }
            }
            if (umisc && umisc2 && umisc3 && umisc4 && umisc5) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "cannot wear more than five miscellanous items.");
                if (flags.exchange_prompt)
                {
                    char ans = yn_function_es(YN_STYLE_ITEM_EXCHANGE, ATR_NONE, CLR_MSG_ATTENTION, "Already Wearing 5 Miscellaneous Items", "Do you want to remove some of them?", ynchars, 'n', yndescs, (const char*)0);
                    if (ans == 'y')
                    {
                        return doremring();
                    }
                }
                return 0;
            }
        }
        else if (eyewear) 
        {
            if (ublindf) 
            {
                if (ublindf->otyp == TOWEL)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s is already covered by a towel.",
                        body_part(FACE));
                }
                else if (ublindf->otyp == BLINDFOLD) 
                {
                    already_wearing_with_exchange_prompt("a blindfold", obj, ublindf);
                }
                else
                {
                    already_wearing(something); /* ??? */
                }
                return 0;
            }
        } 
        else
        {
            /* neither armor nor accessory */
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "wear that!");
            return 0;
        }
    }

    if (!retouch_object(&obj, FALSE))
        return 1; /* costs a turn even though it didn't get worn */

    if (armor) 
    {
        int delay;

        /* if the armor is wielded, release it for wearing (won't be
           welded even if cursed; that only happens for weapons/weptools) */
        if (obj->owornmask & W_WEAPON)
            remove_worn_item(obj, FALSE);
        /*
         * Setting obj->known=1 is done because setworn() causes hero's AC
         * to change so armor's +/- value is evident via the status line.
         * We used to set it here because of that, but then it would stick
         * if a nymph stole the armor before it was fully worn.  Delay it
         * until the aftermv action.  The player may still know this armor's
         * +/- amount if donning gets interrupted, but the hero won't.
         *
        obj->known = 1;
         */
        setworn(obj, mask);
        /* if there's no delay, we'll execute 'aftermv' immediately */
        if (obj == uarm)
            afternmv = Armor_on;
        else if (obj == uarmh)
            afternmv = Helmet_on;
        else if (obj == uarmb)
            afternmv = Bracers_on;
        else if (obj == uarmg)
            afternmv = Gloves_on;
        else if (obj == uarmf)
            afternmv = Boots_on;
        else if (obj == uarms && obj->oclass == ARMOR_CLASS && objects[obj->otyp].oc_armor_category == ARM_SHIELD)
            afternmv = Shield_on;
        else if (obj == uarmc)
            afternmv = Cloak_on;
        else if (obj == uarmo)
            afternmv = Robe_on;
        else if (obj == uarmu)
            afternmv = Shirt_on;
        else
        {
            panic("wearing armor not worn as armor? [%08lx]", obj->owornmask);
            return 0;
        }

        if (!in_takeoff_wear)
            delay = -objects[obj->otyp].oc_delay;
        else
            delay = 0;

        if (delay) 
        {
            nomul(delay);
            multi_reason = "dressing up";
            nomovemsg = "You finish your dressing maneuver.";
        } 
        else 
        {
            unmul(""); /* call (*aftermv)(), clear it+nomovemsg+multi_reason */
            on_msg(obj);
        }

        if(!in_takeoff_wear)
            context.takeoff.mask = context.takeoff.what = context.wear.mask = context.wear.what = 0L;
    } 
    else 
    { /* not armor */
        boolean give_feedback = FALSE;

        /* [releasing wielded accessory handled in Xxx_on()] */
        if (ring) 
        {
            setworn(obj, mask);
            Ring_on(obj);
            give_feedback = TRUE;
        } 
        else if (obj->oclass == AMULET_CLASS) 
        {
            setworn(obj, W_AMUL);
            Amulet_on();
            /* no feedback here if amulet of change got used up */
            give_feedback = (uamul != 0);
        } 
        else if (obj->oclass == MISCELLANEOUS_CLASS) 
        {
            if(!umisc)
            {
                MiscellaneousItem_on(obj, W_MISC);
                give_feedback = (umisc != 0);
            }
            else if (!umisc2)
            {
                MiscellaneousItem_on(obj, W_MISC2);
                give_feedback = (umisc2 != 0);
            }
            else if (!umisc3)
            {
                MiscellaneousItem_on(obj, W_MISC3);
                give_feedback = (umisc3 != 0);
            }
            else if (!umisc4)
            {
                MiscellaneousItem_on(obj, W_MISC4);
                give_feedback = (umisc4 != 0);
            }
            else
            {
                //Must be available
                MiscellaneousItem_on(obj, W_MISC5);
                give_feedback = (umisc5 != 0);
            }
        } 
        else if (eyewear) 
        {
            /* setworn() handled by Blindf_on() */
            Blindf_on(obj);
            /* message handled by Blindf_on(); leave give_feedback False */
        }
        /* feedback for ring or for amulet other than 'change' */
        if (give_feedback && is_worn(obj))
            prinv((char *) 0, obj, 0L);
    }
    return 1;
}

/* the 'W' command */
int
dowear()
{
    struct obj *otmp;

    /* cantweararm() checks for suits of armor, not what we want here;
       verysmall() or nohands() checks for shields, gloves, etc... */
    if (!can_operate_objects(youmonst.data))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Don't even bother.");
        return 0;
    }
    if (uarm && uarmu && uarmc && uarmh && uarms && uarmg && uarmf && uarmo && uarmb && umisc && umisc2 && umisc3 && umisc4 && umisc5
        && uleft && uright && uamul && ublindf) 
    {
        /* 'W' message doesn't mention accessories */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are already wearing a full complement of armor.");
        return 0;
    }
    otmp = getobj(clothes, "wear", 0, "");
    return otmp ? accessory_or_armor_on(otmp, FALSE) : 0;
}

/* the 'P' command */
int
doputon()
{
    struct obj *otmp;

    if (uleft && uright && uamul && ublindf && umisc && umisc2 && umisc3 && umisc4 && umisc5
        && uarm && uarmu && uarmc && uarmh && uarms && uarmg && uarmf && uarmo && uarmb) {
        /* 'P' message doesn't mention armor */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s%s and miscellaneous magic item slots are full, and you're already wearing an amulet and %s.",
             humanoid(youmonst.data) ? "ring-" : "",
             makeplural(body_part(FINGER)),
             "a blindfold");
        return 0;
    }
    otmp = getobj(accessories, "put on", 0, "");

    return otmp ? accessory_or_armor_on(otmp, FALSE) : 0;
}


/* calculate current magic cancellation */
void
find_mc()
{
    u.umc = magic_negation(&youmonst);
    context.botl = 1;
}


/* calculate current armor class */
void
find_ac()
{
    int uac = 0; /* base armor class for current form */
    int uac_natural_base = mons[u.umonnum].ac; /* base armor class for current form */
    int uac_natural = uac_natural_base; /* base armor class for current form */
    int uac_armor_bonus = 0; /* AC bonus due to armor */
    int uac_armor = 10; /* AC due to armor */
    int suit_ac_bonus = 0;
    int robe_ac_bonus = 0;
    int combined_ac_bonus = 0;

    /* armor class from worn gear */
    if (uarm)
        suit_ac_bonus = ARM_AC_BONUS(uarm, youmonst.data);
    if (uarmo)
        robe_ac_bonus = ARM_AC_BONUS(uarmo, youmonst.data);

    combined_ac_bonus = max(suit_ac_bonus, robe_ac_bonus);
    uac_armor_bonus += combined_ac_bonus;
    context.suit_yielding_ac_bonus = (suit_ac_bonus == combined_ac_bonus);
    context.robe_yielding_ac_bonus = (robe_ac_bonus == combined_ac_bonus);

    if (uarmc)
        uac_armor_bonus += ARM_AC_BONUS(uarmc, youmonst.data);
    if (uarmh)
        uac_armor_bonus += ARM_AC_BONUS(uarmh, youmonst.data);
    if (uarmf)
        uac_armor_bonus += ARM_AC_BONUS(uarmf, youmonst.data);
    if (uarms && (is_wielded_item(uarms)))
    {
        uac_armor_bonus += ARM_AC_BONUS(uarms, youmonst.data); /* Only a shield and a wielded weapon can give AC; exclude wielded other armor types */
        if (is_shield(uarms))
            uac_armor_bonus += shield_skill_ac_bonus(P_SKILL_LEVEL(P_SHIELD));
    }
    if (uarmg)
        uac_armor_bonus += ARM_AC_BONUS(uarmg, youmonst.data);
    if (uarmu)
        uac_armor_bonus += ARM_AC_BONUS(uarmu, youmonst.data);
    if (uarmb)
        uac_armor_bonus += ARM_AC_BONUS(uarmb, youmonst.data);

    if (uwep && (is_wielded_item(uwep)))
    {
        uac_armor_bonus += ARM_AC_BONUS(uwep, youmonst.data); /* A wielded weapon can give AC, also a wielded shield (in right hand) */
        if (is_shield(uwep))
            uac_armor_bonus += shield_skill_ac_bonus(P_SKILL_LEVEL(P_SHIELD));
    }

    uac_natural -= (uac_armor_bonus / 3);
    uac_armor -= (uac_armor_bonus + ((10 - uac_natural_base) / 3));

    /* Pick better */
    if (uac_natural <= uac_armor)
        uac = uac_natural;
    else
        uac = uac_armor;

    //if(uac)
    /* Kludge removed by JG -- Now in u.uacbonus */
    /*
    if (uleft && uleft->otyp == RIN_PROTECTION)
        uac -= uleft->enchantment;
    if (uright && uright->otyp == RIN_PROTECTION)
        uac -= uright->enchantment;
    */

    /* Dodge Skill Bonus */
    uac -= dodge_skill_ac_bonus(P_SKILL_LEVEL(P_DODGE));

    /* DEX Bonus */
    if(!Paralyzed && !Sleeping)
        uac -= dexterity_ac_bonus(ACURR(A_DEX));
    
    /* Intrinsic and extrinsic AC Bonus */
    uac -= u.ubaseacbonus;
    uac -= u.uacbonus;

    if (Magical_stoneskin)
        uac -= 18;
    else if (Magical_barkskin)
        uac -= 12;
    else if (Magical_shielding)
        uac -= 6;
    else if (Magical_protection)
        uac -= 3;
    
    /* Divine protection */
    uac -= u.ublessed;

    /* Monk protection */
    uac -= get_role_AC_bonus();

    if (uac < -9999)
        uac = -9999; /* u.uac is a short */
    else if (uac > 9999)
        uac = 9999; /* for completeness */

    if (uac != u.uac) {
        u.uac = (short)uac;
        context.botl = 1;
    }
}

int
get_role_AC_bonus(VOID_ARGS)
{
    if (Role_if(PM_MONK) && !Upolyd && !uarm && (!uarms || is_launcher(uarms)) && (!uwep || is_launcher(uwep)))
        return u.ulevel / 4;
    else
        return 0;
}

int
get_role_MC_bonus(VOID_ARGS)
{
    if (Role_if(PM_MONK) && !Upolyd && !uarm && (!uarms || is_launcher(uarms)) && (!uwep || is_launcher(uwep)))
        return u.ulevel / 12;
    else 
        return 0;
}

void
glibr()
{
    register struct obj *otmp;
    int xfl = 0;
    boolean leftfall, rightfall, wastwoweap = FALSE;
    const char *otherwep = 0, *thiswep, *which, *hand;

    leftfall = (uleft && !uleft->cursed
                && (!uwep || !welded(uwep, &youmonst) || !bimanual(uwep)));
    rightfall = (uright && !uright->cursed && (!welded(uwep, &youmonst)));
    if (!uarmg && (leftfall || rightfall) && !nolimbs(youmonst.data)) {
        /* changed so cursed rings don't fall off, GAN 10/30/86 */
        Your("%s off your %s.",
             (leftfall && rightfall) ? "rings slip" : "ring slips",
             (leftfall && rightfall) ? makeplural(body_part(FINGER))
                                     : body_part(FINGER));
        xfl++;
        if (leftfall) {
            otmp = uleft;
            Ring_off(uleft);
            dropx(otmp);
        }
        if (rightfall) {
            otmp = uright;
            Ring_off(uright);
            dropx(otmp);
        }
    }

    otmp = uarms;
    if (u.twoweap && otmp && !welded(otmp, &youmonst)) {
        /* secondary weapon doesn't need nearly as much handling as
           primary; when in two-weapon mode, we know it's one-handed
           with something else in the other hand and also that it's
           a weapon or weptool rather than something unusual, plus
           we don't need to compare its type with the primary */
        otherwep = is_shield(otmp) ? "shield" : is_sword(otmp) ? c_sword : weapon_descr(otmp);
        if (otmp->quan > 1L)
            otherwep = makeplural(otherwep);
        hand = body_part(HAND);
        which = "left ";
        Your("%s %s%s from your %s%s.", otherwep, xfl ? "also " : "",
             otense(otmp, "slip"), which, hand);
        xfl++;
        wastwoweap = TRUE;
        setuwep((struct obj *) 0, otmp->owornmask);
        if (canletgo(otmp, ""))
            dropx(otmp);
    }
    otmp = uwep;
    if (otmp && !welded(otmp, &youmonst)) {
        long savequan = otmp->quan;

        /* nice wording if both weapons are the same type */
        thiswep = is_sword(otmp) ? c_sword : weapon_descr(otmp);
        if (otherwep && strcmp(thiswep, makesingular(otherwep)))
            otherwep = 0;
        if (otmp->quan > 1L) {
            /* most class names for unconventional wielded items
               are ok, but if wielding multiple apples or rations
               we don't want "your foods slip", so force non-corpse
               food to be singular; skipping makeplural() isn't
               enough--we need to fool otense() too */
            if (!strcmp(thiswep, "food"))
                otmp->quan = 1L;
            else
                thiswep = makeplural(thiswep);
        }
        hand = body_part(HAND);
        which = "";
        if (bimanual(otmp))
            hand = makeplural(hand);
        else if (wastwoweap)
            which = "right "; /* preceding msg was about left */
        pline("%s %s%s %s%s from your %s%s.",
              !strncmp(thiswep, "corpse", 6) ? "The" : "Your",
              otherwep ? "other " : "", thiswep, xfl ? "also " : "",
              otense(otmp, "slip"), which, hand);
        /* xfl++; */
        otmp->quan = savequan;
        setuwep((struct obj *) 0, W_WEP);
        if (canletgo(otmp, ""))
            dropx(otmp);
    }
}

struct obj *
some_armor(victim)
struct monst *victim;
{
    register struct obj *otmph, *otmp;

    otmph = (victim == &youmonst) ? uarmc : which_armor(victim, W_ARMC);
    if (!otmph)
        otmph = (victim == &youmonst) ? uarmo : which_armor(victim, W_ARMO);
    if (!otmph)
        otmph = (victim == &youmonst) ? uarm : which_armor(victim, W_ARM);
    if (!otmph)
        otmph = (victim == &youmonst) ? uarmu : which_armor(victim, W_ARMU);

    otmp = (victim == &youmonst) ? uarmh : which_armor(victim, W_ARMH);
    if (otmp && (!otmph || !rn2(4)))
        otmph = otmp;
    otmp = (victim == &youmonst) ? uarmb : which_armor(victim, W_ARMB);
    if (otmp && (!otmph || !rn2(4)))
        otmph = otmp;
    otmp = (victim == &youmonst) ? uarmg : which_armor(victim, W_ARMG);
    if (otmp && (!otmph || !rn2(4)))
        otmph = otmp;
    otmp = (victim == &youmonst) ? uarmf : which_armor(victim, W_ARMF);
    if (otmp && (!otmph || !rn2(4)))
        otmph = otmp;
    otmp = (victim == &youmonst) ? uarms : which_armor(victim, W_ARMS);
    if (otmp && otmp->oclass == ARMOR_CLASS && (!otmph || !rn2(4)))
        otmph = otmp;
    return otmph;
}

/* used for praying to check and fix levitation trouble */
struct obj *
stuck_ring(ring, otyp)
struct obj *ring;
int otyp;
{
    if (ring != uleft && ring != uright) {
        impossible("stuck_ring: neither left nor right?");
        return (struct obj *) 0;
    }

    if (ring && ring->otyp == otyp) {
        /* reasons ring can't be removed match those checked by select_off();
           limbless case has extra checks because ordinarily it's temporary */
        if (nolimbs(youmonst.data) && uamul
            && uamul->otyp == AMULET_OF_UNCHANGING && uamul->cursed)
            return uamul;
        if (welded(uwep, &youmonst) && (ring == uright || bimanual(uwep)))
            return uwep;
        if (uarmg && uarmg->cursed)
            return uarmg;
        if (ring->cursed)
            return ring;
    }
    /* either no ring or not right type or nothing prevents its removal */
    return (struct obj *) 0;
}

/* also for praying; find worn item that confers "Unchanging" attribute */
struct obj *
unchanger()
{
    if (uamul && uamul->otyp == AMULET_OF_UNCHANGING)
        return uamul;
    return 0;
}

STATIC_PTR
int
select_off(otmp)
register struct obj *otmp;
{
    struct obj *why;
    char buf[BUFSZ];

    if (!otmp)
        return 0;
    *buf = '\0'; /* lint suppression */

    /* special ring checks */
    if (otmp == uright || otmp == uleft) {
        if (nolimbs(youmonst.data)) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "ring is stuck.");
            return 0;
        }
        why = 0; /* the item which prevents ring removal */
        if (welded(uwep, &youmonst) && (otmp == uright || bimanual(uwep))) {
            Sprintf(buf, "free a weapon %s", body_part(HAND));
            why = uwep;
        } else if (uarmg && uarmg->cursed) {
            Sprintf(buf, "take off your %s", c_gloves);
            why = uarmg;
        }
        if (why) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "cannot %s to remove the ring.", buf);
            if (!why->bknown)
            {
                why->bknown = TRUE;
                update_inventory();
            }
            return 0;
        }
    }
    /* special glove checks */
    if (otmp == uarmg) {
        if (welded(uwep, &youmonst)) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "are unable to take off your %s while wielding that %s.",
                c_gloves, is_sword(uwep) ? c_sword : c_weapon);
            if (!uwep->bknown)
            {
                uwep->bknown = TRUE;
                update_inventory();
            }
            return 0;
        } else if (Glib) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_cant_ex(ATR_NONE, CLR_MSG_WARNING, "take off the slippery %s with your slippery %s.",
                     c_gloves, makeplural(body_part(FINGER)));
            return 0;
        }
    }
    /* special boot checks */
    if (otmp == uarmf) {
        if (u.utrap && u.utraptype == TT_BEARTRAP) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "bear trap prevents you from pulling your %s out.",
                      body_part(FOOT));
            return 0;
        } else if (u.utrap && u.utraptype == TT_INFLOOR) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "are stuck in the %s, and cannot pull your %s out.",
                surface(u.ux, u.uy), makeplural(body_part(FOOT)));
            return 0;
        }
    }
    /* special suit and shirt checks */
    if (otmp == uarm || otmp == uarmo || otmp == uarmu) {
        why = 0; /* the item which prevents disrobing */
        if (uarmc && uarmc->cursed) {
            Sprintf(buf, "remove your %s", cloak_simple_name(uarmc));
            why = uarmc;
        } else if ((otmp == uarm || otmp == uarmu) && uarmo && uarmo->cursed) {
            Sprintf(buf, "remove your %s", robe_simple_name(uarmo));
            why = uarmo;
        } else if (otmp == uarmu && uarm && uarm->cursed) {
            Sprintf(buf, "remove your %s", c_suit);
            why = uarm;
        } else if (welded(uwep, &youmonst) && bimanual(uwep)) {
            Sprintf(buf, "release your %s",
                    is_sword(uwep) ? c_sword : (uwep->otyp == BATTLE_AXE)
                                                   ? c_axe
                                                   : c_weapon);
            why = uwep;
        }
        if (why) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_WARNING, "cannot %s to take off %s.", buf, the(xname(otmp)));
            if (!why->bknown)
            {
                why->bknown = TRUE;
                update_inventory();
            }
            return 0;
        }
    }
    /* basic curse check */
    if (otmp == uquiver || otmp == uswapwep || otmp == uswapwep2) {
        ; /* some items can be removed even when cursed */
    } else {
        /* otherwise, this is fundamental */
        if (cursed(otmp))
            return 0;
    }

    if (otmp == uarm)
        context.takeoff.mask |= WORN_ARMOR;
    else if (otmp == uarmc)
        context.takeoff.mask |= WORN_CLOAK;
    else if (otmp == uarmo)
        context.takeoff.mask |= WORN_ROBE;
    else if (otmp == uarmf)
        context.takeoff.mask |= WORN_BOOTS;
    else if (otmp == uarmb)
        context.takeoff.mask |= WORN_BRACERS;
    else if (otmp == umisc)
        context.takeoff.mask |= WORN_MISCELLANEOUS;
    else if (otmp == umisc2)
        context.takeoff.mask |= WORN_MISCELLANEOUS2;
    else if (otmp == umisc3)
        context.takeoff.mask |= WORN_MISCELLANEOUS3;
    else if (otmp == umisc4)
        context.takeoff.mask |= WORN_MISCELLANEOUS4;
    else if (otmp == umisc5)
        context.takeoff.mask |= WORN_MISCELLANEOUS5;
    else if (otmp == uarmg)
        context.takeoff.mask |= WORN_GLOVES;
    else if (otmp == uarmh)
        context.takeoff.mask |= WORN_HELMET;
    else if (otmp == uarms)
        context.takeoff.mask |= WORN_SHIELD;
    else if (otmp == uarmu)
        context.takeoff.mask |= WORN_SHIRT;
    else if (otmp == uleft)
        context.takeoff.mask |= LEFT_RING;
    else if (otmp == uright)
        context.takeoff.mask |= RIGHT_RING;
    else if (otmp == uamul)
        context.takeoff.mask |= WORN_AMUL;
    else if (otmp == ublindf)
        context.takeoff.mask |= WORN_BLINDF;
    else if (otmp == uwep)
        context.takeoff.mask |= W_WEP;
    else if (otmp == uswapwep)
        context.takeoff.mask |= W_SWAPWEP;
    else if (otmp == uswapwep2)
        context.takeoff.mask |= W_SWAPWEP2;
    else if (otmp == uquiver)
        context.takeoff.mask |= W_QUIVER;

    else
        impossible("select_off: %s???", doname(otmp));

    return 0;
}

STATIC_OVL struct obj *
do_takeoff()
{
    struct obj *otmp = (struct obj *) 0;
    struct takeoff_info *doff = &context.takeoff;

    context.takeoff.mask |= I_SPECIAL; /* set flag for cancel_doff() */
    if (doff->what == W_WEP)
    {
        if (!cursed(uwep)) {
            setuwep((struct obj *) 0, W_WEP);
            if(u.twoweap)
                Your("right %s is now empty.", body_part(HAND));
            else
                You("are empty %s.", body_part(HANDED));
            //u.twoweap = FALSE;
        }
    }
    else if (doff->what == W_WEP2)
    {
        if (!cursed(uarms)) {
            setuwep((struct obj*) 0, W_WEP2);
            Your("left %s is now empty.", body_part(HAND));
            //u.twoweap = FALSE;
        }
    }
    else if (doff->what == W_SWAPWEP)
    {
        setuswapwep((struct obj *) 0, W_SWAPWEP);
        You("no longer have a second weapon readied.");
        //u.twoweap = FALSE;
    }
    else if (doff->what == W_SWAPWEP2)
    {
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
        You("no longer have a second weapon readied.");
        //u.twoweap = FALSE;
    }
    else if (doff->what == W_QUIVER) {
        setuqwep((struct obj *) 0);
        You("no longer have ammunition readied.");
    } else if (doff->what == WORN_ARMOR) {
        otmp = uarm;
        if (!cursed(otmp))
            (void) Armor_off();
    } else if (doff->what == WORN_CLOAK) {
        otmp = uarmc;
        if (!cursed(otmp))
            (void) Cloak_off();
    }
    else if (doff->what == WORN_ROBE) {
        otmp = uarmo;
        if (!cursed(otmp))
            (void)Robe_off();
    } else if (doff->what == WORN_BOOTS) {
        otmp = uarmf;
        if (!cursed(otmp))
            (void) Boots_off();
    }
    else if (doff->what == WORN_BRACERS) {
        otmp = uarmb;
        if (!cursed(otmp))
            (void)Bracers_off();
    } else if (doff->what == WORN_MISCELLANEOUS) {
        otmp = umisc;
        if (!cursed(otmp))
            (void)MiscellaneousItem_off(otmp);
    }
    else if (doff->what == WORN_MISCELLANEOUS2) {
        otmp = umisc2;
        if (!cursed(otmp))
            (void)MiscellaneousItem_off(otmp);
    }
    else if (doff->what == WORN_MISCELLANEOUS3) {
        otmp = umisc3;
        if (!cursed(otmp))
            (void)MiscellaneousItem_off(otmp);
    }
    else if (doff->what == WORN_MISCELLANEOUS4) {
        otmp = umisc4;
        if (!cursed(otmp))
            (void)MiscellaneousItem_off(otmp);
    }
    else if (doff->what == WORN_MISCELLANEOUS5) {
        otmp = umisc5;
        if (!cursed(otmp))
            (void)MiscellaneousItem_off(otmp);
    } else if (doff->what == WORN_GLOVES) {
        otmp = uarmg;
        if (!cursed(otmp))
            (void) Gloves_off();
    } else if (doff->what == WORN_HELMET) {
        otmp = uarmh;
        if (!cursed(otmp))
            (void) Helmet_off();
    } else if (doff->what == WORN_SHIELD) {
        otmp = uarms;
        if (!cursed(otmp) && is_shield(otmp))
            (void) Shield_off();
    } else if (doff->what == WORN_SHIRT) {
        otmp = uarmu;
        if (!cursed(otmp))
            (void) Shirt_off();
    } else if (doff->what == WORN_AMUL) {
        otmp = uamul;
        if (!cursed(otmp))
            Amulet_off();
    } else if (doff->what == LEFT_RING) {
        otmp = uleft;
        if (!cursed(otmp))
            Ring_off(uleft);
    } else if (doff->what == RIGHT_RING) {
        otmp = uright;
        if (!cursed(otmp))
            Ring_off(uright);
    } else if (doff->what == WORN_BLINDF) {
        if (!cursed(ublindf))
            Blindf_off(ublindf);
    } else {
        impossible("do_takeoff: taking off %lx", doff->what);
    }
    context.takeoff.mask &= ~I_SPECIAL; /* clear cancel_doff() flag */

    return otmp;
}

/* occupation callback for 'A' */
STATIC_PTR
int
take_off(VOID_ARGS)
{
    register int i;
    register struct obj *otmp;
    struct takeoff_info *doff = &context.takeoff;
    struct wear_info* don = &context.wear;

    if (doff->what) 
    {
        if (doff->delay > 0) 
        {
            doff->delay--;
            return 1; /* still busy */
        }
        if ((otmp = do_takeoff()) != 0)
            off_msg(otmp);

        long oldwhat = doff->what;
        doff->mask &= ~doff->what;
        doff->what = 0L;
        if (!otmp)
        {
            if (
                (oldwhat == W_ARM && uarm && ((doff->what & W_ARMU) || (don->what & W_ARMU)))
                || (oldwhat == W_ARMO && uarmo && ((doff->what & (W_ARMU | W_ARM)) || (don->what & (W_ARMU | W_ARM))))
                || (oldwhat == W_ARMC && uarmc && ((doff->what & (W_ARMU | W_ARM | W_ARMO)) || (don->what & (W_ARMU | W_ARM | W_ARMO))))
                )
            {
                reset_remarm();
                return 0;
            }
        }
    }

    for (i = 0; takeoff_order[i]; i++)
        if (doff->mask & takeoff_order[i]) 
        {
            doff->what = takeoff_order[i];
            break;
        }

    otmp = (struct obj *) 0;
    doff->delay = 0;

    if (doff->what == 0L) 
    {
        if (don->what)
        {
            int idx = -1;
            for(i = 0; i < WEAR_OID_BITS; i++)
                if ((don->what & (1L << i)) != 0) /* Take the first bit; there should be only one */
                {
                    idx = i;
                    break;
                }

            if (idx < 0)
                return 0; /* There is no object to be worn specified or don->what is incorrect; cancel the process */

            unsigned id = don->oid[idx];
            if (id == 0 || (otmp = o_on(id, invent)) == 0)
                return 0; /* Can't find the object to be worn in the inventory; cancel the process */

            if (don->delay > 0)
            {
                don->delay--;
                return 1; /* still busy */
            }

            if (don->what & (W_WEAPON & ~W_ARMS))
            {
                /* Wield a weapon */
            }
            else
            {
                if (accessory_or_armor_on(otmp, TRUE) == 0)
                    return 0;
            }

            don->mask &= ~don->what;
            don->what = 0L;
        }

        for (i = 0; wear_order[i]; i++)
            if (don->mask & wear_order[i])
            {
                don->what = wear_order[i];
                break;
            }

        if (don->what == 0L)
        {
            You("finish %s.", doff->disrobing);
            return 0;
        }
        else if (don->what == W_WEP)
        {
            don->delay = 1;
        }
        else if (don->what == W_SWAPWEP)
        {
            don->delay = 1;
        }
        else if (don->what == W_SWAPWEP2)
        {
            don->delay = 1;
        }
        else if (don->what == W_QUIVER)
        {
            don->delay = 1;
        }
        else if (don->what == WORN_ARMOR)
        {
            otmp = uarm;
        }
        else if (don->what == WORN_ROBE)
        {
            otmp = uarmo;
        }
        else if (don->what == WORN_CLOAK)
        {
            otmp = uarmc;
        }
        else if (don->what == WORN_BOOTS)
        {
            otmp = uarmf;
        }
        else if (don->what == WORN_BRACERS)
        {
            otmp = uarmb;
        }
        else if (don->what == WORN_GLOVES)
        {
            otmp = uarmg;
        }
        else if (don->what == WORN_HELMET)
        {
            otmp = uarmh;
        }
        else if (don->what == WORN_SHIELD)
        {
            otmp = uarms;
        }
        else if (don->what == WORN_SHIRT)
        {
            otmp = uarmu;
        }
        else if (don->what == WORN_AMUL)
        {
            don->delay = 1;
        }
        else if (don->what == LEFT_RING)
        {
            don->delay = 1;
        }
        else if (don->what == RIGHT_RING)
        {
            don->delay = 1;
        }
        else if (don->what & W_MISCITEMS)
        {
            don->delay = 1;
        }
        else if (don->what == WORN_BLINDF)
        {
            don->delay = 1;
        }
        else
        {
            impossible("take_off: wearing %lx", don->what);
            return 0; /* force done */
        }

        if (otmp)
            don->delay += objects[otmp->otyp].oc_delay;

        /* Since setting the occupation now starts the counter next move, that
         * would always produce a delay 1 too big per item unless we subtract
         * 1 here to account for it.
         */
        if (don->delay > 0)
            don->delay--;

        set_occupation(take_off, doff->disrobing, otmp ? objects[otmp->otyp].oc_soundset : OBJECT_SOUNDSET_NONE, OCCUPATION_TAKING_OFF, OCCUPATION_SOUND_TYPE_RESUME, 0);
        return 1; /* get busy */

    } 
    else if (doff->what == W_WEP) 
    {
        doff->delay = 1;
    } 
    else if (doff->what == W_SWAPWEP)
    {
        doff->delay = 1;
    }
    else if (doff->what == W_SWAPWEP2)
    {
        doff->delay = 1;
    }
    else if (doff->what == W_QUIVER)
    {
        doff->delay = 1;
    }
    else if (doff->what == WORN_ARMOR) 
    {
        otmp = uarm;
        /* If a cloak is being worn, add the time to take it off and put
         * it back on again.  Kludge alert! since that time is 0 for all
         * known cloaks, add 1 so that it actually matters...
         */
        if (uarmc && (don->mask & WORN_CLOAK) == 0)
            doff->delay += 2 * objects[uarmc->otyp].oc_delay + 1;
        if (uarmo && (don->mask & WORN_ROBE) == 0)
            doff->delay += 2 * objects[uarmo->otyp].oc_delay + 1;
    }
    else if (doff->what == WORN_ROBE) 
    {
        otmp = uarmo;
        /* If a cloak is being worn, add the time to take it off and put
         * it back on again.  Kludge alert! since that time is 0 for all
         * known cloaks, add 1 so that it actually matters...
         */
        if (uarmc && (don->mask & WORN_CLOAK) == 0)
            doff->delay += 2 * objects[uarmc->otyp].oc_delay + 1;
    } 
    else if (doff->what == WORN_CLOAK)
    {
        otmp = uarmc;
    }
    else if (doff->what == WORN_BOOTS)
    {
        otmp = uarmf;
    }
    else if (doff->what == WORN_BRACERS) 
    {
        otmp = uarmb;
    } 
    else if (doff->what == WORN_GLOVES) 
    {
        otmp = uarmg;
    }
    else if (doff->what == WORN_HELMET) 
    {
        otmp = uarmh;
    }
    else if (doff->what == WORN_SHIELD) 
    {
        otmp = uarms;
    }
    else if (doff->what == WORN_SHIRT) 
    {
        otmp = uarmu;
        /* add the time to take off and put back on armor and/or cloak */
        if (uarm && (don->mask & WORN_ARMOR) == 0)
            doff->delay += 2 * objects[uarm->otyp].oc_delay;
        if (uarmo && (don->mask & WORN_ROBE) == 0)
            doff->delay += 2 * objects[uarmo->otyp].oc_delay + 1;
        if (uarmc && (don->mask & WORN_CLOAK) == 0)
            doff->delay += 2 * objects[uarmc->otyp].oc_delay + 1;
    }
    else if (doff->what == WORN_AMUL) 
    {
        doff->delay = 1;
    }
    else if (doff->what == LEFT_RING) 
    {
        doff->delay = 1;
    }
    else if (doff->what == RIGHT_RING)
    {
        doff->delay = 1;
    }
    else if (doff->what & W_MISCITEMS)
    {
        doff->delay = 1;
    }
    else if (doff->what == WORN_BLINDF)
    {
        /* [this used to be 2, but 'R' (and 'T') only require 1 turn to
           remove a blindfold, so 'A' shouldn't have been requiring 2] */
        doff->delay = 1;
    } 
    else 
    {
        impossible("take_off: taking off %lx", doff->what);
        return 0; /* force done */
    }

    if (otmp)
        doff->delay += objects[otmp->otyp].oc_delay;

    /* Since setting the occupation now starts the counter next move, that
     * would always produce a delay 1 too big per item unless we subtract
     * 1 here to account for it.
     */
    if (doff->delay > 0)
        doff->delay--;

    set_occupation(take_off, doff->disrobing, otmp ? objects[otmp->otyp].oc_soundset : OBJECT_SOUNDSET_NONE, OCCUPATION_TAKING_OFF, OCCUPATION_SOUND_TYPE_RESUME, 0);
    return 1; /* get busy */
}

/* clear saved context to avoid inappropriate resumption of interrupted 'A' */
void
reset_remarm()
{
    context.takeoff.what = context.takeoff.mask = context.wear.what = context.wear.mask = 0L;
    context.takeoff.command = 0;
    int i;
    for (i = 0; i < WEAR_OID_BITS; i++)
        context.wear.oid[i] = 0;
    context.takeoff.disrobing[0] = '\0';
}

/* the M('t') command -- remove multiple worn items */
int
doddoremarm()
{
    int result = 0;
    if (context.takeoff.command != TAKEOFF_WEAR_CMD_TAKEOFF)
        reset_remarm(); /* Cancel exchange */

    if (context.takeoff.what || context.takeoff.mask) {
        You("continue %s.", context.takeoff.disrobing);
        set_occupation(take_off, context.takeoff.disrobing, occsoundset, occtyp, OCCUPATION_SOUND_TYPE_RESUME, 0);
        return 0;
    } else if (!uwep && !uarms && !uswapwep && !uswapwep2 && !uquiver && !uamul && !ublindf && !uleft
               && !uright && !umisc && !umisc2 && !umisc3 && !umisc4 && !umisc5 && !wearing_armor()) {
        You("are not wearing anything.");
        return 0;
    }

    add_valid_menu_class(0); /* reset */
    if (flags.menu_style != MENU_TRADITIONAL
        || (result = ggetobj("take off", select_off, 0, FALSE,
                             (unsigned *) 0, 0)) < -1)
        result = menu_remarm(result);

    activate_take_off();
    /* The time to perform the command is already completely accounted for
     * in take_off(); if we return 1, that would add an extra turn to each
     * disrobe.
     */
    return 0;
}

STATIC_OVL int
menu_remarm(retry)
int retry;
{
    int n, i = 0;
    menu_item *pick_list;
    boolean all_worn_categories = TRUE;

    if (retry) {
        all_worn_categories = (retry == -2);
    } else if (flags.menu_style == MENU_FULL) {
        all_worn_categories = FALSE;
        n = query_category("What type of things do you want to take off?",
                           invent, (WORN_TYPES | ALL_TYPES
                                    | UNPAID_TYPES | UNIDENTIFIED_TYPES | BUCX_TYPES),
                           &pick_list, PICK_ANY);
        if (!n)
            return 0;
        for (i = 0; i < n; i++) {
            if (pick_list[i].item.a_int == ALL_TYPES_SELECTED)
                all_worn_categories = TRUE;
            else
                add_valid_menu_class(pick_list[i].item.a_int);
        }
        free((genericptr_t) pick_list);
    } else if (flags.menu_style == MENU_COMBINATION) {
        unsigned ggofeedback = 0;

        i = ggetobj("take off", select_off, 0, TRUE, &ggofeedback, 0);
        if (ggofeedback & ALL_FINISHED)
            return 0;
        all_worn_categories = (i == -2);
    }
    if (menu_class_present('u')
        || menu_class_present('B') || menu_class_present('U')
        || menu_class_present('C') || menu_class_present('X'))
        all_worn_categories = FALSE;

    n = query_objlist("What do you want to take off?", &invent,
                      (SIGNAL_NOMENU | USE_INVLET | INVORDER_SORT),
                      &pick_list, PICK_ANY,
                      all_worn_categories ? is_worn : is_worn_by_type, 0);
    if (n > 0) {
        for (i = 0; i < n; i++)
            (void) select_off(pick_list[i].item.a_obj);
        free((genericptr_t) pick_list);
    } else if (n < 0 && flags.menu_style != MENU_COMBINATION) {
        There("is nothing else you can remove or unwield.");
    }
    return 0;
}

/* hit by destroy armor scroll/black dragon breath/monster spell */
int
destroy_arm(atmp)
register struct obj *atmp;
{
    register struct obj *otmp;
#define DESTROY_ARM(o)                            \
    ((otmp = (o)) != 0 && (!atmp || atmp == otmp) \
             && (!obj_resists(otmp, 0, 90))       \
         ? (otmp->in_use = TRUE) != 0             \
         : FALSE)

    if (DESTROY_ARM(uarmc)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s crumbles and turns to dust!", cloak_simple_name(uarmc));
        (void) Cloak_off();
        useup(otmp);
    } else if (DESTROY_ARM(uarmo)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s crumbles and turns to dust!", robe_simple_name(uarmc));
        (void)Robe_off();
        useup(otmp);
    } else if (DESTROY_ARM(uarm)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "armor turns to dust and falls to the %s!", surface(u.ux, u.uy));
        (void) Armor_gone();
        useup(otmp);
    } else if (DESTROY_ARM(uarmu)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "shirt crumbles into tiny threads and falls apart!");
        (void) Shirt_off();
        useup(otmp);
    } else if (DESTROY_ARM(uarmh)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s turns to dust and is blown away!", helm_simple_name(uarmh));
        (void) Helmet_off();
        useup(otmp);
    } else if (DESTROY_ARM(uarmb)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_VANISHES);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "bracers vanish!");
        (void)Bracers_off();
        useup(otmp);
    } else if (DESTROY_ARM(uarmg)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_VANISHES);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "gloves vanish!");
        (void) Gloves_off();
        useup(otmp);
        selftouch("You");
    } else if (DESTROY_ARM(uarmf)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "boots disintegrate!");
        (void) Boots_off();
        useup(otmp);
    }
    else if (DESTROY_ARM(uarms) && is_shield(otmp)) {
        if (donning(otmp))
            cancel_don();
        play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "shield crumbles away!");
        (void) Shield_off();
        useup(otmp);
    } else {
        return 0; /* could not destroy anything */
    }

#undef DESTROY_ARM
    stop_occupation();
    return 1;
}

void
adj_abon(otmp, delta)
register struct obj *otmp;
register schar delta;
{
    /* OBSOLETE, DO NOT CALL*/
    if (uarmg && uarmg == otmp && otmp->otyp == GAUNTLETS_OF_DEXTERITY) {
        if (delta) {
            makeknown(uarmg->otyp);
            ABONUS(A_DEX) += (delta);
        }
        context.botl = 1;
    }
    if (uarmh && uarmh == otmp && otmp->otyp == HELM_OF_BRILLIANCE) {
        if (delta) {
            makeknown(uarmh->otyp);
            ABONUS(A_INT) += (delta);
            ABONUS(A_WIS) += (delta);
        }
        context.botl = 1;
    }
    if (uarmu && uarmu == otmp && otmp->otyp == SHIRT_OF_COMELINESS) {
        if (delta) {
            makeknown(uarmu->otyp);
            ABONUS(A_CHA) += (delta);
        }
        context.botl = 1;
    }
    updatemaxhp();
    updatemaxen();

}

/* decide whether a worn item is covered up by some other worn item,
   used for dipping into liquid and applying grease;
   some criteria are different than select_off()'s */
boolean
inaccessible_equipment(obj, verb, only_if_known_cursed, constructing_letters_for_takeoff)
struct obj *obj;
const char *verb; /* "dip" or "grease", or null to avoid messages */
boolean only_if_known_cursed; /* ignore covering unless known to be cursed */
boolean constructing_letters_for_takeoff;
{
    static NEARDATA const char need_to_take_off_outer_armor[] =
        "need to take off %s to %s %s.";
    char buf[BUFSZ];
    boolean anycovering = !only_if_known_cursed; /* more comprehensible... */
#define BLOCKSACCESS(x) (anycovering || ((x)->cursed && (x)->bknown))

    if (!obj || !obj->owornmask || constructing_letters_for_takeoff)
        return FALSE; /* not inaccessible */

    /* check for robe covered by cloak */
    if (obj == uarmo && uarmc && BLOCKSACCESS(uarmc)) {
        if (verb) {
            Strcpy(buf, yname(uarmc));
            You(need_to_take_off_outer_armor, buf, verb, yname(obj));
        }
        return TRUE;
    }

    /* check for suit covered by robe and/or cloak */
    if (obj == uarm
        && ((uarmo && BLOCKSACCESS(uarmo)) || (uarmc && BLOCKSACCESS(uarmc)))) {
        if (verb) {
            char cloaktmp[QBUFSZ], robetmp[QBUFSZ];
            /* if sameprefix, use yname and xname to get "your cloak and suit"
               or "Manlobbi's cloak and suit"; otherwise, use yname and yname
               to get "your cloak and Manlobbi's suit" or vice versa */
            boolean sameprefix = (uarmo && uarmc
                                  && !strcmp(shk_your(cloaktmp, uarmc),
                                             shk_your(robetmp, uarmo)));

            *buf = '\0';
            if (uarmc && BLOCKSACCESS(uarmc))
                Strcat(buf, yname(uarmc));
            if (uarmo && uarmc && BLOCKSACCESS(uarmo) && BLOCKSACCESS(uarmc))
                Strcat(buf, " and ");
            if (uarmo && BLOCKSACCESS(uarmo))
                Strcat(buf, sameprefix ? xname(uarmo) : yname(uarmo));
            You(need_to_take_off_outer_armor, buf, verb, yname(obj));
        }
        return TRUE;
    }

    /* check for shirt covered by suit, robe and/or cloak */
    if (obj == uarmu
        && ((uarm && BLOCKSACCESS(uarm)) || (uarmo && BLOCKSACCESS(uarmo)) || (uarmc && BLOCKSACCESS(uarmc)))) {
        if (verb) {
            *buf = '\0';
            int count = 0;
            if (uarmc && BLOCKSACCESS(uarmc))
            {
                Strcat(buf, yname(uarmc));
                count++;
            }
            if (count > 0)
            {
                if((uarm && BLOCKSACCESS(uarm)) && (uarmo && BLOCKSACCESS(uarmo)) && (uarmc && BLOCKSACCESS(uarmc)))
                    Strcat(buf, ", ");
                else
                    Strcat(buf, " and ");
            }
            if (uarmo && BLOCKSACCESS(uarmo))
            {
                Strcat(buf, xname(uarmo));
                count++;
            }
            if (count > 0)
                Strcat(buf, " and ");
            if (uarm && BLOCKSACCESS(uarm))
                Strcat(buf, xname(uarm));

            You(need_to_take_off_outer_armor, buf, verb, yname(obj));
        }
        return TRUE;
    }

    /* check for ring covered by gloves */
    if ((obj == uleft || obj == uright) && uarmg && BLOCKSACCESS(uarmg)) {
        if (verb) {
            Strcpy(buf, yname(uarmg));
            You(need_to_take_off_outer_armor, buf, verb, yname(obj));
        }
        return TRUE;
    }
    /* item is not inaccessible */
    return FALSE;
}

void
reset_dowear(VOID_ARGS)
{
    Narmorpieces = 0;
    Naccessories = 0;
}

/*do_wear.c*/

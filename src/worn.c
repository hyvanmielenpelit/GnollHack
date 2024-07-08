/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    worn.c    $NHDT-Date: 1550524569 2019/02/18 21:16:09 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.56 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2013. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL void FDECL(m_lose_armor, (struct monst *, struct obj *));
STATIC_DCL boolean FDECL(m_dowear_type,
                      (struct monst *, int64_t, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL int FDECL(extra_pref, (struct monst *, struct obj *));
STATIC_DCL void FDECL(set_mon_temporary_property, (struct monst*, int, UNSIGNED_SHORT_P));


const struct worn {
    int64_t w_mask;
    struct obj **w_obj;
} worn[] = { { W_ARM, &uarm },
             { W_ARMC, &uarmc },
             { W_ARMO, &uarmo },
             { W_ARMH, &uarmh },
             { W_ARMS, &uarms },
             { W_ARMB, &uarmb },
             { W_ARMG, &uarmg },
             { W_ARMF, &uarmf },
             { W_ARMU, &uarmu },
             { W_RINGL, &uleft },
             { W_RINGR, &uright },
             { W_WEP, &uwep },
             { W_SWAPWEP, &uswapwep },
             { W_SWAPWEP2, &uswapwep2 },
             { W_QUIVER, &uquiver },
             { W_AMUL, &uamul },
             { W_MISC, &umisc },
             { W_MISC2, &umisc2 },
             { W_MISC3, &umisc3 },
             { W_MISC4, &umisc4 },
             { W_MISC5, &umisc5 },
             { W_BLINDFOLD, &ublindf },
             { W_BALL, &uball },
             { W_CHAIN, &uchain },
             { 0, 0 }
};

void
setworn(obj, mask)
register struct obj* obj;
int64_t mask;
{
    setworncore(obj, mask, TRUE);
}

/* Does not update stats */
void
setwornquietly(obj, mask)
register struct obj* obj;
int64_t mask;
{
    setworncore(obj, mask, FALSE);
}


/* note: monsters don't have clairvoyance, so your role
   has no significant effect on their use of w_blocks() */

/* Updated to use the extrinsic and blocked fields. */
void
setworncore(obj, mask, verbose_and_update_stats)
register struct obj *obj;
int64_t mask;
boolean verbose_and_update_stats;
{
    register const struct worn *wp;
    register struct obj* oobj = (struct obj*)0;
//    register int p;

    int oldmanamax = u.uenmax;
    int oldhpmax = u.uhpmax;
    int oldstr = ACURR(A_STR);
    int olddex = ACURR(A_DEX);
    int oldcon = ACURR(A_CON);
    int oldint = ACURR(A_INT);
    int oldwis = ACURR(A_WIS);
    int oldcha = ACURR(A_CHA);
    int oldac = u.uac;
    int oldmc = u.umc;

    if ((mask & (W_ARM | I_SPECIAL)) == (W_ARM | I_SPECIAL))
    {
        /* restoring saved game; no properties are conferred via skin */
        uskin = obj;
        /* assert( !uarm ); */
    } 
    else
    {
        if ((mask & W_ARMOR))
            u.uroleplay.nudist = FALSE;
        for (wp = worn; wp->w_mask; wp++)
        {
            if (wp->w_mask & mask) 
            {
                oobj = *(wp->w_obj);
                if (oobj && !(oobj->owornmask & wp->w_mask))
                {
                    impossible("Setworn: mask = %ld, oworn = %ld, update = %d.", wp->w_mask, oobj->owornmask, verbose_and_update_stats);
                }
                /* If old object remove wornmask */
                if (oobj) 
                {
                    oobj->owornmask &= ~wp->w_mask;

                    /* leave as "x = x <op> y", here and below, for broken
                     * compilers */

                    /*
                    if (wp->w_mask & ~(W_SWAPWEP | W_QUIVER)) {
                        struct objclass* peritem = &objects[oobj->otyp];
                        p = objects[oobj->otyp].oc_oprop;
                        u.uprops[p].extrinsic =
                            u.uprops[p].extrinsic & ~wp->w_mask;
                        p = objects[oobj->otyp].oc_oprop2;
                        u.uprops[p].extrinsic =
                            u.uprops[p].extrinsic & ~wp->w_mask;
                        p = objects[oobj->otyp].oc_oprop3;
                        u.uprops[p].extrinsic =
                            u.uprops[p].extrinsic & ~wp->w_mask;
                        if ((p = w_blocks(oobj, mask)) != 0)
                            u.uprops[p].blocked &= ~wp->w_mask;
                        if (oobj->oartifact)
                            set_artifact_intrinsic(oobj, 0, mask);
                    }
                    */
                    /* in case wearing or removal is in progress or removal
                       is pending (via 'A' command for multiple items) */
                    cancel_doff(oobj, wp->w_mask);
                }

                /* Set new object worn */
                *(wp->w_obj) = obj;
                if (obj) 
                {
                    obj->owornmask |= wp->w_mask;
                    obj->speflags &= ~SPEFLAGS_PREVIOUSLY_WIELDED;

                    /* Prevent getting/blocking intrinsics from wielding
                     * potions, through the quiver, etc.
                     * Allow weapon-tools, too.
                     * wp_mask should be same as mask at this point.
                     */
                    /*
                    if (wp->w_mask & ~(W_SWAPWEP | W_QUIVER)) {
                        if (obj->oclass == WEAPON_CLASS || is_weptool(obj)
                            || mask != W_WEP) {
                            struct objclass* peritem = &objects[obj->otyp];
                            p = objects[obj->otyp].oc_oprop;
                            u.uprops[p].extrinsic =
                                u.uprops[p].extrinsic | wp->w_mask;
                            p = objects[obj->otyp].oc_oprop2;
                            u.uprops[p].extrinsic =
                                u.uprops[p].extrinsic | wp->w_mask;
                            p = objects[obj->otyp].oc_oprop3;
                            u.uprops[p].extrinsic =
                                u.uprops[p].extrinsic | wp->w_mask;
                            if ((p = w_blocks(obj, mask)) != 0)
                                u.uprops[p].blocked |= wp->w_mask;
                        }
                        if (obj->oartifact)
                            set_artifact_intrinsic(obj, 1, mask);
                        
                    }
                    */
                }
            }
        }
    }

    /* No need to go further if verbose_and_update_stats == FALSE, as we are in newgame or restoring a saved game */
    if (!verbose_and_update_stats)
        return;

    boolean needbecomecursedmsg = FALSE;
    /* curse first */
    if (obj && (objects[obj->otyp].oc_flags & O1_BECOMES_CURSED_WHEN_WORN) && !obj->cursed && (mask & (W_WEP | W_WEP2 | W_ARMOR | W_ACCESSORY)))
    {
        needbecomecursedmsg = TRUE;
        curse(obj);
    }

    /* Readying a weapon to quiver or swap weapon slot does not trigger artifact name discovery -- JG */
    if ((mask & (W_WEP | W_WEP2 | W_ARMOR | W_ACCESSORY)) && obj && obj->oartifact && !obj->nknown && (artilist[obj->oartifact].aflags & (AF_FAMOUS | AF_NAME_KNOWN_WHEN_PICKED_UP | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED)))
    {
        if (verbose_and_update_stats)
        {
            play_sfx_sound(SFX_ARTIFACT_NAME_KNOWN);
            pline_ex(ATR_NONE, CLR_MSG_HINT, "As you %s %s, you become aware that %s named %s!",
                (mask == W_WEP || (u.twoweap && mask == W_WEP2)) ? "wield" : "wear", the(cxname(obj)),
                (pair_of(obj) || obj->quan > 1) ? "they are" : "it is",
                bare_artifactname(obj));
        }
        obj->nknown = TRUE;
    }

    update_all_character_properties(obj, verbose_and_update_stats);

    /* Note that this does not work for weapons if there is an old weapon, since we do not know whether the change was caused by the old or the new weapon */
    if ((obj && !oobj) || (oobj && !obj))
    {
        if ((
            u.uenmax != oldmanamax
            || u.uhpmax != oldhpmax
            || ACURR(A_STR) != oldstr
            || ACURR(A_DEX) != olddex
            || ACURR(A_CON) != oldcon
            || ACURR(A_INT) != oldint
            || ACURR(A_WIS) != oldwis
            || ACURR(A_CHA) != oldcha
            || (obj && obj->oclass != ARMOR_CLASS && u.uac != oldac)
            || (obj && obj->oclass != ARMOR_CLASS && obj->oclass != WEAPON_CLASS && u.umc != oldmc)
            || (!obj && oobj && oobj->oclass != ARMOR_CLASS && u.uac != oldac)
            || (!obj && oobj && oobj->oclass != ARMOR_CLASS && oobj->oclass != WEAPON_CLASS && u.umc != oldmc)
            )) // this should identify all objects giving hp or mana or stats or non-armors giving ac or mc
        {
            if (obj)
            {
                if (obj->oclass == RING_CLASS || obj->oclass == MISCELLANEOUS_CLASS) //Observable ring
                    learnring(obj, TRUE);
                else
                    makeknown(obj->otyp);
            }
            else if (oobj)
            {
                if (oobj->oclass == RING_CLASS || oobj->oclass == MISCELLANEOUS_CLASS) //Observable ring
                    learnring(oobj, TRUE);
                else
                    makeknown(oobj->otyp);
            }
        }
        else
        {
            if (obj)
            {
                if (obj->oclass == RING_CLASS || obj->oclass == MISCELLANEOUS_CLASS)
                {
                    //Nonobservable ring
                    learnring(obj, FALSE);
                }
            }
            else if (oobj)
            {
                if (oobj->oclass == RING_CLASS || oobj->oclass == MISCELLANEOUS_CLASS)
                {
                    //Nonobservable ring
                    learnring(oobj, FALSE);
                }
            }
        }
        context.botl = context.botlx = 1;
        if (needbecomecursedmsg)
        {
            if (Blind)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s for a moment.", Tobjnam(obj, "vibrate"));
            else
                pline_multi_ex(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_NEGATIVE, no_multiattrs, multicolor_buffer, "%s %s for a moment.", Tobjnam(obj, "glow"),
                    hcolor_multi_buf1(NH_BLACK));
        }
    }

    update_inventory();
}


void
setnotworn(obj)
register struct obj* obj;
{
    setnotworncore(obj, TRUE);
}

void
setnotwornquietly(obj)
register struct obj* obj;
{
    setnotworncore(obj, FALSE);
}

/* called e.g. when obj is destroyed */
/* Updated to use the extrinsic and blocked fields. */
void
setnotworncore(obj, verbose)
register struct obj *obj;
boolean verbose;
{
    register const struct worn *wp;
//    register int p;

    if (!obj)
        return;

    int oldmanamax = u.uenmax;
    int oldhpmax = u.uhpmax;
    int oldstr = ACURR(A_STR);
    int olddex = ACURR(A_DEX);
    int oldcon = ACURR(A_CON);
    int oldint = ACURR(A_INT);
    int oldwis = ACURR(A_WIS);
    int oldcha = ACURR(A_CHA);
    int oldac = u.uac;
    int oldmc = u.umc;

    for (wp = worn; wp->w_mask; wp++)
    {
        if (obj == *(wp->w_obj)) {
            /* in case wearing or removal is in progress or removal
               is pending (via 'A' command for multiple items) */
            cancel_doff(obj, wp->w_mask);

            *(wp->w_obj) = 0;
            /*
            p = objects[obj->otyp].oc_oprop;
            u.uprops[p].extrinsic = u.uprops[p].extrinsic & ~wp->w_mask;

            p = objects[obj->otyp].oc_oprop2;
            u.uprops[p].extrinsic = u.uprops[p].extrinsic & ~wp->w_mask;

            p = objects[obj->otyp].oc_oprop3;
            u.uprops[p].extrinsic = u.uprops[p].extrinsic & ~wp->w_mask;
            */

            obj->owornmask &= ~wp->w_mask;

            /*
            if (obj->oartifact)
                set_artifact_intrinsic(obj, 0, wp->w_mask);
            if ((p = w_blocks(obj, wp->w_mask)) != 0)
                u.uprops[p].blocked &= ~wp->w_mask;
            */
        }
    }

    update_all_character_properties(obj, verbose);

    //int curstr = ACURR(A_STR);

    if (obj)
    {
        if ((
            u.uenmax != oldmanamax
            || u.uhpmax != oldhpmax
            || ACURR(A_STR) != oldstr
            || ACURR(A_DEX) != olddex
            || ACURR(A_CON) != oldcon
            || ACURR(A_INT) != oldint
            || ACURR(A_WIS) != oldwis
            || ACURR(A_CHA) != oldcha
            || (obj->oclass != ARMOR_CLASS && u.uac != oldac)
            || (obj->oclass != ARMOR_CLASS && obj->oclass != WEAPON_CLASS && u.umc != oldmc)
            )) // this should identify all objects giving hp or mana or stats or ac
        {
            if (obj->oclass == RING_CLASS || obj->oclass == MISCELLANEOUS_CLASS) //Observable ring
                learnring(obj, TRUE);
            else
                makeknown(obj->otyp);
        }
        else if (obj->oclass == RING_CLASS || obj->oclass == MISCELLANEOUS_CLASS)
        {
            //Nonobservable ring
            learnring(obj, FALSE);
        }
        context.botl = context.botlx = 1;
    }
    update_inventory();
}

/* return item worn in slot indiciated by wornmask; needed by poly_obj() */
struct obj *
wearmask_to_obj(wornmask)
int64_t wornmask;
{
    const struct worn *wp;

    for (wp = worn; wp->w_mask; wp++)
        if (wp->w_mask & wornmask)
            return *wp->w_obj;
    return (struct obj *) 0;
}

/* return a bitmask of the equipment slot(s) a given item might be worn in */
int64_t
wearslot(obj)
struct obj *obj;
{
    int otyp = obj->otyp;
    /* practically any item can be wielded or quivered; it's up to
       our caller to handle such things--we assume "normal" usage */
    int64_t res = 0L; /* default: can't be worn anywhere */

    switch (obj->oclass) {
    case AMULET_CLASS:
        res = W_AMUL; /* WORN_AMUL */
        break;
    case MISCELLANEOUS_CLASS:
        res = W_MISCITEMS;
        break;
    case RING_CLASS:
        res = W_RINGL | W_RINGR; /* W_RING, BOTH_SIDES */
        break;
    case ARMOR_CLASS:
        switch (objects[otyp].oc_armor_category) {
        case ARM_SUIT:
            res = W_ARM;
            break; /* WORN_ARMOR */
        case ARM_SHIELD:
            res = W_ARMS;
            break; /* WORN_SHIELD */
        case ARM_HELM:
            res = W_ARMH;
            break; /* WORN_HELMET */
        case ARM_GLOVES:
            res = W_ARMG;
            break; /* WORN_GLOVES */
        case ARM_BOOTS:
            res = W_ARMF;
            break; /* WORN_BOOTS */
        case ARM_CLOAK:
            res = W_ARMC;
            break; /* WORN_CLOAK */
        case ARM_SHIRT:
            res = W_ARMU;
            break; /* WORN_SHIRT */
        case ARM_ROBE:
            res = W_ARMO;
            break; /* WORN_ROBE */
        case ARM_BRACERS:
            res = W_ARMB;
            break; /* BRACERS */
        }
        break;
    case WEAPON_CLASS:
        res = W_WEP | W_SWAPWEP;
        if (objects[otyp].oc_merge)
            res |= W_QUIVER;
        if (u.twoweap)
            res |= W_WEP2 | W_SWAPWEP2;
        break;
    case TOOL_CLASS:
        if (otyp == BLINDFOLD || otyp == TOWEL)
            res = W_BLINDFOLD; /* WORN_BLINDF */
        else if (is_weptool(obj) || otyp == TIN_OPENER)
            res = W_WEP | W_SWAPWEP;
        else if (otyp == SADDLE)
            res = W_SADDLE;
        break;
    case FOOD_CLASS:
        if (obj->otyp == MEAT_RING)
            res = W_RINGL | W_RINGR;
        break;
    case GEM_CLASS:
        res = W_QUIVER;
        break;
    case BALL_CLASS:
        res = W_BALL;
        break;
    case CHAIN_CLASS:
        res = W_CHAIN;
        break;
    default:
        break;
    }
    return res;
}

void
nonadditive_increase_mon_property(mon, prop_index, amount)
struct monst* mon;
int prop_index;
int amount;
{
    set_mon_property(mon, prop_index, max(get_mon_property(mon, prop_index), amount));
}

boolean
nonadditive_increase_mon_property_verbosely(mtmp, prop_index, amount)
struct monst* mtmp;
int prop_index;
int amount;
{
    return set_mon_property_verbosely(mtmp, prop_index, max(get_mon_property(mtmp, prop_index), amount));
}

boolean
nonadditive_increase_mon_property_b(mtmp, prop_index, duration, verbose)
struct monst* mtmp;
int prop_index;
int duration;
boolean verbose;
{
    if (verbose)
        return nonadditive_increase_mon_property_verbosely(mtmp, prop_index, duration);
    else
    {
        nonadditive_increase_mon_property(mtmp, prop_index, duration);
        return FALSE;
    }
}


void
increase_mon_property(mtmp, prop_index, duration)
struct monst* mtmp;
int prop_index;
int duration;
{
    int maxvalue = (int)M_TIMEOUT;
    unsigned short value = (unsigned short)max(0, min(maxvalue, (get_mon_property(mtmp, prop_index) + duration)));

    set_mon_property(mtmp, prop_index, value);
}

boolean
increase_mon_property_verbosely(mtmp, prop_index, duration)
struct monst* mtmp;
int prop_index;
int duration;
{
    int maxvalue = (int)M_TIMEOUT;
    unsigned short value = (unsigned short)max(0, min(maxvalue, (get_mon_property(mtmp, prop_index) + duration)));

    return set_mon_property_verbosely(mtmp, prop_index, value);
}

boolean
increase_mon_property_b(mtmp, prop_index, duration, verbose)
struct monst* mtmp;
int prop_index;
int duration;
boolean verbose;
{
    if (verbose)
        return increase_mon_property_verbosely(mtmp, prop_index, duration);
    else
    {
        increase_mon_property(mtmp, prop_index, duration);
        return 0;
    }
}



STATIC_OVL void
set_mon_temporary_property(mon, prop_index, amount)
struct monst* mon;
int prop_index;
unsigned short amount;
{
    if (!mon)
        return;

    if (prop_index < 1 || prop_index > LAST_PROP)
        return;

    if (mon == &youmonst)
    {
        set_itimeout(&u.uprops[prop_index].intrinsic, amount);
        refresh_u_tile_gui_info(TRUE);
        return;
    }

    if (amount > M_TIMEOUT)
        amount = M_TIMEOUT;

    unsigned short otherflags = mon->mprops[prop_index] & ~M_TIMEOUT;
    mon->mprops[prop_index] = (amount | otherflags);
}


boolean
set_mon_property_b(mtmp, prop_index, value, verbose)
struct monst* mtmp;
int prop_index;
int value; /* -1 sets the intrinsic and -2 clears it; -3 clears both temporary and permanent instrinsic */
boolean verbose;
{
    if (verbose)
        return set_mon_property_verbosely(mtmp, prop_index, value);
    else
    {
        set_mon_property(mtmp, prop_index, value);
        return FALSE;
    }
}

void
set_mon_property(mtmp, prop_index, value)
struct monst* mtmp;
int prop_index;
int value; /* -1 sets the intrinsic and -2 clears it; -3 clears both temporary and permanent instrinsic */
{

    if (!mtmp)
        return;

    if (prop_index < 1 || prop_index > LAST_PROP)
        return;

    boolean was_tame = is_tame(mtmp);

    if (value >= 0)
        set_mon_temporary_property(mtmp, prop_index, min((unsigned short)M_TIMEOUT, (unsigned short)value));
    else if (value == -1)
        mtmp->mprops[prop_index] |= M_INTRINSIC_ACQUIRED;
    else if (value == -2)
        mtmp->mprops[prop_index] &= ~M_INTRINSIC_ACQUIRED;
    else if (value == -3)
    {
        mtmp->mprops[prop_index] &= ~M_INTRINSIC_ACQUIRED;
        set_mon_temporary_property(mtmp, prop_index, 0);
    }

    /* Adjustments */
    if (was_tame && !is_tame(mtmp))
    {
        newsym(mtmp->mx, mtmp->my);

    }
    else if (is_tame(mtmp) && !was_tame)
    {
        newsym(mtmp->mx, mtmp->my);
    }

}

int
get_mon_property(mon, prop_index)
struct monst* mon;
int prop_index;
{
    if (!mon)
        return 0;

    if (prop_index < 1 || prop_index > LAST_PROP)
        return 0;

    if (mon == &youmonst)
    {
        return (u.uprops[prop_index].intrinsic & TIMEOUT);
    }

    unsigned short amount = mon->mprops[prop_index] & M_TIMEOUT;
    return (int)amount;
}

boolean
set_mon_property_verbosely(mtmp, prop_index, value)
struct monst* mtmp;
int prop_index;
int value; /* -1 sets the intrinsic and -2 clears it; -3 clears both temporary and permanent instrinsic */
{
    return verbose_wrapper(VERBOSE_FUNCTION_SET_MON_PROPERTY, mtmp, prop_index, value, FALSE);
}

/* return TRUE if a visible effect (something was printed in pline) */
boolean
verbose_wrapper(function_choice, mtmp, prop_index, value, silently)
enum verbose_function_types function_choice;
struct monst* mtmp;
int prop_index;
int value;
boolean silently;
{
    boolean res = FALSE;

    /* works for fast, very fast, and slowed */
    char savedname[BUFSZ * 2] = "";
    Strcpy(savedname, mon_nam(mtmp));

    char SavedName[BUFSZ * 2] = "";
    Strcpy(SavedName, Monnam(mtmp));

    boolean could_spot_mon = canspotmon(mtmp);
    boolean was_invisible = is_invisible(mtmp);
    boolean was_stoning = is_stoning(mtmp);
    boolean was_turning_into_slime = is_turning_into_slime(mtmp);
    boolean was_strangled = is_being_strangled(mtmp);
    boolean was_suffocating = is_suffocating(mtmp);
    boolean was_fast = is_fast(mtmp);
    boolean was_very_fast = is_very_fast(mtmp);
    boolean was_ultra_fast = is_ultra_fast(mtmp);
    boolean was_super_fast = is_super_fast(mtmp);
    boolean was_lightning_fast = is_lightning_fast(mtmp);
    boolean was_slow = is_slow(mtmp);
    boolean was_sleeping = is_sleeping(mtmp);
    boolean was_stunned = is_stunned(mtmp);
    boolean was_paralyzed = is_paralyzed(mtmp);
    boolean was_blinded = is_blinded(mtmp);
    boolean was_confused = is_confused(mtmp);
    boolean was_hallucinating = is_hallucinating(mtmp);
    boolean was_levitating = is_levitating(mtmp);
    boolean was_flying = is_flying(mtmp);
    boolean was_sick = is_sick(mtmp);
    boolean was_mummy_rotted = is_mummy_rotted(mtmp);
    boolean was_fearful = is_fearful(mtmp);
    boolean was_fleeing = is_fleeing(mtmp);
    boolean was_charmed = is_charmed(mtmp);
    boolean was_tame = is_tame(mtmp);
    boolean was_peaceful = is_peaceful(mtmp);
    boolean was_silenced = is_silenced(mtmp);
    boolean was_cancelled = is_cancelled(mtmp);
    boolean was_crazed = is_crazed(mtmp);
    boolean was_heroic = is_heroic(mtmp);
    boolean was_super_heroic = is_super_heroic(mtmp);


    switch (function_choice)
    {
    case VERBOSE_FUNCTION_UPDATE_MON_STATISTICS:
        update_all_mon_statistics_core(mtmp, silently);
        break;
    case VERBOSE_FUNCTION_SET_MON_PROPERTY:
        set_mon_property(mtmp, prop_index, value);
        break;
    default:
        break;
    }

    refresh_m_tile_gui_info(mtmp, !silently);

    if (silently)
        return FALSE;

    if (canspotmon(mtmp))
    {
        if (!could_spot_mon)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly, you can see %s!", mon_nam(mtmp));
        }
        else
        {
            /* Most such messages here */
            if (is_invisible(mtmp) && !was_invisible && knowninvisible(mtmp))
            {
                res = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s turns transparent!", SavedName);
            }
            else if (!is_invisible(mtmp) && was_invisible)
            {
                res = TRUE;
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s body loses its transparency!", s_suffix(Monnam(mtmp)));
            }
        }

        /* Stoned */
        if (is_stoning(mtmp) && !was_stoning)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s starts turning into stone!", Monnam(mtmp));
        }
        else if (!is_stoning(mtmp) && was_stoning)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s stops turning into stone.", Monnam(mtmp));
        }

        /* Slimed */
        if (is_turning_into_slime(mtmp) && !was_turning_into_slime)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s is turning into green slime!", Monnam(mtmp));
        }
        else if (!is_turning_into_slime(mtmp) && was_turning_into_slime)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s is not turning into green slime anymore.", Monnam(mtmp));
        }

        /* Speed */
        if (
            (!is_slow(mtmp) && was_slow)
            )
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : is_peaceful(mtmp) ? CLR_MSG_ATTENTION : CLR_MSG_WARNING, 
                "%s is moving %sfaster.", Monnam(mtmp),
                is_fast(mtmp) || is_very_fast(mtmp) || is_ultra_fast(mtmp) || is_super_fast(mtmp) || is_lightning_fast(mtmp) ? "much " : "");
        }
        else if (
            (is_fast(mtmp) && !was_fast && !was_very_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast)
            || (is_very_fast(mtmp) && !was_very_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast)
            || (is_ultra_fast(mtmp) && !was_ultra_fast && !was_super_fast && !was_lightning_fast)
            || (is_super_fast(mtmp) && !was_super_fast && !was_lightning_fast)
            || (is_lightning_fast(mtmp) && !was_lightning_fast)
            )
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : is_peaceful(mtmp) ? CLR_MSG_ATTENTION : CLR_MSG_WARNING, 
                "%s is moving faster.", Monnam(mtmp));
        }
        else if (
            (!is_fast(mtmp) && !is_very_fast(mtmp) && !is_ultra_fast(mtmp) && !is_super_fast(mtmp) && !is_lightning_fast(mtmp) && (was_fast || was_very_fast || was_ultra_fast || was_super_fast || was_lightning_fast))
            || (!is_very_fast(mtmp) && !is_ultra_fast(mtmp) && !is_super_fast(mtmp) && !is_lightning_fast(mtmp) && (was_very_fast || was_ultra_fast || was_super_fast || was_lightning_fast))
            || (!is_ultra_fast(mtmp) && !is_super_fast(mtmp) && !is_lightning_fast(mtmp) && (was_ultra_fast || was_super_fast || was_lightning_fast))
            || (!is_super_fast(mtmp) && !is_lightning_fast(mtmp) && (was_super_fast || was_lightning_fast))
            || (!is_lightning_fast(mtmp) && (was_lightning_fast))
            )
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is moving slower.", Monnam(mtmp));
        }
        else if (is_slow(mtmp) && !was_slow)
        {
            res = TRUE;
            if ((prop_index == STONED || prop_index == SLIMED) && value > 0)
                pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s is slowing down!", Monnam(mtmp));
            else if (prop_index == SLOWED && value > 0)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s slows down%s.", Monnam(mtmp), was_fast || was_very_fast ? " a lot" : "");
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is moving %sslower.", Monnam(mtmp), was_fast || was_very_fast ? "much " : "");
        }

        /* Silenced */
        if (is_silenced(mtmp) && !was_silenced)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s voice disappears.", s_suffix(Monnam(mtmp)));
        }
        else if (!is_silenced(mtmp) && was_silenced)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s voice returns.", s_suffix(Monnam(mtmp)));
        }

        /* Cancellation */
        if (is_cancelled(mtmp) && !was_cancelled)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s magic seems to stop flowing properly.", s_suffix(Monnam(mtmp)));
        }
        else if (!is_cancelled(mtmp) && was_cancelled)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s magic seems to start flowing properly.", s_suffix(Monnam(mtmp)));
        }

        /* Crazedness */
        if (is_crazed(mtmp) && !was_crazed)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s looks crazed.", Monnam(mtmp));
        }
        else if (!is_crazed(mtmp) && was_crazed)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s looks more sane.", Monnam(mtmp));
        }

        /* Heroism */
        if (is_super_heroic(mtmp) && !was_super_heroic)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_WARNING, "%s looks super-heroic.", Monnam(mtmp));
        }
        else if (!is_super_heroic(mtmp) && was_super_heroic)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks less super-heroic than before.", Monnam(mtmp));
        }
        else if (!is_heroic(mtmp) && !is_super_heroic(mtmp) && was_heroic)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks less heroic than before.", Monnam(mtmp));
        }
        else if (is_heroic(mtmp) && !was_heroic && !was_super_heroic && !is_super_heroic(mtmp))
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_WARNING, "%s looks heroic.", Monnam(mtmp));
        }

        /* Sleeping */
        if (is_sleeping(mtmp) && !was_sleeping)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s falls asleep.", Monnam(mtmp));
        }
        else if (!is_sleeping(mtmp) && was_sleeping)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s wakes up.", Monnam(mtmp));
        }

        /* Paralysis */
        if (is_paralyzed(mtmp) && !was_paralyzed)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s is paralyzed!", Monnam(mtmp));
        }
        else if (!is_paralyzed(mtmp) && was_paralyzed)
        {
            res = TRUE;
            if (mon_can_move(mtmp))
                pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s can move again!", Monnam(mtmp));
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is no longer paralyzed!", Monnam(mtmp));
        }

        /* Blindness */
        if (is_blinded(mtmp) && !was_blinded)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s is blinded!", Monnam(mtmp));
        }
        else if (!has_blinded(mtmp) && was_blinded)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s can see again!", Monnam(mtmp));
        }

        /* Stunned */
        if (is_stunned(mtmp) && !was_stunned)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s is stunned!", Monnam(mtmp));
        }
        else if (!is_stunned(mtmp) && was_stunned)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s looks less stunned.", Monnam(mtmp));
        }

        /* Confusion */
        if (is_confused(mtmp) && !was_confused)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_WARNING : CLR_MSG_ATTENTION, "%s is confused!", Monnam(mtmp));
        }
        else if (!is_confused(mtmp) && was_confused)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s looks less confused.", Monnam(mtmp));
        }

        /* Hallucination */
        if (is_hallucinating(mtmp) && !was_hallucinating)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s looks seriously confused!", Monnam(mtmp));
        }
        else if (!is_hallucinating(mtmp) && was_hallucinating)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s looks more straight-minded.", Monnam(mtmp));
        }


        /* Fearful*/
        if (is_fleeing(mtmp) && !was_fleeing)
        {
            if (M_AP_TYPE(mtmp) != M_AP_FURNITURE && M_AP_TYPE(mtmp) != M_AP_OBJECT)
            {
                res = TRUE;
                if (!mon_can_move(mtmp) || !mtmp->data->mmove)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks frightened.", Monnam(mtmp));
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %sturns to flee.", Monnam(mtmp), is_fearful(mtmp) ? "looks frightened and " : "");
            }
        }
        else if (is_fearful(mtmp) && !was_fearful && was_fleeing)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks even more frightened than before.", Monnam(mtmp));
        }
        else if (!is_fleeing(mtmp) && was_fleeing)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %sstops fleeing.", Monnam(mtmp), !is_fearful(mtmp) && was_fearful ? "looks less frightened and " : "");
        }


        /* Charm */
        if (is_charmed(mtmp) && !was_charmed)
        {
            res = TRUE;
            pline("%s is charmed!", Monnam(mtmp));
            if (is_tame(mtmp) && !was_tame)
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s looks friendly.", Monnam(mtmp));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s looks %s for a while.", Monnam(mtmp), is_tame(mtmp) ? "a little perplexed" :
                    is_peaceful(mtmp) ? "a little uncomfortable" : "uncomfortable");
        }
        if (!is_charmed(mtmp) && was_charmed)
        {
            res = TRUE;
            if (is_tame(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks perplexed for a while.", Monnam(mtmp));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s looks more in control of %sself.", Monnam(mtmp), mhim(mtmp));

            if (!is_peaceful(mtmp) && was_peaceful)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s turns hostile!", Monnam(mtmp));
        }


        /* Levitation */
        if (is_levitating(mtmp) && !was_levitating)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s starts levitating.", Monnam(mtmp));
        }
        else if (!is_levitating(mtmp) && was_levitating)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stops levitating.", Monnam(mtmp));
        }

        /* Flying */
        if (is_flying(mtmp) && !was_flying)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s starts flying.", Monnam(mtmp));
        }
        else if (!is_flying(mtmp) && was_flying)
        {
            res = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stops flying.", Monnam(mtmp));
        }

        if (is_being_strangled(mtmp) && !was_strangled)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s is being strangled to death!", Monnam(mtmp));
        }
        else if (!is_being_strangled(mtmp) && was_strangled)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s stops being strangled.", Monnam(mtmp));
        }

        if (is_suffocating(mtmp) && !was_suffocating)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s is suffocating!", Monnam(mtmp));
        }
        else if (!is_suffocating(mtmp) && was_suffocating)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s stops being suffocated.", Monnam(mtmp));
        }

        if (is_sick(mtmp) && !was_sick)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s looks terminally ill!", Monnam(mtmp));
        }
        else if (!is_sick(mtmp) && was_sick)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s is cured of %s terminal illness.", Monnam(mtmp), mhis(mtmp));
        }

        if (is_mummy_rotted(mtmp) && !was_mummy_rotted)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_NEGATIVE : CLR_MSG_ATTENTION, "%s looks severely ill!", Monnam(mtmp));
        }
        else if (!is_mummy_rotted(mtmp) && was_mummy_rotted)
        {
            res = TRUE;
            pline_ex(ATR_NONE, is_tame(mtmp) ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, "%s is cured of %s rot-causing illness.", Monnam(mtmp), mhis(mtmp));
        }

        /* Cancelled */
        /* Half magic resistance */
        /* No magic resistance */
        /* Summoning is forbidden */
        /* Deaf */
        /* Sick */
        /* Vomiting */
        /* Glib */

    }
    else if (could_spot_mon)
    {
        res = TRUE;
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly, you cannot see %s anymore!", savedname);
    }

    if (res)
    {
        newsym(mtmp->mx, mtmp->my);
        flush_screen(1); /* Make sure you see the change immediately */
    }

    return res;
}


void
update_all_mon_statistics_core(mon, silently)
struct monst* mon;
boolean silently;
{
    update_mon_extrinsics(mon, silently);
    update_mon_abon(mon);
    /* monster do not currently have mana */
    update_mon_maxhp(mon);
}

void
update_all_mon_statistics(mon, silently)
struct monst* mon;
boolean silently;
{
    (void)verbose_wrapper(VERBOSE_FUNCTION_UPDATE_MON_STATISTICS, mon, 0, 0, silently);
}


/* armor put on or taken off; might be magical variety */
void
update_mon_extrinsics(mon, silently)
struct monst *mon;
boolean silently;
{

    if (!mon)
        return;

    int unseen = 0;
    struct obj *otmp = (struct obj*)0;
    boolean was_tame = is_tame(mon);

    /* clear mon extrinsics */
    for (int i = 1; i <= LAST_PROP; i++)
    {
        mon->mprops[i] &= ~M_EXTRINSIC;
    }

    /* add them all back*/
    for (otmp = mon->minvent; otmp; otmp = otmp->nobj)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (!otmp->oartifact && i >= 6)
                break;

            int otyp = otmp->otyp;
            uchar which = 0;
            boolean inappr = inappropriate_monster_character_type(mon, otmp);
            boolean yields_power = FALSE;
            boolean wornrequired = TRUE;

            switch (i)
            {
            case 1:
                which = objects[otyp].oc_oprop;
                if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_ALL_CHARACTERS)
                    yields_power = TRUE;
                else if (inappr && (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
                    yields_power = TRUE;
                else if (!inappr && !(objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
                    yields_power = TRUE;

                if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_WHEN_CARRIED)
                    wornrequired = FALSE;
                break;
            case 2:
                which = objects[otyp].oc_oprop2;
                if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_ALL_CHARACTERS)
                    yields_power = TRUE;
                else if (inappr && (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
                    yields_power = TRUE;
                else if (!inappr && !(objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
                    yields_power = TRUE;

                if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_WHEN_CARRIED)
                    wornrequired = FALSE;
                break;
            case 3:
                which = objects[otyp].oc_oprop3;
                if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_ALL_CHARACTERS)
                    yields_power = TRUE;
                else if (inappr && (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
                    yields_power = TRUE;
                else if (!inappr && !(objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
                    yields_power = TRUE;

                if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_WHEN_CARRIED)
                    wornrequired = FALSE;
                break;
            case 4:
            case 5:
                yields_power = FALSE; /* Set them separately below */
                if (otmp->owornmask)
                {
                    /* Mythic */
                    boolean isprefix = (i == 4);
                    uchar mythic_quality = (isprefix ? otmp->mythic_prefix : otmp->mythic_suffix);
                    if (mythic_quality == 0)
                        continue;

                    const struct mythic_power_definition* mythic_powers = (isprefix ? mythic_prefix_powers : mythic_suffix_powers);
                    const struct mythic_definition* mythic_definitions = (isprefix ? mythic_prefix_qualities : mythic_suffix_qualities);
                    uchar max_mythic_powers = (isprefix ? MAX_MYTHIC_PREFIX_POWERS : MAX_MYTHIC_SUFFIX_POWERS);

                    for (uchar k = 0; k < max_mythic_powers; k++)
                    {
                        if (!mythic_powers[k].name)
                            break;

                        uint64_t mythic_power_bit = (uint64_t)1 << ((uint64_t)k);

                        if ((mythic_definitions[mythic_quality].mythic_powers & mythic_power_bit) && mythic_power_applies_to_obj(otmp, mythic_powers[k].power_flags))
                        {
                            if (mythic_powers[k].power_type == MYTHIC_POWER_TYPE_CONFERS_PROPERTY)
                            {
                                mon->mprops[mythic_powers[k].parameter1] |= M_EXTRINSIC;
                            }
                        }
                    }
                }
                break;
            case 6: /* Artifact carried */
                if (otmp->oartifact)
                {
                    which = artilist[otmp->oartifact].carried_prop;
                    wornrequired = FALSE;
                    yields_power = TRUE;
                }
                break;
            case 7: /* Artifact worn */
                if (otmp->oartifact)
                {
                    which = artilist[otmp->oartifact].worn_prop;
                    wornrequired = TRUE;
                    yields_power = TRUE;
                }
                break;
            case 8: /* Artifact invoked */
                if (otmp->oartifact)
                {
                    which = otmp->invokeon && artilist[otmp->oartifact].inv_prop > 0 ? artilist[otmp->oartifact].inv_prop : 0;
                    wornrequired = FALSE;
                    yields_power = TRUE;
                }
                break;
            case 9: /* Artifact spfx and cspfx */
                yields_power = FALSE; /* Handled separately below */
                if (otmp->oartifact)
                {
                    int k;
                    if (otmp->owornmask)
                    {
                        for (k = 0; k < 32; k++)
                        {
                            uint64_t bit = (uint64_t)1 << k;
                            int propnum = spfx_to_prop(bit);
                            if (artilist[otmp->oartifact].spfx & bit)
                                mon->mprops[propnum] |= M_EXTRINSIC;
                        }
                    }

                    for (k = 0; k < 32; k++)
                    {
                        uint64_t bit = (uint64_t)1 << k;
                        int propnum = spfx_to_prop(bit);
                        if (artilist[otmp->oartifact].cspfx & bit)
                            mon->mprops[propnum] |= M_EXTRINSIC;
                    }
                }
                break;
            default:
                break;

            }

            if (yields_power && (!wornrequired || (wornrequired && otmp->owornmask)))
            {
                /* OK */
            }
            else
            {
                continue;
            }

            unseen = !canseemon(mon);
            mon->mprops[which] |= M_EXTRINSIC;
        }
    }

    if (mon->wormno)
        see_wsegs(mon); /* and any tail too */

    /* if couldn't see it but now can, or vice versa, update display */
    if (!silently && (unseen ^ !canseemon(mon)))
        newsym(mon->mx, mon->my);

    /* Adjustments */
    if (was_tame && !is_tame(mon))
    {
        newsym(mon->mx, mon->my);

    }
    else if (is_tame(mon) && !was_tame)
    {
        newsym(mon->mx, mon->my);
    }


}


int
find_mac(mon)
register struct monst *mon;
{
    register struct obj *obj;
    int natural_ac_base = mon->data->ac;
    int natural_ac = natural_ac_base;
    int armor_bonus = 0;
    int armor_ac = 10;
    int mac = 0;
    int64_t mwflags = mon->worn_item_flags;

    for (obj = mon->minvent; obj; obj = obj->nobj) {
        if (obj->owornmask & mwflags)
            armor_bonus += ARM_AC_BONUS(obj, mon->data);
        /* since ARM_AC_BONUS is positive, subtracting it increases AC */
    }

    natural_ac -= armor_bonus / 3;
    armor_ac -= (armor_bonus + ((10 - natural_ac_base) / 3));

    if (natural_ac <= armor_ac)
        mac = natural_ac;
    else
        mac = armor_ac;

    //DEX bonus for monsters, reduce the number from AC; not add!
    if(mon_can_move(mon))
        mac -= dexterity_ac_bonus(m_acurr(mon, A_DEX));

    mac -= mon->macbonus;

    if (mon->mprops[MAGICAL_STONESKIN])
        mac -= MAGICAL_STONESKIN_AC_BONUS;
    else if (mon->mprops[MAGICAL_BARKSKIN])
        mac -= MAGICAL_BARKSKIN_AC_BONUS;
    else if (mon->mprops[MAGICAL_SHIELDING])
        mac -= MAGICAL_SHIELDING_AC_BONUS;
    else if (mon->mprops[MAGICAL_PROTECTION])
        mac -= MAGICAL_PROTECTION_AC_BONUS;

    return mac;
}


/*
 * weapons are handled separately;
 * rings and eyewear aren't used by monsters
 */

/* Wear the best object of each type that the monster has.  During creation,
 * the monster can put everything on at once; otherwise, wearing takes time.
 * This doesn't affect monster searching for objects--a monster may very well
 * search for objects it would not want to wear, because we don't want to
 * check which_armor() each round.
 *
 * We'll let monsters put on shirts and/or suits under worn cloaks, but
 * not shirts under worn suits.  This is somewhat arbitrary, but it's
 * too tedious to have them remove and later replace outer garments,
 * and preventing suits under cloaks makes it a little bit too easy for
 * players to influence what gets worn.  Putting on a shirt underneath
 * already worn body armor is too obviously buggy...
 */
void
m_dowear(mon, creation, commanded)
register struct monst *mon;
boolean creation, commanded;
{
#define RACE_EXCEPTION TRUE
    /* Note the restrictions here are the same as in dowear in do_wear.c
     * except for the additional restriction on intelligence.  (Players
     * are always intelligent, even if polymorphed).
     */
    if (!can_wear_objects(mon->data))
        return;

    if (mon->mfrozen)
        return;

    /* give mummies a chance to wear their wrappings
     * and let skeletons wear their initial armor */
    if (mindless(mon->data) && (!creation && !commanded)) // !(mon->data->mlet == S_GREATER_UNDEAD || mon->data->mlet == S_LESSER_UNDEAD)))
        return;

    boolean wears_shirt = FALSE;
    boolean wears_suit = FALSE;
    boolean wears_robe = FALSE;
    boolean wears_cloak = FALSE;
    boolean wears_gloves = FALSE;
    boolean wears_helmet = FALSE;
    boolean wears_bracers = FALSE;
    boolean wears_boots = FALSE;
    boolean wears_shield = FALSE;
    boolean wears_amulet = FALSE;
    boolean wears_ringr = FALSE;
    boolean wears_ringl = FALSE;
    boolean wears_misc1 = FALSE;

    struct obj* old_shirt = which_armor(mon, W_ARMU);
    struct obj* old_suit = which_armor(mon, W_ARM);
    struct obj* old_robe = which_armor(mon, W_ARMO);
    struct obj* old_cloak = which_armor(mon, W_ARMC);
    struct obj* old_gloves = which_armor(mon, W_ARMG);
    struct obj* old_helmet = which_armor(mon, W_ARMH);
    struct obj* old_bracers = which_armor(mon, W_ARMB);
    struct obj* old_boots = which_armor(mon, W_ARMF);
    struct obj* old_shield = which_armor(mon, W_ARMS);
    struct obj* old_amulet = which_armor(mon, W_AMUL);
    struct obj* old_ringr = which_armor(mon, W_RINGR);
    struct obj* old_ringl = which_armor(mon, W_RINGL);
    struct obj* old_misc1 = which_armor(mon, W_MISC);

    int old_shirt_delay = old_shirt ? objects[old_shirt->otyp].oc_delay : 0;
    int old_suit_delay = old_suit ? objects[old_suit->otyp].oc_delay : 0;
    int old_robe_delay = old_robe ? objects[old_robe->otyp].oc_delay : 0;
    int old_cloak_delay = old_cloak ? objects[old_cloak->otyp].oc_delay : 0;
    int old_gloves_delay = old_gloves ? objects[old_gloves->otyp].oc_delay : 0;
    int old_helmet_delay = old_helmet ? objects[old_helmet->otyp].oc_delay : 0;
    int old_bracers_delay = old_bracers ? objects[old_bracers->otyp].oc_delay : 0;
    int old_boots_delay = old_boots ? objects[old_boots->otyp].oc_delay : 0;
    int old_shield_delay = old_shield ? objects[old_shield->otyp].oc_delay : 0;
    int old_amulet_delay = old_amulet ? objects[old_amulet->otyp].oc_delay : 0;
    int old_ringr_delay = old_ringr ? objects[old_ringr->otyp].oc_delay : 0;
    int old_ringl_delay = old_ringl ? objects[old_ringl->otyp].oc_delay : 0;
    int old_misc1_delay = old_misc1 ? objects[old_misc1->otyp].oc_delay : 0;

    /* Main armor */
    if (can_wear_shirt(mon->data) && (cursed_items_are_positive_mon(mon) || !((old_cloak && old_cloak->cursed) || (old_robe && old_robe->cursed) || (old_suit && old_suit->cursed))) )
    {
        wears_shirt = m_dowear_type(mon, W_ARMU, creation, FALSE);
    }

    if (can_wear_suit(mon->data) && (cursed_items_are_positive_mon(mon) || !((old_cloak && old_cloak->cursed) || (old_robe && old_robe->cursed))) )
        wears_suit = m_dowear_type(mon, W_ARM, creation, FALSE);
    else
        wears_suit = m_dowear_type(mon, W_ARM, creation, RACE_EXCEPTION);

    if (can_wear_robe(mon->data) && (cursed_items_are_positive_mon(mon) || !(old_cloak && old_cloak->cursed)))
    {
        wears_robe = m_dowear_type(mon, W_ARMO, creation, FALSE);
    }

    if (can_wear_cloak(mon->data))
    {
        wears_cloak = m_dowear_type(mon, W_ARMC, creation, FALSE);
    }

    /* Other armor types */
    if (can_wear_helmet(mon->data))
        wears_helmet = m_dowear_type(mon, W_ARMH, creation, FALSE);
    if (can_wear_shield(mon->data) && (!MON_WEP(mon) || !bimanual(MON_WEP(mon))))
        wears_shield = m_dowear_type(mon, W_ARMS, creation, FALSE);
    if (can_wear_gloves(mon->data) && !(MON_WEP(mon) && mwelded(MON_WEP(mon), mon)))
        wears_gloves = m_dowear_type(mon, W_ARMG, creation, FALSE);
    if (can_wear_boots(mon->data) && !slithy(mon->data) && mon->data->mlet != S_CENTAUR)
        wears_boots = m_dowear_type(mon, W_ARMF, creation, FALSE);
    if (can_wear_bracers(mon->data))
        wears_bracers = m_dowear_type(mon, W_ARMB, creation, FALSE);

    /* Accessories */
    if (can_wear_amulet(mon->data))
        wears_amulet = m_dowear_type(mon, W_AMUL, creation, FALSE);
    if (can_wear_rings(mon->data) && (cursed_items_are_positive_mon(mon) || (!(MON_WEP(mon) && mwelded(MON_WEP(mon), mon)) && !(old_gloves && old_gloves->cursed))))
        wears_ringr = m_dowear_type(mon, W_RINGR, creation, FALSE);
    if (can_wear_rings(mon->data) && (cursed_items_are_positive_mon(mon) || (!(MON_WEP(mon) && mwelded(MON_WEP(mon), mon)) && !(old_gloves && old_gloves->cursed))))
        wears_ringl = m_dowear_type(mon, W_RINGL, creation, FALSE);

    /* Always check miscellaneous */
    wears_misc1 = m_dowear_type(mon, W_MISC, creation, FALSE);

    update_all_mon_statistics(mon, creation);

    struct obj* new_shirt = which_armor(mon, W_ARMU);
    struct obj* new_suit = which_armor(mon, W_ARM);
    struct obj* new_robe = which_armor(mon, W_ARMO);
    struct obj* new_cloak = which_armor(mon, W_ARMC);
    struct obj* new_gloves = which_armor(mon, W_ARMG);
    struct obj* new_helmet = which_armor(mon, W_ARMH);
    struct obj* new_bracers = which_armor(mon, W_ARMB);
    struct obj* new_boots = which_armor(mon, W_ARMF);
    struct obj* new_shield = which_armor(mon, W_ARMS);
    struct obj* new_amulet = which_armor(mon, W_AMUL);
    struct obj* new_ringr = which_armor(mon, W_RINGR);
    struct obj* new_ringl = which_armor(mon, W_RINGL);
    struct obj* new_misc1 = which_armor(mon, W_MISC);

    int new_shirt_delay = new_shirt ? objects[new_shirt->otyp].oc_delay : 0;
    int new_suit_delay = new_suit ? objects[new_suit->otyp].oc_delay : 0;
    int new_robe_delay = new_robe ? objects[new_robe->otyp].oc_delay : 0;
    int new_cloak_delay = new_cloak ? objects[new_cloak->otyp].oc_delay : 0;
    int new_gloves_delay = new_gloves ? objects[new_gloves->otyp].oc_delay : 0;
    int new_helmet_delay = new_helmet ? objects[new_helmet->otyp].oc_delay : 0;
    int new_bracers_delay = new_bracers ? objects[new_bracers->otyp].oc_delay : 0;
    int new_boots_delay = new_boots ? objects[new_boots->otyp].oc_delay : 0;
    int new_shield_delay = new_shield ? objects[new_shield->otyp].oc_delay : 0;
    int new_amulet_delay = new_amulet ? objects[new_amulet->otyp].oc_delay : 0;
    int new_ringr_delay = new_ringr ? objects[new_ringr->otyp].oc_delay : 0;
    int new_ringl_delay = new_ringl ? objects[new_ringl->otyp].oc_delay : 0;
    int new_misc1_delay = new_misc1 ? objects[new_misc1->otyp].oc_delay : 0;

    boolean takes_off_old_suit = wears_shirt || wears_suit;
    boolean takes_off_old_robe = wears_shirt || wears_suit || wears_robe;
    boolean takes_off_old_cloak = wears_shirt || wears_suit || wears_robe || wears_cloak;

    int totaldelay = 0;
    totaldelay += takes_off_old_cloak ? old_cloak_delay : 0;
    totaldelay += takes_off_old_robe ? old_robe_delay : 0;
    totaldelay += takes_off_old_suit ? old_suit_delay : 0;
    totaldelay += wears_shirt ? old_shirt_delay + new_shirt_delay : 0;
    totaldelay += wears_suit ? new_suit_delay : 0;
    totaldelay += wears_robe ? new_robe_delay : 0;
    totaldelay += wears_cloak ? new_cloak_delay : 0;
    totaldelay += wears_gloves ? old_gloves_delay + new_gloves_delay : 0;
    totaldelay += wears_helmet ? old_helmet_delay + new_helmet_delay : 0;
    totaldelay += wears_bracers ? old_bracers_delay + new_bracers_delay : 0;
    totaldelay += wears_boots ? old_boots_delay + new_boots_delay : 0;
    totaldelay += wears_shield ? old_shield_delay + new_shield_delay : 0;
    totaldelay += wears_amulet ? old_amulet_delay + new_amulet_delay : 0;
    totaldelay += wears_ringl ? old_ringl_delay + new_ringl_delay : 0;
    totaldelay += wears_ringr ? old_ringr_delay + new_ringr_delay : 0;
    totaldelay += wears_misc1 ? old_misc1_delay + new_misc1_delay : 0;

    mon->mfrozen = totaldelay;
    if (mon->mfrozen)
        mon->mcanmove = 0;

}

/* 0 if nothing happened, TRUE if new was worn (and old consequently removed, if any) */
STATIC_OVL boolean
m_dowear_type(mon, flag, creation, racialexception)
struct monst *mon;
int64_t flag;
boolean creation;
boolean racialexception;
{
    struct obj *old, *best, *obj;
    int unseen = !canseemon(mon);
    boolean ispeaceful = is_peaceful(mon);
    boolean autocurse;
    char nambuf[BUFSZ];

    if (mon->mfrozen)
        return 0; /* probably putting previous item on */

    /* Get a copy of monster's name before altering its visibility */
    Strcpy(nambuf, See_invisible ? Monnam(mon) : mon_nam(mon));

    old = which_armor(mon, flag);
    if (old && old->cursed && !cursed_items_are_positive_mon(mon))
        return 0;
    if (old && flag == W_AMUL)
        return 0; /* no such thing as better amulets */
    if (old && flag == W_RINGL)
        return 0; /* no such thing as better rings */
    if (old && flag == W_RINGR)
        return 0; /* no such thing as better rings */
    if (old && flag == W_MISC)
        return 0; /* no such thing as better misc items */
    best = old;

    for (obj = mon->minvent; obj; obj = obj->nobj) 
    {
        if (ispeaceful && (obj->speflags & SPEFLAGS_INTENDED_FOR_SALE) != 0)
            continue;

        switch (flag)
        {
        case W_AMUL:
            if (obj->oclass != AMULET_CLASS
                || (obj->otyp != AMULET_OF_LIFE_SAVING && obj->otyp != AMULET_VERSUS_PETRIFICATION && obj->otyp != AMULET_OF_UNCHANGING
                    && obj->otyp != AMULET_VERSUS_POISON && obj->otyp != AMULET_VERSUS_UNDEATH
                    && obj->otyp != AMULET_OF_REFLECTION))
                continue;
            best = obj;
            goto outer_break; /* no such thing as better amulets */
        case W_RINGR:
        case W_RINGL:
            if (obj->oclass != RING_CLASS || (is_priest(mon->data) && obj->cursed) || is_cursed_magic_item(obj) || (obj->owornmask && obj->owornmask != flag))
                continue;
            best = obj;
            goto outer_break; /* no such thing as better rings */
        case W_MISC:
            if (!can_wear_miscellaneous(mon->data, obj->otyp))
                continue;
            if (obj->oclass != MISCELLANEOUS_CLASS || (is_priest(mon->data) && obj->cursed) || is_cursed_magic_item(obj) || (obj->owornmask && obj->owornmask != flag))
                continue;
            if (objects[obj->otyp].oc_subtyp != MISC_BELT && !likes_magic(mon->data) && !(mon->mnum == PM_MINOTAUR && objects[obj->otyp].oc_subtyp == MISC_NOSERING))
                continue;
            best = obj;
            goto outer_break; /* no such thing as better misc items */
        case W_ARMU:
            if (!is_shirt(obj))
                continue;
            break;
        case W_ARMC:
            if (!is_cloak(obj))
                continue;
            break;
        case W_ARMH:
            if (!is_helmet(obj))
                continue;
            /* changing alignment is not implemented for monsters;
               priests and minions could change alignment but wouldn't
               want to, so they reject helms of opposite alignment */
            if (obj->otyp == HELM_OF_OPPOSITE_ALIGNMENT
                && (mon->ispriest || mon->isminion))
                continue;
            /* (flimsy exception matches polyself handling) */
            if (has_horns(mon->data) && !is_flimsy(obj))
                continue;
            break;
        case W_ARMS:
            if (!is_shield(obj))
                continue;
            break;
        case W_ARMG:
            if (!is_gloves(obj))
                continue;
            break;
        case W_ARMF:
            if (!is_boots(obj))
                continue;
            break;
        case W_ARMB:
            if (!is_bracers(obj))
                continue;
            break;
        case W_ARMO:
            if (!is_robe(obj))
                continue;
            break;
        case W_ARM:
            if (!is_suit(obj))
                continue;
            if (racialexception && (racial_exception(mon, obj) < 1))
                continue;
            break;
        }
        if (obj->owornmask)
            continue;
        /* I'd like to define a VISIBLE_ARM_BONUS which doesn't assume the
         * monster knows obj->enchantment, but if I did that, a monster would keep
         * switching forever between two -2 caps since when it took off one
         * it would forget enchantment and once again think the object is better
         * than what it already has.
         */
        if (best && (ARM_AC_BONUS(best, mon->data) + extra_pref(mon, best)
                     >= ARM_AC_BONUS(obj, mon->data) + extra_pref(mon, obj)))
            continue;
        best = obj;
    }
outer_break:
    if (!best || best == old)
        return 0;

    /* same auto-cursing behavior as for hero */
    autocurse = ((objects[best->otyp].oc_flags & O1_BECOMES_CURSED_WHEN_WORN)  && !best->cursed);

    /* Take old off */
    if (old)
    {/* do this first to avoid "(being worn)" */
        old->owornmask = 0L;
        /* intrinsics are updated below */
        if (mon == u.usteed && old->otyp == SADDLE)
            dismount_steed(DISMOUNT_FELL);
    }

    if (!creation) 
    {
        if (canseemon(mon)) 
        {
            char buf[BUFSZ];

            if (old)
                Sprintf(buf, " removes %s and", distant_name(old, doname));
            else
                buf[0] = '\0';
            pline("%s%s puts on %s.", Monnam(mon), buf,
                  distant_name(best, doname));
            if (autocurse)
                pline_multi_ex(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_NEGATIVE, no_multiattrs, multicolor_buffer, "%s %s %s %s for a moment.", s_suffix(Monnam(mon)),
                      simpleonames(best), otense(best, "glow"),
                      hcolor_multi_buf2(NH_BLACK));
        } /* can see it */
    }

    /* Put new on */
    mon->worn_item_flags |= flag;
    best->owornmask |= flag;
    if (autocurse)
        curse(best);

    /* if couldn't see it but now can, or vice versa, */
    if (!creation && (unseen ^ !canseemon(mon))) {
        if (is_invisible(mon) && !See_invisible) {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly you cannot see %s.", nambuf);
            makeknown(best->otyp);
        } /* else if (!mon->minvis) pline("%s suddenly appears!",
             Amonnam(mon)); */
    }

    return 1;
}
#undef RACE_EXCEPTION


struct obj *
which_armor(mon, flag)
struct monst *mon;
int64_t flag;
{
    if (mon == &youmonst) {
        switch (flag) {
        case W_ARM:
            return uarm;
        case W_ARMC:
            return uarmc;
        case W_ARMO:
            return uarmo;
        case W_ARMH:
            return uarmh;
        case W_ARMS:
            return ((uarms && is_shield(uarms)) ? uarms : (struct obj*)0);
        case W_ARMB:
            return uarmb;
        case W_ARMG:
            return uarmg;
        case W_ARMF:
            return uarmf;
        case W_ARMU:
            return uarmu;
        case W_SADDLE: /* might be used to check if you are polymorphed into a horse wearing a saddle */
            return (struct obj*)0; //usaddle;
        default:
            //impossible("bad flag in which_armor");
            return 0;
        }
    } else {
        register struct obj *obj;

        for (obj = mon->minvent; obj; obj = obj->nobj)
            if (obj->owornmask & flag)
                return obj;
        return (struct obj *) 0;
    }
}


/* remove an item of armor and then drop it */
STATIC_OVL void
m_lose_armor(mon, obj)
struct monst *mon;
struct obj *obj;
{
    mon->worn_item_flags &= ~obj->owornmask;
    if (obj->owornmask)
    {
        obj->owornmask = 0L;
        update_all_mon_statistics(mon, FALSE);
        if (mon == u.usteed && obj->otyp == SADDLE)
            dismount_steed(DISMOUNT_FELL);

    }

    Strcpy(debug_buf_2, "m_lose_armor");
    obj_extract_self(obj);
    place_object(obj, mon->mx, mon->my);
    /* call stackobj() if we ever drop anything that can merge */
    newsym(mon->mx, mon->my);
}


/* all objects with their bypass bit set should now be reset to normal */
void
clear_bypasses()
{
    struct obj *otmp, *nobj;
    struct monst *mtmp;

    /*
     * 'Object' bypass is also used for one monster function:
     * polymorph control of long worms.  Activated via setting
     * context.bypasses even if no specific object has been
     * bypassed.
     */

    for (otmp = fobj; otmp; otmp = nobj) {
        nobj = otmp->nobj;
        if (otmp->bypass) {
            otmp->bypass = 0;

            /* bypass will have inhibited any stacking, but since it's
             * used for polymorph handling, the objects here probably
             * have been transformed and won't be stacked in the usual
             * manner afterwards; so don't bother with this.
             * [Changing the fobj chain mid-traversal would also be risky.]
             */
#if 0
            if (objects[otmp->otyp].oc_merge) {
                xchar ox, oy;

                (void) get_obj_location(otmp, &ox, &oy, 0);
                stack_object(otmp);
                newsym(ox, oy);
            }
#endif /*0*/
        }
    }
    for (otmp = invent; otmp; otmp = otmp->nobj)
        otmp->bypass = 0;
    for (otmp = migrating_objs; otmp; otmp = otmp->nobj)
        otmp->bypass = 0;
    for (otmp = magic_objs; otmp; otmp = otmp->nobj)
        otmp->bypass = 0;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
            otmp->bypass = 0;
        /* long worm created by polymorph has mon->mextra->mcorpsenm set
           to PM_LONG_WORM to flag it as not being subject to further
           polymorph (so polymorph zap won't hit monster to transform it
           into a long worm, then hit that worm's tail and transform it
           again on same zap); clearing mcorpsenm reverts worm to normal */
        if (is_long_worm_with_tail(mtmp->data) && has_mcorpsenm(mtmp))
            MCORPSENM(mtmp) = NON_PM;
    }
    for (mtmp = migrating_mons; mtmp; mtmp = mtmp->nmon) {
        for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
            otmp->bypass = 0;
        /* no MCORPSENM(mtmp)==PM_LONG_WORM check here; long worms can't
           be just created by polymorph and migrating at the same time */
    }
    /* billobjs and mydogs chains don't matter here */
    context.bypasses = FALSE;
}


void
bypass_obj(obj)
struct obj *obj;
{
    obj->bypass = 1;
    context.bypasses = TRUE;
}


/* set or clear the bypass bit in a list of objects */
void
bypass_objlist(objchain, on)
struct obj *objchain;
boolean on; /* TRUE => set, FALSE => clear */
{
    if (on && objchain)
        context.bypasses = TRUE;
    while (objchain) {
        objchain->bypass = on ? 1 : 0;
        objchain = objchain->nobj;
    }
}


/* return the first object without its bypass bit set; set that bit
   before returning so that successive calls will find further objects */
struct obj *
nxt_unbypassed_obj(objchain)
struct obj *objchain;
{
    while (objchain) {
        if (!objchain->bypass) {
            bypass_obj(objchain);
            break;
        }
        objchain = objchain->nobj;
    }
    return objchain;
}


/* like nxt_unbypassed_obj() but operates on sortloot_item array rather
   than an object linked list; the array contains obj==Null terminator;
   there's an added complication that the array may have stale pointers
   for deleted objects (see Multiple-Drop case in askchain(invent.c)) */
struct obj *
nxt_unbypassed_loot(lootarray, listhead)
Loot *lootarray;
struct obj *listhead;
{
    struct obj *o, *obj;

    while ((obj = lootarray->obj) != 0) {
        for (o = listhead; o; o = o->nobj)
            if (o == obj)
                break;
        if (o && !obj->bypass) {
            bypass_obj(obj);
            break;
        }
        ++lootarray;
    }
    return obj;
}


void
mon_break_armor(mon, polyspot)
struct monst *mon;
boolean polyspot;
{
    register struct obj *otmp;
    struct permonst *mdat = mon->data;
    boolean vis = cansee(mon->mx, mon->my);
    boolean handless_or_tiny = (nohands(mdat) || verysmall(mdat));
    const char *pronoun = mhim(mon), *ppronoun = mhis(mon);

    if (breakarm(mdat)) {
        if ((otmp = which_armor(mon, W_ARM)) != 0) {
            if ((is_dragon_scales(otmp) && mdat == Dragon_scales_to_pm(otmp))
                || (is_dragon_mail(otmp) && mdat == Dragon_mail_to_pm(otmp)))
                ; /* no message here;
                     "the dragon merges with his scaly armor" is odd
                     and the monster's previous form is already gone */
            else if (vis)
                pline("%s breaks out of %s armor!", Monnam(mon), ppronoun);
            else
                You_hear("a cracking sound.");
            m_useup(mon, otmp);
        }
        if ((otmp = which_armor(mon, W_ARMC)) != 0) {
            if (otmp->oartifact) {
                if (vis)
                    pline("%s %s falls off!", s_suffix(Monnam(mon)),
                          cloak_simple_name(otmp));
                if (polyspot)
                    bypass_obj(otmp);
                m_lose_armor(mon, otmp);
            } else {
                if (vis)
                    pline("%s %s tears apart!", s_suffix(Monnam(mon)),
                          cloak_simple_name(otmp));
                else
                    You_hear("a ripping sound.");
                m_useup(mon, otmp);
            }
        }
        if ((otmp = which_armor(mon, W_ARMU)) != 0) {
            if (vis)
                pline("%s shirt rips to shreds!", s_suffix(Monnam(mon)));
            else
                You_hear("a ripping sound.");
            m_useup(mon, otmp);
        }
    } else if (sliparm(mdat)) {
        if ((otmp = which_armor(mon, W_ARM)) != 0) {
            if (vis)
                pline("%s armor falls around %s!", s_suffix(Monnam(mon)),
                      pronoun);
            else
                You_hear("a thud.");
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
        if ((otmp = which_armor(mon, W_ARMC)) != 0) {
            if (vis) {
                if (is_whirly(mon->data))
                    pline("%s %s falls, unsupported!", s_suffix(Monnam(mon)),
                          cloak_simple_name(otmp));
                else
                    pline("%s shrinks out of %s %s!", Monnam(mon), ppronoun,
                          cloak_simple_name(otmp));
            }
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
        if ((otmp = which_armor(mon, W_ARMU)) != 0) {
            if (vis) {
                if (sliparm(mon->data))
                    pline("%s seeps right through %s shirt!", Monnam(mon),
                          ppronoun);
                else
                    pline("%s becomes much too small for %s shirt!",
                          Monnam(mon), ppronoun);
            }
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
    }
    if (handless_or_tiny) {
        /* [caller needs to handle weapon checks] */
        if ((otmp = which_armor(mon, W_ARMG)) != 0) {
            if (vis)
                pline("%s drops %s gloves%s!", Monnam(mon), ppronoun,
                      MON_WEP(mon) ? " and weapon" : "");
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
        if ((otmp = which_armor(mon, W_ARMS)) != 0) {
            if (vis)
                pline("%s can no longer hold %s shield!", Monnam(mon),
                      ppronoun);
            else
                You_hear("a clank.");
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
    }
    if (handless_or_tiny || has_horns(mdat)) {
        if ((otmp = which_armor(mon, W_ARMH)) != 0
            /* flimsy test for horns matches polyself handling */
            && (handless_or_tiny || !is_flimsy(otmp))) {
            if (vis)
                pline("%s helmet falls to the %s!", s_suffix(Monnam(mon)),
                      surface(mon->mx, mon->my));
            else
                You_hear("a clank.");
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
    }
    if (handless_or_tiny || slithy(mdat) || mdat->mlet == S_CENTAUR) {
        if ((otmp = which_armor(mon, W_ARMF)) != 0) {
            if (vis) {
                if (is_whirly(mon->data))
                    pline("%s boots fall away!", s_suffix(Monnam(mon)));
                else
                    pline("%s boots %s off %s feet!", s_suffix(Monnam(mon)),
                          verysmall(mdat) ? "slide" : "are pushed", ppronoun);
            }
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
        }
    }
    if (!can_saddle(mon)) {
        if ((otmp = which_armor(mon, W_SADDLE)) != 0) {
            if (polyspot)
                bypass_obj(otmp);
            m_lose_armor(mon, otmp);
            if (vis)
                pline("%s saddle falls off.", s_suffix(Monnam(mon)));
        }
        if (mon == u.usteed)
            goto noride;
    } else if (mon == u.usteed && !can_ride(mon)) {
    noride:
        You("can no longer ride %s.", mon_nam(mon));
        if (touch_petrifies(u.usteed->data) && !Stone_resistance && rnl(3)) {
            char buf[BUFSZ];

            You("touch %s.", mon_nam(u.usteed));
            Sprintf(buf, "falling off %s", an(mon_monster_name(u.usteed)));
            killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
            instapetrify(buf);
        }
        dismount_steed(DISMOUNT_FELL);
    }
    return;
}


/* bias a monster's preferences towards armor that has special benefits. */
STATIC_OVL int
extra_pref(mon, obj)
struct monst *mon;
struct obj *obj;
{
    /* currently only does speed boots, but might be expanded if monsters
     * get to use more armor abilities
     */
    if (obj) 
    {
        if ((objects[obj->otyp].oc_oprop == LIGHTNING_FAST || objects[obj->otyp].oc_oprop2 == LIGHTNING_FAST || objects[obj->otyp].oc_oprop3 == LIGHTNING_FAST)
            && !(mon->mprops[LIGHTNING_FAST] & (M_EXTRINSIC | M_INTRINSIC_ACQUIRED)))
            return 50;
        if ((objects[obj->otyp].oc_oprop == SUPER_FAST || objects[obj->otyp].oc_oprop2 == SUPER_FAST || objects[obj->otyp].oc_oprop3 == SUPER_FAST)
            && !(mon->mprops[SUPER_FAST] & (M_EXTRINSIC | M_INTRINSIC_ACQUIRED)))
            return 40;
        if ((objects[obj->otyp].oc_oprop == ULTRA_FAST || objects[obj->otyp].oc_oprop2 == ULTRA_FAST || objects[obj->otyp].oc_oprop3 == ULTRA_FAST)
            && !(mon->mprops[ULTRA_FAST] & (M_EXTRINSIC | M_INTRINSIC_ACQUIRED)))
            return 30;
        if ((objects[obj->otyp].oc_oprop == VERY_FAST || objects[obj->otyp].oc_oprop2 == VERY_FAST || objects[obj->otyp].oc_oprop3 == VERY_FAST)
            && !(mon->mprops[VERY_FAST] & (M_EXTRINSIC | M_INTRINSIC_ACQUIRED)))
            return 20;
    }
    return 0;
}


/*
 * Exceptions to things based on race.
 * Correctly checks polymorphed player race.
 * Returns:
 *       0 No exception, normal rules apply.
 *       1 If the race/object combination is acceptable.
 *      -1 If the race/object combination is unacceptable.
 */
int
racial_exception(mon, obj)
struct monst *mon;
struct obj *obj;
{
    const struct permonst *ptr = raceptr(mon);

    /* Acceptable Exceptions: */
    /* Allow hobbits to wear elven armor - LoTR */
    if (ptr == &mons[PM_HALFLING] && is_elven_armor(obj))
        return 1;
    /* Unacceptable Exceptions: */
    /* Checks for object that certain races should never use go here */
    /*  return -1; */

    return 0;
}

int
count_unworn_items(struct obj* inv)
{
    if (!inv)
        return 0;

    int cnt = 0;
    for (struct obj* otmp = inv; otmp; otmp = otmp->nobj)
    {
        if (otmp->owornmask == 0UL)
        {
            cnt++;
        }
    }
    return cnt;
}
/*worn.c*/

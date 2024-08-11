/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    wield.c    $NHDT-Date: 1543492132 2018/11/29 11:48:52 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.58 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2009. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/* KMH -- Differences between the three weapon slots.
 *
 * The main weapon (uwep):
 * 1.  Is filled by the (w)ield command.
 * 2.  Can be filled with any type of item.
 * 3.  May be carried in one or both hands.
 * 4.  Is used as the melee weapon and as the launcher for
 *     ammunition.
 * 5.  Only conveys intrinsics when it is a weapon, weapon-tool,
 *     or artifact.
 * 6.  Certain cursed items will weld to the hand and cannot be
 *     unwielded or dropped.  See erodeable_wep() and will_weld()
 *     below for the list of which items apply.
 *
 * The secondary weapon (uswapwep):
 * 1.  Is filled by the e(x)change command, which swaps this slot
 *     with the main weapon.  If the "pushweapon" option is set,
 *     the (w)ield command will also store the old weapon in the
 *     secondary slot.
 * 2.  Can be filled with anything that will fit in the main weapon
 *     slot; that is, any type of item.
 * 3.  Is usually NOT considered to be carried in the hands.
 *     That would force too many checks among the main weapon,
 *     second weapon, shield, gloves, and rings; and it would
 *     further be complicated by bimanual weapons.  A special
 *     exception is made for two-weapon combat.
 * 4.  Is used as the second weapon for two-weapon combat, and as
 *     a convenience to swap with the main weapon.
 * 5.  Never conveys intrinsics.
 * 6.  Cursed items never weld (see #3 for reasons), but they also
 *     prevent two-weapon combat.
 *
 * The quiver (uquiver):
 * 1.  Is filled by the (Q)uiver command.
 * 2.  Can be filled with any type of item.
 * 3.  Is considered to be carried in a special part of the pack.
 * 4.  Is used as the item to throw with the (f)ire command.
 *     This is a convenience over the normal (t)hrow command.
 * 5.  Never conveys intrinsics.
 * 6.  Cursed items never weld; their effect is handled by the normal
 *     throwing code.
 * 7.  The autoquiver option will fill it with something deemed
 *     suitable if (f)ire is used when it's empty.
 *
 * No item may be in more than one of these slots.
 */

STATIC_DCL boolean FDECL(cant_wield_corpse, (struct obj *));
STATIC_DCL int FDECL(ready_weapon, (struct obj *, int64_t));

/* used by will_weld() */
/* probably should be renamed */
#define erodeable_wep(optr)                             \
    ((optr)->oclass == WEAPON_CLASS || is_weptool(optr) \
     || (optr)->otyp == HEAVY_IRON_BALL || (optr)->otyp == IRON_CHAIN)

/* used by welded(), and also while wielding */
#define will_weld(otmp, mtmp) \
    ((otmp)->cursed && (erodeable_wep(otmp) || (otmp)->otyp == TIN_OPENER) && !cursed_items_are_positive_mon(mtmp))

/*** Functions that place a given item in a slot ***/
/* Proper usage includes:
 * 1.  Initializing the slot during character generation or a
 *     restore.
 * 2.  Setting the slot due to a player's actions.
 * 3.  If one of the objects in the slot are split off, these
 *     functions can be used to put the remainder back in the slot.
 * 4.  Putting an item that was thrown and returned back into the slot.
 * 5.  Emptying the slot, by passing a null object.  NEVER pass
 *     zeroobj!
 *
 * If the item is being moved from another slot, it is the caller's
 * responsibility to handle that.  It's also the caller's responsibility
 * to print the appropriate messages.
 */

void
setuwep(obj, mask)
register struct obj* obj;
int64_t mask;
{
    setuwepcore(obj, mask, TRUE);
}

void
setuwepquietly(obj, mask)
register struct obj* obj;
int64_t mask;
{
    setuwepcore(obj, mask, FALSE);
}

void
setuwepcore(obj, mask, verbose)
register struct obj *obj;
int64_t mask;
boolean verbose;
{
    struct obj* olduwep = (struct obj*)0;
    if (mask == W_WEP)
    {
        olduwep = uwep;
    }
    else
    {
        olduwep = uarms;
    }
    if (obj == olduwep)
        return; /* necessary to not set unweapon */

    /* This message isn't printed in the caller because it happens
     * *whenever* Sunsword is unwielded, from whatever cause.
     */
    setworncore(obj, mask, verbose);

    boolean invneedsupdate = FALSE;
    if (obj && (objects[obj->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED))
    {
        invneedsupdate = verbose && obj->known != 1;
        obj->known = 1;
    }

    if (uwep == obj && olduwep && (artifact_light(olduwep) || has_obj_mythic_magical_light(olduwep) || obj_shines_magical_light(olduwep)) && olduwep->lamplit) {
        end_burn(olduwep, FALSE);
        if (!Blind && verbose)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s shining.", Tobjnam(olduwep, "stop"));
    }
    
    context.botl = context.botlx = 1;
    
    /* Note: Explicitly wielding a pick-axe will not give a "bashing"
     * message.  Wielding one via 'a'pplying it will.
     * 3.2.2:  Wielding arbitrary objects will give bashing message too.
     */
    update_unweapon();

    if (invneedsupdate)
        update_inventory();
}

void
update_unweapon(VOID_ARGS)
{
    update_hand_unweapon(1);
    update_hand_unweapon(2);
}

void
update_hand_unweapon(hand)
int hand;
{
    struct obj* wep = (hand <= 1 ? uwep : uarms);
    boolean* unweapon_ptr = (hand <= 1 ? &unweapon1 : &unweapon2);

     if (hand >= 2 && !u.twoweap)
    {
        *unweapon_ptr = FALSE;
        return;
    }
        
    if (wep)
    {
        *unweapon_ptr = is_unweapon(wep);
    }
    else
    {
        *unweapon_ptr = ((uarmg && is_weapon(uarmg) && (P_SKILL_LEVEL(P_BARE_HANDED_COMBAT) >= P_SKILLED)) || (P_SKILL_LEVEL(P_MARTIAL_ARTS) >= P_BASIC) || (P_SKILL_LEVEL(P_BARE_HANDED_COMBAT) >= P_EXPERT)) ? FALSE : TRUE; /* for "bare hands" message */
    }
}

STATIC_OVL boolean
cant_wield_corpse(obj)
struct obj *obj;
{
    char kbuf[BUFSZ];

    if (uarmg || obj->otyp != CORPSE || obj->corpsenm < LOW_PM || !touch_petrifies(&mons[obj->corpsenm])
        || Stone_resistance)
        return FALSE;

    /* Prevent wielding cockatrice when not wearing gloves --KAA */
    You("wield %s in your bare %s.",
        corpse_xname(obj, (const char *) 0, CXN_PFX_THE),
        makeplural(body_part(HAND)));
    Sprintf(kbuf, "wielding %s bare-handed", killer_xname(obj));
    killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
    instapetrify(kbuf);
    return TRUE;
}

STATIC_OVL int
ready_weapon(wep, mask)
struct obj *wep;
int64_t mask;
{
    /* Separated function so swapping works easily */
    int res = 0;
    struct obj* wepinhand = (struct obj*)0;
    struct obj* wepinotherhand = (struct obj*)0;
    if (mask == W_WEP)
    {
        wepinhand = uwep;
        wepinotherhand = uarms;
    }
    else if (mask == W_WEP2)
    {
        wepinhand = uarms;
        wepinotherhand = uwep;
    }

    if (!wep)
    {
        /* No weapon */
        if (wepinhand)
        {
            if(u.twoweap)
                Your("%s %s is now empty.", mask == W_WEP2 ? "left" : "right", body_part(HAND));
            else
                You("are now empty %s.", body_part(HANDED));
            setuwep((struct obj *) 0, mask);
            res++;
        } 
        else
        {
            if (u.twoweap)
                Your("%s %s is already empty.", mask == W_WEP2 ? "left" : "right", body_part(HAND));
            else
                You("are already empty %s.", body_part(HANDED));
        }
    } 
    else if (wep->otyp == CORPSE && cant_wield_corpse(wep)) 
    {
        /* hero must have been life-saved to get here; use a turn */
        res++; /* corpse won't be wielded */
    } 
    else if (wepinotherhand && bimanual(wep)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wield a two-handed %s while %s.",
            is_sword(wep) ? "sword" : wep->otyp == BATTLE_AXE ? "axe"
                                                              : "weapon", is_shield(wepinotherhand) ? "wearing a shield" : "wielding a weapon in the other hand");
    } 
    else if (!retouch_object(&wep, FALSE))
    {
        res++; /* takes a turn even though it doesn't get wielded */
    } 
    else 
    {
        /* Weapon WILL be wielded after this point */
        res++;
        if (will_weld(wep, &youmonst)) 
        {
            const char *tmp = xname(wep), *thestr = "The ";

            if (strncmp(tmp, thestr, 4) && !strncmp(The(tmp), thestr, 4))
                tmp = thestr;
            else
                tmp = "";

            play_sfx_sound(SFX_ITEM_WELDS);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s%s %s to your %s!", tmp, aobjnam(wep, "weld"),
                  (wep->quan == 1L) ? "itself" : "themselves", /* a3 */
                  bimanual(wep) ? (const char *) makeplural(body_part(HAND))
                                : body_part(HAND));
            wep->bknown = TRUE;
        } 
        else 
        {
            /* The message must be printed before setuwep (since
             * you might die and be revived from changing weapons),
             * and the message must be before the death message and
             * Lifesaved rewielding.  Yet we want the message to
             * say "weapon in hand", thus this kludge.
             * [That comment is obsolete.  It dates from the days (3.0)
             * when unwielding Firebrand could cause hero to be burned
             * to death in Hell due to loss of fire resistance.
             * "Lifesaved re-wielding or re-wearing" is ancient history.]
             */

            //Kludge removed by Janne Gustafsson; items may now be identified by setuwep
            /*
            int64_t dummy = wep->owornmask;

            wep->owornmask |= W_WEP;
            if (is_obj_tethered_weapon(wep, wep->owornmask))
                You("secure the tether.");
            wep->owornmask = dummy;
            */
        }
        setuwep(wep, mask);
        prinv((char*)0, wep, 0L);

        /* KMH -- Talking artifacts are finally implemented */
        arti_speak(wep);

        if (wep && (artifact_light(wep) || has_obj_mythic_magical_light(wep) || (obj_shines_magical_light(wep) && !inappropriate_monster_character_type(&youmonst, wep))) && !wep->lamplit)
        {
            begin_burn(wep, FALSE);
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s to shine %s!", Tobjnam(wep, "begin"),
                      arti_light_description(wep));
        }

        if (wep && wep->unpaid)
        {
            struct monst *this_shkp;

            if ((this_shkp = shop_keeper(inside_shop(u.ux, u.uy)))
                != (struct monst *) 0) {
                pline("%s says \"You be careful with my %s!\"",
                      shkname(this_shkp), xname(wep));
            }
        }
    }
    return res;
}

void
setuqwep(obj)
register struct obj *obj;
{
    setworncore(obj, W_QUIVER, TRUE);
    context.botl = context.botlx = TRUE;
    status_reassess();
    /* no extra handling needed; this used to include a call to
       update_inventory() but that's already performed by setworn() */
    return;
}

void
setuswapwep(obj, mask)
register struct obj *obj;
int64_t mask;
{
    setworncore(obj, mask, TRUE);
    return;
}

void
setuqwepquietly(obj)
register struct obj* obj;
{
    setworncore(obj, W_QUIVER, FALSE);
    /* no extra handling needed; this used to include a call to
       update_inventory() but that's already performed by setworn() */
    return;
}

void
setuswapwepquietly(obj, mask)
register struct obj* obj;
int64_t mask;
{
    setworncore(obj, mask, FALSE);
    return;
}


/*** Commands to change particular slot(s) ***/

STATIC_VAR NEARDATA const char wield_objs[] = {
    ALL_CLASSES, ALLOW_NONE, WEAPON_CLASS, TOOL_CLASS, 0
};
STATIC_VAR NEARDATA const char ready_objs[] = {
    ALLOW_COUNT, COIN_CLASS, ALL_CLASSES, ALLOW_NONE, WEAPON_CLASS, 0
};
STATIC_VAR NEARDATA const char bullets[] = { /* (note: different from dothrow.c) */
    ALLOW_COUNT, COIN_CLASS, ALL_CLASSES, ALLOW_NONE,
    GEM_CLASS, WEAPON_CLASS, 0
};

STATIC_VAR NEARDATA const char unwield_objs[] = { ALL_CLASSES, 0 };

int
dowield()
{
    struct obj* wep;

    /* May we attempt this? */
    multi = 0;
    if (cantwield(youmonst.data))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Don't be ridiculous!");
        return 0;
    }

    /* Prompt for a new weapon */
    if (!(wep = getobj(wield_objs, "wield", 0, "")))
        /* Cancelled */
        return 0;

    return wield_weapon(wep);
}

int
dowieldprevwep()
{
    multi = 0;
    if (cantwield(youmonst.data))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Don't be ridiculous!");
        return 0;
    }

    if (uwep && (uwep->speflags & SPEFLAGS_NO_PREVIOUS_WEAPON))
    {
        if (welded(uwep, &youmonst))
        {
            weldmsg(uwep);
            /* previously interrupted armor removal mustn't be resumed */
            reset_remarm();
            return 0;
        }
        struct obj* otmp = uwep;
        int result = ready_weapon((struct obj*)0, W_WEP);
        boolean unwield_succeeded = uwep == (struct obj*)0;
        if (unwield_succeeded)
        {
            otmp->speflags &= ~SPEFLAGS_NO_PREVIOUS_WEAPON;
            play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_UNWIELD);
            update_all_character_properties((struct obj*)0, TRUE);
            status_reassess();
        }
        return result;
    }
    else
    {
        struct obj* wep = 0;
        struct obj* otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj)
        {
            if (otmp->speflags & SPEFLAGS_PREVIOUSLY_WIELDED)
            {
                wep = otmp;
                break;
            }
        }

        if (wep)
            return wield_weapon(wep);
        else
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "couldn't locate your previous weapon!");
            return 0;
        }
    }
}

int
wield_weapon(wep)
struct obj* wep;
{
    struct obj* oldwep;
    int result;

    if (u.twoweap)
    {
        if (wep == uwep || wep == uarms)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are already wielding that!");
            return 0;
        }

        int64_t mask = 0;
        char qbuf[BUFSZ] = "";
        if (wep == &zeroobj)
        {
            if (uwep && uarms)
            {
                char answer = '\0';
                do {
                    Sprintf(qbuf, "Which %s, Left or Right?",
                        body_part(HAND));
                    answer = yn_function(qbuf, "lr", '\0', "Left\nRight");
                    switch (answer) {
                    case '\0':
                        return 0;
                    case 'l':
                    case 'L':
                        mask = W_WEP2;
                        break;
                    case 'r':
                    case 'R':
                        mask = W_WEP;
                        break;
                    }
                } while (!mask);

            }
            else if (uwep)
                mask = W_WEP;
            else if (uarms)
                mask = W_WEP2;

        }
        else if (bimanual(wep)) 
        {
            mask = W_WEP;
        }
        else if (uwep && !uarms) 
        {
            if (bimanual(uwep))
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wield another weapon while wielding a two-handed weapon.");
                return 0;
            }
            mask = W_WEP2;
        }
        else if (uarms && !uwep)
        {
            if (bimanual(uarms))
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wield another weapon while wielding a two-handed weapon.");
                return 0;
            }
            mask = W_WEP;
        }
        else 
        {
            char answer = '\0';
            do 
            {
                Sprintf(qbuf, "Which %s, Left or Right?",
                    body_part(HAND));
                answer = yn_function(qbuf, "lr", '\0', "Left\nRight");
                switch (answer) 
                {
                case '\0':
                    return 0;
                case 'l':
                case 'L':
                    mask = W_WEP2;
                    break;
                case 'r':
                case 'R':
                    mask = W_WEP;
                    break;
                }
            } while (!mask);
        }
        if ((mask == W_WEP || bimanual(wep)) && uwep && welded(uwep, &youmonst)) 
        {
            weldmsg(uwep);
            /* previously interrupted armor removal mustn't be resumed */
            reset_remarm();
            return 0;
        }
        if ((mask == W_WEP2 || bimanual(wep)) && uarms && welded(uarms, &youmonst))
        {
            weldmsg(uarms);
            /* previously interrupted armor removal mustn't be resumed */
            reset_remarm();
            return 0;
        }

        /* Handle no object, or object in other slot */
        if (wep == &zeroobj)
            wep = (struct obj*) 0;
        else if (bimanual(wep) && (wep == uswapwep || wep == uswapwep2))
            return doswapweapon();
        else if (wep == uswapwep)
            return dosingleswapweapon(W_SWAPWEP, mask);
        else if (wep == uswapwep2)
            return dosingleswapweapon(W_SWAPWEP2, mask);
        else if (wep == uquiver)
            setuqwep((struct obj*) 0);
        else if (wep->owornmask & (W_ARMOR | W_ACCESSORY | W_SADDLE)) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wield that!");
            return 0;
        }

        /* Set your new primary weapon */
        boolean ready_succeeded = FALSE;
        if(mask == W_WEP)
        {
            oldwep = uwep;
            result = ready_weapon(wep, W_WEP);
            ready_succeeded = (uwep == wep);
            if (flags.pushweapon && oldwep && uwep != oldwep)
                setuswapwep(oldwep, W_SWAPWEP);
        }
        else
        {
            oldwep = uarms;
            result = ready_weapon(wep, W_WEP2);
            ready_succeeded = (uarms == wep);
            if (flags.pushweapon && oldwep && uarms != oldwep)
                setuswapwep(oldwep, W_SWAPWEP2);
        }

        if (ready_succeeded)
        {
            if (wep)
                play_simple_object_sound(wep, OBJECT_SOUND_TYPE_WIELD);
            else if (oldwep)
                play_simple_object_sound(oldwep, OBJECT_SOUND_TYPE_UNWIELD);
        }
    }
    else
    {
        if (wep == uwep) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are already wielding that!");
            return 0;
        } 
        else if (welded(uwep, &youmonst))
        {
            weldmsg(uwep);
            /* previously interrupted armor removal mustn't be resumed */
            reset_remarm();
            return 0;
        }

        /* Handle no object, or object in other slot */
        if (wep == &zeroobj)
            wep = (struct obj *) 0;
        else if (bimanual(wep) && (wep == uswapwep || wep == uswapwep2))
            return doswapweapon();
        else if (wep == uswapwep)
            return dosingleswapweapon(W_SWAPWEP, W_WEP);
        else if (wep == uswapwep2)
            return dosingleswapweapon(W_SWAPWEP2, W_WEP);
        else if (wep == uquiver)
            setuqwep((struct obj *) 0);
        else if (wep->owornmask & (W_ARMOR | W_ACCESSORY | W_SADDLE)) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot wield that!");
            return 0;
        }

        if (wep && bimanual(wep) && uarms && !is_shield(uarms) && !welded(uarms, &youmonst))
        {
            /* Automate the unwield to assist the player */
            /* If weapon is bimanual and you have an unused secondardy weapon in hand, unwield it */
            remove_worn_item(uarms, FALSE);
        }

        /* Set your new primary weapon */
        oldwep = uwep;
        result = ready_weapon(wep, W_WEP);
        boolean ready_succeeded = (uwep == wep);
        if (flags.pushweapon && oldwep && uwep != oldwep)
            setuswapwep(oldwep, W_SWAPWEP);

        if (ready_succeeded)
        {
            if (wep)
                play_simple_object_sound(wep, OBJECT_SOUND_TYPE_WIELD);
            else if (oldwep)
                play_simple_object_sound(oldwep, OBJECT_SOUND_TYPE_UNWIELD);
        }
    }

    update_all_character_properties((struct obj*)0, TRUE);
    status_reassess();

    return result;
}

/* the unwield command */
int
dounwield()
{
    struct obj* otmp = (struct obj*)0;

    if (!uwep && !uwep2) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Not wielding anything.");
        return 0;
    }

    otmp = getobj(unwield_objs, "unwield", 0, "");
    if (!otmp || !(otmp->owornmask & W_WEAPON))
        return 0;

    int64_t mask = 0L;
    
    if (otmp == uwep)
        mask = W_WEP;
    else if (otmp == uwep2)
        mask = W_WEP2;
    else if (otmp == uswapwep)
        mask = W_SWAPWEP;
    else if (otmp == uswapwep2)
        mask = W_SWAPWEP2;
    else if (otmp == uquiver)
        mask = W_QUIVER;
    else
        return 0;

    int result = 0;
    if (mask == W_WEP2 && otmp == uarms && is_shield(otmp))
    {
        result = armor_or_accessory_off(otmp);
    }
    else if (mask == W_SWAPWEP)
    {
        uswapwepgone();
        result = 1;
    }
    else if (mask == W_SWAPWEP2)
    {
        uswapwep2gone();
        result = 1;
    }
    else if (mask == W_QUIVER)
    {
        uqwepgone();
        result = 1;
    }
    else
    {
        if (welded(otmp, &youmonst))
        {
            weldmsg(otmp);
            /* previously interrupted armor removal mustn't be resumed */
            reset_remarm();
            return 0;
        }
        result = ready_weapon((struct obj*)0, mask);
    }

    boolean unwield_succeeded = 
        mask == W_WEP ? (uwep == (struct obj*)0) : 
        mask == W_WEP2 ? (uwep2 == (struct obj*)0) :
        mask == W_SWAPWEP ? (uswapwep == (struct obj*)0) :
        mask == W_SWAPWEP2 ? (uswapwep2 == (struct obj*)0) :
        mask == W_QUIVER ? (uquiver == (struct obj*)0) :
        FALSE;

    if (unwield_succeeded) //Note: shield unwearing may take longer
    {
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_UNWIELD);
        update_all_character_properties((struct obj*)0, TRUE);
        status_reassess();
    }

    return result;
}



int
dosingleswapweapon(swap_wep_mask, swap_target_mask)
int64_t swap_wep_mask, swap_target_mask; // swap_wep_mask = mask of original weapon in swapwep, swap_target_mask is the mask it is going to swapped to
{
    register struct obj *oldwep, * oldswap;
    register struct obj *wep = (struct obj*)0, *altwep = (struct obj*)0, *swapwep = (struct obj*)0, *altswapwep = (struct obj*)0;
    int result = 0;

    if (swap_wep_mask == W_SWAPWEP)
    {
        swapwep = uswapwep;
        altswapwep = uswapwep2;
        if (swap_target_mask == W_WEP)
        {
            wep = uwep;
            altwep = uarms;
        }
        else
        {
            wep = uarms;
            altwep = uwep;
        }
    }
    else
    {
        swapwep = uswapwep2;
        altswapwep = uswapwep;
        if (swap_target_mask == W_WEP2)
        {
            wep = uarms;
            altwep = uwep;
        }
        else
        {
            wep = uwep;
            altwep = uarms;
        }
    }

    /* May we attempt this? */
    multi = 0;
    if (cantwield(youmonst.data)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Don't be ridiculous!");
        return 0;
    }

    if (wep && wep->cursed && swapwep)
    {
        weldmsg(wep);
        return 0;
    }

    if (wep && bimanual(wep) && altswapwep)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot ready a two-handed weapon while having something already readied for the other %s.", body_part(HAND));
        return 0;
    }

    if (wep && altwep && swapwep && bimanual(swapwep))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot swap to a two-handed weapon while holding something in the other %s.", body_part(HAND));
        return 0;
    }

    if (!wep && !swapwep)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s is already empty.", swap_target_mask == W_WEP ? "right" : "left", body_part(HAND));
        return 0;
    }

    //play_ui_sound(UI_SOUND_WEAPON_SWAPPED);

    /* Unwield your current secondary weapon */
    oldwep = wep;
    oldswap = swapwep;

    setuswapwep((struct obj*)0, swap_wep_mask);
    /* Set your new secondary weapon */
    result = ready_weapon(oldswap, swap_target_mask);

    /* Handle swap */
    struct obj* curwep = (swap_target_mask == W_WEP ? uwep : uarms);
    if (curwep == oldwep)
    {
        /* Wield failed for some reason */
        setuswapwep(oldswap, swap_wep_mask);
    }
    else
    {
        if (curwep == oldswap)
        {
            /* Wield succeeded */
            play_simple_object_sound(oldswap, OBJECT_SOUND_TYPE_WIELD);
        }

        setuswapwep(oldwep, swap_wep_mask);
        struct obj* curswapwep = (swap_wep_mask == W_SWAPWEP ? uswapwep : uswapwep2);
        struct obj* curswapwep2 = (swap_wep_mask == W_SWAPWEP ? uswapwep2 : uswapwep);
        if (curswapwep)
            prinv((char*)0, curswapwep, 0L);
        else if (!(curswapwep2 && bimanual(curswapwep2)))
            You("have no %s %s alternate weapon readied.", swap_wep_mask == W_SWAPWEP ? "right" : "left", body_part(HAND));
    }

    if (result)
    {
        /* Do nothing */
    }

    status_reassess();

    //Do not take a turn
    return 0; // result;

}

int
doswaphandedness()
{
    flags.swap_rhand_only = !flags.swap_rhand_only;

    if (flags.swap_rhand_only)
    {
        play_ui_sound(UI_SOUND_WEAPON_SWAPPING_IN_RIGHT_HAND_ONLY);
        You("are now swapping weapons only in your right %s.", body_part(HAND));
    }
    else
    {
        play_ui_sound(UI_SOUND_WEAPON_SWAPPING_IN_BOTH_HANDS);
        You("are now swapping weapons in your both %s.", makeplural(body_part(HAND)));
    }

    return 0;
}


int
doswapweapon_right_or_both()
{
    if (flags.swap_rhand_only)
        return dosingleswapweapon(W_SWAPWEP, W_WEP);
    else
        return doswapweapon();
}


int
doswapweapon()
{
    register struct obj *oldwep, *oldswap;
    register struct obj* oldwep2, * oldswap2;
    int result = 0;
    int result2 = 0;

    /* May we attempt this? */
    multi = 0;
    if (cantwield(youmonst.data))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Don't be ridiculous!");
        return 0;
    }
    if (u.twoweap)
    {
        if (uwep && welded(uwep, &youmonst)) 
        {
            weldmsg(uwep);
            return 0;
        }
        if (uarms && welded(uarms, &youmonst)) 
        {
            weldmsg(uarms);
            return 0;
        }
        if (!uwep && !uswapwep && !uarms && !uswapwep2)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            Your_ex(ATR_NONE, CLR_MSG_FAIL, "both %s are already empty.", makeplural(body_part(HAND)));
            return 0;
        }

        //play_ui_sound(UI_SOUND_WEAPON_SWAPPED);

        /* Unwield your current secondary weapon */
        oldwep = uwep;
        oldswap = uswapwep;
        oldwep2 = uarms;
        oldswap2 = uswapwep2;
        setuswapwep((struct obj*) 0, W_SWAPWEP);
        setuswapwep((struct obj*) 0, W_SWAPWEP2);

        /* Unwield your current weapons */
        if(oldwep)
            setuwep((struct obj*) 0, W_WEP);
        if(oldwep2)
            setuwep((struct obj*) 0, W_WEP2);

        /* Set your new primary weapon */
        if (oldswap)
        {
            result = ready_weapon(oldswap, W_WEP);
            if (uwep == oldswap)
            {
                play_simple_object_sound(oldswap, OBJECT_SOUND_TYPE_WIELD);
            }
        }
        else if (oldwep)
        {
            play_simple_object_sound(oldwep, OBJECT_SOUND_TYPE_UNWIELD);
        }

        if (oldswap2)
        {
            result2 = ready_weapon(oldswap2, W_WEP2);
            if (uarms == oldswap2)
            {
                play_simple_object_sound(oldswap2, OBJECT_SOUND_TYPE_WIELD);
            }
        }
        else if (oldwep2)
        {
            play_simple_object_sound(oldwep2, OBJECT_SOUND_TYPE_UNWIELD);
        }


        /* Set your new secondary weapon */
        if (uwep == oldwep)
        {
            /* Wield failed for some reason */
            if(oldswap)
                setuswapwep(oldswap, W_SWAPWEP);
        }
        else
        {
            if(oldwep)
            {
                setuswapwep(oldwep, W_SWAPWEP);
                if (uswapwep)
                    prinv((char*)0, uswapwep, 0L);
                else if (!(uswapwep2 && bimanual(uswapwep2)))
                    You("have no right hand alternate weapon readied.");
            }
        }

        if (uarms == oldwep2) 
        {
            /* Wield failed for some reason */
            setuswapwep(oldswap2, W_SWAPWEP2);
        }
        else
        {
            if (oldwep2)
            {
                setuswapwep(oldwep2, W_SWAPWEP2);
                if (uswapwep2)
                    prinv((char*)0, uswapwep2, 0L);
                else if (!(uswapwep && bimanual(uswapwep)))
                    You("have no left hand alternate weapon readied.");
            }
        }
    }
    else
    {
        if (welded(uwep, &youmonst))
        {
            weldmsg(uwep);
            return 0;
        }
        if (uarms && uarms->cursed && !cursed_items_are_positive(youmonst.data) /*&& (uswapwep2 || (uswapwep && bimanual(uswapwep))) */ )
        {
            weldmsg(uarms);
            if (!flags.swap_rhand_only && yn_query("Do you want to swap weapons only in your right hand?") == 'y')
            {
                (void)doswaphandedness();
                return doswapweapon_right_or_both();
            }
            return 0;
        }
        if (!uwep && !uswapwep && !uarms && !uswapwep2)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are already empty %s.", body_part(HANDED));
            return 0;
        }

        //play_ui_sound(UI_SOUND_WEAPON_SWAPPED);

        /* Unwield your current secondary weapon */
        oldwep = uwep;
        oldwep2 = uarms;
        oldswap = uswapwep;
        oldswap2 = uswapwep2;
        setuswapwep((struct obj*)0, W_SWAPWEP);
        setuswapwep((struct obj*)0, W_SWAPWEP2);

        /* Unwield your current weapons / shields */
        if(oldwep)
            setuwep((struct obj*) 0, W_WEP);
    
        if (oldwep2)
        {
            if (is_shield(oldwep2))
                remove_worn_item(oldwep2, FALSE);
            else
                setuwep((struct obj*) 0, W_WEP2);
        }

        /* Set your new primary weapon */
        result = ready_weapon(oldswap, W_WEP);

        /* Set your new secondary weapon */
        if (uwep == oldwep)
        {
            /* Wield failed for some reason */
            setuswapwep(oldswap, W_SWAPWEP);
        } 
        else 
        {
            if (uwep == oldswap)
            {
                /* Wield succeeded */
                if (oldswap)
                {
                    play_simple_object_sound(oldswap, OBJECT_SOUND_TYPE_WIELD);
                }
                else if (oldwep)
                {
                    play_simple_object_sound(oldwep, OBJECT_SOUND_TYPE_UNWIELD);
                }
            }

            setuswapwep(oldwep, W_SWAPWEP);
            if (uswapwep)
                prinv((char *) 0, uswapwep, 0L);
            else if (!(uswapwep2 && bimanual(uswapwep2)))
                You("have no alternate weapon readied.");

            if(oldwep2 && !oldswap2)
            {
                setuswapwep(oldwep2, W_SWAPWEP2);
                if (uswapwep2)
                    prinv((char*)0, uswapwep2, 0L);
                else if (!(uswapwep && bimanual(uswapwep)))
                    You("have no alternate shield readied.");

            }
        }

        if (oldswap2 && !(uwep && (bimanual(uwep) || bimanual(oldswap2))))
        {
            if (is_shield(oldswap2))
            {
                setworn(oldswap2, W_ARMS);
                if (uarms)
                    prinv((char*)0, uarms, 0L);
            }
            else if (erodeable_wep(oldswap2))
                ready_weapon(oldswap2, W_WEP2);

            if (uarms == oldwep2)
            {
                /* Wield failed for some reason */
                setuswapwep(oldswap2, W_SWAPWEP2);
            }
            else 
            {
                if (uarms == oldswap2)
                {
                    /* Wield succeeded */
                    if (oldswap2)
                    {
                        play_simple_object_sound(oldswap2, OBJECT_SOUND_TYPE_WIELD);
                    }
                    else if (oldwep2)
                    {
                        play_simple_object_sound(oldwep2, OBJECT_SOUND_TYPE_UNWIELD);
                    }
                }

                setuswapwep(oldwep2, W_SWAPWEP2);
                if (uswapwep2)
                    prinv((char*)0, uswapwep2, 0L);
                else if(!(uswapwep && bimanual(uswapwep)))
                    You("have no alternate shield readied.");
            }
        }
    }

    if (result || result2)
    {
        /* Do nothing */
    }

    status_reassess();

    //Do not take a turn
    return 0; // result;
}

int
dowieldquiver()
{
    char qbuf[QBUFSZ];
    struct obj *newquiver;
    const char *quivee_types;
    int res;
    boolean finish_splitting = FALSE,
            was_uwep = FALSE, was_uarms = FALSE, was_twoweap = u.twoweap;

    /* Since the quiver isn't in your hands, don't check cantwield(), */
    /* will_weld(), touch_petrifies(), etc. */
    multi = 0;
    /* forget last splitobj() before calling getobj() with ALLOW_COUNT */
    context.objsplit.child_oid = context.objsplit.parent_oid = 0;

    /* Prompt for a new quiver: "What do you want to ready?"
       (Include gems/stones as likely candidates if either primary
       or secondary weapon is a sling.) */
    quivee_types = (uslinging()
                    || (uswapwep
                        && objects[uswapwep->otyp].oc_skill == P_SLING))
                   ? bullets
                   : ready_objs;
    newquiver = getobj(quivee_types, "ready", 0, "");

    if (!newquiver) 
    {
        /* Cancelled */
        return 0;
    } 
    else if (newquiver == &zeroobj) 
    { /* no object */
        /* Explicitly nothing */
        if (uquiver) 
        {
            play_simple_object_sound(uquiver, OBJECT_SOUND_TYPE_UNQUIVER);
            You("now have no ammunition readied.");
            /* skip 'quivering: prinv()' */
            setuqwep((struct obj *) 0);
        } 
        else 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "already have no ammunition readied!");
        }
        return 0;
    } 
    else if (newquiver->o_id == context.objsplit.child_oid) 
    {
        /* if newquiver is the result of supplying a count to getobj()
           we don't want to split something already in the quiver;
           for any other item, we need to give it its own inventory slot */
        if (uquiver && uquiver->o_id == context.objsplit.parent_oid)
        {
            unsplitobj(newquiver);
            goto already_quivered;
        }
        finish_splitting = TRUE;
    } 
    else if (newquiver == uquiver) 
    {
    already_quivered:
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "That ammunition is already readied!");
        return 0;
    }
    else if (newquiver->owornmask & (W_ARMOR | W_ACCESSORY | W_SADDLE)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot ready that!");
        return 0;
    }
    else if (newquiver == uwep && uwep) 
    {
        int weld_res = !uwep->bknown;

        if (welded(uwep, &youmonst))
        {
            weldmsg(uwep);
            reset_remarm(); /* same as dowield() */
            return weld_res;
        }

        /* offer to split stack if wielding more than 1 */
        if (uwep->quan > 1L && inv_cnt(FALSE) < 52 && splittable(uwep))
        {
            Sprintf(qbuf, "You are wielding %lld %s.  Ready %lld of them?",
                (long long)uwep->quan, simpleonames(uwep), (long long)uwep->quan - 1);
            switch (ynq(qbuf)) 
            {
            case 'q':
                return 0;
            case 'y':
                /* leave 1 wielded, split rest off and put into quiver */
                newquiver = splitobj(uwep, uwep->quan - 1L);
                finish_splitting = TRUE;
                goto quivering;
            default:
                break;
            }
            Strcpy(qbuf, "Ready all of them instead?");
        }
        else
        {
            boolean use_plural = (is_plural(uwep) || pair_of(uwep));

            Sprintf(qbuf, "You are wielding %s.  Ready %s instead?",
                    !use_plural ? "that" : "those",
                    !use_plural ? "it" : "them");
        }

        /* require confirmation to ready the main weapon */
        if (yn_query(qbuf) != 'y')
        {
            (void) Shk_Your(qbuf, uwep); /* replace qbuf[] contents */
            pline("%s%s %s wielded.", qbuf,
                  simpleonames(uwep), otense(uwep, "remain"));
            return 0;
        }
        /* quivering main weapon, so no longer wielding it */
        setuwep((struct obj *) 0, W_WEP);
        //untwoweapon();
        was_uwep = TRUE;
    }
    else if (newquiver == uarms && uarms) 
    {
        int weld_res = !uarms->bknown;

        if (welded(uarms, &youmonst)) 
        {
            weldmsg(uarms);
            reset_remarm(); /* same as dowield() */
            return weld_res;
        }
        
        /* offer to split stack if wielding more than 1 */
        if (uarms->quan > 1L && inv_cnt(FALSE) < 52 && splittable(uarms))
        {
            Sprintf(qbuf, "You are wielding %lld %s.  Ready %lld of them?",
                (long long)uarms->quan, simpleonames(uarms), (long long)uarms->quan - 1);
            switch (ynq(qbuf))
            {
            case 'q':
                return 0;
            case 'y':
                /* leave 1 wielded, split rest off and put into quiver */
                newquiver = splitobj(uarms, uarms->quan - 1L);
                finish_splitting = TRUE;
                goto quivering;
            default:
                break;
            }
            Strcpy(qbuf, "Ready all of them instead?");
        }
        else 
        {
            boolean use_plural = (is_plural(uarms) || pair_of(uarms));

            Sprintf(qbuf, "You are wielding %s.  Ready %s instead?",
                !use_plural ? "that" : "those",
                !use_plural ? "it" : "them");
        }

        /* require confirmation to ready the main weapon */
        if (yn_query(qbuf) != 'y') 
        {
            (void)Shk_Your(qbuf, uarms); /* replace qbuf[] contents */
            pline("%s%s %s wielded.", qbuf,
                simpleonames(uarms), otense(uarms, "remain"));
            return 0;
        }
        /* quivering main weapon, so no longer wielding it */
        setuwep((struct obj*) 0, W_WEP2);
        //untwoweapon();
        was_uarms = TRUE;
    }
    else if (newquiver == uswapwep && uswapwep)
    {
        if (uswapwep->quan > 1L && inv_cnt(FALSE) < 52
            && splittable(uswapwep)) 
        {
            Sprintf(qbuf, "%s %lld %s.  Ready %lld of them?",
                    "Your alternate weapon is",
                    (long long)uswapwep->quan, simpleonames(uswapwep),
                    (long long)uswapwep->quan - 1);
            switch (ynq(qbuf)) 
            {
            case 'q':
                return 0;
            case 'y':
                /* leave 1 alt-wielded, split rest off and put into quiver */
                newquiver = splitobj(uswapwep, uswapwep->quan - 1L);
                finish_splitting = TRUE;
                goto quivering;
            default:
                break;
            }
            Strcpy(qbuf, "Ready all of them instead?");
        }
        else
        {
            boolean use_plural = (is_plural(uswapwep) || pair_of(uswapwep));

            Sprintf(qbuf, "%s your %s weapon.  Ready %s instead?",
                    !use_plural ? "That is" : "Those are",
                    "alternate",
                    !use_plural ? "it" : "them");
        }

        /* require confirmation to ready the alternate weapon */
        if (yn_query(qbuf) != 'y') 
        {
            (void) Shk_Your(qbuf, uswapwep); /* replace qbuf[] contents */
            pline("%s%s %s %s.", qbuf,
                  simpleonames(uswapwep), otense(uswapwep, "remain"),
                 "as alternate weapon");
            return 0;
        }
        /* quivering alternate weapon, so no more uswapwep */
        setuswapwep((struct obj *) 0, W_SWAPWEP);
        //untwoweapon();
    } 
    else if (newquiver == uswapwep2 && uswapwep2)
    {
        if (uswapwep2->quan > 1L && inv_cnt(FALSE) < 52
            && splittable(uswapwep2)) 
        {
            Sprintf(qbuf, "%s %lld %s.  Ready %lld of them?",
                "Your alternate weapon is",
                (long long)uswapwep2->quan, simpleonames(uswapwep2),
                (long long)uswapwep2->quan - 1);
            switch (ynq(qbuf))
            {
            case 'q':
                return 0;
            case 'y':
                /* leave 1 alt-wielded, split rest off and put into quiver */
                newquiver = splitobj(uswapwep2, uswapwep2->quan - 1L);
                finish_splitting = TRUE;
                goto quivering;
            default:
                break;
            }
            Strcpy(qbuf, "Ready all of them instead?");
        }
        else 
        {
            boolean use_plural = (is_plural(uswapwep2) || pair_of(uswapwep2));

            Sprintf(qbuf, "%s your %s weapon.  Ready %s instead?",
                !use_plural ? "That is" : "Those are",
                "alternate",
                !use_plural ? "it" : "them");
        }

        /* require confirmation to ready the alternate weapon */
        if (yn_query(qbuf) != 'y') 
        {
            (void)Shk_Your(qbuf, uswapwep2); /* replace qbuf[] contents */
            pline("%s%s %s %s.", qbuf,
                simpleonames(uswapwep2), otense(uswapwep2, "remain"),
                "as alternate weapon");
            return 0;
        }
        /* quivering alternate weapon, so no more uswapwep */
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
        //untwoweapon();
    }

 quivering:
    if (finish_splitting) 
    {
        freeinv(newquiver);
        newquiver->nomerge = 1;
        addinv(newquiver);
        newquiver->nomerge = 0;
    }
    /* place item in quiver before printing so that inventory feedback
       includes "(at the ready)" */
    play_simple_object_sound(newquiver, OBJECT_SOUND_TYPE_QUIVER);
    setuqwep(newquiver);
    prinv((char *) 0, newquiver, 0L);

    /* quiver is a convenience slot and manipulating it ordinarily
       consumes no time, but unwielding primary or secondary weapon
       should take time (perhaps we're adjacent to a rust monster
       or disenchanter and want to hit it immediately, but not with
       something we're wielding that's vulnerable to its damage) */
    res = 0;
    if (was_uwep)
    {
        Your("right %s is now empty.", body_part(HAND));
        res = 1;
    }
    else if (was_uarms)
    {
        Your("left %s is now empty.", body_part(HAND));
        res = 1;
    }
    else if (was_twoweap && !u.twoweap) {
        You("are no longer wielding two weapons at once.");
        res = 1;
    }
    return res;
}

/* used for #rub and for applying pick-axe, whip, grappling hook or polearm */
boolean
wield_tool(obj, verb)
struct obj *obj;
const char *verb; /* "rub",&c */
{
    if (!obj)
        return FALSE;

    const char *what;
    boolean more_than_1;

    if (obj == uwep || (u.twoweap && obj == uarms))
        return TRUE; /* nothing to do if already wielding it */

    if (!verb)
        verb = "wield";
    what = xname(obj);
    more_than_1 = (obj->quan > 1L || strstri(what, "pair of ") != 0
                   || strstri(what, "s of ") != 0);

    if (obj->owornmask & W_WORN_NOT_WIELDED)
    {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s while wearing %s.", verb, yname(obj),
                 more_than_1 ? "them" : "it");
        return FALSE;
    }

    boolean selected_hand_is_right = TRUE;
    
    if (u.twoweap && (obj == uarms || obj == uswapwep2))
        selected_hand_is_right = FALSE;
    else if (!uwep)
        selected_hand_is_right = TRUE;
    else if (bimanual(uwep))
        selected_hand_is_right = TRUE;
    else if (u.twoweap)
    {
        if (uarms && welded(uarms, &youmonst))
            selected_hand_is_right = TRUE;
        else if (!uarms)
            selected_hand_is_right = FALSE;
        else if (uwep && welded(uwep, &youmonst))
            selected_hand_is_right = FALSE;
    }

    struct obj* wep = selected_hand_is_right ? uwep : uarms;

    if (wep && welded(wep, &youmonst))
    {
        if (flags.verbose)
        {
            const char *hand = body_part(HAND);

            if (bimanual(uwep))
                hand = makeplural(hand);
            if (strstri(what, "pair of ") != 0)
                more_than_1 = FALSE;
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL,
               "Since your weapon is welded to your %s, you cannot %s %s %s.",
                  hand, verb, more_than_1 ? "those" : "that", xname(obj));
        }
        else 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "do that.");
        }
        return FALSE;
    }

    if (cantwield(youmonst.data))
    {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "hold %s strongly enough.", more_than_1 ? "them" : "it");
        return FALSE;
    }

    /* check shield */
    if (uarms && bimanual(obj)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot %s a two-handed %s while %s.", verb,
            (obj->oclass == WEAPON_CLASS) ? "weapon" : "tool", 
            is_shield(obj) ? "wearing a shield" : is_weapon(obj) ? "wielding a weapon in the other hand" : "wielding an item in the other hand");
        return FALSE;
    }

    obj->speflags &= ~SPEFLAGS_NO_PREVIOUS_WEAPON;

    if (uquiver == obj)
        setuqwep((struct obj *) 0);
    else if (bimanual(obj) && uswapwep == obj)
    {
        (void)doswapweapon();
        /* doswapweapon might fail */
        if (uswapwep == obj)
            return FALSE;
    }
    else if (uswapwep == obj && uwep && bimanual(uwep))
    {
        (void)doswapweapon();
        /* doswapweapon might fail */
        if (uswapwep == obj)
            return FALSE;
    }
    else if (u.twoweap && uswapwep2 == obj && uwep && bimanual(uwep)) /* two-weaponing is needed for swapping, as otherwise the tool wouldn't be ready for use after the function call */
    {
        (void)doswapweapon();
        /* doswapweapon might fail */
        if (uswapwep2 == obj)
            return FALSE;
    }
    else if (uswapwep == obj)
    {
        (void) dosingleswapweapon(W_SWAPWEP, selected_hand_is_right ? W_WEP : W_WEP2);
        /* doswapweapon might fail */
        if (uswapwep == obj)
            return FALSE;
    }
    else if (u.twoweap && uswapwep2 == obj) /* two-weaponing is needed for swapping, as otherwise the tool wouldn't be ready for use after the function call */
    {
        (void)dosingleswapweapon(W_SWAPWEP2, selected_hand_is_right ? W_WEP : W_WEP2);
        /* doswapweapon might fail */
        if (uswapwep2 == obj)
            return FALSE;
    }
    else
    {
        int64_t wepslot = selected_hand_is_right ? W_WEP : W_WEP2;
        int64_t swapwepslot = selected_hand_is_right ? W_SWAPWEP : W_SWAPWEP2;

        /* if not two-weaponing, unwield first if the obj is uarms or swapweapon2, so that it can be wielded normally */
        if (!u.twoweap && uarms == obj)
            setuwep((struct obj*)0, W_WEP2);
        else if (uswapwep2 == obj)
            setuswapwep((struct obj*)0, W_SWAPWEP2);

        struct obj *oldwep = selected_hand_is_right ? uwep: uarms;
        if(!oldwep)
            obj->speflags |= SPEFLAGS_NO_PREVIOUS_WEAPON;

        if (will_weld(obj, &youmonst))
        {
            /* hope none of ready_weapon()'s early returns apply here... */
            (void) ready_weapon(obj, wepslot);
        } 
        else 
        {
            char handbuf[BUFSZ];
            Strcpy(handbuf, "");

            if(u.twoweap)
                Sprintf(handbuf, " in your %s %s", selected_hand_is_right ? "right" : "left", body_part(HAND));

            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "now wield %s%s.", doname(obj), handbuf);

            setuwep(obj, wepslot);
        }

        /* refresh wep */
        wep = selected_hand_is_right ? uwep : uarms;
        boolean setprevwepflag = FALSE;
        if (oldwep && wep != oldwep)
        {
            if (flags.pushweapon)
                setuswapwep(oldwep, swapwepslot);
            else
            {
                setprevwepflag = TRUE;
            }
        }

        struct obj* otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj)
            otmp->speflags &= ~SPEFLAGS_PREVIOUSLY_WIELDED;

        if(setprevwepflag && oldwep)
            oldwep->speflags |= SPEFLAGS_PREVIOUSLY_WIELDED;

        context.botl = context.botlx = TRUE;
        status_reassess();
    }

    /* refresh wep */
    wep = selected_hand_is_right ? uwep : uarms;

    if (wep != obj)
        return FALSE; /* rewielded old object after dying */

    return TRUE;
}


void
drop_uswapwep()
{
    char str[BUFSZ];
    struct obj *obj = uswapwep;

    /* Avoid trashing makeplural's static buffer */
    Strcpy(str, makeplural(body_part(HAND)));
    pline("%s from your %s!", Yobjnam2(obj, "slip"), str);
    dropxf(obj);
}

int
dotwoweapon()
{
    /* You can always toggle it off */
    if (u.twoweap) 
    {
        play_ui_sound(UI_SOUND_STOP_TWO_WEAPON_COMBAT);
        You("stop two weapon fighting.");
        u.twoweap = 0;
    }
    else
    {
        if (P_SKILL_LEVEL(P_DUAL_WEAPON_COMBAT) == P_ISRESTRICTED)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "do not have the dual wielding skill.");
            return 0;
        }
        play_ui_sound(UI_SOUND_START_TWO_WEAPON_COMBAT);
        You("begin two weapon fighting.");
        u.twoweap = 1;
    }

    update_hand_unweapon(2);
    context.botl = context.botlx = TRUE;
    newsym(u.ux, u.uy);
    flush_screen(1);
    update_inventory();
    status_reassess();
    return 0;
}

/*** Functions to empty a given slot ***/
/* These should be used only when the item can't be put back in
 * the slot by life saving.  Proper usage includes:
 * 1.  The item has been eaten, stolen, burned away, or rotted away.
 * 2.  Making an item disappear for a bones pile.
 */
void
uwepgone()
{
    if (uwep) {
        if ((artifact_light(uwep) || has_obj_mythic_magical_light(uwep) || obj_shines_magical_light(uwep)) && uwep->lamplit) {
            end_burn(uwep, FALSE);
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s shining.", Tobjnam(uwep, "stop"));
        }
        setworn((struct obj *) 0, W_WEP);
        update_unweapon();
        update_inventory();
        //status_reassess();
    }
}

void
uwep2gone()
{
    if (uarms) {
        if ((artifact_light(uarms) || has_obj_mythic_magical_light(uarms) || obj_shines_magical_light(uarms)) && uarms->lamplit) {
            end_burn(uarms, FALSE);
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s shining.", Tobjnam(uarms, "stop"));
        }
        setworn((struct obj*) 0, W_ARMS);
        update_unweapon();
        update_inventory();
        status_reassess();
    }
}

void
uswapwepgone()
{
    if (uswapwep) {
        setworn((struct obj *) 0, W_SWAPWEP);
        update_inventory();
    }
}

void
uswapwep2gone()
{
    if (uswapwep2) {
        setworn((struct obj*) 0, W_SWAPWEP2);
        update_inventory();
    }
}


void
uqwepgone()
{
    if (uquiver) {
        setworn((struct obj *) 0, W_QUIVER);
        update_inventory();
        status_reassess();
    }
}

void
untwoweapon()
{
    if (u.twoweap) {
        play_ui_sound(UI_SOUND_STOP_TWO_WEAPON_COMBAT);
        You("can no longer use two weapons at once.");
        u.twoweap = FALSE;
        update_inventory();
    }
    return;
}

int
enchant_weapon(otmp, weapon, amount, dopopup)
register struct obj *otmp;
register struct obj* weapon;
register int amount;
boolean dopopup;
{
    const char *color = (amount < 0) ? NH_BLACK : NH_BLUE;
    const char *xtime, *wepname = "";
    boolean multiple;
    int otyp = STRANGE_OBJECT;
    char buf[BUFSZ] = "";
    boolean enchwepknown = FALSE;
    if (otmp && (otmp->oclass == SPBOOK_CLASS || objects[otmp->otyp].oc_name_known))
        enchwepknown = TRUE;

    if (!weapon || !((is_weapon(weapon) || is_ammo(weapon)) && objects[weapon->otyp].oc_enchantable)) 
    {
        if (amount >= 0 && weapon && will_weld(weapon, &youmonst))
        { /* cursed tin opener */
            play_sfx_sound(SFX_ENCHANT_ITEM_UNCURSE_AND_OTHER);
            if (!Blind)
            {
                Strcpy(buf, "");
                const char* hclr = hcolor_multi_buf2(NH_AMBER);
                pline_multi_ex_popup(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_POSITIVE, no_multiattrs, multicolor_buffer, "Strange Feeling", dopopup,
                    "%s with %s%s aura.",
                        Yobjnam2(weapon, "glow"), an_prefix(hclr), hclr);
                weapon->bknown = !Hallucination;
            }
            else
            {
                /* cursed tin opener is wielded in right hand */
                Sprintf(buf, "Your right %s tingles.", body_part(HAND));
            }

            uncurse(weapon);
            update_inventory();
        }
        else
        {
            play_sfx_sound(SFX_HANDS_ITCH);
            Sprintf(buf, "Your %s %s.", makeplural(body_part(HAND)),
                    (amount >= 0) ? "twitch" : "itch");
        }

        strange_feeling(otmp, buf, dopopup); /* pline()+docall()+useup() */
        exercise(A_DEX, (boolean) (amount >= 0));
        return 0;
    }

    if (otmp && otmp->oclass == SCROLL_CLASS)
        otyp = otmp->otyp;

    if (weapon->otyp == WORM_TOOTH && amount >= 0) 
    {
        if (otyp != STRANGE_OBJECT)
        {
            makeknown(otyp);
            enchwepknown = TRUE;
        }

        play_sfx_sound(SFX_ENCHANT_ITEM_SPECIAL_SUCCESS);
        multiple = (weapon->quan > 1L);
        /* order: message, transformation, shop handling */
        Sprintf(buf, "%s %s much sharper now.", simpleonames(weapon),
             multiple ? "fuse, and become" : "is");
        pline_ex1_popup(ATR_NONE, CLR_MSG_POSITIVE, buf, enchwepknown ? "Enchant Weapon" : "Sharpening Magic", dopopup);
        weapon->otyp = CRYSKNIFE;
        weapon->material = objects[weapon->otyp].oc_material;
        weapon->oerodeproof = 0;

        if (multiple)
        {
            weapon->quan = 1L;
            weapon->owt = weight(weapon);
        }

        if (weapon->cursed)
            uncurse(weapon);

        /* update shop bill to reflect new higher value */
        if (weapon->unpaid)
            alter_cost(weapon, 0L);

        if (multiple)
            encumber_msg();

        update_inventory();
        return 1;

    } 
    else if (weapon->otyp == CRYSKNIFE && amount < 0) 
    {
        if (otyp != STRANGE_OBJECT && otmp->bknown)
        {
            makeknown(otyp);
            enchwepknown = TRUE;
        }

        multiple = (weapon->quan > 1L);
        play_sfx_sound(SFX_ENCHANT_ITEM_SPECIAL_NEGATIVE);

        /* order matters: message, shop handling, transformation */
        Sprintf(buf, "Your %s %s much duller now.", simpleonames(weapon), multiple ? "fuse, and become" : "is");
        pline_ex1_popup(ATR_NONE, CLR_MSG_NEGATIVE, buf, enchwepknown ? "Enchant Weapon" : "Dulling Magic", dopopup);
        costly_alteration(weapon, COST_DEGRD); /* DECHNT? other? */
        weapon->otyp = WORM_TOOTH;
        weapon->material = objects[weapon->otyp].oc_material;
        weapon->oerodeproof = 0;

        if (multiple) 
        {
            weapon->quan = 1L;
            weapon->owt = weight(weapon);
        }

        if (multiple)
            encumber_msg();

        update_inventory();
        return 1;
    }

    if (has_oname(weapon))
        wepname = ONAME(weapon);

    if (amount < 0 && weapon->oartifact && restrict_name(weapon, wepname)) 
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        if (!Blind)
        {
            pline_multi_ex_popup(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, enchwepknown ? "Enchant Weapon" : "Faint Glow", dopopup,
                "%s %s.", Yobjnam2(weapon, "faintly glow"), hcolor_multi_buf1(color));
        }
        return 1;
    }

    int max_ench = get_obj_max_enchantment(weapon);

    int ench_limit_multiplier = 1;
    if(bimanual(weapon) && !is_launcher(weapon))
        ench_limit_multiplier = 2;

    /* there is a (soft) upper and lower limit to weapon->enchantment */
    if (((weapon->enchantment > max_ench * ench_limit_multiplier && amount >= 0) || (weapon->enchantment < -max_ench * ench_limit_multiplier && amount < 0)) && rn2(3))
    {
        if (((weapon->speflags & SPEFLAGS_GIVEN_OUT_BLUE_SMOKE) == 0 || rn2(3)) && (weapon->material == MAT_ORICHALCUM || obj_resists(weapon, 0, 75)))
        {
            play_special_effect_at(SPECIAL_EFFECT_PUFF_OF_SMOKE, u.ux, u.uy, 0, FALSE);
            play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
            special_effect_wait_until_action(0);
            if (!Blind)
            {
                const char* icolor = hcolor_multi_buf1(color);
                const char* bluecolor = hcolor_multi(NH_BLUE, multicolor_buffer, 3);
                pline_multi_ex_popup(ATR_NONE, CLR_MSG_NEGATIVE, no_multiattrs, multicolor_buffer, Blind ? "Puff of Smoke" : "Puff of Blue Smoke", dopopup,
                    "%s %s for a while, and then suddenly %s out a puff of %s smoke.", 
                    Yobjnam2(weapon, "violently glow"), icolor, otense(weapon, "give"), bluecolor);
            }
            else
            {
                Sprintf(buf, "%s for a while, and then suddenly %s out a puff of smoke.", Yobjnam2(weapon, "violently vibrate"), otense(weapon, "give"));
                pline_ex1_popup(ATR_NONE, CLR_MSG_NEGATIVE, buf, Blind ? "Puff of Smoke" : "Puff of Blue Smoke", dopopup);
                otmp->enchantment = 0;
            }
            otmp->enchantment = 0;
            otmp->speflags |= SPEFLAGS_GIVEN_OUT_BLUE_SMOKE;
            update_inventory();
            special_effect_wait_until_end(0);
        }
        else
        {
            play_sfx_sound(SFX_ENCHANT_ITEM_VIBRATE_AND_DESTROY);
            if (!Blind)
            {
                pline_multi_ex_popup(ATR_NONE, CLR_MSG_NEGATIVE, no_multiattrs, multicolor_buffer, "Evaporation", dopopup,
                    "%s %s for a while and then %s.", Yobjnam2(weapon, "violently glow"), hcolor_multi_buf1(color), otense(weapon, "evaporate"));
            }
            else
            {
                Sprintf(buf, "%s.", Yobjnam2(weapon, "evaporate"));
                pline_ex1_popup(ATR_NONE, CLR_MSG_NEGATIVE, buf, "Evaporation", dopopup);
            }

            useupall(weapon); /* let all of them disappear */
        }
        return 1;
    }

    if (amount == 0)
        play_sfx_sound(SFX_ENCHANT_ITEM_VIOLENT_GLOW);
    else
        play_sfx_sound(amount > 1 ? SFX_ENCHANT_ITEM_BLESSED_SUCCESS : amount < 0 ? SFX_ENCHANT_ITEM_NEGATIVE : SFX_ENCHANT_ITEM_SUCCESS);

    if (!Blind)
    {
        if (otyp != STRANGE_OBJECT && weapon->known && (amount > 0 || (amount < 0 && otmp->bknown)))
        {
            makeknown(otyp);
            enchwepknown = TRUE;
        }

        xtime = (amount * amount == 1) ? "moment" : "while";
        pline_multi_ex_popup(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : amount == 0 ? CLR_MSG_WARNING : amount > 0 ? CLR_MSG_POSITIVE : CLR_MSG_NEGATIVE, no_multiattrs, multicolor_buffer,
            enchwepknown ? "Enchant Weapon" : amount == 0 ? "Violent Glow" : "Magical Glow", dopopup,
            "%s %s for a %s.", Yobjnam2(weapon, amount == 0 ? "violently glow" : "glow"), hcolor_multi_buf1(color), xtime);
    }

    if (amount < 0)
        costly_alteration(weapon, COST_DECHNT);

    weapon->enchantment += amount;

    if (amount > 0) 
    {
        if (weapon->cursed)
            uncurse(weapon);
        /* update shop bill to reflect new higher price */
        if (weapon->unpaid)
            alter_cost(weapon, 0L);
    }


    /*
     * Enchantment, which normally improves a weapon, has an
     * addition adverse reaction on Magicbane whose effects are
     * enchantment dependent.  Give an obscure clue here.
     */
    if (weapon->oartifact && artifact_has_flag(weapon, AF_MAGIC_ABSORBING) && weapon->enchantment >= 0) 
    {
        play_sfx_sound(SFX_HANDS_ITCH);
        Sprintf(buf, "Your right %s %sches!", body_part(HAND),
             (((amount > 1) && (weapon->enchantment > 1)) ? "flin" : "it"));
        pline_ex1_popup(ATR_NONE, CLR_MSG_ATTENTION, buf, ((amount > 1) && (weapon->enchantment > 1)) ? "Flinching Sensation" : "Itching Sensation", dopopup);
    }

    /* an elven magic clue, cookie@keebler */
    /* elven weapons vibrate warningly when enchanted beyond a limit */
    if ((weapon->enchantment > max_ench)
        /*&& (is_elven_weapon(weapon) || weapon->oartifact || !rn2(7)) */ ) /* Vibrates for sure */
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_VIBRATE_WARNING);
        Sprintf(buf, "%s unexpectedly.", Yobjnam2(weapon, "suddenly vibrate"));
        pline_ex1_popup(ATR_NONE, CLR_MSG_WARNING, buf, "Unexpected Vibration", dopopup);
    }

    update_inventory();
    return 1;
}

int
welded(obj, mon)
register struct obj *obj;
register struct monst* mon;
{
    if (obj && mon && (obj == uwep || obj == uarms) && will_weld(obj, mon)) 
    {
        if (!obj->bknown)
        {
            obj->bknown = TRUE;
            if (obj->where == OBJ_INVENT)
                update_inventory();
        }
        return 1;
    }
    return 0;
}

void
weldmsg(obj)
register struct obj *obj;
{
    int64_t savewornmask;

    savewornmask = obj->owornmask;
    play_sfx_sound(SFX_GENERAL_WELDED);
    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s welded to your %s%s!", Yobjnam2(obj, "are"), bimanual(obj) ? "" : savewornmask & W_WEP ? "right " : "left ",
          bimanual(obj) ? (const char *) makeplural(body_part(HAND))
                        : body_part(HAND));
    obj->owornmask = savewornmask;
}

/* test whether monster's wielded weapon is stuck to hand/paw/whatever */
boolean
mwelded(obj, mon)
struct obj *obj;
struct monst* mon;
{
    /* caller is responsible for making sure this is a monster's item */
    if (obj && mon && (obj->owornmask & W_WEP) && will_weld(obj, mon)) /* cursed objects won't weld for demons and undead */
        return TRUE;
    return FALSE;
}

/*wield.c*/

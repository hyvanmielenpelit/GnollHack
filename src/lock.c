/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    lock.c    $NHDT-Date: 1548978605 2019/01/31 23:50:05 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.84 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/* at most one of `door' and `box' should be non-null at any given time */
STATIC_VAR NEARDATA struct xlock_s {
    struct rm *door;
    xchar x, y;
    struct obj *box;
    struct obj* key;
    int picktyp, /* key|pick|card for unlock, sharp vs blunt for #force */
        chance, usedtime;
    boolean magic_key;
} xlock;

/* occupation callbacks */
STATIC_PTR int NDECL(picklock);
STATIC_PTR int NDECL(forcelock);

STATIC_DCL const char *NDECL(lock_action);
STATIC_DCL boolean FDECL(obstructed, (int, int, BOOLEAN_P));
STATIC_DCL void FDECL(chest_shatter_msg, (struct obj *, int, int));

boolean
picking_lock(x, y)
int *x, *y;
{
    if (occupation == picklock) {
        *x = u.ux + u.dx;
        *y = u.uy + u.dy;
        return TRUE;
    } else {
        *x = *y = 0;
        return FALSE;
    }
}

boolean
picking_at(x, y)
int x, y;
{
    return (boolean) (occupation == picklock && xlock.door == &levl[x][y]);
}

/* produce an occupation string appropriate for the current activity */
STATIC_OVL const char *
lock_action()
{
    /* "unlocking"+2 == "locking" */
    static const char *actions[] = {
        "unlocking the door",   /* [0] */
        "unlocking the chest",  /* [1] */
        "unlocking the box",    /* [2] */
        "picking the lock"      /* [3] */
    };

    /* if the target is currently unlocked, we're trying to lock it now */
    if (xlock.door && !(xlock.door->doormask & D_LOCKED))
        return actions[0] + 2; /* "locking the door" */
    else if (xlock.box && !xlock.box->olocked)
        return xlock.box->otyp == CHEST ? actions[1] + 2 : actions[2] + 2;
    /* otherwise we're trying to unlock it */
    else if (xlock.picktyp == LOCK_PICK)
        return actions[3]; /* "picking the lock" */
    else if (xlock.picktyp == CREDIT_CARD)
        return actions[3]; /* same as lock_pick */
    else if (xlock.door)
        return actions[0]; /* "unlocking the door" */
    else if (xlock.box)
        return xlock.box->otyp == CHEST ? actions[1] : actions[2];
    else
        return actions[3];
}

/* try to open/close a lock */
STATIC_PTR int
picklock(VOID_ARGS)
{
    boolean key_found = FALSE;
    struct obj* otmp;
    for(otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp == xlock.key)
        {
            key_found = TRUE;
            break;
        }

    if (!key_found)
        return ((xlock.usedtime = 0)); /* The key has been lost */

    if (xlock.box) 
    {
        if (xlock.box->where != OBJ_FLOOR
            || xlock.box->ox != u.ux || xlock.box->oy != u.uy) 
        {
            return ((xlock.usedtime = 0)); /* you or it moved */
        }
        if (has_box_normal_lock(xlock.box)
            && (xlock.picktyp == SKELETON_KEY || xlock.picktyp == CREDIT_CARD || xlock.picktyp == LOCK_PICK))
        {
            //nothing, normal case
        }
        else if (xlock.box->keyotyp != xlock.key->otyp || xlock.box->special_quality != xlock.key->special_quality)
        {
            play_sfx_sound(SFX_KEY_DOES_NOT_FIT);
            if (xlock.box->keyotyp == NUM_OBJECTS)
                pline("%s is locked but it has no observable lock.", The(cxname(xlock.box)));
            else
                pline("%s does not match the lock on the %s.", The(cxname(xlock.key)), cxname(xlock.box));
            return ((xlock.usedtime = 0));
        }
    } 
    else 
    { /* door */
        if (xlock.door != &(levl[u.ux + u.dx][u.uy + u.dy])) 
        {
            return ((xlock.usedtime = 0)); /* you moved */
        }
        switch ((xlock.door->doormask & D_MASK)) 
        {
        case D_NODOOR:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline("This doorway has no door.");
            return ((xlock.usedtime = 0));
        case D_ISOPEN:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You("cannot lock an open door.");
            return ((xlock.usedtime = 0));
        case D_BROKEN:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline("This door is broken.");
            return ((xlock.usedtime = 0));
        case D_PORTCULLIS:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You("cannot lock a portcullis.");
            return ((xlock.usedtime = 0));
        }

        if (has_door_normal_lock_at_ptr(xlock.door)
            && (xlock.picktyp == SKELETON_KEY || xlock.picktyp == CREDIT_CARD || xlock.picktyp == LOCK_PICK))
        {
            //nothing, normal case
        }
        else if (xlock.door->key_otyp != xlock.key->otyp || xlock.door->special_quality != xlock.key->special_quality)
        {
            play_sfx_sound(SFX_KEY_DOES_NOT_FIT);
            //play_simple_location_sound(xlock.x, xlock.y, LOCATION_SOUND_TYPE_TRY_LOCKED);
            if (xlock.door->key_otyp == NUM_OBJECTS)
                pline("The door is locked but it has no observable lock.");
            else
                pline("%s does not match the lock on the door.", The(cxname(xlock.key)));
            return ((xlock.usedtime = 0));
        }
    }

    if (xlock.usedtime++ >= 50 || (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data))) 
    {
        You("give up your attempt at %s.", lock_action());
        exercise(A_DEX, TRUE); /* even if you don't succeed */
        return ((xlock.usedtime = 0));
    }

    if (rn2(100) >= xlock.chance)
        return 1; /* still busy */

    /* using the Master Key of Thievery finds traps if its bless/curse
       state is adequate (non-cursed for rogues, blessed for others;
       checked when setting up 'xlock') */
    if ((!xlock.door ? (int) xlock.box->otrapped
                     : (xlock.door->doormask & D_TRAPPED) != 0)
        && xlock.magic_key) {
        xlock.chance += 20; /* less effort needed next time */
        /* unfortunately we don't have a 'tknown' flag to record
           "known to be trapped" so declining to disarm and then
           retrying lock manipulation will find it all over again */
        if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Trap Found", "You find a trap!  Do you want to try to disarm it?") == 'y') {
            const char *what;
            boolean alreadyunlocked;

            /* disarming while using magic key always succeeds */
            if (xlock.door) {
                xlock.door->doormask &= ~D_TRAPPED;
                what = "door";
                alreadyunlocked = !(xlock.door->doormask & D_LOCKED);
            } else {
                xlock.box->otrapped = 0;
                what = (xlock.box->otyp == CHEST) ? "chest" : "box";
                alreadyunlocked = !xlock.box->olocked;
            }
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "succeed in disarming the trap.  The %s is still %slocked.",
                what, alreadyunlocked ? "un" : "");
            exercise(A_WIS, TRUE);
        } else {
            You("stop %s.", lock_action());
            exercise(A_WIS, FALSE);
        }
        return ((xlock.usedtime = 0));
    }

    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "succeed in %s.", lock_action());
    if (xlock.door) 
    {
        play_simple_location_sound(xlock.x, xlock.y, xlock.door->doormask & D_LOCKED ? LOCATION_SOUND_TYPE_UNLOCK : LOCATION_SOUND_TYPE_LOCK);
        
        if ((xlock.door->doormask & D_LOCKED) && (xlock.door->doormask & L_USES_UP_KEY) && xlock.key && !is_obj_indestructible(xlock.key) && !xlock.key->oartifact)
        {
            play_sfx_sound(SFX_ITEM_VANISHES);
            if (xlock.key->quan > 1)
                pline("One of %s vanishes.", yname(xlock.key));
            else
                pline("%s.", Yobjnam2(xlock.key, "vanish"));

            useup(xlock.key);
            xlock.key = 0;
        }

        if (xlock.door->doormask & D_TRAPPED)
        {
            b_trapped(get_door_name_at_ptr(xlock.door), FINGER, u.ux + u.dx, u.uy + u.dy);
            xlock.door->doormask &= ~D_TRAPPED;
            if (is_door_destroyed_by_booby_trap_at_ptr(xlock.door))
            {
                xlock.door->doormask &= ~D_MASK;
                xlock.door->doormask |= D_NODOOR;
                unblock_vision_and_hearing_at_point(u.ux + u.dx, u.uy + u.dy);
            }
            if (*in_rooms(u.ux + u.dx, u.uy + u.dy, SHOPBASE))
                add_damage(u.ux + u.dx, u.uy + u.dy, SHOP_DOOR_COST);
            newsym(u.ux + u.dx, u.uy + u.dy);
        }
        else if (xlock.door->doormask & D_LOCKED)
        {
            xlock.door->doormask &= ~D_MASK;
            xlock.door->doormask |= D_CLOSED;
        }
        else
        {
            xlock.door->doormask &= ~D_MASK;
            xlock.door->doormask |= D_LOCKED;
        }
    } 
    else 
    {
        play_simple_container_sound(xlock.box, xlock.box->olocked ? CONTAINER_SOUND_TYPE_UNLOCK : CONTAINER_SOUND_TYPE_LOCK);
        
        if (xlock.box->olocked && (xlock.box->speflags & SPEFLAGS_USES_UP_KEY) && xlock.key && !is_obj_indestructible(xlock.key) && !xlock.key->oartifact)
        {
            play_sfx_sound(SFX_ITEM_VANISHES);
            if (xlock.key->quan > 1)
                pline("One of %s vanishes.", yname(xlock.key));
            else
                pline("%s.", Yobjnam2(xlock.key, "vanish"));

            useup(xlock.key);
            xlock.key = 0;
        }

        xlock.box->olocked = !xlock.box->olocked;
        xlock.box->lknown = 1;
        newsym(xlock.box->ox, xlock.box->oy);
        if (xlock.box->otrapped)
            (void) chest_trap(xlock.box, FINGER, FALSE);
    }
    exercise(A_DEX, TRUE);
    return ((xlock.usedtime = 0));
}

boolean
key_fits_the_box_lock(key, box)
struct obj *key, *box;
{
    if (!key || !box)
        return FALSE;

    if (has_box_normal_lock(box)
        && (key->otyp == SKELETON_KEY || key->otyp == CREDIT_CARD || key->otyp == LOCK_PICK))
    {
        return TRUE;
    }
    else if (box->keyotyp != key->otyp || box->special_quality != key->special_quality)
    {
        return FALSE;
    }
    return TRUE;
}

boolean
key_fits_the_door_lock(key, door)
struct obj* key;
struct rm* door;
{
    if (!key || !door)
        return FALSE;

    if (has_door_normal_lock_at_ptr(door)
        && (key->otyp == SKELETON_KEY || key->otyp == CREDIT_CARD || key->otyp == LOCK_PICK))
    {
        return TRUE;
    }
    else if (door->key_otyp != key->otyp || door->special_quality != key->special_quality)
    {
        return FALSE;
    }
    return TRUE;
}


void
breakchestlock(box, destroyit)
struct obj *box;
boolean destroyit;
{
    int x = box->ox, y = box->oy;
    if (!destroyit) 
    { /* bill for the box but not for its contents */
        struct obj *hide_contents = box->cobj;

        box->cobj = 0;
        costly_alteration(box, COST_BRKLCK);
        box->cobj = hide_contents;
        box->olocked = 0;
        box->obroken = 1;
        box->lknown = 1;
        newsym(box->ox, box->oy);
    }
    else 
    { /* #force has destroyed this box (at <u.ux,u.uy>) */
        struct obj *otmp;
        struct monst *shkp = (*u.ushops && costly_spot(u.ux, u.uy))
                                 ? shop_keeper(*u.ushops)
                                 : 0;
        boolean costly = (boolean) (shkp != 0),
                peaceful_shk = costly && is_peaceful(shkp);
        long loss = 0L;

        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "In fact, you've totally destroyed %s.", the(xname(box)));
        /* Put the contents on ground at the hero's feet. */
        while ((otmp = box->cobj) != 0) 
        {
            obj_extract_self(otmp);
            if (!rn2(3) || otmp->oclass == POTION_CLASS) 
            {
                chest_shatter_msg(otmp, x, y);
                if (costly)
                    loss += stolen_value(otmp, u.ux, u.uy, peaceful_shk, TRUE);
                if (otmp->quan == 1L) 
                {
                    obfree(otmp, (struct obj *) 0);
                    continue;
                }
                /* this works because we're sure to have at least 1 left;
                   otherwise it would fail since otmp is not in inventory */
                useup(otmp);
            }
            if (box->otyp == ICE_BOX && otmp->otyp == CORPSE) 
            {
                otmp->age = monstermoves - otmp->age; /* actual age */
                start_corpse_timeout(otmp);
            }
            place_object(otmp, u.ux, u.uy);
            stackobj(otmp);
        }
        if (costly)
            loss += stolen_value(box, u.ux, u.uy, peaceful_shk, TRUE);
        if (loss)
            You("owe %ld %s for objects destroyed.", loss, currency(loss));
        delobj(box);
    }
}

/* try to force a locked chest */
STATIC_PTR int
forcelock(VOID_ARGS)
{
    if ((xlock.box->ox != u.ux) || (xlock.box->oy != u.uy))
        return ((xlock.usedtime = 0)); /* you or it moved */

    if (xlock.box->keyotyp == NUM_OBJECTS)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline("%s has no observable lock to force.", The(cxname(xlock.box)));
        return ((xlock.usedtime = 0));
    }

    if (xlock.usedtime++ >= 50 || !uwep || (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data))) 
    {
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "give up your attempt to force the lock.");
        if (xlock.usedtime >= 50) /* you made the effort */
            exercise((xlock.picktyp) ? A_DEX : A_STR, TRUE);
        return ((xlock.usedtime = 0));
    }

    if (xlock.picktyp) 
    { /* blade */
        if (rn2(1000 - (int) uwep->enchantment) > (992 - greatest_erosion(uwep) * 10)
            && !uwep->cursed && !obj_resists(uwep, 0, 99))
        {
            /* for a +0 weapon, probability that it survives an unsuccessful
             * attempt to force the lock is (.992)^50 = .67
             */
            play_simple_object_sound(uwep, OBJECT_SOUND_TYPE_BREAK);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%sour %s broke!", (uwep->quan > 1L) ? "One of y" : "Y",
                  xname(uwep));
            useup(uwep);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "give up your attempt to force the lock.");
            exercise(A_DEX, TRUE);
            return ((xlock.usedtime = 0));
        }
    }
    else             /* blunt */
        wake_nearby(); /* due to hammering on the container */

    if (rn2(100) >= xlock.chance)
        return 1; /* still busy */

    if (has_box_normal_lock(xlock.box))
    {
        //nothing, normal case
    }
    else if (xlock.box->keyotyp != xlock.key->otyp || xlock.box->special_quality != xlock.key->special_quality)
    {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        if (xlock.box->keyotyp == MAGIC_KEY)
            You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to force the magic lock on the %s.", cxname(xlock.box));
        else
            You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to force the lock on the %s.", cxname(xlock.box));

        return 0;
    }

    play_simple_container_sound(xlock.box, CONTAINER_SOUND_TYPE_BREAK_LOCK);
    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "succeed in forcing the lock.");
    exercise(xlock.picktyp ? A_DEX : A_STR, TRUE);
    /* breakchestlock() might destroy xlock.box; if so, xlock context will
       be cleared (delobj -> obfree -> maybe_reset_pick); but it might not,
       so explicitly clear that manually */
    breakchestlock(xlock.box, (boolean) (!xlock.picktyp && !rn2(3)));
    reset_pick(); /* lock-picking context is no longer valid */

    return 0;
}

void
reset_pick()
{
    xlock.usedtime = xlock.chance = xlock.picktyp = 0;
    xlock.magic_key = FALSE;
    xlock.door = (struct rm *) 0;
    xlock.box = (struct obj *) 0;
}

/* level change or object deletion; context may no longer be valid */
void
maybe_reset_pick(container)
struct obj *container; /* passed from obfree() */
{
    /*
     * If a specific container, only clear context if it is for that
     * particular container (which is being deleted).  Other stuff on
     * the current dungeon level remains valid.
     * However if 'container' is Null, clear context if not carrying
     * xlock.box (which might be Null if context is for a door).
     * Used for changing levels, where a floor container or a door is
     * being left behind and won't be valid on the new level but a
     * carried container will still be.  There might not be any context,
     * in which case redundantly clearing it is harmless.
     */
    if (container ? (container == xlock.box)
                  : (!xlock.box || !carried(xlock.box)))
        reset_pick();
}

/* for doapply(); if player gives a direction or resumes an interrupted
   previous attempt then it costs hero a move even if nothing ultimately
   happens; when told "can't do that" before being asked for direction
   or player cancels with ESC while giving direction, it doesn't */

/* player is applying a key, lock pick, or credit card */
int
pick_lock(pick)
struct obj *pick;
{
    int picktyp;
    coord cc;

    picktyp = pick->otyp;

    /* check whether we're resuming an interrupted previous attempt */
    if (xlock.usedtime && picktyp == xlock.picktyp) 
    {
        static const char no_longer[] = "Unfortunately, you can no longer %s %s.";

        if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data)) 
        {
            const char *what = (picktyp == LOCK_PICK) ? "pick" : "key";

            if (picktyp == CREDIT_CARD)
                what = "card";
            pline_ex(ATR_NONE, CLR_MSG_FAIL, no_longer, "hold the", what);
            reset_pick();
            return PICKLOCK_LEARNED_SOMETHING;
        }
        else if (u.uswallow || (xlock.box && !can_reach_floor(TRUE))) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, no_longer, "reach the", "lock");
            reset_pick();
            return PICKLOCK_LEARNED_SOMETHING;
        } 
        else 
        {
            const char *action = lock_action();

            You("resume your attempt at %s.", action);
            xlock.magic_key = is_magic_key(&youmonst, pick);
            set_occupation(picklock, action, objects[pick->otyp].oc_soundset, OCCUPATION_PICKING_LOCK, OCCUPATION_SOUND_TYPE_START, 0);
            return PICKLOCK_DID_SOMETHING;
        }
    }

    if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data)) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "hold %s -- you have no hands!", doname(pick));
        return PICKLOCK_DID_NOTHING;
    }
    else if (u.uswallow)
    {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "%sunlock %s.", (picktyp == CREDIT_CARD) ? "" : "lock or ",
                 mon_nam(u.ustuck));
        return PICKLOCK_DID_NOTHING;
    }

    if (picktyp != LOCK_PICK
        && picktyp != CREDIT_CARD
        && !is_otyp_key(picktyp)) 
    {
        impossible("picking lock with object %d?", picktyp);
        return PICKLOCK_DID_NOTHING;
    }

    if (!get_adjacent_loc((char *) 0, "Invalid location!", u.ux, u.uy, &cc))
        return PICKLOCK_DID_NOTHING;

    update_u_action(ACTION_TILE_DOOR_USE);
    u_wait_until_action();

    int res = pick_lock_core(pick, cc.x, cc.y, FALSE);

    update_u_action_revert(ACTION_TILE_NO_ACTION);
    return res;
}

int
pick_lock_core(pick, x, y, is_auto)
struct obj* pick;
int x, y;
boolean is_auto;
{
    int picktyp, /*special_quality,*/ c, ch;
    coord cc;
    struct rm* door;
    struct obj* otmp;
    char qbuf[QBUFSZ];
    char kbuf[BUFSZ];

    if (!pick || !isok(x, y))
        return 0;

    strcpy(kbuf, "");
    if (is_auto)
        Sprintf(kbuf, " with %s", yname(pick));

    ch = 0;
    cc.x = x;
    cc.y = y;
    picktyp = pick->otyp;
    //special_quality = pick->special_quality;

    if (cc.x == u.ux && cc.y == u.uy) 
    { /* pick lock on a container */
        const char* verb;
        char qsfx[QBUFSZ];
        boolean it;
        int count;

        if (u.dz < 0) 
        {
            if (!is_auto)
            {
                play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
                There("isn't any sort of lock up %s.",
                    Levitation ? "here" : "there");
            }
            return PICKLOCK_LEARNED_SOMETHING;
        }
        else if (is_lava(u.ux, u.uy))
        {
            if (!is_auto)
            {
                play_sfx_sound(SFX_GENERAL_NOT_A_GOOD_IDEA);
                pline("Doing that would probably melt %s.", yname(pick));
            }
            return PICKLOCK_LEARNED_SOMETHING;
        }
        else if (is_pool(u.ux, u.uy) && !Underwater) 
        {
            if (!is_auto)
            {
                play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
                pline_The("%s has no lock.", hliquid("water"));
            }
            return PICKLOCK_LEARNED_SOMETHING;
        }

        count = 0;
        c = 'n'; /* in case there are no boxes here */
        for (otmp = level.objects[cc.x][cc.y]; otmp; otmp = otmp->nexthere)
            if (Is_box(otmp))
            {
                ++count;
                if (!can_reach_floor(TRUE)) 
                {
                    if (!is_auto)
                    {
                        play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
                        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "reach %s from up here.", the(xname(otmp)));
                    }
                    return PICKLOCK_LEARNED_SOMETHING;
                }
                it = 0;
                if (otmp->obroken)
                    verb = "fix";
                else if (!otmp->olocked)
                    verb = "lock", it = 1;
                else if (picktyp != LOCK_PICK)
                    verb = "unlock", it = 1;
                else
                    verb = "pick";

                /* "There is <a box> here; <verb> <it|its lock>?" */
                Sprintf(qsfx, " here; %s %s%s?", verb, it ? "it" : "its lock", kbuf);
                (void)safe_qbuf(qbuf, "There is ", qsfx, otmp, doname,
                    ansimpleoname, "a box");
                otmp->lknown = 1;

                c = ynq(qbuf);
                if (c == 'q')
                    return 0;
                if (c == 'n')
                    continue;

                if (otmp->obroken) 
                {
                    You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "fix its broken lock with %s.", doname(pick));
                    return PICKLOCK_LEARNED_SOMETHING;
                }
                else if (picktyp == CREDIT_CARD && !otmp->olocked) 
                {
                    /* credit cards are only good for unlocking */
                    You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "do that with %s.",
                        an(simple_typename(picktyp)));
                    return PICKLOCK_LEARNED_SOMETHING;
                }
                
                switch (picktyp) 
                {
                case CREDIT_CARD:
                    ch = ACURR(A_DEX) + 20 * Role_if(PM_ROGUE);
                    break;
                case LOCK_PICK:
                    ch = 4 * ACURR(A_DEX) + 25 * Role_if(PM_ROGUE);
                    break;
                case SKELETON_KEY:
                    ch = 75 + ACURR(A_DEX);
                    break;
                default:
                    if (is_otyp_key(picktyp))
                        ch = 75 + ACURR(A_DEX);
                    else
                        ch = 0;
                }
                if (otmp->cursed)
                    ch /= 2;

                xlock.box = otmp;
                xlock.door = 0;
                xlock.x = 0;
                xlock.y = 0;
                break;
            }

        if (c != 'y') 
        {
            if (!count)
                There("doesn't seem to be any sort of lock here.");
            return PICKLOCK_LEARNED_SOMETHING; /* decided against all boxes */
        }
    }
    else 
    { /* pick the lock in a door */
        struct monst* mtmp;

        if (u.utrap && u.utraptype == TT_PIT) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "reach over the edge of the pit.");
            return PICKLOCK_LEARNED_SOMETHING;
        }

        door = &levl[cc.x][cc.y];
        mtmp = m_at(cc.x, cc.y);
        if (mtmp && canseemon(mtmp) && M_AP_TYPE(mtmp) != M_AP_FURNITURE
            && M_AP_TYPE(mtmp) != M_AP_OBJECT) 
        {
            if (picktyp == CREDIT_CARD
                && (mtmp->isshk))
            {
                play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_NO_CHECKS_NO_CREDIT_NO_PROBLEM);
                verbalize("No checks, no credit, no problem.");
            }
            else
                pline("I don't think %s would appreciate that.",
                    mon_nam(mtmp));
            return PICKLOCK_LEARNED_SOMETHING;
        }
        else if (mtmp && is_door_mappear(mtmp)) 
        {
            /* "The door actually was a <mimic>!" */
            stumble_onto_mimic(mtmp);
            /* mimic might keep the key (50% chance, 10% for PYEC or MKoT) */
            maybe_absorb_item(mtmp, pick, 50, 10);
            return PICKLOCK_LEARNED_SOMETHING;
        }

        if (!IS_DOOR(door->typ)) 
        {
            if (is_drawbridge_wall(cc.x, cc.y) >= 0)
                You("%s no lock on the drawbridge.", Blind ? "feel" : "see");
            else
                You("%s no door there.", Blind ? "feel" : "see");
            return PICKLOCK_LEARNED_SOMETHING;
        }

        switch ((door->doormask & D_MASK)) 
        {
        case D_NODOOR:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline("This doorway has no door.");
            return PICKLOCK_LEARNED_SOMETHING;
        case D_ISOPEN:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You("cannot lock an open door.");
            return PICKLOCK_LEARNED_SOMETHING;
        case D_BROKEN:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline("This door is broken.");
            return PICKLOCK_LEARNED_SOMETHING;
        case D_PORTCULLIS:
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline("There is no lock on the portcullis.");
            return PICKLOCK_LEARNED_SOMETHING;
        default:
            /* credit cards are only good for unlocking */
            if (picktyp == CREDIT_CARD && !(door->doormask & D_LOCKED)) 
            {
                You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "lock a door with a credit card.");
                return PICKLOCK_LEARNED_SOMETHING;
            }

            Sprintf(qbuf, "%s it%s?",
                (door->doormask & D_LOCKED) ? "Unlock" : "Lock", kbuf);

            c = yn_query(qbuf);

            if (c == 'n')
            {
                return 0;
            }

            switch (picktyp) 
            {
            case CREDIT_CARD:
                ch = 2 * ACURR(A_DEX) + 20 * Role_if(PM_ROGUE);
                break;
            case LOCK_PICK:
                ch = 3 * ACURR(A_DEX) + 30 * Role_if(PM_ROGUE);
                break;
            case SKELETON_KEY:
                ch = 70 + ACURR(A_DEX);
                break;
            default:
                if (is_otyp_key(picktyp))
                    ch = 70 + ACURR(A_DEX);
                else
                    ch = 0;
            }
            xlock.door = door;
            xlock.x = cc.x;
            xlock.y = cc.y;
            xlock.box = 0;
        }
    }
    context.move = 0;
    xlock.chance = ch;
    xlock.picktyp = picktyp;
    xlock.key = pick;
    xlock.magic_key = is_magic_key(&youmonst, pick);
    xlock.usedtime = 0;
    set_occupation(picklock, lock_action(), objects[pick->otyp].oc_soundset, OCCUPATION_PICKING_LOCK, OCCUPATION_SOUND_TYPE_START, 0);
    return PICKLOCK_DID_SOMETHING;
}

#ifdef ANDROID
boolean
can_try_force()
{
    if (u.uswallow) {
        return FALSE;
    }

    if (!uwep /* proper type test */
        || ((uwep->oclass == WEAPON_CLASS || is_weptool(uwep))
            ? (objects[uwep->otyp].oc_skill < P_DAGGER
                || objects[uwep->otyp].oc_skill == P_FLAIL
                || objects[uwep->otyp].oc_skill > P_THRUSTING_WEAPON)
            : uwep->oclass != ROCK_CLASS)) {
        return FALSE;
    }

    return can_reach_floor(TRUE);
}
#endif


/* try to force a chest with your weapon */
int
doforce()
{
    register struct obj *otmp;
    register int c, picktyp;
    char qbuf[QBUFSZ];

    if (u.uswallow) {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "force anything from inside here.");
        return 0;
    }
    if (!uwep /* proper type test */
        || ((uwep->oclass == WEAPON_CLASS || is_weptool(uwep))
               ? (objects[uwep->otyp].oc_skill < P_DAGGER
                  || objects[uwep->otyp].oc_skill == P_FLAIL
                  || objects[uwep->otyp].oc_skill >= P_BOW)
               : uwep->oclass != ROCK_CLASS)) {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "force anything %s weapon.",
                 !uwep ? "when not wielding a"
                       : (uwep->oclass != WEAPON_CLASS && !is_weptool(uwep))
                             ? "without a proper"
                             : "with that");
        return 0;
    }
    if (!can_reach_floor(TRUE)) {
        cant_reach_floor(u.ux, u.uy, FALSE, TRUE);
        return 0;
    }

    picktyp = is_blade(uwep) && !is_pick(uwep);
    if (xlock.usedtime && xlock.box && picktyp == xlock.picktyp) {
        You("resume your attempt to force the lock.");
        set_occupation(forcelock, "forcing the lock", objects[uwep->otyp].oc_soundset, OCCUPATION_FORCING_LOCK, OCCUPATION_SOUND_TYPE_RESUME, 0);
        return 1;
    }

    /* A lock is made only for the honest man, the thief will break it. */
    xlock.box = (struct obj *) 0;
    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
        if (Is_box(otmp)) {
            if (otmp->obroken || !otmp->olocked) {
                /* force doname() to omit known "broken" or "unlocked"
                   prefix so that the message isn't worded redundantly;
                   since we're about to set lknown, there's no need to
                   remember and then reset its current value */
                otmp->lknown = 0;
                There("is %s here, but its lock is already %s.",
                      doname(otmp), otmp->obroken ? "broken" : "unlocked");
                otmp->lknown = 1;
                continue;
            }
            (void) safe_qbuf(qbuf, "There is ", " here; force its lock?",
                             otmp, doname, ansimpleoname, "a box");
            otmp->lknown = 1;

            c = ynq(qbuf);
            if (c == 'q')
                return 0;
            if (c == 'n')
                continue;

            if (picktyp)
                You("force %s into a crack and pry.", yname(uwep));
            else
                You("start bashing it with %s.", yname(uwep));
            xlock.box = otmp;
            xlock.chance = objects[uwep->otyp].oc_wldam * 2;
            xlock.picktyp = picktyp;
            xlock.magic_key = FALSE;
            xlock.usedtime = 0;
            xlock.door = 0;
            xlock.x = 0;
            xlock.y = 0;
            break;
        }

    if (xlock.box)
    {
        set_occupation(forcelock, "forcing the lock", objects[uwep->otyp].oc_soundset, OCCUPATION_FORCING_LOCK, OCCUPATION_SOUND_TYPE_START, 0);
    }
    else
        You("decide not to force the issue.");
    return 1;
}

boolean
stumble_on_door_mimic(x, y)
int x, y;
{
    struct monst *mtmp;

    if ((mtmp = m_at(x, y)) && is_door_mappear(mtmp)
        && !Protection_from_shape_changers) {
        stumble_onto_mimic(mtmp);
        return TRUE;
    }
    return FALSE;
}

/* the 'O' command - try to open a door */
int
doopen()
{
    return !!doopen_indir(0, 0);
}

/* try to open a door in direction u.dx/u.dy */
/* 0 = open fails, 1 = open succeeded, 2 = started picking the lock */
int
doopen_indir(x, y)
int x, y;
{
    coord cc;
    register struct rm *door;
    boolean portcullis;
    int res = 0;

    if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "open anything -- you have no hands!");
        return 0;
    }

    if (u.utrap && u.utraptype == TT_PIT) {
        play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "reach over the edge of the pit.");
        return 0;
    }

    if (x > 0 && y > 0) {
        cc.x = x;
        cc.y = y;
    } else if (!get_adjacent_loc((char *) 0, (char *) 0, u.ux, u.uy, &cc))
        return 0;

    /* open at yourself/up/down */
    if ((cc.x == u.ux) && (cc.y == u.uy))
        return doloot();

    if (stumble_on_door_mimic(cc.x, cc.y))
        return 1;

    /* when choosing a direction is impaired, use a turn
       regardless of whether a door is successfully targetted */
    if (Confusion || Stunned)
        res = 1;

    door = &levl[cc.x][cc.y];
    const char* door_name = (door && (door->doormask & D_PORTCULLIS)) ? "portcullis" : get_door_name_at_ptr(door);
    portcullis = (is_drawbridge_wall(cc.x, cc.y) >= 0);
    if (Blind) {
        int oldglyph = door->hero_memory_layers.glyph;
        schar oldlastseentyp = lastseentyp[cc.x][cc.y];

        feel_location(cc.x, cc.y);
        if (door->hero_memory_layers.glyph != oldglyph
            || lastseentyp[cc.x][cc.y] != oldlastseentyp)
            res = 1; /* learned something */
    }

    if (portcullis || !IS_DOOR(door->typ)) {
        /* closed portcullis or spot that opened bridge would span */
        if (is_db_wall(cc.x, cc.y) || door->typ == DRAWBRIDGE_UP)
        {
            play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
            There("is no obvious way to open the drawbridge.");
        }
        else if (portcullis || door->typ == DRAWBRIDGE_DOWN)
        {
            play_sfx_sound(SFX_GENERAL_ALREADY_DONE);
            pline_The("drawbridge is already open.");
        }
        else if (container_at(cc.x, cc.y, TRUE))
        {
            play_sfx_sound(SFX_GENERAL_SOMETHING_INTERESTING_THERE);
            pline("%s like something lootable over there.",
                Blind ? "Feels" : "Seems");
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
            You("%s no door there.", Blind ? "feel" : "see");
        }
        return res;
    }

    if (!(door->doormask & D_CLOSED)) 
    {
        const char *mesg;
        boolean locked = FALSE;
        char lbuf[BUFSZ];
        strcpy(lbuf, "");
        enum sfx_sound_types sfx = SFX_ILLEGAL;

        switch (door->doormask & D_MASK) 
        {
        case D_BROKEN:
            mesg = " is broken";
            sfx = SFX_GENERAL_CANNOT;
            break;
        case D_NODOOR:
            mesg = "way has no door";
            sfx = SFX_GENERAL_NOTHING_THERE;
            break;
        case D_ISOPEN:
            mesg = " is already open";
            sfx = SFX_GENERAL_ALREADY_DONE;
            break;
        case D_PORTCULLIS:
            mesg = " is already open";
            sfx = SFX_GENERAL_ALREADY_DONE;
            break;
        default:
            mesg = " is locked";
            print_lock_with_buf(lbuf, door->key_otyp, door->special_quality, is_door_normally_without_lock_at(cc.x, cc.y));
            update_u_action(ACTION_TILE_DOOR_USE);
            u_wait_until_action();
            locked = TRUE;
            break;
        }
        if(sfx != SFX_ILLEGAL)
            play_sfx_sound(sfx);

        pline("This %s%s%s.", door_name, mesg, lbuf);
#ifdef ANDROID
        if (locked && flags.autokick) {
            autokick();
        }
#endif
        if (locked)
        {
            boolean unlocked = FALSE;
            play_simple_location_sound(cc.x, cc.y, LOCATION_SOUND_TYPE_TRY_LOCKED);
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            if (flags.autounlock)
            {
                struct obj* carried_key = 0;
                if ((carried_key = carrying_fitting_unlocking_tool_for_door(door)) != 0)
                {
                    if (carried_key)
                    {
                        int pick_res = pick_lock_core(carried_key, cc.x, cc.y, TRUE);
                        if (pick_res == PICKLOCK_DID_SOMETHING)
                            res = 2, unlocked = TRUE;
                        else if(pick_res == PICKLOCK_LEARNED_SOMETHING)
                            res = 1;
                    }
                }
            }

            if(!unlocked && context.click_kick_query) /* Click */
            {
                char ans = 'n';
                char qbuf[BUFSIZ];
                context.click_kick_query = 0;

                Sprintf(qbuf, "Do you want to start kicking this door?");
                ans = yn_query(qbuf);

                if (ans == 'y')
                {
                    door->click_kick_ok = 1;
                }
            }

        }
        return res;
    }

    if (verysmall(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        pline("You're too small to pull the %s open.", door_name);
        return res;
    }

    /* door is known to be CLOSED */
    update_u_action(ACTION_TILE_DOOR_USE);
    u_wait_until_action();
    if (rnl(20) < (ACURRSTR + ACURR(A_DEX) + ACURR(A_CON)) / 3) 
    {
        play_simple_location_sound(cc.x, cc.y, LOCATION_SOUND_TYPE_OPEN);
        pline_The("%s opens.", door_name);

        if (door->doormask & D_TRAPPED) 
        {
            b_trapped(door_name, FINGER, cc.x, cc.y);
            door->doormask &= ~D_TRAPPED;
            if (is_door_destroyed_by_booby_trap_at_ptr(door))
            {
                door->doormask &= ~D_MASK;
                door->doormask |= D_NODOOR;
                if (*in_rooms(cc.x, cc.y, SHOPBASE))
                    add_damage(cc.x, cc.y, SHOP_DOOR_COST);
            }
            else
            {
                door->doormask &= ~D_MASK;
                door->doormask |= D_ISOPEN;
            }
        } 
        else
        {
            door->doormask &= ~D_MASK;
            door->doormask |= D_ISOPEN;
        }
        feel_newsym(cc.x, cc.y); /* the hero knows she opened it */
        unblock_vision_and_hearing_at_point(cc.x, cc.y); /* vision: new see through there */
    }
    else 
    {
        exercise(A_STR, TRUE);
        play_simple_location_sound(cc.x, cc.y, LOCATION_SOUND_TYPE_RESISTS_OPEN);
        pline_The("%s resists!", door_name);
    }
    update_u_action_revert(ACTION_TILE_NO_ACTION);

    return 1;
}

STATIC_OVL boolean
obstructed(x, y, quietly)
register int x, y;
boolean quietly;
{
    register struct monst *mtmp = m_at(x, y);

    if (mtmp && M_AP_TYPE(mtmp) != M_AP_FURNITURE) {
        if (M_AP_TYPE(mtmp) == M_AP_OBJECT)
            goto objhere;
        if (!quietly) {
            if ((mtmp->mx != x) || (mtmp->my != y)) {
                /* worm tail */
                play_sfx_sound(SFX_SOMETHING_IN_WAY);
                pline("%s%s blocks the way!",
                      !canspotmon(mtmp) ? Something : s_suffix(Monnam(mtmp)),
                      !canspotmon(mtmp) ? "" : " tail");
            } else {
                play_sfx_sound(SFX_SOMETHING_IN_WAY);
                pline("%s blocks the way!",
                      !canspotmon(mtmp) ? "Some creature" : Monnam(mtmp));
            }
        }
        if (!canspotmon(mtmp))
            map_invisible(x, y);
        return TRUE;
    }
    if (OBJ_AT(x, y)) {
 objhere:
        if (!quietly)
        {
            play_sfx_sound(SFX_SOMETHING_IN_WAY);
            pline("%s's in the way.", Something);
        }
        return TRUE;
    }
    return FALSE;
}

/* the 'C' command - try to close a door */
int
doclose()
{
    register int x, y;
    register struct rm *door;
    boolean portcullis;
    int res = 0;

    if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "close anything -- you have no hands!");
        return 0;
    }

    if (u.utrap && u.utraptype == TT_PIT) {
        play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "reach over the edge of the pit.");
        return 0;
    }

    if (!getdir((char *) 0))
        return 0;

    x = u.ux + u.dx;
    y = u.uy + u.dy;
    if ((x == u.ux) && (y == u.uy)) {
        play_sfx_sound(SFX_SOMETHING_IN_WAY);
        You("are in the way!");
        return 1;
    }

    update_u_facing(TRUE);

    if (!isok(x, y))
        goto nodoor;

    if (stumble_on_door_mimic(x, y))
        return 1;

    /* when choosing a direction is impaired, use a turn
       regardless of whether a door is successfully targetted */
    if (Confusion || Stunned)
        res = 1;

    door = &levl[x][y];
    portcullis = (is_drawbridge_wall(x, y) >= 0);
    if (Blind) {
        int oldglyph = door->hero_memory_layers.glyph;
        schar oldlastseentyp = lastseentyp[x][y];

        feel_location(x, y);
        if (door->hero_memory_layers.glyph != oldglyph || lastseentyp[x][y] != oldlastseentyp)
            res = 1; /* learned something */
    }

    if (portcullis || !IS_DOOR(door->typ)) {
        /* is_db_wall: closed portcullis */
        if (is_db_wall(x, y) || door->typ == DRAWBRIDGE_UP)
        {
            play_sfx_sound(SFX_GENERAL_ALREADY_DONE);
            pline_The("drawbridge is already closed.");
        }
        else if (portcullis || door->typ == DRAWBRIDGE_DOWN)
        {
            play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
            There("is no obvious way to close the drawbridge.");
        }
        else {
 nodoor:
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
            You("%s no door there.", Blind ? "feel" : "see");
        }
        return res;
    }

    if ((door->doormask & D_MASK) == D_NODOOR)
    {
        play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        pline("This doorway has no door.");
        return res;
    } 
    else if ((door->doormask & D_MASK) == D_PORTCULLIS)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline("This portcullis can be closed only by lifting the drawbridge.");
        return res;
    }
    else if (obstructed(x, y, FALSE)) 
    {
        return res;
    } 
    else if ((door->doormask & D_MASK) == D_BROKEN) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline("This %s is broken.", get_door_name_at_ptr(door));
        return res;
    } 
    else if ((door->doormask & D_MASK) & (D_CLOSED | D_LOCKED))
    {
        play_sfx_sound(SFX_GENERAL_ALREADY_DONE);
        pline("This %s is already closed.", get_door_name_at_ptr(door));
        return res;
    }

    if ((door->doormask & D_MASK) == D_ISOPEN)
    {
        if (verysmall(youmonst.data) && !u.usteed) 
        {
            play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
            pline("You're too small to push the door closed.");
            return res;
        }
        update_u_action(ACTION_TILE_DOOR_USE);
        u_wait_until_action();
        if (u.usteed
            || rn2(25) < (ACURRSTR + ACURR(A_DEX) + ACURR(A_CON)) / 3) {
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_CLOSE);
            pline_The("%s closes.", get_door_name_at_ptr(door));
            door->doormask &= ~D_MASK;
            door->doormask |= D_CLOSED;
            feel_newsym(x, y); /* the hero knows she closed it */
            if(door_blocks_vision_at_ptr(door))
                block_vision_and_hearing_at_point(x, y); /* vision:  no longer see there */
        } 
        else
        {
            exercise(A_STR, TRUE);
            play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_RESISTS_OPEN);
            pline_The("%s resists!", get_door_name_at_ptr(door));
        }
        update_u_action_revert(ACTION_TILE_NO_ACTION);
    }

    return 1;
}

/* box obj was hit with spell or wand effect otmp;
   returns true if something happened */
boolean
boxlock(obj, otmp)
struct obj *obj, *otmp; /* obj *is* a box */
{
    boolean res = 0;

    switch (otmp->otyp) {
    case WAN_LOCKING:
    case SPE_WIZARD_LOCK:
        if (!obj->olocked && (has_box_normal_lock(obj) || (obj->keyotyp == MAGIC_KEY && obj->special_quality == 0)))
        { /* lock it; fix if broken */
            play_sfx_sound_at_location(SFX_WIZARD_LOCK_KLUNK, obj->ox, obj->oy);
            pline("Klunk!");
            obj->olocked = 1;
            obj->obroken = 0;
            obj->keyotyp = MAGIC_KEY;
            obj->special_quality = 0;
            if (Role_if(PM_WIZARD))
                obj->lknown = 1;
            else
                obj->lknown = 0;
            res = 1;
        } /* else already closed and locked */
        newsym(obj->ox, obj->oy);
        break;
    case WAN_OPENING:
    case SPE_KNOCK:
        if (obj->olocked) { /* unlock; couldn't be broken */
            if (has_box_normal_lock(obj) || (obj->keyotyp == MAGIC_KEY && obj->special_quality == 0))
            {
                play_sfx_sound_at_location(SFX_KNOCK_KLICK, obj->ox, obj->oy);
                pline("Klick!");
                obj->olocked = 0;
                res = 1;
                if (Role_if(PM_WIZARD))
                    obj->lknown = 1;
                else
                    obj->lknown = 0;
            }
        } else /* silently fix if broken */
            obj->obroken = 0;
        newsym(obj->ox, obj->oy);
        break;
    case WAN_POLYMORPH:
    case SPE_POLYMORPH:
        /* maybe start unlocking chest, get interrupted, then zap it;
           we must avoid any attempt to resume unlocking it */
        if (xlock.box == obj)
            reset_pick();
        break;
    }
    return res;
}

/* Door/secret door was hit with spell or wand effect otmp;
   returns true if something happened */
boolean
doorlock(otmp, x, y)
struct obj *otmp;
int x, y;
{
    register struct rm *door = &levl[x][y];
    boolean res = TRUE;
    int loudness = 0;
    const char *msg = (const char *) 0;
    const char* doormsg = door->subtyp >= 0 && door->subtyp < MAX_DOOR_SUBTYPES ? door_subtype_definitions[door->subtyp].description : "door";
    const char *dustcloud = "A cloud of dust";
    const char *quickly_dissipates = "quickly dissipates";
    boolean mysterywand = (otmp->oclass == WAND_CLASS && !otmp->dknown);

    if (door->typ == SDOOR) {
        switch (otmp->otyp) {
        case WAN_OPENING:
        case SPE_KNOCK:
        case WAN_STRIKING:
        case SPE_FORCE_BOLT:
        case SPE_FORCE_STRIKE:
            transform_location_type_and_flags(x, y, DOOR, door->subtyp, D_CLOSED | (door->doormask & D_TRAPPED));
            newsym(x, y);
            if (cansee(x, y))
                pline("A door appears in the wall!");
            if (otmp->otyp == WAN_OPENING || otmp->otyp == SPE_KNOCK)
                return TRUE;
            break; /* striking: continue door handling below */
        case WAN_LOCKING:
        case SPE_WIZARD_LOCK:
        default:
            return FALSE;
        }
    }

    switch (otmp->otyp) {
    case WAN_LOCKING:
    case SPE_WIZARD_LOCK:
    {
        if ((door->doormask & D_MASK) & D_PORTCULLIS)
            return FALSE;

        if (Is_really_rogue_level(&u.uz)) {
            boolean vis = cansee(x, y);
            /* Can't have real locking in Rogue, so just hide doorway */
            if (vis)
                pline("%s springs up in the older, more primitive doorway.",
                      dustcloud);
            else
                You_hear("a swoosh.");
            if (obstructed(x, y, mysterywand)) {
                if (vis)
                    pline_The("cloud %s.", quickly_dissipates);
                return FALSE;
            }
            block_vision_and_hearing_at_point(x, y);
            door->typ = SDOOR;
            door->doormask &= ~D_MASK;
            door->doormask |= D_NODOOR;
            if (vis)
            {
                play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, x, y);
                pline_The("doorway vanishes!");

            }
            newsym(x, y);
            return TRUE;
        }
        if (obstructed(x, y, mysterywand))
            return FALSE;

        /* Don't allow doors to close over traps.  This is for pits */
        /* & trap doors, but is it ever OK for anything else? */
        if (t_at(x, y)) {
            /* maketrap() clears doormask, so it should be NODOOR */
            pline("%s springs up in the doorway, but %s.", dustcloud,
                  quickly_dissipates);
            return FALSE;
        }

        boolean can_lock = (has_door_normal_lock_at_ptr(door) || (door->key_otyp == MAGIC_KEY && door->special_quality == 0));

        switch ((door->doormask & ~D_TRAPPED) & D_MASK) 
        {
        case D_CLOSED:
            if(can_lock)
                msg = "The %s locks!";
            else
                msg = "The %s creaks for a while!";
            break;
        case D_ISOPEN:
            if (can_lock)
                msg = "The %s swings shut, and locks!";
            else
                msg = "The %s swings shut!";
            break;
        case D_BROKEN:
            if (can_lock)
                msg = "The broken %s reassembles and locks!";
            else
                msg = "The broken %s reassembles!";
            break;
        case D_NODOOR:
            msg =
               "A cloud of dust springs up and assembles itself into %s!";
            doormsg = doormsg ? an(doormsg) : "";
            break;
        case D_PORTCULLIS:
            msg = "";
            doormsg = "";
            /* Not reached */
            break;
        default:
            res = FALSE;
            break;
        }
        if(door_blocks_vision_at_ptr(door))
            block_vision_and_hearing_at_point(x, y);
        if (can_lock)
        {
            door->doormask &= ~D_MASK;
            door->doormask |= D_LOCKED | (door->doormask & D_TRAPPED);
            door->key_otyp = MAGIC_KEY;
            door->special_quality = 0;
        }
        else
        {
            door->doormask &= ~D_MASK;
            door->doormask |= D_CLOSED | (door->doormask & D_TRAPPED);

        }
        newsym(x, y);
        break;
    }
    case WAN_OPENING:
    case SPE_KNOCK:
    {
        boolean can_open = (has_door_normal_lock_at_ptr(door) || (door->key_otyp == MAGIC_KEY && door->special_quality == 0));
        if (can_open && (door->doormask & D_LOCKED)) {
            msg = "The %s unlocks!";
            door->doormask &= ~D_MASK;
            door->doormask |= D_CLOSED | (door->doormask & D_TRAPPED);
        }
        else
            res = FALSE;
        break;
    }
    case WAN_STRIKING:
    case SPE_FORCE_BOLT:
    case SPE_FORCE_STRIKE:
        if (door->doormask & (D_LOCKED | D_CLOSED))
        {
            if (door->doormask & D_TRAPPED)
            {
                boolean spef_on = FALSE;
                if (MON_AT(x, y))
                    (void) mb_trapped(m_at(x, y));
                else if (flags.verbose) 
                {
                    if (cansee(x, y) && !Unaware)
                    {
                        play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_FIERY, x, y, 0.15);
                        play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, x, y, FALSE);
                        special_effect_wait_until_action(0);
                        spef_on = TRUE;
                        pline("KABOOM!!  You see a door explode.");
                    }
                    else if (!Deaf)
                    {
                        play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_FIERY, x, y, 0.15);
                        You_hear("a distant explosion.");
                    }
                }
                door->doormask &= ~D_TRAPPED;
                if (is_door_destroyed_by_booby_trap_at_ptr(door))
                {
                    door->doormask &= ~D_MASK;
                    door->doormask |= D_NODOOR;
                    unblock_vision_and_hearing_at_point(x, y);
                    newsym(x, y);
                }
                if(spef_on)
                    special_effect_wait_until_end(0);
                loudness = 40;
                break;
            }
            if (is_door_destroyed_by_striking_at(x, y))
            {
                door->doormask &= ~D_MASK;
                door->doormask |= D_BROKEN;
                if (flags.verbose) 
                {
                    play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_BREAK);
                    if (cansee(x, y))
                        pline_The("door crashes open!");
                    else
                        You_hear("a crashing sound.");
                }
                unblock_vision_and_hearing_at_point(x, y);
                newsym(x, y);
                /* force vision recalc before printing more messages */
                if (vision_full_recalc)
                    vision_recalc(0);
                loudness = 20;
            }
            else
            {
                if (flags.verbose) 
                {
                    play_simple_location_sound(x, y, LOCATION_SOUND_TYPE_WHAM);
                    if (cansee(x, y))
                        pline_The("%s resists!", get_door_name_at(x, y));
                    else
                        You_hear("a loud bang.");
                }
            }
        } else
            res = FALSE;
        break;
    default:
        impossible("magic (%d) attempted on door.", otmp->otyp);
        break;
    }
    if (msg && strcmp(msg, "") && doormsg && cansee(x, y))
        pline(msg, doormsg);

    if (loudness > 0) {
        /* door was destroyed */
        wake_nearto(x, y, loudness);
        if (*in_rooms(x, y, SHOPBASE))
            add_damage(x, y, 0L);
    }

    if (res && picking_at(x, y)) {
        /* maybe unseen monster zaps door you're unlocking */
        stop_occupation();
        reset_pick();
    }
    return res;
}

STATIC_OVL void
chest_shatter_msg(otmp, x, y)
struct obj *otmp;
int x, y;
{
    if (!otmp || !isok(x, y))
        return;

    const char *disposition;
    const char *thing;
    long save_Blinded;

    play_simple_object_sound_at_location(otmp, x, y, OBJECT_SOUND_TYPE_BREAK);
    if (otmp->oclass == POTION_CLASS) {
        char dcbuf[BUFSZ] = "";
        Sprintf(dcbuf, "You %s %s shatter!", Blind ? "hear" : "see", an(bottlename()));
        pline1(dcbuf);
        if (!has_innate_breathless(youmonst.data) || haseyes(youmonst.data))
            potionbreathe(otmp, dcbuf);
        return;
    }
    /* We have functions for distant and singular names, but not one */
    /* which does _both_... */
    save_Blinded = Blinded;
    Blinded = 1;
    thing = singular(otmp, xname);
    Blinded = save_Blinded;
    switch (objects[otmp->otyp].oc_material) {
    case MAT_PAPER:
        disposition = "is torn to shreds";
        break;
    case MAT_WAX:
        disposition = "is crushed";
        break;
    case MAT_VEGGY:
        disposition = "is pulped";
        break;
    case MAT_FLESH:
        disposition = "is mashed";
        break;
    case MAT_GLASS:
    case MAT_CRYSTAL:
    case MAT_GEMSTONE:
    case MAT_HARD_CRYSTAL:
        disposition = "shatters";
        break;
    case MAT_WOOD:
        disposition = "splinters to fragments";
        break;
    default:
        disposition = "is destroyed";
        break;
    }
    pline("%s %s!", An(thing), disposition);
}

/*lock.c*/

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    pickup.c    $NHDT-Date: 1545785547 2018/12/26 00:52:27 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.222 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 *      Contains code for picking objects up, and container use.
 */

#include "hack.h"

#define CONTAINED_SYM '>' /* from invent.c */

STATIC_DCL void FDECL(simple_look, (struct obj *, BOOLEAN_P));
STATIC_DCL boolean FDECL(query_classes, (char *, boolean *, boolean *,
                                         const char *, struct obj *,
                                         BOOLEAN_P, int *));
STATIC_DCL boolean FDECL(fatal_corpse_mistake, (struct obj *, BOOLEAN_P));
STATIC_DCL void FDECL(check_here, (BOOLEAN_P));
STATIC_DCL boolean FDECL(n_or_more, (struct obj *));
STATIC_DCL boolean FDECL(all_but_uchain, (struct obj *));
#if 0 /* not used */
STATIC_DCL boolean FDECL(allow_cat_no_uchain, (struct obj *));
#endif
STATIC_DCL int FDECL(autopick, (struct obj *, int, menu_item **));
STATIC_DCL int FDECL(count_categories, (struct obj *, int));
STATIC_DCL int FDECL(delta_cwt, (struct obj *, struct obj *));
STATIC_DCL int64_t FDECL(carry_count, (struct obj *, struct obj *, int64_t,
                                    BOOLEAN_P, int *, int *));
STATIC_DCL int FDECL(lift_object, (struct obj *, struct obj *, int64_t *,
                                   BOOLEAN_P));
STATIC_DCL boolean FDECL(mbag_explodes, (struct obj *, struct obj*, int));
STATIC_DCL int64_t FDECL(boh_loss, (struct obj *container, int));
STATIC_PTR int FDECL(in_container_core, (struct obj*, BOOLEAN_P));
STATIC_PTR int FDECL(out_container_core, (struct obj*, BOOLEAN_P, BOOLEAN_P, uchar*));
STATIC_PTR int FDECL(move_container_core, (struct obj*, BOOLEAN_P));
STATIC_PTR int FDECL(out_container_and_drop_core, (struct obj*, BOOLEAN_P));
STATIC_PTR int FDECL(pickup_and_in_container_core, (struct obj*, BOOLEAN_P));
STATIC_PTR int FDECL(in_container, (struct obj*));
STATIC_PTR int FDECL(out_container, (struct obj*));
STATIC_PTR int FDECL(move_container, (struct obj*));
STATIC_PTR int FDECL(out_container_and_drop, (struct obj*));
STATIC_PTR int FDECL(pickup_and_in_container, (struct obj*));
STATIC_PTR int FDECL(out_container_and_autostash, (struct obj*));
STATIC_PTR int FDECL(out_container_and_autostash_nobot, (struct obj*));
STATIC_PTR int FDECL(in_container_nobot, (struct obj*));
STATIC_PTR int FDECL(out_container_nobot, (struct obj*));
STATIC_PTR int FDECL(move_container_nobot, (struct obj*));
STATIC_PTR int FDECL(out_container_and_drop_nobot, (struct obj*));
STATIC_PTR int FDECL(pickup_and_in_container_nobot, (struct obj*));
STATIC_DCL void FDECL(removed_from_icebox, (struct obj *));
STATIC_DCL int64_t FDECL(mbag_item_gone, (int, struct obj *));
STATIC_DCL void FDECL(explain_container_prompt, (BOOLEAN_P));
STATIC_DCL int FDECL(traditional_loot, (int, struct obj*, struct obj*));
STATIC_DCL int FDECL(menu_loot, (int, int, struct obj*, struct obj*));
STATIC_DCL char FDECL(in_or_out_menu, (const char *, struct obj *, BOOLEAN_P,
                                       BOOLEAN_P, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL boolean FDECL(able_to_loot, (int, int, BOOLEAN_P));
STATIC_DCL boolean NDECL(reverse_loot);
//STATIC_DCL boolean FDECL(mon_beside, (int, int));
STATIC_DCL int FDECL(do_loot_cont, (struct obj **, int, int));
STATIC_DCL void FDECL(tipcontainer, (struct obj *));
STATIC_DCL void FDECL(loot_decoration, (int, int, int, boolean*));
STATIC_OVL boolean FDECL(check_nonmergeable_nobj, (struct obj*));
STATIC_DCL boolean FDECL(check_nonmergeable_pick_list, (menu_item*, int, int));

/* define for query_objlist() and autopickup() */
#define FOLLOW(curr, flags) \
    (((flags) & BY_NEXTHERE) ? (curr)->nexthere : (curr)->nobj)

#define GOLD_WT(n) (((n) + 50L) / 100L)
/* if you can figure this out, give yourself a hearty pat on the back... */
#define GOLD_CAPACITY(w, n) (((w) * -100L) - ((n) + 50L) - 1L)

/* A variable set in use_container(), to be used by the callback routines
   in_container() and out_container() from askchain() and use_container().
   Also used by menu_loot() and container_gone(). */
STATIC_VAR NEARDATA struct obj *current_container;
STATIC_VAR NEARDATA struct obj* move_target_container;
STATIC_VAR NEARDATA boolean abort_looting;
#define Icebox (current_container->otyp == ICE_BOX)

STATIC_VAR const char
        moderateloadmsg[] = "You have a little trouble lifting",
        nearloadmsg[] = "You have much trouble lifting",
        overloadmsg[] = "You have extreme difficulty lifting";

/* BUG: this lets you look at cockatrice corpses while blind without
   touching them */
/* much simpler version of the look-here code; used by query_classes() */
STATIC_OVL void
simple_look(otmp, here)
struct obj *otmp; /* list of objects */
boolean here;     /* flag for type of obj list linkage */
{
    /* Neither of the first two cases is expected to happen, since
     * we're only called after multiple classes of objects have been
     * detected, hence multiple objects must be present.
     */
    if (!otmp) {
        impossible("simple_look(null)");
    } else if (!(here ? otmp->nexthere : otmp->nobj)) {
        pline1(doname(otmp));
    } else {
        winid tmpwin = create_nhwindow(NHW_MENU);

        putstr(tmpwin, 0, "");
        do {
            putstr(tmpwin, 0, doname(otmp));
            otmp = here ? otmp->nexthere : otmp->nobj;
        } while (otmp);
        display_nhwindow(tmpwin, TRUE);
        destroy_nhwindow(tmpwin);
    }
}

int
collect_obj_classes(ilets, otmp, here, filter, itemcount)
char ilets[];
register struct obj *otmp;
boolean here;
boolean FDECL((*filter), (OBJ_P));
int *itemcount;
{
    register int iletct = 0;
    register char c;

    *itemcount = 0;
    ilets[iletct] = '\0'; /* terminate ilets so that index() will work */
    while (otmp) {
        c = def_oc_syms[(int) otmp->oclass].sym;
        if (!index(ilets, c) && (!filter || (*filter)(otmp)))
            ilets[iletct++] = c, ilets[iletct] = '\0';
        *itemcount += 1;
        otmp = here ? otmp->nexthere : otmp->nobj;
    }

    return iletct;
}

/*
 * Suppose some '?' and '!' objects are present, but '/' objects aren't:
 *      "a" picks all items without further prompting;
 *      "A" steps through all items, asking one by one;
 *      "?" steps through '?' items, asking, and ignores '!' ones;
 *      "/" becomes 'A', since no '/' present;
 *      "?a" or "a?" picks all '?' without further prompting;
 *      "/a" or "a/" becomes 'A' since there aren't any '/'
 *          (bug fix:  3.1.0 thru 3.1.3 treated it as "a");
 *      "?/a" or "a?/" or "/a?",&c picks all '?' even though no '/'
 *          (ie, treated as if it had just been "?a").
 */
STATIC_OVL boolean
query_classes(oclasses, one_at_a_time, everything, action, objs, here, menu_on_demand)
char oclasses[];
boolean *one_at_a_time, *everything;
const char *action;
struct obj *objs;
boolean here;
int *menu_on_demand;
{
    char ilets[BUFSZ], inbuf[BUFSZ] = DUMMY; /* FIXME: hardcoded ilets[] length */
    int iletct, oclassct;
    boolean not_everything, filtered;
    char qbuf[QBUFSZ];
    char ebuf[QBUFSZ];
    boolean m_seen;
    int itemcount, bcnt, ucnt, ccnt, xcnt, ocnt, tcnt;

    oclasses[oclassct = 0] = '\0';
    *one_at_a_time = *everything = m_seen = FALSE;
    if (menu_on_demand)
        *menu_on_demand = 0;
    iletct = collect_obj_classes(ilets, objs, here, (boolean FDECL((*), (OBJ_P))) 0, &itemcount);
    if (iletct == 0)
        return FALSE;

    if (iletct == 1)
    {
        oclasses[0] = def_char_to_objclass(ilets[0]);
        oclasses[1] = '\0';
    }
    else 
    { /* more than one choice available */
        /* additional choices */
        ilets[iletct++] = ' ';
        ilets[iletct++] = 'a';
        ilets[iletct++] = 'A';
        ilets[iletct++] = (objs == invent ? 'i' : ':');
    }
    if (itemcount && menu_on_demand)
        ilets[iletct++] = 'm';
    if (count_unpaid(objs, 0, here))
        ilets[iletct++] = 'u';
    if (count_unidentified(objs, 0, here))
        ilets[iletct++] = 'I';

    tally_BUCX(objs, here, &bcnt, &ucnt, &ccnt, &xcnt, &ocnt, &tcnt);
    if (bcnt)
        ilets[iletct++] = 'B';
    if (ucnt)
        ilets[iletct++] = 'U';
    if (ccnt)
        ilets[iletct++] = 'C';
    if (xcnt)
        ilets[iletct++] = 'X';
    ilets[iletct] = '\0';

    if (tcnt == 1)
    {
        *everything = TRUE;
    }
    else if (iletct > 1) 
    {
        const char *where = 0;
        char sym, oc_of_sym, *p;

    ask_again:
        oclasses[oclassct = 0] = '\0';
        *one_at_a_time = *everything = FALSE;
        not_everything = filtered = FALSE;
        Sprintf(qbuf, "What kinds of thing do you want to %s?", action);
        Sprintf(ebuf, "[%s]", ilets);
        getlin_ex(GETLINE_GENERAL, ATR_NONE, NO_COLOR, qbuf, inbuf, (char*)0, ebuf, (char*)0);
        if (*inbuf == '\033')
            return FALSE;

        for (p = inbuf; (sym = *p++) != 0; )
        {
            if (sym == ' ')
                continue;
            else if (sym == 'A')
                *one_at_a_time = TRUE;
            else if (sym == 'a')
                *everything = TRUE;
            else if (sym == ':') 
            {
                simple_look(objs, here); /* dumb if objs==invent */
                /* if we just scanned the contents of a container
                   then mark it as having known contents */
                if (objs->where == OBJ_CONTAINED)
                    objs->ocontainer->cknown = 1;
                goto ask_again;
            }
            else if (sym == 'i') 
            {
                (void) display_inventory((char *) 0, TRUE, 1);
                goto ask_again;
            }
            else if (sym == 'm') 
            {
                m_seen = TRUE;
            } 
            else if (index("uIBUCX", sym)) 
            {
                add_valid_menu_class(sym); /* 'u' or 'B','U','C',or 'X' */
                filtered = TRUE;
            }
            else
            {
                oc_of_sym = def_char_to_objclass(sym);
                if (index(ilets, sym)) 
                {
                    add_valid_menu_class(oc_of_sym);
                    oclasses[oclassct++] = oc_of_sym;
                    oclasses[oclassct] = '\0';
                } 
                else 
                {
                    if (!where)
                        where = strstr(action, "pick up") ? "here"
                                : strstr(action, "take out") ? "inside" : "";
                    if (*where)
                        There("are no %c's %s.", sym, where);
                    else
                        You("have no %c's.", sym);
                    not_everything = TRUE;
                }
            }
        } /* for p:sym in inbuf */

        if (m_seen && menu_on_demand) 
        {
            *menu_on_demand = (((*everything || !oclassct) && !filtered)
                               ? -2 : -3);
            return FALSE;
        }
        if (!oclassct && (!*everything || not_everything))
        {
            /* didn't pick anything,
               or tried to pick something that's not present */
            *one_at_a_time = TRUE; /* force 'A' */
            *everything = FALSE;   /* inhibit 'a' */
        }
    }
    return TRUE;
}

/* check whether hero is bare-handedly touching a cockatrice corpse */
STATIC_OVL boolean
fatal_corpse_mistake(obj, remotely)
struct obj *obj;
boolean remotely;
{
    if (uarmg || remotely || obj->otyp != CORPSE
        || !touch_petrifies(&mons[obj->corpsenm]) || Stone_resistance)
        return FALSE;

    if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM)) {
        display_nhwindow(WIN_MESSAGE, FALSE); /* --More-- */
        return FALSE;
    }

    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Touching %s is a fatal mistake.",
          corpse_xname(obj, (const char *) 0, CXN_SINGULAR | CXN_ARTICLE));
    killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
    instapetrify(killer_xname(obj));
    return TRUE;
}

/* attempting to manipulate a Rider's corpse triggers its revival */
boolean
rider_corpse_revival(obj, remotely)
struct obj *obj;
boolean remotely;
{
    if (!obj || obj->otyp != CORPSE || !is_rider(&mons[obj->corpsenm]))
        return FALSE;

    pline_ex(ATR_NONE, CLR_MSG_WARNING, "At your %s, the corpse suddenly moves...",
          remotely ? "attempted acquisition" : "touch");
    (void) revive_corpse(obj);
    exercise(A_WIS, FALSE);
    return TRUE;
}

/* look at the objects at our location, unless there are too many of them */
STATIC_OVL void
check_here(picked_some)
boolean picked_some;
{
    register struct obj *obj;
    register int ct = 0;

    /* count the objects here */
    for (obj = level.objects[u.ux][u.uy]; obj; obj = obj->nexthere) {
        if (obj != uchain)
            ct++;
    }

    /* If there are objects here, take a look. */
    if (ct) {
        if (context.run)
            nomul(0);
        flush_screen(1);
        (void) look_here(ct, picked_some, FALSE);
    } else {
        read_engr_at(u.ux, u.uy);
    }
}

/* Value set by query_objlist() for n_or_more(). */
STATIC_VAR int64_t val_for_n_or_more;

/* query_objlist callback: return TRUE if obj's count is >= reference value */
STATIC_OVL boolean
n_or_more(obj)
struct obj *obj;
{
    if (obj == uchain)
        return FALSE;
    return (boolean) (obj->quan >= val_for_n_or_more);
}

/* list of valid menu classes for query_objlist() and allow_category callback
   (with room for all object classes, 'u'npaid, IK, BUCX, and terminator) */
STATIC_VAR char valid_menu_classes[MAX_OBJECT_CLASSES + 3 + 4 + 1];
STATIC_VAR boolean class_filter, bucx_filter, shop_filter, unidentified_filter, unknown_filter;

/* check valid_menu_classes[] for an entry; also used by askchain() */
boolean
menu_class_present(c)
int c;
{
    return (c && index(valid_menu_classes, c)) ? TRUE : FALSE;
}

void
add_valid_menu_class(c)
int c;
{
    static int vmc_count = 0;

    if (c == 0) { /* reset */
        vmc_count = 0;
        class_filter = bucx_filter = shop_filter = unidentified_filter = unknown_filter = FALSE;
    } else if (!menu_class_present(c)) {
        valid_menu_classes[vmc_count++] = (char) c;
        /* categorize the new class */
        switch (c) {
        case 'B':
        case 'U':
        case 'C': /*FALLTHRU*/
        case 'X':
            bucx_filter = TRUE;
            break;
        case 'I':
            unidentified_filter = TRUE;
            break;
        case 'K':
            unknown_filter = TRUE;
            break;
        case 'u':
            shop_filter = TRUE;
            break;
        default:
            class_filter = TRUE;
            break;
        }
    }
    valid_menu_classes[vmc_count] = '\0';
}

/* query_objlist callback: return TRUE if not uchain */
STATIC_OVL boolean
all_but_uchain(obj)
struct obj *obj;
{
    return (boolean) (obj != uchain);
}

/* query_objlist callback: return TRUE if not a coin */
boolean
allow_all_but_coins(obj)
struct obj* obj;
{
    return (boolean)(obj->oclass != COIN_CLASS);
}


/* query_objlist callback: return TRUE */
/*ARGUSED*/
boolean
allow_all(obj)
struct obj *obj UNUSED;
{
    return TRUE;
}

boolean
allow_category(obj)
struct obj *obj;
{
    /* For coins, if any class filter is specified, accept if coins
     * are included regardless of whether either unpaid or BUC-status
     * is also specified since player has explicitly requested coins.
     * If no class filtering is specified but bless/curse state is,
     * coins are either unknown or uncursed based on an option setting.
     */
    if (obj->oclass == COIN_CLASS)
        return class_filter
                 ? (index(valid_menu_classes, COIN_CLASS) ? TRUE : FALSE)
                 : shop_filter /* coins are never unpaid, but check anyway */
                    ? (obj->unpaid ? TRUE : FALSE)
                    : bucx_filter
                       ? (index(valid_menu_classes, iflags.goldX ? 'X' : 'U')
                          ? TRUE : FALSE)
                       : unidentified_filter ? FALSE
                       : unknown_filter ? FALSE :
                          TRUE; /* catchall: no filters specified, so accept */

    if (Role_if(PM_PRIEST))
        obj->bknown = TRUE;

    /*
     * There are three types of filters possible and the first and
     * third can have more than one entry:
     *  1) object class (armor, potion, &c);
     *  2) unpaid shop item;
     *  3) bless/curse state (blessed, uncursed, cursed, BUC-unknown).
     * When only one type is present, the situation is simple:
     * to be accepted, obj's status must match one of the entries.
     * When more than one type is present, the obj will now only
     * be accepted when it matches one entry of each type.
     * So ?!B will accept blessed scrolls or potions, and [u will
     * accept unpaid armor.  (In 3.4.3, an object was accepted by
     * this filter if it met any entry of any type, so ?!B resulted
     * in accepting all scrolls and potions regardless of bless/curse
     * state plus all blessed non-scroll, non-potion objects.)
     */

    /* if class is expected but obj's class is not in the list, reject */
    if (class_filter && !index(valid_menu_classes, obj->oclass))
        return FALSE;
    /* if unpaid is expected and obj isn't unpaid, reject (treat a container
       holding any unpaid object as unpaid even if isn't unpaid itself) */
    if (shop_filter && !obj->unpaid
        && !(Has_contents(obj) && count_unpaid(obj->cobj, 0, FALSE) > 0))
        return FALSE;
    /* reject fully identified objects */
    if (unidentified_filter && !not_fully_identified(obj))
        return FALSE;
    if (unknown_filter && !is_obj_unknown(obj))
        return FALSE;
    /* check for particular bless/curse state */
    if (bucx_filter) {
        /* first categorize this object's bless/curse state */
        char bucx = !obj->bknown ? 'X'
                      : obj->blessed ? 'B' : obj->cursed ? 'C' : 'U';

        /* if its category is not in the list, reject */
        if (!index(valid_menu_classes, bucx))
            return FALSE;
    }
    /* obj didn't fail any of the filter checks, so accept */
    return TRUE;
}

#if 0 /* not used */
/* query_objlist callback: return TRUE if valid category (class), no uchain */
STATIC_OVL boolean
allow_cat_no_uchain(obj)
struct obj *obj;
{
    if (obj != uchain
        && ((index(valid_menu_classes, 'u') && obj->unpaid)
            || index(valid_menu_classes, obj->oclass)))
        return TRUE;
    return FALSE;
}
#endif

boolean
is_potion_of_water(obj)
struct obj* obj;
{
    return (obj && obj->otyp == POT_WATER);
}

boolean
is_scroll_of_remove_curse(obj)
struct obj* obj;
{
    return (obj && obj->otyp == SCR_REMOVE_CURSE);
}


/* query_objlist callback: return TRUE if valid class and worn */
boolean
is_worn_by_type(otmp)
register struct obj *otmp;
{
    return (is_worn(otmp) && allow_category(otmp)) ? TRUE : FALSE;
}

boolean
is_wearable_by_type(otmp)
register struct obj* otmp;
{
    return (is_wearable(otmp) && allow_category(otmp)) ? TRUE : FALSE;
}

/*
 * Have the hero pick things from the ground
 * or a monster's inventory if swallowed.
 *
 * Arg what:
 *      >0  autopickup
 *      =0  interactive
 *      <0  pickup count of something
 *
 * Returns 1 if tried to pick something up, whether
 * or not it succeeded.
 */
int
pickup(what, do_auto_in_bag)
int what; /* should be a int64_t */
boolean do_auto_in_bag;
{
    int i, n, res = 0, count, n_tried = 0, n_picked = 0;
    menu_item *pick_list = (menu_item *) 0;
    boolean autopickup = what > 0;
    struct obj **objchain_p;
    int traverse_how;
    struct obj* last_container = 0;
    int container_cnt = count_other_containers(invent, (struct obj*)0, &last_container, FALSE);
    struct obj* used_container = 0;

    /* we might have arrived here while fainted or sleeping, via
       random teleport or levitation timeout; if so, skip check_here
       and read_engr_at in addition to bypassing autopickup itself
       [probably ought to check whether hero is using a cockatrice
       corpse for a pillow here... (also at initial faint/sleep)] */
    if (autopickup && ((multi < 0 && unconscious()) || Sleeping || Paralyzed_or_immobile))
        return 0;

    if (what < 0) /* pick N of something */
        count = -what;
    else /* pick anything */
        count = 0;

    if (!u.uswallow) 
    {
        struct trap *ttmp;

        /* no auto-pick if no-pick move, nothing there, or in a pool */
        if (autopickup && (context.nopick || !OBJ_AT(u.ux, u.uy)
                           || (is_pool(u.ux, u.uy) && !Underwater)
                           || is_lava(u.ux, u.uy)))
        {
            read_engr_at(u.ux, u.uy);
            return 0;
        }
        /* no pickup if levitating & not on air or water level */
        if (!can_reach_floor(TRUE))
        {
            if ((multi && !context.run) || (autopickup && !(flags.pickup || flags.pickup_thrown))
                || ((ttmp = t_at(u.ux, u.uy)) != 0
                    && uteetering_at_seen_pit(ttmp)))
                read_engr_at(u.ux, u.uy);
            return 0;
        }
        /* multi && !context.run means they are in the middle of some other
         * action, or possibly paralyzed, sleeping, etc.... and they just
         * teleported onto the object.  They shouldn't pick it up.
         */
        if ((multi && !context.run) || (autopickup && !(flags.pickup || flags.pickup_thrown))) 
        {
            check_here(FALSE);
            return 0;
        }
        if (notake(youmonst.data)) 
        {
            if (!autopickup)
                You("are physically incapable of picking anything up.");
            else
                check_here(FALSE);
            return 0;
        }

        /* if there's anything here, stop running */
        if (OBJ_AT(u.ux, u.uy) && context.run && context.run != 8
            && !context.nopick)
            nomul(0);
    }

    add_valid_menu_class(0); /* reset */
    if (!u.uswallow) 
    {
        objchain_p = &level.objects[u.ux][u.uy];
        traverse_how = BY_NEXTHERE;
    }
    else 
    {
        objchain_p = &u.ustuck->minvent;
        traverse_how = 0; /* nobj */
    }
    /*
     * Start the actual pickup process.  This is split into two main
     * sections, the newer menu and the older "traditional" methods.
     * Automatic pickup has been split into its own menu-style routine
     * to make things less confusing.
     */
    if (autopickup) 
    {
        n = autopick(*objchain_p, traverse_how, &pick_list);
        goto menu_pickup;
    }

    if (flags.menu_style != MENU_TRADITIONAL || iflags.menu_requested) 
    {
        /* use menus exclusively */
        traverse_how |= AUTOSELECT_SINGLE
                        | (flags.sortpack ? INVORDER_SORT : 0);
        if (count) 
        { /* looking for N of something */
            char qbuf[QBUFSZ];

            Sprintf(qbuf, "Pick%s %d of what?", do_auto_in_bag ? " and out into bag" : "", count);
            val_for_n_or_more = count; /* set up callback selector */
            n = query_objlist(qbuf, objchain_p, traverse_how | OBJECT_COMPARISON,
                              &pick_list, PICK_ONE, n_or_more, 2);
            /* correct counts, if any given */
            for (i = 0; i < n; i++)
                pick_list[i].count = count;
        }
        else 
        {
            n = query_objlist(do_auto_in_bag ? "Pick up and auto-stash what?" : "Pick up what?", objchain_p,
                              (traverse_how | FEEL_COCKATRICE | OBJECT_COMPARISON),
                              &pick_list, PICK_ANY, all_but_uchain, 2);
        }

    menu_pickup:
        n_tried = n;
        for (n_picked = i = 0; i < n; i++) 
        {
            uchar obj_gone = FALSE;
            res = pickup_object(pick_list[i].item.a_obj, pick_list[i].count, FALSE, do_auto_in_bag, &obj_gone);
            if (res < 0)
                break; /* can't continue */
            n_picked += res;
            if (!obj_gone && handle_knapsack_prefull(res, container_cnt, &do_auto_in_bag, &used_container, pick_list[i].item.a_obj, i, n, check_nonmergeable_pick_list(pick_list, i + 1, n)) < 0)
                break;
        }
        if (pick_list)
            free((genericptr_t) pick_list);
    } 
    else 
    {
        /* old style interface */
        int ct = 0;
        int64_t lcount;
        boolean all_of_a_type, selective, bycat;
        char oclasses[MAX_OBJECT_CLASSES + 10]; /* +10: room for B,U,C,X plus slop */
        struct obj *obj, *obj2;

        oclasses[0] = '\0';   /* types to consider (empty for all) */
        all_of_a_type = TRUE; /* take all of considered types */
        selective = FALSE;    /* ask for each item */

        /* check for more than one object */
        for (obj = *objchain_p; obj; obj = FOLLOW(obj, traverse_how))
            ct++;

        if (ct == 1 && count)
        {
            /* if only one thing, then pick it */
            obj = *objchain_p;
            lcount = min(obj->quan, (int64_t) count);
            n_tried++;
            if (pickup_object(obj, lcount, FALSE, do_auto_in_bag, (uchar*)0) > 0)
                n_picked++; /* picked something */
            goto end_query;
        } 
        else if (ct >= 2)
        {
            int via_menu = 0;

            There("are %s objects here.", (ct <= 10) ? "several" : "many");
            if (!query_classes(oclasses, &selective, &all_of_a_type,
                do_auto_in_bag ? "pick up and auto-stash" : "pick up", *objchain_p,
                               (traverse_how & BY_NEXTHERE) ? TRUE : FALSE,
                               &via_menu)) 
            {
                if (!via_menu)
                    goto pickupdone;
                if (selective)
                    traverse_how |= INVORDER_SORT;
                n = query_objlist(do_auto_in_bag ? "Pick up and auto-stash what?" : "Pick up what?", objchain_p, traverse_how | OBJECT_COMPARISON,
                                  &pick_list, PICK_ANY,
                                  (via_menu == -2) ? allow_all
                                                   : allow_category, 2);
                goto menu_pickup;
            }
        }
        bycat = (menu_class_present('B') || menu_class_present('U')
                 || menu_class_present('C') || menu_class_present('X'));

        for (obj = *objchain_p; obj; obj = obj2) 
        {
            obj2 = FOLLOW(obj, traverse_how);
            if (bycat ? !allow_category(obj)
                      : (!selective && oclasses[0]
                         && !index(oclasses, obj->oclass)))
                continue;

            lcount = -1L;
            if (!all_of_a_type) 
            {
                char qbuf[BUFSZ];

                (void) safe_qbuf(qbuf, do_auto_in_bag ? "Pick up " : "Pick up and auto-stash ", "?", obj, doname,
                                 ansimpleoname, something);
                switch ((obj->quan < 2L) ? ynaq(qbuf) : ynNaq(qbuf)) 
                {
                case 'q':
                    goto end_query; /* out 2 levels */
                case 'n':
                    continue;
                case 'a':
                    all_of_a_type = TRUE;
                    if (selective) 
                    {
                        selective = FALSE;
                        oclasses[0] = obj->oclass;
                        oclasses[1] = '\0';
                    }
                    break;
                case '#': /* count was entered */
                    if (!yn_number)
                        continue; /* 0 count => No */
                    lcount = (int64_t) yn_number;
                    if (lcount > obj->quan)
                        lcount = obj->quan;
                    /*FALLTHRU*/
                default: /* 'y' */
                    break;
                }
            }
            if (lcount == -1L)
                lcount = obj->quan;

            n_tried++;
            if ((res = pickup_object(obj, lcount, FALSE, do_auto_in_bag, (uchar*)0)) < 0)
                break;
            n_picked += res;
        }
    end_query:
        ; /* statement required after label */
    }

    if (!u.uswallow)
    {
        if (hides_under(youmonst.data))
            (void) hideunder(&youmonst);

        /* position may need updating (invisible hero) */
        if (n_picked)
            newsym_force(u.ux, u.uy);

        /* check if there's anything else here after auto-pickup is done */
        if (autopickup)
            check_here(n_picked > 0);
    }
 pickupdone:
    add_valid_menu_class(0); /* reset */
    update_here_window();

    int more_action = 0;
    if (res == -2 && flags.knapsack_prompt)
        more_action = handle_knapsack_full();

    return (n_tried > 0 || more_action > 0);
}

int
handle_knapsack_prefull(res, container_cnt, do_auto_in_bag_ptr, used_container_ptr, obj, i, n, noncoin_nonmergeable_found)
int res, container_cnt, i, n;
boolean* do_auto_in_bag_ptr;
struct obj** used_container_ptr;
struct obj* obj;
uchar noncoin_nonmergeable_found;
{
    if (res > 0 && container_cnt > 0 && !*do_auto_in_bag_ptr)
    {
        if (*used_container_ptr)
        {
            if (!stash_obj_in_container(obj, *used_container_ptr))
                return -1;
        }
        else if (i < n - 1 && inv_cnt(FALSE) >= 52)
        {
            if (noncoin_nonmergeable_found)
            {
                if (flags.knapsack_prompt)
                {
                    bot();
                    Your_ex1(ATR_NONE, CLR_MSG_ATTENTION, "knapsack cannot accommodate any more items.");
                    char qbuf[QBUFSZ];
                    Sprintf(qbuf, "Stash %s and the remaining items into a container?", thecxname(obj));
                    char ans = yn_function_es(YN_STYLE_KNAPSACK_FULL, ATR_NONE, CLR_MSG_ATTENTION, "Your Knapsack Is Full", qbuf, ynaqchars, 'a', ynaq3descs, (const char*)0);
                    switch (ans)
                    {
                    default:
                    case 'a':
                        (void)auto_bag_in(invent, obj, FALSE);
                        *do_auto_in_bag_ptr = TRUE;
                        break;
                    case 'y':
                        *used_container_ptr = select_other_container(invent, (struct obj*)0, FALSE);
                        if (*used_container_ptr)
                        {
                            if (!stash_obj_in_container(obj, *used_container_ptr))
                                return -1;
                        }
                        else
                            return -1;
                        break;
                    case 'n':
                        return -1;
                    case 'q':
                        return -1;
                    }
                    if (ans == 'a')
                    {
                        *do_auto_in_bag_ptr = TRUE;
                    }
                }
                else
                    *do_auto_in_bag_ptr = TRUE;
            }
        }
    }
    return 0;
}

int
handle_knapsack_full(VOID_ARGS)
{
    int more_action = 0;
    struct obj* last_container = 0;
    int cnt = count_other_containers(invent, (struct obj*)0, &last_container, FALSE);
    if (cnt > 0)
    {
        /* Ask for putting things in a bag or drop items */
        char ans = yn_function_es(YN_STYLE_KNAPSACK_FULL, ATR_NONE, CLR_MSG_ATTENTION, "Your Knapsack Is Full", "Stash items into a container or Drop them?", sadqchars, 'q', sadqdescs, (const char*)0);
        switch (ans)
        {
        case 's':
            {
                struct obj* container = select_other_container(invent, (struct obj*)0, FALSE);
                if (container)
                {
                    current_container = container;
                    add_valid_menu_class(0); /* reset */
                    if (flags.menu_style == MENU_TRADITIONAL)
                        more_action |= traditional_loot(1, (struct obj*)0, (struct obj*)0);
                    else
                        more_action |= (menu_loot(0, 1, (struct obj*)0, (struct obj*)0) > 0);
                    add_valid_menu_class(0);
                    if (more_action)
                        update_inventory();
                }
                break;
            }
        case 'a':
            more_action = doautostash();
            break;
        case 'd':
            more_action = doddrop();
            break;
        case 'q':
        default:
            break;
        }
    }
    else
    {
        char ans = yn_query("Do you want to drop items?");
        if (ans == 'y')
        {
            more_action = doddrop();
        }
    }

    return more_action;
}

boolean
is_autopickup_exception(obj, grab)
struct obj *obj;
boolean grab; /* forced pickup, rather than forced leave behind? */
{
    /*
     *  Does the text description of this match an exception?
     */
    struct autopickup_exception
        *ape = (grab) ? iflags.autopickup_exceptions[AP_GRAB]
                      : iflags.autopickup_exceptions[AP_LEAVE];

    if (ape) {
        char *objdesc = makesingular(doname(obj));

        while (ape) {
            if (regex_match(objdesc, ape->regex))
                return TRUE;
            ape = ape->next;
        }
    }
    return FALSE;
}

boolean
autopick_testobj(otmp, calc_costly)
struct obj *otmp;
boolean calc_costly;
{
    static boolean costly = FALSE;
    const char *otypes = flags.pickup_types;
    boolean pickit = FALSE;

    /* calculate 'costly' just once for a given autopickup operation */
    if (calc_costly)
        costly = (otmp->where == OBJ_FLOOR
                  && costly_spot(otmp->ox, otmp->oy));

    /* first check: reject if an unpaid item in a shop */
    if (costly && !otmp->no_charge)
        return FALSE;

    /* check for pickup_types */
    if (flags.pickup)
    {
        pickit = (!*otypes || index(otypes, otmp->oclass));
        /* check for "always pick up */
        if (!pickit)
            pickit = is_autopickup_exception(otmp, TRUE);
        /* then for "never pick up */
        if (pickit)
            pickit = !is_autopickup_exception(otmp, FALSE);
        /* pickup_thrown overrides pickup_types and exceptions */
    }
    if (!pickit)
        pickit = (flags.pickup_thrown && otmp->was_thrown);
    return pickit;
}

/*
 * Pick from the given list using flags.pickup_types.  Return the number
 * of items picked (not counts).  Create an array that returns pointers
 * and counts of the items to be picked up.  If the number of items
 * picked is zero, the pickup list is left alone.  The caller of this
 * function must free the pickup list.
 */
STATIC_OVL int
autopick(olist, follow, pick_list)
struct obj *olist;     /* the object list */
int follow;            /* how to follow the object list */
menu_item **pick_list; /* list of objects and counts to pick up */
{
    menu_item *pi; /* pick item */
    struct obj *curr;
    int n;
    boolean check_costly = TRUE;

    /* first count the number of eligible items */
    for (n = 0, curr = olist; curr; curr = FOLLOW(curr, follow)) {
        if (autopick_testobj(curr, check_costly))
            ++n;
        check_costly = FALSE; /* only need to check once per autopickup */
    }

    if (n) {
        *pick_list = pi = (menu_item *) alloc(sizeof (menu_item) * (size_t)n);
        for (n = 0, curr = olist; curr; curr = FOLLOW(curr, follow)) {
            if (autopick_testobj(curr, FALSE)) {
                pi[n].item.a_obj = curr;
                pi[n].count = curr->quan;
                n++;
            }
        }
    }
    return n;
}

/*
 * Put up a menu using the given object list.  Only those objects on the
 * list that meet the approval of the allow function are displayed.  Return
 * a count of the number of items selected, as well as an allocated array of
 * menu_items, containing pointers to the objects selected and counts.  The
 * returned counts are guaranteed to be in bounds and non-zero.
 *
 * Query flags:
 *      BY_NEXTHERE       - Follow object list via nexthere instead of nobj.
 *      AUTOSELECT_SINGLE - Don't ask if only 1 object qualifies - just
 *                          use it.
 *      USE_INVLET        - Use object's invlet.
 *      INVORDER_SORT     - Use hero's pack order.
 *      INCLUDE_HERO      - Showing engulfer's invent; show hero too.
 *      SIGNAL_NOMENU     - Return -1 rather than 0 if nothing passes "allow".
 *      SIGNAL_ESCAPE     - Return -1 rather than 0 if player uses ESC to
 *                          pick nothing.
 *      FEEL_COCKATRICE   - touch corpse.
 */
int
query_objlist(qstr, olist_p, qflags, pick_list, how, allow, show_weights)
const char *qstr;                 /* query string */
struct obj **olist_p;             /* the list to pick from */
int qflags;                       /* options to control the query */
menu_item **pick_list;            /* return list of items picked */
int how;                          /* type of query */
boolean FDECL((*allow), (OBJ_P)); /* allow function */
int show_weights;
{
    int i, n;
    winid win;
    struct obj *curr, *last, fake_hero_object, *olist = *olist_p;
    char *pack;
    anything any;
    boolean printed_type_name, first,
            sorted = (qflags & INVORDER_SORT) != 0,
            comparison_stats = (qflags & OBJECT_COMPARISON) != 0 && iflags.show_comparison_stats && !iflags.in_dumplog && !program_state.gameover,
            engulfer = (qflags & INCLUDE_HERO) != 0;
    unsigned sortflags;
    Loot *sortedolist, *srtoli;
    int wtcount = 0;

    boolean loadstonecorrectly = FALSE;

    if (show_weights == 1) // Inventory
        loadstonecorrectly = TRUE;
    else if (show_weights == 2) { // Pick up
        loadstonecorrectly = (boolean)objects[LOADSTONE].oc_name_known;
    }
    else if (show_weights == 3) // Drop or sell
        loadstonecorrectly = TRUE;

    *pick_list = (menu_item *) 0;
    if (!olist && !engulfer)
        return 0;

    /* count the number of items allowed */
    for (n = 0, last = 0, curr = olist; curr; curr = FOLLOW(curr, qflags))
        if ((*allow)(curr)) {
            last = curr;
            n++;
        }
    if (engulfer) {
        ++n;
        /* don't autoselect swallowed hero if it's the only choice */
        qflags &= ~AUTOSELECT_SINGLE;
    }

    if (n == 0) /* nothing to pick here */
        return (qflags & SIGNAL_NOMENU) ? -1 : 0;

    if (n == 1 && (qflags & AUTOSELECT_SINGLE)) {
        *pick_list = (menu_item *) alloc(sizeof (menu_item));
        (*pick_list)->item.a_obj = last;
        (*pick_list)->count = last->quan;
        return 1;
    }

    sortflags = (((flags.sortloot == 'f'
                   || (flags.sortloot == 'l' && !(qflags & USE_INVLET)))
                  ? SORTLOOT_LOOT
                  : ((qflags & USE_INVLET) ? SORTLOOT_INVLET : 0))
                 | (flags.sortpack ? SORTLOOT_PACK : 0)
                 | ((qflags & FEEL_COCKATRICE) ? SORTLOOT_PETRIFY : 0));
    sortedolist = sortloot(&olist, sortflags,
                           (qflags & BY_NEXTHERE) ? TRUE : FALSE, allow);

    win = create_nhwindow(NHW_MENU);
    start_menu_ex(win, GHMENU_STYLE_PICK_ITEM_LIST);
    any = zeroany;
    /*
     * Run through the list and add the objects to the menu.  If
     * INVORDER_SORT is set, we'll run through the list once for
     * each type so we can group them.  The allow function was
     * called by sortloot() and will be called once per item here.
     */
    pack = flags.inv_order;
    first = TRUE;
    do {
        printed_type_name = FALSE;
        for (srtoli = sortedolist; ((curr = srtoli->obj) != 0); ++srtoli) {
            if (sorted && curr->oclass != *pack)
                continue;
            if ((qflags & FEEL_COCKATRICE) && curr->otyp == CORPSE
                && will_feel_cockatrice(curr, FALSE)) {
                destroy_nhwindow(win); /* stop the menu and revert */
                (void) look_here(0, FALSE, TRUE);
                return 0;
            }
            if ((*allow)(curr)) {
                /* if sorting, print type name (once only) */
                if (sorted && !printed_type_name) {
                    any = zeroany;
                    add_extended_menu(win, NO_GLYPH, &any, 
                        0, 0, iflags.menu_headings, NO_COLOR,
                             let_to_name(*pack, FALSE,
                                         ((how != PICK_NONE)
                                          && iflags.menu_head_objsym)),
                             MENU_UNSELECTED,
                        menu_group_heading_info(*pack > ILLOBJ_CLASS && *pack < MAX_OBJECT_CLASSES ? def_oc_syms[(int)*pack].sym : '\0'));
                    printed_type_name = TRUE;
                }

                any.a_obj = curr;

                /* count the weight sum here */
                if(curr->otyp == LOADSTONE && !loadstonecorrectly)
                    wtcount += objects[LUCKSTONE].oc_weight;
                else
                    wtcount += curr->owt;

                char applied_invlet = (qflags & USE_INVLET) ? curr->invlet
                    : (first && curr->oclass == COIN_CLASS) ? GOLD_SYM : 0;

                char applied_group_accelerator = def_oc_syms[(int)objects[curr->otyp].oc_class].sym;

                int used_color = NO_COLOR;
                if ((qflags & WORN_UNSELECTABLE) && is_worn(curr))
                {
                    applied_invlet = 0;
                    applied_group_accelerator = 0;
                    any = zeroany;
                    used_color = CLR_GRAY;
                }

                xchar x = 0, y = 0;
                get_obj_location(curr, &x, &y, CONTAINED_TOO | BURIED_TOO);
                int glyph = obj_to_glyph(curr, rn2_on_display_rng);
                int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_OBJECT, curr, (struct monst*)0, 0UL, 0UL, 0UL, MAT_NONE, 0));
                char objbuf[BUFSZ * 2];
                char attrs[BUFSZ * 2];
                char colors[BUFSZ * 2];
                memset(attrs, ATR_NONE, sizeof(attrs));
                memset(colors, NO_COLOR, sizeof(colors));
                Strcpy(objbuf, 
                    show_weights > 0 ? 
                    (flags.inventory_weights_last ? doname_with_price_and_weight_last(curr, loadstonecorrectly) : doname_with_price_and_weight_first(curr, loadstonecorrectly)) : 
                    doname_with_price(curr));
                struct extended_menu_info eminfo = obj_to_extended_menu_info(curr);
                if (comparison_stats)
                {
                    char compbuf[BUFSZ];
                    print_comparison_stats(curr, compbuf, WIN_ERR, ATR_NONE, NO_COLOR, TRUE, TRUE, objbuf, attrs, colors);
                    eminfo.attrs = attrs;
                    eminfo.colors = colors;
                    eminfo.menu_flags |= MENU_FLAGS_USE_SPECIAL_SYMBOLS;
                }

                add_extended_menu(win, gui_glyph, &any,
                    applied_invlet, applied_group_accelerator, ATR_NONE, used_color,
                    objbuf,
                    MENU_UNSELECTED, eminfo);
                first = FALSE;
            }
        }
        pack++;
    } while (sorted && *pack);
    unsortloot(&sortedolist);

    if (engulfer) {
        char buf[BUFSZ];

        any = zeroany;
        if (sorted && n > 1) {
            Sprintf(buf, "%s Creatures",
                    is_animal(u.ustuck->data) ? "Swallowed" : "Engulfed");
            add_extended_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR, buf, MENU_UNSELECTED, menu_heading_info());
        }
        fake_hero_object = zeroobj;
        fake_hero_object.quan = 1L; /* not strictly necessary... */
        any.a_obj = &fake_hero_object;
        int glyph = any_mon_to_glyph(&youmonst, rn2_on_display_rng);
        int gui_glyph = maybe_get_replaced_glyph(glyph, u.ux, u.uy, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, &youmonst, 0UL, 0UL, 0UL, MAT_NONE, 0));
        add_menu(win, gui_glyph, &any,
                 /* fake inventory letter, no group accelerator */
                 CONTAINED_SYM, 0, ATR_NONE, NO_COLOR, an(self_lookat(buf)),
                 MENU_UNSELECTED);
    }

    if (flags.show_weight_summary)
        add_weight_summary(win, wtcount, show_weights);

    end_menu(win, qstr);
    n = select_menu(win, how, pick_list);
    destroy_nhwindow(win);

    if (n > 0) {
        menu_item *mi;
        int k;

        /* fix up counts:  -1 means no count used => pick all;
           if fake_hero_object was picked, discard that choice */
        for (i = k = 0, mi = *pick_list; i < n; i++, mi++) {
            if (mi->item.a_obj == &fake_hero_object)
                continue;
            if (mi->count == -1L || mi->count > mi->item.a_obj->quan)
                mi->count = mi->item.a_obj->quan;
            if (k < i)
                (*pick_list)[k] = *mi;
            ++k;
        }
        if (!k) {
            /* fake_hero was only choice so discard whole list */
            free((genericptr_t) *pick_list);
            *pick_list = 0;
            n = 0;
        } else if (k < n) {
            /* other stuff plus fake_hero; last slot is now unused */
            (*pick_list)[k].item = zeroany;
            (*pick_list)[k].count = 0L;
            n = k;
        }
    } else if (n < 0) {
        /* -1 is used for SIGNAL_NOMENU, so callers don't expect it
           to indicate that the player declined to make a choice */
        n = (qflags & SIGNAL_ESCAPE) ? -2 : 0;
    }
    return n;
}

/*
 * allow menu-based category (class) selection (for Drop,take off etc.)
 *
 */
int
query_category(qstr, olist, qflags, pick_list, how)
const char *qstr;      /* query string */
struct obj *olist;     /* the list to pick from */
int qflags;            /* behaviour modification flags */
menu_item **pick_list; /* return list of items picked */
int how;               /* type of query */
{
    int n;
    winid win;
    struct obj *curr;
    char *pack;
    anything any;
    boolean collected_type_name;
    char invlet;
    int ccount;
    boolean FDECL((*ofilter), (OBJ_P)) = (boolean FDECL((*), (OBJ_P))) 0;
    boolean do_unpaid = FALSE;
    boolean do_blessed = FALSE, do_cursed = FALSE, do_uncursed = FALSE,
            do_buc_unknown = FALSE, do_unidentified = FALSE, do_unknown = FALSE;
    int num_buc_types = 0;
    *pick_list = (menu_item *) 0;
    if (!olist)
        return 0;

    if (qflags & WORN_TYPES)
        ofilter = is_worn;
    else if (qflags & WEARABLE_TYPES)
        ofilter = is_wearable;

    int objcnt = count_objects(olist, ofilter, (qflags & BY_NEXTHERE) != 0);
    if (!objcnt)
        return 0;

    int unpaid_count = 0;
    if ((qflags & UNPAID_TYPES) && (unpaid_count = count_unpaid(olist, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0)
        do_unpaid = TRUE;

    int unidentified_count = 0;
    if ((qflags & UNIDENTIFIED_TYPES) && (unidentified_count = count_unidentified(olist, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0) {
        do_unidentified = TRUE;
    }
    int unknown_count = 0;
    if ((qflags & UNKNOWN_TYPES) && (unknown_count = count_unknown(olist, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0) {
        do_unknown = TRUE;
    }
    int blessed_count = 0;
    if ((qflags & BUC_BLESSED) && (blessed_count = count_buc(olist, BUC_BLESSED, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0) {
        do_blessed = TRUE;
        num_buc_types++;
    }
    int cursed_count = 0;
    if ((qflags & BUC_CURSED) && (cursed_count = count_buc(olist, BUC_CURSED, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0) {
        do_cursed = TRUE;
        num_buc_types++;
    }
    int uncursed_count = 0;
    if ((qflags & BUC_UNCURSED) && (uncursed_count = count_buc(olist, BUC_UNCURSED, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0) {
        do_uncursed = TRUE;
        num_buc_types++;
    }
    int unknown_buc_count = 0;
    if ((qflags & BUC_UNKNOWN) && (unknown_buc_count = count_buc(olist, BUC_UNKNOWN, ofilter, (qflags & BY_NEXTHERE) != 0)) > 0) {
        do_buc_unknown = TRUE;
        num_buc_types++;
    }

    ccount = count_categories(olist, qflags);
    /* no point in actually showing a menu for a single category */
    if ((objcnt == 1 || (ccount == 1 && (!do_unidentified || unidentified_count == objcnt) && (!do_unknown || unknown_count == objcnt) && (!do_unpaid || unpaid_count == objcnt) && num_buc_types <= 1)) && !(qflags & BILLED_TYPES))
    {
        for (curr = olist; curr; curr = FOLLOW(curr, qflags)) {
            if (ofilter && !(*ofilter)(curr))
                continue;
            break;
        }
        if (curr) {
            *pick_list = (menu_item *) alloc(sizeof(menu_item));
            (*pick_list)->item.a_int = do_unpaid ? 'u' : curr->oclass;
            return 1;
        } else {
            debugpline0("query_category: no single object match");
        }
        return 0;
    }

    win = create_nhwindow(NHW_MENU);
    start_menu_ex(win, GHMENU_STYLE_PICK_CATEGORY_LIST);
    pack = flags.inv_order;
    struct extended_menu_info info = zeroextendedmenuinfo;
    info.menu_flags |= MENU_FLAGS_USE_NUM_ITEMS;

    if (qflags & CHOOSE_ALL) {
        invlet = 'A';
        any = zeroany;
        any.a_int = 'A';
        info.num_items = objcnt;
        info.menu_flags |= MENU_FLAGS_AUTO_CLICK_OK;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 (qflags & WORN_TYPES) ? "Auto-select every item being worn" :
                 (qflags & WEARABLE_TYPES) ? "Auto-select every wearable item" :
                 "Auto-select every item",
                 MENU_UNSELECTED, info);

        any = zeroany;
        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_HALF_SIZE, NO_COLOR, "", MENU_UNSELECTED);
    }

    if ((qflags & ALL_TYPES) && (ccount > 1)) {
        invlet = 'a';
        any = zeroany;
        any.a_int = ALL_TYPES_SELECTED;
        info.num_items = objcnt;
        info.menu_flags |= MENU_FLAGS_AUTO_CLICK_OK;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 (qflags & WORN_TYPES) ? "All worn types" : (qflags & WEARABLE_TYPES) ? "All wearable types" : "All types",
                 MENU_UNSELECTED, info);
        invlet = 'b';
    } else
        invlet = 'a';

    do {
        collected_type_name = FALSE;
        for (curr = olist; curr; curr = FOLLOW(curr, qflags)) {
            if (curr->oclass == *pack) {
                if (ofilter && !(*ofilter)(curr))
                    continue;
                if (!collected_type_name) {
                    any = zeroany;
                    any.a_int = curr->oclass;
                    info.num_items = count_objects_in_class(olist, curr->oclass, ofilter, (qflags & BY_NEXTHERE) != 0);
                    if(ccount == 1)
                        info.menu_flags |= MENU_FLAGS_AUTO_CLICK_OK;
                    else
                        info.menu_flags &= ~MENU_FLAGS_AUTO_CLICK_OK;
                    add_extended_menu(
                        win, NO_GLYPH, &any, invlet++,
                        def_oc_syms[(int) objects[curr->otyp].oc_class].sym,
                        ATR_NONE, NO_COLOR, let_to_name(*pack, FALSE,
                                              (how != PICK_NONE)
                                                  && iflags.menu_head_objsym),
                        MENU_UNSELECTED, info);
                    collected_type_name = TRUE;
                }
            }
        }
        pack++;
        if (invlet >= 'u') {
            impossible("query_category: too many categories");
            return 0;
        }
    } while (*pack);

    info.menu_flags &= ~MENU_FLAGS_AUTO_CLICK_OK;
    if (do_unpaid || do_unidentified || do_unknown || (qflags & BILLED_TYPES) || do_blessed || do_cursed
        || do_uncursed || do_buc_unknown) {
        any = zeroany;
        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_HALF_SIZE, NO_COLOR, "", MENU_UNSELECTED);
    }

    /* unpaid items if there are any */
    if (do_unpaid) {
        invlet = 'u';
        any = zeroany;
        any.a_int = 'u';
        info.num_items = unpaid_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR, "Unpaid items",
                 MENU_UNSELECTED, info);
    }
    /* billed items: checked by caller, so always include if BILLED_TYPES */
    if (qflags & BILLED_TYPES) {
        invlet = 'x';
        any = zeroany;
        any.a_int = 'x';
        add_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 "Unpaid items already used up", MENU_UNSELECTED);
    }
    if (do_unidentified) {
        invlet = 'I';
        any = zeroany;
        any.a_int = 'I';
        info.num_items = unidentified_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
            "Unidentified items", MENU_UNSELECTED, info);
    }

    if (do_unknown) {
        invlet = 'K';
        any = zeroany;
        any.a_int = 'K';
        info.num_items = unknown_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
            "Unknown items", MENU_UNSELECTED, info);
    }

    /* items with b/u/c/unknown if there are any;
       this cluster of menu entries is in alphabetical order,
       reversing the usual sequence of 'U' and 'C' in BUCX */
    if (do_blessed) {
        invlet = 'B';
        any = zeroany;
        any.a_int = 'B';
        info.num_items = blessed_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 "Items known to be Blessed", MENU_UNSELECTED, info);
    }
    if (do_cursed) {
        invlet = 'C';
        any = zeroany;
        any.a_int = 'C';
        info.num_items = cursed_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 "Items known to be Cursed", MENU_UNSELECTED, info);
    }
    if (do_uncursed) {
        invlet = 'U';
        any = zeroany;
        any.a_int = 'U';
        info.num_items = uncursed_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 "Items known to be Uncursed", MENU_UNSELECTED, info);
    }
    if (do_buc_unknown) {
        invlet = 'X';
        any = zeroany;
        any.a_int = 'X';
        info.num_items = unknown_buc_count;
        add_extended_menu(win, NO_GLYPH, &any, invlet, 0, ATR_NONE, NO_COLOR,
                 "Items of unknown Bless/Curse status", MENU_UNSELECTED, info);
    }

    end_menu(win, qstr);
    n = select_menu(win, how, pick_list);
    destroy_nhwindow(win);
    if (n < 0)
        n = 0; /* caller's don't expect -1 */
    return n;
}

STATIC_OVL int
count_categories(olist, qflags)
struct obj *olist;
int qflags;
{
    char *pack;
    boolean counted_category;
    int ccount = 0;
    struct obj *curr;

    pack = flags.inv_order;
    do {
        counted_category = FALSE;
        for (curr = olist; curr; curr = FOLLOW(curr, qflags)) {
            if (curr->oclass == *pack) {
                if ((qflags & WORN_TYPES)
                    && !(curr->owornmask & (W_ARMOR | W_ACCESSORY | W_WEAPON)))
                    continue;
                if ((qflags & WEARABLE_TYPES)
                    && (!(curr->oclass == ARMOR_CLASS || curr->oclass == AMULET_CLASS || curr->oclass == RING_CLASS || curr->oclass == MISCELLANEOUS_CLASS)))
                    continue;
                if (!counted_category) {
                    ccount++;
                    counted_category = TRUE;
                }
            }
        }
        pack++;
    } while (*pack);
    return ccount;
}

/*
 *  How much the weight of the given container will change when the given
 *  object is removed from it.  Use before and after weight amounts rather
 *  than trying to match the calculation used by weight() in mkobj.c.
 */
STATIC_OVL int
delta_cwt(container, obj)
struct obj *container, *obj;
{
    struct obj **prev;
    int owt, nwt;

    if (!Is_weight_changing_bag(container))
        return obj->owt;

    owt = nwt = container->owt;
    /* find the object so that we can remove it */
    for (prev = contained_object_chain_ptr(container); *prev; prev = &(*prev)->nobj)
        if (*prev == obj)
            break;
    if (!*prev) {
        panic("delta_cwt: obj not inside container?");
        return 0;
    } else {
        /* temporarily remove the object and calculate resulting weight */
        *prev = obj->nobj;
        nwt = weight(container);
        *prev = obj; /* put the object back; obj->nobj is still valid */
    }
    return owt - nwt;
}

/* could we carry `obj'? if not, could we carry some of it/them? */
STATIC_OVL int64_t
carry_count(obj, container, count, telekinesis, wt_before, wt_after)
struct obj *obj, *container; /* object to pick up, bag it's coming out of */
int64_t count;
boolean telekinesis;
int *wt_before, *wt_after;
{
    boolean adjust_wt = container && carried(container),
            is_gold = obj->oclass == COIN_CLASS;
    int wt, iw, ow, oow;
    int64_t qq, savequan, umoney;
    unsigned saveowt;
    const char *verb, *prefx1, *prefx2, *suffx;
    char obj_nambuf[BUFSZ], where[BUFSZ];

    savequan = obj->quan;
    saveowt = obj->owt;
    umoney = money_cnt(invent);
    iw = max_capacity();

    if (count != savequan) {
        obj->quan = count;
        obj->owt = (unsigned) weight(obj);
    }
    wt = iw + (int) obj->owt;
    if (adjust_wt)
        wt -= delta_cwt(container, obj);
    /* This will go with silver+copper & new gold weight */
    if (is_gold) /* merged gold might affect cumulative weight */
        wt -= (int)(GOLD_WT(umoney) + GOLD_WT(count) - GOLD_WT(umoney + count));
    if (count != savequan) {
        obj->quan = savequan;
        obj->owt = saveowt;
    }
    *wt_before = iw;
    *wt_after = wt;

    if (wt < 0)
        return count;

    /* see how many we can lift */
    if (is_gold) {
        iw -= (int) GOLD_WT(umoney);
        if (!adjust_wt) {
            qq = GOLD_CAPACITY((int64_t) iw, umoney);
        } else {
            oow = 0;
            qq = 50L - (umoney % 100L) - 1L;
            if (qq < 0L)
                qq += 100L;
            for (; qq <= count; qq += 100L) {
                obj->quan = qq;
                obj->owt = (unsigned) GOLD_WT(qq);
                ow = (int) GOLD_WT(umoney + qq);
                ow -= delta_cwt(container, obj);
                if (iw + ow >= 0)
                    break;
                oow = ow;
            }
            iw -= oow;
            qq -= 100L;
        }
        if (qq < 0L)
            qq = 0L;
        else if (qq > count)
            qq = count;
        wt = iw + (int) GOLD_WT(umoney + qq);
    } else if (count > 1 || count < obj->quan) {
        /*
         * Ugh. Calc num to lift by changing the quan of of the
         * object and calling weight.
         *
         * This works for containers only because containers
         * don't merge.  -dean
         */
        for (qq = 1L; qq <= count; qq++) {
            obj->quan = qq;
            obj->owt = (unsigned) (ow = weight(obj));
            if (adjust_wt)
                ow -= delta_cwt(container, obj);
            if (iw + ow >= 0)
                break;
            wt = iw + ow;
        }
        --qq;
    } else {
        /* there's only one, and we can't lift it */
        qq = 0L;
    }
    obj->quan = savequan;
    obj->owt = saveowt;

    if (qq < count) {
        /* some message will be given */
        Strcpy(obj_nambuf, doname(obj));
        if (container) {
            Sprintf(where, "in %s", the(xname(container)));
            verb = "carry";
        } else {
            Strcpy(where, "lying here");
            verb = telekinesis ? "acquire" : "lift";
        }
    } else {
        /* lint suppression */
        *obj_nambuf = *where = '\0';
        verb = "";
    }
    /* we can carry qq of them */
    if (qq > 0) {
        if (qq < count)
            You("can only %s %s of the %s %s.", verb,
                (qq == 1L) ? "one" : "some", obj_nambuf, where);
        *wt_after = wt;
        return qq;
    }

    if (!container)
        Strcpy(where, "here"); /* slightly shorter form */
    if (invent || umoney) {
        prefx1 = "you cannot ";
        prefx2 = "";
        suffx = " any more";
    } else {
        prefx1 = (obj->quan == 1L) ? "it " : "even one ";
        prefx2 = "is too heavy for you to ";
        suffx = "";
    }

    play_sfx_sound(SFX_GENERAL_TOO_MUCH_ENCUMBRANCE);
    There_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s %s, but %s%s%s%s.", otense(obj, "are"), obj_nambuf, where,
          prefx1, prefx2, verb, suffx);

    /* *wt_after = iw; */
    return 0L;
}

/* determine whether character is able and player is willing to carry `obj' */
STATIC_OVL
int
lift_object(obj, container, cnt_p, telekinesis)
struct obj *obj, *container; /* object to pick up, bag it's coming out of */
int64_t *cnt_p;
boolean telekinesis;
{
    int result, old_wt, new_wt, prev_encumbr, next_encumbr;

    if (obj->otyp == BOULDER && Sokoban) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot get your %s around this %s.", body_part(HAND),
            xname(obj));
        return -1;
    }
    /* override weight consideration for loadstone picked up by anybody
       and for boulder picked up by hero poly'd into a giant; override
       availability of open inventory slot iff not already carrying one */
    if (obj->otyp == LOADSTONE
        || (obj->otyp == BOULDER && throws_rocks(youmonst.data))) {
        if (inv_cnt(FALSE) < 52 || !carrying(obj->otyp)
            || merge_choice(invent, obj))
            return 1; /* lift regardless of current situation */
        /* if we reach here, we're out of slots and already have at least
           one of these, so treat this one more like a normal item */
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are carrying too much stuff to pick up %s %s.",
            (obj->quan == 1L) ? "another" : "more", simpleonames(obj));
        return -1;
    }

    *cnt_p = carry_count(obj, container, *cnt_p, telekinesis,
                         &old_wt, &new_wt);
    if (*cnt_p < 1L) {
        result = -1; /* nothing lifted */
    } else if (obj->oclass != COIN_CLASS
               /* [exception for gold coins will have to change
                   if silver/copper ones ever get implemented] */
               && inv_cnt(FALSE) >= 52 && !merge_choice(invent, obj)) {
        /* if there is some gold here (and we haven't already skipped it),
           we aren't limited by the 52 item limit for it, but caller and
           "grandcaller" aren't prepared to skip stuff and then pickup
           just gold, so the best we can do here is vary the message */
        play_ui_sound(UI_SOUND_KNAPSACK_FULL);
        Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "knapsack cannot accommodate any more items%s.",
            /* floor follows by nexthere, otherwise container so by nobj */
            nxtobj(obj, GOLD_PIECE, (boolean)(obj->where == OBJ_FLOOR))
            ? " (except gold)" : "");        
        result = -2; /* nothing lifted; request for putting things into a bag */
    } else {
        result = 1;
        prev_encumbr = near_capacity();
        if (prev_encumbr < flags.pickup_burden)
            prev_encumbr = flags.pickup_burden;
        next_encumbr = calc_capacity(new_wt - old_wt);
        if (next_encumbr > prev_encumbr) {
            if (telekinesis) {
                result = 0; /* don't lift */
            } else {
                char qbuf[BUFSZ];
                int64_t savequan = obj->quan;

                obj->quan = *cnt_p;
                Strcpy(qbuf, (next_encumbr > HVY_ENCUMBER)
                                 ? overloadmsg
                                 : (next_encumbr > MOD_ENCUMBER)
                                       ? nearloadmsg
                                       : moderateloadmsg);
                if (container)
                    (void) strsubst(qbuf, "lifting", "removing");
                Strcat(qbuf, " ");
                (void) safe_qbuf(qbuf, qbuf, ".  Continue?", obj, doname,
                                 ansimpleoname, something);
                obj->quan = savequan;
                switch (ynq_ex(ATR_NONE, CLR_MSG_ATTENTION, (char*)0, qbuf)) {
                case 'q':
                    result = -1;
                    break;
                case 'n':
                    result = 0;
                    break;
                default:
                    break; /* 'y' => result == 1 */
                }
                clear_nhwindow(WIN_MESSAGE);
            }
        }
    }

    if (obj->otyp == SCR_SCARE_MONSTER && result <= 0 && !container)
        obj->special_quality = 0;

    return result;
}

/*
 * Pick up <count> of obj from the ground and add it to the hero's inventory.
 * Returns -1 if caller should break out of its loop, 0 if nothing picked
 * up, 1 if otherwise.
 */
int
pickup_object(obj, count, telekinesis, do_auto_in_bag, obj_gone_ptr)
struct obj *obj;
int64_t count;
boolean telekinesis, do_auto_in_bag; /* not picking it up directly by hand */
uchar* obj_gone_ptr; /* 1 = merged, 2 = put in bag, 3 = gone */
{
    int res, nearload;

    if (obj->quan < count) 
    {
        impossible("pickup_object: count %ld > quan %ld?", count, obj->quan);
        return 0;
    }

    /* In case of auto-pickup, where we haven't had a chance
       to look at it yet; affects docall(SCR_SCARE_MONSTER). */
    if (!Blind)
        obj->dknown = 1;

    if (obj == uchain)
    { /* do not pick up attached chain */
        return 0;
    }
    else if (obj->oartifact && !touch_artifact(obj, &youmonst)) 
    {
        return 0;
    }
    else if (obj->otyp == CORPSE) 
    {
        if (fatal_corpse_mistake(obj, telekinesis)
            || rider_corpse_revival(obj, telekinesis))
            return -1;
    } 
    else if (obj->otyp == SCR_SCARE_MONSTER) 
    {
        if (obj->blessed)
            obj->blessed = 0;
        else if (obj->special_quality == 0 && !obj->cursed)
            obj->special_quality = SPEQUAL_WILL_TURN_TO_DUST_ON_PICKUP;
        else 
        {
            char dcbuf[BUFSZ] = "";
            play_sfx_sound(SFX_ITEM_CRUMBLES_TO_DUST);
            Sprintf(dcbuf, "The scroll%s %s to dust as you %s %s up.", plur(obj->quan),
                      otense(obj, "turn"), telekinesis ? "raise" : "pick",
                      (obj->quan == 1L) ? "it" : "them");

            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, dcbuf);

            if (!(objects[SCR_SCARE_MONSTER].oc_name_known)
                && !(objects[SCR_SCARE_MONSTER].oc_uname))
                docall(obj, dcbuf);
            useupf(obj, obj->quan);
            return 1; /* tried to pick something up and failed, but
                         don't want to terminate pickup loop yet   */
        }
    }

    if ((res = lift_object(obj, (struct obj *) 0, &count, telekinesis)) <= 0)
        return res;

    /* Whats left of the special case for gold :-) */
    if (obj->oclass == COIN_CLASS)
        context.botl = 1;
    if (obj->quan != count && !(objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED))
        obj = splitobj(obj, count);

    /* Finally, pick the object up */
    struct obj* oldobj = obj;
    obj = pick_obj(obj);
    if(obj_gone_ptr && obj != oldobj) /* merged */
        *obj_gone_ptr = 1;

    if (uwep && uwep == obj)
        mrg_to_wielded = TRUE;
    nearload = near_capacity();

    /* Display message regarding a new item in inventory */
    prinv(nearload == SLT_ENCUMBER ? moderateloadmsg : (char*)0, obj, count);
    mrg_to_wielded = FALSE;

    if (do_auto_in_bag && obj && obj == oldobj)
    {
        int bagres = auto_bag_in(invent, obj, FALSE);
        if (bagres && obj_gone_ptr)
            *obj_gone_ptr = 2;
    }
    return 1;
}

int
auto_bag_in(objchn_container, obj, bynexthere)
struct obj* objchn_container, * obj;
boolean bynexthere;
{
    if (!objchn_container || !obj || Is_container(obj) || unfit_for_container(obj))
        return 0;

    struct obj* curr;
    struct obj* bag_of_holding = 0;
    struct obj* bag_of_the_glutton = 0;
    struct obj* bag_of_treasure_hauling = 0;
    struct obj* bag_of_wizardry = 0;
    struct obj* normal_bag = 0;

    for (curr = objchn_container; curr; curr = (bynexthere ? curr->nexthere : curr->nobj))
    {
        if (objects[curr->otyp].oc_name_known && curr != obj && !curr->olocked)
        {
            if (curr->otyp == BAG_OF_HOLDING && curr->bknown && !curr->cursed && obj->oclass != WAND_CLASS)
            {
                if (!bag_of_holding || (curr->blessed && !bag_of_holding->blessed))
                    bag_of_holding = curr;
            }
            else if (curr->otyp == BAG_OF_THE_GLUTTON && curr->bknown && !curr->cursed)
            {
                if (!bag_of_the_glutton || (curr->blessed && !bag_of_the_glutton->blessed))
                    bag_of_the_glutton = curr;
            }
            else if (curr->otyp == BAG_OF_TREASURE_HAULING && curr->bknown && !curr->cursed)
            {
                if (!bag_of_treasure_hauling || (curr->blessed && !bag_of_treasure_hauling->blessed))
                    bag_of_treasure_hauling = curr;
            }
            else if (curr->otyp == BAG_OF_WIZARDRY && curr->bknown && !curr->cursed)
            {
                if (!bag_of_wizardry || (curr->blessed && !bag_of_wizardry->blessed))
                    bag_of_wizardry = curr;
            }
            else if (Is_proper_container(curr) && !Is_mbag(curr) 
                && (objects[curr->otyp].oc_flags4 & (O4_CONTAINER_ACCEPTS_ONLY_SCROLLS_AND_BOOKS | O4_CONTAINER_ACCEPTS_ONLY_WEAPONS)) == 0 
                && !((objects[curr->otyp].oc_flags4 & (O4_CONTAINER_HAS_LID)) && !(curr->speflags & (SPEFLAGS_LID_OPENED))))
            {
                if (!normal_bag)
                    normal_bag = curr;
            }
        }
    }

    int num_choices = 0;
    if (bag_of_holding)
        num_choices++;
    if (bag_of_the_glutton)
        num_choices++;
    if (bag_of_treasure_hauling)
        num_choices++;
    if (bag_of_wizardry)
        num_choices++;
    if (normal_bag)
        num_choices++;

    if (!num_choices)
        return 0;

    boolean maybe_cancellation = (objects[obj->otyp].oc_name_known || objects[WAN_CANCELLATION].oc_name_known ? (objects[obj->otyp].oc_flags5 & O5_MBAG_DESTROYING_ITEM) != 0 : obj->oclass == WAND_CLASS);
    switch (flags.auto_bag_in_style)
    {
    case 0:
    default:
    {
        struct obj* used_container = 0;
        if (!used_container && bag_of_treasure_hauling && is_obj_weight_reduced_by_treasure_hauling(obj) && !maybe_cancellation)
            used_container = bag_of_treasure_hauling;
        if (!used_container && bag_of_wizardry && is_obj_weight_reduced_by_wizardry(obj) && !maybe_cancellation)
            used_container = bag_of_wizardry;
        if (!used_container && bag_of_the_glutton && is_obj_weight_reduced_by_the_glutton(obj) && !maybe_cancellation)
            used_container = bag_of_the_glutton;
        if (!used_container && bag_of_holding && !maybe_cancellation)
            used_container = bag_of_holding;
        if (!used_container && normal_bag)
            used_container = normal_bag;
        if (!used_container && bag_of_treasure_hauling && !maybe_cancellation)
            used_container = bag_of_treasure_hauling;
        if (!used_container && bag_of_wizardry && !maybe_cancellation)
            used_container = bag_of_wizardry;
        if (!used_container && bag_of_the_glutton && !maybe_cancellation)
            used_container = bag_of_the_glutton;
        if (used_container)
            return stash_obj_in_container(obj, used_container);
        break;
    }
    }
    return 0;
}

int
count_bags_for_stashing(objchn_container, objchn, bynexthere_container, bynexthere_obj)
struct obj* objchn_container, *objchn;
boolean bynexthere_container, bynexthere_obj;
{
    if (!can_stash_objs())
        return 0;

    struct obj* curr;
    int fitcnt = 0;
    int no_cancellation_cnt = 0;
    for (curr = objchn; curr; curr = (bynexthere_obj ? curr->nexthere : curr->nobj))
    {
        if (!Is_container(curr) && !unfit_for_container(curr))
        {
            fitcnt++;
            if (!(objects[curr->otyp].oc_name_known ? (objects[curr->otyp].oc_flags5 & O5_MBAG_DESTROYING_ITEM) != 0 : curr->oclass == WAND_CLASS))
                no_cancellation_cnt++;
        }
    }

    if(!fitcnt)
        return 0;

    int cnt = 0;
    for (curr = objchn_container; curr; curr = (bynexthere_container ? curr->nexthere : curr->nobj))
    {
        if (objects[curr->otyp].oc_name_known)
        {
            if (curr->otyp == BAG_OF_HOLDING && curr->bknown && !curr->cursed && no_cancellation_cnt > 0)
            {
                cnt++;
            }
            else if (curr->otyp == BAG_OF_THE_GLUTTON && curr->bknown && !curr->cursed && no_cancellation_cnt > 0)
            {
                cnt++;
            }
            else if (curr->otyp == BAG_OF_TREASURE_HAULING && curr->bknown && !curr->cursed && no_cancellation_cnt > 0)
            {
                cnt++;
            }
            else if (curr->otyp == BAG_OF_WIZARDRY && curr->bknown && !curr->cursed && no_cancellation_cnt > 0)
            {
                cnt++;
            }
            else if (Is_proper_container(curr) && !Is_mbag(curr) && (objects[curr->otyp].oc_flags4 & (O4_CONTAINER_ACCEPTS_ONLY_SCROLLS_AND_BOOKS | O4_CONTAINER_ACCEPTS_ONLY_WEAPONS)) == 0)
            {
                cnt++;
            }
        }
    }
    return cnt;
}

/*
 * Do the actual work of picking otmp from the floor or monster's interior
 * and putting it in the hero's inventory.  Take care of billing.  Return a
 * pointer to the object where otmp ends up.  This may be different
 * from otmp because of merging.
 */
struct obj *
pick_obj(otmp)
struct obj *otmp;
{
    struct obj *result;
    int ox = otmp->ox, oy = otmp->oy;
    boolean robshop = (!u.uswallow && otmp != uball && costly_spot(ox, oy));

    /* Play first so the location is still available */
    if (iflags.using_gui_sounds)
    {
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_PICK_UP);
        delay_output_milliseconds(ITEM_PICKUP_DROP_DELAY);
    }

    Strcpy(debug_buf_2, "pick_obj");
    obj_extract_self(otmp);
    newsym(ox, oy);

    /* for shop items, addinv() needs to be after addtobill() (so that
       object merger can take otmp->unpaid into account) but before
       remote_robbery() (which calls rob_shop() which calls setpaid()
       after moving costs of unpaid items to shop debt; setpaid()
       calls clear_unpaid() for lots of object chains, but 'otmp' isn't
       on any of those between obj_extract_self() and addinv(); for
       3.6.0, 'otmp' remained flagged as an unpaid item in inventory
       and triggered impossible() every time inventory was examined) */
    if (robshop) {
        char saveushops[5], fakeshop[2];

        /* addtobill cares about your location rather than the object's;
           usually they'll be the same, but not when using telekinesis
           (if ever implemented) or a grappling hook */
        Strcpy(saveushops, u.ushops);
        fakeshop[0] = *in_rooms(ox, oy, SHOPBASE);
        fakeshop[1] = '\0';
        Strcpy(u.ushops, fakeshop);
        /* sets obj->unpaid if necessary */
        addtobill(otmp, TRUE, FALSE, FALSE);
        Strcpy(u.ushops, saveushops);
        robshop = otmp->unpaid && !index(u.ushops, *fakeshop);
    }

    result = addinv(otmp);

    /* if you're taking a shop item from outside the shop, make shk notice */
    if (robshop)
        remote_burglary(ox, oy);

    return result;
}

/*
 * prints a message if encumbrance changed since the last check and
 * returns the new encumbrance value (from near_capacity()).
 */
int
encumber_msg()
{
    int oldcap = u.carrying_capacity_level;
    int newcap = near_capacity();

    if (oldcap < newcap) {
        switch (newcap) {
        case 1:
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "movements are slowed slightly because of your load.");
            break;
        case 2:
            You_ex(ATR_NONE, CLR_MSG_WARNING, "rebalance your load.  Movement is difficult.");
            break;
        case 3:
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s under your heavy load.  Movement is very hard.",
                stagger(youmonst.data, "stagger"));
            break;
        default:
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s move a handspan with this load!",
                newcap == 4 ? "can barely" : "can't even");
            break;
        }
        context.botl = context.botlx = TRUE;
        standard_hint("Being burdened reduces your speed and thus your combat power. Try to remain unburdened at all times, if possible. Some magical bags reduce weight of items. Put items into such bags if you have them.", &u.uhint.got_burdened);
    } else if (oldcap > newcap) {
        switch (newcap) {
        case 0:
            Your_ex(ATR_NONE, CLR_MSG_POSITIVE, "movements are now unencumbered.");
            break;
        case 1:
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "movements are only slowed slightly by your load.");
            break;
        case 2:
            You_ex(ATR_NONE, CLR_MSG_WARNING, "rebalance your load.  Movement is still difficult.");
            break;
        case 3:
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s under your load.  Movement is still very hard.",
                stagger(youmonst.data, "stagger"));
            break;
        }
        context.botl = context.botlx = TRUE;
    }

    u.carrying_capacity_level = newcap;

    if(newcap != oldcap)
        refresh_u_tile_gui_info(TRUE);

    return newcap;
}

/* Is there a container at x,y. Optional: return count of containers at x,y */
int
container_at(x, y, countem)
int x, y;
boolean countem;
{
    struct obj *cobj, *nobj;
    int container_count = 0;

    for (cobj = level.objects[x][y]; cobj; cobj = nobj) {
        nobj = cobj->nexthere;
        if (Is_container(cobj)) {
            container_count++;
            if (!countem)
                break;
        }
    }
    return container_count;
}

STATIC_OVL boolean
able_to_loot(x, y, looting)
int x, y;
boolean looting; /* loot vs tip */
{
    const char *verb = looting ? "loot" : "tip";

    if (!can_reach_floor(TRUE)) {
        if (u.usteed && P_SKILL_LEVEL(P_RIDING) < P_BASIC)
            rider_cant_reach(); /* not skilled enough to reach */
        else
            cant_reach_floor(x, y, FALSE, TRUE);
        return FALSE;
    } else if ((is_pool(x, y) && (looting || !Underwater)) || is_lava(x, y)) {
        /* at present, can't loot in water even when Underwater;
           can tip underwater, but not when over--or stuck in--lava */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot %s things that are deep in the %s.", verb,
            hliquid(is_lava(x, y) ? "lava" : "water"));
        return FALSE;
    } 
#if 0
    else if (nolimbs(youmonst.data)) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        pline("Without limbs, you cannot %s anything.", verb);
        return FALSE;
    } 
    else if (looting && !freehand()) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline("Without a free %s, you cannot loot anything.",
              body_part(HAND));
        return FALSE;
    }
#endif
    return TRUE;
}

#if 0
STATIC_OVL boolean
mon_beside(x, y)
int x, y;
{
    int i, j, nx, ny;

    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++) {
            nx = x + i;
            ny = y + j;
            if (isok(nx, ny) && MON_AT(nx, ny))
                return TRUE;
        }
    return FALSE;
}
#endif

int
do_loot_cont(cobjp, cindex, ccount)
struct obj **cobjp;
int cindex, ccount; /* index of this container (1..N), number of them (N) */
{
    if (!cobjp)
        return 0;

    struct obj *cobj = *cobjp;

    if (!cobj)
        return 0;

    if (cobj->olocked)
    {
        play_simple_container_sound(cobj, CONTAINER_SOUND_TYPE_TRY_LOCKED);
        char lbuf[BUFSZ];
        Strcpy(lbuf, "");
        print_lock_with_buf(lbuf, cobj->keyotyp, cobj->special_quality, FALSE);

        if (ccount < 2)
            pline("%s locked%s.",
                  cobj->lknown ? "It is" : "Hmmm, it turns out to be", lbuf);
        else if (cobj->lknown)
            pline("%s is locked%s.", The(xname(cobj)), lbuf);
        else
            pline("Hmmm, %s turns out to be locked%s.", the(xname(cobj)), lbuf);

        cobj->lknown = 1;

        if (flags.autounlock
            && cobj->where == OBJ_FLOOR && cobj->ox == u.ux && cobj->oy == u.uy)
        {
            struct obj* carried_key = 0;
            if ((carried_key = carrying_fitting_unlocking_tool_for_box(cobj)) != 0)
            {
                if (carried_key)
                {
                    int pick_res = pick_lock_core(carried_key, u.ux, u.uy, TRUE);
                    if (pick_res == PICKLOCK_DID_SOMETHING)
                        return 1;
                    else if (pick_res == PICKLOCK_LEARNED_SOMETHING)
                        return 1;
                }
            }
        }

        return 0;
    }

    cobj->lknown = 1;
#ifdef ANDROID
    if (flags.autokick && can_try_force())
        doforce();
#endif

    if (cobj->otyp == BAG_OF_TRICKS) {
        play_sfx_sound(SFX_SURPRISE_ATTACK);
        You("carefully open %s...", the(xname(cobj)));
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "It develops a huge set of teeth and bites you!");
        losehp(adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "carnivorous bag", KILLED_BY_AN);
        makeknown(BAG_OF_TRICKS);
        abort_looting = TRUE;
        return 1;
    }
    else if (cobj->otyp == POUCH_OF_ENDLESS_BOLTS || cobj->otyp == QUIVER_OF_INFINITE_ARROWS || cobj->otyp == BAG_OF_INFINITE_SLING_BULLETS) {
        You("carefully open %s...", the(xname(cobj)));
        if(cobj->cooldownleft > 0)
            You("find nothing but void inside.");
        else
            (void)endlessarrows(cobj, (cobj->otyp == BAG_OF_INFINITE_SLING_BULLETS ? SLING_BULLET : cobj->otyp == POUCH_OF_ENDLESS_BOLTS ? CROSSBOW_BOLT : ARROW), rnd(10) + 10);
        abort_looting = TRUE;
        return 1;
    }

    boolean lidopened = FALSE;
    if ((objects[cobj->otyp].oc_flags4 & O4_CONTAINER_HAS_LID) && !(cobj->speflags & SPEFLAGS_LID_OPENED))
    {
        lidopened = TRUE;
        cobj->speflags |= SPEFLAGS_LID_OPENED;
        newsym(u.ux, u.uy);
        play_simple_container_sound(cobj, CONTAINER_SOUND_TYPE_OPEN);
        You("carefully push the lid of %s aside...", the(xname(cobj)));
        flush_screen(1);
    }

    if(!(objects[cobj->otyp].oc_flags4 & O4_CONTAINER_CONTENTS_VISIBLE) && !lidopened)
        You("%s%s%s...", (!cobj->cknown || !cobj->lknown) ? "carefully " : "", (objects[cobj->otyp].oc_flags4 & O4_CONTAINER_PEEK_INTO) ? "peek into " : "open ",
            the(xname(cobj)));

    if ((objects[cobj->otyp].oc_flags4 & O4_CONTAINER_MAY_CONTAIN_MONSTER))
    {
        if (maybe_disturb_container_monster(cobj))
        {
            abort_looting = TRUE;
            return 1;
        }
    }

    return use_container(cobjp, 0, (boolean) (cindex < ccount));
}

boolean
maybe_disturb_container_monster(cobj)
struct obj* cobj;
{
    if (!cobj)
        return FALSE;

    xchar x = 0, y = 0;
    get_obj_location(cobj, &x, &y, 0);

    if (isok(x, y) && (objects[cobj->otyp].oc_flags4 & O4_CONTAINER_MAY_CONTAIN_MONSTER) && (has_omonst(cobj) || cobj->corpsenm > NON_PM))
    {
        struct monst* mtmp = (struct monst*)0;
        if (has_omonst(cobj))
        {
            coord xy = { x, y };
            mtmp = montraits(cobj, &xy, FALSE, NON_PM, NON_PM, 0UL);
            if (mtmp)
            {
                free_omonst(cobj);
                set_corpsenm(cobj, NON_PM); /* It's gone */
            }
        }
        
        if(!mtmp && cobj->corpsenm >= LOW_PM)
        {
            mtmp = makemon(&mons[cobj->corpsenm], x, y, MM_ANGRY | MM_ADJACENTOK);
            set_corpsenm(cobj, NON_PM); /* It's gone */
        }

        cobj->owt = weight(cobj);

        if (mtmp)
        {
            boolean lid_opened = FALSE;
            /* Monster opens the lid first */
            if (objects[cobj->otyp].oc_flags4 & O4_CONTAINER_HAS_LID)
            {
                if (!(cobj->speflags & SPEFLAGS_LID_OPENED))
                {
                    lid_opened = TRUE;
                    play_simple_container_sound(cobj, CONTAINER_SOUND_TYPE_OPEN);
                    cobj->speflags |= SPEFLAGS_LID_OPENED;
                    newsym(x, y);
                }
            }

            if (iflags.using_gui_sounds)
                delay_output_milliseconds(300);
            play_sfx_sound(SFX_SURPRISE_ATTACK);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Disturbed, %s %srises from the %s!", a_monnam(mtmp), lid_opened ? "opens the lid and " : "", cxname(cobj));
            flush_screen(1);
            if (iflags.using_gui_sounds)
                delay_output_milliseconds(300);
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_CREATION);
            return TRUE;
        }
    }


    return FALSE;

}

STATIC_OVL void
loot_decoration(x, y, itemnumber, got_something_ptr)
int x, y, itemnumber;
boolean* got_something_ptr;
{
    if (itemnumber == 1)
    {
        boolean itemlit = FALSE;
        if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0)
        {
            if (levl[x][y].lamplit)
            {
                itemlit = TRUE;
                del_light_source(LS_LOCATION, xy_to_any(x, y));
                levl[x][y].lamplit = 0;
            }
            if (levl[x][y].makingsound)
            {
                del_sound_source(SOUNDSOURCE_LOCATION, xy_to_any(x, y));
                levl[x][y].makingsound = 0;
            }
        }
        if (decoration_type_definitions[levl[x][y].decoration_typ].lootable_item != STRANGE_OBJECT)
        {
            levl[x][y].decoration_flags &= ~DECORATION_FLAGS_ITEM_IN_HOLDER;
            boolean subtyp_is_item_special_quality = (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_SUBTYP_IS_OBJ_SPECIAL_QUALITY) != 0;
            boolean item_is_statue = decoration_type_definitions[levl[x][y].decoration_typ].lootable_item == STATUE;
            int mnum = decoration_type_definitions[levl[x][y].decoration_typ].mnum;
            struct obj* newobj = mksobj_with_flags(decoration_type_definitions[levl[x][y].decoration_typ].lootable_item, TRUE, FALSE, MKOBJ_TYPE_GENERATED, (struct monst*)0, MAT_NONE, item_is_statue ? mnum : subtyp_is_item_special_quality ? (int64_t)levl[x][y].decoration_subtyp : 0L, 0L, item_is_statue ? MKOBJ_FLAGS_PARAM_IS_MNUM : subtyp_is_item_special_quality ? MKOBJ_FLAGS_PARAM_IS_SPECIAL_QUALITY : 0UL);
            if (newobj)
            {
                *got_something_ptr = TRUE;
                place_object(newobj, u.ux, u.uy);
                //Light it up
                if (itemlit && !newobj->lamplit)
                {
                    begin_burn(newobj, FALSE);
                }
                if (costly_spot(u.ux, u.uy))
                {
                    char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
                    struct monst* shkp = shop_keeper(*o_shop);
                    if (shkp && inhishop(shkp))
                    {
                        add_one_tobill(newobj, FALSE, shkp);
                    }
                }
                play_simple_object_sound(newobj, OBJECT_SOUND_TYPE_PICK_UP);
                Strcpy(debug_buf_2, "loot_decoration");
                obj_extract_self(newobj);
                newobj = hold_another_object(newobj, "Oops!  %s out of your grasp!",
                    The(aobjnam(newobj, "slip")), (const char*)0);
            }
        }
    }
    else
    {
        int lootable_item = itemnumber == 2 ? decoration_type_definitions[levl[x][y].decoration_typ].lootable_item2 : decoration_type_definitions[levl[x][y].decoration_typ].lootable_item3;
        if (lootable_item != STRANGE_OBJECT)
        {
            struct obj* newobj = mksobj_with_flags(lootable_item, TRUE, FALSE, MKOBJ_TYPE_GENERATED, (struct monst*)0, MAT_NONE, 0L, 0L, 0UL);
            if (newobj)
            {
                if(itemnumber == 2)
                    levl[x][y].decoration_flags &= ~DECORATION_FLAGS_ITEM2_IN_HOLDER;
                else
                    levl[x][y].decoration_flags &= ~DECORATION_FLAGS_ITEM3_IN_HOLDER;

                *got_something_ptr = TRUE;
                play_simple_object_sound_at_location(newobj, u.ux, u.uy, OBJECT_SOUND_TYPE_PICK_UP);
                newobj = hold_another_object(newobj, "Oops!  %s out of your grasp!",
                    The(aobjnam(newobj, "slip")), (const char*)0);
            }
        }
    }
}

/* loot a container on the floor or loot saddle from mon. */
int
doloot()
{
    struct obj *cobj, *nobj;
    register int c = -1;
    int timepassed = 0;
    coord cc;
    boolean underfoot = TRUE;
    const char *dont_find_anything = "don't find anything";
    struct monst *mtmp;
    char qbuf[BUFSZ];
    int prev_inquiry = 0;
    boolean prev_loot = FALSE;
    int num_conts = 0;
    boolean did_something = FALSE;
    boolean got_something = FALSE;

    abort_looting = FALSE;

    if (check_capacity((char *) 0)) {
        /* "Can't do that while carrying so much stuff." */
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        return 0;
    }
#if 0
    if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You("have no hands!"); /* not `body_part(HAND)' */
        return 0;
    }
#endif
    if (Confusion) {
        if (rn2(6) && reverse_loot())
            return 1;
        if (rn2(2)) {
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Being confused, you find nothing to loot.");
            return 1; /* costs a turn */
        }             /* else fallthrough to normal looting */
    }
    cc.x = u.ux;
    cc.y = u.uy;

    if (iflags.menu_requested)
        goto lootmon;

 lootcont:
    if ((num_conts = container_at(cc.x, cc.y, TRUE)) > 0) {
        boolean anyfound = FALSE;

        if (!able_to_loot(cc.x, cc.y, TRUE))
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            return 0;
        }

        if (Blind && !uarmg) {
            /* if blind and without gloves, attempting to #loot at the
               location of a cockatrice corpse is fatal before asking
               whether to manipulate any containers */
            for (nobj = sobj_at(CORPSE, cc.x, cc.y); nobj;
                 nobj = nxtobj(nobj, CORPSE, TRUE))
                if (will_feel_cockatrice(nobj, FALSE)) {
                    feel_cockatrice(nobj, FALSE);
                    /* if life-saved (or poly'd into stone golem),
                       terminate attempt to loot */
                    return 1;
                }
        }

        if (num_conts > 1) {
            /* use a menu to loot many containers */
            int n, i;
            winid win;
            anything any;
            menu_item *pick_list = (menu_item *) 0;

            any.a_void = 0;
            win = create_nhwindow(NHW_MENU);
            start_menu_ex(win, GHMENU_STYLE_PICK_ITEM_LIST);

            for (cobj = level.objects[cc.x][cc.y]; cobj;
                 cobj = cobj->nexthere)
                if (Is_container(cobj)) {
                    any.a_obj = cobj;
                    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                             doname(cobj), MENU_UNSELECTED);
                }
            end_menu(win, "Loot which containers?");
            n = select_menu(win, PICK_ANY, &pick_list);
            destroy_nhwindow(win);

            if (n > 0) {
                for (i = 1; i <= n; i++) {
                    cobj = pick_list[i - 1].item.a_obj;
                    timepassed |= do_loot_cont(&cobj, i, n);
                    if (abort_looting) {
                        /* chest trap or magic bag explosion or <esc> */
                        free((genericptr_t) pick_list);
                        return timepassed;
                    }
                }
                free((genericptr_t) pick_list);
            }
            if (n != 0)
                c = 'y';
        } else {
            for (cobj = level.objects[cc.x][cc.y]; cobj; cobj = nobj) {
                nobj = cobj->nexthere;

                if (Is_container(cobj)) {
                    c = ynq(safe_qbuf(qbuf, "There is ", " here, loot it?",
                                      cobj, doname, ansimpleoname,
                                      "a container"));
                    if (c == 'q')
                        return timepassed;
                    if (c == 'n')
                        continue;
                    anyfound = TRUE;

                    timepassed |= do_loot_cont(&cobj, 1, 1);
                    if (abort_looting)
                        /* chest trap or magic bag explosion or <esc> */
                        return timepassed;
                }
            }
            if (anyfound)
                c = 'y';
        }
    } else if (IS_GRAVE(levl[cc.x][cc.y].typ)) {
        play_sfx_sound(SFX_GENERAL_ANOTHER_ACTION_NEEDED);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "need to dig up the grave to effectively loot it...");
    }

    /*
     * 3.3.1 introduced directional looting for some things.
     */
 lootmon:
    if (c != 'y' /* && mon_beside(u.ux, u.uy) */)
    {
        if (!get_adjacent_loc("Loot in what direction?",
            "Invalid loot location", u.ux, u.uy, &cc))
        {
            return 0;
        }

        if (cc.x == u.ux && cc.y == u.uy) 
        {
            underfoot = TRUE;
            if (container_at(cc.x, cc.y, FALSE))
            {
                did_something = TRUE;
                goto lootcont;
            }
        } 
        else
            underfoot = FALSE;
        
        if (u.dz < 0) 
        {
            did_something = TRUE;
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "%s to loot on the %s.", dont_find_anything,
                ceiling(cc.x, cc.y));
            timepassed = 1;
            return timepassed;
        }

        mtmp = m_at(cc.x, cc.y);
        if (mtmp)
        {
            timepassed = loot_mon(mtmp, &prev_inquiry, &prev_loot, FALSE);
            if(timepassed)
                did_something = TRUE;
        }

        /* always use a turn when choosing a direction is impaired,
           even if you've successfully targetted a saddled creature
           and then answered "no" to the "remove its saddle?" prompt */
        if (Confusion || Stunned)
            timepassed = 1;

        if (levl[cc.x][cc.y].decoration_typ > 0 && (decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE) != 0)
        {
            boolean is_lootable_dir = FALSE;
            switch (levl[cc.x][cc.y].decoration_dir)
            {
            case 0:
                is_lootable_dir = u.uy > cc.y;
                break;
            case 1:
                is_lootable_dir = u.ux > cc.x;
                break;
            case 2:
                is_lootable_dir = u.ux < cc.x;
                break;
            case 3:
                is_lootable_dir = u.uy < cc.y;
                break;
            default:
                break;
            }
            if (is_lootable_dir)
            {
                boolean has_item1 = decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].lootable_item != STRANGE_OBJECT && (levl[cc.x][cc.y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0;
                boolean has_item2 = decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].lootable_item2 != STRANGE_OBJECT && (levl[cc.x][cc.y].decoration_flags & DECORATION_FLAGS_ITEM2_IN_HOLDER) != 0;
                boolean has_item3 = decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].lootable_item3 != STRANGE_OBJECT && (levl[cc.x][cc.y].decoration_flags & DECORATION_FLAGS_ITEM3_IN_HOLDER) != 0;

                if (has_item1 || has_item2 || has_item3)
                {
                    int selected_item = 0;
                    if (has_item1 + has_item2 + has_item3 == 1)
                    {
                        if (has_item1)
                            selected_item = 1;
                        else if (has_item2)
                            selected_item = 2;
                        else if (has_item3)
                            selected_item = 3;

                        did_something = TRUE;
                        loot_decoration(cc.x, cc.y, selected_item, &got_something);
                    }
                    else
                    {
                        menu_item* pick_list = (menu_item*)0;
                        winid win;
                        anything any;

                        any = zeroany;
                        win = create_nhwindow(NHW_MENU);
                        start_menu_ex(win, GHMENU_STYLE_OTHERS_INVENTORY);

                        int item_count = 0;
                        if (has_item1)
                        {
                            any.a_int = 1;
                            char itembuf[BUFSZ * 2] = "";
                            int item_otyp = decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].lootable_item;
                            Sprintf(itembuf, "%s", an(OBJ_NAME(objects[item_otyp])));

                            add_menu(win, NO_GLYPH, &any,
                                0, 0, ATR_NONE, NO_COLOR,
                                itembuf, MENU_UNSELECTED);

                            item_count++;
                        }
                        if (has_item2)
                        {
                            any.a_int = 2;
                            char itembuf[BUFSZ * 2] = "";
                            int item_otyp = decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].lootable_item2;
                            Sprintf(itembuf, "%s", an(OBJ_NAME(objects[item_otyp])));

                            add_menu(win, NO_GLYPH, &any,
                                0, 0, ATR_NONE, NO_COLOR,
                                itembuf, MENU_UNSELECTED);

                            item_count++;
                        }
                        if (has_item3)
                        {
                            any.a_int = 3;
                            char itembuf[BUFSZ * 2] = "";
                            int item_otyp = decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].lootable_item3;
                            Sprintf(itembuf, "%s", an(OBJ_NAME(objects[item_otyp])));

                            add_menu(win, NO_GLYPH, &any,
                                0, 0, ATR_NONE, NO_COLOR,
                                itembuf, MENU_UNSELECTED);

                            item_count++;
                        }

                        /* Finish the menu */
                        end_menu(win, "What do you want to loot?");

                        if (item_count <= 0)
                        {
                            pline1("There's nothing to loot.");
                            destroy_nhwindow(win);
                            return timepassed;
                        }

                        /* Now generate the menu */
                        int pick_count = select_menu(win, PICK_ANY, &pick_list);
                        if (pick_count > 0)
                        {
                            int i;
                            for (i = 0; i < pick_count; i++)
                            {
                                if (pick_list[i].item.a_int > 0)
                                {
                                    selected_item = pick_list[i].item.a_int;
                                    did_something = TRUE;
                                    loot_decoration(cc.x, cc.y, selected_item, &got_something);
                                }
                            }
                            free((genericptr_t)pick_list);
                            destroy_nhwindow(win);
                        }
                        else
                        {
                            pline1(Never_mind);
                            destroy_nhwindow(win);
                            return timepassed;
                        }
                    }

                    newsym(cc.x, cc.y);
                    newsym(u.ux, u.uy);
                    flush_screen(1);
                    vision_full_recalc = 1;
                }
                else
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is empty.", The(decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].description));
                    return timepassed;
                }
            }
            else
            {
                play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
                You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "loot anything there from this direction.");
                return timepassed;
            }
        }

        /* Preserve pre-3.3.1 behaviour for containers.
         * Adjust this if-block to allow container looting
         * from one square away to change that in the future.
         */
        if (!underfoot && !got_something && !did_something) 
        {
            did_something = TRUE;
            if (container_at(cc.x, cc.y, FALSE))
            {
                if (mtmp)
                {
                    play_sfx_sound(SFX_SOMETHING_IN_WAY);
                    You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "loot anything %sthere with %s in the way.",
                             prev_inquiry ? "else " : "", mon_nam(mtmp));
                    return timepassed;
                } 
                else
                {
                    play_sfx_sound(SFX_GENERAL_NOT_AT_RIGHT_LOCATION);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "have to be at a container to loot it.");
                }
            } 
            else
            {
                play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "%s %sthere to loot.", dont_find_anything,
                    (prev_inquiry || prev_loot) ? "else " : "");
                return timepassed;
            }
        }

        if (!did_something)
        {
            You_ex(ATR_NONE, CLR_MSG_FAIL, "%s %sthere to loot.", dont_find_anything,
                (prev_inquiry || prev_loot) ? "else " : "");
            play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        }
    }
    else if (c != 'y' && c != 'n')
    {
        play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s to loot.", dont_find_anything,
            underfoot ? "here" : "there");
    }
    return timepassed;
}

/* called when attempting to #loot while confused */
STATIC_OVL boolean
reverse_loot()
{
    struct obj *goldob = 0, *coffers, *otmp, boxdummy;
    struct monst *mon;
    int64_t contribution;
    int n, x = u.ux, y = u.uy;

    if (!rn2(3)) {
        /* n objects: 1/(n+1) chance per object plus 1/(n+1) to fall off end
         */
        for (n = inv_cnt(TRUE), otmp = invent; otmp; --n, otmp = otmp->nobj)
            if (!rn2(n + 1)) {
                prinv("You find old loot:", otmp, 0L);
                return TRUE;
            }
        return FALSE;
    }

    /* find a money object to mess with */
    for (goldob = invent; goldob; goldob = goldob->nobj)
        if (goldob->oclass == COIN_CLASS) {
            contribution = ((int64_t) rnd(5) * goldob->quan + 4L) / 5L;
            if (contribution < goldob->quan)
                goldob = splitobj(goldob, contribution);
            break;
        }
    if (!goldob)
        return FALSE;

    if (!IS_THRONE(levl[x][y].typ)) {
        dropx(goldob);
        /* the dropped gold might have fallen to lower level */
        if (g_at(x, y))
            pline("Ok, now there is loot here.");
    } else {
        /* find original coffers chest if present, otherwise use nearest one
         */
        otmp = 0;
        for (coffers = fobj; coffers; coffers = coffers->nobj)
            if (coffers->otyp == CHEST) {
                if (coffers->speflags & SPEFLAGS_ROYAL_COFFER)
                    break; /* a throne room chest */
                if (!otmp
                    || distu(coffers->ox, coffers->oy)
                           < distu(otmp->ox, otmp->oy))
                    otmp = coffers; /* remember closest ordinary chest */
            }
        if (!coffers)
            coffers = otmp;

        if (coffers) {
            play_sfx_sound(SFX_VOICE_THRONE_ROOM_THANK_YOU_FOR_YOUR_CONTRIBUTION);
            verbalize_talk1("Thank you for your contribution to reduce the debt.");
            freeinv(goldob);
            (void) add_to_container(coffers, goldob);
            coffers->owt = weight(coffers);
            coffers->cknown = 0;
            if (!coffers->olocked) {
                boxdummy = zeroobj, boxdummy.otyp = SPE_WIZARD_LOCK;
                (void) boxlock(coffers, &boxdummy);
            }
        } else if (levl[x][y].looted != T_LOOTED
                   && (mon = makemon(courtmon(), x, y, NO_MM_FLAGS)) != 0) {
            freeinv(goldob);
            add_to_minv(mon, goldob);
            pline("The exchequer accepts your contribution.");
            if (!rn2(10))
                levl[x][y].looted = T_LOOTED;
        } else {
            You("drop %s.", doname(goldob));
            dropx(goldob);
        }
    }
    return TRUE;
}

/* loot_mon() returns amount of time passed.
 */
int
loot_mon(mtmp, passed_info, prev_loot, do_auto_in_bag)
struct monst *mtmp;
int *passed_info;
boolean *prev_loot;
boolean do_auto_in_bag;
{
    int c = -1;
    int timepassed = 0;
    struct obj *otmp;
    char qbuf[QBUFSZ];

    /* 3.3.1 introduced the ability to remove saddle from a steed.
     *  *passed_info is set to TRUE if a loot query was given.
     *  *prev_loot is set to TRUE if something was actually acquired in here.
     */
    if (mtmp && mtmp != u.usteed && (otmp = which_armor(mtmp, W_SADDLE)))
    {
        int64_t unwornmask;

        if (passed_info)
            *passed_info = 1;

        Sprintf(qbuf, "Do you want to remove the saddle from %s?",
                x_monnam(mtmp, ARTICLE_THE, (char *) 0,
                         SUPPRESS_SADDLE, FALSE));

        if ((c = yn_function(qbuf, ynqchars, 'n', ynqdescs)) == 'y')
        {
            if (nolimbs(youmonst.data)) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "do that without limbs."); /* not body_part(HAND) */
                return 0;
            }
            if (otmp->cursed)
            {
                You_ex(ATR_NONE, CLR_MSG_FAIL, "can't.  The saddle seems to be stuck to %s.",
                    x_monnam(mtmp, ARTICLE_THE, (char *) 0,
                             SUPPRESS_SADDLE, FALSE));
                /* the attempt costs you time */
                return 1;
            }

            play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_APPLY);
            Strcpy(debug_buf_2, "loot_mon");
            obj_extract_self(otmp);

            if ((unwornmask = otmp->owornmask) != 0L)
            {
                mtmp->worn_item_flags &= ~unwornmask;
                otmp->owornmask = 0L;
                update_all_mon_statistics(mtmp, FALSE);
                if (mtmp == u.usteed && otmp->otyp == SADDLE)
                    dismount_steed(DISMOUNT_FELL);

            }
            otmp = hold_another_object(otmp, "You drop %s!", doname(otmp),
                                       (const char *) 0);
            nhUse(otmp);
            timepassed = rnd(3);
            if (prev_loot)
                *prev_loot = TRUE;
        } 
        else if (c == 'q') 
        {
            return 0;
        }
    }
    /* 3.4.0 introduced ability to pick things up from swallower's stomach */
    if (u.uswallow) 
    {
        int count = passed_info ? *passed_info : 0;
        timepassed = pickup(count, do_auto_in_bag);
    }
    return timepassed;
}

/*
 * Decide whether an object being placed into a magic bag will cause
 * it to explode.  If the object is a bag itself, check recursively.
 */
STATIC_OVL boolean
mbag_explodes(obj, container, depthin)
struct obj *obj, *container;
int depthin;
{
    /* these won't cause an explosion when they're empty */
    if (is_obj_mbag_destroying(obj) && obj->charges <= 0)
        return FALSE;

    if (is_obj_mbag_destroying(obj) && does_obj_drain_instead_of_explode(container) && obj->charges > 0)
    {
        //Bag of wizardry will drain the charges of explosing-causing items
        obj->charges = 0;
        return FALSE;
    }

    if (is_obj_indestructible(container) || container->oartifact > 0)
        return FALSE;

    /* odds: 1/1, 2/2, 3/4, 4/8, 5/16, 6/32, 7/64, 8/128, 9/128, 10/128,... */
    if ((Is_mbag(obj) || is_obj_mbag_destroying(obj))
        && (rn2(1 << (depthin > 7 ? 7 : depthin)) <= depthin))
    {
        return TRUE;
    }
    else if (Has_contents(obj)) 
    {
        struct obj *otmp;

        for (otmp = obj->cobj; otmp; otmp = otmp->nobj)
            if (mbag_explodes(otmp, obj, depthin + 1))
                return TRUE;
    }
    return FALSE;
}

STATIC_OVL int64_t
boh_loss(container, held)
struct obj *container;
int held;
{
    /* sometimes toss objects if a cursed magic bag */
    if (Is_mbag(container) && container->cursed && Has_contents(container))
    {
        int64_t loss = 0L;
        struct obj *curr, *otmp;

        for (curr = container->cobj; curr; curr = otmp) 
        {
            otmp = curr->nobj;
            if (!rn2(13)) 
            {
                Strcpy(debug_buf_2, "boh_loss");
                obj_extract_self(curr);
                loss += mbag_item_gone(held, curr);
            }
        }
        return loss;
    }
    return 0;
}

/* Returns: -1 to stop, 1 item was inserted, 0 item was not inserted. */
int
stash_obj_in_container(obj, container)
struct obj* obj, *container;
{
    struct obj* saved_container = current_container;
    current_container = container;
    int res = in_container(obj);
    current_container = saved_container;
    return res;
}

/* Returns: -1 to stop, 1 item was inserted, 0 item was not inserted. */
STATIC_PTR int
in_container(obj)
register struct obj* obj;
{
    return in_container_core(obj, TRUE);
}

STATIC_PTR int
in_container_nobot(obj)
register struct obj* obj;
{
    return in_container_core(obj, FALSE);
}

STATIC_PTR int
in_container_core(obj, dobot)
register struct obj *obj;
boolean dobot;
{
    int res = 0;
    if (!dobot)
        context.skip_botl = TRUE;

    if (!current_container) 
    {
        impossible("<in> no current_container?");
        goto default_incontainer_end_here;
    } 
    else if (obj == uball || obj == uchain) 
    {
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        if(dobot)
            You_ex(ATR_NONE, CLR_MSG_FAIL, "must be kidding.");
        else
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "put %s into %s.", thecxname(obj), thecxname(current_container));
        goto default_incontainer_end_here;
    }
    else if (obj == current_container)
    {
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        if (dobot)
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "That would be an interesting topological exercise.");
        else
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Putting %s into itself would be an interesting topological exercise.", thecxname(obj));
        goto default_incontainer_end_here;
    }
    else if (obj->owornmask & (W_ARMOR | W_ACCESSORY)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        if (dobot)
            Norep_ex(ATR_NONE, CLR_MSG_FAIL, "You cannot %s %s you are wearing.",
              Icebox ? "refrigerate" : "stash", something);
        else
        {
            const char* onam = thecxname(obj);
            boolean useplural = (obj->quan > 1L || strstri(onam, " pair of ") != 0);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s, you are wearing %s.", Icebox ? "refrigerate" : "stash", onam, useplural ? "them" : "it");
        }
        goto default_incontainer_end_here;
    }
    else if ((objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && obj->cursed) 
    {
        play_sfx_sound(SFX_GENERAL_WELDED);
        if (!obj->bknown)
        {
            obj->bknown = TRUE;
            update_inventory();
        }
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "The %s won't leave your person.", dobot && is_graystone(obj) ? (obj->quan == 1 ? "stone" : "stones") : cxname(obj));
        goto default_incontainer_end_here;
    }
    else if (
        ((objects[current_container->otyp].oc_flags4 & O4_CONTAINER_ACCEPTS_ONLY_SCROLLS_AND_BOOKS) && !(obj->oclass == SCROLL_CLASS || obj->oclass == SPBOOK_CLASS))
        || ((objects[current_container->otyp].oc_flags4 & O4_CONTAINER_ACCEPTS_ONLY_WEAPONS) && !(obj->oclass == WEAPON_CLASS))
        )
    {
        play_sfx_sound(SFX_GENERAL_DOES_NOT_FIT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is not made for holding %s.", The(cxname(current_container)), obj->quan > 1 ? cxname(obj) : makeplural(cxname(obj)));
        goto default_incontainer_end_here;
    }
    else if (obj->otyp == AMULET_OF_YENDOR
               || obj->otyp == CANDELABRUM_OF_INVOCATION
               || obj->otyp == BELL_OF_OPENING
               || obj->otyp == SPE_BOOK_OF_THE_DEAD
        )
    {
        /* Prohibit Amulets in containers; if you allow it, monsters can't
         * steal them.  It also becomes a pain to check to see if someone
         * has the Amulet.  Ditto for the Candelabrum, the Bell and the Book.
         */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s cannot be confined in such trappings.", The(xname(obj)));
        goto default_incontainer_end_here;
    }
    else if (obj->otyp == LEASH && obj->leashmon != 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s attached to your pet.", Tobjnam(obj, "are"));
        goto default_incontainer_end_here;
    }
    else if (obj == uwep) 
    {
        if (welded(obj, &youmonst))
        {
            weldmsg(obj);
            goto default_incontainer_end_here;
        }
        setuwep((struct obj *) 0, W_WEP);
        /* This uwep check is obsolete.  It dates to 3.0 and earlier when
         * unwielding Firebrand would be fatal in hell if hero had no other
         * fire resistance.  Life-saving would force it to be re-wielded.
         */
        if (uwep)
            goto default_incontainer_end_here; /* unwielded, died, rewielded */
    }
    else if (u.twoweap && obj == uarms) 
    {
        if (welded(obj, &youmonst))
        {
            weldmsg(obj);
            goto default_incontainer_end_here;
        }
        setuwep((struct obj*) 0, W_WEP2);
        /* This uwep check is obsolete.  It dates to 3.0 and earlier when
         * unwielding Firebrand would be fatal in hell if hero had no other
         * fire resistance.  Life-saving would force it to be re-wielded.
         */
        if (uarms)
            goto default_incontainer_end_here; /* unwielded, died, rewielded */
    }
    else if (obj == uswapwep)
    {
        setuswapwep((struct obj *) 0, W_SWAPWEP);
    }
    else if (obj == uswapwep2)
    {
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
    }
    else if (obj == uquiver) 
    {
        setuqwep((struct obj *) 0);
    }

    if (fatal_corpse_mistake(obj, FALSE))
    {
        res = -1;
        goto default_incontainer_end_here;
    }

    boolean floor_container = !carried(current_container);
    boolean was_unpaid = FALSE;
    char buf[BUFSZ];

    /* boxes, boulders, and big statues can't fit into any container */
    if (obj->otyp == ICE_BOX || obj->otyp == BOOKSHELF || Is_box(obj) || obj->otyp == BOULDER
        || (obj->otyp == STATUE && bigmonst(&mons[obj->corpsenm]))) 
    {
        /*
         *  xname() uses a static result array.  Save obj's name
         *  before current_container's name is computed.  Don't
         *  use the result of strcpy() within You() --- the order
         *  of evaluation of the parameters is undefined.
         */
        play_sfx_sound(SFX_GENERAL_DOES_NOT_FIT);
        Strcpy(buf, the(xname(obj)));
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot fit %s into %s.", buf, the(xname(current_container)));
        return 0;
    }

    /* Prompt if trying to put a container in a container (possibly by mistake) */
    if (Is_container(obj))
    {
        char qbuf[BUFSZ], c;
        Sprintf(qbuf, "You are putting %s into %s. Continue?", the(xname(obj)), the(xname(current_container)));
        if ((c = yn_function_es(YN_STYLE_GENERAL, ATR_NONE, CLR_MSG_ATTENTION, (const char*)0, qbuf, ynchars, 'n', yndescs, (const char*)0)) != 'y')
            return 0;
    }

    freeinv(obj);

    if (obj_is_burning(obj)) /* this used to be part of freeinv() */
        (void) snuff_lit(obj);

    if (floor_container && costly_spot(u.ux, u.uy))
    {
        /* defer gold until after put-in message */
        if (obj->oclass != COIN_CLASS) 
        {
            /* sellobj() will take an unpaid item off the shop bill */
            was_unpaid = obj->unpaid ? TRUE : FALSE;
            /* don't sell when putting the item into your own container,
             * but handle billing correctly */
            sellobj_state(current_container->no_charge
                          ? SELL_DONTSELL : SELL_DELIBERATE);
            sellobj(obj, u.ux, u.uy);
            sellobj_state(SELL_NORMAL);
        }
    }
    if (Icebox && !age_is_relative(obj)) 
    {
        obj->age = monstermoves - obj->age; /* actual age */
        /* stop any corpse timeouts when frozen */
        if (obj->otyp == CORPSE && obj->timed) 
        {
            int64_t rot_alarm = stop_timer(ROT_CORPSE, obj_to_any(obj));

            (void) stop_timer(REVIVE_MON, obj_to_any(obj));
            /* mark a non-reviving corpse as such */
            if (rot_alarm)
                obj->norevive = 1;
        }
    } 
    else if (Is_weight_changing_bag(current_container) && mbag_explodes(obj, current_container, 0)) 
    {
        int saved_otyp = obj->otyp;
        int saved_oclass = obj->oclass;

        /* explicitly mention what item is triggering the explosion */
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "As you put %s inside, you are blasted by a magical explosion!",
              doname(obj));

        livelog_printf(LL_ACHIEVE, "just blew up %s %s", uhis(), cxname(obj));

        /* did not actually insert obj yet */
        if (was_unpaid)
            addtobill(obj, FALSE, FALSE, TRUE);

        boolean destroyobj = !(is_obj_indestructible(obj) || obj->oartifact);
        if (destroyobj)
            obfree(obj, (struct obj *) 0);

        /* if carried, shop goods will be flagged 'unpaid' and obfree() will
           handle bill issues, but if on floor, we need to put them on bill
           before deleting them (non-shop items will be flagged 'no_charge') */
        if (floor_container
            && costly_spot(current_container->ox, current_container->oy)) 
        {
            struct obj save_no_charge;

            save_no_charge.no_charge = current_container->no_charge;
            addtobill(current_container, FALSE, FALSE, FALSE);
            /* addtobill() clears no charge; we need to set it back
               so that useupf() doesn't double bill */
            current_container->no_charge = save_no_charge.no_charge;
        }

        explode(u.ux, u.uy, RAY_MAGIC_MISSILE, (struct monst*)0, 2, 6, 0, saved_otyp, saved_oclass, EXPL_MAGICAL);
        /*delete_contents(current_container);*/
        Strcpy(debug_buf_2, "in_container_core");
        register struct obj* curr;
        while ((curr = current_container->cobj) != 0) {
            obj_extract_self(curr);
            dropy(curr);
            (void)scatter(curr->ox, curr->oy, 3, VIS_EFFECTS | MAY_HIT | MAY_DESTROY | MAY_FRACTURE, curr);
        }

        /* The artifact flies out last */
        if (!destroyobj)
        {
            dropy(obj);
            (void)scatter(obj->ox, obj->oy, 3, VIS_EFFECTS | MAY_HIT | MAY_DESTROY | MAY_FRACTURE, obj);
        }

        if (!floor_container)
            useup(current_container);
        else if (obj_here(current_container, u.ux, u.uy))
            useupf(current_container, current_container->quan);
        else
        {
            panic("in_container:  bag not found.");
            return 0;
        }

        losehp(adjust_damage(d(6, 6), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_SPELL_DAMAGE), "magical explosion", KILLED_BY_AN);
        current_container = 0; /* baggone = TRUE; */

        standard_hint("Putting a wand of cancellation into a magical bag will typically cause it explode. To avoid doing this accidently, put unidentified wands into a non-magical bag.", &u.uhint.bag_destroyed_by_cancellation);
    }

    if (current_container) 
    {
        Strcpy(buf, the(xname(current_container)));
        You("put %s into %s.", doname(obj), buf);
        if (iflags.using_gui_sounds)
        {
            play_object_container_in_sound(obj, current_container);
            delay_output_milliseconds(ITEM_PICKUP_DROP_DELAY);
        }

        /* gold in container always needs to be added to credit */
        if (floor_container && obj->oclass == COIN_CLASS)
            sellobj(obj, current_container->ox, current_container->oy);
        if (Is_magic_chest(current_container))
            (void) add_to_magic_chest(obj);
        else
            (void) add_to_container(current_container, obj);
        current_container->owt = weight(current_container);
    }
    /* gold needs this, and freeinv() many lines above may cause
     * the encumbrance to disappear from the status, so just always
     * update status immediately.
     */
    if(dobot)
        bot();
    res = (current_container ? 1 : -1);
default_incontainer_end_here:
    context.skip_botl = FALSE;
    return res;
}

/* askchain() filter used by in_container();
 * returns True if the container is intact and 'obj' isn't it, False if
 * container is gone (magic bag explosion) or 'obj' is the container itself;
 * also used by getobj() when picking a single item to stash
 */
int
ck_bag(obj)
struct obj *obj;
{
    return (current_container && obj != current_container);
}

/* Returns: -1 to stop, 1 item was moved, 0 item was not moved. */
STATIC_PTR int
move_container(obj)
register struct obj* obj;
{
    return move_container_core(obj, TRUE);
}

STATIC_PTR int
move_container_nobot(obj)
register struct obj* obj;
{
    return move_container_core(obj, FALSE);
}

STATIC_PTR int
move_container_core(obj, dobot)
register struct obj* obj;
boolean dobot;
{
    if (!obj || !move_target_container)
        return -1;

    obj->nomerge = 1;
    int res = 0;
    if ((res = out_container_core(obj, dobot, FALSE, (uchar*)0)) <= 0)
    {
        obj->nomerge = 0;
        return res;
    }
    obj->nomerge = 0;

    if (obj->where != OBJ_INVENT)
        return 0;

    if (!Is_proper_container(move_target_container))
    {
        pline("For some reason, you cannot put %s in %s!", thesimpleoname(obj), thesimpleoname(move_target_container));
        return -1;
    }


    struct obj* savedcontainer = current_container;
    current_container = move_target_container;
    res = in_container_core(obj, dobot);
    current_container = savedcontainer;
    return res;
}

/* Returns: -1 to stop, 1 item was moved, 0 item was not moved. */
STATIC_PTR int
out_container_and_drop(obj)
register struct obj* obj;
{
    return out_container_and_drop_core(obj, TRUE);
}

STATIC_PTR int
out_container_and_drop_nobot(obj)
register struct obj* obj;
{
    return out_container_and_drop_core(obj, FALSE);
}

STATIC_PTR int
out_container_and_drop_core(obj, dobot)
register struct obj* obj;
boolean dobot;
{
    if (!obj)
        return -1;

    obj->nomerge = 1;
    int res = 0;
    if ((res = out_container_core(obj, dobot, FALSE, (uchar*)0)) <= 0)
    {
        obj->nomerge = 0;
        return res;
    }

    obj->nomerge = 0;

    if (obj->where != OBJ_INVENT)
        return 0;

    if(!dobot)
        context.skip_botl = TRUE;
    res = drop(obj);
    context.skip_botl = FALSE;

    return res;
}


/* Returns: -1 to stop, 1 item was moved, 0 item was not moved. */
STATIC_PTR int
pickup_and_in_container(obj)
register struct obj* obj;
{
    return pickup_and_in_container_core(obj, TRUE);
}

STATIC_PTR int
pickup_and_in_container_nobot(obj)
register struct obj* obj;
{
    return pickup_and_in_container_core(obj, FALSE);
}

STATIC_PTR int
pickup_and_in_container_core(obj, dobot)
register struct obj* obj;
boolean dobot;
{
    if (!obj)
        return -1;

    obj->nomerge = 1;
    int res = 0;
    if (!dobot)
        context.skip_botl = TRUE;
    uchar obj_gone = FALSE;
    res = pickup_object(obj, obj->quan, FALSE, FALSE, &obj_gone);
    context.skip_botl = FALSE;
    if (obj_gone)
        return res;
    if (res <= 0)
    {
        obj->nomerge = 0;
        return res;
    }

    obj->nomerge = 0;

    if (obj->where != OBJ_INVENT)
        return 0;

    res = in_container_core(obj, dobot);
    return res;
}

/* Returns: -1 to stop, 1 item was moved, 0 item was not moved. */
STATIC_PTR int
out_container_and_autostash(obj)
register struct obj* obj;
{
    return out_container_core(obj, TRUE, TRUE, (uchar*)0);
}

/* Returns: -1 to stop, 1 item was moved, 0 item was not moved. */
STATIC_PTR int
out_container_and_autostash_nobot(obj)
register struct obj* obj;
{
    return out_container_core(obj, FALSE, TRUE, (uchar*)0);
}


/* Returns: -1 to stop, 1 item was removed, 0 item was not removed. */
STATIC_PTR int
out_container(obj)
register struct obj* obj;
{
    return out_container_core(obj, TRUE, FALSE, (uchar*)0);
}

STATIC_PTR int
out_container_nobot(obj)
register struct obj* obj;
{
    return out_container_core(obj, FALSE, FALSE, (uchar*)0);
}

STATIC_PTR int
out_container_core(obj, dobot, do_auto_in_bag, obj_gone_ptr)
register struct obj *obj;
boolean dobot, do_auto_in_bag;
uchar* obj_gone_ptr;
{
    register struct obj *otmp;
    boolean is_gold = (obj->oclass == COIN_CLASS);
    int res = 0, loadlev;
    int64_t count;
    if (!dobot)
        context.skip_botl = TRUE;

    if (!current_container) 
    {
        impossible("<out> no current_container?");
        res = -1;
        goto default_outcountainer_end;
    }
    else if (is_gold)
    {
        obj->owt = weight(obj);
    }

    if (obj->oartifact && !touch_artifact(obj, &youmonst))
        goto default_outcountainer_end;

    if (fatal_corpse_mistake(obj, FALSE))
    {
        res = -1;
        goto default_outcountainer_end;
    }
    count = obj->quan;
    if ((res = lift_object(obj, current_container, &count, FALSE)) <= 0)
        goto default_outcountainer_end;

    if (obj->quan != count && !(objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED))
        obj = splitobj(obj, count);

    /* Remove the object from the list. */
    Strcpy(debug_buf_2, "out_container_core");
    obj_extract_self(obj);
    current_container->owt = weight(current_container);

    if (Icebox)
        removed_from_icebox(obj);

    if (!obj->unpaid && !carried(current_container)
        && costly_spot(current_container->ox, current_container->oy)) 
    {
        obj->ox = current_container->ox;
        obj->oy = current_container->oy;
        addtobill(obj, FALSE, FALSE, FALSE);
    }
    if (is_pick(obj))
        pick_pick(obj); /* shopkeeper feedback */

    otmp = addinv(obj);
    if (obj_gone_ptr && otmp != obj)
        *obj_gone_ptr = 1;
    loadlev = near_capacity();
    prinv(loadlev ? ((loadlev < MOD_ENCUMBER)
                        ? "You have a little trouble removing"
                        : "You have much trouble removing")
                  : (char *) 0,
          otmp, count);
    if (iflags.using_gui_sounds)
    {
        play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_PICK_UP);
        delay_output_milliseconds(ITEM_PICKUP_DROP_DELAY);
    }
    if (do_auto_in_bag && otmp && otmp == obj)
    {
        int bagres = auto_bag_in(invent, otmp, FALSE);
        if (obj_gone_ptr && bagres)
            *obj_gone_ptr = 2;
    }
    if (dobot && is_gold)
    {
        bot(); /* update character's gold piece count immediately */
    }

    res = 1;
default_outcountainer_end:
    context.skip_botl = FALSE;
    return res;
}

/* taking a corpse out of an ice box needs a couple of adjustments */
STATIC_OVL void
removed_from_icebox(obj)
struct obj *obj;
{
    if (!age_is_relative(obj)) 
    {
        obj->age = monstermoves - obj->age; /* actual age */
        if (obj->otyp == CORPSE)
            start_corpse_timeout(obj);
    }
}

/* an object inside a cursed bag of holding is being destroyed */
STATIC_OVL int64_t
mbag_item_gone(held, item)
int held;
struct obj *item;
{
    struct monst *shkp;
    int64_t loss = 0L;

    if (item->dknown)
        pline("%s %s vanished!", Doname2(item), otense(item, "have"));
    else
        You("%s %s disappear!", Blind ? "notice" : "see", doname(item));

    if (*u.ushops && (shkp = shop_keeper(*u.ushops)) != 0)
    {
        if (held ? (boolean) item->unpaid : costly_spot(u.ux, u.uy))
            loss = stolen_value(item, u.ux, u.uy, is_peaceful(shkp),
                                TRUE);
    }
    obfree(item, (struct obj *) 0);
    return loss;
}

/* used for #loot/apply, #tip, and final disclosure */
void
observe_quantum_cat(box, makecat, givemsg)
struct obj *box;
boolean makecat, givemsg;
{
    STATIC_VAR NEARDATA const char sc[] = "Schroedinger's Cat";
    struct obj *deadcat;
    struct monst *livecat = 0;
    xchar ox, oy;
    boolean itsalive = !rn2(2);

    if (get_obj_location(box, &ox, &oy, 0))
        box->ox = ox, box->oy = oy; /* in case it's being carried */

    /* this isn't really right, since any form of observation
       (telepathic or monster/object/food detection) ought to
       force the determination of alive vs dead state; but basing it
       just on opening or disclosing the box is much simpler to cope with */

    /* SchroedingersBox already has a cat corpse in it */
    deadcat = box->cobj;
    if (itsalive) 
    {
        if (makecat)
            livecat = makemon(&mons[PM_HOUSECAT], box->ox, box->oy,
                              MM_NO_MONSTER_INVENTORY | MM_ADJACENTOK);
        if (livecat) 
        {
            livecat->mpeaceful = 1;
            set_mhostility(livecat);
            if (givemsg) 
            {
                if (!canspotmon(livecat))
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "think %s brushed your %s.", something,
                        body_part(FOOT));
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s inside the box is still alive!",
                          Monnam(livecat));
            }
            (void) christen_monst(livecat, sc);
            livecat->u_know_mname = TRUE; /* It is famous! */
            if (deadcat)
            {
                Strcpy(debug_buf_2, "observe_quantum_cat");
                obj_extract_self(deadcat);
                obfree(deadcat, (struct obj *) 0), deadcat = 0;
            }
            box->owt = weight(box);
            box->speflags &= ~SPEFLAGS_SCHROEDINGERS_BOX;
        }
    } 
    else 
    {
        box->speflags &= ~SPEFLAGS_SCHROEDINGERS_BOX; /* now an ordinary box (with a cat corpse inside) */
        if (deadcat)
        {
            /* set_corpsenm() will start the rot timer that was removed
               when makemon() created SchroedingersBox; start it from
               now rather than from when this special corpse got created */
            deadcat->age = monstermoves;
            set_corpsenm(deadcat, PM_HOUSECAT);
            deadcat = oname(deadcat, sc);
        }
        if (givemsg)
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s inside the box is dead!",
                      Hallucination ? rndmonnam((char *) 0) : "housecat");
    }
    nhUse(deadcat);
    return;
}

#undef Icebox

/* used by askchain() to check for magic bag explosion */
boolean
container_gone(fn)
int FDECL((*fn), (OBJ_P));
{
    /* result is only meaningful while use_container() is executing */
    return ((fn == in_container || fn == out_container || fn == move_container || fn == pickup_and_in_container || fn == out_container_and_drop || fn == out_container_and_autostash
        || fn == in_container_nobot || fn == out_container_nobot || fn == move_container_nobot || fn == pickup_and_in_container_nobot || fn == out_container_and_drop_nobot || fn == out_container_and_autostash_nobot)
            && !current_container);
}

STATIC_OVL void
explain_container_prompt(more_containers)
boolean more_containers;
{
    static const char *const explaintext[] = {
        "Container actions:",
        "",
        " : -- Look: examine contents",
        " o -- Out: take things out",
        " i -- In: put things in",
        " b -- Both: first take things out, then put things in",
        " r -- Reversed: put things in, then take things out",
        " s -- Stash: put one item in", "",
        " n -- Next: loot next selected container",
        " q -- Quit: finished",
        " ? -- Help: display this text.",
        "", 0
    };
    const char *const *txtpp;
    winid win;

    /* "Do what with <container>? [:oibrsq or ?] (q)" */
    if ((win = create_nhwindow(NHW_TEXT)) != WIN_ERR) {
        for (txtpp = explaintext; *txtpp; ++txtpp) {
            if (!more_containers && !strncmp(*txtpp, " n ", 3))
                continue;
            putstr(win, 0, *txtpp);
        }
        display_nhwindow(win, FALSE);
        destroy_nhwindow(win);
    }
}

boolean
u_handsy()
{
    if (nohands(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "have no hands!"); /* not `body_part(HAND)' */
        return FALSE;
    } else if (!freehand()) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "have no free %s.", body_part(HAND));
        return FALSE;
    }
    return TRUE;
}

STATIC_VAR const char stashable[] = { ALLOW_COUNT, COIN_CLASS, ALL_CLASSES, 0 };

int
use_container(objp, held, more_containers)
struct obj **objp;
int held;
boolean more_containers; /* True iff #loot multiple and this isn't last one */
{
    struct obj *otmp, *obj = *objp;
    boolean quantum_cat, cursed_mbag, loot_out, loot_in, loot_in_first,
        stash_one, move_to_another, move_to_another_on_floor, loot_out_and_drop, loot_out_and_stash,
        pickup_and_loot_in, inokay, outokay, outmaybe;
    char c, emptymsg[BUFSZ], qbuf[QBUFSZ], pbuf[QBUFSZ], xbuf[QBUFSZ];
    int used = 0;
    int64_t loss;

    abort_looting = FALSE;
    emptymsg[0] = '\0';

#if 0
    if (!u_handsy() && !is_telekinetic_operator(youmonst.data))
        return 0;
#endif

    boolean insider = (*u.ushops && inside_shop(u.ux, u.uy)
        && *in_rooms(u.ux, u.uy, SHOPBASE) == *u.ushops);

    if (Is_magic_chest(obj) && insider)
    {
        pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "A mysterious force prevents you from opening %s.", thecxname(obj));
        play_sfx_sound(SFX_MYSTERIOUS_FORCE_PREVENTS);
        return 0;
    }
    else if (obj->olocked)
    {
        pline("%s locked.", Tobjnam(obj, "are"));
        if (held)
            You("must put it down to unlock.");
        obj->lknown = 1;
        return 0;
    } 
    else if (obj->otrapped) 
    {
        if (held)
            You("open %s...", the(xname(obj)));

        obj->lknown = 1;
        (void) chest_trap(obj, HAND, FALSE);

        /* even if the trap fails, you've used up this turn */
        if (multi >= 0) 
        { /* in case we didn't become paralyzed */
            nomul(-1);
            multi_reason = "opening a container";
            nomovemsg = "";
        }
        abort_looting = TRUE;
        return 1;
    }
    obj->lknown = 1;
    obj->tknown = 1;

    current_container = obj; /* for use by in/out_container */
    /*
     * From here on out, all early returns go through 'containerdone:'.
     */

    /* check for Schroedinger's Cat */
    quantum_cat = SchroedingersBox(current_container);

    if (quantum_cat) 
    {
        observe_quantum_cat(current_container, TRUE, TRUE);
        used = 1;
    }

    cursed_mbag = Is_weight_changing_bag(current_container)
        && current_container->otyp != BAG_OF_WIZARDRY //Bag of wizardry does not lose items, other mbags may
        && current_container->cursed
        && Has_contents(current_container);

    if (cursed_mbag
        && (loss = boh_loss(current_container, held)) != 0) 
    {
        used = 1;
        You("owe %ld %s for lost merchandise.", loss, currency(loss));
        current_container->owt = weight(current_container);
    }

    inokay = (invent != 0
              && !(invent == current_container && !current_container->nobj));
    outokay = Has_contained_contents(current_container);

    if (!outokay) /* preformat the empty-container message */
        Sprintf(emptymsg, "%s is %sempty.", Ysimple_name2(current_container),
                (quantum_cat || cursed_mbag) ? "now " : "");

    struct obj* last_container = (struct obj*)0;
    struct obj* last_floor_container = (struct obj*)0;
    int other_containter_count = count_other_containers(invent, obj, &last_container, FALSE);
    int floor_containter_count = count_other_containers(level.objects[u.ux][u.uy], obj, &last_floor_container, TRUE);

    /*
     * What-to-do prompt's list of possible actions:
     * always include the look-inside choice (':');
     * include the take-out choice ('o') if container
     * has anything in it or if player doesn't yet know
     * that it's empty (latter can change on subsequent
     * iterations if player picks ':' response);
     * include the put-in choices ('i','s') if hero
     * carries any inventory (including gold);
     * include do-both when 'o' is available, even if
     * inventory is empty--taking out could alter that;
     * include do-both-reversed when 'i' is available,
     * even if container is empty--for similar reason;
     * include the next container choice ('n') when
     * relevant, and make it the default;
     * always include the quit choice ('q'), and make
     * it the default if there's no next containter;
     * include the help choice (" or ?") if `cmdassist'
     * run-time option is set;
     * (Player can pick any of (o,i,b,r,n,s,?) even when
     * they're not listed among the available actions.)
     *
     * Do what with <the/your/Shk's container>? [:oibrs nq or ?] (q)
     * or
     * <The/Your/Shk's container> is empty.  Do what with it? [:irs nq or ?]
     */
    for (;;) 
    { /* repeats iff '?' or ":' gets chosen */
        outmaybe = (outokay || !current_container->cknown);

        if (!outmaybe)
            (void) safe_qbuf(qbuf, (char *) 0, " is empty.  Do what with it?",
                             current_container, Yname2, Ysimple_name2,
                             "This");
        else
            (void) safe_qbuf(qbuf, "Do what with ", "?", current_container,
                             yname, ysimple_name, "it");

        /* ask player about what to do with this container */
        if (flags.menu_style == MENU_PARTIAL
            || flags.menu_style == MENU_FULL) 
        {
            if (!inokay && !outmaybe)
            {
                /* nothing to take out, nothing to put in;
                   trying to do both will yield proper feedback */
                c = 'b';
            } 
            else 
            {
                c = in_or_out_menu(qbuf, current_container, outmaybe, inokay,
                                   (boolean) (used != 0), more_containers);
            }
        } 
        else 
        { /* TRADITIONAL or COMBINATION */

            xbuf[0] = '\0'; /* list of extra acceptable responses */
            Strcpy(pbuf, ":");                   /* look inside */
            Strcat(outmaybe ? pbuf : xbuf, "o"); /* take out */
            Strcat(inokay ? pbuf : xbuf, "i");   /* put in */
            Strcat(outmaybe ? pbuf : xbuf, "b"); /* both */
            Strcat(inokay ? pbuf : xbuf, "rs");  /* reversed, stash */
            Strcat(outmaybe && other_containter_count > 0 ? pbuf : xbuf, "m"); /* move to inventory container */
            Strcat(outmaybe && floor_containter_count > 0 ? pbuf : xbuf, "c"); /* move to floor container */
            Strcat(outmaybe && other_containter_count > 0 ? pbuf : xbuf, "a"); /* take out and auto-stash */
            Strcat(outmaybe ? pbuf : xbuf, "d"); /* take out and drop */
            Strcat(inokay &&
                (level.objects[u.ux][u.uy] && !(level.objects[u.ux][u.uy] == current_container && !level.objects[u.ux][u.uy]->nexthere)) ? pbuf : xbuf,
                "p"); /* pick up and put in */
            Strcat(pbuf, " ");                   /* separator */
            Strcat(more_containers ? pbuf : xbuf, "n"); /* next container */
            Strcat(pbuf, "q");                   /* quit */
            if (iflags.cmdassist)
                /* this unintentionally allows user to answer with 'o' or
                   'r'; fortunately, those are already valid choices here */
                Strcat(pbuf, " or ?"); /* help */
            else
                Strcat(xbuf, "?");
            if (*xbuf)
                Strcat(strcat(pbuf, "\033"), xbuf);
            c = yn_function(qbuf, pbuf, more_containers ? 'n' : 'q', (char*)0);
        } /* PARTIAL|FULL vs other modes */

        if (c == '?') 
        {
            explain_container_prompt(more_containers);
        } 
        else if (c == ':') 
        { /* note: will set obj->cknown */
            if (!current_container->cknown)
                used = 1; /* gaining info */
            display_cinventory(current_container);
            //container_contents(current_container, FALSE, FALSE, TRUE, 1);
        } 
        else
            break;
    } /* loop until something other than '?' or ':' is picked */

    if (c == 'q')
        abort_looting = TRUE;
    if (c == 'n' || c == 'q') /* [not strictly needed; falling thru works] */
        goto containerdone;
    loot_out = (c == 'o' || c == 'b' || c == 'r');
    loot_in = (c == 'i' || c == 'b' || c == 'r');
    loot_in_first = (c == 'r'); /* both, reversed */
    stash_one = (c == 's');
    move_to_another = (c == 'm');
    move_to_another_on_floor = (c == 'c');
    loot_out_and_drop = (c == 'd');
    loot_out_and_stash = (c == 'a');
    pickup_and_loot_in = (c == 'p');

    /* out-only or out before in */
    if (move_to_another || move_to_another_on_floor || (loot_out && !loot_in_first) || loot_out_and_drop || loot_out_and_stash)
    {
        if (!Has_contained_contents(current_container)) 
        {
            pline1(emptymsg); /* <whatever> is empty. */
            if (!current_container->cknown)
                used = 1;
            current_container->cknown = 1;
        } 
        else 
        {
            add_valid_menu_class(0); /* reset */
            if (flags.menu_style == MENU_TRADITIONAL)
                used |= traditional_loot(
                    move_to_another ? 2 : move_to_another_on_floor ? 3 : loot_out_and_drop ? 4 : loot_out_and_stash ? 5 : 0,
                    current_container, 
                    move_to_another_on_floor ? (floor_containter_count == 1 && last_floor_container ? last_floor_container : (struct obj*)0) : (other_containter_count == 1 && last_container ? last_container : (struct obj*)0)
                );
            else
                used |= (menu_loot(0, 
                    move_to_another ? 2 : move_to_another_on_floor ? 3 : loot_out_and_drop ? 4 : loot_out_and_stash ? 5 : 0,
                    current_container, 
                    move_to_another_on_floor ? (floor_containter_count == 1 && last_floor_container ? last_floor_container : (struct obj*)0) : (other_containter_count == 1 && last_container ? last_container : (struct obj*)0)
                ) > 0 );
            add_valid_menu_class(0);
        }
    }

    if ((loot_in || stash_one)
        && (!invent || (invent == current_container && !invent->nobj))) 
    {
        You("don't have anything%s to %s.", invent ? " else" : "",
            stash_one ? "stash" : "put in");
        loot_in = stash_one = FALSE;
    }
    else if (pickup_and_loot_in 
        && (!level.objects[u.ux][u.uy] || (level.objects[u.ux][u.uy] == current_container && !level.objects[u.ux][u.uy]->nexthere)))
    {
        pline("There is nothing%s to %s.", level.objects[u.ux][u.uy] ? " else" : "",
            "pick up and put in");
        pickup_and_loot_in = FALSE;
    }
    else if (pickup_and_loot_in && !able_to_loot(u.ux, u.uy, TRUE))
    {
        pickup_and_loot_in = FALSE;
    }

    /*
     * Gone: being nice about only selecting food if we know we are
     * putting things in an ice chest.
     */
    if (loot_in || pickup_and_loot_in)
    {
        add_valid_menu_class(0); /* reset */
        if (flags.menu_style == MENU_TRADITIONAL)
            used |= traditional_loot(pickup_and_loot_in ? 6 : 1, (struct obj*)0, (struct obj*)0);
        else
            used |= (menu_loot(0, pickup_and_loot_in ? 6 : 1, (struct obj*)0, (struct obj*)0) > 0);
        add_valid_menu_class(0);
    } 
    else if (stash_one) 
    {
        /* put one item into container */
        if ((otmp = getobj(stashable, "stash", 1, "")) != 0) 
        {
            if (in_container(otmp)) 
            {
                used = 1;
            }
            else 
            {
                /* couldn't put selected item into container for some
                   reason; might need to undo splitobj() */
                (void) unsplitobj(otmp);
            }
        }
    }

    /* putting something in might have triggered magic bag explosion */
    if (!current_container)
        loot_out = FALSE;

    /* out after in */
    if (loot_out && loot_in_first) 
    {
        if (!Has_contained_contents(current_container)) 
        {
            pline1(emptymsg); /* <whatever> is empty. */
            if (!current_container->cknown)
                used = 1;
            current_container->cknown = 1;
        } 
        else 
        {
            add_valid_menu_class(0); /* reset */
            if (flags.menu_style == MENU_TRADITIONAL)
                used |= traditional_loot(0, (struct obj*)0, (struct obj*)0);
            else
                used |= (menu_loot(0, 0, (struct obj*)0, (struct obj*)0) > 0);
            add_valid_menu_class(0);
        }
    }

containerdone:
    if (used) 
    {
        /* Not completely correct; if we put something in without knowing
           whatever was already inside, now we suddenly do.  That can't
           be helped unless we want to track things item by item and then
           deal with containers whose contents are "partly known". */
        if (current_container)
            current_container->cknown = 1;
        update_inventory();
        force_redraw_at(u.ux, u.uy);
    }

    *objp = current_container; /* might have become null */
    if (current_container)
        current_container = 0; /* avoid hanging on to stale pointer */
    else
        abort_looting = TRUE;
    return used;
}

/* loot current_container (take things out or put things in), by prompting */
STATIC_OVL int
traditional_loot(command_id, applied_container, other_container)
int command_id;
/* 0 = take out,
   1 = put in,
   2 = move items in a container to a(nother) container in inventory,
   3 = move items in a container to a(nother) container on floor,
   4 = take items out of a container and drop them on floor,
   5 = take items out of a container and stash automatically, and
   6 = pick up items from floor and put them in a container
 */
struct obj* applied_container;
struct obj* other_container;
{
    int FDECL((*actionfunc), (OBJ_P)), FDECL((*checkfunc), (OBJ_P));
    struct obj **objlist;
    char selection[MAX_OBJECT_CLASSES + 10]; /* +10: room for B,U,C,X plus slop */
    const char *action;
    boolean one_by_one, allflag;
    int used = 0, menu_on_request = 0;

    switch (command_id)
    {
    default:
    case 0:
        action = "take out";
        objlist = contained_object_chain_ptr(current_container);
        actionfunc = out_container;
        checkfunc = (int FDECL((*), (OBJ_P))) 0;
        break;
    case 1:
        action = "put in";
        objlist = &invent;
        actionfunc = in_container;
        checkfunc = ck_bag;
        break;
    case 2:
#if 0
        if (other_container)
            move_target_container = other_container;
        else
#endif
        {
            move_target_container = (struct obj*)0;
            /* Select container here */
            move_target_container = select_other_container(invent, applied_container, FALSE);
            if (!move_target_container)
                return 0;
        }

        action = "move to another container in inventory";
        objlist = contained_object_chain_ptr(current_container);
        actionfunc = move_container;
        checkfunc = (int FDECL((*), (OBJ_P))) 0;
        break;
    case 3:
#if 0

        if (other_container)
            move_target_container = other_container;
        else
#endif
        {
            move_target_container = (struct obj*)0;
            /* Select container here */
            move_target_container = select_other_container(level.objects[u.ux][u.uy], applied_container, TRUE);
            if (!move_target_container)
                return 0;
        }

        action = "move to another container on floor";
        objlist = contained_object_chain_ptr(current_container);
        actionfunc = move_container;
        checkfunc = (int FDECL((*), (OBJ_P))) 0;
        break;
    case 4:
        action = "take out and drop";
        objlist = contained_object_chain_ptr(current_container);
        actionfunc = out_container_and_drop;
        checkfunc = (int FDECL((*), (OBJ_P))) 0;
        break;
    case 5:
        action = "take out and auto-stash";
        objlist = contained_object_chain_ptr(current_container);
        actionfunc = out_container_and_autostash;
        checkfunc = (int FDECL((*), (OBJ_P))) 0;
        break;
    case 6:
        action = "pick up and put in";
        objlist = &level.objects[u.ux][u.uy];
        actionfunc = pickup_and_in_container;
        checkfunc = ck_bag;
        break;
    }

    if(command_id == 4 && *u.ushops)
        sellobj_state(SELL_DELIBERATE);

    if (query_classes(selection, &one_by_one, &allflag, action, *objlist, FALSE, &menu_on_request)) 
    {
        if (askchain(objlist, (one_by_one ? (char *) 0 : selection), allflag, actionfunc, checkfunc, 0, action))
            used = 1;
    } 
    else if (menu_on_request < 0) 
    {
        used = (menu_loot(menu_on_request, command_id, applied_container, other_container) > 0);
    }

    if (command_id == 4 && *u.ushops)
        sellobj_state(SELL_NORMAL);

    return used;
}

/* loot current_container (take things out or put things in), using a menu */
STATIC_OVL int
menu_loot(retry, command_id, applied_container, other_container) 
int retry;
int command_id; 
/* 0 = take out, 
   1 = put in, 
   2 = move items in a container to a(nother) container in inventory, 
   3 = move items in a container to a(nother) container on floor,
   4 = take items out of a container and drop them on floor,
   5 = take items out of a container and stash them automatically, and
   6 = pick up items from the floor and put them in a container
 */
struct obj* applied_container;
struct obj* other_container UNUSED;
{
    int n, i, n_looted = 0;
    boolean all_categories = TRUE, loot_everything = FALSE;
    char buf[BUFSZ];
    struct obj* otmp, * otmp2;
    menu_item *pick_list;
    int mflags, res = 0;
    int64_t count;
    int more_action = 0;

    const char* action = "";
    switch (command_id)
    {
    case 0:
        action = "Take out";
        break;
    case 1:
        action = "Put in";
        break;
    case 2:
        action = "Move";
        break;
    case 3:
        action = "Move";
        break;
    case 4:
        action = "Take out and drop";
        break;
    case 5:
        action = "Take out and auto-stash";
        break;
    case 6:
        action = "Pick up and put in";
        break;
    default:
        break;
    }

    /* Select target container if necessary */
    if (command_id == 2)
    {
#if 0
        if (other_container)
            move_target_container = other_container;
        else
#endif
        {
            move_target_container = (struct obj*)0;
            /* Choose another container */
            move_target_container = select_other_container(invent, applied_container, FALSE);
            if (!move_target_container)
                return 0;
        }
    }
    else if (command_id == 3)
    {
#if 0
        if (other_container)
            move_target_container = other_container;
        else
#endif
        {
            move_target_container = (struct obj*)0;
            /* Choose another container */
            move_target_container = select_other_container(level.objects[u.ux][u.uy], applied_container, TRUE);
            if (!move_target_container)
                return 0;
        }
    }
    else if (command_id == 4 && *u.ushops)
        sellobj_state(SELL_DELIBERATE);

    if (retry) 
    {
        all_categories = (retry == -2);
    } 
    else if (flags.menu_style == MENU_FULL) 
    {
        all_categories = FALSE;
        Sprintf(buf, "%s what type of objects?", action);
        mflags = (ALL_TYPES | UNPAID_TYPES | BUCX_TYPES | UNIDENTIFIED_TYPES | UNKNOWN_TYPES | CHOOSE_ALL);
        if (command_id == 6)
            mflags |= BY_NEXTHERE;
        n = query_category(buf, command_id == 1 ? invent : command_id == 6 ? level.objects[u.ux][u.uy] : contained_object_chain(current_container),
                           mflags, &pick_list, PICK_ANY);
        if (!n)
            return 0;

        for (i = 0; i < n; i++) 
        {
            if (pick_list[i].item.a_int == 'A')
            {
                if (ParanoidAutoSelectAll)
                {
                    if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "All Items Selected", "Are you sure to select all items?") != 'y')
                    {
                        free((genericptr_t)pick_list);
                        return 0;
                    }
                }

                loot_everything = TRUE;
            }
            else if (pick_list[i].item.a_int == ALL_TYPES_SELECTED)
            {
                all_categories = TRUE;
            }
            else
                add_valid_menu_class(pick_list[i].item.a_int);
        }
        free((genericptr_t) pick_list);
    }

    boolean do_auto_in_bag = FALSE;
    struct obj* last_container = 0;
    int container_cnt = count_other_containers(invent, current_container, &last_container, FALSE);
    struct obj* used_container = 0;

    if (loot_everything)
    {
        n = 0;
        for (otmp = contained_object_chain(current_container); otmp; otmp = otmp->nobj)
            n++;
        uchar obj_gone;
        switch (command_id)
        {
        case 0:
            current_container->cknown = 1;
            for (otmp = contained_object_chain(current_container), i = 0; otmp; otmp = otmp2, i++)
            {
                otmp2 = otmp->nobj;
                obj_gone = FALSE;
                res = out_container_core(otmp, FALSE, do_auto_in_bag, &obj_gone);
                if (res < 0)
                    break;
                n_looted += res;
                if (!carried(current_container) && !obj_gone && handle_knapsack_prefull(res, container_cnt, &do_auto_in_bag, &used_container, otmp, i, n, check_nonmergeable_nobj(otmp->nobj)) < 0)
                    break;
            }
            bot();
            if (res == -2 && flags.knapsack_prompt)
                more_action = handle_knapsack_full();
            break;
        case 1:
            for (otmp = invent; otmp && current_container; otmp = otmp2)
            {
                otmp2 = otmp->nobj;
                res = in_container_nobot(otmp);
                if (res < 0)
                    break;
                n_looted += res;
            }
            bot();
            break;
        case 2:
            current_container->cknown = 1;
            for (otmp = contained_object_chain(current_container); otmp; otmp = otmp2)
            {
                otmp2 = otmp->nobj;
                res = move_container_nobot(otmp);
                if (res < 0)
                    break;
                n_looted += res;
            }
            bot();
            break;
        case 3:
            current_container->cknown = 1;
            for (otmp = contained_object_chain(current_container); otmp; otmp = otmp2)
            {
                otmp2 = otmp->nobj;
                res = move_container_nobot(otmp);
                if (res < 0)
                    break;
                n_looted += res;
            }
            bot();
            break;
        case 4:
            current_container->cknown = 1;
            for (otmp = contained_object_chain(current_container); otmp; otmp = otmp2)
            {
                otmp2 = otmp->nobj;
                res = out_container_and_drop_nobot(otmp);
                if (res < 0)
                    break;
                n_looted += res;
            }
            bot();
            break;
        case 5:
            current_container->cknown = 1;
            for (otmp = contained_object_chain(current_container); otmp; otmp = otmp2)
            {
                otmp2 = otmp->nobj;
                res = out_container_and_autostash_nobot(otmp);
                if (res < 0)
                    break;
                n_looted += res;
            }
            bot();
            break;
        case 6:
            current_container->cknown = 1;
            for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp2)
            {
                otmp2 = otmp->nexthere;
                res = pickup_and_in_container(otmp);
                if (res < 0)
                    break;
                n_looted += res;
            }
            bot();
            break;
        default:
            break;

        }
    }
    else 
    {
        mflags = INVORDER_SORT | OBJECT_COMPARISON;
        if (command_id == 1 && flags.invlet_constant)
            mflags |= USE_INVLET;
        if (command_id == 6)
            mflags |= BY_NEXTHERE;
        if (command_id == 0 || command_id == 2)
            current_container->cknown = 1;

        char movebuf[BUFSZ] = "";
        if (command_id == 2 || command_id == 3)
            Sprintf(movebuf, " from %s to %s", cxname(current_container), cxname(move_target_container));

        Sprintf(buf, "%s what%s?", action, movebuf);
        n = query_objlist(buf, command_id == 1 ? &invent : command_id == 6 ? &level.objects[u.ux][u.uy] : contained_object_chain_ptr(current_container),
                          mflags, &pick_list, PICK_ANY,
                          all_categories ? allow_all : allow_category, 2);
        if (n)
        {
            n_looted = 0;
            context.quit_pressed = FALSE;
            uchar obj_gone;
            for (i = 0; i < n; i++)
            {
                n_looted++;
                otmp = pick_list[i].item.a_obj;
                count = pick_list[i].count;
                if (count > 0 && count < otmp->quan) 
                {
                    otmp = splitobj(otmp, count);
                    /* special split case also handled by askchain() */
                }

                switch (command_id)
                {
                case 0:
                    obj_gone = FALSE;
                    res = out_container_core(otmp, FALSE, do_auto_in_bag, &obj_gone);
                    if (res == -2 && flags.knapsack_prompt)
                    {
                        bot();
                        more_action = handle_knapsack_full();
                    }
                    else if (!carried(current_container) && !obj_gone && handle_knapsack_prefull(res, container_cnt, &do_auto_in_bag, &used_container, otmp, i, n, check_nonmergeable_nobj(otmp->nobj)) < 0)
                        break;
                    break;
                case 1:
                    res = in_container_nobot(otmp);
                    break;
                case 2:
                    res = move_container_nobot(otmp);
                    break;
                case 3:
                    res = move_container_nobot(otmp);
                    break;
                case 4:
                    res = out_container_and_drop_nobot(otmp);
                    break;
                case 5:
                    res = out_container_and_autostash_nobot(otmp);
                    break;
                case 6:
                    res = pickup_and_in_container_nobot(otmp);
                    break;
                default:
                    res = -1;
                    break;
                }

                if (res < 0) 
                {
                    if (!current_container) 
                    {
                        /* otmp caused current_container to explode;
                           both are now gone */
                        otmp = 0; /* and break loop */
                    } 
                    else if (otmp && otmp != pick_list[i].item.a_obj) 
                    {
                        /* split occurred, merge again */
                        (void) merged(&pick_list[i].item.a_obj, &otmp);
                    }
                    break;
                }

                if (context.quit_pressed)
                    break;
            }
            free((genericptr_t) pick_list);
            context.quit_pressed = FALSE;
            bot();
        }
    }

    if (command_id == 4 && *u.ushops)
        sellobj_state(SELL_NORMAL);

    return n_looted + more_action;
}

STATIC_OVL
boolean
check_nonmergeable_nobj(objchn)
struct obj* objchn;
{
    struct obj* otmp;
    for (otmp = objchn; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass != COIN_CLASS && !merge_choice(invent, otmp))
            return TRUE;
    }
    return FALSE;
}

STATIC_OVL
boolean
check_nonmergeable_pick_list(pick_list, i, n)
menu_item* pick_list;
int i, n;
{
    struct obj* otmp;
    int j;
    for (j = i; j < n; j++)
    {
        otmp = pick_list[j].item.a_obj;
        if (otmp->oclass != COIN_CLASS && !merge_choice(invent, otmp))
            return TRUE;
    }
    return FALSE;
}

STATIC_OVL char
in_or_out_menu(prompt, obj, outokay, inokay, alreadyused, more_containers)
const char *prompt;
struct obj *obj;
boolean outokay, inokay, alreadyused, more_containers;
{
    /* underscore is not a choice; it's used to skip element [0] */
    static const char lootchars[] = "_:oibrsnqmcdpa", abc_chars[] = "_:abcdenqfghij";
    winid win;
    anything any;
    menu_item *pick_list;
    char buf[BUFSZ];
    int n;
    const char *menuselector = flags.lootabc ? abc_chars : lootchars;

    struct obj* last_container = (struct obj*)0;
    int other_containter_count = count_other_containers(invent, obj, &last_container, FALSE);
    int floor_containter_count = count_other_containers(level.objects[u.ux][u.uy], obj, &last_container, TRUE);

    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu_ex(win, GHMENU_STYLE_GENERAL_COMMAND);

    any.a_int = 1; /* ':' */
    Sprintf(buf, "Look inside %s", thesimpleoname(obj));
    add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR, buf,
             MENU_UNSELECTED);

    if (outokay) 
    {
        any.a_int = 2; /* 'o' */
        Sprintf(buf, "take %s out", something);
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
                 buf, MENU_UNSELECTED);
    }
    if (inokay) 
    {
        any.a_int = 3; /* 'i' */
        Sprintf(buf, "put %s in", something);
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
                 buf, MENU_UNSELECTED);
    }
    if (outokay) 
    {
        any.a_int = 4; /* 'b' */
        Sprintf(buf, "%stake out, then put in", inokay ? "both; " : "");
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
                 buf, MENU_UNSELECTED);
    }
    if (inokay) 
    {
        any.a_int = 5; /* 'r' */
        Sprintf(buf, "%sput in, then take out",
                outokay ? "both reversed; " : "");
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
                 buf, MENU_UNSELECTED);
        any.a_int = 6; /* 's' */
        Sprintf(buf, "stash one item into %s", thesimpleoname(obj));
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
                 buf, MENU_UNSELECTED);
    }
    if (outokay && other_containter_count > 0)
    {
        any.a_int = 9; /* 'm' */
        char contbuf[BUFSZ] = "";
        Sprintf(contbuf, "%s container in the inventory", current_container->where == OBJ_INVENT ? "another" : "a");
        Sprintf(buf, "move %s to %s", something, ((0 && other_containter_count == 1 && last_container) ? thesimpleoname(last_container) : contbuf));
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
            buf, MENU_UNSELECTED);
    }
    if (outokay && floor_containter_count > 0)
    {
        any.a_int = 10; /* 'c' */
        char contbuf[BUFSZ] = "";
        Sprintf(contbuf, "%s container on the %s", current_container->where == OBJ_FLOOR ? "another" : "a",
            levl[u.ux][u.uy].typ == GRASS || levl[u.ux][u.uy].floortyp == GRASS ? "ground" : /* move to a container "on the grass" or "on the altar" might sound strange */
            levl[u.ux][u.uy].typ == GROUND || levl[u.ux][u.uy].floortyp == GROUND ? "ground" :
            "floor");
        Sprintf(buf, "move %s to %s", something, ((0 && other_containter_count == 1 && last_container) ? thesimpleoname(last_container) : contbuf));
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
            buf, MENU_UNSELECTED);
    }
    if (outokay && other_containter_count > 0 && !carried(obj))
    {
        any.a_int = 13; /* 'a' */
        Sprintf(buf, "take %s out and auto-stash it", something);
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
            buf, MENU_UNSELECTED);
    }
    if (outokay)
    {
        any.a_int = 11; /* 'd' */
        Sprintf(buf, "take %s out and drop it on the %s", something, IS_ALTAR(levl[u.ux][u.uy].typ) ? "altar" : 
            levl[u.ux][u.uy].typ == GRASS || levl[u.ux][u.uy].floortyp == GRASS ? "grass" : 
            levl[u.ux][u.uy].typ == GROUND || levl[u.ux][u.uy].floortyp == GROUND ? "ground" : 
            "floor");
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
            buf, MENU_UNSELECTED);
    }
    if (inokay && level.objects[u.ux][u.uy] && !(level.objects[u.ux][u.uy] == current_container && !level.objects[u.ux][u.uy]->nexthere))
    {
        any.a_int = 12; /* 'p' */
        Sprintf(buf, "pick %s up and put it in %s", something, thesimpleoname(current_container));
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
            buf, MENU_UNSELECTED);
    }
    any.a_int = 0;
    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_HALF_SIZE, NO_COLOR, " ", MENU_UNSELECTED);
    if (more_containers) {
        any.a_int = 7; /* 'n' */
        add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR,
                 "loot next container", MENU_SELECTED);
    }
    any.a_int = 8; /* 'q' */
    Strcpy(buf, alreadyused ? "done" : "do nothing");
    add_menu(win, NO_GLYPH, &any, menuselector[any.a_int], 0, ATR_NONE, NO_COLOR, buf,
             more_containers ? MENU_UNSELECTED : MENU_SELECTED);

    end_menu(win, prompt);
    n = select_menu(win, PICK_ONE, &pick_list);
    destroy_nhwindow(win);
    if (n > 0) {
        int k = pick_list[0].item.a_int;

        if (n > 1 && k == (more_containers ? 7 : 8))
            k = pick_list[1].item.a_int;
        free((genericptr_t) pick_list);
        return lootchars[k]; /* :,o,i,b,r,s,n,q,m,c,d,p */
    }
    return (n == 0 && more_containers) ? 'n' : 'q'; /* next or quit */
}

STATIC_VAR const char tippables[] = { ALL_CLASSES, TOOL_CLASS, 0 };

/* #tip command -- empty container contents onto floor */
int
dotip()
{
    struct obj *cobj, *nobj;
    coord cc;
    int boxes;
    char c, buf[BUFSZ], qbuf[BUFSZ];
    const char *spillage = 0;

    /*
     * doesn't require free hands;
     * limbs are needed to tip floor containers
     */

    /* at present, can only tip things at current spot, not adjacent ones */
    cc.x = u.ux, cc.y = u.uy;

    /* check floor container(s) first; at most one will be accessed */
    if (!getobj_autoselect_obj && (boxes = container_at(cc.x, cc.y, TRUE)) > 0) 
    {
        Sprintf(buf, "You can't tip %s while carrying so much.",
                !flags.verbose ? "a container" : (boxes > 1) ? "one" : "it");
        if (!check_capacity(buf) && able_to_loot(cc.x, cc.y, FALSE)) {
            if (boxes > 1 && (flags.menu_style != MENU_TRADITIONAL
                              || iflags.menu_requested)) {
                /* use menu to pick a container to tip */
                int n, i;
                winid win;
                anything any;
                menu_item *pick_list = (menu_item *) 0;
                struct obj dummyobj, *otmp;

                any = zeroany;
                win = create_nhwindow(NHW_MENU);
                start_menu_ex(win, GHMENU_STYLE_PICK_ITEM_LIST);

                for (cobj = level.objects[cc.x][cc.y], i = 0; cobj;
                     cobj = cobj->nexthere)
                    if (Is_container(cobj)) {
                        ++i;
                        any.a_obj = cobj;
                        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                                 doname(cobj), MENU_UNSELECTED);
                    }
                if (invent) {
                    any = zeroany;
                    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                             "", MENU_UNSELECTED);
                    any.a_obj = &dummyobj;
                    /* use 'i' for inventory unless there are so many
                       containers that it's already being used */
                    i = (i <= 'i' - 'a' && !flags.lootabc) ? 'i' : 0;
                    add_menu(win, NO_GLYPH, &any, i, 0, ATR_NONE, NO_COLOR,
                             "tip something being carried", MENU_SELECTED);
                }
                end_menu(win, "Tip which container?");
                n = select_menu(win, PICK_ONE, &pick_list);
                destroy_nhwindow(win);
                /*
                 * Deal with quirk of preselected item in pick-one menu:
                 * n ==  0 => picked preselected entry, toggling it off;
                 * n ==  1 => accepted preselected choice via SPACE or RETURN;
                 * n ==  2 => picked something other than preselected entry;
                 * n == -1 => cancelled via ESC;
                 */
                otmp = (n <= 0) ? (struct obj *) 0 : pick_list[0].item.a_obj;
                if (n > 1 && otmp == &dummyobj)
                    otmp = pick_list[1].item.a_obj;
                if (pick_list)
                    free((genericptr_t) pick_list);
                if (otmp && otmp != &dummyobj) {
                    tipcontainer(otmp);
                    return 1;
                }
                if (n == -1)
                    return 0;
                /* else pick-from-invent below */
            } else {
                for (cobj = level.objects[cc.x][cc.y]; cobj; cobj = nobj) {
                    nobj = cobj->nexthere;
                    if (!Is_container(cobj))
                        continue;
                    c = ynq(safe_qbuf(qbuf, "There is ", " here, tip it?",
                                      cobj,
                                      doname, ansimpleoname, "container"));
                    if (c == 'q')
                        return 0;
                    if (c == 'n')
                        continue;
                    tipcontainer(cobj);
                    /* can only tip one container at a time */
                    return 1;
                }
            }
        }
    }

    /* either no floor container(s) or couldn't tip one or didn't tip any */
    cobj = getobj(tippables, "tip", 0, "");
    if (!cobj)
        return 0;

    /* normal case */
    if (Is_container(cobj) || cobj->otyp == HORN_OF_PLENTY) 
    {
        char ynqbuf[BUFSZ];
        Sprintf(ynqbuf, "Are you sure to tip %s?", thecxname(cobj));
        if(!paranoid_query_ex(ATR_NONE, CLR_MSG_WARNING, ParanoidTip, "Tipping Container", ynqbuf))
            return 0;
        tipcontainer(cobj);
        return 1;
    }
    /* assorted other cases */
    if (is_candle(cobj) && cobj->lamplit) {
        /* note "wax" even for tallow candles to avoid giving away info */
        spillage = "wax";
    } else if ((cobj->otyp == POT_OIL && cobj->lamplit)
               || (cobj->otyp == OIL_LAMP && cobj->age != 0L)
               || (cobj->otyp == MAGIC_LAMP && cobj->special_quality == SPEQUAL_MAGIC_LAMP_CONTAINS_DJINN)) {
        spillage = "oil";
        /* todo: reduce potion's remaining burn timer or oil lamp's fuel */
    } else if (cobj->otyp == CAN_OF_GREASE && cobj->charges > 0) {
        /* charged consumed below */
        spillage = "grease";
    } else if (cobj->otyp == FOOD_RATION || cobj->otyp == CRAM_RATION
               || cobj->otyp == ELVEN_WAYBREAD) {
        spillage = "crumbs";
    } else if (cobj->oclass == VENOM_CLASS) {
        spillage = "venom";
    }
    if (spillage) {
        buf[0] = '\0';
        if (is_pool(u.ux, u.uy))
            Sprintf(buf, " and gradually %s", vtense(spillage, "dissipate"));
        else if (is_lava(u.ux, u.uy))
            Sprintf(buf, " and immediately %s away",
                    vtense(spillage, "burn"));
        pline("Some %s %s onto the %s%s.", spillage,
              vtense(spillage, "spill"), surface(u.ux, u.uy), buf);
        /* shop usage message comes after the spill message */
        if (cobj->otyp == CAN_OF_GREASE && cobj->charges > 0) {
            consume_obj_charge(cobj, TRUE);
        }
        /* something [useless] happened */
        return 1;
    }
    /* anything not covered yet */
    if (cobj->oclass == POTION_CLASS) /* can't pour potions... */
        pline_The("%s %s securely sealed.", xname(cobj), otense(cobj, "are"));
    else if (cobj->otyp == STATUE)
        pline("Nothing interesting happens.");
    else
        pline1(nothing_happens);
    return 0;
}

STATIC_OVL void
tipcontainer(box)
struct obj *box; /* or bag */
{
    xchar ox = u.ux, oy = u.uy; /* #tip only works at hero's location */
    boolean empty_it = FALSE, maybeshopgoods;

    /* box is either held or on floor at hero's spot; no need to check for
       nesting; when held, we need to update its location to match hero's;
       for floor, the coordinate updating is redundant */
    if (get_obj_location(box, &ox, &oy, 0))
        box->ox = ox, box->oy = oy;

    /* Shop handling:  can't rely on the container's own unpaid
       or no_charge status because contents might differ with it.
       A carried container's contents will be flagged as unpaid
       or not, as appropriate, and need no special handling here.
       Items owned by the hero get sold to the shop without
       confirmation as with other uncontrolled drops.  A floor
       container's contents will be marked no_charge if owned by
       hero, otherwise they're owned by the shop.  By passing
       the contents through shop billing, they end up getting
       treated the same as in the carried case.   We do so one
       item at a time instead of doing whole container at once
       to reduce the chance of exhausting shk's billing capacity. */
    maybeshopgoods = !carried(box) && costly_spot(box->ox, box->oy);

    /* caveat: this assumes that cknown, lknown, olocked, and otrapped
       fields haven't been overloaded to mean something special for the
       non-standard "container" horn of plenty */
    box->lknown = 1;
    if (box->olocked) {
        pline("It's locked.");
    } else if (box->otrapped) {
        /* we're not reaching inside but we're still handling it... */
        (void) chest_trap(box, HAND, FALSE);
        /* even if the trap fails, you've used up this turn */
        if (multi >= 0) { /* in case we didn't become paralyzed */
            nomul(-1);
            multi_reason = "tipping a container";
            nomovemsg = "";
        }
    } else if (box->otyp == BAG_OF_TRICKS || box->otyp == HORN_OF_PLENTY) {
        boolean bag = box->otyp == BAG_OF_TRICKS;
        int old_charges = box->charges, seen = 0;

        if (maybeshopgoods && !box->no_charge)
            addtobill(box, FALSE, FALSE, TRUE);
        /* apply this bag/horn until empty or monster/object creation fails
           (if the latter occurs, force the former...) */
        do {
            if (!(bag ? bagotricks(box, TRUE, &seen)
                      : hornoplenty(box, TRUE)))
                break;
        } while (box->charges > 0);

        if (box->charges < old_charges) {
            if (bag)
                pline((seen == 0) ? "Nothing seems to happen."
                                  : (seen == 1) ? "A monster appears."
                                                : "Monsters appear!");
            /* check_unpaid wants to see a non-zero charge count */
            box->charges = old_charges;
            check_unpaid_usage(box, TRUE);
            box->charges = 0; /* empty */
            box->cknown = 1;
        }
        if (maybeshopgoods && !box->no_charge)
            subfrombill(box, shop_keeper(*in_rooms(ox, oy, SHOPBASE)));
    } else if (SchroedingersBox(box)) {
        char yourbuf[BUFSZ];

        observe_quantum_cat(box, TRUE, TRUE);
        if (!Has_contained_contents(box)) /* evidently a live cat came out */
            /* container type of "large box" is inferred */
            pline("%sbox is now empty.", Shk_Your(yourbuf, box));
        else /* holds cat corpse */
            empty_it = TRUE;
        box->cknown = 1;
    } else if (!Has_contained_contents(box)) {
        box->cknown = 1;
        pline("It's empty.");
    } else {
        empty_it = TRUE;
    }

    if (empty_it && contained_object_chain(box) != 0) {
        struct obj *otmp, *nobj;
        boolean terse, highdrop = !can_reach_floor(TRUE),
                altarizing = IS_ALTAR(levl[ox][oy].typ),
                cursed_mbag = (Is_mbag(box) && box->cursed);
        int held = carried(box);
        int64_t loss = 0L;

        if (u.uswallow)
            highdrop = altarizing = FALSE;
        terse = !(highdrop || altarizing || costly_spot(box->ox, box->oy));
        box->cknown = 1;
        /* Terse formatting is
         * "Objects spill out: obj1, obj2, obj3, ..., objN."
         * If any other messages intervene between objects, we revert to
         * "ObjK drops to the floor.", "ObjL drops to the floor.", &c.
         */
        pline("%s out%c",
              contained_object_chain(box)->nobj ? "Objects spill" : "An object spills",
              terse ? ':' : '.');
        Strcpy(debug_buf_2, "tip_container");
        for (otmp = contained_object_chain(box); otmp; otmp = nobj) {
            nobj = otmp->nobj;
            obj_extract_self(otmp);
            otmp->ox = box->ox, otmp->oy = box->oy;

            if (box->otyp == ICE_BOX) {
                removed_from_icebox(otmp); /* resume rotting for corpse */
            } else if (cursed_mbag && !rn2(13)) {
                loss += mbag_item_gone(held, otmp);
                /* abbreviated drop format is no longer appropriate */
                terse = FALSE;
                continue;
            }

            if (maybeshopgoods) {
                addtobill(otmp, FALSE, FALSE, TRUE);
                iflags.suppress_price++; /* doname formatting */
            }

            if (highdrop) {
                /* might break or fall down stairs; handles altars itself */
                hitfloor(otmp, TRUE);
            } else {
                if (altarizing) {
                    doaltarobj(otmp);
                } else if (!terse) {
                    pline("%s %s to the %s.", Doname2(otmp),
                          otense(otmp, "drop"), surface(ox, oy));
                } else {
                    pline("%s%c", doname(otmp), nobj ? ',' : '.');
                    iflags.last_msg = PLNMSG_OBJNAM_ONLY;
                }
                dropyf(otmp);
                if (iflags.last_msg != PLNMSG_OBJNAM_ONLY)
                    terse = FALSE; /* terse formatting has been interrupted */
            }
            if (maybeshopgoods)
                iflags.suppress_price--; /* reset */
        }
        if (loss) /* magic bag lost some shop goods */
            You("owe %ld %s for lost merchandise.", loss, currency(loss));
        box->owt = weight(box); /* mbag_item_gone() doesn't update this */
        if (held)
            (void) encumber_msg();
    }
}

boolean
can_stash_objs()
{
    struct obj* otmp;
    for (otmp = invent; otmp; otmp = otmp->nobj)
    {
        if (Is_container(otmp) && !(otmp->dknown && objects[otmp->otyp].oc_name_known && (!Is_proper_container(otmp) || otmp->olocked)))
            return TRUE;
    }
    return FALSE;
}

boolean
can_floor_stash_objs()
{
    if (!can_reach_floor(TRUE))
        return FALSE;

    struct obj* otmp;
    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
    {
        if (Is_container(otmp) && !(otmp->dknown && objects[otmp->otyp].oc_name_known && (!Is_proper_container(otmp) || otmp->olocked)))
            return TRUE;
    }
    return FALSE;
}


STATIC_VAR struct obj dummy_container = { 0 };

void
set_current_container_to_dummyobj()
{
    current_container = &dummy_container;
}


void
set_current_container_to_null()
{
    current_container = 0;
}

/* the stash command */
int
dostash()
{
    struct obj* otmp = (struct obj*)0;

    if (!can_stash_objs()) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1(ATR_NONE, CLR_MSG_FAIL, "do not have any containers to stash items into.");
        return 0;
    }

    current_container = &dummy_container; /* dummy for getobj with stash */
    otmp = getobj(getobj_stash_objs, "stash", 0, "");
    current_container = 0;
    if (!otmp)
    {
        pline1(Never_mind);
        return 0;
    }

    if (Is_container(otmp)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1(ATR_NONE, CLR_MSG_FAIL, "cannot stash that.");
        return 0;
    }

    struct obj* container = select_other_container(invent, (struct obj*)0, FALSE);
    if (!container)
    {
        pline1(Never_mind);
        return 0;
    }

    if (!stash_obj_in_container(otmp, container))
    {
        /* couldn't put selected item into container for some
           reason; might need to undo splitobj() */
        (void)unsplitobj(otmp);
    }
    return 1;
}

/* the stashfloor command */
int
dostashfloor()
{
    struct obj* otmp = (struct obj*)0;

    if (!can_floor_stash_objs()) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1(ATR_NONE, CLR_MSG_FAIL, !can_reach_floor(TRUE) ? "cannot reach any containers on the floor to stash items into." : "do not know of any containers on the floor to stash items into.");
        return 0;
    }

    current_container = &dummy_container; /* dummy for getobj with stash */
    otmp = getobj(getobj_stash_objs, "stash", 0, "");
    current_container = 0;
    if (!otmp)
    {
        pline1(Never_mind);
        return 0;
    }

    if (Is_container(otmp)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1(ATR_NONE, CLR_MSG_FAIL, "cannot stash that.");
        return 0;
    }

    struct obj* container = select_other_container(level.objects[u.ux][u.uy], (struct obj*)0, TRUE);
    if (!container)
    {
        pline1(Never_mind);
        return 0;
    }

    if (!stash_obj_in_container(otmp, container))
    {
        /* couldn't put selected item into container for some
           reason; might need to undo splitobj() */
        (void)unsplitobj(otmp);
    }
    return 1;
}

void
reset_pickup(VOID_ARGS)
{
    current_container = 0;
    move_target_container = 0;
    abort_looting = FALSE;
    class_filter = bucx_filter = shop_filter = unidentified_filter = unknown_filter = FALSE;
}
/*pickup.c*/

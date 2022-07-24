/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    invent.c    $NHDT-Date: 1555196229 2019/04/13 22:57:09 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.253 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Derek S. Ray, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "func_tab.h"

#ifndef M
#define M(c) ((char) (0x80 | (c)))
#endif
#ifndef C /* same as cmd.c */
#define C(c) (0x40 & (c) ? 0x1f & (c) : (0x80 | (0x1f & (c))))
#endif

#define NOINVSYM '#'
#define CONTAINED_SYM '>' /* designator for inside a container */
#define HANDS_SYM '-'

STATIC_DCL void FDECL(loot_classify, (Loot *, struct obj *));
STATIC_DCL char *FDECL(loot_xname, (struct obj *));
STATIC_DCL int FDECL(CFDECLSPEC sortloot_cmp, (const genericptr,
                                               const genericptr));
STATIC_DCL void NDECL(reorder_invent);
STATIC_DCL void FDECL(noarmor, (BOOLEAN_P));
STATIC_DCL void FDECL(invdisp_nothing, (const char *, const char *));
STATIC_DCL boolean FDECL(worn_wield_only, (struct obj *));
STATIC_DCL boolean FDECL(only_here, (struct obj *));
STATIC_DCL void FDECL(compactify, (char *));
STATIC_DCL boolean FDECL(taking_off, (const char *));
STATIC_DCL boolean FDECL(putting_on, (const char *));
STATIC_DCL boolean FDECL(trading_items, (const char*));
STATIC_PTR int FDECL(ckvalidcat, (struct obj *));
STATIC_PTR int FDECL(ckunpaid, (struct obj *));
STATIC_PTR char *FDECL(safeq_xprname, (struct obj *));
STATIC_PTR char *FDECL(safeq_shortxprname, (struct obj *));
STATIC_DCL char FDECL(display_pickinv, (const char *, char *, char *, BOOLEAN_P, long *, int, const char*, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL char FDECL(display_used_invlets, (CHAR_P));
STATIC_DCL boolean FDECL(this_type_only, (struct obj *));
STATIC_DCL void NDECL(dounpaid);
STATIC_DCL struct obj *FDECL(find_unpaid, (struct obj *, struct obj **));
STATIC_DCL int FDECL(menu_identify, (int));
STATIC_DCL boolean FDECL(tool_in_use, (struct obj *));
STATIC_DCL char FDECL(obj_to_let, (struct obj *));

static int lastinvnr = 51; /* 0 ... 51 (never saved&restored) */

/* wizards can wish for venom, which will become an invisible inventory
 * item without this.  putting it in inv_order would mean venom would
 * suddenly become a choice for all the inventory-class commands, which
 * would probably cause mass confusion.  the test for inventory venom
 * is only WIZARD and not wizard because the wizard can leave venom lying
 * around on a bones level for normal players to find.  [Note to the
 * confused:  'WIZARD' used to be a compile-time conditional so this was
 * guarded by #ifdef WIZARD/.../#endif.]
 */
static char venom_inv[] = { VENOM_CLASS, 0 }; /* (constant) */

/* sortloot() classification; called at most once [per sort] for each object */
STATIC_OVL void
loot_classify(sort_item, obj)
Loot *sort_item;
struct obj *obj;
{
    /* we may eventually make this a settable option to always use
       with sortloot instead of only when the 'sortpack' option isn't
       set; it is similar to sortpack's inv_order but items most
       likely to be picked up are moved to the front */
    static char def_srt_order[MAX_OBJECT_CLASSES] = {
        COIN_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, RING_CLASS, WAND_CLASS, POTION_CLASS,
        SCROLL_CLASS, SPBOOK_CLASS, GEM_CLASS, FOOD_CLASS, REAGENT_CLASS, TOOL_CLASS,
        WEAPON_CLASS, ARMOR_CLASS, ROCK_CLASS, BALL_CLASS, CHAIN_CLASS, 0,
    };
    static char armcat[10];
    const char *classorder;
    char *p;
    int k, otyp = obj->otyp, oclass = obj->oclass;
    boolean seen, discovered = objects[otyp].oc_name_known ? TRUE : FALSE;

    /*
     * For the value types assigned by this classification, sortloot()
     * will put lower valued ones before higher valued ones.
     */
    if (!Blind)
        obj->dknown = 1; /* xname(obj) does this; we want it sooner */
    seen = obj->dknown ? TRUE : FALSE,
    /* class order */
    classorder = flags.sortpack ? flags.inv_order : def_srt_order;
    p = index(classorder, oclass);
    if (p)
        k = 1 + (int) (p - classorder);
    else
        k = 1 + (int) strlen(classorder) + (oclass != VENOM_CLASS);
    sort_item->orderclass = (xchar) k;
    /* subclass designation; only a few classes have subclasses
       and the non-armor ones we use are fairly arbitrary */
    switch (oclass) {
    case ARMOR_CLASS:
        if (!armcat[9]) {
            /* one-time init; we use a different order than the subclass
               values defined by objclass.h */
            armcat[ARM_HELM]   = 1; /* [2] */
            armcat[ARM_GLOVES] = 2; /* [3] */
            armcat[ARM_BOOTS]  = 3; /* [4] */
            armcat[ARM_SHIELD] = 4; /* [1] */
            armcat[ARM_CLOAK]  = 5; /* [5] */
            armcat[ARM_SHIRT]  = 6; /* [6] */
            armcat[ARM_ROBE]   = 7; /* [7] */
            armcat[ARM_BRACERS]= 8; /* [8] */
            armcat[ARM_SUIT]   = 9; /* [0] */
            armcat[9]          = 10; /* sanity protection */
        }
        k = objects[otyp].oc_armor_category;
        /* oc_armor_category overloads oc_subtyp which is an 'schar' so guard
           against somebody assigning something unexpected to it */
        if (k < 0 || k >= 9)
            k = 9;
        k = armcat[k];
        break;
    case WEAPON_CLASS:
        /* for weapons, group by ammo (arrows, bolts), launcher (bows),
           missile (darts, boomerangs), stackable (daggers, knives, spears),
           'other' (swords, axes, &c), polearms */
        k = objects[otyp].oc_skill;
        k = (k < 0) ? ((k >= -P_CROSSBOW && k <= -P_BOW) ? 1 : 3)
                    : ((k >= P_BOW && k <= P_CROSSBOW) ? 2
                       : (k == P_SPEAR || k == P_DAGGER) ? 4
                          : !is_pole(obj) && !is_lance(obj) ? 5 : 6);
        break;
    case TOOL_CLASS:
        if (seen && discovered
            && (Is_noncontainer(obj) /*otyp == BAG_OF_TRICKS*/ || otyp == HORN_OF_PLENTY))
            k = 2; /* known pseudo-container */
        else if (Is_container(obj))
            k = 1; /* regular container or unknown bag of tricks */
        else
        {
            switch (otyp) {
            case WOODEN_FLUTE:
            case MAGIC_FLUTE:
            case BRASS_HORN:
            case TOOLED_HORN:
            case FROST_HORN:
            case FIRE_HORN:
            case HORN_OF_CHAOS:
            case WOODEN_HARP:
            case MAGIC_HARP:
            case BUGLE:
            case LEATHER_DRUM:
            case DRUM_OF_EARTHQUAKE:
            case HORN_OF_PLENTY: /* not a musical instrument */
                k = 3; /* instrument or unknown horn of plenty */
                break;
            default:
                k = 4; /* 'other' tool */
                break;
            }
        }
        break;
    case FOOD_CLASS:
        /* [what about separating "partly eaten" within each group?] */
        switch (otyp) {
        case SLIME_MOLD:
            k = 1;
            break;
        default:
            /* [maybe separate one-bite foods from rations and such?] */
            k = obj->globby ? 6 : 2;
            break;
        case TIN:
            k = 3;
            break;
        case EGG:
            k = 4;
            break;
        case CORPSE:
            k = 5;
            break;
        }
        break;
    case GEM_CLASS:
        /*
         * Normally subclass takes priority over discovery status, but
         * that would give away information for gems (assuming we'll
         * group them as valuable gems, next glass, then gray stones,
         * and finally rocks once they're all fully identified).
         *
         * Order:
         *  1) unseen gems and glass ("gem")
         *  2) seen but undiscovered gems and glass ("blue gem"),
         *  3) discovered gems ("sapphire"),
         *  4) discovered glass ("worthless pieced of blue glass"),
         *  5) unseen gray stones and rocks ("stone"),
         *  6) seen but undiscovered gray stones ("gray stone"),
         *  7) discovered gray stones ("touchstone"),
         *  8) seen rocks ("rock").
         */
        switch (objects[obj->otyp].oc_material) {
        case MAT_HARD_CRYSTAL:
        case MAT_GEMSTONE:
            k = !seen ? 1 : !discovered ? 2 : 3;
            break;
        case MAT_CRYSTAL:
        case MAT_GLASS:
            k = !seen ? 1 : !discovered ? 2 : 4;
            break;
        default: /* MAT_MINERAL */
            k = !seen ? 5 : (obj->otyp != ROCK) ? (!discovered ? 6 : 7) : 8;
            break;
        }
        break;
    default:
        /* other classes don't have subclasses; we assign a nonzero
           value because sortloot() uses 0 to mean 'not yet classified' */
        k = 1; /* any non-zero would do */
        break;
    }
    sort_item->subclass = (xchar) k;
    /* discovery status */
    k = !seen ? 1 /* unseen */
        : (discovered || !OBJ_DESCR(objects[otyp])) ? 4
          : (objects[otyp].oc_uname) ? 3 /* named (partially discovered) */
            : 2; /* undiscovered */
    sort_item->disco = (xchar) k;
}

/* sortloot() formatting routine; for alphabetizing, not shown to user */
STATIC_OVL char *
loot_xname(obj)
struct obj *obj;
{
    struct obj saveo;
    boolean save_debug;
    char *res, /* *save_oname,*/ * save_uoname;

    /*
     * Deal with things that xname() includes as a prefix.  We don't
     * want such because they change alphabetical ordering.  First,
     * remember 'obj's current settings.
     */
    saveo.odiluted = obj->odiluted;
    saveo.blessed = obj->blessed, saveo.cursed = obj->cursed;
    saveo.enchantment = obj->enchantment;
    saveo.special_quality = obj->special_quality;
    saveo.charges = obj->charges;
    saveo.speflags = obj->speflags;
    saveo.owt = obj->owt;
    //save_oname = has_oname(obj) ? ONAME(obj) : 0;
    save_uoname = has_uoname(obj) ? UONAME(obj) : 0;
    save_debug = flags.debug;
    /* suppress "diluted" for potions and "holy/unholy" for water;
       sortloot() will deal with them using other criteria than name */
    if (obj->oclass == POTION_CLASS) {
        obj->odiluted = 0;
        if (obj->otyp == POT_WATER)
            obj->blessed = 0, obj->cursed = 0;
    }
    /* make "wet towel" and "moist towel" format as "towel" so that all
       three group together */
    if (obj->otyp == TOWEL)
        obj->special_quality = 0;
    /* group "<size> glob of <foo>" by <foo> rather than by <size> */
    if (obj->globby)
        obj->owt = 200; /* 200: weight of combined glob from ten creatures
                           (five or fewer is "small", more than fifteen is
                           "large", in between has no prefix) */
    /* suppress user-assigned name */
    //if (save_oname && !obj->oartifact)
    //    ONAME(obj) = 0;
    if (save_uoname)
        UONAME(obj) = 0;

    /* avoid wizard mode formatting variations */
    if (wizard) { /* flags.debug */
        /* paranoia:  before toggling off wizard mode, guard against a
           panic in xname() producing a normal mode panic save file */
        program_state.something_worth_saving = 0;
        flags.debug = FALSE;
    }

    res = cxname_singular(obj);

    if (save_debug) {
        flags.debug = TRUE;
        program_state.something_worth_saving = 1;
    }
    /* restore the object */
    if (obj->oclass == POTION_CLASS) {
        obj->odiluted = saveo.odiluted;
        if (obj->otyp == POT_WATER)
            obj->blessed = saveo.blessed, obj->cursed = saveo.cursed;
    }
    if (obj->otyp == TOWEL) {
        obj->special_quality = saveo.special_quality;
        /* give "towel" a suffix that will force wet ones to come first,
           moist ones next, and dry ones last regardless of whether
           they've been flagged as having enchantment known */
        Strcat(res, is_wet_towel(obj) ? ((obj->special_quality >= 3) ? "x" : "y") : "z");
    }
    if (obj->globby) {
        obj->owt = saveo.owt;
        /* we've suppressed the size prefix (above); there normally won't
           be more than one of a given creature type because they coalesce,
           but globs with different bless/curse state won't merge so it is
           feasible to have multiple at the same location; add a suffix to
           get such sorted by size (small first) */
        Strcat(res, (obj->owt <= 100) ? "a"
                    : (obj->owt <= 300) ? "b"
                      : (obj->owt <= 500) ? "c"
                        : "d");
    }
    //if (save_oname && !obj->oartifact)
    //    ONAME(obj) = save_oname;
    if (save_uoname)
        UONAME(obj) = save_uoname;

    return res;
}

/* set by sortloot() for use by sortloot_cmp(); reset by sortloot when done */
static unsigned sortlootmode = 0;

/* qsort comparison routine for sortloot() */
STATIC_OVL int CFDECLSPEC
sortloot_cmp(vptr1, vptr2)
const genericptr vptr1;
const genericptr vptr2;
{
    struct sortloot_item *sli1 = (struct sortloot_item *) vptr1,
                         *sli2 = (struct sortloot_item *) vptr2;
    struct obj *obj1 = sli1->obj,
               *obj2 = sli2->obj;
    char *nam1, *nam2;
    int val1, val2, c, namcmp;

    /* order by object class unless we're doing by-invlet without sortpack */
    if ((sortlootmode & (SORTLOOT_PACK | SORTLOOT_INVLET))
        != SORTLOOT_INVLET) {
        /* Classify each object at most once no matter how many
           comparisons it is involved in. */
        if (!sli1->orderclass)
            loot_classify(sli1, obj1);
        if (!sli2->orderclass)
            loot_classify(sli2, obj2);

        /* Sort by class. */
        val1 = sli1->orderclass;
        val2 = sli2->orderclass;
        if (val1 != val2)
            return (int) (val1 - val2);

        /* skip sub-classes when ordering by sortpack+invlet */
        if ((sortlootmode & SORTLOOT_INVLET) == 0) {
            /* Class matches; sort by subclass. */
            val1 = sli1->subclass;
            val2 = sli2->subclass;
            if (val1 != val2)
                return val1 - val2;

            /* Class and subclass match; sort by discovery status:
             * first unseen, then seen but not named or discovered,
             * then named, lastly discovered.
             * 1) potion
             * 2) pink potion
             * 3) dark green potion called confusion
             * 4) potion of healing
             * Multiple entries within each group will be put into
             * alphabetical order below.
             */
            val1 = sli1->disco;
            val2 = sli2->disco;
            if (val1 != val2)
                return val1 - val2;
        }
    }

    /* order by assigned inventory letter */
    if ((sortlootmode & SORTLOOT_INVLET) != 0) {
        c = obj1->invlet;
        val1 = ('a' <= c && c <= 'z') ? (c - 'a' + 2)
               : ('A' <= c && c <= 'Z') ? (c - 'A' + 2 + 26)
                 : (c == '$') ? 1
                   : (c == '#') ? 1 + 52 + 1
                     : 1 + 52 + 1 + 1; /* none of the above */
        c = obj2->invlet;
        val2 = ('a' <= c && c <= 'z') ? (c - 'a' + 2)
               : ('A' <= c && c <= 'Z') ? (c - 'A' + 2 + 26)
                 : (c == '$') ? 1
                   : (c == '#') ? 1 + 52 + 1
                     : 1 + 52 + 1 + 1; /* none of the above */
        if (val1 != val2)
            return val1 - val2;
    }

    if ((sortlootmode & SORTLOOT_LOOT) == 0)
        goto tiebreak;

    /*
     * Sort object names in lexicographical order, ignoring quantity.
     *
     * Each obj gets formatted at most once (per sort) no matter how many
     * comparisons it gets subjected to.
     */
    nam1 = sli1->str;
    if (!nam1)
        nam1 = sli1->str = dupstr(loot_xname(obj1));
    nam2 = sli2->str;
    if (!nam2)
        nam2 = sli2->str = dupstr(loot_xname(obj2));
    if ((namcmp = strcmpi(nam1, nam2)) != 0)
        return namcmp;

    /* Sort by BUCX. */
    val1 = obj1->bknown ? (obj1->blessed ? 3 : !obj1->cursed ? 2 : 1) : 0;
    val2 = obj2->bknown ? (obj2->blessed ? 3 : !obj2->cursed ? 2 : 1) : 0;
    if (val1 != val2)
        return val2 - val1; /* bigger is better */

    /* Sort by greasing.  This will put the objects in degreasing order. */
    val1 = obj1->greased;
    val2 = obj2->greased;
    if (val1 != val2)
        return val2 - val1; /* bigger is better */

    /* Sort by erosion.  The effective amount is what matters. */
    val1 = greatest_erosion(obj1);
    val2 = greatest_erosion(obj2);
    if (val1 != val2)
        return val1 - val2; /* bigger is WORSE */

    /* Sort by erodeproofing.  Map known-invulnerable to 1, and both
       known-vulnerable and unknown-vulnerability to 0, because that's
       how they're displayed. */
    val1 = obj1->rknown && obj1->oerodeproof;
    val2 = obj2->rknown && obj2->oerodeproof;
    if (val1 != val2)
        return val2 - val1; /* bigger is better */

    /* Sort by enchantment.  Map unknown to -1000, which is comfortably
       below the range of obj->enchantment.  oc_uses_known means that obj->known
       matters, which usually indirectly means that obj->enchantment is relevant.
       Lots of objects use obj->enchantment for some other purpose (see obj.h). */
    if (objects[obj1->otyp].oc_uses_known
        /* exclude eggs (laid by you) and tins (homemade, pureed, &c) */
        && obj1->oclass != FOOD_CLASS) {
        val1 = obj1->known ? obj1->enchantment : -1000;
        val2 = obj2->known ? obj2->enchantment : -1000;
        if (val1 != val2)
            return val2 - val1; /* bigger is better */
    }

 tiebreak:
    /* They're identical, as far as we're concerned.  We want
       to force a deterministic order, and do so by producing a
       stable sort: maintain the original order of equal items. */
    return (sli1->indx - sli2->indx);
}

/*
 * sortloot() - the story so far...
 *
 *      The original implementation constructed and returned an array
 *      of pointers to objects in the requested order.  Callers had to
 *      count the number of objects, allocate the array, pass one
 *      object at a time to the routine which populates it, traverse
 *      the objects via stepping through the array, then free the
 *      array.  The ordering process used a basic insertion sort which
 *      is fine for short lists but inefficient for long ones.
 *
 *      3.6.0 (and continuing with 3.6.1) changed all that so that
 *      sortloot was self-contained as far as callers were concerned.
 *      It reordered the linked list into the requested order and then
 *      normal list traversal was used to process it.  It also switched
 *      to qsort() on the assumption that the C library implementation
 *      put some effort into sorting efficiently.  It also checked
 *      whether the list was already sorted as it got ready to do the
 *      sorting, so re-examining inventory or a pile of objects without
 *      having changed anything would gobble up less CPU than a full
 *      sort.  But it had at least two problems (aside from the ordinary
 *      complement of bugs):
 *      1) some players wanted to get the original order back when they
 *      changed the 'sortloot' option back to 'none', but the list
 *      reordering made that infeasible;
 *      2) object identification giving the 'ID whole pack' result
 *      would call makeknown() on each newly ID'd object, that would
 *      call update_inventory() to update the persistent inventory
 *      window if one existed, the interface would call the inventory
 *      display routine which would call sortloot() which might change
 *      the order of the list being traversed by the identify code,
 *      possibly skipping the ID of some objects.  That could have been
 *      avoided by suppressing 'perm_invent' during identification
 *      (fragile) or by avoiding sortloot() during inventory display
 *      (more robust).
 *
 *      3.6.2 reverts to the temporary array of ordered obj pointers
 *      but has sortloot() do the counting and allocation.  Callers
 *      need to use array traversal instead of linked list traversal
 *      and need to free the temporary array when done.  And the
 *      array contains 'struct sortloot_item' (aka 'Loot') entries
 *      instead of simple 'struct obj *' entries.
 */
Loot *
sortloot(olist, mode, by_nexthere, filterfunc)
struct obj **olist; /* previous version might have changed *olist, we don't */
unsigned mode; /* flags for sortloot_cmp() */
boolean by_nexthere; /* T: traverse via obj->nexthere, F: via obj->nobj */
boolean FDECL((*filterfunc), (OBJ_P));
{
    Loot *sliarray;
    struct obj *o;
    unsigned n, i;
    boolean augment_filter;

    for (n = 0, o = *olist; o; o = by_nexthere ? o->nexthere : o->nobj)
        ++n;
    /* note: if there is a filter function, this might overallocate */
    sliarray = (Loot *) alloc(((size_t)n + 1) * sizeof *sliarray);

    /* the 'keep cockatrice corpses' flag is overloaded with sort mode */
    augment_filter = (mode & SORTLOOT_PETRIFY) ? TRUE : FALSE;
    mode &= ~SORTLOOT_PETRIFY; /* remove flag, leaving mode */
    /* populate aliarray[0..n-1] */
    for (i = 0, o = *olist; o; o = by_nexthere ? o->nexthere : o->nobj) {
        if (filterfunc && !(*filterfunc)(o)
            /* caller may be asking us to override filterfunc (in order
               to do a cockatrice corpse touch check during pickup even
               if/when the filter rejects food class) */
            && (!augment_filter || o->otyp != CORPSE || o->corpsenm < LOW_PM
                || !touch_petrifies(&mons[o->corpsenm])))
            continue;
        sliarray[i].obj = o, sliarray[i].indx = (int) i;
        sliarray[i].str = (char *) 0;
        sliarray[i].orderclass = sliarray[i].subclass = sliarray[i].disco = 0;
        ++i;
    }
    n = i;
    /* add a terminator so that we don't have to pass 'n' back to caller */
    sliarray[n].obj = (struct obj *) 0, sliarray[n].indx = -1;
    sliarray[n].str = (char *) 0;
    sliarray[n].orderclass = sliarray[n].subclass = sliarray[n].disco = 0;

    /* do the sort; if no sorting is requested, we'll just return
       a sortloot_item array reflecting the current ordering */
    if (mode && n > 1) {
        sortlootmode = mode; /* extra input for sortloot_cmp() */
        qsort((genericptr_t) sliarray, n, sizeof *sliarray, sortloot_cmp);
        sortlootmode = 0; /* reset static mode flags */
        /* if sortloot_cmp formatted any objects, discard their strings now */
        for (i = 0; i < n; ++i)
            if (sliarray[i].str)
                free((genericptr_t) sliarray[i].str), sliarray[i].str = 0;
    }
    return sliarray;
}

/* sortloot() callers should use this to free up memory it allocates */
void
unsortloot(loot_array_p)
Loot **loot_array_p;
{
    if (*loot_array_p)
        free((genericptr_t) *loot_array_p), *loot_array_p = (Loot *) 0;
}

#if 0 /* 3.6.0 'revamp' */
void
sortloot(olist, mode, by_nexthere)
struct obj **olist;
unsigned mode; /* flags for sortloot_cmp() */
boolean by_nexthere; /* T: traverse via obj->nexthere, F: via obj->nobj */
{
    struct sortloot_item *sliarray, osli, nsli;
    struct obj *o, **nxt_p;
    unsigned n, i;
    boolean already_sorted = TRUE;

    sortlootmode = mode; /* extra input for sortloot_cmp() */
    for (n = osli.indx = 0, osli.obj = *olist; (o = osli.obj) != 0;
         osli = nsli) {
        nsli.obj = by_nexthere ? o->nexthere : o->nobj;
        nsli.indx = (int) ++n;
        if (nsli.obj && already_sorted
            && sortloot_cmp((genericptr_t) &osli, (genericptr_t) &nsli) > 0)
            already_sorted = FALSE;
    }
    if (n > 1 && !already_sorted) {
        sliarray = (struct sortloot_item *) alloc(n * sizeof *sliarray);
        for (i = 0, o = *olist; o;
             ++i, o = by_nexthere ? o->nexthere : o->nobj)
            sliarray[i].obj = o, sliarray[i].indx = (int) i;

        qsort((genericptr_t) sliarray, n, sizeof *sliarray, sortloot_cmp);
        for (i = 0; i < n; ++i) {
            o = sliarray[i].obj;
            nxt_p = by_nexthere ? &(o->nexthere) : &(o->nobj);
            *nxt_p = (i < n - 1) ? sliarray[i + 1].obj : (struct obj *) 0;
        }
        *olist = sliarray[0].obj;
        free((genericptr_t) sliarray);
    }
    sortlootmode = 0;
}
#endif /*0*/

void
assigninvlet(otmp)
register struct obj *otmp;
{
    boolean inuse[52];
    register int i;
    register struct obj *obj;

    /* there should be at most one of these in inventory... */
    if (otmp->oclass == COIN_CLASS) {
        otmp->invlet = GOLD_SYM;
        return;
    }

    for (i = 0; i < 52; i++)
        inuse[i] = FALSE;
    for (obj = invent; obj; obj = obj->nobj)
        if (obj != otmp) {
            i = obj->invlet;
            if ('a' <= i && i <= 'z')
                inuse[i - 'a'] = TRUE;
            else if ('A' <= i && i <= 'Z')
                inuse[i - 'A' + 26] = TRUE;
            if (i == otmp->invlet)
                otmp->invlet = 0;
        }
    if ((i = otmp->invlet)
        && (('a' <= i && i <= 'z') || ('A' <= i && i <= 'Z')))
        return;
    for (i = lastinvnr + 1; i != lastinvnr; i++) {
        if (i == 52) {
            i = -1;
            continue;
        }
        if (!inuse[i])
            break;
    }
    otmp->invlet =
        (inuse[i] ? NOINVSYM : (i < 26) ? ('a' + i) : ('A' + i - 26));
    lastinvnr = i;
}

/* note: assumes ASCII; toggling a bit puts lowercase in front of uppercase */
#define inv_rank(o) ((o)->invlet ^ 040)

/* sort the inventory; used by addinv() and doorganize() */
STATIC_OVL void
reorder_invent()
{
    struct obj *otmp, *prev, *next;
    boolean need_more_sorting;

    do {
        /*
         * We expect at most one item to be out of order, so this
         * isn't nearly as inefficient as it may first appear.
         */
        need_more_sorting = FALSE;
        for (otmp = invent, prev = 0; otmp;) {
            next = otmp->nobj;
            if (next && inv_rank(next) < inv_rank(otmp)) {
                need_more_sorting = TRUE;
                if (prev)
                    prev->nobj = next;
                else
                    invent = next;
                otmp->nobj = next->nobj;
                next->nobj = otmp;
                prev = next;
            } else {
                prev = otmp;
                otmp = next;
            }
        }
    } while (need_more_sorting);
}

#undef inv_rank

/* scan a list of objects to see whether another object will merge with
   one of them; used in pickup.c when all 52 inventory slots are in use,
   to figure out whether another object could still be picked up */
struct obj *
merge_choice(objlist, obj)
struct obj *objlist, *obj;
{
    struct monst *shkp;
    int save_nocharge;

    if (obj->otyp == SCR_SCARE_MONSTER) /* punt on these */
        return (struct obj *) 0;
    /* if this is an item on the shop floor, the attributes it will
       have when carried are different from what they are now; prevent
       that from eliciting an incorrect result from mergable() */
    save_nocharge = obj->no_charge;
    if (objlist == invent && obj->where == OBJ_FLOOR
        && (shkp = shop_keeper(inside_shop(obj->ox, obj->oy))) != 0) {
        if (obj->no_charge)
            obj->no_charge = 0;
        /* A billable object won't have its `unpaid' bit set, so would
           erroneously seem to be a candidate to merge with a similar
           ordinary object.  That's no good, because once it's really
           picked up, it won't merge after all.  It might merge with
           another unpaid object, but we can't check that here (depends
           too much upon shk's bill) and if it doesn't merge it would
           end up in the '#' overflow inventory slot, so reject it now. */
        else if (inhishop(shkp))
            return (struct obj *) 0;
    }
    while (objlist) {
        if (mergable(objlist, obj))
            break;
        objlist = objlist->nobj;
    }
    obj->no_charge = save_nocharge;
    return objlist;
}

/* merge obj with otmp and delete obj if types agree */
int
merged(potmp, pobj)
struct obj **potmp, **pobj;
{
    register struct obj *otmp = *potmp, *obj = *pobj;

    if (mergable(otmp, obj)) {
        /* Approximate age: we do it this way because if we were to
         * do it "accurately" (merge only when ages are identical)
         * we'd wind up never merging any corpses.
         * otmp->age = otmp->age*(1-proportion) + obj->age*proportion;
         *
         * Don't do the age manipulation if lit.  We would need
         * to stop the burn on both items, then merge the age,
         * then restart the burn.  Glob ages are averaged in the
         * absorb routine, which uses weight rather than quantity
         * to adjust for proportion (glob quantity is always 1).
         */
        if (!obj->lamplit && !obj->globby)
            otmp->age = ((otmp->age * otmp->quan) + (obj->age * obj->quan))
                        / (otmp->quan + obj->quan);

        otmp->quan += obj->quan;
        /* temporary special case for gold objects!!!! */
        if (otmp->oclass == COIN_CLASS)
            otmp->owt = weight(otmp), otmp->bknown = 0;
        /* and puddings!!!1!!one! */
        else if (!Is_pudding(otmp))
            otmp->owt += obj->owt;
        if (!has_uoname(otmp) && has_uoname(obj))
            otmp = *potmp = uoname(otmp, UONAME(obj));
        if (!has_oname(otmp) && has_oname(obj))
            otmp = *potmp = oname(otmp, ONAME(obj));
        obj_extract_self(obj);

        /* really should merge the timeouts */
        if (obj->lamplit)
            obj_merge_light_sources(obj, otmp);
        if (obj->makingsound)
            obj_merge_sound_sources(obj, otmp);
        if (obj->timed)
            obj_stop_timers(obj); /* follows lights */

        /* fixup for `#adjust' merging wielded darts, daggers, &c */
        if (obj->owornmask && carried(otmp)) {
            long wmask = otmp->owornmask | obj->owornmask;

            /* Both the items might be worn in competing slots;
               merger preference (regardless of which is which):
             primary weapon + alternate weapon -> primary weapon;
             primary weapon + quiver -> primary weapon;
             alternate weapon + quiver -> alternate weapon.
               (Prior to 3.3.0, it was not possible for the two
               stacks to be worn in different slots and `obj'
               didn't need to be unworn when merging.) */
            if (wmask & W_WEP)
                wmask = W_WEP;
            else if (wmask & W_SWAPWEP)
                wmask = W_SWAPWEP;
            else if (wmask & W_QUIVER)
                wmask = W_QUIVER;
            else {
                impossible("merging strangely worn items (%lx)", wmask);
                wmask = otmp->owornmask;
            }
            if ((otmp->owornmask & ~wmask) != 0L)
                setnotworn(otmp);
            setworn(otmp, wmask);
            setnotworn(obj);
#if 0
        /* (this should not be necessary, since items
            already in a monster's inventory don't ever get
            merged into other objects [only vice versa]) */
        } else if (obj->owornmask && mcarried(otmp)) {
            if (obj == MON_WEP(otmp->ocarry)) {
                MON_WEP(otmp->ocarry) = otmp;
                otmp->owornmask = W_WEP;
            }
#endif /*0*/
        }

        /* handle puddings a bit differently; absorption will free the
           other object automatically so we can just return out from here */
        if (obj->globby) {
            pudding_merge_message(otmp, obj);
            obj_absorb(potmp, pobj);
            return 1;
        }

        obfree(obj, otmp); /* free(obj), bill->otmp */
        return 1;
    }
    return 0;
}

/*
 * Adjust hero intrinsics as if this object was being added to the hero's
 * inventory.  Called _before_ the object has been added to the hero's
 * inventory.
 *
 * This is called when adding objects to the hero's inventory normally (via
 * addinv) or when an object in the hero's inventory has been polymorphed
 * in-place.
 *
 * It may be valid to merge this code with with addinv_core2().
 */
void
addinv_core1(obj)
struct obj *obj;
{
    if (obj->oclass == COIN_CLASS) 
    {
        context.botl = 1;
    } 
    else if (obj->otyp == AMULET_OF_YENDOR) 
    {
        if (u.uhave.amulet)
            impossible("already have amulet?");
        u.uhave.amulet = 1;
        if (flags.showscore && !u.uachieve.amulet)
            context.botl = 1;
        if (!u.uachieve.amulet)
        {
            achievement_gained("Amulet of Yendor");
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(500);
                play_sfx_sound(SFX_HINT);
            }
            pline_ex(ATR_NONE, CLR_MSG_HINT, "QUEST UPDATE - Sacrifice the Amulet of Yendor on the Astral Plane");
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(500);
                play_sfx_sound(SFX_HINT);
            }
            pline_ex(ATR_NONE, CLR_MSG_HINT, "HINT - Exit the dungeon on level 1 to enter the Elemental Planes");
        }
        u.uachieve.amulet = 1;
    }
    else if (obj->otyp == CANDELABRUM_OF_INVOCATION) 
    {
        if (u.uhave.menorah)
            impossible("already have candelabrum?");
        u.uhave.menorah = 1;
        if (flags.showscore && !u.uachieve.menorah)
            context.botl = 1;
        if (!u.uachieve.menorah)
            achievement_gained("Candelabrum of Invocation");
        u.uachieve.menorah = 1;
    }
    else if (obj->otyp == BELL_OF_OPENING)
    {
        if (u.uhave.bell)
            impossible("already have silver bell?");
        u.uhave.bell = 1;
        if (flags.showscore && !u.uachieve.bell)
            context.botl = 1;
        if (!u.uachieve.bell)
            achievement_gained("Bell of Opening");
        u.uachieve.bell = 1;
    }
    else if (obj->otyp == SPE_BOOK_OF_THE_DEAD)
    {
        if (u.uhave.book)
            impossible("already have the book?");
        u.uhave.book = 1;
        if (flags.showscore && !u.uachieve.book)
            context.botl = 1;
        if (!u.uachieve.bell)
            achievement_gained("Book of the Dead");
        u.uachieve.book = 1;
    } 
    else if (obj->oartifact) 
    {
        if (is_quest_artifact(obj)) 
        {
            if (u.uhave.questart)
                impossible("already have quest artifact?");
            u.uhave.questart = 1;
            artitouch(obj);
        }
        else if (obj->otyp == SPE_BOOK_OF_MODRON)
        {
            if (u.uhave.prime_codex)
                impossible("already have Prime Codex?");
            u.uhave.prime_codex = 1;
            if (flags.showscore && !u.uachieve.prime_codex)
                context.botl = 1;
            if (!u.uachieve.prime_codex)
                achievement_gained("Prime Codex");
            u.uachieve.prime_codex = 1;
        }

        boolean check_achievement = FALSE;
        if (Role_if(PM_BARBARIAN))
        {
            check_achievement = TRUE;
            if (u.ualignbase[A_ORIGINAL] == A_CHAOTIC)
            {
                if (obj->oartifact == ART_STORMBRINGER)
                    u.uevent.role_achievement_1 = 1;
                if (obj->oartifact == ART_MOURNBLADE)
                    u.uevent.role_achievement_2 = 1;
            }
            else
            {
                if (obj->oartifact == ART_VORPAL_BLADE)
                    u.uevent.role_achievement_1 = 1;
                if (obj->oartifact == ART_CLEAVER)
                    u.uevent.role_achievement_2 = 1;
            }
        }
        else if (Role_if(PM_KNIGHT))
        {
            check_achievement = TRUE;
            if (obj->oartifact == ART_EXCALIBUR)
                u.uevent.role_achievement_1 = 1;
            if (obj->oartifact == ART_HOLY_GRAIL)
                u.uevent.role_achievement_2 = 1;
        }
        else if (Role_if(PM_PRIEST))
        {
            check_achievement = TRUE;
            if (obj->oartifact == ART_MACE_OF_SAINT_CUTHBERT)
                u.uevent.role_achievement_1 = 1;
            if (obj->oartifact == ART_MACE_OF_SAINT_CUTHBERT)
                u.uevent.role_achievement_2 = 1;
        }
        else if (Role_if(PM_SAMURAI))
        {
            check_achievement = TRUE;
            if (obj->oartifact == ART_KUSANAGI)
                u.uevent.role_achievement_1 = 1;
            if (obj->oartifact == ART_KATANA_OF_MASAMUNE)
                u.uevent.role_achievement_2 = 1;
        }

        if (check_achievement && u.uevent.role_achievement_1 && u.uevent.role_achievement_2 && !u.uachieve.role_achievement)
        {
            u.uachieve.role_achievement = 1;
            char abuf[BUFSZ];
            strcpy_capitalized_for_title(abuf, get_role_achievement_description(TRUE));
            achievement_gained(abuf);
        }
    }

    if (is_mines_prize(obj))
    {
        if (flags.showscore && !u.uachieve.mines_luckstone)
            context.botl = 1;
        if (!u.uachieve.mines_luckstone)
            achievement_gained("Gladstone");
        u.uachieve.mines_luckstone = 1;
        obj->speflags &= ~(SPEFLAGS_MINES_PRIZE);
        obj->nomerge = 0;
    }
    else if (is_soko_prize(obj)) 
    {
        if (flags.showscore && !u.uachieve.finish_sokoban)
            context.botl = 1;
        if (!u.uachieve.finish_sokoban)
            achievement_gained("Sokoban Solved");
        u.uachieve.finish_sokoban = 1;
        obj->speflags &= ~(SPEFLAGS_SOKO_PRIZE1 | SPEFLAGS_SOKO_PRIZE2);
        obj->nomerge = 0;
    }
    else if (obj->otyp == GRAIL_OF_HEALING && Role_if(PM_KNIGHT)) // Holy Grail
    {
        if (!u.uachieve.role_achievement)
        {
            if (flags.showscore)
                context.botl = 1;
            u.uachieve.role_achievement = 1;
            achievement_gained("Found the Holy Grail");
        }
    }
}

void
update_all_character_properties(otmp, verbose)
struct obj* otmp; /* object to be identified if any state change happens */
boolean verbose;
{
    boolean state_change_detected = FALSE;
    boolean condition_change = FALSE;
    
    boolean was_invisible = Invis;
    boolean was_wearing_blindfold = Blindfolded;
    boolean was_blocking_blindness = Blocks_Blindness;
    boolean saw_invisible = See_invisible;
    boolean was_blocking_invisibility = Blocks_Invisibility;
    boolean was_telepathic = Telepat;
    boolean was_blind_telepathic = Blind_telepat;
    boolean had_xray_vision = XRay_vision;
    boolean had_magical_breathing = Magical_breathing;
    boolean had_warning = Warning;
    boolean had_warning_of_mon = Warning;
    boolean had_orc_warning = Orc_warning;
    boolean had_elf_warning = Elf_warning;
    boolean had_dwarf_warning = Dwarf_warning;
    boolean had_gnome_warning = Gnome_warning;
    boolean had_human_warning = Human_warning;
    boolean had_demon_warning = Demon_warning;
    boolean had_angel_warning = Angel_warning;
    boolean had_ogre_warning = Ogre_warning;
    boolean had_gnoll_warning = Gnoll_warning;
    boolean had_giant_warning = Giant_warning;
    boolean had_dragon_warning = Dragon_warning;
    boolean had_undead_warning = Undead_warning;
    boolean had_troll_warning = Troll_warning;
    boolean had_lycanthrope_warning = Lycanthrope_warning;
    boolean had_protection = Magical_protection;
    boolean had_magical_shielding= Magical_shielding;
    boolean had_magical_barkskin = Magical_barkskin;
    boolean had_magical_stoneskin = Magical_stoneskin;
    boolean had_titan_strength = Titan_strength;
    boolean had_divine_endurance = Divine_endurance;
    boolean had_divine_dexterity = Divine_dexterity;
    boolean had_divine_intellect = Divine_intellect;
    boolean had_divine_wisdom = Divine_wisdom;
    boolean had_divine_charisma = Divine_charisma;
    boolean was_fast = Fast;
    boolean was_very_fast = Very_fast;
    boolean was_ultra_fast = Ultra_fast;
    boolean was_super_fast = Super_fast;
    boolean was_lightning_fast = Lightning_fast;

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
    boolean was_slowed = !!Slowed;
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


    unsigned long previous_warntype_obj = context.warntype.obj;
    int oldstr = ACURR(A_STR);
    int olddex = ACURR(A_DEX);
    int oldcon = ACURR(A_CON);
    int oldint = ACURR(A_INT);
    int oldwis = ACURR(A_WIS);
    int oldcha = ACURR(A_CHA);

    update_extrinsics();
    updateabon();
    updatemaxen();
    updatemaxhp();

    if (!verbose) /* None of the ancillary effects happen either */
        return;

    /* PROPERTY STATE TRANSITIONS */
    /* Levitation and flying */
    if ((was_flying || was_levitating) && !Flying && !Levitation)
    {
        state_change_detected = TRUE;
        if(was_levitating)
            float_down(0, 0);
        else
            pline("You stop flying%s.", !Is_airlevel(&u.uz) ? " and land down" : "");
    }
    else if (!(was_flying || was_levitating) && (Levitation || Flying))
    {
        float_vs_flight();
        state_change_detected = TRUE;
        if (Levitation)
        {
            float_up();
            spoteffects(FALSE);
        }
        else
        {
            You1("start flying.");
        }
    }

    /* Blindness */
    if (Blind && !was_blind)
    {
        if (Hallucination && had_hallucination)
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Oh, bummer!  Everything is dark!  Help!");
        else
        {
            if (Blindfolded && !was_wearing_blindfold)
                You_cant_ex(ATR_NONE, CLR_MSG_WARNING, "see any more.");
            else if (!Blocks_Blindness || was_blocking_blindness)
                You_cant_ex(ATR_NONE, CLR_MSG_WARNING, "see anything now!");
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "A cloud of darkness falls upon you.");
        }

        if (Punished)
            set_bc(0);

        toggle_blindness();
    }
    else if (!Blind && was_blind && !gulp_blnd_check())
    {
        if (Hallucination && had_hallucination)
        {
            if (u.uroleplay.blind) {
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, "For the first time in your life, you can see, and oh wow, it is all so cosmic for sure!");
                u.uroleplay.blind = FALSE;
            }
            else
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, "Far out!  Everything is all cosmic again!");
        }
        else
        {
            if (u.uroleplay.blind) {
                /* this can only happen by putting on the Eyes of the Overworld;
                   that shouldn't actually produce a permanent cure, but we
                   can't let the "blind from birth" conduct remain intact */
                pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, "For the first time in your life, you can see!");
                u.uroleplay.blind = FALSE;
            }
            else
            {
                if (!Blindfolded && was_wearing_blindfold)
                    You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "can see again.");
                else if (Blocks_Blindness || !was_blocking_blindness)
                    You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "can see!");
                else
                    You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "can see again.");
            }
        }

        if (Punished)
            set_bc(0);

        toggle_blindness();
    }

    /* Hallucination */
    if (Hallucination && !had_hallucination)
    {
        state_change_detected = TRUE;
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Oh wow! Everything %s so cosmic!", (!Blind) ? "looks" : "feels");
    }
    else if (!Hallucination && had_hallucination)
    {
        state_change_detected = TRUE;
        eatmupdate();
        if (u.uswallow) {
            swallowed(0); /* redraw swallow display */
        }
        else {
            /* The see_* routines should be called *before* the pline. */
            see_monsters();
            see_objects();
            see_traps();
        }

        update_inventory();
        context.botl = context.botlx = TRUE;
        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Everything %s SO boring now.", (!Blind) ? "looks" : "feels");
    }


    /* Invisibility */
    if (!Blind)
    {
        if (See_invisible && !saw_invisible)
        {
            /* can now see invisible monsters */
            set_mimic_blocking(); /* do special mimic handling */
            see_monsters();

            if (Invis)
            {
                state_change_detected = TRUE;
                if (was_invisible)
                {
                    newsym(u.ux, u.uy);
                    pline1("Suddenly you are transparent, but there!");
                }
                else
                {
                    /* No change in symbol */
                    self_invis_message();
                }
            }
        }
        else if (!See_invisible && saw_invisible)
        {
            set_mimic_blocking(); /* do special mimic handling */
            see_monsters();

            if (Invis)
            {
                state_change_detected = TRUE;
                newsym(u.ux, u.uy);
                pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly you cannot see yourself.");
            }
        }
        else if (Blocks_Invisibility && !was_blocking_invisibility && !Invis && was_invisible)
        {
            state_change_detected = TRUE;
            newsym(u.ux, u.uy);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "can %s!", See_invisible ? "no longer see through yourself"
                : "see yourself");
        }
        else if (!Blocks_Invisibility && was_blocking_invisibility && Invis && !was_invisible)
        {
            state_change_detected = TRUE;
            newsym(u.ux, u.uy);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "can %s.", See_invisible ? "see through yourself"
                : "no longer see yourself");
        }
        else if (Invis && !was_invisible)
        {
            state_change_detected = TRUE;
            newsym(u.ux, u.uy);
            self_invis_message();
        }
        else if (!Invis && was_invisible)
        {
            state_change_detected = TRUE;
            newsym(u.ux, u.uy);
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "body seems to unfade%s.",
                See_invisible ? " completely" : "..");
        }
    }

    /* Telepathy, warning and blindness */
    if ((Telepat && !was_telepathic)
        || (!Telepat && was_telepathic)
        || (Blind_telepat && !was_blind_telepathic)
        || (!Blind_telepat && was_blind_telepathic)
        || (Warning && !had_warning)
        || (!Warning && had_warning)
        || (Warn_of_mon && (!had_warning_of_mon || context.warntype.obj != previous_warntype_obj))
        || (!Warn_of_mon && had_warning_of_mon)
        || (Orc_warning && !had_orc_warning)
        || (!Orc_warning && had_orc_warning)
        || (Elf_warning && !had_elf_warning)
        || (!Elf_warning && had_elf_warning)
        || (Dwarf_warning && !had_dwarf_warning)
        || (!Dwarf_warning && had_dwarf_warning)
        || (Gnome_warning && !had_gnome_warning)
        || (!Gnome_warning && had_gnome_warning)
        || (Human_warning && !had_human_warning)
        || (!Human_warning && had_human_warning)
        || (Demon_warning && !had_demon_warning)
        || (!Demon_warning && had_demon_warning)
        || (Angel_warning && !had_angel_warning)
        || (!Angel_warning && had_angel_warning)
        || (Ogre_warning && !had_ogre_warning)
        || (!Ogre_warning && had_ogre_warning)
        || (Gnoll_warning && !had_gnoll_warning)
        || (!Gnoll_warning && had_gnoll_warning)
        || (Giant_warning && !had_giant_warning)
        || (!Giant_warning && had_giant_warning)
        || (Dragon_warning && !had_dragon_warning)
        || (!Dragon_warning && had_dragon_warning)
        || (Undead_warning && !had_undead_warning)
        || (!Undead_warning && had_undead_warning)
        || (Lycanthrope_warning && !had_lycanthrope_warning)
        || (!Lycanthrope_warning && had_lycanthrope_warning)
        || (Troll_warning && !had_troll_warning)
        || (!Troll_warning && had_troll_warning)
        || (Blind && !was_blind)
        || (!Blind && was_blind)
        )
    {
        see_monsters();
    }

    /* X-ray vision */
    if ((XRay_vision && !had_xray_vision)
        || (!XRay_vision && had_xray_vision)
        )
    {
        vision_full_recalc = 1;
        see_monsters();
    }

    /* Magical breathing*/
    if(!Magical_breathing || had_magical_breathing)
    {
        if (Underwater) 
        {
            if (!has_innate_breathless(youmonst.data) && !amphibious(youmonst.data) && !Swimming) 
            {
                state_change_detected = TRUE;
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "suddenly inhale an unhealthy amount of %s!",
                    hliquid("water"));
                (void)drown();
            }
        }
    }


    /* Speed */
    if ((!was_slowed && Slowed) 
        || (was_lightning_fast && !Lightning_fast)
        || (was_super_fast && !was_lightning_fast && !Super_fast && !Lightning_fast)
        || (was_ultra_fast && !was_super_fast && !was_lightning_fast && !Ultra_fast && !Super_fast && !Lightning_fast)
        || (was_very_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast && !Very_fast && !Ultra_fast && !Super_fast && !Lightning_fast)
        || (was_fast && !was_very_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast && !Fast && !Very_fast && !Ultra_fast && !Super_fast && !Lightning_fast)
        )
    {
        state_change_detected = TRUE;
        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "yourself slow down%s.", Fast ? " a bit" : "");
    }
    else if (Lightning_fast && !was_lightning_fast)
    {
        state_change_detected = TRUE;
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "yourself speed up%s.", was_fast || was_very_fast || was_ultra_fast || was_super_fast ? " a bit more" : "");
    }
    else if (Super_fast && !was_super_fast && !was_lightning_fast)
    {
        state_change_detected = TRUE;
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "yourself speed up%s.", was_fast || was_very_fast || was_ultra_fast ? " a bit more" : "");
    }
    else if (Ultra_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast)
    {
        state_change_detected = TRUE;
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "yourself speed up%s.", was_fast || was_very_fast ? " a bit more" : "");
    }
    else if (Very_fast && !was_very_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast)
    {
        state_change_detected = TRUE;
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "yourself speed up%s.", was_fast ? " a bit more" : "");
    }
    else if (Fast && !was_fast && !was_very_fast && !was_ultra_fast && !was_super_fast && !was_lightning_fast)
    {
        state_change_detected = TRUE;
        You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "yourself speed up.");
    }


    /* Silenced */
    if (Silenced && !was_silenced)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_NEGATIVE, "voice disappears!");
    }
    else if (!Silenced && was_silenced)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_POSITIVE, "voice returns!");
    }

    /* Cancellation */
    if (Cancelled && !was_cancelled)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_NEGATIVE, "voice disappears!");
    }
    else if (!Cancelled && was_cancelled)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_POSITIVE, "voice returns!");
    }

    /* Some spell powers */
    if (Magical_protection && !had_protection)
    {
        state_change_detected = TRUE;
        You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "feel protected!");
    }
    else if (!Magical_protection && had_protection)
    {
        state_change_detected = TRUE;
        You_ex1(ATR_NONE, CLR_MSG_WARNING, "feel unprotected!");
    }

    if (Magical_shielding && !had_magical_shielding)
    {
        state_change_detected = TRUE;
        You_ex1(ATR_NONE, CLR_MSG_POSITIVE, "feel shielded!");
    }
    else if (!Magical_shielding && had_magical_shielding)
    {
        state_change_detected = TRUE;
        You_ex1(ATR_NONE, CLR_MSG_WARNING, "feel unshielded!");
    }

    if (Magical_barkskin && !had_magical_barkskin)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_POSITIVE, "skin thickens into bark!");
    }
    else if (!Magical_barkskin && had_magical_barkskin)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_WARNING, "skin softens!");
    }

    if (Magical_stoneskin && !had_magical_stoneskin)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_POSITIVE, "skin thickens into bark!");
    }
    else if (!Magical_stoneskin && had_magical_stoneskin)
    {
        state_change_detected = TRUE;
        Your_ex1(ATR_NONE, CLR_MSG_WARNING, "skin unstones!");
    }

    if (Titan_strength && !had_titan_strength)
    {
        if(ACURR(A_STR) != oldstr)
            state_change_detected = TRUE;
        //You("feel as strong as a titan!");
    }
    else if (!Titan_strength && had_titan_strength)
    {
        if (ACURR(A_STR) != oldstr)
            state_change_detected = TRUE;
        //You("feel less strong than before.");
    }

    if (Divine_endurance && !had_divine_endurance)
    {
        if (ACURR(A_CON) != oldcon)
            state_change_detected = TRUE;
        //You("feel your constitution is greatly increased!");
    }
    else if (!Divine_endurance && had_divine_endurance)
    {
        if (ACURR(A_CON) != oldcon)
            state_change_detected = TRUE;
        //You("feel your constitution is less than before.");
    }

    if (Divine_dexterity && !had_divine_dexterity)
    {
        if (ACURR(A_DEX) != olddex)
            state_change_detected = TRUE;
        //You("feel your dexterity is greatly increased!");
    }
    else if (!Divine_dexterity && had_divine_dexterity)
    {
        if (ACURR(A_DEX) != olddex)
            state_change_detected = TRUE;
        //You("feel your dexterity is less than before.");
    }

    if (Divine_intellect && !had_divine_intellect)
    {
        if (ACURR(A_INT) != oldint)
            state_change_detected = TRUE;
        //You("feel your intelligence is greatly increased!");
    }
    else if (!Divine_intellect && had_divine_intellect)
    {
        if (ACURR(A_INT) != oldint)
            state_change_detected = TRUE;
        //You("feel your intelligence is less than before.");
    }

    if (Divine_wisdom && !had_divine_wisdom)
    {
        if (ACURR(A_WIS) != oldwis)
            state_change_detected = TRUE;
        //You("feel your wisdom is greatly increased!");
    }
    else if (!Divine_wisdom && had_divine_wisdom)
    {
        if (ACURR(A_WIS) != oldwis)
            state_change_detected = TRUE;
        //You("feel your wisdom is less than before.");
    }

    if (Divine_charisma && !had_divine_charisma)
    {
        if (ACURR(A_CHA) != oldcha)
            state_change_detected = TRUE;
        //You("feel your charisma is greatly increased!");
    }
    else if (!Divine_charisma && had_divine_charisma)
    {
        if (ACURR(A_CHA) != oldcha)
            state_change_detected = TRUE;
        //You("feel your charisma is less than before.");
    }

    if (iflags.perm_invent && (oldstr != ACURR(A_STR) || oldcon != ACURR(A_CON)))
    {
        update_inventory(); /* For weight allowance */
    }

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
        || (old_move != get_u_move_speed(TRUE))
        )
        condition_change = TRUE;

    if (otmp && state_change_detected)
    {
        makeknown(otmp->otyp);
    }

    if (state_change_detected || condition_change)
    {
        context.botl = context.botlx = TRUE;
        force_redraw_at(u.ux, u.uy);
    }
}

/*
 * Adjust hero intrinsics as if this object was being added to the hero's
 * inventory.  Called _after_ the object has been added to the hero's
 * inventory.
 *
 * This is called when adding objects to the hero's inventory normally (via
 * addinv) or when an object in the hero's inventory has been polymorphed
 * in-place.
 */
void
addinv_core2(obj)
struct obj *obj;
{
    update_all_character_properties(obj, TRUE);
}

/*
 * Add obj to the hero's inventory.  Make sure the object is "free".
 * Adjust hero attributes as necessary.
 */
struct obj *
addinv(obj)
struct obj *obj;
{
    if (!obj)
        return obj;

    struct obj *otmp, *prev;
    int saved_otyp = (int) obj->otyp; /* for panic */
    boolean obj_was_thrown;
    long quan = obj->quan;

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


    if (obj->where != OBJ_FREE)
    {
        panic("addinv: obj not free");
        return (struct obj*)0;
    }
    /* normally addtobill() clears no_charge when items in a shop are
       picked up, but won't do so if the shop has become untended */
    obj->no_charge = 0; /* should not be set in hero's invent */
    if (Has_contents(obj))
        picked_container(obj); /* clear no_charge */
    obj_was_thrown = obj->was_thrown;
    obj->was_thrown = 0;       /* not meaningful for invent */
    obj->speflags &= ~SPEFLAGS_GRABBED_FROM_YOU; /* You got it back! */
    obj->speflags &= ~SPEFLAGS_CAUGHT_IN_LEAVES; /* Obviously not caught anymore! */
    
    addinv_core1(obj);

    /* merge with quiver in preference to any other inventory slot
       in case quiver and wielded weapon are both eligible; adding
       extra to quivered stack is more useful than to wielded one */
    if (uquiver && merged(&uquiver, &obj)) {
        obj = uquiver;
        if (!obj)
        {
            panic("addinv: null obj after quiver merge otyp=%d", saved_otyp);
            return (struct obj*)0;
        }
        goto added;
    }
    /* merge if possible; find end of chain in the process */
    for (prev = 0, otmp = invent; otmp; prev = otmp, otmp = otmp->nobj)
        if (merged(&otmp, &obj)) {
            obj = otmp;
            if (!obj)
            {
                panic("addinv: null obj after merge otyp=%d", saved_otyp);
                return (struct obj*)0;
            }
            goto added;
        }
    /* didn't merge, so insert into chain */
    assigninvlet(obj);
    if (flags.invlet_constant || !prev) {
        obj->nobj = invent; /* insert at beginning */
        invent = obj;
        if (flags.invlet_constant)
            reorder_invent();
    } else {
        prev->nobj = obj; /* insert at end */
        obj->nobj = 0;
    }
    obj->where = OBJ_INVENT;

    /* fill empty quiver if obj was thrown */
    if (flags.pickup_thrown && !uquiver && obj_was_thrown
        /* if Mjollnir is thrown and fails to return, we want to
           auto-pick it when we move to its spot, but not into quiver;
           aklyses behave like Mjollnir when thrown while wielded, but
           we lack sufficient information here make them exceptions */
        && !(objects[obj->otyp].oc_flags & O1_CAN_BE_THROWN_ONLY_IF_WIELDED)
        && (throwing_weapon(obj) || is_ammo(obj)))
        setuqwep(obj);
 added:
    addinv_core2(obj);
    carry_obj_effects(obj); /* carrying affects the obj */

    if (obj)
    {
        if (obj->oclass == COIN_CLASS)
        {
            if (quan > 0)
            {
                char cbuf[BUFSZ];
                Sprintf(cbuf, "+%ld %s", quan, "gold" /*currency(quan)*/);
                display_floating_text(u.ux, u.uy, cbuf, FLOATING_TEXT_GOLD_ACQUIRED, ATR_NONE, NO_COLOR, 0UL);

            }
        }
        else
        {
            context.last_picked_obj_oid = obj->o_id;
            /* +1 because picking or any other action through which the item was acquired reduces the counter by one immediately */
            context.last_picked_obj_show_duration_left = 1 + (flags.last_item_show_duration > 0 && flags.last_item_show_duration <= MAX_LAST_ITEM_SHOW_DURATION ? flags.last_item_show_duration : DEF_LAST_ITEM_SHOW_DURATION);
        }
    }
    else
    {
        context.last_picked_obj_oid = 0;
        context.last_picked_obj_show_duration_left = 0;
    }

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
        /* Nonobservable ring */
        learnring(obj, FALSE);
    }



    update_inventory();
    return obj;
}

/*
 * Some objects are affected by being carried.
 * Make those adjustments here. Called _after_ the object
 * has been added to the hero's or monster's inventory,
 * and after hero's intrinsics have been updated.
 */
void
carry_obj_effects(obj)
struct obj *obj;
{
    /* Cursed figurines can spontaneously transform when carried. */
    if (obj->otyp == FIGURINE) {
        if (obj->cursed && obj->corpsenm != NON_PM
            && !dead_species(obj->corpsenm, TRUE)) {
            attach_fig_transform_timeout(obj);
        }
    }
}

/* Add an item to the inventory unless we're fumbling or it refuses to be
 * held (via touch_artifact), and give a message.
 * If there aren't any free inventory slots, we'll drop it instead.
 * If both success and failure messages are NULL, then we're just doing the
 * fumbling/slot-limit checking for a silent grab.  In any case,
 * touch_artifact will print its own messages if they are warranted.
 */
struct obj *
hold_another_object(obj, drop_fmt, drop_arg, hold_msg)
struct obj *obj;
const char *drop_fmt, *drop_arg, *hold_msg;
{
    char buf[BUFSZ];

    if (!Blind)
        obj->dknown = 1; /* maximize mergibility */
    if (obj->oartifact) {
        /* place_object may change these */
        //boolean crysknife = (obj->otyp == CRYSKNIFE);
        //int oerode = obj->oerodeproof;
        boolean wasUpolyd = Upolyd;

        /* in case touching this object turns out to be fatal */
        place_object(obj, u.ux, u.uy);
        newsym(u.ux, u.uy);

        if (!touch_artifact(obj, &youmonst)) {
            obj_extract_self(obj); /* remove it from the floor */
            dropy(obj);            /* now put it back again :-) */
            return obj;
        } else if (wasUpolyd && !Upolyd) {
            /* loose your grip if you revert your form */
            if (drop_fmt)
                pline(drop_fmt, drop_arg);
            obj_extract_self(obj);
            dropy(obj);
            return obj;
        }
        obj_extract_self(obj);
#if 0
        if (crysknife) {
            obj->otyp = CRYSKNIFE;
            obj->oerodeproof = oerode;
        }
#endif
    }
    if (Fumbling) {
        obj->nomerge = 1;
        obj = addinv(obj); /* dropping expects obj to be in invent */
        goto drop_it;
    } else {
        long oquan = obj->quan;
        int prev_encumbr = near_capacity(); /* before addinv() */

        /* encumbrance only matters if it would now become worse
           than max( current_value, stressed ) */
        if (prev_encumbr < MOD_ENCUMBER)
            prev_encumbr = MOD_ENCUMBER;
        /* addinv() may redraw the entire inventory, overwriting
           drop_arg when it comes from something like doname() */
        if (drop_arg)
            drop_arg = strcpy(buf, drop_arg);

        obj = addinv(obj);
        if (inv_cnt(FALSE) > 52 || ((obj->otyp != LOADSTONE || !obj->cursed)
                                    && near_capacity() > prev_encumbr)) {
            /* undo any merge which took place */
            if (obj->quan > oquan)
                obj = splitobj(obj, oquan);
            goto drop_it;
        } else {
            if (flags.autoquiver && !uquiver && !obj->owornmask
                && (is_missile(obj) || ammo_and_launcher(obj, uwep)
                    || ammo_and_launcher(obj, uswapwep)))
                setuqwep(obj);
            if (hold_msg || drop_fmt)
                prinv(hold_msg, obj, oquan);
        }
    }
    newsym(u.ux, u.uy);
    return obj;

 drop_it:
    if (drop_fmt)
        pline(drop_fmt, drop_arg);
    obj->nomerge = 0;
    if (can_reach_floor(TRUE)) {
        dropx(obj);
    } else {
        freeinv(obj);
        hitfloor(obj, FALSE);
    }
    newsym(u.ux, u.uy);
    return (struct obj *) 0; /* might be gone */
}

/* useup() all of an item regardless of its quantity */
void
useupall(obj)
struct obj *obj;
{
    setnotworn(obj);
    freeinv(obj);
    obfree(obj, (struct obj *) 0); /* deletes contents also */
}

void
useup(obj)
register struct obj *obj;
{
    /* Note:  This works correctly for containers because they (containers)
       don't merge. */
    if (obj->quan > 1L) {
        obj->in_use = FALSE; /* no longer in use */
        obj->quan--;
        obj->owt = weight(obj);
        update_inventory();
    } else {
        useupall(obj);
    }
}

/* use one charge from an item and possibly incur shop debt for it */
void
consume_obj_charge(obj, maybe_unpaid)
struct obj *obj;
boolean maybe_unpaid; /* false if caller handles shop billing */
{
    if (maybe_unpaid)
        check_unpaid(obj);
    obj->charges -= 1;
    if (obj->known)
        update_inventory();
}

/*
 * Adjust hero's attributes as if this object was being removed from the
 * hero's inventory.  This should only be called from freeinv() and
 * where we are polymorphing an object already in the hero's inventory.
 *
 * Should think of a better name...
 */
void
freeinv_core(obj)
struct obj *obj;
{
    if (obj->oclass == COIN_CLASS) 
    {
        context.botl = 1;
        return;
    } 
    else if (obj->otyp == AMULET_OF_YENDOR) 
    {
        if (!u.uhave.amulet)
            impossible("don't have amulet?");
        u.uhave.amulet = 0;
    }
    else if (obj->otyp == CANDELABRUM_OF_INVOCATION) 
    {
        if (!u.uhave.menorah)
            impossible("don't have candelabrum?");
        u.uhave.menorah = 0;
    }
    else if (obj->otyp == BELL_OF_OPENING)
    {
        if (!u.uhave.bell)
            impossible("don't have silver bell?");
        u.uhave.bell = 0;
    } 
    else if (obj->otyp == SPE_BOOK_OF_THE_DEAD)
    {
        if (!u.uhave.book)
            impossible("don't have the book?");
        u.uhave.book = 0;
    } 
    else if (obj->oartifact) 
    {
        if (is_quest_artifact(obj)) 
        {
            if (!u.uhave.questart)
                impossible("don't have quest artifact?");
            u.uhave.questart = 0;
        }
        else if (obj->otyp == SPE_BOOK_OF_MODRON)
        {
            if (!u.uhave.prime_codex)
                impossible("don't have Prime Codex?");
            u.uhave.prime_codex = 0;
        }
    }

    if (objects[obj->otyp].oc_flags & O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED) {
        curse(obj);
    }

    update_all_character_properties(obj, TRUE);

    if (obj->otyp == FIGURINE && obj->timed) {
        (void) stop_timer(FIG_TRANSFORM, obj_to_any(obj));
    }
}

/* remove an object from the hero's inventory */
void
freeinv(obj)
register struct obj *obj;
{
    if (obj->o_id == context.last_picked_obj_oid)
    {
        context.last_picked_obj_oid = 0;
        context.last_picked_obj_show_duration_left = 0;
    }
    extract_nobj(obj, &invent);
    freeinv_core(obj);
    update_inventory();
}

void
delallobj(x, y)
int x, y;
{
    struct obj *otmp, *otmp2;

    for (otmp = level.objects[x][y]; otmp; otmp = otmp2) {
        if (otmp == uball)
            unpunish();
        /* after unpunish(), or might get deallocated chain */
        otmp2 = otmp->nexthere;
        if (otmp == uchain)
            continue;
        delobj(otmp);
    }
}

/* destroy object in fobj chain (if unpaid, it remains on the bill) */
/* destroy object in fobj chain (if unpaid, it remains on the bill) */
void
delobj(obj)
register struct obj* obj;
{
    delobj_with_flags(obj, 0UL);
}

void
delobj_with_flags(obj, newsym_flags)
register struct obj *obj;
unsigned long newsym_flags;
{
    boolean update_map;

    if (obj->otyp == AMULET_OF_YENDOR
        || obj->otyp == CANDELABRUM_OF_INVOCATION
        || obj->otyp == BELL_OF_OPENING
        || obj->otyp == SPE_BOOK_OF_THE_DEAD
        ) {
        /* player might be doing something stupid, but we
         * can't guarantee that.  assume special artifacts
         * are indestructible via drawbridges, and exploding
         * chests, and golem creation, and ...
         */
        return;
    }
    update_map = (obj->where == OBJ_FLOOR);
    obj_extract_self(obj);
    if (update_map)
        newsym_with_flags(obj->ox, obj->oy, newsym_flags);
    obfree(obj, (struct obj *) 0); /* frees contents also */
}

/* try to find a particular type of object at designated map location */
struct obj *
sobj_at(otyp, x, y)
int otyp;
int x, y;
{
    register struct obj *otmp;

    for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
        if (otmp->otyp == otyp)
            return otmp;

    return otmp;
}

/* try to find a particular type of noncursed object at designated map location */
struct obj*
noncursed_sobj_at(otyp, x, y)
int otyp;
int x, y;
{
    register struct obj* otmp;

    for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
        if (otmp->otyp == otyp && !otmp->cursed)
            return otmp;

    return otmp;
}

/* try to find a particular type of object at designated map location */
struct obj*
any_obj_at(otyp, x, y)
int otyp;
int x, y;
{
    register struct obj* otmp;

#if 0
    /* Memory objects first */
    for (otmp = level.locations[x][y].hero_memory_layers.memory_objchn; otmp; otmp = otmp->nexthere)
        if (otmp->otyp == otyp)
            return otmp;
#endif

    /* Then actual */
    for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
        if (otmp->otyp == otyp)
            return otmp;
        else if (Is_container(otmp))
        {
            struct obj* otmp2 = (struct obj*)0;
            if ((otmp2 = otyp_in_objchn(otyp, otmp->cobj)) != 0)
                return otmp2;
        }

    /* Then buried, note that you have to check x and y here */
    for (otmp = level.buriedobjlist; otmp; otmp = otmp->nobj)
        if (otmp->ox == x && otmp->oy == y)
        {
            if(otmp->otyp == otyp)
                return otmp;
            else if (Is_container(otmp))
            {
                struct obj* otmp2 = (struct obj*)0;
                if ((otmp2 = otyp_in_objchn(otyp, otmp->cobj)) != 0)
                    return otmp2;
            }
        }

    return otmp;
}

/* Note: uses nobj, not nexthere */
struct obj*
otyp_in_objchn(otyp, objchn)
int otyp;
struct obj* objchn;
{
    struct obj* otmp;
    for (otmp = objchn; otmp; otmp = otmp->nobj)
        if (otmp->otyp == otyp)
            return otmp;
        else if (Is_container(otmp))
        {
            struct obj* otmp2 = (struct obj*)0;
            if ((otmp2 = otyp_in_objchn(otyp, otmp->cobj)) != 0)
                return otmp2;
        }

    return (struct obj*)0;
}

/* sobj_at(&c) traversal -- find next object of specified type */
struct obj *
nxtobj(obj, type, by_nexthere)
struct obj *obj;
int type;
boolean by_nexthere;
{
    register struct obj *otmp;

    otmp = obj; /* start with the object after this one */
    do {
        otmp = !by_nexthere ? otmp->nobj : otmp->nexthere;
        if (!otmp)
            break;
    } while (otmp->otyp != type);

    return otmp;
}

struct obj *
carrying(type)
register int type;
{
    register struct obj *otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == type)
            return  otmp;
    return (struct obj *) 0;
}

struct obj*
carrying_leashed_leash()
{
    register struct obj* otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == LEASH && otmp->leashmon != 0)
            return  otmp;
    return (struct obj*)0;
}


struct obj *
carrying_fitting_unlocking_tool_for_box(box)
struct obj* box;
{
    if (!box)
        return (struct obj*)0;

    register struct obj *otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (is_unlocking_tool(otmp) && key_fits_the_box_lock(otmp, box))
            return  otmp;
    return (struct obj *) 0;
}

struct obj*
carrying_fitting_unlocking_tool_for_door(door)
struct rm* door;
{
    if (!door)
        return (struct obj*)0;

    register struct obj* otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (is_unlocking_tool(otmp) && key_fits_the_door_lock(otmp, door))
            return  otmp;
    return (struct obj*)0;
}

/* Fictional and not-so-fictional currencies.
 * http://concord.wikia.com/wiki/List_of_Fictional_Currencies
 */
static const char *const currencies[] = {
    "Altarian Dollar",       /* The Hitchhiker's Guide to the Galaxy */
    "Ankh-Morpork Dollar",   /* Discworld */
    "auric",                 /* The Domination of Draka */
    "buckazoid",             /* Space Quest */
    "cirbozoid",             /* Starslip */
    "credit chit",           /* Deus Ex */
    "cubit",                 /* Battlestar Galactica */
    "Flanian Pobble Bead",   /* The Hitchhiker's Guide to the Galaxy */
    "fretzer",               /* Jules Verne */
    "imperial credit",       /* Star Wars */
    "Hong Kong Luna Dollar", /* The Moon is a Harsh Mistress */
    "kongbuck",              /* Snow Crash */
    "nanite",                /* System Shock 2 */
    "quatloo",               /* Star Trek, Sim City */
    "simoleon",              /* Sim City */
    "solari",                /* Spaceballs */
    "spacebuck",             /* Spaceballs */
    "sporebuck",             /* Spore */
    "Triganic Pu",           /* The Hitchhiker's Guide to the Galaxy */
    "woolong",               /* Cowboy Bebop */
    "zorkmid",               /* Zork, NetHack */
};

const char *
currency(amount)
long amount;
{
    const char *res;

    res = Hallucination ? currencies[rn2(SIZE(currencies))] : "gold piece";
    if (amount != 1L)
        res = makeplural(res);
    return res;
}

boolean
have_lizard()
{
    register struct obj *otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == CORPSE && otmp->corpsenm == PM_LIZARD)
            return  TRUE;
    return FALSE;
}

/* 3.6 tribute */
struct obj *
u_have_novel()
{
    register struct obj *otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == SPE_NOVEL)
            return otmp;
    return (struct obj *) 0;
}

struct obj *
o_on(id, objchn)
unsigned int id;
register struct obj *objchn;
{
    struct obj *temp;

    while (objchn) {
        if (objchn->o_id == id)
            return objchn;
        if (Has_contents(objchn) && (temp = o_on(id, objchn->cobj)))
            return temp;
        objchn = objchn->nobj;
    }
    return (struct obj *) 0;
}


struct obj*
o_on_memory(id, objchn)
unsigned int id;
register struct obj* objchn;
{
    struct obj* temp;

    while (objchn) {
        if (objchn->o_id_memory == id)
            return objchn;
        if (Has_contents(objchn) && (temp = o_on_memory(id, objchn->cobj)))
            return temp;
        objchn = objchn->nobj;
    }
    return (struct obj*)0;
}

struct obj*
m_on_memory(mid, objchn)
unsigned int mid;
register struct obj* objchn;
{
    struct obj* temp;

    while (objchn) {
        if (objchn->m_id_memory == mid)
            return objchn;
        if (Has_contents(objchn) && (temp = m_on_memory(mid, objchn->cobj)))
            return temp;
        objchn = objchn->nobj;
    }
    return (struct obj*)0;
}

void
reduce_item_cooldown(objchn)
register struct obj *objchn;
{
    boolean reached_zero = FALSE;

    while (objchn) {
        //Reduce cooldown timer
        if (objchn->cooldownleft > 0)
        {
            objchn->cooldownleft--;
            if (!objchn->cooldownleft)
                reached_zero = TRUE;
        }

        if (objchn->repowerleft > 0)
        {
            objchn->repowerleft--;
            if (!objchn->repowerleft)
                reached_zero = TRUE;
        }

        if (objchn->invokeleft > 0)
        {
            objchn->invokeleft--;
            if (!objchn->invokeleft)
                reached_zero = TRUE;
        }

        //If has contents, then reduce the cooldown of the contents, too
        if (Has_contents(objchn))
            reduce_item_cooldown(objchn->cobj);
        
        objchn = objchn->nobj;
    }

    if (objchn == invent && reached_zero)
        update_inventory();
}

boolean
obj_here(obj, x, y)
register struct obj *obj;
int x, y;
{
    register struct obj *otmp;

    for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
        if (obj == otmp)
            return TRUE;
    return FALSE;
}

struct obj *
g_at(x, y)
register int x, y;
{
    register struct obj *obj = level.objects[x][y];

    while (obj) {
        if (obj->oclass == COIN_CLASS)
            return obj;
        obj = obj->nexthere;
    }
    return (struct obj *) 0;
}

/* compact a string of inventory letters by dashing runs of letters */
STATIC_OVL void
compactify(buf)
register char *buf;
{
    register int i1 = 1, i2 = 1;
    register char ilet, ilet1, ilet2;

    ilet2 = buf[0];
    ilet1 = buf[1];
    buf[++i2] = buf[++i1];
    ilet = buf[i1];
    while (ilet) {
        if (ilet == ilet1 + 1) {
            if (ilet1 == ilet2 + 1)
                buf[i2 - 1] = ilet1 = '-';
            else if (ilet2 == '-') {
                buf[i2 - 1] = ++ilet1;
                buf[i2] = buf[++i1];
                ilet = buf[i1];
                continue;
            }
        } else if (ilet == NOINVSYM) {
            /* compact three or more consecutive '#'
               characters into "#-#" */
            if (i2 >= 2 && buf[i2 - 2] == NOINVSYM && buf[i2 - 1] == NOINVSYM)
                buf[i2 - 1] = '-';
            else if (i2 >= 3 && buf[i2 - 3] == NOINVSYM && buf[i2 - 2] == '-'
                     && buf[i2 - 1] == NOINVSYM)
                --i2;
        }
        ilet2 = ilet1;
        ilet1 = ilet;
        buf[++i2] = buf[++i1];
        ilet = buf[i1];
    }
}

/* some objects shouldn't be split when count given to getobj or askchain */
boolean
splittable(obj)
struct obj *obj;
{
    return !(((objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && obj->cursed)
             || (obj == uwep && welded(uwep, &youmonst)));
}

/* match the prompt for either 'T' or 'R' command */
STATIC_OVL boolean
taking_off(action)
const char *action;
{
    return !strcmp(action, "take off") || !strcmp(action, "remove");
}

/* match the prompt for either 'W' or 'P' command */
STATIC_OVL boolean
putting_on(action)
const char *action;
{
    return !strcmp(action, "wear") || !strcmp(action, "put on");
}


STATIC_OVL boolean
trading_items(action)
const char* action;
{
    return !strcmp(action, "buy") || !strcmp(action, "trade");
}


/*
 * getobj returns:
 *      struct obj *xxx:        object to do something with.
 *      (struct obj *) 0        error return: no object.
 *      &zeroobj                explicitly no object (as in w-).
!!!! test if gold can be used in unusual ways (eaten etc.)
!!!! may be able to remove "usegold"
 */
struct obj*
getobj(let, word, show_weights, headertext)
register const char* let, * word;
int show_weights;
const char* headertext;
{
    return getobj_ex(let, word, show_weights, headertext, (boolean (*)(struct obj*))0);
}

struct obj *
getobj_ex(let, word, show_weights, headertext, validitemfunc)
register const char *let, *word;
int show_weights;
const char* headertext;
boolean (*validitemfunc)(struct obj*);
{
    register struct obj *otmp;
    register char ilet = 0;
    char buf[BUFSZ], qbuf[QBUFSZ]; // , leftbuf[BUFSZ], rightbuf[BUFSZ];
    char lets[BUFSZ], altlets[BUFSZ]; //, * ap;
    int foo = 0;
    char *bp = buf;
    xchar allowcnt = 0; /* 0, 1 or 2 */
    boolean usegold = FALSE; /* can't use gold because its illegal */
    boolean allowall = FALSE;
    boolean allownone = FALSE;
    boolean useboulder = FALSE;
    xchar foox = 0;
    long cnt;
    boolean cntgiven = FALSE;
    boolean msggiven = FALSE;
    boolean oneloop = FALSE;
    //long dummymask;
    //Loot *sortedinvent, *srtinv;

    construct_getobj_letters(let, word, validitemfunc, lets, altlets, buf, sizeof lets, sizeof altlets, sizeof buf, &foo, &foox, &bp, &allowcnt, &usegold, &allowall, &allownone, &useboulder, getobj_autoselect_obj);
    if (getobj_autoselect_obj)
    {
        if (index(lets, getobj_autoselect_obj->invlet))
        {
            struct obj* otmp_save = getobj_autoselect_obj;
            getobj_autoselect_obj = (struct obj*)0;
            return otmp_save;
        }
        else
        {
            getobj_autoselect_obj = (struct obj*)0;
            return (struct obj*)0;
        }
    }

#if 0
    if (*let == ALLOW_COUNT)
        let++, allowcnt = 1;
    if (*let == COIN_CLASS)
        let++, usegold = TRUE;
    
    /* Check for "dip into" */
    if (word)
    {
        size_t l = strlen(word);

        size_t leftnum = 3;
        if (l >= leftnum)
        {
            strncpy(leftbuf, word, leftnum);
            leftbuf[leftnum] = '\0';
        }
        else
            strcpy(leftbuf, "");

        size_t rightnum = 4;
        if (l >= rightnum)
        {
            strncpy(rightbuf, word + l - rightnum, rightnum);
            rightbuf[rightnum] = '\0';
        }
        else
            strcpy(rightbuf, "");

        if(!strcmp(leftbuf, "dip") && !strcmp(rightbuf, "into"))
            is_dip_into = TRUE;
    }
    else
    {
        strcpy(leftbuf, "");
        strcpy(rightbuf, "");
        is_dip_into = FALSE;
    }

    /* Equivalent of an "ugly check" for gold */
    if (usegold && !strcmp(word, "eat")
        && (!metallivorous(youmonst.data) || rust_causing_and_ironvorous(youmonst.data)))
        usegold = FALSE;

    if (*let == ALL_CLASSES)
        let++, allowall = TRUE;
    if (*let == ALLOW_NONE)
        let++, allownone = TRUE;
    /* "ugly check" for reading fortune cookies, part 1.
     * The normal 'ugly check' keeps the object on the inventory list.
     * We don't want to do that for shirts/cookies, so the check for
     * them is handled a bit differently (and also requires that we set
     * allowall in the caller).
     */
    if (allowall && !strcmp(word, "read"))
        allowall = FALSE;

    /* another ugly check: show boulders (not statues) */
    if (*let == WEAPON_CLASS && !strcmp(word, "throw")
        && throws_rocks(youmonst.data))
        useboulder = TRUE;

    if (allownone)
        *bp++ = HANDS_SYM, *bp++ = ' '; /* '-' */
    ap = altlets;

    if (!flags.invlet_constant)
        reassign();

    /* force invent to be in invlet order before collecting candidate
       inventory letters */
    sortedinvent = sortloot(&invent, SORTLOOT_INVLET, FALSE,
                            (boolean FDECL((*), (OBJ_P))) 0);

    for (srtinv = sortedinvent; (otmp = srtinv->obj) != 0; ++srtinv) {
        if (&bp[foo] == &buf[sizeof buf - 1]
            || ap == &altlets[sizeof altlets - 1]) {
            /* we must have a huge number of NOINVSYM items somehow */
            impossible("getobj: inventory overflow");
            break;
        }

        if (!*let || index(let, otmp->oclass)
            || (usegold && otmp->invlet == GOLD_SYM)
            || (useboulder && otmp->otyp == BOULDER)) {
            register int otyp = otmp->otyp;

            bp[foo++] = otmp->invlet;
/* clang-format off */
/* *INDENT-OFF* */
            /* ugly check: remove inappropriate things */
            if (
                (taking_off(word) /* exclude if not worn */
                 && !(otmp->owornmask & (W_ARMOR | W_ACCESSORY)))
             || (putting_on(word) /* exclude if already worn */
                 && (otmp->owornmask & (W_ARMOR | W_ACCESSORY)))
             || (trading_items(word) /* exclude if already worn and unpaid items */
                 && ((otmp->owornmask & (W_ARMOR | W_ACCESSORY)) || otmp->unpaid))
#if 0 /* 3.4.1 -- include currently wielded weapon among 'wield' choices */
             || (!strcmp(word, "wield")
                 && (otmp->owornmask & W_WEP))
#endif
             || (!strcmp(word, "ready")    /* exclude when wielded... */
                 && ((otmp == uwep || (otmp == uarms && u.twoweap))
                     && otmp->quan == 1L)) /* ...unless more than one */
             || ((!strcmp(word, "dip") || !strcmp(word, "grease"))
                 && inaccessible_equipment(otmp, (const char *) 0, FALSE))
             ) 
            {
                foo--;
                foox++;
            }
            /* Second ugly check; unlike the first it won't trigger an
             * "else" in "you don't have anything else to ___".
             */
            else if (
                (putting_on(word)
                 && ((otmp->oclass == FOOD_CLASS && otmp->otyp != MEAT_RING)
                     || (otmp->oclass == TOOL_CLASS && otyp != BLINDFOLD
                         && otyp != TOWEL)))
             || (!strcmp(word, "wield")
                 && (otmp->oclass == TOOL_CLASS && !is_weptool(otmp)))
             || (!strcmp(word, "eat") && !is_edible(otmp))
             || (!strcmp(word, "drink") && otmp->oclass == TOOL_CLASS && !is_obj_quaffable(otmp))
             || (!strcmp(word, "sacrifice")
                 && (otyp != CORPSE && otyp != AMULET_OF_YENDOR
                     && otyp != FAKE_AMULET_OF_YENDOR))
             || (!strcmp(word, "write with")
                 && (otmp->oclass == TOOL_CLASS
                     && otyp != MAGIC_MARKER && otyp != TOWEL))
             || (!strcmp(word, "tin")
                 && (otyp != CORPSE || !tinnable(otmp)))
             || (!strcmp(word, "rub")
                 && ((otmp->oclass == TOOL_CLASS && otyp != OIL_LAMP
                      && otyp != MAGIC_LAMP && otyp != BRASS_LANTERN)
                     || (otmp->oclass == GEM_CLASS && !is_graystone(otmp))))
                || (!strcmp(word, "use or apply")
                 /* Picks, axes, pole-weapons, bullwhips */
                 && ((otmp->oclass == WEAPON_CLASS && !is_appliable_weapon(otmp))
                     || (otmp->oclass == POTION_CLASS
                         /* only applicable potion is oil, and it will only
                            be offered as a choice when already discovered */
                         && (otyp != POT_OIL || !otmp->dknown
                             || !objects[POT_OIL].oc_name_known))
                     || (otmp->oclass == FOOD_CLASS
                         && otyp != CREAM_PIE && otyp != EUCALYPTUS_LEAF)
                     || (otmp->oclass == MISCELLANEOUS_CLASS
                         && !is_obj_appliable(otmp))
                     || (otmp->oclass == GEM_CLASS && !is_graystone(otmp))))
             || (!strcmp(word, "invoke")
                 && !otmp->oartifact
                 && !is_otyp_unique(otyp)
                 && !is_otyp_invokable(otyp)
                 && (otyp != FAKE_AMULET_OF_YENDOR || otmp->known)
                 /* note: presenting the possibility of invoking non-artifact
                    mirrors and/or lamps is simply a cruel deception... */
                 && (otyp != OIL_LAMP /* don't list known oil lamp */
                     || (otmp->dknown && objects[OIL_LAMP].oc_name_known)))
                || (!strcmp(word, "untrap with")
                 && ((otmp->oclass == TOOL_CLASS && otyp != CAN_OF_GREASE)
                     || (otmp->oclass == POTION_CLASS
                         /* only applicable potion is oil, and it will only
                            be offered as a choice when already discovered */
                         && (otyp != POT_OIL || !otmp->dknown
                             || !objects[POT_OIL].oc_name_known))))
             || (!strcmp(word, "tip") && !Is_container(otmp)
                 /* include horn of plenty if sufficiently discovered */
                 && (otmp->otyp != HORN_OF_PLENTY || !otmp->dknown
                     || !objects[HORN_OF_PLENTY].oc_name_known))
              || (!strcmp(word, "detect blessedness for") && otmp->bknown)
              || (!strcmp(word, "refill") && !is_refillable_with_oil(otmp))
              || (!strcmp(word, "enchant") && otmp->oclass == TOOL_CLASS && !is_obj_enchantable(otmp))
              || (!strcmp(word, "protect") && otmp->oclass == TOOL_CLASS && !is_obj_enchantable(otmp))
             || (!strcmp(word, "charge") && !is_chargeable(otmp))
              || (!strcmp(word, "fire") && (!uwep || !otmp || (otmp && uwep && !ammo_and_launcher(otmp, uwep))))
             || (!strcmp(word, "open") && otyp != TIN)
             || (!strcmp(word, "call") && !objtyp_is_callable(otyp))
             || (is_dip_into && !otyp_allows_object_to_be_dipped_into_it(otyp))
             || (!strcmp(word, "adjust") && otmp->oclass == COIN_CLASS && !usegold)
             ) {
                foo--;
            }
            /* Third ugly check:  acceptable but not listed as likely
             * candidates in the prompt or in the inventory subset if
             * player responds with '?'.
             */
            else if (
             /* ugly check for unworn armor that can't be worn */
                (putting_on(word) && *let == ARMOR_CLASS
                 && !canwearobj(otmp, &dummymask, FALSE))
             /* or armor with 'P' or 'R' or accessory with 'W' or 'T' */
             || ((putting_on(word) || taking_off(word))
                 && ((*let == ARMOR_CLASS) ^ (otmp->oclass == ARMOR_CLASS)))
             /* or unsuitable items rubbed on known touchstone */
             || (!strncmp(word, "rub on the stone", 16)
                 && *let == GEM_CLASS && otmp->dknown
                 && objects[otyp].oc_name_known)
             /* suppress corpses on astral, amulets elsewhere */
             || (!strcmp(word, "sacrifice")
                 /* (!astral && amulet) || (astral && !amulet) */
                 && (!Is_astralevel(&u.uz) ^ (otmp->oclass != AMULET_CLASS)))
             /* suppress container being stashed into */
             || (!strcmp(word, "stash") && !ck_bag(otmp))
             /* worn armor (shirt, suit) covered by worn armor (suit, cloak)
                or accessory (ring) covered by cursed worn armor (gloves) */
             || (taking_off(word)
                 && inaccessible_equipment(otmp, (const char *) 0,
                                      (boolean) (otmp->oclass == RING_CLASS)))
             || (!strcmp(word, "write on")
                 && (!(otyp == SCR_BLANK_PAPER || otyp == SPE_BLANK_PAPER)
                     || !otmp->dknown || !objects[otyp].oc_name_known))
             ) {
                /* acceptable but not listed as likely candidate */
                foo--;
                allowall = TRUE;
                *ap++ = otmp->invlet;
            }
/* *INDENT-ON* */
/* clang-format on */
        } else {
            /* "ugly check" for reading fortune cookies, part 2 */
            if ((!strcmp(word, "read") && is_readable(otmp)))
                allowall = usegold = TRUE;
        }
    }
    unsortloot(&sortedinvent);

    bp[foo] = 0;
    if (foo == 0 && bp > buf && bp[-1] == ' ')
        *--bp = 0;
    Strcpy(lets, bp); /* necessary since we destroy buf */
    if (foo > 5)      /* compactify string */
        compactify(bp);
    *ap = '\0';

    if (!strcmp(word, "write on")) { /* ugly check for magic marker */
        /* we wanted all scrolls and books in altlets[], but that came with
           'allowall' which we don't want since it prevents "silly thing"
           result if anything other than scroll or spellbook is chosen */
        allowall = FALSE;
    }

#endif

    if (!foo && !allowall && !allownone) 
    {
        char endbuf[BUFSZ] = ".";
        char unidbuf[BUFSZ] = "scrolls or spellbooks";
        boolean writeendremark = FALSE;
        if (!strcmp(word, "write on"))
        {
            struct obj* blankscroll = carrying(SCR_BLANK_PAPER);
            struct obj* blankbook = carrying(SPE_BLANK_PAPER);
            if(blankscroll || blankbook)
            {
                if (blankscroll && blankbook)
                {
                    strcpy(unidbuf, "scrolls or spellbooks");
                }
                else if (blankscroll)
                {
                    strcpy(unidbuf, "scrolls");
                }
                else if (blankbook)
                {
                    strcpy(unidbuf, "spellbooks");
                }
                else
                {
                    strcpy(unidbuf, "writable items");
                }
                Sprintf(endbuf, ", but maybe you have unidentified blank %s with you?", unidbuf);
                writeendremark = TRUE;
            }
        }

        play_sfx_sound(SFX_GENERAL_CANNOT);
        You("don't have anything %sto %s%s", foox ? "else " : "", word, endbuf);
        if(writeendremark)
            pline("(You cannot write on unidentified blank %s.)", unidbuf);
        return (struct obj *) 0;
    }
    

    if (!iflags.force_invmenu && strcmp(headertext, "") != 0)
        pline("%s", headertext);

    for (;;) {
        cnt = 0;
        cntgiven = FALSE;
        Sprintf(qbuf, "What do you want to %s?", word);
        if (in_doagain)
            ilet = readchar();
        else if (iflags.force_invmenu) {
            /* don't overwrite a possible quitchars */
            if (!oneloop)
                ilet = /* *let */ buf[0] ? '?' : '*';
            if (!msggiven)
                putmsghistory(qbuf, FALSE);
            msggiven = TRUE;
            oneloop = TRUE;
        } else {
            if (!buf[0])
                Strcat(qbuf, " [*]");
            else
                Sprintf(eos(qbuf), " [%s or ?*]", buf);

            ilet = yn_function(qbuf, (char *)0, '\0', (char *)0);
        }
        if (digit(ilet)) {
            long tmpcnt = 0;

            if (!allowcnt) {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                pline1("No count allowed with this command.");
                continue;
            }
            ilet = get_count(NULL, ilet, LARGEST_INT, &tmpcnt, TRUE);
            if (tmpcnt) {
                cnt = tmpcnt;
                cntgiven = TRUE;
            }
        }
        if (index(quitchars, ilet)) {
            if (flags.verbose)
                pline1(Never_mind);
            return (struct obj *) 0;
        }
        if (ilet == HANDS_SYM) { /* '-' */
            if (!allownone) {
                char *suf = (char *) 0;

                strcpy(buf, word);
                if ((bp = strstr(buf, " on the ")) != 0) {
                    /* rub on the stone[s] */
                    *bp = '\0';
                    suf = (bp + 1);
                }
                if ((bp = strstr(buf, " or ")) != 0) {
                    *bp = '\0';
                    bp = (rn2(2) ? buf : (bp + 4));
                } else
                    bp = buf;
                You("mime %s something%s%s.", ing_suffix(bp), suf ? " " : "",
                    suf ? suf : "");
            }
            return (allownone ? (struct obj *) &zeroobj : (struct obj *) 0);
        }
 redo_menu:
        /* since gold is now kept in inventory, we need to do processing for
           select-from-invent before checking whether gold has been picked */
        if (ilet == '?' || ilet == '*') {
            char *allowed_choices = (ilet == '?') ? lets : (char *) 0;
            long ctmp = 0;
            char menuquery[QBUFSZ];

            menuquery[0] = qbuf[0] = '\0';
            if (iflags.force_invmenu)
                Sprintf(menuquery, "What do you want to %s?", word);
            if (!strcmp(word, "grease"))
                Sprintf(qbuf, "your %s", makeplural(body_part(FINGER)));
            else if (!strcmp(word, "write with"))
                Sprintf(qbuf, "your %s", body_part(FINGERTIP));
            else if (!strcmp(word, "wield"))
                Sprintf(qbuf, "your %s %s%s", uarmg ? "gloved" : "bare",
                        makeplural(body_part(HAND)),
                        !uwep ? " (wielded)" : "");
            else if (!strcmp(word, "ready"))
                Sprintf(qbuf, "empty quiver%s",
                        !uquiver ? " (nothing readied)" : "");

            if (ilet == '?' && !*lets && *altlets)
                allowed_choices = altlets;
            ilet = display_pickinv(allowed_choices, *qbuf ? qbuf : (char *) 0,
                                   menuquery,
                                   TRUE, allowcnt ? &ctmp : (long *) 0, show_weights, headertext, FALSE, FALSE);
            if (!ilet)
                continue;
            if (ilet == HANDS_SYM)
                return (struct obj *) &zeroobj; /* cast away 'const' */
            if (ilet == '\033') {
                if (flags.verbose)
                    pline1(Never_mind);
                return (struct obj *) 0;
            }
            if (ilet == '*')
                goto redo_menu;
            if (allowcnt && ctmp >= 0) {
                cnt = ctmp;
                cntgiven = TRUE;
            }
            /* they typed a letter (not a space) at the prompt */
        }
        /* find the item which was picked */
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (otmp->invlet == ilet)
                break;
        /* some items have restrictions */
        if (ilet == def_oc_syms[COIN_CLASS].sym
            /* guard against the [hypothetical] chace of having more
               than one invent slot of gold and picking the non-'$' one */
            || (otmp && otmp->oclass == COIN_CLASS)) {
            if (!usegold) {
                You("cannot %s gold.", word);
                return (struct obj *) 0;
            }
            /* Historic note: early GnollHack had a bug which was
             * first reported for Larn, where trying to drop 2^32-n
             * gold pieces was allowed, and did interesting things
             * to your money supply.  The LRS is the tax bureau
             * from Larn.
             */
            if (cntgiven && cnt <= 0) {
                if (cnt < 0)
                    pline_The(
                  "LRS would be very interested to know you have that much.");
                return (struct obj *) 0;
            }
        }
        if (cntgiven && (!strcmp(word, "throw") || !strcmp(word, "fire"))) 
        {
            /* permit counts for throwing gold, but don't accept
             * counts for other things since the throw code will
             * split off a single item anyway */
            if (cnt == 0)
                return (struct obj *) 0;
            if (cnt > 1 && (ilet != def_oc_syms[COIN_CLASS].sym
                && !(otmp && otmp->oclass == COIN_CLASS))) {
                You("can only %s one item at a time.", word);
                continue;
            }
        }
        context.botl = 1; /* May have changed the amount of money */
        savech(ilet);
        /* [we used to set otmp (by finding ilet in invent) here, but
           that's been moved above so that otmp can be checked earlier] */
        /* verify the chosen object */
        if (!otmp) {
            You1("don't have that object.");
            if (in_doagain)
                return (struct obj *) 0;
            continue;
        } else if (cnt < 0 || otmp->quan < cnt) {
            You("don't have that many!  You have only %ld.", otmp->quan);
            if (in_doagain)
                return (struct obj *) 0;
            continue;
        }
        break;
    }
    if (!allowall && let && !index(let, otmp->oclass)
        && !(usegold && otmp->oclass == COIN_CLASS)
        && !(!strcmp(word, "read") && (objects[otmp->otyp].oc_flags3 & O3_READABLE))
        ) {
        silly_thing(word, otmp);
        return (struct obj *) 0;
    }
    if (cntgiven) {
        if (cnt == 0)
            return (struct obj *) 0;
        if (cnt != otmp->quan) {
            /* don't split a stack of cursed loadstones */
            if (splittable(otmp))
                otmp = splitobj(otmp, cnt);
            else if (otmp->otyp == LOADSTONE && otmp->cursed)
                /* kludge for canletgo()'s can't-drop-this message */
                otmp->corpsenm = (int) cnt;
        }
    }
    return otmp;
}

void
construct_getobj_letters(let, word, validitemfunc, lets, altlets, buf, lets_size, altlets_size, buf_size, foo_ptr, foox_ptr, bp_ptr, allowcnt_ptr, usegold_ptr, allowall_ptr, allownone_ptr, useboulder_ptr, otmp_only)
register const char *let, *word;
boolean (*validitemfunc)(struct obj*);
char* lets, *altlets, *buf;
size_t lets_size UNUSED, altlets_size, buf_size;
int* foo_ptr;
xchar *foox_ptr, *allowcnt_ptr;
char** bp_ptr;
boolean *usegold_ptr, *allowall_ptr, *allownone_ptr, *useboulder_ptr;
struct obj* otmp_only;
{
    register struct obj* otmp;
    char leftbuf[BUFSZ], rightbuf[BUFSZ];
    char *ap;
    boolean is_dip_into = FALSE;
    int foo = 0;
    char *bp = *bp_ptr;
    xchar allowcnt = 0; /* 0, 1 or 2 */
    boolean usegold = FALSE; /* can't use gold because its illegal */
    boolean allowall = FALSE;
    boolean allownone = FALSE;
    boolean useboulder = FALSE;
    xchar foox = 0;
    long dummymask;
    Loot* sortedinvent, * srtinv;

    if (*let == ALLOW_COUNT)
        let++, allowcnt = 1;
    if (*let == COIN_CLASS)
        let++, usegold = TRUE;

    /* Check for "dip into" */
    if (word)
    {
        size_t l = strlen(word);

        size_t leftnum = 3;
        if (l >= leftnum)
        {
            strncpy(leftbuf, word, leftnum);
            leftbuf[leftnum] = '\0';
        }
        else
            strcpy(leftbuf, "");

        size_t rightnum = 4;
        if (l >= rightnum)
        {
            strncpy(rightbuf, word + l - rightnum, rightnum);
            rightbuf[rightnum] = '\0';
        }
        else
            strcpy(rightbuf, "");

        if (!strcmp(leftbuf, "dip") && !strcmp(rightbuf, "into"))
            is_dip_into = TRUE;
    }
    else
    {
        word = "";
        strcpy(leftbuf, "");
        strcpy(rightbuf, "");
        is_dip_into = FALSE;
    }

    /* Equivalent of an "ugly check" for gold */
    if (usegold && !strcmp(word, "eat")
        && (!metallivorous(youmonst.data) || rust_causing_and_ironvorous(youmonst.data)))
        usegold = FALSE;

    if (*let == ALL_CLASSES)
        let++, allowall = TRUE;
    if (*let == ALLOW_NONE)
        let++, allownone = TRUE;
    /* "ugly check" for reading fortune cookies, part 1.
     * The normal 'ugly check' keeps the object on the inventory list.
     * We don't want to do that for shirts/cookies, so the check for
     * them is handled a bit differently (and also requires that we set
     * allowall in the caller).
     */
    if (allowall && !strcmp(word, "read"))
        allowall = FALSE;

    /* another ugly check: show boulders (not statues) */
    if (*let == WEAPON_CLASS && !strcmp(word, "throw")
        && throws_rocks(youmonst.data))
        useboulder = TRUE;

    if (allownone)
        *bp++ = HANDS_SYM, * bp++ = ' '; /* '-' */
    ap = altlets;

    if (!flags.invlet_constant)
        reassign();

    /* force invent to be in invlet order before collecting candidate
       inventory letters */
    sortedinvent = sortloot(&invent, SORTLOOT_INVLET, FALSE,
        (boolean FDECL((*), (OBJ_P))) 0);

    for (srtinv = sortedinvent; (otmp = srtinv->obj) != 0; ++srtinv) 
    {
        if (otmp_only && otmp != otmp_only)
            continue;

        if (&bp[foo] == &buf[buf_size - 1]
            || ap == &altlets[altlets_size - 1])
        {
            /* we must have a huge number of NOINVSYM items somehow */
            impossible("getobj: inventory overflow");
            break;
        }

        if (!*let || index(let, otmp->oclass)
            || (usegold && otmp->invlet == GOLD_SYM)
            || (useboulder && otmp->otyp == BOULDER))
        {
            register int otyp = otmp->otyp;

            bp[foo++] = otmp->invlet;
            /* clang-format off */
            /* *INDENT-OFF* */
                        /* ugly check: remove inappropriate things */
            if (
                (taking_off(word) /* exclude if not worn */
                    && !(otmp->owornmask & (W_ARMOR | W_ACCESSORY | W_MISCITEMS)))
                || (!strcmp(word, "unwield") /* exclude if not wielded */
                    && !(otmp->owornmask & W_WIELDED_WEAPON))
                || (!strcmp(word, "stash") /* exclude worn items and other containers */
                    && (otmp->owornmask & (W_ARMOR | W_ACCESSORY | W_MISCITEMS) || Is_container(otmp) || !can_stash_objs()))
                || (!strcmp(word, "mark as auto-stash") /* exclude not a container or already auto-stash */
                    && (!(Is_proper_container(otmp) || (Is_container(otmp) && !objects[otmp->otyp].oc_name_known)) || (otmp->speflags & SPEFLAGS_AUTOSTASH) != 0))
                || (!strcmp(word, "unmark as auto-stash") /* exclude if not an auto-stash */
                    && ((otmp->speflags & SPEFLAGS_AUTOSTASH) == 0))
                || (putting_on(word) /* exclude if already worn */
                    && (otmp->owornmask & (W_ARMOR | W_ACCESSORY | W_MISCITEMS)))
                || (trading_items(word) /* exclude if already worn and unpaid items */
                    && ((otmp->owornmask & (W_ARMOR | W_ACCESSORY | W_MISCITEMS)) || otmp->unpaid))
#if 0 /* 3.4.1 -- include currently wielded weapon among 'wield' choices */
                || (!strcmp(word, "wield")
                    && (otmp->owornmask & W_WEP))
#endif
                || (!strcmp(word, "ready")    /* exclude when wielded... */
                    && (((otmp == uwep || (otmp == uarms && u.twoweap)) && otmp->quan == 1L)  /* ...unless more than one */
                        || (uwep && is_launcher(uwep) && objects[uwep->otyp].oc_skill != -objects[otmp->otyp].oc_skill)))
                || ((!strcmp(word, "dip") || !strcmp(word, "grease"))
                    && inaccessible_equipment(otmp, (const char*)0, FALSE))
                )
            {
                foo--;
                foox++;
            }
            /* Second ugly check; unlike the first it won't trigger an
             * "else" in "you don't have anything else to ___".
             */
            else if (
                (validitemfunc && !(*validitemfunc)(otmp))
                || (putting_on(word)
                    && ((otmp->oclass == FOOD_CLASS && otmp->otyp != MEAT_RING)
                        || (otmp->oclass == TOOL_CLASS && otyp != BLINDFOLD
                            && otyp != TOWEL)))
                || (!strcmp(word, "wield")
                    && (otmp->oclass == TOOL_CLASS && !is_weptool(otmp)))
                || (!strcmp(word, "eat") && !is_edible(otmp))
                || (!strcmp(word, "drink") && otmp->oclass == TOOL_CLASS && !is_obj_quaffable(otmp))
                || (!strcmp(word, "sacrifice")
                    && (otyp != CORPSE && otyp != AMULET_OF_YENDOR
                        && otyp != FAKE_AMULET_OF_YENDOR))
                || (!strcmp(word, "write with")
                    && (otmp->oclass == TOOL_CLASS
                        && otyp != MAGIC_MARKER && otyp != TOWEL))
                || (!strcmp(word, "tin")
                    && (otyp != CORPSE || !tinnable(otmp)))
                || (!strcmp(word, "rub")
                    && ((otmp->oclass == TOOL_CLASS && otyp != OIL_LAMP
                        && otyp != MAGIC_LAMP && otyp != BRASS_LANTERN)
                        || (otmp->oclass == GEM_CLASS && !is_graystone(otmp))))
                || (!strcmp(word, "use or apply")
                    /* Picks, axes, pole-weapons, bullwhips */
                    && ((otmp->oclass == WEAPON_CLASS && !is_appliable_weapon(otmp))
                        || (otmp->oclass == POTION_CLASS
                            /* only applicable potion is oil, and it will only
                               be offered as a choice when already discovered */
                            && (otyp != POT_OIL || !otmp->dknown
                                || !objects[POT_OIL].oc_name_known))
                        || (otmp->oclass == FOOD_CLASS
                            && otyp != CREAM_PIE && otyp != EUCALYPTUS_LEAF)
                        || (otmp->oclass == MISCELLANEOUS_CLASS
                            && !is_obj_appliable(otmp))
                        || (otmp->oclass == GEM_CLASS && !is_graystone(otmp))))
                || (!strcmp(word, "invoke")
                    && !otmp->oartifact
                    && !is_otyp_unique(otyp)
                    && !is_otyp_invokable(otyp)
                    && (otyp != FAKE_AMULET_OF_YENDOR || otmp->known)
                    /* note: presenting the possibility of invoking non-artifact
                       mirrors and/or lamps is simply a cruel deception... */
                    && (otyp != OIL_LAMP /* don't list known oil lamp */
                        || (otmp->dknown && objects[OIL_LAMP].oc_name_known)))
                || (!strcmp(word, "untrap with")
                    && ((otmp->oclass == TOOL_CLASS && otyp != CAN_OF_GREASE)
                        || (otmp->oclass == POTION_CLASS
                            /* only applicable potion is oil, and it will only
                               be offered as a choice when already discovered */
                            && (otyp != POT_OIL || !otmp->dknown
                                || !objects[POT_OIL].oc_name_known))))
                || (!strcmp(word, "tip") && !Is_container(otmp)
                    /* include horn of plenty if sufficiently discovered */
                    && (otmp->otyp != HORN_OF_PLENTY || !otmp->dknown
                        || !objects[HORN_OF_PLENTY].oc_name_known))
                || (!strcmp(word, "detect blessedness for") && otmp->bknown)
                || (!strcmp(word, "refill") && !is_refillable_with_oil(otmp))
                || (!strcmp(word, "enchant") && otmp->oclass == TOOL_CLASS && !is_obj_enchantable(otmp))
                || (!strcmp(word, "protect") && otmp->oclass == TOOL_CLASS && !is_obj_enchantable(otmp))
                || (!strcmp(word, "charge") && !is_chargeable(otmp))
                || (!strcmp(word, "fire") && (!uwep || !otmp || (otmp && uwep && !ammo_and_launcher(otmp, uwep))))
                || (!strcmp(word, "open") && otyp != TIN)
                || (!strcmp(word, "call") && !objtyp_is_callable(otyp))
                || (is_dip_into && !otyp_allows_object_to_be_dipped_into_it(otyp))
                || (!strcmp(word, "adjust") && otmp->oclass == COIN_CLASS && !usegold)
                || (!strcmp(word, "attach candles to") && (!is_obj_candelabrum(otmp) || otmp->special_quality >= objects[otmp->otyp].oc_special_quality))
                || (!strcmp(word, "name the type for") && objects[otmp->otyp].oc_name_known)
                )
            {
                foo--;
            }
            /* Third ugly check:  acceptable but not listed as likely
             * candidates in the prompt or in the inventory subset if
             * player responds with '?'.
             */
            else if (
                /* ugly check for unworn armor that can't be worn */
                (putting_on(word) && *let == ARMOR_CLASS
                    && (!flags.exchange_prompt && !canwearobj(otmp, &dummymask, FALSE)))
                /* or armor with 'P' or 'R' or accessory with 'W' or 'T' */
                || ((putting_on(word) || taking_off(word))
                    && ((*let == ARMOR_CLASS) ^ (otmp->oclass == ARMOR_CLASS)))
                /* or unsuitable items rubbed on known touchstone */
                || (!strncmp(word, "rub on the stone", 16)
                    && *let == GEM_CLASS && otmp->dknown
                    && objects[otyp].oc_name_known)
                /* suppress corpses on astral, amulets elsewhere */
                || (!strcmp(word, "sacrifice")
                    /* (!astral && amulet) || (astral && !amulet) */
                    && (!Is_astralevel(&u.uz) ^ (otmp->oclass != AMULET_CLASS)))
                /* suppress container being stashed into */
                || (!strcmp(word, "stash") && !ck_bag(otmp))
                /* worn armor (shirt, suit) covered by worn armor (suit, cloak)
                   or accessory (ring) covered by cursed worn armor (gloves) */
                || (taking_off(word)
                    && inaccessible_equipment(otmp, (const char*)0,
                        (boolean)(otmp->oclass == RING_CLASS)))
                || (!strcmp(word, "write on")
                    && (!(otyp == SCR_BLANK_PAPER || otyp == SPE_BLANK_PAPER)
                        || !otmp->dknown || !objects[otyp].oc_name_known))
                ) {
                /* acceptable but not listed as likely candidate */
                foo--;
                allowall = TRUE;
                *ap++ = otmp->invlet;
            }
            /* *INDENT-ON* */
            /* clang-format on */
        }
        else
        {
            /* "ugly check" for reading fortune cookies, part 2 */
            if ((!strcmp(word, "read") && is_readable(otmp)))
                allowall = usegold = TRUE;
        }

        if (otmp_only && otmp == otmp_only)
            break;
    }
    unsortloot(&sortedinvent);

    bp[foo] = 0;
    if (foo == 0 && bp > buf && bp[-1] == ' ')
        *--bp = 0;
    Strcpy(lets, bp); /* necessary since we destroy buf */
    if (foo > 5)      /* compactify string */
        compactify(bp);
    *ap = '\0';

    if (!strcmp(word, "write on")) 
    { /* ugly check for magic marker */
     /* we wanted all scrolls and books in altlets[], but that came with
        'allowall' which we don't want since it prevents "silly thing"
        result if anything other than scroll or spellbook is chosen */
        allowall = FALSE;
    }

    *foo_ptr = foo;
    *foox_ptr = foox;
    *bp_ptr = bp;
    *allowcnt_ptr = allowcnt;
    *usegold_ptr = usegold;
    *allowall_ptr = allowall;
    *allownone_ptr = allownone;
    *useboulder_ptr = useboulder;
}

boolean
acceptable_getobj_obj(otmp, let, word)
struct obj* otmp;
register const char* let;
register const char* word;
{
    if (!otmp || !let || !word)
        return FALSE;

    register char ilet = otmp->invlet;
    char buf[BUFSZ] = "";
    char lets[BUFSZ], altlets[BUFSZ];
    int foo = 0;
    char* bp = buf;
    xchar allowcnt = 0; /* 0, 1 or 2 */
    boolean usegold = FALSE; /* can't use gold because its illegal */
    boolean allowall = FALSE;
    boolean allownone = FALSE;
    boolean useboulder = FALSE;
    xchar foox = 0;

    construct_getobj_letters(let, word, (boolean(*)(struct obj*))0, lets, altlets, buf, sizeof lets, sizeof altlets, sizeof buf, &foo, &foox, &bp, &allowcnt, &usegold, &allowall, &allownone, &useboulder, otmp);

    return !!index(lets, ilet);
}


void
silly_thing(word, otmp)
const char *word;
struct obj *otmp;
{
#if 1 /* 'P','R' vs 'W','T' handling is obsolete */
    nhUse(otmp);
#else
    const char *s1, *s2, *s3;
    int ocls = otmp->oclass, otyp = otmp->otyp;

    s1 = s2 = s3 = 0;
    /* check for attempted use of accessory commands ('P','R') on armor
       and for corresponding armor commands ('W','T') on accessories */
    if (ocls == ARMOR_CLASS) {
        if (!strcmp(word, "put on"))
            s1 = "W", s2 = "wear", s3 = "";
        else if (!strcmp(word, "remove"))
            s1 = "T", s2 = "take", s3 = " off";
    } else if ((ocls == RING_CLASS || otyp == MEAT_RING)
               || ocls == AMULET_CLASS
               || (otyp == BLINDFOLD || otyp == TOWEL || otyp == LENSES)) {
        if (!strcmp(word, "wear"))
            s1 = "P", s2 = "put", s3 = " on";
        else if (!strcmp(word, "take off"))
            s1 = "R", s2 = "remove", s3 = "";
    }
    if (s1)
        pline("Use the '%s' command to %s %s%s.", s1, s2,
            !(is_plural(otmp) || pair_of(otmp)) ? "that" : "those", s3);
    else
#endif
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        pline(silly_thing_to, word);
}

STATIC_PTR int
ckvalidcat(otmp)
struct obj *otmp;
{
    /* use allow_category() from pickup.c */
    return (int) allow_category(otmp);
}

STATIC_PTR int
ckunpaid(otmp)
struct obj *otmp;
{
    return (otmp->unpaid || (Has_contents(otmp) && count_unpaid(otmp->cobj)));
}

boolean
wearing_armor()
{
    return (boolean) (uarm || uarmc || uarmf || uarmg
                      || uarmh || uarms || uarmu || uarmo || uarmb);
}

boolean
is_worn(otmp)
struct obj *otmp;
{
    return (otmp->owornmask & (W_ARMOR | W_ACCESSORY | W_SADDLE | W_WEAPON))
            ? TRUE
            : FALSE;
}

boolean
is_worn_correctly(otmp)
struct obj* otmp;
{
    if (otmp->owornmask == 0)
        return FALSE;

    if (is_suit(otmp) && (otmp->owornmask & W_ARM) != 0)
        return TRUE;
    if (is_boots(otmp) && (otmp->owornmask & W_ARMF) != 0)
        return TRUE;
    if (is_helmet(otmp) && (otmp->owornmask & W_ARMH) != 0)
        return TRUE;
    if (is_shirt(otmp) && (otmp->owornmask & W_ARMU) != 0)
        return TRUE;
    if (is_robe(otmp) && (otmp->owornmask & W_ARMO) != 0)
        return TRUE;
    if (is_cloak(otmp) && (otmp->owornmask & W_ARMC) != 0)
        return TRUE;
    if (is_gloves(otmp) && (otmp->owornmask & W_ARMG) != 0)
        return TRUE;
    if (is_bracers(otmp) && (otmp->owornmask & W_ARMB) != 0)
        return TRUE;
    if (is_wielded_item(otmp) && (otmp->owornmask & (W_ARMS | W_WEP)) != 0)
        return TRUE;
    if (is_amulet(otmp) && (otmp->owornmask & W_AMUL) != 0)
        return TRUE;
    if (otmp->oclass == RING_CLASS && (otmp->owornmask & W_RING) != 0)
        return TRUE;
    if (otmp->oclass == MISCELLANEOUS_CLASS && (otmp->owornmask & W_MISCITEMS) != 0)
        return TRUE;
    if (otmp->otyp == SADDLE && (otmp->owornmask & W_SADDLE) != 0)
        return TRUE;
    if ((otmp->otyp == BLINDFOLD || otmp->otyp == TOWEL) && (otmp->owornmask & W_BLINDFOLD) != 0)
        return TRUE;

    return FALSE;
}


/* extra xprname() input that askchain() can't pass through safe_qbuf() */
STATIC_VAR struct xprnctx {
    char let;
    boolean dot;
} safeq_xprn_ctx;

/* safe_qbuf() -> short_oname() callback */
STATIC_PTR char *
safeq_xprname(obj)
struct obj *obj;
{
    return xprname(obj, (char *) 0, safeq_xprn_ctx.let, safeq_xprn_ctx.dot,
                   0L, 0L);
}

/* alternate safe_qbuf() -> short_oname() callback */
STATIC_PTR char *
safeq_shortxprname(obj)
struct obj *obj;
{
    return xprname(obj, ansimpleoname(obj), safeq_xprn_ctx.let,
                   safeq_xprn_ctx.dot, 0L, 0L);
}

static NEARDATA const char removeables[] = { ARMOR_CLASS, WEAPON_CLASS,
                                             RING_CLASS,  AMULET_CLASS,
                                             TOOL_CLASS,  0 };

/* Interactive version of getobj - used for Drop, Identify, and Takeoff (A).
   Return the number of times fn was called successfully.
   If combo is TRUE, we just use this to get a category list. */
int
ggetobj(word, fn, mx, combo, resultflags, show_weights)
const char *word;
int FDECL((*fn), (OBJ_P)), mx;
boolean combo; /* combination menu flag */
unsigned *resultflags;
int show_weights;
{
    int FDECL((*ckfn), (OBJ_P)) = (int FDECL((*), (OBJ_P))) 0;
    boolean FDECL((*ofilter), (OBJ_P)) = (boolean FDECL((*), (OBJ_P))) 0;
    boolean takeoff, ident, allflag, m_seen;
    int itemcount;
    int oletct, iletct, unpaid, oc_of_sym;
    char sym, *ip, olets[MAX_OBJECT_CLASSES + 5], ilets[MAX_OBJECT_CLASSES + 10];
    char extra_removeables[5 + 1]; /* uwep,uswapwep,uquiver */
    char buf[BUFSZ] = DUMMY, qbuf[QBUFSZ], ebuf[QBUFSZ];

    if (!invent) {
        You("have nothing to %s.", word);
        if (resultflags)
            *resultflags = ALL_FINISHED;
        return 0;
    }
    if (resultflags)
        *resultflags = 0;
    takeoff = ident = allflag = m_seen = FALSE;
    add_valid_menu_class(0); /* reset */
    if (taking_off(word)) {
        takeoff = TRUE;
        ofilter = is_worn;
    } else if (!strcmp(word, "identify")) {
        ident = TRUE;
        ofilter = not_fully_identified;
    }

    iletct = collect_obj_classes(ilets, invent, FALSE, ofilter, &itemcount);
    unpaid = count_unpaid(invent);

    if (ident && !iletct) {
        return -1; /* no further identifications */
    } else if (invent) {
        ilets[iletct++] = ' ';
        if (unpaid)
            ilets[iletct++] = 'u';
        if (count_buc(invent, BUC_BLESSED, ofilter))
            ilets[iletct++] = 'B';
        if (count_buc(invent, BUC_UNCURSED, ofilter))
            ilets[iletct++] = 'U';
        if (count_buc(invent, BUC_CURSED, ofilter))
            ilets[iletct++] = 'C';
        if (count_buc(invent, BUC_UNKNOWN, ofilter))
            ilets[iletct++] = 'X';
        ilets[iletct++] = 'a';
    }
    ilets[iletct++] = 'i';
    if (!combo)
        ilets[iletct++] = 'm'; /* allow menu presentation on request */
    ilets[iletct] = '\0';

    for (;;) {
        Sprintf(qbuf, "What kinds of thing do you want to %s?", word);
        Sprintf(ebuf, "[%s]", ilets);
        getlin_ex(GETLINE_GENERAL, ATR_NONE, NO_COLOR, qbuf, buf, (char*)0, ebuf, (char*)0);
        if (buf[0] == '\033')
            return 0;
        if (index(buf, 'i')) {
            char ailets[1+26+26+1+5+1]; /* $ + a-z + A-Z + # + slop + \0 */
            struct obj *otmp;

            /* applicable inventory letters; if empty, show entire invent */
            ailets[0] = '\0';
            if (ofilter)
                for (otmp = invent; otmp; otmp = otmp->nobj)
                    /* index() check: limit overflow items to one '#' */
                    if ((*ofilter)(otmp) && !index(ailets, otmp->invlet))
                        (void) strkitten(ailets, otmp->invlet);
            if (display_inventory(ailets, TRUE, show_weights) == '\033')
                return 0;
        } else
            break;
    }

    extra_removeables[0] = '\0';
    if (takeoff) {
        /* arbitrary types of items can be placed in the weapon slots
           [any duplicate entries in extra_removeables[] won't matter] */
        if (uwep)
            (void) strkitten(extra_removeables, uwep->oclass);
        if (uarms)
            (void)strkitten(extra_removeables, uarms->oclass);
        if (uswapwep)
            (void) strkitten(extra_removeables, uswapwep->oclass);
        if (uswapwep2)
            (void)strkitten(extra_removeables, uswapwep2->oclass);
        if (uquiver)
            (void) strkitten(extra_removeables, uquiver->oclass);
    }

    ip = buf;
    olets[oletct = 0] = '\0';
    while ((sym = *ip++) != '\0') {
        if (sym == ' ')
            continue;
        oc_of_sym = def_char_to_objclass(sym);
        if (takeoff && oc_of_sym != MAX_OBJECT_CLASSES) {
            if (index(extra_removeables, oc_of_sym)) {
                ; /* skip rest of takeoff checks */
            } else if (!index(removeables, oc_of_sym)) {
                pline1("Not applicable.");
                return 0;
            } else if (oc_of_sym == ARMOR_CLASS && !wearing_armor()) {
                noarmor(FALSE);
                return 0;
            } else if (oc_of_sym == WEAPON_CLASS && !uwep && !uarms && !uswapwep && !uswapwep2
                       && !uquiver) {
                You1("are not wielding anything.");
                return 0;
            } else if (oc_of_sym == RING_CLASS && !uright && !uleft) {
                You1("are not wearing rings.");
                return 0;
            } else if (oc_of_sym == AMULET_CLASS && !uamul) {
                You1("are not wearing an amulet.");
                return 0;
            } else if (oc_of_sym == MISCELLANEOUS_CLASS && !umisc && !umisc2 && !umisc3 && !umisc4 && !umisc5) {
                You1("are not wearing miscellaneous items.");
                return 0;
            } else if (oc_of_sym == TOOL_CLASS && !ublindf) {
                You1("are not wearing a blindfold.");
                return 0;
            }
        }

        if (oc_of_sym == COIN_CLASS && !combo) {
            context.botl = 1;
        } else if (sym == 'a') {
            allflag = TRUE;
        } else if (sym == 'A') {
            ; /* same as the default */
        } else if (sym == 'u') {
            add_valid_menu_class('u');
            ckfn = ckunpaid;
        } else if (index("BUCX", sym)) {
            add_valid_menu_class(sym); /* 'B','U','C',or 'X' */
            ckfn = ckvalidcat;
        } else if (sym == 'm') {
            m_seen = TRUE;
        } else if (oc_of_sym == MAX_OBJECT_CLASSES) {
            You("don't have any %c's.", sym);
        } else if (oc_of_sym != VENOM_CLASS) { /* suppress venom */
            if (!index(olets, oc_of_sym)) {
                add_valid_menu_class(oc_of_sym);
                olets[oletct++] = oc_of_sym;
                olets[oletct] = 0;
            }
        }
    }

    if (m_seen) {
        return (allflag
                || (!oletct && ckfn != ckunpaid && ckfn != ckvalidcat))
               ? -2 : -3;
    } else if (flags.menu_style != MENU_TRADITIONAL && combo && !allflag) {
        return 0;
#if 0
    /* !!!! test gold dropping */
    } else if (allowgold == 2 && !oletct) {
        return 1; /* you dropped gold (or at least tried to)  */
#endif
    } else {
        int cnt = askchain(&invent, olets, allflag, fn, ckfn, mx, word);
        /*
         * askchain() has already finished the job in this case
         * so set a special flag to convey that back to the caller
         * so that it won't continue processing.
         * Fix for bug C331-1 reported by Irina Rempt-Drijfhout.
         */
        if (combo && allflag && resultflags)
            *resultflags |= ALL_FINISHED;
        return cnt;
    }
}

/*
 * Walk through the chain starting at objchn and ask for all objects
 * with olet in olets (if nonNULL) and satisfying ckfn (if nonnull)
 * whether the action in question (i.e., fn) has to be performed.
 * If allflag then no questions are asked.  Mx gives the max number
 * of objects to be treated.  Return the number of objects treated.
 */
int
askchain(objchn, olets, allflag, fn, ckfn, mx, word)
struct obj **objchn; /* *objchn might change */
int allflag, mx;
const char *olets, *word; /* olets is an Obj Class char array */
int FDECL((*fn), (OBJ_P)), FDECL((*ckfn), (OBJ_P));
{
    struct obj *otmp, *otmpo;
    register char sym, ilet;
    int cnt = 0, dud = 0, tmp;
    boolean takeoff, nodot, ident, take_out, put_in, first, ininv, bycat;
    char qbuf[QBUFSZ], qpfx[QBUFSZ];
    Loot *sortedchn = 0;

    takeoff = taking_off(word);
    ident = !strcmp(word, "identify");
    take_out = !strcmp(word, "take out");
    put_in = !strcmp(word, "put in");
    nodot = (!strcmp(word, "nodot") || !strcmp(word, "drop") || ident
             || takeoff || take_out || put_in);
    ininv = (*objchn == invent);
    bycat = (menu_class_present('u')
             || menu_class_present('B') || menu_class_present('U')
             || menu_class_present('C') || menu_class_present('X'));

    /* someday maybe we'll sort by 'olets' too (temporarily replace
       flags.packorder and pass SORTLOOT_PACK), but not yet... */
    sortedchn = sortloot(objchn, SORTLOOT_INVLET, FALSE,
                         (boolean FDECL((*), (OBJ_P))) 0);

    first = TRUE;
    /*
     * Interrogate in the object class order specified.
     * For example, if a person specifies =/ then first all rings
     * will be asked about followed by all wands.  -dgk
     */
 nextclass:
    ilet = 'a' - 1;
    if (*objchn && (*objchn)->oclass == COIN_CLASS)
        ilet--;                     /* extra iteration */
    /*
     * Multiple Drop can change the invent chain while it operates
     * (dropping a burning potion of oil while levitating creates
     * an explosion which can destroy inventory items), so simple
     * list traversal
     *  for (otmp = *objchn; otmp; otmp = otmp2) {
     *      otmp2 = otmp->nobj;
     *      ...
     *  }
     * is inadequate here.  Use each object's bypass bit to keep
     * track of which list elements have already been processed.
     */
    bypass_objlist(*objchn, FALSE); /* clear chain's bypass bits */
    while ((otmp = nxt_unbypassed_loot(sortedchn, *objchn)) != 0) {
        if (ilet == 'z')
            ilet = 'A';
        else if (ilet == 'Z')
            ilet = NOINVSYM; /* '#' */
        else
            ilet++;
        if (olets && *olets && otmp->oclass != *olets)
            continue;
        if (takeoff && !is_worn(otmp))
            continue;
        if (ident && !not_fully_identified(otmp))
            continue;
        if (ckfn && !(*ckfn)(otmp))
            continue;
        if (bycat && !ckvalidcat(otmp))
            continue;
        if (!allflag) {
            safeq_xprn_ctx.let = ilet;
            safeq_xprn_ctx.dot = !nodot;
            *qpfx = '\0';
            if (first) {
                /* traditional_loot() skips prompting when only one
                   class of objects is involved, so prefix the first
                   object being queried here with an explanation why */
                if (take_out || put_in)
                    Sprintf(qpfx, "%s: ", word), *qpfx = highc(*qpfx);
                first = FALSE;
            }
            (void) safe_qbuf(qbuf, qpfx, "?", otmp,
                             ininv ? safeq_xprname : flags.inventory_weights_last ? doname_with_weight_last_true : doname_with_weight_first_true, //Looks like we are doing a drop here
                             ininv ? safeq_shortxprname : ansimpleoname,
                             "item");
            sym = (takeoff || ident || otmp->quan < 2L) ? nyaq(qbuf)
                                                        : nyNaq(qbuf);
        } else
            sym = 'y';

        otmpo = otmp;
        if (sym == '#') {
            /* Number was entered; split the object unless it corresponds
               to 'none' or 'all'.  2 special cases: cursed loadstones and
               welded weapons (eg, multiple daggers) will remain as merged
               unit; done to avoid splitting an object that won't be
               droppable (even if we're picking up rather than dropping). */
            if (!yn_number) {
                sym = 'n';
            } else {
                sym = 'y';
                if (yn_number < otmp->quan && splittable(otmp))
                    otmp = splitobj(otmp, yn_number);
            }
        }
        switch (sym) {
        case 'a':
            allflag = 1;
            /*FALLTHRU*/
        case 'y':
            tmp = (*fn)(otmp);
            if (tmp < 0) {
                if (container_gone(fn)) {
                    /* otmp caused magic bag to explode;
                       both are now gone */
                    otmp = 0; /* and return */
                } else if (otmp && otmp != otmpo) {
                    /* split occurred, merge again */
                    (void) merged(&otmpo, &otmp);
                }
                goto ret;
            }
            cnt += tmp;
            if (--mx == 0)
                goto ret;
            /*FALLTHRU*/
        case 'n':
            if (nodot)
                dud++;
        default:
            break;
        case 'q':
            /* special case for seffects() */
            if (ident)
                cnt = -1;
            goto ret;
        }
    }
    if (olets && *olets && *++olets)
        goto nextclass;

    if (!takeoff && (dud || cnt))
        pline1("That was all.");
    else if (!dud && !cnt)
        pline1("No applicable objects.");
 ret:
    unsortloot(&sortedchn);
    bypass_objlist(*objchn, FALSE);
    return cnt;
}

/*
 *      Object identification routines:
 */

/* make an object actually be identified; no display updating */
void
fully_identify_obj(otmp)
struct obj *otmp;
{
    makeknown(otmp->otyp);
    if (otmp->oartifact)
    {
        discover_artifact(otmp->oartifact);
        otmp->aknown = 1;
    }
    otmp->known = otmp->dknown = otmp->bknown = otmp->rknown = otmp->mknown = 1;
    
    if (has_oname(otmp))
        otmp->nknown = 1;

    if (Is_container(otmp) || otmp->otyp == STATUE)
        otmp->cknown = otmp->lknown = otmp->tknown = 1;
    if (otmp->otyp == EGG && otmp->corpsenm > NON_PM)
        learn_egg_type(otmp->corpsenm);
    if (is_obj_rotting_corpse(otmp) && otmp->corpsenm > NON_PM)
        learn_corpse_type(otmp->corpsenm);
    if (is_obj_rotting_corpse(otmp))
        otmp->speflags |= SPEFLAGS_ROTTING_STATUS_KNOWN;
}

/* ggetobj callback routine; identify an object and give immediate feedback */
int
identify(otmp)
struct obj *otmp;
{
    fully_identify_obj(otmp);
    prinv((char *) 0, otmp, 0L);
    display_popup_text(xprname(otmp, (char*)0, obj_to_let(otmp), TRUE, 0L, 0UL), "Item Identified", POPUP_TEXT_IDENTIFY, ATR_NONE, NO_COLOR, NO_GLYPH, POPUP_FLAGS_NONE);

    return 1;
}

/* menu of unidentified objects; select and identify up to id_limit of them */
STATIC_OVL int
menu_identify(id_limit)
int id_limit;
{
    int original_id_limit = id_limit;
    menu_item *pick_list;
    int n, i, first = 1, tryct = 5;
    char buf[BUFSZ];
    int res = 0;
    /* assumptions:  id_limit > 0 and at least one unID'd item is present */

    while (id_limit) 
    {
        Sprintf(buf, "What would you like to identify%s?",
            original_id_limit <= 1 ? "" : first ? " first" : " next");

        n = query_objlist(buf, &invent, (SIGNAL_NOMENU | SIGNAL_ESCAPE
                                         | USE_INVLET | INVORDER_SORT),
                          &pick_list, id_limit == 1 ? PICK_ONE : PICK_ANY, not_fully_identified, 0);

        if (n > 0)
        {
            play_sfx_sound(SFX_IDENTIFY_SUCCESS);
            if (n > id_limit)
                n = id_limit;
            for (i = 0; i < n; i++, id_limit--)
                res += identify(pick_list[i].item.a_obj);
            free((genericptr_t) pick_list);
            mark_synch(); /* Before we loop to pop open another menu */
            first = 0;
        } 
        else if (n == -2)
        { /* player used ESC to quit menu */
            if (first)
                return -1;
            break;
        } 
        else if (n == -1)
        { /* no eligible items found */
            pline1("That was all.");
            break;
        }
        else if (!--tryct) { /* stop re-prompting */
            pline1(thats_enough_tries);
            break;
        } 
        else
        { /* try again */
            pline1("Choose an item; use ESC to decline.");
        }
    }
    return res;
}

/* count the unidentified items */
int
count_unidentified(objchn)
struct obj *objchn;
{
    int unid_cnt = 0;
    struct obj *obj;

    for (obj = objchn; obj; obj = obj->nobj)
        if (not_fully_identified(obj))
            ++unid_cnt;
    return unid_cnt;
}

/* dialog with user to identify a given number of items; 0 means all */
/* returns the number of items identified */
int
identify_pack(id_limit, learning_id)
int id_limit; /* 0 = all, >0 max no of items identified */
boolean learning_id; /* true if we just read unknown identify scroll */
{
    struct obj *obj;
    int n, unid_cnt = count_unidentified(invent);
    int res = 0;

    if (!unid_cnt)
    {
        You("have already identified all %sof your possessions.",
            learning_id ? "the rest " : "");
    } 
    else if (!id_limit || id_limit >= unid_cnt)
    {
        play_sfx_sound(SFX_IDENTIFY_SUCCESS);
        /* identify everything */
        /* TODO:  use fully_identify_obj and cornline/menu/whatever here */
        for (obj = invent; obj; obj = obj->nobj) 
        {
            if (not_fully_identified(obj)) 
            {
                res += identify(obj);
                if (unid_cnt == 1)
                    break;
            }
        }
    } 
    else 
    {
        /* identify up to `id_limit' items */
        n = 0;
        if (flags.menu_style == MENU_TRADITIONAL)
        {
            do
            {
                n = ggetobj("identify", identify, id_limit, FALSE,
                    (unsigned*)0, 0);
                if (n < 0)
                    break; /* quit or no eligible items */
                else
                {
                    play_sfx_sound(SFX_IDENTIFY_SUCCESS);
                    res += n;
                }

            } while ((id_limit -= n) > 0);
        }

        if (n == 0 || n < -1)
        {
            int idres = menu_identify(id_limit);
            if (idres == -1)
                return 0;
            res += idres;
        }
    }
    update_inventory();
    return res;
}

/* called when regaining sight; mark inventory objects which were picked
   up while blind as now having been seen */
void
learn_unseen_invent()
{
    struct obj *otmp;

    if (Blind)
        return; /* sanity check */

    for (otmp = invent; otmp; otmp = otmp->nobj) {
        if (otmp->dknown)
            continue; /* already seen */
        /* set dknown, perhaps bknown (for priest[ess]) */
        (void) xname(otmp);
        /*
         * If object->eknown gets implemented (see learnwand(zap.c)),
         * handle deferred discovery here.
         */
    }
    update_inventory();
}

/* should of course only be called for things in invent */
STATIC_OVL char
obj_to_let(obj)
struct obj *obj;
{
    if (!flags.invlet_constant) {
        obj->invlet = NOINVSYM;
        reassign();
    }
    return obj->invlet;
}

/*
 * Print the indicated quantity of the given object.  If quan == 0L then use
 * the current quantity.
 */
void
prinv(prefix, obj, quan)
const char *prefix;
struct obj *obj;
long quan;
{
    if (!prefix)
        prefix = "";
    pline("%s%s%s", prefix, *prefix ? " " : "",
          xprname(obj, (char *) 0, obj_to_let(obj), TRUE, 0L, quan));
}

char *
xprname(obj, txt, let, dot, cost, quan)
struct obj *obj;
const char *txt; /* text to print instead of obj */
char let;        /* inventory letter */
boolean dot;     /* append period; (dot && cost => Iu) */
long cost;       /* cost (for inventory of unpaid or expended items) */
long quan;       /* if non-0, print this quantity, not obj->quan */
{
#ifdef LINT /* handle static char li[BUFSZ]; */
    char li[BUFSZ];
#else
    static char li[BUFSZ];
#endif
    boolean use_invlet = (flags.invlet_constant
                          && let != CONTAINED_SYM && let != HANDS_SYM);
    long savequan = 0;

    if (quan && obj) {
        savequan = obj->quan;
        obj->quan = quan;
    }

    
    /*
     * If let is:
     *  -  Then obj == null and 'txt' refers to hands or fingers.
     *  *  Then obj == null and we are printing a total amount.
     *  >  Then the object is contained and doesn't have an inventory letter.
     */
    if (cost != 0 || let == '*') {
        /* if dot is true, we're doing Iu, otherwise Ix */
        Sprintf(li,

                    (iflags.menu_tab_sep ? "%c - %s\t%6ld %s"
                                    : "%c - %-45s %6ld %s"),
            (dot && use_invlet ? obj->invlet : let),
            (txt ? txt : doname(obj)), cost, currency(cost));
    } else {
        /* ordinary inventory display or pickup message */
        Sprintf(li, 
            "%c - %s%s", (use_invlet ? obj->invlet : let),
            (txt ? txt : doname(obj)), (dot ? "." : ""));
    }
    if (savequan)
        obj->quan = savequan;

    return li;
}

extern struct ext_func_tab extcmdlist[];

/* the 'i' command */
int
ddoinv()
{
    //(void) display_inventory((char *) 0, FALSE, 1);

    char invlet;
    long pickcnt = 0;

    invlet = display_inventory_with_header((const char*)0, TRUE, &pickcnt, 1, FALSE);
    if (!invlet || invlet == '\033' || invlet == '\0')
        return 0;

    if (flags.inventory_obj_cmd)
    {
        return display_item_command_menu_by_invlet(invlet, pickcnt);
    }
    else
    {
        struct obj* invobj;
        for (invobj = invent; invobj; invobj = invobj->nobj)
            if (invobj->invlet == invlet)
            {
                (void)itemdescription(invobj);
                break;
            }
        return 0;
    }
}

/* the ']' command */
int
doseeworn()
{
    char invlet;
    long pickcnt = 0;

    invlet = display_inventory_with_header((const char*)0, TRUE, &pickcnt, 1, TRUE);
    if (!invlet || invlet == '\033' || invlet == '\0')
        return 0;

    if (flags.inventory_obj_cmd)
    {
        return display_item_command_menu_by_invlet(invlet, pickcnt);
    }
    else
    {
        struct obj* invobj;
        for (invobj = invent; invobj; invobj = invobj->nobj)
            if (invobj->invlet == invlet)
            {
                (void)itemdescription(invobj);
                break;
            }
        return 0;
    }
}


int
display_item_command_menu_by_invlet(invlet, pickcnt)
char invlet;
long pickcnt;
{
    struct obj* otmp = 0;
    struct obj* otmp2;
    for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
        if (otmp2->invlet == invlet)
            otmp = otmp2;

    if (!otmp)
        return 0;
    else
        return display_item_command_menu(otmp, pickcnt);
}

int
display_item_command_menu(otmp, pickcnt)
struct obj* otmp;
long pickcnt;
{
    if (!otmp)
        return 0;

    int cmd_idx = 0;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    xchar x = 0, y = 0;
    get_obj_location(otmp, &x, &y, CONTAINED_TOO | BURIED_TOO);
    int glyph = obj_to_glyph(otmp, rn2_on_display_rng);
    int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_OBJECT, otmp, (struct monst*)0, 0UL));

    any = zeroany;
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_OBJECT_COMMAND_MENU, iflags.using_gui_tiles ? gui_glyph : glyph, extended_create_window_info_from_obj(otmp));
    start_menu_ex(win, GHMENU_STYLE_ITEM_COMMAND);

#define NUM_CMD_SECTIONS 3

    const char* headings[NUM_CMD_SECTIONS] = { "Information", "General Commands", "Item-Specific Commands" };
    unsigned long section_flags[NUM_CMD_SECTIONS] = { SINGLE_OBJ_CMD_INFO, SINGLE_OBJ_CMD_GENERAL, SINGLE_OBJ_CMD_SPECIFIC };
    char buf[BUFSIZ] = "";
    char cmdbuf[BUFSZ] = "";
    char shortcutbuf[BUFSZ] = "";
    char headerbuf[BUFSZ] = "";
    register const struct ext_func_tab* efp;
    int actioncount = 0;
    char class_list[BUFSZ] = "";
    int longest_len = 0;
    int longest_len_header = 0;
    int slen = 0;
    unsigned long allflags = 0UL;
    int i, j;
    for (j = 0; j < NUM_CMD_SECTIONS; j++)
    {
        allflags |= section_flags[j];

        slen = (int)strlen(headings[j]);
        if (slen > longest_len_header)
            longest_len_header = slen;

    }
    for (i = 0; extcmdlist[i].ef_txt; i++)
    {
        if (!(extcmdlist[i].flags & allflags) || !extcmdlist[i].getobj_word)
            continue;

        slen = (int)strlen(extcmdlist[i].ef_txt_word ? extcmdlist[i].ef_txt_word : extcmdlist[i].ef_txt);
        if (slen > longest_len)
            longest_len = slen;
    }

    for (j = 0; j < NUM_CMD_SECTIONS; j++)
    {
        int cnt = 0;
        for (i = 0; extcmdlist[i].ef_txt; i++)
        {
            if (!(extcmdlist[i].flags & section_flags[j]) || !extcmdlist[i].getobj_word)
                continue;

            strcpy(class_list, "");
            if (extcmdlist[i].getobj_classes)
                strcpy(class_list, extcmdlist[i].getobj_classes);
            else if (!strcmp(extcmdlist[i].getobj_word, "break"))
                setbreakclasses(class_list);
            else if (!strcmp(extcmdlist[i].getobj_word, "use or apply"))
                setapplyclasses(class_list);
            else  if (!strcmp(extcmdlist[i].getobj_word, "ready"))
            {
                strcpy(class_list, (uslinging()
                    || (uswapwep
                        && objects[uswapwep->otyp].oc_skill == P_SLING))
                    ? getobj_bullets
                    : getobj_ready_objs);
            }

            if (!strcmp(extcmdlist[i].getobj_word, "stash"))
                set_current_container_to_dummyobj();
            boolean acceptable = acceptable_getobj_obj(otmp, class_list, extcmdlist[i].getobj_word);
            if (!strcmp(extcmdlist[i].getobj_word, "stash"))
                set_current_container_to_null();
            if (!acceptable)
                continue;

            cnt++;
        }
        if (!cnt)
            continue;

        char catbuf[BUFSZ];
        strcpy(catbuf, "");
        if (!iflags.menu_tab_sep)
        {
            slen = (int)strlen(headings[j]);
            int k;
            for (k = 0; k < max(longest_len_header, longest_len + 10) - slen; k++)
                Sprintf(eos(catbuf), "%s", " ");
        }

        char hbuf[BUFSZ];
        Sprintf(hbuf, "%s%s", headings[j], catbuf);

        any = zeroany;
        add_extended_menu(win, NO_GLYPH, &any, menu_heading_info(),
            0, 0, iflags.menu_headings,
            hbuf, MENU_UNSELECTED);

        for (i = 0; extcmdlist[i].ef_txt; i++)
        {
            if (!(extcmdlist[i].flags & section_flags[j]) || !extcmdlist[i].getobj_word)
                continue;

            strcpy(class_list, "");
            if (extcmdlist[i].getobj_classes)
                strcpy(class_list, extcmdlist[i].getobj_classes);
            else if (!strcmp(extcmdlist[i].getobj_word, "break"))
                setbreakclasses(class_list);
            else if (!strcmp(extcmdlist[i].getobj_word, "use or apply"))
                setapplyclasses(class_list);
            else  if (!strcmp(extcmdlist[i].getobj_word, "ready"))
            {
                strcpy(class_list, (uslinging()
                    || (uswapwep
                        && objects[uswapwep->otyp].oc_skill == P_SLING))
                    ? getobj_bullets
                    : getobj_ready_objs);
            }

            if (!strcmp(extcmdlist[i].getobj_word, "stash"))
                set_current_container_to_dummyobj();
            boolean acceptable = acceptable_getobj_obj(otmp, class_list, extcmdlist[i].getobj_word);
            if (!strcmp(extcmdlist[i].getobj_word, "stash"))
                set_current_container_to_null();
            if (!acceptable)
                continue;

            efp = &extcmdlist[i];
            any = zeroany;
            any.a_int = i + 1;
            if (efp->ef_txt_word)
                strcpy(cmdbuf, efp->ef_txt_word);
            else
                strcpy(cmdbuf, efp->ef_txt);

            *cmdbuf = highc(*cmdbuf);

            uchar altmask = 0x80;
            uchar ctrlmask = 0x20 | 0x40;

            char tabbuf[BUFSZ];
            if (iflags.menu_tab_sep)
                strcpy(tabbuf, "\t");
            else
            {
                strcpy(tabbuf, "");
                slen = (int)strlen(cmdbuf);
                int k;
                for (k = 0; k < longest_len + 2 - slen; k++)
                    Sprintf(eos(tabbuf), "%s", " ");
            }

            if (efp->bound_key != '\0' && !(efp->bound_key >= (uchar)M(0) && efp->bound_key <= (uchar)M(9)))
                Sprintf(shortcutbuf, "%s(%s%c)", tabbuf,
                    (efp->bound_key & ctrlmask) == 0 ? "Ctrl-" : (efp->bound_key & altmask) == altmask ? "Alt-" : "",
                    (efp->bound_key & ctrlmask) == 0 ? efp->bound_key | ctrlmask : (efp->bound_key & altmask) == altmask ? efp->bound_key & ~altmask : efp->bound_key);
            else
                strcpy(shortcutbuf, "");

            Sprintf(buf, "%s%s", cmdbuf, shortcutbuf);

            add_menu(win, NO_GLYPH, &any,
                0, 0, ATR_NONE,
                buf, MENU_UNSELECTED);

            actioncount++;
        }
    }

    char obuf[BUFSZ];
    struct obj otmp_cnt = *otmp;
    if (pickcnt > 0 && pickcnt < otmp->quan)
        otmp_cnt.quan = pickcnt;
    Strcpy(obuf, short_oname(&otmp_cnt, doname, thesimpleoname, BUFSZ));
    Sprintf(headerbuf, "What do you want to do with %s?", obuf);

    end_menu(win, headerbuf);


    if (actioncount <= 0)
    {
        You("can't take any actions with the %s.", cxname(otmp));
        destroy_nhwindow(win);
        return 0;
    }

    if (select_menu(win, PICK_ONE, &pick_list) > 0)
    {
        cmd_idx = pick_list->item.a_int;
        free((genericptr_t)pick_list);
    }
    destroy_nhwindow(win);

    if (cmd_idx < 1)
        return 0;

    int res = 0;
    int selected_action = cmd_idx - 1;
    if (extcmdlist[selected_action].ef_funct && pickcnt != 0)
    {
        if (pickcnt <= -1 || pickcnt >= otmp->quan)
            getobj_autoselect_obj = otmp;
        else
        {
            struct obj* otmpsplit = splitobj(otmp, pickcnt);
            getobj_autoselect_obj = otmpsplit;
        }
        res = (extcmdlist[selected_action].ef_funct)();
        getobj_autoselect_obj = (struct obj*)0;
    }

    return res;
}


int
dolastpickeditem()
{
    struct obj* selobj = 0;
    if (context.last_picked_obj_oid > 0)
    {
        struct obj* otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj)
        {
            if (otmp->o_id == context.last_picked_obj_oid)
            {
                selobj = otmp;
                break;
            }
        }
    }
    if (selobj)
    {
        int ret = display_item_command_menu(selobj, -1);
//        if (ret)
            context.last_picked_obj_show_duration_left++;
        return ret;
    }
    else
    {
        pline1("There is no last picked object.");
        return 0;
    }
}

/*
 * find_unpaid()
 *
 * Scan the given list of objects.  If last_found is NULL, return the first
 * unpaid object found.  If last_found is not NULL, then skip over unpaid
 * objects until last_found is reached, then set last_found to NULL so the
 * next unpaid object is returned.  This routine recursively follows
 * containers.
 */
STATIC_OVL struct obj *
find_unpaid(list, last_found)
struct obj *list, **last_found;
{
    struct obj *obj;

    while (list) {
        if (list->unpaid) {
            if (*last_found) {
                /* still looking for previous unpaid object */
                if (list == *last_found)
                    *last_found = (struct obj *) 0;
            } else
                return ((*last_found = list));
        }
        if (Has_contents(list)) {
            if ((obj = find_unpaid(list->cobj, last_found)) != 0)
                return obj;
        }
        list = list->nobj;
    }
    return (struct obj *) 0;
}

/* for perm_invent when operating on a partial inventory display, so that
   the persistent one doesn't get shrunk during filtering for item selection
   then regrown to full inventory, possibly being resized in the process */
static winid cached_pickinv_win = WIN_ERR;

void
free_pickinv_cache()
{
    if (cached_pickinv_win != WIN_ERR) {
        destroy_nhwindow(cached_pickinv_win);
        cached_pickinv_win = WIN_ERR;
    }
}

/*
 * Internal function used by display_inventory and getobj that can display
 * inventory and return a count as well as a letter. If out_cnt is not null,
 * any count returned from the menu selection is placed here.
 */
STATIC_OVL char
display_pickinv(lets, xtra_choice, query, want_reply, out_cnt, show_weights, headertext, addinventoryheader, wornonly)
const char *lets;
char *xtra_choice; /* "fingers", pick hands rather than an object */
char *query;
boolean want_reply;
long *out_cnt;
int show_weights;
const char* headertext;
boolean addinventoryheader, wornonly;
{
    static const char not_carrying_anything[] = "Not carrying anything";
    struct obj *otmp, wizid_fakeobj;
    char ilet, ret;
    char *invlet = flags.inv_order;
    int n, classcount;
    winid win;                        /* windows being used */
    anything any;
    menu_item *selected;
    unsigned sortflags;
    Loot *sortedinvent, *srtinv;
    boolean wizid = FALSE;
    int wtcount = 0;

    boolean loadstonecorrectly = FALSE;

    if(show_weights == 1) // Inventory
        loadstonecorrectly = TRUE;
    else if (show_weights == 2) 
    { // Pick up
        loadstonecorrectly = (boolean)objects[LOADSTONE].oc_name_known;
    }
    else if (show_weights == 3) // Drop
        loadstonecorrectly = TRUE;

    if (lets && !*lets)
        lets = 0; /* simplify tests: (lets) instead of (lets && *lets) */

    if (iflags.perm_invent && (lets || xtra_choice)) 
    {
        /* partial inventory in perm_invent setting; don't operate on
           full inventory window, use an alternate one instead; create
           the first time needed and keep it for re-use as needed later */
        if (cached_pickinv_win == WIN_ERR)
            cached_pickinv_win = create_nhwindow(NHW_MENU);
        win = cached_pickinv_win;
    } 
    else
        win = WIN_INVEN;

    /*
     * Exit early if no inventory -- but keep going if we are doing
     * a permanent inventory update.  We need to keep going so the
     * permanent inventory window updates itself to remove the last
     * item(s) dropped.  One down side:  the addition of the exception
     * for permanent inventory window updates _can_ pop the window
     * up when it's not displayed -- even if it's empty -- because we
     * don't know at this level if its up or not.  This may not be
     * an issue if empty checks are done before hand and the call
     * to here is short circuited away.
     *
     * 2: our count here is only to distinguish between 0 and 1 and
     * more than 1; for the last one, we don't need a precise number.
     * For perm_invent update we force 'more than 1'.
     */
    n = (iflags.perm_invent && !lets && !want_reply) ? 2
        : lets ? (int) strlen(lets)
               : !invent ? 0 : !invent->nobj ? 1 : 2;
    /* for xtra_choice, there's another 'item' not included in initial 'n';
       for !lets (full invent) and for override_ID (wizard mode identify),
       skip message_menu handling of single item even if item count was 1 */
    if (xtra_choice || (n == 1 && (!lets || iflags.override_ID)))
        ++n;

    if (n == 0) 
    {
        pline("%s.", not_carrying_anything);
        return 0;
    }

    /* oxymoron? temporarily assign permanent inventory letters */
    if (!flags.invlet_constant)
        reassign();

    if (n == 1 && !iflags.force_invmenu) 
    {
        /* when only one item of interest, use pline instead of menus;
           we actually use a fake message-line menu in order to allow
           the user to perform selection at the --More-- prompt for tty */
        ret = '\0';
        if (xtra_choice) 
        {
            /* xtra_choice is "bare hands" (wield), "fingertip" (Engrave),
               "nothing" (ready Quiver), or "fingers" (apply grease) */
            ret = message_menu(HANDS_SYM, PICK_ONE,
                               xprname((struct obj *) 0, xtra_choice,
                                       HANDS_SYM, TRUE, 0L, 0L)); /* '-' */
        } 
        else 
        {
            for (otmp = invent; otmp; otmp = otmp->nobj)
                if (!lets || otmp->invlet == lets[0])
                    break;
            if (otmp)
                ret = message_menu(otmp->invlet,
                                   want_reply ? PICK_ONE : PICK_NONE,
                                   xprname(otmp, (char *) 0, lets[0],
                                           TRUE, 0L, 0L));
        }
        if (out_cnt)
            *out_cnt = -1L; /* select all */
        return ret;
    }

    sortflags = (flags.sortloot == 'f') ? SORTLOOT_LOOT : SORTLOOT_INVLET;
    if (flags.sortpack)
        sortflags |= SORTLOOT_PACK;
    sortedinvent = sortloot(&invent, sortflags, FALSE,
                            (boolean FDECL((*), (OBJ_P))) 0);

    start_menu_ex(win, addinventoryheader ? GHMENU_STYLE_INVENTORY : GHMENU_STYLE_PICK_ITEM_LIST);
    any = zeroany;
    if (wizard && iflags.override_ID) 
    {
        int unid_cnt;
        char prompt[QBUFSZ];

        unid_cnt = count_unidentified(invent);
        Sprintf(prompt, "Debug Identify"); /* 'title' rather than 'prompt' */
        if (unid_cnt)
            Sprintf(eos(prompt),
                    " -- unidentified or partially identified item%s",
                    plur(unid_cnt));
        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, prompt, MENU_UNSELECTED);
        if (!unid_cnt) 
        {
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE,
                     "(all items are permanently identified already)",
                     MENU_UNSELECTED);
        } 
        else 
        {
            any.a_obj = &wizid_fakeobj;
            Sprintf(prompt, "select %s to permanently identify",
                    (unid_cnt == 1) ? "it": "any or all of them");
            /* wiz_identify stuffed the wiz_identify command character (^I)
               into iflags.override_ID for our use as an accelerator;
               it could be ambiguous if player has assigned a letter to
               the #wizidentify command */
            if (unid_cnt > 1)
                Sprintf(eos(prompt), " (%s for all)",
                        visctrl(iflags.override_ID));
            add_menu(win, NO_GLYPH, &any, '_', iflags.override_ID, ATR_NONE,
                     prompt, MENU_UNSELECTED);
            wizid = TRUE;
        }
    } 
    else if (xtra_choice) 
    {
        /* wizard override ID and xtra_choice are mutually exclusive */
        if (flags.sortpack)
            add_extended_menu(win, NO_GLYPH, &any, menu_heading_info(), 0, 0, iflags.menu_headings,
                     "Miscellaneous", MENU_UNSELECTED);
        any.a_char = HANDS_SYM; /* '-' */
        add_menu(win, NO_GLYPH, &any, HANDS_SYM, 0, ATR_NONE,
                 xtra_choice, MENU_UNSELECTED);
    }

#if !defined(GNH_MOBILE)
   if(strcmp(headertext, "") != 0)
   {
       add_menu(win, NO_GLYPH, &any, ' ', 0, ATR_NONE,
           headertext, MENU_UNSELECTED);
       add_menu(win, NO_GLYPH, &any, ' ', 0, ATR_NONE,
           "", MENU_UNSELECTED);
   }
#endif
nextclass:
    classcount = 0;
    for (srtinv = sortedinvent; (otmp = srtinv->obj) != 0; ++srtinv) 
    {
        if (lets && !index(lets, otmp->invlet))
            continue;
        if (wornonly && !otmp->owornmask)
            continue;
        if (!flags.sortpack || otmp->oclass == *invlet) 
        {
            if (wizid && !not_fully_identified(otmp))
                continue;
            any = zeroany; /* all bits zero */
            ilet = otmp->invlet;
            if (flags.sortpack && !classcount) 
            {
                add_extended_menu(win, NO_GLYPH, &any, 
                    menu_group_heading_info(*invlet > ILLOBJ_CLASS && *invlet < MAX_OBJECT_CLASSES ? def_oc_syms[(int)(*invlet)].sym : '\0'),
                    0, 0, iflags.menu_headings,
                         let_to_name(*invlet, FALSE,
                                     (want_reply && iflags.menu_head_objsym)),
                         MENU_UNSELECTED);
                classcount++;
            }
            if (wizid)
                any.a_obj = otmp;
            else
                any.a_char = ilet;

            /*calculate weight sum here*/
            if(otmp->otyp == LOADSTONE && !loadstonecorrectly)
                wtcount += objects[LUCKSTONE].oc_weight;
            else
                wtcount += otmp->owt;

            char applied_class_accelerator = wizid ? def_oc_syms[(int)otmp->oclass].sym : 0;

            int glyph = obj_to_glyph(otmp, rn2_on_display_rng);
            int gui_glyph = maybe_get_replaced_glyph(glyph, u.ux, u.uy, data_to_replacement_info(glyph, LAYER_OBJECT, otmp, (struct monst*)0, 0UL));
            add_extended_menu(win, iflags.using_gui_tiles ? gui_glyph : glyph, &any, obj_to_extended_menu_info(otmp), ilet,
                applied_class_accelerator,
                     ATR_NONE, show_weights > 0 ? (flags.inventory_weights_last ? doname_with_weight_last(otmp, loadstonecorrectly) : doname_with_weight_first(otmp, loadstonecorrectly)) : doname(otmp), MENU_UNSELECTED);
        }
    }
    if (flags.sortpack) 
    {
        if (*++invlet)
            goto nextclass;
        if (--invlet != venom_inv) 
        {
            invlet = venom_inv;
            goto nextclass;
        }
    }
    if (iflags.force_invmenu && lets && want_reply) 
    {
        any = zeroany;
        add_extended_menu(win, NO_GLYPH, &any, menu_heading_info(), 0, 0, iflags.menu_headings,
                 "Special", MENU_UNSELECTED);
        any.a_char = '*';
        add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE,
                 "(list everything)", MENU_UNSELECTED);
    }
    unsortloot(&sortedinvent);
    /* for permanent inventory where we intend to show everything but
       nothing has been listed (because there isn't anyhing to list;
       recognized via any.a_char still being zero; the n==0 case above
       gets skipped for perm_invent), put something into the menu */
    if (iflags.perm_invent && !lets && !any.a_char) 
    {
        any = zeroany;
        add_menu(win, NO_GLYPH, &any, 0, 0, 0,
                 not_carrying_anything, MENU_UNSELECTED);
        want_reply = FALSE;
    }
    else
    {
        if (flags.show_weight_summary)
            add_weight_summary(win, wtcount, show_weights);
    }

    if (addinventoryheader)
    {
        char qbuf[BUFSIZ], subtitlebuf[BUFSIZ];
        int icnt = inv_cnt_ex(FALSE, wornonly);
        char weightbuf[BUFSZ];
        printweight(weightbuf, wtcount, FALSE, FALSE);

        int maxwt = enclevelmaximumweight(UNENCUMBERED);
        char maxbuf[BUFSZ];
        printweight(maxbuf, maxwt, FALSE, FALSE);

        if(wornonly)
            strcpy(qbuf, "Worn Items");
        else
            strcpy(qbuf, "Inventory");

        Sprintf(subtitlebuf, "%d/52 slots, %s/%s weight", icnt, weightbuf, maxbuf);
        end_menu_ex(win, qbuf, subtitlebuf);
    }
    else
    {
#if !defined(GNH_MOBILE)
        end_menu(win, query && *query ? query : (char*)0);
#else
        end_menu_ex(win, query && *query ? query : (char*)0, headertext && strcmp(headertext, "") != 0 ? headertext : (char*)0);
#endif
    }

    n = select_menu(win,
                    wizid ? PICK_ANY : want_reply ? PICK_ONE : PICK_NONE,
                    &selected);
    if (n > 0) 
    {
        if (wizid) 
        {
            int i;

            ret = '\0';
            for (i = 0; i < n; ++i) 
            {
                otmp = selected[i].item.a_obj;
                if (otmp == &wizid_fakeobj) 
                {
                    (void)identify_pack(0, FALSE);
                } 
                else 
                {
                    if (not_fully_identified(otmp))
                    {
                        play_sfx_sound(SFX_IDENTIFY_SUCCESS);
                        (void)identify(otmp);
                    }
                }
            }
        } 
        else 
        {
            ret = selected[0].item.a_char;
            if (out_cnt)
                *out_cnt = selected[0].count;
        }
        free((genericptr_t) selected);
    } 
    else
        ret = !n ? '\0' : '\033'; /* cancelled */

    return ret;
}

void
add_weight_summary(win, total_ounce_weight, show_weights)
winid win;
int total_ounce_weight;
int show_weights;
{
    if (show_weights > 0)
    {
        anything any = zeroany;
        add_extended_menu(win, NO_GLYPH, &any, menu_heading_info(), 0, 0, iflags.menu_headings,
            "Weight Summary", MENU_UNSELECTED);

        char wtbuf[BUFSZ];
        char carrybuf[BUFSZ];
        char totalbuf[BUFSZ];

        int iw, wc, yourweight, yourenclevel, curlevelminweight = 0, curlevelmaxweight = 0;
        //        double curlevelminweight_lbs = 0, curlevelmaxweight_lbs = 0;

        iw = inv_weight();
        wc = weight_cap();
        yourenclevel = near_capacity();
        curlevelminweight = enclevelminimumweight(yourenclevel);
        curlevelmaxweight = enclevelmaximumweight(yourenclevel);

        char curlevelminbuf[BUFSZ] = "";
        printweight(curlevelminbuf, curlevelminweight, FALSE, FALSE);

        char curlevelmaxbuf[BUFSZ] = "";
        printweight(curlevelmaxbuf, curlevelmaxweight, FALSE, FALSE);

        //        curlevelminweight_lbs = ((double)curlevelminweight) / 16; //ounces to lbs
        //        curlevelmaxweight_lbs = ((double)curlevelmaxweight) / 16; //ounces to lbs
        yourweight = iw + wc;

        //        double carryingweight = ((double)yourweight) / 16; //ounces to lbs
        //        double burdnedweightlimit = ((double)wc) / 16; //ounces to lbs

        char carryingbuf[BUFSZ] = "";
        printweight(carryingbuf, yourweight, FALSE, FALSE);

        //        double totalweight = ((double)total_ounce_weight) / 16; //ounces to lbs

        const char* burdentype[] = { "unencumbered", "burdened",
                                             "stressed",     "strained",
                                             "overtaxed",    "overloaded" };

        const char* verb = burdentype[yourenclevel];

        // Inventory show_weights = 1
        // Pick up show_weights = 2
        // Drop show_weights = 3
        // Inventory show_weights, but no You are-line = 4
        // Pick up show_weights, but no You are-line = 5
        // Drop show_weights, but no You are-line = 6

        boolean tiles_being_used = FALSE;
#if 0
        tiles_being_used = TRUE;
#endif

        if (total_ounce_weight > 0)
        {
            char weightbuf[BUFSZ];
            printweight(weightbuf, total_ounce_weight, !flags.inventory_weights_last, FALSE);
            if (flags.inventory_weights_last || tiles_being_used)
                Sprintf(wtbuf, "%s of total weight.", weightbuf);
            else
                Sprintf(wtbuf, "  = %s of total weight", weightbuf);

            add_menu(win, NO_GLYPH, &any, 0, 0, 0, wtbuf, MENU_UNSELECTED);
        }

        if (show_weights > 0 && show_weights <= 3)
        {
            //Back end of printout
            if (yourenclevel == UNENCUMBERED)
                Sprintf(wtbuf, "%s until %s.", verb, curlevelmaxbuf); //(int)curlevelmaxweight_lbs, (int)curlevelmaxweight_lbs == 1 ? "lb" : "lbs");
            else if (yourenclevel == OVERLOADED)
                Sprintf(wtbuf, "%s with at least %s.", verb, curlevelminbuf); // (int)curlevelminweight_lbs, (int)curlevelminweight_lbs == 1 ? "lb" : "lbs");
            else
                Sprintf(wtbuf, "%s between %s and %s.", verb, curlevelminbuf, curlevelmaxbuf); // (int)curlevelminweight_lbs, (int)curlevelmaxweight_lbs, ((int)curlevelminweight_lbs == 1 && (int)curlevelmaxweight_lbs == 1) ? "lb" : "lbs");


            //Front end of printout
            if (show_weights == 1 || (show_weights == 2 && total_ounce_weight == yourweight))
            {
                Sprintf(carrybuf, "%s", "You are ");
            }
            else
            {
                Sprintf(carrybuf, "You are carrying %s and ", carryingbuf);
#if 0
                if (carryingweight >= 10)
                    Sprintf(carrybuf, "You are carrying %d %s and ", (int)carryingweight, (int)carryingweight == 1 ? "lb" : "lbs");
                else
                    Sprintf(carrybuf, "You are carrying %1.1f %s and ", carryingweight, carryingweight == 1 ? "lb" : "lbs");
#endif
            }

            Sprintf(totalbuf, "%s%s", carrybuf, wtbuf);
            add_menu(win, NO_GLYPH, &any, 0, 0, 0, totalbuf, MENU_UNSELECTED);
        }
    }
}

int
enclevelminimumweight(enclevel)
enum encumbrance_types enclevel;
{
    int weight;
    int wt = weight_cap();

    if (enclevel == UNENCUMBERED)
        weight = 0;
    else
        weight = wt + (enclevel - 1) * wt / 2;
    return weight;
}

int
enclevelmaximumweight(enclevel)
enum encumbrance_types enclevel;
{
    int weight;
    int wt = weight_cap();

    if (enclevel == OVERLOADED)
        weight = 1000000;
    else
        weight = (wt + enclevel * wt / 2 ) - 1;

    return weight;
}

void
add_weight_summary_putstr(win, total_ounce_weight, show_weights)
winid win;
int total_ounce_weight;
int show_weights;
{
    if (show_weights > 0)
    {
        char buf[BUFSZ];
        char wtbuf[BUFSZ];
        char carrybuf[BUFSZ];
        char totalbuf[BUFSZ];

        int iw, wc, yourweight, yourenclevel, curlevelminweight = 0, curlevelmaxweight = 0;
//        double curlevelminweight_lbs = 0, curlevelmaxweight_lbs = 0;
        iw = inv_weight();
        wc = weight_cap();
        yourenclevel = near_capacity();
        curlevelminweight = enclevelminimumweight(yourenclevel);
        curlevelmaxweight = enclevelmaximumweight(yourenclevel);
//        curlevelminweight_lbs = ((double)curlevelminweight) / 16; //ounces to lbs
//        curlevelmaxweight_lbs = ((double)curlevelmaxweight) / 16; //ounces to lbs

        char curlevelminbuf[BUFSZ] = "";
        printweight(curlevelminbuf, curlevelminweight, FALSE, FALSE);

        char curlevelmaxbuf[BUFSZ] = "";
        printweight(curlevelmaxbuf, curlevelmaxweight, FALSE, FALSE);

        yourweight = iw + wc;

        const char* burdentype[] = { "unencumbered", "burdened",
                                             "stressed",     "strained",
                                             "overtaxed",    "overloaded" };

        const char* verb = burdentype[yourenclevel];

        // Inventory show_weights = 1
        // Pick up show_weights = 2
        // Drop show_weights = 3
        // Inventory show_weights, but no You are-line = 4
        // Pick up show_weights, but no You are-line = 5
        // Drop show_weights, but no You are-line = 6


        //NOTE: Nested container listing should not be used with show_weights on
//        double carryingweight = ((double)yourweight) / 16; // ounces to lbs
        char carryingweightbuf[BUFSZ] = "";
        printweight(carryingweightbuf, yourweight, !flags.inventory_weights_last, FALSE);
        //        double unburdenedweight = ((double)(weight_cap())) / 16; // ounces to lbs
//        double totalweight = ((double)total_ounce_weight) / 16; // ounces to lbs
        char totalweightbuf[BUFSZ] = "";
        printweight(totalweightbuf, total_ounce_weight, FALSE, FALSE);

        if (flags.inventory_weights_last)
            Sprintf(buf, "There is %s of total weight.", totalweightbuf);
        else
            Sprintf(buf, "   = %s of total weight", totalweightbuf);

#if 0
        if (totalweight >= 1000)
            Sprintf(buf, "   = %3.0f %s of total weight", totalweight / 1000, "cwt");
        else if (totalweight >= 10)
            Sprintf(buf, "   = %3.0f %s of total weight", totalweight, totalweight == 1 ? "lb" : "lbs");
        else
            Sprintf(buf, "   = %1.1f %s of total weight", totalweight, totalweight == 1 ? "lb" : "lbs");
#endif

        putstr(win, 0, buf);


        if (show_weights > 0 && show_weights <= 3)
        {
            //Back end of printout
            if (yourenclevel == UNENCUMBERED)
                Sprintf(wtbuf, "%s until %s.", verb, curlevelmaxbuf); // (int)curlevelmaxweight_lbs, (int)curlevelmaxweight_lbs == 1 ? "lb" : "lbs");
            else if (yourenclevel == OVERLOADED)
                Sprintf(wtbuf, "%s with at least %s.", verb, curlevelminbuf); // (int)curlevelminweight_lbs, (int)curlevelminweight_lbs == 1 ? "lb" : "lbs");
            else
                Sprintf(wtbuf, "%s between %s and %s.", verb, curlevelminbuf, curlevelmaxbuf); // , ((int)curlevelminweight_lbs == 1 && (int)curlevelmaxweight_lbs == 1) ? "lb" : "lbs");


            Sprintf(carrybuf, "You are carrying %s and ", carryingweightbuf); // , (int)carryingweight == 1 ? "lb" : "lbs");
#if 0
            if (carryingweight >= 10)
                Sprintf(carrybuf, "You are carrying %d %s and ", (int)carryingweight, (int)carryingweight == 1 ? "lb" : "lbs");
            else
                Sprintf(carrybuf, "You are carrying %1.1f %s and ", carryingweight, carryingweight == 1 ? "lb" : "lbs");
#endif

            Sprintf(totalbuf, "%s%s", carrybuf, wtbuf);

            putstr(win, 0, totalbuf);
        }
    }
}
/*
 * If lets == NULL or "", list all objects in the inventory.  Otherwise,
 * list all objects with object classes that match the order in lets.
 *
 * Returns the letter identifier of a selected item, or 0 if nothing
 * was selected.
 */
char
display_inventory(lets, want_reply, show_weights)
const char *lets;
boolean want_reply;
int show_weights;
{
    return display_pickinv(lets, (char *) 0, (char *) 0,
                           want_reply, (long *) 0, show_weights, "", FALSE, FALSE);
}

char
display_inventory_with_header(lets, want_reply, out_cnt, show_weights, wornonly)
const char* lets;
boolean want_reply, wornonly;
long* out_cnt;
int show_weights;
{
    return display_pickinv(lets, (char*)0, (char*)0,
        want_reply, out_cnt, show_weights, "", TRUE, wornonly);
}

/*
 * Show what is current using inventory letters.
 *
 */
STATIC_OVL char
display_used_invlets(avoidlet)
char avoidlet;
{
    struct obj *otmp;
    char ilet, ret = 0;
    char *invlet = flags.inv_order;
    int n, classcount, invdone = 0;
    winid win;
    anything any;
    menu_item *selected;

    if (invent) {
        win = create_nhwindow(NHW_MENU);
        start_menu_ex(win, GHMENU_STYLE_PICK_ITEM_LIST);
        while (!invdone) {
            any = zeroany; /* set all bits to zero */
            classcount = 0;
            for (otmp = invent; otmp; otmp = otmp->nobj) {
                ilet = otmp->invlet;
                if (ilet == avoidlet)
                    continue;
                if (!flags.sortpack || otmp->oclass == *invlet) {
                    if (flags.sortpack && !classcount) {
                        any = zeroany; /* zero */
                        add_extended_menu(win, NO_GLYPH, &any, menu_heading_info(), 0, 0,
                                 iflags.menu_headings,
                                 let_to_name(*invlet, FALSE, FALSE),
                                 MENU_UNSELECTED);
                        classcount++;
                    }
                    any.a_char = ilet;
                    int glyph = obj_to_glyph(otmp, rn2_on_display_rng);
                    int gui_glyph = maybe_get_replaced_glyph(glyph, u.ux, u.uy, data_to_replacement_info(glyph, LAYER_OBJECT, otmp, (struct monst*)0, 0UL));
                    add_extended_menu(win, iflags.using_gui_tiles ? gui_glyph : glyph,
                             &any, obj_to_extended_menu_info(otmp), ilet, 0, ATR_NONE,
                             (flags.inventory_weights_last ? doname_with_weight_last(otmp, TRUE) : doname_with_weight_first(otmp, TRUE)), MENU_UNSELECTED);
                }
            }
            if (flags.sortpack && *++invlet)
                continue;
            invdone = 1;
        }
        end_menu(win, "Adjust letter to what?");

        n = select_menu(win, PICK_ONE, &selected);
        if (n > 0) {
            ret = selected[0].item.a_char;
            free((genericptr_t) selected);
        } else
            ret = !n ? '\0' : '\033'; /* cancelled */
        destroy_nhwindow(win);
    }
    return ret;
}

/*
 * Returns the number of unpaid items within the given list.  This includes
 * contained objects.
 */
int
count_unpaid(list)
struct obj *list;
{
    int count = 0;

    while (list) {
        if (list->unpaid)
            count++;
        if (Has_contents(list))
            count += count_unpaid(list->cobj);
        list = list->nobj;
    }
    return count;
}

/*
 * Returns the number of items with b/u/c/unknown within the given list.
 * This does NOT include contained objects.
 *
 * Assumes that the hero sees or touches or otherwise senses the objects
 * at some point:  bknown is forced for priest[ess], like in xname().
 */
int
count_buc(list, type, filterfunc)
struct obj *list;
int type;
boolean FDECL((*filterfunc), (OBJ_P));
{
    int count = 0;

    for (; list; list = list->nobj) {
        /* priests always know bless/curse state */
        if (Role_if(PM_PRIEST))
            list->bknown = (list->oclass != COIN_CLASS);
        /* some actions exclude some or most items */
        if (filterfunc && !(*filterfunc)(list))
            continue;

        /* coins are either uncursed or unknown based upon option setting */
        if (list->oclass == COIN_CLASS) {
            if (type == (iflags.goldX ? BUC_UNKNOWN : BUC_UNCURSED))
                ++count;
            continue;
        }
        /* check whether this object matches the requested type */
        if (!list->bknown
                ? (type == BUC_UNKNOWN)
                : list->blessed ? (type == BUC_BLESSED)
                                : list->cursed ? (type == BUC_CURSED)
                                               : (type == BUC_UNCURSED))
            ++count;
    }
    return count;
}

/* similar to count_buc(), but tallies all states at once
   rather than looking for a specific type */
void
tally_BUCX(list, by_nexthere, bcp, ucp, ccp, xcp, ocp)
struct obj *list;
boolean by_nexthere;
int *bcp, *ucp, *ccp, *xcp, *ocp;
{
    /* Future extensions:
     *  Skip current_container when list is invent, uchain when
     *  first object of list is located on the floor.  'ocp' will then
     *  have a function again (it was a counter for having skipped gold,
     *  but that's not skipped anymore).
     */
    *bcp = *ucp = *ccp = *xcp = *ocp = 0;
    for ( ; list; list = (by_nexthere ? list->nexthere : list->nobj)) {
        /* priests always know bless/curse state */
        if (Role_if(PM_PRIEST))
            list->bknown = (list->oclass != COIN_CLASS);
        /* coins are either uncursed or unknown based upon option setting */
        if (list->oclass == COIN_CLASS) {
            if (iflags.goldX)
                ++(*xcp);
            else
                ++(*ucp);
            continue;
        }
        /* ordinary items */
        if (!list->bknown)
            ++(*xcp);
        else if (list->blessed)
            ++(*bcp);
        else if (list->cursed)
            ++(*ccp);
        else /* neither blessed nor cursed => uncursed */
            ++(*ucp);
    }
}

/* count everything inside a container, or just shop-owned items inside */
long
count_contents(container, nested, quantity, everything)
struct obj *container;
boolean nested, /* include contents of any nested containers */
    quantity,   /* count all vs count separate stacks */
    everything; /* all objects vs only unpaid objects */
{
    struct obj *otmp, *topc;
    boolean shoppy = FALSE;
    long count = 0L;

    if (!everything) {
        for (topc = container; topc->where == OBJ_CONTAINED;
             topc = topc->ocontainer)
            continue;
        if (topc->where == OBJ_FLOOR) {
            xchar x, y;

            (void) get_obj_location(topc, &x, &y, CONTAINED_TOO);
            shoppy = costly_spot(x, y);
        }
    }
    for (otmp = container->cobj; otmp; otmp = otmp->nobj) {
        if (nested && Has_contents(otmp))
            count += count_contents(otmp, nested, quantity, everything);
        if (everything || otmp->unpaid || (shoppy && !otmp->no_charge))
            count += quantity ? otmp->quan : 1L;
    }
    return count;
}

STATIC_OVL void
dounpaid()
{
    winid win;
    struct obj *otmp, *marker, *contnr;
    register char ilet;
    char *invlet = flags.inv_order;
    int classcount, count, num_so_far;
    long cost, totcost;

    count = count_unpaid(invent);
    otmp = marker = contnr = (struct obj *) 0;

    if (count == 1) {
        otmp = find_unpaid(invent, &marker);
        contnr = unknwn_contnr_contents(otmp);
    }
    if  (otmp && !contnr) {
        /* 1 item; use pline instead of popup menu */
        cost = unpaid_cost(otmp, FALSE);
        iflags.suppress_price++; /* suppress "(unpaid)" suffix */
        pline1(xprname(otmp, distant_name(otmp, doname),
                       carried(otmp) ? otmp->invlet : CONTAINED_SYM,
                       TRUE, cost, 0L));
        iflags.suppress_price--;
        return;
    }

    win = create_nhwindow(NHW_MENU);
    cost = totcost = 0;
    num_so_far = 0; /* count of # printed so far */
    if (!flags.invlet_constant)
        reassign();

    do {
        classcount = 0;
        for (otmp = invent; otmp; otmp = otmp->nobj) {
            ilet = otmp->invlet;
            if (otmp->unpaid) {
                if (!flags.sortpack || otmp->oclass == *invlet) {
                    if (flags.sortpack && !classcount) {
                        putstr(win, 0, let_to_name(*invlet, TRUE, FALSE));
                        classcount++;
                    }

                    totcost += cost = unpaid_cost(otmp, FALSE);
                    iflags.suppress_price++; /* suppress "(unpaid)" suffix */
                    putstr(win, 0, xprname(otmp, distant_name(otmp, doname),
                                           ilet, TRUE, cost, 0L));
                    iflags.suppress_price--;
                    num_so_far++;
                }
            }
        }
    } while (flags.sortpack && (*++invlet));

    if (count > num_so_far) {
        /* something unpaid is contained */
        if (flags.sortpack)
            putstr(win, 0, let_to_name(CONTAINED_SYM, TRUE, FALSE));
        /*
         * Search through the container objects in the inventory for
         * unpaid items.  The top level inventory items have already
         * been listed.
         */
        for (otmp = invent; otmp; otmp = otmp->nobj) {
            if (Has_contents(otmp)) {
                long contcost = 0L;

                marker = (struct obj *) 0; /* haven't found any */
                while (find_unpaid(otmp->cobj, &marker)) {
                    totcost += cost = unpaid_cost(marker, FALSE);
                    contcost += cost;
                    if (otmp->cknown) {
                        iflags.suppress_price++; /* suppress "(unpaid)" sfx */
                        putstr(win, 0,
                               xprname(marker, distant_name(marker, doname),
                                       CONTAINED_SYM, TRUE, cost, 0L));
                        iflags.suppress_price--;
                    }
                }
                if (!otmp->cknown) {
                    char contbuf[BUFSZ];

                    /* Shopkeeper knows what to charge for contents */
                    Sprintf(contbuf, "%s contents", s_suffix(xname(otmp)));
                    putstr(win, 0,
                           xprname((struct obj *) 0, contbuf, CONTAINED_SYM,
                                   TRUE, contcost, 0L));
                }
            }
        }
    }

    putstr(win, 0, "");
    putstr(win, 0,
           xprname((struct obj *) 0, "Total:", '*', FALSE, totcost, 0L));
    display_nhwindow(win, FALSE);
    destroy_nhwindow(win);
}

/* query objlist callback: return TRUE if obj type matches "this_type" */
static int this_type;

STATIC_OVL boolean
this_type_only(obj)
struct obj *obj;
{
    boolean res = (obj->oclass == this_type);

    if (obj->oclass == COIN_CLASS) {
        /* if filtering by bless/curse state, gold is classified as
           either unknown or uncursed based on user option setting */
        if (this_type && index("BUCX", this_type))
            res = (this_type == (iflags.goldX ? 'X' : 'U'));
    } else {
        switch (this_type) {
        case 'B':
            res = (obj->bknown && obj->blessed);
            break;
        case 'U':
            res = (obj->bknown && !(obj->blessed || obj->cursed));
            break;
        case 'C':
            res = (obj->bknown && obj->cursed);
            break;
        case 'X':
            res = !obj->bknown;
            break;
        default:
            break; /* use 'res' as-is */
        }
    }
    return res;
}

/* the 'I' command */
int
dotypeinv()
{
    char c = '\0';
    int n, i = 0;
    char *extra_types, types[BUFSZ];
    int class_count, oclass, unpaid_count, itemcount;
    int bcnt, ccnt, ucnt, xcnt, ocnt;
    boolean billx = *u.ushops && doinvbill(0);
    menu_item *pick_list;
    boolean traditional = TRUE;
    const char *prompt = "What type of object do you want an inventory of?";

    if (!invent && !billx) {
        You1("aren't carrying anything.");
        return 0;
    }
    unpaid_count = count_unpaid(invent);
    tally_BUCX(invent, FALSE, &bcnt, &ucnt, &ccnt, &xcnt, &ocnt);

    if (flags.menu_style != MENU_TRADITIONAL) {
        if (flags.menu_style == MENU_FULL
            || flags.menu_style == MENU_PARTIAL) {
            traditional = FALSE;
            i = UNPAID_TYPES;
            if (billx)
                i |= BILLED_TYPES;
            if (bcnt)
                i |= BUC_BLESSED;
            if (ucnt)
                i |= BUC_UNCURSED;
            if (ccnt)
                i |= BUC_CURSED;
            if (xcnt)
                i |= BUC_UNKNOWN;
            n = query_category(prompt, invent, i, &pick_list, PICK_ONE);
            if (!n)
                return 0;
            this_type = c = pick_list[0].item.a_int;
            free((genericptr_t) pick_list);
        }
    }
    if (traditional) {
        /* collect a list of classes of objects carried, for use as a prompt
         */
        types[0] = 0;
        class_count = collect_obj_classes(types, invent, FALSE,
                                          (boolean FDECL((*), (OBJ_P))) 0,
                                          &itemcount);
        if (unpaid_count || billx || (bcnt + ccnt + ucnt + xcnt) != 0)
            types[class_count++] = ' ';
        if (unpaid_count)
            types[class_count++] = 'u';
        if (billx)
            types[class_count++] = 'x';
        if (bcnt)
            types[class_count++] = 'B';
        if (ucnt)
            types[class_count++] = 'U';
        if (ccnt)
            types[class_count++] = 'C';
        if (xcnt)
            types[class_count++] = 'X';
        types[class_count] = '\0';
        /* add everything not already included; user won't see these */
        extra_types = eos(types);
        *extra_types++ = '\033';
        if (!unpaid_count)
            *extra_types++ = 'u';
        if (!billx)
            *extra_types++ = 'x';
        if (!bcnt)
            *extra_types++ = 'B';
        if (!ucnt)
            *extra_types++ = 'U';
        if (!ccnt)
            *extra_types++ = 'C';
        if (!xcnt)
            *extra_types++ = 'X';
        *extra_types = '\0'; /* for index() */
        for (i = 0; i < MAX_OBJECT_CLASSES; i++)
            if (!index(types, def_oc_syms[i].sym)) {
                *extra_types++ = def_oc_syms[i].sym;
                *extra_types = '\0';
            }

        if (class_count > 1) {
            c = yn_function(prompt, types, '\0', (char*)0);
            savech(c);
            if (c == '\0') {
                clear_nhwindow(WIN_MESSAGE);
                return 0;
            }
        } else {
            /* only one thing to itemize */
            if (unpaid_count)
                c = 'u';
            else if (billx)
                c = 'x';
            else
                c = types[0];
        }
    }
    if (c == 'x' || (c == 'X' && billx && !xcnt)) {
        if (billx)
            (void) doinvbill(1);
        else
            pline("No used-up objects%s.",
                  unpaid_count ? " on your shopping bill" : "");
        return 0;
    }
    if (c == 'u' || (c == 'U' && unpaid_count && !ucnt)) {
        if (unpaid_count)
            dounpaid();
        else
            You1("are not carrying any unpaid objects.");
        return 0;
    }
    if (traditional) {
        if (index("BUCX", c))
            oclass = c; /* not a class but understood by this_type_only() */
        else
            oclass = def_char_to_objclass(c); /* change to object class */

        if (oclass == COIN_CLASS)
            return doprgold();
        if (index(types, c) > index(types, '\033')) {
            /* '> ESC' => hidden choice, something known not to be carried */
            const char *before = "", *after = "";

            switch (c) {
            case 'B':
                before = "known to be blessed ";
                break;
            case 'U':
                before = "known to be uncursed ";
                break;
            case 'C':
                before = "known to be cursed ";
                break;
            case 'X':
                after = " whose blessed/uncursed/cursed status is unknown";
                break; /* better phrasing is desirable */
            default:
                /* 'c' is an object class, because we've already handled
                   all the non-class letters which were put into 'types[]';
                   could/should move object class names[] array from below
                   to somewhere above so that we can access it here (via
                   lcase(strcpy(classnamebuf, names[(int) c]))), but the
                   game-play value of doing so is low... */
                before = "such ";
                break;
            }
            You("have no %sobjects%s.", before, after);
            return 0;
        }
        this_type = oclass;
    }
    if (query_objlist((char *) 0, &invent,
                      ((flags.invlet_constant ? USE_INVLET : 0)
                       | INVORDER_SORT),
                      &pick_list, PICK_NONE, this_type_only, 1) > 0)
        free((genericptr_t) pick_list);
    return 0;
}

/* return a string describing the dungeon feature at <x,y> if there
   is one worth mentioning at that location; otherwise null */
const char *
dfeature_at(x, y)
int x, y;
{
    struct rm *lev = &levl[x][y];
    int ltyp = lev->typ, lsubtyp = lev->subtyp, cmap = -1, altcmap = -1;
    const char *dfeature = 0;
    static char altbuf[BUFSZ];

    if (IS_DOOR(ltyp)) {
        switch ((lev->doormask & D_MASK)) {
        case D_NODOOR:
            altcmap = S_ndoor;
            break; /* "doorway" */
        case D_ISOPEN:
            altcmap = S_vodoor;
            break; /* "open door" */
        case D_BROKEN:
            altcmap = S_vbdoor;
            break; /* "broken door" */
        case D_PORTCULLIS:
            altcmap = S_voportcullis;
            break; /* "open portcullis" */
        default:
            altcmap = S_vcdoor;
            break; /* "closed door" */
        }

        if (altcmap > -1)
        {
            if(lsubtyp > 0)
                strcpy(altbuf, defsym_variations[lsubtyp - 1 + defsyms[altcmap].variation_offset].explanation);
            else
                strcpy(altbuf, defsyms[altcmap].explanation);

            print_lock_with_buf(eos(altbuf), levl[x][y].key_otyp, levl[x][y].special_quality, is_door_normally_without_lock_at(x, y));
            dfeature = altbuf;
        }

        /* override door description for open drawbridge */
        if (is_drawbridge_wall(x, y) >= 0)
            dfeature = "open drawbridge portcullis", cmap = -1;
    }
    else if (IS_FOUNTAIN(ltyp))
        dfeature = get_fountain_name(x, y); //cmap = S_fountain; /* "fountain" */
    else if (IS_THRONE(ltyp))
        cmap = S_throne; /* "opulent throne" */
    else if (is_lava(x, y))
        cmap = S_lava; /* "molten lava" */
    else if (is_ice(x, y))
        cmap = S_ice; /* "ice" */
    else if (is_pool(x, y))
        dfeature = "pool of water";
    else if (IS_SINK(ltyp))
        cmap = S_sink; /* "sink" */
    else if (IS_ALTAR(ltyp)) {
        if (lsubtyp == ALTAR_SUBTYPE_HIGH)
        {
            if(a_align(x, y) == A_NONE)
                Sprintf(altbuf, "high altar (unaligned)");
            else
                Sprintf(altbuf, "%saltar to %s (%s)", "high ", a_gname_at(x, y),
                    align_str(Amask2align(lev->altarmask & ~AM_SHRINE)));
        }
        else if (lsubtyp == ALTAR_SUBTYPE_MOLOCH)
        {
            Sprintf(altbuf, "%saltar to %s",
                ((lev->altarmask& AM_SHRINE)
                    && (Is_astralevel(&u.uz) || Is_sanctum(&u.uz)))
                ? "high " : "",
                Moloch);
        }
        else
        {
            if (a_align(x, y) == A_NONE)
                Sprintf(altbuf, "%saltar (unaligned)",
                    ((lev->altarmask & AM_SHRINE)
                        && (Is_astralevel(&u.uz) || Is_sanctum(&u.uz)))
                    ? "high "
                    : ""
                    );
            else
                Sprintf(altbuf, "%saltar to %s (%s)",
                ((lev->altarmask& AM_SHRINE)
                    && (Is_astralevel(&u.uz) || Is_sanctum(&u.uz)))
                ? "high "
                : "",
                a_gname_at(x, y),
                align_str(Amask2align(lev->altarmask & ~AM_SHRINE)));
        }

        dfeature = altbuf;
    }
    else if ((x == xupstair && y == yupstair))
        cmap = S_upstair; /* "staircase up" */
    else if ((x == xdnstair && y == ydnstair))
    {
        cmap = S_dnstair; /* "staircase down" */
    }
    else if ((x == sstairs.sx && y == sstairs.sy && sstairs.up))
    {
        cmap = S_upstair; /* "staircase up" */
    }
    else if ((x == sstairs.sx && y == sstairs.sy && !sstairs.up))
    {
        cmap = S_dnstair; /* "staircase down" */
    }
    else if (x == xupladder && y == yupladder)
        cmap = S_upladder; /* "ladder up" */
    else if (x == xdnladder && y == ydnladder)
        cmap = S_dnladder; /* "ladder down" */
    else if (ltyp == DRAWBRIDGE_DOWN)
        cmap = S_vodbridge; /* "lowered drawbridge" */
    else if (ltyp == DBWALL)
        cmap = S_vcdbridge; /* "raised drawbridge" */
    else if (IS_GRAVE(ltyp))
        cmap = S_grave; /* "grave" */
    else if (IS_BRAZIER(ltyp))
    {
        cmap = S_brazier; /* "brazier" */
        /*
        Sprintf(altbuf, "%s%s", lev->lamplit ? "lit " : "unlit ",
            defsyms[S_brazier].explanation);

        dfeature = altbuf;
        */
    }
    else if (IS_SIGNPOST(ltyp))
        cmap = S_signpost; /* "signpost" */
    else if (IS_TREE(ltyp))
        cmap = S_tree; /* "tree" */
    else if (IS_ANVIL(ltyp))
        cmap = S_anvil; /* "anvil" */
    else if (ltyp == IRONBARS)
        dfeature = "set of iron bars";

    if (cmap >= 0)
        dfeature = defsyms[cmap].explanation;
    else if (levl[x][y].floor_doodad && iflags.using_gui_tiles)
    {
        dfeature = get_floor_doodad_explanation_at(x, y);
    }
    return dfeature;
}

const char*
get_floor_doodad_explanation_at(x, y)
int x, y;
{
    const char* explanation = 0;
    if (levl[x][y].floor_doodad)
    {
        if (glyph_is_broken_cmap(levl[x][y].floor_doodad))
        {
            int bcmap = glyph_to_broken_cmap(levl[x][y].floor_doodad);
            if (defsyms[bcmap].broken_explanation && strcmp(defsyms[bcmap].broken_explanation, ""))
                explanation = defsyms[bcmap].broken_explanation;
        }
        else if (glyph_is_broken_cmap_variation(levl[x][y].floor_doodad))
        {
            int bcmapvar = glyph_to_broken_cmap_variation(levl[x][y].floor_doodad);
            if (defsym_variations[bcmapvar].broken_explanation && strcmp(defsym_variations[bcmapvar].broken_explanation, ""))
            {
                explanation = defsym_variations[bcmapvar].broken_explanation;
            }
            else
            {
                int bcmap = defsym_variations[bcmapvar].base_screen_symbol;
                if (defsyms[bcmap].broken_explanation && strcmp(defsyms[bcmap].broken_explanation, ""))
                    explanation = defsyms[bcmap].broken_explanation;
            }
        }
    }

    return explanation;
}

/* look at what is here; if there are many objects (pile_limit or more),
   don't show them unless obj_cnt is 0 */
int
look_here(obj_cnt, picked_some, explicit_cmd)
int obj_cnt; /* obj_cnt > 0 implies that autopickup is in progress */
boolean picked_some, explicit_cmd;
{
    struct obj *otmp;
    struct trap *trap;
    const char *verb = Blind ? "feel" : "see";
    const char *dfeature = (char *) 0;
    char fbuf[BUFSZ];
    winid tmpwin;
    boolean skip_objects, felt_cockatrice = FALSE;
    struct rm* lev = &levl[u.ux][u.uy];

    /* default pile_limit is 5; a value of 0 means "never skip"
       (and 1 effectively forces "always skip") */
    skip_objects = (flags.pile_limit > 0 && obj_cnt >= flags.pile_limit);
    if (u.uswallow && u.ustuck) {
        struct monst *mtmp = u.ustuck;

        Sprintf(fbuf, "Contents of %s %s", s_suffix(mon_nam(mtmp)),
                mbodypart(mtmp, STOMACH));
        /* Skip "Contents of " by using fbuf index 12 */
        You("%s to %s what is lying in %s.", Blind ? "try" : "look around",
            verb, &fbuf[12]);
        otmp = mtmp->minvent;
        if (otmp) {
            for (; otmp; otmp = otmp->nobj) {
                /* If swallower is an animal, it should have become stone
                 * but... */
                if (otmp->otyp == CORPSE)
                    feel_cockatrice(otmp, FALSE);
            }
            if (Blind)
                Strcpy(fbuf, "You feel");
            Strcat(fbuf, ":");
            (void) display_minventory(mtmp, MINV_ALL | PICK_NONE, fbuf);
        } else {
            You("%s no objects here.", verb);
        }
        return !!Blind;
    }
    if (!skip_objects && (trap = t_at(u.ux, u.uy)) && trap->tseen)
    {
        char buf[BUFSZ];
        strcpy(buf, "");
        if (trap && trap->ttyp == LEVER && (trap->tflags & TRAPFLAGS_SWITCHABLE_BETWEEN_STATES))
        {
            if ((trap->tflags & TRAPFLAGS_STATE_MASK) > 0UL)
                Sprintf(buf, " turned left");
            else
                Sprintf(buf, " turned right");
        }
        There("is %s%s here.",
            an(get_trap_explanation(trap)), buf);
    }

    otmp = level.objects[u.ux][u.uy];
    dfeature = dfeature_at(u.ux, u.uy);
    if (dfeature && !strcmp(dfeature, "pool of water") && Underwater)
        dfeature = 0;

    if (Blind) {
        boolean drift = Is_airlevel(&u.uz) || Is_waterlevel(&u.uz);

        if (dfeature && !strncmp(dfeature, "altar ", 6)) {
            /* don't say "altar" twice, dfeature has more info */
            You1("try to feel what is here.");
        } else {
            const char *where = (Blind && !can_reach_floor(TRUE))
                                    ? "lying beneath you"
                                    : "lying here on the ",
                       *onwhat = (Blind && !can_reach_floor(TRUE))
                                     ? ""
                                     : surface(u.ux, u.uy);

            You("try to feel what is %s%s.", drift ? "floating here" : where,
                drift ? "" : onwhat);
        }
        if (dfeature && !drift && !strcmp(dfeature, surface(u.ux, u.uy)))
            dfeature = 0; /* ice already identified */
        if (!can_reach_floor(TRUE)) {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            pline_ex1(ATR_NONE, CLR_MSG_FAIL, "But you can't reach it!");
            return 0;
        }
    }

    char dfbuf[BUFSZ] = "";
    if (IS_BRAZIER(lev->typ))
    {
        if(lev->lamplit)
            strcpy(dfbuf, "lit ");
        else
            strcpy(dfbuf, "unlit ");
    }

    if (dfeature)
    {
        Strcat(dfbuf, dfeature);
        Sprintf(fbuf, "There is %s here.", an(dfbuf));
    }

    if (!otmp || is_lava(u.ux, u.uy)
        || (is_pool(u.ux, u.uy) && !Underwater)) {
        if (dfeature)
            pline1(fbuf);
        read_engr_at(u.ux, u.uy); /* Eric Backus */
        if (!skip_objects && (Blind || !dfeature))
            You("%s no objects here.", verb);
        return !!Blind;
    }

    /* we know there is something here */
    int total_count = 0;
    struct obj* otmp_cnt;
    for (otmp_cnt = otmp; otmp_cnt; otmp_cnt = otmp_cnt->nexthere)
        total_count++;

    if (skip_objects)
    {
        if (dfeature)
            pline1(fbuf);
        read_engr_at(u.ux, u.uy); /* Eric Backus */
        if (obj_cnt == 1 && otmp->quan == 1L)
            There("is %s object here.", picked_some ? "another" : "an");
        else
            There("are %s%s objects here.",
#ifdef ANDROID
            (obj_cnt == 2) ? "two" : (
#endif
                (obj_cnt < 5)
                      ? "a few"
                      : (obj_cnt < 10)
                          ? "several"
                          : "many"
#ifdef ANDROID
                )
#endif
                ,
                  picked_some ? " more" : "");
        for (; otmp; otmp = otmp->nexthere)
            if (otmp->otyp == CORPSE && will_feel_cockatrice(otmp, FALSE)) {
                pline("%s %s%s.",
                      (obj_cnt > 1)
                          ? "Including"
                          : (otmp->quan > 1L)
                              ? "They're"
                              : "It's",
                      corpse_xname(otmp, (const char *) 0, CXN_ARTICLE),
                      poly_when_stoned(youmonst.data)
                          ? ""
                          : ", unfortunately");
                feel_cockatrice(otmp, FALSE);
                break;
            }

    } 
    else if (!otmp->nexthere) 
    {
        /* only one object */
        if (dfeature)
            pline1(fbuf);
        read_engr_at(u.ux, u.uy); /* Eric Backus */
        You("%s here %s.", verb, doname_in_text_with_price_and_weight_last(otmp)); //See on the ground
        iflags.last_msg = PLNMSG_ONE_ITEM_HERE;
        if (otmp->otyp == CORPSE)
            feel_cockatrice(otmp, FALSE);
    }
    else
    {
        if (explicit_cmd) // No here window
        {
            char buf[BUFSZ];
            char buf2[BUFSZ];
            int count = 0;
            int totalweight = 0;

            display_nhwindow(WIN_MESSAGE, FALSE);

            tmpwin = create_nhwindow(NHW_MENU);

            if (dfeature) {
                putstr(tmpwin, 0, fbuf);
                putstr(tmpwin, ATR_HALF_SIZE, " ");
            }
            Sprintf(buf, "%s that %s here:",
                picked_some ? "Other things" : "Things",
                Blind ? "you feel" : "are");
            putstr(tmpwin, ATR_TITLE, buf);
            putstr(tmpwin, ATR_HALF_SIZE, " ");
            totalweight = 0;
            for (; otmp; otmp = otmp->nexthere) 
            {
                count++;
                if (otmp->otyp == CORPSE && will_feel_cockatrice(otmp, FALSE)) {
                    felt_cockatrice = TRUE;
                    Sprintf(buf, "%2d - %s...", count, doname(otmp));
                    putstr(tmpwin, ATR_INDENT_AT_DASH, buf);
                    break;
                }
                if (otmp->otyp == LOADSTONE && !objects[LOADSTONE].oc_name_known)
                    totalweight += objects[LUCKSTONE].oc_weight;
                else
                    totalweight += otmp->owt;
                Sprintf(buf2, "%2d - %s", count, (flags.inventory_weights_last ? doname_with_price_and_weight_last(otmp, objects[LOADSTONE].oc_name_known) : doname_with_price_and_weight_first(otmp, objects[LOADSTONE].oc_name_known))); //Looking at what is on the ground
                putstr(tmpwin, ATR_INDENT_AT_DASH, buf2);
            }

            if (flags.show_weight_summary)
            {
                if (flags.inventory_weights_last)
                    putstr(tmpwin, ATR_HALF_SIZE, " ");
                add_weight_summary_putstr(tmpwin, totalweight, 1);
            }

            display_nhwindow(tmpwin, TRUE);
            destroy_nhwindow(tmpwin);

            if (felt_cockatrice)
                feel_cockatrice(otmp, FALSE);

            read_engr_at(u.ux, u.uy); /* Eric Backus */
        }
        else
        {
            if (dfeature)
                pline1(fbuf);
            read_engr_at(u.ux, u.uy); /* Eric Backus */
            You("%s %s here.", verb, "many objects"); //See on the ground
        }
    }
    update_here_window();
    return !!Blind;
}

void
print_things_here_to_window(VOID_ARGS)
{
    winid tmpwin = WIN_HERE;
    if (tmpwin == WIN_ERR)
        return;

    int attr = 0;
    int textcolor = CLR_MSG_ATTENTION;

    struct obj* otmp;
    const char* dfeature = (char*)0;
    char fbuf[BUFSZ] = "";
    char dfbuf[BUFSZ] = "";
    struct rm* lev = &levl[u.ux][u.uy];

    otmp = level.objects[u.ux][u.uy];
    dfeature = adjusted_dfeature_at(u.ux, u.uy);

    if (IS_BRAZIER(lev->typ))
    {
        if (lev->lamplit)
            strcpy(dfbuf, "lit ");
        else
            strcpy(dfbuf, "unlit ");
    }

    char buf[BUFSZ];
    char buf2[BUFSZ];
    int count = 0;
    int displ_style = here_window_display_style(dfeature, otmp);
    if (displ_style == 0)
        return;
    else if (displ_style == 2)
    {
        putstr_ex(tmpwin, attr, Blind ? "[You feel there are many objects here.]" : "[There are many objects here.]", 0, textcolor);
    }
    else
    {
        Sprintf(buf, "%s that %s here:", "Things", Blind ? "you feel" : "are");
        putstr_ex(tmpwin, attr, buf, 0, textcolor);

        if (dfeature)
        {
            Strcat(dfbuf, dfeature);
            struct layer_info layers = nul_layerinfo;
            char sym = 0;
            nhsym ch = 0;
            int color;
            unsigned long special;
            int glyph = back_to_glyph(u.ux, u.uy);
            layers.glyph = glyph;
            (void)mapglyph(layers, &ch, &color, &special, u.ux, u.uy);

            if (SYMHANDLING(H_UNICODE))
            {
                sym = unicode_to_CP437(ch);
            }
            else
            {
                sym = (char)ch;
            }

            
            putstr_ex(tmpwin, attr, "'", 1, textcolor);
            Sprintf(fbuf, "%c", sym);
            putstr_ex(tmpwin, 0, fbuf, 1, color);
            putstr_ex(tmpwin, attr, "' ", 1, textcolor);
            Sprintf(fbuf, "%s", an(dfbuf));
            putstr_ex(tmpwin, attr, fbuf, 0, textcolor);
            
            //Sprintf(fbuf, "'%c' %s", sym, an(dfbuf));
            //putstr_ex(tmpwin, attr, fbuf, 0, textcolor);
        }

        for (; otmp; otmp = otmp->nexthere) 
        {
            count++;

            struct layer_info layers = nul_layerinfo;
            char sym = 0;
            nhsym ch = 0;
            int color;
            unsigned long special;
            int glyph = otmp->glyph;
            layers.glyph = glyph;
            (void)mapglyph(layers, &ch, &color, &special, u.ux, u.uy);

            if (SYMHANDLING(H_UNICODE))
            {
                sym = unicode_to_CP437(ch);
            }
            else
            {
                sym = (char)ch;
            }
            
            putstr_ex(tmpwin, attr, "'", 1, textcolor);
            Sprintf(buf2, "%c", sym);
            putstr_ex(tmpwin, 0, buf2, 1, color);
            putstr_ex(tmpwin, attr, "' ", 1, textcolor);
            Sprintf(buf2, "%s", (flags.inventory_weights_last ? doname_with_price_and_weight_last(otmp, objects[LOADSTONE].oc_name_known) : doname_with_price_and_weight_first(otmp, objects[LOADSTONE].oc_name_known)));
            putstr_ex(tmpwin, attr, buf2, 0, textcolor);
            
            //Sprintf(buf2, "'%c' %s", sym, (flags.inventory_weights_last ? doname_with_price_and_weight_last(otmp, objects[LOADSTONE].oc_name_known) : doname_with_price_and_weight_first(otmp, objects[LOADSTONE].oc_name_known)));
            //putstr_ex(tmpwin, attr, buf2, 0, textcolor);
        }
    }
}

void
update_here_window(VOID_ARGS)
{
    if (WIN_HERE == WIN_ERR)
        return;

    clear_nhwindow(WIN_HERE);
    if (!Blind)
        print_things_here_to_window();

}

/* the ':' command - explicitly look at what is here, including all objects */
int
dolook()
{
    int res;

    /* don't let
       MSGTYPE={norep,noshow} "You see here"
       interfere with feedback from the look-here command */
    hide_unhide_msgtypes(TRUE, MSGTYP_MASK_REP_SHOW);
    res = look_here(0, FALSE, TRUE);
    /* restore normal msgtype handling */
    hide_unhide_msgtypes(FALSE, MSGTYP_MASK_REP_SHOW);
    return res;
}

boolean
will_feel_cockatrice(otmp, force_touch)
struct obj *otmp;
boolean force_touch;
{
    if ((Blind || force_touch) && !uarmg && !Stone_resistance
        && (otmp->otyp == CORPSE && touch_petrifies(&mons[otmp->corpsenm])))
        return TRUE;
    return FALSE;
}

void
feel_cockatrice(otmp, force_touch)
struct obj *otmp;
boolean force_touch;
{
    char kbuf[BUFSZ];

    if (will_feel_cockatrice(otmp, force_touch)) {
        /* "the <cockatrice> corpse" */
        Strcpy(kbuf, corpse_xname(otmp, (const char *) 0, CXN_PFX_THE));

        if (poly_when_stoned(youmonst.data))
            You("touched %s with your bare %s.", kbuf,
                makeplural(body_part(HAND)));
        else
            pline("Touching %s is a fatal mistake...", kbuf);
        /* normalize body shape here; hand, not body_part(HAND) */
        Sprintf(kbuf, "touching %s bare-handed", killer_xname(otmp));
        /* will call polymon() for the poly_when_stoned() case */
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
        instapetrify(kbuf);
    }
}

void
stackobj(obj)
struct obj *obj;
{
    struct obj *otmp;

    if (!obj || !isok(obj->ox, obj->oy))
        return;

    for (otmp = level.objects[obj->ox][obj->oy]; otmp; otmp = otmp->nexthere)
        if (otmp != obj && merged(&obj, &otmp))
            break;
    return;
}

/* returns TRUE if obj & otmp can be merged; used in invent.c and mkobj.c */
boolean
mergable(otmp, obj)
register struct obj *otmp, *obj;
{
    size_t objnamelth = 0, otmpnamelth = 0;

    /* fail if already the same object, if different types, if either is
       explicitly marked to prevent merge, or if not mergable in general */
    if (obj == otmp || obj->otyp != otmp->otyp
        || obj->nomerge || otmp->nomerge || !objects[obj->otyp].oc_merge)
        return FALSE;

    /* coins of the same kind will always merge */
    if (obj->oclass == COIN_CLASS)
        return TRUE;

    if (obj->unpaid != otmp->unpaid || obj->enchantment != otmp->enchantment || obj->elemental_enchantment != otmp->elemental_enchantment
        || obj->exceptionality != otmp->exceptionality || obj->mythic_prefix != otmp->mythic_prefix || obj->mythic_suffix != otmp->mythic_suffix
        || obj->charges != otmp->charges || obj->special_quality != otmp->special_quality || obj->speflags != otmp->speflags
        || obj->cursed != otmp->cursed || obj->blessed != otmp->blessed
        || obj->no_charge != otmp->no_charge || obj->obroken != otmp->obroken
        || obj->otrapped != otmp->otrapped || obj->lamplit != otmp->lamplit
        || obj->bypass != otmp->bypass)
        return FALSE;

    if (obj->globby)
        return TRUE;
    /* Checks beyond this point either aren't applicable to globs
     * or don't inhibit their merger.
     */

    if (obj->oclass == FOOD_CLASS
        && (obj->oeaten != otmp->oeaten || obj->orotten != otmp->orotten))
        return FALSE;

    if (obj->dknown != otmp->dknown
        || (obj->bknown != otmp->bknown && !Role_if(PM_PRIEST))
        || obj->oeroded != otmp->oeroded || obj->oeroded2 != otmp->oeroded2
        || obj->greased != otmp->greased)
        return FALSE;

    if ((obj->oclass == WEAPON_CLASS || obj->oclass == ARMOR_CLASS)
        && (obj->oerodeproof != otmp->oerodeproof
            || obj->rknown != otmp->rknown))
        return FALSE;

    if (obj->otyp == CORPSE || obj->otyp == EGG || obj->otyp == TIN) {
        if (obj->corpsenm != otmp->corpsenm)
            return FALSE;
    }

    if (is_obj_rotting_corpse(obj) && (obj->speflags & SPEFLAGS_ROTTING_STATUS_KNOWN) != (otmp->speflags & SPEFLAGS_ROTTING_STATUS_KNOWN)) {
        return FALSE;
    }

    /* hatching eggs don't merge; ditto for revivable corpses */
    if ((obj->otyp == EGG && (obj->timed || otmp->timed))
        || (obj->otyp == CORPSE && otmp->corpsenm >= LOW_PM
            && is_reviver(&mons[otmp->corpsenm])))
        return FALSE;

    /* allow candle merging only if their ages are close */
    /* see begin_burn() for a reference for the magic "25" */
    if (is_candle(obj) && obj->age / 25 != otmp->age / 25)
        return FALSE;

    /* burning potions of oil never merge */
    if (obj->otyp == POT_OIL && obj->lamplit)
        return FALSE;

    /* don't merge surcharged item with base-cost item */
    if (obj->unpaid && !same_price(obj, otmp))
        return FALSE;

    /* if they have true names, make sure they're the same */
    objnamelth = strlen(safe_oname(obj));
    otmpnamelth = strlen(safe_oname(otmp));
    if ((objnamelth != otmpnamelth
         && ((objnamelth && otmpnamelth) || obj->otyp == CORPSE))
        || (objnamelth && otmpnamelth
            && strncmp(ONAME(obj), ONAME(otmp), objnamelth)))
        return FALSE;

    /* if they have user-specified names, make sure they're the same */
    objnamelth = strlen(safe_uoname(obj));
    otmpnamelth = strlen(safe_uoname(otmp));
    if ((objnamelth != otmpnamelth
        && ((objnamelth && otmpnamelth) || obj->otyp == CORPSE))
        || (objnamelth && otmpnamelth
            && strncmp(UONAME(obj), UONAME(otmp), objnamelth)))
        return FALSE;

    /* for the moment, any additional information is incompatible */
    if (has_omonst(obj) || has_omid(obj) || has_olong(obj) || has_omonst(otmp)
        || has_omid(otmp) || has_olong(otmp))
        return FALSE;

    if (obj->oartifact != otmp->oartifact)
        return FALSE;

    if (obj->known == otmp->known || !objects[otmp->otyp].oc_uses_known) {
        return (boolean) objects[obj->otyp].oc_merge;
    } else
        return FALSE;
}

/* the '$' command */
int
doprgold()
{
    /* the messages used to refer to "carrying gold", but that didn't
       take containers into account */
    long umoney = money_cnt(invent);

    if (!umoney)
        Your("wallet is empty.");
    else
        Your("wallet contains %ld %s.", umoney, currency(umoney));
    shopper_financial_report();
    return 0;
}

/* the ')' command */
int
doprwep()
{
    if ((!u.twoweap && !uwep) || (u.twoweap && !uwep && !uarms))
    {
        You("are empty %s.", body_part(HANDED));
    }
    else
    {
        if(uwep)
            prinv((char *) 0, uwep, 0L);
        if (u.twoweap && uarms)
            prinv((char *) 0, uarms, 0L);
    }
    return 0;
}

/* caller is responsible for checking !wearing_armor() */
STATIC_OVL void
noarmor(report_uskin)
boolean report_uskin;
{
    if (!uskin || !report_uskin) {
        You1("are not wearing any armor.");
    } else {
        char *p, *uskinname, buf[BUFSZ];

        uskinname = strcpy(buf, simpleonames(uskin));
        /* shorten "set of <color> dragon scales" to "<color> scales"
           and "<color> dragon scale mail" to "<color> scale mail" */
        if (!strncmpi(uskinname, "set of ", 7))
            uskinname += 7;
        if ((p = strstri(uskinname, " dragon ")) != 0)
            while ((p[1] = p[8]) != '\0')
                ++p;

        You("are not wearing armor but have %s embedded in your skin.",
            uskinname);
    }
}

/* the '[' command */
int
doprarm()
{
    char lets[12];
    register int ct = 0;
    /*
     * Note:  players sometimes get here by pressing a function key which
     * transmits ''ESC [ <something>'' rather than by pressing '[';
     * there's nothing we can--or should-do about that here.
     */

    if (!wearing_armor()) {
        noarmor(TRUE);
    } else {
        if (uarmu)
            lets[ct++] = obj_to_let(uarmu);
        if (uarm)
            lets[ct++] = obj_to_let(uarm);
        if (uarmo)
            lets[ct++] = obj_to_let(uarmo);
        if (uarmc)
            lets[ct++] = obj_to_let(uarmc);
        if (uarmh)
            lets[ct++] = obj_to_let(uarmh);
        if (uarms)
            lets[ct++] = obj_to_let(uarms);
        if (uarmb)
            lets[ct++] = obj_to_let(uarmb);
        if (uarmg)
            lets[ct++] = obj_to_let(uarmg);
        if (uarmf)
            lets[ct++] = obj_to_let(uarmf);
        lets[ct] = 0;
        (void) display_inventory(lets, FALSE, 0);
    }
    return 0;
}

/* the '=' command */
int
doprring()
{
    if (!uleft && !uright)
        You1("are not wearing any rings.");
    else {
        char lets[3];
        register int ct = 0;

        if (uleft)
            lets[ct++] = obj_to_let(uleft);
        if (uright)
            lets[ct++] = obj_to_let(uright);
        lets[ct] = 0;
        (void) display_inventory(lets, FALSE, 0);
    }
    return 0;
}

/* the '"' command */
int
dopramulet()
{
    if (!uamul)
        You1("are not wearing an amulet.");
    else
        prinv((char *) 0, uamul, 0L);
    return 0;
}

STATIC_OVL boolean
tool_in_use(obj)
struct obj *obj;
{
    if ((obj->owornmask & (W_BLINDFOLD | W_SADDLE)) != 0L)
        return TRUE;
    if (obj->oclass != TOOL_CLASS)
        return FALSE;
    return (boolean) (obj == uwep || obj->lamplit
                      || (obj->otyp == LEASH && obj->leashmon));
}

/* the '(' command */
int
doprtool()
{
    struct obj *otmp;
    int ct = 0;
    char lets[52 + 1];

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (tool_in_use(otmp))
            lets[ct++] = obj_to_let(otmp);
    lets[ct] = '\0';
    if (!ct)
        You1("are not using any tools.");
    else
        (void) display_inventory(lets, FALSE, 0);
    return 0;
}

/* '*' command; combines the ')' + '[' + '=' + '"' + '(' commands;
   show inventory of all currently wielded, worn, or used objects */
int
doprinuse()
{
    struct obj *otmp;
    int ct = 0;
    char lets[52 + 1];

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (is_worn(otmp) || tool_in_use(otmp))
            lets[ct++] = obj_to_let(otmp);
    lets[ct] = '\0';
    if (!ct)
        You1("are not wearing or wielding anything.");
    else
    {
        //(void)display_inventory(lets, FALSE, 0);
        char invlet;
        long pickcnt = 0;

        invlet = display_inventory_with_header(lets, TRUE, &pickcnt, 1, FALSE);
        if (!invlet || invlet == '\033' || invlet == '\0')
            return 0;

        if (flags.inventory_obj_cmd)
        {
            return display_item_command_menu_by_invlet(invlet, pickcnt);
        }
        else
        {
            struct obj* invobj;
            for (invobj = invent; invobj; invobj = invobj->nobj)
                if (invobj->invlet == invlet)
                {
                    (void)itemdescription(invobj);
                    break;
                }
            return 0;
        }
    }
    return 0;
}

/*
 * uses up an object that's on the floor, charging for it as necessary
 */
void
useupf(obj, numused)
register struct obj* obj;
long numused;
{
    useupf_with_flags(obj, numused, 0UL);
}

void
useupf_with_flags(obj, numused, newsym_flags)
register struct obj *obj;
long numused;
unsigned long newsym_flags;
{
    register struct obj *otmp;
    boolean at_u = (obj->ox == u.ux && obj->oy == u.uy);

    /* burn_floor_objects() keeps an object pointer that it tries to
     * useupf() multiple times, so obj must survive if plural */
    if (obj->quan > numused)
        otmp = splitobj(obj, numused);
    else
        otmp = obj;
    if (costly_spot(otmp->ox, otmp->oy)) 
    {
        if (index(u.urooms, *in_rooms(otmp->ox, otmp->oy, 0)))
        {
            char* o_shop = in_rooms(otmp->ox, otmp->oy, SHOPBASE);
            struct monst* shkp = shop_keeper(*o_shop);
            if (shkp && inhishop(shkp) && !Deaf && !muteshk(shkp) && !(otmp->speflags & SPEFLAGS_ADDED_TO_YOUR_BILL))
            {
                play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_ILL_ADD_THAT_TO_YOUR_BILL);
                otmp->speflags |= SPEFLAGS_ADDED_TO_YOUR_BILL;
            }
            addtobill(otmp, FALSE, FALSE, FALSE);
        }
        else
            (void) stolen_value(otmp, otmp->ox, otmp->oy, FALSE, FALSE);
    }
    delobj_with_flags(otmp, newsym_flags);
    if (at_u && u.uundetected && hides_under(youmonst.data))
        (void) hideunder(&youmonst);
}

/*
 * Conversion from a class to a string for printing.
 * This must match the object class order.
 */
STATIC_VAR NEARDATA const char *names[] = {
    0, "Illegal objects", "Weapons", "Armor", "Rings", "Amulets", "Tools",
    "Comestibles", "Potions", "Scrolls", "Books", "Wands", "Coins",
    "Gems/Stones", "Boulders/Statues", "Iron balls", "Chains", "Venoms", "Reagents",  "Miscellaneous"
};
STATIC_VAR NEARDATA const char oth_symbols[] = { CONTAINED_SYM, '\0' };
STATIC_VAR NEARDATA const char *oth_names[] = { "Bagged/Boxed items" };

STATIC_VAR NEARDATA char *invbuf = (char *) 0;
STATIC_VAR NEARDATA size_t invbufsiz = 0;

char *
let_to_name(let, unpaid, showsym)
char let;
boolean unpaid, showsym;
{
    const char *ocsymfmt = "  ('%c')";
    const size_t invbuf_sympadding = 8; /* arbitrary */
    const char *class_name;
    const char *pos;
    int oclass = (let >= 1 && let < MAX_OBJECT_CLASSES) ? let : 0;
    size_t len;

    if (oclass)
        class_name = names[oclass];
    else if ((pos = index(oth_symbols, let)) != 0)
        class_name = oth_names[pos - oth_symbols];
    else
        class_name = names[0];

    len = strlen(class_name) + (unpaid ? sizeof "unpaid_" : sizeof "")
          + (oclass ? (strlen(ocsymfmt) + invbuf_sympadding) : 0);
    if (len > invbufsiz) {
        if (invbuf)
            free((genericptr_t) invbuf);
        invbufsiz = len + 10; /* add slop to reduce incremental realloc */
        invbuf = (char *) alloc(invbufsiz);
    }
    if (unpaid)
        Strcat(strcpy(invbuf, "Unpaid "), class_name);
    else
        Strcpy(invbuf, class_name);
    if ((oclass != 0) && showsym) {
        char *bp = eos(invbuf);
        size_t mlen = invbuf_sympadding - strlen(class_name);
        while (--mlen > 0) {
            *bp = ' ';
            bp++;
        }
        *bp = '\0';
        Sprintf(eos(invbuf), ocsymfmt, def_oc_syms[oclass].sym);
    }
    return invbuf;
}

const char*
get_class_name(oclass)
char oclass;
{
    return names[(int)oclass];
}


/* release the static buffer used by let_to_name() */
void
free_invbuf()
{
    if (invbuf)
        free((genericptr_t) invbuf), invbuf = (char *) 0;
    invbufsiz = 0;
}

/* give consecutive letters to every item in inventory (for !fixinv mode);
   gold is always forced to '$' slot at head of list */
void
reassign()
{
    int i;
    struct obj *obj, *prevobj, *goldobj;

    /* first, remove [first instance of] gold from invent, if present */
    prevobj = goldobj = 0;
    for (obj = invent; obj; prevobj = obj, obj = obj->nobj)
        if (obj->oclass == COIN_CLASS) {
            goldobj = obj;
            if (prevobj)
                prevobj->nobj = goldobj->nobj;
            else
                invent = goldobj->nobj;
            break;
        }
    /* second, re-letter the rest of the list */
    for (obj = invent, i = 0; obj; obj = obj->nobj, i++)
        obj->invlet =
            (i < 26) ? ('a' + i) : (i < 52) ? ('A' + i - 26) : NOINVSYM;
    /* third, assign gold the "letter" '$' and re-insert it at head */
    if (goldobj) {
        goldobj->invlet = GOLD_SYM;
        goldobj->nobj = invent;
        invent = goldobj;
    }
    if (i >= 52)
        i = 52 - 1;
    lastinvnr = i;
}

/* #adjust command
 *
 *      User specifies a 'from' slot for inventory stack to move,
 *      then a 'to' slot for its destination.  Open slots and those
 *      filled by compatible stacks are listed as likely candidates
 *      but user can pick any inventory letter (including 'from').
 *
 *  to == from, 'from' has a name
 *      All compatible items (same name or no name) are gathered
 *      into the 'from' stack.  No count is allowed.
 *  to == from, 'from' does not have a name
 *      All compatible items without a name are gathered into the
 *      'from' stack.  No count is allowed.  Compatible stacks with
 *      names are left as-is.
 *  to != from, no count
 *      Move 'from' to 'to'.  If 'to' is not empty, merge 'from'
 *      into it if possible, otherwise swap it with the 'from' slot.
 *  to != from, count given
 *      If the user specifies a count when choosing the 'from' slot,
 *      and that count is less than the full size of the stack,
 *      then the stack will be split.  The 'count' portion is moved
 *      to the destination, and the only candidate for merging with
 *      it is the stack already at the 'to' slot, if any.  When the
 *      destination is non-empty but won't merge, whatever is there
 *      will be moved to an open slot; if there isn't any open slot
 *      available, the adjustment attempt fails.
 *
 *      To minimize merging for 'from == to', unnamed stacks will
 *      merge with named 'from' but named ones won't merge with
 *      unnamed 'from'.  Otherwise attempting to collect all unnamed
 *      stacks would lump the first compatible named stack with them
 *      and give them its name.
 *
 *      To maximize merging for 'from != to', compatible stacks will
 *      merge when either lacks a name (or they already have the same
 *      name).  When no count is given and one stack has a name and
 *      the other doesn't, the merged result will have that name.
 *      However, when splitting results in a merger, the name of the
 *      destination overrides that of the source, even if destination
 *      is unnamed and source is named.
 */
int
doorganize() /* inventory organizer by Del Lamb */
{
    struct obj *obj, *otmp, *splitting, *bumped;
    int ix, cur, trycnt, goldstacks;
    char let;
#define GOLD_INDX   0
#define GOLD_OFFSET 1
#define OVRFLW_INDX (GOLD_OFFSET + 52) /* past gold and 2*26 letters */
    char lets[1 + 52 + 1 + 1]; /* room for '$a-zA-Z#\0' */
    char allowall[4]; /* { ALLOW_COUNT, ALL_CLASSES, 0, 0 } */
    char *objname, *uobjname, *otmpname, *uotmpname;
    const char *adj_type;
    boolean ever_mind = FALSE, collect;

    if (!invent) {
        You1("aren't carrying anything to adjust.");
        return 0;
    }

    if (!flags.invlet_constant)
        reassign();
    /* get object the user wants to organize (the 'from' slot) */
    allowall[0] = ALLOW_COUNT;
    allowall[1] = ALL_CLASSES;
    allowall[2] = '\0';
    for (goldstacks = 0, otmp = invent; otmp; otmp = otmp->nobj) {
        /* gold should never end up in a letter slot, nor should two '$'
           slots occur, but if they ever do, allow #adjust to handle them
           (in the past, things like this have happened, usually due to
           bknown being erroneously set on one stack, clear on another;
           object merger isn't fooled by that anymore) */
        if (otmp->oclass == COIN_CLASS
            && (otmp->invlet != GOLD_SYM || ++goldstacks > 1)) {
            allowall[1] = COIN_CLASS;
            allowall[2] = ALL_CLASSES;
            allowall[3] = '\0';
            break;
        }
    }
    if (!(obj = getobj(allowall, "adjust", 1, "")))
        return 0;

    /* figure out whether user gave a split count to getobj() */
    splitting = bumped = 0;
    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->nobj == obj) { /* knowledge of splitobj() operation */
            if (otmp->invlet == obj->invlet)
                splitting = otmp;
            break;
        }

    /* initialize the list with all lower and upper case letters */
    lets[GOLD_INDX] = (obj->oclass == COIN_CLASS) ? GOLD_SYM : ' ';
    for (ix = GOLD_OFFSET, let = 'a'; let <= 'z';)
        lets[ix++] = let++;
    for (let = 'A'; let <= 'Z';)
        lets[ix++] = let++;
    lets[OVRFLW_INDX] = ' ';
    lets[sizeof lets - 1] = '\0';
    /* for floating inv letters, truncate list after the first open slot */
    if (!flags.invlet_constant && (ix = inv_cnt(FALSE)) < 52)
        lets[ix + (splitting ? 0 : 1)] = '\0';

    /* blank out all the letters currently in use in the inventory
       except those that will be merged with the selected object */
    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp != obj && !mergable(otmp, obj)) {
            let = otmp->invlet;
            if (let >= 'a' && let <= 'z')
                lets[GOLD_OFFSET + let - 'a'] = ' ';
            else if (let >= 'A' && let <= 'Z')
                lets[GOLD_OFFSET + let - 'A' + 26] = ' ';
            /* overflow defaults to off, but it we find a stack using that
               slot, switch to on -- the opposite of normal invlet handling */
            else if (let == NOINVSYM)
                lets[OVRFLW_INDX] = NOINVSYM;
        }

    /* compact the list by removing all the blanks */
    for (ix = cur = 0; lets[ix]; ix++)
        if (lets[ix] != ' ' && cur++ < ix)
            lets[cur - 1] = lets[ix];
    lets[cur] = '\0';
    /* and by dashing runs of letters */
    if (cur > 5)
        compactify(lets);

    /* get 'to' slot to use as destination */
    for (trycnt = 1; ; ++trycnt) {
#if GNH_MOBILE
        let = '?';
#else
        char qbuf[QBUFSZ];
        Sprintf(qbuf, "Adjust letter to what [%s]%s?", lets,
            invent ? " (? see used letters)" : "");
        //let = yn_function(qbuf, (char *)0, '\0', (char *)0);
        let = yn_function_ex(YN_STYLE_GENERAL, ATR_NONE, NO_COLOR, NO_GLYPH, (const char*)0, qbuf, (const char*)0, '\0', (const char*)0, (const char*)0, 1UL);
#endif
        if (let == '?' || let == '*') {
            let = display_used_invlets(splitting ? obj->invlet : 0);
            if (!let)
                continue;
            if (let == '\033')
                goto noadjust;
        }
        if (index(quitchars, let)
            /* adjusting to same slot is meaningful since all
               compatible stacks get collected along the way,
               but splitting to same slot is not */
            || (splitting && let == obj->invlet)) {
 noadjust:
            if (splitting)
                (void) merged(&splitting, &obj);
            if (!ever_mind)
                pline1(Never_mind);
            return 0;
        } else if (let == GOLD_SYM && obj->oclass != COIN_CLASS) {
            pline("Only gold coins may be moved into the '%c' slot.",
                  GOLD_SYM);
            ever_mind = TRUE;
            goto noadjust;
        }
        /* letter() classifies '@' as one; compactify() can put '-' in lets;
           the only thing of interest that index() might find is '$' or '#'
           since letter() catches everything else that we put into lets[] */
        if ((letter(let) && let != '@') || (index(lets, let) && let != '-'))
            break; /* got one */
        if (trycnt == 5)
            goto noadjust;
        pline1("Select an inventory slot letter."); /* else try again */
    }

    collect = (let == obj->invlet);
    /* change the inventory and print the resulting item */
    adj_type = collect ? "Collecting" : !splitting ? "Moving:" : "Splitting:";

    /*
     * don't use freeinv/addinv to avoid double-touching artifacts,
     * dousing lamps, losing luck, cursing loadstone, etc.
     */
    extract_nobj(obj, &invent);

    for (otmp = invent; otmp;) {
        /* it's tempting to pull this outside the loop, but merged() could
           free ONAME(obj) [via obfree()] and replace it with ONAME(otmp) */
        objname = has_oname(obj) ? ONAME(obj) : (char *) 0;
        uobjname = has_uoname(obj) ? UONAME(obj) : (char*)0;

        if (collect) {
            /* Collecting: #adjust an inventory stack into its same slot;
               keep it there and merge other compatible stacks into it.
               Traditional inventory behavior is to merge unnamed stacks
               with compatible named ones; we only want that if it is
               the 'from' stack (obj) with a name and candidate (otmp)
               without one, not unnamed 'from' with named candidate. */
            otmpname = has_oname(otmp) ? ONAME(otmp) : (char *) 0;
            uotmpname = has_uoname(otmp) ? UONAME(otmp) : (char*)0;
            if ((!otmpname || (objname && !strcmp(objname, otmpname)))
                && (!uotmpname || (uobjname && !strcmp(uobjname, uotmpname)))
                && merged(&otmp, &obj)) {
                adj_type = "Merging:";
                obj = otmp;
                otmp = otmp->nobj;
                extract_nobj(obj, &invent);
                continue; /* otmp has already been updated */
            }
        } else if (otmp->invlet == let) {
            /* Moving or splitting: don't merge extra compatible stacks.
               Found 'otmp' in destination slot; merge if compatible,
               otherwise bump whatever is there to an open slot. */
            if (!splitting) {
                adj_type = "Swapping:";
                otmp->invlet = obj->invlet;
            } else {
                /* strip 'from' name if it has one */
                if (uobjname) // && !obj->oartifact)
                    UONAME(obj) = (char *) 0;
                if (!mergable(otmp, obj)) {
                    /* won't merge; put 'from' name back */
                    if (uobjname)
                        UONAME(obj) = uobjname;
                } else {
                    /* will merge; discard 'from' name */
                    if (objname)
                        free((genericptr_t) objname), objname = 0;
                }

                if (merged(&otmp, &obj)) {
                    adj_type = "Splitting and merging:";
                    obj = otmp;
                    extract_nobj(obj, &invent);
                } else if (inv_cnt(FALSE) >= 52) {
                    (void) merged(&splitting, &obj); /* undo split */
                    /* "knapsack cannot accommodate any more items" */
                    Your("pack is too full.");
                    return 0;
                } else {
                    bumped = otmp;
                    extract_nobj(bumped, &invent);
                }
            } /* moving vs splitting */
            break; /* not collecting and found 'to' slot */
        } /* collect */
        otmp = otmp->nobj;
    }

    /* inline addinv; insert loose object at beginning of inventory */
    obj->invlet = let;
    obj->nobj = invent;
    obj->where = OBJ_INVENT;
    invent = obj;
    reorder_invent();
    if (bumped) {
        /* splitting the 'from' stack is causing an incompatible
           stack in the 'to' slot to be moved into an open one;
           we need to do another inline insertion to inventory */
        assigninvlet(bumped);
        bumped->nobj = invent;
        bumped->where = OBJ_INVENT;
        invent = bumped;
        reorder_invent();
    }

    /* messages deferred until inventory has been fully reestablished */
    prinv(adj_type, obj, 0L);
    if (bumped)
        prinv("Moving:", bumped, 0L);
    if (splitting)
        clear_splitobjs(); /* reset splitobj context */
    update_inventory();
    return 0;
}

/* common to display_minventory and display_cinventory */
STATIC_OVL void
invdisp_nothing(hdr, txt)
const char *hdr, *txt;
{
    winid win;
    anything any;
    menu_item *selected;

    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu_ex(win, GHMENU_STYLE_OTHERS_INVENTORY);
    add_extended_menu(win, NO_GLYPH, &any, menu_heading_info(), 0, 0, iflags.menu_headings, hdr,
             MENU_UNSELECTED);
    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, "", MENU_UNSELECTED);
    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, txt, MENU_UNSELECTED);
    end_menu(win, (char *) 0);
    if (select_menu(win, PICK_NONE, &selected) > 0)
        free((genericptr_t) selected);
    destroy_nhwindow(win);
    return;
}

/* query_objlist callback: return things that are worn or wielded */
STATIC_OVL boolean
worn_wield_only(obj)
struct obj *obj;
{
#if 1
    /* check for things that *are* worn or wielded (only used for monsters,
       so we don't worry about excluding W_CHAIN, W_ARTIFACT_INVOKED and the like) */
    return (boolean) (obj->owornmask != 0L);
#else
    /* this used to check for things that *might* be worn or wielded,
       but that's not particularly interesting */
    if (is_weptool(obj) || is_wet_towel(obj) || obj->otyp == MEAT_RING)
        return TRUE;
    return (boolean) (obj->oclass == WEAPON_CLASS
                      || obj->oclass == ARMOR_CLASS
                      || obj->oclass == AMULET_CLASS
                      || obj->oclass == RING_CLASS);
#endif
}

/*
 * Display a monster's inventory.
 * Returns a pointer to the object from the monster's inventory selected
 * or NULL if nothing was selected.
 *
 * By default, only worn and wielded items are displayed.  The caller
 * can pick one.  Modifier flags are:
 *
 *      PICK_NONE, PICK_ONE - standard menu control
 *      PICK_ANY            - allowed, but we only return a single object
 *      MINV_NOLET          - nothing selectable
 *      MINV_ALL            - display all inventory
 */
struct obj *
display_minventory(mon, dflags, title)
register struct monst *mon;
int dflags;
char *title;
{
    struct obj *ret;
    char tmp[QBUFSZ];
    int n;
    menu_item *selected = 0;
    int do_all = (dflags & MINV_ALL) != 0,
        incl_hero = (do_all && u.uswallow && mon == u.ustuck),
        have_inv = (mon->minvent != 0), have_any = (have_inv || incl_hero),
        pickings = (dflags & MINV_PICKMASK);

    Sprintf(tmp, "%s %s:", s_suffix(noit_Monnam(mon)),
            do_all ? "possessions" : "armament");

    if (do_all ? have_any : (mon->worn_item_flags || MON_WEP(mon))) {
        /* Fool the 'weapon in hand' routine into
         * displaying 'weapon in claw', etc. properly.
         */
        youmonst.data = mon->data;
        /* in case inside a shop, don't append "for sale" prices */
        iflags.suppress_price++;

        n = query_objlist(title ? title : tmp, &(mon->minvent),
                          (INVORDER_SORT | (incl_hero ? INCLUDE_HERO : 0)),
                          &selected, pickings,
                          do_all ? allow_all : worn_wield_only, 5); //Looking at things in monster's inventory far away

        iflags.suppress_price--;
        /* was 'set_uasmon();' but that potentially has side-effects */
        youmonst.data = &mons[u.umonnum]; /* most basic part of set_uasmon */
    } else {
        invdisp_nothing(title ? title : tmp, "(none)");
        n = 0;
    }

    if (n > 0) {
        ret = selected[0].item.a_obj;
        free((genericptr_t) selected);
    } else
        ret = (struct obj *) 0;
    return ret;
}

/*
 * Display the contents of a container in inventory style.
 * Currently, this is only used for statues, via wand of probing.
 */
struct obj *
display_cinventory(obj)
register struct obj *obj;
{
    struct obj *ret;
    char qbuf[QBUFSZ];
    int n;
    menu_item *selected = 0;

    (void) safe_qbuf(qbuf, "Contents of ", ":", obj, doname, ansimpleoname,
                     "that");

    if (obj->cobj) {
        n = query_objlist(qbuf, &(obj->cobj), INVORDER_SORT,
                          &selected, PICK_NONE, allow_all, 5); //Looking at things in container's inventory far away
    } else {
        invdisp_nothing(qbuf, "(empty)");
        n = 0;
    }
    if (n > 0) {
        ret = selected[0].item.a_obj;
        free((genericptr_t) selected);
    } else
        ret = (struct obj *) 0;
    obj->cknown = 1;
    return ret;
}

/* query objlist callback: return TRUE if obj is at given location */
static coord only;

STATIC_OVL boolean
only_here(obj)
struct obj *obj;
{
    return (obj->ox == only.x && obj->oy == only.y);
}

/*
 * Display a list of buried items in inventory style.  Return a non-zero
 * value if there were items at that spot.
 *
 * Currently, this is only used with a wand of probing zapped downwards.
 */
int
display_binventory(x, y, as_if_seen)
int x, y;
boolean as_if_seen;
{
    struct obj *obj;
    menu_item *selected = 0;
    int n;

    /* count # of objects here */
    for (n = 0, obj = level.buriedobjlist; obj; obj = obj->nobj)
        if (obj->ox == x && obj->oy == y) {
            if (as_if_seen)
                obj->dknown = 1;
            n++;
        }

    if (n) {
        only.x = x;
        only.y = y;
        if (query_objlist("Things that are buried here:",
                          &level.buriedobjlist, INVORDER_SORT,
                          &selected, PICK_NONE, only_here, 2) > 0)
            free((genericptr_t) selected);
        only.x = only.y = 0;
    }
    return n;
}

const char*
adjusted_dfeature_at(x, y)
int x, y;
{
    if (!isok(x, y))
        return 0;

    const char* dfeature = dfeature_at(x, y);
    if (dfeature && !strcmp(dfeature, "pool of water") && Underwater)
        dfeature = 0;

    return dfeature;
}

int
here_window_display_style(dfeature, first_obj_here)
const char* dfeature;
struct obj* first_obj_here;
{
    int total_count = 0;
    struct obj* otmp_cnt;
    for (otmp_cnt = first_obj_here; otmp_cnt; otmp_cnt = otmp_cnt->nexthere)
        total_count++;

    if (total_count == 0 && !dfeature)
        return 0;

    if (total_count + (dfeature ? 1 : 0) > iflags.wc2_here_window_size)
        return 2; /* Many things */

    return 1;
}

/*invent.c*/

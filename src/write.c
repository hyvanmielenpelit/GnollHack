/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    write.c    $NHDT-Date: 1450261366 2015/12/16 10:22:46 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.17 $ */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL boolean FDECL(label_known, (int, struct obj *));
STATIC_DCL char *FDECL(new_book_description, (int, char *));

/*
 * returns basecost of a scroll or a spellbook
 */
int
ink_cost(otmp)
register struct obj* otmp;
{
    if (!otmp)
        return 0;

    return otyp_ink_cost(otmp->otyp);
}

int
otyp_ink_cost(otyp)
int otyp;
{
    if (objects[otyp].oc_class == SPBOOK_CLASS)
    {
        if (otyp == SPE_NOVEL)
            return 30; /* It is very long */
        else if (otyp == SPE_MANUAL)
            return 20; /* It is long */
        else
            return (10 * ((int)objects[otyp].oc_spell_level + 2));
    }

    switch (otyp)
    {
    case SCR_MAIL:
        return 2;
    case SCR_LIGHT:
    case SCR_GOLD_DETECTION:
    case SCR_FOOD_DETECTION:
    case SCR_MAGIC_MAPPING:
    case SCR_AMNESIA:
    case SCR_RETRAINING:
    case SCR_FIRE:
    case SCR_EARTH:
        return 8;
    case SCR_DESTROY_ARMOR:
    case SCR_CREATE_MONSTER:
    case SCR_PUNISHMENT:
        return 10;
    case SCR_CONFUSE_MONSTER:
    case SCR_STINKING_CLOUD:
        return 12;
    case SCR_ENCHANT_ACCESSORY:
    case SCR_IDENTIFY:
    case SCR_TELEPORTATION:
        return 14;
    case SCR_REMOVE_CURSE:
    case SCR_ENCHANT_ARMOR:
    case SCR_ENCHANT_WEAPON:
        return 16;
    case SCR_PROTECT_ARMOR:
    case SCR_PROTECT_WEAPON:
    case SCR_SCARE_MONSTER:
    case SCR_TAMING:
    case SCR_CONFLICT:
        return 20;
    case SCR_CHARGING:
        return 24;
    case SCR_GENOCIDE:
    case SCR_SUPREME_DIABOLISM:
        return 30;
    case SCR_BLANK_PAPER:
    default:
        impossible("You can't write such a weird scroll!");
    }
    return 1000;
}

/* decide whether the hero knowns a particular scroll's label;
   unfortunately, we can't track things that haven't been added to
   the discoveries list and aren't present in current inventory,
   so some scrolls with ought to yield True will end up False */
STATIC_OVL boolean
label_known(scrolltype, objlist)
int scrolltype;
struct obj *objlist;
{
    struct obj *otmp;

    /* only scrolls */
    if (objects[scrolltype].oc_class != SCROLL_CLASS)
        return FALSE;
    /* type known implies full discovery; otherwise,
       user-assigned name implies partial discovery */
    if (objects[scrolltype].oc_name_known || objects[scrolltype].oc_uname)
        return TRUE;
    /* check inventory, including carried containers with known contents */
    for (otmp = objlist; otmp; otmp = otmp->nobj) {
        if (otmp->otyp == scrolltype && otmp->dknown)
            return TRUE;
        if (Has_contained_contents(otmp) && otmp->cknown
            && label_known(scrolltype, otmp->cobj))
            return TRUE;
    }
    /* not found */
    return FALSE;
}

STATIC_VAR NEARDATA const char write_on[] = { SCROLL_CLASS, SPBOOK_CLASS, 0 };

/* write -- applying a magic marker */
int
dowrite(pen)
register struct obj *pen;
{
    register struct obj *paper;
    char namebuf[BUFSZ] = DUMMY, *nm, *bp;
    register struct obj *new_obj;
    int basecost, actualcost;
    int curseval;
    char qbuf[QBUFSZ];
    int first, last, i, deferred, deferralchance;
    boolean by_descr = FALSE;
    const char *typeword;

    if (nohands(youmonst.data)) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "need hands to be able to write!");
        return 0;
    } 
    else if (Glib) 
    {
        pline("%s from your %s.", Tobjnam(pen, "slip"),
              makeplural(body_part(FINGER)));
        dropxf(pen);
        return 1;
    }

    /* get paper to write on */
    paper = getobj(write_on, "write on", 0, "");
    if (!paper)
        return 0;
    /* can't write on a novel (unless/until it's been converted into a blank
       spellbook), but we want messages saying so to avoid "spellbook" */
    typeword = (paper->oclass == SPBOOK_CLASS) ? 
        (paper->otyp == SPE_NOVEL || (objects[paper->otyp].oc_subtyp == BOOKTYPE_NOVEL) ? "novel" :
            paper->otyp == SPE_MANUAL || (objects[paper->otyp].oc_subtyp == BOOKTYPE_MANUAL) ? "manual" :
                objects[paper->otyp].oc_subtyp != BOOKTYPE_SPELLBOOK ? "book": "spellbook")
                     : "scroll";
    if (Blind) 
    {
        if (!paper->dknown) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't know if that %s is blank or not.", typeword);
            return 0;
        }
        else if (paper->oclass == SPBOOK_CLASS) 
        {
            /* can't write a magic book while blind */
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s can't create braille text.",
                  upstart(ysimple_name(pen)));
            return 0;
        }
    }
    paper->dknown = 1;
    if (paper->otyp != SCR_BLANK_PAPER && paper->otyp != SPE_BLANK_PAPER && !((paper->otyp == SPE_NOVEL || paper->otyp == SPE_MANUAL) && paper->special_quality == -1)) 
    {
        play_sfx_sound(SFX_GENERAL_NOT_IN_THE_RIGHT_CONDITION);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "That %s is not blank!", typeword);
        exercise(A_WIS, FALSE);
        return 1;
    }

    /* what to write */
    Sprintf(qbuf, "What type of %s do you want to write?", typeword);
    getlin(qbuf, namebuf);
    (void) mungspaces(namebuf); /* remove any excess whitespace */
    if (namebuf[0] == '\033' || !namebuf[0])
        return 1;
    nm = namebuf;
    if (!strncmpi(nm, "scroll ", 7))
        nm += 7;
    else if (!strncmpi(nm, "spellbook ", 10))
        nm += 10;
    else if (!strncmpi(nm, "manual ", 7) && strncmpi(nm, "manual of the planes", 20))
        nm += 7;
    else if (!strncmpi(nm, "novel ", 6))
        nm += 6;

    if (!strncmpi(nm, "of ", 3))
        nm += 3;
    else if (!strncmpi(nm, "entitled ", 9))
        nm += 9;
    else if (!strncmpi(nm, "called ", 7))
        nm += 7;
    else if (!strncmpi(nm, "named ", 6))
        nm += 6;

    if ((bp = strstri(nm, " armour")) != 0) {
        Strncpy(bp, " armor ", 7); /* won't add '\0' */
        (void) mungspaces(bp + 1);        /* remove the extra space */
    }

    const char* bookname = 0;
    short bookidx = -1;
    if (paper->otyp == SPE_NOVEL)
    {
        i = SPE_NOVEL;
        bookname = lookup_novel(nm, &bookidx);
        if (bookname)
            goto found_novel_or_manual;
    }
    else if (paper->otyp == SPE_MANUAL)
    {
        i = SPE_MANUAL;
        bookname = lookup_manual(nm, &bookidx);
        if (bookname)
            goto found_novel_or_manual;
    }
    else
    {
        deferred = 0;       /* not any scroll or book */
        deferralchance = 0; /* incremented for each oc_uname match */
        first = bases[(int)paper->oclass];
        last = bases[(int)paper->oclass + 1] - 1;
        for (i = first; i <= last; i++) {
            /* extra shufflable descr not representing a real object */
            if (!OBJ_NAME(objects[i]))
                continue;

            if (!strcmpi(OBJ_NAME(objects[i]), nm))
                goto found;
            if (OBJ_DESCR(objects[i]) && !strcmpi(OBJ_DESCR(objects[i]), nm)) {
                by_descr = TRUE;
                goto found;
            }
            /* user-assigned name might match real name of a later
               entry, so we don't simply use first match with it;
               also, player might assign same name multiple times
               and if so, we choose one of those matches randomly */
            if (objects[i].oc_uname && !strcmpi(objects[i].oc_uname, nm)
                /*
                 * First match: chance incremented to 1,
                 *   !rn2(1) is 1, we remember i;
                 * second match: chance incremented to 2,
                 *   !rn2(2) has 1/2 chance to replace i;
                 * third match: chance incremented to 3,
                 *   !rn2(3) has 1/3 chance to replace i
                 *   and 2/3 chance to keep previous 50:50
                 *   choice; so on for higher match counts.
                 */
                && !rn2(++deferralchance))
                deferred = i;
        }
        /* writing by user-assigned name is same as by description:
           fails for books, works for scrolls (having an assigned
           type name guarantees presence on discoveries list) */
        if (deferred) {
            i = deferred;
            by_descr = TRUE;
            goto found;
        }
    }

    play_sfx_sound(SFX_GENERAL_CANNOT);
    There_ex(ATR_NONE, CLR_MSG_FAIL, "is no such %s!", typeword);
    return 1;
found:

    if (i == SCR_BLANK_PAPER || i == SPE_BLANK_PAPER) {
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "write that!");
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "It's obscene!");
        return 1;
    } else if (i == SPE_BOOK_OF_THE_DEAD || i == SPE_BOOK_OF_MODRON) {
        play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "No mere dungeon adventurer could write that.");
        return 1;
    } else if (i == SCR_SUPREME_DIABOLISM) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot write that with mere ink.");
        return 1;
    } else if (by_descr && paper->oclass == SPBOOK_CLASS
               && !objects[i].oc_name_known) {
        /* can't write unknown spellbooks by description */
        play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Unfortunately you don't have enough information to go on.");
        return 1;
    }

found_novel_or_manual:
    /* KMH, conduct */
    if (!u.uconduct.literate++)
        livelog_printf(LL_CONDUCT,
            "became literate by writing %s", an(typeword));

    new_obj = mksobj(i, FALSE, FALSE, FALSE);
    new_obj->bknown = (paper->bknown && pen->bknown);

    /* shk imposes a flat rate per use, not based on actual charges used */
    check_unpaid(pen);

    /* see if there's enough ink */
    basecost = ink_cost(new_obj);
    if (pen->charges < basecost / 2) {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "marker is too dry to write that!");
        obfree(new_obj, (struct obj *) 0);
        return 1;
    }

    /* we're really going to write now, so calculate cost
     */
    if (objects[i].oc_flags6 & O6_ALWAYS_FULL_INK_COST)
        actualcost = basecost;
    else
        actualcost = rn1(max(2, basecost / 2), basecost / 2);

    curseval = bcsign(pen) + bcsign(paper);
    exercise(A_WIS, TRUE);
    /* dry out marker */
    if (pen->charges < actualcost) 
    {
        pen->charges = 0;
        play_simple_object_sound(pen, OBJECT_SOUND_TYPE_APPLY2);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "marker dries out!");
        /* scrolls disappear, spellbooks don't */
        if (paper->oclass == SPBOOK_CLASS)
        {
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s is left unfinished and your writing fades.", paper->otyp == SPE_NOVEL ? "novel" : paper->otyp == SPE_MANUAL ? "manual" : "spellbook");
        } 
        else
        {
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "scroll is now useless and disappears!");
            useup(paper);
        }
        update_inventory(); /* pen charges */
        obfree(new_obj, (struct obj *) 0);
        return 1;
    }
    pen->charges -= actualcost;

    if (paper->otyp == SPE_NOVEL || paper->otyp == SPE_MANUAL)
    {
        if (bookname && bookidx >= 0)
        {
            /* A bit easier way to show the newly written object than below */
            play_simple_object_sound(pen, OBJECT_SOUND_TYPE_APPLY);
            paper->special_quality = bookidx;
            paper = oname(paper, bookname);
            paper->blessed = (curseval > 0);
            paper->cursed = (curseval < 0);
            prinv((const char*)0, paper, 0L);
            update_inventory(); /* pen charges */
        }
        else
        {
            impossible("Writing a novel or manual without bookname or bookidx?");
        }
        obfree(new_obj, (struct obj*)0);
        return 1;
    }

    /*
     * Writing by name requires that the hero knows the scroll or
     * book type.  One has previously been read (and its effect
     * was evident) or been ID'd via scroll/spell/throne and it
     * will be on the discoveries list.
     * (Previous versions allowed scrolls and books to be written
     * by type name if they were on the discoveries list via being
     * given a user-assigned name, even though doing the latter
     * doesn't--and shouldn't--make the actual type become known.)
     *
     * Writing by description requires that the hero knows the
     * description (a scroll's label, that is, since books by_descr
     * are rejected above).  BUG:  We can only do this for known
     * scrolls and for the case where the player has assigned a
     * name to put it onto the discoveries list; we lack a way to
     * track other scrolls which have been seen closely enough to
     * read the label without then being ID'd or named.  The only
     * exception is for currently carried inventory, where we can
     * check for one [with its dknown bit set] of the same type.
     *
     * Normal requirements can be overridden if hero is Lucky.
     */

    /* if known, then either by-name or by-descr works */
    if (!objects[new_obj->otyp].oc_name_known
        /* else if named, then only by-descr works */
        && !(by_descr && label_known(new_obj->otyp, invent))
        /* and Luck might override after both checks have failed */
        && rnl(Role_if(PM_WIZARD) ? 5 : 15)) 
    {

        play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "%s to write that.", by_descr ? "fail" : "don't know how");
        /* scrolls disappear, spellbooks don't */
        if (paper->oclass == SPBOOK_CLASS) 
        {
            You_ex(ATR_NONE, CLR_MSG_FAIL,
      "write in your best handwriting:  \"My Diary\", but it quickly fades.");
            update_inventory(); /* pen charges */
        }
        else 
        {
            if (by_descr) 
            {
                Strcpy(namebuf, OBJ_DESCR(objects[new_obj->otyp]));
                wipeout_text(namebuf, (6 + MAXULEV - u.ulevel) / 6, 0);
            } else
                Sprintf(namebuf, "%s was here!", plname);
            You_multi_ex(ATR_NONE, CLR_MSG_FAIL, no_multiattrs, multicolor_text1, "write \"%s\" and the scroll disappears.", namebuf);
            useup(paper);
        }
        obfree(new_obj, (struct obj *) 0);
        return 1;
    }
    /* can write scrolls when blind, but requires luck too;
       attempts to write books when blind are caught above */
    if ((Blind && rnl(3)) || (Tottering && rn2(7))) {
        /* writing while blind usually fails regardless of
           whether the target scroll is known; even if we
           have passed the write-an-unknown scroll test
           above we can still fail this one, so it's doubly
           hard to write an unknown scroll while blind */
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to write the scroll correctly and it disappears.");
        useup(paper);
        obfree(new_obj, (struct obj *) 0);
        return 1;
    }

    /* useup old scroll / spellbook */
    useup(paper);

    /* success */
    play_simple_object_sound(pen, OBJECT_SOUND_TYPE_APPLY);
    if (new_obj->oclass == SPBOOK_CLASS)
    {
        /* acknowledge the change in the object's description... */
        pline_The("spellbook warps strangely, then turns %s.",
                  new_book_description(new_obj->otyp, namebuf));
    }
    new_obj->blessed = (curseval > 0);
    new_obj->cursed = (curseval < 0);

    if (new_obj->otyp == SCR_MAIL)
        /* 0: delivered in-game via external event (or randomly for fake mail);
           1: from bones or wishing; 2: written with marker */
        new_obj->special_quality = SPEQUAL_MAIL_FROM_MAGIC_MARKER;
    /* unlike alchemy, for example, a successful result yields the
       specifically chosen item so hero recognizes it even if blind;
       the exception is for being lucky writing an undiscovered scroll,
       where the label associated with the type-name isn't known yet */
    new_obj->dknown = label_known(new_obj->otyp, invent) ? 1 : 0;

    new_obj = hold_another_object(new_obj, "Oops!  %s out of your grasp!",
                                  The(aobjnam(new_obj, "slip")),
                                  (const char *) 0);
    nhUse(new_obj); /* try to avoid complaint about dead assignment */
    return 1;
}

/* most book descriptions refer to cover appearance, so we can issue a
   message for converting a plain book into one of those with something
   like "the spellbook turns red" or "the spellbook turns ragged";
   but some descriptions refer to composition and "the book turns vellum"
   looks funny, so we want to insert "into " prior to such descriptions;
   even that's rather iffy, indicating that such descriptions probably
   ought to be eliminated (especially "cloth"!) */
STATIC_OVL char *
new_book_description(booktype, outbuf)
int booktype;
char *outbuf;
{
    /* subset of description strings from objects.c; if it grows
       much, we may need to add a new flag field to objects[] instead */
    static const char *const compositions[] = {
        "parchment",
        "vellum",
        "cloth",
#if 0
        "canvas", "hardcover", /* not used */
        "papyrus", /* not applicable--can't be produced via writing */
#endif /*0*/
        0
    };
    const char *descr, *const *comp_p;

    descr = OBJ_DESCR(objects[booktype]);
    for (comp_p = compositions; *comp_p; ++comp_p)
        if (!strcmpi(descr, *comp_p))
            break;

    Sprintf(outbuf, "%s%s", *comp_p ? "into " : "", descr);
    return outbuf;
}

/*write.c*/

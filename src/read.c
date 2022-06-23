/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    read.c    $NHDT-Date: 1546465285 2019/01/02 21:41:25 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.164 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define Your_Own_Role(mndx)  \
    ((mndx) == urole.monsternum)

#define Your_Own_Race(mndx)  \
    ((mndx) == urace.monsternum)

boolean known;

static NEARDATA const char readable[] = { ALL_CLASSES, SCROLL_CLASS,
                                          SPBOOK_CLASS, 0 };
static const char all_count[] = { ALLOW_COUNT, ALL_CLASSES, 0 };

STATIC_DCL boolean FDECL(learnscrolltyp, (SHORT_P));
STATIC_DCL char *FDECL(erode_obj_text, (struct obj *, char *));
STATIC_DCL char *FDECL(apron_text, (struct obj *, char *buf));
STATIC_DCL void FDECL(forget_single_object, (int));
#if 0 /* not used */
STATIC_DCL void FDECL(forget_objclass, (int));
#endif
STATIC_DCL void FDECL(randomize, (int *, int));
STATIC_DCL void FDECL(forget, (int));
STATIC_PTR void FDECL(set_lit, (int, int, genericptr));
STATIC_DCL void NDECL(do_class_genocide);

STATIC_OVL boolean
learnscrolltyp(scrolltyp)
short scrolltyp;
{
    if (!objects[scrolltyp].oc_name_known) {
        makeknown(scrolltyp);
        //more_experienced(0, 10);
        return TRUE;
    } else
        return FALSE;
}

/* also called from teleport.c for scroll of teleportation */
void
learnscroll(sobj)
struct obj *sobj;
{
    /* it's implied that sobj->dknown is set;
       we couldn't be reading this scroll otherwise */
    if (sobj->oclass != SPBOOK_CLASS)
        (void) learnscrolltyp(sobj->otyp);
}

STATIC_OVL char *
erode_obj_text(otmp, buf)
struct obj *otmp;
char *buf;
{
    int erosion = greatest_erosion(otmp);

    if (erosion)
        wipeout_text(buf, (int) (strlen(buf) * erosion / (2 * MAX_ERODE)),
                     otmp->o_id ^ (unsigned) ubirthday);
    return buf;
}

char *
tshirt_text(tshirt, buf)
struct obj *tshirt;
char *buf;
{
    static const char *shirt_msgs[] = {
        /* Scott Bigham */
      "I explored the Dungeons of Doom and all I got was this lousy T-shirt!",
        "Is that Mjollnir in your pocket or are you just happy to see me?",
      "It's not the size of your sword, it's how #enhance'd you are with it.",
        "Madame Elvira's House O' Succubi Lifetime Customer",
        "Madame Elvira's House O' Succubi Employee of the Month",
        "Ludios Vault Guards Do It In Small, Dark Rooms",
        "Yendor Military Soldiers Do It In Large Groups",
        "I survived Yendor Military Boot Camp",
        "Ludios Accounting School Intra-Mural Lacrosse Team",
        "Oracle(TM) Fountains 10th Annual Wet T-Shirt Contest",
        "Hey, black dragon!  Disintegrate THIS!",
        "I'm With Stupid -->",
        "Don't blame me, I voted for Izchak!",
        "Don't Panic", /* HHGTTG */
        "Furinkan High School Athletic Dept.",                /* Ranma 1/2 */
        "Hel-LOOO, Nurse!",                                   /* Animaniacs */
        "=^.^=",
        "100% goblin hair - do not wash",
        "Aberzombie and Fitch",
        "cK -- Cockatrice touches the Kop",
        "Don't ask me, I only adventure here",
        "Down with pants!",
        "d, your dog or a killer?",
        "FREE PUG AND NEWT!",
        "Go team ant!",
        "Got newt?",
        "Hello, my darlings!", /* Charlie Drake */
        "Hey!  Nymphs!  Steal This T-Shirt!",
        "I <3 Dungeon of Doom",
        "I <3 Maud",
        "I am a Valkyrie.  If you see me running, try to keep up.",
        "I am not a pack rat - I am a collector",
        "I bounced off a rubber tree",         /* Monkey Island */
        "Plunder Island Brimstone Beach Club", /* Monkey Island */
        "If you can read this, I can hit you with my polearm",
        "I'm confused!",
        "I scored with the princess",
        "I want to live forever or die in the attempt.",
        "Lichen Park",
        "LOST IN THOUGHT - please send search party",
        "Meat is Mordor",
        "Minetown Better Business Bureau",
        "Minetown Watch",
 "Ms. Palm's House of Negotiable Affection -- A Very Reputable House Of Disrepute",
        "Protection Racketeer",
        "Real men love Crom",
        "Somebody stole my Mojo!",
        "The Hellhound Gang",
        "The Werewolves",
        "They Might Be Storm Giants",
        "Weapons don't kill people, I kill people",
        "White Zombie",
        "You're killing me!",
        "Anhur State University - Home of the Fighting Fire Ants!",
        "FREE HUGS",
        "Serial Ascender",
        "Real men are valkyries",
        "Young Men's Cavedigging Association",
        "Occupy Fort Ludios",
        "I couldn't afford this T-shirt so I stole it!",
        "Tentacled ones suck",
        "I'm not wearing any pants",
        "Down with the living!",
        "Pudding farmer",
        "Vegetarian",
        "Hello, I'm War!",
        "It is better to light a candle than to curse the darkness",
        "It is easier to curse the darkness than to light a candle",
        /* expanded "rock--paper--scissors" featured in TV show "Big Bang
           Theory" although they didn't create it (and an actual T-shirt
           with pentagonal diagram showing which choices defeat which) */
        "rock--paper--scissors--lizard--Spock!",
        /* "All men must die -- all men must serve" challange and response
           from book series _A_Song_of_Ice_and_Fire_ by George R.R. Martin,
           TV show "Game of Thrones" (probably an actual T-shirt too...) */
        "/Valar morghulis/ -- /Valar dohaeris/",
    };

    Strcpy(buf, shirt_msgs[tshirt->o_id % SIZE(shirt_msgs)]);
    return erode_obj_text(tshirt, buf);
}

STATIC_OVL char *
apron_text(apron, buf)
struct obj *apron;
char *buf;
{
    static const char *apron_msgs[] = {
        "Kiss the cook",
        "I'm making SCIENCE!",
        "Don't mess with the chef",
        "Don't make me poison you",
        "Gehennom's Kitchen",
        "Rat: The other white meat",
        "If you can't stand the heat, get out of Gehennom!",
        "If we weren't meant to eat animals, why are they made out of meat?",
        "If you don't like the food, I'll stab you",
    };

    Strcpy(buf, apron_msgs[apron->o_id % SIZE(apron_msgs)]);
    return erode_obj_text(apron, buf);
}

int
doread()
{
    register struct obj *scroll;
    boolean confused, nodisappear;

    known = FALSE;
    if (check_capacity((char *) 0))
        return 0;
    scroll = getobj(readable, "read", 0, "");
    if (!scroll)
        return 0;

    //Apply only to reading spellbooks, such as Book of the Dead
    if (scroll->oclass == SPBOOK_CLASS && scroll->cooldownleft > 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "cannot read %s before its cooldown has expired.", the(cxname(scroll)));
        return 0;
    }

    /* outrumor has its own blindness check */
    if (scroll->otyp == FORTUNE_COOKIE) 
    {
        if (flags.verbose)
            You("break up the cookie and throw away the pieces.");
        outrumor((struct monst*)0, scroll, bcsign(scroll), BY_COOKIE);
        if (!Blind)
            u.uconduct.literate++;
        useup(scroll);
        return 1;
    } 
    else if (scroll->oclass == ARMOR_CLASS && (objects[scroll->otyp].oc_armor_category == ARM_SHIRT || scroll->otyp == ALCHEMY_SMOCK)) 
    {
        char buf[BUFSZ], *mesg;
        const char *endpunct;

        if (Blind) 
        {
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "feel any Braille writing.");
            return 0;
        }
        /* can't read shirt worn under suit (under cloak is ok though) */
        if (objects[scroll->otyp].oc_armor_category == ARM_SHIRT && (uarm || uarmo) && scroll == uarmu) {
            pline("%s shirt is obscured by %s%s.",
                  scroll->unpaid ? "That" : "Your", shk_your(buf, uarmo ? uarmo : uarm),
                  uarmo ? robe_simple_name(uarmo) : suit_simple_name(uarm));
            return 0;
        }
        u.uconduct.literate++;
        /* populate 'buf[]' */
        if (scroll->otyp == SHIRT_OF_SOUND_MINDEDNESS)
        {
            pline("This T-shirt has the words \"Sound Mind Games\" emblazoned upon it in large friendly letters.");
            if (!objects[scroll->otyp].oc_name_known)
            {
                makeknown(scroll->otyp);
                prinv((char*)0, scroll, 0L);
            }
        }
        else if (scroll->otyp == SHIRT_OF_UNCONTROLLABLE_LAUGHTER)
        {
            pline("This T-shirt has a message of incomparable hilarity emblazoned upon it.");
            if (!objects[scroll->otyp].oc_name_known)
            {
                makeknown(scroll->otyp);
                prinv((char*)0, scroll, 0L);
            }
        }
        else if (scroll->otyp == SHIRT_OF_COMELINESS || scroll->otyp == HAWAIIAN_SHIRT)
        {
            pline("There is no text on this T-shirt.");
        }
        else
        {
            mesg = (scroll->otyp == ALCHEMY_SMOCK) ? apron_text(scroll, buf)
                : tshirt_text(scroll, buf);
            endpunct = "";
            if (flags.verbose) 
            {
                int ln = (int)strlen(mesg);

                /* we will be displaying a sentence; need ending punctuation */
                if (ln > 0 && !index(".!?", mesg[ln - 1]))
                    endpunct = ".";
                pline("It reads:");
            }
            pline("\"%s\"%s", mesg, endpunct);
        }
        return 1;
    } 
    else if (scroll->otyp == CREDIT_CARD) 
    {
        static const char *card_msgs[] = {
            "Leprechaun Gold Tru$t - Shamrock Card",
            "Magic Memory Vault Charge Card",
            "Larn National Bank",                /* Larn */
            "First Bank of Omega",               /* Omega */
            "Bank of Zork - Frobozz Magic Card", /* Zork */
            "Ankh-Morpork Merchant's Guild Barter Card",
            "Ankh-Morpork Thieves' Guild Unlimited Transaction Card",
            "Ransmannsby Moneylenders Association",
            "Bank of Gehennom - 99% Interest Card",
            "Yendorian Express - Copper Card",
            "Yendorian Express - Silver Card",
            "Yendorian Express - Gold Card",
            "Yendorian Express - Mithril Card",
            "Yendorian Express - Platinum Card", /* must be last */
        };

        if (Blind) {
            You("feel the embossed numbers:");
        } else {
            if (flags.verbose)
                pline("It reads:");
            pline("\"%s\"",
                  scroll->oartifact
                      ? card_msgs[SIZE(card_msgs) - 1]
                      : card_msgs[scroll->o_id % (SIZE(card_msgs) - 1)]);
        }
        /* Make a credit card number */
        pline("\"%d0%d %ld%d1 0%d%d0\"%s",
              (((int) scroll->o_id % 89) + 10),
              ((int) scroll->o_id % 4),
              ((((long) scroll->o_id * 499L) % 899999L) + 100000L),
              ((int) scroll->o_id % 10),
              (!((int) scroll->o_id % 3)),
              (((int) scroll->o_id * 7) % 10),
              (flags.verbose || Blind) ? "." : "");
        u.uconduct.literate++;
        return 1;
    }
    else if (scroll->otyp == CAN_OF_GREASE || scroll->otyp == JAR_OF_MEDICINAL_SALVE || scroll->otyp == JAR_OF_BASILISK_BLOOD || scroll->otyp == JAR_OF_EXTRA_HEALING_SALVE || scroll->otyp == JAR_OF_GREATER_HEALING_SALVE || scroll->otyp == JAR_OF_PRODIGIOUS_HEALING_SALVE || scroll->otyp == GRAIL_OF_HEALING) 
    {
        pline("This %s has no label.", singular(scroll, xname));
        return 0;
    } 
    else if (scroll->otyp == MAGIC_MARKER) 
    {
        if (Blind) 
        {
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "feel any Braille writing.");
            return 0;
        }
        if (flags.verbose)
            pline("It reads:");
        pline("\"Magic Marker(TM) Red Ink Marker Pen.  Water Soluble.\"");
        u.uconduct.literate++;
        return 1;
    }
    else if (scroll->oclass == COIN_CLASS)
    {
        if (Blind)
            You("feel the embossed words:");
        else if (flags.verbose)
            You("read:");
        
        if(Hallucination)
            pline("\"1 Zorkmid.  857 GUE.  In Frobs We Trust.\"");
        else
            pline("\"1 Yendorian Gold Coin.  Treasury of Yendor.\"");

        u.uconduct.literate++;
        return 1;
    } 
    else if (scroll->oartifact == ART_ORB_OF_FATE) 
    {
        if (Blind)
            You("feel the engraved signature:");
        else
            pline("It is signed:");
        pline("\"Odin.\"");
        u.uconduct.literate++;
        return 1;
    }
    else if (scroll->oartifact == ART_RULING_RING_OF_YENDOR) 
    {
        pline("The ring is plain and featureless, but you somehow feel that there is more to it than that.");
        return 1;
    } 
    else if (scroll->otyp == CANDY_BAR) 
    {
        static const char *wrapper_msgs[] = {
            "Apollo",       /* Lost */
            "Moon Crunchy", /* South Park */
            "Snacky Cake",    "Chocolate Nuggie", "The Small Bar",
            "Crispy Yum Yum", "Nilla Crunchie",   "Berry Bar",
            "Choco Nummer",   "Om-nom", /* Cat Macro */
            "Fruity Oaty",              /* Serenity */
            "Wonka Bar" /* Charlie and the Chocolate Factory */
        };

        if (Blind) 
        {
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "feel any Braille writing.");
            return 0;
        }
        pline("The wrapper reads: \"%s\".",
              wrapper_msgs[scroll->o_id % SIZE(wrapper_msgs)]);
        u.uconduct.literate++;
        return 1;
    }
    else if (scroll->oclass != SCROLL_CLASS
               && scroll->oclass != SPBOOK_CLASS) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline(silly_thing_to, "read");
        return 0;
    }
    else if (Blind /*&& (scroll->otyp != SPE_BOOK_OF_THE_DEAD)*/) 
    { //Not sure why Book of the Dead should be readable when blind --JG
        const char *what = 0;

        if (scroll->oclass == SPBOOK_CLASS)
            what = "mystic runes";
        else if (!scroll->dknown)
            what = "formula on the scroll";

        if (what) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Being blind, you cannot read the %s.", what);
            return 0;
        }
    }

    confused = (Confusion != 0);
#ifdef MAIL
    if (scroll->otyp == SCR_MAIL) 
    {
        confused = FALSE; /* override */
        /* reading mail is a convenience for the player and takes
           place outside the game, so shouldn't affect gameplay;
           on the other hand, it starts by explicitly making the
           hero actively read something, which is pretty hard
           to simply ignore; as a compromise, if the player has
           maintained illiterate conduct so far, and this mail
           scroll didn't come from bones, ask for confirmation */
        if (!u.uconduct.literate) 
        {
            if (!scroll->special_quality && yn_query(
             "Reading mail will violate \"illiterate\" conduct.  Read anyway?"
                                   ) != 'y')
                return 0;
        }
    }
#endif

    /* Actions required to win the game aren't counted towards conduct */
    /* Novel conduct is handled in read_tribute so exclude it too*/
    if (!(objects[scroll->otyp].oc_flags5 & O5_OK_FOR_ILLITERATE))
        u.uconduct.literate++;

    if (scroll->oclass == SPBOOK_CLASS)
    {
        return study_book(scroll);
    }
    scroll->in_use = TRUE; /* scroll, not spellbook, now being read */
    if (scroll->otyp != SCR_BLANK_PAPER)
    {
        boolean silently = !can_chant(&youmonst);

        /* a few scroll feedback messages describe something happening
           to the scroll itself, so avoid "it disappears" for those */
        nodisappear = (scroll->otyp == SCR_FIRE || scroll->otyp == SCR_IDENTIFY
                       || (scroll->otyp == SCR_REMOVE_CURSE
                           && scroll->cursed));

        if (!silently)
            play_simple_object_sound(scroll, OBJECT_SOUND_TYPE_READ); // play_sfx_sound(SFX_READ);

        if (Blind)
            pline(nodisappear
                      ? "You %s the formula on the scroll."
                      : "As you %s the formula on it, the scroll disappears.",
                  silently ? "cogitate" : "pronounce");
        else
            pline(nodisappear ? "You read the scroll."
                              : "As you read the scroll, it disappears.");
        if (confused) 
        {
            if (Hallucination)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Being so trippy, you screw up...");
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Being confused, you %s the magic words...",
                      silently ? "misunderstand" : "mispronounce");
        }
    }
    boolean effect_happened = 0;
    boolean gone = FALSE;
    boolean billable_scroll = FALSE;
    struct monst* shkp = 0;
    if (scroll->unpaid && costly_spot(u.ux, u.uy))
    {
        char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
        shkp = shop_keeper(*o_shop);
        if (shkp && inhishop(shkp) && is_obj_on_shk_bill(scroll, shkp))
        {
            billable_scroll = TRUE;
        }
    }

    if (!(gone = seffects(scroll, &effect_happened))) 
    {
        if (!objects[scroll->otyp].oc_name_known)
        {
            if (known)
                learnscroll(scroll);
            else if (!objects[scroll->otyp].oc_uname)
                docall(scroll);
        }
        scroll->in_use = FALSE;
        if (scroll->otyp != SCR_BLANK_PAPER && scroll->otyp != SCR_IDENTIFY)
        {
            useup(scroll);
            gone = TRUE;
        }
    }

    if(gone && billable_scroll && shkp && !Deaf && !muteshk(shkp))
        play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_ILL_ADD_THAT_TO_YOUR_BILL);

    return 1;
}

void
read_the_ruling_ring(otmp)
struct obj* otmp;
{
    if (!otmp || otmp->oartifact != ART_RULING_RING_OF_YENDOR)
        return;

    boolean revealed = (otmp->speflags & SPEFLAGS_INSCRIPTION_REVEALED) != 0;

    if (Blind)
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You feel that an inscription appears%s on %s, but you cannot see it.", revealed ? " again" : "", yname(otmp));
    else
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s, an inscription appears on %s in fiery letters:", revealed ? "Again" : "Suddenly", yname(otmp));
        verbalize_ex(ATR_NONE, CLR_MSG_ATTENTION, "Ash na... dur...");
        if (otmp->speflags & SPEFLAGS_INSCRIPTION_REVEALED)
        {
            You("still couldn't make much sense of it.");
        }
        else
        {
            if (!otmp->nknown)
            {
                You("quite couldn't make sense of it, but it seemed like a real tongue twister.");
                otmp->nknown = TRUE;
                otmp->aknown = TRUE;
                if (carried(otmp))
                    prinv((char*)0, otmp, 0L);
            }
            else
            {
                You("quite couldn't make sense of what it said.");
            }
        }
        otmp->speflags |= SPEFLAGS_INSCRIPTION_REVEALED;
        u.uconduct.literate++;
    }
}

void
strip_charges(obj, verbose)
register struct obj *obj;
boolean verbose;
{
    if (obj->blessed || obj->charges <= 0)
    {
        if (verbose)
        {
            if(obj->charges <= 0)
                play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
            pline1(nothing_happens);
        }
    } 
    else 
    {
        /* order matters: message, shop handling, actual transformation */
        if (verbose)
        {
            play_sfx_sound(SFX_STRIP_CHARGES);
            pline("%s briefly.", Yobjnam2(obj, "vibrate"));
        }
        costly_alteration(obj, COST_UNCHRG);
        obj->charges = 0;
//        if (obj->otyp == OIL_LAMP || obj->otyp == BRASS_LANTERN)
//            obj->age = 0;
    }
}

void
p_glow1(otmp, pline_attr, pline_color)
register struct obj *otmp;
register int pline_attr, pline_color;
{
    play_sfx_sound(SFX_AURA_GLOW);
    pline_ex(pline_attr, pline_color, "%s briefly.", Yobjnam2(otmp, Blind ? "vibrate" : "glow"));
}

void
p_glow2(otmp, color, pline_attr, pline_color)
register struct obj *otmp;
register const char *color;
register int pline_attr, pline_color;
{
    play_sfx_sound(SFX_AURA_GLOW);
    pline_ex(pline_attr, pline_color, "%s%s%s for a moment.", Yobjnam2(otmp, Blind ? "vibrate" : "glow"),
          Blind ? "" : " ", Blind ? "" : hcolor(color));
}

/* Is the object chargeable?  For purposes of inventory display; it is
   possible to be able to charge things for which this returns FALSE. */
boolean
is_chargeable(obj)
struct obj *obj;
{
    if (obj->oclass == WAND_CLASS)
        return TRUE;

    /* known && !oc_name_known is possible after amnesia/tentacled one */
//    if (obj->oclass == RING_CLASS)
        return (boolean) (objects[obj->otyp].oc_charged
                          && (obj->known
                              || (obj->dknown
                                  && objects[obj->otyp].oc_name_known)));

#if 0
    if (is_weptool(obj)) /* specific check before general tools */
        return FALSE;
    if (obj->oclass == TOOL_CLASS)
        return (boolean) (objects[obj->otyp].oc_charged != 0);
    return FALSE; /* why are weapons/armor considered charged anyway? */
#endif
}

/* recharge an object; curse_bless is -1 if the recharging implement
   was cursed, +1 if blessed, 0 otherwise. */
void
recharge(obj, curse_bless, verbose)
struct obj *obj;
int curse_bless;
boolean verbose;
{
    if (!obj)
        return;

    register int n;
    boolean is_cursed, is_blessed;
    boolean play_effect = FALSE;

    is_cursed = curse_bless < 0;
    is_blessed = curse_bless > 0;

    enum recharging_types rtype = objects[obj->otyp].oc_recharging;

    if (rtype == RECHARGING_GENERAL)
    {
        if (obj->recharged >= RECHARGE_LIMIT) /* recharge_limit */
            goto not_chargable;

        /* All other charged items here */
        int lim = get_obj_max_charge(obj);

        if (is_cursed)
        {
            strip_charges(obj, verbose);
            update_inventory();
        }
        else if (obj->charges >= lim)
        {
            if (verbose)
                play_sfx_sound(SFX_CHARGES_AT_MAXIMUM);
            obj->charges = lim;
            update_inventory();
        }
        else if (obj->charges < lim)
        {
            obj->recharged++;

            if (is_blessed)
            {
                obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            else if (obj->charges < lim)
            {
                obj->charges++;
                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            update_inventory();
        }
        else
        {
            goto not_chargable;
        }
    }
    else if (rtype == RECHARGING_WAND_GENERAL || rtype == RECHARGING_WAND_WISHING)
    {
        int lim = get_obj_max_charge(obj);
        
        /* undo any prior cancellation, even when is_cursed */
        if (obj->charges == -1)
            obj->charges = 0;

        /*
         * Recharging might cause wands to explode.
         *      v = number of previous recharges
         *            v = percentage chance to explode on this attempt
         *                    v = cumulative odds for exploding
         *      0 :   0       0
         *      1 :   0.29    0.29
         *      2 :   2.33    2.62
         *      3 :   7.87   10.28
         *      4 :  18.66   27.02
         *      5 :  36.44   53.62
         *      6 :  62.97   82.83
         *      7 : 100     100
         */
        n = (int) obj->recharged;
        if (n > 0 && (rtype == RECHARGING_WAND_WISHING
                      || (n * n * n > rn2(7 * 7 * 7))))
        { /* recharge_limit */
            wand_explode(obj, rnd(lim));
            return;
        }
        /* didn't explode, so increment the recharge count */
        obj->recharged = (unsigned) (n + 1);

        /* now handle the actual recharging */
        if (is_cursed) 
        {
            strip_charges(obj, verbose);
        } 
        else 
        {
            n = (lim == 3) ? 3 : rn1(lim / 2, lim + 1 - (lim / 2));
            if (!is_blessed)
                n = rnd(n);

            if (obj->charges < n)
                obj->charges = n;
            else
                obj->charges++;

            if (rtype == RECHARGING_WAND_WISHING && obj->charges > lim)
            {
                wand_explode(obj, 1);
                return;
            }
            if (verbose)
            {
                if(is_blessed)
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                else
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);

                if (obj->charges >= lim)
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                else
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);

                play_effect = TRUE;
            }
#if 0 /*[shop price doesn't vary by charge count]*/
            /* update shop bill to reflect new higher price */
            if (obj->unpaid)
                alter_cost(obj, 0L);
#endif
        }

    }
    else if (objects[obj->otyp].oc_charged && rtype > 0)  // obj->oclass == WEAPON_CLASS)
    {
        int rechrg = (int)obj->recharged;

        if (obj->oclass != TOOL_CLASS)
        {
            /* tools don't have a limit, but the counter used does */
            if (rechrg >= RECHARGE_LIMIT) /* recharge_limit */
                goto not_chargable;
        }

        /* All other charged items here */
        int lim = get_obj_max_charge(obj);

        switch (rtype)
        {
        case RECHARGING_BELL_OF_OPENING:
            if (is_cursed)
                strip_charges(obj, verbose);
            else if (is_blessed)
            {
                if (verbose)
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                obj->charges += rnd(3);
                play_effect = TRUE;
            }
            else
            {
                if (verbose)
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                obj->charges += 1;
                play_effect = TRUE;
            }

            if (obj->charges > 5)
                obj->charges = 5;
            break;
        case RECHARGING_MAGIC_MARKER:
        case RECHARGING_TOOL_NONMAGICAL:
        case RECHARGING_TOOL_GENERAL:
        case RECHARGING_CUBIC_GATE:
        case RECHARGING_CAMERA:
        {
            if (is_cursed)
                strip_charges(obj, verbose);
            else if (rechrg
                && obj->otyp
                == MAGIC_MARKER) { /* previously recharged */
                obj->recharged = 1; /* override increment done above */
                if (verbose)
                {
                    if (obj->charges < 3)
                        Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "marker seems permanently dried out.");
                    else
                        pline1(nothing_happens);
                }
            }
            else if (is_blessed)
            {

                n = rn1(16, 15); /* 15..30 */
                if (obj->charges + n <= (lim * 1) / 2)
                    obj->charges = (lim * 1) / 2;
                else if (obj->charges + n <= (lim * 3) / 4)
                    obj->charges = (lim * 3) / 4;
                else
                {
                    int chrg = (int)obj->charges;
                    if ((chrg + n) > lim)
                        obj->charges = lim;
                    else
                        obj->charges += n;
                }
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            else
            {
                n = rn1(11, 10); /* 10..20 */
                if (obj->charges + n <= (lim * 1) / 2)
                    obj->charges = (lim * 1) / 2;
                else
                {
                    int chrg = (int)obj->charges;
                    if ((chrg + n) > lim)
                        obj->charges = lim;
                    else
                        obj->charges += n;
                }
                if (verbose)
                {
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_WHITE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            break;
        }
#if 0
        /* Charging does not add oil anymore, just charges of charged objects */
        case OIL_LAMP:
        case BRASS_LANTERN:
            if (is_cursed) {
                strip_charges(obj, verbose);
                if (obj->lamplit) {
                    if (!Blind)
                        pline("%s out!", Tobjnam(obj, "go"));
                    end_burn(obj, TRUE);
            }
        }
            else if (is_blessed) {
                obj->special_quality = 1;
                obj->age = 1500;
                if (verbose)
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
            }
            else {
                obj->special_quality = 1;
                obj->age += 750;
                if (obj->age > 1500)
                    obj->age = 1500;
                if (verbose)
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
            }
            break;
#endif
        case RECHARGING_CRYSTAL_BALL:
        {
            if (is_cursed)
            {
                strip_charges(obj, verbose);
            }
            else if (is_blessed)
            {
                obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            else
            {
                if (obj->charges < lim - 1)
                {
                    obj->charges++;
                    if (verbose)
                    {
                        play_sfx_sound(SFX_RECHARGE_SUCCESS);
                        p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                        play_effect = TRUE;
                    }
                }
                else
                    pline1(nothing_happens);
            }
            break;
        }
        case RECHARGING_TOOL_SPECIAL_MAGICAL:
        {
            if (is_cursed)
            {
                strip_charges(obj, verbose);
            }
            else if (is_blessed)
            {
                if (obj->charges <= lim / 4)
                    obj->charges += rn1(10, 6);
                else
                    obj->charges += rn1(5, 6);
                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            else
            {
                obj->charges += rnd(5);
                if (obj->charges > 50)
                    obj->charges = 50;
                if (verbose)
                {
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            break;
        }
        case RECHARGING_CAN_OF_GREASE:
        case RECHARGING_SALVE:
        {
            const char* contents = (OBJ_CONTENT_DESC(obj->otyp) != 0 ? OBJ_CONTENT_DESC(obj->otyp) : "unknown contents"); // (objects[obj->otyp].oc_name_known&& OBJ_CONTENT_NAME(obj->otyp) != 0 ? OBJ_CONTENT_NAME(obj->otyp) : OBJ_CONTENT_DESC(obj->otyp) != 0 ? OBJ_CONTENT_DESC(obj->otyp) : "unknown contents");
            if (is_cursed)
            {
                strip_charges(obj, verbose);
            }
            else if (is_blessed)
            {
                obj->charges += rn1(max(2, (3 * lim) / 4), max(1, (2 * lim) / 4));

                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s%s with %s.", Tobjnam(obj, "fill"), obj->charges >= lim ? " up" : "", contents);
                    play_effect = TRUE;
                }
            }
            else
            {
                obj->charges += rn1(max(2, (2 * lim) / 4), max(1, (1 * lim) / 8));

                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s%s with %s.", Tobjnam(obj, "fill"), obj->charges >= lim ? " up" : "", contents);
                    play_effect = TRUE;
                }
            }
            break;
        }
        case RECHARGING_HOLY_SYMBOL:
        {
            if (is_cursed)
            {
                strip_charges(obj, verbose);
            }
            else if (is_blessed)
            {
                obj->charges += rn1(10, 10);
                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            else
            {
                obj->charges += rnd(5) + 4;
                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            break;
        }
        case RECHARGING_MUSICAL_INSTRUMENT:
        case RECHARGING_UNICORN_HORN:
        {
            if (is_cursed)
            {
                strip_charges(obj, verbose);
            }
            else if (is_blessed)
            {
                obj->charges += d(2, 4);
                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            else
            {
                obj->charges += rnd(4);
                if (obj->charges > lim)
                    obj->charges = lim;
                if (verbose)
                {
                    play_sfx_sound(SFX_RECHARGE_SUCCESS);
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                    play_effect = TRUE;
                }
            }
            break;
        }
        case RECHARGING_NINE_LIVES_STEALER:
            if (is_cursed) 
            {
                strip_charges(obj, verbose);
            }
            else if (obj->recharged >= 5 || !rn2(max(2, 6 - obj->recharged)))
            {
                const char* expltext = !obj->charges ? "suddenly" : "vibrates violently and";
                int dmg = d(3, 9);
                obj->in_use = TRUE; /* in case losehp() is fatal (or --More--^C) */
                if (verbose)
                {
                    play_sfx_sound(SFX_EXPLOSION_MAGICAL);
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s explodes!", Yname2(obj), expltext);
                }
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_MAGM, ADFLAGS_NONE), "exploding sword", KILLED_BY_AN);
                useup(obj);
            }
            else if (obj->charges < lim)
            {
                if (is_blessed)
                {
                    obj->charges = lim;
                    if (verbose)
                    {
                        play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                        p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                        play_effect = TRUE;
                    }
                }
                else if (obj->charges < lim)
                {
                    obj->charges += 1 + rnd(3);
                    if (obj->charges > lim)
                        obj->charges = lim;
                    if (verbose)
                    {
                        play_sfx_sound(SFX_RECHARGE_SUCCESS);
                        p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                        play_effect = TRUE;
                    }
                }
            }
            else
            {
                goto not_chargable;
            }
            break;
        case RECHARGING_RING_OF_THREE_WISHES:
        case RECHARGING_LUCK_BLADE:
            /* Unchargeable */
            if (is_cursed)
            {
                strip_charges(obj, verbose);
                break;
            }
            else
            {
                goto not_chargable;
            }
            break;
        case RECHARGING_RING_OF_CONFLICT:
        case RECHARGING_WAND_OF_ORCUS:
        {
            if (is_cursed)
            {
                strip_charges(obj, verbose);
                break;
            }

            if (obj->charges > lim)
            {
                if(verbose)
                    play_sfx_sound(SFX_CHARGES_AT_MAXIMUM);
                obj->charges = lim;
                break;
            }
            else if (obj->charges < lim)
            {
                if (is_blessed)
                {
                    obj->charges = lim;
                    if (verbose)
                    {
                        play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                        p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE);
                        play_effect = TRUE;
                    }
                }
                else if (obj->charges < lim)
                {
                    obj->charges += 2 + rnd(4);
                    if (obj->charges > lim)
                        obj->charges = lim;
                    if (verbose)
                    {
                        play_sfx_sound(SFX_RECHARGE_SUCCESS);
                        p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE);
                        play_effect = TRUE;
                    }
                }
            }
            else
            {
                goto not_chargable;
            }

            break;
        }
        case RECHARGING_HOLY_GRAIL:
        {
            const char* contents = (objects[obj->otyp].oc_name_known && OBJ_CONTENT_NAME(obj->otyp) != 0 ? OBJ_CONTENT_NAME(obj->otyp) : OBJ_CONTENT_DESC(obj->otyp) != 0 ? OBJ_CONTENT_DESC(obj->otyp) : "unknown contents");
            if (obj->recharged >= 5)
            {
                goto not_chargable;
                break;
            }

            if (is_cursed)
            {
                strip_charges(obj, verbose);
                break;
            }

            if (obj->charges >= lim)
            {
                goto not_chargable;
                break;
            }
            else
            {
                if (is_blessed)
                {
                    obj->charges = lim;
                }
                else if (obj->charges < lim)
                {
                    obj->charges += rnd(2);
                    if (obj->charges > lim)
                        obj->charges = lim;
                }

                if (verbose)
                {
                    if (is_blessed)
                        play_sfx_sound(SFX_BLESSED_RECHARGE_SUCCESS);
                    else
                        play_sfx_sound(SFX_RECHARGE_SUCCESS);

                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s%s with %s.", Tobjnam(obj, "fill"), obj->charges >= lim ? " up" : "", contents);
                    play_effect = TRUE;
                }
                update_inventory();
            }
            break;
        }
        default:
            goto not_chargable;
            break;
        }

        /* items don't have a limit, but the counter used does */
        if (obj->recharged < RECHARGE_LIMIT) /* recharge_limit */
            obj->recharged++;

        update_inventory();
    }
    else
    {
    not_chargable:
        if (verbose)
        {
            play_sfx_sound(SFX_RECHARGE_FAIL);
            You("have a feeling of loss.");
        }
    }

    if (play_effect)
    {
        if (obj->where == OBJ_INVENT)
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_action(0);
            special_effect_wait_until_end(0);
        }
    }
}

void
enchant_ring(obj, curse_bless)
struct obj* obj;
int curse_bless;
{
    boolean is_cursed, is_blessed;

    is_cursed = curse_bless < 0;
    is_blessed = curse_bless > 0;

    if (obj && (obj->oclass == RING_CLASS || obj->oclass == MISCELLANEOUS_CLASS) && objects[obj->otyp].oc_enchantable)
    {
        /* enchantment does not affect ring's curse/bless status */
        int maxcharge = get_obj_max_enchantment(obj);
        int s = is_blessed ? rnd(max(1, maxcharge / 3)) : is_cursed ? -rnd(max(1, maxcharge / 3)) : maxcharge >= 12 ? rnd(max(1, maxcharge / 6)) : 1;
        boolean is_on = (obj == uleft || obj == uright);

        /* destruction depends on current state, not adjustment */
        if ((obj->enchantment > maxcharge || obj->enchantment < -maxcharge) && rn2(3))
        {
            play_sfx_sound(SFX_ENCHANT_ITEM_VIBRATE_AND_DESTROY);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s momentarily, then %s!", Yobjnam2(obj, "pulsate"),
                otense(obj, "explode"));
            if (is_on)
                Ring_gone(obj);
            s = rnd(3 * abs(obj->enchantment)); /* amount of damage */
            useup(obj);
            losehp(adjust_damage(s, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "exploding ring", KILLED_BY_AN);
        }
        else
        {
            //long mask = is_on ? (obj == uleft ? LEFT_RING : RIGHT_RING) : 0L;

            if (s == 0)
                play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
            else if(s > 0)
                play_sfx_sound(SFX_ENCHANT_ITEM_SUCCESS);
            else
                play_sfx_sound(SFX_ENCHANT_ITEM_NEGATIVE);

            pline("%s spins %sclockwise for a moment.", Yname2(obj),
                s < 0 ? "counter" : "");
            if (s < 0)
                costly_alteration(obj, COST_DECHNT);
            /* cause attributes and/or properties to be updated */
//            if (is_on)
//                Ring_off(obj);
            obj->enchantment += s; /* update the ring while it's off */
//            if (is_on)
//                setworn(obj, mask), Ring_on(obj);
            /* oartifact: if a touch-sensitive artifact ring is
               ever created the above will need to be revised  */
               /* update shop bill to reflect new higher price */
            if (s > 0 && obj->unpaid)
                alter_cost(obj, 0L);

            if (obj->enchantment > maxcharge)
            {
                play_sfx_sound(SFX_ENCHANT_ITEM_VIBRATE_WARNING);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s unexpectedly.", Yobjnam2(obj, "suddenly vibrate"));
            }

            update_all_character_properties(obj, TRUE);
            update_inventory();
        }
    }
    else
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        You("have a feeling of loss.");
    }

}

/* Forget known information about this object type. */
STATIC_OVL void
forget_single_object(obj_id)
int obj_id;
{
    objects[obj_id].oc_name_known = 0;
    objects[obj_id].oc_pre_discovered = 0; /* a discovery when relearned */
    if (objects[obj_id].oc_uname) {
        free((genericptr_t) objects[obj_id].oc_uname);
        objects[obj_id].oc_uname = 0;
    }
    undiscover_object(obj_id); /* after clearing oc_name_known */

    /* clear & free object names from matching inventory items too? */
}

#if 0 /* here if anyone wants it.... */
/* Forget everything known about a particular object class. */
STATIC_OVL void
forget_objclass(oclass)
int oclass;
{
    int i;

    for (i = bases[oclass];
         i < NUM_OBJECTS && objects[i].oc_class == oclass; i++)
        forget_single_object(i);
}
#endif

/* randomize the given list of numbers  0 <= i < count */
STATIC_OVL void
randomize(indices, count)
int *indices;
int count;
{
    int i, iswap, temp;

    for (i = count - 1; i > 0; i--) {
        if ((iswap = rn2(i + 1)) == i)
            continue;
        temp = indices[i];
        indices[i] = indices[iswap];
        indices[iswap] = temp;
    }
}

/* Forget % of known objects. */
void
forget_objects(percent)
int percent;
{
    int i, count;
    int indices[NUM_OBJECTS];

    if (percent == 0)
        return;
    if (percent <= 0 || percent > 100) {
        impossible("forget_objects: bad percent %d", percent);
        return;
    }

    indices[0] = 0; /* lint suppression */
    for (count = 0, i = 1; i < NUM_OBJECTS; i++)
        if (OBJ_DESCR(objects[i])
            && (objects[i].oc_name_known || objects[i].oc_uname))
            indices[count++] = i;

    if (count > 0) {
        randomize(indices, count);

        /* forget first % of randomized indices */
        count = ((count * percent) + rn2(100)) / 100;
        for (i = 0; i < count; i++)
            forget_single_object(indices[i]);
    }
}

/* Forget some or all of map (depends on parameters). */
void
forget_map(howmuch)
int howmuch;
{
    register int zx, zy;

    if (Sokoban)
        return;

    known = TRUE;
    for (zx = 0; zx < COLNO; zx++)
        for (zy = 0; zy < ROWNO; zy++)
            if (howmuch & ALL_MAP || rn2(7)) {
                /* Zonk all memory of this location. */
                levl[zx][zy].seenv = 0;
                clear_hero_memory_at(zx, zy);
                lastseentyp[zx][zy] = UNDEFINED_LOCATION;
            }
    /* forget overview data for this level */
    forget_mapseen(ledger_no(&u.uz));
}

/* Forget all traps on the level. */
void
forget_traps()
{
    register struct trap *trap;

    /* forget all traps (except the one the hero is in :-) */
    for (trap = ftrap; trap; trap = trap->ntrap)
        if ((trap->tx != u.ux || trap->ty != u.uy) && !(trap_type_definitions[trap->ttyp].tdflags & TRAPDEF_FLAGS_VISIBLE_AT_START))
            trap->tseen = 0;
}

/*
 * Forget given % of all levels that the hero has visited and not forgotten,
 * except this one.
 */
void
forget_levels(percent)
int percent;
{
    int i, count;
    xchar maxl, this_lev;
    int indices[MAXLINFO];

    if (percent == 0)
        return;

    if (percent <= 0 || percent > 100) {
        impossible("forget_levels: bad percent %d", percent);
        return;
    }

    this_lev = ledger_no(&u.uz);
    maxl = maxledgerno();

    /* count & save indices of non-forgotten visited levels */
    /* Sokoban levels are pre-mapped for the player, and should stay
     * so, or they become nearly impossible to solve.  But try to
     * shift the forgetting elsewhere by fiddling with percent
     * instead of forgetting fewer levels.
     */
    indices[0] = 0; /* lint suppression */
    for (count = 0, i = 0; i <= maxl; i++)
        if ((level_info[i].flags & VISITED)
            && !(level_info[i].flags & FORGOTTEN) && i != this_lev) {
            if (ledger_to_dnum(i) == sokoban_dnum)
                percent += 2;
            else
                indices[count++] = i;
        }

    if (percent > 100)
        percent = 100;

    if (count > 0) {
        randomize(indices, count);

        /* forget first % of randomized indices */
        count = ((count * percent) + 50) / 100;
        for (i = 0; i < count; i++) {
            level_info[indices[i]].flags |= FORGOTTEN;
            forget_mapseen(indices[i]);
        }
    }
}

/*
 * Forget some things (e.g. after reading a scroll of amnesia).  When called,
 * the following are always forgotten:
 *      - felt ball & chain
 *      - traps
 *      - part (6 out of 7) of the map
 *
 * Other things are subject to flags:
 *      howmuch & ALL_MAP       = forget whole map
 *      howmuch & ALL_SPELLS    = forget all spells
 */
STATIC_OVL void
forget(howmuch)
int howmuch;
{
    if (Punished)
        u.bc_felt = 0; /* forget felt ball&chain */

    forget_map(howmuch);
    forget_traps();

    /* 1 in 3 chance of forgetting some levels */
    if (!rn2(3))
        forget_levels(rn2(25));

    /* 1 in 3 chance of forgetting some objects */
    if (!rn2(3))
        forget_objects(rn2(25));

    if (howmuch & ALL_SPELLS)
        losespells();
    /*
     * Make sure that what was seen is restored correctly.  To do this,
     * we need to go blind for an instant --- turn off the display,
     * then restart it.  All this work is needed to correctly handle
     * walls which are stone on one side and wall on the other.  Turning
     * off the seen bits above will make the wall revert to stone,  but
     * there are cases where we don't want this to happen.  The easiest
     * thing to do is to run it through the vision system again, which
     * is always correct.
     */
    docrt(); /* this correctly will reset vision */
}


/* monster is hit by scroll of taming's effect */
int
maybe_tame(mtmp, sobj, origmonst)
struct monst *mtmp;
struct obj *sobj;
struct monst* origmonst;
{
    boolean was_tame = is_tame(mtmp);
    boolean was_peaceful = is_peaceful(mtmp);
    int adj_save = get_saving_throw_adjustment(sobj, origmonst);

    if (sobj->cursed) 
    {
        setmangry(mtmp, FALSE);
        if (was_peaceful && !is_peaceful(mtmp))
            return -1;
    }
    else 
    {
        if (mtmp->isshk)
            make_happy_shk(mtmp, FALSE);
        else if (is_tame(mtmp) && !is_charmed(mtmp))
        {
            /* already as tame as it gets */
            return 0;
        }
        else if (resists_charm(mtmp))
        {
            play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
            pline("%s is unaffected.", Monnam(mtmp));
        }
        else if (!check_ability_resistance_success(mtmp, A_WIS, adj_save))
        {
            int duration = 0;
            int charmed = 0;
            if (sobj && !(objects[sobj->otyp].oc_spell_flags & S1_SPELL_IS_NONREVERSIBLE_PERMANENT))
            {
                int existing_charmed_duration = get_mon_property(mtmp, CHARMED);
                duration += existing_charmed_duration;
                duration += d(objects[sobj->otyp].oc_spell_dur_dice, objects[sobj->otyp].oc_spell_dur_diesize) + objects[sobj->otyp].oc_spell_dur_plus;
                charmed = 1;
            }

            /* tame dog verbosely */
            if (!tamedog(mtmp, (struct obj*) 0, TAMEDOG_NO_FORCED_TAMING, charmed, duration, TRUE, FALSE) || !is_tame(mtmp))
            {
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                pline("%s is unaffected!", Monnam(mtmp));
            }
            else
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        }
        else
        {
            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline("%s resists!", Monnam(mtmp));
        }
        if ((!was_peaceful && is_peaceful(mtmp)) || (!was_tame && is_tame(mtmp)))
            return 1;
    }
    return 0;
}

/* Control Undead */
int
maybe_controlled(mtmp, sobj, origmonst)
struct monst* mtmp;
struct obj* sobj;
struct monst* origmonst;
{
    if (!mtmp || !sobj)
        return 0;

    boolean was_tame = is_tame(mtmp);
    boolean was_peaceful = is_peaceful(mtmp);

    int save_adj = get_saving_throw_adjustment(sobj, origmonst);

    if(is_undead(mtmp->data) || is_vampshifter(mtmp))
    {
        if (sobj->cursed)
        {
            setmangry(mtmp, FALSE);
            if (was_peaceful && !is_peaceful(mtmp))
                return -1;
        }
        else if (mtmp->isshk)
            make_happy_shk(mtmp, FALSE);
        else if (!check_ability_resistance_success(mtmp, A_WIS, save_adj))
        {
            int duration = 0;
            int controlled = 0;
            if (sobj && !(objects[sobj->otyp].oc_spell_flags & S1_SPELL_IS_NONREVERSIBLE_PERMANENT))
            {
                int existing_control_duration = get_mon_property(mtmp, UNDEAD_CONTROL);
                duration += existing_control_duration;
                duration += d(objects[sobj->otyp].oc_spell_dur_dice, objects[sobj->otyp].oc_spell_dur_diesize) + objects[sobj->otyp].oc_spell_dur_plus;
                controlled = 2;
            }

            /* tame dog verbosely */
            if (!tamedog(mtmp, (struct obj*)0, TAMEDOG_NO_FORCED_TAMING, controlled, duration, TRUE, FALSE) || !is_tame(mtmp))
            {
                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
                pline("%s is unaffected!", Monnam(mtmp));
            }
        }
        else
        {
            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
            m_shieldeff(mtmp);
            pline("%s resists!", Monnam(mtmp));
        }
        if ((!was_peaceful && is_peaceful(mtmp)) || (!was_tame && is_tame(mtmp)))
            return 1;
    }
    else
    {
        play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mtmp->mx, mtmp->my);
        pline("%s is unaffected!", Monnam(mtmp));
    }
    return 0;
}

boolean
get_valid_targeted_position(x, y, otyp)
int x, y, otyp;
{
    return (!(!isok(x, y) || !cansee(x, y)
        || !ACCESSIBLE(levl[x][y].typ)
        || distu(x, y) > objects[otyp].oc_spell_range * objects[otyp].oc_spell_range));
}

boolean
get_valid_stinking_cloud_pos(x,y)
int x,y;
{
    return (!(!isok(x,y) || !cansee(x, y)
              || !ACCESSIBLE(levl[x][y].typ)
              || distu(x, y) >= 32));
}

boolean
is_valid_stinking_cloud_pos(x, y, showmsg)
int x, y;
boolean showmsg;
{
    if (!get_valid_stinking_cloud_pos(x,y)) {
        if (showmsg)
            You("smell rotten eggs.");
        return FALSE;
    }
    return TRUE;
}

void
display_stinking_cloud_positions(state)
int state;
{
    if (state == 0) {
        tmp_at(DISP_BEAM, cmap_to_glyph(S_goodpos));
    } else if (state == 1) {
        int x, y, dx, dy;
        int dist = 6;

        for (dx = -dist; dx <= dist; dx++)
            for (dy = -dist; dy <= dist; dy++) {
                x = u.ux + dx;
                y = u.uy + dy;
                if (get_valid_stinking_cloud_pos(x,y))
                    tmp_at(x, y);
            }
    } else {
        tmp_at(DISP_END, 0);
    }
}

/* scroll effects; return 1 if we use up the scroll and possibly make it
   become discovered, 0 if caller should take care of those side-effects */
int
seffects(sobj, effect_happened_ptr)
struct obj *sobj; /* scroll, or fake spellbook object for scroll-like spell */
boolean *effect_happened_ptr;
{
    int cval, otyp = sobj->otyp;
    boolean confused = (Confusion != 0), sblessed = sobj->blessed, sbcsign = bcsign(sobj),
            scursed = sobj->cursed, already_known, old_erodeproof,
            new_erodeproof;
    struct obj *otmp;
    char allowall[2];
    void FDECL((*func), (OBJ_P)) = 0;
    const char* glowcolor = 0;
#define COST_alter (-2)
#define COST_none (-1)
    int costchange = COST_none;
    boolean altfmt = FALSE;
    int duration = d(objects[otyp].oc_spell_dur_dice, objects[otyp].oc_spell_dur_diesize) + objects[otyp].oc_spell_dur_plus;
    boolean is_serviced_spell = !!(sobj->speflags & SPEFLAGS_SERVICED_SPELL);
    int save_adj = get_saving_throw_adjustment(sobj, sobj->oclass != SPBOOK_CLASS || is_serviced_spell ? (struct monst*)0 : &youmonst);
    if (objects[otyp].oc_magic)
        exercise(A_WIS, TRUE);                       /* just for trying */
    already_known = (sobj->oclass == SPBOOK_CLASS /* spell */
                     || objects[otyp].oc_name_known);

    switch (otyp) {
#ifdef MAIL
    case SCR_MAIL:
        known = TRUE;
        if (sobj->special_quality == 2)
            /* "stamped scroll" created via magic marker--without a stamp */
            pline("This scroll is marked \"postage due\".");
        else if (sobj->special_quality)
            /* scroll of mail obtained from bones file or from wishing;
             * note to the puzzled: the game Larn actually sends you junk
             * mail if you win!
             */
            pline(
                "This seems to be junk mail addressed to the finder of the Eye of Larn.");
        else
            readmail(sobj);
        break;
#endif
    case SPE_PROTECT_ARMOR:
    case SCR_PROTECT_ARMOR:
    case SPE_ENCHANT_ARMOR:
    case SCR_ENCHANT_ARMOR: 
    {
        register schar s;
        boolean same_color;

        const char enchant_armor_objects[] = { ALL_CLASSES, ARMOR_CLASS, 0 };

        if (otyp == SPE_PROTECT_ARMOR || otyp == SPE_ENCHANT_ARMOR)
        {
            otmp = getobj(enchant_armor_objects, otyp == SPE_ENCHANT_ARMOR ? "enchant" : "protect", 0, "");
            if (!otmp)
            {
                *effect_happened_ptr = 0;
                return 0;
            }

            if(otmp && otmp->oclass != ARMOR_CLASS)
            {
                play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, !Blind
                    ? "%s then fades."
                    : "%s warm for a moment.", Yobjnam2(otmp, !Blind ? "glow" : "feel"));
                sobj = 0;
                break;
            }
        }
        else
            otmp = some_armor(&youmonst);

    enchantarmor:
        if (!otmp) {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
            special_effect_wait_until_end(0);
            strange_feeling(sobj, !Blind
                ? "Your skin glows then fades."
                : "Your skin feels warm for a moment.");
            sobj = 0; /* useup() in strange_feeling() */
            exercise(A_CON, !scursed);
            exercise(A_STR, !scursed);
            special_effect_wait_until_end(0);
            break;
        }
        if ((!is_serviced_spell && confused) || otyp == SPE_PROTECT_ARMOR || otyp == SCR_PROTECT_ARMOR) {
            old_erodeproof = (otmp->oerodeproof != 0);
            new_erodeproof = !scursed;
            otmp->oerodeproof = 0; /* for messages */
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_PROTECT_ITEM_SUCCESS);
            special_effect_wait_until_action(0);
            if (Blind) {
                otmp->rknown = FALSE;
                pline("%s warm for a moment.", Yobjnam2(otmp, "feel"));
            }
            else {
                otmp->rknown = TRUE;
                if (!confused && !scursed && otyp == SCR_PROTECT_ARMOR)
                    known = TRUE;

                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s covered by a %s %s %s!", Yobjnam2(otmp, "are"),
                    scursed ? "mottled" : "shimmering",
                    hcolor(scursed ? NH_BLACK : NH_GOLDEN),
                    scursed ? "glow"
                    : (is_shield(otmp) ? "layer" : "shield"));
            }
            if (new_erodeproof && (otmp->oeroded || otmp->oeroded2)) {
                otmp->oeroded = otmp->oeroded2 = 0;
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s as good as new!",
                    Yobjnam2(otmp, Blind ? "feel" : "look"));
            }
            if (old_erodeproof && !new_erodeproof) {
                /* restore old_erodeproof before shop charges */
                otmp->oerodeproof = 1;
                costly_alteration(otmp, COST_DEGRD);
            }
            otmp->oerodeproof = new_erodeproof ? 1 : 0;
            special_effect_wait_until_end(0);
            update_inventory();
            break;
        }

        int max_ench = get_obj_max_enchantment(otmp);
        if (scursed)
            same_color = (otmp->otyp == BLACK_DRAGON_SCALE_MAIL
                || otmp->otyp == BLACK_DRAGON_SCALES);
        else
            same_color = (otmp->otyp == SILVER_DRAGON_SCALE_MAIL
                || otmp->otyp == SILVER_DRAGON_SCALES
                || otmp->otyp == SHIELD_OF_REFLECTION);
        if (Blind)
            same_color = FALSE;

        /* KMH -- catch underflow */
        s = scursed ? -otmp->enchantment : otmp->enchantment;
        if (s > max_ench && rn2(max(1, s)))
        {
            play_sfx_sound(SFX_ENCHANT_ITEM_VIBRATE_AND_DESTROY);
            otmp->in_use = TRUE;
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s violently %s%s%s for a while, then %s.", Yname2(otmp),
                otense(otmp, Blind ? "vibrate" : "glow"),
                (!Blind && !same_color) ? " " : "",
                (Blind || same_color) ? "" : hcolor(scursed ? NH_BLACK
                    : NH_SILVER),
                otense(otmp, "evaporate"));
            remove_worn_item(otmp, FALSE);
            useup(otmp);
            break;
        }

        s = scursed ? -1
            : (otmp->enchantment >= 9)
            ? (rn2(max(1, otmp->enchantment)) == 0)
            : sblessed ? rnd(2) : 1;

        if (s >= 0 && is_dragon_scales(otmp)) 
        {
            dragon_scales_to_scale_mail(otmp, sblessed);
            break;
        }

        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);

        if(s == 0)
            play_sfx_sound(SFX_ENCHANT_ITEM_VIOLENT_GLOW);
        else if(s < 0)
            play_sfx_sound(SFX_ENCHANT_ITEM_NEGATIVE);
        else if(s > 1)
            play_sfx_sound(SFX_ENCHANT_ITEM_BLESSED_SUCCESS);
        else
            play_sfx_sound(SFX_ENCHANT_ITEM_SUCCESS);

        special_effect_wait_until_action(0);

        pline_ex(ATR_NONE, scursed ? CLR_MSG_NEGATIVE : CLR_MSG_POSITIVE, "%s %s%s%s%s for a %s.", Yname2(otmp),
            s == 0 ? "violently " : "",
            otense(otmp, Blind ? "vibrate" : "glow"),
            (!Blind && !same_color) ? " " : "",
            (Blind || same_color)
            ? "" : hcolor(scursed ? NH_BLACK : NH_SILVER),
            (s * s > 1) ? "while" : "moment");

        /* [this cost handling will need updating if shop pricing is
           ever changed to care about curse/bless status of armor] */
        if (s < 0)
            costly_alteration(otmp, COST_DECHNT);
        if (scursed && !otmp->cursed)
            curse(otmp);
        else if (sblessed && !otmp->blessed)
            bless(otmp);
        else if (!scursed && otmp->cursed)
            uncurse(otmp);
        
        if (s) 
        {
            otmp->enchantment += s;
            //adj_abon(otmp, s);
            updateabon();
            updatemaxen();
            updatemaxhp();
            known = otmp->known;
            /* update shop bill to reflect new higher price */
            if (s > 0 && otmp->unpaid)
                alter_cost(otmp, 0L);
        }

        if (otmp->enchantment > max_ench)
        {
            play_sfx_sound(SFX_ENCHANT_ITEM_VIBRATE_WARNING);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s %s.", Yobjnam2(otmp, "suddenly vibrate"),
                Blind ? "again" : "unexpectedly");
        }

        special_effect_wait_until_end(0);
        update_inventory();
        break;
    }
    case SCR_DESTROY_ARMOR:
    {
        otmp = some_armor(&youmonst);
        if (confused)
        {
            if (!otmp)
            {
                play_sfx_sound(SFX_HANDS_ITCH);
                strange_feeling(sobj, "Your bones itch.");
                sobj = 0; /* useup() in strange_feeling() */
                exercise(A_STR, FALSE);
                exercise(A_CON, FALSE);
                break;
            }
            old_erodeproof = (otmp->oerodeproof != 0);
            new_erodeproof = scursed;
            otmp->oerodeproof = 0; /* for messages */
            p_glow2(otmp, NH_PURPLE, ATR_NONE, CLR_MSG_POSITIVE);
            if (old_erodeproof && !new_erodeproof) {
                /* restore old_erodeproof before shop charges */
                otmp->oerodeproof = 1;
                costly_alteration(otmp, COST_DEGRD);
            }
            otmp->oerodeproof = new_erodeproof ? 1 : 0;
            update_inventory();
            break;
        }
        if (uarmc && uarmc->otyp == CLOAK_OF_INTEGRITY)
        {
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s the destructive energies of the scroll.", Yobjnam2(uarmc, "absorb"));
            makeknown(uarmc->otyp);
            known = TRUE;
            otmp = uarmc;
            goto enchantarmor;
        }
        else
        {
            if (!scursed || !otmp || !otmp->cursed) {
                if (!destroy_arm(otmp)) {
                    strange_feeling(sobj, "Your skin itches.");
                    sobj = 0; /* useup() in strange_feeling() */
                    exercise(A_STR, FALSE);
                    exercise(A_CON, FALSE);
                    break;
                }
                else
                    known = TRUE;
            }
            else { /* armor and scroll both cursed */
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s.", Yobjnam2(otmp, "vibrate"));
                if (otmp->enchantment >= -6) {
                    otmp->enchantment += -1;
                    //adj_abon(otmp, -1);
                    updateabon();
                    updatemaxen();
                    updatemaxhp();
                }
                make_stunned((HStun & TIMEOUT) + (long)rn1(10, 10), TRUE);
            }
        }
    }   break;
    case SCR_CONFUSE_MONSTER:
    case SPE_CONFUSE_MONSTER:
        if (youmonst.data->mlet != S_HUMAN || scursed) 
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            if (!HConfusion)
            {
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "confused.");
            }
            special_effect_wait_until_action(0);
            make_confused(itimeout_incr(HConfusion, rnd(100)), FALSE);
            special_effect_wait_until_end(0);
        }
        else if (confused) 
        {
            if (!sblessed) 
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
                special_effect_wait_until_action(0);
                Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s begin to %s%s.", makeplural(body_part(HAND)),
                    Blind ? "tingle" : "glow ",
                    Blind ? "" : hcolor(NH_PURPLE));
                make_confused(itimeout_incr(HConfusion, rnd(100)), FALSE);
                special_effect_wait_until_end(0);
            }
            else 
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A %s%s surrounds your %s.",
                    Blind ? "" : hcolor(NH_RED),
                    Blind ? "faint buzz" : " glow", body_part(HEAD));
                make_confused(0L, TRUE);
                special_effect_wait_until_end(0);
            }
        }
        else
        {
            if (!sblessed) 
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%s %s%s.", makeplural(body_part(HAND)),
                    Blind ? "" : " begin to glow",
                    Blind ? (const char*) "tingle" : hcolor(NH_RED),
                    u.umconf ? " even more" : "");
                u.umconf++;
                special_effect_wait_until_end(0);
            }
            else 
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);
                if (Blind)
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s tingle %s sharply.", makeplural(body_part(HAND)),
                        u.umconf ? "even more" : "very");
                else
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s glow a%s brilliant %s.",
                        makeplural(body_part(HAND)),
                        u.umconf ? "n even more" : "", hcolor(NH_RED));
                /* after a while, repeated uses become less effective */
                if (u.umconf >= 40)
                    u.umconf++;
                else
                    u.umconf += rn1(8, 2);
                special_effect_wait_until_end(0);
            }
        }
        break;
    case SCR_SCARE_MONSTER:
    case SPE_MASS_FEAR: {
        register int ct = 0;
        register struct monst* mtmp;

        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
        {
            if (DEADMONSTER(mtmp))
                continue;
            if (resists_fear(mtmp) || check_ability_resistance_success(mtmp, A_WIS, save_adj))
                continue;

            if (cansee(mtmp->mx, mtmp->my)) 
            {
                if (confused || scursed) 
                {
                    mtmp->mflee = mtmp->mfrozen = mtmp->msleeping = 0;
                    mtmp->mcanmove = 1;
                    mtmp->mprops[FEARFUL] = 0;
                    refresh_m_tile_gui_info(mtmp, TRUE);
                }
                else
                {
                    duration = d(objects[otyp].oc_spell_dur_dice, objects[otyp].oc_spell_dur_diesize) + objects[otyp].oc_spell_dur_plus;
                    make_mon_fearful(mtmp, duration ? duration : 100 + rnd(50));
                }
                if (!is_tame(mtmp))
                    ct++; /* pets don't laugh at you */
            }
        }
        if (otyp == SCR_SCARE_MONSTER)
        {
            if(!(confused || scursed))
                play_sfx_sound(SFX_MANIACAL_LAUGHTER);

            You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "%s %s.", (confused || scursed) ? "sad wailing"
                : "maniacal laughter",
                !ct ? "in the distance" : "close by");
        }
        else if (!ct)
            pline("Nothing much seems to happen.");

        break;
    }
    case SCR_BLANK_PAPER:
        if (Blind)
            You("don't remember there being any magic words on this scroll.");
        else
            pline("This scroll seems to be blank.");
        known = TRUE;
        break;
    case SCR_REMOVE_CURSE:
    case SPE_REMOVE_CURSE: 
    {
        register struct obj* obj;

        You_feel_ex(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : !confused && !scursed ? CLR_MSG_POSITIVE : CLR_MSG_ATTENTION, !Hallucination
            ? (!confused ? "like someone is helping you."
                : "like you need some help.")
            : (!confused ? "in touch with the Universal Oneness."
                : "the power of the Force against you!"));

        if (scursed) {
            pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "scroll disintegrates.");
        }
        else
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_end(0);
            for (obj = invent; obj; obj = obj->nobj)
            {
                long wornmask;

                /* gold isn't subject to cursing and blessing */
                if (obj->oclass == COIN_CLASS)
                    continue;
                wornmask = (obj->owornmask & ~(W_BALL | W_ARTIFACT_CARRIED | W_ARTIFACT_INVOKED));
                if (wornmask && !sblessed)
                {
                    /* handle a couple of special cases; we don't
                       allow auxiliary weapon slots to be used to
                       artificially increase number of worn items */
                    if (obj == uswapwep || obj == uswapwep2) 
                    {
                        wornmask = 0L;
                    }
                    else if (obj == uquiver)
                    {
                        if (obj->oclass == WEAPON_CLASS) 
                        {
                            /* mergeable weapon test covers ammo,
                               missiles, spears, daggers & knives */
                            if (!objects[obj->otyp].oc_merge)
                                wornmask = 0L;
                        }
                        else if (obj->oclass == GEM_CLASS)
                        {
                            /* possibly ought to check whether
                               alternate weapon is a sling... */
                            if (!uslinging())
                                wornmask = 0L;
                        }
                        else 
                        {
                            /* weptools don't merge and aren't
                               reasonable quivered weapons */
                            wornmask = 0L;
                        }
                    }
                }

                if (sblessed || wornmask || objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED
                    || objects[obj->otyp].oc_flags & O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED
                    || (obj->otyp == LEASH && obj->leashmon)) 
                {
                    /* water price varies by curse/bless status */
                    boolean shop_h2o = (obj->unpaid && obj->otyp == POT_WATER);

                    if (confused) 
                    {
                        if (!(obj->blessed || obj->cursed))
                        {
                            if (!rn2(2)) 
                            {
                                if (!rn2(2))
                                {
                                    play_sfx_sound(SFX_CURSE_ITEM_SUCCESS);
                                    curse(obj);
                                }
                                else 
                                {
                                    play_sfx_sound(SFX_BLESS_ITEM_SUCCESS);
                                    bless(obj);
                                }
                            }
                        }
                        //blessorcurse(obj, 2);
                        /* lose knowledge of this object's curse/bless
                           state (even if it didn't actually change) */
                        obj->bknown = 0;
                        /* blessorcurse() only affects uncursed items
                           so no need to worry about price of water
                           going down (hence no costly_alteration) */
                        if (shop_h2o && (obj->cursed || obj->blessed))
                            alter_cost(obj, 0L); /* price goes up */
                    }
                    else if (obj->cursed) 
                    {
                        if (shop_h2o)
                            costly_alteration(obj, COST_UNCURS);
                        play_sfx_sound(SFX_UNCURSE_ITEM_SUCCESS);
                        uncurse(obj);
                    }
                }
            }
            special_effect_wait_until_end(0);
        }
        if (Punished && !confused)
        {
            play_sfx_sound(SFX_UNCURSE_ITEM_SUCCESS);
            unpunish();
        }
        if (u.utrap && u.utraptype == TT_BURIEDBALL) 
        {
            buried_ball_to_freedom();
            play_sfx_sound(SFX_ITEM_VANISHES);
            pline_The_ex(ATR_NONE, CLR_MSG_POSITIVE, "clasp on your %s vanishes.", body_part(LEG));
        }
        update_inventory();
        break;
    }
    case SCR_CREATE_MONSTER:
    case SPE_CREATE_MONSTER:
        if (create_critters(1 + ((confused || scursed) ? 12 : 0)
            + ((sblessed || rn2(73)) ? 0 : rnd(4)),
            confused ? &mons[PM_ACID_BLOB]
            : (struct permonst*) 0,
            TRUE))
            known = TRUE;
        /* no need to flush monsters; we ask for identification only if the
         * monsters are not visible
         */
        break;
    case SPE_PROTECT_WEAPON:
    case SCR_PROTECT_WEAPON:
    case SPE_ENCHANT_WEAPON:
    case SCR_ENCHANT_WEAPON:
    {
        const char enchant_weapon_objects[] = { ALL_CLASSES, WEAPON_CLASS, TOOL_CLASS, 0 };

        /* Allow object selection for spells */
        if (otyp == SPE_PROTECT_WEAPON || otyp == SPE_ENCHANT_WEAPON)
        {
            otmp = getobj(enchant_weapon_objects, otyp == SPE_ENCHANT_WEAPON ? "enchant" : "protect", 0, "");
            if (!otmp)
            {
                *effect_happened_ptr = 0;
                return 0;
            }

            /* Check if the selection is not an appropriate weapon */
            if (otmp && 
                (!is_weapon(otmp) 
                    || (otyp == SPE_PROTECT_WEAPON && !erosion_matters(otmp)) 
                    || (otyp == SPE_ENCHANT_WEAPON && objects[otmp->otyp].oc_enchantable == ENCHTYPE_NO_ENCHANTMENT)
                ))
            {
                play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, !Blind
                    ? "%s then fades."
                    : "%s warm for a moment.", Yobjnam2(otmp, !Blind ? "glow" : "feel"));
                sobj = 0;
                break;
            }
        }
        else
        {
            /* Select weapon randomly from left and right hands */
            if(uwep && is_weapon(uwep) && uwep->oclass != ARMOR_CLASS && (!uarms || (!is_weapon(uarms) || uarms->oclass == ARMOR_CLASS)))
                otmp = uwep;
            else if((!uwep || (!is_weapon(uwep) || uwep->oclass == ARMOR_CLASS)) && uarms && is_weapon(uarms) && uarms->oclass != ARMOR_CLASS)
                otmp = uarms;
            else if (uwep && is_weapon(uwep) && uwep->oclass != ARMOR_CLASS && uarms && is_weapon(uarms) && uarms->oclass != ARMOR_CLASS)
                otmp = !rn2(2) ? uwep : uarms;
            else if (uwep && is_weapon(uwep) && uarms && is_weapon(uarms))
                otmp = !rn2(2) ? uwep : uarms;
            else if (uwep && is_weapon(uwep))
                otmp = uwep;
            else if (uarms && is_weapon(uarms))
                otmp = uarms;
            else if (uarmg && is_weapon(uarmg))
                otmp = uarmg;
            else
                otmp = 0;

            if (!otmp)
            {
                play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
                Your("%s tingle for a moment.", makeplural(body_part(FINGER)));
                sobj = 0;
                break;
            }
        }

        /* Check for successful protect weapon */
        if (
            ((confused && !is_serviced_spell) || otyp == SPE_PROTECT_WEAPON || otyp == SCR_PROTECT_WEAPON) && otmp
            && erosion_matters(otmp) && is_weapon(otmp)
            )
        {
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_PROTECT_ITEM_SUCCESS);
            special_effect_wait_until_action(0);

            old_erodeproof = (otmp->oerodeproof != 0);
            new_erodeproof = !scursed;
            otmp->oerodeproof = 0; /* for messages */

            if (Blind)
            {
                otmp->rknown = FALSE;
                Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "weapon feels warm for a moment.");
            }
            else 
            {
                otmp->rknown = TRUE;
                if (!confused && !scursed && otyp == SCR_PROTECT_WEAPON)
                    known = TRUE;

                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s covered by a %s %s %s!", Yobjnam2(otmp, "are"),
                    scursed ? "mottled" : "shimmering",
                    hcolor(scursed ? NH_PURPLE : NH_GOLDEN),
                    scursed ? "glow" : "shield");
            }

            if (new_erodeproof && (otmp->oeroded || otmp->oeroded2)) 
            {
                otmp->oeroded = otmp->oeroded2 = 0;
                pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s as good as new!",
                    Yobjnam2(otmp, Blind ? "feel" : "look"));
            }

            if (old_erodeproof && !new_erodeproof)
            {
                /* restore old_erodeproof before shop charges */
                otmp->oerodeproof = 1;
                costly_alteration(otmp, COST_DEGRD);
            }
            otmp->oerodeproof = new_erodeproof ? 1 : 0;
            special_effect_wait_until_end(0);
            update_inventory();
            break;
        }
        
        /* Blanket selection for protect weapon */
        if (otyp == SPE_PROTECT_WEAPON || otyp == SCR_PROTECT_WEAPON)
        {
            if (otmp)
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "as if your weapon is warmer than normal, but then it passes.");
            else
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "experience a passing feeling of warmth.");

            break;
        }

        boolean isweptwohanded = (otmp && bimanual(otmp));
        int special_threshold = isweptwohanded ? 24 : 12;
        int special_chance = max(1, otmp->enchantment / (isweptwohanded ? 2 : 1));
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);

        /* Otherwise, enchant weapon */
        if (!enchant_weapon(sobj, otmp, scursed ? -1
            : !otmp ? 1
            : (otmp->enchantment >= special_threshold) ? !rn2(special_chance)
            : sblessed ? rnd(2): 1))
            sobj = 0; /* nothing enchanted: strange_feeling -> useup */
        
        special_effect_wait_until_end(0);
        break;
    }
    case SCR_CONFLICT:
        if (!Conflict)
        {
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "like a rabble-rouser.");
            known = TRUE;
        }
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        incr_itimeout(&HConflict, d(2, 10) + 140);
        refresh_u_tile_gui_info(TRUE);
        special_effect_wait_until_end(0);
        break;
    case SCR_TAMING:
    case SPE_SPHERE_OF_CHARMING:
    case SPE_SPHERE_OF_DOMINATION:
    case SPE_MASS_DOMINATION:
    case SPE_MASS_CHARM:
    {
        int candidates, res, results, vis_results;

        if (u.uswallow) {
            candidates = 1;
            results = vis_results = maybe_tame(u.ustuck, sobj, otyp != SCR_TAMING ? &youmonst : (struct monst*)0);
        }
        else {
            int i, j, bd = confused ? 5 : 1;
            struct monst* mtmp;
            coord cc = { u.ux, u.uy };

            if (otyp != SCR_TAMING)
                bd = (int)objects[otyp].oc_spell_radius;

            if (objects[otyp].oc_spell_range > 0)
            {
                pline("Where do you want to center the %s?", OBJ_NAME(objects[otyp]));

                int trycnt = 0;
                while (trycnt < 10)
                {
                    (void)getpos(&cc, TRUE, "the desired position", CURSOR_STYLE_SPELL_CURSOR);
                    if (!get_valid_targeted_position(cc.x, cc.y, otyp))
                    {
                        play_sfx_sound(SFX_GENERAL_NOT_AT_RIGHT_LOCATION);
                        pline("Not a valid target position.");
                        if (trycnt > 4)
                        {
                            cc.x = u.ux;
                            cc.y = u.uy;
                            break;
                        }
                    }
                    else
                        break;

                    trycnt++;
                }
            }


            if (objects[otyp].oc_dir == TARGETED)
            {
                u.dx = cc.x - u.ux;
                update_u_facing(TRUE);
                update_u_action(ACTION_TILE_CAST_NODIR);
                play_simple_monster_sound(&youmonst, MONSTER_SOUND_TYPE_CAST);
                u_wait_until_action();
                u.dx = 0;
            }

            /* note: maybe_tame() can return either positive or
               negative values, but not both for the same scroll */
            candidates = results = vis_results = 0;
            for (i = -bd; i <= bd; i++)
                for (j = -bd; j <= bd; j++)
                {
                    if (!isok(cc.x + i, cc.y + j))
                        continue;
                    if ((mtmp = m_at(cc.x + i, cc.y + j)) != 0 || (!i && !j && (mtmp = u.usteed) != 0))
                    {
                        ++candidates;
                        res = maybe_tame(mtmp, sobj, otyp != SCR_TAMING ? &youmonst : (struct monst*)0);
                        results += res;
                        if (canspotmon(mtmp))
                            vis_results += res;
                    }
                }
        }
        if (!results) 
        {
            pline("Nothing interesting %s.",
                !candidates ? "happens" : "seems to happen");
        }
        else
        {
            pline_The_ex(ATR_NONE, (results < 0) ? CLR_MSG_WARNING : CLR_MSG_POSITIVE, "neighborhood %s %sfriendlier.",
                vis_results ? "is" : "seems",
                (results < 0) ? "un" : "");
            if (vis_results > 0)
                known = TRUE;
        }
        break;
    }
    case SPE_MASS_SLEEP:
    {
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "Glittering dust starts to swirl around you...");

        int candidates, results, vis_results;
        int i, j, bd, res = 0;
        struct monst* mtmp;

        bd = (int)objects[otyp].oc_spell_radius;
        candidates = results = vis_results = 0;

        for (i = -bd; i <= bd; i++)
            for (j = -bd; j <= bd; j++)
            {
                if (!isok(u.ux + i, u.uy + j))
                    continue;
                if ((mtmp = m_at(u.ux + i, u.uy + j)) != 0
                    || (!i && !j && (mtmp = u.usteed) != 0))
                {
                    if(!is_tame(mtmp))
                    {
                        ++candidates;
                        res = 0;
                        if (sleep_monst(mtmp, sobj, &youmonst, duration, save_adj, TELL))
                        {
                            slept_monst(mtmp);
                            res = 1;
                            if (canspotmon(mtmp))
                            {
                                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s falls asleep!", Monnam(mtmp));
                                vis_results += res;
                                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                                special_effect_wait_until_action(0);
                                special_effect_wait_until_end(0);
                            }
                        }
                        results += res;
                    }
                }
            }
    
        if (!results) 
        {
            pline("Nothing interesting %s.",
                !candidates ? "happens" : "seems to happen");
        }
        else 
        {
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "neighborhood %s quieter.",
                vis_results ? "is" : "seems");
            if (vis_results > 0)
                known = TRUE;
        }
        break;
    }
    case SPE_HOLY_WORD:
    {
        You("recite the holy word aloud!");
        pline_ex(ATR_NONE, CLR_MSG_SPELL, "A blinding light blasts around you!");

        int candidates, results, vis_results;
        int i, j, bd, res = 0;
        struct monst* mtmp;

        bd = (int)objects[otyp].oc_spell_radius;
        candidates = results = vis_results = 0;

        for (i = -bd; i <= bd; i++)
        {
            for (j = -bd; j <= bd; j++)
            {
                if (!isok(u.ux + i, u.uy + j))
                    continue;
                if ((mtmp = m_at(u.ux + i, u.uy + j)) != 0
                    || (!i && !j && (mtmp = u.usteed) != 0))
                {
                    ++candidates;
                    res = 0;
                    if (!mtmp->mtame && !is_peaceful(mtmp) 
                        && !check_ability_resistance_success(&youmonst, A_CON, save_adj))
                    {
                        if (mtmp->m_lev < u.ulevel - 10)
                        {
                            if (!DEADMONSTER(mtmp) && !resists_death(mtmp))
                            {
                                mtmp->mhp = 0;
                                if (DEADMONSTER(mtmp))
                                {
                                    killed(mtmp);
                                }
                                res = 1;
                            }
                        }
                        if (res == 0 && mtmp->m_lev < u.ulevel - 5)
                        {
                            if (!mindless(mtmp->data) && !resists_paralysis(mtmp))
                            {
                                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                                special_effect_wait_until_action(0);
                                increase_mon_property_verbosely(mtmp, PARALYZED, duration);
                                special_effect_wait_until_end(0);
                                res = 1;
                            }
                        }
                        if(res == 0 && mtmp->m_lev < u.ulevel)
                        {
                            if(haseyes(mtmp->data) && !resists_blnd(mtmp))
                            {
                                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                                special_effect_wait_until_action(0);
                                increase_mon_property_verbosely(mtmp, BLINDED, duration);
                                special_effect_wait_until_end(0);
                                res = 1;
                            }
                        }
                        if (res == 0)
                        {
                            if (!mindless(mtmp->data) && !is_stunned(mtmp))
                            {
                                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, mtmp->mx, mtmp->my, FALSE);
                                special_effect_wait_until_action(0);
                                increase_mon_property_verbosely(mtmp, STUNNED, 20 + rnd(20));
                                special_effect_wait_until_end(0);
                                res = 1;
                            }
                        }
                    }

                    results += res;
                    if (canspotmon(mtmp))
                        vis_results += res;
                }
            }
        }

        if (!results)
        {
            pline("Nothing interesting %s.",
                !candidates ? "happens" : "seems to happen");
        }
        break;
    }
    case SCR_GENOCIDE:
        if (!already_known)
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have found a scroll of genocide!");
        known = TRUE;
        if (sblessed)
            do_class_genocide();
        else
            do_genocide((!scursed) | (2 * !!Confusion));
        break;
    case SCR_LIGHT:
        if (!confused || rn2(5)) {
            if (!Blind)
                known = TRUE;
            litroom(!confused && !scursed, sobj);
            if (!confused && !scursed) {
                if (lightdamage(sobj, TRUE, 5) > 0)
                    known = TRUE;
            }
        } else {
            /* could be scroll of create monster, don't set known ...*/
            (void) create_critters(1, !scursed ? &mons[PM_YELLOW_LIGHT]
                                               : &mons[PM_BLACK_LIGHT],
                                   TRUE);
        }
        break;
    case SCR_TELEPORTATION:
        if (confused || scursed) {
            level_tele(0, FALSE, zerodlevel);
        } else {
            known = scrolltele(sobj, FALSE, FALSE);
        }
        break;
    case SCR_GOLD_DETECTION:
        if ((confused || scursed) ? trap_detect(sobj) : gold_detect(sobj))
        {
            if(objects[SCR_GOLD_DETECTION].oc_name_known)
                play_simple_object_sound(sobj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
            sobj = 0; /* failure: strange_feeling() -> useup() */
        }
        break;
    case SCR_FOOD_DETECTION:
    case SPE_DETECT_FOOD:
        if (food_detect(sobj))
            sobj = 0; /* nothing detected: strange_feeling -> useup */
        break;
    case SCR_IDENTIFY:
        /* known = TRUE; -- handled inline here */
        /* use up the scroll first, before makeknown() performs a
           perm_invent update; also simplifies empty invent check */
        if (confused)
        {
            play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "identify this as an identify scroll.");
        }
        else if (!already_known || !invent)
        {
            /* force feedback now if invent became
               empty after using up this scroll */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "This is an identify scroll.");
        }

        if (!already_known)
            (void) learnscrolltyp(SCR_IDENTIFY);

        if (confused)
        {
            pline_ex(ATR_NONE, NO_COLOR, "The scroll disappears.");
            useup(sobj);
            sobj = 0; /* it's gone */
            break;
        }

        cval = sblessed ? 3 : scursed ? 1 : 2;
        if (invent)
        {
            int res = identify_pack(cval, !already_known);
            if (res > 0)
            {
                pline_ex(ATR_NONE, NO_COLOR, "The scroll disappears.");
                useup(sobj);
                sobj = 0; /* it's gone */
            }
        }
        else
        {
            pline("You're not carrying anything to be identified.");
        }
        break;
    case SPE_IDENTIFY:
        cval = 1;
        if (confused)
        {
            play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "identify this as a spell of identify.");
        }
        else if (invent)
        {
            (void)identify_pack(cval, !already_known);
        } 
        else
        {
            /* when casting a spell we know we're not confused,
               so inventory must be empty (another message has
               already been given above if reading a scroll) */
            pline("You're not carrying anything to be identified.");
        }
        break;
    case SPE_DETECT_BLESSEDNESS:
        allowall[0] = ALL_CLASSES;
        allowall[1] = '\0';
        if (invent && (!confused || !rn2(2)))
        {
            otmp = getobj(allowall, "detect blessedness for", 0, "");
            if (otmp)
            {
                if (otmp->oclass != COIN_CLASS) 
                {
                    u.uconduct.gnostic++;
                }
                else 
                {
                    /* coins */
                    otmp->blessed = otmp->cursed = 0; /* just in case */
                }

                if (otmp->blessed || otmp->cursed) 
                {
                    There_ex(ATR_NONE, otmp->blessed ? CLR_MSG_POSITIVE : CLR_MSG_NEGATIVE, "is %s flash as you cast the spell on %s.",
                        an(hcolor(otmp->blessed ? NH_AMBER : NH_BLACK)), the(cxname(otmp)));
                    otmp->bknown = 1;
                    update_inventory();
                }
                else 
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "The spell glimmers around %s for a moment and then fades.", the(cxname(otmp)));
                    if (otmp->oclass != COIN_CLASS)
                    {
                        otmp->bknown = 1;
                        update_inventory();
                    }
                }
            }
        }
        else
        {
            if(confused)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "The spell fizzles in the air.");
            else
                pline("You're not carrying anything to detect beautitude for.");
        }
        break;
    case SPE_CURSE:
    case SPE_BLESS:
        allowall[0] = ALL_CLASSES;
        allowall[1] = '\0';
        if (invent && !confused) 
        {
            otmp = getobj(allowall, (otyp == SPE_BLESS ? "bless" : "curse"), 0, "");
            if (otmp)
            {
                u.uconduct.gnostic++;
                enum sfx_sound_types soundid = SFX_BLESS_ITEM_SUCCESS;
                int textcolor = CLR_MSG_ATTENTION;
                if (otyp == SPE_BLESS) 
                {
                    if (otmp->cursed) 
                    {
                        func = uncurse;
                        textcolor = CLR_MSG_POSITIVE;
                        glowcolor = NH_AMBER;
                        costchange = COST_UNCURS;
                        soundid = SFX_UNCURSE_ITEM_SUCCESS;
                    }
                    else if (!otmp->blessed) 
                    {
                        func = bless;
                        textcolor = CLR_MSG_POSITIVE;
                        glowcolor = NH_LIGHT_BLUE;
                        costchange = COST_alter;
                        altfmt = TRUE; /* "with a <color> aura" */
                        soundid = SFX_BLESS_ITEM_SUCCESS;
                    }
                }
                else  
                { //Curse
                    if (otmp->blessed) 
                    {
                        func = unbless;
                        textcolor = CLR_MSG_WARNING;
                        glowcolor = "brown";
                        costchange = COST_UNBLSS;
                        soundid = SFX_UNBLESS_ITEM_SUCCESS;
                    }
                    else if (!otmp->cursed)
                    {
                        func = curse;
                        textcolor = CLR_MSG_NEGATIVE;
                        glowcolor = NH_BLACK;
                        costchange = COST_alter;
                        altfmt = TRUE;
                        soundid = SFX_CURSE_ITEM_SUCCESS;
                    }
                }
                if (func) 
                {
                    /* give feedback before altering the target object;
                       this used to set obj->bknown even when not seeing
                       the effect; now hero has to see the glow, and bknown
                       is cleared instead of set if perception is distorted */
                    glowcolor = hcolor(glowcolor);
                    if (altfmt)
                        pline_ex(ATR_NONE, textcolor, "%s with %s aura.", Yobjnam2(otmp, "glow"), an(glowcolor));
                    else
                        pline_ex(ATR_NONE, textcolor, "%s %s.", Yobjnam2(otmp, "glow"), glowcolor);
                    iflags.last_msg = PLNMSG_OBJ_GLOWS;
                    otmp->bknown = !Hallucination;
                    /* potions of water are the only shop goods whose price depends
                       on their curse/bless state */
                    if (otmp->unpaid && otmp->otyp == POT_WATER) 
                    {
                        if (costchange == COST_alter)
                            /* added blessing or cursing; update shop
                               bill to reflect item's new higher price */
                            alter_cost(otmp, 0L);
                        else if (costchange != COST_none)
                            /* removed blessing or cursing; you
                               degraded it, now you'll have to buy it... */
                            costly_alteration(otmp, costchange);
                    }
                    /* finally, change curse/bless state */
                    play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                    play_sfx_sound(soundid);
                    special_effect_wait_until_action(0);

                    (*func)(otmp);

                    special_effect_wait_until_end(0);
                }
                update_inventory();
            }
        }
        else
        {
            /* when casting a spell we know we're not confused,
               so inventory must be empty (another message has
               already been given above if reading a scroll) */
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline("You're not carrying anything to be %s.", (otyp == SPE_BLESS ? "blessed" : "cursed"));
        }
        break;
    case SCR_CHARGING:
        if(!(sobj->speflags & SPEFLAGS_SERVICED_SPELL))
        {
            if (confused)
            {
                if (scursed)
                {
                    You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "discharged.");
                    u.uen = 0;
                } 
                else 
                {
                    You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "charged up!");
                    u.uen += d(sblessed ? 6 : 4, 4);
                    if (u.uen > u.uenmax) { /* if current energy is already at   */
                        u.ubaseenmax += u.uen - u.uenmax; /* or near maximum, increase maximum */
                        updatemaxen();
                    }
                    else
                        u.uen = u.uenmax; /* otherwise restore current to max  */
                }
                context.botl = 1;
                break;
            }
            /* known = TRUE; -- handled inline here */
            if (!already_known)
            {
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "This is a charging scroll.");
                learnscroll(sobj);
            }
            /* use it up now to prevent it from showing in the
               getobj picklist because the "disappears" message
               was already delivered */
            useup(sobj);
            sobj = 0; /* it's gone */
        }
        otmp = getobj(all_count, "charge", 0, "");
        if (otmp)
        {
            recharge(otmp, scursed ? -1 : sblessed ? 1 : 0, TRUE);
        }
        break;
    case SCR_ENCHANT_ACCESSORY:
        otmp = (struct obj*)0;
        if (sobj->speflags & SPEFLAGS_SERVICED_SPELL)
        {
            const char enchant_accessory_objects[] = { ALL_CLASSES, RING_CLASS, MISCELLANEOUS_CLASS, 0 };
            otmp = getobj(enchant_accessory_objects, "enchant", 0, "");
            if (!otmp)
            {
                *effect_happened_ptr = 0;
                return 0;
            }

            if (otmp && ((otmp->oclass != RING_CLASS && otmp->oclass != MISCELLANEOUS_CLASS) || objects[otmp->otyp].oc_enchantable == ENCHTYPE_NO_ENCHANTMENT))
            {
                play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
                if(objects[otmp->otyp].oc_enchantable == ENCHTYPE_NO_ENCHANTMENT)
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have a feeling of loss.");
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, !Blind
                        ? "%s then fades."
                        : "%s warm for a moment.", Yobjnam2(otmp, !Blind ? "glow" : "feel"));
                sobj = 0;
                break;
            }
        }
        else
        {
            if (confused)
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);
                Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "head spins.");
                make_stunned((HStun& TIMEOUT) + rn1(scursed ? 90 : 40, 10), TRUE);;
                context.botl = context.botlx = 1;
                special_effect_wait_until_end(0);
                break;
            }
            boolean rightok = uright && objects[uright->otyp].oc_enchantable;
            boolean leftok = uleft && objects[uleft->otyp].oc_enchantable;
            boolean misc1ok = umisc && objects[umisc->otyp].oc_enchantable;
            boolean misc2ok = umisc2 && objects[umisc2->otyp].oc_enchantable;
            boolean misc3ok = umisc3 && objects[umisc3->otyp].oc_enchantable;
            boolean misc4ok = umisc4 && objects[umisc4->otyp].oc_enchantable;
            boolean misc5ok = umisc5 && objects[umisc5->otyp].oc_enchantable;
            boolean amuletok = uamul && objects[uamul->otyp].oc_enchantable;

            int numok = rightok + leftok + misc1ok + misc2ok + misc3ok + misc4ok + misc5ok + amuletok;
            int rndindex = numok <= 0 ? 0 : rnd(numok);
            int cnt = 0;
            int i;
            for (i = 1; i <= 8; i++)
            {
                switch (i)
                {
                case 1:
                    if (rightok)
                    {
                        cnt++;
                        otmp = uright;
                    }
                    break;
                case 2:
                    if (leftok)
                    {
                        cnt++;
                        otmp = uleft;
                    }
                    break;
                case 3:
                    if (misc1ok)
                    {
                        cnt++;
                        otmp = umisc;
                    }
                    break;
                case 4:
                    if (misc2ok)
                    {
                        cnt++;
                        otmp = umisc2;
                    }
                    break;
                case 5:
                    if (misc3ok)
                    {
                        cnt++;
                        otmp = umisc3;
                    }
                    break;
                case 6:
                    if (misc4ok)
                    {
                        cnt++;
                        otmp = umisc4;
                    }
                    break;
                case 7:
                    if (misc5ok)
                    {
                        cnt++;
                        otmp = umisc5;
                    }
                    break;
                case 8:
                    if (amuletok)
                    {
                        cnt++;
                        otmp = uamul;
                    }
                    break;
                }

                if (cnt == rndindex)
                    break;
            }
            //otmp = rightok && leftok ? (!rn2(2) ? uright : uleft) : rightok ? uright : leftok ? uleft : (struct obj*)0;
        }

        if (otmp)
        {
            if (otmp->oclass == RING_CLASS)
            {
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);
                enchant_ring(otmp, scursed ? -1 : sblessed ? 1 : 0);
                special_effect_wait_until_end(0);
            }
            else
            {
                /* misc items and other enchantable accessories are enchanted like armors */
                goto enchantarmor;
            }
        }
        else
            Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s tingle.", makeplural(body_part(FINGER)));

        known = TRUE;

        break;
    case SCR_MAGIC_MAPPING:
        if (level.flags.nommap) {
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "mind is filled with crazy lines!");
            if (Hallucination)
                pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "Wow!  Modern art.");
            else
                Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s spins in bewilderment.", body_part(HEAD));
            make_confused(itimeout_incr(HConfusion, rnd(30)), FALSE);
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            break;
        }
        if (sblessed) {
            register int x, y;

            for (x = 1; x < COLNO; x++)
                for (y = 0; y < ROWNO; y++)
                    if (levl[x][y].typ == SDOOR)
                        cvt_sdoor_to_door(x, y);
            /* do_mapping() already reveals secret passages */
        }
        known = TRUE;
        /*FALLTHRU*/
    case SPE_MAGIC_MAPPING:
        if (level.flags.nommap) {
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s spins as %s blocks the spell!", body_part(HEAD),
                 something);
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, rnd(30)), FALSE);
            break;
        }
        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "A map coalesces in your mind!");
        cval = (scursed && !confused);
        if (cval)
            HConfusion = 1; /* to screw up map */
        do_mapping();
        if (cval) 
        {
            HConfusion = 0; /* restore */
            pline("Unfortunately, you can't grasp the details.");
        }
        break;
    case SPE_DETECT_TRAPS:
        if (level.flags.nommap) 
        {
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s spins as %s blocks the spell!", body_part(HEAD),
                something);
            play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, rnd(30)), FALSE);
            break;
        }
        if(trap_detect(sobj) == 0) //Something was detected
            You_ex(ATR_NONE, CLR_MSG_SPELL, "become aware of the location of nearby traps!");
        else
            play_simple_object_sound(sobj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
        break;
    case SCR_AMNESIA:
        known = TRUE;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        forget((!sblessed ? ALL_SPELLS : 0)
               | (!confused || scursed ? ALL_MAP : 0));
        if (Hallucination) /* Ommmmmm! */
            Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "mind releases itself from mundane concerns.");
        else if (!strncmpi(plname, "Maud", 4))
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE,
          "As your mind turns inward on itself, you forget everything else.");
        else if (rn2(2))
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Who was that Maud person anyway?");
        else
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Thinking of Maud you forget everything else.");
        exercise(A_WIS, FALSE);
        special_effect_wait_until_end(0);
        break;
    case SCR_RETRAINING:
        known = TRUE;
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        special_effect_wait_until_action(0);
        
        int maxslots = u.max_weapon_slots;
        lose_weapon_skill(u.ulevel - 1);
        u.max_weapon_slots = maxslots;
        u.weapon_slots = u.max_weapon_slots;
        context.botl = context.botlx = 1;
        update_can_advance_any_skill();

        if (Hallucination) /* Ommmmmm! */
            Your_ex(ATR_NONE, CLR_MSG_MYSTICAL, "mind releases itself from epistemological concerns.");
        else if (!strncmpi(plname, "Maud", 4))
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL,
                "As your mind turns inward on itself, you forget the skills you have learned.");
        else if (rn2(2))
            You_ex(ATR_NONE, CLR_MSG_MYSTICAL, "wonder if Maud wanted you to learn something.");
        else
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "Your mind releases itself from the skills you've learned.");

        special_effect_wait_until_end(0);
        break;
    case SCR_FIRE: 
    {
        coord cc;
        boolean increased_damage = FALSE;

        cc.x = u.ux;
        cc.y = u.uy;
        useup(sobj);
        sobj = 0; /* it's gone */
        if (!already_known)
            (void) learnscrolltyp(SCR_FIRE);
        if (confused)
        {
            if (Fire_immunity || Invulnerable) 
            {
                u_shieldeff();
                if (!Blind)
                    pline("Oh, look, what a pretty fire in your %s.",
                          makeplural(body_part(HAND)));
                else
                    You_feel("a pleasant warmth in your %s.",
                             makeplural(body_part(HAND)));
            }
            else
            {
                pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "scroll catches fire and you burn your %s.",
                          makeplural(body_part(HAND)));
                losehp(adjust_damage(1, (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), "scroll of fire", KILLED_BY_AN);
            }
            break;
        }
        if (Underwater)
        {
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s around you vaporizes violently!", hliquid("water"));
        } 
        else
        {
            if (sblessed)
            {
                increased_damage = TRUE;
                if (!already_known)
                    pline("This is a scroll of fire!");
                pline("Where do you want to center the explosion?");
                getpos_sethilite(display_stinking_cloud_positions,
                                 get_valid_stinking_cloud_pos);
                (void) getpos(&cc, TRUE, "the desired position", CURSOR_STYLE_SPELL_CURSOR);
                if (!is_valid_stinking_cloud_pos(cc.x, cc.y, FALSE)) 
                {
                    /* try to reach too far, get burned */
                    cc.x = u.ux;
                    cc.y = u.uy;
                }
                u.dx = cc.x - u.ux;
                update_u_facing(TRUE);
                u.dx = 0;
            }
            if (cc.x == u.ux && cc.y == u.uy) 
            {
                pline_The("scroll erupts in a tower of flame!");
                iflags.last_msg = PLNMSG_TOWER_OF_FLAME; /* for explode() */
                burn_away_slime();
            }
        }

        if (increased_damage)
            explode(cc.x, cc.y, 11, &youmonst, 5, 6, 10, otyp, SCROLL_CLASS, EXPL_SCROLL_OF_FIRE);
        else
            explode(cc.x, cc.y, 11, &youmonst, 1, 2, (4 * sbcsign + 2) / 3, otyp, SCROLL_CLASS, EXPL_SCROLL_OF_FIRE);

        break;
    }
    case SCR_EARTH:
        /* TODO: handle steeds */
        if (!Is_really_rogue_level(&u.uz) && has_ceiling(&u.uz)
            && (!In_endgame(&u.uz) || Is_earthlevel(&u.uz))) 
        {
            register int x, y;
            int nboulders = 0;
            boolean spef_on = FALSE;
            /* Identify the scroll */
            if (iflags.using_gui_sounds)
            {
                /* Shake effect here would be nice */
                play_sfx_sound(SFX_RUMBLING_EARTH);
                if (!Deaf)
                {
                    if (u.uswallow)
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "rumbling.");
                    else
                        pline_The_ex(ATR_NONE, CLR_MSG_WARNING, "%s rumbles %s you!", ceiling(u.ux, u.uy),
                            sblessed ? "around" : "above");
                }
                delay_output_milliseconds(400);
            }
            else if(!Levitation && !Flying)
            {
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "the %s shaking %s you!", Underwater ? "water" : "earth", sblessed ? "around" : "above");
            }

            if (!scursed)
            {
                spef_on = TRUE;
                play_explosion_animation_at(u.ux, u.uy, EXPL_SCROLL_OF_EARTH);
                explosion_wait_until_action();
            }
            else
            {
                if (iflags.using_gui_sounds)
                    delay_output_milliseconds(600);
            }
            known = 1;
            sokoban_guilt();

            /* Loop through the surrounding squares */
            if (!scursed)
                for (x = u.ux - 1; x <= u.ux + 1; x++) {
                    for (y = u.uy - 1; y <= u.uy + 1; y++) {
                        /* Is this a suitable spot? */
                        if (isok(x, y) && !closed_door(x, y)
                            && !IS_ROCK(levl[x][y].typ)
                            && !IS_AIR(levl[x][y].typ)
                            && (x != u.ux || y != u.uy)) {
                            nboulders +=
                                drop_boulder_on_monster(x, y, confused, TRUE);
                        }
                    }
                }
            /* Attack the player */
            if (!sblessed) 
            {
                drop_boulder_on_player(confused, !scursed, TRUE, FALSE);
            }
            else if (!nboulders)
                pline("But nothing else happens.");

            if(spef_on)
                explosion_wait_until_end();
        }
        break;
    case SCR_PUNISHMENT:
        known = TRUE;
        if (confused || sblessed) {
            play_sfx_sound(SFX_GUILTY);
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "guilty.");
            break;
        }
        play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_MALIGNANT_AURA_SURROUNDS);
        special_effect_wait_until_action(0);
        punish(sobj);
        special_effect_wait_until_end(0);
        break;
    case SPE_FLAME_STRIKE:
    {
        coord cc;
        known = TRUE;
        pline("Where do you want to center the flame strike?");
        cc.x = u.ux;
        cc.y = u.uy;
        int trycnt = 0;
        while (trycnt < 10)
        {

            if (getpos(&cc, TRUE, "the desired position", CURSOR_STYLE_SPELL_CURSOR) < 0) {
                pline1(Never_mind);
                break;
            }
            if (!get_valid_targeted_position(cc.x, cc.y, otyp))
            {
                play_sfx_sound(SFX_GENERAL_NOT_AT_RIGHT_LOCATION);
                pline("Not a valid target position.");
                if (trycnt > 4)
                {
                    cc.x = u.ux;
                    cc.y = u.uy;
                    break;
                }
            }
            else
                break;

            trycnt++;
        }
        if (objects[otyp].oc_dir == TARGETED)
        {
            u.dx = cc.x - u.ux;
            update_u_facing(TRUE);
            update_u_action(ACTION_TILE_CAST_NODIR);
            play_simple_monster_sound(&youmonst, MONSTER_SOUND_TYPE_CAST);
            u_wait_until_action();
            u.dx = 0;
        }
        (void)explode(cc.x, cc.y, RAY_FIRE, &youmonst, 6, 6, 0, otyp, SPBOOK_CLASS, EXPL_FIERY);
        break;
    }
    case SPE_STINKING_CLOUD:
    case SCR_STINKING_CLOUD: {
        coord cc;

        if (!already_known && otyp == SCR_STINKING_CLOUD)
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have found a scroll of stinking cloud!");
        known = TRUE;
        pline("Where do you want to center the %scloud?",
              already_known ? "stinking " : "");
        cc.x = u.ux;
        cc.y = u.uy;
        getpos_sethilite(display_stinking_cloud_positions,
                         get_valid_stinking_cloud_pos);
        if (getpos(&cc, TRUE, "the desired position", CURSOR_STYLE_SPELL_CURSOR) < 0) {
            pline1(Never_mind);
            break;
        }
        if (!is_valid_stinking_cloud_pos(cc.x, cc.y, TRUE))
            break;
        u.dx = cc.x - u.ux;
        update_u_facing(TRUE);
        u.dx = 0;
        (void) create_gas_cloud(cc.x, cc.y, REGION_POISON_GAS, 3 + bcsign(sobj),
                                1, 8 + 4 * bcsign(sobj), 5);
        break;
    }
    default:
        impossible("What weird effect is this? (%u)", otyp);
    }
    /* if sobj is gone, we've already called useup() above and the
       update_inventory() that it performs might have come too soon
       (before charging an item, for instance) */

    *effect_happened_ptr = 1;

    if (!sobj)
        update_inventory();
    return sobj ? 0 : 1;
}

void
drop_boulder_on_player(confused, helmet_protects, byu, skip_uswallow)
boolean confused, helmet_protects, byu, skip_uswallow;
{
    int dmg;
    struct obj *otmp2;

    /* hit monster if swallowed */
    if (u.uswallow && !skip_uswallow) {
        drop_boulder_on_monster(u.ux, u.uy, confused, byu);
        return;
    }

    otmp2 = mksobj(confused ? ROCK : BOULDER, FALSE, FALSE, FALSE);
    if (!otmp2)
        return;
    otmp2->quan = confused ? rn1(5, 2) : 1;
    otmp2->owt = weight(otmp2);
    if (!amorphous(youmonst.data) && !Passes_walls
        && !is_incorporeal(youmonst.data) && !unsolid(youmonst.data)) {
        dmg = weapon_total_dmg_value(otmp2, &youmonst, (struct monst*)0, 1) * (int)otmp2->quan;
        play_object_hit_sound(otmp2, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(&youmonst), dmg, HMON_THROWN);
        You("are hit by %s!", doname(otmp2));
        if (uarmh && helmet_protects) {
            if (is_metallic(uarmh)) {
                pline("Fortunately, you are wearing a hard helmet.");
                if (dmg > 2)
                    dmg = 2;
            } else if (flags.verbose) {
                pline("%s does not protect you.", Yname2(uarmh));
            }
        }
    } else
        dmg = 0;
    wake_nearto(u.ux, u.uy, 4 * 4);
    /* Must be before the losehp(), for bones files */
    if (!flooreffects(otmp2, u.ux, u.uy, "fall")) {
        place_object(otmp2, u.ux, u.uy);
        stackobj(otmp2);
        newsym(u.ux, u.uy);
    }
    if (dmg)
    {
        double damage = adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
        if (damage > 0)
            display_u_being_hit(HIT_GENERAL, (int)damage, 0UL);
        losehp(damage, "scroll of earth", KILLED_BY_AN);
    }
}

boolean
drop_boulder_on_monster(x, y, confused, byu)
int x, y;
boolean confused, byu;
{
    register struct obj *otmp2;
    register struct monst *mtmp;

    /* Make the object(s) */
    otmp2 = mksobj(confused ? ROCK : BOULDER, FALSE, FALSE, FALSE);
    if (!otmp2)
        return FALSE; /* Shouldn't happen */
    otmp2->quan = confused ? rn1(5, 2) : 1;
    otmp2->owt = weight(otmp2);

    /* Find the monster here (won't be player) */
    mtmp = m_at(x, y);
    if (mtmp && !amorphous(mtmp->data) && !passes_walls(mtmp->data)
        && !is_incorporeal(mtmp->data) && !unsolid(mtmp->data)) {
        struct obj *helmet = which_armor(mtmp, W_ARMH);
        int mdmg;

        mdmg = weapon_total_dmg_value(otmp2, mtmp, (struct monst*)0, 1) * (int)otmp2->quan;
        play_object_hit_sound(otmp2, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mtmp), mdmg, HMON_THROWN);

            if (cansee(mtmp->mx, mtmp->my)) {
            pline("%s is hit by %s!", Monnam(mtmp), doname(otmp2));
            if (is_invisible(mtmp) && !canspotmon(mtmp))
                map_invisible(mtmp->mx, mtmp->my);
        } else if (u.uswallow && mtmp == u.ustuck)
            You_hear("something hit %s %s over your %s!",
                     s_suffix(mon_nam(mtmp)), mbodypart(mtmp, STOMACH),
                     body_part(HEAD));

        if (helmet) {
            if (is_metallic(helmet)) {
                if (canspotmon(mtmp))
                    pline("Fortunately, %s is wearing a hard helmet.",
                          mon_nam(mtmp));
                else if (!Deaf)
                    You_hear("a clanging sound.");
                if (mdmg > 2)
                    mdmg = 2;
            } else {
                if (canspotmon(mtmp))
                    pline("%s's %s does not protect %s.", Monnam(mtmp),
                          xname(helmet), mhim(mtmp));
            }
        }
        double damage = adjust_damage(mdmg, byu ? &youmonst : (struct monst*)0, mtmp, AD_PHYS, ADFLAGS_NONE);
        if (damage > 0)
            display_m_being_hit(mtmp, HIT_GENERAL, (int)damage, 0UL, FALSE);

        deduct_monster_hp(mtmp, damage);

        if (DEADMONSTER(mtmp)) 
        {
            if (byu) {
                killed(mtmp);
            } else {
                pline("%s is killed.", Monnam(mtmp));
                mondied(mtmp);
            }
        } else {
            wakeup(mtmp, byu);
        }
        wake_nearto(x, y, 4 * 4);
    } else if (u.uswallow && mtmp == u.ustuck) {
        obfree(otmp2, (struct obj *) 0);
        /* fall through to player */
        drop_boulder_on_player(confused, TRUE, FALSE, TRUE);
        return 1;
    }
    /* Drop the rock/boulder to the floor */
    if (!flooreffects(otmp2, x, y, "fall")) {
        place_object(otmp2, x, y);
        stackobj(otmp2);
        newsym(x, y); /* map the rock */
    }
    return TRUE;
}

/* overcharging any wand or zapping/engraving cursed wand */
void
wand_explode(obj, chg)
struct obj *obj;
int chg; /* recharging */
{
    const char *expl = !chg ? "suddenly" : "vibrates violently and";
    int dmg, n, k;

    /* number of damage dice */
    if (!chg)
        chg = 2; /* zap/engrave adjustment */
    n = obj->charges + chg;
    if (n < 2)
        n = 2; /* arbitrary minimum */
    /* size of damage dice */
    switch (obj->otyp) {
    case WAN_DISJUNCTION:
    case WAN_WISHING:
        k = 12;
        break;
    case WAN_CANCELLATION:
    case WAN_DEATH:
    case WAN_POLYMORPH:
    case WAN_UNDEAD_TURNING:
    case WAN_RESURRECTION:
        k = 10;
        break;
    case WAN_COLD:
    case WAN_FIRE:
    case WAN_LIGHTNING:
    case WAN_MAGIC_MISSILE:
    case WAN_IDENTIFY:
        k = 8;
        break;
    case WAN_NOTHING:
        k = 4;
        break;
    default:
        k = 6;
        break;
    }
    /* inflict damage and destroy the wand */
    dmg = d(n, k);
    obj->in_use = TRUE; /* in case losehp() is fatal (or --More--^C) */
    if (obj->oartifact)
    {
        play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s vanishes in thin air!", Yname2(obj), expl);
    }
    else
    {
        play_sfx_sound(SFX_EXPLOSION_MAGICAL);
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s explodes!", Yname2(obj), expl);
        losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_MAGM, ADFLAGS_SPELL_DAMAGE), "exploding wand", KILLED_BY_AN);
    }
    useup(obj);
    /* obscure side-effect */
    exercise(A_STR, FALSE);
}

/* used to collect gremlins being hit by light so that they can be processed
   after vision for the entire lit area has been brought up to date */
struct litmon {
    struct monst *mon;
    struct litmon *nxt;
};
STATIC_VAR struct litmon *gremlins = 0;

/*
 * Low-level lit-field update routine.
 */
STATIC_PTR void
set_lit(x, y, val)
int x, y;
genericptr_t val;
{
    struct monst *mtmp;
    struct litmon *gremlin;

    if (val) {
        levl[x][y].lit = 1;
        if ((mtmp = m_at(x, y)) != 0 && mtmp->data == &mons[PM_GREMLIN]) {
            gremlin = (struct litmon *) alloc(sizeof *gremlin);
            gremlin->mon = mtmp;
            gremlin->nxt = gremlins;
            gremlins = gremlin;
        }
    } else {
        levl[x][y].lit = 0;
        snuff_light_source(x, y);
    }
}

void
litroom(on, obj)
register boolean on;
struct obj *obj;
{
    char is_lit; /* value is irrelevant; we use its address
                    as a `not null' flag for set_lit() */

    /* first produce the text (provided you're not blind) */
    if (!on) {
        register struct obj *otmp;

        if (!Blind) {
            if (u.uswallow) {
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "It seems even darker in here than before.");
            } else {
                if (uwep && (artifact_light(uwep) || obj_shines_magical_light(uwep) || has_obj_mythic_magical_light(uwep)) && uwep->lamplit)
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "Suddenly, the only light left comes from %s!",
                          the(xname(uwep)));
                else
                    You_ex(ATR_NONE, CLR_MSG_SPELL, "are surrounded by darkness!");
            }
        }

        /* the magic douses lamps, et al, too */
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (otmp->lamplit)
                (void) snuff_lit(otmp);
    } else { /* on */
        if (u.uswallow) {
            if (Blind)
                ; /* no feedback */
            else if (is_animal(u.ustuck->data))
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s %s is lit.", s_suffix(Monnam(u.ustuck)),
                      mbodypart(u.ustuck, STOMACH));
            else if (is_whirly(u.ustuck->data))
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s shines briefly.", Monnam(u.ustuck));
            else
                pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s glistens.", Monnam(u.ustuck));
        } else if (!Blind)
            pline_ex(ATR_NONE, CLR_MSG_SPELL, "A lit field surrounds you!");
    }

    /* No-op when swallowed or in water */
    if (u.uswallow || Underwater || Is_waterlevel(&u.uz))
        return;
    /*
     *  If we are darkening the room and the hero is punished but not
     *  blind, then we have to pick up and replace the ball and chain so
     *  that we don't remember them if they are out of sight.
     */
    if (Punished && !on && !Blind)
        move_bc(1, 0, uball->ox, uball->oy, uchain->ox, uchain->oy);

    if (Is_really_rogue_level(&u.uz)) {
        /* Can't use do_clear_area because MAX_RADIUS is too small */
        /* rogue lighting must light the entire room */
        int rnum = levl[u.ux][u.uy].roomno - ROOMOFFSET;
        int rx, ry;

        if (rnum >= 0) {
            for (rx = rooms[rnum].lx - 1; rx <= rooms[rnum].hx + 1; rx++)
                for (ry = rooms[rnum].ly - 1; ry <= rooms[rnum].hy + 1; ry++)
                    set_lit(rx, ry,
                            (genericptr_t) (on ? &is_lit : (char *) 0));
            rooms[rnum].rlit = on;
        }
        /* hallways remain dark on the rogue level */
    } else
        do_clear_area(u.ux, u.uy,
                      (obj && obj->oclass == SCROLL_CLASS && obj->blessed)
                         ? 9 : 5,
                      set_lit, (genericptr_t) (on ? &is_lit : (char *) 0));

    /*
     *  If we are not blind, then force a redraw on all positions in sight
     *  by temporarily blinding the hero.  The vision recalculation will
     *  correctly update all previously seen positions *and* correctly
     *  set the waslit bit [could be messed up from above].
     */
    if (!Blind) {
        vision_recalc(2);

        /* replace ball&chain */
        if (Punished && !on)
            move_bc(0, 0, uball->ox, uball->oy, uchain->ox, uchain->oy);
    }

    vision_full_recalc = 1; /* delayed vision recalculation */
    if (gremlins) {
        struct litmon *gremlin;

        /* can't delay vision recalc after all */
        vision_recalc(0);
        /* after vision has been updated, monsters who are affected
           when hit by light can now be hit by it */
        do {
            gremlin = gremlins;
            gremlins = gremlin->nxt;
            light_hits_gremlin(gremlin->mon, rnd(5));
            free((genericptr_t) gremlin);
        } while (gremlins);
    }
}

STATIC_OVL void
do_class_genocide()
{
    int i, j, immunecnt, gonecnt, goodcnt, class, feel_dead = 0;
    char buf[BUFSZ] = DUMMY;
    boolean gameover = FALSE; /* true iff killed self */

    for (j = 0;; j++) 
    {
        if (j >= 5)
        {
            pline1(thats_enough_tries);
            return;
        }

        do
        {
            if (iflags.using_gui_tiles || !strcmpi(buf, "?"))
            {
                winid tmpwin;
                menu_item* selected;
                anything any;
                int n, choice;

                tmpwin = create_nhwindow(NHW_MENU);
                start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_SIMPLE);
                any = zeroany; /* zero out all bits */

                char cbuf[BUFSZ];
                strcpy(cbuf, "");
                int k;
                for (k = 0; k < MAX_MONSTER_CLASSES; k++)
                {
                    strcpy(cbuf, def_monsyms[k].name);
                    if (strcmp(cbuf, "") && def_monsyms[k].sym != ' ' && def_monsyms[k].sym != DEF_GHOST)
                    {
                        *cbuf = highc(*cbuf);
                        any.a_int = k + 1;
                        add_menu(tmpwin, NO_GLYPH, &any, def_monsyms[k].sym, 0, ATR_NONE, cbuf, MENU_UNSELECTED);
                    }
                }
                end_menu(tmpwin, "What class of monsters do you wish to genocide?");
                n = select_menu(tmpwin, PICK_ONE, &selected);
                destroy_nhwindow(tmpwin);
                if (n > 0)
                {
                    choice = selected[0].item.a_int - 1;
                    /* skip preselected entry if we have more than one item chosen */
                    free((genericptr_t)selected);
                    buf[0] = def_monsyms[choice].sym;
                    buf[1] = '\0';
                }
                else
                    return;
            }
            else
            {
                getlin_ex(GETLINE_MONSTER_CLASS, ATR_NONE, NO_COLOR, "What class of monsters do you wish to genocide?", buf, (char*)0, (char*)0, (char*)0);
                (void)mungspaces(buf);
            }
        } 
        while (!*buf || *buf == '?');


        /* choosing "none" preserves genocideless conduct */
        if (*buf == '\033' || !strcmpi(buf, "none")
            || !strcmpi(buf, "nothing"))
            return;

        class = name_to_monclass(buf, (int *) 0);
        if (class == 0 && (i = name_to_mon(buf)) != NON_PM)
            class = mons[i].mlet;
        immunecnt = gonecnt = goodcnt = 0;

        for (i = LOW_PM; i < NUM_MONSTERS; i++) 
        {
            if (mons[i].mlet == class) 
            {
                if (!(mons[i].geno & G_GENO))
                    immunecnt++;
                else if (mvitals[i].mvflags & MV_GENOCIDED)
                    gonecnt++;
                else
                    goodcnt++;
            }
        }
        if (!goodcnt && class != mons[urole.monsternum].mlet
            && class != mons[urace.monsternum].mlet)
        {
            if (gonecnt)
                pline("All such monsters are already nonexistent.");
            else if (immunecnt || class == S_invisible)
                You("aren't permitted to genocide such monsters.");
            else if (wizard && buf[0] == '*') {
                register struct monst *mtmp, *mtmp2;

                gonecnt = 0;
                for (mtmp = fmon; mtmp; mtmp = mtmp2) 
                {
                    mtmp2 = mtmp->nmon;
                    if (DEADMONSTER(mtmp))
                        continue;
                    mongone(mtmp);
                    gonecnt++;
                }
                play_sfx_sound(SFX_GENOCIDE);
                pline("Eliminated %d monster%s.", gonecnt, plur(gonecnt));
                return;
            }
            else
                pline("That %s does not represent any monster.",
                      strlen(buf) == 1 ? "symbol" : "response");
            continue;
        }

        boolean sound_played = FALSE;
        for (i = LOW_PM; i < NUM_MONSTERS; i++) 
        {
            if (mons[i].mlet == class)
            {
                char nam[BUFSZ];

                Strcpy(nam, pm_plural_name(&mons[i], 1)); // makeplural(mons[i].mname));
                /* Although "genus" is Latin for race, the hero benefits
                 * from both race and role; thus genocide affects either.
                 */
                if (Your_Own_Role(i) || Your_Own_Race(i)
                    || ((mons[i].geno & G_GENO)
                        && !(mvitals[i].mvflags & MV_GENOCIDED))) 
                {
                    /* This check must be first since player monsters might
                     * have MV_GENOCIDED or !G_GENO.
                     */
                    if (!sound_played)
                    {
                        play_sfx_sound(SFX_GENOCIDE);
                        sound_played = TRUE;
                    }
                    mvitals[i].mvflags |= (MV_GENOCIDED | MV_NOCORPSE);
                    reset_rndmonst(i);
                    kill_genocided_monsters();
                    update_inventory(); /* eggs & tins */
                    pline("Wiped out all %s.", nam);
                    if (Upolyd && i == u.umonnum) 
                    {
                        u.mh = -1;
                        if (Unchanging)
                        {
                            if (!feel_dead++)
                                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die.");
                            /* finish genociding this class of
                               monsters before ultimately dying */
                            gameover = TRUE;
                        } else
                            rehumanize();
                    }
                    /* Self-genocide if it matches either your race
                       or role.  Assumption:  male and female forms
                       share same monster class. */
                    if (i == urole.monsternum || i == urace.monsternum) 
                    {
                        u.uhp = -1;
                        if (Upolyd) 
                        {
                            if (!feel_dead++)
                                You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s inside.", udeadinside());
                        } 
                        else 
                        {
                            if (!feel_dead++)
                                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "die.");
                            gameover = TRUE;
                        }
                    }
                } 
                else if (mvitals[i].mvflags & MV_GENOCIDED)
                {
                    if (!gameover)
                        pline("All %s are already nonexistent.", nam);
                } 
                else if (!gameover) 
                {
                    /* suppress feedback about quest beings except
                       for those applicable to our own role */
                    if ((mons[i].msound != MS_LEADER
                         || quest_info(MS_LEADER) == i)
                        && (mons[i].msound != MS_NEMESIS
                            || quest_info(MS_NEMESIS) == i)
                        && (mons[i].msound != MS_GUARDIAN
                            || quest_info(MS_GUARDIAN) == i)
                        /* non-leader/nemesis/guardian role-specific monster
                           */
                        && (i != PM_NINJA /* nuisance */
                            || Role_if(PM_SAMURAI)))
                    {
                        boolean named, uniq;

                        named = is_mname_proper_name(&mons[i]) ? TRUE : FALSE;
                        uniq = (mons[i].geno & G_UNIQ) ? TRUE : FALSE;
                        /* one special case */
                        if (i == PM_HIGH_PRIEST)
                            uniq = FALSE;

                        You("aren't permitted to genocide %s%s.",
                            (uniq && !named) ? "the " : "",
                            (uniq || named) ? mons[i].mname : nam);
                    }
                }
            }
        }
        
        if (gameover || u.uhp == -1)
        {
            killer.format = KILLED_BY_AN;
            Strcpy(killer.name, "scroll of genocide");
            if (gameover)
                done(GENOCIDED);
        }
        return;
    }
}

#define REALLY 1
#define PLAYER 2
#define ONTHRONE 4
void
do_genocide(how)
int how;
/* 0 = no genocide; create monsters (cursed scroll) */
/* 1 = normal genocide */
/* 3 = forced genocide of player */
/* 5 (4 | 1) = normal genocide from throne */
{
    char buf[BUFSZ] = DUMMY;
    register int i, killplayer = 0;
    register int mndx;
    register struct permonst *ptr;
    const char *which;

    if (how & PLAYER)
    {
        mndx = u.umonster; /* non-polymorphed mon num */
        ptr = &mons[mndx];
        Strcpy(buf, ptr->mname);
        killplayer++;
    } 
    else 
    {
        for (i = 0;; i++) 
        {
            if (i >= 5)
            {
                /* cursed effect => no free pass (unless rndmonst() fails) */
                if (!(how & REALLY) && (ptr = rndmonst()) != 0)
                    break;

                pline1(thats_enough_tries);
                return;
            }
            getlin_ex(GETLINE_GENOCIDE, ATR_NONE, NO_COLOR, "What monster do you want to genocide?", buf, "type the name", (char*)0, (char*)0);
            (void) mungspaces(buf);
            /* choosing "none" preserves genocideless conduct */
            if (*buf == '\033' || !strcmpi(buf, "none")
                || !strcmpi(buf, "nothing")) 
            {
                /* ... but no free pass if cursed */
                if (!(how & REALLY) && (ptr = rndmonst()) != 0)
                    break; /* remaining checks don't apply */

                return;
            }

            mndx = name_to_mon(buf);
            if (mndx == NON_PM || (mvitals[mndx].mvflags & MV_GENOCIDED))
            {
                pline("Such creatures %s exist in this world.",
                      (mndx == NON_PM) ? "do not" : "no longer");
                continue;
            }
            ptr = &mons[mndx];
            /* Although "genus" is Latin for race, the hero benefits
             * from both race and role; thus genocide affects either.
             */
            if (Your_Own_Role(mndx) || Your_Own_Race(mndx))
            {
                killplayer++;
                break;
            }
            if (is_human(ptr))
                adjalign(-sgn(u.ualign.type));
            if (is_demon(ptr))
                adjalign(sgn(u.ualign.type));

            if (!(ptr->geno & G_GENO))
            {
                if (!Deaf) 
                {
                    /* FIXME: unconditional "caverns" will be silly in some
                     * circumstances.  Who's speaking?  Divine pronouncements
                     * aren't supposed to be hampered by deafness....
                     */
                    if (flags.verbose)
                        pline("A thunderous voice booms through the caverns:");

                    play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_NO_MORTAL_THAT_WILL_NOT_BE_DONE);
                    verbalize_ex(ATR_NONE, CLR_MSG_GOD, "No, mortal!  That will not be done.");
                }
                continue;
            }
            /* KMH -- Unchanging prevents rehumanization */
            if (Unchanging && ptr == youmonst.data)
                killplayer++;
            break;
        }
        mndx = monsndx(ptr); /* needed for the 'no free pass' cases */
    }

    which = "all ";
    char singularbuf[BUFSZ] = "";
    char pluralbuf[BUFSZ] = "";
    if (Hallucination)
    {
        if (Upolyd)
        {
            Strcpy(singularbuf, youmonst.data->mname);
        }
        else 
        {
            Strcpy(singularbuf, (flags.female && urole.name.f) ? urole.name.f
                                                       : urole.name.m);
            singularbuf[0] = lowc(singularbuf[0]);
        }
        const char* plr = makeplural(singularbuf);
        Strcpy(pluralbuf, plr);
    }
    else 
    {
        if ((ptr->geno & G_UNIQ) && ptr != &mons[PM_HIGH_PRIEST])
        {
            which = !is_mname_proper_name(ptr) ? "the " : "";
            Strcpy(singularbuf, ptr->mname); /* make sure we have standard singular */
            Strcpy(pluralbuf, ptr->mname); /* make sure we have standard singular */
        }
        else
        {
            Strcpy(singularbuf, pm_common_name(ptr));
            Strcpy(pluralbuf, pm_plural_name(ptr, 1));
        }
    }

    if (how & REALLY) 
    {
        play_sfx_sound(SFX_GENOCIDE);

        /* setting no-corpse affects wishing and random tin generation */
        mvitals[mndx].mvflags |= (MV_GENOCIDED | MV_NOCORPSE);
        pline("Wiped out %s%s.", which, pluralbuf);
              //(*which != 'a') ? buf : makeplural(buf));

        if (killplayer) 
        {
            u.uhp = -1;
            if (how & PLAYER)
            {
                killer.format = KILLED_BY;
                Strcpy(killer.name, "genocidal confusion");
            }
            else if (how & ONTHRONE) 
            {
                /* player selected while on a throne */
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "imperious order");
            }
            else 
            { /* selected player deliberately, not confused */
                killer.format = KILLED_BY_AN;
                Strcpy(killer.name, "scroll of genocide");
            }

            /* Polymorphed characters will die as soon as they're rehumanized.
             */
            /* KMH -- Unchanging prevents rehumanization */
            if (Upolyd && ptr != youmonst.data) 
            {
                delayed_killer(POLYMORPH, killer.format, killer.name);
                You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s inside.", udeadinside());
            } 
            else
                done(GENOCIDED);
        } 
        else if (ptr == youmonst.data) 
        {
            rehumanize();
        }
        reset_rndmonst(mndx);
        kill_genocided_monsters();
        update_inventory(); /* in case identified eggs were affected */
    }
    else 
    {
        int cnt = 0, census = monster_census(FALSE);

        context.makemon_spef_idx = 0;
        if (!(mons[mndx].geno & G_UNIQ)
            && !(mvitals[mndx].mvflags & (MV_GENOCIDED | MV_EXTINCT)))
            for (i = rn1(3, 4); i > 0; i--)
            {
                if (!makemon(ptr, u.ux, u.uy, MM_NO_MONSTER_INVENTORY))
                    break; /* couldn't make one */
                context.makemon_spef_idx++;
                ++cnt;
                if (mvitals[mndx].mvflags & MV_EXTINCT)
                    break; /* just made last one */
            }
        makemon_animation_wait_until_end();

        if (cnt) 
        {
            /* accumulated 'cnt' doesn't take groups into account;
               assume bringing in new mon(s) didn't remove any old ones */
            play_sfx_sound(SFX_SUMMON_MONSTER);
            cnt = monster_census(FALSE) - census;
            pline("Sent in %s%s.", (cnt > 1) ? "some " : "",
                  (cnt > 1) ? makeplural(singularbuf) : an(singularbuf));
        }
        else
            pline1(nothing_happens);
    }
}

void
punish(sobj)
struct obj *sobj;
{
    if (!sobj)
        return;

    struct obj *reuse_ball = (sobj && sobj->otyp == HEAVY_IRON_BALL)
                                ? sobj : (struct obj *) 0;

    /* KMH -- Punishment is still okay when you are riding */
    if (!reuse_ball)
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are being punished for your misbehavior!");

    if (Punished)
    {
        Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "iron ball gets heavier.");
        uball->owt += IRON_BALL_W_INCR * (1 + sobj->cursed);
        return;
    }

    if (amorphous(youmonst.data) || is_whirly(youmonst.data)
        || unsolid(youmonst.data) || is_incorporeal(youmonst.data)) {
        if (!reuse_ball) {
            pline("A ball and chain appears, then falls away.");
            dropy(mkobj(BALL_CLASS, TRUE, FALSE));
        } else {
            dropy(reuse_ball);
        }
        return;
    }
    setworn(mkobj(CHAIN_CLASS, TRUE, FALSE), W_CHAIN);
    if (!reuse_ball)
        setworn(mkobj(BALL_CLASS, TRUE, FALSE), W_BALL);
    else
        setworn(reuse_ball, W_BALL);

    if(Punished) /* Check that punishment succeeded */
        uball->special_quality = 1; /* special ball (see save) */

    /*
     *  Place ball & chain if not swallowed.  If swallowed, the ball & chain
     *  variables will be set at the next call to placebc().
     */
    if (!u.uswallow) {
        placebc();
        if (Blind)
            set_bc(1);      /* set up ball and chain variables */
        newsym(u.ux, u.uy); /* see ball&chain if can't see self */
    }
}

/* remove the ball and chain */
void
unpunish()
{
    struct obj *savechain = uchain;

    obj_extract_self(uchain);
    newsym(uchain->ox, uchain->oy);
    setworn((struct obj *) 0, W_CHAIN);
    dealloc_obj(savechain);
    uball->special_quality = 0;
    setworn((struct obj *) 0, W_BALL);
}

/* some creatures have special data structures that only make sense in their
 * normal locations -- if the player tries to create one elsewhere, or to
 * revive one, the disoriented creature becomes a zombie
 */
boolean
cant_revive(mtype, revival, from_obj)
int *mtype;
boolean revival;
struct obj *from_obj;
{
    /* SHOPKEEPERS can be revived now */
    if (*mtype == PM_GUARD || (*mtype == PM_SHOPKEEPER && !revival)
        || *mtype == PM_HIGH_PRIEST || *mtype == PM_ALIGNED_PRIEST
        || *mtype == PM_ANGEL || *mtype == PM_ALEAX || *mtype == PM_ARCHON) {
        *mtype = PM_HUMAN_ZOMBIE;
        return TRUE;
    } else if (*mtype == PM_LONG_WORM_TAIL) { /* for create_particular() */
        *mtype = PM_LONG_WORM;
        return TRUE;
    } else if (*mtype == PM_ELDER_LONG_WORM_TAIL) { /* for create_particular() */
        *mtype = PM_ELDER_LONG_WORM;
        return TRUE;
    } else if (unique_corpstat(&mons[*mtype])
               && (!from_obj || !has_omonst(from_obj))) {
        /* unique corpses (from bones or wizard mode wish) or
           statues (bones or any wish) end up as shapechangers */
        *mtype = PM_DOPPELGANGER;
        return TRUE;
    }
    return FALSE;
}

struct _create_particular_data {
    int which;
    int fem;
    char monclass;
    boolean randmonst;
    boolean maketame, makepeaceful, makehostile;
    boolean sleeping, saddled, invisible;
};

boolean
create_particular_parse(str, d)
char *str;
struct _create_particular_data *d;
{
    char *bufp = str;
    char *tmpp;

    d->monclass = MAX_MONSTER_CLASSES;
    d->which = urole.monsternum; /* an arbitrary index into mons[] */
    d->fem = -1; /* gender not specified */
    d->randmonst = FALSE;
    d->maketame = d->makepeaceful = d->makehostile = FALSE;
    d->sleeping = d->saddled = d->invisible = FALSE;

    if ((tmpp = strstri(bufp, "saddled ")) != 0) {
        d->saddled = TRUE;
        (void) memset(tmpp, ' ', sizeof "saddled " - 1);
    }
    if ((tmpp = strstri(bufp, "sleeping ")) != 0) {
        d->sleeping = TRUE;
        (void) memset(tmpp, ' ', sizeof "sleeping " - 1);
    }
    if ((tmpp = strstri(bufp, "invisible ")) != 0) {
        d->invisible = TRUE;
        (void) memset(tmpp, ' ', sizeof "invisible " - 1);
    }
    /* check "female" before "male" to avoid false hit mid-word */
    if ((tmpp = strstri(bufp, "female ")) != 0) {
        d->fem = 1;
        (void) memset(tmpp, ' ', sizeof "female " - 1);
    }
    if ((tmpp = strstri(bufp, "male ")) != 0) {
        d->fem = 0;
        (void) memset(tmpp, ' ', sizeof "male " - 1);
    }
    bufp = mungspaces(bufp); /* after potential memset(' ') */
    /* allow the initial disposition to be specified */
    if (!strncmpi(bufp, "tame ", 5)) {
        bufp += 5;
        d->maketame = TRUE;
    } else if (!strncmpi(bufp, "peaceful ", 9)) {
        bufp += 9;
        d->makepeaceful = TRUE;
    } else if (!strncmpi(bufp, "hostile ", 8)) {
        bufp += 8;
        d->makehostile = TRUE;
    }
    /* decide whether a valid monster was chosen */
    if (wizard && (!strcmp(bufp, "*") || !strcmp(bufp, "random"))) {
        d->randmonst = TRUE;
        return TRUE;
    }
    d->which = name_to_mon_ex(bufp, &d->fem);
    if (d->which >= LOW_PM)
        return TRUE; /* got one */
    d->monclass = name_to_monclass(bufp, &d->which);

    if (d->which >= LOW_PM) {
        d->monclass = MAX_MONSTER_CLASSES; /* matters below */
        return TRUE;
    } else if (d->monclass == S_invisible) { /* not an actual monster class */
        d->which = PM_STALKER;
        d->monclass = MAX_MONSTER_CLASSES;
        return TRUE;
    } else if (d->monclass == S_WORM_TAIL) { /* empty monster class */
        d->which = PM_LONG_WORM;
        d->monclass = MAX_MONSTER_CLASSES;
        return TRUE;
    } else if (d->monclass > 0) {
        d->which = urole.monsternum; /* reset from NON_PM */
        return TRUE;
    }
    return FALSE;
}

boolean
create_particular_creation(d)
struct _create_particular_data *d;
{
    struct permonst *whichpm = NULL;
    int i, firstchoice = NON_PM;
    struct monst *mtmp;
    boolean madeany = FALSE;

    if (!d->randmonst)
    {
        firstchoice = d->which;
        if (cant_revive(&d->which, FALSE, (struct obj *) 0)
            && firstchoice != PM_LONG_WORM_TAIL && firstchoice != PM_ELDER_LONG_WORM_TAIL)
        {
            /* wizard mode can override handling of special monsters */
            char buf[BUFSZ];

            Sprintf(buf, "Creating %s instead; force %s?",
                    mons[d->which].mname, mons[firstchoice].mname);
            if (yn_query(buf) == 'y')
                d->which = firstchoice;
        }
        whichpm = &mons[d->which];
    }

    context.makemon_spef_idx = 0;
    for (i = 0; i <= multi; i++)
    {
        if (d->monclass != MAX_MONSTER_CLASSES)
            whichpm = mkclass(d->monclass, 0);
        else if (d->randmonst)
            whichpm = rndmonst();

        /* 'is_FOO()' ought to be called 'always_FOO()' */
        unsigned long genderflag = 0UL;
        if (d->fem != -1 && !is_male(whichpm) && !is_female(whichpm) && !is_neuter(whichpm))
        {
            if(d->fem == 0)
                genderflag = MM_MALE;
            else if (d->fem == 1)
                genderflag = MM_FEMALE;
        }

        mtmp = makemon(whichpm, u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END | genderflag);

        if (!mtmp)
        {
            /* quit trying if creation failed and is going to repeat */
            if (d->monclass == MAX_MONSTER_CLASSES && !d->randmonst)
                break;
            /* otherwise try again */
            continue;
        }
        context.makemon_spef_idx++;

        if (d->maketame) 
        {
            (void) tamedog(mtmp, (struct obj *) 0, TAMEDOG_FORCE_ALL, FALSE, 0, FALSE, FALSE);
        } 
        else if (d->makepeaceful || d->makehostile) 
        {
            mtmp->mtame = 0; /* sanity precaution */
            mtmp->mpeaceful = d->makepeaceful ? 1 : 0;
            set_malign(mtmp);
            newsym(mtmp->mx, mtmp->my);
        }

        if (d->saddled && can_saddle(mtmp) && !which_armor(mtmp, W_SADDLE)) 
        {
            struct obj *otmp = mksobj(SADDLE, TRUE, FALSE, FALSE);

            put_saddle_on_mon(otmp, mtmp);
        }

        if (d->invisible) 
        {
            int mx = mtmp->mx, my = mtmp->my;
            mtmp->mprops[INVISIBILITY] |= M_INTRINSIC_ACQUIRED;
            if (does_block(mx, my, &levl[mx][my]))
                block_vision_and_hearing_at_point(mx, my);
            else
                unblock_vision_and_hearing_at_point(mx, my);
        }

        if (d->sleeping)
            mtmp->msleeping = 1;

        madeany = TRUE;
        /* in case we got a doppelganger instead of what was asked
           for, make it start out looking like what was asked for */
        if (mtmp->cham != NON_PM && firstchoice != NON_PM && mtmp->cham != firstchoice)
            (void) newcham(mtmp, &mons[firstchoice], FALSE, FALSE);

        newsym(mtmp->mx, mtmp->my);
    }
    makemon_animation_wait_until_end();

    return madeany;
}

/*
 * Make a new monster with the type controlled by the user.
 *
 * Note:  when creating a monster by class letter, specifying the
 * "strange object" (']') symbol produces a random monster rather
 * than a mimic.  This behavior quirk is useful so don't "fix" it
 * (use 'm'--or "mimic"--to create a random mimic).
 *
 * Used in wizard mode only (for ^G command and for scroll or spell
 * of create monster).  Once upon a time, an earlier incarnation of
 * this code was also used for the scroll/spell in explore mode.
 */
boolean
create_particular()
{
    char buf[BUFSZ] = DUMMY, *bufp;
    int  tryct = 5;
    struct _create_particular_data d;

    do {
        getlin_ex(GETLINE_GENESIS, ATR_NONE, NO_COLOR, "Create what kind of monster?", buf, "type the name or symbol", (char*)0, (char*)0);
        bufp = mungspaces(buf);
        if (*bufp == '\033')
            return FALSE;

        if (create_particular_parse(bufp, &d))
            break;

        /* no good; try again... */
        pline("I've never heard of such monsters.");
    } while (--tryct > 0);

    if (!tryct)
        pline1(thats_enough_tries);
    else
        return create_particular_creation(&d);

    return FALSE;
}

void
dragon_scales_to_scale_mail(otmp, sblessed)
struct obj* otmp;
boolean sblessed;
{
    if (!otmp || !is_dragon_scales(otmp))
        return;

    xchar ox = u.ux, oy = u.uy;
    boolean worn = !!otmp->owornmask;
    get_obj_location(otmp, &ox, &oy, 0);
    play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, ox, oy, FALSE);
    play_sfx_sound(SFX_ENCHANT_ITEM_SPECIAL_SUCCESS);
    special_effect_wait_until_action(0);

    /* dragon scales get turned into dragon scale mail */
    pline("%s merges and hardens!", Yname2(otmp));

    if(worn)
        setworn((struct obj*)0, W_ARM);
    
    /* assumes same order */
    otmp->otyp += GRAY_DRAGON_SCALE_MAIL - GRAY_DRAGON_SCALES;
    if (sblessed)
    {
        otmp->enchantment++;
        if (!otmp->blessed)
            bless(otmp);
    }
    else if (otmp->cursed)
        uncurse(otmp);
    otmp->known = 1;
    
    if (worn)
        setworn(otmp, W_ARM);
    
    if (otmp->unpaid)
        alter_cost(otmp, 0L); /* shop bill */

    special_effect_wait_until_end(0);
}
/*read.c*/

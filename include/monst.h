/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    monst.h    $NHDT-Date: 1550524559 2019/02/18 21:15:59 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.28 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2016. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MONST_H
#define MONST_H

#include "mextra.h"
#include "prop.h"
#include "attrib.h"

/* The weapon_strategy flag is used two ways:
 * 1) When calling mon_wield_item, is 2-6 depending on what is desired.
 * 2) Between calls to mon_wield_item, is 0 or 1 depending on whether or not
 *    the weapon is known by the monster to be cursed (so it shouldn't bother
 *    trying for another weapon).
 * I originally planned to also use 0 if the monster already had its best
 * weapon, to avoid the overhead of a call to mon_wield_item, but it turns out
 * that there are enough situations which might make a monster change its
 * weapon that this is impractical.  --KAA
 */
enum wpn_chk_flags {
    NO_WEAPON_WANTED    = 0,
    NEED_WEAPON         = 1,
    NEED_RANGED_WEAPON  = 2,
    NEED_HTH_WEAPON     = 3,
    NEED_HTH_NO_POLE    = 4,
    NEED_PICK_AXE       = 5,
    NEED_AXE            = 6,
    NEED_PICK_OR_AXE    = 7
};

/* The following flags are used for the second argument to display_minventory
 * in invent.c:
 *
 * PICK_NONE, PICK_ONE, PICK_ANY (wintype.h)  0, 1, 2
 * MINV_NOLET  If set, don't display inventory letters on monster's inventory.
 * MINV_ALL    If set, display all items in monster's inventory, otherwise
 *           just display wielded weapons and worn items.
 */
#define MINV_PICKMASK 0x03 /* 1|2 */
#define MINV_NOLET    0x04
#define MINV_ALL      0x08

/* monster appearance types */
enum m_ap_types {
    M_AP_NOTHING   = 0, /* mappearance unused--monster appears as itself */
    M_AP_FURNITURE = 1, /* stairs, a door, an altar, etc. */
    M_AP_OBJECT    = 2, /* an object */
    M_AP_MONSTER   = 3  /* a monster; mostly used for cloned Wizard */
};

#define M_AP_TYPMASK  0x7
#define M_AP_F_DKNOWN 0x8
#define U_AP_TYPE (youmonst.m_ap_type & M_AP_TYPMASK)
#define U_AP_FLAG (youmonst.m_ap_type & ~M_AP_TYPMASK)
#define M_AP_TYPE(m) ((m)->m_ap_type & M_AP_TYPMASK)
#define M_AP_FLAG(m) ((m)->m_ap_type & ~M_AP_TYPMASK)
#define MAX_MONST_REVIVALS 250

struct monst {
    struct monst *nmon;
    struct permonst *data;
    unsigned short subtype; /* Reserved for races of dogs, varying appearances for humans, etc. */
    unsigned m_id;
    short mnum;           /* permanent monster index number */
    short cham;           /* if shapeshifter, orig mons[] idx goes here */
    short cham_subtype;   /* if shapeshifter, orig subtype goes here */
    short movement;       /* movement points (derived from permonst definition
                             and added effects */
    uchar m_lev;          /* adjusted difficulty level of monster */
    aligntyp mhostility;  /* hostility of this monster towards the player, (nonnegative = good to kill) */
    xchar mx0, my0;       /* where the monster is moving from */
    xchar mx, my;
    xchar mux, muy;       /* where the monster thinks you are */
#define MTSZ 4
    coord mtrack[MTSZ];   /* monster track */

    struct attribs 
        acurr,            /* monster's current attributes (eg. str)*/
        abonus,           /* monster's bonus attributes (eg. str) */
        afixmin,          /* monster's fixed min attributes (eg. str), set by an item */
        afixmax,          /* monster's fixed max attributes (eg. str), set by an item */
        amin,             /* monster's min attributes (eg. str) */
        amax,             /* monster's max attributes (eg. str) */
        atemp,            /* used for temporary loss/gain */
        atime;            /* used for loss/gain countdown */

    schar mdaminc;
    schar mhitinc;
    schar macbonus;
    schar mmcbonus;

    int mhp, mbasehpmax, mhpmax, mbasehpdrain;
    int mhp_fraction;

    int men, mbaseenmax, menmax, mbaseendrain;
    int men_fraction;

    uchar heads_left;
    
    unsigned mappearance; /* for undetected mimics and the wiz */
    uchar m_ap_type;      /* what mappearance is describing, m_ap_types */

    schar mtame;                /* level of tameness, implies peaceful */

    unsigned short mprops[MAX_PROPS]; /* simplified props structure for monsters, taking 1/6 u.uprops memory space but doing about the same thing anyway --JG */

#define M_TIMEOUT                0x3fff    /* timeout mask for temporary property */
#define M_INTRINSIC_ACQUIRED     0x4000    /* permanent intrisic acquired e.g. by eating monster corpses */
#define M_EXTRINSIC              0x8000    /* extrinsic from items or environment, updated frequently by clearing and checking a new value */

    unsigned short mspec_used;             /* monster's general special ability and spell attack timeout */
    unsigned short mmagespell_used;        /* monster's mage-style normal ability or low-powered spell timeout */
    unsigned short mmageintermediate_used; /* monster's mage-style intermediate ability or medium-powered mage spell timeout */
    unsigned short mmageultimate_used;     /* monster's mage-style ultimate ability or high-powered mage spell timeout */
    unsigned short mclericspell_used;      /* monster's cleric-style normal ability or low-powered spell timeout */
    unsigned short mclericintermediate_used; /* monster's cleric-style intermediate ability or medium-powered spell timeout */
    unsigned short mclericultimate_used;   /* monster's cleric-style ultimate ability or high-powered mage spell timeout */
    unsigned short mdemonsummon_used;      /* demon summon timeout */
    unsigned short mspecialsummon_used;    /* special (nondemon) summon timeout */
    unsigned short mspecialsummon2_used;   /* another special (nondemon) summon timeout */

    short mcomingtou;
    xchar yell_x, yell_y;   /* location where the pet heard you yelling from */
    short notalktimer;
    short notraveltimer;
    short reserved_short1;  
    short reserved_short2;
    short rumorsleft;       /* how many rumors the monster still knows, -1 means that the monster has already told the player that it does not know any more rumors */
    uchar action;           /* the monster is currently in the midst of one of its attacks or actions */

    short mflee_timer;      /* timeout for mflee */
    short mfrozen;
    short mstaying;         /* commanded to stay in place, similar to frozen, but commanded */
    short mcarrying;

    short mrevived;         /* Number of times revived */

    /* Bitfield flags -- Keep all bitfields in a row */
    Bitfield(mflee, 1);     /* fleeing */
    Bitfield(msleeping, 1); /* asleep until woken */
    Bitfield(mcanmove, 1);  /* paralysis, similar to mblinded */
    Bitfield(mwantstomove, 1);  /* mon wants to move, not staying in place */
    Bitfield(mwantstodrop, 1);

    Bitfield(female, 1);         /* is female */
    Bitfield(mburied, 1);        /* has been buried */
    Bitfield(mundetected, 1);    /* not seen in present hiding place;
                                  * implies one of M1_CONCEAL or M1_HIDE,
                                  * but not mimic (that is, snake, spider,
                                  * trapper, piercer, eel)
                                  */
    Bitfield(mcloned, 1);   /* has been cloned from another */

    Bitfield(mavenge, 1);   /* did something to deserve retaliation */
    Bitfield(mpeaceful, 1); /* does not attack unprovoked */
    Bitfield(mtrapped, 1);  /* trapped in a pit, web or bear trap */
    Bitfield(mleashed, 1);  /* monster is on a leash */
    Bitfield(isshk, 1);     /* is shopkeeper */
    Bitfield(isminion, 1);  /* is a minion */
    Bitfield(isgd, 1);      /* is guard */
    Bitfield(ispriest, 1);  /* is an aligned priest or high priest */
    Bitfield(issmith, 1);   /* is a smith */
    Bitfield(isnpc, 1);     /* is a non-player character */

    Bitfield(issummoned, 1);                       /* is a summoned monster */
    Bitfield(disregards_enemy_strength, 1);        /* the monster attacks too strong enemies */
    Bitfield(disregards_own_health, 1);            /* the monster attacks even when its health is low */
    Bitfield(hasbloodlust, 1);                     /* attacks also peaceful */
    Bitfield(ispacifist, 1);                       /* does not attack peaceful, NOW DEACTIVATED, APPLIES TO ALL */
    Bitfield(isfaithful, 1);                       /* being separate from the owner does not reduce tameness */
    Bitfield(isprotector, 1);                      /* attacks hostiles if itself is being peaceful */
    Bitfield(ispartymember, 1);                    /* a peaceful monster that has joined your party (e.g., does not give you the money or items back from its inventory) */
    /* 4 free bits to 32 bit integer */
    /* The following splits bitfields so they fit in one int both in 32 bit and 64 bit systems */
    uchar  talkstate_item_trading;                 /* 1 = has said introduction, 2 = has said non-repeatable secondary question, 3 = has said first repeatable confirmatory question,  4 = has said second repeatable confirmatory question */
    uchar  talkstate_special;                      /* Special index, e.g., for Aleax */
    short  general_special_index;                  /* Special general purpose index */
    Bitfield(leaves_no_corpse, 1);                 /* this particular monster does not leave a corpse */
    Bitfield(delayed_killer_by_you, 1);            /* is petrification or other delayed killer initiated by you */
    Bitfield(u_know_mname, 1);                     /* you know the monster's name */
    Bitfield(told_rumor, 1);                       /* the monster had told the player at least one rumor */
    Bitfield(facing_right, 1);                     /* the monster is facing right */

    Bitfield(iswiz, 1);     /* is the Wizard of Yendor */
    Bitfield(wormno, 5);    /* at most 31 worms on any level */
#define MAX_NUM_WORMS 32    /* should be 2^(wormno bitfield size) */
    Bitfield(heads_tamed, 2); /* How many Cerberos's heads have been tamed; anything else for other monsters */
    Bitfield(boss_fight_started, 1); /* boss fight has already been announced */
    Bitfield(special_talk_flag1, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag2, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag3, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag4, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag5, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag6, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag7, 1); /* general purpose flag NPC talk */
    Bitfield(special_talk_flag8, 1); /* general purpose flag NPC talk */
    /* 10 free bits to 32-bit integer */

    unsigned long mon_flags; /* General easy-to-add flags for monsters for things not covered by the above bitfields */
#define MON_FLAGS_NONE              0x00000000UL
#define MON_FLAGS_RWRAITH           0x00000001UL
#define MON_FLAGS_CHAINED           0x00000002UL
#define MON_FLAGS_CLONED_WIZ        0x00000004UL
#define MON_FLAGS_SUMMONED_AT_ALTAR 0x00000008UL
#define MON_FLAGS_SPLEVEL_RESIDENT  0x00000010UL
#define MON_FLAGS_YOUR_CHILD        0x00000020UL
#define MON_FLAGS_SPOTTED_IN_RUN    0x00000040UL /* Spotted for purposes of determining whether to stop travel, run and similar commands */
#define MON_FLAGS_LEVEL_BOSS        0x00000080UL

    unsigned long mstrategy; /* for monsters with mflag3: current strategy */

#ifdef NHSTDC
#define STRAT_APPEARMSG 0x80000000UL
#else
#define STRAT_APPEARMSG 0x80000000UL
#endif
#define STRAT_ARRIVE    0x40000000UL /* just arrived on current level */
#define STRAT_WAITFORU  0x20000000UL
#define STRAT_CLOSE     0x10000000UL
#define STRAT_WAITMASK  (STRAT_CLOSE | STRAT_WAITFORU)
#define STRAT_HEAL      0x08000000UL
#define STRAT_GROUND    0x04000000UL
#define STRAT_MONSTR    0x02000000UL
#define STRAT_PLAYER    0x01000000UL
#define STRAT_NONE      0x00000000UL
#define STRAT_STRATMASK 0x0f000000UL
#define STRAT_XMASK     0x00ff0000UL
#define STRAT_YMASK     0x0000ff00UL
#define STRAT_GOAL      0x000000ffUL
#define STRAT_GOALX(s) ((xchar) ((s & STRAT_XMASK) >> 16))
#define STRAT_GOALY(s) ((xchar) ((s & STRAT_YMASK) >> 8))

    long mtrapseen;        /* bitmap of traps we've been trapped in */
    long mlstmv;           /* for catching up with lost time */
    long mspare1;
    struct obj *minvent;   /* mon's inventory */

    struct obj *mw;        /* mon's weapon */
    long worn_item_flags;  /* mon's wornmask */
    xchar weapon_strategy; /* flag for whether to try switching weapons */

    int meating;           /* monster is eating timeout */
    long summonduration;   /* duration for summoned units */
    long extra_encounter_xp; /* extra experience yielded by this monster due to encounter difficulty */
    int glyph;
    int gui_glyph;

    xchar timed;           /* # of fuses (timers) attached to this monst */
    struct mextra *mextra; /* point to mextra struct */
};

#define newmonst() (struct monst *) alloc(sizeof (struct monst))

/* these are in mspeed */
#define MSLOW 1 /* slowed monster */
#define MFAST 2 /* speeded monster */

#define MON_WEP(mon) ((mon)->mw)
#define MON_NOWEP(mon) ((mon)->mw = (struct obj *) 0)

#define DEADMONSTER(mon) ((mon)->mhp < 1)
#define is_starting_pet(mon) ((mon)->m_id == context.startingpet_mid)
#define any_spec_used(mon) ((mon)->mspec_used || (mon)->mmagespell_used || (mon)->mmageintermediate_used || (mon)->mmageultimate_used \
  || (mon)->mclericspell_used || (mon)->mclericintermediate_used || (mon)->mclericultimate_used || (mon)->mdemonsummon_used \
  || (mon)->mspecialsummon_used || (mon)->mspecialsummon2_used)

#define cursed_items_are_positive_mon(mon) \
    (cursed_items_are_positive((mon)->data) || ((mon)->cham >= LOW_PM && cursed_items_are_positive(&mons[(mon)->cham])))


/* mimic appearances that block vision/light */
#define is_lightblocker_mappear(mon)                       \
    (is_obj_mappear(mon, BOULDER)                          \
     || (M_AP_TYPE(mon) == M_AP_FURNITURE                    \
         && ((mon)->mappearance == S_hcdoor                \
             || (mon)->mappearance == S_vcdoor             \
             || (mon)->mappearance < S_ndoor /* = walls */ \
             || (mon)->mappearance == S_tree)))
#define is_door_mappear(mon) (M_AP_TYPE(mon) == M_AP_FURNITURE   \
                              && ((mon)->mappearance == S_hcdoor \
                                  || (mon)->mappearance == S_vcdoor))
#define is_obj_mappear(mon,otyp) (M_AP_TYPE(mon) == M_AP_OBJECT \
                                  && (mon)->mappearance == (otyp))

#define check_mon_wearable_items_next_turn(mon) (mon)->worn_item_flags |= I_SPECIAL

#endif /* MONST_H */

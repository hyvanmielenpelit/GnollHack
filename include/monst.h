/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

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
    xchar yell_x, yell_y;   /* location where the pet heard you yelling from */
    uchar action;           /* the monster is currently in the midst of one of its attacks or actions */

    unsigned mappearance;   /* for undetected mimics and the wiz */
    uchar m_ap_type;        /* what mappearance is describing, m_ap_types */

    schar mtame;            /* level of tameness, implies peaceful */

    unsigned short mprops[MAX_PROPS];       /* simplified props structure for monsters, taking 1/6 u.uprops memory space but doing about the same thing anyway --JG */

#define M_TIMEOUT                0x3fff     /* timeout mask for temporary property */
#define M_INTRINSIC_ACQUIRED     0x4000     /* permanent intrisic acquired e.g. by eating monster corpses */
#define M_EXTRINSIC              0x8000     /* extrinsic from items or environment, updated frequently by clearing and checking a new value */

    unsigned short mspec_used;              /* monster's general special ability and spell attack timeout */
    unsigned short mmagespell_used;         /* monster's mage-style normal ability or low-powered spell timeout */
    unsigned short mmageintermediate_used;  /* monster's mage-style intermediate ability or medium-powered mage spell timeout */
    unsigned short mmageultimate_used;      /* monster's mage-style ultimate ability or high-powered mage spell timeout */
    unsigned short mclericspell_used;       /* monster's cleric-style normal ability or low-powered spell timeout */
    unsigned short mclericintermediate_used;/* monster's cleric-style intermediate ability or medium-powered spell timeout */
    unsigned short mclericultimate_used;    /* monster's cleric-style ultimate ability or high-powered mage spell timeout */
    unsigned short mdemonsummon_used;       /* demon summon timeout */
    unsigned short mspecialsummon_used;     /* special (nondemon) summon timeout */
    unsigned short mspecialsummon2_used;    /* another special (nondemon) summon timeout */

    short mcomingtou;
    short notalktimer;
    short notraveltimer;
    short reserved_short1;  
    short reserved_short2;
    short rumorsleft;           /* how many rumors the monster still knows, -1 means that the monster has already told the player that it does not know any more rumors */
    short mflee_timer;          /* timeout for mflee */
    short mfrozen;
    short mstaying;             /* commanded to stay in place, similar to frozen, but commanded */
    short mcarrying;

    /* unsigned int to align the following bitfields reliably across platforms */
    unsigned mrevived;          /* Number of times revived */

    /* Bitfield flags -- Keep all bitfields in a row */
    uint64_t bitflags;
    uint64_t bitflags2;


    uchar wormno;                        /* at most 31 worms on any level */
#define MAX_NUM_WORMS 32                        /* should be 2^(wormno bitfield size) */
    uchar heads_tamed;                   /* How many Cerberos's heads have been tamed; anything else for other monsters */

    int reserved_index;              /* Special general purpose index */
    int meating;                     /* monster is eating timeout */

    uchar talkstate_item_trading;    /* 1 = has said introduction, 2 = has said non-repeatable secondary question, 3 = has said first repeatable confirmatory question,  4 = has said second repeatable confirmatory question */
    uchar talkstate_special;         /* Special index, e.g., for Aleax */
    xchar timed;                     /* # of fuses (timers) attached to this monst */
    xchar weapon_strategy;           /* flag for whether to try switching weapons */

    uint64_t mon_flags;              /* General easy-to-add flags for monsters for things not covered by the above bitfields */
#define MON_FLAGS_NONE                          0x00000000UL
#define MON_FLAGS_RWRAITH                       0x00000001UL
#define MON_FLAGS_CHAINED                       0x00000002UL
#define MON_FLAGS_CLONED_WIZ                    0x00000004UL
#define MON_FLAGS_SUMMONED_AT_ALTAR             0x00000008UL
#define MON_FLAGS_SPLEVEL_RESIDENT              0x00000010UL
#define MON_FLAGS_YOUR_CHILD                    0x00000020UL
#define MON_FLAGS_SPOTTED_IN_RUN_AT_START       0x00000040UL /* Spotted for purposes of determining whether to stop travel, run and similar commands; determined when starting running */
#define MON_FLAGS_LEVEL_BOSS                    0x00000080UL
#define MON_FLAGS_BOSS_HOSTILITY                0x00000100UL
#define MON_FLAGS_VORPAL_WARNING_GIVEN          0x00000200UL
#define MON_FLAGS_SCHROEDINGERS_CAT             0x00000400UL
#define MON_FLAGS_DEBUG_REPLMON                 0x00000800UL
#define MON_FLAGS_DEBUG_DEALLOCATED             0x00001000UL
#define MON_FLAGS_ORC_AND_A_PIE                 0x00002000UL
#define MON_FLAGS_ADD_UNSUMMON_TIMER            0x00004000UL

    uint64_t mstrategy; /* for monsters with mflag3: current strategy */

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

    int64_t mtrapseen;          /* bitmap of traps we've been trapped in */
    int64_t mlstmv;             /* for catching up with lost time */
    int64_t mspare1;
    int64_t worn_item_flags;    /* mon's wornmask */
    int64_t summonduration;     /* duration for summoned units */
    int64_t extra_encounter_xp; /* extra experience yielded by this monster due to encounter difficulty */

    int glyph;
    int gui_glyph;

    struct obj* minvent;        /* mon's inventory */
    struct obj* mw;             /* mon's weapon */
    struct mextra *mextra;      /* point to mextra struct */
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


/* Monster bitflags */
#define MONFLAG_MFLEE                    0x00000001UL
#define MONFLAG_MSLEEPING                0x00000002UL
#define MONFLAG_MCANMOVE                 0x00000004UL
#define MONFLAG_MWANTSTOMOVE             0x00000008UL
#define MONFLAG_MWANTSTODROP             0x00000010UL
#define MONFLAG_FEMALE                   0x00000020UL
#define MONFLAG_MBURIED                  0x00000040UL
#define MONFLAG_MUNDETECTED              0x00000080UL
#define MONFLAG_MCLONED                  0x00000100UL
#define MONFLAG_MAVENGE                  0x00000200UL
#define MONFLAG_MPEACEFUL                0x00000400UL
#define MONFLAG_MTRAPPED                 0x00000800UL
#define MONFLAG_MLEASHED                 0x00001000UL
#define MONFLAG_ISSHK                    0x00002000UL
#define MONFLAG_ISMINION                 0x00004000UL
#define MONFLAG_ISGD                     0x00008000UL
#define MONFLAG_ISPRIEST                 0x00010000UL
#define MONFLAG_ISSMITH                  0x00020000UL
#define MONFLAG_ISNPC                    0x00040000UL
#define MONFLAG_ISSUMMONED               0x00080000UL
#define MONFLAG_DISREGARDS_ENEMY_STRENGTH 0x00100000UL
#define MONFLAG_DISREGARDS_OWN_HEALTH    0x00200000UL
#define MONFLAG_HASBLOODLUST             0x00400000UL
#define MONFLAG_ISPACIFIST               0x00800000UL
#define MONFLAG_ISFAITHFUL               0x01000000UL
#define MONFLAG_ISPROTECTOR              0x02000000UL
#define MONFLAG_ISPARTYMEMBER            0x04000000UL
#define MONFLAG_LEAVES_NO_CORPSE         0x08000000UL
#define MONFLAG_DELAYED_KILLER_BY_YOU    0x10000000UL
#define MONFLAG_U_KNOW_MNAME             0x20000000UL
#define MONFLAG_TOLD_RUMOR               0x40000000UL
#define MONFLAG_FACING_RIGHT             0x80000000UL
#define MONFLAGS2_ISWIZ                  0x00000001UL
#define MONFLAGS2_BOSS_FIGHT_STARTED     0x00000002UL
#define MONFLAGS2_SPECIAL_TALK_FLAG1     0x00000004UL
#define MONFLAGS2_SPECIAL_TALK_FLAG2     0x00000008UL
#define MONFLAGS2_SPECIAL_TALK_FLAG3     0x00000010UL
#define MONFLAGS2_SPECIAL_TALK_FLAG4     0x00000020UL
#define MONFLAGS2_SPECIAL_TALK_FLAG5     0x00000040UL
#define MONFLAGS2_SPECIAL_TALK_FLAG6     0x00000080UL
#define MONFLAGS2_SPECIAL_TALK_FLAG7     0x00000100UL
#define MONFLAGS2_SPECIAL_TALK_FLAG8     0x00000200UL

#define is_mon_mflee(m) (((m)->bitflags & MONFLAG_MFLEE) != 0)
#define set_mon_mflee(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MFLEE) : ((m)->bitflags &= ~MONFLAG_MFLEE))
#define toggle_mon_mflee(m) ((m)->bitflags ^= MONFLAG_MFLEE)
#define is_mon_msleeping(m) (((m)->bitflags & MONFLAG_MSLEEPING) != 0)
#define set_mon_msleeping(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MSLEEPING) : ((m)->bitflags &= ~MONFLAG_MSLEEPING))
#define toggle_mon_msleeping(m) ((m)->bitflags ^= MONFLAG_MSLEEPING)
#define is_mon_mcanmove(m) (((m)->bitflags & MONFLAG_MCANMOVE) != 0)
#define set_mon_mcanmove(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MCANMOVE) : ((m)->bitflags &= ~MONFLAG_MCANMOVE))
#define toggle_mon_mcanmove(m) ((m)->bitflags ^= MONFLAG_MCANMOVE)
#define is_mon_mwantstomove(m) (((m)->bitflags & MONFLAG_MWANTSTOMOVE) != 0)
#define set_mon_mwantstomove(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MWANTSTOMOVE) : ((m)->bitflags &= ~MONFLAG_MWANTSTOMOVE))
#define toggle_mon_mwantstomove(m) ((m)->bitflags ^= MONFLAG_MWANTSTOMOVE)
#define is_mon_mwantstodrop(m) (((m)->bitflags & MONFLAG_MWANTSTODROP) != 0)
#define set_mon_mwantstodrop(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MWANTSTODROP) : ((m)->bitflags &= ~MONFLAG_MWANTSTODROP))
#define toggle_mon_mwantstodrop(m) ((m)->bitflags ^= MONFLAG_MWANTSTODROP)
#define is_mon_female(m) (((m)->bitflags & MONFLAG_FEMALE) != 0)
#define set_mon_female(m, val) ((val) ? ((m)->bitflags |= MONFLAG_FEMALE) : ((m)->bitflags &= ~MONFLAG_FEMALE))
#define toggle_mon_female(m) ((m)->bitflags ^= MONFLAG_FEMALE)
#define is_mon_mburied(m) (((m)->bitflags & MONFLAG_MBURIED) != 0)
#define set_mon_mburied(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MBURIED) : ((m)->bitflags &= ~MONFLAG_MBURIED))
#define toggle_mon_mburied(m) ((m)->bitflags ^= MONFLAG_MBURIED)
#define is_mon_mundetected(m) (((m)->bitflags & MONFLAG_MUNDETECTED) != 0)
#define set_mon_mundetected(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MUNDETECTED) : ((m)->bitflags &= ~MONFLAG_MUNDETECTED))
#define toggle_mon_mundetected(m) ((m)->bitflags ^= MONFLAG_MUNDETECTED)
#define is_mon_mcloned(m) (((m)->bitflags & MONFLAG_MCLONED) != 0)
#define set_mon_mcloned(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MCLONED) : ((m)->bitflags &= ~MONFLAG_MCLONED))
#define toggle_mon_mcloned(m) ((m)->bitflags ^= MONFLAG_MCLONED)
#define is_mon_mavenge(m) (((m)->bitflags & MONFLAG_MAVENGE) != 0)
#define set_mon_mavenge(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MAVENGE) : ((m)->bitflags &= ~MONFLAG_MAVENGE))
#define toggle_mon_mavenge(m) ((m)->bitflags ^= MONFLAG_MAVENGE)
#define is_mon_mpeaceful(m) (((m)->bitflags & MONFLAG_MPEACEFUL) != 0)
#define set_mon_mpeaceful(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MPEACEFUL) : ((m)->bitflags &= ~MONFLAG_MPEACEFUL))
#define toggle_mon_mpeaceful(m) ((m)->bitflags ^= MONFLAG_MPEACEFUL)
#define is_mon_mtrapped(m) (((m)->bitflags & MONFLAG_MTRAPPED) != 0)
#define set_mon_mtrapped(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MTRAPPED) : ((m)->bitflags &= ~MONFLAG_MTRAPPED))
#define toggle_mon_mtrapped(m) ((m)->bitflags ^= MONFLAG_MTRAPPED)
#define is_mon_mleashed(m) (((m)->bitflags & MONFLAG_MLEASHED) != 0)
#define set_mon_mleashed(m, val) ((val) ? ((m)->bitflags |= MONFLAG_MLEASHED) : ((m)->bitflags &= ~MONFLAG_MLEASHED))
#define toggle_mon_mleashed(m) ((m)->bitflags ^= MONFLAG_MLEASHED)
#define is_mon_isshk(m) (((m)->bitflags & MONFLAG_ISSHK) != 0)
#define set_mon_isshk(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISSHK) : ((m)->bitflags &= ~MONFLAG_ISSHK))
#define toggle_mon_isshk(m) ((m)->bitflags ^= MONFLAG_ISSHK)
#define is_mon_isminion(m) (((m)->bitflags & MONFLAG_ISMINION) != 0)
#define set_mon_isminion(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISMINION) : ((m)->bitflags &= ~MONFLAG_ISMINION))
#define toggle_mon_isminion(m) ((m)->bitflags ^= MONFLAG_ISMINION)
#define is_mon_isgd(m) (((m)->bitflags & MONFLAG_ISGD) != 0)
#define set_mon_isgd(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISGD) : ((m)->bitflags &= ~MONFLAG_ISGD))
#define toggle_mon_isgd(m) ((m)->bitflags ^= MONFLAG_ISGD)
#define is_mon_ispriest(m) (((m)->bitflags & MONFLAG_ISPRIEST) != 0)
#define set_mon_ispriest(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISPRIEST) : ((m)->bitflags &= ~MONFLAG_ISPRIEST))
#define toggle_mon_ispriest(m) ((m)->bitflags ^= MONFLAG_ISPRIEST)
#define is_mon_issmith(m) (((m)->bitflags & MONFLAG_ISSMITH) != 0)
#define set_mon_issmith(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISSMITH) : ((m)->bitflags &= ~MONFLAG_ISSMITH))
#define toggle_mon_issmith(m) ((m)->bitflags ^= MONFLAG_ISSMITH)
#define is_mon_isnpc(m) (((m)->bitflags & MONFLAG_ISNPC) != 0)
#define set_mon_isnpc(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISNPC) : ((m)->bitflags &= ~MONFLAG_ISNPC))
#define toggle_mon_isnpc(m) ((m)->bitflags ^= MONFLAG_ISNPC)
#define is_mon_issummoned(m) (((m)->bitflags & MONFLAG_ISSUMMONED) != 0)
#define set_mon_issummoned(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISSUMMONED) : ((m)->bitflags &= ~MONFLAG_ISSUMMONED))
#define toggle_mon_issummoned(m) ((m)->bitflags ^= MONFLAG_ISSUMMONED)
#define is_mon_disregards_enemy_strength(m) (((m)->bitflags & MONFLAG_DISREGARDS_ENEMY_STRENGTH) != 0)
#define set_mon_disregards_enemy_strength(m, val) ((val) ? ((m)->bitflags |= MONFLAG_DISREGARDS_ENEMY_STRENGTH) : ((m)->bitflags &= ~MONFLAG_DISREGARDS_ENEMY_STRENGTH))
#define toggle_mon_disregards_enemy_strength(m) ((m)->bitflags ^= MONFLAG_DISREGARDS_ENEMY_STRENGTH)
#define is_mon_disregards_own_health(m) (((m)->bitflags & MONFLAG_DISREGARDS_OWN_HEALTH) != 0)
#define set_mon_disregards_own_health(m, val) ((val) ? ((m)->bitflags |= MONFLAG_DISREGARDS_OWN_HEALTH) : ((m)->bitflags &= ~MONFLAG_DISREGARDS_OWN_HEALTH))
#define toggle_mon_disregards_own_health(m) ((m)->bitflags ^= MONFLAG_DISREGARDS_OWN_HEALTH)
#define is_mon_hasbloodlust(m) (((m)->bitflags & MONFLAG_HASBLOODLUST) != 0)
#define set_mon_hasbloodlust(m, val) ((val) ? ((m)->bitflags |= MONFLAG_HASBLOODLUST) : ((m)->bitflags &= ~MONFLAG_HASBLOODLUST))
#define toggle_mon_hasbloodlust(m) ((m)->bitflags ^= MONFLAG_HASBLOODLUST)
#define is_mon_ispacifist(m) (((m)->bitflags & MONFLAG_ISPACIFIST) != 0)
#define set_mon_ispacifist(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISPACIFIST) : ((m)->bitflags &= ~MONFLAG_ISPACIFIST))
#define toggle_mon_ispacifist(m) ((m)->bitflags ^= MONFLAG_ISPACIFIST)
#define is_mon_isfaithful(m) (((m)->bitflags & MONFLAG_ISFAITHFUL) != 0)
#define set_mon_isfaithful(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISFAITHFUL) : ((m)->bitflags &= ~MONFLAG_ISFAITHFUL))
#define toggle_mon_isfaithful(m) ((m)->bitflags ^= MONFLAG_ISFAITHFUL)
#define is_mon_isprotector(m) (((m)->bitflags & MONFLAG_ISPROTECTOR) != 0)
#define set_mon_isprotector(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISPROTECTOR) : ((m)->bitflags &= ~MONFLAG_ISPROTECTOR))
#define toggle_mon_isprotector(m) ((m)->bitflags ^= MONFLAG_ISPROTECTOR)
#define is_mon_ispartymember(m) (((m)->bitflags & MONFLAG_ISPARTYMEMBER) != 0)
#define set_mon_ispartymember(m, val) ((val) ? ((m)->bitflags |= MONFLAG_ISPARTYMEMBER) : ((m)->bitflags &= ~MONFLAG_ISPARTYMEMBER))
#define toggle_mon_ispartymember(m) ((m)->bitflags ^= MONFLAG_ISPARTYMEMBER)
#define is_mon_leaves_no_corpse(m) (((m)->bitflags & MONFLAG_LEAVES_NO_CORPSE) != 0)
#define set_mon_leaves_no_corpse(m, val) ((val) ? ((m)->bitflags |= MONFLAG_LEAVES_NO_CORPSE) : ((m)->bitflags &= ~MONFLAG_LEAVES_NO_CORPSE))
#define toggle_mon_leaves_no_corpse(m) ((m)->bitflags ^= MONFLAG_LEAVES_NO_CORPSE)
#define is_mon_delayed_killer_by_you(m) (((m)->bitflags & MONFLAG_DELAYED_KILLER_BY_YOU) != 0)
#define set_mon_delayed_killer_by_you(m, val) ((val) ? ((m)->bitflags |= MONFLAG_DELAYED_KILLER_BY_YOU) : ((m)->bitflags &= ~MONFLAG_DELAYED_KILLER_BY_YOU))
#define toggle_mon_delayed_killer_by_you(m) ((m)->bitflags ^= MONFLAG_DELAYED_KILLER_BY_YOU)
#define is_mon_u_know_mname(m) (((m)->bitflags & MONFLAG_U_KNOW_MNAME) != 0)
#define set_mon_u_know_mname(m, val) ((val) ? ((m)->bitflags |= MONFLAG_U_KNOW_MNAME) : ((m)->bitflags &= ~MONFLAG_U_KNOW_MNAME))
#define toggle_mon_u_know_mname(m) ((m)->bitflags ^= MONFLAG_U_KNOW_MNAME)
#define is_mon_told_rumor(m) (((m)->bitflags & MONFLAG_TOLD_RUMOR) != 0)
#define set_mon_told_rumor(m, val) ((val) ? ((m)->bitflags |= MONFLAG_TOLD_RUMOR) : ((m)->bitflags &= ~MONFLAG_TOLD_RUMOR))
#define toggle_mon_told_rumor(m) ((m)->bitflags ^= MONFLAG_TOLD_RUMOR)
#define is_mon_facing_right(m) (((m)->bitflags & MONFLAG_FACING_RIGHT) != 0)
#define set_mon_facing_right(m, val) ((val) ? ((m)->bitflags |= MONFLAG_FACING_RIGHT) : ((m)->bitflags &= ~MONFLAG_FACING_RIGHT))
#define toggle_mon_facing_right(m) ((m)->bitflags ^= MONFLAG_FACING_RIGHT)
#define is_mon_iswiz(m) (((m)->bitflags2 & MONFLAGS2_ISWIZ) != 0)
#define set_mon_iswiz(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_ISWIZ) : ((m)->bitflags2 &= ~MONFLAGS2_ISWIZ))
#define toggle_mon_iswiz(m) ((m)->bitflags2 ^= MONFLAGS2_ISWIZ)
#define is_mon_boss_fight_started(m) (((m)->bitflags2 & MONFLAGS2_BOSS_FIGHT_STARTED) != 0)
#define set_mon_boss_fight_started(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_BOSS_FIGHT_STARTED) : ((m)->bitflags2 &= ~MONFLAGS2_BOSS_FIGHT_STARTED))
#define toggle_mon_boss_fight_started(m) ((m)->bitflags2 ^= MONFLAGS2_BOSS_FIGHT_STARTED)
#define is_mon_special_talk_flag1(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG1) != 0)
#define set_mon_special_talk_flag1(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG1) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG1))
#define toggle_mon_special_talk_flag1(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG1)
#define is_mon_special_talk_flag2(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG2) != 0)
#define set_mon_special_talk_flag2(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG2) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG2))
#define toggle_mon_special_talk_flag2(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG2)
#define is_mon_special_talk_flag3(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG3) != 0)
#define set_mon_special_talk_flag3(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG3) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG3))
#define toggle_mon_special_talk_flag3(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG3)
#define is_mon_special_talk_flag4(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG4) != 0)
#define set_mon_special_talk_flag4(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG4) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG4))
#define toggle_mon_special_talk_flag4(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG4)
#define is_mon_special_talk_flag5(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG5) != 0)
#define set_mon_special_talk_flag5(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG5) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG5))
#define toggle_mon_special_talk_flag5(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG5)
#define is_mon_special_talk_flag6(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG6) != 0)
#define set_mon_special_talk_flag6(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG6) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG6))
#define toggle_mon_special_talk_flag6(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG6)
#define is_mon_special_talk_flag7(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG7) != 0)
#define set_mon_special_talk_flag7(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG7) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG7))
#define toggle_mon_special_talk_flag7(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG7)
#define is_mon_special_talk_flag8(m) (((m)->bitflags2 & MONFLAGS2_SPECIAL_TALK_FLAG8) != 0)
#define set_mon_special_talk_flag8(m, val) ((val) ? ((m)->bitflags2 |= MONFLAGS2_SPECIAL_TALK_FLAG8) : ((m)->bitflags2 &= ~MONFLAGS2_SPECIAL_TALK_FLAG8))
#define toggle_mon_special_talk_flag8(m) ((m)->bitflags2 ^= MONFLAGS2_SPECIAL_TALK_FLAG8)

#endif /* MONST_H */

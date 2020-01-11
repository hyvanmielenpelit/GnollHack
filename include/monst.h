/* GnollHack 4.0	monst.h	$NHDT-Date: 1550524559 2019/02/18 21:15:59 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.28 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2016. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MONST_H
#define MONST_H

#ifndef MEXTRA_H
#include "mextra.h"
#endif

#ifndef PROP_H
#include "prop.h"
#endif

#ifndef ATTRIB_H
#include "attrib.h"
#endif

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
    NEED_PICK_AXE       = 4,
    NEED_AXE            = 5,
    NEED_PICK_OR_AXE    = 6
};

/* The following flags are used for the second argument to display_minventory
 * in invent.c:
 *
 * PICK_NONE, PICK_ONE, PICK_ANY (wintype.h)  0, 1, 2
 * MINV_NOLET  If set, don't display inventory letters on monster's inventory.
 * MINV_ALL    If set, display all items in monster's inventory, otherwise
 *	       just display wielded weapons and worn items.
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

struct monst {
    struct monst *nmon;
    struct permonst *data;
    unsigned m_id;
    short mnum;           /* permanent monster index number */
    short cham;           /* if shapeshifter, orig mons[] idx goes here */
    short movement;       /* movement points (derived from permonst definition
                             and added effects */
    uchar m_lev;          /* adjusted difficulty level of monster */
    aligntyp malign;      /* alignment of this monster, relative to the
                             player (positive = good to kill) */
    xchar mx, my;
    xchar mux, muy;       /* where the monster thinks you are */
#define MTSZ 4
    coord mtrack[MTSZ];   /* monster track */

	struct attribs 
		acurr,			/* monster's current attributes (eg. str)*/
		abonus,			/* monster's bonus attributes (eg. str) */
		afixmin,		/* monster's fixed min attributes (eg. str), set by an item */
		afixmax,		/* monster's fixed max attributes (eg. str), set by an item */
		amax,			/* monster's max attributes (eg. str) */
		atemp,			/* used for temporary loss/gain */
		atime;			/* used for loss/gain countdown */

	schar mdaminc;
	schar mhitinc;
	schar macbonus;
	schar mmcbonus;

	int mhp, mbasehpmax, mhpmax;
    unsigned mappearance; /* for undetected mimics and the wiz */
    uchar m_ap_type;      /* what mappearance is describing, m_ap_types */

    schar mtame;                /* level of tameness, implies peaceful */

	unsigned short mprops[LAST_PROP + 1]; /* simplified props structure for monsters, taking 1/6 u.uprops memory space but doing about the same thing anyway --JG */

#define M_TIMEOUT				0x3fff	/* timeout mask for temporary property */
#define M_INTRINSIC_ACQUIRED	0x4000  /* permanent intrisic acquired e.g. by eating monster corpses */
#define M_EXTRINSIC				0x8000  /* extrinsic from items or environment, updated frequently by clearing and checking a new value */

	int mspec_used;             /* monster's special ability attack timeout */

    Bitfield(female, 1);		/* is female */
	schar morigtame;			/* tameness before charming */
	Bitfield(morigpeaceful, 1);	/* peacefulness before charming */
	Bitfield(mburied, 1);		/* has been buried */
    Bitfield(mundetected, 1);	/* not seen in present hiding place;
								 * implies one of M1_CONCEAL or M1_HIDE,
                               * but not mimic (that is, snake, spider,
                               * trapper, piercer, eel)
                               */
    Bitfield(mrevived, 1);  /* has been revived from the dead */
    Bitfield(mcloned, 1);   /* has been cloned from another */
    Bitfield(mavenge, 1);   /* did something to deserve retaliation */
    Bitfield(mflee, 1);     /* fleeing */

    Bitfield(mflee_timer, 7);  /* timeout for mflee */
    Bitfield(msleeping, 1); /* asleep until woken */

    Bitfield(mfrozen, 7);
    Bitfield(mcanmove, 1);  /* paralysis, similar to mblinded */

	Bitfield(mstaying, 7);	/* commanded to stay in place, similar to frozen, but commanded */
	Bitfield(mwantstomove, 1);  /* mon wants to move, not staying in place */

	Bitfield(mcarrying, 7);	
	Bitfield(mwantstodrop, 1);

    Bitfield(mpeaceful, 1); /* does not attack unprovoked */
    Bitfield(mtrapped, 1);  /* trapped in a pit, web or bear trap */
    Bitfield(mleashed, 1);  /* monster is on a leash */
    Bitfield(isshk, 1);     /* is shopkeeper */
    Bitfield(isminion, 1);  /* is a minion */
    Bitfield(isgd, 1);      /* is guard */
    Bitfield(ispriest, 1);  /* is an aligned priest or high priest */
	Bitfield(issummoned, 1);					/* is a summoned monster */
	Bitfield(disregards_enemy_strength, 1);		/* the monster attacks too strong enemies */
	Bitfield(disregards_own_health, 1);			/* the monster attacks even when its health is low */
	Bitfield(hasbloodlust, 1);					/* attacks also peaceful */
	Bitfield(ispacifist, 1);					/* does not attack peaceful, NOW DEACTIVATED, APPLIES TO ALL */
	Bitfield(isfaithful, 1);					/* being separate from the owner does not reduce tameness */
	Bitfield(ispartymember, 1);					/* a peaceful monster that has joined your party (e.g., does not give you the money or items back from its inventory) */
	Bitfield(talkstate, 3);						/* 1 = has said introduction, 2 = has said non-repeatable secondary question, 3 = has said first repeatable confirmatory question,  4 = has said second repeatable confirmatory question */
	Bitfield(leaves_no_corpse, 1);				/* this particular monster does not leave a corpse */
	Bitfield(delayed_killer_by_you, 1);			/* is petrification or other delayed killer initiated by you */
	int notalktimer;
	Bitfield(u_know_mname, 1); /* you know the monster's name */
	int max_hp_percentage;						/* % of the monster's maximum theoretical hit points at the time of creation */

    Bitfield(iswiz, 1);     /* is the Wizard of Yendor */
    Bitfield(wormno, 5);    /* at most 31 worms on any level */
    /* 2 free bits */

#define MAX_NUM_WORMS 32    /* should be 2^(wormno bitfield size) */

    unsigned long mstrategy; /* for monsters with mflag3: current strategy */
#ifdef NHSTDC
#define STRAT_APPEARMSG 0x80000000UL
#else
#define STRAT_APPEARMSG 0x80000000L
#endif
#define STRAT_ARRIVE    0x40000000L /* just arrived on current level */
#define STRAT_WAITFORU  0x20000000L
#define STRAT_CLOSE     0x10000000L
#define STRAT_WAITMASK  (STRAT_CLOSE | STRAT_WAITFORU)
#define STRAT_HEAL      0x08000000L
#define STRAT_GROUND    0x04000000L
#define STRAT_MONSTR    0x02000000L
#define STRAT_PLAYER    0x01000000L
#define STRAT_NONE      0x00000000L
#define STRAT_STRATMASK 0x0f000000L
#define STRAT_XMASK     0x00ff0000L
#define STRAT_YMASK     0x0000ff00L
#define STRAT_GOAL      0x000000ffL
#define STRAT_GOALX(s) ((xchar) ((s & STRAT_XMASK) >> 16))
#define STRAT_GOALY(s) ((xchar) ((s & STRAT_YMASK) >> 8))

    long mtrapseen;        /* bitmap of traps we've been trapped in */
    long mlstmv;           /* for catching up with lost time */
    long mspare1;
    struct obj *minvent;   /* mon's inventory */

    struct obj *mw;        /* mon's weapon */
    long worn_item_flags;  /* mon's wornmask */
    xchar weapon_strategy;    /* flag for whether to try switching weapons */

    int meating;           /* monster is eating timeout */
	long summonduration;   /* duration for summoned units */
	long extra_encounter_xp; /* extra experience yielded by this monster due to encounter difficulty */
	xchar timed;		   /* # of fuses (timers) attached to this monst */
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

#endif /* MONST_H */

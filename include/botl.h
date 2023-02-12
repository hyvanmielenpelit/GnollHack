/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0  botl.h  $NHDT-Date: 1554591222 2019/04/06 22:53:42 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.24 $ */
/* Copyright (c) Michael Allison, 2003                            */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef BOTL_H
#define BOTL_H

/* MAXCO must hold longest uncompressed status line, and must be larger
 * than COLNO
 *
 * longest practical second status line at the moment is
Astral Plane \GXXXXNNNN:123456 HP:1234(1234) Pw:1234(1234) AC:-127
 Xp:30/123456789 T:123456  Stone Slime Strngl FoodPois TermIll
 Satiated Overloaded Blind Deaf Stun Conf Hallu Lev Ride
 * -- or about 185 characters.  '$' gets encoded even when it
 * could be used as-is.  The first five status conditions are fatal
 * so it's rare to have more than one at a time.
 *
 * When the full line is wider than the map, the basic status line
 * formatting will move less important fields to the end, so if/when
 * truncation is necessary, it will chop off the least significant
 * information.
 */
#if COLNO <= 160
#define MAXCO 200
#else
#define MAXCO (COLNO + 40)
#endif

struct condmap {
    const char *id;
    unsigned long bitmask;
};

enum statusfields {
    BL_CHARACTERISTICS = -3, /* alias for BL_STR..BL_CH */
    BL_RESET = -2,           /* Force everything to redisplay */
    BL_FLUSH = -1,           /* Finished cycling through bot fields */
    BL_TITLE = 0,
    BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH, BL_GOLD, BL_ALIGN,
    BL_SCORE, BL_CAP, BL_ENE, BL_ENEMAX, 
    BL_XP, BL_2WEP, BL_SKILL, BL_AC, BL_MC_LVL, BL_MC_PCT, BL_MOVE, BL_UWEP, BL_UWEP2, BL_HD, BL_TIME, BL_REALTIME, BL_HUNGER, BL_HP,
    BL_HPMAX, BL_MODE, BL_LEVELDESC, BL_EXP, BL_CONDITION,
    BL_PARTYSTATS, BL_PARTYSTATS2, BL_PARTYSTATS3, BL_PARTYSTATS4, BL_PARTYSTATS5,
    MAXBLSTATS
};

enum relationships { NO_LTEQGT = -1,
                     EQ_VALUE, LT_VALUE, LE_VALUE,
                     GE_VALUE, GT_VALUE, TXT_VALUE };

#define BEFORE  0
#define NOW     1

/* Boolean condition bits for the condition mask */

/* clang-format off */
enum bl_conditions {
    BL_COND_STONE = 0,
    BL_COND_SLIME,
    BL_COND_STRNGL,
    BL_COND_SUFFOC,
    BL_COND_FOODPOIS,
    BL_COND_TERMILL,
    BL_COND_BLIND,
    BL_COND_DEAF,
    BL_COND_STUN,
    BL_COND_CONF,
    BL_COND_HALLU,
    BL_COND_LEV,
    BL_COND_FLY,
    BL_COND_RIDE,
    BL_COND_SLOWED,
    BL_COND_PARALYZED,
    BL_COND_FEARFUL,
    BL_COND_SLEEPING,
    BL_COND_CANCELLED,
    BL_COND_SILENCED,
    BL_COND_GRAB,
    BL_COND_ROT,
    BL_COND_LYCANTHROPY,
    NUM_BL_CONDITIONS
};

#define BL_MASK_STONE           (1UL << BL_COND_STONE) // 0x00000001UL
#define BL_MASK_SLIME           (1UL << BL_COND_SLIME) // 0x00000002UL
#define BL_MASK_STRNGL          (1UL << BL_COND_STRNGL) // 0x00000004UL
#define BL_MASK_SUFFOC          (1UL << BL_COND_SUFFOC) // 0x00000008UL
#define BL_MASK_FOODPOIS        (1UL << BL_COND_FOODPOIS) // 0x00000010UL
#define BL_MASK_TERMILL         (1UL << BL_COND_TERMILL) // 0x00000020UL
#define BL_MASK_BLIND           (1UL << BL_COND_BLIND) // 0x00000040UL
#define BL_MASK_DEAF            (1UL << BL_COND_DEAF) // 0x00000080UL
#define BL_MASK_STUN            (1UL << BL_COND_STUN) // 0x00000100UL
#define BL_MASK_CONF            (1UL << BL_COND_CONF) // 0x00000200UL
#define BL_MASK_HALLU           (1UL << BL_COND_HALLU) // 0x00000400UL
#define BL_MASK_LEV             (1UL << BL_COND_LEV) // 0x00000800UL
#define BL_MASK_FLY             (1UL << BL_COND_FLY) // 0x00001000UL
#define BL_MASK_RIDE            (1UL << BL_COND_RIDE) // 0x00002000UL
#define BL_MASK_SLOWED          (1UL << BL_COND_SLOWED) // 0x00004000UL
#define BL_MASK_PARALYZED       (1UL << BL_COND_PARALYZED) // 0x00008000UL
#define BL_MASK_FEARFUL         (1UL << BL_COND_FEARFUL) // 0x00010000UL
#define BL_MASK_SLEEPING        (1UL << BL_COND_SLEEPING) // 0x00020000UL
#define BL_MASK_CANCELLED       (1UL << BL_COND_CANCELLED) // 0x00040000UL
#define BL_MASK_SILENCED        (1UL << BL_COND_SILENCED) // 0x00080000UL
#define BL_MASK_GRAB            (1UL << BL_COND_GRAB) // 0x00100000UL
#define BL_MASK_ROT             (1UL << BL_COND_ROT) // 0x00200000UL
#define BL_MASK_LYCANTHROPY     (1UL << BL_COND_LYCANTHROPY) // 0x00400000UL
#define BL_MASK_BITS            NUM_BL_CONDITIONS /* number of mask bits that can be set */
/* clang-format on */

#define VIA_WINDOWPORT() \
    ((windowprocs.wincap2 & (WC2_HILITE_STATUS | WC2_FLUSH_STATUS)) != 0)

#define REASSESS_ONLY TRUE

/* #ifdef STATUS_HILITES */
/* hilite status field behavior - coloridx values */
#define BL_HILITE_NONE -1    /* no hilite of this field */
#define BL_HILITE_INVERSE -2 /* inverse hilite */
#define BL_HILITE_BOLD -3    /* bold hilite */
                             /* or any CLR_ index (0 - 15) */
#define BL_TH_NONE 0
#define BL_TH_VAL_PERCENTAGE 100 /* threshold is percentage */
#define BL_TH_VAL_ABSOLUTE 101   /* threshold is particular value */
#define BL_TH_UPDOWN 102         /* threshold is up or down change */
#define BL_TH_CONDITION 103      /* threshold is bitmask of conditions */
#define BL_TH_TEXTMATCH 104      /* threshold text value to match against */
#define BL_TH_ALWAYS_HILITE 105  /* highlight regardless of value */


#define HL_ATTCLR_DIM     CLR_MAX + 0
#define HL_ATTCLR_BLINK   CLR_MAX + 1
#define HL_ATTCLR_ULINE   CLR_MAX + 2
#define HL_ATTCLR_INVERSE CLR_MAX + 3
#define HL_ATTCLR_BOLD    CLR_MAX + 4
#define BL_ATTCLR_MAX     CLR_MAX + 5

enum hlattribs { HL_UNDEF   = 0x00,
                 HL_NONE    = 0x01,
                 HL_BOLD    = 0x02,
                 HL_INVERSE = 0x04,
                 HL_ULINE   = 0x08,
                 HL_BLINK   = 0x10,
                 HL_DIM     = 0x20 };
/* #endif STATUS_HILITES */

extern const char *status_fieldnames[]; /* in botl.c */

#endif /* BOTL_H */

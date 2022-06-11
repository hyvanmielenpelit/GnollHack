/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    permonst.h    $NHDT-Date: 1539804913 2018/10/17 19:35:13 $  $NHDT-Branch: keni-makedefsm $:$NHDT-Revision: 1.12 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Kenneth Lorber, Kensington, Maryland, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef PERMONST_H
#define PERMONST_H

#include "action.h"
#include "general.h"
#include "soundset.h"

/*    This structure covers all attack forms.
 *    aatyp is the gross attack type (eg. claw, bite, breath, ...)
 *    adtyp is the damage type (eg. physical, fire, cold, spell, ...)
 *    damn is the number of hit dice of damage from the attack.
 *    damd is the number of sides on each die.
 *
 *    Some attacks can do no points of damage.  Additionally, some can
 *    have special effects *and* do damage as well.  If damn and damd
 *    are set, they may have a special meaning.  For example, if set
 *    for a blinding attack, they determine the amount of time blinded.
 */

struct attack {
    uchar aatyp, adtyp;
    short damn, damd, damp;
    short mcadj; /* MC and saving throw adjustment*/
    short mlevel; /* level at which the attack is used */
    short range; /* range of ranged attacks */
    unsigned long aflags; /* any relevant attack flags */
    uchar action_tile; /* Action tile used for the attack */
};

#define ATTKFLAG_MUST_SEE_TARGET 0x00000001

struct action_info
{
    short actions[MAX_ACTION_TILES];
    short statue;
    short corpse;
    short swallow;
};

/*    Weight of some bodies
 */

#define WT_HUMAN 1450
#define WT_ELF 800
#define WT_DRAGON 4500

#ifndef ALIGN_H
#include "align.h"
#endif
#include "monattk.h"
#include "monflag.h"

#define NUM_MFLAGS 8

struct permonst {
    const char *mname;          /* full name */
    const char* mtitle;         /* title or description of a unique monster with proper name, e.g., Prince of Demons or Ixoth, red dragon */
    const char* mdescription;   /* description */
    const char* mfemalename;    /* female name, e.g. dwarf queen vs. dwarf king */
    const char* mcommonname;    /* common name for males and females, e.g. dwarf royal */
    char mlet;                  /* symbol */
    schar mlevel,               /* base monster level */
        mmove,                  /* move speed */
        ac,                     /* (base) armor class */
        mc,                     /* (base) magic cancellation (against magical and supernatural touch attacks) */
        mr;                     /* (base) magic resistance (against all other magic) */
    aligntyp maligntyp;         /* basic monster alignment */
    unsigned long geno;         /* creation/geno mask value */
    struct attack mattk[NATTK]; /* attacks matrix */
    unsigned short cwt,         /* weight of corpse */
        cnutrit;                /* its nutritional value */
    uchar msound;               /* noise it makes (6 bits) */
    uchar msize;                /* physical size (3 bits) */
    uchar heads;                /* number of heads */
    int lightrange;                /* range of emitted light, if any */
    uchar natural_armor_material_type; /* one of the obj_material_types */
    int str,                    /* strength */
        dex,                    /* dexterity */
        con,                    /* constitution */
        intl,                    /* intelligence */
        wis,                    /* wisdom */
        cha;                    /* charisma */
    unsigned long mresists;             /* resistances */
    unsigned long mresists2;             /* more resistances */
    unsigned long mconveys;             /* conveyed by eating */
    unsigned long mflags1,      /* boolean bitflags */
        mflags2, mflags3, mflags4, mflags5, mflags6, mflags7, mflags8;       /* more boolean bitflags, if added, remember to update NUM_MFLAGS above */
    int difficulty;        /* toughness (formerly from  makedefs -m) */
#ifdef TEXTCOLOR
    uchar mcolor; /* color to use */
#endif
    enum monster_soundset_types soundset;
    enum monster_soundset_types female_soundset;

    struct action_info animation;
    struct action_info female_animation;

    struct action_info enlargement;
    struct action_info female_enlargement;

    struct action_info replacement;
    struct action_info female_replacement;
};

extern NEARDATA struct permonst mons[]; /* the master list of monster types */

#define VERY_SLOW_SPEED 3
#define SLOW_SPEED 9
#define NORMAL_SPEED 12 /* movement rates */
#define FAST_SPEED 15
#define VERY_FAST_SPEED 24

#define NON_PM (-1)                  /* "not a monster" */
#define LOW_PM (NON_PM + 1)          /* first monster in mons[] */
#define SPECIAL_PM PM_LONG_WORM_TAIL /* [normal] < ~ < [special] */
/* mons[SPECIAL_PM] through mons[NUM_MONSTERS-1], inclusive, are
   never generated randomly and cannot be polymorphed into */

#endif /* PERMONST_H */

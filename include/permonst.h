/* GnollHack 4.0	permonst.h	$NHDT-Date: 1539804913 2018/10/17 19:35:13 $  $NHDT-Branch: keni-makedefsm $:$NHDT-Revision: 1.12 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Kenneth Lorber, Kensington, Maryland, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef PERMONST_H
#define PERMONST_H

/*	This structure covers all attack forms.
 *	aatyp is the gross attack type (eg. claw, bite, breath, ...)
 *	adtyp is the damage type (eg. physical, fire, cold, spell, ...)
 *	damn is the number of hit dice of damage from the attack.
 *	damd is the number of sides on each die.
 *
 *	Some attacks can do no points of damage.  Additionally, some can
 *	have special effects *and* do damage as well.  If damn and damd
 *	are set, they may have a special meaning.  For example, if set
 *	for a blinding attack, they determine the amount of time blinded.
 */

struct attack {
    uchar aatyp, adtyp;
    int damn, damd, damp;
	int mcadj; /* MC and saving throw adjustment*/
};

/*	Max # of attacks for any given monster.
 */

#define NATTK 8

/*	Weight of some bodies
 */

#define WT_HUMAN 1450
#define WT_ELF 800
#define WT_DRAGON 4500

#ifndef ALIGN_H
#include "align.h"
#endif
#include "monattk.h"
#include "monflag.h"

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
	uchar heads;				/* number of heads */
	int lightrange;				/* range of emitted light, if any */
	int str,					/* strength */
		dex,					/* dexterity */
		con,					/* constitution */
		intl,					/* intelligence */
		wis,					/* wisdom */
		cha;                    /* charisma */
	unsigned long mresists;             /* resistances */
	unsigned long mconveys;             /* conveyed by eating */
    unsigned long mflags1,      /* boolean bitflags */
        mflags2, mflags3, mflags4, mflags5;       /* more boolean bitflags */
    int difficulty;		/* toughness (formerly from  makedefs -m) */
#ifdef TEXTCOLOR
    uchar mcolor; /* color to use */
#endif
    int stand_animation;
    int attack_animation;
    int female_stand_animation;
    int female_attack_animation;
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

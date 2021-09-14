/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    attrib.h    $NHDT-Date: 1432512779 2015/05/25 00:12:59 $  $NHDT-Branch: master $:$NHDT-Revision: 1.8 $ */
/* Copyright 1988, Mike Stephenson                  */
/* GnollHack may be freely redistributed.  See license for details. */

/*    attrib.h - Header file for character class processing. */

#ifndef ATTRIB_H
#define ATTRIB_H

enum attrib_types {
    A_STR = 0,
    A_INT,
    A_WIS,
    A_DEX,
    A_CON,
    A_CHA,

    A_MAX /* used in rn2() selection of attrib */
};

/* For spell attributes */
#define A_MAX_INT_WIS A_MAX
#define A_MAX_INT_CHA A_MAX + 1
#define A_MAX_WIS_CHA A_MAX + 2
#define A_MAX_INT_WIS_CHA A_MAX + 3
#define A_AVG_INT_WIS A_MAX + 4
#define A_AVG_INT_CHA A_MAX + 5
#define A_AVG_WIS_CHA A_MAX + 6
#define A_AVG_INT_WIS_CHA A_MAX + 7

#define ABASE(x) (u.acurr.a[x])
#define ABONUS(x) (u.abonus.a[x])
#define AFIXMIN(x) (u.afixmin.a[x])
#define AFIXMAX(x) (u.afixmax.a[x])
#define AEXE(x) (u.aexe.a[x])
#define ACURR(x) (acurr(x))
#define ACURRSTR (acurrstr())

#define M_ABASE(mon, x) (mon->acurr.a[x])
#define M_ABONUS(mon, x) (mon->abonus.a[x])
#define M_AFIXMIN(mon, x) (mon->afixmin.a[x])
#define M_AFIXMAX(mon, x) (mon->afixmax.a[x])
#define M_AEXE(mon, x) (mon->aexe.a[x])
#define M_ACURR(mon, x) (m_acurr(mon, x))
#define M_ACURRSTR(mon) (m_acurrstr(mon))
#define M_AMIN(mon, x) (mon->amin.a[x])
#define M_AMAX(mon, x) (mon->amax.a[x])
#define M_ATEMP(mon, x) (mon->atemp.a[x])
#define M_ATIME(mon, x) (mon->atime.a[x])
#define M_ATTRMAX(mon, x) monster_attribute_maximum((mon)->data, x)
#define M_ATTRMIN(mon, x) monster_attribute_minimum((mon)->data, x)

/* should be: */
/* #define ACURR(x) (ABONUS(x) + ATEMP(x) + (Upolyd  ? MBASE(x) : ABASE(x)) */
#define MCURR(x) (u.macurr.a[x])
#define AMAX(x) (u.amax.a[x])
#define MMAX(x) (u.mamax.a[x])
#define AMIN(x) (u.amin.a[x])
#define MMIN(x) (u.mamin.a[x])

#define ATEMP(x) (u.atemp.a[x])
#define ATIME(x) (u.atime.a[x])

/* KMH -- Conveniences when dealing with strength constants */
#define STR18(x) (18 + (x))  /* 18/xx */
#define STR19(x) (100 + (x)) /* For 19 and above */

struct attribs {
    schar a[A_MAX];
};

#define ATTRMAX(x) (Upolyd ? monster_attribute_maximum(youmonst.data, x) : urace.attrmax[x])
#define ATTRMIN(x) (Upolyd ? monster_attribute_minimum(youmonst.data, x) : urace.attrmin[x])

struct innate {
    schar ulevel;
    int propid;
    //long *ability;
    const char* gainstr, * losestr;
};

#endif /* ATTRIB_H */

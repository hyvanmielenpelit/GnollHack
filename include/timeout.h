/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    timeout.h    $NHDT-Date: 1432512775 2015/05/25 00:12:55 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright 1994, Dean Luick                      */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef TIMEOUT_H
#define TIMEOUT_H

#include "integer.h"

/* generic timeout function */
typedef void FDECL((*timeout_proc), (ANY_P *, int64_t));

/* kind of timer */
enum timer_type {
    TIMER_LEVEL = 0,   /* event specific to level */
    TIMER_GLOBAL,  /* event follows current play */
    TIMER_OBJECT,  /* event follows a object */
    TIMER_MONSTER /* event follows a monster */
};

/* save/restore timer ranges */
#define RANGE_LEVEL 0  /* save/restore timers staying on level */
#define RANGE_GLOBAL 1 /* save/restore timers following global play */

/*
 * Timeout functions.  Add a define here, then put it in the table
 * in timeout.c.  "One more level of indirection will fix everything."
 */
enum timeout_types {
    ROT_ORGANIC = 0, /* for buried organics */
    ROT_CORPSE,
    REVIVE_MON,
    BURN_OBJECT,
    MAKE_SOUND_OBJECT,
    HATCH_EGG,
    FIG_TRANSFORM,
    MELT_ICE_AWAY,
    ITEM_UNSUMMON,
    MONSTER_UNSUMMON,
    TIME_RESTART,

    NUM_TIME_FUNCS
};

/* used in timeout.c */
typedef struct fe {
    struct fe *next;          /* next item in chain */
    int64_t timeout;             /* when we time out */
    uint64_t tid;        /* timer ID */
    short kind;               /* kind of use */
    short func_index;         /* what to call when we time out */
    anything arg;             /* pointer to timeout argument */
    boolean needs_fixup; /* does arg need to be patched? */
} timer_element;

#endif /* TIMEOUT_H */

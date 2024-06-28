/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    engrave.h    $NHDT-Date: 1432512777 2015/05/25 00:12:57 $  $NHDT-Branch: master $:$NHDT-Revision: 1.8 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Kenneth Lorber, Kensington, Maryland, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef ENGRAVE_H
#define ENGRAVE_H

struct engr {
    struct engr *nxt_engr;
    char *engr_txt;
    xchar engr_x, engr_y;
    size_t engr_lth; /* for save & restore; not length of text */
    int64_t engr_time;    /* moment engraving was (will be) finished */
    xchar engr_type;
#define DUST 1
#define ENGRAVE 2
#define BURN 3
#define MARK 4
#define ENGR_BLOOD 5
#define ENGR_HEADSTONE 6
#define ENGR_SIGNPOST 7
#define N_ENGRAVE 7
    unsigned short engr_flags;
};

#define ENGR_FLAGS_NONE             ((unsigned short)0x0000)
#define ENGR_FLAGS_NON_SMUDGING     ((unsigned short)0x0001)
#define ENGR_FLAGS_SEEN             ((unsigned short)0x0002)

#define newengr(lth) \
    (struct engr *) alloc((size_t)(lth) + sizeof(struct engr))
#define dealloc_engr(engr) free((genericptr_t)(engr))

#endif /* ENGRAVE_H */

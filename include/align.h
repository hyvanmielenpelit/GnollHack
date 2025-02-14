/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-14 */

/* GnollHack 4.0    align.h    $NHDT-Date: 1432512779 2015/05/25 00:12:59 $  $NHDT-Branch: master $:$NHDT-Revision: 1.8 $ */
/* Copyright (c) Mike Stephenson, Izchak Miller  1991.          */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef ALIGN_H
#define ALIGN_H

typedef schar aligntyp; /* basic alignment type */

typedef struct align { /* alignment & record */
    aligntyp type;
    int record;
} align;

/* bounds for "record" -- respect initial alignments of 10 */
#define ALIGNLIM ((int)(10L + (moves / 200L)))

#define A_NONE (-128) /* the value range of type */

#define A_CHAOTIC (-1)
#define A_NEUTRAL 0
#define A_LAWFUL 1

#define A_COALIGNED 1
#define A_OPALIGNED (-1)

#define AM_NONE 0
#define AM_CHAOTIC 1
#define AM_NEUTRAL 2
#define AM_LAWFUL 4

#define AM_MASK 7

#define AM_SPLEV_CO 3
#define AM_SPLEV_NONCO 7

#define Amask2align(x)                                          \
    ((aligntyp)((!((x) & AM_MASK)) ? A_NONE : (((x) & AM_MASK) == AM_LAWFUL) ? A_LAWFUL \
                                                     : ((int) ((x) & AM_MASK)) - 2))
#define Align2amask(x) \
    (((x) == A_NONE) ? AM_NONE : ((x) == A_LAWFUL) ? AM_LAWFUL : (x) + 2)

/* For the lack of better place */
#define MAX_DGN_NAME_LENGTH 48
#define MAX_LVL_NAME_LENGTH 64
#define MAX_PROTO_NAME_LENGTH 15

#endif /* ALIGN_H */

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mextra.h    $NHDT-Date: 1547428759 2019/01/14 01:19:19 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.22 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MEXTRA_H
#define MEXTRA_H

#include "align.h"

/*
 *  Adding new mextra structures:
 *
 *       1. Add the structure definition and any required macros in this
 *          file above the mextra struct.
 *       2. Add a pointer to your new struct to the mextra struct in this
 *          file.
 *       3. Add a referencing macro at bottom of this file after the mextra
 *          struct (see MNAME, EGD, EPRI, ESHK, EMIN, or EDOG for examples).
 *       4. Create a newXX(mtmp) function and possibly a free_XX(mtmp)
 *          function in an appropriate new or existing source file and add
 *          a prototype for it to include/extern.h.
 *
 *              void FDECL(newXX, (struct monst *));
 *              void FDECL(free_XX, (struct monst *));
 *
 *              void
 *              newXX(mtmp)
 *              struct monst *mtmp;
 *              {
 *                  if (!mtmp->mextra)
 *                      mtmp->mextra = newmextra();
 *                  if (!XX(mtmp)) {
 *                      XX(mtmp) = (struct XX *) alloc(sizeof (struct XX));
 *                      (void) memset((genericptr_t) XX(mtmp),
 *                                    0, sizeof (struct XX));
 *                  }
 *              }
 *
 *       5. Consider adding a new makemon flag MM_XX flag to include/hack.h
 *          and a corresponding change to makemon() if you require your
 *          structure to be added at monster creation time.  Initialize your
 *          struct after a successful return from makemon().
 *
 *              src/makemon.c:  if (mmflags & MM_XX) newXX(mtmp);
 *              your new code:  mon = makemon(&mons[mnum], x, y, MM_XX);
 *
 *       6. Adjust size_monst() in src/cmd.c appropriately.
 *       7. Adjust dealloc_mextra() in src/mon.c to clean up
 *          properly during monst deallocation.
 *       8. Adjust copy_mextra() in src/mon.c to make duplicate
 *          copies of your struct or data on another monst struct.
 *       9. Adjust restmon() in src/restore.c to deal with your
 *          struct or data during a restore.
 *      10. Adjust savemon() in src/save.c to deal with your
 *          struct or data during a save.
 */

/***
 **     formerly vault.h -- vault guard extension
 */
#define FCSIZ (ROWNO + COLNO)
#define GD_EATGOLD 0x01
#define GD_DESTROYGOLD 0x02

struct fakecorridor {
    xchar fx, fy, ftyp;
};

struct egd {
    int fcbeg, fcend;     /* fcend: first unused pos */
    int vroom;            /* room number of the vault */
    xchar gdx, gdy;       /* goal of guard's walk */
    xchar ogx, ogy;       /* guard's last position */
    d_level gdlevel;      /* level (& dungeon) guard was created in */
    xchar warncnt;        /* number of warnings to follow */
    boolean gddone;       /* true iff guard has released player */
    uchar witness;        /* the guard saw you do something */
    uchar unused;
    struct fakecorridor fakecorr[FCSIZ];
};

#define MAX_SPECIAL_TEACH_SPELLS 32

/***
 **     formerly epri.h -- temple priest extension
 */
struct epri {
    aligntyp shralign;    /* alignment of priest's shrine */
    schar shroom;         /* index in rooms */
    coord shrpos;         /* position of shrine */
    d_level shrlevel;     /* level (& dungeon) of shrine */
    unsigned short priest_flags;
    short special_teach_spells[MAX_SPECIAL_TEACH_SPELLS];
    int64_t intone_time,  /* used to limit verbosity */
        enter_time,       /* of temple entry messages */
        hostile_time,     /* forbidding feeling */
        peaceful_time;    /* sense of peace */
};
/* note: roaming priests (no shrine) switch from ispriest to isminion
   (and emin extension) */

/* smith extension */
struct esmi {
    uchar smith_typ;      /* type of smith */
    schar smithy_room;    /* index in rooms */
    coord anvil_pos;      /* position of anvil */
    d_level smithy_level; /* level (& dungeon) of smithy */
    unsigned short smith_flags;
    int64_t intone_time,  /* used to limit verbosity */
        enter_time,       /* of smithy entry messages */
        hostile_time,     /* forbidding feeling */
        peaceful_time;    /* sense of peace */
};

/* Non-player character (NPC) extension */
struct enpc {
    uchar npc_typ;        /* type of NPC */
    schar npc_room;       /* index in rooms */
    d_level npc_room_level;  /* level (& dungeon) of NPC's room */
    short reserved;
    unsigned short npc_flags;
    short special_teach_spells[MAX_SPECIAL_TEACH_SPELLS];
    int64_t intone_time,  /* used to limit verbosity */
        enter_time,       /* of npc room entry messages */
        hostile_time,     /* forbidding feeling */
        peaceful_time;    /* sense of peace */
};


/***
 **     formerly eshk.h -- shopkeeper extension
 */
#define REPAIR_DELAY 5 /* minimum delay between shop damage & repair */
#define BILLSZ 200

struct bill_x {
    unsigned bo_id;
    boolean useup;
    int64_t price; /* price per unit */
    int64_t bquan; /* amount used up */
};

struct eshk {
    int64_t robbed;          /* amount stolen by most recent customer */
    int64_t credit;          /* amount credited to customer */
    int64_t debit;           /* amount of debt for using unpaid items */
    int64_t loan;            /* shop-gold picked (part of debit) */
    int shoptype;            /* the value of rooms[shoproom].rtype */
    schar shoproom;          /* index in rooms; set by inshop() */
    schar unused;            /* to force alignment for stupid compilers */
    boolean following;       /* following customer since he owes us sth */
    boolean surcharge;       /* angry shk inflates prices */
    boolean dismiss_kops;    /* pacified shk sends kops away */
    coord shk;               /* usual position shopkeeper */
    coord shd;               /* position shop door */
    d_level shoplevel;       /* level (& dungeon) of his shop */
    int billct;              /* no. of entries of bill[] in use */
    struct bill_x bill[BILLSZ];
    struct bill_x *bill_p;
    int visitct;             /* nr of visits by most recent customer */
    char customer[PL_NSIZ];  /* most recent customer */
    char shknam[PL_NSIZ];
};

/***
 **     formerly emin.h -- minion extension
 */
struct emin {
    aligntyp min_align; /* alignment of minion */
    boolean renegade;   /* hostile co-aligned priest or Angel */
};

/***
 **     formerly edog.h -- pet extension
 */
/*      various types of pet food, the lower the value, the better liked */
enum dogfood_types {
    DOGFOOD = 0,
    CADAVER = 1,
    ACCFOOD = 2,
    MANFOOD = 3,
    APPORT  = 4,
    POISON  = 5,
    UNDEF   = 6,
    TABU    = 7
};

struct edog {
    int64_t whistletime;         /* last time he whistled */
    int64_t hungrytime;          /* will get hungry at this time */
    int64_t droptime;            /* moment dog dropped object */
    unsigned dropdist;           /* dist of dropped obj from @ */
    int apport;                  /* amount of training */
    int abuse;                   /* track abuses to this pet */
    int revivals;                /* count pet deaths */
    int mhpmax_penalty;          /* while starving, points reduced */
    int chastised;               /* has a shopkeeper told the dog off; this is duration while the pet remembers being chastised */
    coord ogoal;                 /* previous goal location */
    boolean killed_by_u;         /* you attempted to kill him */
};

/***
 **     mextra.h -- collection of all monster extensions
 */
struct mextra {
    char *mname;
    char *umname;
    struct egd *egd;
    struct epri *epri;
    struct esmi *esmi;
    struct enpc *enpc;
    struct eshk *eshk;
    struct emin *emin;
    struct edog *edog;
    struct monst* mmonst; /* For original form when polymorphed */
    struct obj* mobj;     /* obj that mimic is posing as */
    int mcorpsenm;        /* obj->corpsenm for mimic posing as statue or corpse */
};

#define MNAME(mon) ((mon)->mextra->mname)
#define UMNAME(mon) ((mon)->mextra->umname)
#define EGD(mon) ((mon)->mextra->egd)
#define EPRI(mon) ((mon)->mextra->epri)
#define ESMI(mon) ((mon)->mextra->esmi)
#define ENPC(mon) ((mon)->mextra->enpc)
#define ESHK(mon) ((mon)->mextra->eshk)
#define EMIN(mon) ((mon)->mextra->emin)
#define EDOG(mon) ((mon)->mextra->edog)
#define MMONST(mon) ((mon)->mextra->mmonst)
#define MOBJ(mon) ((mon)->mextra->mobj)
#define MCORPSENM(mon) ((mon)->mextra->mcorpsenm)

#define has_mname(mon) ((mon)->mextra && MNAME(mon))
#define has_umname(mon) ((mon)->mextra && UMNAME(mon))
#define has_egd(mon)   ((mon)->mextra && EGD(mon))
#define has_epri(mon)  ((mon)->mextra && EPRI(mon))
#define has_esmi(mon)  ((mon)->mextra && ESMI(mon))
#define has_enpc(mon)  ((mon)->mextra && ENPC(mon))
#define has_eshk(mon)  ((mon)->mextra && ESHK(mon))
#define has_emin(mon)  ((mon)->mextra && EMIN(mon))
#define has_edog(mon)  ((mon)->mextra && EDOG(mon))
#define has_mmonst(mon)  ((mon)->mextra && MMONST(mon))
#define has_mobj(mon)  ((mon)->mextra && MOBJ(mon))
#define has_mcorpsenm(mon) ((mon)->mextra && MCORPSENM(mon) != NON_PM)
#define has_mextra_for_mcorpsenm(mon) ((mon)->mextra)

#endif /* MEXTRA_H */

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    vision.h    $NHDT-Date: 1432512777 2015/05/25 00:12:57 $  $NHDT-Branch: master $:$NHDT-Revision: 1.9 $ */
/* Copyright (c) Dean Luick, with acknowledgements to Dave Cohrs, 1990. */
/* GnollHack may be freely redistributed.  See license for details.    */

#ifndef VISION_H
#define VISION_H

#if 0 /* (moved to decl.h) */
extern boolean vision_full_recalc;    /* TRUE if need vision recalc */
extern char **viz_array;        /* could see/in sight row pointers */
extern char *viz_rmin;            /* min could see indices */
extern char *viz_rmax;            /* max could see indices */
#endif
#define COULD_SEE 0x01 /* location could be seen, if it were lit */
#define IN_SIGHT 0x02  /* location can be seen */
#define TEMP_LIT 0x04  /* location is temporarily lit */
#define TEMP_MAGICAL_DARKNESS 0x08  /* location is darkened by magic darkness source (overrides TEMP_LIT and lit) */
#define IN_XRAY_SIGHT 0x10  /* location can be seen by xray vision */
#define IN_NV_SIGHT 0x20  /* location can be seen by infrared vision */
#define IN_ANY_SIGHT (IN_SIGHT | IN_XRAY_SIGHT | IN_NV_SIGHT)
/*
 * Light source sources
 */
#define LS_OBJECT 0
#define LS_MONSTER 1
#define LS_LOCATION 2

/*
 *  cansee()    - Returns true if the hero can see the location.
 *
 *  couldsee()    - Returns true if the hero has a clear line of sight to
 *          the location.
 */
#define cansee(x, y) ((viz_array[y][x] & (IN_SIGHT | IN_XRAY_SIGHT | IN_NV_SIGHT)) != 0)
#define couldsee(x, y) ((viz_array[y][x] & COULD_SEE) != 0)
#define templit(x, y) ((viz_array[y][x] & TEMP_LIT) != 0)
#define canxraysee(x, y) ((viz_array[y][x] & IN_XRAY_SIGHT) != 0)
#define cannvsee(x, y) ((viz_array[y][x] & IN_NV_SIGHT) != 0)
#define canseenormal(x, y) ((viz_array[y][x] & IN_SIGHT) != 0)

/*
 *  The following assume the monster is not blind.
 *
 *  m_cansee()    - Returns true if the monster can see the given location.
 *
 *  m_canseeu() - Returns true if the monster could see the hero.  Assumes
 *          that if the hero has a clear line of sight to the monster's
 *          location and the hero is visible, then monster can see the
 *          hero.
 */
#define m_cansee(mtmp, x2, y2) clear_path((mtmp)->mx, (mtmp)->my, (x2), (y2))
#define m_has_active_telepathy(m) (has_telepathy(m) || (has_blind_telepathy(m) && is_blinded(m)) || has_detect_monsters(m))
#define m_senseu_telepathically(m) \
    ((((has_telepathy(m) && dist2(u.ux, u.uy, (m)->mx, (m)->my) <= (TELEPATHY_RANGE * TELEPATHY_RANGE)) || ((has_blind_telepathy(m) || has_telepathy(m)) && is_blinded(m))) && !Mind_shielding) || has_detect_monsters(m))
#define m_sense_m_telepathically(m, mtarg) \
    ((((has_telepathy(m) && dist2((mtarg)->mx, (mtarg)->my, (m)->mx, (m)->my) <= (TELEPATHY_RANGE * TELEPATHY_RANGE)) || ((has_blind_telepathy(m) || has_telepathy(m)) && is_blinded(m))) && !mon_has_no_apparent_mind(m)) || has_detect_monsters(m))
#define m_cannotsenseu(m) \
    ((is_blinded(m) || (Invis && !has_see_invisible(m)) || u.uundetected) && !m_senseu_telepathically(m))
#define m_cannotsense_m(m, mtarg) \
    ((is_blinded(m) || (is_invisible(mtarg) && !has_see_invisible(m)) || (mtarg)->mundetected) && !m_sense_m_telepathically(m,mtarg))

#define m_canseeu(m)                                       \
    ((!Invis || has_see_invisible(m)) && !is_blinded(m)    \
             && !(Underwater || u.uburied || (m)->mburied) \
         ? couldsee((m)->mx, (m)->my)                      \
         : 0)

#define m_cansee_m(magr, mdef)                                       \
    ((!is_invisible(mdef) || has_see_invisible(magr)) && !is_blinded(magr) && !(mdef)->mundetected    \
         ? m_cansee(magr, (mdef)->mx, (mdef)->my)                      \
         : 0)

/*
 *  Circle information
 */
#define MAX_RADIUS 15 /* this is in points from the source */

/* Use this macro to get a list of distances of the edges (see vision.c). */
#define circle_ptr(z) (&circle_data[(int) circle_start[z]])

/* howmonseen() bitmask values */
#define MONSEEN_NORMAL 0x0001   /* normal vision */
#define MONSEEN_SEEINVIS 0x0002 /* seeing invisible */
#define MONSEEN_INFRAVIS 0x0004 /* via infravision */
#define MONSEEN_TELEPAT 0x0008  /* via telepathy */
#define MONSEEN_XRAYVIS 0x0010  /* via Xray vision */
#define MONSEEN_DETECT 0x0020   /* via extended monster detection */
#define MONSEEN_WARNMON 0x0040  /* via type-specific warning */

#endif /* VISION_H */

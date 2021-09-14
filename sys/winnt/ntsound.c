/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    ntsound.c    $NHDT-Date: 1432512794 2015/05/25 00:13:14 $  $NHDT-Branch: master $:$NHDT-Revision: 1.11 $ */
/*   Copyright (c) GnollHack PC Development Team 1993                 */
/*   GnollHack may be freely redistributed.  See license for details. */
/*                                                                  */
/*
 * ntsound.c - Windows NT GnollHack sound support
 *
 *Edit History:
 *     Initial Creation                              93/12/11
 *
 */

#include "win32api.h"
#include "hack.h"
#include <mmsystem.h>

#ifdef USER_SOUNDS

void
play_usersound(filename, volume)
const char *filename;
int volume;
{
    /*    pline("play_usersound: %s (%d).", filename, volume); */
    (void) sndPlaySound(filename, SND_ASYNC | SND_NODEFAULT);
}

#endif /*USER_SOUNDS*/
/* ntsound.c */

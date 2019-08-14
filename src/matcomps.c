/* GnollHack 4.0	objects.c	$NHDT-Date: 1535422421 2018/08/28 02:13:41 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.51 $ */
/* Copyright (c) Janne Gustafsson 2019.                           */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

struct materialcomponentlist matlists[] =
{
	{STRANGE_OBJECT,
	{NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	{SPE_WISH,
	{{DIAMOND, 1, MATCOMP_BLESSED_REQUIRED}, {0, 0, MATCOMP_NO_FLAGS}, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	{SPE_ARMAGEDDON,
	{{BONE_DAGGER, 1, MATCOMP_NO_FLAGS}, {LONG_BOW, 1, MATCOMP_NO_FLAGS}, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	//Array terminator
	{-1,
	{NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

};


void matcomps_init();

/* dummy routine used to force linkage */
void
matcomps_init()
{
    return;
}

/*matcomps.c*/

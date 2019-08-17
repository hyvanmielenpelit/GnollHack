/* GnollHack 4.0	objects.c	$NHDT-Date: 1535422421 2018/08/28 02:13:41 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.51 $ */
/* Copyright (c) Janne Gustafsson 2019.                           */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define NOT_APPLICABLE 0

struct materialcomponentlist matlists[] =
{
	{STRANGE_OBJECT,
	"",
	1,
	{NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	{SPE_WISH,
	"Blessed diamond",
	1,
	{{DIAMOND, NOT_APPLICABLE, 1, MATCOMP_BLESSED_REQUIRED}, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	{SPE_ARMAGEDDON,
	"Dagger and lizard",
	1,
	{{BONE_DAGGER, NOT_APPLICABLE, 1, MATCOMP_DEATH_ENCHANTMENT_REQUIRED | MATCOMP_NOT_SPENT | MATCOMP_NOT_CURSED},
	 {CORPSE, PM_LIZARD, 1, MATCOMP_NOT_CURSED},
	 NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	{SPE_BLACK_BLADE_OF_DISASTER,
	"Black opal",
	1,
	{{BLACK_OPAL, NOT_APPLICABLE, 1, MATCOMP_NOT_CURSED}, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP, NO_MATCOMP}},

	//Array terminator, uses spellsgained
	{STRANGE_OBJECT,
	"",
	0,
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

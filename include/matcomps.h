/* GnollHack 4.0	matcomps.h	$NHDT-Date: 1547255901 2019/01/12 01:18:21 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Janne Gustafsson 2019. */
/* GnollHack may be freely redistributed.  See license for details. */


#ifndef MATCOMPS_H
#define MATCOMPS_H


struct materialcomponent {
	int objectid;
	int monsterid; //Used for corpses and tins
	int amount;
	unsigned long flags;
};

#define MATCOMP_NO_FLAGS 0x00000000
#define MATCOMP_NOT_SPENT 0x00000001
#define MATCOMP_NO_CURSED 0x00000002
#define MATCOMP_BLESSED_REQUIRED 0x00000004
#define MATCOMP_DEATH_ENCHANTMENT_REQUIRED 0x00000008


#define MAX_MATERIALS 10

struct materialcomponentlist {
	int spell_objectid; //Used to link the list with the appropriate spell, one spell can have many, but first we use only the first
	int spellsgained; //How many spells do you get for these material components
	struct materialcomponent matcomp[MAX_MATERIALS];
};

/*extern NEARDATA struct materialcomponentlist matlists[];*/

/*Material components*/
#define NO_MATCOMP {STRANGE_OBJECT, 0, 0, MATCOMP_NO_FLAGS}

extern NEARDATA struct materialcomponentlist matlists[];

#endif /* MATCOMPS_H */

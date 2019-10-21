/* GnollHack 4.0	matcomps.h	$NHDT-Date: 1547255901 2019/01/12 01:18:21 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Janne Gustafsson 2019. */
/* GnollHack may be freely redistributed.  See license for details. */


#ifndef MATCOMPS_H
#define MATCOMPS_H


struct materialcomponent {
	int objectid;  //0 if none
	int monsterid; //Used for corpses and tins, -1 if none
	int amount;
	unsigned long flags;
};

#define MATCOMP_NO_FLAGS 0x00000000
#define MATCOMP_NOT_SPENT 0x00000001
#define MATCOMP_NOT_CURSED 0x00000002
#define MATCOMP_BLESSED_REQUIRED 0x00000004
#define MATCOMP_CURSED_REQUIRED 0x00000008
#define MATCOMP_DEATH_ENCHANTMENT_REQUIRED 0x00000010


#define MAX_MATERIALS 10

struct materialcomponentlist {
	int spell_objectid;			/* Used to link the list with the appropriate spell, one spell can have many, but first we use only the first */
	char* description_short;	/* Used for mixing menu, when selecting the spell */
	int created_objectid;		/* Used for recipes to indicate the object created */
	int spellsgained;			/* How many spells do you get for these material components */
	struct materialcomponent matcomp[MAX_MATERIALS];
};

/*extern NEARDATA struct materialcomponentlist matlists[];*/

/*Material components*/
#define NOT_APPLICABLE -1

#define NO_MATCOMP {STRANGE_OBJECT, NOT_APPLICABLE, 0, MATCOMP_NO_FLAGS}

extern NEARDATA struct materialcomponentlist matlists[];

#endif /* MATCOMPS_H */

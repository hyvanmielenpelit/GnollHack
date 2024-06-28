/* GnollHack 4.0    matcomps.h    $NHDT-Date: 1547255901 2019/01/12 01:18:21 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/*-Copyright (c) Janne Gustafsson 2019. */
/* GnollHack may be freely redistributed.  See license for details. */


#ifndef MATCOMPS_H
#define MATCOMPS_H

#define MAX_MATCOMP_ALTERNATIVES 10
#define MAX_MATCOMP_MONSTER_ALTERNATIVES 10

struct materialcomponent {
    int objectid[MAX_MATCOMP_ALTERNATIVES];  //0 if none
    int monsterid[MAX_MATCOMP_MONSTER_ALTERNATIVES]; //Used for corpses and tins, -1 if none
    const char* description;
    int amount;
    uint64_t flags;
};

#define MATCOMP_NO_FLAGS 0x00000000UL
#define MATCOMP_NOT_SPENT 0x00000001UL
#define MATCOMP_NOT_CURSED 0x00000002UL
#define MATCOMP_BLESSED_REQUIRED 0x00000004UL
#define MATCOMP_CURSED_REQUIRED 0x00000008UL
#define MATCOMP_DEATH_ENCHANTMENT_REQUIRED 0x00000010UL


#define MAX_MATERIALS 10

struct materialcomponentlist {
    int spell_objectid;            /* Used to link the list with the appropriate spell, one spell can have many, but first we use only the first */
    const char* description_short;    /* Used for mixing menu, when selecting the spell */
    int created_objectid;        /* UNUSED: Used for recipes to indicate the object created */
    int spellsgained;            /* How many spells do you get for these material components */
    struct materialcomponent matcomp[MAX_MATERIALS];
};

/*extern NEARDATA struct materialcomponentlist matlists[];*/

/*Material components*/
#define NOT_APPLICABLE -1
#define NO_MATCOMP_ALTERNATIVES { STRANGE_OBJECT, STRANGE_OBJECT, STRANGE_OBJECT, STRANGE_OBJECT, STRANGE_OBJECT, \
                                  STRANGE_OBJECT, STRANGE_OBJECT, STRANGE_OBJECT, STRANGE_OBJECT, STRANGE_OBJECT }

#define NO_MATCOMP_MONSTER_ALTERNATIVES { NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE, NOT_APPLICABLE }

#define NO_MATCOMP {NO_MATCOMP_ALTERNATIVES, NO_MATCOMP_MONSTER_ALTERNATIVES, 0, MATCOMP_NO_FLAGS}

extern NEARDATA struct materialcomponentlist matlists[];

#endif /* MATCOMPS_H */

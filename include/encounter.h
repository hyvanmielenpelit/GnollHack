/* GnollHack 4.0	encounter.h	 */
/* Copyright (c) Janne Gustafsson 2019. */
/* GnollHack may be freely redistributed.  See license for details. */


#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#define MAX_MONSTER_ITEMS 5
#define MAX_MONSTER_RANDOM_ITEM_ALTERNATIVES 5
#define MAX_RANDOM_MONSTER_ALTERNATIVES 5
#define MAX_ENCOUNTER_MONSTER_TYPES 5
#define MAX_ENCOUNTER_MONSTERS 50

struct monsteritemdef {
	int otyp;
	int oartifact;
	int spe_constant;
	int spe_random;
	long iflags;
	int probability;
};

#define MI_BLESSED 0x00000001
#define MI_CURSED 0x00000002
#define MI_UNCURSED 0x00000004
#define MI_ERODEPROOF 0x00000008
#define MI_IGNORE_SPE 0x00000010
#define MI_INITIALIZE 0x00000020
#define MI_ALLOW_ARTIFACTS 0x00000040

struct monsterrandomizeditem {
	struct monsteritemdef random_monster_items[MAX_MONSTER_RANDOM_ITEM_ALTERNATIVES];
};


struct encountermonsterdef {
	int permonstid;
	int generatednumber_constant;
	int generatednumber_random;
	int namelistid;
	long mflags;
	double probability;
	struct monsterrandomizeditem monster_items[MAX_MONSTER_ITEMS];
};

struct randomizedencountermonsterdef {
	struct encountermonsterdef random_encounter_monsters[MAX_RANDOM_MONSTER_ALTERNATIVES];
};

struct encounterdef {
	struct randomizedencountermonsterdef encounter_monster_types[MAX_ENCOUNTER_MONSTER_TYPES];
	double probability;
	long eflags;
};

#define ED_NOHELL 0x00000001
#define ED_HELL_ONLY 0x00000002
#define ED_NOMINES 0x00000004
#define ED_MINES_ONLY 0x00000008

#define ED_ONLY_ONCE 0x80000000


struct encounter_monster {
	int permonstid;
	struct monsterrandomizeditem monster_items[MAX_MONSTER_ITEMS];
	int namelistid;
	long miflags;
};

#define MAX_ENCOUNTER_ATTACKING_MONSTERS 8
struct encounter {
	struct encounter_monster encounter_monsters[MAX_ENCOUNTER_MONSTERS];
	int difficulty_min;
	int difficulty_point_estimate[MAX_ENCOUNTER_ATTACKING_MONSTERS + 1];
	int difficulty_max;
	double probability;
	boolean insearch;
	int appear_count;
	long eflags;
};


#endif /* ENCOUNTER_H */

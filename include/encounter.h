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
	int spe_constant;
	int spe_random;
	long iflags;
	double probability;
};

struct monsterrandomizeditem {
	struct monsteritemdef random_monster_items[MAX_MONSTER_RANDOM_ITEM_ALTERNATIVES];
};


struct encountermonsterdef {
	int permonstid;
	int generatednumber_constant;
	int generatednumber_random;
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



struct monster_item {
	int otyp;
	int spe;
	long iiflags;
};


struct encounter_monster {
	int permonstid;
	struct monster_item monster_items[MAX_MONSTER_ITEMS];
	long miflags;
};


struct encounter {
	struct encounter_monster encounter_monsters[MAX_ENCOUNTER_MONSTERS];
	int difficulty_min;
	int difficulty_point_estimate;
	int difficulty_max;
	double probability;
	boolean insearch;
	long eiflags;
};


#endif /* ENCOUNTER_H */

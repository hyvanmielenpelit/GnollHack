/* GnollHack 4.0    encounter.h     */
/* Copyright (c) Janne Gustafsson 2019. */
/* GnollHack may be freely redistributed.  See license for details. */


#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#define MAX_MONSTER_ITEMS 5
#define MAX_MONSTER_RANDOM_ITEM_ALTERNATIVES 5
#define MAX_RANDOM_MONSTER_ALTERNATIVES 5
#define MAX_ENCOUNTER_MONSTER_TYPES 5
#define MAX_ENCOUNTER_MONSTERS 25

struct monsteritemdef {
    short otyp;
    short oartifact;
    int spe_constant;
    int spe_random;
    int64_t iflags;
    int probability;
};

#define MI_BLESSED 0x00000001L
#define MI_CURSED 0x00000002L
#define MI_UNCURSED 0x00000004L
#define MI_ERODEPROOF 0x00000008L
#define MI_IGNORE_SPE 0x00000010L
#define MI_INITIALIZE 0x00000020L
#define MI_ALLOW_ARTIFACTS 0x00000040L

struct monsterrandomizeditem {
    struct monsteritemdef random_monster_items[MAX_MONSTER_RANDOM_ITEM_ALTERNATIVES];
};


struct encountermonsterdef {
    int permonstid;
    int generatednumber_constant;
    int generatednumber_random;
    int namelistid;
    int64_t mflags;
    double probability;
    struct monsterrandomizeditem monster_items[MAX_MONSTER_ITEMS];
};

#define EM_IGNORE_APPEARANCE_FLAGS 0x00000001L
#define EM_MAXIMUM_HIT_POINTS 0x00000002L
#define EM_FORCE_MALE 0x00000010L
#define EM_FORCE_FEMALE 0x00000020L
#define EM_NAME_KNOWN 0x00000040L
#define EM_FORCE_PEACEFUL 0x00000080L


struct randomizedencountermonsterdef {
    struct encountermonsterdef random_encounter_monsters[MAX_RANDOM_MONSTER_ALTERNATIVES];
};

struct encounterdef {
    struct randomizedencountermonsterdef encounter_monster_types[MAX_ENCOUNTER_MONSTER_TYPES];
    double probability;
    int64_t eflags;
};

#define ED_NOHELL 0x00000001L
#define ED_HELL_ONLY 0x00000002L
#define ED_NOMINES 0x00000004L
#define ED_MINES_ONLY 0x00000008L
#define ED_IGNORE_MONSTER_APPEARANCE_FLAGS 0x00000010L

#define ED_ONLY_ONCE 0x80000000L


struct encounter_monster {
    int permonstid;
    struct monsterrandomizeditem monster_items[MAX_MONSTER_ITEMS];
    int namelistid;
    int64_t mflags;
};

#define MAX_ENCOUNTER_ATTACKING_MONSTERS 8
struct encounter {
    struct encounter_monster encounter_monsters[MAX_ENCOUNTER_MONSTERS];
    int difficulty_min;
    int difficulty_point_estimate[MAX_ENCOUNTER_ATTACKING_MONSTERS + 1];
    int difficulty_max;
    double probability;
    boolean insearch;
    int encounterdefid;
    int64_t eflags;
};

#endif /* ENCOUNTER_H */

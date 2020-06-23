/*
 * soundset.h
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "action.h" 

#ifndef SOUNDSET_H
#define SOUNDSET_H

#define BACKGROUND_MUSIC_VOLUME 0.04f
#define MAX_HEARING_DISTANCE 10


/* All GnollHack Sounds */
enum ghsound_types {
	GHSOUND_NONE = 0,
	GHSOUND_DUNGEON_NORMAL_MUSIC_NORMAL,
	GHSOUND_PLAYER_FOOTSTEPS_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_GOING_BACK_UP,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_ATTACKED,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_CLEARED,
	GHSOUND_PLAYER_STRIKE_GENERAL,
	GHSOUND_PLAYER_HIT_GENERAL,
	GHSOUND_PLAYER_MISS_GENERAL,
	GHSOUND_FOUNTAIN,
	GHSOUND_BEE,
	GHSOUND_FIRE,
	MAX_GHSOUNDS
};



/* play_ghsound function input structs */
struct ghsound_action_info {
	enum ghsound_types ghsound;
	float volume;
};

struct ghsound_ambience_info {
	enum ghsound_types ghsound;
	float intensity;
	float volume;
	int source_x;
	int source_y;
	int* ghsound_ambiance_handle;
};

struct ghsound_effect_info {
	enum ghsound_types ghsound;
	float volume;
};

struct ghsound_hit_info {
	enum ghsound_types ghsound;
	/* Weapon type */
	/* Armor type */
	float volume;
};

struct ghsound_miss_info {
	enum ghsound_types ghsound;
	float volume;
};

struct ghsound_movement_info {
	enum ghsound_types ghsound;
	enum movement_styles floor;
	enum shoe_hardness_types shoe;
	float weight;
	float volume;
	boolean isyou;
	int source_x;
	int source_y;
};

struct ghsound_music_info {
	enum ghsound_types ghsound;
	float intensity;
	float volume;
	int* ghsound_music_handle;
};

struct ghsound_ui_info {
	enum ghsound_types ghsound;
	float volume;
};



/* PLAYER SOUNDSETS */
struct player_soundset_definition {
	char* soundset_name;

	/* Ambient */
	enum ghsound_types stand;

	/* Movement */
	enum ghsound_types movement_sounds[MAX_MOVEMENT_STYLES];

	/* Actions */
	enum ghsound_types action_sounds[MAX_ACTION_TILES];
};

enum player_soundset_types {
	PLAYER_SOUNDSET_NO_SOUNDSET = 0,
	PLAYER_SOUNDSET_GENERAL,
	PLAYER_SOUNDSET_FEMALE_ELVEN_WIZARD,
	MAX_PLAYER_SOUNDSETS
};

extern struct player_soundset_definition player_soundsets[MAX_PLAYER_SOUNDSETS + 1];


/* MONSTER SOUNDSETS */
struct monster_soundset_definition {
	char* soundset_name;

	/* Ambient */
	enum ghsound_types ambient_sound;
	char ambient_volume;

	/* Movement */
	enum ghsound_types movement_sounds[MAX_MOVEMENT_STYLES];

	/* Actions */
	enum ghsound_types action_sounds[MAX_ACTION_TILES];
};

enum monster_soundset_types {
	MONSTER_SOUNDSET_NONE = 0,
	MONSTER_SOUNDSET_GENERAL,
	MONSTER_SOUNDSET_GOBLIN,
	MONSTER_SOUNDSET_DRAGON,
	MONSTER_SOUNDSET_DRACOLICH,
	MONSTER_SOUNDSET_BEE,
	MAX_MONSTER_SOUNDSETS
};

extern struct monster_soundset_definition monster_soundsets[MAX_MONSTER_SOUNDSETS + 1];


/* OBJECT SOUNDSETS */
struct object_soundset_definition {
	char* soundset_name;

	/* Ambient */
	enum ghsound_types ambient_sound;
	char ambient_volume;
};


enum object_soundset_types {
	OBJECT_SOUNDSET_NONE = 0,
	OBJECT_SOUNDSET_GENERAL,
	OBJECT_SOUNDSET_CANDLE,
	MAX_OBJECT_SOUNDSETS
};

extern struct object_soundset_definition object_soundsets[MAX_OBJECT_SOUNDSETS + 1];


/* LOCATION SOUNDSETS */
struct location_soundset_definition {
	char* soundset_name;

	/* Ambient */
	enum ghsound_types ambient_sound;
	char ambient_volume;
};

enum location_soundset_types {
	LOCATION_SOUNDSET_NONE = 0,
	LOCATION_SOUNDSET_GENERAL,
	LOCATION_SOUNDSET_FOUNTAIN,
	MAX_LOCATION_SOUNDSETS
};

extern struct location_soundset_definition location_soundsets[MAX_LOCATION_SOUNDSETS + 1];



enum room_soundset_types {
	ROOM_SOUNDSET_NO_SOUNDSET = 0,
	ROOM_SOUNDSET_GENERAL,
	ROOM_SOUNDSET_BARRACKS,
	ROOM_SOUNDSET_BEEHIVE,
	ROOM_SOUNDSET_SHOP,
	MAX_ROOM_SOUNDSETS
};

enum level_soundset_types {
	LEVEL_SOUNDSET_NO_SOUNDSET = 0,
	LEVEL_SOUNDSET_GENERAL,
	LEVEL_SOUNDSET_MINETOWN,
	LEVEL_SOUNDSET_ORACLE,
	LEVEL_SOUNDSET_BIGROOM,
	LEVEL_SOUNDSET_MEDUSA,
	LEVEL_SOUNDSET_MAZES,
	LEVEL_SOUNDSET_CASTLE,
	LEVEL_SOUNDSET_VALLEY,
	LEVEL_SOUNDSET_ORCUS,
	LEVEL_SOUNDSET_WIZARD,
	LEVEL_SOUNDSET_VLAD,
	LEVEL_SOUNDSET_TARRASQUE,
	LEVEL_SOUNDSET_SANCTUM,
	LEVEL_SOUNDSET_ASTRAL_PLANE,
	MAX_LEVEL_SOUNDSETS
};

enum dungeon_soundset_types {
	DUNGEON_SOUNDSET_NO_SOUNDSET = 0,
	DUNGEON_SOUNDSET_GENERAL,
	DUNGEON_SOUNDSET_DUNGEONS_OF_DOOM,
	DUNGEON_SOUNDSET_GEHENNOM,
	DUNGEON_SOUNDSET_SOKOBAN,
	DUNGEON_SOUNDSET_GNOMISH_MINES,
	DUNGEON_SOUNDSET_MODRON,
	DUNGEON_SOUNDSET_BOVINE,
	DUNGEON_SOUNDSET_QUEST,
	DUNGEON_SOUNDSET_FORT_LUDIOS,
	DUNGEON_SOUNDSET_ENDGAME,
	MAX_DUNGEON_SOUNDSETS
};


enum spell_soundset_types {
	SPELL_SOUNDSET_NO_SOUNDSET = 0,
	SPELL_SOUNDSET_GENERAL,
	SPELL_SOUNDSET_RAY,
	MAX_SPELL_SOUNDSETS
};


enum effect_soundset_types {
	EFFECT_SOUNDSET_NO_SOUNDSET = 0,
	EFFECT_SOUNDSET_SHIELD_EFFECT,
	EFFECT_SOUNDSET_TALK_EFFECT,
	MAX_EFFECT_SOUNDSETS
};


enum ui_soundset_types {
	UI_SOUNDSET_NO_SOUNDSET = 0,
	UI_SOUNDSET_BUTTON,
	MAX_UI_SOUNDSETS
};



#define SOUNDSOURCE_OBJECT 0
#define SOUNDSOURCE_MONSTER 1
#define SOUNDSOURCE_LOCATION 2



#endif /* SOUNDSET_H */
/*
 * soundset.h
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "action.h" 

#ifndef SOUNDSET_H
#define SOUNDSET_H

#define BACKGROUND_MUSIC_VOLUME 0.04f
#define MAX_HEARING_DISTANCE 10


enum soundsource_types {
	SOUNDSOURCE_OBJECT = 0,
	SOUNDSOURCE_MONSTER,
	SOUNDSOURCE_LOCATION,
	SOUNDSOURCE_ROOM, /* Plays while in room */
	SOUNDSOURCE_LEVEL, /* Plays while on level */
	SOUNDSOURCE_DUNGEON, /* Plays while in dungeon */
	SOUNDSOURCE_LEVEL_LOCATION_TYPE, /* Only nearest counts */
	MAX_SOUNDSOURCE_TYPES
};

enum soundsource_ambient_subtypes {
	SOUNDSOURCE_AMBIENT_GENERAL = 0,
	SOUNDSOURCE_AMBIENT_LIT,
	MAX_SOUNDSOURCE_AMBIENT_SUBTYPES
};




/* All GnollHack Sounds */
enum ghsound_types {
	GHSOUND_NONE = 0,
	GHSOUND_MUSIC_SPLASH,
	GHSOUND_MUSIC_PLAYER_SELECTION,
	GHSOUND_MUSIC_INTRO,
	GHSOUND_DUNGEON_NORMAL_MUSIC_NORMAL,
	GHSOUND_PLAYER_FOOTSTEPS_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_GOING_BACK_UP,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_ATTACKED,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_DESERTED,
	GHSOUND_PLAYER_STRIKE_GENERAL,
	GHSOUND_PLAYER_HIT_GENERAL,
	GHSOUND_PLAYER_MISS_GENERAL,
	GHSOUND_FOUNTAIN,
	GHSOUND_BEE,
	GHSOUND_FIRE,
	GHSOUND_STAFF_SWING,
	GHSOUND_STAFF_HIT,
	GHSOUND_STAFF_MISS,
	MAX_GHSOUNDS
};

/* Music types */
enum music_types {
	MUSIC_NONE = 0,
	MUSIC_INTRO,
	MUSIC_NORMAL, /* Level or dungeon */
	MUSIC_ROOM
};

#define MAX_SOUND_PARAMETERS 8

/* Used play_ghsound function input structs */
struct ghsound_immediate_info {
	enum ghsound_types ghsound;
	char* parameter_names[MAX_SOUND_PARAMETERS];
	float parameter_values[MAX_SOUND_PARAMETERS];
	float volume;
};

struct ghsound_music_info {
	enum ghsound_types ghsound;
	float volume;
	boolean stop_music;
};



/* Unused play_ghsound function input structs */
struct function_info_A {
	enum ghsound_types ghsound;
	float volume;
};


struct function_info_C {
	enum ghsound_types ghsound;
	enum strike_surface_types strike_surface;
	float volume;
};

struct function_info_D {
	enum ghsound_types ghsound;
	float volume;
};

struct function_info_E {
	enum ghsound_types ghsound;
	float volume;
};

struct function_info_F {
	enum ghsound_types ghsound;
	float volume;
};



/* General sound information */
struct ghsound_info {
	enum ghsound_types ghsound;
	float volume;
};


/* OBJECT SOUNDSETS */
enum object_sound_types {
	OBJECT_SOUND_TYPE_AMBIENT = 0,		/* Ambient sound that is typically either continuously played or played when the object is lit */
	OBJECT_SOUND_TYPE_SWING_MELEE,		/* Played at the start of the attack animation */
	OBJECT_SOUND_TYPE_HIT_MELEE,		/* Played at the end of the melee attack animation and at the end of throw if the attack hits target */
	OBJECT_SOUND_TYPE_THROW,			/* Played when the object is thrown */
	OBJECT_SOUND_TYPE_FIRE,				/* Played when a launcher is fired */
	OBJECT_SOUND_TYPE_HIT_THROW,		/* Played at the end of the throw/fire attack animation and at the end of throw if the attack hits target */
	OBJECT_SOUND_TYPE_BREAK,			/* Sound upon breaking the item */
	MAX_OBJECT_SOUND_TYPES
};

struct object_soundset_definition {
	char* soundset_name;
	struct ghsound_info sounds[MAX_OBJECT_SOUND_TYPES];
	enum soundsource_ambient_subtypes ambient_subtype;

};

enum object_soundset_types {
	OBJECT_SOUNDSET_NONE = 0,
	OBJECT_SOUNDSET_GENERAL,
	OBJECT_SOUNDSET_CANDLE,
	OBJECT_SOUNDSET_QUARTERSTAFF,
	MAX_OBJECT_SOUNDSETS
};

extern struct object_soundset_definition object_soundsets[MAX_OBJECT_SOUNDSETS];


/* PLAYER SOUNDSETS */
struct player_soundset_definition {
	char* soundset_name;

	/* Ambient */
	struct ghsound_info ambient_sound;
	enum soundsource_ambient_subtypes ambient_subtype;

	/* Bare handed soundset */
	enum object_soundset_types barehanded_soundset;

	/* Movement */
	struct ghsound_info movement_sounds[MAX_MOVEMENT_STYLES];

	/* Actions */
	struct ghsound_info action_sounds[MAX_ACTION_TILES];
};

enum player_soundset_types {
	PLAYER_SOUNDSET_NO_SOUNDSET = 0,
	PLAYER_SOUNDSET_GENERAL,
	PLAYER_SOUNDSET_FEMALE_ELVEN_WIZARD,
	MAX_PLAYER_SOUNDSETS
};

extern struct player_soundset_definition player_soundsets[MAX_PLAYER_SOUNDSETS];


/* MONSTER SOUNDSETS */
struct monster_soundset_definition {
	char* soundset_name;

	/* Ambient */
	struct ghsound_info ambient_sound;
	enum soundsource_ambient_subtypes ambient_subtype;

	/* Attacks */
	enum object_soundset_types attack_soundsets[NATTK];

	/* Movement */
	struct ghsound_info movement_sounds[MAX_MOVEMENT_STYLES];

	/* Actions */
	struct ghsound_info action_sounds[MAX_ACTION_TILES];
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

extern struct monster_soundset_definition monster_soundsets[MAX_MONSTER_SOUNDSETS];



/* LOCATION SOUNDSETS */
enum location_sound_types {
	LOCATION_SOUND_TYPE_AMBIENT = 0,		/* Ambient sound that is typically either continuously played or played when the object is lit */
	LOCATION_SOUND_TYPE_BREAK,			/* Sound upon breaking the item */
	MAX_LOCATION_SOUND_TYPES
};

struct location_soundset_definition {
	char* soundset_name;
	struct ghsound_info sounds[MAX_LOCATION_SOUND_TYPES];
	enum soundsource_ambient_subtypes ambient_subtype;
};

enum location_soundset_types {
	LOCATION_SOUNDSET_NONE = 0,
	LOCATION_SOUNDSET_GENERAL,
	LOCATION_SOUNDSET_FOUNTAIN,
	LOCATION_SOUNDSET_ALTAR,
	MAX_LOCATION_SOUNDSETS
};

extern struct location_soundset_definition location_soundsets[MAX_LOCATION_SOUNDSETS];


/* EFFECT SOUNDS */
struct effect_sound_definition {
	char* sound_name;
	struct ghsound_info sound;
};

enum effect_sound_types {
	EFFECT_SOUND_NONE = 0,
	EFFECT_SOUND_GENERAL,
	EFFECT_SOUND_SHIELD_EFFECT,
	EFFECT_SOUND_TALK_EFFECT,
	MAX_EFFECT_SOUNDS
};

extern struct effect_sound_definition effect_sounds[MAX_EFFECT_SOUNDS];



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



enum ui_soundset_types {
	UI_SOUNDSET_NO_SOUNDSET = 0,
	UI_SOUNDSET_BUTTON,
	MAX_UI_SOUNDSETS
};


#endif /* SOUNDSET_H */
/*
 * soundset.h
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "action.h" 

#ifndef SOUNDSET_H
#define SOUNDSET_H

#define BACKGROUND_MUSIC_VOLUME 0.04f
#define SOKOBAN_MUSIC_VOLUME 0.10f
#define MAX_HEARING_DISTANCE 10


/*
* GHSOUNDS
*/
/* All GnollHack Sounds */
enum ghsound_types {
	GHSOUND_NONE = 0,
	GHSOUND_MUSIC_SPLASH,
	GHSOUND_MUSIC_PLAYER_SELECTION,
	GHSOUND_MUSIC_INTRO,
	GHSOUND_DUNGEON_NORMAL_MUSIC_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_ORACLE,
	GHSOUND_DUNGEON_NORMAL_MUSIC_TEMPLE,
	GHSOUND_DUNGEON_NORMAL_MUSIC_MEDUSA,
	GHSOUND_DUNGEON_NORMAL_MUSIC_CASTLE,
	GHSOUND_GNOMISH_MINES_MUSIC_NORMAL,
	GHSOUND_GNOMISH_MINES_MUSIC_TOWN,
	GHSOUND_SOKOBAN_MUSIC_NORMAL,
	GHSOUND_GEHENNOM_MUSIC_NORMAL,
	GHSOUND_GEHENNOM_MUSIC_VALLEY,
	GHSOUND_GEHENNOM_MUSIC_SANCTUM,
	GHSOUND_PLAYER_FOOTSTEPS_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_SHOPKEEPER_ANGRY,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_SHOPKEEPER_DEAD,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_DESERTED,
	GHSOUND_FOUNTAIN,
	GHSOUND_BEE,
	GHSOUND_FIRE,
	GHSOUND_STAFF_SWING,
	GHSOUND_STAFF_HIT,
	GHSOUND_GEHENNOM_VALLEY_AMBIENT,
	GHSOUND_MORGUE,
	GHSOUND_GARDEN,
	GHSOUND_UI_BUTTON_DOWN,
	GHSOUND_UI_MENU_SELECT,
	GHSOUND_SFX_QUAFF,
	MAX_GHSOUNDS
};


/* 
 * SOUND SOURCES ETC.
 */
/* Mobile ambient sound sources */
enum soundsource_types {
	SOUNDSOURCE_OBJECT = 0,
	SOUNDSOURCE_MONSTER,
	SOUNDSOURCE_LOCATION,
	MAX_SOUNDSOURCE_TYPES
};

enum soundsource_ambient_subtypes {
	SOUNDSOURCE_AMBIENT_GENERAL = 0,
	SOUNDSOURCE_AMBIENT_LIT,
	MAX_SOUNDSOURCE_AMBIENT_SUBTYPES
};

/* Conditional sound sources */
enum conditional_soundsource_types {
	CONDITIONAL_SOUNDSOURCE_RIVER = 0,
	CONDITIONAL_SOUNDSOURCE_LAVA = 0,
	CONDITIONAL_SOUNDSOURCE_FOREST = 0,
	MAX_CONDITIONAL_SOUNDSOURCE_TYPES
};


/* UI sound types */
enum ui_sound_types {
	UI_SOUND_ILLEGAL = 0,
	UI_SOUND_BUTTON_DOWN,
	UI_SOUND_MENU_SELECT,
	UI_SOUND_TICK_BOX,
	UI_SOUND_UNTICK_BOX,
	UI_SOUND_OPEN_WINDOW,
	MAX_UI_SOUND_TYPES
};

/* General effect sound types */
enum sfx_sound_types {
	SFX_ILLEGAL = 0,
	SFX_READ,
	SFX_QUAFF,
	SFX_OPEN_DOOR,
	SFX_CLOSE_DOOR,
	SFX_LOCK_DOOR,
	SFX_UNLOCK_DOOR,
	SFX_OPEN_CHEST,
	SFX_CLOSE_CHEST,
	SFX_LOCK_CHEST,
	SFX_UNLOCK_CHEST,
	SFX_KICK_SWING,
	SFX_KICK_THUMP,
	SFX_MUFFLED_SHATTER,
	SFX_CHIME_OF_CASH_REGISTER,
	SFX_SHIELD_EFFECT,
	SFX_TALK_EFFECT,
	MAX_SFX_SOUND_TYPES
};


/* 
 * SOUND WINDOWPROC FUNCTION DEFINITIONS 
 */
#define MAX_SOUND_PARAMETERS 8
enum immediate_sound_types {
	IMMEDIATE_SOUND_SFX = 0,
	IMMEDIATE_SOUND_UI = 1
};

/* Used play_ghsound function input structs */
struct ghsound_immediate_info {
	enum ghsound_types ghsound;
	char* parameter_names[MAX_SOUND_PARAMETERS];
	float parameter_values[MAX_SOUND_PARAMETERS];
	float volume;
	enum immediate_sound_types sound_type;
};

struct ghsound_music_info {
	enum ghsound_types ghsound;
	float volume;
	boolean stop_music;
};

struct ghsound_level_ambient_info {
	enum ghsound_types ghsound;
	float volume;
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


/* 
 * SOUNDSET DEFINITIONS 
 */
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

extern struct effect_sound_definition ui_sounds[MAX_UI_SOUND_TYPES];
extern struct effect_sound_definition sfx_sounds[MAX_SFX_SOUND_TYPES];




#endif /* SOUNDSET_H */
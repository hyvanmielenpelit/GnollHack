/*
 * soundset.h
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "action.h" 

#ifndef SOUNDSET_H
#define SOUNDSET_H

#define BACKGROUND_MUSIC_VOLUME 0.04f


enum ghsound_types {
	GHSOUND_NOSOUND = 0,
	GHSOUND_DUNGEON_NORMAL_MUSIC_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_GOING_BACK_UP,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_ATTACKED,
	GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_CLEARED,
	GHSOUND_PLAYER_FOOTSTEPS_NORMAL,
	GHSOUND_PLAYER_STRIKE_GENERAL,
	GHSOUND_PLAYER_HIT_GENERAL,
	GHSOUND_PLAYER_MISS_GENERAL,
	MAX_GHSOUNDS
};


enum ghmovement_types {
	GHMOVEMENT_ON_GROUND = 0, 
	GHMOVEMENT_STEALTH,
	GHMOVEMENT_FLYING,
	GHMOVEMENT_LEVITATING,
	GHMOVEMENT_TUNNELING,
	GHMOVEMENT_PASSING_THROUGH_WALLS,
	MAX_GHMOVEMENTS
};

enum ghfloor_types {
	GHFLOOR_NO_FLOOR = 0,
	GHFLOOR_STONE_FLOOR,
	GHFLOOR_WOODEN_FLOOR,
	GHFLOOR_GRASS,
	GHFLOOR_SOIL,
	GHFLOOR_WATER,
	MAX_GHFLOORS
};

enum ghshoe_types {
	GHSHOE_NO_SHOE = 0,
	GHSHOE_SOFT_SHOE,
	GHFLOOR_METAL_SHOE,
	MAX_GHSHOES
};




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
	enum ghfloor_types ghfloor;
	enum ghshoe_types ghshoe;
	float weight;
	float specific_volume_adjustment;
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





enum player_soundset_types {
	PLAYER_SOUNDSET_NO_SOUNDSET = 0,
	PLAYER_SOUNDSET_GENERAL,
	PLAYER_SOUNDSET_FEMALE_ELVEN_WIZARD,
	MAX_PLAYER_SOUNDSETS
};

struct player_soundset_definition {
	/* Ambient */
	enum ghsound_types stand;

	/* Movement */
	enum ghsound_types movement_sounds[MAX_GHMOVEMENTS];

	/* Actions */
	enum ghsound_types action_sounds[MAX_ACTION_TILES];
};

extern struct player_soundset_definition player_soundsets[MAX_PLAYER_SOUNDSETS + 1];


/* Monster soundsets */
enum monster_soundset_types {
	MONSTER_SOUNDSET_NONE = 0,
	MONSTER_SOUNDSET_GENERAL,
	MONSTER_SOUNDSET_GOBLIN,
	MONSTER_SOUNDSET_DRAGON,
	MONSTER_SOUNDSET_DRACOLICH,
	MAX_MONSTER_SOUNDSETS
};


struct monster_soundset_definition {
	/* Ambient */
	enum ghsound_types stand;

	/* Movement */
	enum ghsound_types movement_sounds[MAX_GHMOVEMENTS];

	/* Actions */
	enum ghsound_types action_sounds[MAX_ACTION_TILES];
};

extern struct monster_soundset_definition monster_soundsets[MAX_MONSTER_SOUNDSETS + 1];


enum item_soundset_types {
	ITEM_SOUNDSET_NO_SOUNDSET = 0,
	ITEM_SOUNDSET_GENERAL,
	ITEM_SOUNDSET_CANDLE,
	MAX_ITEM_SOUNDSETS
};

enum location_soundset_types {
	LOCATION_SOUNDSET_NO_SOUNDSET = 0,
	LOCATION_SOUNDSET_GENERAL,
	LOCATION_SOUNDSET_FOUNTAIN,
	MAX_LOCATION_SOUNDSETS
};




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


#endif /* SOUNDSET_H */
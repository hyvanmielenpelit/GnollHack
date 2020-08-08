/*
 * Actions. Copyright (c) Janne Gustafsson, 2020
 *
 */

#ifndef ACTION_H
#define ACTION_H

/* Action tiles */
enum action_tile_types
{
    ACTION_TILE_NO_ACTION = 0,
    ACTION_TILE_ATTACK = 1,
    ACTION_TILE_THROW,
    ACTION_TILE_FIRE,
    ACTION_TILE_CAST,
    ACTION_TILE_SPECIAL_ATTACK,
    ACTION_TILE_KICK,
    ACTION_TILE_PASSIVE_DEFENSE,
    ACTION_TILE_DEFEND,
    ACTION_TILE_RECEIVE_DAMAGE,
    ACTION_TILE_ITEM_USE,
    ACTION_TILE_DOOR_USE,
    ACTION_TILE_DEATH,
    MAX_ACTION_TILES
};


/*	Max # of attacks for any given monster. -- Moved here from permonst.h --JG
 */

#define NATTK 8	    /* Maximum number of attacks per monster */
#define NRAYS 10	/* Maximum number of rays */


 /* [misnamed] definition of a type of object; many objects are composites
	(liquid potion inside glass bottle, metal arrowhead on wooden shaft)
	and object definitions only specify one type on a< best-fit basis */
enum obj_material_types {
	MAT_NONE = 0,
	MAT_LIQUID = 1, /* currently only for venom */  /* Organics start here */
	MAT_OIL = 2, /* flammable liquid */
	MAT_WAX = 3,
	MAT_VEGGY = 4, /* foodstuffs */
	MAT_FLESH = 5, /*   ditto    */
	MAT_ORGANIC = 6, /* non-veggy, non-flesh organic material, e.g. bat guano, feathers */
	MAT_PAPER = 7,
	MAT_CLOTH = 8,
	MAT_SILK = 9,
	MAT_LEATHER = 10, /* Flimsy materials stop here */
	MAT_WOOD = 11, /* Organics stop here */
	MAT_BONE = 12,
	MAT_CHITIN = 13,
	MAT_TOOTH = 14, /* Hard bone-like materials stop here */
	MAT_DRAGON_HIDE = 15, /* not leather! */
	MAT_IRON = 16, /* Fe - includes steel */
	MAT_METAL = 17, /* Sn, &c. */
	MAT_COPPER = 18, /* Cu - includes brass */
	MAT_SILVER = 19, /* Ag */
	MAT_GOLD = 20, /* Au */
	MAT_PLATINUM = 21, /* Pt */
	MAT_ORICHALCUM = 22,
	MAT_ADAMANTIUM = 23,
	MAT_MITHRIL = 24,
	MAT_PLASTIC = 25,
	MAT_GLASS = 26,
	MAT_GEMSTONE = 27,
	MAT_MINERAL = 28,
	MAT_MODRONITE = 29,
	MAT_PLANARRIFT = 30,
	MAT_FORCEFIELD = 31,
	MAT_AIR = 32,
	MAT_FIRE = 33,
	MAT_ENERGY = 34,
	MAT_INCORPOREAL = 35,
	MAT_ICE = 36,
	MAT_SOIL = 37,
	MAX_MATERIAL_TYPES
};

enum material_phase {
	PHASE_SOLID = 0,
	PHASE_LIQUID,
	PHASE_GAS,
	PHASE_ENERGY,
	PHASE_VOID
};

/* Other definitions */
enum hit_surface_source_types {
	HIT_SURFACE_SOURCE_NONE = 0,
	HIT_SURFACE_SOURCE_MONSTER,
	HIT_SURFACE_SOURCE_OBJECT,
	HIT_SURFACE_SOURCE_LOCATION,
	HIT_SURFACE_SOURCE_TRAP
};

enum hit_surface_types {
	HIT_SURFACE_NONE = 0,
	HIT_SURFACE_FLESH,
	HIT_SURFACE_ORGANIC, /* Jellies */
	HIT_SURFACE_BONE,  /* Also chitin etc. */
	HIT_SURFACE_LEATHER,
	HIT_SURFACE_METAL,
	HIT_SURFACE_WOOD,
	HIT_SURFACE_STONE,
	HIT_SURFACE_GLASS,
	HIT_SURFACE_LIQUID,
	HIT_SURFACE_IMMATERIAL,
	MAX_HIT_SURFACE_TYPES
};


/* Movement paramters (+weight) */
enum floor_surface_types {
	FLOOR_SURFACE_NONE = 0,
	FLOOR_SURFACE_STONE,
	FLOOR_SURFACE_WOOD,
	FLOOR_SURFACE_GROUND,
	FLOOR_SURFACE_GRASS,
	FLOOR_SURFACE_METAL,
	FLOOR_SURFACE_CARPET,
	FLOOR_SURFACE_LIQUID,
	MAX_FLOOR_SURFACE_TYPES
};

enum floor_treading_types {
	FLOOR_TREADING_TYPE_NORMAL = 0,
	FLOOR_TREADING_TYPE_STEALTH,
	FLOOR_TREADING_TYPE_RUN,
	MAX_FLOOR_TREADING_TYPES
};

enum climbing_types {
	CLIMBING_TYPE_NONE = 0,
	CLIMBING_TYPE_STAIRS_UP,
	CLIMBING_TYPE_STAIRS_DOWN,
	CLIMBING_TYPE_LADDER_UP,
	CLIMBING_TYPE_LADDER_DOWN,
	MAX_CLIMBING_TYPES
};

enum location_passing_types {
	LOCATION_PASSING_TYPE_NORMAL = 0,
	LOCATION_PASSING_TYPE_WATERWALKING,			/* Has waterwalking on water or lava, so does not strictly splash the water or lava */
	LOCATION_PASSING_TYPE_THROUGH_SOLID_WALLS,  /* Like a ghost, xorn or earth elemental; sound inside solid wall, maybe normal while not inside? */
	LOCATION_PASSING_TYPE_SUBMERGED,			/* Maybe an amulet of magical breathing and no swimming */
	MAX_LOCATION_PASSING_TYPES
};


#endif /* ACTION_H */
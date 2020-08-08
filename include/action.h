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

#endif /* ACTION_H */
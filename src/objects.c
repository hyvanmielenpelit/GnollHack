/* GnollHack 4.0	objects.c	$NHDT-Date: 1535422421 2018/08/28 02:13:41 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.51 $ */
/* Copyright (c) Mike Threepoint, 1989.                           */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * The data in this file is processed twice, to construct two arrays.
 * On the first pass, only object name and object description matter.
 * On the second pass, all object-class fields except those two matter.
 * 2nd pass is a recursive inclusion of this file, not a 2nd compilation.
 * The name/description array is also used by makedefs and lev_comp.
 *
 * #ifndef OBJECTS_PASS_2_
 * # define OBJECT(name,desc,foo,bar,glorkum) name,desc
 * struct objdescr obj_descr[] =
 * #else
 * # define OBJECT(name,desc,foo,bar,glorkum) foo,bar,glorkum
 * struct objclass objects[] =
 * #endif
 * {
 *   { OBJECT("strange object",NULL, 1,2,3) },
 *   { OBJECT("arrow","pointy stick", 4,5,6) },
 *   ...
 *   { OBJECT(NULL,NULL, 0,0,0) }
 * };
 * #define OBJECTS_PASS_2_
 * #include "objects.c"
 */

/* *INDENT-OFF* */
/* clang-format off */

#ifndef OBJECTS_PASS_2_
/* first pass */
struct monst { struct monst *dummy; };  /* lint: struct obj's union */
#include "config.h"
#include "obj.h"
#include "objclass.h"
#include "prop.h"
#include "skills.h"
#include "attrib.h"

#else /* !OBJECTS_PASS_2_ */
/* second pass */
#include "color.h"
#define COLOR_FIELD(X) X,
#endif /* !OBJECTS_PASS_2_ */

/* objects have symbols: ) [ = " ( % ! ? + / $ * ` 0 _ . */

/*
 *      Note:  OBJ() and BITS() macros are used to avoid exceeding argument
 *      limits imposed by some compilers.  The ctnr field of BITS currently
 *      does not map into struct objclass, and is ignored in the expansion.
 *      The 0 in the expansion corresponds to oc_pre_discovered, which is
 *      set at run-time during role-specific character initialization.
 */

#ifndef OBJECTS_PASS_2_
/* first pass -- object descriptive text */
#define OBJ(name,desc)  name, desc
#define OBJECT(obj,bits,prp1,prp2,prp3,sym,prob,dly,wt, \
               cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,oc1,oc2,oc3,oc4,nut,color, dirsubtype, materials, flags)  { obj }
#define None (char *) 0 /* less visual distraction for 'no description' */

NEARDATA struct objdescr obj_descr[] =
#else
/* second pass -- object definitions */
#define BITS(nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,mtrl) \
  nmkn,mrg,uskn,0,mgc,chrg,uniq,nwsh,big,tuf,dir,mtrl,sub /*SCO cpp fodder*/
#define OBJECT(obj,bits,prp1,prp2,prp3,sym,prob,dly,wt,cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,oc1,oc2,oc3,oc4,nut,color,dirsubtype,materials,flags) \
  { 0, 0, (char *) 0, bits, prp1, prp2, prp3, sym, dly, COLOR_FIELD(color) prob, wt, \
    cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, oc1, oc2, oc3, oc4, nut, dirsubtype, materials, flags }
#ifndef lint
#define HARDGEM(n) (n >= 8)
#else
#define HARDGEM(n) (0)
#endif

NEARDATA struct objclass objects[] =
#endif
{
/* dummy object[0] -- description [2nd arg] *must* be NULL */
OBJECT(OBJ("strange object", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0),
       0, 0, 0, ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),

/* weapons ... */
#define WEAPON(name,desc,kn,mg,bi,prob,wt,                \
               cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,hitbon,typ,sub,metal,color, flags) \
    OBJECT(OBJ(name,desc),                                          \
           BITS(kn, mg, 1, 0, 0, 1, 0, 0, bi, 0, typ, sub, metal),  \
           0, 0, 0, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, 0, 0, wt, color, 0, 0, flags)
#define PROJECTILE(name,desc,kn,prob,wt,                  \
                   cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,hitbon,metal,sub,color, flags) \
    OBJECT(OBJ(name,desc),                                          \
           BITS(kn, 1, 1, 0, 0, 1, 0, 0, 0, 0, PIERCE, sub, metal), \
           0, 0, 0, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, 0, 0, wt, color, 0, 0, flags)
#define BOW(name,desc,kn,bi,prob,wt,cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,hitbon,metal,sub,color, flags) \
    OBJECT(OBJ(name,desc),                                          \
           BITS(kn, 0, 1, 0, 0, 1, 0, 0, bi, 0, 0, sub, metal),      \
           0, 0, 0, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, 0, 0, wt, color, 0, 0, flags)

/* Note: for weapons that don't do an even die of damage (ex. 2-7 or 3-18)
   the extra damage is added on in weapon.c, not here! */

#define P PIERCE
#define S SLASH
#define B WHACK

/* missiles; materiel reflects the arrowhead, not the shaft */
PROJECTILE("arrow", None,
           1, 55, 1, 2, 1, 6, 0, 1, 6, 0, 0,        IRON, -P_BOW, HI_METAL, O1_NONE),
PROJECTILE("elven arrow", "runed arrow",
           0, 20, 1, 2, 1, 7, 0, 1, 6, 0, 0,        WOOD, -P_BOW, HI_WOOD, O1_NONE),
PROJECTILE("orcish arrow", "crude arrow",
           0, 20, 1, 2, 1, 5, 0, 1, 6, 0, 0,        IRON, -P_BOW, CLR_BLACK, O1_NONE),
PROJECTILE("silver arrow", None,
           1, 12, 1, 5, 1, 6, 0, 1, 6, 0, 0,        SILVER, -P_BOW, HI_SILVER, O1_NONE),
PROJECTILE("ya", "bamboo arrow",
           0, 15, 1, 4, 1, 7, 0, 1, 7, 0, 1,        METAL, -P_BOW, HI_METAL, O1_NONE),
PROJECTILE("bone arrow", None,
           1, 10, 1, 6, 1, 4, 0, 1, 5, 0, 0,        BONE, -P_BOW, CLR_WHITE, O1_NONE),
PROJECTILE("crossbow bolt", None,
           1, 40, 1, 2, 1, 3, 1, 1, 4, 1, 0,        IRON, -P_CROSSBOW, HI_METAL, O1_NONE), //1d3+1 //1d4+1
PROJECTILE("gnollish quarrel", "crude crossbow bolt",
           0, 15, 1, 2, 1, 2, 1, 1, 3, 1, 0,        IRON, -P_CROSSBOW, HI_METAL, O1_NONE), //1d2+1 //1d3+1
PROJECTILE("bone quarrel", None,
           1, 10, 1, 2, 1, 2, 0, 1, 3, 0, 0,        BONE, -P_CROSSBOW, CLR_WHITE, O1_NONE), //1d2 //1d3

/* missiles that don't use a launcher */
WEAPON("dart", None,
       1, 1, 0, 50,   1,   1, 2, 0, 1, 3, 0,  2, 0, P,   -P_DART, IRON, HI_METAL, O1_NONE),
WEAPON("shuriken", "throwing star",
       0, 1, 0, 25,   1,   1, 5, 0, 1,  8, 0,  6, 2, P,   -P_SHURIKEN, IRON, HI_METAL, O1_NONE),
WEAPON("boomerang", None,
       1, 1, 0, 15,   5,  20,  1, 9, 0, 1, 9, 0, 0, 0,   -P_BOOMERANG, WOOD, HI_WOOD, O1_NONE),

/* spears [note: javelin used to have a separate skill from spears,
   because the latter are primarily stabbing weapons rather than
   throwing ones; but for playability, they've been merged together
   under spear skill and spears can now be thrown like javelins] */
WEAPON("spear", None,
       1, 1, 0, 50,  30,   3,  1, 6, 0, 1,  8, 0,  0, P,   P_SPEAR, IRON, HI_METAL, O1_NONE),
WEAPON("elven spear", "runed spear",
       0, 1, 0, 10,  30,   3,  1, 7, 0, 1,  8, 0, 0, P,   P_SPEAR, WOOD, HI_WOOD, O1_NONE),
WEAPON("orcish spear", "crude spear",
       0, 1, 0, 13,  30,   3,  1, 5, 0, 1, 8, 0, 0, P,   P_SPEAR, IRON, CLR_BLACK, O1_NONE),
WEAPON("dwarvish spear", "stout spear",
       0, 1, 0, 12,  35,   3,  1, 8, 0, 1, 8, 0, 0, P,   P_SPEAR, IRON, HI_METAL, O1_NONE),
WEAPON("silver spear", None,
       1, 1, 0,  2,  36,  40,  1, 6, 0,  1, 8, 0, 0, P,   P_SPEAR, SILVER, HI_SILVER, O1_NONE),
WEAPON("javelin", "throwing spear",
       0, 1, 0, 10,  20,   3,  1, 6, 0, 1, 6, 0, 0, P,   P_SPEAR, IRON, HI_METAL, O1_NONE),

/* spearish; doesn't stack, not intended to be thrown */
WEAPON("trident", None,
       1, 0, 0,  8,  25,   5, 1,  6, 1, 3,  4, 0, 0, P,   P_TRIDENT, IRON, HI_METAL, O1_NONE),
        /* +1 small, +2d4 large */

/* blades; all stack */
WEAPON("dagger", None,
       1, 1, 0, 24,  10,   4,  1, 4, 0, 1,  3, 0, 2, P,   P_DAGGER, IRON, HI_METAL, O1_NONE),
WEAPON("elven dagger", "runed dagger",
       0, 1, 0, 10,  10,   4,  1, 5, 0, 1, 3, 0, 2, P,   P_DAGGER, WOOD, HI_WOOD, O1_NONE),
WEAPON("orcish dagger", "crude dagger",
       0, 1, 0, 12,  10,   4,  1, 3, 0, 1,  3, 0, 2, P,   P_DAGGER, IRON, CLR_BLACK, O1_NONE),
WEAPON("bone dagger", None,
       1, 1, 0, 6,  10,   4,  1, 3, 0, 1,  3, 0, 2, P,   P_DAGGER, BONE, CLR_WHITE, O1_NONE),
WEAPON("silver dagger", None,
       1, 1, 0,  3,  12,  40,  1, 4, 0, 1,  3, 0, 2, P,   P_DAGGER, SILVER, HI_SILVER, O1_NONE),
WEAPON("athame", None,
       1, 1, 0,  0,  10,   4,  1, 4, 0, 1,  3, 0, 2, S,   P_DAGGER, IRON, HI_METAL, O1_NONE),
WEAPON("scalpel", None,
       1, 1, 0,  0,   5,   6,  1, 3, 0, 1, 3, 0, 2, S,   P_KNIFE, METAL, HI_METAL, O1_NONE),
WEAPON("knife", None,
       1, 1, 0, 20,   5,   4,  1, 3, 0, 1, 2, 0, 0, P|S, P_KNIFE, IRON, HI_METAL, O1_NONE),
WEAPON("stiletto", None,
       1, 1, 0,  5,   5,   4, 1, 3, 0, 1, 2, 0, 0, P|S, P_KNIFE, IRON, HI_METAL, O1_NONE),
/* 3.6: worm teeth and crysknives now stack;
   when a stack of teeth is enchanted at once, they fuse into one crysknife;
   when a stack of crysknives drops, the whole stack reverts to teeth */
WEAPON("worm tooth", None,
       1, 1, 0,  0,  20,   2,  1, 2, 0, 1,  2, 0, 0, 0,   P_KNIFE, 0, CLR_WHITE, O1_NONE),
WEAPON("crysknife", None,
       1, 1, 0,  0,  20, 100, 1, 10, 0, 1, 10, 0, 3, P,   P_KNIFE, MINERAL, CLR_WHITE, O1_NONE),

/* axes */
WEAPON("axe", None,
       1, 0, 0, 40,  50,   8,  1, 6, 0, 1,  4, 0, 0, S,   P_AXE, IRON, HI_METAL, O1_NONE),
WEAPON("battle-axe", "double-headed axe",       /* "double-bitted"? */
       0, 0, 1, 10, 100,  40,  2, 6, 0, 3, 4, 1, 0, S,   P_AXE, IRON, HI_METAL, O1_NONE),

/* swords */
WEAPON("short sword", None,
       1, 0, 0,  8,  45,  10,  1, 6, 0, 1,  8, 0, 0, P,   P_SHORT_SWORD, IRON, HI_METAL, O1_NONE),
WEAPON("elven short sword", "runed short sword",
       0, 0, 0,  2,  40,  10,  1, 8, 0, 1, 8, 0, 0, P,   P_SHORT_SWORD, WOOD, HI_WOOD, O1_NONE),
WEAPON("orcish short sword", "crude short sword",
       0, 0, 0,  3,  50,  10,  1, 5, 0, 1,  8, 0, 0, P,   P_SHORT_SWORD, IRON, CLR_BLACK, O1_NONE),
WEAPON("dwarvish short sword", "broad short sword",
       0, 0, 0,  2,  50,  10,  1, 7, 0, 1, 8, 0, 0, P,   P_SHORT_SWORD, IRON, HI_METAL, O1_NONE),
WEAPON("scimitar", "curved sword",
       0, 0, 0, 15,  60,  15,  1, 8, 0, 1, 8, 0, 0, S,   P_SCIMITAR, IRON, HI_METAL, O1_NONE),
WEAPON("silver saber", None,
       1, 0, 0,  6,  55,  75,  1, 8, 0, 1, 8, 0, 0, S,   P_SABER, SILVER, HI_SILVER, O1_NONE),
WEAPON("broadsword", None,
       1, 0, 0,  8,  60,  10,  2, 4, 0, 1, 6, 1, 0, S,   P_BROAD_SWORD, IRON, HI_METAL, O1_NONE),
        /* +d4 small, +1 large */
WEAPON("elven broadsword", "runed broadsword",
       0, 0, 0,  4,  55,  10,  2, 4, 0, 1, 6, 1, 0, S,   P_BROAD_SWORD, WOOD, HI_WOOD, O1_NONE),
        /* +d4 small, +1 large */
WEAPON("long sword", None,
       1, 0, 0, 40,  60,  15,  1, 8, 0, 1, 12, 0, 0, S,   P_LONG_SWORD, IRON, HI_METAL, O1_NONE),
WEAPON("silver long sword", None,
	   1, 0, 0, 0, 55, 500, 1, 8, 0, 1, 12, 0, 0, S, P_LONG_SWORD, SILVER, HI_SILVER, O1_NONE),
WEAPON("two-handed sword", None,
       1, 0, 1, 20, 120,  50, 1, 12, 0, 3, 6, 0, 0, S,   P_TWO_HANDED_SWORD,
                                                        IRON, HI_METAL, O1_NONE),
        /* +2d6 large */
WEAPON("katana", "samurai sword",
       0, 0, 0,  4,  80,  80, 1, 10, 0, 1, 12, 0, 1, S,   P_LONG_SWORD, IRON, HI_METAL, O1_NONE),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword",
       0, 0, 1,  0,  100, 500, 1, 16, 0, 3, 6, 1, 2, S,   P_TWO_HANDED_SWORD,
                                                            METAL, HI_METAL, O1_NONE),
        /* +2d6 large */
WEAPON("runesword", "runed broadsword",
       0, 0, 0,  0,  55, 300,  2, 4, 0, 1, 6, 1, 0, S,   P_BROAD_SWORD, IRON, CLR_BLACK, O1_NONE),
        /* +d4 small, +1 large; Stormbringer: +5d2 +d8 from level drain */

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
       0, 0, 1,  5,  80,  10,  1, 6, 0, 1, 6, 1, 0, P,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1 large */
WEAPON("ranseur", "hilted polearm",
       0, 0, 1,  5,  50,   6,  2, 4, 0, 2, 4, 0, 0, P,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +d4 both */
WEAPON("spetum", "forked polearm",
       0, 0, 1,  5,  50,  1, 5, 1, 2, 6, 0, 6, 0, P,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1 small, +d6 large */
WEAPON("glaive", "single-edged polearm",
       0, 0, 1,  8,  75,   6, 1, 6, 0, 1, 10, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
WEAPON("lance", None,
       1, 0, 0,  4, 180,  10, 1, 6, 0, 1, 8, 0, 0, P,   P_LANCE, IRON, HI_METAL, O1_NONE),
        /* +2d10 when jousting with lance as primary weapon */
/* axe-type */
WEAPON("halberd", "angled poleaxe",
       0, 0, 1,  8, 150,  10, 1, 10, 0, 2, 6, 0, 0, P|S, P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d6 large */
WEAPON("bardiche", "long poleaxe",
       0, 0, 1,  4, 120,   7,  2, 4, 0, 3, 4, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small, +2d4 large */
WEAPON("voulge", "pole cleaver",
       0, 0, 1,  4, 125,   5, 2, 4, 0, 2, 4, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +d4 both */
WEAPON("dwarvish mattock", "broad pick",
       0, 0, 1, 13, 120,  50, 1, 12, 0, 3, 6, 1, -1, B,  P_PICK_AXE, IRON, HI_METAL, O1_NONE),
/* curved/hooked */
WEAPON("fauchard", "pole sickle",
       0, 0, 1,  6,  60,   5,  1, 6, 0, 1, 8, 0, 0, P|S, P_POLEARMS, IRON, HI_METAL, O1_NONE),
WEAPON("guisarme", "pruning hook",
       0, 0, 1,  6,  80,   5,  2, 4, 0, 1, 8, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small */
WEAPON("bill-guisarme", "hooked polearm",
       0, 0, 1,  4, 120,   7,  2, 4, 0, 1, 10, 0, 0, P|S, P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small */
WEAPON("infernal ancus", "black ornamental hooked polearm",
       0, 0, 0,  0, 100, 100,  1, 6, 1, 1, 8, 1, 0, P|S, P_POLEARMS, IRON, CLR_BLACK, O1_NONE),
/* other */
WEAPON("lucern hammer", "pronged polearm",
       0, 0, 1,  5, 150,   7,  2, 4, 0, 1, 6, 0, 0, B|P, P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small */
WEAPON("bec de corbin", "beaked polearm",
       0, 0, 1,  4, 100,   1, 8, 0, 1,  8, 0,  6, 0, B|P, P_POLEARMS, IRON, HI_METAL, O1_NONE),

/* bludgeons */
WEAPON("mace", None,
       1, 0, 0, 38,  60,   5,  1, 6, 1, 1, 6, 0, 0, B,   P_MACE, IRON, HI_METAL, O1_NONE),
        /* +1 small */
WEAPON("silver mace", None,
	   1, 0, 0, 2, 55, 250, 1, 6, 1, 1, 6, 0, 0, B, P_MACE, SILVER, HI_SILVER, O1_NONE),
		/* +1 small */
WEAPON("morning star", None,
	   1, 0, 0, 12, 60,  10,  2, 4, 0, 1,  6, 1, 0, B,   P_MORNING_STAR, IRON, HI_METAL, O1_NONE),
        /* +d4 small, +1 large */
WEAPON("war hammer", None,
       1, 0, 0, 15,  50,   5,  1, 4, 1, 1, 4, 0, 0, B,   P_HAMMER, IRON, HI_METAL, O1_NONE),
        /* +1 small */
WEAPON("club", None,
       1, 0, 0, 12,  40,   3,  1, 6, 0, 1, 3, 0, 0, B,   P_CLUB, WOOD, HI_WOOD, O1_NONE),
WEAPON("infernal jagged-toothed club", "black jagged-toothed club",
       0, 0, 0,  0,  60, 100,  1, 6, 1, 1, 6, 1, 0, B,   P_CLUB, WOOD, CLR_BLACK, O1_NONE),
WEAPON("rubber hose", None,
       1, 0, 0,  0,  20,   3,  1, 4, 0, 1, 3, 0, 0, B,   P_WHIP, PLASTIC, CLR_BROWN, O1_NONE),
WEAPON("quarterstaff", "staff",
       0, 0, 1, 11,  30,   5,  1, 6, 0, 1, 6, 0, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD, O1_NONE),
/* two-piece */
WEAPON("aklys", "thonged club",
       0, 0, 0,  8,  15,   4,  1, 6, 0, 1, 3, 0, 0, B,   P_CLUB, IRON, HI_METAL, O1_NONE),
WEAPON("flail", None,
       1, 0, 0, 40,  35,   4,  1, 6, 1, 2, 4, 0, 0, B,   P_FLAIL, IRON, HI_METAL, O1_NONE),
	/* +1 small, +1d4 large */
WEAPON("double-headed flail", None,
	   1, 0, 0,  0,  55, 250, 1, 10, 1, 3, 4, 0, -1, B, P_FLAIL, IRON, HI_METAL, O1_NONE),
	/* one-handed, flail damage x c. 1.5, -1 hit bonus (difficult to wield)*/
WEAPON("triple-headed flail", None,
	   1, 0, 0, 0, 70, 1000, 2, 6, 2, 4, 4, 0, -2, B, P_FLAIL, IRON, HI_METAL, O1_NONE),
	/* one-handed, flail damage x 2, -2 hit bonus (difficult to wield)*/


/* misc */
WEAPON("bullwhip", None,
       1, 0, 0,  2,  20,   4,  1, 2, 0,  1, 1, 0,  0, 0,   P_WHIP, LEATHER, CLR_BROWN, O1_NONE),
WEAPON("black blade of disintegration", "sword-shaped planar rift",
   	   0, 1, 1,  0,  0,    0,  1, 16, 0,  3, 6, 1, 0, S, P_TWO_HANDED_SWORD, PLANARRIFT, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),

/* bows */
BOW("bow", None,							1, 1, 0,  30, 30, 1, 3, 0, 1, 4, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("short bow", None,						1, 1, 12, 30, 30, 1, 3, 0, 1, 4, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("elven long bow", "runed long bow",		0, 1, 12, 40, 200, 1, 5, 0, 1, 7, 0, 1, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("orcish short bow", "crude short bow",  0, 1, 12, 35, 10, 1, 2, 0, 1, 3, 0, 0, WOOD, P_BOW, CLR_BLACK, O1_NONE),
BOW("long bow", None,						1, 1, 12, 45, 75, 1, 4, 0, 1, 6, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("composite long bow", None,				1, 1, 6,  50, 250, 1, 5, 1, 1, 7, 1, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("composite short bow", None,			1, 1, 6,  35, 100, 1, 4, 1, 1, 5, 1, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("yumi", "oriental long bow",			0, 1, 0,  55, 150, 1, 4, 0, 1, 8, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("sling", None,							1, 0, 40,  3, 20, 1, 2, 0, 1, 4, 0, 0, LEATHER, P_SLING, HI_LEATHER, O1_NONE),
BOW("crossbow", None,						1, 1, 30, 100, 75, 1, 5, 0, 1, 5, 0, 0, WOOD, P_CROSSBOW, HI_WOOD, O1_NONE),
BOW("hand crossbow", None,					1, 0, 5,  40, 50, 1, 2, 0, 1, 2, 0, 0, WOOD, P_CROSSBOW, HI_WOOD, O1_NONE),
BOW("heavy crossbow", None,					1, 1, 10, 200, 150, 1, 8, 0, 1, 8, 0, 0, WOOD, P_CROSSBOW, HI_WOOD, O1_NONE),

#undef P
#undef S
#undef B

#undef WEAPON
#undef PROJECTILE
#undef BOW

/* armor ... */
        /* IRON denotes ferrous metals, including steel.
         * Only IRON weapons and armor can rust.
         * Only COPPER (including brass) corrodes.
         * Some creatures are vulnerable to SILVER.
         */
#define ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,  \
              cost,ac,can,sub,metal,c, flags)                   \
    OBJECT(OBJ(name, desc),                                         \
           BITS(kn, 0, 1, 0, mgc, 1, 0, 0, blk, 0, 0, sub, metal),  \
           power, 0, 0, ARMOR_CLASS, prob, delay, wt,                     \
           cost, 0, 0, 0, 0, 0, 0, 10 - ac, can, 0, 0, wt, c, 0, 0, flags)
#define HELM(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_HELM, metal, c, flags)
#define CLOAK(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_CLOAK, metal, c, flags)
#define SHIELD(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,metal,c,flags) \
    ARMOR(name, desc, kn, mgc, blk, power, prob, delay, wt, \
          cost, ac, can, ARM_SHIELD, metal, c, flags)
#define GLOVES(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_GLOVES, metal, c, flags)
#define BOOTS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_BOOTS, metal, c,flags)
#define SHIRT(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_SHIRT, metal, c, flags)
#define ROBE(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_ROBE, metal, c, flags)
#define BRACERS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_BRACERS, metal, c, flags)
#define BELT(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_BELT, metal, c, flags)
#define PANTS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, prob, delay, wt,  \
          cost, ac, can, ARM_PANTS, metal, c, flags)

/* helmets */
HELM("elven leather helm", "leather hat",
     0, 0,           0,  6, 1,  3,  8,  9, 0, LEATHER, HI_LEATHER, O1_NONE),
HELM("cotton hood", None,
     1, 0,           0,  3, 1,  3,  8,  10, 0, CLOTH, CLR_BLACK, O1_NONE),
HELM("gnollish hood", "crude leather hood",
     0, 0,           0,  3, 1,  3,  8,  9, 0, LEATHER, HI_LEATHER, O1_NONE),
HELM("orcish helm", "iron skull cap",
     0, 0,           0,  6, 1, 30, 10,  9, 0, IRON, CLR_BLACK, O1_NONE),
HELM("dwarvish iron helm", "hard hat",
     0, 0,           0,  6, 1, 40, 20,  8, 0, IRON, HI_METAL, O1_NONE),
HELM("fedora", None,
     1, 0,           0,  0, 0,  3,  1, 10, 0, CLOTH, CLR_BROWN, O1_NONE),
HELM("cornuthaum", "conical hat",
     0, 1, CLAIRVOYANT,  3, 1,  4, 80, 10, 1, CLOTH, CLR_BLUE, O1_NONE),
        /* name coined by devteam; confers clairvoyance for wizards,
           blocks clairvoyance if worn by role other than wizard */
HELM("dunce cap", "conical hat",
     0, 1,           0,  3, 1,  4,  1, 10, 0, CLOTH, CLR_BLUE, O1_NONE),
HELM("dented pot", None,
     1, 0,           0,  2, 0, 30,  8,  9, 0, IRON, CLR_BLACK, O1_NONE),
/* with shuffled appearances... */
HELM("helmet", "plumed helmet",
     0, 0,           0, 10, 1, 50, 10,  9, 0, IRON, HI_METAL, O1_NONE),
HELM("helm of brilliance", "etched helmet",
     0, 1,           0,  6, 1, 50, 50,  9, 0, IRON, CLR_GREEN, O1_NO_SPELL_CASTING_PENALTY),
HELM("helm of opposite alignment", "crested helmet",
     0, 1,           0,  6, 1, 50, 50,  9, 0, IRON, HI_METAL, O1_NONE),
HELM("helm of telepathy", "visored helmet",
     0, 1,     TELEPAT,  2, 1, 50, 50,  9, 0, IRON, HI_METAL, O1_HALF_SPELL_CASTING_PENALTY),

/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in obj.h, objnam.c, mon.c, read.c that assumes (2).
 *      (1) The dragon scale mails and the dragon scales are together.
 *      (2) That the order of the dragon scale mail and dragon scales
 *          is the same as order of dragons defined in monst.c.
 */
#define DRGN_ARMR(name,mgc,power,cost,ac,color,flags)  \
    ARMOR(name, None, 1, mgc, 1, power, 0, 5, 550,  \
          cost, ac, 0, ARM_SUIT, DRAGON_HIDE, color, flags)
/* 3.4.1: dragon scale mail reclassified as "magic" since magic is
   needed to create them */
DRGN_ARMR("gray dragon scale mail",    1, ANTIMAGIC,  6000, 1, CLR_GRAY, O1_NONE),
DRGN_ARMR("silver dragon scale mail",  1, REFLECTING, 6000, 1, DRAGON_SILVER, O1_NONE),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scale mail", 1, DISPLACED, 1200, 1, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scale mail",     1, FIRE_RES,   4000, 1, CLR_RED, O1_FIRE_RESISTANT),
DRGN_ARMR("white dragon scale mail",   1, COLD_RES,   3000, 1, CLR_WHITE, O1_COLD_RESISTANT),
DRGN_ARMR("orange dragon scale mail",  1, SLEEP_RES,  4000, 1, CLR_ORANGE, O1_NONE),
DRGN_ARMR("black dragon scale mail",   1, DISINT_RES, 5000, 1, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),
DRGN_ARMR("blue dragon scale mail",    1, SHOCK_RES,  4000, 1, CLR_BLUE, O1_LIGHTNING_RESISTANT),
DRGN_ARMR("green dragon scale mail",   1, POISON_RES, 3000, 1, CLR_GREEN, O1_NONE),
DRGN_ARMR("yellow dragon scale mail",  1, ACID_RES,   3000, 1, CLR_YELLOW, O1_NONE),
/* For now, only dragons leave these. */
/* 3.4.1: dragon scales left classified as "non-magic"; they confer
   magical properties but are produced "naturally" */
DRGN_ARMR("gray dragon scales",        0, ANTIMAGIC,  1500, 7, CLR_GRAY, O1_NONE),
DRGN_ARMR("silver dragon scales",      0, REFLECTING, 1500, 7, DRAGON_SILVER, O1_NONE),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scales",  0, DISPLACED,   700, 7, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scales",         0, FIRE_RES,   1000, 7, CLR_RED, O1_FIRE_RESISTANT),
DRGN_ARMR("white dragon scales",       0, COLD_RES,    750, 7, CLR_WHITE, O1_COLD_RESISTANT),
DRGN_ARMR("orange dragon scales",      0, SLEEP_RES,  1000, 7, CLR_ORANGE, O1_NONE),
DRGN_ARMR("black dragon scales",       0, DISINT_RES, 1250, 7, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),
DRGN_ARMR("blue dragon scales",        0, SHOCK_RES,  1000, 7, CLR_BLUE, O1_LIGHTNING_RESISTANT),
DRGN_ARMR("green dragon scales",       0, POISON_RES,  750, 7, CLR_GREEN, O1_NONE),
DRGN_ARMR("yellow dragon scales",      0, ACID_RES,    750, 7, CLR_YELLOW, O1_NONE),
#undef DRGN_ARMR
/* other suits */
ARMOR("full plate mail", None,
	  1, 0, 1,			 0, 10, 7, 960,2000,  1, 2,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("field plate mail", None,
	  1, 0, 1,			 0, 20, 6, 840,1000,  2, 2,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("plate mail", None,
      1, 0, 1,			 0, 40, 5, 720, 600,  3, 2,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("crystal plate mail", None,
      1, 0, 1,  REFLECTING,  5, 5, 640,3000,  3, 2,  ARM_SUIT, GLASS, CLR_WHITE, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT | O1_HALF_SPELL_CASTING_PENALTY),
ARMOR("bronze plate mail", None,
      1, 0, 1,			 0, 20, 5, 800, 150,  4, 1,  ARM_SUIT, COPPER, HI_COPPER, O1_NONE),
ARMOR("splint mail", None,
      1, 0, 1,			 0, 40, 5, 640, 125,  4, 1,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("banded mail", None,
      1, 0, 1,			 0, 42, 5, 560, 200,  4, 1,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("dwarvish mithril-coat", None,
      1, 0, 0,			 0, 10, 1, 250, 750,  4, 2,  ARM_SUIT, MITHRIL, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT | O1_HALF_SPELL_CASTING_PENALTY),
ARMOR("elven mithril-coat", None,
      1, 0, 0,			 0, 15, 1, 150, 250,  5, 2,  ARM_SUIT, MITHRIL, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT | O1_HALF_SPELL_CASTING_PENALTY),
ARMOR("chain mail", None,
      1, 0, 0,			 0, 45, 5, 480,  75,  5, 1,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("orcish chain mail", "crude chain mail",
      0, 0, 0,			 0, 20, 5, 450,  40,  6, 1,  ARM_SUIT, IRON, CLR_BLACK, O1_NONE),
ARMOR("scale mail", None,
      1, 0, 0,			 0, 45, 5, 500,  45,  6, 1,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("studded leather armor", None,
      1, 0, 0,			 0, 45, 3, 200,  15,  7, 1,  ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("gnollish studded leather armor", "crude studded leather armor",
      0, 0, 0,			 0, 12, 3, 175,  15,  8, 1,  ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("ring mail", None,
      1, 0, 0,			 0, 45, 5, 500,  30,  7, 1,  ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("orcish ring mail", "crude ring mail",
      0, 0, 0,			 0, 20, 5, 450,  15,  8, 1,  ARM_SUIT, IRON, CLR_BLACK, O1_NONE),
ARMOR("leather armor", None,
      1, 0, 0,			 0, 50, 3, 150,   5,  8, 1,  ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("gnollish leather armor", "crude leather armor",
      0, 0, 0,			 0, 12, 3, 125,   3,  9, 1,  ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("leather jacket", None,
      1, 0, 0,			 0, 12, 0,  30,  10,  9, 0,  ARM_SUIT, LEATHER, CLR_BLACK, O1_NONE),
ARMOR("force field armor", "armor-shaped force field",
      0, 1, 0,			 0, 0,  0,   0,   4,  4, 0,  ARM_SUIT, FORCEFIELD, CLR_WHITE, O1_NO_SPELL_CASTING_PENALTY | O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT | O1_COLD_RESISTANT | O1_CORROSION_RESISTANT | O1_RUST_RESISTANT),

/* shirts */
ARMOR("Hawaiian shirt", None, //STARTMARKER 1
    1, 0, 0, 0,			 8, 0, 5,  5, 10, 0, ARM_SHIRT, CLOTH, CLR_MAGENTA, O1_NONE),
/* with shuffled appearances... */
ARMOR("shirt of uncontrollable laughter", "funny T-shirt", //STARTMARKER 2
    0, 1, 0,  LAUGHING,  6, 0, 5, 50, 10, 0, ARM_SHIRT, CLOTH, CLR_BLUE, O1_NONE),
ARMOR("shirt of comeliness", "black T-shirt",
	0, 1, 0, 0,			 6, 0, 5, 50, 10, 0, ARM_SHIRT, CLOTH, CLR_BLACK, O1_NONE),
ARMOR("shirt of sound mindedness", "green T-shirt", 
	0, 1, 0, HALLUC_RES, 6, 0, 5, 50, 10, 0, ARM_SHIRT, CLOTH, CLR_GREEN, O1_NONE),
ARMOR("T-shirt", "old T-shirt",//ENDMARKER 1 & 2
    0, 0, 0, 0,			 4, 0, 5,  2, 10, 0, ARM_SHIRT, CLOTH, CLR_WHITE, O1_NONE),


/* cloaks */
CLOAK("elven cloak", "faded pall",
      0, 1,    STEALTH,  8, 0, 10, 60,  9, 1,  CLOTH, CLR_BLACK, O1_NONE),
CLOAK("orcish cloak", "coarse mantelet",
      0, 0,				 0, 8,  0, 10, 40, 10, 1,  CLOTH, CLR_BLACK, O1_NONE),
CLOAK("dwarvish cloak", "hooded cloak",
      0, 0,			     0, 8,  0, 10, 50, 10, 1,  CLOTH, HI_CLOTH, O1_NONE),
CLOAK("oilskin cloak", "slippery cloak",
      0, 0,				 0, 8,  0, 10, 50,  9, 2,  CLOTH, HI_CLOTH, O1_NONE),
CLOAK("alchemy smock", "apron",
      0, 1, POISON_RES,  9, 0, 10, 50,  9, 1,  CLOTH, CLR_WHITE, O1_NONE),
CLOAK("leather cloak", None,
      1, 0,				 0, 8,  0, 15, 40,  9, 1,  LEATHER, CLR_BROWN, O1_NONE),
/* with shuffled appearances... */
CLOAK("cloak of protection", "tattered cape",
      0, 1, PROTECTION,  9, 0, 10, 250,  7, 3,  CLOTH, HI_CLOTH, O1_NONE),
        /* cloak of protection is now the only item conferring MC 3 */
CLOAK("cloak of invisibility", "opera cloak",
      0, 1,      INVIS,  9, 0, 10, 250,  9, 1,  CLOTH, CLR_BRIGHT_MAGENTA, O1_NONE),
CLOAK("cloak of magic resistance", "ornamental cope",
      0, 1,  ANTIMAGIC,  6, 0, 10, 250,  9, 1,  CLOTH, CLR_GRAY, O1_NONE),
        /*  'cope' is not a spelling mistake... leave it be */
CLOAK("cloak of integrity", "hard woven cloak",
      0, 1,  DISINT_RES, 6, 0, 10, 250,  8, 1,  CLOTH, CLR_WHITE, O1_DISINTEGRATION_RESISTANT | O1_FIRE_RESISTANT),
CLOAK("cloak of displacement", "piece of cloth",
      0, 1,  DISPLACED,  7, 0, 10, 250,  9, 1,  CLOTH, HI_CLOTH, O1_NONE),

/* robes and gowns */
/* with shuffled appearances... */
ROBE("robe", "old robe", //STARTMARKER 1 & 2
	0, 0, 0,		  10, 1, 20,  25,  10, 2, CLOTH, CLR_RED, O1_NONE),
ROBE("robe of protection", "golden ornamental robe",
	0, 1, PROTECTION, 10, 1, 25, 400,  7, 3, CLOTH, HI_GOLD, O1_NONE),
ROBE("robe of magic resistance", "silvery ornamental robe",
	0, 1, ANTIMAGIC,   9, 1, 25, 400,  9, 2, CLOTH, HI_SILVER, O1_NONE),
ROBE("robe of the archmagi", "shining blue robe",
	0, 1, 0,		   5, 2, 30, 400,  9, 3, CLOTH, HI_ZAP, O1_NONE),
ROBE("robe of starry wisdom", "shining black robe", //ENDMARKER 2
	0, 1, 0,		   5, 2, 30, 400,  9, 3, CLOTH, CLR_BLACK, O1_NONE),
/* without shuffled appearances... */
ROBE("bathrobe", "soft cotton robe",
	0, 0, 0,		   6, 1, 25,   5, 10, 0, CLOTH, CLR_WHITE, O1_NONE),
ROBE("clerical gown", None,
	1, 0, 0,		   6, 1, 15,  25, 10, 2, CLOTH, CLR_BLACK, O1_NONE),
ROBE("gnollish haircloth robe", "crude robe",
	0, 0, 0,		   6, 1, 30,  10,  9, 2, CLOTH, CLR_BROWN, O1_NONE),
ROBE("mummy wrapping", None, //ENDMARKER 1
	1, 0, 0,		   0, 0,  3,   2, 10, 1, CLOTH, CLR_GRAY, O1_NONE),
	/* worn mummy wrapping blocks invisibility */


/* bracers */
/* with shuffled appearances... */
BRACERS("leather bracers", "old bracers", //STARTMARKER 1 & 2
	0, 0, 0,		  8, 1, 10,   5, 9, 0, CLOTH, HI_LEATHER, O1_NONE),
BRACERS("bracers of defense", "runed bracers",
	0, 1, PROTECTION, 8, 1, 10, 300, 7, 2, CLOTH, HI_LEATHER, O1_NONE),
BRACERS("bracers of archery", "deerskin bracers",
	0, 1, 0,		  7, 1, 10, 300, 9, 0, CLOTH, HI_LEATHER, O1_NONE),
BRACERS("bracers of magic resistance", "ornamental bracers",//ENDMARKER 1 & 2
	0, 1, ANTIMAGIC,  8, 1, 10, 300, 9, 2, CLOTH, HI_LEATHER, O1_NONE),


/* belts */
/* with shuffled appearances... */
BELT("leather belt", "old belt", //STARTMARKER 1 & 2
	0, 0, 0,		 10, 0, 5,   5, 10, 0, CLOTH, HI_LEATHER, O1_NO_SPELL_CASTING_PENALTY),
BELT("belt of dwarvenkind", "broad belt", 
	0, 1, POISON_RES, 8, 0, 5, 250, 10, 1, CLOTH, HI_LEATHER, O1_NONE),
BELT("belt of giant strength", "ornamental belt", //ENDMARKER 1 & 2
	0, 1, 0,		  7, 0, 5, 250, 10, 0, CLOTH, HI_LEATHER, O1_NONE),

/* pants */
/* no shuffled appearances... */
PANTS("leather pants", None, //STARTMARKER
	1, 0, 0,		8, 2, 50, 10,  9, 0, CLOTH, HI_LEATHER, O1_NONE),
PANTS("beige shorts", "beige short pants",
	0, 0, 0,		3, 2, 15, 10, 10, 0, CLOTH, CLR_BROWN, O1_NONE),
PANTS("jeans", "blue long pants", 
	0, 0, 0,		3, 2, 50,  5, 10, 0, CLOTH, CLR_BLUE, O1_NONE),
PANTS("skirt", None, //STARTMARKER FOR NO "PAIR OF"
	1, 0, 0,		3, 1, 10,  5, 10, 0, CLOTH, CLR_RED, O1_NONE),
PANTS("kilt", None,  //ENDMARKER FOR NO "PAIR OF"
	1, 0, 0,		3, 1, 15, 10, 10, 0, CLOTH, CLR_MAGENTA, O1_NONE),
PANTS("trunks of swimming", "blue short pants", //ENDMARKER
	0, 1, SWIMMING, 3, 1,  3, 50, 10, 0, CLOTH, CLR_BLUE, O1_NONE),

/* shields */
SHIELD("small shield", None,
       1, 0, 0,          0, 6, 0,  80,  3, 9, 0,  WOOD, HI_WOOD, O1_NONE),
SHIELD("elven shield", "blue and green shield",
       0, 0, 0,          0, 2, 0,  140,  7, 8, 0,  WOOD, CLR_GREEN, O1_NONE),
SHIELD("Uruk-hai shield", "white-handed shield",
       0, 0, 0,          0, 2, 0,  120,  7, 9, 0,  IRON, HI_METAL, O1_NONE),
SHIELD("orcish shield", "red-eyed shield",
       0, 0, 0,          0, 2, 0,  100,  7, 9, 0,  IRON, CLR_RED, O1_NONE),
SHIELD("large shield", None,
       1, 0, 1,          0, 5, 0, 160, 10, 8, 0,  IRON, HI_METAL, O1_NONE),
SHIELD("dwarvish roundshield", "large round shield",
       0, 0, 0,          0, 4, 0, 150, 10, 8, 0,  IRON, HI_METAL, O1_NONE),
SHIELD("shield of reflection", "polished silver shield",
       0, 1, 0, REFLECTING, 3, 0, 70, 750, 8, 0,  SILVER, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT),

/* gloves */
/* These have their color but not material shuffled, so the IRON must
 * stay CLR_BROWN (== HI_LEATHER) even though it's normally either
 * HI_METAL or CLR_BLACK.  All have shuffled descriptions.
 */
GLOVES("leather gloves", "old gloves",
       0, 0,        0, 15, 1, 10,  8, 9, 0,  LEATHER, HI_LEATHER, O1_NONE),
GLOVES("gauntlets of fumbling", "padded gloves",
       0, 1, FUMBLING,  8, 1, 10, 100, 9, 0,  LEATHER, HI_LEATHER, O1_NONE),
GLOVES("gauntlets of power", "riding gloves",
       0, 1,        0,  8, 1, 30, 100, 9, 0,  IRON, CLR_BROWN, O1_NONE),
GLOVES("gauntlets of dexterity", "fencing gloves",
       0, 1,        0,  8, 1, 10, 100, 9, 0,  LEATHER, HI_LEATHER, O1_NONE),

/* boots */
BOOTS("low boots", "walking shoes",
      0, 0,          0, 14, 2, 10,  8, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("iron shoes", "hard shoes",
      0, 0,          0,  7, 2, 60, 16, 8, 0, IRON, HI_METAL, O1_NONE),
BOOTS("leather sandals", None,
      1, 0,          0, 2, 1, 6,  4, 10, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("cotton slippers", None,
      1, 0,          0, 2, 1, 3,  4, 10, 0, CLOTH, CLR_WHITE, O1_NONE),
BOOTS("high boots", "jackboots",
      0, 0,          0, 12, 2, 30, 12, 8, 0, LEATHER, HI_LEATHER, O1_NONE),
/* with shuffled appearances... */
BOOTS("speed boots", "combat boots",
      0, 1,       FAST, 12, 2, 30, 50, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("water walking boots", "jungle boots",
      0, 1,   WWALKING, 12, 2, 15, 50, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("jumping boots", "hiking boots",
      0, 1,    JUMPING, 12, 2, 30, 50, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("elven boots", "mud boots",
      0, 1,    STEALTH, 12, 2, 20,  8, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("kicking boots", "buckled boots",
      0, 1,          0, 12, 2, 60,  8, 9, 0, IRON, CLR_BROWN, O1_NONE),
        /* CLR_BROWN for same reason as gauntlets of power */
BOOTS("fumble boots", "riding boots",
      0, 1,   FUMBLING, 12, 2, 30, 30, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("levitation boots", "snow boots",
      0, 1, LEVITATION, 12, 2, 15, 30, 9, 0, LEATHER, HI_LEATHER, O1_NONE),
#undef HELM
#undef CLOAK
#undef SHIELD
#undef SHIRT
#undef ROBE
#undef BRACERS
#undef BELT
#undef PANTS
#undef GLOVES
#undef BOOTS
#undef ARMOR

/* rings ... */
#define RING(name,stone,power,power2,power3,cost,mgc,spec,mohs,metal,color, flags) \
    OBJECT(OBJ(name, stone),                                          \
           BITS(0, 0, spec, 0, mgc, spec, 0, 0, 0,                    \
                HARDGEM(mohs), 0, P_NONE, metal),                     \
           power, power2, power3, RING_CLASS, 0, 0, 1, cost,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, color, 0, 0, flags)
RING("adornment", "wooden",
     ADORNED, 0, 0, 100, 1, 1, 2, WOOD, HI_WOOD, O1_NONE),
RING("gain strength", "granite",
     0, 0, 0, 150, 1, 1, 7, MINERAL, HI_MINERAL, O1_NONE),
RING("gain constitution", "opal",
     0, 0, 0, 150, 1, 1, 7, MINERAL, HI_MINERAL, O1_NONE),
RING("increase accuracy", "clay",
     0, 0, 0, 150, 1, 1, 4, MINERAL, CLR_RED, O1_NONE),
RING("increase damage", "coral",
     0, 0, 0, 150, 1, 1, 4, MINERAL, CLR_ORANGE, O1_NONE),
RING("protection", "black onyx",
     PROTECTION, 0, 0, 100, 1, 1, 7, MINERAL, CLR_BLACK, O1_NONE),
        /* 'PROTECTION' intrinsic enhances MC from worn armor by +1,
           regardless of ring's enchantment; wearing a second ring of
           protection (or even one ring of protection combined with
           cloak of protection) doesn't give a second MC boost */
RING("regeneration", "moonstone",
     REGENERATION, 0, 0, 200, 1, 0,  6, MINERAL, HI_MINERAL, O1_NONE),
RING("mana regeneration", "crystal",
     ENERGY_REGENERATION, 0, 0, 300, 1, 0,  6, MINERAL, HI_MINERAL, O1_NONE),
RING("searching", "tiger eye",
     SEARCHING, 0, 0, 200, 1, 0,  6, GEMSTONE, CLR_BROWN, O1_NONE),
RING("stealth", "jade",
     STEALTH, 0, 0, 100, 1, 0,  6, GEMSTONE, CLR_GREEN, O1_NONE),
RING("sustain ability", "bronze",
     FIXED_ABIL, 0, 0, 100, 1, 0,  4, COPPER, HI_COPPER, O1_NONE),
RING("levitation", "agate",
     LEVITATION, 0, 0, 200, 1, 0,  7, GEMSTONE, CLR_RED, O1_NONE),
RING("hunger", "topaz",
     HUNGER, 0, 0, 100, 1, 0,  8, GEMSTONE, CLR_CYAN, O1_NONE),
RING("aggravate monster", "sapphire",
     AGGRAVATE_MONSTER, 0, 0,        150, 1, 0,  9, GEMSTONE, CLR_BLUE, O1_NONE),
RING("conflict", "ruby",
     CONFLICT, 0, 0, 300, 1, 0,  9, GEMSTONE, CLR_RED, O1_NONE),
RING("warning", "diamond",
     WARNING, 0, 0, 100, 1, 0, 10, GEMSTONE, CLR_WHITE, O1_NONE),
RING("poison resistance", "pearl",
     POISON_RES, 0, 0,               150, 1, 0,  4, BONE, CLR_WHITE, O1_NONE),
RING("fire resistance", "iron",
     FIRE_RES, 0, 0, 200, 1, 0,  5, IRON, HI_METAL, O1_FIRE_RESISTANT),
RING("cold resistance", "brass",
     COLD_RES, 0, 0, 150, 1, 0,  4, COPPER, HI_COPPER, O1_COLD_RESISTANT),
RING("shock resistance", "copper",
     SHOCK_RES, 0, 0, 150, 1, 0,  3, COPPER, HI_COPPER, O1_LIGHTNING_RESISTANT),
RING("life protection", "platinum",
     DEATH_RES, DRAIN_RES, 0,               200, 1, 0,  3, COPPER, HI_COPPER, O1_LIGHTNING_RESISTANT),
RING("free action", "twisted",
     FREE_ACTION, 0, 0, 200, 1, 0,  6, IRON, HI_METAL, O1_NONE),
RING("slow digestion", "steel",
     SLOW_DIGESTION, 0, 0, 200, 1, 0,  8, IRON, HI_METAL, O1_NONE),
RING("teleportation", "silver",
     TELEPORT, 0, 0, 200, 1, 0,  3, SILVER, HI_SILVER, O1_NONE),
RING("teleport control", "gold",
     TELEPORT_CONTROL, 0, 0, 300, 1, 0,  3, GOLD, HI_GOLD, O1_NONE),
RING("polymorph", "ivory",
     POLYMORPH, 0, 0, 300, 1, 0,  4, BONE, CLR_WHITE, O1_POLYMORPH_RESISTANT),
RING("polymorph control", "emerald",
     POLYMORPH_CONTROL, 0, 0, 300, 1, 0,  8, GEMSTONE, CLR_BRIGHT_GREEN, O1_POLYMORPH_RESISTANT),
RING("invisibility", "wire",
     INVIS, 0, 0, 150, 1, 0,  5, IRON, HI_METAL, O1_NONE),
RING("see invisible", "engagement",
     SEE_INVIS, 0, 0, 150, 1, 0,  5, IRON, HI_METAL, O1_NONE),
RING("protection from shape changers", "shiny",
     PROT_FROM_SHAPE_CHANGERS, 0, 0, 100, 1, 0,  5, IRON, CLR_BRIGHT_CYAN, O1_POLYMORPH_RESISTANT),
#undef RING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,power,prob, flags) \
    OBJECT(OBJ(name, desc),                                            \
           BITS(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, P_NONE, IRON),        \
           power, 0, 0,  AMULET_CLASS, prob, 0, 5, 150,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, HI_METAL, 0, 0, flags)
AMULET("amulet of ESP",                "circular", TELEPAT, 175, O1_NONE),
AMULET("amulet of life saving",       "spherical", LIFESAVED, 75, O1_NONE),
AMULET("amulet of strangulation",          "oval", STRANGLED, 135, O1_NONE),
AMULET("amulet of restful sleep",    "triangular", SLEEPY, 135, O1_NONE),
AMULET("amulet versus poison",        "pyramidal", POISON_RES, 165, O1_NONE),
AMULET("amulet of change",               "square", 0, 130, O1_NONE),
AMULET("amulet of unchanging",          "concave", UNCHANGING, 45, O1_POLYMORPH_RESISTANT),
AMULET("amulet of reflection",        "hexagonal", REFLECTING, 75, O1_NONE),
AMULET("amulet of magical breathing", "octagonal", MAGICAL_BREATHING, 65, O1_NONE),
/* fixed descriptions; description duplication is deliberate;
 * fake one must come before real one because selection for
 * description shuffling stops when a non-magic amulet is encountered
 */
OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
           "Amulet of Yendor"),
       BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, PLASTIC),
       0, 0, 0, AMULET_CLASS, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, HI_METAL, 0, 0, O1_NONE),
OBJECT(OBJ("Amulet of Yendor", /* note: description == name */
           "Amulet of Yendor"),
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, MITHRIL),
       0, 0, 0, AMULET_CLASS, 0, 0, 10, 30000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, HI_METAL, 0, 0, O1_INDESTRUCTIBLE),
#undef AMULET

/* tools ... */
/* tools with weapon characteristics come last */
#define TOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color,flags) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, mrg, chg, 0, mgc, chg, 0, 0, 0, 0, 0, P_NONE, mat), \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wt, color, 0, 0, flags)
#define SPELLTOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,dir,dirsubtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus,mat,color,flags) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, mrg, chg, 0, mgc, chg, 0, 0, 0, 0, dir, P_NONE, mat), \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, 0, 0, 0, 0, wt, color, dirsubtype, 0, flags)
#define CONTAINER(name,desc,kn,mgc,chg,prob,wt,cost,mat,color,flags) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, 0, chg, 1, mgc, chg, 0, 0, 0, 0, 0, P_NONE, mat),   \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wt, color, 0, 0, flags)
#define WEPTOOL(name,desc,kn,mgc,bi,prob,wt,cost,sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon,sub,mat,clr,flags)\
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, 0, 1, 0, mgc, 1, 0, 0, bi, 0, hitbon, sub, mat),    \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, 0, 0, wt, clr, 0, 0, flags)
/* containers */
CONTAINER("large box",       None, 1, 0, 0, 40, 350,   8, WOOD, HI_WOOD, O1_NONE),
CONTAINER("chest",           None, 1, 0, 0, 35, 600,  16, WOOD, HI_WOOD, O1_NONE),
CONTAINER("ice box",         None, 1, 0, 0,  5, 900,  42, PLASTIC, CLR_WHITE, O1_NONE),
CONTAINER("bookshelf",		 None, 1, 0, 0,  0, 1600, 36, WOOD, HI_WOOD, O1_NONE),
CONTAINER("sack",           "bag", 0, 0, 0, 35,  15,   2, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("oilskin sack",   "bag", 0, 0, 0,  5,  15, 100, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("bag of holding", "bag", 0, 1, 0, 20,  15, 100, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("bag of tricks",  "bag", 0, 1, 1, 20,  15, 100, CLOTH, HI_CLOTH, O1_NONE),
#undef CONTAINER

/* lock opening tools */
TOOL("skeleton key",       "key", 0, 0, 0, 0, 80,  2, 10, IRON, HI_METAL, O1_NONE),
TOOL("lock pick",           None, 1, 0, 0, 0, 60,  2, 20, IRON, HI_METAL, O1_NONE),
TOOL("credit card",         None, 1, 0, 0, 0, 15,  1, 10, PLASTIC, CLR_WHITE, O1_NONE),
/* light sources */
TOOL("tallow candle",   "candle", 0, 1, 0, 0, 20,  1, 10, WAX, CLR_WHITE, O1_NONE),
TOOL("wax candle",      "candle", 0, 1, 0, 0,  5,  1, 20, WAX, CLR_WHITE, O1_NONE),
TOOL("brass lantern",       None, 1, 0, 0, 0, 30, 30, 12, COPPER, CLR_YELLOW, O1_NONE),
TOOL("oil lamp",          "lamp", 0, 0, 0, 0, 45, 20, 10, COPPER, CLR_YELLOW, O1_NONE),
TOOL("magic lamp",        "lamp", 0, 0, 1, 0, 15, 20, 50, COPPER, CLR_YELLOW, O1_NONE),
/* other tools */
TOOL("expensive camera",    None, 1, 0, 0, 1, 15, 12,200, PLASTIC, CLR_BLACK, O1_NONE),
TOOL("mirror",   "looking glass", 0, 0, 0, 0, 35, 13, 10, GLASS, HI_SILVER, O1_NONE),
TOOL("holy symbol", "religious symbol", 0, 0, 1, 0, 10, 10,100, SILVER, HI_SILVER, O1_NONE),
TOOL("crystal ball", "glass orb", 0, 0, 1, 1, 15,150, 60, GLASS, HI_GLASS, O1_NONE),
TOOL("lenses",              None, 1, 0, 0, 0,  5,  3, 80, GLASS, HI_GLASS, O1_NONE),
TOOL("blindfold",           None, 1, 0, 0, 0, 50,  2, 20, CLOTH, CLR_BLACK, O1_NONE),
TOOL("towel",               None, 1, 0, 0, 0, 50,  2, 50, CLOTH, CLR_MAGENTA, O1_NONE),
TOOL("saddle",              None, 1, 0, 0, 0,  5,200,150, LEATHER, HI_LEATHER, O1_NONE),
TOOL("leash",               None, 1, 0, 0, 0, 65, 12, 20, LEATHER, HI_LEATHER, O1_NONE),
TOOL("stethoscope",         None, 1, 0, 0, 0, 25,  4, 75, IRON, HI_METAL, O1_NONE),
TOOL("tinning kit",         None, 1, 0, 0, 1, 15,100, 30, IRON, HI_METAL, O1_NONE),
TOOL("tin opener",          None, 1, 0, 0, 0, 35,  4, 30, IRON, HI_METAL, O1_NONE),
TOOL("can of grease",       None, 1, 0, 0, 1, 15, 15, 20, IRON, HI_METAL, O1_NONE),
TOOL("figurine",            None, 1, 0, 1, 0, 25, 50, 80, MINERAL, HI_MINERAL, O1_NONE),
        /* monster type specified by obj->corpsenm */
TOOL("magic marker",        None, 1, 0, 1, 1, 15,  2, 50, PLASTIC, CLR_RED, O1_NONE),
/* traps */
TOOL("land mine",           None, 1, 0, 0, 0, 0, 300,180, IRON, CLR_RED, O1_NONE),
TOOL("beartrap",            None, 1, 0, 0, 0, 0, 200, 60, IRON, HI_METAL, O1_NONE),
/* instruments;
   "If tin whistles are made out of tin, what do they make foghorns out of?" */
TOOL("tin whistle",    "whistle", 0, 0, 0, 0,100, 2, 10, METAL, HI_METAL, O1_NONE),
TOOL("magic whistle",  "whistle", 0, 0, 1, 0, 30, 2, 10, METAL, HI_METAL, O1_NONE),
TOOL("wooden flute",     "flute", 0, 0, 0, 0,  4, 5, 12, WOOD, HI_WOOD, O1_NONE),
TOOL("magic flute",      "flute", 0, 0, 1, 1,  2, 5, 36, WOOD, HI_WOOD, O1_NONE),
TOOL("tooled horn",       "horn", 0, 0, 0, 0,  5, 18, 15, BONE, CLR_WHITE, O1_NONE),
SPELLTOOL("frost horn",   "horn", 0, 0, 1, 1,  2, 18, 50, RAY, RAY_WND_COLD, 6, 6, 0, 6, 6, 0, BONE, CLR_WHITE, O1_NONE),
SPELLTOOL("fire horn",    "horn", 0, 0, 1, 1,  2, 18, 50, RAY, RAY_WND_FIRE, 6, 6, 0, 6, 6, 0, BONE, CLR_WHITE, O1_NONE),
TOOL("horn of plenty",    "horn", 0, 0, 1, 1,  2, 18, 50, BONE, CLR_WHITE, O1_NONE),
        /* horn, but not an instrument */
TOOL("wooden harp",       "harp", 0, 0, 0, 0,  4, 30, 50, WOOD, HI_WOOD, O1_NONE),
TOOL("magic harp",        "harp", 0, 0, 1, 1,  2, 30, 50, WOOD, HI_WOOD, O1_NONE),
TOOL("bell",                None, 1, 0, 0, 0,  2, 30, 50, COPPER, HI_COPPER, O1_NONE),
TOOL("bugle",               None, 1, 0, 0, 0,  4, 10, 15, COPPER, HI_COPPER, O1_NONE),
TOOL("leather drum",      "drum", 0, 0, 0, 0,  4, 25, 25, LEATHER, HI_LEATHER, O1_NONE),
TOOL("drum of earthquake","drum", 0, 0, 1, 1,  2, 25, 25, LEATHER, HI_LEATHER, O1_NONE),
/* tools useful as weapons */
WEPTOOL("pick-axe", None,
        1, 0, 0, 20, 75,  50,  1, 6, 0, 1, 3, 0, WHACK,  P_PICK_AXE, IRON, HI_METAL, O1_NONE),
WEPTOOL("grappling hook", "iron hook",
        0, 0, 0,  5,  30,  50,  1, 2, 0, 1, 6, 0, WHACK,  P_FLAIL,    IRON, HI_METAL, O1_NONE),
WEPTOOL("unicorn horn", None,
        1, 1, 1,  0,  20, 100, 1, 12, 0, 1, 12, 0, PIERCE, P_UNICORN_HORN,
                                                           BONE, CLR_WHITE, O1_NONE),
        /* 3.4.1: unicorn horn left classified as "magic" */
/* two unique tools;
 * not artifacts, despite the comment which used to be here
 */
OBJECT(OBJ("Candelabrum of Invocation", "candelabrum"),
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, GOLD),
       0, 0, 0, TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, HI_GOLD, 0, 0, O1_INDESTRUCTIBLE),
OBJECT(OBJ("Bell of Opening", "silver bell"),
       BITS(0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, P_NONE, SILVER),
       0, 0, 0, TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, HI_SILVER, 0, 0, O1_INDESTRUCTIBLE),
#undef TOOL
#undef WEPTOOL

/* Comestibles ... */
#define FOOD(name, prob, delay, wt, unk, tin, nutrition, color)         \
    OBJECT(OBJ(name, None),                                       \
           BITS(1, 1, unk, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, tin), 0, 0, 0,     \
           FOOD_CLASS, prob, delay, wt, nutrition / 20 + 5,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           nutrition, color, 0, 0, O1_NONE)
/* All types of food (except tins & corpses) must have a delay of at least 1.
 * Delay on corpses is computed and is weight dependant.
 * Domestic pets prefer tripe rations above all others.
 * Fortune cookies can be read, using them up without ingesting them.
 * Carrots improve your vision.
 * +0 tins contain monster meat.
 * +1 tins (of spinach) make you stronger (like Popeye).
 * Meatballs/sticks/rings are only created from objects via stone to flesh.
 */
/* meat */
FOOD("tripe ration",        140,  2, 10, 0, FLESH, 200, CLR_BROWN),
FOOD("corpse",                0,  1,  0, 0, FLESH,   0, CLR_BROWN),
FOOD("egg",                  85,  1,  1, 1, FLESH,  80, CLR_WHITE),
FOOD("meatball",              0,  1,  1, 0, FLESH,   5, CLR_BROWN),
FOOD("meat stick",            0,  1,  1, 0, FLESH,   5, CLR_BROWN),
FOOD("huge chunk of meat",    0, 20,400, 0, FLESH,2000, CLR_BROWN),
/* special case because it's not mergable */
OBJECT(OBJ("meat ring", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FLESH),
       0, 0, 0, FOOD_CLASS, 0, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, CLR_BROWN, 0, 0, 0),
/* pudding 'corpses' will turn into these and combine;
   must be in same order as the pudding monsters */
FOOD("glob of gray ooze",     0,  2, 20, 0, FLESH,  20, CLR_GRAY),
FOOD("glob of brown pudding", 0,  2, 20, 0, FLESH,  20, CLR_BROWN),
FOOD("glob of green slime",   0,  2, 20, 0, FLESH,  20, CLR_GREEN),
FOOD("glob of black pudding", 0,  2, 20, 0, FLESH,  20, CLR_BLACK),

FOOD("clump of bat guano",	  5,  2, 10, 0, FLESH,  20, CLR_BROWN),

/* fruits & veggies */
FOOD("kelp frond",            0,  1,  1, 0, VEGGY,  30, CLR_GREEN),
FOOD("eucalyptus leaf",       3,  1,  1, 0, VEGGY,  30, CLR_GREEN),
FOOD("apple",                10,  1,  2, 0, VEGGY,  50, CLR_RED),
FOOD("orange",               10,  1,  2, 0, VEGGY,  80, CLR_ORANGE),
FOOD("pear",                 10,  1,  2, 0, VEGGY,  50, CLR_BRIGHT_GREEN),
FOOD("melon",                10,  1,  5, 0, VEGGY, 100, CLR_BRIGHT_GREEN),
FOOD("banana",               10,  1,  2, 0, VEGGY,  80, CLR_YELLOW),
FOOD("carrot",               10,  1,  2, 0, VEGGY,  50, CLR_ORANGE),
FOOD("ginseng root",		  7, 1, 1, 0, VEGGY, 40, CLR_BROWN),
FOOD("mandrake root",		  7, 1, 1, 0, VEGGY, 40, CLR_MAGENTA),
FOOD("fungal spore",		  5,  1,  1, 0, VEGGY, 1, CLR_GRAY),
FOOD("sprig of wolfsbane",    7,  1,  1, 0, VEGGY,  40, CLR_GREEN),
FOOD("clove of garlic",       7,  1,  1, 0, VEGGY,  40, CLR_WHITE),
/* name of slime mold is changed based on player's OPTION=fruit:something
   and bones data might have differently named ones from prior games */
FOOD("slime mold",           75,  1,  5, 0, VEGGY, 250, HI_ORGANIC),

/* people food */
FOOD("lump of royal jelly",   0,  1,  2, 0, VEGGY, 200, CLR_YELLOW),
FOOD("cream pie",            25,  1, 10, 0, VEGGY, 100, CLR_WHITE),
FOOD("candy bar",            13,  1,  2, 0, VEGGY, 100, CLR_BROWN),
FOOD("fortune cookie",       41,  1,  1, 0, VEGGY,  40, CLR_YELLOW),
FOOD("pancake",              25,  2,  2, 0, VEGGY, 200, CLR_YELLOW),
FOOD("lembas wafer",         20,  2,  5, 0, VEGGY, 800, CLR_WHITE),
FOOD("cram ration",          20,  3, 15, 0, VEGGY, 600, HI_ORGANIC),
FOOD("food ration",         380,  5, 20, 0, VEGGY, 800, HI_ORGANIC),
FOOD("K-ration",              0,  1, 10, 0, VEGGY, 400, HI_ORGANIC),
FOOD("C-ration",              0,  1, 10, 0, VEGGY, 300, HI_ORGANIC),
/* tins have type specified by obj->spe (+1 for spinach, other implies
   flesh; negative specifies preparation method {homemade,boiled,&c})
   and by obj->corpsenm (type of monster flesh) */
FOOD("tin",                  75,  0, 10, 1, METAL,   0, HI_METAL),
#undef FOOD

/* potions ... */
#define POTION(name,desc,mgc,power,prob,cost,color,flags) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(0, 1, 0, 0, mgc, 0, 0, 0, 0, 0, 0, P_NONE, GLASS),      \
           power, 0, 0, POTION_CLASS, prob, 0, 12, cost,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, color, 0, 0, flags)
POTION("gain ability",           "ruby",  1, 0, 42, 300, CLR_RED, O1_NONE),
POTION("restore ability",        "pink",  1, 0, 40, 100, CLR_BRIGHT_MAGENTA, O1_NONE),
POTION("confusion",            "orange",  1, CONFUSION, 42, 100, CLR_ORANGE, O1_NONE),
POTION("blindness",            "yellow",  1, BLINDED, 40, 150, CLR_YELLOW, O1_NONE),
POTION("paralysis",           "emerald",  1, 0, 42, 300, CLR_BRIGHT_GREEN, O1_NONE),
POTION("speed",            "dark green",  1, FAST, 42, 200, CLR_GREEN, O1_NONE),
POTION("levitation",             "cyan",  1, LEVITATION, 42, 200, CLR_CYAN, O1_NONE),
POTION("hallucination",      "sky blue",  1, HALLUC, 40, 100, CLR_CYAN, O1_NONE),
POTION("invisibility", "brilliant blue",  1, INVIS, 40, 150, CLR_BRIGHT_BLUE, O1_NONE),
POTION("see invisible",       "magenta",  1, SEE_INVIS, 42, 50, CLR_MAGENTA, O1_NONE),
POTION("healing",          "purple-red",  1, 0, 57, 100, CLR_MAGENTA, O1_NONE),
POTION("extra healing",          "puce",  1, 0, 47, 100, CLR_RED, O1_NONE),
POTION("gain level",            "milky",  1, 0, 20, 300, CLR_WHITE, O1_NONE),
POTION("enlightenment",        "swirly",  1, 0, 20, 200, CLR_BROWN, O1_NONE),
POTION("monster detection",    "bubbly",  1, 0, 40, 150, CLR_WHITE, O1_NONE),
POTION("object detection",      "smoky",  1, 0, 42, 150, CLR_GRAY, O1_NONE),
POTION("gain energy",          "cloudy",  1, 0, 42, 150, CLR_WHITE, O1_NONE),
POTION("sleeping",       "effervescent",  1, 0, 42, 100, CLR_GRAY, O1_NONE),
POTION("full healing",          "black",  1, 0, 10, 200, CLR_BLACK, O1_NONE),
POTION("polymorph",            "golden",  1, 0, 10, 200, CLR_YELLOW, O1_POLYMORPH_RESISTANT),
POTION("booze",                 "brown",  0, 0, 42,  50, CLR_BROWN, O1_COLD_RESISTANT),
POTION("sickness",              "fizzy",  0, 0, 42,  50, CLR_CYAN, O1_NONE),
POTION("fruit juice",            "dark",  0, 0, 42,  50, CLR_BLACK, O1_NONE),
POTION("acid",                  "white",  0, 0, 10, 250, CLR_WHITE, O1_COLD_RESISTANT),
POTION("oil",                   "murky",  0, 0, 30, 250, CLR_BROWN, O1_COLD_RESISTANT),
/* fixed description
 */
POTION("water",                 "clear",  0, 0, 92, 100, CLR_CYAN, O1_NONE),
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,mgc,prob,cost,flags) \
    OBJECT(OBJ(name, text),                                           \
           BITS(0, 1, 0, 0, mgc, 0, 0, 0, 0, 0, 0, P_NONE, PAPER),    \
           0, 0, 0, SCROLL_CLASS, prob, 0, 4, cost,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 6, HI_PAPER, 0, 0, flags)
SCROLL("enchant armor",              "ZELGO MER",  1,  63,  80, O1_NONE),
SCROLL("destroy armor",         "JUYED AWK YACC",  1,  45, 100, O1_NONE),
SCROLL("confuse monster",                 "NR 9",  1,  53, 100, O1_NONE),
SCROLL("scare monster",   "XIXAXA XOXAXA XUXAXA",  1,  35, 100, O1_NONE),
SCROLL("remove curse",             "PRATYAVAYAH",  1,  65,  80, O1_NONE),
SCROLL("enchant weapon",         "DAIYEN FOOELS",  1,  80,  60, O1_NONE),
SCROLL("create monster",       "LEP GEX VEN ZEA",  1,  45, 200, O1_NONE),
SCROLL("taming",                   "PRIRUTSENIE",  1,  15, 200, O1_NONE),
SCROLL("genocide",                  "ELBIB YLOH",  1,  15, 300, O1_NONE),
SCROLL("light",                 "VERR YED HORRE",  1,  90,  50, O1_NONE),
SCROLL("teleportation",        "VENZAR BORGAVVE",  1,  55, 100, O1_NONE),
SCROLL("gold detection",                 "THARR",  1,  33, 100, O1_NONE),
SCROLL("food detection",               "YUM YUM",  1,  25, 100, O1_NONE),
SCROLL("identify",                  "KERNOD WEL",  1, 180,  20, O1_NONE),
SCROLL("magic mapping",              "ELAM EBOW",  1,  45, 100, O1_NONE),
SCROLL("amnesia",                   "DUAM XNAHT",  1,  35, 200, O1_NONE),
SCROLL("fire",                  "ANDOVA BEGARIN",  1,  30, 100, O1_FIRE_RESISTANT),
SCROLL("earth",                          "KIRJE",  1,  18, 200, O1_NONE),
SCROLL("punishment",            "VE FORBRYDERNE",  1,  15, 300, O1_NONE),
SCROLL("charging",                "HACKEM MUCHE",  1,  15, 300, O1_NONE),
SCROLL("stinking cloud",             "VELOX NEB",  1,  15, 300, O1_NONE),
    /* Extra descriptions, shuffled into use at start of new game.
     * Code in win/share/tilemap.c depends on SCR_STINKING_CLOUD preceding
     * these and on how many of them there are.  If a real scroll gets added
     * after stinking cloud or the number of extra descriptions changes,
     * tilemap.c must be modified to match.
     */
SCROLL(None,      "FOOBIE BLETCH",  1,   0, 100, O1_NONE),
SCROLL(None,              "TEMOV",  1,   0, 100, O1_NONE),
SCROLL(None,         "GARVEN DEH",  1,   0, 100, O1_NONE),
SCROLL(None,            "READ ME",  1,   0, 100, O1_NONE),
SCROLL(None,      "ETAOIN SHRDLU",  1,   0, 100, O1_NONE),
SCROLL(None,        "LOREM IPSUM",  1,   0, 100, O1_NONE),
SCROLL(None,              "FNORD",  1,   0, 100, O1_NONE), /* Illuminati */
SCROLL(None,            "KO BATE",  1,   0, 100, O1_NONE), /* Kurd Lasswitz */
SCROLL(None,      "ABRA KA DABRA",  1,   0, 100, O1_NONE), /* traditional incantation */
SCROLL(None,       "ASHPD SODALG",  1,   0, 100, O1_NONE), /* Portal */
SCROLL(None,            "ZLORFIK",  1,   0, 100, O1_NONE), /* Zak McKracken */
SCROLL(None,      "GNIK SISI VLE",  1,   0, 100, O1_NONE), /* Zak McKracken */
SCROLL(None,    "HAPAX LEGOMENON",  1,   0, 100, O1_NONE),
SCROLL(None,  "EIRIS SAZUN IDISI",  1,   0, 100, O1_NONE), /* Merseburg Incantations */
SCROLL(None,    "PHOL ENDE WODAN",  1,   0, 100, O1_NONE), /* Merseburg Incantations */
SCROLL(None,              "GHOTI",  1,   0, 100, O1_NONE), /* pronounced as 'fish',
                                                        George Bernard Shaw */
SCROLL(None, "MAPIRO MAHAMA DIROMAT", 1, 0, 100, O1_NONE), /* Wizardry */
SCROLL(None,  "VAS CORP BET MANI",  1,   0, 100, O1_NONE), /* Ultima */
SCROLL(None,            "XOR OTA",  1,   0, 100, O1_NONE), /* Aarne Haapakoski */
SCROLL(None, "STRC PRST SKRZ KRK",  1,   0, 100, O1_NONE), /* Czech and Slovak
                                                        tongue-twister */
    /* These must come last because they have special fixed descriptions.
     */
#ifdef MAIL
SCROLL("mail",          "stamped",  0,   0,   0),
#endif
SCROLL("blank paper", "unlabeled",  0,  28,  60, O1_NONE),
#undef SCROLL

/* spellbooks ... */
    /* Expanding beyond 52 spells would require changes in spellcasting
     * or imposition of a limit on number of spells hero can know because
     * they are currently assigned successive letters, a-zA-Z, when learned.
     * [The existing spell sorting capability could conceivably be extended
     * to enable moving spells from beyond Z to within it, bumping others
     * out in the process, allowing more than 52 spells be known but keeping
     * only 52 be castable at any given time.]
     */
#define SPELL(name,desc,sub,prob,learndelay,cooldown,level,manacost,attr,mgc,dir,dirsubtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,color,flags)  \
    OBJECT(OBJ(name, desc),                                             \
           BITS(0, 0, 0, 0, mgc, 0, 0, 0, 0, 0, dir, sub, PAPER),       \
           0, 0, 0, SPBOOK_CLASS, prob, learndelay, 50, level * 100,               \
           sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, cooldown, level, manacost, attr, 20, color,dirsubtype, 0, flags)
/* Spellbook description normally refers to book covers (primarily color).
   Parchment and vellum would never be used for such, but rather than
   eliminate those, finagle their definitions to refer to the pages
   rather than the cover.  They are made from animal skin (typically of
   a goat or sheep) and books using them for pages generally need heavy
   covers with straps or clamps to tightly close the book in order to
   keep the pages flat.  (However, a wooden cover might itself be covered
   by a sheet of parchment, making this become less of an exception.  Also,
   changing the internal composition from paper to leather makes eating a
   parchment or vellum spellbook break vegetarian conduct, as it should.) */
#define PAPER LEATHER /* override enum for use in SPELL() expansion */

SPELL("dig",             "parchment",
      P_TRANSMUTATION_SPELL,      20,  4, 4, 3, 10, A_INT, 1, RAY, RAY_DIGGING, 0, 0, 0, 0, 0, 0, HI_LEATHER, O1_NONE),
SPELL("magic missile",   "vellum",
      P_ARCANE_SPELL,      20,  2, 0, 0,  7, A_INT, 1, RAY, RAY_MAGIC_MISSILE, 3, 4, 0, 0, 0, 0, HI_LEATHER, O1_NONE),
#undef PAPER /* revert to normal material */
SPELL("fire bolt",        "light red",
      P_ARCANE_SPELL,      20,  4, 0, 3, 20, A_INT, 1, RAY, RAY_FIRE, 3, 6, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT),
SPELL("fireball",        "ragged",
      P_ARCANE_SPELL,      20,  6, 2, 5, 50, A_INT, 1, TARGETED, RAY_FIRE, 5, 6, 0, 0, 0, 0, HI_PAPER, O1_FIRE_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("fire storm",       "crimson",
      P_ARCANE_SPELL,      10,  8, 3, 7,100, A_INT, 1, TARGETED, RAY_FIRE, 12, 6, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("cone of cold",    "dog eared",
      P_ARCANE_SPELL,      10,  6, 0, 5, 50, A_INT, 1, RAY, RAY_COLD, 15, 6, 0, 0, 0, 0, HI_PAPER, O1_COLD_RESISTANT),
SPELL("ice storm",		  "mithril-plated",
      P_ARCANE_SPELL,      10,  7, 2, 6, 75, A_INT, 1, TARGETED, RAY_COLD, 4, 10, 8, 0, 0, 0, HI_SILVER, O1_COLD_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("sleep",           "mottled",
      P_ENCHANTMENT_SPELL, 10,  1, 0, 3,  5, A_CHA, 1, RAY, RAY_SLEEP, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("disintegrate",	 "stained",
      P_ARCANE_SPELL,       5, 12, 4, 10,300, A_INT, 1, RAY, RAY_DISINTEGRATION, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_DISINTEGRATION_RESISTANT),
SPELL("lightning bolt",  "shimmering",
      P_ARCANE_SPELL,      10,  3, 	0, 2, 15, A_INT, 1, RAY, RAY_LIGHTNING, 6, 6, 0, 0, 0, 0, HI_PAPER, O1_LIGHTNING_RESISTANT),
SPELL("thunderstorm",	 "strange",
      P_ARCANE_SPELL,      10,  10, 3, 8,150, A_INT, 1, TARGETED, RAY_LIGHTNING, 10, 12, 0, 0, 0, 0, HI_PAPER, O1_LIGHTNING_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("light",           "cloth",
      P_DIVINATION_SPELL,  10,  1, 0, 0,  2, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_CLOTH, O1_NONE),
SPELL("black blade of disaster", "ebony",
	P_CONJURATION_SPELL,		5,  10, 20, 9,200, A_INT, 1, NODIR, 0, 0, 0, 0, 3, 4, 8, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),
SPELL("summon ogre",	  "old",
	P_CONJURATION_SPELL,		   15,  3, 2, 3, 30, A_CHA, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("summon demon", "worn",
	P_CONJURATION_SPELL,			5,  5, 4, 7,100, A_CHA, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("detect monsters", "leathery",
      P_DIVINATION_SPELL,  18,  1, 1, 0, 5, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_LEATHER, O1_NONE),
SPELL("healing",         "white",
      P_HEALING_SPELL,     15,  2, 0, 1,10, A_WIS, 1, IMMEDIATE, 0, 4, 4, 0, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("knock",           "pink",
      P_TRANSMUTATION_SPELL,      25,  1, 0, -1, 1, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_MAGENTA, O1_NONE),
SPELL("force bolt",      "red",
      P_ARCANE_SPELL,      15,  2, 0, 1,10, A_INT, 1, IMMEDIATE, 0, 3, 6, 1, 0, 0, 0, CLR_RED, O1_NONE),
SPELL("magic arrow",      "hardcover",
      P_ARCANE_SPELL,      15,  1, 0, -1, 4, A_INT, 1, IMMEDIATE, 0, 1, 8, 1, 0, 0, 0, CLR_BROWN, O1_NONE),
SPELL("confuse monster", "orange",
      P_ENCHANTMENT_SPELL, 15,  2, 0, 0,  7, A_CHA, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_ORANGE, O1_NONE),
SPELL("cure blindness",  "yellow",
      P_HEALING_SPELL,     15,  2, 1, 0,  7, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_YELLOW, O1_NONE),
SPELL("drain life",      "velvet",
      P_ARCANE_SPELL,      10,  2, 1, 5, 50, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_MAGENTA, O1_NONE),
SPELL("slow monster",    "light green",
      P_ENCHANTMENT_SPELL, 20,  2, 0, 1, 10, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_GREEN, O1_NONE),
SPELL("wizard lock",     "dark green",
      P_TRANSMUTATION_SPELL,      30,  3, 0, -1, 3, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_GREEN, O1_NONE),
SPELL("create monster",  "turquoise",
      P_CLERIC_SPELL,      20,  3, 3, 3, 30, A_CHA, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_CYAN, O1_NONE),
SPELL("detect food",     "cyan",
      P_DIVINATION_SPELL,  20,  3, 0, -1, 4, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE),
SPELL("cause fear",      "light blue",
      P_ENCHANTMENT_SPELL, 20,  3, 1, 1, 10, A_CHA, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_BLUE, O1_NONE),
SPELL("clairvoyance",    "dark blue",
      P_DIVINATION_SPELL,  15,  3, 3, 2, 15, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLUE, O1_NONE),
SPELL("cure sickness",   "indigo",
      P_HEALING_SPELL,     17,  3, 3, 2, 15, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLUE, O1_NONE),
SPELL("charm monster",   "magenta",
      P_ENCHANTMENT_SPELL, 20,  3, 2, 6, 75, A_CHA, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_MAGENTA, O1_NONE),
SPELL("haste self",      "purple",
      P_MOVEMENT_SPELL,      18,  4, 4, 3, 30, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_MAGENTA, O1_NONE),
SPELL("detect unseen",   "violet",
      P_DIVINATION_SPELL,  20,  4, 2, 1, 10, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_MAGENTA, O1_NONE),
SPELL("levitation",      "tan",
      P_MOVEMENT_SPELL,      20,  4, 3, 2, 15, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE),
SPELL("extra healing",   "plaid",
      P_HEALING_SPELL,     17,  5, 4, 3, 20, A_WIS, 1, IMMEDIATE, 0, 8, 4, 0, 0, 0, 0, CLR_GREEN, O1_NONE),
SPELL("full healing",    "light gray",
      P_HEALING_SPELL,     10,  6, 6, 5, 50, A_WIS, 1, IMMEDIATE, 0, 0, 0, 255, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("restore ability", "light brown",
      P_HEALING_SPELL,     20,  5, 6, 5, 50, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE),
SPELL("invisibility",    "dark brown",
      P_MOVEMENT_SPELL,      20,  5, 3, 2, 15, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE),
SPELL("detect treasure", "gray",
      P_DIVINATION_SPELL,  20,  5, 1, 0, 7, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_GRAY, O1_NONE),
SPELL("remove curse",    "wrinkled",
      P_CLERIC_SPELL,      15,  5, 5, 4, 20, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("magic mapping",   "dusty",
      P_DIVINATION_SPELL,  18,  7, 5, 4, 60, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("identify",        "bronze-plated",
      P_DIVINATION_SPELL,  20,  5, 5, 4, 30, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_COPPER, O1_NONE),
SPELL("turn undead",     "copper-plated",
      P_CLERIC_SPELL,      16,  8, 6, 5, 50, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_COPPER, O1_NONE),
SPELL("polymorph",       "silver-plated",
      P_TRANSMUTATION_SPELL,      10,  6, 6, 5, 50, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_SILVER, O1_NONE),
SPELL("teleport away",   "gold-plated",
      P_MOVEMENT_SPELL,      15,  6, 5, 4, 30, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_GOLD, O1_NONE),
SPELL("create familiar", "glittering",
      P_CLERIC_SPELL,      10,  7, 5, 4, 30, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("cancellation",    "shining",
      P_TRANSMUTATION_SPELL,      15,  8, 6, 5, 50, A_INT, 1, IMMEDIATE, 0, 0, 0, 0, 1, 20, 30, CLR_WHITE, O1_SPELL_BYPASSES_MAGIC_RESISTANCE),
SPELL("protection",      "dull",
	P_ABJURATION_SPELL,      16, 3,200, 2, 15, A_WIS, 1, NODIR, 0, 0, 0, 0, 2, 10, 40, HI_PAPER, O1_NONE),
SPELL("jumping",         "thin",
      P_MOVEMENT_SPELL,      15,  3, 0,-1,  4, A_DEX, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("stone to flesh",  "thick",
      P_HEALING_SPELL,     11,  1, 4, 3, 20, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("touch of death", "heavy",
	P_NECROMANCY_SPELL,		    5,  8, 8, 7,100, A_INT, 1, TOUCH, RAY_DEATH, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("finger of death", "leather bound",
	P_NECROMANCY_SPELL,		    5, 10, 10, 8,150, A_INT, 1, RAY, RAY_DEATH, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("deathspell",	     "morbid",
	P_NECROMANCY_SPELL,		    5, 10, 12, 9,200, A_INT, 1, TARGETED, RAY_DEATH, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE | O1_SPELL_EXPLOSION_EFFECT),
SPELL("armageddon",      "pitch black",
	P_NECROMANCY_SPELL,       2, 14, 20,10,300, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLACK, O1_NONE),
SPELL("wish",			 "platinum-plated",
      P_CONJURATION_SPELL,       2, 15, 20,10,300, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_SILVER, O1_NONE),
SPELL("time stop",		  "ancient",
	P_TRANSMUTATION_SPELL,  2, 10, 12, 9,200, A_INT, 1, NODIR, 0, 0, 0, 0, 1, 4, 3, HI_PAPER, O1_NONE),
SPELL("mage armor",		  "ornamental",
	  P_ABJURATION_SPELL,	   15,  1,360, 1, 10, A_INT, 1, NODIR, 0, 0, 0, 0, 4, 20, 200, CLR_BLACK, O1_NONE),
SPELL("bless",			  "spiritual",
	P_CLERIC_SPELL,			   10, 1, 4, 3, 20, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLACK, O1_NONE),
SPELL("curse",			  "blasphemous",
	  P_CLERIC_SPELL,	       10,  1,4, 3, 20, A_WIS, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLACK, O1_NONE),
SPELL("enchant armor",		  "glowing",
	  P_ENCHANTMENT_SPELL,		10, 3, 4, 3,20, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("enchant weapon",		  "dark gray",
	  P_ENCHANTMENT_SPELL,		10, 3, 5, 4,30, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("protect armor",		  "glowing",
	  P_ENCHANTMENT_SPELL,		10, 2, 2, 1,10, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("protect weapon",		  "dark gray",
	  P_ENCHANTMENT_SPELL,		10, 2, 3, 2,15, A_INT, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("resurrection",		  "bright-colored",
	P_CLERIC_SPELL,				10, 5,10, 5,50, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("negate undeath",		  "inspiring",
	P_CLERIC_SPELL,				5, 4 , 8, 7,100, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("banish demon",		  "thought-provoking",
	P_CLERIC_SPELL,				5, 4 , 9, 8,150, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("anti-magic shell", "immaculate",
	  P_ABJURATION_SPELL,	    5,  1,120, 7,100, A_WIS, 1, NODIR, ANTIMAGIC, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("reflection", "polished silver",
	  P_ABJURATION_SPELL,	    5,  1,120, 6, 75, A_WIS, 1, NODIR, REFLECTING, 0, 0, 0, 10, 6, 25, HI_SILVER, O1_NONE),
SPELL("protection from fire", "flame-patterned",
	  P_ABJURATION_SPELL,	    5,  1,120, 2, 15, A_WIS, 1, NODIR, FIRE_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_FIRE_RESISTANT),
SPELL("protection from lightning", "cloudy",
	  P_ABJURATION_SPELL,	    5,  1,120, 2, 15, A_WIS, 1, NODIR, SHOCK_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_LIGHTNING_RESISTANT),
SPELL("protection from cold", "blue and white",
	  P_ABJURATION_SPELL,	    5,  1,120, 2, 15, A_WIS, 1, NODIR, COLD_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_COLD_RESISTANT),
SPELL("protection from acid", "incomplete",
	  P_ABJURATION_SPELL,	    5,  1,120, 1, 10, A_WIS, 1, NODIR, ACID_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from poison", "intimidating-looking",
	P_ABJURATION_SPELL,			5, 1, 120, 1, 10, A_WIS, 1, NODIR, POISON_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from life draining", "adorned",
	P_ABJURATION_SPELL,			5, 1, 120, 0, 7, A_WIS, 1, NODIR, DRAIN_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from death magic", "decorated",
	P_ABJURATION_SPELL,			5, 1, 120, 3, 20, A_WIS, 1, NODIR, DEATH_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from disintegration", "expensive-looking",
	P_ABJURATION_SPELL,			5, 1, 120, 4, 30, A_WIS, 1, NODIR, DISINT_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_DISINTEGRATION_RESISTANT),
SPELL("protection from sickness", "baroque",
	P_ABJURATION_SPELL,			5, 1, 120, 4, 30, A_WIS, 1, NODIR, SICK_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from petrification", "aged",
	P_ABJURATION_SPELL,			5, 1, 120, 5, 50, A_WIS, 1, NODIR, STONE_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("globe of invulnerability", "striped",
	P_ABJURATION_SPELL,			5, 1,  60, 8,150, A_WIS, 1, NODIR, INVULNERABLE, 0, 0, 0, 1, 6, 4, HI_PAPER, O1_INDESTRUCTIBLE),
SPELL("divine intervention", "square-patterned",
	P_ABJURATION_SPELL,			5, 1, 120, 9,200, A_WIS, 1, NODIR, LIFESAVED, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from lycanthropy", "strange-looking",
	P_ABJURATION_SPELL,			5, 1, 120, 0, 7, A_WIS, 1, NODIR, LYCANTHROPY_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("lower magic resistance", "dim",
	P_ABJURATION_SPELL,			5, 1, 5, 4, 30, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_SPELL_BYPASSES_MAGIC_RESISTANCE),
SPELL("negate magic resistance", "dark",
	P_ABJURATION_SPELL,			5, 1, 9, 8, 150, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_SPELL_BYPASSES_MAGIC_RESISTANCE),
SPELL("forbid summoning",		"oval-patterned",
	P_ABJURATION_SPELL,			5, 1, 4, 3, 20, A_WIS, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),



/* books with fixed descriptions
 */
SPELL("blank paper", "plain", P_NONE, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
/* tribute book for 3.6 */
OBJECT(OBJ("novel", "paperback"),
       BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, PAPER),
       0, 0, 0, SPBOOK_CLASS, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 20, CLR_BRIGHT_BLUE, 0, 0, O1_NONE),
/* a special, one of a kind, spellbook */
OBJECT(OBJ("Book of the Dead", "papyrus"),
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, PAPER),
       0, 0, 0, SPBOOK_CLASS, 0, 0, 20, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 20, HI_PAPER, 0, 0, O1_INDESTRUCTIBLE),
#undef SPELL

/* wands ... */
#define WAND(name,typ,prob,cost,mgc,dir,dirsubtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,metal,color,flags) \
    OBJECT(OBJ(name, typ),                                              \
           BITS(0, 0, 1, 0, mgc, 1, 0, 0, 0, 0, dir, P_NONE, metal),    \
           0, 0, 0, WAND_CLASS, prob, 0, 6, cost, sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, 0, 0, 0, 0, 30, color, dirsubtype, 0, flags)
WAND("light",           "glass", 95, 100, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, GLASS, HI_GLASS, O1_NONE),
WAND("secret door detection",
                        "balsa", 50, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("enlightenment", "crystal", 15, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, GLASS, HI_GLASS, O1_NONE),
WAND("create monster",  "maple", 45, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("wishing",          "pine",  5, 500, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_LIGHTNING_RESISTANT),
WAND("nothing",           "oak", 25, 100, 0, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("striking",        "ebony", 70, 150, 1, IMMEDIATE, 0, 2, 12, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("make invisible", "marble", 45, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, MINERAL, HI_MINERAL, O1_NONE),
WAND("slow monster",      "tin", 50, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("speed monster",   "brass", 50, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
WAND("undead turning", "copper", 50, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
WAND("polymorph",      "silver", 45, 200, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, SILVER, HI_SILVER, O1_POLYMORPH_RESISTANT),
WAND("cancellation", "platinum", 45, 200, 1, IMMEDIATE, 0, 0, 0, 0, 1, 20, 30, PLATINUM, CLR_WHITE, O1_SPELL_BYPASSES_MAGIC_RESISTANCE | O1_LIGHTNING_RESISTANT | O1_POLYMORPH_RESISTANT | O1_DISINTEGRATION_RESISTANT),
WAND("teleportation", "iridium", 45, 200, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, METAL, CLR_BRIGHT_CYAN, O1_NONE),
WAND("opening",          "zinc", 25, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("locking",      "aluminum", 25, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("probing",       "uranium", 30, 150, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("digging",          "iron", 50, 150, 1, RAY, RAY_WND_DIGGING, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND("magic missile",   "steel", 50, 150, 1, RAY, RAY_WND_MAGIC_MISSILE, 3, 4, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND("fire",        "hexagonal", 40, 175, 1, RAY, RAY_WND_FIRE, 8, 6, 0, 0, 0, 0, IRON, HI_METAL, O1_FIRE_RESISTANT),
WAND("cold",            "short", 40, 175, 1, RAY, RAY_WND_COLD, 12, 6, 0, 0, 0, 0, IRON, HI_METAL, O1_COLD_RESISTANT),
WAND("sleep",		    "runed", 50, 175, 1, RAY, RAY_WND_SLEEP, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND("disintegration",   "long",  5, 500, 1, RAY, RAY_WND_DISINTEGRATION, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_DISINTEGRATION_RESISTANT),
WAND("lightning",      "curved", 40, 175, 1, RAY, RAY_WND_LIGHTNING, 6, 6, 0, 0, 0, 0, IRON, HI_METAL, O1_LIGHTNING_RESISTANT),
WAND("death",	 "skull-headed",  5, 500, 1, RAY, RAY_WND_DEATH, 0, 0, 0, 0, 0, 0, BONE, CLR_WHITE, O1_NONE),
WAND("resurrection", "ornamental",5, 200, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, IRON, CLR_MAGENTA, O1_NONE),
/* extra descriptions, shuffled into use at start of new game */
WAND(None,             "forked",  0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND(None,             "spiked",  0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND(None,            "jeweled",  0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, IRON, HI_MINERAL, O1_NONE),
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal,worth) \
    OBJECT(OBJ(name, None),                                        \
           BITS(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, metal),    \
           0, 0, 0, COIN_CLASS, prob, 0, 1, worth,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HI_GOLD, 0, 0, O1_NONE)
COIN("gold piece", 1000, GOLD, 1),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(0, 1, 0, 0, 0, 0, 0, 0, 0,                              \
                HARDGEM(mohs), 0, -P_SLING, glass),                     \
           0, 0, 0, GEM_CLASS, prob, 0, 1, gval, 1, 3, 0, 1, 3, 0, 0, 0, 0, 0, nutr, color, 0, 0, O1_NONE)
#define ROCK(name,desc,kn,prob,wt,gval, sdam, ldam, mgc,nutr,mohs,glass,color) \
    OBJECT(OBJ(name, desc),                                             \
           BITS(kn, 1, 0, 0, mgc, 0, 0, 0, 0,                           \
                HARDGEM(mohs), 0, -P_SLING, glass),                     \
           0, 0, 0, GEM_CLASS, prob, 0, wt, gval, 1, sdam, 0, 1, ldam, 0, 0, 0, 0, 0, nutr, color, 0, 0, O1_NONE)
GEM("dilithium crystal", "white",  2, 1, 4500, 15,  5, GEMSTONE, CLR_WHITE),
GEM("diamond",           "white",  3, 1, 4000, 15,  10, GEMSTONE, CLR_WHITE),
GEM("black pearl",		 "black",  3, 1, 3750, 15,  5, GEMSTONE, CLR_BLACK),
GEM("ruby",                "red",  4, 1, 3500, 15,  9, GEMSTONE, CLR_RED),
GEM("jacinth",          "orange",  3, 1, 3250, 15,  9, GEMSTONE, CLR_ORANGE),
GEM("sapphire",           "blue",  4, 1, 3000, 15,  9, GEMSTONE, CLR_BLUE),
GEM("black opal",        "black",  3, 1, 2750, 15,  8, GEMSTONE, CLR_BLACK),
GEM("emerald",           "green",  5, 1, 2500, 15,  8, GEMSTONE, CLR_GREEN),
GEM("turquoise",         "green",  6, 1, 2000, 15,  6, GEMSTONE, CLR_GREEN),
GEM("citrine",          "yellow",  4, 1, 1500, 15,  6, GEMSTONE, CLR_YELLOW),
GEM("aquamarine",        "green",  6, 1, 1500, 15,  8, GEMSTONE, CLR_GREEN),
GEM("amber",   "yellowish brown",  8, 1, 1000, 15,  2, GEMSTONE, CLR_BROWN),
GEM("topaz",   "yellowish brown", 10, 1,  900, 15,  8, GEMSTONE, CLR_BROWN),
GEM("jet",               "black", 10, 1,  850, 15,  7, GEMSTONE, CLR_BLACK),
GEM("opal",              "white", 12, 1,  800, 15,  6, GEMSTONE, CLR_WHITE),
GEM("chrysoberyl",      "yellow", 10, 1,  700, 15,  5, GEMSTONE, CLR_YELLOW),
GEM("garnet",              "red", 12, 1,  700, 15,  7, GEMSTONE, CLR_RED),
GEM("amethyst",         "violet", 12, 1,  600, 15,  7, GEMSTONE, CLR_MAGENTA),
GEM("jasper",              "red", 12, 1,  500, 15,  7, GEMSTONE, CLR_RED),
GEM("fluorite",         "violet", 12, 1,  400, 15,  4, GEMSTONE, CLR_MAGENTA),
GEM("pearl",			 "white", 12, 1,  300, 15,  5, GEMSTONE, CLR_WHITE),
GEM("obsidian",          "black", 10, 1,  200, 15,  6, GEMSTONE, CLR_BLACK),
GEM("agate",            "orange", 12, 1,  200, 15,  6, GEMSTONE, CLR_ORANGE),
GEM("jade",              "green", 10, 1,  300, 15,  6, GEMSTONE, CLR_GREEN),
GEM("worthless piece of white glass", "white",
    70, 1, 0, 6, 5, GLASS, CLR_WHITE),
GEM("worthless piece of blue glass", "blue",
    70, 1, 0, 6, 5, GLASS, CLR_BLUE),
GEM("worthless piece of red glass", "red",
    77, 1, 0, 6, 5, GLASS, CLR_RED),
GEM("worthless piece of yellowish brown glass", "yellowish brown",
    77, 1, 0, 6, 5, GLASS, CLR_BROWN),
GEM("worthless piece of orange glass", "orange",
    77, 1, 0, 6, 5, GLASS, CLR_ORANGE),
GEM("worthless piece of yellow glass", "yellow",
    77, 1, 0, 6, 5, GLASS, CLR_YELLOW),
GEM("worthless piece of black glass", "black",
    77, 1, 0, 6, 5, GLASS, CLR_BLACK),
GEM("worthless piece of green glass", "green",
    76, 1, 0, 6, 5, GLASS, CLR_GREEN),
GEM("worthless piece of violet glass", "violet",
    76, 1, 0, 6, 5, GLASS, CLR_MAGENTA),

/* Placement note: there is a wishable subrange for
 * "gray stones" in the o_ranges[] array in objnam.c
 * that is currently everything between luckstones and flint
 * (inclusive).
 */
ROCK("luckstone", "gray",  0,  10,  10, 60, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY),
ROCK("loadstone", "gray",  0,  10, 500,  1, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY),
ROCK("touchstone", "gray", 0,   8,  10, 45, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY),
ROCK("flint", "gray",      0,  10,  10,  1, 6, 6, 0, 10, 7, MINERAL, CLR_GRAY),
ROCK("rock", None,         1, 100,  10,  0, 3, 3, 0, 10, 7, MINERAL, CLR_GRAY),
#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
OBJECT(OBJ("boulder", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, P_NONE, MINERAL), 0, 0, 0,
       ROCK_CLASS, 100, 0, 8000, 0, 1, 20, 0, 1, 20, 0, 0, 0, 0, 0, 2000, HI_MINERAL, 0, 0, O1_NONE),
OBJECT(OBJ("statue", None),
       BITS(1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, P_NONE, MINERAL), 0, 0, 0,
       ROCK_CLASS, 900, 0, 4000, 0, 1, 20, 0, 1, 20, 0, 0, 0, 0, 0, 2500, CLR_WHITE, 0, 0, O1_NONE),
OBJECT(OBJ("heavy iron ball", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0, 0, 0,
       BALL_CLASS, 1000, 0, 800, 10, 1, 25, 0, 1, 25, 0, 0, 0, 0, 0, 200, HI_METAL, 0, 0, O1_NONE),
        /* +d4 when "very heavy" */
OBJECT(OBJ("iron chain", None),
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0, 0, 0,
       CHAIN_CLASS, 1000, 0, 240, 0, 1, 4, 0, 1, 4, 0, 0, 0, 0, 0, 200, HI_METAL, 0, 0, O1_NONE),
        /* +1 both l & s */

/* Venom is normally a transitory missile (spit by various creatures)
 * but can be wished for in wizard mode so could occur in bones data.
 */
OBJECT(OBJ("blinding venom", "splash of venom"),
       BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0, 0, 0,
       VENOM_CLASS, 500, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HI_ORGANIC, 0, 0, O1_NONE),
OBJECT(OBJ("acid venom", "splash of venom"),
       BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0, 0, 0,
       VENOM_CLASS, 500, 0, 1, 0, 2, 6, 0, 2, 6, 0, 0, 0, 0, 0, 0, HI_ORGANIC, 0, 0, O1_NONE),
        /* +d6 small or large */

/* Reagents here, which do not belong to any class, includes also all other odd non-food objects with no other apparent purpose*/
OBJECT(OBJ("thread of spider silk", None), /* STARTMARKER 1&2 */
	BITS(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, SILK), 0, 0, 0,
	REAGENT_CLASS, 500, 0, 2, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, CLR_GRAY, 0, 0, O1_NONE),
OBJECT(OBJ("raven feather", None), /* ENDMARKER 1&2 */
	BITS(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, FLESH), 0, 0, 0,
	REAGENT_CLASS, 500, 0, 3, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, CLR_GRAY, 0, 0, O1_NONE),


/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
OBJECT(OBJ(None, None),
       BITS(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0), 0, 0, 0,
       ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, O1_NONE)
}; /* objects[] */

#ifndef OBJECTS_PASS_2_

/* perform recursive compilation for second structure */
#undef OBJ
#undef OBJECT
#define OBJECTS_PASS_2_
#include "objects.c"

/* clang-format on */
/* *INDENT-ON* */

void NDECL(objects_init);

/* dummy routine used to force linkage */
void
objects_init()
{
    return;
}

#endif /* !OBJECTS_PASS_2_ */

/*objects.c*/

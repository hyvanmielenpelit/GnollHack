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
#define OBJECT(obj,contentdesc,bits,prp1,prp2,prp3,sym,prob,dly,wt, \
               cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,oc1,oc2,oc3,oc4,oc5,oc6,nut,color, dirsubtype,materials,cooldown,level,flags)  { obj }
#define None (char *) 0 /* less visual distraction for 'no description' */

NEARDATA struct objdescr obj_descr[] =
#else
/* second pass -- object definitions */
#define BITS(nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,mtrl) \
  nmkn,mrg,uskn,0,mgc,chrg,uniq,nwsh,big,tuf,dir,mtrl,sub /*SCO cpp fodder*/
#define OBJECT(obj,contentdesc,bits,prp1,prp2,prp3,sym,prob,dly,wt,cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,oc1,oc2,oc3,oc4,oc5,oc6,nut,color,dirsubtype,materials,cooldown,level,flags) \
  { 0, 0, (char *) 0, contentdesc, bits, prp1, prp2, prp3, sym, dly, COLOR_FIELD(color) prob, wt, \
    cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, oc1, oc2, oc3, oc4, oc5, oc6, nut, dirsubtype,materials,cooldown,level,flags }
#ifndef lint
#define HARDGEM(n) (n >= 8)
#else
#define HARDGEM(n) (0)
#endif

NEARDATA struct objclass objects[] =
#endif
{
/* dummy object[0] -- description [2nd arg] *must* be NULL */
OBJECT(OBJ("strange object", None), None,
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0),
       0, 0, 0, ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),

/* weapons ... */
#define WEAPON(name,desc,kn,mg,bi,prob,wt,                \
               cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,hitbon,manabon,hpbon,power,power2,power3,typ,sub,metal,color,flags) \
    OBJECT(OBJ(name,desc), None,                                          \
           BITS(kn, mg, 1, 0, 0, 1, 0, 0, bi, 0, typ, sub, metal),  \
           power, power2, power3, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, manabon, hpbon, 0, 0, wt, color, 0, 0, 0, 0, flags)
#define PROJECTILE(name,desc,kn,prob,wt,                  \
                   cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,hitbon,metal,sub,color,flags) \
    OBJECT(OBJ(name,desc), None,                                         \
           BITS(kn, 1, 1, 0, 0, 1, 0, 0, 0, 0, PIERCE, sub, metal), \
           0, 0, 0, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, 0, 0, 0, 0, wt, color, 0, 0, 0, 0, flags)
#define BOW(name,desc,kn,bi,prob,wt,cost,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,hitbon,manabon,hpbon,power,power2,power3,metal,sub,color,flags) \
    OBJECT(OBJ(name,desc), None,                                         \
           BITS(kn, 0, 1, 0, 0, 1, 0, 0, bi, 0, 0, sub, metal),      \
           power, power2, power3, WEAPON_CLASS, prob, 0, wt,                            \
           cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, manabon, hpbon, 0, 0, wt, color, 0, 0, 0, 0, flags)

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
           1, 12, 1, 5, 1, 6, 0, 1, 6, 0, 0,        SILVER, -P_BOW, HI_SILVER, O1_TREATED_AS_MATERIAL_COMPONENT),
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
       1, 1, 0, 50,   1,   1, 2, 0, 1, 3, 0,  2, 0, 0, 0, 0, 0, 0, P,   -P_DART, IRON, HI_METAL, O1_NONE),
WEAPON("shuriken", "throwing star",
       0, 1, 0, 25,   1,   1, 5, 0, 1,  8, 0,  6, 2, 0, 0, 0, 0, 0, P,   -P_SHURIKEN, IRON, HI_METAL, O1_NONE),
WEAPON("boomerang", None,
       1, 1, 0, 15,   5,  20,  1, 9, 0, 1, 9, 0, 0, 0, 0, 0, 0, 0, 0,   -P_BOOMERANG, WOOD, HI_WOOD, O1_NONE),

/* spears [note: javelin used to have a separate skill from spears,
   because the latter are primarily stabbing weapons rather than
   throwing ones; but for playability, they've been merged together
   under spear skill and spears can now be thrown like javelins] */
WEAPON("spear", None,
       1, 1, 0, 50,  30,   3,  1, 6, 0, 1,  8, 0, 0, 0, 0, 0, 0, 0, P,   P_SPEAR, IRON, HI_METAL, O1_NONE),
WEAPON("elven spear", "runed spear",
       0, 1, 0, 10,  30,   3,  1, 7, 0, 1,  8, 0, 0, 0, 0, 0, 0, 0, P,   P_SPEAR, WOOD, HI_WOOD, O1_NONE),
WEAPON("orcish spear", "crude spear",
       0, 1, 0, 13,  30,   3,  1, 5, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, P,   P_SPEAR, IRON, CLR_BLACK, O1_NONE),
WEAPON("dwarvish spear", "stout spear",
       0, 1, 0, 12,  35,   3,  1, 8, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, P,   P_SPEAR, IRON, HI_METAL, O1_NONE),
WEAPON("silver spear", None,
       1, 1, 0,  2,  36,  40,  1, 6, 0,  1, 8, 0, 0, 0, 0, 0, 0, 0, P,   P_SPEAR, SILVER, HI_SILVER, O1_NONE),
WEAPON("javelin", "throwing spear",
       0, 1, 0, 10,  20,   3,  1, 6, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, P,   P_SPEAR, IRON, HI_METAL, O1_NONE),

/* spearish; doesn't stack, not intended to be thrown */
WEAPON("trident", None,
       1, 0, 0,  8,  25,   5, 1,  6, 1, 3,  4, 0, 0, 0, 0, 0, 0, 0, P,   P_TRIDENT, IRON, HI_METAL, O1_NONE),
        /* +1 small, +2d4 large */

/* blades; all stack */
WEAPON("dagger", None,
       1, 1, 0, 24,  10,   4,  1, 4, 0, 1,  3, 0, 2, 0, 0, 0, 0, 0, P,   P_DAGGER, IRON, HI_METAL, O1_NONE),
WEAPON("elven dagger", "runed dagger",
       0, 1, 0, 10,  10,   4,  1, 5, 0, 1, 3, 0, 2, 0, 0, 0, 0, 0, P,   P_DAGGER, WOOD, HI_WOOD, O1_NONE),
WEAPON("orcish dagger", "crude dagger",
       0, 1, 0, 12,  10,   4,  1, 3, 0, 1,  3, 0, 2, 0, 0, 0, 0, 0, P,   P_DAGGER, IRON, CLR_BLACK, O1_NONE),
WEAPON("bone dagger", None,
       1, 1, 0, 6,  10,   4,  1, 3, 0, 1,  3, 0, 2, 0, 0, 0, 0, 0, P,   P_DAGGER, BONE, CLR_WHITE, O1_TREATED_AS_MATERIAL_COMPONENT),
WEAPON("silver dagger", None,
       1, 1, 0,  3,  12,  40,  1, 4, 0, 1,  3, 0, 2, 0, 0, 0, 0, 0, P,   P_DAGGER, SILVER, HI_SILVER, O1_NONE),
WEAPON("athame", None,
       1, 1, 0,  0,  10,   4,  1, 4, 0, 1,  3, 0, 2, 0, 0, 0, 0, 0, S,   P_DAGGER, IRON, HI_METAL, O1_NONE),
WEAPON("scalpel", None,
       1, 1, 0,  0,   5,   6,  1, 3, 0, 1, 3, 0, 2, 0, 0, 0, 0, 0, S,   P_KNIFE, METAL, HI_METAL, O1_NONE),
WEAPON("knife", None,
       1, 1, 0, 20,   5,   4,  1, 3, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, P|S, P_KNIFE, IRON, HI_METAL, O1_NONE),
WEAPON("stiletto", None,
       1, 1, 0,  5,   5,   4, 1, 3, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, P|S, P_KNIFE, IRON, HI_METAL, O1_NONE),
/* 3.6: worm teeth and crysknives now stack;
   when a stack of teeth is enchanted at once, they fuse into one crysknife;
   when a stack of crysknives drops, the whole stack reverts to teeth */
WEAPON("worm tooth", None,
       1, 1, 0,  0,  20,   2,  1, 2, 0, 1,  2, 0, 0, 0, 0, 0, 0, 0, 0,   P_KNIFE, 0, CLR_WHITE, O1_NONE),
WEAPON("crysknife", None,
       1, 1, 0,  0,  20, 100, 1, 10, 0, 1, 10, 0, 3, 0, 0, 0, 0, 0, P,   P_KNIFE, MINERAL, CLR_WHITE, O1_NONE),

/* axes */
WEAPON("axe", None,
       1, 0, 0, 40,  50,   8,  1, 6, 0, 1,  4, 0, 0, 0, 0, 0, 0, 0, S,   P_AXE, IRON, HI_METAL, O1_NONE),
WEAPON("battle-axe", "double-headed axe",       /* "double-bitted"? */
       0, 0, 1, 10, 100,  40,  2, 6, 0, 3, 4, 1, 0, 0, 0, 0, 0, 0, S,   P_AXE, IRON, HI_METAL, O1_NONE),

/* swords */
WEAPON("short sword", None,
       1, 0, 0,  8,  45,  10,  1, 6, 0, 1,  8, 0, 0, 0, 0, 0, 0, 0, P,   P_SHORT_SWORD, IRON, HI_METAL, O1_NONE),
WEAPON("elven short sword", "runed short sword",
       0, 0, 0,  2,  40,  10,  1, 8, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, P,   P_SHORT_SWORD, WOOD, HI_WOOD, O1_NONE),
WEAPON("orcish short sword", "crude short sword",
       0, 0, 0,  3,  50,  10,  1, 5, 0, 1,  8, 0, 0, 0, 0, 0, 0, 0, P,   P_SHORT_SWORD, IRON, CLR_BLACK, O1_NONE),
WEAPON("dwarvish short sword", "broad short sword",
       0, 0, 0,  2,  50,  10,  1, 7, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, P,   P_SHORT_SWORD, IRON, HI_METAL, O1_NONE),
WEAPON("scimitar", "curved sword",
       0, 0, 0, 15,  60,  15,  1, 8, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, S,   P_SCIMITAR, IRON, HI_METAL, O1_NONE),
WEAPON("silver saber", None,
       1, 0, 0,  6,  55,  75,  1, 8, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, S,   P_SABER, SILVER, HI_SILVER, O1_NONE),
WEAPON("broadsword", None,
       1, 0, 0,  8,  60,  10,  2, 4, 0, 1, 6, 1, 0, 0, 0, 0, 0, 0, S,   P_BROAD_SWORD, IRON, HI_METAL, O1_NONE),
        /* +d4 small, +1 large */
WEAPON("elven broadsword", "runed broadsword",
       0, 0, 0,  4,  55,  10,  2, 4, 0, 1, 6, 1, 0, 0, 0, 0, 0, 0, S,   P_BROAD_SWORD, WOOD, HI_WOOD, O1_NONE),
        /* +d4 small, +1 large */
WEAPON("long sword", None,
       1, 0, 0, 38,  60,  15,  1, 8, 0, 1, 12, 0, 0, 0, 0, 0, 0, 0, S,   P_LONG_SWORD, IRON, HI_METAL, O1_NONE),
WEAPON("silver long sword", None,
	   1, 0, 0, 0,   55, 500, 1, 8, 0, 1, 12, 0, 0, 0, 0, 0, 0, 0, S, P_LONG_SWORD, SILVER, HI_SILVER, O1_NONE),
WEAPON("glass sword", None,
	   1, 0, 0, 2,   20, 500, 1, 8, 0, 1, 12, 0, 0, 0, 0, 0, 0, 0, S, P_LONG_SWORD, GLASS, HI_GLASS, O1_NONE),
WEAPON("two-handed sword", None,
       1, 0, 1, 20, 120,  50, 1, 12, 0, 3, 6, 0, 0, 0, 0, 0, 0, 0, S,   P_TWO_HANDED_SWORD,
                                                        IRON, HI_METAL, O1_NONE),
        /* +2d6 large */
WEAPON("katana", "samurai sword",
       0, 0, 0,  4,  80,  80, 1, 10, 0, 1, 12, 0, 1, 0, 0, 0, 0, 0, S,   P_LONG_SWORD, IRON, HI_METAL, O1_NONE),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword",
       0, 0, 1,  0,  100, 500, 1, 16, 0, 3, 6, 1, 2, 0, 0, 0, 0, 0, S,   P_TWO_HANDED_SWORD,
                                                            METAL, HI_METAL, O1_NONE),
        /* +2d6 large */
WEAPON("runesword", "runed broadsword",
       0, 0, 0,  0,  55, 300,  2, 4, 0, 1, 6, 1, 0, 0, 0, 0, 0, 0, S,   P_BROAD_SWORD, IRON, CLR_BLACK, O1_NONE),
        /* +d4 small, +1 large; Stormbringer: +5d2 +d8 from level drain */

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
       0, 0, 1,  5,  80,  10,  1, 6, 0, 1, 6, 1, 0, 0, 0, 0, 0, 0, P,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1 large */
WEAPON("ranseur", "hilted polearm",
       0, 0, 1,  5,  50,   6,  2, 4, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, P,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +d4 both */
WEAPON("spetum", "forked polearm",
       0, 0, 1,  5,  50,  1, 5, 1, 2, 6, 0, 6, 0, 0, 0, 0, 0, 0, P,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1 small, +d6 large */
WEAPON("glaive", "single-edged polearm",
       0, 0, 1,  8,  75,   6, 1, 6, 0, 1, 10, 0, 0, 0, 0, 0, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
WEAPON("lance", None,
       1, 0, 0,  4, 180,  10, 1, 6, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, P,   P_LANCE, IRON, HI_METAL, O1_NONE),
        /* +2d10 when jousting with lance as primary weapon */
/* axe-type */
WEAPON("halberd", "angled poleaxe",
       0, 0, 1,  8, 150,  10, 1, 10, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, P|S, P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d6 large */
WEAPON("bardiche", "long poleaxe",
       0, 0, 1,  4, 120,   7,  2, 4, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small, +2d4 large */
WEAPON("voulge", "pole cleaver",
       0, 0, 1,  4, 125,   5, 2, 4, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +d4 both */
WEAPON("dwarvish mattock", "broad pick",
       0, 0, 1, 13, 120,  50, 1, 12, 0, 3, 6, 1, -1, 0, 0, 0, 0, 0, B,  P_PICK_AXE, IRON, HI_METAL, O1_NONE),
/* curved/hooked */
WEAPON("fauchard", "pole sickle",
       0, 0, 1,  6,  60,   5,  1, 6, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, P|S, P_POLEARMS, IRON, HI_METAL, O1_NONE),
WEAPON("guisarme", "pruning hook",
       0, 0, 1,  6,  80,   5,  2, 4, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, S,   P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small */
WEAPON("bill-guisarme", "hooked polearm",
       0, 0, 1,  4, 120,   7,  2, 4, 0, 1, 10, 0, 0, 0, 0, 0, 0, 0, P|S, P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small */
WEAPON("infernal ancus", "black ornamental hooked polearm",
       0, 0, 0,  0, 100, 100,  1, 6, 1, 1, 8, 1, 0, 0, 0, 0, 0, 0, P|S, P_POLEARMS, IRON, CLR_BLACK, O1_NONE),
/* other */
WEAPON("lucern hammer", "pronged polearm",
       0, 0, 1,  5, 150,   7,  2, 4, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, B|P, P_POLEARMS, IRON, HI_METAL, O1_NONE),
        /* +1d4 small */
WEAPON("bec de corbin", "beaked polearm",
       0, 0, 1,  4, 100,   1, 8, 0, 1,  8, 0,  6, 0, 0, 0, 0, 0, 0, B|P, P_POLEARMS, IRON, HI_METAL, O1_NONE),

/* bludgeons */
WEAPON("mace", None,
       1, 0, 0, 38,  60,   5,  1, 6, 1, 1, 6, 0, 0, 0, 0, 0, 0, 0, B,   P_MACE, IRON, HI_METAL, O1_NONE),
        /* +1 small */
WEAPON("silver mace", None,
	   1, 0, 0, 2, 55, 250, 1, 6, 1, 1, 6, 0, 0, 0, 0, 0, 0, 0, B, P_MACE, SILVER, HI_SILVER, O1_NONE),
		/* +1 small */
WEAPON("morning star", None,
	   1, 0, 0, 12, 60,  10,  2, 4, 0, 1,  6, 1, 0, 0, 0, 0, 0, 0, B,   P_MORNING_STAR, IRON, HI_METAL, O1_NONE),
        /* +d4 small, +1 large */
WEAPON("war hammer", None,
       1, 0, 0, 11,  50,   5,  1, 4, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, B,   P_HAMMER, IRON, HI_METAL, O1_NONE),
        /* +1 small */
WEAPON("club", None,
       1, 0, 0, 12,  40,   3,  1, 6, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, B,   P_CLUB, WOOD, HI_WOOD, O1_NONE),
WEAPON("infernal jagged-toothed club", "black jagged-toothed club",
       0, 0, 0,  0,  60, 100,  1, 6, 1, 1, 6, 1, 0, 0, 0, 0, 0, 0, B,   P_CLUB, WOOD, CLR_BLACK, O1_NONE),
WEAPON("rubber hose", None,
       1, 0, 0,  0,  20,   3,  1, 4, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, B,   P_WHIP, PLASTIC, CLR_BROWN, O1_NONE),
WEAPON("quarterstaff", "staff",
       0, 0, 1, 11,  30,   5,  1, 6, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD, O1_NONE),
WEAPON("staff of the magi", "staff",
       0, 1, 1, 4,  30,  500,  3, 6, 0, 3, 6, 0, 0, 50, 0, 0, 0, 0, B,   P_QUARTERSTAFF, WOOD, HI_WOOD, O1_MANA_PERCENTAGE_BONUS | O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT),
/* two-piece */
WEAPON("aklys", "thonged club",
       0, 0, 0,  8,  15,   4,  1, 6, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, B,   P_CLUB, IRON, HI_METAL, O1_NONE),
WEAPON("flail", None,
       1, 0, 0, 40,  35,   4,  1, 6, 1, 2, 4, 0, 0, 0, 0, 0, 0, 0, B,   P_FLAIL, IRON, HI_METAL, O1_NONE),
	/* +1 small, +1d4 large */
WEAPON("double-headed flail", None,
	   1, 0, 0,  0,  55, 250, 1, 10, 1, 3, 4, 0, -1, 0, 0, 0, 0, 0, B, P_FLAIL, IRON, HI_METAL, O1_NONE),
	/* one-handed, flail damage x c. 1.5, -1 hit bonus (difficult to wield)*/
WEAPON("triple-headed flail", None,
	   1, 0, 0, 0, 70, 1000, 2, 6, 2, 4, 4, 0, -2, 0, 0, 0, 0, 0, B, P_FLAIL, IRON, HI_METAL, O1_NONE),
	/* one-handed, flail damage x 2, -2 hit bonus (difficult to wield)*/


/* misc */
WEAPON("bullwhip", None,
       1, 0, 0,  2,  20,   4,  1, 2, 0,  1, 1, 0,  0, 0, 0, 0, 0, 0, 0,   P_WHIP, LEATHER, CLR_BROWN, O1_NONE),
WEAPON("black blade of disintegration", "sword-shaped planar rift",
   	   0, 1, 1,  0,  0,    0,  1, 16, 0,  3, 6, 1, 0, 0, 0, 0, 0, 0, S, P_TWO_HANDED_SWORD, PLANARRIFT, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),

/* bows */
BOW("bow", None,							1, 1, 0,  30, 30, 1, 3, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("short bow", None,						1, 1, 12, 30, 30, 1, 3, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("elven long bow", "runed long bow",		0, 1, 12, 40, 200, 1, 5, 0, 1, 7, 0, 1, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("orcish short bow", "crude short bow",  0, 1, 12, 35, 10, 1, 2, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, CLR_BLACK, O1_NONE),
BOW("long bow", None,						1, 1, 12, 45, 75, 1, 4, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("composite long bow", None,				1, 1, 6,  50, 250, 1, 5, 1, 1, 7, 1, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("composite short bow", None,			1, 1, 6,  35, 100, 1, 4, 1, 1, 5, 1, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("yumi", "oriental long bow",			0, 1, 0,  55, 150, 1, 4, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, WOOD, P_BOW, HI_WOOD, O1_NONE),
BOW("sling", None,							1, 0, 40,  3, 20, 1, 2, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, LEATHER, P_SLING, HI_LEATHER, O1_NONE),
BOW("crossbow", None,						1, 1, 30, 100, 75, 1, 5, 0, 1, 5, 0, 0, 0, 0, 0, 0, 0, WOOD, P_CROSSBOW, HI_WOOD, O1_NONE),
BOW("hand crossbow", None,					1, 0, 5,  40, 50, 1, 2, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, WOOD, P_CROSSBOW, HI_WOOD, O1_NONE),
BOW("heavy crossbow", None,					1, 1, 10, 200, 150, 1, 8, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, WOOD, P_CROSSBOW, HI_WOOD, O1_NONE),

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
#define ARMOR(name,desc,kn,mgc,blk,power,power2,power3,prob,delay,wt,  \
              cost,ac,can,manabon,hpbon,bonusattrs,abon,sub,metal,c,flags)                   \
    OBJECT(OBJ(name, desc), None,                                        \
           BITS(kn, 0, 1, 0, mgc, 1, 0, 0, blk, 0, 0, sub, metal),  \
           power, power2, power3, ARMOR_CLASS, prob, delay, wt,                     \
           cost, 0, 0, 0, 0, 0, 0, 10 - ac, can, manabon, hpbon, bonusattrs, abon, wt, c, 0, 0, 0, 0, flags)
#define HELM(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_HELM, metal, c, flags)
#define CLOAK(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_CLOAK, metal, c, flags)
#define SHIELD(name,desc,kn,mgc,blk,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c,flags) \
    ARMOR(name, desc, kn, mgc, blk, power, power2, power3, prob, delay, wt, \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_SHIELD, metal, c, flags)
#define GLOVES(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_GLOVES, metal, c, flags)
#define BOOTS(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c,flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_BOOTS, metal, c,flags)
#define SHIRT(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_SHIRT, metal, c, flags)
#define ROBE(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_ROBE, metal, c, flags)
#define BRACERS(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_BRACERS, metal, c, flags)
#define BELT(name,desc,kn,mgc,power,power2,power3,prob,delay,wt,cost,ac,can,manabon,hpbon,bonusattrs,abon,metal,c, flags)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, prob, delay, wt,  \
          cost, ac, can, manabon, hpbon, bonusattrs, abon, ARM_BELT, metal, c, flags)

/* helmets */
HELM("elven leather helm", "leather hat",
     0, 0,           0, 0, 0,  6, 1,  3,  8,  9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
HELM("cotton hood", None,
     1, 0,           0, 0, 0,  3, 1,  3,  8,  10, 0, 0, 0, 0, 0, CLOTH, CLR_BLACK, O1_NONE),
HELM("gnollish hood", "crude leather hood",
     0, 0,           0, 0, 0,  3, 1,  3,  8,  9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
HELM("orcish helm", "iron skull cap",
     0, 0,           0, 0, 0,  6, 1, 30, 10,  9, 0, 0, 0, 0, 0, IRON, CLR_BLACK, O1_NONE),
HELM("dwarvish iron helm", "hard hat",
     0, 0,           0, 0, 0,  6, 1, 40, 20,  8, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
HELM("fedora", None,
     1, 0,           0, 0, 0,  0, 0,  3,  1, 10, 0, 0, 0, 0, 0, CLOTH, CLR_BROWN, O1_NONE),
HELM("cornuthaum", "conical hat",
     0, 1, CLAIRVOYANT, 0, 0, 3, 1,  4, 80, 10, 1, 0, 0, 0, 0, CLOTH, CLR_BLUE, O1_NONE),
        /* name coined by devteam; confers clairvoyance for wizards,
           blocks clairvoyance if worn by role other than wizard */
HELM("dunce cap", "conical hat",
     0, 1,           0, 0, 0,  3, 1,  4,  1, 10, 0, 0, 0, BONUS_TO_INT | BONUS_TO_WIS | FIXED_IS_MAXIMUM | SETS_FIXED_ATTRIBUTE, 6, CLOTH, CLR_BLUE, O1_BECOMES_CURSED_WHEN_WORN),
HELM("dented pot", None,
     1, 0,           0, 0, 0,  2, 0, 30,  8,  9, 0, 0, 0, 0, 0, IRON, CLR_BLACK, O1_NONE),
/* with shuffled appearances... */
HELM("helmet", "plumed helmet",
     0, 0,           0, 0, 0, 10, 1, 50, 10,  9, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
HELM("helm of brilliance", "etched helmet",
     0, 1,           0, 0, 0,  6, 1, 50, 50,  9, 0, 0, 0, BONUS_TO_INT | BONUS_TO_WIS, 0, IRON, CLR_GREEN, O1_NO_SPELL_CASTING_PENALTY),
HELM("helm of opposite alignment", "crested helmet",
     0, 1,           0, 0, 0,  6, 1, 50, 50,  9, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_BECOMES_CURSED_WHEN_WORN),
HELM("helm of telepathy", "visored helmet",
     0, 1,     TELEPAT, 0, 0, 2, 1, 50, 50,  9, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_HALF_SPELL_CASTING_PENALTY),

/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in obj.h, objnam.c, mon.c, read.c that assumes (2).
 *      (1) The dragon scale mails and the dragon scales are together.
 *      (2) That the order of the dragon scale mail and dragon scales
 *          is the same as order of dragons defined in monst.c.
 */
#define DRGN_ARMR(name,mgc,power,power2,power3,cost,ac,manabon,hpbon,bonusattrs,abon,color,flags)  \
    ARMOR(name, None, 1, mgc, 1, power, power2, power3, 0, 5, 550,  \
          cost, ac, 0, manabon, hpbon, bonusattrs, abon, ARM_SUIT, DRAGON_HIDE, color, flags)
/* 3.4.1: dragon scale mail reclassified as "magic" since magic is
   needed to create them */
DRGN_ARMR("gray dragon scale mail",    1, ANTIMAGIC,  0, 0, 6000, 1, 0, 0, 0, 0, CLR_GRAY, O1_NONE),
DRGN_ARMR("silver dragon scale mail",  1, REFLECTING, 0, 0, 6000, 1, 0, 0, 0, 0, DRAGON_SILVER, O1_NONE),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scale mail", 1, DISPLACED, 1200, 1, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scale mail",     1, FIRE_RES,   0, 0, 4000, 1, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT),
DRGN_ARMR("white dragon scale mail",   1, COLD_RES,   0, 0, 3000, 1, 0, 0, 0, 0, CLR_WHITE, O1_COLD_RESISTANT),
DRGN_ARMR("orange dragon scale mail",  1, SLEEP_RES,  0, 0, 4000, 1, 0, 0, 0, 0, CLR_ORANGE, O1_NONE),
DRGN_ARMR("black dragon scale mail",   1, DISINT_RES, 0, 0, 5000, 1, 0, 0, 0, 0, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),
DRGN_ARMR("blue dragon scale mail",    1, SHOCK_RES,  0, 0, 4000, 1, 0, 0, 0, 0, CLR_BLUE, O1_LIGHTNING_RESISTANT),
DRGN_ARMR("green dragon scale mail",   1, POISON_RES, 0, 0, 3000, 1, 0, 0, 0, 0, CLR_GREEN, O1_NONE),
DRGN_ARMR("yellow dragon scale mail",  1, ACID_RES,   0, 0, 3000, 1, 0, 0, 0, 0, CLR_YELLOW, O1_NONE),
/* For now, only dragons leave these. */
/* 3.4.1: dragon scales left classified as "non-magic"; they confer
   magical properties but are produced "naturally" */
DRGN_ARMR("gray dragon scales",        0, ANTIMAGIC,  0, 0, 1500, 7, 0, 0, 0, 0, CLR_GRAY, O1_NONE),
DRGN_ARMR("silver dragon scales",      0, REFLECTING, 0, 0, 1500, 7, 0, 0, 0, 0, DRAGON_SILVER, O1_NONE),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scales",  0, DISPLACED,   700, 7, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scales",         0, FIRE_RES,   0, 0, 1000, 7, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT),
DRGN_ARMR("white dragon scales",       0, COLD_RES,   0, 0,  750, 7, 0, 0, 0, 0, CLR_WHITE, O1_COLD_RESISTANT),
DRGN_ARMR("orange dragon scales",      0, SLEEP_RES,  0, 0, 1000, 7, 0, 0, 0, 0, CLR_ORANGE, O1_NONE),
DRGN_ARMR("black dragon scales",       0, DISINT_RES, 0, 0, 1250, 7, 0, 0, 0, 0, CLR_BLACK, O1_DISINTEGRATION_RESISTANT),
DRGN_ARMR("blue dragon scales",        0, SHOCK_RES,  0, 0, 1000, 7, 0, 0, 0, 0, CLR_BLUE, O1_LIGHTNING_RESISTANT),
DRGN_ARMR("green dragon scales",       0, POISON_RES, 0, 0, 750, 7, 0, 0, 0, 0, CLR_GREEN, O1_NONE),
DRGN_ARMR("yellow dragon scales",      0, ACID_RES,   0, 0,  750, 7, 0, 0, 0, 0, CLR_YELLOW, O1_NONE),
#undef DRGN_ARMR
/* other suits */
ARMOR("full plate mail", None,
	  1, 0, 1,			 0, 0, 0, 10, 7, 960,2000,  1, 2, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("field plate mail", None,
	  1, 0, 1,			 0, 0, 0, 20, 6, 840,1000,  2, 2, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("plate mail", None,
      1, 0, 1,			 0, 0, 0, 40, 5, 720, 600,  3, 2, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("crystal plate mail", None,
      1, 0, 1,  REFLECTING, 0, 0, 5, 5, 640,3000,  3, 2, 0, 0, 0, 0, ARM_SUIT, GLASS, CLR_WHITE, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT | O1_HALF_SPELL_CASTING_PENALTY),
ARMOR("bronze plate mail", None,
      1, 0, 1,			 0, 0, 0, 20, 5, 800, 150,  4, 1, 0, 0, 0, 0, ARM_SUIT, COPPER, HI_COPPER, O1_NONE),
ARMOR("splint mail", None,
      1, 0, 1,			 0, 0, 0, 40, 5, 640, 125,  4, 1, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("banded mail", None,
      1, 0, 1,			 0, 0, 0, 40, 5, 560, 200,  4, 1, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("dwarvish mithril-coat", None,
      1, 0, 0,			 0, 0, 0, 10, 1, 250, 750,  4, 2, 0, 0, 0, 0, ARM_SUIT, MITHRIL, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT | O1_HALF_SPELL_CASTING_PENALTY),
ARMOR("elven mithril-coat", None,
      1, 0, 0,			 0, 0, 0, 15, 1, 150, 250,  5, 2, 0, 0, 0, 0, ARM_SUIT, MITHRIL, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT | O1_HALF_SPELL_CASTING_PENALTY),
ARMOR("chain mail", None,
      1, 0, 0,			 0, 0, 0, 50, 5, 480,  75,  5, 1, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("orcish chain mail", "crude chain mail",
      0, 0, 0,			 0, 0, 0, 20, 5, 450,  40,  6, 1, 0, 0, 0, 0, ARM_SUIT, IRON, CLR_BLACK, O1_NONE),
ARMOR("scale mail", None,
      1, 0, 0,			 0, 0, 0, 40, 5, 500,  45,  6, 1, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("studded leather armor", None,
      1, 0, 0,			 0, 0, 0, 50, 3, 200,  15,  7, 1, 0, 0, 0, 0, ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("gnollish studded leather armor", "crude studded leather armor",
      0, 0, 0,			 0, 0, 0, 15, 3, 175,  15,  8, 1, 0, 0, 0, 0, ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("ring mail", None,
      1, 0, 0,			 0, 0, 0, 50, 5, 500,  30,  7, 1, 0, 0, 0, 0, ARM_SUIT, IRON, HI_METAL, O1_NONE),
ARMOR("orcish ring mail", "crude ring mail",
      0, 0, 0,			 0, 0, 0, 20, 5, 450,  15,  8, 1, 0, 0, 0, 0, ARM_SUIT, IRON, CLR_BLACK, O1_NONE),
ARMOR("leather armor", None,
      1, 0, 0,			 0, 0, 0, 50, 3, 150,   5,  8, 1, 0, 0, 0, 0, ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("gnollish leather armor", "crude leather armor",
      0, 0, 0,			 0, 0, 0, 12, 3, 125,   3,  9, 1, 0, 0, 0, 0, ARM_SUIT, LEATHER, HI_LEATHER, O1_NONE),
ARMOR("leather jacket", None,
      1, 0, 0,			 0, 0, 0, 10, 0,  30,  10,  9, 0, 0, 0, 0, 0, ARM_SUIT, LEATHER, CLR_BLACK, O1_NONE),
ARMOR("force field armor", "armor-shaped force field",
      0, 1, 0,			 0, 0, 0, 0,  0,   0,   4,  4, 0, 0, 0, 0, 0, ARM_SUIT, FORCEFIELD, CLR_WHITE, O1_NO_SPELL_CASTING_PENALTY | O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT | O1_COLD_RESISTANT | O1_CORROSION_RESISTANT | O1_RUST_RESISTANT),

/* shirts */
ARMOR("Hawaiian shirt", None, //STARTMARKER 1
    1, 0, 0, 0,			 0, 0, 8, 0, 5,  5, 10, 0, 0, 0, 0, 0, ARM_SHIRT, CLOTH, CLR_MAGENTA, O1_NONE),
/* with shuffled appearances... */
ARMOR("shirt of uncontrollable laughter", "funny T-shirt", //STARTMARKER 2
    0, 1, 0,  LAUGHING,  0, 0, 6, 0, 5, 50, 10, 0, 0, 0, 0, 0, ARM_SHIRT, CLOTH, CLR_BLUE, O1_NONE),
ARMOR("shirt of comeliness", "black T-shirt",
	0, 1, 0, 0,			 0, 0, 6, 0, 5, 50, 10, 0, 0, 0, BONUS_TO_CHA, 0, ARM_SHIRT, CLOTH, CLR_BLACK, O1_NONE),
ARMOR("shirt of sound mindedness", "green T-shirt", 
	0, 1, 0, HALLUC_RES, 0, 0, 6, 0, 5, 50, 10, 0, 0, 0, 0, 0, ARM_SHIRT, CLOTH, CLR_GREEN, O1_NONE),
ARMOR("T-shirt", "old T-shirt",//ENDMARKER 1 & 2
    0, 0, 0, 0,			 0, 0, 4, 0, 5,  2, 10, 0, 0, 0, 0, 0, ARM_SHIRT, CLOTH, CLR_WHITE, O1_NONE),


/* cloaks */
CLOAK("elven cloak", "faded pall",
      0, 1,    STEALTH, 0, 0, 8, 0, 10, 60,  9, 1, 0, 0, 0, 0, CLOTH, CLR_BLACK, O1_NONE),
CLOAK("orcish cloak", "coarse mantelet",
      0, 0,				 0, 0, 0, 8,  0, 10, 40, 10, 1, 0, 0, 0, 0, CLOTH, CLR_BLACK, O1_NONE),
CLOAK("dwarvish cloak", "hooded cloak",
      0, 0,			     0, 0, 0, 8,  0, 10, 50, 10, 1, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
CLOAK("oilskin cloak", "slippery cloak",
      0, 0,				 0, 0, 0, 8,  0, 10, 50,  9, 2, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
CLOAK("alchemy smock", "apron",
      0, 1, POISON_RES, 0, 0, 9, 0, 10, 50,  9, 1, 0, 0, 0, 0, CLOTH, CLR_WHITE, O1_NONE),
CLOAK("leather cloak", None,
      1, 0,				 0, 0, 0, 8,  0, 15, 40,  9, 1, 0, 0, 0, 0, LEATHER, CLR_BROWN, O1_NONE),
/* with shuffled appearances... */
CLOAK("cloak of protection", "tattered cape",
      0, 1, PROTECTION, 0, 0, 9, 0, 10, 250,  7, 3, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
        /* cloak of protection is now the only item conferring MC 3 */
CLOAK("cloak of invisibility", "opera cloak",
      0, 1,      INVIS, 0, 0, 9, 0, 10, 250,  9, 1, 0, 0, 0, 0, CLOTH, CLR_BRIGHT_MAGENTA, O1_NONE),
CLOAK("cloak of magic resistance", "ornamental cope",
      0, 1,  ANTIMAGIC, 0, 0, 6, 0, 10, 250,  9, 1, 0, 0, 0, 0, CLOTH, CLR_GRAY, O1_NONE),
        /*  'cope' is not a spelling mistake... leave it be */
CLOAK("cloak of integrity", "hard woven cloak",
      0, 1,  DISINT_RES, 0, 0, 6, 0, 10, 250,  8, 1, 0, 0, 0, 0, CLOTH, CLR_WHITE, O1_DISINTEGRATION_RESISTANT | O1_FIRE_RESISTANT),
CLOAK("cloak of displacement", "piece of cloth",
      0, 1,  DISPLACED, 0, 0, 7, 0, 10, 250,  9, 1, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),

/* robes and gowns */
/* with shuffled appearances... */
ROBE("robe", "old robe", //STARTMARKER 1 & 2
	0, 0, 0,		  0, 0, 10, 1, 20,  25,  10, 2, 0, 0, 0, 0, CLOTH, CLR_RED, O1_NONE),
ROBE("robe of protection", "golden ornamental robe",
	0, 1, PROTECTION, 0, 0, 10, 1, 25, 400,  7, 3, 0, 0, 0, 0, CLOTH, HI_GOLD, O1_NONE),
ROBE("robe of magic resistance", "silvery ornamental robe",
	0, 1, ANTIMAGIC,  0, 0, 9, 1, 25, 400,  9, 2, 0, 0, 0, 0, CLOTH, HI_SILVER, O1_NONE),
ROBE("gown of the archbishops", "shining purple robe",
	0, 1, CURSE_RES, DEATH_RES, DRAIN_RES, 5, 2, 30, 400,  9, 3,15, 15, 0, 0, CLOTH, HI_ZAP, O1_NOT_CURSEABLE | O1_HP_PERCENTAGE_BONUS | O1_MANA_PERCENTAGE_BONUS),
ROBE("robe of the archmagi", "shining blue robe",
	0, 1, 0,		   0, 0, 5, 2, 30, 400,  9, 3, 0, 0, BONUS_TO_INT | SETS_FIXED_ATTRIBUTE, 20, CLOTH, HI_ZAP, O1_NONE),
ROBE("robe of starry wisdom", "shining black robe", //ENDMARKER 2
	0, 1, 0,		   0, 0, 5, 2, 30, 400,  9, 3, 0, 0, BONUS_TO_WIS | SETS_FIXED_ATTRIBUTE, 20, CLOTH, CLR_BLACK, O1_NONE),
/* without shuffled appearances... */
ROBE("bathrobe", "soft cotton robe",
	0, 0, 0,		   0, 0, 6, 1, 25,   5, 10, 0, 0, 0, 0, 0, CLOTH, CLR_WHITE, O1_NONE),
ROBE("clerical gown", None,
	1, 0, 0,		   0, 0, 6, 1, 15,  25, 10, 2, 0, 0, 0, 0, CLOTH, CLR_BLACK, O1_NONE),
ROBE("gnollish haircloth robe", "crude robe",
	0, 0, 0,		   0, 0, 6, 1, 30,  10,  9, 2, 0, 0, 0, 0, CLOTH, CLR_BROWN, O1_NONE),
ROBE("mummy wrapping", None, //ENDMARKER 1
	1, 0, 0,		   0, 0, 0, 0,  3,   2, 10, 1, 0, 0, 0, 0, CLOTH, CLR_GRAY, O1_TREATED_AS_MATERIAL_COMPONENT),
	/* worn mummy wrapping blocks invisibility */


/* bracers */
/* with shuffled appearances... */
BRACERS("leather bracers", "old bracers", //STARTMARKER 1 & 2
	0, 0, 0,		  0, 0, 8, 1, 10,   5, 9, 0, 0, 0, 0, 0, CLOTH, HI_LEATHER, O1_NONE),
BRACERS("bracers of defense", "runed bracers",
	0, 1, PROTECTION, 0, 0, 8, 1, 10, 300, 7, 2, 0, 0, 0, 0, CLOTH, HI_LEATHER, O1_NONE),
BRACERS("bracers of archery", "deerskin bracers",
	0, 1, 0,		  0, 0, 7, 1, 10, 300, 9, 0, 0, 0, 0, 0, CLOTH, HI_LEATHER, O1_NONE),
BRACERS("bracers of magic resistance", "ornamental bracers",//ENDMARKER 1 & 2
	0, 1, ANTIMAGIC,  0, 0, 8, 1, 10, 300, 9, 2, 0, 0, 0, 0, CLOTH, HI_LEATHER, O1_NONE),


/* belts */
/* with shuffled appearances... */
BELT("leather belt", "old belt", //STARTMARKER 1 & 2
	0, 0, 0,		  0, 0, 10, 0, 5,   5, 10, 0, 0, 0, 0, 0, CLOTH, HI_LEATHER, O1_NO_SPELL_CASTING_PENALTY),
BELT("belt of dwarvenkind", "broad belt", 
	0, 1, POISON_RES, 0, 0,  8, 0, 5, 250, 10, 1, 0, 0, BONUS_TO_CON, 0, CLOTH, HI_LEATHER, O1_NONE),
BELT("belt of fortitude", "brown belt", 
	0, 1, 0,		  0, 0,  8, 0, 5, 250, 10, 1, 0, 50, 0, 0, CLOTH, HI_LEATHER, O1_HP_PERCENTAGE_BONUS),
BELT("belt of giant strength", "ornamental belt", //ENDMARKER 1 & 2
	0, 1, 0,		  0, 0,  7, 0, 5, 250, 10, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(20), CLOTH, HI_LEATHER, O1_NONE),


/* shields */
SHIELD("small shield", None,
       1, 0, 0,          0, 0, 0, 6, 0,  80,  3, 9, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
SHIELD("elven shield", "blue and green shield",
       0, 0, 0,          0, 0, 0, 2, 0,  140,  7, 8, 0, 0, 0, 0, 0, WOOD, CLR_GREEN, O1_NONE),
SHIELD("Uruk-hai shield", "white-handed shield",
       0, 0, 0,          0, 0, 0, 2, 0,  120,  7, 9, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
SHIELD("orcish shield", "red-eyed shield",
       0, 0, 0,          0, 0, 0, 2, 0,  100,  7, 9, 0, 0, 0, 0, 0, IRON, CLR_RED, O1_NONE),
SHIELD("large shield", None,
       1, 0, 1,          0, 0, 0, 5, 0, 160, 10, 8, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
SHIELD("dwarvish roundshield", "large round shield",
       0, 0, 0,          0, 0, 0, 4, 0, 150, 10, 8, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
SHIELD("shield of reflection", "polished silver shield",
       0, 1, 0, REFLECTING, 0, 0, 3, 0, 70, 750, 8, 0, 0, 0, 0, 0, SILVER, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT),

/* gloves */
/* These have their color but not material shuffled, so the IRON must
 * stay CLR_BROWN (== HI_LEATHER) even though it's normally either
 * HI_METAL or CLR_BLACK.  All have shuffled descriptions.
 */
GLOVES("leather gloves", "old gloves",
       0, 0,        0, 0, 0, 8, 1, 10,  8, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
GLOVES("gauntlets of fumbling", "padded gloves",
       0, 1, FUMBLING, 0, 0, 8, 1, 10, 100, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
GLOVES("gloves of spellcasting", "deerskin gloves",
       0, 1,		0, 0, 0, 8, 1, 10, 100, 9, 0, 0, 25, 0, 0, LEATHER, HI_LEATHER, O1_MANA_PERCENTAGE_BONUS),
GLOVES("gauntlets of power", "riding gloves",
       0, 1,        0, 0, 0, 8, 1, 30, 100, 9, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE | IGNORE_SPE, STR18(100) , IRON, CLR_BROWN, O1_NONE),
GLOVES("gauntlets of dexterity", "fencing gloves",
       0, 1,        0, 0, 0, 8, 1, 10, 100, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),

/* boots */
BOOTS("low boots", "walking shoes",
      0, 0,          0, 0, 0, 14, 2, 10,  8, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("iron shoes", "hard shoes",
      0, 0,          0, 0, 0, 7, 2, 60, 16, 8, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
BOOTS("leather sandals", None,
      1, 0,          0, 0, 0, 2, 1, 6,  4, 10, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("cotton slippers", None,
      1, 0,          0, 0, 0, 2, 1, 3,  4, 10, 0, 0, 0, 0, 0, CLOTH, CLR_WHITE, O1_NONE),
BOOTS("high boots", "jackboots",
      0, 0,          0, 0, 0, 12, 2, 30, 12, 8, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
/* with shuffled appearances... */
BOOTS("speed boots", "combat boots",
      0, 1,       FAST, 0, 0, 12, 2, 30, 50, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("water walking boots", "jungle boots",
      0, 1,   WWALKING, 0, 0, 12, 2, 15, 50, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("jumping boots", "hiking boots",
      0, 1,    JUMPING, 0, 0, 12, 2, 30, 50, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("elven boots", "mud boots",
      0, 1,    STEALTH, 0, 0, 12, 2, 20,  8, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("kicking boots", "buckled boots",
      0, 1,          0, 0, 0, 12, 2, 60,  8, 9, 0, 0, 0, 0, 0, IRON, CLR_BROWN, O1_NONE),
        /* CLR_BROWN for same reason as gauntlets of power */
BOOTS("fumble boots", "riding boots",
      0, 1,   FUMBLING, 0, 0, 12, 2, 30, 30, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
BOOTS("levitation boots", "snow boots",
      0, 1, LEVITATION, 0, 0, 12, 2, 15, 30, 9, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
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
#define RING(name,stone,power,power2,power3,cost,mgc,spec,mohs,manabon,hpbon,bonusattrs,abon,metal,color,flags) \
    OBJECT(OBJ(name, stone), None,                                         \
           BITS(0, 0, spec, 0, mgc, spec, 0, 0, 0,                    \
                HARDGEM(mohs), 0, P_NONE, metal),                     \
           power, power2, power3, RING_CLASS, 0, 0, 1, cost,  0, 0, 0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattrs,abon, 15, color, 0, 0, 0, 0, flags)
RING("adornment", "wooden",
     ADORNED, 0, 0, 100, 1, 1, 2, 0, 0, BONUS_TO_CHA, 0, WOOD, HI_WOOD, O1_NONE),
RING("gain strength", "granite",
     0, 0, 0, 150, 1, 1, 7, 0, 0, BONUS_TO_STR, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("gain dexterity", "thin",
     0, 0, 0, 150, 1, 1, 7, 0, 0, BONUS_TO_DEX, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("gain constitution", "opal",
     0, 0, 0, 150, 1, 1, 7, 0, 0, BONUS_TO_CON, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("gain intelligence", "adamantine",
     0, 0, 0, 150, 1, 1, 7, 0, 0, BONUS_TO_INT, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("gain wisdom", "polished",
     0, 0, 0, 150, 1, 1, 7, 0, 0, BONUS_TO_WIS, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("power", "runed",
     0, 0, 0, 300, 1, 1, 7, 0, 0, BONUS_TO_ALLSTATS, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("increase accuracy", "clay",
     0, 0, 0, 150, 1, 1, 4, 0, 0, BONUS_TO_HIT, 0, MINERAL, CLR_RED, O1_NONE),
RING("increase damage", "coral",
     0, 0, 0, 150, 1, 1, 4, 0, 0, BONUS_TO_DAMAGE, 0, MINERAL, CLR_ORANGE, O1_NONE),
RING("protection", "black onyx",
     PROTECTION, 0, 0, 100, 1, 1, 7, 0, 0, BONUS_TO_AC, 0, MINERAL, CLR_BLACK, O1_NONE),
        /* 'PROTECTION' intrinsic enhances MC from worn armor by +1,
           regardless of ring's enchantment; wearing a second ring of
           protection (or even one ring of protection combined with
           cloak of protection) doesn't give a second MC boost */
RING("regeneration", "moonstone",
     REGENERATION, 0, 0, 200, 1, 0,  6, 0, 0, 0, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("replenishment", "crystal",
     ENERGY_REGENERATION, 0, 0, 300, 1, 0,  6, 0, 0, 0, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("the serpent god", "serpent-headed",
     0, 0, 0, 300, 1, 0,  6,150, 0, 0, 0, MINERAL, HI_MINERAL, O1_NONE),
RING("fortitude", "immaculate",
     0, 0, 0, 200, 1, 0,  6,  0, 25, 0, 0, MINERAL, HI_MINERAL, O1_HP_PERCENTAGE_BONUS),
RING("searching", "tiger eye",
     SEARCHING, 0, 0, 200, 1, 0,  6, 0, 0, 0, 0, GEMSTONE, CLR_BROWN, O1_NONE),
RING("stealth", "jade",
     STEALTH, 0, 0, 100, 1, 0,  6, 0, 0, 0, 0, GEMSTONE, CLR_GREEN, O1_NONE),
RING("sustain ability", "bronze",
     FIXED_ABIL, 0, 0, 100, 1, 0,  4, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
RING("levitation", "agate",
     LEVITATION, 0, 0, 200, 1, 0,  7, 0, 0, 0, 0, GEMSTONE, CLR_RED, O1_NONE),
RING("hunger", "topaz",
     HUNGER, 0, 0, 100, 1, 0,  8, 0, 0, 0, 0, GEMSTONE, CLR_CYAN, O1_NONE),
RING("aggravate monster", "sapphire",
     AGGRAVATE_MONSTER, 0, 0,        150, 1, 0,  9, 0, 0, 0, 0, GEMSTONE, CLR_BLUE, O1_NONE),
RING("conflict", "ruby",
     CONFLICT, 0, 0, 300, 1, 0,  9, 0, 0, 0, 0, GEMSTONE, CLR_RED, O1_NONE),
RING("warning", "diamond",
     WARNING, 0, 0, 100, 1, 0, 10, 0, 0, 0, 0, GEMSTONE, CLR_WHITE, O1_NONE),
RING("poison resistance", "pearl",
     POISON_RES, 0, 0,               150, 1, 0,  4, 0, 0, 0, 0, BONE, CLR_WHITE, O1_NONE),
RING("fire resistance", "iron",
     FIRE_RES, 0, 0, 200, 1, 0,  5, 0, 0, 0, 0, IRON, HI_METAL, O1_FIRE_RESISTANT),
RING("cold resistance", "brass",
     COLD_RES, 0, 0, 150, 1, 0,  4, 0, 0, 0, 0, COPPER, HI_COPPER, O1_COLD_RESISTANT),
RING("shock resistance", "copper",
     SHOCK_RES, 0, 0, 150, 1, 0,  3, 0, 0, 0, 0, COPPER, HI_COPPER, O1_LIGHTNING_RESISTANT),
RING("life protection", "platinum",
     DEATH_RES, DRAIN_RES, 0,               200, 1, 0,  3, 0, 0, 0, 0, COPPER, HI_COPPER, O1_LIGHTNING_RESISTANT),
RING("free action", "twisted",
     FREE_ACTION, 0, 0, 200, 1, 0,  6, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
RING("slow digestion", "steel",
     SLOW_DIGESTION, 0, 0, 200, 1, 0,  8, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
RING("teleportation", "silver",
     TELEPORT, 0, 0, 200, 1, 0,  3, 0, 0, 0, 0, SILVER, HI_SILVER, O1_NONE),
RING("teleport control", "gold",
     TELEPORT_CONTROL, 0, 0, 300, 1, 0,  3, 0, 0, 0, 0, GOLD, HI_GOLD, O1_NONE),
RING("polymorph", "ivory",
     POLYMORPH, 0, 0, 300, 1, 0,  4, 0, 0, 0, 0, BONE, CLR_WHITE, O1_POLYMORPH_RESISTANT),
RING("polymorph control", "emerald",
     POLYMORPH_CONTROL, 0, 0, 300, 1, 0,  8, 0, 0, 0, 0, GEMSTONE, CLR_BRIGHT_GREEN, O1_POLYMORPH_RESISTANT),
RING("invisibility", "wire",
     INVIS, 0, 0, 150, 1, 0,  5, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
RING("see invisible", "engagement",
     SEE_INVIS, 0, 0, 150, 1, 0,  5, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
RING("protection from shape changers", "shiny",
     PROT_FROM_SHAPE_CHANGERS, 0, 0, 100, 1, 0,  5, 0, 0, 0, 0, IRON, CLR_BRIGHT_CYAN, O1_POLYMORPH_RESISTANT),
#undef RING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,prob,power,power2,power3,manabonus,hpbonus,bonusattrs,abon,flags) \
    OBJECT(OBJ(name, desc), None,                                           \
           BITS(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, P_NONE, IRON),        \
           power, power2, power3,  AMULET_CLASS, prob, 0, 5, 150,  0, 0, 0, 0, 0, 0, 0, 0, manabonus, hpbonus, bonusattrs, abon, 20, HI_METAL, 0, 0, 0, 0, flags)
AMULET("amulet of ESP",                "circular", 125, TELEPAT, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet of life saving",       "spherical", 75,  LIFESAVED, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet of strangulation",          "oval", 110, STRANGLED, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet of restful sleep",    "triangular", 135, SLEEPY, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet versus poison",        "pyramidal", 115, POISON_RES, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet of change",               "square", 130, 0, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet of unchanging",          "concave", 45,  UNCHANGING, 0, 0, 0, 0, 0, 0, O1_POLYMORPH_RESISTANT),
AMULET("amulet of reflection",        "hexagonal", 75,  REFLECTING, 0, 0, 0, 0, 0, 0, O1_NONE),
AMULET("amulet of mana",		        "convex",		50,  0, 0, 0, 75, 0, 0, 0, O1_NONE), //100 mana points
AMULET("demon blood talisman", "non-Euclidian-shaped",	25, 0, 0, 0, 100, 0, 0, 0, O1_MANA_PERCENTAGE_BONUS), //doubles mana capacity
AMULET("periapt of vitality",	        "linear",		50, 0, 0, 0, 0, 25, 0, 0, O1_NONE), //50 hit points
AMULET("amulet of magical breathing", "octagonal", 65,  MAGICAL_BREATHING, 0, 0, 0, 0, 0, 0, O1_NONE),
/* fixed descriptions; description duplication is deliberate;
 * fake one must come before real one because selection for
 * description shuffling stops when a non-magic amulet is encountered
 */
OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
           "Amulet of Yendor"), None,
       BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, PLASTIC),
       0, 0, 0, AMULET_CLASS, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, HI_METAL, 0, 0, 0, 0, O1_NONE),
OBJECT(OBJ("Amulet of Yendor", /* note: description == name */
           "Amulet of Yendor"), None,
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, MITHRIL),
       0, 0, 0, AMULET_CLASS, 0, 0, 10, 30000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, HI_METAL, 0, 0, 0, 0, O1_INDESTRUCTIBLE),
#undef AMULET

/* decorations */
#define MISCELLANEOUSITEM(name,desc,sub,specialworntext,kn,magic,mergeable,charged,prob,cost,wt,power,power2,power3,manabonus,hpbonus,bonusattrs,abon,nut,material,color,flags) \
    OBJECT(OBJ(name, desc), specialworntext,                                           \
           BITS(kn, mergeable, charged, 0, magic, charged, 0, 0, 0, 0, 0, sub, material),        \
           power, power2, power3,  MISCELLANEOUS_CLASS, prob, 0, wt, cost,  0, 0, 0, 0, 0, 0, 0, 0, manabonus, hpbonus, bonusattrs, abon, nut, color, 0, 0, 0, 0, flags)
MISCELLANEOUSITEM("brooch of shielding", "brooch",	MISC_MULTIPLE_PERMITTED, None,
	0, 1, 0, 0, 200, 150, 5,
	PROTECTION, 0, 0, 0, 0, 0, 0,
	20, GOLD, HI_GOLD, O1_NONE),
MISCELLANEOUSITEM("nose ring of bull strength", "golden nose ring", MISC_NOSERING, None,
	0, 1, 0, 0, 200, 200, 50,
	0, 0, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, 18,
	50, GOLD, HI_GOLD, O1_NONE),
MISCELLANEOUSITEM("headband of intellect", "shining blue headband", MISC_HEADBAND, None,
	0, 1, 0, 0, 100, 200, 50,
	0, 0, 0, 0, 0, BONUS_TO_INT | SETS_FIXED_ATTRIBUTE, 19,
	50, CLOTH, CLR_BLUE, O1_NONE),
MISCELLANEOUSITEM("golden earrings", None, MISC_EARRINGS, None,
	1, 0, 0, 0, 100, 200, 50,
	0, 0, 0, 0, 0, BONUS_TO_CHA, 1,
	50, GOLD, HI_GOLD, O1_CONFERS_POWERS_TO_FEMALE_ONLY),
MISCELLANEOUSITEM("ioun stone of protection", "ioun stone", MISC_IOUN_STONE, "orbiting your head",
	0, 1, 0, 0, 100, 200, 50, //STARTMARKER FOR IOUNSTONES
	PROTECTION, 0, 0, 0, 0, 0, 0,
	50, MINERAL, CLR_RED, O1_NONE),
MISCELLANEOUSITEM("ioun stone of regeneration", "ioun stone", MISC_IOUN_STONE, "orbiting your head",		
	0, 1, 0, 0, 100, 200, 50,
	REGENERATION, 0, 0, 0, 0, 0, 0,
	50, MINERAL, CLR_WHITE, O1_NONE),
MISCELLANEOUSITEM("ioun stone of sustenance", "ioun stone", MISC_IOUN_STONE, "orbiting your head",		
	0, 1, 0, 0, 100, 200, 50, //ENDMARKER FOR IOUNSTONES
	SLOW_DIGESTION, 0, 0, 0, 0, 0, 0,
	50, MINERAL, CLR_WHITE, O1_NONE),
MISCELLANEOUSITEM("wings of flying", "artificial wings", MISC_WINGS, "attached to your back",		
	0, 1, 0, 0, 100, 200, 50,
	FLYING, 0, 0, 0, 0, 0, 0,
	50, ORGANIC, CLR_WHITE, O1_NONE),
#undef MISCELLANEOUSITEM

/* tools ... */
/* tools with weapon characteristics come last */
#define TOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,abon,mat,color,flags) \
    OBJECT(OBJ(name, desc), None,                                            \
           BITS(kn, mrg, chg, 0, mgc, chg, 0, 0, 0, 0, 0, P_NONE, mat), \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost,  0, 0, 0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattr, abon, wt, color, 0, 0, cooldown, 0, flags)
#define SPELLTOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,dir,dirsubtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus,cooldown,mat,color,flags) \
    OBJECT(OBJ(name, desc), None,                                            \
           BITS(kn, mrg, chg, 0, mgc, chg, 0, 0, 0, 0, dir, P_NONE, mat), \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, 0, 0, 0, 0, 0, 0, wt, color, dirsubtype, 0, cooldown, 0, flags)
#define CONTAINER(name,desc,kn,mgc,chg,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,abon,mat,color,flags) \
    OBJECT(OBJ(name, desc), None,                                            \
           BITS(kn, 0, chg, 1, mgc, chg, 0, 0, 0, 0, 0, P_NONE, mat),   \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost,  0, 0, 0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattr, abon, wt, color, 0, 0, cooldown, 0, flags)
#define WEPTOOL(name,desc,kn,mgc,bi,prob,wt,cost,sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon,sub,cooldown,mat,clr,flags)\
    OBJECT(OBJ(name, desc), None,                                            \
           BITS(kn, 0, 1, 0, mgc, 1, 0, 0, bi, 0, hitbon, sub, mat),    \
           0, 0, 0, TOOL_CLASS, prob, 0, wt, cost, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, hitbon, 0, 0, 0, 0, 0, wt, clr, 0, 0, cooldown, 0, flags)
/* containers */
CONTAINER("large box",       None, 1, 0, 0, 40, 350,   8, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE), //STARTMARKER 1
CONTAINER("chest",           None, 1, 0, 0, 30, 600,  16, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
CONTAINER("ice box",         None, 1, 0, 0,  5, 900,  42, 0, 0, 0, 0, 0, PLASTIC, CLR_WHITE, O1_NONE),
CONTAINER("bookshelf",		 None, 1, 0, 0,  0, 1600, 36, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
CONTAINER("backpack",		 None, 1, 0, 0,  5,  15,   5, 0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("leather bag",	 "bag",0, 0, 0,	 5,  15,  25, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
CONTAINER("oriental silk sack", "bag",0,0,0, 5,   3,  50, 0, 0, 0, 0, 0, SILK, HI_CLOTH, O1_NONE),
CONTAINER("expensive handbag", "bag", 0, 0, 0, 2, 5, 500, 0, 0, 0, BONUS_TO_CHA, 2, SILK, HI_GOLD, O1_CONFERS_POWERS_WHEN_CARRIED | O1_CONFERS_POWERS_TO_FEMALE_ONLY),
CONTAINER("sack",           "bag", 0, 0, 0, 10,  10,   5, 0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("oilskin sack",   "bag", 0, 0, 0,  5,  10, 100, 0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
/* magic bags start here */
CONTAINER("bag of holding", "bag",					0, 1, 0, 20, 15, 100,   0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE), //STARTMARKER 2
CONTAINER("bag of wizardry", "bag",					0, 1, 0, 10, 15, 100,   0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("bag of treasure hauling", "bag",			0, 1, 0, 15, 15, 100,   0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE),
CONTAINER("quiver of infinite arrows", "bag",		0, 1, 0,  4, 15, 100, 300, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_SPECIAL_ENCHANTABLE),
CONTAINER("pouch of endless bolts", "bag",			0, 1, 0,  4, 15, 100, 300, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_SPECIAL_ENCHANTABLE),
CONTAINER("bag of tricks",  "bag",					0, 1, 1, 20, 15, 100,  0, 0, 0, 0, 0, CLOTH, HI_CLOTH, O1_NONE), //ENDMARKER 1&2
#undef CONTAINER

/* lock opening tools */
TOOL("skeleton key",       "key", 0, 0, 0, 0, 80,  2, 10, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
TOOL("lock pick",           None, 1, 0, 0, 0, 60,  2, 20, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
TOOL("credit card",         None, 1, 0, 0, 0, 15,  1, 10, 0, 0, 0, 0, 0, PLASTIC, CLR_WHITE, O1_NONE),
/* light sources */
TOOL("tallow candle",   "candle", 0, 1, 0, 0, 20,  1, 10, 0, 0, 0, 0, 0, WAX, CLR_WHITE, O1_NONE),
TOOL("wax candle",      "candle", 0, 1, 0, 0,  5,  1, 20, 0, 0, 0, 0, 0, WAX, CLR_WHITE, O1_NONE),
TOOL("brass lantern",       None, 1, 0, 0, 0, 30, 30, 12, 0, 0, 0, 0, 0, COPPER, CLR_YELLOW, O1_NONE),
TOOL("oil lamp",          "lamp", 0, 0, 0, 0, 45, 20, 10, 0, 0, 0, 0, 0, COPPER, CLR_YELLOW, O1_NONE),
TOOL("magic lamp",        "lamp", 0, 0, 1, 0, 15, 20, 50, 0, 0, 0, 0, 0, COPPER, CLR_YELLOW, O1_NONE),
/* other tools */
TOOL("expensive camera",    None, 1, 0, 0, 1, 15, 12,200, 0, 0, 0, 0, 0, PLASTIC, CLR_BLACK, O1_NONE),
TOOL("mirror",   "looking glass", 0, 0, 0, 0, 35, 13, 10, 0, 0, 0, 0, 0, GLASS, HI_SILVER, O1_NONE),
TOOL("magic mirror", "looking glass", 0, 0, 0, 0, 0, 13, 10, 0, 30, 0, 0, 0, GLASS, HI_SILVER, O1_CONFERS_POWERS_WHEN_CARRIED | O1_MANA_PERCENTAGE_BONUS),
TOOL("holy symbol", "religious symbol", 0, 0, 1, 0, 10, 10,100, 0, 0, 0, 0, 0, SILVER, HI_SILVER, O1_TREATED_AS_MATERIAL_COMPONENT),
TOOL("crystal ball", "glass orb", 0, 0, 1, 1, 15,150, 60, 100, 0, 0, 0, 0, GLASS, HI_GLASS, O1_NONE),
TOOL("lenses",              None, 1, 0, 0, 0,  5,  3, 80, 0, 0, 0, 0, 0, GLASS, HI_GLASS, O1_NONE),
TOOL("blindfold",           None, 1, 0, 0, 0, 50,  2, 20, 0, 0, 0, 0, 0, CLOTH, CLR_BLACK, O1_NONE),
TOOL("towel",               None, 1, 0, 0, 0, 50,  2, 50, 0, 0, 0, 0, 0, CLOTH, CLR_MAGENTA, O1_NONE),
TOOL("saddle",              None, 1, 0, 0, 0,  5,200,150, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
TOOL("leash",               None, 1, 0, 0, 0, 50, 12, 20, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
TOOL("stethoscope",         None, 1, 0, 0, 0, 25,  4, 75, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
TOOL("tinning kit",         None, 1, 0, 0, 1, 15,100, 30, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
TOOL("tin opener",          None, 1, 0, 0, 0, 30,  4, 30, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
TOOL("can of grease",       None, 1, 0, 0, 1, 15, 15, 20, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
TOOL("figurine",            None, 1, 0, 1, 0, 25, 50, 80, 0, 0, 0, 0, 0, MINERAL, HI_MINERAL, O1_NONE),
        /* monster type specified by obj->corpsenm */
TOOL("magic marker",        None, 1, 0, 1, 1, 15,  2, 50, 0, 0, 0, 0, 0, PLASTIC, CLR_RED, O1_NONE),
/* traps */
TOOL("land mine",           None, 1, 0, 0, 0, 0, 300,180, 0, 0, 0, 0, 0, IRON, CLR_RED, O1_NONE),
TOOL("beartrap",            None, 1, 0, 0, 0, 0, 200, 60, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
/* instruments;
   "If tin whistles are made out of tin, what do they make foghorns out of?" */
TOOL("tin whistle",    "whistle", 0, 0, 0, 0,100, 2, 10, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
TOOL("magic whistle",  "whistle", 0, 0, 1, 0, 30, 2, 10, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
TOOL("wooden flute",     "flute", 0, 0, 0, 0,  4, 5, 12, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
TOOL("magic flute",      "flute", 0, 0, 1, 1,  2, 5, 36, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
TOOL("tooled horn",       "horn", 0, 0, 0, 0,  5, 18, 15, 0, 0, 0, 0, 0, BONE, CLR_WHITE, O1_NONE),
SPELLTOOL("frost horn",   "horn", 0, 0, 1, 1,  2, 18, 50, RAY, RAY_WND_COLD, 6, 6, 0, 0, 0, 0, 5, BONE, CLR_WHITE, O1_NONE),
SPELLTOOL("fire horn",    "horn", 0, 0, 1, 1,  2, 18, 50, RAY, RAY_WND_FIRE, 6, 6, 0, 0, 0, 0, 5, BONE, CLR_WHITE, O1_NONE),
TOOL("horn of plenty",    "horn", 0, 0, 1, 1,  2, 18, 50, 300, 0, 0, 0, 0, BONE, CLR_WHITE, O1_NONE),
        /* horn, but not an instrument */
TOOL("wooden harp",       "harp", 0, 0, 0, 0,  4, 30, 50, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
TOOL("magic harp",        "harp", 0, 0, 1, 1,  2, 30, 50, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
TOOL("bell",                None, 1, 0, 0, 0,  2, 30, 50, 0, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
TOOL("bugle",               None, 1, 0, 0, 0,  4, 10, 15, 0, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
TOOL("leather drum",      "drum", 0, 0, 0, 0,  4, 25, 25, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
TOOL("drum of earthquake","drum", 0, 0, 1, 1,  2, 25, 25, 0, 0, 0, 0, 0, LEATHER, HI_LEATHER, O1_NONE),
/* tools useful as weapons */
WEPTOOL("pick-axe", None,
        1, 0, 0, 20, 75,  50,  1, 6, 0, 1, 3, 0, WHACK,  P_PICK_AXE, 0, IRON, HI_METAL, O1_NONE),
WEPTOOL("grappling hook", "iron hook",
        0, 0, 0,  5,  30,  50,  1, 2, 0, 1, 6, 0, WHACK,  P_FLAIL, 0, IRON, HI_METAL, O1_NONE),
WEPTOOL("unicorn horn", None,
        1, 1, 1,  0,  20, 100, 1, 12, 0, 1, 12, 0, PIERCE, P_UNICORN_HORN, 100,
                                                           BONE, CLR_WHITE, O1_NONE),
        /* 3.4.1: unicorn horn left classified as "magic" */
/* two unique tools;
 * not artifacts, despite the comment which used to be here
 */
OBJECT(OBJ("Candelabrum of Invocation", "candelabrum"), None,
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, GOLD),
       0, 0, 0, TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, HI_GOLD, 0, 0, 0, 0, O1_INDESTRUCTIBLE),
OBJECT(OBJ("Bell of Opening", "silver bell"), None,
       BITS(0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, P_NONE, SILVER),
       0, 0, 0, TOOL_CLASS, 0, 0, 10, 5000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, HI_SILVER, 0, 0, 0, 0, O1_INDESTRUCTIBLE),
#undef TOOL
#undef WEPTOOL

/* Comestibles ... */
#define FOOD(name, prob, delay, wt, unk, tin, nutrition, color, flags)         \
    OBJECT(OBJ(name, None), None,                                      \
           BITS(1, 1, unk, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, tin), 0, 0, 0,     \
           FOOD_CLASS, prob, delay, wt, nutrition / 20 + 5,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           nutrition, color, 0, 0, 0, 0, flags)
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
FOOD("tripe ration",        140,  2, 10, 0, FLESH, 200, CLR_BROWN, O1_NONE),
FOOD("corpse",                0,  1,  0, 0, FLESH,   0, CLR_BROWN, O1_NONE),
FOOD("egg",                  85,  1,  1, 1, FLESH,  80, CLR_WHITE, O1_NONE),
FOOD("meatball",              0,  1,  1, 0, FLESH,   5, CLR_BROWN, O1_NONE),
FOOD("meat stick",            0,  1,  1, 0, FLESH,   5, CLR_BROWN, O1_NONE),
FOOD("huge chunk of meat",    0, 20,400, 0, FLESH,2000, CLR_BROWN, O1_NONE),
/* special case because it's not mergable */
OBJECT(OBJ("meat ring", None), None,
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FLESH),
       0, 0, 0, FOOD_CLASS, 0, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, CLR_BROWN, 0, 0, 0, 0, O1_NONE),
/* pudding 'corpses' will turn into these and combine;
   must be in same order as the pudding monsters */
FOOD("glob of gray ooze",     0,  2, 20, 0, FLESH,  20, CLR_GRAY, O1_NONE),
FOOD("glob of brown pudding", 0,  2, 20, 0, FLESH,  20, CLR_BROWN, O1_NONE),
FOOD("glob of green slime",   0,  2, 20, 0, FLESH,  20, CLR_GREEN, O1_NONE),
FOOD("glob of black pudding", 0,  2, 20, 0, FLESH,  20, CLR_BLACK, O1_NONE),

/* fruits & veggies */
FOOD("kelp frond",            0,  1,  1, 0, VEGGY,  30, CLR_GREEN, O1_NONE),
FOOD("eucalyptus leaf",       6,  1,  1, 0, VEGGY,  30, CLR_GREEN, O1_NONE),
FOOD("apple",                10,  1,  2, 0, VEGGY,  50, CLR_RED, O1_NONE),
FOOD("orange",               10,  1,  2, 0, VEGGY,  80, CLR_ORANGE, O1_NONE),
FOOD("pear",                 10,  1,  2, 0, VEGGY,  50, CLR_BRIGHT_GREEN, O1_NONE),
FOOD("melon",                10,  1,  5, 0, VEGGY, 100, CLR_BRIGHT_GREEN, O1_NONE),
FOOD("banana",               10,  1,  2, 0, VEGGY,  80, CLR_YELLOW, O1_NONE),
FOOD("carrot",               10,  1,  2, 0, VEGGY,  50, CLR_ORANGE, O1_NONE),
/* name of slime mold is changed based on player's OPTION=fruit:something
   and bones data might have differently named ones from prior games */
FOOD("slime mold",           75,  1,  5, 0, VEGGY, 250, HI_ORGANIC, O1_NONE),

/* people food */
FOOD("lump of royal jelly",   0,  1,  2, 0, VEGGY, 200, CLR_YELLOW, O1_NONE),
FOOD("cream pie",            25,  1, 10, 0, VEGGY, 100, CLR_WHITE, O1_NONE),
FOOD("candy bar",            15,  1,  2, 0, VEGGY, 100, CLR_BROWN, O1_NONE),
FOOD("fortune cookie",       49,  1,  1, 0, VEGGY,  40, CLR_YELLOW, O1_NONE),
FOOD("pancake",              30,  2,  2, 0, VEGGY, 200, CLR_YELLOW, O1_NONE),
FOOD("lembas wafer",         30,  2,  5, 0, VEGGY, 800, CLR_WHITE, O1_NONE),
FOOD("cram ration",          30,  3, 15, 0, VEGGY, 600, HI_ORGANIC, O1_NONE),
FOOD("food ration",         380,  5, 20, 0, VEGGY, 800, HI_ORGANIC, O1_NONE),
FOOD("K-ration",              0,  1, 10, 0, VEGGY, 400, HI_ORGANIC, O1_NONE),
FOOD("C-ration",              0,  1, 10, 0, VEGGY, 300, HI_ORGANIC, O1_NONE),
/* tins have type specified by obj->spe (+1 for spinach, other implies
   flesh; negative specifies preparation method {homemade,boiled,&c})
   and by obj->corpsenm (type of monster flesh) */
FOOD("tin",                  75,  0, 10, 1, METAL,   0, HI_METAL, O1_NONE),
#undef FOOD

/* potions ... */
#define POTION(name,desc,mgc,power,prob,cost,color,flags) \
    OBJECT(OBJ(name, desc), None,                                            \
           BITS(0, 1, 0, 0, mgc, 0, 0, 0, 0, 0, 0, P_NONE, GLASS),      \
           power, 0, 0, POTION_CLASS, prob, 0, 12, cost,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, color, 0, 0, 0, 0, flags)
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
POTION("greater healing",	 "deep red",  1, 0, 20, 200, CLR_RED, O1_NONE),
POTION("full healing",          "black",  1, 0, 10, 200, CLR_BLACK, O1_NONE),
POTION("gain level",            "milky",  1, 0, 20, 300, CLR_WHITE, O1_NONE),
POTION("enlightenment",        "swirly",  1, 0, 20, 200, CLR_BROWN, O1_NONE),
POTION("monster detection",    "bubbly",  1, 0, 40, 150, CLR_WHITE, O1_NONE),
POTION("object detection",      "smoky",  1, 0, 32, 150, CLR_GRAY, O1_NONE),
POTION("gain energy",          "cloudy",  1, 0, 42, 150, CLR_WHITE, O1_NONE),
POTION("greater energy",     "greenish",  1, 0, 20, 150, CLR_GREEN, O1_NONE),
POTION("full energy",		  "silvery",  1, 0, 10, 150, CLR_WHITE, O1_NONE),
POTION("sleeping",       "effervescent",  1, 0, 32, 100, CLR_GRAY, O1_NONE),
POTION("polymorph",            "golden",  1, 0, 10, 200, CLR_YELLOW, O1_POLYMORPH_RESISTANT),
POTION("booze",                 "brown",  0, 0, 32,  50, CLR_BROWN, O1_COLD_RESISTANT),
POTION("sickness",              "fizzy",  0, 0, 32,  50, CLR_CYAN, O1_NONE),
POTION("fruit juice",            "dark",  0, 0, 32,  50, CLR_BLACK, O1_NONE),
POTION("acid",                  "white",  0, 0, 10, 250, CLR_WHITE, O1_COLD_RESISTANT),
POTION("oil",                   "murky",  0, 0, 30, 250, CLR_BROWN, O1_COLD_RESISTANT),
/* fixed description
 */
POTION("water",                 "clear",  0, 0, 92, 100, CLR_CYAN, O1_TREATED_AS_MATERIAL_COMPONENT),
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,mgc,prob,cost,flags) \
    OBJECT(OBJ(name, text), None,                                           \
           BITS(0, 1, 0, 0, mgc, 0, 0, 0, 0, 0, 0, P_NONE, PAPER),    \
           0, 0, 0, SCROLL_CLASS, prob, 0, 4, cost,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, HI_PAPER, 0, 0, 0, 0, flags)
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
#define SPELL(name,desc,contentdesc,sub,prob,learndelay,cooldown,level,manacost,attr,range,radius,mgc,dir,dirsubtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,color,flags)  \
    OBJECT(OBJ(name, desc), contentdesc,                                            \
           BITS(0, 0, 0, 0, mgc, 0, 0, 0, 0, 0, dir, sub, PAPER),       \
           0, 0, 0, SPBOOK_CLASS, prob, learndelay, 50, (level + 2) * 50 + (level + 1) * (level + 1) * 5,               \
           sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, cooldown, level, manacost, attr, range, radius, 20, color,dirsubtype, 0, 0, 0, flags)
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

SPELL("dig",             "parchment", None,
      P_TRANSMUTATION_SPELL,	10,  4,			 4, 3, 10, A_INT, 10, 0, 1, RAY, RAY_DIGGING, 0, 0, 0, 0, 0, 0, HI_LEATHER, O1_NONE),
SPELL("magic missile",   "vellum", None,
      P_ARCANE_SPELL,			10,  2,			 0, 0,  7, A_INT, 10, 0, 1, RAY, RAY_MAGIC_MISSILE, 3, 4, 0, 0, 0, 0, HI_LEATHER, O1_NONE),
#undef PAPER /* revert to normal material */
SPELL("fire bolt",        "light red", None,
      P_ARCANE_SPELL,			10,  4,			 0, 3, 20, A_INT, 10, 0, 1, RAY, RAY_FIRE, 3, 6, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT),
SPELL("fireball",         "ragged", None,
      P_ARCANE_SPELL,			10,  6,			 2, 5, 50, A_INT, 14, 0, 1, TARGETED, RAY_FIRE, 5, 6, 0, 0, 0, 0, HI_PAPER, O1_FIRE_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("fire storm",       "crimson", None,
      P_ARCANE_SPELL,			 5,  8,			 4, 7,100, A_INT, 16, 0, 1, TARGETED, RAY_FIRE, 12, 6, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("meteor swarm",     "oval-patterned", None,
      P_ARCANE_SPELL,			 5, 10,			 6, 9,200, A_INT, 18, 0, 1, TARGETED, RAY_FIRE, 8, 6, 0, 2, 2, 1, CLR_RED, O1_FIRE_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("cone of cold",     "dog eared", None,
      P_ARCANE_SPELL,			10,  6,			 0, 5, 50, A_INT, 12, 0, 1, RAY, RAY_COLD, 15, 6, 0, 0, 0, 0, HI_PAPER, O1_COLD_RESISTANT),
SPELL("ice storm",		  "mithril-plated", None,
      P_ARCANE_SPELL,			10,  7,			 2, 6, 75, A_INT, 14, 0, 1, TARGETED, RAY_COLD, 4, 10, 8, 0, 0, 0, HI_SILVER, O1_COLD_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),

SPELL("sleep",				"mottled", None,
      P_ENCHANTMENT_SPELL,		10,  1,			20, 1, 10, A_CHA, 12, 0, 1, RAY, RAY_SLEEP, 0, 0, 0, 2, 6, 7, HI_PAPER, O1_NONE),
SPELL("mass sleep",         "obtuse", None,
      P_ENCHANTMENT_SPELL,		 5,  5,			30, 5, 50, A_CHA, 0, 6, 1, NODIR, 0, 0, 0, 0, 4, 6, 14, HI_PAPER, O1_NONE),

SPELL("disintegrate",	  "stained", None,
      P_ARCANE_SPELL,			 5, 12,			 4, 10,300, A_INT, 8, 0, 1, RAY, RAY_DISINTEGRATION, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_DISINTEGRATION_RESISTANT),
SPELL("lightning bolt",   "shimmering", None,
      P_ARCANE_SPELL,			10,  3,			 0, 2, 15, A_INT, 14, 0, 1, RAY, RAY_LIGHTNING, 6, 6, 0, 0, 0, 0, HI_PAPER, O1_LIGHTNING_RESISTANT),
SPELL("thunderstorm",	  "strange", None,
      P_ARCANE_SPELL,			10,  10,		 3, 8,150, A_INT, 18, 0, 1, TARGETED, RAY_LIGHTNING, 10, 12, 0, 0, 0, 0, HI_PAPER, O1_LIGHTNING_RESISTANT | O1_SPELL_EXPLOSION_EFFECT),
SPELL("light",            "cloth", None,
      P_DIVINATION_SPELL,		10,  1,			 0, 0,  2, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_CLOTH, O1_NONE),
SPELL("black blade of disaster", "ebony", "some long-forgotten arcane magic",
	P_CONJURATION_SPELL,		 5, 10,			20,11,500, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 3, 10, 30, CLR_BLACK, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY | O1_DISINTEGRATION_RESISTANT),

SPELL("power word kill",   "preposterous", "some long-forgotten arcane magic",
    P_ARCANE_SPELL,				 5,  5,			 0,11,500, A_INT, 6, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("power word stun",   "ludicrous", None, //Note monsters do not have a stun timer
    P_ARCANE_SPELL,				 5,  4,			 0, 3, 20, A_INT,10, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("power word blind",   "anomalous", None,//Note monsters have a blindness timer
    P_ARCANE_SPELL,				 5,  3,			 0, 4, 30, A_INT, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 5, 10, 50, HI_PAPER, O1_NONE),

SPELL("holy word",			"antediluvian", None,
	P_CLERIC_SPELL,				5,  6,			 0, 8,150, A_WIS, 0, 5, 1, NODIR, 0, 0, 0, 0, 5, 10, 50, HI_PAPER, O1_NONE),

SPELL("animate air",	  "wave-patterned", None,
	P_CONJURATION_SPELL,		10,  3,			15, 7,100, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 30, HI_PAPER, O1_NONE),
SPELL("animate earth",	  "octagonal-patterned", None,
	P_CONJURATION_SPELL,		10,  3,			20, 4, 30, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 30, HI_PAPER, O1_NONE),
SPELL("animate fire",	  "flame-patterned", None,
	P_CONJURATION_SPELL,		10,  3,			 5, 5, 50, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 30, HI_PAPER, O1_NONE),
SPELL("animate water",	  "water-drop-patterned", None,
	P_CONJURATION_SPELL,		10,  3,			30, 3, 20, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 30, HI_PAPER, O1_NONE),

SPELL("create gold golem", "gold-circled", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 1, 10, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_GOLD, O1_NONE),
SPELL("create glass golem", "crystalline", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 6, 75, A_AVG_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("create gemstone golem", "ruby-studded", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 8,150, A_AVG_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_RED, O1_NONE),
SPELL("create clay golem", "clay-covered", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 2, 15, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("create stone golem", "stone-studded", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 3, 20, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_METAL, O1_NONE),
SPELL("create iron golem", "iron-reinforced", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 6, 75, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("create paper golem", "flimsy", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 0,  7, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("create wood golem", "wooden", None,
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 1, 10, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),

SPELL("summon demon",	  "tenebrous", None,
	P_CONJURATION_SPELL,		 5,  5,	       300, 7,100, A_AVG_INT_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 280, HI_PAPER, O1_NONE),
SPELL("call Demogorgon",  "eldritch", "some aeon-old prayer",
	P_CONJURATION_SPELL,		 5, 20,		   400,9,200, A_AVG_INT_WIS_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 2, 100, 200, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),

SPELL("guardian angel",	  "wing-patterned", None,
	P_CLERIC_SPELL,				 5,  5,		   300, 4, 30, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 280, HI_PAPER, O1_NONE),
SPELL("divine mount",	  "horn-patterned", None,
	P_CLERIC_SPELL,				 5,  5,		   300, 6, 75, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 280, HI_PAPER, O1_NONE),
SPELL("heavenly army",	  "angelic", "some aeon-old prayer",
	P_CLERIC_SPELL,				 5,  5,		   150, 9,300, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 10, 140, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),

SPELL("detect monsters",  "leathery", None,
      P_DIVINATION_SPELL,		13,  1,			 1, 0, 5, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_LEATHER, O1_NONE),
SPELL("knock",            "pink", None,
      P_TRANSMUTATION_SPELL,	15,  1,			 0, -1, 1, A_INT, 12, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_MAGENTA, O1_NONE),
SPELL("force bolt",       "red", None,
      P_ARCANE_SPELL,			15,  2,			 0, 1, 10, A_INT, 10, 0, 1, IMMEDIATE, 0, 3, 6, 1, 0, 0, 0, CLR_RED, O1_NONE),
SPELL("magic arrow",      "hardcover", None,
      P_ARCANE_SPELL,			15,  1,			 0, -1, 4, A_INT, 18, 0, 1, IMMEDIATE, 0, 1, 8, 1, 0, 0, 0, CLR_BROWN, O1_NONE),
SPELL("confuse monster", "orange", None,
      P_ENCHANTMENT_SPELL,		15,  2,			 0, 0,  7, A_AVG_INT_CHA, 16, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_ORANGE, O1_NONE),
SPELL("cure blindness",  "yellow", None,
      P_HEALING_SPELL,			10,  2,			 1, 0,  7, A_WIS, 0, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_YELLOW, O1_NONE),
SPELL("drain life",      "velvet", None,
      P_ARCANE_SPELL,			10,  2,			 1, 5, 50, A_AVG_INT_WIS, 10, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_MAGENTA, O1_NONE),
SPELL("slow monster",    "light green", None,
      P_ENCHANTMENT_SPELL,		10,  2,			 0, 1, 10, A_INT, 12, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_GREEN, O1_NONE),
SPELL("wizard lock",     "dark green", None,
      P_TRANSMUTATION_SPELL,	10,  3,			 0, -1, 3, A_INT, 12, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_GREEN, O1_NONE),
SPELL("create monster",  "turquoise", None,
      P_CLERIC_SPELL,			10,  3,			 3, 3, 30, A_AVG_INT_WIS_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_CYAN, O1_NONE),
SPELL("detect food",     "cyan", None,
      P_DIVINATION_SPELL,		10,  3,			 0, -1, 4, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE),

SPELL("fear",			"blue-hued", None,
      P_ENCHANTMENT_SPELL,		10,  1,			 0, 0, 7, A_AVG_INT_WIS_CHA, 10, 0, 1, IMMEDIATE, 0, 0, 0, 0, 6, 6, 10, CLR_BRIGHT_BLUE, O1_NONE),
SPELL("mass fear",      "light blue", None,
      P_ENCHANTMENT_SPELL,		10,  2,			 0, 4, 30, A_AVG_INT_WIS_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 6, 6, 10, CLR_BRIGHT_BLUE, O1_NONE),

SPELL("clairvoyance",    "dark blue", None,
      P_DIVINATION_SPELL,		10,  3,			 3, 2, 15, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLUE, O1_NONE),
SPELL("cure sickness",   "indigo", None,
      P_HEALING_SPELL,			12,  3,			 3, 2, 15, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLUE, O1_NONE),
SPELL("cure petrification",   "runic", None,
      P_HEALING_SPELL,			 5,  5,			 0, 4, 30, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE),

SPELL("charm monster",   "eye-patterned", None,
      P_ENCHANTMENT_SPELL,		10,  3,			 2, 3, 20, A_AVG_INT_WIS_CHA, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 10, 50, HI_PAPER, O1_NONE),
SPELL("sphere of charming", "magenta", None,
      P_ENCHANTMENT_SPELL,		10,  5,			 5, 5, 50, A_AVG_INT_WIS_CHA, 0, 1, 1, NODIR, 0, 0, 0, 0, 10, 10, 50, CLR_MAGENTA, O1_NONE),
SPELL("mass charm",		 "multi-hued", None,
      P_ENCHANTMENT_SPELL,		5,  7,			10, 7,100, A_AVG_INT_WIS_CHA, 0, 5, 1, NODIR, 0, 0, 0, 0, 10, 10, 50, CLR_MAGENTA, O1_NONE),
SPELL("dominate monster",   "eye-and-triangle-patterned", None,
      P_ENCHANTMENT_SPELL,		5,  6,			 2, 6, 75, A_AVG_INT_WIS_CHA, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELL_IS_NONREVERSIBLE_PERMANENT),
SPELL("sphere of domination", "pyramid-patterned", None,
      P_ENCHANTMENT_SPELL,		5,  8,			 5, 8,150, A_AVG_INT_WIS_CHA, 0, 1, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELL_IS_NONREVERSIBLE_PERMANENT),
SPELL("mass domination", "fractal-patterned", "some long-forgotten arcane magic",
      P_ENCHANTMENT_SPELL,		5, 10,			10,10,300, A_AVG_INT_WIS_CHA, 0, 5, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELL_IS_NONREVERSIBLE_PERMANENT | O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),

SPELL("haste self",      "purple", None,
      P_MOVEMENT_SPELL,			10,  4,			 4, 3, 30, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 10, 100, CLR_MAGENTA, O1_NONE),
SPELL("detect unseen",   "violet", None,
      P_DIVINATION_SPELL,		10,  4,			 2, 1, 10, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 5, 10, 100, CLR_MAGENTA, O1_NONE),
SPELL("levitation",      "tan", None,
      P_MOVEMENT_SPELL,			10,  4,			 3, 2, 15, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 5, 10, 140, CLR_BROWN, O1_NONE),
SPELL("healing",         "white", None,
      P_HEALING_SPELL,			10,  2,			 0, 1, 10, A_WIS, 0, 0, 1, IMMEDIATE, 0, 4, 4, 0, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("extra healing",   "plaid", None,
      P_HEALING_SPELL,			10,  5,			 2, 3, 20, A_WIS, 0, 0, 1, IMMEDIATE, 0, 6, 6, 0, 0, 0, 0, CLR_GREEN, O1_NONE),
SPELL("greater healing",  "darkened", None,
      P_HEALING_SPELL,			10,  5,			 4, 5, 50, A_WIS, 0, 0, 1, IMMEDIATE, 0, 8, 8, 8, 0, 0, 0, CLR_GREEN, O1_NONE),
SPELL("full healing",     "light gray", None,
      P_HEALING_SPELL,			10,  6,			 6, 7, 50, A_WIS, 0, 0, 1, IMMEDIATE, 0, 0, 0, 255, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("restore ability",  "light brown", None,
      P_HEALING_SPELL,			10,  5,			 6, 5, 50, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE),
SPELL("invisibility",     "dark brown", None,
      P_ENCHANTMENT_SPELL,		10,  5,			120,2, 15, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 2, 20, 50, CLR_BROWN, O1_NONE),
SPELL("detect treasure",  "gray", None,
      P_DIVINATION_SPELL,		10,  5,			 1, 0,  7, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_GRAY, O1_NONE),
SPELL("remove curse",     "wrinkled", None,
      P_CLERIC_SPELL,			10,  5,			 5, 4, 20, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("magic mapping",    "dusty", None,
      P_DIVINATION_SPELL,		10,  7,			 5, 4, 60, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("identify",         "bronze-plated", None,
      P_DIVINATION_SPELL,		10,  5,			 5, 4, 30, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_COPPER, O1_NONE),
SPELL("turn undead",      "copper-plated", None,
      P_CLERIC_SPELL,			11,  8,			 6, 5, 50, A_WIS, 0, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_COPPER, O1_NONE),
SPELL("polymorph",        "silver-plated", None,
      P_TRANSMUTATION_SPELL,    10,  6,			 6, 5, 50, A_INT, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_SILVER, O1_NONE),
SPELL("teleport away",    "gold-plated", None,
      P_MOVEMENT_SPELL,			10,  6,			 5, 4, 30, A_INT, 0, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_GOLD, O1_NONE),
SPELL("create familiar",  "glittering", None,
      P_CLERIC_SPELL,			10,  7,			 5, 4, 30, A_AVG_INT_WIS_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("cancellation",     "shining", None,
      P_TRANSMUTATION_SPELL,	10,  8,			 6, 5, 50, A_INT, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 1, 20, 30, CLR_WHITE, O1_SPELL_BYPASSES_MAGIC_RESISTANCE),
SPELL("protection",       "dull", None,
	P_ABJURATION_SPELL,			10, 3,		   200, 2, 15, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 2, 10, 40, HI_PAPER, O1_NONE),
SPELL("jumping",          "thin", None,
      P_MOVEMENT_SPELL,			10,  3,			 0,-1,  4, A_DEX, 0, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("stone to flesh",   "thick", None,
      P_HEALING_SPELL,			10,  1,			 4, 3, 20, A_AVG_INT_WIS, 1, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("touch of petrification",   "convoluted", None,
	P_TRANSMUTATION_SPELL,		5,  6,			 0, 6, 75, A_INT, 1, 0, 1, TOUCH, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("flesh to stone",   "amorphous", None,
	P_TRANSMUTATION_SPELL,		5,  8,			 2, 8,150, A_INT, 6, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("touch of death",   "heavy", "ancient Stygian magic",
	P_NECROMANCY_SPELL,		    5,  8,			 8, 8,150, A_AVG_INT_WIS, 1, 0, 1, TOUCH, RAY_DEATH, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("finger of death",  "leather bound", "ancient Stygian magic",
	P_NECROMANCY_SPELL,		    5, 10,			10, 9,200, A_AVG_INT_WIS,  8, 0, 1, RAY, RAY_DEATH, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("deathspell",	      "morbid", "ancient Stygian magic",
	P_NECROMANCY_SPELL,		    5, 10,			12,10,300, A_AVG_INT_WIS, 12, 1, 1, TARGETED, RAY_DEATH, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY | O1_SPELL_EXPLOSION_EFFECT),
SPELL("armageddon",       "pitch black", "ancient Stygian magic",
	P_NECROMANCY_SPELL,			2, 14,			20,12,750, A_AVG_INT_WIS, 0, 255, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLACK, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("wish",			  "platinum-plated", "some long-forgotten arcane magic",
      P_CONJURATION_SPELL,		2, 15,			20,10,300, A_AVG_INT_WIS_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_SILVER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("time stop",		   "ancient", "some long-forgotten arcane magic",
	P_TRANSMUTATION_SPELL,		2, 10,			12, 9,200, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 4, 3, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("mage armor",		   "ornamental", None,
	  P_ABJURATION_SPELL,	   10,  1,		   360, 1, 10, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 4, 20, 200, CLR_BLACK, O1_NONE),
SPELL("bless",			   "spiritual", None,
	P_CLERIC_SPELL,			   10, 1,			 4, 3, 20, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLACK, O1_NONE),
SPELL("curse",			   "blasphemous", None,
	  P_CLERIC_SPELL,	       10,  1,			 4, 3, 20, A_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, CLR_BLACK, O1_NONE),
SPELL("enchant armor",		  "glowing", None,
	  P_ENCHANTMENT_SPELL,		10, 3,			 4, 3,20, A_AVG_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("enchant weapon",		  "dark gray", None,
	  P_ENCHANTMENT_SPELL,		10, 3,			 5, 4,30, A_AVG_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("protect armor",		  "glowing", None,
	  P_ENCHANTMENT_SPELL,		10, 2,			 2, 1,10, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("protect weapon",		  "dark gray", None,
	  P_ENCHANTMENT_SPELL,		10, 2,			 3, 2,15, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("resurrection",		  "bright-colored", None,
	P_CLERIC_SPELL,				10, 5,			10, 5,50, A_WIS, 6, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, CLR_WHITE, O1_NONE),
SPELL("negate undeath",		  "inspiring", None,
	P_CLERIC_SPELL,				5, 4,			 8, 7,100, A_WIS, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("banish demon",		  "squamous", None,
	P_CLERIC_SPELL,				5, 4,			 9, 8,150, A_WIS, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("anti-magic shell",   "immaculate", None,
	  P_ABJURATION_SPELL,	    5,  1,		   120, 7,100, A_INT, 0, 0, 1, NODIR, ANTIMAGIC, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("reflection", "polished silver", None,
	  P_ABJURATION_SPELL,	    5,  1,		   120, 6, 75, A_MAX_INT_WIS, 0, 0, 1, NODIR, REFLECTING, 0, 0, 0, 10, 6, 25, HI_SILVER, O1_NONE),
SPELL("protection from fire", "smoky", None,
	  P_ABJURATION_SPELL,	    5,  1,		   120, 2, 15, A_MAX_INT_WIS, 0, 0, 1, NODIR, FIRE_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_FIRE_RESISTANT),
SPELL("protection from lightning", "cloudy", None,
	  P_ABJURATION_SPELL,	    5,  1,		   120, 2, 15, A_MAX_INT_WIS, 0, 0, 1, NODIR, SHOCK_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_LIGHTNING_RESISTANT),
SPELL("protection from cold", "blue and white", None,
	  P_ABJURATION_SPELL,	    5,  1,		   120, 2, 15, A_MAX_INT_WIS, 0, 0, 1, NODIR, COLD_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_COLD_RESISTANT),
SPELL("protection from acid", "incomplete", None,
	  P_ABJURATION_SPELL,	    5,  1,		   120, 1, 10, A_WIS, 0, 0, 1, NODIR, ACID_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from poison", "intimidating-looking", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 1, 10, A_MAX_INT_WIS, 0, 0, 1, NODIR, POISON_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from life draining", "adorned", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 0, 7, A_WIS, 0, 0, 1, NODIR, DRAIN_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from death magic", "decorated", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 3, 20, A_WIS, 0, 0, 1, NODIR, DEATH_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from disintegration", "aberrant", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 4, 30, A_INT, 0, 0, 1, NODIR, DISINT_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_DISINTEGRATION_RESISTANT),
SPELL("protection from sickness", "baroque", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 4, 30, A_WIS, 0, 0, 1, NODIR, SICK_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from petrification", "aged", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 5, 50, A_MAX_INT_WIS, 0, 0, 1, NODIR, STONE_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("globe of invulnerability", "striped", "some long-forgotten arcane magic",
	P_ABJURATION_SPELL,			5, 1,			60, 8,150, A_WIS, 0, 0, 1, NODIR, INVULNERABLE, 0, 0, 0, 1, 6, 4, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY | O1_INDESTRUCTIBLE),
SPELL("divine intervention", "square-patterned", "some aeon-old prayer",
	P_ABJURATION_SPELL,			5, 1,		   120, 9,200, A_WIS, 0, 0, 1, NODIR, LIFESAVED, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("protection from lycanthropy", "strange-looking", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 0, 7, A_WIS, 0, 0, 1, NODIR, LYCANTHROPY_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("protection from curses", "cross-patterned", None,
	P_ABJURATION_SPELL,			5, 1,		   120, 3,20, A_WIS, 0, 0, 1, NODIR, CURSE_RES, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("water breathing",		"navy blue", None,
	P_TRANSMUTATION_SPELL,		5, 1,			 0, 3,20, A_INT, 0, 0, 1, NODIR, MAGICAL_BREATHING, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("water walking",			"circle-patterned", None,
	P_MOVEMENT_SPELL,			5, 1,			 0, 1,10, A_MAX_INT_WIS, 0, 0, 1, NODIR, WWALKING, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("lower magic resistance", "dim", None,
	P_ABJURATION_SPELL,			5, 1,			 5, 4, 30, A_MAX_INT_WIS, 12, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_SPELL_BYPASSES_MAGIC_RESISTANCE),
SPELL("negate magic resistance", "dark", None,
	P_ABJURATION_SPELL,			5, 1,			 9, 8, 150, A_AVG_INT_WIS, 10, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_SPELL_BYPASSES_MAGIC_RESISTANCE),
SPELL("forbid summoning",		"oval-patterned", None,
	P_ABJURATION_SPELL,			5, 1,			 4, 3, 20, A_MAX_INT_WIS, 8, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, HI_PAPER, O1_NONE),
SPELL("cold-enchant item",		"baleful", None,
	P_ENCHANTMENT_SPELL,		5, 1,			 0, 1, 10, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_COLD_RESISTANT),
SPELL("fire-enchant item",		"deformed", None,
	P_ENCHANTMENT_SPELL,		5, 1,			 0, 2, 15, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_FIRE_RESISTANT),
SPELL("lightning-enchant item",	"degraded", None,
	P_ENCHANTMENT_SPELL,		5, 1,			 0, 3, 20, A_INT, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_LIGHTNING_RESISTANT),
SPELL("death-enchant item",		"diabolical", None,
	P_ENCHANTMENT_SPELL,		5, 3,			 0, 8,150, A_AVG_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("telepathy",				"unnameable", None,
	P_DIVINATION_SPELL,			5, 3,			 0, 3, 20, A_MAX_INT_WIS, 0, 0, 1, NODIR, TELEPAT, 0, 0, 0, 6, 10, 150, HI_PAPER, O1_NONE),
SPELL("detect traps",			"incongruous", None,
	P_DIVINATION_SPELL,			5, 3,			 0, 6, 75, A_MAX_INT_WIS, 0, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("mirror image",			"nonsensical", None,
	P_ENCHANTMENT_SPELL,		5, 3,			 0, 1, 10, A_MAX_INT_WIS, 0, 0, 1, NODIR, DISPLACED, 0, 0, 0, 6, 10, 180, HI_PAPER, O1_NONE),
SPELL("mass conflict",			"odious", None,
	P_ENCHANTMENT_SPELL,		5, 3,			 0, 9,200, A_CHA, 0, 0, 1, NODIR, CONFLICT, 0, 0, 0, 3, 10, 50, HI_PAPER, O1_NONE),
SPELL("stinking cloud",			"peculiar", None,
	P_CONJURATION_SPELL,		5, 3,			 0, 2, 15, A_INT, 0, 0, 1, TARGETED, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("great Yendorian summoning", "hideous", "some long-forgotten arcane magic",
	P_CONJURATION_SPELL,		5,  5,		   150,10,300, A_AVG_INT_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 2, 10, 140, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),
SPELL("call ghoul",				"repulsive", None,
	P_NECROMANCY_SPELL,			5,  5,		    75, 7, 75, A_AVG_INT_CHA, 0, 0, 1, NODIR, 0, 0, 0, 0, 1, 10, 70, HI_PAPER, O1_NONE),
SPELL("raise zombie",			"unspeakable", None,
	P_NECROMANCY_SPELL,			5,  5,		    50, 4, 30, A_AVG_INT_WIS, 0, 8, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("create mummy",			"terrifying", None,
	P_NECROMANCY_SPELL,			5,  5,		    50, 6, 75, A_AVG_INT_WIS, 0, 8, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
SPELL("create dracolich",		"sacrilegious", "ancient Stygian magic",
	P_NECROMANCY_SPELL,			5,  7,		   200, 11,500, A_AVG_INT_WIS, 0, 1, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY),

SPELL("lower water",			"obnoxious", None,
      P_TRANSMUTATION_SPELL,	5,  3,			 0, 5, 50, A_INT, 10, 0, 1, RAY, RAY_EVAPORATION, 6, 6, 0, 0, 0, 0, CLR_RED, O1_NONE),


/* books with fixed descriptions
 */
SPELL("blank paper", "plain", None, P_NONE, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HI_PAPER, O1_NONE),
/* tribute book for 3.6 */
OBJECT(OBJ("novel", "paperback"), None,
       BITS(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, PAPER),
       0, 0, 0, SPBOOK_CLASS, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 20, CLR_BRIGHT_BLUE, 0, 0, 0, 0, O1_NON_SPELL_SPELLBOOK),
/* a special, one of a kind, spellbook */
OBJECT(OBJ("Book of the Dead", "papyrus"), None,
       BITS(0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, P_NONE, PAPER),
       0, 0, 0, SPBOOK_CLASS, 0, 0, 20, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 20, HI_PAPER, 0, 0, 0, 0, O1_INDESTRUCTIBLE | O1_NON_SPELL_SPELLBOOK),
#undef SPELL

/* wands ... */
#define WAND(name,typ,prob,cost,mgc,dir,dirsubtype,range,radius,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,metal,color,flags) \
    OBJECT(OBJ(name, typ),  None,                                             \
           BITS(0, 0, 1, 0, mgc, 1, 0, 0, 0, 0, dir, P_NONE, metal),    \
           0, 0, 0, WAND_CLASS, prob, 0, 6, cost, sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, 0, 0, 0, 0, range, radius, 30, color, dirsubtype, 0, 0, 0, flags)
WAND("light",           "glass", 95, 100, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, GLASS, HI_GLASS, O1_NONE),
WAND("secret door detection",
                        "balsa", 50, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("enlightenment", "crystal", 15, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, GLASS, HI_GLASS, O1_NONE),
WAND("create monster",  "maple", 45, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("wishing",          "pine",  5, 500, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_LIGHTNING_RESISTANT),
WAND("nothing",           "oak", 25, 100, 0, IMMEDIATE, 0,  0, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("striking",        "ebony", 70, 150, 1, IMMEDIATE, 0, 10, 0, 2, 12, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND("make invisible", "marble", 45, 150, 1, IMMEDIATE, 0,  8, 0, 0, 0, 0, 0, 0, 0, MINERAL, HI_MINERAL, O1_NONE),
WAND("slow monster",      "tin", 50, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("speed monster",   "brass", 50, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
WAND("undead turning", "copper", 50, 150, 1, IMMEDIATE, 0,  8, 0, 0, 0, 0, 0, 0, 0, COPPER, HI_COPPER, O1_NONE),
WAND("polymorph",      "silver", 45, 200, 1, IMMEDIATE, 0,  6, 0, 0, 0, 0, 0, 0, 0, SILVER, HI_SILVER, O1_POLYMORPH_RESISTANT),
WAND("cancellation", "platinum", 45, 200, 1, IMMEDIATE, 0, 10, 0, 0, 0, 0, 1, 20, 30, PLATINUM, CLR_WHITE, O1_SPELL_BYPASSES_MAGIC_RESISTANCE | O1_LIGHTNING_RESISTANT | O1_POLYMORPH_RESISTANT | O1_DISINTEGRATION_RESISTANT),
WAND("teleportation", "iridium", 45, 200, 1, IMMEDIATE, 0, 10, 0, 0, 0, 0, 0, 0, 0, METAL, CLR_BRIGHT_CYAN, O1_NONE),
WAND("opening",          "zinc", 25, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("locking",      "aluminum", 25, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("probing",       "uranium", 30, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, METAL, HI_METAL, O1_NONE),
WAND("digging",          "iron", 50, 150, 1, RAY, RAY_WND_DIGGING,			10, 0, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND("magic missile",   "steel", 50, 150, 1, RAY, RAY_WND_MAGIC_MISSILE,	12, 0, 3, 4, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND("fire",        "hexagonal", 40, 175, 1, RAY, RAY_WND_FIRE,				 8, 0, 8, 6, 0, 0, 0, 0, IRON, HI_METAL, O1_FIRE_RESISTANT),
WAND("cold",            "short", 40, 175, 1, RAY, RAY_WND_COLD,				12, 0, 12, 6, 0, 0, 0, 0, IRON, HI_METAL, O1_COLD_RESISTANT),
WAND("sleep",		    "runed", 45, 175, 1, RAY, RAY_WND_SLEEP,			10, 0, 0, 0, 0, 4, 6, 0, IRON, HI_METAL, O1_NONE),
WAND("disintegration",   "long",  5, 500, 1, RAY, RAY_WND_DISINTEGRATION,	 8, 0, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_DISINTEGRATION_RESISTANT),
WAND("lightning",      "curved", 40, 175, 1, RAY, RAY_WND_LIGHTNING,		14, 0, 6, 6, 0, 0, 0, 0, IRON, HI_METAL, O1_LIGHTNING_RESISTANT),
WAND("death",	 "skull-headed",  5, 500, 1, RAY, RAY_WND_DEATH,			 8, 0, 0, 0, 0, 0, 0, 0, BONE, CLR_WHITE, O1_NONE),
WAND("resurrection", "ornamental",5, 200, 1, IMMEDIATE, 0,					 6, 0, 0, 0, 0, 0, 0, 0, IRON, CLR_MAGENTA, O1_NONE),
WAND("evaporation",    "flimsy",  5, 150, 1, RAY, RAY_WND_EVAPORATION,		10, 0, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
/* extra descriptions, shuffled into use at start of new game */
WAND(None,             "forked",  0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WOOD, HI_WOOD, O1_NONE),
WAND(None,             "spiked",  0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, IRON, HI_METAL, O1_NONE),
WAND(None,            "jeweled",  0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, IRON, HI_MINERAL, O1_NONE),
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal,worth) \
    OBJECT(OBJ(name, None),  None,                                       \
           BITS(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, metal),    \
           0, 0, 0, COIN_CLASS, prob, 0, 1, worth,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HI_GOLD, 0, 0, 0, 0, O1_NONE)
COIN("gold piece", 1000, GOLD, 1),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color,flags) \
    OBJECT(OBJ(name, desc), None,                                             \
           BITS(0, 1, 0, 0, 0, 0, 0, 0, 0,                              \
                HARDGEM(mohs), 0, -P_SLING, glass),                     \
           0, 0, 0, GEM_CLASS, prob, 0, 1, gval, 1, 3, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, nutr, color, 0, 0, 0, 0, flags)
#define ROCK(name,desc,power1,power2,power3,kn,prob,wt,gval, sdam, ldam, mgc,nutr,mohs,glass,color,flags) \
    OBJECT(OBJ(name, desc),  None,                                            \
           BITS(kn, 1, 0, 0, mgc, 0, 0, 0, 0,                           \
                HARDGEM(mohs), 0, -P_SLING, glass),                     \
           power1, power2, power3, GEM_CLASS, prob, 0, wt, gval, 1, sdam, 0, 1, ldam, 0, 0, 0, 0, 0, 0, 0, nutr, color, 0, 0, 0, 0, flags)
GEM("dilithium crystal", "white",  2, 1, 4500, 15,  5, GEMSTONE, CLR_WHITE, O1_NONE),
GEM("diamond",           "white",  3, 1, 4000, 15,  10, GEMSTONE, CLR_WHITE, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("black pearl",		 "black",  3, 1, 3750, 15,  5, GEMSTONE, CLR_BLACK, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("ruby",                "red",  4, 1, 3500, 15,  9, GEMSTONE, CLR_RED, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("jacinth",          "orange",  3, 1, 3250, 15,  9, GEMSTONE, CLR_ORANGE, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("sapphire",           "blue",  4, 1, 3000, 15,  9, GEMSTONE, CLR_BLUE, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("black opal",        "black",  3, 1, 2750, 15,  8, GEMSTONE, CLR_BLACK, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("emerald",           "green",  5, 1, 2500, 15,  8, GEMSTONE, CLR_GREEN, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("turquoise",         "green",  6, 1, 2000, 15,  6, GEMSTONE, CLR_GREEN, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("citrine",          "yellow",  4, 1, 1500, 15,  6, GEMSTONE, CLR_YELLOW, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("aquamarine",        "green",  6, 1, 1500, 15,  8, GEMSTONE, CLR_GREEN, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("amber",   "yellowish brown",  8, 1, 1000, 15,  2, GEMSTONE, CLR_BROWN, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("topaz",   "yellowish brown", 10, 1,  900, 15,  8, GEMSTONE, CLR_BROWN, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("jet",               "black", 10, 1,  850, 15,  7, GEMSTONE, CLR_BLACK, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("opal",              "white", 12, 1,  800, 15,  6, GEMSTONE, CLR_WHITE, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("chrysoberyl",      "yellow", 10, 1,  700, 15,  5, GEMSTONE, CLR_YELLOW, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("garnet",              "red", 12, 1,  700, 15,  7, GEMSTONE, CLR_RED, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("amethyst",         "violet", 12, 1,  600, 15,  7, GEMSTONE, CLR_MAGENTA, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("jasper",              "red", 12, 1,  500, 15,  7, GEMSTONE, CLR_RED, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("fluorite",         "violet", 12, 1,  400, 15,  4, GEMSTONE, CLR_MAGENTA, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("pearl",			 "white", 12, 1,  300, 15,  5, GEMSTONE, CLR_WHITE, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("obsidian",          "black", 10, 1,  200, 15,  6, GEMSTONE, CLR_BLACK, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("agate",            "orange", 12, 1,  200, 15,  6, GEMSTONE, CLR_ORANGE, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("jade",              "green", 10, 1,  300, 15,  6, GEMSTONE, CLR_GREEN, O1_TREATED_AS_MATERIAL_COMPONENT),
GEM("worthless piece of white glass", "white",
    70, 1, 0, 6, 5, GLASS, CLR_WHITE, O1_NONE),
GEM("worthless piece of blue glass", "blue",
    70, 1, 0, 6, 5, GLASS, CLR_BLUE, O1_NONE),
GEM("worthless piece of red glass", "red",
    77, 1, 0, 6, 5, GLASS, CLR_RED, O1_NONE),
GEM("worthless piece of yellowish brown glass", "yellowish brown",
    77, 1, 0, 6, 5, GLASS, CLR_BROWN, O1_NONE),
GEM("worthless piece of orange glass", "orange",
    77, 1, 0, 6, 5, GLASS, CLR_ORANGE, O1_NONE),
GEM("worthless piece of yellow glass", "yellow",
    77, 1, 0, 6, 5, GLASS, CLR_YELLOW, O1_NONE),
GEM("worthless piece of black glass", "black",
    77, 1, 0, 6, 5, GLASS, CLR_BLACK, O1_NONE),
GEM("worthless piece of green glass", "green",
    76, 1, 0, 6, 5, GLASS, CLR_GREEN, O1_NONE),
GEM("worthless piece of violet glass", "violet",
    76, 1, 0, 6, 5, GLASS, CLR_MAGENTA, O1_NONE),

/* Placement note: there is a wishable subrange for
 * "gray stones" in the o_ranges[] array in objnam.c
 * that is currently everything between luckstones and flint
 * (inclusive).
 */
ROCK("luckstone", "gray",	//Gray stone start marker
	0, 0, 0,				0,  10,  10, 60, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY, O1_CONFERS_LUCK | O1_CONFERS_POWERS_WHEN_CARRIED),
ROCK("loadstone", "gray", 
	0, 0, 0,				0,  10, 500,  1, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY, O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED | O1_CANNOT_BE_DROPPED_IF_CURSED),
ROCK("jinxstone", "gray",  
	FUMBLING, 0, 0,			0,  10,  10,  1, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY, O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED | O1_CANNOT_BE_DROPPED_IF_CURSED | O1_CONFERS_POWERS_WHEN_CARRIED),
ROCK("touchstone", "gray", 
	0, 0, 0,				0,   8,  10, 45, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY, O1_NONE),
ROCK("flint", "gray",      //Gray stone end marker 
	0, 0, 0,				0,  10,  10,  1, 6, 6, 0, 10, 7, MINERAL, CLR_GRAY, O1_NONE),
ROCK("rock", None,         
	0, 0, 0,				1,  90,  10,  0, 3, 3, 0, 10, 7, MINERAL, CLR_GRAY, O1_NONE),
#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
OBJECT(OBJ("boulder", None), None,
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, P_NONE, MINERAL), 0, 0, 0,
       ROCK_CLASS, 100, 0, 8000, 0, 1, 20, 0, 1, 20, 0, 0, 0, 0, 0, 0, 0, 2000, HI_MINERAL, 0, 0, 0, 0, O1_NONE),
OBJECT(OBJ("statue", None), None,
       BITS(1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, P_NONE, MINERAL), 0, 0, 0,
       ROCK_CLASS, 900, 0, 4000, 0, 1, 20, 0, 1, 20, 0, 0, 0, 0, 0, 0, 0, 2500, CLR_WHITE, 0, 0, 0, 0, O1_NONE),
OBJECT(OBJ("heavy iron ball", None), None,
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0, 0, 0,
       BALL_CLASS, 1000, 0, 800, 10, 1, 25, 0, 1, 25, 0, 0, 0, 0, 0, 0, 0, 200, HI_METAL, 0, 0, 0, 0, O1_NONE),
        /* +d4 when "very heavy" */
OBJECT(OBJ("iron chain", None), None,
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, WHACK, P_NONE, IRON), 0, 0, 0,
       CHAIN_CLASS, 1000, 0, 240, 0, 1, 4, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 200, HI_METAL, 0, 0, 0, 0, O1_NONE),
        /* +1 both l & s */

/* Venom is normally a transitory missile (spit by various creatures)
 * but can be wished for in wizard mode so could occur in bones data.
 */
OBJECT(OBJ("blinding venom", "splash of venom"), None,
       BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0, 0, 0,
       VENOM_CLASS, 500, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HI_ORGANIC, 0, 0, 0, 0, O1_NONE),
OBJECT(OBJ("acid venom", "splash of venom"), None,
       BITS(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, P_NONE, LIQUID), 0, 0, 0,
       VENOM_CLASS, 500, 0, 1, 0, 2, 6, 0, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, HI_ORGANIC, 0, 0, 0, 0, O1_NONE),
        /* +d6 small or large */

/* Reagents here, which do not belong to any class, includes also all other odd non-food objects with no other apparent purpose*/
#define REAGENT(name, prob, eatdelay, wt, cost, material, ediblesubtype, nutrition, color, flags)         \
    OBJECT(OBJ(name, None), None,                                      \
           BITS(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, material), 0, 0, 0,     \
           REAGENT_CLASS, prob, eatdelay, wt, cost,  0, 0, 0, 0, 0, 0, ediblesubtype, 0, 0, 0, 0, 0, \
           nutrition, color, 0, 0, 0, 0, flags)

	REAGENT("thread of spider silk",	125, 0, 1, 10, SILK, 0,					 2, CLR_GRAY, O1_TREATED_AS_MATERIAL_COMPONENT), /* STARTMARKER 1&2 */

	REAGENT("clump of bat guano",		125, 2, 5, 10, ORGANIC, EDIBLE_ROTTEN,	20, CLR_BROWN, O1_EDIBLE_NONFOOD | O1_TREATED_AS_MATERIAL_COMPONENT),
	REAGENT("ginseng root",				125, 1, 3, 10, VEGGY, EDIBLE_NORMAL,	40, CLR_BROWN, O1_EDIBLE_NONFOOD | O1_TREATED_AS_MATERIAL_COMPONENT),
	REAGENT("mandrake root",			125, 1, 3, 10, VEGGY, EDIBLE_NORMAL,	40, CLR_MAGENTA, O1_EDIBLE_NONFOOD | O1_TREATED_AS_MATERIAL_COMPONENT),
	REAGENT("fungal spore",				125, 1, 1, 10, ORGANIC, EDIBLE_NORMAL,	 1, CLR_GRAY, O1_EDIBLE_NONFOOD | O1_TREATED_AS_MATERIAL_COMPONENT),
	REAGENT("sprig of wolfsbane",		125, 1, 2, 10, VEGGY, EDIBLE_POISONOUS,	40, CLR_GREEN, O1_EDIBLE_NONFOOD | O1_TREATED_AS_MATERIAL_COMPONENT),
	REAGENT("clove of garlic",			125, 1, 2, 10, VEGGY, EDIBLE_NORMAL,	40, CLR_WHITE, O1_EDIBLE_NONFOOD | O1_TREATED_AS_MATERIAL_COMPONENT),

	REAGENT("raven feather",			125, 0, 1, 10, ORGANIC, 0,				10, CLR_BLACK, O1_TREATED_AS_MATERIAL_COMPONENT), /* STARTMARKER 1&2 */

#undef REAGENT

/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
OBJECT(OBJ(None, None), None,
       BITS(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0), 0, 0, 0,
       ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, O1_NONE)
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

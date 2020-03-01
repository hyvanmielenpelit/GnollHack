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
#include "monattk.h"
#include "monflag.h"
#include "monsym.h"

#else /* !OBJECTS_PASS_2_ */
/* second pass */
#include "color.h"
//#define COLOR_FIELD(X) X,
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
#define OBJECT(obj,contentdesc,shortdesc,bits,prp1,prp2,prp3,pflags,sym,prob,multigen,dly,wt,cost, \
               dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, edmgtype,edice,edam,edmgplus,aflags,critpct,  hitbon,mcadj,fixdmgbon,range,  oc1,oc2,oc3,oc4,oc5,oc6,oc7,oc8,  nut,color,  dirsubtype,materials,cooldown,level,  powconfermask,permittedtargets,flags,flags2,flags3)  { obj }
#define None (char *) 0 /* less visual distraction for 'no description' */

NEARDATA struct objdescr obj_descr[] =
#else
/* second pass -- object definitions */
#define BITS(nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,skill,mtrl) \
  nmkn,mrg,uskn,0,mgc,chrg,uniq,nwsh,big,tuf,dir,mtrl,sub,skill /*SCO cpp fodder*/
#define OBJECT(obj,contentdesc,shortdesc,bits,prp1,prp2,prp3,pflags,sym,prob,multigen,dly,wt,cost,dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct,   hitbon,mcadj,fixdmgbon,range,  oc1,oc2,oc3,oc4,oc5,oc6,oc7,oc8,  nut,color,  dirsubtype,materials,cooldown,level,  powconfermask,permittedtargets,flags,flags2,flags3) \
  { 0, 0, (char *) 0, contentdesc, shortdesc, bits, prp1, prp2, prp3, pflags, sym, dly, color, prob, wt, nut,  \
    cost, dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, hitbon, mcadj, fixdmgbon, range,   oc1, oc2, oc3, oc4, oc5, oc6, oc7, oc8,   dirsubtype, materials, cooldown, level,  flags,flags2,flags3,powconfermask, permittedtargets, critpct, multigen }
#ifndef lint
#define HARDGEM(n) (n >= 8)
#else
#define HARDGEM(n) (0)
#endif

NEARDATA struct objclass objects[] =
#endif
{
	/* dummy object[0] -- description [2nd arg] *must* be NULL */
	OBJECT(OBJ("strange object", None), None, None, \
		   BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0), \
		   NO_POWER, NO_POWER, NO_POWER, P1_NONE, ILLOBJ_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   0, 0, \
		   0, 0, 0, 0, \
		   PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),

	/* weapons ... */
	#define WEAPON(name,desc,nmkn,mrg,mgc,bi,prob,multigen,wt,cost,\
				dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct, \
				hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
				power,power2,power3,pflags,typ,sub,skill,metal,color,\
				flags,flags2,flags3,powconfermask,permittedtargets) \
	    OBJECT(OBJ(name,desc), None, None,                                         \
	           BITS(nmkn, mrg, 1, 0, mgc, CHARGED_GENERAL, 0, 0, bi, 0, typ, sub, skill, metal),  \
	           power, power2, power3, pflags, WEAPON_CLASS, prob, multigen, 0, wt, cost, \
			   dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, critpct, \
			   hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
			   wt, color, \
			   0, 0, 0, 0, \
			   powconfermask,permittedtargets, flags, flags2, flags3)


	#define PROJECTILE(name,desc,  nmkn,mgc,prob,multigen,wt,cost,  dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct,  hitbon,mcadj,  metal,sub,launcher_skill,skill,color,  flags,flags2,flags3, permittedtargets) \
	    OBJECT(OBJ(name,desc), None, None,   \
	           BITS(nmkn, 1, 1, 0, mgc, CHARGED_GENERAL, 0, 0, 0, 0, PIERCE, sub, skill, metal), \
	           NO_POWER, NO_POWER, NO_POWER, P1_NONE, WEAPON_CLASS, prob, multigen, 0, wt, cost, \
			   dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, critpct, \
			   hitbon, mcadj, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
			   wt, color, \
			   launcher_skill, 0, 0, 0, \
			   PERMITTED_ALL, permittedtargets, flags, flags2, flags3)


	#define BOW(name,desc,nmkn,mgc,bi,prob,multigen,wt,cost,dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct,hitbon,mcadj,fixeddmgbon,launchrange,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,power,power2,power3,pflags,metal,sub,skill,color,flags,flags2,flags3,powconfermask,permittedtargets) \
	    OBJECT(OBJ(name,desc), None, None,                                         \
	           BITS(nmkn, 0, 1, 0, mgc, CHARGED_GENERAL, 0, 0, bi, 0, 0, sub, skill, metal),      \
	           power, power2, power3, pflags, WEAPON_CLASS, prob, multigen, 0, wt, cost,                            \
	           dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, critpct, \
			   hitbon, mcadj, fixeddmgbon, launchrange, 0, 0, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount,  \
			   color, wt, \
			   0, 0, 0, 0, \
			   powconfermask, permittedtargets, flags, flags2, flags3)

	/* Note: for weapons that don't do an even die of damage (ex. 2-7 or 3-18)
	   the extra damage is added on in weapon.c, not here! */

	#define P PIERCE
	#define S SLASH
	#define B WHACK

/* missiles; materiel reflects the arrowhead, not the shaft */
PROJECTILE("arrow", None,
	1, 0, 38, MULTIGEN_2D6_3, 1, 2,
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, MAT_IRON, WEP_ARROW, P_BOW, -P_BOW, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("elven arrow", "runed arrow",
	0, 0, 20, MULTIGEN_2D6_3, 1, 2, 
	AD_PHYS, 1, 7, 0, 1, 6, 0,	 AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_WOOD, WEP_ARROW, P_BOW, -P_BOW, HI_WOOD,
	O1_NONE, O2_ELVEN_ITEM, O3_NONE, ALL_TARGETS),
PROJECTILE("orcish arrow", "crude arrow",
	0, 0, 20, MULTIGEN_2D6_3, 1, 2, 
	AD_PHYS, 1, 5, 0, 1, 6, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_IRON, WEP_ARROW, P_BOW, -P_BOW, CLR_BLACK,
	O1_NONE, O2_ORCISH_ITEM, O3_NONE, ALL_TARGETS),
PROJECTILE("silver arrow", None,
	1, 0, 8, MULTIGEN_1D6_2, 1, 5,  
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_SILVER, WEP_ARROW, P_BOW, -P_BOW, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("ya", "bamboo arrow",
	0, 0, 11, MULTIGEN_2D6_3, 1, 4, 
	AD_PHYS, 1, 7, 0, 1, 7, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	1, 0, MAT_METAL, WEP_ARROW, P_BOW, -P_BOW, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("bone arrow", None,
	1, 0, 10, MULTIGEN_1D6_3, 1, 6, 
	AD_PHYS, 1, 4, 0, 1, 5, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_BONE, WEP_ARROW, P_BOW, -P_BOW, CLR_WHITE,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("crossbow bolt", None,
	1, 0, 35, MULTIGEN_2D6_3, 1, 2, 
	AD_PHYS, 1, 3, 1, 1, 4, 1, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_IRON, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS), //1d3+1 //1d4+1
PROJECTILE("gnollish quarrel", "crude crossbow bolt",
	0, 0, 15, MULTIGEN_2D6_3, 1, 2, 
	AD_PHYS, 1, 2, 1, 1, 3, 1, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_IRON, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, HI_METAL,
	O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, ALL_TARGETS), //1d2+1 //1d3+1
PROJECTILE("silver crossbow bolt", None,
	1, 0, 8, MULTIGEN_1D6_2, 1, 2, 
	AD_PHYS, 1, 3, 1, 1, 4, 1, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_SILVER, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS), //1d3+1 //1d4+1
PROJECTILE("bone quarrel", None,
	1, 0, 10, MULTIGEN_1D6_3, 1, 2, 
	AD_PHYS, 1, 2, 0, 1, 3, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_BONE, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, CLR_WHITE,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS), //1d2 //1d3
PROJECTILE("leaden sling-bullet", None,
	1, 0, 10, MULTIGEN_1D6_2, 2, 2, 
	AD_PHYS, 1, 6, 0, 1, 8, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_METAL, WEP_SLING_BULLET, P_SLING, -P_SLING, CLR_GRAY,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("iron sling-bullet", None,
	1, 0, 5, MULTIGEN_1D6_2, 2, 2, 
	AD_PHYS, 1, 8, 0, 1, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_IRON, WEP_SLING_BULLET, P_SLING, -P_SLING, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("silver sling-bullet", None,
	1, 0, 5, MULTIGEN_1D6_2, 2, 2, 
	AD_PHYS, 1, 7, 0, 1, 9, 0, AD_PHYS,	0, 0, 0, A1_NONE, 0, 
	0, 0, MAT_SILVER, WEP_SLING_BULLET, P_SLING, -P_SLING, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, ALL_TARGETS),
PROJECTILE("arrow of dragon slaying", "black arrow",
	0, 1, 2, MULTIGEN_SINGLE, 1, 100,
	AD_PHYS, 1, 6, 0,  1, 6, 0,  AD_PHYS, 0, 0, 0, 
	A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_ITEM_VANISHES_ON_HIT | A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET, 100,
	0, -12, MAT_METAL, WEP_ARROW, P_BOW, -P_BOW, CLR_BLACK,
	O1_SPE_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NONE, S_DRAGON),

/* missiles that don't use a launcher */
WEAPON("dart", None,
    1, 1, 0, 0, 50, MULTIGEN_1D10, 1, 2,
	AD_PHYS,	1, 2, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_2_EXPERT_1D2_SKILLED,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, P, WEP_DART, -P_THROWN_WEAPON, MAT_IRON, HI_METAL,
	O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("shuriken", "throwing star",
    0, 1, 0, 0, 25, MULTIGEN_1D8, 1, 6,
	AD_PHYS, 1, 5, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_2_EXPERT_1D2_SKILLED,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, P, WEP_SHURIKEN, -P_THROWN_WEAPON, MAT_IRON, HI_METAL,
	O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("boomerang", None,
    1, 1, 0, 0, 15, MULTIGEN_SINGLE, 5, 20,
	AD_PHYS, 1, 9, 0, 1, 9, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, WEP_BOOMERANG, -P_THROWN_WEAPON, MAT_WOOD, HI_WOOD,
	O1_THROWN_WEAPON_ONLY | O1_RETURNS_TO_HAND_AFTER_THROWING, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),

/* spears [note: javelin used to have a separate skill from spears,
   because the latter are primarily stabbing weapons rather than
   throwing ones; but for playability, they've been merged together
   under spear skill and spears can now be thrown like javelins] */
/* Note from JG: Javelins are now marked as thrown weapons while spears are not, but spears can still be thrown relatively far */
WEAPON("spear", None,
	1, 1, 0, 0, 45, MULTIGEN_SINGLE, 30,   3,	
	AD_PHYS,	1, 6, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -60, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven spear", "runed spear",
	0, 1, 0, 0, 10, MULTIGEN_SINGLE, 30,   3,	
	AD_PHYS,	1, 7, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_WOOD, HI_WOOD,
	O1_NONE, O2_ELVEN_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("orcish spear", "crude spear",
	0, 1, 0, 0, 13, MULTIGEN_SINGLE, 30,   3,	
	AD_PHYS,	1, 5, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -55, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_IRON, CLR_BLACK,
	O1_NONE, O2_ORCISH_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish spear", "stout spear",
	0, 1, 0, 0, 12, MULTIGEN_SINGLE, 35,   3, 
	AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_IRON, HI_METAL,
	O1_NONE, O2_DWARVEN_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver spear", None,
	1, 1, 0, 0,  2, MULTIGEN_SINGLE, 36,  40, 
	AD_PHYS, 1, 6, 0,  1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -60, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("javelin", "throwing spear",
	0, 0, 0, 0, 10, MULTIGEN_SINGLE, 20,   3, 
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -100, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_JAVELIN, P_SPEAR, MAT_IRON, HI_METAL,
	O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("javelin of returning", "throwing spear",
	0, 1, 1, 0, 5, MULTIGEN_SINGLE, 20,   3, 
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, -100, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_JAVELIN, P_SPEAR, MAT_IRON, HI_METAL,
	O1_THROWN_WEAPON_ONLY | O1_WEIGHT_DOES_NOT_REDUCE_RANGE | O1_RETURNS_TO_HAND_AFTER_THROWING, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),

/* spearish; doesn't stack, not intended to be thrown */
WEAPON("trident", None,
	1, 0, 0, 0,  6, MULTIGEN_SINGLE, 25,   5, 
	AD_PHYS, 1,  6, 1, 3,  4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_TRIDENT, P_SPEAR, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver fork", None,
	1, 0, 0, 0, 2, MULTIGEN_SINGLE, 5, 40, 
	AD_PHYS, 1, 2, 0, 0, 0, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_FORK, P_SPEAR, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),


/* blades; all stack */
WEAPON("dagger", None,
	1, 1, 0, 0, 14, MULTIGEN_SINGLE, 10,   4, 
	AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, HI_METAL,
	O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven dagger", "runed dagger",
	0, 1, 0, 0, 10, MULTIGEN_SINGLE, 10,   4, 
	AD_PHYS, 1, 5, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_WOOD, HI_WOOD,
	O1_MELEE_AND_THROWN_WEAPON, O2_ELVEN_ITEM, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven runedagger", "runed dagger", /* Base item for Sting */
	0, 1, 0, 0, 0, MULTIGEN_SINGLE, 10,   4, 
	AD_PHYS, 1, 5, 2, 1, 3, 2, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_WOOD, HI_WOOD,
	O1_MELEE_AND_THROWN_WEAPON, O2_ELVEN_ITEM, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("orcish dagger", "crude dagger",
	0, 1, 0, 0, 12, MULTIGEN_SINGLE, 10, 4, 
	AD_PHYS, 1, 3, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, CLR_BLACK,
	O1_MELEE_AND_THROWN_WEAPON, O2_ORCISH_ITEM, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("ornamental orcish dagger", "crudely ornated dagger", /* Base weapon for Grimtooth */
    0, 1, 0, 0, 0, MULTIGEN_SINGLE, 10, 4, 
	AD_PHYS, 3, 3, 0, 3, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, CLR_BLACK,
	O1_MELEE_AND_THROWN_WEAPON, O2_ORCISH_ITEM, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("bone dagger", None,
	1, 1, 0, 0, 6, MULTIGEN_SINGLE, 10,   4, 
	AD_PHYS, 1, 3, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_BONE, CLR_WHITE,
	O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver dagger", None,
	1, 1, 0, 0, 3, MULTIGEN_SINGLE, 12,  40, 
	AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_SILVER, HI_SILVER, 
	O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("Morgul blade", "black dagger",
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 10, 100,  
	AD_PHYS, 1, 4, 0, 1, 3, 0, AD_PHYS, 1, 3, 1, A1_WOUNDING, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, CLR_BLACK, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("main-gauche", None,
	1, 1, 0, 0, 4, MULTIGEN_SINGLE, 10,   4, 
	AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, HI_METAL, 
	O1_IS_ARMOR_WHEN_WIELDED, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("athame", None,
	1, 1, 0, 0, 0, MULTIGEN_SINGLE, 10,   4, 
	AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_KNIFE, P_DAGGER, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("scalpel", None,
	1, 1, 0, 0, 0, MULTIGEN_SINGLE, 5,   6, 
	AD_PHYS, 1, 3, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_KNIFE, P_DAGGER, MAT_METAL, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("knife", None,
	1, 1, 0, 0, 8, MULTIGEN_SINGLE, 5, 4, 
	AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P|S, WEP_KNIFE, P_DAGGER, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver knife", None,
	1, 1, 0, 0, 2, MULTIGEN_SINGLE, 5, 40, 
	AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P|S, WEP_KNIFE, P_DAGGER, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("stiletto", None,
	1, 1, 0, 0, 5, MULTIGEN_SINGLE, 5,   4, 
	AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P|S, WEP_KNIFE, P_DAGGER, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
/* 3.6: worm teeth and crysknives now stack;
   when a stack of teeth is enchanted at once, they fuse into one crysknife;
   when a stack of crysknives drops, the whole stack reverts to teeth */
WEAPON("worm tooth", None,
	1, 1, 0, 0, 0, MULTIGEN_SINGLE, 20,   2, 
	AD_PHYS, 1, 2, 0, 1,  2, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_WORM_TOOTH, P_DAGGER, 0, CLR_WHITE,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("crysknife", None,
	1, 1, 0, 0, 0, MULTIGEN_SINGLE, 20, 100, 
	AD_PHYS, 1, 10, 0, 1, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_WORM_TOOTH, P_DAGGER, MAT_MINERAL, CLR_WHITE,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),

/* axes */
WEAPON("axe", None,
	1, 0, 0, 0, 22, MULTIGEN_SINGLE, 50,   8, 
	AD_PHYS, 1, 6, 1, 1, 8, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_AXE, P_AXE, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("infernal axe", "black axe",
	0, 0, 0, 0, 0, MULTIGEN_SINGLE, 60, 200, 
	AD_PHYS, 2, 4, 1, 2, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_AXE, P_AXE, MAT_IRON, CLR_BLACK, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("throwing axe", None,
    1, 0, 0, 0, 10, MULTIGEN_SINGLE, 50,   8, 
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_THROWING_AXE, P_AXE, MAT_IRON, HI_METAL, 
	O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("battle-axe", "double-headed axe",       /* "double-bitted"? */
    0, 0, 0, 1, 8, MULTIGEN_SINGLE, 100,  40, 
	AD_PHYS, 2, 8, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_AXE, P_AXE, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("battle-axe of cleaving", "runed double-headed axe",    
    0, 0, 1, 1, 2, MULTIGEN_SINGLE, 100, 4000,
	AD_PHYS, 2, 8, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_BISECT, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_AXE, P_AXE, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),

/* swords */
WEAPON("short sword", None,
    1, 0, 0, 0,  8, MULTIGEN_SINGLE, 45,  10, 
	AD_PHYS, 1, 6, 0, 1,  8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven short sword", "runed short sword",
	0, 0, 0, 0,  2, MULTIGEN_SINGLE, 40,  10, 
	AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_WOOD, HI_WOOD,
	O1_NONE, O2_ELVEN_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("orcish short sword", "crude short sword",
	0, 0, 0, 0,  3, MULTIGEN_SINGLE, 50,  10, 
	AD_PHYS, 1, 5, 0, 1,  8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_IRON, CLR_BLACK,
	O1_NONE, O2_ORCISH_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish short sword", "broad short sword",
	0, 0, 0, 0, 2, MULTIGEN_SINGLE, 50, 10, 
	AD_PHYS, 1, 7, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_NONE, O2_DWARVEN_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("scimitar", "curved sword",
	0, 0, 0, 0, 10, MULTIGEN_SINGLE, 60, 15, 
	AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_SCIMITAR, P_SWORD, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver saber", None,
	1, 0, 0, 0, 6, MULTIGEN_SINGLE, 55, 75, 
	AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_SABER, P_SWORD, MAT_SILVER, HI_SILVER, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("saber of lycanthrope slaying", None, /* Base weapon for Werebane */
	1, 0, 0, 0, 2, MULTIGEN_SINGLE, 55, 500, 
	AD_PHYS, 1, 8, 1, 1, 8, 1, AD_PHYS, 2, 8, 0, A1_NONE, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	WARN_LYCANTHROPE, NO_POWER, NO_POWER, P1_NONE, S, WEP_SABER, P_SWORD, MAT_SILVER, HI_SILVER,
	O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG | O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS,
	PERMITTED_ALL, M2_WERE),
WEAPON("broadsword", None,
	1, 0, 0, 0, 8, MULTIGEN_SINGLE, 60, 10, 
	AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_BROADSWORD, P_SWORD, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven broadsword", "runed broadsword",
	0, 0, 0, 0, 4, MULTIGEN_SINGLE, 55, 10, 
	AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_BROADSWORD, P_SWORD, MAT_WOOD, HI_WOOD,
	O1_NONE, O2_ELVEN_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("long sword", None,
	1, 0, 0, 0, 24, MULTIGEN_SINGLE, 60, 15, 
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver long sword", None,
	1, 0, 0, 0, 1, MULTIGEN_SINGLE, 55, 500, 
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_LONG_SWORD, P_SWORD, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("sword of demon slaying", "shining long sword",
	0, 0, 1, 0, 2, MULTIGEN_SINGLE, 60, 3000,
	AD_PHYS, 1, 8, 1, 1, 12, 1, AD_PHYS, 2, 12, 0, A1_WOUNDING, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	WARN_DEMON, NO_POWER, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_SILVER, HI_SILVER,
	O1_FLICKERS_WHEN_MONSTERS_DETECTED,
	O2_FLICKER_COLOR_RED,
	O3_TARGET_PERMISSION_IS_M2_FLAG | O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS,
	PERMITTED_ALL, M2_DEMON),
WEAPON("sword of holy vengeance", "radiant long sword",
	0, 0, 1, 0, 2, MULTIGEN_SINGLE, 60, 5000,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, 0,
	0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
	WARN_UNDEAD, WARN_DEMON, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_SILVER, HI_SILVER,
	O1_NOT_CURSEABLE | O1_SPE_AFFECTS_MC | O1_FLICKERS_WHEN_MONSTERS_DETECTED,
	O2_ANGELIC_ITEM | O2_GENERATED_BLESSED | O2_SHINES_MAGICAL_LIGHT | O2_FLICKER_COLOR_WHITE,
	O3_TARGET_PERMISSION_IS_M2_FLAG | O3_PERMTTED_TARGET_CHAOTIC | O3_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS, 
	PERMITTED_ALIGNMENT_LAWFUL, M2_UNDEAD | M2_DEMON),
WEAPON("sword of unholy desecration", "black long sword",
	0, 0, 1, 0, 2, MULTIGEN_SINGLE, 60, 3000,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_LEVEL_DRAIN | A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, 0,
	0, -4, 0, 0, 5, 0, 0, 0, 0, 0, 0,
	WARN_ANGEL, NO_POWER, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_METAL, CLR_BLACK,
	O1_SPE_AFFECTS_MC_ADJUSTMENT | O1_FLICKERS_WHEN_MONSTERS_DETECTED,
	O2_DEMON_ITEM | O2_FLICKER_COLOR_BLACK,
	O3_PERMTTED_TARGET_LAWFUL | O3_PERMTTED_TARGET_NEUTRAL | O3_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS,
	PERMITTED_ALIGNMENT_CHAOTIC, S_ANGEL),
WEAPON("sword of defense", "runed long sword",  // STARTMARKER FOR MAGIC SWORDS WITH SHUFFLED APPREARANCES
	0, 0, 1, 0, 4, MULTIGEN_SINGLE, 60, 750,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S,WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_IS_ARMOR_WHEN_WIELDED, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("sword of sharpness", "ornamental long sword",
	0, 0, 1, 0, 4, MULTIGEN_SINGLE, 60, 3000,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 1, A1_SHARPNESS, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("vorpal sword", "immaculate long sword",
	0, 0, 1, 0, 2, MULTIGEN_SINGLE, 60, 5000,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 1, A1_VORPAL, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("sword of wounding", "exquisite long sword",
	0, 0, 1, 0, 4, MULTIGEN_SINGLE, 60, 750,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 1, A1_WOUNDING, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("sword of life stealing", "gold-hilted long sword",
	0, 0, 1, 0, 3, MULTIGEN_SINGLE, 60, 3000,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_LIFE_LEECH | A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA | A1_LEVEL_DRAIN | A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES | A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL, 2,
	0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_GOLD,
	O1_SPE_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("sword of orc slaying", "silver-hilted long sword",
	0, 0, 1, 0, 3, MULTIGEN_SINGLE, 60, 500,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 1, 12, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, BONUS_TO_HIT | IGNORE_SPE, 2, 0, 0,
	POISON_RES, WARN_ORC, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_SILVER,
	O1_NONE, O2_ELVEN_ITEM | O2_FLICKER_COLOR_BLUE, O3_TARGET_PERMISSION_IS_M2_FLAG | O3_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS, 
	(PERMITTED_RACE_MASK & ~PERMITTED_RACE_ORC), M2_ORC),
WEAPON("sword of troll slaying", "mithril-hilted long sword",
	0, 0, 1, 0, 3, MULTIGEN_SINGLE, 60, 750,
	AD_PHYS, 1, 8, 1, 1, 12, 1, AD_PHYS, 2, 12, 0, A1_WOUNDING, 0,
	1, 0, 0, 0, 0, 0, 0, BONUS_TO_HIT | IGNORE_SPE, 3, 0, 0,
	WARN_TROLL, NO_POWER, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_SILVER,
	O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_FLICKER_COLOR_BLUE, O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS,
	PERMITTED_ALL, S_TROLL),
WEAPON("sword of giant slaying", "decorated long sword",
	0, 0, 1, 0, 3, MULTIGEN_SINGLE, 60, 1000,
	AD_PHYS, 1, 8, 1, 1, 12, 1, AD_PHYS, 3, 12, 0, A1_NONE, 0,
	1, 0, 0, 0, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(19), 0, 0,
	WARN_GIANT, NO_POWER, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_FLICKER_COLOR_BLUE, O3_TARGET_PERMISSION_IS_M2_FLAG,
	PERMITTED_ALL, M2_GIANT),
WEAPON("sword of dragon slaying", "ancient long sword", //ENDMARKER FOR MAGIC SWORDS WITH SHUFFLED APPREARANCES
	0, 0, 1, 0, 3, MULTIGEN_SINGLE, 60, 1500,
	AD_PHYS, 1, 8, 1, 1, 12, 1, AD_PHYS, 4, 12, 0, A1_NONE, 0,
	1, 0, 0, 0, 0, 0, 0, BONUS_TO_HIT | IGNORE_SPE, 6, 0, 0,
	WARN_DRAGON, NO_POWER, NO_POWER, P1_NONE, S, WEP_BROADSWORD, P_SWORD, MAT_IRON, HI_METAL,
	O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_NONE, O3_NONE, PERMITTED_ALL, S_DRAGON),
WEAPON("glass sword", None,
	1, 0, 1, 0, 3, MULTIGEN_SINGLE, 20, 500,
	AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_GLASS, HI_GLASS,
	O1_NONE, O2_GENERATED_DEATH_OR_LIGHTNING_ENCHANTED, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("two-handed sword", None,
	1, 0, 0, 1, 18, MULTIGEN_SINGLE, 120, 50, 
	AD_PHYS, 2, 6, 0, 3, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_SWORD, P_SWORD, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("katana", "samurai sword",
	0, 0, 0, 0, 4, MULTIGEN_SINGLE, 80, 80, 
	AD_PHYS, 1, 10, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_KATANA, P_SWORD, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword", /* Base item for Tsurugi of Muramasa */
	0, 0, 0, 1, 0, MULTIGEN_SINGLE, 100, 500, 
	AD_PHYS, 2, 8, 0, 3, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P1_NONE, S, WEP_TSURUGI, P_SWORD,
	MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("runesword", "runed broadsword", /* Base item for Stormbringer */
	0, 0, 0, 0, 0, MULTIGEN_SINGLE, 55, 300, 
	AD_PHYS, 2, 4, 1, 1, 6, 2, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_BROADSWORD, P_SWORD, MAT_IRON, CLR_BLACK, 
	O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ALL, ALL_TARGETS),

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
	0, 0, 0, 1, 4, MULTIGEN_SINGLE, 80, 10, 
	AD_PHYS, 2, 6, 0, 2, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("ranseur", "hilted polearm",
	0, 0, 0, 1, 4, MULTIGEN_SINGLE, 50, 6, 
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("spetum", "forked polearm",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 50, 6, 
	AD_PHYS, 2, 5, 1, 2, 7, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("glaive", "single-edged polearm",
	0, 0, 0, 1, 7, MULTIGEN_SINGLE, 75, 6, 
	AD_PHYS, 2, 6, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("lance", None,
	1, 0, 0, 0, 4, MULTIGEN_SINGLE, 180, 10, 
	AD_PHYS, 1, 6, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P, WEP_LANCE, P_SPEAR, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
/* +2d10 when jousting with lance as primary weapon */
/* axe-type */
WEAPON("halberd", "angled poleaxe",
	0, 0, 0, 1, 7, MULTIGEN_SINGLE, 150, 10, 
	AD_PHYS, 2, 5, 0, 3, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P|S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("bardiche", "long poleaxe",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 120, 7, 
	AD_PHYS, 2, 4, 0, 3, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("huge infernal bardiche", "huge black poleaxe",
	0, 0, 0, 1, 0, MULTIGEN_SINGLE, 200, 250, 
	AD_PHYS, 3, 4, 3, 4, 4, 4, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_POLEARM, P_POLEARM, MAT_IRON, CLR_BLACK, 
	O1_NONE, O2_DEMON_ITEM, O3_EXTENDED_POLEARM_REACH,
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("voulge", "pole cleaver",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 125, 5, 
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish mattock", "broad pick",
	0, 0, 0, 1, 13, MULTIGEN_SINGLE, 120, 50, 
	AD_PHYS, 2, 6, 0, 3, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_PICK_AXE, P_PICK_AXE, MAT_IRON, HI_METAL, 
	O1_NONE, O2_DWARVEN_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
/* curved/hooked */
WEAPON("fauchard", "pole sickle",
	0, 0, 0, 1, 2, MULTIGEN_SINGLE, 60, 5, 
	AD_PHYS, 2, 6, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P | S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("guisarme", "pruning hook",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 80, 5, 
	AD_PHYS, 3, 4, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, S,WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("bill-guisarme", "hooked polearm",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 120, 7, 
	AD_PHYS, 3, 4, 0, 2, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P | S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("infernal ancus", "black ornamental hooked polearm",
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 100, 100, 
	AD_PHYS, 1, 6, 1, 1, 8, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, P | S, WEP_POLEARM, P_POLEARM, MAT_IRON, CLR_BLACK,
	O1_NONE, O2_DEMON_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
/* other */
WEAPON("lucern hammer", "pronged polearm",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 150, 7, 
	AD_PHYS, 3, 4, 0, 2, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B | P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("bec de corbin", "beaked polearm",
	0, 0, 0, 1, 3, MULTIGEN_SINGLE, 100, 1, 
	AD_PHYS, 2, 8, 1, 2, 8, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B | P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
/* magical */
WEAPON("polearm of reach", "elongated polearm",
	0, 0, 1, 1, 2, MULTIGEN_SINGLE, 100, 1,
	AD_PHYS, 2, 8, 2, 2, 8, 2, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, B | P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_EXTENDED_POLEARM_REACH,
	PERMITTED_ALL, ALL_TARGETS),

/* bludgeons */
WEAPON("mace", None,
	1, 0, 0, 0, 31, MULTIGEN_SINGLE, 60, 5, 
	AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver mace", None,
	1, 0, 0, 0, 2, MULTIGEN_SINGLE, 55, 250, 
	AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("mace of disruption", "runed silver mace",
	0, 0, 1, 0, 3, MULTIGEN_SINGLE, 60, 750,
	AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 2, 6, 2, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_SILVER, HI_SILVER,
	O1_NONE, 
	O2_SHINES_MAGICAL_LIGHT | O2_FLICKER_COLOR_WHITE, 
	O3_TARGET_PERMISSION_IS_M2_FLAG, 
	PERMITTED_ALL, M2_UNDEAD | M2_DEMON),
WEAPON("mace of death", "skull-headed mace", /* Also base weapon for Wand of Orcus */
	0, 0, 1, 0, 2, MULTIGEN_SINGLE, 40, 1500, 
	AD_PHYS, 1, 6, 2, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_MAGIC_RESISTANCE_PROTECTS, 10, 
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_BONE, CLR_BLACK, 
	O1_NONE, O2_DEMON_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("morning star", None,
	1, 0, 0, 0, 10, MULTIGEN_SINGLE, 60, 10, 
	AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MORNING_STAR, P_FLAIL, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("morning star of troll slaying", "mithril-spiked morning star", /* Base item for Trollsbane */
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 60, 500,
	AD_PHYS, 2, 4, 1, 1, 6, 2, AD_PHYS, 1, 6, 1, A1_WOUNDING, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	REGENERATION, WARN_TROLL, NO_POWER, P1_NONE, B, WEP_MORNING_STAR, P_FLAIL, MAT_IRON, HI_METAL,
	O1_INDESTRUCTIBLE | O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_FLICKER_COLOR_BLUE, O3_NO_WISH | O3_NO_GENERATION | O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS,
	PERMITTED_ALL, S_TROLL),
WEAPON("war hammer", None,
	1, 0, 0, 0, 11, MULTIGEN_SINGLE, 50, 5, 
	AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_HAMMER, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("heavy war hammer", "runed war hammer", /* Base object for Mjollnir */
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 100, 1000, 
	AD_PHYS, 1, 8, 1, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_HAMMER, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL,
	O1_INDESTRUCTIBLE | O1_MELEE_AND_THROWN_WEAPON | O1_WEIGHT_DOES_NOT_REDUCE_RANGE | O1_RETURNS_TO_HAND_AFTER_THROWING | O1_CAN_BE_THROWN_ONLY_IF_WIELDED,
	O2_NONE, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ROLE_VALKYRIE, ALL_TARGETS),
WEAPON("war hammer of ogre slaying", "runed war hammer", /* Base object for Ogresmasher */
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 100, 500,
	AD_PHYS, 1, 6, 2, 1, 6, 1, AD_PHYS, 2, 8, 0, A1_NONE, 0,
	1, 0, 0, 0, 0, 0, 0, BONUS_TO_CON | SETS_FIXED_ATTRIBUTE | IGNORE_SPE, 25, 0, 0,
	WARN_OGRE, NO_POWER, NO_POWER, P1_NONE, B, WEP_HAMMER, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL,
	O1_INDESTRUCTIBLE | O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_NONE, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ALL, S_OGRE),
WEAPON("war hammer of gnoll slaying", "runed war hammer",
	0, 0, 1, 0, 2, MULTIGEN_SINGLE, 50, 500,
	AD_PHYS, 1, 6, 1, 1, 6, 1, AD_PHYS, 2, 6, 0, A1_NONE, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	WARN_GNOLL, NO_POWER, NO_POWER, P1_NONE, B, WEP_HAMMER, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL,
	O1_FLICKERS_WHEN_MONSTERS_DETECTED, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG | O3_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS,
	(PERMITTED_RACE_MASK & ~PERMITTED_RACE_GNOLL), M2_GNOLL),
WEAPON("club", None,
	1, 0, 0, 0, 12, MULTIGEN_SINGLE, 40, 3, 
	AD_PHYS, 1, 6, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_CLUB, P_BLUDGEONING_WEAPON, MAT_WOOD, HI_WOOD, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("infernal jagged-toothed club", "black jagged-toothed club",
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 60, 100, 
	AD_PHYS, 1, 6, 1, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_SPIKED_CLUB, P_BLUDGEONING_WEAPON, MAT_WOOD, CLR_BLACK, 
	O1_NONE, O2_DEMON_ITEM, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("rubber hose", None,
	1, 0, 0, 0, 0, MULTIGEN_SINGLE, 20, 3, 
	AD_PHYS, 1, 4, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_HOSE, P_WHIP, MAT_PLASTIC, CLR_BROWN, 
	O1_NONE, O2_FLIMSY, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
	
/* staffs */
WEAPON("flindbar", "iron staff",
	0, 0, 0, 1, 0, MULTIGEN_SINGLE, 160, 5, 
	AD_PHYS, 2, 10, 0, 3, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_ROD, P_QUARTERSTAFF, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("quarterstaff", "wooden staff",
	0, 0, 0, 1, 11, MULTIGEN_SINGLE, 30, 5, 
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of the magi", "runed staff", //STARTMARKER FOR SHUFFLED STAFFS
	0, 0, 1, 1, 3, MULTIGEN_SINGLE, 30, 1500,
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_ELEC, 2, 6, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 50, 0, BONUS_TO_SPELL_CASTING, 20, 0, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_MANA_PERCENTAGE_BONUS, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD,
	O1_COLD_RESISTANT | O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT, 
	O2_NONE, O3_NONE, 
	PERMITTED_ROLE_WIZARD, ALL_TARGETS),
WEAPON("staff of fire", "ornamental staff",
	0, 0, 1, 1, 5, MULTIGEN_SINGLE, 30, 1000,
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_FIRE, 2, 6, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, -8, 0,
	FIRE_RES, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD,
	O1_FIRE_RESISTANT | O1_SPE_AFFECTS_MC, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of frost", "blue-hued staff",
	0, 0, 1, 1, 8, MULTIGEN_SINGLE, 30, 500,
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_COLD, 1, 6, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, -4, 0,
	COLD_RES, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD,
	O1_COLD_RESISTANT, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of thunder and lightning", "forked staff",
	0, 0, 1, 1, 3, MULTIGEN_SINGLE, 30, 1500,
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_ELEC, 3, 6, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, -12, 0,
	SHOCK_RES, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD,
	O1_LIGHTNING_RESISTANT | O1_SPE_AFFECTS_MC, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of life", "serpent-entwined staff",
	0, 0, 1, 1, 7, MULTIGEN_SINGLE, 30, 750,
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	DRAIN_RES, DEATH_RES, REGENERATION, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD,
	O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, 
	PERMITTED_ALL, M2_UNDEAD),
WEAPON("staff of withering", "twisted staff", //ENDMARKER FOR SHUFFLED STAFFS
	0, 0, 1, 1, 3, MULTIGEN_SINGLE, 30, 1500,
	AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_LEVEL_DRAIN, 0,
	0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	DRAIN_RES, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD,
	O1_SPE_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),

/* two-piece */
WEAPON("aklys", "thonged club",
	0, 0, 0, 0, 8, MULTIGEN_SINGLE, 15, 4, 
	AD_PHYS, 1, 6, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_SPIKED_CLUB, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL, 
	O1_THROWN_WEAPON_ONLY | O1_RETURNS_TO_HAND_AFTER_THROWING, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("flail", None,
    1, 0, 0, 0, 33, MULTIGEN_SINGLE, 35, 4, 
	AD_PHYS, 1, 6, 1, 2, 4, 0, 	AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver flail", None,
    1, 0, 0, 0, 4, MULTIGEN_SINGLE, 35, 40, 
	AD_PHYS, 1, 6, 1, 2, 4, 0, 	AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_SILVER, HI_SILVER, 
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("double-headed flail", None,
	1, 0, 0, 0, 3, MULTIGEN_SINGLE, 55, 250, 
	AD_PHYS, 1, 6, 1, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_MELEE_2_EXPERT_1D2_SKILLED, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
	/* one-handed, strikes up to two times, -1 hit bonus (difficult to wield) */
WEAPON("triple-headed flail", None,
	1, 0, 0, 0, 0, MULTIGEN_SINGLE, 70, 1000, 
	AD_PHYS, 1, 6, 1, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0, 
	-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_MELEE_3_EXPERT_2_SKILLED, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
	/* one-handed, strikes up to three times, -2 hit bonus (difficult to wield) */
WEAPON("triple-headed flail of life draining", "runed triple-headed flail", /* The flail of Yeenoghu, currently non-artifact */
	0, 0, 1, 0, 0, MULTIGEN_SINGLE, 70, 8000, 
	AD_PHYS, 1, 6, 3, 2, 4, 2, AD_PHYS, 0, 0, 0, A1_LEVEL_DRAIN, 0, 
	0, -2, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_MELEE_3_EXPERT_2_SKILLED, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 
	O1_SPE_AFFECTS_MC_ADJUSTMENT, O2_DEMON_ITEM, O3_NO_WISH | O3_NO_GENERATION, PERMITTED_ALL, ALL_TARGETS),
WEAPON("runed flail", None, /* Base item for Howling Flail */
    1, 0, 1, 0, 0, MULTIGEN_SINGLE, 35, 1000, 
	AD_PHYS, 1, 6, 2, 2, 4, 1, 	AD_PHYS, 2, 4, 0, A1_NONE, 0, 
	1, 0, 0, 0, 0, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE | IGNORE_SPE, 25, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL,
	O1_INDESTRUCTIBLE, O2_GNOLLISH_ITEM | O2_DEMON_ITEM, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_RACE_GNOLL, ALL_TARGETS),


/* misc */
WEAPON("bullwhip", None,
    1, 0, 0, 0, 2, MULTIGEN_SINGLE, 20, 4,
	AD_PHYS, 1, 2, 0,  1, 1, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, 0, WEP_WHIP, P_WHIP, MAT_LEATHER, CLR_BROWN,
	O1_NONE, O2_NONE, O3_NONE, 
	PERMITTED_ALL, ALL_TARGETS),
WEAPON("black blade of disintegration", "sword-shaped planar rift",
   	0, 0, 1, 1, 0, MULTIGEN_SINGLE, 0, 0, 
	AD_PHYS, 2, 8, 0,  3, 8, 0, AD_PHYS, 0, 0, 0, A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK, 100, 
	0, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_SWORD, P_SWORD, MAT_PLANARRIFT, CLR_BLACK, 
	O1_DISINTEGRATION_RESISTANT | O1_SPE_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NO_WISH | O3_NO_GENERATION,
	PERMITTED_ALL, ALL_TARGETS),

/* bows */
BOW("bow", None,							1, 0, 1, 0, MULTIGEN_SINGLE, 30, 30, 
	AD_PHYS, 1, 3, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("short bow", None,						1, 0, 1, 12, MULTIGEN_SINGLE, 30, 30, 
	AD_PHYS, 1, 3, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -80,  0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("elven long bow", "runed long bow",		0, 0, 1, 12, MULTIGEN_SINGLE, 40, 200, 
	AD_PHYS, 1, 5, 0, 1, 7, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		1, 0, 0, -110, 0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, O1_NONE, O2_ELVEN_ITEM, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("orcish short bow", "crude short bow",  0, 0, 1, 12, MULTIGEN_SINGLE, 35, 10, 
	AD_PHYS, 1, 2, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -70,  0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, CLR_BLACK, O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("long bow", None,						1, 0, 1, 12, MULTIGEN_SINGLE, 45, 75, 
	AD_PHYS, 1, 4, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("composite long bow", None,				1, 0, 1, 6, MULTIGEN_SINGLE, 50, 250, 
	AD_PHYS, 1, 5, 1, 1, 7, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -130, 0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("composite short bow", None,			1, 0, 1, 6, MULTIGEN_SINGLE, 35, 100, 
	AD_PHYS, 1, 4, 1, 1, 5, 1, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -90,  0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("yumi", "oriental long bow",			0, 0, 1, 0, MULTIGEN_SINGLE, 55, 150, 
	AD_PHYS, 1, 4, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -120, 0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("sling", None,							1, 0, 0, 34, MULTIGEN_SINGLE, 3, 20, 
	AD_PHYS, 1, 2, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 0, 0, -60,  0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_LEATHER, WEP_SLING, P_SLING, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("crossbow", None,						1, 0, 1, 30, MULTIGEN_SINGLE, 100, 75, 
	AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		0, 5, 3, 18,   0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_CROSSBOW, P_CROSSBOW, HI_WOOD, O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, PERMITTED_ALL, ALL_TARGETS),
BOW("hand crossbow", None,					1, 0, 0, 5, MULTIGEN_SINGLE, 40, 50, 
	AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0,	A1_NONE, 0,		0, 0, 0, 9,    0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_HAND_CROSSBOW, P_CROSSBOW, HI_WOOD, O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, PERMITTED_ALL, ALL_TARGETS),
BOW("heavy crossbow", None,					1, 0, 1, 10, MULTIGEN_SINGLE, 200, 150, 
	AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,	0, 0, 8, 24,   0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_CROSSBOW, P_CROSSBOW, HI_WOOD, O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, PERMITTED_ALL, ALL_TARGETS),
BOW("repeating crossbow", None,				1, 0, 1, 5, MULTIGEN_SINGLE, 180, 75, 
	AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,		-2, 0, 3, 18,  0, 0, 0, 0, 0, MULTISHOT_LAUNCHER_2_NOSKILL, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_REPEATING_CROSSBOW, P_CROSSBOW, HI_WOOD, O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, PERMITTED_ALL, ALL_TARGETS),
BOW("repeating heavy crossbow", None,		1, 0, 1, 1, MULTIGEN_SINGLE, 360, 1000,
	AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,	-2, 0, 6, 24,	0, 0, 0, 0, 0, MULTISHOT_LAUNCHER_2_NOSKILL, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_REPEATING_CROSSBOW, P_CROSSBOW, HI_WOOD, O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, PERMITTED_ALL, ALL_TARGETS),

#undef P
#undef S
#undef B

#undef WEAPON
#undef PROJECTILE
#undef BOW

/* armor ... */
        /* MAT_IRON denotes ferrous metals, including steel.
         * Only MAT_IRON weapons and armor can rust.
         * Only MAT_COPPER (including brass) corrodes.
         * Some creatures are vulnerable to MAT_SILVER.
         */
#define ARMOR(name,desc,kn,mgc,blk,power,power2,power3,pflags,prob,delay,wt,  \
              cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,sub,skill,metal,c,flags,flags2,flags3,powconfermask)                   \
    OBJECT(OBJ(name, desc), None, None,                                       \
           BITS(kn, 0, 1, 0, mgc, CHARGED_GENERAL, 0, 0, blk, 0, 0, sub, skill, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, delay, wt, cost,                     \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 10 - ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, 0, \
		   wt, c, \
		   0, 0, 0, 0, \
		   powconfermask, ALL_TARGETS, flags, flags2, flags3 )
#define HELM(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_HELM, P_NONE, metal, c, flags, flags2, flags3,powconfermask)
#define CLOAK(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_CLOAK, P_NONE, metal, c, flags, flags2, flags3,powconfermask)
#define SHIELD(name,desc,kn,mgc,blk,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask) \
    ARMOR(name, desc, kn, mgc, blk, power, power2, power3, pflags, prob, delay, wt, \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_SHIELD, P_NONE, metal, c, flags, flags2, flags3,powconfermask)
#define GLOVES(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_GLOVES, P_NONE, metal, c, flags, flags2, flags3, powconfermask)
#define BOOTS(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_BOOTS, P_NONE, metal, c, flags, flags2, flags3, powconfermask)
#define SHIRT(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags,prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_SHIRT, P_NONE, metal, c, flags, flags2, flags3, powconfermask)
#define ROBE(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_ROBE, P_NONE, metal, c, flags, flags2, flags3, powconfermask)
#define BRACERS(name,desc,kn,mgc,power,power2,power3,pflags,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_BRACERS, P_NONE, metal, c, flags, flags2, flags3, powconfermask)

/* note: acbon = 10 - ac used in ARMOR, i.e., 1 gives -1 AC bonus to the player */
#define WEAPONSHIELD(name,desc,kn,mgc,prob,wt,cost,\
			dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct, \
			hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
			power,power2,power3,pflags,typ,skill,metal,color,\
			flags,flags2,flags3,powconfermask,permittedtargets) \
    OBJECT(OBJ(name,desc), None, None,                                         \
           BITS(kn, 0, 1, 0, mgc, CHARGED_GENERAL, 0, 0, 0, 0, typ, ARM_SHIELD, skill, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
		   dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, critpct, \
		   hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
		   wt, color, \
		   0, 0, 0, 0, \
		   powconfermask,permittedtargets, O1_IS_WEAPON_WHEN_WIELDED | flags, flags2, flags3)

#define WEAPONBOOTS(name,desc,kn,mgc,delay,prob,wt,cost,\
			dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct, \
			hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
			power,power2,power3,pflags,typ,metal,color,\
			flags,flags2,flags3,powconfermask,permittedtargets) \
    OBJECT(OBJ(name,desc), None, None,                                         \
           BITS(kn, 0, 1, 0, mgc, CHARGED_GENERAL, 0, 0, 0, 0, typ, ARM_BOOTS, P_MARTIAL_ARTS, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, delay, wt, cost, \
		   dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, critpct, \
		   hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
		   wt, color, \
		   0, 0, 0, 0, \
		   powconfermask,permittedtargets, flags, flags2, flags3)

#define WEAPONGLOVES(name,desc,kn,mgc,delay,prob,wt,cost,\
			dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,critpct, \
			hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
			power,power2,power3,pflags,typ,metal,color,\
			flags,flags2,flags3,powconfermask,permittedtargets) \
    OBJECT(OBJ(name,desc), None, None,                                         \
           BITS(kn, 0, 1, 0, mgc, CHARGED_GENERAL, 0, 0, 0, 0, typ, ARM_GLOVES, P_BARE_HANDED_COMBAT, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, delay, wt, cost, \
		   dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, critpct, \
		   hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
		   wt, color, \
		   0, 0, 0, 0, \
		   powconfermask,permittedtargets, flags, flags2, flags3)


/* helmets */
HELM("elven leather helm", "leather hat",
    0, 0,           0, 0, 0, P1_NONE,
	6, 1,  3,  8,  9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, 
	O1_NONE, O2_ELVEN_ITEM, O3_NONE, PERMITTED_ALL),
HELM("cotton hood", None,
    1, 0,           0, 0, 0, P1_NONE,  
	3, 1,  3,  8,  10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("gnollish hood", "crude leather hood",
    0, 0,           0, 0, 0, P1_NONE,
	3, 1,  3,  8,  9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, 
	O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, PERMITTED_ALL),
HELM("orcish helm", "iron skull cap",
    0, 0,           0, 0, 0, P1_NONE,
	6, 1, 30, 10,  9, 0, 0, 0, 0, 0, 4, MAT_IRON, CLR_BLACK, 
	O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL),
HELM("dwarvish iron helm", "hard hat",
    0, 0,           0, 0, 0, P1_NONE,
	6, 1, 40, 20,  8, 0, 0, 0, 0, 0, 4, MAT_IRON, HI_METAL, 
	O1_NONE, O2_DWARVEN_ITEM, O3_NONE, PERMITTED_ALL),
HELM("fedora", None,
    1, 0,           0, 0, 0, P1_NONE, 0, 0,
	3,  1, 10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BROWN, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("tinfoil hat of mind shielding", "thin metal hat",
    0, 1, MIND_SHIELDING, CHARM_RES, ODD_IDEAS, P1_NONE,
	3, 1, 6, 80, 10, 1, 0, 0, BONUS_TO_INT | BONUS_TO_WIS, -2, 6, MAT_METAL, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("ducal crown", "golden crown", //STARTMARKER FOR SHUFFLED CROWNS & CROWN WISH RANGE
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	3, 1, 20, 1000, 10, 0, 0, 0, 0, 0, 0, MAT_GOLD, HI_GOLD,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("crown of rulership", "ornamental crown", //ENDMARKER FOR SHUFFLED CROWNS & CROWN WISH RANGE
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	3, 1, 20, 5000, 10, 0, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE, 20, 0, MAT_GOLD, HI_GOLD, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("cornuthaum", "blue conical hat", //STARTMARKER FOR CONICAL HATS
    0, 1, CLAIRVOYANT, BLOCKS_CLAIRVOYANCE, NO_POWER, P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS | P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY,
	3, 1, 4, 80, 10, 1, 0, 0, BONUS_TO_CHA | IGNORE_SPE, 1, 0, MAT_CLOTH, CLR_BLUE, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ROLE_WIZARD),
        /* name coined by devteam; confers clairvoyance for wizards,
           blocks clairvoyance if worn by role other than wizard */
HELM("dunce cap", "white conical hat",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	3, 1,  4,  1, 10, 0, 0, 0, BONUS_TO_INT | BONUS_TO_WIS | FIXED_IS_MAXIMUM | SETS_FIXED_ATTRIBUTE, 6, 0, MAT_CLOTH, CLR_WHITE, 
	O1_BECOMES_CURSED_WHEN_WORN, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("gnomish felt hat", "red conical hat",  //ENDMARKER FOR CONICAL HATS
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	3, 1, 4, 1, 10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_RED, 
	O1_NONE, O2_GNOMISH_ITEM, O3_NONE, PERMITTED_ALL),
HELM("silk top hat", "cylindrical hat", 
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	3, 1, 4, 100, 10, 0, 0, 0, 0, 0, 0, MAT_SILK, CLR_BLACK, 
	O1_NONE, O2_GNOMISH_ITEM, O3_NONE, PERMITTED_ALL),
HELM("dented pot", None,
    1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	2, 0, 30,  8,  9, 0, 0, 0, 0, 0, 4, MAT_IRON, CLR_BLACK, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
HELM("helmet", "plumed helmet",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	10, 1, 50, 10,  9, 0, 0, 0, 0, 0, 4, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("helm of brilliance", "etched helmet",
	0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	6, 1, 50, 50,  9, 0, 0, 0, BONUS_TO_INT | BONUS_TO_WIS, 0, 0, MAT_IRON, CLR_GREEN, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
HELM("helm of opposite alignment", "crested helmet",
     0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	6, 1, 50, 50,  9, 0, 0, 0, 0, 0, 4, MAT_IRON, HI_METAL, 
	O1_BECOMES_CURSED_WHEN_WORN, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
HELM("helm of telepathy", "visored helmet",
    0, 1, TELEPAT, NO_POWER, NO_POWER, P1_NONE,
	2, 1, 50, 50,  9, 0, 0, 0, 0, 0, 2, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in obj.h, objnam.c, mon.c, read.c that assumes (2).
 *      (1) The dragon scale mails and the dragon scales are together.
 *      (2) That the order of the dragon scale mail and dragon scales
 *          is the same as order of dragons defined in monst.c.
 */
#define DRGN_ARMR(name,mgc,power,power2,power3,pflags,cost,ac,mc,manabon,hpbon,bonusattrs,attrbonus,splcastpen,color,flags,flags2,flags3,powconfermask)  \
    ARMOR(name, None, 1, mgc, 1, power, power2, power3, pflags, 0, 5, 550,  \
          cost, ac, mc, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_SUIT, P_NONE, MAT_DRAGON_HIDE, color, flags, flags2, flags3, powconfermask)
/* 3.4.1: dragon scale mail reclassified as "magic" since magic is
   needed to create them */
DRGN_ARMR("gray dragon scale mail",    1, ANTIMAGIC,  0, 0, P1_NONE, 6000, 1, 4, 0, 0, 0, 0, 40, CLR_GRAY, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("silver dragon scale mail",  1, REFLECTING, 0, 0, P1_NONE, 6000, 1, 4, 0, 0, 0, 0, 30, DRAGON_SILVER, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scale mail", 1, DISPLACED, 1200, 1, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scale mail",     1, FIRE_RES,   0, 0, P1_NONE, 4000, 1, 4, 0, 0, 0, 0, 20, CLR_RED, O1_FIRE_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("white dragon scale mail",   1, COLD_RES,   0, 0, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 20, CLR_WHITE, O1_COLD_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("orange dragon scale mail",  1, SLEEP_RES,  0, 0, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 20, CLR_ORANGE, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("black dragon scale mail",   1, DISINT_RES, 0, 0, P1_NONE, 5000, 1, 4, 0, 0, 0, 0, 20, CLR_BLACK, O1_DISINTEGRATION_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("blue dragon scale mail",    1, SHOCK_RES,  0, 0, P1_NONE, 4000, 1, 4, 0, 0, 0, 0, 20, CLR_BLUE, O1_LIGHTNING_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("green dragon scale mail",   1, POISON_RES, 0, 0, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 20, CLR_GREEN, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("yellow dragon scale mail",  1, ACID_RES,   0, 0, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 20, CLR_YELLOW, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, PERMITTED_ALL),
/* For now, only dragons leave these. */
/* 3.4.1: dragon scales left classified as "non-magic"; they confer
   magical properties but are produced "naturally" */
DRGN_ARMR("gray dragon scales",        0, ANTIMAGIC,  0, 0, P1_NONE, 1500, 7, 1, 0, 0, 0, 0, 40, CLR_GRAY, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("silver dragon scales",      0, REFLECTING, 0, 0, P1_NONE, 1500, 7, 1, 0, 0, 0, 0, 30, DRAGON_SILVER, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scales",  0, DISPLACED,   700, 7, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scales",         0, FIRE_RES,   0, 0, P1_NONE, 1000, 7, 1, 0, 0, 0, 0, 20, CLR_RED, O1_FIRE_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("white dragon scales",       0, COLD_RES,   0, 0, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 20, CLR_WHITE, O1_COLD_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("orange dragon scales",      0, SLEEP_RES,  0, 0, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 20, CLR_ORANGE, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("black dragon scales",       0, DISINT_RES, 0, 0, P1_NONE, 1250, 7, 1, 0, 0, 0, 0, 20, CLR_BLACK, O1_DISINTEGRATION_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("blue dragon scales",        0, SHOCK_RES,  0, 0, P1_NONE, 1000, 7, 1, 0, 0, 0, 0, 20, CLR_BLUE, O1_LIGHTNING_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("green dragon scales",       0, POISON_RES, 0, 0, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 20, CLR_GREEN, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
DRGN_ARMR("yellow dragon scales",      0, ACID_RES,   0, 0, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 20, CLR_YELLOW, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, PERMITTED_ALL),
#undef DRGN_ARMR
/* other suits */
ARMOR("adamantium full plate mail", None, 1, 
	0, 1, DISINT_RES, NO_POWER, NO_POWER, P1_NONE,
	5, 7, 960, 3000, -1, 5, 0, 0, 0, 0, 20, 
	ARM_SUIT, P_NONE, MAT_ADAMANTIUM, HI_SILVER, 
	O1_INDESTRUCTIBLE | O1_DISINTEGRATION_RESISTANT | O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, 
	O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("mithril full plate mail", None,
	  1, 0, 1,			 0, 0, 0, P1_NONE, 5, 7, 550,3000,  0, 8, 0, 0, 0, 0, 12, ARM_SUIT, P_NONE, MAT_MITHRIL, HI_SILVER, O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("orichalcum full plate mail", None,
	  1, 0, 1,	 ANTIMAGIC, 0, 0, P1_NONE, 5, 7, 960,3500,  1, 12, 0, 0, 0, 0, 40, ARM_SUIT, P_NONE, MAT_ORICHALCUM, HI_GOLD, O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("full plate mail", None,
	  1, 0, 1,			 0, 0, 0, P1_NONE, 10, 7, 960,2000,  1, 5, 0, 0, 0, 0, 20, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("field plate mail", None,
	  1, 0, 1,			 0, 0, 0, P1_NONE, 20, 6, 840,1000,  2, 4, 0, 0, 0, 0, 18, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("plate mail", None,
      1, 0, 1,			 0, 0, 0, P1_NONE, 30, 5, 720, 600,  3, 3, 0, 0, 0, 0, 16, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("crystal plate mail", None,
      1, 0, 1,  REFLECTING, 0, 0, P1_NONE, 5, 5, 640,3000,  3, 3, 0, 0, 0, 0,  4, ARM_SUIT, P_NONE, MAT_GLASS, CLR_WHITE, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("bronze plate mail", None,
      1, 0, 1,			 0, 0, 0, P1_NONE, 20, 5, 800, 150,  4, 8, 0, 0, 0, 0, 16, ARM_SUIT, P_NONE, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("splint mail", None,
      1, 0, 1,			 0, 0, 0, P1_NONE, 30, 5, 640, 125,  4, 3, 0, 0, 0, 0, 12, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("banded mail", None,
      1, 0, 1,			 0, 0, 0, P1_NONE, 30, 5, 560, 200,  4, 3, 0, 0, 0, 0, 10, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("dwarvish mithril-coat", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 10, 1, 250, 750,  4, 6, 0, 0, 0, 0, 8, ARM_SUIT, P_NONE, MAT_MITHRIL, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_DWARVEN_ITEM, O3_NONE, PERMITTED_ALL),
ARMOR("elven mithril-coat", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 15, 1, 150, 250,  5, 4, 0, 0, 0, 0, 0, ARM_SUIT, P_NONE, MAT_MITHRIL, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_ELVEN_ITEM, O3_NONE, PERMITTED_ALL),
ARMOR("chain mail", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 35, 5, 480,  75,  5, 3, 0, 0, 0, 0, 8, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("orcish chain mail", "crude chain mail",
      0, 0, 0,			 0, 0, 0, P1_NONE, 20, 5, 450,  40,  6, 3, 0, 0, 0, 0, 12, ARM_SUIT, P_NONE, MAT_IRON, CLR_BLACK, O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL),
ARMOR("scale mail", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 35, 5, 500,  45,  6, 3, 0, 0, 0, 0, 6, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("studded leather armor", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 35, 3, 200,  15,  7, 3, 0, 0, 0, 0, 4, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("gnollish studded leather armor", "crude studded leather armor",
      0, 0, 0,			 0, 0, 0, P1_NONE, 30, 3, 175,  15,  8, 3, 0, 0, 0, 0, 6, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, PERMITTED_ALL),
ARMOR("ring mail", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 35, 5, 500,  30,  7, 2, 0, 0, 0, 0, 6, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("orcish ring mail", "crude ring mail",
      0, 0, 0,			 0, 0, 0, P1_NONE, 20, 5, 450,  15,  8, 2, 0, 0, 0, 0, 10, ARM_SUIT, P_NONE, MAT_IRON, CLR_BLACK, O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL),
ARMOR("leather armor", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 35, 3, 150,   5,  8, 2, 0, 0, 0, 0, 2, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("gnollish leather armor", "crude leather armor",
      0, 0, 0,			 0, 0, 0, P1_NONE, 20, 3, 125,   3,  9, 2, 0, 0, 0, 0, 4, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, PERMITTED_ALL),
ARMOR("leather jacket", None,
      1, 0, 0,			 0, 0, 0, P1_NONE, 10, 0,  30,  10,  9, 1, 0, 0, 0, 0, 0, ARM_SUIT, P_NONE, MAT_LEATHER, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ARMOR("force field armor", "armor-shaped force field",
      0, 1, 0,			 0, 0, 0, P1_NONE, 0,  0,   0,   4,  4, 6, 0, 0, 0, 0, -2, ARM_SUIT, P_NONE, MAT_FORCEFIELD, HI_GLASS, O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT | O1_COLD_RESISTANT | O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, PERMITTED_ALL),

/* shirts */
ARMOR("Hawaiian shirt", None, //STARTMARKER 1
    1, 0, 0, 0,			 0, 0, P1_NONE, 8, 0, 5,  5, 10, 1, 0, 0, 0, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_MAGENTA, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
/* with shuffled appearances... */
ARMOR("shirt of uncontrollable laughter", "funny T-shirt", //STARTMARKER 2
    0, 1, 0,  LAUGHING,  0, 0, P1_NONE, 6, 0, 5, 50, 10, 1, 0, 0, 0, 0, 6, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_BLUE, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_READABLE, PERMITTED_ALL),
ARMOR("shirt of comeliness", "black T-shirt",
	0, 1, 0, 0,			 0, 0, P1_NONE, 6, 0, 5, 50, 10, 1, 0, 0, BONUS_TO_CHA, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_BLACK, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
ARMOR("shirt of sound mindedness", "green T-shirt", 
	0, 1, 0, HALLUC_RES, 0, 0, P1_NONE, 6, 0, 5, 50, 10, 1, 0, 0, 0, 0, -2, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_GREEN, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
ARMOR("T-shirt", "old T-shirt",//ENDMARKER 1 & 2
    0, 0, 0, 0,			 0, 0, P1_NONE, 4, 0, 5,  2, 10, 1, 0, 0, 0, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_WHITE, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),


/* cloaks */
CLOAK("elven cloak", "faded pall",
      0, 1,        STEALTH, 0, 0, P1_NONE, 8,  0, 10, 60,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, O1_NONE, O2_ELVEN_ITEM, O3_NONE, PERMITTED_ALL),
CLOAK("orcish cloak", "coarse mantelet",
      0, 0,				 0, 0, 0, P1_NONE, 8,  0, 10, 40, 10, 1, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL),
CLOAK("dwarvish cloak", "hooded cloak",
      0, 0,			     0, 0, 0, P1_NONE, 8,  0, 10, 50, 10, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, PERMITTED_ALL),
CLOAK("oilskin cloak", "slippery cloak",
      0, 0,				 0, 0, 0, P1_NONE, 8,  0, 10, 50,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CLOAK("alchemy smock", "apron",
      0, 1, POISON_RES, ACID_RES, 0, P1_NONE, 9, 0, 10, 50,  9, 1, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
CLOAK("leather cloak", None,
      1, 0,				 0, 0, 0, P1_NONE, 8,  0, 15, 40,  9, 1, 0, 0, 0, 0, 0, MAT_LEATHER, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
CLOAK("cloak of protection", "tattered cape",
      0, 1, NO_POWER, 0, 0, P1_NONE, 9, 0, 10, 250,  7, 3, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, O1_SPE_AFFECTS_MC, O2_NONE, O3_NONE, PERMITTED_ALL),
        /* cloak of protection is now the only item conferring MC 3 */
CLOAK("cloak of invisibility", "opera cloak",
      0, 1,      INVISIBILITY, 0, 0, P1_NONE, 9, 0, 10, 250,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CLOAK("cloak of magic resistance", "ornamental cope",
      0, 1,  ANTIMAGIC, 0, 0, P1_NONE, 6, 0, 10, 250,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
        /*  'cope' is not a spelling mistake... leave it be */
CLOAK("cloak of integrity", "hard woven cloak",
      0, 1,  DISINT_RES, 0, 0, P1_NONE, 6, 0, 10, 250,  8, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, O1_DISINTEGRATION_RESISTANT | O1_FIRE_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
CLOAK("cloak of displacement", "piece of cloth",
      0, 1,  DISPLACED, 0, 0, P1_NONE, 7, 0, 10, 250,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* robes and gowns */
/* with shuffled appearances... */
ROBE("robe", "old robe", //STARTMARKER 1 & 2
	0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 10, 1, 20,  25,  10, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ROBE("robe of protection", "golden ornamental robe",
	0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 10, 1, 25, 400,  7, 4, 0, 0, 0, 0, 0, MAT_CLOTH, HI_GOLD, O1_SPE_AFFECTS_MC, O2_NONE, O3_NONE, PERMITTED_ALL),
ROBE("robe of magic resistance", "silvery ornamental robe",
	0, 1, ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, 9, 1, 25, 400,  9, 3, 0, 0, 0, 0, 0, MAT_CLOTH, HI_SILVER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ROBE("gown of the archbishops", "shining purple robe",
	0, 1, CURSE_RES, DEATH_RES, DRAIN_RES, P1_MANA_PERCENTAGE_BONUS | P1_HP_PERCENTAGE_BONUS, 5, 2, 30, 400,  9, 4,15, 15, 0, 0, 0, MAT_CLOTH, CLR_MAGENTA, O1_NOT_CURSEABLE, O2_GENERATED_BLESSED, O3_NONE, PERMITTED_ALL),
ROBE("robe of powerlessness", "shining green robe",
	0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 10, 1, 25, 400,  9, 2, 0, 0, BONUS_TO_STR | BONUS_TO_INT | SETS_FIXED_ATTRIBUTE | FIXED_IS_MAXIMUM | IGNORE_SPE, 3, 0, MAT_CLOTH, CLR_GREEN, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
ROBE("robe of eyes", "star-patterned robe",
	0, 1, INFRAVISION, SEE_INVISIBLE, SEARCHING, P1_NONE, 5, 1, 25, 400,  9, 3, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ROBE("robe of the archmagi", "shining blue robe",
	0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 5, 2, 30, 400,  9, 3, 0, 0, BONUS_TO_INT | SETS_FIXED_ATTRIBUTE, 20, 0, MAT_CLOTH, HI_ZAP, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ROBE("robe of starry wisdom", "shining black robe", //ENDMARKER 2
	0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 5, 2, 30, 400,  9, 3, 0, 0, BONUS_TO_WIS | SETS_FIXED_ATTRIBUTE, 20, 0, MAT_CLOTH, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* without shuffled appearances... */
ROBE("bathrobe", "soft cotton robe",
	0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 6, 1, 25,   5, 10, 1, 0, 0, 0, 0, 2, MAT_CLOTH, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
ROBE("clerical gown", None,
	1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 6, 1, 15,  25, 10, 3, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, O1_NONE, O2_GENERATED_BLESSED, O3_NONE, PERMITTED_ALL),
ROBE("tailored silk robe", "fine robe",
	0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 3, 1, 30, 100, 10, 1, 0, 0, 0, 0, 0, MAT_SILK, CLR_BROWN, O1_NONE, O2_GNOMISH_ITEM, O3_NONE, PERMITTED_ALL),
ROBE("gnollish haircloth robe", "crude robe",
	0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 6, 1, 30,  10,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BROWN, O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, PERMITTED_ALL),
ROBE("mummy wrapping", None, //ENDMARKER 1
	1, 0, BLOCKS_INVISIBILITY, NO_POWER, NO_POWER, P1_NONE, 0, 0,  3,   2, 10, 1, 0, 0, 0, 0, 4, MAT_CLOTH, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
	/* worn mummy wrapping blocks invisibility */


/* bracers */
/* with shuffled appearances... */
BRACERS("leather bracers", "old bracers", //STARTMARKER 1 & 2
	0, 0, 0,		  0, 0, P1_NONE, 10, 1, 10,   5, 9, 1, 0, 0, 0, 0, 0, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BRACERS("bracers of defense", "runed bracers",
	0, 1, NO_POWER, 0, 0, P1_NONE, 10, 1, 10, 300, 7, 3, 0, 0, 0, 0, 0, MAT_CLOTH, HI_LEATHER, O1_SPE_AFFECTS_MC, O2_NONE, O3_NONE, PERMITTED_ALL),
BRACERS("bracers of archery", "deerskin bracers",
	0, 1, 0,		  0, 0, P1_NONE, 10, 1, 10, 300, 9, 1, 0, 0, 0, 0, 4, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BRACERS("bracers of spell casting", "shining bracers",
	0, 1, 0,		  0, 0, P1_MANA_PERCENTAGE_BONUS, 10, 1, 10, 300, 9, 1, 10, 0, 0, 0, -7, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BRACERS("bracers of magic resistance", "ornamental bracers",//ENDMARKER 1 & 2
	0, 1, ANTIMAGIC,  0, 0, P1_NONE, 10, 1, 10, 300, 9, 1, 0, 0, 0, 0, 0, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),



/* shields */
SHIELD("small shield", None,
       1, 0, 0,          0, 0, 0, P1_NONE, 6, 0,  80,  3, 9, 0, 0, 0, 0, 0, 8, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SHIELD("elven shield", "blue and green shield",
       0, 0, 0,          0, 0, 0, P1_NONE, 2, 0,  140,  7, 8, 0, 0, 0, 0, 0, 6, MAT_WOOD, CLR_GREEN, O1_NONE, O2_ELVEN_ITEM, O3_NONE, PERMITTED_ALL),
SHIELD("Uruk-hai shield", "white-handed shield",
       0, 0, 0,          0, 0, 0, P1_NONE, 2, 0,  120,  7, 9, 0, 0, 0, 0, 0, 12, MAT_IRON, HI_METAL, O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL),
SHIELD("orcish shield", "red-eyed shield",
       0, 0, 0,          0, 0, 0, P1_NONE, 2, 0,  100,  7, 9, 0, 0, 0, 0, 0, 10, MAT_IRON, CLR_RED, O1_NONE, O2_ORCISH_ITEM, O3_NONE, PERMITTED_ALL),
SHIELD("large shield", None,
       1, 0, 1,          0, 0, 0, P1_NONE, 5, 0, 160, 10, 8, 0, 0, 0, 0, 0, 12, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SHIELD("dwarvish roundshield", "large round shield",
       0, 0, 0,          0, 0, 0, P1_NONE, 4, 0, 150, 10, 8, 0, 0, 0, 0, 0, 12, MAT_IRON, HI_METAL, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, PERMITTED_ALL),
SHIELD("shield of reflection", "polished silver shield",
       0, 1, 0, REFLECTING, 0, 0, P1_NONE, 3, 0, 70, 750, 8, 0, 0, 0, 0, 0, 10, MAT_SILVER, HI_SILVER, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),

WEAPONSHIELD("spiked shield", None,
	1, 0, 5, 130, 50, 
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, PIERCE, P_SPEAR, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONSHIELD("spiked silver shield", None,
	1, 0, 2, 130, 500, 
	AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, P1_NONE, PIERCE, P_SPEAR, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),


/* gloves */
/* These have their color but not material shuffled, so the MAT_IRON must
 * stay CLR_BROWN (== HI_LEATHER) even though it's normally either
 * HI_METAL or CLR_BLACK.  All have shuffled descriptions.
 */
GLOVES("leather gloves", "old gloves",
       0, 0,        0, 0, 0, P1_NONE, 8, 1, 10,  8, 9, 0, 0, 0, 0, 0, 2, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
GLOVES("gauntlets of fumbling", "padded gloves",
       0, 1, FUMBLING, 0, 0, P1_NONE, 8, 1, 10, 100, 9, 0, 0, 0, 0, 0, 12, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
GLOVES("gloves of haste", "brown gloves",
       0, 1, VERY_FAST, 0, 0, P1_NONE, 8, 1, 10, 100, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
GLOVES("gloves of spell casting", "deerskin gloves",
       0, 1,		0, 0, 0, P1_MANA_PERCENTAGE_BONUS, 8, 1, 10, 100, 9, 0, 20, 0, BONUS_TO_SPELL_CASTING, 16, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
WEAPONGLOVES("gauntlets of ogre power", "riding gloves",
	0, 1, 1, 8, 30, 100,
	AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 1, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE | IGNORE_SPE, STR18(100), 6, 0,
	0, 0, 0, P1_NONE, WHACK, MAT_IRON, CLR_BROWN,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
GLOVES("gauntlets of dexterity", "fencing gloves",
       0, 1,        0, 0, 0, P1_NONE, 8, 1, 10, 100, 9, 0, 0, 0, BONUS_TO_DEX, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* without shuffled appearances */
WEAPONGLOVES("spiked gauntlets", None,
	1, 0, 1, 5, 30, 30,
	AD_PHYS, 1, 4, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 8, 0,
	0, 0, 0, P1_NONE, PIERCE, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONGLOVES("silver gauntlets", None,
	1, 0, 1, 5, 30, 300,
	AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0,
	0, 0, 0, P1_NONE, WHACK, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONGLOVES("spiked silver gauntlets", None,
	1, 0, 1, 2, 30, 500,
	AD_PHYS, 1, 4, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 8, 0,
	0, 0, 0, P1_NONE, PIERCE, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),


/* boots */
BOOTS("low boots", "walking shoes",
      0, 0,          0, 0, 0, P1_NONE, 14, 2, 10,  8, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
//BOOTS("iron shoes", "hard shoes",
//      0, 0,          0, 0, 0, 7, 2, 60, 16, 8, 0, 0, 0, 0, 0, 2, MAT_IRON, HI_METAL, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, PERMITTED_ALL),
WEAPONBOOTS("iron shoes", "hard shoes",
	1, 0, 2, 7, 60, 16,
	AD_PHYS, 1, 3, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, WHACK, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
BOOTS("leather sandals", None,
      1, 0,          0, 0, 0, P1_NONE, 2, 1, 6,  4, 10, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BOOTS("cotton slippers", None,
      1, 0,          0, 0, 0, P1_NONE, 2, 1, 3,  4, 10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BOOTS("high boots", "jackboots",
      0, 0,          0, 0, 0, P1_NONE, 12, 2, 30, 12, 8, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
BOOTS("speed boots", "combat boots",
      0, 1,       VERY_FAST, 0, 0, P1_NONE, 12, 2, 30, 50, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BOOTS("water walking boots", "jungle boots",
      0, 1,   WATER_WALKING, 0, 0, P1_NONE, 12, 2, 15, 50, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BOOTS("jumping boots", "hiking boots",
      0, 1,    JUMPING, 0, 0, P1_NONE, 12, 2, 30, 50, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
BOOTS("elven boots", "mud boots",
      0, 1,    STEALTH, 0, 0, P1_NONE, 12, 2, 20,  8, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_ELVEN_ITEM, O3_NONE, PERMITTED_ALL),
WEAPONBOOTS("kicking boots", "buckled boots",
	0, 1, 2, 12, 60, 50,
	AD_PHYS, 1, 7, 0, 1, 7, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0,
	MAGICAL_KICKING, NO_POWER, NO_POWER, P1_NONE, WHACK, MAT_IRON, CLR_BROWN,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
/* CLR_BROWN for same reason as gauntlets of power */

BOOTS("fumble boots", "riding boots",
      0, 1,   FUMBLING, 0, 0, P1_NONE, 12, 2, 30, 30, 9, 0, 0, 0, 0, 0, 4, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
BOOTS("levitation boots", "snow boots",
      0, 1, LEVITATION, 0, 0, P1_NONE, 12, 2, 15, 30, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),

/* more without shuffled appearances */
WEAPONBOOTS("spiked boots", None,
	1, 0, 2, 5, 60, 20,
	AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, PIERCE, MAT_IRON, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONBOOTS("silver shoes", None,
	1, 0, 2, 4, 60, 200,
	AD_PHYS, 1, 3, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, WHACK, MAT_SILVER, HI_SILVER,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONBOOTS("spiked silver boots", None,
	1, 0, 2, 2, 60, 400,
	AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, 0,
	0, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, PIERCE, MAT_SILVER, HI_METAL,
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL, ALL_TARGETS),


#undef HELM
#undef CLOAK
#undef SHIELD
#undef SHIRT
#undef ROBE
#undef BRACERS
#undef GLOVES
#undef BOOTS
#undef ARMOR
#undef WEAPONSHIELD
#undef WEAPONGLOVES
#undef WEAPONBOOTS

/* rings ... */
#define RING(name,desc,prob,power,power2,power3,pflags,cost,mgc,spec,mohs,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), None, None,                                         \
           BITS(0, 0, spec == CHARGED_NOT_CHARGED ? 0 : 1 , 0, mgc, spec, 0, 0, 0,                    \
                HARDGEM(mohs), 0, 0, P_NONE, metal),                     \
           power, power2, power3, pflags, RING_CLASS, prob, MULTIGEN_SINGLE, 0, 1, cost,  \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattrs,attrbonus, splcastpen, 0, \
		   15, color, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
RING("adornment", "wooden", /* STARTMARKER FOR SHUFFLED RINGS */
    30, ADORNED, 0, 0, P1_NONE, 100, 1, CHARGED_RING_NORMAL, 2, 0, 0, BONUS_TO_CHA, 0, 0, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("gain strength", "granite",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_NORMAL, 7, 0, 0, BONUS_TO_STR, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("gain dexterity", "thin",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_NORMAL, 7, 0, 0, BONUS_TO_DEX, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("gain constitution", "opal",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_NORMAL, 7, 0, 0, BONUS_TO_CON, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("gain intelligence", "adamantium",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_NORMAL, 7, 0, 0, BONUS_TO_INT, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("gain wisdom", "polished",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_NORMAL, 7, 0, 0, BONUS_TO_WIS, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("power", "runed",
	10, 0, 0, 0, P1_NONE, 300, 1, CHARGED_RING_POWER, 7, 0, 0, BONUS_TO_ALLSTATS, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("increase accuracy", "clay",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_1_7, 4, 0, 0, BONUS_TO_HIT, 0, 0, MAT_MINERAL, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("increase damage", "coral",
	30, 0, 0, 0, P1_NONE, 150, 1, CHARGED_RING_1_7, 4, 0, 0, BONUS_TO_DAMAGE, 0, 0, MAT_MINERAL, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("protection", "black onyx",
	35, NO_POWER, 0, 0, P1_NONE, 100, 1, CHARGED_RING_NORMAL, 7, 0, 0, BONUS_TO_AC | BONUS_TO_MC, 0, 0, MAT_MINERAL, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("regeneration", "moonstone",
	35, REGENERATION, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  6, 0, 0, 0, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("replenishment", "crystal",
	30, ENERGY_REGENERATION, 0, 0, P1_NONE, 300, 1, CHARGED_NOT_CHARGED,  6, 0, 0, 0, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("the serpent god", "serpent-headed",
	10, 0, 0, 0, P1_MANA_PERCENTAGE_BONUS, 300, 1, CHARGED_NOT_CHARGED,  6, 50, 0, 0, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("fortitude", "immaculate",
	30, 0, 0, 0, P1_HP_PERCENTAGE_BONUS, 200, 1, CHARGED_NOT_CHARGED,  6,  0, 25, 0, 0, 0, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("searching", "tiger eye",
	30, SEARCHING, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  6, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("stealth", "jade",
	30, STEALTH, 0, 0, P1_NONE, 100, 1, CHARGED_NOT_CHARGED,  6, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("sustain ability", "bronze",
	30, FIXED_ABIL, 0, 0, P1_NONE, 100, 1, CHARGED_NOT_CHARGED,  4, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("levitation", "agate",
	30, LEVITATION, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  7, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("hunger", "topaz",
	30, HUNGER, 0, 0, P1_NONE, 100, 1, CHARGED_NOT_CHARGED,  8, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_CYAN, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
RING("aggravate monster", "sapphire",
	30, AGGRAVATE_MONSTER, 0, 0, P1_NONE, 150, 1, CHARGED_NOT_CHARGED,  9, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_BLUE, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
RING("conflict", "ruby",
	10, CONFLICT, 0, 0, P1_NONE, 300, 1, CHARGED_NOT_CHARGED,  9, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("warning", "diamond",
	30, WARNING, 0, 0, P1_NONE, 100, 1, CHARGED_NOT_CHARGED, 10, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("poison resistance", "pearl",
	30, POISON_RES, 0, 0, P1_NONE, 150, 1, CHARGED_NOT_CHARGED,  4, 0, 0, 0, 0, 0, MAT_BONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("fire resistance", "iron",
	30, FIRE_RES, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  5, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("cold resistance", "brass",
	30, COLD_RES, 0, 0, P1_NONE, 150, 1, CHARGED_NOT_CHARGED,  4, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, O1_COLD_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("shock resistance", "copper",
	30, SHOCK_RES, 0, 0, P1_NONE, 150, 1, CHARGED_NOT_CHARGED,  3, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("life protection", "platinum",
	25, DEATH_RES, DRAIN_RES, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  3, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("free action", "twisted",
	30, FREE_ACTION, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  6, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("slow digestion", "steel",
	30, SLOW_DIGESTION, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  8, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("teleportation", "silver",
	30, TELEPORT, 0, 0, P1_NONE, 200, 1, CHARGED_NOT_CHARGED,  3, 0, 0, 0, 0, 0, MAT_SILVER, HI_SILVER, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
RING("teleport control", "gold",
	20, TELEPORT_CONTROL, 0, 0, P1_NONE, 300, 1, CHARGED_NOT_CHARGED,  3, 0, 0, 0, 0, 0, MAT_GOLD, HI_GOLD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("polymorph", "ivory",
	30, POLYMORPH, 0, 0, P1_NONE, 300, 1, CHARGED_NOT_CHARGED,  4, 0, 0, 0, 0, 0, MAT_BONE, CLR_WHITE, O1_POLYMORPH_RESISTANT, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
RING("polymorph control", "emerald",
	20, POLYMORPH_CONTROL, 0, 0, P1_NONE, 300, 1, CHARGED_NOT_CHARGED,  8, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_BRIGHT_GREEN, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("invisibility", "wire",
	25, INVISIBILITY, 0, 0, P1_NONE, 150, 1, CHARGED_NOT_CHARGED,  5, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("see invisible", "engagement",
	30, SEE_INVISIBLE, 0, 0, P1_NONE, 150, 1, CHARGED_NOT_CHARGED,  5, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
RING("protection from shape changers", "shiny", /* ENDMARKER FOR SHUFFLED RINGS */
	30, PROT_FROM_SHAPE_CHANGERS, 0, 0, P1_NONE, 100, 1, CHARGED_NOT_CHARGED,  5, 0, 0, 0, 0, 0, MAT_IRON, CLR_BRIGHT_CYAN, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),

/* without shuffled apprearances */
RING("supreme power", "golden runed", /* Base item for the One Ring */
    0, INVISIBILITY, CLAIRVOYANT, SEE_INVISIBLE, P1_MANA_PERCENTAGE_BONUS, 10000, 1, CHARGED_NOT_CHARGED,  5, 100, 0, BONUS_TO_ALLSTATS, 5, -20, MAT_GOLD, HI_GOLD,
	O1_INDESTRUCTIBLE | O1_DISINTEGRATION_RESISTANT | O1_FIRE_RESISTANT | O1_COLD_RESISTANT | O1_LIGHTNING_RESISTANT,
	O2_NONE, 
	O3_NO_WISH | O3_NO_GENERATION | O3_READABLE, PERMITTED_ALL),
#undef RING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,prob,power,power2,power3,pflags,manabonus,hpbonus,bonusattrs,attrbonus,splcastpen,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), None, None,                                          \
           BITS(0, 0, 0, 0, 1, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, MAT_IRON),        \
           power, power2, power3, pflags, AMULET_CLASS, prob, MULTIGEN_SINGLE, 0, 5, 150, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, manabonus, hpbonus, bonusattrs, attrbonus, splcastpen, 0, \
		   20, HI_METAL, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
AMULET("amulet of ESP",                "circular", 90,	TELEPAT, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet of life saving",       "spherical", 75,  LIFESAVED, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet of strangulation",          "oval", 100, STRANGLED, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
AMULET("amulet of restful sleep",    "triangular", 100, SLEEPY, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
AMULET("amulet versus poison",        "pyramidal", 90,	POISON_RES, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet versus life draining",   "cubical", 75,	DRAIN_RES, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet of change",               "square", 100, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
AMULET("amulet of unchanging",          "concave", 45,  UNCHANGING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet of reflection",        "hexagonal", 75,  REFLECTING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet of mana",		        "convex",  75,	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 75, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("demon blood talisman",	   "tetrahedral",  35,	NO_POWER, NO_POWER, NO_POWER, P1_MANA_PERCENTAGE_BONUS, 100, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), //doubles mana capacity
AMULET("periapt of vitality",	        "linear",  75,	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 25, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet of magical breathing", "octagonal", 65,  MAGICAL_BREATHING, 0, 0, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
AMULET("amulet",					"cylindrical",  0,  NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Base item for artifact amulets */
/* fixed descriptions; description duplication is deliberate;
 * fake one must come before real one because selection for
 * description shuffling stops when a non-magic amulet is encountered
 */
OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
           "Amulet of Yendor"), None, None,
       BITS(0, 0, 1, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, 0, MAT_PLASTIC),
       0, 0, 0, P1_NONE, AMULET_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   1, HI_METAL, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),
OBJECT(OBJ("Amulet of Yendor", /* note: description == name */
           "Amulet of Yendor"), None, None,
       BITS(0, 0, 1, 0, 1, CHARGED_NOT_CHARGED, 1, 1, 0, 0, 0, 0, 0, MAT_MITHRIL),
       0, 0, 0, P1_NONE, AMULET_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 30000,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   20, HI_METAL, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE, O2_NONE, O3_NONE),
#undef AMULET

/* miscellaneous (magic) items */
#define MISCELLANEOUSITEM(name,desc,sub,specialworntext,shortdesc,kn,magic,mergeable,charged,prob,cost,wt,power,power2,power3,pflags,manabonus,hpbonus,bonusattrs,attrbonus,splcastpen,nut,material,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), specialworntext, shortdesc,                                          \
           BITS(kn, mergeable, charged, 0, magic, charged, 0, 0, 0, 0, 0, sub, 0, material),        \
           power, power2, power3, pflags, MISCELLANEOUS_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost,  \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, manabonus, hpbonus, bonusattrs, attrbonus, splcastpen, 0, \
		   nut, color, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
MISCELLANEOUSITEM("brooch of shielding", "golden brooch", MISC_BROOCH, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 50, 150, 5,
	NO_POWER, 0, 0, P1_NONE, 0, 0, BONUS_TO_MC, 5, 0,
	20, MAT_GOLD, HI_GOLD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("nose ring of bull strength", "golden nose ring", MISC_NOSERING, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 50, 200, 2, //STARTMARKER FOR NOSE RING WISH-CLASS, SHUFFLE
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR18(76), 0,
	50, MAT_GOLD, HI_GOLD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("nose ring of bullheadedness", "silver nose ring", MISC_NOSERING, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 50, 200, 2,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_WIS | SETS_FIXED_ATTRIBUTE | FIXED_IS_MAXIMUM, 3, 0,
	50, MAT_SILVER, HI_SILVER, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("nose ring of cerebral safeguarding", "bronze nose ring", MISC_NOSERING, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 50, 200, 2, //ENDMARKER FOR NOSE RING WISH-CLASS, SHUFFLE
	BRAIN_PROTECTION, 0, 0, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("headband of intellect", "shining blue headband", MISC_HEADBAND, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 30, 200, 3,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_INT | SETS_FIXED_ATTRIBUTE, 19, 0,
	50, MAT_CLOTH, CLR_BLUE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("golden earrings", None, MISC_EARRINGS, None, None,
	1, 0, 0, CHARGED_NOT_CHARGED, 10, 200, 2,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_CHA, 1, 0,
	50, MAT_GOLD, HI_GOLD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_GENDER_FEMALE),
MISCELLANEOUSITEM("ioun stone of protection", "red ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10, //STARTMARKER FOR IOUNSTONE WISH-CLASS
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_MC, 4, 0,
	50, MAT_MINERAL, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of spell mastery", "blue ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 50, 0, 0, 0, -20,
	50, MAT_MINERAL, CLR_BLUE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of magic resistance", "brown ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MINERAL, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of strength", "pink ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR, 2, 0,
	50, MAT_MINERAL, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of dexterity", "green ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_DEX, 2, 0,
	50, MAT_MINERAL, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of constitution", "purple ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_CON, 2, 0,
	50, MAT_MINERAL, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of intelligence", "yellow ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 50,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_INT, 2, 0,
	50, MAT_MINERAL, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of wisdom", "white ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_WIS, 2, 0,
	50, MAT_MINERAL, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of charisma", "black ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_CHA, 2, 0,
	50, MAT_MINERAL, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of experience", "bright blue ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_EXPERIENCE, 3, 0,
	50, MAT_MINERAL, CLR_BRIGHT_BLUE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of awareness", "gray ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	WARNING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MINERAL, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of regeneration", "orange ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	REGENERATION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MINERAL, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of incessant hunger", "bright green ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	HUNGER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MINERAL, CLR_BRIGHT_GREEN, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of restful sleep", "transparent ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10,
	SLEEPY, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MINERAL, CLR_BRIGHT_CYAN, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of sustenance", "cyan ioun stone", MISC_IOUN_STONE, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 200, 10, //ENDMARKER FOR IOUNSTONE WISH-CLASS
	SLOW_DIGESTION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MINERAL, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("wings of flying", "artificial wings", MISC_WINGS, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 10, 200, 50,
	FLYING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	50, MAT_MODRONITE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("lenses", "gold-framed eyeglasses", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 40, 80, 3, //STARTMARKER FOR EYEGLASSES WISH-CLASS AND SHUFFLED EYEGLASSES
	ENHANCED_VISION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_GLASS, HI_GLASS, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("eyeglasses of hallucination", "oval eyeglasses", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 40, 150, 3,
	HALLUC, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_GLASS, HI_GLASS, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("eyeglasses of awkwardness", "square eyeglasses", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 40, 150, 3,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE | FIXED_IS_MAXIMUM, 3, 0,
	20, MAT_GLASS, HI_GLASS, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("eyeglasses of see invisible", "round eyeglasses", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 40, 150, 3,  //ENDMARKER FOR SHUFFLED EYEGLASSES
	SEE_INVISIBLE, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_GLASS, HI_GLASS, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("sunglasses", "shaded eyeglasses", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 20, 80, 3,   //ENDMARKER FOR EYEGLASSES WISH-CLASS
	FLASH_RES, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_GLASS, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("goggles of night", "leather-framed goggles", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 40, 100, 3,  //STARTMARKER FOR GOGGLES WISH-CLASS, SHUFFLED GOGGLES
	INFRAVISION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_PLASTIC, HI_GLASS, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("goggles of eye protection", "transparent goggles", MISC_EYEGLASSES, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 40, 100, 3, //ENDMARKER FOR GOGGLES WISH-CLASS, ENDMARKER FOR SHUFFLED GOGGLES
	EYE_PROTECTION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_PLASTIC, HI_GLASS, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("expensive watch", None, MISC_WRIST_WATCH, "on left wrist", None,
	1, 0, 0, CHARGED_NOT_CHARGED, 10, 200, 6,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CHA, 1, 0,
	50, MAT_GOLD, HI_GOLD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_GENDER_MALE),
MISCELLANEOUSITEM("leather belt", "old belt", MISC_BELT, None, None,
	0, 0, 0, CHARGED_NOT_CHARGED, 50, 5, 5, //STARTMARKER FOR BELTS
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of change", "brown belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 50, 250, 5,
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of dwarvenkind", "sturdy belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_GENERAL, 40, 250, 5,
	POISON_RES, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CON, 0, 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of fortitude", "runed belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 40, 250, 5,
	0, 0, 0, P1_HP_PERCENTAGE_BONUS, 0, 50, 0, 0, 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of hill giant strength", "ornamental belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 25, 250, 5,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(20), 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of stone giant strength", "heavy belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 20, 250, 5,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(21), 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of frost giant strength", "decorative belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 15, 250, 5,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(22), 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of fire giant strength", "coarse belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 10, 250, 5,
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(23), 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of storm giant strength", "rudimentary belt", MISC_BELT, None, None,
	0, 1, 0, CHARGED_NOT_CHARGED, 5, 250, 5, //ENDMARKER FOR BELTS
	0, 0, 0, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(24), 0,
	20, MAT_CLOTH, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),


#undef MISCELLANEOUSITEM

/* tools ... */
/* tools with weapon characteristics come last */
#define TOOL(name,desc,shortdesc,kn,mrg,mgc,chg,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,attrbonus,splcastpen,power1,power2,power3,pflags,mat,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), None, shortdesc,                                           \
           BITS(kn, mrg, chg, 0, mgc, chg, 0, 0, 0, 0, 0, 0, P_NONE, mat), \
           power1, power2, power3, pflags,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattr, attrbonus, splcastpen, 0, \
		   wt, color, 0, 0, cooldown, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
#define SPELLTOOL(name,desc,shortdesc,kn,mrg,mgc,chg,prob,wt,cost,dir,dirsubtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus,cooldown,mat,color,flags,flags2,flags3) \
    OBJECT(OBJ(name, desc), None, shortdesc,                                           \
           BITS(kn, mrg, chg ? 1 : 0, 0, mgc, chg, 0, 0, 0, 0, dir, 0, P_NONE, mat), \
           0, 0, 0, P1_NONE,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
		   AD_PHYS, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   wt, color, dirsubtype, 0, cooldown, 0, PERMITTED_ALL, ALL_TARGETS, O1_WAND_LIKE_TOOL | flags, flags2, flags3)
#define CONTAINER(name,desc,shortdesc,kn,mgc,chg,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,attrbonus,pflags,mat,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), None, shortdesc,                                           \
           BITS(kn, 0, chg ? 1 : 0, 1, mgc, chg, 0, 0, 0, 0, 0, 0, P_NONE, mat),   \
           0, 0, 0, pflags,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattr, attrbonus, 0, 0, \
		   wt, color, 0, 0, cooldown, 0, powconfermask, ALL_TARGETS, flags, O2_CONTAINER | flags2, flags3)
#define WEPTOOL(name,desc,shortdesc,kn,mgc,bi,prob,wt,cost,sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, aflags, critpct, hitbon,splcastpen,sub,skill,cooldown,mat,clr,flags,flags2,flags3,powconfermask)\
    OBJECT(OBJ(name, desc), None, shortdesc,                                           \
           BITS(kn, 0, 1, 0, mgc, CHARGED_GENERAL, 0, 0, bi, 0, hitbon, sub, skill, mat),    \
           0, 0, 0, P1_NONE,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
		   AD_PHYS, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, 0, 0, 0, 0, aflags, critpct, \
		   hitbon, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   wt, clr, 0, 0, cooldown, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)

/* containers */
CONTAINER("large box",       None, None,
	1, 0, CHARGED_NOT_CHARGED, 40, 350,   8, 0, 0, 0, 0, 0, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_CONTAINER_BOX, O3_NONE, PERMITTED_ALL), //STARTMARKER 1
CONTAINER("chest",           None, None,
	1, 0, CHARGED_NOT_CHARGED, 30, 600,  16, 0, 0, 0, 0, 0, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_CONTAINER_BOX, O3_NONE, PERMITTED_ALL),
CONTAINER("golden chest",           None, None,
	1, 0, CHARGED_NOT_CHARGED, 0, 600, 1000, 0, 0, 0, 0, 0, P1_NONE, MAT_WOOD, HI_GOLD,
	O1_FIRE_RESISTANT | O1_INDESTRUCTIBLE | O1_DISINTEGRATION_RESISTANT | O1_NOT_CURSEABLE,
	O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG | O2_GENERATED_BLESSED, O3_NO_WISH | O3_NO_GENERATION, PERMITTED_ALL),
CONTAINER("ice box",         None, None,
	1, 0, CHARGED_NOT_CHARGED,  5, 900,  42, 0, 0, 0, 0, 0, P1_NONE, MAT_PLASTIC, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CONTAINER("bookshelf",		 None, None,
	1, 0, CHARGED_NOT_CHARGED,  0, 1600, 36, 0, 0, 0, 0, 0, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CONTAINER("backpack",		 None, None,
	1, 0, CHARGED_NOT_CHARGED,  5,  15,   5, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CONTAINER("leather bag",	 "bag", None,
	0, 0, CHARGED_NOT_CHARGED,  5,  15,  25, 0, 0, 0, 0, 0, P1_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CONTAINER("oriental silk sack", "bag", None,
	0, 0, CHARGED_NOT_CHARGED,  5,  3,  50, 0, 0, 0, 0, 0, P1_NONE, MAT_SILK, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CONTAINER("expensive handbag", "bag", None,
	0, 0, CHARGED_NOT_CHARGED, 2, 5, 500, 0, 0, 0, BONUS_TO_CHA, 2, P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED, MAT_SILK, HI_GOLD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_GENDER_FEMALE),
CONTAINER("sack",           "bag", None,
	0, 0, CHARGED_NOT_CHARGED, 10,  10,   5, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
CONTAINER("oilskin sack",   "bag", None,
	0, 0, CHARGED_NOT_CHARGED,  5,  10, 100, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* magic bags start here */
CONTAINER("bag of holding", "bag", "Reduces the weight of contents by half",
	0, 1, CHARGED_NOT_CHARGED, 20, 15, 100, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, PERMITTED_ALL), //STARTMARKER 2
CONTAINER("bag of wizardry", "bag", "Reduces the weight of spellbooks, scrolls, reagents, and wands to 1/8",
	0, 1, CHARGED_NOT_CHARGED, 15, 15, 100, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, PERMITTED_ALL),
CONTAINER("bag of treasure hauling", "bag", "Reduces the weight of coins, gems, and other treasure to 1/32",
	0, 1, CHARGED_NOT_CHARGED, 15, 15, 100, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, PERMITTED_ALL),
CONTAINER("quiver of infinite arrows", "bag", None,
	0, 1, CHARGED_NOT_CHARGED, 4, 15, 100, 300, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_ELEMENTAL_ENCHANTABLE, PERMITTED_ALL),
CONTAINER("pouch of endless bolts", "bag", None,
	0, 1, CHARGED_NOT_CHARGED, 4, 15, 100, 300, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_NONE, O3_ELEMENTAL_ENCHANTABLE, PERMITTED_ALL),
CONTAINER("bag of tricks", "bag", None,
	0, 1, CHARGED_BAG_OF_TRICKS, 20, 15, 100, 0, 0, 0, 0, 0, P1_NONE, MAT_CLOTH, HI_CLOTH, O1_NONE, O2_CONTAINER_MAGIC_BAG, O3_NONE, PERMITTED_ALL), //ENDMARKER 1&2
#undef CONTAINER

/* lock opening tools */
TOOL("skeleton key",       "key", None, 0, 0, 0, CHARGED_NOT_CHARGED, 80,  2, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("master key",		   "key", None, 0, 0, 0, CHARGED_NOT_CHARGED,  0, 2, 10, 0, 0, 0, 0, 0, 0, /* base item for master key of thievery */
	ENHANCED_UNTRAP, NO_POWER, NO_POWER, P1_POWER_1_APPLIES_WHEN_CARRIED, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, PERMITTED_ROLE_ROGUE),
TOOL("lock pick",           None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 60,  2, 20, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("credit card",         None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 15,  1, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_PLASTIC, CLR_WHITE, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),

/* light sources */
TOOL("tallow candle",   "candle", None, 0, 1, 0, CHARGED_NOT_CHARGED, 18,  1, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WAX, CLR_WHITE, O1_NONE, O2_CANDLE, O3_IGNITABLE | O3_RELATIVE_AGE, PERMITTED_ALL),
TOOL("wax candle",      "candle", None, 0, 1, 0, CHARGED_NOT_CHARGED,  9,  1, 20, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WAX, CLR_WHITE, O1_NONE, O2_CANDLE, O3_IGNITABLE | O3_RELATIVE_AGE, PERMITTED_ALL),
TOOL("magic candle",	"candle", None, 0, 1, 0, CHARGED_NOT_CHARGED,  3,  1, 20, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WAX, CLR_WHITE, O1_NONE, O2_CANDLE, O3_IGNITABLE | O3_RELATIVE_AGE, PERMITTED_ALL),
TOOL("brass lantern",       None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 30, 30, 12, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_COPPER, CLR_YELLOW, O1_NONE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE, PERMITTED_ALL),
TOOL("oil lamp",          "lamp", None, 0, 0, 0, CHARGED_NOT_CHARGED, 40, 20, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_COPPER, CLR_YELLOW, O1_NONE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE, PERMITTED_ALL),
TOOL("magic lamp",        "lamp", None, 0, 0, 1, CHARGED_NOT_CHARGED, 15, 20, 50, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_COPPER, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE /*intentionally not O3_IGNITABLE*/, PERMITTED_ALL),

/* other tools */
TOOL("expensive camera",    None, None, 1, 0, 0, CHARGED_MAGIC_MARKER, 15, 12,200, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_PLASTIC, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("mirror",   "looking glass", None, 0, 0, 0, CHARGED_NOT_CHARGED, 35, 13, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_GLASS, HI_SILVER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("magic mirror", "looking glass", None, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 13, 10, 0, 30, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_MANA_BONUS_APPLIES_WHEN_CARRIED | P1_MANA_PERCENTAGE_BONUS, MAT_GLASS, HI_SILVER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("holy symbol", "religious symbol", "Religious symbol that can be applied to turn undead", 0, 0, 1, CHARGED_NOT_CHARGED, 10, 10,100, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_SILVER, HI_SILVER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("crystal ball", "glass orb", None, 0, 0, 1, 1, 15,150, 60, 100, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_GLASS, HI_GLASS, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("blindfold",           None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 50,  2, 20, 0, 0, 0, 0, 0, 0, 
	BLINDFOLDED, NO_POWER, NO_POWER, P1_NONE, MAT_CLOTH, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("towel",               None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 50,  2, 50, 0, 0, 0, 0, 0, 0, 
	BLINDFOLDED, NO_POWER, NO_POWER, P1_NONE, MAT_CLOTH, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("saddle",              None, None, 1, 0, 0, CHARGED_NOT_CHARGED,  5,200,150, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("leash",               None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 50, 12, 20, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("stethoscope",         None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 25,  4, 75, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("tinning kit",         None, None, 1, 0, 0, CHARGED_MAGIC_MARKER, 15,100, 30, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("tin opener",          None, None, 1, 0, 0, CHARGED_NOT_CHARGED, 30,  4, 30, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("can of grease",       None, None, 1, 0, 0, CHARGED_CAN_OF_GREASE, 15, 15, 20, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
TOOL("figurine",            None, None, 1, 0, 1, CHARGED_NOT_CHARGED, 0, 50, 80, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, PERMITTED_ALL),
        /* FIGURINE REMOVED FROM THE GAME -- JG -- monster type specified by obj->corpsenm */
TOOL("magic marker",        None, None, 1, 0, 1, CHARGED_MAGIC_MARKER, 25,  2, 50, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_PLASTIC, CLR_RED, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),

/* traps */
TOOL("land mine",           None, None, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 300,180, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("beartrap",            None, None, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 200, 60, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* instruments;
   "If tin whistles are made out of tin, what do they make foghorns out of?" */
TOOL("tin whistle",    "whistle", None, 0, 0, 0, CHARGED_NOT_CHARGED,100, 2, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("magic whistle",  "whistle", None, 0, 0, 1, CHARGED_NOT_CHARGED, 45, 2, 10, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("wooden flute",     "flute", None, 0, 0, 0, CHARGED_NOT_CHARGED,  4, 5, 12, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("magic flute",      "flute", None, 0, 0, 1, CHARGED_HORN_NORMAL,  2, 5, 36, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("tooled horn",       "horn", None, 0, 0, 0, CHARGED_NOT_CHARGED,  5, 18, 15, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_BONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SPELLTOOL("frost horn",   "horn", None, 0, 0, 1, CHARGED_HORN_NORMAL,  2, 18, 50, RAY, RAY_WND_COLD, 6, 6, 0, 0, 0, 0, 5, MAT_BONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELLTOOL("fire horn",    "horn", None, 0, 0, 1, CHARGED_HORN_NORMAL,  2, 18, 50, RAY, RAY_WND_FIRE, 6, 6, 0, 0, 0, 0, 5, MAT_BONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
TOOL("horn of plenty",    "horn", None, 0, 0, 1, CHARGED_BAG_OF_TRICKS,  2, 18, 50, 300, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_BONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
        /* horn, but not an instrument */
TOOL("wooden harp",       "harp", None, 0, 0, 0, CHARGED_NOT_CHARGED,  4, 30, 50, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("magic harp",        "harp", None, 0, 0, 1, CHARGED_HORN_NORMAL,  2, 30, 50, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("bell",                None, None, 1, 0, 0, CHARGED_NOT_CHARGED,  2, 30, 50, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("bugle",               None, None, 1, 0, 0, CHARGED_NOT_CHARGED,  4, 10, 15, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("leather drum",      "drum", None, 0, 0, 0, CHARGED_NOT_CHARGED,  4, 25, 25, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
TOOL("drum of earthquake","drum", None, 0, 0, 1, CHARGED_HORN_NORMAL,  2, 25, 25, 0, 0, 0, 0, 0, 0, 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_LEATHER, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* tools useful as weapons */
WEPTOOL("pick-axe", None, None,
	1, 0, 0, 15, 75, 50,  
	1, 6, 0, 1, 3, 0, A1_NONE, 0, 
	WHACK, 0, WEP_PICK_AXE, P_PICK_AXE, 0, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
WEPTOOL("shovel", None, None,
	1, 0, 0,  5, 60, 20,
	1, 5, 0, 1, 3, 0, A1_NONE, 0, 
	SLASH, 0, WEP_SHOVEL, P_PICK_AXE, 0, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
WEPTOOL("golf club", "club-headed metal rod", None,
	0, 0, 0,  1, 12, 50,
	1, 3, 0, 1, 2, 0, A1_NONE, 0, 
	WHACK, 0, WEP_CLUB, P_BLUDGEONING_WEAPON, 0, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
WEPTOOL("grappling hook", "iron hook", None,
	0, 0, 0,  4, 30,  50,
	1, 2, 0, 1, 6, 0, A1_NONE, 0, WHACK, 0, WEP_HOOK, P_THROWN_WEAPON, 0, MAT_IRON, HI_METAL, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
WEPTOOL("unicorn horn", None, None,
	1, 1, 1,  0, 20, 100, 
	1, 12, 0, 1, 12, 0, A1_NONE, 0, 
	PIERCE, 0, WEP_UNICORN_HORN, P_SPEAR, 100, MAT_BONE, CLR_WHITE, 
	O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
        /* 3.4.1: unicorn horn left classified as "magic" */
/* two unique tools;
 * not artifacts, despite the comment which used to be here
 */
OBJECT(OBJ("Candelabrum of Invocation", "candelabrum"),None, None,
       BITS(0, 0, 1, 0, 1, CHARGED_NOT_CHARGED, 1, 1, 0, 0, 0, 0, P_NONE, MAT_GOLD),
       0, 0, 0, P1_NONE, TOOL_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 5000,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   200, HI_GOLD, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE),
OBJECT(OBJ("Bell of Opening", "silver bell"), None, None,
       BITS(0, 0, 1, 0, 1, CHARGED_ALWAYS_3, 1, 1, 0, 0, 0, 0, P_NONE, MAT_SILVER),
       0, 0, 0, P1_NONE, TOOL_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 5000,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   50, HI_SILVER, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE, O2_NONE, O3_NONE),
#undef TOOL
#undef WEPTOOL

/* Comestibles ... */
#define FOOD(name, desc, sub, known, prob, multigen, delay, wt, unk, material, ediblesubtype, edibleeffect, acquireprob, durdice, durdiesize, durplus, nutrition, color, flags, flags2, flags3, powconfermask)         \
    OBJECT(OBJ(name, desc), None, None,                                     \
           BITS(known, 1, unk, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, sub, P_NONE, material), \
		   0, 0, 0, 0, FOOD_CLASS, prob, multigen, delay, wt, nutrition / 20 + 5,  \
		   0, 0, 0, 0, durdice, durdiesize, durplus, 0, 0, 0, 0, A1_NONE, acquireprob, \
		   0, 0, 0, 0, ediblesubtype, edibleeffect, 0, 0, 0, 0, 0, 0, \
           nutrition, color, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)

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
FOOD("tripe ration",		None, FOODTYPE_RATION, 1,	
	140, MULTIGEN_SINGLE, 2, 10, 0, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	200, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("corpse",				None, FOODTYPE_CORPSE, 1,	  
	0, MULTIGEN_SINGLE, 1,  0, 0, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	0, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("egg",					None, FOODTYPE_EGG, 1,	 
	85, MULTIGEN_SINGLE, 1,  1, 1, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_EGG, 100, 0, 0, 0, 
	80, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("meatball",			None, FOODTYPE_MEAT, 1,	  
	0, MULTIGEN_SINGLE, 1,  1, 0, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	5, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("meat stick",			None, FOODTYPE_MEAT, 1,	  
	0, MULTIGEN_SINGLE, 1,  1, 0, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	5, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("huge chunk of meat",	None, FOODTYPE_MEAT, 1,	  
	0, MULTIGEN_SINGLE, 20, 400, 0, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	2000, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* special case because it's not mergable */
OBJECT(OBJ("meat ring", None), None, None,
       BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FOODTYPE_MEAT, 0, MAT_FLESH),
       0, 0, 0, P1_NONE, FOOD_CLASS, 0, MULTIGEN_SINGLE, 1, 5, 1,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   5, CLR_BROWN, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),
/* pudding 'corpses' will turn into these and combine;
   must be in same order as the pudding monsters */
FOOD("glob of gray ooze",		"gray glob",  FOODTYPE_GLOB, 0, 
	0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	20, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("glob of brown pudding",	"brown glob", FOODTYPE_GLOB, 0,	
	0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	20, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("glob of green slime",		"green glob", FOODTYPE_GLOB, 0,	
	0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	20, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("glob of black pudding",	"black glob", FOODTYPE_GLOB, 0,	
	0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	20, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* fruits & veggies */
FOOD("kelp frond", None,		FOODTYPE_KELP,		1,  
	0, MULTIGEN_SINGLE,		1,  1, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	30, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("eucalyptus leaf", None,	FOODTYPE_LEAF,		1, 
	10, MULTIGEN_SINGLE,		1,  1, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_CURE_SICKNESS, 100, 0, 0, 0, 
	30, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("handful of nuts", None,	FOODTYPE_NUTS,		1, 
	10, MULTIGEN_1D6,		1,	2, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	100, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("apple", None,				FOODTYPE_FRUIT,		1, 
	10, MULTIGEN_1D2,		1,  5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_APPLE, 100, 0, 0, 0, 
	150, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("orange", None,			FOODTYPE_FRUIT,		1, 
	10, MULTIGEN_1D2,		1,  5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	150, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("pear", None,				FOODTYPE_FRUIT,		1, 
	10, MULTIGEN_1D2,		1,  5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	150, CLR_BRIGHT_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("melon", None,				FOODTYPE_FRUIT,		1, 
	10, MULTIGEN_SINGLE,		1,  80, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	800, CLR_BRIGHT_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("carrot", None,			FOODTYPE_VEGETABLE, 1, 
	10, MULTIGEN_1D3,		1,  3, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_CURE_BLIDNESS, 100, 0, 0, 0, 
	100, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

FOOD("handful of spinach leaves", None, FOODTYPE_LEAF, 1, 
	2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_GAIN_STRENGTH, 100, 0, 0, 0, 
	100, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("banana", None, FOODTYPE_FRUIT, 1, 
	2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_GAIN_DEXTERITY, 100, 0, 0, 0, 
	150, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("pomegranate", None, FOODTYPE_FRUIT, 1, 
	2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_GAIN_CONSTITUTION, 100, 0, 0, 0, 
	150, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("avocado", None, FOODTYPE_FRUIT, 1, 
	2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_GAIN_CHARISMA, 100, 0, 0, 0, 
	150, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* shuffled mushrooms */
FOOD("champignon", "brown mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2,	1, 3, 1, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0,
	100, CLR_BROWN, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("penny bun", "black mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2,	1, 3, 1, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0,
	100, CLR_BLACK, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("chanterelle", "golden mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0,
	100, CLR_YELLOW, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("fly agaris", "red mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_POISONOUS, EDIBLE_NO_EFFECT, 0, 0, 0, 0,
	100, CLR_RED, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("death cap", "pale white mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_DEADLY_POISONOUS, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	100, CLR_WHITE, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("mana mushroom", "violet mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_NORMAL, ENERGY_REGENERATION, 100, 1, 50, 50, 
	100, CLR_MAGENTA, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("healing mushroom", "orange mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_NORMAL, REGENERATION,100, 1, 50, 50, 
	100, CLR_ORANGE, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("panther cap", "gray mushroom", FOODTYPE_MUSHROOM, 0, 
	3, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_NORMAL, BLIND_TELEPAT, 100, 0, 0, 0, 
	100, CLR_GRAY, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
FOOD("magic mushroom", "green mushroom", FOODTYPE_MUSHROOM, 0, 
	5, MULTIGEN_1D2, 1, 3, 1, MAT_VEGGY, EDIBLE_HALLUCINATING, EDIBLE_NO_EFFECT, 0, 1, 100, 100, 
	100, CLR_GREEN, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES, PERMITTED_ALL),
/* end of suffled mushrooms */

/* name of slime mold is changed based on player's OPTION=fruit:something
   and bones data might have differently named ones from prior games */
FOOD("slime mold", None, FOODTYPE_MOLD, 1, 
	65, MULTIGEN_SINGLE,	1,  5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	300, HI_ORGANIC, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

/* people food */
FOOD("lump of royal jelly", None, FOODTYPE_JELLY, 1, 
	0, MULTIGEN_SINGLE,	1,  2, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_ROYAL_JELLY, 100, 0, 0, 0, 
	200, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("cream pie", None, FOODTYPE_CAKE, 1, 
	15, MULTIGEN_SINGLE,	1, 10, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	150, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("candy bar", None, FOODTYPE_CANDY, 1, 
	15, MULTIGEN_SINGLE,	1,  2, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	150, CLR_BROWN, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
FOOD("fortune cookie", None, FOODTYPE_CAKE, 1, 
	32, MULTIGEN_SINGLE,	1,  1, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_READ_FORTUNE, 0, 0, 0, 0, 
	50, CLR_YELLOW, O1_NONE, O2_NONE, O3_READABLE, PERMITTED_ALL),
FOOD("pancake", None, FOODTYPE_CAKE, 1, 
	22, MULTIGEN_SINGLE,	2,  2, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	250, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("lembas wafer", None, FOODTYPE_BREAD, 1, 
	30, MULTIGEN_SINGLE,	2,  5, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	1000, CLR_WHITE, O1_NONE, O2_NONE, O3_NONROTTING_FOOD, PERMITTED_ALL),
FOOD("cram ration", None, FOODTYPE_RATION, 1, 
	30, MULTIGEN_SINGLE,	3, 15, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	900, HI_ORGANIC, O1_NONE, O2_NONE, O3_NONROTTING_FOOD, PERMITTED_ALL),
FOOD("food ration", None, FOODTYPE_RATION, 1, 
	370, MULTIGEN_SINGLE,	5, 20, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	800, HI_ORGANIC, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("K-ration", None, FOODTYPE_RATION, 1, 
	0, MULTIGEN_SINGLE,	1, 10, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	525, HI_ORGANIC, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
FOOD("C-ration", None, FOODTYPE_RATION, 1, 
	0, MULTIGEN_SINGLE,	1, 10, 0, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	450, HI_ORGANIC, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
/* tins have type specified by obj->spe (+1 for spinach, other implies
   flesh; negative specifies preparation method {homemade,boiled,&c})
   and by obj->corpsenm (type of monster flesh) */
FOOD("tin", None, FOODTYPE_TIN, 1, 
	75, MULTIGEN_SINGLE,	0, 10, 1, MAT_METAL, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 0, 0, 0, 
	0, HI_METAL, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
#undef FOOD

/* potions ... */
#define POTION(name,desc,mgc,power,prob,cost,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), None, None,                                           \
           BITS(0, 1, 0, 0, mgc, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, MAT_GLASS),      \
           power, 0, 0, P1_NONE, POTION_CLASS, prob, MULTIGEN_SINGLE, 0, 12, cost,  \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   10, color, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
POTION("gain ability",           "ruby",  1, 0, 42, 400, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("restore ability",        "pink",  1, 0, 40, 100, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("confusion",            "orange",  1, CONFUSION, 42, 100, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("blindness",            "yellow",  1, BLINDED, 30, 150, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("paralysis",           "emerald",  1, 0, 42, 300, CLR_BRIGHT_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("speed",            "dark green",  1, VERY_FAST, 32, 200, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("levitation",             "cyan",  1, LEVITATION, 32, 200, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("hallucination",      "sky blue",  1, HALLUC, 40, 100, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("invisibility", "brilliant blue",  1, INVISIBILITY, 40, 150, CLR_BRIGHT_BLUE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("see invisible",       "magenta",  1, SEE_INVISIBLE, 42, 50, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("magic resistance",		 "gray",  1, ANTIMAGIC, 30, 100, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("healing",          "purple-red",  1, 0, 57, 100, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("extra healing",          "puce",  1, 0, 47, 150, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("greater healing",	 "deep red",  1, 0, 20, 200, CLR_RED, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("full healing",          "black",  1, 0, 10, 300, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("gain level",            "milky",  1, 0, 20, 400, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("enlightenment",        "swirly",  1, 0, 20, 150, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("monster detection",    "bubbly",  1, 0, 40, 150, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("object detection",      "smoky",  1, 0, 32, 150, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("gain energy",          "cloudy",  1, 0, 42, 150, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("greater energy",     "greenish",  1, 0, 20, 200, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("full energy",		  "silvery",  1, 0, 10, 300, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("sleeping",       "effervescent",  1, 0, 32, 100, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("polymorph",            "golden",  1, 0, 10, 200, CLR_YELLOW, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("booze",                 "brown",  0, 0, 32,  50, CLR_BROWN, O1_COLD_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("sickness",              "fizzy",  0, 0, 32,  50, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("fruit juice",            "dark",  0, 0, 32,  50, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("acid",                  "white",  0, 0, 10, 250, CLR_WHITE, O1_COLD_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
POTION("oil",                   "murky",  0, 0, 30, 250, CLR_BROWN, O1_COLD_RESISTANT, O3_NONE, O3_IGNITABLE | O3_RELATIVE_AGE, PERMITTED_ALL),
/* fixed description
 */
POTION("water",                 "clear",  0, 0, 92, 100, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,mgc,prob,cost,sflags,flags,flags2,flags3,powconfermask)   \
    OBJECT(OBJ(name, text), None, None,   \
           BITS(0, 1, 0, 0, mgc, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, MAT_PAPER),    \
           0, 0, 0, P1_NONE, SCROLL_CLASS, prob, MULTIGEN_SINGLE, 0, 4, cost, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, sflags, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   6, HI_PAPER, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
SCROLL("enchant armor",              "ZELGO MER",  1,  60,  80, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("destroy armor",         "JUYED AWK YACC",  1,  45, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("protect armor",				   "VEX DOL",  1,  33,  50, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("confuse monster",                 "NR 9",  1,  33, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("scare monster",   "XIXAXA XOXAXA XUXAXA",  1,  35, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("remove curse",             "PRATYAVAYAH",  1,  65,  80, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("enchant weapon",         "DAIYEN FOOELS",  1,  80,  60, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("protect weapon",			   "ZEPH NI",  1,  30,  50, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("create monster",       "LEP GEX VEN ZEA",  1,  45, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("taming",                   "PRIRUTSENIE",  1,  15, 200, S1_SPELL_IS_NONREVERSIBLE_PERMANENT, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("genocide",                  "ELBIB YLOH",  1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("light",                 "VERR YED HORRE",  1,  50,  50, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("teleportation",        "VENZAR BORGAVVE",  1,  55, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("gold detection",                 "THARR",  1,  33, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("food detection",               "YUM YUM",  1,  25, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("identify",                  "KERNOD WEL",  1, 180,  20, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("magic mapping",              "ELAM EBOW",  1,  45, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("amnesia",                   "DUAM XNAHT",  1,  35, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("fire",                  "ANDOVA BEGARIN",  1,  30, 100, S1_NONE, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("earth",                          "KIRJE",  1,  18, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("punishment",            "VE FORBRYDERNE",  1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("charging",                "HACKEM MUCHE",  1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL("stinking cloud",             "VELOX NEB",  1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
    /* Extra descriptions, shuffled into use at start of new game.
     * Code in win/share/tilemap.c depends on SCR_STINKING_CLOUD preceding
     * these and on how many of them there are.  If a real scroll gets added
     * after stinking cloud or the number of extra descriptions changes,
     * tilemap.c must be modified to match.
     */
SCROLL(None,      "FOOBIE BLETCH",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,              "TEMOV",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,         "GARVEN DEH",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,            "READ ME",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,      "ETAOIN SHRDLU",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,        "LOREM IPSUM",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,              "FNORD",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Illuminati */
SCROLL(None,            "KO BATE",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Kurd Lasswitz */
SCROLL(None,      "ABRA KA DABRA",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* traditional incantation */
SCROLL(None,       "ASHPD SODALG",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Portal */
SCROLL(None,            "ZLORFIK",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Zak McKracken */
SCROLL(None,      "GNIK SISI VLE",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Zak McKracken */
SCROLL(None,    "HAPAX LEGOMENON",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
SCROLL(None,  "EIRIS SAZUN IDISI",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Merseburg Incantations */
SCROLL(None,    "PHOL ENDE WODAN",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Merseburg Incantations */
SCROLL(None,              "GHOTI",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* pronounced as 'fish',
                                                        George Bernard Shaw */
SCROLL(None, "MAPIRO MAHAMA DIROMAT", 1, 0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Wizardry */
SCROLL(None,  "VAS CORP BET MANI",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Ultima */
SCROLL(None,            "XOR OTA",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Aarne Haapakoski */
SCROLL(None, "STRC PRST SKRZ KRK",  1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* Czech and Slovak
                                                        tongue-twister */
    /* These must come last because they have special fixed descriptions.
     */
#ifdef MAIL
SCROLL("mail",          "stamped",  0,   0,   0, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
#endif
SCROLL("blank paper", "unlabeled",  0,  28,  60, S1_NONE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
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
#define SPELL(name,desc,contentdesc,shortdesc,skill,prob,learndelay,cooldown,level,manacost,attr,range,radius,skillchance,savingthrowadj,mgc,dir,dirsubtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,sflags,color,flags,flags2,flags3)  \
    OBJECT(OBJ(name, desc), contentdesc, shortdesc,                                           \
           BITS(0, 0, 0, 0, mgc, CHARGED_NOT_CHARGED, 0, 0, 0, 0, dir, 0, skill, MAT_PAPER),       \
           0, 0, 0, P1_NONE, SPBOOK_CLASS, prob, MULTIGEN_SINGLE, learndelay, 50, (level + 2) * 50 + (level + 1) * (level + 1) * 5,               \
           0, sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, 0, 0, 0, 0, sflags, 0, \
		   0, savingthrowadj, 0, 0, cooldown, level, manacost, attr, range, radius, skillchance, 0, \
		   20, color,dirsubtype, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, flags, flags2, flags3)
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

SPELL("dig",             "parchment", None, "Digs stone",
      P_TRANSMUTATION_SPELL,	 5,  4,			 0, 3, 10, A_INT, 10, 0, 100, 0, 1, RAY, RAY_DIGGING, 0, 0, 0, 0, 0, 0, S1_NONE, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE),
SPELL("knock",            "pink", None, "Opens the target door",
      P_TRANSMUTATION_SPELL,	 5,  1,			 0, -1, 3, A_INT, 12, 0, 50, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("wizard lock",     "dark green", None, "Locks the target door",
      P_TRANSMUTATION_SPELL,	 5,  3,			 0, -1, 3, A_INT, 12, 0, 50, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("create gold golem", "gold-circled", None, "Creates a permanent gold golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 1, 10, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_GOLD, O1_NONE, O2_NONE, O3_NONE),
SPELL("create glass golem", "crystalline", None, "Creates a permanent glass golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 6, 75, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create gemstone golem", "ruby-studded", None, "Creates a permanent gemstone golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 8,150, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_RED, O1_NONE, O2_NONE, O3_NONE),
SPELL("create clay golem", "clay-covered", None, "Creates a permanent clay golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 2, 15, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create stone golem", "stone-studded", None, "Creates a permanent stone golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 3, 20, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
SPELL("create iron golem", "iron-reinforced", None, "Creates a permanent iron golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 6, 75, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create paper golem", "flimsy", None, "Creates a permanent paper golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 0,  7, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create wood golem", "wooden", None, "Creates a permanent wood golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 1, 10, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create silver golem", "silvery", None, "Creates a permanent silver golem",
	P_TRANSMUTATION_SPELL,		 5,   4,	   100, 9,200, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_SILVER, O1_NONE, O2_NONE, O3_NONE),
SPELL("polymorph",        "silver-plated", None, "Polymorphs monsters and items into something else",
      P_TRANSMUTATION_SPELL,    5,  6,			 0, 5, 50, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_SILVER, O1_NONE, O2_NONE, O3_NONE),
SPELL("cancellation",     "shining", None, "Prohibits the target from using magic or magical attacks",
      P_TRANSMUTATION_SPELL,	5,  8,			 0, 5, 50, A_MAX_WIS_CHA, 8, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 1, 20, 30, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("silence",     "tree-patterned", None, "Prohibits the target from making noises and casting spells",
      P_TRANSMUTATION_SPELL,	5,  2,			 0, 2, 15, A_MAX_WIS_CHA, 12, 3, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 1, 20, 30, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("lower water",			"obnoxious", None, "Evaporates water",
      P_TRANSMUTATION_SPELL,	5,  3,			 0, 5, 50, A_MAX_INT_WIS, 10, 0, 100, 0, 1, RAY, RAY_EVAPORATION, 6, 6, 0, 0, 0, 0, S1_NONE, CLR_RED, O1_NONE, O2_NONE, O3_NONE),
SPELL("touch of petrification",   "convoluted", None, "Petrifies the target over a few rounds",
	P_TRANSMUTATION_SPELL,		5,  8,			 0, 7,100, A_MAX_INT_WIS, 1, 0, 100, 0, 1, TOUCH, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("flesh to stone",   "amorphous", None, "Petrifies the target over a few rounds",
	P_TRANSMUTATION_SPELL,		5,  8,			 0, 8,150, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("gaze of Medusa",   "stone-studded", None, "Petrifies the target instantly",
	P_TRANSMUTATION_SPELL,		5,  8,			 0, 9,200, A_CHA, 10, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("water breathing",		"navy blue", None, "Makes the caster able to breathe underwater",
	P_TRANSMUTATION_SPELL,		5, 1,			 0, 3,20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_BREATHING, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("stone to flesh",   "thick", None, "Turns a statue back to a living being",
	P_TRANSMUTATION_SPELL,			5,  1,			 0, 3, 20, A_MAX_INT_WIS, 1, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),

SPELL("magic missile",   "vellum", None, "Shoots a ray of magic missiles",
      P_ARCANE_SPELL,			10,  2,			 0, 0, 4, A_INT, 10, 0, 20, 0, 1, RAY, RAY_MAGIC_MISSILE, 2, 6, 0, 0, 0, 0, S1_NONE, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE),
SPELL("burning hands",    "hand-patterned", None, "Causes the target to suffer fire damage",
      P_ARCANE_SPELL,			 5,  4,			 3, 2, 7, A_INT, 1, 0, 20, 0, 1, TOUCH, 0, 8, 6, 0, 0, 0, 0, S1_NONE, CLR_RED, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("fire bolt",        "light red", None, "Shoots a bolt of fire",
      P_ARCANE_SPELL,			 5,  4,			 0, 2, 7, A_INT, 10, 0, 20, 0, 1, RAY, RAY_FIRE, 4, 6, 0, 0, 0, 0, S1_NONE, CLR_RED, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("magical implosion", "radiating", None, "Creates a magical implosion at the target",
      P_ARCANE_SPELL,			 5,  5,		     0, 3, 10, A_INT, 12, 0, 20, 0, 1, RAY, RAY_MAGIC_MISSILE, 4, 6, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("fireball",         "ragged", None, "Shoots an exploding fireball",
      P_ARCANE_SPELL,			 5,  4,			 0, 4, 15, A_INT, 14, 0, 20, 0, 1, RAY, RAY_FIRE, 6, 6, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT, HI_PAPER, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("fire storm",       "crimson", None, "Conjures a storm of fire",
      P_ARCANE_SPELL,			 5,  8,			 4, 6, 35, A_INT, 16, 0, 50, 0, 1, RAY, RAY_FIRE, 12, 6, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT, CLR_RED, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("meteor swarm",     "oval-patterned", None, "Shoots 4 exploding meteors",
      P_ARCANE_SPELL,			 5, 10,			10, 9,100, A_INT, 18, 0,100, 0, 1, RAY, RAY_FIRE, 8, 6, 0, 0, 0, 4, S1_SPELL_EXPLOSION_EFFECT, CLR_RED, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("cone of cold",     "dog-eared", None, "Shoots a ray of cold",
      P_ARCANE_SPELL,			 5,  6,			 0, 5, 25, A_INT, 12, 0, 20, 0, 1, RAY, RAY_COLD, 12, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_COLD_RESISTANT, O2_NONE, O3_NONE),
SPELL("freezing touch",    "glowing blue", None, "Causes the target to suffer cold damage",
      P_ARCANE_SPELL,			 5,  4,			 4, 4, 15, A_INT, 1, 0, 20, 0, 1, TOUCH, 0, 16, 6, 0, 0, 0, 0, S1_NONE, CLR_BLUE, O1_COLD_RESISTANT, O2_NONE, O3_NONE),
SPELL("ice storm",		  "mithril-plated", None, "Conjures a storm of cold",
      P_ARCANE_SPELL,			 5,  5,			 5, 5, 25, A_INT, 14, 0, 20, 0, 1, RAY, RAY_COLD, 8, 6, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT, HI_SILVER, O1_COLD_RESISTANT, O2_NONE, O3_NONE),
SPELL("force bolt",       "red", None, "Shoots a bolt of force",
      P_ARCANE_SPELL,			15,  2,			 0, 1, 5, A_INT, 10, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_MAGIC_MISSILE_NONBEAM , 3, 6, 0, 0, 0, 0, S1_NONE, CLR_RED, O1_NONE, O2_NONE, O3_NONE),
SPELL("magic arrow",      "hardcover", None, "Shoots a magical arrow",
      P_ARCANE_SPELL,			15,  1,			 0, -1, 2, A_INT, 18, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_ITEM_ARROW, 1, 8, 1, 0, 0, 0, S1_NONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE),
SPELL("lightning bolt",   "shimmering", None, "Shoots a ray of lightning",
      P_ARCANE_SPELL,			 5,  3,			 0, 3, 10, A_INT, 14, 0, 20, 0, 1, RAY, RAY_LIGHTNING, 6, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
SPELL("shocking touch",   "hand-patterned", None, "Causes the target to be jolted with electricity",
      P_ARCANE_SPELL,			 5,  3,			 3, 3, 10, A_INT, 1, 0, 20, 0, 1, TOUCH, 0, 12, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
SPELL("thunderstorm",	  "strange", None, "Conjures a storm of lightning",
      P_ARCANE_SPELL,			 5,  10,		 5, 7, 50, A_INT, 18, 0, 50, 0, 1, RAY, RAY_LIGHTNING, 16, 6, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT, HI_PAPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
SPELL("magic storm",	  "scintillating", None, "Conjures a storm of arcane energy",
      P_ARCANE_SPELL,			 5,  5,		 5, 8,75, A_INT, 18, 0, 50, 0, 1, RAY, RAY_MAGIC_MISSILE, 24, 6, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("drain life",      "velvet", None, "Drains life of the target",
      P_ARCANE_SPELL,			 5,  2,			 0, 5, 25, A_INT, 5, 0, 20, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("disintegrate",	  "stained", None, "Shoots a disintegration ray",
      P_ARCANE_SPELL,			 5, 12,			 5, 10,150, A_INT, 8, 0, 100, 0, 1, RAY, RAY_DISINTEGRATION, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE),
SPELL("power word kill",   "preposterous", "some long-forgotten arcane magic", "Kills the target",
    P_ARCANE_SPELL,				 5,  5,		   500,12, 375, A_INT, 6, 0, 10, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("power word stun",   "ludicrous", None, "Stuns the target",//Note monsters do not have a stun timer
    P_ARCANE_SPELL,				 5,  5,		   250, 6, 35, A_INT,10, 0, 5, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("power word blind",   "anomalous", None, "Blinds the target", //Note monsters have a blindness timer
    P_ARCANE_SPELL,				 5,  5,		   250, 8, 75, A_INT, 8, 0, 5, 0, 1, IMMEDIATE, 0, 0, 0, 0, 5, 10, 50, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("sphere of annihilation",	"jet-studded", "some long-forgotten arcane magic", "Disintegrates everything around you",
      P_ARCANE_SPELL,			 5, 12,			50, 12,500, A_INT, 0, 5, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE),
SPELL("circle of magic",	"diamond-studded", None, "Damages the monsters around you with magic",
      P_ARCANE_SPELL,			 5,  4,			 5, 4, 25, A_INT, 0, 1, 50, 0, 1, NODIR, 0, 6, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("circle of fire",	"sapphire-studded", None, "Damages the monsters around you with fire",
      P_ARCANE_SPELL,			 5,  4,			 5, 5, 25, A_INT, 0, 1, 50, 0, 1, NODIR, 0, 8, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("circle of lightning",	"crystal-studded", None, "Damages the monsters around you with lightning",
      P_ARCANE_SPELL,			 5,  4,			 5, 6, 35, A_INT, 0, 1, 50, 0, 1, NODIR, 0, 12, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("circle of frost",	"pearl-studded", None, "Damages the monsters around you with cold",
      P_ARCANE_SPELL,			 5,  4,			 5, 7, 50, A_INT, 0, 1, 50, 0, 1, NODIR, 0, 16, 6, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),

SPELL("animate air",	  "wave-patterned", None, "Summons an air elemental for a duration",
	P_CONJURATION_SPELL,		 5,  3,			90, 7,100, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 20, 150, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("animate earth",	  "octagonal-patterned", None, "Summons an earth elemental for a duration",
	P_CONJURATION_SPELL,		 5,  3,			90, 4, 30, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 20, 150, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("animate fire",	  "flame-patterned", None, "Summons a fire elemental for a duration",
	P_CONJURATION_SPELL,		 5,  3,			90, 5, 50, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 20, 150, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("animate water",	  "water-drop-patterned", None, "Summons a water elemental for a duration",
	P_CONJURATION_SPELL,		 5,  3,			90, 3, 20, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 20, 150, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("summon demon",	  "tenebrous", None, "Summons a demon for a duration",
	P_CONJURATION_SPELL,		 5,  5,	       300, 8,150, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 100, 400, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("faithful hound",	  "canine-patterned", None, "Summons a permanent faithful large dog",
	P_CONJURATION_SPELL,		 5,  3,	       300, 4,30, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("call Demogorgon",  "eldritch", "some aeon-old prayer", "Calls the Prince of Demons from the Abyss",
	P_CONJURATION_SPELL,		 5, 20,		   400,9,200, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 2, 100, 300, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("call hierarch modron",  "spherical", "some aeon-old prayer", "Calls a hierarch modron",
	P_CONJURATION_SPELL,		 0,  5,		   300, 6, 75, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 100, 400, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NO_GENERATION),
SPELL("create monster",  "turquoise", None, "Creates a random monster",
      P_CONJURATION_SPELL,		 5,  3,			 0, 3, 30, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BRIGHT_CYAN, O1_NONE, O2_NONE, O3_NONE),
SPELL("create familiar",  "glittering", None, "Creates a tame monster",
      P_CONJURATION_SPELL,		 5,  1,			 0, 1, 10, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("summoning call",  "whistle-patterned", None, "Magically teleports your pets and allies to you.",
      P_CONJURATION_SPELL,		 5,  1,			 0, -1, 4, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("great Yendorian summoning", "hideous", "some long-forgotten arcane magic", "Summons 1d3 permanent nasties",
	P_CONJURATION_SPELL,		 5,  5,		   150,10,300, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("stinking cloud",			"peculiar", None, "Conjures a stinking cloud",
	P_CONJURATION_SPELL,		 5, 3,			 0, 2, 15, A_CHA, 0, 0, 100, 0, 1, TARGETED, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("wish",			  "platinum-plated", "some long-forgotten arcane magic", "The caster can wish for an object",
      P_CONJURATION_SPELL,		 3, 15,			20,12,750, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_SILVER, O1_NONE, O2_NONE, O3_NO_WISH),
SPELL("black blade of disaster", "ebony", "some long-forgotten arcane magic", "Summons a blade that disintegrates targets on hit",
	P_CONJURATION_SPELL,		 5, 10,			20,11,500, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 3, 10, 30, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, CLR_BLACK, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE),
SPELL("mage armor",		   "ornamental", None, "Summons a wizard-friendly magical armor with AC4",
	P_CONJURATION_SPELL,	     5,  1,		   360, 1, 10, A_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 4, 20, 200, S1_NONE, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE),

SPELL("commune",	  "beige", None, "Gives hints about your praying and luck status",
	P_CLERIC_SPELL,				 5,  1,		   0, -1, 4, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE),
SPELL("enlightenment",	  "illuminating", None, "Provides enlightenment about your statistics",
	P_CLERIC_SPELL,				 5,  1,		   0, 0, 7, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("stick to snake",	  "stick-patterned", None, "Summons a permanent snake",
	P_CLERIC_SPELL,				 5,  1,		   0, 1, 10, A_WIS, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("stick to cobra",	  "snake-patterned", None, "Summons a permanent cobra",
	P_CLERIC_SPELL,				 5,  1,		   0, 2, 15, A_WIS, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("heavenly warrior", "feather-patterned", None, "Summons an Aleax for a duration",
	P_CLERIC_SPELL,				 5,  5,		   300, 4, 30, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 280, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("guardian angel",	  "wing-patterned", "some aeon-old prayer", "Summons a guardian angel for a duration",
	P_CLERIC_SPELL,				 5,  5,		   300, 5, 50, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 280, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("divine mount",	  "horn-patterned", None, "Summons a ki-rin for a duration",
	P_CLERIC_SPELL,				 5,  5,		   300, 6, 75, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 20, 280, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("heavenly army",	  "angelic", "some aeon-old prayer", "Summons 2d4 angelic beings for a duration",
	P_CLERIC_SPELL,				 5,  5,		   150, 9,300, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 10, 140, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("holy word",			"antediluvian", None, "Stuns, blinds, and kills nearby enemies",
	P_CLERIC_SPELL,				 5,  7,			 0, 8,150, A_WIS, 0, 5, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 10, 50, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("remove curse",     "wrinkled", None, "Removes curse from an item",
      P_CLERIC_SPELL,			5,  5,			 0, 4, 20, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("turn undead",      "copper-plated", None, "Makes damages undead creatures and makes them flee",
      P_CLERIC_SPELL,			5,  3,			 5, 3, 20, A_WIS, 0, 0, 100, 0, 1, IMMEDIATE, 3, 6, 0, 0, 2, 50, 200, S1_NONE, HI_COPPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("bless",			   "spiritual", None, "Blesses on object",
	P_CLERIC_SPELL,			    5, 1,			 0, 3, 20, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE),
SPELL("curse",			   "blasphemous", None, "Curses one object",
	  P_CLERIC_SPELL,	        5,  1,			 0, 3, 20, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE),
SPELL("resurrection",		  "bright-colored", None, "Brings the target corpse back to life",
	P_CLERIC_SPELL,				5, 5,			 0, 5,50, A_WIS, 6, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("negate undeath",		  "inspiring", None, "Destroys the target undead",
	P_CLERIC_SPELL,				5, 4,			10, 7,100, A_WIS, 8, 0, 100, 0, 1, IMMEDIATE, 0, 20, 6, 0, 1, 50, 100, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("banish demon",		  "squamous", None, "Destroys the target demon",
	P_CLERIC_SPELL,				5, 4,			10, 8,150, A_WIS, 8, 0, 100, 0, 1, IMMEDIATE, 0, 30, 6, 0, 1, 50, 100, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("prayer",			   "biblical", "some aeon-old prayer", "May invoke a divine intervention from your god",
	P_CLERIC_SPELL,			    5, 5,		   175, 4, 30, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("absolution",		   "ecclesiastical", "some aeon-old prayer", "Absolves your sins",
	P_CLERIC_SPELL,			    5, 5,		  1000, 6,175, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),

SPELL("light",            "cloth", None, "Lights up the nearby area",
      P_DIVINATION_SPELL,		10,  1,			 0, 0,  2, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE),
SPELL("probe monster",    "adamantium-plated", None, "Displays the monster's statistics",
      P_DIVINATION_SPELL,		5,  1,			 0, -1, 4, A_MAX_INT_WIS_CHA, 12, 0,   5, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_CLOTH, O1_NONE, O2_NONE, O3_NONE),
SPELL("detect monsters",  "leathery", None, "The caster can sense all monsters on the level",
      P_DIVINATION_SPELL,		8,  1,			 0, 0, 5, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE),
SPELL("detect food",     "cyan", None, "Detects the location of all comestibles",
      P_DIVINATION_SPELL,		10,  3,			 0, -1, 4, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE),
SPELL("clairvoyance",    "dark blue", None, "Grants clairvoyance",
      P_DIVINATION_SPELL,		5,  3,			 0, 2, 15, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BLUE, O1_NONE, O2_NONE, O3_NONE),
SPELL("detect unseen",   "violet", None, "Enables the caster to see invisible monsters and secret doors",
      P_DIVINATION_SPELL,		10,  4,			 0, 1, 10, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 10, 100, S1_NONE, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("detect treasure",  "gray", None, "The caster senses the location of all objects on the level",
      P_DIVINATION_SPELL,		10,  5,			 0, 0,  7, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE),
SPELL("identify",         "bronze-plated", None, "Identifies an item",
      P_DIVINATION_SPELL,		10,  1,			 0, 1, 10, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_COPPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("magic mapping",    "dusty", None, "The caster gains the knowledge of the map of the level",
      P_DIVINATION_SPELL,		5,  7,			 0, 4, 60, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("telepathy",				"unnameable", None, "Grants the caster telepathy",
	P_DIVINATION_SPELL,			5, 3,			 0, 3, 20, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, TELEPAT, 0, 0, 0, 6, 10, 150, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("detect traps",			"incongruous", None, "The caster gains the knowledge of traps on the level",
	P_DIVINATION_SPELL,			5, 3,			 0, 6, 75, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),

SPELL("sleep", "mottled", None, "Shoots a sleep ray",
	P_ENCHANTMENT_SPELL,		10, 1, 20, 5, 50, A_MAX_INT_CHA, 12, 0, 100, 0, 1, RAY, RAY_SLEEP, 0, 0, 0, 2, 6, 7, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass sleep", "obtuse", None, "Causes nearby monsters fall asleep",
	P_ENCHANTMENT_SPELL,		5, 5, 30, 7, 100, A_MAX_INT_CHA, 0, 6, 100, 0, 1, NODIR, 0, 0, 0, 0, 4, 6, 14, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("fear",			"blue-hued", None, "Causes monsters to flee",
      P_ENCHANTMENT_SPELL,		10,  3,			 0, 4, 30, A_MAX_INT_CHA, 10, 0, 100, 0, 1, IMMEDIATE_MULTIPLE_TARGETS, 0, 0, 0, 0, 1, 25, 50, S1_NONE, CLR_BRIGHT_BLUE, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass fear",      "light blue", None, "Causes all monsters on the level to flee",
      P_ENCHANTMENT_SPELL,		 5,  3,			 0, 6, 75, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 50, 100, S1_NONE, CLR_BRIGHT_BLUE, O1_NONE, O2_NONE, O3_NONE),
SPELL("confuse monster", "orange", None, "The caster can confuse monsters on hit",
      P_ENCHANTMENT_SPELL,		10,  2,			 0, 1, 10, A_MAX_INT_CHA, 16, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE),
SPELL("charm monster",   "eye-patterned", None, "Charms a monster for a duration",
      P_ENCHANTMENT_SPELL,		10,  3,			 2, 3, 20, A_MAX_INT_CHA, 8, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 10, 50, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("sphere of charming", "magenta", None, "Charms adjacent monsters for a duration",
      P_ENCHANTMENT_SPELL,		10,  5,			 5, 5, 50, A_MAX_INT_CHA, 0, 1, 100, 0, 1, NODIR, 0, 0, 0, 0, 10, 10, 50, S1_NONE, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass charm",		 "multi-hued", None, "Charms nearby monsters for a duration",
      P_ENCHANTMENT_SPELL,		5,  7,			10, 7,100, A_MAX_INT_CHA, 0, 5, 100, 0, 1, NODIR, 0, 0, 0, 0, 10, 10, 50, S1_NONE, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("dominate monster",   "eye-and-triangle-patterned", None, "Charms a monster permanently",
      P_ENCHANTMENT_SPELL,		5,  6,			 2, 6, 75, A_MAX_INT_CHA, 8, 0, 100, -4, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_IS_NONREVERSIBLE_PERMANENT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("sphere of domination", "pyramid-patterned", None, "Charms adjacent monsters permanently",
      P_ENCHANTMENT_SPELL,		5,  8,			 5, 8,150, A_MAX_INT_CHA, 0, 1, 100, -4, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_IS_NONREVERSIBLE_PERMANENT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass domination", "fractal-patterned", "some long-forgotten arcane magic", "Charms nearby monsters permanently",
      P_ENCHANTMENT_SPELL,		5, 10,			10,10,300, A_MAX_INT_CHA, 0, 5, 100, -4, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_IS_NONREVERSIBLE_PERMANENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("invisibility",     "dark brown", None, "The caster becomes invisible",
      P_ENCHANTMENT_SPELL,		5,  5,			120,2, 15, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 2, 10, 80, S1_NONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE),
SPELL("enchant armor",		  "glowing", None, "Increases the enchantment on a piece of armor",
	  P_ENCHANTMENT_SPELL,		5, 3,			 0, 3,20, A_MAX_INT_CHA, 0, 0, 300, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("enchant weapon",		  "dark gray", None, "Increases the enchantment on a weapon",
	  P_ENCHANTMENT_SPELL,		5, 3,			 0, 4,30, A_MAX_INT_CHA, 0, 0, 300, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protect armor",		  "glowing", None, "Makes a piece of armor rust- or burn-proof",
	  P_ENCHANTMENT_SPELL,		5, 2,			 0, 1,10, A_MAX_INT_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protect weapon",		  "antiquarian", None, "Makes a weapon rust- or burn-proof",
	  P_ENCHANTMENT_SPELL,		5, 2,			 0, 2,15, A_MAX_INT_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("cold-enchant item",		"baleful", None, "Cold-enchants one suitable item",
	P_ENCHANTMENT_SPELL,		5, 1,			 0, 1, 10, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_COLD_RESISTANT, O2_NONE, O3_NONE),
SPELL("fire-enchant item",		"deformed", None, "Fire-enchants one suitable item",
	P_ENCHANTMENT_SPELL,		5, 1,			 0, 2, 15, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("lightning-enchant item",	"degraded", None, "Lightning-enchants one suitable item",
	P_ENCHANTMENT_SPELL,		5, 1,			 0, 3, 20, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
SPELL("death-enchant item",		"diabolical", None, "Death-enchants one suitable item",
	P_ENCHANTMENT_SPELL,		5, 3,			 0, 8,150, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mirror image",			"nonsensical", None, "A displaced mirror image appears beside the caster",
	P_ENCHANTMENT_SPELL,		5, 3,			 0, 1, 10, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, DISPLACED, 0, 0, 0, 6, 10, 180, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass conflict",			"odious", None, "Causes all monsters to attach each other on the level",
	P_ENCHANTMENT_SPELL,		5, 3,			 0, 9,200, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, CONFLICT, 0, 0, 0, 3, 10, 50, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),

SPELL("cure sickness",   "indigo", None, "Cures sickness, terminal illness, and sliming",
      P_HEALING_SPELL,			7,  3,			 0, 2, 15, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BLUE, O1_NONE, O2_NONE, O3_NONE),
SPELL("cure petrification",   "runic", None, "Cures petrification",
      P_HEALING_SPELL,			5,  5,			 0, 4, 30, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE),
SPELL("cure blindness",  "yellow", None, "Cures blindness",
      P_HEALING_SPELL,			5,  2,			 0, 0,  7, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE),
SPELL("minor healing",         "white", None, "Heals a target for 3d6 hit points",
      P_HEALING_SPELL,			5,  1,			 0, -1, 3, A_WIS, 5, 0, 50, 0, 1, IMMEDIATE, 0, 3, 6, 0, 0, 0, 0, S1_NONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("healing",         "white", None, "Heals a target for 6d6 hit points",
      P_HEALING_SPELL,			5,  2,			 0, 1,  7, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 6, 6, 0, 0, 0, 0, S1_NONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("extra healing",   "plaid", None, "Heals a target for 12d6 hit points",
      P_HEALING_SPELL,			5,  5,			 2, 3, 15, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 12, 6, 0, 0, 0, 0, S1_NONE, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("greater healing",  "darkened", None, "Heals a target for 20d6+30 hit points",
      P_HEALING_SPELL,			5,  5,			 4, 5, 35, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 20, 6, 30, 0, 0, 0, S1_NONE, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("full healing",     "light gray", None, "Heals a target to full hit points",
      P_HEALING_SPELL,			5,  6,			 6, 7, 75, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 255, 0, 0, 0, S1_NONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("restore ability",  "light brown", None, "Restores lost ability points",
      P_HEALING_SPELL,			5,  5,			 0, 2, 15, A_WIS, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE),

SPELL("slow monster",    "light green", None, "Slows down the target",
	P_MOVEMENT_SPELL,			5,  2,			 0, 1, 10, A_MAX_INT_WIS_CHA, 12, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 1, 50, 100, S1_NONE, CLR_BRIGHT_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass slow",    "olive green", None, "Slows down monsters with 15' radius",
	P_MOVEMENT_SPELL,			5,  4,			 0, 4, 30, A_MAX_INT_WIS_CHA, 12, 3, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 1, 50, 100, S1_NONE, CLR_BRIGHT_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("hold monster",    "fist-patterned", None, "Paralyzes the target",
	P_MOVEMENT_SPELL,			5,  3,			 0, 4, 30, A_MAX_INT_WIS_CHA, 18, 0, 100, -3, 1, IMMEDIATE, 0, 0, 0, 0, 1, 6, 10, S1_NONE, HI_LEATHER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass hold",    "cerulean", None, "Paralyzes the targets within the area of effect",
	P_MOVEMENT_SPELL,			5,  5,			 0, 6, 75, A_MAX_INT_WIS_CHA, 18, 4, 100, -6, 1, IMMEDIATE, 0, 0, 0, 0, 2, 6, 20, S1_NONE, CLR_BRIGHT_BLUE, O1_NONE, O2_NONE, O3_NONE),
SPELL("haste self",      "purple", None, "Increases the movement and attack speed of the caster",
      P_MOVEMENT_SPELL,			5,  4,			 0, 3, 20, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 10, 100, S1_NONE, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("haste monster",    "cinereous", None, "Increases the speed of the target",
      P_MOVEMENT_SPELL,			5,  6,			 0, 5, 50, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 3, 10, 300, S1_NONE, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE),
SPELL("levitation",      "tan", None, "Raises the caster off the ground",
      P_MOVEMENT_SPELL,			5,  4,			 0, 2, 15, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 5, 10, 140, S1_NONE, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE),
SPELL("teleport self",    "jade-studded", None, "Teleports the caster to a random location on the level",
      P_MOVEMENT_SPELL,			5,  2,			 3, 2, 15, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("teleport monster",    "gold-plated", None, "Teleports the target to a random location on the level",
      P_MOVEMENT_SPELL,			5,  4,			 3, 4, 30, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_GOLD, O1_NONE, O2_NONE, O3_NONE),
SPELL("controlled teleport",    "byzantine", None, "Teleports the caster to the chosen location on the level",
      P_MOVEMENT_SPELL,			5,  5,			 5, 5, 50, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE),
SPELL("circle of teleportation",    "carmine", None, "Teleports the monsters around the caster to random locations on the level",
      P_MOVEMENT_SPELL,			5,  5,			 5, 6, 75, A_MAX_INT_WIS_CHA, 0, 1, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_RED, O1_NONE, O2_NONE, O3_NONE),
SPELL("level teleport",    "viridian", None, "Teleports the caster to another level",
      P_MOVEMENT_SPELL,			5,  5,			 5, 5, 50, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE),
SPELL("controlled level teleport",    "glaucous", None, "Teleports the caster to the chosen level",
      P_MOVEMENT_SPELL,			5,  5,			 5, 8,150, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE),
SPELL("portal",    "fulvous", None, "Teleports the caster to the chosen, previously visited branch of the dungeon",
      P_MOVEMENT_SPELL,			5,  5,			 5, 6, 75, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE),
SPELL("jumping",          "thin", None, "Enables the caster to jump",
      P_MOVEMENT_SPELL,			5,  1,			 0,-1,  4, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("water walking",			"circle-patterned", None, "Makes the caster able to walk on water",
	P_MOVEMENT_SPELL,			5, 3,			 0, 3,20, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, WATER_WALKING, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("time stop",		   "ancient", "some long-forgotten arcane magic", "Stops the flow of time, except for the caster",
	P_MOVEMENT_SPELL,			2, 10,			15, 9,200, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 1, 4, 3, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),

SPELL("protection",       "dull", None, "Gives -3 bonus to AC and +3 to MC",
	P_ABJURATION_SPELL,			5, 3,		   0, 2, 15, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, PROTECTION, 0, 0, 0, 2, 10, 130, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("shield",       "ruby-studded", None, "Gives -4 bonus to AC",
	P_ABJURATION_SPELL,			5, 3,		   0, 1, 10, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_SHIELDING, 0, 0, 0, 2, 10, 130, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("barkskin",       "emerald-studded", None, "Gives +7 bonus to MC",
	P_ABJURATION_SPELL,			5, 3,		   0, 3, 20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_BARKSKIN, 0, 0, 0, 2, 10, 130, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("stoneskin",       "diamond-studded", None, "Gives -10 bonus to AC",
	P_ABJURATION_SPELL,			5, 3,		   0, 4, 50, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_STONESKIN, 0, 0, 0, 2, 10, 130, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("anti-magic shell",   "immaculate", None, "Grants resistance to magic",
	  P_ABJURATION_SPELL,	    5,  1,		   120, 7,100, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, ANTIMAGIC, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("reflection", "polished silver", None, "Grants reflection, which protects from rays",
	  P_ABJURATION_SPELL,	    5,  1,		   120, 6, 75, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, REFLECTING, 0, 0, 0, 10, 6, 25, S1_NONE, HI_SILVER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from fire", "smoky", None, "Grants immunity to fire attacks",
	  P_ABJURATION_SPELL,	    5,  1,		   120, 2, 15, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, FIRE_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
SPELL("protection from lightning", "cloudy", None, "Grants immunity to lightning attacks",
	  P_ABJURATION_SPELL,	    5,  1,		   120, 2, 15, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, SHOCK_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
SPELL("protection from cold", "blue and white", None, "Grants immunity to cold attacks",
	  P_ABJURATION_SPELL,	    5,  1,		   120, 2, 15, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, COLD_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_COLD_RESISTANT, O2_NONE, O3_NONE),
SPELL("protection from acid", "incomplete", None, "Grants immunity to acid attacks",
	  P_ABJURATION_SPELL,	    5,  1,		   120, 1, 10, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, ACID_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from poison", "intimidating-looking", None, "Grants immunity to poison",
	P_ABJURATION_SPELL,			5, 1,		   120, 1, 10, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, POISON_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from life draining", "adorned", None, "Grants immunity to life draining",
	P_ABJURATION_SPELL,			5, 1,		   120, 0, 7, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, DRAIN_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from death magic", "decorated", None, "Grants immunity to death magic",
	P_ABJURATION_SPELL,			5, 1,		   120, 3, 20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, DEATH_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from disintegration", "aberrant", None, "Grants immunity to disintegration",
	P_ABJURATION_SPELL,			5, 1,		   120, 4, 30, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, DISINT_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE),
SPELL("protection from sickness", "baroque", None, "Grants immunity to sickness",
	P_ABJURATION_SPELL,			5, 1,		   120, 4, 30, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, SICK_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from petrification", "aged", None, "Immunity to petrification attacks",
	P_ABJURATION_SPELL,			5, 1,		   120, 5, 50, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, STONE_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("globe of invulnerability", "striped", "some long-forgotten arcane magic", "Makes the caster invulnerable to physical damage",
	P_ABJURATION_SPELL,			5, 1,			60, 8,150, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, INVULNERABLE, 0, 0, 0, 1, 6, 4, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_INDESTRUCTIBLE, O2_NONE, O3_NONE),
SPELL("divine intervention", "square-patterned", "some aeon-old prayer", "The life of the caster will be saved",
	P_ABJURATION_SPELL,			5, 1,		   120, 9,200, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, LIFESAVED, 0, 0, 0, 10, 6, 25, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from lycanthropy", "strange-looking", None, "Grants immunity to lycanthropy",
	P_ABJURATION_SPELL,			5, 1,		   120, 0, 7, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, LYCANTHROPY_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("protection from curses", "cross-patterned", None, "Items of the caster cannot be cursed",
	P_ABJURATION_SPELL,			5, 1,		   120, 3,20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, CURSE_RES, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("lower magic resistance", "dim", None, "Cuts magic resistance of the target by 25%",
	P_ABJURATION_SPELL,			5, 1,			 0, 3, 20, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("halve magic resistance", "unclear", None, "Cuts magic resistance of the target by half",
	P_ABJURATION_SPELL,			5, 2,			 0, 5, 50, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("diminish magic resistance", "obscure", None, "Cuts magic resistance of the target by 75%",
	P_ABJURATION_SPELL,			5, 3,			 0, 7, 100, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("negate magic resistance", "dark", None, "Cuts magic resistance of the target to 0%",
	P_ABJURATION_SPELL,			5, 4,			 0, 9, 300, A_MAX_INT_WIS, 10, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("forbid summoning",		"oval-patterned", None, "The target cannot use summoning abilities",
	P_ABJURATION_SPELL,			5, 1,			 0, 3, 20, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 10, 6, 25, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),

SPELL("touch of death",   "heavy", "ancient Stygian magic", "Kills the target upon touch",
	P_NECROMANCY_SPELL,		    5,  8,			 8, 8,150, A_MAX_INT_WIS_CHA, 1, 0, 100, 0, 1, TOUCH, RAY_DEATH, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("finger of death",  "leather bound", "ancient Stygian magic", "Shoots a death ray",
	P_NECROMANCY_SPELL,		    5, 10,			10, 9,200, A_MAX_INT_WIS_CHA,  8, 0, 100, 0, 1, RAY, RAY_DEATH, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("deathspell",	      "morbid", "ancient Stygian magic", "Kills the targets in the area of effect",
	P_NECROMANCY_SPELL,		    5, 10,			12,10,300, A_MAX_INT_WIS_CHA, 12, 1, 100, 0, 1, RAY, RAY_DEATH, 0, 0, 0, 0, 0, 0, S1_SPELL_EXPLOSION_EFFECT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("armageddon",       "pitch black", "ancient Stygian magic", "Kills all creatures on the level",
	P_NECROMANCY_SPELL,			2, 14,			20,12,750, A_MAX_INT_WIS_CHA, 0, 255, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE),
SPELL("call ghoul",				"repulsive", None, "Summons 2d4 ghouls for a duration",
	P_NECROMANCY_SPELL,			5,  5,		   300, 3, 20, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, 0, 0, 0, 0, 2, 10, 140, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise minor zombie",			"unspeakable", None, "Raises a zombifiable non-giant corpse into a permanent zombie",
	P_NECROMANCY_SPELL,			5,  5,		    0, 0, 7, A_MAX_INT_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create minor mummy",			"terrifying", None, "Makes a mummifiable non-giant corpse into a permanent mummy",
	P_NECROMANCY_SPELL,			5,  5,		    0, 2, 15, A_MAX_INT_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise giant zombie",			"unspeakable", None, "Raises a giant or ettin corpse into a permanent zombie",
	P_NECROMANCY_SPELL,			5,  5,		    0, 4, 30, A_MAX_INT_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create giant mummy",			"malevolent", None, "Makes a giant or ettin corpse into a permanent mummy",
	P_NECROMANCY_SPELL,			5,  5,		    0, 5, 50, A_MAX_INT_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("create dracolich",		"sacrilegious", "ancient Stygian magic", "Raises a dragon corpse into a permanent dracolich",
	P_NECROMANCY_SPELL,			5,  7,		    0, 11,500, A_MAX_INT_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass raise zombie",		"abnormal", None, "Raises zombifiable corpses into permanent zombies",
	P_NECROMANCY_SPELL,			5,  5,		    50, 6, 75, A_MAX_INT_WIS_CHA, 0, 8, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass create mummy",		"cadaverous", None, "Makes mummifiable corpses into permanent mummies",
	P_NECROMANCY_SPELL,			5,  5,		    50, 8, 150, A_MAX_INT_WIS_CHA, 0, 8, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("mass create dracolich",	"insipid", "ancient Stygian magic", "Raises dragon corpses into permanent dracoliches",
	P_NECROMANCY_SPELL,			1,  9,		   200, 12,750, A_MAX_INT_WIS_CHA, 0, 8, 100, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise skeleton",			"loathsome", None, "Summons a permanent skeleton",
	P_NECROMANCY_SPELL,			5,  5,		    0, 1, 10, A_MAX_INT_WIS_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise giant skeleton",			"stupendous", None, "Summons a permanent giant skeleton",
	P_NECROMANCY_SPELL,			5,  5,		    0, 3, 20, A_MAX_INT_WIS_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise skeleton warrior",			"lunatic", None, "Summons a permanent skeleton warrior",
	P_NECROMANCY_SPELL,			4,  5,		    0, 5, 50, A_MAX_INT_WIS_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise skeleton lord",			"octopoid-patterned", None, "Summons a permanent skeleton lord",
	P_NECROMANCY_SPELL,			3,  5,		    0, 8, 150, A_MAX_INT_WIS_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("raise skeleton king",			"skeletal", None, "Summons a permanent skeleton king",
	P_NECROMANCY_SPELL,			2,  5,		    0, 10, 300, A_MAX_INT_WIS_CHA, 0, 0, 200, 0, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NONE, O2_NONE, O3_NONE),
SPELL("replenish undeath",         "skull-patterned", None, "Heals a target undead for 6d6 hit points",
	P_NECROMANCY_SPELL,			5,  3,			 0, 2, 10, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, IMMEDIATE, 0, 6, 6, 0, 0, 0, 0, S1_NONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
SPELL("greater undeath replenishment",   "bone-patterned", None, "Heals a target undead for 12d6 hit points",
	P_NECROMANCY_SPELL,			5,  6,			 2, 4, 20, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, IMMEDIATE, 0, 12, 6, 0, 0, 0, 0, S1_NONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),


/* books with fixed descriptions
 */
SPELL("blank paper", "plain", None, None, P_NONE, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, HI_PAPER, O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE),
/* tribute book for 3.6 */
OBJECT(OBJ("novel", "paperback"), None, None,
       BITS(0, 0, 1, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, MAT_PAPER),
       0, 0, 0, P1_NONE, SPBOOK_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 20,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
	   20, CLR_BRIGHT_BLUE, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE),
/* a special, one of a kind, spellbook */
OBJECT(OBJ("Book of the Dead", "papyrus"), None, None,
       BITS(0, 0, 1, 0, 1, CHARGED_NOT_CHARGED, 1, 1, 0, 0, 0, 0, P_NONE, MAT_PAPER),
       0, 0, 0, P1_NONE, SPBOOK_CLASS, 0, MULTIGEN_SINGLE, 0, 20, 10000,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0,
	   20, HI_PAPER, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE | O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE),
#undef SPELL

/* wands ... */
#define WAND(name,typ,charged,prob,cost,mgc,dir,dirsubtype,range,radius,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,sflags,metal,color,flags,flags2,flags3) \
    OBJECT(OBJ(name, typ),  None, None,                                             \
           BITS(0, 0, 1, 0, mgc, charged, 0, 0, 0, 0, dir, 0, P_NONE, metal),    \
           0, 0, 0, P1_NONE, WAND_CLASS, prob, MULTIGEN_SINGLE, 0, 6, cost, \
		   0, sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, 0, 0, 0, 0, sflags, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, range, radius, 0, 0, \
		   30, color, dirsubtype, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, flags, flags2, flags3)
WAND("light",           "glass", CHARGED_WAND_NORMAL_NODIR, 65, 100, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_GLASS, HI_GLASS, O1_NONE, O2_NONE, O3_NONE),
WAND("secret door detection",
                        "balsa", CHARGED_WAND_NORMAL_NODIR, 30, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
WAND("ore detection", 
				   "orichalcum", CHARGED_WAND_NORMAL_NODIR, 30, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_ORICHALCUM, HI_GOLD, O1_NONE, O2_NONE, O3_NONE),
WAND("enlightenment", "crystal", CHARGED_WAND_NORMAL_NODIR, 15, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_GLASS, HI_GLASS, O1_NONE, O2_NONE, O3_NONE),
WAND("create monster",  "maple", CHARGED_WAND_NORMAL_NODIR, 45, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
WAND("wishing",          "pine", CHARGED_WAND_NORMAL_NODIR, 5, 500, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
WAND("identify",	  "mithril", CHARGED_WAND_NORMAL_NODIR, 10, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("nothing",           "oak", CHARGED_WAND_NORMAL_DIR, 25, 100, 0, IMMEDIATE, 0,  0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
WAND("striking",        "ebony", CHARGED_1D45_45, 70, 150, 1, IMMEDIATE, 0, 10, 0, 3, 6, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
WAND("make invisible", "marble", CHARGED_WAND_NORMAL_DIR, 45, 150, 1, IMMEDIATE, 0,  8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_MINERAL, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE),
WAND("slow monster",      "tin", CHARGED_WAND_NORMAL_DIR, 50, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("speed monster",   "brass", CHARGED_WAND_NORMAL_DIR, 50, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE),
WAND("undead turning", "copper", CHARGED_WAND_NORMAL_DIR, 45, 150, 1, IMMEDIATE, 0,  8, 0, 3, 6, 0, 1, 50, 200, S1_NONE, MAT_COPPER, HI_COPPER, O1_NONE, O2_NONE, O3_NONE),
WAND("polymorph",      "silver", CHARGED_WAND_NORMAL_DIR, 45, 200, 1, IMMEDIATE, 0,  6, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_SILVER, HI_SILVER, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE),
WAND("cancellation", "platinum", CHARGED_WAND_NORMAL_DIR, 45, 200, 1, IMMEDIATE, 0, 10, 0, 0, 0, 0, 1, 20, 30, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, MAT_PLATINUM, CLR_WHITE, O1_LIGHTNING_RESISTANT | O1_POLYMORPH_RESISTANT | O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE),
WAND("teleportation", "iridium", CHARGED_WAND_NORMAL_DIR, 45, 200, 1, IMMEDIATE, 0, 10, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, CLR_BRIGHT_CYAN, O1_NONE, O2_NONE, O3_NONE),
WAND("opening",          "zinc", CHARGED_WAND_NORMAL_DIR, 25, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("locking",      "aluminum", CHARGED_WAND_NORMAL_DIR, 25, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("probing",       "uranium", CHARGED_1D15_15, 30, 200, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("digging",          "iron", CHARGED_WAND_NORMAL_DIR, 50, 150, 1, RAY, RAY_WND_DIGGING,			10, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("magic missile",   "steel", CHARGED_1D75_75, 50, 150, 1, RAY, RAY_WND_MAGIC_MISSILE,	12, 0, 2, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("fire",        "hexagonal", CHARGED_1D30_30, 40, 175, 1, RAY, RAY_WND_FIRE,				 8, 0, 4, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_FIRE_RESISTANT, O2_NONE, O3_NONE),
WAND("cold",            "short", CHARGED_1D6_6, 40, 175, 1, RAY, RAY_WND_COLD,				12, 0, 12, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_COLD_RESISTANT, O2_NONE, O3_NONE),
WAND("sleep",		    "runed", CHARGED_WAND_NORMAL_DIR, 45, 175, 1, RAY, RAY_WND_SLEEP,			10, 0, 0, 0, 0, 4, 6, 0, S1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND("disintegration",   "long", CHARGED_WAND_NORMAL_DIR, 5, 500, 1, RAY, RAY_WND_DISINTEGRATION,	 8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE),
WAND("lightning",      "curved", CHARGED_1D15_15, 40, 175, 1, RAY, RAY_WND_LIGHTNING,		14, 0, 6, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE),
WAND("death",	 "skull-headed", CHARGED_WAND_NORMAL_DIR, 5, 500, 1, RAY, RAY_WND_DEATH,			 8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_BONE, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE),
WAND("resurrection", 
	               "ornamental", CHARGED_WAND_NORMAL_DIR, 5, 200, 1, IMMEDIATE, 0,					 6, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
WAND("evaporation",    "flimsy", CHARGED_WAND_NORMAL_DIR, 20, 150, 1, RAY, RAY_WND_EVAPORATION,		10, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
/* extra descriptions, shuffled into use at start of new game */
WAND(None,             "forked", CHARGED_WAND_NORMAL_DIR, 0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, O1_NONE, O2_NONE, O3_NONE),
WAND(None,             "spiked", CHARGED_WAND_NORMAL_DIR, 0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, O1_NONE, O2_NONE, O3_NONE),
WAND(None,            "jeweled", CHARGED_WAND_NORMAL_DIR, 0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_MINERAL, O1_NONE, O2_NONE, O3_NONE),
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal,worth) \
    OBJECT(OBJ(name, None),  None, None,                                       \
           BITS(0, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, metal),    \
           0, 0, 0, P1_NONE, COIN_CLASS, prob, MULTIGEN_SINGLE, 0, 1, worth, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   0, HI_GOLD, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_READABLE)
COIN("gold piece", 1000, MAT_GOLD, 1),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc), None, None,                                             \
           BITS(0, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0,                              \
                HARDGEM(mohs), 0, WEP_SLING_BULLET, -P_SLING, glass),                     \
           0, 0, 0, P1_NONE, GEM_CLASS, prob, MULTIGEN_SINGLE, 0, 1, gval, \
		   0, 1, 3, 0, 1, 3, 0, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   nutr, color, WEP_SLING, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
#define ROCK(name,desc,power1,power2,power3,pflags,kn,prob,wt,gval, sdice, sdam, splus, ldice, ldam, lplus, mgc,nutr,mohs,glass,color,flags,flags2,flags3,powconfermask) \
    OBJECT(OBJ(name, desc),  None, None,                                            \
           BITS(kn, 1, 0, 0, mgc, CHARGED_NOT_CHARGED, 0, 0, 0,                           \
                HARDGEM(mohs), 0, WEP_SLING_BULLET, -P_SLING, glass), \
           power1, power2, power3, pflags, GEM_CLASS, prob, MULTIGEN_SINGLE, 0, wt, gval, \
		   0, sdice, sdam, splus, ldice, ldam, lplus, 0, 0, 0, 0, A1_NONE, 0, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
		   nutr, color, WEP_SLING, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)
GEM("dilithium crystal", "white",  2, 1, 2250, 15,  5, MAT_GEMSTONE, CLR_WHITE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("diamond",           "white",  3, 1, 2000, 15,  10, MAT_GEMSTONE, CLR_WHITE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("black pearl",		 "black",  3, 1, 1750, 15,  5, MAT_GEMSTONE, CLR_BLACK, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("ruby",                "red",  4, 1, 1500, 15,  9, MAT_GEMSTONE, CLR_RED, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("jacinth",          "orange",  3, 1, 1400, 15,  9, MAT_GEMSTONE, CLR_ORANGE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("sapphire",           "blue",  4, 1, 1300, 15,  9, MAT_GEMSTONE, CLR_BLUE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("black opal",        "black",  3, 1, 1200, 15,  8, MAT_GEMSTONE, CLR_BLACK, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("emerald",           "green",  5, 1, 1100, 15,  8, MAT_GEMSTONE, CLR_GREEN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("turquoise",         "green",  6, 1, 1000, 15,  6, MAT_GEMSTONE, CLR_GREEN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("citrine",          "yellow",  4, 1,  950, 15,  6, MAT_GEMSTONE, CLR_YELLOW, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("aquamarine",        "green",  6, 1,  900, 15,  8, MAT_GEMSTONE, CLR_GREEN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("amber",   "yellowish brown",  8, 1,  850, 15,  2, MAT_GEMSTONE, CLR_BROWN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("topaz",   "yellowish brown", 10, 1,  800, 15,  8, MAT_GEMSTONE, CLR_BROWN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("jet",               "black", 10, 1,  750, 15,  7, MAT_GEMSTONE, CLR_BLACK, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("opal",              "white", 10, 1,  700, 15,  6, MAT_GEMSTONE, CLR_WHITE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("chrysoberyl",      "yellow", 10, 1,  650, 15,  5, MAT_GEMSTONE, CLR_YELLOW, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("garnet",              "red", 10, 1,  600, 15,  7, MAT_GEMSTONE, CLR_RED, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("amethyst",         "violet", 10, 1,  550, 15,  7, MAT_GEMSTONE, CLR_MAGENTA, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("jasper",              "red", 10, 1,  500, 15,  7, MAT_GEMSTONE, CLR_RED, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("fluorite",         "violet", 10, 1,  400, 15,  4, MAT_GEMSTONE, CLR_MAGENTA, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("pearl",			 "white", 10, 1,  300, 15,  5, MAT_GEMSTONE, CLR_WHITE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("obsidian",          "black", 10, 1,  200, 15,  6, MAT_GEMSTONE, CLR_BLACK, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("agate",            "orange", 10, 1,  200, 15,  6, MAT_GEMSTONE, CLR_ORANGE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("jade",              "green", 10, 1,  300, 15,  6, MAT_GEMSTONE, CLR_GREEN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of white glass", "white",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_WHITE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of blue glass", "blue",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_BLUE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of red glass", "red",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_RED, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of yellowish brown glass", "yellowish brown",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_BROWN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of orange glass", "orange",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_ORANGE, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of yellow glass", "yellow",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_YELLOW, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of black glass", "black",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_BLACK, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of green glass", "green",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_GREEN, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),
GEM("worthless piece of violet glass", "violet",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_MAGENTA, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, PERMITTED_ALL),

/* Placement note: there is a wishable subrange for
 * "gray stones" in the o_ranges[] array in objnam.c
 * that is currently everything between luckstones and flint
 * (inclusive).
 */
ROCK("luckstone", "gray",	//Gray stone start marker
	NO_POWER, NO_POWER, NO_POWER, P1_CONFERS_LUCK | P1_LUCK_APPLIES_WHEN_CARRIED | P1_CURSED_ITEM_YIELDS_NEGATIVE,
	0,  10,  10, 60, 1, 3, 0, 1, 3, 0, 1, 10, 7, MAT_MINERAL, CLR_GRAY, 
	O1_THROWN_WEAPON_ONLY, 
	O2_GRAYSTONE, O3_NONE, PERMITTED_ALL),
ROCK("loadstone", "gray", 
	NO_POWER, NO_POWER, NO_POWER, P1_NONE,
	0,  10, 500,  1, 1, 10, 0, 1, 10, 0, 1, 10, 6, MAT_MINERAL, CLR_GRAY,
	O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED | O1_CANNOT_BE_DROPPED_IF_CURSED | O1_THROWN_WEAPON_ONLY,
	O2_GRAYSTONE | O2_CURSED_MAGIC_ITEM | O2_GENERATED_CURSED, O3_NONE, PERMITTED_ALL),
ROCK("jinxstone", "gray",  
	FUMBLING, NO_POWER, NO_POWER, P1_POWER_1_APPLIES_WHEN_CARRIED | P1_CONFERS_UNLUCK | P1_LUCK_APPLIES_WHEN_CARRIED,
	0,  10,  10,  1, 1, 3, 0, 1, 3, 0, 1, 10, 6, MAT_MINERAL, CLR_GRAY, 
	O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED | O1_CANNOT_BE_DROPPED_IF_CURSED | O1_THROWN_WEAPON_ONLY,
	O2_GRAYSTONE | O2_CURSED_MAGIC_ITEM | O2_GENERATED_CURSED, O3_NONE, PERMITTED_ALL),
ROCK("touchstone", "gray", 
	0, 0, 0, P1_NONE, 0,   8,  10, 45, 1, 3, 0, 1, 3, 0, 1, 10, 6, MAT_MINERAL, CLR_GRAY, O1_THROWN_WEAPON_ONLY, O2_GRAYSTONE, O3_NONE, PERMITTED_ALL),
ROCK("flint", "gray",      //Gray stone end marker 
	0, 0, 0, P1_NONE, 0,   8,  10,  1, 1, 8, 0, 1, 8, 0, 0, 10, 7, MAT_MINERAL, CLR_GRAY, O1_THROWN_WEAPON_ONLY, O2_GRAYSTONE, O3_NONE, PERMITTED_ALL),
ROCK("rock", None,         
	0, 0, 0, P1_NONE, 1,  30,  10,  0, 1, 6, 0, 1, 6, 0, 0, 10, 7, MAT_MINERAL, CLR_GRAY, O1_THROWN_WEAPON_ONLY, O2_ROCK, O3_NONE, PERMITTED_ALL),
ROCK("stone pebble", None,         
	0, 0, 0, P1_NONE, 1,  43,	  2, 0, 1, 4, 0, 1, 4, 0, 0, 10, 7, MAT_MINERAL, CLR_GRAY, O1_THROWN_WEAPON_ONLY, O2_ROCK, O3_NONE, PERMITTED_ALL),
ROCK("clay pebble", None,         
	0, 0, 0, P1_NONE, 1,  43,   2, 0, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_MINERAL, CLR_BROWN, O1_THROWN_WEAPON_ONLY, O2_ROCK, O3_NONE, PERMITTED_ALL),
ROCK("nugget of iron ore", None,         //Ore startmarker
	0, 0, 0, P1_NONE, 1,  10,  16,    1, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_IRON, HI_METAL, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of copper ore", None,
	0, 0, 0, P1_NONE, 1,   9,  16,    2, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_COPPER, HI_GOLD, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of silver ore", None,
	0, 0, 0, P1_NONE, 1,   5,  16,    5, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_SILVER, HI_SILVER, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of gold ore", None,
	0, 0, 0, P1_NONE, 1,   4,  16,   25, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_GOLD, HI_GOLD, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of platinum ore", None,
	0, 0, 0, P1_NONE, 1,   3,  16,   50, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_PLATINUM, HI_SILVER, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of orichalcum ore", None,
	0, 0, 0, P1_NONE, 1,   2,  16,  200, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_ORICHALCUM, HI_SILVER, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of adamantium ore", None,
	0, 0, 0, P1_NONE, 1,   2,  16,  150, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_ADAMANTIUM, HI_SILVER, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),
ROCK("nugget of mithril ore", None,		//Ore endmarker
	0, 0, 0, P1_NONE, 1,   2,  16,  100, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_MITHRIL, HI_SILVER, O1_NONE, O2_ORE, O3_NONE, PERMITTED_ALL),


#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
OBJECT(OBJ("boulder", None), None, None,
       BITS(1, 0, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 1, 0, 0, 0, P_NONE, MAT_MINERAL),
	   0, 0, 0, P1_NONE, ROCK_CLASS, 100, MULTIGEN_SINGLE, 0, 8000, 0,
	   AD_PHYS, 2, 6, 8, 2, 6, 8, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   2000, HI_MINERAL, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE),
OBJECT(OBJ("statue", None), None, None,
       BITS(1, 0, 0, 1, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, MAT_MINERAL),
	   0, 0, 0, P1_NONE, ROCK_CLASS, 900, MULTIGEN_SINGLE, 0, 4000, 0,
	   AD_PHYS, 1, 20, 0, 1, 20, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   2500, CLR_WHITE, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),
OBJECT(OBJ("heavy iron ball", None), None, None,
       BITS(1, 0, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, WHACK, 0, P_NONE, MAT_IRON),
	   0, 0, 0, P1_NONE, BALL_CLASS, 1000, MULTIGEN_SINGLE, 0, 800, 10,
	   AD_PHYS, 1, 25, 0, 1, 25, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  200, HI_METAL, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE),
        /* +d4 when "very heavy" */
OBJECT(OBJ("iron chain", None), None, None,
       BITS(1, 0, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, WHACK, 0, P_NONE, MAT_IRON),
	   0, 0, 0, P1_NONE, CHAIN_CLASS, 1000, MULTIGEN_SINGLE, 0, 240, 0,
	   AD_PHYS, 1, 4, 0, 1, 4, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   200, HI_METAL, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),
        /* +1 both l & s */

/* Venom is normally a transitory missile (spit by various creatures)
 * but can be wished for in wizard mode so could occur in bones data.
 */
OBJECT(OBJ("blinding venom", "splash of venom"), None, None,
       BITS(0, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 1, 0, 0, 0, 0, P_NONE, MAT_LIQUID),
	   0, 0, 0, P1_NONE, VENOM_CLASS, 500, MULTIGEN_SINGLE, 0, 1, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, HI_ORGANIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),
OBJECT(OBJ("acid venom", "splash of venom"), None, None,
       BITS(0, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 1, 0, 0, 0, 0, P_NONE, MAT_LIQUID),
	   0, 0, 0, P1_NONE, VENOM_CLASS, 500, MULTIGEN_SINGLE, 0, 1, 0,
	   AD_PHYS, 2, 6, 0, 2, 6, 0, 0, 0, 0, 0, A1_NONE, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, HI_ORGANIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE),
        /* +d6 small or large */

/* Reagents here, which do not belong to any class, includes also all other odd non-food objects with no other apparent purpose*/
#define REAGENT(name, desc, known, prob, multigen, eatdelay, wt, cost, material, ediblesubtype, edibleeffect, ediblechance, nutrition, color, flags, flags2, flags3, powconfermask)         \
    OBJECT(OBJ(name, desc), None, None,                                      \
           BITS(known, 1, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, material), \
		   0, 0, 0, P1_NONE, REAGENT_CLASS, prob, multigen, eatdelay, wt, cost, \
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, ediblechance, \
		   0, 0, 0, 0, ediblesubtype, edibleeffect, 0, 0, 0, 0, 0, 0, \
           nutrition, color, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3)

	REAGENT("thread of spider silk", None, 1,	50, MULTIGEN_1D3, 0, 1, 10, MAT_SILK, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 2, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* STARTMARKER 1&2 */

	REAGENT("clump of bat guano", None, 1,	100, MULTIGEN_1D3, 2, 2,  10, MAT_ORGANIC, EDIBLE_ROTTEN, EDIBLE_NO_EFFECT, 0, 20, CLR_BROWN, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("ginseng root", None, 1, 150, MULTIGEN_1D3, 1, 3,  10, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_BROWN, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("mandrake root", None, 1, 50, MULTIGEN_1D2, 1, 3,  10, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_MAGENTA, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("heap of sporal powder", None, 1, 50, MULTIGEN_1D2, 1, 1,  10, MAT_ORGANIC, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 1, CLR_GRAY, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("sprig of wolfsbane", None, 1, 100, MULTIGEN_1D2, 1, 1,  10, MAT_VEGGY, EDIBLE_POISONOUS, EDIBLE_CURE_LYCANTHROPY, 100, 40, CLR_GREEN, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("clove of garlic", None, 1, 100, MULTIGEN_1D3, 1, 1,  10, MAT_VEGGY, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_WHITE, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("bone", None, 1, 100, MULTIGEN_1D3, 1, 16, 10, MAT_BONE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("human skull", None, 1, 50, MULTIGEN_SINGLE, 1, 32, 10, MAT_BONE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("piece of wood", None, 1, 100, MULTIGEN_1D3, 1, 16, 10, MAT_WOOD, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),
	REAGENT("pinch of sulfurous ash", None, 1, 100, MULTIGEN_1D3, 1, 1,  10, MAT_MINERAL, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 40, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL),

	REAGENT("modronite ball", "alien ball-shaped object",						0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),
	REAGENT("modronite monopole", "alien pole-shaped object",					0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),
	REAGENT("modronite tetrahedron", "alien pyramidal object",					0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),
	REAGENT("modronite cube", "alien cubic object",								0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),
	REAGENT("modronite octahedron", "alien eight-faced polyhedral object",		0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),
	REAGENT("modronite dodecahedron", "alien twelve-faced polyhedral object",	0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),
	REAGENT("modronite icosahedron", "alien twenty-faced polyhedral object",	0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 160, CLR_CYAN, O1_NONE, O2_MODRON_ITEM, O3_NONE, PERMITTED_ALL),

	REAGENT("feather", None, 1, 50, MULTIGEN_SINGLE, 0, 1, 10, MAT_ORGANIC, EDIBLE_NORMAL, EDIBLE_NO_EFFECT, 0, 10, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, PERMITTED_ALL), /* STARTMARKER 1&2 */

#undef REAGENT

/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
OBJECT(OBJ(None, None), None, None,
       BITS(0, 0, 0, 0, 0, CHARGED_NOT_CHARGED, 0, 0, 0, 0, 0, 0, P_NONE, 0),
	   0, 0, 0, P1_NONE, ILLOBJ_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 0, \
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, 0, \
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	   0, 0, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE)
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

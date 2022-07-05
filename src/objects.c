/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    objects.c    $NHDT-Date: 1535422421 2018/08/28 02:13:41 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.51 $ */
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
#include "animation.h"
#include "soundset.h"
#include "general.h"

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
#define OBJ(name,desc,contentname,contentdesc,itemdesc,height,stand_anim,enlarge,replacement)  name, desc, contentname, contentdesc, itemdesc, height, stand_anim, enlarge, replacement
#define OBJECT(obj,bits,prp1,prp2,prp3,pflags,sym,prob,multigen,dly,wt,cost, \
               dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct,  hitbon,mcadj,fixdmgbon,range,  oc1,oc2,oc3,oc4,oc5,oc6,oc7,oc8,  nut,color, soundset,  dirsubtype,materials,cooldown,special_quality,  powconfermask,permittedtargets,flags,flags2,flags3,flags4,flags5,flags6)  { obj }
#define None (char *) 0 /* less visual distraction for 'no description' */

NEARDATA struct objdescr obj_descr[] =
#else
/* second pass -- object definitions */
#define BITS(nmkn,mrg,uskn,ctnr,mgc,spetype,chrg,recharging,uniq,nwsh,big,tuf,dir,sub,skill,mtrl) \
  nmkn,mrg,uskn,0,mgc,spetype,chrg,recharging,uniq,nwsh,big,tuf,dir,mtrl,sub,skill /*SCO cpp fodder*/
#define OBJECT(obj,bits,prp1,prp2,prp3,pflags,sym,prob,multigen,dly,wt,cost,dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct,   hitbon,mcadj,fixdmgbon,range,  oc1,oc2,oc3,oc4,oc5,oc6,oc7,oc8,  nut,color,soundset,  dirsubtype,materials,cooldown,special_quality,  powconfermask,permittedtargets,flags,flags2,flags3,flags4,flags5,flags6) \
  { 0, 0, (char *) 0, bits, prp1, prp2, prp3, pflags, sym, dly, color, prob, wt, nut,  \
    cost, dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, hitbon, mcadj, fixdmgbon, range,   oc1, oc2, oc3, oc4, oc5, oc6, oc7, oc8,   dirsubtype, materials, cooldown, special_quality,  flags,flags2,flags3,flags4,flags5,flags6,powconfermask, permittedtargets, critpct, multigen, soundset }
#ifndef lint
#define HARDGEM(n) (n >= 8)
#else
#define HARDGEM(n) (0)
#endif

NEARDATA struct objclass objects[] =
#endif
{
    /* dummy object[0] -- description [2nd arg] *must* be NULL */
    OBJECT(OBJ("strange object", None, None, None, None, 0, 0, 0, 0), \
           BITS(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P_NONE, 0), \
           NO_POWER, NO_POWER, NO_POWER, P1_NONE, ILLOBJ_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           0, 0, OBJECT_SOUNDSET_NONE, \
           0, 0, 0, 0, \
           PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

    /* weapons ... */
#define CHARGEDWEAPON(name,desc,nmkn,mrg,mgc,charged,recharging,bi,prob,multigen,enchtype,wt,cost,\
                dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct, \
                hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
                power,power2,power3,pflags,typ,sub,skill,metal,color,height,soundset,\
                flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets) \
        OBJECT(OBJ(name,desc, None, None, None, height, 0, 0, 0),                                         \
               BITS(nmkn, mrg, 1, 0, mgc, enchtype, charged, recharging, 0, 0, bi, 0, typ, sub, skill, metal),  \
               power, power2, power3, pflags, WEAPON_CLASS, prob, multigen, 0, wt, cost, \
               dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, critpct, \
               hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
               wt, color, soundset, \
               0, 0, 0, 0, \
               powconfermask,permittedtargets, flags, flags2, flags3, flags4, flags5, flags6)

#define WEAPON(name,desc,nmkn,mrg,mgc,bi,prob,multigen,enchtype,wt,cost,\
                dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct, \
                hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
                power,power2,power3,pflags,typ,sub,skill,metal,color,height,soundset,\
                flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets) \
        CHARGEDWEAPON(name,desc,nmkn,mrg,mgc,CHARGED_NOT_CHARGED,RECHARGING_NOT_RECHARGEABLE,bi,prob,multigen,enchtype,wt,cost,\
                dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct, \
                hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
                power,power2,power3,pflags,typ,sub,skill,metal,color,height,soundset,\
                flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets)

    #define PROJECTILE(name,desc,  nmkn,mgc,prob,multigen,wt,cost,  dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct,  hitbon,mcadj,  metal,sub,launcher_skill,skill,color,height,soundset,  flags,flags2,flags3,flags4,flags5,flags6, permittedtargets) \
        OBJECT(OBJ(name,desc, None, None, None, height, 0, 0, 0),   \
               BITS(nmkn, 1, 1, 0, mgc, ENCHTYPE_PROJECTILE, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, PIERCE, sub, skill, metal), \
               NO_POWER, NO_POWER, NO_POWER, P1_NONE, WEAPON_CLASS, prob, multigen, 0, wt, cost, \
               dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, critpct, \
               hitbon, mcadj, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
               wt, color, soundset,\
               launcher_skill, 0, 0, 0, \
               PERMITTED_ALL, permittedtargets, flags, flags2, flags3, O4_MISSILE_TILE | flags4, flags5, flags6)


    #define BOW(name,desc,  nmkn,mgc,bi,prob,multigen,enchtype,wt,cost,  dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct,hitbon,mcadj,fixeddmgbon,launchrange,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,power,power2,power3,pflags,metal,sub,skill,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets) \
        OBJECT(OBJ(name,desc, None, None, None, height, 0, 0, 0),                                         \
               BITS(nmkn, 0, 1, 0, mgc, enchtype, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, bi, 0, WHACK, sub, skill, metal),      \
               power, power2, power3, pflags, WEAPON_CLASS, prob, multigen, 0, wt, cost,                            \
               dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, critpct, \
               hitbon, mcadj, fixeddmgbon, launchrange, 0, 0, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount,  \
               wt, color, soundset,\
               0, 0, 0, 0, \
               powconfermask, permittedtargets, flags, flags2, flags3, O4_NON_MYTHIC | flags4, flags5, flags6)

    /* Note: for weapons that don't do an even die of damage (ex. 2-7 or 3-18)
       the extra damage is added on in weapon.c, not here! */

    #define P PIERCE
    #define S SLASH
    #define B WHACK

/* missiles; materiel reflects the arrowhead, not the shaft */
PROJECTILE("arrow", None,
    1, 0, 38, MULTIGEN_2D6_3, 1, 2,
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, MAT_IRON, WEP_ARROW, P_BOW, -P_BOW, HI_METAL, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("elven arrow", "runed arrow",
    0, 0, 20, MULTIGEN_2D6_3, 1, 2, 
    AD_PHYS, 1, 7, 0, 1, 6, 0,     AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_METAL, WEP_ARROW, P_BOW, -P_BOW, HI_METAL, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, ALL_TARGETS),
PROJECTILE("orcish arrow", "crude arrow",
    0, 0, 20, MULTIGEN_2D6_3, 1, 2, 
    AD_PHYS, 1, 5, 0, 1, 6, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_IRON, WEP_ARROW, P_BOW, -P_BOW, CLR_BLACK, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, ALL_TARGETS),
PROJECTILE("silver arrow", None,
    1, 0, 8, MULTIGEN_1D6_2, 1, 5,  
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_SILVER, WEP_ARROW, P_BOW, -P_BOW, HI_SILVER, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("ya", "bamboo arrow",
    0, 0, 11, MULTIGEN_2D6_3, 1, 4, 
    AD_PHYS, 1, 7, 0, 1, 7, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    1, 0, MAT_METAL, WEP_ARROW, P_BOW, -P_BOW, HI_METAL, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("bone arrow", None,
    1, 0, 10, MULTIGEN_1D6_3, 1, 6, 
    AD_PHYS, 1, 4, 0, 1, 5, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_BONE, WEP_ARROW, P_BOW, -P_BOW, CLR_WHITE, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("enchanted arrow", None, /* Special item for display in arrow spells */
    1, 1, 0, MULTIGEN_SINGLE, 1, 50,
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_FORCEFIELD, WEP_ARROW, P_BOW, -P_BOW, HI_ZAP, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NO_GENERATION | O3_NO_WISH, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("crossbow bolt", None,
    1, 0, 35, MULTIGEN_2D6_3, 1, 2, 
    AD_PHYS, 1, 3, 1, 1, 4, 1, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_IRON, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, HI_METAL, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS), //1d3+1 //1d4+1
PROJECTILE("gnollish quarrel", "crude crossbow bolt",
    0, 0, 15, MULTIGEN_2D6_3, 1, 2, 
    AD_PHYS, 1, 2, 1, 1, 3, 1, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_IRON, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, HI_METAL, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, ALL_TARGETS), //1d2+1 //1d3+1
PROJECTILE("silver crossbow bolt", None,
    1, 0, 8, MULTIGEN_1D6_2, 1, 2, 
    AD_PHYS, 1, 3, 1, 1, 4, 1, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_SILVER, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, HI_SILVER, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS), //1d3+1 //1d4+1
PROJECTILE("bone quarrel", None,
    1, 0, 10, MULTIGEN_1D6_3, 1, 2, 
    AD_PHYS, 1, 2, 0, 1, 3, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_BONE, WEP_QUARREL, P_CROSSBOW, -P_CROSSBOW, CLR_WHITE, 16, OBJECT_SOUNDSET_ARROW,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, ALL_TARGETS), //1d2 //1d3
PROJECTILE("leaden sling-bullet", None,
    1, 0, 10, MULTIGEN_2D6_6, 2, 2, 
    AD_PHYS, 1, 6, 0, 1, 8, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_METAL, WEP_SLING_BULLET, P_SLING, -P_SLING, CLR_GRAY, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("iron sling-bullet", None,
    1, 0, 5, MULTIGEN_2D6_6, 2, 2, 
    AD_PHYS, 1, 8, 0, 1, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_IRON, WEP_SLING_BULLET, P_SLING, -P_SLING, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, ALL_TARGETS),
PROJECTILE("silver sling-bullet", None,
    1, 0, 5, MULTIGEN_2D6_1, 2, 2, 
    AD_PHYS, 1, 7, 0, 1, 9, 0, AD_PHYS,    0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, MAT_SILVER, WEP_SLING_BULLET, P_SLING, -P_SLING, HI_SILVER, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, ALL_TARGETS),

/* missiles that don't use a launcher */
WEAPON("dart", None,
    1, 1, 0, 0, 50, MULTIGEN_1D10, ENCHTYPE_THROWN_WEAPON, 1, 2,
    AD_PHYS,    1, 2, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DART,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, P, WEP_DART, -P_THROWN_WEAPON, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("shuriken", "throwing star",
    0, 1, 0, 0, 24, MULTIGEN_1D8, ENCHTYPE_THROWN_WEAPON, 1, 6,
    AD_PHYS, 1, 5, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_SHURIKEN,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, P, WEP_SHURIKEN, -P_THROWN_WEAPON, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC,
    O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("boomerang", None,
    1, 1, 0, 0, 10, MULTIGEN_SINGLE, ENCHTYPE_THROWN_WEAPON, 5, 20,
    AD_PHYS, 1, 9, 0, 1, 9, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, WEP_BOOMERANG, -P_THROWN_WEAPON, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_GENERIC,
    O1_THROWN_WEAPON_ONLY | O1_RETURNS_TO_HAND_AFTER_THROWING, O2_NONE, O3_NONE, O4_MISSILE_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),

/* spears [note: javelin used to have a separate skill from spears,
   because the latter are primarily stabbing weapons rather than
   throwing ones; but for playability, they've been merged together
   under spear skill and spears can now be thrown like javelins] */
/* Note from JG: Javelins are now marked as thrown weapons while spears are not, but spears can still be thrown relatively far */
WEAPON("spear", None,
    1, 1, 0, 0, 45, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 30, 3,
    AD_PHYS,    1, 6, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, -60, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven spear", "runed spear",
    0, 1, 0, 0, 12, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_ELVEN_MELEE_WEAPON, 30, 3,
    AD_PHYS, 1, 7, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, -65, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("orcish spear", "crude spear",
    0, 1, 0, 0, 13, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 30, 3,
    AD_PHYS, 1, 5, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, -55, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_IRON, CLR_BLACK, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish spear", "stout spear",
    0, 1, 0, 0, 12, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 35, 3,
    AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver spear", None,
    1, 1, 0, 0,  5, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 36, 40,
    AD_PHYS, 1, 6, 0,  1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, -60, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SPEAR, P_SPEAR, MAT_SILVER, HI_SILVER, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("javelin", "throwing spear",
    0, 1, 0, 0, 10, MULTIGEN_SINGLE, ENCHTYPE_THROWN_WEAPON, 20, 3,
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, -100, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_JAVELIN, P_SPEAR, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* spearish; doesn't stack, not intended to be thrown */
WEAPON("trident", None,
    1, 0, 0, 0,  6, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 25, 5,
    AD_PHYS, 1, 6, 1, 3,  4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_TRIDENT, P_SPEAR, MAT_IRON, HI_METAL, 20, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver fork", None,
    1, 0, 0, 0, 2, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 5, 40,
    AD_PHYS, 1, 2, 0, 0, 0, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_FORK, P_SPEAR, MAT_SILVER, HI_SILVER, 24, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NON_MYTHIC, O5_NONE, O6_NONE, 
    PERMITTED_ALL, ALL_TARGETS),


/* blades; all stack */
WEAPON("dagger", None,
    1, 1, 0, 0, 13, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10, 4,
    AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven dagger", "runed dagger",
    0, 1, 0, 0, 10, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_ELVEN_MELEE_WEAPON, 10, 4,
    AD_PHYS, 1, 5, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_ELVEN_ITEM, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven runedagger", "runed dagger", /* Base item for Sting */
    0, 1, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_ELVEN_MELEE_WEAPON, 10, 4,
    AD_PHYS, 1, 5, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    4, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_ELVEN_ITEM, O3_NO_WISH | O3_NO_GENERATION, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("orcish dagger", "crude dagger",
    0, 1, 0, 0, 10, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10, 4,
    AD_PHYS, 1, 3, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, CLR_BLACK, 16, OBJECT_SOUNDSET_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_ORCISH_ITEM, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("ornamental orcish dagger", "crudely decorated dagger", /* Base weapon for Grimtooth */
    0, 1, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10, 4,
    AD_PHYS, 3, 3, 0, 3, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, CLR_BLACK, 16, OBJECT_SOUNDSET_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_ORCISH_ITEM, O3_NO_WISH | O3_NO_GENERATION, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("bone dagger", None,
    1, 1, 0, 0, 6, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10, 4,
    AD_PHYS, 1, 3, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_BONE, CLR_WHITE, 16, OBJECT_SOUNDSET_BONE_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver dagger", None,
    1, 1, 0, 0, 3, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 12,  40,
    AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_THROWN_DAGGER,
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_SILVER, HI_SILVER, 16, OBJECT_SOUNDSET_DAGGER,
    O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("wraithblade", "black dagger",
    0, 0, 1, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10, 100,
    AD_PHYS, 1, 4, 0, 1, 3, 0, AD_PHYS, 1, 3, 1, A1_WOUNDING, A2_NONE, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_METAL, CLR_BLACK, 16, OBJECT_SOUNDSET_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_GENERATED_INFERNAL | O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("main-gauche", None,
    1, 1, 0, 0, 4, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10,   4,
    AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_DAGGER, P_DAGGER, MAT_IRON, HI_METAL,16, OBJECT_SOUNDSET_DAGGER,
    O1_IS_ARMOR_WHEN_WIELDED, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("athame", None,
    1, 1, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 10, 4,
    AD_PHYS, 1, 4, 0, 1,  3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S,WEP_KNIFE, P_DAGGER, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("scalpel", None,
    1, 1, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 5, 6,
    AD_PHYS, 1, 3, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S,WEP_KNIFE, P_DAGGER, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("knife", None,
    1, 1, 0, 0, 8, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 5, 4,
    AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P|S, WEP_KNIFE, P_DAGGER, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver knife", None,
    1, 1, 0, 0, 2, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 5, 40,
    AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P|S, WEP_KNIFE, P_DAGGER, MAT_SILVER, HI_SILVER, 16, OBJECT_SOUNDSET_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("stiletto", None,
    1, 1, 0, 0, 5, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 5,   4,
    AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P|S, WEP_KNIFE, P_DAGGER, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
/* 3.6: worm teeth and crysknives now stack;
   when a stack of teeth is enchanted at once, they fuse into one crysknife;
   when a stack of crysknives drops, the whole stack reverts to teeth -- OBSOLETE in GnollHack - JG */
WEAPON("worm tooth", None,
    1, 1, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 20,   2,
    AD_PHYS, 1, 2, 0, 1,  2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_WORM_TOOTH, P_DAGGER, MAT_TOOTH, CLR_WHITE, 16, OBJECT_SOUNDSET_BONE_DAGGER,
    O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("crysknife", None,
    1, 1, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 20, 100,
    AD_PHYS, 1, 10, 0, 1, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_WORM_TOOTH, P_DAGGER, MAT_TOOTH, CLR_WHITE, 16, OBJECT_SOUNDSET_BONE_DAGGER,
    O1_NONE, O1_MELEE_AND_THROWN_WEAPON, O3_NONE, O4_MISSILE_TILE | O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* axes */
WEAPON("axe", None,
    1, 0, 0, 0, 19, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 50, 8,
    AD_PHYS, 1, 6, 1, 1, 8, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_AXE, P_AXE, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_AXE,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish axe", "broad axe",
    0, 0, 0, 0, 3, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 50, 8,
    AD_PHYS, 1, 6, 2, 1, 8, 2, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_AXE, P_AXE, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_AXE,
    O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("throwing axe", None,
    1, 0, 0, 0, 8, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 50, 8,
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_THROWING_AXE, P_AXE, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_AXE,
    O1_MELEE_AND_THROWN_WEAPON, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("battle-axe", "double-headed axe",       /* "double-bitted"? */
    0, 0, 0, 1, 8, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 100, 40,
    AD_PHYS, 2, 8, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_AXE, P_AXE, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_AXE,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* swords */
WEAPON("short sword", None,
    1, 0, 0, 0, 30, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 45,  10,
    AD_PHYS, 1, 6, 0, 1,  8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven short sword", "runed short sword",
    0, 0, 0, 0,  3, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_ELVEN_MELEE_WEAPON, 40,  10,
    AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("orcish short sword", "crude short sword",
    0, 0, 0, 0,  3, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 50,  10,
    AD_PHYS, 1, 5, 0, 1,  8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_IRON, CLR_BLACK, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish short sword", "broad short sword",
    0, 0, 0, 0, 4, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 50, 10,
    AD_PHYS, 1, 7, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_SHORT_SWORD, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("scimitar", "curved sword",
    0, 0, 0, 0, 12, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 15,
    AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_SCIMITAR, P_SWORD, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver saber", None,
    1, 0, 0, 0, 8, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 55, 75,
    AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_SABER, P_SWORD, MAT_SILVER, HI_SILVER, 24, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("broadsword", None,
    1, 0, 0, 0, 13, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 10,
    AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_BROADSWORD, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("elven broadsword", "runed broadsword",
    0, 0, 0, 0, 6, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_ELVEN_MELEE_WEAPON, 55, 10,
    AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_BROADSWORD, P_SWORD, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("long sword", None,
    1, 0, 0, 0, 34, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 15,
    AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver long sword", None,
    1, 0, 0, 0, 6, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 55, 150,
    AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_SILVER, HI_SILVER, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("sword of holy vengeance", "shining long sword",
    0, 0, 1, 0, 2, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 1000,
    AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, A2_NONE, 0,
    0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
    WARN_UNDEAD, WARN_DEMON, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_SILVER, HI_SILVER, 16, OBJECT_SOUNDSET_SWORD,
    O1_NOT_CURSEABLE | O1_ENCHANTMENT_AFFECTS_MC | O1_FLICKERS_WHEN_MONSTERS_DETECTED,
    O2_ANGELIC_ITEM | O2_GENERATED_BLESSED | O2_SHINES_MAGICAL_LIGHT | O2_FLICKER_COLOR_WHITE,
    O3_TARGET_PERMISSION_IS_M2_FLAG | O3_PERMTTED_TARGET_CHAOTIC, 
    O4_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS | O4_INAPPROPRIATE_CHARACTERS_CANT_HANDLE | O4_FLOOR_TILE | O4_RARE, O5_NONE, O6_NONE,
    PERMITTED_ALIGNMENT_LAWFUL, M2_UNDEAD | M2_DEMON),
WEAPON("sword of unholy desecration", "black long sword", // STARTMARKER FOR SHUFFLED SWORDS
    0, 0, 1, 0, 2, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 900,
    AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_LEVEL_DRAIN | A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, A2_NONE, 0,
    0, -4, 0, 0, 5, 0, 0, 0, 0, 0, 0,
    WARN_ANGEL, NO_POWER, NO_POWER, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_METAL, CLR_BLACK, 16, OBJECT_SOUNDSET_SWORD,
    O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT | O1_FLICKERS_WHEN_MONSTERS_DETECTED,
    O2_DEMON_ITEM | O2_FLICKER_COLOR_BLACK,
    O3_TARGET_PERMISSION_IS_M2_FLAG | O3_PERMTTED_TARGET_LAWFUL | O3_PERMTTED_TARGET_NEUTRAL,
    O4_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS | O4_INAPPROPRIATE_CHARACTERS_CANT_HANDLE | O4_FLOOR_TILE | O4_RARE, O5_NONE, O6_NONE,
    PERMITTED_ALIGNMENT_CHAOTIC, M2_ANGEL),
CHARGEDWEAPON("nine lives stealer", "black-hilted long sword", // ENDMARKER FOR SHUFFLED SWORDS
    0, 0, 1, CHARGED_1D8_1, RECHARGING_NINE_LIVES_STEALER, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 1350,
    AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_MAGIC_RESISTANCE_PROTECTS, A2_REQUIRES_AND_EXPENDS_A_CHARGE, 100,
    0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_IRON, CLR_BLACK, 16, OBJECT_SOUNDSET_SWORD,
    O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NO_GENERATION, O4_FLOOR_TILE | O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPON("glass sword", None,
    1, 0, 1, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 20, 500,
    AD_PHYS, 1, 8, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, P1_NONE, S, WEP_LONG_SWORD, P_SWORD, MAT_GLASS, HI_GLASS, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_GENERATED_DEATH_OR_COLD_ENCHANTED, O3_NO_GENERATION, O4_NEVER_GENERATED_WITH_EXCEPTIONALITY | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("two-handed sword", None,
    1, 0, 0, 1, 22, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 120, 50,
    AD_PHYS, 2, 6, 0, 3, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_SWORD, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("two-handed gemstone sword", None, /* Base weapon for The Emerald Sword */
    1, 0, 0, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 90, 4000,
    AD_PHYS, 2, 6, 0, 3, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_SWORD, P_SWORD, MAT_GEMSTONE, CLR_RED, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NO_GENERATION | O3_NO_WISH, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("katana", "samurai sword",
    0, 0, 0, 0, 4, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 80, 80,
    AD_PHYS, 1, 10, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_KATANA, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword", /* Base item for Tsurugi of Muramasa */
    0, 0, 0, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 100, 500,
    AD_PHYS, 2, 8, 0, 3, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, P1_NONE, S, WEP_TSURUGI, P_SWORD,
    MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_SWORD, O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION,
    O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("runesword", "runed broadsword", /* Base item for Stormbringer */
    0, 0, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 55, 300,
    AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_BROADSWORD, P_SWORD, MAT_IRON, CLR_BLACK, 16, OBJECT_SOUNDSET_SWORD,
    O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_NON_MYTHIC | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
CHARGEDWEAPON("sword of luckiness", "bright broadsword", /* Base item for Luck Blade */
    0, 0, 0, CHARGED_LUCK_BLADE, RECHARGING_LUCK_BLADE, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 55, 500,
    AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_CONFERS_LUCK | P1_CURSED_ITEM_YIELDS_NEGATIVE, S, WEP_BROADSWORD, P_SWORD, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_SWORD,
    O1_ENCHANTMENT_AFFECTS_MC, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_NON_MYTHIC | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
    0, 0, 0, 1, 4, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 80, 10,
    AD_PHYS, 2, 6, 0, 2, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("ranseur", "hilted polearm",
    0, 0, 0, 1, 4, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 50, 6, 
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("spetum", "forked polearm",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 50, 6,
    AD_PHYS, 2, 5, 1, 2, 7, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("glaive", "single-edged polearm",
    0, 0, 0, 1, 7, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 75, 6,
    AD_PHYS, 2, 6, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("lance", None,
    1, 0, 0, 0, 4, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 180, 10,
    AD_PHYS, 1, 6, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P, WEP_LANCE, P_SPEAR, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
/* +2d10 when jousting with lance as primary weapon */
/* axe-type */
WEAPON("halberd", "angled poleaxe",
    0, 0, 0, 1, 8, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 150, 10,
    AD_PHYS, 2, 5, 0, 3, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P|S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("bardiche", "long poleaxe",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 120, 7,
    AD_PHYS, 2, 4, 0, 3, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S,WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("voulge", "pole cleaver",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 125, 5,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("dwarvish mattock", "broad pick",
    0, 0, 0, 1, 11, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 120, 50,
    AD_PHYS, 2, 6, 0, 3, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_PICK_AXE, P_PICK_AXE, MAT_IRON, HI_METAL, 38, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
/* curved/hooked */
WEAPON("fauchard", "pole sickle",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 60, 5,
    AD_PHYS, 2, 6, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P | S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("guisarme", "pruning hook",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 80, 5,
    AD_PHYS, 3, 4, 0, 2, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, S,WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("bill-guisarme", "hooked polearm",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 120, 7,
    AD_PHYS, 3, 4, 0, 2, 10, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P | S, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("ancus", "black ornamental hooked polearm",
    0, 0, 1, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 100, 100,
    AD_PHYS, 1, 6, 1, 1, 8, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, P | S, WEP_POLEARM, P_POLEARM, MAT_IRON, CLR_BLACK, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_DEMON_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
/* other */
WEAPON("lucern hammer", "pronged polearm",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 150, 7,
    AD_PHYS, 3, 4, 0, 2, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B | P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("bec de corbin", "beaked polearm",
    0, 0, 0, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 100, 1,
    AD_PHYS, 2, 8, 1, 2, 8, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B | P, WEP_POLEARM, P_POLEARM, MAT_IRON, HI_METAL, 28, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* bludgeons */
WEAPON("mace", None,
    1, 0, 0, 0, 35, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 5,
    AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
CHARGEDWEAPON("runed sceptre", None,
    1, 0, 0, CHARGED_ALWAYS_9, RECHARGING_GENERAL, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 0,
    AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_GOLD, HI_GOLD, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver mace", None,
    1, 0, 0, 0, 3, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 55, 50,
    AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_SILVER, HI_SILVER, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("heavenly oak mace", "wooden mace", /* Base item for Mace of Saint Cuthbert */
    0, 0, 1, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 1000,
    AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_ONE_FOURTH_MAX_HP_DAMAGE_TO_UNIQUE_MONSTERS, A2_NONE, CRITICAL_STRIKE_SPECIAL_PERCENTAGE_HIT_DICE_SAVES,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_WOOD, HI_WOOD, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NOT_CURSEABLE,
    O2_SHINES_MAGICAL_LIGHT | O2_FLICKER_COLOR_WHITE | O2_GENERATED_BLESSED,
    O3_TARGET_PERMISSION_IS_M2_FLAG | O3_NO_WISH | O3_NO_GENERATION, 
    O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, M2_UNDEAD | M2_DEMON),
CHARGEDWEAPON("mace of the underworld", "skull-headed obsidian mace", /* Base weapon for Wand of Orcus */
    0, 0, 1, CHARGED_ALWAYS_7, RECHARGING_WAND_OF_ORCUS, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 40, 1500,
    AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_MAGIC_RESISTANCE_PROTECTS, A2_NONE, 15,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_MACE, P_BLUDGEONING_WEAPON, MAT_BONE, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE, O2_DEMON_ITEM, O3_NO_WISH | O3_NO_GENERATION, 
    O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("morning star", None,
    1, 0, 0, 0, 10, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 10,
    AD_PHYS, 2, 4, 0, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_MORNING_STAR, P_FLAIL, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("war hammer", None,
    1, 0, 0, 0, 11, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 50, 5,
    AD_PHYS, 1, 6, 1, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_HAMMER, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("heavy war hammer", "runed war hammer", /* Base object for Mjollnir */
    0, 0, 1, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 100, 1000,
    AD_PHYS, 1, 8, 1, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_HAMMER, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE | O1_MELEE_AND_THROWN_WEAPON | O1_WEIGHT_DOES_NOT_REDUCE_RANGE | O1_RETURNS_TO_HAND_AFTER_THROWING | O1_CAN_BE_THROWN_ONLY_IF_WIELDED,
    O2_NONE, 
    O3_NO_WISH | O3_NO_GENERATION,
    O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ROLE_VALKYRIE, ALL_TARGETS),
WEAPON("club", None,
    1, 0, 0, 0, 12, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 40, 3,
    AD_PHYS, 1, 6, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_CLUB, P_BLUDGEONING_WEAPON, MAT_WOOD, HI_WOOD, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("jagged-toothed club", "black jagged-toothed club",
    0, 0, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 60, 100,
    AD_PHYS, 1, 6, 1, 1, 6, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_SPIKED_CLUB, P_BLUDGEONING_WEAPON, MAT_WOOD, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_DEMON_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("rubber hose", None,
    1, 0, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 20, 3,
    AD_PHYS, 1, 4, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_HOSE, P_WHIP, MAT_PLASTIC, CLR_BROWN, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_FLIMSY, O3_NONE, O4_NONE, O5_NONE, O6_NONE, 
    PERMITTED_ALL, ALL_TARGETS),
    
/* staffs */
WEAPON("flindbar", "iron staff",
    0, 0, 0, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 160, 800,
    AD_PHYS, 2, 6, 0, 2, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_ROD, P_QUARTERSTAFF, MAT_IRON, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("quarterstaff", "wooden staff",
    0, 0, 0, 1, 11, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 5,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 16, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of the magi", "runed staff", //STARTMARKER FOR SHUFFLED STAFFS
    0, 0, 1, 1, 2, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 1500,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_ELEC, 4, 6, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 50, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 24, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_MANA_PERCENTAGE_BONUS, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 16, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_COLD_RESISTANT | O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT, 
    O2_NONE, O3_NONE, O4_FLOOR_TILE | O4_RARE, O5_NONE, O6_NONE,
    PERMITTED_ROLE_WIZARD, ALL_TARGETS),
WEAPON("staff of fire", "ornamental staff",
    0, 0, 1, 1, 9, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 250,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_FIRE, 1, 6, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 4, 0, 0,
    IMPROVED_FIRE_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 16, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_FIRE_RESISTANT | O1_ENCHANTMENT_AFFECTS_MC, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of thunder and lightning", "forked staff",
    0, 0, 1, 1, 5, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 500,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_ELEC, 2, 6, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 8, 0, 0,
    IMPROVED_SHOCK_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 20, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_LIGHTNING_RESISTANT | O1_ENCHANTMENT_AFFECTS_MC, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of frost", "blue-hued staff",
    0, 0, 1, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 1000,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_COLD, 3, 6, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 12, 0, 0,
    IMPROVED_COLD_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 16, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("staff of life", "serpent-entwined staff",
    0, 0, 1, 1, 7, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 500,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 16, 0, 0,
    DRAIN_RESISTANCE, DEATH_RESISTANCE, REGENERATION, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 32, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, 
    O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, M2_UNDEAD),
WEAPON("staff of withering", "twisted staff", //ENDMARKER FOR SHUFFLED STAFFS
    0, 0, 1, 1, 3, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 30, 1500,
    AD_PHYS, 2, 4, 0, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_LEVEL_DRAIN, A2_NONE, 0,
    0, -1, 0, 0, 0, 0, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 20, 0, 0,
    DRAIN_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, B, WEP_STAFF, P_QUARTERSTAFF, MAT_WOOD, HI_WOOD, 16, OBJECT_SOUNDSET_QUARTERSTAFF,
    O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* two-piece */
WEAPON("aklys", "thonged club",
    0, 0, 0, 0, 8, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 15, 4,
    AD_PHYS, 1, 6, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_SPIKED_CLUB, P_BLUDGEONING_WEAPON, MAT_IRON, HI_METAL, 36, OBJECT_SOUNDSET_GENERIC,
    O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_MISSILE_TILE | O4_TETHERED_WEAPON | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("flail", None,
    1, 0, 0, 0, 33, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 35, 4,
    AD_PHYS, 1, 6, 1, 2, 4, 0,     AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("silver flail", None,
    1, 0, 0, 0, 4, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 35, 40,
    AD_PHYS, 1, 6, 1, 2, 4, 0,     AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_SILVER, HI_SILVER, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("double-headed flail", None,
    1, 0, 0, 0, 3, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 55, 500,
    AD_PHYS, 1, 6, 1, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_MELEE_DOUBLE_HEADED_FLAIL, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_DOUBLE_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
    /* one-handed, strikes up to two times, -1 hit bonus (difficult to wield) */
WEAPON("triple-headed flail", None, /* Also a base item for Triple-Headed Flail of Yeenaghu */
    1, 0, 0, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 70, 2000,
    AD_PHYS, 1, 6, 1, 2, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, 
    -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, MULTISHOT_MELEE_TRIPLE_HEADED_FLAIL,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_TRIPLE_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
    /* one-handed, strikes up to three times, -2 hit bonus (difficult to wield) */
WEAPON("runed flail", None, /* Base item for Howling Flail */
    1, 0, 1, 0, 0, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 35, 1000,
    AD_PHYS, 1, 6, 1, 2, 4, 0,     AD_PHYS, 2, 4, 0, A1_NONE, A2_NONE, 0, 
    0, 0, 0, 0, 0, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE | IGNORE_ENCHANTMENT, 25, 0, 0, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, B, WEP_FLAIL, P_FLAIL, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE, O2_GNOLLISH_ITEM | O2_DEMON_ITEM, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_RACE_GNOLL, ALL_TARGETS),


/* misc */
WEAPON("bullwhip", None,
    1, 0, 0, 0, 2, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON, 20, 4,
    AD_PHYS, 1, 2, 0,  1, 1, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, B, WEP_WHIP, P_WHIP, MAT_LEATHER, CLR_BROWN, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_APPLIABLE, O4_NONE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),
WEAPON("black blade of disintegration", "sword-shaped planar rift",
       0, 0, 1, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_MELEE_WEAPON, 0, 0,
    AD_PHYS, 2, 8, 0,  3, 8, 0, AD_PHYS, 0, 0, 0, A1_CRITICAL_STRIKE | A1_CRITICAL_STRIKE_IS_DEADLY | A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK, A2_NONE, 100,
    0, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, P1_NONE, S, WEP_TWO_HANDED_SWORD, P_SWORD, MAT_PLANARRIFT, CLR_BLACK, 16, OBJECT_SOUNDSET_GENERIC,
    O1_DISINTEGRATION_RESISTANT | O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_NON_MYTHIC | O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* bows */
BOW("bow", None,                            
    1, 0, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 30, 30, 
    AD_PHYS, 1, 3, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_NONE, 0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("short bow", None,                        
    1, 0, 1, 15, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 30, 30,
    AD_PHYS, 1, 3, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -80,  0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("elven long bow", "runed long bow",        
    0, 0, 1, 12, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_ELVEN_LAUNCHER, 40, 300,
    AD_PHYS, 1, 3, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_NONE,
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("orcish short bow", "crude short bow",  
    0, 0, 1, 12, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 35, 25,
    AD_PHYS, 1, 5, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    -1, 0, 0, -80,  0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, CLR_BLACK, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("long bow", None,                        
    1, 0, 1, 12, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 45, 75,
    AD_PHYS, 1, 4, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("composite long bow", None,                
    1, 0, 1, 6, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 50, 250,
    AD_PHYS, 1, 5, 1, 1, 7, 1, AD_PHYS, 
    0, 0, 0, A1_NONE, A2_NONE, 0,        0, 0, 0, -130, 0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("divine long bow", "ornamental long bow",        /* Base item for Longbow of Diana*/
    0, 0, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 40, 300,
    AD_PHYS, 1, 4, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_LAUNCHER_MULTISHOT_BOW,
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NO_GENERATION | O3_NO_WISH, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("composite short bow", None,            
    1, 0, 1, 6, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 35, 100,
    AD_PHYS, 1, 4, 1, 1, 5, 1, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -90,  0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_SHORT_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("yumi", "oriental long bow",            
    0, 0, 1, 0, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 55, 150,
    AD_PHYS, 1, 4, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -120, 0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("sling", None,                            
    1, 0, 0, 30, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_LAUNCHER, 3, 20,
    AD_PHYS, 1, 2, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -60,  0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_LEATHER, WEP_SLING, P_SLING, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("crossbow", None,                        
    1, 0, 1, 30, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 100, 75,
    AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0,  6, 18,   0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_CROSSBOW, P_CROSSBOW, HI_WOOD, 0, OBJECT_SOUNDSET_CROSSBOW, 
    O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("hand crossbow", None,                    
    1, 0, 0, 5, MULTIGEN_SINGLE, ENCHTYPE_SINGLE_HANDED_LAUNCHER, 40, 50,
    AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0,    A1_NONE, A2_NONE, 0,        
    0, 0,  3,  9,    0, 0, 0, 0, 0, MULTISHOT_NONE,
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_HAND_CROSSBOW, P_CROSSBOW, HI_WOOD, 0, OBJECT_SOUNDSET_CROSSBOW, 
    O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("heavy crossbow", None,                    
    1, 0, 1, 10, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 200, 150,
    AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,       
    0, 0,  9, 24,   0, 0, 0, 0, 0, MULTISHOT_NONE, 
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_CROSSBOW, P_CROSSBOW, HI_WOOD, 0, OBJECT_SOUNDSET_CROSSBOW, 
    O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("repeating crossbow", None,                
    1, 0, 1, 5, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 180, 75,
    AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,       
    -2, 0,  4, 18,  0, 0, 0, 0, 0, MULTISHOT_LAUNCHER_REPEATING_CROSSBOW,
    0, 0, 0, P1_NONE, MAT_WOOD, WEP_REPEATING_CROSSBOW, P_CROSSBOW, HI_WOOD, 0, OBJECT_SOUNDSET_CROSSBOW, 
    O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("repeating heavy crossbow",  None, 
    1, 0, 1, 1, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_LAUNCHER, 360, 1000,
    AD_PHYS, 1, 8, 0, 1, 8, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,       
    -2, 0,  7, 24,    0, 0, 0, 0, 0, MULTISHOT_LAUNCHER_REPEATING_CROSSBOW,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WOOD, WEP_REPEATING_CROSSBOW, P_CROSSBOW, HI_WOOD, 0, OBJECT_SOUNDSET_CROSSBOW,
    O1_NONE, O2_NONE, O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH, O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOW("Galadhrim bow", "ornamental long bow",
    0, 1, 1, 1, MULTIGEN_SINGLE, ENCHTYPE_TWO_HANDED_ELVEN_LAUNCHER, 45, 500,
    AD_PHYS, 1, 4, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,        
    0, 0, 0, -100, 0, 0, 0, 0, 0, MULTISHOT_LAUNCHER_MULTISHOT_BOW, 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, MAT_WOOD, WEP_LONG_BOW, P_BOW, HI_WOOD, 0, OBJECT_SOUNDSET_BOW, 
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_VERY_RARE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL, ALL_TARGETS),

#undef P
#undef S
#undef B

#undef WEAPON
#undef CHARGEDWEAPON
#undef PROJECTILE
#undef BOW

/* armor ... */
        /* MAT_IRON denotes ferrous metals, including steel.
         * Only MAT_IRON weapons and armor can rust.
         * Only MAT_COPPER (including brass) corrodes.
         * Some creatures are vulnerable to MAT_SILVER.

         */

#define GENERAL_ARMOR(name,desc,kn,mgc,blk,power,power2,power3,pflags,enchtype,prob,delay,wt,  \
              cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,sub,skill,metal,c,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,anim,enl,repl)                   \
    OBJECT(OBJ(name, desc, None, None, None, height, anim, enl, repl),                                       \
           BITS(kn, 0, 1, 0, mgc, enchtype, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, blk, 0, 0, sub, skill, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, delay, wt, cost,                     \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 10 - ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, 0, \
           wt, c, soundset,\
           0, 0, 0, 0, \
           powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6 )

#define ARMOR(name,desc,kn,mgc,blk,power,power2,power3,pflags,enchtype,prob,delay,wt,  \
              cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,sub,skill,metal,c,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)                   \
        GENERAL_ARMOR(name,desc,kn,mgc,blk,power,power2,power3,pflags,enchtype,prob,delay,wt,  \
              cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,sub,skill,metal,c,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,0,0,0)
#define HELM(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,floor_height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_HELM, P_NONE, metal, c, floor_height, soundset, flags, flags2, flags3, flags4, flags5, flags6, powconfermask)
#define CLOAK(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_CLOAK, P_NONE, metal, c, 0, soundset, flags, flags2, flags3, flags4 | O4_NON_MYTHIC, flags5,flags6, powconfermask)
#define SHIELD(name,desc,kn,mgc,blk,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    ARMOR(name, desc, kn, mgc, blk, power, power2, power3, pflags, enchtype, prob, delay, wt, \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_SHIELD, P_NONE, metal, c, 0, soundset, flags, flags2, flags3, flags4, flags5, flags6, powconfermask)
#define GLOVES(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_GLOVES, P_NONE, metal, c, 24, soundset, flags, flags2, flags3, flags4 | O4_NON_MYTHIC | O4_FLOOR_TILE, flags5, flags6, powconfermask)
#define GENERAL_BOOTS(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,anim,enl,repl)  \
    GENERAL_ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_BOOTS, P_NONE, metal, c, 32, soundset, flags, flags2, flags3, flags4 | O4_NON_MYTHIC, flags5, flags6, powconfermask,anim,enl,repl)
#define BOOTS(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    GENERAL_BOOTS(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,0,0,0)
#define SHIRT(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
        ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_SHIRT, P_NONE, metal, c, 0, soundset, flags, flags2, flags3, flags4 | O4_NON_MYTHIC, flags5, flags6, powconfermask)
#define ROBE(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_ROBE, P_NONE, metal, c, height, soundset, flags, flags2, flags3, flags4 | O4_NON_MYTHIC, flags5, flags6, powconfermask)
#define BRACERS(name,desc,kn,mgc,power,power2,power3,pflags,enchtype,prob,delay,wt,cost,ac,mgccancel,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,c,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    ARMOR(name, desc, kn, mgc, 0, power, power2, power3, pflags, enchtype, prob, delay, wt,  \
          cost, ac, mgccancel, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_BRACERS, P_NONE, metal, c, 24, soundset, flags, flags2, flags3, flags4 | O4_NON_MYTHIC | O4_FLOOR_TILE, flags5, flags6, powconfermask)

/* note: acbon = 10 - ac used in ARMOR, i.e., 1 gives -1 AC bonus to the player */
#define WEAPONSHIELD(name,desc,kn,mgc,prob,wt,cost,enchtype,\
            dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct, \
            hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
            power,power2,power3,pflags,typ,skill,metal,color,soundset,\
            flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets) \
    OBJECT(OBJ(name,desc, None, None, None, 0, 0, 0, 0),                                         \
           BITS(kn, 0, 1, 0, mgc, enchtype, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, typ, ARM_SHIELD, skill, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost,\
           dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, critpct, \
           hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
           wt, color, soundset,\
           0, 0, 0, 0, \
           powconfermask,permittedtargets, O1_IS_WEAPON_WHEN_WIELDED | flags, flags2, flags3, flags4, flags5, flags6)

#define WEAPONBOOTS(name,desc,kn,mgc,delay,prob,wt,cost,enchtype,\
            dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct, \
            hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
            power,power2,power3,pflags,typ,metal,color,soundset,\
            flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets) \
    OBJECT(OBJ(name,desc, None, None, None, 32, 0, 0, 0),                                         \
           BITS(kn, 0, 1, 0, mgc, enchtype, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, typ, ARM_BOOTS, P_MARTIAL_ARTS, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, delay, wt, cost, \
           dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, critpct, \
           hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
           wt, color, soundset, \
           0, 0, 0, 0, \
           powconfermask,permittedtargets, flags, flags2, flags3, flags4 | O4_NON_MYTHIC, flags5,flags6)

#define WEAPONGLOVES(name,desc,kn,mgc,delay,prob,wt,cost,enchtype,\
            dmgtype,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,edmgtype,edice,edam,edmgplus,aflags,aflags2,critpct, \
            hitbon,mcadj,throwrange,acbon,mcbon,manabon,hpbon,bonusattrs,attrbonus,splcastpen,multicount,\
            power,power2,power3,pflags,typ,metal,color,soundset,\
            flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets) \
    OBJECT(OBJ(name,desc, None, None, None, 24, 0, 0, 0),                                         \
           BITS(kn, 0, 1, 0, mgc, enchtype, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, typ, ARM_GLOVES, P_BARE_HANDED_COMBAT, metal),  \
           power, power2, power3, pflags, ARMOR_CLASS, prob, MULTIGEN_SINGLE, delay, wt, cost, \
           dmgtype, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, edmgtype, edice, edam, edmgplus, aflags, aflags2, critpct, \
           hitbon, mcadj, 0, throwrange, acbon, mcbon, manabon, hpbon, bonusattrs, attrbonus, splcastpen, multicount, \
           wt, color, soundset,\
           0, 0, 0, 0, \
           powconfermask,permittedtargets, flags, flags2, flags3, flags4 | O4_NON_MYTHIC | O4_FLOOR_TILE, flags5 | O5_IS_WEAPON_WHEN_WORN, flags6)


/* helmets */
HELM("elven leather helm", "runed leather hat",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR,
    6, 1,  3,  8,  9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
HELM("cotton hood", None,
    1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR,
    3, 1,  3,  8,  10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("gnollish hood", "crude leather hood",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR,
    3, 1,  3,  8,  9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
HELM("orcish helm", "iron skull cap",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    6, 1, 30, 10,  9, 0, 0, 0, 0, 0, 1, MAT_IRON, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
HELM("dwarvish iron helm", "hard hat",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    5, 1, 40, 20,  8, 0, 0, 0, 0, 0, 1, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
HELM("leather hat", None,
    1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR,
    1, 0, 3,  1, 10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BROWN, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("fedora", None,
    1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR,
    0, 0, 3,  1, 10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BROWN, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("tinfoil hat of mind shielding", "thin metal hat",
    0, 1, MIND_SHIELDING, CHARM_RESISTANCE, ODD_IDEAS, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    3, 1, 6, 80, 10, 1, 0, 0, BONUS_TO_INT | BONUS_TO_WIS, -2, 2, MAT_METAL, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("royal crown", "golden crown", //STARTMARKER FOR SHUFFLED CROWNS & CROWN WISH RANGE
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    4, 1, 20, 1000, 10, 0, 0, 0, 0, 0, 1, MAT_GOLD, HI_GOLD, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("crown of rulership", "ornamental crown", //ENDMARKER FOR SHUFFLED CROWNS & CROWN WISH RANGE
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    2, 1, 20, 5000, 10, 0, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE | IGNORE_ENCHANTMENT, 24, 1, MAT_GOLD, HI_GOLD, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("cornuthaum", "blue conical hat", //STARTMARKER FOR CONICAL HATS
    0, 1, CLAIRVOYANT, BLOCKS_CLAIRVOYANCE, NO_POWER, 
    P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS | P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY, 
    ENCHTYPE_ELVEN_ARMOR,
    3, 1, 4, 80, 10, 1, 0, 0, BONUS_TO_CHA | IGNORE_ENCHANTMENT, 1, 0, 
    MAT_CLOTH, CLR_BLUE, 34, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ROLE_WIZARD),
        /* name coined by devteam; confers clairvoyance for wizards,
           blocks clairvoyance if worn by role other than wizard */
HELM("dunce cap", "green conical hat",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    3, 1,  4,  1, 10, 0, 0, 0, BONUS_TO_INT | BONUS_TO_WIS | FIXED_IS_MAXIMUM | SETS_FIXED_ATTRIBUTE, 6, 0, MAT_CLOTH, CLR_GREEN, 34, OBJECT_SOUNDSET_GENERIC,
    O1_BECOMES_CURSED_WHEN_WORN, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("gnomish felt hat", "red conical hat",  //ENDMARKER FOR CONICAL HATS
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    3, 1, 4, 1, 10, 0, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_RED, 34, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_GNOMISH_ITEM, O3_NONE, O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
HELM("silk top hat", "cylindrical hat", 
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    3, 1, 4, 100, 10, 0, 0, 0, BONUS_TO_CHA | IGNORE_ENCHANTMENT, 1, 0, MAT_SILK, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_GNOMISH_ITEM, O3_NONE, O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("dented pot", None,
    1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    2, 0, 30,  8,  9, 0, 0, 0, 0, 0, 1, MAT_IRON, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE | O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
HELM("helmet", "plumed helmet",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    10, 1, 50, 10,  9, 0, 0, 0, 0, 0, 1, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("helm of brilliance", "etched helmet",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    6, 1, 50, 50,  9, 0, 0, 0, BONUS_TO_INT | BONUS_TO_WIS | BONUS_TO_ALL_SPELL_CASTING, 0, 0, MAT_IRON, CLR_GREEN, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("helm of opposite alignment", "crested helmet",
     0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    6, 1, 50, 50,  9, 0, 0, 0, 0, 0, 1, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_BECOMES_CURSED_WHEN_WORN, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
HELM("helm of telepathy", "visored helmet",
    0, 1, TELEPAT, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    2, 1, 50, 50,  9, 0, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in obj.h, objnam.c, mon.c, read.c that assumes (2).
 *      (1) The dragon scale mails and the dragon scales are together.
 *      (2) That the order of the dragon scale mail and dragon scales
 *          is the same as order of dragons defined in monst.c.
 */
#define DRGN_ARMR(name,mgc,power,power2,power3,pflags,cost,ac,mc,manabon,hpbon,bonusattrs,attrbonus,splcastpen,color,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)  \
    ARMOR(name, None, 1, mgc, 1, power, power2, power3, pflags, ENCHTYPE_GENERAL_ARMOR, 0, 5, 550,  \
          cost, ac, mc, manabon, hpbon, bonusattrs, attrbonus, splcastpen, ARM_SUIT, P_NONE, MAT_DRAGON_HIDE, color, 0, soundset, flags, flags2, flags3, O4_NON_MYTHIC | flags4, flags5, flags6, powconfermask)
/* 3.4.1: dragon scale mail reclassified as "magic" since magic is
   needed to create them */
DRGN_ARMR("gray dragon scale mail",    1, ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, 6000, 1, 4, 0, 0, 0, 0, 10, CLR_GRAY, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("silver dragon scale mail",  1, REFLECTING, NO_POWER, NO_POWER, P1_NONE, 6000, 1, 4, 0, 0, 0, 0, 5, DRAGON_SILVER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scale mail", 1, DISPLACED, 1200, 1, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scale mail",     1, FIRE_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, 4000, 1, 4, 0, 0, 0, 0, 5, CLR_RED, OBJECT_SOUNDSET_GENERIC, O1_FIRE_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("white dragon scale mail",   1, COLD_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 5, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_COLD_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("orange dragon scale mail",  1, SLEEP_RESISTANCE,  FREE_ACTION, NO_POWER, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 5, CLR_ORANGE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("black dragon scale mail",   1, DISINTEGRATION_RESISTANCE, DEATH_RESISTANCE, DRAIN_RESISTANCE, P1_NONE, 5000, 1, 4, 0, 0, 0, 0, 5, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_DISINTEGRATION_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("blue dragon scale mail",    1, SHOCK_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, 4000, 1, 4, 0, 0, 0, 0, 5, CLR_BLUE, OBJECT_SOUNDSET_GENERIC, O1_LIGHTNING_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("green dragon scale mail",   1, POISON_RESISTANCE, SICK_RESISTANCE, NO_POWER, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 5, CLR_GREEN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("yellow dragon scale mail",  1, ACID_IMMUNITY, STONE_RESISTANCE, NO_POWER, P1_NONE, 3000, 1, 4, 0, 0, 0, 0, 5, CLR_YELLOW, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALE_MAIL, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* For now, only dragons leave these. */
/* 3.4.1: dragon scales left classified as "non-magic"; they confer
   magical properties but are produced "naturally" */
DRGN_ARMR("gray dragon scales",        0, ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, 1500, 7, 1, 0, 0, 0, 0, 10, CLR_GRAY, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("silver dragon scales",      0, REFLECTING, NO_POWER, NO_POWER, P1_NONE, 1500, 7, 1, 0, 0, 0, 0, 5, DRAGON_SILVER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#if 0 /* DEFERRED */
DRGN_ARMR("shimmering dragon scales",  0, DISPLACED,   700, 7, CLR_CYAN),
#endif
DRGN_ARMR("red dragon scales",         0, FIRE_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, 1000, 7, 1, 0, 0, 0, 0, 5, CLR_RED, OBJECT_SOUNDSET_GENERIC, O1_FIRE_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("white dragon scales",       0, COLD_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 5, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_COLD_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("orange dragon scales",      0, SLEEP_RESISTANCE, FREE_ACTION, NO_POWER, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 5, CLR_ORANGE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("black dragon scales",       0, DISINTEGRATION_RESISTANCE, DEATH_RESISTANCE, DRAIN_RESISTANCE, P1_NONE, 1250, 7, 1, 0, 0, 0, 0, 5, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_DISINTEGRATION_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("blue dragon scales",        0, SHOCK_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, 1000, 7, 1, 0, 0, 0, 0, 5, CLR_BLUE, OBJECT_SOUNDSET_GENERIC, O1_LIGHTNING_RESISTANT, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("green dragon scales",       0, POISON_RESISTANCE, SICK_RESISTANCE, NO_POWER, P1_NONE, 750, 7, 1, 0, 0, 0, 0, 5, CLR_GREEN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
DRGN_ARMR("yellow dragon scales",      0, ACID_IMMUNITY, STONE_RESISTANCE, NO_POWER, P1_NONE, 1000, 7, 1, 0, 0, 0, 0, 5, CLR_YELLOW, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DRAGON_ITEM | O2_MONSTER_SCALES, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#undef DRGN_ARMR
/* other suits */
ARMOR("adamantium full plate mail", None, 1, 
    0, 1, DISINTEGRATION_RESISTANCE, BISECTION_RESISTANCE, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    5, 7, 960, 4000, -1, 5, 0, 0, 0, 0, 15, 
    ARM_SUIT, P_NONE, MAT_ADAMANTIUM, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE | O1_DISINTEGRATION_RESISTANT | O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, 
    O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("mithril full plate mail", None, 1, 
    0, 1, BISECTION_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    5, 7, 550, 4000,  0, 8, 0, 0, 0, 0, 13, 
    ARM_SUIT, P_NONE, MAT_MITHRIL, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC,
    O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, 
    O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("orichalcum full plate mail", None, 1, 
    0, 1, ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR,
    5, 7, 960,5000,  1, 12, 0, 0, 0, 0, 19, 
    ARM_SUIT, P_NONE, MAT_ORICHALCUM, HI_GOLD, 0, OBJECT_SOUNDSET_GENERIC,
    O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, 
    O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("full plate mail", None,
      1, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 7, 960,2000,  1, 5, 0, 0, 0, 0, 14, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("field plate mail", None,
      1, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 20, 6, 840,1000,  2, 4, 0, 0, 0, 0, 12, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("plate mail", None,
      1, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 30, 5, 720, 600,  3, 3, 0, 0, 0, 0, 10, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("crystal plate mail", None,
      1, 0, 1,    REFLECTING, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 5,  640,3000,  3, 3, 0, 0, 0, 0, 2, ARM_SUIT, P_NONE, MAT_GLASS, CLR_WHITE, 0, OBJECT_SOUNDSET_GENERIC, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("bronze plate mail", None,
      1, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 20, 5, 800, 150,  4, 8, 0, 0, 0, 0, 10, ARM_SUIT, P_NONE, MAT_COPPER, HI_COPPER, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("splint mail", None,
      1, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 30, 5, 640, 125,  4, 3, 0, 0, 0, 0, 7, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("banded mail", None,
      1, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 30, 5, 560, 200,  4, 3, 0, 0, 0, 0, 6, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("gnollish bone mail", "crude bone armor",
      0, 0, 1,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 5, 420, 100,  5, 3, 0, 0, 0, 0, 4, ARM_SUIT, P_NONE, MAT_BONE, CLR_WHITE, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("dwarvish mithril-coat", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 250, 750,  4, 6, 0, 0, 0, 0, 3, ARM_SUIT, P_NONE, MAT_MITHRIL, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_DWARVEN_ITEM, O3_NONE, O4_RARE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("elven mithril-coat", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_ELVEN_ARMOR, 15, 1, 150, 250,  5, 4, 0, 0, 0, 0, 2, ARM_SUIT, P_NONE, MAT_MITHRIL, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_ELVEN_ITEM, O3_NONE, O4_RARE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("chain mail", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 30, 5, 480,  75,  5, 3, 0, 0, 0, 0, 5, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("orcish chain mail", "crude chain mail",
      0, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 20, 5, 450,  40,  6, 3, 0, 0, 0, 0, 5, ARM_SUIT, P_NONE, MAT_IRON, CLR_BLACK, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("scale mail", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 30, 5, 500,  45,  6, 3, 0, 0, 0, 0, 4, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("studded leather armor", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_BODY_ARMOR, 30, 3, 200,  15,  7, 3, 0, 0, 0, 0, 2, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, 40, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("gnollish studded leather armor", "crude studded leather armor",
      0, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_BODY_ARMOR, 20, 3, 175,  15,  8, 3, 0, 0, 0, 0, 2, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, 40, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("ring mail", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 35, 5, 500,  30,  7, 2, 0, 0, 0, 0, 3, ARM_SUIT, P_NONE, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("orcish ring mail", "crude ring mail",
      0, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 20, 5, 450,  15,  8, 2, 0, 0, 0, 0, 3, ARM_SUIT, P_NONE, MAT_IRON, CLR_BLACK, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("leather armor", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_BODY_ARMOR, 35, 3, 150,   5,  8, 2, 0, 0, 0, 0, 1, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, 40, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("gnollish leather armor", "crude leather armor",
      0, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_BODY_ARMOR, 20, 3, 125,   3,  9, 2, 0, 0, 0, 0, 1, ARM_SUIT, P_NONE, MAT_LEATHER, HI_LEATHER, 40, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
ARMOR("leather jacket", None,
      1, 0, 0,             0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_BODY_ARMOR, 10, 0,  30,  10,  9, 1, 0, 0, 0, 0, 0, ARM_SUIT, P_NONE, MAT_LEATHER, CLR_BLACK, 40, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_DOUBLE_MYTHIC_CHANCE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("force field armor", "armor-shaped force field",
      0, 1, 0,             0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 0,  0,   0,   4,  4, 6, 0, 0, 0, 0, 0, ARM_SUIT, P_NONE, MAT_FORCEFIELD, HI_GLASS, 0, OBJECT_SOUNDSET_GENERIC, O1_FIRE_RESISTANT | O1_LIGHTNING_RESISTANT | O1_COLD_RESISTANT | O1_CORROSION_RESISTANT | O1_RUST_RESISTANT, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL),

/* shirts */
ARMOR("Hawaiian shirt", None, //STARTMARKER 1
    1, 0, 0, 0,             0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8, 0, 5,  5, 10, 1, 0, 0, 0, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_MAGENTA, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
ARMOR("shirt of uncontrollable laughter", "funny T-shirt", //STARTMARKER 2
    0, 1, 0,  LAUGHING,  0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 0, 5, 50, 10, 1, 0, 0, 0, 0, 3, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_BLUE, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("shirt of comeliness", "black T-shirt",
    0, 1, 0, 0,             0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 0, 5, 50, 10, 1, 0, 0, BONUS_TO_CHA, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("shirt of sound mindedness", "green T-shirt", 
    0, 1, 0, HALLUC_RES, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 0, 5, 50, 10, 1, 0, 0, 0, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_GREEN, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ARMOR("T-shirt", "old T-shirt",//ENDMARKER 1 & 2
    0, 0, 0, 0,             0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 4, 0, 5,  2, 10, 1, 0, 0, 0, 0, 0, ARM_SHIRT, P_NONE, MAT_CLOTH, CLR_WHITE, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),


/* cloaks */
CLOAK("elven cloak", "faded pall",
      0, 1,        STEALTH, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_ELVEN_ARMOR, 8,  0, 10, 60,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("orcish cloak", "coarse mantelet",
      0, 0,                 0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8,  0, 10, 40, 10, 1, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("dwarvish cloak", "hooded cloak",
      0, 0,                 0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8,  0, 10, 50, 10, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("oilskin cloak", "slippery cloak",
      0, 0,                 0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8,  0, 10, 50,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("alchemy smock", "apron",
      0, 1, POISON_RESISTANCE, IMPROVED_ACID_RESISTANCE, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 9, 0, 10, 50,  9, 1, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("leather cloak", None,
      1, 0,                 0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR, 8,  0, 15, 40,  9, 1, 0, 0, 0, 0, 0, MAT_LEATHER, CLR_BROWN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
CLOAK("cloak of protection", "tattered cape",
      0, 1, NO_POWER, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 9, 0, 10, 250,  7, 3, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, OBJECT_SOUNDSET_GENERIC, O1_ENCHANTMENT_AFFECTS_MC, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
        /* cloak of protection is now the only item conferring MC 3 */
CLOAK("cloak of invisibility", "opera cloak",
      0, 1,      INVISIBILITY, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 9, 0, 10, 250,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BRIGHT_MAGENTA, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("cloak of magic resistance", "ornamental cope",
      0, 1,  ANTIMAGIC, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 0, 10, 250,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_GRAY, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
        /*  'cope' is not a spelling mistake... leave it be */
CLOAK("cloak of integrity", "hard woven cloak",
      0, 1,  DISINTEGRATION_RESISTANCE, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 0, 10, 250,  8, 2, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_DISINTEGRATION_RESISTANT | O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CLOAK("cloak of displacement", "piece of cloth",
      0, 1,  DISPLACED, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 7, 0, 10, 250,  9, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_CLOTH, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* robes and gowns */
/* with shuffled appearances... */
ROBE("woven robe", "old robe", //STARTMARKER 1 & 2
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 20,  25,  10, 5, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_RED, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of protection", "golden ornamental robe",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 25, 400,  4, 8, 0, 0, 0, 0, 0, MAT_CLOTH, HI_GOLD, 0, OBJECT_SOUNDSET_GENERIC, O1_ENCHANTMENT_AFFECTS_MC, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of magic resistance", "shining white robe",
    0, 1, ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 9, 1, 25, 400,  9, 5, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("gown of the archbishops", "shining purple robe",
    0, 1, CURSE_RESISTANCE, DEATH_RESISTANCE, DRAIN_RESISTANCE, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 2, 30, 400,  9, 4, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_MAGENTA, 0, OBJECT_SOUNDSET_GENERIC, O1_NOT_CURSEABLE, O2_GENERATED_BLESSED, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of powerlessness", "shining green robe",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 25, 400,  9, 4, 0, 0, BONUS_TO_STR | BONUS_TO_INT | SETS_FIXED_ATTRIBUTE | FIXED_IS_MAXIMUM | IGNORE_ENCHANTMENT, 3, 0, MAT_CLOTH, CLR_GREEN, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of eyes", "star-patterned robe",
    0, 1, INFRAVISION, SEE_INVISIBLE, SEARCHING, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 1, 25, 400,  9, 3, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_CYAN, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of the archmagi", "shining blue robe",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 2, 30, 400,  9, 5, 0, 0, BONUS_TO_INT | SETS_FIXED_ATTRIBUTE, 20, 0, MAT_CLOTH, HI_ZAP, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of splendor", "shining purple robe",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 2, 30, 400,  9, 5, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE, 20, 0, MAT_CLOTH, CLR_MAGENTA, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("robe of starry wisdom", "shining black robe", //ENDMARKER 2
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 2, 30, 400,  9, 5, 0, 0, BONUS_TO_WIS | SETS_FIXED_ATTRIBUTE, 20, 0, MAT_CLOTH, CLR_BLACK, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* without shuffled appearances... */
ROBE("bathrobe", "soft cotton robe",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 1, 25,   5, 10, 4, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_WHITE, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("clerical gown", None,
    1, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 1, 15,  25, 10, 5, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BLACK, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_GENERATED_BLESSED, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("tailored silk robe", "fine robe",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 4, 1, 30, 100, 10, 5, 0, 0, BONUS_TO_CHA | IGNORE_ENCHANTMENT, 1, 0, MAT_SILK, CLR_BROWN, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_GNOMISH_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("gnollish haircloth robe", "crude robe",
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR, 6, 1, 30,  10,  9, 6, 0, 0, 0, 0, 0, MAT_CLOTH, CLR_BROWN, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_GNOLLISH_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
ROBE("mummy wrapping", None, //ENDMARKER 1
    1, 0, BLOCKS_INVISIBILITY, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 0, 0,  3,   2, 10, 6, 0, 0, 0, 0, 2, MAT_CLOTH, CLR_GRAY, 20, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
    /* worn mummy wrapping blocks invisibility */


/* bracers */
/* with shuffled appearances... */
BRACERS("leather bracers", "old bracers", //STARTMARKER FOR BRACER CLASS AND SHUFFLED BRACERS
    0, 0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR, 10, 1, 10,   5, 9, 1, 0, 0, 0, 0, 0, MAT_CLOTH, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BRACERS("bracers of defense", "runed bracers",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 10, 300, 7, 2, 0, 0, 0, 0, 0, MAT_CLOTH, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_ENCHANTMENT_AFFECTS_MC, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BRACERS("bracers of archery", "deerskin bracers",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_CURSED_ITEM_YIELDS_NEGATIVE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 10, 300, 9, 1, 0, 0, BONUS_TO_ARCHERY, 3, 0, MAT_CLOTH, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BRACERS("bracers of spell casting", "shining bracers",
    0, 1, NO_POWER, NO_POWER, NO_POWER, P1_MANA_PERCENTAGE_BONUS, ENCHTYPE_GENERAL_ARMOR, 10, 1, 10, 300, 9, 1, 10, 0, BONUS_TO_ALL_SPELL_CASTING, 8, 0, MAT_CLOTH, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BRACERS("bracers against magic missiles", "ornamental bracers",//ENDMARKER FOR SHUFFLED BRACERS
    0, 1, MAGIC_MISSILE_IMMUNITY, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 1, 10, 300, 9, 1, 0, 0, 0, 0, 0, MAT_CLOTH, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BRACERS("bracers of reflection", "silver bracers", //ENDMARKER FOR BRACER CLASS
    0, 1, REFLECTING, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 9, 1, 25, 400,  9, 1, 0, 0, 0, 0, 0, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),


/* shields */
SHIELD("small shield", None,
       1, 0, 0,          0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 6, 0,  80,  3, 9, 0, 0, 0, 0, 0, 3, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SHIELD("elven shield", "blue and green shield",
       0, 0, 0,          NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_ELVEN_ARMOR, 2, 0,  140,  7, 8, 0, 0, 0, 0, 0, 2, MAT_WOOD, CLR_GREEN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
SHIELD("great orcish shield", "large black shield",
       0, 0, 1,          0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 2, 0,  160,  7, 8, 0, 0, 0, 0, 0, 4, MAT_IRON, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
SHIELD("orcish shield", "black shield",
       0, 0, 0,          0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 2, 0,  100,  7, 9, 0, 0, 0, 0, 0, 4, MAT_IRON, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ORCISH_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
SHIELD("large shield", None,
       1, 0, 1,          0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 5, 0, 160, 10, 8, 0, 0, 0, 0, 0,  5, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SHIELD("dwarvish roundshield", "large round shield",
       0, 0, 0,          0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 4, 0, 150, 10, 8, 0, 0, 0, 0, 0,  6, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_NONE, O5_NO_MYTHIC_RACIAL_PREFIXES, O6_NONE, PERMITTED_ALL),
SHIELD("shield of reflection", "polished silver shield",
       0, 1, 0, REFLECTING, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 3, 0, 70, 750, 8, 0, 0, 0, 0, 0,  5, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_GENERIC, O1_RUST_RESISTANT | O1_CORROSION_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

WEAPONSHIELD("spiked shield", None,
    1, 0, 5, 130, 50, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, PIERCE, P_SPEAR, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONSHIELD("spiked silver shield", None,
    1, 0, 2, 130, 500, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 6, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, P1_NONE, PIERCE, P_SPEAR, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),


/* gloves */
/* These have their color but not material shuffled, so the MAT_IRON must
 * stay CLR_BROWN (== HI_LEATHER) even though it's normally either
 * HI_METAL or CLR_BLACK.  All have shuffled descriptions.
 */
GLOVES("leather gloves", "old gloves",
       0, 0,        0, 0, 0, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR, 8, 1, 10,  8, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GLOVES("gauntlets of fumbling", "padded gloves",
       0, 1, FUMBLING, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8, 1, 10, 100, 9, 0, 0, 0, 0, 0, 6, MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GLOVES("gloves of haste", "brown gloves",
       0, 1, VERY_FAST, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8, 1, 10, 100, 9, 0, 0, 0, 0, 0, 0, MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GLOVES("gloves of spell casting", "deerskin gloves",
       0, 1,        0, 0, 0, P1_MANA_PERCENTAGE_BONUS, ENCHTYPE_GENERAL_ARMOR, 8, 1, 10, 100, 9, 0, 20, 0, BONUS_TO_ALL_SPELL_CASTING, 12, 0, MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
WEAPONGLOVES("gauntlets of ogre power", "riding gloves",
    0, 1, 1, 8, 30, 100, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 1, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE | IGNORE_ENCHANTMENT, STR18(100), 2, 0,
    0, 0, 0, P1_NONE, WHACK, MAT_IRON, CLR_BROWN, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
GLOVES("gauntlets of dexterity", "fencing gloves",
       0, 1,        0, 0, 0, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 8, 1, 10, 100, 9, 0, 0, 0, BONUS_TO_DEX, 0, 0, MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
WEAPONGLOVES("gauntlets of balance", "runed gloves", /* Base item for Gauntlets of Yin and Yang */
    0, 1, 1, 0, 10, 1000, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 1, 0, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE | IGNORE_ENCHANTMENT, STR19(19), 0, 0,
    VERY_FAST, FREE_ACTION, NO_POWER, P1_NONE, WHACK, MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC,
    O1_ROT_RESISTANT | O1_POLYMORPH_RESISTANT | O1_FIRE_RESISTANT,
    O2_NONE,
    O3_NO_WISH | O3_NO_GENERATION,
    O4_NON_MYTHIC, O5_NONE, O6_NONE,
    PERMITTED_ALL, ALL_TARGETS),

/* without shuffled appearances */
WEAPONGLOVES("spiked gauntlets", None,
    1, 0, 1, 5, 30, 30, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 4, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0,
    0, 0, 0, P1_NONE, PIERCE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONGLOVES("silver gauntlets", None,
    1, 0, 1, 5, 30, 300, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 2, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 0, P1_NONE, WHACK, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONGLOVES("spiked silver gauntlets", None,
    1, 0, 1, 2, 30, 500, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 4, 0, 1, 4, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0,
    0, 0, 0, P1_NONE, PIERCE, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),


/* boots */
BOOTS("low boots", "walking shoes",
    0, 0,          
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 14, 2, 10, 8, 9, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
//BOOTS("iron shoes", "hard shoes",
//      0, 0,          0, 0, 0, 7, 2, 60, 16, 8, 0, 0, 0, 0, 0, 2, MAT_IRON, HI_METAL, 0, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
WEAPONBOOTS("iron shoes", "hard shoes",
    0, 0, 2, 7, 60, 16, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 3, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, WHACK, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
BOOTS("leather sandals", None,
    1, 0,          
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_LEATHER_NONBODY_ARMOR, 2, 1, 6,  4, 10, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_BOOTS("cotton slippers", None,
    1, 0,          
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 2, 1, 3,  4, 10, 0, 0, 0, 0, 0, 0,
    MAT_CLOTH, CLR_WHITE, OBJECT_SOUNDSET_COTTON_SLIPPERS, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_HAS_ALTERNATIVE_APPEARANCE, O6_NONE, PERMITTED_ALL, NO_ANIMATION, NO_REPLACEMENT, COTTON_SLIPPERS_REPLACEMENT),
BOOTS("high boots", "jackboots",
    0, 0,          
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 2, 30, 12, 8, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* with shuffled appearances... */
BOOTS("speed boots", "combat boots",
    0, 1,       
    VERY_FAST, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 2, 30, 50, 9, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BOOTS("Galadhrim boots", "runed boots",
    0, 1,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_ELVEN_ARMOR, 10, 2, 20, 50, 9, 0, 0, 0, BONUS_TO_DEX | SETS_FIXED_ATTRIBUTE, 20, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BOOTS("water walking boots", "jungle boots",
    0, 1,   
    WATER_WALKING, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 2, 15, 50, 9, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BOOTS("jumping boots", "hiking boots",
    0, 1,    
    JUMPING, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 10, 2, 30, 50, 9, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BOOTS("elven boots", "mud boots",
    0, 1,
    STEALTH, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_ELVEN_ARMOR, 10, 2, 20,  8, 9, 0, 0, 0, 0, 0, 0, 
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_ELVEN_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
WEAPONBOOTS("kicking boots", "buckled boots",
    0, 1, 2, 12, 60, 50, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 7, 0, 1, 7, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,
    MAGICAL_KICKING, NO_POWER, NO_POWER, P1_NONE, WHACK, MAT_IRON, CLR_BROWN, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
/* CLR_BROWN for same reason as gauntlets of power */

BOOTS("fumble boots", "riding boots",
    0, 1,   
    FUMBLING, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 12, 2, 30, 30, 9, 0, 0, 0, 0, 0, 3,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
BOOTS("levitation boots", "snow boots",
    0, 1, 
    LEVITATION, NO_POWER, NO_POWER, P1_NONE, ENCHTYPE_GENERAL_ARMOR, 12, 2, 15, 30, 9, 0, 0, 0, 0, 0, 0,
    MAT_LEATHER, HI_LEATHER, OBJECT_SOUNDSET_GENERIC, 
    O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* more without shuffled appearances */
WEAPONBOOTS("spiked boots", None,
    1, 0, 2, 5, 60, 20, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, PIERCE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONBOOTS("silver shoes", None,
    1, 0, 2, 4, 60, 200, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 3, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, WHACK, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEAPONBOOTS("spiked silver boots", None,
    1, 0, 2, 2, 60, 400, ENCHTYPE_GENERAL_ARMOR,
    AD_PHYS, 1, 5, 0, 1, 5, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, PIERCE, MAT_SILVER, HI_METAL, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),


#undef HELM
#undef CLOAK
#undef SHIELD
#undef SHIRT
#undef ROBE
#undef BRACERS
#undef GLOVES
#undef BOOTS
#undef ARMOR
#undef GENERAL_ARMOR
#undef WEAPONSHIELD
#undef WEAPONGLOVES
#undef WEAPONBOOTS

/* rings ... */
#define CHARGEDRING(name,desc,prob,power,power2,power3,pflags,cost,mgc,ench,chargetype,recharging,mohs,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,color,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, None, None, None, 16, 0, 0, 0),                                         \
           BITS(0, 0, ench == ENCHTYPE_NO_ENCHANTMENT && chargetype == CHARGED_NOT_CHARGED  ? 0 : 1 , 0, mgc, ench, chargetype, recharging, 0, 0, 0,                    \
                HARDGEM(mohs), 0, 0, P_NONE, metal),                     \
           power, power2, power3, pflags, RING_CLASS, prob, MULTIGEN_SINGLE, 0, 1, cost,  \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattrs,attrbonus, splcastpen, 0, \
           15, color, soundset, \
           0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4 | O4_FLOOR_TILE, flags5, flags6)

#define RING(name,desc,prob,power,power2,power3,pflags,cost,mgc,ench,mohs,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,color,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
     CHARGEDRING(name,desc,prob,power,power2,power3,pflags,cost,mgc,ench,CHARGED_NOT_CHARGED,RECHARGING_NOT_RECHARGEABLE,mohs,manabon,hpbon,bonusattrs,attrbonus,splcastpen,metal,color,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)

RING("adornment", "wooden", /* STARTMARKER FOR SHUFFLED RINGS */
    30, ADORNED, 0, 0, P1_NONE, 100, 1, ENCHTYPE_RING_NORMAL, 2, 0, 0, BONUS_TO_CHA, 0, 0, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("gain strength", "granite",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_NORMAL, 7, 0, 0, BONUS_TO_STR, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("gain dexterity", "thin",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_NORMAL, 7, 0, 0, BONUS_TO_DEX, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("gain constitution", "opal",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_NORMAL, 7, 0, 0, BONUS_TO_CON, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("gain intelligence", "adamantium",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_NORMAL, 7, 0, 0, BONUS_TO_INT, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("gain wisdom", "polished",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_NORMAL, 7, 0, 0, BONUS_TO_WIS, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("power", "runed",
    10, 0, 0, 0, P1_NONE, 300, 1, ENCHTYPE_RING_POWER, 7, 0, 0, BONUS_TO_ALLSTATS, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("increase accuracy", "clay",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_1_7, 4, 0, 0, BONUS_TO_HIT, 0, 0, MAT_MINERAL, CLR_RED, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("increase damage", "coral",
    30, 0, 0, 0, P1_NONE, 150, 1, ENCHTYPE_RING_1_7, 4, 0, 0, BONUS_TO_DAMAGE, 0, 0, MAT_MINERAL, CLR_ORANGE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("protection", "black onyx",
    30, NO_POWER, 0, 0, P1_NONE, 100, 1, ENCHTYPE_RING_NORMAL, 7, 0, 0, BONUS_TO_AC | BONUS_TO_MC, 0, 0, MAT_MINERAL, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("regeneration", "moonstone",
    30, REGENERATION, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 0, 0, 0, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("replenishment", "crystal",
    30, ENERGY_REGENERATION, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 0, 0, 0, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("the serpent god", "serpent-headed",
    15, 0, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 0, 0, BONUS_TO_ALL_SPELL_CASTING, 20, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("wizardry", "black pearl",
    25, 0, 0, 0, P1_MANA_PERCENTAGE_BONUS, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 50, 0, 0, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("fortitude", "immaculate",
    30, 0, 0, 0, P1_HP_PERCENTAGE_BONUS, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  6,  0, 25, 0, 0, 0, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("searching", "tiger eye",
    30, SEARCHING, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_BROWN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("stealth", "jade",
    30, STEALTH, 0, 0, P1_NONE, 100, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_GREEN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("sustain ability", "bronze",
    30, FIXED_ABIL, 0, 0, P1_NONE, 100, 1, ENCHTYPE_NO_ENCHANTMENT,  4, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("levitation", "agate",
    30, LEVITATION, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  7, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_RED, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("hunger", "topaz",
    30, HUNGER, 0, 0, P1_NONE, 100, 1, ENCHTYPE_NO_ENCHANTMENT,  8, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_CYAN, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("aggravate monster", "sapphire",
    30, AGGRAVATE_MONSTER, 0, 0, P1_NONE, 150, 1, ENCHTYPE_NO_ENCHANTMENT,  9, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_BLUE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#if 0 /* Changed to be an artifact */
RING("conflict", "ruby",
    10, CONFLICT, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  9, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_RED, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#endif
RING("warning", "diamond",
    25, WARNING, 0, 0, P1_NONE, 100, 1, ENCHTYPE_NO_ENCHANTMENT, 10, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("poison resistance", "pearl",
    30, POISON_RESISTANCE, 0, 0, P1_NONE, 150, 1, ENCHTYPE_NO_ENCHANTMENT,  4, 0, 0, 0, 0, 0, MAT_BONE, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("fire resistance", "iron",
    30, FIRE_IMMUNITY, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  5, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("cold resistance", "brass",
    30, COLD_IMMUNITY, 0, 0, P1_NONE, 150, 1, ENCHTYPE_NO_ENCHANTMENT,  4, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, OBJECT_SOUNDSET_GENERIC, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("shock resistance", "copper",
    30, SHOCK_IMMUNITY, 0, 0, P1_NONE, 150, 1, ENCHTYPE_NO_ENCHANTMENT,  3, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, OBJECT_SOUNDSET_GENERIC, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("protection from undeath", "platinum",
    25, DEATH_RESISTANCE, DRAIN_RESISTANCE, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  3, 0, 0, 0, 0, 0, MAT_COPPER, HI_COPPER, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("free action", "twisted",
    25, FREE_ACTION, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  6, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("slow digestion", "steel",
    25, SLOW_DIGESTION, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  8, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("teleportation", "silver",
    25, TELEPORT, 0, 0, P1_NONE, 200, 1, ENCHTYPE_NO_ENCHANTMENT,  3, 0, 0, 0, 0, 0, MAT_SILVER, HI_SILVER,OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("teleport control", "gold",
    20, TELEPORT_CONTROL, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  3, 0, 0, 0, 0, 0, MAT_GOLD, HI_GOLD, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("polymorph", "ivory",
    30, POLYMORPH, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  4, 0, 0, 0, 0, 0, MAT_BONE, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, O1_POLYMORPH_RESISTANT, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("polymorph control", "emerald",
    20, POLYMORPH_CONTROL, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  8, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_BRIGHT_GREEN, OBJECT_SOUNDSET_GENERIC, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("invisibility", "wire",
    25, INVISIBILITY, 0, 0, P1_NONE, 150, 1, ENCHTYPE_NO_ENCHANTMENT,  5, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("see invisible", "engagement",
    25, SEE_INVISIBLE, 0, 0, P1_NONE, 150, 1, ENCHTYPE_NO_ENCHANTMENT,  5, 0, 0, 0, 0, 0, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("X-ray vision", "black opal",
    20, XRAY_VISION, 0, 0, P1_NONE, 300, 1, ENCHTYPE_NO_ENCHANTMENT,  5, 0, 0, 0, 0, 0, MAT_MINERAL, CLR_BLACK, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
RING("protection from shape changers", "shiny", /* ENDMARKER FOR SHUFFLED RINGS */
    25, PROT_FROM_SHAPE_CHANGERS, 0, 0, P1_NONE, 100, 1, ENCHTYPE_NO_ENCHANTMENT,  5, 0, 0, 0, 0, 0, MAT_IRON, CLR_BRIGHT_CYAN, OBJECT_SOUNDSET_GENERIC, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* without shuffled apprearances */
RING("supreme power", "golden runed", /* Base item for the Ruling Ring of Yendor */
    0, INVISIBILITY, CLAIRVOYANT, SEE_INVISIBLE, P1_NONE, 5000, 1, ENCHTYPE_NO_ENCHANTMENT,  5, 0, 0, BONUS_TO_ALLSTATS | BONUS_TO_AC | BONUS_TO_MC, 3, 0, MAT_GOLD, HI_GOLD, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE | O1_DISINTEGRATION_RESISTANT | O1_FIRE_RESISTANT | O1_COLD_RESISTANT | O1_LIGHTNING_RESISTANT,
    O2_NONE, 
    O3_NO_WISH | O3_NO_GENERATION | O3_READABLE,
    O4_NONE, O5_NONE, O6_NONE, 
    PERMITTED_ALL),
CHARGEDRING("three charges", "ornate golden", /* Base item for the Ring of Three Wishes */
    0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 1000, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_ALWAYS_3, RECHARGING_RING_OF_THREE_WISHES, 9, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_RED, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE,
    O2_NONE, 
    O3_NO_WISH | O3_NO_GENERATION,
    O4_NONE, O5_NONE, O6_NONE, 
    PERMITTED_ALL),
CHARGEDRING("seven charges", "ornate silver", /* Base item for the Ring of Conflict (artifact) */
    0, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 1000, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_ALWAYS_7, RECHARGING_RING_OF_CONFLICT, 9, 0, 0, 0, 0, 0, MAT_GEMSTONE, CLR_RED, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE,
    O2_NONE, 
    O3_NO_WISH | O3_NO_GENERATION, 
    O4_NONE, O5_NONE, O6_NONE, 
    PERMITTED_ALL),
#undef RING
#undef CHARGEDRING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,prob,power,power2,power3,pflags,manabonus,hpbonus,bonusattrs,attrbonus,splcastpen,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, None, None, None, 24, 0, 0, 0),                                          \
           BITS(0, 0, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, MAT_IRON),        \
           power, power2, power3, pflags, AMULET_CLASS, prob, MULTIGEN_SINGLE, 0, 5, 150, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, manabonus, hpbonus, bonusattrs, attrbonus, splcastpen, 0, \
           20, HI_METAL, soundset, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
AMULET("amulet of ESP",                "circular", 70,    TELEPAT, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of life saving",       "spherical", 70,  LIFESAVED, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of strangulation",          "oval", 100, STRANGLED, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of restful sleep",    "triangular", 100, SLEEPY, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet versus poison",        "pyramidal", 70,    POISON_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet versus undeath",         "cubical", 70,    DEATH_RESISTANCE, DRAIN_RESISTANCE, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet versus petrification","icosahedral",70,  STONE_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of change",               "square", 100, NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of unchanging",          "concave", 45,  UNCHANGING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of reflection",        "hexagonal", 70,  REFLECTING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of mana",                "convex",  70,    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 75, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("demon blood talisman",       "tetrahedral",  25,    NO_POWER, NO_POWER, NO_POWER, P1_MANA_PERCENTAGE_BONUS, 100, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), //doubles mana capacity
AMULET("periapt of vitality",            "linear",  70,    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 25, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of magical breathing", "octagonal", 70,  MAGICAL_BREATHING, 0, 0, P1_NONE, 0, 0, 0, 0, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
AMULET("amulet of seeing",            "cylindrical",  0,  NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 24, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NO_GENERATION | O3_NO_WISH, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Base item for Eye of the Aethiopica */
/* fixed descriptions; description duplication is deliberate;
 * fake one must come before real one because selection for
 * description shuffling stops when a non-magic amulet is encountered
 */
OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
           "Amulet of Yendor", None, None, None, 24, 0, 0, 0),
       BITS(0, 0, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, 0, MAT_PLASTIC),
       0, 0, 0, P1_NONE, AMULET_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       1, HI_METAL, OBJECT_SOUNDSET_GENERIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_UNBURIABLE, O4_NONE, O5_NONE, O6_NONE),
OBJECT(OBJ("Amulet of Yendor", /* note: description == name */
           "Amulet of Yendor", None, None, None, 24, 0, 0, 0),
       BITS(0, 0, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 1, 1, 0, 0, 0, 0, 0, MAT_MITHRIL),
       0, 0, 0, P1_NONE, AMULET_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 30000,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       20, HI_METAL, OBJECT_SOUNDSET_GENERIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE, O2_NONE, O3_INVOKABLE | O3_UNBURIABLE | O3_NO_GENERATION | O3_NO_WISH, O4_NONE, O5_NONE, O6_NONE),
#undef AMULET

/* miscellaneous (magic) items */
#define MISCELLANEOUSITEM(name,desc,sub,itemdesc,kn,magic,mergeable,spetype,charged,recharging,prob,cost,wt,power,power2,power3,pflags,manabonus,hpbonus,bonusattrs,attrbonus,splcastpen,nut,material,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, None, None, itemdesc, height, 0, 0, 0),                                          \
           BITS(kn, mergeable, (charged) != 0 || (spetype) != 0 ? 1 : 0, 0, magic, spetype, charged, recharging, 0, 0, 0, 0, 0, sub, 0, material),        \
           power, power2, power3, pflags, MISCELLANEOUS_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost,  \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, manabonus, hpbonus, bonusattrs, attrbonus, splcastpen, 0, \
           nut, color, soundset, \
           0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
MISCELLANEOUSITEM("brooch of shielding", "golden brooch", MISC_BROOCH, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 150, 5,
    IMPROVED_MAGIC_MISSILE_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_MC, 5, 0,
    20, MAT_GOLD, HI_GOLD, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("nose ring of bull strength", "golden nose ring", MISC_NOSERING, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 200, 2, //STARTMARKER FOR NOSE RING WISH-CLASS, SHUFFLE
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR18(76), 0,
    50, MAT_GOLD, HI_GOLD, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("nose ring of bullheadedness", "silver nose ring", MISC_NOSERING,  None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 200, 2,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_WIS | SETS_FIXED_ATTRIBUTE | FIXED_IS_MAXIMUM, 3, 0,
    50, MAT_SILVER, HI_SILVER, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("nose ring of cerebral safeguarding", "bronze nose ring", MISC_NOSERING, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 200, 2, //ENDMARKER FOR NOSE RING WISH-CLASS, SHUFFLE
    BRAIN_PROTECTION, 0, 0, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_COPPER, HI_COPPER, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("headband of intellect", "shining blue headband", MISC_HEADBAND, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 30, 200, 3,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_INT | SETS_FIXED_ATTRIBUTE, 19, 0,
    50, MAT_CLOTH, CLR_BLUE, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("golden earrings", None, MISC_EARRINGS, None,
    1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10, 200, 2,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CHA, 1, 0,
    50, MAT_GOLD, HI_GOLD, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_GENDER_FEMALE),
MISCELLANEOUSITEM("ioun stone of protection", "red ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10, //STARTMARKER FOR IOUNSTONE WISH-CLASS
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_AC | BONUS_TO_MC | FULL_MC_BONUS, 3, 0,
    50, MAT_MINERAL, CLR_RED, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of spell mastery", "blue ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 50, 0, BONUS_TO_UNRESTRICTED_SPELL_CASTING, 20, 0,
    50, MAT_MINERAL, CLR_BLUE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of magic resistance", "brown ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    ANTIMAGIC, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MINERAL, CLR_BROWN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of strength", "pink ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR, 0, 0,
    50, MAT_MINERAL, CLR_BRIGHT_MAGENTA, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of dexterity", "green ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_DEX, 0, 0,
    50, MAT_MINERAL, CLR_GREEN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of constitution", "purple ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CON, 0, 0,
    50, MAT_MINERAL, CLR_MAGENTA, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of intelligence", "yellow ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 50,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_INT, 0, 0,
    50, MAT_MINERAL, CLR_YELLOW, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of wisdom", "white ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_WIS, 0, 0,
    50, MAT_MINERAL, CLR_WHITE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of charisma", "black ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CHA, 0, 0,
    50, MAT_MINERAL, CLR_BLACK, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of experience", "bright blue ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_EXPERIENCE, 3, 0,
    50, MAT_MINERAL, CLR_BRIGHT_BLUE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of awareness", "gray ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    WARNING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MINERAL, CLR_GRAY, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of regeneration", "orange ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    REGENERATION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MINERAL, CLR_ORANGE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of incessant hunger", "bright green ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    HUNGER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MINERAL, CLR_BRIGHT_GREEN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of restful sleep", "transparent ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10,
    SLEEPY, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MINERAL, CLR_BRIGHT_CYAN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("ioun stone of sustenance", "cyan ioun stone", MISC_IOUN_STONE, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 200, 10, //ENDMARKER FOR IOUNSTONE WISH-CLASS
    SLOW_DIGESTION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MINERAL, CLR_CYAN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("wings of flying", "artificial wings", MISC_WINGS, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10, 200, 50,
    FLYING, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    50, MAT_MODRONITE, CLR_WHITE, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("lenses", "gold-framed eyeglasses", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 45, 80, 3, //STARTMARKER FOR EYEGLASSES WISH-CLASS AND SHUFFLED EYEGLASSES
    ENHANCED_VISION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_GLASS, HI_GLASS, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("eyeglasses of hallucination", "oval eyeglasses", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 45, 150, 3,
    HALLUC, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_GLASS, HI_GLASS, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("eyeglasses of awkwardness", "square eyeglasses", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 45, 150, 3,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CHA | SETS_FIXED_ATTRIBUTE | FIXED_IS_MAXIMUM, 3, 0,
    20, MAT_GLASS, HI_GLASS, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("eyeglasses of see invisible", "round eyeglasses", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 45, 150, 3,  //ENDMARKER FOR SHUFFLED EYEGLASSES
    SEE_INVISIBLE, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_GLASS, HI_GLASS, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("sunglasses", "shaded eyeglasses", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 25, 80, 3,   //ENDMARKER FOR EYEGLASSES WISH-CLASS
    FLASH_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_GLASS, CLR_BLACK, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("goggles of night", "leather-framed goggles", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 40, 100, 3,  //STARTMARKER FOR GOGGLES WISH-CLASS, SHUFFLED GOGGLES
    INFRAVISION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_PLASTIC, HI_GLASS, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("goggles of eye protection", "transparent goggles", MISC_EYEGLASSES, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 30, 100, 3, //ENDMARKER FOR GOGGLES WISH-CLASS, ENDMARKER FOR SHUFFLED GOGGLES
    EYE_PROTECTION, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_PLASTIC, HI_GLASS, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("expensive watch", None, MISC_WRIST_WATCH, None,
    1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5, 200, 6,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CHA, 1, 0,
    40, MAT_GOLD, HI_GOLD, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_APPLIABLE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_GENDER_MALE),
MISCELLANEOUSITEM("gnollish leather mask", "crude leather mask", MISC_MASK, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10, 2, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    10, MAT_CLOTH, HI_LEATHER, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("beak mask of sickness resistance", "beak-shaped mask", MISC_MASK, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5, 10, 5,
    SICK_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_CLOTH, HI_LEATHER, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("leather belt", "old belt", MISC_BELT, None,
    0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 5, 5, //STARTMARKER FOR BELTS
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of change", "brown belt", MISC_BELT, "Changes the gender of the wearer.",
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 250, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, 0, 0, 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CURSED_MAGIC_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of dwarvenkind", "sturdy belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_MISCELLANEOUS_NORMAL, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 250, 5,
    POISON_RESISTANCE, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_CON, 0, 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of fortitude", "runed belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 250, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_HP_PERCENTAGE_BONUS, 0, 50, 0, 0, 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of hill giant strength", "ornamental belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10, 250, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(20), 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of stone giant strength", "heavy belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 8, 250, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(21), 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_FLOOR_TILE | O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of frost giant strength", "decorative belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 6, 250, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(22), 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_FLOOR_TILE | O4_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of fire giant strength", "coarse belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 4, 250, 5,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(23), 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_FLOOR_TILE | O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),
MISCELLANEOUSITEM("belt of storm giant strength", "rudimentary belt", MISC_BELT, None,
    0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 2, 250, 5, //ENDMARKER FOR BELTS
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, 0, BONUS_TO_STR | SETS_FIXED_ATTRIBUTE, STR19(24), 0,
    20, MAT_CLOTH, HI_LEATHER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS, O4_FLOOR_TILE | O4_VERY_RARE, O5_NONE, O6_NONE, PERMITTED_ALL),


#undef MISCELLANEOUSITEM

/* tools ... */
/* tools with weapon characteristics come last */
#define GENERAL_TOOL(name,desc,contentname,contentdesc,itemdesc, stand_anim, enl, repl, subtype,kn,mrg,mgc,spetype,charged,recharging,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,attrbonus,splcastpen,power1,power2,power3,pflags,special_quality,mat,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, contentname, contentdesc, itemdesc, height, stand_anim, enl, repl),                                           \
           BITS(kn, mrg, (charged) != 0 || (spetype) != 0 ? 1 : 0, 0, mgc, spetype, charged, recharging, 0, 0, 0, 0, 0, subtype, P_NONE, mat), \
           power1, power2, power3, pflags,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattr, attrbonus, splcastpen, 0, \
           wt, color, soundset, \
           0, 0, cooldown, special_quality, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
#define TOOL(name,desc,contentname,contentdesc,itemdesc,subtype,kn,mrg,mgc,spetype,charged,recharging,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,attrbonus,splcastpen,power1,power2,power3,pflags,special_quality,mat,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
   GENERAL_TOOL(name,desc,contentname,contentdesc,itemdesc, 0, 0, 0, subtype,kn,mrg,mgc,spetype,charged,recharging,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,attrbonus,splcastpen,power1,power2,power3,pflags,special_quality,mat,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)

#define GENERAL_SPELLTOOL(name,desc,contentname,contentdesc,itemdesc, stand_anim, enl, repl, subtype,kn,mrg,mgc,charged,recharging,prob,wt,cost,dir,dirsubtype, adtyp, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus,cooldown,special_quality, skill, mat,color,height,soundset,flags,flags2,flags3, flags4,flags5,flags6) \
    OBJECT(OBJ(name, desc, contentname, contentdesc, itemdesc, height, stand_anim, enl, repl),                                           \
           BITS(kn, mrg, charged ? 1 : 0, 0, mgc, ENCHTYPE_NO_ENCHANTMENT, charged, recharging, 0, 0, 0, 0, dir, subtype, skill, mat), \
           0, 0, 0, P1_NONE,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
           adtyp, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           wt, color, soundset, dirsubtype, 0, cooldown, special_quality, PERMITTED_ALL, ALL_TARGETS, O1_WAND_LIKE_TOOL | flags, flags2, flags3, flags4, flags5, flags6)
#define SPELLTOOL(name,desc,contentname,contentdesc,itemdesc,subtype,kn,mrg,mgc,charged,recharging,prob,wt,cost,dir,dirsubtype, adtyp, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus,cooldown,special_quality, skill, mat,color,height,soundset,flags,flags2,flags3, flags4, flags5, flags6) \
    GENERAL_SPELLTOOL(name,desc,contentname,contentdesc,itemdesc, 0, 0, 0, subtype,kn,mrg,mgc,charged,recharging,prob,wt,cost,dir,dirsubtype, adtyp, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus,cooldown,special_quality, skill, mat,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6)
/*
OBJECT(OBJ(name, desc, contentname, contentdesc, itemdesc, 0, 0, 0),                                           \
           BITS(kn, mrg, charged ? 1 : 0, 0, mgc, ENCHTYPE_NO_ENCHANTMENT, charged, recharging, 0, 0, 0, 0, dir, subtype, P_NONE, mat), \
           0, 0, 0, P1_NONE,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
           AD_PHYS, sdice, sdam, sdmgplus, ldice, ldam, ldmgplus, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           wt, color, height,soundset, dirsubtype, 0, cooldown, 0, PERMITTED_ALL, ALL_TARGETS, O1_WAND_LIKE_TOOL | flags, flags2, flags3, flags4)
*/
#define CONTAINER(name,desc,itemdesc,subtype,stand_anim, enl, repl,kn,mgc,charged,recharging,prob,wt,cost,cooldown,manabon,hpbon,bonusattr,attrbonus,pflags,special_quality,mat,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, None, None, itemdesc, height, stand_anim, enl, repl),                                           \
           BITS(kn, 0, charged ? 1 : 0, 1, mgc, ENCHTYPE_NO_ENCHANTMENT, charged, recharging, 0, 0, 0, 0, 0, subtype, P_NONE, mat),   \
           0, 0, 0, pflags,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, manabon, hpbon, bonusattr, attrbonus, 0, 0, \
           wt, color, soundset, 0, 0, cooldown, special_quality, powconfermask, ALL_TARGETS, flags, O2_CONTAINER | flags2, flags3, flags4, flags5, flags6)
#define WEPTOOL(name,desc,itemdesc,kn,mgc,bi,prob,wt,cost, dmgtype, sdice, sdiesize, sdmgplus, ldice, ldiesize, ldmgplus, edmgtype, edice, ediesize, edmgplus, aflags, aflags2, critpct, hitbon,splcastpen,sub,skill,cooldown,special_quality,mat,clr,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,permittedtargets)\
    OBJECT(OBJ(name, desc, None, None, itemdesc, height, 0, 0, 0),                                           \
           BITS(kn, 0, 1, 0, mgc, ENCHTYPE_GENERAL_WEAPON, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, bi, 0, hitbon, sub, skill, mat),    \
           0, 0, 0, P1_NONE,  TOOL_CLASS, prob, MULTIGEN_SINGLE, 0, wt, cost, \
           dmgtype, sdice, sdiesize, sdmgplus, ldice, ldiesize, ldmgplus, edmgtype, edice, ediesize, edmgplus, aflags, aflags2, critpct, \
           hitbon, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           wt, clr, soundset, 0, 0, cooldown, special_quality, powconfermask, permittedtargets, flags, flags2, flags3, O4_WEAPON_TOOL | flags4, flags5, flags6)

/* containers */
CONTAINER("large box",       None, None, TOOLTYPE_BOX, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 37, 350,   8, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_CHEST, O1_NONE, O2_CONTAINER_BOX, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), //STARTMARKER 1
CONTAINER("chest",           None, None, TOOLTYPE_CHEST, NO_ANIMATION, NO_ENLARGEMENT, CHEST_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 28, 600,  16, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_CHEST, O1_ROT_RESISTANT, O2_CONTAINER_BOX, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("golden chest",           None, None, TOOLTYPE_CHEST, NO_ANIMATION, NO_ENLARGEMENT, GOLDEN_CHEST_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 600, 500, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_GOLD, HI_GOLD, 0, OBJECT_SOUNDSET_CHEST,
    O1_DISINTEGRATION_RESISTANT | O1_NOT_CURSEABLE,
    O2_CONTAINER_BOX, O3_NO_WISH | O3_NO_GENERATION, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("ice box",         None, None, TOOLTYPE_BOX, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5, 900,  42, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_PLASTIC, CLR_WHITE, 0, OBJECT_SOUNDSET_CHEST, O1_ROT_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("bookshelf",         None, None, TOOLTYPE_GENERAL, NO_ANIMATION, BOOKSHELF_ENLARGEMENT, NO_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 1600, 36, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FULL_SIZED_BITMAP | O4_CONTAINER_CONTENTS_VISIBLE | O4_CONTAINER_ACCEPTS_ONLY_SCROLLS_AND_BOOKS | O4_CONTAINER_PEEK_INTO | O4_NO_CLASS_GENERATION_IN_SHOP, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("weapon rack",         None, None, TOOLTYPE_GENERAL, NO_ANIMATION, WEAPON_RACK_ENLARGEMENT, NO_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 26,  600,  8, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FULL_SIZED_BITMAP | O4_CONTAINER_CONTENTS_VISIBLE | O4_CONTAINER_ACCEPTS_ONLY_WEAPONS | O4_NO_CLASS_GENERATION_IN_SHOP, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("mine cart",         None, None, TOOLTYPE_GENERAL, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 2,  3200,  36, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_IRON, CLR_GRAY, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_CONTAINER_PEEK_INTO | O4_NO_CLASS_GENERATION_IN_SHOP, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("sarcophagus",     None, None, TOOLTYPE_GENERAL, NO_ANIMATION, NO_ENLARGEMENT, SARCOPHAGUS_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 2,  2400,  36, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_MINERAL, CLR_YELLOW, 0, OBJECT_SOUNDSET_SARCOPHAGUS, O1_ROT_RESISTANT, O2_NONE, O3_NONE, O4_FULL_SIZED_BITMAP | O4_CONTAINER_HAS_LID | O4_CONTAINER_PEEK_INTO | O4_CONTAINER_MAY_CONTAIN_MONSTER | O4_NO_CLASS_GENERATION_IN_SHOP, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("coffin",     None, None, TOOLTYPE_GENERAL, NO_ANIMATION, NO_ENLARGEMENT, COFFIN_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5,  1600,  36, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_WOOD, CLR_BROWN, 0, OBJECT_SOUNDSET_COFFIN, O1_ROT_RESISTANT, O2_NONE, O3_NONE, O4_FULL_SIZED_BITMAP | O4_CONTAINER_HAS_LID | O4_CONTAINER_PEEK_INTO | O4_CONTAINER_MAY_CONTAIN_MONSTER | O4_NO_CLASS_GENERATION_IN_SHOP, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("backpack",         None, None, TOOLTYPE_GENERAL, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    1, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5,  15,   5, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* shuffled bags start here */
CONTAINER("leather bag",     "brown bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,//STARTMARKER FOR SHUFFLED BAGS AND BAG WISHCLASS
    0, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5,  15,  25, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_LEATHER, HI_LEATHER, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("sack",           "vintage bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10,  10,   5, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("oilskin sack",   "decorative bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5,  10, 100, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* magic bags start here */
CONTAINER("bag of holding", "ornamental bag", "Reduces the weight of contents by half", TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 20, 15, 100, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), //STARTMARKER 2
CONTAINER("bag of wizardry", "antiquated bag", "Reduces the weight of spellbooks, scrolls, reagents, and wands to 1/8", TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 15, 100, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, O4_NONE, O5_CANCELLATION_NO_EXPLOSION_BUT_DRAIN, O6_NONE, PERMITTED_ALL),
CONTAINER("bag of treasure hauling", "silvery bag", "Reduces the weight of coins, gems, and other treasure to 1/32", TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 15, 100, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("bag of the glutton", "dilapidated bag", "Reduces the weight of comestibles and potions to 1/10", TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 15, 100, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_MAGIC_BAG | O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("pouch of endless bolts", "old bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 4, 15, 100, 1000, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_NONCONTAINER, O3_ELEMENTAL_ENCHANTABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("bag of infinite sling bullets", "old-fashioned bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 4, 15, 100, 1000, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_NONCONTAINER, O3_ELEMENTAL_ENCHANTABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("bag of tricks", "runed bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,//ENDMARKER FOR SHUFFLED
    0, 1, CHARGED_BAG_OF_TRICKS, RECHARGING_TOOL_SPECIAL_MAGICAL, 20, 15, 100, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_NONCONTAINER | O2_CONTAINER_MAGIC_BAG, O3_NONE, O4_NONE, O5_MBAG_DESTROYING_ITEM, O6_NONE, PERMITTED_ALL), //ENDMARKER 1&2
/* end of shuffled bags */
CONTAINER("expensive handbag", "gold-encrusted bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 2, 5, 500, 0, 0, 0, BONUS_TO_CHA, 2, P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED, 0, MAT_SILK, HI_GOLD, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_GENDER_FEMALE),
CONTAINER("oriental silk sack", "silk-woven bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,//ENDMARKER BAG WISHCLASS
    0, 0, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 5,  3,  50, 0, 0, 0, 0, 0, P1_NONE, 0, MAT_SILK, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
CONTAINER("quiver of infinite arrows", "cylindrical bag", None, TOOLTYPE_BAG, NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT,
    0, 1, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 4, 15, 100, 1000, 0, 0, 0, 0, P1_NONE, 0, MAT_CLOTH, HI_CLOTH, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_CONTAINER_NONCONTAINER, O3_ELEMENTAL_ENCHANTABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#undef CONTAINER

/* lock opening tools */
TOOL("skeleton key",       "key", "metal", None, None, TOOLTYPE_GENERAL, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 70,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_KEY, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("magic key", None, "magic", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_METAL, HI_ZAP, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_KEY, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("geometric key",       "key", "geometric", None, None, TOOLTYPE_GENERAL, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_MODRONITE, CLR_WHITE, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("ornamental key",       "key", "ornamental", None, None, TOOLTYPE_GENERAL, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_METAL, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("stone key", None, "stone", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("bronze key", None, "bronze", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_METAL, HI_COPPER, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("silver key", None, "silver", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_SILVER, HI_SILVER, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("gold key", None, "gold", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_GOLD, HI_GOLD, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("platinum key", None, "platinum", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_PLATINUM, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("mithril key", None, "mithril", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_MITHRIL, HI_SILVER, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("orichalcum key", None, "orichalcum", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_ORICHALCUM, HI_GOLD, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("adamantium key",       None, "adamantium", None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0,  2, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_ADAMANTIUM, HI_SILVER, 24, OBJECT_SOUNDSET_GENERIC, O1_INDESTRUCTIBLE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("master key",           "key", "master", None, None, TOOLTYPE_GENERAL, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 2, 10, 0, 0, 0, 0, 0, 0, /* base item for master key of thievery */
    ENHANCED_UNTRAP, NO_POWER, NO_POWER, P1_POWER_1_APPLIES_WHEN_CARRIED, 0, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_KEY, O3_NO_WISH | O3_NO_GENERATION, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ROLE_ROGUE),
TOOL("lock pick",           None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 55,  2, 20, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("credit card",         None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10,  1, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_PLASTIC, CLR_WHITE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* light sources */
GENERAL_TOOL("tallow candle",   "old-fashioned candle", None, None, None, NO_ANIMATION, NO_ENLARGEMENT, OLD_FASHIONED_CANDLE_LIT_REPLACEMENT, TOOLTYPE_CANDLE, 0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 18,  1, 10, 0, 0, 0, 0, 0, 0, //STARTMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WAX, CLR_WHITE, 0, OBJECT_SOUNDSET_CANDLE, O1_NONE, O2_CANDLE, O3_IGNITABLE | O3_RELATIVE_AGE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_TOOL("wax candle",      "twisted candle", None, None, None, NO_ANIMATION, NO_ENLARGEMENT, TWISTED_CANDLE_LIT_REPLACEMENT, TOOLTYPE_CANDLE, 0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 9,  1, 20, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WAX, CLR_WHITE, 0, OBJECT_SOUNDSET_CANDLE, O1_NONE, O2_CANDLE, O3_IGNITABLE | O3_RELATIVE_AGE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), //ENDMARKER
GENERAL_TOOL("magic candle",    "handcrafted candle", None, None, None, NO_ANIMATION, NO_ENLARGEMENT, HANDCRAFTED_CANDLE_LIT_REPLACEMENT, TOOLTYPE_CANDLE, 0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 3,  1, 20, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WAX, CLR_WHITE, 0, OBJECT_SOUNDSET_CANDLE, O1_NONE, O2_CANDLE, O3_IGNITABLE | O3_RELATIVE_AGE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_TOOL("brass lantern",       None, None, None, None, NO_ANIMATION, NO_ENLARGEMENT, BRASS_LANTERN_LIT_REPLACEMENT, TOOLTYPE_LANTERN, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 30, 30, 12, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_COPPER, CLR_YELLOW, 0, OBJECT_SOUNDSET_LANTERN, O1_NONE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE | O3_REFILLABLE_WITH_OIL, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_TOOL("oil lamp",          "antiquated brass lamp", None, None, None, NO_ANIMATION, NO_ENLARGEMENT, ANTIQUATED_BRASS_LAMP_LIT_REPLACEMENT, TOOLTYPE_LAMP, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 37, 20, 10, 0, 0, 0, 0, 0, 0, //STARTMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_COPPER, CLR_YELLOW, 32, OBJECT_SOUNDSET_LAMP, O1_NONE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE | O3_REFILLABLE_WITH_OIL, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_TOOL("magic lamp",        "oriental brass lamp", None, None, None, NO_ANIMATION, NO_ENLARGEMENT, ORIENTAL_BRASS_LAMP_LIT_REPLACEMENT, TOOLTYPE_LAMP, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 15, 20, 50, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_COPPER, CLR_YELLOW, 32, OBJECT_SOUNDSET_LAMP, O1_NONE, O2_NONE, O3_INVOKABLE | O3_REFILLABLE_WITH_OIL | O3_IGNITABLE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL), //ENDMARKER
GENERAL_TOOL("large five-branched candelabrum", None, None, None, None, NO_ANIMATION, NO_ENLARGEMENT, LARGE_FIVE_BRANCHED_CANDELABRUM_REPLACEMENT, TOOLTYPE_CANDELABRUM, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 3, 300, 50, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 5, MAT_COPPER, CLR_YELLOW, 32, OBJECT_SOUNDSET_CANDELABRUM, O1_NONE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE | O3_NO_PICKUP, O4_FULL_SIZED_BITMAP | O4_CANDELABRUM, O5_NONE, O6_NONE, PERMITTED_ALL),

/* other tools */
TOOL("expensive camera",    None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_MAGIC_MARKER, RECHARGING_CAMERA, 10, 12,200, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_PLASTIC, CLR_BLACK, 24, OBJECT_SOUNDSET_CAMERA, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("mirror",   "looking glass", None, None, None, TOOLTYPE_GENERAL, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 25, 13, 10, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_GLASS, HI_SILVER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_INVOKABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("magic mirror", "looking glass", None, None, None, TOOLTYPE_GENERAL, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 13, 10, 0, 0, 0, 0, 0, 0, /* Base item for The Magic Mirror of Merlin */
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_GLASS, HI_SILVER, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_INVOKABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("holy symbol", "religious symbol", None, None, "Religious symbol that can be applied to turn undead", TOOLTYPE_GENERAL, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_1D15_15, RECHARGING_HOLY_SYMBOL, 10, 10,100, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_SILVER, HI_SILVER, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_SPECIAL_PRAYING_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("crystal ball", "glass orb", None, None, None, TOOLTYPE_GENERAL, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_CRYSTAL_BALL, RECHARGING_CRYSTAL_BALL, 15, 100, 60, 25, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_GLASS, HI_GLASS, 0, OBJECT_SOUNDSET_CRYSTAL_BALL, O1_NONE, O2_NONE, O3_INVOKABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("blindfold",           None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 50, 2, 20, 0, 0, 0, 0, 0, 0,
    BLINDFOLDED, NO_POWER, NO_POWER, P1_NONE, 0, MAT_CLOTH, CLR_BLACK, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("towel",               None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 35, 2, 50, 0, 0, 0, 0, 0, 0,
    BLINDFOLDED, NO_POWER, NO_POWER, P1_NONE, 0, MAT_CLOTH, CLR_MAGENTA, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("saddle",              None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 10,200,150, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_LEATHER, HI_LEATHER, 0, OBJECT_SOUNDSET_SADDLE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("leash",               None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 20, 12, 20, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_LEATHER, HI_LEATHER, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("stethoscope",         None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 25,  4, 75, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("tinning kit",         None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_MAGIC_MARKER, RECHARGING_MAGIC_MARKER, 13,100, 30, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("tin opener",          None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 13,  4, 30, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("can of grease",       "metal can", "grease", "viscous fluid", None, TOOLTYPE_CAN, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_CAN_OF_GREASE, RECHARGING_CAN_OF_GREASE, 15, 15, 20, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_CAN_OF_GREASE, O1_NONE, O2_NONE, O3_READABLE | O3_CONTENT_DESCRIPTION_SHUFFLED, O4_ALLOWS_DIPPING_INTO, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_TOOL("figurine",            None, None, None, None, NO_ANIMATION, NO_ENLARGEMENT, FIGURINE_REPLACEMENT, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 50, 80, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_MINERAL, HI_MINERAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
        /* FIGURINE REMOVED FROM THE GAME -- JG -- monster type specified by obj->corpsenm */
TOOL("magic marker",        None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_MAGIC_MARKER, RECHARGING_MAGIC_MARKER, 25,  2, 50, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_PLASTIC, CLR_RED, 24, OBJECT_SOUNDSET_MAGIC_MARKER, O1_NONE, O2_NONE, O3_READABLE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("cubic gate",        "small runed cube", None, None, "Allows the user to level teleport.", TOOLTYPE_GENERAL, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_2D6_8, RECHARGING_CUBIC_GATE, 10,  3, 250, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* traps */
TOOL("land mine",           None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 300,180, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, CLR_RED, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("beartrap",            None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 200, 60, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* instruments;
   "If tin whistles are made out of tin, what do they make foghorns out of?" */
TOOL("tin whistle",    "old whistle", None, None, None, TOOLTYPE_WHISTLE, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 49, 2, 10, 0, 0, 0, 0, 0, 0, //STARTMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("magic whistle",  "shiny whistle", None, None, None, TOOLTYPE_WHISTLE, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 40, 2, 10, 0, 0, 0, 0, 0, 0, //ENDMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_METAL, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("wooden flute",     "oak flute", None, None, None, TOOLTYPE_FLUTE, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 6, 5, 12, 0, 0, 0, 0, 0, 0, //STARTMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WOOD, HI_WOOD, 32, OBJECT_SOUNDSET_FLUTE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("magic flute",      "blackwood flute", None, None, None, TOOLTYPE_FLUTE, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_HORN_NORMAL, RECHARGING_MUSICAL_INSTRUMENT, 3, 5, 36, 0, 0, 0, 0, 0, 0, //ENDMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WOOD, HI_WOOD, 32, OBJECT_SOUNDSET_FLUTE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("brass horn",       None, None, None, None, TOOLTYPE_HORN, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_MUSICAL_INSTRUMENT, 5, 18, 10, 0, 0, 0, 0, 0, 0, //STARTMARKER FOR HORN WISH CLASS
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_COPPER, CLR_YELLOW, 0, OBJECT_SOUNDSET_HORN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("tooled horn",       "arched horn", None, None, None, TOOLTYPE_HORN, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_1D15_15, RECHARGING_MUSICAL_INSTRUMENT, 5, 18, 15, 0, 0, 0, 0, 0, 0, //STARTMARKER FOR SHUFFLED HORNS
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_BONE, CLR_WHITE, 0, OBJECT_SOUNDSET_HORN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SPELLTOOL("frost horn",   "spiral horn", "blast of frost", None, None, TOOLTYPE_HORN, 0, 0, 1, CHARGED_1D30_30, RECHARGING_MUSICAL_INSTRUMENT, 3, 18, 50, RAY, RAY_WND_COLD, AD_COLD, 12, 6, 0, 0, 0, 0, 0, 0, P_WAND, MAT_BONE, CLR_WHITE, 0, OBJECT_SOUNDSET_HORN, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELLTOOL("fire horn",    "curved horn", "blast of fire", None, None, TOOLTYPE_HORN, 0, 0, 1, CHARGED_1D30_30, RECHARGING_MUSICAL_INSTRUMENT, 3, 18, 50, RAY, RAY_WND_FIRE, AD_FIRE, 12, 6, 0, 0, 0, 0, 0, 0, P_WAND, MAT_BONE, CLR_WHITE, 0, OBJECT_SOUNDSET_HORN, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELLTOOL("horn of chaos", "polished horn", None, None, None, TOOLTYPE_HORN, 0, 0, 1, CHARGED_1D4_4, RECHARGING_MUSICAL_INSTRUMENT, 3, 18, 50, NODIR, NODIR_NONE, AD_NONE, 0, 0, 0, 1, 10, 70, 0, 0, P_WAND, MAT_BONE, CLR_WHITE, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
TOOL("horn of plenty",    "twisted horn", None, None, None, TOOLTYPE_HORN, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_BAG_OF_TRICKS, RECHARGING_TOOL_SPECIAL_MAGICAL, 3, 18, 50, 300, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_BONE, CLR_WHITE, 0, OBJECT_SOUNDSET_HORN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),//ENDMARKER FOR HORNS
        /* horn, but not an instrument */
TOOL("wooden harp",       "ornamental harp", None, None, None, TOOLTYPE_HARP, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_MUSICAL_INSTRUMENT, 4, 30, 50, 0, 0, 0, 0, 0, 0, //STARTMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_HARP, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("magic harp",        "runed harp", None, None, None, TOOLTYPE_HARP, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_HORN_NORMAL, RECHARGING_MUSICAL_INSTRUMENT, 3, 30, 50, 0, 0, 0, 0, 0, 0, //ENDMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_WOOD, HI_WOOD, 0, OBJECT_SOUNDSET_HARP, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("bell",                None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_MUSICAL_INSTRUMENT,  3, 30, 50, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_COPPER, HI_COPPER, 32, OBJECT_SOUNDSET_BELL, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("bugle",               None, None, None, None, TOOLTYPE_GENERAL, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_HORN_NORMAL, RECHARGING_MUSICAL_INSTRUMENT, 4, 10, 15, 0, 0, 0, 0, 0, 0,
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_COPPER, HI_COPPER, 0, OBJECT_SOUNDSET_HORN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("leather drum",      "old drum", None, None, None, TOOLTYPE_DRUM, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 4, 25, 25, 0, 0, 0, 0, 0, 0, //STARTMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_LEATHER, HI_LEATHER, 0, OBJECT_SOUNDSET_DRUM, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
TOOL("drum of earthquake","antiquated drum", None, None, None, TOOLTYPE_DRUM, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_HORN_NORMAL, RECHARGING_MUSICAL_INSTRUMENT, 3, 25, 25, 0, 0, 0, 0, 0, 0, //ENDMARKER
    NO_POWER, NO_POWER, NO_POWER, P1_NONE, 0, MAT_LEATHER, HI_LEATHER, 0, OBJECT_SOUNDSET_DRUM, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GENERAL_SPELLTOOL("jar of extra healing salve", "glass jar", "healing salve", "yellow ointment", "Heals a target for 12d6 hit points", NO_ANIMATION, NO_ENLARGEMENT, JAR_YELLOW_REPLACEMENT,
    TOOLTYPE_JAR, 0, 0, 1, CHARGED_1D15_15, RECHARGING_SALVE, //STARTMARKER for jars and salves and shuffled jars
    20, 15, 150, TOUCH, TOUCH_NONE, AD_HEAL, 12, 6, 0, 0, 0, 0, 0, 0,
    P_NONE, MAT_GLASS, HI_GLASS, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_READABLE | O3_CONTENT_DESCRIPTION_SHUFFLED, O4_ALLOWS_DIPPING_INTO, O5_NONE, O6_NONE),
GENERAL_SPELLTOOL("jar of greater healing salve", "crystal jar", "healing salve", "purple ointment", "Heals a target for 24d6 hit points", NO_ANIMATION, NO_ENLARGEMENT, JAR_PURPLE_REPLACEMENT,
    TOOLTYPE_JAR, 0, 0, 1, CHARGED_1D15_15, RECHARGING_SALVE,
    10, 15, 300, TOUCH, TOUCH_NONE, AD_HEAL, 24, 6, 0, 0, 0, 0, 0, 0,
    P_NONE, MAT_GLASS, HI_GLASS, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_READABLE | O3_CONTENT_DESCRIPTION_SHUFFLED, O4_ALLOWS_DIPPING_INTO | O4_RARE, O5_NONE, O6_NONE),
GENERAL_SPELLTOOL("jar of medicinal salve", "crystalline jar", "medicinal salve", "white ointment", "Cures a target from sickness", NO_ANIMATION, NO_ENLARGEMENT, JAR_WHITE_REPLACEMENT,
    TOOLTYPE_JAR, 0, 0, 1, CHARGED_1D6_6, RECHARGING_SALVE,
    20, 15, 150, TOUCH, TOUCH_NONE, AD_HEAL, 0, 0, 0, 0, 0, 0, 0, 0,
    P_NONE, MAT_GLASS, HI_GLASS, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_READABLE | O3_CONTENT_DESCRIPTION_SHUFFLED, O4_ALLOWS_DIPPING_INTO, O5_NONE, O6_NONE),
GENERAL_SPELLTOOL("jar of prodigious healing salve", "transparent jar", "healing salve", "violet ointment", "Heals a target for 48d6 hit points", NO_ANIMATION, NO_ENLARGEMENT, JAR_VIOLET_REPLACEMENT,
    TOOLTYPE_JAR, 0, 0, 1, CHARGED_1D15_15, RECHARGING_SALVE,  //ENDMARKER for salves and shuffled jars
    5, 15, 500, TOUCH, TOUCH_NONE, AD_HEAL, 48, 6, 0, 0, 0, 0, 0, 0,
    P_NONE, MAT_GLASS, HI_GLASS, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_READABLE | O3_CONTENT_DESCRIPTION_SHUFFLED, O4_ALLOWS_DIPPING_INTO | O4_VERY_RARE, O5_NONE, O6_NONE), // Endmarker for jars
GENERAL_SPELLTOOL("jar of basilisk blood", "ornamental jar", "basilisk blood", "crimson liquid", "Cures a target from petrification", NO_ANIMATION, NO_ENLARGEMENT, JAR_CRIMSON_REPLACEMENT,
    TOOLTYPE_JAR, 0, 0, 1, CHARGED_1D4_4, RECHARGING_SALVE,
    15, 15, 150, TOUCH, TOUCH_NONE, AD_HEAL, 0, 0, 0, 0, 0, 0, 0, 0,
    P_NONE, MAT_GLASS, HI_GLASS, 32, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_READABLE | O3_QUAFFABLE, O4_ALLOWS_DIPPING_INTO, O5_NONE, O6_NONE),
SPELLTOOL("grail of healing", "wooden grail", "sacred wine", "red liquid", "Heals a target for 1000 hit points and 500 mana", 
    TOOLTYPE_GRAIL, 0, 0, 1, CHARGED_HOLY_GRAIL, RECHARGING_HOLY_GRAIL,  //Base item for Holy Grail
    0, 15, 1000, TOUCH, TOUCH_NONE, AD_HEAL, 0, 0, 1000, 0, 0, 0, 0, 0,
    P_NONE, MAT_WOOD, CLR_BROWN, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NO_WISH | O3_NO_GENERATION | O3_QUAFFABLE, O4_ALLOWS_DIPPING_INTO, O5_NONE, O6_NONE),

/* tools useful as weapons */
WEPTOOL("pick-axe", None, None,
    1, 0, 0, 15, 75, 50,  
    AD_PHYS, 1, 6, 0, 1, 3, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    WHACK, 0, TOOLTYPE_PICK_AXE, P_PICK_AXE, 0, 0, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEPTOOL("spade of colossal excavation", "large shovel", None,
    0, 1, 1,  5, 60, 100,
    AD_PHYS, 1, 10, 1, 2, 6, 0, AD_PHYS, 0, 0, 0, A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS, A2_NONE, 0,
    SLASH, 0, TOOLTYPE_SHOVEL, P_PICK_AXE, 0, 0, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_DOUBLE_DIGGING_EFFORT | O3_TARGET_PERMISSION_IS_M4_FLAG, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, M4_STONY),
WEPTOOL("saw of mighty cutting", "silvery saw", None,
    0, 1, 1,  4, 15, 50,
    AD_PHYS, 1, 3, 0, 1, 4, 0, AD_PHYS, 4, 8, 0, A1_NONE, A2_NONE, 0,
    SLASH, 0, TOOLTYPE_SAW, P_SWORD, 0, 0, MAT_ADAMANTIUM, HI_SILVER, 0, OBJECT_SOUNDSET_GENERIC,
    O1_INDESTRUCTIBLE, O2_NONE, O3_DOUBLE_DIGGING_EFFORT | O3_TARGET_PERMISSION_IS_M4_FLAG, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL, M4_WOODEN),
WEPTOOL("golf club", "club-headed metal rod", None,
    0, 0, 0,  1, 12, 50,
    AD_PHYS, 1, 3, 0, 1, 2, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
    WHACK, 0, TOOLTYPE_GOLF_CLUB, P_BLUDGEONING_WEAPON, 0, 0, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC,
    O1_NONE, O2_NONE, O3_NONE, O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
WEPTOOL("grappling hook", "iron hook", None,
    0, 0, 0,  4, 30,  50,
    AD_PHYS, 1, 2, 0, 1, 6, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0, WHACK, 0, TOOLTYPE_HOOK, P_THROWN_WEAPON, 0, 0, MAT_IRON, HI_METAL, 0, OBJECT_SOUNDSET_GENERIC,
    O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NON_MYTHIC, O5_NONE, O6_NONE, PERMITTED_ALL, ALL_TARGETS),
OBJECT(OBJ("unicorn horn", None, None, None, "Unicorn horn is known for its powers to cure ailments.", 0, 0, 0, 0),
       BITS(1, 0, 1, 0, 1, ENCHTYPE_GENERAL_WEAPON, CHARGED_HORN_NORMAL, RECHARGING_UNICORN_HORN, 0, 0, 0, 0, 0, TOOLTYPE_HORN, P_SPEAR, MAT_BONE),
       NO_POWER, NO_POWER, NO_POWER, P1_NONE, TOOL_CLASS, 0, MULTIGEN_SINGLE, 0, 20, 100,
       AD_PHYS, 1, 12, 0, 1, 12, 0, AD_PHYS, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       100, CLR_WHITE, OBJECT_SOUNDSET_GENERIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_WEAPON_TOOL | O4_NON_MYTHIC, O5_NONE, O6_NONE),

/* two unique tools;
 * not artifacts, despite the comment which used to be here
 */
OBJECT(OBJ("Candelabrum of Invocation", "candelabrum", None, None, None, 0, 0, 0, CANDELABRUM_REPLACEMENT),
       BITS(0, 0, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 1, 1, 0, 0, 0, TOOLTYPE_CANDELABRUM, P_NONE, MAT_GOLD),
       0, 0, 0, P1_NONE, TOOL_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 5000,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       200, HI_GOLD, OBJECT_SOUNDSET_CANDELABRUM, 0, 0, 0, 7, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE | O3_NO_GENERATION | O3_NO_WISH | O3_UNBURIABLE, O4_CANDELABRUM, O5_NONE, O6_NONE),
OBJECT(OBJ("Bell of Opening", "silver bell", None, None, None, 32, 0, 0, 0),
       BITS(0, 0, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_ALWAYS_3, RECHARGING_BELL_OF_OPENING, 1, 1, 0, 0, 0, TOOLTYPE_BELL, P_NONE, MAT_SILVER),
       0, 0, 0, P1_NONE, TOOL_CLASS, 0, MULTIGEN_SINGLE, 0, 10, 5000,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       50, HI_SILVER, OBJECT_SOUNDSET_BELL, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE, O2_NONE, O3_NO_GENERATION | O3_NO_WISH | O3_UNBURIABLE, O4_NONE, O5_NONE, O6_NONE),
#undef TOOL
#undef WEPTOOL
#undef SPELLTOOL
#undef CONTAINER
#undef UNICORNHORN

/* Comestibles ... */
#define GENERAL_FOOD(name, desc, itemdesc, sub, known, prob, multigen, delay, wt, uses_known, material, ediblesubtype, edibleeffect, acquireprob, durdice, durdiesize, durplus, nutrition, color, height,soundset, flags, flags2, flags3, flags4, flags5, flags6,powconfermask,anim,enl,repl)         \
    OBJECT(OBJ(name, desc, None, None, itemdesc, height, anim, enl, repl),                                     \
           BITS(known, 1, uses_known, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, sub, P_NONE, material), \
           0, 0, 0, 0, FOOD_CLASS, prob, multigen, delay, wt, nutrition / 20 + 5,  \
           0, 0, 0, 0, durdice, durdiesize, durplus, 0, 0, 0, 0, A1_NONE, A2_NONE, acquireprob, \
           0, 0, 0, 0, ediblesubtype, edibleeffect, 0, 0, 0, 0, 0, 0, \
           nutrition, color, soundset, \
           0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)

#define FOOD(name, desc, itemdesc, sub, known, prob, multigen, delay, wt, uses_known, material, ediblesubtype, edibleeffect, acquireprob, durdice, durdiesize, durplus, nutrition, color, height,soundset, flags, flags2, flags3, flags4, flags5, flags6,powconfermask)         \
    GENERAL_FOOD(name, desc, itemdesc, sub, known, prob, multigen, delay, wt, uses_known, material, ediblesubtype, edibleeffect, acquireprob, durdice, durdiesize, durplus, nutrition, color, height,soundset, flags, flags2, flags3, flags4, flags5, flags6,powconfermask, 0, 0, 0)
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
FOOD("tripe ration",        None, None, FOODTYPE_RATION, 1,
    140, MULTIGEN_SINGLE, 2, 10, 0, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    200, CLR_BROWN, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("corpse",                None, None, FOODTYPE_CORPSE, 1,
    0, MULTIGEN_SINGLE, 1,  0, 0, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    0, CLR_BROWN, 0, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE /*| O4_FULL_SIZED_BITMAP*/, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("egg",                    None, None, FOODTYPE_EGG, 1,
    85, MULTIGEN_SINGLE, 1,  1, 1, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_EGG, 100, 0, 0, 0, 
    80, CLR_WHITE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("meatball",            None, None, FOODTYPE_MEAT, 1,
    0, MULTIGEN_SINGLE, 1,  1, 0, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    5, CLR_BROWN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("meat stick",            None, None, FOODTYPE_MEAT, 1,
    0, MULTIGEN_SINGLE, 1,  1, 0, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    5, CLR_BROWN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("huge chunk of meat",    None, None, FOODTYPE_MEAT, 1,
    0, MULTIGEN_SINGLE, 20, 400, 0, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    2000, CLR_BROWN, 48, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* special case because it's not mergable */
OBJECT(OBJ("meat ring", None, None, None, None, 24, 0, 0, 0),
       BITS(1, 0, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, FOODTYPE_MEAT, 0, MAT_FLESH),
       0, 0, 0, P1_NONE, FOOD_CLASS, 0, MULTIGEN_SINGLE, 1, 5, 1,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       5, CLR_BROWN, OBJECT_SOUNDSET_GENERIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
/* pudding 'corpses' will turn into these and combine;
   must be in same order as the pudding monsters */
GENERAL_FOOD("glob of gray ooze",        "gray glob", None, FOODTYPE_GLOB, 0,
    0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    20, CLR_GRAY, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, NO_ANIMATION, NO_ENLARGEMENT, GRAY_GLOB_REPLACEMENT),
GENERAL_FOOD("glob of brown pudding",    "brown glob", None, FOODTYPE_GLOB, 0,
    0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    20, CLR_BROWN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, NO_ANIMATION, NO_ENLARGEMENT, BROWN_GLOB_REPLACEMENT),
GENERAL_FOOD("glob of green slime",        "green glob", None, FOODTYPE_GLOB, 0,
    0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    20, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, NO_ANIMATION, NO_ENLARGEMENT, GREEN_GLOB_REPLACEMENT),
GENERAL_FOOD("glob of black pudding",    "black glob", None, FOODTYPE_GLOB, 0,
    0, MULTIGEN_SINGLE, 2, 20, 1, MAT_FLESH, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    20, CLR_BLACK, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL, NO_ANIMATION, NO_ENLARGEMENT, BLACK_GLOB_REPLACEMENT),

/* fruits & veggies */
FOOD("kelp frond", None, None, FOODTYPE_KELP,        1,
    0, MULTIGEN_SINGLE,        1,  1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    30, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("eucalyptus leaf", None, "Cures sickness when eaten", FOODTYPE_LEAF,        1,
    5, MULTIGEN_SINGLE,        1,  1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_CURE_SICKNESS, 100, 0, 0, 0, 
    30, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("handful of nuts", None, None, FOODTYPE_NUTS,        1,
    10, MULTIGEN_1D6,        1,    2, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    100, CLR_BROWN, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("apple", None, None, FOODTYPE_FRUIT,        1,
    10, MULTIGEN_1D2,        1,  5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_APPLE, 100, 0, 0, 0, 
    150, CLR_RED, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("orange", None, None, FOODTYPE_FRUIT,        1,
    10, MULTIGEN_1D2,        1,  5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    150, CLR_ORANGE, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("pear", None, None, FOODTYPE_FRUIT,        1,
    10, MULTIGEN_1D2,        1,  5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    150, CLR_BRIGHT_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("cluster of dates", None, None, FOODTYPE_FRUIT,        1,
    5, MULTIGEN_1D2,        1,  20, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    600, CLR_BROWN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("melon", None, None, FOODTYPE_FRUIT,        1,
    10, MULTIGEN_SINGLE,        1,  80, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    800, CLR_BRIGHT_GREEN, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("carrot", None, "Cures blindness when eaten", FOODTYPE_VEGETABLE, 1,
    10, MULTIGEN_1D3,        1,  3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_CURE_BLINDNESS, 100, 0, 0, 0, 
    50, CLR_ORANGE, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* attribute-increasing food stuffs */
FOOD("handful of spinach leaves", None, "Increases strength when eaten", FOODTYPE_LEAF, 1,
    2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_GAIN_STRENGTH, 100, 0, 0, 0, 
    50, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("banana", None, "Increases dexterity when eaten", FOODTYPE_FRUIT, 1,
    2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_GAIN_DEXTERITY, 100, 0, 0, 0, 
    50, CLR_YELLOW, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("pomegranate", None, "Increases constitution when eaten", FOODTYPE_FRUIT, 1,
    2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_GAIN_CONSTITUTION, 100, 0, 0, 0, 
    50, CLR_RED, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("avocado", None, "Increases charisma when eaten", FOODTYPE_FRUIT, 1,
    2, MULTIGEN_SINGLE, 1, 5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_GAIN_CHARISMA, 100, 0, 0, 0, 
    50, CLR_BRIGHT_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* special effect foods */
FOOD("cloudberry", None, "Restores ability when eaten", FOODTYPE_FRUIT, 1,
    15, MULTIGEN_SINGLE, 1, 1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_RESTORE_ABILITY, 100, 0, 0, 0, 
    10, CLR_ORANGE, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("fig", None, "Cures sickness when eaten", FOODTYPE_FRUIT, 1,
    40, MULTIGEN_1D2, 1, 1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_CURE_SICKNESS, 100, 0, 0, 0,
    10, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("dragon fruit", None, "Cures petrification when eaten", FOODTYPE_FRUIT, 1,
    25, MULTIGEN_1D2, 1, 1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_CURE_PETRIFICATION, 100, 0, 0, 0,
    10, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("phantomberry", None, "Grants one experience level", FOODTYPE_FRUIT, 1,
    2, MULTIGEN_SINGLE, 1, 1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_GAIN_LEVEL, 100, 0, 0, 0,
    10, CLR_BLACK, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* shuffled mushrooms */
FOOD("champignon", "brown mushroom", "Edible mushroom", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2,    1, 3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0,
    100, CLR_BROWN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("penny bun", "black mushroom", "Edible mushroom", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2,    1, 3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0,
    100, CLR_BLACK, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("chanterelle", "golden mushroom", "Edible mushroom", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0,
    100, CLR_YELLOW, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("fly agaric", "red mushroom", "Poisonous mushroom", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_POISONOUS, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0,
    100, CLR_RED, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("death cap", "pale white mushroom", "Deadly poisonous mushroom", FOODTYPE_MUSHROOM, 0,
    4, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_DEADLY_POISONOUS, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    50, CLR_WHITE, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("mana mushroom", "violet mushroom", "Confers temporary mana regeneration", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, ENERGY_REGENERATION, 100, 1, 50, 50, 
    50, CLR_MAGENTA, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("healing mushroom", "orange mushroom", "Confers temporary regeneration", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, REGENERATION,100, 1, 50, 50, 
    50, CLR_ORANGE, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("panther cap", "gray mushroom", "Confers blind telepathy", FOODTYPE_MUSHROOM, 0,
    3, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, BLIND_TELEPATHY, 100, 0, 0, 0, 
    50, CLR_GRAY, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("oracular toadstool", "green mushroom", "Causes hallucination", FOODTYPE_MUSHROOM, 0,
    5, MULTIGEN_1D2, 1, 3, 0, MAT_VEGGY, EDIBLETYPE_HALLUCINATING, EDIBLEFX_NO_EFFECT, 0, 1, 100, 100, 
    100, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_EATING_IDENTIFIES | O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* end of suffled mushrooms */

/* name of slime mold is changed based on player's OPTION=fruit:something
   and bones data might have differently named ones from prior games */
FOOD("slime mold", None, None, FOODTYPE_MOLD, 1,
    60, MULTIGEN_SINGLE,    1,  5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    300, HI_ORGANIC, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* people food */
FOOD("lump of royal jelly", None, "Increases strength when eaten", FOODTYPE_JELLY, 1,
    0, MULTIGEN_SINGLE,    1,  2, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_ROYAL_JELLY, 100, 0, 0, 0, 
    200, CLR_YELLOW, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("cream pie", None, None, FOODTYPE_CAKE, 1,
    15, MULTIGEN_SINGLE,    1, 10, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    150, CLR_WHITE, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("candy bar", None, None, FOODTYPE_CANDY, 1,
    15, MULTIGEN_SINGLE,    1,  2, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    150, CLR_BROWN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("fortune cookie", None, "Contains an aphorism or another message", FOODTYPE_CAKE, 1,
    15, MULTIGEN_SINGLE,    1,  1, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_READ_FORTUNE, 100, 0, 0, 0, 
    50, CLR_YELLOW, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("pancake", None, None, FOODTYPE_CAKE, 1,
    20, MULTIGEN_SINGLE,    2,  2, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    250, CLR_YELLOW, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("elven waybread", None, None, FOODTYPE_BREAD, 1,
    30, MULTIGEN_SINGLE,    2,  5, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    1000, CLR_WHITE, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_ELVEN_ITEM, O3_NONROTTING_FOOD, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("cram ration", None, None, FOODTYPE_RATION, 1,
    30, MULTIGEN_SINGLE,    3, 15, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    900, HI_ORGANIC, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONROTTING_FOOD, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("food ration", None, None, FOODTYPE_RATION, 1,
    318, MULTIGEN_SINGLE,    5, 20, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    800, HI_ORGANIC, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("K-ration", None, None, FOODTYPE_RATION, 1,
    0, MULTIGEN_SINGLE,    1, 10, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    525, HI_ORGANIC, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
FOOD("C-ration", None, None, FOODTYPE_RATION, 1,
    0, MULTIGEN_SINGLE,    1, 10, 0, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    450, HI_ORGANIC, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* tins have type specified by obj->special_quality (+1 for spinach, other implies
   flesh; negative specifies preparation method {homemade,boiled,&c})
   and by obj->corpsenm (type of monster flesh) */
FOOD("tin", None, None, FOODTYPE_TIN, 1,
    70, MULTIGEN_SINGLE,    0, 10, 1, MAT_METAL, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 0, 0, 0, 
    0, HI_METAL, 16, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#undef FOOD

/* potions ... */
#define POTION(name,desc,itemdesc,mgc,power1,power2,power3,prob,cost,breathe_n,breathe_d,breathe_p,breathe_pbuc,normal_n,normal_d,normal_p,normal_pbuc,nut_n,nut_d,nut_p,nut_pbuc,extra1,breathe_dicebuc,normal_dicebuc,color,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, None, None, itemdesc, 32, 0, 0, 0),                                           \
           BITS(0, 1, 0, 0, mgc, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, MAT_GLASS),      \
           power1, power2, power3, P1_NONE, POTION_CLASS, prob, MULTIGEN_SINGLE, 0, 12, cost,  \
           0, breathe_n,breathe_d,breathe_p, normal_n,normal_d,normal_p, 0, nut_n, nut_d, nut_p, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, breathe_pbuc, normal_pbuc, nut_pbuc, extra1, breathe_dicebuc, normal_dicebuc, 0, 0, \
           10, color, OBJECT_SOUNDSET_POTION, \
           0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4 | O4_FLOOR_TILE /*| O4_MISSILE_TILE | O4_SINGLE_MISSILE_TILE */, flags5, flags6)
POTION("gain ability",           "ruby", 
    "Increases a random ability score by 1. Blessed potions have twice the normal effect.",
    1, NO_POWER, NO_POWER, NO_POWER, 20, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_RED, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("restore ability",        "pink",
    "Restores one or more lost ability points and experience levels",
    1, NO_POWER, NO_POWER, NO_POWER, 30, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("confusion",            "orange", None, 1, CONFUSION, NO_POWER, NO_POWER, 30, 100, 4, 8, 0, 0, 6, 8, 0, 0, 0, 0, 0, 0, 0, -1, -2, CLR_ORANGE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("blindness",            "yellow", None, 1, BLINDED, NO_POWER, NO_POWER, 30, 150, 4, 8, 0, 0, 1, 200, 250, -125, 0, 0, 0, 0, 0, -1, 0, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("paralysis",           "emerald", None, 1, NO_POWER, NO_POWER, NO_POWER, 30, 300, 3, 4, 0, 0, 5, 4, 0, 0, 0, 0, 0, 0, 0, -1, -2, CLR_BRIGHT_GREEN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("speed",            "dark green", None, 1, ULTRA_FAST, NO_POWER, NO_POWER, 25, 200, 1, 5, 0, 0, 1, 10, 100, 60, 0, 0, 0, 0, 0, 0, 0, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("greater speed",    "shimmering", None, 1, SUPER_FAST, NO_POWER, NO_POWER, 20, 300, 1, 5, 0, 0, 1, 10, 100, 60, 0, 0, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("lightning speed",      "bright", None, 1, LIGHTNING_FAST, NO_POWER, NO_POWER, 15, 400, 1, 5, 0, 0, 1, 10, 100, 60, 0, 0, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("levitation",             "cyan", None, 1, LEVITATION, NO_POWER, NO_POWER, 25, 200, 0, 0, 0, 0, 1, 140, 10, 0, 0, 0, 0, 0, 240, 0, 0, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("hallucination",      "sky blue", None, 1, HALLUC, NO_POWER, NO_POWER, 25, 100, 0, 0, 0, 0, 1, 200, 600, -300, 0, 0, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("invisibility", "brilliant blue", None, 1, INVISIBILITY, NO_POWER, NO_POWER, 25, 150, 0, 0, 0, 0, 10, 10, 400, 0, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_BLUE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("see invisible",       "magenta", None, 1, SEE_INVISIBLE, NO_POWER, NO_POWER, 20, 50, 0, 0, 0, 0, 1, 100, 750, 0, 0, 0, 0, 0, 0, 0, 0, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("magic resistance",         "gray", 
    "Grants magic resistance for a duration",
    1, ANTIMAGIC, NO_POWER, NO_POWER, 15, 100, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("fire immunity",       "crimson", 
    "Grants full fire resistance for a duration",
    1, FIRE_IMMUNITY, NO_POWER, NO_POWER, 15, 100, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_RED, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("cold immunity",   "bright cyan", 
    "Grants full cold resistance for a duration",
    1, COLD_IMMUNITY, NO_POWER, NO_POWER, 15, 100, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_CYAN, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("shock immunity",   "bright magenta", 
    "Grants full shock resistance for a duration",
    1, SHOCK_IMMUNITY, NO_POWER, NO_POWER, 15, 100, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_MAGENTA, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("titan strength",          "red", 
    "Raises strength to 25 for a duration",
    1, TITAN_STRENGTH, NO_POWER, NO_POWER, 20, 100, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_RED, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("heroism",              "purple", 
    "All attacks deal 50% more damage for a duration",
    1, HEROISM, NO_POWER, NO_POWER, 20, 200, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("super-heroism",        "violet", 
    "All attacks deal double damage for a duration",
    1, SUPER_HEROISM, NO_POWER, NO_POWER, 10, 400, 1, 5, 0, 0, 1, 10, 180, 90, 0, 0, 0, 0, 0, 0, 0, CLR_BRIGHT_MAGENTA, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("lesser regeneration",   "ochre", 
    "Restores hit points over time",
    1, RAPIDER_REGENERATION, NO_POWER, NO_POWER, 20, 100, 1, 5, 0, 0, 1, 10, 40, 15, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("regeneration",     "dark ochre", 
    "Restores a large amount of hit points over time",
    1, RAPIDEST_REGENERATION, NO_POWER, NO_POWER, 15, 200, 1, 5, 0, 0, 1, 10, 40, 15, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("greater regeneration",  "umber", 
    "Restores a great amount of hit points over time",
    1, DIVINE_REGENERATION, NO_POWER, NO_POWER, 10, 300, 1, 5, 0, 0, 1, 10, 40, 15, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("lesser rejuvenation",   "olive", 
    "Restores hit points and mana over time", 
    1, RAPID_REGENERATION, RAPID_ENERGY_REGENERATION, NO_POWER, 20, 100, 1, 5, 0, 0, 1, 10, 40, 15, 0, 0, 0, 0, 0, 0, 0, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("rejuvenation",     "lime green", 
    "Restores a large amount of hit points and mana over time",
    1, RAPIDER_REGENERATION, RAPIDER_ENERGY_REGENERATION, NO_POWER, 15, 200, 1, 5, 0, 0, 1, 10, 40, 15, 0, 0, 0, 0, 0, 0, 0, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("greater rejuvenation","verdant", 
    "Restores a great amount of hit points and mana over time",
    1, RAPIDEST_REGENERATION, RAPIDEST_ENERGY_REGENERATION, NO_POWER, 10, 300, 1, 5, 0, 0, 1, 10, 40, 15, 0, 0, 0, 0, 0, 0, 0, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("healing",          "purple-red", 
    "Restores 12d6 hit points", 
    1, NO_POWER, NO_POWER, NO_POWER, 25, 100, 0, 0, 0, 0, 12, 6, 0, 0, 0, 0, 0, 0, 1, 0, 6, CLR_MAGENTA, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("extra healing",          "puce", 
    "Restores 24d6 hit points", 
    1, NO_POWER, NO_POWER, NO_POWER, 25, 150, 0, 0, 0, 0, 24, 6, 0, 0, 0, 0, 0, 0, 2, 0, 15, CLR_RED, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("greater healing",     "deep red", 
    "Restores 36d8 hit points", 
    1, NO_POWER, NO_POWER, NO_POWER, 15, 200, 0, 0, 0, 0, 36, 8, 0, 0, 0, 0, 0, 0, 3, 0, 18, CLR_RED, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("full healing",          "black", 
    "Restores up to 1000 hit points", 
    1, NO_POWER, NO_POWER, NO_POWER, 10, 300, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 4, 0, 0, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("gain level",            "milky", 
    "Adds one experience level. Cursed potions influence the dungeon level instead.", 
    1, NO_POWER, NO_POWER, NO_POWER, 10, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("enlightenment",        "swirly", None, 1, NO_POWER, NO_POWER, NO_POWER, 25, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("monster detection",    "bubbly", None, 1, NO_POWER, NO_POWER, NO_POWER, 25, 150, 0, 0, 0, 0, 1, 50, 100, 0, 0, 0, 0, 0, 0, 0, 0, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("object detection",      "smoky", None, 1, NO_POWER, NO_POWER, NO_POWER, 25, 150, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("gain energy",          "cloudy", 
    "Restores 6d6+20 mana", 
    1, NO_POWER, NO_POWER, NO_POWER, 25, 150, 0, 0, 0, 0, 6, 6, 20, 0, 0, 0, 0, 0, 1, 0, 0, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("greater energy",     "greenish", 
    "Restores 12d6+60 mana", 
    1, NO_POWER, NO_POWER, NO_POWER, 25, 200, 0, 0, 0, 0, 12, 6, 60, 0, 0, 0, 0, 0, 2, 0, 0, CLR_GREEN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("full energy",          "silvery", 
    "Restores 24d6+500 mana",
    1, NO_POWER, NO_POWER, NO_POWER, 20, 300, 0, 0, 0, 0, 24, 6, 500, 0, 0, 0, 0, 0, 3, 0, 0, CLR_WHITE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("sleeping",       "effervescent", None, 1, NO_POWER, NO_POWER, NO_POWER, 30, 100, 3, 4, 0, 0, 5, 4, 0, 0, 0, 0, 0, 0, 0, -1, -2, CLR_GRAY, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("polymorph",            "golden", None, 1, NO_POWER, NO_POWER, NO_POWER, 10, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_YELLOW, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("elven herbal brew",     "brown", None, 0, NO_POWER, NO_POWER, NO_POWER, 20,  50, 1, 5, 0, 0, 3, 8, 0, 0, 0, 0, 20, -10, 15, 0, 0, CLR_BROWN, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("sickness",              "fizzy", None, 0, NO_POWER, NO_POWER, NO_POWER, 20,  50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("fruit juice",            "dark", None, 0, NO_POWER, NO_POWER, NO_POWER, 20,  50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, -10, 0, 0, 0, CLR_BLACK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("acid",                  "white", None, 0, NO_POWER, NO_POWER, NO_POWER, 15, 250, 0, 0, 0, 0, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, -2, CLR_WHITE, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("oil",                   "murky", None, 0, NO_POWER, NO_POWER, NO_POWER, 25,  50, 0, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_BROWN, O1_COLD_RESISTANT, O2_NONE, O3_IGNITABLE | O3_RELATIVE_AGE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("poison",             "sapphire", None, 0, NO_POWER, NO_POWER, NO_POWER, 30,  50, 0, 0, 0, 0, 1, 10, 5, -5, 0, 0, 0, 0, 3, 0, 0, CLR_BLUE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
/* fixed description
 */
POTION("water",                 "clear", None, 0, NO_POWER, NO_POWER, NO_POWER, 80, 100, 0, 0, 0, 0, 2, 6, 0, 0, 1, 10, 0, 0, 0, 0, 0, CLR_CYAN, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* MARKER FOR FIRST NON-SHUFFLED */
POTION("dwarven mushroom brew","dark ruby", None, 0, NO_POWER, NO_POWER, NO_POWER, 10,   5, 0, 0, 0, 0, 1, 4, 3, -3, 0, 0, 20, 0, 0, 0, 0, CLR_BLACK, O1_NONE, O2_DWARVEN_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
POTION("urine",           "pale yellow", None, 0, NO_POWER, NO_POWER, NO_POWER, 15,   5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CLR_YELLOW, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,itemdesc,mgc,prob,cost,sflags,flags,flags2,flags3,flags4,flags5,flags6,powconfermask)   \
    OBJECT(OBJ(name, text, None, None, itemdesc, 32, 0, 0, 0),   \
           BITS(0, 1, 0, 0, mgc, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, MAT_PAPER),    \
           0, 0, 0, P1_NONE, SCROLL_CLASS, prob, MULTIGEN_SINGLE, 0, 4, cost, \
           AD_NONE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, sflags, S2_NONE,0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           6, HI_PAPER, OBJECT_SOUNDSET_SCROLL, 0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
SCROLL("enchant armor",              "ZELGO MER", None, 1,  60,  80, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("destroy armor",         "JUYED AWK YACC", None, 1,  45, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("protect armor",                   "VEX DOL", None, 1,  30,  50, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("confuse monster",                 "NR 9", None, 1,  30, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("scare monster",   "XIXAXA XOXAXA XUXAXA", None, 1,  35, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("remove curse",             "PRATYAVAYAH", None, 1,  65,  80, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("enchant weapon",         "DAIYEN FOOELS", None, 1,  80,  60, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("protect weapon",               "ZEPH NI", None, 1,  30,  50, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("enchant accessory",              "REX SAPH", None, 1,  30,  60, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("create monster",       "LEP GEX VEN ZEA", None, 1,  40, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("taming",                   "PRIRUTSENIE", None, 1,  15, 200, S1_SPELL_IS_NONREVERSIBLE_PERMANENT, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("genocide",                  "ELBIB YLOH", None, 1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("conflict",                 "HOCUS POCUS", None, 1,  30, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("light",                 "VERR YED HORRE", None, 1,  45,  50, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("teleportation",        "VENZAR BORGAVVE", None, 1,  45, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("gold detection",                 "THARR", None, 1,  30, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("food detection",               "YUM YUM", None, 1,  25, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("identify",                  "KERNOD WEL", None, 1, 150,  20, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("magic mapping",              "ELAM EBOW", None, 1,  40, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("amnesia",                   "DUAM XNAHT", None, 1,  30, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("retraining",               "NUGG YEBBEH", None, 1,  20, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("fire",                  "ANDOVA BEGARIN", None, 1,  25, 100, S1_NONE, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("earth",                          "KIRJE", None, 1,  15, 200, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("punishment",            "VE FORBRYDERNE", None, 1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("charging",                "HACKEM MUCHE", None, 1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("stinking cloud",             "VELOX NEB", None, 1,  15, 300, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    /* Extra descriptions, shuffled into use at start of new game.
     * Code in win/share/tilemap.c depends on SCR_STINKING_CLOUD preceding
     * these and on how many of them there are.  If a real scroll gets added
     * after stinking cloud or the number of extra descriptions changes,
     * tilemap.c must be modified to match.
     */
SCROLL(None,      "FOOBIE BLETCH", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,              "TEMOV", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,         "GARVEN DEH", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,            "READ ME", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,      "ETAOIN SHRDLU", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,        "LOREM IPSUM", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,              "FNORD", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Illuminati */
SCROLL(None,            "KO BATE", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Kurd Lasswitz */
SCROLL(None,      "ABRA KA DABRA", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* traditional incantation */
SCROLL(None,       "ASHPD SODALG", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Portal */
SCROLL(None,            "ZLORFIK", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Zak McKracken */
SCROLL(None,      "GNIK SISI VLE", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Zak McKracken */
SCROLL(None,    "HAPAX LEGOMENON", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL(None,  "EIRIS SAZUN IDISI", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Merseburg Incantations */
SCROLL(None,    "PHOL ENDE WODAN", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Merseburg Incantations */
SCROLL(None,              "GHOTI", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* pronounced as 'fish',
                                                        George Bernard Shaw */
SCROLL(None, "MAPIRO MAHAMA DIROMAT", None, 1, 0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Wizardry */
SCROLL(None,  "VAS CORP BET MANI", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Ultima */
SCROLL(None,            "XOR OTA", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Aarne Haapakoski */
SCROLL(None, "STRC PRST SKRZ KRK", None, 1,   0, 100, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* Czech and Slovak
                                                        tongue-twister */
    /* These must come last because they have special fixed descriptions.
     */
SCROLL("mail",          "stamped", None, 0,   0,   0, S1_NONE, O1_NONE, O2_NONE, O3_NO_GENERATION | O3_NO_WISH, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
SCROLL("blank paper", "unlabeled", None, 0,  25,  60, S1_NONE, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_OK_FOR_ILLITERATE, O6_NONE, PERMITTED_ALL),
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
#define SPELL(name,desc,effectdesc,contentdesc,itemdesc,skill,prob,learndelay,cooldown,level,manacost,attr,range,radius,skillchance,savingthrowadj,mgc,dir,dirsubtype,extraspelldata,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,sflags,sflags2,mflags,adtype,color,soundset,flags,flags2,flags3,flags4,flags5,flags6)  \
    OBJECT(OBJ(name, desc, effectdesc, contentdesc, itemdesc, 0, 0, 0, 0),                                           \
           BITS(0, 0, 0, 0, mgc, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, dir, 0, skill, MAT_PAPER),       \
           0, 0, 0, P1_NONE, SPBOOK_CLASS, prob, MULTIGEN_SINGLE, learndelay, 50, (level + 2) * 50 + (level + 1) * (level + 1) * 5,               \
           adtype, sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, 0, 0, 0, 0, sflags, sflags2, 0, \
           0, savingthrowadj, 0, 0, cooldown, level, manacost, attr, range, radius, skillchance, extraspelldata, \
           20, color, soundset, \
           dirsubtype, 0, 0, 0, PERMITTED_ALL, mflags, flags, flags2, flags3, flags4, flags5, flags6)
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

SPELL("dig",             "parchment", None, None, "Digs stone",
      P_TRANSMUTATION_SPELL,       5,  4,             0,  2,10, A_INT, 10, 0, 100, 0, 1, RAY, RAY_DIGGING, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("knock",            "pink", None, None, "Opens the target door",
      P_TRANSMUTATION_SPELL,       5,  1,             0, -1, 4, A_INT, 12, 0, 50, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BRIGHT_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("wizard lock",     "dark green", None, None, "Locks the target door",
      P_TRANSMUTATION_SPELL,       5,  3,             0, -1, 4, A_INT, 12, 0, 50, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_GREEN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create paper golem", "flimsy", None, None, "Creates a permanent paper golem",
    P_TRANSMUTATION_SPELL,         5,   4,         0, -1, 8, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create gold golem", "gold-circled", None, None, "Creates a permanent gold golem",
    P_TRANSMUTATION_SPELL,         5,   4,         0, 1, 24, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_GOLD, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create stone golem", "stone-studded", None, None, "Creates a permanent stone golem",
    P_TRANSMUTATION_SPELL,         5,   4,         0, 4, 60, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_METAL, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create glass golem", "crystalline", None, None, "Creates a permanent glass golem",
    P_TRANSMUTATION_SPELL,         5,   4,         0, 4, 60, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create gemstone golem", "ruby-studded", None, None, "Creates a permanent gemstone golem",
    P_TRANSMUTATION_SPELL,         5,   4,         0, 5, 80, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create iron golem", "iron-reinforced", None, None, "Creates a permanent iron golem",
    P_TRANSMUTATION_SPELL,         5,   4,         0, 6,100, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create silver golem", "silvery", None, None, "Creates a permanent silver golem",
    P_TRANSMUTATION_SPELL,         4,   4,         0, 9,200, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_SILVER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("polymorph",        "silver-plated", None, None, "Polymorphs monsters and items into something else",
      P_TRANSMUTATION_SPELL,       4,  6,             0, 6, 50, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE_ONE_TO_SEVEN_TARGETS, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_SILVER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("lower water",            "obnoxious", None, None, "Evaporates water",
      P_TRANSMUTATION_SPELL,       4,  3,             0, 5, 40, A_MAX_INT_WIS, 10, 0, 100, 0, 1, RAY, RAY_EVAPORATION, 0, 6, 6, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("touch of petrification",   "convoluted", None, None, "Petrifies the target over a few rounds",
    P_TRANSMUTATION_SPELL,         4,  8,             0, 8, 80, A_INT, 1, 0, 100, -10, 1, TOUCH, TOUCH_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("flesh to stone",   "amorphous", None, None, "Shoots a ray that causes petrification over a few rounds",
    P_TRANSMUTATION_SPELL,         4,  8,             0, 9,100, A_INT, 8, 0, 100, 0, 1, RAY, RAY_PETRIFICATION, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("water breathing",        "navy blue", None, None, "Makes the caster able to breathe underwater",
    P_TRANSMUTATION_SPELL,         5,  1,             0, 3, 20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_BREATHING, 0, 0, 0, 0, 5, 6, 125, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("stone to flesh",   "thick", None, None, "Turns a statue back to a living being",
    P_TRANSMUTATION_SPELL,         5,  1,           0, 3, 20, A_MAX_INT_WIS, 1, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("titan strength",    "titanic", None, None, "Raises the caster's strength to 25",
    P_TRANSMUTATION_SPELL,         4, 1,             0, 5, 40, A_INT, 0, 0, 100, 0, 1, NODIR, TITAN_STRENGTH, 0, 0, 0, 0, 10, 6, 250, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("disintegrate",      "stained", None, None, "Shoots a disintegration ray",
    P_TRANSMUTATION_SPELL,         4, 12,             0, 10,150, A_INT, 8, 0, 100, 0, 1, RAY, RAY_DISINTEGRATION, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_DISN, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),


SPELL("magic missile",   "vellum", None, None, "Shoots a ray of magic missiles",
      P_ARCANE_SPELL,              5,  2,             0, 0, 4, A_INT, 10, 0, 20, 0, 1, RAY, RAY_MAGIC_MISSILE, 8, 3, 5, 0, 1, 5, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater magic missile", "arrow-patterned", None, None, "Shoots a massive ray of magic missiles",
      P_ARCANE_SPELL,              5,  2,             0, 6, 25, A_INT, 12, 0, 20, 0, 1, RAY, RAY_MAGIC_MISSILE, 4, 10, 6, 0, 1, 10, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("burning hands",    "hand-patterned", None, None, "Causes the target to suffer fire damage",
      P_ARCANE_SPELL,              5,  4,             0, 2, 8, A_INT, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 4, 6, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("fire bolt",        "light red", None, None, "Shoots a bolt of fire",
      P_ARCANE_SPELL,              5,  4,             0, 2, 8, A_INT, 10, 0, 20, 0, 1, RAY, RAY_FIRE, 6, 4, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("magical implosion", "radiating", "magical blast", None, "Creates a magical implosion at the target",
      P_ARCANE_SPELL,              4,  5,             0, 2, 8, A_INT, 12, 0, 20, 0, 1, RAY, RAY_MAGIC_MISSILE, 6, 4, 6, 0, 1, 6, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("fireball",         "ragged", "ball of fire", None, "Shoots an exploding fireball",
      P_ARCANE_SPELL,              4,  4,             0, 3, 10, A_INT, 14, 0, 20, 0, 1, RAY, RAY_FIRE, 8, 5, 6, 0, 1, 10, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("fire storm",       "crimson", "fire storm", None, "Conjures a storm of fire",
      P_ARCANE_SPELL,              4,  8,             0, 6, 25, A_INT, 16, 0, 50, 0, 1, RAY, RAY_FIRE, 4, 10, 6, 0, 1, 10, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("meteor swarm",     "oval-patterned", "meteor explosion", None, "Shoots 4 exploding meteors",
      P_ARCANE_SPELL,              4, 10,             0, 9, 50, A_INT, 18, 0,100, 0, 1, RAY, RAY_FIRE, 8, 5, 6, 0, 1, 10, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_PHYS, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("cone of cold",     "dog-eared", None, None, "Shoots a ray of cold",
      P_ARCANE_SPELL,              4,  6,             0, 5, 25, A_INT, 12, 0, 20, 0, 1, RAY, RAY_COLD, 3, 8, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_COLD, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("freezing touch",    "glowing blue", None, None, "Causes the target to suffer cold damage",
      P_ARCANE_SPELL,              4,  4,             0, 4, 15, A_INT, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 4, 10, 6, 0, 1, 10, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_COLD, CLR_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("ice storm",          "mithril-plated", "ice storm", None, "Conjures a storm of cold",
      P_ARCANE_SPELL,              4,  5,             0, 5, 20, A_INT, 14, 0, 20, 0, 1, RAY, RAY_COLD, 3, 8, 6, 0, 1, 6, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_COLD, HI_SILVER, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("force bolt",       "red", None, None, "Shoots a bolt of force",
      P_ARCANE_SPELL,              4,  2,             0, 1, 6, A_INT, 10, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_MAGIC_MISSILE_NONBEAM, 8, 3, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("magic arrow",      "dart-patterned", None, None, "Shoots a magical arrow",
      P_ARCANE_SPELL,              4,  1,             0, -1, 2, A_INT, 18, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_ITEM_ARROW, 12, 2, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("lightning bolt",   "shimmering", None, None, "Shoots a ray of lightning",
      P_ARCANE_SPELL,              4,  3,             0, 3, 10, A_INT, 14, 0, 20, 0, 1, RAY, RAY_LIGHTNING, 8, 5, 6, 0, 1, 10, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_ELEC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("shocking touch",   "hand-patterned", None, None, "Causes the target to be jolted with electricity",
      P_ARCANE_SPELL,              4,  3,             0, 3, 10, A_INT, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 3, 8, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_ELEC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("thunderstorm",      "strange", "thunderstorm", None, "Conjures a storm of lightning",
      P_ARCANE_SPELL,              4,  10,         0, 7, 30, A_INT, 18, 0, 50, 0, 1, RAY, RAY_LIGHTNING, 2, 12, 6, 0, 1, 6, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_ELEC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("magic storm",      "scintillating", "storm of magic", None, "Conjures a storm of arcane energy",
      P_ARCANE_SPELL,              4,  5,             0, 8, 40, A_INT, 18, 0, 50, 0, 1, RAY, RAY_MAGIC_MISSILE, 3, 16, 6, 0, 2, 6, 0, S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of magic",    "agate-studded", None, None, "Damages the monsters around you with magic",
      P_ARCANE_SPELL,              4,  4,             0, 4, 15, A_INT, 0, 1, 50, 0, 1, NODIR, NODIR_NONE, 4, 6, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of fire",    "sapphire-studded", None, None, "Damages the monsters around you with fire",
      P_ARCANE_SPELL,              4,  4,             0, 5, 20, A_INT, 0, 1, 50, 0, 1, NODIR, NODIR_NONE, 3, 8, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of lightning",    "crystal-studded", None, None, "Damages the monsters around you with lightning",
      P_ARCANE_SPELL,             4,  4,             0, 6, 25, A_INT, 0, 1, 50, 0, 1, NODIR, NODIR_NONE, 4, 10, 6, 0, 1, 10, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_ELEC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of frost",    "pearl-studded", None, None, "Damages the monsters around you with cold",
      P_ARCANE_SPELL,              4,  4,             0, 7, 30, A_INT, 0, 1, 50, 0, 1, NODIR, NODIR_NONE, 2, 12, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_COLD, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("force strike",       "translucent", None, None, "Shoots a powerful bolt of force",
      P_ARCANE_SPELL,              4,  4,             0, 7, 30, A_INT, 12, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_MAGIC_MISSILE_NONBEAM, 2, 12, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("incinerate",    "ashen", None, None, "Causes the target to suffer fire damage",
      P_ARCANE_SPELL,              4,  4,             0, 6, 25, A_INT, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 3, 16, 6, 0, 2, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("electrocute",   "cyan-hued", None, None, "Causes the target to be jolted with electricity",
      P_ARCANE_SPELL,              4,  3,             0, 7, 30, A_INT, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 2, 20, 6, 0, 1, 10, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_ELEC, CLR_CYAN, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("glacial grasp",   "icy", None, None, "Causes the target to suffer cold damage",
      P_ARCANE_SPELL,              4,  4,             0, 8, 40, A_INT, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 1, 24, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_COLD, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("flame burst",        "deep red", None, None, "Shoots a great bolt of fire",
      P_ARCANE_SPELL,              4,  4,             0, 7, 30, A_INT, 10, 0, 20, 0, 1, RAY, RAY_FIRE, 2, 12, 6, 0, 1, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_FIRE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("thunderstrike",     "dark cyan", None, None, "Shoots a great ray of lightning",
      P_ARCANE_SPELL,              4,  3,             0, 8, 40, A_INT, 14, 0, 20, 0, 1, RAY, RAY_LIGHTNING, 3, 16, 6, 0, 2, 6, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_ELEC, CLR_CYAN, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater frost ray",  "glacial", None, None, "Shoots a great ray of cold",
      P_ARCANE_SPELL,              4,  6,             0, 9, 50, A_INT, 12, 0, 20, 0, 1, RAY, RAY_COLD, 2, 20, 6, 0, 1, 10, 0, S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_COLD, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

SPELL("animate air",      "wave-patterned", None, None, "Summons a permanent air elemental",
    P_CONJURATION_SPELL,           4,  3,             0, 4,  60, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("animate earth",      "octagonal-patterned", None, None, "Summons a permanent earth elemental",
    P_CONJURATION_SPELL,           4,  3,             0, 4,  60, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("animate fire",      "flame-patterned", None, None, "Summons a permanent fire elemental",
    P_CONJURATION_SPELL,           4,  3,             0, 5,  80, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("animate water",      "water-drop-patterned", None, None, "Summons a permanent water elemental",
    P_CONJURATION_SPELL,           4,  3,             0, 3,  40, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater animate air",      "wave-patterned", None, None, "Summons a permanent elder air elemental",
    P_CONJURATION_SPELL,           3,  3,             0, 6, 100, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater animate earth",      "octagonal-patterned", None, None, "Summons a permanent elder earth elemental",
    P_CONJURATION_SPELL,           3,  3,             0, 6, 100, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater animate fire",      "flame-patterned", None, None, "Summons a permanent elder fire elemental",
    P_CONJURATION_SPELL,           3,  3,             0, 7, 120, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater animate water",      "water-drop-patterned", None, None, "Summons a permanent elder water elemental",
    P_CONJURATION_SPELL,           4,  3,             0, 5,  80, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon dragon",      "water-drop-patterned", None, None, "Summons a permanent dragon of random color",
    P_CONJURATION_SPELL,           4,  3,             0, 6, 100, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon ancient dragon",      "water-drop-patterned", None, None, "Summons a permanent ancient dragon of random color",
    P_CONJURATION_SPELL,           3,  3,             0, 8, 160, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create monster",  "turquoise", None, None, "Creates a permanent random monster",
      P_CONJURATION_SPELL,         4,  3,             0, 4,  60, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BRIGHT_CYAN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create familiar",  "glittering", None, None, "Creates a permanent tame monster",
      P_CONJURATION_SPELL,         4,  1,             0, 1,  24, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summoning call",  "whistle-patterned", None, None, "Magically teleports your pets and allies to you.",
      P_CONJURATION_SPELL,         4,  1,             0, -1,  4, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("great Yendorian summoning", "hideous", None, "ancient conjuration magic", "Summons 1d2 permanent nasties",
    P_CONJURATION_SPELL,           2,  5,             0,  9, 200, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("stinking cloud",            "peculiar", None, None, "Conjures a stinking cloud",
    P_CONJURATION_SPELL,           4,  3,             0, 2, 16, A_MAX_INT_CHA, 0, 0, 100, 0, 1, TARGETED, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("wish",              "jewel-encrusted", None, "ancient conjuration magic", "The caster can wish for an object",
      P_CONJURATION_SPELL,         2, 15,             0,10,300, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_SILVER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NO_WISH, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("black blade of disaster", "ebony", None, "long-forgotten conjuration magic", "Summons a blade that disintegrates targets on hit",
    P_CONJURATION_SPELL,           2, 10,             0,11,200, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 3, 10, 30, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("mage armor",           "ornamental", None, None, "Summons a wizard-friendly magical armor with AC4",
    P_CONJURATION_SPELL,           4,  1,             0, 1, 12, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 4, 20, 200, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("call hierarch modron",  "spherical", None, "alien conjuration magic", "Summons a permanent hierarch modron",
    P_CONJURATION_SPELL,           0,  5,             0,10,300, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NO_GENERATION, O4_NONE, O5_NONE, O6_NONE),
SPELL("sphere of annihilation",    "jet-studded", None, "long-forgotten conjuration magic", "Disintegrates everything around you",
    P_CONJURATION_SPELL,           3, 12,            50, 12,300, A_MAX_INT_CHA, 0, 5, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_DISN, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),

SPELL("commune",      "beige", None, None, "Gives hints about your praying and luck status",
    P_CLERIC_SPELL,                4,  1,             0, -1, 4, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("enlightenment",      "illuminating", None, None, "Provides enlightenment about your statistics",
    P_CLERIC_SPELL,                4,  1,             0, 3, 20, A_WIS, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("remove curse",     "wrinkled", None, None, "Removes curse from an item",
      P_CLERIC_SPELL,              4,  5,             0, 5, 40, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("bless",               "spiritual", None, None, "Blesses on object",
    P_CLERIC_SPELL,                4,  1,             0, 5, 40, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("curse",               "blasphemous", None, None, "Curses one object",
      P_CLERIC_SPELL,              4,  1,             0, 4, 30, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("resurrection",          "bright-colored", None, None, "Brings the target corpse back to life",
    P_CLERIC_SPELL,                4,  5,             0, 6, 50, A_WIS, 6, 0, 100, 0, 1, IMMEDIATE, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("prayer",               "biblical", None, None, "You safely pray to your god",
    P_CLERIC_SPELL,                4,  5,           175, 8,160, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("absolution",           "ecclesiastical", None, "ancient clerical prayer", "Absolves your sins",
    P_CLERIC_SPELL,                3,  5,          1000, 7,120, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from curses", "cross-patterned", None, None, "Items of the caster cannot be cursed",
    P_CLERIC_SPELL,                4,  1,           120, 5, 40, A_WIS, 0, 0, 100, 0, 1, NODIR, CURSE_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection",       "dull", None, None, "Gives -3 bonus to AC and +3 to MC",
    P_CLERIC_SPELL,                4,  3,             0, 2, 16, A_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_PROTECTION, 0, 0, 0, 0, 2, 10, 130, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create clay golem", "clay-covered", None, None, "Creates a permanent clay golem",
    P_CLERIC_SPELL,               4,   4,             0, 4, 60, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create food", "smelly", None, None, "Creates a food ration",
    P_CLERIC_SPELL,               4,   1,             0, 1, 12, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create fruits", "sticky", None, None, "Creates a random fruit",
    P_CLERIC_SPELL,               4,   2,             0, 2, 16, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create water", "moist", None, None, "Creates a potion of water",
    P_CLERIC_SPELL,               4,   3,             0, 3, 20, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("congregate",  "human-patterned", None, None, "Magically teleports your pets and allies to you.",
    P_CLERIC_SPELL,               4,   1,             0, 1, 12, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("divine strength",    "gigantic", None, None, "Raises the caster's strength to 25",
    P_CLERIC_SPELL,               4,   1,             0, 7, 60, A_WIS, 0, 0, 100, 0, 1, NODIR, TITAN_STRENGTH, 0, 0, 0, 0, 6, 10, 270, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("divine endurance",    "clay-studded", None, None, "Raises the caster's constitution to 25",
    P_CLERIC_SPELL,               4,   1,             0, 5, 40, A_WIS, 0, 0, 100, 0, 1, NODIR, DIVINE_ENDURANCE, 0, 0, 0, 0, 4, 10, 180, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("flame strike",    "pillar-patterned", None, None, "Projects a 3x3 fiery pillar of fire at target location",
    P_CLERIC_SPELL,               4,   1,             0, 5, 20, A_WIS, 10, 1, 100, 0, 1, TARGETED, RAY_FIRE, 2, 8, 8, 8, 1, 8, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_EXPLOSION_EFFECT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

SPELL("celestial dove",      "dove-patterned", None, None, "Summons a permanent celestial dove",
    P_CELESTIAL_SPELL,            4,   5,             0, 1, 24, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("celestial eagle",      "eagle-patterned", None, None, "Summons a permanent celestial dove",
    P_CELESTIAL_SPELL,            4,   5,             0, 3, 40, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("turn undead",      "copper-plated", None, None, "Makes damages undead creatures and makes them flee",
    P_CELESTIAL_SPELL,            4,   3,             5, 3, 20, A_WIS, 0, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 3, 6, 0, 0, 2, 50, 200, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_CLRC, HI_COPPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("hold undead",    "palm-patterned", None, None, "Paralyzes the target undead creature",
    P_CELESTIAL_SPELL,            4,   3,             0, 4, 30, A_WIS, 18, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 1, 6, 10, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_CLRC, HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("heavenly serpent", "feather-patterned", None, None, "Summons a permanent Aleax",
    P_CELESTIAL_SPELL,            4,   5,             0, 4, 60, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("negate undeath",          "inspiring", None, None, "Destroys the target undead",
    P_CELESTIAL_SPELL,            4,   4,             0, 7,100, A_WIS, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 24, 6, 0, 1, 50, 100, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("guardian angel",      "wing-patterned", None, "ancient celestial prayer", "Summons a permanent angel",
    P_CELESTIAL_SPELL,            4,   5,             0, 6,100, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("banish demon",          "squamous", None, None, "Destroys the target demon",
    P_CELESTIAL_SPELL,            4,   4,             0, 8, 80, A_WIS, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 24, 6, 0, 1, 50, 100, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("holy word",            "antediluvian", None, "ancient celestial prayer", "Stuns, blinds, and kills nearby enemies",
    P_CELESTIAL_SPELL,            4,   7,             0, 8, 80, A_WIS, 0, 5, 100, -10, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 5, 10, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon gold dragon",      "dragon-patterned", None, None, "Summons a permanent gold dragon",
    P_CELESTIAL_SPELL,            4,   5,             0, 8,160, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("obliterate",           "uneven", None, None, "Disintegrates a hostile target",
    P_CELESTIAL_SPELL,            4,  12,             0, 9,100, A_WIS, 1, 0, 100, -10, 1, TOUCH, TOUCH_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_CON, S2_NONE, ALL_TARGETS, AD_DISN, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("divine intervention", "square-patterned", None, "an aeon-old celestial prayer", "The life of the caster will be saved",
    P_CELESTIAL_SPELL,            3,   1,           120,10,300, A_WIS, 0, 0, 100, 0, 1, NODIR, LIFESAVED, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("call Bahamut",  "platinum-plated", None, "an aeon-old celestial prayer", "Calls the Platinum Dragon from its heavenly abode",
    P_CELESTIAL_SPELL,            2,  20,             0,12,600, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("divine mount",      "horn-patterned", None, None, "Summons a permanent ki-rin",
    P_CELESTIAL_SPELL,            3,   5,             0, 6, 80, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon ancient gold dragon",      "oriental", None, "ancient celestial prayer", "Summons a permanent ancient gold dragon",
    P_CELESTIAL_SPELL,            3,   5,             0,10,300, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("heavenly army",      "angelic", None, "an aeon-old celestial prayer", "Summons 2d4 angelic beings for a duration",
    P_CELESTIAL_SPELL,            2,   5,           300,12,300, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 8, 8, 888, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("sunlight beam",   "sun-patterned", None, None, "Causes damage to demons and undead",
    P_CELESTIAL_SPELL,            4,   4,             0, 3, 10, A_WIS, 18, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_FLASHED_LIGHT, 2, 10, 6, 0, 1, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_DEMON | M2_UNDEAD, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_NONE, O5_NONE, O6_NONE),
SPELL("ray of radiance",   "seraphic", None, None, "Causes damage to demons and undead",
    P_CELESTIAL_SPELL,            4,   4,             0, 7, 30, A_WIS, 18, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_FLASHED_LIGHT, 1, 24, 6, 0, 1, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_DEMON | M2_UNDEAD, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of sunlight",    "glimmering", None, None, "Damages undead monsters and demons around you with celestial light",
    P_CELESTIAL_SPELL,            4,   4,             0, 4, 15, A_WIS, 0, 1, 50, 0, 1, NODIR, NODIR_NONE, 2, 12, 6, 0, 1, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_UNDEAD | M2_DEMON, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of radiance",    "empyrean", None, None, "Damages undead monsters and demons around you with celestial light",
    P_CELESTIAL_SPELL,            3,   4,             0, 8, 40, A_WIS, 0, 1, 50, 0, 1, NODIR, NODIR_NONE, 3, 32, 6, 0, 4, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_UNDEAD | M2_DEMON, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("heavenly touch",    "soft", None, None, "Causes the target undead or demon to suffer celestial damage",
    P_CELESTIAL_SPELL,            4,   4,             0, 2, 8, A_WIS, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 2, 12, 6, 0, 1, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_UNDEAD | M2_DEMON, AD_CLRC, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_NONE, O5_NONE, O6_NONE),
SPELL("touch of divinity", "soothing", None, None, "Causes the target undead or demon to suffer celestial damage",
    P_CELESTIAL_SPELL,            3,   4,             0, 6, 25, A_WIS, 1, 0, 20, 0, 1, TOUCH, TOUCH_NONE, 1, 24, 6, 0, 1, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_UNDEAD | M2_DEMON, AD_CLRC, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_NONE, O5_NONE, O6_NONE),
SPELL("celestial storm",      "disquieting", "celestial storm", None, "Conjures a storm of celestial energy upon target",
    P_CELESTIAL_SPELL,            4,   5,             0, 5, 20, A_WIS, 18, 0, 50, 0, 1, RAY, RAY_LIGHTNING, 3, 16, 6, 0, 4, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_UNDEAD | M2_DEMON, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_NONE, O5_NONE, O6_NONE),
SPELL("wrath of god",      "awe-inspiring", "storm of divine wrath", None, "Conjures a great storm of celestial energy upon target",
    P_CELESTIAL_SPELL,            3,   5,             0, 9, 50, A_WIS, 18, 0, 50, 0, 1, RAY, RAY_LIGHTNING, 2, 36, 6, 0, 3, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_EXPLOSION_EFFECT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, M2_UNDEAD | M2_DEMON, AD_CLRC, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_TARGET_PERMISSION_IS_M2_FLAG, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("summon archon",      "blazing-eye-patterned", None, "ancient celestial prayer", "Summons a permanent archon",
    P_CELESTIAL_SPELL,            3,   5,             0, 8,160, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY | S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("power word blind",   "anomalous", None, None, "Blinds the target", //Note monsters have a blindness timer
    P_CELESTIAL_SPELL,            4,   5,           250, 6, 60, A_MAX_WIS_CHA, 8, 0, 5, -8, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 5, 10, 50, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_BLND, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),

SPELL("stick to snake",      "stick-patterned", None, None, "Summons a permanent snake",
    P_NATURE_SPELL,               4,   1,             0, -1, 8, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create wood golem", "wooden", None, None, "Creates a permanent wood golem",
    P_NATURE_SPELL,               4,   4,             0, -1, 8, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("stick to cobra",      "snake-patterned", None, None, "Summons a permanent cobra",
    P_NATURE_SPELL,               4,   1,             0, 1, 24, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("stick to python",      "topaz-studded", None, None, "Summons a permanent python",
    P_NATURE_SPELL,               4,   1,             0, 2, 32, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("stick to boa",      "jade-studded", None, None, "Summons a permanent boa constrictor",
    P_NATURE_SPELL,               4,   1,             0, 6,100, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("barkskin",       "emerald-studded", None, None, "Gives -12 bonus to AC and +4 to MC",
    P_NATURE_SPELL,               4,   3,             0, 4, 15, A_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_BARKSKIN, 0, 0, 0, 0, 2, 10, 130, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("stoneskin",       "diamond-studded", None, None, "Gives -18 bonus to AC and +6 to MC",
    P_NATURE_SPELL,               4,   3,             0, 7, 30, A_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_STONESKIN, 0, 0, 0, 0, 2, 10, 130, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("faithful hound",      "canine-patterned", None, None, "Summons a permanent faithful large dog",
    P_NATURE_SPELL,               4,   3,             0, 2, 32, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon treant",      "tree-patterned", None, None, "Summons a permanent treant",
    P_NATURE_SPELL,               4,   5,             0, 2, 32, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("arrow of Diana",      "bow-patterned", None, None, "Shoots a magical arrow causing massive damage",
    P_NATURE_SPELL,               4,   5,             0, 6, 25, A_WIS, 24, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_ITEM_ARROW, 1, 12, 6, 0, 1, 6, 0, S1_NO_SOMATIC_COMPONENT | S1_LDMG_IS_PER_LEVEL_DMG_INCREASE, S2_NONE, ALL_TARGETS, AD_MAGM, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon elder treant", "leaf-patterned", None, None, "Summons a permanent elder treant",
    P_NATURE_SPELL,               2,   5,             0, 7,120, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("gaze of petrification",   "garnet-studded", None, None, "Petrifies the target instantly",
    P_NATURE_SPELL,               4,   5,             0,11,200, A_MAX_WIS_CHA, 10, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_STON, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("summon phoenix",      "bird-patterned", None, None, "Summons a permanent phoenix",
    P_NATURE_SPELL,               3,   5,             0, 7,120, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("power word stun",   "ludicrous", None, None, "Stuns the target",//Note monsters do not have a stun timer
    P_NATURE_SPELL,               4,   5,           250, 9,100, A_MAX_WIS_CHA,10, 0, 5, -4, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_STUN, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),

SPELL("light",            "cloth", None, None, "Lights up the nearby area",
      P_DIVINATION_SPELL,         4,   1,             0, 0,  2, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_CLOTH, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("probe",    "adamantium-plated", None, None, "Displays monsters' and items' statistics",
      P_DIVINATION_SPELL,         4,   1,             0, -1, 4, A_MAX_INT_WIS_CHA, 12, 0,   5, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_NONE, HI_CLOTH, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("detect monsters",  "leathery", None, None, "The caster can sense all monsters on the level",
      P_DIVINATION_SPELL,         4,   1,             0, 2, 16, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE,  HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("improved monster detection",  "tarnished", None, None, "The caster can sense all monsters on the level for a duration",
      P_DIVINATION_SPELL,         4,   1,             0, 6, 60, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 7, 10, 175, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE,  HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("detect food",     "cyan", None, None, "Detects the location of all comestibles",
      P_DIVINATION_SPELL,         4,   3,             0, -1, 4, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_CYAN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("clairvoyance",    "dark blue", None, None, "Grants clairvoyance",
      P_DIVINATION_SPELL,         4,   3,             0, 2, 16, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("detect unseen",   "violet", None, None, "Enables the caster to see invisible monsters and secret doors",
      P_DIVINATION_SPELL,         4,   4,             0, 3, 24, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 5, 10, 100, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("detect treasure",  "gray", None, None, "The caster senses the location of all objects on the level",
      P_DIVINATION_SPELL,         4,   5,             0, 3, 24, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_GRAY, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("identify",         "bronze-plated", None, None, "Identifies an item",
      P_DIVINATION_SPELL,         4,   1,             0, 3, 24, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_COPPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("magic mapping",    "dusty", None, None, "The caster gains the knowledge of the map of the level",
      P_DIVINATION_SPELL,         4,   7,             0, 6,100, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("telepathy",                "unnameable", None, None, "Grants the caster telepathy",
    P_DIVINATION_SPELL,           4,   3,             0, 5, 40, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, TELEPAT, 0, 0, 0, 0, 6, 10, 150, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("detect traps",            "incongruous", None, None, "The caster gains the knowledge of traps on the level",
    P_DIVINATION_SPELL,           5,   3,             0, 7,120, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("minor consultation",        "damaged", None, None, "The caster receives a minor hint about the game",
    P_DIVINATION_SPELL,           5,   3,             0, -1, 4, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("major consultation",        "scratched", None, None, "The caster receives a major hint about the game",
    P_DIVINATION_SPELL,           5,   3,             0, 4, 30, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("detect blessedness",     "orichalcum-plated", None, None, "Detects whether an item is cursed, uncursed, or blessed",
      P_DIVINATION_SPELL,         5,   1,             0, 1, 12, A_MAX_INT_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_COPPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

SPELL("sleep", "mottled", None, None, "Shoots a sleep ray",
    P_ENCHANTMENT_SPELL,        5, 1,            0, 4, 30, A_MAX_INT_CHA, 12, 0, 100, 0, 1, RAY, RAY_SLEEP, 0, 0, 0, 0, 2, 6, 7, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_SLEE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass sleep", "obtuse", None, None, "Causes nearby monsters fall asleep",
    P_ENCHANTMENT_SPELL,        5, 5,            0, 6, 50, A_MAX_INT_CHA, 0, 6, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 4, 6, 14, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_SLEE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("fear",            "blue-hued", None, None, "Causes monsters to flee",
      P_ENCHANTMENT_SPELL,        5,  3,             0, 3, 20, A_MAX_INT_CHA, 10, 0, 100, 0, 1, IMMEDIATE_MULTIPLE_TARGETS, 0, 0, 0, 0, 0, 1, 25, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BRIGHT_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass fear",      "light blue", None, None, "Causes all monsters on the level to flee",
      P_ENCHANTMENT_SPELL,        5,  3,             0, 5, 40, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 1, 50, 100, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BRIGHT_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("confuse monster", "orange", None, None, "The caster can confuse monsters on hit",
      P_ENCHANTMENT_SPELL,        5,  2,             0, 2, 16, A_MAX_INT_CHA, 16, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, CLR_ORANGE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("charm monster",   "eye-patterned", None, None, "Charms a monster for a duration",
      P_ENCHANTMENT_SPELL,        5,  3,             0, 6, 50, A_MAX_INT_CHA, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 10, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("sphere of charming", "magenta", None, None, "Charms adjacent monsters for a duration",
      P_ENCHANTMENT_SPELL,        5,  5,             0, 8, 80, A_MAX_INT_CHA, 0, 1, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 10, 10, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, CLR_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass charm",         "multi-hued", None, None, "Charms nearby monsters for a duration",
      P_ENCHANTMENT_SPELL,        5,  7,             0, 10, 150, A_MAX_INT_CHA, 10, 2, 100, 0, 1, TARGETED, NODIR_NONE, 0, 0, 0, 0, 10, 10, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, CLR_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("dominate monster",   "eye-and-triangle-patterned", None, None, "Charms a monster permanently",
      P_ENCHANTMENT_SPELL,        4,  6,             0, 7,120, A_MAX_INT_CHA, 8, 0, 100, -4, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_IS_NONREVERSIBLE_PERMANENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("sphere of domination", "pyramid-patterned", None, None, "Charms adjacent monsters permanently",
      P_ENCHANTMENT_SPELL,        3,  8,             0, 9,200, A_MAX_INT_CHA, 0, 1, 100, -4, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_IS_NONREVERSIBLE_PERMANENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("mass domination", "fractal-patterned", None, "ancient enchantment magic", "Charms nearby monsters permanently",
      P_ENCHANTMENT_SPELL,        2, 10,             0,11,400, A_MAX_INT_CHA, 10, 2, 100, -4, 1, TARGETED, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_IS_NONREVERSIBLE_PERMANENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("invisibility",     "dark brown", None, None, "The caster becomes invisible",
      P_ENCHANTMENT_SPELL,        5,  5,             0, 4, 30, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 2, 10, 80, S1_NO_SOMATIC_COMPONENT | S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("enchant armor",          "glowing", None, None, "Increases the enchantment on a piece of armor",
      P_ENCHANTMENT_SPELL,        5, 3,             0, 3, 40, A_MAX_INT_CHA, 0, 0, 300, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("enchant weapon",          "dark gray", None, None, "Increases the enchantment on a weapon",
      P_ENCHANTMENT_SPELL,        5, 3,             0, 4, 60, A_MAX_INT_CHA, 0, 0, 300, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protect armor",          "glowing", None, None, "Makes a piece of armor rust- or burn-proof",
      P_ENCHANTMENT_SPELL,        5, 2,             0, 3, 40, A_MAX_INT_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protect weapon",          "antiquarian", None, None, "Makes a weapon rust- or burn-proof",
      P_ENCHANTMENT_SPELL,        5, 2,             0, 4, 60, A_MAX_INT_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("cold-enchant item",        "baleful", None, None, "Cold-enchants one suitable item",
    P_ENCHANTMENT_SPELL,        5, 1,             0, 5, 80, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("fire-enchant item",        "deformed", None, None, "Fire-enchants one suitable item",
    P_ENCHANTMENT_SPELL,        5, 1,             0, 2, 32, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("lightning-enchant item",    "degraded", None, None, "Lightning-enchants one suitable item",
    P_ENCHANTMENT_SPELL,        5, 1,             0, 3, 40, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("death-enchant item",        "diabolical", None, None, "Death-enchants one suitable item",
    P_ENCHANTMENT_SPELL,        3, 3,             0, 8,160, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("mass conflict",            "odious", None, None, "Causes all monsters to attach each other on the level",
    P_ENCHANTMENT_SPELL,        3, 3,             0, 9,100, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, CONFLICT, 0, 0, 0, 0, 3, 10, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("mirror image",            "nonsensical", None, None, "A displaced mirror image appears beside the caster",
    P_ENCHANTMENT_SPELL,        5, 3,             0, 1, 6, A_MAX_INT_CHA, 0, 0, 100, 0, 1, NODIR, DISPLACED, 0, 0, 0, 0, 6, 10, 180, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

SPELL("cure sickness",   "indigo", None, None, "Cures sickness, terminal illness, and sliming",
      P_HEALING_SPELL,            5,  3,             0, 3, 20, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("cure petrification",   "runic", None, None, "Cures petrification",
      P_HEALING_SPELL,            5,  5,             0, 4, 30, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("cure blindness",  "yellow", None, None, "Cures blindness",
      P_HEALING_SPELL,            5,  2,             0, 0,  8, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_YELLOW, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("minor healing",         "white", None, None, "Heals a target for 2d6 hit points",
      P_HEALING_SPELL,            5,  1,             2, -1, 3, A_WIS, 5, 0, 50, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 2, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("healing",         "white", None, None, "Heals a target for 6d6 hit points",
      P_HEALING_SPELL,            5,  1,             2, 1,  9, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 6, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("extra healing",   "plaid", None, None, "Heals a target for 12d6 hit points",
      P_HEALING_SPELL,            5,  2,             2, 3, 15, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 12, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_GREEN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater healing",  "darkened", None, None, "Heals a target for 24d6 hit points",
      P_HEALING_SPELL,            5,  3,             2, 5, 30, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 24, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_GRAY, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("prodigious healing",  "blackened", None, None, "Heals a target for 36d6 hit points",
      P_HEALING_SPELL,            4,  4,             2, 7, 45, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 36, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("full healing",     "light gray", None, None, "Heals a target to full hit points",
      P_HEALING_SPELL,            3,  5,             2, 9, 150, A_WIS, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 1000, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("restore ability",  "light brown", None, None, "Restores lost ability points",
      P_HEALING_SPELL,            5,  2,             0, 4, 30, A_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

SPELL("slow monster",    "light green", None, None, "Slows down the target",
    P_MOVEMENT_SPELL,            5,  2,             0, 1, 12, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 1, 50, 100, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_SLOW, CLR_BRIGHT_GREEN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass slow",    "olive green", None, None, "Slows down monsters with 15' radius",
    P_MOVEMENT_SPELL,            5,  4,             0, 4, 30, A_MAX_INT_WIS, 12, 3, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 1, 50, 100, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_SLOW, CLR_BRIGHT_GREEN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("hold monster",    "fist-patterned", None, None, "Paralyzes the target",
    P_MOVEMENT_SPELL,            5,  3,             0, 4, 30, A_MAX_INT_WIS, 18, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 1, 6, 10, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_PLYS, HI_LEATHER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass hold",    "cerulean", None, None, "Paralyzes the targets within the area of effect",
    P_MOVEMENT_SPELL,            5,  5,             0, 6, 50, A_MAX_INT_WIS, 18, 4, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 2, 6, 20, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_PLYS, CLR_BRIGHT_BLUE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("haste self",      "purple", None, None, "Increases the movement and attack speed of the caster",
      P_MOVEMENT_SPELL,            5,  4,             0, 3, 20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 1, 10, 100, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("haste monster",    "cinereous", None, None, "Increases the speed of the target",
      P_MOVEMENT_SPELL,            5,  6,             0, 5, 40, A_MAX_INT_WIS, 0, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 3, 10, 300, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_GRAY, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("levitation",      "tan", None, None, "Raises the caster off the ground",
      P_MOVEMENT_SPELL,            5,  4,             0, 2, 16, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 5, 10, 140, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("teleport self",    "jade-studded", None, None, "Teleports the caster to a random location on the level",
      P_MOVEMENT_SPELL,            5,  2,             2, 4, 30, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_GREEN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("teleport monster",    "gold-plated", None, None, "Teleports the target to a random location on the level",
      P_MOVEMENT_SPELL,            5,  4,             2, 5, 40, A_MAX_INT_WIS, 0, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_GOLD, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("controlled teleport",    "byzantine", None, None, "Teleports the caster to the chosen location on the level",
      P_MOVEMENT_SPELL,            5,  5,             2, 6,100, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("circle of teleportation",    "carmine", None, None, "Teleports the monsters around the caster to random locations on the level",
      P_MOVEMENT_SPELL,            5,  5,             2, 6, 50, A_MAX_INT_WIS, 0, 1, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_RED, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("level teleport",    "viridian", None, None, "Teleports the caster to another level",
      P_MOVEMENT_SPELL,            5,  5,             2, 5, 40, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_GREEN, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("controlled level teleport",    "glaucous", None, None, "Teleports the caster to the chosen level",
      P_MOVEMENT_SPELL,            5,  5,             2, 8,160, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_GRAY, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("portal",    "fulvous", None, None, "Teleports the caster to the chosen, previously visited branch of the dungeon",
      P_MOVEMENT_SPELL,            5,  5,             2, 7, 60, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, CLR_ORANGE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("jumping",          "thin", None, None, "Enables the caster to jump",
      P_MOVEMENT_SPELL,            5,  1,             0,-1,  4, A_MAX_INT_WIS, 0, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("water walking",            "circle-patterned", None, None, "Makes the caster able to walk on water",
    P_MOVEMENT_SPELL,            5, 3,             0, 3,20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, WATER_WALKING, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("time stop",           "ancient", None, "an ancient movement spell", "Stops the flow of time, except for the caster",
    P_MOVEMENT_SPELL,            2, 10,            15,10,150, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 1, 4, 3, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),

SPELL("shield",       "obsidian-studded", None, None, "Gives -6 bonus to AC and +2 to MC",
    P_ABJURATION_SPELL,            4,  3,             0, 1, 6, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, MAGICAL_SHIELDING, 0, 0, 0, 0, 2, 10, 130, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("reflection", "polished silver", None, None, "Grants reflection, which protects from rays",
      P_ABJURATION_SPELL,        5,  1,           120, 3, 20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, REFLECTING, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_SILVER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from fire", "smoky", None, None, "Grants immunity to fire attacks",
      P_ABJURATION_SPELL,        5,  1,           120, 2, 16, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, FIRE_IMMUNITY, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from lightning", "cloudy", None, None, "Grants immunity to lightning attacks",
      P_ABJURATION_SPELL,        5,  1,           120, 2, 16, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, SHOCK_IMMUNITY, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from cold", "blue and white", None, None, "Grants immunity to cold attacks",
      P_ABJURATION_SPELL,        5,  1,           120, 2, 16, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, COLD_IMMUNITY, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from acid", "incomplete", None, None, "Grants immunity to acid attacks",
      P_ABJURATION_SPELL,        5,  1,           120, 1, 12, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, ACID_IMMUNITY, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from poison", "intimidating-looking", None, None, "Grants immunity to poison",
    P_ABJURATION_SPELL,            5, 1,           120, 1, 12, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, POISON_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from life draining", "adorned", None, None, "Grants immunity to life draining",
    P_ABJURATION_SPELL,            5, 1,           120, 4, 30, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, DRAIN_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from death magic", "decorated", None, None, "Grants immunity to death magic",
    P_ABJURATION_SPELL,            5, 1,           120, 4, 30, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, DEATH_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from disintegration", "aberrant", None, None, "Grants immunity to disintegration",
    P_ABJURATION_SPELL,            5, 1,           120, 7, 60, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, DISINTEGRATION_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from sickness", "baroque", None, None, "Grants immunity to sickness",
    P_ABJURATION_SPELL,            5, 1,           120, 4, 30, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, SICK_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from petrification", "aged", None, None, "Immunity to petrification attacks",
    P_ABJURATION_SPELL,            5, 1,           120, 5, 40, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, STONE_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("protection from lycanthropy", "strange-looking", None, None, "Grants immunity to lycanthropy",
    P_ABJURATION_SPELL,            5, 1,           120, 0, 8, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, LYCANTHROPY_RESISTANCE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("lower magic resistance", "dim", None, None, "Cuts magic resistance of the target by 25%",
    P_ABJURATION_SPELL,            5, 1,             0, 1, 12, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE | S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("diminish magic resistance", "unclear", None, None, "Cuts magic resistance of the target by 50%",
    P_ABJURATION_SPELL,            5, 2,             0, 3, 20, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE | S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("abolish magic resistance", "obscure", None, None, "Cuts magic resistance of the target by 75%",
    P_ABJURATION_SPELL,            5, 3,             0, 5, 40, A_MAX_INT_WIS, 12, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE | S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("negate magic resistance", "dark", None, None, "Cuts magic resistance of the target to 0%",
    P_ABJURATION_SPELL,            3, 4,             0, 7, 60, A_MAX_INT_WIS, 10, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE | S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("forbid summoning",        "oval-patterned", None, None, "The target cannot use summoning abilities",
    P_ABJURATION_SPELL,            5, 1,             0, 6, 50, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("cancellation",     "shining", None, None, "Prohibits the target from using magic or magical attacks",
    P_ABJURATION_SPELL,            5,  3,             0, 5, 50, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 2, 4, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NOT_CANCELLABLE, O6_NONE),
SPELL("disjunction",     "scintillating", None, "long-forgotten abjuration magic", "Prohibits the target from using magic or magical attacks",
    P_ABJURATION_SPELL,            2,  8,             0, 10, 300, A_MAX_INT_WIS, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 4, 4, 4, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE | S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NOT_CANCELLABLE, O6_NONE),
SPELL("silence",     "tree-patterned", None, None, "Prohibits the target from making noises and casting spells",
    P_ABJURATION_SPELL,            5,  2,             0, 6, 50, A_MAX_INT_WIS, 12, 3, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 1, 20, 30, S1_NO_SOMATIC_COMPONENT | S1_SPELL_BYPASSES_MAGIC_RESISTANCE, S2_NONE, ALL_TARGETS, AD_NONE, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("globe of invulnerability", "striped", None, "ancient abjuration magic", "Makes the caster invulnerable to physical damage",
    P_ABJURATION_SPELL,         4,  1,          60, 9,100, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, INVULNERABLE, 0, 0, 0, 0, 1, 6, 4, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_INDESTRUCTIBLE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("anti-magic shell",   "immaculate", None, None, "Grants resistance to magic",
    P_ABJURATION_SPELL,            4,  1,           120, 3, 20, A_MAX_INT_WIS, 0, 0, 100, 0, 1, NODIR, ANTIMAGIC, 0, 0, 0, 0, 10, 6, 25, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

SPELL("touch of death",   "heavy", None, "ancient necromantic magic", "Kills the target upon touch",
    P_NECROMANCY_SPELL,            5,  8,             0, 8, 80, A_WIS, 1, 0, 100, -10, 1, TOUCH, RAY_DEATH, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_DRAY, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("finger of death",  "leather bound", None, "ancient necromantic magic", "Shoots a death ray",
    P_NECROMANCY_SPELL,            5, 10,             0, 9,100, A_WIS,  8, 0, 100, 0, 1, RAY, RAY_DEATH, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_DRAY, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("deathspell",          "morbid", "death field", "ancient necromantic magic", "Kills the targets in the area of effect",
    P_NECROMANCY_SPELL,            5, 10,             0,10,150, A_WIS, 12, 1, 100, 0, 1, RAY, RAY_DEATH, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_EXPLOSION_EFFECT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_DRAY, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("power word kill",   "preposterous", None, "ancient necromantic magic", "Kills the target",
    P_NECROMANCY_SPELL,            2,  5,           500,12,300, A_WIS, 6, 0, 10, -12, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_DRAY, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("armageddon",       "pitch black", None, "long-forgotten necromantic magic", "Kills all creatures on the level",
    P_NECROMANCY_SPELL,            2, 14,            50,12,600, A_WIS, 0, 255, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_DETH, CLR_BLACK, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("call ghoul",                "repulsive", None, None, "Summons 2d4 ghouls for a duration",
    P_NECROMANCY_SPELL,            5,  5,           180, 3, 20, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 2, 10, 140, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("raise minor zombie",            "unspeakable", None, None, "Raises a zombifiable non-giant corpse into a permanent zombie",
    P_NECROMANCY_SPELL,            5,  5,             0, 0, 8, A_MAX_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create minor mummy",            "terrifying", None, None, "Makes a mummifiable non-giant corpse into a permanent mummy",
    P_NECROMANCY_SPELL,            5,  5,             0, 2, 16, A_MAX_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("raise giant zombie",            "unspeakable", None, None, "Raises a giant or ettin corpse into a permanent zombie",
    P_NECROMANCY_SPELL,            5,  5,             0, 2, 16, A_MAX_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create giant mummy",            "malevolent", None, None, "Makes a giant or ettin corpse into a permanent mummy",
    P_NECROMANCY_SPELL,            5,  5,             0, 5, 40, A_MAX_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("create dracolich",        "sacrilegious", None, "ancient necromantic magic", "Raises a dragon corpse into a permanent dracolich",
    P_NECROMANCY_SPELL,            4,  7,             0, 11,200, A_MAX_WIS_CHA, 5, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("mass raise zombie",        "abnormal", None, None, "Raises zombifiable corpses into permanent zombies",
    P_NECROMANCY_SPELL,            5,  5,             0, 4, 30, A_MAX_WIS_CHA, 0, 8, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass create mummy",        "cadaverous", None, None, "Makes mummifiable corpses into permanent mummies",
    P_NECROMANCY_SPELL,            3,  5,             0, 7,  60, A_MAX_WIS_CHA, 0, 8, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("mass create dracolich",    "insipid", None, "long-forgotten necromantic magic", "Raises dragon corpses into permanent dracoliches",
    P_NECROMANCY_SPELL,            1,  9,             0, 12,300, A_MAX_WIS_CHA, 0, 8, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("raise skeleton",            "loathsome", None, None, "Summons a permanent skeleton",
    P_NECROMANCY_SPELL,            5,  5,             0, 1, 12, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("raise giant skeleton",            "stupendous", None, None, "Summons a permanent giant skeleton",
    P_NECROMANCY_SPELL,            5,  5,             0, 2, 16, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("raise skeleton warrior",            "lunatic", None, None, "Summons a permanent skeleton warrior",
    P_NECROMANCY_SPELL,            4,  5,               0, 3, 20, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("raise skeleton lord",            "octopoid-patterned", None, None, "Summons a permanent skeleton lord",
    P_NECROMANCY_SPELL,            3,  5,             0, 5, 40, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("raise skeleton king",            "skeletal", None, None, "Summons a permanent skeleton king",
    P_NECROMANCY_SPELL,            2,  5,             0, 7, 60, A_MAX_WIS_CHA, 0, 0, 200, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("replenish undeath",         "skull-patterned", None, None, "Heals a target undead for 6d6 hit points",
    P_NECROMANCY_SPELL,            5,  3,             2, 2,  8, A_WIS, 0, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 6, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("greater undeath replenishment",   "bone-patterned", None, None, "Heals a target undead for 12d6 hit points",
    P_NECROMANCY_SPELL,            3,  6,             2, 4, 15, A_WIS, 0, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 12, 6, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT | S1_DOES_NOT_TAKE_A_TURN, S2_NONE, ALL_TARGETS, AD_HEAL, CLR_WHITE, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("summon demon",      "tenebrous", None, None, "Summons a permanent demon",
    P_NECROMANCY_SPELL,            5,  5,             0, 6,100, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("call Demogorgon",  "eldritch", None, "sinister necromantic magic", "Calls the Prince of Demons from the Abyss",
    P_NECROMANCY_SPELL,            2,  9,           999,10,300, A_MAX_WIS_CHA, 0, 0, 100, 0, 1, NODIR, NODIR_NONE, 0, 0, 0, 0, 9, 9, 666, S1_NO_SOMATIC_COMPONENT | S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
SPELL("drain level",      "velvet", None, None, "Drains one experience level from the target",
    P_NECROMANCY_SPELL,            5,  2,             0, 3, 20, A_WIS, 5, 0, 20, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 0, 0, 0, S1_NO_SOMATIC_COMPONENT, S2_NONE, ALL_TARGETS, AD_DRLI, CLR_MAGENTA, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
SPELL("control undead",   "tombstone-patterned", None, None, "Takes the control of target undead creature for a duration",
    P_NECROMANCY_SPELL,            5,  3,             0, 4, 30, A_MAX_WIS_CHA, 8, 0, 100, 0, 1, IMMEDIATE, IMMEDIATE_NONE, 0, 0, 0, 0, 10, 10, 50, S1_NO_SOMATIC_COMPONENT | S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),


/* books with fixed descriptions
 */
SPELL("blank paper", "plain", None, None, None, P_NONE, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, ALL_TARGETS, AD_NONE, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE, O4_NONE, O5_OK_FOR_ILLITERATE, O6_NONE),
/* tribute book for 3.6 */
OBJECT(OBJ("novel", "paperback", None, None, None, 0, 0, 0, 0),
       BITS(0, 0, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, BOOKTYPE_NOVEL, P_NONE, MAT_PAPER),
       0, 0, 0, P1_NONE, SPBOOK_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 20,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       20, CLR_BRIGHT_BLUE, OBJECT_SOUNDSET_SPELLBOOK, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE, O4_NONE, O5_OK_FOR_ILLITERATE, O6_NONE),
OBJECT(OBJ("manual", "hardcover", None, None, None, 0, 0, 0, 0),
       BITS(1, 0, 1, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, BOOKTYPE_MANUAL, P_NONE, MAT_PAPER),
       0, 0, 0, P1_NONE, SPBOOK_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 10,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       20, CLR_BROWN, OBJECT_SOUNDSET_SPELLBOOK, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE, O4_NONE, O5_OK_FOR_ILLITERATE, O6_NONE),
/* Base item for The Prime Codex */
OBJECT(OBJ("Book of Modron", "ornamental white", None, None, None, 0, 0, 0, 0),
       BITS(0, 0, 0, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 1, 1, 0, 0, 0, BOOKTYPE_CODEX, P_NONE, MAT_PAPER),
       0, 0, 0, P1_NONE, SPBOOK_CLASS, 0, MULTIGEN_SINGLE, 3, 20, 5000,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       20, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE | O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE | O3_NO_WISH | O3_NO_GENERATION, O4_NONE, O5_FULL_NAME | O5_OK_FOR_ILLITERATE | O5_NOT_CANCELLABLE, O6_NONE),
/* a special, one of a kind, spellbook */
OBJECT(OBJ("Book of the Dead", "papyrus", None, None, None, 0, 0, 0, 0),
       BITS(0, 0, 1, 0, 1, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 1, 1, 0, 0, 0, BOOKTYPE_GRIMOIRE, P_NONE, MAT_PAPER),
       0, 0, 0, P1_NONE, SPBOOK_CLASS, 0, MULTIGEN_SINGLE, 0, 20, 10000,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, S2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0,
       20, HI_PAPER, OBJECT_SOUNDSET_SPELLBOOK, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_INDESTRUCTIBLE | O1_NON_SPELL_SPELLBOOK, O2_NONE, O3_READABLE, O4_NONE, O5_FULL_NAME | O5_OK_FOR_ILLITERATE | O5_NOT_CANCELLABLE, O6_NONE),
#undef SPELL

/* wands ... */
#define WAND(name,typ,effectdesc,contentdesc,itemdesc,charged,recharging,prob,cost,mgc,dir,dirsubtype,range,radius,sdice,sdam,sdmgplus,ldice,ldam,ldmgplus,sflags,metal,color,soundset,flags,flags2,flags3,flags4,flags5,flags6) \
    OBJECT(OBJ(name, typ, effectdesc, contentdesc, itemdesc, 24, 0, 0, 0),                                             \
           BITS(0, 0, 1, 0, mgc, ENCHTYPE_NO_ENCHANTMENT, charged, recharging, 0, 0, 0, 0, dir, 0, P_NONE, metal),    \
           0, 0, 0, P1_NONE, WAND_CLASS, prob, MULTIGEN_SINGLE, 0, 6, cost, \
           AD_NONE, sdice,sdam,sdmgplus,ldice,ldam,ldmgplus, 0, 0, 0, 0, sflags, S2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, range, radius, 0, 0, \
           30, color, soundset, \
           dirsubtype, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
WAND("disjunction", "plutonium", None, None, "Prohibits the target from using magic or magical attacks.", CHARGED_ALWAYS_5, RECHARGING_WAND_GENERAL, 0, 1000, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 4, 4, 4, S1_SPELL_BYPASSES_MAGIC_RESISTANCE | S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE, MAT_METAL, CLR_BLACK, OBJECT_SOUNDSET_WAND, O1_LIGHTNING_RESISTANT | O1_POLYMORPH_RESISTANT | O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NO_WISH | O3_NO_GENERATION, O4_NONE, O5_MBAG_DESTROYING_ITEM | O5_NOT_CANCELLABLE, O6_NONE), /* Base item for Rod of Disjunction */
/* Shuffled wands start here */
WAND("light",           "glass", None, None, None, CHARGED_WAND_NORMAL_NODIR, RECHARGING_WAND_GENERAL, 40, 100, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_GLASS, HI_GLASS, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE), /* Start marker for shuffled wands */
WAND("secret door detection",
                        "balsa", None, None, None, CHARGED_WAND_NORMAL_NODIR, RECHARGING_WAND_GENERAL, 30, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("ore detection", 
                   "orichalcum", None, None, None, CHARGED_1D4_4, RECHARGING_WAND_GENERAL, 30, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_ORICHALCUM, HI_GOLD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("trap detection",
                     "mahogany", None, None, None, CHARGED_1D4_4, RECHARGING_WAND_GENERAL, 30, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("town portal", "blackwood", None, None, "Teleports to the Mine Town and back.\nFails with nearby hostile creatures and recent combat damage.", CHARGED_WAND_NORMAL_NODIR, RECHARGING_WAND_GENERAL, 25, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, CLR_BLACK, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("enlightenment", "crystal", None, None, None, CHARGED_WAND_NORMAL_NODIR, RECHARGING_WAND_GENERAL, 25, 150, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_GLASS, HI_GLASS, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("create monster",  "maple", None, None, None, CHARGED_WAND_NORMAL_NODIR, RECHARGING_WAND_GENERAL, 40, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("wishing",          "pine", None, None, None, CHARGED_WAND_WISHING, RECHARGING_WAND_WISHING, 5, 500, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_VERY_RARE, O5_NONE, O6_NONE),
WAND("identify",      "mithril", None, None, None, CHARGED_WAND_NORMAL_NODIR, RECHARGING_WAND_GENERAL, 20, 200, 1, NODIR, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("nothing",           "oak", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 25, 100, 0, IMMEDIATE, 0,  0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("striking",        "ebony", None, None, None, CHARGED_1D45_45, RECHARGING_WAND_GENERAL, 45, 150, 1, IMMEDIATE, 0, 10, 0, 3, 6, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("make invisible", "marble", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 45, 150, 1, IMMEDIATE, 0,  8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_MINERAL, HI_MINERAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("slow monster",      "tin", None, None, None, CHARGED_2D6_8, RECHARGING_WAND_GENERAL, 40, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("speed monster",   "brass", None, None, None, CHARGED_2D6_8, RECHARGING_WAND_GENERAL, 40, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_COPPER, HI_COPPER, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("undead turning", "copper", None, None, None, CHARGED_2D6_8, RECHARGING_WAND_GENERAL, 40, 150, 1, IMMEDIATE, 0,  8, 0, 3, 6, 0, 1, 50, 200, S1_NONE, MAT_COPPER, HI_COPPER, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("polymorph",      "silver", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 30, 200, 1, IMMEDIATE_ONE_TO_SEVEN_TARGETS, 0,  6, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_SILVER, HI_SILVER, OBJECT_SOUNDSET_WAND, O1_POLYMORPH_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("cancellation", "platinum", None, None, "Prohibits the target from using magic or magical attacks.", CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 35, 200, 1, IMMEDIATE_MULTIPLE_TARGETS, 0, 10, 0, 0, 0, 0, 2, 4, 0, S1_SPELL_BYPASSES_MAGIC_RESISTANCE, MAT_PLATINUM, CLR_WHITE, OBJECT_SOUNDSET_WAND, O1_LIGHTNING_RESISTANT | O1_POLYMORPH_RESISTANT | O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_MBAG_DESTROYING_ITEM | O5_NOT_CANCELLABLE, O6_NONE),
WAND("teleportation", "iridium", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 40, 200, 1, IMMEDIATE, 0, 10, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, CLR_BRIGHT_CYAN, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("opening",          "zinc", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 25, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("locking",      "aluminum", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 25, 150, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("probing",       "uranium", None, None, None, CHARGED_1D20_20, RECHARGING_WAND_GENERAL, 40, 200, 1, IMMEDIATE, 0, 12, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_METAL, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("digging",          "iron", None, None, None, CHARGED_2D6_8, RECHARGING_WAND_GENERAL, 40, 150, 1, RAY, RAY_WND_DIGGING,            10, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("magic missile",   "steel", None, None, None, CHARGED_1D75_75, RECHARGING_WAND_GENERAL, 45, 150, 1, RAY, RAY_WND_MAGIC_MISSILE,    12, 0, 3, 5, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("fire",        "hexagonal", None, None, None, CHARGED_1D30_30, RECHARGING_WAND_GENERAL, 40, 175, 1, RAY, RAY_WND_FIRE,                 8, 0, 4, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_FIRE_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("cold",            "short", None, None, None, CHARGED_1D6_6, RECHARGING_WAND_GENERAL, 40, 175, 1, RAY, RAY_WND_COLD,                12, 0, 8, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_COLD_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("sleep",            "runed", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 45, 175, 1, RAY, RAY_WND_SLEEP,            10, 0, 0, 0, 0, 4, 6, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("disintegration",   "long", None, None, None, CHARGED_1D4_1, RECHARGING_WAND_GENERAL, 10, 500, 1, RAY, RAY_WND_DISINTEGRATION,     8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_DISINTEGRATION_RESISTANT, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE),
WAND("petrification", "adamantium", None, None, None, CHARGED_1D4_1, RECHARGING_WAND_GENERAL, 10, 500, 1, RAY, RAY_WND_PETRIFICATION, 8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE),
WAND("lightning",      "curved", None, None, None, CHARGED_1D15_15, RECHARGING_WAND_GENERAL, 40, 175, 1, RAY, RAY_WND_LIGHTNING,        14, 0, 5, 6, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_LIGHTNING_RESISTANT, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("death",     "skull-headed", None, None, None, CHARGED_1D4_1, RECHARGING_WAND_GENERAL, 10, 500, 1, RAY, RAY_WND_DEATH,             8, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_BONE, CLR_WHITE, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_RARE, O5_NONE, O6_NONE),
WAND("resurrection", 
                   "ornamental", None, None, None, CHARGED_1D4_4, RECHARGING_WAND_GENERAL, 20, 200, 1, IMMEDIATE, 0,                     6, 0, 0, 0, 0, 0, 0, 0, S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND("evaporation",    "flimsy", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 25, 150, 1, RAY, RAY_WND_EVAPORATION,        10, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
/* extra descriptions, shuffled into use at start of new game */
WAND(None,             "forked", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_WOOD, HI_WOOD, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND(None,             "spiked", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_METAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
WAND(None,            "jeweled", None, None, None, CHARGED_WAND_NORMAL_DIR, RECHARGING_WAND_GENERAL, 0, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, S1_NONE, MAT_IRON, HI_MINERAL, OBJECT_SOUNDSET_WAND, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
/* Shuffled wands end here */
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal,worth) \
    OBJECT(OBJ(name, None, None, None, None, 0, 0, 0, COIN_REPLACEMENT),                                       \
           BITS(0, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, metal),    \
           0, 0, 0, P1_NONE, COIN_CLASS, prob, MULTIGEN_SINGLE, 0, 1, worth, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           0, HI_GOLD, OBJECT_SOUNDSET_COIN, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_READABLE, O4_NONE, O5_NONE, O6_NONE)
COIN("gold piece", 1000, MAT_GOLD, 1),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask) \
    OBJECT(OBJ(name, desc, None, None, None, height, 0, 0, 0),                                             \
           BITS(0, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0,                              \
                HARDGEM(mohs), 0, WEP_SLING_BULLET, -P_SLING, glass),                     \
           0, 0, 0, P1_NONE, GEM_CLASS, prob, MULTIGEN_SINGLE, 0, 1, gval, \
           0, 1, 3, 0, 1, 3, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           nutr, color, soundset, \
           WEP_SLING, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
#define ROCK(name,desc,power1,power2,power3,pflags,kn,prob,wt,gval, enchtype, sdice, sdam, splus, ldice, ldam, lplus, mgc,nutr,mohs,glass,color,height,soundset,flags,flags2,flags3,flags4,flags5,flags6,powconfermask,stand_anim,enlarge,replacement) \
    OBJECT(OBJ(name, desc, None, None, None, height, stand_anim, enlarge, replacement),                                            \
           BITS(kn, 1, (enchtype > 0 ? 1 : 0), 0, mgc, enchtype, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0,                           \
                HARDGEM(mohs), 0, WEP_SLING_BULLET, -P_SLING, glass), \
           power1, power2, power3, pflags, GEM_CLASS, prob, MULTIGEN_SINGLE, 0, wt, gval, \
           0, sdice, sdam, splus, ldice, ldam, lplus, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
           nutr, color, soundset, \
           WEP_SLING, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)
GEM("dilithium crystal", "white",  2, 1, 2250, 15,  5, MAT_GEMSTONE, CLR_WHITE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("diamond",           "white",  3, 1, 2000, 15,  10, MAT_GEMSTONE, CLR_WHITE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("black pearl",         "black",  3, 1, 1750, 15,  5, MAT_GEMSTONE, CLR_BLACK, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("ruby",                "red",  4, 1, 1500, 15,  9, MAT_GEMSTONE, CLR_RED, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("jacinth",          "orange",  3, 1, 1400, 15,  9, MAT_GEMSTONE, CLR_ORANGE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("sapphire",           "blue",  4, 1, 1300, 15,  9, MAT_GEMSTONE, CLR_BLUE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("black opal",        "black",  3, 1, 1200, 15,  8, MAT_GEMSTONE, CLR_BLACK, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("emerald",           "green",  5, 1, 1100, 15,  8, MAT_GEMSTONE, CLR_GREEN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("turquoise",         "green",  6, 1, 1000, 15,  6, MAT_GEMSTONE, CLR_GREEN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("citrine",          "yellow",  4, 1,  950, 15,  6, MAT_GEMSTONE, CLR_YELLOW, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("aquamarine",        "green",  6, 1,  900, 15,  8, MAT_GEMSTONE, CLR_GREEN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("amber",   "yellowish brown",  8, 1,  850, 15,  2, MAT_GEMSTONE, CLR_BROWN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("topaz",   "yellowish brown", 10, 1,  800, 15,  8, MAT_GEMSTONE, CLR_BROWN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("jet",               "black", 10, 1,  750, 15,  7, MAT_GEMSTONE, CLR_BLACK, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("opal",              "white", 10, 1,  700, 15,  6, MAT_GEMSTONE, CLR_WHITE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("chrysoberyl",      "yellow", 10, 1,  650, 15,  5, MAT_GEMSTONE, CLR_YELLOW, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("garnet",              "red", 10, 1,  600, 15,  7, MAT_GEMSTONE, CLR_RED, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("amethyst",         "violet", 10, 1,  550, 15,  7, MAT_GEMSTONE, CLR_MAGENTA, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("jasper",              "red", 10, 1,  500, 15,  7, MAT_GEMSTONE, CLR_RED, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("fluorite",         "violet", 10, 1,  400, 15,  4, MAT_GEMSTONE, CLR_MAGENTA, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("pearl",             "white", 10, 1,  300, 15,  5, MAT_GEMSTONE, CLR_WHITE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("obsidian",          "black", 10, 1,  200, 15,  6, MAT_GEMSTONE, CLR_BLACK, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("agate",            "orange", 10, 1,  200, 15,  6, MAT_GEMSTONE, CLR_ORANGE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("jade",              "green", 10, 1,  300, 15,  6, MAT_GEMSTONE, CLR_GREEN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of white glass", "white",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_WHITE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of blue glass", "blue",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_BLUE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of red glass", "red",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_RED, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of yellowish brown glass", "yellowish brown",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_BROWN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of orange glass", "orange",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_ORANGE, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of yellow glass", "yellow",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_YELLOW, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of black glass", "black",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_BLACK, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of green glass", "green",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_GREEN, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
GEM("worthless piece of violet glass", "violet",
    70, 1, 0, 6, 5, MAT_GLASS, CLR_MAGENTA, 16, OBJECT_SOUNDSET_GEM, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

/* Placement note: there is a wishable subrange for
 * "gray stones" in the o_ranges[] array in objnam.c
 * that is currently everything between luckstones and flint
 * (inclusive).
 */
ROCK("luckstone", "gray",    //Gray stone start marker, also must immediately follow pieces of glass for tiledata processing
    NO_POWER, NO_POWER, NO_POWER, P1_CONFERS_LUCK | P1_LUCK_APPLIES_WHEN_CARRIED | P1_CURSED_ITEM_YIELDS_NEGATIVE,
    0,  10,  10, 60, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 1, 10, 7, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY, 
    O2_GRAYSTONE, 
    O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("loadstone", "gray", 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    0,  10, 500,  1, ENCHTYPE_NO_ENCHANTMENT, 1, 10, 0, 1, 10, 0, 1, 10, 6, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_STONE,
    O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED | O1_CANNOT_BE_DROPPED_IF_CURSED | O1_THROWN_WEAPON_ONLY,
    O2_GRAYSTONE | O2_CURSED_MAGIC_ITEM | O2_GENERATED_CURSED, 
    O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("jinxstone", "gray",  
    FUMBLING, NO_POWER, NO_POWER, P1_POWER_1_APPLIES_WHEN_CARRIED | P1_CONFERS_UNLUCK | P1_LUCK_APPLIES_WHEN_CARRIED,
    0,  10,  10,  1, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 1, 10, 6, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_STONE,
    O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED | O1_CANNOT_BE_DROPPED_IF_CURSED | O1_THROWN_WEAPON_ONLY,
    O2_GRAYSTONE | O2_CURSED_MAGIC_ITEM | O2_GENERATED_CURSED, 
    O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("prayerstone", "gray",
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    0,  10,  10, 60, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 1, 10, 7, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY, 
    O2_GRAYSTONE | O2_SPECIAL_PRAYING_ITEM,
    O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("touchstone", "gray", 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    0, 8, 10, 45, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 1, 10, 6, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY, 
    O2_GRAYSTONE, 
    O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("flint", "gray",      //Gray stone end marker 
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    0, 8, 5, 1, ENCHTYPE_PROJECTILE_ALWAYS_START_0, 1, 8, 0, 1, 8, 0, 0, 10, 7, MAT_MINERAL, CLR_GRAY, 24, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY,
    O2_GRAYSTONE, 
    O3_ELEMENTAL_ENCHANTABLE, 
    O4_NONE, O5_NONE, O6_NONE,
    PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("rock", None,         
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    1, 26, 10,  0, ENCHTYPE_PROJECTILE_ALWAYS_START_0, 1, 6, 0, 1, 6, 0, 0, 10, 7, MAT_MINERAL, CLR_GRAY, 18, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY, 
    O2_ROCK, 
    O3_ELEMENTAL_ENCHANTABLE,
    O4_FLOOR_TILE, O5_NONE, O6_NONE,
    PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("stone pebble", None,         
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    1, 40, 2, 0, ENCHTYPE_PROJECTILE_ALWAYS_START_0, 1, 4, 0, 1, 4, 0, 0, 10, 7, MAT_MINERAL, CLR_GRAY, 0, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY, 
    O2_ROCK,
    O3_ELEMENTAL_ENCHANTABLE,
    O4_NONE, O5_NONE, O6_NONE,
    PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, STONE_PEBBLE_REPLACEMENT),
ROCK("clay pebble", None,         
    NO_POWER, NO_POWER, NO_POWER, P1_NONE,
    1,  40, 2, 0, ENCHTYPE_PROJECTILE_ALWAYS_START_0, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_MINERAL, CLR_BROWN, 0, OBJECT_SOUNDSET_STONE,
    O1_THROWN_WEAPON_ONLY, 
    O2_ROCK, 
    O3_ELEMENTAL_ENCHANTABLE, 
    O4_NONE, O5_NONE, O6_NONE,
    PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, CLAY_PEBBLE_REPLACEMENT),
ROCK("nugget of iron ore", None,         //Ore startmarker
    0, 0, 0, P1_NONE, 1,  10,  16,    50, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_IRON, HI_METAL, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of copper ore", None,
    0, 0, 0, P1_NONE, 1,   9,  16,   100, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_COPPER, HI_GOLD, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of silver ore", None,
    0, 0, 0, P1_NONE, 1,   5,  16,   150, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_SILVER, HI_SILVER, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of gold ore", None,
    0, 0, 0, P1_NONE, 1,   4,  16,   200, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_GOLD, HI_GOLD, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of platinum ore", None,
    0, 0, 0, P1_NONE, 1,   3,  16,   250, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_PLATINUM, HI_SILVER, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of orichalcum ore", None,
    0, 0, 0, P1_NONE, 1,   2,  16,   400, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_ORICHALCUM, HI_SILVER, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of adamantium ore", None,
    0, 0, 0, P1_NONE, 1,   2,  16,   450, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_ADAMANTIUM, HI_SILVER, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),
ROCK("nugget of mithril ore", None,        //Ore endmarker
    0, 0, 0, P1_NONE, 1,   2,  16,   350, ENCHTYPE_NO_ENCHANTMENT, 1, 3, 0, 1, 3, 0, 0, 10, 7, MAT_MITHRIL, HI_SILVER, 32, OBJECT_SOUNDSET_STONE, O1_NONE, O2_ORE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL,
    NO_ANIMATION, NO_ENLARGEMENT, NO_REPLACEMENT),


#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
OBJECT(OBJ("boulder", None, None, None, None, 0, 0, 0, 0),
       BITS(1, 0, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 1, 0, 0, 0, P_NONE, MAT_MINERAL),
       0, 0, 0, P1_NONE, ROCK_CLASS, 100, MULTIGEN_SINGLE, 0, 8000, 0,
       AD_PHYS, 2, 6, 8, 2, 6, 8, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       2000, HI_MINERAL, OBJECT_SOUNDSET_BOULDER, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_FULL_SIZED_BITMAP | O4_DRAWN_IN_FRONT, O5_TILE_IS_TILESET_DEPENDENT, O6_NONE),
OBJECT(OBJ("statue", None, None, None, None, 0, 0, 0, 0),
       BITS(1, 0, 0, 1, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, MAT_MINERAL),
       0, 0, 0, P1_NONE, ROCK_CLASS, 900, MULTIGEN_SINGLE, 0, 4000, 0,
       AD_PHYS, 1, 20, 0, 1, 20, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       2500, CLR_WHITE, OBJECT_SOUNDSET_BOULDER, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_FULL_SIZED_BITMAP, O5_NONE, O6_NONE),
OBJECT(OBJ("heavy iron ball", None, None, None, None, 0, NO_ANIMATION, NO_ENLARGEMENT, BALL_REPLACEMENT),
       BITS(1, 0, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, WHACK, 0, P_NONE, MAT_IRON),
       0, 0, 0, P1_NONE, BALL_CLASS, 1000, MULTIGEN_SINGLE, 0, 800, 10,
       AD_PHYS, 1, 25, 0, 1, 25, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      200, HI_METAL, OBJECT_SOUNDSET_GENERIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_THROWN_WEAPON_ONLY, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),
OBJECT(OBJ("iron chain", None, None, None, None, 0, NO_ANIMATION, NO_ENLARGEMENT, CHAIN_REPLACEMENT),
       BITS(1, 0, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, WHACK, 0, P_NONE, MAT_IRON),
       0, 0, 0, P1_NONE, CHAIN_CLASS, 1000, MULTIGEN_SINGLE, 0, 240, 0,
       AD_PHYS, 1, 4, 0, 1, 4, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       200, HI_METAL, OBJECT_SOUNDSET_GENERIC, 0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE),

/* Venom is normally a transitory missile (spit by various creatures)
 * but can be wished for in wizard mode so could occur in bones data.
 */
OBJECT(OBJ("blinding venom", "splash of blinding venom", None, None, None, 0, 0, 0, 0),
       BITS(0, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 1, 0, 0, 0, 0, P_NONE, MAT_LIQUID),
       0, 0, 0, P1_NONE, VENOM_CLASS, 500, MULTIGEN_SINGLE, 0, 1, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, HI_ORGANIC, OBJECT_SOUNDSET_GENERIC,
       0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE, O5_NONE, O6_NONE),
OBJECT(OBJ("acid venom", "splash of acid venom", None, None, None, 0, 0, 0, 0),
       BITS(0, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 1, 0, 0, 0, 0, P_NONE, MAT_LIQUID),
       0, 0, 0, P1_NONE, VENOM_CLASS, 500, MULTIGEN_SINGLE, 0, 1, 0,
       AD_PHYS, 2, 6, 0, 2, 6, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, HI_ORGANIC, OBJECT_SOUNDSET_GENERIC,
       0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_MISSILE_TILE, O5_NONE, O6_NONE),
        /* +d6 small or large */

/* Reagents here, which do not belong to any class, includes also all other odd non-food objects with no other apparent purpose*/
#define REAGENT(name, desc, known, prob, multigen, eatdelay, wt, cost, material, ediblesubtype, edibleeffect, ediblechance, nutrition, color, height,soundset, flags, flags2, flags3, flags4, flags5, flags6, powconfermask)         \
    OBJECT(OBJ(name, desc, None, None, None, height, 0, 0, 0),                                      \
           BITS(known, 1, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, material), \
           0, 0, 0, P1_NONE, REAGENT_CLASS, prob, multigen, eatdelay, wt, cost, \
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, ediblechance, \
           0, 0, 0, 0, ediblesubtype, edibleeffect, 0, 0, 0, 0, 0, 0, \
           nutrition, color, soundset, \
           0, 0, 0, 0, powconfermask, ALL_TARGETS, flags, flags2, flags3, flags4, flags5, flags6)

    REAGENT("thread of spider silk", None, 1,    50, MULTIGEN_1D3, 0, 1, 10, MAT_SILK, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 2, CLR_GRAY, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* STARTMARKER 1&2 */

    REAGENT("clump of bat guano", None, 1,    100, MULTIGEN_1D3, 2, 2,  10, MAT_ORGANIC, EDIBLETYPE_ROTTEN, EDIBLEFX_NO_EFFECT, 0, 20, CLR_BROWN, 16, OBJECT_SOUNDSET_GENERIC, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("ginseng root", None, 1, 150, MULTIGEN_1D3, 1, 3,  10, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_BROWN, 24, OBJECT_SOUNDSET_GENERIC, O1_EDIBLE_NONFOOD, O2_NONE, O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("mandrake root", None, 1, 50, MULTIGEN_1D2, 1, 3,  10, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_MAGENTA, 24, OBJECT_SOUNDSET_GENERIC, O1_EDIBLE_NONFOOD, O2_NONE, O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("heap of sporal powder", None, 1, 50, MULTIGEN_1D2, 1, 1,  10, MAT_ORGANIC, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 1, CLR_GRAY, 24, OBJECT_SOUNDSET_GENERIC, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("sprig of wolfsbane", None, 1, 100, MULTIGEN_1D2, 1, 1,  10, MAT_VEGGY, EDIBLETYPE_POISONOUS, EDIBLEFX_CURE_LYCANTHROPY, 100, 40, CLR_GREEN, 24, OBJECT_SOUNDSET_GENERIC, O1_EDIBLE_NONFOOD, O2_NONE, O3_NONE, O4_FLOOR_TILE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("clove of garlic", None, 1, 100, MULTIGEN_1D3, 1, 1,  10, MAT_VEGGY, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_WHITE, 24, OBJECT_SOUNDSET_GENERIC, O1_EDIBLE_NONFOOD, O2_NONE, O3_BURIED_SEARCHABLE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("bone", None, 1, 100, MULTIGEN_1D3, 1, 16, 10, MAT_BONE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_WHITE, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("human skull", None, 1, 50, MULTIGEN_SINGLE, 1, 32, 10, MAT_BONE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_WHITE, 32, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("piece of wood", None, 1, 100, MULTIGEN_1D3, 1, 16, 10, MAT_WOOD, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_BROWN, 48, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("pinch of sulfurous ash", None, 1, 100, MULTIGEN_1D3, 1, 1,  10, MAT_MINERAL, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 40, CLR_BLACK, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("feather", None, 1, 50, MULTIGEN_SINGLE, 0, 1, 10, MAT_ORGANIC, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 10, CLR_BLACK, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL), /* ENDMARKER 1&2 */

    REAGENT("modronite sphere", "alien ball-shaped object",                        0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("modronite monopole", "alien pole-shaped object",                    0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("modronite tetrahedron", "alien pyramidal object",                    0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("modronite cube", "alien cubic object",                                0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("modronite octahedron", "alien eight-faced polyhedral object",        0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("modronite dodecahedron", "alien twelve-faced polyhedral object",    0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),
    REAGENT("modronite icosahedron", "alien twenty-faced polyhedral object",    0, 0, MULTIGEN_SINGLE, 1, 16, 100, MAT_MODRONITE, EDIBLETYPE_NORMAL, EDIBLEFX_NO_EFFECT, 0, 160, CLR_CYAN, 24, OBJECT_SOUNDSET_GENERIC, O1_NONE, O2_MODRON_ITEM, O3_NONE, O4_NONE, O5_NONE, O6_NONE, PERMITTED_ALL),

#undef REAGENT

/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
OBJECT(OBJ(None, None, None, None, None, 0, 0, 0, 0),
       BITS(0, 0, 0, 0, 0, ENCHTYPE_NO_ENCHANTMENT, CHARGED_NOT_CHARGED, RECHARGING_NOT_RECHARGEABLE, 0, 0, 0, 0, 0, 0, P_NONE, 0),
       0, 0, 0, P1_NONE, ILLOBJ_CLASS, 0, MULTIGEN_SINGLE, 0, 0, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, A1_NONE, A2_NONE, 0, \
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
       0, 0, OBJECT_SOUNDSET_NONE, \
       0, 0, 0, 0, PERMITTED_ALL, ALL_TARGETS, O1_NONE, O2_NONE, O3_NONE, O4_NONE, O5_NONE, O6_NONE)
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

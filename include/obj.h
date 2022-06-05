/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    obj.h    $NHDT-Date: 1508827590 2017/10/24 06:46:30 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.60 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "general.h"

#ifndef OBJ_H
#define OBJ_H

/* #define obj obj_nh */ /* uncomment for SCO UNIX, which has a conflicting
                          * typedef for "obj" in <sys/types.h> */

union vptrs {
    struct obj *v_nexthere;   /* floor location lists */
    struct obj *v_ocontainer; /* point back to container */
    struct monst *v_ocarry;   /* point back to carrying monst */
};

/****
 ***    oextra -- collection of all object extensions
 **    (see the note at the bottom of this file before adding oextra fields)
 */
struct oextra {
    char *oname;          /* ptr to name of object */
    char* uoname;          /* ptr to name of object */
    struct monst *omonst; /* ptr to attached monst struct */
    unsigned *omid;       /* ptr to m_id */
    long *olong;          /* ptr to misc long (temporary gold object) */
    char *omailcmd;       /* response_cmd for mail deliver */
};

struct obj {
    struct obj *nobj;
    union vptrs v;
#define nexthere v.v_nexthere
#define ocontainer v.v_ocontainer
#define ocarry v.v_ocarry

    struct obj *cobj; /* contents list for containers */
    unsigned o_id;
    xchar ox, oy;
    xchar ox0, oy0;
    short otyp; /* object class number */
    unsigned owt;
    long quan; /* number of items */

    short enchantment; /* Always set to zero by cancellation
                  quality of weapon, weptool, armor or ring (+ or -);
                  OBSOLETE (moved to charges): number of charges for wand or charged tool ( >= -1 );
                  OBSOLETE (moved to special_quality): number of candles attached to candelabrum;
                  OBSOLETE (moved to special_quality and speflags): marks your eggs, tin variety and spinach tins;
                  OBSOLETE (moved to special_quality and speflags): Schroedinger's Box (1) or royal coffers for a court (2);
                  OBSOLETE (moved to special_quality): tells which fruit a fruit is;
                  OBSOLETE (moved to special_quality): special for uball and amulet;
                  OBSOLETE (moved to special_quality): scroll of mail (normal==0, bones or wishing==1, written==2);
                  OBSOLETE (moved to speflags): historic and gender for statues */
    short charges; /* number of charges for wand or charged tool ( >= -1 ), always set to -1/0 by cancellation */
    short special_quality; /* item-specific special quality, e.g., the amount of wetness of a towel, number of candles attached to candelabrum, not affected by cancellation */
    unsigned long speflags; /* anything else that might be going on with an item, not affected by cancellation */

#define SPEFLAGS_YOURS                         0x00000001UL
#define SPEFLAGS_FEMALE                        0x00000002UL
#define SPEFLAGS_MALE                          0x00000004UL
#define SPEFLAGS_FACING_RIGHT                  0x00000008UL
#define SPEFLAGS_SCHROEDINGERS_BOX             0x00000010UL
#define SPEFLAGS_STATUE_HISTORIC               0x00000020UL
#define SPEFLAGS_CORPSE_ON_ICE                 0x00000040UL
#define SPEFLAGS_DETECTED                      0x00000080UL
#define SPEFLAGS_SERVICED_SPELL                0x00000100UL
#define SPEFLAGS_MINES_PRIZE                   0x00000200UL
#define SPEFLAGS_SOKO_PRIZE1                   0x00000400UL
#define SPEFLAGS_SOKO_PRIZE2                   0x00000800UL
#define SPEFLAGS_FROM_SINK                     0x00001000UL
#define SPEFLAGS_INDESTRUCTIBLE                0x00002000UL
#define SPEFLAGS_USES_UP_KEY                   0x00004000UL
#define SPEFLAGS_NO_PICKUP                     0x00008000UL  /* Monsters will not pick up this item */
#define SPEFLAGS_ROYAL_COFFER                  0x00010000UL
#define SPEFLAGS_LID_OPENED                    0x00020000UL
#define SPEFLAGS_GRABBED_FROM_YOU              0x00040000UL
#define SPEFLAGS_ADDED_TO_YOUR_BILL            0x00080000UL
#define SPEFLAGS_WILL_TURN_TO_DUST_ON_PICKUP   0x00100000UL
#define SPEFLAGS_CAUGHT_IN_LEAVES              0x00200000UL
#define SPEFLAGS_CLONED_ITEM                   0x00400000UL
#define SPEFLAGS_INSCRIPTION_REVEALED          0x00800000UL
#define SPEFLAGS_ALTERNATIVE_APPEARANCE        0x01000000UL /* Alternative glyph is used for the object */
#define SPEFLAGS_ROTTING_STATUS_KNOWN          0x02000000UL
#define SPEFLAGS_AUTOSTASH                     0x04000000UL

    char oclass;    /* object class */
    char invlet;    /* designation in inventory */
    short oartifact; /* artifact array index */
    uchar mythic_prefix; /* magical quality for a weapon or armor giving additional powers */
    uchar mythic_suffix;  /* magical quality for a weapon or armor giving additional powers */
    uchar exceptionality; /* exceptional, elite, etc. weapon, multiplies base damage */
    uchar elemental_enchantment; /* cold, fire, lightning, or deathly */

#define COLD_ENCHANTMENT 1
#define FIRE_ENCHANTMENT 2
#define LIGHTNING_ENCHANTMENT 3
#define DEATH_ENCHANTMENT 4

#define ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL 3 /* How many stackable items are enchanted. Also 1/quan is wear-off chance for non-stackable items */
#define ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION 2 
#define DEATH_ENCHANTMENT_QUANTITY_NORMAL 1 /* How many stackable items are enchanted. Also 1/quan is wear-off chance for non-stackable items */
#define DEATH_ENCHANTMENT_QUANTITY_BUC_VARIATION 1 
#define ELEMENTAL_ENCHANTMENT_BAG_WEAR_OFF_ONE_PER_CHANCE 3

    char recharged; /* number of times it's been recharged */
#define RECHARGE_LIMIT 100

    xchar where;        /* where the object thinks it is */
#define OBJ_FREE 0      /* object not attached to anything */
#define OBJ_FLOOR 1     /* object on floor */
#define OBJ_CONTAINED 2 /* object in a container */
#define OBJ_INVENT 3    /* object in the hero's inventory */
#define OBJ_MINVENT 4   /* object in a monster inventory */
#define OBJ_MIGRATING 5 /* object sent off to another level */
#define OBJ_BURIED 6    /* object buried */
#define OBJ_ONBILL 7    /* object on shk bill */
#define OBJ_HEROMEMORY 8  /* object remembered by hero */
#define NOBJ_STATES 9
    xchar timed; /* # of fuses (timers) attached to this obj */

    Bitfield(cursed, 1);
    Bitfield(blessed, 1);
    Bitfield(unpaid, 1);    /* on some bill */
    Bitfield(no_charge, 1); /* if shk shouldn't charge for this */
    Bitfield(known, 1);     /* exact nature & enchantment & charges known */
    Bitfield(dknown, 1);    /* description = color or text known */
    Bitfield(bknown, 1);    /* blessing or curse known */
    Bitfield(rknown, 1);    /* rustproof or not known */
    Bitfield(oeroded, 2);  /* rusted/burnt weapon/armor */

    Bitfield(oeroded2, 2); /* corroded/rotted weapon/armor */
#define greatest_erosion(otmp)                                 \
    (int)((otmp)->oeroded > (otmp)->oeroded2 ? (otmp)->oeroded \
                                             : (otmp)->oeroded2)
#define MAX_ERODE 3
#define orotten oeroded  /* rotten food */
#define odiluted oeroded /* diluted potions */
#define norevive oeroded2
    Bitfield(oerodeproof, 1); /* erodeproof weapon/armor */
    Bitfield(olocked, 1);     /* object is locked */
    Bitfield(obroken, 1);     /* lock has been broken */
#define degraded_horn obroken /* unicorn horn will poly to non-magic */
    Bitfield(otrapped, 1);    /* container is trapped */
/* or accidental tripped rolling boulder trap */
#define opoisoned otrapped /* object (weapon) is coated with poison */
    Bitfield(lamplit, 1);   /* a light-source -- can be lit */
    Bitfield(makingsound, 1);   /* a sound-source -- can be turned on to make noise */
    Bitfield(globby, 1);    /* combines with like types on adjacent squares */
    Bitfield(greased, 1);    /* covered with grease */

    Bitfield(nomerge, 1);    /* set temporarily to prevent merging */
    Bitfield(was_thrown, 1); /* thrown by hero since last picked up */
    Bitfield(has_special_tileset, 1); /* thrown by hero since last picked up */
    Bitfield(in_use, 1); /* for magic items before useup items */
    Bitfield(bypass, 1); /* mark this as an object to be skipped by bhito() */
    Bitfield(cknown, 1); /* contents of container assumed to be known */
    Bitfield(lknown, 1); /* locked/unlocked status is known */
    Bitfield(tknown, 1); /* trapped status of a container is known */
    Bitfield(nknown, 1); /* artifact's true name is known */
    Bitfield(aknown, 1); /* artifact status is known; if set, the artifact will be termed "the Artifact" instead of "item named Artifact" */

    Bitfield(mknown, 1); /* mythic quality is known */
    /* 1 free bit to 32-bit integer */

    int corpsenm;         /* type of corpse is mons[corpsenm] */
#define leashmon corpsenm /* gets m_id of attached pet */
#define appearanceidx corpsenm /* index for alternative appearance */
#define novelidx special_quality /* 3.6 tribute - the index of the novel title */
#define manualidx special_quality /* the index of the manual title */
#define keyotyp corpsenm  /* otyp of the key capable of locking / unlocking the chest (0 = SKELETON_KEY). Special_quality additionally defines the type of the key (its matching special_quality) */
    int usecount;           /* overloaded for various things that tally */
#define spestudied usecount /* # of times a spellbook has been studied */
#define ring_text_appeared usecount /* the round the text of Ruling Ring of Yendor appeared */
    unsigned oeaten;        /* nutrition left in food, if partly eaten */
    long age;               /* creation date */
    long owornmask;
    short cooldownleft;       /* item cooldown left before it can be used again*/
    short repowerleft;       /* artifact cooldown left before its invoke ability can be used again*/
    short detectioncount;    /* monsters detected for WARN_ORC and other similar properties */
    boolean invokeon;      /* the object's / artifact's invoked ability is on */
    unsigned o_id_memory;  /* This is a memory object of this o_id */
    unsigned m_id_memory;  /* This is a memory object of this mimic m_id */

    uchar special_tileset;
    int glyph;
    int gui_glyph;
    struct oextra *oextra; /* pointer to oextra struct */
};

#define newobj() (struct obj *) alloc(sizeof(struct obj))

/* property blocking */
/* This only allows for one blocking item per property */
#define w_blocks(o, m) \
    ((o->otyp == MUMMY_WRAPPING && ((m) & W_ARMO))                          \
         ? INVISIBILITY                                                            \
         : (o->otyp == CORNUTHAUM && ((m) & W_ARMH) && !Role_if(PM_WIZARD)) \
               ? CLAIRVOYANT                                                \
               : 0)

/***
 **    oextra referencing and testing macros
 */

#define ONAME(o) ((o)->oextra->oname)
#define UONAME(o) ((o)->oextra->uoname)
#define OMID(o) ((o)->oextra->omid)
#define OMONST(o) ((o)->oextra->omonst)
#define OLONG(o) ((o)->oextra->olong)
#define OMAILCMD(o) ((o)->oextra->omailcmd)

#define has_oname(o) ((o)->oextra && ONAME(o))
#define has_uoname(o) ((o)->oextra && UONAME(o))
#define has_omid(o) ((o)->oextra && OMID(o))
#define has_omonst(o) ((o)->oextra && OMONST(o))
#define has_olong(o) ((o)->oextra && OLONG(o))
#define has_omailcmd(o) ((o)->oextra && OMAILCMD(o))

/* Weapons and weapon-tools */
/* KMH -- now based on skill categories.  Formerly:
 *    #define is_sword(otmp)    (otmp->oclass == WEAPON_CLASS && \
 *             objects[otmp->otyp].oc_wepcat == WEP_SWORD)
 *    #define is_blade(otmp)    (otmp->oclass == WEAPON_CLASS && \
 *             (objects[otmp->otyp].oc_wepcat == WEP_BLADE || \
 *              objects[otmp->otyp].oc_wepcat == WEP_SWORD))
 *    #define is_weptool(o)    ((o)->oclass == TOOL_CLASS && \
 *             objects[(o)->otyp].oc_weptool)
 *    #define is_multigen(otyp) (otyp <= SHURIKEN)
 *    #define is_poisonable(otyp) (otyp <= BEC_DE_CORBIN)
 */
#define is_otyp_appliable(otyp) ((objects[(otyp)].oc_flags3 & O3_APPLIABLE) != 0)
#define is_otyp_invokable(otyp) ((objects[(otyp)].oc_flags3 & O3_INVOKABLE) != 0)
#define is_obj_appliable(otmp) is_otyp_appliable((otmp)->otyp)
#define is_obj_invokable(otmp) is_otyp_invokable((otmp)->otyp)

#define is_otyp_indestructible(otyp) ((objects[(otyp)].oc_flags & O1_INDESTRUCTIBLE) != 0)
#define is_obj_indestructible(o) (is_otyp_indestructible((o)->otyp) || ((o)->speflags & SPEFLAGS_INDESTRUCTIBLE) != 0 \
                                  || ((o)->oartifact > 0 && (artilist[(o)->oartifact].aflags2 & AF2_INDESTRUCTIBLE) != 0))

#define is_otyp_no_pickup(otyp) ((objects[(otyp)].oc_flags3 & O3_NO_PICKUP) != 0)
#define is_obj_no_pickup(o) (is_otyp_no_pickup((o)->otyp) || ((o)->speflags & SPEFLAGS_NO_PICKUP) != 0)

#define is_blade(otmp)                           \
    ((otmp)->oclass == WEAPON_CLASS                \
     && objects[(otmp)->otyp].oc_skill >= P_DAGGER \
     && objects[(otmp)->otyp].oc_skill <= P_SWORD)
#define is_axe(otmp)                                              \
    ((((otmp)->oclass == WEAPON_CLASS || (otmp)->oclass == TOOL_CLASS) \
     && objects[(otmp)->otyp].oc_skill == P_AXE) || ((otmp)->oartifact && is_artifact_applicable_as_axe(otmp)))
#define is_pick(otmp)                                             \
    (((otmp)->oclass == WEAPON_CLASS || (otmp)->oclass == TOOL_CLASS) \
     && objects[(otmp)->otyp].oc_skill == P_PICK_AXE)
#define is_sword(otmp)                                \
    ((otmp)->oclass == WEAPON_CLASS                     \
     && objects[(otmp)->otyp].oc_skill == P_SWORD)
#define is_pole(otmp)                                             \
    ((otmp)->oclass == WEAPON_CLASS && objects[(otmp)->otyp].oc_subtyp == WEP_POLEARM)
#define is_spear(otmp) \
    ((otmp)->oclass == WEAPON_CLASS && objects[(otmp)->otyp].oc_subtyp == WEP_SPEAR)
#define is_lance(otmp) \
    ((otmp)->oclass == WEAPON_CLASS && objects[(otmp)->otyp].oc_subtyp == WEP_LANCE)
#define is_appliable_pole_type_weapon(otmp)   \
    (is_pole(otmp) || is_spear(otmp) || is_lance(otmp))
#define is_appliable_weapon(otmp) \
    ((otmp)->oclass == WEAPON_CLASS && (is_pick(otmp) || is_axe(otmp) || is_appliable_pole_type_weapon(otmp) || is_obj_appliable(otmp)))
#define is_launcher(otmp)                                                  \
    ((otmp)->oclass == WEAPON_CLASS && objects[(otmp)->otyp].oc_skill >= P_BOW \
     && objects[(otmp)->otyp].oc_skill <= P_CROSSBOW)
#define is_ammo(otmp)                                            \
    (((otmp)->oclass == WEAPON_CLASS || (otmp)->oclass == GEM_CLASS) \
     && objects[(otmp)->otyp].oc_skill >= -P_CROSSBOW              \
     && objects[(otmp)->otyp].oc_skill <= -P_BOW)
#define matching_launcher(a, l) \
    ((l) && objects[(a)->otyp].oc_skill == -objects[(l)->otyp].oc_skill)
#define ammo_and_launcher(a, l) (is_ammo(a) && matching_launcher(a, l))
#define is_thrown_weapon_only(o) ((objects[(o)->otyp].oc_flags & O1_THROWN_WEAPON_ONLY) != 0)
#define is_weptool(o) \
    ((o)->oclass == TOOL_CLASS && (objects[(o)->otyp].oc_flags4 & O4_WEAPON_TOOL) != 0)
        /* towel is not a weptool:  enchantment isn't an enchantment, cursed towel
           doesn't weld to hand, and twoweapon won't work with one */
#define is_wielded_weapon(o) \
    ((o)->oclass == WEAPON_CLASS || is_weptool(o) || (objects[(o)->otyp].oc_flags & O1_IS_WEAPON_WHEN_WIELDED) != 0)
#define is_weapon(o) \
    (is_wielded_weapon(o) || (objects[(o)->otyp].oc_flags5 & O5_IS_WEAPON_WHEN_WORN) != 0)
#define is_missile(o)                                          \
    (((o)->oclass == WEAPON_CLASS || is_weptool(o)) && is_thrown_weapon_only(o))

#define is_amulet(o) \
    ((o)->oclass == AMULET_CLASS)
#define is_wet_towel(o) ((o)->otyp == TOWEL && (o)->special_quality > 0)
#define bimanual(otmp)                                            \
    (((otmp)->oclass == WEAPON_CLASS || (otmp)->oclass == TOOL_CLASS) \
     && objects[(otmp)->otyp].oc_bimanual)
#define is_multigen(otmp)                           \
    (objects[(otmp)->otyp].oc_multigen_type > MULTIGEN_SINGLE)

#define is_poisonable(otmp) \
    ((is_weapon(otmp) && !is_launcher(otmp) && objects[(otmp)->otyp].oc_dir > WHACK) || objects[(otmp)->otyp].oc_flags3 & O3_POISONABLE)

#define is_obj_tethered_weapon(o, wmask)  \
    ((objects[(o)->otyp].oc_flags4 & O4_TETHERED_WEAPON) != 0 && ((wmask) & W_WIELDED_WEAPON) != 0)


#define uslinging() (uwep && objects[uwep->otyp].oc_skill == P_SLING)
/* 'is_quest_artifact()' only applies to the current role's artifact */
#define any_quest_artifact(o) ((o)->oartifact >= ART_ORB_OF_DETECTION)

#define is_otyp_enchantable(otyp)     \
    (objects[(otyp)].oc_enchantable > ENCHTYPE_NO_ENCHANTMENT)

#define is_obj_enchantable(o) is_otyp_enchantable((o)->otyp) 

#define is_otyp_elemental_enchantable(otyp)     \
    (objects[(otyp)].oc_flags3 & O3_ELEMENTAL_ENCHANTABLE)

/* Unusual definition to account for weapons appropriately */
#define is_elemental_enchantable(o)     ((is_weapon(o) && !is_launcher(o)) || is_otyp_elemental_enchantable((o)->otyp))

#define is_otyp_material_death_enchantable(otyp)     \
    (material_definitions[objects[otyp].oc_material].death_enchantable != 0)

#define is_death_enchantable(o)  (is_elemental_enchantable(o) && is_otyp_material_death_enchantable((o)->otyp))

#define can_otyp_have_exceptionality(otyp)     \
    (objects[(otyp)].oc_flags4 & O4_CAN_HAVE_EXCEPTIONALITY)

/* Unusual definition to account for weapons appropriately */
#define can_have_exceptionality(o)     (is_weapon(o) || can_otyp_have_exceptionality((o)->otyp))

#define otyp_allows_specially_dipping_into(otyp) (objects[(otyp)].oc_flags4 & O4_ALLOWS_DIPPING_INTO)
#define otyp_allows_object_to_be_dipped_into_it(otyp) (objects[(otyp)].oc_class == POTION_CLASS || otyp_allows_specially_dipping_into(otyp))
#define obj_allows_object_to_be_dipped_into_it(o) otyp_allows_object_to_be_dipped_into_it((o)->otyp)

#define is_cursed_magic_item(otmp)                                            \
    (objects[(otmp)->otyp].oc_flags2 & O2_CURSED_MAGIC_ITEM)

#define is_obj_generated_cursed(otmp)                                            \
    (objects[(otmp)->otyp].oc_flags2 & O2_GENERATED_CURSED)

#define is_obj_generated_blessed(otmp)                                            \
    (objects[(otmp)->otyp].oc_flags2 & O2_GENERATED_BLESSED)

#define oresist_disintegration(otmp)                                       \
    (objects[(otmp)->otyp].oc_flags & O1_DISINTEGRATION_RESISTANT || is_obj_indestructible(otmp) \
     || ((otmp)->otyp == CORPSE && pm_resists_disint(&mons[(otmp)->corpsenm])) \
     || obj_resists(otmp, 2, 50) \
     || is_quest_artifact(otmp) )

#define oresist_fire(otmp)                                       \
    (objects[(otmp)->otyp].oc_flags & O1_FIRE_RESISTANT || is_obj_indestructible(otmp) \
     || ((otmp)->otyp == CORPSE && pm_resists_fire(&mons[(otmp)->corpsenm])) \
     || obj_resists(otmp, 0, 0) \
     || is_quest_artifact(otmp) )

#define oresist_cold(otmp)                                       \
    (objects[(otmp)->otyp].oc_flags & O1_COLD_RESISTANT || is_obj_indestructible(otmp) \
     || ((otmp)->otyp == CORPSE && pm_resists_cold(&mons[(otmp)->corpsenm])) \
     || obj_resists(otmp, 0, 0) \
     || is_quest_artifact(otmp) )

#define oresist_elec(otmp)                                       \
    (objects[(otmp)->otyp].oc_flags & O1_LIGHTNING_RESISTANT || is_obj_indestructible(otmp) \
     || ((otmp)->otyp == CORPSE && pm_resists_elec(&mons[(otmp)->corpsenm])) \
     || obj_resists(otmp, 0, 0) \
     || is_quest_artifact(otmp) )

/* Armor */
#define is_shield(otmp)          \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_SHIELD)
#define is_helmet(otmp) \
    ((otmp)->oclass == ARMOR_CLASS && objects[(otmp)->otyp].oc_armor_category == ARM_HELM)
#define is_conical_hat(otmp) \
    ((otmp)->otyp >= CORNUTHAUM && (otmp)->otyp <= GNOMISH_FELT_HAT)
#define is_boots(otmp)           \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_BOOTS)
#define is_gloves(otmp)          \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_GLOVES)
#define is_cloak(otmp)           \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_CLOAK)
#define is_shirt(otmp)           \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_SHIRT)
#define is_robe(otmp)           \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_ROBE)
#define is_bracers(otmp)           \
    ((otmp)->oclass == ARMOR_CLASS \
     && objects[(otmp)->otyp].oc_armor_category == ARM_BRACERS)
#define is_suit(otmp) \
    ((otmp)->oclass == ARMOR_CLASS && objects[(otmp)->otyp].oc_armor_category == ARM_SUIT)
#define is_elven_armor(otmp)                                              \
    ((otmp)->oclass == ARMOR_CLASS && is_elven_obj(otmp))
#define is_orcish_armor(otmp)                                            \
    ((otmp)->oclass == ARMOR_CLASS && is_orcish_obj(otmp))
#define is_dwarvish_armor(otmp)               \
    ((otmp)->oclass == ARMOR_CLASS && is_dwarvish_obj(otmp))
#define is_gnollish_armor(otmp) \
    ((otmp)->oclass == ARMOR_CLASS && is_gnollish_obj(otmp))
#define is_gnomish_armor(otmp) \
    ((otmp)->oclass == ARMOR_CLASS && is_gnomish_obj(otmp))

/* Wielded items */
#define is_wielded_item(o) (is_wielded_weapon(o) || is_shield(o))

/* Eggs and other food */
#define MAX_EGG_HATCH_TIME 200 /* longest an egg can remain unhatched */
#define stale_egg(egg) \
    ((monstermoves - (egg)->age) > (2 * MAX_EGG_HATCH_TIME))
#define ofood(o) ((o)->otyp == CORPSE || (o)->otyp == EGG || (o)->otyp == TIN)
#define polyfodder(o) (ofood(o) && pm_to_cham((o)->corpsenm) != NON_PM)
#define mlevelgain(o) (ofood(o) && (o)->corpsenm >= LOW_PM && (mons[(o)->corpsenm].mconveys & MC_LEVEL_GAIN))
#define mhealup(o) (ofood(o) && (o)->corpsenm == PM_NURSE)
#define mcurepetrification(o) (ofood(o) && (o)->corpsenm == PM_LIZARD)
#define Is_pudding(o) ((o)->oclass == FOOD_CLASS && objects[(o)->otyp].oc_subtyp == FOODTYPE_GLOB)

/* Containers */
#define carried(o) ((o)->where == OBJ_INVENT)
#define mcarried(o) ((o)->where == OBJ_MINVENT)
#define Has_contents(o)                                \
    (/* (Is_container(o) || (o)->otyp == STATUE) && */ \
     (o)->cobj != (struct obj *) 0)
#define Is_otyp_container(otyp) ((objects[(otyp)].oc_flags2 & O2_CONTAINER) != 0)
#define Is_container(o) Is_otyp_container((o)->otyp)
#define Has_otyp_lid(otyp) ((objects[(otyp)].oc_flags4 & O4_CONTAINER_HAS_LID) != 0)
#define Is_otyp_container_with_lid(otyp) (Is_otyp_container(otyp) && Has_otyp_lid(otyp))
#define Is_container_with_lid(o) Is_otyp_container_with_lid((o)->otyp)
#define Is_noncontainer(o) ((objects[(o)->otyp].oc_flags2 & O2_CONTAINER_NONCONTAINER) != 0)
#define Is_proper_container(o) (Is_container(o) && !Is_noncontainer(o))
#define Is_box(o) ((objects[(o)->otyp].oc_flags2 & O2_CONTAINER_BOX) != 0)
#define Is_mbag(o) \
    ((objects[(o)->otyp].oc_flags2 & O2_CONTAINER_MAGIC_BAG) != 0)
#define Is_weight_changing_bag(o) \
    ((objects[(o)->otyp].oc_flags2 & O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG) != 0)
#define SchroedingersBox(o) ((o)->otyp == LARGE_BOX && ((o)->speflags & SPEFLAGS_SCHROEDINGERS_BOX) != 0)

/* dragon gear */
#define is_dragon_scales(obj) \
    (is_dragon_obj(obj) && (objects[(obj)->otyp].oc_flags2 & O2_MONSTER_SCALES))
#define is_dragon_mail(obj)  \
    (is_dragon_obj(obj) && (objects[(obj)->otyp].oc_flags2 & O2_MONSTER_SCALE_MAIL))
#define is_dragon_scale_armor(obj) \
    (is_dragon_scales(obj) || is_dragon_mail(obj))
#define Dragon_scales_to_pm(obj) \
    &mons[PM_GRAY_DRAGON + (obj)->otyp - GRAY_DRAGON_SCALES]
#define Dragon_mail_to_pm(obj) \
    &mons[PM_GRAY_DRAGON + (obj)->otyp - GRAY_DRAGON_SCALE_MAIL]
#define Dragon_to_scales(pm) (GRAY_DRAGON_SCALES + (pm - mons))

#define is_dragon_armor(obj) \
    (obj->oclass == ARMOR_CLASS && is_dragon_obj(obj))
#define is_dragon_obj(obj) ((objects[(obj)->otyp].oc_flags2 & O2_DRAGON_ITEM) != 0)

/* Elven gear */
#define is_elven_weapon(otmp)                                             \
    ((otmp)->oclass == WEAPON_CLASS && is_elven_obj(otmp))
#define is_elven_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_ELVEN_ITEM) != 0)

/* Orcish gear */
#define is_orcish_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_ORCISH_ITEM) != 0)

/* Dwarvish gear */
#define is_dwarvish_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_DWARVEN_ITEM) != 0)

/* Gnomish gear */
#define is_gnomish_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_GNOMISH_ITEM) != 0)

/* Gnollish gear */
#define is_gnollish_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_GNOLLISH_ITEM) != 0)

/* Modron gear */
#define is_modron_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_MODRON_ITEM) != 0)

/* Demon gear */
#define is_demon_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_DEMON_ITEM) != 0 || (otmp)->exceptionality == EXCEPTIONALITY_INFERNAL)

/* Angel gear */
#define is_angel_obj(otmp) ((objects[(otmp)->otyp].oc_flags2 & O2_ANGEL_ITEM) != 0 || (otmp)->exceptionality == EXCEPTIONALITY_CELESTIAL)


/* Light sources */
#define is_otyp_candle(otyp) \
    ((objects[(otyp)].oc_flags2 & O2_CANDLE) != 0)
#define is_candle(otmp) \
    is_otyp_candle((otmp)->otyp)
#define is_otyp_candelabrum(otyp) \
    ((objects[(otyp)].oc_flags4 & O4_CANDELABRUM) != 0)
#define is_obj_candelabrum(otmp) \
    is_otyp_candelabrum((otmp)->otyp)
#define otyp_shines_magical_light(otyp)                                             \
    ((objects[(otyp)].oc_flags2 & O2_SHINES_MAGICAL_LIGHT) != 0)
#define obj_shines_magical_light(otmp)     \
    otyp_shines_magical_light((otmp)->otyp)
#define is_otyp_special_praying_item(otyp)                                             \
    ((objects[(otyp)].oc_flags2 & O2_SPECIAL_PRAYING_ITEM) != 0)
#define is_obj_special_praying_item(otmp)     \
    is_otyp_special_praying_item((otmp)->otyp)

/* Wand-like tools */
#define is_wand_like_tool(otmp) \
    ((objects[(otmp)->otyp].oc_flags & O1_WAND_LIKE_TOOL) != 0)

/* Other tools */
#define is_saw(otmp)                                              \
    ((otmp)->oclass == TOOL_CLASS \
     && objects[(otmp)->otyp].oc_subtyp == TOOLTYPE_SAW)

#define MAX_OIL_IN_FLASK 400 /* maximum amount of oil in a potion of oil */

/* MAGIC_LAMP intentionally excluded below */
/* age field of this is relative age rather than absolute */
#define age_is_relative(otmp)                                       \
    ((objects[(otmp)->otyp].oc_flags3 & O3_RELATIVE_AGE) != 0)

/* object can be ignited */
#define is_otyp_ignitable(otyp)                                             \
    ((objects[(otyp)].oc_flags3 & O3_IGNITABLE) != 0)

#define is_obj_ignitable(otmp)                                             \
    (is_otyp_ignitable((otmp)->otyp))

/* object can be refilled with oil */
#define is_refillable_with_oil(otmp)                                             \
    ((objects[(otmp)->otyp].oc_flags3 & O3_REFILLABLE_WITH_OIL) != 0)

/* things that can be read */
#define is_readable(otmp)                                                    \
    ((objects[(otmp)->otyp].oc_flags3 & O3_READABLE) || ((otmp)->oartifact && artilist[(otmp)->oartifact].aflags & AF_READABLE))

/* special stones */
#define is_otyp_rock(otyp)                                 \
    ((objects[otyp].oc_flags2 & O2_ROCK) != 0)
#define is_otyp_ore(otyp)                                 \
    ((objects[otyp].oc_flags2 & O2_ORE) != 0)

#define is_otyp_graystone(otyp)                                 \
    ((objects[otyp].oc_flags2 & O2_GRAYSTONE) != 0)
#define is_graystone(obj)                                 \
    (is_otyp_graystone((obj)->otyp))
#define is_rock(obj)                                 \
    (is_otyp_rock((obj)->otyp))
#define is_ore(obj)                                 \
    (is_otyp_ore((obj)->otyp))

/* other */
#define is_otyp_key(otyp)                                 \
    ((objects[(otyp)].oc_flags2 & O2_KEY) != 0)
#define is_key(obj)                                 \
    (is_otyp_key((obj)->otyp))
#define is_unlocking_tool(obj)                                 \
    (is_key(obj) || (obj)->otyp == LOCK_PICK || (obj)->otyp == CREDIT_CARD)
#define is_locking_tool(obj)                                 \
    (is_key(obj) || (obj)->otyp == LOCK_PICK)

/* misc helpers, simple enough to be macros */
#define is_flimsy(otmp)                           \
    (objects[(otmp)->otyp].oc_material <= MAT_LEATHER \
     || (objects[(otmp)->otyp].oc_flags2 & O2_FLIMSY))
#define is_plural(o) \
    ((o)->quan != 1L                                                    \
     /* "the Eyes of the Overworld" are plural, but                     \
        "a pair of lenses named the Eyes of the Overworld" is not */    \
     || ((o)->oartifact == ART_EYES_OF_THE_OVERWORLD && (o)->nknown && (o)->known))

#define pair_of(o) ((o->oclass == MISCELLANEOUS_CLASS && \
    (objects[(o)->otyp].oc_subtyp == MISC_EYEGLASSES || objects[(o)->otyp].oc_subtyp == MISC_EARRINGS || objects[(o)->otyp].oc_subtyp == MISC_PANTS || objects[(o)->otyp].oc_subtyp == MISC_WINGS || objects[(o)->otyp].oc_subtyp == MISC_EXTRA_ARMS)) \
     || is_gloves(o) || is_boots(o) || is_bracers(o))

#define is_otyp_unique(otyp) (objects[otyp].oc_unique || (objects[otyp].oc_flags3 & O3_UNIQUE))
#define is_otyp_nowish(otyp) (objects[otyp].oc_nowish || (objects[otyp].oc_flags3 & O3_NO_WISH))
#define is_otyp_unburiable(otyp) ((objects[otyp].oc_flags3 & O3_UNBURIABLE))

#define is_obj_unique(obj) is_otyp_unique((obj)->otyp)
#define is_obj_nowish(obj) is_otyp_nowish((obj)->otyp)
#define is_obj_unburiable(obj) \
    (is_otyp_unburiable((obj)->otyp) || obj_resists(obj, 0, 0) || (obj) == uchain || ((obj)->otyp == CORPSE && (obj)->corpsenm >= LOW_PM && is_rider(&mons[(obj)->corpsenm])))

#define nonrotting_food(otyp) \
    ((objects[(otyp)].oc_flags3 & O3_NONROTTING_FOOD) != 0)

#define is_obj_rotting_corpse(o) ((o)->otyp == CORPSE || (o)->globby)

#define has_otyp_double_digging_effort(otyp) \
    ((objects[(otyp)].oc_flags3 & O3_DOUBLE_DIGGING_EFFORT) != 0)
#define is_otyp_buried_searchable(otyp) \
    ((objects[(otyp)].oc_flags3 & O3_BURIED_SEARCHABLE) != 0)
#define is_otyp_content_description_shuffled(otyp) \
    ((objects[(otyp)].oc_flags3 & O3_CONTENT_DESCRIPTION_SHUFFLED) != 0)
#define otyp_consumes_nutrition_every_20_rounds(otyp) \
    ((objects[(otyp)].oc_flags3 & O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS) != 0)
#define obj_consumes_nutrition_every_20_rounds(otmp) \
    otyp_consumes_nutrition_every_20_rounds((otmp)->otyp)
#define is_otyp_quaffable(otyp)                                 \
    ((objects[otyp].oc_flags3 & O3_QUAFFABLE) != 0)
#define is_obj_quaffable(obj)                                 \
    is_otyp_quaffable((obj)->otyp)

#define is_obj_normally_edible(otmp) \
    ((otmp)->oclass == FOOD_CLASS || ((otmp)->oclass == REAGENT_CLASS && (objects[(otmp)->otyp].oc_flags & O1_EDIBLE_NONFOOD) != 0))

#define has_otyp_extended_polearm_reach(otyp) \
    ((objects[(otyp)].oc_flags4 & O4_EXTENDED_POLEARM_REACH) != 0)

#define has_otyp_missile_tile(otyp)                                 \
    ((objects[otyp].oc_flags4 & O4_MISSILE_TILE) != 0)

#define is_otyp_drawn_in_front(otyp, tx, ty) \
     ((objects[(otyp)].oc_flags4 & O4_DRAWN_IN_FRONT) && (tx) == u.ux && (ty) == u.uy)

#define is_obj_drawn_in_front(obj) \
    (is_otyp_drawn_in_front((obj)->otyp, (obj)->ox, (obj)->oy))

#define has_otyp_floor_tile(otyp)                                 \
    ((objects[(otyp)].oc_flags4 & O4_FLOOR_TILE) != 0)

#define has_obj_floor_tile(o) has_otyp_floor_tile((o)->otyp)     

#define is_otyp_mbag_destroying(otyp)                                 \
    ((objects[(otyp)].oc_flags5 & O5_MBAG_DESTROYING_ITEM) != 0)

#define is_obj_mbag_destroying(obj) \
    (is_otyp_mbag_destroying((obj)->otyp))

#define does_otyp_drain_instead_of_explode(otyp)                                 \
    ((objects[(otyp)].oc_flags5 & O5_CANCELLATION_NO_EXPLOSION_BUT_DRAIN) != 0)

#define does_obj_drain_instead_of_explode(obj) \
    (does_otyp_drain_instead_of_explode((obj)->otyp))

#define can_obj_cause_choking(o) (obj_nutrition(o) > 50)

/* 'PRIZE' values override obj->corpsenm so prizes mustn't be object types
   which use that field for monster type (or other overloaded purpose) */

#define is_mines_prize(o) \
    ((o)->otyp == iflags.mines_prize_type && (o)->oartifact == iflags.mines_prize_oartifact              \
     && ((o)->speflags & SPEFLAGS_MINES_PRIZE) != 0)
#define is_soko_prize(o) \
    (((o)->otyp == iflags.soko_prize_type1               \
      && ((o)->speflags & SPEFLAGS_SOKO_PRIZE1) != 0)     \
     || ((o)->otyp == iflags.soko_prize_type2            \
         && ((o)->speflags & SPEFLAGS_SOKO_PRIZE2) != 0))

/* true for gems/rocks that should have " stone" appended to their names */
#define GemStone(typ)                                                  \
    (objects[typ].oc_class == GEM_CLASS && (typ == FLINT               \
     || (objects[typ].oc_material == MAT_GEMSTONE                      \
         && (typ != DILITHIUM_CRYSTAL && typ != RUBY && typ != DIAMOND \
             && typ != SAPPHIRE && typ != BLACK_OPAL && typ != EMERALD \
             && typ != OPAL && typ != PEARL && typ != BLACK_PEARL))))

/* Exceptionality */
enum exceptionality_types {
    EXCEPTIONALITY_NORMAL = 0,
    EXCEPTIONALITY_EXCEPTIONAL,
    EXCEPTIONALITY_ELITE,
    EXCEPTIONALITY_CELESTIAL,
    EXCEPTIONALITY_PRIMORDIAL,
    EXCEPTIONALITY_INFERNAL,
    MAX_EXCEPTIONALITY_TYPES
};


/* Mythic */
struct mythic_definition {
    const char* name;
    const char* mythic_affix;
    const char* description;
    short probability;
    double price_multiplier;
    long price_addition;
    unsigned long mythic_powers;
    unsigned long mythic_flags;
};

#define MYTHIC_FLAG_NONE                        0x00000000UL
#define MYTHIC_FLAG_WEAPON_REQUIRED             0x00000001UL /* Must be a weapon */
#define MYTHIC_FLAG_ARMOR_REQUIRED              0x00000002UL /* Must be an armor */
#define MYTHIC_FLAG_NO_PIERCING_WEAPONS         0x00000004UL
#define MYTHIC_FLAG_NO_SLASHING_WEAPONS         0x00000008UL
#define MYTHIC_FLAG_NO_BLUDGEONING_WEAPONS      0x00000010UL
#define MYTHIC_FLAG_DIRECTLY_WISHABLE           0x00000020UL
#define MYTHIC_FLAG_NON_WISHABLE                0x00000040UL
#define MYTHIC_FLAG_NO_CELESTIAL_WEAPONS        0x00000080UL
#define MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS       0x00000100UL
#define MYTHIC_FLAG_NO_INFERNAL_WEAPONS         0x00000200UL
#define MYTHIC_FLAG_POLEARM_LANCE_SPEAR_ONLY    0x00000400UL
#define MYTHIC_FLAG_NO_WEAPON                   0x00000800UL /* Must not be a weapon */
#define MYTHIC_FLAG_NO_ARMOR                    0x00001000UL /* Must not be an armor, e.g. a weapon shield (spiked shield) will not do */
#define MYTHIC_FLAG_HELMET_REQUIRED             0x00002000UL /* Must be a helmet */
#define MYTHIC_FLAG_SHIELD_REQUIRED             0x00004000UL /* Must be a shield */
#define MYTHIC_FLAG_SUIT_REQUIRED               0x00008000UL /* Must be a suit */
#define MYTHIC_FLAG_NO_THROWN_OR_AMMO           0x00010000UL
#define MYTHIC_FLAG_THROWN_WEAPON_ONLY          0x00020000UL
#define MYTHIC_FLAG_NO_RETURNING_WEAPONS        0x00040000UL
#define MYTHIC_FLAG_NO_LOW_SPELLCASTING_PENALTY_ITEMS 0x00080000UL
#define MYTHIC_FLAG_RACIAL_PREFIX               0x00100000UL
#define MYTHIC_FLAG_NO_ORCISH_ITEMS             0x00200000UL
#define MYTHIC_FLAG_NO_ELVEN_ITEMS              0x00400000UL
#define MYTHIC_FLAG_NO_DEMONIC_ITEMS            0x00800000UL
#define MYTHIC_FLAG_NO_OTHER_SORCERY            0x01000000UL

#define MYTHIC_FLAG_SLASHING_WEAPONS_ONLY    (MYTHIC_FLAG_NO_PIERCING_WEAPONS | MYTHIC_FLAG_NO_BLUDGEONING_WEAPONS)
#define MYTHIC_FLAG_PIERCING_WEAPONS_ONLY    (MYTHIC_FLAG_NO_BLUDGEONING_WEAPONS | MYTHIC_FLAG_NO_SLASHING_WEAPONS)
#define MYTHIC_FLAG_BLUDGEONING_WEAPONS_ONLY (MYTHIC_FLAG_NO_PIERCING_WEAPONS | MYTHIC_FLAG_NO_SLASHING_WEAPONS)
#define MYTHIC_FLAG_SHARP_WEAPONS_ONLY        MYTHIC_FLAG_NO_BLUDGEONING_WEAPONS

enum mythic_prefix_types {
    MYTHIC_PREFIX_NONE = 0,
    MYTHIC_PREFIX_STYGIAN,
    MYTHIC_PREFIX_HYPERBOREAN,
    MYTHIC_PREFIX_ASGARDIAN,
    MYTHIC_PREFIX_ATLANTEAN,
    MYTHIC_PREFIX_ELYSIAN,
    MYTHIC_PREFIX_CYCLOPEAN,
    MYTHIC_PREFIX_HERCULEAN,
    MYTHIC_PREFIX_VAMPIRIC,
    MYTHIC_PREFIX_RADIANT,
    MYTHIC_PREFIX_HALLOWED,
    MYTHIC_PREFIX_WITCH_KINGS,
    MYTHIC_PREFIX_SORCERERS,
    MYTHIC_PREFIX_OLYMPIAN,
    MYTHIC_PREFIX_JOTUNHEIMIAN,
    MAX_MYTHIC_PREFIXES
};

enum mythic_suffix_types {
    MYTHIC_SUFFIX_NONE = 0,
    MYTHIC_SUFFIX_LIGHTNESS,
    MYTHIC_SUFFIX_SORCERY,
    MYTHIC_SUFFIX_TROLL_SLAYING,
    MYTHIC_SUFFIX_OGRE_SLAYING,
    MYTHIC_SUFFIX_DEMON_SLAYING,
    MYTHIC_SUFFIX_DRAGON_SLAYING,
    MYTHIC_SUFFIX_GIANT_SLAYING,
    MYTHIC_SUFFIX_LYCANTHROPE_SLAYING,
    MYTHIC_SUFFIX_DISRUPTION,
    MYTHIC_SUFFIX_SPEED,
    MYTHIC_SUFFIX_WOUNDING,
    MYTHIC_SUFFIX_DEFENSE,
    MYTHIC_SUFFIX_SHARPNESS,
    MYTHIC_SUFFIX_REACH,
    MYTHIC_SUFFIX_PROVIDENCE,
    MYTHIC_SUFFIX_FREE_ACTION,
    MYTHIC_SUFFIX_FIRE_RESISTANCE,
    MYTHIC_SUFFIX_COLD_RESISTANCE,
    MYTHIC_SUFFIX_SHOCK_RESISTANCE,
    MYTHIC_SUFFIX_COCKATRICE_SLAYING,
    MYTHIC_SUFFIX_EYES,
    MYTHIC_SUFFIX_ELEMENTAL_PROTECTION,
    MYTHIC_SUFFIX_ORC_SLAYING,
    MYTHIC_SUFFIX_ELF_SLAYING,
    MYTHIC_SUFFIX_RETURNING,
    MYTHIC_SUFFIX_BANISHMENT,
    MYTHIC_SUFFIX_LAST_ALLIANCE,
    MAX_MYTHIC_SUFFIXES
};


struct mythic_power_definition {
    const char* name;
    const char* description;
    uchar power_type;
    long parameter1;                /* E.g., damage multiplier */
    double parameter2;              /* E.g., damage multiplier */
    char parameter3;                /* E.g., monster or item class */
    unsigned long parameter4;       /* E.g., M2_ flag */
    unsigned long power_flags;
};

enum mythic_power_types {
    MYTHIC_POWER_TYPE_GENERAL = 0,
    MYTHIC_POWER_TYPE_SLAYING,
    MYTHIC_POWER_TYPE_PREVENTS_REVIVAL,
    MYTHIC_POWER_TYPE_CONFERS_PROPERTY,
};

#define MYTHIC_POWER_FLAG_NONE                  0x00000000 /* Works for all */
#define MYTHIC_POWER_FLAG_WEAPON_ONLY           0x00000001 /* Works only for weapons */
#define MYTHIC_POWER_FLAG_ARMOR_ONLY            0x00000002 /* Works only for armors */
#define MYTHIC_POWER_FLAG_SUIT_ONLY             0x00000004 /* Works only for body armors (suits) */
#define MYTHIC_POWER_FLAG_HELMET_ONLY           0x00000008 /* Works only for helmets */
#define MYTHIC_POWER_FLAG_SHIELD_ONLY           0x00000010 /* Works only for shields */
#define MYTHIC_POWER_FLAG_ALSO_SHAPESHIFTERS    0x00000020 /* Works also against shapeshifted monsters of the type */
#define MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO     0x00000040 /* No thrown weapons or ammo for this power */
#define MYTHIC_POWER_FLAG_THROWN_WEAPONS_ONLY   0x00000080 /* Works only for thrown weapons */


enum mythic_prefix_power_types {
    MYTHIC_PREFIX_POWER_INDEX_LEVEL_DRAIN = 0,
    MYTHIC_PREFIX_POWER_INDEX_MANA_GAIN_25,
    MYTHIC_PREFIX_POWER_INDEX_HP_GAIN_25,
    MYTHIC_PREFIX_POWER_INDEX_LIFE_DRAINING,
    MYTHIC_PREFIX_POWER_INDEX_SHINES_LIGHT,
    MYTHIC_PREFIX_POWER_INDEX_ARMOR_DEATH_RESISTANCE,
    MYTHIC_PREFIX_POWER_INDEX_ARMOR_DRAIN_RESISTANCE,
    MYTHIC_PREFIX_POWER_INDEX_REGENERATION,
    MYTHIC_PREFIX_POWER_INDEX_MANA_REGENERATION,
    MYTHIC_PREFIX_POWER_INDEX_CURSE_RESISTANCE,
    MYTHIC_PREFIX_POWER_INDEX_TRIPLE_BASE_DAMAGE,
    MYTHIC_PREFIX_POWER_INDEX_ADDED_ENCHANTABILITY,
    MYTHIC_PREFIX_POWER_INDEX_UNCURSEABLE,
    MYTHIC_PREFIX_POWER_INDEX_GREAT_STRENGTH,
    MYTHIC_PREFIX_POWER_INDEX_SORCERY,
    MAX_MYTHIC_PREFIX_POWERS
};

#define MYTHIC_PREFIX_POWER_NONE                    0x00000000UL
#define MYTHIC_PREFIX_POWER_LEVEL_DRAIN             (1UL << MYTHIC_PREFIX_POWER_INDEX_LEVEL_DRAIN)
#define MYTHIC_PREFIX_POWER_MANA_GAIN_25            (1UL << MYTHIC_PREFIX_POWER_INDEX_MANA_GAIN_25)
#define MYTHIC_PREFIX_POWER_HP_GAIN_25              (1UL << MYTHIC_PREFIX_POWER_INDEX_HP_GAIN_25)
#define MYTHIC_PREFIX_POWER_LIFE_DRAINING           (1UL << MYTHIC_PREFIX_POWER_INDEX_LIFE_DRAINING)
#define MYTHIC_PREFIX_POWER_SHINES_LIGHT            (1UL << MYTHIC_PREFIX_POWER_INDEX_SHINES_LIGHT)
#define MYTHIC_PREFIX_POWER_ARMOR_DEATH_RESISTANCE  (1UL << MYTHIC_PREFIX_POWER_INDEX_ARMOR_DEATH_RESISTANCE)
#define MYTHIC_PREFIX_POWER_ARMOR_DRAIN_RESISTANCE  (1UL << MYTHIC_PREFIX_POWER_INDEX_ARMOR_DRAIN_RESISTANCE)
#define MYTHIC_PREFIX_POWER_REGENERATION            (1UL << MYTHIC_PREFIX_POWER_INDEX_REGENERATION)
#define MYTHIC_PREFIX_POWER_MANA_REGENERATION       (1UL << MYTHIC_PREFIX_POWER_INDEX_MANA_REGENERATION)
#define MYTHIC_PREFIX_POWER_CURSE_RESISTANCE        (1UL << MYTHIC_PREFIX_POWER_INDEX_CURSE_RESISTANCE)
#define MYTHIC_PREFIX_POWER_TRIPLE_BASE_DAMAGE      (1UL << MYTHIC_PREFIX_POWER_INDEX_TRIPLE_BASE_DAMAGE)
#define MYTHIC_PREFIX_POWER_ADDED_ENCHANTABILITY    (1UL << MYTHIC_PREFIX_POWER_INDEX_ADDED_ENCHANTABILITY)
#define MYTHIC_PREFIX_POWER_UNCURSEABLE             (1UL << MYTHIC_PREFIX_POWER_INDEX_UNCURSEABLE)
#define MYTHIC_PREFIX_POWER_GREAT_STRENGTH          (1UL << MYTHIC_PREFIX_POWER_INDEX_GREAT_STRENGTH)
#define MYTHIC_PREFIX_POWER_SORCERY                 (1UL << MYTHIC_PREFIX_POWER_INDEX_SORCERY)

enum mythic_suffix_power_types {
    MYTHIC_SUFFIX_POWER_INDEX_LIGHTNESS = 0,
    MYTHIC_SUFFIX_POWER_INDEX_SORCERY,
    MYTHIC_SUFFIX_POWER_INDEX_TROLL_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_TROLL_REVIVAL_PREVENTION,
    MYTHIC_SUFFIX_POWER_INDEX_WERE_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_LYCANTHROPY_RESISTANCE,
    MYTHIC_SUFFIX_POWER_INDEX_GIANT_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_DEMON_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_ANGEL_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_OGRE_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_ORC_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_ELF_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_DRAGON_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_UNDEAD_DESTRUCTION,
    MYTHIC_SUFFIX_POWER_INDEX_SPEED,
    MYTHIC_SUFFIX_POWER_INDEX_WOUNDING,
    MYTHIC_SUFFIX_POWER_INDEX_DEFENSE,
    MYTHIC_SUFFIX_POWER_INDEX_SHARPNESS,
    MYTHIC_SUFFIX_POWER_INDEX_REACH,
    MYTHIC_SUFFIX_POWER_INDEX_LUCK,
    MYTHIC_SUFFIX_POWER_INDEX_FREE_ACTION,
    MYTHIC_SUFFIX_POWER_INDEX_FIRE_RESISTANCE,
    MYTHIC_SUFFIX_POWER_INDEX_COLD_RESISTANCE,
    MYTHIC_SUFFIX_POWER_INDEX_SHOCK_RESISTANCE,
    MYTHIC_SUFFIX_POWER_INDEX_COCKATRICE_SLAYING,
    MYTHIC_SUFFIX_POWER_INDEX_STONE_RESISTANCE,
    MYTHIC_SUFFIX_POWER_INDEX_SEARCHING,
    MYTHIC_SUFFIX_POWER_INDEX_SEE_INVISIBLE,
    MYTHIC_SUFFIX_POWER_INDEX_RETURN_TO_HAND_AFTER_THROW,
    MAX_MYTHIC_SUFFIX_POWERS
};

#define MYTHIC_SUFFIX_POWER_NONE                        0x00000000UL
#define MYTHIC_SUFFIX_POWER_LIGHTNESS                   (1UL << MYTHIC_SUFFIX_POWER_INDEX_LIGHTNESS)
#define MYTHIC_SUFFIX_POWER_SORCERY                     (1UL << MYTHIC_SUFFIX_POWER_INDEX_SORCERY)
#define MYTHIC_SUFFIX_POWER_TROLL_SLAYING               (1UL << MYTHIC_SUFFIX_POWER_INDEX_TROLL_SLAYING)
#define MYTHIC_SUFFIX_POWER_TROLL_REVIVAL_PREVENTION    (1UL << MYTHIC_SUFFIX_POWER_INDEX_TROLL_REVIVAL_PREVENTION)
#define MYTHIC_SUFFIX_POWER_WERE_SLAYING                (1UL << MYTHIC_SUFFIX_POWER_INDEX_WERE_SLAYING)
#define MYTHIC_SUFFIX_POWER_LYCANTHROPY_RESISTANCE      (1UL << MYTHIC_SUFFIX_POWER_INDEX_LYCANTHROPY_RESISTANCE)
#define MYTHIC_SUFFIX_POWER_GIANT_SLAYING               (1UL << MYTHIC_SUFFIX_POWER_INDEX_GIANT_SLAYING)
#define MYTHIC_SUFFIX_POWER_DEMON_SLAYING               (1UL << MYTHIC_SUFFIX_POWER_INDEX_DEMON_SLAYING)
#define MYTHIC_SUFFIX_POWER_ANGEL_SLAYING               (1UL << MYTHIC_SUFFIX_POWER_INDEX_ANGEL_SLAYING)
#define MYTHIC_SUFFIX_POWER_OGRE_SLAYING                (1UL << MYTHIC_SUFFIX_POWER_INDEX_OGRE_SLAYING)
#define MYTHIC_SUFFIX_POWER_ORC_SLAYING                 (1UL << MYTHIC_SUFFIX_POWER_INDEX_ORC_SLAYING)
#define MYTHIC_SUFFIX_POWER_ELF_SLAYING                 (1UL << MYTHIC_SUFFIX_POWER_INDEX_ELF_SLAYING)
#define MYTHIC_SUFFIX_POWER_DRAGON_SLAYING              (1UL << MYTHIC_SUFFIX_POWER_INDEX_DRAGON_SLAYING)
#define MYTHIC_SUFFIX_POWER_UNDEAD_DESTRUCTION          (1UL << MYTHIC_SUFFIX_POWER_INDEX_UNDEAD_DESTRUCTION)
#define MYTHIC_SUFFIX_POWER_SPEED                       (1UL << MYTHIC_SUFFIX_POWER_INDEX_SPEED)
#define MYTHIC_SUFFIX_POWER_WOUNDING                    (1UL << MYTHIC_SUFFIX_POWER_INDEX_WOUNDING)
#define MYTHIC_SUFFIX_POWER_DEFENSE                     (1UL << MYTHIC_SUFFIX_POWER_INDEX_DEFENSE)
#define MYTHIC_SUFFIX_POWER_SHARPNESS                   (1UL << MYTHIC_SUFFIX_POWER_INDEX_SHARPNESS)
#define MYTHIC_SUFFIX_POWER_REACH                       (1UL << MYTHIC_SUFFIX_POWER_INDEX_REACH)
#define MYTHIC_SUFFIX_POWER_LUCK                        (1UL << MYTHIC_SUFFIX_POWER_INDEX_LUCK)
#define MYTHIC_SUFFIX_POWER_FREE_ACTION                 (1UL << MYTHIC_SUFFIX_POWER_INDEX_FREE_ACTION)
#define MYTHIC_SUFFIX_POWER_FIRE_RESISTANCE             (1UL << MYTHIC_SUFFIX_POWER_INDEX_FIRE_RESISTANCE)
#define MYTHIC_SUFFIX_POWER_COLD_RESISTANCE             (1UL << MYTHIC_SUFFIX_POWER_INDEX_COLD_RESISTANCE)
#define MYTHIC_SUFFIX_POWER_SHOCK_RESISTANCE            (1UL << MYTHIC_SUFFIX_POWER_INDEX_SHOCK_RESISTANCE)
#define MYTHIC_SUFFIX_POWER_COCKATRICE_SLAYING          (1UL << MYTHIC_SUFFIX_POWER_INDEX_COCKATRICE_SLAYING)
#define MYTHIC_SUFFIX_POWER_STONE_RESISTANCE            (1UL << MYTHIC_SUFFIX_POWER_INDEX_STONE_RESISTANCE)
#define MYTHIC_SUFFIX_POWER_SEARCHING                   (1UL << MYTHIC_SUFFIX_POWER_INDEX_SEARCHING)
#define MYTHIC_SUFFIX_POWER_SEE_INVISIBLE               (1UL << MYTHIC_SUFFIX_POWER_INDEX_SEE_INVISIBLE)
#define MYTHIC_SUFFIX_POWER_RETURN_TO_HAND_AFTER_THROW  (1UL << MYTHIC_SUFFIX_POWER_INDEX_RETURN_TO_HAND_AFTER_THROW)


extern NEARDATA struct mythic_definition mythic_prefix_qualities[MAX_MYTHIC_PREFIXES];
extern NEARDATA struct mythic_definition mythic_suffix_qualities[MAX_MYTHIC_SUFFIXES];
extern NEARDATA struct mythic_power_definition mythic_prefix_powers[MAX_MYTHIC_PREFIX_POWERS];
extern NEARDATA struct mythic_power_definition mythic_suffix_powers[MAX_MYTHIC_SUFFIX_POWERS];


#define otyp_non_mythic(otyp) \
    ((objects[otyp].oc_flags4 & O4_NON_MYTHIC) != 0 || objects[otyp].oc_magic) /* Inherently (already special) magical items cannot be made mythical, this is just for normal boring objects */

#define can_obj_have_mythic(o) \
    (!otyp_non_mythic((o)->otyp) && (is_weapon(o) || (o)->oclass == ARMOR_CLASS))

#define mythic_power_applies_to_obj(o, pwrflags) \
    (!(!is_weapon(o) && ((pwrflags) & MYTHIC_POWER_FLAG_WEAPON_ONLY) != 0) && \
     !((o)->oclass != ARMOR_CLASS && ((pwrflags) & MYTHIC_POWER_FLAG_ARMOR_ONLY) != 0) && \
     !((is_missile(o) || is_ammo(o)) && ((pwrflags) & MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO) != 0) && \
     !(!is_missile(o) && ((pwrflags) & MYTHIC_POWER_FLAG_THROWN_WEAPONS_ONLY) != 0) && \
     !(((o)->oclass != ARMOR_CLASS || objects[(o)->otyp].oc_armor_category != ARM_SUIT) && ((pwrflags) & MYTHIC_POWER_FLAG_SUIT_ONLY) != 0) && \
     !(((o)->oclass != ARMOR_CLASS || objects[(o)->otyp].oc_armor_category != ARM_HELM) && ((pwrflags) & MYTHIC_POWER_FLAG_HELMET_ONLY) != 0) && \
     !(((o)->oclass != ARMOR_CLASS || objects[(o)->otyp].oc_armor_category != ARM_SHIELD) && ((pwrflags) & MYTHIC_POWER_FLAG_SHIELD_ONLY) != 0) )

#define has_obj_mythic_prefix_power(o, pwrindex) \
    ((mythic_prefix_qualities[(o)->mythic_prefix].mythic_powers & (1UL << (pwrindex))) != 0 && mythic_power_applies_to_obj(o, mythic_prefix_powers[(pwrindex)].power_flags))

#define has_obj_mythic_suffix_power(o, pwrindex) \
    ((mythic_suffix_qualities[(o)->mythic_suffix].mythic_powers & (1UL << (pwrindex))) != 0 && mythic_power_applies_to_obj(o, mythic_suffix_powers[(pwrindex)].power_flags))

#define has_obj_mythic_lightness(o)             has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_LIGHTNESS)
#define has_obj_mythic_spellcasting(o)          (has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_SORCERY) || has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_SORCERY))
#define has_obj_mythic_level_drain(o)           has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_LEVEL_DRAIN)
#define has_obj_mythic_mana_gain_25(o)          has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_MANA_GAIN_25)
#define has_obj_mythic_hp_gain_25(o)            has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_HP_GAIN_25)
#define has_obj_mythic_wounding(o)              has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_WOUNDING)
#define mythic_wounding_amount(o)               (d(1, 4) + (o)->enchantment)
#define has_obj_mythic_defense(o)               has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_DEFENSE)
#define has_obj_mythic_sharpness(o)             has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_SHARPNESS)
#define has_obj_mythic_life_draining(o)         has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_LIFE_DRAINING)
#define mythic_life_draining_amount(o)          (d(2, 6) + (o)->enchantment) 
#define has_obj_mythic_magical_light(o)         has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_SHINES_LIGHT)
#define has_obj_mythic_reach(o)                 has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_REACH)
#define has_obj_mythic_luck(o)                  has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_LUCK)
#define has_obj_mythic_triple_base_damage(o)    has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_TRIPLE_BASE_DAMAGE)
#define has_obj_mythic_added_enchantability(o)  has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_ADDED_ENCHANTABILITY)
#define has_obj_mythic_return_to_hand(o)        has_obj_mythic_suffix_power(o, MYTHIC_SUFFIX_POWER_INDEX_RETURN_TO_HAND_AFTER_THROW)
#define has_obj_mythic_uncurseable(o)           has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_UNCURSEABLE)
#define has_obj_mythic_great_strength(o)        has_obj_mythic_prefix_power(o, MYTHIC_PREFIX_POWER_INDEX_GREAT_STRENGTH)

/* Manuals */
enum manual_types
{
    MANUAL_WANDS_101 = 0,
    MANUAL_ARMOR_101,
    MANUAL_WEAPONS_101,
    MANUAL_GRAY_STONES_101,
    MANUAL_BASICS_OF_KICKING,
    MANUAL_BASICS_OF_ENCHANTMENT,
    MANUAL_BASICS_OF_EATING_AND_DRINKING,
    MANUAL_INTRODUCTION_TO_DANGEROUS_MONSTERS,
    MANUAL_INTRODUCTION_TO_BLESSED_AND_CURSED_ITEMS,
    MANUAL_GUIDE_TO_PRAYING,
    MANUAL_ESSENTIAL_SURVIVAL_TIPS,
    MANUAL_ASSORTED_TIPS_AND_TRICKS,
    MANUAL_ELEMENTARY_MECHANICS,
    MANUAL_PRINCIPLES_OF_MAGIC,
    MANUAL_UNDERSTANDING_PETS_AND_HIRELINGS,
    MANUAL_ITEM_IDENTIFICATION_101,
    MANUAL_ITEM_IDENTIFICATION_102,
    /* Non-randomly generated below */
    MANUAL_GUIDE_TO_DRAGON_SCALE_MAILS,
    MANUAL_GUIDE_TO_ALTARS_AND_SACRIFICE,
    MANUAL_SECRETS_OF_SCARE_MONSTER,
    MANUAL_GURATHULS_GUIDE_TO_ASCENSION,
    MANUAL_MASTER_CLASS_IN_WANDS,
    MANUAL_INFERNAL_INHABITANTS_OF_GEHENNOM,
    MANUAL_ADVANCED_READING_IN_KNOWN_MONSTERS,
    MAX_MANUAL_TYPES
};

/* Flags for get_obj_location(). */
#define CONTAINED_TOO 0x1
#define BURIED_TOO 0x2

/* object erosion types */
#define ERODE_BURN 0
#define ERODE_RUST 1
#define ERODE_ROT 2
#define ERODE_CORRODE 3

/* erosion flags for erode_obj() */
#define EF_NONE 0
#define EF_GREASE 0x1  /* check for a greased object */
#define EF_DESTROY 0x2 /* potentially destroy the object */
#define EF_VERBOSE 0x4 /* print extra messages */
#define EF_PAY 0x8     /* it's the player's fault */

/* erosion return values for erode_obj(), water_damage() */
#define ER_NOTHING 0   /* nothing happened */
#define ER_GREASED 1   /* protected by grease */
#define ER_DAMAGED 2   /* object was damaged in some way */
#define ER_DESTROYED 3 /* object was destroyed */

/* propeller method for potionhit() */
#define POTHIT_HERO_BASH   0 /* wielded by hero */
#define POTHIT_HERO_THROW  1 /* thrown by hero */
#define POTHIT_MONST_THROW 2 /* thrown by a monster */
#define POTHIT_OTHER_THROW 3 /* propelled by some other means [scatter()] */

#define GLOB_SMALL_MAXIMUM_WEIGHT   100
#define GLOB_MEDIUM_MAXIMUM_WEIGHT  300
#define GLOB_LARGE_MAXIMUM_WEIGHT   500


/*
 *  Notes for adding new oextra structures:
 *
 *     1. Add the structure definition and any required macros in an
 *          appropriate header file that precedes this one.
 *     2. Add a pointer to your new struct to oextra struct in this file.
 *     3. Add a referencing macro to this file after the newobj macro above
 *        (see ONAME, OMONST, OMIN, OLONG, or OMAILCMD for examples).
 *     4. Add a testing macro after the set of referencing macros
 *        (see has_oname(), has_omonst(), has_omin(), has_olong(),
 *        has_omailcmd() for examples).
 *     5. Create newXX(otmp) function and possibly free_XX(otmp) function
 *        in an appropriate new or existing source file and add a prototype
 *        for it to include/extern.h.  The majority of these are currently
 *        located in mkobj.c for convenience.
 *
 *        void FDECL(newXX, (struct obj *));
 *            void FDECL(free_XX, (struct obj *));
 *
 *              void
 *              newxx(otmp)
 *              struct obj *otmp;
 *              {
 *                  if (!otmp->oextra) otmp->oextra = newoextra();
 *                  if (!XX(otmp)) {
 *                      XX(otmp) = (struct XX *)alloc(sizeof(struct xx));
 *                      (void) memset((genericptr_t) XX(otmp),
 *                                 0, sizeof(struct xx));
 *                  }
 *              }
 *
 *     6. Adjust size_obj() in src/cmd.c appropriately.
 *     7. Adjust dealloc_oextra() in src/mkobj.c to clean up
 *        properly during obj deallocation.
 *     8. Adjust copy_oextra() in src/mkobj.c to make duplicate
 *        copies of your struct or data onto another obj struct.
 *     9. Adjust restobj() in src/restore.c to deal with your
 *        struct or data during a restore.
 *    10. Adjust saveobj() in src/save.c to deal with your
 *        struct or data during a save.
 */

#endif /* OBJ_H */

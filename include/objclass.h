/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-01-06 */

/* GnollHack 4.0    objclass.h    $NHDT-Date: 1547255901 2019/01/12 01:18:21 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "general.h"
#include "action.h"
#include "soundset.h"

#ifndef OBJCLASS_H
#define OBJCLASS_H

enum multigen_types {
    MULTIGEN_SINGLE        = 0,
    MULTIGEN_1D2        = 1,
    MULTIGEN_1D3        = 2,
    MULTIGEN_1D4        = 3,
    MULTIGEN_1D6        = 4,
    MULTIGEN_1D8        = 5,
    MULTIGEN_1D10        = 6,
    MULTIGEN_2D6        = 7,
    MULTIGEN_3D6        = 8,
    MULTIGEN_4D6        = 9,
    MULTIGEN_5D6        = 10,
    MULTIGEN_6D6        = 11,
    MULTIGEN_1D6_1        = 12,
    MULTIGEN_1D6_2        = 13,
    MULTIGEN_1D6_3        = 14,
    MULTIGEN_1D6_4        = 15,
    MULTIGEN_1D6_5        = 16,
    MULTIGEN_1D6_6        = 17,
    MULTIGEN_2D6_1        = 18,
    MULTIGEN_2D6_2        = 19,
    MULTIGEN_2D6_3        = 20,
    MULTIGEN_2D6_4        = 21,
    MULTIGEN_2D6_5        = 22,
    MULTIGEN_2D6_6        = 23,
    MULTIGEN_1D2_1        = 24,
    MULTIGEN_1D3_1        = 25,
    MULTIGEN_1D4_1        = 26,
    MULTIGEN_1D2_2        = 27,
    MULTIGEN_1D3_2        = 28,
    MULTIGEN_1D4_2        = 29,
    MULTIGEN_1D3_3        = 30,
    MULTIGEN_1D4_3        = 31,
    MULTIGEN_1D4_4        = 32
};

enum multishot_types {
    MULTISHOT_NONE = 0,
    MULTISHOT_LAUNCHER_MULTISHOT_BOW,
    MULTISHOT_LAUNCHER_REPEATING_CROSSBOW,
    MULTISHOT_LAUNCHER_STAFF_SLING,
    MULTISHOT_THROWN_DAGGER,
    MULTISHOT_THROWN_DART,
    MULTISHOT_THROWN_SHURIKEN,
    MULTISHOT_MELEE_DOUBLE_HEADED_FLAIL,
    MULTISHOT_MELEE_TRIPLE_HEADED_FLAIL,
    MAX_MULTISHOT_TYPES
};
extern const char* multishot_style_names[MAX_MULTISHOT_TYPES]; /* in objnam.c */


struct material_definition {
    const char* name;
    enum material_phase phase;
    enum hit_surface_types hit_surface_mapping;
    enum floor_surface_types floor_surface_mapping;
    Bitfield(flammable, 1);
    Bitfield(rustprone, 1);
    Bitfield(corrodeable, 1);
    Bitfield(rottable, 1);

    Bitfield(melts_in_fire, 1);        /* Solids melt, liquids boil */
    Bitfield(death_enchantable, 1);
    Bitfield(flimsy, 1);
    Bitfield(metallic, 1);
    Bitfield(mineral, 1);

    Bitfield(gemstone, 1);
    Bitfield(organic, 1);
    Bitfield(edible, 1);
    Bitfield(slurpable, 1);

    Bitfield(fragile, 1);
    Bitfield(destroyed_in_lava, 1);

    const char* foodword;
};
extern struct material_definition material_definitions[MAX_MATERIAL_TYPES]; /* in objnam.c */

enum obj_armor_types {
    ARM_SUIT    = 0,
    ARM_SHIELD  = 1,        /* needed for special wear function */
    ARM_HELM    = 2,
    ARM_GLOVES  = 3,
    ARM_BOOTS   = 4,
    ARM_CLOAK   = 5,
    ARM_SHIRT   = 6,
    ARM_ROBE    = 7,
    ARM_BRACERS = 8,
    MAX_ARMOR_TYPES
};
extern const char* armor_type_names[MAX_ARMOR_TYPES]; /* in objnam.c */


enum obj_weapon_types {
    WEP_SHORT_SWORD = 0,
    WEP_LONG_SWORD = 1,
    WEP_TWO_HANDED_SWORD = 2,
    WEP_BROADSWORD = 3,
    WEP_BASTARD_SWORD = 4,
    WEP_SCIMITAR = 5,
    WEP_SABER = 6,
    WEP_KATANA = 7,
    WEP_TSURUGI = 8,
    WEP_AXE = 9,
    WEP_THROWING_AXE = 10,
    WEP_TWO_HANDED_AXE = 11,
    WEP_DAGGER = 12,
    WEP_KNIFE = 13,
    WEP_MACE = 14,
    WEP_SPIKED_MACE = 15,
    WEP_FLAIL = 16,
    WEP_MORNING_STAR = 17,
    WEP_CLUB = 18,
    WEP_SPIKED_CLUB = 19,
    WEP_HAMMER = 20,
    WEP_SPIKED_HAMMER = 21,
    WEP_STAFF = 22,
    WEP_ROD = 23,
    WEP_SPEAR = 24,
    WEP_JAVELIN = 25,
    WEP_POLEARM = 26,
    WEP_TRIDENT = 27,
    WEP_DART = 28,
    WEP_SHURIKEN = 29,
    WEP_BOOMERANG = 30,
    WEP_SHORT_BOW = 31,
    WEP_LONG_BOW = 32,
    WEP_HAND_CROSSBOW = 33,
    WEP_CROSSBOW = 34,
    WEP_REPEATING_CROSSBOW = 35,
    WEP_SLING = 36,
    WEP_ARROW = 37,
    WEP_QUARREL = 38,
    WEP_SLING_BULLET = 39,
    WEP_PICK_AXE = 40,
    WEP_UNICORN_HORN = 41,
    WEP_HORN = 42,
    WEP_WORM_TOOTH = 43,
    WEP_SHOVEL = 44,
    WEP_HOOK = 45,
    WEP_LANCE = 46,
    WEP_HOSE = 47,
    WEP_FORK = 48,
    WEP_WHIP = 49,
    WEP_DOUBLE_FLAIL = 50,
    WEP_TRIPLE_FLAIL = 51,
    MAX_WEAPON_TYPES
};
extern const char* weapon_type_names[MAX_WEAPON_TYPES]; /* in objnam.c */


enum obj_food_types {
    FOODTYPE_GENERAL = 0,
    FOODTYPE_RATION = 1,
    FOODTYPE_FRUIT = 2,
    FOODTYPE_VEGETABLE = 3,
    FOODTYPE_NUTS = 4,
    FOODTYPE_SEEDS = 5,
    FOODTYPE_LEAF = 6,
    FOODTYPE_KELP = 7,
    FOODTYPE_BREAD = 8,
    FOODTYPE_CAKE = 9,
    FOODTYPE_CANDY = 10,
    FOODTYPE_MUSHROOM = 11,
    FOODTYPE_MOLD = 12,
    FOODTYPE_GLOB = 13,
    FOODTYPE_EGG = 14,
    FOODTYPE_MEAT = 15,
    FOODTYPE_JELLY = 16,
    FOODTYPE_CORPSE = 17,
    FOODTYPE_TIN = 18,
    MAX_FOOD_TYPES
};
extern const char* food_type_names[MAX_FOOD_TYPES]; /* in objnam.c */

enum obj_miscellaneous_types {
    MISC_MULTIPLE_PERMITTED = 0,
    MISC_WINGS = 1,       
    MISC_PANTS = 2,
    MISC_SKIRT = 3,
    MISC_WRIST_WATCH = 4,
    MISC_NOSERING = 5,
    MISC_HEADBAND = 6,
    MISC_EARRINGS = 7,
    MISC_IOUN_STONE = 8,
    MISC_BRACELET = 9,
    MISC_BRACERS = 10,        /* alternative implementation of bracers */
    MISC_BELT = 11,            
    MISC_EYEGLASSES = 12,        
    MISC_BLINDFOLD = 13,    /* alternative implementation of blindfold */
    MISC_SCARF = 14,
    MISC_EXTRA_ARMS = 15,
    MISC_BROOCH = 16,
    MISC_MASK = 17,
    MISC_NECKTIE = 18,
    MAX_MISC_TYPES
};

extern const char* misc_type_names[MAX_MISC_TYPES]; /* in objnam.c */
extern const char* misc_type_worn_texts[MAX_MISC_TYPES]; /* in objnam.c */

/* Note: there are only those where the description is not suitable for !dknown, others are of general tooltype */
enum obj_tool_types {
    TOOLTYPE_GENERAL = 0,
    TOOLTYPE_BOX = 1,
    TOOLTYPE_CHEST = 2,
    TOOLTYPE_BAG = 3,
    TOOLTYPE_PICK_AXE = 4,
    TOOLTYPE_SHOVEL = 5,
    TOOLTYPE_GOLF_CLUB = 6,
    TOOLTYPE_HOOK = 7,
    TOOLTYPE_HORN = 8,
    TOOLTYPE_CANDELABRUM = 9,
    TOOLTYPE_BELL = 10,
    TOOLTYPE_CANDLE = 11,
    TOOLTYPE_LAMP = 12,
    TOOLTYPE_LANTERN = 13,
    TOOLTYPE_WHISTLE = 14,
    TOOLTYPE_FLUTE = 15,
    TOOLTYPE_HARP = 16,
    TOOLTYPE_DRUM = 17,
    TOOLTYPE_SAW = 18,
    TOOLTYPE_JAR = 19,
    TOOLTYPE_CAN = 20,
    TOOLTYPE_GRAIL = 21,
    TOOLTYPE_TORCH = 22,
    MAX_TOOL_TYPES
};
extern const char* tool_type_names[MAX_TOOL_TYPES]; /* in objnam.c */

enum obj_book_types {
    BOOKTYPE_SPELLBOOK = 0,
    BOOKTYPE_BOOK,
    BOOKTYPE_NOVEL,
    BOOKTYPE_MANUAL,
    BOOKTYPE_TOME,
    BOOKTYPE_GRIMOIRE,
    BOOKTYPE_CODEX,
    MAX_BOOK_TYPES
};
extern const char* book_type_names[MAX_BOOK_TYPES]; /* in objnam.c */

enum charged_init_types {
    CHARGED_NOT_CHARGED = 0,
    CHARGED_GENERAL = 1,
    CHARGED_ALWAYS_1 = 2,
    CHARGED_ALWAYS_2 = 3,
    CHARGED_ALWAYS_3 = 4,
    CHARGED_ALWAYS_4 = 5,
    CHARGED_ALWAYS_5 = 6,
    CHARGED_WAND_NORMAL_NODIR = 7,
    CHARGED_WAND_NORMAL_DIR = 8,
    CHARGED_WAND_WISHING = 9,
    CHARGED_HORN_NORMAL = 10,
    CHARGED_BAG_OF_TRICKS = 11,
    CHARGED_CRYSTAL_BALL = 12,
    CHARGED_CAN_OF_GREASE = 13,
    CHARGED_MAGIC_MARKER = 14,
    CHARGED_1D6_6  = 15,
    CHARGED_1D15_15 = 16,
    CHARGED_1D30_30 = 17,
    CHARGED_1D45_45 = 18,
    CHARGED_1D75_75 = 19,
    CHARGED_ALWAYS_6 = 20,
    CHARGED_ALWAYS_7 = 21,
    CHARGED_ALWAYS_8 = 22,
    CHARGED_ALWAYS_9 = 23,
    CHARGED_1D8_1 = 24,
    CHARGED_LUCK_BLADE = 25,
    CHARGED_1D6_3 = 26,
    CHARGED_2D6_8 = 27,
    CHARGED_HOLY_GRAIL = 28,
    CHARGED_UNICORN_HORN = 29,
    CHARGED_1D4_4 = 30,
    CHARGED_1D20_20 = 31,
    CHARGED_1D4_1 = 32
};

enum recharging_types {
    RECHARGING_NOT_RECHARGEABLE = 0,
    RECHARGING_GENERAL,
    RECHARGING_WAND_GENERAL,
    RECHARGING_WAND_WISHING,
    RECHARGING_TOOL_GENERAL,
    RECHARGING_TOOL_SPECIAL_MAGICAL,
    RECHARGING_TOOL_NONMAGICAL,
    RECHARGING_MUSICAL_INSTRUMENT,
    RECHARGING_CAN_OF_GREASE,
    RECHARGING_UNICORN_HORN,
    RECHARGING_HOLY_SYMBOL,
    RECHARGING_CRYSTAL_BALL,
    RECHARGING_MAGIC_MARKER,
    RECHARGING_CAMERA,
    RECHARGING_BELL_OF_OPENING,
    RECHARGING_NINE_LIVES_STEALER,
    RECHARGING_RING_OF_THREE_WISHES,
    RECHARGING_LUCK_BLADE,
    RECHARGING_RING_OF_CONFLICT,
    RECHARGING_WAND_OF_ORCUS,
    RECHARGING_HOLY_GRAIL,
    RECHARGING_SALVE,
    RECHARGING_CUBIC_GATE,
    RECHARGING_HOWLING_FLAIL,
    MAX_RECHARGING_TYPES
};

enum enchantment_init_types {
    ENCHTYPE_NO_ENCHANTMENT = 0,
    ENCHTYPE_GENERAL_WEAPON,
    ENCHTYPE_GENERAL_ARMOR,
    ENCHTYPE_ELVEN_ARMOR,
    ENCHTYPE_LEATHER_NONBODY_ARMOR,
    ENCHTYPE_LEATHER_BODY_ARMOR,
    ENCHTYPE_ALWAYS_1,
    ENCHTYPE_ALWAYS_2,
    ENCHTYPE_ALWAYS_3,
    ENCHTYPE_ALWAYS_4,
    ENCHTYPE_ALWAYS_5,
    ENCHTYPE_RING_NORMAL,
    ENCHTYPE_RING_1_7,
    ENCHTYPE_RING_DOUBLE,
    ENCHTYPE_RING_POWER,
    ENCHTYPE_MISCELLANEOUS_NORMAL,
    ENCHTYPE_GENERAL_WEAPON_ALWAYS_START_0,
    ENCHTYPE_GENERAL_ARMOR_ALWAYS_START_0,
    ENCHTYPE_PROJECTILE,
    ENCHTYPE_THROWN_WEAPON,
    ENCHTYPE_SINGLE_HANDED_MELEE_WEAPON,
    ENCHTYPE_TWO_HANDED_MELEE_WEAPON,
    ENCHTYPE_SINGLE_HANDED_LAUNCHER,
    ENCHTYPE_TWO_HANDED_LAUNCHER,
    ENCHTYPE_TWO_HANDED_ELVEN_LAUNCHER,
    ENCHTYPE_SINGLE_HANDED_ELVEN_MELEE_WEAPON,
    ENCHTYPE_PROJECTILE_ALWAYS_START_0,
    ENCHTYPE_GENERAL_WEAPON_ARMOR,
    MAX_ENCHTYPES
};

enum material_init_types {
    MATINIT_NORMAL = 0,
};


struct objclass {
    short oc_name_idx;              /* index of actual name */
    short oc_descr_idx;             /* description when name unknown */
    char *oc_uname;                 /* called by user */
    Bitfield(oc_name_known, 1);     /* discovered */
    Bitfield(oc_merge, 1);          /* merge otherwise equal objects */
    Bitfield(oc_uses_known, 1);     /* obj->known affects full description;
                                       otherwise, obj->dknown and obj->bknown
                                       tell all, and obj->known should always
                                       be set for proper merging behavior. */
    Bitfield(oc_pre_discovered, 1); /* Already known at start of game;
                                       won't be listed as a discovery. */
    Bitfield(oc_magic, 1);          /* inherently magical object */
    uchar oc_enchantable;           /* Uses +X statistic (enchantment) */
    uchar oc_charged;               /* may have +n or (n) charges */
    uchar oc_recharging;            /* recharging type */
    Bitfield(oc_unique, 1);         /* special one-of-a-kind object */
    Bitfield(oc_nowish, 1);         /* cannot wish for this object */

    Bitfield(oc_big, 1);
#define oc_bimanual oc_big /* for weapons & tools used as weapons */
#define oc_bulky oc_big    /* for armor */
    Bitfield(oc_tough, 1); /* hard gems/rings */

    Bitfield(oc_dir, 4);
#define DIR_NOTDEF 0     /* for wands/spells:      not defined */
#define NODIR 1     /* for wands/spells:      non-directional */
#define IMMEDIATE 2 /*                        directional at one target */
#define RAY 3       /*                        zap beams */
#define TARGETED 4  /*                        targeted using throwspell */
#define TOUCH 5     /*                        like immediate but touch distance */
#define IMMEDIATE_MULTIPLE_TARGETS 6 /*       directional but many targets in a line like a ray */
#define IMMEDIATE_ONE_TO_THREE_TARGETS 7 /*   directional but 1 target for cursed, 2 targets for uncursed and 3 targets for blessed in a line like a ray */
#define IMMEDIATE_ONE_TO_SEVEN_TARGETS 8 /*   directional but 1 target for cursed, 4 targets for uncursed and 7 targets for blessed in a line like a ray */
#define IMMEDIATE_TWO_TO_SIX_TARGETS 9 /*     directional but 2 target for cursed, 4 targets for uncursed and 6 targets for blessed in a line like a ray */

#define IMMEDIATE_ITEM_ARROW 1
#define IMMEDIATE_NONE 0
#define NODIR_NONE 0
#define TOUCH_NONE 0
#define TOUCH_NONE 0
#define NOTDEF_NONE 0

#define PIERCE 1 /* for weapons & tools used as weapons */
#define SLASH 2  /* (latter includes iron ball & chain) */
#define WHACK 0
    /* 4 free bits */

    uchar oc_material_init_type;
    uchar oc_material; /* one of obj_material_types */

#define is_organic(otmp) (material_definitions[(otmp)->material].organic != 0) //(objects[otmp->otyp].oc_material <= MAT_WOOD)
#define is_slurpable(otmp) (material_definitions[(otmp)->material].slurpable != 0)
#define melts_in_lava(otmp) (material_definitions[(otmp)->material].destroyed_in_lava != 0)
#define is_metallic(otmp)  (material_definitions[(otmp)->material].metallic != 0) 
#define is_fragile(otmp)  (material_definitions[(otmp)->material].fragile != 0) 
#define is_obj_stony(otmp)  (material_definitions[(otmp)->material].mineral != 0) 

/* primary damage: fire/rust/--- */
/* is_flammable(otmp), is_rottable(otmp) in mkobj.c */
#define is_rustprone(otmp) ((material_definitions[(otmp)->material].rustprone != 0) && !(objects[(otmp)->otyp].oc_flags & O1_RUST_RESISTANT))

/* secondary damage: rot/acid/acid */
#define is_corrodeable(otmp)                   \
    ((material_definitions[(otmp)->material].corrodeable != 0) && !(objects[(otmp)->otyp].oc_flags & O1_CORROSION_RESISTANT))

#define is_damageable(otmp)                                        \
    (is_rustprone(otmp) || is_flammable(otmp) || is_rottable(otmp) \
     || is_corrodeable(otmp))

    /* 3 free bits */

    schar oc_subtyp;             /* armors: armor category, weapons: weapon category, miscellaneous magic items: subclass, etc.*/
    schar oc_skill;              /* Skills of weapons, spellbooks, tools, gems */
    uchar oc_oprop, oc_oprop2, oc_oprop3; /* properties (invis, &c.) conveyed */
    unsigned long oc_pflags;     /* Power and property flags */

#define P1_NONE                                                    0x00000000UL
#define P1_POWER_1_APPLIES_TO_ALL_CHARACTERS                       0x00000001UL  
#define P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY        0x00000002UL  
#define P1_POWER_1_APPLIES_WHEN_CARRIED                            0x00000004UL 
#define P1_POWER_2_APPLIES_TO_ALL_CHARACTERS                       0x00000008UL  
#define P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY        0x00000010UL  
#define P1_POWER_2_APPLIES_WHEN_CARRIED                            0x00000020UL  
#define P1_POWER_3_APPLIES_TO_ALL_CHARACTERS                       0x00000040UL  
#define P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY        0x00000080UL  
#define P1_POWER_3_APPLIES_WHEN_CARRIED                            0x00000100UL  
#define P1_HP_BONUS_APPLIES_TO_ALL_CHARACTERS                      0x00000200UL  
#define P1_HP_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY       0x00000400UL  
#define P1_HP_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS           0x00000800UL  
#define P1_HP_BONUS_APPLIES_WHEN_CARRIED                           0x00001000UL  
#define P1_MANA_BONUS_APPLIES_TO_ALL_CHARACTERS                    0x00002000UL  
#define P1_MANA_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY     0x00004000UL  
#define P1_MANA_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS         0x00008000UL  
#define P1_MANA_BONUS_APPLIES_WHEN_CARRIED                         0x00010000UL  
#define P1_ATTRIBUTE_BONUS_APPLIES_TO_ALL_CHARACTERS               0x00020000UL 
#define P1_ATTRIBUTE_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY 0x00040000UL  
#define P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS    0x00080000UL  
#define P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED                    0x00100000UL  
#define P1_LUCK_APPLIES_TO_ALL_CHARACTERS                          0x00200000UL 
#define P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY           0x00400000UL  
#define P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS               0x00800000UL  
#define P1_LUCK_APPLIES_WHEN_CARRIED                               0x01000000UL  
/* free bit */
/* free bit */
#define P1_CURSED_ITEM_YIELDS_NEGATIVE                             0x08000000UL  
#define P1_CONFERS_LUCK                                            0x10000000UL            /* Adds one positive luck bonus; these need to be flags instead of props because they stack */
#define P1_CONFERS_UNLUCK                                          0x20000000UL            /* Adds one negative luck bonus (the same as cursed luckstone) */
#define P1_MANA_PERCENTAGE_BONUS                                   0x40000000UL
#define P1_HP_PERCENTAGE_BONUS                                     0x80000000UL


    char  oc_class; /* object class (enum obj_class_types) */
    schar oc_delay; /* delay when using such an object */
    uchar oc_color; /* color of the object */

    short oc_prob;             /* probability, used in mkobj() */
    unsigned int oc_weight;    /* encumbrance (1 oz = 1/16 lb.) previously (1 cn = 0.1 lb.) */
    unsigned int oc_nutrition; /* food value */

    long oc_cost;              /* base cost in shops */
    
    /* Check the AD&D rules!  The FIRST is small monster damage. */
    /* for weapons, and tools, rocks, and gems useful as weapons */
    short oc_damagetype;                        /* Type of damage caused by the (magic) weapon, the same as for monster attacks */
    short oc_wsdice, oc_wsdam, oc_wsdmgplus;    /* small monster damage, also used for spell damage */
    short oc_wldice, oc_wldam, oc_wldmgplus;    /* large monster damage, also used for duration for spells */
    short oc_extra_damagetype;                  /* Type of extra damage caused by the (magic) weapon */
    short oc_wedice, oc_wedam, oc_wedmgplus;    /* extra damage used as a special effect influenced by target permissions mask */
    unsigned long oc_aflags, oc_aflags2;        /* attack related flags, e.g. whether the attack is vorpal */

/* Attack flags for weapons, armor, weapon-like tools, and miscellaneous items */
#define A1_NONE                                                      0x00000000UL
#define A1_CRITICAL_STRIKE                                           0x00000001UL  /* extra damage is caused only by chance determined by oc_critical_strike_percentage */
#define A1_CRITICAL_STRIKE_IS_DEADLY                                 0x00000002UL  
        /* successful critical strike causes lethal damage. If this flag is on, critical strike is considered not to be connected with extra damage; rather, it is treated similarly to vorpal attack */
#define A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK                    0x00000004UL  /* lethal damage is death magic */
#define A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK           0x00000008UL  /* lethal damage is disintegration */
#define A1_DEADLY_CRITICAL_STRIKE_USES_EXTRA_DAMAGE_TYPE (A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)  
        /* lethal damage is of extra damage type; note that normal critical strike always follows extra_damagetype */
#define A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK (A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)  
#define A1_DEADLY_CRITICAL_STRIKE_ONE_FOURTH_MAX_HP_DAMAGE_TO_UNIQUE_MONSTERS 0x00000010UL  /* Unique monsters suffer damage equal to 25% of their hit points instead of being slain */
#define A1_CRITICAL_STRIKE_DISRESPECTS_TARGETS                       0x00000020UL  
#define A1_CRITICAL_STRIKE_DISRESPECTS_CHARACTERS                    0x00000040UL  
#define A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES   0x00000080UL  /* All other special types use critical strike probability instead of being certain or using their own standard probability */
#define A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL                  0x00000100UL  /* The number specified by oc_critical_strike_percentage is a die roll on d20 (X or less, e.g., 1 = 5% chance or 2 = 10% chance on any attack regardless of actual hit chance) */

#define A1_WOUNDING                                                  0x00000200UL  /* extra damage caused is permanent damage */
#define A1_WOUNDING_DISRESPECTS_TARGETS                              0x00000400UL  
#define A1_WOUNDING_DISRESPECTS_CHARACTERS                           0x00000800UL  

#define A1_LIFE_LEECH                                                0x00001000UL  /* heals hit points equal to the extra damage caused */
#define A1_LIFE_LEECH_DISRESPECTS_TARGETS                            0x00002000UL  
#define A1_LIFE_LEECH_DISRESPECTS_CHARACTERS                         0x00004000UL  

#define A1_SHARPNESS                                                 0x00008000UL   /* 3/20 chance of the monster losing 15% of maximum hit points */
#define A1_VORPAL                                                    0x00010000UL   /* 1/20 chance of the monster being beheaded */
#define A1_BISECT (A1_SHARPNESS | A1_VORPAL)                                        /* 1/20 chance of a small monster being bisected and a big monster losing 50% of maximum hit points */
#define A1_SVB_MASK (A1_SHARPNESS | A1_VORPAL)
#define A1_VORPAL_LIKE_DISRESPECTS_TARGETS                           0x00020000UL
#define A1_VORPAL_LIKE_DISRESPECTS_CHARACTERS                        0x00040000UL

#define A1_LEVEL_DRAIN                                               0x00080000UL   /* drains a level from monsters */
#define A1_LEVEL_DRAIN_DISRESPECTS_TARGETS                           0x00100000UL  
#define A1_LEVEL_DRAIN_DISRESPECTS_CHARACTERS                        0x00200000UL  
/* free bit */
/* free bit */

/* General */
#define A1_MAGIC_RESISTANCE_PROTECTS                                 0x01000000UL
#define A1_BYPASSES_MC                                               0x02000000UL  /* No MC check */  
#define A1_EXTRA_DAMAGE_DISRESPECTS_TARGETS                          0x04000000UL  
#define A1_EXTRA_DAMAGE_DISRESPECTS_CHARACTERS                       0x08000000UL  
#define A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA                          0x10000000UL  /* abilities such as wounding and life leech are based on full caused damage, not just extra damage */
#define A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS                  0x20000000UL
#define A1_ITEM_VANISHES_ON_HIT                                      0x40000000UL
#define A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET                    0x80000000UL


/* More attack flags */
#define A2_NONE                                                      0x00000000UL
#define A2_HITS_ADJACENT_SQUARES                                     0x00000001UL  /* like Cleaver */
#define A2_REQUIRES_AND_EXPENDS_A_CHARGE                             0x00000002UL  /* Nine lives stealer */
#define A2_REQUIRES_ARTIFACT_INVOKE_ON                               0x00000004UL  /* Nine lives stealer */


#define oc_spell_flags oc_aflags
#define oc_spell_effect_flags oc_aflags2
#define oc_potion_effect_flags oc_aflags2
#define oc_scroll_effect_flags oc_aflags2

/* Spell flags for spells, scrolls, potions, spell-like tools, and wands */
#define S1_NONE                                  0x00000000UL
#define S1_SPELL_EXPLOSION_EFFECT                0x00000001UL
#define S1_SPELL_BYPASSES_MAGIC_RESISTANCE       0x00000002UL
#define S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY    0x00000004UL
#define S1_SPELL_IS_NONREVERSIBLE_PERMANENT      0x00000008UL
#define S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT       0x00000010UL
#define S1_NO_SOMATIC_COMPONENT                  0x00000020UL
#define S1_NO_VERBAL_COMPONENT                   0x00000040UL
#define S1_DOES_NOT_TAKE_A_TURN                  0x00000080UL
#define S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE 0x00000100UL
#define S1_LDMG_IS_PER_LEVEL_DMG_INCREASE        0x00000200UL

#define S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE                        0x00000400UL
#define S1_FLAGS_EFFECT_DOES_NOT_USE_MAGIC_RESISTANCE                0x00000800UL

#define S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE_MASK        (S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE | S1_FLAGS_EFFECT_DOES_NOT_USE_MAGIC_RESISTANCE)

#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_STR                    0x00001000UL
#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_DEX                    0x00002000UL
#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_CON                    0x00004000UL
#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_INT                    0x00008000UL
#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_WIS                    0x00010000UL
#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_CHA                    0x00020000UL
#define S1_FLAGS_EFFECT_USES_SAVING_THROW_VS_MAGIC_CANCELLATION     0x00040000UL

#define S1_FLAGS_EFFECT_USES_SAVING_THROW_MASK        0x000FF000UL

#define S2_NONE                                       0x00000000UL
#define POTFLAGS_NONE                                 0x00000000UL
#define POTFLAGS_CURSED_CURE_SICKNESS                 0x00000001UL
#define POTFLAGS_UNCURSED_CURE_SICKNESS               0x00000002UL
#define POTFLAGS_BLESSED_CURE_SICKNESS                0x00000004UL
#define POTFLAGS_NONCURSED_CURE_SICKNESS              (POTFLAGS_UNCURSED_CURE_SICKNESS | POTFLAGS_BLESSED_CURE_SICKNESS)
#define POTFLAGS_ALL_CURE_SICKNESS                    (POTFLAGS_CURSED_CURE_SICKNESS | POTFLAGS_UNCURSED_CURE_SICKNESS | POTFLAGS_BLESSED_CURE_SICKNESS)
#define POTFLAGS_CURSED_CURE_BLINDNESS                0x00000008UL
#define POTFLAGS_UNCURSED_CURE_BLINDNESS              0x00000010UL
#define POTFLAGS_BLESSED_CURE_BLINDNESS               0x00000020UL
#define POTFLAGS_NONCURSED_CURE_BLINDNESS             (POTFLAGS_UNCURSED_CURE_BLINDNESS | POTFLAGS_BLESSED_CURE_BLINDNESS)
#define POTFLAGS_ALL_CURE_BLINDNESS                   (POTFLAGS_CURSED_CURE_BLINDNESS | POTFLAGS_UNCURSED_CURE_BLINDNESS | POTFLAGS_BLESSED_CURE_BLINDNESS)
#define POTFLAGS_CURSED_CURE_HALLUCINATION            0x00000040UL
#define POTFLAGS_UNCURSED_CURE_HALLUCINATION          0x00000080UL
#define POTFLAGS_BLESSED_CURE_HALLUCINATION           0x00000100UL
#define POTFLAGS_NONCURSED_CURE_HALLUCINATION         (POTFLAGS_UNCURSED_CURE_HALLUCINATION | POTFLAGS_BLESSED_CURE_HALLUCINATION)
#define POTFLAGS_ALL_CURE_HALLUCINATION               (POTFLAGS_CURSED_CURE_HALLUCINATION | POTFLAGS_UNCURSED_CURE_HALLUCINATION | POTFLAGS_BLESSED_CURE_HALLUCINATION)
#define POTFLAGS_CURSED_CURE_STUN                     0x00000200UL
#define POTFLAGS_UNCURSED_CURE_STUN                   0x00000400UL
#define POTFLAGS_BLESSED_CURE_STUN                    0x00000800UL
#define POTFLAGS_NONCURSED_CURE_STUN                  (POTFLAGS_UNCURSED_CURE_STUN | POTFLAGS_BLESSED_CURE_STUN)
#define POTFLAGS_ALL_CURE_STUN                        (POTFLAGS_CURSED_CURE_STUN | POTFLAGS_UNCURSED_CURE_STUN | POTFLAGS_BLESSED_CURE_STUN)
#define POTFLAGS_CURSED_CURE_CONFUSION                0x00001000UL
#define POTFLAGS_UNCURSED_CURE_CONFUSION              0x00002000UL
#define POTFLAGS_BLESSED_CURE_CONFUSION               0x00004000UL
#define POTFLAGS_NONCURSED_CURE_CONFUSION             (POTFLAGS_UNCURSED_CURE_CONFUSION | POTFLAGS_BLESSED_CURE_CONFUSION)
#define POTFLAGS_ALL_CURE_CONFUSION                   (POTFLAGS_CURSED_CURE_CONFUSION | POTFLAGS_UNCURSED_CURE_CONFUSION | POTFLAGS_BLESSED_CURE_CONFUSION)

    short oc_hitbonus;                         /* weapons: "to hit" bonus */
    short oc_mc_adjustment;                    /* weapons: adjustment to any MC checks; spells and wands: MC adjustment */
    short oc_fixed_damage_bonus;               /* fixed strength-based damage bonus for crossbows; O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH must be on; can be used for other purposes for a spellbook flag */
    short oc_range;                            /* launchers: range for ammo, others throw range: >0 Fixed range, <0 Percentage of STR */

    /* general purpose */
    long oc_oc1;        /* Used for spell cooldown; weapons and armors: ac bonus */
    long oc_oc2;        /* Used for spell level; weapons and armors: mc bonus */
    long oc_oc3;        /* Used for spell mana cost; other items: mana pool bonus */
    long oc_oc4;        /* Used for spell attributes; other items: hit point bonus */
    long oc_oc5;        /* Used for spell range; non-spellbooks: specification of attributes or other properties item gives bonuses to using otmp->enchantment */
    long oc_oc6;        /* Used for spell radius; non-spellbooks: 0 => enchantment is used, otherise fixed bonus */
    long oc_oc7;        /* Used for spell casting penalty */
    long oc_oc8;        /* Used for multishot count */

/* general*/
#define oc_armor_class oc_oc1                        /* weapons and armor: AC used in ARM_AC_BONUS in do.c */
#define oc_magic_cancellation oc_oc2                 /* weapons and armor: MC, i.e., resistance level to magical attacks */
#define oc_mana_bonus oc_oc3                         /* non-spellbooks: mana pool bonus: Fixed points unless O1_MANA_PERCENTAGE_BONUS is specified */
#define oc_hp_bonus oc_oc4                           /* non-spellbooks: hit point bonus: Fixed points unless O1_HP_PERCENTAGE_BONUS is specified */
#define oc_bonus_attributes oc_oc5                   /* non-spellbooks: gives bonuses using enchantment / oc_oc6 to attributes and properties */
#define oc_attribute_bonus oc_oc6                    /* non-spellbooks: 0 => enchantment is used, otherise fixed bonus */
#define oc_spell_casting_penalty oc_oc7              /* non-spells/wands: spell casting penalty when worn */
#define oc_multishot_style oc_oc8                    /* multishot style specifying when the weapon can multishot and how many times */

#define BONUS_TO_STR            0x00000001
#define BONUS_TO_DEX            0x00000002
#define BONUS_TO_CON            0x00000004
#define BONUS_TO_INT            0x00000008
#define BONUS_TO_WIS            0x00000010
#define BONUS_TO_CHA            0x00000020
#define BONUS_TO_DAMAGE         0x00000040
#define BONUS_TO_HIT            0x00000080
#define BONUS_TO_AC             0x00000100
#define BONUS_TO_MC             0x00000200
#define BONUS_TO_UNRESTRICTED_SPELL_CASTING 0x00000400
#define BONUS_TO_EXPERIENCE     0x00000800
#define BONUS_TO_ARCHERY        0x00001000
#define BONUS_TO_ALL_SPELL_CASTING 0x00002000
#define FULL_MC_BONUS           0x00004000
    /* free bit */
#define SETS_FIXED_ATTRIBUTE    0x00010000
#define FIXED_IS_MAXIMUM        0x00020000
#define IGNORE_ENCHANTMENT      0x00040000

#define BONUS_TO_ALLSTATS BONUS_TO_STR | BONUS_TO_DEX | BONUS_TO_CON | BONUS_TO_INT | BONUS_TO_WIS | BONUS_TO_CHA


/* weapons */
/* oc_oc1 AC bonus, to make wielded objects consistent with armors, but a wielded object needs to be a weapon */
/* oc_oc2 MC bonus, to make wielded objects consistent with armors, but a wielded object needs to be a weapon */
/* oc_oc3 mana pool bonus */
/* oc_oc4 hit point bonus */
/* oc_oc5 attributes giving bonus to using enchantment / oc_oc6 to attributes and properties */
/* oc_oc6 modifier to oc_oc5: 0 => enchantment is used, otherise fixed bonus */
/* oc_oc7 spell casting penalty */
/* oc_oc8 multishot style */

/* armor */
#define oc_armor_category oc_subtyp                    /* armor: (enum obj_armor_types) */
/* oc_oc1 AC bonus */
/* oc_oc2 MC bonus */
/* oc_oc3 mana pool bonus */
/* oc_oc4 hit point bonus */
/* oc_oc5 attributes giving bonus to using enchantment / oc_oc6 to attributes and properties */
/* oc_oc6 modifier to oc_oc5: 0 => enchantment is used, otherise fixed bonus */
/* oc_oc7 spell casting penalty */
/* oc_oc8 multishot style */

/* comestibles and reagents (and other edibles) */
#define oc_edible_subtype oc_oc1        /* edibles: is rotten, poisoned, etc. */
#define oc_edible_effect oc_oc2            /* edibles: gain ability score, etc., need two, since sprig of wolfsbane is both poisonous and cures lycanthropy */

#define EDIBLETYPE_NORMAL 0 
#define EDIBLETYPE_ROTTEN 1 
#define EDIBLETYPE_SICKENING 2 
#define EDIBLETYPE_ACIDIC 3 
#define EDIBLETYPE_POISONOUS 4 
#define EDIBLETYPE_TAINTED 5 
#define EDIBLETYPE_HALLUCINATING 6 
#define EDIBLETYPE_DEADLY_POISONOUS 7 

#define EDIBLEFX_NO_EFFECT 0 
#define EDIBLEFX_GAIN_STRENGTH -1 
#define EDIBLEFX_GAIN_DEXTERITY -2 
#define EDIBLEFX_GAIN_CONSTITUTION -3 
#define EDIBLEFX_GAIN_INTELLIGENCE -4
#define EDIBLEFX_GAIN_WISDOM -5
#define EDIBLEFX_GAIN_CHARISMA -6
#define EDIBLEFX_CURE_LYCANTHROPY -7
#define EDIBLEFX_CURE_BLINDNESS -8
#define EDIBLEFX_READ_FORTUNE -9
#define EDIBLEFX_CURE_SICKNESS -10
#define EDIBLEFX_ROYAL_JELLY -11
#define EDIBLEFX_APPLE -12
#define EDIBLEFX_EGG -13
#define EDIBLEFX_RESTORE_ABILITY -14
#define EDIBLEFX_GAIN_LEVEL -15
#define EDIBLEFX_CURE_PETRIFICATION -16


/* spells, wands, scrolls */
#define oc_spell_cooldown oc_oc1         /* books: spell cooldown time */
#define oc_spell_level oc_oc2            /* books: spell level */
#define oc_spell_mana_cost oc_oc3        /* books: spell mana cost */
#define oc_spell_attribute oc_oc4        /* books: spell primary casting attribute */
#define oc_spell_range oc_oc5            /* books: spell range */
#define oc_spell_radius oc_oc6           /* books: spell radius */
#define oc_spell_skill_chance oc_oc7     /* books: % chance of gaining skill points upon casting, also % multiplier for mixing */
#define oc_spell_per_level_step oc_oc8   /* books: how many levels to get one per level bonus */
#define oc_spell_saving_throw_adjustment oc_mc_adjustment    /* books: spell saving throw adjustment */
#define oc_spell_dmg_dice oc_wsdice      /* books: spell damage no of dice */
#define oc_spell_dmg_diesize oc_wsdam    /* books: spell damage size of dice */
#define oc_spell_dmg_plus oc_wsdmgplus   /* books: spell damage constant added */
#define oc_spell_per_level_dice oc_wldice       /* books: spell per level bonus no of dice (overlaps with duration, which is not allowed) */
#define oc_spell_per_level_diesize oc_wldam     /* books: spell per level bonus size of dice (overlaps with duration, which is not allowed) */
#define oc_spell_per_level_plus oc_wldmgplus    /* books: spell per level bonus constant added (overlaps with duration, which is not allowed) */
#define oc_spell_dur_dice oc_wldice      /* books: spell duration no of dice */
#define oc_spell_dur_diesize oc_wldam    /* books: spell duration size of dice */
#define oc_spell_dur_plus oc_wldmgplus   /* books: spell duration constant added */
#define oc_spell_dur_buc_plus oc_extra_damagetype   /* books: spell duration constant added multiplied by bcsign */

/* potions */
#define oc_potion_breathe_buc_multiplier oc_oc1
#define oc_potion_normal_buc_multiplier oc_oc2
#define oc_potion_nutrition_buc_multiplier oc_oc3
#define oc_potion_extra_data1 oc_oc4
#define oc_potion_breathe_dice_buc_multiplier oc_oc5
#define oc_potion_normal_dice_buc_multiplier oc_oc6
#define oc_potion_nutrition_dice_buc_multiplier oc_oc7
#define oc_potion_saving_throw_adjustment oc_mc_adjustment    /* potions: potion saving throw adjustment */
#define oc_potion_breathe_dice oc_wsdice                      /* potions: potion breathe duration no of dice */
#define oc_potion_breathe_diesize oc_wsdam                    /* potions: potion breathe duration size of dice */
#define oc_potion_breathe_plus oc_wsdmgplus                   /* potions: potion breathe duration constant added */
/* Note potion duration data locations are aligned with spell duration data locations for simplicity */
#define oc_potion_normal_dice oc_spell_dur_dice               /* potions: potion quaff duration no of dice */
#define oc_potion_normal_diesize oc_spell_dur_diesize         /* potions: potion quaff duration size of dice */
#define oc_potion_normal_plus oc_spell_dur_plus               /* potions: potion quaff duration constant added */
#define oc_potion_nutrition_dice oc_wedice                    /* potions: potion nutrition no of dice */
#define oc_potion_nutrition_diesize oc_wedam                  /* potions: potion nutrition size of dice */
#define oc_potion_nutrition_plus oc_wedmgplus                 /* potions: potion nutrition constant added */

/* scrolls */
#define oc_scroll_dmg_dice oc_spell_dmg_dice                  /* scrolls: effect damage no of dice */
#define oc_scroll_dmg_diesize oc_spell_dmg_diesize            /* scrolls: effect damage size of dice */
#define oc_scroll_dmg_plus oc_spell_dmg_plus                  /* scrolls: effect damage constant added */
#define oc_scroll_dur_dice oc_spell_dur_dice                  /* scrolls: effect duration no of dice */
#define oc_scroll_dur_diesize oc_spell_dur_diesize            /* scrolls: effect duration size of dice */
#define oc_scroll_dur_plus oc_spell_dur_plus                  /* scrolls: effect duration constant added */

/* wands */
#define oc_wand_dmg_dice oc_spell_dmg_dice                    /* wands: effect damage no of dice */
#define oc_wand_dmg_diesize oc_spell_dmg_diesize              /* wands: effect damage size of dice */
#define oc_wand_dmg_plus oc_spell_dmg_plus                    /* wands: effect damage constant added */
#define oc_wand_dur_dice oc_spell_dur_dice                    /* wands: effect duration no of dice */
#define oc_wand_dur_diesize oc_spell_dur_diesize              /* wands: effect duration size of dice */
#define oc_wand_dur_plus oc_spell_dur_plus                    /* wands: effect duration constant added */

    int oc_dir_subtype;               /* spells: ID for type of ray or immediate effect, weapons: damage type */

    int oc_material_components;       /* spells: ID for material component list for a spell or to make the item (if recipe is known), other items: ID of components produced when breaking */
    int oc_item_cooldown;             /* cooldown before the item can be used / applied / zapped / read etc. again */
    int oc_special_quality;           /* special quality, e.g. maximum number of candles in the candelabrum */

    unsigned long oc_flags;         /* E.g. if indestructible or disintegration resistant */
    unsigned long oc_flags2;        /* More flags */
    unsigned long oc_flags3;        /* Even more flags */
    unsigned long oc_flags4;        /* Even more flags */
    unsigned long oc_flags5;        /* Even more flags */
    unsigned long oc_flags6;        /* Even more flags */

    unsigned long oc_power_permissions;   /* roles, races, genders, and alignments that the item's powers are conferred to */
    unsigned long oc_target_permissions;  /* symbol, M1 flag, M2 flag, M3 flag, etc. for which extra damage is deal to */
    int oc_critical_strike_percentage;    /* percentage to be used with A1_CRITICAL_STRIKE; can be used for other purposes for a S1_ flag, too */
#define oc_effect_probability oc_critical_strike_percentage /* comestibles: probability of edible effect taking place */
    int oc_multigen_type;                 /* class number multi multigen_type */
    enum object_soundset_types oc_soundset;

/* oc_dir_subtypes for spells */
#define RAY_WND_MAGIC_MISSILE 0 
#define RAY_WND_FIRE 1 
#define RAY_WND_COLD 2 
#define RAY_WND_SLEEP 3 
#define RAY_WND_DISINTEGRATION 4 
#define RAY_WND_LIGHTNING 5 
#define RAY_WND_POISON_GAS 6 
#define RAY_WND_ACID 7 
#define RAY_WND_DEATH 8 
#define RAY_WND_PETRIFICATION 9 
#define RAY_WND_DIGGING 101 
#define RAY_WND_EVAPORATION 102 

#define IMMEDIATE_MAGIC_MISSILE_NONBEAM -1 
#define IMMEDIATE_FIRE_NONBEAM_NONBEAM -2
#define IMMEDIATE_COLD_NONBEAM -3 
#define IMMEDIATE_SLEEP_NONBEAM -4 
#define IMMEDIATE_DISINTEGRATION_NONBEAM -5 
#define IMMEDIATE_LIGHTNING_NONBEAM -6 
#define IMMEDIATE_POISON_GAS_NONBEAM -7 
#define IMMEDIATE_ACID_NONBEAM -8 
#define IMMEDIATE_DEATH_NONBEAM -9 
#define IMMEDIATE_PETRIFICATION_NONBEAM -10 

#define IMMEDIATE_MAGIC_MISSILE_BEAM -11 
#define IMMEDIATE_FIRE_NONBEAM_BEAM -12
#define IMMEDIATE_COLD_BEAM -13 
#define IMMEDIATE_SLEEP_BEAM -14 
#define IMMEDIATE_DISINTEGRATION_BEAM -15 
#define IMMEDIATE_LIGHTNING_BEAM -16 
#define IMMEDIATE_POISON_GAS_BEAM -17 
#define IMMEDIATE_ACID_BEAM -18 
#define IMMEDIATE_DEATH_BEAM -19 
#define IMMEDIATE_PETRIFICATION_BEAM -20 

#define IMMEDIATE_FLASHED_LIGHT -21 

#define RAY_MAGIC_MISSILE 10 
#define RAY_FIRE 11 
#define RAY_COLD 12 
#define RAY_SLEEP 13 
#define RAY_DISINTEGRATION 14 
#define RAY_LIGHTNING 15 
#define RAY_POISON_GAS 16 
#define RAY_ACID 17 
#define RAY_DEATH 18 
#define RAY_PETRIFICATION 19 
#define RAY_DIGGING 111 
#define RAY_EVAPORATION 112 


/* Item resistances -- General: cannot be overriden */
#define O1_NONE                              0x00000000UL 
#define O1_INDESTRUCTIBLE                    0x00000001UL 
#define O1_DISINTEGRATION_RESISTANT          0x00000002UL 
#define O1_FIRE_RESISTANT                    0x00000004UL 
#define O1_COLD_RESISTANT                    0x00000008UL 
#define O1_LIGHTNING_RESISTANT               0x00000010UL
#define O1_POLYMORPH_RESISTANT               0x00000020UL
#define O1_RUST_RESISTANT                    0x00000040UL
#define O1_CORROSION_RESISTANT               0x00000080UL
#define O1_ROT_RESISTANT                     0x00000100UL

#define O1_ENCHANTMENT_AFFECTS_MC            0x00000200UL            /* +X of the item influences also its MC */
#define O1_EROSION_DOES_NOT_AFFECT_MC        0x00000400UL            /* erosion level does not affect the item's MC */
#define O1_ENCHANTMENT_DOES_NOT_AFFECT_AC    0x00000800UL            /* +X of the item does not affect its AC bonus */
#define O1_EROSION_DOES_NOT_AFFECT_AC        0x00001000UL            /* erosion level does not affect the item's MC */
#define O1_IS_ARMOR_WHEN_WIELDED             0x00002000UL            /* acts as an armor when wielded giving AC using oc_armor_class, which must be specified */
#define O1_IS_WEAPON_WHEN_WIELDED            0x00004000UL            /* acts as a weapon when wielded (or worn in shield slot in two-weapon fighting) using damage statistics */
#define O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT 0x00008000UL            /* +X of the item influences also its MC adjustment (i.e., saving throw penalty for attacks) */

#define O1_EDIBLE_NONFOOD                    0x00010000UL

#define O1_BECOMES_CURSED_WHEN_WORN                            0x00020000UL
#define O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED           0x00040000UL
#define O1_CANNOT_BE_DROPPED_IF_CURSED                         0x00080000UL

#define O1_FLICKERS_WHEN_MONSTERS_DETECTED                     0x00100000UL
#define O1_OFLAG_POWERS_APPLY_TO_ALL_CHARACTERS                0x00200000UL
#define O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY 0x00400000UL
#define O1_OFLAG_POWERS_APPLY_WHEN_CARRIED                     0x00800000UL

#define O1_THROWN_WEAPON_ONLY                0x01000000UL            /* says "Thrown weapon" instead of "Melee weapon", default range is larger, can use throwrange data value, gets penalties in melee combat */
#define O1_MELEE_AND_THROWN_WEAPON           0x02000000UL            /* says "Melee and thrown weapon" instead of "Melee weapon", default range is larger, can use throwrange data value */
#define O1_WEIGHT_DOES_NOT_REDUCE_RANGE      0x04000000UL            /* the object magically flies when thrown, ignoring its weight */
#define O1_RETURNS_TO_HAND_AFTER_THROWING    0x08000000UL            /* the object returns to the owner's hand after throwing */
#define O1_CAN_BE_THROWN_ONLY_IF_WIELDED     0x10000000UL            /* has to be wielded to be thrown, e.g., Mjollnir */
#define O1_SPELLTOOL                         0x20000000UL            /* apply is the same as zap, uses spellbook/wand paramters and flags instead of normal flags */
#define O1_NON_SPELL_SPELLBOOK               0x40000000UL            /* uses non-spellbook flags and other non-spellbook stats */
#define O1_NOT_CURSEABLE                     0x80000000UL


/* Flags 2 */
#define O2_NONE                   0x00000000UL
#define O2_ELVEN_ITEM             0x00000001UL
#define O2_DWARVEN_ITEM           0x00000002UL
#define O2_ORCISH_ITEM            0x00000004UL
#define O2_GNOLLISH_ITEM          0x00000008UL
#define O2_DRAGON_ITEM            0x00000010UL
#define O2_DEMON_ITEM             0x00000020UL
#define O2_ANGELIC_ITEM           0x00000040UL
#define O2_MODRON_ITEM            0x00000080UL
#define O2_GNOMISH_ITEM           0x00000100UL
#define O2_UNDEAD_ITEM            0x00000200UL

#define O2_CONTAINER              0x00000400UL    
#define O2_CONTAINER_BOX          0x00000800UL    
#define O2_CONTAINER_MAGIC_BAG    0x00001000UL    
#define O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG 0x00002000UL
#define O2_CONTAINER_NONCONTAINER 0x00004000UL    

#define O2_MONSTER_SCALES         0x00008000UL    
#define O2_MONSTER_SCALE_MAIL     0x00010000UL
#define O2_CANDLE                 0x00020000UL    
#define O2_SHINES_MAGICAL_LIGHT   0x00040000UL    
#define O2_FLICKER_COLOR_RED    O2_NONE    
#define O2_FLICKER_COLOR_WHITE    0x00080000UL    
#define O2_FLICKER_COLOR_BLUE     0x00100000UL    
#define O2_FLICKER_COLOR_BLACK (O2_FLICKER_COLOR_WHITE | O2_FLICKER_COLOR_BLUE)    
#define O2_FLICKER_COLOR_MASK (O2_FLICKER_COLOR_WHITE | O2_FLICKER_COLOR_BLUE)    

#define O2_GRAYSTONE              0x00200000UL    
#define O2_ROCK                   0x00400000UL    
#define O2_ORE                    0x00800000UL    
#define O2_FLIMSY                 0x01000000UL

#define O2_SPECIAL_PRAYING_ITEM   0x02000000UL
#define O2_KEY                    0x04000000UL
#define O2_CREDIT_CARD            0x08000000UL

#define O2_GENERATED_DEATH_OR_COLD_ENCHANTED 0x10000000UL                 /* if deathenchantable item, then death, otherwise lightning*/
#define O2_GENERATED_BLESSED      0x20000000UL                            /* Always generated blessed */                                    
#define O2_GENERATED_CURSED       0x40000000UL                            /* Always generated cursed */                                
#define O2_CURSED_MAGIC_ITEM      0x80000000UL                            /* The effect of the item is negative; typically 90% chance of being cursed */                                    

/* Flags 3 */
#define O3_NONE                                        0x00000000UL
#define O3_NO_GENERATION                               0x00000001UL  /* item is not generated normally */
#define O3_NO_WISH                                     0x00000002UL  /* item is special, it cannot be wished for, mimics oc_nowish */
#define O3_UNIQUE                                      0x00000004UL  /* the item is unique, mimics oc_unique */
#define O3_UNBURIABLE                                  0x00000008UL  /* the item cannot be buried */

#define O3_DOUBLE_DIGGING_EFFORT                       0x00000010UL  /* double normal digging effort */
#define O3_BURIED_SEARCHABLE                           0x00000020UL  /* if buried, can be found by explicit searching */
#define O3_CONTENT_DESCRIPTION_SHUFFLED                0x00000040UL  /* uses (shuffled) description index instead of name index for content description */
#define O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS          0x00000080UL  /* consumes nutrition every 20 rounds when worn. WORKS ONLY FOR MISCELLANEOUS MAGIC ITEMS */
#define O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH 0x00000100UL 

#define O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS       0x00000200UL  /* wielding or wearing prohibits the revival of permitted targets */
#define O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS     0x00000400UL  /* wielding or wearing prohibits AD_XXXX-type innate summoning by permitted targets; the target can still summon using spells */

#define O3_NO_PICKUP                                   0x00000800UL  /* monsters do not pick up this item type */

#define O3_INVOKABLE                                   0x00001000UL  /* can be invoked using invoke command */
#define O3_APPLIABLE                                   0x00002000UL  /* can be applied as a tool */
#define O3_WIELDABLE                                   0x00004000UL  /* can be wielded in a weapon slot */
#define O3_READABLE                                    0x00008000UL  /* can be read */                                
#define O3_IGNITABLE                                   0x00010000UL    
#define O3_RELATIVE_AGE                                0x00020000UL    
#define O3_REFILLABLE_WITH_OIL                         0x00040000UL    
#define O3_ELEMENTAL_ENCHANTABLE                       0x00080000UL
#define O3_EATING_IDENTIFIES                           0x00100000UL    
#define O3_NONROTTING_FOOD                             0x00200000UL
#define O3_POISONABLE                                  0x00400000UL
#define O3_QUAFFABLE                                   0x00800000UL  /* can be drunk */            

#define O3_PERMTTED_TARGET_LAWFUL                      0x01000000UL
#define O3_PERMTTED_TARGET_NEUTRAL                     0x02000000UL
#define O3_PERMTTED_TARGET_CHAOTIC                     0x04000000UL

#define O3_TARGET_PERMISSION_IS_M1_FLAG                0x08000000UL  /* Note: if no flag, then default is a monster symbol */
#define O3_TARGET_PERMISSION_IS_M2_FLAG                0x10000000UL
#define O3_TARGET_PERMISSION_IS_M3_FLAG                0x20000000UL 
#define O3_TARGET_PERMISSION_IS_M4_FLAG                0x40000000UL 
#define O3_TARGET_PERMISSION_IS_M5_FLAG                0x80000000UL 



/* Flags 4 */
#define O4_NONE                        0x00000000UL
#define O4_MISSILE_TILE                0x00000001UL
#define O4_SINGLE_MISSILE_TILE         0x00000002UL
#define O4_FLOOR_TILE                  0x00000004UL
#define O4_FULL_SIZED_BITMAP           0x00000008UL
#define O4_DRAWN_IN_FRONT              0x00000010UL
#define O4_ALLOWS_DIPPING_INTO         0x00000020UL
#define O4_TETHERED_WEAPON             0x00000040UL
#define O4_EXTENDED_POLEARM_REACH      0x00000080UL
#define O4_CAN_HAVE_EXCEPTIONALITY     0x00000100UL
#define O4_GENERATED_EXCEPTIONAL       0x00000200UL
#define O4_GENERATED_ELITE             0x00000400UL
#define O4_GENERATED_CELESTIAL         0x00000800UL
#define O4_GENERATED_PRIMORDIAL        0x00001000UL
#define O4_GENERATED_INFERNAL          0x00002000UL
#define O4_NON_EXCEPTIONAL             0x00004000UL
#define O4_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS        0x00008000UL    /* deals damage when wielded like artifacts */
#define O4_INAPPROPRIATE_CHARACTERS_CANT_HANDLE            0x00010000UL
#define O4_NON_MYTHIC                                      0x00020000UL
#define O4_DOUBLE_MYTHIC_CHANCE                            0x00040000UL
#define O4_HEIGHT_IS_CLIPPING                              0x00080000UL    /* Item graphics get clipped rather than scaled by height */
#define O4_CONTAINER_CONTENTS_VISIBLE                      0x00100000UL    /* Can always see the contents */
#define O4_CONTAINER_CONTENTS_USE_ON                       0x00200000UL    /* Contents are put "on" the container rather than "in" the container  */
#define O4_CONTAINER_ACCEPTS_ONLY_SCROLLS_AND_BOOKS        0x00400000UL    /* Only books and scrolls fit in */
#define O4_CONTAINER_ACCEPTS_ONLY_WEAPONS                  0x00800000UL    /* Only weapon fit in */
#define O4_CONTAINER_HAS_LID                               0x01000000UL    /* Has a lid that will be opened permanently on first time (sarcophagus, coffin) */
#define O4_CONTAINER_PEEK_INTO                             0x02000000UL    /* You peek into this container */
#define O4_CONTAINER_MAY_CONTAIN_MONSTER                   0x04000000UL
#define O4_WEAPON_TOOL                                     0x08000000UL
#define O4_RARE                                            0x10000000UL
#define O4_VERY_RARE                                       0x20000000UL
#define O4_NO_CLASS_GENERATION_IN_SHOP                     0x40000000UL
#define O4_CANDELABRUM                                     0x80000000UL

#define O5_NONE                        0x00000000UL
#define O5_TILE_IS_TILESET_DEPENDENT   0x00000001UL
#define O5_MBAG_DESTROYING_ITEM        0x00000002UL
#define O5_CANCELLATION_NO_EXPLOSION_BUT_DRAIN 0x00000004UL
#define O5_NO_MYTHIC_RACIAL_PREFIXES   0x00000008UL
#define O5_IS_WEAPON_WHEN_WORN         0x00000010UL /* Weapon gloves only */
#define O5_FULL_NAME                   0x00000020UL /* Do not append or prepend anything to the name */
#define O5_OK_FOR_ILLITERATE           0x00000040UL /* Does not break illiterate conduct */
#define O5_NOT_CANCELLABLE             0x00000080UL /* Cannot be cancelled */
#define O5_JOUSTING_WEAPON             0x00000100UL
#define O5_HAS_ALTERNATIVE_APPEARANCE  0x00000200UL /* Alternative appearance available */
#define O5_EFFECT_IS_HEALING           0x00000400UL /* (Potion) Effect data is healing */
#define O5_EFFECT_IS_MANA              0x00000800UL /* (Potion) Effect data is mana */
#define O5_EFFECT_IS_DAMAGE            0x00001000UL /* (Potion) Effect data is damage */
#define O5_EFFECT_FOR_BLESSED_ONLY     0x00002000UL /* Effect data is for blessed potion only (monster detection, scroll of fire) */
#define O5_EFFECT_FLAGS_ARE_HEALING    0x00004000UL /* aflags2 bits determine what ailments the effect cures */
/* free bit */
#define O5_LIGHT_SOURCE                0x00010000UL
#define O5_BURNS_INFINITELY            0x00020000UL
#define O5_NO_CATALOGUE                0x00040000UL
#define O5_DOUBLE_EXCEPTIONALITY_CHANCE 0x00080000UL
#define O5_HALF_EXCEPTIONALITY_CHANCE  0x00100000UL
#define O5_CANNOT_BE_CELESTIAL         0x00200000UL
#define O5_CANNOT_BE_PRIMORDIAL        0x00400000UL
#define O5_CANNOT_BE_INFERNAL          0x00800000UL
#define O5_MIXTURE_CLEARS              0x01000000UL
#define O5_TORCH                       0x02000000UL
#define O5_LAMP                        0x04000000UL


#define O6_NONE                        0x00000000UL

/* flag values for CONFERS_POWERS_TO_SPECIFIED_CHARACTERS_ONLY in nonspell_oc7 */
#define PERMITTED_ALL                 0x00000000UL

#define PERMITTED_RACE_HUMAN          0x00000001UL
#define PERMITTED_RACE_ELF            0x00000002UL
#define PERMITTED_RACE_DWARF          0x00000004UL
#define PERMITTED_RACE_GNOLL          0x00000008UL
#define PERMITTED_RACE_GNOME          0x80000010UL
#define PERMITTED_RACE_ORC            0x00000020UL

#define PERMITTED_RACE_MASK (PERMITTED_RACE_HUMAN | PERMITTED_RACE_ELF | PERMITTED_RACE_DWARF | PERMITTED_RACE_GNOLL | PERMITTED_RACE_GNOME | PERMITTED_RACE_ORC)


#define PERMITTED_ROLE_ARCHAEOLOGIST    0x00000040UL
#define PERMITTED_ROLE_BARBARIAN        0x00000080UL
#define PERMITTED_ROLE_CAVEMAN          0x00000100UL
#define PERMITTED_ROLE_HEALER           0x00000200UL
#define PERMITTED_ROLE_KNIGHT           0x00000400UL
#define PERMITTED_ROLE_MONK             0x00000800UL
#define PERMITTED_ROLE_PRIEST           0x00001000UL
#define PERMITTED_ROLE_ROGUE            0x00002000UL
#define PERMITTED_ROLE_RANGER           0x00004000UL
#define PERMITTED_ROLE_SAMURAI          0x00008000UL
#define PERMITTED_ROLE_TOURIST          0x00010000UL
#define PERMITTED_ROLE_VALKYRIE         0x00020000UL
#define PERMITTED_ROLE_WIZARD           0x00040000UL

#define PERMITTED_ROLE_MASK (PERMITTED_ROLE_ARCHAEOLOGIST | PERMITTED_ROLE_BARBARIAN | PERMITTED_ROLE_CAVEMAN | PERMITTED_ROLE_HEALER | PERMITTED_ROLE_KNIGHT | \
    PERMITTED_ROLE_MONK | PERMITTED_ROLE_PRIEST | PERMITTED_ROLE_ROGUE | PERMITTED_ROLE_RANGER | PERMITTED_ROLE_SAMURAI | PERMITTED_ROLE_TOURIST | \
    PERMITTED_ROLE_VALKYRIE | PERMITTED_ROLE_WIZARD)

#define PERMITTED_ALIGNMENT_LAWFUL     0x00080000UL
#define PERMITTED_ALIGNMENT_NEUTRAL    0x00100000UL
#define PERMITTED_ALIGNMENT_CHAOTIC    0x00200000UL

#define PERMITTED_ALIGNMENT_MASK (PERMITTED_ALIGNMENT_LAWFUL | PERMITTED_ALIGNMENT_NEUTRAL | PERMITTED_ALIGNMENT_CHAOTIC)

#define PERMITTED_GENDER_MALE          0x00400000UL
#define PERMITTED_GENDER_FEMALE        0x00800000UL

#define PERMITTED_GENDER_MASK (PERMITTED_GENDER_MALE | PERMITTED_GENDER_FEMALE)

#define PERMITTED_CHARACTER_MASK (PERMITTED_RACE_MASK | PERMITTED_ROLE_MASK | PERMITTED_ALIGNMENT_MASK | PERMITTED_GENDER_MASK)

/* Permitted target mask */
#define ALL_TARGETS                    0x00000000UL

};

/* Special values for critical_strike_percentage */
enum critical_strike_special_percentage_type {
    CRITICAL_STRIKE_SPECIAL_PERCENTAGE_HIT_DICE_SAVES = -1,    /* Chance is 100% - 5% x Hit Dice */
};

#define MAX_CRITICAL_STRIKE_SPECIAL_PERCENTAGES (-1 * CRITICAL_STRIKE_SPECIAL_PERCENTAGE_HIT_DICE_SAVES)

extern const char* critical_strike_special_percentage_names[MAX_CRITICAL_STRIKE_SPECIAL_PERCENTAGES];

struct class_sym {
    char sym;
    const char *name;
    const char *explain;
};

struct objdescr {
    const char *oc_name;                /* actual name */
    const char *oc_descr;               /* description when name unknown */
    const char* oc_content_name;        /* true description of contents (jars) */
    const char* oc_content_description; /* unknown description of contents (spellbooks, jars) */
    const char* oc_item_description;    /* description of the item */
    int oc_tile_floor_height;           /* (scaled) height of the item tile in pixels when it appears on the floor */
    short stand_animation;
    short enlargement;
    short replacement;
};

extern NEARDATA struct objclass objects[];
extern NEARDATA struct objdescr obj_descr[];

/*
 * All objects have a class. Make sure that all classes have a corresponding
 * symbol below.
 */
enum obj_class_types {
    RANDOM_CLASS =  0, /* used for generating random objects */
    ILLOBJ_CLASS =  1,
    WEAPON_CLASS =  2,
    ARMOR_CLASS  =  3,
    RING_CLASS   =  4,
    AMULET_CLASS =  5,
    TOOL_CLASS   =  6,
    FOOD_CLASS   =  7,
    POTION_CLASS =  8,
    SCROLL_CLASS =  9,
    SPBOOK_CLASS = 10, /* actually SPELL-book */
    WAND_CLASS   = 11,
    COIN_CLASS   = 12,
    GEM_CLASS    = 13,
    ROCK_CLASS   = 14,
    BALL_CLASS   = 15,
    CHAIN_CLASS  = 16,
    VENOM_CLASS  = 17,
    REAGENT_CLASS = 18,
    MISCELLANEOUS_CLASS = 19,

    MAX_OBJECT_CLASSES  = 20
};


#define VEGETARIAN_CLASS (MAX_OBJECT_CLASSES + 1)

#define ALLOW_COUNT (MAX_OBJECT_CLASSES + 1) /* Can be used in the object class    */
#define ALL_CLASSES (MAX_OBJECT_CLASSES + 2) /* input to getobj().                 */
#define ALLOW_NONE  (MAX_OBJECT_CLASSES + 3)

#define BURNING_OIL (MAX_OBJECT_CLASSES + 1) /* Can be used as input to explode.   */
#define MON_EXPLODE (MAX_OBJECT_CLASSES + 2) /* Exploding monster (e.g. gas spore) */

#if 0 /* moved to decl.h so that makedefs.c won't see them */
extern const struct class_sym
        def_oc_syms[MAX_OBJECT_CLASSES];       /* default class symbols */
extern uchar oc_syms[MAX_OBJECT_CLASSES];      /* current class symbols */
#endif

/* Default definitions of all object-symbols (must match classes above). */

#define ILLOBJ_SYM ']' /* also used for mimics */
#define WEAPON_SYM ')'
#define ARMOR_SYM '['
#define RING_SYM '='
#define AMULET_SYM '"'
#define TOOL_SYM '('
#define FOOD_SYM '%'
#define POTION_SYM '!'
#define SCROLL_SYM '?'
#define SPBOOK_SYM '+'
#define WAND_SYM '/'
#define GOLD_SYM '$'
#define GEM_SYM '*'
#define ROCK_SYM '`'
#define BALL_SYM '0'
#define CHAIN_SYM '_'
#define VENOM_SYM '.'
#define REAGENT_SYM '9'
#define MISCELLANEOUS_SYM '8'
#define GOLD_SYM_ALTERNATE '\''

struct fruit {
    char fname[PL_FSIZ];
    int fid;
    struct fruit *nextf;
};
#define newfruit() (struct fruit *) alloc(sizeof(struct fruit))
#define dealloc_fruit(rind) free((genericptr_t)(rind))

#define OBJ_NAME(obj) (obj_descr[(obj).oc_name_idx].oc_name)
#define OBJ_DESCR(obj) (obj_descr[(obj).oc_descr_idx].oc_descr)

#define OBJ_CONTENT_NAME(otyp) (obj_descr[objects[(otyp)].oc_name_idx].oc_content_name)
#define OBJ_CONTENT_DESC(otyp) (obj_descr[is_otyp_content_description_shuffled(otyp) ? objects[(otyp)].oc_descr_idx : objects[(otyp)].oc_name_idx].oc_content_description)

#define OBJ_ITEM_DESC(otyp) (obj_descr[objects[(otyp)].oc_name_idx].oc_item_description)

#define OBJ_TILE_HEIGHT(otyp) (obj_descr[objects[(otyp)].oc_descr_idx].oc_tile_floor_height)
#define OBJ_STAND_ANIMATION(otyp) (obj_descr[objects[(otyp)].oc_descr_idx].stand_animation)

#define has_spell_otyp_per_level_bonus(otyp) \
    ((objects[otyp].oc_spell_flags & S1_LDMG_IS_PER_LEVEL_DMG_INCREASE) != 0 && objects[otyp].oc_spell_per_level_step > 0)

#endif /* OBJCLASS_H */

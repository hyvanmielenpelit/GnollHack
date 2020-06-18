/* GnollHack 4.0	objclass.h	$NHDT-Date: 1547255901 2019/01/12 01:18:21 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef OBJCLASS_H
#define OBJCLASS_H

enum multigen_types {
	MULTIGEN_SINGLE		= 0,
	MULTIGEN_1D2		= 1,
	MULTIGEN_1D3		= 2,
	MULTIGEN_1D4		= 3,
	MULTIGEN_1D6		= 4,
	MULTIGEN_1D8		= 5,
	MULTIGEN_1D10		= 6,
	MULTIGEN_2D6		= 7,
	MULTIGEN_3D6		= 8,
	MULTIGEN_4D6		= 9,
	MULTIGEN_5D6		= 10,
	MULTIGEN_6D6		= 11,
	MULTIGEN_1D6_1		= 12,
	MULTIGEN_1D6_2		= 13,
	MULTIGEN_1D6_3		= 14,
	MULTIGEN_1D6_4		= 15,
	MULTIGEN_1D6_5		= 16,
	MULTIGEN_1D6_6		= 17,
	MULTIGEN_2D6_1		= 18,
	MULTIGEN_2D6_2		= 19,
	MULTIGEN_2D6_3		= 20,
	MULTIGEN_2D6_4		= 21,
	MULTIGEN_2D6_5		= 22,
	MULTIGEN_2D6_6		= 23,
	MULTIGEN_1D2_1		= 24,
	MULTIGEN_1D3_1		= 25,
	MULTIGEN_1D4_1		= 26,
	MULTIGEN_1D2_2		= 27,
	MULTIGEN_1D3_2		= 28,
	MULTIGEN_1D4_2		= 29,
	MULTIGEN_1D3_3		= 30,
	MULTIGEN_1D4_3		= 31,
	MULTIGEN_1D4_4		= 32
};

enum multishot_types {
	MULTISHOT_NONE = 0,
	MULTISHOT_LAUNCHER_1D2_NOSKILL = 1,
	MULTISHOT_LAUNCHER_2_NOSKILL = 2,
	MULTISHOT_LAUNCHER_1D2_1_NOSKILL = 3,
	MULTISHOT_LAUNCHER_3_NOSKILL = 4,
	MULTISHOT_LAUNCHER_1D3_NOSKILL = 5,
	MULTISHOT_THROWN_1D2_NOSKILL = 6,
	MULTISHOT_THROWN_2_NOSKILL = 7,
	MULTISHOT_THROWN_1D2_1_NOSKILL = 8,
	MULTISHOT_THROWN_3_NOSKILL = 9,
	MULTISHOT_THROWN_1D3_NOSKILL = 10,
	MULTISHOT_MELEE_1D2_NOSKILL = 11,
	MULTISHOT_MELEE_2_NOSKILL = 12,
	MULTISHOT_MELEE_1D2_1_NOSKILL = 13,
	MULTISHOT_MELEE_3_NOSKILL = 14,
	MULTISHOT_MELEE_1D3_NOSKILL = 15,
	MULTISHOT_LAUNCHER_1D2_BASIC = 16, /* NOSKILL = no multishot */
	MULTISHOT_THROWN_1D2_BASIC = 17,
	MULTISHOT_MELEE_1D2_BASIC = 18,
	MULTISHOT_LAUNCHER_2_SKILLED_1D2_BASIC = 19,
	MULTISHOT_THROWN_2_SKILLED_1D2_BASIC = 20,
	MULTISHOT_MELEE_2_SKILLED_1D2_BASIC = 21,
	MULTISHOT_LAUNCHER_1D2_1_EXPERT_2_SKILLED_1D2_BASIC = 22,
	MULTISHOT_THROWN_1D2_1_EXPERT_2_SKILLED_1D2_BASIC = 23,
	MULTISHOT_MELEE_1D2_1_EXPERT_2_SKILLED_1D2_BASIC = 24,
	MULTISHOT_LAUNCHER_3_EXPERT_2_SKILLED_1D2_BASIC = 25,
	MULTISHOT_THROWN_3_EXPERT_2_SKILLED_1D2_BASIC = 26,
	MULTISHOT_MELEE_3_EXPERT_2_SKILLED_1D2_BASIC = 27,
	MULTISHOT_LAUNCHER_4_EXPERT_3_SKILLED_2_BASIC = 28,
	MULTISHOT_THROWN_4_EXPERT_3_SKILLED_2_BASIC = 29,
	MULTISHOT_MELEE_4_EXPERT_3_SKILLED_2_BASIC = 30,
	MULTISHOT_LAUNCHER_1D2_SKILLED = 31, /* NOSKILL and BASIC = no multishot */
	MULTISHOT_THROWN_1D2_SKILLED = 32,
	MULTISHOT_MELEE_1D2_SKILLED = 33,
	MULTISHOT_LAUNCHER_2_EXPERT_1D2_SKILLED = 34,
	MULTISHOT_THROWN_2_EXPERT_1D2_SKILLED = 35,
	MULTISHOT_MELEE_2_EXPERT_1D2_SKILLED = 36,
	MULTISHOT_LAUNCHER_3_EXPERT_2_SKILLED = 37,
	MULTISHOT_THROWN_3_EXPERT_2_SKILLED = 38,
	MULTISHOT_MELEE_3_EXPERT_2_SKILLED = 39,
	MULTISHOT_LAUNCHER_1D2_EXPERT = 40, /* NOSKILL and BASIC = no multishot */
	MULTISHOT_THROWN_1D2_EXPERT = 41,
	MULTISHOT_MELEE_1D2_EXPERT = 42,
	MULTISHOT_LAUNCHER_2_EXPERT = 43, /* NOSKILL, BASIC, and SKILLED = no multishot */
	MULTISHOT_THROWN_2_EXPERT = 44,
	MULTISHOT_MELEE_2_EXPERT = 45,
	MULTISHOT_LAUNCHER_2_BASIC = 46,
	MULTISHOT_LAUNCHER_2_SKILLED = 47,
	MULTISHOT_THROWN_2_BASIC = 48,
	MULTISHOT_THROWN_2_SKILLED = 49,
	MULTISHOT_MELEE_2_BASIC = 50,
	MULTISHOT_MELEE_2_SKILLED = 51,
	MAX_MULTISHOT_TYPES
};

static const char* multishot_style_names[MAX_MULTISHOT_TYPES] = {
	"None", 
	"1d2 when firing", "2 when firing", "1d2+1 when firing", "3 when firing", "1d3 when firing",
	"1d2 when thrown", "2 when thrown", "1d2+1 when thrown", "3 when thrown", "1d3 when thrown",
	"1d2 in melee", "2 in melee", "1d2+1 in melee", "3 in melee", "1d3 in melee",
	"1d2 when firing with basic skill",
	"1d2 when thrown with basic skill",
	"1d2 in melee with basic skill",
	"2 when firing with basic skill",
	"2 when thrown with basic skill",
	"2 in melee with basic skill",
	"1d2/2/1d2+1 by skill level when firing",
	"1d2/2/1d2+1 by skill level when thrown",
	"1d2/2/1d2+1 by skill level in melee",
	"1d2/2/3 by skill level when firing",
	"1d2/2/3 by skill level when thrown",
	"1d2/2/3 by skill level in melee",
	"2/3/4 by skill level when firing",
	"2/3/4 by skill level when thrown",
	"2/3/4 by skill level in melee",
	"1d2 when firing with skilled skill",
	"1d2 when thrown with skilled skill",
	"1d2 in melee with skilled skill",
	"1/1d2/2 by skill level when firing",
	"1/1d2/2 by skill level when thrown",
	"1/1d2/2 by skill level in melee",
	"1/2/3 by skill level when firing",
	"1/2/3 by skill level when thrown",
	"1/2/3 by skill level in melee",
	"1d2 when firing with expert skill",
	"1d2 when thrown with expert skill",
	"1d2 in melee with expert skill",
	"2 when firing with expert skill",
	"2 when thrown with expert skill",
	"2 in melee with expert skill",
	"2 when firing with basic skill",
	"2 when firing with skilled skill",
	"2 when thrown with basic skill",
	"2 when thrown with skilled skill",
	"2 in melee with basic skill",
	"2 in melee with skilled skill"
};


/* [misnamed] definition of a type of object; many objects are composites
   (liquid potion inside glass bottle, metal arrowhead on wooden shaft)
   and object definitions only specify one type on a best-fit basis */
enum obj_material_types {
    MAT_LIQUID      =  1, /* currently only for venom */  /* Organics start here */
    MAT_WAX         =  2,
    MAT_VEGGY       =  3, /* foodstuffs */
	MAT_FLESH       =  4, /*   ditto    */
	MAT_ORGANIC		=  5, /* non-veggy, non-flesh organic material, e.g. bat guano, feathers */
	MAT_PAPER       =  6,
    MAT_CLOTH       =  7,
	MAT_SILK		=  8,
	MAT_LEATHER     =  9,
    MAT_WOOD        = 10, /* Organics stop here */
    MAT_BONE		= 11,
    MAT_DRAGON_HIDE = 12, /* not leather! */
    MAT_IRON        = 13, /* Fe - includes steel */
    MAT_METAL       = 14, /* Sn, &c. */
    MAT_COPPER      = 15, /* Cu - includes brass */
    MAT_SILVER      = 16, /* Ag */
    MAT_GOLD        = 17, /* Au */
    MAT_PLATINUM    = 18, /* Pt */
	MAT_ORICHALCUM	= 19,
	MAT_ADAMANTIUM  = 20,
	MAT_MITHRIL     = 21,
    MAT_PLASTIC     = 22,
    MAT_GLASS       = 23,
    MAT_GEMSTONE    = 24,
    MAT_MINERAL     = 25,
	MAT_MODRONITE	= 26,
	MAT_PLANARRIFT	= 27,
	MAT_FORCEFIELD  = 28
};

enum obj_armor_types {
    ARM_SUIT    = 0,
    ARM_SHIELD  = 1,        /* needed for special wear function */
    ARM_HELM    = 2,
    ARM_GLOVES  = 3,
    ARM_BOOTS   = 4,
    ARM_CLOAK   = 5,
    ARM_SHIRT   = 6,
	ARM_ROBE	= 7,
	ARM_BRACERS = 8, //END MARKER FOR ARMOR TYPES
	MAX_ARMOR_TYPES
};

static const char* armor_type_names[MAX_ARMOR_TYPES] = {
	"suit of armor", "shield", "helmet", "gloves", "boots", "cloak", "shirt", "robe", "bracers",
};


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

static const char* weapon_type_names[MAX_WEAPON_TYPES] = {
	"short sword", "long sword", "two-handed sword", "broadsword", "bastard sword", "scimitar", "saber", "katana",  "tsurugi",
	"axe", "throwing axe", "two-handed axe", "dagger", "knife", "mace", "spiked mace", "flail", "morning star", "club", "spiked club", 
	"hammer", "spiked hammer",  "staff", "rod", "spear", "javelin", "polearm", "trident",
	"dart", "shuriken", "boomerang", "short bow", "long bow", "hand crossbow", "crossbow", "repeating crossbow", "sling",
	"arrow", "quarrel", "sling-bullet", "pick-axe", "unicorn horn", "horn", "worm tooth",
	"shovel", "hook", "lance", "hose", "fork", "whip", "double-headed flail", "triple-headed flail"
};

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

static const char* food_type_names[MAX_FOOD_TYPES] = {
	"general food", "ration", "fruit", "vegetable", "nuts", "seeds", "leaf", "kelp",  "bread",
	"cake", "candy", "mushroom", "mold", "glob", "egg", "meat", "jelly", "corpse",  "tin"
};

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
	MISC_BRACERS = 10,		/* alternative implementation of bracers */
	MISC_BELT = 11,			
	MISC_EYEGLASSES = 12,		
	MISC_BLINDFOLD = 13,	/* alternative implementation of blindfold */
	MISC_SCARF = 14,
	MISC_EXTRA_ARMS = 15,
	MISC_BROOCH = 16,
	MISC_MASK = 17,
	MISC_NECKTIE = 18,
	MAX_MISC_TYPES
};

static const char* misc_type_names[MAX_MISC_TYPES] = {
	"", "set of wings", "pair of pants", "skirt", "wrist watch",
	"nose ring", "headband", "pair of earrings",
	"ioun stone", "bracelet", "pair of bracers", "belt", "pair of eyeglasses", "blindfold", "scarf", "set of extra arms", "brooch", "mask", "necktie"
};

static const char* misc_type_worn_texts[MAX_MISC_TYPES] = {
	"", "attached to back", "", "", "on left wrist",
	"on nose", "", "",
	"orbiting head", "", "", "", "", "", "", "attached to body", "", "", ""
};

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
	MAX_TOOL_TYPES
};

static const char* tool_type_names[MAX_TOOL_TYPES] = {
	"tool", "box", "chest", "bag", "pick-axe", "shovel", "long club-headed weapon", "hook", "horn", "candelabrum", "bell", 
	"candle", "lamp", "lantern", "whistle", "flute", "harp", "drum", "saw", "jar", "can", "grail"
};

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
	CHARGED_UNICORN_HORN = 29
};

enum enchantment_init_types {
	ENCHTYPE_NO_ENCHANTMENT = 0,
	ENCHTYPE_GENERAL = 1,
	ENCHTYPE_ALWAYS_1 = 2,
	ENCHTYPE_ALWAYS_2 = 3,
	ENCHTYPE_ALWAYS_3 = 4,
	ENCHTYPE_ALWAYS_4 = 5,
	ENCHTYPE_ALWAYS_5 = 6,
	ENCHTYPE_RING_NORMAL = 7,
	ENCHTYPE_RING_1_7 = 8,
	ENCHTYPE_RING_DOUBLE = 9,
	ENCHTYPE_RING_POWER = 10,
	ENCHTYPE_MISCELLANEOUS_NORMAL = 11,
	ENCHTYPE_GENERAL_ALWAYS_START_0 = 12
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
	uchar oc_enchantable;				/* Uses +X statistic (enchantment) */
	uchar oc_charged;		        /* may have +n or (n) charges */
    Bitfield(oc_unique, 1);         /* special one-of-a-kind object */
    Bitfield(oc_nowish, 1);         /* cannot wish for this object */

    Bitfield(oc_big, 1);
#define oc_bimanual oc_big /* for weapons & tools used as weapons */
#define oc_bulky oc_big    /* for armor */
    Bitfield(oc_tough, 1); /* hard gems/rings */

    Bitfield(oc_dir, 4);
#define NODIR 1     /* for wands/spells:	non-directional */
#define IMMEDIATE 2 /*						directional at one target */
#define RAY 3       /*						zap beams */
#define TARGETED 4  /*						targeted using throwspell */
#define TOUCH 5     /*						like immediate but touch distance */
#define IMMEDIATE_MULTIPLE_TARGETS 6 /*		directional but many targets in a line like a ray */
#define IMMEDIATE_ONE_TO_THREE_TARGETS 7 /*		directional but 1 target for cursed, 2 targets for uncursed and 3 targets for blessed in a line like a ray */

#define IMMEDIATE_ITEM_ARROW 1

#define PIERCE 1 /* for weapons & tools used as weapons */
#define SLASH 2  /* (latter includes iron ball & chain) */
#define WHACK 0
	/* 4 free bits */

    Bitfield(oc_material, 5); /* one of obj_material_types */

#define is_organic(otmp) (objects[otmp->otyp].oc_material <= MAT_WOOD)
#define is_metallic(otmp)                    \
    (objects[otmp->otyp].oc_material >= MAT_IRON \
     && objects[otmp->otyp].oc_material <= MAT_MITHRIL)

/* primary damage: fire/rust/--- */
/* is_flammable(otmp), is_rottable(otmp) in mkobj.c */
#define is_rustprone(otmp) (objects[(otmp)->otyp].oc_material == MAT_IRON && !(objects[(otmp)->otyp].oc_flags & O1_RUST_RESISTANT))

/* secondary damage: rot/acid/acid */
#define is_corrodeable(otmp)                   \
    ((objects[otmp->otyp].oc_material == MAT_COPPER \
     || objects[otmp->otyp].oc_material == MAT_IRON) && !(objects[otmp->otyp].oc_flags & O1_CORROSION_RESISTANT))

#define is_damageable(otmp)                                        \
    (is_rustprone(otmp) || is_flammable(otmp) || is_rottable(otmp) \
     || is_corrodeable(otmp))

    /* 3 free bits */

    schar oc_subtyp;		 /* armors: armor category, weapons: weapon category, miscellaneous magic items: subclass, etc.*/
	schar oc_skill;			 /* Skills of weapons, spellbooks, tools, gems */
	uchar oc_oprop, oc_oprop2, oc_oprop3; /* properties (invis, &c.) conveyed */
	unsigned long oc_pflags;	 /* Power and property flags */

#define P1_NONE													0x00000000UL
#define P1_POWER_1_APPLIES_TO_ALL_CHARACTERS					0x00000001UL  
#define P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY		0x00000002UL  
#define P1_POWER_1_APPLIES_WHEN_CARRIED							0x00000004UL 
#define P1_POWER_2_APPLIES_TO_ALL_CHARACTERS					0x00000008UL  
#define P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY		0x00000010UL  
#define P1_POWER_2_APPLIES_WHEN_CARRIED							0x00000020UL  
#define P1_POWER_3_APPLIES_TO_ALL_CHARACTERS					0x00000040UL  
#define P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY		0x00000080UL  
#define P1_POWER_3_APPLIES_WHEN_CARRIED							0x00000100UL  
#define P1_HP_BONUS_APPLIES_TO_ALL_CHARACTERS					0x00000200UL  
#define P1_HP_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY	0x00000400UL  
#define P1_HP_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS		0x00000800UL  
#define P1_HP_BONUS_APPLIES_WHEN_CARRIED						0x00001000UL  
#define P1_MANA_BONUS_APPLIES_TO_ALL_CHARACTERS					0x00002000UL  
#define P1_MANA_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY	0x00004000UL  
#define P1_MANA_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS		0x00008000UL  
#define P1_MANA_BONUS_APPLIES_WHEN_CARRIED						0x00010000UL  
#define P1_ATTRIBUTE_BONUS_APPLIES_TO_ALL_CHARACTERS			0x00020000UL 
#define P1_ATTRIBUTE_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY	0x00040000UL  
#define P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS	0x00080000UL  
#define P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED					0x00100000UL  
#define P1_LUCK_APPLIES_TO_ALL_CHARACTERS						0x00200000UL 
#define P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY		0x00400000UL  
#define P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS			0x00800000UL  
#define P1_LUCK_APPLIES_WHEN_CARRIED							0x01000000UL  
/* free bit */
/* free bit */
#define P1_CURSED_ITEM_YIELDS_NEGATIVE							0x08000000UL  
#define P1_CONFERS_LUCK											0x10000000UL			/* Adds one positive luck bonus; these need to be flags instead of props because they stack */
#define P1_CONFERS_UNLUCK										0x20000000UL			/* Adds one negative luck bonus (the same as cursed luckstone) */
#define P1_MANA_PERCENTAGE_BONUS								0x40000000UL
#define P1_HP_PERCENTAGE_BONUS									0x80000000UL


	char  oc_class; /* object class (enum obj_class_types) */
    schar oc_delay; /* delay when using such an object */
    uchar oc_color; /* color of the object */

    short oc_prob;            /* probability, used in mkobj() */
    unsigned int oc_weight; /* encumbrance (1 oz = 1/16 lb.) previously (1 cn = 0.1 lb.) */
	unsigned int oc_nutrition; /* food value */

	int oc_cost;            /* base cost in shops */
    
							  /* Check the AD&D rules!  The FIRST is small monster damage. */
    /* for weapons, and tools, rocks, and gems useful as weapons */
	int oc_damagetype;						/* Type of damage caused by the (magic) weapon, the same as for monster attacks */
	int oc_wsdice, oc_wsdam, oc_wsdmgplus;	/* small monster damage, also used for spell damage */
	int oc_wldice, oc_wldam, oc_wldmgplus;	/* large monster damage, also used for duration for spells */
	int oc_extra_damagetype;				/* Type of extra damage caused by the (magic) weapon */
	int oc_wedice, oc_wedam, oc_wedmgplus;	/* extra damage used as a special effect influenced by target permissions mask */
	unsigned long oc_aflags;				/* attack related flags, e.g. whether the attack is vorpal */

/* Attack flags for weapons, armor, weapon-like tools, and miscellaneous items */
#define A1_NONE														0x00000000UL
#define A1_CRITICAL_STRIKE											0x00000001UL  /* extra damage is caused only by chance determined by oc_critical_strike_percentage */
#define A1_CRITICAL_STRIKE_IS_DEADLY								0x00000002UL  
		/* successful critical strike causes lethal damage. If this flag is on, critical strike is considered not to be connected with extra damage; rather, it is treated similarly to vorpal attack */
#define A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK					0x00000004UL  /* lethal damage is death magic */
#define A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK			0x00000008UL  /* lethal damage is disintegration */
#define A1_DEADLY_CRITICAL_STRIKE_USES_EXTRA_DAMAGE_TYPE (A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)  
		/* lethal damage is of extra damage type; note that normal critical strike always follows extra_damagetype */
#define A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK (A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK | A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)  
#define A1_CRITICAL_STRIKE_DISRESPECTS_TARGETS						0x00000010UL  /* successful critical strike causes lethal damage */
#define A1_CRITICAL_STRIKE_DISRESPECTS_CHARACTERS					0x00000020UL  /* successful critical strike causes lethal damage */
#define A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES	0x00000040UL  /* All other special types use critical strike probability instead of being certain or using their own standard probability */
#define A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL					0x00000080UL  /* The number specified by oc_critical_strike_percentage is a die roll on d20 (X or less, e.g., 1 = 5% chance or 2 = 10% chance on any attack regardless of actual hit chance) */

#define A1_WOUNDING													0x00000100UL  /* extra damage caused is permanent damage */
#define A1_WOUNDING_DISRESPECTS_TARGETS								0x00000200UL  /* successful critical strike causes lethal damage */
#define A1_WOUNDING_DISRESPECTS_CHARACTERS							0x00000400UL  /* successful critical strike causes lethal damage */

#define A1_LIFE_LEECH												0x00000800UL  /* heals hit points equal to the extra damage caused */
#define A1_LIFE_LEECH_DISRESPECTS_TARGETS							0x00001000UL  /* successful critical strike causes lethal damage */
#define A1_LIFE_LEECH_DISRESPECTS_CHARACTERS						0x00002000UL  /* successful critical strike causes lethal damage */

#define A1_SHARPNESS												0x00004000UL	/* 2/20 chance of the monster losing 25% of maximum hit points */
#define A1_VORPAL													0x00008000UL	/* 1/20 chance of the monster being beheaded */
#define A1_BISECT (A1_SHARPNESS | A1_VORPAL) /* 1/20 chance of a small monster being bisected and a big monster losing 50% of maximum hit points */
#define A1_SVB_MASK (A1_SHARPNESS | A1_VORPAL)
#define A1_VORPAL_LIKE_DISRESPECTS_TARGETS							0x00010000UL
#define A1_VORPAL_LIKE_DISRESPECTS_CHARACTERS						0x00020000UL

#define A1_LEVEL_DRAIN												0x00040000UL  /* drains a level from monsters */
#define A1_LEVEL_DRAIN_DISRESPECTS_TARGETS							0x00080000UL  
#define A1_LEVEL_DRAIN_DISRESPECTS_CHARACTERS						0x00100000UL  

/* Extra weapon options */
#define A1_STUN														0x00200000UL  /* stuns target, not implemented */
#define A1_HITS_ADJACENT_SQUARES									0x00400000UL  /* like Cleaver */
#define A1_REQUIRES_AND_EXPENDS_A_CHARGE							0x00800000UL  /* Nine lives stealer */

/* General */
#define A1_MAGIC_RESISTANCE_PROTECTS								0x01000000UL
#define A1_BYPASSES_MC												0x02000000UL	/* No MC check */  
#define A1_EXTRA_DAMAGE_DISRESPECTS_TARGETS							0x04000000UL  
#define A1_EXTRA_DAMAGE_DISRESPECTS_CHARACTERS						0x08000000UL  
#define A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA							0x10000000UL /* abilities such as wounding and life leech are based on full caused damage, not just extra damage */
#define A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS					0x20000000UL
#define A1_ITEM_VANISHES_ON_HIT										0x40000000UL
#define A1_ITEM_VANISHES_ONLY_IF_PERMITTED_TARGET					0x80000000UL


#define oc_spell_flags oc_aflags

/* Spell flags for spells, scrolls, potions, spell-like tools, and wands */
#define S1_NONE									0x00000000UL
#define S1_SPELL_EXPLOSION_EFFECT				0x00000001UL
#define S1_SPELL_BYPASSES_MAGIC_RESISTANCE		0x00000002UL
#define S1_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY	0x00000004UL
#define S1_SPELL_IS_NONREVERSIBLE_PERMANENT		0x00000008UL
#define S1_SPELL_STOPS_AT_FIRST_HIT_OBJECT		0x00000010UL
#define S1_NO_SOMATIC_COMPONENT					0x00000020UL
#define S1_NO_VERBAL_COMPONENT					0x00000040UL
#define S1_DOES_NOT_TAKE_A_TURN					0x00000080UL


	int oc_hitbonus;						/* weapons: "to hit" bonus */
	int oc_mc_adjustment;					/* weapons: adjustment to any MC checks; spells and wands: MC adjustment */
	int oc_fixed_damage_bonus;				/* fixed strength-based damage bonus for crossbows; O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH must be on; can be used for other purposes for a spellbook flag */
	int oc_range;							/* launchers: range for ammo, others throw range: >0 Fixed range, <0 Percentage of STR */

	/* general purpose */
	int oc_oc1;		/* Used for spell cooldown; weapons and armors: ac bonus */
	int oc_oc2;		/* Used for spell level; weapons and armors: mc bonus */
	int oc_oc3;		/* Used for spell mana cost; other items: mana pool bonus */
	int oc_oc4;		/* Used for spell attributes; other items: hit point bonus */
	int oc_oc5;		/* Used for spell range; non-spellbooks: specification of attributes or other properties item gives bonuses to using otmp->enchantment */
	int oc_oc6;		/* Used for spell radius; non-spellbooks: 0 => enchantment is used, otherise fixed bonus */
	int oc_oc7;		/* Used for spell casting penalty */
	int oc_oc8;		/* Used for multishot count */

/* general*/
#define oc_armor_class oc_oc1						/* weapons and armor: AC used in ARM_AC_BONUS in do.c */
#define oc_magic_cancellation oc_oc2				/* weapons and armor: MC, i.e., resistance level to magical attacks */
#define oc_mana_bonus oc_oc3						/* non-spellbooks: mana pool bonus: Fixed points unless O1_MANA_PERCENTAGE_BONUS is specified */
#define oc_hp_bonus oc_oc4							/* non-spellbooks: hit point bonus: Fixed points unless O1_HP_PERCENTAGE_BONUS is specified */
#define oc_bonus_attributes oc_oc5					/* non-spellbooks: gives bonuses using enchantment / oc_oc6 to attributes and properties */
#define oc_attribute_bonus oc_oc6					/* non-spellbooks: 0 => enchantment is used, otherise fixed bonus */
#define oc_spell_casting_penalty oc_oc7				/* non-spells/wands: spell casting penalty when worn */
#define oc_multishot_style oc_oc8					/* multishot style specifying when the weapon can multishot and how many times */

#define BONUS_TO_STR 0x0001
#define BONUS_TO_DEX 0x0002
#define BONUS_TO_CON 0x0004
#define BONUS_TO_INT 0x0008
#define BONUS_TO_WIS 0x0010
#define BONUS_TO_CHA 0x0020
#define BONUS_TO_DAMAGE 0x0040
#define BONUS_TO_HIT 0x0080
#define BONUS_TO_AC 0x0100
#define BONUS_TO_MC 0x0200
#define BONUS_TO_SPELL_CASTING 0x0400
#define BONUS_TO_EXPERIENCE 0x0800
#define BONUS_TO_ARCHERY 0x0800

#define BONUS_TO_ALLSTATS BONUS_TO_STR | BONUS_TO_DEX | BONUS_TO_CON | BONUS_TO_INT | BONUS_TO_WIS | BONUS_TO_CHA

#define SETS_FIXED_ATTRIBUTE 0x1000
#define FIXED_IS_MAXIMUM 0x2000
#define IGNORE_ENCHANTMENT 0x4000


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
#define oc_armor_category oc_subtyp					/* armor: (enum obj_armor_types) */
/* oc_oc1 AC bonus */
/* oc_oc2 MC bonus */
/* oc_oc3 mana pool bonus */
/* oc_oc4 hit point bonus */
/* oc_oc5 attributes giving bonus to using enchantment / oc_oc6 to attributes and properties */
/* oc_oc6 modifier to oc_oc5: 0 => enchantment is used, otherise fixed bonus */
/* oc_oc7 spell casting penalty */
/* oc_oc8 multishot style */

/* comestibles and reagents (and other edibles) */
#define oc_edible_subtype oc_oc1		/* edibles: is rotten, poisoned, etc. */
#define oc_edible_effect oc_oc2			/* edibles: gain ability score, etc., need two, since sprig of wolfsbane is both poisonous and cures lycanthropy */

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


/* spells */
#define oc_spell_cooldown oc_oc1		/* books: spell cooldown time */
#define oc_spell_level oc_oc2			/* books: spell level */
#define oc_spell_mana_cost oc_oc3		/* books: spell mana cost */
#define oc_spell_attribute oc_oc4		/* books: spell primary casting attribute */
#define oc_spell_range oc_oc5			/* books: spell range */
#define oc_spell_radius oc_oc6			/* books: spell radius */
#define oc_spell_skill_chance oc_oc7	/* books: % chance of gaining skill points upon casting, also % multiplier for mixing */
/* oc_oc8 unused */
#define oc_spell_saving_throw_adjustment oc_mc_adjustment	/* books: spell saving throw adjustment */
#define oc_spell_dmg_dice oc_wsdice		/* books: spell damage no of dice */
#define oc_spell_dmg_diesize oc_wsdam	/* books: spell damage size of dice */
#define oc_spell_dmg_plus oc_wsdmgplus	/* books: spell damage constant added */
#define oc_spell_dur_dice oc_wldice		/* books: spell duration no of dice */
#define oc_spell_dur_diesize oc_wldam	/* books: spell duration size of dice */
#define oc_spell_dur_plus oc_wldmgplus	/* books: spell duration constant added */

	int oc_dir_subtype;				/* spells: ID for type of ray or immediate effect, weapons: damage type */

	int oc_material_components;		/* spells: ID for material component list for a spell or to make the item (if recipe is known), other items: ID of components produced when breaking */
	int oc_item_cooldown;			/* cooldown before the item can be used / applied / zapped / read etc. again */
	int oc_item_level;				/* item level, to be used with loot tables */

	unsigned long oc_flags;			/* E.g. if indestructible or disintegration resistant */
	unsigned long oc_flags2;		/* More flags */
	unsigned long oc_flags3;		/* Even more flags */
	unsigned long oc_flags4;		/* Even more flags */

	unsigned long oc_power_permissions; /* roles, races, genders, and alignments that the item's powers are conferred to */
	unsigned long oc_target_permissions; /* symbol, M1 flag, M2 flag, M3 flag, etc. for which extra damage is deal to */
	int oc_critical_strike_percentage;	/* percentage to be used with A1_CRITICAL_STRIKE; can be used for other purposes for a S1_ flag, too */
	int oc_multigen_type;				/* class number multi multigen_type */
	int oc_tile_floor_height;			/* (scaled) height of the item tile in pixels when it appears on the floor */

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
#define O1_NONE								0x00000000UL 
#define O1_INDESTRUCTIBLE					0x00000001UL 
#define O1_DISINTEGRATION_RESISTANT			0x00000002UL 
#define O1_FIRE_RESISTANT					0x00000004UL 
#define O1_COLD_RESISTANT					0x00000008UL 
#define O1_LIGHTNING_RESISTANT				0x00000010UL
#define O1_POLYMORPH_RESISTANT				0x00000020UL
#define O1_RUST_RESISTANT					0x00000040UL
#define O1_CORROSION_RESISTANT				0x00000080UL
#define O1_ROT_RESISTANT					0x00000100UL

#define O1_ENCHANTMENT_AFFECTS_MC			0x00000200UL			/* +X of the item influences also its MC */
#define O1_EROSION_DOES_NOT_AFFECT_MC		0x00000400UL			/* erosion level does not affect the item's MC */
#define O1_ENCHANTMENT_DOES_NOT_AFFECT_AC	0x00000800UL			/* +X of the item does not affect its AC bonus */
#define O1_EROSION_DOES_NOT_AFFECT_AC		0x00001000UL			/* erosion level does not affect the item's MC */
#define O1_IS_ARMOR_WHEN_WIELDED			0x00002000UL			/* acts as an armor when wielded giving AC using oc_armor_class, which must be specified */
#define O1_IS_WEAPON_WHEN_WIELDED			0x00004000UL			/* acts as a weapon when wielded (or worn in shield slot in two-weapon fighting) using damage statistics */
#define O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT 0x00008000UL			/* +X of the item influences also its MC adjustment (i.e., saving throw penalty for attacks) */

#define O1_EDIBLE_NONFOOD					0x00010000UL

#define O1_BECOMES_CURSED_WHEN_WORN							0x00020000UL
#define O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED		0x00040000UL
#define O1_CANNOT_BE_DROPPED_IF_CURSED						0x00080000UL

#define O1_FLICKERS_WHEN_MONSTERS_DETECTED					0x00100000UL
#define O1_OFLAG_POWERS_APPLY_TO_ALL_CHARACTERS				0x00200000UL
#define O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY	0x00400000UL
#define O1_OFLAG_POWERS_APPLY_WHEN_CARRIED					0x00800000UL

#define O1_THROWN_WEAPON_ONLY				0x01000000UL			/* says "Thrown weapon" instead of "Melee weapon", default range is larger, can use throwrange data value, gets penalties in melee combat */
#define O1_MELEE_AND_THROWN_WEAPON			0x02000000UL			/* says "Melee and thrown weapon" instead of "Melee weapon", default range is larger, can use throwrange data value */
#define O1_WEIGHT_DOES_NOT_REDUCE_RANGE		0x04000000UL			/* the object magically flies when thrown, ignoring its weight */
#define O1_RETURNS_TO_HAND_AFTER_THROWING	0x08000000UL			/* the object returns to the owner's hand after throwing */
#define O1_CAN_BE_THROWN_ONLY_IF_WIELDED	0x10000000UL			/* has to be wielded to be thrown, e.g., Mjollnir */
#define O1_WAND_LIKE_TOOL					0x20000000UL			/* apply is the same as zap, uses spellbook/wand paramters and flags instead of normal flags */
#define O1_NON_SPELL_SPELLBOOK				0x40000000UL			/* uses non-spellbook flags and other non-spellbook stats */
#define O1_NOT_CURSEABLE					0x80000000UL


/* Flags 2 */
#define O2_NONE					0x00000000UL
#define O2_ELVEN_ITEM			0x00000001UL
#define O2_DWARVEN_ITEM			0x00000002UL
#define O2_ORCISH_ITEM			0x00000004UL
#define O2_GNOLLISH_ITEM		0x00000008UL
#define O2_DRAGON_ITEM			0x00000010UL
#define O2_DEMON_ITEM			0x00000020UL
#define O2_ANGELIC_ITEM			0x00000040UL
#define O2_MODRON_ITEM			0x00000080UL
#define O2_GNOMISH_ITEM			0x00000100UL
	/* free bit */

#define O2_CONTAINER			0x00000400UL	
#define O2_CONTAINER_BOX		0x00000800UL	
#define O2_CONTAINER_MAGIC_BAG	0x00001000UL	
#define O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG 0x00002000UL
#define O2_CONTAINER_NONCONTAINER 0x00004000UL	

#define O2_MONSTER_SCALES		0x00008000UL	
#define O2_MONSTER_SCALE_MAIL	0x00010000UL
#define O2_CANDLE				0x00020000UL	
#define O2_SHINES_MAGICAL_LIGHT 0x00040000UL	
#define O2_FLICKER_COLOR_RED	O2_NONE	
#define O2_FLICKER_COLOR_WHITE	0x00080000UL	
#define O2_FLICKER_COLOR_BLUE	0x00100000UL	
#define O2_FLICKER_COLOR_BLACK (O2_FLICKER_COLOR_WHITE | O2_FLICKER_COLOR_BLUE)	
#define O2_FLICKER_COLOR_MASK (O2_FLICKER_COLOR_WHITE | O2_FLICKER_COLOR_BLUE)	

#define O2_GRAYSTONE			0x00200000UL	
#define O2_ROCK					0x00400000UL	
#define O2_ORE					0x00800000UL	
#define O2_FLIMSY				0x01000000UL

#define O2_SPECIAL_PRAYING_ITEM 0x02000000UL
	/* free bit */
	/* free bit */

#define O2_GENERATED_DEATH_OR_COLD_ENCHANTED 0x10000000UL			/* if deathenchantable item, then death, otherwise lightning*/
#define O2_GENERATED_BLESSED	0x20000000UL							/* Always generated blessed */									
#define O2_GENERATED_CURSED		0x40000000UL							/* Always generated cursed */								
#define O2_CURSED_MAGIC_ITEM	0x80000000UL							/* The effect of the item is negative; typically 90% chance of being cursed */									

/* Flags 3 */
#define O3_NONE										0x00000000UL
#define O3_NO_GENERATION							0x00000001UL  /* item is not generated normally */
#define O3_NO_WISH									0x00000002UL  /* item is special, it cannot be wished for, mimics oc_nowish */
#define O3_UNIQUE									0x00000004UL  /* the item is unique, mimics oc_unique */

/* free bit */
#define O3_EXTENDED_POLEARM_REACH					0x00000010UL  /* range is max 13 instead of normal 8 */
#define O3_DOUBLE_DIGGING_EFFORT					0x00000020UL  /* double normal digging effort */
#define O3_BURIED_SEARCHABLE						0x00000040UL  /* if buried, can be found by explicit searching */
#define O3_CONTENT_DESCRIPTION_SHUFFLED				0x00000080UL  /* uses (shuffled) description index instead of name index for content description */
#define O3_CONSUMES_NUTRITION_EVERY_20_ROUNDS		0x00000100UL  /* consumes nutrition every 20 rounds when worn. WORKS ONLY FOR MISCELLANEOUS MAGIC ITEMS */
/* free bit */

#define O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS	0x00000400UL  /* wielding or wearing prohibits the revival of permitted targets */
#define O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS	0x00000800UL  /* wielding or wearing prohibits AD_XXXX-type innate summoning by permitted targets; the target can still summon using spells */
#define O3_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS	0x00001000UL	/* deals damage when wielded like artifacts */

#define O3_INVOKABLE								0x00002000UL	/* can be invoked using invoke command */
#define O3_APPLIABLE								0x00004000UL	/* can be applied as a tool */
#define O3_WIELDABLE								0x00008000UL	/* can be wielded in a weapon slot */
#define O3_READABLE									0x00010000UL	/* can be read */								
#define O3_IGNITABLE								0x00020000UL	
#define O3_RELATIVE_AGE								0x00040000UL	
#define O3_ELEMENTAL_ENCHANTABLE					0x00080000UL
#define O3_EATING_IDENTIFIES						0x00100000UL	
#define O3_NONROTTING_FOOD							0x00200000UL
#define O3_POISONABLE								0x00400000UL
#define O3_QUAFFABLE								0x00800000UL    /* can be drunk */			

#define O3_PERMTTED_TARGET_LAWFUL					0x01000000UL
#define O3_PERMTTED_TARGET_NEUTRAL					0x02000000UL
#define O3_PERMTTED_TARGET_CHAOTIC					0x04000000UL

#define O3_TARGET_PERMISSION_IS_M1_FLAG				0x08000000UL /* Note: if no flag, then default is a monster symbol */
#define O3_TARGET_PERMISSION_IS_M2_FLAG				0x10000000UL
#define O3_TARGET_PERMISSION_IS_M3_FLAG				0x20000000UL 
#define O3_TARGET_PERMISSION_IS_M4_FLAG				0x40000000UL 

#define O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH 0x80000000UL 


/* Flags 4 */
#define O4_NONE						0x00000000UL
#define O4_MISSILE_TILE				0x00000001UL
#define O4_SINGLE_MISSILE_TILE		0x00000002UL
#define O4_FULL_SIZED_BITMAP		0x00000004UL
#define O4_DRAWN_IN_FRONT		    0x00000008UL


/* flag values for CONFERS_POWERS_TO_SPECIFIED_CHARACTERS_ONLY in nonspell_oc7 */
#define PERMITTED_ALL				0x00000000UL

#define PERMITTED_RACE_HUMAN		0x00000001UL
#define PERMITTED_RACE_ELF			0x00000002UL
#define PERMITTED_RACE_DWARF		0x00000004UL
#define PERMITTED_RACE_GNOLL		0x00000008UL
#define PERMITTED_RACE_GNOME		0x80000010UL
#define PERMITTED_RACE_ORC			0x00000020UL

#define PERMITTED_RACE_MASK (PERMITTED_RACE_HUMAN | PERMITTED_RACE_ELF | PERMITTED_RACE_DWARF | PERMITTED_RACE_GNOLL | PERMITTED_RACE_GNOME | PERMITTED_RACE_ORC)


#define PERMITTED_ROLE_ARCHEOLOGIST	0x00000040UL
#define PERMITTED_ROLE_BARBARIAN	0x00000080UL
#define PERMITTED_ROLE_CAVEMAN		0x00000100UL
#define PERMITTED_ROLE_HEALER		0x00000200UL
#define PERMITTED_ROLE_KNIGHT		0x00000400UL
#define PERMITTED_ROLE_MONK			0x00000800UL
#define PERMITTED_ROLE_PRIEST		0x00001000UL
#define PERMITTED_ROLE_ROGUE		0x00002000UL
#define PERMITTED_ROLE_RANGER		0x00004000UL
#define PERMITTED_ROLE_SAMURAI		0x00008000UL
#define PERMITTED_ROLE_TOURIST		0x00010000UL
#define PERMITTED_ROLE_VALKYRIE		0x00020000UL
#define PERMITTED_ROLE_WIZARD		0x00040000UL

#define PERMITTED_ROLE_MASK (PERMITTED_ROLE_ARCHEOLOGIST | PERMITTED_ROLE_BARBARIAN | PERMITTED_ROLE_CAVEMAN | PERMITTED_ROLE_HEALER | PERMITTED_ROLE_KNIGHT | \
	PERMITTED_ROLE_MONK | PERMITTED_ROLE_PRIEST | PERMITTED_ROLE_ROGUE | PERMITTED_ROLE_RANGER | PERMITTED_ROLE_SAMURAI | PERMITTED_ROLE_TOURIST | \
	PERMITTED_ROLE_VALKYRIE | PERMITTED_ROLE_WIZARD)

#define PERMITTED_ALIGNMENT_LAWFUL	0x00080000UL
#define PERMITTED_ALIGNMENT_NEUTRAL	0x00100000UL
#define PERMITTED_ALIGNMENT_CHAOTIC	0x00200000UL

#define PERMITTED_ALIGNMENT_MASK (PERMITTED_ALIGNMENT_LAWFUL | PERMITTED_ALIGNMENT_NEUTRAL | PERMITTED_ALIGNMENT_CHAOTIC)

#define PERMITTED_GENDER_MALE		0x00400000UL
#define PERMITTED_GENDER_FEMALE		0x00800000UL

#define PERMITTED_GENDER_MASK (PERMITTED_GENDER_MALE | PERMITTED_GENDER_FEMALE)

#define PERMITTED_CHARACTER_MASK (PERMITTED_RACE_MASK | PERMITTED_ROLE_MASK | PERMITTED_ALIGNMENT_MASK | PERMITTED_GENDER_MASK)

/* Permitted target mask */
#define ALL_TARGETS				0x00000000UL

};

struct class_sym {
    char sym;
    const char *name;
    const char *explain;
};

struct objdescr {
    const char *oc_name;  /* actual name */
    const char *oc_descr; /* description when name unknown */
	const char* oc_content_description; /* description of contents (spellbooks) */
	const char* oc_item_description; /* description of the item */
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

#define OBJ_CONTENT_DESC(otyp) (obj_descr[is_otyp_content_description_shuffled(otyp) ?  objects[(otyp)].oc_descr_idx :objects[(otyp)].oc_name_idx].oc_content_description)
#define OBJ_ITEM_DESC(otyp) (obj_descr[objects[(otyp)].oc_name_idx].oc_item_description)

#define OBJ_STAND_ANIMATION(otyp) (obj_descr[objects[(otyp)].oc_descr_idx].stand_animation)



#endif /* OBJCLASS_H */

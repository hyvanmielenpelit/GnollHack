/* GnollHack 4.0	objclass.h	$NHDT-Date: 1547255901 2019/01/12 01:18:21 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef OBJCLASS_H
#define OBJCLASS_H

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
	MAT_ADAMANTINE  = 20,
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
    ARM_SUIT   = 0,
    ARM_SHIELD = 1,        /* needed for special wear function */
    ARM_HELM   = 2,
    ARM_GLOVES = 3,
    ARM_BOOTS  = 4,
    ARM_CLOAK  = 5,
    ARM_SHIRT  = 6,
	ARM_ROBE	= 7,
	ARM_BRACERS = 8
};

enum obj_weapon_types {
	WEP_SWORD = 0,
	WEP_DAGGER = 1,
	WEP_MACE = 2,
	WEP_FLAIL = 3,
	WEP_STAFF = 4,
	WEP_HAMMER = 5,
	WEP_AXE = 6,
	WEP_SPEAR = 7,
	WEP_POLEARM = 8,
	WEP_CLUB = 9,
	WEP_TRIDENT = 10,
	WEP_DART = 11,
	WEP_SHURIKEN = 12,
	WEP_JAVELIN = 13,
	WEP_BOOMERANG = 14,
	WEP_BOW = 15,
	WEP_CROSSBOW = 16,
	WEP_SLING = 17,
	WEP_ARROW = 18,
	WEP_QUARREL = 19,
	WEP_SLING_BULLET = 20,
	WEP_PICK_AXE = 21,
	WEP_UNICORN_HORN = 22,
	WEP_HORN = 23,
	WEP_TOOTH = 24,
	WEP_KNIFE= 25,
	WEP_SHOVEL = 26,
	WEP_HOOK = 27,
	WEP_LANCE = 28,
	WEP_WHIP = 28
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
	MISC_LENSES = 12,		/* alternative implementation of eyeglasses */
	MISC_BLINDFOLD = 13,	/* alternative implementation of blindfold */
	MISC_SCARF = 14,
	MISC_EXTRA_ARMS = 15
};

static const char* misc_type_names[] = {
	"", "set of wings", "pair of pants", "skirt", "wrist watch",
	"nose ring", "headband", "pair of earrings",
	"ioun stone", "bracelet", "pair of bracers", "belt", "pair of eyeglasses", "blindfold", "scarf", "set of extra arms",
};

static const char* misc_type_worn_texts[] = {
	"", "attached to back", "", "", "on left wrist",
	"on nose", "", "",
	"orbiting head", "", "", "", "", "", "", "attached to body",
};


struct objclass {
    short oc_name_idx;              /* index of actual name */
    short oc_descr_idx;             /* description when name unknown */
    char *oc_uname;                 /* called by user */
	char* oc_content_desc;          /* high-level books: description of contents */
	char* oc_short_description;     /* spellbooks: one line summary of what the spell does; longer description is in encyclopedia */
	Bitfield(oc_name_known, 1);     /* discovered */
    Bitfield(oc_merge, 1);          /* merge otherwise equal objects */
    Bitfield(oc_uses_known, 1);     /* obj->known affects full description;
                                       otherwise, obj->dknown and obj->bknown
                                       tell all, and obj->known should always
                                       be set for proper merging behavior. */
    Bitfield(oc_pre_discovered, 1); /* Already known at start of game;
                                       won't be listed as a discovery. */
    Bitfield(oc_magic, 1);          /* inherently magical object */
    Bitfield(oc_charged, 1);        /* may have +n or (n) charges */
    Bitfield(oc_unique, 1);         /* special one-of-a-kind object */
    Bitfield(oc_nowish, 1);         /* cannot wish for this object */

    Bitfield(oc_big, 1);
#define oc_bimanual oc_big /* for weapons & tools used as weapons */
#define oc_bulky oc_big    /* for armor */
    Bitfield(oc_tough, 1); /* hard gems/rings */

    Bitfield(oc_dir, 3);
#define NODIR 1     /* for wands/spells:	non-directional */
#define IMMEDIATE 2 /*						directional */
#define RAY 3       /*						zap beams */
#define TARGETED 4  /*						targeted using throwspell */
#define TOUCH 5     /*						like immediate but touch distance */

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
#define is_rustprone(otmp) (objects[otmp->otyp].oc_material == MAT_IRON)

/* secondary damage: rot/acid/acid */
#define is_corrodeable(otmp)                   \
    (objects[otmp->otyp].oc_material == MAT_COPPER \
     || objects[otmp->otyp].oc_material == MAT_IRON)

#define is_damageable(otmp)                                        \
    (is_rustprone(otmp) || is_flammable(otmp) || is_rottable(otmp) \
     || is_corrodeable(otmp))

    /* 3 free bits */

    schar oc_subtyp;		 /* armors: armor category, miscellaneous magic items: subclass, etc.*/
	schar oc_skill;			 /* Skills of weapons, spellbooks, tools, gems */
	uchar oc_oprop, oc_oprop2, oc_oprop3; /* properties (invis, &c.) conveyed */
	
	char  oc_class; /* object class (enum obj_class_types) */
    schar oc_delay; /* delay when using such an object */
    uchar oc_color; /* color of the object */

    short oc_prob;            /* probability, used in mkobj() */
    unsigned short oc_weight; /* encumbrance (1 oz = 1/16 lb.) previously (1 cn = 0.1 lb.) */
	unsigned short oc_nutrition; /* food value */

	short oc_cost;            /* base cost in shops */
    
							  /* Check the AD&D rules!  The FIRST is small monster damage. */
    /* for weapons, and tools, rocks, and gems useful as weapons */
	int oc_damagetype;						/* Type of damage caused by the (magic) weapon, the same as for monster attacks */
	int oc_wsdice, oc_wsdam, oc_wsdmgplus;	/* small monster damage, also used for spell damage */
	int oc_wldice, oc_wldam, oc_wldmgplus;	/* large monster damage, also used for duration for spells */
	int oc_extra_damagetype;				/* Type of extra damage caused by the (magic) weapon */
	int oc_wedice, oc_wedam, oc_wedmgplus;	/* extra damage used as a special effect influenced by target permissions mask */
	long oc_aflags;							/* attack related flags, e.g. whether the attack is vorpal */

/* Attack flags for weapons, armor, weapon-like tools, and miscellaneous items */
#define AFLAGS_NONE				0x00000000
#define AFLAGS_CRITICAL_STRIKE	0x00000001  /* extra damage is caused only by chance determined by oc_dir_subtype */
#define AFLAGS_CRITICAL_STRIKE_IS_DEADLY				0x00000002  /* successful critical strike causes lethal damage */
#define AFLAGS_CRITICAL_STRIKE_DISRESPECTS_TARGETS		0x00000004  /* successful critical strike causes lethal damage */
#define AFLAGS_CRITICAL_STRIKE_DISRESPECTS_CHARACTERS	0x00000008  /* successful critical strike causes lethal damage */

#define AFLAGS_LEVEL_DRAIN		0x00000010  /* drains a level from monsters */
#define AFLAGS_LEVEL_DRAIN_DISRESPECTS_TARGETS		0x00000020  /* successful critical strike causes lethal damage */
#define AFLAGS_LEVEL_DRAIN_DISRESPECTS_CHARACTERS	0x00000040  /* successful critical strike causes lethal damage */

#define AFLAGS_WOUNDING			0x00000080  /* extra damage caused is permanent damage */
#define AFLAGS_WOUNDING_DISRESPECTS_TARGETS		0x00000100  /* successful critical strike causes lethal damage */
#define AFLAGS_WOUNDING_DISRESPECTS_CHARACTERS	0x00000200  /* successful critical strike causes lethal damage */

#define AFLAGS_LIFE_LEECH		0x00000400  /* heals hit points equal to the extra damage caused */
#define AFLAGS_LIFE_LEECH_DISRESPECTS_TARGETS		0x00000800  /* successful critical strike causes lethal damage */
#define AFLAGS_LIFE_LEECH_DISRESPECTS_CHARACTERS	0x00001000  /* successful critical strike causes lethal damage */

#define AFLAGS_SHARPNESS		0x00002000	/* 2/20 chance of the monster losing 25% of maximum hit points */
#define AFLAGS_VORPAL			0x00004000	/* 1/20 chance of the monster being beheaded */
#define AFLAGS_BISECT (AFLAGS_SHARPNESS | AFLAGS_VORPAL) /* 1/20 chance of a small monster being bisected and a big monster losing 50% of maximum hit points */
#define AFLAGS_VORPAL_LIKE_DOUBLE_CHANCE_FOR_PERMITTED_TARGETS	0x00008000
#define AFLAGS_VORPAL_LIKE_DISRESPECTS_TARGETS					0x00010000
#define AFLAGS_VORPAL_LIKE_DISRESPECTS_CHARACTERS				0x00020000

#define AFLAGS_MAGIC_MISSILE_STRIKE							0x00040000  /* showers the target with magic missiles */
#define AFLAGS_DALIGN										0x00080000  /* attack bonus on non-aligned monsters  */
#define AFLAGS_STUN											0x00100000  /* stuns target */

#define AFLAGS_EXTRA_DAMAGE_DISRESPECTS_TARGETS		0x01000000  /* successful critical strike causes lethal damage */
#define AFLAGS_EXTRA_DAMAGE_DISRESPECTS_CHARACTERS	0x02000000  /* successful critical strike causes lethal damage */

#define AFLAGS_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA				0x10000000 /* abilities such as wounding and life leech are based on full caused damage, not just extra damage */
#define AFLAGS_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS		0x20000000


/* Spell flags for spells, scrolls, potions, spell-like tools, and wands */
#define SFLAGS_NONE 0x00000000
#define SFLAGS_SPELL_EXPLOSION_EFFECT				0x00000001
#define SFLAGS_SPELL_BYPASSES_MAGIC_RESISTANCE		0x00000002
#define SFLAGS_SPELLBOOK_MUST_BE_READ_TO_IDENTIFY	0x00000004
#define SFLAGS_SPELL_IS_NONREVERSIBLE_PERMANENT		0x00000008


	int oc_hitbonus;						/* weapons: "to hit" bonus */
	int oc_range;							/* launchers: range for ammo, others throw range: >0 Fixed range, <0 Percentage of STR */

	/* general purpose */
	int oc_oc1;		/* Used for spell cooldown; weapons and armors: ac bonus */
	int oc_oc2;		/* Used for spell level; weapons and armors: mc bonus */
	int oc_oc3;		/* Used for spell mana cost; other items: mana pool bonus */
	int oc_oc4;		/* Used for spell attributes; other items: hit point bonus */
	int oc_oc5;		/* Used for spell range; non-spellbooks: specification of attributes or other properties item gives bonuses (abon) to using otmp->spe */
	int oc_oc6;		/* Used for spell radius; non-spellbooks: 0 => spe is used, otherise fixed bonus */
	int oc_oc7;		/* Used for spell casting penalty */
	int oc_oc8;		/* Used for multishot strength */

/* general*/
#define oc_armor_class oc_oc1						/* weapons and armor: AC used in ARM_AC_BONUS in do.c */
#define oc_magic_cancellation oc_oc2				/* weapons and armor: MC, i.e., resistance level to magical attacks */
#define oc_mana_bonus oc_oc3						/* non-spellbooks: mana pool bonus: Fixed points unless O1_MANA_PERCENTAGE_BONUS is specified */
#define oc_hp_bonus oc_oc4							/* non-spellbooks: hit point bonus: Fixed points unless O1_HP_PERCENTAGE_BONUS is specified */
#define oc_bonus_attributes oc_oc5					/* non-spellbooks: gives bonuses using spe / oc_oc6 to attributes and properties */
#define oc_attribute_bonus oc_oc6					/* non-spellbooks: 0 => spe is used, otherise fixed bonus */
#define oc_spell_casting_penalty oc_oc7				/* non-spells/wands: spell casting penalty when worn */
#define oc_multishot_str oc_oc8						/* strength required for launcher multishot */

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

#define BONUS_TO_ALLSTATS BONUS_TO_STR | BONUS_TO_DEX | BONUS_TO_CON | BONUS_TO_INT | BONUS_TO_WIS | BONUS_TO_CHA

#define SETS_FIXED_ATTRIBUTE 0x1000
#define FIXED_IS_MAXIMUM 0x2000
#define IGNORE_SPE 0x4000


/* weapons */
/* oc_oc1 AC bonus, to make wielded objects consistent with armors, but a wielded object needs to be a weapon */
/* oc_oc2 MC bonus, to make wielded objects consistent with armors, but a wielded object needs to be a weapon */
/* oc_oc3 mana pool bonus */
/* oc_oc4 hit point bonus */
/* oc_oc5 attributes giving bonus to using spe / oc_oc6 to attributes and properties */
/* oc_oc6 modifier to oc_oc5: 0 => spe is used, otherise fixed bonus */
/* oc_oc7 spell casting penalty */

/* armor */
#define oc_armor_category oc_subtyp					/* armor: (enum obj_armor_types) */
/* oc_oc1 AC bonus */
/* oc_oc2 MC bonus */
/* oc_oc3 mana pool bonus */
/* oc_oc4 hit point bonus */
/* oc_oc5 attributes giving bonus to using spe / oc_oc6 to attributes and properties */
/* oc_oc6 modifier to oc_oc5: 0 => spe is used, otherise fixed bonus */
/* oc_oc7 spell casting penalty */

/* comestibles and reagents (and other edibles) */
#define oc_edible_subtype oc_oc1		/* edibles: is rotten, poisoned, et*/

#define EDIBLE_NORMAL 0 
#define EDIBLE_ROTTEN 1 
#define EDIBLE_SICKENING 2 
#define EDIBLE_ACIDIC 3 
#define EDIBLE_POISONOUS 4 
#define EDIBLE_TAINTED 5 


/* spells */
#define oc_spell_cooldown oc_oc1		/* books: spell cooldown time */
#define oc_spell_level oc_oc2			/* books: spell level */
#define oc_spell_mana_cost oc_oc3		/* books: spell mana cost */
#define oc_spell_attribute oc_oc4		/* books: spell primary casting attribute */
#define oc_spell_range oc_oc5			/* books: spell range */
#define oc_spell_radius oc_oc6			/* books: spell radius */
#define oc_spell_saving_throw_adjustment oc_oc7	/* books: spell saving throw adjustment */
#define oc_spell_dmg_dice oc_wsdice		/* books: spell damage no of dice */
#define oc_spell_dmg_dicesize oc_wsdam	/* books: spell damage size of dice */
#define oc_spell_dmg_plus oc_wsdmgplus	/* books: spell damage constant added */
#define oc_spell_dur_dice oc_wldice		/* books: spell duration no of dice */
#define oc_spell_dur_dicesize oc_wldam	/* books: spell duration size of dice */
#define oc_spell_dur_plus oc_wldmgplus	/* books: spell duration constant added */

	int oc_dir_subtype;				/* spells: ID for type of ray or immediate effect, weapons: damage type */
	int oc_material_components;		/* spells: ID for material component list for a spell or to make the item (if recipe is known), other items: ID of components produced when breaking */
	int oc_item_cooldown;			/* cooldown before the item can be used / applied / zapped / read etc. again */
	int oc_item_level;				/* item level, to be used with loot tables */

	unsigned long oc_flags;			/* E.g. if indestructible or disintegration resistant */
	unsigned long oc_flags2;		/* More flags */
	unsigned long oc_flags3;		/* Even more flags */

	unsigned long oc_power_permissions; /* roles, races, genders, and alignments that the item's powers are conferred to */
	unsigned long oc_target_permissions; /* symbol, M1 flag, M2 flag, M3 flag, etc. for which extra damage is deal to */
	int oc_invokeitem;					/* Filled by item connects: Item effect when invoked, special effect if negative, 0 not invokable */

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
#define RAY_WND_DIGGING 101 
#define RAY_WND_EVAPORATION 102 

#define RAY_MAGIC_MISSILE 10 
#define RAY_FIRE 11 
#define RAY_COLD 12 
#define RAY_SLEEP 13 
#define RAY_DISINTEGRATION 14 
#define RAY_LIGHTNING 15 
#define RAY_POISON_GAS 16 
#define RAY_ACID 17 
#define RAY_DEATH 18 
#define RAY_DIGGING 111 
#define RAY_EVAPORATION 112 


/* Item resistances -- General: cannot be overriden */
#define O1_NONE								0x00000000 
#define O1_INDESTRUCTIBLE					0x00000001 
#define O1_DISINTEGRATION_RESISTANT			0x00000002 
#define O1_FIRE_RESISTANT					0x00000004 
#define O1_COLD_RESISTANT					0x00000008 
#define O1_LIGHTNING_RESISTANT				0x00000010
#define O1_POLYMORPH_RESISTANT				0x00000020
#define O1_RUST_RESISTANT					0x00000040
#define O1_CORROSION_RESISTANT				0x00000080

#define O1_SPE_AFFECTS_MC					0x00000100			/* +X of the item influences also its MC */
#define O1_EROSION_DOES_NOT_AFFECT_MC		0x00000200			/* erosion level does not affect the item's MC */
#define O1_IS_ARMOR_WHEN_WIELDED			0x00000400			/* acts as an armor when wielded giving AC using oc_armor_class, which must be specified */
#define O1_IS_WEAPON_WHEN_WIELDED			0x00000800			/* acts as a weapon when wielded (or worn in shield slot in two-weapon fighting) using damage statistics */
#define O1_READABLE							0x00002000								
#define O1_EDIBLE_NONFOOD					0x00008000
#define O1_EDIBLE_BY_BONE_EATERS			0x00010000

#define O1_MANA_PERCENTAGE_BONUS			0x00020000
#define O1_HP_PERCENTAGE_BONUS				0x00040000
#define O1_BECOMES_CURSED_WHEN_WORN			0x00080000
#define O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED 0x00100000
#define O1_CANNOT_BE_DROPPED_IF_CURSED		0x00200000

#define O1_CONFERS_LUCK						0x00400000
#define O1_CONFERS_POWERS_WHEN_CARRIED		0x00800000

#define O1_THROWN_WEAPON					0x01000000										/* says "Thrown weapon" instead of "Melee weapon", default range is larger, can use throwrange data value */
#define O1_WEIGHT_DOES_NOT_REDUCE_RANGE		0x02000000						/* the object magically flies when thrown, ignoring its weight */
#define O1_RETURNS_TO_HAND_AFTER_THROWING	0x04000000					/* the object returns to the owner's hand after throwing */
#define O1_CAN_BE_THROWN_ONLY_IF_WIELDED	0x08000000						/* has to be wielded to be thrown, e.g., Mjollnir */
#define O1_WAND_LIKE_TOOL					0x10000000									/* apply is the same as zap, uses spellbook/wand paramters and flags instead of normal flags */
#define O1_SPECIAL_ENCHANTABLE				0x20000000
#define O1_NON_SPELL_SPELLBOOK				0x40000000								/* uses non-spellbook flags and other non-spellbook stats */
#define O1_NOT_CURSEABLE					0x80000000


/* Flags 2 */
#define O2_NONE					0x00000000
#define O2_ELVEN_ITEM			0x00000001
#define O2_DWARVEN_ITEM			0x00000002
#define O2_ORCISH_ITEM			0x00000004
#define O2_GNOLLISH_ITEM		0x00000008
#define O2_DRAGON_ITEM			0x00000010
#define O2_DEMON_ITEM			0x00000020
#define O2_ANGELIC_ITEM			0x00000040
#define O2_MODRON_ITEM			0x00000080

#define O2_CONTAINER			0x00000100	
#define O2_CONTAINER_BOX		0x00000200	
#define O2_CONTAINER_MAGIC_BAG	0x00000400	
#define O2_CONTAINER_WEIGHT_REDUCING_MAGIC_BAG 0x00000800

#define O2_MONSTER_SCALES		0x00001000	
#define O2_MONSTER_SCALE_MAIL	0x00002000
#define O2_CANDLE				0x00004000	
#define O2_SHINES_MAGICAL_LIGHT 0x00008000	
#define O2_FLICKER_COLOR_WHITE	0x00010000	
#define O2_FLICKER_COLOR_BLUE	0x00020000	
#define O2_FLICKER_COLOR_BLACK (O2_FLICKER_COLOR_WHITE | O2_FLICKER_COLOR_BLUE)	
#define O2_IGNITABLE			0x00040000	
#define O2_RELATIVE_AGE			0x00080000	

#define O2_GRAYSTONE			0x00100000	
#define O2_ROCK					0x00200000	
#define O2_ORE					0x00400000	

#define O2_FLIMSY				0x00800000	

#define O2_GENERATED_DEATH_OR_LIGHTNING_ENCHANTED 0x10000000			/* if deathenchantable item, then death, otherwise lightning*/
#define O2_GENERATED_BLESSED	0x20000000								/* Always generated blessed */									
#define O2_GENERATED_CURSED		0x40000000								/* Always generated cursed */								
#define O2_CURSED_MAGIC_ITEM	0x80000000								/* The effect of the item is negative; typically 90% chance of being cursed */									

/* Artifact flags - to do: Attack type, half physical and spell damage, bonus to monsters only */
/* Flags 3 */
#define O3_NONE				0x00000000
#define O3_NOWISH			0x00000001  /* item is special, it cannot be wished for */
#define O3_POWER_1_DISRESPECTS_CHARACTERS			0x00000002 
#define O3_POWER_2_DISRESPECTS_CHARACTERS			0x00000004 
#define O3_POWER_3_DISRESPECTS_CHARACTERS			0x00000008 
#define O3_HP_BONUS_DISRESPECTS_CHARACTERS			0x00000010
#define O3_MANA_BONUS_DISRESPECTS_CHARACTERS		0x00000020
#define O3_ATTRIBUTE_BONUS_DISRESPECTS_CHARACTERS	0x00000040
#define O3_LUCK_DISRESPECTS_CHARACTERS				0x00000080

#define O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS	0x00000400 /* wielding or wearing prohibits the revival of permitted targets */
#define O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS	0x00000800 /* TODO: wielding or wearing prohibits summoning by permitted targets */
#define O3_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS	0x00001000	/* deals damage when wielded like artifacts */

#define O3_INVOKABLE		0x00002000	/* can be invoked using invoke command */
#define O3_APPLIABLE		0x00004000	/* can be applied as a tool */
#define O3_WIELDABLE		0x00008000	/* can be wielded in a weapon slot */

/* One flag slot here */

#define O3_PERMTTED_TARGET_LAWFUL	0x00100000
#define O3_PERMTTED_TARGET_NEUTRAL	0x00200000
#define O3_PERMTTED_TARGET_CHAOTIC	0x00400000
/* One flag slot here */

#define O3_TARGET_PERMISSION_IS_M1_FLAG 0x10000000 /* Note: if no flag, then default is a monster symbol */
#define O3_TARGET_PERMISSION_IS_M2_FLAG 0x20000000
#define O3_TARGET_PERMISSION_IS_M3_FLAG 0x40000000 






/* flag values for CONFERS_POWERS_TO_SPECIFIED_CHARACTERS_ONLY in nonspell_oc7 */
#define PERMITTED_ALL				0x00000000

#define PERMITTED_RACE_HUMAN		0x00000001
#define PERMITTED_RACE_ELF			0x00000002
#define PERMITTED_RACE_DWARF		0x00000004
#define PERMITTED_RACE_GNOLL		0x00000008
#define PERMITTED_RACE_ORC			0x00000010

#define PERMITTED_RACE_MASK (PERMITTED_RACE_HUMAN | PERMITTED_RACE_ELF | PERMITTED_RACE_DWARF | PERMITTED_RACE_GNOLL | PERMITTED_RACE_ORC)


#define PERMITTED_ROLE_ARCHEOLOGIST	0x00000020
#define PERMITTED_ROLE_BARBARIAN	0x00000040
#define PERMITTED_ROLE_CAVEMAN		0x00000080
#define PERMITTED_ROLE_HEALER		0x00000100
#define PERMITTED_ROLE_KNIGHT		0x00000200
#define PERMITTED_ROLE_MONK			0x00000400
#define PERMITTED_ROLE_PRIEST		0x00000800
#define PERMITTED_ROLE_ROGUE		0x00001000
#define PERMITTED_ROLE_RANGER		0x00002000
#define PERMITTED_ROLE_SAMURAI		0x00004000
#define PERMITTED_ROLE_TOURIST		0x00008000
#define PERMITTED_ROLE_VALKYRIE		0x00010000
#define PERMITTED_ROLE_WIZARD		0x00020000

#define PERMITTED_ROLE_MASK (PERMITTED_ROLE_ARCHEOLOGIST | PERMITTED_ROLE_BARBARIAN | PERMITTED_ROLE_CAVEMAN | PERMITTED_ROLE_HEALER | PERMITTED_ROLE_KNIGHT | \
	PERMITTED_ROLE_MONK | PERMITTED_ROLE_PRIEST | PERMITTED_ROLE_ROGUE | PERMITTED_ROLE_RANGER | PERMITTED_ROLE_SAMURAI | PERMITTED_ROLE_TOURIST | \
	PERMITTED_ROLE_VALKYRIE | PERMITTED_ROLE_WIZARD)

#define PERMITTED_ALIGNMENT_LAWFUL	0x00040000
#define PERMITTED_ALIGNMENT_NEUTRAL	0x00080000
#define PERMITTED_ALIGNMENT_CHAOTIC	0x00100000

#define PERMITTED_ALIGNMENT_MASK (PERMITTED_ALIGNMENT_LAWFUL | PERMITTED_ALIGNMENT_NEUTRAL | PERMITTED_ALIGNMENT_CHAOTIC)

#define PERMITTED_GENDER_MALE		0x00200000
#define PERMITTED_GENDER_FEMALE		0x00400000

#define PERMITTED_GENDER_MASK (PERMITTED_GENDER_MALE | PERMITTED_GENDER_FEMALE)

#define PERMITTED_CHARACTER_MASK (PERMITTED_RACE_MASK | PERMITTED_ROLE_MASK | PERMITTED_ALIGNMENT_MASK | PERMITTED_GENDER_MASK)

/* Permitted target mask */
#define ALL_TARGETS				0x00000000

};

struct class_sym {
    char sym;
    const char *name;
    const char *explain;
};

struct objdescr {
    const char *oc_name;  /* actual name */
    const char *oc_descr; /* description when name unknown */
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

    MAXOCLASSES  = 20
};

#define ALLOW_COUNT (MAXOCLASSES + 1) /* Can be used in the object class    */
#define ALL_CLASSES (MAXOCLASSES + 2) /* input to getobj().                 */
#define ALLOW_NONE  (MAXOCLASSES + 3)

#define BURNING_OIL (MAXOCLASSES + 1) /* Can be used as input to explode.   */
#define MON_EXPLODE (MAXOCLASSES + 2) /* Exploding monster (e.g. gas spore) */

#if 0 /* moved to decl.h so that makedefs.c won't see them */
extern const struct class_sym
        def_oc_syms[MAXOCLASSES];       /* default class symbols */
extern uchar oc_syms[MAXOCLASSES];      /* current class symbols */
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

struct fruit {
    char fname[PL_FSIZ];
    int fid;
    struct fruit *nextf;
};
#define newfruit() (struct fruit *) alloc(sizeof(struct fruit))
#define dealloc_fruit(rind) free((genericptr_t)(rind))

#define OBJ_NAME(obj) (obj_descr[(obj).oc_name_idx].oc_name)
#define OBJ_DESCR(obj) (obj_descr[(obj).oc_descr_idx].oc_descr)



#endif /* OBJCLASS_H */

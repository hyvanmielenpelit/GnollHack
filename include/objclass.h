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
    LIQUID      =  1, /* currently only for venom */
    WAX         =  2,
    VEGGY       =  3, /* foodstuffs */
    FLESH       =  4, /*   ditto    */
    PAPER       =  5,
    CLOTH       =  6,
	SILK		=  7,
	LEATHER     =  8,
    WOOD        =  9,
    BONE        = 10,
    DRAGON_HIDE = 11, /* not leather! */
    IRON        = 12, /* Fe - includes steel */
    METAL       = 13, /* Sn, &c. */
    COPPER      = 14, /* Cu - includes brass */
    SILVER      = 15, /* Ag */
    GOLD        = 16, /* Au */
    PLATINUM    = 17, /* Pt */
    MITHRIL     = 18,
    PLASTIC     = 19,
    GLASS       = 20,
    GEMSTONE    = 21,
    MINERAL     = 22,
	PLANARRIFT	= 23,
	FORCEFIELD  = 24
};

enum obj_armor_types {
    ARM_SUIT   = 0,
    ARM_SHIELD = 1,        /* needed for special wear function */
    ARM_HELM   = 2,
    ARM_GLOVES = 3,
    ARM_BOOTS  = 4,
    ARM_CLOAK  = 5,
    ARM_SHIRT  = 6,
	ARM_ROBE = 7,
	ARM_BRACERS = 8,
	ARM_BELT = 9,
	ARM_PANTS = 10
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
    Bitfield(oc_charged, 1);        /* may have +n or (n) charges */
    Bitfield(oc_unique, 1);         /* special one-of-a-kind object */
    Bitfield(oc_nowish, 1);         /* cannot wish for this object */

    Bitfield(oc_big, 1);
#define oc_bimanual oc_big /* for weapons & tools used as weapons */
#define oc_bulky oc_big    /* for armor */
    Bitfield(oc_tough, 1); /* hard gems/rings */

    Bitfield(oc_dir, 3);
#define NODIR 1     /* for wands/spells: non-directional */
#define IMMEDIATE 2 /*               directional */
#define RAY 3       /*               zap beams */
#define TARGETED 4  /*               targeted using throwspell */
#define TOUCH 5       /*             like immediate but touch distance */

#define PIERCE 1 /* for weapons & tools used as weapons */
#define SLASH 2  /* (latter includes iron ball & chain) */
#define WHACK 0
	/* 4 free bits */

    Bitfield(oc_material, 5); /* one of obj_material_types */

#define is_organic(otmp) (objects[otmp->otyp].oc_material <= WOOD)
#define is_metallic(otmp)                    \
    (objects[otmp->otyp].oc_material >= IRON \
     && objects[otmp->otyp].oc_material <= MITHRIL)

/* primary damage: fire/rust/--- */
/* is_flammable(otmp), is_rottable(otmp) in mkobj.c */
#define is_rustprone(otmp) (objects[otmp->otyp].oc_material == IRON)

/* secondary damage: rot/acid/acid */
#define is_corrodeable(otmp)                   \
    (objects[otmp->otyp].oc_material == COPPER \
     || objects[otmp->otyp].oc_material == IRON)

#define is_damageable(otmp)                                        \
    (is_rustprone(otmp) || is_flammable(otmp) || is_rottable(otmp) \
     || is_corrodeable(otmp))

    /* 3 free bits */

    schar oc_subtyp;
#define oc_skill oc_subtyp  /* Skills of weapons, spellbooks, tools, gems */
#define oc_armcat oc_subtyp /* for armor (enum obj_armor_types) */

    uchar oc_oprop, oc_oprop2, oc_oprop3; /* property (invis, &c.) conveyed */
	
	char  oc_class; /* object class (enum obj_class_types) */
    schar oc_delay; /* delay when using such an object */
    uchar oc_color; /* color of the object */

    short oc_prob;            /* probability, used in mkobj() */
    unsigned short oc_weight; /* encumbrance (1 oz = 1/16 lb.) previously (1 cn = 0.1 lb.) */
    short oc_cost;            /* base cost in shops */
    /* Check the AD&D rules!  The FIRST is small monster damage. */
    /* for weapons, and tools, rocks, and gems useful as weapons */
	int oc_wsdice, oc_wsdam, oc_wsdmgplus; /* small monster damage, also used for spell damage */
	int oc_wldice, oc_wldam, oc_wldmgplus; /* large monster damage, also used for duration for spells */
	int oc_oc1;
	int oc_oc2, oc_oc3; //Spell levels can be negative, spell mana cost can be over 255
	int oc_oc4;  //Used for spell attributes and duration

/* weapons */
#define oc_hitbon oc_oc1		/* weapons: "to hit" bonus */

/* armor */
#define a_ac oc_oc1				/* armor class, used in ARM_BONUS in do.c */
#define a_can oc_oc2			/* armor: used in mhitu.c */

/* spells */
#define oc_spell_cooldown oc_oc1		/* books: spell cooldown time */
#define oc_spell_level oc_oc2			/* books: spell level */
#define oc_spell_mana_cost oc_oc3		/* books: spell mana cost */
#define oc_spell_attribute oc_oc4		/* books: spell primary casting attribute */
#define oc_spell_dmg_dice oc_wsdice		/* books: spell damage no of dice */
#define oc_spell_dmg_dicesize oc_wsdam	/* books: spell damage size of dice */
#define oc_spell_dmg_plus oc_wsdmgplus	/* books: spell damage constant added */
#define oc_spell_dur_dice oc_wldice		/* books: spell duration no of dice */
#define oc_spell_dur_dicesize oc_wldam	/* books: spell duration size of dice */
#define oc_spell_dur_plus oc_wldmgplus	/* books: spell duration constant added */

    unsigned short oc_nutrition; /* food value */

	int oc_dir_subtype; /* ID for type of ray or immediate effect */

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

	int oc_material_components;		/* ID for material component list for a spell or to make the item (if recipe is known) */
	int oc_item_cooldown;			/* cooldown before the item can be used / applied / zapped / read etc. again */
	int oc_item_level;				/* item level, to be used with loot tables */
	unsigned long oc_flags;			/* E.g. if indestructible or disintegration resistant */

#define O1_NONE 0x00000000 
#define O1_INDESTRUCTIBLE 0x00000001 
#define O1_DISINTEGRATION_RESISTANT 0x00000002 
#define O1_FIRE_RESISTANT 0x00000004 
#define O1_COLD_RESISTANT 0x00000008 
#define O1_LIGHTNING_RESISTANT 0x00000010
#define O1_POLYMORPH_RESISTANT 0x00000020
#define O1_RUST_RESISTANT 0x00000040
#define O1_CORROSION_RESISTANT 0x00000080

#define O1_SPELL_EXPLOSION_EFFECT 0x00000100
#define O1_SPELL_BYPASSES_MAGIC_RESISTANCE 0x00000200

#define O1_NO_SPELL_CASTING_PENALTY 0x00001000
#define O1_HALF_SPELL_CASTING_PENALTY 0x00002000

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

    MAXOCLASSES  = 19
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
#define REAGENT_SYM '\''

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

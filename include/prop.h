/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    prop.h    $NHDT-Date: 1547514641 2019/01/15 01:10:41 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.20 $ */
/* Copyright (c) 1989 Mike Threepoint                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef PROP_H
#define PROP_H

#include "botl.h"
#include "tiledata.h"

#define NO_POWER 0

/* Resistances to troubles */
enum prop_types {
    FIRE_IMMUNITY     =  1,
    COLD_IMMUNITY     =  2,
    SLEEP_RESISTANCE  =  3,
    DISINTEGRATION_RESISTANCE =  4,
    SHOCK_IMMUNITY    =  5,
    POISON_RESISTANCE =  6,
    ACID_IMMUNITY     =  7,
    STONE_RESISTANCE  =  8,
    /* note: for the first eight properties, MR_xxx == (1 << (xxx_RESISTANCE/IMMUNITY - 1)) */
    DRAIN_RESISTANCE  =  9,
    SICK_RESISTANCE   = 10,
    INVULNERABLE      = 11,
    ANTIMAGIC         = 12,
    /* Troubles */
    STUNNED           = 13,
    CONFUSION         = 14,
    BLINDED           = 15,
    DEAF              = 16,
    SICK              = 17,
    STONED            = 18,
    STRANGLED         = 19,
    VOMITING          = 20,
    GLIB              = 21,
    SLIMED            = 22,
    HALLUC            = 23,
    HALLUC_RES        = 24,
    FUMBLING          = 25,
    WOUNDED_LEGS      = 26,
    SLEEPY            = 27,
    HUNGER            = 28,
    /* Vision and senses */
    SEE_INVISIBLE     = 29,
    TELEPAT           = 30,  /* Blind telepathy is now its own property type*/
    WARNING           = 31,
    WARN_OF_MON       = 32,  /* Obsolete -- JG */
    WARN_UNDEAD       = 33,
    SEARCHING         = 34,
    CLAIRVOYANT       = 35,
    INFRAVISION       = 36,
    DETECT_MONSTERS   = 37,
    /* Appearance and behavior */
    ADORNED           = 38,
    INVISIBILITY      = 39,
    DISPLACED         = 40,
    STEALTH           = 41,
    AGGRAVATE_MONSTER = 42,
    CONFLICT          = 43,
    /* Transportation */
    JUMPING           = 44,
    TELEPORT          = 45,
    TELEPORT_CONTROL  = 46,
    LEVITATION        = 47,
    FLYING            = 48,
    WATER_WALKING     = 49,
    SWIMMING          = 50,
    MAGICAL_BREATHING = 51,
    PASSES_WALLS      = 52,
    /* Physical attributes */
    SLOW_DIGESTION    = 53,
    HALF_SPELL_DAMAGE = 54,
    HALF_PHYSICAL_DAMAGE = 55,
    REGENERATION      = 56,
    ENERGY_REGENERATION = 57,
    MAGICAL_PROTECTION = 58,
    PROT_FROM_SHAPE_CHANGERS = 59,
    POLYMORPH         = 60,
    POLYMORPH_CONTROL = 61,
    UNCHANGING        = 62,
    FAST              = 63,
    REFLECTING        = 64,
    FREE_ACTION       = 65,
    FIXED_ABIL        = 66,
    LIFESAVED         = 67,
    /* New */
    DEATH_RESISTANCE  = 68,
    LYCANTHROPY_RESISTANCE = 69,
    CURSE_RESISTANCE  = 70,
    BLIND_TELEPATHY   = 71,
    ENHANCED_VISION   = 72,
    FLASH_RESISTANCE  = 73,
    EYE_PROTECTION    = 74,
    BRAIN_PROTECTION  = 75,
    WARN_DEMON        = 76,
    WARN_ORC          = 77,
    WARN_TROLL        = 78,
    WARN_GIANT        = 79,
    WARN_DRAGON       = 80,
    WARN_ELF          = 81,
    WARN_DWARF        = 82,
    WARN_GNOLL        = 83,
    WARN_HUMAN        = 84,
    WARN_LYCANTHROPE  = 85,
    WARN_ANGEL        = 86,
    WARN_OGRE         = 87,
    WARN_GNOME        = 88,
    CHARM_RESISTANCE  = 89,
    MIND_SHIELDING    = 90,
    ODD_IDEAS         = 91,
    AIRLESS_ENVIRONMENT = 92,
    MAGICAL_KICKING   = 93,
    BLOCKS_INVISIBILITY = 94,
    BLOCKS_BLINDNESS  = 95,
    BLOCKS_CLAIRVOYANCE = 96,
    HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS = 97,
    MAGICAL_SHIELDING = 98,
    MAGICAL_BARKSKIN  = 99,
    MAGICAL_STONESKIN = 100,
    XRAY_VISION       = 101,
    VERY_FAST         = 102,
    SLOWED            = 103,
    CANCELLED         = 104,
    HALVED_MAGIC_RESISTANCE = 105,
    NO_MAGIC_RESISTANCE = 106,
    CHARMED           = 107,
    PARALYZED         = 108,
    FEARFUL           = 109,
    FEAR_RESISTANCE   = 110,
    SLEEPING          = 111,
    SUMMON_FORBIDDEN  = 112,
    SILENCED          = 113,
    LEVITATION_CONTROL= 114,
    FIRE_VULNERABILITY = 115,
    COLD_VULNERABILITY = 116,
    SHOCK_VULNERABILITY = 117,
    MAGIC_MISSILE_VULNERABILITY = 118,
    DOUBLE_PHYSICAL_DAMAGE = 119,
    DOUBLE_SPELL_DAMAGE = 120,
    ENHANCED_UNTRAP   = 121,
    BLOCKS_LEVITATION = 122,
    BLOCKS_FLYING     = 123,
    ONE_FOURTH_MAGIC_RESISTANCE = 124,
    THREE_FOURTHS_MAGIC_RESISTANCE = 125,
    BLINDFOLDED       = 126,
    TITAN_STRENGTH    = 127,
    MAGIC_MISSILE_IMMUNITY = 128,
    STUN_RESISTANCE   = 129,
    FOOD_POISONED     = 130,
    BISECTION_RESISTANCE = 131,
    DIVINE_ENDURANCE  = 132,
    DIVINE_INTELLECT  = 133,
    DIVINE_WISDOM     = 134,
    DIVINE_DEXTERITY  = 135,
    DIVINE_CHARISMA   = 136,
    MUMMY_ROT         = 137,
    FIRE_RESISTANCE   = 138,
    COLD_RESISTANCE   = 139,
    SHOCK_RESISTANCE  = 140,
    MAGIC_MISSILE_RESISTANCE = 141,
    ACID_RESISTANCE   = 142,
    ULTRA_FAST        = 143,
    SUPER_FAST        = 144,
    LIGHTNING_FAST    = 145,
    HEROISM           = 146,
    SUPER_HEROISM     = 147,
    RAPID_REGENERATION = 148,
    RAPIDER_REGENERATION = 149,
    RAPIDEST_REGENERATION = 150,
    RAPID_ENERGY_REGENERATION = 151,
    RAPIDER_ENERGY_REGENERATION = 152,
    RAPIDEST_ENERGY_REGENERATION = 153,
    WOUNDING_RESISTANCE = 154,
    CRAZED            = 155,
    DIVINE_REGENERATION = 156,
    IMPROVED_FIRE_RESISTANCE = 157,
    IMPROVED_COLD_RESISTANCE = 158,
    IMPROVED_SHOCK_RESISTANCE = 159,
    IMPROVED_MAGIC_MISSILE_RESISTANCE = 160,
    IMPROVED_ACID_RESISTANCE = 161,
    UNDEAD_IMMOBILITY = 162,
    UNDEAD_CONTROL    = 163,
    CANCELLATION_RESISTANCE = 164,
    HALF_SLOW_DIGESTION = 165,
    SLIME_RESISTANCE = 166,
    LAUGHING          = 167,
    TOTTERING         = 168,
    MARTIAL_PROWESS   = 169,
    MAX_PROPS         = 170
};
#define LAST_PROP (MAX_PROPS - 1)

/*** Where the properties come from ***/
/* Definitions were moved here from obj.h and you.h */
struct prop {
    /*** Properties conveyed by objects ***/
    int64_t extrinsic;
/* Armor */
#define W_ARM  0x00000001L  /* Body armor */
#define W_ARMC 0x00000002L  /* Cloak */
#define W_ARMH 0x00000004L  /* Helmet/hat */
#define W_ARMS 0x00000008L  /* Shield */
#define W_ARMG 0x00000010L  /* Gloves/gauntlets */
#define W_ARMF 0x00000020L  /* Footwear */
#define W_ARMU 0x00000040L  /* Undershirt */
#define W_ARMO 0x00000080L  /* Robe or overcoat */
#define W_ARMB 0x00000100L  /* Bracers */

#define W_ARMOR (W_ARM | W_ARMC | W_ARMH | W_ARMS | W_ARMG | W_ARMF | W_ARMU | W_ARMO | W_ARMB)


/* Weapons */
#define W_WEP         0x00000200L        /* Wielded weapon in the primary hand */
#define W_WEP2        W_ARMS             /* Wielded weapon in the secondary hand (uses the same hand as W_ARMS) */
#define W_AUTOSTASH   0x00000400L        /* Autostash for putting items in automatically; NOT USED currently */
#define W_QUIVER      0x00000800L        /* Quiver for (f)iring ammo */
#define W_SWAPWEP     0x00001000L        /* Alternate weapon in the primary hand */
#define W_SWAPWEP2    0x00002000L        /* Alternate weapon/shield in the secondary hand */

#define W_ENVIRONMENT 0x00004000L        /* Caused by environment, e.g., suffocation due to no air */
#define W_STUCK       0x00008000L        /* A monster in u.ustuck had grabbed you and is e.g. constricting you */

#define W_WIELDED_WEAPON (W_WEP | W_WEP2)
#define W_SWAP_WEAPON (W_SWAPWEP | W_SWAPWEP2)
#define W_WEAPON (W_WIELDED_WEAPON | W_SWAP_WEAPON | W_QUIVER)

/* Amulets, rings, tools, and other items */
#define W_AMUL  0x00010000L      /* Amulet */
#define W_RINGL 0x00020000L      /* Left ring */
#define W_RINGR 0x00040000L      /* Right ring */
#define W_RING (W_RINGL | W_RINGR)
#define W_BLINDFOLD 0x00080000L  /* Eyewear */

    /* historical note: originally in slash'em, 'worn' saddle stayed in
       hero's inventory; in GnollHack, it's kept in the steed's inventory */
#define W_SADDLE 0x00100000L     /* KMH -- For riding monsters */
#define W_BALL   0x00200000L     /* Punishment ball */
#define W_CHAIN  0x00400000L     /* Punishment chain */

    /* new accessories*/
#define W_MISC  0x00800000L      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
#define W_MISC2 0x01000000L      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
#define W_MISC3 0x02000000L      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
#define W_MISC4 0x04000000L      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */
#define W_MISC5 0x08000000L      /* Special miscellaneous item, such as a belt, a brooch, bracelet, nose ring */

#define W_MISCITEMS (W_MISC  | W_MISC2 | W_MISC3 | W_MISC4 | W_MISC5)
#define W_ACCESSORY (W_RING | W_AMUL | W_MISCITEMS | W_BLINDFOLD)
#define W_WORN_NOT_WIELDED ((W_ARMOR & ~W_ARMS) | W_ACCESSORY)
#define W_WORN_AND_WIELDED (W_WORN_NOT_WIELDED | W_WIELDED_WEAPON)

/* Special */
#define W_ARTIFACT_CARRIED 0x10000000L     /* Carrying artifact (not really worn) */
/* 0x20000000 used by I_SPECIAL */
#define W_ARTIFACT_INVOKED 0x40000000L     /* Invoked artifact  (not really worn) */
#define W_CARRIED 0x80000000L              /* Carried */

    /*** Timeouts, permanent properties, and other flags ***/
    int64_t intrinsic;

/* Timed properties */
#define TIMEOUT 0x00ffffffL       /* Up to 16 million turns */
                                  /* Permanent properties */
#define FROM_ROLE 0x01000000L     /* Gain/lose with experience, for role */
#define FROM_RACE 0x02000000L     /* Gain/lose with experience, for race */
#define FROM_ACQUIRED 0x04000000L /* By corpses, prayer, thrones, etc. */
#define INTRINSIC (FROM_ACQUIRED | FROM_RACE | FROM_ROLE)
/* Control flags */
#define FROM_FORM 0x10000000L     /* Polyd; conferred by monster form */
#define I_SPECIAL 0x20000000L     /* Property is controllable */
};

struct prop_color {
    uchar r, g, b;
};
struct prop_info {
    const char* prop_tile_name;

    boolean show_buff;
    boolean buff_text_needs_background;
    boolean recurring;            /* Is property recurring? EProperty causes timeout in HProperty to increase by recurring_constant + rnd(recurring_random) */

    short recurring_constant;
    short recurring_random;

    struct prop_color buff_text_color;
    struct prop_color buff_bk_color;

    uint64_t pflags;
};

#define PROPFLAGS_NONE                  0x00000000UL
#define PROPFLAGS_BUFF_CANCELLABLE      0x00000001UL


extern struct prop_info property_definitions[MAX_PROPS];

/*** Definitions for backwards compatibility ***/
#define LEFT_RING W_RINGL
#define RIGHT_RING W_RINGR
#define LEFT_SIDE LEFT_RING
#define RIGHT_SIDE RIGHT_RING
#define BOTH_SIDES (LEFT_SIDE | RIGHT_SIDE)
#define WORN_ARMOR W_ARM
#define WORN_CLOAK W_ARMC
#define WORN_HELMET W_ARMH
#define WORN_SHIELD W_ARMS
#define WORN_GLOVES W_ARMG
#define WORN_BOOTS W_ARMF
#define WORN_AMUL W_AMUL
#define WORN_BLINDF W_BLINDFOLD
#define WORN_SHIRT W_ARMU
#define WORN_ROBE W_ARMO
#define WORN_BRACERS W_ARMB
#define WORN_MISCELLANEOUS W_MISC
#define WORN_MISCELLANEOUS2 W_MISC2
#define WORN_MISCELLANEOUS3 W_MISC3
#define WORN_MISCELLANEOUS4 W_MISC4
#define WORN_MISCELLANEOUS5 W_MISC5
#define CARRIED_ITEM W_CARRIED

struct propname {
    int prop_num;
    const char* prop_name;
    const char* prop_noun;
    const char* prop_desc;
};

extern const struct propname propertynames[];
extern const char* condition_names[NUM_BL_CONDITIONS];
extern const char* status_names[MAX_STATUS_MARKS];

extern enum game_ui_status_mark_types statusmarkorder[MAX_STATUS_MARKS];

#endif /* PROP_H */

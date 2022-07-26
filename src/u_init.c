/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    u_init.c    $NHDT-Date: 1539510426 2018/10/14 09:47:06 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.43 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

struct trobj {
    short trotyp;
    schar trspe;
    char trclass;
    Bitfield(trquan, 6);
    Bitfield(trbless, 2);
    uchar elemental_enchantment;
    uchar exceptionality;
};

STATIC_DCL void FDECL(ini_inv, (struct trobj *));
STATIC_DCL void FDECL(knows_object, (int));
#if 0
STATIC_DCL void FDECL(knows_class, (CHAR_P));
#endif
STATIC_DCL boolean FDECL(restricted_spell_discipline, (int));
STATIC_DCL void NDECL(add_school_specific_spellbooks);
STATIC_DCL struct obj* FDECL(mk_obj_in_container_known, (struct obj*, int));

#define UNDEF_TYP 0
#define UNDEF_SPE '\177'
#define UNDEF_BLESS 2

/*
 *      Initial inventory for the various roles.
 */

static struct trobj Archaeologist[] = {
    /* if adventure has a name...  idea from tan@uvm-gen */
    { BULLWHIP, 3, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { LEATHER_JACKET, 2, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { HAND_CROSSBOW, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { CROSSBOW_BOLT, 0, WEAPON_CLASS, 15, UNDEF_BLESS, 0, 0 },
    { CROSSBOW_BOLT, 0, WEAPON_CLASS, 6, UNDEF_BLESS, LIGHTNING_ENCHANTMENT, 0 },
    { FEDORA, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { HANDFUL_OF_NUTS, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { PICK_AXE, 2, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { TINNING_KIT, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { WAN_DIGGING, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { TOUCHSTONE, 0, GEM_CLASS, 1, 0, 0, 0 },
    { BACKPACK, 0, TOOL_CLASS, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Barbarian[] = {
#define B_MAJOR 0 /* two-handed sword or battle-axe  */
#define B_MINOR 1 /* matched with axe or short sword */
    { TWO_HANDED_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { AXE, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { RING_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SACK, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Cave_man[] = {
#define C_AMMO 2
    { CLUB, 3, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SLING, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { FLINT, 0, GEM_CLASS, 15, UNDEF_BLESS, 0, 0 },
    { STONE_PEBBLE, 0, GEM_CLASS, 3, 0, 0, 0 },             /* yields 18..33 */
    { PRAYERSTONE, 0, GEM_CLASS, 1, 1, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, AMULET_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SACK, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 3, 0, 0 },
    { LEATHER_ARMOR, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};

static struct trobj Healer[] = {
    { SCALPEL, 3, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { LEATHER_GLOVES, 3, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { MEDIEVAL_ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { BEAK_MASK_OF_SICKNESS_RESISTANCE, 0, MISCELLANEOUS_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { LEATHER_HAT, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { STETHOSCOPE, 0, TOOL_CLASS, 1, 0, 0 },
    { POT_HEALING, 0, POTION_CLASS, 4, UNDEF_BLESS, 0, 0 },
    { POT_EXTRA_HEALING, 0, POTION_CLASS, 4, UNDEF_BLESS, 0, 0 },
    { JAR_OF_EXTRA_HEALING_SALVE, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { JAR_OF_MEDICINAL_SALVE, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { WAN_PROBING, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { WAN_SLEEP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0, 0 },
    /* always blessed, so it's guaranteed readable */
    { SPE_MINOR_HEALING, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_HEALING, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_EXTRA_HEALING, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_CURE_SICKNESS, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_SLEEP, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_PROBE, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { GINSENG_ROOT, 0, REAGENT_CLASS, 4, 0, 0, 0 },
    { MANDRAKE_ROOT, 0, REAGENT_CLASS, 4, 0, 0, 0 },
    { CLOVE_OF_GARLIC, 0, REAGENT_CLASS, 2, 0, 0, 0 },
    { APPLE, 0, FOOD_CLASS, 5, 0, 0, 0 },
    { LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};

static struct trobj Knight[] = {
    { LONG_SWORD, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { LANCE, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { PLATE_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { HELMET, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { APPLE, 0, FOOD_CLASS, 2, 0, 0, 0 },
    { CARROT, 0, FOOD_CLASS, 2, 0, 0, 0 },
    { LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Monk[] = {
#define M_FOOD_SPELLBOOK 8
    { LEATHER_GLOVES, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SIMPLE_GOWN, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { POT_HEALING, 0, POTION_CLASS, 3, UNDEF_BLESS, 0, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { APPLE, 0, FOOD_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { ORANGE, 0, FOOD_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { HANDFUL_OF_NUTS, 0, FOOD_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SPE_MINOR_HEALING, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_CREATE_FOOD, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    /* Yes, we know fortune cookies aren't really from China.  They were
     * invented by George Jung in Los Angeles, California, USA in 1916.
     */
    { FORTUNE_COOKIE, 0, FOOD_CLASS, 3, UNDEF_BLESS, 0, 0 },
    { ORIENTAL_SILK_SACK, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Priest[] = {
    { BANDED_MAIL, 0, ARMOR_CLASS, 1, 1, 0, 0 },
    { CLERICAL_GOWN, 0, ARMOR_CLASS, 1, 1, 0, 0 },
    { HOLY_SYMBOL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 }, //Blessed holy symbol is even more powerful
    { POT_WATER, 0, POTION_CLASS, 4, 1, 0, 0 }, /* holy water */
    { CLOVE_OF_GARLIC, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { SPRIG_OF_WOLFSBANE, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { GINSENG_ROOT, 0, REAGENT_CLASS, 2, 0, 0, 0 },
    { SPE_MINOR_HEALING, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_HEALING, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_CONGREGATE, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 2, UNDEF_BLESS, 0, 0 },
    { LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Ranger[] = {
#define RAN_BOW 1
#define RAN_ONE_ARROWS 2
#define RAN_ZERO_ARROWS 3
    { DAGGER, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SHORT_BOW, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { ARROW, 1, WEAPON_CLASS, 15, UNDEF_BLESS, 0, 0 },
    { ARROW, 0, WEAPON_CLASS, 25, UNDEF_BLESS, 0, 0 },
    { LEATHER_BRACERS, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { CLOAK_OF_PROTECTION, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { BACKPACK, 0, TOOL_CLASS, 1, 0, 0, 0 },
    { CRAM_RATION, 0, FOOD_CLASS, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Rogue[] = {
#define R_DAGGERS 1
    { SHORT_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { DAGGER, 0, WEAPON_CLASS, 5, 0, 0 }, /* quan is variable */
    { HAND_CROSSBOW, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { CROSSBOW_BOLT, 1, WEAPON_CLASS, 15, UNDEF_BLESS, 0, 0 },
    { LEATHER_ARMOR, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { COTTON_HOOD, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { POT_POISON, 0, POTION_CLASS, 1, 0, 0, 0 },
    { LOCK_PICK, 0, TOOL_CLASS, 1, 0, 0, 0 },
    { BAG_OF_TREASURE_HAULING, 0, TOOL_CLASS, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Samurai[] = {
#define S_ARROWS 3
    { KATANA, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { SHORT_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 }, /* wakizashi */
    { YUMI, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { YA, 0, WEAPON_CLASS, 25, UNDEF_BLESS, 0, 0 }, /* variable quan */
    { SPLINT_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { ORIENTAL_SILK_SACK, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Tourist[] = {
#define T_DARTS 0
    { DART, 3, WEAPON_CLASS, 25, UNDEF_BLESS, 0, 0 }, /* quan is variable */
    { GOLF_CLUB, 3, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { POT_EXTRA_HEALING, 0, POTION_CLASS, 2, UNDEF_BLESS, 0, 0 },
    { SCR_MAGIC_MAPPING, 0, SCROLL_CLASS, 4, UNDEF_BLESS, 0, 0 },
    { HAWAIIAN_SHIRT, 2, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { LEATHER_SANDALS, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { EXPENSIVE_CAMERA, UNDEF_SPE, TOOL_CLASS, 1, 0, 0, 0 },
    { WAN_TELEPORTATION, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { RIN_TELEPORT_CONTROL, UNDEF_SPE, RING_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { CREDIT_CARD, 0, TOOL_CLASS, 1, 0, 0, 0 },
    { SUNGLASSES, 0, MISCELLANEOUS_CLASS, 1, 0, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 10, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj TouristMale[] = {
    { LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { EXPENSIVE_WATCH, 0, MISCELLANEOUS_CLASS, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj TouristFemale[] = {
    { EXPENSIVE_HANDBAG, 0, TOOL_CLASS, 1, 0, 0, 0 },
    { GOLDEN_EARRINGS, 0, MISCELLANEOUS_CLASS, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Valkyrie[] = {
    { LONG_SWORD, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { DAGGER, 0, WEAPON_CLASS, 1, UNDEF_BLESS, COLD_ENCHANTMENT, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0, 0 },
    { SACK, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj Wizard[] = 
{
    { QUARTERSTAFF, 1, WEAPON_CLASS, 1, 1, 0, 0 },
    { CLOAK_OF_MAGIC_RESISTANCE, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { WIZARD_S_ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { BAG_OF_WIZARDRY, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 2, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, POTION_CLASS, 3, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 3, UNDEF_BLESS, 0, 0 },
    { SPE_MAGIC_ARROW, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_SHIELD, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};
static struct trobj WizardAlternate[] = 
{
    { STAFF_OF_FIRE, 1, WEAPON_CLASS, 1, 1, 0, 0 },
    { LEATHER_CLOAK, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { WIZARD_S_ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { BAG_OF_WIZARDRY, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 2, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, POTION_CLASS, 3, UNDEF_BLESS, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 3, UNDEF_BLESS, 0, 0 },
    { SPE_MAGIC_ARROW, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { SPE_SHIELD, 0, SPBOOK_CLASS, 1, 1, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};

/*
 *      Optional extra inventory items.
 */

static struct trobj Tinopener[] = { { TIN_OPENER, 0, TOOL_CLASS, 1, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Magicmarker[] = { { MAGIC_MARKER, UNDEF_SPE, TOOL_CLASS,
                                        1, 0, 0, 0 },
                                      { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Lamp[] = { { OIL_LAMP, 1, TOOL_CLASS, 1, 0, 0, 0 },
                               { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Blindfold[] = { { BLINDFOLD, 0, TOOL_CLASS, 1, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Instrument[] = { { WOODEN_FLUTE, 0, TOOL_CLASS, 1, 0, 0, 0 },
                                     { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Xtra_food[] = { { UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 2, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Leash[] = { { LEASH, 0, TOOL_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Wishing[] = { { WAN_WISHING, 3, WAND_CLASS, 1, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj OreDetection[] = { { WAN_ORE_DETECTION, UNDEF_SPE, WAND_CLASS, 1, 0, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj Money[] = { { GOLD_PIECE, 0, COIN_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };

static struct trobj DeathQuarrel[] = { { BONE_QUARREL, 0, WEAPON_CLASS,
                                        1, 0, DEATH_ENCHANTMENT, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
#if 0
static struct trobj LightningQuarrel[] = { { GNOLLISH_QUARREL, 0, WEAPON_CLASS,
                                        2, 0, LIGHTNING_ENCHANTMENT, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
#endif
static struct trobj LightningArrow[] = { { ARROW, 0, WEAPON_CLASS,
                                        2, 0, LIGHTNING_ENCHANTMENT, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj LightningElvenArrow[] = { { ELVEN_ARROW, 0, WEAPON_CLASS,
                                        2, 0, LIGHTNING_ENCHANTMENT, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj LightningOrcishArrow[] = { { ORCISH_ARROW, 0, WEAPON_CLASS,
                                        2, 0, LIGHTNING_ENCHANTMENT, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };

static struct trobj PriestSilverMace[] = { { SILVER_MACE, 2, WEAPON_CLASS, 1, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestNormalMace[] = { { MACE, 3, WEAPON_CLASS, 1, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestLawfulSummonSpell[] = { { SPE_CELESTIAL_DOVE, 0, SPBOOK_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestNeutralSummonSpell[] = { { SPE_STICK_TO_SNAKE, 0, SPBOOK_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestChaoticSummonSpell[] = { { SPE_RAISE_MINOR_ZOMBIE, 0, SPBOOK_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightSmallShield[] = { { SMALL_SHIELD, 0, ARMOR_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightSpikedSilverShield[] = { { SPIKED_SILVER_SHIELD, 0, ARMOR_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestSmallShield[] = { { SMALL_SHIELD, 1, ARMOR_CLASS, 1, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestSpikedSilverShield[] = { { SPIKED_SILVER_SHIELD, 0, ARMOR_CLASS, 1, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj ValkyrieSpikedShield[] = { { SPIKED_SHIELD, 3, ARMOR_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj ValkyrieSpikedSilverShield[] = { { SPIKED_SILVER_SHIELD, 3, ARMOR_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightLeatherGloves[] = { { LEATHER_GLOVES, 0, ARMOR_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightSilverGauntlets[] = { { SILVER_GAUNTLETS, 0, ARMOR_CLASS, 1, 0, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestSilverGauntlets[] = { { SILVER_GAUNTLETS, 0, ARMOR_CLASS, 1, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj ScrollOfIdentify[] = { { SCR_IDENTIFY, 0, SCROLL_CLASS, 1, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj TwoScrollsOfIdentify[] = { { SCR_IDENTIFY, 0, SCROLL_CLASS, 2, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj ThreeScrollsOfIdentify[] = { { SCR_IDENTIFY, 0, SCROLL_CLASS, 3, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj FourScrollsOfIdentify[] = { { SCR_IDENTIFY, 0, SCROLL_CLASS, 4, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj WandOfProbing[] = { { WAN_PROBING, 0, WAND_CLASS, 1, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };
static struct trobj ScrollOfRemoveCurse[] = { { SCR_REMOVE_CURSE, 0, SCROLL_CLASS, 1, 0, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0, 0 } };

/* race-based substitutions for initial inventory;
   the weaker cloak for elven rangers is intentional--they shoot better */
static struct inv_sub {
    short race_pm, item_otyp, subs_otyp;
} inv_subs[] = {
    { PM_ELF, DAGGER, ELVEN_DAGGER },
    { PM_ELF, SPEAR, ELVEN_SPEAR },
    { PM_ELF, SHORT_SWORD, ELVEN_SHORT_SWORD },
    { PM_ELF, SHORT_BOW, ELVEN_LONG_BOW },
    { PM_ELF, LONG_BOW, ELVEN_LONG_BOW },
    { PM_ELF, ARROW, ELVEN_ARROW },
    { PM_ELF, HELMET, ELVEN_LEATHER_HELM },
    /* { PM_ELF, SMALL_SHIELD, ELVEN_SHIELD }, */
    { PM_ELF, CLOAK_OF_DISPLACEMENT, ELVEN_CLOAK },
    { PM_ELF, CRAM_RATION, ELVEN_WAYBREAD },
    { PM_ORC, DAGGER, ORCISH_DAGGER },
    { PM_ORC, SPEAR, ORCISH_SPEAR },
    { PM_ORC, SHORT_SWORD, ORCISH_SHORT_SWORD },
    { PM_ORC, SHORT_BOW, ORCISH_SHORT_BOW },
    { PM_ORC, LONG_BOW, ORCISH_SHORT_BOW },
    { PM_ORC, ARROW, ORCISH_ARROW },
    { PM_ORC, HELMET, ORCISH_HELM },
    { PM_ORC, SMALL_SHIELD, ORCISH_SHIELD },
    { PM_ORC, RING_MAIL, ORCISH_RING_MAIL },
    { PM_ORC, CHAIN_MAIL, ORCISH_CHAIN_MAIL },
    { PM_ORC, CRAM_RATION, TRIPE_RATION },
    { PM_ORC, ELVEN_WAYBREAD, TRIPE_RATION },
    { PM_DWARF, SPEAR, DWARVISH_SPEAR },
    { PM_DWARF, SHORT_SWORD, DWARVISH_SHORT_SWORD },
    { PM_DWARF, HELMET, DWARVISH_IRON_HELM },
    /* { PM_DWARF, SMALL_SHIELD, DWARVISH_ROUNDSHIELD }, */
    /* { PM_DWARF, PICK_AXE, DWARVISH_MATTOCK }, */
    { PM_DWARF, AXE, DWARVISH_AXE },
    { PM_DWARF, LANCE, DWARVISH_SPEAR },
    { PM_DWARF, LONG_SWORD, DWARVISH_AXE },
    { PM_DWARF, HELMET, DWARVISH_IRON_HELM },
    { PM_DWARF, SMALL_SHIELD, DWARVISH_ROUNDSHIELD },
    { PM_DWARF, APPLE, PENNY_BUN },
    { PM_DWARF, CARROT, CHANTERELLE },
    { PM_DWARF, ELVEN_WAYBREAD, CRAM_RATION },
    { PM_GNOLL, LONG_SWORD, FLAIL },
    { PM_GNOLL, SHORT_SWORD, FLAIL },
    { PM_GNOLL, SCALPEL, BONE_DAGGER },
    { PM_GNOLL, DAGGER, BONE_DAGGER },
    { PM_GNOLL, MACE, FLAIL },
    { PM_GNOLL, SILVER_MACE, SILVER_FLAIL },
    { PM_GNOLL, CLUB, FLAIL },
    { PM_GNOLL, SHORT_BOW, CROSSBOW },
    { PM_GNOLL, LONG_BOW, CROSSBOW },
    { PM_GNOLL, ARROW, GNOLLISH_QUARREL },
    { PM_GNOLL, BONE_ARROW, BONE_QUARREL },
    { PM_GNOLL, HELMET, GNOLLISH_HOOD },
    { PM_GNOLL, COTTON_HOOD, GNOLLISH_HOOD },
    { PM_GNOLL, LEATHER_HAT, GNOLLISH_HOOD },
    { PM_GNOLL, BANDED_MAIL, GNOLLISH_BONE_MAIL },
    { PM_GNOLL, RING_MAIL, GNOLLISH_LEATHER_ARMOR },
    { PM_GNOLL, CHAIN_MAIL, GNOLLISH_STUDDED_LEATHER_ARMOR },
    { PM_GNOLL, LEATHER_ARMOR, GNOLLISH_LEATHER_ARMOR },
    { PM_GNOLL, STUDDED_LEATHER_ARMOR, GNOLLISH_STUDDED_LEATHER_ARMOR },
    { PM_GNOLL, MEDIEVAL_ROBE, GNOLLISH_HAIRCLOTH_ROBE },
    { PM_GNOLL, CLERICAL_GOWN, GNOLLISH_HAIRCLOTH_ROBE },
    { PM_GNOLL, SIMPLE_GOWN, GNOLLISH_HAIRCLOTH_ROBE },
    { PM_GNOLL, WIZARD_S_ROBE, GNOLLISH_HAIRCLOTH_ROBE },
    { PM_GNOLL, BEAK_MASK_OF_SICKNESS_RESISTANCE, GNOLLISH_LEATHER_MASK },
    { PM_GNOLL, CRAM_RATION, TRIPE_RATION },
    { PM_GNOLL, ELVEN_WAYBREAD, TRIPE_RATION },
    { PM_GNOLL, FOOD_RATION, TRIPE_RATION },
    { PM_GNOLL, APPLE, TIN },
    { PM_GNOLL, CARROT, TIN },
    { PM_GNOLL, ORANGE, TIN },
    { PM_GNOLL, BANANA, TIN },
    { PM_GNOLL, PEAR, TIN },
    { NON_PM, STRANGE_OBJECT, STRANGE_OBJECT }
};

static const struct def_skill Skill_A_Max[] = {
    { P_DAGGER, P_MASTER },
    { P_DIGGING, P_GRAND_MASTER },
    { P_SWORD, P_MASTER },
    { P_BLUDGEONING_WEAPON, P_MASTER },
    { P_QUARTERSTAFF, P_MASTER },
    { P_SLING, P_MASTER },
    { P_THROWN_WEAPON, P_MASTER },
    { P_WHIP, P_GRAND_MASTER },
    { P_CROSSBOW, P_MASTER },
    { P_ARCANE_SPELL, P_EXPERT },
    { P_HEALING_SPELL, P_EXPERT },
    { P_ABJURATION_SPELL, P_MASTER },
    { P_DIVINATION_SPELL, P_MASTER },
    { P_TRANSMUTATION_SPELL, P_MASTER },
    { P_NECROMANCY_SPELL, P_MASTER },
    { P_RIDING, P_MASTER },
    { P_TWO_WEAPON_COMBAT, P_MASTER },
    { P_BARE_HANDED_COMBAT, P_MASTER },
    { P_DODGE, P_EXPERT },
    { P_WAND, P_MASTER },
    { P_DISARM_TRAP, P_MASTER },
    { P_NONE, 0 }
};

static const struct def_skill Skill_A_Init[] = {
    { P_WHIP, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_B_Max[] = {
    { P_DAGGER, P_MASTER },
    { P_AXE, P_GRAND_MASTER },
    { P_DIGGING, P_EXPERT },
    { P_SWORD, P_GRAND_MASTER },
    { P_BLUDGEONING_WEAPON, P_MASTER },
    { P_FLAIL, P_MASTER },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_THRUSTING_WEAPON, P_EXPERT },
    { P_BOW, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
    { P_BARE_HANDED_COMBAT, P_MASTER },
    { P_DODGE, P_MASTER },
    { P_SHIELD, P_EXPERT },
    { P_DISARM_TRAP, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_B_Init[] = {
    { P_AXE, P_BASIC },
    { P_SWORD, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_C_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_EXPERT },
    { P_DIGGING, P_MASTER },
    { P_BLUDGEONING_WEAPON, P_GRAND_MASTER },
    { P_FLAIL, P_GRAND_MASTER },
    { P_QUARTERSTAFF, P_GRAND_MASTER },
    { P_POLEARM, P_BASIC },
    { P_THRUSTING_WEAPON, P_GRAND_MASTER },
    { P_BOW, P_SKILLED },
    { P_SLING, P_GRAND_MASTER },
    { P_THROWN_WEAPON, P_MASTER },
    { P_NATURE_SPELL, P_MASTER },
    { P_BARE_HANDED_COMBAT, P_MASTER },
    { P_DODGE, P_MASTER },
    { P_SHIELD, P_EXPERT },
    { P_WAND, P_EXPERT },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_C_Init[] = {
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_QUARTERSTAFF, P_BASIC },
    { P_THRUSTING_WEAPON, P_BASIC },
    { P_SLING, P_BASIC },
    { P_THROWN_WEAPON, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_NATURE_SPELL, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_H_Max[] = {
    { P_DAGGER, P_GRAND_MASTER },
    { P_SWORD, P_SKILLED },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_QUARTERSTAFF, P_GRAND_MASTER },
    { P_SLING, P_SKILLED },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_HEALING_SPELL, P_GRAND_MASTER },
    { P_ABJURATION_SPELL, P_GRAND_MASTER },
    { P_DIVINATION_SPELL, P_GRAND_MASTER },
    { P_ENCHANTMENT_SPELL, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_DODGE, P_SKILLED },
    { P_WAND, P_MASTER },
    { P_DISARM_TRAP, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_H_Init[] = {
    { P_DAGGER, P_SKILLED },
    { P_QUARTERSTAFF, P_BASIC },
    { P_HEALING_SPELL, P_SKILLED },
    { P_ABJURATION_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_BASIC },
    { P_WAND, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_K_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_MASTER },
    { P_SWORD, P_GRAND_MASTER },
    { P_BLUDGEONING_WEAPON, P_MASTER },
    { P_FLAIL, P_GRAND_MASTER },
    { P_POLEARM, P_GRAND_MASTER },
    { P_THRUSTING_WEAPON, P_GRAND_MASTER },
    { P_BOW, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_SKILLED },
    { P_HEALING_SPELL, P_SKILLED },
    { P_CLERIC_SPELL, P_SKILLED },
    { P_CELESTIAL_SPELL, P_SKILLED },
    { P_ABJURATION_SPELL, P_SKILLED },
    { P_RIDING, P_EXPERT },
    { P_TWO_WEAPON_COMBAT, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_DODGE, P_MASTER },
    { P_SHIELD, P_GRAND_MASTER },
    { P_WAND, P_BASIC },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_K_Init[] = {
    { P_SWORD, P_BASIC },
    { P_THRUSTING_WEAPON, P_BASIC },
    { P_SHIELD, P_BASIC },
    { P_RIDING, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_Monk_Max[] = {
    { P_QUARTERSTAFF, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_ABJURATION_SPELL, P_GRAND_MASTER },
    { P_HEALING_SPELL, P_GRAND_MASTER },
    { P_DIVINATION_SPELL, P_GRAND_MASTER },
    { P_CLERIC_SPELL, P_GRAND_MASTER },
    { P_MOVEMENT_SPELL, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_GRAND_MASTER },
    { P_MARTIAL_ARTS, P_GRAND_MASTER },
    { P_DODGE, P_GRAND_MASTER },
    { P_WAND, P_SKILLED },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Monk_Init[] = {
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_MARTIAL_ARTS, P_UNSKILLED },
    { P_ABJURATION_SPELL, P_BASIC },
    { P_MOVEMENT_SPELL, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_P_Max_Chaotic[] = {
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_EXPERT },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_THROWN_WEAPON, P_SKILLED },
    { P_HEALING_SPELL, P_GRAND_MASTER },
    { P_DIVINATION_SPELL, P_GRAND_MASTER },
    { P_CLERIC_SPELL, P_GRAND_MASTER },
    { P_ABJURATION_SPELL, P_GRAND_MASTER },
    { P_NECROMANCY_SPELL, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_DODGE, P_SKILLED },
    { P_SHIELD, P_MASTER },
    { P_WAND, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_P_Max_Neutral[] = {
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_EXPERT },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_THROWN_WEAPON, P_SKILLED },
    { P_HEALING_SPELL, P_GRAND_MASTER },
    { P_DIVINATION_SPELL, P_GRAND_MASTER },
    { P_CLERIC_SPELL, P_GRAND_MASTER },
    { P_ABJURATION_SPELL, P_GRAND_MASTER },
    { P_NATURE_SPELL, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_DODGE, P_SKILLED },
    { P_SHIELD, P_MASTER },
    { P_WAND, P_SKILLED },
    { P_NONE, 0 }
};
static const struct def_skill Skill_P_Max_Lawful[] = {
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_EXPERT },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_THROWN_WEAPON, P_SKILLED },
    { P_HEALING_SPELL, P_GRAND_MASTER },
    { P_DIVINATION_SPELL, P_GRAND_MASTER },
    { P_CLERIC_SPELL, P_GRAND_MASTER },
    { P_ABJURATION_SPELL, P_GRAND_MASTER },
    { P_CELESTIAL_SPELL, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_DODGE, P_SKILLED },
    { P_SHIELD, P_MASTER },
    { P_WAND, P_SKILLED },
    { P_NONE, 0 }
};

static const struct def_skill Skill_P_Init_Chaotic[] = {
    { P_BLUDGEONING_WEAPON, P_BASIC },
    { P_CLERIC_SPELL, P_BASIC },
    { P_NECROMANCY_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_P_Init_Neutral[] = {
    { P_BLUDGEONING_WEAPON, P_BASIC },
    { P_CLERIC_SPELL, P_BASIC },
    { P_NATURE_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_NONE, 0 }
};
static const struct def_skill Skill_P_Init_Lawful[] = {
    { P_BLUDGEONING_WEAPON, P_BASIC },
    { P_CLERIC_SPELL, P_BASIC },
    { P_CELESTIAL_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_Rogue_Max[] = {
    { P_DAGGER, P_GRAND_MASTER },
    { P_SWORD, P_MASTER },
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_EXPERT },
    { P_BOW, P_MASTER },
    { P_SLING, P_MASTER },
    { P_CROSSBOW, P_MASTER },
    { P_THROWN_WEAPON, P_MASTER },
    { P_DIVINATION_SPELL, P_SKILLED },
    { P_MOVEMENT_SPELL, P_SKILLED },
    { P_TRANSMUTATION_SPELL, P_SKILLED },
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_MASTER },
    { P_BARE_HANDED_COMBAT, P_MASTER },
    { P_DODGE, P_GRAND_MASTER },
    { P_SHIELD, P_SKILLED },
    { P_WAND, P_EXPERT },
    { P_DISARM_TRAP, P_GRAND_MASTER },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Rogue_Init[] = {
    { P_DAGGER, P_BASIC },
    { P_SWORD, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_THROWN_WEAPON, P_BASIC },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_Ranger_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_EXPERT },
    { P_DIGGING, P_BASIC },
    { P_SWORD, P_EXPERT },
    { P_QUARTERSTAFF, P_SKILLED },
    { P_POLEARM, P_SKILLED },
    { P_THRUSTING_WEAPON, P_EXPERT },
    { P_BOW, P_GRAND_MASTER },
    { P_SLING, P_EXPERT },
    { P_CROSSBOW, P_GRAND_MASTER },
    { P_THROWN_WEAPON, P_GRAND_MASTER },
    { P_WHIP, P_BASIC },
    { P_HEALING_SPELL, P_SKILLED },
    { P_DIVINATION_SPELL, P_SKILLED },
    { P_NATURE_SPELL, P_SKILLED },
    { P_RIDING, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
    { P_DODGE, P_SKILLED },
    { P_WAND, P_SKILLED },
    { P_DISARM_TRAP, P_MASTER },
    { P_NONE, 0 }
};
static const struct def_skill Skill_Ranger_Init[] = {
    { P_DAGGER, P_BASIC },
    { P_THRUSTING_WEAPON, P_BASIC },
    { P_BOW, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_S_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_SWORD, P_GRAND_MASTER },
    { P_FLAIL, P_GRAND_MASTER },
    { P_QUARTERSTAFF, P_GRAND_MASTER },
    { P_POLEARM, P_GRAND_MASTER },
    { P_THRUSTING_WEAPON, P_GRAND_MASTER },
    { P_BOW, P_GRAND_MASTER },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_GRAND_MASTER },
    { P_MARTIAL_ARTS, P_SKILLED },
    { P_DODGE, P_MASTER },
    { P_SHIELD, P_EXPERT },
    { P_WAND, P_BASIC },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_S_Init[] = {
    { P_SWORD, P_BASIC },
    { P_THROWN_WEAPON, P_BASIC },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARM, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_T_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_EXPERT },
    { P_DIGGING, P_EXPERT },
    { P_SWORD, P_EXPERT },
    { P_BLUDGEONING_WEAPON, P_MASTER },
    { P_FLAIL, P_EXPERT },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARM, P_EXPERT },
    { P_THRUSTING_WEAPON, P_EXPERT },
    { P_BOW, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_GRAND_MASTER },
    { P_WHIP, P_EXPERT },
    { P_ABJURATION_SPELL, P_EXPERT },
    { P_ARCANE_SPELL, P_EXPERT },
    { P_CLERIC_SPELL, P_EXPERT },
    { P_CONJURATION_SPELL, P_EXPERT },
    { P_DIVINATION_SPELL, P_MASTER },
    { P_ENCHANTMENT_SPELL, P_EXPERT },
    { P_HEALING_SPELL, P_EXPERT },
    { P_MOVEMENT_SPELL, P_GRAND_MASTER },
    { P_TRANSMUTATION_SPELL, P_EXPERT },
    { P_RIDING, P_EXPERT },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
    { P_DODGE, P_EXPERT },
    { P_SHIELD, P_EXPERT },
    { P_WAND, P_EXPERT },
    { P_DISARM_TRAP, P_EXPERT },
    { P_NONE, 0 }
};

static const struct def_skill Skill_T_Init[] = {
    { P_THROWN_WEAPON, P_BASIC },
    { P_NONE, 0 }
};


static const struct def_skill Skill_V_Max[] = {
    { P_DAGGER, P_GRAND_MASTER },
    { P_AXE, P_GRAND_MASTER },
    { P_SWORD, P_GRAND_MASTER },
    { P_BLUDGEONING_WEAPON, P_GRAND_MASTER },
    { P_POLEARM, P_MASTER },
    { P_THRUSTING_WEAPON, P_MASTER },
    { P_BOW, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_GRAND_MASTER },
    { P_BARE_HANDED_COMBAT, P_GRAND_MASTER },
    { P_DODGE, P_MASTER },
    { P_SHIELD, P_GRAND_MASTER },
    { P_WAND, P_BASIC },
    { P_DISARM_TRAP, P_SKILLED },
    { P_NONE, 0 }
};

static const struct def_skill Skill_V_Init[] = {
    { P_DAGGER, P_BASIC },
    { P_AXE, P_BASIC },
    { P_SWORD, P_BASIC },
    { P_BLUDGEONING_WEAPON, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_BASIC },
    { P_SHIELD, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_W_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_SWORD, P_BASIC },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_THROWN_WEAPON, P_SKILLED },
    { P_ABJURATION_SPELL, P_EXPERT },
    { P_ARCANE_SPELL, P_GRAND_MASTER },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_ENCHANTMENT_SPELL, P_GRAND_MASTER },
    { P_MOVEMENT_SPELL, P_GRAND_MASTER },
    { P_TRANSMUTATION_SPELL, P_GRAND_MASTER },
    { P_CONJURATION_SPELL, P_GRAND_MASTER },
    { P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
    { P_DODGE, P_EXPERT },
    { P_WAND, P_GRAND_MASTER },
    { P_DISARM_TRAP, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_W_Init[] = {
    { P_ARCANE_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_BASIC },
    { P_TRANSMUTATION_SPELL, P_BASIC },
    { P_WAND, P_BASIC },
    { P_NONE, 0 }
};


STATIC_OVL void
knows_object(obj)
register int obj;
{
    discover_object(obj, TRUE, FALSE);
    objects[obj].oc_pre_discovered = 1; /* not a "discovery" */
}

#if 0
/* Know ordinary (non-magical) objects of a certain class,
 * like all gems except the loadstone and luckstone.
 */
STATIC_OVL void
knows_class(sym)
register char sym;
{
    register int ct;
    for (ct = 1; ct < NUM_OBJECTS; ct++)
        if (objects[ct].oc_class == sym && !objects[ct].oc_magic)
            knows_object(ct);
}
#endif

void
u_init()
{
    register int i;
    struct u_roleplay tmpuroleplay = u.uroleplay; /* set by rcfile options */

    flags.female = flags.initgend;
    flags.beginner = 1;

    /* zero u, including pointer values --
     * necessary when aborting from a failed restore */
    (void) memset((genericptr_t) &u, 0, sizeof(u));
    u.ustuck = (struct monst *) 0;
    (void) memset((genericptr_t) &ubirthday, 0, sizeof(ubirthday));
    (void) memset((genericptr_t) &urealtime, 0, sizeof(urealtime));

    u.uroleplay = tmpuroleplay; /* restore options set via rcfile */

#if 0  /* documentation of more zero values as desirable */
    u.usick_cause[0] = 0;
    u.uluck  = u.moreluck = 0;
    uarmu = 0;
    uarm = uarmc = uarmh = uarms = uarmg = uarmf = 0;
    uwep = uball = uchain = uleft = uright = 0;
    uswapwep = uquiver = 0;
    u.twoweap = 0;
    u.ublessed = 0;                     /* not worthy yet */
    u.ugangr   = 0;                     /* gods not angry */
    u.ugifts   = 0;                     /* no divine gifts bestowed */
    u.uevent.uhand_of_elbereth = 0;
    u.uevent.uheard_tune = 0;
    u.uevent.uopened_dbridge = 0;
    u.uevent.udemigod = 0;              /* not a demi-god yet... */
    u.udg_cnt = 0;
    u.mh = u.mhmax = u.basehpmax = u.mtimedone = 0;
    u.uz.dnum = u.uz0.dnum = 0;
    u.utotype = 0;
#endif /* 0 */
    props_init();

    u.uz.dlevel = 1;
    u.uz0.dlevel = 0;
    u.utolev = u.uz;

    u.umoved = FALSE;
    u.umortality = 0;
    u.ugrave_arise = NON_PM;
    u.facing_right = 0;
    u.action = ACTION_TILE_NO_ACTION;

    u.umonnum = u.umonster = urole.monsternum;
    u.ulycn = NON_PM;
    set_uasmon();
    flags.female = flags.female;

    u.carrying_capacity_level = UNENCUMBERED;

    u.ulevel = 0; /* set up some of the initial attributes */

    u.ubasehpmax = newhp();
    u.ubaseenmax = newpw();

    u.uhp_fraction = 0;
    u.mh_fraction = 0;
    u.uen_fraction = 0;
    
    adjabil(0, 1);
    u.ulevel = u.ulevelmax = 1;

    init_attr(75); /* init attribute values */
    max_rank_sz(); /* set max str size for class ranks */

    //With level and attributes known, calculate hp and maxhp, en, and maxen
    updatemaxhp();
    updatemaxen();
    u.uhp = u.uhpmax;
    u.uen = u.uenmax;

    init_uhunger();
    for (i = 0; i <= MAXSPELL; i++)
        spl_book[i].sp_id = NO_SPELL;
    u.uprayer_timeout = Role_if(PM_PRIEST) ? 150 : 300; /* no prayers just yet */
    u.ualignbase[A_CURRENT] = u.ualignbase[A_ORIGINAL] = u.ualign.type =
        aligns[flags.initalign].value;

#if defined(BSD) && !defined(POSIX_TYPES)
    (void) time((long *) &ubirthday);
#else
    (void) time(&ubirthday);
#endif

    /*
     *  For now, everyone starts out with a night vision range of 1 and
     *  their xray range disabled.
     */
    u.nv_range = 1;
    u.xray_range = -1;

    /*** Role-specific initializations ***/
    switch (Role_switch) {
    /* rn2(100) > 50 necessary for some choices because some
     * random number generators are bad enough to seriously
     * skew the results if we use rn2(2)...  --KAA
     */
    case PM_ARCHAEOLOGIST:
        ini_inv(Archaeologist);
        if (!rn2(10))
            ini_inv(Tinopener);
        else if (!rn2(4))
            ini_inv(Lamp);
        else if (!rn2(10))
            ini_inv(Magicmarker);
        knows_object(SACK);
        knows_object(BACKPACK);
        knows_object(TOUCHSTONE);
        break;
    case PM_BARBARIAN:
        if (rn2(100) >= 50) { /* see above comment */
            Barbarian[B_MAJOR].trotyp = BATTLE_AXE;
            Barbarian[B_MINOR].trotyp = SHORT_SWORD;
        }
        ini_inv(Barbarian);
        if (!rn2(6))
            ini_inv(Lamp);
        //knows_class(WEAPON_CLASS);
        //knows_class(ARMOR_CLASS);
        break;
    case PM_CAVEMAN:
        Cave_man[C_AMMO].trquan = rn1(11, 10); /* 10..20 */
        ini_inv(Cave_man);
        skill_init(Skill_C_Init, Skill_C_Max);
        break;
    case PM_HEALER:
        u.umoney0 = rn1(1000, 1001);
        ini_inv(Healer);
        if (!rn2(25))
            ini_inv(Lamp);
        knows_object(POT_HEALING);
        knows_object(POT_EXTRA_HEALING);
        knows_object(POT_GREATER_HEALING);
        knows_object(POT_FULL_HEALING);
        knows_object(POT_SLEEPING);
        knows_object(POT_SICKNESS);
        knows_object(POT_POISON);
        knows_object(JAR_OF_MEDICINAL_SALVE);
        knows_object(JAR_OF_EXTRA_HEALING_SALVE);
        knows_object(JAR_OF_GREATER_HEALING_SALVE);
        knows_object(JAR_OF_PRODIGIOUS_HEALING_SALVE);
        knows_object(JAR_OF_BASILISK_BLOOD);
        break;
    case PM_KNIGHT:
        ini_inv(Knight);
        if (!rn2(10))
            ini_inv(KnightSpikedSilverShield);
        else
            ini_inv(KnightSmallShield);

        if (!rn2(12))
            ini_inv(KnightSilverGauntlets);
        else
            ini_inv(KnightLeatherGloves);

        //knows_class(WEAPON_CLASS);
        //knows_class(ARMOR_CLASS);
        break;
    case PM_MONK: {
        /* Alternatively, may have create fruits instead of create food */
        if (!rn2(2))
            Monk[M_FOOD_SPELLBOOK].trotyp = SPE_CREATE_FRUITS;

        ini_inv(Monk);
        if (!rn2(5))
            ini_inv(Magicmarker);
        else if (!rn2(10))
            ini_inv(Lamp);
        //knows_class(ARMOR_CLASS);
        /* sufficiently martial-arts oriented item to ignore language issue */
        knows_object(SHURIKEN);
        break;
    }
    case PM_PRIEST:
        if (u.ualign.type == A_CHAOTIC)
            ini_inv(PriestChaoticSummonSpell);
        else if (u.ualign.type == A_LAWFUL)
            ini_inv(PriestLawfulSummonSpell);
        else
            ini_inv(PriestNeutralSummonSpell);

        if (!rn2(8))
            ini_inv(PriestSilverMace);
        else
            ini_inv(PriestNormalMace);

        ini_inv(Priest);

        if (!rn2(10))
            ini_inv(PriestSpikedSilverShield);
        else
            ini_inv(PriestSmallShield);

        if (!rn2(12))
            ini_inv(PriestSilverGauntlets);

        if (!rn2(10))
            ini_inv(Magicmarker);
        else if (!rn2(10))
            ini_inv(Lamp);

        knows_object(POT_WATER);
        /* KMH, conduct --
         * Some may claim that this isn't agnostic, since they
         * are literally "priests" and they have holy water.
         * But we don't count it as such.  Purists can always
         * avoid playing priests and/or confirm another player's
         * role in their YAAP.
         */
        break;
    case PM_RANGER:
        Ranger[RAN_ONE_ARROWS].trquan = rn1(11, 15);
        Ranger[RAN_ZERO_ARROWS].trquan = rn1(25, 26);
        ini_inv(Ranger);

        if(Race_if(PM_GNOLL))
            ini_inv(DeathQuarrel);
        else if (Race_if(PM_ELF))
            ini_inv(LightningElvenArrow);
        else if (Race_if(PM_ORC))
            ini_inv(LightningOrcishArrow);
        else
            ini_inv(LightningArrow);

        break;
    case PM_ROGUE:
        Rogue[R_DAGGERS].trquan = rn1(5, 6);
        u.umoney0 = 0;
        ini_inv(Rogue);
        if (!rn2(5))
            ini_inv(Blindfold);
        knows_object(SACK);
        knows_object(BAG_OF_TREASURE_HAULING);
        knows_object(POT_POISON);
        break;
    case PM_SAMURAI:
        Samurai[S_ARROWS].trquan = rn1(20, 26);
        ini_inv(Samurai);
        if (!rn2(5))
            ini_inv(Blindfold);
        //knows_class(WEAPON_CLASS);
        //knows_class(ARMOR_CLASS);
        break;
    case PM_TOURIST:
        u.umoney0 = 300 + rnd(700);
        Tourist[T_DARTS].trquan = rn1(20, 21);
        ini_inv(Tourist);

        if (flags.female)
        {
            ini_inv(TouristFemale);
            knows_object(EXPENSIVE_HANDBAG);
            knows_object(GOLDEN_EARRINGS);
        }
        else
        {
            ini_inv(TouristMale);
            knows_object(LEATHER_BAG);
            knows_object(EXPENSIVE_WATCH);
        }

        if (!rn2(25))
            ini_inv(Tinopener);
        else if (!rn2(25))
            ini_inv(Leash);
        else if (!rn2(25))
            ini_inv(Magicmarker);
        break;
    case PM_VALKYRIE:
        ini_inv(Valkyrie);
        if (!rn2(8))
            ini_inv(ValkyrieSpikedSilverShield);
        else
            ini_inv(ValkyrieSpikedShield);


        if (!rn2(6))
            ini_inv(Lamp);
        //knows_class(WEAPON_CLASS);
        //knows_class(ARMOR_CLASS);
        break;
    case PM_WIZARD:
        ini_inv(!rn2(2) ? Wizard : WizardAlternate);
        if (!rn2(5))
            ini_inv(Magicmarker);
        if (!rn2(5))
            ini_inv(Blindfold);
        break;

    default: /* impossible */
        break;
    }


    /* Initilize skills based on roles */
    u_skills_init();

    /*** Race-specific initializations ***/
    switch (Race_switch) {
    case PM_HUMAN:
        /* Nothing special */
        break;

    case PM_ELF:
        /*
         * Elves are people of music and song, or they are warriors.
         * Non-warriors get an instrument.  We use a kludge to
         * get only non-magic instruments.
         */
        if (Role_if(PM_PRIEST) || Role_if(PM_WIZARD)) {
            static int trotyp[] = { WOODEN_FLUTE, BRASS_HORN,  WOODEN_HARP,
                                    BELL,         BUGLE,       LEATHER_DRUM };
            Instrument[0].trotyp = trotyp[rn2(SIZE(trotyp))];
            ini_inv(Instrument);
        }

        /* Elves can recognize all elvish objects */
        knows_object(ELVEN_SHORT_SWORD);
        knows_object(ELVEN_ARROW);
        knows_object(ELVEN_LONG_BOW);
        knows_object(ELVEN_SPEAR);
        knows_object(ELVEN_DAGGER);
        knows_object(ELVEN_BROADSWORD);
        knows_object(ELVEN_MITHRIL_COAT);
        knows_object(ELVEN_LEATHER_HELM);
        knows_object(ELVEN_SHIELD);
        knows_object(ELVEN_BOOTS);
        knows_object(ELVEN_CLOAK);
        break;

    case PM_DWARF:
        /* Dwarves can recognize all dwarvish objects */
        if (!rn2(6))
            ini_inv(OreDetection);
        knows_object(DWARVISH_SPEAR);
        knows_object(DWARVISH_SHORT_SWORD);
        knows_object(DWARVISH_MATTOCK);
        knows_object(DWARVISH_IRON_HELM);
        knows_object(DWARVISH_MITHRIL_COAT);
        knows_object(DWARVISH_CLOAK);
        knows_object(DWARVISH_ROUNDSHIELD);
        switch (Role_switch)
        {
        case PM_ARCHAEOLOGIST:
            P_MAX_SKILL_LEVEL(P_DIGGING) = P_GRAND_MASTER;
            if (P_SKILL_LEVEL(P_DIGGING) < P_SKILLED)
                P_SKILL_LEVEL(P_DIGGING) = P_SKILLED;
            break;
        case PM_CAVEMAN:
            P_MAX_SKILL_LEVEL(P_DIGGING) = P_GRAND_MASTER;
            if (P_SKILL_LEVEL(P_DIGGING) < P_BASIC)
                P_SKILL_LEVEL(P_DIGGING) = P_BASIC;
            break;
        case PM_KNIGHT:
        case PM_VALKYRIE:
            P_MAX_SKILL_LEVEL(P_DIGGING) = P_GRAND_MASTER;
            break;
        default:
            if (P_MAX_SKILL_LEVEL(P_DIGGING) < P_EXPERT)
                P_MAX_SKILL_LEVEL(P_DIGGING) = P_EXPERT;
            break;
        }
        if (P_SKILL_LEVEL(P_DIGGING) == P_ISRESTRICTED)
            P_SKILL_LEVEL(P_DIGGING) = P_UNSKILLED;        
        break;
    case PM_GNOLL:
        knows_object(GNOLLISH_HAIRCLOTH_ROBE);
        knows_object(GNOLLISH_HOOD);
        knows_object(GNOLLISH_LEATHER_ARMOR);
        knows_object(GNOLLISH_STUDDED_LEATHER_ARMOR);
        knows_object(GNOLLISH_QUARREL);
        switch (Role_switch)
        {
        case PM_BARBARIAN:
        case PM_CAVEMAN:
            P_MAX_SKILL_LEVEL(P_FLAIL) = P_GRAND_MASTER;
            break;
        case PM_ROGUE:
            if (P_MAX_SKILL_LEVEL(P_FLAIL) < P_MASTER)
                P_MAX_SKILL_LEVEL(P_FLAIL) = P_MASTER;
            break;
        case PM_PRIEST:
        case PM_RANGER:
            if (P_MAX_SKILL_LEVEL(P_FLAIL) < P_EXPERT)
                P_MAX_SKILL_LEVEL(P_FLAIL) = P_EXPERT;
            break;
        case PM_WIZARD:
        case PM_HEALER:
            if (P_MAX_SKILL_LEVEL(P_FLAIL) < P_SKILLED)
                P_MAX_SKILL_LEVEL(P_FLAIL) = P_SKILLED;
            break;
        default:
            if (P_MAX_SKILL_LEVEL(P_FLAIL) < P_BASIC)
                P_MAX_SKILL_LEVEL(P_FLAIL) = P_BASIC;
            break;
        }
        if (P_SKILL_LEVEL(P_FLAIL) == P_ISRESTRICTED)
            P_SKILL_LEVEL(P_FLAIL) = P_UNSKILLED;
        break;
    case PM_ORC:
        /* compensate for generally inferior equipment */
        if (!Role_if(PM_WIZARD))
            ini_inv(Xtra_food);
        /* Orcs can recognize all orcish objects */
        knows_object(ORCISH_SHORT_SWORD);
        knows_object(ORCISH_ARROW);
        knows_object(ORCISH_SHORT_BOW);
        knows_object(ORCISH_SPEAR);
        knows_object(ORCISH_DAGGER);
        knows_object(ORCISH_CHAIN_MAIL);
        knows_object(ORCISH_RING_MAIL);
        knows_object(ORCISH_HELM);
        knows_object(ORCISH_SHIELD);
        knows_object(GREAT_ORCISH_SHIELD);
        knows_object(ORCISH_CLOAK);
        break;

    default: /* impossible */
        break;
    }

    /* Add school-specific spells */
    add_school_specific_spellbooks();

    /* 
     * Difficulty level specific initializations 
     */
    /* Everybody starts with one or more scrolls of identify */
    if (context.game_difficulty <= -4)
        ini_inv(FourScrollsOfIdentify);
    else if (context.game_difficulty == -3)
        ini_inv(ThreeScrollsOfIdentify);
    else if (context.game_difficulty == -2)
        ini_inv(TwoScrollsOfIdentify);
    else
        ini_inv(ScrollOfIdentify);

    /* Scroll of identify self-identifies itself, in the case previous did not do it automatically */
    knows_object(SCR_IDENTIFY);


    /* Wand of probing if playing below expert */
    if (context.game_difficulty <= WAND_OF_PROBING_DIFFICULTY_THRESHOLD && !carrying(WAN_PROBING))
    {
        ini_inv(WandOfProbing);
        knows_object(WAN_PROBING);
    }

    /* Easy mode scroll of remove curse */
    if (context.game_difficulty <= SCROLL_OF_REMOVE_CURSE_DIFFICULTY_THRESHOLD)
    {
        ini_inv(ScrollOfRemoveCurse);
        knows_object(SCR_REMOVE_CURSE);
    }
    
    /* 
     * Final stuff
     */
    if (discover)
        ini_inv(Wishing);

    if (wizard)
        read_wizkit();

    if (u.umoney0)
        ini_inv(Money);

    u.umoney0 += hidden_gold(); /* in case sack has gold in it */

    /* clear out last picked item */
    context.last_picked_obj_oid = 0;
    context.last_picked_obj_show_duration_left = 0;

    /* make sure you can carry all you have - especially for Tourists */
    while (inv_weight() > 0) 
    {
        if (adjattrib(A_STR, 1, TRUE))
            continue;
        if (adjattrib(A_CON, 1, TRUE))
            continue;
        /* only get here when didn't boost strength or constitution */
        break;
    }


    /* Sort spells */
    sortspells();

    /* Check if any skill can be advanced */
    update_can_advance_any_skill();

    /* Run initial stats */
    update_all_character_properties((struct obj*)0, FALSE);
    update_inventory();

    /* Make sure that hp and mana are at maximum */
    u.uhp = u.uhpmax;
    u.uen = u.uenmax;

    return;
}


void
u_skills_init()
{
    /*** Role-specific skill initializations ***/
    switch (Role_switch) 
    {
    case PM_ARCHAEOLOGIST:
        skill_init(Skill_A_Init, Skill_A_Max);
        break;
    case PM_BARBARIAN:
        skill_init(Skill_B_Init, Skill_B_Max);
        break;
    case PM_CAVEMAN:
        skill_init(Skill_C_Init, Skill_C_Max);
        break;
    case PM_HEALER:
        skill_init(Skill_H_Init, Skill_H_Max);
        break;
    case PM_KNIGHT:
        skill_init(Skill_K_Init, Skill_K_Max);
        break;
    case PM_MONK:
        skill_init(Skill_Monk_Init, Skill_Monk_Max);
        break;
    case PM_PRIEST:
        if (u.ualign.type == A_CHAOTIC)
            skill_init(Skill_P_Init_Chaotic, Skill_P_Max_Chaotic);
        else if (u.ualign.type == A_LAWFUL)
            skill_init(Skill_P_Init_Lawful, Skill_P_Max_Lawful);
        else
            skill_init(Skill_P_Init_Neutral, Skill_P_Max_Neutral);
        break;
    case PM_RANGER:
        skill_init(Skill_Ranger_Init, Skill_Ranger_Max);
        break;
    case PM_ROGUE:
        skill_init(Skill_Rogue_Init, Skill_Rogue_Max);
        break;
    case PM_SAMURAI:
        skill_init(Skill_S_Init, Skill_S_Max);
        break;
    case PM_TOURIST:
        skill_init(Skill_T_Init, Skill_T_Max);
        break;
    case PM_VALKYRIE:
        skill_init(Skill_V_Init, Skill_V_Max);
        break;
    case PM_WIZARD:
        skill_init(Skill_W_Init, Skill_W_Max);
        break;
    default: /* impossible */
        break;
    }
}

STATIC_OVL
void
add_school_specific_spellbooks(VOID_ARGS)
{
    int skill;
    for (skill = P_FIRST_SPELL; skill <= P_LAST_SPELL; skill++)
    {
        if (P_SKILL_LEVEL(skill) >= P_BASIC)
        {
            int cnt = max(0, P_SKILL_LEVEL(skill) - 1);
            int i;
            for (i = 0; i < cnt; i++)
            {
                struct obj* obj = mkobj_with_flags(SPBOOK_CLASS, FALSE, FALSE, (struct monst*)0, MKOBJ_FLAGS_NORMAL_SPELLBOOK);
                int otyp = obj->otyp;

                while (otyp == SPE_BLANK_PAPER
                    || already_learnt_spell_type(otyp)
                    || (otyp == SPE_PROTECTION_FROM_LYCANTHROPY && Race_if(PM_GNOLL))
                    || (otyp == SPE_PROTECTION_FROM_POISON && Race_if(PM_ORC))
                    || objects[otyp].oc_skill != skill
                    || (objects[otyp].oc_spell_level > 2 + P_SKILL_LEVEL(skill)
                        || restricted_spell_discipline(otyp)
                        || (Role_if(PM_WIZARD) && !(objects[otyp].oc_spell_attribute == A_INT
                            || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS
                            || objects[otyp].oc_spell_attribute == A_MAX_INT_CHA
                            || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS_CHA
                            || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS
                            || objects[otyp].oc_spell_attribute == A_AVG_INT_CHA
                            || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS_CHA
                            ))
                        || (Role_if(PM_PRIEST) && !(objects[otyp].oc_spell_attribute == A_WIS
                            || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS
                            || objects[otyp].oc_spell_attribute == A_MAX_WIS_CHA
                            || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS_CHA
                            || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS
                            || objects[otyp].oc_spell_attribute == A_AVG_WIS_CHA
                            || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS_CHA
                            ))
                        ))
                {
                    dealloc_obj(obj);
                    obj = mkobj_with_flags(SPBOOK_CLASS, FALSE, FALSE, (struct monst*)0, MKOBJ_FLAGS_NORMAL_SPELLBOOK);
                    otyp = obj->otyp;
                }

                obj = addinv(obj);
                if (OBJ_DESCR(objects[otyp]) && obj->known)
                    discover_object(otyp, TRUE, FALSE);

                initialspell(obj);
                useup(obj);
            }
        }
    }
}

/* skills aren't initialized, so we use the role-specific skill lists */
STATIC_OVL boolean
restricted_spell_discipline(otyp)
int otyp;
{
    const struct def_skill *skills;
    int this_skill = spell_skilltype(otyp);

    switch (Role_switch) {
    case PM_ARCHAEOLOGIST:
        skills = Skill_A_Max;
        break;
    case PM_BARBARIAN:
        skills = Skill_B_Max;
        break;
    case PM_CAVEMAN:
        skills = Skill_C_Max;
        break;
    case PM_HEALER:
        skills = Skill_H_Max;
        break;
    case PM_KNIGHT:
        skills = Skill_K_Max;
        break;
    case PM_MONK:
        skills = Skill_Monk_Max;
        break;
    case PM_PRIEST:
        skills = u.ualign.type == A_CHAOTIC ? Skill_P_Max_Chaotic : u.ualign.type == A_LAWFUL ? Skill_P_Max_Lawful : Skill_P_Max_Neutral;
        break;
    case PM_RANGER:
        skills = Skill_Ranger_Max;
        break;
    case PM_ROGUE:
        skills = Skill_Rogue_Max;
        break;
    case PM_SAMURAI:
        skills = Skill_S_Max;
        break;
    case PM_TOURIST:
        skills = Skill_T_Max;
        break;
    case PM_VALKYRIE:
        skills = Skill_V_Max;
        break;
    case PM_WIZARD:
        skills = Skill_W_Max;
        break;
    default:
        skills = 0; /* lint suppression */
        break;
    }

    while (skills && skills->skill != P_NONE) {
        if (skills->skill == this_skill)
            return FALSE;
        ++skills;
    }
    return TRUE;
}

STATIC_OVL void
ini_inv(trop)
register struct trobj *trop;
{
    struct obj *obj;
    int otyp;

    while (trop->trclass) 
    {
        otyp = (int) trop->trotyp;
        if (otyp != UNDEF_TYP)
        {
            obj = mksobj(otyp, TRUE, FALSE, FALSE);
        }
        else 
        { /* UNDEF_TYP */
            static NEARDATA short nocreate = STRANGE_OBJECT;
            static NEARDATA short nocreate2 = STRANGE_OBJECT;
            static NEARDATA short nocreate3 = STRANGE_OBJECT;
            static NEARDATA short nocreate4 = STRANGE_OBJECT;
            /*
             * For random objects, do not create certain overly powerful
             * items: wand of wishing, ring of levitation, or the
             * polymorph/polymorph control combination.  Specific objects,
             * i.e. the discovery wishing, are still OK.
             * Also, don't get a couple of really useless items.  (Note:
             * punishment isn't "useless".  Some players who start out with
             * one will immediately read it and use the iron ball as a
             * weapon.)
             */
            obj = mkobj(trop->trclass, FALSE, FALSE);
            otyp = obj->otyp;

            while (otyp == WAN_WISHING || otyp == nocreate
                   || otyp == nocreate2 || otyp == nocreate3
                   || otyp == nocreate4 || otyp == RIN_LEVITATION
                   || otyp == AMULET_OF_LIFE_SAVING
                   /* 'useless' items */
                   || is_cursed_magic_item(obj)
                   || objects[otyp].oc_flags2 & O2_GENERATED_CURSED
                   || otyp == POT_HALLUCINATION
                   || otyp == POT_ACID
                   || otyp == POT_URINE
                   || (otyp == POT_DWARVEN_MUSHROOM_BREW && !Race_if(PM_DWARF))
                   || (otyp == SPE_PROTECTION_FROM_LYCANTHROPY && Race_if(PM_GNOLL))
                   || (otyp == SPE_PROTECTION_FROM_POISON && Race_if(PM_ORC))
                   || (otyp == SPE_PROTECTION_FROM_POISON && Role_if(PM_BARBARIAN))
                   || (otyp == AMULET_VERSUS_POISON && Race_if(PM_ORC))
                   || (otyp == AMULET_VERSUS_POISON && Role_if(PM_BARBARIAN))
                   || otyp == SCR_AMNESIA
                   || otyp == SCR_RETRAINING /* No need in the beginning */
                   || otyp == SCR_FIRE
                   || otyp == SCR_BLANK_PAPER
                   || otyp == SPE_BLANK_PAPER
                   || otyp == WAN_NOTHING
                   || already_learnt_spell_type(otyp)
                   /* orcs start with poison resistance */
                   || (otyp == RIN_POISON_RESISTANCE && Race_if(PM_ORC))
                   /* Monks don't use weapons */
                   || (otyp == SCR_ENCHANT_WEAPON && Role_if(PM_MONK))
                   /* powerful spells are either useless to
                      low level players or unbalancing; also
                      spells in restricted skill categories */
                   || (obj->oclass == SPBOOK_CLASS
                       && (objects[otyp].oc_spell_level > 3
                           || carrying(otyp)
                           || restricted_spell_discipline(otyp) 
                           || (Role_if(PM_WIZARD) && !(objects[otyp].oc_spell_attribute == A_INT
                               || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS
                               || objects[otyp].oc_spell_attribute == A_MAX_INT_CHA
                               || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS_CHA
                               || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS
                               || objects[otyp].oc_spell_attribute == A_AVG_INT_CHA
                               || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS_CHA
                               ))
                           || (Role_if(PM_PRIEST) && !(objects[otyp].oc_spell_attribute == A_WIS
                               || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS
                               || objects[otyp].oc_spell_attribute == A_MAX_WIS_CHA
                               || objects[otyp].oc_spell_attribute == A_MAX_INT_WIS_CHA
                               || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS
                               || objects[otyp].oc_spell_attribute == A_AVG_WIS_CHA
                               || objects[otyp].oc_spell_attribute == A_AVG_INT_WIS_CHA
                               ))
                           || (Role_if(PM_MONK) && !(objects[otyp].oc_skill == P_ENCHANTMENT_SPELL
                               || objects[otyp].oc_skill == P_HEALING_SPELL
                               || objects[otyp].oc_skill == P_ABJURATION_SPELL
                               || objects[otyp].oc_skill == P_MOVEMENT_SPELL
                               ))
                           ))) 
            {
                dealloc_obj(obj);
                obj = mkobj(trop->trclass, FALSE, FALSE);
                otyp = obj->otyp;
            }

            /* Heavily relies on the fact that 1) we create wands
             * before rings, 2) that we create rings before
             * spellbooks, and that 3) not more than 1 object of a
             * particular symbol is to be prohibited.  (For more
             * objects, we need more nocreate variables...)
             */
            switch (otyp) 
            {
            case WAN_POLYMORPH:
            case RIN_POLYMORPH:
            case POT_POLYMORPH:
                nocreate = RIN_POLYMORPH_CONTROL;
                break;
            case RIN_POLYMORPH_CONTROL:
                nocreate = RIN_POLYMORPH;
                nocreate2 = SPE_POLYMORPH;
                nocreate3 = POT_POLYMORPH;
            }
            /* Don't have 2 of the same ring or spellbook */
            if (obj->oclass == RING_CLASS || obj->oclass == SPBOOK_CLASS)
                nocreate4 = otyp;
        }

        /* Don't start with +0 or negative rings */
        if (objects[otyp].oc_enchantable && obj->enchantment <= 0)
            obj->enchantment = rne(3);

        if (urace.monsternum != PM_HUMAN) 
        {
            /* substitute race-specific items; this used to be in
               the 'if (otyp != UNDEF_TYP) { }' block above, but then
               substitutions didn't occur for randomly generated items
               (particularly food) which have racial substitutes */
            int i;
            for (i = 0; inv_subs[i].race_pm != NON_PM; ++i)
                if (inv_subs[i].race_pm == urace.monsternum
                    && otyp == inv_subs[i].item_otyp)
                {
                    debugpline3("ini_inv: substituting %s for %s%s",
                        OBJ_NAME(objects[inv_subs[i].subs_otyp]),
                        (trop->trotyp == UNDEF_TYP) ? "random " : "",
                        OBJ_NAME(objects[otyp]));
                    otyp = obj->otyp = inv_subs[i].subs_otyp;
                }
        }

        /* set tin type */
        if (obj->otyp == TIN)
        {
            switch (rn2(5))
            {
            case 0:
                obj->corpsenm = PM_SEWER_RAT;
                break;
            case 1:
                obj->corpsenm = PM_NEWT;
                break;
            case 2:
                obj->corpsenm = PM_LIZARD;
                break;
            case 3:
                obj->corpsenm = PM_GECKO;
                break;
            case 4:
                obj->corpsenm = PM_GIANT_RAT;
                break;
            default:
                obj->corpsenm = PM_SEWER_RAT;
                break;
            }
        }

        /* nudist gets no armor */
        if (u.uroleplay.nudist && obj->oclass == ARMOR_CLASS) {
            dealloc_obj(obj);
            trop++;
            continue;
        }

        /* rogues get poisoned crossbow bolts */
        if (Role_if(PM_ROGUE))
        {
            if (obj->otyp == CROSSBOW_BOLT)
                obj->opoisoned = 1;
        }

        /* Set sack contents*/
        if (Role_if(PM_TOURIST))
        {
            if (otyp == LEATHER_BAG || otyp == EXPENSIVE_HANDBAG)
            {
                (void)mk_obj_in_container_known(obj, BATHROBE);
                (void)mk_obj_in_container_known(obj, COTTON_SLIPPERS);
                (void)mk_obj_in_container_known(obj, TOWEL);
                knows_object(BATHROBE);
                obj->owt = weight(obj);
            }
        }
        else if (Role_if(PM_MONK))
        {
            if (otyp == ORIENTAL_SILK_SACK)
            {
                (void)mk_obj_in_container_known(obj, GINSENG_ROOT);
                (void)mk_obj_in_container_known(obj, GINSENG_ROOT);
                (void)mk_obj_in_container_known(obj, APPLE);
                (void)mk_obj_in_container_known(obj, APPLE);
                (void)mk_obj_in_container_known(obj, APPLE);
                (void)mk_obj_in_container_known(obj, APPLE);
                (void)mk_obj_in_container_known(obj, APPLE);
                (void)mk_obj_in_container_known(obj, ORANGE);
                (void)mk_obj_in_container_known(obj, ORANGE);
                (void)mk_obj_in_container_known(obj, ORANGE);
                (void)mk_obj_in_container_known(obj, ORANGE);
                (void)mk_obj_in_container_known(obj, ORANGE);
                (void)mk_obj_in_container_known(obj, HANDFUL_OF_NUTS);
                if(!rn2(2))
                    (void)mk_obj_in_container_known(obj, MELON);
                if (!rn2(2))
                    (void)mk_obj_in_container_known(obj, FOOD_RATION);
                obj->owt = weight(obj);
            }
        }
        else if (Role_if(PM_ARCHAEOLOGIST))
        {
            if (otyp == BACKPACK)
            {
                (void)mk_obj_in_container_known(obj, BANANA);
                (void)mk_obj_in_container_known(obj, POT_WATER);
                (void)mk_obj_in_container_known(obj, SCR_BLANK_PAPER);
                (void)mk_obj_in_container_known(obj, SPE_BLANK_PAPER);
                (void)mk_obj_in_container_known(obj, HANDFUL_OF_NUTS);
                if (!rn2(2))
                    (void)mk_obj_in_container_known(obj, FOOD_RATION);
                if (!rn2(2))
                    (void)mk_obj_in_container_known(obj, FOOD_RATION);
                knows_object(POT_WATER);
                knows_object(SCR_BLANK_PAPER);
                knows_object(SPE_BLANK_PAPER);
                obj->owt = weight(obj);
            }
        }
        else if (Role_if(PM_KNIGHT))
        {
            if (otyp == LEATHER_BAG)
            {
                if (Race_if(PM_DWARF))
                {
                    (void)mk_obj_in_container_known(obj, CRAM_RATION);
                    if (!rn2(2))
                        (void)mk_obj_in_container_known(obj, CRAM_RATION);
                    (void)mk_obj_in_container_known(obj, POT_FRUIT_JUICE);
                    (void)mk_obj_in_container_known(obj, CHANTERELLE);
                    (void)mk_obj_in_container_known(obj, CHANTERELLE);
                    if (!rn2(2))
                        (void)mk_obj_in_container_known(obj, CHANTERELLE);
                    (void)mk_obj_in_container_known(obj, PENNY_BUN);
                    (void)mk_obj_in_container_known(obj, PENNY_BUN);
                    if (!rn2(2))
                        (void)mk_obj_in_container_known(obj, PENNY_BUN);
                    knows_object(CHANTERELLE);
                    knows_object(PENNY_BUN);
                    knows_object(POT_FRUIT_JUICE);
                    obj->owt = weight(obj);
                }
                else
                {
                    (void)mk_obj_in_container_known(obj, SILVER_FORK);
                    (void)mk_obj_in_container_known(obj, SILVER_KNIFE);
                    (void)mk_obj_in_container_known(obj, FOOD_RATION);
                    if (!rn2(2))
                        (void)mk_obj_in_container_known(obj, FOOD_RATION);
                    (void)mk_obj_in_container_known(obj, APPLE);
                    (void)mk_obj_in_container_known(obj, APPLE);
                    (void)mk_obj_in_container_known(obj, APPLE);
                    (void)mk_obj_in_container_known(obj, APPLE);
                    if (!rn2(2))
                        (void)mk_obj_in_container_known(obj, APPLE);
                    (void)mk_obj_in_container_known(obj, CARROT);
                    (void)mk_obj_in_container_known(obj, CARROT);
                    if (!rn2(2))
                        (void)mk_obj_in_container_known(obj, CARROT);
                    (void)mk_obj_in_container_known(obj, POT_WATER);
                    knows_object(POT_WATER);
                    obj->owt = weight(obj);
                }
            }
        }
        else if (Role_if(PM_WIZARD))
        {
            if (otyp == BAG_OF_WIZARDRY)
            {
                /* Add one sulfur */
                struct obj* otmp = (struct obj*)0;
                int i;

                otmp = mksobj(PINCH_OF_SULFUROUS_ASH, FALSE, FALSE, TRUE);
                otmp->known = 1;
                otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
                otmp = add_to_container(obj, otmp);


                /* Add some food */
                for (i = 1; i <= 2; i++)
                {
                    otmp = mksobj(!rn2(3) ? CHANTERELLE : !rn2(2) ? CHAMPIGNON : PENNY_BUN, TRUE, FALSE, TRUE);
                    knows_object(otmp->otyp);
                    otmp->known = 1;
                    otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
                    otmp = add_to_container(obj, otmp);
                }

                otmp = mksobj(SLIME_MOLD, TRUE, FALSE, TRUE);
                knows_object(otmp->otyp);
                otmp->known = 1;
                otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
                otmp = add_to_container(obj, otmp);


                /* Add arrows for magic arrow */
                otmp = mksobj(ARROW, FALSE, FALSE, TRUE);
                otmp->quan = 10;
                otmp->owt = weight(otmp);
                otmp->known = 1;
                otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
                otmp = add_to_container(obj, otmp);

                int n = 2 + rn2(4); //2...5
                for (i = 0; i < n; i++)
                {
                    struct obj* otmp2 = mksobj(randomreagent(FALSE, 2), FALSE, FALSE, FALSE);
                    if (otmp)
                    {
                        knows_object(otmp2->otyp);
                        otmp2->known = 1;
                        otmp2->dknown = otmp2->bknown = otmp2->rknown = otmp2->nknown = 1;
                        (void)add_to_container(obj, otmp2);
                    }
                }

                /* Last update bag weight */
                obj->owt = weight(obj);

            }
        }
        else if (Role_if(PM_HEALER) || Role_if(PM_PRIEST))
        {
            if (otyp == LEATHER_BAG)
            {
                /* Add ginseng roots */
                (void)mk_obj_in_container_known(obj, GINSENG_ROOT);
                (void)mk_obj_in_container_known(obj, GINSENG_ROOT);
                (void)mk_obj_in_container_known(obj, GINSENG_ROOT);

                int n = rnd(3); //1...3
                int i;
                for (i = 0; i < n; i++)
                {
                    struct obj* otmp = mkobj(REAGENT_CLASS, FALSE, TRUE);
                    otmp->bknown = 1;
                    if (otmp)
                    {
                        knows_object(otmp->otyp);
                        otmp = add_to_container(obj, otmp);
                    }
                }

                /* Last update bag weight */
                obj->owt = weight(obj);

            }
        }

        if (trop->trclass == COIN_CLASS) 
        {
            /* no "blessed" or "identified" money */
            obj->quan = u.umoney0;
        }
        else 
        {
            if (objects[otyp].oc_uses_known)
                obj->known = 1;
            obj->dknown = obj->bknown = obj->rknown = obj->nknown = 1;
            
            if (Is_container(obj) || obj->otyp == STATUE) 
            {
                obj->cknown = obj->lknown = obj->tknown = 1;
                obj->otrapped = 0;
            }

            obj->cursed = 0;
            
            if (obj->opoisoned && u.ualign.type != A_CHAOTIC)
                obj->opoisoned = 0;
           
            if (obj->oclass == WEAPON_CLASS || obj->oclass == TOOL_CLASS) 
            {
                obj->quan = (long) trop->trquan;
                trop->trquan = 1;
            } 
            else if (obj->oclass == GEM_CLASS && is_graystone(obj))
            {
                obj->quan = 1L;
            }

            if (trop->trspe != UNDEF_SPE)
                obj->enchantment = trop->trspe;
            if (trop->trbless != UNDEF_BLESS)
                obj->blessed = trop->trbless;
            if (trop->elemental_enchantment > 0)
                obj->elemental_enchantment = trop->elemental_enchantment;
            if (obj->elemental_enchantment == DEATH_ENCHANTMENT && u.ualign.type != A_CHAOTIC)
            {
                obj->elemental_enchantment = LIGHTNING_ENCHANTMENT;
                if(is_ammo(obj))
                    obj->quan += rnd(2);
            }

            obj->exceptionality = trop->exceptionality;
        }
        /* defined after setting otyp+quan + blessedness */
        obj->owt = weight(obj);
        obj = addinv(obj);

        /* Make the type known if necessary */
        if (OBJ_DESCR(objects[otyp]) && obj->known)
            discover_object(otyp, TRUE, FALSE);
        if (otyp == OIL_LAMP)
            discover_object(POT_OIL, TRUE, FALSE);

        if (obj->oclass == ARMOR_CLASS) 
        {
            if (is_shield(obj) && !uarms && !(uwep && bimanual(uwep)))
            {
                setwornquietly(obj, W_ARMS);
                /* 3.6.2: this used to unset uswapwep if it was set, but
                   wearing a shield doesn't prevent having an alternate
                   weapon ready to swap with the primary; just make sure we
                   aren't two-weaponing (academic; no one starts that way) */
                u.twoweap = FALSE;
            } 
            else if (is_helmet(obj) && !uarmh)
                setwornquietly(obj, W_ARMH);
            else if (is_gloves(obj) && !uarmg)
                setwornquietly(obj, W_ARMG);
            else if (is_shirt(obj) && !uarmu)
                setwornquietly(obj, W_ARMU);
            else if (is_cloak(obj) && !uarmc)
                setwornquietly(obj, W_ARMC);
            else if (is_robe(obj) && !uarmo)
                setwornquietly(obj, W_ARMO);
            else if (is_bracers(obj) && !uarmb)
                setwornquietly(obj, W_ARMB);
            else if (is_boots(obj) && !uarmf)
                setwornquietly(obj, W_ARMF);
            else if (is_suit(obj) && !uarm)
                setwornquietly(obj, W_ARM);
        }

        if (obj->oclass == AMULET_CLASS && !uamul)
        {
            setwornquietly(obj, W_AMUL);
        }

        if (obj->oclass == MISCELLANEOUS_CLASS && !(umisc && umisc2 && umisc3 && umisc4 && umisc5))
        {
            if (objects[obj->otyp].oc_subtyp != MISC_MULTIPLE_PERMITTED &&
                ((umisc && objects[umisc->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
                || (umisc2 && objects[umisc2->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
                || (umisc3 && objects[umisc3->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
                || (umisc4 && objects[umisc4->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
                || (umisc5 && objects[umisc5->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
                )
               )
            {
                //Nothing
            }
            else
            {
                if(!umisc)
                    setwornquietly(obj, W_MISC);
                else if (!umisc2)
                    setwornquietly(obj, W_MISC2);
                else if (!umisc3)
                    setwornquietly(obj, W_MISC3);
                else if (!umisc4)
                    setwornquietly(obj, W_MISC4);
                else if (!umisc5)
                    setwornquietly(obj, W_MISC5);
            }
        }


        if (obj->oclass == WEAPON_CLASS || is_weptool(obj) || otyp == TIN_OPENER || otyp == FLINT || is_rock(obj)) 
        {
            if (is_ammo(obj) || is_missile(obj)) 
            {
                if (!uquiver)
                    setuqwepquietly(obj);
            }
            else if (!uwep && (!uarms || !bimanual(obj)) && !is_launcher(obj))
            {
                setuwepquietly(obj, W_WEP);
            }
            else if (!uswapwep && is_launcher(obj))
            {
                setuswapwepquietly(obj, W_SWAPWEP);
            }
        }
        if (obj->oclass == SPBOOK_CLASS && obj->otyp != SPE_BLANK_PAPER)
        {
            initialspell(obj);
            useup(obj);
        }
#if !defined(PYRAMID_BUG) && !defined(MAC)
        if (--trop->trquan)
            continue; /* make a similar object */
#else
        if (trop->trquan) 
        { /* check if zero first */
            --trop->trquan;
            if (trop->trquan)
                continue; /* make a similar object */
        }
#endif
        trop++;
    }
}

STATIC_OVL
struct obj* 
mk_obj_in_container_known(container, itemtype)
struct obj* container;
int itemtype;
{
    struct obj* otmp = add_to_container(container, mksobj(itemtype, FALSE, FALSE, TRUE));
    if (otmp)
    {
        otmp->bknown = 1;
        /* Small kludge here to make sure that tourist starts with the right kind of slippers */
        if (otmp->otyp == COTTON_SLIPPERS)
        {
            if (flags.female)
                otmp->speflags |= SPEFLAGS_ALTERNATIVE_APPEARANCE;
            else
                otmp->speflags &= ~SPEFLAGS_ALTERNATIVE_APPEARANCE;
        }
    }

    return otmp;
}


const char*
get_role_achievement_description(is_past_participle)
boolean is_past_participle;
{
    const char* res = "";
    switch (urole.rolenum)
    {
    case ROLE_ARCHAEOLOGIST:
        if(is_past_participle)
            res = "defeated two greater mummy pharaohs";
        else
            res = "defeat two greater mummy pharaohs";
        break;
    case ROLE_BARBARIAN:
        if (u.ualignbase[A_ORIGINAL] == A_CHAOTIC)
        {
            if (is_past_participle)
                res = "found Stormbringer and Mournblade";
            else
                res = "find Stormbringer and Mournblade";
        }
        else
        {
            if (is_past_participle)
                res = "found the Vorpal Blade and Cleaver";
            else
                res = "find the Vorpal Blade and Cleaver";
        }
        break;
    case ROLE_CAVEMAN:
        if (is_past_participle)
            res = "attained the level of Grand Master in bludgeoning weapons";
        else
            res = "attain the level of Grand Master in bludgeoning weapons";
        break;
    case ROLE_HEALER:
        if (is_past_participle)
            res = "attained the level of Grand Master in healing";
        else
            res = "attain the level of Grand Master in healing";
        break;
    case ROLE_KNIGHT:
        if (is_past_participle)
            res = "found the Holy Grail and Excalibur";
        else
            res = "find the Holy Grail and Excalibur";
        break;
    case ROLE_MONK:
        if (is_past_participle)
            res = "attained the level of Grand Master in martial arts";
        else
            res = "attain the level of Grand Master in martial arts";
        break;
    case ROLE_PRIEST:
        if (is_past_participle)
            res = "found the Mace of St. Cuthbert";
        else
            res = "find the Mace of St. Cuthbert";
        break;
    case ROLE_ROGUE:
        if (is_past_participle)
            res = "defeated Croesus";
        else
            res = "defeat Croesus";
        break;
    case ROLE_RANGER:
        if (is_past_participle)
            res = "attained the level of Grand Master in bow or crossbow";
        else
            res = "attain the level of Grand Master in bow or crossbow";
        break;
    case ROLE_SAMURAI:
        if (is_past_participle)
            res = "found the Kusanagi and the Katana of Masamune";
        else
            res = "find the Kusanagi and the Katana of Masamune";
        break;
    case ROLE_TOURIST:
        if (is_past_participle)
            res = "taken a selfie with Demogorgon";
        else
            res = "take a selfie with Demogorgon";
        break;
    case ROLE_VALKYRIE:
        if (is_past_participle)
            res = "attained the level of Grand Master in two-weapon combat";
        else
            res = "achieve the level of Grand Master in two-weapon combat";
        break;
    case ROLE_WIZARD:
        if (is_past_participle)
            res = "successfully cast a spell of level 11 or higher";
        else
            res = "successfully cast a spell of level 11 or higher";
        break;
    default:
        res = "";
        break;
    }

    return res;
}

boolean
is_known_spell_school(knownspellschools, skill_id)
unsigned long knownspellschools;
int skill_id;
{
    if (skill_id < P_FIRST_SPELL || skill_id > P_LAST_SPELL)
        return FALSE;

    unsigned long knownspellbit = 1UL << (skill_id - P_FIRST_SPELL);
    return (knownspellschools & knownspellbit) != 0;
}

unsigned long
mon_known_spell_schools(mon)
struct monst* mon;
{
    if (!mon)
        return FALSE;
    unsigned long mflags = mons[mon->mnum].mflags7;
    unsigned long knownspellschools = 0UL;

    int i;
    for (i = 0; i < NUM_ROLES; i++)
    {
        unsigned long bit = M7_ARCHAEOLOGIST << i; // Should be 1UL
        if (mflags & bit)
        {
            const struct def_skill* maxskills = 0;
            switch (i)
            {
            case ROLE_ARCHAEOLOGIST:
                maxskills = Skill_A_Max;
                break;
            case ROLE_BARBARIAN:
                maxskills = Skill_B_Max;
                break;
            case ROLE_CAVEMAN:
                maxskills = Skill_C_Init;
                break;
            case ROLE_HEALER:
                maxskills = Skill_H_Max;
                break;
            case ROLE_KNIGHT:
                maxskills = Skill_K_Max;
                break;
            case ROLE_MONK:
                maxskills = Skill_Monk_Max;
                break;
            case ROLE_PRIEST:
                if (mon->malign < 0)
                    maxskills = Skill_P_Max_Chaotic;
                else if (mon->malign > 0)
                    maxskills = Skill_P_Max_Lawful;
                else
                    maxskills = Skill_P_Max_Neutral;
                break;
            case ROLE_ROGUE:
                maxskills = Skill_Rogue_Max;
                break;
            case ROLE_RANGER:
                maxskills = Skill_Ranger_Max;
                break;
            case ROLE_SAMURAI:
                maxskills = Skill_S_Max;
                break;
            case ROLE_VALKYRIE:
                maxskills = Skill_V_Max;
                break;
            case ROLE_WIZARD:
                maxskills = Skill_W_Max;
                break;
            default:
                break;
            }

            if (maxskills)
            {
                while (maxskills->skill > P_NONE)
                {
                    if (maxskills->skill >= P_FIRST_SPELL && maxskills->skill <= P_LAST_SPELL)
                    {
                        unsigned long spbit = 1UL << (maxskills->skill - P_FIRST_SPELL);
                        knownspellschools |= spbit;
                    }
                    maxskills++;
                }
            }
        }
    }

    return knownspellschools;
}

/*u_init.c*/

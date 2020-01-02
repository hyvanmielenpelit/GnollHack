/* GnollHack 4.0	u_init.c	$NHDT-Date: 1539510426 2018/10/14 09:47:06 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.43 $ */
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
	int elemental_enchantment;
};

STATIC_DCL void FDECL(ini_inv, (struct trobj *));
STATIC_DCL void FDECL(knows_object, (int));
STATIC_DCL void FDECL(knows_class, (CHAR_P));
STATIC_DCL boolean FDECL(restricted_spell_discipline, (int));

#define UNDEF_TYP 0
#define UNDEF_SPE '\177'
#define UNDEF_BLESS 2

/*
 *      Initial inventory for the various roles.
 */

static struct trobj Archeologist[] = {
    /* if adventure has a name...  idea from tan@uvm-gen */
    { BULLWHIP, 2, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { LEATHER_JACKET, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ FEDORA, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0 },
	{ HANDFUL_OF_NUTS, 0, FOOD_CLASS, 1, 0, 0 },
	{ PICK_AXE, 2, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
    { TINNING_KIT, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ TOUCHSTONE, 0, GEM_CLASS, 1, 0, 0 },
    { BACKPACK, 0, TOOL_CLASS, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0 }
};
static struct trobj Barbarian[] = {
#define B_MAJOR 0 /* two-handed sword or battle-axe  */
#define B_MINOR 1 /* matched with axe or short sword */
    { TWO_HANDED_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { AXE, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { RING_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ SACK, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0 }
};
static struct trobj Cave_man[] = {
#define C_AMMO 2
    { CLUB, 3, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { SLING, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { FLINT, 0, GEM_CLASS, 15, UNDEF_BLESS, 0 },
    { STONE_PEBBLE, 0, GEM_CLASS, 3, 0, 0 },             /* yields 18..33 */
	{ UNDEF_TYP, UNDEF_SPE, AMULET_CLASS, 1, UNDEF_BLESS, 0 },
	{ SACK, UNDEF_SPE, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ FOOD_RATION, 0, FOOD_CLASS, 3, 0 },
	{ LEATHER_ARMOR, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
    { 0, 0, 0, 0, 0, 0 }
};
static struct trobj Healer[] = {
    { SCALPEL, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { LEATHER_GLOVES, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ ROBE, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ STETHOSCOPE, 0, TOOL_CLASS, 1, 0 },
    { POT_HEALING, 0, POTION_CLASS, 4, UNDEF_BLESS, 0 },
    { POT_EXTRA_HEALING, 0, POTION_CLASS, 4, UNDEF_BLESS, 0 },
    { WAN_SLEEP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0 },
    /* always blessed, so it's guaranteed readable */
    { SPE_HEALING, 0, SPBOOK_CLASS, 1, 1, 0 },
    { SPE_EXTRA_HEALING, 0, SPBOOK_CLASS, 1, 1, 0 },
    { SPE_STONE_TO_FLESH, 0, SPBOOK_CLASS, 1, 1, 0 },
	{ GINSENG_ROOT, 0, SPBOOK_CLASS, 3, 0, 0 },
	{ GINSENG_ROOT, 0, SPBOOK_CLASS, 1, 1, 0 },
	{ CLOVE_OF_GARLIC, 0, SPBOOK_CLASS, 2, 0, 0 },
	{ APPLE, 0, FOOD_CLASS, 5, 0, 0 },
	{ LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Knight[] = {
    { LONG_SWORD, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { LANCE, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { RING_MAIL, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
    { HELMET, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
    { APPLE, 0, FOOD_CLASS, 2, 0, 0 },
    { CARROT, 0, FOOD_CLASS, 2, 0, 0 },
	{ LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Monk[] = {
#define M_BOOK 2
    { LEATHER_GLOVES, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
    { ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, 1, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 1, UNDEF_BLESS, 0 },
    { POT_HEALING, 0, POTION_CLASS, 3, UNDEF_BLESS, 0 },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0 },
    { APPLE, 0, FOOD_CLASS, 1, UNDEF_BLESS, 0 },
    { ORANGE, 0, FOOD_CLASS, 1, UNDEF_BLESS, 0 },
	{ HANDFUL_OF_NUTS, 0, FOOD_CLASS, 1, UNDEF_BLESS, 0 },
	/* Yes, we know fortune cookies aren't really from China.  They were
     * invented by George Jung in Los Angeles, California, USA in 1916.
     */
    { FORTUNE_COOKIE, 0, FOOD_CLASS, 3, UNDEF_BLESS, 0 },
	{ ORIENTAL_SILK_SACK, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Priest[] = {
	{ RING_MAIL, 0, ARMOR_CLASS, 1, 1, 0 },
	{ CLERICAL_GOWN, 0, ARMOR_CLASS, 1, 1, 0 },
	{ HOLY_SYMBOL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 }, //Blessed holy symbol is even more powerful
	{ POT_WATER, 0, POTION_CLASS, 4, 1, 0 }, /* holy water */
	{ CLOVE_OF_GARLIC, 0, FOOD_CLASS, 1, 0, 0 },
	{ SPRIG_OF_WOLFSBANE, 0, FOOD_CLASS, 1, 0, 0 },
    { UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 2, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 2, UNDEF_BLESS, 0 },
	{ LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Ranger[] = {
#define RAN_BOW 1
#define RAN_ONE_ARROWS 2
#define RAN_ZERO_ARROWS 3
	{ DAGGER, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
	{ SHORT_BOW, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { ARROW, 1, WEAPON_CLASS, 15, UNDEF_BLESS, 0 },
    { ARROW, 0, WEAPON_CLASS, 25, UNDEF_BLESS, 0 },
	{ LEATHER_BRACERS, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
	{ CLOAK_OF_PROTECTION, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ BACKPACK, 0, TOOL_CLASS, 1, 0, 0 },
	{ CRAM_RATION, 0, FOOD_CLASS, 4, 0, 0 },
    { 0, 0, 0, 0, 0, 0 }
};
static struct trobj Rogue[] = {
#define R_DAGGERS 1
    { SHORT_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { DAGGER, 0, WEAPON_CLASS, 5, 0 }, /* quan is variable */
	{ HAND_CROSSBOW, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
	{ CROSSBOW_BOLT, 1, WEAPON_CLASS, 15, UNDEF_BLESS, 0 },
	{ LEATHER_ARMOR, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ COTTON_HOOD, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ POT_SICKNESS, 0, POTION_CLASS, 1, 0, 0 },
    { LOCK_PICK, 0, TOOL_CLASS, 1, 0, 0 },
    { BAG_OF_TREASURE_HAULING, 0, TOOL_CLASS, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0 }
};
static struct trobj Samurai[] = {
#define S_ARROWS 3
    { KATANA, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { SHORT_SWORD, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 }, /* wakizashi */
    { YUMI, 0, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { YA, 0, WEAPON_CLASS, 25, UNDEF_BLESS, 0 }, /* variable quan */
    { SPLINT_MAIL, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ ORIENTAL_SILK_SACK, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Tourist[] = {
#define T_DARTS 0
	{ DART, 2, WEAPON_CLASS, 25, UNDEF_BLESS, 0 }, /* quan is variable */
	{ GOLF_CLUB, 2, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ POT_EXTRA_HEALING, 0, POTION_CLASS, 2, UNDEF_BLESS, 0 },
    { SCR_MAGIC_MAPPING, 0, SCROLL_CLASS, 4, UNDEF_BLESS, 0 },
    { HAWAIIAN_SHIRT, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ LEATHER_SANDALS, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ EXPENSIVE_CAMERA, UNDEF_SPE, TOOL_CLASS, 1, 0, 0 },
    { CREDIT_CARD, 0, TOOL_CLASS, 1, 0, 0 },
	{ LEATHER_BAG, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ EXPENSIVE_WATCH, 0, MISCELLANEOUS_CLASS, 1, 0, 0 },
	{ SUNGLASSES, 0, MISCELLANEOUS_CLASS, 1, 0, 0 },
	{ UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 10, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj TouristFemale[] = {
	{ DART, 2, WEAPON_CLASS, 25, UNDEF_BLESS, 0 }, /* quan is variable */
	{ GOLF_CLUB, 2, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ POT_EXTRA_HEALING, 0, POTION_CLASS, 2, UNDEF_BLESS, 0 },
	{ SCR_MAGIC_MAPPING, 0, SCROLL_CLASS, 4, UNDEF_BLESS, 0 },
	{ HAWAIIAN_SHIRT, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ LEATHER_SANDALS, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ EXPENSIVE_CAMERA, UNDEF_SPE, TOOL_CLASS, 1, 0, 0 },
	{ CREDIT_CARD, 0, TOOL_CLASS, 1, 0, 0 },
	{ EXPENSIVE_HANDBAG, 0, TOOL_CLASS, 1, 0, 0 },
	{ GOLDEN_EARRINGS, 0, MISCELLANEOUS_CLASS, 1, 0, 0 },
	{ SUNGLASSES, 0, MISCELLANEOUS_CLASS, 1, 0, 0 },
	{ UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 10, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Valkyrie[] = {
    { LONG_SWORD, 1, WEAPON_CLASS, 1, UNDEF_BLESS, 0 },
    { DAGGER, 0, WEAPON_CLASS, 1, UNDEF_BLESS, COLD_ENCHANTMENT },
    { FOOD_RATION, 0, FOOD_CLASS, 1, 0, 0 },
	{ SACK, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj Wizard[] = 
{
    { QUARTERSTAFF, 1, WEAPON_CLASS, 1, 1, 0 },
	{ CLOAK_OF_MAGIC_RESISTANCE, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ BAG_OF_WIZARDRY, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0 },
    { UNDEF_TYP, UNDEF_SPE, RING_CLASS, 2, UNDEF_BLESS, 0 },
    { UNDEF_TYP, UNDEF_SPE, POTION_CLASS, 3, UNDEF_BLESS, 0 },
    { UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 3, UNDEF_BLESS, 0 },
	{ SPE_MAGIC_ARROW, 0, SPBOOK_CLASS, 1, 1, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};
static struct trobj WizardAlternate[] = 
{
	{ STAFF_OF_FROST, 1, WEAPON_CLASS, 1, 1, 0 },
	{ LEATHER_CLOAK, 0, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ ROBE, 1, ARMOR_CLASS, 1, UNDEF_BLESS, 0 },
	{ BAG_OF_WIZARDRY, 0, TOOL_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, WAND_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, RING_CLASS, 2, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, POTION_CLASS, 3, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SCROLL_CLASS, 3, UNDEF_BLESS, 0 },
	{ SPE_MAGIC_ARROW, 0, SPBOOK_CLASS, 1, 1, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0 },
	{ UNDEF_TYP, UNDEF_SPE, SPBOOK_CLASS, 1, UNDEF_BLESS, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

/*
 *      Optional extra inventory items.
 */

static struct trobj ExpensiveHandbag[] = { { EXPENSIVE_HANDBAG, 0, TOOL_CLASS, 1, 0, 0 },
									{ 0, 0, 0, 0, 0, 0 } };
static struct trobj Tinopener[] = { { TIN_OPENER, 0, TOOL_CLASS, 1, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0 } };
static struct trobj Magicmarker[] = { { MAGIC_MARKER, UNDEF_SPE, TOOL_CLASS,
                                        1, 0, 0 },
                                      { 0, 0, 0, 0, 0, 0 } };
static struct trobj Lamp[] = { { OIL_LAMP, 1, TOOL_CLASS, 1, 0, 0 },
                               { 0, 0, 0, 0, 0, 0 } };
static struct trobj Blindfold[] = { { BLINDFOLD, 0, TOOL_CLASS, 1, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0 } };
static struct trobj Instrument[] = { { WOODEN_FLUTE, 0, TOOL_CLASS, 1, 0, 0 },
                                     { 0, 0, 0, 0, 0, 0 } };
static struct trobj Xtra_food[] = { { UNDEF_TYP, UNDEF_SPE, FOOD_CLASS, 2, 0, 0 },
                                    { 0, 0, 0, 0, 0, 0 } };
static struct trobj Leash[] = { { LEASH, 0, TOOL_CLASS, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0 } };
static struct trobj Towel[] = { { TOWEL, 0, TOOL_CLASS, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0 } };
static struct trobj Wishing[] = { { WAN_WISHING, 3, WAND_CLASS, 1, 0, 0 },
                                  { 0, 0, 0, 0, 0, 0 } };
static struct trobj OreDetection[] = { { WAN_ORE_DETECTION, UNDEF_SPE, WAND_CLASS, 1, 0, 0 },
								  { 0, 0, 0, 0, 0, 0 } };
static struct trobj Money[] = { { GOLD_PIECE, 0, COIN_CLASS, 1, 0, 0 },
                                { 0, 0, 0, 0, 0, 0 } };

static struct trobj DeathQuarrel[] = { { BONE_QUARREL, 0, WEAPON_CLASS,
										1, 0, DEATH_ENCHANTMENT },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj LightningQuarrel[] = { { GNOLLISH_QUARREL, 0, WEAPON_CLASS,
										2, 0, LIGHTNING_ENCHANTMENT },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj LightningArrow[] = { { ARROW, 0, WEAPON_CLASS,
										2, 0, LIGHTNING_ENCHANTMENT },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj LightningElvenArrow[] = { { ELVEN_ARROW, 0, WEAPON_CLASS,
										2, 0, LIGHTNING_ENCHANTMENT },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj LightningOrcishArrow[] = { { ORCISH_ARROW, 0, WEAPON_CLASS,
										2, 0, LIGHTNING_ENCHANTMENT },
								{ 0, 0, 0, 0, 0, 0 } };

static struct trobj PriestSilverMace[] = { { SILVER_MACE, 2, WEAPON_CLASS, 1, 1, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestNormalMace[] = { { MACE, 2, WEAPON_CLASS, 1, 1, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightSmallShield[] = { { SMALL_SHIELD, 0, ARMOR_CLASS, 1, 0, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightSpikedSilverShield[] = { { SPIKED_SILVER_SHIELD, 0, ARMOR_CLASS, 1, 0, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestSmallShield[] = { { SMALL_SHIELD, 0, ARMOR_CLASS, 1, 1, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestSpikedSilverShield[] = { { SPIKED_SILVER_SHIELD, 0, ARMOR_CLASS, 1, 1, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj ValkyrieSpikedShield[] = { { SPIKED_SHIELD, 3, ARMOR_CLASS, 1, 0, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj ValkyrieSpikedSilverShield[] = { { SPIKED_SILVER_SHIELD, 3, ARMOR_CLASS, 1, 0, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightLeatherGloves[] = { { LEATHER_GLOVES, 0, ARMOR_CLASS, 1, 0, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj KnightSilverGauntlets[] = { { SILVER_GAUNTLETS, 0, ARMOR_CLASS, 1, 0, 0 },
								{ 0, 0, 0, 0, 0, 0 } };
static struct trobj PriestSilverGauntlets[] = { { SILVER_GAUNTLETS, 0, ARMOR_CLASS, 1, 1, 0 },
								{ 0, 0, 0, 0, 0, 0 } };

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
    { PM_ELF, CRAM_RATION, LEMBAS_WAFER },
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
    { PM_ORC, LEMBAS_WAFER, TRIPE_RATION },
    { PM_DWARF, SPEAR, DWARVISH_SPEAR },
    { PM_DWARF, SHORT_SWORD, DWARVISH_SHORT_SWORD },
    { PM_DWARF, HELMET, DWARVISH_IRON_HELM },
    /* { PM_DWARF, SMALL_SHIELD, DWARVISH_ROUNDSHIELD }, */
    /* { PM_DWARF, PICK_AXE, DWARVISH_MATTOCK }, */
    { PM_DWARF, LEMBAS_WAFER, CRAM_RATION },
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
	{ PM_GNOLL, RING_MAIL, GNOLLISH_LEATHER_ARMOR },
	{ PM_GNOLL, CHAIN_MAIL, GNOLLISH_STUDDED_LEATHER_ARMOR },
	{ PM_GNOLL, LEATHER_ARMOR, GNOLLISH_LEATHER_ARMOR },
	{ PM_GNOLL, STUDDED_LEATHER_ARMOR, GNOLLISH_STUDDED_LEATHER_ARMOR },
	{ PM_GNOLL, ROBE, GNOLLISH_HAIRCLOTH_ROBE },
	{ PM_GNOLL, CLERICAL_GOWN, GNOLLISH_HAIRCLOTH_ROBE },
	{ PM_GNOLL, CRAM_RATION, TRIPE_RATION },
	{ PM_GNOLL, LEMBAS_WAFER, TRIPE_RATION },
	{ PM_GNOLL, FOOD_RATION, TRIPE_RATION },
	{ PM_GNOLL, APPLE, TIN },
	{ PM_GNOLL, CARROT, TIN },
	{ PM_GNOLL, ORANGE, TIN },
	{ PM_GNOLL, BANANA, TIN },
	{ PM_GNOLL, PEAR, TIN },
	{ NON_PM, STRANGE_OBJECT, STRANGE_OBJECT }
};

static const struct def_skill Skill_A_Max[] = {
    { P_DAGGER, P_BASIC },
    { P_PICK_AXE, P_EXPERT },
    { P_SWORD, P_SKILLED },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_QUARTERSTAFF, P_SKILLED },
    { P_SLING, P_SKILLED },
    { P_THROWN_WEAPON, P_SKILLED },
    { P_WHIP, P_EXPERT },
    { P_ARCANE_SPELL, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
	{ P_ABJURATION_SPELL, P_BASIC },
	{ P_DIVINATION_SPELL, P_EXPERT },
    { P_TRANSMUTATION_SPELL, P_BASIC },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_DISARM_TRAP, P_SKILLED },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_A_Init[] = {
	{ P_WHIP, P_BASIC },
	{ P_DIVINATION_SPELL, P_BASIC },
	{ P_NONE, 0 }
};




static const struct def_skill Skill_B_Max[] = {
    { P_DAGGER, P_BASIC },
    { P_AXE, P_EXPERT },
    { P_PICK_AXE, P_SKILLED },
    { P_SWORD, P_EXPERT },
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_BASIC },
    { P_QUARTERSTAFF, P_BASIC },
    { P_SPEAR, P_SKILLED },
    { P_BOW, P_BASIC },
    { P_ARCANE_SPELL, P_BASIC },
    { P_MOVEMENT_SPELL, P_BASIC }, /* special spell is haste self */
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
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
    { P_DAGGER, P_BASIC },
    { P_AXE, P_SKILLED },
    { P_PICK_AXE, P_BASIC },
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARM, P_SKILLED },
    { P_SPEAR, P_EXPERT },
    { P_BOW, P_SKILLED },
    { P_SLING, P_EXPERT },
    { P_ARCANE_SPELL, P_BASIC },
    { P_TRANSMUTATION_SPELL, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_DISARM_TRAP, P_BASIC },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_C_Init[] = {
	{ P_BLUDGEONING_WEAPON, P_SKILLED },
	{ P_QUARTERSTAFF, P_BASIC },
	{ P_SPEAR, P_BASIC },
	{ P_SLING, P_BASIC },
	{ P_THROWN_WEAPON, P_BASIC },
	{ P_BARE_HANDED_COMBAT, P_BASIC },
	{ P_NONE, 0 }
};


static const struct def_skill Skill_H_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_SWORD, P_SKILLED },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARM, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_SLING, P_SKILLED },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_HEALING_SPELL, P_EXPERT },
	{ P_ABJURATION_SPELL, P_SKILLED },
	{ P_CLERIC_SPELL, P_BASIC },
	{ P_BARE_HANDED_COMBAT, P_BASIC },
	{ P_DISARM_TRAP, P_SKILLED },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_H_Init[] = {
	{ P_DAGGER, P_BASIC },
	{ P_QUARTERSTAFF, P_BASIC },
	{ P_HEALING_SPELL, P_SKILLED },
	{ P_NONE, 0 }
};



static const struct def_skill Skill_K_Max[] = {
    { P_DAGGER, P_BASIC },
    { P_AXE, P_SKILLED },
    { P_PICK_AXE, P_BASIC },
    { P_SWORD, P_EXPERT },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_FLAIL, P_BASIC },
    { P_POLEARM, P_SKILLED },
    { P_SPEAR, P_EXPERT },
    { P_BOW, P_BASIC },
    { P_CROSSBOW, P_SKILLED },
    { P_ARCANE_SPELL, P_SKILLED },
    { P_HEALING_SPELL, P_SKILLED },
    { P_CLERIC_SPELL, P_SKILLED },
	{ P_ABJURATION_SPELL, P_SKILLED },
	{ P_RIDING, P_EXPERT },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_DISARM_TRAP, P_BASIC },
	{ P_NONE, 0 }
};

static const struct def_skill Skill_K_Init[] = {
	{ P_SWORD, P_BASIC },
	{ P_SPEAR, P_BASIC },
	{ P_RIDING, P_BASIC },
	{ P_NONE, 0 }
};


static const struct def_skill Skill_Monk_Max[] = {
    { P_QUARTERSTAFF, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_THROWN_WEAPON, P_BASIC },
    { P_ARCANE_SPELL, P_BASIC },
	{ P_ABJURATION_SPELL, P_EXPERT },
	{ P_HEALING_SPELL, P_EXPERT },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_CLERIC_SPELL, P_SKILLED },
    { P_MOVEMENT_SPELL, P_EXPERT },
    { P_TRANSMUTATION_SPELL, P_BASIC },
	{ P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_MARTIAL_ARTS, P_GRAND_MASTER },
	{ P_TWO_WEAPON_COMBAT, P_EXPERT },
	{ P_DISARM_TRAP, P_BASIC },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_Monk_Init[] = {
	{ P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_MARTIAL_ARTS, P_BASIC },
	{ P_HEALING_SPELL, P_BASIC },
	{ P_ENCHANTMENT_SPELL, P_BASIC },
	{ P_MOVEMENT_SPELL, P_BASIC },
	{ P_ABJURATION_SPELL, P_BASIC },
	{ P_NONE, 0 }
};


static const struct def_skill Skill_P_Max[] = {
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_FLAIL, P_EXPERT },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARM, P_SKILLED },
    { P_SPEAR, P_SKILLED },
    { P_BOW, P_BASIC },
    { P_SLING, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_THROWN_WEAPON, P_BASIC },
    { P_HEALING_SPELL, P_EXPERT },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_CLERIC_SPELL, P_EXPERT },
	{ P_ABJURATION_SPELL, P_EXPERT },
	{ P_CONJURATION_SPELL, P_SKILLED },
	{ P_NECROMANCY_SPELL, P_EXPERT },
	{ P_BARE_HANDED_COMBAT, P_BASIC },
    { P_NONE, 0 }
};

static const struct def_skill Skill_P_Init_Chaotic[] = {
	{ P_BLUDGEONING_WEAPON, P_BASIC },
	{ P_CLERIC_SPELL, P_BASIC },
	{ P_NECROMANCY_SPELL, P_BASIC },
	{ P_HEALING_SPELL, P_BASIC },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_P_Init_NonChaotic[] = {
	{ P_BLUDGEONING_WEAPON, P_BASIC },
	{ P_FLAIL, P_BASIC },
	{ P_HEALING_SPELL, P_BASIC },
	{ P_CLERIC_SPELL, P_BASIC },
	{ P_ABJURATION_SPELL, P_BASIC },
	{ P_NONE, 0 }
};


static const struct def_skill Skill_Rogue_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_SWORD, P_EXPERT },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_FLAIL, P_BASIC },
    { P_POLEARM, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_DIVINATION_SPELL, P_SKILLED },
    { P_MOVEMENT_SPELL, P_SKILLED },
    { P_TRANSMUTATION_SPELL, P_SKILLED },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_DISARM_TRAP, P_EXPERT },
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
    { P_AXE, P_SKILLED },
    { P_PICK_AXE, P_BASIC },
    { P_SWORD, P_BASIC },
    { P_FLAIL, P_SKILLED },
    { P_BLUDGEONING_WEAPON, P_BASIC },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARM, P_SKILLED },
    { P_SPEAR, P_EXPERT },
    { P_BOW, P_EXPERT },
    { P_SLING, P_EXPERT },
    { P_CROSSBOW, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_WHIP, P_BASIC },
    { P_HEALING_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_MOVEMENT_SPELL, P_BASIC },
	{ P_ABJURATION_SPELL, P_BASIC },
	{ P_CONJURATION_SPELL, P_EXPERT },
	{ P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
	{ P_DISARM_TRAP, P_EXPERT },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_Ranger_Init[] = {
	{ P_DAGGER, P_BASIC },
	{ P_SPEAR, P_BASIC },
	{ P_BOW, P_BASIC },
	{ P_CROSSBOW, P_BASIC },
	{ P_DISARM_TRAP, P_BASIC },
	{ P_NONE, 0 }
};


static const struct def_skill Skill_S_Max[] = {
    { P_DAGGER, P_SKILLED },
    { P_SWORD, P_EXPERT },
    { P_FLAIL, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARM, P_EXPERT },
    { P_SPEAR, P_SKILLED },
    { P_BOW, P_EXPERT },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_ARCANE_SPELL, P_BASIC },
    { P_DIVINATION_SPELL, P_BASIC }, /* special spell is clairvoyance */
    { P_CLERIC_SPELL, P_SKILLED },
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
	{ P_BARE_HANDED_COMBAT, P_EXPERT },
	{ P_MARTIAL_ARTS, P_SKILLED },
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
    { P_AXE, P_BASIC },
    { P_PICK_AXE, P_BASIC },
    { P_SWORD, P_EXPERT },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_FLAIL, P_BASIC },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARM, P_BASIC },
    { P_SPEAR, P_BASIC },
    { P_BOW, P_BASIC },
    { P_SLING, P_BASIC },
    { P_CROSSBOW, P_BASIC },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_WHIP, P_BASIC },
    { P_DIVINATION_SPELL, P_BASIC },
    { P_ENCHANTMENT_SPELL, P_BASIC },
    { P_MOVEMENT_SPELL, P_SKILLED },
    { P_RIDING, P_BASIC },
    { P_TWO_WEAPON_COMBAT, P_SKILLED },
    { P_BARE_HANDED_COMBAT, P_SKILLED },
	{ P_DISARM_TRAP, P_SKILLED },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_T_Init[] = {
	{ P_THROWN_WEAPON, P_BASIC },
	{ P_NONE, 0 }
};


static const struct def_skill Skill_V_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_EXPERT },
    { P_PICK_AXE, P_SKILLED },
    { P_SWORD, P_EXPERT },
    { P_BLUDGEONING_WEAPON, P_EXPERT },
    { P_QUARTERSTAFF, P_BASIC },
    { P_POLEARM, P_SKILLED },
    { P_SPEAR, P_SKILLED },
    { P_SLING, P_BASIC },
    { P_ARCANE_SPELL, P_BASIC },
    { P_MOVEMENT_SPELL, P_BASIC },
    { P_RIDING, P_SKILLED },
    { P_TWO_WEAPON_COMBAT, P_EXPERT },
    { P_BARE_HANDED_COMBAT, P_EXPERT },
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
	{ P_NONE, 0 }
};

static const struct def_skill Skill_W_Max[] = {
    { P_DAGGER, P_EXPERT },
    { P_AXE, P_SKILLED },
    { P_SWORD, P_BASIC },
    { P_BLUDGEONING_WEAPON, P_SKILLED },
    { P_QUARTERSTAFF, P_EXPERT },
    { P_POLEARM, P_SKILLED },
    { P_SPEAR, P_BASIC },
    { P_SLING, P_SKILLED },
    { P_THROWN_WEAPON, P_EXPERT },
    { P_ARCANE_SPELL, P_EXPERT },
    { P_DIVINATION_SPELL, P_EXPERT },
    { P_ENCHANTMENT_SPELL, P_SKILLED },
    { P_MOVEMENT_SPELL, P_EXPERT },
    { P_TRANSMUTATION_SPELL, P_EXPERT },
	{ P_CONJURATION_SPELL, P_EXPERT },
	{ P_ABJURATION_SPELL, P_SKILLED },
	{ P_NECROMANCY_SPELL, P_SKILLED },
	{ P_RIDING, P_BASIC },
    { P_BARE_HANDED_COMBAT, P_BASIC },
	{ P_DISARM_TRAP, P_BASIC },
	{ P_NONE, 0 }
};

static const struct def_skill Skill_W_Init_Chaotic[] = {
	{ P_ARCANE_SPELL, P_BASIC },
	{ P_NECROMANCY_SPELL, P_BASIC },
	{ P_NONE, 0 }
};
static const struct def_skill Skill_W_Init_NonChaotic[] = {
	{ P_ARCANE_SPELL, P_BASIC },
	{ P_ENCHANTMENT_SPELL, P_BASIC },
	{ P_NONE, 0 }
};


STATIC_OVL void
knows_object(obj)
register int obj;
{
    discover_object(obj, TRUE, FALSE);
    objects[obj].oc_pre_discovered = 1; /* not a "discovery" */
}

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

void
props_init()
{
	u.uprops[LAUGHING].recurring = TRUE;
	u.uprops[LAUGHING].recurring_constant = 0;
	u.uprops[LAUGHING].recurring_random = 20;

	u.uprops[FUMBLING].recurring = TRUE;
	u.uprops[FUMBLING].recurring_constant = 0;
	u.uprops[FUMBLING].recurring_random = 20;

	u.uprops[ODD_IDEAS].recurring = TRUE;
	u.uprops[ODD_IDEAS].recurring_constant = 150;
	u.uprops[ODD_IDEAS].recurring_random = 100;

	u.uprops[SLEEPY].recurring = TRUE;
	u.uprops[SLEEPY].recurring_constant = 0;
	u.uprops[SLEEPY].recurring_random = 100;

	u.uprops[STRANGLED].recurring = TRUE;
	u.uprops[STRANGLED].recurring_constant = 6;
	u.uprops[STRANGLED].recurring_random = 0;

	u.uprops[AIRLESS_ENVIRONMENT].recurring = TRUE;
	u.uprops[AIRLESS_ENVIRONMENT].recurring_constant = 6;
	u.uprops[AIRLESS_ENVIRONMENT].recurring_random = 0;
}

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

    u.umonnum = u.umonster = (flags.female && urole.femalenum != NON_PM)
                                 ? urole.femalenum
                                 : urole.malenum;
    u.ulycn = NON_PM;
    set_uasmon();

	u.carrying_capacity_level = UNENCUMBERED;

    u.ulevel = 0; /* set up some of the initial attributes */

	u.ubasehpmax = newhp();
	u.ubaseenmax = newpw();

	u.uspellprot = 0;
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
    u.ublesscnt = Role_if(PM_PRIEST) ? 150 : 300; /* no prayers just yet */
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

	int n = 0;

    /*** Role-specific initializations ***/
    switch (Role_switch) {
    /* rn2(100) > 50 necessary for some choices because some
     * random number generators are bad enough to seriously
     * skew the results if we use rn2(2)...  --KAA
     */
    case PM_ARCHEOLOGIST:
        ini_inv(Archeologist);
        if (!rn2(10))
            ini_inv(Tinopener);
        else if (!rn2(4))
            ini_inv(Lamp);
        else if (!rn2(10))
            ini_inv(Magicmarker);
		knows_object(SACK);
		knows_object(BACKPACK);
        knows_object(TOUCHSTONE);
        skill_init(Skill_A_Init, Skill_A_Max);
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
        skill_init(Skill_B_Init, Skill_B_Max);
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
        skill_init(Skill_H_Init, Skill_H_Max);
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
        skill_init(Skill_K_Init, Skill_K_Max);
        break;
    case PM_MONK: {
        static short M_spell[] = { SPE_HEALING, SPE_PROTECTION, SPE_SLEEP };

        Monk[M_BOOK].trotyp = M_spell[rn2(90) / 30]; /* [0..2] */
        ini_inv(Monk);
        if (!rn2(5))
            ini_inv(Magicmarker);
        else if (!rn2(10))
            ini_inv(Lamp);
        //knows_class(ARMOR_CLASS);
        /* sufficiently martial-arts oriented item to ignore language issue */
        knows_object(SHURIKEN);
        skill_init(Skill_Monk_Init, Skill_Monk_Max);
        break;
    }
    case PM_PRIEST:
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
		if(u.ualign.type == A_CHAOTIC)
	        skill_init(Skill_P_Init_Chaotic, Skill_P_Max);
		else
			skill_init(Skill_P_Init_NonChaotic, Skill_P_Max);
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

        skill_init(Skill_Ranger_Init, Skill_Ranger_Max);
        break;
    case PM_ROGUE:
        Rogue[R_DAGGERS].trquan = rn1(5, 6);
        u.umoney0 = 0;
        ini_inv(Rogue);
        if (!rn2(5))
            ini_inv(Blindfold);
        knows_object(SACK);
		knows_object(BAG_OF_TREASURE_HAULING);
		skill_init(Skill_Rogue_Init, Skill_Rogue_Max);
        break;
    case PM_SAMURAI:
        Samurai[S_ARROWS].trquan = rn1(20, 26);
        ini_inv(Samurai);
        if (!rn2(5))
            ini_inv(Blindfold);
        //knows_class(WEAPON_CLASS);
        //knows_class(ARMOR_CLASS);
        skill_init(Skill_S_Init, Skill_S_Max);
        break;
    case PM_TOURIST:
		u.umoney0 = 300 + rnd(700);
		if (flags.female)
		{
			TouristFemale[T_DARTS].trquan = rn1(20, 21);
			ini_inv(TouristFemale);
		}
		else
		{
			Tourist[T_DARTS].trquan = rn1(20, 21);
			ini_inv(Tourist);
		}

		if (!rn2(25))
            ini_inv(Tinopener);
        else if (!rn2(25))
            ini_inv(Leash);
        else if (!rn2(25))
            ini_inv(Magicmarker);
        skill_init(Skill_T_Init, Skill_T_Max);
		knows_object(EXPENSIVE_HANDBAG);
		knows_object(LEATHER_BAG);
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
        skill_init(Skill_V_Init, Skill_V_Max);
        break;
    case PM_WIZARD:
        ini_inv(!rn2(2) ? Wizard : WizardAlternate);
        if (!rn2(5))
            ini_inv(Magicmarker);
        if (!rn2(5))
            ini_inv(Blindfold);

		if (u.ualign.type == A_CHAOTIC)
			skill_init(Skill_W_Init_Chaotic, Skill_W_Max);
		else
			skill_init(Skill_W_Init_NonChaotic, Skill_W_Max);

        break;

    default: /* impossible */
        break;
    }

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
            static int trotyp[] = { WOODEN_FLUTE, TOOLED_HORN, WOODEN_HARP,
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
		break;

    case PM_GNOLL:
		knows_object(GNOLLISH_HAIRCLOTH_ROBE);
		knows_object(GNOLLISH_HOOD);
		knows_object(GNOLLISH_LEATHER_ARMOR);
		knows_object(GNOLLISH_STUDDED_LEATHER_ARMOR);
		knows_object(GNOLLISH_QUARREL);
		P_MAX_SKILL(P_FLAIL) = P_EXPERT;
		if(P_MAX_SKILL(P_CROSSBOW) < P_SKILLED)
			P_MAX_SKILL(P_CROSSBOW) = P_SKILLED;
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
        knows_object(URUK_HAI_SHIELD);
        knows_object(ORCISH_CLOAK);
        break;

    default: /* impossible */
        break;
    }

    if (discover)
        ini_inv(Wishing);

    if (wizard)
        read_wizkit();

    if (u.umoney0)
        ini_inv(Money);
    u.umoney0 += hidden_gold(); /* in case sack has gold in it */

	/* //OBSOLETE, DONE BELOW
	find_ac();     
	find_mc();
	*/

	/*
     *  Do we really need this?
     */
    for (i = 0; i < A_MAX; i++)
        if (!rn2(20)) {
            register int xd = rn2(7) - 2; /* biased variation */

            (void) adjattrib(i, xd, TRUE);
            if (ABASE(i) < AMAX(i))
                AMAX(i) = ABASE(i);
        }

    /* make sure you can carry all you have - especially for Tourists */
    while (inv_weight() > 0) {
        if (adjattrib(A_STR, 1, TRUE))
            continue;
        if (adjattrib(A_CON, 1, TRUE))
            continue;
        /* only get here when didn't boost strength or constitution */
        break;
    }

	//Check if any skill can be advanced
	update_can_advance_any_skill();

	/* Run initial stats */
	update_extrinsics();
	updateabon();
	updatemaxen();
	updatemaxhp();

    return;
}

/* skills aren't initialized, so we use the role-specific skill lists */
STATIC_OVL boolean
restricted_spell_discipline(otyp)
int otyp;
{
    const struct def_skill *skills;
    int this_skill = spell_skilltype(otyp);

    switch (Role_switch) {
    case PM_ARCHEOLOGIST:
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
        skills = Skill_P_Max;
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
    int otyp, i;

    while (trop->trclass) {
        otyp = (int) trop->trotyp;
        if (otyp != UNDEF_TYP)
		{
            obj = mksobj(otyp, TRUE, FALSE, FALSE);
		} else { /* UNDEF_TYP */
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
                   || otyp == SCR_AMNESIA
                   || otyp == SCR_FIRE
                   || otyp == SCR_BLANK_PAPER
                   || otyp == SPE_BLANK_PAPER
                   || otyp == WAN_NOTHING
				   || already_learnt_spell_type(otyp)
                   /* orcs start with poison resistance */
                   || (otyp == RIN_POISON_RESISTANCE && Race_if(PM_ORC))
                   /* Monks don't use weapons */
                   || (otyp == SCR_ENCHANT_WEAPON && Role_if(PM_MONK))
                   /* wizard patch -- they already have one */
                   || (otyp == SPE_FORCE_BOLT && Role_if(PM_WIZARD))
				   || (otyp == SPE_MAGIC_ARROW && Role_if(PM_WIZARD))
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
						   ))) {
                dealloc_obj(obj);
                obj = mkobj(trop->trclass, FALSE, FALSE);
                otyp = obj->otyp;
            }

            /* Don't start with +0 or negative rings */
            if (objects[otyp].oc_charged && obj->spe <= 0)
                obj->spe = rne(3);

            /* Heavily relies on the fact that 1) we create wands
             * before rings, 2) that we create rings before
             * spellbooks, and that 3) not more than 1 object of a
             * particular symbol is to be prohibited.  (For more
             * objects, we need more nocreate variables...)
             */
            switch (otyp) {
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

        if (urace.malenum != PM_HUMAN) {
            /* substitute race-specific items; this used to be in
               the 'if (otyp != UNDEF_TYP) { }' block above, but then
               substitutions didn't occur for randomly generated items
               (particularly food) which have racial substitutes */
            for (i = 0; inv_subs[i].race_pm != NON_PM; ++i)
				if (inv_subs[i].race_pm == urace.malenum
					&& otyp == inv_subs[i].item_otyp) {
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
				(void)add_to_container(obj, mksobj(BATHROBE, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(COTTON_SLIPPERS, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(TOWEL, FALSE, FALSE, TRUE));
				knows_object(BATHROBE);
				obj->owt = weight(obj);
			}
		}
		else if (Role_if(PM_MONK))
		{
			if (otyp == ORIENTAL_SILK_SACK)
			{
				(void)add_to_container(obj, mksobj(GINSENG_ROOT, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(GINSENG_ROOT, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(ORANGE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(ORANGE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(ORANGE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(ORANGE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(ORANGE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(HANDFUL_OF_NUTS, TRUE, FALSE, TRUE));
				if(!rn2(2))
					(void)add_to_container(obj, mksobj(MELON, FALSE, FALSE, TRUE));
				if (!rn2(2))
					(void)add_to_container(obj, mksobj(FOOD_RATION, FALSE, FALSE, TRUE));
				obj->owt = weight(obj);
			}
		}
		else if (Role_if(PM_ARCHEOLOGIST))
		{
			if (otyp == BACKPACK)
			{
				(void)add_to_container(obj, mksobj(BANANA, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(POT_WATER, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(SCR_BLANK_PAPER, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(SPE_BLANK_PAPER, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(HANDFUL_OF_NUTS, TRUE, FALSE, TRUE));
				if (!rn2(2))
					(void)add_to_container(obj, mksobj(FOOD_RATION, FALSE, FALSE, TRUE));
				if (!rn2(2))
					(void)add_to_container(obj, mksobj(FOOD_RATION, FALSE, FALSE, TRUE));
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
				(void)add_to_container(obj, mksobj(SILVER_FORK, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(SILVER_KNIFE, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(FOOD_RATION, FALSE, FALSE, TRUE));
				if(!rn2(2))
					(void)add_to_container(obj, mksobj(FOOD_RATION, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(APPLE, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(CARROT, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(CARROT, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(CARROT, TRUE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(POT_WATER, FALSE, FALSE, TRUE));
				knows_object(POT_WATER);
				obj->owt = weight(obj);
			}
		}
		else if (Role_if(PM_WIZARD))
		{
			if (otyp == BAG_OF_WIZARDRY)
			{
				/* Add one sulfur */
				struct obj* otmp = (struct obj*)0;
				otmp = mksobj(PINCH_OF_SULFUROUS_ASH, FALSE, FALSE, TRUE);
				otmp->known = 1;
				otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
				(void)add_to_container(obj, otmp);

				/* Add arrows for magic arrow */
				otmp = mksobj(ARROW, FALSE, FALSE, TRUE);
				otmp->quan = 2 + rnd(5);
				otmp->owt = weight(otmp);
				otmp->known = 1;
				otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
				(void)add_to_container(obj, otmp);

				int n = 2 + rn2(4); //2...5
				for (int i = 0; i < n; i++)
				{
					struct obj* otmp = mksobj(randomreagent(FALSE, 2), FALSE, FALSE, FALSE);
					if (otmp)
					{
						knows_object(otmp->otyp);
						otmp->known = 1;
						otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
						(void)add_to_container(obj, otmp);
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
				(void)add_to_container(obj, mksobj(GINSENG_ROOT, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(GINSENG_ROOT, FALSE, FALSE, TRUE));
				(void)add_to_container(obj, mksobj(GINSENG_ROOT, TRUE, FALSE, TRUE));

				int n = rnd(3); //1...3
				for (int i = 0; i < n; i++)
				{
					struct obj* otmp = mkobj(REAGENT_CLASS, FALSE, TRUE);
					if (otmp)
					{
						knows_object(otmp->otyp);
						(void)add_to_container(obj, otmp);
					}
				}

				/* Last update bag weight */
				obj->owt = weight(obj);

			}
		}

		if (trop->trclass == COIN_CLASS) {
            /* no "blessed" or "identified" money */
            obj->quan = u.umoney0;
        } else {
            if (objects[otyp].oc_uses_known)
                obj->known = 1;
            obj->dknown = obj->bknown = obj->rknown = obj->nknown = 1;
            if (Is_container(obj) || obj->otyp == STATUE) {
                obj->cknown = obj->lknown = 1;
                obj->otrapped = 0;
            }
            obj->cursed = 0;
            if (obj->opoisoned && u.ualign.type != A_CHAOTIC)
                obj->opoisoned = 0;
            if (obj->oclass == WEAPON_CLASS || obj->oclass == TOOL_CLASS) {
                obj->quan = (long) trop->trquan;
                trop->trquan = 1;
            } else if (obj->oclass == GEM_CLASS && is_graystone(obj)) {
                obj->quan = 1L;
            }
            if (trop->trspe != UNDEF_SPE)
                obj->spe = trop->trspe;
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
		}
        /* defined after setting otyp+quan + blessedness */
        obj->owt = weight(obj);
        obj = addinv(obj);

        /* Make the type known if necessary */
        if (OBJ_DESCR(objects[otyp]) && obj->known)
            discover_object(otyp, TRUE, FALSE);
        if (otyp == OIL_LAMP)
            discover_object(POT_OIL, TRUE, FALSE);

        if (obj->oclass == ARMOR_CLASS) {
            if (is_shield(obj) && !uarms && !(uwep && bimanual(uwep))) {
                setworn(obj, W_ARMS);
                /* 3.6.2: this used to unset uswapwep if it was set, but
                   wearing a shield doesn't prevent having an alternate
                   weapon ready to swap with the primary; just make sure we
                   aren't two-weaponing (academic; no one starts that way) */
                u.twoweap = FALSE;
            } else if (is_helmet(obj) && !uarmh)
                setworn(obj, W_ARMH);
            else if (is_gloves(obj) && !uarmg)
                setworn(obj, W_ARMG);
            else if (is_shirt(obj) && !uarmu)
                setworn(obj, W_ARMU);
            else if (is_cloak(obj) && !uarmc)
                setworn(obj, W_ARMC);
			else if (is_robe(obj) && !uarmo)
				setworn(obj, W_ARMO);
			else if (is_bracers(obj) && !uarmb)
				setworn(obj, W_ARMB);
			else if (is_boots(obj) && !uarmf)
                setworn(obj, W_ARMF);
            else if (is_suit(obj) && !uarm)
                setworn(obj, W_ARM);
        }

		if (obj->oclass == AMULET_CLASS && !uamul)
		{
			setworn(obj, W_AMUL);
		}

		if (obj->oclass == MISCELLANEOUS_CLASS && !(umisc && umisc2 && umisc3 && umisc4 && umisc5))
		{
			if (objects[obj->otyp].oc_subtyp != MISC_MULTIPLE_PERMITTED &&
				(umisc && objects[umisc->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
				|| (umisc2 && objects[umisc2->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
				|| (umisc3 && objects[umisc3->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
				|| (umisc4 && objects[umisc4->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
				|| (umisc5 && objects[umisc5->otyp].oc_subtyp == objects[obj->otyp].oc_subtyp)
				)
			{
				//Nothing
			}
			else
			{
				if(!umisc)
					setworn(obj, W_MISC);
				else if (!umisc2)
					setworn(obj, W_MISC2);
				else if (!umisc3)
					setworn(obj, W_MISC3);
				else if (!umisc4)
					setworn(obj, W_MISC4);
				else if (!umisc5)
					setworn(obj, W_MISC5);
			}
		}


        if (obj->oclass == WEAPON_CLASS || is_weptool(obj) || otyp == TIN_OPENER || otyp == FLINT || is_rock(obj)) 
		{
            if (is_ammo(obj) || is_missile(obj)) 
			{
                if (!uquiver)
                    setuqwep(obj);
            }
			else if (!uwep && (!uarms || !bimanual(obj)) && !is_launcher(obj))
			{
                setuwep(obj, W_WEP);
            }
			else if (!uswapwep && is_launcher(obj))
			{
                setuswapwep(obj, W_SWAPWEP);
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
        if (trop->trquan) { /* check if zero first */
            --trop->trquan;
            if (trop->trquan)
                continue; /* make a similar object */
        }
#endif
        trop++;
    }
}

/*u_init.c*/

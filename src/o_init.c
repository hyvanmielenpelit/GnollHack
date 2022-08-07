/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    o_init.c    $NHDT-Date: 1545383615 2018/12/21 09:13:35 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.25 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h" /* save & restore info */

#define MYTHIC_STANDARD_PRICE_MULTIPLIER 2.0
#define MYTHIC_STANDARD_PRICE_ADDITION 200L

/* Saved initial object data */
NEARDATA struct objdescr saved_obj_descr[NUM_OBJECTS];
NEARDATA struct objclass saved_objects[NUM_OBJECTS];

NEARDATA struct mythic_definition mythic_prefix_qualities[MAX_MYTHIC_PREFIXES] =
{
    { "", "", "", 0, 1.0, 0L, 0UL, 0UL },
    { 
        "Stygian", "Stygian ", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_LEVEL_DRAIN | MYTHIC_PREFIX_POWER_ARMOR_DEATH_RESISTANCE | MYTHIC_PREFIX_POWER_ARMOR_DRAIN_RESISTANCE,
        MYTHIC_FLAG_NO_CELESTIAL_WEAPONS | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "Hyperborean", "Hyperborean ", "", 25, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_MANA_GAIN_25,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_NO_THROWN_OR_AMMO | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "Asgardian", "Asgardian ", "", 25, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_HP_GAIN_25,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_NO_THROWN_OR_AMMO | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "Atlantean", "Atlantean ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_MANA_REGENERATION,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "Elysian", "Elysian ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_REGENERATION,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "Cyclopean", "Cyclopean ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_TRIPLE_BASE_DAMAGE | MYTHIC_PREFIX_POWER_ADDED_ENCHANTABILITY,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "Herculean", "Herculean ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_GREAT_STRENGTH,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_SUIT_REQUIRED | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "vampiric", "vampiric ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_LIFE_DRAINING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_THROWN_OR_AMMO
    },
    {
        "radiant", "radiant ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_SHINES_LIGHT,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_THROWN_OR_AMMO
    },
    {
        "hallowed", "hallowed ", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_CURSE_RESISTANCE | MYTHIC_PREFIX_POWER_UNCURSEABLE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_NO_INFERNAL_WEAPONS | MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS | MYTHIC_FLAG_NO_THROWN_OR_AMMO | MYTHIC_FLAG_NO_DEMONIC_ITEMS
    },
    {
        "witch-king's", "witch-king's ", "", 3, MYTHIC_STANDARD_PRICE_MULTIPLIER + 0.5, MYTHIC_STANDARD_PRICE_ADDITION * 4L,
        MYTHIC_PREFIX_POWER_LEVEL_DRAIN | MYTHIC_PREFIX_POWER_MANA_GAIN_25 | MYTHIC_PREFIX_POWER_HP_GAIN_25,
        MYTHIC_FLAG_NO_CELESTIAL_WEAPONS | MYTHIC_FLAG_NO_THROWN_OR_AMMO | MYTHIC_FLAG_RACIAL_PREFIX
    },
    {
        "sorcerer's", "sorcerer's ", "", 20,  MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_PREFIX_POWER_SORCERY,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED | MYTHIC_FLAG_NO_LOW_SPELLCASTING_PENALTY_ITEMS | MYTHIC_FLAG_NO_OTHER_SORCERY
    },
    {
        "Olympian", "Olympian ", "", 5, MYTHIC_STANDARD_PRICE_MULTIPLIER + 0.5, MYTHIC_STANDARD_PRICE_ADDITION * 2L,
        MYTHIC_PREFIX_POWER_TRIPLE_BASE_DAMAGE | MYTHIC_PREFIX_POWER_ADDED_ENCHANTABILITY | MYTHIC_PREFIX_POWER_CURSE_RESISTANCE | MYTHIC_PREFIX_POWER_UNCURSEABLE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_RACIAL_PREFIX | MYTHIC_FLAG_NO_INFERNAL_WEAPONS | MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS | MYTHIC_FLAG_NO_THROWN_OR_AMMO | MYTHIC_FLAG_NO_DEMONIC_ITEMS
    },
    {
        "Jotunheimian", "Jotunheimian ", "", 5, MYTHIC_STANDARD_PRICE_MULTIPLIER + 0.5, MYTHIC_STANDARD_PRICE_ADDITION * 3L,
        MYTHIC_PREFIX_POWER_TRIPLE_BASE_DAMAGE | MYTHIC_PREFIX_POWER_ADDED_ENCHANTABILITY | MYTHIC_PREFIX_POWER_LIFE_DRAINING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_RACIAL_PREFIX | MYTHIC_FLAG_NO_THROWN_OR_AMMO | MYTHIC_FLAG_NO_CELESTIAL_WEAPONS
    },
};

NEARDATA struct mythic_power_definition mythic_prefix_powers[MAX_MYTHIC_PREFIX_POWERS] =
{
    { "Level drain", "Causes level drain", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Mana gain 25%", "Increases maximum mana by 25%", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Hit point gain 25%", "Increases maximum hit points by 25%", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Life draining", "Heals 2d6 hit points plus enchantment bonus on hit", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Shines light", "Shines magical light", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Armor death resistance", "Death resistance", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, DEATH_RESISTANCE, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_ARMOR_ONLY },
    { "Armor drain resistance", "Drain resistance", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, DRAIN_RESISTANCE, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_ARMOR_ONLY },
    { "Regeneration", "Regeneration", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, REGENERATION, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Energy regeneration", "Energy regeneration", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, ENERGY_REGENERATION, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Curse resistance", "Curse resistance", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, CURSE_RESISTANCE, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Triple base damage", "Triple base damage", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Improved enchantability", "Increases maximum enchantable level by 50%", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NONE },
    { "Uncurseable", "Cannot be cursed", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NONE },
    { "Great power", "Raises strength to 18/00 plus enchantment bonus", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NONE },
    { "Sorcery", "Incurs no spellcasting penalty", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
};



NEARDATA struct mythic_definition mythic_suffix_qualities[MAX_MYTHIC_SUFFIXES] =
{
    { "", "", "", 0, 1.0, 0L, 0UL, 0UL },
    { 
        "lightness", " of lightness", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_LIGHTNESS, 
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED
    },
    { 
        "sorcery", " of sorcery", "", 20,  MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_SORCERY,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED | MYTHIC_FLAG_NO_LOW_SPELLCASTING_PENALTY_ITEMS | MYTHIC_FLAG_NO_OTHER_SORCERY
    },
    {
        "troll slaying", " of troll slaying", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_TROLL_SLAYING | MYTHIC_SUFFIX_POWER_TROLL_REVIVAL_PREVENTION,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED
    },
    {
        "ogre slaying", " of ogre slaying", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_OGRE_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED
    },
    {
        "demon slaying", " of demon slaying", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_DEMON_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_INFERNAL_WEAPONS | MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS | MYTHIC_FLAG_NO_DEMONIC_ITEMS
    },
    {
        "dragon slaying", " of dragon slaying", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_DRAGON_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED
    },
    {
        "giant slaying", " of giant slaying", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_GIANT_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED
    },
    {
        "lycanthrope slaying", " of lycanthrope slaying", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_WERE_SLAYING | MYTHIC_SUFFIX_POWER_LYCANTHROPY_RESISTANCE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED
    },
    {
        "disruption", " of disruption", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_UNDEAD_DESTRUCTION,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_BLUDGEONING_WEAPONS_ONLY | MYTHIC_FLAG_NO_INFERNAL_WEAPONS | MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS | MYTHIC_FLAG_NO_DEMONIC_ITEMS
    },
    {
        "speed", " of speed", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_SPEED,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_THROWN_OR_AMMO
    },
    {
        "wounding", " of wounding", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_WOUNDING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_BLUDGEONING_WEAPONS
    },
    {
        "defense", " of defense", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_DEFENSE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_THROWN_OR_AMMO
    },
    {
        "sharpness", " of sharpness", "", 5, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_SHARPNESS,
        MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_SLASHING_WEAPONS_ONLY
    },
    {
        "reach", " of reach", "", 30, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_REACH,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_POLEARM_LANCE_SPEAR_ONLY
    },
    {
        "providence", " of providence", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_LUCK,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_NO_THROWN_OR_AMMO
    },
    {
        "free action", " of free action", "", 15, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_FREE_ACTION,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_NO_THROWN_OR_AMMO
    },
    {
        "fire resistance", " of fire resistance", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_FIRE_RESISTANCE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED
    },
    {
        "cold resistance", " of cold resistance", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_COLD_RESISTANCE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED
    },
    {
        "shock resistance", " of shock resistance", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_SHOCK_RESISTANCE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED
    },
    {
        "cockatrice slaying", " of cockatrice slaying", "", 10, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_COCKATRICE_SLAYING | MYTHIC_SUFFIX_POWER_STONE_RESISTANCE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED
    },
    {
        "eyes", " of eyes", "", 20, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_SEARCHING | MYTHIC_SUFFIX_POWER_SEE_INVISIBLE,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_ARMOR_REQUIRED
    },
    {
        "elemental protection", " of elemental protection", "", 2, MYTHIC_STANDARD_PRICE_MULTIPLIER + 0.5, MYTHIC_STANDARD_PRICE_ADDITION * 6L,
        MYTHIC_SUFFIX_POWER_FIRE_RESISTANCE | MYTHIC_SUFFIX_POWER_COLD_RESISTANCE | MYTHIC_SUFFIX_POWER_SHOCK_RESISTANCE,
        MYTHIC_FLAG_ARMOR_REQUIRED
    },
    {
        "orc slaying", " of orc slaying", "", 10, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_ORC_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_ORCISH_ITEMS
    },
    {
        "elf slaying", " of elf slaying", "", 5, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_ELF_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_ELVEN_ITEMS
    },
    {
        "returning", " of returning", "", 5, MYTHIC_STANDARD_PRICE_MULTIPLIER, MYTHIC_STANDARD_PRICE_ADDITION,
        MYTHIC_SUFFIX_POWER_RETURN_TO_HAND_AFTER_THROW,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_THROWN_WEAPON_ONLY | MYTHIC_FLAG_NO_RETURNING_WEAPONS
    },
    {
        "banishment", " of banishment", "", 4, MYTHIC_STANDARD_PRICE_MULTIPLIER + 0.5, MYTHIC_STANDARD_PRICE_ADDITION * 6L,
        MYTHIC_SUFFIX_POWER_UNDEAD_DESTRUCTION | MYTHIC_SUFFIX_POWER_DEMON_SLAYING,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_BLUDGEONING_WEAPONS_ONLY | MYTHIC_FLAG_NO_INFERNAL_WEAPONS | MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS | MYTHIC_FLAG_NO_DEMONIC_ITEMS
    },
    {
        "Last Alliance", " of the Last Alliance", "", 4, MYTHIC_STANDARD_PRICE_MULTIPLIER + 0.5, MYTHIC_STANDARD_PRICE_ADDITION * 2L,
        MYTHIC_SUFFIX_POWER_ORC_SLAYING | MYTHIC_SUFFIX_POWER_OGRE_SLAYING | MYTHIC_SUFFIX_POWER_GIANT_SLAYING | MYTHIC_SUFFIX_POWER_TROLL_SLAYING | MYTHIC_SUFFIX_POWER_TROLL_REVIVAL_PREVENTION,
        MYTHIC_FLAG_DIRECTLY_WISHABLE | MYTHIC_FLAG_WEAPON_REQUIRED | MYTHIC_FLAG_NO_ORCISH_ITEMS
    },
};

NEARDATA struct mythic_power_definition mythic_suffix_powers[MAX_MYTHIC_SUFFIX_POWERS] =
{
    { "Lightness", "Weighs one-eight of normal", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Sorcery", "Incurs no spellcasting penalty", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Troll slaying", "Triple damage to trolls", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, S_TROLL, 0UL , MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Troll revival prevention", "Prevents revival of trolls", MYTHIC_POWER_TYPE_PREVENTS_REVIVAL, 0L, 0.0, S_TROLL, 0UL , MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Were slaying", "Triple damage to lycanthropes", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_WERE, MYTHIC_POWER_FLAG_WEAPON_ONLY | MYTHIC_POWER_FLAG_ALSO_SHAPESHIFTERS },
    { "Lycanthropy resistance", "Lycanthropy resistance", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, LYCANTHROPY_RESISTANCE, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Giant slaying", "Triple damage to giants", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_GIANT, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Demon slaying", "Triple damage to demons", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_DEMON, MYTHIC_POWER_FLAG_WEAPON_ONLY | MYTHIC_POWER_FLAG_ALSO_SHAPESHIFTERS },
    { "Angel slaying", "Triple damage to angels", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_ANGEL, MYTHIC_POWER_FLAG_WEAPON_ONLY | MYTHIC_POWER_FLAG_ALSO_SHAPESHIFTERS },
    { "Ogre slaying", "Triple damage to ogres", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, S_OGRE, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Orc slaying", "Triple damage to orcs", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, S_ORC, M2_ORC, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Elf slaying", "Triple damage to elves", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_ELF, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Dragon slaying", "Triple damage to dragons", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, S_DRAGON, 0UL , MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Undead destruction", "Triple damage to undead", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_UNDEAD, MYTHIC_POWER_FLAG_WEAPON_ONLY | MYTHIC_POWER_FLAG_ALSO_SHAPESHIFTERS },
    { "Speed", "Increases speed to very fast", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, VERY_FAST, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Wounding", "Causes permanent damage equal to 1d4 + enchantment", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Defense", "Enchantment and quality provide AC and MC", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY | MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Sharpness", "Has 15% chance of dealing damage equal to 15% of max HP", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Reach", "Has extended range", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_WEAPON_ONLY | MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Luck", "Confers luck", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Free action", "Free action", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, FREE_ACTION, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Fire resistance", "Fire immunity", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, FIRE_IMMUNITY, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Cold resistance", "Cold immunity", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, COLD_IMMUNITY, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Shock resistance", "Shock immunity", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, SHOCK_IMMUNITY, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Cockatrice slaying", "Triple damage to cockatrices", MYTHIC_POWER_TYPE_SLAYING, 0L, 3.0, 0, M2_COCKATRICE, MYTHIC_POWER_FLAG_WEAPON_ONLY },
    { "Stone resistance", "Petrification resistance", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, STONE_RESISTANCE, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Searching", "Searching", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, SEARCHING, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "See invisible", "See invisible", MYTHIC_POWER_TYPE_CONFERS_PROPERTY, SEE_INVISIBLE, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_NO_THROWN_OR_AMMO },
    { "Returns to hand", "Returns to hand after throwing", MYTHIC_POWER_TYPE_GENERAL, 0L, 0.0, 0, 0UL, MYTHIC_POWER_FLAG_THROWN_WEAPONS_ONLY },
};

//STATIC_DCL void FDECL(setgemprobs, (d_level *));
STATIC_DCL void FDECL(shuffle, (int, int, BOOLEAN_P));
STATIC_DCL void NDECL(shuffle_all);
STATIC_DCL boolean FDECL(interesting_to_discover, (int));
STATIC_DCL char *FDECL(oclass_to_name, (CHAR_P, char *));

STATIC_VAR NEARDATA short disco[NUM_OBJECTS] = DUMMY;


#ifdef USE_TILES
STATIC_DCL void NDECL(shuffle_tiles);

/* Shuffle tile assignments to match descriptions, so a red potion isn't
 * displayed with a blue tile and so on.
 *
 * Tile assignments are not saved, and shouldn't be so that a game can
 * be resumed on an otherwise identical non-tile-using binary, so we have
 * to reshuffle the assignments from oc_descr_idx information when a game
 * is restored.  So might as well do that the first time instead of writing
 * another routine.
 */
STATIC_OVL void
shuffle_tiles()
{
    int i;
    int tmp_tilemap[NUM_OBJECTS];
    int tmp_tileflags[NUM_OBJECTS];
    int tmp_replacementmap[NUM_OBJECTS];

    for (i = 0; i < NUM_OBJECTS; i++)
    {
        tmp_tilemap[i] = glyph2tile[objects[i].oc_descr_idx + GLYPH_OBJ_OFF];
        tmp_tileflags[i] = glyphtileflags[objects[i].oc_descr_idx + GLYPH_OBJ_OFF];
        tmp_replacementmap[i] = glyph2replacement[objects[i].oc_descr_idx + GLYPH_OBJ_OFF];
    }

    for (i = 0; i < NUM_OBJECTS; i++)
    {
        glyph2tile[i + GLYPH_OBJ_OFF] = tmp_tilemap[i];
        glyphtileflags[i + GLYPH_OBJ_OFF] = tmp_tileflags[i];
        glyph2replacement[i + GLYPH_OBJ_OFF] = tmp_replacementmap[i];
    }

    for (int j = 0; j < NUM_MISSILE_DIRS; j++)
    {
        for (i = 0; i < NUM_OBJECTS; i++)
        {
            tmp_tilemap[i] = glyph2tile[objects[i].oc_descr_idx * NUM_MISSILE_DIRS + j + GLYPH_OBJ_MISSILE_OFF];
            tmp_tileflags[i] = glyphtileflags[objects[i].oc_descr_idx * NUM_MISSILE_DIRS + j + GLYPH_OBJ_MISSILE_OFF];
            tmp_replacementmap[i] = glyph2replacement[objects[i].oc_descr_idx * NUM_MISSILE_DIRS + j + GLYPH_OBJ_MISSILE_OFF];
        }
        for (i = 0; i < NUM_OBJECTS; i++)
        {
            glyph2tile[i * NUM_MISSILE_DIRS + j + GLYPH_OBJ_MISSILE_OFF] = tmp_tilemap[i];
            glyphtileflags[i * NUM_MISSILE_DIRS + j + GLYPH_OBJ_MISSILE_OFF] = tmp_tileflags[i];
            glyph2replacement[i * NUM_MISSILE_DIRS + j + GLYPH_OBJ_MISSILE_OFF] = tmp_replacementmap[i];
        }
    }
}
#endif /* USE_TILES */

#if 0
STATIC_OVL void
setgemprobs(dlev)
d_level *dlev;
{
    int j, first, lev;

    if (dlev)
        lev = (ledger_no(dlev) > maxledgerno()) ? maxledgerno()
                                                : ledger_no(dlev);
    else
        lev = 0;
    first = bases[GEM_CLASS];

    for (j = 0; j < 9 - lev / 3; j++)
        objects[first + j].oc_prob = 0;
    first += j;
    if (first > LAST_GEM || objects[first].oc_class != GEM_CLASS
        || OBJ_NAME(objects[first]) == (char *) 0) {
        raw_printf("Not enough gems? - first=%d j=%d LAST_GEM=%d", first, j,
                   LAST_GEM);
        wait_synch();
    }
    for (j = first; j <= LAST_GEM; j++)
        objects[j].oc_prob = (171 + j - first) / (LAST_GEM + 1 - first);
}
#endif

/* shuffle descriptions on objects o_low to o_high */
STATIC_OVL void
shuffle(o_low, o_high, domaterial)
int o_low, o_high;
boolean domaterial;
{
    int i, j, num_to_shuffle;
    short sw;
    uchar mat;
    int color;

    for (num_to_shuffle = 0, j = o_low; j <= o_high; j++)
        if (!objects[j].oc_name_known)
            num_to_shuffle++;
    if (num_to_shuffle < 2)
        return;

    for (j = o_low; j <= o_high; j++) {
        if (objects[j].oc_name_known)
            continue;
        do
            i = j + rn2(o_high - j + 1);
        while (objects[i].oc_name_known);
        sw = objects[j].oc_descr_idx;
        objects[j].oc_descr_idx = objects[i].oc_descr_idx;
        objects[i].oc_descr_idx = sw;
        sw = objects[j].oc_tough;
        objects[j].oc_tough = objects[i].oc_tough;
        objects[i].oc_tough = sw;
        color = objects[j].oc_color;
        objects[j].oc_color = objects[i].oc_color;
        objects[i].oc_color = color;

        /* shuffle material */
        if (domaterial) {
            mat = objects[j].oc_material;
            objects[j].oc_material = objects[i].oc_material;
            objects[i].oc_material = mat;
        }
    }
}

void
init_objects()
{
    register int i, first, last, sum;
    register char oclass;
#ifdef TEXTCOLOR
#define COPY_OBJ_DESCR(o_dst, o_src) \
    o_dst.oc_descr_idx = o_src.oc_descr_idx, o_dst.oc_color = o_src.oc_color
#else
#define COPY_OBJ_DESCR(o_dst, o_src) o_dst.oc_descr_idx = o_src.oc_descr_idx
#endif

    /* bug fix to prevent "initialization error" abort on Intel Xenix.
     * reported by mikew@semike
     */
    for (i = 0; i < MAX_OBJECT_CLASSES; i++)
        bases[i] = 0;
    
    for (i = 0; i < NUM_OBJECTS; i++)
    {
        /* initialize object descriptions */
        objects[i].oc_name_idx = objects[i].oc_descr_idx = i;
    }
    
    /* initialize object material components, if many pointing to the same spell, the last index remains */
    matcomps_init();

    for (int j = 0; matlists[j].spellsgained > 0; j++)
    {
        objects[matlists[j].spell_objectid].oc_material_components = j;
    }

    /* init base; if probs given check that they add up to 1000,
       otherwise compute probs */
    first = 0;
    while (first < NUM_OBJECTS) {
        oclass = objects[first].oc_class;
        last = first + 1;
        while (last < NUM_OBJECTS && objects[last].oc_class == oclass)
            last++;
        bases[(int) oclass] = first;

        if (oclass == GEM_CLASS) {
           // setgemprobs((d_level *) 0);

            if (rn2(2)) { /* change turquoise from green to blue? */
                COPY_OBJ_DESCR(objects[TURQUOISE], objects[SAPPHIRE]);
            }
            if (rn2(2)) { /* change aquamarine from green to blue? */
                COPY_OBJ_DESCR(objects[AQUAMARINE], objects[SAPPHIRE]);
            }
            switch (rn2(4)) { /* change fluorite from violet? */
            case 0:
                break;
            case 1: /* blue */
                COPY_OBJ_DESCR(objects[FLUORITE], objects[SAPPHIRE]);
                break;
            case 2: /* white */
                COPY_OBJ_DESCR(objects[FLUORITE], objects[DIAMOND]);
                break;
            case 3: /* green */
                COPY_OBJ_DESCR(objects[FLUORITE], objects[EMERALD]);
                break;
            }
        }
    check:
        sum = 0;
        for (i = first; i < last; i++)
            sum += objects[i].oc_prob;
        if (sum == 0) {
            for (i = first; i < last; i++)
                objects[i].oc_prob = (1000 + i - first) / (last - first);
            goto check;
        }
        if (sum != 1000)
            error("init-prob error for class %d (%d%%)", oclass, sum);
        first = last;
    }
    /* shuffle descriptions */
    shuffle_all();
#ifdef USE_TILES
    shuffle_tiles();
#endif
    objects[WAN_NOTHING].oc_dir = rn2(2) ? NODIR : IMMEDIATE;

}

/* retrieve the range of objects that otyp shares descriptions with */
void
obj_shuffle_range(otyp, lo_p, hi_p)
int otyp;         /* input: representative item */
int *lo_p, *hi_p; /* output: range that item belongs among */
{
    int i, ocls = objects[otyp].oc_class;

    /* default is just the object itself */
    *lo_p = *hi_p = otyp;

    switch (ocls) {
    case ARMOR_CLASS:
        if (otyp >= HELMET && otyp <= HELM_OF_TELEPATHY)
            *lo_p = HELMET, *hi_p = HELM_OF_TELEPATHY;
        else if (otyp >= ROYAL_CROWN && otyp <= CROWN_OF_RULERSHIP)
            *lo_p = ROYAL_CROWN, * hi_p = CROWN_OF_RULERSHIP;
        else if (otyp >= CORNUTHAUM && otyp <= GNOMISH_FELT_HAT)
            *lo_p = CORNUTHAUM, * hi_p = GNOMISH_FELT_HAT;
        else if (otyp >= LEATHER_GLOVES && otyp <= GAUNTLETS_OF_DEXTERITY)
            *lo_p = LEATHER_GLOVES, *hi_p = GAUNTLETS_OF_DEXTERITY;
        else if (otyp >= CLOAK_OF_PROTECTION && otyp <= CLOAK_OF_DISPLACEMENT)
            *lo_p = CLOAK_OF_PROTECTION, *hi_p = CLOAK_OF_DISPLACEMENT;
        else if (otyp >= SPEED_BOOTS && otyp <= LEVITATION_BOOTS)
            *lo_p = SPEED_BOOTS, *hi_p = LEVITATION_BOOTS;
        else if (otyp >= MEDIEVAL_ROBE && otyp <= ROBE_OF_STARRY_WISDOM)
            * lo_p = MEDIEVAL_ROBE, * hi_p = ROBE_OF_STARRY_WISDOM;
        else if (otyp >= LEATHER_BRACERS && otyp <= BRACERS_AGAINST_MAGIC_MISSILES)
            * lo_p = LEATHER_BRACERS, * hi_p = BRACERS_AGAINST_MAGIC_MISSILES;
        else if (otyp >= SHIRT_OF_UNCONTROLLABLE_LAUGHTER && otyp <= T_SHIRT)
            * lo_p = SHIRT_OF_UNCONTROLLABLE_LAUGHTER, * hi_p = T_SHIRT;
        break;
    case WEAPON_CLASS:
        if (otyp >= STAFF_OF_THE_MAGI && otyp <= STAFF_OF_WITHERING)
            *lo_p = STAFF_OF_THE_MAGI, * hi_p = STAFF_OF_WITHERING;
        break;
    case TOOL_CLASS:
        if (otyp >= LEATHER_BAG && otyp <= BAG_OF_TRICKS)
            *lo_p = LEATHER_BAG, * hi_p = BAG_OF_TRICKS;
        if (otyp >= TALLOW_CANDLE && otyp <= MAGIC_CANDLE)
            *lo_p = TALLOW_CANDLE, * hi_p = MAGIC_CANDLE;
        if (otyp >= OIL_LAMP && otyp <= MAGIC_LAMP)
            *lo_p = OIL_LAMP, * hi_p = MAGIC_LAMP;
        if (otyp >= TIN_WHISTLE && otyp <= MAGIC_WHISTLE)
            *lo_p = TIN_WHISTLE, * hi_p = MAGIC_WHISTLE;
        if (otyp >= WOODEN_FLUTE && otyp <= MAGIC_FLUTE)
            *lo_p = WOODEN_FLUTE, * hi_p = MAGIC_FLUTE;
        if (otyp >= WOODEN_HARP && otyp <= MAGIC_HARP)
            *lo_p = WOODEN_HARP, * hi_p = MAGIC_HARP;
        if (otyp >= LEATHER_DRUM && otyp <= DRUM_OF_EARTHQUAKE)
            *lo_p = LEATHER_DRUM, * hi_p = DRUM_OF_EARTHQUAKE;
        if (otyp >= JAR_OF_EXTRA_HEALING_SALVE && otyp <= JAR_OF_PRODIGIOUS_HEALING_SALVE)
            *lo_p = JAR_OF_EXTRA_HEALING_SALVE, * hi_p = JAR_OF_PRODIGIOUS_HEALING_SALVE;
        if (otyp >= TOOLED_HORN && otyp <= HORN_OF_PLENTY)
            *lo_p = TOOLED_HORN, * hi_p = HORN_OF_PLENTY;
        break;
    case MISCELLANEOUS_CLASS:
        if (otyp >= IOUN_STONE_OF_PROTECTION && otyp <= IOUN_STONE_OF_SUSTENANCE)
            *lo_p = IOUN_STONE_OF_PROTECTION, * hi_p = IOUN_STONE_OF_SUSTENANCE;
        else if (otyp >= LEATHER_BELT && otyp <= BELT_OF_STORM_GIANT_STRENGTH)
            *lo_p = LEATHER_BELT, * hi_p = BELT_OF_STORM_GIANT_STRENGTH;
        else if (otyp >= NOSE_RING_OF_BULL_STRENGTH && otyp <= NOSE_RING_OF_CEREBRAL_SAFEGUARDING)
            *lo_p = NOSE_RING_OF_BULL_STRENGTH, * hi_p = NOSE_RING_OF_CEREBRAL_SAFEGUARDING;
        else if (otyp >= LENSES && otyp <= EYEGLASSES_OF_SEE_INVISIBLE)
            *lo_p = LENSES, * hi_p = EYEGLASSES_OF_SEE_INVISIBLE;
        else if (otyp >= GOGGLES_OF_NIGHT && otyp <= GOGGLES_OF_EYE_PROTECTION)
            *lo_p = GOGGLES_OF_NIGHT, * hi_p = GOGGLES_OF_EYE_PROTECTION;
        break;
    case FOOD_CLASS:
        if (otyp >= CHAMPIGNON && otyp <= ORACULAR_TOADSTOOL)
            *lo_p = CHAMPIGNON, * hi_p = ORACULAR_TOADSTOOL;
        break;
    case POTION_CLASS:
        /* potion of water has the only fixed description */
        *lo_p = bases[POTION_CLASS];
        *hi_p = POT_WATER - 1;
        break;
    case AMULET_CLASS:
    case SCROLL_CLASS:
    case SPBOOK_CLASS:
        /* exclude non-magic types and also unique ones */
        *lo_p = bases[ocls];
        for (i = *lo_p; objects[i].oc_class == ocls; i++)
            if (is_otyp_unique(i) || !objects[i].oc_magic)
                break;
        *hi_p = i - 1;
        break;
    case RING_CLASS:
        if (otyp >= RIN_ADORNMENT && otyp <= RIN_PROTECTION_FROM_SHAPE_CHANGERS)
            *lo_p = RIN_ADORNMENT, * hi_p = RIN_PROTECTION_FROM_SHAPE_CHANGERS;
        break;
    case WAND_CLASS:
        if (otyp >= WAN_LIGHT && otyp <= LAST_SHUFFLED_WAND)
            *lo_p = WAN_LIGHT, * hi_p = LAST_SHUFFLED_WAND;
        break;
    case REAGENT_CLASS:
    case VENOM_CLASS:
        /* entire class */
        *lo_p = bases[ocls];
        for (i = *lo_p; objects[i].oc_class == ocls; i++)
            continue;
        *hi_p = i - 1;
        break;
    }

    /* artifact checking might ask about item which isn't part of any range
       but fell within the classes that do have ranges specified above */
    if (otyp < *lo_p || otyp > *hi_p)
        *lo_p = *hi_p = otyp;
    return;
}

/* randomize object descriptions */
STATIC_OVL void
shuffle_all()
{
    /* entire classes; obj_shuffle_range() handles their exceptions */
    static const char shuffle_classes[] = {
        AMULET_CLASS, POTION_CLASS, SCROLL_CLASS,
        SPBOOK_CLASS, VENOM_CLASS,  
    };
    /* armor sub-class type ranges (one item from each group) */
    static const short shuffle_types[] = {
        HELMET, LEATHER_GLOVES, SHIRT_OF_UNCONTROLLABLE_LAUGHTER, CLOAK_OF_PROTECTION, SPEED_BOOTS, SWORD_OF_HOLY_VENGEANCE, STAFF_OF_THE_MAGI, LEATHER_BAG,
        TALLOW_CANDLE, OIL_LAMP, TIN_WHISTLE, WOODEN_FLUTE, TOOLED_HORN, WOODEN_HARP, LEATHER_DRUM, JAR_OF_EXTRA_HEALING_SALVE
    };
    static const short shuffle_types_with_material[] = {
         WAN_LIGHT, MEDIEVAL_ROBE, LEATHER_BRACERS, NOSE_RING_OF_BULL_STRENGTH, IOUN_STONE_OF_PROTECTION,
         LENSES, GOGGLES_OF_NIGHT, LEATHER_BELT, ROYAL_CROWN, CORNUTHAUM, CHAMPIGNON, 
         RIN_ADORNMENT
    };
    int first, last, idx;

    /* do whole classes (amulets, &c) */
    for (idx = 0; idx < SIZE(shuffle_classes); idx++) {
        obj_shuffle_range(bases[(int) shuffle_classes[idx]], &first, &last);
        shuffle(first, last, TRUE);
    }
    /* do type ranges (helms, &c) without materials */
    for (idx = 0; idx < SIZE(shuffle_types); idx++) {
        obj_shuffle_range(shuffle_types[idx], &first, &last);
        shuffle(first, last, FALSE);
    }
    /* do type ranges with materials */
    for (idx = 0; idx < SIZE(shuffle_types_with_material); idx++) {
        obj_shuffle_range(shuffle_types_with_material[idx], &first, &last);
        shuffle(first, last, TRUE);
    }
    return;
}

/* find the object index for snow boots; used [once] by slippery ice code */
int
find_skates()
{
    register int i;
    register const char *s;

    for (i = SPEED_BOOTS; i <= LEVITATION_BOOTS; i++)
        if ((s = OBJ_DESCR(objects[i])) != 0 && !strcmp(s, "snow boots"))
            return i;

    impossible("snow boots not found?");
    return -1; /* not 0, or caller would try again each move */
}

/* level dependent initialization */
void
oinit()
{
    //setgemprobs(&u.uz);
}

void
savenames(fd, mode)
int fd, mode;
{
    register int i;
    size_t len;

    if (perform_bwrite(mode)) {
        bwrite(fd, (genericptr_t) bases, sizeof bases);
        bwrite(fd, (genericptr_t) disco, sizeof disco);
        bwrite(fd, (genericptr_t) objects,
               sizeof(struct objclass) * NUM_OBJECTS);
    }
    /* as long as we use only one version of Hack we
       need not save oc_name and oc_descr, but we must save
       oc_uname for all objects */
    for (i = 0; i < NUM_OBJECTS; i++)
    {
        if (objects[i].oc_uname) {
            if (perform_bwrite(mode)) {
                len = strlen(objects[i].oc_uname) + 1;
                bwrite(fd, (genericptr_t) &len, sizeof len);
                bwrite(fd, (genericptr_t) objects[i].oc_uname, len);
            }
            if (release_data(mode)) {
                free((genericptr_t) objects[i].oc_uname);
                objects[i].oc_uname = 0;
            }
        }
    }
}

void
reset_names(VOID_ARGS)
{
    register int i;

    memset((genericptr_t)bases, 0, sizeof bases);
    memset((genericptr_t)disco, 0, sizeof disco);


    for (i = 0; i < NUM_OBJECTS; i++)
    {
        if (objects[i].oc_uname) {
            free((genericptr_t)objects[i].oc_uname);
            objects[i].oc_uname = 0;
        }
    }
    memcpy((genericptr_t)objects, (genericptr_t)saved_objects,
        sizeof(struct objclass) * NUM_OBJECTS);
    memcpy((genericptr_t)obj_descr, (genericptr_t)saved_obj_descr,
        sizeof(struct objdescr) * NUM_OBJECTS);

}


void
restnames(fd)
register int fd;
{
    register int i;
    size_t len;

    mread(fd, (genericptr_t) bases, sizeof bases);
    mread(fd, (genericptr_t) disco, sizeof disco);
    mread(fd, (genericptr_t) objects, sizeof(struct objclass) * NUM_OBJECTS);
    for (i = 0; i < NUM_OBJECTS; i++)
    {
        if (objects[i].oc_uname) {
            mread(fd, (genericptr_t) &len, sizeof len);
            objects[i].oc_uname = (char *) alloc(len);
            mread(fd, (genericptr_t) objects[i].oc_uname, len);
        }
    }
#ifdef USE_TILES
    shuffle_tiles();
#endif
}

void
discover_object(oindx, mark_as_known, credit_hero)
register int oindx;
boolean mark_as_known;
boolean credit_hero;
{
    if (!objects[oindx].oc_name_known) {
        register int dindx, acls = objects[oindx].oc_class;

        /* Loop thru disco[] 'til we find the target (which may have been
           uname'd) or the next open slot; one or the other will be found
           before we reach the next class...
         */
        for (dindx = bases[acls]; disco[dindx] != 0; dindx++)
            if (disco[dindx] == oindx)
                break;
        disco[dindx] = oindx;

        if (mark_as_known) {
            objects[oindx].oc_name_known = 1;
            if (credit_hero)
                exercise(A_WIS, TRUE);
        }
        /* moves==1L => initial inventory, gameover => final disclosure */
        if (moves > 1L && !program_state.gameover) {
            //if (objects[oindx].oc_class == GEM_CLASS)
            gem_learned(oindx); /* could affect price of unpaid gems -- actually of any items in shop --JG */
            update_inventory();
        }
    }
}

/* if a class name has been cleared, we may need to purge it from disco[] */
void
undiscover_object(oindx)
register int oindx;
{
    if (!objects[oindx].oc_name_known) {
        register int dindx, acls = objects[oindx].oc_class;
        register boolean found = FALSE;

        /* find the object; shift those behind it forward one slot */
        for (dindx = bases[acls]; dindx < NUM_OBJECTS && disco[dindx] != 0
                                  && objects[dindx].oc_class == acls;
             dindx++)
            if (found)
                disco[dindx - 1] = disco[dindx];
            else if (disco[dindx] == oindx)
                found = TRUE;

        /* clear last slot */
        if (found)
            disco[dindx - 1] = 0;
        else
            impossible("named object not in disco");

        //if (objects[oindx].oc_class == GEM_CLASS)
        gem_learned(oindx); /* ok, it's actually been unlearned  -- actually of any items in shop --JG */
        update_inventory();
    }
}

STATIC_OVL boolean
interesting_to_discover(i)
register int i;
{
    /* Pre-discovered objects are now printed with a '*' */
    return (boolean) (objects[i].oc_uname != (char *) 0
                      || (objects[i].oc_name_known
                          && OBJ_DESCR(objects[i]) != (char *) 0));
}

/* items that should stand out once they're known */
STATIC_VAR const short uniq_objs[] = {
    AMULET_OF_YENDOR, SPE_BOOK_OF_THE_DEAD, CANDELABRUM_OF_INVOCATION,
    BELL_OF_OPENING,
};

/* the '\' command - show discovered object types */
int
dodiscovered() /* free after Robert Viduya */
{
    register int i, dis;
    int ct = 0;
    char *s, oclass, prev_class, classes[MAX_OBJECT_CLASSES], buf[BUFSZ];
    winid tmpwin;

    tmpwin = create_nhwindow(NHW_MENU);
    putstr(tmpwin, ATR_TITLE, "Discoveries");
    putstr(tmpwin, ATR_HALF_SIZE, " ");

    /* gather "unique objects" into a pseudo-class; note that they'll
       also be displayed individually within their regular class */
    for (i = dis = 0; i < SIZE(uniq_objs); i++)
        if (objects[uniq_objs[i]].oc_name_known) {
            if (!dis++)
                putstr(tmpwin, iflags.menu_headings | ATR_HEADING, "Unique items");
            Sprintf(buf, "  %s", OBJ_NAME(objects[uniq_objs[i]]));
            putstr(tmpwin, 0, buf);
            ++ct;
        }
    /* display any known artifacts as another pseudo-class */
    ct += disp_artifact_discoveries(tmpwin);

    /* several classes are omitted from packorder; one is of interest here */
    Strcpy(classes, flags.inv_order);
    if (!index(classes, VENOM_CLASS))
        (void) strkitten(classes, VENOM_CLASS); /* append char to string */

    for (s = classes; *s; s++) {
        oclass = *s;
        prev_class = oclass + 1; /* forced different from oclass */
        for (i = bases[(int) oclass];
             i < NUM_OBJECTS && objects[i].oc_class == oclass; i++) {
            if ((dis = disco[i]) != 0 && interesting_to_discover(dis)) {
                ct++;
                if (oclass != prev_class) {
                    putstr(tmpwin, iflags.menu_headings | ATR_HEADING,
                           let_to_name(oclass, FALSE, FALSE));
                    prev_class = oclass;
                }
                Sprintf(buf, "%s %s",
                        (objects[dis].oc_pre_discovered ? "*" : " "),
                        obj_typename(dis));
                putstr(tmpwin, objects[dis].oc_pre_discovered ? ATR_INDENT_AT_ASTR : ATR_INDENT_AT_SPACE, buf);
            }
        }
    }
    if (ct == 0) {
        You("haven't discovered anything yet...");
    } else
        display_nhwindow(tmpwin, TRUE);
    destroy_nhwindow(tmpwin);

    return 0;
}

/* lower case let_to_name() output, which differs from def_oc_syms[].name */
STATIC_OVL char *
oclass_to_name(oclass, buf)
char oclass;
char *buf;
{
    char *s;

    Strcpy(buf, let_to_name(oclass, FALSE, FALSE));
    for (s = buf; *s; ++s)
        *s = lowc(*s);
    return buf;
}

/* the '`' command - show discovered object types for one class */
int
doclassdisco()
{
    STATIC_VAR NEARDATA const char
        prompt[] = "View discoveries for which sort of objects?",
        havent_discovered_any[] = "haven't discovered any %s yet.",
        unique_items[] = "unique items",
        artifact_items[] = "artifacts";
    char *s, c, oclass, menulet, allclasses[MAX_OBJECT_CLASSES],
        discosyms[2 + MAX_OBJECT_CLASSES + 1], buf[BUFSZ];
    int i, ct, dis, xtras;
    boolean traditional;
    winid tmpwin = WIN_ERR;
    anything any;
    menu_item *pick_list = 0;

    discosyms[0] = '\0';
    traditional = (flags.menu_style == MENU_TRADITIONAL
                   || flags.menu_style == MENU_COMBINATION);
    if (!traditional) {
        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
    }
    any = zeroany;
    menulet = 'a';

    /* check whether we've discovered any unique objects */
    for (i = 0; i < SIZE(uniq_objs); i++)
        if (objects[uniq_objs[i]].oc_name_known) {
            Strcat(discosyms, "u");
            if (!traditional) {
                any.a_int = 'u';
                add_menu(tmpwin, NO_GLYPH, &any, menulet++, 0, ATR_NONE,
                         unique_items, MENU_UNSELECTED);
            }
            break;
        }

    /* check whether we've discovered any artifacts */
    if (disp_artifact_discoveries(WIN_ERR) > 0) {
        Strcat(discosyms, "a");
        if (!traditional) {
            any.a_int = 'a';
            add_menu(tmpwin, NO_GLYPH, &any, menulet++, 0, ATR_NONE,
                     artifact_items, MENU_UNSELECTED);
        }
    }

    /* collect classes with discoveries, in packorder ordering; several
       classes are omitted from packorder and one is of interest here */
    Strcpy(allclasses, flags.inv_order);
    if (!index(allclasses, VENOM_CLASS))
        (void) strkitten(allclasses, VENOM_CLASS); /* append char to string */
    /* construct discosyms[] */
    for (s = allclasses; *s; ++s) {
        oclass = *s;
        c = def_oc_syms[(int) oclass].sym;
        for (i = bases[(int) oclass];
             i < NUM_OBJECTS && objects[i].oc_class == oclass; ++i)
            if ((dis = disco[i]) != 0 && interesting_to_discover(dis)) {
                if (!index(discosyms, c)) {
                    Sprintf(eos(discosyms), "%c", c);
                    if (!traditional) {
                        any.a_int = c;
                        add_menu(tmpwin, NO_GLYPH, &any, menulet++, c,
                                 ATR_NONE, oclass_to_name(oclass, buf),
                                 MENU_UNSELECTED);
                    }
                }
            }
    }

    /* there might not be anything for us to do... */
    if (!discosyms[0]) {
        You(havent_discovered_any, "items");
        if (tmpwin != WIN_ERR)
            destroy_nhwindow(tmpwin);
        return 0;
    }

    /* have player choose a class */
    c = '\0'; /* class not chosen yet */
    if (traditional) {
        /* we'll prompt even if there's only one viable class; we add all
           nonviable classes as unseen acceptable choices so player can ask
           for discoveries of any class whether it has discoveries or not */
        for (s = allclasses, xtras = 0; *s; ++s) {
            c = def_oc_syms[(int) *s].sym;
            if (!index(discosyms, c)) {
                if (!xtras++)
                    (void) strkitten(discosyms, '\033');
                (void) strkitten(discosyms, c);
            }
        }
        /* get the class (via its symbol character) */
        c = yn_function(prompt, discosyms, '\0', (char*)0);
        savech(c);
        if (!c)
            clear_nhwindow(WIN_MESSAGE);
    } else {
        /* menustyle:full or menustyle:partial */
        if (!discosyms[1] && flags.menu_style == MENU_PARTIAL) {
            /* only one class; menustyle:partial normally jumps past class
               filtering straight to final menu so skip class filter here */
            c = discosyms[0];
        } else {
            /* more than one choice, or menustyle:full which normally has
               an intermediate class selection menu before the final menu */
            end_menu(tmpwin, prompt);
            i = select_menu(tmpwin, PICK_ONE, &pick_list);
            if (i > 0) {
                c = pick_list[0].item.a_int;
                free((genericptr_t) pick_list);
            } /* else c stays 0 */
        }
        destroy_nhwindow(tmpwin);
    }
    if (!c)
        return 0; /* player declined to make a selection */

    /*
     * show discoveries for object class c
     */
    tmpwin = create_nhwindow(NHW_MENU);
    ct = 0;
    switch (c) {
    case 'u':
        putstr(tmpwin, iflags.menu_headings,
               upstart(strcpy(buf, unique_items)));
        for (i = 0; i < SIZE(uniq_objs); i++)
            if (objects[uniq_objs[i]].oc_name_known) {
                Sprintf(buf, "  %s", OBJ_NAME(objects[uniq_objs[i]]));
                putstr(tmpwin, 0, buf);
                ++ct;
            }
        if (!ct)
            You(havent_discovered_any, unique_items);
        break;
    case 'a':
        /* disp_artifact_discoveries() includes a header */
        ct = disp_artifact_discoveries(tmpwin);
        if (!ct)
            You(havent_discovered_any, artifact_items);
        break;
    default:
        oclass = def_char_to_objclass(c);
        Sprintf(buf, "Discovered %s", let_to_name(oclass, FALSE, FALSE));
        putstr(tmpwin, iflags.menu_headings, buf);
        for (i = bases[(int) oclass];
             i < NUM_OBJECTS && objects[i].oc_class == oclass; ++i) {
            if ((dis = disco[i]) != 0 && interesting_to_discover(dis)) {
                Sprintf(buf, "%s %s",
                        objects[dis].oc_pre_discovered ? "*" : " ",
                        obj_typename(dis));
                putstr(tmpwin, 0, buf);
                ++ct;
            }
        }
        if (!ct)
            You(havent_discovered_any, oclass_to_name(oclass, buf));
        break;
    }
    if (ct)
        display_nhwindow(tmpwin, TRUE);
    destroy_nhwindow(tmpwin);
    return 0;
}

/* put up nameable subset of discoveries list as a menu */
void
rename_disco()
{
    register int i, dis;
    int ct = 0, mn = 0, sl;
    char *s, oclass, prev_class;
    winid tmpwin;
    anything any;
    menu_item *selected = 0;

    any = zeroany;
    tmpwin = create_nhwindow(NHW_MENU);
    start_menu(tmpwin);

    /*
     * Skip the "unique objects" section (each will appear within its
     * regular class if it is nameable) and the artifacts section.
     * We assume that classes omitted from packorder aren't nameable
     * so we skip venom too.
     */

    /* for each class, show discoveries in that class */
    for (s = flags.inv_order; *s; s++) {
        oclass = *s;
        prev_class = oclass + 1; /* forced different from oclass */
        for (i = bases[(int) oclass];
             i < NUM_OBJECTS && objects[i].oc_class == oclass; i++) {
            dis = disco[i];
            if (!dis || !interesting_to_discover(dis))
                continue;
            ct++;
            if (!objtyp_is_callable(dis))
                continue;
            mn++;

            if (oclass != prev_class) {
                any.a_int = 0;
                add_extended_menu(tmpwin, NO_GLYPH, &any, menu_heading_info(), 0, 0, iflags.menu_headings,
                         let_to_name(oclass, FALSE, FALSE),
                         MENU_UNSELECTED);
                prev_class = oclass;
            }
            any.a_int = dis;
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE,
                     obj_typename(dis), MENU_UNSELECTED);
        }
    }
    if (ct == 0) {
        You("haven't discovered anything yet...");
    } else if (mn == 0) {
        pline("None of your discoveries can be assigned names...");
    } else {
        end_menu(tmpwin, "Pick an object type to name");
        dis = STRANGE_OBJECT;
        sl = select_menu(tmpwin, PICK_ONE, &selected);
        if (sl > 0) {
            dis = selected[0].item.a_int;
            free((genericptr_t) selected);
        }
        if (dis != STRANGE_OBJECT) {
            struct obj odummy;

            odummy = zeroobj;
            odummy.otyp = dis;
            odummy.oclass = objects[dis].oc_class;
            odummy.quan = 1L;
            odummy.known = !objects[dis].oc_uses_known;
            odummy.dknown = 1;
            docall(&odummy, (char*)0);
        }
    }
    destroy_nhwindow(tmpwin);
    return;
}

void
randomize_mythic_quality(obj, is_wish, prefix_ptr, suffix_ptr)
struct obj* obj;
uchar is_wish; /* 1 = mythic wishing, 2 = legendary wishing */
uchar *prefix_ptr, *suffix_ptr;
{
    if (!obj || !prefix_ptr || !suffix_ptr)
        return;

    *prefix_ptr = 0;
    *suffix_ptr = 0;

    if (!can_obj_have_mythic(obj))
        return;

    uchar eligible_prefix[MAX_MYTHIC_PREFIXES] = { 0 };
    uchar eligible_suffix[MAX_MYTHIC_SUFFIXES] = { 0 };

    uchar start = 1;
    uchar end = 1;
    xchar levdif = level_difficulty();
    if (is_wish == 2 || 
        (is_wish == 0 && 
         (
             (levdif < 16 && levdif >= 8 && !rn2(40))
          || (levdif < depth(&medusa_level) && levdif >= 16 && !rn2(10))
          || (levdif < depth(&stronghold_level) && levdif >= depth(&medusa_level) && !rn2(5))
          || (Inhell && !rn2(4))
          || (In_endgame(&u.uz) && !rn2(3))
          || (levdif > depth(&stronghold_level) && !In_endgame(&u.uz) && !Inhell && !rn2(4))
         )
        )
       )
    {
        start = 0;
        end = 1;
    }
    else
    {
        int ok_cnt[2] = { 0, 0 };
        for (int j = 0; j <= 1; j++)
        {
            uchar max_mythic = (j == 0 ? MAX_MYTHIC_PREFIXES : MAX_MYTHIC_SUFFIXES);
            for (uchar i = 1; i < max_mythic; i++)
            {
                if (!is_mythic_affix_ok(j, i, obj, is_wish))
                    continue;

                ok_cnt[j]++;
            }
        }

        if (ok_cnt[0] == 0 && ok_cnt[1] == 0)
        {
            start = 0;
            end = 1;
        }
        else if (ok_cnt[0] == 0)
        {
            start = 1;
            end = 1;
        }
        else if (ok_cnt[1] == 0)
        {
            start = 0;
            end = 0;
        }
        else if (rn2(ok_cnt[0] + ok_cnt[1]) < ok_cnt[0])
        {
            start = 0;
            end = 0;
        }
        else
        {
            start = 1;
            end = 1;
        }
    }

    for (uchar j = start; j <= end; j++)
    {
        struct mythic_definition* mythic_definitions = (j == 0 ? mythic_prefix_qualities : mythic_suffix_qualities);
        uchar* eligible = (j == 0 ? eligible_prefix : eligible_suffix);
        uchar max_mythic = (j == 0 ? MAX_MYTHIC_PREFIXES : MAX_MYTHIC_SUFFIXES);
        uchar* affix_ptr = (j == 0 ? prefix_ptr : suffix_ptr);

        int cnt = 0;
        int total_prob = 0;
        uchar i;
        for (i = 1; i < max_mythic; i++)
        {
            eligible[i] = FALSE;
            if(!is_mythic_affix_ok(j, i, obj, is_wish))
                continue;

            eligible[i] = TRUE;
            cnt++;
            total_prob += (int)mythic_definitions[i].probability;
        }

        if (cnt == 0 || total_prob == 0)
        {
            *affix_ptr = 0;
            continue;
        }

        int roll = total_prob > 1 ? rn2(total_prob) : 0;
        for (i = 1; i < max_mythic; i++)
        {
            if (!eligible[i])
                continue;
            roll -= (int)mythic_definitions[i].probability;
            if (roll <= 0)
            {
                *affix_ptr = i;
                break;
            }
        }
    }
}

boolean
is_mythic_affix_ok(affix_type, affix_idx, obj, is_wish)
uchar affix_type; /* 0 = prefix, 1 = suffix  */
uchar affix_idx;
struct obj* obj;
uchar is_wish; /* 1 = mythic wishing, 2 = legendary wishing */
{
    struct mythic_definition* mythic_definitions = (affix_type == 0 ? mythic_prefix_qualities : mythic_suffix_qualities);

    if (!obj)
        return FALSE;
    if(!can_obj_have_mythic(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NON_WISHABLE) && is_wish)
        return FALSE;
    /* Deactivated until there are more valid prefixes for racial items, particularly armors */
    //if (affix_type == 0 && (mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_RACIAL_PREFIX) && (objects[(obj)->otyp].oc_flags5 & O5_NO_MYTHIC_RACIAL_PREFIXES))
    //    return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_WEAPON_REQUIRED) && !is_weapon(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_ARMOR_REQUIRED) && obj->oclass != ARMOR_CLASS)
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_HELMET_REQUIRED) && (obj->oclass != ARMOR_CLASS || objects[obj->otyp].oc_armor_category != ARM_HELM))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_SHIELD_REQUIRED) && (obj->oclass != ARMOR_CLASS || objects[obj->otyp].oc_armor_category != ARM_SHIELD))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_SUIT_REQUIRED) && (obj->oclass != ARMOR_CLASS || objects[obj->otyp].oc_armor_category != ARM_SUIT))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_BLUDGEONING_WEAPONS) && (!is_weapon(obj) || (is_weapon(obj) && objects[obj->otyp].oc_dir == WHACK)))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_PIERCING_WEAPONS) && (!is_weapon(obj) || (is_weapon(obj) && objects[obj->otyp].oc_dir == PIERCE)))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_SLASHING_WEAPONS) && (!is_weapon(obj) || (is_weapon(obj) && objects[obj->otyp].oc_dir == SLASH)))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_CELESTIAL_WEAPONS) && obj->exceptionality == EXCEPTIONALITY_CELESTIAL)
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_PRIMORDIAL_WEAPONS) && obj->exceptionality == EXCEPTIONALITY_PRIMORDIAL)
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_INFERNAL_WEAPONS) && obj->exceptionality == EXCEPTIONALITY_INFERNAL)
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_POLEARM_LANCE_SPEAR_ONLY) && !(is_pole(obj) || is_lance(obj) || is_spear(obj)))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_WEAPON) && is_weapon(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_ARMOR) && obj->oclass == ARMOR_CLASS)
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_THROWN_OR_AMMO) && (is_missile(obj) || is_ammo(obj)))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_THROWN_WEAPON_ONLY) && !is_missile(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_RETURNING_WEAPONS) && ((objects[(obj)->otyp].oc_flags4 & O4_TETHERED_WEAPON) != 0 || (objects[(obj)->otyp].oc_flags & O1_RETURNS_TO_HAND_AFTER_THROWING) != 0))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_LOW_SPELLCASTING_PENALTY_ITEMS) && objects[(obj)->otyp].oc_spell_casting_penalty < 1)
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_ORCISH_ITEMS) && is_orcish_obj(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_ELVEN_ITEMS) && is_elven_obj(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_DEMONIC_ITEMS) && is_demon_obj(obj))
        return FALSE;
    if ((mythic_definitions[affix_idx].mythic_flags & MYTHIC_FLAG_NO_OTHER_SORCERY) && (affix_type == 0 ? (obj->mythic_suffix == MYTHIC_SUFFIX_SORCERY) : (obj->mythic_prefix == MYTHIC_PREFIX_SORCERERS)))
        return FALSE;

    return TRUE;
}

double
get_mythic_dmg_multiplier(otmp, mon, mattacker)
struct obj* otmp;
struct monst* mon;
struct monst* mattacker UNUSED;
{
    if (!otmp || !mon || (otmp->mythic_prefix == 0 && otmp->mythic_suffix == 0))
        return 1.0;

    double multiplier = 1.0;
    for (uchar j = 0; j <= 1; j++)
    {
        uchar mythic_quality = (j == 0 ? otmp->mythic_prefix : otmp->mythic_suffix);
        if (mythic_quality == 0)
            continue;

        struct mythic_power_definition* mythic_powers = (j == 0 ? mythic_prefix_powers : mythic_suffix_powers);
        struct mythic_definition* mythic_definitions = (j == 0 ? mythic_prefix_qualities : mythic_suffix_qualities);
        uchar max_mythic_powers = (j == 0 ? MAX_MYTHIC_PREFIX_POWERS : MAX_MYTHIC_SUFFIX_POWERS);

        for (uchar i = 0; i < max_mythic_powers; i++)
        {
            if (!mythic_powers[i].name)
                break;

            unsigned long mythic_power_bit = 1UL << ((unsigned long)i);

            if ((mythic_definitions[mythic_quality].mythic_powers & mythic_power_bit) && mythic_power_applies_to_obj(otmp, mythic_powers[i].power_flags))
            {
                if (mythic_powers[i].power_type == MYTHIC_POWER_TYPE_SLAYING && 
                    (mythic_powers[i].parameter3 == mon->data->mlet 
                        || (mythic_powers[i].parameter4 & mon->data->mflags2) 
                        || ((mythic_powers[i].power_flags & MYTHIC_POWER_FLAG_ALSO_SHAPESHIFTERS) && mon->cham >= LOW_PM && ((mythic_powers[i].parameter3 == mons[mon->cham].mlet || (mythic_powers[i].parameter4 & mons[mon->cham].mflags2))))
                    )
                   )
                {
                    double pmult = mythic_powers[i].parameter2;
                    if (pmult > 1.0)
                    {
                        multiplier += pmult - 1.0;
                    }
                }
            }
        }
    }

    return multiplier;
}

int
get_obj_exceptionality_ac_bonus(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    if (obj->exceptionality > 0 && (is_armor(obj) || (objects[obj->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || has_obj_mythic_defense(obj)))
    {
        int multiplier = has_obj_mythic_defense(obj) ? 4 : is_suit(obj) ? 4 : is_shield(obj) ? 3 : 2;
        if (obj->exceptionality > EXCEPTIONALITY_ELITE)
            return 3 * multiplier;
        else
            return multiplier * obj->exceptionality;
    }
    return 0;
}

int
get_obj_exceptionality_mc_bonus(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    return get_obj_exceptionality_ac_bonus(obj) / 3;
}


STATIC_VAR boolean object_init_values_saved = FALSE;

void
save_initial_objects_values(VOID_ARGS)
{
    if (!object_init_values_saved)
    {
        memcpy((genericptr_t)saved_objects, (genericptr_t)objects,
            sizeof(struct objclass) * NUM_OBJECTS);
        memcpy((genericptr_t)saved_obj_descr, (genericptr_t)obj_descr,
            sizeof(struct objdescr) * NUM_OBJECTS);
        object_init_values_saved = TRUE;
    }

}





/*o_init.c*/

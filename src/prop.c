/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0 prop.c */
/* Copyright (c) Janne Gustafsson, 2021.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#define PCLR_BLACK {0, 0, 0}
#define PCLR_NONE  {0, 0, 0}
#define PCLR_WHITE {255, 255, 255}

NEARDATA struct prop_info property_definitions[MAX_PROPS] =
{
    { "",                             0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* = 0 */
    { "fire-immunity",                1, 1, 0,  0, 0,  PCLR_BLACK,  { 255, 255, 192 }, PROPFLAGS_BUFF_CANCELLABLE}, /* FIRE_IMMUNITY = 1 */
    { "cold-immunity",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* COLD_IMMUNITY = 2 */
    { "sleep-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SLEEP_RESISTANCE = 3 */
    { "disintegration-resistance",    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DISINTEGRATION_RESISTANCE = 4 */
    { "shock-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SHOCK_IMMUNITY = 5 */
    { "poison-resistance",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* POISON_RESISTANCE = 6 */
    { "acid-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ACID_IMMUNITY = 7 */
    { "stone-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* STONE_RESISTANCE = 8 */
    { "drain-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DRAIN_RESISTANCE = 9 */
    { "sick-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SICK_RESISTANCE = 10 */
    { "invulnerable",                 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* INVULNERABLE = 11 */
    { "antimagic",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ANTIMAGIC = 12 */
    { "stunned",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* STUNNED = 13 */
    { "confusion",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CONFUSION = 14 */
    { "blinded",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLINDED = 15 */
    { "deaf",                         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* DEAF = 16 */
    { "sick",                         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SICK = 17 */
    { "stoned",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* STONED = 18 */
    { "strangled",                    0, 0, 1,  6, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* STRANGLED = 19 */
    { "vomiting",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* VOMITING = 20 */
    { "glib",                         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* GLIB = 21 */
    { "slimed",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SLIMED = 22 */
    { "hallucination",                0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* HALLUC = 23 */
    { "hallucination-resistance",     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALLUC_RES = 24 */
    { "fumbling",                     0, 0, 1,  0, 20,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FUMBLING = 25 */
    { "wounded-legs",                 0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* WOUNDED_LEGS = 26 */
    { "sleepy",                       0, 0, 1,  0, 100,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SLEEPY = 27 */
    { "hunger",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* HUNGER = 28 */
    { "see-invisible",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SEE_INVISIBLE = 29 */
    { "telepathy",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TELEPAT = 30 */ 
    { "warning",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARNING = 31 */
    { "warn-of-mon",                  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_OF_MON = 32 */
    { "warn-undead",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_UNDEAD = 33 */
    { "searching",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SEARCHING = 34 */
    { "clairvoyant",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CLAIRVOYANT = 35 */
    { "infravision",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* INFRAVISION = 36 */
    { "detect-monsters",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DETECT_MONSTERS = 37 */
    { "adorned",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* ADORNED = 38 */
    { "invisibility",                 0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* INVISIBILITY = 39 */
    { "displaced",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DISPLACED = 40 */
    { "stealth",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* STEALTH = 41 */
    { "aggravate-monster",            0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* AGGRAVATE_MONSTER = 42 */
    { "conflict",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CONFLICT = 43 */
    { "jumping",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* JUMPING = 44 */
    { "teleport",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TELEPORT = 45 */
    { "teleport-control",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TELEPORT_CONTROL = 46 */
    { "levitation",                   0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LEVITATION = 47 */
    { "flying",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FLYING = 48 */
    { "water-walking",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WATER_WALKING = 49 */
    { "swimming",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SWIMMING = 50 */
    { "magical-breathing",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_BREATHING = 51 */
    { "passes-walls",                 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* PASSES_WALLS = 52 */
    { "slow-digestion",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SLOW_DIGESTION = 53 */
    { "half-spell-damage",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_SPELL_DAMAGE = 54 */
    { "half-physical-damage",         1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_PHYSICAL_DAMAGE = 55 */
    { "regeneration",                 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* REGENERATION = 56 */
    { "energy-regeneration",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ENERGY_REGENERATION = 57 */
    { "magical-protection",           1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_PROTECTION = 58 */
    { "prot-from-shapechangers",      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* PROT_FROM_SHAPE_CHANGERS = 59 */
    { "polymorph",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* POLYMORPH = 60 */
    { "polymorph-control",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* POLYMORPH_CONTROL = 61 */
    { "unchanging",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* UNCHANGING = 62 */
    { "fast",                         1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FAST = 63 */
    { "reflecting",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* REFLECTING = 64 */
    { "free-action",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FREE_ACTION = 65 */
    { "fixed-ability",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FIXED_ABIL = 66 */
    { "life-saved",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LIFESAVED = 67 */
    { "death-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DEATH_RESISTANCE = 68 */
    { "lycanthropy-resistance",       1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LYCANTHROPY_RESISTANCE = 69 */
    { "curse-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CURSE_RESISTANCE = 70 */
    { "blind-telepathy",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* BLIND_TELEPATHY = 71 */
    { "enhanced-vision",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ENHANCED_VISION = 72 */
    { "flash-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FLASH_RESISTANCE = 73 */
    { "eye-protection",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* EYE_PROTECTION = 74 */
    { "brain-protection",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* BRAIN_PROTECTION = 75 */
    { "warn-demon",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_DEMON = 76 */
    { "warn-orc",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_ORC = 77 */
    { "warn-troll",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_TROLL = 78 */
    { "warn-giant",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_GIANT = 79 */
    { "warn-dragon",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_DRAGON = 80 */
    { "warn-elf",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_ELF = 81 */
    { "warn-dwarf",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_DWARF = 82 */
    { "warn-gnoll",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_GNOLL = 83 */
    { "warn-human",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_HUMAN = 84 */
    { "warn-lycanthrope",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_LYCANTHROPE = 85 */
    { "warn-angel",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_ANGEL = 86 */
    { "warn-ogre",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_OGRE = 87 */
    { "warn-gnome",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_GNOME = 88 */
    { "charm-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CHARM_RESISTANCE = 89 */
    { "mind-shielding",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MIND_SHIELDING = 90 */
    { "odd-ideas",                    0, 0, 1,  100, 150,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ODD_IDEAS = 91 */
    { "airless-environment",          0, 0, 1,  6, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* AIRLESS_ENVIRONMENT = 92 */
    { "magical-kicking",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_KICKING = 93 */
    { "blocks-invisibility",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_INVISIBILITY = 94 */
    { "blocks-blindness",             0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_BLINDNESS = 95 */
    { "blocks-clairvoyance",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_CLAIRVOYANCE = 96 */
    { "half-physical-damage-against-undead-and-demons",  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS = 97 */
    { "magical-shielding",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_SHIELDING = 98 */
    { "magical-barkskin",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_BARKSKIN = 99 */
    { "magical-stoneskin",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_STONESKIN = 100 */
    { "xray-vision",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* XRAY_VISION = 101 */
    { "very-fast",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* VERY_FAST = 102 */
    { "slowed",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SLOWED = 103 */
    { "cancelled",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CANCELLED = 104 */
    { "halved-magic-resistance",      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALVED_MAGIC_RESISTANCE = 105 */
    { "no-magic-resistance",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* NO_MAGIC_RESISTANCE = 106 */
    { "charmed",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CHARMED = 107 */
    { "paralyzed",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* PARALYZED = 108 */
    { "fearful",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FEARFUL = 109 */
    { "fear-resistance",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FEAR_RESISTANCE = 110 */
    { "sleeping",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SLEEPING = 111 */
    { "summon-forbidden",             0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SUMMON_FORBIDDEN = 112 */
    { "silenced",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SILENCED = 113 */
    { "levitation-control",           1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LEVITATION_CONTROL = 114 */
    { "fire-vulnerability",           0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FIRE_VULNERABILITY = 115 */
    { "cold-vulnerability",           0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* COLD_VULNERABILITY = 116 */
    { "shock-vulnerability",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SHOCK_VULNERABILITY = 117 */
    { "magic-missile-vulnerability",  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGIC_MISSILE_VULNERABILITY = 118 */
    { "double-physical-damage",       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DOUBLE_PHYSICAL_DAMAGE = 119 */
    { "double-spell-damage",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DOUBLE_SPELL_DAMAGE = 120 */
    { "enhanced-untrap",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ENHANCED_UNTRAP = 121 */
    { "blocks-levitation",            0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_LEVITATION = 122 */
    { "blocks-flying",                0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_FLYING = 123 */
    { "one-fourth-magic-resistance",  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ONE_FOURTH_MAGIC_RESISTANCE = 124 */
    { "three-fourths-magic-resistance", 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* THREE_FOURTHS_MAGIC_RESISTANCE = 125 */
    { "blindfolded",                  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLINDFOLDED = 126 */
    { "tital-strength",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TITAN_STRENGTH = 127 */
    { "magic-missile-immunity",       1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGIC_MISSILE_IMMUNITY = 128 */
    { "stun-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* STUN_RESISTANCE = 129 */
    { "food-poisoned",                0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FOOD_POISONED = 130 */
    { "bisection-resistance",         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* BISECTION_RESISTANCE = 131 */
    { "divine-endurance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_ENDURANCE = 132 */
    { "divine-intellect",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_INTELLECT = 133 */
    { "divine-wisdon",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_WISDOM = 134 */
    { "divine-dexterity",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_DEXTERITY = 135 */
    { "divine-charisma",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_CHARISMA = 136 */
    { "mummy-rot",                    0, 0, 1, 50, 50,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MUMMY_ROT = 137 */
    { "fire-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FIRE_RESISTANCE = 138 */
    { "cold-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* COLD_RESISTANCE = 139 */
    { "shock-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SHOCK_RESISTANCE = 140 */
    { "magic-missile-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGIC_MISSILE_RESISTANCE = 141 */
    { "acid-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ACID_RESISTANCE = 142 */
    { "ultra-fast",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ULTRA_FAST = 143 */
    { "super-fast",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SUPER_FAST = 144 */
    { "lightning-fast",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LIGHTNING_FAST = 145 */
    { "heroism",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HEROISM = 146 */
    { "super-heroism",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SUPER_HEROISM = 147 */
    { "rapid-regeneration",           1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPID_REGENERATION = 148 */
    { "rapider-regeneration",         1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDER_REGENERATION = 149 */
    { "rapidest-regeneration",        1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDEST_REGENERATION = 150 */
    { "rapid-energy-regeneration",    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPID_ENERGY_REGENERATION = 151 */
    { "rapider-energy-regeneration",  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDER_ENERGY_REGENERATION = 152 */
    { "rapidest-energy-regeneration", 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDEST_ENERGY_REGENERATION = 153 */
    { "melee-life-leech",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MELEE_LIFE_LEECH = 154 */
    { "crazed",                       1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CRAZED = 155 */
    { "divine-regeneration",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_REGENERATION = 156 */
    { "improved-fire-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_FIRE_RESISTANCE = 157 */
    { "improved-cold-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_COLD_RESISTANCE = 158 */
    { "improved-shock-resistance",    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_SHOCK_RESISTANCE = 159 */
    { "improved-magic-missile-resistance", 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_MAGIC_MISSILE_RESISTANCE = 160 */
    { "improved-acid-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_ACID_RESISTANCE = 161 */
    { "undead-immobility",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* UNDEAD_IMMOBILITY = 162 */
    { "undead-control",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* UNDEAD_CONTROL = 163 */
    { "cancellation-resistance",      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CANCELLATION_RESISTANCE = 164 */
    { "half-slow-digestion",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_SLOW_DIGESTION = 165 */
    { "laughing",                     0, 0, 1,  0, 20,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* LAUGHING = 166 */
};

void
props_init()
{
    /* Force linkage */
    return;
}

/* prop.c */

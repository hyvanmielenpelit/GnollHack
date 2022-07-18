/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    role.c    $NHDT-Date: 1547086250 2019/01/10 02:10:50 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.56 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985-1999. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*** Table of all roles ***/
/* According to AD&D, HD for some classes (ex. Wizard) should be smaller
 * (4-sided for wizards).  But this is not AD&D, and using the AD&D
 * rule here produces an unplayable character.  Thus I have used a minimum
 * of an 10-sided hit die for everything.  Another AD&D change: wizards get
 * a minimum strength of 4 since without one you can't teleport or cast
 * spells. --KAA
 *
 * As the wizard has been updated (wizard patch 5 jun '96) their HD can be
 * brought closer into line with AD&D. This forces wizards to use magic more
 * and distance themselves from their attackers. --LSZ
 *
 * With the introduction of races, some hit points and energy
 * has been reallocated for each race.  The values assigned
 * to the roles has been reduced by the amount allocated to
 * humans.  --KMH
 *
 * God names use a leading underscore to flag goddesses.
 */
#define SKILL_PROGRESS_INI_UNSKILLED  {0, 0, 3, 7, 13, 21, 31}
#define SKILL_PROGRESS_INI_BASIC      {0, 0, 0, 4,  9, 16, 25}
#define SKILL_PROGRESS_INI_SKILLED    {0, 0, 0, 0,  5, 11, 19}
#define SKILL_PROGRESS_NONE           {0, 0, 0, 0,  0,  0,  0}

const struct Role roles[] = {
    { { "Archaeologist", 0 },
      { { "Digger", 0 },
        { "Field Worker", 0 },
        { "Investigator", 0 },
        { "Exhumer", 0 },
        { "Excavator", 0 },
        { "Spelunker", 0 },
        { "Speleologist", 0 },
        { "Collector", 0 },
        { "Curator", 0 } },
      "Quetzalcoatl", "Camaxtli", "Huhetotl", /* Central American */
      "Arc",
      "the College of Archaeology",
      "the Tomb of the Toltec Kings",
      0,
      { "", "", "", "", ""},
      PM_ARCHAEOLOGIST,
      NON_PM,
      PM_LORD_CARNARVON,
      PM_STUDENT,
      PM_MINION_OF_HUHETOTL,
      NON_PM,
      PM_HUMAN_MUMMY,
      S_SNAKE,
      S_GREATER_UNDEAD,
      ART_ORB_OF_DETECTION,
      MH_HUMAN | MH_DWARF | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL
          | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      {  7, 10, 10, 13,  7,  7 },
      { 20, 20, 20, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 11, 0, 0, 8, 1, 0 }, /* Hit points */
      { 2, 2, 2, 2, 2, 2 }, /* Energy 1d3+1*/
      MAXULEV,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_INI_UNSKILLED, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_NONE, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */
        }
    
    },
    { { "Barbarian", 0 },
      { { "Plunderer", "Plunderess" },
        { "Pillager", 0 },
        { "Bandit", 0 },
        { "Brigand", 0 },
        { "Raider", 0 },
        { "Reaver", 0 },
        { "Slayer", 0 },
        { "Chieftain", "Chieftainess" },
        { "Conqueror", "Conqueress" } },
      "Mitra", "Crom", "Set", /* Hyborian */
      "Bar",
      "the Camp of the Duali Tribe",
      "the Duali Oasis",
      1,
      { "", "", "", "", ""},
      PM_BARBARIAN,
      NON_PM,
      PM_PELIAS,
      PM_CHIEFTAIN,
      PM_THOTH_AMON,
      PM_OGRE,
      PM_TROLL,
      S_OGRE,
      S_TROLL,
      ART_HEART_OF_AHRIMAN,
      MH_HUMAN | MH_ORC | MH_GNOLL | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL
          | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 16, 7, 7, 15, 16, 6 },
      { 30, 6, 7, 20, 30, 7 },
      /* Init   Lower  Higher */
      { 17, 0, 0, 12, 3, 0 }, /* Hit points */
      { 1, 2, 1, 2, 1, 2 }, /* Energy 1d3*/
      MAXULEV,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_NONE, /* P_HEALING_SPELL */
            SKILL_PROGRESS_NONE, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Caveman", "Cavewoman" },
      { { "Troglodyte", 0 },
        { "Aborigine", 0 },
        { "Wanderer", 0 },
        { "Vagrant", 0 },
        { "Wayfarer", 0 },
        { "Roamer", 0 },
        { "Nomad", 0 },
        { "Rover", 0 },
        { "Pioneer", 0 } },
      "Anu", "_Ishtar", "Anshar", /* Babylonian */
      "Cav",
      "the Caves of the Ancestors",
      "the Dragon's Lair",
      2,
      { "Can practice cannibalism", "", "", "", ""},
      PM_CAVEMAN,
      PM_LITTLE_DOG,
      PM_SHAMAN_KARNOV,
      PM_NEANDERTHAL,
      PM_TIAMAT,
      PM_BUGBEAR,
      PM_HILL_GIANT,
      S_HUMANOID,
      S_GIANT,
      ART_SCEPTRE_OF_MIGHT,
      MH_HUMAN | MH_DWARF | MH_GNOLL | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL
          | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 10, 7,  9,  7,  8, 6 },
      { 30, 6, 20, 20, 30, 7 },
      /* Init   Lower  Higher */
      { 16, 0, 0, 12, 2, 1 }, /* Hit points */
      { 1, 1, 0, 2, 0, 2 }, /* Energy 1d3-1*/
      MAXULEV,
      0,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_NONE, /* P_HEALING_SPELL */
            SKILL_PROGRESS_NONE, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Healer", 0 },
      { { "Rhizotomist", 0 },
        { "Empiric", 0 },
        { "Embalmer", 0 },
        { "Dresser", 0 },
        { "Medicus ossium", "Medica ossium" },
        { "Herbalist", 0 },
        { "Magister", "Magistra" },
        { "Physician", 0 },
        { "Chirurgeon", 0 } },
      "_Athena", "Hermes", "Poseidon", /* Greek */
      "Hea",
      "the Temple of Epidaurus",
      "the Temple of Coeus",
      3,
      { "", "", "", "", ""},
      PM_HEALER,
      NON_PM,
      PM_HIPPOCRATES,
      PM_ATTENDANT,
      PM_CYCLOPS,
      PM_GIANT_RAT,
      PM_SNAKE,
      S_RODENT,
      S_YETI,
      ART_STAFF_OF_AESCULAPIUS,
      MH_HUMAN | MH_GNOLL | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      {  7,  13, 13, 10, 11, 16 },
      { 15, 20, 20, 15, 25, 5 },
      /* Init   Lower  Higher */
      { 11, 0, 0, 7, 1, 0 }, /* Hit points */
      { 5, 2, 4, 3, 4, 3 }, /* Energy 1d4+3*/
      MAXULEV,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_SKILLED, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_NONE, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Knight", 0 },
      { { "Gallant", 0 },
        { "Esquire", 0 },
        { "Bachelor", 0 },
        { "Sergeant", 0 },
        { "Knight", 0 },
        { "Banneret", 0 },
        { "Chevalier", "Chevaliere" },
        { "Seignieur", "Dame" },
        { "Paladin", 0 } },
      "Lugh", "_Brigit", "Manannan Mac Lir", /* Celtic */
      "Kni",
      "Camelot Castle",
      "the Isle of Glass",
      4,
      { "Alignment penalty if attacks disadvantaged monsters", "", "", "", ""},
      PM_KNIGHT,
      PM_PONY,
      PM_KING_ARTHUR,
      PM_PAGE,
      PM_IXOTH,
      PM_QUASIT,
      PM_OCHRE_JELLY,
      S_IMP,
      S_JELLY,
      ART_MAGIC_MIRROR_OF_MERLIN,
      MH_HUMAN | MH_DWARF | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
      /* Str Int Wis Dex Con Cha */
      { 15,  7, 12,  8, 14, 17 },
      { 30, 15, 15, 10, 20, 10 },
      /* Init   Lower  Higher */
      { 15, 0, 0, 11, 2, 1 }, /* Hit points */
      { 3, 2, 3, 2, 3, 2 }, /* Energy 1d3+2*/
      MAXULEV,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_HEALING_SPELL */
            SKILL_PROGRESS_NONE, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_BASIC, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Monk", 0 },
      { { "Candidate", 0 },
        { "Novice", 0 },
        { "Initiate", 0 },
        { "Student of Stones", 0 },
        { "Student of Waters", 0 },
        { "Student of Metals", 0 },
        { "Student of Winds", 0 },
        { "Student of Fire", 0 },
        { "Master", 0 } },
      "Shan Lai Ching", "Chih Sung-tzu", "Huan Ti", /* Chinese */
      "Mon",
      "the Monastery of Chan-Sune",
      "the Monastery of the Earth-Lord",
      5,
      { 
          "Vegetarianism", 
          "AC and MC bonus if without weapon, shield, and suit",
          "No staggering blow when wearing a shield or suit", 
          "-30 to-hit penalty when wearing a suit",
          ""
      },
      PM_MONK,
      NON_PM,
      PM_GRAND_MASTER,
      PM_ABBOT,
      PM_MASTER_KAEN,
      PM_EARTH_ELEMENTAL,
      PM_XORN,
      S_ELEMENTAL,
      S_XORN,
      ART_EYES_OF_THE_OVERWORLD,
      MH_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL
          | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 10,  7, 10, 10, 10, 10 },
      { 20, 10, 20, 20, 20, 15 },
      /* Init   Lower  Higher */
      { 12, 0, 0, 8, 1, 1 }, /* Hit points */
      { 3, 3, 3, 3, 3, 3 }, /* Energy 1d4+2*/
      MAXULEV,
      10,
      30,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_NONE, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Priest", "Priestess" },
      { { "Aspirant", 0 },
        { "Acolyte", 0 },
        { "Adept", 0 },
        { "Priest", "Priestess" },
        { "Curate", 0 },
        { "Canon", "Canoness" },
        { "Lama", 0 },
        { "Patriarch", "Matriarch" },
        { "High Priest", "High Priestess" } },
      0, 0, 0, /* deities from a randomly chosen other role will be used */
      "Pri",
      "the Great Temple",
      "the Temple of Nalzok",
      6,
      { "Sees the beatitude of all objects", "Can pray twice as often as normal", "", "", ""},
      PM_PRIEST,
      NON_PM,
      PM_ARCH_PRIEST,
      PM_ACOLYTE,
      PM_NALZOK,
      PM_HUMAN_ZOMBIE,
      PM_WRAITH,
      S_LESSER_UNDEAD,
      S_WRAITH,
      ART_MITRE_OF_HOLINESS,
      MH_HUMAN | MH_ELF | MH_GNOLL | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL
          | ROLE_CHAOTIC | ROLE_ALIGNMENT_TILES,
      /* Str Int Wis Dex Con Cha */
      { 7,   7, 13,  7,  7,  7 },
      { 15, 10, 30, 15, 20, 10 },
      /* Init   Lower  Higher */
      { 12, 0, 0, 7, 1, 0 }, /* Hit points */
      { 6, 2, 5, 3, 5, 3 },  /* Energy 1d4+4*/
      10,
      0,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_NATURE_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    /* Note:  Rogue precedes Ranger so that use of `-R' on the command line
       retains its traditional meaning. */
    { { "Rogue", 0 },
      { { "Footpad", 0 },
        { "Cutpurse", 0 },
        { "Rogue", 0 },
        { "Pilferer", 0 },
        { "Robber", 0 },
        { "Burglar", 0 },
        { "Filcher", 0 },
        { "Magsman", "Magswoman" },
        { "Thief", 0 } },
      "Issek", "Mog", "Kos", /* Nehwon */
      "Rog",
      "the Thieves' Guild Hall",
      "the Assassins' Guild Hall",
      7,
      { "Can backstab fleeing monsters", "", "", "", ""},
      PM_ROGUE,
      NON_PM,
      PM_MASTER_OF_THIEVES,
      PM_THUG,
      PM_MASTER_ASSASSIN,
      PM_LEPRECHAUN,
      PM_GUARDIAN_NAGA,
      S_NYMPH,
      S_NAGA,
      ART_MASTER_KEY_OF_THIEVERY,
      MH_HUMAN | MH_ORC | MH_GNOLL | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 7,   7,  7, 16,  7,  6 },
      { 20, 10, 10, 30, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0, 0, 8, 1, 0 }, /* Hit points */
      { 2, 1, 1, 2, 1, 2 },  /* Energy 1d3*/
      11,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_NONE, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Ranger", 0 },
      {
#if 0 /* OBSOLETE */
        {"Edhel",   "Elleth"},
        {"Edhel",   "Elleth"},         /* elf-maid */
        {"Ohtar",   "Ohtie"},          /* warrior */
        {"Kano",    "Kanie"},          /* commander (Q.) ['a] educated guess,
                                          until further research- SAC */
        {"Arandur"," Aranduriel"}, /* king's servant, minister (Q.) - guess */
        {"Hir",         "Hiril"},      /* lord, lady (S.) ['ir] */
        {"Aredhel",     "Arwen"},      /* noble elf, maiden (S.) */
        {"Ernil",       "Elentariel"}, /* prince (S.), elf-maiden (Q.) */
        {"Elentar",     "Elentari"},   /* Star-king, -queen (Q.) */
        "Solonor Thelandira", "Aerdrie Faenya", "Lolth", /* Elven */
#endif
        { "Tenderfoot", 0 },
        { "Lookout", 0 },
        { "Trailblazer", 0 },
        { "Reconnoiterer", "Reconnoiteress" },
        { "Scout", 0 },
        { "Arbalester", 0 }, /* One skilled at crossbows */
        { "Archer", 0 },
        { "Sharpshooter", 0 },
        { "Marksman", "Markswoman" } },
      "Mercury", "_Venus", "Mars", /* Roman/planets */
      "Ran",
      "Orion's camp",
      "the cave of the wumpus",
      8,
      { "", "", "", "", ""},
      PM_RANGER,
      PM_LITTLE_DOG /* Orion & canis major */,
      PM_ORION,
      PM_HUNTER,
      PM_SCORPIUS,
      PM_FOREST_CENTAUR,
      PM_SCORPION,
      S_CENTAUR,
      S_SPIDER,
      ART_LONGBOW_OF_DIANA,
      MH_HUMAN | MH_ELF | MH_GNOLL | MH_ORC | ROLE_MALE | ROLE_FEMALE
          | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      { 13,  9,  9, 14, 13, 12 },
      { 30, 10, 10, 30, 20, 10 },
      /* Init   Lower  Higher */
      { 13, 0, 0, 9, 1, 0 }, /* Hit points */
      { 2, 1, 1, 2, 1, 2 },  /* Energy 1d3*/
      12,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_NONE, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Samurai", 0 },
      { { "Hatamoto", 0 },       /* Banner Knight */
        { "Ronin", 0 },          /* no allegiance */
        { "Ninja", "Kunoichi" }, /* secret society */
        { "Joshu", 0 },          /* heads a castle */
        { "Ryoshu", 0 },         /* has a territory */
        { "Kokushu", 0 },        /* heads a province */
        { "Daimyo", 0 },         /* a samurai lord */
        { "Kuge", 0 },           /* Noble of the Court */
        { "Shogun", 0 } },       /* supreme commander, warlord */
      "_Amaterasu Omikami", "Raijin", "Susanowo", /* Japanese */
      "Sam",
      "the Castle of the Taro Clan",
      "the Shogun's Castle",
      9,
      { "Starts with a faithful dog", "", "", "", ""},
      PM_SAMURAI,
      PM_LITTLE_DOG,
      PM_LORD_SATO,
      PM_ROSHI,
      PM_ASHIKAGA_TAKAUJI,
      PM_WOLF,
      PM_STALKER,
      S_DOG,
      S_ELEMENTAL,
      ART_TSURUGI_OF_MURAMASA,
      MH_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
      /* Str Int Wis Dex Con Cha */
      { 10,  8, 7, 10, 17, 6 },
      { 30, 10, 8, 30, 14, 8 },
      /* Init   Lower  Higher */
      { 13, 0, 0, 9, 1, 1 }, /* Hit points */
      { 2, 2, 1, 3, 1, 3 }, /* Energy  1d4*/
      MAXULEV,
      10,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_NONE, /* P_HEALING_SPELL */
            SKILL_PROGRESS_NONE, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Tourist", 0 },
      { { "Rambler", 0 },
        { "Sightseer", 0 },
        { "Excursionist", 0 },
        { "Peregrinator", "Peregrinatrix" },
        { "Traveler", 0 },
        { "Journeyer", 0 },
        { "Voyager", 0 },
        { "Explorer", 0 },
        { "Adventurer", 0 } },
      "Blind Io", "_The Lady", "Offler", /* Discworld */
      "Tou",
      "Ankh-Morpork",
      "the Thieves' Guild Hall",
      10,
      { "Worse prices in shops below level 15", "", "", "", ""},
      PM_TOURIST,
      PM_SMALL_LUGGAGE,
      PM_TWOFLOWER,
      PM_GUIDE,
      PM_MASTER_OF_THIEVES,
      PM_GIANT_SPIDER,
      PM_FOREST_CENTAUR,
      S_SPIDER,
      S_CENTAUR,
      ART_YENDORIAN_EXPRESS_CARD,
      MH_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 7,  10,  6,  7,  7, 10 },
      { 15, 10, 10, 15, 30, 20 },
      /* Init   Lower  Higher */
      { 12, 0, 0, 6, 0, 1 }, /* Hit points */
      { 1, 3, 0, 4, 0, 4 }, /* Energy */
      MAXULEV,
      0,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_INI_UNSKILLED, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */        }
      },
    { { "Valkyrie", 0 },
      { { "Stripling", 0 },
        { "Skirmisher", 0 },
        { "Fighter", 0 },
        { "Man-at-arms", "Woman-at-arms" },
        { "Warrior", 0 },
        { "Swashbuckler", 0 },
        { "Hero", "Heroine" },
        { "Champion", 0 },
        { "Lord", "Lady" } },
      "Tyr", "Odin", "Loki", /* Norse */
      "Val",
      "the Shrine of Destiny",
      "the cave of Surtur",
      11,
      { "", "", "", "", ""},
      PM_VALKYRIE,
      PM_DIREWOLF_CUB,
      PM_NORN,
      PM_WARRIOR,
      PM_LORD_SURTUR,
      PM_FIRE_ANT,
      PM_FIRE_GIANT,
      S_ANT,
      S_GIANT,
      ART_ORB_OF_FATE,
      MH_HUMAN | MH_DWARF | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL,
      /* Str Int Wis Dex Con Cha */
      { 10, 7, 7,  7, 10, 7 },
      { 30, 6, 7, 20, 30, 7 },
      /* Init   Lower  Higher */
      { 14, 0, 0, 10, 2, 0 }, /* Hit points */
      { 1, 1, 0, 2, 0, 2 },  /* Energy 1d3-1*/
      MAXULEV,
      0,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_NONE, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_NONE, /* P_HEALING_SPELL */
            SKILL_PROGRESS_NONE, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_NONE, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_INI_BASIC, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */
        }
      },
    { { "Wizard", 0 },
      { { "Evoker", 0 },
        { "Conjurer", 0 },
        { "Thaumaturge", 0 },
        { "Magician", 0 },
        { "Enchanter", "Enchantress" },
        { "Sorcerer", "Sorceress" },
        { "Necromancer", 0 },
        { "Wizard", 0 },
        { "Mage", 0 } },
      "Ptah", "Thoth", "Anhur", /* Egyptian */
      "Wiz",
      "the Lonely Tower",
      "the Tower of Darkness",
      NUM_ROLES - 1,
      { "", "", "", "", ""},
      PM_WIZARD,
      PM_KITTEN,
      PM_NEFERET_THE_GREEN,
      PM_APPRENTICE,
      PM_DARK_ONE,
      PM_VAMPIRE_BAT,
      PM_XORN,
      S_BAT,
      S_WRAITH,
      ART_EYE_OF_THE_AETHIOPICA,
      MH_HUMAN | MH_ELF | MH_ORC | ROLE_MALE | ROLE_FEMALE
          | ROLE_NEUTRAL | ROLE_CHAOTIC,
      /* Str Int Wis Dex Con Cha */
      {  7, 13,  7,  7,  7,  7 },
      { 10, 30, 10, 20, 20, 10 },
      /* Init   Lower  Higher */
      { 10, 0, 0, 6, 0, 1 }, /* Hit points */
      { 8, 2, 5, 5, 5, 5 }, /* Energy 1d6+4*/
      MAXULEV,
      0,
      0,
        {
            SKILL_PROGRESS_NONE, /* P_NONE */
            SKILL_PROGRESS_NONE, /* P_DAGGER */
            SKILL_PROGRESS_NONE, /* P_AXE */
            SKILL_PROGRESS_NONE, /* P_PICK_AXE */
            SKILL_PROGRESS_NONE, /* P_SWORD */
            SKILL_PROGRESS_NONE, /* P_BLUDGEONING_WEAPON */
            SKILL_PROGRESS_NONE, /* P_FLAIL */
            SKILL_PROGRESS_NONE, /* P_QUARTERSTAFF */
            SKILL_PROGRESS_NONE, /* P_POLEARM */
            SKILL_PROGRESS_NONE, /* P_SPEAR */
            SKILL_PROGRESS_NONE, /* P_BOW */
            SKILL_PROGRESS_NONE, /* P_SLING */
            SKILL_PROGRESS_NONE, /* P_CROSSBOW */
            SKILL_PROGRESS_NONE, /* P_THROWN_WEAPON */
            SKILL_PROGRESS_NONE, /* P_WHIP */

            SKILL_PROGRESS_INI_BASIC, /* P_ARCANE_SPELL */
            SKILL_PROGRESS_NONE, /* P_CLERIC_SPELL */
            SKILL_PROGRESS_NONE, /* P_HEALING_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_DIVINATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_ABJURATION_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_MOVEMENT_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_TRANSMUTATION_SPELL */
            SKILL_PROGRESS_INI_BASIC, /* P_ENCHANTMENT_SPELL */
            SKILL_PROGRESS_INI_UNSKILLED, /* P_CONJURATION_SPELL */
            SKILL_PROGRESS_NONE, /* P_CELESTIAL_SPELL */
            SKILL_PROGRESS_NONE, /* P_NATURE_SPELL */
            SKILL_PROGRESS_NONE, /* P_NECROMANCY_SPELL */

            SKILL_PROGRESS_NONE, /* P_BARE_HANDED_COMBAT */
            SKILL_PROGRESS_NONE, /* P_MARTIAL_ARTS */
            SKILL_PROGRESS_NONE, /* P_TWO_WEAPON_COMBAT */
            SKILL_PROGRESS_NONE, /* P_SHIELD */
            SKILL_PROGRESS_NONE, /* P_WAND */
            SKILL_PROGRESS_NONE, /* P_RIDING */
            SKILL_PROGRESS_NONE  /* P_DISARM_TRAP */
        }
      },
    /* Array terminator */
    { { 0, 0 } }
};

/* The player's role, created at runtime from initial
 * choices.  This may be munged in role_init().
 */

struct Role urole = {
    { "Undefined", 0 },
    { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
      { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    "L", "N", "C",
    "Xxx", "home", "locate",
    0,
    { "", "", "", "", ""},
    NON_PM, NON_PM, NON_PM, NON_PM, NON_PM, NON_PM, NON_PM,
    0, 0, 0, 0,
    /* Str Int Wis Dex Con Cha */
    { 7, 7, 7, 7, 7, 7 },
    { 20, 15, 15, 20, 20, 10 },
    /* Init   Lower  Higher */
    { 10, 0, 0, 8, 1, 0 }, /* Hit points: initial z, d; low-levels z, d; high-level, z, d  */
    { 2, 0, 0, 2, 0, 3 }, /* Energy: initial z, d; low-levels z, d; high-level, z, d */
    14,
     0,
     0,
    {
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE,
        SKILL_PROGRESS_NONE
    }
};


/* Table of all races */
const struct Race races[] = {
    {
        "human",
        "human",
        "humanity",
        "Hum",
        { "man", "woman" },
        0,
        { "", "", "", "", ""},
        PM_HUMAN,
        PM_HUMAN_MUMMY,
        PM_HUMAN_ZOMBIE,
        MH_HUMAN | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL | ROLE_NEUTRAL | ROLE_CHAOTIC,
        MH_HUMAN,
        0,
        MH_GNOLL | MH_ORC,
        /*    Str     Int Wis Dex Con Cha */
        { 3, 3, 3, 3, 3, 3 },
        { STR18(100), 18, 18, 18, 18, 18 },
        /* Init   Lower  Higher */
        { 2, 0, 0, 2, 1, 0 }, /* Hit points */
        { 1, 3, 1, 3, 1, 3 }  /* Energy 1d4*/
    },
    {
        "elf",
        "elven",
        "elvenkind",
        "Elf",
        { 0, 0 },
        1,
        { "Increased nutrition from elven waybread", "", "", "", ""},
        PM_ELF,
        PM_ELF_MUMMY,
        PM_ELF_ZOMBIE,
        MH_ELF | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL,
        MH_ELF,
        MH_ELF,
        MH_ORC | MH_GNOLL,
        /*  Str    Int Wis Dex Con Cha */
        { 3, 3, 3, 3, 3, 3 },
        { 18, 20, 20, 18, 16, 18 },
        /* Init   Lower  Higher */
        { 1, 0, 0, 1, 1, 0 }, /* Hit points */
        { 1, 5, 1, 5, 1, 5 }  /* Energy  1d6*/
    },
    {
        "dwarf",
        "dwarven",
        "dwarvenkind",
        "Dwa",
        { 0, 0 },
        2,
        { "Double digging speed", "Increased nutrition from cram ration", "", "", ""},
        PM_DWARF,
        PM_DWARF_MUMMY,
        PM_DWARF_ZOMBIE,
        MH_DWARF | ROLE_MALE | ROLE_FEMALE | ROLE_LAWFUL,
        MH_DWARF,
        MH_DWARF,
        MH_ORC,
        /*    Str     Int Wis Dex Con Cha */
        { 3, 3, 3, 3, 3, 3 },
        { STR18(100), 16, 16, 20, 20, 16 },
        /* Init   Lower  Higher */
        { 4, 0, 0, 3, 2, 0 }, /* Hit points */
        { 0, 0, 0, 0, 0, 0 }  /* Energy 0d1*/
    },
    {
        "gnoll",
        "gnollish",
        "gnollkind",
        "Gnl",
        { 0, 0 },
        3,
        { "Smells rotten food", "Smells underground roots", "Triple nutrition from tripe ration", "", ""},
        PM_GNOLL,
        PM_GNOLL_MUMMY,
        PM_GNOLL_ZOMBIE,
        MH_GNOLL | ROLE_MALE | ROLE_FEMALE | ROLE_NEUTRAL | ROLE_CHAOTIC,
        MH_GNOLL,
        MH_GNOLL,
        MH_HUMAN,
        /*  Str    Int Wis Dex Con Cha */
        { 3, 3, 3, 3, 3, 3 },
        { STR18(100), 16, 16, 19, 19, 16 },
        /* Init   Lower  Higher */
        { 3, 0, 0, 3, 1, 0 }, /* Hit points */
        { 1, 2, 1, 2, 1, 2 }  /* Energy 1d3*/
    },
    {
        "orc",
        "orcish",
        "orcdom",
        "Orc",
        { 0, 0 },
        NUM_RACES - 1,
        { "Can practice cannibalism", "Reduced nutrition from elven waybread", "", "", ""},
        PM_ORC,
        PM_ORC_MUMMY,
        PM_ORC_ZOMBIE,
        MH_ORC | ROLE_MALE | ROLE_FEMALE | ROLE_CHAOTIC,
        MH_ORC,
        0,
        MH_HUMAN | MH_ELF | MH_DWARF,
        /*  Str    Int Wis Dex Con Cha */
        { 3, 3, 3, 3, 3, 3 },
        { STR18(100), 16, 16, 18, 18, 16 },
        /* Init   Lower  Higher */
        { 3, 0, 0, 3, 1, 0 }, /* Hit points */
        { 1, 1, 1, 1, 1, 1 }  /* Energy 1d2*/
    },
            /* Array terminator */
    { 0, 0, 0, 0 }
};

/* The player's race, created at runtime from initial
 * choices.  This may be munged in role_init().
 */
struct Race urace = {
    "something",
    "undefined",
    "something",
    "Xxx",
    { 0, 0 },
    0,
    { "", "", "", "", ""},
    NON_PM,
    NON_PM,
    NON_PM,
    0,
    0,
    0,
    0,
    /*    Str     Int Wis Dex Con Cha */
    { 3, 3, 3, 3, 3, 3 },
    { STR18(100), 18, 18, 18, 18, 18 },
    /* Init   Lower  Higher */
    { 2, 0, 0, 2, 1, 0 }, /* Hit points */
    { 1, 0, 2, 0, 2, 0 }  /* Energy */
};

/* Table of all genders */
const struct Gender genders[] = {
    { "male", "he", "him", "his", "Mal", ROLE_MALE },
    { "female", "she", "her", "her", "Fem", ROLE_FEMALE },
    { "neuter", "it", "it", "its", "Ntr", ROLE_NEUTER }
};

/* Table of all alignments */
const struct Align aligns[] = {
    { "law", "lawful", "Law", ROLE_LAWFUL, A_LAWFUL },
    { "balance", "neutral", "Neu", ROLE_NEUTRAL, A_NEUTRAL },
    { "chaos", "chaotic", "Cha", ROLE_CHAOTIC, A_CHAOTIC },
    { "evil", "unaligned", "Una", 0, A_NONE }
};

/* Filters */
static struct {
    boolean roles[SIZE(roles)];
    unsigned long mask;
} rfilter;

STATIC_DCL int NDECL(randrole_filtered);
STATIC_DCL char *FDECL(promptsep, (char *, int));
STATIC_DCL int FDECL(role_gendercount, (int));
STATIC_DCL int FDECL(race_alignmentcount, (int));

/* used by str2XXX() */
static char NEARDATA randomstr[] = "random";

boolean
validrole(rolenum)
int rolenum;
{
    return (boolean) (rolenum >= 0 && rolenum < SIZE(roles) - 1);
}

int
randrole(for_display)
boolean for_display;
{
    int res = SIZE(roles) - 1;

    if (for_display)
        res = rn2_on_display_rng(res);
    else
        res = rn2(res);
    return res;
}

STATIC_OVL int
randrole_filtered()
{
    int i, n = 0, set[SIZE(roles)];

    /* this doesn't rule out impossible combinations but attempts to
       honor all the filter masks */
    for (i = 0; i < SIZE(roles) - 1; ++i)
        if (ok_role(i, ROLE_NONE, ROLE_NONE, ROLE_NONE)
            && ok_race(i, ROLE_RANDOM, ROLE_NONE, ROLE_NONE)
            && ok_gend(i, ROLE_NONE, ROLE_RANDOM, ROLE_NONE)
            && ok_align(i, ROLE_NONE, ROLE_NONE, ROLE_RANDOM))
            set[n++] = i;
    return n ? set[rn2(n)] : randrole(FALSE);
}

int
str2role(str)
const char *str;
{
    int i;
    size_t len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; roles[i].name.m; i++) {
        /* Does it match the male name? */
        if (!strncmpi(str, roles[i].name.m, len))
            return i;
        /* Or the female name? */
        if (roles[i].name.f && !strncmpi(str, roles[i].name.f, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, roles[i].filecode))
            return i;
    }

    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

boolean
validrace(rolenum, racenum)
int rolenum, racenum;
{
    /* Assumes validrole */
    return (boolean) (racenum >= 0 && racenum < SIZE(races) - 1
                      && (roles[rolenum].allow & races[racenum].allow
                          & ROLE_RACEMASK));
}

int
randrace(rolenum)
int rolenum;
{
    int i, n = 0;

    /* Count the number of valid races */
    for (i = 0; races[i].noun; i++)
        if (roles[rolenum].allow & races[i].allow & ROLE_RACEMASK)
            n++;

    /* Pick a random race */
    /* Use a factor of 100 in case of bad random number generators */
    if (n)
        n = rn2(n * 100) / 100;
    for (i = 0; races[i].noun; i++)
        if (roles[rolenum].allow & races[i].allow & ROLE_RACEMASK) {
            if (n)
                n--;
            else
                return i;
        }

    /* This role has no permitted races? */
    return rn2(SIZE(races) - 1);
}

int
str2race(str)
const char *str;
{
    int i;
    size_t len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; races[i].noun; i++) {
        /* Does it match the noun? */
        if (!strncmpi(str, races[i].noun, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, races[i].filecode))
            return i;
    }

    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

boolean
validgend(rolenum, racenum, gendnum)
int rolenum, racenum, gendnum;
{
    /* Assumes validrole and validrace */
    return (boolean) (gendnum >= 0 && gendnum < ROLE_GENDERS
                      && (roles[rolenum].allow & races[racenum].allow
                          & genders[gendnum].allow & ROLE_GENDMASK));
}

int
randgend(rolenum, racenum)
int rolenum, racenum;
{
    int i, n = 0;

    /* Count the number of valid genders */
    for (i = 0; i < ROLE_GENDERS; i++)
        if (roles[rolenum].allow & races[racenum].allow & genders[i].allow
            & ROLE_GENDMASK)
            n++;

    /* Pick a random gender */
    if (n)
        n = rn2(n);
    for (i = 0; i < ROLE_GENDERS; i++)
        if (roles[rolenum].allow & races[racenum].allow & genders[i].allow
            & ROLE_GENDMASK) {
            if (n)
                n--;
            else
                return i;
        }

    /* This role/race has no permitted genders? */
    return rn2(ROLE_GENDERS);
}

int
str2gend(str)
const char *str;
{
    int i;
    size_t len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; i < ROLE_GENDERS; i++) {
        /* Does it match the adjective? */
        if (!strncmpi(str, genders[i].adj, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, genders[i].filecode))
            return i;
    }
    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

boolean
validalign(rolenum, racenum, alignnum)
int rolenum, racenum, alignnum;
{
    /* Assumes validrole and validrace */
    return (boolean) (alignnum >= 0 && alignnum < ROLE_ALIGNS
                      && (roles[rolenum].allow & races[racenum].allow
                          & aligns[alignnum].allow & ROLE_ALIGNMASK));
}

int
randalign(rolenum, racenum)
int rolenum, racenum;
{
    int i, n = 0;

    /* Count the number of valid alignments */
    for (i = 0; i < ROLE_ALIGNS; i++)
        if (roles[rolenum].allow & races[racenum].allow & aligns[i].allow
            & ROLE_ALIGNMASK)
            n++;

    /* Pick a random alignment */
    if (n)
        n = rn2(n);
    for (i = 0; i < ROLE_ALIGNS; i++)
        if (roles[rolenum].allow & races[racenum].allow & aligns[i].allow
            & ROLE_ALIGNMASK) {
            if (n)
                n--;
            else
                return i;
        }

    /* This role/race has no permitted alignments? */
    return rn2(ROLE_ALIGNS);
}

int
str2align(str)
const char *str;
{
    int i;
    size_t len;

    /* Is str valid? */
    if (!str || !str[0])
        return ROLE_NONE;

    /* Match as much of str as is provided */
    len = strlen(str);
    for (i = 0; i < ROLE_ALIGNS; i++) {
        /* Does it match the adjective? */
        if (!strncmpi(str, aligns[i].adj, len))
            return i;
        /* Or the filecode? */
        if (!strcmpi(str, aligns[i].filecode))
            return i;
    }
    if ((len == 1 && (*str == '*' || *str == '@'))
        || !strncmpi(str, randomstr, len))
        return ROLE_RANDOM;

    /* Couldn't find anything appropriate */
    return ROLE_NONE;
}

/* is rolenum compatible with any racenum/gendnum/alignnum constraints? */
boolean
ok_role(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum, gendnum, alignnum;
{
    int i;
    unsigned long allow;

    if (rolenum >= 0 && rolenum < SIZE(roles) - 1) {
        if (rfilter.roles[rolenum])
            return FALSE;
        allow = roles[rolenum].allow;
        if (racenum >= 0 && racenum < SIZE(races) - 1
            && !(allow & races[racenum].allow & ROLE_RACEMASK))
            return FALSE;
        if (gendnum >= 0 && gendnum < ROLE_GENDERS
            && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
            return FALSE;
        if (alignnum >= 0 && alignnum < ROLE_ALIGNS
            && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < SIZE(roles) - 1; i++) {
            if (rfilter.roles[i])
                continue;
            allow = roles[i].allow;
            if (racenum >= 0 && racenum < SIZE(races) - 1
                && !(allow & races[racenum].allow & ROLE_RACEMASK))
                continue;
            if (gendnum >= 0 && gendnum < ROLE_GENDERS
                && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
                continue;
            if (alignnum >= 0 && alignnum < ROLE_ALIGNS
                && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* pick a random role subject to any racenum/gendnum/alignnum constraints */
/* If pickhow == PICK_RIGID a role is returned only if there is  */
/* a single possibility */
int
pick_role(racenum, gendnum, alignnum, pickhow)
int racenum, gendnum, alignnum, pickhow;
{
    int i;
    int roles_ok = 0, set[SIZE(roles)];

    for (i = 0; i < SIZE(roles) - 1; i++) {
        if (ok_role(i, racenum, gendnum, alignnum)
            && ok_race(i, (racenum >= 0) ? racenum : ROLE_RANDOM,
                       gendnum, alignnum)
            && ok_gend(i, racenum,
                       (gendnum >= 0) ? gendnum : ROLE_RANDOM, alignnum)
            && ok_race(i, racenum,
                       gendnum, (alignnum >= 0) ? alignnum : ROLE_RANDOM))
            set[roles_ok++] = i;
    }
    if (roles_ok == 0 || (roles_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    return set[rn2(roles_ok)];
}

/* is racenum compatible with any rolenum/gendnum/alignnum constraints? */
boolean
ok_race(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum, gendnum, alignnum;
{
    int i;
    unsigned long allow;

    if (racenum >= 0 && racenum < SIZE(races) - 1) {
        if (rfilter.mask & races[racenum].selfmask)
            return FALSE;
        allow = races[racenum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1
            && !(allow & roles[rolenum].allow & ROLE_RACEMASK))
            return FALSE;
        if (gendnum >= 0 && gendnum < ROLE_GENDERS
            && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
            return FALSE;
        if (alignnum >= 0 && alignnum < ROLE_ALIGNS
            && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < SIZE(races) - 1; i++) {
            if (rfilter.mask & races[i].selfmask)
                continue;
            allow = races[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1
                && !(allow & roles[rolenum].allow & ROLE_RACEMASK))
                continue;
            if (gendnum >= 0 && gendnum < ROLE_GENDERS
                && !(allow & genders[gendnum].allow & ROLE_GENDMASK))
                continue;
            if (alignnum >= 0 && alignnum < ROLE_ALIGNS
                && !(allow & aligns[alignnum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* Pick a random race subject to any rolenum/gendnum/alignnum constraints.
   If pickhow == PICK_RIGID a race is returned only if there is
   a single possibility. */
int
pick_race(rolenum, gendnum, alignnum, pickhow)
int rolenum, gendnum, alignnum, pickhow;
{
    int i;
    int races_ok = 0;

    for (i = 0; i < SIZE(races) - 1; i++) {
        if (ok_race(rolenum, i, gendnum, alignnum))
            races_ok++;
    }
    if (races_ok == 0 || (races_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    races_ok = rn2(races_ok);
    for (i = 0; i < SIZE(races) - 1; i++) {
        if (ok_race(rolenum, i, gendnum, alignnum)) {
            if (races_ok == 0)
                return i;
            else
                races_ok--;
        }
    }
    return ROLE_NONE;
}

/* is gendnum compatible with any rolenum/racenum/alignnum constraints? */
/* gender and alignment are not comparable (and also not constrainable) */
boolean
ok_gend(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum, gendnum;
int alignnum UNUSED;
{
    int i;
    unsigned long allow;

    if (gendnum >= 0 && gendnum < ROLE_GENDERS) {
        if (rfilter.mask & genders[gendnum].allow)
            return FALSE;
        allow = genders[gendnum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1
            && !(allow & roles[rolenum].allow & ROLE_GENDMASK))
            return FALSE;
        if (racenum >= 0 && racenum < SIZE(races) - 1
            && !(allow & races[racenum].allow & ROLE_GENDMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < ROLE_GENDERS; i++) {
            if (rfilter.mask & genders[i].allow)
                continue;
            allow = genders[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1
                && !(allow & roles[rolenum].allow & ROLE_GENDMASK))
                continue;
            if (racenum >= 0 && racenum < SIZE(races) - 1
                && !(allow & races[racenum].allow & ROLE_GENDMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* pick a random gender subject to any rolenum/racenum/alignnum constraints */
/* gender and alignment are not comparable (and also not constrainable) */
/* If pickhow == PICK_RIGID a gender is returned only if there is  */
/* a single possibility */
int
pick_gend(rolenum, racenum, alignnum, pickhow)
int rolenum, racenum, alignnum, pickhow;
{
    int i;
    int gends_ok = 0;

    for (i = 0; i < ROLE_GENDERS; i++) {
        if (ok_gend(rolenum, racenum, i, alignnum))
            gends_ok++;
    }
    if (gends_ok == 0 || (gends_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    gends_ok = rn2(gends_ok);
    for (i = 0; i < ROLE_GENDERS; i++) {
        if (ok_gend(rolenum, racenum, i, alignnum)) {
            if (gends_ok == 0)
                return i;
            else
                gends_ok--;
        }
    }
    return ROLE_NONE;
}

/* is alignnum compatible with any rolenum/racenum/gendnum constraints? */
/* alignment and gender are not comparable (and also not constrainable) */
boolean
ok_align(rolenum, racenum, gendnum, alignnum)
int rolenum, racenum;
int gendnum UNUSED;
int alignnum;
{
    int i;
    unsigned long allow;

    if (alignnum >= 0 && alignnum < ROLE_ALIGNS) {
        if (rfilter.mask & aligns[alignnum].allow)
            return FALSE;
        allow = aligns[alignnum].allow;
        if (rolenum >= 0 && rolenum < SIZE(roles) - 1
            && !(allow & roles[rolenum].allow & ROLE_ALIGNMASK))
            return FALSE;
        if (racenum >= 0 && racenum < SIZE(races) - 1
            && !(allow & races[racenum].allow & ROLE_ALIGNMASK))
            return FALSE;
        return TRUE;
    } else {
        /* random; check whether any selection is possible */
        for (i = 0; i < ROLE_ALIGNS; i++) {
            if (rfilter.mask & aligns[i].allow)
                return FALSE;
            allow = aligns[i].allow;
            if (rolenum >= 0 && rolenum < SIZE(roles) - 1
                && !(allow & roles[rolenum].allow & ROLE_ALIGNMASK))
                continue;
            if (racenum >= 0 && racenum < SIZE(races) - 1
                && !(allow & races[racenum].allow & ROLE_ALIGNMASK))
                continue;
            return TRUE;
        }
        return FALSE;
    }
}

/* Pick a random alignment subject to any rolenum/racenum/gendnum constraints;
   alignment and gender are not comparable (and also not constrainable).
   If pickhow == PICK_RIGID an alignment is returned only if there is
   a single possibility. */
int
pick_align(rolenum, racenum, gendnum, pickhow)
int rolenum, racenum, gendnum, pickhow;
{
    int i;
    int aligns_ok = 0;

    for (i = 0; i < ROLE_ALIGNS; i++) {
        if (ok_align(rolenum, racenum, gendnum, i))
            aligns_ok++;
    }
    if (aligns_ok == 0 || (aligns_ok > 1 && pickhow == PICK_RIGID))
        return ROLE_NONE;
    aligns_ok = rn2(aligns_ok);
    for (i = 0; i < ROLE_ALIGNS; i++) {
        if (ok_align(rolenum, racenum, gendnum, i)) {
            if (aligns_ok == 0)
                return i;
            else
                aligns_ok--;
        }
    }
    return ROLE_NONE;
}

void
rigid_role_checks()
{
    int tmp;

    /* Some roles are limited to a single race, alignment, or gender and
     * calling this routine prior to XXX_player_selection() will help
     * prevent an extraneous prompt that actually doesn't allow
     * you to choose anything further. Note the use of PICK_RIGID which
     * causes the pick_XX() routine to return a value only if there is one
     * single possible selection, otherwise it returns ROLE_NONE.
     *
     */
    if (flags.initrole == ROLE_RANDOM) {
        /* If the role was explicitly specified as ROLE_RANDOM
         * via -uXXXX-@ or OPTIONS=role:random then choose the role
         * in here to narrow down later choices.
         */
        flags.initrole = pick_role(flags.initrace, flags.initgend,
                                   flags.initalign, PICK_RANDOM);
        if (flags.initrole < 0)
            flags.initrole = randrole_filtered();
    }
    if (flags.initrace == ROLE_RANDOM
        && (tmp = pick_race(flags.initrole, flags.initgend,
                            flags.initalign, PICK_RANDOM)) != ROLE_NONE)
        flags.initrace = tmp;
    if (flags.initalign == ROLE_RANDOM
        && (tmp = pick_align(flags.initrole, flags.initrace,
                             flags.initgend, PICK_RANDOM)) != ROLE_NONE)
        flags.initalign = tmp;
    if (flags.initgend == ROLE_RANDOM
        && (tmp = pick_gend(flags.initrole, flags.initrace,
                            flags.initalign, PICK_RANDOM)) != ROLE_NONE)
        flags.initgend = tmp;

    if (flags.initrole != ROLE_NONE) {
        if (flags.initrace == ROLE_NONE)
            flags.initrace = pick_race(flags.initrole, flags.initgend,
                                       flags.initalign, PICK_RIGID);
        if (flags.initalign == ROLE_NONE)
            flags.initalign = pick_align(flags.initrole, flags.initrace,
                                         flags.initgend, PICK_RIGID);
        if (flags.initgend == ROLE_NONE)
            flags.initgend = pick_gend(flags.initrole, flags.initrace,
                                       flags.initalign, PICK_RIGID);
    }
}

boolean
setrolefilter(bufp)
const char *bufp;
{
    int i;
    boolean reslt = TRUE;

    if ((i = str2role(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.roles[i] = TRUE;
    else if ((i = str2race(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.mask |= races[i].selfmask;
    else if ((i = str2gend(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.mask |= genders[i].allow;
    else if ((i = str2align(bufp)) != ROLE_NONE && i != ROLE_RANDOM)
        rfilter.mask |= aligns[i].allow;
    else
        reslt = FALSE;
    return reslt;
}

boolean
gotrolefilter()
{
    int i;

    if (rfilter.mask)
        return TRUE;
    for (i = 0; i < SIZE(roles); ++i)
        if (rfilter.roles[i])
            return TRUE;
    return FALSE;
}

void
clearrolefilter()
{
    int i;

    for (i = 0; i < SIZE(roles); ++i)
        rfilter.roles[i] = FALSE;
    rfilter.mask = 0;
}

#define BP_ALIGN 0
#define BP_GEND 1
#define BP_RACE 2
#define BP_ROLE 3
#define NUM_BP 4

STATIC_VAR char pa[NUM_BP], post_attribs;

STATIC_OVL char *
promptsep(buf, num_post_attribs)
char *buf;
int num_post_attribs;
{
    const char *conjuct = "and ";

    if (num_post_attribs > 1 && post_attribs < num_post_attribs
        && post_attribs > 1)
        Strcat(buf, ",");
    Strcat(buf, " ");
    --post_attribs;
    if (!post_attribs && num_post_attribs > 1)
        Strcat(buf, conjuct);
    return buf;
}

STATIC_OVL int
role_gendercount(rolenum)
int rolenum;
{
    int gendcount = 0;

    if (validrole(rolenum)) {
        if (roles[rolenum].allow & ROLE_MALE)
            ++gendcount;
        if (roles[rolenum].allow & ROLE_FEMALE)
            ++gendcount;
        if (roles[rolenum].allow & ROLE_NEUTER)
            ++gendcount;
    }
    return gendcount;
}

STATIC_OVL int
race_alignmentcount(racenum)
int racenum;
{
    int aligncount = 0;

    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM) {
        if (races[racenum].allow & ROLE_CHAOTIC)
            ++aligncount;
        if (races[racenum].allow & ROLE_LAWFUL)
            ++aligncount;
        if (races[racenum].allow & ROLE_NEUTRAL)
            ++aligncount;
    }
    return aligncount;
}

char *
root_plselection_prompt(suppliedbuf, buflen, rolenum, racenum, gendnum,
                        alignnum)
char *suppliedbuf;
size_t buflen;
int rolenum, racenum, gendnum, alignnum;
{
    int k, gendercount = 0, aligncount = 0;
    char buf[BUFSZ];
    static char err_ret[] = " character's";
    boolean donefirst = FALSE;
    int maxroles = sizeof(roles) / sizeof(roles[0]);
    int maxraces = sizeof(races) / sizeof(races[0]);

    if (rolenum >= maxroles)
        rolenum = maxroles - 1;

    if (racenum >= maxraces)
        racenum = maxraces - 1;

    if (!suppliedbuf || buflen < 1)
        return err_ret;

    /* initialize these static variables each time this is called */
    post_attribs = 0;
    for (k = 0; k < NUM_BP; ++k)
        pa[k] = 0;
    buf[0] = '\0';
    *suppliedbuf = '\0';

    /* How many alignments are allowed for the desired race? */
    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM)
        aligncount = race_alignmentcount(racenum);

    if (alignnum != ROLE_NONE && alignnum != ROLE_RANDOM
        && ok_align(rolenum, racenum, gendnum, alignnum)) {
        /* if race specified, and multiple choice of alignments for it */
        if ((racenum >= 0) && (aligncount > 1)) {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, aligns[alignnum].adj);
            donefirst = TRUE;
        } else {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, aligns[alignnum].adj);
            donefirst = TRUE;
        }
    } else {
        /* in case we got here by failing the ok_align() test */
        if (alignnum != ROLE_RANDOM)
            alignnum = ROLE_NONE;
        /* if alignment not specified, but race is specified
           and only one choice of alignment for that race then
           don't include it in the later list */
        if ((((racenum != ROLE_NONE && racenum != ROLE_RANDOM)
              && ok_race(rolenum, racenum, gendnum, alignnum))
             && (aligncount > 1))
            || (racenum == ROLE_NONE || racenum == ROLE_RANDOM)) {
            pa[BP_ALIGN] = 1;
            post_attribs++;
        }
    }
    /* <your lawful> */

    /* How many genders are allowed for the desired role? */
    if (validrole(rolenum))
        gendercount = role_gendercount(rolenum);

    if (gendnum != ROLE_NONE && gendnum != ROLE_RANDOM) {
        if (validrole(rolenum)) {
            /* if role specified, and multiple choice of genders for it,
               and name of role itself does not distinguish gender */
            if ((rolenum != ROLE_NONE) && (gendercount > 1)
                && !roles[rolenum].name.f) {
                if (donefirst)
                    Strcat(buf, " ");
                Strcat(buf, genders[gendnum].adj);
                donefirst = TRUE;
            }
        } else {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, genders[gendnum].adj);
            donefirst = TRUE;
        }
    } else {
        /* if gender not specified, but role is specified
                and only one choice of gender then
                don't include it in the later list */
        if ((validrole(rolenum) && (gendercount > 1))
            || !validrole(rolenum)) {
            pa[BP_GEND] = 1;
            post_attribs++;
        }
    }
    /* <your lawful female> */

    if (racenum != ROLE_NONE && racenum != ROLE_RANDOM) {
        if (validrole(rolenum)
            && ok_race(rolenum, racenum, gendnum, alignnum)) {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, (rolenum == ROLE_NONE) ? races[racenum].noun
                                               : races[racenum].adj);
            donefirst = TRUE;
        } else if (!validrole(rolenum)) {
            if (donefirst)
                Strcat(buf, " ");
            Strcat(buf, races[racenum].noun);
            donefirst = TRUE;
        } else {
            pa[BP_RACE] = 1;
            post_attribs++;
        }
    } else {
        pa[BP_RACE] = 1;
        post_attribs++;
    }
    /* <your lawful female gnomish> || <your lawful female gnome> */

    if (validrole(rolenum)) {
        if (donefirst)
            Strcat(buf, " ");
        if (gendnum != ROLE_NONE) {
            if (gendnum == 1 && roles[rolenum].name.f)
                Strcat(buf, roles[rolenum].name.f);
            else
                Strcat(buf, roles[rolenum].name.m);
        } else {
            if (roles[rolenum].name.f) {
                Strcat(buf, roles[rolenum].name.m);
                Strcat(buf, "/");
                Strcat(buf, roles[rolenum].name.f);
            } else
                Strcat(buf, roles[rolenum].name.m);
        }
        donefirst = TRUE;
    } else if (rolenum == ROLE_NONE) {
        pa[BP_ROLE] = 1;
        post_attribs++;
    }

    if ((racenum == ROLE_NONE || racenum == ROLE_RANDOM)
        && !validrole(rolenum)) {
        if (donefirst)
            Strcat(buf, " ");
        Strcat(buf, "character");
        donefirst = TRUE;
    }
    /* <your lawful female gnomish cavewoman> || <your lawful female gnome>
     *    || <your lawful female character>
     */
    if (buflen > strlen(buf) + 1) {
        Strcpy(suppliedbuf, buf);
        return suppliedbuf;
    } else
        return err_ret;
}

char *
build_plselection_prompt(buf, buflen, rolenum, racenum, gendnum, alignnum, add_ynq)
char *buf;
size_t buflen;
int rolenum, racenum, gendnum, alignnum;
boolean add_ynq;
{
    const char *defprompt = "Shall I pick a character for you?";
    const char *defprompt_ynq = "Shall I pick a character for you? [ynaq] ";
    int num_post_attribs = 0;
    char tmpbuf[BUFSZ], *p;

    if (buflen < QBUFSZ)
        return (char *) (add_ynq ? defprompt_ynq : defprompt);

    Strcpy(tmpbuf, "Shall I pick ");
    if (racenum != ROLE_NONE || validrole(rolenum))
        Strcat(tmpbuf, "your ");
    else
        Strcat(tmpbuf, "a ");
    /* <your> */

    (void) root_plselection_prompt(eos(tmpbuf), buflen - strlen(tmpbuf),
                                   rolenum, racenum, gendnum, alignnum);
    /* "Shall I pick a character's role, race, gender, and alignment for you?"
       plus " [ynaq] (y)" is a little too long for a conventional 80 columns;
       also, "pick a character's <anything>" sounds a bit stilted */
    strsubst(tmpbuf, "pick a character", "pick character");
    Sprintf(buf, "%s", s_suffix(tmpbuf));
    /* don't bother splitting caveman/cavewoman or priest/priestess
       in order to apply possessive suffix to both halves, but do
       change "priest/priestess'" to "priest/priestess's" */
    if ((p = strstri(buf, "priest/priestess'")) != 0
        && p[sizeof "priest/priestess'" - sizeof ""] == '\0')
        strkitten(buf, 's');

    /* buf should now be:
     *    <your lawful female gnomish cavewoman's>
     * || <your lawful female gnome's>
     * || <your lawful female character's>
     *
     * Now append the post attributes to it
     */
    num_post_attribs = post_attribs;
    if (!num_post_attribs) {
        /* some constraints might have been mutually exclusive, in which case
           some prompting that would have been omitted is needed after all */
        if (flags.initrole == ROLE_NONE && !pa[BP_ROLE])
            pa[BP_ROLE] = ++post_attribs;
        if (flags.initrace == ROLE_NONE && !pa[BP_RACE])
            pa[BP_RACE] = ++post_attribs;
        if (flags.initalign == ROLE_NONE && !pa[BP_ALIGN])
            pa[BP_ALIGN] = ++post_attribs;
        if (flags.initgend == ROLE_NONE && !pa[BP_GEND])
            pa[BP_GEND] = ++post_attribs;
        num_post_attribs = post_attribs;
    }
    if (num_post_attribs) {
        if (pa[BP_RACE]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "race");
        }
        if (pa[BP_ROLE]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "role");
        }
        if (pa[BP_GEND]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "gender");
        }
        if (pa[BP_ALIGN]) {
            (void) promptsep(eos(buf), num_post_attribs);
            Strcat(buf, "alignment");
        }
    }
    Strcat(buf, " for you?");
    if(add_ynq)
        Strcat(buf, " [ynaq] ");
    return buf;
}

#undef BP_ALIGN
#undef BP_GEND
#undef BP_RACE
#undef BP_ROLE
#undef NUM_BP

void
plnamesuffix()
{
    char *sptr, *eptr;
    int i;

    /* some generic user names will be ignored in favor of prompting */
    if (sysopt.genericusers) {
        if (*sysopt.genericusers == '*') {
            *plname = '\0';
        } else {
            i = (int) strlen(plname);
            if ((sptr = strstri(sysopt.genericusers, plname)) != 0
                && (sptr == sysopt.genericusers || sptr[-1] == ' ')
                && (sptr[i] == ' ' || sptr[i] == '\0'))
                *plname = '\0'; /* call askname() */
        }
    }

    do {
        if (!*plname)
            askname(); /* fill plname[] if necessary, or set defer_plname */

        /* Look for tokens delimited by '-' */
        if ((eptr = index(plname, '-')) != (char *) 0)
            *eptr++ = '\0';
        while (eptr) {
            /* Isolate the next token */
            sptr = eptr;
            if ((eptr = index(sptr, '-')) != (char *) 0)
                *eptr++ = '\0';

            /* Try to match it to something */
            if ((i = str2role(sptr)) != ROLE_NONE)
                flags.initrole = i;
            else if ((i = str2race(sptr)) != ROLE_NONE)
                flags.initrace = i;
            else if ((i = str2gend(sptr)) != ROLE_NONE)
                flags.initgend = i;
            else if ((i = str2align(sptr)) != ROLE_NONE)
                flags.initalign = i;
        }
    } while (!*plname && !iflags.defer_plname);

    /* commas in the plname confuse the record file, convert to spaces */
    for (sptr = plname; *sptr; sptr++) {
        if (*sptr == ',')
            *sptr = ' ';
    }
}

/* show current settings for name, role, race, gender, and alignment
   in the specified window */
void
role_selection_prolog(which, where)
int which;
winid where;
{
    static const char NEARDATA choosing[] = " choosing now",
                               not_yet[] = " not yet specified",
                               rand_choice[] = " random";
    char buf[BUFSZ];
    int r, c, g, a;
    unsigned long allowmask;

    r = flags.initrole;
    c = flags.initrace;
    g = flags.initgend;
    a = flags.initalign;
    if (r >= 0) 
    {
        allowmask = roles[r].allow;
        if ((allowmask & ROLE_RACEMASK) == MH_HUMAN)
            c = 0; /* races[human] */
        else if (c >= 0 && !(allowmask & ROLE_RACEMASK & races[c].allow))
            c = ROLE_RANDOM;
        if ((allowmask & ROLE_GENDMASK) == ROLE_MALE)
            g = 0; /* role forces male (hypothetical) */
        else if ((allowmask & ROLE_GENDMASK) == ROLE_FEMALE)
            g = 1; /* role forces female (valkyrie) */
        if ((allowmask & ROLE_ALIGNMASK) == AM_LAWFUL)
            a = 0; /* aligns[lawful] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_NEUTRAL)
            a = 1; /* aligns[neutral] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_CHAOTIC)
            a = 2; /* alings[chaotic] */
    }
    if (c >= 0) {
        allowmask = races[c].allow;
        if ((allowmask & ROLE_ALIGNMASK) == AM_LAWFUL)
            a = 0; /* aligns[lawful] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_NEUTRAL)
            a = 1; /* aligns[neutral] */
        else if ((allowmask & ROLE_ALIGNMASK) == AM_CHAOTIC)
            a = 2; /* alings[chaotic] */
        /* [c never forces gender] */
    }
    /* [g and a don't constrain anything sufficiently
       to narrow something done to a single choice] */

    Sprintf(buf, "%12s ", "name:");
    Strcat(buf, (which == RS_NAME) ? choosing : !*plname ? not_yet : plname);
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "role:");
    Strcat(buf, (which == RS_ROLE) ? choosing : (r == ROLE_NONE)
                                                    ? not_yet
                                                    : (r == ROLE_RANDOM)
                                                          ? rand_choice
                                                          : roles[r].name.m);
    if (r >= 0 && roles[r].name.f) {
        /* distinct female name [caveman/cavewoman, priest/priestess] */
        if (g == 1)
            /* female specified; replace male role name with female one */
            Sprintf(index(buf, ':'), ": %s", roles[r].name.f);
        else if (g < 0)
            /* gender unspecified; append slash and female role name */
            Sprintf(eos(buf), "/%s", roles[r].name.f);
    }
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "race:");
    Strcat(buf, (which == RS_RACE) ? choosing : (c == ROLE_NONE)
                                                    ? not_yet
                                                    : (c == ROLE_RANDOM)
                                                          ? rand_choice
                                                          : races[c].noun);
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "gender:");
    Strcat(buf, (which == RS_GENDER) ? choosing : (g == ROLE_NONE)
                                                      ? not_yet
                                                      : (g == ROLE_RANDOM)
                                                            ? rand_choice
                                                            : genders[g].adj);
    putstr(where, 0, buf);
    Sprintf(buf, "%12s ", "alignment:");
    Strcat(buf, (which == RS_ALGNMNT) ? choosing : (a == ROLE_NONE)
                                                       ? not_yet
                                                       : (a == ROLE_RANDOM)
                                                             ? rand_choice
                                                             : aligns[a].adj);
    putstr(where, 0, buf);
}

/* add a "pick alignment first"-type entry to the specified menu */
void
role_menu_extra(which, where, preselect)
int which;
winid where;
boolean preselect;
{
    static NEARDATA const char RS_menu_let[] = {
        '=',  /* name */
        '?',  /* role */
        '/',  /* race */
        '\"', /* gender */
        '[',  /* alignment */
    };
    anything any;
    char buf[BUFSZ];
    const char *what = 0, *constrainer = 0, *forcedvalue = 0;
    int f = 0, r, c, g, a, i;
    unsigned long allowmask;

    r = flags.initrole;
    c = flags.initrace;
    switch (which) {
    case RS_NAME:
        what = "name";
        break;
    case RS_ROLE:
        what = "role";
        f = r;
        for (i = 0; i < SIZE(roles); ++i)
            if (i != f && !rfilter.roles[i])
                break;
        if (i == SIZE(roles)) {
            constrainer = "filter";
            forcedvalue = "role";
        }
        break;
    case RS_RACE:
        what = "race";
        f = flags.initrace;
        c = ROLE_NONE; /* override player's setting */
        if (r >= 0) {
            allowmask = roles[r].allow & ROLE_RACEMASK;
            if (allowmask == MH_HUMAN)
                c = 0; /* races[human] */
            if (c >= 0) {
                constrainer = "role";
                forcedvalue = races[c].noun;
            } else if (f >= 0
                       && (allowmask & ~rfilter.mask) == races[f].selfmask) {
                /* if there is only one race choice available due to user
                   options disallowing others, race menu entry is disabled */
                constrainer = "filter";
                forcedvalue = "race";
            }
        }
        break;
    case RS_GENDER:
        what = "gender";
        f = flags.initgend;
        g = ROLE_NONE;
        if (r >= 0) {
            allowmask = roles[r].allow & ROLE_GENDMASK;
            if (allowmask == ROLE_MALE)
                g = 0; /* genders[male] */
            else if (allowmask == ROLE_FEMALE)
                g = 1; /* genders[female] */
            if (g >= 0) {
                constrainer = "role";
                forcedvalue = genders[g].adj;
            } else if (f >= 0
                       && (allowmask & ~rfilter.mask) == genders[f].allow) {
                /* if there is only one gender choice available due to user
                   options disallowing other, gender menu entry is disabled */
                constrainer = "filter";
                forcedvalue = "gender";
            }
        }
        break;
    case RS_ALGNMNT:
        what = "alignment";
        f = flags.initalign;
        a = ROLE_NONE;
        if (r >= 0) {
            allowmask = roles[r].allow & ROLE_ALIGNMASK;
            if (allowmask == AM_LAWFUL)
                a = 0; /* aligns[lawful] */
            else if (allowmask == AM_NEUTRAL)
                a = 1; /* aligns[neutral] */
            else if (allowmask == AM_CHAOTIC)
                a = 2; /* aligns[chaotic] */
            if (a >= 0)
                constrainer = "role";
        }
        if (c >= 0 && !constrainer) {
            allowmask = races[c].allow & ROLE_ALIGNMASK;
            if (allowmask == AM_LAWFUL)
                a = 0; /* aligns[lawful] */
            else if (allowmask == AM_NEUTRAL)
                a = 1; /* aligns[neutral] */
            else if (allowmask == AM_CHAOTIC)
                a = 2; /* aligns[chaotic] */
            if (a >= 0)
                constrainer = "race";
        }
        if (f >= 0 && !constrainer
            && (ROLE_ALIGNMASK & ~rfilter.mask) == aligns[f].allow) {
            /* if there is only one alignment choice available due to user
               options disallowing others, algn menu entry is disabled */
            constrainer = "filter";
            forcedvalue = "alignment";
        }
        if (a >= 0)
            forcedvalue = aligns[a].adj;
        break;
    }

    any = zeroany; /* zero out all bits */
    if (constrainer) {
        any.a_int = 0;
        /* use four spaces of padding to fake a grayed out menu choice */
        Sprintf(buf, "%4s%s forces %s", "", constrainer, forcedvalue);
        add_menu(where, NO_GLYPH, &any, 0, 0, ATR_NONE, buf,
                 MENU_UNSELECTED);
    } else if (what) {
        any.a_int = RS_menu_arg(which);
        Sprintf(buf, "Pick%s %s first", (f >= 0) ? " another" : "", what);
        add_menu(where, NO_GLYPH, &any, RS_menu_let[which], 0, ATR_NONE, buf,
                 MENU_UNSELECTED);
    } else if (which == RS_filter) {
        any.a_int = RS_menu_arg(RS_filter);
        add_menu(where, NO_GLYPH, &any, '~', 0, ATR_NONE,
                 "Reset role/race/&c filtering", MENU_UNSELECTED);
    } else if (which == ROLE_RANDOM) {
        any.a_int = ROLE_RANDOM;
        add_menu(where, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random",
                 preselect ? MENU_SELECTED : MENU_UNSELECTED);
    } else if (which == ROLE_NONE) {
        any.a_int = ROLE_NONE;
        add_menu(where, NO_GLYPH, &any, 'q', 0, ATR_NONE, "Quit",
                 preselect ? MENU_SELECTED : MENU_UNSELECTED);
    } else {
        impossible("role_menu_extra: bad arg (%d)", which);
    }
}

/*
 *      Special setup modifications here:
 *
 *      Unfortunately, this is going to have to be done
 *      on each newgame or restore, because you lose the permonst mods
 *      across a save/restore.  :-)
 *
 *      1 - The Rogue Leader is the Tourist Nemesis.
 *      2 - Priests start with a random alignment - convert the leader and
 *          guardians here.
 *      3 - Priests also get their of deities from a randomly chosen role.
 *      4 - [obsolete] Elves can have one of two different leaders,
 *          but can't work it out here because it requires hacking the
 *          level file data (see sp_lev.c).
 *
 * This code also replaces quest_init().
 */
void
role_init()
{
    int alignmnt;
    struct permonst *pm;

    /* Strip the role letter out of the player name.
     * This is included for backwards compatibility.
     */
    plnamesuffix();

    /* Check for a valid role.  Try flags.initrole first. */
    if (!validrole(flags.initrole)) {
        /* Try the player letter second */
        if ((flags.initrole = str2role(pl_character)) < 0)
            /* None specified; pick a random role */
            flags.initrole = randrole_filtered();
    }

    /* We now have a valid role index.  Copy the role name back. */
    /* This should become OBSOLETE */
    Strcpy(pl_character, roles[flags.initrole].name.m);
    pl_character[PL_CSIZ - 1] = '\0';

    /* Check for a valid race */
    if (!validrace(flags.initrole, flags.initrace))
        flags.initrace = randrace(flags.initrole);

    /* Check for a valid gender.  If new game, check both initgend
     * and female.  On restore, assume flags.female is correct. */
    if (flags.pantheon == -1) { /* new game */
        if (!validgend(flags.initrole, flags.initrace, flags.female))
            flags.female = !flags.female;
    }
    if (!validgend(flags.initrole, flags.initrace, flags.initgend))
        /* Note that there is no way to check for an unspecified gender. */
        flags.initgend = flags.female;

    /* Check for a valid alignment */
    if (!validalign(flags.initrole, flags.initrace, flags.initalign))
        /* Pick a random alignment */
        flags.initalign = randalign(flags.initrole, flags.initrace);
    alignmnt = aligns[flags.initalign].value;

    /* Initialize urole and urace */
    urole = roles[flags.initrole];
    urace = races[flags.initrace];

    /* Fix up the quest leader */
    if (urole.ldrnum != NON_PM) {
        pm = &mons[urole.ldrnum];
        pm->msound = MS_LEADER;
        pm->mflags2 |= (M2_PEACEFUL);
        pm->mflags3 |= M3_CLOSE;
        pm->maligntyp = alignmnt * 3;
        /* if gender is random, we choose it now instead of waiting
           until the leader monster is created */
        quest_status.ldrgend =
            is_neuter(pm) ? 2 : is_female(pm) ? 1 : is_male(pm)
                                                        ? 0
                                                        : (rn2(100) < 50);
    }

    /* Fix up the quest guardians */
    if (urole.guardnum != NON_PM) {
        pm = &mons[urole.guardnum];
        pm->mflags2 |= (M2_PEACEFUL);
        pm->maligntyp = alignmnt * 3;
    }

    /* Fix up the quest nemesis */
    if (urole.neminum != NON_PM) {
        pm = &mons[urole.neminum];
        pm->msound = MS_NEMESIS;
        pm->mflags2 &= ~(M2_PEACEFUL);
        pm->mflags2 |= (M2_NASTY | M2_STALK | M2_HOSTILE);
        pm->mflags3 &= ~(M3_CLOSE);
        pm->mflags3 |= M3_WANTSARTI | M3_WAITFORU;
        /* if gender is random, we choose it now instead of waiting
           until the nemesis monster is created */
        quest_status.nemgend = is_neuter(pm) ? 2 : is_female(pm) ? 1
                                   : is_male(pm) ? 0 : (rn2(100) < 50);
    }

    /* Fix up the god names */
    if (flags.pantheon == -1) {             /* new game */
        flags.pantheon = flags.initrole;    /* use own gods */
        while (!roles[flags.pantheon].lgod) /* unless they're missing */
            flags.pantheon = randrole(FALSE);
    }
    if (!urole.lgod) {
        urole.lgod = roles[flags.pantheon].lgod;
        urole.ngod = roles[flags.pantheon].ngod;
        urole.cgod = roles[flags.pantheon].cgod;
    }
    /* 0 or 1; no gods are neuter, nor is gender randomized */
    quest_status.godgend = !strcmpi(align_gtitle(alignmnt), "goddess");

#if 0
/*
 * Disable this fixup so that mons[] can be const.  The only
 * place where it actually matters for the hero is in set_uasmon()
 * and that can use mons[race] rather than mons[role] for this
 * particular property.  Despite the comment, it is checked--where
 * needed--via intrinsic 'Infravision' which set_uasmon() manages.
 */
    /* Fix up infravision */
    if (mons[urace.monsternum].mflags3 & M3_INFRAVISION) {
        /* although an infravision intrinsic is possible, infravision
         * is purely a property of the physical race.  This means that we
         * must put the infravision flag in the player's current race
         * (either that or have separate permonst entries for
         * elven/non-elven members of each class).  The side effect is that
         * all NPCs of that class will have (probably bogus) infravision,
         * but since infravision has no effect for NPCs anyway we can
         * ignore this.
         */
        mons[urole.monsternum].mflags3 |= M3_INFRAVISION;
    }
#endif /*0*/

    /* Artifacts are fixed in hack_artifacts() */

    /* Success! */
    return;
}

const char *
Hello(mtmp)
struct monst *mtmp;
{
    switch (Role_switch) {
    case PM_KNIGHT:
        return "Salutations"; /* Olde English */
    case PM_SAMURAI:
        return (mtmp && mtmp->isshk)
                    ? "Irasshaimase"
                    : "Konnichi wa"; /* Japanese */
    case PM_TOURIST:
        return "Aloha"; /* Hawaiian */
    case PM_VALKYRIE:
        return
               (mtmp && mtmp->data == &mons[PM_MAIL_DAEMON]) ? "Hallo" :
               "Velkommen"; /* Norse */
    default:
        return "Hello";
    }
}

const char *
Goodbye()
{
    switch (Role_switch) {
    case PM_KNIGHT:
        return "Fare thee well"; /* Olde English */
    case PM_SAMURAI:
        return "Sayonara"; /* Japanese */
    case PM_TOURIST:
        return "Aloha"; /* Hawaiian */
    case PM_VALKYRIE:
        return "Farvel"; /* Norse */
    default:
        return "Goodbye";
    }
}

int
u_to_glyph()
{

    if (Upolyd || (uarmo && uarmo->otyp == MUMMY_WRAPPING))
    {
        int mnum = Upolyd ? u.umonnum : (uarmo && uarmo->otyp == MUMMY_WRAPPING) ? urace.mummynum : u.umonster;
        return (u.facing_right ? -1 : 1) * 
            (
                u.action == ACTION_TILE_ATTACK ? (flags.female ? female_attacking_monnum_to_glyph(mnum) : attacking_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_THROW ? (flags.female ? female_throwing_monnum_to_glyph(mnum) : throwing_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_FIRE ? (flags.female ? female_firing_monnum_to_glyph(mnum) : firing_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_CAST_NODIR ? (flags.female ? female_nodir_casting_monnum_to_glyph(mnum) : nodir_casting_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_CAST_DIR ? (flags.female ? female_dir_casting_monnum_to_glyph(mnum) : dir_casting_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_SPECIAL_ATTACK ? (flags.female ? female_special_attacking_monnum_to_glyph(mnum) : special_attacking_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_KICK ? (flags.female ? female_kicking_monnum_to_glyph(mnum) : kicking_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_PASSIVE_DEFENSE ? (flags.female ? female_passive_defense_monnum_to_glyph(mnum) : passive_defense_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_SPECIAL_ATTACK_2 ? (flags.female ? female_special_attacking2_monnum_to_glyph(mnum) : special_attacking2_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_SPECIAL_ATTACK_3 ? (flags.female ? female_special_attacking3_monnum_to_glyph(mnum) : special_attacking3_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_ITEM_USE ? (flags.female ? female_item_using_monnum_to_glyph(mnum) : item_using_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_DOOR_USE ? (flags.female ? female_door_using_monnum_to_glyph(mnum) : door_using_monnum_to_glyph(mnum))
                : u.action == ACTION_TILE_DEATH ? (flags.female ? female_dying_monnum_to_glyph(mnum) : dying_monnum_to_glyph(mnum))
                : ((flags.female ? female_monnum_to_glyph(mnum) : monnum_to_glyph(mnum))));
    }

    int player_role = urole.rolenum;
    int player_race = urace.racenum;
    int player_gender = (int)flags.female;
    int player_alignment = u.ualign.type;
    int player_glyph_level = 0;
    int player_glyph_index = player_to_glyph_index(player_role, player_race, player_gender, player_alignment, player_glyph_level);
    int player_glyph_offset = (
        u.action == ACTION_TILE_ATTACK ? GLYPH_PLAYER_ATTACK_OFF 
        : u.action == ACTION_TILE_THROW ? GLYPH_PLAYER_THROW_OFF
        : u.action == ACTION_TILE_FIRE ? GLYPH_PLAYER_FIRE_OFF
        : u.action == ACTION_TILE_CAST_NODIR ? GLYPH_PLAYER_CAST_NODIR_OFF
        : u.action == ACTION_TILE_CAST_DIR ? GLYPH_PLAYER_CAST_DIR_OFF
        : u.action == ACTION_TILE_SPECIAL_ATTACK ? GLYPH_PLAYER_SPECIAL_ATTACK_OFF
        : u.action == ACTION_TILE_KICK ? GLYPH_PLAYER_KICK_OFF
        : u.action == ACTION_TILE_PASSIVE_DEFENSE ? GLYPH_PLAYER_PASSIVE_DEFENSE_OFF
        : u.action == ACTION_TILE_SPECIAL_ATTACK_2 ? GLYPH_PLAYER_SPECIAL_ATTACK_2_OFF
        : u.action == ACTION_TILE_SPECIAL_ATTACK_3 ? GLYPH_PLAYER_SPECIAL_ATTACK_3_OFF
        : u.action == ACTION_TILE_ITEM_USE ? GLYPH_PLAYER_ITEM_USE_OFF
        : u.action == ACTION_TILE_DOOR_USE ? GLYPH_PLAYER_DOOR_USE_OFF
        : u.action == ACTION_TILE_DEATH ? GLYPH_PLAYER_DEATH_OFF
        : GLYPH_PLAYER_OFF);

    int res = (u.facing_right ? -1 : 1) * (player_glyph_index + player_glyph_offset);

    return res;
}

int
player_to_glyph_index(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    //int number_of_roles = NUM_ROLES;
    int number_of_races = NUM_RACES;
    int number_of_genders = 2;
    int number_of_alignments = 3;
    int number_of_glyph_levels = NUM_PLAYER_GLYPH_LEVELS;

    int player_role = roleidx;
    int player_race = raceidx;
    int player_gender = genderidx;
    int player_alignment = alignmentidx + 1; /* 0...2 */
    int player_glyph_level = levelidx;

    int res = player_glyph_level +
        number_of_glyph_levels * player_alignment +
        number_of_glyph_levels * number_of_alignments * player_gender +
        number_of_glyph_levels * number_of_alignments * number_of_genders * player_race +
        number_of_glyph_levels * number_of_alignments * number_of_genders * number_of_races * player_role;

    return res;
}


boolean
player_has_action_tile(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* There's always a stand tile */
    if (action == ACTION_TILE_NO_ACTION)
        return TRUE;

    if (levelidx > 0)
        return FALSE;

    /* All characters have attack and kick tiles */
    if (action == ACTION_TILE_ATTACK || action == ACTION_TILE_KICK)
        return TRUE;

    /* Rangers have fire tiles */
    if (action == ACTION_TILE_FIRE)
    {
        if (roleidx == ROLE_RANGER)
            return TRUE;
    }

    /* All spell casters have cast indir and cast dir tiles*/
    if (action == ACTION_TILE_CAST_NODIR || action == ACTION_TILE_CAST_DIR)
    {
        if(roleidx == ROLE_WIZARD || roleidx == ROLE_PRIEST || roleidx == ROLE_HEALER || roleidx == ROLE_MONK)
            return TRUE;
    }

    /* Other cases */
    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_SAMURAI && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return TRUE;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF)
            return TRUE;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return TRUE;
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF)
            return TRUE;
        break;
    case ACTION_TILE_CAST_NODIR:
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN)
            return TRUE;
        break;
    case ACTION_TILE_CAST_DIR:
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return TRUE;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN)
            return TRUE;
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN)
            return TRUE;
        break;
    case ACTION_TILE_KICK:
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_KNIGHT && raceidx == RACE_DWARF)
            return TRUE;
        if (roleidx == ROLE_MONK && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_SAMURAI && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_PRIEST && raceidx == RACE_ELF && genderidx == GENDER_MALE && alignmentidx == A_CHAOTIC)
            return TRUE;
        if (roleidx == ROLE_ROGUE && raceidx == RACE_ORC && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_ORC && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_BARBARIAN && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_HUMAN && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_VALKYRIE && raceidx == RACE_DWARF && genderidx == GENDER_FEMALE)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_GNOLL && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_RANGER && raceidx == RACE_ELF)
            return TRUE;
        if (roleidx == ROLE_WIZARD && raceidx == RACE_HUMAN && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_CAVEMAN && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return TRUE;
        if (roleidx == ROLE_ARCHAEOLOGIST && raceidx == RACE_DWARF && genderidx == GENDER_MALE)
            return TRUE;
        break;
    case ACTION_TILE_PASSIVE_DEFENSE:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_2:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_3:
        break;
    case ACTION_TILE_ITEM_USE:
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN)
            return TRUE;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN)
            return TRUE;
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }

    return FALSE;
}

unsigned long
get_player_action_flags(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx UNUSED, alignmentidx UNUSED, levelidx;
{
    /* There's always a stand tile */
    if (action == ACTION_TILE_NO_ACTION)
        return ACTION_ITEM_USE_FLAGS_NONE;

    if (levelidx > 0)
        return ACTION_ITEM_USE_FLAGS_NONE;

    /* Real cases */
    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        if (roleidx == ROLE_ARCHAEOLOGIST)
            return ACTION_ATTACK_FLAGS_PICK_AXE;
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        break;
    case ACTION_TILE_CAST_NODIR:
        break;
    case ACTION_TILE_CAST_DIR:
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN)
            return ACTION_SPECIAL_ATTACK_FLAGS_CAMERA;
        break;
    case ACTION_TILE_KICK:
        break;
    case ACTION_TILE_PASSIVE_DEFENSE:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_2:
        break;
    case ACTION_TILE_SPECIAL_ATTACK_3:
        break;
    case ACTION_TILE_ITEM_USE:
        if (roleidx == ROLE_TOURIST && raceidx == RACE_HUMAN)
            return ACTION_ITEM_USE_FLAGS_CAMERA;
        if (roleidx == ROLE_HEALER && raceidx == RACE_HUMAN)
            return ACTION_ITEM_USE_FLAGS_POTION;
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }

    return ACTION_ITEM_USE_FLAGS_NONE;
}

unsigned long
u_action_flags(action)
enum action_tile_types action; 
{
    return Upolyd ? 0UL /* Monster action flags here */ :
        get_player_action_flags(action, urole.rolenum, urace.racenum, flags.female, u.ualign.type + 1, 0);
}

unsigned long 
u_item_use_flags()
{
    return u_action_flags(ACTION_TILE_ITEM_USE);
}

int
glyph_to_player_mon(int glyph)
{
    int offset = glyph - GLYPH_PLAYER_OFF;

    //int glyph_level_idx = NUM_PLAYER_GLYPH_LEVELS > 1 ? (offset % NUM_PLAYER_GLYPH_LEVELS) : 0;
    //int alignment_idx = (offset % (3 * NUM_PLAYER_GLYPH_LEVELS)) / NUM_PLAYER_GLYPH_LEVELS;
    //int gender_idx = (offset % (2 * 3 * NUM_PLAYER_GLYPH_LEVELS)) / (3 * NUM_PLAYER_GLYPH_LEVELS);
    int race_idx = (offset % (2 * 3 * NUM_PLAYER_GLYPH_LEVELS * NUM_RACES)) / (2 * 3 * NUM_PLAYER_GLYPH_LEVELS);
    int role_idx = (offset % (2 * 3 * NUM_PLAYER_GLYPH_LEVELS * NUM_RACES * NUM_ROLES)) / (2 * 3 * NUM_PLAYER_GLYPH_LEVELS * NUM_RACES);

    if (flags.showrace)
    {
        if(race_idx >= 0 && race_idx < NUM_RACES)
            return races[race_idx].monsternum;
        else
            return urace.monsternum;
    }
    else
    {
        if (role_idx >= 0 && role_idx < NUM_ROLES)
            return roles[role_idx].monsternum;
        else
            return urole.monsternum;
    }
}

int
attack_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_ATTACK_OFF + GLYPH_PLAYER_OFF);
}

int
throw_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_THROW_OFF + GLYPH_PLAYER_OFF);
}

int
fire_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_FIRE_OFF + GLYPH_PLAYER_OFF);
}

int
nodir_cast_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_CAST_NODIR_OFF + GLYPH_PLAYER_OFF);
}

int
dir_cast_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_CAST_DIR_OFF + GLYPH_PLAYER_OFF);
}

int
special_attack_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_SPECIAL_ATTACK_OFF + GLYPH_PLAYER_OFF);
}

int
kick_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_KICK_OFF + GLYPH_PLAYER_OFF);
}


int
item_use_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_ITEM_USE_OFF + GLYPH_PLAYER_OFF);
}

int
door_use_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_DOOR_USE_OFF + GLYPH_PLAYER_OFF);
}

int
death_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_DEATH_OFF + GLYPH_PLAYER_OFF);
}

int
special_attack3_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_SPECIAL_ATTACK_3_OFF + GLYPH_PLAYER_OFF);
}

int
passive_defense_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_PASSIVE_DEFENSE_OFF + GLYPH_PLAYER_OFF);
}

int
special_attack2_glyph_to_player_mon(int glyph)
{
    return glyph_to_player_mon(glyph - GLYPH_PLAYER_SPECIAL_ATTACK_2_OFF + GLYPH_PLAYER_OFF);
}

/* role.c */

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    drawing.c    $NHDT-Date: 1546656404 2019/01/05 02:46:44 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.59 $ */
/* Copyright (c) GnollHack Development Team 1992.                   */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "tcap.h"

/* Relevant header information in rm.h, objclass.h, and monsym.h. */

#ifdef C
#undef C
#endif

#ifdef TEXTCOLOR
#define C(n) n
#else
#define C(n)
#endif

struct symsetentry symset[NUM_GRAPHICS];

int currentgraphics = 0;

nhsym showsyms[SYM_MAX] = DUMMY; /* symbols to be displayed */
nhsym l_syms[SYM_MAX] = DUMMY;   /* loaded symbols          */
nhsym r_syms[SYM_MAX] = DUMMY;   /* rogue symbols           */

nhsym warnsyms[WARNCOUNT] = DUMMY; /* the current warning display symbols */
const char invisexplain[] = "remembered, unseen, creature",
           altinvisexplain[] = "unseen creature"; /* for clairvoyance */

/* Default object class symbols.  See objclass.h.
 * {symbol, name, explain}
 *     name:    used in object_detect().
 *     explain: used in do_look().
 */
const struct class_sym def_oc_syms[MAX_OBJECT_CLASSES] = {
    { '\0', "", "" }, /* placeholder for the "random class" */
    { ILLOBJ_SYM, "illegal objects", "strange object" },
    { WEAPON_SYM, "weapons", "weapon" },
    { ARMOR_SYM, "armor", "suit or piece of armor" },
    { RING_SYM, "rings", "ring" },
    { AMULET_SYM, "amulets", "amulet" },
    { TOOL_SYM, "tools", "tool" }, //useful item (pick-axe, key, lamp...)
    { FOOD_SYM, "food", "piece of food" },
    { POTION_SYM, "potions", "potion" },
    { SCROLL_SYM, "scrolls", "scroll" },
    { SPBOOK_SYM, "spellbooks", "spellbook" },
    { WAND_SYM, "wands", "wand" },
    { GOLD_SYM, "coins", "pile of coins" },
    { GEM_SYM, "rocks", "gem or rock" },
    { ROCK_SYM, "large stones", "boulder or statue" },
    { BALL_SYM, "iron balls", "iron ball" },
    { CHAIN_SYM, "chains", "iron chain" },
    { VENOM_SYM, "venoms", "splash of venom" },
    { REAGENT_SYM, "reagents", "reagent" },
    { MISCELLANEOUS_SYM, "miscellaneous items", "miscellaneous item" },
    { ART_SYM, "art objects", "art object" }
};

/* Default monster class symbols.  See monsym.h. */
const struct class_sym def_monsyms[MAX_MONSTER_CLASSES] = {
    { '\0', "", "" },
    { DEF_ANT, "ants and other insects", "ant or other insect" },
    { DEF_BLOB, "blobs", "blob" },
    { DEF_COCKATRICE, "cockatrices", "cockatrice" },
    { DEF_DOG, "dogs, other canines, and hyenas", "dog, other canine, or hyena" },
    { DEF_EYE, "eyes and spheres", "eye or sphere" },
    { DEF_FELINE, "cats and other felines", "cat or other feline" },
    { DEF_GNOME, "gnomes, gremlins, and gargoyles", "gnome, gremlin, or gargoyle" },
    { DEF_HUMANOID, "dwarves, humanoids, and tentacled ones", "dwarf, humanoid, or tentacled one" },
    { DEF_IMP, "imps and other minor demons", "imp or minor demon" },
    { DEF_JELLY, "jellies", "jelly" },
    { DEF_KOBOLD, "kobolds", "kobold" },
    { DEF_LEPRECHAUN, "leprechauns", "leprechaun" },
    { DEF_MIMIC, "mimics", "mimic" },
    { DEF_NYMPH, "nymphs", "nymph" },
    { DEF_ORC, "orcs", "orc" },
    { DEF_PIERCER, "piercers", "piercer" },
    { DEF_QUADRUPED, "quadrupeds", "quadruped" },
    { DEF_RODENT, "rodents", "rodent" },
    { DEF_SPIDER, "arachnids and centipedes", "arachnid or centipede" },
    { DEF_TRAPPER, "trappers and lurkers above", "trapper or lurker above" },
    { DEF_UNICORN, "unicorns and horses", "unicorn or horse" },
    { DEF_VORTEX, "vortices", "vortex" },
    { DEF_WORM, "worms", "worm" },
    { DEF_XAN, "xans and other mythical/fantastic insects", "xan or other mythical/fantastic insect" },
    { DEF_LIGHT, "lights", "light" },
    { DEF_LESSER_UNDEAD, "zombies, skeletons, and other lesser undead", "zombie, skeleton, or other lesser undead" },
    { DEF_ANGEL, "angelic beings", "angelic being" },
    { DEF_BAT, "bats and birds", "bat or bird" },
    { DEF_CENTAUR, "centaurs", "centaur" },
    { DEF_DRAGON, "dragons", "dragon" },
    { DEF_ELEMENTAL, "elementals", "elemental" },
    { DEF_FUNGUS, "fungi and molds", "fungus or mold" },
    { DEF_GNOLL, "gnolls", "gnoll" },
    { DEF_GIANT, "giant humanoids", "giant humanoid" },
    { '\0', "invisible monsters", "invisible monster" },
    { DEF_JABBERWOCK, "jabberwocks and juggernauts", "jabberwock or juggernaut" },
    { DEF_KOP, "Keystone Kops", "Keystone Kop" },
    { DEF_LICH, "liches", "lich" },
    { DEF_MODRON, "modrons", "modron" },
    { DEF_NAGA, "nagas", "naga" },
    { DEF_OGRE, "ogres", "ogre" },
    { DEF_PUDDING, "puddings and oozes", "pudding or ooze" },
    { DEF_QUANTMECH, "quantum mechanics", "quantum mechanic" },
    { DEF_RUSTMONST, "rust monsters and disenchanters", "rust monster or disenchanter" },
    { DEF_SNAKE, "snakes", "snake" },
    { DEF_TROLL, "trolls and monsters with similar powers", "troll or monster with similar powers" },
    { DEF_UNDER, "underworld hulks, otyughs, or chimeras", "umbral hulk, otyugh, or chimera" },
    { DEF_VAMPIRE, "vampires", "vampire" },
    { DEF_WRAITH, "wraiths", "wraith" },
    { DEF_XORN, "xorns", "xorn" },
    { DEF_YETI, "ape- and bear-like creatures", "ape- or bear-like creature" },
    { DEF_GREATER_UNDEAD, "mummies and other greater undead", "mummy or other greater undead" },
    { DEF_HUMAN, "humans and elves", "human or elf" },
    { DEF_GHOST, "ghosts", "ghost" },
    { DEF_GOLEM, "golems", "golem" },
    { DEF_DEMON, "major demons and devils", "major demon or devil" },
    { DEF_EEL, "sea monsters", "sea monster" },
    { DEF_LIZARD, "lizards", "lizard" },
    { DEF_TREANT, "treants", "treant" },
    { DEF_WORM_TAIL, "long worm tails", "long worm tail" },
    { DEF_MIMIC_DEF, "mimics", "mimic" },
};

const struct symdef def_warnsyms[WARNCOUNT] = {
    /* normal red warning  */
    { '0', "unknown creature causing you little worry",    C(CLR_RED) },
    /* normal red warning   */
    { '1', "unknown creature causing you worry",  C(CLR_RED) },
    /* normal red warning    */
    { '2', "unknown creature causing you concern",  C(CLR_RED) },
    /* normal red warning   */
    { '3', "unknown creature causing you anxiety", C(CLR_RED) },
    /* normal red warning   */
    { '4', "unknown creature causing you disquiet", C(CLR_RED) },
    /* magenta warning */
    { '5', "unknown creature causing you alarm",    C(CLR_MAGENTA) },
    /* bright magenta warning  */
    { '6', "unknown creature causing you dread",    C(CLR_BRIGHT_MAGENTA) },
};


/*
 *  Default screen symbols with explanations and colors.
 */
/* You can define a new screen symbol / tile as a CMAP symbol, CMAP variation, or a GUI replacement tile */
/* NOTE: Use CMAP symbols for things that can have their own base symbols in TTY, e.g. walls. CMAP variations are color variations of the base tile with their own descriptions and GUI tiles. Replacements are tiles that show up only in GUI. */
const struct symdef_cmap defsyms[MAX_CMAPPED_CHARS] = {
/* 0*/ { ' ', "unexplored", "",          UNDEFINED_LOCATION, LAYER_FLOOR,   FALSE,     0, 0,
            { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/  C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0}},  /* unexplored */
/* 1*/ { ' ', "stone", "",               STONE, LAYER_FLOOR,   FALSE,     STONE_VARIATIONS, STONE_VARIATION_OFFSET,
            { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/  C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_STONE_REPLACEMENT, GNOMISH_MINES_STONE_REPLACEMENT, GEHENNOM_STONE_REPLACEMENT, DUNGEON_UNDEAD_STONE_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_STONE_REPLACEMENT, GARDEN_STONE_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0}},  /* stone */
       { '|', "wall", "",                VWALL, LAYER_FEATURE, FALSE,     VWALL_VARIATIONS, VWALL_VARIATION_OFFSET,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_VWALL_REPLACEMENT, GNOMISH_MINES_VWALL_REPLACEMENT, GEHENNOM_VWALL_REPLACEMENT, DUNGEON_UNDEAD_VWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_VWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* vwall */
       { '-', "wall", "",                HWALL, LAYER_FEATURE, FALSE,      HWALL_VARIATIONS, HWALL_VARIATION_OFFSET,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_HWALL_REPLACEMENT, GNOMISH_MINES_HWALL_REPLACEMENT, GEHENNOM_HWALL_REPLACEMENT, DUNGEON_UNDEAD_HWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_HWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* hwall */
       { '-', "wall", "",                TLCORNER, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_TLCORN_REPLACEMENT, GNOMISH_MINES_TLCORN_REPLACEMENT, GEHENNOM_TLCORN_REPLACEMENT, DUNGEON_UNDEAD_TLCORN_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_TLCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tlcorn */
       { '-', "wall", "",                TRCORNER, LAYER_FEATURE, FALSE,     0, 0,     
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_TRCORN_REPLACEMENT, GNOMISH_MINES_TRCORN_REPLACEMENT, GEHENNOM_TRCORN_REPLACEMENT, DUNGEON_UNDEAD_TRCORN_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_TRCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* trcorn */
       { '-', "wall", "",                BLCORNER, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_BLCORN_REPLACEMENT, GNOMISH_MINES_BLCORN_REPLACEMENT, GEHENNOM_BLCORN_REPLACEMENT, DUNGEON_UNDEAD_BLCORN_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_BLCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* blcorn */
       { '-', "wall", "",                BRCORNER, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_BRCORN_REPLACEMENT, GNOMISH_MINES_BRCORN_REPLACEMENT, GEHENNOM_BRCORN_REPLACEMENT, DUNGEON_UNDEAD_BRCORN_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_BRCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* brcorn */
       { '-', "wall", "",                CROSSWALL, LAYER_FEATURE, FALSE,     CRWALL_VARIATIONS, CRWALL_VARIATION_OFFSET,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_CRWALL_REPLACEMENT, GNOMISH_MINES_CRWALL_REPLACEMENT, GEHENNOM_CRWALL_REPLACEMENT, DUNGEON_UNDEAD_CRWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_CRWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* crwall */
       { '-', "wall", "",                TUWALL, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_TUWALL_REPLACEMENT, GNOMISH_MINES_TUWALL_REPLACEMENT, GEHENNOM_TUWALL_REPLACEMENT, DUNGEON_UNDEAD_TUWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_TUWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tuwall */
       { '-', "wall", "",                TDWALL, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_TDWALL_REPLACEMENT, GNOMISH_MINES_TDWALL_REPLACEMENT, GEHENNOM_TDWALL_REPLACEMENT, DUNGEON_UNDEAD_TDWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_TDWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tdwall */
/*10*/ { '|', "wall", "",                TLWALL, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_TLWALL_REPLACEMENT, GNOMISH_MINES_TLWALL_REPLACEMENT, GEHENNOM_TLWALL_REPLACEMENT, DUNGEON_UNDEAD_TLWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_TLWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tlwall */
       { '|', "wall", "",                TRWALL, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  {DUNGEON_NORMAL_TRWALL_REPLACEMENT, GNOMISH_MINES_TRWALL_REPLACEMENT, GEHENNOM_TRWALL_REPLACEMENT, DUNGEON_UNDEAD_TRWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_TRWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* trwall */
       { '.', "doorway", "",             DOOR, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_BLACK), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GREEN), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '-', "open door", "",           DOOR, LAYER_FEATURE, FALSE,     VODOOR_VARIATIONS, VODOOR_VARIATION_OFFSET,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },           /* vodoor */
       { '|', "open door", "",           DOOR, LAYER_FEATURE, FALSE,     HODOOR_VARIATIONS, HODOOR_VARIATION_OFFSET,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },           /* hodoor */
       { '+', "closed door", "",         DOOR, LAYER_FEATURE, FALSE,     VCDOOR_VARIATIONS, VCDOOR_VARIATION_OFFSET,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* vcdoor */
       { '+', "closed door", "",         DOOR, LAYER_FEATURE, FALSE,     HCDOOR_VARIATIONS, HCDOOR_VARIATION_OFFSET,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* hcdoor */
       { '.', "broken door",  "",        DOOR, LAYER_FEATURE, FALSE,     VBDOOR_VARIATIONS, VBDOOR_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_BLACK), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GREEN), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '.', "broken door",  "",        DOOR, LAYER_FEATURE, FALSE,     HBDOOR_VARIATIONS, HBDOOR_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_BLACK), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GREEN), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '.', "open portcullis",  "",    DOOR, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),   /*5*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),      /*10*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '.', "open portcullis",  "",    DOOR, LAYER_FEATURE, FALSE,     0, 0,
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),   /*5*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),      /*10*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '#', "iron bars",  "",          IRONBARS, LAYER_FEATURE, FALSE,     0, 0,     
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL),      /*5*/ C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL),      /*10*/C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_IRON_BAR_REPLACEMENT, GNOMISH_MINES_IRON_BAR_REPLACEMENT, GEHENNOM_IRON_BAR_REPLACEMENT, DUNGEON_UNDEAD_IRON_BAR_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_IRON_BAR_REPLACEMENT, GARDEN_IRON_BAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },            /* bars */
       { '#', "tree", "tree stump",      TREE, LAYER_COVER_FEATURE, TRUE,     TREE_VARIATIONS, TREE_VARIATION_OFFSET,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {TREE_ENLARGEMENT, 0, TREE_GEHENNOM_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* tree */
       { '.', "floor of a room", "",    ROOM, LAYER_FLOOR, FALSE,       FLOOR_VARIATIONS, FLOOR_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_ROOM_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
/*20*/ { '.', "dark part of a room", "", ROOM, LAYER_FLOOR, FALSE,       0, 0,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_ROOM_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* dark room */
/*+1*/ { ',', "grass",  "",              GRASS, LAYER_FLOOR, FALSE,       GRASS_VARIATIONS, GRASS_VARIATION_OFFSET,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),/*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GRASS_REPLACEMENT, DUNGEON_NORMAL_GRASS_REPLACEMENT, GEHENNOM_GRASS_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* grass */
/*+1*/ { ',', "grass in dark",  "",      GRASS, LAYER_FLOOR, FALSE,       0, 0,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GRASS_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* grass in dark */
/*+1*/ { ',', "ground",  "",             GROUND, LAYER_FLOOR, FALSE,       GROUND_VARIATIONS, GROUND_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),/*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* grass */
/*+1*/ { ',', "ground in dark",  "",     GROUND, LAYER_FLOOR, FALSE,       0, 0,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* grass in dark */
       { '#', "corridor",  "",           CORR, LAYER_FLOOR, FALSE,       0, 0,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),     /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },             /* dark corr */
       { '#', "lit corridor",  "",       CORR, LAYER_FLOOR, FALSE,       CORRIDOR_VARIATIONS, CORRIDOR_VARIATION_OFFSET,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },   /* lit corr (see mapglyph.c) */
       { '<', "staircase up",  "",       STAIRS, LAYER_FEATURE, FALSE,      STAIRCASE_VARIATIONS, STAIRCASE_UP_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),     /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_UPSTAIRS_ENLARGEMENT, GNOMISH_MINES_UPSTAIRS_ENLARGEMENT, 0, DUNGEON_UNDEAD_UPSTAIRS_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_STAIRS_UP_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* upstair */
       { '>', "staircase down",  "",     STAIRS, LAYER_FEATURE, FALSE,      STAIRCASE_VARIATIONS, STAIRCASE_DOWN_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),     /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_STAIRS_DOWN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* dnstair */
       { '<', "ladder up",  "",          LADDER, LAYER_FEATURE, FALSE,      0, 0,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },           /* upladder */
       { '>', "ladder down",  "",        LADDER, LAYER_FEATURE, FALSE,      0, 0,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* dnladder */
       { '_', "anvil",  "broken anvil",  ANVIL, LAYER_FEATURE, FALSE,      0, 0,
            { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE),      /*5*/ C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE),      /*10*/ C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* anvil */
       { '_', "altar",  "",              ALTAR, LAYER_FEATURE, FALSE,      ALTAR_VARIATIONS, ALTAR_VARIATION_OFFSET,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {UNALIGNED_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
/*+1*/ { '0', "boulder", "some rubble",  ROOM, LAYER_COVER_OBJECT, TRUE,        0, 0,
            { C(CLR_GRAY), C(CLR_BROWN), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BRIGHT_BLUE), /*5*/ C(CLR_WHITE), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0 /*DUNGEON_NORMAL_BOULDER_ENLARGEMENT*/, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { '|', "grave", "open grave",     GRAVE, LAYER_FEATURE, TRUE,      0, 0,     
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* grave */
       { '|', "brazier", "broken brazier", BRAZIER, LAYER_FEATURE, FALSE,      BRAZIER_VARIATIONS, BRAZIER_VARIATION_OFFSET,
            { C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), /*5*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), /*10*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {BRAZIER_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* brazier */
       { '|', "signpost", "broken signpost", SIGNPOST, LAYER_FEATURE, FALSE,      SIGNPOST_VARIATIONS, SIGNPOST_VARIATION_OFFSET,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* signpost */
       { '\\', "opulent throne", "", THRONE, LAYER_FEATURE, FALSE,      0, 0,
            { C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD),           /*5*/C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD),            /*10*/ C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* throne */
/*30*/ { '#', "sink",  "remains of a sink", SINK, LAYER_FEATURE, TRUE,      0, 0,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_RED), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* sink */
       { '{', "fountain", "dried fountain", FOUNTAIN, LAYER_FEATURE, TRUE,      FOUNTAIN_VARIATIONS, FOUNTAIN_VARIATION_OFFSET,
            { C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { '}', "water", "", POOL, LAYER_FLOOR, FALSE,        0, 0,
            { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_WATER_REPLACEMENT, 0, GEHENNOM_WATER_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* pool */
       { '.', "ice", "", ICE, LAYER_FLOOR, FALSE,        0, 0,
           { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_ICE_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                  /* ice */
       { '}', "molten lava", "", LAVAPOOL, LAYER_FLOOR, FALSE,        0, 0,
           { C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_LAVA_REPLACEMENT, 0, GEHENNOM_LAVA_REPLACEMENT, 0, 0, 0, 0, GEHENNOM_CAVERNOUS_LAVA_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },           /* lava */
       { '.', "lowered drawbridge", "", DRAWBRIDGE_DOWN, LAYER_FEATURE, FALSE,      0, 0,
           { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },  /* vodbridge */
       { '.', "lowered drawbridge", "", DRAWBRIDGE_DOWN, LAYER_FEATURE, FALSE,      0, 0,
           { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },  /* hodbridge */
       { '#', "raised drawbridge", "", DRAWBRIDGE_UP,  LAYER_FEATURE, FALSE,      0, 0,
           { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },   /* vcdbridge */
       { '#', "raised drawbridge", "", DRAWBRIDGE_UP,  LAYER_FEATURE, FALSE,      0, 0,
           { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },   /* hcdbridge */
       { ' ', "air", "",  AIR, LAYER_FLOOR, FALSE,        0, 0,
           { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_AIR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                  /* open air */
/*40*/ { '#', "cloud", "",  CLOUD, LAYER_FLOOR, FALSE,      0, 0,
           { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_CLOUD_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* [part of] a cloud */
       { '}', "water", "",  WATER, LAYER_FLOOR, FALSE,        0, 0,     
           { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_WATER2_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* under water */
/* TRAPS START HERE */
       { '^', "arrow trap", "",          ROOM, LAYER_TRAP, FALSE,    ARROW_TRAP_VARIATIONS, ARROW_TRAP_VARIATION_OFFSET,     { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },           /* trap */
       { '^', "dart trap",  "",          ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "falling rock trap", "",   ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },    /* trap */
       { '^', "squeaky board", "",       ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* trap */
       { '^', "bear trap", "",           ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {BEAR_TRAP_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "land mine", "",           ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED) }, 
           {LAND_MINE_ACTIVATED_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },             /* trap */
       { '^', "rolling boulder trap", "", ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
       { '^', "sleeping gas trap", "",   ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* trap */
/*50*/ { '^', "rust trap", "",           ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "fire trap", "",           ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, 
           { FIRE_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },          /* trap */
       { '^', "pit", "",                 ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* trap */
       { '^', "spiked pit", "",          ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },          /* trap */
       { '^', "hole", "",                ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* trap */
       { '^', "trap door", "",           ROOM, LAYER_TRAP, FALSE,    0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },           /* trap */
       { '^', "teleportation trap", "",  ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) }, 
           {TELEPORT_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },  /* trap */
       { '^', "level teleporter", "",    ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) }, 
           {LEVEL_TELEPORT_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },    /* trap */
       { '^', "magic portal", "",        ROOM, LAYER_TRAP, TRUE,    0, 0,
           { C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA) }, 
           {MAGIC_PORTAL_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
       { '"', "web", "remains of a web", ROOM, LAYER_TRAP, TRUE,    0, 0,
           { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                    /* web */
/*60*/ { '^', "statue trap", "",         ROOM, LAYER_TRAP, FALSE,   0, 0,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, 
            {STATUE_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "magic trap", "",          ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, 
           {MAGIC_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* trap */
       { '^', "anti-magic field", "",    ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, 
           {ANTIMAGIC_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* trap */
       { '^', "polymorph trap", "",      ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, 
           {POLYMORPH_TRAP_STAND_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
/*+1*/ { '^', "geometric magic portal", "", ROOM, LAYER_TRAP, FALSE,    MODRON_PORTAL_VARIATIONS, MODRON_PORTAL_VARIATION_OFFSET,
           { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
/*+1*/ { '\\', "lever", "",              ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_LEVER_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /* lever */
       { '~', "vibrating square", "",    ROOM, LAYER_TRAP, FALSE,    0, 0,
           { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) }, 
           {VIBRATING_SQUARE_ANIMATION, VIBRATING_SQUARE_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },    /* "trap" */
/* zap colors are changed by mapglyph() to match type of beam */
       { '|', "", "",               ROOM, LAYER_ZAP, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* vbeam */
       { '-', "", "",               ROOM, LAYER_ZAP, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* hbeam */
       { '\\', "", "",              ROOM, LAYER_ZAP, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* lslant */
       { '/', "", "",               ROOM, LAYER_ZAP, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* rslant */
       { '*', "", "",               ROOM, LAYER_ZAP, FALSE,    0, 0, { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* dig beam */
       { '!', "", "",               ROOM, LAYER_ZAP, FALSE,    0, 0, { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },               /* camera flash beam */
/*+1*/ { '(', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open right */
/*+1*/ { '-', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open right */
       { ')', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open left */
/*+1*/ { '(', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open right */
/*+1*/ { ')', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open left */
/*70*/ { '(', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open right */
/*+1*/ { '-', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open right */
/*+1*/ { ')', "", "",               ROOM, LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open left */
       { '0', "", "",               ROOM, LAYER_MONSTER_EFFECT, FALSE,  0, 0,     { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                  /* 4 magic shield symbols */
       { '#', "", "",               ROOM, LAYER_MONSTER_EFFECT, FALSE, 0, 0,      { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { '@', "", "",               ROOM, LAYER_MONSTER_EFFECT, FALSE,  0, 0,    { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { '*', "", "",               ROOM, LAYER_MONSTER_EFFECT, FALSE,  0, 0,        { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { '?', "", "",               ROOM, LAYER_MONSTER_EFFECT, FALSE,    0, 0, { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },   /* talk effect */
       { '#', "poison cloud", "",   ROOM, LAYER_ENVIRONMENT, FALSE,    0, 0,
           { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, 
           {POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION, POISON_CLOUD_ANIMATION},
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
           {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },   /* part of a cloud */
       { '?', "valid position", "", ROOM, LAYER_GENERAL_UI, FALSE,    0, 0, { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} }, /*  target position */
       /* swallow colors are changed by mapglyph() to match engulfing monst */
       { '/', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow top left      */
       { '-', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow top center    */
       { '\\', "", "",        ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* swallow top right     */
/*80*/ { '|', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow middle left   */
/*+1*/ { '-', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow middle center  */
       { '|', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow middle right  */
       { '\\', "", "",        ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* swallow bottom left   */
       { '-', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow bottom center */
       { '/', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow bottom right  */
       /* explosion colors are changed by mapglyph() to match type of expl. */
       { '/', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion top left     */
       { '-', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion top center   */
       { '\\', "", "",        ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* explosion top right    */
       { '|', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion middle left  */
       { ' ', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion middle center*/
/*90*/ { '|', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion middle right */
       { '\\', "", "",        ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* explosion bottom left  */
       { '-', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion bottom center*/
       { '/', "", "",         ROOM, LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion bottom right */
};

/* NOTE: Use CMAP variations for differently colored base symbols, e.g. altars, stairs, and fountains, and for storing autodraw base tiles */
/* GUI tiles that appear the exactly same in TTY graphics should be implemented using replacements, unless they are somehow more conveniently defined in a table format for several CMAP types */
const struct symdef_cmap_variation defsym_variations[MAX_VARIATIONS] =
{
       { S_crwall, "crwall-all-ends", "wall", "", FALSE, 
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* crwall */

       { S_stone, "stone-variation-1", "", "", FALSE,
            { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),    /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_STONE_REPLACEMENT, GNOMISH_MINES_STONE_REPLACEMENT, GEHENNOM_STONE_REPLACEMENT, DUNGEON_UNDEAD_STONE_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_STONE_REPLACEMENT, GARDEN_STONE_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_stone, "stone-variation-2", "", "", FALSE,
            { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),    /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_STONE_REPLACEMENT, GNOMISH_MINES_STONE_REPLACEMENT, GEHENNOM_STONE_REPLACEMENT, DUNGEON_UNDEAD_STONE_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_STONE_REPLACEMENT, GARDEN_STONE_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_stone, "stone-variation-3", "", "", FALSE,
            { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),    /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_STONE_REPLACEMENT, GNOMISH_MINES_STONE_REPLACEMENT, GEHENNOM_STONE_REPLACEMENT, DUNGEON_UNDEAD_STONE_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_STONE_REPLACEMENT, GARDEN_STONE_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */

       { S_vwall, "vwall-variation-1", "", "", FALSE,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_VWALL_REPLACEMENT, GNOMISH_MINES_VWALL_REPLACEMENT, GEHENNOM_VWALL_REPLACEMENT, DUNGEON_UNDEAD_VWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_VWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_vwall, "vwall-variation-2", "", "", FALSE,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_VWALL_REPLACEMENT, GNOMISH_MINES_VWALL_REPLACEMENT, GEHENNOM_VWALL_REPLACEMENT, DUNGEON_UNDEAD_VWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_VWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_vwall, "vwall-variation-3", "", "", FALSE,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_VWALL_REPLACEMENT, GNOMISH_MINES_VWALL_REPLACEMENT, GEHENNOM_VWALL_REPLACEMENT, DUNGEON_UNDEAD_VWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_VWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */

       { S_hwall, "hwall-variation-1", "", "", FALSE,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_HWALL_REPLACEMENT, GNOMISH_MINES_HWALL_REPLACEMENT, GEHENNOM_HWALL_REPLACEMENT, DUNGEON_UNDEAD_HWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_HWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_hwall, "hwall-variation-2", "", "", FALSE,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_HWALL_REPLACEMENT, GNOMISH_MINES_HWALL_REPLACEMENT, GEHENNOM_HWALL_REPLACEMENT, DUNGEON_UNDEAD_HWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_HWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_hwall, "hwall-variation-3", "", "", FALSE,
            { C(CLR_GRAY), C(HI_MINES), C(HI_GEHENNOM), C(HI_UNDEAD), C(HI_SOKOBAN),   /*5*/ C(HI_ELEMENTAL_PLANES), C(HI_ASTRAL), C(HI_GEHENNOM), C(HI_GARDEN), C(HI_MODRON),      /*10*/ C(HI_SHOP), C(HI_REINFORCED), C(HI_NEST), C(HI_TOWER), C(HI_OPULENT), C(HI_TEMPLE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_HWALL_REPLACEMENT, GNOMISH_MINES_HWALL_REPLACEMENT, GEHENNOM_HWALL_REPLACEMENT, DUNGEON_UNDEAD_HWALL_REPLACEMENT, 0, 0, 0, GEHENNOM_CAVERNOUS_HWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */

       { S_room, "floor-normal-variation-1", "", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_ROOM_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "floor-normal-variation-2", "", "", FALSE, 
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_ROOM_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "floor-normal-variation-3", "", "", FALSE, 
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_ROOM_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "floor-marble-variation-0", "marble floor", "", FALSE, 
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* marble floor */
       { S_room, "floor-marble-variation-1", "marble floor", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* marble floor */
       { S_room, "floor-marble-variation-2", "marble floor", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* marble floor */
       { S_room, "floor-marble-variation-3", "marble floor", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* marble floor */
       { S_room, "floor-parquet-variation-0", "parquet", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),    /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),      /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* parquet */
       { S_room, "floor-parquet-variation-1", "parquet", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),    /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),      /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* parquet */
       { S_room, "floor-parquet-variation-2", "parquet", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),    /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),      /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* parquet */
       { S_room, "floor-parquet-variation-3", "parquet", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),    /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),      /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* parquet */
       { S_room, "floor-cobblestone-variation-0", "cobblestone", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "floor-cobblestone-variation-1", "cobblestone", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "floor-cobblestone-variation-2", "cobblestone", "", FALSE, 
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "floor-cobblestone-variation-3", "cobblestone", "", FALSE, 
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */

       { S_grass, "grass-variation-1", "", "",  FALSE,   
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),    /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),      /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GRASS_REPLACEMENT, DUNGEON_NORMAL_GRASS_REPLACEMENT, GEHENNOM_GRASS_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */
       { S_grass, "grass-variation-2", "", "",  FALSE,       
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),    /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),      /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GRASS_REPLACEMENT, DUNGEON_NORMAL_GRASS_REPLACEMENT, GEHENNOM_GRASS_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */
       { S_grass, "grass-variation-swampy",   "swampy grass", "",  FALSE,          
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),    /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),      /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GRASS_SWAMPY_REPLACEMENT, DUNGEON_NORMAL_GRASS_SWAMPY_REPLACEMENT, GEHENNOM_GRASS_SWAMPY_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */

       { S_ground, "ground-variation-1", "", "",  FALSE,   
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { S_ground, "ground-variation-2", "", "",  FALSE,       
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { S_ground, "ground-variation-3", "", "",  FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { S_ground, "ground-variation-swampy", "swampy ground", "",  FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, DUNGEON_NORMAL_GROUND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { S_ground, "ground-variation-desert-sand", "desert sand", "",  FALSE,
            { C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW),    /*5*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW),      /*10*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_GROUND_DESERT_SAND_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },

       { S_litcorr, "corridor-variation-1", "", "",  FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { S_litcorr, "corridor-variation-2", "", "",  FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },
       { S_litcorr, "corridor-variation-3", "", "",  FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },

       { S_upstair, "branch-staircase-up", "", "",  FALSE,          
            { C(CLR_BRIGHT_BLUE), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_BRANCH_UPSTAIRS_ENLARGEMENT, GNOMISH_MINES_BRANCH_UPSTAIRS_ENLARGEMENT, 0, DUNGEON_UNDEAD_BRANCH_UPSTAIRS_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* upstair */
       { S_upstair, "special-branch-staircase-up",  "", "", FALSE, 
            { C(CLR_WHITE), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_SPECIAL_BRANCH_UPSTAIRS_ENLARGEMENT, GNOMISH_MINES_SPECIAL_BRANCH_UPSTAIRS_ENLARGEMENT, 0, DUNGEON_UNDEAD_SPECIAL_BRANCH_UPSTAIRS_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },         /* upstair */

       { S_dnstair, "branch-staircase-down",   "",  "", FALSE,     
            { C(CLR_YELLOW), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BRIGHT_BLUE), /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* dnstair */
       { S_dnstair, "special-branch-staircase-down", "", "", FALSE,  
            { C(CLR_BLACK), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },       /* dnstair */

       { S_altar, "lawful-altar",  "lawful altar", "", FALSE,     
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),  /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {LAWFUL_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "neutral-altar", "neutral altar", "", FALSE,     
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {NEUTRAL_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "chaotic-altar", "chaotic altar", "", FALSE,     
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {CHAOTIC_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "high-altar",  "high altar",  "", FALSE,      
            { C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), /*5*/ C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), /*10*/ C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {HIGH_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "altar-of-Moloch", "altar of Moloch", "", FALSE,   
            { C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED),           /*5*/ C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED),         /*10*/ C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {ALTAR_OF_MOLOCH_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */

       { S_fountain, "magic-fountain", "",  "", TRUE,               
            { C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE) }, 
            {MAGIC_FOUNTAIN_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-healing",  "", "", TRUE,         
            { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, 
            {FOUNTAIN_OF_HEALING_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-mana",     "", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {FOUNTAIN_OF_MANA_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-power",    "", "", TRUE,         
            { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, 
            {FOUNTAIN_OF_POWER_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-poison",   "", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {FOUNTAIN_OF_POISON_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-water", "", "", TRUE,     
            { C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE) }, 
            {FOUNTAIN_OF_WATER_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */

       { S_arrow_trap, "crossbow-bolt-trap",   "crossbow bolt trap", "", TRUE,         
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */

       { S_modron_portal, "spherical-modron-portal",   "spherical magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */
       { S_modron_portal, "cylindrical-modron-portal",   "cylindrical magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */
       { S_modron_portal, "tetrahedral-modron-portal",   "tetrahedral magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */
       { S_modron_portal, "cubical-modron-portal",   "cubical magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */
       { S_modron_portal, "octahedral-modron-portal",   "octahedral magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */
       { S_modron_portal, "dodecahedral-modron-portal",   "dodecahedral magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */
       { S_modron_portal, "icosahedral-modron-portal",   "icosahedral magic portal", "", TRUE,         
            { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* modron portal */

       { S_vodoor, "vopen-windowed-wooden-door",   "open windowed door", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-iron-door",   "open iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-windowed-iron-door",   "open windowed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-iron-bars-door",   "open iron bars door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-magic-door",   "open magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-windowed-magic-door",   "open windowed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-modron-door",   "open alien door", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-reinforced-door",   "open reinforced door", "", FALSE,
            { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-stone-door",   "open stone door", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-obsidian-door",   "open obsidian door", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */
       { S_vodoor, "vopen-black-gate",   "open black gate", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vodoor */

       { S_hodoor, "hopen-windowed-wooden-door",   "open windowed door", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-iron-door",   "open iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-windowed-iron-door",   "open windowed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-iron-bars-door",   "open iron bars door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-magic-door",   "open magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-windowed-magic-door",   "open windowed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-modron-door",   "open alien door", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-reinforced-door",   "open reinforced door", "", FALSE,
            { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-stone-door",   "open stone door", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-obsidian-door",   "open obsidian door", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */
       { S_hodoor, "hopen-black-gate",   "open black gate", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hodoor */

       { S_vcdoor, "vclosed-windowed-wooden-door",   "closed windowed door", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-iron-door",   "closed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-windowed-iron-door",   "closed windowed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-iron-bars-door",   "closed iron bars door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-magic-door",   "closed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-windowed-magic-door",   "closed windowed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-modron-door",   "closed alien door", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-reinforced-door",   "closed reinforced door", "", FALSE,
            { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-stone-door",   "closed stone door", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-obsidian-door",   "closed obsidian door", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */
       { S_vcdoor, "vclosed-black-gate",   "closed black gate", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vcdoor */

       { S_hcdoor, "hclosed-windowed-wooden-door",   "closed windowed door", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_WHITE), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-iron-door",   "closed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-windowed-iron-door",   "closed windowed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-iron-bars-door",   "closed iron bars door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-magic-door",   "closed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-windowed-magic-door",   "closed windowed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-modron-door",   "closed alien door", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-reinforced-door",   "closed reinforced door", "", FALSE,
            { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-stone-door",   "closed stone door", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-obsidian-door",   "closed obsidian door", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */
       { S_hcdoor, "hclosed-black-gate",   "closed black gate", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hcdoor */

       { S_vbdoor, "vbroken-windowed-wooden-door",   "broken windowed door", "", FALSE,
            { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_BLACK), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-iron-door",   "broken iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-windowed-iron-door",   "broken windowed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-iron-bars-door",   "broken iron bars door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-magic-door",   "broken magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-windowed-magic-door",   "broken windowed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-modron-door",   "broken alien door", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-reinforced-door",   "broken reinforced door", "", FALSE,
            { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-stone-door",   "broken stone door", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-obsidian-door",   "broken obsidian door", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */
       { S_vbdoor, "vbroken-black-gate",   "broken black gate", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* vbdoor */

       { S_hbdoor, "hbroken-windowed-wooden-door",   "broken windowed door", "", FALSE,
            { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_BLACK), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-iron-door",   "broken iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-windowed-iron-door",   "broken windowed iron door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-iron-bars-door",   "broken iron bars door", "", FALSE,
            { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-magic-door",   "broken magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-windowed-magic-door",   "broken windowed magic door", "", FALSE,
            { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-modron-door",   "broken alien door", "", FALSE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-reinforced-door",   "broken reinforced door", "", FALSE,
            { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-stone-door",   "broken stone door", "", FALSE,
            { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-obsidian-door",   "broken obsidian door", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */
       { S_hbdoor, "hbroken-black-gate",   "broken black gate", "", FALSE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) },
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },      /* hbdoor */

       { S_brazier, "brazier-subtype-1",  "small brazier", "", FALSE,     
            { C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), /*5*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW),  /*10*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {BRAZIER_SUBTYPE_1_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* brazier */
       { S_brazier, "brazier-subtype-2",  "large brazier", "", FALSE,     
            { C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), /*5*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW),  /*10*/ C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_YELLOW) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {BRAZIER_SUBTYPE_2_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* brazier */

       { S_signpost, "signpost-subtype-1",  "signpost with a small symbol", "", FALSE,     
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),  /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* signpost */
       { S_signpost, "signpost-subtype-2",  "signpost with a large symbol", "", FALSE,
            { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),  /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* signpost */

       { S_tree, "tree-oak",  "oak tree", "", TRUE,     
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {OAK_TREE_ENLARGEMENT, 0, OAK_TREE_GEHENNOM_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-spruce",  "spruce tree", "", TRUE,     
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {SPRUCE_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-fir",  "fir tree", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {FIR_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-date-palm",  "date palm", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DATE_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-date-palm-2",  "date palm", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DATE_TREE_2_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-date-palm-3",  "date palm", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DATE_TREE_3_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-apple",  "apple tree", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {APPLE_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-banana",  "banana plant", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {BANANA_PLANT_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-fig",  "fig tree", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {FIG_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-orange",  "orange tree", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {ORANGE_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-pitaya",  "pitaya plant", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {PITAYA_PLANT_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-small",  "small tree", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-fir-snowy",  "snowy fir tree", "", TRUE,
            { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),  /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {SNOWY_FIR_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-wizened",  "wizened tree", "", TRUE,
            { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK),  /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {WIZENED_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { S_tree, "tree-eucalyptus",  "eucalyptus tree", "", TRUE,
            { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),  /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {EUCALYPTUS_TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
};


/* default rogue level symbols */
STATIC_VAR const uchar def_r_oc_syms[MAX_OBJECT_CLASSES] = {
/* 0*/ '\0', ILLOBJ_SYM, WEAPON_SYM, ']', /* armor */
       RING_SYM,
/* 5*/ ',',                     /* amulet */
       TOOL_SYM, ':',           /* food */
       POTION_SYM, SCROLL_SYM,
/*10*/ SPBOOK_SYM, WAND_SYM,
       GEM_SYM,                /* gold -- yes it's the same as gems */
       GEM_SYM, ROCK_SYM,
/*15*/ BALL_SYM, CHAIN_SYM, VENOM_SYM, MISCELLANEOUS_SYM, ART_SYM
};

#undef C

#ifdef TERMLIB
void NDECL((*decgraphics_mode_callback)) = 0; /* set in tty_start_screen() */
#endif /* TERMLIB */

#ifdef PC9800
void NDECL((*ibmgraphics_mode_callback)) = 0; /* set in tty_start_screen() */
void NDECL((*ascgraphics_mode_callback)) = 0; /* set in tty_start_screen() */
#endif

#ifdef CURSES_GRAPHICS
void NDECL((*cursesgraphics_mode_callback)) = 0;
#endif

/*
 * Convert the given character to an object class.  If the character is not
 * recognized, then MAX_OBJECT_CLASSES is returned.  Used in detect.c, invent.c,
 * objnam.c, options.c, pickup.c, sp_lev.c, lev_main.c, and tilemap.c.
 */
int
def_char_to_objclass(ch)
char ch;
{
    int i;
    for (i = 1; i < MAX_OBJECT_CLASSES; i++)
        if (ch == def_oc_syms[i].sym)
            break;
    return i;
}

/*
 * Convert a character into a monster class.  This returns the _first_
 * match made.  If there are are no matches, return MAX_MONSTER_CLASSES.
 * Used in detect.c, options.c, read.c, sp_lev.c, and lev_main.c
 */
int
def_char_to_monclass(ch)
char ch;
{
    int i;
    for (i = 1; i < MAX_MONSTER_CLASSES; i++)
        if (ch == def_monsyms[i].sym)
            break;
    return i;
}

/*
 * Explanations of the functions found below:
 *
 * init_symbols()
 *                     Sets the current display symbols, the
 *                     loadable symbols to the default GnollHack
 *                     symbols, including the r_syms rogue level
 *                     symbols. This would typically be done
 *                     immediately after execution begins. Any
 *                     previously loaded external symbol sets are
 *                     discarded.
 *
 * switch_symbols(arg)
 *                     Called to swap in new current display symbols
 *                     (showsyms) from either the default symbols,
 *                     or from the loaded symbols.
 *
 *                     If (arg == 0) then showsyms are taken from
 *                     defsyms, def_oc_syms, and def_monsyms.
 *
 *                     If (arg != 0), which is the normal expected
 *                     usage, then showsyms are taken from the
 *                     adjustable display symbols found in l_syms.
 *                     l_syms may have been loaded from an external
 *                     symbol file by config file options or interactively
 *                     in the Options menu.
 *
 * assign_graphics(arg)
 *
 *                     This is used in the game core to toggle in and
 *                     out of other {rogue} level display modes.
 *
 *                     If arg is ROGUESET, this places the rogue level
 *                     symbols from r_syms into showsyms.
 *
 *                     If arg is PRIMARY, this places the symbols
 *                     from l_monsyms into showsyms.
 *
 * update_l_symset()
 *                     Update a member of the loadable (l_*) symbol set.
 *
 * update_r_symset()
 *                     Update a member of the rogue (r_*) symbol set.
 *
 */

void
init_symbols()
{
    init_l_symbols();
    init_showsyms();
    init_r_symbols();
}

void
update_bouldersym()
{
    nhsym boulder = iflags.bouldersym;

    if (!boulder)
        boulder = (nhsym)def_oc_syms[ROCK_CLASS].sym; /* (nhsym) ROCK_SYM */
    showsyms[SYM_BOULDER + SYM_OFF_X] = boulder;
    l_syms[SYM_BOULDER + SYM_OFF_X] = boulder;
    r_syms[SYM_BOULDER + SYM_OFF_X] = boulder;
}

void
init_showsyms()
{
    register int i;

    for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        showsyms[i + SYM_OFF_P] = (nhsym)defsyms[i].sym;
    for (i = 0; i < MAX_OBJECT_CLASSES; i++)
        showsyms[i + SYM_OFF_O] = (nhsym)def_oc_syms[i].sym;
    for (i = 0; i < MAX_MONSTER_CLASSES; i++)
        showsyms[i + SYM_OFF_M] = (nhsym)def_monsyms[i].sym;
    for (i = 0; i < WARNCOUNT; i++)
        showsyms[i + SYM_OFF_W] = (nhsym)def_warnsyms[i].sym;
    for (i = 0; i < MAXOTHER; i++) {
        if (i == SYM_BOULDER)
            showsyms[i + SYM_OFF_X] = (iflags.bouldersym
                                        ? iflags.bouldersym
                                        : (nhsym)def_oc_syms[ROCK_CLASS].sym);
        else if (i == SYM_INVISIBLE)
            showsyms[i + SYM_OFF_X] = (nhsym)DEF_INVISIBLE;
    }
}

/* initialize defaults for the loadable symset */
void
init_l_symbols()
{
    register int i;

    for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        l_syms[i + SYM_OFF_P] = defsyms[i].sym;
    for (i = 0; i < MAX_OBJECT_CLASSES; i++)
        l_syms[i + SYM_OFF_O] = def_oc_syms[i].sym;
    for (i = 0; i < MAX_MONSTER_CLASSES; i++)
        l_syms[i + SYM_OFF_M] = def_monsyms[i].sym;
    for (i = 0; i < WARNCOUNT; i++)
        l_syms[i + SYM_OFF_W] = def_warnsyms[i].sym;
    for (i = 0; i < MAXOTHER; i++) {
        if (i == SYM_BOULDER)
            l_syms[i + SYM_OFF_X] = iflags.bouldersym
                                      ? iflags.bouldersym
                                      : (nhsym)def_oc_syms[ROCK_CLASS].sym;
        else if (i == SYM_INVISIBLE)
            l_syms[i + SYM_OFF_X] = DEF_INVISIBLE;
    }

    clear_symsetentry(PRIMARY, FALSE);
}

void
init_r_symbols()
{
    register int i;

    /* These are defaults that can get overwritten
       later by the roguesymbols option */

    for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        r_syms[i + SYM_OFF_P] = defsyms[i].sym;
    r_syms[S_vodoor] = r_syms[S_hodoor] = r_syms[S_ndoor] = r_syms[S_hbdoor] = r_syms[S_vbdoor] = r_syms[S_hoportcullis] = r_syms[S_voportcullis] = '+';
    r_syms[S_upstair] = r_syms[S_dnstair] = '%';

    for (i = 0; i < MAX_OBJECT_CLASSES; i++)
        r_syms[i + SYM_OFF_O] = def_r_oc_syms[i];
    for (i = 0; i < MAX_MONSTER_CLASSES; i++)
        r_syms[i + SYM_OFF_M] = def_monsyms[i].sym;
    for (i = 0; i < WARNCOUNT; i++)
        r_syms[i + SYM_OFF_W] = def_warnsyms[i].sym;
    for (i = 0; i < MAXOTHER; i++) {
        if (i == SYM_BOULDER)
            r_syms[i + SYM_OFF_X] = iflags.bouldersym
                                      ? iflags.bouldersym
                                      : (nhsym)def_oc_syms[ROCK_CLASS].sym;
        else if (i == SYM_INVISIBLE)
            r_syms[i + SYM_OFF_X] = DEF_INVISIBLE;
    }

    clear_symsetentry(ROGUESET, FALSE);
    /* default on Rogue level is no color
     * but some symbol sets can override that
     */
    symset[ROGUESET].nocolor = 1;
}

void
assign_graphics(whichset)
int whichset;
{
    register int i;

    switch (whichset) {
    case ROGUESET:
        /* Adjust graphics display characters on Rogue levels */

        for (i = 0; i < SYM_MAX; i++)
            showsyms[i] = r_syms[i];

#if defined(MSDOS) && defined(USE_TILES)
        if (iflags.grmode)
            tileview(FALSE);
#endif
        currentgraphics = ROGUESET;
        break;

    case PRIMARY:
    default:
        for (i = 0; i < SYM_MAX; i++)
            showsyms[i] = l_syms[i];

#if defined(MSDOS) && defined(USE_TILES)
        if (iflags.grmode)
            tileview(TRUE);
#endif
        currentgraphics = PRIMARY;
        break;
    }
}

void
switch_symbols(nondefault)
int nondefault;
{
    register int i;

    if (nondefault) {
        for (i = 0; i < SYM_MAX; i++)
            showsyms[i] = l_syms[i];
#ifdef PC9800
        if (SYMHANDLING(H_IBM) && ibmgraphics_mode_callback)
            (*ibmgraphics_mode_callback)();
        else if (!symset[currentgraphics].name && ascgraphics_mode_callback)
            (*ascgraphics_mode_callback)();
#endif
#ifdef TERMLIB
        if (SYMHANDLING(H_DEC) && decgraphics_mode_callback)
            (*decgraphics_mode_callback)();
#endif
# ifdef CURSES_GRAPHICS
        if (SYMHANDLING(H_CURS) && cursesgraphics_mode_callback)
            (*cursesgraphics_mode_callback)();
# endif
    } else {
        init_l_symbols();
        init_showsyms();
    }
}

void
update_l_symset(symp, val)
struct symparse *symp;
nhsym val;
{
    l_syms[symp->idx] = val;
}

void
update_r_symset(symp, val)
struct symparse *symp;
nhsym val;
{
    r_syms[symp->idx] = val;
}

void
clear_symsetentry(which_set, name_too)
int which_set;
boolean name_too;
{
    if (symset[which_set].desc)
        free((genericptr_t) symset[which_set].desc);
    symset[which_set].desc = (char *) 0;

    symset[which_set].handling = H_UNK;
    symset[which_set].nocolor = 0;
    /* initialize restriction bits */
    symset[which_set].primary = 0;
    symset[which_set].rogue = 0;

    if (name_too) {
        if (symset[which_set].name)
            free((genericptr_t) symset[which_set].name);
        symset[which_set].name = (char *) 0;
    }
}


/*
 * If you are adding code somewhere to be able to recognize
 * particular types of symset "handling", define a
 * H_XXX macro in include/rm.h and add the name
 * to this array at the matching offset.
 */
const char *known_handling[] = {
    "UNKNOWN", /* H_UNK  */
    "IBM",     /* H_IBM  */
    "DEC",     /* H_DEC  */
    "CURS",    /* H_CURS */
    "UNICODE", /* H_UNICODE */
    (const char *) 0,
};

/*
 * Accepted keywords for symset restrictions.
 * These can be virtually anything that you want to
 * be able to test in the code someplace.
 * Be sure to:
 *    - add a corresponding Bitfield to the symsetentry struct in rm.h
 *    - initialize the field to zero in parse_sym_line in the SYM_CONTROL
 *      case 0 section of the idx switch. The location is prefaced with
 *      with a comment stating "initialize restriction bits".
 *    - set the value appropriately based on the index of your keyword
 *      under the case 5 sections of the same SYM_CONTROL idx switches.
 *    - add the field to clear_symsetentry()
 */
const char *known_restrictions[] = {
    "primary", "rogue", (const char *) 0,
};

struct symparse loadsyms[] = {
    { SYM_CONTROL, 0, "start" },
    { SYM_CONTROL, 0, "begin" },
    { SYM_CONTROL, 1, "finish" },
    { SYM_CONTROL, 2, "handling" },
    { SYM_CONTROL, 3, "description" },
    { SYM_CONTROL, 4, "color" },
    { SYM_CONTROL, 4, "colour" },
    { SYM_CONTROL, 5, "restrictions" },
    { SYM_PCHAR, S_unexplored, "S_unexplored" },
    { SYM_PCHAR, S_stone, "S_stone" },
    { SYM_PCHAR, S_vwall, "S_vwall" },
    { SYM_PCHAR, S_hwall, "S_hwall" },
    { SYM_PCHAR, S_tlcorn, "S_tlcorn" },
    { SYM_PCHAR, S_trcorn, "S_trcorn" },
    { SYM_PCHAR, S_blcorn, "S_blcorn" },
    { SYM_PCHAR, S_brcorn, "S_brcorn" },
    { SYM_PCHAR, S_crwall, "S_crwall" },
    { SYM_PCHAR, S_tuwall, "S_tuwall" },
    { SYM_PCHAR, S_tdwall, "S_tdwall" },
    { SYM_PCHAR, S_tlwall, "S_tlwall" },
    { SYM_PCHAR, S_trwall, "S_trwall" },
    { SYM_PCHAR, S_ndoor, "S_ndoor" },
    { SYM_PCHAR, S_vodoor, "S_vodoor" },
    { SYM_PCHAR, S_hodoor, "S_hodoor" },
    { SYM_PCHAR, S_vcdoor, "S_vcdoor" },
    { SYM_PCHAR, S_hcdoor, "S_hcdoor" },
    { SYM_PCHAR, S_vbdoor, "S_vbdoor" },
    { SYM_PCHAR, S_hbdoor, "S_hbdoor" },
    { SYM_PCHAR, S_voportcullis, "S_voportcullis" },
    { SYM_PCHAR, S_hoportcullis, "S_hoportcullis" },
    { SYM_PCHAR, S_bars, "S_bars" },
    { SYM_PCHAR, S_tree, "S_tree" },
    { SYM_PCHAR, S_room, "S_room" },
    { SYM_PCHAR, S_darkroom, "S_darkroom" },
    { SYM_PCHAR, S_grass, "S_grass" },
    { SYM_PCHAR, S_darkgrass, "S_darkgrass" },
    { SYM_PCHAR, S_ground, "S_ground" },
    { SYM_PCHAR, S_darkground, "S_darkground" },
    { SYM_PCHAR, S_corr, "S_corr" },
    { SYM_PCHAR, S_litcorr, "S_litcorr" },
    { SYM_PCHAR, S_upstair, "S_upstair" },
    { SYM_PCHAR, S_dnstair, "S_dnstair" },
    { SYM_PCHAR, S_upladder, "S_upladder" },
    { SYM_PCHAR, S_dnladder, "S_dnladder" },
    { SYM_PCHAR, S_anvil, "S_anvil" },
    { SYM_PCHAR, S_altar, "S_altar" },
    { SYM_PCHAR, S_extra_boulder, "S_extra_boulder" },
    { SYM_PCHAR, S_grave, "S_grave" },
    { SYM_PCHAR, S_brazier, "S_brazier" },
    { SYM_PCHAR, S_signpost, "S_signpost" },
    { SYM_PCHAR, S_throne, "S_throne" },
    { SYM_PCHAR, S_sink, "S_sink" },
    { SYM_PCHAR, S_fountain, "S_fountain" },
    { SYM_PCHAR, S_pool, "S_pool" },
    { SYM_PCHAR, S_ice, "S_ice" },
    { SYM_PCHAR, S_lava, "S_lava" },
    { SYM_PCHAR, S_vodbridge, "S_vodbridge" },
    { SYM_PCHAR, S_hodbridge, "S_hodbridge" },
    { SYM_PCHAR, S_vcdbridge, "S_vcdbridge" },
    { SYM_PCHAR, S_hcdbridge, "S_hcdbridge" },
    { SYM_PCHAR, S_air, "S_air" },
    { SYM_PCHAR, S_cloud, "S_cloud" },
    { SYM_PCHAR, S_talkeffect, "S_talkeffect" },
    { SYM_PCHAR, S_poisoncloud, "S_poisoncloud" },
    { SYM_PCHAR, S_water, "S_water" },
    { SYM_PCHAR, S_arrow_trap, "S_arrow_trap" },
    { SYM_PCHAR, S_dart_trap, "S_dart_trap" },
    { SYM_PCHAR, S_falling_rock_trap, "S_falling_rock_trap" },
    { SYM_PCHAR, S_squeaky_board, "S_squeaky_board" },
    { SYM_PCHAR, S_bear_trap, "S_bear_trap" },
    { SYM_PCHAR, S_land_mine, "S_land_mine" },
    { SYM_PCHAR, S_rolling_boulder_trap, "S_rolling_boulder_trap" },
    { SYM_PCHAR, S_sleeping_gas_trap, "S_sleeping_gas_trap" },
    { SYM_PCHAR, S_rust_trap, "S_rust_trap" },
    { SYM_PCHAR, S_fire_trap, "S_fire_trap" },
    { SYM_PCHAR, S_pit, "S_pit" },
    { SYM_PCHAR, S_spiked_pit, "S_spiked_pit" },
    { SYM_PCHAR, S_hole, "S_hole" },
    { SYM_PCHAR, S_trap_door, "S_trap_door" },
    { SYM_PCHAR, S_teleportation_trap, "S_teleportation_trap" },
    { SYM_PCHAR, S_level_teleporter, "S_level_teleporter" },
    { SYM_PCHAR, S_magic_portal, "S_magic_portal" },
    { SYM_PCHAR, S_web, "S_web" },
    { SYM_PCHAR, S_statue_trap, "S_statue_trap" },
    { SYM_PCHAR, S_magic_trap, "S_magic_trap" },
    { SYM_PCHAR, S_anti_magic_trap, "S_anti_magic_trap" },
    { SYM_PCHAR, S_polymorph_trap, "S_polymorph_trap" },
    { SYM_PCHAR, S_modron_portal, "S_modron_portal" },
    { SYM_PCHAR, S_lever, "S_lever" },
    { SYM_PCHAR, S_vibrating_square, "S_vibrating_square" },
    { SYM_PCHAR, S_vbeam, "S_vbeam" },
    { SYM_PCHAR, S_hbeam, "S_hbeam" },
    { SYM_PCHAR, S_lslant, "S_lslant" },
    { SYM_PCHAR, S_rslant, "S_rslant" },
    { SYM_PCHAR, S_digbeam, "S_digbeam" },
    { SYM_PCHAR, S_flashbeam, "S_flashbeam" },
    { SYM_PCHAR, S_boom_tl, "S_boom_tl" },
    { SYM_PCHAR, S_boom_tc, "S_boom_tc" },
    { SYM_PCHAR, S_boom_tr, "S_boom_tr" },
    { SYM_PCHAR, S_boom_ml, "S_boom_ml" },
    { SYM_PCHAR, S_boom_mr, "S_boom_mr" },
    { SYM_PCHAR, S_boom_bl, "S_boom_bl" },
    { SYM_PCHAR, S_boom_bc, "S_boom_bc" },
    { SYM_PCHAR, S_boom_br, "S_boom_br" },
    { SYM_PCHAR, S_goodpos, "S_goodpos" },
    { SYM_PCHAR, S_ss1, "S_ss1" },
    { SYM_PCHAR, S_ss2, "S_ss2" },
    { SYM_PCHAR, S_ss3, "S_ss3" },
    { SYM_PCHAR, S_ss4, "S_ss4" },
    { SYM_PCHAR, S_sw_tl, "S_sw_tl" },
    { SYM_PCHAR, S_sw_tc, "S_sw_tc" },
    { SYM_PCHAR, S_sw_tr, "S_sw_tr" },
    { SYM_PCHAR, S_sw_ml, "S_sw_ml" },
    { SYM_PCHAR, S_sw_mc, "S_sw_mc" },
    { SYM_PCHAR, S_sw_mr, "S_sw_mr" },
    { SYM_PCHAR, S_sw_bl, "S_sw_bl" },
    { SYM_PCHAR, S_sw_bc, "S_sw_bc" },
    { SYM_PCHAR, S_sw_br, "S_sw_br" },
    { SYM_PCHAR, S_explode1, "S_explode1" },
    { SYM_PCHAR, S_explode2, "S_explode2" },
    { SYM_PCHAR, S_explode3, "S_explode3" },
    { SYM_PCHAR, S_explode4, "S_explode4" },
    { SYM_PCHAR, S_explode5, "S_explode5" },
    { SYM_PCHAR, S_explode6, "S_explode6" },
    { SYM_PCHAR, S_explode7, "S_explode7" },
    { SYM_PCHAR, S_explode8, "S_explode8" },
    { SYM_PCHAR, S_explode9, "S_explode9" },
    { SYM_OC, ILLOBJ_CLASS + SYM_OFF_O, "S_strange_obj" },
    { SYM_OC, WEAPON_CLASS + SYM_OFF_O, "S_weapon" },
    { SYM_OC, ARMOR_CLASS + SYM_OFF_O, "S_armor" },
    { SYM_OC, ARMOR_CLASS + SYM_OFF_O, "S_armour" },
    { SYM_OC, RING_CLASS + SYM_OFF_O, "S_ring" },
    { SYM_OC, AMULET_CLASS + SYM_OFF_O, "S_amulet" },
    { SYM_OC, MISCELLANEOUS_CLASS + SYM_OFF_O, "S_miscellaneous" },
    { SYM_OC, TOOL_CLASS + SYM_OFF_O, "S_tool" },
    { SYM_OC, FOOD_CLASS + SYM_OFF_O, "S_food" },
    { SYM_OC, POTION_CLASS + SYM_OFF_O, "S_potion" },
    { SYM_OC, SCROLL_CLASS + SYM_OFF_O, "S_scroll" },
    { SYM_OC, SPBOOK_CLASS + SYM_OFF_O, "S_book" },
    { SYM_OC, WAND_CLASS + SYM_OFF_O, "S_wand" },
    { SYM_OC, COIN_CLASS + SYM_OFF_O, "S_coin" },
    { SYM_OC, GEM_CLASS + SYM_OFF_O, "S_gem" },
    { SYM_OC, ART_CLASS + SYM_OFF_O, "S_art" },
    { SYM_OC, ROCK_CLASS + SYM_OFF_O, "S_rock" },
    { SYM_OC, BALL_CLASS + SYM_OFF_O, "S_ball" },
    { SYM_OC, CHAIN_CLASS + SYM_OFF_O, "S_chain" },
    { SYM_OC, VENOM_CLASS + SYM_OFF_O, "S_venom" },
    { SYM_OC, REAGENT_CLASS + SYM_OFF_O, "S_reagent" },
    { SYM_MON, S_ANT + SYM_OFF_M, "S_ant" },
    { SYM_MON, S_BLOB + SYM_OFF_M, "S_blob" },
    { SYM_MON, S_COCKATRICE + SYM_OFF_M, "S_cockatrice" },
    { SYM_MON, S_DOG + SYM_OFF_M, "S_dog" },
    { SYM_MON, S_EYE + SYM_OFF_M, "S_eye" },
    { SYM_MON, S_FELINE + SYM_OFF_M, "S_feline" },
    { SYM_MON, S_GNOME + SYM_OFF_M, "S_gnome" },
    { SYM_MON, S_HUMANOID + SYM_OFF_M, "S_humanoid" },
    { SYM_MON, S_IMP + SYM_OFF_M, "S_imp" },
    { SYM_MON, S_JELLY + SYM_OFF_M, "S_jelly" },
    { SYM_MON, S_KOBOLD + SYM_OFF_M, "S_kobold" },
    { SYM_MON, S_LEPRECHAUN + SYM_OFF_M, "S_leprechaun" },
    { SYM_MON, S_MIMIC + SYM_OFF_M, "S_mimic" },
    { SYM_MON, S_NYMPH + SYM_OFF_M, "S_nymph" },
    { SYM_MON, S_ORC + SYM_OFF_M, "S_orc" },
    { SYM_MON, S_PIERCER + SYM_OFF_M, "S_piercer" },
    { SYM_MON, S_QUADRUPED + SYM_OFF_M, "S_quadruped" },
    { SYM_MON, S_RODENT + SYM_OFF_M, "S_rodent" },
    { SYM_MON, S_SPIDER + SYM_OFF_M, "S_spider" },
    { SYM_MON, S_TRAPPER + SYM_OFF_M, "S_trapper" },
    { SYM_MON, S_UNICORN + SYM_OFF_M, "S_unicorn" },
    { SYM_MON, S_VORTEX + SYM_OFF_M, "S_vortex" },
    { SYM_MON, S_WORM + SYM_OFF_M, "S_worm" },
    { SYM_MON, S_XAN + SYM_OFF_M, "S_xan" },
    { SYM_MON, S_LIGHT + SYM_OFF_M, "S_light" },
    { SYM_MON, S_LESSER_UNDEAD + SYM_OFF_M, "S_zruty" },
    { SYM_MON, S_ANGEL + SYM_OFF_M, "S_angel" },
    { SYM_MON, S_BAT + SYM_OFF_M, "S_bat" },
    { SYM_MON, S_CENTAUR + SYM_OFF_M, "S_centaur" },
    { SYM_MON, S_DRAGON + SYM_OFF_M, "S_dragon" },
    { SYM_MON, S_ELEMENTAL + SYM_OFF_M, "S_elemental" },
    { SYM_MON, S_FUNGUS + SYM_OFF_M, "S_fungus" },
    { SYM_MON, S_GNOLL + SYM_OFF_M, "S_gnoll" },
    { SYM_MON, S_GIANT + SYM_OFF_M, "S_giant" },
    { SYM_MON, S_JABBERWOCK + SYM_OFF_M, "S_jabberwock" },
    { SYM_MON, S_KOP + SYM_OFF_M, "S_kop" },
    { SYM_MON, S_LICH + SYM_OFF_M, "S_lich" },
    { SYM_MON, S_MODRON + SYM_OFF_M, "S_mummy" },
    { SYM_MON, S_NAGA + SYM_OFF_M, "S_naga" },
    { SYM_MON, S_OGRE + SYM_OFF_M, "S_ogre" },
    { SYM_MON, S_PUDDING + SYM_OFF_M, "S_pudding" },
    { SYM_MON, S_QUANTMECH + SYM_OFF_M, "S_quantmech" },
    { SYM_MON, S_RUSTMONST + SYM_OFF_M, "S_rustmonst" },
    { SYM_MON, S_SNAKE + SYM_OFF_M, "S_snake" },
    { SYM_MON, S_TROLL + SYM_OFF_M, "S_troll" },
    { SYM_MON, S_UNDER + SYM_OFF_M, "S_under" },
    { SYM_MON, S_VAMPIRE + SYM_OFF_M, "S_vampire" },
    { SYM_MON, S_WRAITH + SYM_OFF_M, "S_wraith" },
    { SYM_MON, S_XORN + SYM_OFF_M, "S_xorn" },
    { SYM_MON, S_YETI + SYM_OFF_M, "S_yeti" },
    { SYM_MON, S_GREATER_UNDEAD + SYM_OFF_M, "S_zombie" },
    { SYM_MON, S_HUMAN + SYM_OFF_M, "S_human" },
    { SYM_MON, S_GHOST + SYM_OFF_M, "S_ghost" },
    { SYM_MON, S_GOLEM + SYM_OFF_M, "S_golem" },
    { SYM_MON, S_DEMON + SYM_OFF_M, "S_demon" },
    { SYM_MON, S_EEL + SYM_OFF_M, "S_eel" },
    { SYM_MON, S_LIZARD + SYM_OFF_M, "S_lizard" },
    { SYM_MON, S_TREANT + SYM_OFF_M, "S_treant" },
    { SYM_MON, S_WORM_TAIL + SYM_OFF_M, "S_worm_tail" },
    { SYM_MON, S_MIMIC_DEF + SYM_OFF_M, "S_mimic_def" },
    { SYM_OTH, SYM_BOULDER + SYM_OFF_X, "S_boulder" },
    { SYM_OTH, SYM_INVISIBLE + SYM_OFF_X, "S_invisible" },
    { 0, 0, (const char *) 0 } /* fence post */
};

const char*
get_cmap_tilename(idx)
int idx;
{
    if (idx >= 0 && idx < MAX_CMAPPED_CHARS)
    {
        for (int i = 0; loadsyms[i].range > 0; i++)
        {
            if (loadsyms[i].range == SYM_PCHAR && loadsyms[i].idx == idx)
            {
                return loadsyms[i].name;
            }
        }
    }
    return "unknown cmap";
}


/*drawing.c*/

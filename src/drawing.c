/* GnollHack 4.0	drawing.c	$NHDT-Date: 1546656404 2019/01/05 02:46:44 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.59 $ */
/* Copyright (c) GnollHack Development Team 1992.                   */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "tcap.h"
#include "layer.h"

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
	{ MISCELLANEOUS_SYM, "miscellaneous items", "miscellaneous item" }
};

/* Default monster class symbols.  See monsym.h. */
const struct class_sym def_monsyms[MAX_MONSTER_CLASSES] = {
    { '\0', "", "" },
    { DEF_ANT, "", "ant or other insect" },
    { DEF_BLOB, "", "blob" },
    { DEF_COCKATRICE, "", "cockatrice" },
    { DEF_DOG, "", "dog, other canine, or hyena" },
    { DEF_EYE, "", "eye or sphere" },
    { DEF_FELINE, "", "cat or other feline" },
    { DEF_GNOME, "", "gnome, gremlin, or gargoyle" },
    { DEF_HUMANOID, "", "humanoid" },
    { DEF_IMP, "", "imp or minor demon" },
    { DEF_JELLY, "", "jelly" },
    { DEF_KOBOLD, "", "kobold" },
    { DEF_LEPRECHAUN, "", "leprechaun" },
    { DEF_MIMIC, "", "mimic" },
    { DEF_NYMPH, "", "nymph" },
    { DEF_ORC, "", "orc" },
    { DEF_PIERCER, "", "piercer" },
    { DEF_QUADRUPED, "", "quadruped" },
    { DEF_RODENT, "", "rodent" },
    { DEF_SPIDER, "", "arachnid or centipede" },
    { DEF_TRAPPER, "", "trapper or lurker above" },
    { DEF_UNICORN, "", "unicorn or horse" },
    { DEF_VORTEX, "", "vortex" },
    { DEF_WORM, "", "worm" },
    { DEF_XAN, "", "xan or other mythical/fantastic insect" },
    { DEF_LIGHT, "", "light" },
    { DEF_LESSER_UNDEAD, "", "zombie, skeleton, or other lesser undead" },
    { DEF_ANGEL, "", "angelic being" },
    { DEF_BAT, "", "bat or bird" },
    { DEF_CENTAUR, "", "centaur" },
    { DEF_DRAGON, "", "dragon" },
    { DEF_ELEMENTAL, "", "elemental" },
    { DEF_FUNGUS, "", "fungus or mold" },
    { DEF_GNOLL, "", "gnoll" },
    { DEF_GIANT, "", "giant humanoid" },
    { '\0', "", "invisible monster" },
    { DEF_JABBERWOCK, "", "jabberwock or juggernaut" },
    { DEF_KOP, "", "Keystone Kop" },
    { DEF_LICH, "", "lich" },
    { DEF_MODRON, "", "modron" },
    { DEF_NAGA, "", "naga" },
    { DEF_OGRE, "", "ogre" },
    { DEF_PUDDING, "", "pudding or ooze" },
    { DEF_QUANTMECH, "", "quantum mechanic" },
    { DEF_RUSTMONST, "", "rust monster or disenchanter" },
    { DEF_SNAKE, "", "snake" },
    { DEF_TROLL, "", "troll" },
    { DEF_UMBER, "", "umber hulk, otyugh, or chimera" },
    { DEF_VAMPIRE, "", "vampire" },
    { DEF_WRAITH, "", "wraith" },
    { DEF_XORN, "", "xorn" },
    { DEF_YETI, "", "ape- or bear-like creature" },
    { DEF_GREATER_UNDEAD, "", "mummy or other greater undead" },
    { DEF_HUMAN, "", "human or elf" },
    { DEF_GHOST, "", "ghost" },
    { DEF_GOLEM, "", "golem" },
	{ DEF_DEMON, "", "major demon or devil" },
    { DEF_EEL, "", "sea monster" },
    { DEF_LIZARD, "", "lizard" },
	{ DEF_TREANT, "", "treant" },
	{ DEF_WORM_TAIL, "", "long worm tail" },
    { DEF_MIMIC_DEF, "", "mimic" },
};

const struct symdef def_warnsyms[WARNCOUNT] = {
    /* white warning  */
    { '0', "unknown creature causing you little worry",    C(CLR_WHITE) },
    /* pink warning   */
    { '1', "unknown creature causing you worry",  C(CLR_RED) },
    /* red warning    */
    { '2', "unknown creature causing you concern",  C(CLR_RED) },
    /* ruby warning   */
    { '3', "unknown creature causing you anxiety", C(CLR_RED) },
    /* ruby warning   */
    { '4', "unknown creature causing you disquiet", C(CLR_RED) },
    /* purple warning */
    { '5', "unknown creature causing you alarm",    C(CLR_MAGENTA) },
    /* black warning  */
    { '6', "unknown creature causing you dread",    C(CLR_BRIGHT_MAGENTA) },
};


/*
 *  Default screen symbols with explanations and colors.
 */
/* You can define a new screen symbol / tile as a CMAP symbol, CMAP variation, or a GUI replacement tile */
/* NOTE: Use CMAP symbols for things that can have their own base symbols in TTY, e.g. walls. CMAP variations are color variations of the base tile with their own descriptions and GUI tiles. Replacements are tiles that show up only in GUI. */
const struct symdef_cmap defsyms[MAX_CMAPPED_CHARS] = {
/* 0*/ { ' ', "unexplored",          LAYER_FLOOR,   FALSE,     0, 0,                                          { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/  C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },  /* unexplored */
/* 1*/ { ' ', "stone",               LAYER_FLOOR,   FALSE,     0, 0,      { C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*5*/ C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR),      /*10*/  C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR), C(NO_COLOR) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                                {DUNGEON_NORMAL_STONE_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },  /* stone */
       { '|', "wall",                LAYER_FEATURE, FALSE,     0, 0,      { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                             {DUNGEON_NORMAL_VWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* vwall */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,      { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},         {DUNGEON_NORMAL_HWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* hwall */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_TLCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tlcorn */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_TRCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* trcorn */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_BLCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* blcorn */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_BRCORN_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* brcorn */
       { '-', "wall",                LAYER_FEATURE, FALSE,     CRWALL_VARIATIONS, CRWALL_VARIATION_OFFSET,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},        {DUNGEON_NORMAL_CRWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* crwall */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_TUWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tuwall */
       { '-', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_TDWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tdwall */
/*10*/ { '|', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_TLWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* tlwall */
       { '|', "wall",                LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                                              {DUNGEON_NORMAL_TRWALL_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* trwall */
       { '.', "doorway",             LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '-', "open door",           LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },           /* vodoor */
       { '|', "open door",           LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },           /* hodoor */
       { '+', "closed door",         LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* vcdoor */
       { '+', "closed door",         LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_RED), C(CLR_BROWN), C(CLR_BROWN),   /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* hcdoor */
       { '.', "broken door",         LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '.', "broken door",         LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '.', "open portcullis",     LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),   /*5*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),      /*10*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '.', "open portcullis",     LAYER_FEATURE, FALSE,     0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),   /*5*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN),      /*10*/ C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },              /* ndoor */
       { '#', "iron bars",           LAYER_FEATURE, FALSE,     0, 0,     { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL),      /*5*/ C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL),      /*10*/C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {IRON_BAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },            /* bars */
       { '#', "tree",                LAYER_COVER_FEATURE, TRUE,     0, 0,     { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {TREE_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* tree */
       { '.', "floor of a room",     LAYER_FLOOR, FALSE,       FLOOR_VARIATIONS, FLOOR_VARIATION_OFFSET,
                                               { C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
/*20*/ { '.', "dark part of a room", LAYER_FLOOR, FALSE,       0, 0,
                                               { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BROWN), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* dark room */
/*+1*/ { ',', "grass",               LAYER_FLOOR, FALSE,       GRASS_VARIATIONS, GRASS_VARIATION_OFFSET,     { C(CLR_BRIGHT_GREEN), C(CLR_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),/*5*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), /*10*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },		 /* grass */
/*+1*/ { ',', "grass in dark",       LAYER_FLOOR, FALSE,       0, 0,
                                               { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BROWN), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* grass in dark */
/*+1*/ { ',', "ground",              LAYER_FLOOR, FALSE,       GROUND_VARIATIONS, GROUND_VARIATION_OFFSET,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),/*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },		 /* grass */
/*+1*/ { ',', "ground in dark",      LAYER_FLOOR, FALSE,       0, 0,
                                               { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BROWN), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* grass in dark */
       { '#', "corridor",            LAYER_FLOOR, FALSE,       0, 0,
                                               { C(CLR_GRAY), C(CLR_GRAY), C(CLR_BROWN), C(CLR_GRAY), C(CLR_GRAY),     /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },             /* dark corr */
       { '#', "lit corridor",        LAYER_FLOOR, FALSE,       0, 0,
                                               { C(CLR_WHITE), C(CLR_WHITE), C(CLR_YELLOW), C(CLR_WHITE), C(CLR_WHITE),    /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),      /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   /* lit corr (see mapglyph.c) */
       { '<', "staircase up",        LAYER_FEATURE, FALSE,      STAIRCASE_VARIATIONS, STAIRCASE_UP_VARIATION_OFFSET,
                                               { C(CLR_GRAY), C(CLR_YELLOW), C(CLR_RED), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),     /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_UPSTAIRS_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* upstair */
       { '>', "staircase down",      LAYER_FEATURE, FALSE,      STAIRCASE_VARIATIONS, STAIRCASE_DOWN_VARIATION_OFFSET,
                                               { C(CLR_GRAY), C(CLR_YELLOW), C(CLR_RED), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),     /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* dnstair */
       { '<', "ladder up",           LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_BROWN), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },           /* upladder */
       { '>', "ladder down",         LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_BROWN), C(CLR_YELLOW), C(CLR_YELLOW), C(CLR_BROWN), C(CLR_BROWN), /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* dnladder */
       { '_', "altar",               LAYER_FEATURE, FALSE,      ALTAR_VARIATIONS, ALTAR_VARIATION_OFFSET,
                                               { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
/*+1*/ { '0', "boulder",             LAYER_COVER_OBJECT, TRUE,        0, 0,     { C(CLR_GRAY), C(CLR_BROWN), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_BRIGHT_BLUE), /*5*/ C(CLR_WHITE), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0 /*DUNGEON_NORMAL_BOULDER_ENLARGEMENT*/, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { '|', "grave",               LAYER_FEATURE, TRUE,      0, 0,     { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },               /* grave */
       { '\\', "opulent throne",     LAYER_FEATURE, FALSE,      0, 0,     { C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD),           /*5*/C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD),            /*10*/ C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD), C(HI_GOLD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* throne */
/*30*/ { '#', "sink",                LAYER_FEATURE, TRUE,      0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_RED), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* sink */
       { '{', "fountain",            LAYER_FEATURE, TRUE,      FOUNTAIN_VARIATIONS, FOUNTAIN_VARIATION_OFFSET,
                                               { C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { '}', "water",               LAYER_FLOOR, FALSE,        0, 0,     { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* pool */
       { '.', "ice",                 LAYER_FLOOR, FALSE,        0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                  /* ice */
       { '}', "molten lava",         LAYER_FLOOR, FALSE,        0, 0,     { C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },           /* lava */
       { '.', "lowered drawbridge",  LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },  /* vodbridge */
       { '.', "lowered drawbridge",  LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },  /* hodbridge */
       { '#', "raised drawbridge",   LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   /* vcdbridge */
       { '#', "raised drawbridge",   LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   /* hcdbridge */
       { ' ', "air",                 LAYER_FLOOR, FALSE,        0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                  /* open air */
/*40*/ { '#', "cloud",               LAYER_FEATURE, FALSE,      0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* [part of] a cloud */
       { '}', "water",               LAYER_FLOOR, FALSE,        0, 0,     { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* under water */
       { '^', "arrow trap",          LAYER_FEATURE, FALSE,    0, 0,     { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },           /* trap */
       { '^', "dart trap",           LAYER_FEATURE, FALSE,    0, 0,     { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "falling rock trap",   LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },    /* trap */
       { '^', "squeaky board",       LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* trap */
       { '^', "bear trap",           LAYER_FEATURE, FALSE,    0, 0,     { C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL), C(HI_METAL) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "land mine",           LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },             /* trap */
       { '^', "rolling boulder trap",LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
       { '^', "sleeping gas trap",   LAYER_FEATURE, FALSE,    0, 0,     { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* trap */
/*50*/ { '^', "rust trap",           LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "fire trap",           LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },          /* trap */
       { '^', "pit",                 LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* trap */
       { '^', "spiked pit",          LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },          /* trap */
       { '^', "hole",                LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* trap */
       { '^', "trap door",           LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },           /* trap */
       { '^', "teleportation trap",  LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },  /* trap */
       { '^', "level teleporter",    LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },    /* trap */
       { '^', "magic portal",        LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
       { '"', "web",                 LAYER_FEATURE, TRUE,    0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                    /* web */
/*60*/ { '^', "statue trap",         LAYER_FEATURE, FALSE,   0, 0,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },            /* trap */
       { '^', "magic trap",          LAYER_FEATURE, FALSE,    0, 0,     { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },               /* trap */
       { '^', "anti-magic field",    LAYER_FEATURE, FALSE,    0, 0,     { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* trap */
       { '^', "polymorph trap",      LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
/*+1*/ { '^', "octahedral magic portal", LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
/*+1*/ { '^', "tetrahedral magic portal",LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
/*+1*/ { '^', "cubical magic portal",    LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
/*+1*/ { '^', "dodecahedral magic portal", LAYER_FEATURE, FALSE,    0, 0,  { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /* trap */
       { '~', "vibrating square",    LAYER_FEATURE, FALSE,    0, 0,     { C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },    /* "trap" */
/* zap colors are changed by mapglyph() to match type of beam */
       { '|', "",               LAYER_MISSILE, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* vbeam */
       { '-', "",               LAYER_MISSILE, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* hbeam */
       { '\\', "",              LAYER_MISSILE, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },               /* lslant */
       { '/', "",               LAYER_MISSILE, FALSE,    0, 0, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* rslant */
       { '*', "",               LAYER_MISSILE, FALSE,    0, 0, { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },               /* dig beam */
       { '!', "",               LAYER_MISSILE, FALSE,    0, 0, { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },               /* camera flash beam */
       { ')', "",               LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open left */
/*70*/ { '(', "",               LAYER_MISSILE, FALSE,    0, 0, { C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD), C(HI_WOOD) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* boomerang open right */
       { '0', "",               LAYER_MONSTER_EFFECT, FALSE,  0, 0,     { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                  /* 4 magic shield symbols */
       { '#', "",               LAYER_MONSTER_EFFECT, FALSE, 0, 0,      { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
       { '@', "",               LAYER_MONSTER_EFFECT, FALSE,  0, 0,    { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
       { '*', "",               LAYER_MONSTER_EFFECT, FALSE,  0, 0,        { C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP), C(HI_ZAP) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
       { '?', "",               LAYER_MONSTER_EFFECT, FALSE,    0, 0, { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   /* talk effect */
       { '#', "poison cloud",   LAYER_ENVIRONMENT, FALSE,    0, 0, { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   /* part of a cloud */
       { '?', "valid position", LAYER_GENERAL_UI, FALSE,    0, 0, { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }, /*  target position */
       /* swallow colors are changed by mapglyph() to match engulfing monst */
       { '/', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow top left      */
       { '-', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow top center    */
       { '\\', "",        LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* swallow top right     */
/*80*/ { '|', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow middle left   */
       { '|', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow middle right  */
       { '\\', "",        LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* swallow bottom left   */
       { '-', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow bottom center */
       { '/', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* swallow bottom right  */
       /* explosion colors are changed by mapglyph() to match type of expl. */
       { '/', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion top left     */
       { '-', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion top center   */
       { '\\', "",        LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* explosion top right    */
       { '|', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion middle left  */
       { ' ', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion middle center*/
/*90*/ { '|', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion middle right */
       { '\\', "",        LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* explosion bottom left  */
       { '-', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion bottom center*/
       { '/', "",         LAYER_GENERAL_EFFECT, FALSE, 0, 0,    { C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE), C(CLR_ORANGE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },        /* explosion bottom right */
};

/* NOTE: Use CMAP variations for differently colored base symbols, e.g. altars, stairs, and fountains, and for storing autodraw base tiles */
/* GUI tiles that appear the exactly same in TTY graphics should be implemented using replacements, unless they are somehow more conveniently defined in a table format for several CMAP types */
const struct symdef_cmap_variation defsym_variations[MAX_VARIATIONS] =
{
       { S_crwall, "crwall-all-ends", "wall", FALSE, { C(CLR_GRAY), C(CLR_BROWN), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                 /* crwall */

       { S_room, "room-floor-variation-1", "cracks on the floor", FALSE, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "room-floor-variation-2", "cracks on the floor", FALSE, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */
       { S_room, "room-floor-variation-3", "cracks on the floor", FALSE, { C(CLR_GRAY), C(CLR_GRAY), C(CLR_YELLOW), C(CLR_GRAY), C(CLR_GRAY),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* room */

       { S_grass, "grass-variation-1", "flowering grass",  FALSE,   { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_YELLOW), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),    /*5*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),      /*10*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */
       { S_grass, "grass-variation-2", "flowering grass",  FALSE,       { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_YELLOW), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),    /*5*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),      /*10*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */
       { S_grass, "grass-variation-3",   "flowering grass",  FALSE,          { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_YELLOW), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),    /*5*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),      /*10*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */

       { S_ground, "ground-variation-dirt", "dirt",  FALSE,   { C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),    /*5*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN),      /*10*/ C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BROWN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */
       { S_ground, "ground-variation-grass", "grass",  FALSE,       { C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_YELLOW), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),    /*5*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN),      /*10*/ C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN), C(CLR_BRIGHT_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */
       { S_ground, "ground-variation-swamp", "swampy ground",  FALSE,          { C(CLR_GREEN), C(CLR_GREEN), C(CLR_YELLOW), C(CLR_GREEN), C(CLR_GREEN),    /*5*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN),      /*10*/ C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN), C(CLR_GREEN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* grass */

       { S_upstair, "branch-staircase-up", "",  FALSE,          { C(CLR_BRIGHT_BLUE), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),   /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_BRANCH_UPSTAIRS_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* upstair */
       { S_upstair, "special-branch-staircase-up",  "", FALSE, { C(CLR_WHITE), C(CLR_YELLOW), C(CLR_RED), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {DUNGEON_NORMAL_SPECIAL_BRANCH_UPSTAIRS_ENLARGEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },         /* upstair */

       { S_dnstair, "branch-staircase-down",   "",  FALSE,     { C(CLR_YELLOW), C(CLR_WHITE), C(CLR_ORANGE), C(CLR_GRAY), C(CLR_BRIGHT_BLUE), /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* dnstair */
       { S_dnstair, "special-branch-staircase-down", "", FALSE,  { C(CLR_BLACK), C(CLR_YELLOW), C(CLR_RED), C(CLR_GRAY), C(CLR_BRIGHT_BLUE),    /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_BLACK),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },       /* dnstair */

       { S_altar, "lawful-altar",  "lawful altar", FALSE,     { C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), /*5*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE),  /*10*/ C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE), C(CLR_WHITE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {LAWFUL_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "neutral-altar", "neutral altar", FALSE,     { C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),      /*5*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY),     /*10*/ C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY), C(CLR_GRAY) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {NEUTRAL_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "chaotic-altar", "chaotic altar", FALSE,     { C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*5*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), /*10*/ C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK), C(CLR_BLACK) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {CHAOTIC_ALTAR_REPLACEMENT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "altar-of-Moloch", "altar of Moloch", FALSE,   { C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED),           /*5*/ C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_GRAY),         /*10*/ C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED), C(CLR_RED) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */
       { S_altar, "high-altar",  "high altar",  FALSE,      { C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), /*5*/ C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), /*10*/ C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA), C(CLR_BRIGHT_MAGENTA) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },                /* altar */

       { S_fountain, "magic-fountain", "",  TRUE,               { C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-healing",  "", TRUE,         { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-mana",     "", TRUE,         { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-power",    "", TRUE,         { C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE), C(CLR_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-poison",   "", TRUE,         { C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN), C(CLR_CYAN) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */
       { S_fountain, "fountain-of-spring-water", "", TRUE,     { C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE), C(CLR_BRIGHT_BLUE) }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} , {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },      /* fountain */

};



/* default rogue level symbols */
static const uchar def_r_oc_syms[MAX_OBJECT_CLASSES] = {
/* 0*/ '\0', ILLOBJ_SYM, WEAPON_SYM, ']', /* armor */
       RING_SYM,
/* 5*/ ',',                     /* amulet */
       TOOL_SYM, ':',           /* food */
       POTION_SYM, SCROLL_SYM,
/*10*/ SPBOOK_SYM, WAND_SYM,
       GEM_SYM,                /* gold -- yes it's the same as gems */
       GEM_SYM, ROCK_SYM,
/*15*/ BALL_SYM, CHAIN_SYM, VENOM_SYM
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
    nhsym boulder = (nhsym) iflags.bouldersym;

    if (!boulder)
        boulder = def_oc_syms[ROCK_CLASS].sym; /* (nhsym) ROCK_SYM */
    showsyms[SYM_BOULDER + SYM_OFF_X] = boulder;
    l_syms[SYM_BOULDER + SYM_OFF_X] = boulder;
    r_syms[SYM_BOULDER + SYM_OFF_X] = boulder;
}

void
init_showsyms()
{
    register int i;

    for (i = 0; i < MAX_CMAPPED_CHARS; i++)
        showsyms[i + SYM_OFF_P] = defsyms[i].sym;
    for (i = 0; i < MAX_OBJECT_CLASSES; i++)
        showsyms[i + SYM_OFF_O] = def_oc_syms[i].sym;
    for (i = 0; i < MAX_MONSTER_CLASSES; i++)
        showsyms[i + SYM_OFF_M] = def_monsyms[i].sym;
    for (i = 0; i < WARNCOUNT; i++)
        showsyms[i + SYM_OFF_W] = def_warnsyms[i].sym;
    for (i = 0; i < MAXOTHER; i++) {
        if (i == SYM_BOULDER)
            showsyms[i + SYM_OFF_X] = iflags.bouldersym
                                        ? iflags.bouldersym
                                        : def_oc_syms[ROCK_CLASS].sym;
        else if (i == SYM_INVISIBLE)
            showsyms[i + SYM_OFF_X] = DEF_INVISIBLE;
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
                                      : def_oc_syms[ROCK_CLASS].sym;
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
                                      : def_oc_syms[ROCK_CLASS].sym;
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
int val;
{
    l_syms[symp->idx] = val;
}

void
update_r_symset(symp, val)
struct symparse *symp;
int val;
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
    { SYM_PCHAR, S_altar, "S_altar" },
    { SYM_PCHAR, S_extra_boulder, "S_extra_boulder" },
    { SYM_PCHAR, S_grave, "S_grave" },
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
    { SYM_PCHAR, S_octahedral_portal, "S_octahedral_portal" },
    { SYM_PCHAR, S_tetrahedral_portal, "S_tetrahedral_portal" },
    { SYM_PCHAR, S_cubical_portal, "S_cubical_portal" },
    { SYM_PCHAR, S_dodecahedral_portal, "S_dodecahedral_portal" },
    { SYM_PCHAR, S_vibrating_square, "S_vibrating_square" },
    { SYM_PCHAR, S_vbeam, "S_vbeam" },
    { SYM_PCHAR, S_hbeam, "S_hbeam" },
    { SYM_PCHAR, S_lslant, "S_lslant" },
    { SYM_PCHAR, S_rslant, "S_rslant" },
    { SYM_PCHAR, S_digbeam, "S_digbeam" },
    { SYM_PCHAR, S_flashbeam, "S_flashbeam" },
    { SYM_PCHAR, S_boomleft, "S_boomleft" },
    { SYM_PCHAR, S_boomright, "S_boomright" },
    { SYM_PCHAR, S_goodpos, "S_goodpos" },
    { SYM_PCHAR, S_ss1, "S_ss1" },
    { SYM_PCHAR, S_ss2, "S_ss2" },
    { SYM_PCHAR, S_ss3, "S_ss3" },
    { SYM_PCHAR, S_ss4, "S_ss4" },
    { SYM_PCHAR, S_sw_tl, "S_sw_tl" },
    { SYM_PCHAR, S_sw_tc, "S_sw_tc" },
    { SYM_PCHAR, S_sw_tr, "S_sw_tr" },
    { SYM_PCHAR, S_sw_ml, "S_sw_ml" },
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
    { SYM_MON, S_UMBER + SYM_OFF_M, "S_umber" },
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

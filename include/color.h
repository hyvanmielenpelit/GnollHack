/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    color.h    $NHDT-Date: 1432512776 2015/05/25 00:12:56 $  $NHDT-Branch: master $:$NHDT-Revision: 1.13 $ */
/* Copyright (c) Steve Linhart, Eric Raymond, 1989. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef COLOR_H
#define COLOR_H

/*
 * The color scheme used is tailored for an IBM PC.  It consists of the
 * standard 8 colors, followed by their bright counterparts.  There are
 * exceptions, these are listed below.    Bright black doesn't mean very
 * much, so it is used as the "default" foreground color of the screen.
 */
#define CLR_BLACK 0
#define CLR_RED 1
#define CLR_GREEN 2
#define CLR_BROWN 3 /* on IBM, low-intensity yellow is brown */
#define CLR_BLUE 4
#define CLR_MAGENTA 5
#define CLR_CYAN 6
#define CLR_GRAY 7 /* low-intensity white */
#define NO_COLOR 8
#define CLR_ORANGE 9
#define CLR_BRIGHT_GREEN 10
#define CLR_YELLOW 11
#define CLR_BRIGHT_BLUE 12
#define CLR_BRIGHT_MAGENTA 13
#define CLR_BRIGHT_CYAN 14
#define CLR_WHITE 15
#define CLR_MAX 16

#define CLR_ALT_GOLD 3 /* Gold instead of brown */

/* The "half-way" point for tty based color systems.  This is used in */
/* the tty color setup code.  (IMHO, it should be removed - dean).    */
#define BRIGHT 8

/* these can be configured */
#define HI_OBJ CLR_MAGENTA
#define HI_METAL CLR_CYAN
#define HI_COPPER CLR_YELLOW
#define HI_SILVER CLR_GRAY
#define HI_GOLD CLR_YELLOW
#define HI_LEATHER CLR_BROWN
#define HI_CLOTH CLR_BROWN
#define HI_ORGANIC CLR_BROWN
#define HI_WOOD CLR_BROWN
#define HI_PAPER CLR_WHITE
#define HI_GLASS CLR_BRIGHT_CYAN
#define HI_MINERAL CLR_GRAY
#define DRAGON_SILVER CLR_BRIGHT_CYAN
#define HI_ZAP CLR_BRIGHT_BLUE

#define HI_MINES CLR_BROWN
#define HI_GEHENNOM CLR_ORANGE
#define HI_UNDEAD CLR_BLACK
#define HI_SOKOBAN CLR_BLUE
#define HI_ELEMENTAL_PLANES CLR_GRAY /* HI_MINERAL */
#define HI_ASTRAL CLR_BRIGHT_MAGENTA
#define HI_GARDEN CLR_GREEN
#define HI_MODRON CLR_WHITE /* color of modronite */
#define HI_SHOP CLR_BROWN /* HI_WOOD */
#define HI_REINFORCED CLR_CYAN /* HI_METAL */
#define HI_NEST CLR_YELLOW
#define HI_TOWER CLR_BRIGHT_BLUE /* HI_ZAP */
#define HI_OPULENT CLR_MAGENTA /* could be also CLR_MAGENTA but best choice seems to depend on the color scheme */
#define HI_TEMPLE CLR_WHITE

#define CLR_MSG_POSITIVE      CLR_GREEN
#define CLR_MSG_NEGATIVE      CLR_RED
#define CLR_MSG_WARNING       CLR_ORANGE
#define CLR_MSG_ATTENTION     CLR_YELLOW
#define CLR_MSG_HALLUCINATED  CLR_BRIGHT_MAGENTA
#define CLR_MSG_GOD           CLR_MAGENTA
#define CLR_MSG_SPELL         CLR_BRIGHT_BLUE /* The same as HI_ZAP */
#define CLR_MSG_MYSTICAL      CLR_CYAN
#define CLR_MSG_SUCCESS       CLR_BRIGHT_GREEN
#define CLR_MSG_FAIL          CLR_BROWN
#define CLR_MSG_HINT          CLR_BRIGHT_CYAN

struct menucoloring {
    struct nhregex *match;
    char *origstr;
    int color, attr;
    struct menucoloring *next;
};

#endif /* COLOR_H */

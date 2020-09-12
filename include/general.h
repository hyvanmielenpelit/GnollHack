/*
 * general.h
 * Copyright (c) Janne Gustafsson, 2020
 */

#ifndef GENERAL_H
#define GENERAL_H

 /* types of calls to bhit() */
enum bhit_call_types {
    ZAPPED_WAND = 0,
    THROWN_WEAPON = 1,
    THROWN_TETHERED_WEAPON = 2,
    KICKED_WEAPON = 3,
    FLASHED_LIGHT = 4,
    INVIS_BEAM = 5,
    GOLF_SWING = 6
};

/* Total number of cmap indices in the shield_static[] array. */
#define SHIELD_COUNT 21


/* Various info structs */
struct replacement_info {
    struct obj* object;
};


struct extended_menu_info {
    struct obj* object;
    int color;
    unsigned long menu_flags;
};




#endif /* GENERAL_H */


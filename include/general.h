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


#endif /* GENERAL_H */


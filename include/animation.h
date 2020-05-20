/*
 * animation.h
 * Copyright 2020 by Janne Gustafsson
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_FRAMES_PER_ANIMATION 32

enum main_tile_use_types
{
    ANIMATION_MAIN_TILE_USE_FIRST = 0,
    ANIMATION_MAIN_TILE_USE_LAST,
    ANIMATION_MAIN_TILE_IGNORE
};

struct animation_definition {
    char* animation_name;
    short number_of_tiles;
    int number_of_frames;
    int glyph_offset;
    int intervals_between_frames;
    enum main_tile_use_types main_tile_use_style; /* 0 = play as first tile and frame, 1 = play as last tile and frame, 2 = ignore */
    short frame2tile[MAX_FRAMES_PER_ANIMATION];
};

enum animation_types
{
    NO_ANIMATION = 0,
    HANDCRAFTED_CANDLE_LIT_ANIMATION,
    TWISTED_CANDLE_LIT_ANIMATION /* Keep this last */
};

#define NUM_ANIMATIONS TWISTED_CANDLE_LIT_ANIMATION

#define HANDCRAFTED_CANDLE_ANIMATION_OFF (0)
#define HANDCRAFTED_CANDLE_ANIMATION_FRAMES 1
#define TWISTED_CANDLE_ANIMATION_OFF (HANDCRAFTED_CANDLE_ANIMATION_FRAMES + HANDCRAFTED_CANDLE_ANIMATION_OFF)
#define TWISTED_CANDLE_ANIMATION_FRAMES 6

#define MAX_ANIMATION_FRAMES (TWISTED_CANDLE_ANIMATION_FRAMES + TWISTED_CANDLE_ANIMATION_OFF)

extern NEARDATA struct animation_definition animations[];


#endif /* ANIMATION_H */
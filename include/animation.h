/*
 * animation.h
 * Copyright 2020 by Janne Gustafsson
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_FRAMES_PER_ANIMATION 32

struct animation_definition {
    char* animation_name;
    short number_of_tiles;
    int number_of_frames;
    int glyph_offset;
    int intervals_between_frames;
    short frame2tile[MAX_FRAMES_PER_ANIMATION];
};

enum animation_types
{
    HANDCRAFTED_CANDLE_ANIMATION = 0,
    TWISTED_CANDLE_ANIMATION = 1,
    MAX_ANIMATIONS
};

#define HANDCRAFTED_CANDLE_ANIMATION_OFF (0)
#define HANDCRAFTED_CANDLE_ANIMATION_FRAMES 6
#define TWISTED_CANDLE_ANIMATION_OFF (HANDCRAFTED_CANDLE_ANIMATION_FRAMES + HANDCRAFTED_CANDLE_ANIMATION_OFF)
#define TWISTED_CANDLE_ANIMATION_FRAMES 6

#define MAX_ANIMATION_FRAMES (TWISTED_CANDLE_ANIMATION_FRAMES + TWISTED_CANDLE_ANIMATION_OFF)

extern NEARDATA struct animation_definition animations[];


#endif /* ANIMATION_H */
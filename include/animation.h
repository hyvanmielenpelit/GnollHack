/*
 * animation.h
 * Copyright 2020 by Janne Gustafsson
 */

#ifndef ANIMATION_H
#define ANIMATION_H

struct animation_definition {
    char* animation_name;
    uchar number_of_frames;
    int glyph_offset;
    uchar intervals_between_frames;
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
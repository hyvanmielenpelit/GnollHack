/*
 * Copyright 2020 by Janne Gustafsson
 */
#include "hack.h"

NEARDATA struct animation_definition animations[MAX_ANIMATIONS] =
{
    { "handcrafted-candle-lit-animation", 6, 
      HANDCRAFTED_CANDLE_ANIMATION_FRAMES, HANDCRAFTED_CANDLE_ANIMATION_OFF, 3,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
    },
    { "twisted-candle-lit-animation", 6,
      TWISTED_CANDLE_ANIMATION_FRAMES, TWISTED_CANDLE_ANIMATION_OFF, 3,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
    }
};


void init_animations();

void
init_animations()
{
    /* force linkage */
    return;
}

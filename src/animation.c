/*
 * Copyright 2020 by Janne Gustafsson
 */
#include "hack.h"

NEARDATA struct animation_definition animations[MAX_ANIMATIONS] =
{
    { "handcrafted-candle-lit-animation", HANDCRAFTED_CANDLE_ANIMATION_FRAMES, HANDCRAFTED_CANDLE_ANIMATION_OFF, 3},
    { "twisted-candle-lit-animation", TWISTED_CANDLE_ANIMATION_FRAMES, TWISTED_CANDLE_ANIMATION_OFF, 3}
};


void init_animations();

void
init_animations()
{
    /* force linkage */
    return;
}

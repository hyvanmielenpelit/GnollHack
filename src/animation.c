/*
 * Copyright 2020 by Janne Gustafsson
 */
#include "hack.h"
#ifdef USE_TILES
extern short glyph2tile[MAX_GLYPH];
extern short tile2animation[MAX_GLYPH];
#endif

NEARDATA struct animation_definition animations[NUM_ANIMATIONS + 1] =
{
    { "", 0,
      0, 0, 0,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
    },

    { "handcrafted-candle-lit-animation", 1, 
      HANDCRAFTED_CANDLE_ANIMATION_FRAMES, HANDCRAFTED_CANDLE_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
    },
    { "twisted-candle-lit-animation", 6,
      TWISTED_CANDLE_ANIMATION_FRAMES, TWISTED_CANDLE_ANIMATION_OFF, 3,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
    }
};


void
init_animations()
{
    /* force linkage */
    return;
}

short
get_player_stand_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    return 0;
}

short
get_player_attack_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    return 0;
}

short
maybe_get_animated_tile(ntile, interval_counter, mapAnimated)
short ntile;
unsigned long interval_counter;
boolean* mapAnimated;
{
#ifdef USE_TILES
    short animation_idx = tile2animation[ntile];
    if (animation_idx > 0)
    {
        if (mapAnimated)
            *mapAnimated = TRUE;

        int main_tile_frame_position = -1; /* ignore */
        if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_FIRST)
        {
            main_tile_frame_position = 0;
        }
        else if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_LAST)
        {
            main_tile_frame_position = animations[animation_idx].number_of_frames;
        }
        int additional_tile_num = (main_tile_frame_position > -1 ? 1 : 0);
        int animation_tile_offset = (main_tile_frame_position == 0 ? 1 : 0);
        int numframes = animations[animation_idx].number_of_frames + additional_tile_num; /* add original tile as the first tile and frame */
        int current_animation_frame = (interval_counter / animations[animation_idx].intervals_between_frames) % numframes;
        
        if (current_animation_frame > main_tile_frame_position) /* 0 is the original picture */
        {
            int animation_frame_index = current_animation_frame - animation_tile_offset;
            if (animation_frame_index >= animations[animation_idx].number_of_frames)
                return ntile; /* original tile is the last if number_of_frames is exceeded by numframes */

            int animation_tile_glyph_idx = animations[animation_idx].frame2tile[animation_frame_index];
            if(animation_tile_glyph_idx <= -1)
                return ntile; /* original tile if frame tile mapping specifies a negative number */

            int animation_glyph = animation_tile_glyph_idx + animations[animation_idx].glyph_offset + GLYPH_ANIMATION_OFF;
            return glyph2tile[animation_glyph]; /* animated version selected */
        }
    }
#endif
    return ntile;
}

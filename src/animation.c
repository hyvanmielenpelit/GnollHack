/*
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "hack.h"

#ifdef USE_TILES
extern short glyph2tile[MAX_GLYPH];
extern short tile2animation[MAX_GLYPH];
extern short tile2enlargement[MAX_GLYPH];
#endif


/*
 * Animations 
 */
NEARDATA struct animation_definition animations[NUM_ANIMATIONS + 1] =
{
    { "", 0, 0,
      0, 0, 0,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "handcrafted-candle-lit-animation", MAGIC_CANDLE + GLYPH_OBJ_LIT_OFF,  1,
      HANDCRAFTED_CANDLE_ANIMATION_FRAMES, HANDCRAFTED_CANDLE_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "elf_wizard_female_cast-animation", 369 + GLYPH_PLAYER_CAST_OFF,  1,
      PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION_FRAMES, PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "twisted-candle-lit-animation", WAX_CANDLE + GLYPH_OBJ_LIT_OFF, 6,
      TWISTED_CANDLE_ANIMATION_FRAMES, TWISTED_CANDLE_ANIMATION_OFF, 3,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
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
get_player_throw_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    return 0;
}

short
get_player_fire_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    return 0;
}

short
get_player_cast_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
        return PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION;

    return 0;
}

short
get_player_special_attack_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    return 0;
}

short
get_player_item_use_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    return 0;
}

short
get_player_door_use_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
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
        if (animations[animation_idx].number_of_frames < 1)
            return ntile;

        if (mapAnimated)
            *mapAnimated = TRUE;

        char main_tile_frame_position = -1; /* ignore */
        if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_FIRST)
        {
            main_tile_frame_position = 0;
        }
        else if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_LAST)
        {
            main_tile_frame_position = animations[animation_idx].number_of_frames;
        }
        char additional_tile_num = (main_tile_frame_position > -1 ? 1 : 0);
        char animation_tile_offset = (main_tile_frame_position == 0 ? 1 : 0);
        unsigned long numframes = (unsigned long)(animations[animation_idx].number_of_frames + additional_tile_num); /* add original tile as the first tile and frame */
        char current_animation_frame = (char)((interval_counter / (unsigned long)animations[animation_idx].intervals_between_frames) % numframes);
        
        if (current_animation_frame > main_tile_frame_position) /* 0 is the original picture */
        {
            char animation_frame_index = current_animation_frame - animation_tile_offset;
            if (animation_frame_index >= animations[animation_idx].number_of_frames)
                return ntile; /* original tile is the last if number_of_frames is exceeded by numframes */

            char animation_tile_glyph_idx = animations[animation_idx].frame2tile[animation_frame_index];
            if(animation_tile_glyph_idx <= -1)
                return ntile; /* original tile if frame tile mapping specifies a negative number */

            int animation_glyph = (int)animation_tile_glyph_idx + animations[animation_idx].glyph_offset + GLYPH_ANIMATION_OFF;
            return glyph2tile[animation_glyph]; /* animated version selected */
        }
    }
#endif
    return ntile;
}




/*
 * Enlargements
 */
NEARDATA struct enlargement_definition enlargements[NUM_ENLARGEMENTS + 1] =
{
    { "", 
      0,
      0, 0, 0,
      0, 0, 0,
      { 0, 1, 2, 3, 4 },
      { -1, 0, 1, -1, 1 }
    },

    { "dracolich-enlargement", 
      PM_DRACOLICH + GLYPH_MON_OFF, 5,
      DRACOLICH_ENLARGEMENT_FRAMES, DRACOLICH_ENLARGEMENT_OFF,
      3, 2, 0,
      { 0, 1, 2, 3, 4 },
      { -1, 0, 1, -1, 1 }
    }
};


short
get_player_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_attack_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_throw_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_fire_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_cast_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_special_attack_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_item_use_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

short
get_player_door_use_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}

/* animation.c */



/*
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "hack.h"

/*
 * Animations 
 */
NEARDATA struct animation_definition animations[NUM_ANIMATIONS + 1] =
{
    { "", 0,
      0, 0, 0,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "handcrafted-candle-lit-animation",  1,
      HANDCRAFTED_CANDLE_ANIMATION_FRAMES, HANDCRAFTED_CANDLE_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "elf_wizard_female_cast-animation", 1,
      PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION_FRAMES, PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "twisted-candle-lit-animation", 6,
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
get_player_kick_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
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
get_player_death_animation(roleidx, raceidx, genderidx, alignmentidx, levelidx)
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
    { "", 0,
      0, 0,
      0, 0, 0,
      { 0, 1, 2, 3, 4 },
      { -1, 0, 1, -1, 1 }
    },

    { "dracolich-enlargement", 5,
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
get_player_kick_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
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

short
get_player_death_enlargement(roleidx, raceidx, genderidx, alignmentidx, levelidx)
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    return 0;
}


short
get_animation_base_tile(animidx)
short animidx;
{
    for (int i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        if (mons[i].animation.stand == animidx)
            return glyph2tile[i + GLYPH_MON_OFF];
        if (mons[i].animation.attacks == animidx)
            return glyph2tile[i + GLYPH_ATTACK_OFF];
        if (mons[i].animation.throws == animidx)
            return glyph2tile[i + GLYPH_THROW_OFF];
        if (mons[i].animation.fire == animidx)
            return glyph2tile[i + GLYPH_FIRE_OFF];
        if (mons[i].animation.cast == animidx)
            return glyph2tile[i + GLYPH_CAST_OFF];
        if (mons[i].animation.special_attack == animidx)
            return glyph2tile[i + GLYPH_SPECIAL_ATTACK_OFF];
        if (mons[i].animation.kick == animidx)
            return glyph2tile[i + GLYPH_KICK_OFF];
        if (mons[i].animation.item_use == animidx)
            return glyph2tile[i + GLYPH_ITEM_USE_OFF];
        if (mons[i].animation.door_use == animidx)
            return glyph2tile[i + GLYPH_DOOR_USE_OFF];
        if (mons[i].animation.death == animidx)
            return glyph2tile[i + GLYPH_DEATH_OFF];
        if (mons[i].animation.corpse == animidx)
            return glyph2tile[i + GLYPH_BODY_OFF];
        if (mons[i].animation.statue == animidx)
            return glyph2tile[i + GLYPH_STATUE_OFF];

        if (mons[i].female_animation.stand == animidx)
            return glyph2tile[i + GLYPH_FEMALE_MON_OFF];
        if (mons[i].female_animation.attacks == animidx)
            return glyph2tile[i + GLYPH_FEMALE_MON_OFF];
        if (mons[i].female_animation.throws == animidx)
            return glyph2tile[i + GLYPH_FEMALE_THROW_OFF];
        if (mons[i].female_animation.fire == animidx)
            return glyph2tile[i + GLYPH_FEMALE_FIRE_OFF];
        if (mons[i].female_animation.cast == animidx)
            return glyph2tile[i + GLYPH_FEMALE_CAST_OFF];
        if (mons[i].female_animation.special_attack == animidx)
            return glyph2tile[i + GLYPH_FEMALE_SPECIAL_ATTACK_OFF];
        if (mons[i].female_animation.kick == animidx)
            return glyph2tile[i + GLYPH_FEMALE_KICK_OFF];
        if (mons[i].female_animation.item_use == animidx)
            return glyph2tile[i + GLYPH_FEMALE_ITEM_USE_OFF];
        if (mons[i].female_animation.door_use == animidx)
            return glyph2tile[i + GLYPH_FEMALE_DOOR_USE_OFF];
        if (mons[i].female_animation.death == animidx)
            return glyph2tile[i + GLYPH_FEMALE_DEATH_OFF];
        if (mons[i].female_animation.corpse == animidx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
        if (mons[i].female_animation.statue == animidx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
    }

    for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if(obj_descr[objects[i].oc_descr_idx].stand_animation == animidx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
        if (obj_descr[objects[i].oc_descr_idx].lit_animation == animidx)
            return glyph2tile[i + GLYPH_OBJ_LIT_OFF];
    }

    for (int cmap_idx = 0; cmap_idx < CMAP_TYPE_MAX; cmap_idx++)
    {
        for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].stand_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
        }

        for (int i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].stand_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
        }
    }

    for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (int gender = 0; gender <= 1; gender++)
            {
                for (int alignment = -1; alignment <= 1; alignment++)
                {
                    for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment, level);
                        if(get_player_stand_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_OFF];
                        if (get_player_attack_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_ATTACK_OFF];
                        if (get_player_throw_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_THROW_OFF];
                        if (get_player_fire_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_FIRE_OFF];
                        if (get_player_cast_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_CAST_OFF];
                        if (get_player_special_attack_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_SPECIAL_ATTACK_OFF];
                        if (get_player_kick_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_KICK_OFF];
                        if (get_player_item_use_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_ITEM_USE_OFF];
                        if (get_player_door_use_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_DOOR_USE_OFF];
                        if (get_player_death_animation(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_DEATH_OFF];
                    }
                }
            }
        }
    }

    return -1;
}


short
get_enlargement_base_tile(enlidx)
short enlidx;
{
    for (int i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        if (mons[i].enlargement.stand == enlidx)
            return glyph2tile[i + GLYPH_MON_OFF];
        if (mons[i].enlargement.attacks == enlidx)
            return glyph2tile[i + GLYPH_ATTACK_OFF];
        if (mons[i].enlargement.throws == enlidx)
            return glyph2tile[i + GLYPH_THROW_OFF];
        if (mons[i].enlargement.fire == enlidx)
            return glyph2tile[i + GLYPH_FIRE_OFF];
        if (mons[i].enlargement.cast == enlidx)
            return glyph2tile[i + GLYPH_CAST_OFF];
        if (mons[i].enlargement.special_attack == enlidx)
            return glyph2tile[i + GLYPH_SPECIAL_ATTACK_OFF];
        if (mons[i].enlargement.kick == enlidx)
            return glyph2tile[i + GLYPH_KICK_OFF];
        if (mons[i].enlargement.item_use == enlidx)
            return glyph2tile[i + GLYPH_ITEM_USE_OFF];
        if (mons[i].enlargement.door_use == enlidx)
            return glyph2tile[i + GLYPH_DOOR_USE_OFF];
        if (mons[i].enlargement.death == enlidx)
            return glyph2tile[i + GLYPH_DEATH_OFF];
        if (mons[i].enlargement.corpse == enlidx)
            return glyph2tile[i + GLYPH_BODY_OFF];
        if (mons[i].enlargement.statue == enlidx)
            return glyph2tile[i + GLYPH_STATUE_OFF];

        if (mons[i].female_enlargement.stand == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_MON_OFF];
        if (mons[i].female_enlargement.attacks == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_MON_OFF];
        if (mons[i].female_enlargement.throws == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_THROW_OFF];
        if (mons[i].female_enlargement.fire == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_FIRE_OFF];
        if (mons[i].female_enlargement.cast == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_CAST_OFF];
        if (mons[i].female_enlargement.special_attack == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_SPECIAL_ATTACK_OFF];
        if (mons[i].female_enlargement.kick == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_KICK_OFF];
        if (mons[i].female_enlargement.item_use == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_ITEM_USE_OFF];
        if (mons[i].female_enlargement.door_use == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_DOOR_USE_OFF];
        if (mons[i].female_enlargement.death == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_DEATH_OFF];
        if (mons[i].female_enlargement.corpse == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
        if (mons[i].female_enlargement.statue == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
    }

    for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if (obj_descr[objects[i].oc_descr_idx].enlargement == enlidx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
        if (obj_descr[objects[i].oc_descr_idx].lit_enlargement == enlidx)
            return glyph2tile[i + GLYPH_OBJ_LIT_OFF];
    }

    for (int cmap_idx = 0; cmap_idx < CMAP_TYPE_MAX; cmap_idx++)
    {

        for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
        }

        for (int i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
        }
    }
    for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (int gender = 0; gender <= 1; gender++)
            {
                for (int alignment = -1; alignment <= 1; alignment++)
                {
                    for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment, level);
                        if (get_player_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_OFF];
                        if (get_player_attack_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_ATTACK_OFF];
                        if (get_player_throw_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_THROW_OFF];
                        if (get_player_fire_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_FIRE_OFF];
                        if (get_player_cast_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_CAST_OFF];
                        if (get_player_special_attack_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_SPECIAL_ATTACK_OFF];
                        if (get_player_kick_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_KICK_OFF];
                        if (get_player_item_use_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_ITEM_USE_OFF];
                        if (get_player_door_use_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_DOOR_USE_OFF];
                        if (get_player_death_enlargement(roleidx, raceidx, gender, alignment, level))
                            return glyph2tile[player_glyph_index + GLYPH_PLAYER_DEATH_OFF];
                    }
                }
            }
        }
    }

    return -1;
}

/* animation.c */



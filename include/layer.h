/*
 * Layers. Copyright (c) Janne Gustafsson, 2020
 *
 */

#ifndef LAYER_H
#define LAYER_H

struct layer_info {
    int glyph; /* For ascii compatibility */
    int bkglyph; /* For ascii compatibility */

    int floor_glyph;
    int dungeon_feature_glyph;
    int doodad_glyph;
    int object_glyph;
    int monster_glyph;
    int monster_effect_glyph;
    int missile_glyph;
    int cover_glyph;
    int environment_glyph;
    int general_effect_glyph;
    int ui_glyph; /* death, hit */

    unsigned long layer_flags;

    /* Monster info for display */
    int current_hp;
    int max_hp;
    int current_mana;
    int max_mana;
    unsigned long condition_flags;

    int damage_displayed;

};

#define LFLAGS_M_PET                0x00000001UL
#define LFLAGS_M_PEACEFUL           0x00000002UL
#define LFLAGS_M_DETECTED           0x00000004UL
#define LFLAGS_M_RIDDEN             0x00000008UL
#define LFLAGS_M_SADDLED            0x00000010UL
#define LFLAGS_M_RECEIVING_DAMAGE   0x00000020UL

#define LFLAGS_M_YOU                0x00000080UL

#define LFLAGS_O_DRAWN_IN_FRONT     0x00000100UL

#define LFLAGS_UNEXPLORED           0x00001000UL



#endif /* LAYER_H */
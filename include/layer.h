/*
 * Layers. Copyright (c) Janne Gustafsson, 2020
 *
 */

#ifndef LAYER_H
#define LAYER_H

enum layer_types
{
    LAYER_FLOOR = 0,
    LAYER_FEATURE,
    LAYER_DOODAD,
    LAYER_OBJECT,
    LAYER_MONSTER,
    LAYER_MONSTER_EFFECT,
    LAYER_MISSILE,
    LAYER_COVER,
    LAYER_ENVIRONMENT,
    LAYER_GENERAL_EFFECT,
    LAYER_GENERAL_UI,
    MAX_LAYERS
};


struct layer_info {
    int glyph; /* For ascii compatibility */
    int bkglyph; /* For ascii compatibility */

    int layer_glyphs[MAX_LAYERS];

    unsigned long layer_flags;

    /* Monster info for display */
    struct obj object_data;     /* Note not a pointer to avoid spurious pointers, contained pointers set to zero */
    struct monst monster_data;  /* Note not a pointer to avoid spurious pointers, contained pointers set to zero */
    struct edog pet_data;       /* Note not a pointer to avoid spurious pointers */

    int damage_displayed;

};

#define LFLAGS_M_PET                0x00000001UL
#define LFLAGS_M_PEACEFUL           0x00000002UL
#define LFLAGS_M_DETECTED           0x00000004UL
#define LFLAGS_M_RIDDEN             0x00000008UL
#define LFLAGS_M_SADDLED            0x00000010UL
#define LFLAGS_M_YOU                0x00000020UL
#define LFLAGS_M_DATA_SET           0x00000040UL
#define LFLAGS_M_PET_DATA_SET       0x00000080UL
#define LFLAGS_M_BEING_HIT          0x00000100UL
#define LFLAGS_M_HIT_TEXT_MASK      0x00000E00UL /* 3 bits indicating display of hit text 1-8 */
#define LFLAGS_M_HIT_TEXT_MASK_BIT_OFFSET 9 
#define LFLAGS_M_MASK               0x00000FFFUL

#define LFLAGS_O_DRAWN_IN_FRONT     0x00001000UL
#define LFLAGS_O_PILE               0x00002000UL
#define LFLAGS_O_DATA_SET           0x00004000UL
#define LFLAGS_O_MASK               0x000FF000UL

#define LFLAGS_DISINTEGRATED        0x00100000UL
#define LFLAGS_PETRIFIED            0x00200000UL
#define LFLAGS_CAITIFF              0x00400000UL
#define LFLAGS_BACKSTAB             0x00800000UL
#define LFLAGS_BREAK_WEAPON         0x01000000UL
#define LFLAGS_SMASH                0x02000000UL
#define LFLAGS_STUNNED_FROM_BLOW    0x04000000UL
#define LFLAGS_EXPLODE              0x08000000UL

#define LFLAGS_UNEXPLORED           0x10000000UL
#define LFLAGS_CMAP_MASK            0xF0000000UL


#define CMAP_FLAG_FEATURE_LAYER 0x00
#define CMAP_FLAG_FLOOR_LAYER 0x01



struct replacement_info {
    struct obj* object;
};


struct extended_menu_info {
    struct obj* object;
    int color;
    unsigned long menu_flags;
};


#endif /* LAYER_H */
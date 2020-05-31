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
    LAYER_TRAP,
    LAYER_OBJECT,
    LAYER_MONSTER,
    LAYER_MISSILE,
    LAYER_MONSTER_EFFECT,
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
    genericptr_t monster_comp_ptr;  /* invalidate the pointer use for any other purposes than comparing the memory address */

    int damage_displayed;

};

/* These are standard flags replacing earlier glyph system -- To be used mostly in map_glyph for tty */
#define LFLAGS_M_PET                0x00000001UL
#define LFLAGS_M_PEACEFUL           0x00000002UL
#define LFLAGS_M_DETECTED           0x00000004UL
#define LFLAGS_M_RIDDEN             0x00000008UL
#define LFLAGS_M_SADDLED            0x00000010UL
#define LFLAGS_M_YOU                0x00000020UL

/* These are flags for showing extra stuff in graphical interfaces */
#define LFLAGS_M_BEING_HIT          0x00000100UL
#define LFLAGS_M_HIT_TILE_MASK      0x00000E00UL /* 3 bits indicating display of hit tile 0-7 */
#define LFLAGS_M_HIT_TILE_MASK_BIT_OFFSET 9 
#define LFLAGS_M_MASK               0x0000FFFFUL

/* These are flags for showing extra stuff in graphical interfaces */
#define LFLAGS_O_PILE               0x00010000UL
#define LFLAGS_O_MASK               0x000F0000UL

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
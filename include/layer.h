/*
 * Layers. Copyright (c) Janne Gustafsson, 2020
 *
 */

#ifndef LAYER_H
#define LAYER_H

enum layer_types
{
    LAYER_FLOOR = 0,
    LAYER_FLOOR_DOODAD,   /* Doodads underneath features and trapds */
    LAYER_FEATURE,
    LAYER_TRAP,
    LAYER_FEATURE_DOODAD, /* Doodads above features and traps */
    LAYER_OBJECT,
    LAYER_MONSTER,
    LAYER_MISSILE,
    LAYER_MONSTER_EFFECT,
    LAYER_COVER_OBJECT,
    LAYER_COVER_FEATURE,
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

    struct obj* memory_objchn;     /* Pointer to the location's first memory object*/

    int damage_displayed;

};

/* These are standard flags replacing earlier glyph system -- To be used mostly in map_glyph for tty */
#define LFLAGS_M_PET                0x00000001UL
#define LFLAGS_M_PEACEFUL           0x00000002UL
#define LFLAGS_M_DETECTED           0x00000004UL
#define LFLAGS_M_RIDDEN             0x00000008UL
#define LFLAGS_M_SADDLED            0x00000010UL
#define LFLAGS_M_YOU                0x00000020UL
/* free for m_ flag */
/* free for m_ flag */

/* These are flags for showing extra stuff in graphical interfaces */
#define LFLAGS_M_BEING_HIT          0x00000100UL
#define LFLAGS_M_HIT_TILE_MASK      0x00000E00UL /* 3 bits indicating display of hit tile 0-7 */
#define LFLAGS_M_HIT_TILE_MASK_BIT_OFFSET 9 
#define LFLAGS_M_DISINTEGRATED      0x00001000UL
#define LFLAGS_M_PETRIFIED          0x00002000UL
#define LFLAGS_M_CAITIFF            0x00004000UL
#define LFLAGS_M_BACKSTAB           0x00008000UL
#define LFLAGS_M_BREAK_WEAPON       0x00010000UL
/* free for m_ flag */
/* free for m_ flag */
/* free for m_ flag */
#define LFLAGS_M_MASK               0x000FFFFFUL

/* These are flags for showing extra stuff in graphical interfaces */
#define LFLAGS_O_PILE               0x00100000UL
/* free for another obj flag */
#define LFLAGS_O_MASK               0x00300000UL

/* free for cmaps */
/* free for cmaps */
#define LFLAGS_CMAP_MASK            0x00C00000UL

/* Rest are important general flags */
#define LFLAGS_SHOWING_MEMORY       0x01000000UL /* also implies that you cannot see the location (and hence showing memory) */
/* reserved for waslit flag */
/* free bits */

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
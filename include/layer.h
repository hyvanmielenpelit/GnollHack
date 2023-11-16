/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-05-22 */

/*
 * Layers. Copyright (c) Janne Gustafsson, 2020
 *
 */
#include "tradstdc.h"
#include "config.h"

#ifndef LAYER_H
#define LAYER_H

enum layer_types
{
    LAYER_FLOOR = 0,
    LAYER_CARPET,
    LAYER_FLOOR_DOODAD,   /* Doodads underneath features and traps */
    LAYER_FEATURE,
    LAYER_TRAP,
    LAYER_FEATURE_DOODAD, /* Doodads above features and traps */
    LAYER_BACKGROUND_EFFECT,
    LAYER_CHAIN,
    LAYER_OBJECT,
    LAYER_MONSTER,
    LAYER_MISSILE,
    LAYER_COVER_TRAP,
    LAYER_COVER_OBJECT,
    LAYER_COVER_FEATURE,
    LAYER_LEASH,
    LAYER_ENVIRONMENT,
    LAYER_ZAP,
    LAYER_GENERAL_EFFECT,
    LAYER_MONSTER_EFFECT,
    LAYER_GENERAL_UI,
    MAX_LAYERS
};


#define MAXLEASHED 2
#define NUM_BUFF_BIT_ULONGS 8 // 1 + (MAX_PROPS - 1) / 32

struct layer_info {
    int glyph; /* For ascii compatibility */
    int bkglyph; /* For ascii compatibility */

    int layer_glyphs[MAX_LAYERS];
    int layer_gui_glyphs[MAX_LAYERS];

    unsigned long layer_flags;

    unsigned m_id;  /* check that the monster found at the square is the one that is supposed to be drawn by comparing their m_ids */
    unsigned o_id;  /* this is the o_id of the possibly moving boulder */

    struct obj* memory_objchn;     /* Pointer to the location's first memory object*/

    short damage_displayed;
    short hit_tile;

    schar special_feature_doodad_layer_height;
    schar special_monster_layer_height;
    schar monster_origin_x;
    schar monster_origin_y;
    int monster_hp;
    int monster_maxhp;
    int rider_glyph;
    int rider_gui_glyph;
    unsigned long status_bits;
    unsigned long condition_bits;
    unsigned long buff_bits[NUM_BUFF_BIT_ULONGS];
    xchar wsegdir;
    xchar reverse_prev_wsegdir;
    unsigned long monster_flags;

    short object_height;

    uchar missile_poisoned;
    uchar missile_material;
    short missile_special_quality;
    uchar missile_elemental_enchantment;
    uchar missile_exceptionality;
    uchar missile_mythic_prefix;
    uchar missile_mythic_suffix;
    uchar missile_eroded;
    uchar missile_eroded2;
    unsigned long missile_flags;
    short missile_height;
    xchar missile_origin_x;
    xchar missile_origin_y;

    xchar leash_mon_x[MAXLEASHED + 1]; /* the last coordinate is the other end of the leash, i.e., u.ux at the time */
    xchar leash_mon_y[MAXLEASHED + 1]; /* the last coordinate is the other end of the leash, i.e., u.uy at the time */
};

struct simple_layer_info {
    int glyph; /* For ascii compatibility */
    int bkglyph; /* For ascii compatibility */
    unsigned long layer_flags;
    unsigned m_id;  /* check that the monster found at the square is the one that is supposed to be drawn by comparing their m_ids */
    unsigned o_id;  /* this is the o_id of the possibly moving boulder */
};

/* Location */
#define LFLAGS_L_LEGAL                  0x00000001UL
#define LFLAGS_L_ILLEGAL                0x00000002UL
#define LFLAGS_L_ENGRAVING              0x00000004UL /* Location has an engraving (a flag improving efficiency of locating engravings) */

/* Objects */
/* Reserved */
#define LFLAGS_O_CHAIN                  0x00002000UL
#define LFLAGS_O_PILE                   0x00004000UL
#define LFLAGS_O_IN_PIT                 0x00008000UL
#define LFLAGS_O_MASK                   0x0000F000UL

/* Effects */
/* Reserved */
#define LFLAGS_E_GEN_FADE_OUT           0x00020000UL
#define LFLAGS_E_BKG_FADE_IN            0x00040000UL
#define LFLAGS_E_BKG_FADE_OUT           0x00080000UL

/* Rays */
#define LFLAGS_ZAP_LEADING_EDGE         0x00100000UL /* First (leading) tile in a zap */
#define LFLAGS_ZAP_TRAILING_EDGE        0x00200000UL /* Last (trailing) tile in a zap */
#define LFLAGS_ZAP_MASK                 (LFLAGS_ZAP_LEADING_EDGE | LFLAGS_ZAP_TRAILING_EDGE)

/* Traps */
#define LFLAGS_T_TRAPPED                0x00400000UL

/* Cmap */
#define LFLAGS_C_DECORATION             0x00800000UL
#define LFLAGS_C_CARPET                 0x01000000UL

/* Important general flags */
#define LFLAGS_SHOWING_MEMORY           0x02000000UL /* also implies that you cannot see the location (and hence showing memory) */
#define LFLAGS_SHOWING_DETECTION        0x04000000UL /* Do not darken */
#define LFLAGS_ASCENSION_RADIANCE       0x08000000UL /* Lighten up */
#define LFLAGS_CAN_SEE                  0x10000000UL /* cansee(x, y) is true (if not, then darken etc.) */
#define LFLAGS_UXUY                     0x20000000UL /* x == u.ux && y == u.uy is true */
#define LFLAGS_APPEARS_UNLIT            0x40000000UL
#define LFLAGS_NO_WALL_END_AUTODRAW     0x80000000UL /* NO_WALL_END_AUTODRAW(x, y) is true */

/* These are standard flags replacing earlier glyph system -- To be used mostly in map_glyph for tty */
#define LMFLAGS_PET                         0x00000001UL
#define LMFLAGS_PEACEFUL                    0x00000002UL
#define LMFLAGS_DETECTED                    0x00000004UL
#define LMFLAGS_RIDDEN                      0x00000008UL
#define LMFLAGS_CANSPOTMON                  0x00000010UL /* canspotmon is true */
#define LMFLAGS_YOU                         0x00000020UL
#define LMFLAGS_BEING_HIT                   0x00000040UL
#define LMFLAGS_KILLED                      0x00000080UL
#define LMFLAGS_U_TETHERED                  0x00000100UL /* You are tethered */
#define LMFLAGS_TETHERED                    0x00000200UL /* Monster is tethered */
#define LMFLAGS_DROPPING_PIERCER            0x00000400UL
#define LMFLAGS_WORM_TAIL                   0x00000800UL
#define LMFLAGS_WORM_SEEN                   0x00001000UL
#define LMFLAGS_WORM_HEAD                   0x00002000UL
#define LMFLAGS_WORM_TAILEND                0x00004000UL
#define LMFLAGS_INVISIBLE_TRANSPARENT       0x00008000UL
#define LMFLAGS_SEMI_TRANSPARENT            0x00010000UL
#define LMFLAGS_RADIAL_TRANSPARENCY         0x00020000UL
#define LMFLAGS_GLASS_TRANSPARENCY          0x00040000UL
#define LMFLAGS_CAN_SPOT_SELF               0x00080000UL
#define LMFLAGS_LONG_WORM_WITH_TAIL         0x00100000UL
#define LMFLAGS_LONG_WORM_TAIL              0x00200000UL
#define LMFLAGS_BOSS_MONSTER_FIGHT          0x00400000UL
#define LMFLAGS_FADES_UPON_DEATH            0x00800000UL
#define LMFLAGS_FLYING                      0x01000000UL
#define LMFLAGS_LEVITATING                  0x02000000UL
#define LMFLAGS_BLOBBY_ANIMATION            0x04000000UL
#define LMFLAGS_SWIM_ANIMATION              0x08000000UL
#define LMFLAGS_SHARK_ANIMATION             0x10000000UL
#define LMFLAGS_HUMAN_BREATHE_ANIMATION     0x20000000UL
#define LMFLAGS_ANIMAL_BREATHE_ANIMATION    0x40000000UL
#define LMFLAGS_STONED                      0x80000000UL

#define MISSILE_FLAGS_CORRODEABLE   0x00000001UL
#define MISSILE_FLAGS_ROTTABLE      0x00000002UL
#define MISSILE_FLAGS_FLAMMABLE     0x00000004UL
#define MISSILE_FLAGS_RUSTPRONE     0x00000008UL
#define MISSILE_FLAGS_ERODEPROOF    0x00000010UL
#define MISSILE_FLAGS_POISONABLE    0x00000020UL
#define MISSILE_FLAGS_TETHERED      0x00000040UL /* Missile is tethered */
#define MISSILE_FLAGS_LIT           0x00000080UL


struct monst_info {
    int glyph;
    int gui_glyph;

    char name[UTF8BUFSZ];
    unsigned m_id;

    int mhp;
    int mhpmax;

    unsigned long status_bits;
    unsigned long condition_bits;
    unsigned long buff_bits[NUM_BUFF_BIT_ULONGS];

    unsigned long monster_flags;
};

#endif /* LAYER_H */

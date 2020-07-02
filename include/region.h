/* GnollHack 4.0	region.h	$NHDT-Date: 1432512779 2015/05/25 00:12:59 $  $NHDT-Branch: master $:$NHDT-Revision: 1.13 $ */
/* Copyright (c) 1996 by Jean-Christophe Collet			  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef REGION_H
#define REGION_H

/* generic callback function */

typedef boolean FDECL((*callback_proc), (genericptr_t, genericptr_t));

/*
 * player_flags
 */
#define REG_HERO_INSIDE 0x01
#define REG_NOT_HEROS 0x02
#define hero_inside(r) ((r)->player_flags & REG_HERO_INSIDE)
#define heros_fault(r) (!((r)->player_flags & REG_NOT_HEROS))
#define set_hero_inside(r) ((r)->player_flags |= REG_HERO_INSIDE)
#define clear_hero_inside(r) ((r)->player_flags &= ~REG_HERO_INSIDE)
#define set_heros_fault(r) ((r)->player_flags &= ~REG_NOT_HEROS)
#define clear_heros_fault(r) ((r)->player_flags |= REG_NOT_HEROS)

enum region_types {
    REGION_GENERAL = 0, /* Normal region */
    REGION_POISON_GAS,
    REGION_SMOKE,
    REGION_CLOUD,
    REGION_FIRE,
    REGION_LIGHTNING,
    REGION_FROST,
    REGION_DEATH,
    REGION_ANNIHILATION,
    REGION_MAGICAL_DARKNESS,
    REGION_MAGICAL_SILENCE,
    REGION_FORCE_FIELD_TRANSPARENT,
    REGION_FORCE_FIELD_OPAQUE,
    MAX_REGION_TYPES
};

enum region_base_types {
    REGION_BASETYPE_GENERAL = 0, /* Normal region */
    REGION_BASETYPE_GAS,
    REGION_BASETYPE_ELEMENTAL_EFFECT,
    REGION_BASETYPE_MAGICAL_EFFECT,
    REGION_BASETYPE_FORCEFIELD,
    MAX_REGION_BASETYPES
};

struct region_type_definition {
    const char* name;
    enum region_base_types base_type;
    boolean causes_damage;
    int damage_type;
    boolean visible;
    int screen_symbol;
    int variation;
    boolean sensed_blind_at_location;
    boolean sensed_by_touching_around;
    boolean touch_causes_effect;
    boolean accessible;
    boolean causes_blindness;
    boolean causes_silence;
    boolean blocks_vision;
    xchar is_light_source;
    enum region_soundset_types soundset;
    unsigned long rt_flags;
};

extern struct region_type_definition region_type_definitions[MAX_REGION_TYPES];

/*
 * Note: if you change the size/type of any of the fields below,
 *       or add any/remove any fields, you must update the
 *       bwrite() calls in save_regions(), and the
 *       mread() calls in rest_regions() in src/region.c
 *       to reflect the changes.
 */

typedef struct nhregion {
    NhRect bounding_box;        /* Bounding box of the region */
    NhRect *rects;              /* Rectangles composing the region */
    short nrects;               /* Number of rectangles  */
    boolean attach_2_u;         /* Region attached to player ? */
    unsigned int attach_2_m;    /* Region attached to monster ? */
    /*struct obj *attach_2_o;*/ /* Region attached to object ? UNUSED YET */
    const char *enter_msg;      /* Message when entering */
    const char *leave_msg;      /* Message when leaving */
    long ttl;                   /* Time to live. -1 is forever */
    short expire_f;            /* Function to call when region's ttl expire */
    short can_enter_f;         /* Function to call to check whether the player
                                  can, or can not, enter the region */
    short enter_f;             /* Function to call when the player enters*/
    short can_leave_f;         /* Function to call to check whether the player
                                  can, or can not, leave the region */
    short leave_f;             /* Function to call when the player leaves */
    short inside_f;            /* Function to call every turn if player's
                                  inside */
    unsigned int player_flags; /* (see above) */
    unsigned int *monsters;    /* Monsters currently inside this region */
    short n_monst;             /* Number of monsters inside this region */
    short max_monst;           /* Maximum number of monsters that can be
                                  listed without having to grow the array */
#define MONST_INC 5

    /* Should probably do the same thing about objects */

    boolean visible; /* Is the region visible ? */
    int glyph;       /* Which glyph to use if visible */
    anything arg;    /* Optional user argument (Ex: strength of
                        force field, damage of a fire zone, ...*/
    enum region_types typ;  /* type of the region, normally indicating visible, audible, and game play effects */
    int dmg_dice;
    int dmg_diesize;
    int dmg_plus;
    unsigned long region_flags; /* No teleport region etc. */

    boolean lamplit;        /* region is attached to a light source, which temp-lights the region */
    boolean makingsound;    /* region is attached to a sound source, which may cause ambient sounds outside and inside the region */
} NhRegion;

#endif /* REGION_H */

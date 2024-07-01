/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    trap.h    $NHDT-Date: 1547255912 2019/01/12 01:18:32 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.17 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2016. */
/* GnollHack may be freely redistributed.  See license for details. */

/* note for 3.1.0 and later: no longer manipulated by 'makedefs' */

#ifndef TRAP_H
#define TRAP_H

enum lever_effect_types {
    LEVER_EFFECT_NONE = 0,
    LEVER_EFFECT_OPEN_DOOR,
    LEVER_EFFECT_LOCK_DOOR,
    LEVER_EFFECT_OPEN_LOCK_DOOR,
    LEVER_EFFECT_CREATE_CLOSED_DOOR,
    LEVER_EFFECT_CREATE_LOCATION_TYPE,
    LEVER_EFFECT_CREATE_UNCREATE_LOCATION_TYPE,
    LEVER_EFFECT_CREATE_TRAP,
    LEVER_EFFECT_CREATE_OBJECT,
    LEVER_EFFECT_CREATE_MONSTER,
    MAX_LEVER_EFFECT_TYPES
};


union vlaunchinfo {
    short v_launch_otyp; /* type of object to be triggered */
    coord v_launch2;     /* secondary launch point (for boulders) */
    uchar v_conjoined;   /* conjoined pit locations */
    short v_tnote;       /* boards: 12 notes        */
};

struct trap {
    struct trap *ntrap;
    xchar tx, ty;
    d_level dst; /* destination for portals */
    coord launch;
    uchar ttyp;
    uchar tsubtyp;
    enum lever_effect_types lever_effect;
    int64_t effect_param1;
    int64_t effect_param2;
    int64_t effect_param3;
    int64_t effect_param4;
    uint64_t effect_flags;
    uint64_t tflags;
    uint64_t activation_count;
    boolean tseen;
    boolean once;
    boolean madeby_u; /* So monsters may take offence when you trap
                              them.  Recognizing who made the trap isn't
                              completely unreasonable, everybody has
                              their own style.  This flag is also needed
                              when you untrap a monster.  It would be too
                              easy to make a monster peaceful if you could
                              set a trap for it and then untrap it. */
    boolean madeby_mon; /* To differentiate for originally generated traps, especially in Sokoban */
    union vlaunchinfo vl;
#define launch_otyp vl.v_launch_otyp
#define launch2 vl.v_launch2
#define conjoined vl.v_conjoined
#define tnote vl.v_tnote
};

#define TRAPFLAGS_NONE                          0x00000000UL
#define TRAPFLAGS_ACTIVATED                     0x00000001UL  /* Trap is in the activated state */
/* Three more bit for possible trap states */
#define TRAPFLAGS_STATE_MASK                    0x0000000FUL  /* Bit mask for trap states */
#define TRAPFLAGS_CONTINUOUSLY_SWITCHABLE       0x00000010UL  /* Can be switched many times by apply */
#define TRAPFLAGS_SWITCHABLE_BETWEEN_STATES     0x00000020UL  /* Switching changes the state of the trap */
#define TRAPFLAGS_LEVEL_TELEPORTER              0x00000040UL  /* Magic portal will teleport between levels instead of branches; modron portal instead of around the level*/
#define TRAPFLAGS_LEVEL_TELEPORT_DOWN           0x00000080UL  /* Level teleports down instead of defining a destination */
#define TRAPFLAGS_LEVEL_TELEPORT_UP             0x00000100UL  /* Level teleports up instead of defining a destination */
#define TRAPFLAGS_LEVEL_TELEPORT_NO_OTHER_END   0x00000200UL  /* Level teleport location at destination is random instead of a portal at the other end */

extern struct trap *ftrap;
#define newtrap() (struct trap *) alloc(sizeof(struct trap))
#define dealloc_trap(trap) free((genericptr_t)(trap))

/* reasons for statue animation */
#define ANIMATE_NORMAL 0
#define ANIMATE_SHATTER 1
#define ANIMATE_SPELL 2

/* reasons for animate_statue's failure */
#define AS_OK 0            /* didn't fail */
#define AS_NO_MON 1        /* makemon failed */
#define AS_MON_IS_UNIQUE 2 /* statue monster is unique */

/* Note: if adding/removing a trap, adjust trap_engravings[] in mklev.c */

/* unconditional traps */
enum trap_types {
    NO_TRAP      =  0,
    ARROW_TRAP   =  1,
    DART_TRAP    =  2,
    ROCKTRAP     =  3,
    SQKY_BOARD   =  4,
    BEAR_TRAP    =  5,
    LANDMINE     =  6,
    ROLLING_BOULDER_TRAP = 7,
    SLP_GAS_TRAP =  8,
    RUST_TRAP    =  9,
    FIRE_TRAP    = 10,
    PIT          = 11,
    SPIKED_PIT   = 12,
    HOLE         = 13,
    TRAPDOOR     = 14,
    TELEP_TRAP   = 15,
    LEVEL_TELEP  = 16,
    MAGIC_PORTAL = 17,
    WEB          = 18,
    STATUE_TRAP  = 19,
    MAGIC_TRAP   = 20,
    ANTI_MAGIC_TRAP = 21,
    POLY_TRAP    = 22,
    MODRON_PORTAL = 23,
    LEVER = 24,
    VIBRATING_SQUARE = 25,

    TRAPNUM      = 26
};

enum trap_difficulty_types
{
    SIMPLE_MECHANICAL_TRAP = 0,
    COMPLEX_MECHANICAL_TRAP,
    SIMPLE_MAGICAL_TRAP,
    COMPLEX_MAGICAL_TRAP,
    SPECIALLY_DISARMABLE_TRAP,
    NOT_DISARMABLE_TRAP,
};

struct trap_type_definition {
    const char* name;
    const char* type_name;
    const char* apply_verb;
    enum obj_material_types material;
    enum trap_difficulty_types tdifficulty;
    enum trap_difficulty_types thelpdifficulty;
    int training;
    uint64_t tdflags;
};

#define TRAPDEF_FLAGS_NONE                      0x00000000UL
#define TRAPDEF_FLAGS_VISIBLE_AT_START          0x00000001UL
#define TRAPDEF_FLAGS_NOT_GENERATED             0x00000002UL
#define TRAPDEF_FLAGS_NOT_OVERRIDEN             0x00000004UL
#define TRAPDEF_FLAGS_NOT_DISARMABLE            0x00000008UL
#define TRAPDEF_FLAGS_NO_STEP_CONFIRMATION      0x00000010UL
#define TRAPDEF_FLAGS_IGNORED_BY_MONSTERS       0x00000020UL
#define TRAPDEF_FLAGS_TELEOK                    0x00000040UL
#define TRAPDEF_FLAGS_NO_TRY_ESCAPE             0x00000080UL
#define TRAPDEF_FLAGS_APPLIABLE                 0x00000100UL


extern const struct trap_type_definition trap_type_definitions[TRAPNUM];

#define is_pit(ttyp) ((ttyp) == PIT || (ttyp) == SPIKED_PIT)
#define is_hole(ttyp)  ((ttyp) == HOLE || (ttyp) == TRAPDOOR)
#define is_lever(ttyp)  ((ttyp) == LEVER)
#define is_trap_avoidable_by_flying(ttyp) ((is_hole(ttyp) && !Sokoban) || is_pit(ttyp) || is_lever(ttyp) || ttyp == BEAR_TRAP || ttyp == SQKY_BOARD || ttyp == ROLLING_BOULDER_TRAP || ttyp == VIBRATING_SQUARE)

#define MKTRAP_NO_FLAGS                    0x00000000UL
#define MKTRAPFLAG_GARDEN_GNOME_ITEMS      0x00000001UL
#define MKTRAPFLAG_MADE_BY_U               0x00000002UL
#define MKTRAPFLAG_MADE_BY_MON             0x00000004UL


#endif /* TRAP_H */

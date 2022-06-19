/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    lev.h    $NHDT-Date: 1432512781 2015/05/25 00:13:01 $  $NHDT-Branch: master $:$NHDT-Revision: 1.12 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

/*    Common include file for save and restore routines */

#ifndef LEV_H
#define LEV_H

#include "soundset.h"
#include "align.h"
#include "wintype.h"

#define COUNT_SAVE 0x1
#define WRITE_SAVE 0x2
#define FREE_SAVE 0x4

#define MAX_BMASK 4

/* operations of the various saveXXXchn & co. routines */
#define perform_bwrite(mode) ((mode) & (COUNT_SAVE | WRITE_SAVE))
#define release_data(mode) ((mode) &FREE_SAVE)

/* The following are used in mkmaze.c */
struct container {
    struct container *next;
    xchar x, y;
    short what;
    genericptr_t list;
};

enum bubble_contains_types {
    CONS_OBJ = 0,
    CONS_MON,
    CONS_HERO,
    CONS_TRAP
};

struct bubble {
    xchar x, y;   /* coordinates of the upper left corner */
    schar dx, dy; /* the general direction of the bubble's movement */
    uchar bm[MAX_BMASK + 2];    /* bubble bit mask */
    struct bubble *prev, *next; /* need to traverse the list up and down */
    struct container *cons;
};

/* used in light.c */
typedef struct lightsource_t {
    struct lightsource_t*next;
    xchar x, y;  /* source's position */
    short range; /* source's current range */
    short flags;
    short type;  /* type of light source */
    anything id; /* source's identifier */
} light_source;

/* used in soundset.c */
typedef struct soundsource_t {
    struct soundsource_t* next;
    xchar x, y;  /* source's position */
    enum ghsound_types ghsound; /* played ghsound */
    float heard_volume; /* source's volume as heard by the player, range 0.0f to 1.0f */
    float source_volume; /* source's volume at origin, range 0.0f to 1.0f */
    short flags;
    enum soundsource_types type;  /* type of sound source, monster, object, location */
    enum soundsource_ambient_subtypes subtype; /* subtype of soundsource, produced always, or while lit */
    anything id; /* source's identifier */
    void* ambient_ghsound_ptr; /* ambient sound pointer for storing */
} sound_source;


/* Used in save and restore */
struct save_game_stats {
    int glyph;
    int gui_glyph;
    short rolenum;
    short racenum;
    boolean gender;
    aligntyp alignment;
    short ulevel;
    char dgn_name[MAX_DGN_NAME_LENGTH];
    char level_name[MAX_LVL_NAME_LENGTH];
    xchar dnum;
    xchar dlevel;
    schar depth;
    schar game_difficulty;
    long umoves;
    boolean debug_mode;
    boolean explore_mode;
    boolean modern_mode;
    boolean casual_mode;
    time_t time_stamp;
};

struct save_game_data {
    char* playername;
    boolean is_running;
    struct save_game_stats gamestats;
};


#endif /* LEV_H */

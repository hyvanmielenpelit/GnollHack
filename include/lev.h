/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

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
    short sidedness;
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
    int64_t umoves;
    boolean debug_mode;
    boolean explore_mode;
    boolean modern_mode;
    boolean casual_mode;
    uchar save_flags;
    int64_t time_stamp;

    /* Other information */
    uint64_t num_recoveries;
};

#define SAVEFLAGS_NONE              0x00
#define SAVEFLAGS_NON_SCORING       0x01
#define SAVEFLAGS_TOURNAMENT_MODE   0x02
#define SAVEFLAGS_FILETRACK_SUPPORT 0x04
#define SAVEFLAGS_FILETRACK_ON      0x08

struct save_game_data {
    char* playername;
    char filename[256];
    boolean is_running;
    boolean is_error_save_file;
    boolean is_imported_save_file;
    struct save_game_stats gamestats;
};

/* Special view */
enum special_view_types
{
    SPECIAL_VIEW_NONE = 0,
    SPECIAL_VIEW_CHAT_MESSAGE,
    SPECIAL_VIEW_SHOW_SPECIAL_EFFECT,
    SPECIAL_VIEW_GUI_TIPS,
    SPECIAL_VIEW_CRASH_DETECTED,
    SPECIAL_VIEW_PANIC,
    SPECIAL_VIEW_DEBUGLOG,
    SPECIAL_VIEW_MESSAGE,
    SPECIAL_VIEW_SELFIE,
    SPECIAL_VIEW_HELP_DIR,
    SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N,
    SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y,
    SPECIAL_VIEW_SAVE_FILE_TRACKING_SAVE,
    SPECIAL_VIEW_SAVE_FILE_TRACKING_LOAD,
    MAX_SPECIAL_VIEW_TYPES
};

struct special_view_info {
    enum special_view_types viewtype;
    const char* text;
    const char* title;
    int attr;
    int color;
    int64_t time_stamp;
};

#endif /* LEV_H */

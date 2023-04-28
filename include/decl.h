/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0  decl.h  $NHDT-Date: 1547025154 2019/01/09 09:12:34 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.147 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2007. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef DECL_H
#define DECL_H

#include "general.h"
#include "soundset.h"

#define E extern

E int NDECL((*occupation));
E int NDECL((*afternmv));

E const char *hname;
E int hackpid;
#if defined(UNIX) || defined(VMS)
E int locknum;
#endif
#ifdef DEF_PAGER
E char *catmore;
#endif /* DEF_PAGER */

E char SAVEF[];
#ifdef MICRO
E char SAVEP[];
#endif

/* max size of a windowtype option */
#define WINTYPELEN 16
E char chosen_windowtype[WINTYPELEN];

E NEARDATA int bases[MAX_OBJECT_CLASSES];

E NEARDATA int multi;
E const char *multi_reason;
E NEARDATA int nroom;
E NEARDATA int nsubroom;
E NEARDATA int occtime;

E nhsym warnsyms[WARNCOUNT];
//E NEARDATA int warn_obj_cnt; /* count of monsters meeting criteria */

E int x_maze_max, y_maze_max;
E int otg_temp;

E NEARDATA int in_doagain;
E NEARDATA int skip_savech;

E struct dgn_topology { /* special dungeon levels for speed */
    d_level d_oracle_level;
    d_level d_bigroom_level; /* unused */
    d_level d_treasure_island;
    d_level d_rogue_level;
    d_level d_medusa_level;
    d_level d_minotaur_level;
    d_level d_stronghold_level;
    d_level d_valley_level;
    d_level d_wiz1_level;
    d_level d_wiz2_level;
    d_level d_wiz3_level;
    d_level d_jubilex_level;
    d_level d_orcus_level;
    d_level d_baalzebub_level; /* unused */
    d_level d_dispater_level;  /* unused */
    d_level d_yeenaghu_level;  /* unused */
    d_level d_tarrasque_level; /* unused */
    d_level d_portal_level;    /* only in goto_level() [do.c] */
    d_level d_sanctum_level;
    d_level d_earth_level;
    d_level d_water_level;
    d_level d_fire_level;
    d_level d_air_level;
    d_level d_astral_level;
    xchar d_main_dungeon_dnum, d_gehennom_dnum, d_endgame_dnum;
    xchar d_tower_dnum;
    xchar d_sokoban_dnum;
    xchar d_mines_dnum, d_quest_dnum;
    d_level d_qstart_level, d_qlocate_level, d_nemesis_level;
    d_level d_knox_level;
    d_level d_quinton_modron_level;
    d_level d_quarton_modron_level;
    d_level d_tertian_modron_level;
    d_level d_secundus_modron_level;
    d_level d_primus_modron_level;
    xchar d_modron_dnum;
    d_level d_bovine_level;
    xchar d_bovine_dnum;
    d_level d_quantum_level;
    xchar d_large_circular_dgn_dnum;
    d_level d_minetown_level;
    d_level d_mineend_level;
    d_level d_sokoend_level;
} dungeon_topology;
/* macros for accessing the dungeon levels by their old names */
/* clang-format off */
#define oracle_level            (dungeon_topology.d_oracle_level)
#define bigroom_level           (dungeon_topology.d_bigroom_level)
#define rogue_level             (dungeon_topology.d_rogue_level)
#define medusa_level            (dungeon_topology.d_medusa_level)
#define minotaur_level          (dungeon_topology.d_minotaur_level)
#define stronghold_level        (dungeon_topology.d_stronghold_level)
#define valley_level            (dungeon_topology.d_valley_level)
#define wiz1_level              (dungeon_topology.d_wiz1_level)
#define wiz2_level              (dungeon_topology.d_wiz2_level)
#define wiz3_level              (dungeon_topology.d_wiz3_level)
#define jubilex_level           (dungeon_topology.d_jubilex_level)
#define orcus_level             (dungeon_topology.d_orcus_level)
#define baalzebub_level         (dungeon_topology.d_baalzebub_level)
#define dispater_level          (dungeon_topology.d_dispater_level)
#define yeenaghu_level          (dungeon_topology.d_yeenaghu_level)
#define tarrasque_level         (dungeon_topology.d_tarrasque_level)
#define portal_level            (dungeon_topology.d_portal_level)
#define sanctum_level           (dungeon_topology.d_sanctum_level)
#define earth_level             (dungeon_topology.d_earth_level)
#define water_level             (dungeon_topology.d_water_level)
#define fire_level              (dungeon_topology.d_fire_level)
#define air_level               (dungeon_topology.d_air_level)
#define astral_level            (dungeon_topology.d_astral_level)
#define main_dungeon_dnum       (dungeon_topology.d_main_dungeon_dnum)
#define gehennom_dnum           (dungeon_topology.d_gehennom_dnum)
#define quest_dnum              (dungeon_topology.d_quest_dnum)
#define tower_dnum              (dungeon_topology.d_tower_dnum)
#define sokoban_dnum            (dungeon_topology.d_sokoban_dnum)
#define mines_dnum              (dungeon_topology.d_mines_dnum)
#define endgame_dnum            (dungeon_topology.d_endgame_dnum)
#define quest_dnum              (dungeon_topology.d_quest_dnum)
#define qstart_level            (dungeon_topology.d_qstart_level)
#define qlocate_level           (dungeon_topology.d_qlocate_level)
#define nemesis_level           (dungeon_topology.d_nemesis_level)
#define knox_level              (dungeon_topology.d_knox_level)
#define quinton_modron_level    (dungeon_topology.d_quinton_modron_level)
#define quarton_modron_level    (dungeon_topology.d_quarton_modron_level)
#define tertian_modron_level    (dungeon_topology.d_tertian_modron_level)
#define secundus_modron_level   (dungeon_topology.d_secundus_modron_level)
#define primus_modron_level     (dungeon_topology.d_primus_modron_level)
#define modron_dnum             (dungeon_topology.d_modron_dnum)
#define bovine_level            (dungeon_topology.d_bovine_level)
#define bovine_dnum             (dungeon_topology.d_bovine_dnum)
#define quantum_level           (dungeon_topology.d_quantum_level)
#define treasure_island_level   (dungeon_topology.d_treasure_island)
#define large_circular_dgn_dnum (dungeon_topology.d_large_circular_dgn_dnum)
#define minetown_level          (dungeon_topology.d_minetown_level)
#define mineend_level           (dungeon_topology.d_mineend_level)
#define sokoend_level           (dungeon_topology.d_sokoend_level)
/* clang-format on */

E NEARDATA stairway dnstair, upstair; /* stairs up and down */
#define xdnstair (dnstair.sx)
#define ydnstair (dnstair.sy)
#define xupstair (upstair.sx)
#define yupstair (upstair.sy)

E NEARDATA stairway dnladder, upladder; /* ladders up and down */
#define xdnladder (dnladder.sx)
#define ydnladder (dnladder.sy)
#define xupladder (upladder.sx)
#define yupladder (upladder.sy)

E NEARDATA stairway sstairs;

E NEARDATA dest_area updest, dndest; /* level-change destination areas */
E NEARDATA dest_area noteledest; /* no teleport area in the level */

E NEARDATA coord inv_pos;
E NEARDATA dungeon dungeons[];
E NEARDATA s_level *sp_levchn;
#define dunlev_reached(x) (dungeons[(x)->dnum].dunlev_ureached)

#include "quest.h"
E struct q_score quest_status;

E NEARDATA char pl_character[PL_CSIZ];
E NEARDATA char pl_race; /* character's race */

E NEARDATA char pl_fruit[PL_FSIZ];
E NEARDATA struct fruit *ffruit;

E NEARDATA char tune[6];

#define MAXLINFO (MAXDUNGEON * MAXLEVEL)
E struct linfo level_info[MAXLINFO];

E NEARDATA struct sinfo {
    int gameover;  /* self explanatory? */
    int stopprint; /* inhibit further end of game disclosure */
#ifdef HANGUPHANDLING
    volatile int done_hup; /* SIGHUP or moral equivalent received
                            * -- no more screen output */
    int preserve_locks;    /* don't remove level files prior to exit */
#endif
    boolean animation_hangup;  /* animation may be hanging up the game; skip drawing animations */
    int something_worth_saving; /* in case of panic */
    int panicking;              /* `panic' is in progress */
    int exiting;                /* an exit handler is executing */
    int exit_status;            /* status for exit */
    int freeing_dynamic_data;   /* freeing dynamic data */
    int in_bones;               /* working to create a bones file */
    int in_moveloop;
    int in_impossible;
#ifdef PANICLOG
    int in_paniclog;
#endif
    int wizkit_wishing;
} program_state;

E boolean restoring;
E boolean saving;
E boolean reseting;
E boolean ransacked;

E const char quitchars[];
E const char vowels[];
E const char ynchars[];
E const char ynqchars[];
E const char ynaqchars[];
E const char ynNaqchars[];
E const char dnqchars[];
E const char sdqchars[];

E const char yndescs[];
E const char ynqdescs[];
E const char ynq2descs[];
E const char ynaqdescs[];
E const char ynaq2descs[];
E const char ynNaqdescs[];
E const char ciqdescs[];
E const char bcqdescs[];
E const char fnqdescs[];
E const char dnqdescs[];
E const char sdqdescs[];

E NEARDATA long yn_number;

E const char disclosure_options[];

E NEARDATA int smeq[MAXNROFROOMS + 1];
E NEARDATA int doorindex;
E NEARDATA char *save_cm;

E NEARDATA struct kinfo {
    struct kinfo *next; /* chain of delayed killers */
    int id;             /* uprop keys to ID a delayed killer */
    int format;         /* one of the killer formats */
#define KILLED_BY_AN 0
#define KILLED_BY 1
#define NO_KILLER_PREFIX 2
    int hint_idx;
    char name[BUFSZ]; /* actual killer name */
} killer;

E NEARDATA struct obj* getobj_autoselect_obj;
E NEARDATA const char getobj_comestibles[];
E NEARDATA const char getobj_offerfodder[];
E NEARDATA const char getobj_all_count[];
E NEARDATA const char getobj_allobj[];
E NEARDATA const char getobj_styluses[];
E NEARDATA const char getobj_drop_types[];
E NEARDATA const char getobj_callable[];
E NEARDATA const char getobj_allowall[];
E NEARDATA const char getobj_recharge_type[];
E NEARDATA const char getobj_invoke_types[];
E NEARDATA const char getobj_clothes[];
E NEARDATA const char getobj_accessories[];
E NEARDATA const char getobj_beverages[];
E NEARDATA const char getobj_wield_objs[];
E NEARDATA const char getobj_unwield_objs[];
E NEARDATA const char getobj_stash_objs[];
E NEARDATA const char getobj_ready_objs[];
E NEARDATA const char getobj_bullets[];
E NEARDATA const char getobj_readable[];
E NEARDATA const char getobj_cuddly[];
E NEARDATA const char getobj_toss_objs[];
E NEARDATA const char getobj_tippables[];
E NEARDATA const char getobj_zap_syms[];
E NEARDATA const char getobj_mark_autostashs[];
E NEARDATA const char getobj_unmark_autostashs[];

E long done_money;
E NEARDATA char plname[PL_NSIZ];
E NEARDATA char recovery_plname[PL_NSIZ];
E NEARDATA boolean plname_from_error_savefile;
E NEARDATA boolean plname_from_imported_savefile;
E NEARDATA char dogname[];
E NEARDATA char catname[];
E NEARDATA char horsename[];
E NEARDATA char ramname[];
E NEARDATA char luggagename[];
E NEARDATA char wolfname[];
E short doggender;
E short catgender;
E short horsegender;
E short ramgender;
/* No luggagegender, because luggages are neuter */
E short wolfgender;

E unsigned short dogbreed;
E unsigned short catbreed;

struct breed_definition {
    const char* name;
    const char* description;
    const char* breed_name;
    const char* color_name;
};

E const struct breed_definition dog_breed_definitions[NUM_DOG_BREEDS];
E const struct breed_definition cat_breed_definitions[NUM_CAT_BREEDS];

E char preferred_pet;

E NEARDATA unsigned long n_game_recoveries;

E const char *occtxt; /* defined when occupation != NULL */
E enum object_soundset_types occsoundset; /* defined when occupation != NULL */
E enum object_occupation_types occtyp; /* defined when occupation != NULL */

E const char *nomovemsg;
E char lock[];

E const schar xdir[], ydir[], zdir[];

E NEARDATA schar tbx, tby; /* set in mthrowu.c */

E NEARDATA struct multishot {
    int n, i;
    short o;
    boolean s;
} m_shot;

E NEARDATA long moves, monstermoves;
E NEARDATA long wailmsg;

E NEARDATA boolean in_mklev;
E NEARDATA boolean stoned;
E NEARDATA boolean unweapon1;
E NEARDATA boolean unweapon2;
E NEARDATA boolean mrg_to_wielded;
E NEARDATA boolean defer_see_monsters;

E NEARDATA boolean in_steed_dismounting;
E NEARDATA boolean has_strong_rngseed;

E const int shield_static[];

#include "spell.h"
E NEARDATA struct spell spl_book[MAXSPELL + 1]; /* sized in decl.c */

#include "color.h"

E const struct class_sym def_oc_syms[MAX_OBJECT_CLASSES]; /* default class symbols */
E uchar oc_syms[MAX_OBJECT_CLASSES];                      /* current class symbols */
E const struct class_sym def_monsyms[MAX_MONSTER_CLASSES]; /* default class symbols */
E uchar monsyms[MAX_MONSTER_CLASSES];                      /* current class symbols */

#include "obj.h"
E NEARDATA struct obj *invent, *uarm, *uarmc, *uarmh, *uarms, *uarmg, *uarmf,
    *uarmu, /* under-wear, so to speak */
    *uarmo, /* over-wear, so to speak */
    *uarmb, /* bracers */
    *umisc, *umisc2, *umisc3, *umisc4, *umisc5, /* miscellaneous */
    *uskin, *uamul, *uleft, *uright, *ublindf, *uwep, *uswapwep, *uswapwep2, *uquiver;

#define uwep2 uarms

struct zap_type_definition {
    const char* name;
    const int color;
    short animation;
};

E NEARDATA const struct zap_type_definition zap_type_definitions[MAX_ZAP_TYPES];


struct explosion_type_definition {
    const char* name;
    const int color;
    short animation;
    enum sfx_sound_types sfx;
};

E NEARDATA const struct explosion_type_definition explosion_type_definitions[MAX_EXPLOSIONS];

E NEARDATA struct obj *uchain; /* defined only when punished */
E NEARDATA struct obj *uball;
E NEARDATA struct obj *migrating_objs;
E NEARDATA struct obj *billobjs;
E NEARDATA struct obj *memoryobjs;
E NEARDATA struct obj *current_wand, *thrownobj, *kickedobj;

E NEARDATA const struct obj zeroobj; /* for init; also, &zeroobj is used
                                      * as special value */

E NEARDATA const anything zeroany;   /* init'd and defined in decl.c */

E NEARDATA const struct layer_info zerolayerinfo;
E NEARDATA const struct replacement_info zeroreplacementinfo;
E NEARDATA const struct extended_menu_info zeroextendedmenuinfo;
E NEARDATA const struct extended_create_window_info zerocreatewindowinfo;

E NEARDATA const struct d_level zerodlevel;

E NEARDATA int gnh_has_rogue_level;

#include "you.h"
E NEARDATA struct you u;
E NEARDATA time_t ubirthday;
E NEARDATA struct u_realtime urealtime;

#include "onames.h"
#ifndef PM_H /* (pm.h has already been included via youprop.h) */
#include "pm.h"
#endif

E NEARDATA const struct monst zeromonst; /* for init of new or temp monsters */
E NEARDATA struct monst youmonst; /* monster details when hero is poly'd */
E NEARDATA struct monst *mydogs, *migrating_mons;

E NEARDATA struct mvitals {
    uchar born;
    uchar died;
    uchar died_female;
    uchar mvflags;
} mvitals[NUM_MONSTERS];

E NEARDATA long domove_attempting;
E NEARDATA long domove_succeeded;
#define DOMOVE_WALK         0x00000001L
#define DOMOVE_RUSH         0x00000002L

E NEARDATA struct c_color_names {
    const char *const c_black, *const c_amber, *const c_golden,
        *const c_light_blue, *const c_red, *const c_green, *const c_silver,
        *const c_blue, *const c_purple, *const c_white, *const c_orange;
} c_color_names;
#define NH_BLACK c_color_names.c_black
#define NH_AMBER c_color_names.c_amber
#define NH_GOLDEN c_color_names.c_golden
#define NH_LIGHT_BLUE c_color_names.c_light_blue
#define NH_RED c_color_names.c_red
#define NH_GREEN c_color_names.c_green
#define NH_SILVER c_color_names.c_silver
#define NH_BLUE c_color_names.c_blue
#define NH_PURPLE c_color_names.c_purple
#define NH_WHITE c_color_names.c_white
#define NH_ORANGE c_color_names.c_orange

/* The names of the colors used for gems, etc. */
E const char *c_obj_colors[];

E struct c_common_strings {
    const char *const c_nothing_happens, *const c_thats_enough_tries,
        *const c_silly_thing_to, *const c_shudder_for_moment,
        *const c_something, *const c_Something, *const c_You_can_move_again,
        *const c_Never_mind, *c_vision_clears, *const c_the_your[2];
} c_common_strings;
#define nothing_happens c_common_strings.c_nothing_happens
#define thats_enough_tries c_common_strings.c_thats_enough_tries
#define silly_thing_to c_common_strings.c_silly_thing_to
#define shudder_for_moment c_common_strings.c_shudder_for_moment
#define something c_common_strings.c_something
#define Something c_common_strings.c_Something
#define You_can_move_again c_common_strings.c_You_can_move_again
#define Never_mind c_common_strings.c_Never_mind
#define vision_clears c_common_strings.c_vision_clears
#define the_your c_common_strings.c_the_your

/* Monster name articles */
#define ARTICLE_NONE 0
#define ARTICLE_THE 1
#define ARTICLE_A 2
#define ARTICLE_YOUR 3

/* Monster name suppress masks */
#define SUPPRESS_IT 0x01
#define SUPPRESS_INVISIBLE 0x02
#define SUPPRESS_HALLUCINATION 0x04
#define SUPPRESS_SADDLE 0x08
#define EXACT_NAME 0x0F
#define SUPPRESS_NAME 0x10

/* Vision */
E NEARDATA boolean vision_full_recalc; /* TRUE if need vision recalc */
E NEARDATA char **viz_array;           /* could see/in sight row pointers */

/* Sound */
E NEARDATA boolean hearing_full_recalc; /* TRUE if need hearing recalc */
E NEARDATA float hearing_array[COLNO][ROWNO]; 

/* Window system stuff */
E NEARDATA winid WIN_MESSAGE;
E NEARDATA winid WIN_STATUS;
E NEARDATA winid WIN_MAP, WIN_INVEN, WIN_HERE;

/* pline (et al) for a single string argument (suppress compiler warning) */
#define pline1(cstr) pline("%s", cstr)
#define pline_The1(cstr) pline_The("%s", cstr)
#define Your1(cstr) Your("%s", cstr)
#define You1(cstr) You("%s", cstr)
#define verbalize1(cstr) verbalize("%s", cstr)
#define You_hear1(cstr) You_hear("%s", cstr)
#define Sprintf1(buf, cstr) Sprintf(buf, "%s", cstr)
#define panic1(cstr) panic("%s", cstr)

#define pline_ex1(a, b, cstr) pline_ex(a, b, "%s", cstr)
#define Your_ex1(a, b, cstr) Your_ex(a, b, "%s", cstr)
#define You_ex1(a, b, cstr) You_ex(a, b, "%s", cstr)
#define You_hear_ex1(a, b, cstr) You_hear_ex(a, b, "%s", cstr)
#define You_cant_ex1(a, b, cstr) You_cant_ex(a, b, "%s", cstr)
#define verbalize_ex1(a, b, cstr) verbalize_ex(a, b, "%s", cstr)

#define pline_ex1_popup(a, b, cstr, title, dopop) pline_ex1(a, b, cstr); if (dopop) display_popup_text(cstr, title, POPUP_TEXT_GENERAL, a, b, NO_GLYPH, 0)

E char toplines[TBUFSZ];
E char toplineattrs[TBUFSZ];
E char toplinecolors[TBUFSZ];

#ifndef TCAP_H
E struct tc_gbl_data {   /* also declared in tcap.h */
    char *tc_AS, *tc_AE; /* graphics start and end (tty font swapping) */
    int tc_LI, tc_CO;    /* lines and columns */
} tc_gbl_data;
#define AS tc_gbl_data.tc_AS
#define AE tc_gbl_data.tc_AE
#define LI tc_gbl_data.tc_LI
#define CO tc_gbl_data.tc_CO
#endif

/* xxxexplain[] is in drawing.c */
E const char *const monexplain[], invisexplain[], *const oclass_names[];

/* Some systems want to use full pathnames for some subsets of file names,
 * rather than assuming that they're all in the current directory.  This
 * provides all the subclasses that seem reasonable, and sets up for all
 * prefixes being null.  Port code can set those that it wants.
 */
#define HACKPREFIX 0
#define LEVELPREFIX 1
#define SAVEPREFIX 2
#define BONESPREFIX 3
#define DATAPREFIX 4 /* this one must match hardcoded value in dlb.c */
#define SCOREPREFIX 5
#define LOCKPREFIX 6
#define SYSCONFPREFIX 7
#define CONFIGPREFIX 8
#define TROUBLEPREFIX 9
#define PREFIX_COUNT 10
/* used in files.c; xxconf.h can override if needed */
#ifndef FQN_MAX_FILENAME
#define FQN_MAX_FILENAME 512
#endif

#if defined(NOCWD_ASSUMPTIONS) || defined(VAR_PLAYGROUND)
/* the bare-bones stuff is unconditional above to simplify coding; for
 * ports that actually use prefixes, add some more localized things
 */
#define PREFIXES_IN_USE
#endif

E char *fqn_prefix[PREFIX_COUNT];
#ifdef PREFIXES_IN_USE
E const char *fqn_prefix_names[PREFIX_COUNT];
#endif

E NEARDATA struct savefile_info sfcap, sfrestinfo, sfsaveinfo;

struct opvar {
    xchar spovartyp; /* one of SPOVAR_foo */
    union {
        char *str;
        long l;
    } vardata;
};

struct autopickup_exception {
    struct nhregex *regex;
    char *pattern;
    boolean grab;
    struct autopickup_exception *next;
};

struct plinemsg_type {
    xchar msgtype;  /* one of MSGTYP_foo */
    struct nhregex *regex;
    char *pattern;
    struct plinemsg_type *next;
};

#define MSGTYP_NORMAL   0
#define MSGTYP_NOREP    1
#define MSGTYP_NOSHOW   2
#define MSGTYP_STOP     3
/* bitmask for callers of hide_unhide_msgtypes() */
#define MSGTYP_MASK_REP_SHOW ((1 << MSGTYP_NOREP) | (1 << MSGTYP_NOSHOW))

E struct plinemsg_type *plinemsg_types;

#ifdef PANICTRACE
E const char *ARGV0;
#endif

enum earlyarg {ARG_DEBUG, ARG_VERSION
#ifdef WIN32
    ,ARG_WINDOWS
#endif
};

struct early_opt {
    enum earlyarg e;
    const char *name;
    int minlength;
    boolean valallowed;
};

NEARDATA E coord bhitpos; /* place where throw or zap hits or stops */
E boolean disallow_keyboard_commands_in_wait_loop;

E const char* Moloch;

/* From cmd.c so wizard mode commands can use these in other .c files */
/* for rejecting attempts to use wizard mode commands */
E const char unavailcmd[];
/* for rejecting #if !SHELL, !SUSPEND */
E const char cmdnotavail[];

E const char* const hofe_titles[3];

E unsigned long file_end_marker;

E char debug_buf_1[BUFSIZ];
E char debug_buf_2[BUFSIZ];
E char debug_buf_3[BUFSIZ];
E char debug_buf_4[BUFSIZ];

#undef E

#endif /* DECL_H */

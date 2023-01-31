/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-01-06 */

/* GnollHack 4.0    decl.c    $NHDT-Date: 1547025164 2019/01/09 09:12:44 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.141 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2009. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

int NDECL((*afternmv));
int NDECL((*occupation));

/* from xxxmain.c */
const char *hname = 0; /* name of the game (argv[0] of main) */
int hackpid = 0;       /* current process id */
#if defined(UNIX) || defined(VMS)
int locknum = 0; /* max num of simultaneous users */
#endif
#ifdef DEF_PAGER
char *catmore = 0; /* default pager */
#endif
char chosen_windowtype[WINTYPELEN];

NEARDATA int bases[MAX_OBJECT_CLASSES] = DUMMY;

NEARDATA int multi = 0;
const char *multi_reason = NULL;
NEARDATA int nroom = 0;
NEARDATA int nsubroom = 0;
NEARDATA int occtime = 0;

/* maze limits must be even; masking off lowest bit guarantees that */
int x_maze_max = (COLNO - 1) & ~1, y_maze_max = (ROWNO - 1) & ~1;

int otg_temp; /* used by object_to_glyph() [otg] */

NEARDATA int in_doagain = 0;
NEARDATA int skip_savech = 0;

/*
 *      The following structure will be initialized at startup time with
 *      the level numbers of some "important" things in the game.
 */
struct dgn_topology dungeon_topology = { DUMMY };

struct q_score quest_status = DUMMY;

//NEARDATA int warn_obj_cnt = 0;
NEARDATA int smeq[MAXNROFROOMS + 1] = DUMMY;
NEARDATA int doorindex = 0;
NEARDATA char *save_cm = 0;

NEARDATA struct kinfo killer = DUMMY;
NEARDATA long done_money = 0;
const char *nomovemsg = 0;
NEARDATA char plname[PL_NSIZ] = DUMMY; /* player name */
NEARDATA char recovery_plname[PL_NSIZ] = DUMMY;  /* Set at program start to check for recoverable games before ask_name */
NEARDATA boolean plname_from_error_savefile = FALSE;
NEARDATA boolean plname_from_imported_savefile = FALSE;
NEARDATA char pl_character[PL_CSIZ] = DUMMY;
NEARDATA char pl_race = '\0';

NEARDATA char pl_fruit[PL_FSIZ] = DUMMY;
NEARDATA struct fruit *ffruit = (struct fruit *) 0;

NEARDATA char tune[6] = DUMMY;
NEARDATA boolean ransacked = 0;

NEARDATA unsigned long n_game_recoveries = 0;

const char *occtxt = DUMMY;
enum object_soundset_types occsoundset = OBJECT_SOUNDSET_NONE; /* defined when occupation != NULL */
enum object_occupation_types occtyp = OCCUPATION_NONE; /* defined when occupation != NULL */

const char quitchars[] = " \r\n\033";
const char vowels[] = "aeiouAEIOU";
const char ynchars[] = "yn";
const char ynqchars[] = "ynq";
const char ynaqchars[] = "ynaq";
const char ynNaqchars[] = "yn#aq";
const char dnqchars[] = "dnq";
const char sdqchars[] = "sdq";

const char yndescs[] = "Yes\nNo";
const char ynqdescs[] = "Yes\nNo\nCancel";
const char ynq2descs[] = "Yes\nNo\nQuit";
const char ynaqdescs[] = "Yes\nNo\nAll\nCancel";
const char ynaq2descs[] = "Yes\nNo\nAll\nQuit";
const char ynNaqdescs[] = "Yes\nNo\nCount\nAll\nCancel";
const char dnqdescs[] = "Disarm\nNeglect\nCancel";
const char sdqdescs[] = "Stash\nDrop\nNothing";

NEARDATA long yn_number = 0L;

const char disclosure_options[] = "iavgco";

#if defined(MICRO) || defined(WIN32)
char hackdir[PATHLEN]; /* where rumors, help, record are */
#ifdef MICRO
char levels[PATHLEN]; /* where levels are */
#endif
#endif /* MICRO || WIN32 */

#ifdef MFLOPPY
char permbones[PATHLEN]; /* where permanent copy of bones go */
int ramdisk = FALSE;     /* whether to copy bones to levels or not */
int saveprompt = TRUE;
const char *alllevels = "levels.*";
const char *allbones = "bones*.*";
#endif

struct linfo level_info[MAXLINFO];

NEARDATA struct sinfo program_state;

/* x/y/z deltas for the 10 movement directions (8 compass pts, 2 up/down) */
const schar xdir[10] = { -1, -1, 0, 1, 1, 1, 0, -1, 0, 0 };
const schar ydir[10] = { 0, -1, -1, -1, 0, 1, 1, 1, 0, 0 };
const schar zdir[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, -1 };

NEARDATA schar tbx = 0, tby = 0; /* mthrowu: target */

/* for xname handling of multiple shot missile volleys:
   number of shots, index of current one, validity check, shoot vs throw */
NEARDATA struct multishot m_shot = { 0, 0, STRANGE_OBJECT, FALSE };

NEARDATA dungeon dungeons[MAXDUNGEON]; /* ini'ed by init_dungeon() */
NEARDATA s_level *sp_levchn;
NEARDATA stairway upstair = { 0, 0, { 0, 0 }, 0 },
                  dnstair = { 0, 0, { 0, 0 }, 0 };
NEARDATA stairway upladder = { 0, 0, { 0, 0 }, 0 },
                  dnladder = { 0, 0, { 0, 0 }, 0 };
NEARDATA stairway sstairs = { 0, 0, { 0, 0 }, 0 };
NEARDATA dest_area updest = { 0, 0, 0, 0, 0, 0, 0, 0 };
NEARDATA dest_area dndest = { 0, 0, 0, 0, 0, 0, 0, 0 };
NEARDATA dest_area noteledest = { 0, 0, 0, 0, 0, 0, 0, 0 };
NEARDATA coord inv_pos = { 0, 0 };

NEARDATA boolean defer_see_monsters = FALSE;
NEARDATA boolean in_mklev = FALSE;
NEARDATA boolean stoned = FALSE; /* done to monsters hit by 'c' */
NEARDATA boolean unweapon1 = FALSE;
NEARDATA boolean unweapon2 = FALSE;
NEARDATA boolean mrg_to_wielded = FALSE;
/* weapon picked is merged with wielded one */

NEARDATA boolean in_steed_dismounting = FALSE;
NEARDATA boolean has_strong_rngseed = FALSE;

NEARDATA coord bhitpos = DUMMY;
NEARDATA coord doors[DOORMAX] = { DUMMY };

NEARDATA struct mkroom rooms[(MAXNROFROOMS + 1) * 2] = { DUMMY };
NEARDATA struct mkroom *subrooms = &rooms[MAXNROFROOMS + 1];
struct mkroom *upstairs_room, *dnstairs_room, *sstairs_room;

dlevel_t level; /* level map */
struct trap *ftrap = (struct trap *) 0;
NEARDATA struct monst youmonst = DUMMY;
NEARDATA struct context_info context = DUMMY;
NEARDATA struct flag flags = DUMMY;
#ifdef SYSFLAGS
NEARDATA struct sysflag sysflags = DUMMY;
#endif
NEARDATA struct instance_flags iflags = DUMMY;
NEARDATA struct you u = DUMMY;
NEARDATA time_t ubirthday = DUMMY;
NEARDATA struct u_realtime urealtime = DUMMY;

schar lastseentyp[COLNO][ROWNO] = {
    DUMMY
}; /* last seen/touched dungeon typ */

NEARDATA struct obj
    *invent = (struct obj *) 0,
    *uwep = (struct obj *) 0, *uarm = (struct obj *) 0,
    *uswapwep = (struct obj *) 0,
    *uswapwep2 = (struct obj*) 0,
    *uquiver = (struct obj *) 0,       /* quiver */
    *uarmu = (struct obj *) 0,     /* under-wear, so to speak */
    *uarmo = (struct obj *) 0,     /* over-wear, so to speak */
    *uarmb = (struct obj*) 0,     /* bracers */
    *uskin = (struct obj *) 0, /* dragon armor, if a dragon */
    *uarmc = (struct obj *) 0, *uarmh = (struct obj *) 0,
    *uarms = (struct obj *) 0, *uarmg = (struct obj *) 0,
    *uarmf = (struct obj *) 0, *uamul = (struct obj *) 0,
    *umisc = (struct obj*) 0, *umisc2 = (struct obj*) 0, *umisc3 = (struct obj*) 0, *umisc4 = (struct obj*) 0, *umisc5 = (struct obj*) 0,     /* miscellaneous */
    *uright = (struct obj *) 0, *uleft = (struct obj *) 0,
    *ublindf = (struct obj *) 0, *uchain = (struct obj *) 0,
    *uball = (struct obj *) 0;
/* some objects need special handling during destruction or placement */
NEARDATA struct obj
    *current_wand = 0,  /* wand currently zapped/applied */
    *thrownobj = 0,     /* object in flight due to throwing */
    *kickedobj = 0;     /* object in flight due to kicking */

NEARDATA const struct zap_type_definition zap_type_definitions[MAX_ZAP_TYPES] = {
    {"magic", HI_ZAP, MAGIC_MISSILE_RAY_ANIMATION},
    {"fire", CLR_ORANGE, FIRE_RAY_ANIMATION},
    {"frost", CLR_WHITE, FROST_RAY_ANIMATION},
    {"sleep", HI_ZAP, SLEEP_RAY_ANIMATION},
    {"disintegration", CLR_BLACK, DISINTEGRATION_RAY_ANIMATION},
    {"lightning", CLR_WHITE, LIGHTNING_RAY_ANIMATION},
    {"poison", CLR_GREEN, NO_ANIMATION},
    {"acid", CLR_YELLOW, NO_ANIMATION},
    {"death", CLR_GRAY, DEATH_RAY_ANIMATION},
    {"petrification", CLR_BROWN, PETRIFICATION_RAY_ANIMATION},

    {"magic-breath", HI_ZAP, NO_ANIMATION},
    {"fire-breath", CLR_ORANGE, FIRE_BREATH_ANIMATION},
    {"frost-breath", CLR_WHITE, FROST_BREATH_ANIMATION},
    {"sleep-breath", HI_ZAP, SLEEP_RAY_ANIMATION},
    {"disintegration-breath", CLR_BLACK, DISINTEGRATION_RAY_ANIMATION},
    {"lightning-breath", CLR_WHITE, LIGHTNING_RAY_ANIMATION},
    {"poison-breath", CLR_GREEN, NO_ANIMATION},
    {"acid-breath", CLR_YELLOW, NO_ANIMATION},
    {"death-breath", CLR_GRAY, DEATH_RAY_ANIMATION},
    {"petrification-breath", CLR_BROWN, PETRIFICATION_RAY_ANIMATION},

    {"digging", CLR_GRAY, DIGGING_RAY_ANIMATION},
    {"evaporation", CLR_BLUE, NO_ANIMATION},
    {"flashed-light", CLR_WHITE, NO_ANIMATION},
};

NEARDATA const struct explosion_type_definition explosion_type_definitions[MAX_EXPLOSIONS] = {
    {"dark", CLR_BLACK, NO_ANIMATION, SFX_EXPLOSION_DARK},
    {"noxious", CLR_GREEN, NOXIOUS_EXPLOSION_ANIMATION, SFX_EXPLOSION_NOXIOUS},
    {"muddy", CLR_BROWN, NO_ANIMATION, SFX_EXPLOSION_MUDDY},
    {"wet", CLR_BLUE, NO_ANIMATION, SFX_EXPLOSION_WET},
    {"magical", CLR_MAGENTA, MAGIC_EXPLOSION_ANIMATION, SFX_EXPLOSION_MAGICAL},
    {"fiery", CLR_ORANGE, FIERY_EXPLOSION_ANIMATION, SFX_EXPLOSION_FIERY},
    {"frosty", CLR_WHITE, FROSTY_EXPLOSION_ANIMATION, SFX_EXPLOSION_FROSTY},
    /* Additional explosions */
    {"scroll-of-fire", CLR_ORANGE, SCROLL_OF_FIRE_ANIMATION, SFX_TOWER_OF_FLAME_ERUPTS},
    {"meteor-swarm", CLR_ORANGE, METEOR_SWARM_ANIMATION, SFX_EXPLOSION_FIERY},
    /* Large effects start here */
    {"circle-of-fire", CLR_ORANGE, CIRCLE_OF_FIRE_ANIMATION, SFX_CIRCLE_OF_FIRE},
    {"circle-of-frost", CLR_WHITE, CIRCLE_OF_FROST_ANIMATION, SFX_CIRCLE_OF_FROST},
    {"circle-of-lightning", CLR_BRIGHT_CYAN, CIRCLE_OF_LIGHTNING_ANIMATION, SFX_CIRCLE_OF_LIGHTNING},
    {"circle-of-magic", CLR_MAGENTA, CIRCLE_OF_MAGIC_ANIMATION, SFX_CIRCLE_OF_MAGIC},
    {"circle-of-radiance", CLR_MAGENTA, CIRCLE_OF_RADIANCE_ANIMATION, SFX_CIRCLE_OF_RADIANCE},
    {"scroll-of-earth", CLR_BROWN, SCROLL_OF_EARTH_ANIMATION, SFX_RUMBLING_EARTH},
    {"flaming-sphere", CLR_ORANGE, FLAMING_SPHERE_EXPLOSION_ANIMATION, SFX_EXPLOSION_FLAMING_SPHERE},
    {"freezing-sphere", CLR_WHITE, FREEZING_SPHERE_EXPLOSION_ANIMATION, SFX_EXPLOSION_FREEZING_SPHERE},
    {"shocking-sphere", CLR_BRIGHT_CYAN, SHOCKING_SPHERE_EXPLOSION_ANIMATION, SFX_EXPLOSION_SHOCKING_SPHERE},
};


const int shield_static[SHIELD_COUNT] = {
    S_ss1, S_ss2, S_ss3, S_ss2, S_ss1, S_ss2, S_ss4, /* 7 per row */
    S_ss1, S_ss2, S_ss3, S_ss2, S_ss1, S_ss2, S_ss4,
    S_ss1, S_ss2, S_ss3, S_ss2, S_ss1, S_ss2, S_ss4,
};


NEARDATA struct spell spl_book[MAXSPELL + 1] = { DUMMY };

NEARDATA long moves = 1L, monstermoves = 1L;
/* These diverge when player is Fast */
NEARDATA long wailmsg = 0L;

/* objects that are moving to another dungeon level */
NEARDATA struct obj *migrating_objs = (struct obj *) 0;
/* objects not yet paid for */
NEARDATA struct obj *billobjs = (struct obj *) 0;
/* objects in hero's memory */
NEARDATA struct obj* memoryobjs = (struct obj*)0;

/* used to zero all elements of a struct obj and a struct monst */
NEARDATA const struct obj zeroobj = DUMMY;
NEARDATA const struct monst zeromonst = DUMMY;
/* used to zero out union any; initializer deliberately omitted */
NEARDATA const anything zeroany;

NEARDATA const struct layer_info zerolayerinfo = DUMMY;
NEARDATA const struct replacement_info zeroreplacementinfo = DUMMY;
NEARDATA const struct extended_menu_info zeroextendedmenuinfo = DUMMY;
NEARDATA const struct extended_create_window_info zerocreatewindowinfo = DUMMY;
NEARDATA const struct d_level zerodlevel = DUMMY;

#ifdef GNH_MOBILE
NEARDATA int gnh_has_rogue_level = 0;
#else
NEARDATA int gnh_has_rogue_level = 1;
#endif


boolean disallow_keyboard_commands_in_wait_loop = FALSE;

NEARDATA struct obj* getobj_autoselect_obj = (struct obj*)0;

/* getobj class definitions */
NEARDATA const char getobj_comestibles[] = { FOOD_CLASS, REAGENT_CLASS, 0 };
NEARDATA const char getobj_offerfodder[] = { FOOD_CLASS, AMULET_CLASS, 0 };
NEARDATA const char getobj_all_count[] = { ALLOW_COUNT, ALL_CLASSES, 0 };

/* Gold must come first for getobj(). */
NEARDATA const char getobj_allobj[] = {
    COIN_CLASS,   WEAPON_CLASS, ARMOR_CLASS,  POTION_CLASS,
    SCROLL_CLASS, WAND_CLASS,   RING_CLASS,   AMULET_CLASS, MISCELLANEOUS_CLASS,
    FOOD_CLASS,   REAGENT_CLASS,  TOOL_CLASS,   GEM_CLASS,
    ROCK_CLASS,      BALL_CLASS,   CHAIN_CLASS,  SPBOOK_CLASS, 0
};

NEARDATA const char getobj_styluses[] = { ALL_CLASSES, ALLOW_NONE,
                                          TOOL_CLASS,  WEAPON_CLASS,
                                          WAND_CLASS,  GEM_CLASS,
                                          RING_CLASS,  0 };

NEARDATA const char getobj_drop_types[] = { ALLOW_COUNT, COIN_CLASS,
                                            ALL_CLASSES, 0 };

NEARDATA const char getobj_callable[] = {
    SCROLL_CLASS, POTION_CLASS, WAND_CLASS,  RING_CLASS, AMULET_CLASS, FOOD_CLASS,
    GEM_CLASS, SPBOOK_CLASS, ARMOR_CLASS, TOOL_CLASS, MISCELLANEOUS_CLASS, WEAPON_CLASS, 0
};

NEARDATA const char getobj_allowall[] = { ALL_CLASSES, 0 };

NEARDATA const char getobj_recharge_type[] = { ALLOW_COUNT, ALL_CLASSES, 0 };
NEARDATA const char getobj_invoke_types[] = { ALL_CLASSES, 0 };
NEARDATA const char getobj_clothes[] = {
    ARMOR_CLASS, RING_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, TOOL_CLASS, FOOD_CLASS, 0
};
NEARDATA const char getobj_accessories[] = {
    RING_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, TOOL_CLASS, FOOD_CLASS, ARMOR_CLASS, 0
};
NEARDATA const char getobj_beverages[] = { POTION_CLASS, TOOL_CLASS, 0 };
NEARDATA const char getobj_wield_objs[] = {
    ALL_CLASSES, ALLOW_NONE, WEAPON_CLASS, TOOL_CLASS, 0
};
NEARDATA const char getobj_unwield_objs[] = { ALL_CLASSES,  0 };
NEARDATA const char getobj_stash_objs[] = { ALLOW_COUNT, COIN_CLASS, ALL_CLASSES, 0 };
NEARDATA const char getobj_ready_objs[] = {
    ALLOW_COUNT, COIN_CLASS, ALL_CLASSES, ALLOW_NONE, WEAPON_CLASS, 0
};
NEARDATA const char getobj_bullets[] = { /* (note: different from dothrow.c) */
    ALLOW_COUNT, COIN_CLASS, ALL_CLASSES, ALLOW_NONE,
    GEM_CLASS, WEAPON_CLASS, 0
};
NEARDATA const char getobj_readable[] = { ALL_CLASSES, SCROLL_CLASS,
                                          SPBOOK_CLASS, 0 };
NEARDATA const char getobj_cuddly[] = { TOOL_CLASS, GEM_CLASS, 0 };

NEARDATA const char getobj_toss_objs[] = { ALLOW_COUNT, COIN_CLASS,
                                           ALL_CLASSES, WEAPON_CLASS, 0 };
NEARDATA const char getobj_tippables[] = { ALL_CLASSES, TOOL_CLASS, 0 };
NEARDATA const char getobj_zap_syms[] = { WAND_CLASS, 0 };
NEARDATA const char getobj_mark_autostashs[] = { ALL_CLASSES,  TOOL_CLASS, 0 };
NEARDATA const char getobj_unmark_autostashs[] = { ALL_CLASSES,  TOOL_CLASS, 0 };

/* originally from dog.c */
NEARDATA char dogname[PL_PSIZ] = DUMMY;
NEARDATA char catname[PL_PSIZ] = DUMMY;
NEARDATA char horsename[PL_PSIZ] = DUMMY;
NEARDATA char ramname[PL_PSIZ] = DUMMY;
NEARDATA char luggagename[PL_PSIZ] = DUMMY;
NEARDATA char wolfname[PL_PSIZ] = DUMMY;
char preferred_pet; /* '\0', 'c', 'd', 'n' (none) */
short doggender, catgender, horsegender, ramgender, wolfgender; /* 0 = random, 1 = male, 2 = female */

/* monsters that went down/up together with @ */
NEARDATA struct monst *mydogs = (struct monst *) 0;
/* monsters that are moving to another dungeon level */
NEARDATA struct monst *migrating_mons = (struct monst *) 0;

NEARDATA struct mvitals mvitals[NUM_MONSTERS];
NEARDATA long domove_attempting = 0L;
NEARDATA long domove_succeeded = 0L;

NEARDATA struct c_color_names c_color_names = {
    "black",  "amber", "golden", "light blue", "red",   "green",
    "silver", "blue",  "purple", "white",      "orange"
};

struct menucoloring *menu_colorings = NULL;

const char *c_obj_colors[] = {
    "black",          /* CLR_BLACK */
    "red",            /* CLR_RED */
    "green",          /* CLR_GREEN */
    "brown",          /* CLR_BROWN */
    "blue",           /* CLR_BLUE */
    "magenta",        /* CLR_MAGENTA */
    "cyan",           /* CLR_CYAN */
    "gray",           /* CLR_GRAY */
    "transparent",    /* no_color */
    "orange",         /* CLR_ORANGE */
    "bright green",   /* CLR_BRIGHT_GREEN */
    "yellow",         /* CLR_YELLOW */
    "bright blue",    /* CLR_BRIGHT_BLUE */
    "bright magenta", /* CLR_BRIGHT_MAGENTA */
    "bright cyan",    /* CLR_BRIGHT_CYAN */
    "white",          /* CLR_WHITE */
};

struct c_common_strings c_common_strings = { "Nothing happens.",
                                             "That's enough tries!",
                                             "That is a silly thing to %s.",
                                             "shudder for a moment.",
                                             "something",
                                             "Something",
                                             "You can move again.",
                                             "Never mind.",
                                             "vision quickly clears.",
                                             { "the", "your" } };


/* Vision */
NEARDATA boolean vision_full_recalc = 0;
NEARDATA char **viz_array = 0; /* used in cansee() and couldsee() macros */

/* Vision */
NEARDATA boolean hearing_full_recalc = 0;
NEARDATA float hearing_array[COLNO][ROWNO] = { { 0 } };

/* Global windowing data, defined here for multi-window-system support */
NEARDATA winid WIN_MESSAGE = WIN_ERR;
NEARDATA winid WIN_STATUS = WIN_ERR;
NEARDATA winid WIN_MAP = WIN_ERR, WIN_INVEN = WIN_ERR, WIN_HERE = WIN_ERR;

char toplines[TBUFSZ];
char toplinecolors[TBUFSZ];
char toplineattrs[TBUFSZ];

/* Windowing stuff that's really tty oriented, but present for all ports */
struct tc_gbl_data tc_gbl_data = { 0, 0, 0, 0 }; /* AS,AE, LI,CO */

char *fqn_prefix[PREFIX_COUNT] = { (char *) 0, (char *) 0, (char *) 0,
                                   (char *) 0, (char *) 0, (char *) 0,
                                   (char *) 0, (char *) 0, (char *) 0,
                                   (char *) 0 };
                                   
#ifdef PREFIXES_IN_USE
const char *fqn_prefix_names[PREFIX_COUNT] = {
    "hackdir",  "leveldir", "savedir",    "bonesdir",  "datadir",
    "scoredir", "lockdir",  "sysconfdir", "configdir", "troubledir"
};
#endif

NEARDATA struct savefile_info sfcap = {
#ifdef NHSTDC
    0x00000000UL
#else
    0x00000000L
#endif
#if defined(COMPRESS) || defined(ZLIB_COMP)
        | SFI1_EXTERNALCOMP
#endif
#if defined(ZEROCOMP)
        | SFI1_ZEROCOMP
#endif
#if defined(RLECOMP)
        | SFI1_RLECOMP
#endif
    ,
#ifdef NHSTDC
    0x00000000UL, 0x00000000UL
#else
    0x00000000L, 0x00000000L
#endif
};

NEARDATA struct savefile_info sfrestinfo, sfsaveinfo = {
#ifdef NHSTDC
    0x00000000UL
#else
    0x00000000L
#endif
#if defined(COMPRESS) || defined(ZLIB_COMP)
        | SFI1_EXTERNALCOMP
#endif
#if defined(ZEROCOMP)
        | SFI1_ZEROCOMP
#endif
#if defined(RLECOMP)
        | SFI1_RLECOMP
#endif
    ,
#ifdef NHSTDC
    0x00000000UL, 0x00000000UL
#else
    0x00000000L, 0x00000000L
#endif
};

struct plinemsg_type *plinemsg_types = (struct plinemsg_type *) 0;

#ifdef PANICTRACE
const char *ARGV0;
#endif

/* support for lint.h */
unsigned nhUse_dummy = 0;
unsigned long file_end_marker = 0xF23EE6D8;


NEARDATA const char* Moloch = "Moloch";
/* for rejecting attempts to use wizard mode commands */
NEARDATA const char unavailcmd[] = "Unavailable command '%s'.";
/* for rejecting #if !SHELL, !SUSPEND */
NEARDATA const char cmdnotavail[] = "'%s' command not available.";

NEARDATA const char* const hofe_titles[3] = { "the Hand of Elbereth",
                                            "the Envoy of Balance",
                                            "the Glory of Arioch" };

NEARDATA char debug_buf_1[BUFSIZ] = "";
NEARDATA char debug_buf_2[BUFSIZ] = "";
NEARDATA char debug_buf_3[BUFSIZ] = "";
NEARDATA char debug_buf_4[BUFSIZ] = "";

/* dummy routine used to force linkage */
void
decl_init()
{
    return;
}

/*decl.c*/

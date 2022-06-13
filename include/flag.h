/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-13 */

/* GnollHack 4.0    flag.h    $NHDT-Date: 1554155745 2019/04/01 21:55:45 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.150 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

/* If you change the flag structure make sure you increment EDITLEVEL in   */
/* patchlevel.h if needed.  Changing the instance_flags structure does     */
/* not require incrementing EDITLEVEL.                                     */

#ifndef FLAG_H
#define FLAG_H

/*
 * Persistent flags that are saved and restored with the game.
 *
 */

struct flag {
    boolean acoustics;  /* allow dungeon sound messages */
    boolean autodig;    /* MRKR: Automatically dig */
    boolean autoquiver; /* Automatically fill quiver */
    boolean autoopen;   /* open doors by walking into them */
#ifdef ANDROID
    boolean  autokick;    /* Automatically kick doors and force locks */
#endif
    boolean autounlock;   /* unlock doors and boxes whenever you find them being locked */
    boolean baseacasbonus; /* show AC bonus for items (e.g., +1 or +2) rather than the AC they give (e.g, 8 or 9) */
    boolean beginner;
    boolean biff;      /* enable checking for mail */
    boolean bones;     /* allow saving/loading bones */
    boolean confirm;   /* confirm before hitting tame monsters */
    boolean dark_room; /* show shadows in lit rooms */
    boolean debug;     /* in debugging mode */
#define wizard flags.debug
    boolean end_own; /* list all own scores */
    boolean exchange_prompt; /* automatic prompt for exchanging armors */
    boolean explore; /* in exploration mode */
    boolean female;  /* initial selection */
#define discover flags.explore
    boolean friday13;        /* it's Friday the 13th */
    boolean help;            /* look in data file for info about stuff */
    boolean ibm2utf8;        /* converts IBMgraphics (cp437) to UTF-8 */
    boolean ignintr;         /* ignore interrupts */
    boolean ins_chkpt;       /* checkpoint as appropriate; INSURANCE */
    boolean invlet_constant; /* let objects keep their inventory symbol */
    boolean knapsack_prompt; /* automatic prompt for putting items in a bag / dropping them when knapsack is full */
    boolean legacy;          /* print game entry "story" */
    boolean lit_corridor;    /* show a dark corr as lit if it is in sight */
    boolean nap;             /* `timed_delay' option for display effects */
    boolean null;            /* OK to send nulls to the terminal */
    boolean p__obsolete;     /* [3.6.2: perm_invent moved to iflags] */
    boolean pickup;          /* whether you pickup or move and look */
    boolean pickup_thrown;   /* auto-pickup items you threw */
    boolean prefer_fast_move;/* g and G are swapped, and likewise for m and M */
    boolean pushweapon; /* When wielding, push old weapon into second slot */
    boolean rest_on_space;   /* space means rest */
    boolean self_click_action; /* enable action taken by clicking self in GUI (e.g., wait or pickup) */
    boolean safe_dog;        /* give complete protection to the dog */
    boolean displace_peaceful;  /* displace peaceful monsters */
    boolean showexp;         /* show experience points */
    boolean tellexp;         /* tell experience points when received */
    boolean showmove;         /* show move on statusline */
    boolean show_weapon_style;         /* show weapon style on statusline */
    boolean show_buff_timer;    /* show buff timers in GUI */
    boolean search_box_traps; /* search all boxes for traps when pressing s */
    boolean underline_peaceful;     /* underline non-tame peaceful monsters */
    boolean partylinecolor;     /* use fixed partyline coloring */
    boolean partydetails;     /* print additional info like AC and MC for all party members */
    boolean partymultiline;     /* print each party member on its own line */
    boolean metric_system;   /* use the metric system (kilograms) instead of avoirdupois pounds and ounces */
    boolean inventory_weights_last; /* inventory weights are shown after the name of the item rather first, as normal */
    boolean inventory_obj_cmd; /* list object commands in inventory */
    boolean classic_statue_symbol; /* statue is shown as rock */
    boolean classic_colors; /* use normal coloring scheme */
    boolean blinking_cursor_on_tiles;    /* cursor blinks on tiles */
    boolean detailed_weights; /* use ounces and grams instead of lbs and kg */
    boolean show_cursor_on_u; /* force show cursor on you even with tiles */
    boolean force_paint_at_cursor; /* force paint at cursor (normally, because the cursor appears on you) */
    uchar active_cursor_style;
    boolean show_tile_u_mark; /* show u tile mark on tiles */
    boolean show_tile_monster_target; /* show monster tile mark */
    boolean show_tile_mon_hp_bar; /* show mon hp bar on tiles */
    boolean show_tile_pet_hp_bar; /* show pet hp bar on tiles */
    boolean show_tile_u_hp_bar; /* show your hp bar on tiles */
    boolean show_weight_summary; /* show weight summary */
    boolean show_grid; /* show grid on tiles */
    boolean showscore;       /* show score */
    boolean silent;          /* whether the bell rings or not */
    /* The story so far:
     * 'sortloot' originally took a True/False value but was changed
     * to use a letter instead.  3.6.0 was released without changing its
     * type from 'boolean' to 'char'.  A compiler was smart enough to
     * complain that assigning any of the relevant letters was not 0 or 1
     * so not appropriate for boolean (by a configuration which used
     * SKIP_BOOLEAN to bypass GnollHack's 'boolean' and use a C++-compatible
     * one).  So the type was changed to 'xchar', which is guaranteed to
     * match the size of 'boolean' (this guarantee only applies for the
     * !SKIP_BOOLEAN config, unfortunately).  Since xchar does not match
     * actual use, the type was later changed to 'char'.  But that would
     * break 3.6.0 savefile compatibility for configurations which typedef
     * 'schar' to 'short int' instead of to 'char'.  (Needed by pre-ANSI
     * systems that use unsigned characters without a way to force them
     * to be signed.)  So, the type has been changed back to 'xchar' for
     * 3.6.x.
     *
     * TODO:  change to 'char' (and move out of this block of booleans,
     * and get rid of these comments...) once 3.6.0 savefile compatibility
     * eventually ends.
     */
#ifndef SKIP_BOOLEAN
    /* this is the normal configuration; assigning a character constant
       for a normal letter to an 'xchar' variable should always work even
       if 'char' is unsigned since character constants are actually 'int'
       and letters are within the range where signedness shouldn't matter */
    xchar   sortloot; /* 'n'=none, 'l'=loot (pickup), 'f'=full ('l'+invent) */
#else
    /* with SKIP_BOOLEAN, we have no idea what underlying type is being
       used, other than it isn't 'xchar' (although its size might match
       that) or a bitfield (because it must be directly addressable);
       it's probably either 'char' for compactness or 'int' for access,
       but we don't know which and it might be something else anyway;
       flip a coin here and guess 'char' for compactness */
    char    sortloot; /* 'n'=none, 'l'=loot (pickup), 'f'=full ('l'+invent) */
#endif
    boolean sortpack;        /* sorted inventory */
    boolean sparkle;         /* show "resisting" special FX (Scott Bigham) */
    boolean standout;        /* use standout for --More-- */
    boolean    swap_rhand_only; /* swap only right hand */
    boolean time;            /* display elapsed 'time' */
    boolean tombstone;       /* print tombstone */
    boolean verbose;         /* max battle info */
    boolean wiz_mstatusline; /* extra information on monsters */
    boolean wiz_alwaysenc;   /* creates always an encounter */

    int animation_frame_interval_in_milliseconds; /* custom animation frame interval in milliseconds. If 0, then the default at 25 milliseconds is used. Delay output is twice this amount, defaulting at 50 milliseconds */
    int move_interval_in_milliseconds; /* custom move step interval in milliseconds. If 0, then the default at 50 milliseconds is used. Delay output is twice this amount, defaulting at 50 milliseconds */
    int crawl_interval_in_milliseconds; /* custom crawl step interval in milliseconds. If 0, then the default at 250 milliseconds is used. Delay output is twice this amount, defaulting at 50 milliseconds */
    double screen_scale_adjustment; /* scale = 1.0 + screen_scale_adjustment */
    int preferred_screen_scale; /* user set preference for screen scale MIN_PREF_SCREEN_SCALE...MAX_PREF_SCREEN_SCALE */
    int shield_effect_length;
    int talk_effect_length;
    int last_item_show_duration;
    int sound_volume_general;   /* user set preference for general game volume 1...100 */
    int sound_volume_music;     /* user set preference for music volume 1...100 */
    int sound_volume_ambient;   /* user set preference for ambient volume 1...100 */
    int sound_volume_effects;   /* user set preference for sound effect volume 1...100 */
    int sound_volume_dialogue;   /* user set preference for dialogue volume 1...100 */
    int sound_volume_ui;        /* user set preference for user interface sound volume 1...100 */

    boolean modern_mode;  /* in modern mode */
#define ModernMode flags.modern_mode
    boolean casual_mode;  /* in casual mode */
#define CasualMode flags.casual_mode

    int end_top, end_around; /* describe desired score list */
    unsigned moonphase;
    unsigned long suppress_alert;
#define NEW_MOON 0
#define FULL_MOON 4
    unsigned paranoia_bits; /* alternate confirmation prompting */
#define PARANOID_CONFIRM    0x0001
#define PARANOID_QUIT       0x0002
#define PARANOID_DIE        0x0004
#define PARANOID_BONES      0x0008
#define PARANOID_HIT        0x0010
#define PARANOID_PRAY       0x0020
#define PARANOID_REMOVE     0x0040
#define PARANOID_BREAK      0x0080
#define PARANOID_WERECHANGE 0x0100
#define PARANOID_WATER      0x0200
#define PARANOID_TRAP       0x0400
#define PARANOID_AUTOALL    0x0800
    int pickup_burden; /* maximum burden before prompt */
    int pile_limit;    /* controls feedback when walking over objects */
    char inv_order[MAX_OBJECT_CLASSES];
    char pickup_types[MAX_OBJECT_CLASSES];
#define NUM_DISCLOSURE_OPTIONS 6 /* i,a,v,g,c,o (decl.c) */
#define DISCLOSE_PROMPT_DEFAULT_YES 'y'
#define DISCLOSE_PROMPT_DEFAULT_NO 'n'
#define DISCLOSE_PROMPT_DEFAULT_SPECIAL '?' /* v, default a */
#define DISCLOSE_YES_WITHOUT_PROMPT '+'
#define DISCLOSE_NO_WITHOUT_PROMPT '-'
#define DISCLOSE_SPECIAL_WITHOUT_PROMPT '#' /* v, use a */
    char end_disclose[NUM_DISCLOSURE_OPTIONS + 1]; /* disclose various
                                                      info upon exit */
    char menu_style;    /* User interface style setting */
    boolean made_fruit; /* don't easily let the user overflow the number of
                           fruits */

    /* KMH, role patch -- Variables used during startup.
     *
     * If the user wishes to select a role, race, gender, and/or alignment
     * during startup, the choices should be recorded here.  This
     * might be specified through command-line options, environmental
     * variables, a popup dialog box, menus, etc.
     *
     * These values are each an index into an array.  They are not
     * characters or letters, because that limits us to 26 roles.
     * They are not booleans, because someday someone may need a neuter
     * gender.  Negative values are used to indicate that the user
     * hasn't yet specified that particular value.  If you determine
     * that the user wants a random choice, then you should set an
     * appropriate random value; if you just left the negative value,
     * the user would be asked again!
     *
     * These variables are stored here because the u structure is
     * cleared during character initialization, and because the
     * flags structure is restored for saved games.  Thus, we can
     * use the same parameters to build the role entry for both
     * new and restored games.
     *
     * These variables should not be referred to after the character
     * is initialized or restored (specifically, after role_init()
     * is called).
     */
    int initrole;  /* starting role      (index into roles[])   */
    int initrace;  /* starting race      (index into races[])   */
    int initgend;  /* starting gender    (index into genders[]) */
    int initalign; /* starting alignment (index into aligns[])  */
    int randomall; /* randomly assign everything not specified */
    int pantheon;  /* deity selection for priest character */
    /* Items which were in iflags in 3.4.x to preserve savefile compatibility
     */
    boolean lootabc;   /* use "a/b/c" rather than "o/i/b" when looting */
    boolean showrace;  /* show hero glyph by race rather than by role */
    boolean travelcmd; /* allow travel command */
    int runmode;       /* update screen display during run moves */
};

/*
 * System-specific flags that are saved with the game if SYSFLAGS is defined.
 */

#if defined(AMIFLUSH) || defined(AMII_GRAPHICS) || defined(OPT_DISPMAP)
#define SYSFLAGS
#else
#if defined(MFLOPPY) || defined(MAC)
#define SYSFLAGS
#endif
#endif

#ifdef SYSFLAGS
struct sysflag {
    char sysflagsid[10];
#ifdef AMIFLUSH
    boolean altmeta;  /* use ALT keys as META */
    boolean amiflush; /* kill typeahead */
#endif
#ifdef AMII_GRAPHICS
    int numcols;
    unsigned short
        amii_dripens[20]; /* DrawInfo Pens currently there are 13 in v39 */
    AMII_COLOR_TYPE amii_curmap[AMII_MAXCOLORS]; /* colormap */
#endif
#ifdef OPT_DISPMAP
    boolean fast_map; /* use optimized, less flexible map display */
#endif
#ifdef MFLOPPY
    boolean asksavedisk;
#endif
#ifdef MAC
    boolean page_wait; /* put up a --More-- after a page of messages */
#endif
};
#endif

/*
 * Flags that are set each time the game is started.
 * These are not saved with the game.
 *
 */

/* values for iflags.getpos_coords */
#define GPCOORDS_NONE    'n'
#define GPCOORDS_MAP     'm'
#define GPCOORDS_COMPASS 'c'
#define GPCOORDS_COMFULL 'f'
#define GPCOORDS_SCREEN  's'

enum getloc_filters {
    GFILTER_NONE = 0,
    GFILTER_VIEW,
    GFILTER_AREA,

    NUM_GFILTER
};

struct debug_flags {
    boolean test;
#ifdef TTY_GRAPHICS
    boolean ttystatus;
#endif
#ifdef WIN32
    boolean immediateflips;
#endif
};

struct instance_flags {
    /* stuff that really isn't option or platform related. They are
     * set and cleared during the game to control the internal
     * behaviour of various GnollHack functions and probably warrant
     * a structure of their own elsewhere some day.
     */
    boolean debug_fuzzer;  /* fuzz testing */
    boolean defer_plname;  /* X11 hack: askname() might not set plname */
    boolean herecmd_menu;  /* use menu when mouseclick on yourself */
    boolean invis_goldsym; /* gold symbol is ' '? */
    int failing_untrap;    /* move_into_trap() -> spoteffects() -> dotrap() */
    int in_lava_effects;   /* hack for Boots_off() */
    int last_msg;          /* indicator of last message player saw */
    int override_ID;       /* true to force full identification of objects */
    int parse_config_file_src;  /* hack for parse_config_line() */
    int purge_monsters;    /* # of dead monsters still on fmon list */
    int suppress_price;    /* controls doname() for unpaid objects */
    int terrainmode; /* for getpos()'s autodescribe when #terrain is active */
#define TER_MAP    0x01
#define TER_TRP    0x02
#define TER_OBJ    0x04
#define TER_MON    0x08
#define TER_DETECT 0x10    /* detect_foo magic rather than #terrain */
    boolean getloc_travelmode;
    int getloc_filter;     /* GFILTER_foo */
    boolean getloc_usemenu;
    boolean getloc_moveskip;
    coord travelcc;        /* coordinates for travel_cache */
    boolean trav_debug;    /* display travel path (#if DEBUG only) */
    boolean window_inited; /* true if init_nhwindows() completed */
    boolean vision_inited; /* true if vision is ready */
    boolean sanity_check;  /* run sanity checks */
    boolean mon_polycontrol; /* debug: control monster polymorphs */
    boolean in_dumplog;    /* doing the dumplog right now? */
    boolean in_parse;      /* is a command being parsed? */

    /* stuff that is related to options and/or user or platform preferences
     */
    unsigned msg_history; /* hint: # of top lines to save */
    int getpos_coords;    /* show coordinates when getting cursor position */
    int menu_headings;    /* ATR for menu headings */
    int *opt_booldup;     /* for duplication of boolean opts in config file */
    int *opt_compdup;     /* for duplication of compound opts in conf file */
#ifdef ALTMETA
    boolean altmeta;      /* Alt-c sends ESC c rather than M-c */
#endif
    boolean autodescribe;     /* autodescribe mode in getpos() */
    boolean cbreak;           /* in cbreak mode, rogue format */
    boolean deferred_X;       /* deferred entry into explore mode */
    boolean echo;             /* 1 to echo characters */
    boolean force_invmenu;    /* always menu when handling inventory */
    /* FIXME: goldX belongs in flags, but putting it in iflags avoids
       breaking 3.6.[01] save files */
    boolean goldX;            /* for BUCX filtering, whether gold is X or U */
    boolean using_gui_tiles;  /* using GnollHack GUI tiles mode */
    boolean using_gui_sounds;  /* using GnollHack GUI with sounds */
    boolean has_main_menu;     /* upon nh_terminate with EXIT_SUCCESS, fall back to main menu */
    boolean hilite_pile;      /* mark piles of objects with a hilite */
    boolean implicit_uncursed; /* maybe omit "uncursed" status in inventory */
    boolean mention_walls;    /* give feedback when bumping walls */
    boolean menu_head_objsym; /* Show obj symbol in menu headings */
    boolean menu_overlay;     /* Draw menus over the map */
    boolean menu_requested;   /* Flag for overloaded use of 'm' prefix
                               * on some non-move commands */
    boolean menu_tab_sep;     /* Use tabs to separate option menu fields */
    boolean skill_table_format; /* use table format for skill menu */
    boolean spell_table_format; /* use table format for spell menu */
    boolean news;             /* print news */
    boolean num_pad;          /* use numbers for movement commands */
    boolean enablettyarrowkeys;  /* experimental: enable tty arrow keys but comes with drawvback of double ESC */
    boolean perm_invent;      /* keep full inventories up until dismissed */
    boolean renameallowed;    /* can change hero name during role selection */
    boolean renameinprogress; /* we are changing hero name */
    boolean status_updates;   /* allow updates to bottom status lines;
                               * disable to avoid excessive noise when using
                               * a screen reader (use ^X to review status) */
    boolean toptenwin;        /* ending list in window instead of stdout */
    boolean use_background_glyph; /* use background glyph when appropriate */
    boolean use_menu_color;   /* use color in menus; only if wc_color */
#ifdef STATUS_HILITES
    long hilite_delta;     /* number of moves to leave a temp hilite lit */
    long unhilite_deadline; /* time when oldest temp hilite should be unlit */
#endif
    boolean zerocomp;         /* write zero-compressed save files */
    boolean rlecomp;          /* alternative to zerocomp; run-length encoding
                               * compression of levels when writing savefile */
    uchar num_pad_mode;
    boolean cursesgraphics;     /* Use portable curses extended characters */
#if 0   /* XXXgraphics superseded by symbol sets */
    boolean  DECgraphics;       /* use DEC VT-xxx extended character set */
    boolean  IBMgraphics;       /* use IBM extended character set */
#ifdef MAC_GRAPHICS_ENV
    boolean  MACgraphics;       /* use Macintosh extended character set, as
                                   as defined in the special font HackFont */
#endif
#endif
    nhsym bouldersym; /* symbol for boulder display */
    char prevmsg_window; /* type of old message window to use */
    boolean extmenu;     /* extended commands use menu interface */
#ifdef MFLOPPY
    boolean checkspace; /* check disk space before writing files */
                        /* (in iflags to allow restore after moving
                         * to >2GB partition) */
#endif
#ifdef MICRO
    boolean BIOS; /* use IBM or ST BIOS calls when appropriate */
#endif
#if defined(MICRO) || defined(WIN32)
    boolean rawio; /* whether can use rawio (IOCTL call) */
#endif
#ifdef MAC_GRAPHICS_ENV
    boolean MACgraphics; /* use Macintosh extended character set, as
                            as defined in the special font HackFont */
    unsigned use_stone;  /* use the stone ppats */
#endif
#if defined(MSDOS) || defined(WIN32)
    boolean hassound;     /* has a sound card */
    boolean usesound;     /* use the sound card */
    boolean usepcspeaker; /* use the pc speaker */
    boolean tile_view;
    boolean over_view;
    boolean traditional_view;
#endif
#ifdef MSDOS
    boolean hasvga; /* has a vga adapter */
    boolean usevga; /* use the vga adapter */
    boolean hasvesa; /* has a VESA-capable VGA adapter */
    boolean usevesa; /* use the VESA-capable VGA adapter */
    boolean grmode; /* currently in graphics mode */
#endif
#ifdef LAN_FEATURES
    boolean lan_mail;         /* mail is initialized */
    boolean lan_mail_fetched; /* mail is awaiting display */
#endif
#ifdef TTY_TILES_ESCCODES
    boolean vt_tiledata;     /* output console codes for tile support in TTY */
#endif
    boolean clicklook;       /* allow right-clicking for look */
    boolean cmdassist;       /* provide detailed assistance for some comnds */
    boolean time_botl;       /* context.botl for 'time' (moves) only */
    boolean wizweight;       /* display weight of everything in wizard mode */

    /* Is the system in demo version */
    boolean demo_version;  /* in demo version mode */
#define In_Demo iflags.demo_version

    /*
     * Window capability support.
     */
    boolean wc_color;         /* use color graphics                  */
    boolean wc_hilite_pet;    /* hilight pets                        */
    boolean wc_ascii_map;     /* show map using traditional ascii    */
    boolean wc_tiled_map;     /* show map using tiles                */
    boolean wc_preload_tiles; /* preload tiles into memory           */
    int wc_tile_width;        /* tile width                          */
    int wc_tile_height;       /* tile height                         */
    char *wc_tile_file[MAX_TILE_SHEETS];       /* name of tile file;overrides default */
    boolean wc_inverse;       /* use inverse video for some things   */
    int wc_align_status;      /*  status win at top|bot|right|left   */
    int wc_align_message;     /* message win at top|bot|right|left   */
    int wc_vary_msgcount;     /* show more old messages at a time    */
    char *wc_foregrnd_menu; /* points to foregrnd color name for menu win   */
    char *wc_backgrnd_menu; /* points to backgrnd color name for menu win   */
    char *wc_foregrnd_message; /* points to foregrnd color name for msg win */
    char *wc_backgrnd_message; /* points to backgrnd color name for msg win */
    char *wc_foregrnd_status; /* points to foregrnd color name for status   */
    char *wc_backgrnd_status; /* points to backgrnd color name for status   */
    char *wc_foregrnd_text; /* points to foregrnd color name for text win   */
    char *wc_backgrnd_text; /* points to backgrnd color name for text win   */
    char *wc_font_map;      /* points to font name for the map win */
    char *wc_font_message;  /* points to font name for message win */
    char *wc_font_status;   /* points to font name for status win  */
    char *wc_font_menu;     /* points to font name for menu win    */
    char *wc_font_text;     /* points to font name for text win    */
    int wc_fontsiz_map;     /* font size for the map win           */
    int wc_fontsiz_message; /* font size for the message window    */
    int wc_fontsiz_status;  /* font size for the status window     */
    int wc_fontsiz_menu;    /* font size for the menu window       */
    int wc_fontsiz_text;    /* font size for text windows          */
    int wc_scroll_amount;   /* scroll this amount at scroll_margin */
    int wc_scroll_margin;   /* scroll map when this far from the edge */
    int wc_map_mode;        /* specify map viewing options, mostly
                             * for backward compatibility */
    int wc_player_selection;    /* method of choosing character */
    boolean wc_splash_screen;   /* display an opening splash screen or not */
    boolean wc_popup_dialog;    /* put queries in pop up dialogs instead of
                                 * in the message window */
    boolean wc_eight_bit_input; /* allow eight bit input               */
    boolean wc2_fullscreen;     /* run fullscreen */
    boolean wc2_softkeyboard;   /* use software keyboard */
    boolean wc2_wraptext;       /* wrap text */
    boolean wc2_selectsaved;    /* display a menu of user's saved games */
    boolean wc2_darkgray;       /* try to use dark-gray color for black glyphs */
    boolean wc2_hitpointbar;    /* show graphical bar representing hit points */
    boolean wc2_guicolor;       /* allow colours in gui (outside map) */
    boolean wc2_autostatuslines;  /* automatically adjust the number of status lines */
    boolean wc2_herewindow;     /* show here window */
    int wc2_here_window_size;   /* default = 10; can be set up to 20 */
    int wc_mouse_support;       /* allow mouse support */
    int wc2_term_cols;          /* terminal width, in characters */
    int wc2_term_rows;          /* terminal height, in characters */
    int wc2_statuslines;        /* default = 2, can handle up to 8 */
    int wc2_windowborders;      /* display borders on GnollHack windows */
    int wc2_petattr;            /* text attributes for pet */
    char* wc2_master_bank_file; /* name of custom master bank file, overrides default */
    char* wc2_master_strings_bank_file; /* name of custom master strings bank file, overrides default */
    char* wc2_auxiliary_bank_file; /* name of custom auxiliary bank file, overrides default */
    struct autopickup_exception *autopickup_exceptions[2];
#define AP_LEAVE 0
#define AP_GRAB 1
#ifdef WIN32
#define MAX_ALTKEYHANDLER 25
    char altkeyhandler[MAX_ALTKEYHANDLER];
#endif
    /* copies of values in struct u, used during detection when the
       originals are temporarily cleared; kept here rather than
       locally so that they can be restored during a hangup save */
    Bitfield(save_uswallow, 1);
    Bitfield(save_uinwater, 1);
    Bitfield(save_uburied, 1);
    /* item types used to acomplish "special achievements"; find the target
       object and you'll be flagged as having achieved something... */
    short mines_prize_type;     /* luckstone */
    short mines_prize_oartifact; /* The Gladstone */
    short soko_prize_type1;     /* bag of holding or    */
    short soko_prize_type2;     /* amulet of reflection */
#ifdef ANDROID
    boolean dumplog; /* enable/disable dump logs */
#endif
    struct debug_flags debug;
    boolean windowtype_locked;  /* windowtype can't change from configfile */
    boolean windowtype_deferred; /* pick a windowport and store it in
                                    chosen_windowport[], but do not switch to
                                    it in the midst of options processing */
    boolean obsolete;  /* obsolete options can point at this, it isn't used */
};

/*
 * Old deprecated names
 */
#ifdef TTY_GRAPHICS
#define eight_bit_tty wc_eight_bit_input
#endif
#define use_color wc_color
#define hilite_pet wc_hilite_pet
#define use_inverse wc_inverse
#ifdef MAC_GRAPHICS_ENV
#define large_font obsolete
#endif
#ifdef MAC
#define popup_dialog wc_popup_dialog
#endif
#define preload_tiles wc_preload_tiles

extern NEARDATA struct flag flags;
#ifdef SYSFLAGS
extern NEARDATA struct sysflag sysflags;
#endif
extern NEARDATA struct instance_flags iflags;

/* last_msg values
 * Usage:
 *  pline("some message");
 *    pline: vsprintf + putstr + iflags.last_msg = PLNMSG_UNKNOWN;
 *  iflags.last_msg = PLNMSG_some_message;
 * and subsequent code can adjust the next message if it is affected
 * by some_message.  The next message will clear iflags.last_msg.
 */
enum plnmsg_types {
    PLNMSG_UNKNOWN = 0,         /* arbitrary */
    PLNMSG_ONE_ITEM_HERE,       /* "you see <single item> here" */
    PLNMSG_TOWER_OF_FLAME,      /* scroll of fire */
    PLNMSG_CAUGHT_IN_EXPLOSION, /* explode() feedback */
    PLNMSG_OBJ_GLOWS,           /* "the <obj> glows <color>" */
    PLNMSG_OBJNAM_ONLY          /* xname/doname only, for #tip */
};

/* runmode options */
enum runmode_types {
    RUN_TPORT = 0, /* don't update display until movement stops */
    RUN_LEAP,      /* update display every 7 steps */
    RUN_STEP,      /* update display every single step */
    RUN_CRAWL      /* walk w/ extra delay after each update */
};

/* paranoid confirmation prompting */
/* any yes confirmations also require explicit no (or ESC) to reject */
#define ParanoidConfirm ((flags.paranoia_bits & PARANOID_CONFIRM) != 0)
/* quit: yes vs y for "Really quit?" and "Enter explore mode?" */
#define ParanoidQuit ((flags.paranoia_bits & PARANOID_QUIT) != 0)
/* die: yes vs y for "Die?" (dying in explore mode or wizard mode) */
#define ParanoidDie ((flags.paranoia_bits & PARANOID_DIE) != 0)
/* hit: yes vs y for "Save bones?" in wizard mode */
#define ParanoidBones ((flags.paranoia_bits & PARANOID_BONES) != 0)
/* hit: yes vs y for "Really attack <the peaceful monster>?" */
#define ParanoidHit ((flags.paranoia_bits & PARANOID_HIT) != 0)
/* pray: ask "Really pray?" (accepts y answer, doesn't require yes),
   taking over for the old prayconfirm boolean option */
#define ParanoidPray ((flags.paranoia_bits & PARANOID_PRAY) != 0)
/* remove: remove ('R') and takeoff ('T') commands prompt for an inventory
   item even when only one accessory or piece of armor is currently worn */
#define ParanoidRemove ((flags.paranoia_bits & PARANOID_REMOVE) != 0)
/* breakwand: Applying a wand */
#define ParanoidBreak ((flags.paranoia_bits & PARANOID_BREAK) != 0)
/* werechange: accepting randomly timed werecreature change to transform
   from human to creature or vice versa while having polymorph control */
#define ParanoidWerechange ((flags.paranoia_bits & PARANOID_WERECHANGE) != 0)
/* water: accepting entry into water or lava */
#define ParanoidWater ((flags.paranoia_bits & PARANOID_WATER) != 0)
/* trap: accepting entry into a trap */
#define ParanoidTrap ((flags.paranoia_bits & PARANOID_TRAP) != 0)
/* auto-select all: accepting auto-select all */
#define ParanoidAutoSelectAll ((flags.paranoia_bits & PARANOID_AUTOALL) != 0)

/* command parsing, mainly dealing with number_pad handling;
   not saved and restored */

#ifdef NHSTDC
/* forward declaration sufficient to declare pointers */
struct ext_func_tab; /* from func_tab.h */
#endif

/* special key functions */
enum nh_keyfunc {
    NHKF_ESC = 0,
    NHKF_DOAGAIN,
    NHKF_DOAGAIN2,

    NHKF_REQMENU,

    /* run ... clicklook need to be in a continuous block */
    NHKF_RUN,
    NHKF_RUN2,
    NHKF_RUSH,
    NHKF_FIGHT,
    NHKF_FIGHT2,
    NHKF_NOPICKUP,
    NHKF_RUN_NOPICKUP,
    NHKF_DOINV,
    NHKF_DOSEEWORN,
    NHKF_TRAVEL,
    NHKF_TRAVEL_ATTACK,
    NHKF_TRAVEL_WALK,
    NHKF_CLICKLOOK,

    NHKF_REDRAW,
    NHKF_REDRAW2,
    NHKF_GETDIR_SELF,
    NHKF_GETDIR_SELF2,
    NHKF_GETDIR_HELP,
    NHKF_COUNT,
    NHKF_GETPOS_SELF,
    NHKF_GETPOS_PICK,
    NHKF_GETPOS_PICK_Q,  /* quick */
    NHKF_GETPOS_PICK_O,  /* once */
    NHKF_GETPOS_PICK_V,  /* verbose */
    NHKF_GETPOS_SHOWVALID,
    NHKF_GETPOS_AUTODESC,
    NHKF_GETPOS_MON_NEXT,
    NHKF_GETPOS_MON_PREV,
    NHKF_GETPOS_OBJ_NEXT,
    NHKF_GETPOS_OBJ_PREV,
    NHKF_GETPOS_DOOR_NEXT,
    NHKF_GETPOS_DOOR_PREV,
    NHKF_GETPOS_UNEX_NEXT,
    NHKF_GETPOS_UNEX_PREV,
    NHKF_GETPOS_INTERESTING_NEXT,
    NHKF_GETPOS_INTERESTING_PREV,
    NHKF_GETPOS_VALID_NEXT,
    NHKF_GETPOS_VALID_PREV,
    NHKF_GETPOS_HELP,
    NHKF_GETPOS_MENU,
    NHKF_GETPOS_LIMITVIEW,
    NHKF_GETPOS_MOVESKIP,

    NUM_NHKF
};

enum gloctypes {
    GLOC_MONS = 0,
    GLOC_OBJS,
    GLOC_DOOR,
    GLOC_EXPLORE,
    GLOC_INTERESTING,
    GLOC_VALID,

    NUM_GLOCS
};

enum travel_modes {
    TRAVEL_MODE_NORMAL = 0,
    TRAVEL_MODE_ATTACK_AND_WALK,
    TRAVEL_MODE_WALK
};

/* commands[] is used to directly access cmdlist[] instead of looping
   through it to find the entry for a given input character;
   move_X is the character used for moving one step in direction X;
   alphadirchars corresponds to old sdir,
   dirchars corresponds to ``iflags.num_pad ? ndir : sdir'';
   pcHack_compat and phone_layout only matter when num_pad is on,
   swap_yz only matters when it's off */
struct cmd {
    unsigned serialno;     /* incremented after each update */
    boolean num_pad;       /* same as iflags.num_pad except during updates */
    boolean pcHack_compat; /* for numpad:  affects 5, M-5, and M-0 */
    boolean phone_layout;  /* inverted keypad:  1,2,3 above, 7,8,9 below */
    boolean swap_yz;       /* QWERTZ keyboards; use z to move NW, y to zap */
    boolean gnh_layout;    /* arrow keys for non-diagonal movement, yuhj for diagonal movement */
    char move_W, move_NW, move_N, move_NE, move_E, move_SE, move_S, move_SW;
    const char *dirchars;      /* current movement/direction characters */
    const char *alphadirchars; /* same as dirchars if !numpad */
    const struct ext_func_tab *commands[256]; /* indexed by input character */
    char spkeys[NUM_NHKF];
};

extern NEARDATA struct cmd Cmd;

#endif /* FLAG_H */

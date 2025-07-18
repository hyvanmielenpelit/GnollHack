/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    options.c    $NHDT-Date: 1554591224 2019/04/06 22:53:44 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.363 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2008. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifdef OPTION_LISTS_ONLY /* (AMIGA) external program for opt lists */
#include "config.h"
#include "objclass.h"
#include "flag.h"
NEARDATA struct flag flags; /* provide linkage */
#ifdef SYSFLAGS
NEARDATA struct sysflag sysflags; /* provide linkage */
#endif
NEARDATA struct instance_flags iflags; /* provide linkage */
#define static
#else
#include "hack.h"
#include "tcap.h"
#include <ctype.h>
#endif

#include <wchar.h>

#define BACKWARD_COMPAT

#ifdef DEFAULT_WC_TILED_MAP
#define PREFER_TILED TRUE
#else
#define PREFER_TILED FALSE
#endif

#ifdef CURSES_GRAPHICS
extern int curses_read_attrs(const char *attrs);
extern char *curses_fmt_attrs(char *);
#endif

enum window_option_types {
    MESSAGE_OPTION = 1,
    STATUS_OPTION,
    MAP_OPTION,
    MENU_OPTION,
    TEXT_OPTION
};

#define PILE_LIMIT_DFLT 5
#define HERE_WIN_SIZ_DFLT 8
#define DEFAULT_PILE_LIMIT (iflags.wc2_herewindow ? iflags.wc2_here_window_size + 1 : PILE_LIMIT_DFLT)
const char* mouse_cmd_names[MAX_CLICK_TYPES] = { "default", "by role", "?", "off", "look", "move", "cast", "fire", "zap" };

/*
 *  NOTE:  If you add (or delete) an option, please update the short
 *  options help (option_help()), the long options help (dat/opthelp),
 *  and the current options setting display function (doset()),
 *  and also the Guidebooks.
 *
 *  The order matters.  If an option is a an initial substring of another
 *  option (e.g. time and timed_delay) the shorter one must come first.
 */

static struct Bool_Opt {
    const char *name;
    const char* descr;
    boolean *addr, initvalue;
    int optflags;
} boolopt[] = {
    { "acoustics", "can your character hear anything", &flags.acoustics, TRUE, SET_IN_GAME },
#if defined(SYSFLAGS) && defined(AMIGA)
    /* Amiga altmeta causes Alt+key to be converted into Meta+key by
       low level GnollHack code; on by default, can be toggled off if
       Alt+key is needed for some ASCII chars on non-ASCII keyboard */
    { "altmeta", "treat \"ESC c\" as M-c (Meta+c, 8th bit set)", &sysflags.altmeta, TRUE, DISP_IN_GAME },
#else
#ifdef ALTMETA
    /* non-Amiga altmeta causes GnollHack's top level command loop to treat
       two character sequence "ESC c" as M-c, for terminals or emulators
       which send "ESC c" when Alt+c is pressed; off by default, enabling
       this can potentially make trouble if user types ESC when GnollHack
       is honoring this conversion request (primarily after starting a
       count prefix prior to a command and then deciding to cancel it) */
    { "altmeta", "treat \"ESC c\" as M-c (Meta+c, 8th bit set)", &iflags.altmeta, FALSE, SET_IN_GAME },
#else
    { "altmeta", "treat \"ESC c\" as M-c (Meta+c, 8th bit set)", (boolean *) 0, TRUE, DISP_IN_GAME },
#endif
#endif
    { "ascii_map", "show map as text", &iflags.wc_ascii_map, !PREFER_TILED, SET_IN_GAME }, /*WC*/
#if defined(SYSFLAGS) && defined(MFLOPPY)
    { "asksavedisk", "prompt for saving to a disk", &sysflags.asksavedisk, FALSE, SET_IN_GAME},
#else
    { "asksavedisk", "prompt for saving to a disk", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "autodescribe", "describe terrain under cursor", &iflags.autodescribe, TRUE, SET_IN_GAME },
    { "autodig", "dig if moving and wielding a digging tool", &flags.autodig, 
#ifdef GNH_MOBILE
    TRUE,
#else
    FALSE,
#endif
    SET_IN_GAME },
#ifdef ANDROID
    {"autokick", "walking into a door attempts to kick it", &flags.autokick, TRUE, SET_IN_GAME},
#endif
    { "autoopen", "walking into a door attempts to open it", &flags.autoopen, TRUE, SET_IN_GAME },
    { "autopickup", "automatically pick up objects", &flags.pickup, FALSE, SET_IN_GAME },
    { "autoquiver", "fill empty quiver automatically when firing", &flags.autoquiver, FALSE, SET_IN_GAME },
    { "autostatuslines", "adjust the number of status lines automatically", &iflags.wc2_autostatuslines, FALSE, SET_IN_FILE },
    { "autoswap_launchers", "automatically swap launchers on for ranged attacks and off for melee attacks", &iflags.autoswap_launchers, TRUE, SET_IN_GAME },
    { "autounlock", "automatically unlock a locked door or chest", &flags.autounlock, TRUE, SET_IN_GAME },
    { "baseacasbonus", "display base armor class as a bonus rather than a number starting at 10", &flags.baseacasbonus, TRUE, SET_IN_GAME },
#if defined(MICRO) && !defined(AMIGA)
    { "BIOS", "use IBM ROM BIOS calls", &iflags.BIOS, FALSE, SET_IN_FILE },
#else
    { "BIOS", "use IBM ROM BIOS calls", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "blind", "your character is permanently blind", &u.uroleplay.blind, FALSE, DISP_IN_GAME },
    { "blinking_cursor_on_tiles", "display a blinking cursor when using tiles", &flags.blinking_cursor_on_tiles, FALSE, SET_IN_GAME },
    { "bones", "allow loading bones files", &flags.bones, TRUE, SET_IN_FILE },
#ifdef INSURANCE
    { "checkpoint", "save game state after each level change", &flags.ins_chkpt, TRUE, SET_IN_GAME },
#else
    { "checkpoint", "save game state after each level change", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
#ifdef MFLOPPY
    { "checkspace", "check disk space on a floppy disk", &iflags.checkspace, TRUE, SET_IN_GAME },
#else
    { "checkspace", "check disk space on a floppy disk", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "classic_statue_symbol", "use classic symbol \'`\' for statues", &flags.classic_statue_symbol, FALSE, SET_IN_GAME},
    { "classic_colors", "use classic colors for boulders and floor symbols", &flags.classic_colors, FALSE, SET_IN_GAME},
    { "clickfire", "enable firing via mouse click", &iflags.clickfire, TRUE, SET_IN_GAME},
    { "clicklook", "enable looking via mouse click", &iflags.clicklook, TRUE, SET_IN_GAME},
    { "cmdassist", "give help for errors on direction input", &iflags.cmdassist, TRUE, SET_IN_GAME},
#if defined(MICRO) || defined(WIN32) || defined(ANDROID) ||  defined(GNH_MOBILE) || defined(CURSES_GRAPHICS)
    { "color", "use color in map", &iflags.wc_color, TRUE, SET_IN_GAME }, /*WC*/
#else /* systems that support multiple terminals, many monochrome */
    { "color", "use color in map", &iflags.wc_color, FALSE, SET_IN_GAME }, /*WC*/
#endif
    { "confirm", "ask before hitting tame or peaceful monsters", &flags.confirm, TRUE, SET_IN_GAME },
    { "dark_room", "show floor outside line of sight differently", &flags.dark_room, TRUE, SET_IN_GAME },
    { "detailed_weights", "show object weights using more accurate units", &flags.detailed_weights, FALSE, SET_IN_GAME },
    { "displace_peaceful", "walking into a peaceful monster attempts to displace it", &flags.displace_peaceful, TRUE, SET_IN_GAME},
#ifdef ANDROID
    {"dumplog", "print dumplogs", &iflags.dumplog, FALSE, SET_IN_FILE},
#endif
    { "eight_bit_tty", "send 8-bit characters directly to terminal", &iflags.wc_eight_bit_input, FALSE, SET_IN_GAME }, /*WC*/
#if defined(UNIX) && !defined(GNH_MOBILE) 
    { "enablettyarrowkeys", "enable tty arrow keys", &iflags.enablettyarrowkeys, FALSE, SET_IN_GAME},
#endif
    { "exchange_prompt", "prompt when exchanging a worn object for another", &flags.exchange_prompt, TRUE, SET_IN_GAME},
#if defined(TTY_GRAPHICS) || defined(CURSES_GRAPHICS) || defined(X11_GRAPHICS)
    { "extmenu", "use menu for getting extended commands", &iflags.extmenu, FALSE, SET_IN_GAME },
#elif defined(ANDROID)
    { "extmenu", "use menu for getting extended commands" &iflags.extmenu, TRUE, SET_IN_GAME },
#else
    { "extmenu", "use menu for getting extended commands", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
#ifdef OPT_DISPMAP
    { "fast_map", "use optimized, less flexible map display", &flags.fast_map, TRUE, SET_IN_GAME},
#else
    { "fast_map", "use optimized, less flexible map display", (boolean *) 0, TRUE, SET_IN_FILE },
#endif
    { "female", "is your character female", &flags.female, FALSE, DISP_IN_GAME},
    { "fixinv", "inventory items keep their letters", &flags.invlet_constant, TRUE, SET_IN_GAME },
#if defined(SYSFLAGS) && defined(AMIFLUSH)
    { "flush", "use flush on Amiga", &sysflags.amiflush, FALSE, SET_IN_GAME},
#else
    { "flush", "use flush on Amiga", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
#ifdef ANDROID
    { "force_invmenu", "commands asking for inventory item show a menu", &iflags.force_invmenu, TRUE, SET_IN_GAME },
#else
    { "force_invmenu", "commands asking for inventory item show a menu", &iflags.force_invmenu, FALSE, SET_IN_GAME },
#endif
    { "force_hint", "force giving hints regardless of other settings", &flags.force_hint, FALSE, SET_IN_GAME },
    { "fullscreen", "toggle fullscreen", &iflags.wc2_fullscreen, FALSE, SET_IN_FILE }, /*WC2*/
    { "fullstatuslineorder", "use full status line order", &flags.fullstatuslineorder, TRUE, SET_IN_GAME },
    { "goldX", "classify gold as unknown or uncursed", &iflags.goldX, FALSE, SET_IN_GAME },
    { "guicolor",  "use color for UI", &iflags.wc2_guicolor, TRUE, SET_IN_GAME}, /*WC2*/
    { "help",  "show all available info when using whatis-command", &flags.help, TRUE, SET_IN_GAME },
    { "herecmd_menu", "show commands available in this location", &iflags.herecmd_menu, FALSE, SET_IN_GAME },
    { "herewindow", "show here window", &iflags.wc2_herewindow, TRUE, SET_IN_FILE }, /*WC2*/
    { "hilite_pet", "use highlight for pets", &iflags.wc_hilite_pet, FALSE, SET_IN_GAME }, /*WC*/
    { "hilite_pile", "highlight piles of items", &iflags.hilite_pile, FALSE, SET_IN_GAME },
    { "hitpointbar", "show colored bar for hit points", &iflags.wc2_hitpointbar, FALSE, SET_IN_GAME }, /*WC2*/
    { "ibm2utf8", "convert CP437 symbols to UTF8 format before output", &flags.ibm2utf8, FALSE, SET_IN_GAME },
#ifndef MAC
    { "ignintr", "ignore interrupt signals", &flags.ignintr, FALSE, SET_IN_GAME },
#else
    { "ignintr", "ignore interrupt signals", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "ignore_stopping", "travelling is not interrupted by items, doors, or engravings", &flags.ignore_stopping, TRUE, SET_IN_GAME },
    { "implicit_uncursed", "omit \"uncursed\" from inventory", &iflags.implicit_uncursed, TRUE, SET_IN_GAME },
    { "inventory_obj_cmd", "display a command menu upon selecting an object in inventory", &flags.inventory_obj_cmd, TRUE, SET_IN_GAME},
    { "inventory_weights_last", "display object weights in parentheses after object name", &flags.inventory_weights_last, FALSE, SET_IN_GAME},
    { "knapsack_prompt", "prompt for an action when inventory is full", &flags.knapsack_prompt, TRUE, SET_IN_GAME},
    { "large_font", "obsolete: use large font", &iflags.obsolete, FALSE, SET_IN_FILE}, /* OBSOLETE */
    { "legacy", "show introductory message", &flags.legacy, TRUE, DISP_IN_GAME },
    { "lit_corridor", "show dark corridors as lit if in sight", &flags.lit_corridor, FALSE, SET_IN_GAME },
    { "long_charge_text", "long format for charges and rechargings in inventory", &flags.long_charge_text, FALSE, SET_IN_GAME},
    { "lootabc", "use a/b/c rather than o/i/c when looting", &flags.lootabc, FALSE, SET_IN_GAME },
#ifdef MAIL
    { "mail", "enable the mail daemon", &flags.biff, TRUE, SET_IN_GAME },
#else
    { "mail", "enable the mail daemon", (boolean *) 0, TRUE, SET_IN_FILE },
#endif
    { "mention_walls", "give feedback when walking into walls", &iflags.mention_walls, FALSE, SET_IN_GAME },
#if defined(ANDROID) || defined(GNH_MOBILE)
    { "menucolors", "use colors in menus", &iflags.use_menu_color, TRUE, SET_IN_GAME },
#else
    { "menucolors", "use colors in menus", &iflags.use_menu_color, FALSE, SET_IN_GAME },
#endif
    /* for menu debugging only*/
    { "menu_tab_sep", "menu formatting", &iflags.menu_tab_sep, FALSE, SET_IN_WIZGAME },
    { "menu_objsyms", "show object symbols in menus", &iflags.menu_head_objsym, FALSE, SET_IN_GAME },
#ifdef TTY_GRAPHICS
    { "menu_overlay", "menus overlay and align to right", &iflags.menu_overlay, TRUE, SET_IN_GAME },
#else
    { "menu_overlay", "menus overlay and align to right", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "metric_system", "use the metric system for weights", &flags.metric_system, FALSE, SET_IN_GAME },
    { "monpolycontrol", "control monster polymorphs", &iflags.mon_polycontrol, FALSE, SET_IN_WIZGAME },
#ifdef NEWS
    { "news", "show any news at game start", &iflags.news, TRUE, DISP_IN_GAME },
#else
    { "news", "show any news at game start", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "nudist", "start your character without armor", &u.uroleplay.nudist, FALSE, DISP_IN_GAME },
    { "null", "allow nulls to be sent to terminal", &flags.null, TRUE, SET_IN_GAME },
#if defined(SYSFLAGS) && defined(MAC)
    { "page_wait", "page wait on Mac", &sysflags.page_wait, TRUE, SET_IN_GAME},
#else
    { "page_wait", "page wait on Mac", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "partydetails", "give detailed information for each pet", &flags.partydetails, FALSE, SET_IN_GAME },
    { "partylinecolor", "use colors for pet statistics", &flags.partylinecolor, TRUE, SET_IN_GAME },
    { "partymultiline", "print statistics of each pet on a separate status line", &flags.partymultiline, FALSE, SET_IN_GAME },
    /* 3.6.2: move perm_invent from flags to iflags and out of save file */
    { "perm_invent", "show permanent inventory window", &iflags.perm_invent, FALSE, SET_IN_GAME },
    { "pickup_thrown", "autopickup thrown items", &flags.pickup_thrown, TRUE, SET_IN_GAME },
    { "popup_dialog", "use popup dialog", &iflags.wc_popup_dialog, FALSE, SET_IN_GAME},   /*WC*/
    { "prefer_fast_move", "swap slow move and fast move commands' key bindings", &flags.prefer_fast_move, FALSE, SET_IN_GAME},
    { "preload_tiles", "preload tiles", &iflags.wc_preload_tiles, TRUE, DISP_IN_GAME}, /*WC*/
    { "pushweapon", "previous weapon goes to secondary slot", &flags.pushweapon, FALSE, SET_IN_GAME },
#if defined(MICRO) && !defined(AMIGA)
    { "rawio", "allow use to raw I/O", &iflags.rawio, FALSE, DISP_IN_GAME },
#else
    { "rawio", "allow use to raw I/O", (boolean *) 0, FALSE, SET_IN_FILE },
#endif
    { "rest_on_space", "space bar is bound to the rest-command", &flags.rest_on_space, FALSE, SET_IN_GAME },
#ifdef RLECOMP
    { "rlecomp", "use rlecomp", &iflags.rlecomp,
#if defined(COMPRESS) || defined(ZLIB_COMP)
      FALSE,
#else
      TRUE,
#endif
      DISP_IN_GAME },
#endif
    { "safe_pet", "prevent you from hitting pets", &flags.safe_dog, TRUE, SET_IN_GAME },
    { "sanity_check", "perform data sanity checks", &iflags.sanity_check, 
#ifdef DEBUG
        TRUE,  /* Set to false in set_playmode if not in wizard mode */
#else
        FALSE,
#endif
        SET_IN_WIZGAME },
    { "search_box_traps", "search command searches boxes for traps first", &flags.search_box_traps, TRUE, SET_IN_GAME },
    { "selectsaved", "select a saved game at program start", &iflags.wc2_selectsaved, TRUE, DISP_IN_GAME}, /*WC*/
    { "self_click_action", "clicking the player character executes an action", &flags.self_click_action, TRUE, SET_IN_GAME},
    { "showexp", "show experience points in status line", &flags.showexp, TRUE, SET_IN_GAME},
    { "showmove", "show current movement speed in status line", &flags.showmove, TRUE, SET_IN_GAME },
    { "showrace", "show your character by race rather than role", &flags.showrace, FALSE, SET_IN_GAME },
    { "showrealtime", "show elapsed wall-clock time in status line", &flags.showrealtime, TRUE, SET_IN_GAME },
    { "showscore", "show current score in status line", &flags.showscore, TRUE, SET_IN_GAME },
    { "show_buff_timer", "show buff timer on tiles", &flags.show_buff_timer, FALSE, SET_IN_GAME},
    { "show_comparison_stats", "show comparison statistics for items when picking them up", &iflags.show_comparison_stats, TRUE, SET_IN_GAME },
    { "show_decorations", "show decorations via colors in ASCII mode", &flags.show_decorations, TRUE, SET_IN_GAME },
    { "show_dice_as_ranges", "show dice as ranges (e.g., 2-12 instead of 2d6)", &iflags.show_dice_as_ranges,
#ifdef GNH_MOBILE
        TRUE,
#else
        FALSE,
#endif
        SET_IN_GAME },
    { "show_grid", "show grid between tiles", &flags.show_grid, FALSE, SET_IN_GAME},
    { "show_tile_mon_hp_bar", "show monster hit points on tiles", &flags.show_tile_mon_hp_bar, FALSE, SET_IN_GAME},
    { "show_tile_pet_hp_bar", "show pet hit points on tiles", &flags.show_tile_pet_hp_bar, FALSE, SET_IN_GAME },
    { "show_tile_u_hp_bar", "show player hit points on tiles", &flags.show_tile_u_hp_bar, FALSE, SET_IN_GAME },
    { "show_weapon_style", "show used weapon type in status line", &flags.show_weapon_style, TRUE, SET_IN_GAME },
    { "show_weight_summary", "show total weight at the end of inventory", &flags.show_weight_summary, TRUE, SET_IN_GAME },
    { "silent", "don't use terminal bell", &flags.silent, TRUE, SET_IN_GAME },
    { "skill_table_format", "show skills in a table format rather than a list",  &iflags.skill_table_format, TRUE, SET_IN_GAME},
    { "softkeyboard", "soft keyboard", &iflags.wc2_softkeyboard, FALSE, SET_IN_FILE}, /*WC2*/
    { "sortpack", "group inventory items by type", &flags.sortpack, TRUE, SET_IN_GAME },
    { "sparkle", "display sparkly effect when resisting magic", &flags.sparkle, TRUE, SET_IN_GAME },
    { "spell_table_format", "show spells in a table format rather than a list", &iflags.spell_table_format, TRUE, SET_IN_GAME },
    { "splash_screen", "show splash screen", &iflags.wc_splash_screen, TRUE, DISP_IN_GAME}, /*WC*/
    { "standout", "use standout for --more--", &flags.standout, FALSE, SET_IN_GAME },
    { "stash_on_autopickup", "stash items into a container on autopickup (but no thrown if pick_thrown is on)", &flags.stash_on_autopickup, FALSE, SET_IN_GAME },
    { "status_updates", "allow the status lines to update", &iflags.status_updates, TRUE, DISP_IN_GAME },
    { "swap_rhand_only", "swap right hand weapon only rather than objects in both hands", &flags.swap_rhand_only, FALSE, SET_IN_GAME},
    { "takeoff_uses_all", "takeoff command uses takeoffall command rather than normal implementation", &iflags.takeoff_uses_all, TRUE, SET_IN_GAME},
    { "tellexp", "report experience points gained", &flags.tellexp, TRUE, SET_IN_GAME },
    { "tiled_map", "tiled map", &iflags.wc_tiled_map, PREFER_TILED, DISP_IN_GAME}, /*WC*/
    { "time", "display game turns in status line", &flags.time, FALSE, SET_IN_GAME },
#ifdef TIMED_DELAY
    { "timed_delay", "use time delay", &flags.nap, TRUE, SET_IN_GAME },
#else
    { "timed_delay", "use time delay", (boolean *) 0, FALSE, SET_IN_GAME },
#endif
    { "tombstone", "show tombstone when your character dies", &flags.tombstone, TRUE, SET_IN_GAME },
    { "toptenwin", "show top scores in window", &iflags.toptenwin, FALSE, SET_IN_GAME },
    { "travel", "enable traveling via mouse click", &flags.travelcmd, TRUE, SET_IN_GAME },
#ifdef DEBUG
    { "travel_debug", "display debugging graphics for travel algorithm", &iflags.trav_debug, FALSE, SET_IN_WIZGAME}, /*hack.c*/
#endif
    { "underline_peaceful", "underline peaceful monsters", &flags.underline_peaceful, TRUE, SET_IN_GAME},
    { "use_darkgray",  "use bold black color instead of blue", &iflags.wc2_darkgray, TRUE, SET_IN_FILE }, /*WC2*/
#if defined(WIN32) || defined(ANDROID)
    { "use_inverse",  "display detected monsters in inverse", &iflags.wc_inverse, TRUE, SET_IN_GAME }, /*WC*/
#else
    { "use_inverse",  "display detected monsters in inverse", &iflags.wc_inverse, FALSE, SET_IN_GAME }, /*WC*/
#endif
    { "verbose", "enable longer messages", &flags.verbose, TRUE, SET_IN_GAME },
#ifdef TTY_TILES_ESCCODES
    { "vt_tiledata", (char*)0, &iflags.vt_tiledata, FALSE, SET_IN_FILE },
#else
    { "vt_tiledata", "use VT codes for tiles", (boolean*)0, FALSE, SET_IN_FILE},
#endif
    { "whatis_menu",  "show menu when getting a map location", &iflags.getloc_usemenu, FALSE, SET_IN_GAME },
    { "whatis_moveskip", "skip same glyph when getting map location", &iflags.getloc_moveskip, FALSE, SET_IN_GAME },
    { "wiz_alwaysenc", "always generate an encounter in wizard mode", &flags.wiz_alwaysenc, FALSE, SET_IN_WIZGAME},
    { "wiz_mstatusline", "enable extended monster status line in wizard mode", &flags.wiz_mstatusline, FALSE, SET_IN_WIZGAME},
    { "wizweight", "show weights in inventory in wizard mode", &iflags.wizweight, FALSE, SET_IN_WIZGAME},
    { "wraptext", "wrap text", &iflags.wc2_wraptext, FALSE, SET_IN_GAME}, /*WC2*/
#ifdef ZEROCOMP
    { "zerocomp", "use zerocomp", &iflags.zerocomp,
#if defined(COMPRESS) || defined(ZLIB_COMP)
      FALSE,
#else
      TRUE,
#endif
      DISP_IN_GAME },
#endif
    { (char *) 0, (char*) 0, (boolean *) 0, FALSE, 0 }
};

/* compound options, for option_help() and external programs like Amiga
 * frontend */
static struct Comp_Opt {
    const char *name, *descr;
    int size; /* for frontends and such allocating space --
               * usually allowed size of data in game, but
               * occasionally maximum reasonable size for
               * typing when game maintains information in
               * a different format */
    int optflags;
} compopt[] = {
    { "animation_interval", "animation frame interval in milliseconds", 3, SET_IN_GAME },  /*WC2*/
    { "align", "your starting alignment (lawful, neutral, or chaotic)", 8,
      DISP_IN_GAME },
    { "align_message", "message window alignment", 20, DISP_IN_GAME }, /*WC*/
    { "align_status", "status window alignment", 20, DISP_IN_GAME },   /*WC*/
    { "altkeyhandler", "alternate key handler", 20, SET_IN_GAME },
#ifdef BACKWARD_COMPAT
    { "boulder", "deprecated (use S_boulder in sym file instead)", 1,
      SET_IN_GAME },
#endif
    { "catbreed", "the breed of your (first) cat (e.g., catbreed:white persian)", 32, DISP_IN_GAME },
    { "catgender", "the gender of your (first) cat (e.g., catgender:female)", 7, DISP_IN_GAME },
    { "catname", "the name of your (first) cat (e.g., catname:Tabby)",
      PL_PSIZ, DISP_IN_GAME },
    { "crawl_interval", "crawl movement interval in milliseconds", 3, SET_IN_GAME },
    { "disclose", "the kinds of information to disclose at end of game",
      sizeof flags.end_disclose * 2, SET_IN_GAME },
    { "dogbreed", "the breed of your (first) dog (e.g., dogbreed:black labrador)", 32, DISP_IN_GAME },
    { "doggender", "the gender of your (first) dog (e.g., doggender:male)", 7, DISP_IN_GAME },
    { "dogname", "the name of your (first) dog (e.g., dogname:Fang)", PL_PSIZ,
      DISP_IN_GAME },
    { "dungeon", "the symbols to use in drawing the dungeon map",
      MAX_DUNGEON_CHARS + 1, SET_IN_FILE },
    { "effects", "the symbols to use in drawing special effects",
      MAX_EFFECT_CHARS + 1, SET_IN_FILE },
    { "font_map", "the font to use in the map window", 40,
      DISP_IN_GAME },                                              /*WC*/
    { "font_menu", "the font to use in menus", 40, DISP_IN_GAME }, /*WC*/
    { "font_message", "the font to use in the message window", 40,
      DISP_IN_GAME },                                                  /*WC*/
    { "font_size_map", "the size of the map font", 20, DISP_IN_GAME }, /*WC*/
    { "font_size_menu", "the size of the menu font", 20,
      DISP_IN_GAME }, /*WC*/
    { "font_size_message", "the size of the message font", 20,
      DISP_IN_GAME }, /*WC*/
    { "font_size_status", "the size of the status font", 20,
      DISP_IN_GAME }, /*WC*/
    { "font_size_text", "the size of the text font", 20,
      DISP_IN_GAME }, /*WC*/
    { "font_status", "the font to use in status window", 40,
      DISP_IN_GAME }, /*WC*/
    { "font_text", "the font to use in text windows", 40,
      DISP_IN_GAME }, /*WC*/
    { "fruit", "the name of a fruit you enjoy eating", PL_FSIZ, SET_IN_GAME },
    { "gender", "your starting gender (male or female)", 8, DISP_IN_GAME },
    { "here_window_size",
      "number of rows in the here window",
      20, SET_IN_GAME
    }, /*WC2*/
    { "horsegender", "the gender of your (first) horse (e.g., horsegender:female)", 7, DISP_IN_GAME },
    { "horsename", "the name of your (first) horse (e.g., horsename:Silver)",
      PL_PSIZ, DISP_IN_GAME },
    { "last_item_show_duration", "duration for showing last item in context menu", 3,
      SET_IN_GAME },
    { "luggagename", "the name of your (first) luggage (e.g., luggagename:Albert)",
      PL_PSIZ, DISP_IN_GAME },
    { "map_mode", "map display mode under Windows", 20, DISP_IN_GAME }, /*WC*/
    { "max_hint_difficulty", "maximum difficulty level for showing hints", 3,
      DISP_IN_GAME },
    { "menustyle", "user interface for object selection", MENUTYPELEN,
      SET_IN_GAME },
    { "menu_deselect_all", "deselect all items in a menu", 4, SET_IN_FILE },
    { "menu_deselect_page", "deselect all items on this page of a menu", 4,
      SET_IN_FILE },
    { "menu_first_page", "jump to the first page in a menu", 4, SET_IN_FILE },
    { "menu_headings", "text attribute for menu headings", 9, SET_IN_GAME },
    { "menu_invert_all", "invert all items in a menu", 4, SET_IN_FILE },
    { "menu_invert_page", "invert all items on this page of a menu", 4,
      SET_IN_FILE },
    { "menu_last_page", "jump to the last page in a menu", 4, SET_IN_FILE },
    { "menu_next_page", "goto the next menu page", 4, SET_IN_FILE },
    { "menu_previous_page", "goto the previous menu page", 4, SET_IN_FILE },
    { "menu_search", "search for a menu item", 4, SET_IN_FILE },
    { "menu_select_all", "select all items in a menu", 4, SET_IN_FILE },
    { "menu_select_page", "select all items on this page of a menu", 4,
      SET_IN_FILE },
    { "middle_click_command", "command upon clicking middle mouse button", 32, SET_IN_GAME },
    { "monsters", "the symbols to use for monsters", MAX_MONSTER_CLASSES,
      SET_IN_FILE },
    { "move_interval", "normal movement interval in milliseconds", 3, SET_IN_GAME },
    { "msghistory", "number of top line messages to save", 5, DISP_IN_GAME },
#if defined(TTY_GRAPHICS) || defined(CURSES_GRAPHICS)
    { "msg_window", "the type of message window required", 1, SET_IN_GAME },
#else
    { "msg_window", "the type of message window required", 1, SET_IN_FILE },
#endif
    { "name", "your character's name (e.g., name:Merlin-W)", PL_NSIZ,
      DISP_IN_GAME },
    { "mouse_support", "game receives click info from mouse", 0, SET_IN_GAME },
    { "number_pad", "use the number pad for movement", 1, SET_IN_GAME },
    { "objects", "the symbols to use for objects", MAX_OBJECT_CLASSES, SET_IN_FILE },
    { "packorder", "the inventory order of the items in your pack",
      MAX_OBJECT_CLASSES, SET_IN_GAME },
#ifdef CHANGE_COLOR
    { "palette",
#ifndef WIN32
      "palette (00c/880/-fff is blue/yellow/reverse white)", 15, SET_IN_GAME
#else
      "palette (adjust an RGB color in palette (color-R-G-B)", 15, SET_IN_FILE
#endif
    },
#if defined(MAC)
    { "hicolor", "same as palette, only order is reversed", 15, SET_IN_FILE },
#endif
#endif
    { "paranoid_confirmation", "extra prompting in certain situations", 28,
      SET_IN_GAME },
    { "petattr",  "attributes for highlighting pets", 88, SET_IN_GAME },
    { "pettype", "your preferred initial pet type", 4, DISP_IN_GAME },
    { "pickup_burden", "maximum burden picked up before prompt", 20,
      SET_IN_GAME },
    { "pickup_types", "types of objects to pick up automatically",
      MAX_OBJECT_CLASSES, SET_IN_GAME },
    { "pile_limit", "threshold for \"there are many objects here\"", 24,
      SET_IN_GAME },
    { "playmode", "classic or modern play, non-scoring explore or casual mode, or debug mode", 8,
      DISP_IN_GAME },
    { "player_selection", "choose character via dialog or prompts", 12,
      DISP_IN_GAME },
#ifdef USE_TILES
    { "preferred_screen_scale", "preferred screen scale", 3,
      SET_IN_GAME },
#endif
    { "race", "your starting race (e.g., Human, Elf)", PL_CSIZ,
      DISP_IN_GAME },
    { "ramgender", "the gender of your (first) ram (e.g., ramgender:male)", 7, DISP_IN_GAME },
    { "ramname", "the name of your (first) ram (e.g., ramname:Silver)",
      PL_PSIZ, DISP_IN_GAME },
    { "right_click_command", "command upon clicking right mouse button", 32, SET_IN_GAME },
    { "role", "your starting role (e.g., Barbarian, Valkyrie)", PL_CSIZ,
      DISP_IN_GAME },
    { "runmode", "display frequency when `running' or `travelling'",
      sizeof "teleport", SET_IN_GAME },
    { "run_spot_distance", "longest distance for spotting a monster to stop running or travelling", 3,
      SET_IN_GAME },
    { "scores", "the parts of the score list you wish to see", 32,
      SET_IN_GAME },
    { "scroll_amount", "amount to scroll map when scroll_margin is reached",
      20, DISP_IN_GAME }, /*WC*/
    { "scroll_margin", "scroll map when this far from the edge", 20,
      DISP_IN_GAME }, /*WC*/
    { "shield_effect_length", "shield effect length", 3,
      SET_IN_GAME },
    { "sortloot", "sort object selection lists by description", 4,
      SET_IN_GAME },
    { "sound_volume_ambient", "ambient volume", 3,
      SET_IN_GAME }, /*WC2*/
    { "sound_volume_dialogue", "dialogue volume", 3,
      SET_IN_GAME }, /*WC2*/
    { "sound_volume_effects", "sound effect volume", 3,
      SET_IN_GAME }, /*WC2*/
    { "sound_volume_general", "general game volume", 3,
      SET_IN_GAME }, /*WC2*/
    { "sound_volume_music", "music volume", 3,
      SET_IN_GAME }, /*WC2*/
    { "sound_volume_ui", "user interface sound volume", 3,
      SET_IN_GAME }, /*WC2*/
    { "spellorder", "default spell sorting", 3,
      SET_IN_GAME },
#ifdef MSDOS
    { "soundcard", "type of sound card to use", 20, SET_IN_FILE },
#endif
    { "statushilites",
#ifdef STATUS_HILITES
      "0=no status highlighting, N=show highlights for N turns",
      20, SET_IN_GAME
#else
    "highlight control", 20, SET_IN_FILE
#endif
    },
    { "statuslines",
#if defined(CURSES_GRAPHICS) || defined(MSWIN_GRAPHICS) || defined(NUKLEAR_GRAPHICS) || defined(DLL_GRAPHICS) || defined(LIB_GRAPHICS)
      "2 to 8 lines for horizonal (bottom or top) status display",
      20, SET_IN_GAME
#else
      "2 to 8 lines for status display",
      20, SET_IN_FILE
#endif
    }, /*WC2*/
    { "symset", "load a set of display symbols from the symbols file", 70,
      SET_IN_GAME },
    { "roguesymset",
      "load a set of rogue display symbols from the symbols file", 70,
      SET_IN_GAME },
#ifdef WIN32
    { "subkeyvalue", "override keystroke value", 7, SET_IN_FILE },
#endif
    { "suppress_alert", "suppress alerts about version-specific features", 8,
      SET_IN_GAME },
    { "talk_effect_length", "talk effect length", 3,
      SET_IN_GAME },
    /* term_cols,term_rows -> WC2_TERM_SIZE (6: room to format 1..32767) */
    { "term_cols", "number of columns", 6, SET_IN_FILE }, /*WC2*/
    { "term_rows", "number of rows", 6, SET_IN_FILE }, /*WC2*/
    { "tile_width", "width of tiles", 20, DISP_IN_GAME },   /*WC*/
    { "tile_height", "height of tiles", 20, DISP_IN_GAME }, /*WC*/
    { "tile_file", "name of tile file", 70, DISP_IN_GAME }, /*WC*/
    { "traps", "the symbols to use in drawing traps", MAX_TRAP_CHARS + 1,
      SET_IN_FILE },
    { "vary_msgcount", "show more old messages at a time", 20,
      DISP_IN_GAME }, /*WC*/
#ifdef MSDOS
    { "video", "method of video updating", 20, SET_IN_FILE },
#endif
#ifdef VIDEOSHADES
    { "videocolors", "color mappings for internal screen routines", 40,
      DISP_IN_GAME },
    { "videoshades", "gray shades to map to black/gray/white", 32,
      DISP_IN_GAME },
#endif
    { "whatis_coord", "show coordinates when auto-describing cursor position",
      1, SET_IN_GAME },
    { "whatis_filter",
      "filter coordinate locations when targeting next or previous",
      1, SET_IN_GAME },
    { "windowborders", "0 (off), 1 (on), 2 (auto)", 9, SET_IN_GAME }, /*WC2*/
    { "windowcolors", "the foreground/background colors of windows", /*WC*/
      80, DISP_IN_GAME },
    { "windowtype", "windowing system to use", WINTYPELEN, DISP_IN_GAME },
#ifdef WINCHAIN
    { "windowchain", "window processor to use", WINTYPELEN, SET_IN_SYS },
#endif
    { "wolfgender", "the gender of your (first) direwolf (e.g., wolfgender:female)", 7, DISP_IN_GAME },
    { "wolfname", "the name of your (first) direwolf (e.g., wolfname:Shadow)",
        PL_PSIZ, DISP_IN_GAME },
#ifdef BACKWARD_COMPAT
    { "DECgraphics", "load DECGraphics display symbols", 70, SET_IN_FILE },
    { "IBMgraphics", "load IBMGraphics display symbols", 70, SET_IN_FILE },
#ifdef CURSES_GRAPHICS
    { "cursesgraphics", "load curses display symbols", 70, SET_IN_FILE },
#endif
#ifdef MAC_GRAPHICS_ENV
    { "Macgraphics", "load MACGraphics display symbols", 70, SET_IN_FILE },
#endif
#endif
    { (char *) 0, (char *) 0, 0, 0 }
};

#ifdef OPTION_LISTS_ONLY
#undef static

#else /* use rest of file */

extern char configfile[]; /* for messages */

extern struct symparse loadsyms[];
static boolean need_redraw; /* for doset() */
static boolean need_set_sound_volume; /* for doset() */
static boolean need_update_inventory; /* for doset() */
static boolean need_set_animation_timer_interval;
static boolean need_update_space_binding;
static boolean need_status_initialize;
static boolean need_issue_gui_command;
static boolean need_stretch_map;
static boolean need_here_window;

#if defined(TOS) && defined(TEXTCOLOR)
extern boolean colors_changed;  /* in tos.c */
#endif

#ifdef VIDEOSHADES
extern char *shade[3];          /* in sys/msdos/video.c */
extern char ttycolors[CLR_MAX]; /* in sys/msdos/video.c */
#endif

static char def_inv_order[MAX_OBJECT_CLASSES] = {
    COIN_CLASS, AMULET_CLASS, WEAPON_CLASS, ARMOR_CLASS, MISCELLANEOUS_CLASS, RING_CLASS, 
    FOOD_CLASS, SCROLL_CLASS, SPBOOK_CLASS, POTION_CLASS, WAND_CLASS,
    TOOL_CLASS, REAGENT_CLASS, GEM_CLASS, ART_CLASS, ROCK_CLASS, BALL_CLASS, CHAIN_CLASS, 0,
};

/*
 * Default menu manipulation command accelerators.  These may _not_ be:
 *
 *      + a number - reserved for counts
 *      + an upper or lower case US ASCII letter - used for accelerators
 *      + ESC - reserved for escaping the menu
 *      + NULL, CR or LF - reserved for commiting the selection(s).  NULL
 *        is kind of odd, but the tty's xwaitforspace() will return it if
 *        someone hits a <ret>.
 *      + a default object class symbol - used for object class accelerators
 *
 * Standard letters (for now) are:
 *
 *              <  back 1 page
 *              >  forward 1 page
 *              ^  first page
 *              |  last page
 *              :  search
 *
 *              page            all
 *               ,    select     .
 *               \    deselect   -
 *               ~    invert     @
 *
 * The command name list is duplicated in the compopt array.
 */
typedef struct {
    const char *name;
    char cmd;
    const char *desc;
} menu_cmd_t;

static const menu_cmd_t default_menu_cmd_info[] = {
 { "menu_first_page", MENU_FIRST_PAGE, "Go to first page" },
 { "menu_last_page", MENU_LAST_PAGE, "Go to last page" },
 { "menu_next_page", MENU_NEXT_PAGE, "Go to next page" },
 { "menu_previous_page", MENU_PREVIOUS_PAGE, "Go to previous page" },
 { "menu_select_all", MENU_SELECT_ALL, "Select all items" },
 { "menu_deselect_all", MENU_UNSELECT_ALL, "Unselect all items" },
 { "menu_invert_all", MENU_INVERT_ALL, "Invert selection" },
 { "menu_select_page", MENU_SELECT_PAGE, "Select items in current page" },
 { "menu_deselect_page", MENU_UNSELECT_PAGE,
   "Unselect items in current page" },
 { "menu_invert_page", MENU_INVERT_PAGE, "Invert current page selection" },
 { "menu_search", MENU_SEARCH, "Search and toggle matching items" },
};

/*
 * Allow the user to map incoming characters to various menu commands.
 * The accelerator list must be a valid C string.
 */
#define MAX_MENU_MAPPED_CMDS 32 /* some number */
char mapped_menu_cmds[MAX_MENU_MAPPED_CMDS + 1]; /* exported */
static char mapped_menu_op[MAX_MENU_MAPPED_CMDS + 1];
static short n_menu_mapped = 0;

static boolean initial, from_file;

STATIC_DCL void FDECL(nmcpy, (char *, const char *, int));
STATIC_DCL void FDECL(escapes, (const char *, char *));
STATIC_DCL void FDECL(escapes_ex, (const char*, void*, BOOLEAN_P));
STATIC_DCL void FDECL(rejectoption, (const char *));
STATIC_DCL char *FDECL(string_for_opt, (char *, BOOLEAN_P));
STATIC_DCL char *FDECL(string_for_env_opt, (const char *, char *, BOOLEAN_P));
STATIC_DCL void FDECL(bad_negation, (const char *, BOOLEAN_P));
STATIC_DCL int FDECL(change_inv_order, (char *));
STATIC_DCL boolean FDECL(warning_opts, (char *, const char *));
STATIC_DCL int FDECL(feature_alert_opts, (char *, const char *));
STATIC_DCL boolean FDECL(duplicate_opt_detection, (const char *, int));
STATIC_DCL void FDECL(complain_about_duplicate, (const char *, int));

STATIC_DCL const char *FDECL(attr2attrname, (int));
STATIC_DCL const char * FDECL(msgtype2name, (int));
STATIC_DCL int NDECL(query_msgtype);
STATIC_DCL boolean FDECL(msgtype_add, (int, char *));
STATIC_DCL void FDECL(free_one_msgtype, (int));
STATIC_DCL int NDECL(msgtype_count);
STATIC_DCL boolean FDECL(test_regex_pattern, (const char *, const char *));
STATIC_DCL boolean FDECL(add_menu_coloring_parsed, (char *, int, int));
STATIC_DCL void FDECL(free_one_menu_coloring, (int));
STATIC_DCL int NDECL(count_menucolors);
STATIC_DCL boolean FDECL(parse_role_opts, (BOOLEAN_P, const char *,
                                           char *, char **));

STATIC_DCL void FDECL(doset_add_menu, (winid, const char *, int, int, int));
STATIC_DCL void FDECL(opts_add_others, (winid, const char *, int,
                                        char *, int));
STATIC_DCL int FDECL(handle_add_list_remove, (const char *, int));
STATIC_DCL boolean FDECL(special_handling, (const char *,
                                            BOOLEAN_P, BOOLEAN_P));
STATIC_DCL const char *FDECL(get_compopt_value, (const char *, char *));
STATIC_DCL void FDECL(remove_autopickup_exception,
                      (struct autopickup_exception *));
STATIC_DCL int FDECL(count_ape_maps, (int *, int *));

STATIC_DCL boolean FDECL(is_wc_option, (const char *));
STATIC_DCL boolean FDECL(wc_supported, (const char *));
STATIC_DCL boolean FDECL(is_wc2_option, (const char *));
STATIC_DCL boolean FDECL(wc2_supported, (const char *));
STATIC_DCL void FDECL(wc_set_font_name, (int, char *));
STATIC_DCL int FDECL(wc_set_window_colors, (char *));

void
reglyph_darkroom()
{
    if (flags.dark_room && iflags.use_color)
    {
        showsyms[S_darkroom] = showsyms[S_room];
        showsyms[S_darkgrass] = showsyms[S_grass];
        showsyms[S_darkground] = showsyms[S_ground];
    }
    else
    {
        showsyms[S_darkroom] = showsyms[S_unexplored];
        showsyms[S_darkgrass] = showsyms[S_unexplored];
        showsyms[S_darkground] = showsyms[S_unexplored];
    }
}

/* check whether a user-supplied option string is a proper leading
   substring of a particular option name; option string might have
   a colon or equals sign and arbitrary value appended to it */
boolean
match_optname(user_string, opt_name, min_length, val_allowed)
const char *user_string, *opt_name;
size_t min_length;
boolean val_allowed;
{
    size_t len = strlen(user_string);

    if (val_allowed) {
        const char *p = index(user_string, ':'),
                   *q = index(user_string, '=');

        if (!p || (q && q < p))
            p = q;
        if (p) {
            /* 'user_string' hasn't necessarily been through mungspaces()
               so might have tabs or consecutive spaces */
            while (p > user_string && isspace((uchar) *(p - 1)))
                p--;
            len = (size_t) (p - user_string);
        }
    }

    return (boolean) (len >= min_length
                      && !strncmpi(opt_name, user_string, len));
}

/* most environment variables will eventually be printed in an error
 * message if they don't work, and most error message paths go through
 * BUFSZ buffers, which could be overflowed by a maliciously long
 * environment variable.  If a variable can legitimately be long, or
 * if it's put in a smaller buffer, the responsible code will have to
 * bounds-check itself.
 */
char *
nh_getenv(ev)
const char *ev;
{
    char *getev = getenv(ev);

    if (getev && strlen(getev) <= (BUFSZ / 2))
        return getev;
    else
        return (char *) 0;
}

/* process options, possibly including SYSCF */
void
process_options_file()
{
    init_options();
#ifdef SYSCF
/* someday there may be other SYSCF alternatives besides text file */
#ifdef SYSCF_FILE
    /* If SYSCF_FILE is specified, it _must_ exist... */

    assure_syscf_file();
    config_error_init(TRUE, SYSCF_FILE, FALSE);

    /* ... and _must_ parse correctly. */
    if (!read_config_file(SYSCF_FILE, SET_IN_SYS)) {
        if (config_error_done())
            nh_terminate(EXIT_FAILURE);
    }

    config_error_done();

    /*
     * TODO [maybe]: parse the sysopt entries which are space-separated
     * lists of usernames into arrays with one name per element.
     */
#endif
#endif /* SYSCF */
    finish_options();

}

void
init_options()
{
#if (defined(UNIX) || defined(VMS)) && defined (TTY_GRAPHICS)
    char *opts;
#endif
    int i;

    n_menu_mapped = 0;
    /* set up the command parsing */
    reset_commands(TRUE); /* init */

    /* initialize the random number generator(s) */
    init_random(rn2);
    init_random(rn2_on_display_rng);

    /* for detection of configfile options specified multiple times */
    iflags.opt_booldup = iflags.opt_compdup = (int *) 0;

    for (i = 0; boolopt[i].name; i++) {
        if (boolopt[i].addr)
            *(boolopt[i].addr) = boolopt[i].initvalue;
    }
#if defined(COMPRESS) || defined(ZLIB_COMP)
    set_savepref("externalcomp");
    set_restpref("externalcomp");
#ifdef RLECOMP
    set_savepref("!rlecomp");
    set_restpref("!rlecomp");
#endif
#else
#ifdef ZEROCOMP
    set_savepref("zerocomp");
    set_restpref("zerocomp");
#endif
#ifdef RLECOMP
    set_savepref("rlecomp");
    set_restpref("rlecomp");
#endif
#endif
#ifdef SYSFLAGS
    Strcpy(sysflags.sysflagsid, "sysflags");
    sysflags.sysflagsid[9] = (char) sizeof (struct sysflag);
#endif
    flags.end_own = FALSE;
    flags.end_top = 3;
    flags.end_around = 2;
    flags.paranoia_bits = PARANOID_PRAY | PARANOID_AUTOALL; /* old prayconfirm=TRUE */
    flags.runmode = RUN_LEAP;
    iflags.msg_history = 20;
    /* msg_window has conflicting defaults for multi-interface binary */
#ifdef TTY_GRAPHICS
    iflags.prevmsg_window = 'f';
#else
#ifdef CURSES_GRAPHICS
    iflags.prevmsg_window = 'r';
#endif
#endif
    iflags.menu_headings = ATR_INVERSE;
    iflags.getpos_coords = GPCOORDS_NONE;

    /* hero's role, race, &c haven't been chosen yet */
    flags.initrole = flags.initrace = flags.initgend = flags.initalign
        = ROLE_NONE;

    /* Set the default monster and object class symbols. */
    init_symbols();
    for (i = 0; i < WARNCOUNT; i++)
        warnsyms[i] = def_warnsyms[i].sym;
    iflags.bouldersym = 0;

    /* for "special achievement" tracking (see obj.h,
       create_object(sp_lev.c), addinv_core1(invent.c) */
    iflags.mines_prize_type = LUCKSTONE;
    iflags.mines_prize_oartifact = ART_GLADSTONE;
    iflags.soko_prize_type1 = BAG_OF_HOLDING;
    iflags.soko_prize_type2 = AMULET_OF_REFLECTION;

    /* assert( sizeof flags.inv_order == sizeof def_inv_order ); */
    (void) memcpy((genericptr_t) flags.inv_order,
                  (genericptr_t) def_inv_order, sizeof flags.inv_order);
    flags.pickup_types[0] = '\0';
    flags.pickup_burden = MOD_ENCUMBER;
    flags.sortloot = 'l'; /* sort only loot by default */

    for (i = 0; i < NUM_DISCLOSURE_OPTIONS; i++)
        flags.end_disclose[i] = DISCLOSE_PROMPT_DEFAULT_NO;
    switch_symbols(FALSE); /* set default characters */
    init_r_symbols();
#if defined(UNIX) && defined(TTY_GRAPHICS)
    /*
     * Set defaults for some options depending on what we can
     * detect about the environment's capabilities.
     * This has to be done after the global initialization above
     * and before reading user-specific initialization via
     * config file/environment variable below.
     */
    /* this detects the IBM-compatible console on most 386 boxes */
    if ((opts = nh_getenv("TERM")) && !strncmp(opts, "AT", 2)) {
        if (!symset[PRIMARY].name)
            load_symset("IBMGraphics", PRIMARY);
        if (!symset[ROGUESET].name)
            load_symset("RogueIBM", ROGUESET);
        switch_symbols(TRUE);
#ifdef TEXTCOLOR
        iflags.use_color = TRUE;
#endif
    }
#endif /* UNIX && TTY_GRAPHICS */
#if (defined(UNIX) || defined(VMS)) && defined (TTY_GRAPHICS)
    /* detect whether a "vt" terminal can handle alternate charsets */
    if ((opts = nh_getenv("TERM"))
        /* [could also check "xterm" which emulates vtXXX by default] */
        && !strncmpi(opts, "vt", 2)
        && AS && AE && index(AS, '\016') && index(AE, '\017')) {
        if (!symset[PRIMARY].name)
            load_symset("DECGraphics", PRIMARY);
        switch_symbols(TRUE);
    }
#endif /* (UNIX || VMS) && TTY_GRAPHICS */

#if defined(MSDOS) || defined(WIN32)
    /* Use IBM defaults. Can be overridden via config file */
    if (!symset[PRIMARY].name) {
        load_symset("IBMGraphics_2", PRIMARY);
    }
    if (!symset[ROGUESET].name) {
        load_symset("RogueEpyx", ROGUESET);
    }
#endif
#ifdef MAC_GRAPHICS_ENV
    if (!symset[PRIMARY].name)
        load_symset("MACGraphics", PRIMARY);
    switch_symbols(TRUE);
#endif /* MAC_GRAPHICS_ENV */
    flags.menu_style = MENU_FULL;

    iflags.wc_align_message = ALIGN_TOP;
    iflags.wc_align_status = ALIGN_BOTTOM;
    /* these are currently only used by curses */
    iflags.wc2_statuslines = 2;
    iflags.wc2_windowborders = 2; /* 'Auto' */
    iflags.wc2_here_window_size = HERE_WIN_SIZ_DFLT;
    flags.pile_limit = PILE_LIMIT_DFLT;  /* 5 */

    flags.sound_volume_ambient = 50;
    flags.sound_volume_dialogue = 50;
    flags.sound_volume_effects = 50;
    flags.sound_volume_general = 100;
    flags.sound_volume_music = 50;
    flags.sound_volume_ui = 50;

    flags.spellorder = SORTBY_NONE;
    flags.auto_bag_in_style = 0;
    flags.force_hint = (CasualMode || ModernMode);
    flags.max_hint_difficulty = DEFAULT_MAX_HINT_DIFFICULTY;
    iflags.run_spot_distance = DEFAULT_RUN_SPOT_DISTANCE;

    if (initial_flags.click_action_set)
        flags.self_click_action = initial_flags.click_action_value;

    if (initial_flags.dice_as_ranges_set)
        iflags.show_dice_as_ranges = initial_flags.dice_as_ranges_value;

    if (initial_flags.autodig_set)
        flags.autodig = initial_flags.autodig_value;

    if (initial_flags.ignore_stopping_set)
        flags.ignore_stopping = initial_flags.ignore_stopping_value;

    if (initial_flags.getpos_arrows_set)
        iflags.getpos_arrows = initial_flags.getpos_arrows_value;

    if (initial_flags.save_file_tracking_supported_set)
        iflags.save_file_tracking_supported = initial_flags.save_file_tracking_supported_value;

    if (initial_flags.save_file_tracking_needed_set)
        iflags.save_file_tracking_needed = initial_flags.save_file_tracking_needed_value;

    if (initial_flags.save_file_tracking_on_set)
        iflags.save_file_tracking_on = initial_flags.save_file_tracking_on_value;

#if defined(DGAMELAUNCH) // Unix server
    iflags.save_file_secure = TRUE;
#endif

    flags.save_file_tracking_migrated = TRUE; /* Always set by this new version for migration (old version has it at 0) */
    flags.save_file_tracking_value = iflags.save_file_secure ? SAVEFILETRACK_VALID : SAVEFILETRACK_INVALID;
    if (iflags.save_file_tracking_supported)
    {
        if (iflags.save_file_tracking_needed)
        {
            if (iflags.save_file_tracking_on)
                flags.save_file_tracking_value = SAVEFILETRACK_VALID;
        }
        else
        {
            flags.save_file_tracking_value = SAVEFILETRACK_VALID;
        }
    }

    /* since this is done before init_objects(), do partial init here */
    objects[SLIME_MOLD].oc_name_idx = SLIME_MOLD;
    nmcpy(pl_fruit, OBJ_NAME(objects[SLIME_MOLD]), PL_FSIZ);
}

void
finish_options()
{
#ifndef MAC
    char *opts = getenv("NETHACKOPTIONS");

    if (!opts)
        opts = getenv("HACKOPTIONS");
    if (opts)
    {
        if (*opts == '/' || *opts == '\\' || *opts == '@')
        {
            if (*opts == '@')
                opts++; /* @filename */
            /* looks like a filename */
            if (strlen(opts) < BUFSZ / 2) {
                config_error_init(TRUE, opts, CONFIG_ERROR_SECURE);
                read_config_file(opts, SET_IN_FILE);
                config_error_done();
            }
        }
        else 
        {
            config_error_init(TRUE, (char *) 0, FALSE);
            read_config_file((char *) 0, SET_IN_FILE);
            config_error_done();
            /* let the total length of options be long;
             * parseoptions() will check each individually
             */
            config_error_init(FALSE, "NETHACKOPTIONS", FALSE);
            (void) parseoptions(opts, TRUE, FALSE);
            config_error_done();
        }
    } 
    else
#endif /* !MAC */
    /*else*/ 
    {
        config_error_init(TRUE, (char *) 0, FALSE);
        read_config_file((char *) 0, SET_IN_FILE);
        config_error_done();
    }

    (void) fruitadd(pl_fruit, (struct fruit *) 0);
    /*
     * Remove "slime mold" from list of object names.  This will
     * prevent it from being wished unless it's actually present
     * as a named (or default) fruit.  Wishing for "fruit" will
     * result in the player's preferred fruit [better than "\033"].
     */
    obj_descr[SLIME_MOLD].oc_name = "fruit";

    if (iflags.bouldersym)
        update_bouldersym();
    reglyph_darkroom();

    /* wait_on_space */
    if (flags.rest_on_space)
    {
        (void)bind_key(' ', "wait");
        update_bindings_list();
    }

#ifdef STATUS_HILITES
    /*
     * A multi-interface binary might only support status highlighting
     * for some of the interfaces; check whether we asked for it but are
     * using one which doesn't.
     *
     * Option processing can take place before a user-decided WindowPort
     * is even initialized, so check for that too.
     */
    if (!WINDOWPORT("safe-startup")) {
        if (iflags.hilite_delta && !wc2_supported("statushilites")) {
            raw_printf("Status highlighting not supported for %s interface.",
                       windowprocs.name);
            iflags.hilite_delta = 0;
        }
    }
#endif
    return;
}

/* copy up to maxlen-1 characters; 'dest' must be able to hold maxlen;
   treat comma as alternate end of 'src' */
STATIC_OVL void
nmcpy(dest, src, maxlen)
char *dest;
const char *src;
int maxlen;
{
    int count;

    for (count = 1; count < maxlen; count++) {
        if (*src == ',' || *src == '\0')
            break; /*exit on \0 terminator*/
        *dest++ = *src++;
    }
    *dest = '\0';
}

/*
 * escapes(): escape expansion for showsyms.  C-style escapes understood
 * include \n, \b, \t, \r, \xnnn (hex), \onnn (octal), \nnn (decimal).
 * (Note: unlike in C, leading digit 0 is not used to indicate octal;
 * the letter o (either upper or lower case) is used for that.
 * The ^-prefix for control characters is also understood, and \[mM]
 * has the effect of 'meta'-ing the value which follows (so that the
 * alternate character set will be enabled).
 *
 * X     normal key X
 * ^X    control-X
 * \mX   meta-X
 *
 * For 3.4.3 and earlier, input ending with "\M", backslash, or caret
 * prior to terminating '\0' would pull that '\0' into the output and then
 * keep processing past it, potentially overflowing the output buffer.
 * Now, trailing \ or ^ will act like \\ or \^ and add '\\' or '^' to the
 * output and stop there; trailing \M will fall through to \<other> and
 * yield 'M', then stop.  Any \X or \O followed by something other than
 * an appropriate digit will also fall through to \<other> and yield 'X'
 * or 'O', plus stop if the non-digit is end-of-string.
 */
STATIC_OVL void
escapes(cp, tp)
const char* cp; /* might be 'tp', updating in place */
char* tp; /* result is never longer than 'cp' */
{
    escapes_ex(cp, tp, FALSE);
}

STATIC_OVL void
escapes_ex(cp, tp, tp_is_nhsym)
const char *cp; /* might be 'tp', updating in place */
void *tp; /* result is never longer than 'cp' */
boolean tp_is_nhsym;
{
    static NEARDATA const char oct[] = "01234567", dec[] = "0123456789",
                               hex[] = "00112233445566778899aAbBcCdDeEfF";
    const char *dp;
    int64_t cval, dcount;
    int meta;
    nhsym* tp_nhsym = (nhsym*)tp;
    char* tp_char = (char*)tp;

    while (*cp) {
        /* \M has to be followed by something to do meta conversion,
           otherwise it will just be \M which ultimately yields 'M' */
        meta = (*cp == '\\' && (cp[1] == 'm' || cp[1] == 'M') && cp[2]);
        if (meta)
            cp += 2;

        cval = dcount = 0; /* for decimal, octal, hexadecimal cases */
        if ((*cp != '\\' && *cp != '^') || !cp[1]) {
            /* simple character, or nothing left for \ or ^ to escape */
            cval = *cp++;
        } else if (*cp == '^') { /* expand control-character syntax */
            cval = (*++cp & 0x1f);
            ++cp;

        /* remaining cases are all for backslash; we know cp[1] is not \0 */
        } else if (index(dec, cp[1])) {
            ++cp; /* move past backslash to first digit */
            do {
                cval = (cval * 10) + (*cp - '0');
            } while (*++cp && index(dec, *cp) && ++dcount < 3);
        } else if ((cp[1] == 'o' || cp[1] == 'O') && cp[2]
                   && index(oct, cp[2])) {
            cp += 2; /* move past backslash and 'O' */
            do {
                cval = (cval * 8) + (*cp - '0');
            } while (*++cp && index(oct, *cp) && ++dcount < 3);
        } else if ((cp[1] == 'x' || cp[1] == 'X') && cp[2]
                   && (dp = index(hex, cp[2])) != 0) {
            cp += 2; /* move past backslash and 'X' */
            do {
                cval = (cval * 16) + ((int) (dp - hex) / 2);
            } while (*++cp && (dp = index(hex, *cp)) != 0 && ++dcount < 2);
        } else if ((cp[1] == 'u' || cp[1] == 'U') && cp[2]
                   && (dp = index(hex, cp[2])) != 0) {
            cp += 2; /* move past backslash and 'U' */
            do {
                cval = (cval * 16) + ((int) (dp - hex) / 2);
            } while (*++cp && (dp = index(hex, *cp)) != 0 && ++dcount < 6);
        } else { /* C-style character escapes */
            switch (*++cp) {
            case '\\':
                cval = '\\';
                break;
            case 'n':
                cval = '\n';
                break;
            case 't':
                cval = '\t';
                break;
            case 'b':
                cval = '\b';
                break;
            case 'r':
                cval = '\r';
                break;
            default:
                cval = *cp;
            }
            ++cp;
        }

        if (meta)
            cval |= 0x80;

        if(tp_is_nhsym)
            *tp_nhsym++ = (nhsym)cval;
        else
            *tp_char++ = (char)cval;
    }

    if (tp_is_nhsym)
        *tp_nhsym = '\0';
    else
        *tp_char = '\0';
}

STATIC_OVL void
rejectoption(optname)
const char *optname;
{
#ifdef MICRO
    pline("\"%s\" settable only from %s.", optname, configfile);
#else
    pline("%s can be set only from NETHACKOPTIONS or %s.", optname,
          configfile);
#endif
}

/*

# errors:
OPTIONS=aaaaaaaaaa[ more than 247 (255 - 8 for 'OPTIONS=') total ]aaaaaaaaaa
OPTIONS
OPTIONS=
MSGTYPE=stop"You swap places with "
MSGTYPE=st.op "You swap places with "
MSGTYPE=stop "You swap places with \"
MENUCOLOR=" blessed "green&none
MENUCOLOR=" holy " = green&reverse
MENUCOLOR=" cursed " = red&uline
MENUCOLOR=" unholy " = reed
OPTIONS=!legacy:true,fooo
OPTIONS=align:!pin
OPTIONS=gender

*/

STATIC_OVL char *
string_for_opt(opts, val_optional)
char *opts;
boolean val_optional;
{
    char *colon, *equals;

    colon = index(opts, ':');
    equals = index(opts, '=');
    if (!colon || (equals && equals < colon))
        colon = equals;

    if (!colon || !*++colon) {
        if (!val_optional)
            config_error_add("Missing parameter for '%.60s'", opts);
        return (char *) 0;
    }
    return colon;
}

STATIC_OVL char *
string_for_env_opt(optname, opts, val_optional)
const char *optname;
char *opts;
boolean val_optional;
{
    if (!initial) {
        rejectoption(optname);
        return (char *) 0;
    }
    return string_for_opt(opts, val_optional);
}

STATIC_OVL void
bad_negation(optname, with_parameter)
const char *optname;
boolean with_parameter;
{
    pline_The("%s option may not %sbe negated.", optname,
              with_parameter ? "both have a value and " : "");
}

/*
 * Change the inventory order, using the given string as the new order.
 * Missing characters in the new order are filled in at the end from
 * the current inv_order, except for gold, which is forced to be first
 * if not explicitly present.
 *
 * This routine returns 1 unless there is a duplicate or bad char in
 * the string.
 */
STATIC_OVL int
change_inv_order(op)
char *op;
{
    int oc_sym, num;
    char *sp, buf[QBUFSZ];
    int retval = 1;

    num = 0;
    if (!index(op, GOLD_SYM))
        buf[num++] = COIN_CLASS;

    for (sp = op; *sp; sp++) {
        boolean fail = FALSE;
        oc_sym = def_char_to_objclass(*sp);
        /* reject bad or duplicate entries */
        if (oc_sym == MAX_OBJECT_CLASSES) { /* not an object class char */
            config_error_add("Not an object class '%c'", *sp);
            retval = 0;
            fail = TRUE;
        } else if (!index(flags.inv_order, oc_sym)) {
            /* VENOM_CLASS, RANDOM_CLASS, and ILLOBJ_CLASS are excluded
               because they aren't in def_inv_order[] so don't make it
               into flags.inv_order, hence always fail this index() test */
            config_error_add("Object class '%c' not allowed", *sp);
            retval = 0;
            fail = TRUE;
        } else if (index(sp + 1, *sp)) {
            config_error_add("Duplicate object class '%c'", *sp);
            retval = 0;
            fail = TRUE;
        }
        /* retain good ones */
        if (!fail)
            buf[num++] = (char) oc_sym;
    }
    buf[num] = '\0';

    /* fill in any omitted classes, using previous ordering */
    for (sp = flags.inv_order; *sp; sp++)
        if (!index(buf, *sp))
            (void) strkitten(&buf[num++], *sp);
    buf[MAX_OBJECT_CLASSES - 1] = '\0';

    Strcpy(flags.inv_order, buf);
    return retval;
}

STATIC_OVL boolean
warning_opts(opts, optype)
register char *opts;
const char *optype;
{
    nhsym translate[WARNCOUNT];
    int length = 0, i;

    if (!(opts = string_for_env_opt(optype, opts, FALSE)))
        return FALSE;

    nhsym outstr[64] = { 0 };
    escapes_ex(opts, outstr, TRUE);

    for(i = 0; i < 64; i++)
        if(outstr[i])
            length++;

    /* match the form obtained from PC configuration files */
    for (i = 0; i < WARNCOUNT; i++)
        translate[i] = (i >= length) ? 0
                                     : outstr[i] ? outstr[i]
                                               : (nhsym)def_warnsyms[i].sym;
    assign_warnings(translate);
    return TRUE;
}

void
assign_warnings(graph_chars)
register nhsym *graph_chars;
{
    int i;

    for (i = 0; i < WARNCOUNT; i++)
        if (graph_chars[i])
            warnsyms[i] = graph_chars[i];
}

STATIC_OVL int
feature_alert_opts(op, optn)
char *op;
const char *optn;
{
    char buf[BUFSZ];
    uint64_t fnv = get_feature_notice_ver(op); /* version.c */

    if (fnv == 0L)
        return 0;
    if (fnv > get_current_feature_ver()) {
        if (!initial) {
            You_cant("disable new feature alerts for future versions.");
        } else {
            config_error_add(
                        "%s=%s Invalid reference to a future version ignored",
                             optn, op);
        }
        return 0;
    }

    flags.suppress_alert = fnv;
    if (!initial) {
        Sprintf(buf, "%llu.%llu.%llu", (unsigned long long)FEATURE_NOTICE_VER_MAJ,
            (unsigned long long)FEATURE_NOTICE_VER_MIN, (unsigned long long)FEATURE_NOTICE_VER_PATCH);
        pline(
          "Feature change alerts disabled for GnollHack %s features and prior.",
              buf);
    }
    return 1;
}

void
set_duplicate_opt_detection(on_or_off)
int on_or_off;
{
    int k, *optptr;

    if (on_or_off != 0) {
        /*-- ON --*/
        if (iflags.opt_booldup)
            impossible("iflags.opt_booldup already on (memory leak)");
        iflags.opt_booldup = (int *) alloc(SIZE(boolopt) * sizeof (int));
        optptr = iflags.opt_booldup;
        for (k = 0; k < SIZE(boolopt); ++k)
            *optptr++ = 0;

        if (iflags.opt_compdup)
            impossible("iflags.opt_compdup already on (memory leak)");
        iflags.opt_compdup = (int *) alloc(SIZE(compopt) * sizeof (int));
        optptr = iflags.opt_compdup;
        for (k = 0; k < SIZE(compopt); ++k)
            *optptr++ = 0;
    } else {
        /*-- OFF --*/
        if (iflags.opt_booldup)
            free((genericptr_t) iflags.opt_booldup);
        iflags.opt_booldup = (int *) 0;
        if (iflags.opt_compdup)
            free((genericptr_t) iflags.opt_compdup);
        iflags.opt_compdup = (int *) 0;
    }
}

STATIC_OVL boolean
duplicate_opt_detection(opts, iscompound)
const char *opts;
int iscompound; /* 0 == boolean option, 1 == compound */
{
    int i, *optptr;

    if (!iscompound && iflags.opt_booldup && initial && from_file) {
        for (i = 0; boolopt[i].name; i++) {
            if (match_optname(opts, boolopt[i].name, 3, FALSE)) {
                optptr = iflags.opt_booldup + i;
                *optptr += 1;
                if (*optptr > 1)
                    return TRUE;
                else
                    return FALSE;
            }
        }
    } else if (iscompound && iflags.opt_compdup && initial && from_file) {
        for (i = 0; compopt[i].name; i++) {
            if (match_optname(opts, compopt[i].name, strlen(compopt[i].name),
                              TRUE)) {
                optptr = iflags.opt_compdup + i;
                *optptr += 1;
                if (*optptr > 1)
                    return TRUE;
                else
                    return FALSE;
            }
        }
    }
    return FALSE;
}

STATIC_OVL void
complain_about_duplicate(opts, iscompound)
const char *opts;
int iscompound; /* 0 == boolean option, 1 == compound */
{
#ifdef MAC
    /* the Mac has trouble dealing with the output of messages while
     * processing the config file.  That should get fixed one day.
     * For now just return.
     */
#else /* !MAC */
    config_error_add("%s option specified multiple times: %s",
                     iscompound ? "compound" : "boolean", opts);
#endif /* ?MAC */
    return;
}

/* paranoia[] - used by parseoptions() and special_handling() */
STATIC_VAR const struct paranoia_opts {
    int flagmask;        /* which paranoid option */
    const char *argname; /* primary name */
    int argMinLen;       /* minimum number of letters to match */
    const char *synonym; /* alternate name (optional) */
    int synMinLen;
    const char *explain; /* for interactive menu */
} paranoia[] = {
    /* there are some initial-letter conflicts: "a"ttack vs "a"ll, "attack"
       takes precedence and "all" isn't present in the interactive menu,
       and "d"ie vs "d"eath, synonyms for each other so doesn't matter;
       (also "p"ray vs "P"aranoia, "pray" takes precedence since "Paranoia"
       is just a synonym for "Confirm"); "b"ones vs "br"eak-wand, the
       latter requires at least two letters; "wand"-break vs "Were"-change,
       both require at least two letters during config processing and use
       case-senstivity for 'O's interactive menu */
    { PARANOID_CONFIRM, "Confirm", 1, "Paranoia", 2,
      "for \"yes\" confirmations, require \"no\" to reject" },
    { PARANOID_QUIT, "quit", 1, "explore", 1,
      "yes vs y to quit or to enter explore mode" },
    { PARANOID_DIE, "die", 1, "death", 2,
      "yes vs y to die (explore mode or debug mode)" },
    { PARANOID_BONES, "bones", 1, 0, 0,
      "yes vs y to save bones data when dying in debug mode" },
    { PARANOID_HIT, "attack", 1, "hit", 1,
      "yes vs y to attack a peaceful monster" },
    { PARANOID_BREAK, "Break", 2, (const char*)0, 0,
      "yes vs y to break an item" },
    { PARANOID_WERECHANGE, "lycanthropy-change", 2, "were-change", 2,
      "yes vs y to change form when lycanthropy is controllable" },
    { PARANOID_PRAY, "pray", 1, 0, 0,
      "y to pray (supersedes old \"prayconfirm\" option)" },
    { PARANOID_REMOVE, "Remove", 1, "Takeoff", 1,
      "always pick from inventory for Remove and Takeoff" },
    { PARANOID_WATER, "water", 2, (const char*)0, 0,
      "yes vs y to enter a pool of water or lava" },
    { PARANOID_TRAP, "trap", 2, (const char*)0, 0,
      "yes vs y to enter into a trap" },
    { PARANOID_AUTOALL, "Autoall", 1, (const char*)0, 0,
      "y to select all items" },
    { PARANOID_TIP, "Tip", 2, (const char*)0, 0,
      "yes vs y to tip a container" },
    { PARANOID_WIELDED_WEAPON, "Wielded-weapon", 1, (const char*)0, 0,
      "y to attack with a weapon" },
      /* for config file parsing; interactive menu skips these */
    { 0, "none", 4, 0, 0, 0 }, /* require full word match */
    { ~0, "all", 3, 0, 0, 0 }, /* ditto */
};

extern struct menucoloring *menu_colorings;

static const struct {
    const char *name;
    const int color;
} colornames[] = {
    { "black", CLR_BLACK },
    { "red", CLR_RED },
    { "green", CLR_GREEN },
    { "brown", CLR_BROWN },
    { "blue", CLR_BLUE },
    { "magenta", CLR_MAGENTA },
    { "cyan", CLR_CYAN },
    { "gray", CLR_GRAY },
    { "orange", CLR_ORANGE },
    { "light green", CLR_BRIGHT_GREEN },
    { "yellow", CLR_YELLOW },
    { "light blue", CLR_BRIGHT_BLUE },
    { "light magenta", CLR_BRIGHT_MAGENTA },
    { "light cyan", CLR_BRIGHT_CYAN },
    { "white", CLR_WHITE },
    { "no color", NO_COLOR },
    { NULL, CLR_BLACK }, /* everything after this is an alias */
    { "transparent", NO_COLOR },
    { "purple", CLR_MAGENTA },
    { "light purple", CLR_BRIGHT_MAGENTA },
    { "bright purple", CLR_BRIGHT_MAGENTA },
    { "grey", CLR_GRAY },
    { "bright red", CLR_ORANGE },
    { "bright green", CLR_BRIGHT_GREEN },
    { "bright blue", CLR_BRIGHT_BLUE },
    { "bright magenta", CLR_BRIGHT_MAGENTA },
    { "bright cyan", CLR_BRIGHT_CYAN }
};

static const struct {
    const char *name;
    const int attr;
} attrnames[] = {
    { "none", ATR_NONE },
    { "bold", ATR_BOLD },
    { "dim", ATR_DIM },
    { "underline", ATR_ULINE },
    { "blink", ATR_BLINK },
    { "inverse", ATR_INVERSE },
    { NULL, ATR_NONE }, /* everything after this is an alias */
    { "normal", ATR_NONE },
    { "uline", ATR_ULINE },
    { "reverse", ATR_INVERSE },
};

const char *
clr2colorname(clr)
int clr;
{
    int i;

    for (i = 0; i < SIZE(colornames); i++)
        if (colornames[i].name && colornames[i].color == clr)
            return colornames[i].name;
    return (char *) 0;
}

/* Note: compares only the const pointers, not the actual strings */
int
clrnameptr2color(clrnameptr)
const char* clrnameptr;
{
    int i;

    for (i = 0; i < SIZE(colornames); i++)
        if (colornames[i].name && colornames[i].name == clrnameptr)
            return colornames[i].color;
    return NO_COLOR;
}

int
match_str2clr(str)
char *str;
{
    int i, c = CLR_MAX;

    /* allow "lightblue", "light blue", and "light-blue" to match "light blue"
       (also junk like "_l i-gh_t---b l u e" but we won't worry about that);
       also copes with trailing space; caller has removed any leading space */
    for (i = 0; i < SIZE(colornames); i++)
        if (colornames[i].name
            && fuzzymatch(str, colornames[i].name, " -_", TRUE)) {
            c = colornames[i].color;
            break;
        }
    if (i == SIZE(colornames) && digit(*str))
        c = atoi(str);

    if (c < 0 || c >= CLR_MAX) {
        config_error_add("Unknown color '%.60s'", str);
        c = CLR_MAX; /* "none of the above" */
    }    
    return c;
}

STATIC_OVL const char *
attr2attrname(attr)
int attr;
{
    int i;

    for (i = 0; i < SIZE(attrnames); i++)
        if (attrnames[i].attr == attr)
            return attrnames[i].name;
    return (char *) 0;
}

int
match_str2attr(str, complain)
const char *str;
boolean complain;
{
    int i, a = -1;

    for (i = 0; i < SIZE(attrnames); i++)
        if (attrnames[i].name
            && fuzzymatch(str, attrnames[i].name, " -_", TRUE)) {
            a = attrnames[i].attr;
            break;
        }

    if (a == -1 && complain)
        config_error_add("Unknown text attribute '%.60s'", str);

    return a;
}

int
query_color(prompt)
const char *prompt;
{
    winid tmpwin;
    anything any;
    int i, pick_cnt;
    menu_item *picks = (menu_item *) 0;

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu(tmpwin);
    any = zeroany;
    for (i = 0; i < SIZE(colornames); i++) {
        if (!colornames[i].name)
            break;
        any.a_int = i + 1;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, colornames[i].color, colornames[i].name,
                 (colornames[i].color == NO_COLOR) ? MENU_SELECTED
                                                   : MENU_UNSELECTED);
    }
    end_menu(tmpwin, (prompt && *prompt) ? prompt : "Pick a color");
    pick_cnt = select_menu(tmpwin, PICK_ONE, &picks);
    destroy_nhwindow(tmpwin);
    if (pick_cnt > 0) {
        i = colornames[picks[0].item.a_int - 1].color;
        /* pick_cnt==2: explicitly picked something other than the
           preselected entry */
        if (pick_cnt == 2 && i == NO_COLOR)
            i = colornames[picks[1].item.a_int - 1].color;
        free((genericptr_t) picks);
        return i;
    } else if (pick_cnt == 0) {
        /* pick_cnt==0: explicitly picking preselected entry toggled it off */
        return NO_COLOR;
    }
    return -1;
}

/* ask about highlighting attribute; for menu headers and menu
   coloring patterns, only one attribute at a time is allowed;
   for status highlighting, multiple attributes are allowed [overkill;
   life would be much simpler if that were restricted to one also...] */
int
query_attr(prompt)
const char *prompt;
{
    winid tmpwin;
    anything any;
    int i, pick_cnt;
    menu_item *picks = (menu_item *) 0;
    boolean allow_many = (prompt && !strncmpi(prompt, "Choose", 6));
    int default_attr = ATR_NONE;

    if (prompt && strstri(prompt, "menu headings"))
        default_attr = iflags.menu_headings;
    tmpwin = create_nhwindow(NHW_MENU);
    start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_SIMPLE);
    any = zeroany;
    for (i = 0; i < SIZE(attrnames); i++) {
        if (!attrnames[i].name)
            break;
        any.a_int = i + 1;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, attrnames[i].attr, NO_COLOR,
                 attrnames[i].name,
                 (attrnames[i].attr == default_attr) ? MENU_SELECTED
                                                     : MENU_UNSELECTED);
    }
    end_menu(tmpwin, (prompt && *prompt) ? prompt : "Pick an attribute");
    pick_cnt = select_menu(tmpwin, allow_many ? PICK_ANY : PICK_ONE, &picks);
    destroy_nhwindow(tmpwin);
    if (pick_cnt > 0) {
        int j, k = 0;

        if (allow_many) {
            /* PICK_ANY, with one preselected entry (ATR_NONE) which
               should be excluded if any other choices were picked */
            for (i = 0; i < pick_cnt; ++i) {
                j = picks[i].item.a_int - 1;
                if (attrnames[j].attr != ATR_NONE || pick_cnt == 1) {
                    switch (attrnames[j].attr) {
                    case ATR_DIM:
                        k |= HL_DIM;
                        break;
                    case ATR_BLINK:
                        k |= HL_BLINK;
                        break;
                    case ATR_ULINE:
                        k |= HL_ULINE;
                        break;
                    case ATR_INVERSE:
                        k |= HL_INVERSE;
                        break;
                    case ATR_BOLD:
                        k |= HL_BOLD;
                        break;
                    case ATR_NONE:
                        k = HL_NONE;
                        break;
                    }
                }
            }
        } else {
            /* PICK_ONE, but might get 0 or 2 due to preselected entry */
            j = picks[0].item.a_int - 1;
            /* pick_cnt==2: explicitly picked something other than the
               preselected entry */
            if (pick_cnt == 2 && attrnames[j].attr == default_attr)
                j = picks[1].item.a_int - 1;
            k = attrnames[j].attr;
        }
        free((genericptr_t) picks);
        return k;
    } else if (pick_cnt == 0 && !allow_many) {
        /* PICK_ONE, preselected entry explicitly chosen */
        return default_attr;
    }
    /* either ESC to explicitly cancel (pick_cnt==-1) or
       PICK_ANY with preselected entry toggled off and nothing chosen */
    return -1;
}

static const struct {
    const char *name;
    xchar msgtyp;
    const char *descr;
} msgtype_names[] = {
    { "show", MSGTYP_NORMAL, "Show message normally" },
    { "hide", MSGTYP_NOSHOW, "Hide message" },
    { "noshow", MSGTYP_NOSHOW, NULL },
    { "stop", MSGTYP_STOP, "Prompt for more after the message" },
    { "more", MSGTYP_STOP, NULL },
    { "norep", MSGTYP_NOREP, "Do not repeat the message" }
};

STATIC_OVL const char *
msgtype2name(typ)
int typ;
{
    int i;

    for (i = 0; i < SIZE(msgtype_names); i++)
        if (msgtype_names[i].descr && msgtype_names[i].msgtyp == typ)
            return msgtype_names[i].name;
    return (char *) 0;
}

int
query_msgtype()
{
    winid tmpwin;
    anything any;
    int i, pick_cnt;
    menu_item *picks = (menu_item *) 0;

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu(tmpwin);
    any = zeroany;
    for (i = 0; i < SIZE(msgtype_names); i++)
        if (msgtype_names[i].descr) {
            any.a_int = msgtype_names[i].msgtyp + 1;
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                 msgtype_names[i].descr, MENU_UNSELECTED);
        }
    end_menu(tmpwin, "How to show the message");
    pick_cnt = select_menu(tmpwin, PICK_ONE, &picks);
    destroy_nhwindow(tmpwin);
    if (pick_cnt > 0) {
        i = picks->item.a_int - 1;
        free((genericptr_t) picks);
        return i;
    }
    return -1;
}

STATIC_OVL boolean
msgtype_add(typ, pattern)
int typ;
char *pattern;
{
    struct plinemsg_type *tmp = (struct plinemsg_type *) alloc(sizeof *tmp);

    tmp->msgtype = typ;
    tmp->regex = regex_init();
    if (!regex_compile(pattern, tmp->regex)) {
        static const char *re_error = "MSGTYPE regex error";

        config_error_add("%s: %s", re_error, regex_error_desc(tmp->regex));
        regex_free(tmp->regex);
        free((genericptr_t) tmp);
        return FALSE;
    }
    tmp->pattern = dupstr(pattern);
    tmp->next = plinemsg_types;
    plinemsg_types = tmp;
    return TRUE;
}

void
msgtype_free()
{
    struct plinemsg_type *tmp, *tmp2 = 0;

    for (tmp = plinemsg_types; tmp; tmp = tmp2) {
        tmp2 = tmp->next;
        free((genericptr_t) tmp->pattern);
        regex_free(tmp->regex);
        free((genericptr_t) tmp);
    }
    plinemsg_types = (struct plinemsg_type *) 0;
}

STATIC_OVL void
free_one_msgtype(idx)
int idx; /* 0 .. */
{
    struct plinemsg_type *tmp = plinemsg_types;
    struct plinemsg_type *prev = NULL;

    while (tmp) {
        if (idx == 0) {
            struct plinemsg_type *next = tmp->next;

            regex_free(tmp->regex);
            free((genericptr_t) tmp->pattern);
            free((genericptr_t) tmp);
            if (prev)
                prev->next = next;
            else
                plinemsg_types = next;
            return;
        }
        idx--;
        prev = tmp;
        tmp = tmp->next;
    }
}

int
msgtype_type(msg, norepeat)
const char *msg;
boolean norepeat; /* called from Norep(via pline) */
{
    struct plinemsg_type *tmp = plinemsg_types;

    while (tmp) {
        /* we don't exclude entries with negative msgtype values
           because then the msg might end up matching a later pattern */
        if (regex_match(msg, tmp->regex))
            return tmp->msgtype;
        tmp = tmp->next;
    }
    return norepeat ? MSGTYP_NOREP : MSGTYP_NORMAL;
}

/* negate one or more types of messages so that their type handling will
   be disabled or re-enabled; MSGTYPE_NORMAL (value 0) is not affected */
void
hide_unhide_msgtypes(hide, hide_mask)
boolean hide;
int hide_mask;
{
    struct plinemsg_type *tmp;
    int mt;

    /* negative msgtype value won't be recognized by pline, so does nothing */
    for (tmp = plinemsg_types; tmp; tmp = tmp->next) {
        mt = tmp->msgtype;
        if (!hide)
            mt = -mt; /* unhide: negate negative, yielding positive */
        if (mt > 0 && ((1 << mt) & hide_mask))
            tmp->msgtype = -tmp->msgtype;
    }
}

STATIC_OVL int
msgtype_count(VOID_ARGS)
{
    int c = 0;
    struct plinemsg_type *tmp = plinemsg_types;

    while (tmp) {
        c++;
        tmp = tmp->next;
    }
    return c;
}

boolean
msgtype_parse_add(str)
char *str;
{
    char pattern[256];
    char msgtype[11];

    if (sscanf(str, "%10s \"%255[^\"]\"", msgtype, pattern) == 2) 
    {
        msgtype[10] = '\0';
        int typ = -1;
        int i;

        for (i = 0; i < SIZE(msgtype_names); i++)
            if (!strncmpi(msgtype_names[i].name, msgtype, strlen(msgtype))) 
            {
                typ = msgtype_names[i].msgtyp;
                break;
            }

        if (typ != -1)
            return msgtype_add(typ, pattern);
        else
            config_error_add("Unknown message type '%s'", msgtype);

    } 
    else 
    {
        config_error_add("Malformed MSGTYPE");
    }
    return FALSE;
}

STATIC_OVL boolean
test_regex_pattern(str, errmsg)
const char *str;
const char *errmsg;
{
    static const char re_error[] = "Regex error";
    struct nhregex *match;
    boolean retval = TRUE;

    if (!str)
        return FALSE;

    match = regex_init();
    if (!match) {
        config_error_add("NHregex error");
        return FALSE;
    }

    if (!regex_compile(str, match)) {
        config_error_add("%s: %s", errmsg ? errmsg : re_error,
                         regex_error_desc(match));
        retval = FALSE;
    }
    regex_free(match);
    return retval;
}

boolean
add_menu_coloring_parsed(str, c, a)
char *str;
int c, a;
{
    static const char re_error[] = "Menucolor regex error";
    struct menucoloring *tmp;

    if (!str)
        return FALSE;

    tmp = (struct menucoloring *) alloc(sizeof (struct menucoloring));
    tmp->match = regex_init();

    if (!regex_compile(str, tmp->match)) 
    {
        config_error_add("%s: %s", re_error, regex_error_desc(tmp->match));
        regex_free(tmp->match);
        free(tmp);
        return FALSE;
    } 
    else 
    {
        tmp->next = menu_colorings;
        tmp->origstr = dupstr(str);
        tmp->color = c;
        tmp->attr = a;
        menu_colorings = tmp;
        return TRUE;
    }
}

/* parse '"regex_string"=color&attr' and add it to menucoloring */
boolean
add_menu_coloring(tmpstr)
char *tmpstr; /* never Null but could be empty */
{
    int c = NO_COLOR, a = ATR_NONE;
    char *tmps, *cs, *amp;
    char str[BUFSIZ];

    //Sprintf(str, "%s", tmpstr);
    Strncpy(str, tmpstr, (sizeof str) - 1);
    str[(sizeof str) - 1] = '\0';

    if ((cs = index(str, '=')) == 0) {
        config_error_add("Malformed MENUCOLOR");
        return FALSE;
    }

    tmps = cs + 1; /* advance past '=' */
    mungspaces(tmps);
    if ((amp = index(tmps, '&')) != 0)
        *amp = '\0';

    c = match_str2clr(tmps);
    if (c >= CLR_MAX || c < 0)
        return FALSE;

    if (amp) {
        tmps = amp + 1; /* advance past '&' */
        a = match_str2attr(tmps, TRUE);
        if (a == -1)
            return FALSE;
    }

    /* the regexp portion here has not been condensed by mungspaces() */
    *cs = '\0';
    tmps = str;
    if (*tmps == '"' || *tmps == '\'') {
        cs--;
        while (isspace((uchar) *cs))
            cs--;
        if (*cs == *tmps) {
            *cs = '\0';
            tmps++;
        }
    }
    return add_menu_coloring_parsed(tmps, c, a);
}

boolean
get_menu_coloring(str, color, attr)
const char *str;
int *color, *attr;
{
    struct menucoloring *tmpmc;

    if (iflags.use_menu_color)
        for (tmpmc = menu_colorings; tmpmc; tmpmc = tmpmc->next)
            if (regex_match(str, tmpmc->match)) {
                *color = tmpmc->color;
                *attr = tmpmc->attr;
                return TRUE;
            }
    return FALSE;
}

int
get_menu_coloring_index(str)
const char* str;
{
    int indx = 0;
    struct menucoloring* tmpmc;

    if (iflags.use_menu_color)
        for (tmpmc = menu_colorings; tmpmc; tmpmc = tmpmc->next)
        {
            if (regex_match(str, tmpmc->match))
            {
                return indx;
            }
            indx++;
        }

    //Nothing found
    return -1;
}

boolean
free_menu_coloring_str(str)
const char* str;
{
    int i = get_menu_coloring_index(str);
    if (i >= 0)
    {
        free_one_menu_coloring(i);
        return TRUE;
    }
    return FALSE;
}



void
free_menu_coloring()
{
    struct menucoloring *tmp, *tmp2;

    for (tmp = menu_colorings; tmp; tmp = tmp2) {
        tmp2 = tmp->next;
        if (tmp->match)
            regex_free(tmp->match);
        tmp->match = 0;
        if(tmp->origstr)
            free((genericptr_t) tmp->origstr);
        tmp->origstr = 0;
        free((genericptr_t) tmp);
    }
    menu_colorings = 0;
}

STATIC_OVL void
free_one_menu_coloring(idx)
int idx; /* 0 .. */
{
    struct menucoloring *tmp = menu_colorings;
    struct menucoloring *prev = NULL;

    while (tmp) {
        if (idx == 0) {
            struct menucoloring *next = tmp->next;

            regex_free(tmp->match);
            free((genericptr_t) tmp->origstr);
            free((genericptr_t) tmp);
            if (prev)
                prev->next = next;
            else
                menu_colorings = next;
            return;
        }
        idx--;
        prev = tmp;
        tmp = tmp->next;
    }
}

STATIC_OVL int
count_menucolors(VOID_ARGS)
{
    struct menucoloring *tmp;
    int count = 0;

    for (tmp = menu_colorings; tmp; tmp = tmp->next)
        count++;
    return count;
}

boolean
menu_style_allows_menu_coloring(style)
int style;
{
    switch (style)
    {
    case GHMENU_STYLE_INVENTORY:
    case GHMENU_STYLE_PERMANENT_INVENTORY:
    case GHMENU_STYLE_OTHERS_INVENTORY:
    case GHMENU_STYLE_PICK_ITEM_LIST:
    case GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK:
    case GHMENU_STYLE_CHAT_CHOOSE_ITEM:
    case GHMENU_STYLE_SKILLS:
    case GHMENU_STYLE_SPELLS:
    case GHMENU_STYLE_VIEW_SPELL:
        return TRUE;
    case GHMENU_STYLE_GENERAL:
    case GHMENU_STYLE_PICK_CATEGORY_LIST:
    case GHMENU_STYLE_ITEM_COMMAND:
    case GHMENU_STYLE_CHAT:
    case GHMENU_STYLE_CHOOSE_SIMPLE:
    case GHMENU_STYLE_CHOOSE_COMMAND:
    case GHMENU_STYLE_CHOOSE_SAVED_GAME:
    case GHMENU_STYLE_CHOOSE_PLAYER:
    case GHMENU_STYLE_ACCEPT_PLAYER:
    case GHMENU_STYLE_CHOOSE_DIFFICULTY:
    case GHMENU_STYLE_CHARACTER:
    case GHMENU_STYLE_ATTRIBUTES:
    case GHMENU_STYLE_SKILLS_ALTERNATE:
    case GHMENU_STYLE_SPELLS_ALTERNATE:
    case GHMENU_STYLE_VIEW_SPELL_ALTERNATE:
    case GHMENU_STYLE_DUNGEON_OVERVIEW:
    case GHMENU_STYLE_OPTIONS:
    case GHMENU_STYLE_HELP:
    case GHMENU_STYLE_SKILL_COMMAND:
    case GHMENU_STYLE_SPELL_COMMAND:
    case GHMENU_STYLE_GENERAL_COMMAND:
    case GHMENU_STYLE_MONSTER_ABILITY:
    case GHMENU_STYLE_DELETE_SAVED_GAME:
    case GHMENU_STYLE_START_GAME_MENU:
    case GHMENU_STYLE_PREVIOUS_MESSAGES:
    case GHMENU_STYLE_VIEW_FILE:
        return FALSE;
    default:
        return TRUE;
    }
}

STATIC_OVL boolean
parse_role_opts(negated, fullname, opts, opp)
boolean negated;
const char *fullname;
char *opts;
char **opp;
{
    char *op = *opp;

    if (negated) {
        bad_negation(fullname, FALSE);
    } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
        boolean val_negated = FALSE;

        while ((*op == '!') || !strncmpi(op, "no", 2)) {
            if (*op == '!')
                op++;
            else
                op += 2;
            val_negated = !val_negated;
        }
        if (val_negated) {
            if (!setrolefilter(op)) {
                config_error_add("Unknown negated parameter '%s'", op);
                return FALSE;
            }
        } else {
            if (duplicate_opt_detection(opts, 1))
                complain_about_duplicate(opts, 1);
            *opp = op;
            return TRUE;
        }
    }
    return FALSE;
}

/* Check if character c is illegal as a menu command key */
boolean
illegal_menu_cmd_key(c)
char c;
{
    if (c == 0 || c == '\r' || c == '\n' || c == '\033'
        || c == ' ' || digit(c) || (letter(c) && c != '@')) {
        config_error_add("Reserved menu command key '%s'", visctrl(c));
        return TRUE;
    } else { /* reject default object class symbols */
        int j;
        for (j = 1; j < MAX_OBJECT_CLASSES; j++)
            if (c == def_oc_syms[j].sym) {
                config_error_add("Menu command key '%s' is an object class",
                                 visctrl(c));
                return TRUE;
            }
    }
    return FALSE;
}

boolean
parseoptions(opts, tinitial, tfrom_file)
register char *opts;
boolean tinitial, tfrom_file;
{
    char *op;
    unsigned num;
    boolean negated, duplicate;
    int i;
    const char *fullname;
    const char* fullname2;
    boolean retval = TRUE;

    initial = tinitial;
    from_file = tfrom_file;
    if ((op = index(opts, ',')) != 0) {
        *op++ = 0;
        if (!parseoptions(op, initial, from_file))
            retval = FALSE;
    }
    if (strlen(opts) > BUFSZ / 2) {
        config_error_add("Option too long, max length is %i characters",
                         (BUFSZ / 2));
        return FALSE;
    }

    /* strip leading and trailing white space */
    while (isspace((uchar) *opts))
        opts++;
    op = eos(opts);
    while (--op >= opts && isspace((uchar) *op))
        *op = '\0';

    if (!*opts) {
        config_error_add("Empty statement");
        return FALSE;
    }
    negated = FALSE;
    while ((*opts == '!') || !strncmpi(opts, "no", 2)) {
        if (*opts == '!')
            opts++;
        else
            opts += 2;
        negated = !negated;
    }

    /* variant spelling */

    if (match_optname(opts, "colour", 5, FALSE))
        Strcpy(opts, "color"); /* fortunately this isn't longer */

    /* special boolean options */

    if (match_optname(opts, "female", 3, FALSE)) {
        if (duplicate_opt_detection(opts, 0))
            complain_about_duplicate(opts, 0);
        if (!initial && flags.female == negated) {
            config_error_add("That is not anatomically possible.");
            return FALSE;
        } else
            flags.initgend = flags.female = !negated;
        return retval;
    }

    if (match_optname(opts, "male", 4, FALSE)) {
        if (duplicate_opt_detection(opts, 0))
            complain_about_duplicate(opts, 0);
        if (!initial && flags.female != negated) {
            config_error_add("That is not anatomically possible.");
            return FALSE;
        } else
            flags.initgend = flags.female = negated;
        return retval;
    }

#if defined(MICRO) && !defined(AMIGA)
    /* included for compatibility with old GnollHack.cnf files */
    if (match_optname(opts, "IBM_", 4, FALSE)) {
        iflags.BIOS = !negated;
        return retval;
    }
#endif /* MICRO */

    /* compound options */

    /* This first batch can be duplicated if their values are negated */

    /* align:string */
    fullname = "align";
    if (match_optname(opts, fullname, sizeof "align" - 1, TRUE)) {
        if (parse_role_opts(negated, fullname, opts, &op)) {
            if ((flags.initalign = str2align(op)) == ROLE_NONE) {
                config_error_add("Unknown %s '%s'", fullname, op);
                return FALSE;
            }
        } else
            return FALSE;
        return retval;
    }

    /* role:string or character:string */
    fullname = "role";
    if (match_optname(opts, fullname, 4, TRUE)
        || match_optname(opts, (fullname = "character"), 4, TRUE)) {
        if (parse_role_opts(negated, fullname, opts, &op)) {
            if ((flags.initrole = str2role(op)) == ROLE_NONE) {
                config_error_add("Unknown %s '%s'", fullname, op);
                return FALSE;
            } else /* Backwards compatibility */
                nmcpy(pl_character, op, PL_NSIZ);
        } else
            return FALSE;
        return retval;
    }

    /* race:string */
    fullname = "race";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (parse_role_opts(negated, fullname, opts, &op)) {
            if ((flags.initrace = str2race(op)) == ROLE_NONE) {
                config_error_add("Unknown %s '%s'", fullname, op);
                return FALSE;
            } else /* Backwards compatibility */
                pl_race = *op;
        } else
            return FALSE;
        return retval;
    }

    /* gender:string */
    fullname = "gender";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (parse_role_opts(negated, fullname, opts, &op)) {
            if ((flags.initgend = str2gend(op)) == ROLE_NONE) {
                config_error_add("Unknown %s '%s'", fullname, op);
                return FALSE;
            } else
                flags.female = flags.initgend;
        } else
            return FALSE;
        return retval;
    }

    /* We always check for duplicates on the remaining compound options,
       although individual option processing can choose to complain or not */

    duplicate = duplicate_opt_detection(opts, 1); /* 1: check compounds */

    fullname = "pettype";
    if (match_optname(opts, fullname, 3, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            if (negated) {
                bad_negation(fullname, TRUE);
                return FALSE;
            } else
                switch (lowc(*op)) {
                case 'd': /* dog */
                    preferred_pet = 'd';
                    break;
                case 'c': /* cat */
                case 'f': /* feline */
                    preferred_pet = 'c';
                    break;
                case 'h': /* horse */
                case 'r': /* ram */
                case 'q': /* quadruped */
                    /* avoids giving "unrecognized type of pet" but
                       pet_type(dog.c) won't actually honor this */
                    preferred_pet = 'h';
                    break;
                case 'n': /* no pet */
                    preferred_pet = 'n';
                    flags.no_pets_preference = TRUE;
                    break;
                case '*': /* random */
                    preferred_pet = '\0';
                    break;
                default:
                    config_error_add("Unrecognized pet type '%s'.", op);
                    return FALSE;
                    break;
                }
        }
        else if (negated)
        {
            preferred_pet = 'n';
            flags.no_pets_preference = TRUE;
        }
        return retval;
    }

    fullname = "catname";
    if (match_optname(opts, fullname, 7, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(catname, op, PL_PSIZ);
        } else
            return FALSE;
        sanitize_name(catname);
        return retval;
    }

    fullname = "dogname";
    if (match_optname(opts, fullname, 7, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(dogname, op, PL_PSIZ);
        } else
            return FALSE;
        sanitize_name(dogname);
        return retval;
    }

    fullname = "horsename";
    if (match_optname(opts, fullname, 9, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(horsename, op, PL_PSIZ);
        } else
            return FALSE;
        sanitize_name(horsename);
        return retval;
    }

    fullname = "ramname";
    if (match_optname(opts, fullname, 7, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(ramname, op, PL_PSIZ);
        }
        else
            return FALSE;
        sanitize_name(ramname);
        return retval;
    }

    fullname = "luggagename";
    if (match_optname(opts, fullname, 11, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(luggagename, op, PL_PSIZ);
        }
        else
            return FALSE;
        sanitize_name(luggagename);
        return retval;
    }

    fullname = "wolfname";
    if (match_optname(opts, fullname, 8, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(wolfname, op, PL_PSIZ);
        }
        else
            return FALSE;
        sanitize_name(wolfname);
        return retval;
    }

    fullname = "doggender";
    if (match_optname(opts, fullname, 9, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            switch (lowc(*op)) {
            case 'f': /* female */
                doggender = 2;
                break;
            case 'm': /* male */
                doggender = 1;
                break;
            case 'n': /* neuter or none */
            case 'r': /* random */
            case '*': /* random */
                doggender = 0;
                break;
            default:
                config_error_add("Unrecognized dog gender '%s'.", op);
                return FALSE;
                break;
            }
        }
        return retval;
    }

    fullname = "catgender";
    if (match_optname(opts, fullname, 9, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            switch (lowc(*op)) {
            case 'f': /* female */
                catgender = 2;
                break;
            case 'm': /* male */
                catgender = 1;
                break;
            case 'n': /* neuter or none */
            case 'r': /* random */
            case '*': /* random */
                catgender = 0;
                break;
            default:
                config_error_add("Unrecognized cat gender '%s'.", op);
                return FALSE;
                break;
            }
        }
        return retval;
    }

    fullname = "horsegender";
    if (match_optname(opts, fullname, 11, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            switch (lowc(*op)) {
            case 'f': /* female */
                horsegender = 2;
                break;
            case 'm': /* male */
                horsegender = 1;
                break;
            case 'n': /* neuter or none */
            case 'r': /* random */
            case '*': /* random */
                horsegender = 0;
                break;
            default:
                config_error_add("Unrecognized horse gender '%s'.", op);
                return FALSE;
                break;
            }
        }
        return retval;
    }

    fullname = "ramgender";
    if (match_optname(opts, fullname, 9, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            switch (lowc(*op)) {
            case 'f': /* female */
                ramgender = 2;
                break;
            case 'm': /* male */
                ramgender = 1;
                break;
            case 'n': /* neuter or none */
            case 'r': /* random */
            case '*': /* random */
                ramgender = 0;
                break;
            default:
                config_error_add("Unrecognized ram gender '%s'.", op);
                return FALSE;
                break;
            }
        }
        return retval;
    }

    fullname = "wolfgender";
    if (match_optname(opts, fullname, 10, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            switch (lowc(*op)) {
            case 'f': /* female */
                wolfgender = 2;
                break;
            case 'm': /* male */
                wolfgender = 1;
                break;
            case 'n': /* neuter or none */
            case 'r': /* random */
            case '*': /* random */
                wolfgender = 0;
                break;
            default:
                config_error_add("Unrecognized wolf gender '%s'.", op);
                return FALSE;
                break;
            }
        }
        return retval;
    }

    fullname = "dogbreed";
    if (match_optname(opts, fullname, 8, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            int indx = 0;
            boolean breedfound = FALSE;
            for (indx = 0; indx < NUM_DOG_BREEDS; indx++)
            {
                if ((dog_breed_definitions[indx].name && !strcmpi(op, dog_breed_definitions[indx].name)) 
                    || (dog_breed_definitions[indx].description && !strcmpi(op, dog_breed_definitions[indx].description))
                    || (dog_breed_definitions[indx].breed_name && !strcmpi(op, dog_breed_definitions[indx].breed_name))
                    )
                {
                    breedfound = TRUE;
                    dogbreed = indx;
                    break;
                }
            }
            if (!breedfound)
            {
                if (!strcmpi(op, "labrador") || !strcmpi(op, "labrador retriever"))
                    dogbreed = DOG_BREED_LABRADOR_BLACK + rn2(DOG_BREED_LABRADOR_YELLOW - DOG_BREED_LABRADOR_BLACK + 1);
                else
                {
                    dogbreed = 0;
                    config_error_add("Unrecognized dog breed '%s'.", op);
                    return FALSE;
                }
            }
        }
        return retval;
    }

    fullname = "catbreed";
    if (match_optname(opts, fullname, 8, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if ((op = string_for_env_opt(fullname, opts, negated)) != 0) {
            int indx = 0;
            boolean breedfound = FALSE;
            for (indx = 0; indx < NUM_CAT_BREEDS; indx++)
            {
                if ((cat_breed_definitions[indx].name && !strcmpi(op, cat_breed_definitions[indx].name))
                    || (cat_breed_definitions[indx].description && !strcmpi(op, cat_breed_definitions[indx].description))
                    || (cat_breed_definitions[indx].breed_name && !strcmpi(op, cat_breed_definitions[indx].breed_name))
                    )
                {
                    breedfound = TRUE;
                    catbreed = indx;
                    break;
                }
            }
            if (!breedfound)
            {
                if (!strcmpi(op, "persian"))
                    catbreed = CAT_BREED_PERSIAN_GREY + rn2(CAT_BREED_PERSIAN_WHITE - CAT_BREED_PERSIAN_GREY + 1);
                else
                {
                    catbreed = 0;
                    config_error_add("Unrecognized cat breed '%s'.", op);
                    return FALSE;
                }
            }
        }
        return retval;
    }

    fullname = "mouse_support";
    if (match_optname(opts, fullname, 13, TRUE)) {
        boolean compat = (strlen(opts) <= 13);

        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, (compat || !initial));
        if (!op) {
            if (compat || negated || initial) {
                /* for backwards compatibility, "mouse_support" without a
                   value is a synonym for mouse_support:1 */
                iflags.wc_mouse_support = !negated;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        } else {
            int mode = atoi(op);

            if (mode < 0 || mode > 2 || (mode == 0 && *op != '0')) {
                config_error_add("Illegal %s parameter '%s'", fullname, op);
                return FALSE;
            } else { /* mode >= 0 */
                iflags.wc_mouse_support = mode;
            }
        }
        return retval;
    }

    fullname = "number_pad";
    if (match_optname(opts, fullname, 10, TRUE)) {
        boolean compat = (strlen(opts) <= 10);

        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, (compat || !initial));
        if (!op) {
            if (compat || negated || initial) {
                /* for backwards compatibility, "number_pad" without a
                   value is a synonym for number_pad:1 */
                iflags.num_pad = !negated;
                iflags.num_pad_mode = 0;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        } else {
            int mode = atoi(op);

            if (mode < -2 || mode > 4 || (mode == 0 && *op != '0')) {
                config_error_add("Illegal %s parameter '%s'", fullname, op);
                return FALSE;
            } else if (mode <= 0) {
                iflags.num_pad = FALSE;
                iflags.num_pad_mode = 0;

                /* German keyboard; y and z keys swapped */
                if (mode == -1)
                    iflags.num_pad_mode |= 1;
                else if(mode == -2)
                    iflags.num_pad_mode |= 4;

            } else {                              /* mode > 0 */
                iflags.num_pad = TRUE;
                iflags.num_pad_mode = 0;
                /* PC Hack / MSDOS compatibility */
                if (mode == 2 || mode == 4)
                    iflags.num_pad_mode |= 1;
                /* phone keypad layout */
                if (mode == 3 || mode == 4)
                    iflags.num_pad_mode |= 2;
            }
        }
        reset_commands(FALSE);
        number_pad(iflags.num_pad ? 1 : 0);
        return retval;
    }

    fullname = "roguesymset";
    if (match_optname(opts, fullname, 7, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_opt(opts, FALSE)) != 0) {
            symset[ROGUESET].name = dupstr(op);
            if (!read_sym_file(ROGUESET)) {
                clear_symsetentry(ROGUESET, TRUE);
                config_error_add(
                               "Unable to load symbol set \"%s\" from \"%s\"",
                                 op, SYMBOLS);
                return FALSE;
            } else {
                if (!initial && Is_really_rogue_level(&u.uz))
                    assign_graphics(ROGUESET);
                need_redraw = TRUE;
            }
        } else
            return FALSE;
        return retval;
    }

    fullname = "symset";
    if (match_optname(opts, fullname, 6, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_opt(opts, FALSE)) != 0) {
            symset[PRIMARY].name = dupstr(op);
            if (!read_sym_file(PRIMARY)) {
                clear_symsetentry(PRIMARY, TRUE);
                config_error_add(
                               "Unable to load symbol set \"%s\" from \"%s\"",
                                 op, SYMBOLS);
                return FALSE;
            } else {
                switch_symbols(symset[PRIMARY].name != (char *) 0);
                need_redraw = TRUE;
            }
        } else
            return FALSE;
        return retval;
    }

    fullname = "runmode";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            flags.runmode = RUN_TPORT;
        } else if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (!strncmpi(op, "teleport", strlen(op)))
                flags.runmode = RUN_TPORT;
            else if (!strncmpi(op, "run", strlen(op)))
                flags.runmode = RUN_LEAP;
            else if (!strncmpi(op, "walk", strlen(op)))
                flags.runmode = RUN_STEP;
            else if (!strncmpi(op, "crawl", strlen(op)))
                flags.runmode = RUN_CRAWL;
            else {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else
            return FALSE;
        return retval;
    }

    /* menucolor:"regex_string"=color */
    fullname = "menucolor";
    if (match_optname(opts, fullname, 9, TRUE)) {
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            if (!add_menu_coloring(op))
                return FALSE;
        } else
            return FALSE;
        return retval;
    }

    fullname = "msghistory";
    if (match_optname(opts, fullname, 3, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_env_opt(fullname, opts, negated);
        if ((negated && !op) || (!negated && op)) {
            iflags.msg_history = negated ? 0 : atoi(op);
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    fullname = "msg_window";
    /* msg_window:single, combo, full or reversed */
    if (match_optname(opts, fullname, 4, TRUE)) {
/* allow option to be silently ignored by non-tty ports */
#ifdef TTY_GRAPHICS
        int tmp;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (!(op = string_for_opt(opts, TRUE))) {
            tmp = negated ? 's' : 'f';
        } else {
            if (negated) {
                bad_negation(fullname, TRUE);
                return FALSE;
            }
            tmp = lowc(*op);
        }
        switch (tmp) {
        case 's': /* single message history cycle (default if negated) */
            iflags.prevmsg_window = 's';
            break;
        case 'c': /* combination: two singles, then full page */
            iflags.prevmsg_window = 'c';
            break;
        case 'f': /* full page (default if specified without argument) */
            iflags.prevmsg_window = 'f';
            break;
        case 'r': /* full page (reversed) */
            iflags.prevmsg_window = 'r';
            break;
        default:
            config_error_add("Unknown %s parameter '%s'", fullname, op);
            retval = FALSE;
        }
#endif
        return retval;
    }

    /* WINCAP
     * setting font options  */
    fullname = "font";
    if (!strncmpi(opts, fullname, 4)) {
        int opttype = -1;
        char *fontopts = opts + 4;

        if (!strncmpi(fontopts, "map", 3) || !strncmpi(fontopts, "_map", 4))
            opttype = MAP_OPTION;
        else if (!strncmpi(fontopts, "message", 7)
                 || !strncmpi(fontopts, "_message", 8))
            opttype = MESSAGE_OPTION;
        else if (!strncmpi(fontopts, "text", 4)
                 || !strncmpi(fontopts, "_text", 5))
            opttype = TEXT_OPTION;
        else if (!strncmpi(fontopts, "menu", 4)
                 || !strncmpi(fontopts, "_menu", 5))
            opttype = MENU_OPTION;
        else if (!strncmpi(fontopts, "status", 6)
                 || !strncmpi(fontopts, "_status", 7))
            opttype = STATUS_OPTION;
        else if (!strncmpi(fontopts, "_size", 5)) {
            if (!strncmpi(fontopts, "_size_map", 8))
                opttype = MAP_OPTION;
            else if (!strncmpi(fontopts, "_size_message", 12))
                opttype = MESSAGE_OPTION;
            else if (!strncmpi(fontopts, "_size_text", 9))
                opttype = TEXT_OPTION;
            else if (!strncmpi(fontopts, "_size_menu", 9))
                opttype = MENU_OPTION;
            else if (!strncmpi(fontopts, "_size_status", 11))
                opttype = STATUS_OPTION;
            else {
                config_error_add("Unknown %s parameter '%s'", fullname, opts);
                return FALSE;
            }
            if (duplicate)
                complain_about_duplicate(opts, 1);
            if (opttype > 0 && !negated
                && (op = string_for_opt(opts, FALSE)) != 0) {
                switch (opttype) {
                case MAP_OPTION:
                    iflags.wc_fontsiz_map = atoi(op);
                    break;
                case MESSAGE_OPTION:
                    iflags.wc_fontsiz_message = atoi(op);
                    break;
                case TEXT_OPTION:
                    iflags.wc_fontsiz_text = atoi(op);
                    break;
                case MENU_OPTION:
                    iflags.wc_fontsiz_menu = atoi(op);
                    break;
                case STATUS_OPTION:
                    iflags.wc_fontsiz_status = atoi(op);
                    break;
                }
            }
            return retval;
        } else {
            config_error_add("Unknown %s parameter '%s'", fullname, opts);
            return FALSE;
        }
        if (opttype > 0 && (op = string_for_opt(opts, FALSE)) != 0) {
            wc_set_font_name(opttype, op);
#ifdef MAC
            set_font_name(opttype, op);
#endif
            return retval;
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

#ifdef CHANGE_COLOR
    if (match_optname(opts, "palette", 3, TRUE)
#ifdef MAC
        || match_optname(opts, "hicolor", 3, TRUE)
#endif
        ) {
        int color_number, color_incr;

#if !(defined(WIN32) && !defined(GNH_WIN))
        if (duplicate)
            complain_about_duplicate(opts, 1);
#endif
#ifdef MAC
        if (match_optname(opts, "hicolor", 3, TRUE)) {
            if (negated) {
                bad_negation("hicolor", FALSE);
                return FALSE;
            }
            color_number = CLR_MAX + 4; /* HARDCODED inverse number */
            color_incr = -1;
        } else
#endif
        {
            if (negated) {
                bad_negation("palette", FALSE);
                return FALSE;
            }
            color_number = 0;
            color_incr = 1;
        }
#if defined(WIN32) && !defined(GNH_WIN)
        op = string_for_opt(opts, TRUE);
        if (!alternative_palette(op)) {
            config_error_add("Error in palette parameter '%s'", op);
            return FALSE;
        }
#else
        if ((op = string_for_opt(opts, FALSE)) != (char *) 0) {
            char *pt = op;
            int cnt, tmp, reverse;
            int64_t rgb;

            while (*pt && color_number >= 0) {
                cnt = 3;
                rgb = 0L;
                if (*pt == '-') {
                    reverse = 1;
                    pt++;
                } else {
                    reverse = 0;
                }
                while (cnt-- > 0) {
                    if (*pt && *pt != '/') {
#ifdef AMIGA
                        rgb <<= 4;
#else
                        rgb <<= 8;
#endif
                        tmp = *pt++;
                        if (isalpha((uchar) tmp)) {
                            tmp = (tmp + 9) & 0xf; /* Assumes ASCII... */
                        } else {
                            tmp &= 0xf; /* Digits in ASCII too... */
                        }
#ifndef AMIGA
                        /* Add an extra so we fill f -> ff and 0 -> 00 */
                        rgb += tmp << 4;
#endif
                        rgb += tmp;
                    }
                }
                if (*pt == '/')
                    pt++;
                change_color(color_number, rgb, reverse);
                color_number += color_incr;
            }
        }
#endif /* !WIN32 */
        if (!initial) {
            need_redraw = TRUE;
        }
        return retval;
    }
#endif /* CHANGE_COLOR */

    if (match_optname(opts, "fruit", 2, TRUE)) {
        struct fruit *forig = 0;
        char empty_str = '\0';

        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated || !initial);
        if (negated) {
            if (op) {
                bad_negation("fruit", TRUE);
                return FALSE;
            }
            op = &empty_str;
            goto goodfruit;
        }
        if (!op)
            return FALSE;
        /* 3.6.2: strip leading and trailing spaces, condense internal ones */
        mungspaces(op);
        if (!initial) {
            struct fruit *f;
            int fnum = 0;

            /* count number of named fruits; if 'op' is found among them,
               then the count doesn't matter because we won't be adding it */
            f = fruit_from_name(op, FALSE, &fnum);
            if (!f) {
                if (!flags.made_fruit)
                    forig = fruit_from_name(pl_fruit, FALSE, (int *) 0);

                if (!forig && fnum >= 100) {
                    config_error_add(
                             "Doing that so many times isn't very fruitful.");
                    return retval;
                }
            }
        }
 goodfruit:
        nmcpy(pl_fruit, op, PL_FSIZ);
        sanitize_name(pl_fruit);
        /* OBJ_NAME(objects[SLIME_MOLD]) won't work for this after
           initialization; it gets changed to generic "fruit" */
        if (!*pl_fruit)
            nmcpy(pl_fruit, "slime mold", PL_FSIZ);
        if (!initial) {
            /* if 'forig' is nonNull, we replace it rather than add
               a new fruit; it can only be nonNull if no fruits have
               been created since the previous name was put in place */
            (void) fruitadd(pl_fruit, forig);
            pline("Fruit is now \"%s\".", pl_fruit);
        }
        /* If initial, then process_options_file is allowed to do it instead
         * of here (process_options_file always has to do it even if there's
         * no fruit option at all.  Also, we don't want people
         * setting multiple fruits in their options.)
         */
        return retval;
    }

    fullname = "whatis_coord";
    if (match_optname(opts, fullname, 8, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            iflags.getpos_coords = GPCOORDS_NONE;
            return retval;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            static const char gpcoords[] = { GPCOORDS_NONE, GPCOORDS_COMPASS,
                                       GPCOORDS_COMFULL, GPCOORDS_MAP,
                                       GPCOORDS_SCREEN, '\0' };
            char c = lowc(*op);

            if (c && index(gpcoords, c))
                iflags.getpos_coords = c;
            else {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else
            return FALSE;
        return retval;
    }

    fullname = "whatis_filter";
    if (match_optname(opts, fullname, 8, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            iflags.getloc_filter = GFILTER_NONE;
            return retval;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            char c = lowc(*op);

            switch (c) {
            case 'n':
                iflags.getloc_filter = GFILTER_NONE;
                break;
            case 'v':
                iflags.getloc_filter = GFILTER_VIEW;
                break;
            case 'a':
                iflags.getloc_filter = GFILTER_AREA;
                break;
            default: {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
            }
        } else
            return FALSE;
        return retval;
    }

    fullname = "warnings";
    if (match_optname(opts, fullname, 5, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        return warning_opts(opts, fullname);
    }

    /* boulder:symbol */
    fullname = "boulder";
    if (match_optname(opts, fullname, 7, TRUE)) {
#ifdef BACKWARD_COMPAT
        int clash = 0;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        /* if (!(opts = string_for_env_opt(fullname, opts, FALSE)))
         */
        if (!(opts = string_for_opt(opts, FALSE)))
            return FALSE;

        nhsym outstr[64] = { 0 };
        escapes_ex(opts, outstr, TRUE);
        /* note: dummy monclass #0 has symbol value '\0'; we allow that--
           attempting to set bouldersym to '^@'/'\0' will reset to default */
        if (outstr[0] < 128)
        {
            if (def_char_to_monclass((char)outstr[0]) != MAX_MONSTER_CLASSES)
                clash = outstr[0] ? 1 : 0;
            else if (outstr[0] >= '1' && outstr[0] < WARNCOUNT + '0')
                clash = 2;
        }
        if (clash) {
            /* symbol chosen matches a used monster or warning
               symbol which is not good - reject it */
            config_error_add(
            "Badoption - boulder symbol '%s' would conflict with a %s symbol",
                             visctrl((char)outstr[0]),
                             (clash == 1) ? "monster" : "warning");
        } else {
            /*
             * Override the default boulder symbol.
             */
            iflags.bouldersym = outstr[0];
            /* for 'initial', update_bouldersym() is done in
               finish_options(), after all symset options
               have been processed */
            if (!initial) {
                update_bouldersym();
                need_redraw = TRUE;
            }
        }
        return retval;
#else
        config_error_add("'%s' no longer supported; use S_boulder:c instead",
                         fullname);
        return FALSE;
#endif
    }

    /* name:string */
    fullname = "name";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            nmcpy(plname, op, PL_NSIZ);
        } else
            return FALSE;
        return retval;
    }

    /* altkeyhandler:string */
    fullname = "altkeyhandler";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_opt(opts, negated)) != 0) {
#if defined(WIN32) && defined(TTY_GRAPHICS)
            set_altkeyhandler(op);
#endif
        } else
            return FALSE;
        return retval;
    }

    /* WINCAP
     * align_status:[left|top|right|bottom] */
    fullname = "align_status";
    if (match_optname(opts, fullname, sizeof "align_status" - 1, TRUE)) {
        op = string_for_opt(opts, negated);
        if (op && !negated) {
            if (!strncmpi(op, "left", sizeof "left" - 1))
                iflags.wc_align_status = ALIGN_LEFT;
            else if (!strncmpi(op, "top", sizeof "top" - 1))
                iflags.wc_align_status = ALIGN_TOP;
            else if (!strncmpi(op, "right", sizeof "right" - 1))
                iflags.wc_align_status = ALIGN_RIGHT;
            else if (!strncmpi(op, "bottom", sizeof "bottom" - 1))
                iflags.wc_align_status = ALIGN_BOTTOM;
            else {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* WINCAP
     * align_message:[left|top|right|bottom] */
    fullname = "align_message";
    if (match_optname(opts, fullname, sizeof "align_message" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if (op && !negated) {
            if (!strncmpi(op, "left", sizeof "left" - 1))
                iflags.wc_align_message = ALIGN_LEFT;
            else if (!strncmpi(op, "top", sizeof "top" - 1))
                iflags.wc_align_message = ALIGN_TOP;
            else if (!strncmpi(op, "right", sizeof "right" - 1))
                iflags.wc_align_message = ALIGN_RIGHT;
            else if (!strncmpi(op, "bottom", sizeof "bottom" - 1))
                iflags.wc_align_message = ALIGN_BOTTOM;
            else {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* the order to list inventory */
    fullname = "packorder";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (!(op = string_for_opt(opts, FALSE)))
            return FALSE;

        if (!change_inv_order(op))
            return FALSE;
        return retval;
    }

    /* user can change required response for some prompts (quit, die, hit),
       or add an extra prompt (pray, Remove) that isn't ordinarily there */
    fullname = "paranoid_confirmation";
    if (match_optname(opts, fullname, 8, TRUE)) {
        /* at present we don't complain about duplicates for this
           option, but we do throw away the old settings whenever
           we process a new one [clearing old flags is essential
           for handling default paranoid_confirm:pray sanely] */
        flags.paranoia_bits = 0; /* clear all */
        if (negated) {
            flags.paranoia_bits = 0; /* [now redundant...] */
        } else if ((op = string_for_opt(opts, TRUE)) != 0) {
            char *pp, buf[BUFSZ];

            Strncpy(buf, op, sizeof buf - 1);
            buf[sizeof buf - 1] = '\0';
            op = mungspaces(buf);
            for (;;) {
                /* We're looking to parse
                   "paranoid_confirm:whichone wheretwo whothree"
                   and "paranoid_confirm:" prefix has already
                   been stripped off by the time we get here */
                pp = index(op, ' ');
                if (pp)
                    *pp = '\0';
                /* we aren't matching option names but match_optname()
                   does what we want once we've broken the space
                   delimited aggregate into separate tokens */
                for (i = 0; i < SIZE(paranoia); ++i) {
                    if (match_optname(op, paranoia[i].argname,
                                      paranoia[i].argMinLen, FALSE)
                        || (paranoia[i].synonym
                            && match_optname(op, paranoia[i].synonym,
                                             (size_t)paranoia[i].synMinLen, FALSE))) {
                        if (paranoia[i].flagmask)
                            flags.paranoia_bits |= paranoia[i].flagmask;
                        else /* 0 == "none", so clear all */
                            flags.paranoia_bits = 0;
                        break;
                    }
                }
                if (i == SIZE(paranoia)) {
                    /* didn't match anything, so arg is bad;
                       any flags already set will stay set */
                    config_error_add("Unknown %s parameter '%s'",
                                     fullname, op);
                    return FALSE;
                }
                /* move on to next token */
                if (pp)
                    op = pp + 1;
                else
                    break; /* no next token */
            } /* for(;;) */
        } else
            return FALSE;
        return retval;
    }

    /* accept deprecated boolean; superseded by paranoid_confirm:pray */
    fullname = "prayconfirm";
    if (match_optname(opts, fullname, 4, FALSE)) {
        if (negated)
            flags.paranoia_bits &= ~PARANOID_PRAY;
        else
            flags.paranoia_bits |= PARANOID_PRAY;
        return retval;
    }

    /* maximum burden picked up before prompt (Warren Cheung) */
    fullname = "pickup_burden";
    if (match_optname(opts, fullname, 8, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            switch (lowc(*op)) {
            case 'u': /* Unencumbered */
                flags.pickup_burden = UNENCUMBERED;
                break;
            case 'b': /* Burdened (slight encumbrance) */
                flags.pickup_burden = SLT_ENCUMBER;
                break;
            case 's': /* streSsed (moderate encumbrance) */
                flags.pickup_burden = MOD_ENCUMBER;
                break;
            case 'n': /* straiNed (heavy encumbrance) */
                flags.pickup_burden = HVY_ENCUMBER;
                break;
            case 'o': /* OverTaxed (extreme encumbrance) */
            case 't':
                flags.pickup_burden = EXT_ENCUMBER;
                break;
            case 'l': /* overLoaded */
                flags.pickup_burden = OVERLOADED;
                break;
            default:
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else
            return FALSE;
        return retval;
    }

    /* types of objects to pick up automatically */
    fullname = "pickup_types";
    if (match_optname(opts, fullname, 8, TRUE)) {
        char ocl[MAX_OBJECT_CLASSES + 1], tbuf[MAX_OBJECT_CLASSES + 1],
             qbuf[QBUFSZ], abuf[BUFSZ];
        int oc_sym;
        boolean badopt = FALSE, compat = (strlen(opts) <= 6), use_menu;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        oc_to_str(flags.pickup_types, tbuf);
        flags.pickup_types[0] = '\0'; /* all */
        op = string_for_opt(opts, (compat || !initial));
        if (!op) {
            if (compat || negated || initial) {
                /* for backwards compatibility, "pickup" without a
                   value is a synonym for autopickup of all types
                   (and during initialization, we can't prompt yet) */
                flags.pickup = !negated;
                return retval;
            }
            oc_to_str(flags.inv_order, ocl);
            use_menu = TRUE;
            if (flags.menu_style == MENU_TRADITIONAL
                || flags.menu_style == MENU_COMBINATION) {
                boolean wasspace;

                use_menu = FALSE;
                Sprintf(qbuf, "New %s: [%s am] (%s)", fullname, ocl,
                        *tbuf ? tbuf : "all");
                abuf[0] = '\0';
                getlin(qbuf, abuf);
                wasspace = (abuf[0] == ' '); /* before mungspaces */
                op = mungspaces(abuf);
                if (wasspace && !abuf[0])
                    ; /* one or more spaces will remove old value */
                else if (!abuf[0] || abuf[0] == '\033')
                    op = tbuf; /* restore */
                else if (abuf[0] == 'm')
                    use_menu = TRUE;
                /* note: abuf[0]=='a' is already handled via clearing the
                   the old value (above) as a default action */
            }
            if (use_menu) {
                if (wizard && !index(ocl, VENOM_SYM))
                    strkitten(ocl, VENOM_SYM);
                (void) choose_classes_menu("Autopickup what?", 1, TRUE, ocl,
                                           tbuf);
                op = tbuf;
            }
        }
        if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        while (*op == ' ')
            op++;
        if (*op != 'a' && *op != 'A') {
            num = 0;
            while (*op) {
                oc_sym = def_char_to_objclass(*op);
                /* make sure all are valid obj symbols occurring once */
                if (oc_sym != MAX_OBJECT_CLASSES
                    && !index(flags.pickup_types, oc_sym)) {
                    flags.pickup_types[num] = (char) oc_sym;
                    flags.pickup_types[++num] = '\0';
                } else
                    badopt = TRUE;
                op++;
            }
            if (badopt) {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        }
        return retval;
    }

    /* pile limit: when walking over objects, number which triggers
       "there are several/many objects here" instead of listing them */
    fullname = "pile_limit";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if ((negated && !op) || (!negated && op))
            flags.pile_limit = negated ? 0 : atoi(op);
        else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        } else /* !op */
            flags.pile_limit = PILE_LIMIT_DFLT;
        /* sanity check */
        if (flags.pile_limit < 0)
            flags.pile_limit = PILE_LIMIT_DFLT;
        return retval;
    }

    /* play mode: normal, explore/discovery, or debug/wizard */
    fullname = "playmode";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated)
            bad_negation(fullname, FALSE);
        if (duplicate || negated)
            return FALSE;
        op = string_for_opt(opts, FALSE);
        if (!op)
            return FALSE;
        if (!strncmpi(op, "normal", 6) || !strncmpi(op, "classic", 7) || !strcmpi(op, "play")) {
            wizard = discover = CasualMode = ModernMode = FALSE;
        } else if (!strncmpi(op, "explore", 6)
                   || !strncmpi(op, "discovery", 6)) {
            wizard = CasualMode = ModernMode = FALSE, discover = TRUE;
        } else if (!strncmpi(op, "debug", 5) || !strncmpi(op, "wizard", 6)) {
            wizard = TRUE, discover = CasualMode = ModernMode = FALSE;
        } else if (!strncmpi(op, "casual", 6)) {
            CasualMode = TRUE, ModernMode = TRUE, wizard = FALSE, discover = FALSE;
        } else if (!strncmpi(op, "modern", 6)) {
            CasualMode = FALSE, ModernMode = TRUE, wizard = FALSE, discover = FALSE;
        } else if (!strncmpi(op, "reloadable", 10)) {
            CasualMode = TRUE, ModernMode = FALSE, wizard = FALSE, discover = FALSE;
        } else {
            config_error_add("Invalid value for \"%s\":%s", fullname, op);
            return FALSE;
        }
        return retval;
    }

    /* WINCAP
     * player_selection: dialog | prompt/prompts/prompting */
    fullname = "player_selection";
    if (match_optname(opts, fullname, sizeof "player_selection" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if (op && !negated) {
            if (!strncmpi(op, "dialog", sizeof "dialog" - 1)) {
                iflags.wc_player_selection = VIA_DIALOG;
            } else if (!strncmpi(op, "prompt", sizeof "prompt" - 1)) {
                iflags.wc_player_selection = VIA_PROMPTS;
            } else {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* things to disclose at end of game */
    fullname = "disclose";
    if (match_optname(opts, fullname, 7, TRUE)) {
        /*
         * The order that the end_disclose options are stored:
         *      inventory, attribs, vanquished, genocided,
         *      conduct, overview.
         * There is an array in flags:
         *      end_disclose[NUM_DISCLOSURE_OPT];
         * with option settings for the each of the following:
         * iagvc [see disclosure_options in decl.c]:
         * Allowed setting values in that array are:
         *      DISCLOSE_PROMPT_DEFAULT_YES  ask with default answer yes
         *      DISCLOSE_PROMPT_DEFAULT_NO   ask with default answer no
         *      DISCLOSE_YES_WITHOUT_PROMPT  always disclose and don't ask
         *      DISCLOSE_NO_WITHOUT_PROMPT   never disclose and don't ask
         *      DISCLOSE_PROMPT_DEFAULT_SPECIAL  for 'vanquished' only...
         *      DISCLOSE_SPECIAL_WITHOUT_PROMPT  ...to set up sort order.
         *
         * Those setting values can be used in the option
         * string as a prefix to get the desired behaviour.
         *
         * For backward compatibility, no prefix is required,
         * and the presence of a i,a,g,v, or c without a prefix
         * sets the corresponding value to DISCLOSE_YES_WITHOUT_PROMPT.
         */
        int idx, prefix_val;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, TRUE);
        if (op && negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        /* "disclose" without a value means "all with prompting"
           and negated means "none without prompting" */
        if (!op || !strcmpi(op, "all") || !strcmpi(op, "none")) {
            if (op && !strcmpi(op, "none"))
                negated = TRUE;
            for (num = 0; num < NUM_DISCLOSURE_OPTIONS; num++)
                flags.end_disclose[num] = negated
                                              ? DISCLOSE_NO_WITHOUT_PROMPT
                                              : DISCLOSE_PROMPT_DEFAULT_YES;
            return retval;
        }

        num = 0;
        prefix_val = -1;
        while (*op && num < sizeof flags.end_disclose - 1) {
            static const char valid_settings[] = {
                DISCLOSE_PROMPT_DEFAULT_YES, DISCLOSE_PROMPT_DEFAULT_NO,
                DISCLOSE_PROMPT_DEFAULT_SPECIAL,
                DISCLOSE_YES_WITHOUT_PROMPT, DISCLOSE_NO_WITHOUT_PROMPT,
                DISCLOSE_SPECIAL_WITHOUT_PROMPT, '\0'
            };
            register char c, *dop;

            c = lowc(*op);
            if (c == 'k')
                c = 'v'; /* killed -> vanquished */
            if (c == 'd')
                c = 'o'; /* dungeon -> overview */
            dop = index(disclosure_options, c);
            if (dop) {
                idx = (int) (dop - disclosure_options);
                if (idx < 0 || idx > NUM_DISCLOSURE_OPTIONS - 1) {
                    impossible("bad disclosure index %d %c", idx, c);
                    continue;
                }
                if (prefix_val != -1) {
                    if (*dop != 'v') {
                        if (prefix_val == DISCLOSE_PROMPT_DEFAULT_SPECIAL)
                            prefix_val = DISCLOSE_PROMPT_DEFAULT_YES;
                        if (prefix_val == DISCLOSE_SPECIAL_WITHOUT_PROMPT)
                            prefix_val = DISCLOSE_YES_WITHOUT_PROMPT;
                    }
                    flags.end_disclose[idx] = prefix_val;
                    prefix_val = -1;
                } else
                    flags.end_disclose[idx] = DISCLOSE_YES_WITHOUT_PROMPT;
            } else if (index(valid_settings, c)) {
                prefix_val = c;
            } else if (c == ' ') {
                ; /* do nothing */
            } else {
                config_error_add("Unknown %s parameter '%c'", fullname, *op);
                return FALSE;
            }
            op++;
        }
        return retval;
    }

    /* scores:5t[op] 5a[round] o[wn] */
    fullname = "scores";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        }
        if (!(op = string_for_opt(opts, FALSE)))
            return FALSE;

        while (*op) {
            int inum = 1;

            if (digit(*op)) {
                inum = atoi(op);
                while (digit(*op))
                    op++;
            } else if (*op == '!') {
                negated = !negated;
                op++;
            }
            while (*op == ' ')
                op++;

            switch (*op) {
            case 't':
            case 'T':
                flags.end_top = inum;
                break;
            case 'a':
            case 'A':
                flags.end_around = inum;
                break;
            case 'o':
            case 'O':
                flags.end_own = !negated;
                break;
            default:
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
            while (letter(*++op) || *op == ' ')
                continue;
            if (*op == '/')
                op++;
        }
        return retval;
    }

    fullname = "sortloot";
    if (match_optname(opts, fullname, 4, TRUE)) {
        op = string_for_env_opt(fullname, opts, FALSE);
        if (op) {
            char c = lowc(*op);

            switch (c) {
            case 'n': /* none */
            case 'l': /* loot (pickup) */
            case 'f': /* full (pickup + invent) */
                flags.sortloot = c;
                break;
            default:
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else
            return FALSE;
        return retval;
    }

    fullname = "suppress_alert";
    if (match_optname(opts, fullname, 4, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (op)
            (void) feature_alert_opts(op, fullname);
        return retval;
    }

#ifdef VIDEOSHADES
    /* videocolors:string */
    fullname = "videocolors";
    if (match_optname(opts, fullname, 6, TRUE)
        || match_optname(opts, "videocolours", 10, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (!(opts = string_for_env_opt(fullname, opts, FALSE))) {
            return FALSE;
        }
        if (!assign_videocolors(opts)) {
            config_error_add("Unknown error handling '%s'", fullname);
            return FALSE;
        }
        return retval;
    }
    /* videoshades:string */
    fullname = "videoshades";
    if (match_optname(opts, fullname, 6, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (!(opts = string_for_env_opt(fullname, opts, FALSE))) {
            return FALSE;
        }
        if (!assign_videoshades(opts)) {
            config_error_add("Unknown error handling '%s'", fullname);
            return FALSE;
        }
        return retval;
    }
#endif /* VIDEOSHADES */

#ifdef MSDOS
#ifdef NO_TERMS
    /* video:string -- must be after longer tests */
    fullname = "video";
    if (match_optname(opts, fullname, 5, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (!(opts = string_for_env_opt(fullname, opts, FALSE))) {
            return FALSE;
        }
        if (!assign_video(opts)) {
            config_error_add("Unknown error handling '%s'", fullname);
            return FALSE;
        }
        return retval;
    }
#endif /* NO_TERMS */
    /* soundcard:string -- careful not to match boolean 'sound' */
    fullname = "soundcard";
    if (match_optname(opts, fullname, 6, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (!(opts = string_for_env_opt(fullname, opts, FALSE))) {
            return FALSE;
        }
        if (!assign_soundcard(opts)) {
            config_error_add("Unknown error handling '%s'", fullname);
            return FALSE;
        }
        return retval;
    }
#endif /* MSDOS */

    /* WINCAP
     *
     *  map_mode:[tiles|ascii4x6|ascii6x8|ascii8x8|ascii16x8|ascii7x12
     *            |ascii8x12|ascii16x12|ascii12x16|ascii10x18|fit_to_screen]
     */
    fullname = "map_mode";
    if (match_optname(opts, fullname, sizeof "map_mode" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if (op && !negated) {
            if (!strncmpi(op, "tiles", sizeof "tiles" - 1))
                iflags.wc_map_mode = MAP_MODE_TILES;
            else if (!strncmpi(op, "ascii4x6", sizeof "ascii4x6" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII4x6;
            else if (!strncmpi(op, "ascii6x8", sizeof "ascii6x8" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII6x8;
            else if (!strncmpi(op, "ascii8x8", sizeof "ascii8x8" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII8x8;
            else if (!strncmpi(op, "ascii16x8", sizeof "ascii16x8" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII16x8;
            else if (!strncmpi(op, "ascii7x12", sizeof "ascii7x12" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII7x12;
            else if (!strncmpi(op, "ascii8x12", sizeof "ascii8x12" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII8x12;
            else if (!strncmpi(op, "ascii16x12", sizeof "ascii16x12" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII16x12;
            else if (!strncmpi(op, "ascii12x16", sizeof "ascii12x16" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII12x16;
            else if (!strncmpi(op, "ascii10x18", sizeof "ascii10x18" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII10x18;
            else if (!strncmpi(op, "fit_to_screen",
                               sizeof "fit_to_screen" - 1))
                iflags.wc_map_mode = MAP_MODE_ASCII_FIT_TO_SCREEN;
            else {
                config_error_add("Unknown %s parameter '%s'", fullname, op);
                return FALSE;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* WINCAP
     * scroll_amount:nn */
    fullname = "scroll_amount";
    if (match_optname(opts, fullname, sizeof "scroll_amount" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if ((negated && !op) || (!negated && op)) {
            iflags.wc_scroll_amount = negated ? 1 : atoi(op);
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* WINCAP
     * scroll_margin:nn */
    fullname = "scroll_margin";
    if (match_optname(opts, fullname, sizeof "scroll_margin" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if ((negated && !op) || (!negated && op)) {
            iflags.wc_scroll_margin = negated ? 5 : atoi(op);
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    fullname = "subkeyvalue";
    if (match_optname(opts, fullname, 5, TRUE)) {
        /* no duplicate complaint here */
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
#if defined(WIN32)
        } else {
            op = string_for_opt(opts, 0);
            if (!op)
                return FALSE;
#ifdef TTY_GRAPHICS
            map_subkeyvalue(op);
#endif
#endif
        }
        return retval;
    }

    /* WINCAP
     * tile_width:nn */
    fullname = "tile_width";
    if (match_optname(opts, fullname, sizeof "tile_width" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if ((negated && !op) || (!negated && op)) {
            iflags.wc_tile_width = negated ? 0 : atoi(op);
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* WINCAP
     * tile_file:name */
    fullname = "tile_file";
    fullname2 = "tile_file_1";
    if (match_optname(opts, fullname, sizeof "tile_file" - 1, TRUE) || match_optname(opts, fullname2, sizeof "tile_file_1" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (iflags.wc_tile_file[0])
                free(iflags.wc_tile_file[0]);
            iflags.wc_tile_file[0] = dupstr(op);
        } else
            return FALSE;
        return retval;
    }

    int tile_sheet_idx;
    for(tile_sheet_idx = 1; tile_sheet_idx < MAX_TILE_SHEETS; tile_sheet_idx++)
    {
        char fullnamebuf[BUFSZ] = "";
        fullname = "tile_file";
        Sprintf(fullnamebuf, "%s_%d", fullname, tile_sheet_idx + 1);

        if (match_optname(opts, fullnamebuf, strlen(fullnamebuf) - 1, TRUE)) {
            if (duplicate)
                complain_about_duplicate(opts, 1);
            if ((op = string_for_opt(opts, FALSE)) != 0) {
                if (iflags.wc_tile_file[tile_sheet_idx])
                    free(iflags.wc_tile_file[tile_sheet_idx]);
                iflags.wc_tile_file[tile_sheet_idx] = dupstr(op);
            }
            else
                return FALSE;
            return retval;
        }
    }

    /* WINCAP
     * tile_height:nn */
    fullname = "tile_height";
    if (match_optname(opts, fullname, sizeof "tile_height" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if ((negated && !op) || (!negated && op)) {
            iflags.wc_tile_height = negated ? 0 : atoi(op);
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    /* WINCAP
     * vary_msgcount:nn */
    fullname = "vary_msgcount";
    if (match_optname(opts, fullname, sizeof "vary_msgcount" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, negated);
        if ((negated && !op) || (!negated && op)) {
            iflags.wc_vary_msgcount = negated ? 0 : atoi(op);
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

    fullname = "master_bank_file";
    if (match_optname(opts, fullname, sizeof "master_bank_file" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (iflags.wc2_master_bank_file)
                free(iflags.wc2_master_bank_file);
            iflags.wc2_master_bank_file = dupstr(op);
        } else
            return FALSE;
        return retval;
    }

    fullname = "master_strings_bank_file";
    if (match_optname(opts, fullname, sizeof "master_strings_bank_file" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (iflags.wc2_master_strings_bank_file)
                free(iflags.wc2_master_strings_bank_file);
            iflags.wc2_master_strings_bank_file = dupstr(op);
        }
        else
            return FALSE;
        return retval;
    }

    fullname = "auxiliary_bank_file";
    if (match_optname(opts, fullname, sizeof "auxiliary_bank_file" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (iflags.wc2_auxiliary_bank_file)
                free(iflags.wc2_auxiliary_bank_file);
            iflags.wc2_auxiliary_bank_file = dupstr(op);
        }
        else
            return FALSE;
        return retval;
    }

    fullname = "intro_bank_file";
    if (match_optname(opts, fullname, sizeof "intro_bank_file" - 1, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (iflags.wc2_intro_bank_file)
                free(iflags.wc2_intro_bank_file);
            iflags.wc2_intro_bank_file = dupstr(op);
        }
        else
            return FALSE;
        return retval;
    }

    /*
     * windowtype:  option to choose the interface for binaries built
     * with support for more than one interface (tty + X11, for instance).
     *
     * Ideally, 'windowtype' should be processed first, because it
     * causes the wc_ and wc2_ flags to be set up.
     * For user, making it be first in a config file is trivial, use
     * OPTIONS=windowtype:Foo
     * as the first non-comment line of the file.
     * Making it first in NETHACKOPTIONS requires it to be at the _end_
     * because comma-separated option strings are processed from right
     * to left.
     */
    fullname = "windowtype";
    if (match_optname(opts, fullname, 3, TRUE)) {
        if (iflags.windowtype_locked)
            return retval;
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            if (!iflags.windowtype_deferred) {
                char buf[WINTYPELEN];

                nmcpy(buf, op, WINTYPELEN);
                choose_windows(buf);
            } else {
                nmcpy(chosen_windowtype, op, WINTYPELEN);
            }
        } else
            return FALSE;
        return retval;
    }

#ifdef WINCHAIN
    fullname = "windowchain";
    if (match_optname(opts, fullname, 3, TRUE)) {
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if ((op = string_for_env_opt(fullname, opts, FALSE)) != 0) {
            char buf[WINTYPELEN];

            nmcpy(buf, op, WINTYPELEN);
            addto_windowchain(buf);
        } else
            return FALSE;
        return retval;
    }
#endif

    /* WINCAP
     * setting window colors
     * syntax: windowcolors=menu foregrnd/backgrnd text foregrnd/backgrnd
     */
    fullname = "windowcolors";
    if (match_optname(opts, fullname, 7, TRUE)) {
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if ((op = string_for_opt(opts, FALSE)) != 0) {
            if (!wc_set_window_colors(op)) {
                config_error_add("Could not set %s '%s'", fullname, op);
                return FALSE;
            }
        } else if (negated) {
            bad_negation(fullname, TRUE);
            return FALSE;
        }
        return retval;
    }

#ifdef CURSES_GRAPHICS
    /* WINCAP2
     * term_cols:amount or term_rows:amount */
    fullname = "term_cols";
    if (match_optname(opts, fullname, 8, TRUE)
        /* alternate spelling */
        || match_optname(opts, "term_columns", 9, TRUE)
        /* different option but identical handlng */
        || (fullname = "term_rows", match_optname(opts, fullname, 8, TRUE))) {
        int64_t ltmp;

        op = string_for_opt(opts, negated);
        ltmp = atol(op);
        if (negated) {
            bad_negation(fullname, FALSE);
            retval = FALSE;

        /* this just checks atol() sanity, not logical window size sanity */
        } else if (ltmp <= 0L || ltmp >= (int64_t) LARGEST_INT) {
            config_error_add("Invalid %s: %ld", fullname, ltmp);
            retval = FALSE;

        } else {
            if (!strcmp(fullname, "term_rows"))
                iflags.wc2_term_rows = (int) ltmp;
            else /* !strcmp(fullname, "term_cols") */
                iflags.wc2_term_cols = (int) ltmp;
        }
        return retval;
    }

    /* WINCAP2
     * petattr:string */
    fullname = "petattr";
    if (match_optname(opts, fullname, sizeof "petattr" - 1, TRUE)) {
        op = string_for_opt(opts, negated);
        if (op && negated) {
            bad_negation(fullname, TRUE);
            retval = FALSE;
        } else if (op) {
#ifdef CURSES_GRAPHICS
            int itmp = curses_read_attrs(op);

            if (itmp == -1) {
                config_error_add("Unknown %s parameter '%s'", fullname, opts);
                retval = FALSE;
            } else
                iflags.wc2_petattr = itmp;
#else
            /* non-curses windowports will not use this flag anyway
             * but the above will not compile if we don't have curses.
             * Just set it to a sensible default: */
            iflags.wc2_petattr = ATR_INVERSE;
#endif
        } else if (negated) {
            iflags.wc2_petattr = ATR_NONE;
        }
        if (retval) {
            iflags.hilite_pet = (iflags.wc2_petattr != ATR_NONE);
            if (!initial)
                need_redraw = TRUE;
        }
        return retval;
    }

    /* WINCAP2
     * windowborders:n */
    fullname = "windowborders";
    if (match_optname(opts, fullname, 10, TRUE)) {
        op = string_for_opt(opts, negated);
        if (negated && op) {
            bad_negation(fullname, TRUE);
            retval = FALSE;
        } else {
            int itmp;

            if (negated)
                itmp = 0; /* Off */
            else if (!op)
                itmp = 1; /* On */
            else    /* Value supplied; expect 0 (off), 1 (on), or 2 (auto) */
                itmp = atoi(op);

            if (itmp < 0 || itmp > 2) {
                config_error_add("Invalid %s (should be 0, 1, or 2): %s",
                                 fullname, opts);
                retval = FALSE;
            } else {
                iflags.wc2_windowborders = itmp;
            }
        }
        return retval;
    }
#endif /* CURSES_GRAPHICS */

    /* WINCAP2
     * statuslines:n */
    fullname = "statuslines";
    if (match_optname(opts, fullname, 11, TRUE)) {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated) {
            bad_negation(fullname, TRUE);
            itmp = 2;
            retval = FALSE;
        } else if (op) {
            itmp = atoi(op);
        }
        if (itmp < 2 || itmp > 8) {
            config_error_add("'%s' requires a value between 2 and 8", fullname);
            retval = FALSE;
        } else {
            iflags.wc2_statuslines = itmp;
            if (!initial)
            {
                need_status_initialize = TRUE;
                need_redraw = TRUE;
            }
        }
        return retval;
    }

    fullname = "here_window_size";
    if (match_optname(opts, fullname, 16, TRUE)) {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated) {
            bad_negation(fullname, TRUE);
            itmp = HERE_WIN_SIZ_DFLT;
            retval = FALSE;
        }
        else if (op) {
            itmp = atoi(op);
        }
        if (itmp < 2 || itmp > 20) {
            config_error_add("'%s' requires a value between 2 and 20", fullname);
            retval = FALSE;
        }
        else {
            iflags.wc2_here_window_size = itmp;
            if (!initial)
            {
                need_redraw = TRUE;
            }
        }
        return retval;
    }

    fullname = "preferred_screen_scale";
    if (match_optname(opts, fullname, 22, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op) 
        {
            itmp = atoi(op);
        }

        if (itmp < MIN_PREF_SCREEN_SCALE || itmp > MAX_PREF_SCREEN_SCALE)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, MIN_PREF_SCREEN_SCALE, MAX_PREF_SCREEN_SCALE);
            retval = FALSE;
        }
        else 
        {
            flags.preferred_screen_scale = itmp;
        }
        return retval;
    }

    fullname = "shield_effect_length";
    if (match_optname(opts, fullname, 20, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = GNH_DEF_SHIELD_COUNT;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 1 || itmp > SHIELD_COUNT)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 1, SHIELD_COUNT);
            retval = FALSE;
        }
        else
        {
            flags.shield_effect_length = itmp;
        }
        return retval;
    }

    fullname = "talk_effect_length";
    if (match_optname(opts, fullname, 18, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = GNH_DEF_TALK_COUNT;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 1 || itmp > TALK_COUNT)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 1, TALK_COUNT);
            retval = FALSE;
        }
        else
        {
            flags.talk_effect_length = itmp;
        }
        return retval;
    }

    fullname = "last_item_show_duration";
    if (match_optname(opts, fullname, 23, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = GNH_DEF_TALK_COUNT;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 1 || itmp > MAX_LAST_ITEM_SHOW_DURATION)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 1, MAX_LAST_ITEM_SHOW_DURATION);
            retval = FALSE;
        }
        else
        {
            flags.talk_effect_length = itmp;
        }
        return retval;
    }

    /* This is in fact milliseconds in delay_output, which slows down the game, not just any animation */
    fullname = "animation_interval";
    if (match_optname(opts, fullname, 17, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 0;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 999)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 999);
            retval = FALSE;
        }
        else
        {
            flags.animation_frame_interval_in_milliseconds = itmp;
            need_set_animation_timer_interval = TRUE;
        }
        return retval;
    }

    fullname = "move_interval";
    if (match_optname(opts, fullname, 13, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 0;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 999)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 999);
            retval = FALSE;
        }
        else
        {
            flags.move_interval_in_milliseconds = itmp;
        }
        return retval;
    }

    fullname = "crawl_interval";
    if (match_optname(opts, fullname, 14, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 0;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 999)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 999);
            retval = FALSE;
        }
        else
        {
            flags.crawl_interval_in_milliseconds = itmp;
        }
        return retval;
    }

    fullname = "sound_volume_general";
    if (match_optname(opts, fullname, 20, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 100)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 100);
            retval = FALSE;
        }
        else
        {
            flags.sound_volume_general = itmp;
            need_set_sound_volume = TRUE;
        }
        return retval;
    }

    fullname = "sound_volume_music";
    if (match_optname(opts, fullname, 18, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 100)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 100);
            retval = FALSE;
        }
        else
        {
            flags.sound_volume_music = itmp;
            need_set_sound_volume = TRUE;
        }
        return retval;
    }

    fullname = "sound_volume_ambient";
    if (match_optname(opts, fullname, 20, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 100)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 100);
            retval = FALSE;
        }
        else
        {
            flags.sound_volume_ambient = itmp;
            need_set_sound_volume = TRUE;
        }
        return retval;
    }

    fullname = "sound_volume_dialogue";
    if (match_optname(opts, fullname, 21, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 100)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 100);
            retval = FALSE;
        }
        else
        {
            flags.sound_volume_dialogue = itmp;
            need_set_sound_volume = TRUE;
        }
        return retval;
    }

    fullname = "sound_volume_effects";
    if (match_optname(opts, fullname, 20, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 100)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 100);
            retval = FALSE;
        }
        else
        {
            flags.sound_volume_effects = itmp;
            need_set_sound_volume = TRUE;
        }
        return retval;
    }

    fullname = "sound_volume_ui";
    if (match_optname(opts, fullname, 15, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 100;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < 0 || itmp > 100)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, 0, 100);
            retval = FALSE;
        }
        else
        {
            flags.sound_volume_ui = itmp;
            need_set_sound_volume = TRUE;
        }
        return retval;
    }

    fullname = "spellorder";
    if (match_optname(opts, fullname, 10, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = SORTBY_NONE;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < SORTBY_NONE || itmp > SORTBY_CURRENT)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, SORTBY_NONE, SORTBY_CURRENT);
            retval = FALSE;
        }
        else
        {
            flags.spellorder = itmp;
        }
        if(!initial && flags.spellorder > SORTBY_NONE) //Spellbook is empty during initial options, so need to sort after spells are recorded in the spell book
            sortspells();
        return retval;
    }

    fullname = "max_hint_difficulty";
    if (match_optname(opts, fullname, 19, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = MIN_DIFFICULTY_LEVEL;
            retval = FALSE;
        }
        else if (op)
        {
            itmp = atoi(op);
        }

        if (itmp < MIN_DIFFICULTY_LEVEL - 1 || itmp > MAX_DIFFICULTY_LEVEL)
        {
            config_error_add("'%s' requires a value between %d and %d", fullname, MIN_DIFFICULTY_LEVEL - 1, MAX_DIFFICULTY_LEVEL);
            retval = FALSE;
        }
        else
        {
            flags.max_hint_difficulty = itmp;
        }
        return retval;
    }

    fullname = "right_click_command";
    fullname2 = "middle_click_command";
    boolean is_middle = match_optname(opts, fullname2, 20, TRUE);
    if (is_middle || match_optname(opts, fullname, 19, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 0;
            retval = FALSE;
        }
        else if (op)
        {
            boolean found = FALSE;
            for (i = 0; i < SIZE(mouse_cmd_names); i++) {
                if (i >= 2 && i <= 2)
                    continue;
                if (!strcmp(op, mouse_cmd_names[i]))
                {
                    found = TRUE;
                    itmp = i;
                    break;
                }
            }
            if(!found)
                itmp = -1;
        }

        if (itmp < 0)
        {
            char valuesbuf[BUFSZ * 2] = "";
            for (i = 0; i < SIZE(mouse_cmd_names); i++) {
                if (i >= 2 && i <= 2)
                    continue;
                if (*valuesbuf)
                {
                    Strcat(valuesbuf, ", ");
                    if(i == SIZE(mouse_cmd_names) - 1)
                        Strcat(valuesbuf, "or ");
                }
                Strcat(valuesbuf, mouse_cmd_names[i]);
            }

            config_error_add("'%s' requires one of the following values: %s", is_middle ? fullname2 : fullname, valuesbuf);
            retval = FALSE;
        }
        else
        {
            if(is_middle)
                flags.middle_click_command = (uchar)itmp;
            else
                flags.right_click_command = (uchar)itmp;
        }
        return retval;
    }

    fullname = "run_spot_distance";
    if (match_optname(opts, fullname, 17, TRUE))
    {
        int itmp = 0;

        op = string_for_opt(opts, negated);
        if (negated)
        {
            bad_negation(fullname, TRUE);
            itmp = 0;
            retval = FALSE;
        }
        else if (op)
        {
            if (!*op || *op == 'o' || *op == 'O') /* suppose this means off; 0 means the character never stops upon seeing somebody */
                itmp = 0;
            else if (*op == 'i' || *op == 'I')  /* suppose this means infinity, so the character will always stop */
                itmp = -1;
            else if (*op != '-' && *op != '+' && *op != ' ' && !(*op >= '0' && *op <= '9')) /* invalid */
                itmp = -2;
            else
                itmp = atoi(op);
        }

        if (itmp < -1 || itmp > 999)
        {
            config_error_add("'%s' requires a value between %d and %d (-1 is infinity)", fullname, -1, 999);
            retval = FALSE;
        }
        else
        {
            iflags.run_spot_distance = itmp;
        }
        return retval;
    }


    /* menustyle:traditional or combination or full or partial */
    fullname = "menustyle";
    if (match_optname(opts, fullname, 4, TRUE)) {
        int tmp;
        boolean val_required = (strlen(opts) > 5 && !negated);

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (!(op = string_for_opt(opts, !val_required))) {
            if (val_required)
                return FALSE; /* string_for_opt gave feedback */
            tmp = negated ? 'n' : 'f';
        } else {
            tmp = lowc(*op);
        }
        switch (tmp) {
        case 'n': /* none */
        case 't': /* traditional: prompt for class(es) by symbol,
                     prompt for each item within class(es) one at a time */
            flags.menu_style = MENU_TRADITIONAL;
            break;
        case 'c': /* combination: prompt for class(es) by symbol,
                     choose items within selected class(es) by menu */
            flags.menu_style = MENU_COMBINATION;
            break;
        case 'f': /* full: choose class(es) by first menu,
                     choose items within selected class(es) by second menu */
            flags.menu_style = MENU_FULL;
            break;
        case 'p': /* partial: skip class filtering,
                     choose items among all classes by menu */
            flags.menu_style = MENU_PARTIAL;
            break;
        default:
            config_error_add("Unknown %s parameter '%s'", fullname, op);
            return FALSE;
        }
        return retval;
    }

    fullname = "menu_headings";
    if (match_optname(opts, fullname, 12, TRUE)) {
        int tmpattr;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (negated) {
            bad_negation(fullname, FALSE);
            return FALSE;
        } else if (!(opts = string_for_env_opt(fullname, opts, FALSE))) {
            return FALSE;
        }
        tmpattr = match_str2attr(opts, TRUE);
        if (tmpattr == -1)
            return FALSE;
        else
            iflags.menu_headings = tmpattr;
        return retval;
    }

    /* check for menu command mapping */
    for (i = 0; i < SIZE(default_menu_cmd_info); i++) {
        fullname = default_menu_cmd_info[i].name;
        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (match_optname(opts, fullname, strlen(fullname), TRUE)) {
            if (negated) {
                bad_negation(fullname, FALSE);
                return FALSE;
            } else if ((op = string_for_opt(opts, FALSE)) != 0) {
                char c, op_buf[BUFSZ];

                escapes(op, op_buf);
                c = *op_buf;

                if (illegal_menu_cmd_key(c))
                    return FALSE;

                add_menu_cmd_alias(c, default_menu_cmd_info[i].cmd);
            }
            return retval;
        }
    }

    /* hilite fields in status prompt */
    fullname = "hilite_status";
    if (match_optname(opts, fullname, 13, TRUE)) {
#ifdef STATUS_HILITES
        if (duplicate)
            complain_about_duplicate(opts, 1);
        op = string_for_opt(opts, TRUE);
        if (op && negated) {
            clear_status_hilites();
            return retval;
        } else if (!op) {
            config_error_add("Value is mandatory for hilite_status");
            return FALSE;
        }
        if (!parse_status_hl1(op, tfrom_file))
            return FALSE;
        return retval;
#else
        config_error_add("'%s' is not supported", fullname);
        return FALSE;
#endif
    }

    /* control over whether highlights should be displayed, and for how long */
    fullname = "statushilites";
    if (match_optname(opts, fullname, 9, TRUE)) {
#ifdef STATUS_HILITES
        if (negated) {
            iflags.hilite_delta = 0L;
        } else {
            op = string_for_opt(opts, TRUE);
            iflags.hilite_delta = (!op || !*op) ? 3L : atol(op);
            if (iflags.hilite_delta < 0L)
                iflags.hilite_delta = 1L;
        }
        if (!tfrom_file)
            reset_status_hilites();
        return retval;
#else
        config_error_add("'%s' is not supported", fullname);
        return FALSE;
#endif
    }

    fullname = "DECgraphics";
    if (match_optname(opts, fullname, 3, TRUE)) {
#ifdef BACKWARD_COMPAT
        boolean badflag = FALSE;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (!negated) {
            /* There is no rogue level DECgraphics-specific set */
            if (symset[PRIMARY].name) {
                badflag = TRUE;
            } else {
                symset[PRIMARY].name = dupstr(fullname);
                if (!read_sym_file(PRIMARY)) {
                    badflag = TRUE;
                    clear_symsetentry(PRIMARY, TRUE);
                } else
                    switch_symbols(TRUE);
            }
            if (badflag) {
                config_error_add("Failure to load symbol set %s.", fullname);
                return FALSE;
            }
        }
        return retval;
#else
        config_error_add("'%s' no longer supported; use 'symset:%s' instead",
                         fullname, fullname);
        return FALSE;
#endif
    } /* "DECgraphics" */

    fullname = "IBMgraphics";
    if (match_optname(opts, fullname, 3, TRUE)) {
#ifdef BACKWARD_COMPAT
        const char *sym_name = fullname;
        boolean badflag = FALSE;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (!negated) {
            for (i = 0; i < NUM_GRAPHICS; ++i) {
                if (symset[i].name) {
                    badflag = TRUE;
                } else {
                    if (i == ROGUESET)
                        sym_name = "RogueIBM";
                    symset[i].name = dupstr(sym_name);
                    if (!read_sym_file(i)) {
                        badflag = TRUE;
                        clear_symsetentry(i, TRUE);
                        break;
                    }
                }
            }
            if (badflag) {
                config_error_add("Failure to load symbol set %s.", sym_name);
                return FALSE;
            } else {
                switch_symbols(TRUE);
                if (!initial && Is_really_rogue_level(&u.uz))
                    assign_graphics(ROGUESET);
            }
        }
        return retval;
#else
        config_error_add("'%s' no longer supported; use 'symset:%s' instead",
                         fullname, fullname);
        return FALSE;
#endif
    } /* "IBMgraphics" */

    fullname = "MACgraphics";
    if (match_optname(opts, fullname, 3, TRUE)) {
#if defined(MAC_GRAPHICS_ENV) && defined(BACKWARD_COMPAT)
        boolean badflag = FALSE;

        if (duplicate)
            complain_about_duplicate(opts, 1);
        if (!negated) {
            if (symset[PRIMARY].name) {
                badflag = TRUE;
            } else {
                symset[PRIMARY].name = dupstr(fullname);
                if (!read_sym_file(PRIMARY)) {
                    badflag = TRUE;
                    clear_symsetentry(PRIMARY, TRUE);
                }
            }
            if (badflag) {
                config_error_add("Failure to load symbol set %s.", fullname);
                return FALSE;
            } else {
                switch_symbols(TRUE);
                if (!initial && Is_really_rogue_level(&u.uz))
                    assign_graphics(ROGUESET);
            }
        }
        return retval;
#else   /* !(MAC_GRAPHICS_ENV && BACKWARD_COMPAT) */
        config_error_add("'%s' %s; use 'symset:%s' instead",
                         fullname,
#ifdef MAC_GRAPHICS_ENV /* implies BACKWARD_COMPAT is not defined */
                         "no longer supported",
#else
                         "is not supported",
#endif
                         fullname);
        return FALSE;
#endif  /* ?(MAC_GRAPHICS_ENV && BACKWARD_COMPAT) */
    } /* "MACgraphics" */

    /*
     * OK, if we still haven't recognized the option, check the boolean
     * options list.
     */
    for (i = 0; boolopt[i].name; i++)
    {
        if (match_optname(opts, boolopt[i].name, 3, TRUE))
        {
            /* options that don't exist */
            if (!boolopt[i].addr)
            {
                if (!initial && !negated)
                    pline_The("\"%s\" option is not available.",
                              boolopt[i].name);
                return retval;
            }
            /* options that must come from config file */
            if (!initial && (boolopt[i].optflags == SET_IN_FILE))
            {
                rejectoption(boolopt[i].name);
                return retval;
            }

            op = string_for_opt(opts, TRUE);

            if (op) 
            {
                if (negated) 
                {
                    config_error_add(
                           "Negated boolean '%s' should not have a parameter",
                                     boolopt[i].name);
                    return FALSE;
                }
                if (!strcmp(op, "true") || !strcmp(op, "yes"))
                {
                    negated = FALSE;
                } else if (!strcmp(op, "false") || !strcmp(op, "no"))
                {
                    negated = TRUE;
                } else {
                    config_error_add("Illegal parameter for a boolean");
                    return FALSE;
                }
            }

            *(boolopt[i].addr) = !negated;

            /* 0 means boolean opts */
            if (duplicate_opt_detection(boolopt[i].name, 0))
                complain_about_duplicate(boolopt[i].name, 0);
#ifdef RLECOMP
            if (boolopt[i].addr == &iflags.rlecomp)
                set_savepref(iflags.rlecomp ? "rlecomp" : "!rlecomp");
#endif
#ifdef ZEROCOMP
            if (boolopt[i].addr == &iflags.zerocomp)
                set_savepref(iflags.zerocomp ? "zerocomp" : "externalcomp");
#endif
            if (boolopt[i].addr == &iflags.wc_ascii_map) 
            {
                /* toggling ascii_map; set tiled_map to its opposite;
                   what does it mean to turn off ascii map if tiled map
                   isn't supported? -- right now, we do nothing */
                iflags.wc_tiled_map = negated;
            } else if (boolopt[i].addr == &iflags.wc_tiled_map) 
            {
                /* toggling tiled_map; set ascii_map to its opposite;
                   as with ascii_map, what does it mean to turn off tiled
                   map if ascii map isn't supported? */
                iflags.wc_ascii_map = negated;
            }
            /* only do processing below if setting with doset() */
            if (initial)
                return retval;

            if (boolopt[i].addr == &flags.time
                || boolopt[i].addr == &flags.showscore
                || boolopt[i].addr == &flags.showrealtime
                || boolopt[i].addr == &flags.showexp
                || boolopt[i].addr == &flags.showmove
                || boolopt[i].addr == &flags.show_weapon_style
                )
            {
                status_reassess();
                context.botl = TRUE;
            }
            else if(boolopt[i].addr == &flags.fullstatuslineorder)
            {
                (*windowprocs.win_status_init)(1);
                status_reassess();
                context.botl = TRUE;
            }
            else if (boolopt[i].addr == &flags.invlet_constant)
            {
                if (flags.invlet_constant) 
                {
                    reassign();
                    if (iflags.perm_invent)
                        need_redraw = TRUE;
                }
            } 
            else if (boolopt[i].addr == &flags.ibm2utf8)
            {
                    need_redraw = TRUE;
                    need_issue_gui_command = TRUE;
            }
            else if (boolopt[i].addr == &flags.self_click_action)
            {
                issue_boolean_gui_command(GUI_CMD_TOGGLE_CHARACTER_CLICK_ACTION, flags.self_click_action);
            }
            else if (boolopt[i].addr == &iflags.show_dice_as_ranges)
            {
                issue_boolean_gui_command(GUI_CMD_TOGGLE_DICE_AS_RANGES, iflags.show_dice_as_ranges);
            }
            else if (boolopt[i].addr == &flags.autodig)
            {
                issue_boolean_gui_command(GUI_CMD_TOGGLE_AUTODIG, flags.autodig);
            }
            else if (boolopt[i].addr == &flags.ignore_stopping)
            {
                issue_boolean_gui_command(GUI_CMD_TOGGLE_IGNORE_STOPPING, flags.ignore_stopping);
            }
            else if (boolopt[i].addr == &flags.classic_statue_symbol || boolopt[i].addr == &flags.classic_colors || boolopt[i].addr == &flags.show_decorations)
            {
                need_redraw = TRUE;
            }
            else if (boolopt[i].addr == &flags.show_grid || boolopt[i].addr == &flags.show_tile_mon_hp_bar || boolopt[i].addr == &flags.show_tile_pet_hp_bar || boolopt[i].addr == &flags.show_tile_u_hp_bar || boolopt[i].addr == &flags.show_buff_timer)
            {
                need_redraw = TRUE;
            }
            else if (boolopt[i].addr == &flags.show_weight_summary || boolopt[i].addr == &flags.inventory_weights_last || boolopt[i].addr == &flags.detailed_weights)
            {
                need_update_inventory = TRUE;
            }
            else if (boolopt[i].addr == &flags.rest_on_space)
            {
                need_update_space_binding = TRUE;
            }
            else if (boolopt[i].addr == &flags.lit_corridor
                       || boolopt[i].addr == &flags.dark_room)
            {
                /*
                 * All corridor squares seen via night vision or
                 * candles & lamps change.  Update them by calling
                 * newsym() on them.  Don't do this if we are
                 * initializing the options --- the vision system
                 * isn't set up yet.
                 */
                vision_recalc(2);       /* shut down vision */
                vision_full_recalc = 1; /* delayed recalc */
                if (iflags.use_color)
                    need_redraw = TRUE; /* darkroom refresh */
            } 
            else if (boolopt[i].addr == &flags.showrace
                       || boolopt[i].addr == &iflags.use_inverse
                       || boolopt[i].addr == &iflags.hilite_pile
                       || boolopt[i].addr == &iflags.perm_invent
#ifdef CURSES_GRAPHICS
                       || boolopt[i].addr == &iflags.cursesgraphics
#endif
                       || boolopt[i].addr == &iflags.wc_ascii_map
                       || boolopt[i].addr == &iflags.wc_tiled_map) 
            {
                need_redraw = TRUE;
            }
            else if (boolopt[i].addr == &iflags.wc2_herewindow)
            {
                need_redraw = TRUE;
                need_here_window = TRUE;
            }
            else if (boolopt[i].addr == &iflags.hilite_pet)
            {
#ifdef CURSES_GRAPHICS
                if (WINDOWPORT("curses")) 
                {
                    /* if we're enabling hilite_pet and petattr isn't set,
                       set it to Inverse; if we're disabling, leave petattr
                       alone so that re-enabling will get current value back */
                    if (iflags.hilite_pet && !iflags.wc2_petattr)
                        iflags.wc2_petattr = curses_read_attrs("I");
                }
#endif
                need_redraw = TRUE;
            } 
            else if (boolopt[i].addr == &iflags.wc2_hitpointbar)
            {
                status_reassess();
                need_redraw = TRUE;
#ifdef TEXTCOLOR
            }
            else if (boolopt[i].addr == &iflags.use_color) 
            {
                need_redraw = TRUE;
#ifdef TOS
                if (iflags.BIOS) 
                {
                    if (colors_changed)
                        restore_colors();
                    else
                        set_colors();
                }
#endif
#endif /* TEXTCOLOR */
            }
            return retval;
        }
    }

    /* Is it a symbol? */
    if (strstr(opts, "S_") == opts && parsesymbols(opts))
    {
        switch_symbols(TRUE);
        check_gold_symbol();
        return retval;
    }

    /* out of valid options */
    config_error_add("Unknown option '%s'", opts);
    return FALSE;
}

/* parse key:command */
boolean
parsebindings(bindings)
char* bindings;
{
    char *bind;
    char key;
    int i;
    //boolean ret = FALSE;

    /* break off first binding from the rest; parse the rest */
    if ((bind = index(bindings, ',')) != 0) {
        *bind++ = 0;
        /* ret |= */ (void)parsebindings(bind);
    }

    /* parse a single binding: first split around : */
    if (! (bind = index(bindings, ':')))
        return FALSE; /* it's not a binding */
    *bind++ = 0;

    /* read the key to be bound */
    key = txt2key(bindings);
    if (!key) {
        config_error_add("Unknown key binding key '%s'", bindings);
        return FALSE;
    }

    bind = trimspaces(bind);

    /* is it a special key? */
    if (bind_specialkey(key, bind))
        return TRUE;

    /* is it a menu command? */
    for (i = 0; i < SIZE(default_menu_cmd_info); i++) {
        if (!strcmp(default_menu_cmd_info[i].name, bind)) {
            if (illegal_menu_cmd_key(key)) {
                config_error_add("Bad menu key %s:%s", visctrl(key), bind);
                return FALSE;
            } else
                add_menu_cmd_alias(key, default_menu_cmd_info[i].cmd);
            return TRUE;
        }
    }

    /* extended command? */
    if (!bind_key(key, bind)) {
        config_error_add("Unknown key binding command '%s'", bind);
        return FALSE;
    }
    return TRUE;
}

STATIC_VAR NEARDATA const char *menutype[] = { "traditional", "combination",
                                           "full", "partial" };

STATIC_VAR NEARDATA const char *burdentype[] = { "unencumbered", "burdened",
                                             "stressed",     "strained",
                                             "overtaxed",    "overloaded" };

STATIC_VAR NEARDATA const char *runmodes[] = { "teleport", "run", "walk",
                                           "crawl" };

STATIC_VAR NEARDATA const char *sortltype[] = { "none", "loot", "full" };

/*
 * Convert the given string of object classes to a string of default object
 * symbols.
 */
void
oc_to_str(src, dest)
char *src, *dest;
{
    int i;

    while ((i = (int) *src++) != 0) {
        if (i < 0 || i >= MAX_OBJECT_CLASSES)
            impossible("oc_to_str:  illegal object class %d", i);
        else
            *dest++ = def_oc_syms[i].sym;
    }
    *dest = '\0';
}

/*
 * Add the given mapping to the menu command map list.  Always keep the
 * maps valid C strings.
 */
void
add_menu_cmd_alias(from_ch, to_ch)
char from_ch, to_ch;
{
    if (n_menu_mapped >= MAX_MENU_MAPPED_CMDS) {
        pline("out of menu map space.");
    } else {
        mapped_menu_cmds[n_menu_mapped] = from_ch;
        mapped_menu_op[n_menu_mapped] = to_ch;
        n_menu_mapped++;
        mapped_menu_cmds[n_menu_mapped] = '\0';
        mapped_menu_op[n_menu_mapped] = '\0';
    }
}

char
get_menu_cmd_key(ch)
char ch;
{
    char *found = index(mapped_menu_op, ch);

    if (found) {
        int idx = (int) (found - mapped_menu_op);

        ch = mapped_menu_cmds[idx];
    }
    return ch;
}

/*
 * Map the given character to its corresponding menu command.  If it
 * doesn't match anything, just return the original.
 */
char
map_menu_cmd(ch)
char ch;
{
    char *found = index(mapped_menu_cmds, ch);

    if (found) {
        int idx = (int) (found - mapped_menu_cmds);

        ch = mapped_menu_op[idx];
    }
    return ch;
}

void
show_menu_controls(win, dolist)
winid win;
boolean dolist;
{
    char buf[BUFSZ];

    putstr(win, 0, "Menu control keys:");
    if (dolist) {
        int i;

        for (i = 0; i < SIZE(default_menu_cmd_info); i++) {
            Sprintf(buf, "%-8s %s",
                    visctrl(get_menu_cmd_key(default_menu_cmd_info[i].cmd)),
                    default_menu_cmd_info[i].desc);
            putstr(win, 0, buf);
        }
    } else {
        putstr(win, 0, "");
        putstr(win, 0, "          Page    All items");
        Sprintf(buf, "  Select   %s       %s",
                visctrl(get_menu_cmd_key(MENU_SELECT_PAGE)),
                visctrl(get_menu_cmd_key(MENU_SELECT_ALL)));
        putstr(win, 0, buf);
        Sprintf(buf, "Deselect   %s       %s",
                visctrl(get_menu_cmd_key(MENU_UNSELECT_PAGE)),
                visctrl(get_menu_cmd_key(MENU_UNSELECT_ALL)));
        putstr(win, 0, buf);
        Sprintf(buf, "  Invert   %s       %s",
                visctrl(get_menu_cmd_key(MENU_INVERT_PAGE)),
                visctrl(get_menu_cmd_key(MENU_INVERT_ALL)));
        putstr(win, 0, buf);
        putstr(win, 0, "");
        Sprintf(buf, "   Go to   %s   Next page",
                visctrl(get_menu_cmd_key(MENU_NEXT_PAGE)));
        putstr(win, 0, buf);
        Sprintf(buf, "           %s   Previous page",
                visctrl(get_menu_cmd_key(MENU_PREVIOUS_PAGE)));
        putstr(win, 0, buf);
        Sprintf(buf, "           %s   First page",
                visctrl(get_menu_cmd_key(MENU_FIRST_PAGE)));
        putstr(win, 0, buf);
        Sprintf(buf, "           %s   Last page",
                visctrl(get_menu_cmd_key(MENU_LAST_PAGE)));
        putstr(win, 0, buf);
        putstr(win, 0, "");
        Sprintf(buf, "           %s   Search and toggle matching entries",
                visctrl(get_menu_cmd_key(MENU_SEARCH)));
        putstr(win, 0, buf);
    }
}

#if defined(MICRO) || defined(MAC) || defined(WIN32)
#define OPTIONS_HEADING "OPTIONS"
#else
#define OPTIONS_HEADING "NETHACKOPTIONS"
#endif

#define MAX_OPT_VALUE_LENGTH 20

static char fmtstr_doset[] = "%s%-15s [%s]   ";
static char fmtstr_doset_tab[] = "%s\t[%s]";
static char n_currently_set[] = "(%d currently set)";

/* doset('O' command) menu entries for compound options */
STATIC_OVL void
doset_add_menu(win, option, idx, indexoffset, notruncate)
winid win;          /* window to add to */
const char *option; /* option name */
int indexoffset;    /* value to add to index in compopt[], or zero
                       if option cannot be changed */
int idx, notruncate;
{
    const char *value = "unknown"; /* current value */
    char buf[BUFSZ], buf2[BUFSZ], buf3[BUFSZ];
    anything any;
    int i;

    Strcpy(buf, "");
    Strcpy(buf2, "");
    Strcpy(buf3, "");
    any = zeroany;
    if (idx >= 0)
        i = idx;
    else
    {
        for (i = 0; compopt[i].name; i++)
            if (strcmp(option, compopt[i].name) == 0)
                break;
    }

    if (i >= SIZE(compopt))
        return;

    if (compopt[i].descr)
    {
        if (!iflags.menu_tab_sep)
#ifdef GNH_MOBILE
            Sprintf(eos(buf3), "|%s|", compopt[i].descr);
#else
            Sprintf(eos(buf3), "%s", compopt[i].descr);
#endif
        else
            Sprintf(eos(buf3), "\t%s", compopt[i].descr);
    }

    if (indexoffset == 0) {
        any.a_int = 0;
        value = get_compopt_value(option, buf2);
    } else {
        if (compopt[i].name) {
            any.a_int = i + 1 + indexoffset;
            value = get_compopt_value(option, buf2);
        } else {
            /* We are trying to add an option not found in compopt[].
               This is almost certainly bad, but we'll let it through anyway
               (with a zero value, so it can't be selected). */
            any.a_int = 0;
        }
    }
    /* "    " replaces "a - " -- assumes menus follow that style */

    char valuebuf[BUFSZ];
#ifdef GNH_MOBILE
    Strcpy(valuebuf, value);
#else
    if (notruncate && !iflags.menu_tab_sep)
    {
        Strcpy(valuebuf, value);
    }
    else
    {
        /* truncate value */
        Strncpy(valuebuf, value, MAX_OPT_VALUE_LENGTH);
        valuebuf[MAX_OPT_VALUE_LENGTH] = 0;
        if (strlen(value) > MAX_OPT_VALUE_LENGTH)
            valuebuf[MAX_OPT_VALUE_LENGTH - 3] = valuebuf[MAX_OPT_VALUE_LENGTH - 2] = valuebuf[MAX_OPT_VALUE_LENGTH - 1] = '.';
    }
#endif

    if (!iflags.menu_tab_sep)
    {
        char buf4[BUFSZ];
        Strcpy(buf4, "");
        char* p = buf4;
        int j;
        int len = (int)strlen(valuebuf);
        for (j = 0; j < MAX_OPT_VALUE_LENGTH - len; j++, p++)
            *p = ' ';
        *p = 0;
        Strcat(buf4, "  ");

        Sprintf(buf, fmtstr_doset, any.a_int ? "" : "    ", option, valuebuf);
        Strcat(buf, buf4);
    }
    else
    {
        Sprintf(buf, fmtstr_doset_tab, option, valuebuf);
    }
    Strcat(buf, buf3);
    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_INDENT_AT_DOUBLE_SPACE | ATR_ALT_DIVISORS, NO_COLOR, buf, MENU_UNSELECTED);
}

STATIC_OVL void
opts_add_others(win, name, id, bufx, nset)
winid win;
const char *name;
int id;
char *bufx;
int nset;
{
    char buf[BUFSZ], buf2[BUFSZ];
    anything any = zeroany;

    any.a_int = id;
    if (!bufx)
        Sprintf(buf2, n_currently_set, nset);
    else
        Sprintf(buf2, "%s", bufx);
    if (!iflags.menu_tab_sep)
        Sprintf(buf, fmtstr_doset, any.a_int ? "" : "    ",
                name, buf2);
    else
        Sprintf(buf, fmtstr_doset_tab, name, buf2);
    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_INDENT_AT_DOUBLE_SPACE | ATR_ALT_DIVISORS, NO_COLOR, buf, MENU_UNSELECTED);
}

int
count_apes(VOID_ARGS)
{
    return count_ape_maps((int *) 0, (int *) 0);
}

enum opt_other_enums {
    OPT_OTHER_MSGTYPE = -4,
    OPT_OTHER_MENUCOLOR = -3,
    OPT_OTHER_STATHILITE = -2,
    OPT_OTHER_APEXC = -1
    /* these must be < 0 */
};

static struct other_opts {
    const char *name;
    int optflags;
    enum opt_other_enums code;
    int NDECL((*othr_count_func));
} othropt[] = {
    { "autopickup exceptions", SET_IN_GAME, OPT_OTHER_APEXC, count_apes },
    { "menu colors", SET_IN_GAME, OPT_OTHER_MENUCOLOR, count_menucolors },
    { "message types", SET_IN_GAME, OPT_OTHER_MSGTYPE, msgtype_count },
#ifdef STATUS_HILITES
    { "status hilite rules", SET_IN_GAME, OPT_OTHER_STATHILITE,
      count_status_hilites },
#endif
    { (char *) 0, 0, (enum opt_other_enums) 0 },
};

/* the 'O' command */
int
doset() /* changing options via menu by Per Liboriussen */
{
    static boolean made_fmtstr = FALSE;
    char buf[BUFSZ], buf2[BUFSZ] = DUMMY;
    const char *name;
    int i = 0, pass, boolcount, pick_cnt, pick_idx, opt_indx;
    boolean *bool_p;
    winid tmpwin;
    anything any;
    menu_item *pick_list;
    int indexoffset, startpass, endpass, optflags;
    boolean setinitial = FALSE, fromfile = FALSE;
    size_t longest_name_len;

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu_ex(tmpwin, GHMENU_STYLE_OPTIONS);

#ifdef notyet /* SYSCF */
    /* XXX I think this is still fragile.  Fixing initial/from_file and/or
       changing the SET_* etc to bitmaps will let me make this better. */
    if (wizard)
        startpass = SET_IN_SYS;
    else
#endif
        startpass = DISP_IN_GAME;
    endpass = (wizard) ? SET_IN_WIZGAME : SET_IN_GAME;

    if (!made_fmtstr && !iflags.menu_tab_sep) {
        /* spin through the options to find the longest name
           and adjust the format string accordingly */
        longest_name_len = 0;
        for (pass = 0; pass <= 2; pass++)
            for (i = 0; (name = ((pass == 0) ? boolopt[i].name
                                 : (pass == 1) ? compopt[i].name
                                   : othropt[i].name)) != 0; i++) {
                if (pass == 0 && !boolopt[i].addr)
                    continue;
                optflags = (pass == 0) ? boolopt[i].optflags
                           : (pass == 1) ? compopt[i].optflags
                             : othropt[i].optflags;
                if (optflags < startpass || optflags > endpass)
                    continue;
                if ((is_wc_option(name) && !wc_supported(name))
                    || (is_wc2_option(name) && !wc2_supported(name)))
                    continue;

                if (strlen(name) > longest_name_len)
                    longest_name_len = strlen(name);
            }
        Sprintf(fmtstr_doset, "%%s%%-%us [%%s]", (unsigned)longest_name_len);
        made_fmtstr = TRUE;
    }

    any = zeroany;
    add_extended_menu(tmpwin, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_NOTABS | ATR_HEADING, NO_COLOR,
             "Booleans (selecting will toggle value):", MENU_UNSELECTED, menu_heading_info());
    any.a_int = 0;
    /* first list any other non-modifiable booleans, then modifiable ones */
    for (pass = 0; pass <= 1; pass++)
        for (i = 0; (name = boolopt[i].name) != 0; i++)
            if ((bool_p = boolopt[i].addr) != 0
                && ((boolopt[i].optflags <= DISP_IN_GAME && pass == 0)
                    || (boolopt[i].optflags >= SET_IN_GAME && pass == 1))) {
                if (bool_p == &flags.female)
                    continue; /* obsolete */
                if (boolopt[i].optflags == SET_IN_WIZGAME && !wizard)
                    continue;
                if ((is_wc_option(name) && !wc_supported(name))
                    || (is_wc2_option(name) && !wc2_supported(name)))
                    continue;

                any.a_int = (pass == 0) ? 0 : i + 1;
                if (!iflags.menu_tab_sep)
                    Sprintf(buf, fmtstr_doset, (pass == 0) ? "    " : "",
                        name, *bool_p ? "true" : "false");
                else
                    Sprintf(buf, fmtstr_doset_tab,
                        name, *bool_p ? "true" : "false");

                if (boolopt[i].descr)
                {
                    if (!iflags.menu_tab_sep)
                        Strcat(buf, *bool_p ? "   " : "  ");
                    if (!iflags.menu_tab_sep)
#ifdef GNH_MOBILE
                        Sprintf(eos(buf), "|%s|", boolopt[i].descr);
#else
                        Sprintf(eos(buf), "%s", boolopt[i].descr);
#endif
                    else
                        Sprintf(eos(buf), "\t%s", boolopt[i].descr);
                }

                add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_INDENT_AT_DOUBLE_SPACE | ATR_ALT_DIVISORS, NO_COLOR, buf,
                         MENU_UNSELECTED);
            }

    boolcount = i;
    indexoffset = boolcount;
    any = zeroany;
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "", MENU_UNSELECTED);
    add_extended_menu(tmpwin, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_NOTABS | ATR_HEADING, NO_COLOR,
             "Compounds (selecting will prompt for new value):",
             MENU_UNSELECTED, menu_heading_info());

    /* deliberately put playmode, name, role+race+gender+align first */
    doset_add_menu(tmpwin, "playmode", -1, 0, 0);
    doset_add_menu(tmpwin, "name", -1, 0, 0);
    doset_add_menu(tmpwin, "role", -1, 0, 0);
    doset_add_menu(tmpwin, "race", -1, 0, 0);
    doset_add_menu(tmpwin, "gender", -1, 0, 0);
    doset_add_menu(tmpwin, "align", -1, 0, 0);

    for (pass = startpass; pass <= endpass; pass++)
        for (i = 0; (name = compopt[i].name) != 0; i++)
            if (compopt[i].optflags == pass) {
                if (!strcmp(name, "playmode")  || !strcmp(name, "name")
                    || !strcmp(name, "role")   || !strcmp(name, "race")
                    || !strcmp(name, "gender") || !strcmp(name, "align"))
                    continue;
                if ((is_wc_option(name) && !wc_supported(name))
                    || (is_wc2_option(name) && !wc2_supported(name)))
                    continue;

                doset_add_menu(tmpwin, name, i, (pass == DISP_IN_GAME) ? 0 : indexoffset, 0);
            }

    any = zeroany;
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "", MENU_UNSELECTED);
    add_extended_menu(tmpwin, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_NOTABS | ATR_HEADING, NO_COLOR,
             "Other settings:", MENU_UNSELECTED, menu_heading_info());

    for (i = 0; (name = othropt[i].name) != 0; i++) {
        if ((is_wc_option(name) && !wc_supported(name))
            || (is_wc2_option(name) && !wc2_supported(name)))
            continue;
        opts_add_others(tmpwin, name, othropt[i].code,
                        (char *) 0, othropt[i].othr_count_func());
    }

#ifdef PREFIXES_IN_USE
    any = zeroany;
    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "", MENU_UNSELECTED);
    add_extended_menu(tmpwin, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_NOTABS | ATR_HEADING, NO_COLOR,
             "Variable playground locations:", MENU_UNSELECTED, menu_heading_info());
    for (i = 0; i < PREFIX_COUNT; i++)
        doset_add_menu(tmpwin, fqn_prefix_names[i], -1, 0, 1);
#endif
    end_menu(tmpwin, "Set what options?");
    
    need_redraw = FALSE;
    need_set_sound_volume = FALSE;
    need_update_inventory = FALSE;
    need_set_animation_timer_interval = FALSE;
    need_update_space_binding = FALSE;
    need_status_initialize = FALSE;
    need_here_window = FALSE;

    if ((pick_cnt = select_menu(tmpwin, PICK_ANY, &pick_list)) > 0) {
        /*
         * Walk down the selection list and either invert the booleans
         * or prompt for new values. In most cases, call parseoptions()
         * to take care of options that require special attention, like
         * redraws.
         */
        for (pick_idx = 0; pick_idx < pick_cnt; ++pick_idx) {
            opt_indx = pick_list[pick_idx].item.a_int - 1;
            if (opt_indx < -1)
                opt_indx++; /* -1 offset for select_menu() */
            if (opt_indx == OPT_OTHER_APEXC) {
                (void) special_handling("autopickup_exception", setinitial,
                                        fromfile);
#ifdef STATUS_HILITES
            } 
            else if (opt_indx == OPT_OTHER_STATHILITE)
            {
                if (!status_hilite_menu())
                {
                    pline("Bad status hilite(s) specified.");
                } 
                else 
                {
                    if (wc2_supported("hilite_status"))
                        preference_update("hilite_status");
                }
#endif
            } 
            else if (opt_indx == OPT_OTHER_MENUCOLOR) 
            {
                    (void) special_handling("menu_colors", setinitial,
                                            fromfile);
            } 
            else if (opt_indx == OPT_OTHER_MSGTYPE) 
            {
                    (void) special_handling("msgtype", setinitial, fromfile);
            } 
            else if (opt_indx < boolcount && opt_indx >= 0)
            {
                /* boolean option */
                Sprintf(buf, "%s%s", *boolopt[opt_indx].addr ? "!" : "",
                        boolopt[opt_indx].name);
                (void) parseoptions(buf, setinitial, fromfile);
                if (wc_supported(boolopt[opt_indx].name)
                    || wc2_supported(boolopt[opt_indx].name))
                    preference_update(boolopt[opt_indx].name);
            } else {
                /* compound option */
                opt_indx -= boolcount;

                if (opt_indx >= 0)
                {
                    if (!special_handling(compopt[opt_indx].name, setinitial,
                        fromfile)) {
                        Sprintf(buf, "Set %s to what?", compopt[opt_indx].name);
                        getlin(buf, buf2);
                        if (buf2[0] == '\033')
                            continue;
                        Sprintf(buf, "%s:%s", compopt[opt_indx].name, buf2);
                        /* pass the buck */
                        (void)parseoptions(buf, setinitial, fromfile);
                    }
                    if (wc_supported(compopt[opt_indx].name)
                        || wc2_supported(compopt[opt_indx].name))
                        preference_update(compopt[opt_indx].name);
                }
            }
        }
        free((genericptr_t) pick_list), pick_list = (menu_item *) 0;
    }

    destroy_nhwindow(tmpwin);

    if (need_here_window)
    {
        if (windowprocs.wincap2 & WC2_HEREWINDOW)
        {
            if (iflags.wc2_herewindow)
            {
                if (WIN_HERE == WIN_ERR)
                    WIN_HERE = create_nhwindow(NHW_HERE);
            }
            else
            {
                if (WIN_HERE != WIN_ERR)
                {
                    destroy_nhwindow(WIN_HERE);
                    WIN_HERE = WIN_ERR;
                }
            }
        }
    }

    if (need_issue_gui_command)
    {
        issue_simple_gui_command(GUI_CMD_PREFERENCE_SET);
    }

    if (need_stretch_map)
    {
        stretch_window();
    }

    if (need_status_initialize)
    {
        (*windowprocs.win_status_init)(1);
        status_reassess();
        need_redraw = TRUE;
    }

    if (need_redraw)
    {
        check_gold_symbol();
        reglyph_darkroom();
        (void) doredraw();
    }

    if (need_set_sound_volume)
    {
        dosetsoundvolume();
    }

    if (need_update_inventory)
    {
        update_inventory();
    }

    if (need_set_animation_timer_interval)
    {
        set_animation_timer_interval(flags.animation_frame_interval_in_milliseconds <= 0 ? ANIMATION_FRAME_INTERVAL : flags.animation_frame_interval_in_milliseconds);
    }
    if (need_update_space_binding)
    {
        if (flags.rest_on_space)
            (void)bind_key(' ', "wait");
        else
            (void)bind_key(' ', "nothing");

        update_bindings_list();
    }

    return 0;
}

/* common to msg-types, menu-colors, autopickup-exceptions */
STATIC_OVL int
handle_add_list_remove(optname, numtotal)
const char *optname;
int numtotal;
{
    winid tmpwin;
    anything any;
    int i, pick_cnt, opt_idx;
    menu_item *pick_list = (menu_item *) 0;
    static const struct action {
        char letr;
        const char *desc;
    } action_titles[] = {
        { 'a', "add new %s" },         /* [0] */
        { 'l', "list %s" },            /* [1] */
        { 'r', "remove existing %s" }, /* [2] */
        { 'x', "exit this menu" },     /* [3] */
    };

    opt_idx = 0;
    tmpwin = create_nhwindow(NHW_MENU);
    start_menu(tmpwin);
    any = zeroany;
    for (i = 0; i < SIZE(action_titles); i++) {
        char tmpbuf[BUFSZ];

        any.a_int++;
        /* omit list and remove if there aren't any yet */
        if (!numtotal && (i == 1 || i == 2))
            continue;
        Sprintf(tmpbuf, action_titles[i].desc,
                (i == 1) ? makeplural(optname) : optname);
        add_menu(tmpwin, NO_GLYPH, &any, action_titles[i].letr, 0, ATR_NONE, NO_COLOR,
                 tmpbuf, (i == 3) ? MENU_SELECTED : MENU_UNSELECTED);
    }
    end_menu(tmpwin, "Do what?");
    if ((pick_cnt = select_menu(tmpwin, PICK_ONE, &pick_list)) > 0) {
        opt_idx = pick_list[0].item.a_int - 1;
        if (pick_cnt > 1 && opt_idx == 3)
            opt_idx = pick_list[1].item.a_int - 1;
        free((genericptr_t) pick_list);
    } else
        opt_idx = 3; /* none selected, exit menu */
    destroy_nhwindow(tmpwin);
    return opt_idx;
}

struct symsetentry *symset_list = 0; /* files.c will populate this with
                                      * list of available sets */

STATIC_OVL boolean
special_handling(optname, setinitial, setfromfile)
const char *optname;
boolean setinitial, setfromfile;
{
    winid tmpwin;
    anything any;
    int i, n;
    char buf[BUFSZ];

    /* Special handling of menustyle, pickup_burden, pickup_types,
     * disclose, runmode, msg_window, menu_headings, sortloot,
     * and number_pad options.
     * Also takes care of interactive autopickup_exception_handling changes.
     */
    if (!strcmp("menustyle", optname)) {
        const char *style_name;
        menu_item *style_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < SIZE(menutype); i++) {
            style_name = menutype[i];
            /* note: separate `style_name' variable used
               to avoid an optimizer bug in VAX C V2.3 */
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, *style_name, 0, ATR_NONE, NO_COLOR,
                     style_name, MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select menustyle:");
        if (select_menu(tmpwin, PICK_ONE, &style_pick) > 0) {
            flags.menu_style = style_pick->item.a_int - 1;
            free((genericptr_t) style_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("paranoid_confirmation", optname)) {
        menu_item *paranoia_picks = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; paranoia[i].flagmask != 0; ++i) {
            if (paranoia[i].flagmask == PARANOID_BONES && !wizard)
                continue;
            any.a_int = paranoia[i].flagmask;
            add_menu(tmpwin, NO_GLYPH, &any, *paranoia[i].argname, 0,
                     ATR_NONE, NO_COLOR, paranoia[i].explain,
                     (flags.paranoia_bits & paranoia[i].flagmask)
                         ? MENU_SELECTED
                         : MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Actions requiring extra confirmation:");
        i = select_menu(tmpwin, PICK_ANY, &paranoia_picks);
        if (i >= 0) {
            /* player didn't cancel; we reset all the paranoia options
               here even if there were no items picked, since user
               could have toggled off preselected ones to end up with 0 */
            flags.paranoia_bits = 0;
            if (i > 0) {
                /* at least 1 item set, either preselected or newly picked */
                while (--i >= 0)
                    flags.paranoia_bits |= paranoia_picks[i].item.a_int;
                free((genericptr_t) paranoia_picks);
            }
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("pickup_burden", optname)) {
        const char *burden_name, *burden_letters = "ubsntl";
        menu_item *burden_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < SIZE(burdentype); i++) {
            burden_name = burdentype[i];
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, burden_letters[i], 0, ATR_NONE, NO_COLOR,
                     burden_name, MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select encumbrance level:");
        if (select_menu(tmpwin, PICK_ONE, &burden_pick) > 0) {
            flags.pickup_burden = burden_pick->item.a_int - 1;
            free((genericptr_t) burden_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("pickup_types", optname)) {
        /* parseoptions will prompt for the list of types */
        (void) parseoptions(strcpy(buf, "pickup_types"),
                            setinitial, setfromfile);
    } else if (!strcmp("disclose", optname)) {
        /* order of disclose_names[] must correspond to
           disclosure_options in decl.c */
        static const char *disclosure_names[] = {
            "inventory", "attributes", "vanquished",
            "genocides", "conduct",    "overview",
        };
        int disc_cat[NUM_DISCLOSURE_OPTIONS];
        int pick_cnt, pick_idx, opt_idx;
        char c;
        menu_item *disclosure_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < NUM_DISCLOSURE_OPTIONS; i++) {
            Sprintf(buf, "%-12s[%c%c]", disclosure_names[i],
                    flags.end_disclose[i], disclosure_options[i]);
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, disclosure_options[i], 0,
                     ATR_NONE, NO_COLOR, buf, MENU_UNSELECTED);
            disc_cat[i] = 0;
        }
        end_menu(tmpwin, "Change which disclosure options categories:");
        pick_cnt = select_menu(tmpwin, PICK_ANY, &disclosure_pick);
        if (pick_cnt > 0) {
            for (pick_idx = 0; pick_idx < pick_cnt; ++pick_idx) {
                opt_idx = disclosure_pick[pick_idx].item.a_int - 1;
                disc_cat[opt_idx] = 1;
            }
            free((genericptr_t) disclosure_pick);
            disclosure_pick = (menu_item *) 0;
        }
        destroy_nhwindow(tmpwin);

        for (i = 0; i < NUM_DISCLOSURE_OPTIONS; i++) {
            if (disc_cat[i]) {
                c = flags.end_disclose[i];
                Sprintf(buf, "Disclosure options for %s:",
                        disclosure_names[i]);
                tmpwin = create_nhwindow(NHW_MENU);
                start_menu(tmpwin);
                any = zeroany;
                /* 'y','n',and '+' work as alternate selectors; '-' doesn't */
                any.a_char = DISCLOSE_NO_WITHOUT_PROMPT;
                add_menu(tmpwin, NO_GLYPH, &any, 0, any.a_char, ATR_NONE, NO_COLOR,
                         "Never disclose, without prompting",
                         (c == any.a_char) ? MENU_SELECTED : MENU_UNSELECTED);
                any.a_char = DISCLOSE_YES_WITHOUT_PROMPT;
                add_menu(tmpwin, NO_GLYPH, &any, 0, any.a_char, ATR_NONE, NO_COLOR,
                         "Always disclose, without prompting",
                         (c == any.a_char) ? MENU_SELECTED : MENU_UNSELECTED);
                if (*disclosure_names[i] == 'v') {
                    any.a_char = DISCLOSE_SPECIAL_WITHOUT_PROMPT; /* '#' */
                    add_menu(tmpwin, NO_GLYPH, &any, 0, any.a_char, ATR_NONE, NO_COLOR,
                             "Always disclose, pick sort order from menu",
                             (c == any.a_char) ? MENU_SELECTED
                                               : MENU_UNSELECTED);
                }
                any.a_char = DISCLOSE_PROMPT_DEFAULT_NO;
                add_menu(tmpwin, NO_GLYPH, &any, 0, any.a_char, ATR_NONE, NO_COLOR,
                         "Prompt, with default answer of \"No\"",
                         (c == any.a_char) ? MENU_SELECTED : MENU_UNSELECTED);
                any.a_char = DISCLOSE_PROMPT_DEFAULT_YES;
                add_menu(tmpwin, NO_GLYPH, &any, 0, any.a_char, ATR_NONE, NO_COLOR,
                         "Prompt, with default answer of \"Yes\"",
                         (c == any.a_char) ? MENU_SELECTED : MENU_UNSELECTED);
                if (*disclosure_names[i] == 'v') {
                    any.a_char = DISCLOSE_PROMPT_DEFAULT_SPECIAL; /* '?' */
                    add_menu(tmpwin, NO_GLYPH, &any, 0, any.a_char, ATR_NONE, NO_COLOR,
                "Prompt, with default answer of \"Ask\" to request sort menu",
                             (c == any.a_char) ? MENU_SELECTED
                                               : MENU_UNSELECTED);
                }
                end_menu(tmpwin, buf);
                n = select_menu(tmpwin, PICK_ONE, &disclosure_pick);
                if (n > 0) {
                    flags.end_disclose[i] = disclosure_pick[0].item.a_char;
                    if (n > 1 && flags.end_disclose[i] == c)
                        flags.end_disclose[i] = disclosure_pick[1].item.a_char;
                    free((genericptr_t) disclosure_pick);
                }
                destroy_nhwindow(tmpwin);
            }
        }
    } else if (!strcmp("runmode", optname)) {
        const char *mode_name;
        menu_item *mode_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < SIZE(runmodes); i++) {
            mode_name = runmodes[i];
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, *mode_name, 0, ATR_NONE, NO_COLOR,
                     mode_name, MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select run/travel display mode:");
        if (select_menu(tmpwin, PICK_ONE, &mode_pick) > 0) {
            flags.runmode = mode_pick->item.a_int - 1;
            free((genericptr_t) mode_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("whatis_coord", optname)) {
        menu_item *window_pick = (menu_item *) 0;
        int pick_cnt;
        char gp = iflags.getpos_coords;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        any.a_char = GPCOORDS_COMPASS;
        add_menu(tmpwin, NO_GLYPH, &any, GPCOORDS_COMPASS, 0, ATR_NONE, NO_COLOR,
                 "compass ('east' or '3s' or '2n,4w')",
                 (gp == GPCOORDS_COMPASS) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_char = GPCOORDS_COMFULL;
        add_menu(tmpwin, NO_GLYPH, &any, GPCOORDS_COMFULL, 0, ATR_NONE, NO_COLOR,
                 "full compass ('east' or '3south' or '2north,4west')",
                 (gp == GPCOORDS_COMFULL) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_char = GPCOORDS_MAP;
        add_menu(tmpwin, NO_GLYPH, &any, GPCOORDS_MAP, 0, ATR_NONE, NO_COLOR,
                 "map <x,y>",
                 (gp == GPCOORDS_MAP) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_char = GPCOORDS_SCREEN;
        add_menu(tmpwin, NO_GLYPH, &any, GPCOORDS_SCREEN, 0, ATR_NONE, NO_COLOR,
                 "screen [row,column]",
                 (gp == GPCOORDS_SCREEN) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_char = GPCOORDS_NONE;
        add_menu(tmpwin, NO_GLYPH, &any, GPCOORDS_NONE, 0, ATR_NONE, NO_COLOR,
                 "none (no coordinates displayed)",
                 (gp == GPCOORDS_NONE) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_long = 0L;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "", MENU_UNSELECTED);
        Sprintf(buf, "map: upper-left: <%d,%d>, lower-right: <%d,%d>%s",
                1, 0, COLNO - 1, ROWNO - 1,
                flags.verbose ? "; column 0 unused, off left edge" : "");
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf, MENU_UNSELECTED);
        if (strcmp(windowprocs.name, "tty")) /* only show for non-tty */
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
       "screen: row is offset to accommodate tty interface's use of top line",
                     MENU_UNSELECTED);
#if COLNO == 80
#define COL80ARG flags.verbose ? "; column 80 is not used" : ""
#else
#define COL80ARG ""
#endif
        Sprintf(buf, "screen: upper-left: [%02d,%02d], lower-right: [%d,%d]%s",
                0 + 2, 1, ROWNO - 1 + 2, COLNO - 1, COL80ARG);
#undef COL80ARG
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf, MENU_UNSELECTED);
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "", MENU_UNSELECTED);
        end_menu(tmpwin,
            "Select coordinate display when auto-describing a map position:");
        if ((pick_cnt = select_menu(tmpwin, PICK_ONE, &window_pick)) > 0) {
            iflags.getpos_coords = window_pick[0].item.a_char;
            /* PICK_ONE doesn't unselect preselected entry when
               selecting another one */
            if (pick_cnt > 1 && iflags.getpos_coords == gp)
                iflags.getpos_coords = window_pick[1].item.a_char;
            free((genericptr_t) window_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("whatis_filter", optname)) {
        menu_item *window_pick = (menu_item *) 0;
        int pick_cnt;
        char gf = iflags.getloc_filter;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        any.a_char = (GFILTER_NONE + 1);
        add_menu(tmpwin, NO_GLYPH, &any, 'n',
                 0, ATR_NONE, NO_COLOR, "no filtering",
                 (gf == GFILTER_NONE) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_char = (GFILTER_VIEW + 1);
        add_menu(tmpwin, NO_GLYPH, &any, 'v',
                 0, ATR_NONE, NO_COLOR, "in view only",
                 (gf == GFILTER_VIEW) ? MENU_SELECTED : MENU_UNSELECTED);
        any.a_char = (GFILTER_AREA + 1);
        add_menu(tmpwin, NO_GLYPH, &any, 'a',
                 0, ATR_NONE, NO_COLOR, "in same area",
                 (gf == GFILTER_AREA) ? MENU_SELECTED : MENU_UNSELECTED);
        end_menu(tmpwin,
      "Select location filtering when going for next/previous map position:");
        if ((pick_cnt = select_menu(tmpwin, PICK_ONE, &window_pick)) > 0) {
            iflags.getloc_filter = (window_pick[0].item.a_char - 1);
            /* PICK_ONE doesn't unselect preselected entry when
               selecting another one */
            if (pick_cnt > 1 && iflags.getloc_filter == gf)
                iflags.getloc_filter = (window_pick[1].item.a_char - 1);
            free((genericptr_t) window_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("msg_window", optname)) {
#if defined(TTY_GRAPHICS) || defined(CURSES_GRAPHICS)
        if (WINDOWPORT("tty") || WINDOWPORT("curses")) {
            /* by Christian W. Cooper */
            menu_item *window_pick = (menu_item *) 0;

            tmpwin = create_nhwindow(NHW_MENU);
            start_menu(tmpwin);
            any = zeroany;
            if (!WINDOWPORT("curses")) {
                any.a_char = 's';
                add_menu(tmpwin, NO_GLYPH, &any, 's', 0, ATR_NONE, NO_COLOR,
                         "single", MENU_UNSELECTED);
                any.a_char = 'c';
                add_menu(tmpwin, NO_GLYPH, &any, 'c', 0, ATR_NONE, NO_COLOR,
                         "combination", MENU_UNSELECTED);
            }
            any.a_char = 'f';
            add_menu(tmpwin, NO_GLYPH, &any, 'f', 0, ATR_NONE, NO_COLOR, "full",
                     MENU_UNSELECTED);
            any.a_char = 'r';
            add_menu(tmpwin, NO_GLYPH, &any, 'r', 0, ATR_NONE, NO_COLOR, "reversed",
                     MENU_UNSELECTED);
            end_menu(tmpwin, "Select message history display type:");
            if (select_menu(tmpwin, PICK_ONE, &window_pick) > 0) {
                iflags.prevmsg_window = window_pick->item.a_char;
                free((genericptr_t) window_pick);
            }
            destroy_nhwindow(tmpwin);
        } else
#endif /* msg_window for tty or curses */
            pline("'%s' option is not supported for '%s'.",
                  optname, windowprocs.name);
    } else if (!strcmp("sortloot", optname)) {
        const char *sortl_name;
        menu_item *sortl_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < SIZE(sortltype); i++) {
            sortl_name = sortltype[i];
            any.a_char = *sortl_name;
            add_menu(tmpwin, NO_GLYPH, &any, *sortl_name, 0, ATR_NONE, NO_COLOR,
                     sortl_name, (flags.sortloot == *sortl_name)
                                    ? MENU_SELECTED : MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select loot sorting type:");
        n = select_menu(tmpwin, PICK_ONE, &sortl_pick);
        if (n > 0) {
            char c = sortl_pick[0].item.a_char;

            if (n > 1 && c == flags.sortloot)
                c = sortl_pick[1].item.a_char;
            flags.sortloot = c;
            free((genericptr_t) sortl_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("align_message", optname)
               || !strcmp("align_status", optname)) {
        menu_item *window_pick = (menu_item *) 0;
        char abuf[BUFSZ];
        boolean msg = (*(optname + 6) == 'm');

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        any.a_int = ALIGN_TOP;
        add_menu(tmpwin, NO_GLYPH, &any, 't', 0, ATR_NONE, NO_COLOR, "top",
                 MENU_UNSELECTED);
        any.a_int = ALIGN_BOTTOM;
        add_menu(tmpwin, NO_GLYPH, &any, 'b', 0, ATR_NONE, NO_COLOR, "bottom",
                 MENU_UNSELECTED);
        any.a_int = ALIGN_LEFT;
        add_menu(tmpwin, NO_GLYPH, &any, 'l', 0, ATR_NONE, NO_COLOR, "left",
                 MENU_UNSELECTED);
        any.a_int = ALIGN_RIGHT;
        add_menu(tmpwin, NO_GLYPH, &any, 'r', 0, ATR_NONE, NO_COLOR, "right",
                 MENU_UNSELECTED);
        Sprintf(abuf, "Select %s window placement relative to the map:",
                msg ? "message" : "status");
        end_menu(tmpwin, abuf);
        if (select_menu(tmpwin, PICK_ONE, &window_pick) > 0) {
            if (msg)
                iflags.wc_align_message = window_pick->item.a_int;
            else
                iflags.wc_align_status = window_pick->item.a_int;
            free((genericptr_t) window_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("number_pad", optname)) {
        static const char *npchoices[] = {
            " 0 (off)", " 1 (on)", " 2 (on, MSDOS compatible)",
            " 3 (on, phone-style digit layout)",
            " 4 (on, phone-style layout, MSDOS compatible)",
            "-1 (off, 'z' to move upper-left, 'y' to zap wands)",
            "-2 (off, diagonal movement with yuhj)"
        };
        menu_item *mode_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < SIZE(npchoices); i++) {
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, 'a' + i, 0, ATR_NONE, NO_COLOR,
                     npchoices[i], MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select number_pad mode:");
        if (select_menu(tmpwin, PICK_ONE, &mode_pick) > 0) {
            switch (mode_pick->item.a_int - 1) {
            case 0:
                iflags.num_pad = FALSE;
                iflags.num_pad_mode = 0;
                break;
            case 1:
                iflags.num_pad = TRUE;
                iflags.num_pad_mode = 0;
                break;
            case 2:
                iflags.num_pad = TRUE;
                iflags.num_pad_mode = 1;
                break;
            case 3:
                iflags.num_pad = TRUE;
                iflags.num_pad_mode = 2;
                break;
            case 4:
                iflags.num_pad = TRUE;
                iflags.num_pad_mode = 3;
                break;
            /* second last menu choice: number_pad == -1 */
            case 5:
                iflags.num_pad = FALSE;
                iflags.num_pad_mode = 1;
                break;
                /* last menu choice: number_pad == -2 */
            case 6:
                iflags.num_pad = FALSE;
                iflags.num_pad_mode = 4;
                break;
            }
            reset_commands(FALSE);
            number_pad(iflags.num_pad ? 1 : 0);
            free((genericptr_t) mode_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("spellorder", optname)) {
        menu_item* mode_pick = (menu_item*)0;
        char splbuf[BUFSZ];
        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = SORTBY_NONE; i < SORTBY_CURRENT - 1; i++) {
            Sprintf(splbuf, "%d (%s)", i, spl_sortchoices[i]);
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, 'a' + i, 0, ATR_NONE, NO_COLOR,
                splbuf, MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select spell sort method:");
        if (select_menu(tmpwin, PICK_ONE, &mode_pick) > 0) {
            flags.spellorder = mode_pick->item.a_int - 1;
            sortspells();
            free((genericptr_t)mode_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("max_hint_difficulty", optname)) {
        menu_item* mode_pick = (menu_item*)0;
        char splbuf[BUFSZ];
        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = MIN_DIFFICULTY_LEVEL - 1; i <= MAX_DIFFICULTY_LEVEL; i++) {
            char dlbuf[BUFSZ];
            Strcpy(dlbuf, i < MIN_DIFFICULTY_LEVEL ? "off" : get_game_difficulty_text(i));
            *dlbuf = highc(*dlbuf);
            Sprintf(splbuf, "%s (%d)", dlbuf, i);
            any.a_int = i + 1 - (MIN_DIFFICULTY_LEVEL - 1);
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                splbuf, MENU_UNSELECTED);
        }
        end_menu(tmpwin, "Select maximum hint difficulty level:");
        if (select_menu(tmpwin, PICK_ONE, &mode_pick) > 0) {
            flags.max_hint_difficulty = mode_pick->item.a_int - 1 + (MIN_DIFFICULTY_LEVEL - 1);
            free((genericptr_t)mode_pick);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("right_click_command", optname) || !strcmp("middle_click_command", optname)) {
        boolean is_middle = !strcmp("middle_click_command", optname);
        menu_item *mode_pick = (menu_item *) 0;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu(tmpwin);
        any = zeroany;
        for (i = 0; i < SIZE(mouse_cmd_names); i++) {
            if (i >= 1 && i <= 2)
                continue;
            any.a_int = i + 1;
            add_menu(tmpwin, NO_GLYPH, &any, 'a' + i, 0, ATR_NONE, NO_COLOR,
                mouse_cmd_names[i], MENU_UNSELECTED);
        }
        end_menu(tmpwin, is_middle ? "Select middle button command:" : "Select right button command:");
        if (select_menu(tmpwin, PICK_ONE, &mode_pick) > 0) {
            int res = mode_pick->item.a_int - 1;
            if (is_middle)
                flags.middle_click_command = (uchar)res;
            else
                flags.right_click_command = (uchar)res;
            free((genericptr_t) mode_pick);
            /* Notification is needed */
            if (is_middle)
                issue_gui_command(GUI_CMD_REPORT_MOUSE_COMMAND, (int)flags.middle_click_command, 1, (const char*)0);
            else
                issue_gui_command(GUI_CMD_REPORT_MOUSE_COMMAND, (int)flags.right_click_command, 0, (const char*)0);
        }
        destroy_nhwindow(tmpwin);
    } else if (!strcmp("menu_headings", optname)) {
        int mhattr = query_attr("How to highlight menu headings:");

        if (mhattr != -1)
            iflags.menu_headings = mhattr;
    } else if (!strcmp("msgtype", optname)) {
        int opt_idx, nmt, mttyp;
        char mtbuf[BUFSZ] = DUMMY;

 msgtypes_again:
        nmt = msgtype_count();
        opt_idx = handle_add_list_remove("message type", nmt);
        if (opt_idx == 3) { /* done */
            return TRUE;
        } else if (opt_idx == 0) { /* add new */
            getlin("What new message pattern?", mtbuf);
            if (*mtbuf == '\033')
                return TRUE;
            if (*mtbuf
                && test_regex_pattern(mtbuf, (const char *)0)
                && (mttyp = query_msgtype()) != -1
                && !msgtype_add(mttyp, mtbuf)) {
                pline("Error adding the message type.");
                wait_synch();
            }
            goto msgtypes_again;
        } else { /* list (1) or remove (2) */
            int pick_idx, pick_cnt;
            int mt_idx;
            size_t ln;
            const char *mtype;
            menu_item *pick_list = (menu_item *) 0;
            struct plinemsg_type *tmp = plinemsg_types;

            tmpwin = create_nhwindow(NHW_MENU);
            start_menu(tmpwin);
            any = zeroany;
            mt_idx = 0;
            while (tmp) {
                mtype = msgtype2name(tmp->msgtype);
                any.a_int = ++mt_idx;
                Sprintf(mtbuf, "%-5s \"", mtype);
                ln = sizeof mtbuf - strlen(mtbuf) - sizeof "\"";
                if (strlen(tmp->pattern) > ln)
                    Strcat(strncat(mtbuf, tmp->pattern, ln - 3), "...\"");
                else
                    Strcat(strcat(mtbuf, tmp->pattern), "\"");
                add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, mtbuf,
                         MENU_UNSELECTED);
                tmp = tmp->next;
            }
            Sprintf(mtbuf, "%s message types",
                    (opt_idx == 1) ? "List of" : "Remove which");
            end_menu(tmpwin, mtbuf);
            pick_cnt = select_menu(tmpwin,
                                   (opt_idx == 1) ? PICK_NONE : PICK_ANY,
                                   &pick_list);
            if (pick_cnt > 0) {
                for (pick_idx = 0; pick_idx < pick_cnt; ++pick_idx)
                    free_one_msgtype(pick_list[pick_idx].item.a_int - 1
                                           - pick_idx);
                free((genericptr_t) pick_list), pick_list = (menu_item *) 0;
            }
            destroy_nhwindow(tmpwin);
            if (pick_cnt >= 0)
                goto msgtypes_again;
        }
    } else if (!strcmp("menu_colors", optname)) {
        int opt_idx, nmc, mcclr, mcattr;
        char mcbuf[BUFSZ] = DUMMY;

 menucolors_again:
        nmc = count_menucolors();
        opt_idx = handle_add_list_remove("menucolor", nmc);
        if (opt_idx == 3) { /* done */
 menucolors_done:
            if (nmc > 0 && !iflags.use_menu_color)
                pline(
    "To have menu colors become active, toggle 'menucolors' option to True.");
            return TRUE;
        } else if (opt_idx == 0) { /* add new */
            getlin("What new menucolor pattern?", mcbuf);
            if (*mcbuf == '\033')
                goto menucolors_done;
            if (*mcbuf
                && test_regex_pattern(mcbuf, (const char *)0)
                && (mcclr = query_color((char *) 0)) != -1
                && (mcattr = query_attr((char *) 0)) != -1
                && !add_menu_coloring_parsed(mcbuf, mcclr, mcattr)) {
                pline("Error adding the menu color.");
                wait_synch();
            }
            goto menucolors_again;
        } else { /* list (1) or remove (2) */
            int pick_idx, pick_cnt;
            int mc_idx;
            size_t ln;
            const char *sattr, *sclr;
            menu_item *pick_list = (menu_item *) 0;
            struct menucoloring *tmp = menu_colorings;
            char clrbuf[QBUFSZ];

            tmpwin = create_nhwindow(NHW_MENU);
            start_menu(tmpwin);
            any = zeroany;
            mc_idx = 0;
            while (tmp) {
                sattr = attr2attrname(tmp->attr);
                sclr = strcpy(clrbuf, clr2colorname(tmp->color));
                (void) strNsubst(clrbuf, " ", "-", 0);
                any.a_int = ++mc_idx;
                /* construct suffix */
                Sprintf(buf, "\"\"=%s%s%s", sclr,
                        (tmp->attr != ATR_NONE) ? "&" : "",
                        (tmp->attr != ATR_NONE) ? sattr : "");
                /* now main string */
                ln = sizeof buf - strlen(buf) - 1; /* length available */
                Strcpy(mcbuf, "\"");
                if (strlen(tmp->origstr) > ln)
                    Strcat(strncat(mcbuf, tmp->origstr, ln - 3), "...");
                else
                    Strcat(mcbuf, tmp->origstr);
                /* combine main string and suffix */
                Strcat(mcbuf, &buf[1]); /* skip buf[]'s initial quote */
                add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, mcbuf,
                         MENU_UNSELECTED);
                tmp = tmp->next;
            }
            Sprintf(mcbuf, "%s menu colors",
                    (opt_idx == 1) ? "List of" : "Remove which");
            end_menu(tmpwin, mcbuf);
            pick_cnt = select_menu(tmpwin,
                                   (opt_idx == 1) ? PICK_NONE : PICK_ANY,
                                   &pick_list);
            if (pick_cnt > 0) {
                for (pick_idx = 0; pick_idx < pick_cnt; ++pick_idx)
                    free_one_menu_coloring(pick_list[pick_idx].item.a_int - 1
                                           - pick_idx);
                free((genericptr_t) pick_list), pick_list = (menu_item *) 0;
            }
            destroy_nhwindow(tmpwin);
            if (pick_cnt >= 0)
                goto menucolors_again;
        }
    } else if (!strcmp("autopickup_exception", optname)) {
        int opt_idx, pass, totalapes = 0, numapes[2] = { 0, 0 };
        char apebuf[1 + BUFSZ]; /* so &apebuf[1] is BUFSZ long for getlin() */
        struct autopickup_exception *ape;

 ape_again:
        totalapes = count_ape_maps(&numapes[AP_LEAVE], &numapes[AP_GRAB]);
        opt_idx = handle_add_list_remove("autopickup exception", totalapes);
        if (opt_idx == 3) { /* done */
            return TRUE;
        } else if (opt_idx == 0) { /* add new */
            /* EDIT_GETLIN:  assume user doesn't user want previous
               exception used as default input string for this one... */
            apebuf[0] = apebuf[1] = '\0';
            getlin("What new autopickup exception pattern?", &apebuf[1]);
            mungspaces(&apebuf[1]); /* regularize whitespace */
            if (apebuf[1] == '\033')
                return TRUE;
            if (apebuf[1]) {
                apebuf[0] = '\"';
                /* guarantee room for \" prefix and \"\0 suffix;
                   -2 is good enough for apebuf[] but -3 makes
                   sure the whole thing fits within normal BUFSZ */
                apebuf[sizeof apebuf - 3] = '\0';
                Strcat(apebuf, "\"");
                add_autopickup_exception(apebuf);
            }
            goto ape_again;
        } else { /* list (1) or remove (2) */
            int pick_idx, pick_cnt;
            menu_item *pick_list = (menu_item *) 0;

            tmpwin = create_nhwindow(NHW_MENU);
            start_menu(tmpwin);
            for (pass = AP_LEAVE; pass <= AP_GRAB; ++pass) {
                if (numapes[pass] == 0)
                    continue;
                ape = iflags.autopickup_exceptions[pass];
                any = zeroany;
                add_extended_menu(tmpwin, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR,
                         (pass == 0) ? "Never pickup" : "Always pickup",
                         MENU_UNSELECTED, menu_heading_info());
                for (i = 0; i < numapes[pass] && ape; i++) {
                    any.a_void = (opt_idx == 1) ? 0 : ape;
                    /* length of pattern plus quotes is less than BUFSZ */
                    Sprintf(apebuf, "\"%s\"", ape->pattern);
                    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, apebuf,
                             MENU_UNSELECTED);
                    ape = ape->next;
                }
            }
            Sprintf(apebuf, "%s autopickup exceptions",
                    (opt_idx == 1) ? "List of" : "Remove which");
            end_menu(tmpwin, apebuf);
            pick_cnt = select_menu(tmpwin,
                                   (opt_idx == 1) ? PICK_NONE : PICK_ANY,
                                   &pick_list);
            if (pick_cnt > 0) {
                for (pick_idx = 0; pick_idx < pick_cnt; ++pick_idx)
                    remove_autopickup_exception(
                                         (struct autopickup_exception *)
                                             pick_list[pick_idx].item.a_void);
                free((genericptr_t) pick_list), pick_list = (menu_item *) 0;
            }
            destroy_nhwindow(tmpwin);
            if (pick_cnt >= 0)
                goto ape_again;
        }
    } else if (!strcmp("symset", optname)
               || !strcmp("roguesymset", optname)) {
        menu_item *symset_pick = (menu_item *) 0;
        boolean rogueflag = (*optname == 'r'),
                ready_to_switch = FALSE,
                nothing_to_do = FALSE;
        char *symset_name, fmtstr[20];
        struct symsetentry *sl;
        int res, which_set, setcount = 0, chosen = -2;

        which_set = rogueflag ? ROGUESET : PRIMARY;
        symset_list = (struct symsetentry *) 0;
        /* clear symset[].name as a flag to read_sym_file() to build list */
        symset_name = symset[which_set].name;
        symset[which_set].name = (char *) 0;

        res = read_sym_file(which_set);
        /* put symset name back */
        symset[which_set].name = symset_name;

        if (res && symset_list) {
            int thissize, biggest = 0;

            for (sl = symset_list; sl; sl = sl->next) {
                /* check restrictions */
                if (rogueflag ? sl->primary : sl->rogue)
                    continue;

                setcount++;
                /* find biggest name */
                thissize = sl->name ? (int) strlen(sl->name) : 0;
                if (thissize > biggest)
                    biggest = thissize;
            }
            if (!setcount) {
                pline("There are no appropriate %s symbol sets available.",
                      rogueflag ? "rogue level" : "primary");
                return TRUE;
            }

            Sprintf(fmtstr, "%%-%ds %%s", biggest + 5);
            tmpwin = create_nhwindow(NHW_MENU);
            start_menu(tmpwin);
            any = zeroany;
            any.a_int = 1;
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                     "Default Symbols", MENU_UNSELECTED);

            for (sl = symset_list; sl; sl = sl->next) {
                /* check restrictions */
                if (rogueflag ? sl->primary : sl->rogue)
                    continue;

                if (sl->name) {
                    any.a_int = sl->idx + 2;
                    Sprintf(buf, fmtstr, sl->name, sl->desc ? sl->desc : "");
                    add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                             buf, MENU_UNSELECTED);
                }
            }
            Sprintf(buf, "Select %ssymbol set:",
                    rogueflag ? "rogue level " : "");
            end_menu(tmpwin, buf);
            if (select_menu(tmpwin, PICK_ONE, &symset_pick) > 0) {
                chosen = symset_pick->item.a_int - 2;
                free((genericptr_t) symset_pick);
            }
            destroy_nhwindow(tmpwin);

            if (chosen > -1) {
                /* chose an actual symset name from file */
                for (sl = symset_list; sl; sl = sl->next)
                    if (sl->idx == chosen)
                        break;
                if (sl) {
                    /* free the now stale attributes */
                    clear_symsetentry(which_set, TRUE);

                    /* transfer only the name of the symbol set */
                    symset[which_set].name = dupstr(sl->name);
                    ready_to_switch = TRUE;
                }
            } else if (chosen == -1) {
                /* explicit selection of defaults */
                /* free the now stale symset attributes */
                clear_symsetentry(which_set, TRUE);
            } else
                nothing_to_do = TRUE;
        } else if (!res) {
            /* The symbols file could not be accessed */
            pline("Unable to access \"%s\" file.", SYMBOLS);
            return TRUE;
        } else if (!symset_list) {
            /* The symbols file was empty */
            pline("There were no symbol sets found in \"%s\".", SYMBOLS);
            return TRUE;
        }

        /* clean up */
        while ((sl = symset_list) != 0) {
            symset_list = sl->next;
            if (sl->name)
                free((genericptr_t) sl->name), sl->name = (char *) 0;
            if (sl->desc)
                free((genericptr_t) sl->desc), sl->desc = (char *) 0;
            free((genericptr_t) sl);
        }

        if (nothing_to_do)
            return TRUE;

        /* Set default symbols and clear the handling value */
        if (rogueflag)
            init_r_symbols();
        else
            init_l_symbols();

        if (symset[which_set].name) {
            /* non-default symbols */
            if (read_sym_file(which_set)) {
                ready_to_switch = TRUE;
            } else {
                clear_symsetentry(which_set, TRUE);
                return TRUE;
            }
        }

        if (ready_to_switch)
            switch_symbols(TRUE);

        if (Is_really_rogue_level(&u.uz)) {
            if (rogueflag)
                assign_graphics(ROGUESET);
        } else if (!rogueflag)
            assign_graphics(PRIMARY);
        preference_update("symset");
        need_issue_gui_command = TRUE;
        need_stretch_map = TRUE;
        need_redraw = TRUE;

    } else {
        /* didn't match any of the special options */
        return FALSE;
    }
    return TRUE;
}

#define rolestring(val, array, field) \
    ((val >= 0) ? array[val].field : (val == ROLE_RANDOM) ? randomrole : none)

/* This is ugly. We have all the option names in the compopt[] array,
   but we need to look at each option individually to get the value. */
STATIC_OVL const char *
get_compopt_value(optname, buf)
const char *optname;
char *buf;
{
    static const char none[] = "(none)", randomrole[] = "random",
                      to_be_done[] = "(to be done)",
                      defopt[] = "default", defbrief[] = "def";
    char ocl[MAX_OBJECT_CLASSES + 1];
    int i;

    buf[0] = '\0';
    if (!strcmp(optname, "align_message")
        || !strcmp(optname, "align_status")) {
        int which = !strcmp(optname, "align_status") ? iflags.wc_align_status
                                                     : iflags.wc_align_message;
        Sprintf(buf, "%s",
                (which == ALIGN_TOP) ? "top"
                : (which == ALIGN_LEFT) ? "left"
                  : (which == ALIGN_BOTTOM) ? "bottom"
                    : (which == ALIGN_RIGHT) ? "right"
                      : defopt);
    }
    else if (!strcmp(optname, "animation_interval"))
    {
        if(flags.animation_frame_interval_in_milliseconds > 0)
            Sprintf(buf, "%d ms", flags.animation_frame_interval_in_milliseconds);
        else
            Sprintf(buf, "%s, %d ms", defopt, ANIMATION_FRAME_INTERVAL);
    }
    else if (!strcmp(optname, "move_interval"))
    {
        if (flags.move_interval_in_milliseconds > 0)
            Sprintf(buf, "%d ms", flags.move_interval_in_milliseconds);
        else
            Sprintf(buf, "%s, %d ms", defopt, DEFAULT_MOVE_INTERVAL);
    }
    else if (!strcmp(optname, "crawl_interval"))
    {
        if (flags.crawl_interval_in_milliseconds > 0)
            Sprintf(buf, "%d ms", flags.crawl_interval_in_milliseconds);
        else
            Sprintf(buf, "%s, %d ms", defopt, DEFAULT_CRAWL_INTERVAL);
    }
    else if (!strcmp(optname, "align"))
        Sprintf(buf, "%s", rolestring(flags.initalign, aligns, adj));
#ifdef WIN32
    else if (!strcmp(optname, "altkeyhandler"))
        Sprintf(buf, "%s",
                iflags.altkeyhandler[0] ? iflags.altkeyhandler : "default");
#endif
#ifdef BACKWARD_COMPAT
    else if (!strcmp(optname, "boulder"))
        Sprintf(buf, "%lc", (wint_t)(iflags.bouldersym ? iflags.bouldersym
                    : showsyms[(int) objects[BOULDER].oc_class + SYM_OFF_O]));
#endif
    else if (!strcmp(optname, "catname"))
        Sprintf(buf, "%s", catname[0] ? catname : none);
    else if (!strcmp(optname, "disclose"))
        for (i = 0; i < NUM_DISCLOSURE_OPTIONS; i++) {
            if (i)
                (void) strkitten(buf, ' ');
            (void) strkitten(buf, flags.end_disclose[i]);
            (void) strkitten(buf, disclosure_options[i]);
        }
    else if (!strcmp(optname, "dogname"))
        Sprintf(buf, "%s", dogname[0] ? dogname : none);
    else if (!strcmp(optname, "dungeon"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "effects"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "font_map"))
        Sprintf(buf, "%s", iflags.wc_font_map ? iflags.wc_font_map : defopt);
    else if (!strcmp(optname, "font_message"))
        Sprintf(buf, "%s",
                iflags.wc_font_message ? iflags.wc_font_message : defopt);
    else if (!strcmp(optname, "font_status"))
        Sprintf(buf, "%s",
                iflags.wc_font_status ? iflags.wc_font_status : defopt);
    else if (!strcmp(optname, "font_menu"))
        Sprintf(buf, "%s",
                iflags.wc_font_menu ? iflags.wc_font_menu : defopt);
    else if (!strcmp(optname, "font_text"))
        Sprintf(buf, "%s",
                iflags.wc_font_text ? iflags.wc_font_text : defopt);
    else if (!strcmp(optname, "font_size_map")) {
        if (iflags.wc_fontsiz_map)
            Sprintf(buf, "%d", iflags.wc_fontsiz_map);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "font_size_message")) {
        if (iflags.wc_fontsiz_message)
            Sprintf(buf, "%d", iflags.wc_fontsiz_message);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "font_size_status")) {
        if (iflags.wc_fontsiz_status)
            Sprintf(buf, "%d", iflags.wc_fontsiz_status);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "font_size_menu")) {
        if (iflags.wc_fontsiz_menu)
            Sprintf(buf, "%d", iflags.wc_fontsiz_menu);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "font_size_text")) {
        if (iflags.wc_fontsiz_text)
            Sprintf(buf, "%d", iflags.wc_fontsiz_text);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "fruit"))
        Sprintf(buf, "%s", pl_fruit);
    else if (!strcmp(optname, "gender"))
        Sprintf(buf, "%s", rolestring(flags.initgend, genders, adj));
    else if (!strcmp(optname, "horsename"))
        Sprintf(buf, "%s", horsename[0] ? horsename : none);
    else if (!strcmp(optname, "ramname"))
        Sprintf(buf, "%s", ramname[0] ? ramname : none);
    else if (!strcmp(optname, "luggagename"))
        Sprintf(buf, "%s", luggagename[0] ? luggagename : none);
    else if (!strcmp(optname, "wolfname"))
        Sprintf(buf, "%s", wolfname[0] ? wolfname : none);
    else if (!strcmp(optname, "map_mode")) {
        i = iflags.wc_map_mode;
        Sprintf(buf, "%s",
                (i == MAP_MODE_TILES) ? "tiles"
                : (i == MAP_MODE_ASCII4x6) ? "ascii4x6"
                  : (i == MAP_MODE_ASCII6x8) ? "ascii6x8"
                    : (i == MAP_MODE_ASCII8x8) ? "ascii8x8"
                      : (i == MAP_MODE_ASCII16x8) ? "ascii16x8"
                        : (i == MAP_MODE_ASCII7x12) ? "ascii7x12"
                          : (i == MAP_MODE_ASCII8x12) ? "ascii8x12"
                            : (i == MAP_MODE_ASCII16x12) ? "ascii16x12"
                              : (i == MAP_MODE_ASCII12x16) ? "ascii12x16"
                                : (i == MAP_MODE_ASCII10x18) ? "ascii10x18"
                                  : (i == MAP_MODE_ASCII_FIT_TO_SCREEN)
                                    ? "fit_to_screen"
                                    : defopt);
    } else if (!strcmp(optname, "menustyle"))
        Sprintf(buf, "%s", menutype[(int) flags.menu_style]);
    else if (!strcmp(optname, "menu_deselect_all"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_deselect_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_first_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_invert_all"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_headings"))
        Sprintf(buf, "%s", attr2attrname(iflags.menu_headings));
    else if (!strcmp(optname, "menu_invert_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_last_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_next_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_previous_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_search"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_select_all"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "menu_select_page"))
        Sprintf(buf, "%s", to_be_done);
    else if (!strcmp(optname, "monsters")) {
        Sprintf(buf, "%s", to_be_done);
    } else if (!strcmp(optname, "msghistory")) {
        Sprintf(buf, "%u", iflags.msg_history);
#ifdef TTY_GRAPHICS
    } else if (!strcmp(optname, "msg_window")) {
        Sprintf(buf, "%s", (iflags.prevmsg_window == 's') ? "single"
                           : (iflags.prevmsg_window == 'c') ? "combination"
                             : (iflags.prevmsg_window == 'f') ? "full"
                               : "reversed");
#endif
    } else if (!strcmp(optname, "name")) {
        Sprintf(buf, "%s", plname);
    } else if (!strcmp(optname, "mouse_support")) {
#ifdef WIN32
#define MOUSEFIX1 ", QuickEdit off"
#define MOUSEFIX2 ", QuickEdit unchanged"
#else
#define MOUSEFIX1 ", O/S adjusted"
#define MOUSEFIX2 ", O/S unchanged"
#endif
        static const char *mousemodes[][2] = {
            { "0=off", "" },
            { "1=on",  MOUSEFIX1 },
            { "2=on",  MOUSEFIX2 },
        };
#undef MOUSEFIX1
#undef MOUSEFIX2
        int ms = iflags.wc_mouse_support;

        if (ms >= 0 && ms <= 2)
            Sprintf(buf, "%s%s", mousemodes[ms][0], mousemodes[ms][1]);
    } else if (!strcmp(optname, "number_pad")) {
        static const char *numpadmodes[] = {
            "0=off", "1=on", "2=on, MSDOS compatible",
            "3=on, phone-style layout",
            "4=on, phone layout, MSDOS compatible",
            "-1=off, y & z swapped", /*[5]*/
            "-2=off, yuhj diagonal movement", /*[6]*/
        };
        int indx = Cmd.num_pad
                       ? (Cmd.phone_layout ? (Cmd.pcHack_compat ? 4 : 3)
                                           : (Cmd.pcHack_compat ? 2 : 1))
                       : (Cmd.gnh_layout ? 6 : Cmd.swap_yz ? 5 : 0);

        Strcpy(buf, numpadmodes[indx]);
    } else if (!strcmp(optname, "objects")) {
        Sprintf(buf, "%s", to_be_done);
    } else if (!strcmp(optname, "packorder")) {
        oc_to_str(flags.inv_order, ocl);
        Sprintf(buf, "%s", ocl);
#ifdef CHANGE_COLOR
    } else if (!strcmp(optname, "palette")) {
        Sprintf(buf, "%s", get_color_string());
#endif
    } else if (!strcmp(optname, "paranoid_confirmation")) {
        char tmpbuf[QBUFSZ];

        tmpbuf[0] = '\0';
        for (i = 0; paranoia[i].flagmask != 0; ++i)
            if (flags.paranoia_bits & paranoia[i].flagmask)
                Sprintf(eos(tmpbuf), " %s", paranoia[i].argname);
        Strcpy(buf, tmpbuf[0] ? &tmpbuf[1] : "none");
    } else if (!strcmp(optname, "petattr")) {
#ifdef CURSES_GRAPHICS
        if (WINDOWPORT("curses")) {
            char tmpbuf[QBUFSZ];

            Strcpy(buf, curses_fmt_attrs(tmpbuf));
        } else
#endif
        if (iflags.wc2_petattr != 0)
            Sprintf(buf, "0x%08x", iflags.wc2_petattr);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "pettype")) {
        Sprintf(buf, "%s", (preferred_pet == 'c') ? "cat"
                           : (preferred_pet == 'd') ? "dog"
                             : (preferred_pet == 'h') ? "horse"
                               : (preferred_pet == 'n') ? "none"
                                 : "random");
    } else if (!strcmp(optname, "doggender")) {
        Sprintf(buf, "%s", (doggender == 1) ? "male"
                           : (doggender == 2) ? "female"
                                 : "random");
    } else if (!strcmp(optname, "catgender")) {
        Sprintf(buf, "%s", (catgender == 1) ? "male"
                           : (catgender == 2) ? "female"
                                 : "random");
    } else if (!strcmp(optname, "horsegender")) {
        Sprintf(buf, "%s", (horsegender == 1) ? "male"
                           : (horsegender == 2) ? "female"
                                 : "random");
    } else if (!strcmp(optname, "ramgender")) {
        Sprintf(buf, "%s", (ramgender == 1) ? "male"
                           : (ramgender == 2) ? "female"
                                 : "random");
    } else if (!strcmp(optname, "wolfgender")) {
        Sprintf(buf, "%s", (wolfgender == 1) ? "male"
                           : (wolfgender == 2) ? "female"
                                 : "random");
    } else if (!strcmp(optname, "dogbreed")) {
        Sprintf(buf, "%s", !dogbreed ? "generic"
                           : (dogbreed >= NUM_DOG_BREEDS) ? "invalid breed"
                                 : dog_breed_definitions[dogbreed].name ? dog_breed_definitions[dogbreed].name : "unnamed breed");
    } else if (!strcmp(optname, "catbreed")) {
        Sprintf(buf, "%s", !catbreed ? "generic"
                           : (catbreed >= NUM_CAT_BREEDS) ? "invalid breed"
                                 : cat_breed_definitions[dogbreed].name ? cat_breed_definitions[dogbreed].name : "unnamed breed");
    } else if (!strcmp(optname, "pickup_burden")) {
        Sprintf(buf, "%s", burdentype[flags.pickup_burden]);
    } else if (!strcmp(optname, "pickup_types")) {
        oc_to_str(flags.pickup_types, ocl);
        Sprintf(buf, "%s", ocl[0] ? ocl : "all");
    } else if (!strcmp(optname, "pile_limit")) {
        Sprintf(buf, "%d", flags.pile_limit);
    } else if (!strcmp(optname, "playmode")) {
        Strcpy(buf, wizard ? "debug" : discover ? "explore" : CasualMode ? (ModernMode ? "casual" : "reloadable") : ModernMode ? "modern" : "normal");
    } else if (!strcmp(optname, "preferred_screen_scale")) {
        if (flags.preferred_screen_scale)
            Sprintf(buf, "%d", flags.preferred_screen_scale);
        else
            Sprintf(buf, "%s, 100", defopt);
    } else if (!strcmp(optname, "shield_effect_length")) {
        if (flags.shield_effect_length)
            Sprintf(buf, "%d", flags.shield_effect_length);
        else
            Sprintf(buf, "%s, %d", defopt, GNH_DEF_SHIELD_COUNT);
    } else if (!strcmp(optname, "talk_effect_length")) {
        if (flags.talk_effect_length)
            Sprintf(buf, "%d", flags.talk_effect_length);
        else
            Sprintf(buf, "%s, %d", defopt, GNH_DEF_TALK_COUNT);
    } else if (!strcmp(optname, "last_item_show_duration")) {
        if (flags.last_item_show_duration)
            Sprintf(buf, "%d", flags.last_item_show_duration);
        else
            Sprintf(buf, "%s, %d", defopt, DEF_LAST_ITEM_SHOW_DURATION);
    } else if (!strcmp(optname, "race")) {
        Sprintf(buf, "%s", rolestring(flags.initrace, races, noun));
    } else if (!strcmp(optname, "roguesymset")) {
        Sprintf(buf, "%s",
                symset[ROGUESET].name ? symset[ROGUESET].name : "default");
        if (currentgraphics == ROGUESET && symset[ROGUESET].name)
            Strcat(buf, ", active");
    } else if (!strcmp(optname, "role")) {
        Sprintf(buf, "%s", rolestring(flags.initrole, roles, name.m));
    } else if (!strcmp(optname, "runmode")) {
        Sprintf(buf, "%s", runmodes[flags.runmode]);
    } else if (!strcmp(optname, "whatis_coord")) {
        Sprintf(buf, "%s",
                (iflags.getpos_coords == GPCOORDS_MAP) ? "map"
                : (iflags.getpos_coords == GPCOORDS_COMPASS) ? "compass"
                : (iflags.getpos_coords == GPCOORDS_COMFULL) ? "full compass"
                : (iflags.getpos_coords == GPCOORDS_SCREEN) ? "screen"
                : "none");
    } else if (!strcmp(optname, "whatis_filter")) {
        Sprintf(buf, "%s",
                (iflags.getloc_filter == GFILTER_VIEW) ? "view"
                : (iflags.getloc_filter == GFILTER_AREA) ? "area"
                : "none");
    } else if (!strcmp(optname, "scores")) {
        Sprintf(buf, "%d top/%d around%s", flags.end_top, flags.end_around,
                flags.end_own ? "/own" : "");
    } else if (!strcmp(optname, "scroll_amount")) {
        if (iflags.wc_scroll_amount)
            Sprintf(buf, "%d", iflags.wc_scroll_amount);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "scroll_margin")) {
        if (iflags.wc_scroll_margin)
            Sprintf(buf, "%d", iflags.wc_scroll_margin);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "sortloot")) {
        for (i = 0; i < SIZE(sortltype); i++)
            if (flags.sortloot == sortltype[i][0]) {
                Strcpy(buf, sortltype[i]);
                break;
            }
    } else if (!strcmp(optname, "player_selection")) {
        Sprintf(buf, "%s", iflags.wc_player_selection ? "prompts" : "dialog");
#ifdef MSDOS
    } else if (!strcmp(optname, "soundcard")) {
        Sprintf(buf, "%s", to_be_done);
#endif
#ifdef STATUS_HILITES
    } else if (!strcmp("statushilites", optname)) {
        if (!iflags.hilite_delta)
            Strcpy(buf, "0 (off: don't highlight status fields)");
        else
            Sprintf(buf, "%lld (on: highlight status for %lld turns)",
                (long long)iflags.hilite_delta, (long long)iflags.hilite_delta);
#endif
    } 
    else if (!strcmp(optname,"statuslines"))
    {
        if (wc2_supported(optname))
        { 
            boolean auto_on = wc2_supported("autostatuslines") && iflags.wc2_autostatuslines;
            Sprintf(buf, "%d%s", (iflags.wc2_statuslines < 3) ? 2 : (iflags.wc2_statuslines > 7) ? 8 : iflags.wc2_statuslines, auto_on ? ", auto" : "");
        }
        /* else default to "unknown" */
    } 
    else if (!strcmp(optname, "here_window_size"))
    {
        if (wc2_supported(optname))
        {
            Sprintf(buf, "%d", (iflags.wc2_here_window_size < 3) ? 2 : (iflags.wc2_here_window_size > 20) ? 20 : iflags.wc2_here_window_size);
        }
        /* else default to "unknown" */
    }
    else if (!strcmp(optname, "preferred_screen_scale"))
    {
        if (wc2_supported(optname) && flags.preferred_screen_scale != 0)
        {
            Sprintf(buf, "%d", flags.preferred_screen_scale);
        }
        /* else default to "unknown" */
    }
    else if (!strcmp(optname, "shield_effect_length"))
    {
        Sprintf(buf, "%d", flags.shield_effect_length);
    }
    else if (!strcmp(optname, "talk_effect_length"))
    {
        Sprintf(buf, "%d", flags.talk_effect_length);
    }
    else if (!strcmp(optname, "last_item_show_duration"))
    {
        Sprintf(buf, "%d", flags.last_item_show_duration);
    }
    else if (!strcmp(optname, "max_hint_difficulty"))
    {
        char dlbuf[BUFSZ];
        Strcpy(dlbuf, flags.max_hint_difficulty < MIN_DIFFICULTY_LEVEL ? "off" : get_game_difficulty_text(flags.max_hint_difficulty));
        Sprintf(buf, "%s (%d)", dlbuf, (int)flags.max_hint_difficulty);
    }
    else if (!strcmp(optname, "right_click_command") || !strcmp(optname, "middle_click_command"))
    {
        boolean is_middle = !strcmp(optname, "middle_click_command");
        uchar cmdidx = is_middle ? flags.middle_click_command : flags.right_click_command;
        Sprintf(buf, "%s", cmdidx >= MAX_CLICK_TYPES ? "invalid" : mouse_cmd_names[cmdidx]);
    }
    else if (!strcmp(optname, "run_spot_distance"))
    {
        if(iflags.run_spot_distance <= 0)
            Sprintf(buf, "%s (%d)", iflags.run_spot_distance < 0 ? "infinity" : "off", iflags.run_spot_distance);
        else
            Sprintf(buf, "%d squares", iflags.run_spot_distance);
    }
    else if (!strcmp(optname, "sound_volume_ambient"))
    {
        Sprintf(buf, "%d", flags.sound_volume_ambient);
    }
    else if (!strcmp(optname, "sound_volume_dialogue"))
    {
        Sprintf(buf, "%d", (int)flags.sound_volume_dialogue);
    }
    else if (!strcmp(optname, "sound_volume_effects"))
    {
        Sprintf(buf, "%d", (int)flags.sound_volume_effects);
    }
    else if (!strcmp(optname, "sound_volume_general"))
    {
        Sprintf(buf, "%d", (int)flags.sound_volume_general);
    }
    else if (!strcmp(optname, "sound_volume_music"))
    {
        Sprintf(buf, "%d", flags.sound_volume_music);
    }
    else if (!strcmp(optname, "sound_volume_ui"))
    {
       Sprintf(buf, "%d", (int)flags.sound_volume_ui);
    }
    else if (!strcmp(optname, "spellorder"))
    {
       Sprintf(buf, "%d (%s)", (int)flags.spellorder, spl_sortchoices[flags.spellorder]);
    }
    else if (!strcmp(optname, "suppress_alert")) {
        if (flags.suppress_alert == 0L)
            Strcpy(buf, none);
        else
            Sprintf(buf, "%llu.%llu.%llu", (unsigned long long)FEATURE_NOTICE_VER_MAJ,
                (unsigned long long)FEATURE_NOTICE_VER_MIN, (unsigned long long)FEATURE_NOTICE_VER_PATCH);
    } else if (!strcmp(optname, "symset")) {
        Sprintf(buf, "%s",
                symset[PRIMARY].name ? symset[PRIMARY].name : "default");
        if (currentgraphics == PRIMARY && symset[PRIMARY].name)
            Strcat(buf, ", active");
    } else if (!strcmp(optname, "term_cols")) {
        if (iflags.wc2_term_cols)
            Sprintf(buf, "%d", iflags.wc2_term_cols);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "term_rows")) {
        if (iflags.wc2_term_rows)
            Sprintf(buf, "%d", iflags.wc2_term_rows);
        else
            Strcpy(buf, defopt);
    } 
    else if (!strncmp(optname, "tile_file", 9)) 
    {
        int tile_sheet_idx;
        for (tile_sheet_idx = 0; tile_sheet_idx < MAX_TILE_SHEETS; tile_sheet_idx++)
        {
            char optbuf[BUFSZ] = "";
            Sprintf(optbuf, "%s_%d", optname, tile_sheet_idx + 1);
            if (strcmp(optname, optbuf))
            {
                Sprintf(buf, "%s",
                    iflags.wc_tile_file[tile_sheet_idx] ? iflags.wc_tile_file[tile_sheet_idx] : defopt);
            }
        }
    }
    else if (!strcmp(optname, "tile_height")) {
        if (iflags.wc_tile_height)
            Sprintf(buf, "%d", iflags.wc_tile_height);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "tile_width")) {
        if (iflags.wc_tile_width)
            Sprintf(buf, "%d", iflags.wc_tile_width);
        else
            Strcpy(buf, defopt);
    } else if (!strcmp(optname, "traps")) {
        Sprintf(buf, "%s", to_be_done);
    } else if (!strcmp(optname, "vary_msgcount")) {
        if (iflags.wc_vary_msgcount)
            Sprintf(buf, "%d", iflags.wc_vary_msgcount);
        else
            Strcpy(buf, defopt);
#ifdef MSDOS
    } else if (!strcmp(optname, "video")) {
        Sprintf(buf, "%s", to_be_done);
#endif
#ifdef VIDEOSHADES
    } else if (!strcmp(optname, "videoshades")) {
        Sprintf(buf, "%s-%s-%s", shade[0], shade[1], shade[2]);
    } else if (!strcmp(optname, "videocolors")) {
        Sprintf(buf, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d",
                ttycolors[CLR_RED], ttycolors[CLR_GREEN],
                ttycolors[CLR_BROWN], ttycolors[CLR_BLUE],
                ttycolors[CLR_MAGENTA], ttycolors[CLR_CYAN],
                ttycolors[CLR_ORANGE], ttycolors[CLR_BRIGHT_GREEN],
                ttycolors[CLR_YELLOW], ttycolors[CLR_BRIGHT_BLUE],
                ttycolors[CLR_BRIGHT_MAGENTA], ttycolors[CLR_BRIGHT_CYAN]);
#endif /* VIDEOSHADES */
    } else if (!strcmp(optname,"windowborders")) {
        Sprintf(buf, "%s",
                (iflags.wc2_windowborders == 0) ? "0=off"
                : (iflags.wc2_windowborders == 1) ? "1=on"
                  : (iflags.wc2_windowborders == 2) ? "2=auto"
                    : defopt);
    } else if (!strcmp(optname, "windowtype")) {
        Sprintf(buf, "%s", windowprocs.name);
    } else if (!strcmp(optname, "windowcolors")) {
        Sprintf(
            buf, "%s/%s %s/%s %s/%s %s/%s",
            iflags.wc_foregrnd_menu ? iflags.wc_foregrnd_menu : defbrief,
            iflags.wc_backgrnd_menu ? iflags.wc_backgrnd_menu : defbrief,
            iflags.wc_foregrnd_message ? iflags.wc_foregrnd_message
                                       : defbrief,
            iflags.wc_backgrnd_message ? iflags.wc_backgrnd_message
                                       : defbrief,
            iflags.wc_foregrnd_status ? iflags.wc_foregrnd_status : defbrief,
            iflags.wc_backgrnd_status ? iflags.wc_backgrnd_status : defbrief,
            iflags.wc_foregrnd_text ? iflags.wc_foregrnd_text : defbrief,
            iflags.wc_backgrnd_text ? iflags.wc_backgrnd_text : defbrief);
#ifdef PREFIXES_IN_USE
    } else {
        for (i = 0; i < PREFIX_COUNT; ++i)
            if (!strcmp(optname, fqn_prefix_names[i]) && fqn_prefix[i])
                Sprintf(buf, "%s", fqn_prefix[i]);
#endif
    }

    if (!buf[0])
        Strcpy(buf, "unknown");
    return buf;
}

int
dotogglepickup()
{
    char buf[BUFSZ], ocl[MAX_OBJECT_CLASSES + 1];
    int color = NO_COLOR;

    flags.pickup = !flags.pickup;
    if (flags.pickup) 
    {
        color = CLR_GREEN;
        oc_to_str(flags.pickup_types, ocl);
        Sprintf(buf, "ON, for %s objects%s", ocl[0] ? ocl : "all",
                (iflags.autopickup_exceptions[AP_LEAVE]
                 || iflags.autopickup_exceptions[AP_GRAB])
                    ? ((count_ape_maps((int *) 0, (int *) 0) == 1)
                           ? ", with one exception"
                           : ", with some exceptions")
                    : "");
    } 
    else 
    {
        color = CLR_RED;
        Strcpy(buf, "OFF");
    }
    custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "Autopickup: ", ATR_NONE, NO_COLOR, "", ATR_NONE, color, 0UL, "%s.", buf);
    return 0;
}

int
add_autopickup_exception(mapping)
const char *mapping;
{
    static const char
        APE_regex_error[] = "regex error in AUTOPICKUP_EXCEPTION",
        APE_syntax_error[] = "syntax error in AUTOPICKUP_EXCEPTION";

    struct autopickup_exception *ape, **apehead;
    char text[256], end;
    int n;
    boolean grab = FALSE;

    /* scan length limit used to be 255, but smaller size allows the
       quoted value to fit within BUFSZ, simplifying formatting elsewhere;
       this used to ignore the possibility of trailing junk but now checks
       for it, accepting whitespace but rejecting anything else unless it
       starts with '#" for a comment */
    end = '\0';
    if ((n = sscanf(mapping, "\"<%253[^\"]\" %c", text, &end)) == 1
        || (n == 2 && end == '#')) {
        grab = TRUE;
    } else if ((n = sscanf(mapping, "\">%253[^\"]\" %c", text, &end)) == 1
               || (n = sscanf(mapping, "\"%253[^\"]\" %c", text, &end)) == 1
               || (n == 2 && end == '#')) {
        grab = FALSE;
    } else {
        config_error_add("%s", APE_syntax_error);
        return 0;
    }

    ape = (struct autopickup_exception *) alloc(sizeof *ape);
    ape->regex = regex_init();
    if (!regex_compile(text, ape->regex)) {
        config_error_add("%s: %s", APE_regex_error,
                         regex_error_desc(ape->regex));
        regex_free(ape->regex);
        free((genericptr_t) ape);
        return 0;
    }
    apehead = (grab) ? &iflags.autopickup_exceptions[AP_GRAB]
                     : &iflags.autopickup_exceptions[AP_LEAVE];

    ape->pattern = dupstr(text);
    ape->grab = grab;
    ape->next = *apehead;
    *apehead = ape;
    return 1;
}

STATIC_OVL void
remove_autopickup_exception(whichape)
struct autopickup_exception *whichape;
{
    struct autopickup_exception *ape, *freeape, *prev = 0;
    int chain = whichape->grab ? AP_GRAB : AP_LEAVE;

    for (ape = iflags.autopickup_exceptions[chain]; ape;) {
        if (ape == whichape) {
            freeape = ape;
            ape = ape->next;
            if (prev)
                prev->next = ape;
            else
                iflags.autopickup_exceptions[chain] = ape;
            regex_free(freeape->regex);
            free((genericptr_t) freeape->pattern);
            free((genericptr_t) freeape);
        } else {
            prev = ape;
            ape = ape->next;
        }
    }
}

int
dotoggledecorations()
{
    char buf[BUFSZ];
    int color = NO_COLOR;

    flags.show_decorations = !flags.show_decorations;
    if (flags.show_decorations) 
    {
        Strcpy(buf, "ON");
        color = CLR_GREEN;
    }
    else 
    {
        Strcpy(buf, "OFF");
        color = CLR_RED;
    }
    custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "Displaying decorations: ", ATR_NONE, NO_COLOR, "", ATR_NONE, color, 0UL, "%s.", buf);
    (void)doredraw();

    return 0;
}

STATIC_OVL int
count_ape_maps(leave, grab)
int *leave, *grab;
{
    struct autopickup_exception *ape;
    int pass, totalapes, numapes[2];

    numapes[0] = numapes[1] = 0;
    for (pass = AP_LEAVE; pass <= AP_GRAB; ++pass) {
        ape = iflags.autopickup_exceptions[pass];
        while (ape) {
            ape = ape->next;
            numapes[pass]++;
        }
    }
    totalapes = numapes[AP_LEAVE] + numapes[AP_GRAB];
    if (leave)
        *leave = numapes[AP_LEAVE];
    if (grab)
        *grab = numapes[AP_GRAB];
    return totalapes;
}

void
free_autopickup_exceptions()
{
    struct autopickup_exception *ape;
    int pass;

    for (pass = AP_LEAVE; pass <= AP_GRAB; ++pass) {
        while ((ape = iflags.autopickup_exceptions[pass]) != 0) {
            regex_free(ape->regex);
            free((genericptr_t) ape->pattern);
            iflags.autopickup_exceptions[pass] = ape->next;
            free((genericptr_t) ape);
        }
    }
}

/* bundle some common usage into one easy-to-use routine */
int
load_symset(s, which_set)
const char *s;
int which_set;
{
    clear_symsetentry(which_set, TRUE);

    if (symset[which_set].name)
        free((genericptr_t) symset[which_set].name);
    symset[which_set].name = dupstr(s);

    if (read_sym_file(which_set)) {
        switch_symbols(TRUE);
    } else {
        clear_symsetentry(which_set, TRUE);
        return 0;
    }
    return 1;
}

void
free_symsets()
{
    clear_symsetentry(PRIMARY, TRUE);
    clear_symsetentry(ROGUESET, TRUE);

    /* symset_list is cleaned up as soon as it's used, so we shouldn't
       have to anything about it here */
    /* assert( symset_list == NULL ); */
}

/* Parse the value of a SYMBOLS line from a config file */
boolean
parsesymbols(opts)
register char *opts;
{
    nhsym val;
    char *op, *symname, *strval;
    struct symparse *symp;

    if ((op = index(opts, ',')) != 0) {
        *op++ = 0;
        if (!parsesymbols(op))
            return FALSE;
    }

    /* S_sample:string */
    symname = opts;
    strval = index(opts, ':');
    if (!strval)
        strval = index(opts, '=');
    if (!strval)
        return FALSE;
    *strval++ = '\0';

    /* strip leading and trailing white space from symname and strval */
    mungspaces(symname);
    mungspaces(strval);

    symp = match_sym(symname);
    if (!symp)
        return FALSE;

    if (symp->range && symp->range != SYM_CONTROL) {
        val = sym_val(strval);
        update_l_symset(symp, val);
    }
    return TRUE;
}

struct symparse *
match_sym(buf)
char *buf;
{
    size_t len = strlen(buf);
    const char *p = index(buf, ':'), *q = index(buf, '=');
    struct symparse *sp = loadsyms;

    if (!p || (q && q < p))
        p = q;
    if (p) {
        /* note: there will be at most one space before the '='
           because caller has condensed buf[] with mungspaces() */
        if (p > buf && p[-1] == ' ')
            p--;
        len = (int) (p - buf);
    }
    while (sp->range) {
        if ((len >= strlen(sp->name)) && !strncmpi(buf, sp->name, len))
            return sp;
        sp++;
    }
    return (struct symparse *) 0;
}

nhsym
sym_val(strval)
const char *strval; /* up to 4*BUFSZ-1 long; only first few chars matter */
{
    nhsym buf[QBUFSZ];
    char tmp[QBUFSZ]; /* to hold trucated copy of 'strval' */

    buf[0] = '\0';
    if (!strval[0] || !strval[1]) { /* empty, or single character */
        /* if single char is space or tab, leave buf[0]=='\0' */
        if (!isspace((uchar) strval[0]))
            buf[0] = (nhsym)strval[0];
    } else if (strval[0] == '\'') { /* single quote */
        /* simple matching single quote; we know strval[1] isn't '\0' */
        if (strval[2] == '\'' && !strval[3]) {
            /* accepts '\' as backslash and ''' as single quote */
            buf[0] = (nhsym)strval[1];

        /* if backslash, handle single or double quote or second backslash */
        } else if (strval[1] == '\\' && strval[2] && strval[3] == '\''
            && index("'\"\\", strval[2]) && !strval[4]) {
            buf[0] = (nhsym)strval[2];

        /* not simple quote or basic backslash;
           strip closing quote and let escapes() deal with it */
        } else {
            char *p;

            /* +1: skip opening single quote */
            Strncpy(tmp, strval + 1, sizeof tmp - 1);
            tmp[sizeof tmp - 1] = '\0';
            if ((p = rindex(tmp, '\'')) != 0) {
                *p = '\0';
                escapes_ex(tmp, buf, TRUE);
            } /* else buf[0] stays '\0' */
        }
    } else { /* not lone char nor single quote */
        Strncpy(tmp, strval, sizeof tmp - 1);
        tmp[sizeof tmp - 1] = '\0';
        escapes_ex(tmp, buf, TRUE);
    }

    return (nhsym) *buf;
}

/* data for option_help() */
static const char *opt_intro[] = {
    "",
    "                 GnollHack Options Help:", "",
#define CONFIG_SLOT 3 /* fill in next value at run-time */
    (char *) 0,
#if !defined(MICRO) && !defined(MAC)
    "or use `NETHACKOPTIONS=\"<options>\"' in your environment",
#endif
    "(<options> is a list of options separated by commas)",
#ifdef VMS
    "-- for example, $ DEFINE NETHACKOPTIONS \"noautopickup,fruit:kumquat\"",
#endif
    "or press \"O\" while playing and use the menu.",
    "",
 "Boolean options (which can be negated by prefixing them with '!' or \"no\"):",
    (char *) 0
};

static const char *opt_epilog[] = {
    "",
    "Some of the options can be set only before the game is started; those",
    "items will not be selectable in the 'O' command's menu.",
    (char *) 0
};

void
option_help()
{
    char buf[BUFSZ], buf2[BUFSZ];
    register int i;
    winid datawin;

    datawin = create_nhwindow(NHW_TEXT);
    Sprintf(buf, "Set options as OPTIONS=<options> in %s", configfile);
    opt_intro[CONFIG_SLOT] = (const char *) buf;
    for (i = 0; opt_intro[i]; i++)
        putstr(datawin, 0, opt_intro[i]);

    /* Boolean options */
    for (i = 0; boolopt[i].name; i++) {
        if (boolopt[i].addr) {
            if (boolopt[i].addr == &iflags.sanity_check && !wizard)
                continue;
            if (boolopt[i].addr == &iflags.menu_tab_sep && !wizard)
                continue;
            next_opt(datawin, boolopt[i].name);
        }
    }
    next_opt(datawin, "");

    /* Compound options */
    putstr(datawin, 0, "Compound options:");
    for (i = 0; compopt[i].name; i++) {
        Sprintf(buf2, "`%s'", compopt[i].name);
        Sprintf(buf, "%-20s - %s%c", buf2, compopt[i].descr,
                compopt[i + 1].name ? ',' : '.');
        putstr(datawin, 0, buf);
    }

    for (i = 0; opt_epilog[i]; i++)
        putstr(datawin, 0, opt_epilog[i]);

    display_nhwindow(datawin, FALSE);
    destroy_nhwindow(datawin);
    return;
}

/*
 * prints the next boolean option, on the same line if possible, on a new
 * line if not. End with next_opt("").
 */
void
next_opt(datawin, str)
winid datawin;
const char *str;
{
    static char *buf = 0;
    size_t i;
    char *s;

    if (!buf)
        *(buf = (char *) alloc((size_t)BUFSZ)) = '\0';

    if (!*str) {
        s = eos(buf);
        if (s > &buf[1] && s[-2] == ',')
            Strcpy(s - 2, "."); /* replace last ", " */
        i = COLNO;              /* (greater than COLNO - 2) */
    } else {
        i = strlen(buf) + strlen(str) + 2;
    }

    if (i > COLNO - 2) { /* rule of thumb */
        putstr(datawin, 0, buf);
        buf[0] = 0;
    }
    if (*str) {
        Strcat(buf, str);
        Strcat(buf, ", ");
    } else {
        putstr(datawin, 0, str);
        free((genericptr_t) buf), buf = 0;
    }
    return;
}

/* Returns the fid of the fruit type; if that type already exists, it
 * returns the fid of that one; if it does not exist, it adds a new fruit
 * type to the chain and returns the new one.
 * If replace_fruit is sent in, replace the fruit in the chain rather than
 * adding a new entry--for user specified fruits only.
 */
int
fruitadd(str, replace_fruit)
char *str;
struct fruit *replace_fruit;
{
    register int i;
    register struct fruit *f;
    int highest_fruit_id = 0, globpfx;
    char buf[PL_FSIZ], altname[PL_FSIZ];
    boolean user_specified = (str == pl_fruit);
    /* if not user-specified, then it's a fruit name for a fruit on
     * a bones level or from orctown raider's loot...
     */

    /* Note: every fruit has an id (kept in obj->special_quality) of at least 1;
     * 0 is an error.
     */
    if (user_specified) {
        boolean found = FALSE, numeric = FALSE;

        /* force fruit to be singular; this handling is not
           needed--or wanted--for fruits from bones because
           they already received it in their original game;
           str==pl_fruit but makesingular() creates a copy
           so we need to copy that back into pl_fruit */
        nmcpy(pl_fruit, makesingular(str), PL_FSIZ);
        /* (assertion doesn't matter; we use 'pl_fruit' from here on out) */
        /* assert( str == pl_fruit ); */

        /* disallow naming after other foods (since it'd be impossible
         * to tell the difference); globs might have a size prefix which
         * needs to be skipped in order to match the object type name
         */
        globpfx = (!strncmp(pl_fruit, "small ", 6)
                   || !strncmp(pl_fruit, "large ", 6)) ? 6
                  : (!strncmp(pl_fruit, "very large ", 11)) ? 11
                    : 0;
        for (i = bases[FOOD_CLASS]; objects[i].oc_class == FOOD_CLASS; i++) {
            if (!strcmp(OBJ_NAME(objects[i]), pl_fruit)
                || (globpfx > 0
                    && !strcmp(OBJ_NAME(objects[i]), &pl_fruit[globpfx]))) {
                found = TRUE;
                break;
            }
        }
        if (!found) {
            char *c;

            for (c = pl_fruit; *c >= '0' && *c <= '9'; c++)
                continue;
            if (!*c || isspace((uchar) *c))
                numeric = TRUE;
        }
        if (found || numeric
            /* these checks for applying food attributes to actual items
               are case sensitive; "glob of foo" is caught by 'found'
               if 'foo' is a valid glob; when not valid, allow it as-is */
            || !strncmp(pl_fruit, "cursed ", 7)
            || !strncmp(pl_fruit, "uncursed ", 9)
            || !strncmp(pl_fruit, "blessed ", 8)
            || !strncmp(pl_fruit, "partly eaten ", 13)
            || (!strncmp(pl_fruit, "tin of ", 7)
                && (!strcmp(pl_fruit + 7, "spinach")
                    || name_to_mon(pl_fruit + 7) >= LOW_PM))
            || !strcmp(pl_fruit, "empty tin")
            || (!strcmp(pl_fruit, "glob")
                || (globpfx > 0 && !strcmp("glob", &pl_fruit[globpfx])))
            || ((str_end_is(pl_fruit, " corpse")
                 || str_end_is(pl_fruit, " egg"))
                && name_to_mon(pl_fruit) >= LOW_PM)) {
            Strcpy(buf, pl_fruit);
            Strcpy(pl_fruit, "candied ");
            nmcpy(pl_fruit + 8, buf, PL_FSIZ - 8);
        }
        *altname = '\0';
        /* This flag indicates that a fruit has been made since the
         * last time the user set the fruit.  If it hasn't, we can
         * safely overwrite the current fruit, preventing the user from
         * setting many fruits in a row and overflowing.
         * Possible expansion: check for specific fruit IDs, not for
         * any fruit.
         */
        flags.made_fruit = FALSE;
        if (replace_fruit) {
            /* replace_fruit is already part of the fruit chain;
               update it in place rather than looking it up again */
            f = replace_fruit;
            copynchars(f->fname, pl_fruit, PL_FSIZ - 1);
            goto nonew;
        }
    }
    else
    {
        /* not user_supplied, so assumed to be from bones (or orc gang) */
        copynchars(altname, str, PL_FSIZ - 1);
        sanitize_name(altname);
        flags.made_fruit = TRUE; /* for safety.  Any fruit name added from a
                                  * bones level should exist anyway. */
    }
    f = fruit_from_name(*altname ? altname : str, FALSE, &highest_fruit_id);
    if (f)
        goto nonew;

    /* Maximum number of named fruits is 127, even if obj->special_quality can
       handle bigger values.  If adding another fruit would overflow,
       use a random fruit instead... we've got a lot to choose from.
       current_fruit remains as is. */
    if (highest_fruit_id >= 127)
        return rnd(127);

    f = newfruit();
    (void) memset((genericptr_t) f, 0, sizeof (struct fruit));
    copynchars(f->fname, *altname ? altname : str, PL_FSIZ - 1);
    f->fid = ++highest_fruit_id;
    /* we used to go out of our way to add it at the end of the list,
       but the order is arbitrary so use simpler insertion at start */
    f->nextf = ffruit;
    ffruit = f;
 nonew:
    if (user_specified)
        context.current_fruit = f->fid;
    return f->fid;
}

/*
 * This is a somewhat generic menu for taking a list of GnollHack style
 * class choices and presenting them via a description
 * rather than the traditional GnollHack characters.
 * (Benefits users whose first exposure to GnollHack is via tiles).
 *
 * prompt
 *           The title at the top of the menu.
 *
 * category: 0 = monster class
 *           1 = object  class
 *
 * way
 *           FALSE = PICK_ONE, TRUE = PICK_ANY
 *
 * class_list
 *           a null terminated string containing the list of choices.
 *
 * class_selection
 *           a null terminated string containing the selected characters.
 *
 * Returns number selected.
 */
int
choose_classes_menu(prompt, category, way, class_list, class_select)
const char *prompt;
int category;
boolean way;
char *class_list;
char *class_select;
{
    menu_item *pick_list = (menu_item *) 0;
    winid win;
    anything any;
    char buf[BUFSZ];
    int i, n;
    int ret;
    int next_accelerator, accelerator;

    if (class_list == (char *) 0 || class_select == (char *) 0)
        return 0;
    accelerator = 0;
    next_accelerator = 'a';
    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu(win);
    while (*class_list) {
        const char *text;
        boolean selected;

        text = (char *) 0;
        selected = FALSE;
        switch (category) {
        case 0:
            text = def_monsyms[def_char_to_monclass(*class_list)].explain;
            accelerator = *class_list;
            Sprintf(buf, "%s", text);
            break;
        case 1:
            text = def_oc_syms[def_char_to_objclass(*class_list)].explain;
            accelerator = next_accelerator;
            Sprintf(buf, "%c  %s", *class_list, text);
            break;
        default:
            impossible("choose_classes_menu: invalid category %d", category);
        }
        if (way && *class_select) { /* Selections there already */
            if (index(class_select, *class_list)) {
                selected = TRUE;
            }
        }
        any.a_int = *class_list;
        add_menu(win, NO_GLYPH, &any, accelerator, category ? *class_list : 0,
                 ATR_NONE, NO_COLOR, buf, selected);
        ++class_list;
        if (category > 0) {
            ++next_accelerator;
            if (next_accelerator == ('z' + 1))
                next_accelerator = 'A';
            if (next_accelerator == ('Z' + 1))
                break;
        }
    }
    if (category == 1 && next_accelerator <= 'z') {
        /* for objects, add "A - ' '  all classes", after a separator */
        any = zeroany;
        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "", MENU_UNSELECTED);
        any.a_int = (int) ' ';
        Sprintf(buf, "%c  %s", (char) any.a_int, "all classes of objects");
        /* we won't preselect this even if the incoming list is empty;
           having it selected means that it would have to be explicitly
           de-selected in order to select anything else */
        add_menu(win, NO_GLYPH, &any, 'A', 0, ATR_NONE, NO_COLOR, buf, MENU_UNSELECTED);
    }
    end_menu(win, prompt);
    n = select_menu(win, way ? PICK_ANY : PICK_ONE, &pick_list);
    destroy_nhwindow(win);
    if (n > 0) {
        if (category == 1) {
            /* for object classes, first check for 'all'; it means 'use
               a blank list' rather than 'collect every possible choice' */
            for (i = 0; i < n; ++i)
                if (pick_list[i].item.a_int == ' ') {
                    pick_list[0].item.a_int = ' ';
                    n = 1; /* return 1; also an implicit 'break;' */
                }
        }
        for (i = 0; i < n; ++i)
            *class_select++ = (char) pick_list[i].item.a_int;
        free((genericptr_t) pick_list);
        ret = n;
    } else if (n == -1) {
        class_select = eos(class_select);
        ret = -1;
    } else
        ret = 0;
    *class_select = '\0';
    return ret;
}

static const struct wc_Opt wc_options[] = {
    { "ascii_map", WC_ASCII_MAP },
    { "color", WC_COLOR },
    { "eight_bit_tty", WC_EIGHT_BIT_IN },
    { "hilite_pet", WC_HILITE_PET },
    { "popup_dialog", WC_POPUP_DIALOG },
    { "player_selection", WC_PLAYER_SELECTION },
    { "preload_tiles", WC_PRELOAD_TILES },
    { "tiled_map", WC_TILED_MAP },
    { "tile_file", WC_TILE_FILE },
    { "tile_width", WC_TILE_WIDTH },
    { "tile_height", WC_TILE_HEIGHT },
    { "use_inverse", WC_INVERSE },
    { "align_message", WC_ALIGN_MESSAGE },
    { "align_status", WC_ALIGN_STATUS },
    { "font_map", WC_FONT_MAP },
    { "font_menu", WC_FONT_MENU },
    { "font_message", WC_FONT_MESSAGE },
#if 0
    {"perm_invent", WC_PERM_INVENT},
#endif
    { "font_size_map", WC_FONTSIZ_MAP },
    { "font_size_menu", WC_FONTSIZ_MENU },
    { "font_size_message", WC_FONTSIZ_MESSAGE },
    { "font_size_status", WC_FONTSIZ_STATUS },
    { "font_size_text", WC_FONTSIZ_TEXT },
    { "font_status", WC_FONT_STATUS },
    { "font_text", WC_FONT_TEXT },
    { "map_mode", WC_MAP_MODE },
    { "scroll_amount", WC_SCROLL_AMOUNT },
    { "scroll_margin", WC_SCROLL_MARGIN },
    { "splash_screen", WC_SPLASH_SCREEN },
    { "vary_msgcount", WC_VARY_MSGCOUNT },
    { "windowcolors", WC_WINDOWCOLORS },
    { "mouse_support", WC_MOUSE_SUPPORT },
    { (char *) 0, 0L }
};
static const struct wc_Opt wc2_options[] = {
    { "fullscreen", WC2_FULLSCREEN },
    { "softkeyboard", WC2_SOFTKEYBOARD },
    { "wraptext", WC2_WRAPTEXT },
    { "use_darkgray", WC2_DARKGRAY },
    { "hitpointbar", WC2_HITPOINTBAR },
    { "hilite_status", WC2_HILITE_STATUS },
    /* name shown in 'O' menu is different */
    { "status hilite rules", WC2_HILITE_STATUS },
    /* statushilites doesn't have its own bit */
    { "statushilites", WC2_HILITE_STATUS },
    { "term_cols", WC2_TERM_SIZE },
    { "term_rows", WC2_TERM_SIZE },
    { "petattr", WC2_PETATTR },
    { "guicolor", WC2_GUICOLOR },
    { "statuslines", WC2_STATUSLINES },
    { "herewindow", WC2_HEREWINDOW },
    { "here_window_size", WC2_HEREWINDOW },
    { "windowborders", WC2_WINDOWBORDERS },
    { "autostatuslines", WC2_AUTOSTATUSLINES },
    { "preferred_screen_scale", WC2_PREFERRED_SCREEN_SCALE },
    { "sound_volume_ambient", WC2_VOLUME_CONTROLS },
    { "sound_volume_dialogue", WC2_VOLUME_CONTROLS },
    { "sound_volume_effects", WC2_VOLUME_CONTROLS },
    { "sound_volume_general", WC2_VOLUME_CONTROLS },
    { "sound_volume_music", WC2_VOLUME_CONTROLS },
    { "sound_volume_ui", WC2_VOLUME_CONTROLS },
    { "animation_interval", WC2_ANIMATIONS },
    { (char *) 0, 0L }
};

/*
 * If a port wants to change or ensure that the SET_IN_SYS,
 * SET_IN_FILE, DISP_IN_GAME, or SET_IN_GAME status of an option is
 * correct (for controlling its display in the option menu) call
 * set_option_mod_status()
 * with the appropriate second argument.
 */
void
set_option_mod_status(optnam, status)
const char *optnam;
int status;
{
    int k;

    if (SET__IS_VALUE_VALID(status)) {
        impossible("set_option_mod_status: status out of range %d.", status);
        return;
    }
    for (k = 0; boolopt[k].name; k++) {
        if (!strncmpi(boolopt[k].name, optnam, strlen(optnam))) {
            boolopt[k].optflags = status;
            return;
        }
    }
    for (k = 0; compopt[k].name; k++) {
        if (!strncmpi(compopt[k].name, optnam, strlen(optnam))) {
            compopt[k].optflags = status;
            return;
        }
    }
}

/*
 * You can set several wc_options in one call to
 * set_wc_option_mod_status() by setting
 * the appropriate bits for each option that you
 * are setting in the optmask argument
 * prior to calling.
 *    example: set_wc_option_mod_status(WC_COLOR|WC_SCROLL_MARGIN,
 * SET_IN_GAME);
 */
void
set_wc_option_mod_status(optmask, status)
uint64_t optmask;
int status;
{
    int k = 0;

    if (SET__IS_VALUE_VALID(status)) {
        impossible("set_wc_option_mod_status: status out of range %d.",
                   status);
        return;
    }
    while (wc_options[k].wc_name) {
        if (optmask & wc_options[k].wc_bit) {
            set_option_mod_status(wc_options[k].wc_name, status);
        }
        k++;
    }
}

STATIC_OVL boolean
is_wc_option(optnam)
const char *optnam;
{
    int k = 0;

    while (wc_options[k].wc_name) {
        if (strcmp(wc_options[k].wc_name, optnam) == 0)
            return TRUE;
        k++;
    }
    return FALSE;
}

STATIC_OVL boolean
wc_supported(optnam)
const char *optnam;
{
    int k;

    for (k = 0; wc_options[k].wc_name; ++k) {
        if (!strcmp(wc_options[k].wc_name, optnam))
            return (windowprocs.wincap & wc_options[k].wc_bit) ? TRUE : FALSE;
    }
    return FALSE;
}

/*
 * You can set several wc2_options in one call to
 * set_wc2_option_mod_status() by setting
 * the appropriate bits for each option that you
 * are setting in the optmask argument
 * prior to calling.
 *    example:
 * set_wc2_option_mod_status(WC2_FULLSCREEN|WC2_SOFTKEYBOARD|WC2_WRAPTEXT,
 * SET_IN_FILE);
 */

void
set_wc2_option_mod_status(optmask, status)
uint64_t optmask;
int status;
{
    int k = 0;

    if (SET__IS_VALUE_VALID(status)) {
        impossible("set_wc2_option_mod_status: status out of range %d.",
                   status);
        return;
    }
    while (wc2_options[k].wc_name) {
        if (optmask & wc2_options[k].wc_bit) {
            set_option_mod_status(wc2_options[k].wc_name, status);
        }
        k++;
    }
}

STATIC_OVL boolean
is_wc2_option(optnam)
const char *optnam;
{
    int k = 0;

    while (wc2_options[k].wc_name) {
        if (strcmp(wc2_options[k].wc_name, optnam) == 0)
            return TRUE;
        k++;
    }
    return FALSE;
}

STATIC_OVL boolean
wc2_supported(optnam)
const char *optnam;
{
    int k;

    for (k = 0; wc2_options[k].wc_name; ++k) {
        if (!strcmp(wc2_options[k].wc_name, optnam))
            return (windowprocs.wincap2 & wc2_options[k].wc_bit) ? TRUE
                                                                 : FALSE;
    }
    return FALSE;
}

STATIC_OVL void
wc_set_font_name(opttype, fontname)
int opttype;
char *fontname;
{
    char **fn = (char **) 0;

    if (!fontname)
        return;
    switch (opttype) {
    case MAP_OPTION:
        fn = &iflags.wc_font_map;
        break;
    case MESSAGE_OPTION:
        fn = &iflags.wc_font_message;
        break;
    case TEXT_OPTION:
        fn = &iflags.wc_font_text;
        break;
    case MENU_OPTION:
        fn = &iflags.wc_font_menu;
        break;
    case STATUS_OPTION:
        fn = &iflags.wc_font_status;
        break;
    default:
        return;
    }
    if (fn) {
        if (*fn)
            free((genericptr_t) *fn);
        *fn = dupstr(fontname);
    }
    return;
}

STATIC_OVL int
wc_set_window_colors(op)
char *op;
{
    /* syntax:
     *  menu white/black message green/yellow status white/blue text
     * white/black
     */
    int j;
    char buf[BUFSZ];
    char *wn, *tfg, *tbg, *newop;
    static const char *wnames[] = { "menu", "message", "status", "text" };
    static const char *shortnames[] = { "mnu", "msg", "sts", "txt" };
    static char **fgp[] = { &iflags.wc_foregrnd_menu,
                            &iflags.wc_foregrnd_message,
                            &iflags.wc_foregrnd_status,
                            &iflags.wc_foregrnd_text };
    static char **bgp[] = { &iflags.wc_backgrnd_menu,
                            &iflags.wc_backgrnd_message,
                            &iflags.wc_backgrnd_status,
                            &iflags.wc_backgrnd_text };

    Strcpy(buf, op);
    newop = mungspaces(buf);
    while (newop && *newop) {
        wn = tfg = tbg = (char *) 0;

        /* until first non-space in case there's leading spaces - before
         * colorname*/
        if (*newop == ' ')
            newop++;
        if (*newop)
            wn = newop;
        else
            return 0;

        /* until first space - colorname*/
        while (*newop && *newop != ' ')
            newop++;
        if (*newop)
            *newop = '\0';
        else
            return 0;
        newop++;

        /* until first non-space - before foreground*/
        if (*newop == ' ')
            newop++;
        if (*newop)
            tfg = newop;
        else
            return 0;

        /* until slash - foreground */
        while (*newop && *newop != '/')
            newop++;
        if (*newop)
            *newop = '\0';
        else
            return 0;
        newop++;

        /* until first non-space (in case there's leading space after slash) -
         * before background */
        if (*newop == ' ')
            newop++;
        if (*newop)
            tbg = newop;
        else
            return 0;

        /* until first space - background */
        while (*newop && *newop != ' ')
            newop++;
        if (*newop)
            *newop++ = '\0';

        for (j = 0; j < 4; ++j) {
            if (!strcmpi(wn, wnames[j]) || !strcmpi(wn, shortnames[j])) {
                if (tfg && !strstri(tfg, " ")) {
                    if (*fgp[j])
                        free((genericptr_t) *fgp[j]);
                    *fgp[j] = dupstr(tfg);
                }
                if (tbg && !strstri(tbg, " ")) {
                    if (*bgp[j])
                        free((genericptr_t) *bgp[j]);
                    *bgp[j] = dupstr(tbg);
                }
                break;
            }
        }
    }
    return 1;
}

/* set up for wizard mode if player or save file has requested to it;
   called from port-specific startup code to handle `GnollHack -D' or
   OPTIONS=playmode:debug, or from dorestore()'s restgamestate() if
   restoring a game which was saved in wizard mode */
void
set_playmode()
{
#ifndef ANDROID
    if (wizard) {
        if (authorize_wizard_mode())
            Strcpy(plname, "wizard");
        else
            wizard = FALSE; /* not allowed or not available */
        /* force explore mode if we didn't make it into wizard mode */
        discover = !wizard;
        iflags.deferred_X = FALSE;
    }
    /* don't need to do anything special for explore mode or normal play */
#endif

    /* Turn off sanity_check if not in wizard mode */
    if (!wizard)
        iflags.sanity_check = FALSE;
}

#endif /* OPTION_LISTS_ONLY */

/*options.c*/

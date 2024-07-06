/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    winprocs.h    $NHDT-Date: 1553204011 2019/03/21 21:33:31 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.47 $ */
/* Copyright (c) David Cohrs, 1992                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef WINPROCS_H
#define WINPROCS_H

#include "botl.h"
#include "general.h"
#include "layer.h"
#include "lev.h"
#include "obj.h"
#include "decl.h"

/* NB: this MUST match chain_procs below */
struct window_procs {
    const char *name;     /* Names should start with [a-z].  Names must
                           * not start with '-'.  Names starting with
                           * '+' are reserved for processors. */
    uint64_t wincap; /* window port capability options supported */
    uint64_t wincap2; /* additional window port capability options */
    void FDECL((*win_init_nhwindows), (int *, char **));
    void NDECL((*win_player_selection));
    void NDECL((*win_askname));
    void NDECL((*win_get_nh_event));
    void FDECL((*win_exit_nhwindows), (const char *));
    void FDECL((*win_suspend_nhwindows), (const char *));
    void NDECL((*win_resume_nhwindows));
    winid FDECL((*win_create_nhwindow_ex), (int, int, int, struct extended_create_window_info));
    void FDECL((*win_clear_nhwindow), (winid));
    void FDECL((*win_display_nhwindow), (winid, BOOLEAN_P));
    void FDECL((*win_destroy_nhwindow), (winid));
    void FDECL((*win_curs), (winid, int, int));
    void FDECL((*win_putstr_ex), (winid, const char *, int, int, int));
    void FDECL((*win_putstr_ex2), (winid, const char*, const char*, const char*, int, int, int));
    void FDECL((*win_putmixed_ex), (winid, const char *, int, int, int));
    void FDECL((*win_display_file), (const char *, BOOLEAN_P));
    void FDECL((*win_start_menu_ex), (winid, int));
    void FDECL((*win_add_menu), (winid, int, const ANY_P *, CHAR_P, CHAR_P,
                                 int, int, const char *, BOOLEAN_P));
    void FDECL((*win_add_extended_menu), (winid, int, const ANY_P*, CHAR_P, CHAR_P,
        int, int, const char*, BOOLEAN_P, struct extended_menu_info));
    void FDECL((*win_end_menu_ex), (winid, const char *, const char*));
    int FDECL((*win_select_menu), (winid, int, MENU_ITEM_P **));
    char FDECL((*win_message_menu), (CHAR_P, int, const char *));
    void NDECL((*win_update_inventory));
    void NDECL((*win_mark_synch));
    void NDECL((*win_wait_synch));
#ifdef CLIPPING
    void FDECL((*win_cliparound), (int, int, BOOLEAN_P));
#endif
#ifdef POSITIONBAR
    void FDECL((*win_update_positionbar), (char *));
#endif
    void FDECL((*win_print_glyph), (winid, XCHAR_P, XCHAR_P, struct layer_info));
    void FDECL((*win_issue_gui_command), (int, int, int, const char*));
    void FDECL((*win_raw_print), (const char *));
    void FDECL((*win_raw_print_bold), (const char *));
    int NDECL((*win_nhgetch));
    int FDECL((*win_nh_poskey), (int *, int *, int *));
    void NDECL((*win_nhbell));
    int NDECL((*win_doprev_message));
    char FDECL((*win_yn_function_ex), (int, int, int, int, const char*, const char *, const char *, CHAR_P, const char*, const char*, uint64_t));
    void FDECL((*win_getlin_ex), (int, int, int, const char *, char *, const char*, const char*, const char*));
    int NDECL((*win_get_ext_cmd));
    void FDECL((*win_number_pad), (int));
    void NDECL((*win_delay_output));
    void FDECL((*win_delay_output_milliseconds), (int));
    void FDECL((*win_delay_output_intervals), (int));
#ifdef CHANGE_COLOR
    void FDECL((*win_change_color), (int, int64_t, int));
#ifdef MAC
    void FDECL((*win_change_background), (int));
    short FDECL((*win_set_font_name), (winid, char *));
#endif
    char *NDECL((*win_get_color_string));
#endif

    /* other defs that really should go away (they're tty specific) */
    void NDECL((*win_start_screen));
    void NDECL((*win_end_screen));

    void FDECL((*win_outrip), (winid, int, time_t));
    void FDECL((*win_preference_update), (const char *));
    char *FDECL((*win_getmsghistory_ex), (char**, char**, BOOLEAN_P));
    void FDECL((*win_putmsghistory_ex), (const char *, const char *, const char *, BOOLEAN_P));
    void FDECL((*win_status_init), (int));
    void NDECL((*win_status_finish));
    void FDECL((*win_status_enablefield),
               (int, const char *, const char *, BOOLEAN_P));
    void FDECL((*win_status_update), (int, genericptr_t, int, int, int, uint64_t *));
    boolean NDECL((*win_can_suspend));
    void NDECL((*win_stretch_window));
    void FDECL((*win_set_animation_timer_interval), (unsigned int));
    int FDECL((*win_open_special_view), (struct special_view_info));
    void FDECL((*win_stop_all_sounds), (struct stop_all_info));
    void FDECL((*win_play_immediate_ghsound), (struct ghsound_immediate_info));
    void FDECL((*win_play_ghsound_occupation_ambient), (struct ghsound_occupation_ambient_info));
    void FDECL((*win_play_ghsound_effect_ambient), (struct ghsound_effect_ambient_info));
    void FDECL((*win_set_effect_ambient_volume), (struct effect_ambient_volume_info));
    void FDECL((*win_play_ghsound_music), (struct ghsound_music_info));
    void FDECL((*win_play_ghsound_level_ambient), (struct ghsound_level_ambient_info));
    void FDECL((*win_play_ghsound_environment_ambient), (struct ghsound_environment_ambient_info));
    void NDECL((*win_adjust_ghsound_general_volumes));
    void FDECL((*win_add_ambient_ghsound), (sound_source*));
    void FDECL((*win_delete_ambient_ghsound), (sound_source*));
    void FDECL((*win_set_ambient_ghsound_volume), (sound_source*));
    void NDECL((*win_clear_context_menu));
    void FDECL((*win_add_context_menu), (int, int, int, int, const char*, const char*, int, int));
    void FDECL((*win_update_status_button), (int, int, int, uint64_t));
    void FDECL((*win_toggle_animation_timer), (int, int, int, int, int, int, uint64_t)); /* timettype, id, state = on/off, x, y, flags */
    void FDECL((*win_display_floating_text), (int, int, const char*, int, int, int, uint64_t));
    void FDECL((*win_display_screen_text), (const char*, const char*, const char*, int, int, int, uint64_t));
    void FDECL((*win_display_popup_text), (const char*, const char*, int, int, int, int, uint64_t));
    void FDECL((*win_display_gui_effect), (int, int, int, int, int, int, uint64_t));
    void FDECL((*win_update_cursor), (int, int, int));
    int NDECL((*win_ui_has_input));
    void FDECL((*win_exit_hack), (int));
};

extern
#ifdef HANGUPHANDLING
    volatile
#endif
    NEARDATA struct window_procs windowprocs;

/*
 * If you wish to only support one window system and not use procedure
 * pointers, add the appropriate #ifdef below.
 */

#define init_nhwindows (*windowprocs.win_init_nhwindows)
#define player_selection (*windowprocs.win_player_selection)
#define askname (*windowprocs.win_askname)
#define get_nh_event (*windowprocs.win_get_nh_event)
#define exit_nhwindows (*windowprocs.win_exit_nhwindows)
#define suspend_nhwindows (*windowprocs.win_suspend_nhwindows)
#define resume_nhwindows (*windowprocs.win_resume_nhwindows)
#define create_nhwindow_ex (*windowprocs.win_create_nhwindow_ex)
#define create_nhwindow(x) create_nhwindow_ex(x, 0, NO_GLYPH, zerocreatewindowinfo)
#define clear_nhwindow (*windowprocs.win_clear_nhwindow)
#define display_nhwindow (*windowprocs.win_display_nhwindow)
#define destroy_nhwindow (*windowprocs.win_destroy_nhwindow)
#define curs (*windowprocs.win_curs)
#define putstr_ex (*windowprocs.win_putstr_ex)
#define putstr_ex2 (*windowprocs.win_putstr_ex2)
#define putstr(x, y, z) putstr_ex(x, z, y, NO_COLOR, 0)
#define putmixed_ex (*windowprocs.win_putmixed_ex)
#define putmixed(x, y, z) putmixed_ex(x, z, y, NO_COLOR, 0)
#define display_file (*windowprocs.win_display_file)
#define start_menu_ex (*windowprocs.win_start_menu_ex)
#define start_menu(x) start_menu_ex(x, 0)
#define add_menu (*windowprocs.win_add_menu)
#define add_active_menu(a, b, c, d, e, f, g, h, i) add_extended_menu(a, b, c, d, e, f, g, h, i, active_menu_info())
#define add_extended_menu (*windowprocs.win_add_extended_menu)
#define end_menu_ex (*windowprocs.win_end_menu_ex)
#define end_menu(a, b) end_menu_ex(a, b, (const char*)0)
#define select_menu (*windowprocs.win_select_menu)
#define message_menu (*windowprocs.win_message_menu)
#define update_inventory (*windowprocs.win_update_inventory)
#define mark_synch (*windowprocs.win_mark_synch)
#define wait_synch (*windowprocs.win_wait_synch)
#ifdef CLIPPING
#define cliparound (*windowprocs.win_cliparound)
#endif
#ifdef POSITIONBAR
#define update_positionbar (*windowprocs.win_update_positionbar)
#endif
#define print_glyph (*windowprocs.win_print_glyph)
#define issue_gui_command (*windowprocs.win_issue_gui_command)
#define issue_simple_gui_command(x) (*windowprocs.win_issue_gui_command)(x, 0, 0, (char*)0)
#define issue_parametered_gui_command(x, y) (*windowprocs.win_issue_gui_command)(x, y, 0, (char*)0)
#define issue_boolean_gui_command(x, y) (*windowprocs.win_issue_gui_command)(x, (int)(y), 0, (char*)0)
#define issue_debuglog(i, s) if(windowprocs.win_issue_gui_command) (*windowprocs.win_issue_gui_command)(GUI_CMD_DEBUGLOG, DEBUGLOG_GENERAL, i, s)
#define issue_debuglog_fd(f, s) if(windowprocs.win_issue_gui_command) (*windowprocs.win_issue_gui_command)(GUI_CMD_DEBUGLOG, DEBUGLOG_FILE_DESCRIPTOR, f, s)
#define raw_print (*windowprocs.win_raw_print)
#define raw_print_bold (*windowprocs.win_raw_print_bold)
#define nhgetch (*windowprocs.win_nhgetch)
#define nh_poskey (*windowprocs.win_nh_poskey)
#define nhbell (*windowprocs.win_nhbell)
#define nh_doprev_message (*windowprocs.win_doprev_message)
#define getlin_ex (*windowprocs.win_getlin_ex)
#define getlin(x, y) (*windowprocs.win_getlin_ex)(GETLINE_GENERAL, ATR_NONE, NO_COLOR, x, y, (char*)0, (char*)0, (char*)0)
#define get_ext_cmd (*windowprocs.win_get_ext_cmd)
#define number_pad (*windowprocs.win_number_pad)
#define delay_output (*windowprocs.win_delay_output)
#define delay_output_milliseconds (*windowprocs.win_delay_output_milliseconds)
#define delay_output_intervals (*windowprocs.win_delay_output_intervals)
#ifdef CHANGE_COLOR
#define change_color (*windowprocs.win_change_color)
#ifdef MAC
#define change_background (*windowprocs.win_change_background)
#define set_font_name (*windowprocs.win_set_font_name)
#endif
#define get_color_string (*windowprocs.win_get_color_string)
#endif

/* 3.4.2: There is a real yn_function() in the core now, which does
 *        some buffer length validation on the parameters prior to
 *        invoking the window port routine. yn_function() is in cmd.c
 */
/* #define yn_function (*windowprocs.win_yn_function_ex) */

/* other defs that really should go away (they're tty specific) */
#define start_screen (*windowprocs.win_start_screen)
#define end_screen (*windowprocs.win_end_screen)

#define outrip (*windowprocs.win_outrip)
#define preference_update (*windowprocs.win_preference_update)
#define getmsghistory_ex (*windowprocs.win_getmsghistory_ex)
#define putmsghistory_ex (*windowprocs.win_putmsghistory_ex)
#define getmsghistory(x) getmsghistory_ex((char**)0, (char**)0, x)
#define putmsghistory(x, y) putmsghistory_ex(x, (char*)0, (char*)0, y)
/* there is a status_initialize() in botl.c,
 * which calls win_status_init() directly; same with status_finish.
 */
#define status_enablefield (*windowprocs.win_status_enablefield)
#define status_update (*windowprocs.win_status_update)
#define stretch_window (*windowprocs.win_stretch_window)
#define set_animation_timer_interval (*windowprocs.win_set_animation_timer_interval)

#define open_special_view (*windowprocs.win_open_special_view)
#define stop_all_sounds (*windowprocs.win_stop_all_sounds)
#define play_immediate_ghsound (*windowprocs.win_play_immediate_ghsound)
#define play_ghsound_occupation_ambient (*windowprocs.win_play_ghsound_occupation_ambient)
#define play_ghsound_effect_ambient (*windowprocs.win_play_ghsound_effect_ambient)
#define set_effect_ambient_volume (*windowprocs.win_set_effect_ambient_volume)
#define play_ghsound_music (*windowprocs.win_play_ghsound_music)
#define play_ghsound_level_ambient (*windowprocs.win_play_ghsound_level_ambient)
#define play_ghsound_environment_ambient (*windowprocs.win_play_ghsound_environment_ambient)
#define adjust_ghsound_general_volumes (*windowprocs.win_adjust_ghsound_general_volumes)
#define add_ambient_ghsound (*windowprocs.win_add_ambient_ghsound)
#define delete_ambient_ghsound (*windowprocs.win_delete_ambient_ghsound)
#define set_ambient_ghsound_volume (*windowprocs.win_set_ambient_ghsound_volume)
#define clear_context_menu (*windowprocs.win_clear_context_menu)
#define add_context_menu (*windowprocs.win_add_context_menu)
#define update_status_button (*windowprocs.win_update_status_button)
#define toggle_animation_timer (*windowprocs.win_toggle_animation_timer)
#define display_floating_text (*windowprocs.win_display_floating_text)
#define display_screen_text (*windowprocs.win_display_screen_text)
#define display_popup_text (*windowprocs.win_display_popup_text)
#define display_gui_effect (*windowprocs.win_display_gui_effect)
#define update_cursor (*windowprocs.win_update_cursor)
#define ui_has_input (*windowprocs.win_ui_has_input)
#define exit_hack (*windowprocs.win_exit_hack)

/*
 * WINCAP
 * Window port preference capability bits.
 * Some day this might be better in its own wincap.h file.
 */
/* clang-format off */
#define WC_COLOR         0x00000001UL /* 01 Port can display things in color  */
#define WC_HILITE_PET    0x00000002UL /* 02 supports hilite pet               */
#define WC_ASCII_MAP     0x00000004UL /* 03 supports an ascii map             */
#define WC_TILED_MAP     0x00000008UL /* 04 supports a tiled map              */
#define WC_PRELOAD_TILES 0x00000010UL /* 05 supports pre-loading tiles        */
#define WC_TILE_WIDTH    0x00000020UL /* 06 prefer this width of tile         */
#define WC_TILE_HEIGHT   0x00000040UL /* 07 prefer this height of tile        */
#define WC_TILE_FILE     0x00000080UL /* 08 alternative tile file name        */
#define WC_INVERSE       0x00000100UL /* 09 Port supports inverse video       */
#define WC_ALIGN_MESSAGE 0x00000200UL /* 10 supports mesg alignment top|b|l|r */
#define WC_ALIGN_STATUS  0x00000400UL /* 11 supports status alignmt top|b|l|r */
#define WC_VARY_MSGCOUNT 0x00000800UL /* 12 supports varying message window   */
#define WC_FONT_MAP      0x00001000UL /* 13 supports spec of map window font  */
#define WC_FONT_MESSAGE  0x00002000UL /* 14 supports spec of message font     */
#define WC_FONT_STATUS   0x00004000UL /* 15 supports spec of status font      */
#define WC_FONT_MENU     0x00008000UL /* 16 supports spec of menu font        */
#define WC_FONT_TEXT     0x00010000UL /* 17 supports spec of text window font */
#define WC_FONTSIZ_MAP   0x00020000UL /* 18 supports spec of map font size    */
#define WC_FONTSIZ_MESSAGE 0x040000UL /* 19 supports spec of mesg font size   */
#define WC_FONTSIZ_STATUS 0x0080000UL /* 20 supports spec of status font size */
#define WC_FONTSIZ_MENU  0x00100000UL /* 21 supports spec of menu font size   */
#define WC_FONTSIZ_TEXT  0x00200000UL /* 22 supports spec of text font size   */
#define WC_SCROLL_MARGIN 0x00400000UL /* 23 supports setting map scroll marg  */
#define WC_SPLASH_SCREEN 0x00800000UL /* 24 supports display of splash screen */
#define WC_POPUP_DIALOG  0x01000000UL /* 25 supports queries in popup dialogs */
#define WC_SCROLL_AMOUNT 0x02000000UL /* 26 scroll this amount at scroll marg */
#define WC_EIGHT_BIT_IN  0x04000000UL /* 27 8-bit character input             */
#define WC_PERM_INVENT   0x08000000UL /* 28 supports persistent inventory win */
#define WC_MAP_MODE      0x10000000UL /* 29 map_mode option                   */
#define WC_WINDOWCOLORS  0x20000000UL /* 30 background color for mesg window  */
#define WC_PLAYER_SELECTION 0x40000000UL /* 31 supports player selection      */
#ifdef NHSTDC
#define WC_MOUSE_SUPPORT 0x80000000UL /* 32 mouse support                    */
#else
#define WC_MOUSE_SUPPORT 0x80000000UL /* 32 mouse support                     */
#endif
                                     /* no free bits */

#define WC2_FULLSCREEN              0x00000001UL /* 01 display full screen             */
#define WC2_SOFTKEYBOARD            0x00000002UL /* 02 software keyboard               */
#define WC2_WRAPTEXT                0x00000004UL /* 03 wrap long lines of text         */
#define WC2_HILITE_STATUS           0x00000008UL /* 04 hilite fields in status         */
#define WC2_SELECTSAVED             0x00000010UL /* 05 saved game selection menu       */
#define WC2_DARKGRAY                0x00000020UL /* 06 use bold black for black glyphs */
#define WC2_HITPOINTBAR             0x00000040UL /* 07 show bar representing hit points */
#define WC2_FLUSH_STATUS            0x00000080UL /* 08 call status_update(BL_FLUSH)
                                                  *    after updating status window fields */
#define WC2_RESET_STATUS            0x00000100UL /* 09 call status_update(BL_RESET) to
                                   *    indicate 'draw everything'      */
#define WC2_TERM_SIZE               0x00000200UL /* 10 support setting terminal size   */
#define WC2_STATUSLINES             0x00000400UL /* 11 switch between 2 and 8 lines of status */
#define WC2_WINDOWBORDERS           0x00000800UL /* 12 display borders on nh windows   */
#define WC2_PETATTR                 0x00001000UL /* 13 attributes for hilite_pet       */
#define WC2_GUICOLOR                0x00002000UL /* 14 display colours outside map win */
/* pline() can overload the display attributes argument passed to putstr()
   with one or more flags and at most one of bold/blink/inverse/&c */
#define WC2_URGENT_MESG             0x00004000UL /* 15 putstr(WIN_MESSAGE) supports urgency
                                                  *    via non-display attribute flag  */
#define WC2_SUPPRESS_HIST           0x00008000UL /* 16 putstr(WIN_MESSAGE) supports history
                                                  *    suppression via non-disp attr   */
#define WC2_AUTOSTATUSLINES         0x00010000UL /* 17 windowing system can automatically adjust number of status lines */
#define WC2_PREFERRED_SCREEN_SCALE  0x00020000UL /* 18 preferred screen scale is shown in options */
#define WC2_HEREWINDOW              0x00040000UL /* 19 supports here window */
#define WC2_SCREEN_TEXT             0x00080000UL /* 20 windowing system can show screen texts */
#define WC2_ANIMATIONS              0x00100000UL /* 21 supports displaying some sort of animations with tiles; animation options are shown in options */
#define WC2_VOLUME_CONTROLS         0x00200000UL /* 22 volume controls are shown in options */
/* free bit */
#define WC2_SPECIAL_SYMBOLS         0x00800000UL /* 24 handles special symbols in text of the type &symbol_name; */
#define WC2_MENU_SUFFIXES           0x01000000UL /* 25 supports placing text in parentheses on a different line in menus */
#define WC2_FADING_ANIMATIONS       0x02000000UL /* 26 supports fading animations */
#define WC2_MENU_SHOWS_OK_CANCEL    0x04000000UL /* 27 menus always show OK and Cancel buttons (so you can then potentially omit e.g. a quit choice in the menu) */

/* 5 free bits */

#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2
#define ALIGN_TOP    3
#define ALIGN_BOTTOM 4

/* player_selection */
#define VIA_DIALOG  0
#define VIA_PROMPTS 1

/* map_mode settings - deprecated */
#define MAP_MODE_TILES      0
#define MAP_MODE_ASCII4x6   1
#define MAP_MODE_ASCII6x8   2
#define MAP_MODE_ASCII8x8   3
#define MAP_MODE_ASCII16x8  4
#define MAP_MODE_ASCII7x12  5
#define MAP_MODE_ASCII8x12  6
#define MAP_MODE_ASCII16x12 7
#define MAP_MODE_ASCII12x16 8
#define MAP_MODE_ASCII10x18 9
#define MAP_MODE_ASCII_FIT_TO_SCREEN 10
#define MAP_MODE_TILES_FIT_TO_SCREEN 11

#if 0
#define WC_SND_SOUND   0x0001L /* 01 Port has some sound capabilities     */
#define WC_SND_SPEAKER 0x0002L /* 02 Sound supported via built-in speaker */
#define WC_SND_STEREO  0x0004L /* 03 Stereo sound supported               */
#define WC_SND_RAW     0x0008L /* 04 Raw sound supported                  */
#define WC_SND_WAVE    0x0010L /* 05 Wave support                         */
#define WC_SND_MIDI    0x0020L /* 06 Midi support                         */
                               /* 26 free bits */
#endif

struct wc_Opt {
    const char *wc_name;
    uint64_t wc_bit;
};

/* Macro for the currently active Window Port whose function
   pointers have been loaded */
#define WINDOWPORT(wn) \
    (windowprocs.name && !strncmpi((wn), windowprocs.name, strlen((wn))))

/* role selection by player_selection(); this ought to be in the core... */
#define RS_NAME    0
#define RS_ROLE    1
#define RS_RACE    2
#define RS_GENDER  3
#define RS_ALGNMNT 4
#define RS_filter  5
#define RS_menu_arg(x) (ROLE_RANDOM - ((x) + 1)) /* 0..5 -> -3..-8 */

/* Choose_windows() may be called multiple times; these constants tell the
 * init function whether the window system is coming or going. */
#define WININIT      0
#define WININIT_UNDO 1

#ifdef WINCHAIN
/* Setup phases for window chain elements.
        void *  rv = X_procs_chain(int,    int, void *, void *, void *);
                Xprivate*       ALLOC n    0    0    0
                -           INIT     n   self    next    nextdata
   where:
        Xprivate* is anything window chain entry type X wants back
        n is the link count (starting with 1)
        self is the Xprivate* returned earlier
        next is struct winprocs * or struct chainprocs * for the next link
        nextdata is the Xprivate* for the next link in the chain
*/
#define WINCHAIN_ALLOC 0
#define WINCHAIN_INIT  1

/* clang-format on */

#define CARGS void *

extern FILE *wc_tracelogf; /* Expose log file for additional debugging. */

struct chain_procs {
    const char *name;     /* Names should start with [a-z].  Names must
                           * not start with '-'.  Names starting with
                           * '+' are reserved for processors. */
    uint64_t wincap; /* window port capability options supported */
    uint64_t wincap2; /* additional window port capability options */
    void FDECL((*win_init_nhwindows), (CARGS, int *, char **));
    void FDECL((*win_player_selection), (CARGS));
    void FDECL((*win_askname), (CARGS));
    void FDECL((*win_get_nh_event), (CARGS));
    void FDECL((*win_exit_nhwindows), (CARGS, const char *));
    void FDECL((*win_suspend_nhwindows), (CARGS, const char *));
    void FDECL((*win_resume_nhwindows), (CARGS));
    winid FDECL((*win_create_nhwindow_ex), (CARGS, int, int, int, struct extended_create_window_info));
    void FDECL((*win_clear_nhwindow), (CARGS, winid));
    void FDECL((*win_display_nhwindow), (CARGS, winid, BOOLEAN_P));
    void FDECL((*win_destroy_nhwindow), (CARGS, winid));
    void FDECL((*win_curs), (CARGS, winid, int, int));
    void FDECL((*win_putstr_ex), (CARGS, winid, const char *, int, int, int));
    void FDECL((*win_putstr_ex2), (CARGS, winid, const char*, const char*, const char*, int, int, int));
    void FDECL((*win_putmixed_ex), (CARGS, winid, const char *, int, int, int));
    void FDECL((*win_display_file), (CARGS, const char *, BOOLEAN_P));
    void FDECL((*win_start_menu_ex), (CARGS, winid, int));
    void FDECL((*win_add_menu), (CARGS, winid, int, const ANY_P *, CHAR_P,
                                 CHAR_P, int, int, const char *, BOOLEAN_P));
    void FDECL((*win_add_extended_menu), (CARGS, winid, int, const ANY_P*, CHAR_P,
        CHAR_P, int, int, const char*, BOOLEAN_P, struct extended_menu_info));
    void FDECL((*win_end_menu_ex), (CARGS, winid, const char *, const char*));
    int FDECL((*win_select_menu), (CARGS, winid, int, MENU_ITEM_P **));
    char FDECL((*win_message_menu), (CARGS, CHAR_P, int, const char *));
    void FDECL((*win_update_inventory), (CARGS));
    void FDECL((*win_mark_synch), (CARGS));
    void FDECL((*win_wait_synch), (CARGS));
#ifdef CLIPPING
    void FDECL((*win_cliparound), (CARGS, int, int, BOOLEAN_P));
#endif
#ifdef POSITIONBAR
    void FDECL((*win_update_positionbar), (CARGS, char *));
#endif
    void FDECL((*win_print_glyph), (CARGS, winid, XCHAR_P, XCHAR_P, struct layer_info));
    void FDECL((*win_issue_gui_command), (int, int, int, const char*));
    void FDECL((*win_raw_print), (CARGS, const char *));
    void FDECL((*win_raw_print_bold), (CARGS, const char *));
    int FDECL((*win_nhgetch), (CARGS));
    int FDECL((*win_nh_poskey), (CARGS, int *, int *, int *));
    void FDECL((*win_nhbell), (CARGS));
    int FDECL((*win_doprev_message), (CARGS));
    char FDECL((*win_yn_function_ex),
               (CARGS, int, int, int, int, const char *, const char *, CHAR_P, const char*, const char*, uint64_t));
    void FDECL((*win_getlin_ex), (CARGS, int, int, int, const char *, char *, const char*, const char*, const char*));
    int FDECL((*win_get_ext_cmd), (CARGS));
    void FDECL((*win_number_pad), (CARGS, int));
    void FDECL((*win_delay_output), (CARGS));
    void FDECL((*win_delay_output_milliseconds), (CARGS, int));
    void FDECL((*win_delay_output_intervals), (CARGS, int));
#ifdef CHANGE_COLOR
    void FDECL((*win_change_color), (CARGS, int, int64_t, int));
#ifdef MAC
    void FDECL((*win_change_background), (CARGS, int));
    short FDECL((*win_set_font_name), (CARGS, winid, char *));
#endif
    char *FDECL((*win_get_color_string), (CARGS));
#endif

    /* other defs that really should go away (they're tty specific) */
    void FDECL((*win_start_screen), (CARGS));
    void FDECL((*win_end_screen), (CARGS));

    void FDECL((*win_outrip), (CARGS, winid, int, time_t));
    void FDECL((*win_preference_update), (CARGS, const char *));
    char *FDECL((*win_getmsghistory_ex), (CARGS, char**, char**, BOOLEAN_P));
    void FDECL((*win_putmsghistory_ex), (CARGS, const char *, const char*, const char*, BOOLEAN_P));
    void FDECL((*win_status_init), (CARGS, int));
    void FDECL((*win_status_finish), (CARGS));
    void FDECL((*win_status_enablefield),
               (CARGS, int, const char *, const char *, BOOLEAN_P));
    void FDECL((*win_status_update), (CARGS, int, genericptr_t, int, int, int, uint64_t *));
    boolean FDECL((*win_can_suspend), (CARGS));
    void FDECL((*win_stretch_window, (CARGS)));
    void FDECL((*win_set_animation_timer_interval), (CARGS, unsigned int));
    int FDECL((*win_open_special_view), (CARGS, struct special_view_info));
    void FDECL((*win_stop_all_sounds), (CARGS, struct stop_all_info));
    void FDECL((*win_play_immediate_ghsound), (CARGS, struct ghsound_immediate_info));
    void FDECL((*win_play_ghsound_occupation_ambient), (CARGS, struct ghsound_occupation_ambient_info));
    void FDECL((*win_play_ghsound_effect_ambient), (CARGS, struct ghsound_effect_ambient_info));
    void FDECL((*win_set_effect_ambient_volume), (CARGS, struct effect_ambient_volume_info));
    void FDECL((*win_play_ghsound_music), (CARGS, struct ghsound_music_info));
    void FDECL((*win_play_ghsound_level_ambient), (CARGS, struct ghsound_level_ambient_info));
    void FDECL((*win_play_ghsound_environment_ambient), (CARGS, struct ghsound_environment_ambient_info));
    void FDECL((*win_adjust_ghsound_general_volumes, (CARGS))));
    void FDECL((*win_add_ambient_ghsound), (CARGS, sound_source*));
    void FDECL((*win_delete_ambient_ghsound), (CARGS, sound_source*));
    void FDECL((*win_set_ambient_ghsound_volume), (CARGS, sound_source*));
    void FDECL((*win_clear_context_menu), (CARGS));
    void FDECL((*win_add_context_menu), (CARGS, int, int, int, int, const char*, const char*, int, int));
    void FDECL((*win_update_status_button), (CARGS, int, int, int, uint64_t));
    void FDECL((*win_toggle_animation_timer), (CARGS, int, int, int, int, int, int, uint64_t));
    void FDECL((*win_display_floating_text), (CARGS, int, int, const char*, int, int, int, uint64_t));
    void FDECL((*win_display_screen_text), (CARGS, const char*, const char*, const char*, int, int, int, uint64_t));
    void FDECL((*win_display_popup_text), (CARGS, const char*, const char*, int, int, int, int, uint64_t));
    void FDECL((*win_display_gui_effect), (CARGS, int, int, int, int, int, int, uint64_t));
    void FDECL((*win_update_cursor), (CARGS, int, int, int));
    int FDECL((*win_ui_has_input), (CARGS));
    void FDECL((*win_exit_hack), (CARGS, int));
};
#endif /* WINCHAIN */

#ifdef SAFEPROCS
/*
 * window port routines available in sys/share/safeproc.c
 */
extern struct window_procs *FDECL(get_safe_procs, (int));
extern void FDECL(safe_init_nhwindows, (int *, char **));
extern void NDECL(safe_player_selection);
extern void NDECL(safe_askname);
extern void NDECL(safe_get_nh_event);
extern void FDECL(safe_exit_nhwindows, (const char *));
extern void FDECL(safe_suspend_nhwindows, (const char *));
extern void NDECL(safe_resume_nhwindows);
extern winid FDECL(safe_create_nhwindow_ex, (int, int, int, struct extended_create_window_info));
extern void FDECL(safe_clear_nhwindow, (winid));
extern void FDECL(safe_display_nhwindow, (winid, BOOLEAN_P));
extern void FDECL(safe_destroy_nhwindow, (winid));
extern void FDECL(safe_curs, (winid, int, int));
extern void FDECL(safe_putstr_ex, (winid, const char *, int, int, int));
extern void FDECL(safe_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));
extern void FDECL(safe_putmixed_ex, (winid, const char *, int, int, int));
extern void FDECL(safe_display_file, (const char *, BOOLEAN_P));
extern void FDECL(safe_start_menu_ex, (winid, int));
extern void FDECL(safe_add_menu, (winid, int, const ANY_P *, CHAR_P, CHAR_P,
                                  int, int, const char *, BOOLEAN_P));
extern void FDECL(safe_add_extended_menu, (winid, int, const ANY_P*, CHAR_P, CHAR_P,
    int, int, const char*, BOOLEAN_P, struct extended_menu_info));
extern void FDECL(safe_end_menu_ex, (winid, const char *, const char*));
extern int FDECL(safe_select_menu, (winid, int, MENU_ITEM_P **));
extern char FDECL(safe_message_menu, (CHAR_P, int, const char *));
extern void NDECL(safe_update_inventory);
extern void NDECL(safe_mark_synch);
extern void NDECL(safe_wait_synch);
#ifdef CLIPPING
extern void FDECL(safe_cliparound, (int, int, BOOLEAN_P));
#endif
#ifdef POSITIONBAR
extern void FDECL(safe_update_positionbar, (char *));
#endif
extern void FDECL(safe_print_glyph, (winid, XCHAR_P, XCHAR_P, struct layer_info));
extern void FDECL(safe_issue_gui_command, (int, int, int, const char*));
extern void FDECL(safe_raw_print, (const char *));
extern void FDECL(safe_raw_print_bold, (const char *));
extern int NDECL(safe_nhgetch);
extern int FDECL(safe_nh_poskey, (int *, int *, int *));
extern void NDECL(safe_nhbell);
extern int NDECL(safe_doprev_message);
extern char FDECL(safe_yn_function_ex, (int, int, int, int, const char *, const char *, const char *, CHAR_P, const char*, const char*, uint64_t));
extern void FDECL(safe_getlin_ex, (int, int, int, const char *, char *, const char*, const char*, const char*));
extern int NDECL(safe_get_ext_cmd);
extern void FDECL(safe_number_pad, (int));
extern void NDECL(safe_delay_output);
extern void FDECL(safe_delay_output_milliseconds, (int));
extern void FDECL(safe_delay_output_intervals, (int));
#ifdef CHANGE_COLOR
extern void FDECL(safe_change_color, (int, int64_t, int));
#ifdef MAC
extern void FDECL(safe_change_background, (int));
extern short FDECL(safe_set_font_name, (winid, char *));
#endif
extern char *NDECL(safe_get_color_string);
#endif
extern void NDECL(safe_start_screen);
extern void NDECL(safe_end_screen);
extern void FDECL(safe_outrip, (winid, int, time_t));
extern void FDECL(safe_preference_update, (const char *));
extern char *FDECL(safe_getmsghistory_ex, (char**, char**, BOOLEAN_P));
extern void FDECL(safe_putmsghistory_ex, (const char *, const char *, const char *, BOOLEAN_P));
extern void FDECL(safe_status_init, (int));
extern void NDECL(safe_status_finish);
extern void FDECL(safe_status_enablefield,
                    (int, const char *, const char *, BOOLEAN_P));
extern void FDECL(safe_status_update, (int, genericptr_t, int, int, int, uint64_t *));
extern boolean NDECL(safe_can_suspend);
extern void NDECL(safe_stretch_window);
extern void FDECL(safe_set_animation_timer_interval, (unsigned int));
extern int FDECL(safe_open_special_view, (struct special_view_info));
extern void FDECL(safe_stop_all_sounds, (struct stop_all_info));
extern void FDECL(safe_play_ghsound_occupation_ambient, (struct ghsound_occupation_ambient_info));
extern void FDECL(safe_play_ghsound_effect_ambient, (struct ghsound_effect_ambient_info));
extern void FDECL(safe_set_effect_ambient_volume, (struct effect_ambient_volume_info));
extern void FDECL(safe_play_ghsound_level_ambient, (struct ghsound_level_ambient_info));
extern void FDECL(safe_play_ghsound_environment_ambient, (struct ghsound_environment_ambient_info));
extern void FDECL(safe_play_ghsound_music, (struct ghsound_music_info));
extern void FDECL(safe_play_immediate_ghsound, (struct ghsound_immediate_info));
extern void NDECL(safe_adjust_ghsound_general_volumes);
extern void FDECL(safe_add_ambient_ghsound, (struct soundsource_t*));
extern void FDECL(safe_delete_ambient_ghsound, (struct soundsource_t*));
extern void FDECL(safe_set_ambient_ghsound_volume, (struct soundsource_t*));
extern void NDECL(safe_clear_context_menu);
extern void FDECL(safe_add_context_menu, (int, int, int, int, const char*, const char*, int, int));
extern void FDECL(safe_update_status_button, (int, int, int, uint64_t));
extern void FDECL(safe_toggle_animation_timer, (int, int, int, int, int, int, uint64_t));
extern void FDECL(safe_display_floating_text, (int, int, const char*, int, int, int, uint64_t));
extern void FDECL(safe_display_screen_text, (const char*, const char*, const char*, int, int, int, uint64_t));
extern void FDECL(safe_display_popup_text, (const char*, const char*, int, int, int, int, uint64_t));
extern void FDECL(safe_display_gui_effect, (int, int, int, int, int, int, uint64_t));
extern void FDECL(safe_update_cursor, (int, int, int));
extern int NDECL(safe_ui_has_input);
extern void FDECL(safe_exit_hack, (int));

extern void FDECL(stdio_raw_print, (const char *));
extern void FDECL(stdio_raw_print_bold, (const char *));
extern void NDECL(stdio_wait_synch);
extern int NDECL(stdio_nhgetch);
#endif /* SAFEPROCS */
#endif /* WINPROCS_H */


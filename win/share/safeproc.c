/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    safeproc.c   */    
/* Copyright (c) Michael Allison, 2018                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <stdio.h>

/*
 * ***********************************************************
 * This is a complete WindowPort implementation that can be
 * assigned to the windowproc function pointers very early
 * in the startup initialization, perhaps immediately even.
 * It requires only the following call:
 *          windowprocs = *get_safe_procs(0);
 *
 * The game startup can trigger functions in other modules
 * that make assumptions on a WindowPort being available
 * and bad things can happen if any function pointers are
 * null at that time.
 *
 * Some ports prior to 3.6.2 made attempts to early init
 * various pieces of one of their WindowPorts, but that
 * caused conflicts if that particular WindowPort wasn't
 * the one that the user ended up selecting in their
 * config file later. The WindowPort interfaced was designed
 * to allow multiple WindowPorts to be linked into the same
 * game binary.
 *
 * The base functions established by a call to get_safe_procs()
 * accomplish the goal of preventing crashes, but not much
 * else.
 * 
 * There are also a few additional functions provided in here
 * that can be selected optionally to provide some startup
 * functionality for getting messages out to the user about
 * issues that are being experienced during startup in
 * general or during options parsing. The ones in here are
 * deliberately free from any platforms or OS specific code.
 * Please leave them using stdio C routines as much as
 * possible.  That isn't to say you can't do fancier functions
 * prior to initialization of the primary WindowPort, but you
 * can provide those platform-specific functions elsewhere,
 * and assign them the same way that these more generic versions
 * are assigned.
 *
 * The additional platform-independent, but more functional
 * routines provided in here should be assigned after the
 *    windowprocs = *get_safe_procs(n)
 * call.
 *
 *  Usage:
 *   
 *    windowprocs = *get_safe_procs(0);
 *   initializes a set of winprocs function pointers that ensure
 *   none of the function pointers are left null, but that's all
 *   it does.
 *   
 *    windowprocs = *get_safe_procs(1);
 *   initializes a set of winprocs functions pointers that ensure
 *   none of the function pointers are left null, but also
 *   provides some basic output and input functionality using
 *   nothing other than C stdio routines (no platform-specific
 *   or OS-specific code).
 *
 * ***********************************************************
 */

struct window_procs safe_procs = {
    "safe-startup", 0L, 0L,
    safe_init_nhwindows, safe_player_selection, safe_askname, safe_get_nh_event,
    safe_exit_nhwindows, safe_suspend_nhwindows, safe_resume_nhwindows,
    safe_create_nhwindow_ex, safe_clear_nhwindow, safe_display_nhwindow,
    safe_destroy_nhwindow, safe_curs, safe_putstr_ex, safe_putstr_ex2, genl_putmixed_ex,
    safe_display_file, safe_start_menu_ex, safe_add_menu, safe_add_extended_menu, safe_end_menu_ex,
    safe_select_menu, safe_message_menu, safe_update_inventory, safe_mark_synch,
    safe_wait_synch,
#ifdef CLIPPING
    safe_cliparound,
#endif
#ifdef POSITIONBAR
    safe_update_positionbar,
#endif
    safe_print_glyph, safe_issue_gui_command, safe_raw_print, safe_raw_print_bold, safe_nhgetch,
    safe_nh_poskey, safe_nhbell, safe_doprev_message, safe_yn_function_ex,
    safe_getlin_ex, safe_get_ext_cmd, safe_number_pad, safe_delay_output, safe_delay_output_milliseconds, safe_delay_output_intervals,
#ifdef CHANGE_COLOR /* the Mac uses a palette device */
    safe_change_color,
#ifdef MAC
    safe_change_background, set_safe_font_name,
#endif
    safe_get_color_string,
#endif
    safe_start_screen, safe_end_screen, genl_outrip,
    safe_preference_update,
    safe_getmsghistory_ex, safe_putmsghistory_ex,
    safe_status_init,
    safe_status_finish, safe_status_enablefield,
#ifdef STATUS_HILITES
    safe_status_update,
#else
    safe_status_update,
#endif
    safe_can_suspend,
    safe_stretch_window,
    safe_set_animation_timer_interval,
    safe_open_special_view,
    safe_stop_all_sounds,
    safe_play_immediate_ghsound,
    safe_play_ghsound_occupation_ambient,
    safe_play_ghsound_effect_ambient,
    safe_set_effect_ambient_volume,
    safe_play_ghsound_music,
    safe_play_ghsound_level_ambient,
    safe_play_ghsound_environment_ambient,
    safe_adjust_ghsound_general_volumes,
    safe_add_ambient_ghsound,
    safe_delete_ambient_ghsound,
    safe_set_ambient_ghsound_volume,
    safe_clear_context_menu,
    safe_add_context_menu,
    safe_update_status_button,
    safe_toggle_animation_timer,
    safe_display_floating_text,
    safe_display_screen_text,
    safe_display_popup_text,
    safe_display_gui_effect,
    safe_update_cursor,
    safe_ui_has_input,
    safe_exit_hack,
};


struct window_procs *
get_safe_procs(int optn)
{
    if (optn) {
        /* include the slightly more functional stdc versions */
        safe_procs.win_raw_print = stdio_raw_print;
        safe_procs.win_raw_print_bold = stdio_raw_print_bold;
        safe_procs.win_nhgetch = stdio_nhgetch;
        safe_procs.win_wait_synch = stdio_wait_synch;
    }
    return &safe_procs;
}

/*ARGSUSED*/
void
safe_init_nhwindows(int *argcp UNUSED, char **argv UNUSED)
{
    return;
}

void
safe_player_selection(void)
{
    return;
}

void
safe_askname(void)
{
    return;
}

void
safe_get_nh_event(void)
{
    return;
}

void
safe_suspend_nhwindows(const char *str)
{
    return;
}

void
safe_resume_nhwindows(void)
{
    return;
}

void
safe_exit_nhwindows(const char *str)
{
    return;
}

winid
safe_create_nhwindow_ex(int type, int style UNUSED, int glyph UNUSED, struct extended_create_window_info info UNUSED)
{
    return WIN_ERR;
}

void
safe_clear_nhwindow(winid window)
{
    return;
}

/*ARGSUSED*/
void
safe_display_nhwindow(winid window, boolean blocking)
{
    return;
}

void
safe_dismiss_nhwindow(winid window)
{
    return;
}

void
safe_destroy_nhwindow(winid window)
{
    return;
}

void
safe_curs(winid window, int x, int y)
{
    return;
}

void
safe_putstr_ex(winid window, const char *str, int attr, int color, int app)
{
    return;
}

void
safe_putstr_ex2(winid window, const char *str, const char *attrs, const char *colors, int attr, int color, int app)
{
    return;
}

void
safe_display_file(const char *fname, boolean complain)
{
    return;
}

void
safe_start_menu_ex(winid window, int style, int glyph, uint64_t mflags)
{
    return;
}

/*
 * Parameters:
 *   window: window to use, must be of type NHW_MENU
 *   glyph: glyph to display with item (not used)
 *   identifier: what to return if selected
 *   ch: keyboard accelerator (0 = pick our own)
 *   gch: group accelerator (0 = no group)
 *   attr: attribute for string (like safe_putstr())
 *   color: color for string (like safe_putstr_ex())
 *   str: menu string
 *   preselected: item is marked as selected
 */
/*ARGSUSED*/
/*
 * Add a menu item to the beginning of the menu list.  This list is reversed
 * later.
 */
void
safe_add_menu(winid window, int glyph UNUSED, const anything *identifier, char ch, char gch, int attr, int color, const char *str, boolean preselected)
{
    return;
}

/*
 * Parameters:
 *   window: window to use, must be of type NHW_MENU
 *   glyph: glyph to display with item (not used)
 *   identifier: what to return if selected
 *   ch: keyboard accelerator (0 = pick our own)
 *   gch: group accelerator (0 = no group)
 *   attr: attribute for string (like safe_putstr())
 *   color: color for string (like safe_putstr_ex())
 *   str: menu string
 *   preselected: item is marked as selected
 */
void
safe_add_extended_menu(winid window, int glyph UNUSED, const anything *identifier, char ch, char gch, int attr, int color, const char *str, boolean preselected, struct extended_menu_info info)
{
    return;
}

/*
 * Parameters:
 *   window: menu to use
 *   prompt, subtitle: prompt to for menu
 */
/*
 * End a menu in this window, window must a type NHW_MENU.
 */
void
safe_end_menu_ex(winid window, const char *prompt, const char *subtitle)
{
    return;
}

int
safe_select_menu(winid window, int how, menu_item **menu_list)
{
    return 0;
}

/* special hack for treating top line --More-- as a one item menu */
char
safe_message_menu(char let, int how, const char *mesg)
{
    return '\033';
}

void
safe_update_inventory(void)
{
    return;
}

void
safe_mark_synch(void)
{
}

void
safe_wait_synch(void)
{
}

#ifdef CLIPPING
void
safe_cliparound(int x, int y, boolean force)
{
}
#endif /* CLIPPING */

void
safe_issue_gui_command(int cmd_id, int cmd_param, int cmd_param2, const char *cmd_str)
{
    return;
}

/*
 *  safe_print_glyph
 *
 *  Print the glyph to the output device.  Don't flush the output device.
 */
void
safe_print_glyph(winid window, xchar x, xchar y, struct layer_info layers)
{
    return;
}

void
safe_raw_print(const char *str)
{
    return;
}

void
safe_raw_print_bold(const char *str)
{
    return;
}

int
safe_nhgetch(void)
{
    return '\033';
}

/*
 * return a key, or 0, in which case a mouse button was pressed
 * mouse events should be returned as character postitions in the map window.
 * Since normal tty's don't have mice, just return a key.
 */
/*ARGSUSED*/
int
safe_nh_poskey(int *x, int *y, int *mod)
{
    return '\033';
}

void
win_safe_init(int dir)
{
    return;
}

#ifdef POSITIONBAR
void
safe_update_positionbar(char *posbar)
{
    return;
}
#endif /* POSITIONBAR */

/*
 *  safe_status_init()
 *      -- initialize the port-specific data structures.
 */
void
safe_status_init(int reassessment)
{
    return;
}

boolean
safe_can_suspend(void)
{
    return FALSE;
}

void
safe_stretch_window(void)
{
    return;
}

void
safe_set_animation_timer_interval(unsigned int interval)
{
    return;
}

int
safe_open_special_view(struct special_view_info info)
{
    switch (info.viewtype)
    {
    case SPECIAL_VIEW_CHAT_MESSAGE:
        break;
    case SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N:
        return 'n';
    case SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y:
        return 'y';
    default:
        break;
    }
    return 0;
}

void
safe_stop_all_sounds(struct stop_all_info info)
{
    return;
}

void
safe_play_immediate_ghsound(struct ghsound_immediate_info info)
{
    return;
}

void
safe_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info)
{
    return;
}

void
safe_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info)
{
    return;
}

void
safe_set_effect_ambient_volume(struct effect_ambient_volume_info info)
{
    return;
}

void
safe_play_ghsound_music(struct ghsound_music_info info)
{
    return;
}

void
safe_play_ghsound_level_ambient(struct ghsound_level_ambient_info info)
{
    return;
}

void
safe_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info)
{
    return;
}

void
safe_adjust_ghsound_general_volumes(void)
{
    return;
}

void
safe_add_ambient_ghsound(struct soundsource_t* soundsource)
{
    return;
}

void
safe_delete_ambient_ghsound(struct soundsource_t* soundsource)
{
    return;
}

void
safe_set_ambient_ghsound_volume(struct soundsource_t* soundsource)
{
    return;
}

void
safe_clear_context_menu(void)
{
    return;
}

int
safe_ui_has_input(void)
{
    return FALSE;
}

void
safe_add_context_menu(int cmd_def_char UNUSED, int cmd_cur_char UNUSED, int style UNUSED, int glyph UNUSED, const char* cmd_text UNUSED, const char* target_text UNUSED, int attr UNUSED, int color UNUSED)
{
    return;
}

void
safe_update_status_button(int cmd UNUSED, int btn UNUSED, int val UNUSED, uint64_t bflags UNUSED)
{
    return;
}


void
safe_toggle_animation_timer(int type UNUSED, int id UNUSED, int state UNUSED, int x UNUSED, int y UNUSED, int layer UNUSED, uint64_t tflags UNUSED)
{
    return;
}

void
safe_display_floating_text(int x UNUSED, int y UNUSED, const char* text UNUSED, int style UNUSED, int attr UNUSED, int color UNUSED, uint64_t tflags UNUSED)
{
    return;
}

void
safe_display_screen_text(const char* text UNUSED, const char* supertext UNUSED, const char* subtext UNUSED, int style UNUSED, int attr UNUSED, int color UNUSED, uint64_t tflags UNUSED)
{
    return;
}

void
safe_display_popup_text(const char* text UNUSED, const char* title UNUSED, int style UNUSED, int attr UNUSED, int color UNUSED, int glyph UNUSED, uint64_t tflags UNUSED)
{
    return;
}

void
safe_display_gui_effect(int style UNUSED, int subtype UNUSED, int x UNUSED, int y UNUSED, int x2 UNUSED, int y2 UNUSED, uint64_t tflags UNUSED)
{
    return;
}

void
safe_update_cursor(int style UNUSED, int force_paint UNUSED, int show_on_u UNUSED)
{
    return;
}

void
safe_exit_hack(int status)
{

}


void
safe_nhbell(void)
{
    return;
}

int
safe_doprev_message(void)
{
    return 0;
}

char
safe_yn_function_ex(int style, int attr, int color, int glyph, const char *title, const char *query, const char *resp, char def, const char *resp_desc, const char *introline, uint64_t ynflags)
{
    return '\033';
}

/*ARGSUSED*/
void
safe_getlin_ex(int style UNUSED, int attr UNUSED, int color UNUSED, const char *prompt UNUSED, char *outbuf, const char *placeholder UNUSED, const char *linesuffix UNUSED, const char *introline UNUSED)
{
    Strcpy(outbuf, "\033");
}

int
safe_get_ext_cmd(void)
{
    return '\033';
}

void
safe_number_pad(int mode)
{
    return;
}

void
safe_delay_output(void)
{
    return;
}

void
safe_delay_output_milliseconds(int interval)
{
    return;
}

void
safe_delay_output_intervals(int intervals)
{
    return;
}

#ifdef CHANGE_COLOR
/*ARGSUSED*/
void
safe_change_color(int color, int64_t rgb, int reverse)
{
    return;
}

#ifdef MAC
/*ARGUSED*/
void
safe_change_background(int arg UNUSED)
{
    return;
}

/*ARGSUSED*/
short
safe_set_font_name(winid window, char *fontname)
{
    return 0;
}
#endif /* MAC */

char*
safe_get_color_string(void)
{
    return (char*)0;
}
#endif /* CHANGE_COLOR */


void
safe_start_screen(void)
{
    return;
}

void
safe_end_screen(void)
{
    return;
}

void
safe_outrip(winid tmpwin, int how, time_t when)
{
    return;
}

/*ARGSUSED*/
void
safe_preference_update(const char *pref UNUSED)
{
    return;
}

char*
safe_getmsghistory_ex(char **attrs_ptr, char **colors_ptr, boolean init UNUSED)
{
    if (attrs_ptr)
        *attrs_ptr = (char*)0;
    if (colors_ptr)
        *colors_ptr = (char*)0;
    return (char *) 0;
}

void
safe_putmsghistory_ex(const char *msg UNUSED, const char *attrs UNUSED, const char *colors UNUSED, boolean is_restoring UNUSED)
{
}

void
safe_status_finish(void)
{
}

void
safe_status_enablefield(int fieldidx, const char *nm, const char *fmt, boolean enable)
{
}

#ifdef STATUS_HILITES
/* call once for each field, then call with BL_FLUSH to output the result */
void
safe_status_update(int idx, genericptr_t ptr, int chg UNUSED, int percent UNUSED, int color UNUSED, uint64_t *colormasks UNUSED)
{
}
#endif /* STATUS_HILITES */

/**************************************************************
 * These are some optionally selectable routines that add
 * some base functionality over the safe_* versions above.
 * The safe_* versions are primarily designed to ensure that
 * there are no null function pointers remaining at early
 * game startup/initialization time.
 *
 * The slightly more functional versions in here should be kept
 * free of platform-specific code or OS-specific code. If you
 * want to use versions that involve platform-specific or
 * OS-specific code, go right ahead but use your own replacement
 * version of the functions in a platform-specific or
 * OS-specific source file, not in here.
 ***************************************************************/

/* Add to your code: windowprocs.win_raw_print = stdio_wait_synch; */
void
stdio_wait_synch(void)
{
#ifdef GNH_MOBILE
    return;
#else
    if (!nhgetch)
        return;

    char valid[] = {' ', '\n', '\r', '\033', '\0'};

    fprintf(stdout, "--More--");
    (void) fflush(stdout);
    while (!index(valid, nhgetch()))
        ;
#endif
}

/* Add to your code: windowprocs.win_raw_print = stdio_raw_print; */
void
stdio_raw_print(const char *str)
{
    if (str)
        fprintf(stdout, "%s\n", str);
    return;
}

/* Add to your code: windowprocs.win_raw_print_bold = stdio_raw_print_bold; */
void
stdio_raw_print_bold(const char *str)
{
    stdio_raw_print(str);
    return;
}

/* Add to your code: windowprocs.win_nhgetch = stdio_nhgetch; */
int
stdio_nhgetch(void)
{
    return getchar != 0 ? getchar() : safe_nhgetch();
}


/* safeprocs.c */

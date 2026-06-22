/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0	wc_chainin.c	$NHDT-Date: 1433806610 2015/06/08 23:36:50 $  $NHDT-Branch: master $:$NHDT-Revision: 1.7 $ */
/* Copyright (c) Kenneth Lorber, 2012				  */
/* GnollHack may be freely redistributed.  See license for details. */

/* -chainin is an internal processor that changes the flow from window_procs
 * to chain_procs. */

#include "hack.h"

struct chainin_data {
    struct chain_procs *nprocs;
    void *ndata;

    int linknum;
};

/* Normally, a processor gets this information from the first parm of each
 * call, but here we are keeping the original API, so that parm doesn't exist,
 * so we use this instead. */
static struct chainin_data *cibase;

void *
chainin_procs_chain(int cmd, int n, void *me, void *nextprocs, void *nextdata)
{
    struct chainin_data *tdp = 0;

    switch (cmd) {
    case WINCHAIN_ALLOC:
        tdp = (struct chainin_data *) alloc(sizeof *tdp);
        tdp->nprocs = 0;
        tdp->ndata = 0;
        tdp->linknum = n;
        cibase = 0;
        break;
    case WINCHAIN_INIT:
        tdp = me;
        tdp->nprocs = nextprocs;
        tdp->ndata = nextdata;
        break;
    default:
        panic("chainin_procs_chain: bad cmd\n");
        /*NOTREACHED*/
    }
    return tdp;
}

/* XXX if we don't need this, take it out of the table */
void
chainin_procs_init(int dir UNUSED)
{
}

/***
 *** winprocs
 ***/

void
chainin_init_nhwindows(int *argcp, char **argv)
{
    (*cibase->nprocs->win_init_nhwindows)(cibase->ndata, argcp, argv);
}

void
chainin_player_selection()
{
    (*cibase->nprocs->win_player_selection)(cibase->ndata);
}

void
chainin_askname()
{
    (*cibase->nprocs->win_askname)(cibase->ndata);
}

void
chainin_get_nh_event()
{
    (*cibase->nprocs->win_get_nh_event)(cibase->ndata);
}

void
chainin_exit_nhwindows(const char *str)
{
    (*cibase->nprocs->win_exit_nhwindows)(cibase->ndata, str);
}

void
chainin_suspend_nhwindows(const char *str)
{
    (*cibase->nprocs->win_suspend_nhwindows)(cibase->ndata, str);
}

void
chainin_resume_nhwindows()
{
    (*cibase->nprocs->win_resume_nhwindows)(cibase->ndata);
}

winid
chainin_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info)
{
    winid rv;

    rv = (*cibase->nprocs->win_create_nhwindow_ex)(cibase->ndata, type, style, glyph, info);

    return rv;
}

void
chainin_clear_nhwindow(winid window)
{
    (*cibase->nprocs->win_clear_nhwindow)(cibase->ndata, window);
}

void
chainin_display_nhwindow(winid window, boolean blocking)
{
    (*cibase->nprocs->win_display_nhwindow)(cibase->ndata, window, blocking);
}

void
chainin_destroy_nhwindow(winid window)
{
    (*cibase->nprocs->win_destroy_nhwindow)(cibase->ndata, window);
}

void
chainin_curs(winid window, int x, int y)
{
    (*cibase->nprocs->win_curs)(cibase->ndata, window, x, y);
}

void
chainin_putstr_ex(window, str, attr color, app,)
winid window;
int attr, app, color;
const char *str;
{
    (*cibase->nprocs->win_putstr_ex)(cibase->ndata, window, str, attr, color, app);
}

void
chainin_putstr_ex2(winid window, const char *str, const char *attrs, const char *colors, int attr, int color, int app)
{
    (*cibase->nprocs->win_putstr_ex2)(cibase->ndata, window, str, attrs, colors, attr, color, app);
}

void
chainin_putmixed_ex(winid window, const char *str, int attr, int color, int app)
{
    (*cibase->nprocs->win_putmixed_ex)(cibase->ndata, window, str, attr, color), app;
}

void
chainin_display_file(const char *fname, boolean complain)
{
    (*cibase->nprocs->win_display_file)(cibase->ndata, fname, complain);
}

void
chainin_start_menu_ex(winid window, int style)
{
    (*cibase->nprocs->win_start_menu_ex)(cibase->ndata, window, style);
}

/*
 * Parameters:
 *   window: window to use, must be of type NHW_MENU
 *   glyph: glyph to display with item (unused)
 *   identifier: what to return if selected
 *   ch: keyboard accelerator (0 = pick our own)
 *   gch: group accelerator (0 = no group)
 *   attr: attribute for string (like tty_putstr())
 *   color: color for string (like tty_putstr_ex())
 *   str: menu string
 *   preselected: item is marked as selected
 */
void
chainin_add_menu(winid window, int glyph, const anything *identifier, char ch, char gch, int attr, int color, const char *str, boolean preselected)
{
    (*cibase->nprocs->win_add_menu)(cibase->ndata, window, glyph, identifier,
                                    ch, gch, attr, color, str, preselected);
}

/*
 * Parameters:
 *   window: window to use, must be of type NHW_MENU
 *   glyph: glyph to display with item (unused)
 *   identifier: what to return if selected
 *   ch: keyboard accelerator (0 = pick our own)
 *   gch: group accelerator (0 = no group)
 *   attr: attribute for string (like tty_putstr())
 *   color: color for string (like tty_putstr_ex())
 *   str: menu string
 *   preselected: item is marked as selected
 */
void
chainin_add_extended_menu(winid window, int glyph, const anything *identifier, char ch, char gch, int attr, int color, const char *str, boolean preselected, struct extended_menu_info info)
{
    (*cibase->nprocs->win_add_extended_menu)(cibase->ndata, window, glyph, identifier,
        ch, gch, attr, color, str, preselected, info);
}
void
chainin_end_menu_ex(winid window, const char *prompt, const char *subtitle)
{
    (*cibase->nprocs->win_end_menu_ex)(cibase->ndata, window, prompt, subtitle);
}

int
chainin_select_menu(winid window, int how, menu_item **menu_list)
{
    int rv;

    rv = (*cibase->nprocs->win_select_menu)(cibase->ndata, window, how,
                                            (void *) menu_list);

    return rv;
}

char
chainin_message_menu(char let, int how, const char *mesg)
{
    char rv;

    rv = (*cibase->nprocs->win_message_menu)(cibase->ndata, let, how, mesg);

    return rv;
}

void
chainin_update_inventory()
{
    (*cibase->nprocs->win_update_inventory)(cibase->ndata);
}

void
chainin_mark_synch()
{
    (*cibase->nprocs->win_mark_synch)(cibase->ndata);
}

void
chainin_wait_synch()
{
    (*cibase->nprocs->win_wait_synch)(cibase->ndata);
}

#ifdef CLIPPING
void
chainin_cliparound(int x, int y, boolean force)
{
    (*cibase->nprocs->win_cliparound)(cibase->ndata, x, y, force);
}
#endif

#ifdef POSITIONBAR
void
chainin_update_positionbar(char *posbar)
{
    (*cibase->nprocs->win_update_positionbar)(cibase->ndata, posbar);
}
#endif

/* XXX can we decode the glyph in a meaningful way? */
void
chainin_issue_gui_command(int cmd_id, int cmd_param, int cmd_param2, const char *cmd_str)
{
    (*cibase->nprocs->win_issue_gui_command)(cibase->ndata, cmd_id, cmd_param, cmd_param2, cmd_str);
}

void
chainin_print_glyph(winid window, xchar x, xchar y, struct layer_info layers)
{
    (*cibase->nprocs->win_print_glyph)(cibase->ndata, window, x, y, layers);
}

void
chainin_raw_print(const char *str)
{
    (*cibase->nprocs->win_raw_print)(cibase->ndata, str);
}

void
chainin_raw_print_bold(const char *str)
{
    (*cibase->nprocs->win_raw_print_bold)(cibase->ndata, str);
}

int
chainin_nhgetch()
{
    int rv;

    rv = (*cibase->nprocs->win_nhgetch)(cibase->ndata);

    return rv;
}

int
chainin_nh_poskey(int *x, int *y, int *mod)
{
    int rv;

    rv = (*cibase->nprocs->win_nh_poskey)(cibase->ndata, x, y, mod);

    return rv;
}

void
chainin_nhbell()
{
    (*cibase->nprocs->win_nhbell)(cibase->ndata);
}

int
chainin_doprev_message()
{
    int rv;

    rv = (*cibase->nprocs->win_doprev_message)(cibase->ndata);

    return rv;
}

char
chainin_yn_function_ex(int style, int attr, int color, int glyph, const char *title, const char *query, const char *resp, char def, const char *resp_desc, const char *introline, uint64_t ynflags)
{
    int rv;

    rv = (*cibase->nprocs->win_yn_function)(cibase->ndata, style, attr, color, glyph, title, query, resp, def, resp_desc, ynflags);

    return rv;
}

void
chainin_getlin(int attr, int color, const char *query, char *bufp)
{
    (*cibase->nprocs->win_getlin)(cibase->ndata, query, bufp);
}

int
chainin_get_ext_cmd()
{
    int rv;

    rv = (*cibase->nprocs->win_get_ext_cmd)(cibase->ndata);

    return rv;
}

void
chainin_number_pad(int state)
{
    (*cibase->nprocs->win_number_pad)(cibase->ndata, state);
}

void
chainin_delay_output()
{
    (*cibase->nprocs->win_delay_output)(cibase->ndata);
}

void
chainin_delay_output_milliseconds(int interval)
{
    (*cibase->nprocs->win_delay_output_milliseconds)(cibase->ndata, interval);
}

void
chainin_delay_output_intervals(int intervals)
{
    (*cibase->nprocs->win_delay_output_intervals)(cibase->ndata, intervals);
}

#ifdef CHANGE_COLOR
void
chainin_change_color(int color, int64_t value, int reverse)
{
    (*cibase->nprocs->win_change_color)(cibase->ndata, color, value, reverse);
}

#ifdef MAC
void
chainin_change_background(int bw)
{
    (*cibase->nprocs->win_change_background)(cibase->ndata, bw);
}

short
chainin_set_font_name(winid window, char *font)
{
    short rv;

    rv = (*cibase->nprocs->win_set_font_name)(cibase->ndata, window, font);

    return rv;
}
#endif

char *
trace_get_color_string()
{
    char *rv;

    rv = (*cibase->nprocs->win_get_color_string)(cibase->ndata);

    return rv;
}

#endif

/* other defs that really should go away (they're tty specific) */
void
chainin_start_screen()
{
    (*cibase->nprocs->win_start_screen)(cibase->ndata);
}

void
chainin_end_screen()
{
    (*cibase->nprocs->win_end_screen)(cibase->ndata);
}

void
chainin_outrip(winid tmpwin, int how, time_t when)
{
    (*cibase->nprocs->win_outrip)(cibase->ndata, tmpwin, how, when);
}

void
chainin_preference_update(const char *pref)
{
    (*cibase->nprocs->win_preference_update)(cibase->ndata, pref);
}

char*
chainin_getmsghistory_ex(boolean init, char **attrs_ptr, char **colors_ptr)
{
    char *rv;

    rv = (*cibase->nprocs->win_getmsghistory_ex)(cibase->ndata, attrs_ptr, colors_ptr, init);

    return rv;
}

void
chainin_putmsghistory_ex(const char *msg, const char *attrs, const char *colors, boolean is_restoring)
{
    (*cibase->nprocs->win_putmsghistory_ex)(cibase->ndata, msg, attrs, colors, is_restoring);
}

void
chainin_status_init(int reassessment)
{
    (*cibase->nprocs->win_status_init)(cibase->ndata, reassessment);
}

void
chainin_status_finish()
{
    (*cibase->nprocs->win_status_finish)(cibase->ndata);
}

void
chainin_status_enablefield(int fieldidx, const char *nm, const char *fmt, boolean enable)
{
    (*cibase->nprocs->win_status_enablefield)(cibase->ndata, fieldidx, nm,
                                              fmt, enable);
}

void
chainin_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, uint64_t *colormasks)
{
    (*cibase->nprocs->win_status_update)(cibase->ndata, idx, ptr, chg,
                                         percent, color, colormasks);
}

boolean
chainin_can_suspend()
{
    boolean rv;

    rv = (*cibase->nprocs->win_can_suspend)(cibase->ndata);

    return rv;
}

struct window_procs chainin_procs = {
    "-chainin", 0, /* wincap */
    0,             /* wincap2 */
    /*
    XXX problem - the above need to come from the real window port, possibly
    modified.  May need to do something to call an additional init fn later
    or if this is the only place like this the choose_windows fn can do the
    fixup
    (but not if the value can be modified by the stack?)  TBD
    */
    chainin_init_nhwindows,
    chainin_player_selection, chainin_askname, chainin_get_nh_event,
    chainin_exit_nhwindows, chainin_suspend_nhwindows,
    chainin_resume_nhwindows, chainin_create_nhwindow_ex, chainin_clear_nhwindow,
    chainin_display_nhwindow, chainin_destroy_nhwindow, chainin_curs,
    chainin_putstr_ex, chainin_putstr_ex2, chainin_putmixed_ex, chainin_display_file,
    chainin_start_menu_ex, chainin_add_menu, chainin_add_extended_menu, chainin_end_menu_ex,
    chainin_select_menu, chainin_message_menu, chainin_update_inventory,
    chainin_mark_synch, chainin_wait_synch,
#ifdef CLIPPING
    chainin_cliparound,
#endif
#ifdef POSITIONBAR
    chainin_update_positionbar,
#endif
    chainin_print_glyph, chainin_issue_gui_command, chainin_raw_print, chainin_raw_print_bold,
    chainin_nhgetch, chainin_nh_poskey, chainin_nhbell,
    chainin_doprev_message, chainin_yn_function_ex, chainin_getlin_ex,
    chainin_get_ext_cmd, chainin_number_pad, chainin_delay_output, chainin_delay_output_milliseconds, chainin_delay_output_intervals,
#ifdef CHANGE_COLOR
    chainin_change_color,
#ifdef MAC
    chainin_change_background, chainin_set_font_name,
#endif
    chainin_get_color_string,
#endif

    chainin_start_screen, chainin_end_screen,

    chainin_outrip, chainin_preference_update, chainin_getmsghistory_ex,
    chainin_putmsghistory_ex,
    chainin_status_init, chainin_status_finish, chainin_status_enablefield,
    chainin_status_update,
    chainin_can_suspend,
    genl_stretch_window,
    genl_set_animation_timer_interval,
    genl_open_special_view,
    genl_stop_all_sounds,
    genl_play_immediate_ghsound,
    genl_play_ghsound_occupation_ambient,
    genl_play_ghsound_effect_ambient,
    genl_set_effect_ambient_volume,
    genl_play_ghsound_music,
    genl_play_ghsound_level_ambient,
    genl_play_ghsound_environment_ambient,
    genl_adjust_ghsound_general_volumes,
    genl_add_ambient_ghsound,
    genl_delete_ambient_ghsound,
    genl_set_ambient_ghsound_volume,
    genl_clear_context_menu,
    genl_add_context_menu,
    genl_update_status_button,
    genl_toggle_animation_timer,
    genl_display_floating_text,
    genl_display_screen_text,
    genl_display_popup_text,
    genl_display_gui_effect,
    genl_update_cursor,
    genl_ui_has_input,
    genl_exit_hack,
};

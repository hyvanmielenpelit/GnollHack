/*
 *  dllproc.c
 * Copyright (c) Janne Gustafsson, 2021
 */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * This file implements the interface between the window port specific
 * code in the mswin port and the rest of the GnollHack game engine.
*/

#include "win10.h"
#include "hack.h"
#include "dlb.h"
#include "func_tab.h" /* for extended commands */
#include "dllproc.h"
#include <assert.h>
#if 0
#include "winMS.h"
#include "color.h"
#include <mmsystem.h>
#include "mhmap.h"
#include "mhstatus.h"
#include "mhtext.h"
#include "mhmsgwnd.h"
#include "mhmenu.h"
#include "mhsplash.h"
#include "mhmsg.h"
#include "mhinput.h"
#include "mhaskyn.h"
#include "mhdlg.h"
#include "mhrip.h"
#include "mhmain.h"
#include "mhfont.h"
#endif
#include "resource.h"
#include "callback.h"
#include "dllhack.h"

#define LLEN 128

#define NHTRACE_LOG "nhtrace.log"

#ifdef DEBUG
# ifdef _DEBUG
static FILE* _s_debugfp = NULL;
extern void dll_logDebug(const char *fmt, ...);
# endif
#endif

#ifndef _DEBUG
void
dll_logDebug(const char *fmt, ...)
{
}
#endif

strbuf_t dll_raw_print_strbuf = { 0 };

/* Interface definition, for windows.c */
struct window_procs dll_procs = {
    "DLL",
    WC_COLOR | WC_HILITE_PET | WC_ALIGN_MESSAGE | WC_ALIGN_STATUS | WC_INVERSE
        | WC_SCROLL_AMOUNT | WC_SCROLL_MARGIN | WC_MAP_MODE | WC_FONT_MESSAGE
        | WC_FONT_STATUS | WC_FONT_MENU | WC_FONT_TEXT | WC_FONT_MAP
        | WC_FONTSIZ_MESSAGE | WC_FONTSIZ_STATUS | WC_FONTSIZ_MENU
        | WC_FONTSIZ_TEXT | WC_TILE_WIDTH | WC_TILE_HEIGHT | WC_TILE_FILE
        | WC_VARY_MSGCOUNT | WC_WINDOWCOLORS | WC_PLAYER_SELECTION
        | WC_SPLASH_SCREEN | WC_POPUP_DIALOG | WC_MOUSE_SUPPORT,
#ifdef STATUS_HILITES
    WC2_HITPOINTBAR | WC2_FLUSH_STATUS | WC2_RESET_STATUS | WC2_HILITE_STATUS |
#endif
    WC2_PREFERRED_SCREEN_SCALE, dll_init_nhwindows, dll_player_selection, dll_askname,
    dll_get_nh_event, dll_exit_nhwindows, dll_suspend_nhwindows,
    dll_resume_nhwindows, dll_create_nhwindow_ex, dll_clear_nhwindow,
    dll_display_nhwindow, dll_destroy_nhwindow, dll_curs, dll_putstr_ex, dll_putstr_ex2,
    genl_putmixed_ex, dll_display_file, dll_start_menu_ex, dll_add_menu, dll_add_extended_menu,
    dll_end_menu_ex, dll_select_menu,
    genl_message_menu, /* no need for X-specific handling */
    dll_update_inventory, dll_mark_synch, dll_wait_synch,
#ifdef CLIPPING
    dll_cliparound,
#endif
#ifdef POSITIONBAR
    donull,
#endif
    dll_print_glyph, dll_issue_gui_command, dll_raw_print, dll_raw_print_bold, dll_nhgetch,
    dll_nh_poskey, dll_nhbell, dll_doprev_message, dll_yn_function_ex,
    dll_getlin_ex, dll_get_ext_cmd, dll_number_pad, dll_delay_output, dll_delay_output_milliseconds, dll_delay_output_intervals,
#ifdef CHANGE_COLOR /* only a Mac option currently */
    mswin, dll_change_background,
#endif
    /* other defs that really should go away (they're tty specific) */
    dll_start_screen, dll_end_screen, dll_outrip,
    dll_preference_update, dll_getmsghistory_ex, dll_putmsghistory_ex,
    dll_status_init, dll_status_finish, dll_status_enablefield,
    dll_status_update,
    genl_can_suspend_yes,
    dll_stretch_window,
    dll_set_animation_timer_interval,
    dll_open_special_view,
    dll_stop_all_sounds,
    dll_play_immediate_ghsound,
    dll_play_ghsound_occupation_ambient,
    dll_play_ghsound_effect_ambient,
    dll_set_effect_ambient_volume,
    dll_play_ghsound_music,
    dll_play_ghsound_level_ambient,
    dll_play_ghsound_environment_ambient,
    dll_adjust_ghsound_general_volumes,
    dll_add_ambient_ghsound,
    dll_delete_ambient_ghsound,
    dll_set_ambient_ghsound_volume,
    dll_clear_context_menu,
    dll_add_context_menu,
    dll_update_status_button,
    dll_toggle_animation_timer,
    dll_display_floating_text,
    dll_display_screen_text,
    dll_display_popup_text,
    dll_display_gui_effect,
    dll_update_cursor,
    dll_ui_has_input,
    dll_exit_hack,
};

struct callback_procs dll_callbacks = { 0 }; /* To be set by RunGnollHack in dllmain.c */

/*
init_nhwindows(int* argcp, char** argv)
                -- Initialize the windows used by GnollHack.  This can also
                   create the standard windows listed at the top, but does
                   not display them.
                -- Any commandline arguments relevant to the windowport
                   should be interpreted, and *argcp and *argv should
                   be changed to remove those arguments.
                -- When the message window is created, the variable
                   iflags.window_inited needs to be set to TRUE.  Otherwise
                   all plines() will be done via raw_print().
                ** Why not have init_nhwindows() create all of the "standard"
                ** windows?  Or at least all but WIN_INFO?      -dean
*/
#define DLL_DEF_CLIPAROUND_MARGIN 5
#define DLL_DEF_CLIPAROUND_AMOUNT 1
#define DLL_NHFONT_DEFAULT_SIZE 9
#define DLL_NHFONT_SIZE_MIN 3
#define DLL_NHFONT_SIZE_MAX 20

#define DLL_MAX_LOADSTRING 100


void
dll_init_nhwindows(int *argc, char **argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

#ifdef DEBUG
# ifdef _DEBUG
    if (showdebug(NHTRACE_LOG) && !_s_debugfp) {
        /* truncate trace file */
        _s_debugfp = fopen(NHTRACE_LOG, "w");
    }
# endif
#endif
    dll_logDebug("dll_init_nhwindows()\n");

    /* set it to WIN_ERR so we can detect attempts to
       use this ID before it is inialized */
    WIN_MAP = WIN_ERR;

    /* check default values */
    if (iflags.wc_fontsiz_status < DLL_NHFONT_SIZE_MIN
        || iflags.wc_fontsiz_status > DLL_NHFONT_SIZE_MAX)
        iflags.wc_fontsiz_status = DLL_NHFONT_DEFAULT_SIZE;

    if (iflags.wc_fontsiz_message < DLL_NHFONT_SIZE_MIN
        || iflags.wc_fontsiz_message > DLL_NHFONT_SIZE_MAX)
        iflags.wc_fontsiz_message = DLL_NHFONT_DEFAULT_SIZE;

    if (iflags.wc_fontsiz_text < DLL_NHFONT_SIZE_MIN
        || iflags.wc_fontsiz_text > DLL_NHFONT_SIZE_MAX)
        iflags.wc_fontsiz_text = DLL_NHFONT_DEFAULT_SIZE;

    if (iflags.wc_fontsiz_menu < DLL_NHFONT_SIZE_MIN
        || iflags.wc_fontsiz_menu > DLL_NHFONT_SIZE_MAX)
        iflags.wc_fontsiz_menu = DLL_NHFONT_DEFAULT_SIZE;

    if (iflags.wc_align_message == 0)
        iflags.wc_align_message = ALIGN_TOP;
    if (iflags.wc_align_status == 0)
        iflags.wc_align_status = ALIGN_BOTTOM;
    if (iflags.wc_scroll_margin == 0)
        iflags.wc_scroll_margin = DLL_DEF_CLIPAROUND_MARGIN;
    if (iflags.wc_scroll_amount == 0)
        iflags.wc_scroll_amount = DLL_DEF_CLIPAROUND_AMOUNT;
    if (iflags.wc_tile_width == 0)
        iflags.wc_tile_width = TILE_X;
    if (iflags.wc_tile_height == 0)
        iflags.wc_tile_height = TILE_Y;

    if (iflags.wc_vary_msgcount == 0)
        iflags.wc_vary_msgcount = 4;

    /* force tabs in menus */
    iflags.menu_tab_sep = 1;

    /* force toptenwin to be true.  toptenwin is the option that decides
     * whether to
     * write output to a window or stdout.  stdout doesn't make sense on
     * Windows
     * non-console applications
     */
    iflags.toptenwin = 1;
    set_option_mod_status("toptenwin", SET_IN_FILE);
    //set_option_mod_status("perm_invent", SET_IN_FILE);
    set_option_mod_status("mouse_support", SET_IN_GAME);

    /* set tile-related options to readonly */
    set_wc_option_mod_status(WC_TILE_WIDTH | WC_TILE_HEIGHT | WC_TILE_FILE,
                             DISP_IN_GAME);

    /* set font-related options to change in the game */
    set_wc_option_mod_status(
        WC_HILITE_PET | WC_ALIGN_MESSAGE | WC_ALIGN_STATUS | WC_SCROLL_AMOUNT
            | WC_SCROLL_MARGIN | WC_MAP_MODE | WC_FONT_MESSAGE
            | WC_FONT_STATUS | WC_FONT_MENU | WC_FONT_TEXT
            | WC_FONTSIZ_MESSAGE | WC_FONTSIZ_STATUS | WC_FONTSIZ_MENU
            | WC_FONTSIZ_TEXT | WC_VARY_MSGCOUNT,
        SET_IN_GAME);

    dll_callbacks.callback_init_nhwindows();

    iflags.window_inited = TRUE;
}

/* Do a window-port specific player type selection. If player_selection()
   offers a Quit option, it is its responsibility to clean up and terminate
   the process. You need to fill in pl_character[0].
*/
void
dll_player_selection(void)
{
    int res = common_player_selection();
    if (res == 1)
    {
        res = dll_callbacks.callback_player_selection();
        if (res != 0)
            dll_bail((char*)0);
    }
    else if (res == 2)
    {
        dll_bail((char*)0);
    }
#if 0
    dll_logDebug("dll_player_selection()\n");

    if (iflags.wc_player_selection == VIA_DIALOG) 
    {
        /* pick player type randomly (use pre-selected
         * role/race/gender/alignment) */
        if (flags.randomall)
        {
            if (flags.initrole < 0) 
            {
                flags.initrole = pick_role(flags.initrace, flags.initgend,
                                           flags.initalign, PICK_RANDOM);
                if (flags.initrole < 0)
                {
                    raw_print("Incompatible role!");
                    flags.initrole = randrole(FALSE);
                }
            }

            if (flags.initrace < 0
                || !validrace(flags.initrole, flags.initrace))
            {
                flags.initrace = pick_race(flags.initrole, flags.initgend,
                                           flags.initalign, PICK_RANDOM);
                if (flags.initrace < 0) 
                {
                    raw_print("Incompatible race!");
                    flags.initrace = randrace(flags.initrole);
                }
            }

            if (flags.initgend < 0
                || !validgend(flags.initrole, flags.initrace,
                              flags.initgend)) 
            {
                flags.initgend = pick_gend(flags.initrole, flags.initrace,
                                           flags.initalign, PICK_RANDOM);
                if (flags.initgend < 0) 
                {
                    raw_print("Incompatible gender!");
                    flags.initgend = randgend(flags.initrole, flags.initrace);
                }
            }

            if (flags.initalign < 0
                || !validalign(flags.initrole, flags.initrace,
                               flags.initalign)) 
            {
                flags.initalign = pick_align(flags.initrole, flags.initrace,
                                             flags.initgend, PICK_RANDOM);
                if (flags.initalign < 0) 
                {
                    raw_print("Incompatible alignment!");
                    flags.initalign =
                        randalign(flags.initrole, flags.initrace);
                }
            }
        } 
        else 
        {
            /* select a role */
            if (!dll_callbacks.callback_player_selection())
            {
                dll_bail(0);
            }
        }
    } 
    else 
    { /* iflags.wc_player_selection == VIA_PROMPTS */
        prompt_for_player_selection();
    }
#endif
}

#if 0
void
prompt_for_player_selection(void)
{
    int i, k, n;
    char pick4u = 'n', thisch, lastch = 0;
    char pbuf[QBUFSZ], plbuf[QBUFSZ];
    winid win;
    anything any;
    menu_item *selected = 0;
    DWORD box_result;

    dll_logDebug("prompt_for_player_selection()\n");

    /* prevent an unnecessary prompt */
    rigid_role_checks();

    /* Should we randomly pick for the player? */
    if (!flags.randomall
        && (flags.initrole == ROLE_NONE || flags.initrace == ROLE_NONE
            || flags.initgend == ROLE_NONE || flags.initalign == ROLE_NONE)
        ) 
    {
        /* int echoline; */
        char *prompt = build_plselection_prompt(
            pbuf, QBUFSZ, flags.initrole, flags.initrace, flags.initgend,
            flags.initalign, FALSE);

        /* tty_putstr(BASE_WINDOW, 0, ""); */
        /* echoline = wins[BASE_WINDOW]->cury; */
        box_result = GNHMessageBox(prompt, MB_YESNOCANCEL | MB_DEFBUTTON1
                                                    | MB_ICONQUESTION);
        pick4u =
            (box_result == IDYES) ? 'y' : (box_result == IDNO) ? 'n' : '\033';
        /* tty_putstr(BASE_WINDOW, 0, prompt); */
        do 
        {
            /* pick4u = lowc(readchar()); */
            if (index(quitchars, pick4u))
                pick4u = 'y';
        } 
        while (!index(ynqchars, pick4u));

        if ((int) strlen(prompt) + 1 < CO) 
        {
            /* Echo choice and move back down line */
            /* tty_putsym(BASE_WINDOW, (int)strlen(prompt)+1, echoline,
             * pick4u); */
            /* tty_putstr(BASE_WINDOW, 0, ""); */
        } 
        else
            /* Otherwise it's hard to tell where to echo, and things are
             * wrapping a bit messily anyway, so (try to) make sure the next
             * question shows up well and doesn't get wrapped at the
             * bottom of the window.
             */
            /* tty_clear_nhwindow(BASE_WINDOW) */;

        if (pick4u != 'y' && pick4u != 'n') 
        {
        give_up: /* Quit */
            if (selected)
                free((genericptr_t) selected);
            dll_bail((char *) 0);
            /*NOTREACHED*/
            return;
        }
    }

    (void) root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
                                   flags.initrace, flags.initgend,
                                   flags.initalign);

    /* Select a role, if necessary */
    /* we'll try to be compatible with pre-selected race/gender/alignment,
     * but may not succeed */
    if (flags.initrole < 0)
    {
        char rolenamebuf[QBUFSZ];
        /* Process the choice */
        if (pick4u == 'y' || flags.initrole == ROLE_RANDOM
            || flags.randomall) 
        {
            /* Pick a random role */
            flags.initrole = pick_role(flags.initrace, flags.initgend,
                                       flags.initalign, PICK_RANDOM);
            if (flags.initrole < 0)
            {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible role!"); */
                flags.initrole = randrole(FALSE);
            }
        } 
        else
        {
            /* tty_clear_nhwindow(BASE_WINDOW); */
            /* tty_putstr(BASE_WINDOW, 0, "Choosing Character's Role"); */
            /* Prompt for a role */
            win = create_nhwindow(NHW_MENU);
            start_menu(win);
            any = zeroany; /* zero out all bits */
            for (i = 0; roles[i].name.m; i++) 
            {
                if (ok_role(i, flags.initrace, flags.initgend,
                            flags.initalign)) 
                {
                    any.a_int = i + 1; /* must be non-zero */
                    thisch = lowc(roles[i].name.m[0]);
                    if (thisch == lastch)
                        thisch = highc(thisch);
                    if (flags.initgend != ROLE_NONE
                        && flags.initgend != ROLE_RANDOM) 
                    {
                        if (flags.initgend == 1 && roles[i].name.f)
                            Strcpy(rolenamebuf, roles[i].name.f);
                        else
                            Strcpy(rolenamebuf, roles[i].name.m);
                    }
                    else 
                    {
                        if (roles[i].name.f)
                        {
                            Strcpy(rolenamebuf, roles[i].name.m);
                            Strcat(rolenamebuf, "/");
                            Strcat(rolenamebuf, roles[i].name.f);
                        } 
                        else
                            Strcpy(rolenamebuf, roles[i].name.m);
                    }
                    add_menu(win, NO_GLYPH, &any, thisch, 0, ATR_NONE,
                             an(rolenamebuf), MENU_UNSELECTED);
                    lastch = thisch;
                }
            }
            any.a_int = pick_role(flags.initrace, flags.initgend,
                                  flags.initalign, PICK_RANDOM) + 1;
            if (any.a_int == 0) /* must be non-zero */
                any.a_int = randrole(FALSE) + 1;
            add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random",
                     MENU_UNSELECTED);
            any.a_int = i + 1; /* must be non-zero */
            add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, "Quit",
                     MENU_UNSELECTED);
            Sprintf(pbuf, "Pick a role for your %s", plbuf);
            end_menu(win, pbuf);
            n = select_menu(win, PICK_ONE, &selected);
            destroy_nhwindow(win);

            /* Process the choice */
            if (n != 1 || selected[0].item.a_int == any.a_int)
                goto give_up; /* Selected quit */

            flags.initrole = selected[0].item.a_int - 1;
            free((genericptr_t) selected), selected = 0;
        }
        (void) root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
                                       flags.initrace, flags.initgend,
                                       flags.initalign);
    }

    /* Select a race, if necessary */
    /* force compatibility with role, try for compatibility with
     * pre-selected gender/alignment */
    if (flags.initrace < 0 || !validrace(flags.initrole, flags.initrace)) 
    {
        /* pre-selected race not valid */
        if (pick4u == 'y' || flags.initrace == ROLE_RANDOM
            || flags.randomall) {
            flags.initrace = pick_race(flags.initrole, flags.initgend,
                                       flags.initalign, PICK_RANDOM);
            if (flags.initrace < 0) {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible race!"); */
                flags.initrace = randrace(flags.initrole);
            }
        } 
        else 
        { /* pick4u == 'n' */
            /* Count the number of valid races */
            n = 0; /* number valid */
            k = 0; /* valid race */
            for (i = 0; races[i].noun; i++)
            {
                if (ok_race(flags.initrole, i, flags.initgend,
                            flags.initalign)) {
                    n++;
                    k = i;
                }
            }

            if (n == 0) {
                for (i = 0; races[i].noun; i++) 
                {
                    if (validrace(flags.initrole, i)) 
                    {
                        n++;
                        k = i;
                    }
                }
            }

            /* Permit the user to pick, if there is more than one */
            if (n > 1) 
            {
                /* tty_clear_nhwindow(BASE_WINDOW); */
                /* tty_putstr(BASE_WINDOW, 0, "Choosing Race"); */
                win = create_nhwindow(NHW_MENU);
                start_menu(win);
                any = zeroany; /* zero out all bits */
                for (i = 0; races[i].noun; i++)
                    if (ok_race(flags.initrole, i, flags.initgend,
                                flags.initalign)) 
                    {
                        any.a_int = i + 1; /* must be non-zero */
                        add_menu(win, NO_GLYPH, &any, races[i].noun[0], 0,
                                 ATR_NONE, races[i].noun, MENU_UNSELECTED);
                    }

                any.a_int = pick_race(flags.initrole, flags.initgend,
                                      flags.initalign, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randrace(flags.initrole) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random",
                         MENU_UNSELECTED);
                any.a_int = i + 1; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, "Quit",
                         MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the race of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == any.a_int)
                    goto give_up; /* Selected quit */

                k = selected[0].item.a_int - 1;
                free((genericptr_t) selected), selected = 0;
            }
            flags.initrace = k;
        }
        (void) root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
                                       flags.initrace, flags.initgend,
                                       flags.initalign);
    }

    /* Select a gender, if necessary */
    /* force compatibility with role/race, try for compatibility with
     * pre-selected alignment */
    if (flags.initgend < 0
        || !validgend(flags.initrole, flags.initrace, flags.initgend)) 
    {
        /* pre-selected gender not valid */
        if (pick4u == 'y' || flags.initgend == ROLE_RANDOM
            || flags.randomall)
        {
            flags.initgend = pick_gend(flags.initrole, flags.initrace,
                                       flags.initalign, PICK_RANDOM);
            if (flags.initgend < 0) {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible gender!"); */
                flags.initgend = randgend(flags.initrole, flags.initrace);
            }
        } 
        else 
        { /* pick4u == 'n' */
            /* Count the number of valid genders */
            n = 0; /* number valid */
            k = 0; /* valid gender */
            for (i = 0; i < ROLE_GENDERS; i++) 
            {
                if (ok_gend(flags.initrole, flags.initrace, i,
                            flags.initalign)) 
                {
                    n++;
                    k = i;
                }
            }

            if (n == 0)
            {
                for (i = 0; i < ROLE_GENDERS; i++)
                {
                    if (validgend(flags.initrole, flags.initrace, i))
                    {
                        n++;
                        k = i;
                    }
                }
            }

            /* Permit the user to pick, if there is more than one */
            if (n > 1)
            {
                /* tty_clear_nhwindow(BASE_WINDOW); */
                /* tty_putstr(BASE_WINDOW, 0, "Choosing Gender"); */
                win = create_nhwindow(NHW_MENU);
                start_menu(win);
                any = zeroany; /* zero out all bits */
                for (i = 0; i < ROLE_GENDERS; i++)
                    if (ok_gend(flags.initrole, flags.initrace, i,
                                flags.initalign))
                    {
                        any.a_int = i + 1;
                        add_menu(win, NO_GLYPH, &any, genders[i].adj[0], 0,
                                 ATR_NONE, genders[i].adj, MENU_UNSELECTED);
                    }

                any.a_int = pick_gend(flags.initrole, flags.initrace,
                                      flags.initalign, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randgend(flags.initrole, flags.initrace) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random",
                         MENU_UNSELECTED);
                any.a_int = i + 1; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, "Quit",
                         MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the gender of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == any.a_int)
                    goto give_up; /* Selected quit */

                k = selected[0].item.a_int - 1;
                free((genericptr_t) selected), selected = 0;
            }
            flags.initgend = k;
        }
        (void) root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
                                       flags.initrace, flags.initgend,
                                       flags.initalign);
    }

    /* Select an alignment, if necessary */
    /* force compatibility with role/race/gender */
    if (flags.initalign < 0
        || !validalign(flags.initrole, flags.initrace, flags.initalign)) 
    {
        /* pre-selected alignment not valid */
        if (pick4u == 'y' || flags.initalign == ROLE_RANDOM
            || flags.randomall) 
        {
            flags.initalign = pick_align(flags.initrole, flags.initrace,
                                         flags.initgend, PICK_RANDOM);
            if (flags.initalign < 0) 
            {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible alignment!"); */
                flags.initalign = randalign(flags.initrole, flags.initrace);
            }
        } 
        else 
        { /* pick4u == 'n' */
            /* Count the number of valid alignments */
            n = 0; /* number valid */
            k = 0; /* valid alignment */
            for (i = 0; i < ROLE_ALIGNS; i++) 
            {
                if (ok_align(flags.initrole, flags.initrace, flags.initgend,
                             i)) 
                {
                    n++;
                    k = i;
                }
            }
            if (n == 0) 
            {
                for (i = 0; i < ROLE_ALIGNS; i++) 
                {
                    if (validalign(flags.initrole, flags.initrace, i)) 
                    {
                        n++;
                        k = i;
                    }
                }
            }

            /* Permit the user to pick, if there is more than one */
            if (n > 1) 
            {
                /* tty_clear_nhwindow(BASE_WINDOW); */
                /* tty_putstr(BASE_WINDOW, 0, "Choosing Alignment"); */
                win = create_nhwindow(NHW_MENU);
                start_menu(win);
                any = zeroany; /* zero out all bits */
                for (i = 0; i < ROLE_ALIGNS; i++)
                    if (ok_align(flags.initrole, flags.initrace,
                                 flags.initgend, i)) 
                    {
                        any.a_int = i + 1;
                        add_menu(win, NO_GLYPH, &any, aligns[i].adj[0], 0,
                                 ATR_NONE, aligns[i].adj, MENU_UNSELECTED);
                    }

                any.a_int = pick_align(flags.initrole, flags.initrace,
                                       flags.initgend, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randalign(flags.initrole, flags.initrace) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, "Random",
                         MENU_UNSELECTED);
                any.a_int = i + 1; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, "Quit",
                         MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the alignment of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == any.a_int)
                    goto give_up; /* Selected quit */

                k = selected[0].item.a_int - 1;
                free((genericptr_t) selected), selected = 0;
            }
            flags.initalign = k;
        }
    }
    /* Success! */
    /* tty_display_nhwindow(BASE_WINDOW, FALSE); */
}

#endif

/* Ask the user for a player name. */
void
dll_askname(void)
{
    dll_logDebug("dll_askname()\n");
    char* askedname = dll_callbacks.callback_askname("", "");
    if (!askedname)
    {
        dll_bail("bye-bye");
        /* not reached */
    }
    else
    {
        strncpy(plname, askedname, PL_NSIZ - 1);
        plname[PL_NSIZ - 1] = '\0';
    }
}

/* Does window event processing (e.g. exposure events).
   A noop for the tty and X window-ports.
*/
void
dll_get_nh_event(void)
{
    dll_logDebug("dll_get_nh_event()\n");
    dll_callbacks.callback_get_nh_event();
}

/* Exits the window system.  This should dismiss all windows,
   except the "window" used for raw_print().  str is printed if possible.
*/
void
dll_exit_nhwindows(const char *str)
{
    dll_logDebug("dll_exit_nhwindows(%s)\n", str);

    /* set things back to failsafes */
    windowprocs = *get_safe_procs(0);

    /* and make sure there is still a way to communicate something */
    windowprocs.win_raw_print = dll_raw_print;
    windowprocs.win_raw_print_bold = dll_raw_print_bold;
    windowprocs.win_wait_synch = dll_wait_synch;
    windowprocs.win_exit_hack = dll_exit_hack;

    dll_callbacks.callback_exit_nhwindows(str);
}

/* Prepare the window to be suspended. */
void
dll_suspend_nhwindows(const char *str)
{
    dll_logDebug("dll_suspend_nhwindows(%s)\n", str);
    dll_callbacks.callback_suspend_nhwindows(str);
}

/* Restore the windows after being suspended. */
void
dll_resume_nhwindows()
{
    dll_logDebug("dll_resume_nhwindows()\n");
    dll_callbacks.callback_resume_nhwindows();
}

/*  Create a window of type "type" which can be
        NHW_MESSAGE     (top line)
        NHW_STATUS      (bottom lines)
        NHW_MAP         (main dungeon)
        NHW_MENU        (inventory or other "corner" windows)
        NHW_TEXT        (help/text, full screen paged window)
*/
winid
dll_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info)
{
    winid i = 0;
    dll_logDebug("dll_create_nhwindow_ex(%d)\n", type);
    struct objclassdata ocdata = get_objclassdata(info.object);
    i = dll_callbacks.callback_create_nhwindow_ex(type, style, glyph, (info.object ? 1 : 0) | (info.monster ? 2 : 0) | (Hallucination ? 4 : 0), info.object, &ocdata);

#if 0
    /* Return the next available winid
     */

    MSNHMsgAddWnd data;

    for (i = 1; i < MAXWINDOWS; i++)
        if (GetNHApp()->windowlist[i].win == NULL
            && !GetNHApp()->windowlist[i].dead)
            break;
    if (i == MAXWINDOWS)
    {
        panic("ERROR:  No windows available...\n");
        return 0;
    }

    switch (type) {
    case NHW_MAP: {
        GetNHApp()->windowlist[i].win = mswin_init_map_window();
        GetNHApp()->windowlist[i].type = type;
        GetNHApp()->windowlist[i].dead = 0;
        break;
    }
    case NHW_MESSAGE: {
        GetNHApp()->windowlist[i].win = mswin_init_message_window();
        GetNHApp()->windowlist[i].type = type;
        GetNHApp()->windowlist[i].dead = 0;
        break;
    }
    case NHW_STATUS: {
        GetNHApp()->windowlist[i].win = mswin_init_status_window();
        GetNHApp()->windowlist[i].type = type;
        GetNHApp()->windowlist[i].dead = 0;
        break;
    }
    case NHW_MENU: {
        GetNHApp()->windowlist[i].win = NULL; // will create later
        GetNHApp()->windowlist[i].type = type;
        GetNHApp()->windowlist[i].dead = 1;
        break;
    }
    case NHW_TEXT: {
        GetNHApp()->windowlist[i].win = mswin_init_text_window();
        GetNHApp()->windowlist[i].type = type;
        GetNHApp()->windowlist[i].dead = 0;
        break;
    }
    }

    ZeroMemory(&data, sizeof(data));
    data.wid = i;
    SendMessage(GetNHApp()->hMainWnd, WM_MSNH_COMMAND,
                (WPARAM) MSNH_MSG_ADDWND, (LPARAM) &data);
#endif
    return i;
}

/* Clear the given window, when asked to. */
void
dll_clear_nhwindow(winid wid)
{
    dll_logDebug("dll_clear_nhwindow(%d)\n", wid);
    dll_callbacks.callback_clear_nhwindow((int)wid);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)
        && (GetNHApp()->windowlist[wid].win != NULL)) {
        if (GetNHApp()->windowlist[wid].type == NHW_MAP) {
            if (Is_really_rogue_level(&u.uz))
                if (iflags.wc_map_mode == MAP_MODE_ASCII_FIT_TO_SCREEN ||
                    iflags.wc_map_mode == MAP_MODE_TILES_FIT_TO_SCREEN)

                    mswin_map_mode(dll_hwnd_from_winid(WIN_MAP),
                                   ROGUE_LEVEL_MAP_MODE_FIT_TO_SCREEN);
                else
                    mswin_map_mode(dll_hwnd_from_winid(WIN_MAP),
                                   ROGUE_LEVEL_MAP_MODE);
            else
                mswin_map_mode(dll_hwnd_from_winid(WIN_MAP),
                               iflags.wc_map_mode);
        }

        SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CLEAR_WINDOW, (LPARAM) NULL);
    }
#endif
}

/* -- Display the window on the screen.  If there is data
                   pending for output in that window, it should be sent.
                   If blocking is TRUE, display_nhwindow() will not
                   return until the data has been displayed on the screen,
                   and acknowledged by the user where appropriate.
                -- All calls are blocking in the tty window-port.
                -- Calling display_nhwindow(WIN_MESSAGE,???) will do a
                   --more--, if necessary, in the tty window-port.
*/
void
dll_display_nhwindow(winid wid, BOOLEAN_P block)
{
    dll_logDebug("dll_display_nhwindow(%d, %d)\n", wid, block);
    dll_callbacks.callback_display_nhwindow((int)wid, (boolean)block);

#if 0
    if (GetNHApp()->windowlist[wid].win != NULL) {
        ShowWindow(GetNHApp()->windowlist[wid].win, SW_SHOW);
        mswin_layout_main_window(GetNHApp()->windowlist[wid].win);
        if (GetNHApp()->windowlist[wid].type == NHW_MENU) {
            MENU_ITEM_P *p;
            mswin_menu_window_select_menu(GetNHApp()->windowlist[wid].win,
                                          PICK_NONE, &p, TRUE);
        }
        if (GetNHApp()->windowlist[wid].type == NHW_TEXT) {
            mswin_display_text_window(GetNHApp()->windowlist[wid].win);
        }
        if (GetNHApp()->windowlist[wid].type == NHW_RIP) {
            mswin_display_RIP_window(GetNHApp()->windowlist[wid].win);
        } else {
            if (!block) {
                UpdateWindow(GetNHApp()->windowlist[wid].win);
            } else {
                if (GetNHApp()->windowlist[wid].type == NHW_MAP) {
                    (void) dll_nhgetch();
                }
            }
        }
        SetFocus(GetNHApp()->hMainWnd);
    }
#endif
}


/* Destroy will dismiss the window if the window has not
 * already been dismissed.
*/
void
dll_destroy_nhwindow(winid wid)
{
    dll_logDebug("dll_destroy_nhwindow(%d)\n", wid);
    dll_callbacks.callback_destroy_nhwindow((int)wid);

#if 0
    if ((GetNHApp()->windowlist[wid].type == NHW_MAP)
        || (GetNHApp()->windowlist[wid].type == NHW_MESSAGE)
        || (GetNHApp()->windowlist[wid].type == NHW_STATUS)) {
        /* main windows is going to take care of those */
        return;
    }

    if (wid != -1) {
        if (!GetNHApp()->windowlist[wid].dead
            && GetNHApp()->windowlist[wid].win != NULL)
            DestroyWindow(GetNHApp()->windowlist[wid].win);
        GetNHApp()->windowlist[wid].win = NULL;
        GetNHApp()->windowlist[wid].type = 0;
        GetNHApp()->windowlist[wid].dead = 0;
    }
#endif
}

/* Next output to window will start at (x,y), also moves
 displayable cursor to (x,y).  For backward compatibility,
 1 <= x < cols, 0 <= y < rows, where cols and rows are
 the size of window.
*/
void
dll_curs(winid wid, int x, int y)
{
    dll_logDebug("dll_curs(%d, %d, %d)\n", wid, x, y);
    dll_callbacks.callback_curs((int)wid, x, y);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)
        && (GetNHApp()->windowlist[wid].win != NULL)) {
        MSNHMsgCursor data;
        data.x = x;
        data.y = y;
        SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CURSOR, (LPARAM) &data);
    }
#endif
}

/*
putstr(window, attr, str)
                -- Print str on the window with the given attribute.  Only
                   printable ASCII characters (040-0126) must be supported.
                   Multiple putstr()s are output on separate lines.
Attributes
                   can be one of
                        ATR_NONE (or 0)
                        ATR_ULINE
                        ATR_BOLD
                        ATR_BLINK
                        ATR_INVERSE
                   If a window-port does not support all of these, it may map
                   unsupported attributes to a supported one (e.g. map them
                   all to ATR_INVERSE).  putstr() may compress spaces out of
                   str, break str, or truncate str, if necessary for the
                   display.  Where putstr() breaks a line, it has to clear
                   to end-of-line.
                -- putstr should be implemented such that if two putstr()s
                   are done consecutively the user will see the first and
                   then the second.  In the tty port, pline() achieves this
                   by calling more() or displaying both on the same line.
*/
void
dll_putstr(winid wid, int attr, const char *text)
{
    dll_logDebug("dll_putstr(%d, %d, %s)\n", wid, attr, text);

    dll_putstr_ex(wid, attr, text, 0, NO_COLOR);
}

void
dll_putstr_ex(winid wid, int attr, const char *text, int app, int color)
{
    dll_callbacks.callback_putstr_ex((int)wid, attr, text, app, color);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)) {
        if (GetNHApp()->windowlist[wid].win == NULL
            && GetNHApp()->windowlist[wid].type == NHW_MENU) {
            GetNHApp()->windowlist[wid].win =
                mswin_init_menu_window(MENU_TYPE_TEXT);
            GetNHApp()->windowlist[wid].dead = 0;
        }

        if (GetNHApp()->windowlist[wid].win != NULL) {
            MSNHMsgPutstr data;
            ZeroMemory(&data, sizeof(data));
            data.attr = attr;
            data.text = text;
            data.append = app;
            SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                        (WPARAM) MSNH_MSG_PUTSTR, (LPARAM) &data);
        }
        /* yield a bit so it gets done immediately */
        dll_get_nh_event();
    } else {
        // build text to display later in message box
        GetNHApp()->saved_text =
            realloc(GetNHApp()->saved_text,
                    strlen(text) + strlen(GetNHApp()->saved_text) + 1);
        strcat(GetNHApp()->saved_text, text);
    }
#endif
}

void
dll_putstr_ex2(winid wid, const char* text, const char* attrs, const char* colors, int attr, int color, int app)
{
    dll_callbacks.callback_putstr_ex2((int)wid, text, attrs, colors, attr, color, app);
}

/* Display the file named str.  Complain about missing files
                   iff complain is TRUE.
*/
void
dll_display_file(const char *filename, BOOLEAN_P must_exist)
{
    dlb *f;
    char wbuf[BUFSZ];

    dll_logDebug("dll_display_file(%s, %d)\n", filename, must_exist);

    f = dlb_fopen(filename, RDTMODE);
    if (!f) 
    {
        if (must_exist) 
        {
            char message[90];
            _stprintf(message, TEXT("Warning! Could not find file: %s\n"),
                      GNH_A2W(filename, wbuf, sizeof(wbuf)));
            GNHMessageBox(message,
                         MB_OK | MB_ICONEXCLAMATION);
        }
    } 
    else 
    {
        winid text;
        char line[LLEN];

        text = dll_create_nhwindow_ex(NHW_TEXT, 0, NO_GLYPH, zerocreatewindowinfo);

        while (dlb_fgets(line, LLEN, f)) {
            size_t len;
            len = strlen(line);
            if (line[len - 1] == '\n')
                line[len - 1] = '\x0';
            dll_putstr(text, ATR_NONE, line);
        }
        (void) dlb_fclose(f);

        dll_display_nhwindow(text, 1);
        dll_destroy_nhwindow(text);
    }
}

/* Start using window as a menu.  You must call start_menu()
   before add_menu().  After calling start_menu() you may not
   putstr() to the window.  Only windows of type NHW_MENU may
   be used for menus.
*/
void
dll_start_menu_ex(winid wid, int style)
{
    dll_logDebug("dll_start_menu_ex(%d)\n", wid);
    dll_callbacks.callback_start_menu_ex((int)wid, style);
#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)) {
        if (GetNHApp()->windowlist[wid].win == NULL
            && GetNHApp()->windowlist[wid].type == NHW_MENU) {
            GetNHApp()->windowlist[wid].win =
                mswin_init_menu_window(MENU_TYPE_MENU);
            GetNHApp()->windowlist[wid].dead = 0;
        }

        if (GetNHApp()->windowlist[wid].win != NULL) {
            SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                        (WPARAM) MSNH_MSG_STARTMENU, (LPARAM) NULL);
        }
    }
#endif
}

/*
add_menu(windid window, int glyph, const anything identifier,
                                char accelerator, char groupacc,
                                int attr, char *str, boolean preselected)
                -- Add a text line str to the given menu window.  If
identifier
                   is 0, then the line cannot be selected (e.g. a title).
                   Otherwise, identifier is the value returned if the line is
                   selected.  Accelerator is a keyboard key that can be used
                   to select the line.  If the accelerator of a selectable
                   item is 0, the window system is free to select its own
                   accelerator.  It is up to the window-port to make the
                   accelerator visible to the user (e.g. put "a - " in front
                   of str).  The value attr is the same as in putstr().
                   Glyph is an optional glyph to accompany the line.  If
                   window port cannot or does not want to display it, this
                   is OK.  If there is no glyph applicable, then this
                   value will be NO_GLYPH.
                -- All accelerators should be in the range [A-Za-z].
                -- It is expected that callers do not mix accelerator
                   choices.  Either all selectable items have an accelerator
                   or let the window system pick them.  Don't do both.
                -- Groupacc is a group accelerator.  It may be any character
                   outside of the standard accelerator (see above) or a
                   number.  If 0, the item is unaffected by any group
                   accelerator.  If this accelerator conflicts with
                   the menu command (or their user defined aliases), it loses.
                   The menu commands and aliases take care not to interfere
                   with the default object class symbols.
                -- If you want this choice to be preselected when the
                   menu is displayed, set preselected to TRUE.
*/
void
dll_add_extended_menu(winid wid, int glyph, const ANY_P *identifier, struct extended_menu_info info,
               CHAR_P accelerator, CHAR_P group_accel, int attr,
               const char *str, BOOLEAN_P presel)
{
    dll_logDebug("dll_add_menu(%d, %d, %p, %c, %c, %d, %s, %d)\n", wid, glyph,
             identifier, (char) accelerator, (char) group_accel, attr, str,
             presel);
     
    int color = CLR_WHITE;
#ifdef TEXTCOLOR
    get_menu_coloring(str, &color, &attr);
#endif

    struct objclassdata ocdata = get_objclassdata(info.object);
    dll_callbacks.callback_add_extended_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr,
        str, presel, color, info.object ? info.object->quan : 0, info.object ? info.object->o_id : 0, 
        info.monster ? info.monster->m_id : 0, info.heading_for_group_accelerator, info.special_mark, info.menu_flags, 1, info.style, 
        info.object, &ocdata);
}

void
dll_add_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel)
{
    dll_add_extended_menu(wid, glyph, identifier, zeroextendedmenuinfo,
        accelerator, group_accel, attr,
        str, presel);
}

/*
end_menu(window, prompt)
                -- Stop adding entries to the menu and flushes the window
                   to the screen (brings to front?).  Prompt is a prompt
                   to give the user.  If prompt is NULL, no prompt will
                   be printed.
                ** This probably shouldn't flush the window any more (if
                ** it ever did).  That should be select_menu's job.  -dean
*/
void
dll_end_menu_ex(winid wid, const char *prompt, const char* subtitle)
{
    dll_logDebug("dll_end_menu_ex(%d, %s)\n", wid, prompt);
    dll_callbacks.callback_end_menu_ex(wid, prompt, subtitle);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)
        && (GetNHApp()->windowlist[wid].win != NULL)) {
        MSNHMsgEndMenu data;
        ZeroMemory(&data, sizeof(data));
        data.text = prompt;

        SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_ENDMENU, (LPARAM) &data);
    }
#endif
}

/*
int select_menu(windid window, int how, menu_item **selected)
                -- Return the number of items selected; 0 if none were chosen,
                   -1 when explicitly cancelled.  If items were selected, then
                   selected is filled in with an allocated array of menu_item
                   structures, one for each selected line.  The caller must
                   free this array when done with it.  The "count" field
                   of selected is a user supplied count.  If the user did
                   not supply a count, then the count field is filled with
                   -1 (meaning all).  A count of zero is equivalent to not
                   being selected and should not be in the list.  If no items
                   were selected, then selected is NULL'ed out.  How is the
                   mode of the menu.  Three valid values are PICK_NONE,
                   PICK_ONE, and PICK_N, meaning: nothing is selectable,
                   only one thing is selectable, and any number valid items
                   may selected.  If how is PICK_NONE, this function should
                   never return anything but 0 or -1.
                -- You may call select_menu() on a window multiple times --
                   the menu is saved until start_menu() or destroy_nhwindow()
                   is called on the window.
                -- Note that NHW_MENU windows need not have select_menu()
                   called for them. There is no way of knowing whether
                   select_menu() will be called for the window at
                   create_nhwindow() time.
*/
int
dll_select_menu(winid wid, int how, MENU_ITEM_P **selected)
{
    int nReturned = -1;

    dll_logDebug("dll_select_menu(%d, %d)\n", wid, how);

    long long* picklist;
    int picklistsize;

    nReturned = dll_callbacks.callback_select_menu((int)wid, how, &picklist, &picklistsize);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)
        && (GetNHApp()->windowlist[wid].win != NULL)) {
        ShowWindow(GetNHApp()->windowlist[wid].win, SW_SHOW);
        nReturned = mswin_menu_window_select_menu(
            GetNHApp()->windowlist[wid].win, how, selected,
            !(iflags.perm_invent && wid == WIN_INVEN
              && how == PICK_NONE) /* don't activate inventory window if
                                      perm_invent is on */
            );
    }
#endif

    return nReturned;
}

/*
    -- Indicate to the window port that the inventory has been changed.
    -- Merely calls display_inventory() for window-ports that leave the
        window up, otherwise empty.
*/
void
dll_update_inventory()
{
    dll_logDebug("dll_update_inventory()\n");
    if (iflags.perm_invent && program_state.something_worth_saving
        && iflags.window_inited && WIN_INVEN != WIN_ERR)
        display_inventory(NULL, FALSE, 0);
}

/*
mark_synch()    -- Don't go beyond this point in I/O on any channel until
                   all channels are caught up to here.  Can be an empty call
                   for the moment
*/
void
dll_mark_synch()
{
    dll_logDebug("dll_mark_synch()\n");
}

/*
wait_synch()    -- Wait until all pending output is complete (*flush*() for
                   streams goes here).
                -- May also deal with exposure events etc. so that the
                   display is OK when return from wait_synch().
*/
void
dll_wait_synch()
{
    dll_logDebug("dll_wait_synch()\n");
    dll_raw_print_flush();
}

/*
cliparound(x, y)-- Make sure that the user is more-or-less centered on the
                   screen if the playing area is larger than the screen.
                -- This function is only defined if CLIPPING is defined.
*/
void
dll_cliparound(int x, int y, BOOLEAN_P force)
{
    winid wid = WIN_MAP;

    dll_logDebug("dll_cliparound(%d, %d)\n", x, y);
    dll_callbacks.callback_cliparound(x, y, force);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)
        && (GetNHApp()->windowlist[wid].win != NULL)) {
        MSNHMsgClipAround data;
        data.x = x;
        data.y = y;
        SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CLIPAROUND, (LPARAM) &data);
    }
#endif
}

void
dll_issue_gui_command(int initid)
{
    return;
}

/*
print_glyph(window, x, y, layers)
                -- Print the glyph at (x,y) on the given window.  Glyphs are
                   integers at the interface, mapped to whatever the window-
                   port wants (symbol, font, color, attributes, ...there's
                   a 1-1 map between glyphs and distinct things on the map).
        -- bkglyph is a background glyph for potential use by some
           graphical or tiled environments to allow the depiction
           to fall against a background consistent with the grid 
           around x,y.
                   
*/
void
dll_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers)
{
    int glyph = layers.glyph;
    int bkglyph = layers.bkglyph;

    dll_logDebug("dll_print_glyph(%d, %d, %d, %d, %d)\n", wid, x, y, glyph, bkglyph);
    long symbol;
    nhsym sym = 0;
    int ocolor = 0;
    unsigned long special = 0UL;
    (void)mapglyph(layers, &sym, &ocolor, &special, 0, 0);
    symbol = (long)sym;

    dll_callbacks.callback_print_glyph(wid, (int)x, (int)y, glyph, bkglyph, symbol, ocolor, special, &layers);

#if 0
    if ((wid >= 0) && (wid < MAXWINDOWS)
        && (GetNHApp()->windowlist[wid].win != NULL)) {
        MSNHMsgPrintGlyph data;

        ZeroMemory(&data, sizeof(data));
        data.x = x;
        data.y = y;
        data.layers = layers;
        SendMessage(GetNHApp()->windowlist[wid].win, WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_PRINT_GLYPH, (LPARAM) &data);
    }
#endif
}

/*
 * dll_raw_print_accumulate() accumulate the given text into
 *   raw_print_strbuf.
 */
void
dll_raw_print_accumulate(const char * str, boolean bold)
{
    bold; // ignored for now

    if (dll_raw_print_strbuf.str != NULL)
        strbuf_append(&dll_raw_print_strbuf, "\n");

    strbuf_append(&dll_raw_print_strbuf, str);

}

/*
 * dll_raw_print_flush() - display any text found in raw_print_strbuf in a
 *   dialog box and clear raw_print_strbuf.
 */
void
dll_raw_print_flush()
{
    if (dll_raw_print_strbuf.str != NULL) 
    {
        size_t wlen = strlen(dll_raw_print_strbuf.str) + 1;
        TCHAR * wbuf = (TCHAR *) alloc(wlen * sizeof(TCHAR));
        if (wbuf != NULL)
        {
            GNHMessageBox(GNH_A2W(dll_raw_print_strbuf.str, wbuf, wlen),
                            MB_ICONINFORMATION | MB_OK);
            free(wbuf);
        }
        strbuf_empty(&dll_raw_print_strbuf);
    }
}


/*
raw_print(str)  -- Print directly to a screen, or otherwise guarantee that
                   the user sees str.  raw_print() appends a newline to str.
                   It need not recognize ASCII control characters.  This is
                   used during startup (before windowing system initialization
                   -- maybe this means only error startup messages are raw),
                   for error messages, and maybe other "msg" uses.  E.g.
                   updating status for micros (i.e, "saving").
*/
void
dll_raw_print(const char *str)
{
    dll_logDebug("dll_raw_print(%s)\n", str);

    if (str && *str)
    {
        extern int redirect_stdout;
        if (!redirect_stdout)
            dll_raw_print_accumulate(str, FALSE);
        else
            dll_callbacks.callback_raw_print(str); //fprintf(stdout, "%s", str);
    }
}

/*
raw_print_bold(str)
                -- Like raw_print(), but prints in bold/standout (if
possible).
*/
void
dll_raw_print_bold(const char *str)
{
    dll_logDebug("dll_raw_print_bold(%s)\n", str);
    if (str && *str) 
    {
        extern int redirect_stdout;
        if (!redirect_stdout)
            dll_raw_print_accumulate(str, TRUE);
        else
            dll_callbacks.callback_raw_print_bold(str); //fprintf(stdout, "%s", str);
    }
}

/*
int nhgetch()   -- Returns a single character input from the user.
                -- In the tty window-port, nhgetch() assumes that tgetch()
                   will be the routine the OS provides to read a character.
                   Returned character _must_ be non-zero.
*/
int
dll_nhgetch()
{
    return dll_callbacks.callback_nhgetch();

#if 0
    PMSNHEvent event;
    int key = 0;

    dll_logDebug("dll_nhgetch()\n");

    while ((event = mswin_input_pop()) == NULL || event->type != NHEVENT_CHAR)
        dll_main_loop();

    key = event->kbd.ch;
    return (key);
#endif
}

/*
int nh_poskey(int *x, int *y, int *mod)
                -- Returns a single character input from the user or a
                   a positioning event (perhaps from a mouse).  If the
                   return value is non-zero, a character was typed, else,
                   a position in the MAP window is returned in x, y and mod.
                   mod may be one of

                        CLICK_1         -- mouse click type 1
                        CLICK_2         -- mouse click type 2

                   The different click types can map to whatever the
                   hardware supports.  If no mouse is supported, this
                   routine always returns a non-zero character.
*/
int
dll_nh_poskey(int *x, int *y, int *mod)
{
    return dll_callbacks.callback_nh_poskey(x, y, mod);

#if 0
    PMSNHEvent event;
    int key;

    dll_logDebug("dll_nh_poskey()\n");

    while ((event = mswin_input_pop()) == NULL)
        dll_main_loop();

    if (event->type == NHEVENT_MOUSE) {
    if (iflags.wc_mouse_support) {
            *mod = event->ms.mod;
            *x = event->ms.x;
            *y = event->ms.y;
        }
        key = 0;
    } else {
        key = event->kbd.ch;
    }
    return (key);
#endif
}

/*
nhbell()        -- Beep at user.  [This will exist at least until sounds are
                   redone, since sounds aren't attributable to windows
anyway.]
*/
void
dll_nhbell()
{
    dll_logDebug("dll_nhbell()\n");
    dll_callbacks.callback_nhbell();
}

/*
doprev_message()
                -- Display previous messages.  Used by the ^P command.
                -- On the tty-port this scrolls WIN_MESSAGE back one line.
*/
int
dll_doprev_message()
{
    dll_logDebug("dll_doprev_message()\n");
    return dll_callbacks.callback_doprev_message();
#if 0
    SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_VSCROLL,
                MAKEWPARAM(SB_LINEUP, 0), (LPARAM) NULL);
    return 0;
#endif
}

/*
char yn_function(const char *ques, const char *choices, char default)
                -- Print a prompt made up of ques, choices and default.
                   Read a single character response that is contained in
                   choices or default.  If choices is NULL, all possible
                   inputs are accepted and returned.  This overrides
                   everything else.  The choices are expected to be in
                   lower case.  Entering ESC always maps to 'q', or 'n',
                   in that order, if present in choices, otherwise it maps
                   to default.  Entering any other quit character (SPACE,
                   RETURN, NEWLINE) maps to default.
                -- If the choices string contains ESC, then anything after
                   it is an acceptable response, but the ESC and whatever
                   follows is not included in the prompt.
                -- If the choices string contains a '#' then accept a count.
                   Place this value in the global "yn_number" and return '#'.
                -- This uses the top line in the tty window-port, other
                   ports might use a popup.
*/
char
dll_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char *question, const char *choices, CHAR_P def, const char* resp_desc, const char* introline, unsigned long ynflags)
{
    dll_logDebug("dll_yn_function_ex(%s, %s, %d)\n", question, choices, def);
    char defs[2] = { 0, 0 };
    defs[0] = def;
    return dll_callbacks.callback_yn_function_ex(style, attr, color, glyph, title, question, choices, defs, resp_desc, introline, ynflags);

#if 0
    char ch;
    char yn_esc_map = '\033';
    char message[BUFSZ + QBUFSZ];
    char res_ch[2];
    int createcaret;
    boolean digit_ok, allow_num;

    dll_logDebug("dll_yn_function_ex(%s, %s, %d)\n", question, choices, def);

    if (WIN_MESSAGE == WIN_ERR && choices == ynchars) {
        char *text =
            realloc(strdup(GetNHApp()->saved_text),
                    strlen(question) + strlen(GetNHApp()->saved_text) + 1);
        DWORD box_result;
        strcat(text, question);
        box_result =
            GNHMessageBox(GNH_W2A(text, message, sizeof(message)),
                         MB_ICONQUESTION | MB_YESNOCANCEL
                             | ((def == 'y') ? MB_DEFBUTTON1
                                             : (def == 'n') ? MB_DEFBUTTON2
                                                            : MB_DEFBUTTON3));
        free(text);
        GetNHApp()->saved_text = strdup("");
        return box_result == IDYES ? 'y' : box_result == IDNO ? 'n' : '\033';
    }

    if (choices) {
        char *cb, choicebuf[QBUFSZ];

        allow_num = (index(choices, '#') != 0);

        Strcpy(choicebuf, choices);
        if ((cb = index(choicebuf, '\033')) != 0) {
            /* anything beyond <esc> is hidden */
            *cb = '\0';
        }
        (void) strncpy(message, question, QBUFSZ - 1);
        message[QBUFSZ - 1] = '\0';
        sprintf(eos(message), " [%s]", choicebuf);
        if (def)
            sprintf(eos(message), " (%c)", def);
        Strcat(message, " ");
        /* escape maps to 'q' or 'n' or default, in that order */
        yn_esc_map =
            (index(choices, 'q') ? 'q' : (index(choices, 'n') ? 'n' : def));
    } else {
        Strcpy(message, question);
        Strcat(message, " ");
    }

    createcaret = 1;
    SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                (WPARAM) MSNH_MSG_CARET, (LPARAM) &createcaret);

    dll_clear_nhwindow(WIN_MESSAGE);
    dll_putstr(WIN_MESSAGE, ATR_BOLD, message);

    /* Only here if main window is not present */
    ch = 0;
    do {
        ShowCaret(dll_hwnd_from_winid(WIN_MESSAGE));
        ch = dll_nhgetch();
        HideCaret(dll_hwnd_from_winid(WIN_MESSAGE));
        if (choices)
            ch = lowc(ch);
        else
            break; /* If choices is NULL, all possible inputs are accepted and
                      returned. */

        digit_ok = allow_num && digit(ch);
        if (ch == '\033') {
            if (index(choices, 'q'))
                ch = 'q';
            else if (index(choices, 'n'))
                ch = 'n';
            else
                ch = def;
            break;
        } else if (index(quitchars, ch)) {
            ch = def;
            break;
        } else if (!index(choices, ch) && !digit_ok) {
            dll_nhbell();
            ch = (char) 0;
            /* and try again... */
        } else if (ch == '#' || digit_ok) {
            char z, digit_string[2];
            int n_len = 0;
            long value = 0;
            dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, ("#"), 1, NO_COLOR);
            n_len++;
            digit_string[1] = '\0';
            if (ch != '#') {
                digit_string[0] = ch;
                dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, digit_string, 1, NO_COLOR);
                n_len++;
                value = ch - '0';
                ch = '#';
            }
            do { /* loop until we get a non-digit */
                z = lowc(readchar());
                if (digit(z)) {
                    value = (10 * value) + (z - '0');
                    if (value < 0)
                        break; /* overflow: try again */
                    digit_string[0] = z;
                    dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, digit_string, 1, NO_COLOR);
                    n_len++;
                } else if (z == 'y' || index(quitchars, z)) {
                    if (z == '\033')
                        value = -1; /* abort */
                    z = '\n';       /* break */
                } else if (z == '\b') {
                    if (n_len <= 1) {
                        value = -1;
                        break;
                    } else {
                        value /= 10;
                        dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, digit_string,
                                        -1, NO_COLOR);
                        n_len--;
                    }
                } else {
                    value = -1; /* abort */
                    dll_nhbell();
                    break;
                }
            } while (z != '\n');
            if (value > 0)
                yn_number = value;
            else if (value == 0)
                ch = 'n'; /* 0 => "no" */
            else {        /* remove number from top line, then try again */
                dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, digit_string, -n_len, NO_COLOR);
                n_len = 0;
                ch = (char) 0;
            }
        }
    } while (!ch);

    createcaret = 0;
    SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                (WPARAM) MSNH_MSG_CARET, (LPARAM) &createcaret);

    /* display selection in the message window */
    if (isprint((uchar) ch) && ch != '#') {
        res_ch[0] = ch;
        res_ch[1] = '\x0';
        dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, res_ch, 1, NO_COLOR);
    }

    return ch;
#endif
}

/*
getlin(const char *ques, char *input)
            -- Prints ques as a prompt and reads a single line of text,
               up to a newline.  The string entered is returned without the
               newline.  ESC is used to cancel, in which case the string
               "\033\000" is returned.
            -- getlin() must call flush_screen(1) before doing anything.
            -- This uses the top line in the tty window-port, other
               ports might use a popup.
*/
void
dll_getlin_ex(int style, int attr, int color, const char *question, char *input, const char* placeholder, const char* linesuffix, const char* introline)
{
    dll_logDebug("dll_getlin(%s, %p)\n", question, input);
    char* res = dll_callbacks.callback_getlin_ex(style, attr, color, question, placeholder, linesuffix, introline);
    if (res && input)
        strcpy(input, res);

#if 0
    if (!iflags.wc_popup_dialog) {
        char c;
        int len;
        int done;
        int createcaret;

        createcaret = 1;
        SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CARET, (LPARAM) &createcaret);

        /* dll_clear_nhwindow(WIN_MESSAGE); */
        dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, question, 0, NO_COLOR);
        dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, " ", 1, NO_COLOR, NO_COLOR);
#ifdef EDIT_GETLIN
        dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, input, 0, NO_COLOR);
        len = strlen(input);
#else
        input[0] = '\0';
        len = 0;
#endif
        ShowCaret(dll_hwnd_from_winid(WIN_MESSAGE));
        done = FALSE;
        while (!done) {
            c = dll_nhgetch();
            switch (c) {
            case VK_ESCAPE:
                strcpy(input, "\033");
                done = TRUE;
                break;
            case '\n':
            case '\r':
            case -115:
                done = TRUE;
                break;
            default:
                if (input[0])
                    dll_putstr_ex(WIN_MESSAGE, ATR_NONE, input, -len, NO_COLOR);
                if (c == VK_BACK) {
                    if (len > 0)
                        len--;
                    input[len] = '\0';
                } else if (len>=(BUFSZ-1)) {
                    PlaySound((LPCSTR)SND_ALIAS_SYSTEMEXCLAMATION, NULL, SND_ALIAS_ID|SND_ASYNC);
                } else {
                    input[len++] = c;
                    input[len] = '\0';
                }
                dll_putstr_ex(WIN_MESSAGE, ATR_NONE, input, 1, NO_COLOR);
                break;
            }
        }
        HideCaret(dll_hwnd_from_winid(WIN_MESSAGE));
        createcaret = 0;
        SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CARET, (LPARAM) &createcaret);
    } else {
        if (mswin_getlin_window(GETLINE_GENERAL, question, input, BUFSZ) == IDCANCEL) {
            strcpy(input, "\033");
        }
    }
#endif
}

/*
int get_ext_cmd(void)
            -- Get an extended command in a window-port specific way.
               An index into extcmdlist[] is returned on a successful
               selection, -1 otherwise.
*/
int
dll_get_ext_cmd()
{
    dll_logDebug("dll_get_ext_cmd()\n");
    return dll_callbacks.callback_get_ext_cmd();

#if 0
    int ret;
    if (!iflags.wc_popup_dialog) {
        char c;
        char cmd[BUFSZ];
        int i, len;
        int createcaret;

        createcaret = 1;
        SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CARET, (LPARAM) &createcaret);

        cmd[0] = '\0';
        i = -2;
        dll_clear_nhwindow(WIN_MESSAGE);
        dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, "#", 0, NO_COLOR);
        len = 0;
        ShowCaret(dll_hwnd_from_winid(WIN_MESSAGE));
        while (i == -2) {
            int oindex, com_index;
            c = dll_nhgetch();
            switch (c) {
            case VK_ESCAPE:
                i = -1;
                break;
            case '\n':
            case '\r':
            case -115:
                for (i = 0; extcmdlist[i].ef_txt != (char *) 0; i++)
                    if (!strcmpi(cmd, extcmdlist[i].ef_txt))
                        break;

                if (extcmdlist[i].ef_txt == (char *) 0) {
                    pline("%s: unknown extended command.", cmd);
                    i = -1;
                }
                break;
            default:
                if (cmd[0])
                    dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, cmd,
                                    -(int) strlen(cmd), NO_COLOR);
                if (c == VK_BACK) {
                    if (len > 0)
                        len--;
                    cmd[len] = '\0';
                } else {
                    cmd[len++] = c;
                    cmd[len] = '\0';
                    /* Find a command with this prefix in extcmdlist */
                    com_index = -1;
                    for (oindex = 0; extcmdlist[oindex].ef_txt != (char *) 0;
                         oindex++) {
                        if ((extcmdlist[oindex].flags & AUTOCOMPLETE)
                            && !(!wizard && (extcmdlist[oindex].flags & WIZMODECMD))
                            && !(!wizard && !discover && !CasualMode && (extcmdlist[oindex].flags & CASUALMODECMD))
                            && !strncmpi(cmd, extcmdlist[oindex].ef_txt, len)) {
                            if (com_index == -1) /* no matches yet */
                                com_index = oindex;
                            else
                                com_index =
                                    -2; /* two matches, don't complete */
                        }
                    }
                    if (com_index >= 0) {
                        Strcpy(cmd, extcmdlist[com_index].ef_txt);
                    }
                }
                dll_putstr_ex(WIN_MESSAGE, ATR_BOLD, cmd, 1, NO_COLOR);
                break;
            }
        }
        HideCaret(dll_hwnd_from_winid(WIN_MESSAGE));
        createcaret = 0;
        SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_CARET, (LPARAM) &createcaret);
        return i;
    } else {
        if (mswin_ext_cmd_window(&ret) == IDCANCEL)
            return -1;
        else
            return ret;
    }
#endif
}

/*
number_pad(state)
            -- Initialize the number pad to the given state.
*/
void
dll_number_pad(int state)
{
    /* Do Nothing */
    dll_logDebug("dll_number_pad(%d)\n", state);
    dll_callbacks.callback_number_pad(state);
}

/*
delay_output()  -- Causes a visible delay of 50ms in the output.
               Conceptually, this is similar to wait_synch() followed
               by a nap(50ms), but allows asynchronous operation.
*/
void
dll_delay_output()
{
    dll_logDebug("dll_delay_output()\n");
    dll_callbacks.callback_delay_output();
}

void
dll_delay_output_milliseconds(int interval)
{
    dll_logDebug("dll_delay_output_milliseconds()\n");
    dll_callbacks.callback_delay_output_milliseconds(interval);
}

void
dll_delay_output_intervals(int intervals)
{
    dll_logDebug("dll_delay_output_intervals()\n");
    dll_callbacks.callback_delay_output_intervals(intervals);
}


void
dll_change_color()
{
    dll_logDebug("dll_change_color()\n");
    dll_callbacks.callback_change_color();
}

char *
dll_get_color_string()
{
    dll_logDebug("dll_get_color_string()\n");
    return dll_callbacks.callback_get_color_string();
}

/*
start_screen()  -- Only used on Unix tty ports, but must be declared for
               completeness.  Sets up the tty to work in full-screen
               graphics mode.  Look at win/tty/termcap.c for an
               example.  If your window-port does not need this function
               just declare an empty function.
*/
void
dll_start_screen()
{
    /* Do Nothing */
    dll_logDebug("dll_start_screen()\n");
    dll_callbacks.callback_start_screen();
}

/*
end_screen()    -- Only used on Unix tty ports, but must be declared for
               completeness.  The complement of start_screen().
*/
void
dll_end_screen()
{
    /* Do Nothing */
    dll_logDebug("dll_end_screen()\n");
    dll_callbacks.callback_end_screen();
}

/*
outrip(winid, int, when)
            -- The tombstone code.  If you want the traditional code use
               genl_outrip for the value and check the #if in rip.c.
*/
#define STONE_LINE_LEN 16
void
dll_outrip(winid wid, int how, time_t when)
{
    char buf[BUFSZ];
    long year;

    dll_logDebug("dll_outrip(%d, %d, %ld)\n", wid, how, (long) when);
    //dll_callbacks.callback_outrip_begin(wid); //Begin

    /* Put name on stone */
    Sprintf(buf, "%s", plname);
    buf[STONE_LINE_LEN] = 0;
    putstr(wid, 0, buf);

    /* Put $ on stone */
    Sprintf(buf, "%ld Au", done_money);
    buf[STONE_LINE_LEN] = 0; /* It could be a *lot* of gold :-) */
    putstr(wid, 0, buf);

    /* Put together death description */
    formatkiller(buf, sizeof buf, how, FALSE);

    /* Put death type on stone */
    putstr(wid, 0, buf);

    /* Put year on stone */
    year = yyyymmdd(when) / 10000L;
    Sprintf(buf, "%4ld", year);
    putstr(wid, 0, buf);

    //dll_callbacks.callback_outrip_end(wid);  //End
}

/* handle options updates here */
void
dll_preference_update(const char *pref)
{
#if 0
    if (stricmp(pref, "font_menu") == 0
        || stricmp(pref, "font_size_menu") == 0) 
    {
        if (iflags.wc_fontsiz_menu == 0)
            iflags.wc_fontsiz_menu = NHFONT_DEFAULT_SIZE;
        else if (iflags.wc_fontsiz_menu < NHFONT_SIZE_MIN)
            iflags.wc_fontsiz_menu = NHFONT_SIZE_MIN;
        else if (iflags.wc_fontsiz_menu > NHFONT_SIZE_MAX)
            iflags.wc_fontsiz_menu = NHFONT_SIZE_MAX;

        HDC hdc = GetDC(GetNHApp()->hMainWnd);
        mswin_get_font(NHW_MENU, ATR_NONE, hdc, TRUE);
        mswin_get_font(NHW_MENU, ATR_BOLD, hdc, TRUE);
        mswin_get_font(NHW_MENU, ATR_DIM, hdc, TRUE);
        mswin_get_font(NHW_MENU, ATR_ULINE, hdc, TRUE);
        mswin_get_font(NHW_MENU, ATR_BLINK, hdc, TRUE);
        mswin_get_font(NHW_MENU, ATR_INVERSE, hdc, TRUE);
        ReleaseDC(GetNHApp()->hMainWnd, hdc);

        mswin_layout_main_window(NULL);
        return;
    }

    if (stricmp(pref, "font_status") == 0
        || stricmp(pref, "font_size_status") == 0) 
    {
        if (iflags.wc_fontsiz_status == 0)
            iflags.wc_fontsiz_status = NHFONT_DEFAULT_SIZE;
        else if (iflags.wc_fontsiz_status < NHFONT_SIZE_MIN)
            iflags.wc_fontsiz_status = NHFONT_SIZE_MIN;
        else if (iflags.wc_fontsiz_menu > NHFONT_SIZE_MAX)
            iflags.wc_fontsiz_status = NHFONT_SIZE_MAX;

        HDC hdc = GetDC(GetNHApp()->hMainWnd);
        mswin_get_font(NHW_STATUS, ATR_NONE, hdc, TRUE);
        mswin_get_font(NHW_STATUS, ATR_BOLD, hdc, TRUE);
        mswin_get_font(NHW_STATUS, ATR_DIM, hdc, TRUE);
        mswin_get_font(NHW_STATUS, ATR_ULINE, hdc, TRUE);
        mswin_get_font(NHW_STATUS, ATR_BLINK, hdc, TRUE);
        mswin_get_font(NHW_STATUS, ATR_INVERSE, hdc, TRUE);
        ReleaseDC(GetNHApp()->hMainWnd, hdc);

        for (int i = 1; i < MAXWINDOWS; i++) {
            if (GetNHApp()->windowlist[i].type == NHW_STATUS
                && GetNHApp()->windowlist[i].win != NULL) {
                InvalidateRect(GetNHApp()->windowlist[i].win, NULL, TRUE);
            }
        }
        mswin_layout_main_window(NULL);
        return;
    }

    if (stricmp(pref, "font_message") == 0
        || stricmp(pref, "font_size_message") == 0)
    {
        if (iflags.wc_fontsiz_message == 0)
            iflags.wc_fontsiz_message = NHFONT_DEFAULT_SIZE;
        else if (iflags.wc_fontsiz_message < NHFONT_SIZE_MIN)
            iflags.wc_fontsiz_message = NHFONT_SIZE_MIN;
        else if (iflags.wc_fontsiz_message > NHFONT_SIZE_MAX)
            iflags.wc_fontsiz_message = NHFONT_SIZE_MAX;

        HDC hdc = GetDC(GetNHApp()->hMainWnd);
        mswin_get_font(NHW_MESSAGE, ATR_NONE, hdc, TRUE);
        mswin_get_font(NHW_MESSAGE, ATR_BOLD, hdc, TRUE);
        mswin_get_font(NHW_MESSAGE, ATR_DIM, hdc, TRUE);
        mswin_get_font(NHW_MESSAGE, ATR_ULINE, hdc, TRUE);
        mswin_get_font(NHW_MESSAGE, ATR_BLINK, hdc, TRUE);
        mswin_get_font(NHW_MESSAGE, ATR_INVERSE, hdc, TRUE);
        ReleaseDC(GetNHApp()->hMainWnd, hdc);

        InvalidateRect(dll_hwnd_from_winid(WIN_MESSAGE), NULL, TRUE);
        mswin_layout_main_window(NULL);
        return;
    }

    if (stricmp(pref, "font_text") == 0
        || stricmp(pref, "font_size_text") == 0) 
    {
        if (iflags.wc_fontsiz_text == 0)
            iflags.wc_fontsiz_text = NHFONT_DEFAULT_SIZE;
        else if (iflags.wc_fontsiz_text < NHFONT_SIZE_MIN)
            iflags.wc_fontsiz_text = NHFONT_SIZE_MIN;
        else if (iflags.wc_fontsiz_text > NHFONT_SIZE_MAX)
            iflags.wc_fontsiz_text = NHFONT_SIZE_MAX;

        HDC hdc = GetDC(GetNHApp()->hMainWnd);
        mswin_get_font(NHW_TEXT, ATR_NONE, hdc, TRUE);
        mswin_get_font(NHW_TEXT, ATR_BOLD, hdc, TRUE);
        mswin_get_font(NHW_TEXT, ATR_DIM, hdc, TRUE);
        mswin_get_font(NHW_TEXT, ATR_ULINE, hdc, TRUE);
        mswin_get_font(NHW_TEXT, ATR_BLINK, hdc, TRUE);
        mswin_get_font(NHW_TEXT, ATR_INVERSE, hdc, TRUE);
        ReleaseDC(GetNHApp()->hMainWnd, hdc);

        mswin_layout_main_window(NULL);
        return;
    }
#endif

    if (stricmp(pref, "scroll_amount") == 0) {
        dll_cliparound(u.ux, u.uy, TRUE);
        return;
    }

    if (stricmp(pref, "scroll_margin") == 0) {
        dll_cliparound(u.ux, u.uy, TRUE);
        return;
    }

    if (stricmp(pref, "perm_invent") == 0) {
        dll_update_inventory();
        return;
    }

    if (stricmp(pref, "preferred_screen_scale") == 0) {
        dozoomnormal();
        dll_stretch_window();
        return;
    }

    /* Otherwise do callback */
    dll_callbacks.callback_preference_update(pref);
}

char *
dll_getmsghistory_ex(char** attrs_ptr, char** colors_ptr, BOOLEAN_P init)
{
    if (attrs_ptr)
        *attrs_ptr = (char*)0;
    if (colors_ptr)
        *colors_ptr = (char*)0;

    return (char*)0; // dll_callbacks.callback_getmsghistory(init);

#if 0
    static PMSNHMsgGetText text = 0;
    static char *next_message = 0;

    if (init) {
        text = (PMSNHMsgGetText) malloc(sizeof(MSNHMsgGetText)
                                        + TEXT_BUFFER_SIZE);

        if (!text)
            return (char*)0;

        text->max_size =
            TEXT_BUFFER_SIZE
            - 1; /* make sure we always have 0 at the end of the buffer */

        ZeroMemory(text->buffer, TEXT_BUFFER_SIZE);
        SendMessage(dll_hwnd_from_winid(WIN_MESSAGE), WM_MSNH_COMMAND,
                    (WPARAM) MSNH_MSG_GETTEXT, (LPARAM) text);

        next_message = text->buffer;
    }

    if (!(next_message && next_message[0])) {
        free(text);
        next_message = 0;
        return (char *) 0;
    } else {
        char *retval = next_message;
        char *p;
        next_message = p = strchr(next_message, '\n');
        if (next_message)
            next_message++;
        if (p)
            while (p >= retval && isspace((uchar) *p))
                *p-- = (char) 0; /* delete trailing whitespace */
        return retval;
    }
#endif
}

void
dll_putmsghistory_ex(const char *msg, const char* attrs, const char* colors, BOOLEAN_P restoring)
{
    dll_callbacks.callback_putmsghistory(msg, attrs ? attrs[0] : ATR_NONE, colors ? colors[0] : NO_COLOR, restoring);
#if 0
    BOOL save_sound_opt;

    UNREFERENCED_PARAMETER(restoring);

    if (!msg)
        return; /* end of message history restore */
    save_sound_opt = GetNHApp()->bNoSounds;
    GetNHApp()->bNoSounds =
        TRUE; /* disable sounds while restoring message history */
    dll_putstr_ex(WIN_MESSAGE, ATR_NONE, msg, 0, NO_COLOR);
    clear_nhwindow(WIN_MESSAGE); /* it is in fact end-of-turn indication so
                                    each message will print on the new line */
    GetNHApp()->bNoSounds = save_sound_opt; /* restore sounds option */
#endif
}

/* clean up and quit */
void
dll_bail(const char *mesg)
{
    clearlocks();
    dll_exit_nhwindows(mesg);
    nh_terminate(EXIT_SUCCESS);
    /*NOTREACHED*/
}

#ifdef DEBUG
# ifdef _DEBUG
#include <stdarg.h>

void
dll_logDebug(const char *fmt, ...)
{
    va_list args;

    if (!showdebug(NHTRACE_LOG) || !_s_debugfp)
        return;

    va_start(args, fmt);
    vfprintf(_s_debugfp, fmt, args);
    va_end(args);
    fflush(_s_debugfp);
}
# endif
#endif


int
GNHMessageBox(char* text, unsigned int type)
{
    char title[DLL_MAX_LOADSTRING];
    HINSTANCE hApp = GetModuleHandle(NULL);

    LoadString(hApp, IDS_APP_TITLE_SHORT, title, DLL_MAX_LOADSTRING);

    return dll_callbacks.callback_messagebox(text, title, type);
}


#if 0
/* when status hilites are enabled, we use an array of mswin_status_strings
 * to represent what needs to be rendered. */
typedef struct dll_status_string {
    const char* str; /* ascii string to be displayed */
    boolean space_in_front; /* render with a space in front of string */
    int color; /* string text color index */
    int attribute; /* string text attributes */
    boolean draw_bar; /* draw a percentage bar  */
    int bar_percent; /* a percentage to indicate */
    int bar_color; /* color index of percentage bar */
    int bar_attribute; /* attributes of percentage bar */
} dll_status_string;

#define DLL_MAX_LINE_STRINGS 100
#define DLL_MAX_LINE_FIELDS 32

typedef struct dll_status_strings
{
    int count;
    dll_status_string* status_strings[DLL_MAX_LINE_STRINGS];
} dll_status_strings;

typedef struct dll_status_field {
    int field_index; // field index
    boolean enabled; // whether the field is enabled
    const char* name; // name of status field
    const char* format; // format of field
    boolean space_in_front; // add a space in front of the field

    int percent;
    int color;
    int attribute;
    char string[BUFSZ];

} dll_status_field;

typedef struct dll_condition_field {
    unsigned long mask;
    const char* name;
    int bit_position;
} dll_condition_field;

typedef struct dll_status_fields {
    int count;
    dll_status_field* status_fields[DLL_MAX_LINE_FIELDS];
} dll_status_fields;

typedef struct dll_status_line {
    dll_status_strings status_strings;
    dll_status_fields status_fields;
} dll_status_line;

typedef struct dll_status_lines {
    dll_status_line lines[MAX_STATUS_LINES]; /* number of strings to be rendered on each line */
} dll_status_lines;

static dll_status_lines _status_lines;
static dll_status_string _status_strings[MAXBLSTATS];
static dll_status_string _condition_strings[BL_MASK_BITS];
static dll_status_field _status_fields[MAXBLSTATS];

static dll_condition_field _condition_fields[BL_MASK_BITS] = {
    { BL_MASK_STONE, "Stone" },
    { BL_MASK_SLIME, "Slime" },
    { BL_MASK_STRNGL, "Strngl" },
    { BL_MASK_SUFFOC, "Suffoc" },
    { BL_MASK_FOODPOIS, "FoodPois" },
    { BL_MASK_TERMILL, "TermIll" },
    { BL_MASK_BLIND, "Blind" },
    { BL_MASK_DEAF, "Deaf" },
    { BL_MASK_STUN, "Stun" },
    { BL_MASK_CONF, "Conf" },
    { BL_MASK_HALLU, "Hallu" },
    { BL_MASK_LEV, "Lev" },
    { BL_MASK_FLY, "Fly" },
    { BL_MASK_RIDE, "Ride" },
    { BL_MASK_SLOWED, "Slow" },
    { BL_MASK_PARALYZED, "Paral" },
    { BL_MASK_FEARFUL, "Fear" },
    { BL_MASK_SLEEPING, "Sleep" },
    { BL_MASK_CANCELLED, "Cancl" },
    { BL_MASK_SILENCED, "Silent" },
    { BL_MASK_GRAB, "Grab" },
    { BL_MASK_ROT, "Rot" },
    { BL_MASK_LYCANTHROPY, "Lyca" },
};


extern winid WIN_STATUS;

#ifdef STATUS_HILITES
typedef struct hilite_data_struct {
    int thresholdtype;
    anything value;
    int behavior;
    int under;
    int over;
} hilite_data_t;
static hilite_data_t _status_hilites[MAXBLSTATS];
#endif /* STATUS_HILITES */

#endif
/*
status_init()   -- core calls this to notify the window port that a status
                   display is required. The window port should perform
                   the necessary initialization in here, allocate memory, etc.
*/
void
dll_status_init(void)
{
    dll_logDebug("dll_status_init()\n");
    dll_callbacks.callback_status_init();

#if 0
    for (int i = 0; i < SIZE(_status_fields); i++) {
        dll_status_field * status_field = &_status_fields[i];
        status_field->field_index = i;
        status_field->enabled = FALSE;
    }

    for (int i = 0; i < SIZE(_condition_fields); i++) {
        dll_condition_field * condition_field = &_condition_fields[i];
        nhassert(condition_field->mask == (1 << i));
        condition_field->bit_position = i;
    }

    for (int i = 0; i < SIZE(_status_strings); i++) {
        dll_status_string * status_string = &_status_strings[i];
        status_string->str = NULL;
    }

    for (int i = 0; i < SIZE(_condition_strings); i++) {
        dll_status_string * status_string = &_condition_strings[i];
        status_string->str = NULL;
    }

    for (int lineIndex = 0; lineIndex < SIZE(_status_lines.lines); lineIndex++) {
        dll_status_line * line = &_status_lines.lines[lineIndex];

        dll_status_fields * status_fields = &line->status_fields;
        status_fields->count = 0;

        dll_status_strings * status_strings = &line->status_strings;
        status_strings->count = 0;

        for (int i = 0; i < iflags.wc2_statuslines == 2 ? fieldcounts_2statuslines[lineIndex] : fieldcounts[lineIndex]; i++) {
            int field_index = iflags.wc2_statuslines == 2 ? fieldorders_2statuslines[lineIndex][i] : fieldorders[lineIndex][i];
            nhassert(field_index <= SIZE(_status_fields));

            nhassert(status_fields->count <= SIZE(status_fields->status_fields));
            status_fields->status_fields[status_fields->count++] = &_status_fields[field_index];

            nhassert(status_strings->count <= SIZE(status_strings->status_strings));
            status_strings->status_strings[status_strings->count++] =
                &_status_strings[field_index];

            if (field_index == BL_CONDITION) {
                for (int j = 0; j < BL_MASK_BITS; j++) {
                    nhassert(status_strings->count <= SIZE(status_strings->status_strings));
                    status_strings->status_strings[status_strings->count++] =
                        &_condition_strings[j];
                }
            }
        }
    }

    for (int i = 0; i < MAXBLSTATS; ++i) {
#ifdef STATUS_HILITES
        _status_hilites[i].thresholdtype = 0;
        _status_hilites[i].behavior = BL_TH_NONE;
        _status_hilites[i].under = BL_HILITE_NONE;
        _status_hilites[i].over = BL_HILITE_NONE;
#endif /* STATUS_HILITES */
    }
#endif
    /* Use a window for the genl version; backward port compatibility */
    WIN_STATUS = create_nhwindow(NHW_STATUS);
    display_nhwindow(WIN_STATUS, FALSE);
}

/*
status_finish() -- called when it is time for the window port to tear down
                   the status display and free allocated memory, etc.
*/
void
dll_status_finish(void)
{
    dll_logDebug("dll_status_finish()\n");
    dll_callbacks.callback_status_finish();
}

/*
status_enablefield(int fldindex, char fldname, char fieldfmt, boolean enable)
                -- notifies the window port which fields it is authorized to
                   display.
                -- This may be called at any time, and is used
                   to disable as well as enable fields, depending on the
                   value of the final argument (TRUE = enable).
                -- fldindex could be one of the following from botl.h:
                   BL_TITLE, BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH,
                   BL_ALIGN, BL_SCORE, BL_CAP, BL_GOLD, BL_ENE, BL_ENEMAX,
                   BL_XP, BL_AC, BL_HD, BL_TIME, BL_HUNGER, BL_HP, BL_HPMAX,
                   BL_LEVELDESC, BL_EXP, BL_CONDITION
                -- There are MAXBLSTATS status fields (from botl.h)
*/
void
dll_status_enablefield(int fieldidx, const char *nm, const char *fmt,
                         boolean enable)
{
    dll_logDebug("dll_status_enablefield(%d, %s, %s, %d)\n", fieldidx, nm, fmt,
             (int) enable);
    dll_callbacks.callback_status_enablefield(fieldidx, nm, fmt, enable);

#if 0
    nhassert(fieldidx <= SIZE(_status_fields));
    dll_status_field * field = &_status_fields[fieldidx];

    nhassert(fieldidx <= SIZE(_status_strings));
    dll_status_string * string = &_status_strings[fieldidx];

    if (field != NULL) {
        field->format = fmt;
        field->space_in_front = (fmt[0] == ' ');
        if (field->space_in_front) field->format++;
        field->name = nm;
        field->enabled = enable;

        string->str = (field->enabled ? field->string : NULL);
        string->space_in_front = field->space_in_front;

        if (field->field_index == BL_CONDITION)
            string->str = NULL;

        string->draw_bar = (field->enabled && field->field_index == BL_TITLE);
    }
#endif

}


/*

status_update(int fldindex, genericptr_t ptr, int chg, int percent, int color, unsigned long *colormasks)
                -- update the value of a status field.
                -- the fldindex identifies which field is changing and
                   is an integer index value from botl.h
                -- fldindex could be any one of the following from botl.h:
                   BL_TITLE, BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH,
                   BL_ALIGN, BL_SCORE, BL_CAP, BL_GOLD, BL_ENE, BL_ENEMAX,
                   BL_XP, BL_AC, BL_HD, BL_TIME, BL_HUNGER, BL_HP, BL_HPMAX,
                   BL_LEVELDESC, BL_EXP, BL_CONDITION
        -- fldindex could also be BL_FLUSH, which is not really
           a field index, but is a special trigger to tell the 
           windowport that it should output all changes received
                   to this point. It marks the end of a bot() cycle.
        -- fldindex could also be BL_RESET, which is not really
           a field index, but is a special advisory to to tell the 
           windowport that it should redisplay all its status fields,
           even if no changes have been presented to it.
                -- ptr is usually a "char *", unless fldindex is BL_CONDITION.
                   If fldindex is BL_CONDITION, then ptr is a long value with
                   any or none of the following bits set (from botl.h):
                        BL_MASK_STONE           0x00000001L
                        BL_MASK_SLIME           0x00000002L
                        BL_MASK_STRNGL          0x00000004L
                        BL_MASK_FOODPOIS        0x00000008L
                        BL_MASK_TERMILL         0x00000010L
                        BL_MASK_BLIND           0x00000020L
                        BL_MASK_DEAF            0x00000040L
                        BL_MASK_STUN            0x00000080L
                        BL_MASK_CONF            0x00000100L
                        BL_MASK_HALLU           0x00000200L
                        BL_MASK_LEV             0x00000400L
                        BL_MASK_FLY             0x00000800L
                        BL_MASK_RIDE            0x00001000L
                -- The value passed for BL_GOLD includes an encoded leading
                   symbol for MAT_GOLD "\GXXXXNNNN:nnn". If window port needs
                   textual gold amount without the leading "$:" the port will
                   have to skip past ':' in passed "ptr" for the BL_GOLD case.
                -- color is the color that the GnollHack core is telling you to
                   use to display the text.
                -- condmasks is a pointer to a set of BL_ATTCLR_MAX unsigned
                   longs telling which conditions should be displayed in each
                   color and attriubte.
*/
void
dll_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long *condmasks)
{
    dll_logDebug("dll_status_update(%d, %p, %d, %d, %x, %p)\n", idx, ptr, chg, percent, color, condmasks);
    //dll_callbacks.callback_status_update(idx, ptr, chg, percent, color, condmasks);

#if 0
    long cond, * condptr = (long*)ptr;
    char* text = (char*)ptr;
    MSNHMsgUpdateStatus update_cmd_data;
    int ocolor, ochar;
    unsigned long ospecial;

    if (idx >= 0) {

        nhassert(idx <= SIZE(_status_fields));
        dll_status_field * status_field = &_status_fields[idx];
        nhassert(status_field->field_index == idx);

        nhassert(idx <= SIZE(_status_strings));
        dll_status_string * status_string = &_status_strings[idx];

        if (!status_field->enabled) {
            nhassert(status_string->str == NULL);
            return;
        }

        status_field->color = status_string->color = color & 0xff;
        status_field->attribute = status_string->attribute = (color >> 8) & 0xff;

        switch (idx) {
        case BL_CONDITION: {
            dll_condition_field * condition_field = _condition_fields;

            nhassert(status_string->str == NULL);

            cond = *condptr;

            for (int i = 0; i < BL_MASK_BITS; i++, condition_field++) {
                status_string = &_condition_strings[i];

                if (condition_field->mask & cond) {
                    status_string->str = condition_field->name;
                    status_string->space_in_front = TRUE;
                    status_string->color = dll_condcolor(condition_field->mask, condmasks);
                    status_string->attribute = dll_condattr(condition_field->mask, condmasks);
                }
                else
                    status_string->str = NULL;
            }
        } break;
            
        case BL_GOLD: {
            char buf[BUFSZ];
            char *p;

            ZeroMemory(buf, sizeof(buf));
            if (iflags.invis_goldsym)
                ochar = GOLD_SYM;
            else
            {
                struct layer_info layers = nul_layerinfo;
                layers.glyph = objnum_to_glyph(GOLD_PIECE);
                mapglyph(layers,
                    &ochar, &ocolor, &ospecial, 0, 0);

            }
            buf[0] = ochar;
            p = strchr(text, ':');
            if (p) {
                strncpy(buf + 1, p, sizeof(buf) - 2);
            } else {
                buf[1] = ':';
                strncpy(buf + 2, text, sizeof(buf) - 3);
            }
            buf[sizeof buf - 1] = '\0';
            Sprintf(status_field->string,
                    status_field->format ? status_field->format : "%s", buf);
            nhassert(status_string->str == status_field->string);
        } break;
        default: {
            Sprintf(status_field->string,
                    status_field->format ? status_field->format : "%s", text);
            nhassert(status_string->str == status_field->string);
        } break;
        }

        /* if we received an update for the hp field, we must update the
         * bar percent and bar color for the title string */
        if (idx == BL_HP || idx == BL_HPMAX)
        {
            dll_status_string * title_string = &_status_strings[BL_TITLE];

            if (idx == BL_HP)
            {
                title_string->bar_color = color & 0xff;
                title_string->bar_attribute = (color >> 8) & 0xff;
            }
            title_string->bar_percent = percent;

        }

    }

    if (idx == BL_FLUSH || idx == BL_RESET) {
        /* send command to status window to update */
        ZeroMemory(&update_cmd_data, sizeof(update_cmd_data));
        update_cmd_data.status_lines = &_status_lines;
        SendMessage(dll_hwnd_from_winid(WIN_STATUS), WM_MSNH_COMMAND,
            (WPARAM)MSNH_MSG_UPDATE_STATUS, (LPARAM)&update_cmd_data);
    }
#endif
}

void
dll_stretch_window(void)
{
    dll_callbacks.callback_stretch_window();

#if 0
    if (GetNHApp()->windowlist[WIN_MAP].win != NULL) {
        MSNHMsgClipAround data;

        ZeroMemory(&data, sizeof(data));
        data.x = GetNHApp()->mapTile_X;
        data.y = GetNHApp()->mapTile_Y;
        SendMessage(GetNHApp()->windowlist[WIN_MAP].win, WM_MSNH_COMMAND,
            (WPARAM)MSNH_MSG_STRETCH_MAP, (LPARAM)&data);
    }
#endif
}

void
dll_set_animation_timer_interval(unsigned int interval)
{
    dll_callbacks.callback_set_animation_timer_interval(interval);
#if 0
    if (GetNHApp()->windowlist[WIN_MAP].win != NULL) {
        UINT data;

        ZeroMemory(&data, sizeof(data));
        data = interval;
        SendMessage(GetNHApp()->windowlist[WIN_MAP].win, WM_MSNH_COMMAND,
            (WPARAM)MSNH_MSG_SET_ANIMATION_TIMER, (LPARAM)&data);
    }
#endif
}


int
dll_open_special_view(struct special_view_info info)
{
    switch (info.viewtype)
    {
    case SPECIAL_VIEW_CHAT_MESSAGE:
        break;
    default:
        break;
    }
    return 0;
}

void
dll_stop_all_sounds(struct stop_all_info info)
{
#if 0
    if (!dll_callbacks.callback_stop_all_sounds())
    {
        impossible("Cannot stop all sounds!");
    }
#endif
}

void
dll_play_immediate_ghsound(struct ghsound_immediate_info info)
{
#if 0
    if (!dll_callbacks.callback_play_immediate_ghsound(info.ghsound, (double)info.volume))
    {
        impossible("Cannot play immediate sound!");
    }
#endif
}

void
dll_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info)
{
#if 0
    if (!dll_callbacks.callback_play_ghsound_occupation_ambient(info.ghsound, (double)info.volume))
    {
        impossible("Cannot play occupation ambient sound!");
    }
#endif
}

void
dll_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info)
{
#if 0
    if (!dll_callbacks.callback_play_ghsound_effect_ambient(info.ghsound, (double)info.volume))
    {
        impossible("Cannot play effect ambient sound!");
    }
#endif
}

void
dll_set_effect_ambient_volume(struct effect_ambient_volume_info info)
{
    if (!dll_callbacks.callback_set_effect_ambient_volume((double)info.volume))
    {
        impossible("Cannot play set effect ambient volume!");
    }
}

void
dll_play_ghsound_music(struct ghsound_music_info info)
{
#if 0
    if (!dll_callbacks.callback_play_ghsound_music(info.ghsound, (double)info.volume))
    {
        impossible("Cannot play music!");
    }
#endif
}

void
dll_play_ghsound_level_ambient(struct ghsound_level_ambient_info info)
{
    //if (!dll_callbacks.callback_play_ghsound_level_ambient(info.ghsound, (double)info.volume))
    //{
    //    impossible("Cannot play level ambient sound!");
    //}
}

void
dll_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info)
{
    //if (!dll_callbacks.callback_play_ghsound_environment_ambient(info.ghsound, (double)info.volume))
    //{
    //    impossible("Cannot play environment ambient sound!");
    //}
}

void
dll_adjust_ghsound_general_volumes(VOID_ARGS)
{
    float new_general_volume = ((float)flags.sound_volume_general) / 100.0f;
    float new_music_volume = ((float)flags.sound_volume_music) / 100.0f;
    float new_ambient_volume = ((float)flags.sound_volume_ambient) / 100.0f;
    float new_dialogue_volume = ((float)flags.sound_volume_dialogue) / 100.0f;
    float new_effects_volume = ((float)flags.sound_volume_effects) / 100.0f;
    float new_ui_volume = ((float)flags.sound_volume_ui) / 100.0f;

    if (!dll_callbacks.callback_adjust_ghsound_general_volumes(new_general_volume, new_music_volume, new_ambient_volume, new_dialogue_volume, new_effects_volume, new_ui_volume))
    {
        impossible("Cannot adjust volume!");
    }
}

void
dll_add_ambient_ghsound(struct soundsource_t* soundsource)
{
    //if (!dll_callbacks.callback_add_ambient_ghsound(soundsource->ghsound, soundsource->heard_volume, soundsource->ambient_ghsound_ptr))
    //{
    //    impossible("Cannot add ambient sound!");
    //}
}

void
dll_delete_ambient_ghsound(struct soundsource_t* soundsource)
{
    //if (!dll_callbacks.callback_delete_ambient_ghsound(soundsource->ambient_ghsound_ptr))
    //{
    //    impossible("Cannot delete ambient sound!");
    //}
}

void
dll_set_ambient_ghsound_volume(struct soundsource_t* soundsource)
{
    //if (!dll_callbacks.callback_set_ambient_ghsound_volume((unsigned long long)soundsource->ambient_ghsound_ptr, (double)soundsource->heard_volume))
    //{
    //    impossible("Cannot set ambient sound volume!");
    //}
}

void
dll_init_platform(VOID_ARGS)
{

}

void
dll_exit_platform(int status)
{

}

void
dll_exit_hack(int status)
{
    dll_exit_platform(status);
    dll_callbacks.callback_exit_hack(status);
}

void
dll_clear_context_menu(VOID_ARGS)
{
    return;
}

void
dll_add_context_menu(int cmd_def_char, int cmd_cur_char, int style, int glyph, const char* cmd_text, const char* target_text, int attr, int color)
{
    return;
}

void
dll_update_status_button(int cmd, int btn, int val, unsigned long bflags)
{
    return;
}

void
dll_toggle_animation_timer(int timertype, int timerid, int state, int x, int y, int layer, unsigned long tflags)
{
    return;
}

void
dll_display_floating_text(int x, int y, const char* text, int style, int attr, int color, unsigned long tflags)
{
    return;
}

void
dll_display_screen_text(const char* text, const char* supertext, const char* subtext, int style, int attr, int color, unsigned long tflags)
{

}

void
dll_display_popup_text(const char* text, const char* title, int style, int attr, int color, int glyph, unsigned long tflags)
{

}

void
dll_display_gui_effect(int x, int y, int style, unsigned long tflags)
{
    return;
}

void
dll_update_cursor(int style, int force_paint, int show_on_u)
{
    return;
}

int
dll_ui_has_input(VOID_ARGS)
{
    return FALSE;
}

void
set_wincaps(wincap1, wincap2)
unsigned long wincap1, wincap2;
{
    if(wincap1 > 0)
        dll_procs.wincap = wincap1;
    if (wincap2 > 0)
        dll_procs.wincap2 = wincap2;
}


DLL boolean dll_validrole(int role)
{
    return validrole(role);
}

DLL int dll_str2role(const char* role_str)
{
    return str2role(role_str);
}


char* dll_getcwd(char* dest_buf, int size_in_bytes)
{
   char* cwd = dll_callbacks.callback_getcwd();
   if (!cwd)
       return 0;

   size_t length = strlen(cwd);
   if (length >= size_in_bytes)
       return 0;

   strncpy(dest_buf, cwd, size_in_bytes - 1);
   dest_buf[size_in_bytes - 1] = '\0';

   return dest_buf;
}

DLL void
dll_reduce_counters_intervals(int intervals)
{
    reduce_counters_intervals(intervals);
}



/* Test */
DLL int DoSomeCalc2()
{
    HINSTANCE hinst = GetModuleHandle(NULL);
    char buf[256];
    size_t length = 0;
    if (getcwd(buf, 256))
        length = strlen(buf);
    return (int)length;
}

void
mswin_destroy_reg()
{
    /* Nothing */
}
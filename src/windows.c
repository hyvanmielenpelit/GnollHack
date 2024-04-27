/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    windows.c    $NHDT-Date: 1526933747 2018/05/21 20:15:47 $  $NHDT-Branch: GnollHack-3.6.2 $:$NHDT-Revision: 1.48 $ */
/* Copyright (c) D. Cohrs, 1993. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"

#ifdef TTY_GRAPHICS
#include "wintty.h"
#endif
#ifdef CURSES_GRAPHICS
extern struct window_procs curses_procs;
#endif
#ifdef X11_GRAPHICS
/* Cannot just blindly include winX.h without including all of X11 stuff
   and must get the order of include files right.  Don't bother. */
extern struct window_procs X11_procs;
extern void FDECL(win_X11_init, (int));
#endif
#ifdef QT_GRAPHICS
extern struct window_procs Qt_procs;
#endif
#ifdef GEM_GRAPHICS
#include "wingem.h"
#endif
#ifdef MAC
extern struct window_procs mac_procs;
#endif
#ifdef BEOS_GRAPHICS
extern struct window_procs beos_procs;
extern void FDECL(be_win_init, (int));
FAIL /* be_win_init doesn't exist? XXX*/
#endif
#ifdef AMIGA_INTUITION
extern struct window_procs amii_procs;
extern struct window_procs amiv_procs;
extern void FDECL(ami_wininit_data, (int));
#endif
#ifdef WIN32_GRAPHICS
extern struct window_procs win32_procs;
#endif
#ifdef GNOME_GRAPHICS
#include "winGnome.h"
extern struct window_procs Gnome_procs;
#endif
#ifdef MSWIN_GRAPHICS
extern struct window_procs mswin_procs;
#endif
#ifdef DLL_GRAPHICS
extern struct window_procs dll_procs;
#endif
#ifdef LIB_GRAPHICS
extern struct window_procs lib_procs;
#endif
#ifdef NUKLEAR_GRAPHICS
extern struct window_procs nuklear_procs;
#endif
#ifdef ANDROID_GRAPHICS
extern struct window_procs and_procs;
#endif
#ifdef WINCHAIN
extern struct window_procs chainin_procs;
extern void FDECL(chainin_procs_init, (int));
extern void *FDECL(chainin_procs_chain, (int, int, void *, void *, void *));

extern struct chain_procs chainout_procs;
extern void FDECL(chainout_procs_init, (int));
extern void *FDECL(chainout_procs_chain, (int, int, void *, void *, void *));

extern struct chain_procs trace_procs;
extern void FDECL(trace_procs_init, (int));
extern void *FDECL(trace_procs_chain, (int, int, void *, void *, void *));
#endif

STATIC_DCL void FDECL(def_raw_print, (const char *s));
STATIC_DCL void NDECL(def_wait_synch);
STATIC_DCL void FDECL(itemdesc_putstr_ex, (winid, const char*, int, int, int));
STATIC_DCL void FDECL(itemdesc_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));

#if defined (DUMPLOG) || defined (DUMPHTML)
STATIC_DCL winid FDECL(dump_create_nhwindow_ex, (int, int, int, struct extended_create_window_info));
STATIC_DCL void FDECL(dump_clear_nhwindow, (winid));
STATIC_DCL void FDECL(dump_display_nhwindow, (winid, BOOLEAN_P));
STATIC_DCL void FDECL(dump_destroy_nhwindow, (winid));
STATIC_DCL void FDECL(dump_start_menu_ex, (winid, int));
STATIC_DCL void FDECL(dump_add_menu, (winid, int, const ANY_P *, CHAR_P,
                                      CHAR_P, int, int, const char *, BOOLEAN_P));
STATIC_DCL void FDECL(dump_add_extended_menu, (winid, int, const ANY_P*, CHAR_P,
    CHAR_P, int, int, const char*, BOOLEAN_P, struct extended_menu_info));
STATIC_DCL void FDECL(dump_end_menu_ex, (winid, const char *, const char*));
STATIC_DCL int FDECL(dump_select_menu, (winid, int, MENU_ITEM_P **));
STATIC_DCL void FDECL(dump_putstr_ex, (winid, const char *, int, int, int));
STATIC_DCL void FDECL(dump_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));
STATIC_DCL void NDECL(dump_headers);
STATIC_DCL void NDECL(dump_footers);
STATIC_DCL void FDECL(dump_status_update, (int, genericptr_t, int, int, int, unsigned long*));
STATIC_DCL void NDECL(dump_render_status);
#ifdef DUMPHTML
STATIC_DCL void FDECL(dump_set_color_attr, (int, int, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL void NDECL(html_init_sym);
STATIC_DCL void NDECL(dump_css);
STATIC_DCL void FDECL(dump_outrip, (winid, int, time_t));
STATIC_DCL void FDECL(html_dump_char, (FILE*, nhsym, BOOLEAN_P));
STATIC_DCL void FDECL(html_dump_str, (FILE*, const char*, const char*, const char*, int, int));
STATIC_DCL void FDECL(html_dump_line, (FILE*, winid, const char*, const char*, int, int, int, const char*));
STATIC_DCL void FDECL(html_write_tags, (FILE*, winid, int, int, int, BOOLEAN_P, struct extended_menu_info, BOOLEAN_P)); /* Tags before/after string */
#endif
#ifdef DUMPLOG
STATIC_VAR FILE* dumplog_file;
#endif
#ifdef DUMPHTML
STATIC_VAR FILE* dumphtml_file;
#endif
#endif /* DUMPLOG */

#ifdef HANGUPHANDLING
volatile
#endif

NEARDATA struct window_procs windowprocs;

#ifdef WINCHAIN
#define CHAINR(x) , x
#else
#define CHAINR(x)
#endif

STATIC_VAR struct win_choices {
    struct window_procs *procs;
    void FDECL((*ini_routine), (int)); /* optional (can be 0) */
#ifdef WINCHAIN
    void *FDECL((*chain_routine), (int, int, void *, void *, void *));
#endif
} winchoices[] = {
#ifdef TTY_GRAPHICS
    { &tty_procs, win_tty_init CHAINR(0) },
#endif
#ifdef CURSES_GRAPHICS
    { &curses_procs, 0 },
#endif
#ifdef X11_GRAPHICS
    { &X11_procs, win_X11_init CHAINR(0) },
#endif
#ifdef QT_GRAPHICS
    { &Qt_procs, 0 CHAINR(0) },
#endif
#ifdef GEM_GRAPHICS
    { &Gem_procs, win_Gem_init CHAINR(0) },
#endif
#ifdef MAC
    { &mac_procs, 0 CHAINR(0) },
#endif
#ifdef BEOS_GRAPHICS
    { &beos_procs, be_win_init CHAINR(0) },
#endif
#ifdef AMIGA_INTUITION
    { &amii_procs,
      ami_wininit_data CHAINR(0) }, /* Old font version of the game */
    { &amiv_procs,
      ami_wininit_data CHAINR(0) }, /* Tile version of the game */
#endif
#ifdef WIN32_GRAPHICS
    { &win32_procs, 0 CHAINR(0) },
#endif
#ifdef GNOME_GRAPHICS
    { &Gnome_procs, 0 CHAINR(0) },
#endif
#ifdef MSWIN_GRAPHICS
    { &mswin_procs, 0 CHAINR(0) },
#endif
#ifdef DLL_GRAPHICS
    { &dll_procs, 0 CHAINR(0) },
#endif
#ifdef LIB_GRAPHICS
    { &lib_procs, 0 CHAINR(0) },
#endif
#ifdef NUKLEAR_GRAPHICS
    { &nuklear_procs, 0 CHAINR(0) },
#endif
#ifdef ANDROID_GRAPHICS
    { &and_procs, 0 CHAINR(0) },
#endif
#ifdef WINCHAIN
    { &chainin_procs, chainin_procs_init, chainin_procs_chain },
    { (struct window_procs *) &chainout_procs, chainout_procs_init,
      chainout_procs_chain },

    { (struct window_procs *) &trace_procs, trace_procs_init,
      trace_procs_chain },
#endif
    { 0, 0 CHAINR(0) } /* must be last */
};

#ifdef WINCHAIN
struct winlink {
    struct winlink *nextlink;
    struct win_choices *wincp;
    void *linkdata;
};
/* NB: this chain does not contain the terminal real window system pointer */

STATIC_VAR struct winlink *chain = 0;

STATIC_OVL struct winlink *
wl_new()
{
    struct winlink *wl = (struct winlink *) alloc(sizeof *wl);

    wl->nextlink = 0;
    wl->wincp = 0;
    wl->linkdata = 0;

    return wl;
}

STATIC_OVL void
wl_addhead(struct winlink *wl)
{
    wl->nextlink = chain;
    chain = wl;
}

STATIC_OVL void
wl_addtail(struct winlink *wl)
{
    struct winlink *p = chain;

    if (!chain) {
        chain = wl;
        return;
    }
    while (p->nextlink) {
        p = p->nextlink;
    }
    p->nextlink = wl;
    return;
}
#endif /* WINCHAIN */

STATIC_VAR struct win_choices *last_winchoice = 0;

boolean
genl_can_suspend_no(VOID_ARGS)
{
    return FALSE;
}

boolean
genl_can_suspend_yes(VOID_ARGS)
{
    return TRUE;
}

void
genl_stretch_window(VOID_ARGS)
{
    return;
}

void
genl_set_animation_timer_interval(interval)
unsigned int interval UNUSED;
{
    return;
}

int
genl_open_special_view(info)
struct special_view_info info;
{
    switch (info.viewtype)
    {
    case SPECIAL_VIEW_CHAT_MESSAGE:
        genl_chat_message();
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
genl_stop_all_sounds(info)
struct stop_all_info info UNUSED;
{
    return;
}

void
genl_play_immediate_ghsound(info)
struct ghsound_immediate_info info UNUSED;
{
    return;
}

void
genl_play_ghsound_occupation_ambient(info)
struct ghsound_occupation_ambient_info info UNUSED;
{
    return;
}

void
genl_play_ghsound_effect_ambient(info)
struct ghsound_effect_ambient_info info UNUSED;
{
    return;
}

void
genl_set_effect_ambient_volume(info)
struct effect_ambient_volume_info info UNUSED;
{
    return;
}

void
genl_play_ghsound_music(info)
struct ghsound_music_info info UNUSED;
{
    return;
}

void
genl_play_ghsound_level_ambient(info)
struct ghsound_level_ambient_info info UNUSED;
{
    return;
}

void
genl_play_ghsound_environment_ambient(info)
struct ghsound_environment_ambient_info info UNUSED;
{
    return;
}

void
genl_adjust_ghsound_general_volumes(VOID_ARGS)
{
    return;
}

void
genl_add_ambient_ghsound(struct soundsource_t* soundsource UNUSED)
{
    return;
}

void
genl_delete_ambient_ghsound(struct soundsource_t* soundsource UNUSED)
{
    return;
}

void
genl_set_ambient_ghsound_volume(struct soundsource_t* soundsource UNUSED)
{
    return;
}

void
genl_clear_context_menu(VOID_ARGS)
{
    return;
}

void
genl_add_context_menu(int cmd_def_char UNUSED, int cmd_cur_char UNUSED, int style UNUSED, int glyph UNUSED, const char* cmd_text UNUSED, const char* target_text UNUSED, int attr UNUSED, int color UNUSED)
{
    return;
}

void
genl_update_status_button(int cmd UNUSED, int btn UNUSED, int val UNUSED, unsigned long bflags UNUSED)
{
    return;
}

void
genl_toggle_animation_timer(int type UNUSED, int id UNUSED, int state UNUSED, int x UNUSED, int y UNUSED, int layer UNUSED, unsigned long tflags UNUSED)
{
    return;
}

void
genl_display_floating_text(int x UNUSED, int y UNUSED, const char* text UNUSED, int style UNUSED, int attr UNUSED, int color UNUSED, unsigned long tflags UNUSED)
{
    return;
}

void
genl_display_screen_text(const char* text UNUSED, const char* supertext UNUSED, const char* subtext UNUSED, int style UNUSED, int attr UNUSED, int color UNUSED, unsigned long tflags UNUSED)
{
    return;
}

void
genl_display_popup_text(const char* text UNUSED, const char* title UNUSED, int style UNUSED, int attr UNUSED, int color UNUSED, int glyph UNUSED, unsigned long tflags UNUSED)
{
    return;
}

void
genl_display_gui_effect(int style UNUSED, int subtype UNUSED, int x UNUSED, int y UNUSED, int x2 UNUSED, int y2 UNUSED, unsigned long tflags UNUSED)
{
    return;
}

void
genl_update_cursor(int style UNUSED, int force_paint UNUSED, int show_on_u UNUSED)
{
    return;
}

int
genl_ui_has_input(VOID_ARGS)
{
    return FALSE;
}

void
genl_exit_hack(int status UNUSED)
{
    
}

STATIC_OVL
void
def_raw_print(s)
const char *s;
{
    puts(s);
}

STATIC_OVL
void
def_wait_synch(VOID_ARGS)
{
    /* Config file error handling routines
     * call wait_sync() without checking to
     * see if it actually has a value,
     * leading to spectacular violations
     * when you try to execute address zero.
     * The existence of this allows early
     * processing to have something to execute
     * even though it essentially does nothing
     */
     return;
}

#ifdef WINCHAIN
STATIC_OVL struct win_choices *
win_choices_find(s)
const char *s;
{
    register int i;

    for (i = 0; winchoices[i].procs; i++) {
        if (!strcmpi(s, winchoices[i].procs->name)) {
            return &winchoices[i];
        }
    }
    return (struct win_choices *) 0;
}
#endif

void
choose_windows(s)
const char *s;
{
    int i;
    char* tmps = 0;

    for (i = 0; winchoices[i].procs; i++) {
        if ('+' == winchoices[i].procs->name[0])
            continue;
        if ('-' == winchoices[i].procs->name[0])
            continue;
        if (!strcmpi(s, winchoices[i].procs->name)) {
            windowprocs = *winchoices[i].procs;

            if (last_winchoice && last_winchoice->ini_routine)
                (*last_winchoice->ini_routine)(WININIT_UNDO);
            if (winchoices[i].ini_routine)
                (*winchoices[i].ini_routine)(WININIT);
            last_winchoice = &winchoices[i];
            return;
        }
    }

    if (!windowprocs.win_raw_print)
        windowprocs.win_raw_print = def_raw_print;
    if (!windowprocs.win_wait_synch)
        /* early config file error processing routines call this */
        windowprocs.win_wait_synch = def_wait_synch;

    if (!winchoices[0].procs) {
        raw_printf("No window types supported?");
        nh_terminate(EXIT_FAILURE);
        return;
    }

    /* 50: arbitrary, no real window_type names are anywhere near that long;
   used to prevent potential raw_printf() overflow if user supplies a
   very long string (on the order of 1200 chars) on the command line
   (config file options can't get that big; they're truncated at 1023) */
#define WINDOW_TYPE_MAXLEN 50
    if (strlen(s) >= WINDOW_TYPE_MAXLEN) {
        tmps = (char*)alloc(WINDOW_TYPE_MAXLEN);
        Strncpy(tmps, s, WINDOW_TYPE_MAXLEN - 1);
        tmps[WINDOW_TYPE_MAXLEN - 1] = '\0';
        s = tmps;
    }
#undef WINDOW_TYPE_MAXLEN

    if (!winchoices[1].procs) {
        config_error_add(
                     "Window type %s not recognized.  The only choice is: %s",
                         s, winchoices[0].procs->name);
    } else {
        char buf[BUFSZ];
        boolean first = TRUE;

        buf[0] = '\0';
        for (i = 0; winchoices[i].procs; i++) {
            if ('+' == winchoices[i].procs->name[0])
                continue;
            if ('-' == winchoices[i].procs->name[0])
                continue;
            Sprintf(eos(buf), "%s%s",
                    first ? "" : ", ", winchoices[i].procs->name);
            first = FALSE;
        }
        config_error_add("Window type %s not recognized.  Choices are:  %s",
                         s, buf);
    }

    if (tmps)
        free((genericptr_t)tmps) /*, tmps = 0*/;

    if (windowprocs.win_raw_print == def_raw_print
            || WINDOWPORT("safe-startup"))
        nh_terminate(EXIT_SUCCESS);
}

#ifdef WINCHAIN
void
addto_windowchain(s)
const char *s;
{
    register int i;

    for (i = 0; winchoices[i].procs; i++) {
        if ('+' != winchoices[i].procs->name[0])
            continue;
        if (!strcmpi(s, winchoices[i].procs->name)) {
            struct winlink *p = wl_new();

            p->wincp = &winchoices[i];
            wl_addtail(p);
            /* NB: The ini_routine() will be called during commit. */
            return;
        }
    }

    windowprocs.win_raw_print = def_raw_print;

    raw_printf("Window processor %s not recognized.  Choices are:", s);
    for (i = 0; winchoices[i].procs; i++) {
        if ('+' != winchoices[i].procs->name[0])
            continue;
        raw_printf("        %s", winchoices[i].procs->name);
    }

    nh_terminate(EXIT_FAILURE);
}

void
commit_windowchain()
{
    struct winlink *p;
    int n;
    int wincap, wincap2;

    if (!chain)
        return;

    /* Save wincap* from the real window system - we'll restore it below. */
    wincap = windowprocs.wincap;
    wincap2 = windowprocs.wincap2;

    /* add -chainin at head and -chainout at tail */
    p = wl_new();
    p->wincp = win_choices_find("-chainin");
    if (!p->wincp) {
        raw_printf("Can't locate processor '-chainin'");

    gnollhack_exit(EXIT_FAILURE);
    }
    wl_addhead(p);

    p = wl_new();
    p->wincp = win_choices_find("-chainout");
    if (!p->wincp) {
        raw_printf("Can't locate processor '-chainout'");

    gnollhack_exit(EXIT_FAILURE);
    }
    wl_addtail(p);

    /* Now alloc() init() similar to Objective-C. */
    for (n = 1, p = chain; p; n++, p = p->nextlink) {
        p->linkdata = (*p->wincp->chain_routine)(WINCHAIN_ALLOC, n, 0, 0, 0);
    }

    for (n = 1, p = chain; p; n++, p = p->nextlink) {
        if (p->nextlink) {
            (void) (*p->wincp->chain_routine)(WINCHAIN_INIT, n, p->linkdata,
                                              p->nextlink->wincp->procs,
                                              p->nextlink->linkdata);
        } else {
            (void) (*p->wincp->chain_routine)(WINCHAIN_INIT, n, p->linkdata,
                                              last_winchoice->procs, 0);
        }
    }

    /* Restore the saved wincap* values.  We do it here to give the
     * ini_routine()s a chance to change or check them. */
    chain->wincp->procs->wincap = wincap;
    chain->wincp->procs->wincap2 = wincap2;

    /* Call the init procs.  Do not re-init the terminal real win. */
    p = chain;
    while (p->nextlink) {
        if (p->wincp->ini_routine) {
            (*p->wincp->ini_routine)(WININIT);
        }
        p = p->nextlink;
    }

    /* Install the chain into window procs very late so ini_routine()s
     * can raw_print on error. */
    windowprocs = *chain->wincp->procs;

    p = chain;
    while (p) {
        struct winlink *np = p->nextlink;
        free(p);
        p = np; /* assignment, not proof */
    }
}
#endif /* WINCHAIN */

/*
 * tty_message_menu() provides a means to get feedback from the
 * --More-- prompt; other interfaces generally don't need that.
 */
/*ARGSUSED*/
char
genl_message_menu(let, how, mesg)
char let UNUSED;
int how UNUSED;
const char *mesg;
{
    pline("%s", mesg);
    return 0;
}

/*ARGSUSED*/
void
genl_preference_update(pref)
const char *pref UNUSED;
{
    /* window ports are expected to provide
       their own preference update routine
       for the preference capabilities that
       they support.
       Just return in this genl one. */
    return;
}

char *
genl_getmsghistory_ex(attrs_ptr, colors_ptr, init)
char** attrs_ptr, **colors_ptr;
boolean init UNUSED;
{
    if (attrs_ptr)
        *attrs_ptr = (char*)0;
    if (colors_ptr)
        *colors_ptr = (char*)0;

    /* window ports can provide
       their own getmsghistory() routine to
       preserve message history between games.
       The routine is called repeatedly from
       the core save routine, and the window
       port is expected to successively return
       each message that it wants saved, starting
       with the oldest message first, finishing
       with the most recent.
       Return null pointer when finished.
     */
    return (char *) 0;
}

void
genl_putmsghistory_ex(msg, attrs, colors, is_restoring)
const char *msg;
const char* attrs, *colors;
boolean is_restoring;
{
    /* window ports can provide
       their own putmsghistory() routine to
       load message history from a saved game.
       The routine is called repeatedly from
       the core restore routine, starting with
       the oldest saved message first, and
       finishing with the latest.
       The window port routine is expected to
       load the message recall buffers in such
       a way that the ordering is preserved.
       The window port routine should make no
       assumptions about how many messages are
       forthcoming, nor should it assume that
       another message will follow this one,
       so it should keep all pointers/indexes
       intact at the end of each call.
    */

    /* this doesn't provide for reloading the message window with the
       previous session's messages upon restore, but it does put the quest
       message summary lines there by treating them as ordinary messages */
    if (!is_restoring)
        pline_ex(attrs[0], colors[0], "%s", msg);
    return;
}

#ifdef HANGUPHANDLING
/*
 * Dummy windowing scheme used to replace current one with no-ops
 * in order to avoid all terminal I/O after hangup/disconnect.
 */

STATIC_DCL int NDECL(hup_nhgetch);
STATIC_DCL char FDECL(hup_yn_function_ex, (int, int, int, int, const char *, const char *, const char *, CHAR_P, const char*, const char*, unsigned long));
STATIC_DCL int FDECL(hup_nh_poskey, (int *, int *, int *));
STATIC_DCL void FDECL(hup_getlin_ex, (int, int, int, const char *, char *, const char*, const char*, const char*));
STATIC_DCL void FDECL(hup_init_nhwindows, (int *, char **));
STATIC_DCL void FDECL(hup_exit_nhwindows, (const char *));
STATIC_DCL winid FDECL(hup_create_nhwindow_ex, (int, int, int, struct extended_create_window_info));
STATIC_DCL void FDECL(hup_start_menu_ex, (winid, int));
STATIC_DCL int FDECL(hup_select_menu, (winid, int, MENU_ITEM_P **));
STATIC_DCL void FDECL(hup_add_menu, (winid, int, const anything *, CHAR_P, CHAR_P,
                                 int, int, const char *, BOOLEAN_P));
STATIC_DCL void FDECL(hup_add_extended_menu, (winid, int, const anything*, CHAR_P, CHAR_P,
    int, int, const char*, BOOLEAN_P, struct extended_menu_info));
STATIC_DCL void FDECL(hup_end_menu_ex, (winid, const char *, const char*));
STATIC_DCL void FDECL(hup_putstr_ex, (winid, const char *, int, int, int));
STATIC_DCL void FDECL(hup_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));
STATIC_DCL void FDECL(hup_print_glyph, (winid, XCHAR_P, XCHAR_P, struct layer_info));
STATIC_DCL void FDECL(hup_issue_gui_command, (int, int, int, const char*));
STATIC_DCL void FDECL(hup_outrip, (winid, int, time_t));
STATIC_DCL void FDECL(hup_curs, (winid, int, int));
STATIC_DCL void FDECL(hup_display_nhwindow, (winid, BOOLEAN_P));
STATIC_DCL void FDECL(hup_display_file, (const char *, BOOLEAN_P));
#ifdef CLIPPING
STATIC_DCL void FDECL(hup_cliparound, (int, int, BOOLEAN_P));
#endif
#ifdef CHANGE_COLOR
STATIC_DCL void FDECL(hup_change_color, (int, long, int));
#ifdef MAC
STATIC_DCL short FDECL(hup_set_font_name, (winid, char *));
#endif
STATIC_DCL char *NDECL(hup_get_color_string);
#endif /* CHANGE_COLOR */
STATIC_DCL void FDECL(hup_status_update, (int, genericptr_t, int, int, int,
                                      unsigned long *));

STATIC_DCL int NDECL(hup_int_ndecl);
STATIC_DCL void NDECL(hup_void_ndecl);
STATIC_DCL void FDECL(hup_void_fdecl_int, (int));
STATIC_DCL void FDECL(hup_void_fdecl_winid, (winid));
STATIC_DCL void FDECL(hup_void_fdecl_constchar_p, (const char *));

STATIC_VAR struct window_procs hup_procs = {
    "hup", 0L, 0L, hup_init_nhwindows,
    hup_void_ndecl,                                    /* player_selection */
    hup_void_ndecl,                                    /* askname */
    hup_void_ndecl,                                    /* get_nh_event */
    hup_exit_nhwindows, hup_void_fdecl_constchar_p,    /* suspend_nhwindows */
    hup_void_ndecl,                                    /* resume_nhwindows */
    hup_create_nhwindow_ex, hup_void_fdecl_winid,         /* clear_nhwindow */
    hup_display_nhwindow, hup_void_fdecl_winid,        /* destroy_nhwindow */
    hup_curs, hup_putstr_ex, hup_putstr_ex2, hup_putstr_ex,            /* putmixed */
    hup_display_file, hup_start_menu_ex,               /* start_menu */
    hup_add_menu, hup_add_extended_menu, hup_end_menu_ex, hup_select_menu, genl_message_menu,
    hup_void_ndecl,                                    /* update_inventory */
    hup_void_ndecl,                                    /* mark_synch */
    hup_void_ndecl,                                    /* wait_synch */
#ifdef CLIPPING
    hup_cliparound,
#endif
#ifdef POSITIONBAR
    (void FDECL((*), (char *))) hup_void_fdecl_constchar_p,
                                                      /* update_positionbar */
#endif
    hup_print_glyph,
    hup_issue_gui_command,
    hup_void_fdecl_constchar_p,                       /* raw_print */
    hup_void_fdecl_constchar_p,                       /* raw_print_bold */
    hup_nhgetch, hup_nh_poskey, hup_void_ndecl,       /* nhbell  */
    hup_int_ndecl,                                    /* doprev_message */
    hup_yn_function_ex, hup_getlin_ex, hup_int_ndecl,       /* get_ext_cmd */
    hup_void_fdecl_int,                               /* number_pad */
    hup_void_ndecl,                                   /* delay_output  */
    hup_void_fdecl_int,                               /* delay_output_milliseconds */
    hup_void_fdecl_int,                               /* delay_output_frames */
#ifdef CHANGE_COLOR
    hup_change_color,
#ifdef MAC
    hup_void_fdecl_int,                               /* change_background */
    hup_set_font_name,
#endif
    hup_get_color_string,
#endif /* CHANGE_COLOR */
    hup_void_ndecl,                                   /* start_screen */
    hup_void_ndecl,                                   /* end_screen */
    hup_outrip, genl_preference_update, genl_getmsghistory_ex,
    genl_putmsghistory_ex,
    hup_void_fdecl_int,                               /* status_init */
    hup_void_ndecl,                                   /* status_finish */
    genl_status_enablefield, hup_status_update,
    genl_can_suspend_no,
};

STATIC_VAR void FDECL((*previnterface_exit_nhwindows), (const char *)) = 0;

/* hangup has occurred; switch to no-op user interface */
void
nhwindows_hangup()
{
    char *FDECL((*previnterface_getmsghistory_ex), (char**, char**, BOOLEAN_P)) = 0;

#ifdef ALTMETA
    /* command processor shouldn't look for 2nd char after seeing ESC */
    iflags.altmeta = FALSE;
#endif

    /* don't call exit_nhwindows() directly here; if a hangup occurs
       while interface code is executing, exit_nhwindows could knock
       the interface's active data structures out from under itself */
    if (iflags.window_inited
        && windowprocs.win_exit_nhwindows != hup_exit_nhwindows)
        previnterface_exit_nhwindows = windowprocs.win_exit_nhwindows;

    /* also, we have to leave the old interface's getmsghistory()
       in place because it will be called while saving the game */
    if (windowprocs.win_getmsghistory_ex != hup_procs.win_getmsghistory_ex)
        previnterface_getmsghistory_ex = windowprocs.win_getmsghistory_ex;

    windowprocs = hup_procs;

    if (previnterface_getmsghistory_ex)
        windowprocs.win_getmsghistory_ex = previnterface_getmsghistory_ex;
}

STATIC_OVL void
hup_exit_nhwindows(lastgasp)
const char *lastgasp;
{
    /* core has called exit_nhwindows(); call the previous interface's
       shutdown routine now; xxx_exit_nhwindows() needs to call other
       xxx_ routines directly rather than through windowprocs pointers */
    if (previnterface_exit_nhwindows) {
        lastgasp = 0; /* don't want exit routine to attempt extra output */
        (*previnterface_exit_nhwindows)(lastgasp);
        previnterface_exit_nhwindows = 0;
    }
    iflags.window_inited = 0;
}

STATIC_OVL int
hup_nhgetch(VOID_ARGS)
{
    return '\033'; /* ESC */
}

/*ARGSUSED*/
STATIC_OVL char
hup_yn_function_ex(style, attr, color, glyph, title, prompt, resp, deflt, resp_desc, introline, ynflags)
int style UNUSED, attr UNUSED, color UNUSED, glyph UNUSED;
const char *title UNUSED, *prompt UNUSED, *resp UNUSED, *resp_desc UNUSED, *introline UNUSED;
char deflt;
unsigned long ynflags UNUSED;
{
    if (!deflt)
        deflt = '\033';
    return deflt;
}

/*ARGSUSED*/
STATIC_OVL int
hup_nh_poskey(x, y, mod)
int *x UNUSED, *y UNUSED, *mod UNUSED;
{
    return '\033';
}

/*ARGSUSED*/
STATIC_OVL void
hup_getlin_ex(style, attr, color, prompt, outbuf, placeholder, linesuffix, introline)
int style UNUSED, attr UNUSED, color UNUSED;
const char *prompt UNUSED;
const char* placeholder UNUSED;
const char* linesuffix UNUSED;
const char* introline UNUSED;
char *outbuf;
{
    Strcpy(outbuf, "\033");
}

/*ARGSUSED*/
STATIC_OVL void
hup_init_nhwindows(argc_p, argv)
int *argc_p UNUSED;
char **argv UNUSED;
{
    iflags.window_inited = 1;
}

/*ARGUSED*/
STATIC_OVL winid
hup_create_nhwindow_ex(type, style, glyph, info)
int type UNUSED;
int style UNUSED;
int glyph UNUSED;
struct extended_create_window_info info UNUSED;
{
    return WIN_ERR;
}

/*ARGUSED*/
STATIC_OVL void
hup_start_menu_ex(window, style)
winid window UNUSED;
int style UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL int
hup_select_menu(window, how, menu_list)
winid window UNUSED;
int how UNUSED;
struct mi **menu_list UNUSED;
{
    return -1;
}

/*ARGSUSED*/
STATIC_OVL void
hup_add_menu(window, glyph, identifier, sel, grpsel, attr, color, txt, preselected)
winid window UNUSED;
int glyph UNUSED, attr UNUSED, color UNUSED;
const anything *identifier UNUSED;
char sel UNUSED, grpsel UNUSED;
const char *txt UNUSED;
boolean preselected UNUSED;
{
    return;
}

STATIC_OVL void
hup_add_extended_menu(window, glyph, identifier, sel, grpsel, attr, color, txt, preselected, info)
winid window UNUSED;
int glyph UNUSED, attr UNUSED, color UNUSED;
const anything* identifier UNUSED;
char sel UNUSED, grpsel UNUSED;
const char* txt UNUSED;
boolean preselected UNUSED;
struct extended_menu_info info UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_end_menu_ex(window, prompt, subtitle)
winid window UNUSED;
const char *prompt UNUSED;
const char *subtitle UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_putstr_ex(window, text, attr, color, app)
winid window UNUSED;
int attr UNUSED, app UNUSED, color UNUSED;
const char *text UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_putstr_ex2(window, text, attrs, colors, attr, color, app)
winid window UNUSED;
int attr UNUSED, color UNUSED, app UNUSED;
const char* text UNUSED, *attrs UNUSED, *colors UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_print_glyph(window, x, y, layers)
winid window UNUSED;
xchar x UNUSED, y UNUSED;
struct layer_info layers UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_issue_gui_command(cmd_id, cmd_param, cmd_param2, cmd_str)
int cmd_id UNUSED, cmd_param UNUSED, cmd_param2 UNUSED;
const char* cmd_str UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_outrip(tmpwin, how, when)
winid tmpwin UNUSED;
int how UNUSED;
time_t when UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_curs(window, x, y)
winid window UNUSED;
int x UNUSED, y UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_display_nhwindow(window, blocking)
winid window UNUSED;
boolean blocking UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
hup_display_file(fname, complain)
const char *fname UNUSED;
boolean complain UNUSED;
{
    return;
}

#ifdef CLIPPING
/*ARGSUSED*/
STATIC_OVL void
hup_cliparound(x, y, force)
int x UNUSED, y UNUSED;
boolean force UNUSED;
{
    return;
}
#endif

#ifdef CHANGE_COLOR
/*ARGSUSED*/
STATIC_OVL void
hup_change_color(color, rgb, reverse)
int color, reverse;
long rgb;
{
    return;
}

#ifdef MAC
/*ARGSUSED*/
STATIC_OVL short
hup_set_font_name(window, fontname)
winid window;
char *fontname;
{
    return 0;
}
#endif /* MAC */

STATIC_OVL char *
hup_get_color_string(VOID_ARGS)
{
    return (char *) 0;
}
#endif /* CHANGE_COLOR */

/*ARGSUSED*/
STATIC_OVL void
hup_status_update(idx, ptr, chg, pc, color, colormasks)
int idx UNUSED;
genericptr_t ptr UNUSED;
int chg UNUSED, pc UNUSED, color UNUSED;
unsigned long *colormasks UNUSED;

{
    return;
}

/*
 * Non-specific stubs.
 */

STATIC_OVL int
hup_int_ndecl(VOID_ARGS)
{
    return -1;
}

STATIC_OVL void
hup_void_ndecl(VOID_ARGS)
{
    return;
}

/*ARGUSED*/
STATIC_OVL void
hup_void_fdecl_int(arg)
int arg UNUSED;
{
    return;
}

/*ARGUSED*/
STATIC_OVL void
hup_void_fdecl_winid(window)
winid window UNUSED;
{
    return;
}

/*ARGUSED*/
STATIC_OVL void
hup_void_fdecl_constchar_p(string)
const char *string UNUSED;
{
    return;
}

#endif /* HANGUPHANDLING */


/****************************************************************************/
/* genl backward compat stuff                                               */
/****************************************************************************/

const char *status_fieldnm[MAXBLSTATS];
const char *status_fieldfmt[MAXBLSTATS];
char *status_vals[MAXBLSTATS];
boolean status_activefields[MAXBLSTATS];
//NEARDATA winid WIN_STATUS;

void
genl_status_init(reassessment)
int reassessment;
{
    if (reassessment)
        return;

    int i;

    for (i = 0; i < MAXBLSTATS; ++i) {
        status_vals[i] = (char *) alloc(MAXCO);
        *status_vals[i] = '\0';
        status_activefields[i] = FALSE;
        status_fieldfmt[i] = (const char *) 0;
    }
    /* Use a window for the genl version; backward port compatibility */
    WIN_STATUS = create_nhwindow(NHW_STATUS);
    display_nhwindow(WIN_STATUS, FALSE);
}

void
genl_status_finish(VOID_ARGS)
{
    /* tear down routine */
    int i;

    /* free alloc'd memory here */
    for (i = 0; i < MAXBLSTATS; ++i) {
        if (status_vals[i])
            free((genericptr_t) status_vals[i]), status_vals[i] = (char *) 0;
    }
}

void
genl_status_enablefield(fieldidx, nm, fmt, enable)
int fieldidx;
const char *nm;
const char *fmt;
boolean enable;
{
    status_fieldfmt[fieldidx] = fmt;
    status_fieldnm[fieldidx] = nm;
    status_activefields[fieldidx] = enable;
}

/* call once for each field, then call with BL_FLUSH to output the result */
void
genl_status_update(idx, ptr, chg, percent, color, colormasks)
int idx;
genericptr_t ptr;
int chg UNUSED, percent UNUSED, color UNUSED;
unsigned long *colormasks UNUSED;
{
    char newbot1[MAXCO], newbot2[MAXCO];
    long cond, *condptr = (long *) ptr;
    register int i;
    unsigned pass, lndelta;
    enum statusfields idx1, idx2, *fieldlist;
    char *nb, *text = (char *) ptr;

    static enum statusfields gsu_fieldorder[][24] = {
        /* line one */
        { BL_TITLE, BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH, BL_GOLD, //BL_ALIGN,
          BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH,
          BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH },
        /* line two, default order */
        { BL_MODE, BL_LEVELDESC, // BL_GOLD,
          BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_UQUIVER, BL_XP, BL_EXP, BL_HD,
          BL_TIME, BL_REALTIME,
          BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_FLUSH },
        /* move time to the end */
        { BL_MODE, BL_LEVELDESC, //BL_GOLD,
          BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_UQUIVER, BL_XP, BL_EXP, BL_HD,
          BL_2WEP, BL_SKILL,BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_TIME, BL_REALTIME, BL_FLUSH },
        /* move experience and time to the end */
        { BL_MODE, BL_LEVELDESC, // BL_GOLD,
          BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_UQUIVER, BL_XP, BL_EXP, BL_HD, BL_TIME, BL_REALTIME, BL_FLUSH },
        /* move level description plus gold and experience and time to end */
        { BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_MODE, BL_LEVELDESC, //BL_GOLD,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_UQUIVER, BL_XP, BL_EXP, BL_HD, BL_TIME, BL_REALTIME, BL_FLUSH },
    };

    /* in case interface is using genl_status_update() but has not
       specified WC2_FLUSH_STATUS (status_update() for field values
       is buffered so final BL_FLUSH is needed to produce output) */
    windowprocs.wincap2 |= WC2_FLUSH_STATUS;

    if (idx >= 0) {
        if (!status_activefields[idx])
            return;
        switch (idx) {
        case BL_CONDITION:
            cond = condptr ? *condptr : 0L;
            nb = status_vals[idx];
            *nb = '\0';
            if (cond & BL_MASK_GRAB)
                Strcpy(nb = eos(nb), " Grab");
            if (cond & BL_MASK_STONE)
                Strcpy(nb = eos(nb), " Stone");
            if (cond & BL_MASK_SLIME)
                Strcpy(nb = eos(nb), " Slime");
            if (cond & BL_MASK_STRNGL)
                Strcpy(nb = eos(nb), " Strngl");
            if (cond & BL_MASK_SUFFOC)
                Strcpy(nb = eos(nb), " Suffoc");
            if (cond & BL_MASK_FOODPOIS)
                Strcpy(nb = eos(nb), " FoodPois");
            if (cond & BL_MASK_TERMILL)
                Strcpy(nb = eos(nb), " TermIll");
            if (cond & BL_MASK_ROT)
                Strcpy(nb = eos(nb), " Rot");
            if (cond & BL_MASK_LYCANTHROPY)
                Strcpy(nb = eos(nb), " Lyca");
            if (cond & BL_MASK_PARALYZED)
                Strcpy(nb = eos(nb), " Paral");
            if (cond & BL_MASK_FEARFUL)
                Strcpy(nb = eos(nb), " Fear");
            if (cond & BL_MASK_SLEEPING)
                Strcpy(nb = eos(nb), " Sleep");
            if (cond & BL_MASK_BLIND)
                Strcpy(nb = eos(nb), " Blind");
            if (cond & BL_MASK_DEAF)
                Strcpy(nb = eos(nb), " Deaf");
            if (cond & BL_MASK_STUN)
                Strcpy(nb = eos(nb), " Stun");
            if (cond & BL_MASK_CONF)
                Strcpy(nb = eos(nb), " Conf");
            if (cond & BL_MASK_HALLU)
                Strcpy(nb = eos(nb), " Hallu");
            if (cond & BL_MASK_SLOWED)
                Strcpy(nb = eos(nb), " Slow");
            if (cond & BL_MASK_SILENCED)
                Strcpy(nb = eos(nb), " Silent");
            if (cond & BL_MASK_CANCELLED)
                Strcpy(nb = eos(nb), " Cancl");
            if (cond & BL_MASK_LEV)
                Strcpy(nb = eos(nb), " Lev");
            if (cond & BL_MASK_FLY)
                Strcpy(nb = eos(nb), " Fly");
            if (cond & BL_MASK_RIDE)
                Strcpy(nb = eos(nb), " Ride");
            break;
        default:
            Sprintf(status_vals[idx],
                    status_fieldfmt[idx] ? status_fieldfmt[idx] : "%s",
                    text ? text : "");
            break;
        }
        return; /* processed one field other than BL_FLUSH */
    } /* (idx >= 0, thus not BL_FLUSH, BL_RESET, BL_CHARACTERISTICS) */

    /* does BL_RESET require any specific code to ensure all fields ? */

    if (!(idx == BL_FLUSH || idx == BL_RESET))
        return;

    /* We've received BL_FLUSH; time to output the gathered data */
    nb = newbot1;
    *nb = '\0';
    /* BL_FLUSH is the only pseudo-index value we need to check for
       in the loop below because it is the only entry used to pad the
       end of the gsu_fieldorder array. We could stop on any
       negative (illegal) index, but this should be fine */
    for (i = 0; (idx1 = gsu_fieldorder[0][i]) != BL_FLUSH; ++i) {
        if (status_activefields[idx1])
            Strcpy(nb = eos(nb), status_vals[idx1]);
    }
    /* if '$' is encoded, buffer length of \GXXXXNNNN is 9 greater than
       single char; we want to subtract that 9 when checking display length */
    lndelta = 0; // (status_activefields[BL_GOLD]
               //&& strstr(status_vals[BL_GOLD], "\\G")) ? 9 : 0;
    /* basic bot2 formats groups of second line fields into five buffers,
       then decides how to order those buffers based on comparing lengths
       of [sub]sets of them to the width of the map; we have more control
       here but currently emulate that behavior */
    for (pass = 1; pass <= 4; pass++) {
        fieldlist = gsu_fieldorder[pass];
        nb = newbot2;
        *nb = '\0';
        for (i = 0; (idx2 = fieldlist[i]) != BL_FLUSH; ++i) {
            if (status_activefields[idx2]) {
                const char *val = status_vals[idx2];

                switch (idx2) {
                case BL_HP: /* for pass 4, Hp comes first; mungspaces()
                               will strip the unwanted leading spaces */
                case BL_XP: 
                case BL_HD:
                case BL_MOVE:
                case BL_UWEP:
                case BL_TIME:
                case BL_REALTIME:
                    Strcpy(nb = eos(nb), " ");
                    break;
                case BL_MODE:
                case BL_LEVELDESC:
                    /* leveldesc has no leading space, so if we've moved
                       it past the first position, provide one */
                    if (i != 0)
                        Strcpy(nb = eos(nb), " ");
                    break;
                /*
                 * We want "  hunger encumbrance conditions"
                 *   or    "  encumbrance conditions"
                 *   or    "  hunger conditions"
                 *   or    "  conditions"
                 * 'hunger'      is either " " or " hunger_text";
                 * 'encumbrance' is either " " or " encumbrance_text";
                 * 'conditions'  is either ""  or " cond1 cond2...".
                 */
                case BL_HUNGER:
                    /* hunger==" " - keep it, end up with " ";
                       hunger!=" " - insert space and get "  hunger" */
                    if (strcmp(val, " "))
                        Strcpy(nb = eos(nb), " ");
                    break;
                case BL_SKILL:
                    /* skill==" " - keep it, end up with " ";
                       skill!=" " - insert space and get "  skill" */
                    if (strcmp(val, " "))
                        Strcpy(nb = eos(nb), " ");
                    break;
                case BL_2WEP:
                    /* skill==" " - keep it, end up with " ";
                       skill!=" " - insert space and get "  skill" */
                    if (strcmp(val, " "))
                        ++val;
                    break;
                case BL_CAP:
                    /* cap==" " - suppress it, retain "  hunger" or " ";
                       cap!=" " - use it, get "  hunger cap" or "  cap" */
                    if (!strcmp(val, " "))
                        ++val;
                    break;
                default:
                    break;
                }
                Strcpy(nb = eos(nb), val); /* status_vals[idx2] */
            } /* status_activefields[idx2] */

            if (idx2 == BL_CONDITION && pass < 4
                && strlen(newbot2) - lndelta > COLNO)
                break; /* switch to next order */
        } /* i */

        if (idx2 == BL_FLUSH) { /* made it past BL_CONDITION */
            if (pass > 1)
                mungspaces(newbot2);
            break;
        }
    } /* pass */
    curs(WIN_STATUS, 1, 0);
    putstr(WIN_STATUS, 0, newbot1);
    curs(WIN_STATUS, 1, 1);
    putmixed(WIN_STATUS, 0, newbot2); /* putmixed() due to MAT_GOLD glyph */
}

#if defined (DUMPLOG) || defined (DUMPHTML)
STATIC_VAR struct window_procs dumplog_windowprocs_backup;
STATIC_VAR int menu_headings_backup;
STATIC_VAR time_t dumplog_now;

#ifdef DUMPLOG
char*
print_dumplog_filename_to_buffer(buf)
char* buf;
{
    char* fname;

#ifdef SYSCF
    if (!sysopt.dumplogfile)
        return 0;
    fname = dump_fmtstr(sysopt.dumplogfile, buf);
#elif defined(ANDROID)
    if (iflags.dumplog)
    {
        char buf_[BUFSZ];
        dump_fmtstr(DUMPLOG_FILE, buf_);
        and_get_dumplog_dir(buf);
        if (strlen(buf_) + strlen(buf) < BUFSZ - 1)
            fname = strcat(buf, buf_);
        else
            fname = strcpy(buf, buf_);
    }
    else
        fname = 0;
#else
    fname = dump_fmtstr(DUMPLOG_FILE, buf);
#endif
    return fname;
}
#endif

#ifdef DUMPHTML
char*
print_dumphtml_filename_to_buffer(buf)
char* buf;
{
    char* fname;

#ifdef SYSCF
    if (!sysopt.dumphtmlfile)
        return 0;
    fname = dump_fmtstr(sysopt.dumphtmlfile, buf);
#elif defined(ANDROID)
    if (iflags.dumplog)
    {
        char buf_[BUFSZ];
        dump_fmtstr(DUMPHTML_FILE, buf_);
        and_get_dumplog_dir(buf);
        if (strlen(buf_) + strlen(buf) < BUFSZ - 1)
            fname = strcat(buf, buf_);
        else
            fname = strcpy(buf, buf_);
    }
    else
        fname = 0;
#else
    fname = dump_fmtstr(DUMPHTML_FILE, buf);
#endif
    return fname;
}
#endif

char *
dump_fmtstr(fmt, buf)
const char *fmt;
char *buf;
{
    const char *fp = fmt;
    char *bp = buf;
    size_t slen, len = 0;
    char tmpbuf[BUFSZ];
    char verbuf[BUFSZ];
    long uid;
    time_t now;

    now = dumplog_now;
    uid = (long) getuid();

    /*
     * Note: %t and %T assume that time_t is a 'long int' number of
     * seconds since some epoch value.  That's quite iffy....  The
     * unit of time might be different and the datum size might be
     * some variant of 'long long int'.  [Their main purpose is to
     * construct a unique file name rather than record the date and
     * time; violating the 'long seconds since base-date' assumption
     * may or may not interfere with that usage.]
     */

    while (fp && *fp && len < BUFSZ-1) {
        if (*fp == '%') {
            fp++;
            switch (*fp) {
            default:
                goto finish;
            case '\0': /* fallthrough */
            case '%':  /* literal % */
                Sprintf(tmpbuf, "%%");
                break;
            case 't': /* game start, timestamp */
                Sprintf(tmpbuf, "%lu", (unsigned long) ubirthday);
                break;
            case 'T': /* current time, timestamp */
                Sprintf(tmpbuf, "%lu", (unsigned long) now);
                break;
            case 'd': /* game start, YYYYMMDDhhmmss */
                Sprintf(tmpbuf, "%08ld%06ld",
                        yyyymmdd(ubirthday), hhmmss(ubirthday));
                break;
            case 'D': /* current time, YYYYMMDDhhmmss */
                Sprintf(tmpbuf, "%08ld%06ld", yyyymmdd(now), hhmmss(now));
                break;
            case 'v': /* version, eg. "3.6.2-0" */
                Sprintf(tmpbuf, "%s", version_string(verbuf));
                break;
            case 'u': /* UID */
                Sprintf(tmpbuf, "%ld", uid);
                break;
            case 'n': /* player name */
                Sprintf(tmpbuf, "%s", *plname ? plname : "unknown");
                break;
            case 'N': /* first character of player name */
                Sprintf(tmpbuf, "%c", *plname ? *plname : 'u');
                break;
            }

            (void)strNsubst(tmpbuf, " ", "_", 0);
            (void)strNsubst(tmpbuf, "/", "_", 0);
            (void)strNsubst(tmpbuf, "\\", "_", 0);

            slen = strlen(tmpbuf);
            if (len + slen < BUFSZ-1) {
                len += slen;
                Sprintf(bp, "%s", tmpbuf);
                bp += slen;
                if (*fp) fp++;
            } else
                break;
        } else {
            *bp = *fp;
            bp++;
            fp++;
            len++;
        }
    }
 finish:
    *bp = '\0';
    return buf;
}

struct dump_status_fields {
    int idx;
    int color;
    int attr;
};

static const enum statusfields** fieldorder;
static unsigned long* dump_colormasks;
static long dump_condition_bits;
static struct dump_status_fields dump_status[MAXBLSTATS];
static int hpbar_percent, hpbar_color;

/* condcolor and condattr are needed to render the HTML status bar.
   These static routines exist verbatim in at least two other window
   ports. They should be promoted to the core (maybe botl.c).
   Please delete this comment after the above suggestion has been enacted
   or ignored.
 */

static int
condcolor(bm, bmarray)
long bm;
unsigned long* bmarray;
{
#if defined(STATUS_HILITES) && defined(TEXTCOLOR)
    int i;

    if (bm && bmarray)
        for (i = 0; i < CLR_MAX; ++i) {
            if ((bmarray[i] & bm) != 0)
                return i;
        }
#endif
    return NO_COLOR;
}

STATIC_OVL int
condattr(bm, bmarray)
long bm;
unsigned long* bmarray;
{
    int attr = 0;
#ifdef STATUS_HILITES
    int i;

    if (bm && bmarray) {
        for (i = HL_ATTCLR_DIM; i < BL_ATTCLR_MAX; ++i) {
            if ((bmarray[i] & bm) != 0) {
                switch (i) {
                case HL_ATTCLR_DIM:
                    attr |= HL_DIM;
                    break;
                case HL_ATTCLR_BLINK:
                    attr |= HL_BLINK;
                    break;
                case HL_ATTCLR_ULINE:
                    attr |= HL_ULINE;
                    break;
                case HL_ATTCLR_INVERSE:
                    attr |= HL_INVERSE;
                    break;
                case HL_ATTCLR_BOLD:
                    attr |= HL_BOLD;
                    break;
                default:
                    break;
                }
            }
        }
    }
#endif /* STATUS_HILITES */
    return attr;
}



STATIC_OVL void
dump_render_status(VOID_ARGS)
{
    long mask, bits;
    int i, idx, c, row, num_rows, coloridx = 0, attrmask = 0;
    char* text;

    num_rows = iflags.wc2_statuslines < 3 ? 2 : iflags.wc2_statuslines > 8 ? 8 : iflags.wc2_statuslines;
    for (row = 0; row < num_rows; ++row) {
        if (fieldorder[row] == NULL)
            break;

        int pad = COLNO + 1;
        if (dumphtml_file)
            fprintf(dumphtml_file, "<div class=\"nh_screen\">  "); /* 2 space left margin */
        for (i = 0; (idx = fieldorder[row][i]) != BL_FLUSH; ++i) {
            boolean hitpointbar = (idx == BL_TITLE
                && iflags.wc2_hitpointbar);

            if (!status_activefields[idx])
                continue;
            text = status_vals[idx]; /* always "" for BL_CONDITION */

            if (idx == BL_CONDITION) {
                /* | Condition Codes | */
                bits = dump_condition_bits;
                for (c = 0; c < SIZE(condition_definitions) && bits != 0L; ++c) {
                    mask = condition_definitions[c].mask;
                    if (bits & mask) {
                        putstr_ex(NHW_STATUS, " ", ATR_NONE, NO_COLOR, 1);
                        pad--;
#ifdef STATUS_HILITES
                        if (iflags.hilite_delta) {
                            attrmask = condattr(mask, dump_colormasks);
                            coloridx = condcolor(mask, dump_colormasks);
                            dump_set_color_attr(coloridx, attrmask, TRUE, FALSE);
                        }
#endif
                        putstr_ex(NHW_STATUS, condition_definitions[c].text[0], ATR_NONE, NO_COLOR, 1);
                        pad -= strlen(condition_definitions[c].text[0]);
#ifdef STATUS_HILITES
                        if (iflags.hilite_delta) {
                            dump_set_color_attr(coloridx, attrmask, FALSE, FALSE);
                        }
#endif
                        bits &= ~mask;
                    }
                }
            }
            else if (hitpointbar) {
                /* | Title with Hitpoint Bar | */
                /* hitpointbar using hp percent calculation */
                int bar_len, bar_pos = 0;
                char bar[MAXCO], * bar2 = (char*)0, savedch = '\0';
                boolean twoparts = (hpbar_percent < 100);

                /* force exactly 30 characters, padded with spaces
                   if shorter or truncated if longer */
                if (strlen(text) != 30) {
                    Sprintf(bar, "%-30.30s", text);
                    Strcpy(status_vals[BL_TITLE], bar);
                }
                else
                    Strcpy(bar, text);
                bar_len = (int)strlen(bar); /* always 30 */
                /* when at full HP, the whole title will be highlighted;
                   when injured or dead, there will be a second portion
                   which is not highlighted */
                if (twoparts) {
                    /* figure out where to separate the two parts */
                    bar_pos = (bar_len * hpbar_percent) / 100;
                    if (bar_pos < 1 && hpbar_percent > 0)
                        bar_pos = 1;
                    if (bar_pos >= bar_len && hpbar_percent < 100)
                        bar_pos = bar_len - 1;
                    bar2 = &bar[bar_pos];
                    savedch = *bar2;
                    *bar2 = '\0';
                }
                putstr_ex(NHW_STATUS,  "[", ATR_NONE, NO_COLOR, 1);
                if (*bar) { /* always True, unless twoparts+dead (0 HP) */
                    dump_set_color_attr(hpbar_color, HL_INVERSE, TRUE, FALSE);
                    putstr_ex(NHW_STATUS, bar, ATR_NONE, NO_COLOR, 0);
                    dump_set_color_attr(hpbar_color, HL_INVERSE, FALSE, FALSE);
                }
                if (twoparts) { /* no highlighting for second part */
                    *bar2 = savedch;
                    putstr_ex(NHW_STATUS, bar2, ATR_NONE, NO_COLOR, 1);
                }
                putstr_ex(NHW_STATUS, "]", ATR_NONE, NO_COLOR, 1);
                pad -= (bar_len + 2);
            }
            else {
                /* | Everything else not in a special case above | */
#ifdef STATUS_HILITES
                if (iflags.hilite_delta) {
                    while (*text == ' ') {
                        putstr(NHW_STATUS, 0, " ");
                        text++;
                        pad--;
                    }
                    if (*text == '/' && idx == BL_EXP) {
                        putstr(NHW_STATUS, 0, "/");
                        text++;
                        pad--;
                    }
                    attrmask = dump_status[idx].attr;
                    coloridx = dump_status[idx].color;
                    dump_set_color_attr(coloridx, attrmask, TRUE, FALSE);
                }
#endif
                putstr_ex(NHW_STATUS, text, ATR_NONE, NO_COLOR, 1);
                pad -= strlen(text);
#ifdef STATUS_HILITES
                if (iflags.hilite_delta) {
                    dump_set_color_attr(coloridx, attrmask, FALSE, FALSE);
                }
#endif
            }
        }
        if (dumphtml_file)
            fprintf(dumphtml_file, "%*s</div>\n", pad, " ");
    }
    return;
}


STATIC_OVL void
dump_status_update(fldidx, ptr, chg, percent, color, colormasks)
int fldidx, chg UNUSED, percent, color;
genericptr_t ptr;
unsigned long* colormasks;
{
    int attrmask;
    long* condptr = (long*)ptr;
    char* text = (char*)ptr;
    char* lastchar, * p;
    char goldbuf[40] = "";
    const char* fmt;

    static boolean inited = FALSE;

    if (!inited) {
        int i;
        fieldorder = iflags.wc2_statuslines < 3 ? fieldorders_2statuslines : flags.fullstatuslineorder ? fieldorders_alt : fieldorders;
        for (i = 0; i < MAXBLSTATS; ++i) {
            dump_status[i].idx = BL_FLUSH;
            dump_status[i].color = NO_COLOR;
            dump_status[i].attr = ATR_NONE;
        }
        dump_condition_bits = 0L;
        hpbar_percent = 0, hpbar_color = NO_COLOR;
        inited = TRUE;
    }

    if ((fldidx < BL_RESET) || (fldidx >= MAXBLSTATS))
        return;

    if ((fldidx >= 0 && fldidx < MAXBLSTATS) && !status_activefields[fldidx])
        return;

    switch (fldidx) {
    case BL_RESET:
    case BL_FLUSH:
        dump_render_status();
        return;
    case BL_CONDITION:
        dump_status[fldidx].idx = fldidx;
        dump_condition_bits = *condptr;
        dump_colormasks = colormasks;
        break;
    case BL_GOLD:
        text = decode_mixed(goldbuf, text);
        /*FALLTHRU*/
    default:
        attrmask = (color >> 8) & 0x00FF;
#ifndef TEXTCOLOR
        color = NO_COLOR;
#endif
        fmt = status_fieldfmt[fldidx];
        if (!fmt)
            fmt = "%s";
        /* should be checking for first enabled field here rather than
           just first field, but 'fieldorder' doesn't start any rows
           with fields which can be disabled so [any_row][0] suffices */
        if (*fmt == ' ' && ((fieldorder[0] != NULL && fldidx == fieldorder[0][0])
            || (fieldorder[1] != NULL && fldidx == fieldorder[1][0])
            || (fieldorder[2] != NULL && fldidx == fieldorder[2][0])
            || (fieldorder[3] != NULL && fldidx == fieldorder[3][0])
            || (fieldorder[4] != NULL && fldidx == fieldorder[4][0])
            || (fieldorder[5] != NULL && fldidx == fieldorder[5][0])
            || (fieldorder[6] != NULL && fldidx == fieldorder[6][0])
            || (fieldorder[7] != NULL && fldidx == fieldorder[7][0])
            ))
            ++fmt; /* skip leading space for first field on line */
        Sprintf(status_vals[fldidx], fmt, text);
        dump_status[fldidx].idx = fldidx;
        dump_status[fldidx].color = (color & 0x00FF);
        dump_status[fldidx].attr = attrmask;
        break;
    }

    /* default processing above was required before these */
    switch (fldidx) {
    case BL_HP:
    case BL_HPMAX:
        if (iflags.wc2_hitpointbar) {
            /* Special additional processing for hitpointbar */
            hpbar_percent = percent;
            if (fldidx == BL_HP)
            {
                hpbar_color = (color & 0x00FF);
                dump_status[BL_TITLE].color = hpbar_color;
            }
        }
        break;
    case BL_LEVELDESC:
    case BL_2WEP:
    case BL_SKILL:
    case BL_CAP:
    case BL_HUNGER:
        /* The core sends trailing blanks for some fields.
           Let's suppress the trailing blanks */
        p = status_vals[fldidx];
        for (lastchar = eos(p); lastchar > p && *--lastchar == ' '; ) {
            *lastchar = '\0';
        }
        break;
    }
    /* 3.6.2 we only render on BL_FLUSH (or BL_RESET) */
    return;
}



/*ARGSUSED*/
STATIC_OVL void
dump_putstr_ex(win, str, attr, color, app)
#ifdef DUMPHTML
winid win;
#else
winid win UNUSED;
#endif
int attr UNUSED, app UNUSED, color UNUSED;
const char *str;
{
    char buf[UTF8BUFSZ * 2] = "";
    if(str)
        write_text2buf_utf8(buf, sizeof(buf), str);

#ifdef DUMPLOG
    if (dumplog_file && win != NHW_STATUS)
        fprintf(dumplog_file, "%s%s", buf, app ? "" : "\n");
#endif
#ifdef DUMPHTML
    if (dumphtml_file && win != NHW_DUMPTXT) {
        if (win == NHW_STATUS)
            html_dump_str(dumphtml_file, str, 0, 0, attr, color);
        else
            html_dump_line(dumphtml_file, win, 0, 0, attr, color, app, str);
    }
#endif
}

/*ARGSUSED*/
STATIC_OVL void
dump_putstr_ex2(win, str, attrs, colors, attr, color, app)
winid win;
int attr, color, app;
const char* str, *attrs, *colors;
{
    char buf[UTF8BUFSZ * 2] = "";
    if (str)
        write_text2buf_utf8(buf, sizeof(buf), str);

#ifdef DUMPLOG
    if (dumplog_file && win != NHW_STATUS)
        fprintf(dumplog_file, "%s%s", buf, app ? "" : "\n");
#endif
#ifdef DUMPHTML
    if (dumphtml_file && win != NHW_DUMPTXT) {
        if (win == NHW_STATUS)
            html_dump_str(dumphtml_file, str, attrs, colors, attr, color);
        else
            html_dump_line(dumphtml_file, win, attrs, colors, attr, color, app, str);
    }
#endif
}

/*ARGSUSED*/
void
dump_putstr_no_utf8(win, attr, str)
winid win UNUSED;
int attr UNUSED;
const char* str;
{
#ifdef DUMPLOG
    if (dumplog_file)
        fprintf(dumplog_file, "%s\n", str);
#endif
}

STATIC_OVL winid
dump_create_nhwindow_ex(dummy, style, glyph, info)
int dummy;
int style UNUSED;
int glyph UNUSED;
struct extended_create_window_info info UNUSED;
{
    return dummy;
}

/*ARGUSED*/
STATIC_OVL void
dump_clear_nhwindow(win)
winid win UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
dump_display_nhwindow(win, p)
winid win UNUSED;
boolean p UNUSED;
{
    return;
}

/*ARGUSED*/
STATIC_OVL void
dump_destroy_nhwindow(win)
winid win UNUSED;
{
    return;
}

/*ARGUSED*/
STATIC_OVL void
dump_start_menu_ex(win, style)
winid win UNUSED;
int style UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL void
dump_add_menu(win, glyph, identifier, ch, gch, attr, color, str, preselected)
winid win;
int glyph;
const anything *identifier;
char ch;
char gch;
int attr;
int color;
const char *str;
boolean preselected;
{
    dump_add_extended_menu(win, glyph, identifier, ch, gch, attr, color, str, preselected, zeroextendedmenuinfo);
}

/*ARGSUSED*/
STATIC_OVL void
dump_add_extended_menu(win, glyph, identifier, ch, gch, attr, color, str, preselected, info)
winid win UNUSED;
int glyph UNUSED;
const anything* identifier UNUSED;
char ch UNUSED;
char gch UNUSED;
int attr UNUSED;
int color UNUSED;
const char* str;
boolean preselected UNUSED;
struct extended_menu_info info UNUSED;
{
    if (!str || ((!*str || !strcmp(str, " ")) && (attr & ATR_HALF_SIZE)))
        return;

    char buf[UTF8BUFSZ * 2] = "";
    if (str)
        write_text2buf_utf8(buf, sizeof(buf), str);

#ifdef DUMPLOG
    if (dumplog_file) {
        if (glyph == NO_GLYPH)
            fprintf(dumplog_file, " %s\n", buf);
        else
            fprintf(dumplog_file, "  %c - %s\n", ch, buf);
    }
#endif
#ifdef DUMPHTML
    if (dumphtml_file) {
        int htmlcolor;
        boolean iscolor = FALSE;
        /* Don't use NHW_MENU for inv items as this makes bullet points */
        if (!attr && glyph != NO_GLYPH)
            win = (winid)0;
        if (glyph != NO_GLYPH)
            fprintf(dumphtml_file, "%s", "<div class=\"nh_menu_item\">");
        else
            fprintf(dumphtml_file, "%s", "<div>");
        html_write_tags(dumphtml_file, win, attr, color, 0, TRUE, info, FALSE);
        if (iflags.use_menu_color && get_menu_coloring(str, &htmlcolor, &attr)) {
            iscolor = TRUE;
        }
        else if (color != NO_COLOR)
        {
            htmlcolor = color;
            iscolor = TRUE;
        }

        if (iscolor)
            fprintf(dumphtml_file, "<span class=\"nh_color_%d\">", htmlcolor);

        if (glyph != NO_GLYPH) {
            fprintf(dumphtml_file, "%c - ", ch);
        }
        html_dump_str(dumphtml_file, str, 0, 0, ATR_NONE, NO_COLOR);
        fprintf(dumphtml_file, "%s", iscolor ? "</span>" : "");
        html_write_tags(dumphtml_file, win, attr, color, 0, FALSE, info, FALSE);
        fprintf(dumphtml_file, "%s\n", "</div>");
    }
#endif
}

/*ARGSUSED*/
STATIC_OVL void
dump_end_menu_ex(win, str, str2)
winid win UNUSED;
const char *str UNUSED, *str2 UNUSED;
{
#ifdef DUMPLOG
    if (dumplog_file) 
    {
        fputs("\n", dumplog_file);
    }
#endif

//    char buf[UTF8BUFSZ * 4 + 3] = "";
//    char buf1[UTF8BUFSZ * 2] = "";
//    char buf2[UTF8BUFSZ * 2] = "";
//    const char* txt = 0;
//    txt = (str && str2) ? " - " : "";
//
//    if (str)
//        write_text2buf_utf8(buf1, sizeof(buf1), str);
//    if (str2)
//        write_text2buf_utf8(buf2, sizeof(buf2), str2);
//
//    Sprintf(buf, "%s%s%s", buf1, txt, buf2);
//
//#ifdef DUMPLOG
//    if (dumplog_file) 
//    {
//        if (str || str2)
//            fprintf(dumplog_file, "%s\n", buf);
//        else
//            fputs("\n", dumplog_file);
//    }
//#endif
//#ifdef DUMPHTML
//    if (dumphtml_file && (str || str2))
//        html_dump_line(dumphtml_file, 0, 0, 0, 0, 0, 0, buf);
//#endif
}

STATIC_OVL int
dump_select_menu(win, how, item)
winid win UNUSED;
int how UNUSED;
menu_item **item;
{
    *item = (menu_item *) 0;
    return 0;
}

void
dump_redirect(onoff_flag)
boolean onoff_flag;
{
#if defined (DUMPLOG) && defined (DUMPHTML)
    if (dumplog_file || dumphtml_file)
#elif defined (DUMPLOG)
    if (dumplog_file)
#elif defined (DUMPHTML)
    if (dumphtml_file)
#endif
    {
        if (onoff_flag) 
        {
#ifdef STATUS_HILITES
            botl_save_hilites();
#endif
            status_finish(); // 
            windowprocs.win_create_nhwindow_ex = dump_create_nhwindow_ex;
            windowprocs.win_clear_nhwindow = dump_clear_nhwindow;
            windowprocs.win_display_nhwindow = dump_display_nhwindow;
            windowprocs.win_destroy_nhwindow = dump_destroy_nhwindow;
            windowprocs.win_start_menu_ex = dump_start_menu_ex;
            windowprocs.win_add_menu = dump_add_menu;
            windowprocs.win_add_extended_menu = dump_add_extended_menu;
            windowprocs.win_end_menu_ex = dump_end_menu_ex;
            windowprocs.win_select_menu = dump_select_menu;
            windowprocs.win_putstr_ex = dump_putstr_ex;
            windowprocs.win_putstr_ex2 = dump_putstr_ex2;
#ifdef DUMPHTML
            windowprocs.win_outrip = dump_outrip;
#endif
            windowprocs.win_status_init = genl_status_init;
            windowprocs.win_status_finish = genl_status_finish;
            windowprocs.win_status_update = dump_status_update;
            windowprocs.win_status_enablefield = genl_status_enablefield;
            status_initialize(FALSE);
#ifdef STATUS_HILITES
            botl_restore_hilites();
#endif
        } else {
            windowprocs = dumplog_windowprocs_backup;
        }
        iflags.in_dumplog = onoff_flag;
    } else {
        iflags.in_dumplog = FALSE;
    }
}


STATIC_VAR int prev_app = 0;

#ifdef DUMPHTML
/****************************/
/* HTML DUMP LOG processing */
/****************************/

/* various tags - These were in a 2D array, but this is more readable */
#define HEAD_S "<h2>"
#define HEAD_E "</h2>"
#define SUBH_S "<h3>"
#define SUBH_E "</h3>"
#define PREF_S "<pre>"
#define PREF_E "</pre>"
#define LIST_S "<ul>"
#define LIST_E "</ul>"
#define OLIST_S "<ol>"
#define OLIST_E "</ol>"
#define LITM_S "<li>"
#define LITM_E "</li>"
#define TABLE_S "<table>"
#define TABLE_E "</table>"
#define TABLE_HEADER_S "<theader>"
#define TABLE_HEADER_E "</theader>"
#define TABLE_BODY_S "<tbody>"
#define TABLE_BODY_E "</tbody>"
#define TR_S "<tr>"
#define TR_E "</tr>"
#define TD_S "<td>"
#define TD_E "</td>"
#define TH_S "<th>"
#define TH_E "</th>"
#define BOLD_S "<b>"
#define BOLD_E "</b>"
#define UNDL_S "<u>"
#define UNDL_E "</u>"
#define PARAGRAPH_S "<p class=\"nh_paragraph_line\">"
#define PARAGRAPH_E "</p>"
/* Blinking text on webpages is gross (and tedious), replace with italics */
#define BLNK_S "<i>"
#define BLNK_E "</i>"
#define SPAN_E "</span>"
#define DIV_E "</div>"
#define SECTION_E "</section>"
#define LINEBREAK "<br />"

/** HTML putstr() handling **/

/* If we're using the NHW_MENU window,
   try to make a bullet-list of the contents.
   [Note, the inventory code uses the add_menu codepath
   and is not processed here. This is for container contents,
   dungeon overview, conduct, etc]
   When we get a heading or subheading we close any existing list with </ul>,
   and any preformatted block with </pre>.
   Then print the heading.
   For non-headings, we start a list if we don't already have one with <ul>
   then delimit the item with <li></li>
   for preformatted text, we don't mess with any existing bullet list, but try to
   keep consecutive preformatted strings in a single block.  */

STATIC_VAR uchar in_list = FALSE;
//STATIC_VAR boolean in_preform = FALSE;

STATIC_OVL
void
html_write_tags(fp, win, attr, color, app, before, info, usebr)
FILE* fp;
winid win;
int attr, color UNUSED, app;
boolean before, usebr; /* Tags before/after string */
struct extended_menu_info info; 
{
    if (!fp) return;

    uchar was_in_list = in_list;
    boolean is_table_row = (attr & (ATR_TABLE_ROW | ATR_TABLE_HEADER)) != 0;
    boolean is_heading = !(info.menu_flags & (MENU_FLAGS_IS_GROUP_HEADING)) && ((attr & ATR_SUBHEADING) == ATR_HEADING || (attr & ATR_SUBTITLE) == ATR_TITLE || (info.menu_flags & (MENU_FLAGS_IS_HEADING)));
    boolean is_subheading = (attr & ATR_SUBHEADING) == ATR_SUBHEADING || (attr & ATR_SUBTITLE) == ATR_SUBTITLE || (info.menu_flags & (MENU_FLAGS_IS_GROUP_HEADING));
    boolean is_bold = (attr & ATR_ATTR_MASK) == ATR_BOLD;
    boolean is_paragraph = (attr & ATR_PARAGRAPH_LINE) != 0;
    if (before) { /* before next string is written,
                     close any finished blocks
                     and open a new block if necessary */
        //if (attr & ATR_PREFORM) {
        //    if (!in_preform) {
        //        fprintf(fp, "%s", PREF_S);
        //        in_preform = TRUE;
        //    }
        //    return;
        //}
        //if (in_preform) {
        //    fprintf(fp, "%s", PREF_E);
        //    in_preform = FALSE;
        //}
        if (attr & ATR_START_TABLE) {
            fprintf(fp, "%s\n", "<table class=\"nh_table\">");
        }
        if (attr & ATR_TABLE_HEADER) {
            fprintf(fp, "%s", TABLE_HEADER_S);
        }
        if (attr & ATR_START_TABLE_BODY) {
            fprintf(fp, "%s\n", TABLE_BODY_S);
        }
        if (attr & ATR_TABLE_ROW) {
            fprintf(fp, "%s", TR_S);
        }
        if (!(attr & (ATR_SUBHEADING | ATR_SUBTITLE | ATR_PARAGRAPH_LINE)) && (info.menu_flags & (MENU_FLAGS_IS_HEADING | MENU_FLAGS_IS_GROUP_HEADING)) == 0 && win == NHW_MENU && !prev_app) {
            /* This is a bullet point */
            if (!in_list) {
                fprintf(fp, "%s\n", (attr & ATR_ORDERED_LIST) ? OLIST_S : LIST_S);
                in_list = (attr & ATR_ORDERED_LIST) ? 2 : TRUE;
            }
            fprintf(fp, LITM_S);
            return;
        }

        if (!prev_app)
        {
            if (in_list) {
                fprintf(fp, "%s\n", in_list == 2 ? OLIST_E : LIST_E);
                in_list = FALSE;
            }
            fprintf(fp, "%s", is_paragraph ? PARAGRAPH_S : "");
            fprintf(fp, "%s", is_heading ? HEAD_S : is_subheading ? SUBH_S : "");
            fprintf(fp, "%s", is_bold ? BOLD_S : "");
        }
        return;
    }
    /* after string is written */
    //if (in_preform) {
    //    if(!app)
    //        fprintf(fp, "%s\n", LINEBREAK); /* preform still gets <br /> at end of line */
    //    return; /* don't write </pre> until we get the next thing */
    //}
    if (in_list && !app) {
        fprintf(fp, "%s\n", LITM_E); /* </li>, but not </ul> yet */
        return;
    }
    if (attr & ATR_TABLE_ROW) {
        fprintf(fp, "%s\n", TR_E);
    }
    if (attr & ATR_END_TABLE_BODY) {
        fprintf(fp, "%s\n", TABLE_BODY_E);
    }
    if (attr & ATR_TABLE_HEADER) {
        fprintf(fp, "%s\n", TABLE_HEADER_E);
    }
    if (attr & ATR_END_TABLE) {
        fprintf(fp, "%s\n", TABLE_E);
    }

    if (!app)
    {
        fprintf(fp, "%s", is_bold ? BOLD_E : "");
        if (is_heading || is_subheading)
            fprintf(fp, "%s\n", is_heading ? HEAD_E : is_subheading ? SUBH_E : "");
        if(is_paragraph)
            fprintf(fp, "%s\n", PARAGRAPH_E);
        if(!is_paragraph && !is_heading && !is_subheading && usebr && !was_in_list && !is_table_row)
            fprintf(fp, "%s\n", LINEBREAK);
    }
}

extern const nhsym cp437toUnicode[256]; /* From hacklib.c */

/* Write HTML-escaped char to a file */
STATIC_OVL void
html_dump_char(fp, c, replacespace)
FILE* fp;
nhsym c; /* assumed to be either CP437 or Unicode */
boolean replacespace;
{
    if (!fp) return;
    switch (c) {
    case '<':
        fprintf(fp, "&lt;");
        break;
    case '>':
        fprintf(fp, "&gt;");
        break;
    case '&':
        fprintf(fp, "&amp;");
        break;
    case '\"':
        fprintf(fp, "&quot;");
        break;
    case '\'':
        fprintf(fp, "&#39;");
        break;
    case '\n':
        fprintf(fp, "<br />\n");
        break;
    case ' ':
        if (replacespace)
        {
            fprintf(fp, "&nbsp;");
            break;
        }
        /* FALLTHRU */
    default:
        if(c < 128)
            fprintf(fp, "%c", (char)c);
        else
        {
            nhsym ch = c;
            if (!!SYMHANDLING(H_IBM) && c >= 0 && c < 256) /* Convert CP437 to Unicode */
                ch = cp437toUnicode[c];
            fprintf(fp, "&#%d", (int)ch);
        }
    }
}

#ifndef DUMPHTML_DEFAULT_URL_FONT_NORMAL
#define DUMPHTML_DEFAULT_URL_FONT_NORMAL "https://fonts.cdnfonts.com/s/108/DejaVuSansMono.woff"
#endif
#ifndef DUMPHTML_DEFAULT_URL_FONT_BOLD
#define DUMPHTML_DEFAULT_URL_FONT_BOLD "https://fonts.cdnfonts.com/s/108/DejaVuSansMono-Bold.woff"
#endif
#ifndef DUMPHTML_DEFAULT_URL_FONT_ITALIC
#define DUMPHTML_DEFAULT_URL_FONT_ITALIC "https://fonts.cdnfonts.com/s/108/DejaVuSansMono-Oblique.woff"
#endif
#ifndef DUMPHTML_DEFAULT_URL_FONT_BOLD_ITALIC
#define DUMPHTML_DEFAULT_URL_FONT_BOLD_ITALIC "https://fonts.cdnfonts.com/s/108/DejaVuSansMono-BoldOblique.woff"
#endif

STATIC_OVL void
dump_css()
{
    int c = 0;
    FILE* css = 0;
    if (!dumphtml_file)
        return;

#ifdef DUMPHTML_CSS_FILE
    css = fopen_datafile(DUMPHTML_CSS_FILE, "r", DATAPREFIX); //"gnhdump.css"
#endif
    if (!css) {
        int i;

#ifndef DUMPHTML_WEBFONT_LINK
        const char* default_fontface_strings[4] = {
            "@font-face { font-family: \"DejaVu Sans Mono\"; font-style: normal; font-weight: normal;  src: local(DejaVu Sans Mono), local(DejaVuSansMono), url(DejaVuSansMono.woff), url(" DUMPHTML_DEFAULT_URL_FONT_NORMAL ")  format(\"woff\") }",
            "@font-face { font-family: \"DejaVu Sans Mono\"; font-style: normal; font-weight: bold; src: local(DejaVu Sans Mono Bold), local(DejaVuSansMono-Bold), url(DejaVuSansMono-Bold.woff), url(" DUMPHTML_DEFAULT_URL_FONT_BOLD ")  format(\"woff\"); }",
            "@font-face { font-family: \"DejaVu Sans Mono\"; font-style: oblique; font-weight: normal; src: local(DejaVu Sans Mono Oblique), local(DejaVuSansMono-Oblique), url(DejaVuSansMono-Oblique.woff), url(" DUMPHTML_DEFAULT_URL_FONT_ITALIC ")  format(\"woff\"); }",
            "@font-face { font-family: \"DejaVu Sans Mono\"; font-style: oblique; font-weight: bold; src: local(DejaVu Sans Mono Bold Oblique), local(DejaVuSansMono-BoldOblique), url(DejaVuSansMono-BoldOblique.woff), url(" DUMPHTML_DEFAULT_URL_FONT_BOLD_ITALIC ")  format(\"woff\"); }",
        };

        const char* sysopt_fontface_strings[4] = {
            sysopt.dumphtml_css_fontface_normal,
            sysopt.dumphtml_css_fontface_bold,
            sysopt.dumphtml_css_fontface_italic,
            sysopt.dumphtml_css_fontface_bolditalic,
        };

        for (i = 0; i < 4; i++)
        {
            const char* fontfacestring = sysopt_fontface_strings[i] ? sysopt_fontface_strings[i] : default_fontface_strings[i];
            fprintf(dumphtml_file, "%s\n", fontfacestring);
        }
#endif

        const char* css_strings[] = {
        "body {",
        "    color: #CCCCCC;",
        "    background-color: #222222;",
        "    font-family: %sDejaVu Sans Mono, Consolas, monospace;",
        "    font-size: 12px;",
        "    padding: 10px;",
        "    max-width:800px;",
        "    margin:0 auto;",
        "}",
        "",
        "section.nh_screen {",
        "    font-family: %sDejaVu Sans Mono, Consolas, monospace;",
        "    font-size: 12px;",
        "    background-color: black;",
        "    width: fit-content;",
        "    white-space: nowrap;",
        "}",
        "",
        ".nh_menu_item {",
        "    text-indent: -4ch;",
        "    padding-left: 7ch;",
        "}",
        "",
        "table.nh_table {",
        "    border-collapse: collapse;",
        "    border: none;",
        "}",
        "",
        "table.nh_table td, table.nh_table th {",
        "    padding: 0px 5px;",
        "    vertical-align:top;",
        "}",
        "",
        "p.nh_paragraph_line {",
        "    padding: 1px 0px 0px 5px;",
        "}",
        "",
        ".tooltip {",
        "    position: relative;",
        "    display: inline-block;",
        "}",
        "",
        /* Tooltip text */
        ".tooltip .tooltiptext {",
        "    display: none;",
        "    background-color: #222222;",
        "    color: #fff;",
        "    text-align: center;",
        "    font-size: 12px;",
        "    padding: 5px 5px;",
        "    border-radius: 6px;",
        "    position: absolute;",
        "    z-index: 1;",
        "    white-space: nowrap;",
        "    top: -10px;",
        "    left: 10px;",
        "}",
        "",
        /* Show the tooltip text when you mouse over the tooltip container */
        ".tooltip:hover .tooltiptext {",
        "    display: inline-block;",
        "}",
        "",
        "h2 {",
        "    color: white;",
        "    font-size: 14px;",
        "    padding-top: 1.5em;",
        "}",
        "",
        "h3 {",
        "    color: white;",
        "    font-size: 13px;",
        "    margin: 6px 0px 3px 5px;",
        "}",
        "",
        "blockquote {",
        "    margin-top: 0px;",
        "}",
        "",
        "div.nh_screen {",
        "    background-color: black;",
        "}",
        "",
        ".nh_color_0 { color: #555555; }  /* CLR_BLACK          */",
        ".nh_color_1 { color: #AA0000; }  /* CLR_RED            */",
        ".nh_color_2 { color: #00AA00; }  /* CLR_GREEN          */",
        ".nh_color_3 { color: #AA5500; }  /* CLR_BROWN          */",
        ".nh_color_4 { color: #0000AA; }  /* CLR_BLUE           */",
        ".nh_color_5 { color: #AA00AA; }  /* CLR_MAGENTA        */",
        ".nh_color_6 { color: #00AAAA; }  /* CLR_CYAN           */",
        ".nh_color_7 { color: #AAAAAA; }  /* CLR_GRAY           */",
        ".nh_color_8 { color: #555555; }  /* NO_COLOR           */",
        ".nh_color_9 { color: #FF5555; }  /* CLR_ORANGE         */",
        ".nh_color_10 { color: #55FF55; } /* CLR_BRIGHT_GREEN   */",
        ".nh_color_11 { color: #FFFF55; } /* CLR_YELLOW         */",
        ".nh_color_12 { color: #5555FF; } /* CLR_BRIGHT_BLUE    */",
        ".nh_color_13 { color: #FF55FF; } /* CLR_BRIGHT_MAGENTA */",
        ".nh_color_14 { color: #55FFFF; } /* CLR_BRIGHT_CYAN    */",
        ".nh_color_15 { color: #FCFCFC; } /* CLR_WHITE          */",
        ".nh_inv_0 { color: black; background-color: #555555; }  /* CLR_BLACK          */",
        ".nh_inv_1 { color: black; background-color: #AA0000; }  /* CLR_RED            */",
        ".nh_inv_2 { color: black; background-color: #00AA00; }  /* CLR_GREEN          */",
        ".nh_inv_3 { color: black; background-color: #AA5500; }  /* CLR_BROWN          */",
        ".nh_inv_4 { color: black; background-color: #0000AA; }  /* CLR_BLUE           */",
        ".nh_inv_5 { color: black; background-color: #AA00AA; }  /* CLR_MAGENTA        */",
        ".nh_inv_6 { color: black; background-color: #00AAAA; }  /* CLR_CYAN           */",
        ".nh_inv_7 { color: black; background-color: #AAAAAA; }  /* CLR_GRAY           */",
        ".nh_inv_8 { color: black; background-color: #555555; }  /* NO_COLOR           */",
        ".nh_inv_9 { color: black; background-color: #FF5555; }  /* CLR_ORANGE         */",
        ".nh_inv_10 { color: black; background-color: #55FF55; } /* CLR_BRIGHT_GREEN   */",
        ".nh_inv_11 { color: black; background-color: #FFFF55; } /* CLR_YELLOW         */",
        ".nh_inv_12 { color: black; background-color: #5555FF; } /* CLR_BRIGHT_BLUE    */",
        ".nh_inv_13 { color: black; background-color: #FF55FF; } /* CLR_BRIGHT_MAGENTA */",
        ".nh_inv_14 { color: black; background-color: #55FFFF; } /* CLR_BRIGHT_CYAN    */",
        ".nh_inv_15 { color: black; background-color: #FCFCFC; } /* CLR_WHITE          */",
        "",
        ".tombstone-wrapper {",
        "    padding-top: 20px;",
        "    max-width: 480px;",
        "    text-align:center;",
        "}",
        "",
        ".tombstone {",
        "    display: inline-block;",
        "    height:360px;",
        "    width:286px;",
        "    padding:30px 0px 0px 15px;",
            "    text-align:center;",
            "    font-size:16px;",
            "    color:white;",
            "    background-repeat:no-repeat;",
            "    background-image: url(\'data:image/png;base64,%s\');",
        "}",
        "",
        ".ts_row {",
        "    display:inline-block;",
        "    text-overflow: ellipsis;",
        "    overflow: hidden;",
        "}",
        "",
        ".ts_rip {",
        "    padding:10px 0px 10px 0px;",
        "}",
        "",
        ".ts_name {",
        "    padding:20px 0px 20px 0px;",
        "    font-size:20px;",
        "    width:230px;",
        "}",
        "",
        ".ts_points {",
        "    padding:5px;",
        "    width:220px;",
        "}",
        "",
        ".ts_killer {",
        "    padding:15px;",
        "    width:180px;",
        "    height:60px;",
        "}",
        "",
        ".ts_year {",
        "    padding:15px;",
        "    display:inline-block;",
        "}",
        0
        };

        const char* backgroundimagesplit[] = {
            "iVBORw0KGgoAAAANSUhEUgAAAR4AAAFoCAMAAAC2bGQ4AAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAyxpVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuNi1jMTQ1IDc5LjE2MjMxOSwgMjAxOC8wMi8xNS0yMDoyOTo0MyAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRw",
            "Oi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIEVsZW1lbnRzIDE3LjAgKFdpbmRvd3MpIiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOkJGQkFGN0FGMjA5QTExRUVBQjBBQjYyRTVDMkFFOEI4IiB4bXBNTTpEb2N1bWVudElEPSJ4bXAuZGlkOkJGQkFGN0IwMjA5QTExRUVBQjBBQjYyRTVDMkFFOEI4Ij4gPHhtcE1NOkRlcml2ZWRGcm9tIHN0UmVmOmluc3RhbmNlSUQ9InhtcC5paWQ6QkZCQUY3QUQyMDlBMTFFRUFC",
            "MEFCNjJFNUMyQUU4QjgiIHN0UmVmOmRvY3VtZW50SUQ9InhtcC5kaWQ6QkZCQUY3QUUyMDlBMTFFRUFCMEFCNjJFNUMyQUU4QjgiLz4gPC9yZGY6RGVzY3JpcHRpb24+IDwvcmRmOlJERj4gPC94OnhtcG1ldGE+IDw/eHBhY2tldCBlbmQ9InIiPz6YxtonAAABgFBMVEXa2toxMTHg4ODU1NTc3NwiIiIbGxuYmJgtLS04ODgSEhKWlpZmZmbe3t5tbW3R0dELCwuSkpKUlJQ0NDRoaGgpKSmQkJCIiIiNjY2CgoLLy8uLi4t/f38DAwPPz89qamqKioqFhYV8fHyGhoZ5eXlycnLj4+N3d3d+fn5wcHB2dnZ0dHTn5+e4uLi3t7eoqKilpaWmpqZlZWWjo6OkpKS5ubmioqKhoaG6urq7u7ugoKC8vLy9vb2+vr6/v7+fn5+enp7AwMDBwcHCwsLDw8OdnZ3ExMTFxcWcnJzGxsbH",
            "x8fIyMibm5tRUVFNTU1VVVXJyclZWVlJSUlPT09dXV1QUFBhYWFTU1NLS0tOTk5SUlJMTExERERUVFRaWlpbW1tYWFhWVlbKysrY2NhXV1dKSkpISEhcXFxHR0deXl5gYGBjY2NfX19GRkZiYmI/Pz+np6eqqqqpqamzs7OxsbG1tbWvr6+2trasrKyysrKtra2wsLCurq6rq6u0tLQAAACgcPXGAAAAgHRSTlP/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ADgFS2cAADLlSURBVHja5N2JVxvHlgdggUlAD0wGgw04nBATiJ/tgEGSJRC2JS+KZUcCs4ql2cK+CrPv+ten967l3qrqZnFmUm/O",
            "nPOSN+c53/zuvdXVre5Q6buth6VSvfE/+v8qK5V+KP0TV+ju/qv+U/qx9LCqLBwuGkvTtKJWdFfYWPX1ZVVVD3/Q/3P/8+/h+bEULk7kBwdnZqbGRkYmpycmlpeHh1dW1ta2tzV7FYEVDpfpVP/5f8vzY6k4PDM7Pz9YKOTzhszkwACJo8E4YXbVV/3wveJ0Wzzh6aX1uaGlBT0zham8paPjGDrDug6aHI7GWXqUvkOSQreQmem5rdG5ofklHWdwpmDojI0x0ZHjGCRl5qoi1w8//l/mCRfGN9bnZoeG5udNHCY6XF0xNA6JHhajg2vbK8sDY1MzM4P6WiiMDSyv6f8X4aq7K7Ub5CnLb+5sja7OmTpudKbY6BB1RUemuD05uLrZ2tH2pKmuOpeKvbw8Ozo4vTo87urq1FfX8eHV/un52V4sXVnX2KD9n+IprkZ2ttZHOR2nsOyuQ0THhilqA/ORjrbGulzs8uCqq7O3t6+vt/fVK8ND",
            "X4fuujLX4eGxwfWqt6+/v/841vS0VPzH82g7NTsbW5bOrKmzYPVkLDpmalYK489amipjZ1ddrwwOPR1X+/v7V4K176xTcx2c7u93dvYepp6Uwv9UHm2zZnN83NWh2w6hY7Vkk2ZlJvK8sbL76LDLLpnTU+ef+0qyGB9jnZ8fHBwe9vY0z//jeMrmGiKbO4bOupOdeSY7A67Otk6zMtjQ9ihxdnV8rKsY/3DWP6ciDuajr6Oj8/2ug0elqn8Oz3SDjuPp8G3Hi86aXlID689/Tp9cHV6dmv9EB2Kdw0O279BAvM/R0cXR6fFZSfsn8Dycb4hEXB09OqukztSUO88n9HGsTa8/fRQ9v7o6MP8xpDqH3BL5eDz6Ors4PX7w3Xl2aiLGoirLbspUdiZWtJWlZ42pi/1T84/v6JyzOiIbRoj0OeB99HXeVZf/fjzhiI3jZceuLK8pG9GZHt6eGG2v2zs4PdL/1BeszgGUnUN8MT5MeZE8Jycn",
            "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",
            0
        };

        i = 0;
        while (css_strings[i])
        {
            if (index(css_strings[i], '%'))
            {
                if (strstr(css_strings[i], "font-family"))
                {
                    char fontbuf[BUFSZ] = "";
                    char dbuf[BUFSZ * 2];
                    if (sysopt.dumphtmlfontname && *sysopt.dumphtmlfontname)
                        Sprintf(fontbuf, "%s, ", sysopt.dumphtmlfontname);
                    Sprintf(dbuf, css_strings[i], fontbuf);
                    fprintf(dumphtml_file, "%s\n", dbuf);
                }
                else if (strstr(css_strings[i], "background-image"))
                {
                    int j;
                    size_t totalsize = 0;
                    for (j = 0; backgroundimagesplit[j] != 0; j++)
                    {
                        totalsize += strlen(backgroundimagesplit[j]);
                    }
                    char* dynamic_buffer = (char*)alloc(totalsize + 1);
                    if (dynamic_buffer)
                    {
                        dynamic_buffer[0] = 0;
                        for (j = 0; backgroundimagesplit[j] != 0; j++)
                            Strcat(dynamic_buffer, backgroundimagesplit[j]);
                        dynamic_buffer[totalsize] = 0; /* Should be ok, but just in case */

                        fprintf(dumphtml_file, css_strings[i], dynamic_buffer);
                        fprintf(dumphtml_file, "\n");
                        free((genericptr_t)dynamic_buffer);
                    }
                    else
                    {
                        char dbuf[BUFSZ * 2];
                        Sprintf(dbuf, css_strings[i], "");
                        fprintf(dumphtml_file, "%s\n", dbuf);
                    }
                }
                else
                    fprintf(dumphtml_file, "%s\n", css_strings[i]);
            }
            else
                fprintf(dumphtml_file, "%s\n", css_strings[i]);
            i++;
        }
    }
    else
    {
        while ((c = fgetc(css)) != EOF) {
            fputc(c, dumphtml_file);
        }
        fclose(css);
    }
}

STATIC_OVL void
dump_outrip(win, how, when)
winid win UNUSED;
int how;
time_t when;
{
    if (dumphtml_file) 
    {
        html_write_tags(dumphtml_file, 0, 0, 0, 0, TRUE, zeroextendedmenuinfo, TRUE); /* </ul>, </pre> if needed */
        fprintf(dumphtml_file, "<div class=\"tombstone-wrapper\">\n");
        fprintf(dumphtml_file, "<div class=\"tombstone\">\n");
        fprintf(dumphtml_file, "<div class=\"ts_row ts_rip\">REST<br />IN<br />PEACE</div><br/>\n");
        fprintf(dumphtml_file, "<div class=\"ts_row ts_name\">");
        html_dump_str(dumphtml_file, plname, 0, 0, ATR_NONE, NO_COLOR);
        fprintf(dumphtml_file, "</div><br />\n");
        fprintf(dumphtml_file, "<div class=\"ts_row ts_points\">%ld points</div><br />\n", u.u_gamescore);
        fprintf(dumphtml_file, "<div class=\"ts_row ts_killer\">");
        char kbuf[BUFSZ * 2];
        formatkiller(kbuf, sizeof kbuf, how, FALSE);
        html_dump_str(dumphtml_file, kbuf, 0, 0, ATR_NONE, NO_COLOR);
        fprintf(dumphtml_file, "</div><br />\n");
        long year = yyyymmdd(when) / 10000L;
        fprintf(dumphtml_file, "<div class=\"ts_row ts_year\">%4ld</div>\n", year);
        fprintf(dumphtml_file, "%s\n", DIV_E);
        fprintf(dumphtml_file, "%s\n", DIV_E);
    }
    genl_outrip(NHW_DUMPTXT, how, when);
}

/* Write HTML-escaped string to a file */
STATIC_OVL void
html_dump_str(fp, str, attrs, colors, attr, color)
FILE* fp;
const char* str, *attrs, *colors;
int attr, color;
{
    if (!fp || !str) return;

    const char* p = str;
    int curcolor = NO_COLOR, curattr = ATR_NONE;
    int prevcolor = NO_COLOR, prevattr = ATR_NONE;
    if (attr & ATR_ORDERED_LIST)
    {
        const char* dp = 0;
        if ((attr & ATR_INDENT_MASK) == ATR_INDENT_AT_DASH)
            dp = strchr(p, '-');

        if (dp && dp > p)
        {
            dp++;
            while (*dp == ' ')
                dp++;
            int diff = (int)(dp - p);
            if (diff < (int)strlen(str))
            {
                p += diff;
                if (attrs)
                    attrs += diff;
                if (colors)
                    colors += diff;
            }
        }
    }

    boolean td_added = FALSE;
    boolean td_changed = FALSE;
    int i = 0;
    int len = (int)strlen(p);
    const char* endp = p + len;
    for (; *p && i < len; p++, i++)
    {
        td_changed = FALSE;
        curattr = attrs ? attrs[i] : attr;
        curcolor = colors ? colors[i] : color;

        if (attr & (ATR_TABLE_ROW | ATR_TABLE_HEADER))
        {
            if ((attr & ATR_INDENT_MASK) == ATR_INDENT_AT_COLON ? (*p == ':') : (p < endp - 1 && *p == ' ' && *(p + 1) == ' '))
            {
                if (td_added)
                    td_changed = TRUE;
            }
        }

        if (curattr != prevattr || td_changed)
        {
            if(prevattr != ATR_NONE)
                dump_set_color_attr(NO_COLOR, prevattr, FALSE, FALSE);
        }
        if (curcolor != prevcolor || td_changed)
        {
            if (prevcolor != NO_COLOR)
                dump_set_color_attr(prevcolor, ATR_NONE, FALSE, FALSE);
        }

        if (attr & (ATR_TABLE_ROW | ATR_TABLE_HEADER))
        {
            if (((attr & ATR_INDENT_MASK) == ATR_INDENT_AT_COLON ? (*p == ':') : (p < endp - 1 && *p == ' ' && *(p + 1) == ' ')) || i == 0)
            {
                if (td_added)
                    fprintf(fp, "%s", (attr & ATR_TABLE_HEADER) ? TH_E : TD_E);
                fprintf(fp, "%s", (attr & ATR_TABLE_HEADER) ? TH_S : TD_S);
                if (i > 0)
                {
                    p++;
                    i++;
                }
                while (*p == ' ')
                {
                    p++;
                    i++;
                }
                td_added = TRUE;
            }
        }

        if (!*p)
            break;

        if (curattr != prevattr || td_changed)
        {
            if (curattr != ATR_NONE)
                dump_set_color_attr(NO_COLOR, curattr, TRUE, FALSE);
        }
        if (curcolor != prevcolor || td_changed)
        {
            if (curcolor != NO_COLOR)
                dump_set_color_attr(curcolor, ATR_NONE, TRUE, FALSE);
        }

        html_dump_char(fp, (nhsym)*p, FALSE);
        prevattr = curattr;
        prevcolor = curcolor;
    }
    if(curcolor != NO_COLOR || curattr != ATR_NONE)
        dump_set_color_attr(curcolor, curattr, FALSE, FALSE);

    if (td_added)
    {
        fprintf(fp, "%s", (attr & ATR_TABLE_HEADER) ? TH_E : TD_E);
    }
}

STATIC_OVL void
html_dump_line(fp, win, attrs, colors, attr, color, app, str)
FILE* fp;
winid win;
int attr, color, app;
const char* str, *attrs, *colors;
{
    if ((strlen(str) == 0 || !strcmp(str, " ")) && !app && !prev_app) 
    {
        /* if it's a blank line, just print a blank line */
        if(!in_list && !(attr & ATR_HALF_SIZE))
            fprintf(fp, "%s\n", LINEBREAK);
        return;
    }

    html_write_tags(fp, win, attr, color, app, TRUE, zeroextendedmenuinfo, TRUE);
    html_dump_str(fp, str, attrs, colors, attr, color);
    html_write_tags(fp, win, attr, color, app, FALSE, zeroextendedmenuinfo, TRUE);
    prev_app = app;
}

/** HTML Map **/

/* Construct a symset for HTML line-drawing symbols.
   dat/symbols can't be used here because nhsym is uchar,
   and we require 16 bit values */

static int htmlsym[SYM_MAX] = DUMMY;

STATIC_OVL void
html_init_sym()
{
    /* see https://html-css-js.com/html/character-codes/drawing/ */

    /* Minimal set, based on IBMGraphics_1 set.
       Add more as required. */
    htmlsym[S_vwall] = 9474;
    htmlsym[S_hwall] = 9472;
    htmlsym[S_tlcorn] = 9484;
    htmlsym[S_trcorn] = 9488;
    htmlsym[S_blcorn] = 9492;
    htmlsym[S_brcorn] = 9496;
    htmlsym[S_crwall] = 9532;
    htmlsym[S_tuwall] = 9524;
    htmlsym[S_tdwall] = 9516;
    htmlsym[S_tlwall] = 9508;
    htmlsym[S_trwall] = 9500;
    htmlsym[S_vbeam] = 9474;
    htmlsym[S_hbeam] = 9472;
    htmlsym[S_sw_ml] = 9474;
    htmlsym[S_sw_mr] = 9474;
    htmlsym[S_explode4] = 9474;
    htmlsym[S_explode6] = 9474;
    /* and some extras */
    htmlsym[S_corr] = 9617;
    htmlsym[S_litcorr] = 9618;
}

/* convert 'special' flags returned from mapglyph to
  highlight attrs (currently just inverse) */
STATIC_OVL unsigned
mg_hl_attr(special)
unsigned long special;
{
    unsigned hl = 0;
    if ((special & MG_PET) && iflags.hilite_pet)
        hl |= HL_INVERSE; /* Could use wc2_petattr from curses here */
    if ((special & MG_DETECT) && iflags.use_inverse)
        hl |= HL_INVERSE;
    if ((special & MG_OBJPILE) && iflags.hilite_pile)
        hl |= HL_INVERSE;
    if ((special & MG_BW_LAVA) && iflags.use_inverse)
        hl |= HL_INVERSE;
    return hl;
}

void
html_dump_glyph(x, y, sym, ch, color, special)
int x, y, sym, color;
nhsym ch;
unsigned long special;
{
    char descbuf[BUFSZ] = "";
    const char* firstmatch = 0;
    coord cc;
    int desc_found = 0;
    unsigned attr;

    if (!dumphtml_file) return;

    if (x == 1) /* start row */
        fprintf(dumphtml_file, "<div class=\"nh_screen\">  "); /* 2 space left margin */
    cc.x = x;
    cc.y = y;
    desc_found = do_screen_description(cc, TRUE, ch, descbuf, &firstmatch, (struct permonst**)0);
    if (desc_found)
        fprintf(dumphtml_file, "<div class=\"tooltip\">");
    attr = mg_hl_attr(special);
    dump_set_color_attr(color, attr, TRUE, TRUE);
    if (htmlsym[sym])
        fprintf(dumphtml_file, "&#%d;", htmlsym[sym]);
    else
        html_dump_char(dumphtml_file, ch, TRUE);
    dump_set_color_attr(color, attr, FALSE, TRUE);
    if (desc_found)
        fprintf(dumphtml_file, "<div class=\"tooltiptext\">%s</div></div>", descbuf);
    if (x == COLNO - 1)
        fprintf(dumphtml_file, "  </div>\n"); /* 2 trailing spaces and newline */
}

/* Status and map highlighting */
STATIC_OVL void
dump_set_color_attr(coloridx, attrmask, onoff, usediv)
int coloridx, attrmask;
boolean onoff, usediv;
{
    if (!dumphtml_file) return;
    if (onoff) {
        if (attrmask & HL_BOLD)
            fprintf(dumphtml_file, BOLD_S);
        if (attrmask & HL_ULINE)
            fprintf(dumphtml_file, UNDL_S);
        if (attrmask & HL_BLINK)
            fprintf(dumphtml_file, BLNK_S);
        if (attrmask & HL_INVERSE)
            fprintf(dumphtml_file, "<%s class=\"nh_inv_%d\">", usediv ? "div" : "span", coloridx);
        else if (coloridx != NO_COLOR)
            fprintf(dumphtml_file, "<%s class=\"nh_color_%d\">", usediv ? "div" : "span", coloridx);
        /* ignore HL_DIM */
    }
    else {
        /* reverse order for nesting */
        if ((attrmask & HL_INVERSE) || coloridx != NO_COLOR)
            fprintf(dumphtml_file, usediv ? DIV_E : SPAN_E);
        if (attrmask & HL_BLINK)
            fprintf(dumphtml_file, BLNK_E);
        if (attrmask & HL_ULINE)
            fprintf(dumphtml_file, UNDL_E);
        if (attrmask & HL_BOLD)
            fprintf(dumphtml_file, BOLD_E);
    }
}

#endif /* DUMPHTML */


/** HTML Headers and footers **/

STATIC_OVL void
dump_headers()
{
#ifdef DUMPHTML
    char vers[BUFSZ]; /* buffer for short version string */

    /* TODO: make portable routine for getting iso8601 datetime */
    struct tm* t;
    char iso8601[BUFSZ];
    t = localtime(&dumplog_now);
    strftime(iso8601, BUFSZ, "%Y-%m-%dT%H:%M:%S%z", t);

    if (!dumphtml_file)
        return;

#ifdef DUMPHTML_WEBFONT_LINK
    const char* fontlink = sysopt.dumphtmlfontlink ? sysopt.dumphtmlfontlink : "https://fonts.cdnfonts.com/css/dejavu-sans-mono";
#endif
    fprintf(dumphtml_file, "<!DOCTYPE html>\n");
    fprintf(dumphtml_file, "<head>\n");
    fprintf(dumphtml_file, "<title>GnollHack %s (%s)</title>\n", version_string(vers), plname);
    fprintf(dumphtml_file, "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n");
    fprintf(dumphtml_file, "<meta name=\"generator\" content=\"GnollHack %s (%s)\" />\n", vers, plname);
    fprintf(dumphtml_file, "<meta name=\"date\" content=\"%s\" />\n", iso8601);
    fprintf(dumphtml_file, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n");
#ifdef DUMPHTML_WEBFONT_LINK
    fprintf(dumphtml_file, "<link href=\"%s\" title=\"Default\" rel=\"stylesheet\" type=\"text/css\" media=\"all\" />\n", fontlink);
#endif
    fprintf(dumphtml_file, "<style type=\"text/css\">\n");
    dump_css();
    fprintf(dumphtml_file, "</style>\n</head>\n<body>\n");

#endif
}

STATIC_OVL void
dump_footers()
{
#ifdef DUMPHTML
    if (dumphtml_file) {
        html_write_tags(dumphtml_file, 0, 0, 0, 0, TRUE, zeroextendedmenuinfo, TRUE); /* close </ul> and </pre> if open */
        fprintf(dumphtml_file, "</body>\n</html>\n");
    }
#endif
}

/** HTML Map and status bar (collectively, the 'screendump') **/

void
dump_start_screendump()
{
#ifdef DUMPHTML
    if (!dumphtml_file) return;
    html_init_sym();
    
    fprintf(dumphtml_file, "<div style=\"overflow-x:auto;\">\n");
    fprintf(dumphtml_file, "<section class=\"nh_screen\">\n");
#endif
}

void
dump_end_screendump()
{
#ifdef DUMPHTML
    if (dumphtml_file)
    {
        fprintf(dumphtml_file, "%s\n", SECTION_E);
        fprintf(dumphtml_file, "%s\n", DIV_E);
    }
#endif
}

#endif /* DUMPLOG || DUMPHTML */


void
dump_open_log(now)
time_t now;
{
#if defined (DUMPLOG) || defined (DUMPHTML)
    char buf[BUFSZ];
    char* fname;
    boolean fileexists = FALSE;

    dumplog_now = now;
#ifdef DUMPLOG
    fname = print_dumplog_filename_to_buffer(buf);
    if (fname)
        dumplog_file = fopen(fname, "w");
    fileexists = fileexists || (dumplog_file != 0);
#endif
#ifdef DUMPHTML
    fname = print_dumphtml_filename_to_buffer(buf);
    if (fname)
        dumphtml_file = fopen(fname, "w");
    fileexists = fileexists || (dumphtml_file != 0);
#endif
    if (fileexists) {
        dumplog_windowprocs_backup = windowprocs;
        menu_headings_backup = iflags.menu_headings;
    }
    dump_headers();
#else /*!DUMPLOG*/
    nhUse(now);
#endif /*?DUMPLOG*/
}

void
dump_close_log(VOID_ARGS)
{
#if defined(DUMPLOG) || defined (DUMPHTML)
    dump_footers();
#ifdef DUMPLOG
    if (dumplog_file) {
        (void)fclose(dumplog_file);
        dumplog_file = (FILE*)0;
    }
#endif
#ifdef DUMPHTML
    if (dumphtml_file) {
        (void)fclose(dumphtml_file);
        dumphtml_file = (FILE*)0;
    }
#endif
#endif
}

void
dump_forward_putstr(win, attr, str, no_forward, app)
winid win;
int attr;
const char* str;
int no_forward, app;
{
#if defined(DUMPLOG) || defined (DUMPHTML)
    dump_putstr_ex(win == NHW_DUMPTXT || win == NHW_DUMPHTML ? win : 0, str, attr, NO_COLOR, app);
#endif
    if (!no_forward)
        putstr_ex(win, str, attr, NO_COLOR, app);
}

STATIC_OVL void
itemdesc_putstr_ex(win, str, attr, color, app)
winid win UNUSED;
int attr UNUSED, app UNUSED, color UNUSED;
const char* str UNUSED;
{
    return;
}

STATIC_OVL void
itemdesc_putstr_ex2(win, str, attrs, colors, attr, color, app)
winid win UNUSED;
int attr UNUSED, color UNUSED, app UNUSED;
const char* str UNUSED, * attrs UNUSED, * colors UNUSED;
{
    return;
}

void
itemdesc_redirect(VOID_ARGS)
{
    windowprocs.win_create_nhwindow_ex = dump_create_nhwindow_ex;
    windowprocs.win_clear_nhwindow = dump_clear_nhwindow;
    windowprocs.win_display_nhwindow = dump_display_nhwindow;
    windowprocs.win_destroy_nhwindow = dump_destroy_nhwindow;
    windowprocs.win_putstr_ex = itemdesc_putstr_ex;
    windowprocs.win_putstr_ex2 = itemdesc_putstr_ex2;
}

void
reset_windows(VOID_ARGS)
{
    prev_app = 0;
    in_list = 0;
    //in_preform = 0;
}

/*windows.c*/

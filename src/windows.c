/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-05-22 */

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
STATIC_DCL void FDECL(dump_putstr_ex, (winid, int, const char *, int, int));
STATIC_DCL void FDECL(dump_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));
STATIC_DCL void NDECL(dump_headers);
STATIC_DCL void NDECL(dump_footers);
STATIC_DCL void FDECL(dump_status_update, (int, genericptr_t, int, int, int, unsigned long*));
STATIC_DCL void NDECL(dump_render_status);
#ifdef DUMPHTML
STATIC_DCL void FDECL(dump_set_color_attr, (int, int, BOOLEAN_P));
STATIC_DCL void NDECL(html_init_sym);
STATIC_DCL void NDECL(dump_css);
STATIC_DCL void FDECL(dump_outrip, (winid, int, time_t));
STATIC_DCL void FDECL(html_dump_str, (FILE*, const char*, const char*, const char*, int, int, int));
STATIC_DCL void FDECL(html_dump_line, (FILE*, winid, const char*, const char*, int, int, int, const char*));
STATIC_DCL void FDECL(html_write_tags, (FILE*, winid, int, int, int, BOOLEAN_P, struct extended_menu_info)); /* Tags before/after string */
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
genl_display_gui_effect(int x UNUSED, int y UNUSED, int style UNUSED, unsigned long tflags UNUSED)
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
        (void)strncpy(tmps, s, WINDOW_TYPE_MAXLEN - 1);
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
STATIC_DCL void FDECL(hup_putstr_ex, (winid, int, const char *, int, int));
STATIC_DCL void FDECL(hup_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));
STATIC_DCL void FDECL(hup_print_glyph, (winid, XCHAR_P, XCHAR_P, struct layer_info));
STATIC_DCL void FDECL(hup_issue_gui_command, (int, int, const char*));
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
hup_putstr_ex(window, attr, text, app, color)
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
hup_issue_gui_command(cmd_id, cmd_param, cmd_str)
int cmd_id UNUSED, cmd_param UNUSED;
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

    static enum statusfields gsu_fieldorder[][23] = {
        /* line one */
        { BL_TITLE, BL_STR, BL_DX, BL_CO, BL_IN, BL_WI, BL_CH, BL_GOLD, //BL_ALIGN,
          BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH,
          BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH, BL_FLUSH },
        /* line two, default order */
        { BL_MODE, BL_LEVELDESC, // BL_GOLD,
          BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_XP, BL_EXP, BL_HD,
          BL_TIME, BL_REALTIME,
          BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_FLUSH },
        /* move time to the end */
        { BL_MODE, BL_LEVELDESC, //BL_GOLD,
          BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_XP, BL_EXP, BL_HD,
          BL_2WEP, BL_SKILL,BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_TIME, BL_REALTIME, BL_FLUSH },
        /* move experience and time to the end */
        { BL_MODE, BL_LEVELDESC, // BL_GOLD,
          BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_XP, BL_EXP, BL_HD, BL_TIME, BL_REALTIME, BL_FLUSH },
        /* move level description plus gold and experience and time to end */
        { BL_HP, BL_HPMAX, BL_ENE, BL_ENEMAX, BL_AC, BL_MC_LVL, BL_MC_PCT,
          BL_2WEP, BL_SKILL, BL_HUNGER, BL_CAP, BL_CONDITION,
          BL_MODE, BL_LEVELDESC, //BL_GOLD,
          BL_MOVE, BL_UWEP, BL_UWEP2, BL_XP, BL_EXP, BL_HD, BL_TIME, BL_REALTIME, BL_FLUSH },
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
            fprintf(dumphtml_file, "<span class=\"nh_screen\">  "); /* 2 space left margin */
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
                        putstr_ex(NHW_STATUS, 0, " ", 1, NO_COLOR);
                        pad--;
#ifdef STATUS_HILITES
                        if (iflags.hilite_delta) {
                            attrmask = condattr(mask, dump_colormasks);
                            coloridx = condcolor(mask, dump_colormasks);
                            dump_set_color_attr(coloridx, attrmask, TRUE);
                        }
#endif
                        putstr_ex(NHW_STATUS, 0, condition_definitions[c].text[0], 1, NO_COLOR);
                        pad -= strlen(condition_definitions[c].text[0]);
#ifdef STATUS_HILITES
                        if (iflags.hilite_delta) {
                            dump_set_color_attr(coloridx, attrmask, FALSE);
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
                putstr_ex(NHW_STATUS, 0, "[", 1, NO_COLOR);
                if (*bar) { /* always True, unless twoparts+dead (0 HP) */
                    dump_set_color_attr(hpbar_color, HL_INVERSE, TRUE);
                    putstr_ex(NHW_STATUS, ATR_NONE, bar, 0, NO_COLOR);
                    dump_set_color_attr(hpbar_color, HL_INVERSE, FALSE);
                }
                if (twoparts) { /* no highlighting for second part */
                    *bar2 = savedch;
                    putstr_ex(NHW_STATUS, 0, bar2, 1, NO_COLOR);
                }
                putstr_ex(NHW_STATUS, 0, "]", 1, NO_COLOR);
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
                    dump_set_color_attr(coloridx, attrmask, TRUE);
                }
#endif
                putstr_ex(NHW_STATUS, 0, text, 1, NO_COLOR);
                pad -= strlen(text);
#ifdef STATUS_HILITES
                if (iflags.hilite_delta) {
                    dump_set_color_attr(coloridx, attrmask, FALSE);
                }
#endif
            }
        }
        if (dumphtml_file)
            fprintf(dumphtml_file, "%*s</span>\n", pad, " ");
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
dump_putstr_ex(win, attr, str, app, color)
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
            html_dump_str(dumphtml_file, str, 0, 0, attr, color, app);
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
            html_dump_str(dumphtml_file, str, attrs, colors, attr, color, app);
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
        html_write_tags(dumphtml_file, win, attr, color, 0, TRUE, info);
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
            fprintf(dumphtml_file, "<span class=\"nh_item_letter\">%c</span> - ", ch);
        }
        html_dump_str(dumphtml_file, str, 0, 0, ATR_NONE, NO_COLOR, 1);
        fprintf(dumphtml_file, "%s", iscolor ? "</span>" : "");
        html_write_tags(dumphtml_file, win, attr, color, 0, FALSE, info);
    }
#endif
}

/*ARGSUSED*/
STATIC_OVL void
dump_end_menu_ex(win, str, str2)
winid win UNUSED;
const char *str, *str2;
{
    char buf[UTF8BUFSZ * 4 + 3] = "";
    char buf1[UTF8BUFSZ * 2] = "";
    char buf2[UTF8BUFSZ * 2] = "";
    const char* txt = 0;
    txt = (str && str2) ? " - " : "";

    if (str)
        write_text2buf_utf8(buf1, sizeof(buf1), str);
    if (str2)
        write_text2buf_utf8(buf2, sizeof(buf2), str2);

    Sprintf(buf, "%s%s%s", buf1, txt, buf2);

#ifdef DUMPLOG
    if (dumplog_file) 
    {
        if (str || str2)
            fprintf(dumplog_file, "%s\n", buf);
        else
            fputs("\n", dumplog_file);
    }
#endif
#ifdef DUMPHTML
    if (dumphtml_file)
        html_dump_line(dumphtml_file, 0, 0, 0, 0, 0, 0, str || str2 ? buf : "");
#endif
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
#define LITM_S "<li>"
#define LITM_E "</li>"
#define BOLD_S "<b>"
#define BOLD_E "</b>"
#define UNDL_S "<u>"
#define UNDL_E "</u>"
/* Blinking text on webpages is gross (and tedious), replace with italics */
#define BLNK_S "<i>"
#define BLNK_E "</i>"
#define SPAN_E "</span>"
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
STATIC_OVL
void
html_write_tags(fp, win, attr, color, app, before, info)
FILE* fp;
winid win;
int attr, color UNUSED, app;
boolean before; /* Tags before/after string */
struct extended_menu_info info; 
{
    static boolean in_list = FALSE;
    static boolean in_preform = FALSE;
    if (!fp) return;

    boolean is_heading = !(info.menu_flags & (MENU_FLAGS_IS_GROUP_HEADING)) && ((attr & ATR_SUBHEADING) == ATR_HEADING || (attr & ATR_SUBTITLE) == ATR_TITLE || (info.menu_flags & (MENU_FLAGS_IS_HEADING)));
    boolean is_subheading = (attr & ATR_SUBHEADING) == ATR_SUBHEADING || (attr & ATR_SUBTITLE) == ATR_SUBTITLE || (info.menu_flags & (MENU_FLAGS_IS_GROUP_HEADING));
    boolean is_bold = (attr & ATR_ATTR_MASK) == ATR_BOLD;
    if (before) { /* before next string is written,
                     close any finished blocks
                     and open a new block if necessary */
        if (attr & ATR_PREFORM) {
            if (!in_preform) {
                fprintf(fp, "%s", PREF_S);
                in_preform = TRUE;
            }
            return;
        }
        if (in_preform) {
            fprintf(fp, PREF_E);
            in_preform = FALSE;
        }
        if (!(attr & (ATR_SUBHEADING | ATR_SUBTITLE)) && (info.menu_flags & (MENU_FLAGS_IS_HEADING | MENU_FLAGS_IS_GROUP_HEADING)) == 0 && win == NHW_MENU) {
            /* This is a bullet point */
            if (!in_list) {
                fprintf(fp, "%s\n", LIST_S);
                in_list = TRUE;
            }
            fprintf(fp, LITM_S);
            return;
        }
        if (in_list) {
            fprintf(fp, "%s\n", LIST_E);
            in_list = FALSE;
        }
                fprintf(fp, "%s", is_heading ? HEAD_S :
            is_subheading ? SUBH_S : is_bold ? BOLD_S : "");
        return;
    }
    /* after string is written */
    if (in_preform) {
        if(!app)
            fprintf(fp, LINEBREAK); /* preform still gets <br /> at end of line */
        return; /* don't write </pre> until we get the next thing */
    }
    if (in_list) {
        fprintf(fp, "%s\n", LITM_E); /* </li>, but not </ul> yet */
        return;
    }
    fprintf(fp, "%s", is_heading ? HEAD_E:
        is_subheading ? SUBH_E : is_bold ? BOLD_E : "");

    if(!app)
        fprintf(fp, "%s", !is_heading && !is_subheading ? LINEBREAK : "");
}

extern const nhsym cp437toUnicode[256]; /* From hacklib.c */

/* Write HTML-escaped char to a file */
STATIC_OVL void
html_dump_char(fp, c)
FILE* fp;
nhsym c; /* assumed to be either CP437 or Unicode */
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
        "    font-size: min(2vw, 2vh);",
        "}",
        "",
        "pre.nh_screen {",
        "    font-family: %sDejaVu Sans Mono, Consolas, monospace;",
        "    font-size: min(2.25vw, 2.25vh);",
        "    background-color: black;",
        "    width: fit-content;",
        "}",
        "",
        ".nh_item_letter {",
        "    padding-left: 3ex;",
        "}",
        "",
        ".tooltip {",
        "    position: relative;",
        "    display: inline-block;",
        "}",
        "",
        /* Tooltip text */
        ".tooltip .tooltiptext {",
        "    visibility: hidden;",
        "    background-color: #222222;",
        "    color: #fff;",
        "    text-align: center;",
        "    font-size: min(2vw, 2vh);",
        "    padding: 5px 5px;",
        "    border-radius: 6px;",
        "    position: absolute;",
        "    z-index: 1;",
        "}",
        "",
        /* Show the tooltip text when you mouse over the tooltip container */
        ".tooltip:hover .tooltiptext {",
        "    visibility: visible;",
        "}",
        "",
        "h2 {",
        "    color: white;",
        "    font-size: min(2.5vw, 2.5vh)vw;",
        "    margin: 0.5ex;",
        "    padding-top: 1.5em;",
        "}",
        "",
        "h3 {",
        "    color: white;",
        "    font-size: min(2.25vw, 2.25vh);",
        "    margin: 1ex 0ex 0.25ex 1.5ex;",
        "}",
        "",
        "blockquote {",
        "    margin-top: 0px;",
        "}",
        "",
        "span.nh_screen {",
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
        0
        };

        i = 0;
        while (css_strings[i])
        {
            if (index(css_strings[i], '%'))
            {
                char fontbuf[BUFSZ] = "";
                char dbuf[BUFSZ];
                if(sysopt.dumphtmlfontname && *sysopt.dumphtmlfontname)
                    Sprintf(fontbuf, "%s, ", sysopt.dumphtmlfontname);
                Sprintf(dbuf, css_strings[i], fontbuf);
                fprintf(dumphtml_file, "%s\n", dbuf);
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
winid win;
int how;
time_t when;
{
    if (dumphtml_file) {
        html_write_tags(dumphtml_file, 0, 0, 0, 0, TRUE, zeroextendedmenuinfo); /* </ul>, </pre> if needed */
        fprintf(dumphtml_file, "%s\n", PREF_S);
    }
    genl_outrip(win, how, when);
    if (dumphtml_file)
        fprintf(dumphtml_file, "%s\n", PREF_E);

}

/* Write HTML-escaped string to a file */
STATIC_OVL void
html_dump_str(fp, str, attrs, colors, attr, color, app)
FILE* fp;
const char* str, *attrs, *colors;
int attr, color, app;
{
    if (!fp) return;

    const char* p;
    int curcolor = NO_COLOR, curattr = ATR_NONE;
    int prevcolor = NO_COLOR, prevattr = ATR_NONE;
    int i = 0;
    for (p = str; *p; p++, i++)
    {
        curattr = attrs ? attrs[i] : attr;
        curcolor = colors ? colors[i] : color;
        if (curattr != prevattr)
        {
            if(prevattr != ATR_NONE)
                dump_set_color_attr(NO_COLOR, prevattr, FALSE);
            if (curattr != ATR_NONE)
                dump_set_color_attr(NO_COLOR, curattr, TRUE);
        }
        if (curcolor != prevcolor)
        {
            if (prevcolor != NO_COLOR)
                dump_set_color_attr(prevcolor, ATR_NONE, FALSE);
            if (curcolor != NO_COLOR)
                dump_set_color_attr(curcolor, ATR_NONE, TRUE);
        }
        html_dump_char(fp, (nhsym)*p);
        prevattr = curattr;
        prevcolor = curcolor;
    }
    if(curcolor != NO_COLOR || curattr != ATR_NONE)
        dump_set_color_attr(curcolor, curattr, FALSE);
}

STATIC_OVL void
html_dump_line(fp, win, attrs, colors, attr, color, app, str)
FILE* fp;
winid win;
int attr, color, app;
const char* str, *attrs, *colors;
{
    if ((strlen(str) == 0 || !strcmp(str, " ")) && !app) {
        /* if it's a blank line, just print a blank line */
        fprintf(fp, "%s\n", LINEBREAK);
        return;
    }

    html_write_tags(fp, win, attr, color, app, TRUE, zeroextendedmenuinfo);
    html_dump_str(fp, str, attrs, colors, attr, color, app);
    html_write_tags(fp, win, attr, color, app, FALSE, zeroextendedmenuinfo);
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
    char buf[BUFSZ]; /* do_screen_description requires this :( */
    const char* firstmatch = "unknown"; /* and this */
    coord cc;
    int desc_found = 0;
    unsigned attr;

    if (!dumphtml_file) return;

    if (x == 1) /* start row */
        fprintf(dumphtml_file, "<span class=\"nh_screen\">  "); /* 2 space left margin */
    cc.x = x;
    cc.y = y;
    desc_found = do_screen_description(cc, TRUE, ch, buf, &firstmatch, (struct permonst**)0);
    if (desc_found)
        fprintf(dumphtml_file, "<div class=\"tooltip\">");
    attr = mg_hl_attr(special);
    dump_set_color_attr(color, attr, TRUE);
    if (htmlsym[sym])
        fprintf(dumphtml_file, "&#%d;", htmlsym[sym]);
    else
        html_dump_char(dumphtml_file, ch);
    dump_set_color_attr(color, attr, FALSE);
    if (desc_found)
        fprintf(dumphtml_file, "<span class=\"tooltiptext\">%s</span></div>", firstmatch);
    if (x == COLNO - 1)
        fprintf(dumphtml_file, "  </span>\n"); /* 2 trailing spaces and newline */
}

/* Status and map highlighting */
STATIC_OVL void
dump_set_color_attr(coloridx, attrmask, onoff)
int coloridx, attrmask;
boolean onoff;
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
            fprintf(dumphtml_file, "<span class=\"nh_inv_%d\">", coloridx);
        else if (coloridx != NO_COLOR)
            fprintf(dumphtml_file, "<span class=\"nh_color_%d\">", coloridx);
        /* ignore HL_DIM */
    }
    else {
        /* reverse order for nesting */
        if ((attrmask & HL_INVERSE) || coloridx != NO_COLOR)
            fprintf(dumphtml_file, SPAN_E);
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
    char vers[16]; /* buffer for short version string */

    /* TODO: make portable routine for getting iso8601 datetime */
    struct tm* t;
    char iso8601[32];
    t = localtime(&dumplog_now);
    strftime(iso8601, 32, "%Y-%m-%dT%H:%M:%S%z", t);

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
        html_write_tags(dumphtml_file, 0, 0, 0, 0, TRUE, zeroextendedmenuinfo); /* close </ul> and </pre> if open */
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
    fprintf(dumphtml_file, "<pre class=\"nh_screen\">\n");
#endif
}

void
dump_end_screendump()
{
#ifdef DUMPHTML
    if (dumphtml_file)
        fprintf(dumphtml_file, "%s\n", PREF_E);
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
dump_forward_putstr(win, attr, str, no_forward)
winid win;
int attr;
const char* str;
int no_forward;
{
#if defined(DUMPLOG) || defined (DUMPHTML)
    dump_putstr_ex(win, attr, str, 0, NO_COLOR);
#endif
    if (!no_forward)
        putstr(win, attr, str);
}

/*windows.c*/

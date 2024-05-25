/* GnollHack 4.0    dllhack.c    $NHDT-Date: 1449488876 2015/12/07 11:47:56 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.44 $ */
/* Copyright (C) 2001 by Alex Kompel, 2021 Janne Gustafsson      */
/* GnollHack may be freely redistributed.  See license for details. */

#include "win10.h"
#include <process.h>
#include <math.h>

#include "dllproc.h"
#include "hack.h"
#include "dlb.h"
#include "resource.h"

#if !defined(SAFEPROCS)
#error You must #define SAFEPROCS to build winhack.c
#endif

static TCHAR *_get_cmd_arg(TCHAR *pCmdLine);
extern int GUILaunched;     /* We tell shared startup code in windmain.c
                               that the GUI was launched via this */

#ifdef __BORLANDC__
#define _stricmp(s1, s2) stricmp(s1, s2)
#define _strdup(s1) strdup(s1)
#endif

// Forward declarations of functions included in this code module:
extern boolean FDECL(main, (int, char **));

#define MAX_CMDLINE_PARAM 255

int GnollHackStart(cmdlineargs)
char* cmdlineargs;
{
    int argc;
    char *argv[MAX_CMDLINE_PARAM];
    size_t len;
    TCHAR *p;
    TCHAR wbuf[BUFSZ];
    char buf[BUFSZ];

    HINSTANCE hInstance = GetModuleHandle(NULL);

    /*
     * Get a set of valid safe windowport function
     * pointers during early startup initialization.
     *
     * When get_safe_procs is called with 0 as the param,
     * non-functional, but safe function pointers are set
     * for all windowport routines.
     *
     * When get_safe_procs is called with 1 as the param,
     * raw_print, raw_print_bold, and wait_synch, and nhgetch
     * are set to use C stdio routines via stdio_raw_print,
     * stdio_raw_print_bold, stdio_wait_synch, and
     * stdio_nhgetch.
     */
    windowprocs = *get_safe_procs(0);

    /*
     * Now we are going to override a couple
     * of the windowprocs functions so that
     * error messages are handled in a suitable
     * way for the graphical version.
     */
    windowprocs.win_raw_print = dll_raw_print;
    windowprocs.win_raw_print_bold = dll_raw_print_bold;
    windowprocs.win_wait_synch = dll_wait_synch;

    win10_init();
    sys_early_init();
    dll_init_platform();

    /* get command line parameters */
    p = _get_cmd_arg(GetCommandLine());
    p = _get_cmd_arg(NULL); /* skip first paramter - command name */
    for (argc = 1; p && argc < MAX_CMDLINE_PARAM; argc++) {
        len = _tcslen(p);
        if (len > 0) {
            argv[argc] = _strdup(GNH_W2A(p, buf, BUFSZ));
        } else {
            argv[argc] = "";
        }
        p = _get_cmd_arg(NULL);
    }
    GetModuleFileName(NULL, wbuf, BUFSZ);
    argv[0] = _strdup(GNH_W2A(wbuf, buf, BUFSZ));

    if (argc == 2) {
        TCHAR *savefile = strdup(argv[1]);
        TCHAR *plname;
        for (p = savefile; *p && *p != '-'; p++)
            ;
        if (*p) {
            /* we found a '-' */
            plname = p + 1;
            for (p = plname; *p && *p != '.'; p++)
                ;
            if (*p) {
                if (strcmp(p + 1, "GnollHack-saved-game") == 0) {
                    *p = '\0';
                    argv[1] = "-u";
                    argv[2] = _strdup(plname);
                    argc = 3;
                }
            }
        }
        free(savefile);
    }

    GUILaunched = 1;
    iflags.using_gui_tiles = TRUE;
    iflags.using_gui_sounds = TRUE;

    /* let main do the argument processing */
    (void) main(argc, argv);

    dll_exit_platform(EXIT_SUCCESS);

    return 0;
}

TCHAR *
_get_cmd_arg(TCHAR *pCmdLine)
{
    static TCHAR *pArgs = NULL;
    TCHAR *pRetArg;
    BOOL bQuoted;

    if (!pCmdLine && !pArgs)
        return NULL;
    if (!pArgs)
        pArgs = pCmdLine;

    /* skip whitespace */
    for (pRetArg = pArgs; *pRetArg && _istspace(*pRetArg);
         pRetArg = CharNext(pRetArg))
        ;
    if (!*pRetArg) {
        pArgs = NULL;
        return NULL;
    }

    /* check for quote */
    if (*pRetArg == TEXT('"')) {
        bQuoted = TRUE;
        pRetArg = CharNext(pRetArg);
        pArgs = _tcschr(pRetArg, TEXT('"'));
    } else {
        /* skip to whitespace */
        for (pArgs = pRetArg; *pArgs && !_istspace(*pArgs);
             pArgs = CharNext(pArgs))
            ;
    }

    if (pArgs && *pArgs) {
        TCHAR *p;
        p = pArgs;
        pArgs = CharNext(pArgs);
        *p = (TCHAR) 0;
    } else {
        pArgs = NULL;
    }

    return pRetArg;
}

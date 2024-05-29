/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/*	SCCS Id: @(#)winconf.h	3.4	2011/03/31	*/
/* Copyright (c) Kenneth Lorber, Bethesda, Maryland, 1990, 1991, 1992, 1993. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifdef GNH_WIN
#ifndef WINCONF_H
#define WINCONF_H

#define error debuglog

#define TEXTCOLOR /* Color text */
#define CHANGE_COLOR
#define HOLD_LOCKFILE_OPEN /* Keep an exclusive lock on the .0 file */
#define SELF_RECOVER /* Allow the game itself to recover from an aborted game */
#define USER_SOUNDS
#define SELECTSAVED /* Provide menu of saved games to choose from at start */
#define NO_FILE_LINKS /* if no hard links */
#define LOCKDIR "." /* where to put locks */ 
#define ASCIIGRAPH
#define NO_SIGNAL
#define CONTINUE_PLAYING_AFTER_SAVING
#define ALTMETA /* support altmeta run-time option */
#define SAFERHANGUP

#ifdef getchar
#undef getchar
#endif
#define getchar nhgetch
#undef tgetch
#define tgetch nhgetch
#define getuid() 1

#if defined (DUMPLOG) || defined (DUMPHTML)
#undef DUMPLOG_FILE
#define DUMPLOG_FILE        "dumplog/gnollhack.%n.%d.txt" /* Note: Actually the one in sysconf is used, not this one */
#undef DUMPLOG_DIR
#define DUMPLOG_DIR        "dumplog" /* Note: this is just used to create a directory, DUMPLOG_FILE still needs to have the full path */
#endif

/*
 * -----------------------------------------------------------------
 *  The remaining code shouldn't need modification.
 * -----------------------------------------------------------------
 */
 /* #define SHORT_FILENAMES */ /* All NT filesystems support long names now
  */

  /* Stuff to help the user with some common, yet significant errors */
#define INTERJECT_PANIC 0
#define INTERJECTION_TYPES (INTERJECT_PANIC + 1)
extern void FDECL(interject_assistance,
    (int, int, genericptr_t, genericptr_t));
extern void FDECL(interject, (int));

/*
 *===============================================
 * Compiler-specific adjustments
 *===============================================
 */

#ifdef _MSC_VER
#if (_MSC_VER > 1000)
/* Visual C 8 warning elimination */
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#pragma warning(disable : 4996) /* VC8 deprecation warnings */
#pragma warning(disable : 4142) /* benign redefinition */
#pragma warning(disable : 4267) /* conversion from 'size_t' to XX */
#if (_MSC_VER > 1600)
#pragma warning(disable : 4459) /* hide global declaration */
#endif                          /* _MSC_VER > 1600 */
#endif                          /* _MSC_VER > 1000 */
#pragma warning(disable : 4761) /* integral size mismatch in arg; conv \
                                   supp*/
#ifdef YYPREFIX
#pragma warning(disable : 4102) /* unreferenced label */
#endif
#ifdef __cplusplus
/* suppress a warning in cppregex.cpp */
#pragma warning(disable : 4101) /* unreferenced local variable */
#endif
#ifndef HAS_STDINT_H
#define HAS_STDINT_H    /* force include of stdint.h in integer.h */
#endif
#endif /* _MSC_VER */

/* The following is needed for prototypes of certain functions */
#if defined(_MSC_VER)
#include <process.h> /* Provides prototypes of exit(), spawn()      */
#endif

#include <string.h> /* Provides prototypes of strncmpi(), etc.     */
#ifdef STRNCMPI
#define strncmpi(a, b, c) strnicmp(a, b, c)
#endif

#ifdef _MSC_VER
/* Visual Studio defines this in their own headers, which we don't use */
#ifndef snprintf
#define snprintf _snprintf
#pragma warning( \
    disable : 4996) /* deprecation warning suggesting snprintf_s */
#endif
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __BORLANDC__
#undef randomize
#undef random
#endif

#define PATHLEN BUFSZ  /* maximum pathlength */
#define FILENAME BUFSZ /* maximum filename length (conservative) */

#if defined(_MAX_PATH) && defined(_MAX_FNAME)
#if (_MAX_PATH < BUFSZ) && (_MAX_FNAME < BUFSZ)
#undef PATHLEN
#undef FILENAME
#define PATHLEN _MAX_PATH
#define FILENAME _MAX_FNAME
#endif
#endif

#define index strchr
#define rindex strrchr

/* Time stuff */
#include <time.h>

#define USE_STDARG

/* Use the high quality random number routines. */
#ifdef USE_ISAAC64
#undef RANDOM
#else
#define RANDOM
#define Rand() random()
#endif

/* Fall back to C's if nothing else, but this really isn't acceptable */
#if !defined(USE_ISAAC64) && !defined(RANDOM)
#define Rand() rand()
#endif

#include <sys/stat.h>
#define FCMASK (_S_IREAD | _S_IWRITE) /* file creation mask */
#define regularize nt_regularize
#define HLOCK "NHPERM"

#if defined(DLB)
#define FILENAME_CMP stricmp /* case insensitive */
#endif

#ifndef M
#define M(c) ((char) (0x80 | (c)))
/* #define M(c)        ((c) - 128) */
#endif

#ifndef C
#define C(c) (0x40 & (c) ? 0x1f & (c) : (0x80 | (0x1f & (c))))
#endif

#define ABORT C('a')
#define getuid() 1
#define getlogin() ((char *) 0)
extern void NDECL(win32_abort);

#include <fcntl.h>
#include <io.h>
#include <direct.h>
#ifndef CURSES_GRAPHICS
#include <conio.h>      /* conflicting definitions with curses.h */
#endif
#undef kbhit /* Use our special NT kbhit */
#define kbhit (*nt_kbhit)

#ifdef NDEBUG
#define nhassert(expression) ((void)0)
#else
extern void FDECL(nhassert_failed, (const char* exp, const char* file,
    int line));

#define nhassert(expression) (void)((!!(expression)) || \
        (nhassert_failed(#expression, __FILE__, __LINE__), 0))
#endif

#endif /* WINCONF_H */
#endif /* GNH_WIN */

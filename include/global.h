/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    global.h    $NHDT-Date: 1557254325 2019/05/07 18:38:45 $  $NHDT-Branch: GnollHack-3.6.2 $:$NHDT-Revision: 1.71 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2006. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>

/* #define BETA */  /* development or beta testing [MRS] */

#ifndef GNH_MOBILE /* release version for mobile builds */
#ifndef DEBUG  /* allow tool chains to define without causing warnings */
#define DEBUG
#endif
#endif

#if 0 /* Toggle memory leak detection on and off */
#ifndef GNH_CRTDBG
#define GNH_CRTDBG
#endif
#endif

/*
 * Files expected to exist in the playground directory.
 */

#define RECORD "record"         /* file containing list of topscorers */
#define HELP "help"             /* file containing command descriptions */
#define SHELP "hh"              /* abbreviated form of the same */
#define KEYHELP "keyhelp"       /* explanatory text for 'whatdoes' command */
#define DEBUGHELP "wizhelp"     /* file containing debug mode cmds */
#define RUMORFILE "rumors"      /* file with fortune cookies */
#define ORACLEFILE "oracles"    /* file with oracular information */
#define DATAFILE "data"         /* file giving the meaning of symbols used */
#define CMDHELPFILE "cmdhelp"   /* file telling what commands do */
#define HISTORY "history"       /* file giving GnollHack's history */
#define LICENSE "license"       /* file with license information */
#define CREDITSFILE "credits"   /* file with GnollHack specific credits */
#define OPTIONFILE "opthelp"    /* file explaining runtime options */
#define OPTIONS_USED "options"  /* compile-time options, for #version */
#define SYMBOLS "symbols"       /* replacement symbol sets */
#define EPITAPHFILE "epitaph"   /* random epitaphs on graves */
#define ENGRAVEFILE "engrave"   /* random engravings on the floor */
#define BOGUSMONFILE "bogusmon" /* hallucinatory monsters */
#define TRIBUTEFILE "tribute"   /* 3.6 tribute to Terry Pratchett */
#define LEV_EXT ".lev"          /* extension for special level files */

/* Assorted definitions that may depend on selections in config.h. */

/*
 * for DUMB preprocessor and compiler, e.g., cpp and pcc supplied
 * with Microport SysV/AT, which have small symbol tables;
 * DUMB if needed is defined in CFLAGS
 */
#ifdef DUMB
#ifdef BITFIELDS
#undef BITFIELDS
#endif
#ifndef STUPID
#define STUPID
#endif
#endif /* DUMB */

/*
 * type xchar: small integers in the range 0 - 127, usually coordinates
 * although they are nonnegative they must not be declared unsigned
 * since otherwise comparisons with signed quantities are done incorrectly
 */
typedef schar xchar;
//#if defined(__GNUC__) && defined(WIN32) && defined(__cplusplus)
#ifdef __MINGW32__
/* Resolve conflict with Qt 5 and MinGW-w32 */
typedef uchar boolean; /* 0 or 1 */
#else
#ifndef SKIP_BOOLEAN
typedef xchar boolean; /* 0 or 1 */
#endif
#endif

#ifndef TRUE /* defined in some systems' native include files */
#define TRUE 1 //((boolean) 1)
#define FALSE 0 //((boolean) 0)
#endif

#ifndef STRNCMPI
#ifndef __SASC_60 /* SAS/C already shifts to stricmp */
#define strcmpi(a, b) strncmpi((a), (b), -1)
#endif
#endif

/* comment out to test effects of each #define -- these will probably
 * disappear eventually
 */
#ifdef INTERNAL_COMP
#define RLECOMP  /* run-length compression of levl array - JLee */
#define ZEROCOMP /* zero-run compression of everything - Olaf Seibert */
#endif

/* #define SPECIALIZATION */ /* do "specialized" version of new topology */

#ifdef BITFIELDS
#define Bitfield(x, n) unsigned x : n
#else
#define Bitfield(x, n) uchar x
#endif

#define SIZE(x) (int)(sizeof(x) / sizeof(x[0]))

/* A limit for some GnollHack int variables.  It need not, and for comparable
 * scoring should not, depend on the actual limit on integers for a
 * particular machine, although it is set to the minimum required maximum
 * signed integer for C (2^15 -1).
 */
#define LARGEST_INT 32767

#include "coord.h"
/*
 * Automatic inclusions for the subsidiary files.
 * Please don't change the order.  It does matter.
 */

#ifdef VMS
#include "vmsconf.h"
#endif

#if defined (UNIX)
#include "unixconf.h"
#endif

#ifdef OS2
#include "os2conf.h"
#endif

#ifdef MSDOS
#include "pcconf.h"
#endif

#ifdef TOS
#include "tosconf.h"
#endif

#ifdef AMIGA
#include "amiconf.h"
#endif

#ifdef MAC
#include "macconf.h"
#endif

#ifdef __BEOS__
#include "beconf.h"
#endif

#ifdef WIN32
#if defined(WIN_CE)
#include "wceconf.h"
#endif
#if !defined(GNH_WIN)
#include "ntconf.h"
#endif
#endif

#ifdef ANDROID
#include "androidconf.h"
#endif

#ifdef GNH_ANDROID
#include "droidconf.h"
#endif

#ifdef GNH_IOS
#include "iosconf.h"
#endif

#ifdef GNH_WIN
#include "winconf.h"
#endif

/* Displayable name of this port; don't redefine if defined in *conf.h */
#ifndef PORT_ID
#ifdef AMIGA
#define PORT_ID "Amiga"
#endif
#ifdef MAC
#define PORT_ID "Mac"
#endif
#if defined(__APPLE__)  && !defined(GNH_MOBILE)
#define PORT_ID "MacOSX"
#endif
#ifdef MSDOS
#ifdef PC9800
#define PORT_ID "PC-9800"
#else
#define PORT_ID "PC"
#endif
#ifdef DJGPP
#define PORT_SUB_ID "djgpp"
#else
#ifdef OVERLAY
#define PORT_SUB_ID "overlaid"
#else
#define PORT_SUB_ID "non-overlaid"
#endif
#endif
#endif
#ifdef OS2
#define PORT_ID "OS/2"
#endif
#ifdef TOS
#define PORT_ID "ST"
#endif
/* Check again in case something more specific has been defined above. */
#ifndef PORT_ID
#ifdef UNIX
#define PORT_ID "Unix"
#endif
#endif
#ifdef VMS
#define PORT_ID "VMS"
#endif
#ifdef WIN32
#define PORT_ID "Windows"
#endif
#endif

#if defined(MICRO)
#if !defined(AMIGA) && !defined(TOS) && !defined(OS2_HPFS)
#define SHORT_FILENAMES /* filenames are 8.3 */
#endif
#endif

#if (!defined(MAC) && !defined(O_WRONLY) && !defined(AZTEC_C)) \
    || defined(USE_FCNTL)
#include <fcntl.h>
#endif

/* Some really stupid OSes treat binary and text files differently.
 * They need O_TEXT flag passed to open() for text files.
 * Making this 0 will have no effect on systems where write() actually
 * writes the contents of the buffer without arbitrarily modifying it.
 */
#ifndef O_TEXT
#define O_TEXT 0
#endif

#ifdef VMS
/* vms_exit() (sys/vms/vmsmisc.c) expects the non-VMS EXIT_xxx values below.
 * these definitions allow all systems to be treated uniformly, provided
 * main() routines do not terminate with return(), whose value is not
 * so massaged.
 */
#ifdef EXIT_SUCCESS
#undef EXIT_SUCCESS
#endif
#ifdef EXIT_FAILURE
#undef EXIT_FAILURE
#endif
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#if !defined(NO_TILES)
#if defined(X11_GRAPHICS) || defined(QT_GRAPHICS) || defined(GNOME_GRAPHICS) \
    || defined(WIN32) || defined(ANDROID_GRAPHICS) || defined(LIB_GRAPHICS) || defined(DLL_GRAPHICS)
#ifndef USE_TILES
#define USE_TILES /* glyph2tile[] will be available */
#endif
#endif
#if defined(AMII_GRAPHICS) || defined(GEM_GRAPHICS)
#ifndef USE_TILES
#define USE_TILES
#endif
#endif
#endif

#if defined(UNIX) || defined(VMS) || defined(__EMX__) || defined(WIN32)
#define HANGUPHANDLING
#endif
#if defined(SAFERHANGUP) \
    && (defined(NOSAVEONHANGUP) || !defined(HANGUPHANDLING))
#undef SAFERHANGUP
#endif

#define Sprintf (void) sprintf
#define Strcat (void) strcat
#define Strcpy (void) strcpy
#define Strncpy (void) strncpy
#ifdef NEED_VARARGS
#define Vprintf (void) vprintf
#define Vfprintf (void) vfprintf
#define Vsprintf (void) vsprintf
#endif

/* primitive memory leak debugging; see alloc.c */
#ifdef MONITOR_HEAP
extern int64_t *FDECL(nhalloc, (size_t, const char *, int));
extern void FDECL(nhfree, (genericptr_t, const char *, int));
extern char *FDECL(nhdupstr, (const char *, const char *, int));
#ifndef __FILE__
#define __FILE__ ""
#endif
#ifndef __LINE__
#define __LINE__ 0
#endif
#define alloc(a) nhalloc(a, __FILE__, (int) __LINE__)
#define free(a) nhfree(a, __FILE__, (int) __LINE__)
#define dupstr(s) nhdupstr(s, __FILE__, (int) __LINE__)
#else /* !MONITOR_HEAP */
extern long *FDECL(alloc, (size_t));  /* alloc.c */
extern char *FDECL(dupstr, (const char *)); /* ditto */
#endif
extern char* FDECL(setstr, (const char*, CHAR_P)); /* ditto */
extern char* FDECL(cpystr, (const char*, const char*)); /* ditto */

#include "integer.h"

/* Used for consistency checks of various data files; declare it here so
   that utility programs which include config.h but not hack.h can see it. */
struct version_info {
    uint64_t incarnation;   /* actual version number */
    uint64_t feature_set;   /* bitmask of config settings */
    uint64_t entity_count;  /* # of monsters and objects */
    uint64_t struct_sizes1; /* size of key structs */
    uint64_t struct_sizes2; /* size of more key structs */
    unsigned char short_size;
    unsigned char int_size;
    unsigned char long_size;
    unsigned char ptr_size;
    uint64_t version_compatibility; /* used in cases where an older version of GnollHack tries to load a compatible but newer saved game; tells what is the oldest compatible version */
};

struct savefile_info {
    uint64_t sfi1; /* compression etc. */
    uint64_t sfi2; /* miscellaneous */
    uint64_t sfi3; /* thirdparty */
};

#ifdef NHSTDC
#define SFI1_EXTERNALCOMP (1UL)
#define SFI1_RLECOMP (1UL << 1)
#define SFI1_ZEROCOMP (1UL << 2)
#else
#define SFI1_EXTERNALCOMP (1L)
#define SFI1_RLECOMP (1L << 1)
#define SFI1_ZEROCOMP (1L << 2)
#endif

/*
 * Configurable internal parameters.
 *
 * Please be very careful if you are going to change one of these.  Any
 * changes in these parameters, unless properly done, can render the
 * executable inoperative.
 */

/* size of terminal screen is (at least) (ROWNO+3) by COLNO */
#define COLNO 80
#define ROWNO 21

#define MAXNROFROOMS 40 /* max number of rooms per level */
#define MAX_SUBROOMS 24 /* max # of subrooms in a given room */
#define DOORMAX 120     /* max number of doors per level */

#define PL_NSIZ 32 /* name of player, ghost, shopkeeper */
#define PL_CSIZ 32 /* sizeof pl_character */
#define PL_FSIZ 32 /* fruit name */
#define PL_PSIZ 63 /* player-given names for pets, other monsters, objects */

#define BUFSZ 256  /* for getlin buffers */
#define QBUFSZ 256 /* for building question text */
#define IBUFSZ (BUFSZ * 3)  /* for getlin introline buffers */
#define PBUFSZ (IBUFSZ + QBUFSZ + BUFSZ)  /* for getlin combined introline + question (+ extras) buffers */
#define TBUFSZ 300 /* toplines[] buffer max msg: 3 81char names */
#define PREFIXBUFSZ 128 /* for object prefix buffer; "an uncursed greased partly eaten guardian naga hatchling [corpse]" and "an uncursed +10 erodeproof greased poisoned death-magical witch-king's primordial adamantium [long sword]" */
#define OBUFSZ (BUFSZ * 2 + PL_FSIZ + PL_PSIZ * 2 + PREFIXBUFSZ)  /* for object name buffers */
#define UTF8BUFSZ (BUFSZ * 4)  /* for getlin buffers in UTF8 */
#define UTF8QBUFSZ (QBUFSZ * 4)  /* for building question text in UTF8 */
#define UTF8IBUFSZ (IBUFSZ * 4)  /* for getlin introline buffers in UTF8 */

/* plus longest prefix plus a few extra words */

#define MAXDUNGEON 16 /* current maximum number of dungeons */
#define MAXLEVEL 32   /* max number of levels in one dungeon */
#define MAXSTAIRS 1   /* max # of special stairways in a dungeon */
#define ALIGNWEIGHT 4 /* generation weight of alignment */

#define MAXULEV 50 /* max character experience level */
#define MAX_MONSTER_LEVEL 127 /* max m_lev for a monster, maximum value for a signed char */

#define MAXMONNO 240 /* extinct monst after this number created (JG -- doubled) */
#define MHPMAX 500   /* maximum monster hp (JG -- seems to apply only to worms...) */

/* PANICTRACE: Always defined for BETA but only for supported platforms. */
#ifdef UNIX
#ifdef BETA
/* see end.c */
#ifndef PANICTRACE
#define PANICTRACE
#endif
#endif
#endif
/* The following are meaningless if PANICTRACE is not defined: */
#if defined(__linux__) && defined(__GLIBC__) && (__GLIBC__ >= 2)
#define PANICTRACE_LIBC
#endif
#if defined(MACOSX)
#define PANICTRACE_LIBC
#endif
#ifdef UNIX
#define PANICTRACE_GDB
#endif

/* LIVELOG message type flags */
#define LL_WISH       0x0001 /* Report stuff people type at the wish prompt. */
#define LL_ACHIEVE    0x0002 /* Achievements bitfield + invocation, planes */
#define LL_UMONST     0x0004 /* Kill, Bribe or otherwise dispatch unique monsters */
#define LL_DIVINEGIFT 0x0008 /* Sacrifice gifts, crowning */
#define LL_LIFESAVE   0x0010 /* Use up amulet of lifesaving */
#define LL_CONDUCT    0x0020 /* Break conduct - not reported early-game */
#define LL_ARTIFACT   0x0040 /* Excalibur, Sting, Orcrist, plus sac gifts and artwishes */
#define LL_GENOCIDE   0x0080 /* Logging of genocides */
#define LL_KILLEDPET  0x0100 /* Killed a tame monster */
#define LL_ALIGNMENT  0x0200 /* changed alignment temporarily or permanently */
#define LL_DUMP_ASC   0x0400 /* Log URL for dumplog if ascended */
#define LL_DUMP_ALL   0x0800 /* Log dumplog url for all games */
#define LL_MINORAC    0x1000 /* Log 'minor' achievements - can be spammy */
#define LL_SPOILER    0x2000 /* reveals information so don't show in-game
                               * via #chronicle unless in wizard mode */
#define LL_DUMP       0x4000 /* none of the above but should be in dumplog */
#define LL_DEBUG      0x8000 /* For debugging messages and other spam */

/* #chronicle details */
/* 'major' events for dumplog; inclusion or exclusion here may need tuning */
#define LL_majors                                                        \
    (0L | LL_WISH | LL_ACHIEVE | LL_UMONST | LL_DIVINEGIFT | LL_LIFESAVE \
     | LL_ARTIFACT | LL_GENOCIDE | LL_DUMP) /* explicitly for dumplog */
#define majorevent(m) (((m)->flags & LL_majors) != 0)
#define spoilerevent(m) (((m)->flags & LL_SPOILER) != 0)

#define LL_postables                                                        \
    (LL_WISH | LL_ACHIEVE | LL_UMONST | LL_DIVINEGIFT | LL_LIFESAVE \
     | LL_ARTIFACT | LL_GENOCIDE) /* explicitly for forum posting */


/* Supply GnollHack_enter macro if not supplied by port */
#ifndef GnollHack_enter
#define GnollHack_enter(argc, argv) ((void) 0)
#endif


#endif /* GLOBAL_H */

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    files.c    $NHDT-Date: 1546144856 2018/12/30 04:40:56 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.249 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Derek S. Ray, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

#define NEED_VARARGS

#include "hack.h"
#include "dlb.h"
#ifdef UNIX
#include <stdio.h>
#endif
#ifdef TTY_GRAPHICS
#include "wintty.h" /* more() */
#endif

#if (!defined(MAC) && !defined(O_WRONLY) && !defined(AZTEC_C)) \
    || defined(USE_FCNTL)
#include <fcntl.h>
#endif

#include <errno.h>
#ifdef _MSC_VER /* MSC 6.0 defines errno quite differently */
#if (_MSC_VER >= 600)
#define SKIP_ERRNO
#endif
#else
#ifdef NHSTDC
#define SKIP_ERRNO
#endif
#endif
#ifndef SKIP_ERRNO
#ifdef _DCC
const
#endif
    extern int errno;
#endif

#ifdef ZLIB_COMP /* RLC 09 Mar 1999: Support internal ZLIB */
#include "zlib.h"
#ifndef COMPRESS_EXTENSION
#define COMPRESS_EXTENSION ".gz"
#endif
#endif

#if defined(UNIX) && (defined(QT_GRAPHICS) || defined(ANDROID) || defined(GNH_MOBILE))
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/resource.h>
#ifdef GNH_IOS
#include <fcntl.h>
#include <errno.h>
#include <sys/param.h>
#endif
#endif

#if defined(UNIX) || defined(VMS) || !defined(NO_SIGNAL)
#include <signal.h>
#endif

#if defined(MSDOS) || defined(OS2) || defined(TOS) || defined(WIN32)
#ifndef GNUDOS
#include <sys\stat.h>
#else
#include <sys/stat.h>
#endif
#elif (defined (EXTRAINFO_FN) || defined (WHEREIS_FILE)) && defined(UNIX)
#include <sys/stat.h>
#endif

#ifndef O_BINARY /* used for micros, no-op for others */
#define O_BINARY 0
#endif

#ifdef WHEREIS_FILE
char whereis_file[255] = WHEREIS_FILE;
#endif

#ifdef PREFIXES_IN_USE
#define FQN_NUMBUF 4
STATIC_VAR char fqn_filename_buffer[FQN_NUMBUF][FQN_MAX_FILENAME];
#endif

#if !defined(MFLOPPY) && !defined(VMS) && !defined(WIN32)
char bones[] = "bones-ynn.xxx";
char lock[PL_NSIZ + 16] = "1lock"; /* long enough for uid+name+.99 */
#else
#if defined(MFLOPPY)
char bones[FILENAME]; /* pathname of bones files */
char lock[FILENAME];  /* pathname of level files */
#endif
#if defined(VMS)
char bones[] = "bones-ynn.xxx;1";
char lock[PL_NSIZ + 19] = "1lock"; /* long enough for _uid+name+.99;1 */
#endif
#if defined(WIN32)
char bones[] = "bones-ynn.xxx";
char lock[PL_NSIZ + 27]; /* long enough for username+-+name+.99 */
#endif
#endif

#if defined(UNIX) || defined(__BEOS__) || defined(GNH_MOBILE)
#define SAVESIZE (PL_NSIZ + 13) /* save/99999player.e */
#else
#ifdef VMS
#define SAVESIZE (PL_NSIZ + 22) /* [.save]<uid>player.e;1 */
#else
#if defined(WIN32)
#define SAVESIZE (PL_NSIZ + 40) /* username-player.GnollHack-saved-game */
#else
#define SAVESIZE FILENAME /* from macconf.h or pcconf.h */
#endif
#endif
#endif

#if !defined(SAVE_EXTENSION)
#ifdef MICRO
#define SAVE_EXTENSION ".sav"
#endif
#if defined (WIN32) && !defined(GNH_MOBILE)
#define SAVE_EXTENSION ".GnollHack-saved-game"
#endif
#endif

#define ERROR_EXTENSION "e"          /* extension for error save files */
#define IMPORTED_EXTENSION "i"       /* extension for imported save files */
#define BACKUP_EXTENSION "bup"       /* extension for backup save files */
#define ALT_BACKUP_EXTENSION "bak"   /* extension for backup save files (alternative) */
#define TEMP_BACKUP_EXTENSION "tmp"  /* extension for temp backup save files */
#define SAVE_FILE_TRACKING_EXTENSION "ghsft"  /* extension for save file tracking files */

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

char SAVEF[SAVESIZE]; /* holds relative path of save file from playground */
#ifdef MICRO
char SAVEP[SAVESIZE]; /* holds path of directory for save file */
#endif

#ifdef HOLD_LOCKFILE_OPEN
struct level_ftrack {
    int init;
    int fd;    /* file descriptor for level file     */
    int oflag; /* open flags                         */
    boolean GnollHack_thinks_it_is_open; /* Does GnollHack think it's open? */
} lftrack;
#if defined(WIN32)
#include <share.h>
#endif
#endif /*HOLD_LOCKFILE_OPEN*/

#define WIZKIT_MAX 128
STATIC_VAR char wizkit[WIZKIT_MAX];
STATIC_DCL FILE *NDECL(fopen_wizkit_file);
STATIC_DCL void FDECL(wizkit_addinv, (struct obj *));
#ifndef MICRO
STATIC_DCL void FDECL(print_special_savefile_extension, (char*, const char*));
STATIC_DCL void FDECL(print_error_savefile_extension, (char*));
STATIC_DCL void FDECL(print_imported_savefile_extension, (char*));
#endif
#ifdef WHEREIS_FILE
STATIC_DCL void FDECL(write_whereis, (int));
#endif

#ifdef AMIGA
extern char PATH[]; /* see sys/amiga/amidos.c */
extern char bbs_id[];
STATIC_VAR int lockptr;
#ifdef __SASC_60
#include <proto/dos.h>
#endif

#include <libraries/dos.h>
extern void FDECL(amii_set_text_font, (char *, int));
#endif

#if defined(WIN32) || defined(MSDOS)
STATIC_VAR int lockptr;
#ifdef MSDOS
#define Delay(a) msleep(a)
#endif
#define Close close
#ifndef WIN_CE
#define DeleteFile unlink
#endif
#endif

#ifdef MAC
#undef unlink
#define unlink macunlink
#endif

#if (defined(macintosh) && (defined(__SC__) || defined(__MRC__))) \
    || defined(__MWERKS__)
#define PRAGMA_UNUSED
#endif

#ifdef USER_SOUNDS
extern char *sounddir;
#endif

extern int n_dgns; /* from dungeon.c */

#if defined(UNIX) && defined(QT_GRAPHICS)
#define SELECTSAVED
#endif

#ifdef SELECTSAVED
STATIC_PTR int FDECL(CFDECLSPEC savegamedata_strcmp_wrap, (const void *, const void *));
#endif
STATIC_DCL char *FDECL(set_bonesfile_name, (char *, d_level *));
STATIC_DCL char *NDECL(set_bonestemp_name);
#ifdef COMPRESS
STATIC_DCL void FDECL(redirect, (const char *, const char *, FILE *,
                                 BOOLEAN_P));
#endif
#if defined(COMPRESS) || defined(ZLIB_COMP)
STATIC_DCL void FDECL(docompress_file, (const char *, BOOLEAN_P));
#endif
#if defined(ZLIB_COMP)
STATIC_DCL boolean FDECL(make_compressed_name, (const char *, char *));
#endif
#ifndef USE_FCNTL
STATIC_DCL char *FDECL(make_lockname, (const char *, char *));
#endif
STATIC_DCL void FDECL(set_configfile_name, (const char *));
STATIC_DCL FILE *FDECL(fopen_config_file, (const char *, int));
STATIC_DCL int FDECL(get_nhsyms, (char*, nhsym*, BOOLEAN_P, int, const char*));
boolean FDECL(proc_wizkit_line, (char *));
boolean FDECL(parse_config_line, (char *));
STATIC_DCL boolean FDECL(parse_conf_file, (FILE *, boolean (*proc)(char *)));
STATIC_DCL FILE *NDECL(fopen_sym_file);
boolean FDECL(proc_symset_line, (char *));
STATIC_DCL void FDECL(set_symhandling, (char *, int));
#ifdef NOCWD_ASSUMPTIONS
STATIC_DCL void FDECL(adjust_prefix, (char *, int));
#endif
STATIC_DCL boolean FDECL(config_error_nextline, (const char *));
STATIC_DCL void NDECL(free_config_sections);
STATIC_DCL char *FDECL(choose_random_part, (char *, CHAR_P));
STATIC_DCL boolean FDECL(is_config_section, (const char *));
STATIC_DCL boolean FDECL(handle_config_section, (char *));
#ifdef SELF_RECOVER
STATIC_DCL boolean FDECL(copy_bytes, (int, int));
#endif
#ifdef HOLD_LOCKFILE_OPEN
STATIC_DCL int FDECL(open_levelfile_exclusively, (const char *, int, int));
#endif

STATIC_DCL void FDECL(livelog_write_string, (unsigned int, const char*));
STATIC_DCL void FDECL(livelog_post_to_forum, (unsigned int, const char*));
STATIC_DCL void FDECL(livelog_post_to_forum_rt, (unsigned int, struct u_realtime, const char*));
STATIC_DCL int FDECL(copy_savefile, (const char*, const char*));

#define INBUF_SIZ 8 * BUFSZ

STATIC_VAR char config_section_chosen[INBUF_SIZ]; // = (char*)0;
STATIC_VAR char config_section_current[INBUF_SIZ]; // = (char*)0;

/*
 * fname_encode()
 *
 *   Args:
 *      legal       zero-terminated list of acceptable file name characters
 *      quotechar   lead-in character used to quote illegal characters as
 *                  hex digits
 *      s           string to encode
 *      callerbuf   buffer to house result
 *      bufsz       size of callerbuf
 *
 *   Notes:
 *      The hex digits 0-9 and A-F are always part of the legal set due to
 *      their use in the encoding scheme, even if not explicitly included in
 *      'legal'.
 *
 *   Sample:
 *      The following call:
 *  (void)fname_encode("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
 *                     '%', "This is a % test!", buf, 512);
 *      results in this encoding:
 *          "This%20is%20a%20%25%20test%21"
 */
char *
fname_encode(legal, quotechar, s, callerbuf, bufsz)
const char *legal;
char quotechar;
char *s, *callerbuf;
int bufsz;
{
    char *sp, *op;
    int cnt = 0;
    static const char hexdigits[] = "0123456789ABCDEF";

    sp = s;
    op = callerbuf;
    *op = '\0';

    while (*sp) {
        /* Do we have room for one more character or encoding? */
        if ((bufsz - cnt) <= 4)
            return callerbuf;

        if (*sp == quotechar) {
            (void) sprintf(op, "%c%02X", quotechar, *sp);
            op += 3;
            cnt += 3;
        } else if ((index(legal, *sp) != 0) || (index(hexdigits, *sp) != 0)) {
            *op++ = *sp;
            *op = '\0';
            cnt++;
        } else {
            (void) sprintf(op, "%c%02X", quotechar, *sp);
            op += 3;
            cnt += 3;
        }
        sp++;
    }
    return callerbuf;
}

int is_imported_error_savefile_name(savefilename)
const char* savefilename;
{
    if (!savefilename || !*savefilename)
        return FALSE;

    size_t dlen = strlen(savefilename);
    char ebuf[BUFSZ] = "";
    print_error_savefile_extension(ebuf);
    print_imported_savefile_extension(ebuf);
    size_t elen = strlen(ebuf);
    if (dlen <= elen)
        return FALSE;

    size_t i;
    for (i = 0; i < elen; i++)
        if (savefilename[dlen - 1 - i] != ebuf[elen - 1 - i])
            return FALSE;

    return TRUE;
}

int is_imported_backup_savefile_name(savefilename)
const char* savefilename;
{
    if (!savefilename || !*savefilename)
        return FALSE;

    size_t dlen = strlen(savefilename);
    char ebuf[BUFSZ] = "";
    print_special_savefile_extension(ebuf, BACKUP_EXTENSION);
    print_imported_savefile_extension(ebuf);
    size_t elen = strlen(ebuf);
    if (dlen <= elen)
        return FALSE;

    size_t i;
    for (i = 0; i < elen; i++)
        if (savefilename[dlen - 1 - i] != ebuf[elen - 1 - i])
            return FALSE;

    return TRUE;
}

int is_save_file_tracking_file_name(savefilename)
const char* savefilename;
{
    if (!savefilename || !*savefilename)
        return FALSE;

    size_t dlen = strlen(savefilename);
    char ebuf[BUFSZ] = "";
    print_special_savefile_extension(ebuf, SAVE_FILE_TRACKING_EXTENSION);
    size_t elen = strlen(ebuf);
    if (dlen <= elen)
        return FALSE;

    size_t i;
    for (i = 0; i < elen; i++)
        if (savefilename[dlen - 1 - i] != ebuf[elen - 1 - i])
            return FALSE;

    return TRUE;
}

int is_backup_savefile_name(savefilename)
const char* savefilename;
{
    if (!savefilename || !*savefilename)
        return FALSE;

    size_t dlen = strlen(savefilename);
    char ebuf[BUFSZ] = "";
    const char* extensions[] = { BACKUP_EXTENSION, ALT_BACKUP_EXTENSION, TEMP_BACKUP_EXTENSION, 0 };
    int j;
    for (j = 0; extensions[j] != 0; j++)
    {
        print_special_savefile_extension(ebuf, extensions[j]);
        size_t elen = strlen(ebuf);
        if (dlen <= elen)
            continue;

        size_t i;
        boolean docontinue = FALSE;
        for (i = 0; i < elen; i++)
            if (savefilename[dlen - 1 - i] != ebuf[elen - 1 - i])
            {
                docontinue = TRUE;
                break;
            }

        if (docontinue)
            continue;

        return TRUE;
    }
    return FALSE;
}

/*
 * fname_decode()
 *
 *   Args:
 *      quotechar   lead-in character used to quote illegal characters as
 *                  hex digits
 *      s           string to decode
 *      callerbuf   buffer to house result
 *      bufsz       size of callerbuf
 */
char *
fname_decode(quotechar, s, callerbuf, bufsz)
char quotechar;
char *s, *callerbuf;
int bufsz;
{
    char *sp, *op;
    int k, calc, cnt = 0;
    static const char hexdigits[] = "0123456789ABCDEF";

    sp = s;
    op = callerbuf;
    *op = '\0';
    calc = 0;

    while (*sp) {
        /* Do we have room for one more character? */
        if ((bufsz - cnt) <= 2)
            return callerbuf;
        if (*sp == quotechar) {
            sp++;
            for (k = 0; k < 16; ++k)
                if (*sp == hexdigits[k])
                    break;
            if (k >= 16)
                return callerbuf; /* impossible, so bail */
            calc = k << 4;
            sp++;
            for (k = 0; k < 16; ++k)
                if (*sp == hexdigits[k])
                    break;
            if (k >= 16)
                return callerbuf; /* impossible, so bail */
            calc += k;
            sp++;
            *op++ = calc;
            *op = '\0';
        } else {
            *op++ = *sp++;
            *op = '\0';
        }
        cnt++;
    }
    return callerbuf;
}

#ifdef PREFIXES_IN_USE
#define UNUSED_if_not_PREFIXES_IN_USE /*empty*/
#else
#define UNUSED_if_not_PREFIXES_IN_USE UNUSED
#endif

/*ARGSUSED*/
const char *
fqname(basenam, whichprefix, buffnum)
const char *basenam;
int whichprefix UNUSED_if_not_PREFIXES_IN_USE;
int buffnum UNUSED_if_not_PREFIXES_IN_USE;
{
#ifndef PREFIXES_IN_USE
    return basenam;
#else
    if (!basenam || whichprefix < 0 || whichprefix >= PREFIX_COUNT)
        return basenam;
    if (!fqn_prefix[whichprefix])
        return basenam;
    if (buffnum < 0 || buffnum >= FQN_NUMBUF) {
        impossible("Invalid fqn_filename_buffer specified: %d", buffnum);
        buffnum = 0;
    }
    if (strlen(fqn_prefix[whichprefix]) + strlen(basenam)
        >= FQN_MAX_FILENAME) {
        impossible("fqname too long: %s + %s", fqn_prefix[whichprefix],
                   basenam);
        return basenam; /* XXX */
    }
    Strcpy(fqn_filename_buffer[buffnum], fqn_prefix[whichprefix]);
    Strcat(fqn_filename_buffer[buffnum], basenam);

    return fqn_filename_buffer[buffnum];
#endif
}

int
validate_prefix_locations(reasonbuf)
char *reasonbuf; /* reasonbuf must be at least BUFSZ, supplied by caller */
{
#if defined(NOCWD_ASSUMPTIONS)
    FILE *fp;
    const char *filename;
    int prefcnt, failcount = 0;
    char panicbuf1[BUFSZ], panicbuf2[BUFSZ];
    const char *details;
#endif

    if (reasonbuf)
        reasonbuf[0] = '\0';
#if defined(NOCWD_ASSUMPTIONS)
    for (prefcnt = 1; prefcnt < PREFIX_COUNT; prefcnt++) {
        /* don't test writing to configdir or datadir; they're readonly */
        if (prefcnt == SYSCONFPREFIX || prefcnt == CONFIGPREFIX
            || prefcnt == DATAPREFIX)
            continue;
        filename = fqname("validate", prefcnt, 3);
        if ((fp = fopen(filename, "w"))) {
            fclose(fp);
            (void) unlink(filename);
        } else {
            if (reasonbuf) {
                if (failcount)
                    Strcat(reasonbuf, ", ");
                Strcat(reasonbuf, fqn_prefix_names[prefcnt]);
            }
            /* the paniclog entry gets the value of errno as well */
            Sprintf(panicbuf1, "Invalid %s", fqn_prefix_names[prefcnt]);
#if defined(NHSTDC) && !defined(NOTSTDC)
            if (!(details = strerror(errno)))
#endif
                details = "";
            Sprintf(panicbuf2, "\"%s\", (%d) %s", fqn_prefix[prefcnt], errno,
                    details);
            paniclog(panicbuf1, panicbuf2);
            failcount++;
        }
    }
    if (failcount)
        return 0;
    else
#endif
        return 1;
}

/* fopen a file, with OS-dependent bells and whistles */
/* NOTE: a simpler version of this routine also exists in util/dlb_main.c */
FILE *
fopen_datafile(filename, mode, prefix)
const char *filename, *mode;
int prefix;
{
    FILE *fp;

    filename = fqname(filename, prefix, prefix == TROUBLEPREFIX ? 3 : 0);
    fp = fopen(filename, mode);
    return fp;
}

/* ----------  BEGIN LEVEL FILE HANDLING ----------- */

#ifdef MFLOPPY
/* Set names for bones[] and lock[] */
void
set_lock_and_bones()
{
    if (!ramdisk) {
        Strcpy(levels, permbones);
        Strcpy(bones, permbones);
    }
    append_slash(permbones);
    append_slash(levels);
#ifdef AMIGA
    strncat(levels, bbs_id, PATHLEN);
#endif
    append_slash(bones);
    Strcat(bones, "bonesnn.*");
    Strcpy(lock, levels);
#ifndef AMIGA
    Strcat(lock, alllevels);
#endif
    return;
}
#endif /* MFLOPPY */

/* Construct a file name for a level-type file, which is of the form
 * something.level (with any old level stripped off).
 * This assumes there is space on the end of 'file' to append
 * a two digit number.  This is true for 'level'
 * but be careful if you use it for other things -dgk
 */
void
set_levelfile_name(file, lev)
char *file;
int lev;
{
    char *tf;

    tf = rindex(file, '.');
    if (!tf)
        tf = eos(file);
    Sprintf(tf, ".%d", lev);
#ifdef VMS
    Strcat(tf, ";1");
#endif
    return;
}

int
create_levelfile(lev, errbuf)
int lev;
char errbuf[];
{
    int fd;
    const char *fq_lock;

    if (errbuf)
        *errbuf = '\0';
    set_levelfile_name(lock, lev);
    fq_lock = fqname(lock, LEVELPREFIX, 0);

#if defined(MICRO) || defined(WIN32)
/* Use O_TRUNC to force the file to be shortened if it already
 * exists and is currently longer.
 */
#ifdef HOLD_LOCKFILE_OPEN
    if (lev == 0)
        fd = open_levelfile_exclusively(
            fq_lock, lev, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY);
    else
#endif
        fd = open(fq_lock, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, FCMASK);
#else
#ifdef MAC
    fd = maccreat(fq_lock, LEVL_TYPE);
#else
    fd = creat(fq_lock, FCMASK);
#endif
#endif /* MICRO || WIN32 */
    //issue_debuglog_fd(fd, "create_levelfile");
    if (fd >= 0)
        level_info[lev].flags |= LFILE_EXISTS;
    else if (errbuf) /* failure explanation */
    {
        Sprintf(errbuf, "Cannot create file \"%s\" for level %d (errno %d).",
            lock, lev, errno);
        maybe_report_file_descriptors(errbuf, errno);
    }

    return fd;
}

#if defined(UNIX) && defined(GNH_MOBILE)
/* This changes the soft limit */
boolean
increase_file_descriptor_limit_to_at_least(new_cur_minimum)
uint64_t new_cur_minimum;
{
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) 
    {
        if (rlim.rlim_max != RLIM_INFINITY && rlim.rlim_max < (rlim_t)new_cur_minimum)
        {
            /* Limit the file descriptor number to lower of 32768 and current hard limit */
            rlim_t maxlimit = min((rlim_t)32768, rlim.rlim_max);
            new_cur_minimum = min(new_cur_minimum, (uint64_t)maxlimit);
        }
        if (rlim.rlim_cur != RLIM_INFINITY && rlim.rlim_cur < (rlim_t)new_cur_minimum)
            rlim.rlim_cur = (rlim_t)new_cur_minimum;
        if (setrlimit(RLIMIT_NOFILE, &rlim) == -1) 
        {
            /* Failed */
            return FALSE;
        }
        return TRUE;
    }
    else 
    {
        /* Failed */
        return FALSE;
    }
}

int
get_file_descriptor_limit(is_max_limit)
boolean is_max_limit;
{
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
    {
        if (is_max_limit)
        {
            if (rlim.rlim_max == RLIM_INFINITY)
                return -1;
            else
                return (int)rlim.rlim_max;
        }
        else
        {
            if (rlim.rlim_cur == RLIM_INFINITY)
                return -1;
            else
                return (int)rlim.rlim_cur;
        }
    }
    else
    {
        /* Failed */
        return -2;
    }
}

/* Dummy for non-Mac/iOS systems */
#ifndef F_GETPATH
#define F_GETPATH 127
#endif

char *
gnh_lsof(VOID_ARGS)
{
    char* outptr = 0;
#if defined (GNH_IOS) && defined(UNIX)
#define GNH_LSOF_BUFFER_SIZE (MAXPATHLEN + 1 + BUFSZ * 2)
#define GNH_LSOF_OUTPUT_BUFFER_SIZE (GNH_LSOF_BUFFER_SIZE + BUFSZ * 2)
    int flags;
    int fd;
    char buf[GNH_LSOF_BUFFER_SIZE];
    int n = 1;
    char outbuf[GNH_LSOF_OUTPUT_BUFFER_SIZE];

    for (fd = 0; fd < (int)FD_SETSIZE; fd++) {
        *buf = 0;
        *outbuf = 0;
        errno = 0;
        flags = fcntl(fd, F_GETFD, 0);
        if (flags == -1 && errno) {
            if (errno != EBADF) {
                return outptr;
            }
            else
                continue;
        }
        fcntl(fd, F_GETPATH, buf);
        buf[GNH_LSOF_BUFFER_SIZE - 1] = 0;
        Sprintf(outbuf, "File Descriptor %d (number %d) in use for: %s | ", fd, n, buf);
        if (!outptr)
        {
            outptr = (char*)alloc(strlen(outbuf) + 1);
            if (!outptr)
                break;
            Strcpy(outptr, outbuf);
        }
        else
        {
            char* newptr = (char*)alloc(strlen(outptr) + strlen(outbuf) + 1);
            if (!newptr)
                break;
            Strcpy(newptr, outptr);
            Strcat(newptr, outbuf);
            free((genericptr_t)outptr);
            outptr = newptr;
        }
        ++n;
    }
#undef GNH_LSOF_BUFFER_SIZE
#undef GNH_LSOF_OUTPUT_BUFFER_SIZE
#endif
    return outptr;
}
#endif

void
maybe_report_file_descriptors(logtext, error_number)
const char* logtext UNUSED;
int error_number UNUSED;
{
#if defined(UNIX) && defined(GNH_MOBILE)
    if (error_number == EMFILE)
    {
        char extbuf[BUFSZ] = "";
        struct rlimit rlim = { 0 };
        if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
        {
            char curbuf[BUFSZ];
            char maxbuf[BUFSZ];
            if (rlim.rlim_cur == RLIM_INFINITY)
                Strcpy(curbuf, "Inf");
            else
                Sprintf(curbuf, "%ld", (long)rlim.rlim_cur);

            if (rlim.rlim_max == RLIM_INFINITY)
                Strcpy(maxbuf, "Inf");
            else
                Sprintf(maxbuf, "%ld", (long)rlim.rlim_max);

            Sprintf(extbuf, " (c:%s, m:%s)", curbuf, maxbuf);
        }

#if defined(GNH_IOS)
        char* ptr = gnh_lsof();
        if (ptr)
        {
            if (issue_gui_command)
            {
                char titlebuf[BUFSZ];
                Strcpy(titlebuf, "gnh_lsof");
                Strcat(titlebuf, extbuf);
                issue_gui_command(GUI_CMD_POST_DIAGNOSTIC_DATA, DIAGNOSTIC_DATA_CREATE_ATTACHMENT_FROM_TEXT, DIAGNOSTIC_DATA_ATTACHMENT_FILE_DESCRIPTOR_LIST, ptr);
                issue_gui_command(GUI_CMD_POST_DIAGNOSTIC_DATA, DIAGNOSTIC_DATA_CRITICAL, 0, titlebuf);
            }
            free((genericptr_t)ptr);
        }
#else
        char cmd[BUFSZ];
        char msgbuf[BUFSZ] = "";
        //(void)mkdir("temp", 0777);
        int pid = (int)getpid();
        //char cwd[PATH_MAX];
        //char pathbuf[PATH_MAX + BUFSZ];
        //if (getcwd(cwd, sizeof(cwd)) != NULL)
        //    Sprintf(pathbuf, "%s/temp/file_descriptors.txt", cwd);
        //else
        //    Strcpy(pathbuf, "temp/file_descriptors.txt");

#ifdef GNH_ANDROID
        Sprintf(cmd, "lsof -p %d 2>&1", pid);
#else
        Sprintf(cmd, "ls -l /proc/%d/fd 2>&1", pid);
#endif
        FILE* poutput = popen(cmd, "r");
        if (poutput)
        {
            char buf[BUFSZ];
            char* outputbuf = 0;
            while (fgets(buf, BUFSZ - 1, poutput))
            {
                buf[BUFSZ - 1] = 0;
                if (!outputbuf)
                {
                    outputbuf = (char*)alloc(strlen(buf) + 1);
                    if (!outputbuf)
                        break;
                    Strcpy(outputbuf, buf);
                }
                else
                {
                    char* newoutbuf = (char*)alloc(strlen(outputbuf) + strlen(buf) + 1);
                    if (!newoutbuf)
                        break;
                    Strcpy(newoutbuf, outputbuf);
                    Strcat(newoutbuf, buf);
                    free((genericptr_t)outputbuf);
                    outputbuf = newoutbuf;
                }
            }
            pclose(poutput);

            if (logtext)
            {
                Strcpy(msgbuf, logtext);
                Strcat(msgbuf, " - ");
            }
            Strcat(msgbuf, cmd);
            Strcat(msgbuf, extbuf);

            if (issue_gui_command)
            {
                issue_gui_command(GUI_CMD_POST_DIAGNOSTIC_DATA, DIAGNOSTIC_DATA_CREATE_ATTACHMENT_FROM_TEXT, DIAGNOSTIC_DATA_ATTACHMENT_FILE_DESCRIPTOR_LIST, outputbuf ? outputbuf : "(No output)");
                issue_gui_command(GUI_CMD_POST_DIAGNOSTIC_DATA, DIAGNOSTIC_DATA_CRITICAL, 0, msgbuf);
            }
            if (outputbuf)
                free((genericptr_t)outputbuf);
        }
#endif
    }
#endif
}

int
open_levelfile(lev, errbuf)
int lev;
char errbuf[];
{
    int fd;
    const char *fq_lock;

    if (errbuf)
        *errbuf = '\0';
    set_levelfile_name(lock, lev);
    fq_lock = fqname(lock, LEVELPREFIX, 0);
#ifdef MFLOPPY
    /* If not currently accessible, swap it in. */
    if (level_info[lev].where != ACTIVE)
        swapin_file(lev);
#endif
#ifdef MAC
    fd = macopen(fq_lock, O_RDONLY | O_BINARY, LEVL_TYPE);
#else
#ifdef HOLD_LOCKFILE_OPEN
    if (lev == 0)
        fd = open_levelfile_exclusively(fq_lock, lev, O_RDONLY | O_BINARY);
    else
#endif
        fd = open(fq_lock, O_RDONLY | O_BINARY, 0);
#endif
    //issue_debuglog_fd(fd, "open_levelfile");

    /* for failure, return an explanation that our caller can use;
       settle for `lock' instead of `fq_lock' because the latter
       might end up being too big for GnollHack's BUFSZ */
    if (fd < 0 && errbuf)
    {
        Sprintf(errbuf, "Cannot open file \"%s\" for level %d (errno %d).",
            lock, lev, errno);
        maybe_report_file_descriptors(errbuf, errno);
    }
    return fd;
}

void
delete_levelfile(lev)
int lev;
{
    /*
     * Level 0 might be created by port specific code that doesn't
     * call create_levfile(), so always assume that it exists.
     */
    if (lev == 0 || (level_info[lev].flags & LFILE_EXISTS)) {
        set_levelfile_name(lock, lev);
#ifdef HOLD_LOCKFILE_OPEN
        if (lev == 0)
            really_close();
#endif
        (void) unlink(fqname(lock, LEVELPREFIX, 0));
        level_info[lev].flags &= ~LFILE_EXISTS;
    }
}

void
delete_excess_levelfiles(VOID_ARGS)
{
    /* These might happen if a backup save file is restored after crash */
    xchar ltmp;
    xchar maxnoofledgers = maxledgerno();
    for (ltmp = (xchar)1; ltmp <= maxnoofledgers; ltmp++) 
    {
        if ((level_info[ltmp].flags & LFILE_EXISTS))
            continue;

        int lev = (int)ltmp;
        set_levelfile_name(lock, lev);
        const char* fq_name = fqname(lock, LEVELPREFIX, 0);
        if (!access(fq_name, F_OK))
            (void)unlink(fq_name);
    }
}

void
clearlocks()
{
#ifdef HANGUPHANDLING
    if (program_state.preserve_locks)
        return;
#endif
#if !defined(PC_LOCKING) && defined(MFLOPPY) && !defined(AMIGA)
    eraseall(levels, alllevels);
    if (ramdisk)
        eraseall(permbones, alllevels);
#else
    {
        register int x;

#ifndef NO_SIGNAL
        (void) signal(SIGINT, SIG_IGN);
#endif
#if !defined(ANDROID) && !defined(GNH_MOBILE)
#if defined(UNIX) || defined(VMS)
        sethanguphandler((void FDECL((*), (int) )) SIG_IGN);
#endif
#endif
        /* can't access maxledgerno() before dungeons are created -dlc */
        for (x = (n_dgns ? maxledgerno() : 0); x >= 0; x--)
            delete_levelfile(x); /* not all levels need be present */
    }
#endif /* ?PC_LOCKING,&c */

#ifdef WHEREIS_FILE
    delete_whereis();
#endif

}

#if defined(SELECTSAVED)
/* qsort comparison routine */
STATIC_OVL int CFDECLSPEC
savegamedata_strcmp_wrap(p, q)
const void *p;
const void *q;
{
    if (!p || !q)
        return 0;

    struct save_game_data* savegame1 = (struct save_game_data*)p;
    struct save_game_data* savegame2 = (struct save_game_data*)q;
    int priority1 = savegame1->is_running ? 3 : savegame1->is_error_save_file ? 2 : savegame1->is_imported_save_file ? 1 : 0;
    int priority2 = savegame2->is_running ? 3 : savegame2->is_error_save_file ? 2 : savegame2->is_imported_save_file ? 1 : 0;
    if (priority1 != priority2)
        return priority2 - priority1;

#if defined(UNIX) && defined(QT_GRAPHICS)
    return strncasecmp((((struct save_game_data*)p)->playername), (((struct save_game_data*)q)->playername), 16);
#else
    return strncmpi((((struct save_game_data*)p)->playername), (((struct save_game_data*)q)->playername), 16);
#endif
}
#endif

#ifdef HOLD_LOCKFILE_OPEN
STATIC_OVL int
open_levelfile_exclusively(name, lev, oflag)
const char *name;
int lev, oflag;
{
    int reslt, fd;

    if (!lftrack.init) {
        lftrack.init = 1;
        lftrack.fd = -1;
    }
    if (lftrack.fd >= 0) {
        /* check for compatible access */
        if (lftrack.oflag == oflag) {
            fd = lftrack.fd;
            reslt = lseek(fd, 0L, SEEK_SET);
            if (reslt == -1L)
            {
                panic("open_levelfile_exclusively: lseek failed %d", errno);
                return 0;
            }
            lftrack.GnollHack_thinks_it_is_open = TRUE;
        } else {
            really_close();
            fd = sopen(name, oflag, SH_DENYRW, FCMASK);
            lftrack.fd = fd;
            lftrack.oflag = oflag;
            lftrack.GnollHack_thinks_it_is_open = TRUE;
        }
    } else {
        fd = sopen(name, oflag, SH_DENYRW, FCMASK);
        lftrack.fd = fd;
        lftrack.oflag = oflag;
        if (fd >= 0)
            lftrack.GnollHack_thinks_it_is_open = TRUE;
    }
    return fd;
}

void
really_close()
{
    int fd;

    if (lftrack.init) {
        fd = lftrack.fd;

        lftrack.GnollHack_thinks_it_is_open = FALSE;
        lftrack.fd = -1;
        lftrack.oflag = 0;
        if (fd != -1)
            (void) close(fd);
    }
    return;
}

int
nhclose(fd)
int fd;
{
    if (lftrack.fd == fd) {
        really_close(); /* close it, but reopen it to hold it */
        fd = open_levelfile(0, (char *) 0);
        lftrack.GnollHack_thinks_it_is_open = FALSE;
        return 0;
    }
    return close(fd);
}
#else /* !HOLD_LOCKFILE_OPEN */

int
nhclose(fd)
int fd;
{
    return close(fd);
}
#endif /* ?HOLD_LOCKFILE_OPEN */

/* ----------  END LEVEL FILE HANDLING ----------- */

/* ----------  BEGIN BONES FILE HANDLING ----------- */

/* set up "file" to be file name for retrieving bones, and return a
 * bonesid to be read/written in the bones file.
 */
STATIC_OVL char *
set_bonesfile_name(file, lev)
char *file;
d_level *lev;
{
    s_level *sptr;
    char *dptr;

    /*
     * "bonD0.nn"   = bones for level nn in the main dungeon;
     * "bonM0.T"    = bones for Minetown;
     * "bonQBar.n"  = bones for level n in the Barbarian quest;
     * "bon3D0.nn"  = \
     * "bon3M0.T"   =  > same as above, but for bones pool #3.
     * "bon3QBar.n" = /
     *
     * Return value for content validation skips "bon" and the
     * pool number (if present), making it feasible for the admin
     * to manually move a bones file from one pool to another by
     * renaming it.
     */
    Strcpy(file, "bon");
#ifdef SYSCF
    if (sysopt.bones_pools > 1) {
        unsigned poolnum = min((unsigned) sysopt.bones_pools, 10);

        poolnum = (unsigned) ubirthday % poolnum; /* 0..9 */
        Sprintf(eos(file), "%u", poolnum);
    }
#endif
    dptr = eos(file); /* this used to be after the following Sprintf()
                         and the return value was (dptr - 2) */
    /* when this naming scheme was adopted, 'filecode' was one letter;
       3.3.0 turned it into a three letter string (via roles[] in role.c);
       from that version through 3.6.0, 'dptr' pointed past the filecode
       and the return value of (dptr - 2)  was wrong for bones produced
       in the quest branch, skipping the boneid character 'Q' and the
       first letter of the role's filecode; bones loading still worked
       because the bonesid used for validation had the same error */
    Sprintf(dptr, "-%d", (int)(context.game_difficulty - MIN_DIFFICULTY_LEVEL));

    Sprintf(eos(dptr), "%c%s", dungeons[lev->dnum].boneid,
            In_quest(lev) ? urole.filecode : "0");

    if ((sptr = Is_special(lev)) != 0)
        Sprintf(eos(dptr), ".%c", sptr->boneid);
    else
        Sprintf(eos(dptr), ".%d", lev->dlevel);

#ifdef VMS
    Strcat(dptr, ";1");
#endif
    return dptr;
}

/* set up temporary file name for writing bones, to avoid another game's
 * trying to read from an uncompleted bones file.  we want an uncontentious
 * name, so use one in the namespace reserved for this game's level files.
 * (we are not reading or writing level files while writing bones files, so
 * the same array may be used instead of copying.)
 */
STATIC_OVL char *
set_bonestemp_name()
{
    char *tf;

    tf = rindex(lock, '.');
    if (!tf)
        tf = eos(lock);
    Sprintf(tf, ".bn");
#ifdef VMS
    Strcat(tf, ";1");
#endif
    return lock;
}

int
create_bonesfile(lev, bonesid, errbuf)
d_level *lev;
char **bonesid;
char errbuf[];
{
    const char *file;
    int fd;

    if (errbuf)
        *errbuf = '\0';
    *bonesid = set_bonesfile_name(bones, lev);
    file = set_bonestemp_name();
    file = fqname(file, BONESPREFIX, 0);

#if defined(MICRO) || defined(WIN32)
    /* Use O_TRUNC to force the file to be shortened if it already
     * exists and is currently longer.
     */
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, FCMASK);
#else
#ifdef MAC
    fd = maccreat(file, BONE_TYPE);
#else
    fd = creat(file, FCMASK);
#endif
#endif
    if (fd < 0 && errbuf) /* failure explanation */
    {
        Sprintf(errbuf, "Cannot create bones \"%s\", id %s (errno %d).", lock,
            *bonesid, errno);
        maybe_report_file_descriptors(errbuf, errno);
    }
#if defined(VMS) && !defined(SECURE)
    /*
       Re-protect bones file with world:read+write+execute+delete access.
       umask() doesn't seem very reliable; also, vaxcrtl won't let us set
       delete access without write access, which is what's really wanted.
       Can't simply create it with the desired protection because creat
       ANDs the mask with the user's default protection, which usually
       denies some or all access to world.
     */
    (void) chmod(file, FCMASK | 007); /* allow other users full access */
#endif /* VMS && !SECURE */

    return fd;
}

#ifdef MFLOPPY
/* remove partial bonesfile in process of creation */
void
cancel_bonesfile()
{
    const char *tempname;

    tempname = set_bonestemp_name();
    tempname = fqname(tempname, BONESPREFIX, 0);
    (void) unlink(tempname);
}
#endif /* MFLOPPY */

/* move completed bones file to proper name */
void
commit_bonesfile(lev)
d_level *lev;
{
    const char *fq_bones, *tempname;
    int ret;

    (void) set_bonesfile_name(bones, lev);
    fq_bones = fqname(bones, BONESPREFIX, 0);
    tempname = set_bonestemp_name();
    tempname = fqname(tempname, BONESPREFIX, 1);

#if (defined(SYSV) && !defined(SVR4) && !defined(ANDROID)) || defined(GENIX)
    /* old SYSVs don't have rename.  Some SVR3's may, but since they
     * also have link/unlink, it doesn't matter. :-)
     */
    (void) unlink(fq_bones);
    ret = link(tempname, fq_bones);
    ret += unlink(tempname);
#else
    ret = rename(tempname, fq_bones);
#endif
    if (wizard && ret != 0)
        pline("couldn't rename %s to %s.", tempname, fq_bones);
}

int
open_bonesfile(lev, bonesid)
d_level *lev;
char **bonesid;
{
    const char *fq_bones;
    int fd;

    *bonesid = set_bonesfile_name(bones, lev);
    fq_bones = fqname(bones, BONESPREFIX, 0);
    nh_uncompress(fq_bones); /* no effect if nonexistent */
#ifdef MAC
    fd = macopen(fq_bones, O_RDONLY | O_BINARY, BONE_TYPE);
#else
    fd = open(fq_bones, O_RDONLY | O_BINARY, 0);
#endif
    return fd;
}

int
delete_bonesfile(lev)
d_level *lev;
{
    (void) set_bonesfile_name(bones, lev);
    return !(unlink(fqname(bones, BONESPREFIX, 0)) < 0);
}

/* assume we're compressing the recently read or created bonesfile, so the
 * file name is already set properly */
void
compress_bonesfile()
{
    nh_compress(fqname(bones, BONESPREFIX, 0));
}

/* ----------  END BONES FILE HANDLING ----------- */

/* ----------  BEGIN SAVE FILE HANDLING ----------- */

/* set savefile name in OS-dependent manner from pre-existing plname,
 * avoiding troublesome characters */
void
set_savefile_name(regularize_it)
boolean regularize_it;
{
#ifdef VMS
    Sprintf(SAVEF, "[.save]%d%s", getuid(), plname);
    if (regularize_it)
        regularize(SAVEF + 7);
    Strcat(SAVEF, ";1");
#else
#if defined(MICRO)
    Strcpy(SAVEF, SAVEP);
#ifdef AMIGA
    strncat(SAVEF, bbs_id, PATHLEN);
#endif
    {
        int i = strlen(SAVEP);
#ifdef AMIGA
        /* plname has to share space with SAVEP and ".sav" */
        (void) strncat(SAVEF, plname, FILENAME - i - 4);
#else
        (void) strncat(SAVEF, plname, 8);
#endif
        if (regularize_it)
            regularize(SAVEF + i);
    }
    Strcat(SAVEF, SAVE_EXTENSION);
#else
#if defined(WIN32) && !defined(GNH_MOBILE)
    {
        static const char okchars[] =
            "*ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-.";
        const char *legal = okchars;
        char fnamebuf[BUFSZ], encodedfnamebuf[BUFSZ];

        /* Obtain the name of the logged on user and incorporate
         * it into the name. */
        Sprintf(fnamebuf, "%s-%s", get_username(0), plname);
        if (regularize_it)
            ++legal; /* skip '*' wildcard character */
        (void) fname_encode(legal, '%', fnamebuf, encodedfnamebuf, BUFSZ);
        Sprintf(SAVEF, "%s%s", encodedfnamebuf, SAVE_EXTENSION);
    }
#else  /* not VMS or MICRO or WIN32 */

    char dir_separator = '/';
#ifdef GNH_WIN
    dir_separator = '\\';
#endif
    Sprintf(SAVEF, "save%c%d%s", dir_separator, (int)getuid(), plname);
    if (regularize_it)
        regularize(SAVEF + 5); /* avoid . or / in name */
#endif /* WIN32 */
#endif /* MICRO */
#endif /* VMS   */
    if (plname_from_error_savefile)
        set_error_savefile();
    if (plname_from_imported_savefile)
        set_imported_savefile();
}

#ifdef INSURANCE
void
save_savefile_name(fd)
int fd;
{
    (void) write(fd, (genericptr_t) SAVEF, sizeof(SAVEF));
}
#endif

#ifndef MICRO
STATIC_OVL void
print_special_savefile_extension(printbuf, ext)
char* printbuf;
const char* ext;
{
#ifdef VMS
    {
        char* semi_colon = rindex(printbuf, ';');

        if (semi_colon)
            *semi_colon = '\0';
    }
    Sprintf(eos(printbuf), ".%s;1", ext);
#else
#ifdef MAC
    Sprintf(eos(printbuf), "-%s", ext);
#else
    Sprintf(eos(printbuf), ".%s", ext);
#endif
#endif
}

STATIC_OVL void
print_error_savefile_extension(printbuf)
char* printbuf;
{
    print_special_savefile_extension(printbuf, ERROR_EXTENSION);
}

STATIC_OVL void
print_imported_savefile_extension(printbuf)
char* printbuf;
{
    print_special_savefile_extension(printbuf, IMPORTED_EXTENSION);
}

/* change pre-existing savefile name to indicate an error savefile */
void
set_error_savefile(VOID_ARGS)
{
    print_error_savefile_extension(SAVEF);
}

void
set_imported_savefile(VOID_ARGS)
{
    print_imported_savefile_extension(SAVEF);
}
#endif

/* create save file, overwriting one if it already exists */
int
create_savefile()
{
    const char *fq_save;
    int fd;

    fq_save = fqname(SAVEF, SAVEPREFIX, 0);
#if defined(MICRO) || defined(WIN32)
    fd = open(fq_save, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, FCMASK);
#else
#ifdef MAC
    fd = maccreat(fq_save, SAVE_TYPE);
#else
    fd = creat(fq_save, FCMASK);
#endif
#if defined(VMS) && !defined(SECURE)
    /*
       Make sure the save file is owned by the current process.  That's
       the default for non-privileged users, but for priv'd users the
       file will be owned by the directory's owner instead of the user.
    */
#undef getuid
    (void) chown(fq_save, getuid(), getgid());
#define getuid() vms_getuid()
#endif /* VMS && !SECURE */
#endif /* MICRO */
    //issue_debuglog_fd(fd, "create_savefile");

    return fd;
}

/* open savefile for reading */
int
open_savefile()
{
    const char *fq_save;
    int fd;

    fq_save = fqname(SAVEF, SAVEPREFIX, 0);
#ifdef MAC
    fd = macopen(fq_save, O_RDONLY | O_BINARY, SAVE_TYPE);
#else
    fd = open(fq_save, O_RDONLY | O_BINARY, 0);
#endif
    //issue_debuglog_fd(fd, "open_savefile");
    return fd;
}

/* delete savefile */
int
delete_savefile(VOID_ARGS)
{
    (void) unlink(fqname(SAVEF, SAVEPREFIX, 0));
    return 0; /* for open_and_validate_saved_game(TRUE, (boolean*)0) (ex-xxxmain.c) test */
}

int
ask_delete_invalid_savefile(adjective, allow_replace_backup)
const char* adjective;
boolean allow_replace_backup;
{
    struct special_view_info info = { 0 };
    char txtbuf[BUFSZ * 4] = "";
    char titlebuf[BUFSZ * 4] = "";
    int res;
    if (!adjective)
        adjective = "invalid";
    if (allow_replace_backup && check_has_backup_savefile())
    {
        info.viewtype = SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N;
        Sprintf(titlebuf, "Replace %s Save File", str_upper_start(adjective));
        info.title = titlebuf;
        Sprintf(txtbuf, "Save file \"%s\" is %s. Replace it with its backup?", SAVEF, adjective);
        info.text = txtbuf;
        res = open_special_view(info);
        if (res == 'y')
        {
            pline("Replacing %s save file \"%s\".", an(adjective), SAVEF);
            (void)delete_tmp_backup_savefile();
            if (!restore_backup_savefile(TRUE))
                return -2;
            else
                pline("Replacing \"%s\" failed.", SAVEF);
        }
    }
    info.viewtype = SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_Y;
    Sprintf(titlebuf, "Delete %s Save File", str_upper_start(adjective));
    info.title = titlebuf;
    Sprintf(txtbuf, "Save file \"%s\" is %s. Delete it?", SAVEF, adjective);
    info.text = txtbuf;
    res = open_special_view(info);
    if (res == 'y')
    {
        pline("Deleting %s save file \"%s\".", an(adjective), SAVEF);
        (void) delete_tmp_backup_savefile();
        (void) delete_backup_savefile();
        return delete_savefile();
    }
    else
    {
        return -1;
    }
}

int
query_about_corrupted_savefile(VOID_ARGS)
{
    if (check_has_backup_savefile())
    {
        struct special_view_info info = { 0 };
        char txtbuf[BUFSZ * 4] = "";
        int res;
        info.viewtype = SPECIAL_VIEW_GUI_YN_CONFIRMATION_DEFAULT_N;
        info.title = "Corrupted Save File";
        Sprintf(txtbuf, "Save file \"%s\" is corrupted, but you have a backup save file.  Continue loading the corrupted save file anyway?", SAVEF);
        info.text = txtbuf;
        res = open_special_view(info);
        if (res == 'y')
            return 1;
    }
    return 0;
}

STATIC_VAR char fq_tmp_backup[4096];

int
make_tmp_backup_savefile_from_uncompressed_savefile(filename)
const char* filename; /* Filename must have already been uncompressed */
{
    Strcpy(fq_tmp_backup, "");
    if (sysopt.make_backup_savefiles && filename && *filename && !plname_from_imported_savefile) /* Imported save file comes with its own backup */
    {
        if (access(filename, F_OK) != 0) 
        {
            return -3; /* given savefile does not exist, cannot copy it */
        }
        char tobuf[4096];
        size_t len = strlen(filename);
        char extbuf[BUFSZ];
        Strcpy(extbuf, "");
        print_special_savefile_extension(extbuf, BACKUP_EXTENSION);
        size_t bupextlen = strlen(extbuf);
        Strcpy(extbuf, "");
        print_special_savefile_extension(extbuf, TEMP_BACKUP_EXTENSION);
        size_t tmpextlen = strlen(extbuf);
        if (len + bupextlen + tmpextlen + 1 > sizeof(tobuf))
            return -2;
        size_t copy_len = min(sizeof(tobuf) - 1, len);
        Strncpy(tobuf, filename, copy_len);
        tobuf[copy_len] = 0;
        print_special_savefile_extension(tobuf, BACKUP_EXTENSION);
        print_special_savefile_extension(tobuf, TEMP_BACKUP_EXTENSION);
        nh_uncompress(tobuf);
        if (access(tobuf, F_OK) == 0)
        {
            (void)unlink(tobuf);
        }
        int res = copy_savefile(tobuf, filename);
        if (!res)
        {
            nh_compress(tobuf);
            Strcpy(fq_tmp_backup, tobuf);
        }

        return res < 0 ? -4 : res;
    }
    return -1;
}

int
move_tmp_backup_savefile_to_actual_backup_savefile(VOID_ARGS)
{
    /* fq_tmp_backup is compressed */
    if (sysopt.make_backup_savefiles && *fq_tmp_backup)
    {
        size_t len = strlen(fq_tmp_backup);
        char extbuf[BUFSZ];
        Strcpy(extbuf, "");
        print_special_savefile_extension(extbuf, BACKUP_EXTENSION);
        size_t bupextlen = strlen(extbuf);
        Strcpy(extbuf, "");
        print_special_savefile_extension(extbuf, TEMP_BACKUP_EXTENSION);
        size_t tmpextlen = strlen(extbuf);
        if (len <= bupextlen + tmpextlen)
        {
            Strcpy(fq_tmp_backup, "");
            return -2;
        }

        char fq_act_backup[4096];
        Strcpy(fq_act_backup, fq_tmp_backup);
        fq_act_backup[len - tmpextlen] = 0;
        nh_uncompress(fq_act_backup);
        if (access(fq_act_backup, F_OK) == 0) 
        {
            (void)unlink(fq_act_backup);
        }
        nh_uncompress(fq_tmp_backup); /* Uncompress at original location */
        int res = rename(fq_tmp_backup, fq_act_backup);
        if (!res)
        {
            nh_compress(fq_act_backup); /* Finally compress at new location */
            Strcpy(fq_tmp_backup, "");
        }
        return res;
    }
    return -1;
}

int
restore_backup_savefile(dodelete_existing)
boolean dodelete_existing;
{
    if (sysopt.make_backup_savefiles && *SAVEF)
    {
        char bakbuf[4096];
        const char* fq_save = fqname(SAVEF, SAVEPREFIX, 0);
        Strcpy(bakbuf, fq_save);
        print_special_savefile_extension(bakbuf, BACKUP_EXTENSION);
        nh_uncompress(bakbuf);
        if (access(bakbuf, F_OK) != 0)
            return -2; /* Backup does not exist */

        nh_uncompress(fq_save);
        if (access(fq_save, F_OK) == 0)
        {
            if (dodelete_existing)
                (void)unlink(fq_save);
            else
            {
                nh_compress(fq_save);
                return -3; /* Save file exists and dodelete_existing is off, aborting restoring backup */
            }
        }
        int res = copy_savefile(fq_save, bakbuf);
        nh_compress(bakbuf);
        if (!res)
            nh_compress(fq_save);

        return res < 0 ? -4 : res;
    }
    return -1; /* Making backups is not on */
}

int
delete_backup_savefile(VOID_ARGS)
{
    if (sysopt.make_backup_savefiles && *SAVEF)
    {
        char bakbuf[4096];
        Strcpy(bakbuf, fqname(SAVEF, SAVEPREFIX, 0));
        print_special_savefile_extension(bakbuf, BACKUP_EXTENSION);
        nh_uncompress(bakbuf);
        if (access(bakbuf, F_OK) != 0)
            return -2; /* Backup file does not exist */
        return unlink(bakbuf);
    }
    return -1; /* Making backups is not on */
}

int
delete_tmp_backup_savefile(VOID_ARGS)
{
    if (sysopt.make_backup_savefiles && *SAVEF)
    {
        char bakbuf[4096];
        Strcpy(bakbuf, fqname(SAVEF, SAVEPREFIX, 0));
        print_special_savefile_extension(bakbuf, BACKUP_EXTENSION);
        print_special_savefile_extension(bakbuf, TEMP_BACKUP_EXTENSION);
        nh_uncompress(bakbuf);
        if (access(bakbuf, F_OK) != 0)
            return -2; /* Backup temp file does not exist */
        return unlink(bakbuf);
    }
    return -1; /* Making backups is not on */
}

int
delete_error_savefile(VOID_ARGS)
{
    if (*SAVEF)
    {
        char bakbuf[4096];
        Strcpy(bakbuf, fqname(SAVEF, SAVEPREFIX, 0));
        print_special_savefile_extension(bakbuf, ERROR_EXTENSION);
        nh_uncompress(bakbuf);
        if (access(bakbuf, F_OK) != 0)
            return -2; /* Error file does not exist */
        return unlink(bakbuf);
    }
    return -1;
}

int
delete_running_files(VOID_ARGS)
{
    int lev, fd;
    char errbuf[BUFSZ];
#if defined(UNIX) || defined(GNH_MOBILE)
    Sprintf(lock, "%d%s", (int)getuid(), plname);
#elif defined(WIN32)
    char fnamebuf[BUFSZ], encodedfnamebuf[BUFSZ];
    Sprintf(fnamebuf, "%s-%s", get_username(0), plname);
    (void)fname_encode(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-.", '%',
        fnamebuf, encodedfnamebuf, BUFSZ);
    Sprintf(lock, "%s", encodedfnamebuf);
#else
    Sprintf(lock, "%s", plname);
#endif
    for (lev = 0; lev < 256; lev++)
    {
        fd = open_levelfile(lev, errbuf);
        if (fd >= 0) /* exists */
        {
            (void)nhclose(fd);
            const char* fq_lock;
            set_levelfile_name(lock, lev);
            fq_lock = fqname(lock, LEVELPREFIX, 3);
            (void)unlink(fq_lock);
        }
    }
    return 0;
}

boolean check_has_backup_savefile(VOID_ARGS)
{
    if (sysopt.make_backup_savefiles && *SAVEF)
    {
        char bakbuf[4096];
        Strcpy(bakbuf, fqname(SAVEF, SAVEPREFIX, 0));
        print_special_savefile_extension(bakbuf, BACKUP_EXTENSION);
        nh_uncompress(bakbuf);
        int accres = access(bakbuf, F_OK);
        int valres = -2;
        if (!accres)
        {
            int fd;
#ifdef MAC
            fd = macopen(bakbuf, O_RDONLY | O_BINARY, SAVE_TYPE);
#else
            fd = open(bakbuf, O_RDONLY | O_BINARY, 0);
#endif
            if (fd >= 0)
            {
                valres = validate(fd, (char*)0);
                (void)nhclose(fd), fd = -1;
            }
            nh_compress(bakbuf);
        }
        return (boolean)(!accres && !valres);
    }
    return FALSE;
}

int
delete_savefile_if_exists(VOID_ARGS)
{
    if (*SAVEF)
    {
        const char* fq_save = fqname(SAVEF, SAVEPREFIX, 0);
        nh_uncompress(fq_save);
        if (access(fq_save, F_OK) == 0)
            return delete_savefile();
        return -2; /* fq_save does not exist */
    }
    return -1; /* SAVEF is empty */
}

/* returns 1 if save file exists, otherwise 0 */
boolean
check_existing_save_file()
{
    const char* fq_save;
    register int fd;
    fq_save = fqname(SAVEF, SAVEPREFIX, 1); /* level files take 0 */

#if defined(MICRO) && defined(MFLOPPY)
    if (!saveDiskPrompt(0))
        return 0;
#endif

    nh_uncompress(fq_save);
    fd = open_savefile();
    if (fd >= 0) {
        (void)nhclose(fd);
        /* There is an old save file, let's compress it back */
        nh_compress(fq_save);
        return 1;
    }
    return 0;
}

/* returns 1 if save file exists, otherwise 0 */
boolean
check_existing_error_save_file()
{
    const char* fq_save;
    char fq_error[4096];
    fq_save = fqname(SAVEF, SAVEPREFIX, 1); /* level files take 0 */
    Strcpy(fq_error, fq_save);
    print_error_savefile_extension(fq_error);

#if defined(MICRO) && defined(MFLOPPY)
    if (!saveDiskPrompt(0))
        return 0;
#endif

    nh_uncompress(fq_error);
    if (access(fq_error, F_OK) == 0)
    {
        nh_compress(fq_error);
        return 1;
    }
    else
        return 0;
}

/* try to open up a save file and prepare to restore it */
int
open_and_validate_saved_game(allow_replace_backup, is_backup_ptr)
boolean allow_replace_backup;
boolean* is_backup_ptr;
{
    const char *fq_save;
    int fd;
    boolean backup_replaced = FALSE;
    if (is_backup_ptr)
        *is_backup_ptr = FALSE;

    reset_restpref();
    set_savefile_name(TRUE);
#ifdef MFLOPPY
    if (!saveDiskPrompt(1))
        return -1;
#endif /* MFLOPPY */
    fq_save = fqname(SAVEF, SAVEPREFIX, 0);

#ifdef COMPRESS
    /* Handle corrupted compressed file */
    if (allow_replace_backup)
    {
        /* Uncompress might fail due to corruption; if so, restore backup file */
        if (!restore_backup_savefile(FALSE))
        {
            allow_replace_backup = FALSE;
            backup_replaced = TRUE;
        }
    }
#endif
    nh_uncompress(fq_save);

    /* Handle error and backup save files in the case of a missing fq_save (which normally does not happen if you select your character from the load saved game menu) */
    char fbuf[4096];
    if (access(fq_save, F_OK) != 0) /* cannot access */
    {
        boolean filerenamed = FALSE;
        if (sysopt.allow_loading_error_savefiles)
        {
            Strcpy(fbuf, fq_save);
            print_error_savefile_extension(fbuf);
            nh_uncompress(fbuf);
            if (access(fbuf, F_OK) == 0)
            {
                (void)rename(fbuf, fq_save);
                filerenamed = TRUE;
            }
        }
        
        if (!filerenamed && allow_replace_backup && !backup_replaced)
        {
            Strcpy(fbuf, fq_save);
            print_special_savefile_extension(fbuf, BACKUP_EXTENSION);
            nh_uncompress(fbuf);
            if (access(fbuf, F_OK) == 0)
            {
                (void)rename(fbuf, fq_save);
                if (is_backup_ptr)
                    *is_backup_ptr = TRUE;
            }
        }
    }

    (void) make_tmp_backup_savefile_from_uncompressed_savefile(fq_save);
    if ((fd = open_savefile()) < 0)
        return fd;

    if ((validate(fd, fq_save)) != 0) {
        (void) nhclose(fd), fd = -1;
        (void) delete_tmp_backup_savefile();
        if (ask_delete_invalid_savefile("invalid", allow_replace_backup) == -2)
        {
            backup_replaced = TRUE;
            fd = open_and_validate_saved_game(FALSE, (boolean*)0);
        }
    }
    if (is_backup_ptr)
        *is_backup_ptr = backup_replaced;
    return fd;
}

void 
mode_message(VOID_ARGS)
{
    if (discover || CasualMode)
        You_ex(ATR_NONE, CLR_MSG_HINT, "are in %s mode.", get_game_mode_text(TRUE));
}

void
create_gamestate_levelfile(VOID_ARGS)
{
    /* Set up level 0 file to keep the game state.
     */
    int fd = create_levelfile(0, (char*)0);
    if (fd < 0)
    {
        raw_print("Cannot create game state level 0 file");
    }
    else
    {
        hackpid = 1;
        (void)write(fd, (genericptr_t)&hackpid, sizeof(hackpid));
        close(fd);
    }
}

int
load_saved_game(load_type)
int load_type; // 0 = at start normally, 1 = load after saving, corresponds to exit_hack_code at start
{
    reseting = TRUE;
    boolean is_backup = FALSE;
    int fd = open_and_validate_saved_game(TRUE, &is_backup);
    if (fd >= 0)
    {
        /* Since wizard is actually flags.debug, restoring might
         * overwrite it.
         */
        boolean remember_wiz_mode = wizard;
        const char* fq_save = fqname(SAVEF, SAVEPREFIX, 1);

        if (load_type == 0)
        {
#ifdef NEWS
            if (iflags.news)
            {
                display_file(NEWS, FALSE);
                iflags.news = FALSE; /* in case dorestore() fails */
            }
#endif
            pline("Restoring save file...");
            mark_synch(); /* flush output */
        }
        int loadres = dorestore0(fd);
        if (!loadres && !is_backup) //This deletes the save file in normal modes
        {
            if (!restore_backup_savefile(TRUE))
            {
                pline("Restoring save file failed.  Replaced save file with back-up save file.");
                mark_synch(); /* flush output */
                fd = open_and_validate_saved_game(FALSE, (boolean*)0);
                if (fd >= 0)
                {
                    if (load_type == 0)
                    {
                        pline("Restoring back-up save file...");
                        mark_synch(); /* flush output */
                    }
                    loadres = dorestore0(fd);
                }
            }
        }
        if (!loadres)
        {
            reseting = FALSE;
            return 0;
        }
        if (!wizard && remember_wiz_mode)
            wizard = TRUE;

        reseting = FALSE;
        
        /* Success! */
        encounter_init();
        delete_excess_levelfiles();

        switch (load_type)
        {
        case 0:
            /* Welcome */
            issue_simple_gui_command(GUI_CMD_FADE_FROM_BLACK_SLOWLY_NONBLOCKING);
            welcome(FALSE);
            check_special_room(FALSE);
            mode_message();
            break;
        case 1:
            flush_screen(1);
            update_game_music();
            play_level_ambient_sounds();
            play_environment_ambient_sounds();
            break;
        }

        boolean savefilekept = FALSE;
        if (discover || wizard || CasualMode)
        {
            //Note that you can be in both Casual and wizard mode
            if (CasualMode)
            {
                if(load_type == 0)
                    pline("Keeping the save file.");

                savefilekept = TRUE;
                nh_compress(fq_save);
            }
            else
            {
                if (load_type == 0 && yn_query("Do you want to keep the save file?") == 'n')
                {
                    (void)delete_savefile();
                }
                else
                {
                    savefilekept = TRUE;
                    nh_compress(fq_save);
                }
            }
        }

        reset_save_file_name(savefilekept);
        return 1;
    }
    reseting = FALSE;
    return 0;
}

void
reset_save_file_name(savefilekept)
boolean savefilekept;
{
    if (plname_from_error_savefile || plname_from_imported_savefile)
    {
        boolean was_from_imported_savefile = plname_from_imported_savefile;
        if (plname_from_imported_savefile)
        {
            flags.non_scoring = TRUE;
        }
        char oldsavef[SAVESIZE];
        Strcpy(oldsavef, SAVEF);
        plname_from_error_savefile = FALSE;
        plname_from_imported_savefile = FALSE;
        set_savefile_name(TRUE);
        if (savefilekept && strcmp(SAVEF, oldsavef))
        {
            const char* fq_save_old = fqname(oldsavef, SAVEPREFIX, 0);
            const char* fq_save = fqname(SAVEF, SAVEPREFIX, 0);
            nh_uncompress(fq_save_old);
            nh_uncompress(fq_save); //If it happens to exist
            (void)remove(fq_save); //If it happens to exist
            (void)rename(fq_save_old, fq_save);
            nh_compress(fq_save);
        }
        if (was_from_imported_savefile)
        {
            /* If an imported backup savefile exists, rename it too */
            char backupfilename[BUFSZ];
            Strcpy(backupfilename, SAVEF);
            print_special_savefile_extension(backupfilename, BACKUP_EXTENSION);
            print_special_savefile_extension(backupfilename, IMPORTED_EXTENSION);
            const char* fq_save_backup = fqname(backupfilename, SAVEPREFIX, 0);
            nh_uncompress(fq_save_backup);
            if (access(fq_save_backup, F_OK) == 0)
            {
                char nonimportedbackupfilename[BUFSZ];
                Strcpy(nonimportedbackupfilename, SAVEF);
                print_special_savefile_extension(nonimportedbackupfilename, BACKUP_EXTENSION);
                const char* fq_save_nonimportedbackup = fqname(nonimportedbackupfilename, SAVEPREFIX, 0);
                nh_uncompress(fq_save_nonimportedbackup); //If it happens to exist
                (void)remove(fq_save_nonimportedbackup); //If it happens to exist
                (void)rename(fq_save_backup, fq_save_nonimportedbackup);
                nh_compress(fq_save_nonimportedbackup);
            }
        }
    }
}


#if defined(SELECTSAVED)
struct save_game_data 
newsavegamedata(playername, filename, gamestats, is_running, is_error_savefile, is_imported_savefile)
char* playername;
char* filename;
struct save_game_stats gamestats;
boolean is_running, is_error_savefile, is_imported_savefile;
{
    struct save_game_data svgd = { 0 };
    svgd.playername = playername;
    Strcpy(svgd.filename, filename ? filename : "");
    svgd.is_running = is_running;
    svgd.is_error_save_file = is_error_savefile;
    svgd.is_imported_save_file = is_imported_savefile;
    svgd.gamestats = gamestats;
    return svgd;
}

char *
plname_from_file(filename, stats_ptr)
const char *filename;
struct save_game_stats* stats_ptr;
{
    int fd;
    char *result = 0;
    boolean dodeletefile = FALSE;

    Strcpy(SAVEF, filename);
#ifdef COMPRESS_EXTENSION
    SAVEF[strlen(SAVEF) - strlen(COMPRESS_EXTENSION)] = '\0';
#endif
    nh_uncompress(SAVEF);
    if ((fd = open_savefile()) >= 0) {
        if (validate(fd, filename) == 0) {
            char tplname[PL_NSIZ];
            get_plname_from_file(fd, tplname);
            get_save_game_stats_from_file(fd, stats_ptr);
            result = dupstr(tplname);
        }
        else
        {
            dodeletefile = TRUE;
        }
        (void) nhclose(fd);
    }
    nh_compress(SAVEF);

    //Delete mismatching save files, so they do not hang around for nothing
    if (dodeletefile)
    {
        (void)delete_tmp_backup_savefile();
        (void)ask_delete_invalid_savefile("invalid", TRUE);
    }
    return result;
#if 0
/* --------- obsolete - used to be ifndef STORE_PLNAME_IN_FILE ----*/
#if defined(UNIX) && defined(QT_GRAPHICS)
    /* Name not stored in save file, so we have to extract it from
       the filename, which loses information
       (eg. "/", "_", and "." characters are lost. */
    int k;
    int uid;
    char name[64]; /* more than PL_NSIZ */
#ifdef COMPRESS_EXTENSION
#define EXTSTR COMPRESS_EXTENSION
#else
#define EXTSTR ""
#endif

    if ( sscanf( filename, "%*[^/]/%d%63[^.]" EXTSTR, &uid, name ) == 2 ) {
#undef EXTSTR
        /* "_" most likely means " ", which certainly looks nicer */
        for (k=0; name[k]; k++)
            if ( name[k] == '_' )
                name[k] = ' ';
        return dupstr(name);
    } else
#endif /* UNIX && QT_GRAPHICS */
    {
        return 0;
    }
/* --------- end of obsolete code ----*/
#endif /* 0 - WAS STORE_PLNAME_IN_FILE*/
}

char*
plname_from_running(filename, stats_ptr)
const char* filename;
struct save_game_stats* stats_ptr;
{
    int fd;
    char* result = 0;
    int savelev, hpid, pltmpsiz;
    struct version_info version_data;
    char savename[SAVESIZE];
    struct savefile_info sfi;
    char tmpplbuf[PL_NSIZ];

    /* level 0 file contains:
     *  pid of creating process (ignored here)
     *  level number for current level of save file
     *  name of save file nethack would have created
     *  savefile info
     *  player name
     *  and game state
     */
    if ((fd = open(filename, O_RDONLY | O_BINARY, 0)) >= 0) {
        if (read(fd, (genericptr_t)&hpid, sizeof hpid) == sizeof hpid
            && read(fd, (genericptr_t)&savelev, sizeof(savelev)) == sizeof savelev
            && read(fd, (genericptr_t)savename, sizeof savename) == sizeof savename
            && read(fd, (genericptr_t)&version_data, sizeof version_data) == sizeof version_data
            && read(fd, (genericptr_t)&sfi, sizeof sfi) == sizeof sfi
            && read(fd, (genericptr_t)&pltmpsiz, sizeof pltmpsiz) == sizeof pltmpsiz
            && pltmpsiz > 0 && pltmpsiz <= PL_NSIZ
            && read(fd, (genericptr_t)&tmpplbuf, pltmpsiz) == pltmpsiz
            && read(fd, (genericptr_t)stats_ptr, sizeof * stats_ptr) == sizeof * stats_ptr
            ) {
            result = dupstr(tmpplbuf);
        }
        close(fd);
    }

    return result;
}

#if defined(UNIX) && (defined(ANDROID) || defined(GNH_MOBILE))
int is_error_savefile_name(savefilename)
const char* savefilename;
{
    if (!savefilename || !*savefilename)
        return FALSE;

    size_t dlen = strlen(savefilename);
    char ebuf[BUFSZ] = "";
    print_error_savefile_extension(ebuf);
    size_t elen = strlen(ebuf);
    if (dlen <= elen)
        return FALSE;

    size_t i;
    for (i = 0; i < elen; i++)
        if (savefilename[dlen - 1 - i] != ebuf[elen - 1 - i])
            return FALSE;

    return TRUE;
}

int is_imported_savefile_name(savefilename)
const char* savefilename;
{
    if (!savefilename || !*savefilename)
        return FALSE;

    size_t dlen = strlen(savefilename);
    char ebuf[BUFSZ] = "";
    print_imported_savefile_extension(ebuf);
    size_t elen = strlen(ebuf);
    if (dlen <= elen)
        return FALSE;

    size_t i;
    for (i = 0; i < elen; i++)
        if (savefilename[dlen - 1 - i] != ebuf[elen - 1 - i])
            return FALSE;

    return TRUE;
}

int filter_running(entry)
const struct dirent* entry;
{
    return *entry->d_name && entry->d_name[strlen(entry->d_name) - 1] == '0';
}

int filter_error(entry)
const struct dirent* entry;
{
    return is_error_savefile_name(entry->d_name);
}

int filter_imported(entry)
const struct dirent* entry;
{
    return is_imported_savefile_name(entry->d_name);
}

int filter_imported_error(entry)
const struct dirent* entry;
{
    return is_imported_error_savefile_name(entry->d_name);
}

int filter_backup(entry)
const struct dirent* entry;
{
    return is_backup_savefile_name(entry->d_name);
}

int filter_imported_backup(entry)
const struct dirent* entry;
{
    return is_imported_backup_savefile_name(entry->d_name);
}

#endif
#endif /* defined(SELECTSAVED) */

struct save_game_data *
get_saved_games()
{
#if defined(SELECTSAVED)
    int j = 0;
    struct save_game_stats gamestats = { 0 };
    struct save_game_data* result = 0;
#ifdef WIN32
    {
        char *foundfile;
        char  usedfoundfile[4096];
        char  foundfileprefix[32];

        const char *fq_save;
        char fq_save_ebuf[BUFSZ];
        char fq_save_ibuf[BUFSZ];
        char fq_lock_rbuf[BUFSZ];
        char saved_plname[PL_NSIZ];

        Strcpy(saved_plname, plname);
        Strcpy(plname, "*");
        set_savefile_name(FALSE);
#ifdef GNH_WIN
        Sprintf(fq_lock_rbuf, "%d*.0", getuid());
        Strcpy(foundfileprefix, "save\\");
#else
        Strcpy(fq_lock_rbuf, "*-*.0");
        Strcpy(foundfileprefix, "");
#endif
#if defined(ZLIB_COMP)
        Strcat(SAVEF, COMPRESS_EXTENSION);
#endif
        fq_save = fqname(SAVEF, SAVEPREFIX, 0);
        Strcpy(fq_save_ebuf, fq_save);
        Strcpy(fq_save_ibuf, fq_save);
#ifndef MICRO
        print_error_savefile_extension(fq_save_ebuf);
        print_imported_savefile_extension(fq_save_ibuf);
#endif

        int n = 0;
        int n2 = 0;
        int n3 = 0;
        int n4 = 0;
        foundfile = foundfile_buffer();
        if (findfirst(fq_save)) {
            do {
                ++n;
            } while (findnext());
        }
#ifndef MICRO
        if (findfirst(fq_save_ebuf)) {
            do {
                ++n2;
            } while (findnext());
        }
        if (!TournamentMode)
        {
            if (findfirst(fq_save_ibuf)) {
                do {
                    ++n3;
                } while (findnext());
            }
        }
        if (findfirst(fq_lock_rbuf)) {
            do {
                ++n4;
            } while (findnext());
        }
#endif
        if (n > 0 || n2 > 0 || n3 > 0 || n4 > 0) 
        {
            result = (struct save_game_data*) alloc(((size_t)n + (size_t)n2 + (size_t)n3 + (size_t)n4 + 1) * sizeof(struct save_game_data)); /* at most */
            (void) memset((genericptr_t) result, 0, ((size_t)n + (size_t)n2 + (size_t)n3 + (size_t)n4 + 1) * sizeof(struct save_game_data));
            if (n > 0)
            {
                if (findfirst(fq_save)) {
                    j = n = 0;
                    do {
                        Sprintf(usedfoundfile, "%s%s", foundfileprefix, foundfile);
                        if (is_backup_savefile_name(usedfoundfile))
                            continue;
                        if (is_imported_backup_savefile_name(usedfoundfile))
                            continue;
                        if (is_save_file_tracking_file_name(usedfoundfile))
                            continue;
                        char* r;
                        r = plname_from_file(usedfoundfile, &gamestats);
                        if (r)
                        {
                            if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                                continue;
                            if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                                continue;
                            result[j++] = newsavegamedata(r, usedfoundfile, gamestats, FALSE, FALSE, FALSE);
                        }
                        ++n;
                    } while (findnext());
                }
            }
            if (n2 > 0)
            {
                if (findfirst(fq_save_ebuf)) {
                    n2 = 0;
                    do {
                        Sprintf(usedfoundfile, "%s%s", foundfileprefix, foundfile);
                        if (is_imported_backup_savefile_name(usedfoundfile))
                            continue;
                        char* r;
                        r = plname_from_file(usedfoundfile, &gamestats);
                        if (r)
                        {
                            if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                                continue;
                            if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                                continue;
                            result[j++] = newsavegamedata(r, usedfoundfile, gamestats, FALSE, TRUE, FALSE);
                        }
                        ++n2;
                    } while (findnext());
                }
            }
            if (n3 > 0)
            {
                if (findfirst(fq_save_ibuf)) {
                    n3 = 0;
                    do {
                        char* r;
                        Sprintf(usedfoundfile, "%s%s", foundfileprefix, foundfile);
                        r = plname_from_file(usedfoundfile, &gamestats);
                        if (r)
                        {
                            if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                                continue;
                            if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                                continue;
                            boolean isimportederror = is_imported_error_savefile_name(usedfoundfile);
                            result[j++] = newsavegamedata(r, usedfoundfile, gamestats, FALSE, isimportederror, TRUE);
                        }
                        ++n3;
                    } while (findnext());
                }
            }
            if (n4 > 0)
            {
                if (findfirst(fq_lock_rbuf)) {
                    n4 = 0;
                    do {
                        char* r;
                        Sprintf(usedfoundfile, "%s", foundfile); //"%s%s", foundfileprefix, foundfile);
                        r = plname_from_running(usedfoundfile, &gamestats);
                        if (r)
                        {
                            if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                                continue;
                            if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                                continue;
                            result[j++] = newsavegamedata(r, usedfoundfile, gamestats, TRUE, FALSE, FALSE);
                        }
                        ++n4;
                    } while (findnext());
                }
            }
        }
        Strcpy(plname, saved_plname);
    }
#elif defined(UNIX) && defined(QT_GRAPHICS)
    /* posixly correct version */
    int myuid = getuid();
    int n;
    DIR *dir;

    if ((dir = opendir(fqname("save", SAVEPREFIX, 0)))) {
        for (n = 0; readdir(dir); n++)
            ;
        closedir(dir);
        if (n > 0) {
            int i;

            if (!(dir = opendir(fqname("save", SAVEPREFIX, 0))))
                return 0;
            result = (struct save_game_data*) alloc((n + 1) * sizeof(struct save_game_data)); /* at most */
            (void) memset((genericptr_t) result, 0, (n + 1) * sizeof(struct save_game_data));
            for (i = 0, j = 0; i < n; i++) {
                int uid;
                char name[64]; /* more than PL_NSIZ */
                struct dirent *entry = readdir(dir);

                if (!entry)
                    break;
                if (sscanf(entry->d_name, "%d%63s", &uid, name) == 2) {
                    if (uid == myuid) {
                        boolean isbackupfile = !!filter_backup(namelist[i]);
                        if (isbackupfile)
                            continue;
                        char filename[BUFSZ];
                        char *r;
                        Sprintf(filename, "save/%d%s", uid, name);
                        r = plname_from_file(filename, &gamestats);
                        if (r)
                        {
                            if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                                continue;
                            if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                                continue;
                            result[j++] = newsavegamedata(r, filename, gamestats, FALSE, FALSE, FALSE);
                        }
                    }
                }
            }
            closedir(dir);
        }
    }
#elif defined(UNIX) && (defined(ANDROID) || defined(GNH_MOBILE))
    int myuid = getuid();
    struct dirent** namelist;
    struct dirent** namelist2;
    int n1 = scandir("save", &namelist, 0, 0);
    int n2 = scandir(".", &namelist2, filter_running, 0);
    if (n1 < 0) n1 = 0;
    if (n2 < 0) n2 = 0;

    int i, uid;
    char name[64]; /* more than PL_NSIZ */
    if (n1 > 0 || n2 > 0) {
        result = (struct save_game_data*)alloc((n1 + n2 + 1) * sizeof(struct save_game_data)); /* at most */
        (void)memset((genericptr_t)result, 0, (n1 + n2 + 1) * sizeof(struct save_game_data));
    }
    for (i = 0; i < n1; i++) {
        if (sscanf(namelist[i]->d_name, "%d%63s", &uid, name) == 2) {
            if (uid == myuid) {
                boolean isbackupfile = !!filter_backup(namelist[i]);
                boolean isimportedbackupfile = !!filter_imported_backup(namelist[i]);
                boolean isimportedfile = !!filter_imported(namelist[i]);
                if (isbackupfile || isimportedbackupfile || (TournamentMode && isimportedfile))
                    continue;
                char filename[BUFSZ];
                char* r;
                Sprintf(filename, "save/%d%s", uid, name);
                r = plname_from_file(filename, &gamestats);
                if (r)
                {
                    if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                        continue;
                    if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                        continue;
                    boolean iserrorfile = filter_error(namelist[i]) || filter_imported_error(namelist[i]);
                    result[j++] = newsavegamedata(r, filename, gamestats, FALSE, iserrorfile, isimportedfile);
                }
            }
        }
    }
    for (i = 0; i < n2; i++) {
        if (sscanf(namelist2[i]->d_name, "%d%63[^.].0", &uid, name) == 2) {
            if (uid == myuid) {
                char* r;
                r = plname_from_running(namelist2[i]->d_name, &gamestats);
                if (r)
                {
                    if (TournamentMode && !(gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE))
                        continue;
                    if (!TournamentMode && (gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0)
                        continue;
                    result[j++] = newsavegamedata(r, namelist2[i]->d_name, gamestats, TRUE, FALSE, FALSE);
                }
            }
        }
    }
#endif
#ifdef VMS
    Strcpy(plname, "*");
    set_savefile_name(FALSE);
    j = vms_get_saved_games(SAVEF, &result);
#endif /* VMS */

    if (j > 0) {
        if (j > 1)
            qsort(result, j, sizeof (struct save_game_data), savegamedata_strcmp_wrap);
        result[j].playername = 0;
        return result;
    } else if (result) { /* could happen if save files are obsolete */
        free_saved_games(result);
    }
#endif /* SELECTSAVED */
    return 0;
}

void
free_saved_games(saved)
struct save_game_data *saved;
{
    if (saved) 
    {
        int i = 0;

        while (saved[i].playername)
            free((genericptr_t) saved[i++].playername);
        free((genericptr_t) saved);
    }
}

/* ----------  END SAVE FILE HANDLING ----------- */

/* ----------  BEGIN FILE COMPRESSION HANDLING ----------- */

#ifdef COMPRESS

STATIC_OVL void
redirect(filename, mode, stream, uncomp)
const char *filename, *mode;
FILE *stream;
boolean uncomp;
{
    if (freopen(filename, mode, stream) == (FILE *) 0) {
        (void) fprintf(stderr, "freopen of %s for %scompress failed\n",
                       filename, uncomp ? "un" : "");
        nh_terminate(EXIT_FAILURE);
    }
}

/*
 * using system() is simpler, but opens up security holes and causes
 * problems on at least Interactive UNIX 3.0.1 (SVR3.2), where any
 * setuid is renounced by /bin/sh, so the files cannot be accessed.
 *
 * cf. child() in unixunix.c.
 */
STATIC_OVL void
docompress_file(filename, uncomp)
const char *filename;
boolean uncomp;
{
#define MAX_FILE_NAME_BUFFER_SIZE 512
    char cfn[MAX_FILE_NAME_BUFFER_SIZE];
    FILE *cf;
    const char *args[10];
#ifdef COMPRESS_OPTIONS
    char opts[80];
#endif
    int i = 0;
    int f;
#ifdef TTY_GRAPHICS
    boolean istty = WINDOWPORT("tty");
#endif

    Strncpy(cfn, filename, MAX_FILE_NAME_BUFFER_SIZE - 5);
    cfn[MAX_FILE_NAME_BUFFER_SIZE - 5] = 0;
#ifdef COMPRESS_EXTENSION
    Strcat(cfn, COMPRESS_EXTENSION);
#endif
    /* when compressing, we know the file exists */
    if (uncomp) {
        if ((cf = fopen(cfn, RDBMODE)) == (FILE *) 0)
            return;
        (void) fclose(cf);
    }

    args[0] = COMPRESS;
    if (uncomp)
        args[++i] = "-d"; /* uncompress */
#ifdef COMPRESS_OPTIONS
    {
        /* we can't guarantee there's only one additional option, sigh */
        char *opt;
        boolean inword = FALSE;

        Strcpy(opts, COMPRESS_OPTIONS);
        opt = opts;
        while (*opt) {
            if ((*opt == ' ') || (*opt == '\t')) {
                if (inword) {
                    *opt = '\0';
                    inword = FALSE;
                }
            } else if (!inword) {
                args[++i] = opt;
                inword = TRUE;
            }
            opt++;
        }
    }
#endif
    args[++i] = (char *) 0;

#ifdef TTY_GRAPHICS
    /* If we don't do this and we are right after a y/n question *and*
     * there is an error message from the compression, the 'y' or 'n' can
     * end up being displayed after the error message.
     */
    if (istty)
        mark_synch();
#endif
    f = fork();
    if (f == 0) { /* child */
#ifdef TTY_GRAPHICS
        /* any error messages from the compression must come out after
         * the first line, because the more() to let the user read
         * them will have to clear the first line.  This should be
         * invisible if there are no error messages.
         */
        if (istty)
            raw_print("");
#endif
        /* run compressor without privileges, in case other programs
         * have surprises along the line of gzip once taking filenames
         * in GZIP.
         */
        /* assume all compressors will compress stdin to stdout
         * without explicit filenames.  this is true of at least
         * compress and gzip, those mentioned in config.h.
         */
        if (uncomp) {
            redirect(cfn, RDBMODE, stdin, uncomp);
            redirect(filename, WRBMODE, stdout, uncomp);
        } else {
            redirect(filename, RDBMODE, stdin, uncomp);
            redirect(cfn, WRBMODE, stdout, uncomp);
        }
        (void) setgid(getgid());
        (void) setuid(getuid());
        (void) execv(args[0], (char *const *) args);
        perror((char *) 0);
        (void) fprintf(stderr, "Exec to %scompress %s failed.\n",
                       uncomp ? "un" : "", filename);
        nh_terminate(EXIT_FAILURE);
    } else if (f == -1) {
        perror((char *) 0);
        pline("Fork to %scompress %s failed.", uncomp ? "un" : "", filename);
        return;
    }
#ifndef NO_SIGNAL
    (void) signal(SIGINT, SIG_IGN);
    (void) signal(SIGQUIT, SIG_IGN);
    (void) wait((int *) &i);
    (void) signal(SIGINT, (SIG_RET_TYPE) done1);
    if (wizard)
        (void) signal(SIGQUIT, SIG_DFL);
#else
    /* I don't think we can really cope with external compression
     * without signals, so we'll declare that compress failed and
     * go on.  (We could do a better job by forcing off external
     * compression if there are no signals, but we want this for
     * testing with FailSafeC
     */
    i = 1;
#endif
    if (i == 0) {
        /* (un)compress succeeded: remove file left behind */
        if (uncomp)
            (void) unlink(cfn);
        else
            (void) unlink(filename);
    } else {
        /* (un)compress failed; remove the new, bad file */
        if (uncomp) {
            raw_printf("Unable to uncompress %s", filename);
            (void) unlink(filename);
        } else {
            /* no message needed for compress case; life will go on */
            (void) unlink(cfn);
        }
#ifdef TTY_GRAPHICS
        /* Give them a chance to read any error messages from the
         * compression--these would go to stdout or stderr and would get
         * overwritten only in tty mode.  It's still ugly, since the
         * messages are being written on top of the screen, but at least
         * the user can read them.
         */
        if (istty && iflags.window_inited) {
            clear_nhwindow(WIN_MESSAGE);
            more();
            /* No way to know if this is feasible */
            /* doredraw(); */
        }
#endif
    }
}
#endif /* COMPRESS */

#if defined(COMPRESS) || defined(ZLIB_COMP)
#define UNUSED_if_not_COMPRESS /*empty*/
#else
#define UNUSED_if_not_COMPRESS UNUSED
#endif

/* compress file */
void
nh_compress(filename)
const char *filename UNUSED_if_not_COMPRESS;
{
#if !defined(COMPRESS) && !defined(ZLIB_COMP)
#ifdef PRAGMA_UNUSED
#pragma unused(filename)
#endif
#else
    docompress_file(filename, FALSE);
#endif
}

/* uncompress file if it exists */
void
nh_uncompress(filename)
const char *filename UNUSED_if_not_COMPRESS;
{
#if !defined(COMPRESS) && !defined(ZLIB_COMP)
#ifdef PRAGMA_UNUSED
#pragma unused(filename)
#endif
#else
    docompress_file(filename, TRUE);
#endif
}

#ifdef ZLIB_COMP /* RLC 09 Mar 1999: Support internal ZLIB */
STATIC_OVL boolean
make_compressed_name(filename, cfn)
const char *filename;
char *cfn;
{
#ifndef SHORT_FILENAMES
    /* Assume free-form filename with no 8.3 restrictions */
    Strcpy(cfn, filename);
    Strcat(cfn, COMPRESS_EXTENSION);
    return TRUE;
#else
#ifdef SAVE_EXTENSION
    char *bp = (char *) 0;

    Strcpy(cfn, filename);
    if ((bp = strstri(cfn, SAVE_EXTENSION))) {
        strsubst(bp, SAVE_EXTENSION, ".saz");
        return TRUE;
    } else {
        /* find last occurrence of bon */
        bp = eos(cfn);
        while (bp-- > cfn) {
            if (strstri(bp, "bon")) {
                strsubst(bp, "bon", "boz");
                return TRUE;
            }
        }
    }
#endif /* SAVE_EXTENSION */
    return FALSE;
#endif /* SHORT_FILENAMES */
}

STATIC_OVL void
docompress_file(filename, uncomp)
const char *filename;
boolean uncomp;
{
    gzFile compressedfile;
    FILE *uncompressedfile;
    char cfn[256];
    char buf[1024];
    unsigned len, len2;

    if (!make_compressed_name(filename, cfn))
        return;

    if (!uncomp) {
        /* Open the input and output files */
        /* Note that gzopen takes "wb" as its mode, even on systems where
           fopen takes "r" and "w" */

        uncompressedfile = fopen(filename, RDBMODE);
        if (!uncompressedfile) {
            pline("Error in zlib docompress_file %s", filename);
            return;
        }
        compressedfile = gzopen(cfn, "wb");
        if (compressedfile == NULL) {
            if (errno == 0) {
                pline("zlib failed to allocate memory");
            } else {
                panic("Error in docompress_file %d", errno);
            }
            fclose(uncompressedfile);
            return;
        }

        /* Copy from the uncompressed to the compressed file */

        while (1) {
            len = fread(buf, 1, sizeof(buf), uncompressedfile);
            if (ferror(uncompressedfile)) {
                pline("Failure reading uncompressed file");
                pline("Can't compress %s.", filename);
                fclose(uncompressedfile);
                gzclose(compressedfile);
                (void) unlink(cfn);
                return;
            }
            if (len == 0)
                break; /* End of file */

            len2 = gzwrite(compressedfile, buf, len);
            if (len2 == 0) {
                pline("Failure writing compressed file");
                pline("Can't compress %s.", filename);
                fclose(uncompressedfile);
                gzclose(compressedfile);
                (void) unlink(cfn);
                return;
            }
        }

        fclose(uncompressedfile);
        gzclose(compressedfile);

        /* Delete the file left behind */

        (void) unlink(filename);

    } else { /* uncomp */

        /* Open the input and output files */
        /* Note that gzopen takes "rb" as its mode, even on systems where
           fopen takes "r" and "w" */

        compressedfile = gzopen(cfn, "rb");
        if (compressedfile == NULL) {
            if (errno == 0) {
                pline("zlib failed to allocate memory");
            } else if (errno != ENOENT) {
                panic("Error in zlib docompress_file %s, %d", filename,
                      errno);
            }
            return;
        }
        uncompressedfile = fopen(filename, WRBMODE);
        if (!uncompressedfile) {
            pline("Error in zlib docompress file uncompress %s", filename);
            gzclose(compressedfile);
            return;
        }

        /* Copy from the compressed to the uncompressed file */

        while (1) {
            len = gzread(compressedfile, buf, sizeof(buf));
            if (len == (unsigned) -1) {
                pline("Failure reading compressed file");
                pline("Can't uncompress %s.", filename);
                fclose(uncompressedfile);
                gzclose(compressedfile);
                (void) unlink(filename);
                return;
            }
            if (len == 0)
                break; /* End of file */

            fwrite(buf, 1, len, uncompressedfile);
            if (ferror(uncompressedfile)) {
                pline("Failure writing uncompressed file");
                pline("Can't uncompress %s.", filename);
                fclose(uncompressedfile);
                gzclose(compressedfile);
                (void) unlink(filename);
                return;
            }
        }

        fclose(uncompressedfile);
        gzclose(compressedfile);

        /* Delete the file left behind */
        (void) unlink(cfn);
    }
}
#endif /* RLC 09 Mar 1999: End ZLIB patch */

/* ----------  END FILE COMPRESSION HANDLING ----------- */

/* ----------  BEGIN FILE LOCKING HANDLING ----------- */

STATIC_VAR int nesting = 0;

#if defined(NO_FILE_LINKS) || defined(USE_FCNTL) /* implies UNIX */
STATIC_VAR int lockfd; /* for lock_file() to pass to unlock_file() */
#endif
#ifdef USE_FCNTL
struct flock sflock; /* for unlocking, same as above */
#endif

#define HUP if (!program_state.done_hup)

#ifndef USE_FCNTL
STATIC_OVL char *
make_lockname(filename, lockname)
const char *filename;
char *lockname;
{
#if defined(UNIX) || defined(VMS) || defined(AMIGA) || defined(WIN32) \
    || defined(MSDOS)
#ifdef NO_FILE_LINKS
    Strcpy(lockname, LOCKDIR);
    Strcat(lockname, "/");
    Strcat(lockname, filename);
#else
    Strcpy(lockname, filename);
#endif
#ifdef VMS
    {
        char *semi_colon = rindex(lockname, ';');
        if (semi_colon)
            *semi_colon = '\0';
    }
    Strcat(lockname, ".lock;1");
#else
    Strcat(lockname, "_lock");
#endif
    return lockname;
#else /* !(UNIX || VMS || AMIGA || WIN32 || MSDOS) */
#ifdef PRAGMA_UNUSED
#pragma unused(filename)
#endif
    lockname[0] = '\0';
    return (char *) 0;
#endif
}
#endif /* !USE_FCNTL */

/* lock a file */
boolean
lock_file(filename, whichprefix, retryct)
const char *filename;
int whichprefix;
int retryct;
{
#if defined(PRAGMA_UNUSED) && !(defined(UNIX) || defined(VMS)) \
    && !(defined(AMIGA) || defined(WIN32) || defined(MSDOS))
#pragma unused(retryct)
#endif
#ifndef USE_FCNTL
    char locknambuf[BUFSZ];
    const char *lockname;
#endif

    nesting++;
    if (nesting > 1) {
        impossible("TRIED TO NEST LOCKS");
        return TRUE;
    }

#ifndef USE_FCNTL
    lockname = make_lockname(filename, locknambuf);
#ifndef NO_FILE_LINKS /* LOCKDIR should be subsumed by LOCKPREFIX */
    lockname = fqname(lockname, LOCKPREFIX, 2);
#endif
#endif
    filename = fqname(filename, whichprefix, 0);
#ifdef USE_FCNTL
    lockfd = open(filename, O_RDWR);
    if (lockfd == -1) {
        HUP raw_printf("Cannot open file %s.  Is GnollHack installed correctly?",
                       filename);
        nesting--;
        return FALSE;
    }
    sflock.l_type = F_WRLCK;
    sflock.l_whence = SEEK_SET;
    sflock.l_start = 0;
    sflock.l_len = 0;
#endif

#if defined(UNIX) || defined(VMS)
#ifdef USE_FCNTL
    while (fcntl(lockfd, F_SETLK, &sflock) == -1) {
#else
#ifdef NO_FILE_LINKS
    while ((lockfd = open(lockname, O_RDWR | O_CREAT | O_EXCL, 0666)) == -1) {
#else
    while (link(filename, lockname) == -1) {
#endif
#endif

#ifdef USE_FCNTL
        if (retryct--) {
            HUP raw_printf(
               "Waiting for release of fcntl lock on %s.  (%d retries left.)",
                           filename, retryct);
            sleep(1);
        } else {
            HUP(void) raw_print("I give up.  Sorry.");
            HUP raw_printf("Some other process has an unnatural grip on %s.",
                           filename);
            nesting--;
            return FALSE;
        }
#else
        register int errnosv = errno;

        switch (errnosv) { /* George Barbanis */
        case EEXIST:
            if (retryct--) {
                HUP raw_printf(
                    "Waiting for access to %s.  (%d retries left).", filename,
                    retryct);
#if defined(SYSV) || defined(ULTRIX) || defined(VMS)
                (void)
#endif
                    sleep(1);
            } else {
                HUP(void) raw_print("I give up.  Sorry.");
                HUP raw_printf("Perhaps there is an old %s around?",
                               lockname);
                nesting--;
                return FALSE;
            }

            break;
        case ENOENT:
            HUP raw_printf("Can't find file %s to lock!", filename);
            nesting--;
            return FALSE;
        case EACCES:
            HUP raw_printf("No write permission to lock %s!", filename);
            nesting--;
            return FALSE;
#ifdef VMS /* c__translate(vmsfiles.c) */
        case EPERM:
            /* could be misleading, but usually right */
            HUP raw_printf("Can't lock %s due to directory protection.",
                           filename);
            nesting--;
            return FALSE;
#endif
        case EROFS:
            /* take a wild guess at the underlying cause */
            HUP perror(lockname);
            HUP raw_printf("Cannot lock %s.", filename);
            HUP raw_printf(
  "(Perhaps you are running GnollHack from inside the distribution package?).");
            nesting--;
            return FALSE;
        default:
            HUP perror(lockname);
            HUP raw_printf("Cannot lock %s for unknown reason (%d).",
                           filename, errnosv);
            nesting--;
            return FALSE;
        }
#endif /* USE_FCNTL */
    }
#endif /* UNIX || VMS */

#if (defined(AMIGA) || defined(WIN32) || defined(MSDOS)) \
    && !defined(USE_FCNTL)
#ifdef AMIGA
#define OPENFAILURE(fd) (!fd)
    lockptr = 0;
#else
#define OPENFAILURE(fd) (fd < 0)
    lockptr = -1;
#endif
    while (--retryct && OPENFAILURE(lockptr)) {
#if defined(WIN32) && !defined(WIN_CE) && !defined(GNH_WIN)
        lockptr = sopen(lockname, O_RDWR | O_CREAT, SH_DENYRW, S_IWRITE);
#else
        (void) DeleteFile(lockname); /* in case dead process was here first */
#ifdef AMIGA
        lockptr = Open(lockname, MODE_NEWFILE);
#else
        lockptr = open(lockname, O_RDWR | O_CREAT | O_EXCL, S_IWRITE);
#endif
#endif
        if (OPENFAILURE(lockptr)) {
            raw_printf("Waiting for access to %s.  (%d retries left).",
                       filename, retryct);
            Delay(50);
        }
    }
    if (!retryct) {
        raw_printf("I give up.  Sorry.");
        nesting--;
        return FALSE;
    }
#endif /* AMIGA || WIN32 || MSDOS */
    return TRUE;
}

#ifdef VMS /* for unlock_file, use the unlink() routine in vmsunix.c */
#ifdef unlink
#undef unlink
#endif
#define unlink(foo) vms_unlink(foo)
#endif

/* unlock file, which must be currently locked by lock_file */
void
unlock_file(filename)
const char *filename;
{
#ifndef USE_FCNTL
    char locknambuf[BUFSZ];
    const char *lockname;
#endif

    if (nesting == 1) {
#ifdef USE_FCNTL
        sflock.l_type = F_UNLCK;
        if (fcntl(lockfd, F_SETLK, &sflock) == -1) {
            HUP raw_printf("Can't remove fcntl lock on %s.", filename);
            (void) close(lockfd);
        }
#else
        lockname = make_lockname(filename, locknambuf);
#ifndef NO_FILE_LINKS /* LOCKDIR should be subsumed by LOCKPREFIX */
        lockname = fqname(lockname, LOCKPREFIX, 2);
#endif

#if defined(UNIX) || defined(VMS)
        if (unlink(lockname) < 0)
            HUP raw_printf("Can't unlink %s.", lockname);
#ifdef NO_FILE_LINKS
        (void) nhclose(lockfd);
#endif

#endif /* UNIX || VMS */

#if defined(AMIGA) || defined(WIN32) || defined(MSDOS)
        if (lockptr)
            Close(lockptr);
        DeleteFile(lockname);
        lockptr = 0;
#endif /* AMIGA || WIN32 || MSDOS */
#endif /* USE_FCNTL */
    }

    nesting--;
}

/* ----------  END FILE LOCKING HANDLING ----------- */

/* ----------  BEGIN CONFIG FILE HANDLING ----------- */

const char *default_configfile =
#if defined(UNIX) && !defined(ANDROID) && !defined(GNH_MOBILE)
    ".gnollhackrc";
#else
#if defined(MAC) || defined(__BEOS__)
    "GnollHack Defaults";
#else
#if defined(MSDOS) || defined(WIN32) || defined(ANDROID) || defined(GNH_MOBILE)
    "defaults.gnh";
#else
    "GnollHack.cnf";
#endif
#endif
#endif

/* used for messaging */
char configfile[BUFSZ];

#ifdef MSDOS
/* conflict with speed-dial under windows
 * for XXX.cnf file so support of GnollHack.cnf
 * is for backward compatibility only.
 * Preferred name (and first tried) is now defaults.gnh but
 * the game will try the old name if there
 * is no defaults.gnh.
 */
const char *backward_compat_configfile = "GnollHack.cnf";
#endif

/* remember the name of the file we're accessing;
   if may be used in option reject messages */
STATIC_OVL void
set_configfile_name(fname)
const char *fname;
{
    Strncpy(configfile, fname, sizeof configfile - 1);
    configfile[sizeof configfile - 1] = '\0';
}

#ifndef MFLOPPY
#define fopenp fopen
#endif

STATIC_OVL FILE *
fopen_config_file(filename, src)
const char *filename;
int src;
{
    FILE *fp;
#if (defined(UNIX) || defined(VMS)) && !(defined(ANDROID) || defined(GNH_MOBILE))
    char tmp_config[BUFSZ];
    char *envp;
#endif

    if (src == SET_IN_SYS) {
        /* SYSCF_FILE; if we can't open it, caller will bail */
        if (filename && *filename) {
            set_configfile_name(fqname(filename, SYSCONFPREFIX, 0));
            fp = fopenp(configfile, "r");
        } else
            fp = (FILE *) 0;
        return  fp;
    }
    /* If src != SET_IN_SYS, "filename" is an environment variable, so it
     * should hang around. If set, it is expected to be a full path name
     * (if relevant)
     */
    if (filename && *filename) {
        set_configfile_name(filename);
#ifdef UNIX
        if (access(configfile, 4) == -1) { /* 4 is R_OK on newer systems */
            /* nasty sneaky attempt to read file through
             * GnollHack's setuid permissions -- this is the only
             * place a file name may be wholly under the player's
             * control (but SYSCF_FILE is not under the player's
             * control so it's OK).
             */
            raw_printf("Access to %s denied (%d).", configfile, errno);
            wait_synch();
            /* fall through to standard names */
        } else
#endif
        if ((fp = fopenp(configfile, "r")) != (FILE *) 0) {
            return  fp;
#if defined(UNIX) || defined(VMS)
        } else {
            /* access() above probably caught most problems for UNIX */
            raw_printf("Couldn't open requested config file %s (%d).",
                       configfile, errno);
            wait_synch();
#endif
        }
    }
    /* fall through to standard names */

#if defined(MICRO) || defined(MAC) || defined(__BEOS__) || defined(WIN32) || defined(ANDROID) || defined(GNH_MOBILE)
    set_configfile_name(fqname(default_configfile, CONFIGPREFIX, 0));
    if ((fp = fopenp(configfile, "r")) != (FILE *) 0) {
        return fp;
    } else if (strcmp(default_configfile, configfile)) {
        set_configfile_name(default_configfile);
        if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
            return fp;
    }
#ifdef MSDOS
    set_configfile_name(fqname(backward_compat_configfile, CONFIGPREFIX, 0));
    if ((fp = fopenp(configfile, "r")) != (FILE *) 0) {
        return fp;
    } else if (strcmp(backward_compat_configfile, configfile)) {
        set_configfile_name(backward_compat_configfile);
        if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
            return fp;
    }
#endif
#else
/* constructed full path names don't need fqname() */
#ifdef VMS
    /* no punctuation, so might be a logical name */
    set_configfile_name(fqname("GnollHackini", CONFIGPREFIX, 0));
    if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
        return fp;
    set_configfile_name("sys$login:GnollHack.ini");
    if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
        return fp;

    envp = nh_getenv("HOME");
    if (!envp || !*envp)
        Strcpy(tmp_config, "GnollHack.cnf");
    else
        Sprintf(tmp_config, "%s%s%s", envp,
                !index(":]>/", envp[strlen(envp) - 1]) ? "/" : "",
                "GnollHack.cnf");
    set_configfile_name(tmp_config);
    if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
        return fp;
#else /* should be only UNIX left */
    envp = nh_getenv("HOME");
    if (!envp)
        Strcpy(tmp_config, ".gnollhackrc");
    else
        Sprintf(tmp_config, "%s/%s", envp, ".gnollhackrc");

    set_configfile_name(tmp_config);
    if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
        return fp;
#if defined(__APPLE__) && !defined(GNH_MOBILE) /* UNIX+__APPLE__ => MacOSX */
    /* try an alternative */
    if (envp) {
        /* OSX-style configuration settings */
        Sprintf(tmp_config, "%s/%s", envp,
                "Library/Preferences/GnollHack Defaults");
        set_configfile_name(tmp_config);
        if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
            return fp;
        /* may be easier for user to edit if filename as '.txt' suffix */
        Sprintf(tmp_config, "%s/%s", envp,
                "Library/Preferences/GnollHack Defaults.txt");
        set_configfile_name(tmp_config);
        if ((fp = fopenp(configfile, "r")) != (FILE *) 0)
            return fp;
    }
#endif /*__APPLE__*/
    if (errno != ENOENT) {
        const char *details;

        /* e.g., problems when setuid GnollHack can't search home
           directory restricted to user */
#if defined(NHSTDC) && !defined(NOTSTDC)
        if ((details = strerror(errno)) == 0)
#endif
            details = "";
        raw_printf("Couldn't open default config file %s %s(%d).",
                   configfile, details, errno);
        wait_synch();
    }
#endif /* !VMS => Unix */
#endif /* !(MICRO || MAC || __BEOS__ || WIN32) */
    return (FILE *) 0;
}

/*
 * Retrieve a list of integers from buf into a nhsym array.
 *
 * NOTE: zeros are inserted unless modlist is TRUE, in which case the list
 *  location is unchanged.  Callers must handle zeros if modlist is FALSE.
 */
STATIC_OVL int
get_nhsyms(bufp, list, modlist, size, name)
char* bufp;       /* current pointer */
nhsym* list;      /* return list */
boolean modlist;  /* TRUE: list is being modified in place */
int size;         /* return list size */
const char* name; /* name of option for error message */
{
    unsigned int num = 0;
    int count = 0;
    boolean havenum = FALSE;

    while (1) {
        switch (*bufp) {
        case ' ':
        case '\0':
        case '\t':
        case '\n':
            if (havenum) {
                /* if modifying in place, don't insert zeros */
                if (num || !modlist)
                    list[count] = num;
                count++;
                num = 0;
                havenum = FALSE;
            }
            if (count == size || !*bufp)
                return count;
            bufp++;
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            havenum = TRUE;
            num = num * 10 + (*bufp - '0');
            bufp++;
            break;

        case '\\':
            goto gi_error;
            break;

        default:
        gi_error:
            raw_printf("Syntax error in %s", name);
            wait_synch();
            return count;
        }
    }
    /*NOTREACHED*/
}

#ifdef NOCWD_ASSUMPTIONS
STATIC_OVL void
adjust_prefix(bufp, prefixid)
char *bufp;
int prefixid;
{
    char *ptr;

    if (!bufp)
        return;
    /* Backward compatibility, ignore trailing ;n */
    if ((ptr = index(bufp, ';')) != 0)
        *ptr = '\0';
    if (strlen(bufp) > 0) {
        fqn_prefix[prefixid] = (char *) alloc(strlen(bufp) + 2);
        Strcpy(fqn_prefix[prefixid], bufp);
        append_slash(fqn_prefix[prefixid]);
    }
}
#endif

/* Choose at random one of the sep separated parts from str. Mangles str. */
STATIC_OVL char *
choose_random_part(str,sep)
char *str;
char sep;
{
    int nsep = 1;
    int csep;
    int len = 0;
    char *begin = str;

    if (!str)
        return (char *) 0;

    while (*str) {
        if (*str == sep)
            nsep++;
        str++;
    }
    csep = rn2(nsep);
    str = begin;
    while ((csep > 0) && *str) {
        str++;
        if (*str == sep)
            csep--;
    }
    if (*str) {
        if (*str == sep)
            str++;
        begin = str;
        while (*str && *str != sep) {
            str++;
            len++;
        }
        *str = '\0';
        if (len)
            return begin;
    }
    return (char *) 0;
}

STATIC_OVL void
free_config_sections()
{
    Strcpy(config_section_chosen, "");
    Strcpy(config_section_current, "");
#if 0
    if (config_section_chosen) {
        free(config_section_chosen);
        config_section_chosen = NULL;
    }
    if (config_section_current) {
        free(config_section_current);
        config_section_current = NULL;
    }
#endif
}

STATIC_OVL boolean
is_config_section(str)
const char *str;
{
    const char *a = rindex(str, ']');

    return (a && *str == '[' && *(a+1) == '\0' && (int)(a - str) > 0);
}

STATIC_OVL boolean
handle_config_section(buf)
char *buf;
{
    if (is_config_section(buf)) 
    {
        char *send;
#if 0
        if (strcmp(config_section_current, ""))
        {
            Strcpy(config_section_current, "");
            //free(config_section_current);
        }
#endif
        Strcpy(config_section_current, &buf[1]);
        //config_section_current = dupstr(&buf[1]);
        send = rindex(config_section_current, ']');
        *send = '\0';
        debugpline1("set config section: '%s'", config_section_current);
        return TRUE;
    }

    if (strcmp(config_section_current, ""))
    {
        if (!strcmp(config_section_chosen, ""))
            return TRUE;
        if (strcmp(config_section_current, config_section_chosen))
            return TRUE;
    }
    return FALSE;
}

#define match_varname(INP, NAM, LEN) match_optname(INP, NAM, LEN, TRUE)

/* find the '=' or ':' */
char *
find_optparam(buf)
const char *buf;
{
    char *bufp, *altp;

    bufp = index(buf, '=');
    altp = index(buf, ':');
    if (!bufp || (altp && altp < bufp))
        bufp = altp;

    return bufp;
}

boolean
parse_config_line(origbuf)
char *origbuf;
{
#if defined(MICRO) && !defined(NOCWD_ASSUMPTIONS)
    static boolean ramdisk_specified = FALSE;
#endif
#ifdef SYSCF
    int n;
#endif
    char *bufp, buf[INBUF_SIZ];
    nhsym translate[MAX_CMAPPED_CHARS];
    int len;
    boolean retval = TRUE;
    int src = iflags.parse_config_file_src;

    while (*origbuf == ' ' || *origbuf == '\t') /* skip leading whitespace */
        ++origbuf;                   /* (caller probably already did this) */
    Strncpy(buf, origbuf, sizeof buf - 1);
    buf[sizeof buf - 1] = '\0'; /* strncpy not guaranteed to NULL terminate */

    /* convert any tab to space, condense consecutive spaces into one,
       remove leading and trailing spaces (exception: if there is nothing
       but spaces, one of them will be kept even though it leads/trails) */
    mungspaces(buf);

    /* find the '=' or ':' */
    bufp = find_optparam(buf);
    if (!bufp)
    {
        config_error_add("Not a config statement, missing '='");
        return FALSE;
    }
    /* skip past '=', then space between it and value, if any */
    ++bufp;
    if (*bufp == ' ')
        ++bufp;

    /* Go through possible variables */
    /* some of these (at least LEVELS and SAVE) should now set the
     * appropriate fqn_prefix[] rather than specialized variables
     */
    if (match_varname(buf, "OPTIONS", 4))
    {
        /* hack: un-mungspaces to allow consecutive spaces in
           general options until we verify that this is unnecessary;
           '=' or ':' is guaranteed to be present */
        bufp = find_optparam(origbuf);
        ++bufp; /* skip '='; parseoptions() handles spaces */

        if (!parseoptions(bufp, TRUE, TRUE))
            retval = FALSE;
    } 
    else if (match_varname(buf, "AUTOPICKUP_EXCEPTION", 5)) 
    {
        add_autopickup_exception(bufp);
    } 
    else if (match_varname(buf, "BINDINGS", 4))
    {
        if (!parsebindings(bufp))
            retval = FALSE;
        update_bindings_list();
    }
    else if (match_varname(buf, "AUTOCOMPLETE", 5))
    {
        parseautocomplete(bufp, TRUE);
    }
    else if (match_varname(buf, "MSGTYPE", 7))
    {
        if (!msgtype_parse_add(bufp))
            retval = FALSE;
#ifdef NOCWD_ASSUMPTIONS
    } 
    else if (match_varname(buf, "HACKDIR", 4))
    {
        adjust_prefix(bufp, HACKPREFIX);
    } 
    else if (match_varname(buf, "LEVELDIR", 4)
               || match_varname(buf, "LEVELS", 4))
    {
        adjust_prefix(bufp, LEVELPREFIX);
    } 
    else if (match_varname(buf, "SAVEDIR", 4))
    {
        adjust_prefix(bufp, SAVEPREFIX);
    } 
    else if (match_varname(buf, "BONESDIR", 5)) 
    {
        adjust_prefix(bufp, BONESPREFIX);
    } 
    else if (match_varname(buf, "DATADIR", 4)) 
    {
        adjust_prefix(bufp, DATAPREFIX);
    } 
    else if (match_varname(buf, "SCOREDIR", 4))
    {
        adjust_prefix(bufp, SCOREPREFIX);
    } 
    else if (match_varname(buf, "LOCKDIR", 4))
    {
        adjust_prefix(bufp, LOCKPREFIX);
    } 
    else if (match_varname(buf, "CONFIGDIR", 4)) 
    {
        adjust_prefix(bufp, CONFIGPREFIX);
    } 
    else if (match_varname(buf, "TROUBLEDIR", 4)) 
    {
        adjust_prefix(bufp, TROUBLEPREFIX);
#else /*NOCWD_ASSUMPTIONS*/
#ifdef MICRO
    } 
    else if (match_varname(buf, "HACKDIR", 4)) 
    {
        Strncpy(hackdir, bufp, PATHLEN - 1);
#ifdef MFLOPPY
    }
    else if (match_varname(buf, "RAMDISK", 3)) 
    {
/* The following ifdef is NOT in the wrong
 * place.  For now, we accept and silently
 * ignore RAMDISK */
#ifndef AMIGA
        if (strlen(bufp) >= PATHLEN)
            bufp[PATHLEN - 1] = '\0';
        Strcpy(levels, bufp);
        ramdisk = (strcmp(permbones, levels) != 0);
        ramdisk_specified = TRUE;
#endif
#endif
    } 
    else if (match_varname(buf, "LEVELS", 4)) 
    {
        if (strlen(bufp) >= PATHLEN)
            bufp[PATHLEN - 1] = '\0';
        Strcpy(permbones, bufp);
        if (!ramdisk_specified || !*levels)
            Strcpy(levels, bufp);
        ramdisk = (strcmp(permbones, levels) != 0);
    } else if (match_varname(buf, "SAVE", 4)) 
    {
#ifdef MFLOPPY
        extern int saveprompt;
#endif
        char *ptr;

        if ((ptr = index(bufp, ';')) != 0) 
        {
            *ptr = '\0';
#ifdef MFLOPPY
            if (*(ptr + 1) == 'n' || *(ptr + 1) == 'N') 
            {
                saveprompt = FALSE;
            }
#endif
        }
#if defined(SYSFLAGS) && defined(MFLOPPY)
        else
            saveprompt = sysflags.asksavedisk;
#endif

        Strncpy(SAVEP, bufp, SAVESIZE - 1);
        append_slash(SAVEP);
#endif /* MICRO */
#endif /*NOCWD_ASSUMPTIONS*/

    }
    else if (match_varname(buf, "NAME", 4))
    {
        Strncpy(plname, bufp, PL_NSIZ - 1);
    }
    else if (match_varname(buf, "ROLE", 4)
               || match_varname(buf, "CHARACTER", 4)) 
    {
        if ((len = str2role(bufp)) >= 0)
            flags.initrole = len;
    }
    else if (match_varname(buf, "DOGNAME", 3)) 
    {
        Strncpy(dogname, bufp, PL_PSIZ - 1);
    }
    else if (match_varname(buf, "CATNAME", 3)) 
    {
        Strncpy(catname, bufp, PL_PSIZ - 1);

#ifdef SYSCF
    }
    else if (src == SET_IN_SYS && match_varname(buf, "WIZARDS", 7)) 
    {
        if (sysopt.wizards)
            free((genericptr_t) sysopt.wizards);
        sysopt.wizards = dupstr(bufp);
        if (strlen(sysopt.wizards) && strcmp(sysopt.wizards, "*")) 
        {
            /* pre-format WIZARDS list now; it's displayed during a panic
               and since that panic might be due to running out of memory,
               we don't want to risk attempting to allocate any memory then */
            if (sysopt.fmtd_wizard_list)
                free((genericptr_t) sysopt.fmtd_wizard_list);
            sysopt.fmtd_wizard_list = build_english_list(sysopt.wizards);
        }
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "SHELLERS", 8)) 
    {
        if (sysopt.shellers)
            free((genericptr_t) sysopt.shellers);
        sysopt.shellers = dupstr(bufp);
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "EXPLORERS", 7)) 
    {
        if (sysopt.explorers)
            free((genericptr_t) sysopt.explorers);
        sysopt.explorers = dupstr(bufp);
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DEBUGFILES", 5)) 
    {
        /* if showdebug() has already been called (perhaps we've added
           some debugpline() calls to option processing) and has found
           a value for getenv("DEBUGFILES"), don't override that */
        if (sysopt.env_dbgfl <= 0) 
        {
            if (sysopt.debugfiles)
                free((genericptr_t) sysopt.debugfiles);
            sysopt.debugfiles = dupstr(bufp);
        }
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "SNAPJSONFILE", 12)) {
#if defined (DUMPLOG) || defined (DUMPHTML)
        if (sysopt.snapjsonfile)
            free((genericptr_t)sysopt.snapjsonfile);
        sysopt.snapjsonfile = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPLOGFILE", 7))
    {
#if defined (DUMPLOG)
        if (sysopt.dumplogfile)
            free((genericptr_t) sysopt.dumplogfile);
        sysopt.dumplogfile = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "SNAPSHOTFILE", 8))
    {
#if defined (DUMPLOG)
        if (sysopt.snapshotfile)
            free((genericptr_t)sysopt.snapshotfile);
        sysopt.snapshotfile = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTMLFILE", 12)) {
#if defined (DUMPHTML)
        if (sysopt.dumphtmlfile)
            free((genericptr_t)sysopt.dumphtmlfile);
        sysopt.dumphtmlfile = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "SNAPHTMLFILE", 12)) {
#if defined (DUMPHTML)
        if (sysopt.snaphtmlfile)
            free((genericptr_t)sysopt.snaphtmlfile);
        sysopt.snaphtmlfile = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTMLFONTNAME", 16)) {
#if defined (DUMPHTML)
        if (sysopt.dumphtmlfontname)
            free((genericptr_t)sysopt.dumphtmlfontname);
        sysopt.dumphtmlfontname = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTMLFONTLINK", 16)) {
#if defined (DUMPHTML) && defined (DUMPHTML_WEBFONT_LINK)
        if (sysopt.dumphtmlfontlink)
            free((genericptr_t)sysopt.dumphtmlfontlink);
        sysopt.dumphtmlfontlink = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTML_CSS_FONTFACE_NORMAL", 28)) {
#if defined (DUMPHTML)
        if (sysopt.dumphtml_css_fontface_normal)
            free((genericptr_t)sysopt.dumphtml_css_fontface_normal);
        sysopt.dumphtml_css_fontface_normal = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTML_CSS_FONTFACE_BOLD", 26)) {
#if defined (DUMPHTML)
        if (sysopt.dumphtml_css_fontface_bold)
            free((genericptr_t)sysopt.dumphtml_css_fontface_bold);
        sysopt.dumphtml_css_fontface_bold = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTML_CSS_FONTFACE_ITALIC", 28)) {
#if defined (DUMPHTML)
        if (sysopt.dumphtml_css_fontface_italic)
            free((genericptr_t)sysopt.dumphtml_css_fontface_italic);
        sysopt.dumphtml_css_fontface_italic = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "DUMPHTML_CSS_FONTFACE_BOLD_ITALIC", 33)) {
#if defined (DUMPHTML)
        if (sysopt.dumphtml_css_fontface_bolditalic)
            free((genericptr_t)sysopt.dumphtml_css_fontface_bolditalic);
        sysopt.dumphtml_css_fontface_bolditalic = dupstr(bufp);
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "GENERICUSERS", 12))
    {
        if (sysopt.genericusers)
            free((genericptr_t) sysopt.genericusers);
        sysopt.genericusers = dupstr(bufp);
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "BONES_POOLS", 10))
    {
        /* max value of 10 guarantees (N % bones.pools) will be one digit
           so we don't lose control of the length of bones file names */
        n = atoi(bufp);
        sysopt.bones_pools = (n <= 0) ? 0 : min(n, 10);
        /* note: right now bones_pools==0 is the same as bones_pools==1,
           but we could change that and make bones_pools==0 become an
           indicator to suppress bones usage altogether */
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "MIN_DIFFICULTY", 14))
    {
       n = atoi(bufp);
       sysopt.min_difficulty = (n <= MIN_DIFFICULTY_LEVEL) ? MIN_DIFFICULTY_LEVEL : min(n, MAX_DIFFICULTY_LEVEL);
    }
    else if (src == SET_IN_SYS && match_varname(buf, "MAX_DIFFICULTY", 14))
    {
        n = atoi(bufp);
        sysopt.max_difficulty = (n <= MIN_DIFFICULTY_LEVEL) ? MIN_DIFFICULTY_LEVEL : min(n, MAX_DIFFICULTY_LEVEL);
    }
    else if (src == SET_IN_SYS && match_varname(buf, "SUPPORT", 7))
    {
        if (sysopt.support)
            free((genericptr_t) sysopt.support);
        sysopt.support = dupstr(bufp);
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "RECOVER", 7))
    {
        if (sysopt.recover)
            free((genericptr_t) sysopt.recover);
        sysopt.recover = dupstr(bufp);
    } 
    else if (src == SET_IN_SYS
               && match_varname(buf, "CHECK_SAVE_UID", 14)) 
    {
        n = atoi(bufp);
        sysopt.check_save_uid = n;
    }
    else if (src == SET_IN_SYS
               && match_varname(buf, "CHECK_PLNAME", 12)) 
    {
        n = atoi(bufp);
        sysopt.check_plname = n;
    }
    else if (src == SET_IN_SYS
        && match_varname(buf, "SELECT_PET_DETAILS", 18))
    {
        n = atoi(bufp);
        sysopt.select_pet_details = n;
    }
    else if (src == SET_IN_SYS
        && match_varname(buf, "MAKE_BACKUP_SAVEFILES", 21))
    {
            n = atoi(bufp);
            sysopt.make_backup_savefiles = n;
    }
    else if (src == SET_IN_SYS
        && match_varname(buf, "ALLOW_LOADING_ERROR_SAVEFILES", 29))
        {
            n = atoi(bufp);
            sysopt.allow_loading_error_savefiles = n;
            }
    else if (match_varname(buf, "SEDUCE", 6))
    {
        n = !!atoi(bufp); /* XXX this could be tighter */
        /* allow anyone to turn it off, but only sysconf to turn it on*/
        if (src != SET_IN_SYS && n != 0)
        {
            config_error_add("Illegal value in SEDUCE");
            return FALSE;
        }
        sysopt.seduce = n;
        sysopt_seduce_set(sysopt.seduce);
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "MAXPLAYERS", 10)) 
    {
        n = atoi(bufp);
        /* XXX to get more than 25, need to rewrite all lock code */
        if (n < 0 || n > 25)
        {
            config_error_add("Illegal value in MAXPLAYERS (maximum is 25).");
            return FALSE;
        }
        sysopt.maxplayers = n;
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "PERSMAX", 7)) 
    {
        n = atoi(bufp);
        if (n < 1)
        {
            config_error_add("Illegal value in PERSMAX (minimum is 1).");
            return FALSE;
        }
        sysopt.persmax = n;
    }
    else if (src == SET_IN_SYS && match_varname(buf, "PERS_IS_UID", 11)) 
    {
        n = atoi(bufp);
        if (n != 0 && n != 1) 
        {
            config_error_add("Illegal value in PERS_IS_UID (must be 0 or 1).");
            return FALSE;
        }
        sysopt.pers_is_uid = n;
    }
    else if (src == SET_IN_SYS && match_varname(buf, "ENTRYMAX", 8)) 
{
        n = atoi(bufp);
        if (n < 10) 
        {
            config_error_add("Illegal value in ENTRYMAX (minimum is 10).");
            return FALSE;
        }
        sysopt.entrymax = n;
    } 
    else if ((src == SET_IN_SYS) && match_varname(buf, "POINTSMIN", 9)) 
    {
        n = atoi(bufp);
        if (n < 1) 
        {
            config_error_add("Illegal value in POINTSMIN (minimum is 1).");
            return FALSE;
        }
        sysopt.pointsmin = n;
    } else if (src == SET_IN_SYS
               && match_varname(buf, "MAX_STATUENAME_RANK", 10)) 
    {
        n = atoi(bufp);
        if (n < 1)
        {
            config_error_add(
                      "Illegal value in MAX_STATUENAME_RANK (minimum is 1).");
            return FALSE;
        }
        sysopt.tt_oname_maxrank = n;

    } else if (src == SET_IN_SYS && match_varname(buf, "LIVELOG", 7)) {
#ifdef LIVELOGFILE
        n = strtol(bufp, NULL, 0);
        if (n < 0 || n > 0xFFFF) {
            raw_printf("Illegal value in LIVELOG (must be between 0 and 0xFFFF).");
            return 0;
        }
        sysopt.livelog = n;
#else
        raw_printf("WARNING: LIVELOG value configured but LIVELOGFILE not #defined. Ignored.");
#endif
    }
    else if (src == SET_IN_SYS && match_varname(buf, "LLC_TURNS", 9)) {
            n = atoi(bufp);
            if (n < 0) {
                raw_printf("Illegal value in LLC_TURNS (must be a positive integer).");
                return 0;
            }
            sysopt.ll_conduct_turns = n;

    /* SYSCF PANICTRACE options */
    } else if (src == SET_IN_SYS
               && match_varname(buf, "PANICTRACE_LIBC", 15)) 
    {
        n = atoi(bufp);
#if defined(PANICTRACE) && defined(PANICTRACE_LIBC)
        if (n < 0 || n > 2) {
            config_error_add("Illegal value in PANICTRACE_LIBC (not 0,1,2).");
            return FALSE;
        }
#endif
        sysopt.panictrace_libc = n;
    }
    else if (src == SET_IN_SYS
               && match_varname(buf, "PANICTRACE_GDB", 14)) 
    {
        n = atoi(bufp);
#if defined(PANICTRACE)
        if (n < 0 || n > 2)
        {
            config_error_add("Illegal value in PANICTRACE_GDB (not 0,1,2).");
            return FALSE;
        }
#endif
        sysopt.panictrace_gdb = n;
    }
    else if (src == SET_IN_SYS && match_varname(buf, "GDBPATH", 7))
    {
#if defined(PANICTRACE) && !defined(VMS)
        if (!file_exists(bufp)) {
            config_error_add("File specified in GDBPATH does not exist.");
            return FALSE;
        }
#endif
        if (sysopt.gdbpath)
            free((genericptr_t) sysopt.gdbpath);
        sysopt.gdbpath = dupstr(bufp);
    } 
    else if (src == SET_IN_SYS && match_varname(buf, "GREPPATH", 7))
    {
#if defined(PANICTRACE) && !defined(VMS)
        if (!file_exists(bufp)) {
            config_error_add("File specified in GREPPATH does not exist.");
            return FALSE;
        }
#endif
        if (sysopt.greppath)
            free((genericptr_t) sysopt.greppath);
        sysopt.greppath = dupstr(bufp);
#endif /* SYSCF */

    } 
    else if (match_varname(buf, "BOULDER", 3)) 
    {
        (void) get_nhsyms(bufp, &iflags.bouldersym, TRUE, 1,
                          "BOULDER");
    }
    else if (match_varname(buf, "MENUCOLOR", 9))
    {
        if (!add_menu_coloring(bufp))
            retval = FALSE;
    }
    else if (match_varname(buf, "HILITE_STATUS", 6))
    {
#ifdef STATUS_HILITES
        if (!parse_status_hl1(bufp, TRUE))
            retval = FALSE;
#endif
    } 
    else if (match_varname(buf, "WARNINGS", 5)) 
    {
        (void) get_nhsyms(bufp, translate, FALSE, WARNCOUNT,
                          "WARNINGS");
        assign_warnings(translate);
    }
    else if (match_varname(buf, "SYMBOLS", 4)) 
    {
        if (!parsesymbols(bufp))
        {
            config_error_add("Error in SYMBOLS definition '%s'", bufp);
            retval = FALSE;
        }
        switch_symbols(TRUE);
    }
    else if (match_varname(buf, "WIZKIT", 6)) 
    {
        Strncpy(wizkit, bufp, WIZKIT_MAX - 1);
#ifdef AMIGA
    } 
    else if (match_varname(buf, "FONT", 4))
    {
        char *t;

        if (t = strchr(buf + 5, ':'))
        {
            *t = 0;
            amii_set_text_font(buf + 5, atoi(t + 1));
            *t = ':';
        }
    }
    else if (match_varname(buf, "PATH", 4))
    {
        Strncpy(PATH, bufp, PATHLEN - 1);
    }
    else if (match_varname(buf, "DEPTH", 5)) 
    {
        extern int amii_numcolors;
        int val = atoi(bufp);

        amii_numcolors = 1L << min(DEPTH, val);
#ifdef SYSFLAGS
    } 
    else if (match_varname(buf, "DRIPENS", 7))
    {
        int i, val;
        char *t;

        for (i = 0, t = strtok(bufp, ",/"); t != (char *) 0;
             i < 20 && (t = strtok((char *) 0, ",/")), ++i)
        {
            sscanf(t, "%d", &val);
            sysflags.amii_dripens[i] = val;
        }
#endif
    } 
    else if (match_varname(buf, "SCREENMODE", 10)) 
    {
        extern int64_t amii_scrnmode;

        if (!stricmp(bufp, "req"))
            amii_scrnmode = 0xffffffff; /* Requester */
        else if (sscanf(bufp, "%x", &amii_scrnmode) != 1)
            amii_scrnmode = 0;
    } 
    else if (match_varname(buf, "MSGPENS", 7)) 
    {
        extern int amii_msgAPen, amii_msgBPen;
        char *t = strtok(bufp, ",/");

        if (t)
        {
            sscanf(t, "%d", &amii_msgAPen);
            if (t = strtok((char *) 0, ",/"))
                sscanf(t, "%d", &amii_msgBPen);
        }
    } 
    else if (match_varname(buf, "TEXTPENS", 8)) 
    {
        extern int amii_textAPen, amii_textBPen;
        char *t = strtok(bufp, ",/");

        if (t) 
        {
            sscanf(t, "%d", &amii_textAPen);
            if (t = strtok((char *) 0, ",/"))
                sscanf(t, "%d", &amii_textBPen);
        }
    } 
    else if (match_varname(buf, "MENUPENS", 8))
    {
        extern int amii_menuAPen, amii_menuBPen;
        char *t = strtok(bufp, ",/");

        if (t) 
        {
            sscanf(t, "%d", &amii_menuAPen);
            if (t = strtok((char *) 0, ",/"))
                sscanf(t, "%d", &amii_menuBPen);
        }
    } 
    else if (match_varname(buf, "STATUSPENS", 10)) 
    {
        extern int amii_statAPen, amii_statBPen;
        char *t = strtok(bufp, ",/");

        if (t) 
        {
            sscanf(t, "%d", &amii_statAPen);
            if (t = strtok((char *) 0, ",/"))
                sscanf(t, "%d", &amii_statBPen);
        }
    } 
    else if (match_varname(buf, "OTHERPENS", 9)) 
    {
        extern int amii_otherAPen, amii_otherBPen;
        char *t = strtok(bufp, ",/");

        if (t)
        {
            sscanf(t, "%d", &amii_otherAPen);
            if (t = strtok((char *) 0, ",/"))
                sscanf(t, "%d", &amii_otherBPen);
        }
    }
    else if (match_varname(buf, "PENS", 4)) 
    {
        extern unsigned short amii_init_map[AMII_MAXCOLORS];
        int i;
        char *t;

        for (i = 0, t = strtok(bufp, ",/");
             i < AMII_MAXCOLORS && t != (char *) 0;
             t = strtok((char *) 0, ",/"), ++i) 
        {
            sscanf(t, "%hx", &amii_init_map[i]);
        }
        amii_setpens(amii_numcolors = i);
    } 
    else if (match_varname(buf, "FGPENS", 6)) 
    {
        extern int foreg[AMII_MAXCOLORS];
        int i;
        char *t;

        for (i = 0, t = strtok(bufp, ",/");
             i < AMII_MAXCOLORS && t != (char *) 0;
             t = strtok((char *) 0, ",/"), ++i)
        {
            sscanf(t, "%d", &foreg[i]);
        }
    }
    else if (match_varname(buf, "BGPENS", 6))
    {
        extern int backg[AMII_MAXCOLORS];
        int i;
        char *t;

        for (i = 0, t = strtok(bufp, ",/");
             i < AMII_MAXCOLORS && t != (char *) 0;
             t = strtok((char *) 0, ",/"), ++i) 
        {
            sscanf(t, "%d", &backg[i]);
        }
#endif /*AMIGA*/
#ifdef USER_SOUNDS
    } 
    else if (match_varname(buf, "SOUNDDIR", 8)) 
    {
        sounddir = dupstr(bufp);
    }
    else if (match_varname(buf, "SOUND", 5)) 
    {
        add_sound_mapping(bufp);
#endif
    } 
    else if (match_varname(buf, "QT_TILEWIDTH", 12)) 
    {
#ifdef QT_GRAPHICS
        extern char *qt_tilewidth;

        if (qt_tilewidth == NULL)
            qt_tilewidth = dupstr(bufp);
#endif
    } 
    else if (match_varname(buf, "QT_TILEHEIGHT", 13)) 
    {
#ifdef QT_GRAPHICS
        extern char *qt_tileheight;

        if (qt_tileheight == NULL)
            qt_tileheight = dupstr(bufp);
#endif
    }
    else if (match_varname(buf, "QT_FONTSIZE", 11)) 
    {
#ifdef QT_GRAPHICS
        extern char *qt_fontsize;

        if (qt_fontsize == NULL)
            qt_fontsize = dupstr(bufp);
#endif
    }
    else if (match_varname(buf, "QT_COMPACT", 10)) 
    {
#ifdef QT_GRAPHICS
        extern int qt_compact_mode;

        qt_compact_mode = atoi(bufp);
#endif
    } 
    else 
    {
        config_error_add("Unknown config statement");
        return FALSE;
    }
    return retval;
}

#ifdef USER_SOUNDS
boolean
can_read_file(filename)
const char *filename;
{
    return (boolean) (access(filename, 4) == 0);
}
#endif /* USER_SOUNDS */

struct _config_error_frame {
    int line_num;
    int num_errors;
    boolean origline_shown;
    boolean fromfile;
    boolean secure;
    char origline[INBUF_SIZ];
    char source[BUFSZ];
    struct _config_error_frame *next;
};

STATIC_VAR struct _config_error_frame *config_error_data = 0;

void
config_error_init(from_file, sourcename, secure)
boolean from_file;
const char *sourcename;
boolean secure;
{
    struct _config_error_frame *tmp = (struct _config_error_frame *)
        alloc(sizeof (struct _config_error_frame));

    tmp->line_num = 0;
    tmp->num_errors = 0;
    tmp->origline_shown = FALSE;
    tmp->fromfile = from_file;
    tmp->secure = secure;
    tmp->origline[0] = '\0';
    if (sourcename && sourcename[0]) {
        Strncpy(tmp->source, sourcename, sizeof (tmp->source) - 1);
        tmp->source[sizeof (tmp->source) - 1] = '\0';
    } else
        tmp->source[0] = '\0';

    tmp->next = config_error_data;
    config_error_data = tmp;
}

STATIC_OVL boolean
config_error_nextline(line)
const char *line;
{
    struct _config_error_frame *ced = config_error_data;

    if (!ced)
        return FALSE;

    if (ced->num_errors && ced->secure)
        return FALSE;

    ced->line_num++;
    ced->origline_shown = FALSE;
    if (line && line[0])
    {
        Strncpy(ced->origline, line, sizeof (ced->origline) - 1);
        ced->origline[sizeof (ced->origline) - 1] = '\0';
    } 
    else
        ced->origline[0] = '\0';

    return TRUE;
}

/* varargs 'config_error_add()' moved to pline.c */
void
config_erradd(buf)
const char *buf;
{
    char lineno[QBUFSZ];

    if (!buf || !*buf)
        buf = "Unknown error";

    if (!config_error_data) {
        /* either very early, where pline() will use raw_print(), or
           player gave bad value when prompted by interactive 'O' command */
        pline("%s%s.", !iflags.window_inited ? "config_error_add: " : "", buf);
        wait_synch();
        return;
    }

    config_error_data->num_errors++;
    if (!config_error_data->origline_shown && !config_error_data->secure) {
        pline("\n%s", config_error_data->origline);
        config_error_data->origline_shown = TRUE;
    }
    if (config_error_data->line_num > 0 && !config_error_data->secure) {
        Sprintf(lineno, "Line %d: ", config_error_data->line_num);
    } else
        lineno[0] = '\0';

    pline("%s %s%s.", config_error_data->secure ? "Error:" : " *",
          lineno, buf);
}

int
config_error_done()
{
    int n;
    struct _config_error_frame *tmp = config_error_data;

    if (!config_error_data)
        return 0;
    n = config_error_data->num_errors;
    if (n) {
        pline("\n%d error%s in %s.\n", n,
                   (n > 1) ? "s" : "",
                   *config_error_data->source
              ? config_error_data->source : configfile);
        wait_synch();
    }
    config_error_data = tmp->next;
    free(tmp);
    return n;
}

boolean
read_config_file(filename, src)
const char *filename;
int src;
{
    FILE *fp;
    boolean rv = TRUE;

    if (!(fp = fopen_config_file(filename, src)))
        return FALSE;

    /* begin detection of duplicate configfile options */
    set_duplicate_opt_detection(1);
    free_config_sections();
    iflags.parse_config_file_src = src;

    rv = parse_conf_file(fp, parse_config_line);
    (void) fclose(fp);
    
    free_config_sections();
    /* turn off detection of duplicate configfile options */
    set_duplicate_opt_detection(0);
    return rv;
}

STATIC_OVL FILE *
fopen_wizkit_file()
{
    FILE *fp;
    char *envp;

    envp = nh_getenv("WIZKIT");
    if (envp && *envp)
        Strncpy(wizkit, envp, WIZKIT_MAX - 1);
    if (!wizkit[0])
        return (FILE *) 0;

#ifdef UNIX
    if (access(wizkit, 4) == -1) {
        /* 4 is R_OK on newer systems */
        /* nasty sneaky attempt to read file through
         * GnollHack's setuid permissions -- this is a
         * place a file name may be wholly under the player's
         * control
         */
        raw_printf("Access to %s denied (%d).", wizkit, errno);
        wait_synch();
        /* fall through to standard names */
    } else
#endif
        if ((fp = fopenp(wizkit, "r")) != (FILE *) 0) {
        return fp;
#if defined(UNIX) || defined(VMS)
    } else {
        /* access() above probably caught most problems for UNIX */
        raw_printf("Couldn't open requested config file %s (%d).", wizkit,
                   errno);
        wait_synch();
#endif
    }

#if defined(MICRO) || defined(MAC) || defined(__BEOS__) || defined(WIN32) || defined(ANDROID) || defined(GNH_MOBILE)
    if ((fp = fopenp(fqname(wizkit, CONFIGPREFIX, 0), "r")) != (FILE *) 0)
        return fp;
#else
    char tmp_wizkit[BUFSZ];
#ifdef VMS
    envp = nh_getenv("HOME");
    if (envp)
        Sprintf(tmp_wizkit, "%s%s", envp, wizkit);
    else
        Sprintf(tmp_wizkit, "%s%s", "sys$login:", wizkit);
    if ((fp = fopenp(tmp_wizkit, "r")) != (FILE *) 0)
        return fp;
#else /* should be only UNIX left */
    envp = nh_getenv("HOME");
    if (envp)
        Sprintf(tmp_wizkit, "%s/%s", envp, wizkit);
    else
        Strcpy(tmp_wizkit, wizkit);
    if ((fp = fopenp(tmp_wizkit, "r")) != (FILE *) 0)
        return fp;
    else if (errno != ENOENT) {
        /* e.g., problems when setuid GnollHack can't search home
         * directory restricted to user */
        raw_printf("Couldn't open default wizkit file %s (%d).", tmp_wizkit,
                   errno);
        wait_synch();
    }
#endif
#endif
    return (FILE *) 0;
}

/* add to hero's inventory if there's room, otherwise put item on floor */
STATIC_DCL void
wizkit_addinv(obj)
struct obj *obj;
{
    if (!obj || obj == &zeroobj)
        return;

    /* subset of starting inventory pre-ID */
    obj->dknown = 1;
    if (Role_if(PM_PRIEST))
        obj->bknown = 1;
    /* same criteria as lift_object()'s check for available inventory slot */
    if (obj->oclass != COIN_CLASS && inv_cnt(FALSE) >= 52
        && !merge_choice(invent, obj)) {
        /* inventory overflow; can't just place & stack object since
           hero isn't in position yet, so schedule for arrival later */
        add_to_migration(obj);
        obj->ox = 0; /* index of main dungeon */
        obj->oy = 1; /* starting level number */
        obj->owornmask =
            (int64_t) (MIGR_WITH_HERO | MIGR_NOBREAK | MIGR_NOSCATTER);
    } else {
        (void) addinv(obj);
    }
}


boolean
proc_wizkit_line(buf)
char *buf;
{
    struct obj *otmp;

    if (strlen(buf) >= INBUF_SIZ)
        buf[INBUF_SIZ - 1] = '\0';

    otmp = readobjnam(buf, (struct obj*) 0, TRUE, (boolean*)0);

    if (otmp) {
        if (otmp != &zeroobj)
            wizkit_addinv(otmp);
    } else {
        /* .60 limits output line width to 79 chars */
        config_error_add("Bad wizkit item: \"%.60s\"", buf);
        return FALSE;
    }
    return TRUE;
}

void
read_wizkit()
{
    FILE *fp;

    if (!wizard || !(fp = fopen_wizkit_file()))
        return;

    program_state.wizkit_wishing = 1;
    config_error_init(TRUE, "WIZKIT", FALSE);

    parse_conf_file(fp, proc_wizkit_line);
    (void) fclose(fp);

    config_error_done();
    program_state.wizkit_wishing = 0;

    return;
}

/* parse_conf_file
 *
 * Read from file fp, handling comments, empty lines, config sections,
 * CHOOSE, and line continuation, calling proc for every valid line.
 *
 * Continued lines are merged together with one space in between.
 */
STATIC_OVL boolean
parse_conf_file(fp, proc)
FILE *fp;
boolean FDECL((*proc), (char *));
{
    char inbuf[INBUF_SIZ];
    boolean rv = TRUE; /* assume successful parse */
    char *ep;
    boolean skip = FALSE, morelines = FALSE;
    char buf[INBUF_SIZ];
    Strcpy(buf, "");
    size_t inbufsz = sizeof inbuf;

    free_config_sections();

    while (fgets(inbuf, (int) inbufsz, fp)) 
    {
        if (inbuf[0] == '\r' && inbuf[1] == '\0')
            continue; /* Disregard all lines with just '\r', as this indicates that Windows and Unix text files have been mixed up */

        ep = index(inbuf, '\n');
        if (skip)
        { /* in case previous line was too long */
            if (ep)
                skip = FALSE; /* found newline; next line is normal */
        }
        else 
        {
            if (!ep)
            {  /* newline missing */
                if (strlen(inbuf) < (inbufsz - 2))
                {
                    /* likely the last line of file is just
                       missing a newline; process it anyway  */
                    ep = eos(inbuf);
                } 
                else 
                {
                    config_error_add("Line too long, skipping");
                    skip = TRUE; /* discard next fgets */
                }
            } 
            else
            {
                *ep = '\0'; /* remove newline */
            }

            if (ep) 
            {
                //size_t len;
                boolean ignoreline = FALSE;
                boolean oldline = FALSE;

                /* line continuation (trailing '\') */
                morelines = (--ep >= inbuf && *ep == '\\');
                if (morelines)
                    *ep = '\0';

                /* trim off spaces at end of line */
                while (--ep >= inbuf
                       && (*ep == ' ' || *ep == '\t' || *ep == '\r'))
                    *ep = '\0';

                if (!config_error_nextline(inbuf)) 
                {
                    rv = FALSE;
                    if (strcmp(buf, ""))
                        Strcpy(buf, "");
                    break;
                }

                ep = inbuf;
                while (*ep == ' ' || *ep == '\t' || *ep == '\r') ep++;

                /* lines beginning with '#' are comments. ignore empty lines. */
                if (!*ep || *ep == '#')
                    ignoreline = TRUE;

                if (strcmp(buf, ""))
                    oldline = TRUE;

                /* merge now read line with previous ones, if necessary */
                if (!ignoreline) 
                {
/*
                    len = strlen(inbuf) + 1;
                    if (buf)
                        len += strlen(buf);
                    tmpbuf = (char *) alloc(len);
                    if (buf) {
                        Sprintf(tmpbuf, "%s %s", buf, inbuf);
                        free(buf);
                    } else
                        Strcpy(tmpbuf, inbuf);
                    buf = tmpbuf;
                    if (strlen(buf) >= sizeof inbuf)
                        buf[sizeof inbuf - 1] = '\0';
*/

                    //len = strlen(ep) + 1; /* +1: final '\0' */
                    //if (buf)
                    //    len += strlen(buf) + 1; /* +1: space */
                    char tmpbuf[INBUF_SIZ]; // = (char*)alloc(sizeof inbuf);
                    *tmpbuf = '\0';
                    if (strcmp(buf, ""))
                    {
                        Strcat(strcpy(tmpbuf, buf), " ");
                    }
                    Strcpy(buf, strcat(tmpbuf, ep));
                    //free(tmpbuf);
                    if (strlen(buf) >= sizeof inbuf)
                        buf[sizeof inbuf - 1] = '\0';
                }

                if (morelines || (ignoreline && !oldline))
                    continue;

                if (handle_config_section(buf)) 
                { //ep
                    Strcpy(buf, "");
                    continue;
                }

                /* from here onwards, we'll handle buf only */

                if (match_varname(buf, "CHOOSE", 6)) 
                {
                    char *section;
                    char *bufp = find_optparam(buf);
                    if (!bufp)
                    {
                        config_error_add(
                                    "Format is CHOOSE=section1,section2,...");
                        rv = FALSE;
                        Strcpy(buf, "");
                        continue;
                    }
                    bufp++;

                    if (strcmp(config_section_chosen, ""))
                        Strcpy(config_section_chosen, ""); // free(config_section_chosen), config_section_chosen = 0;

                    section = choose_random_part(bufp, ',');

                    if (section)
                    {
                        Strcpy(config_section_chosen, section); // = dupstr(section);
                    } 
                    else
                    {
                        config_error_add("No config section to choose");
                        rv = FALSE;
                    }
                    Strcpy(buf, "");
                    continue;
                }

                if (!proc(buf))
                    rv = FALSE;

                Strcpy(buf, "");
            }
        }
    }

    free_config_sections();
    return rv;
}

extern struct symsetentry *symset_list;  /* options.c */
extern struct symparse loadsyms[];       /* drawing.c */
extern const char *known_handling[];     /* drawing.c */
extern const char *known_restrictions[]; /* drawing.c */
STATIC_VAR int symset_count = 0;             /* for pick-list building only */
STATIC_VAR boolean chosen_symset_start = FALSE, chosen_symset_end = FALSE;
STATIC_VAR int symset_which_set = 0;

STATIC_OVL
FILE *
fopen_sym_file()
{
    FILE *fp;

    fp = fopen_datafile(SYMBOLS, "r", HACKPREFIX);
    return fp;
}

/*
 * Returns 1 if the chose symset was found and loaded.
 *         0 if it wasn't found in the sym file or other problem.
 */
int
read_sym_file(which_set)
int which_set;
{
    FILE *fp = fopen_sym_file();
    if (!fp)
        return 0;

    symset_count = 0;
    chosen_symset_start = chosen_symset_end = FALSE;
    symset_which_set = which_set;

    config_error_init(TRUE, "symbols", FALSE);

    parse_conf_file(fp, proc_symset_line);
    (void) fclose(fp);

    if (!chosen_symset_start && !chosen_symset_end) {
        /* name caller put in symset[which_set].name was not found;
           if it looks like "Default symbols", null it out and return
           success to use the default; otherwise, return failure */
        if (symset[which_set].name
            && (fuzzymatch(symset[which_set].name, "Default symbols",
                           " -_", TRUE)
                || !strcmpi(symset[which_set].name, "default")))
            clear_symsetentry(which_set, TRUE);
        config_error_done();
        return (symset[which_set].name == 0) ? 1 : 0;
    }
    if (!chosen_symset_end)
        config_error_add("Missing finish for symset \"%s\"",
                         symset[which_set].name ? symset[which_set].name
                                                : "unknown");
    config_error_done();
    return 1;
}

boolean
proc_symset_line(buf)
char *buf;
{
    return !((boolean) parse_sym_line(buf, symset_which_set));
}

/* returns 0 on error */
int
parse_sym_line(buf, which_set)
char *buf;
int which_set;
{
    nhsym val;
    int i;
    struct symparse *symp = (struct symparse *) 0;
    char *bufp, *commentp, *altp;

    if (strlen(buf) >= INBUF_SIZ)
        buf[INBUF_SIZ - 1] = '\0';

    /* convert each instance of whitespace (tabs, consecutive spaces)
       into a single space; leading and trailing spaces are stripped */
    mungspaces(buf);

    /* remove trailing comment, if any (this isn't strictly needed for
       individual symbols, and it won't matter if "X#comment" without
       separating space slips through; for handling or set description,
       symbol set creator is responsible for preceding '#' with a space
       and that comment itself doesn't contain " #") */
    if ((commentp = rindex(buf, '#')) != 0 && commentp[-1] == ' ')
        commentp[-1] = '\0';

    /* find the '=' or ':' */
    bufp = index(buf, '=');
    altp = index(buf, ':');
    if (!bufp || (altp && altp < bufp))
        bufp = altp;
    if (!bufp) {
        if (strncmpi(buf, "finish", 6) == 0) {
            /* end current graphics set */
            if (chosen_symset_start)
                chosen_symset_end = TRUE;
            chosen_symset_start = FALSE;
            return 1;
        }
        config_error_add("No \"finish\"");
        return 0;
    }
    /* skip '=' and space which follows, if any */
    ++bufp;
    if (*bufp == ' ')
        ++bufp;

    symp = match_sym(buf);
    if (!symp) {
        config_error_add("Unknown sym keyword");
        return 0;
    }

    if (!symset[which_set].name) {
        /* A null symset name indicates that we're just
           building a pick-list of possible symset
           values from the file, so only do that */
        if (symp->range == SYM_CONTROL) {
            struct symsetentry *tmpsp;

            switch (symp->idx) {
            case 0:
                tmpsp = (struct symsetentry *) alloc(sizeof *tmpsp);
                tmpsp->next = symset_list;
                symset_list = tmpsp;
                tmpsp->idx = symset_count++;
                tmpsp->name = dupstr(bufp);
                tmpsp->desc = (char *) 0;
                tmpsp->handling = H_UNK;
                /* initialize restriction bits */
                tmpsp->nocolor = 0;
                tmpsp->primary = 0;
                tmpsp->rogue = 0;
                break;
            case 2:
                /* handler type identified */
                tmpsp = symset_list; /* most recent symset */
                for (i = 0; known_handling[i]; ++i)
                    if (!strcmpi(known_handling[i], bufp)) {
                        tmpsp->handling = i;
                        break; /* for loop */
                    }
                break;
            case 3:                  /* description:something */
                tmpsp = symset_list; /* most recent symset */
                if (tmpsp && !tmpsp->desc)
                    tmpsp->desc = dupstr(bufp);
                break;
            case 5:
                /* restrictions: xxxx*/
                tmpsp = symset_list; /* most recent symset */
                for (i = 0; known_restrictions[i]; ++i) {
                    if (!strcmpi(known_restrictions[i], bufp)) {
                        switch (i) {
                        case 0:
                            tmpsp->primary = 1;
                            break;
                        case 1:
                            tmpsp->rogue = 1;
                            break;
                        }
                        break; /* while loop */
                    }
                }
                break;
            }
        }
        return 1;
    }
    if (symp->range) {
        if (symp->range == SYM_CONTROL) {
            switch (symp->idx) {
            case 0:
                /* start of symset */
                if (!strcmpi(bufp, symset[which_set].name)) {
                    /* matches desired one */
                    chosen_symset_start = TRUE;
                    /* these init_*() functions clear symset fields too */
                    if (which_set == ROGUESET)
                        init_r_symbols();
                    else if (which_set == PRIMARY)
                        init_l_symbols();
                }
                break;
            case 1:
                /* finish symset */
                if (chosen_symset_start)
                    chosen_symset_end = TRUE;
                chosen_symset_start = FALSE;
                break;
            case 2:
                /* handler type identified */
                if (chosen_symset_start)
                    set_symhandling(bufp, which_set);
                break;
            /* case 3: (description) is ignored here */
            case 4: /* color:off */
                if (chosen_symset_start) {
                    if (bufp) {
                        if (!strcmpi(bufp, "true") || !strcmpi(bufp, "yes")
                            || !strcmpi(bufp, "on"))
                            symset[which_set].nocolor = 0;
                        else if (!strcmpi(bufp, "false")
                                 || !strcmpi(bufp, "no")
                                 || !strcmpi(bufp, "off"))
                            symset[which_set].nocolor = 1;
                    }
                }
                break;
            case 5: /* restrictions: xxxx*/
                if (chosen_symset_start) {
                    int n = 0;

                    while (known_restrictions[n]) {
                        if (!strcmpi(known_restrictions[n], bufp)) {
                            switch (n) {
                            case 0:
                                symset[which_set].primary = 1;
                                break;
                            case 1:
                                symset[which_set].rogue = 1;
                                break;
                            }
                            break; /* while loop */
                        }
                        n++;
                    }
                }
                break;
            }
        } else { /* !SYM_CONTROL */
            val = sym_val(bufp);
            if (chosen_symset_start) {
                if (which_set == PRIMARY) {
                    update_l_symset(symp, val);
                } else if (which_set == ROGUESET) {
                    update_r_symset(symp, val);
                }
            }
        }
    }
    return 1;
}

STATIC_OVL void
set_symhandling(handling, which_set)
char *handling;
int which_set;
{
    int i = 0;

    symset[which_set].handling = H_UNK;
    while (known_handling[i]) {
        if (!strcmpi(known_handling[i], handling)) {
            symset[which_set].handling = i;
            return;
        }
        i++;
    }
}

/* ----------  END CONFIG FILE HANDLING ----------- */

/* ----------  BEGIN SCOREBOARD CREATION ----------- */

#ifdef OS2_CODEVIEW
#define UNUSED_if_not_OS2_CODEVIEW /*empty*/
#else
#define UNUSED_if_not_OS2_CODEVIEW UNUSED
#endif

/* verify that we can write to scoreboard file; if not, try to create one */
/*ARGUSED*/
void
check_recordfile(dir)
const char *dir UNUSED_if_not_OS2_CODEVIEW;
{
#if defined(PRAGMA_UNUSED) && !defined(OS2_CODEVIEW)
#pragma unused(dir)
#endif
    const char *fq_record;
    int fd;

#if defined(UNIX) || defined(VMS)
    fq_record = fqname(RECORD, SCOREPREFIX, 0);
    fd = open(fq_record, O_RDWR, 0);
    if (fd >= 0) {
#ifdef VMS /* must be stream-lf to use UPDATE_RECORD_IN_PLACE */
        if (!file_is_stmlf(fd)) {
            raw_printf(
                   "Warning: scoreboard file '%s' is not in stream_lf format",
                       fq_record);
            wait_synch();
        }
#endif
        (void) nhclose(fd); /* RECORD is accessible */
    } else if ((fd = open(fq_record, O_CREAT | O_RDWR, FCMASK)) >= 0) {
        (void) nhclose(fd); /* RECORD newly created */
#if defined(VMS) && !defined(SECURE)
        /* Re-protect RECORD with world:read+write+execute+delete access. */
        (void) chmod(fq_record, FCMASK | 007);
#endif /* VMS && !SECURE */
    } else {
        raw_printf("Warning: cannot write scoreboard file '%s'", fq_record);
        wait_synch();
    }
#endif /* !UNIX && !VMS */
#if defined(MICRO) || defined(WIN32)
    char tmp[PATHLEN];

#ifdef OS2_CODEVIEW /* explicit path on opening for OS/2 */
    /* how does this work when there isn't an explicit path or fopenp
     * for later access to the file via fopen_datafile? ? */
    Strncpy(tmp, dir, PATHLEN - 1);
    tmp[PATHLEN - 1] = '\0';
    if ((strlen(tmp) + 1 + strlen(RECORD)) < (PATHLEN - 1)) {
        append_slash(tmp);
        Strcat(tmp, RECORD);
    }
    fq_record = tmp;
#else
    Strcpy(tmp, RECORD);
    fq_record = fqname(RECORD, SCOREPREFIX, 0);
#endif
#ifdef WIN32
    /* If dir is NULL it indicates create but
       only if it doesn't already exist */
    if (!dir) {
        char buf[BUFSZ];

        buf[0] = '\0';
        fd = open(fq_record, O_RDWR);
        if (!(fd == -1 && errno == ENOENT)) {
            if (fd >= 0) {
                (void) nhclose(fd);
            } else {
                /* explanation for failure other than missing file */
                Sprintf(buf, "error   \"%s\", (errno %d).",
                        fq_record, errno);
                paniclog("scorefile", buf);
            }
            return;
        }
        Sprintf(buf, "missing \"%s\", creating new scorefile.",
                fq_record);
        paniclog("scorefile", buf);
    }
#endif

    if ((fd = open(fq_record, O_RDWR)) < 0) {
        /* try to create empty 'record' */
#if defined(AZTEC_C) || defined(_DCC) \
    || (defined(__GNUC__) && defined(__AMIGA__))
        /* Aztec doesn't use the third argument */
        /* DICE doesn't like it */
        fd = open(fq_record, O_CREAT | O_RDWR);
#else
        fd = open(fq_record, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
#endif
        if (fd <= 0) {
            raw_printf("Warning: cannot write record '%s'", tmp);
            wait_synch();
        } else {
            (void) nhclose(fd);
        }
    } else {
        /* open succeeded => 'record' exists */
        (void) nhclose(fd);
    }
#else /* MICRO || WIN32*/

#ifdef MAC
    /* Create the "record" file, if necessary */
    fq_record = fqname(RECORD, SCOREPREFIX, 0);
    fd = macopen(fq_record, O_RDWR | O_CREAT, TEXT_TYPE);
    if (fd != -1)
        macclose(fd);
#endif /* MAC */

#endif /* MICRO || WIN32*/
}

int
check_current_fd(str)
const char* str;
{
#ifdef UNIX
    const char* fq_record = fqname(RECORD, SCOREPREFIX, 0);
    int fd = open(fq_record, O_RDWR, 0);
    if (str)
        issue_debuglog_fd(fd, str);
    if (fd != -1)
        (void)close(fd);
    return fd;
#else
    return -2;
#endif
}

/* ----------  END SCOREBOARD CREATION ----------- */

/* ----------  BEGIN PANIC/IMPOSSIBLE/TESTING LOG ----------- */

/*ARGSUSED*/
void
paniclog(type, reason)
const char *type;   /* panic, impossible, trickery */
const char *reason; /* explanation */
{
#ifdef PANICLOG
    FILE *lfile;

    if (!program_state.in_paniclog) {
        program_state.in_paniclog = 1;
        lfile = fopen_datafile(PANICLOG, "a", TROUBLEPREFIX);
        if (lfile) {
#ifdef PANICLOG_FMT2
            (void) fprintf(lfile, "%ld %s: %s %s\n",
                           ubirthday, (strcmp(plname, "") ? plname : "(none)"),
                           type, reason);
#else
            char buf[BUFSZ];
            time_t now = getnow();
            int uid = getuid();
            char playmode = wizard ? 'D' : discover ? 'X' : CasualMode ? (ModernMode ? 'C' : 'R') : ModernMode ? 'M' : '-';

            (void) fprintf(lfile, "%s %08lld %06lld %d %c: %s %s\n",
                           version_string(buf), (long long)yyyymmdd(now), (long long)hhmmss(now),
                           uid, playmode, type, reason);
#endif /* !PANICLOG_FMT2 */
            (void) fclose(lfile);
        }
        program_state.in_paniclog = 0;
    }
#endif /* PANICLOG */
    return;
}

void
testinglog(filenm, type, reason)
const char *filenm;   /* ad hoc file name */
const char *type;
const char *reason;   /* explanation */
{
    FILE *lfile;
    char fnbuf[BUFSZ];

    if (!filenm)
        return;
    Strcpy(fnbuf, filenm);
    if (index(fnbuf, '.') == 0)
        Strcat(fnbuf, ".log");
    lfile = fopen_datafile(fnbuf, "a", TROUBLEPREFIX);
    if (lfile) {
        (void) fprintf(lfile, "%s\n%s\n", type, reason);
        (void) fclose(lfile);
    }
    return;
}

/* ----------  END PANIC/IMPOSSIBLE/TESTING LOG ----------- */

#ifdef SELF_RECOVER

/* ----------  BEGIN INTERNAL RECOVER ----------- */
boolean
recover_savefile()
{
    int gfd, lfd, sfd;
    int lev, savelev, hpid;
    int pltmpsiz;
    xchar levc;
    struct version_info version_data;
    int processed[256];
    char savename[SAVESIZE], errbuf[BUFSZ];
    struct savefile_info sfi;
    struct save_game_stats gamestats;
    char tmpplbuf[PL_NSIZ];

    for (lev = 0; lev < 256; lev++)
        processed[lev] = 0;

    /* level 0 file contains:
     *  pid of creating process (ignored here)
     *  level number for current level of save file
     *  name of save file GnollHack would have created
     *  savefile info
     *  player name
     *  and game state
     */
    gfd = open_levelfile(0, errbuf);
    if (gfd < 0) {
        raw_printf("%s\n", errbuf);
        return FALSE;
    }
    if (read(gfd, (genericptr_t) &hpid, (readLenType)sizeof hpid) != sizeof hpid) {
        raw_printf("\n%s\n%s\n",
            "Checkpoint data incompletely written or subsequently clobbered.",
                   "Recovery impossible.");
        (void) nhclose(gfd);
        return FALSE;
    }
    if (read(gfd, (genericptr_t) &savelev, (readLenType)sizeof(savelev))
        != sizeof(savelev)) {
        raw_printf(
         "\nCheckpointing was not in effect for %s -- recovery impossible.\n",
                   lock);
        (void) nhclose(gfd);
        return FALSE;
    }
    if ((read(gfd, (genericptr_t) savename, (readLenType)sizeof savename)
         != sizeof savename)
        || (read(gfd, (genericptr_t) &version_data, (readLenType)sizeof version_data)
            != sizeof version_data)
        || (read(gfd, (genericptr_t) &sfi, (readLenType)sizeof sfi) != sizeof sfi)
        || (read(gfd, (genericptr_t) &pltmpsiz, (readLenType)sizeof pltmpsiz)
            != sizeof pltmpsiz) || (pltmpsiz > PL_NSIZ)
        || (read(gfd, (genericptr_t) &tmpplbuf, (readLenType)pltmpsiz) != pltmpsiz)
        || (read(gfd, (genericptr_t)&gamestats, (readLenType)sizeof gamestats) != sizeof gamestats)
        )
    {
        raw_printf("\nError reading %s -- can't recover.\n", lock);
        (void) nhclose(gfd);
        return FALSE;
    }

    /* Add number of recoveries by one and update gamestats time_stamp so it can be used for new save file tracking */
    gamestats.num_recoveries++;
    gamestats.time_stamp = (int64_t)getnow();

    /* save file should contain:
     *  version info
     *  savefile info
     *  player name
     *  save game stats
     *  current level (including pets)
     *  (non-level-based) game state
     *  other levels
     */
    set_savefile_name(TRUE);
    sfd = create_savefile();
    if (sfd < 0) {
        raw_printf("\nCannot recover savefile %s.\n", SAVEF);
        (void) nhclose(gfd);
        return FALSE;
    }

    lfd = open_levelfile(savelev, errbuf);
    if (lfd < 0) {
        raw_printf("\n%s\n", errbuf);
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        delete_savefile();
        return FALSE;
    }

    if (write(sfd, (genericptr_t) &version_data, sizeof version_data)
        != sizeof version_data) {
        raw_printf("\nError writing %s; recovery failed.", SAVEF);
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        (void) nhclose(lfd);
        delete_savefile();
        return FALSE;
    }

    if (write(sfd, (genericptr_t) &sfi, sizeof sfi) != sizeof sfi) {
        raw_printf("\nError writing %s; recovery failed (savefile_info).\n",
                   SAVEF);
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        (void) nhclose(lfd);
        delete_savefile();
        return FALSE;
    }

    if (write(sfd, (genericptr_t) &pltmpsiz, sizeof pltmpsiz)
        != sizeof pltmpsiz) {
        raw_printf("Error writing %s; recovery failed (player name size).\n",
                   SAVEF);
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        (void) nhclose(lfd);
        delete_savefile();
        return FALSE;
    }

    if (write(sfd, (genericptr_t) &tmpplbuf, pltmpsiz) != pltmpsiz) {
        raw_printf("Error writing %s; recovery failed (player name).\n",
                   SAVEF);
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        (void) nhclose(lfd);
        delete_savefile();
        return FALSE;
    }

    if (write(sfd, (genericptr_t)&gamestats, sizeof gamestats) != sizeof gamestats) {
        raw_printf("\nError writing %s; recovery failed (save_game_stats).\n",
            SAVEF);
        (void)nhclose(gfd);
        (void)nhclose(sfd);
        (void)nhclose(lfd);
        delete_savefile();
        return FALSE;
    }

    /* copy current level */
    if (!copy_bytes(lfd, sfd)) {
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        (void) nhclose(lfd);
        delete_savefile();
        return FALSE;
    }
    (void) nhclose(lfd);
    processed[savelev] = 1;

    /* copy game state */
    if (!copy_bytes(gfd, sfd)) {
        (void) nhclose(gfd);
        (void) nhclose(sfd);
        delete_savefile();
        return FALSE;
    }
    (void) nhclose(gfd);
    processed[0] = 1;

    for (lev = 1; lev < 256; lev++) {
        /* level numbers are kept in xchars in save.c, so the
         * maximum level number (for the endlevel) must be < 256
         */
        if (lev != savelev) {
            lfd = open_levelfile(lev, (char *) 0);
            if (lfd >= 0) {
                /* any or all of these may not exist */
                levc = (xchar) lev;
                (void)write(sfd, (genericptr_t) &levc, sizeof(levc));
                if (!copy_bytes(lfd, sfd)) {
                    (void) nhclose(lfd);
                    (void) nhclose(sfd);
                    delete_savefile();
                    return FALSE;
                }
                (void) nhclose(lfd);
                processed[lev] = 1;
            }
        }
    }
    (void) nhclose(sfd);

#ifdef HOLD_LOCKFILE_OPEN
    really_close();
#endif

    /*
     * We have a successful savefile!
     * Only now do we erase the level files.
     */
    for (lev = 0; lev < 256; lev++) {
        if (processed[lev]) {
            const char *fq_lock;

            set_levelfile_name(lock, lev);
            fq_lock = fqname(lock, LEVELPREFIX, 3);
            (void) unlink(fq_lock);
        }
    }

    const char* fq_save = fqname(SAVEF, SAVEPREFIX, 0);
    track_new_save_file(fq_save, gamestats.time_stamp);

#ifdef ANDROID
    /* if the new savefile isn't compressed
     * it will be overwritten when the old
     * savefile is restored in open_and_validate_saved_game(TRUE, (boolean*)0)
     */
    nh_compress(fq_save);
#endif

    return TRUE;
}

boolean
copy_bytes(ifd, ofd)
int ifd, ofd;
{
    char buf[BUFSIZ];
    int nfrom, nto;

    do {
        nfrom = (int)read(ifd, buf, BUFSIZ);
        nto = (int)write(ofd, buf, min(sizeof(buf)/sizeof(buf[0]), nfrom));
        if (nto != nfrom)
            return FALSE;
    } while (nfrom == BUFSIZ);
    return TRUE;
}

/* ----------  END INTERNAL RECOVER ----------- */
#endif /*SELF_RECOVER*/

/* ----------  OTHER ----------- */

#ifdef SYSCF
#ifdef SYSCF_FILE
void
assure_syscf_file()
{
    int fd;

    /*
     * All we really care about is the end result - can we read the file?
     * So just check that directly.
     *
     * Not tested on most of the old platforms (which don't attempt
     * to implement SYSCF).
     * Some ports don't like open()'s optional third argument;
     * VMS overrides open() usage with a macro which requires it.
     */
#ifndef VMS
# if defined(NOCWD_ASSUMPTIONS) && defined(WIN32)
    fd = open(fqname(SYSCF_FILE, SYSCONFPREFIX, 0), O_RDONLY);
# else
    fd = open(SYSCF_FILE, O_RDONLY);
# endif
#else
    fd = open(SYSCF_FILE, O_RDONLY, 0);
#endif
    if (fd >= 0) {
        /* readable */
        close(fd);
        return;
    }
    raw_printf("Unable to open SYSCF_FILE.\n");
    gnollhack_exit(EXIT_FAILURE);
}

#endif /* SYSCF_FILE */
#endif /* SYSCF */

#ifdef DEBUG
/* used by debugpline() to decide whether to issue a message
 * from a particular source file; caller passes __FILE__ and we check
 * whether it is in the source file list supplied by SYSCF's DEBUGFILES
 *
 * pass FALSE to override wildcard matching; useful for files
 * like dungeon.c and questpgr.c, which generate a ridiculous amount of
 * output if DEBUG is defined and effectively block the use of a wildcard */
boolean
debugcore(filename, wildcards)
const char *filename;
boolean wildcards;
{
    const char *debugfiles, *p;

    if (!filename || !*filename)
        return FALSE; /* sanity precaution */

    if (sysopt.env_dbgfl == 0) {
        /* check once for DEBUGFILES in the environment;
           if found, it supersedes the sysconf value
           [note: getenv() rather than nh_getenv() since a long value
           is valid and doesn't pose any sort of overflow risk here] */
        if ((p = getenv("DEBUGFILES")) != 0) {
            if (sysopt.debugfiles)
                free((genericptr_t) sysopt.debugfiles);
            sysopt.debugfiles = dupstr(p);
            sysopt.env_dbgfl = 1;
        } else
            sysopt.env_dbgfl = -1;
    }

    debugfiles = sysopt.debugfiles;
    /* usual case: sysopt.debugfiles will be empty */
    if (!debugfiles || !*debugfiles)
        return FALSE;

/* strip filename's path if present */
#ifdef UNIX
    if ((p = rindex(filename, '/')) != 0)
        filename = p + 1;
#endif
#ifdef VMS
    filename = vms_basename(filename);
    /* vms_basename strips off 'type' suffix as well as path and version;
       we want to put suffix back (".c" assumed); since it always returns
       a pointer to a static buffer, we can safely modify its result */
    Strcat((char *) filename, ".c");
#endif

    /*
     * Wildcard match will only work if there's a single pattern (which
     * might be a single file name without any wildcarding) rather than
     * a space-separated list.
     * [to NOT do: We could step through the space-separated list and
     * attempt a wildcard match against each element, but that would be
     * overkill for the intended usage.]
     */
    if (wildcards && pmatch(debugfiles, filename))
        return TRUE;

    /* check whether filename is an element of the list */
    if ((p = strstr(debugfiles, filename)) != 0) {
        int l = (int) strlen(filename);

        if ((p == debugfiles || p[-1] == ' ' || p[-1] == '/')
            && (p[l] == ' ' || p[l] == '\0'))
            return TRUE;
    }
    return FALSE;
}

#endif /*DEBUG*/

/* ----------  BEGIN TRIBUTE ----------- */

/* 3.6 tribute code
 */

#define SECTIONSCOPE 1
#define TITLESCOPE 2
#define PASSAGESCOPE 3

#define MAXPASSAGES SIZE(context.novel.pasg) /* 20 */

STATIC_DCL int FDECL(choose_passage, (int, unsigned));

/* choose a random passage that hasn't been chosen yet; once all have
   been chosen, reset the tracking to make all passages available again */
STATIC_OVL int
choose_passage(passagecnt, oid)
int passagecnt; /* total of available passages */
unsigned oid; /* book.o_id, used to determine whether re-reading same book */
{
    int idx, res;

    if (passagecnt < 1)
        return 0;

    /* if a different book or we've used up all the passages already,
       reset in order to have all 'passagecnt' passages available */
    if (oid != context.novel.id || context.novel.count == 0) {
        int i, range = passagecnt, limit = MAXPASSAGES;

        context.novel.id = oid;
        if (range <= limit) {
            /* collect all of the N indices */
            context.novel.count = passagecnt;
            for (idx = 0; idx < MAXPASSAGES; idx++)
                context.novel.pasg[idx] = (xchar) ((idx < passagecnt)
                                                   ? idx + 1 : 0);
        } else {
            /* collect MAXPASSAGES of the N indices */
            context.novel.count = MAXPASSAGES;
            for (idx = i = 0; i < passagecnt; ++i, --range)
                if (range > 0 && rn2(range) < limit) {
                    context.novel.pasg[idx++] = (xchar) (i + 1);
                    --limit;
                }
        }
    }

    idx = rn2(context.novel.count);
    res = (int) context.novel.pasg[idx];
    /* move the last slot's passage index into the slot just used
       and reduce the number of passages available */
    context.novel.pasg[idx] = context.novel.pasg[--context.novel.count];
    return res;
}

/* Returns True if you were able to read something. */
boolean
read_tribute(tribsection, tribtitle, tribpassage, nowin_buf, bufsz, oid)
const char *tribsection, *tribtitle;
int tribpassage, bufsz;
char *nowin_buf;
unsigned oid; /* book identifier */
{
    dlb *fp;
    char line[BUFSZ], lastline[BUFSZ];

    int scope = 0;
    //int linect = 0;
    int passagecnt = 0, targetpassage = 0;
    const char *badtranslation = "an incomprehensible foreign translation";
    boolean matchedsection = FALSE, matchedtitle = FALSE;
    winid tribwin = WIN_ERR;
    boolean grasped = FALSE;
    boolean foundpassage = FALSE;

    if (nowin_buf)
        *nowin_buf = '\0';

    /* check for mandatories */
    if (!tribsection || !tribtitle) 
    {
        if (!tribtitle)
        {
            pline1("This novel is all blank.");
            makeknown(SPE_NOVEL);
        }
        else if (!nowin_buf)
            pline("It's %s of \"%s\"!", badtranslation, tribtitle);
        return grasped;
    }

    debugpline3("read_tribute %s, %s, %d.", tribsection, tribtitle,
                tribpassage);

    fp = dlb_fopen(TRIBUTEFILE, "r");
    if (!fp) {
        /* this is actually an error - cannot open tribute file! */
        if (!nowin_buf)
            pline("You feel too overwhelmed to continue!");
        return grasped;
    }

    /*
     * Syntax (not case-sensitive):
     *  %section books
     *
     * In the books section:
     *    %title booktitle (n)
     *          where booktitle=book title without quotes
     *          (n)= total number of passages present for this title
     *    %passage k
     *          where k=sequential passage number
     *
     * %e ends the passage/book/section
     *    If in a passage, it marks the end of that passage.
     *    If in a book, it marks the end of that book.
     *    If in a section, it marks the end of that section.
     *
     *  %section death
     */

    *line = *lastline = '\0';
    while (dlb_fgets(line, sizeof line, fp) != 0) {
        //linect++;
        (void) strip_newline(line);
        switch (line[0]) {
        case '%':
            if (!strncmpi(&line[1], "section ", sizeof "section " - 1)) {
                char *st = &line[9]; /* 9 from "%section " */

                scope = SECTIONSCOPE;
                matchedsection = !strcmpi(st, tribsection) ? TRUE : FALSE;
            } else if (!strncmpi(&line[1], "title ", sizeof "title " - 1)) {
                char *st = &line[7]; /* 7 from "%title " */
                char *p1, *p2;

                if ((p1 = index(st, '(')) != 0) {
                    *p1++ = '\0';
                    (void) mungspaces(st);
                    if ((p2 = index(p1, ')')) != 0) {
                        *p2 = '\0';
                        passagecnt = atoi(p1);
                        scope = TITLESCOPE;
                        if (matchedsection && !strcmpi(st, tribtitle)) {
                            matchedtitle = TRUE;
                            targetpassage = !tribpassage
                                             ? choose_passage(passagecnt, oid)
                                             : (tribpassage <= passagecnt)
                                                ? tribpassage : 0;
                        } else {
                            matchedtitle = FALSE;
                        }
                    }
                }
            } else if (!strncmpi(&line[1], "passage ",
                                 sizeof "passage " - 1)) {
                int passagenum = 0;
                char *st = &line[9]; /* 9 from "%passage " */

                mungspaces(st);
                passagenum = atoi(st);
                if (passagenum > 0 && passagenum <= passagecnt) {
                    scope = PASSAGESCOPE;
                    if (matchedtitle && passagenum == targetpassage) {
                        foundpassage = TRUE;
                        if (!nowin_buf) {
                            tribwin = create_nhwindow(NHW_MENU);
                            if (tribwin == WIN_ERR)
                                goto cleanup;
                        }
                    }
                }
            } else if (!strncmpi(&line[1], "e ", sizeof "e " - 1)) {
                if (foundpassage)
                    goto cleanup;
                if (scope == TITLESCOPE)
                    matchedtitle = FALSE;
                if (scope == SECTIONSCOPE)
                    matchedsection = FALSE;
                if (scope)
                    --scope;
            } else {
                //debugpline1("tribute file error: bad %% command, line %d.",
                //            linect);
            }
            break;
        case '#':
            /* comment only, next! */
            break;
        default:
            if (foundpassage) {
                if (!nowin_buf) {
                    /* outputting multi-line passage to text window */
                    putstr(tribwin, 0, line);
                    if (*line)
                        Strcpy(lastline, line);
                } else {
                    /* fetching one-line passage into buffer */
                    copynchars(nowin_buf, line, bufsz - 1);
                    goto cleanup; /* don't wait for "%e passage" */
                }
            }
        }
    }

cleanup:
    (void) dlb_fclose(fp);
    if (nowin_buf) {
        /* one-line buffer */
        grasped = *nowin_buf ? TRUE : FALSE;
    } else {
        if (tribwin != WIN_ERR) { /* implies 'foundpassage' */
            /* multi-line window, normal case;
               if lastline is empty, there were no non-empty lines between
               "%passage n" and "%e passage" so we leave 'grasped' False */
            if (*lastline) {
                display_nhwindow(tribwin, FALSE);
                /* put the final attribution line into message history,
                   analogous to the summary line from long quest messages */
                if (index(lastline, '['))
                    mungspaces(lastline); /* to remove leading spaces */
                else /* construct one if necessary */
                    Sprintf(lastline, "[%s, by Terry Pratchett]", tribtitle);
                putmsghistory(lastline, FALSE);
                grasped = TRUE;
            }
            destroy_nhwindow(tribwin);
        }
        if (!grasped)
            /* multi-line window, problem */
            pline("It seems to be %s of \"%s\"!", badtranslation, tribtitle);
    }
    return grasped;
}

boolean
Death_quote(buf, bufsz)
char *buf;
int bufsz;
{
    unsigned death_oid = 1; /* chance of oid #1 being a novel is negligible */

    return read_tribute("Death", "Death Quotes", 0, buf, bufsz, death_oid);
}

/* ----------  END TRIBUTE ----------- */

void
list_files()
{
#if defined(UNIX) && (defined(QT_GRAPHICS) || defined(ANDROID) || defined(GNH_MOBILE))
    char fileslist[BUFSIZ * 5] = "";
    char savelist[BUFSIZ * 5] = "";

    DIR* d;
    struct dirent* dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char* endptr = eos(fileslist);
            if (endptr + strlen(dir->d_name) - fileslist >= BUFSIZ * 5)
                break;
            Sprintf(endptr, "%s\n", dir->d_name);
        }
        closedir(d);
    }
    d = opendir("./save");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char* endptr = eos(savelist);
            if (endptr + strlen(dir->d_name) - savelist >= BUFSIZ * 5)
                break;
            Sprintf(endptr, "%s\n", dir->d_name);
        }
        closedir(d);
    }
#endif
    return;
}

void
reset_files(VOID_ARGS)
{
    nesting = 0;
    config_error_data = 0;
#if defined(WIN32) || defined(MSDOS)
    lockptr = 0;
#endif
#ifdef AMIGA
    lockptr = 0;
#endif
    *config_section_chosen = 0;
    *config_section_current = 0;
    *fq_tmp_backup = 0;
}


/* Live logging - taken directly from 3.4.3-nao code base,
 * but now uses \t separator instead of : as per xlogfile
 */
 /* Locks the live log file and writes 'buffer'
  * IF the ll_type matches sysopt.livelog mask
  * lltype is included in LL entry for post-process filtering also
  */
#if defined LIVELOGFILE
STATIC_OVL void
livelog_write_string(ll_type, buffer)
unsigned int ll_type;
const char* buffer;
{
#define LLOG_SEP '\t' /* livelog field separator */
    FILE* livelogfile;

    if (!(ll_type & sysopt.livelog)) return;
    if ((ll_type == LL_CONDUCT) && (moves < sysopt.ll_conduct_turns)) return;
    if (lock_file(LIVELOGFILE, SCOREPREFIX, 10)) {
        if (!(livelogfile = fopen_datafile(LIVELOGFILE, "a", SCOREPREFIX))) {
            pline("Cannot open live log file!");
        }
        else {
            char tmpbuf[1024 + 1];
            char msgbuf[512 + 1];
            char* c1 = msgbuf;
            Strncpy(msgbuf, buffer, 512);
            msgbuf[512] = '\0';
            while (*c1 != '\0') {
                if (*c1 == LLOG_SEP) *c1 = '_';
                c1++;
            }
            snprintf(tmpbuf, 1024, "lltype=%d%cplayer=%s%crole=%s%crace=%s%cgender=%s%calign=%s%cturns=%lld%crealtime=%lld%cstarttime=%lld%ccurtime=%lld%cmessage=%s\n",
                (ll_type & sysopt.livelog),
                LLOG_SEP,
                plname,
                LLOG_SEP,
                urole.filecode,
                LLOG_SEP,
                urace.filecode,
                LLOG_SEP,
                genders[Ufemale].filecode,
                LLOG_SEP,
                aligns[u.ualign.type == A_NONE ? 3 : 1 - u.ualign.type].filecode,
                LLOG_SEP,
                (long long)moves,
                LLOG_SEP,
                (long long)(urealtime.realtime + ((int64_t)getnow() - urealtime.start_timing)), LLOG_SEP,
                (long long)ubirthday,
                LLOG_SEP,
                (long long)time(NULL),
                LLOG_SEP,
                msgbuf);

            fprintf(livelogfile, "%s", tmpbuf);
            (void)fclose(livelogfile);
        }
        unlock_file(LIVELOGFILE);
    }
#undef LLOG_SEP
}

#else /* LIVELOGFILE */

STATIC_OVL void
livelog_write_string(log_type, buffer)
unsigned int log_type UNUSED;
const char* buffer UNUSED;
{
}
#endif /* LIVELOGFILE */

void
gamelog_add(glflags, gltime, str)
int64_t glflags, gltime;
const char* str;
{
    struct gamelog_line* tmp;
    struct gamelog_line* lst = gamelog;

    tmp = (struct gamelog_line*)alloc(sizeof(struct gamelog_line));
    tmp->turn = gltime;
    tmp->flags = glflags;
    tmp->text = dupstr(str);
    tmp->next = NULL;
    while (lst && lst->next)
        lst = lst->next;
    if (!lst)
        gamelog = tmp;
    else
        lst->next = tmp;
}

void
reset_gamelog(VOID_ARGS)
{
    struct gamelog_line* next;
    while (gamelog)
    {
        if (gamelog->text)
        {
            free((genericptr_t)gamelog->text);
            gamelog->text = 0;
        }
        next = gamelog->next;
        free((genericptr_t)gamelog);
        gamelog = next;
    }
}

STATIC_OVL void
livelog_post_to_forum(ll_type, str)
unsigned int ll_type;
const char* str;
{
    struct u_realtime used_realtime = urealtime;
    livelog_post_to_forum_rt(ll_type, used_realtime, str);
}

STATIC_OVL void
livelog_post_to_forum_rt(ll_type, used_realtime, str)
unsigned int ll_type;
struct u_realtime used_realtime;
const char* str;
{
    if (!str)
        return;

    if (ll_type & LL_postables)
    {
        IfModeAllowsPostToForum
        {
            char postbuf[BUFSZ * 2];
            char mbuf[BUFSZ] = "";
            char cbuf[BUFSZ];
            (void)describe_mode(mbuf);
            Sprintf(cbuf, "%.3s %.3s %.3s %.3s XL:%d", urole.filecode,
                urace.filecode, genders[Ufemale].filecode,
                aligns[1 - u.ualign.type].filecode, u.ulevel);
            int64_t currenttime = calculate_current_game_duration(used_realtime);
            char* duration = format_duration_with_units(currenttime);
            Sprintf(postbuf, "%s (%s) %s, on T:%lld (%s) [%s]", plname, cbuf, str, (long long)moves, duration, mbuf);

            int post_type;
            if (ll_type & (LL_GAME_START | LL_GAME_RESTORE | LL_GAME_SAVE))
            {
                post_type = GAME_STATUS_POST_IF_CLOUD_REPLAY_ON;
                if (ll_type & (LL_GAME_RESTORE | LL_GAME_SAVE))
                {
                    Sprintf(eos(postbuf), " [%llx]", (unsigned long long)used_realtime.finish_time);
                }
            }
            else
                post_type = GAME_STATUS_GENERAL_POST;

            issue_gui_command(GUI_CMD_POST_GAME_STATUS, post_type, (int)ll_type, postbuf);
        }
    }
}

/* #chronicle details */
void
show_gamelog(final)
int final;
{
    struct gamelog_line* llmsg;
    winid win;
    char buf[BUFSZ];
    int eventcnt = 0;
    int eventidx = 0;

    win = create_nhwindow(NHW_TEXT);
    Sprintf(buf, "%s events:", final ? "Major" : "Logged");
    putstr(win, ATR_TITLE, buf);
    for (llmsg = gamelog; llmsg; llmsg = llmsg->next) {
        if (final && !majorevent(llmsg))
            continue;
        if (!final && !wizard && spoilerevent(llmsg))
            continue;

        eventcnt++;
    }
    for (llmsg = gamelog; llmsg; llmsg = llmsg->next) {
        if (final && !majorevent(llmsg))
            continue;
        if (!final && !wizard && spoilerevent(llmsg))
            continue;
        if (!eventidx++)
            putstr(win, ATR_START_TABLE | ATR_TABLE_HEADER, " Turn  ");
        Sprintf(buf, "%5lld: %s", (long long)llmsg->turn, llmsg->text);
        putstr(win, ATR_INDENT_AT_COLON | ATR_TABLE_ROW | (eventidx == 1 ? ATR_START_TABLE_BODY : 0) | (eventidx == eventcnt ? (ATR_END_TABLE_BODY | ATR_END_TABLE) : 0), buf);
    }
    /* since start of game is logged as a major event, 'eventcnt' should
       never end up as 0; for 'final', end of game is a major event too */
    if (!eventcnt)
        putstr(win, ATR_NONE, " none");

    display_nhwindow(win, TRUE);
    destroy_nhwindow(win);
    return;
}

void
livelog_printf
VA_DECL2(unsigned int, ll_type, const char*, fmt)
{
    char ll_msgbuf[512];
    VA_START(fmt);
    VA_INIT(fmt, char*);
    vsnprintf(ll_msgbuf, 512, fmt, VA_ARGS);
    gamelog_add((int64_t)ll_type, moves, ll_msgbuf);
    livelog_write_string(ll_type, ll_msgbuf);
    livelog_post_to_forum(ll_type, ll_msgbuf);
    VA_END();
}

void
post_to_forum_printf
VA_DECL2(unsigned int, ll_type, const char*, fmt)
{
    char ll_msgbuf[512];
    VA_START(fmt);
    VA_INIT(fmt, char*);
    vsnprintf(ll_msgbuf, 512, fmt, VA_ARGS);
    livelog_post_to_forum(ll_type, ll_msgbuf);
    VA_END();
}

void
post_to_forum_rt_printf
VA_DECL3(unsigned int, ll_type, struct u_realtime, used_realtime, const char*, fmt)
{
    char ll_msgbuf[512];
    VA_START(fmt);
    VA_INIT(fmt, char*);
    vsnprintf(ll_msgbuf, 512, fmt, VA_ARGS);
    livelog_post_to_forum_rt(ll_type, used_realtime, ll_msgbuf);
    VA_END();
}

#ifdef EXTRAINFO_FN
/* This probably belongs in files.c, but it
 * uses dump_fmtstr() which is static here.
 */

char*
extrainfo_fmtstr(fmt, buf)
const char* fmt;
char* buf;
{
    const char* fp = fmt;
    char* bp = buf;
    size_t slen, len = 0;
    char tmpbuf[BUFSZ];
    char verbuf[BUFSZ];
    int64_t uid;
    time_t now = getnow();

    uid = (int64_t)getuid();

    /*
     * Note: %t and %T assume that time_t is a 'long int' number of
     * seconds since some epoch value.  That's quite iffy....  The
     * unit of time might be different and the datum size might be
     * some variant of 'long long int'.  [Their main purpose is to
     * construct a unique file name rather than record the date and
     * time; violating the 'long seconds since base-date' assumption
     * may or may not interfere with that usage.]
     */

    while (fp && *fp && len < BUFSZ - 1) {
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
                Sprintf(tmpbuf, "%llu", (unsigned long long)ubirthday);
                break;
            case 'T': /* current time, timestamp */
                Sprintf(tmpbuf, "%llu", (unsigned long long)now);
                break;
            case 'd': /* game start, YYYYMMDDhhmmss */
                Sprintf(tmpbuf, "%08lld%06lld",
                    (long long)yyyymmdd(ubirthday), (long long)hhmmss(ubirthday));
                break;
            case 'D': /* current time, YYYYMMDDhhmmss */
                Sprintf(tmpbuf, "%08lld%06lld", (long long)yyyymmdd(now), (long long)hhmmss(now));
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
            if (len + slen < BUFSZ - 1) {
                len += slen;
                Sprintf(bp, "%s", tmpbuf);
                bp += slen;
                if (*fp) fp++;
            }
            else
                break;
        }
        else {
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


void
mk_dgl_extrainfo()
{
    FILE* extrai = (FILE*)0;
#ifdef UNIX
    mode_t eimode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
#endif
    char new_fn[512];

    extrainfo_fmtstr(EXTRAINFO_FN, new_fn);

    extrai = fopen(new_fn, "w");
    if (!extrai) {
    }
    else {
        int sortval = 0;
        char tmpdng[16];
        sortval += (u.uhave.amulet ? 1024 : 0);
        if (Is_knox(&u.uz)) {
            Sprintf(tmpdng, "%s", "Knx");
            sortval += 245;
        }
        else if (In_quest(&u.uz)) {
            Sprintf(tmpdng, "%s%i", "Q", dunlev(&u.uz));
            sortval += 250 + (dunlev(&u.uz));
        }
        else if (In_endgame(&u.uz)) {
            Sprintf(tmpdng, "%s", "End");
            sortval += 256;
        }
        else if (In_V_tower(&u.uz)) {
            Sprintf(tmpdng, "T%i", dunlev(&u.uz));
            sortval += 235 + (depth(&u.uz));
        }
        else if (In_sokoban(&u.uz)) {
            Sprintf(tmpdng, "S%i", dunlev(&u.uz));
            sortval += 225 + (depth(&u.uz));
        }
        else if (In_mines(&u.uz)) {
            Sprintf(tmpdng, "M%i", dunlev(&u.uz));
            sortval += 215 + (dunlev(&u.uz));
        }
        else {
            Sprintf(tmpdng, "D%i", depth(&u.uz));
            sortval += (depth(&u.uz));
        }
#ifdef UNIX
        chmod(new_fn, eimode);
#endif
        fprintf(extrai, "%i|%c %s", sortval, (u.uhave.amulet ? 'A' : ' '), tmpdng);
        fclose(extrai);
    }
}
#endif /* EXTRAINFO_FN */

void
livelog_dump_url(llflags)
unsigned int llflags;
{
#if defined (DUMPLOG)
    char buf[BUFSZ];
    char* dumpurl;

#ifdef SYSCF
    if (!sysopt.dumplogurl)
        return;
    dumpurl = dump_fmtstr(sysopt.dumplogurl, buf);
#else
    dumpurl = dump_fmtstr(DUMPLOG_URL, buf);
#endif
    livelog_write_string(llflags, dumpurl);
#else
    nhUse(llflags);
#endif /*?DUMPLOG*/
}


#ifdef WISH_TRACKER
const char* wish_tracker_file =
#ifdef UNIX
"wishtracker";
#else
#if defined(MAC) || defined(__BEOS__)
"Wish Tracker";
#else
"wishes.txt";
#endif
#endif /* WISH_TRACKER */

void
trackwish(wishstring)
char* wishstring;
{
    char bigbuf[512];
    FILE* fp;
    time_t now = getnow();

    fp = fopen_datafile(wish_tracker_file, "a+", LEVELPREFIX);
    if (fp) {
        Sprintf(bigbuf, "%s wished for %s (%ld%s wish, on T:%lld on %08lld at %06lld hrs)\n",
            plname, wishstring, u.uconduct.wishes,
            u.uconduct.wishes == 1 ? "st" : u.uconduct.wishes == 2 ? "nd" :
            u.uconduct.wishes == 3 ? "rd" : "th", (long long)moves, (long long)yyyymmdd(now), (long long)hhmmss(now));
        fwrite(bigbuf, strlen(bigbuf), 1, fp);
        fclose(fp);
    }
}
#endif

#ifdef WHEREIS_FILE
/** Set the filename for the whereis file. */
void
set_whereisfile()
{
    char* p = (char*)strstr(whereis_file, "%n");

    if (p) {
        int new_whereis_len = strlen(whereis_file) + strlen(plname) - 2; /* %n */
        char* new_whereis_fn = (char*)alloc((unsigned)(new_whereis_len + 1));
        char* q = new_whereis_fn;

        Strncpy(q, whereis_file, p - whereis_file);
        q += p - whereis_file;
        Strncpy(q, plname, strlen(plname) + 1);
        regularize(q);
        q[strlen(plname)] = '\0';
        q += strlen(q);
        p += 2;   /* skip "%n" */
        Strncpy(q, p, strlen(p));
        new_whereis_fn[new_whereis_len] = '\0';
        Sprintf(whereis_file, "%s", new_whereis_fn);
        free(new_whereis_fn); /* clean up the pointer */
    }
}

/** Write out information about current game to plname.whereis. */
void
write_whereis(playing)
boolean playing; /**< True if game is running.  */
{
    FILE* fp;
    char whereis_work[511];

    if (strstr(whereis_file, "%n"))
        set_whereisfile();
    Sprintf(whereis_work,
        "player=%s:depth=%d:dnum=%d:dname=%s:hp=%d:maxhp=%d:turns=%ld:score=%ld:role=%s:race=%s:gender=%s:align=%s:conduct=0x%lx:amulet=%d:ascended=%d:playing=%d\n",
        plname,
        depth(&u.uz),
        u.uz.dnum,
        dungeons[u.uz.dnum].dname,
        u.uhp,
        u.uhpmax,
        moves,
        botl_score(),
        urole.filecode,
        urace.filecode,
        genders[Ufemale].filecode,
        aligns[u.ualign.type == A_NONE ? 3 : 1 - u.ualign.type].filecode,
        encodeconduct(),
        u.uhave.amulet ? 1 : 0,
        u.uevent.ascended ? 2 : *killer.name ? 1 : 0,
        playing);

    fp = fopen_datafile(whereis_file, "w", LEVELPREFIX);
    if (fp) {
#ifdef UNIX
        mode_t whereismode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
        chmod(fqname(whereis_file, LEVELPREFIX, 2), whereismode);
#endif
        fwrite(whereis_work, strlen(whereis_work), 1, fp);
        fclose(fp);
    }
    else {
        pline("Can't open %s for output.", whereis_file);
        pline("No whereis file created.");
    }
}

/** Signal handler to update whereis information. */
void
signal_whereis(sig_unused)
int sig_unused UNUSED;
{
    touch_whereis();
}

void
touch_whereis()
{
    write_whereis(TRUE);
}

void
delete_whereis()
{
    write_whereis(FALSE);
}
#endif /* WHEREIS_FILE */

STATIC_OVL
int copy_savefile(to, from)
const char* to, *from;
{
    int fd_to, fd_from;
    char buf[4096];
    int nread;
    int saved_errno;

#ifdef MAC
    fd_from = macopen(from, O_RDONLY | O_BINARY, SAVE_TYPE);
#else
    fd_from = open(from, O_RDONLY | O_BINARY, 0);
#endif
    if (fd_from < 0)
        return -1;

#if defined(MICRO) || defined(WIN32)
    fd_to = open(to, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, FCMASK);
#else
#ifdef MAC
    fd_to = maccreat(to, SAVE_TYPE);
#else
    fd_to = creat(to, FCMASK);
#endif
#endif /* MICRO */

    if (fd_to < 0)
        goto out_error;

    while (nread = (int)read(fd_from, buf, (readLenType)(sizeof buf)), nread > 0)
    {
        char* out_ptr = buf;
        int nwritten;

        do {
            nwritten = (int)write(fd_to, out_ptr, (unsigned)nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        (void)close(fd_from);

        /* Success! */
        return 0;
    }

out_error:
    saved_errno = errno;

    (void)close(fd_from);
    if (fd_to >= 0)
        (void)close(fd_to);

    errno = saved_errno;
    return -1;
}

void
make_dumplog_dir(VOID_ARGS)
{
#if (defined(DUMPLOG) || defined(DUMPHTML))
#if defined(DUMPLOG_DIR)
    /* Make DUMPLOG_DIR if defined */
    struct stat st = { 0 };

    if (stat(DUMPLOG_DIR, &st) == -1)
    {
#if WIN32
        (void)mkdir(DUMPLOG_DIR);
#else
        (void)mkdir(DUMPLOG_DIR, 0700);
#endif
    }
#endif
#if defined(SNAPSHOT_DIR)
    /* Make SNAPSHOT_DIR if defined */
    struct stat st2 = { 0 };

    if (stat(SNAPSHOT_DIR, &st2) == -1)
    {
#if WIN32
        (void)mkdir(SNAPSHOT_DIR);
#else
        (void)mkdir(SNAPSHOT_DIR, 0700);
#endif
    }
#endif
#endif
}


/*files.c*/

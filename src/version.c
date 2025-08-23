/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    version.c    $NHDT-Date: 1552353060 2019/03/12 01:11:00 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.52 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "dlb.h"
#if defined(GNH_MOBILE) && !defined(GNH_WIN)
#include "date_unix.h"
#else
#include "date.h"
#endif

/*
 * All the references to the contents of patchlevel.h have been moved
 * into makedefs....
 */
#ifdef SHORT_FILENAMES
#include "patchlev.h"
#else
#include "patchlevel.h"
#endif

#if defined(GnollHack_GIT_SHA)
const char *GnollHack_git_sha = GnollHack_GIT_SHA;
#endif
#if defined(GnollHack_GIT_BRANCH)
const char *GnollHack_git_branch = GnollHack_GIT_BRANCH;
#endif

STATIC_DCL void FDECL(insert_rtoption, (char *));

/* fill buffer with short version (so caller can avoid including date.h) */
char *
version_string(buf)
char *buf;
{
    return strcpy(buf, VERSION_STRING);
}

uint64_t
get_version_number(VOID_ARGS)
{
    return VERSION_NUMBER;
}

uint64_t
get_version_compatibility(VOID_ARGS)
{
    return EARLIEST_COMPATIBLE_VERSION_NUMBER;
}

/* fill and return the given buffer with the long GnollHack version string */
char *
getversionstring(buf)
char *buf;
{
    Strcpy(buf, VERSION_ID);

#if defined(RUNTIME_PORT_ID) \
    || defined(GnollHack_GIT_SHA) || defined(GnollHack_GIT_BRANCH)
    {
        int c = 0;
#if defined(RUNTIME_PORT_ID)
        char tmpbuf[BUFSZ], *tmp;
#endif
        char *p = eos(buf);
        boolean dotoff = (p > buf && p[-1] == '.');

        if (dotoff)
            --p;
        Strcpy(p, " (");
#if defined(RUNTIME_PORT_ID)
        tmp = get_port_id(tmpbuf);
        if (tmp)
            Sprintf(eos(buf), "%s%s", c++ ? "," : "", tmp);
#endif
#if defined(GnollHack_GIT_SHA)
        if (GnollHack_git_sha)
            Sprintf(eos(buf), "%s%s", c++ ? "," : "", GnollHack_git_sha);
#endif
#if defined(GnollHack_GIT_BRANCH)
#if defined(BETA)
        if (GnollHack_git_branch)
            Sprintf(eos(buf), "%sbranch:%s",
                    c++ ? "," : "", GnollHack_git_branch);
#endif
#endif
        if (c)
            Strcat(buf, ")");
        else /* if nothing has been added, strip " (" back off */
            *p = '\0';
        if (dotoff)
            Strcat(buf, ".");
    }
#endif /* RUNTIME_PORT_ID || GnollHack_GIT_SHA || GnollHack_GIT_BRANCH */

    return buf;
}

/* the 'v' command */
int
doversion()
{
    char buf[BUFSZ];

    pline("%s", getversionstring(buf));
    return 0;
}

/* the '#version' command; also a choice for '?' */
int
doextversion()
{
    dlb *f;
    char buf[BUFSZ], *p = 0;
    winid win = create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_DISPLAY_FILE_WITH_INDENTED_TEXT, NO_GLYPH, zerocreatewindowinfo);

    /* instead of using ``display_file(OPTIONS_USED,TRUE)'' we handle
       the file manually so we can include dynamic version info */

    (void) getversionstring(buf);
    /* if extra text (git info) is present, put it on separate line, unless it is (Hot Fix) */

    if (strlen(buf) >= COLNO)
        p = rindex(buf, '(');
    if(p && p > buf && p[1] == 'H') /* First one might be (Hot Fix) */
        p = rindex(p + 1, '(');
    if (p && p > buf && p[-1] == ' ' && p[1] != 'H')
        p[-1] = '\0';
    else
        p = 0;

    putstr(win, ATR_TITLE, buf);
    if (p) {
        *--p = ' ';
        putstr(win, ATR_SUBTITLE, p);
    }

    f = dlb_fopen(OPTIONS_USED, "r");
    if (!f) {
        putstr(win, 0, "");
        Sprintf(buf, "[Configuration '%s' not available?]", OPTIONS_USED);
        putstr(win, 0, buf);
    } else {
        /*
         * already inserted above:
         * + outdented program name and version plus build date and time
         * dat/options; display contents with lines prefixed by '-' deleted:
         * - blank-line
         * -     indented program name and version
         *   blank-line
         *   outdented feature header
         * - blank-line
         *       indented feature list
         *       spread over multiple lines
         *   blank-line
         *   outdented windowing header
         * - blank-line
         *       indented windowing choices with
         *       optional second line for default
         * - blank-line
         * - EOF
         */
        boolean prolog = TRUE; /* to skip indented program name */

        while (dlb_fgets(buf, BUFSZ, f)) {
            (void) strip_newline(buf);
            if (index(buf, '\t') != 0)
                (void) tabexpand(buf);

            if (*buf && *buf != ' ') {
                /* found outdented header; insert a separator since we'll
                   have skipped corresponding blank line inside the file */
                putstr(win, 0, "");
                prolog = FALSE;
            }
            /* skip blank lines and prolog (progame name plus version) */
            if (prolog || !*buf)
                continue;

            if (index(buf, ':'))
                insert_rtoption(buf);

            if (*buf)
                putstr(win, 0, buf);
        }
        (void) dlb_fclose(f);
        display_nhwindow(win, FALSE);
        destroy_nhwindow(win);
    }
    return 0;
}

void early_version_info(pastebuf)
boolean pastebuf;
{
    char buf[BUFSZ], buf2[BUFSZ];
    char *tmp = getversionstring(buf);

    /* this is early enough that we have to do
       our own line-splitting */
    if (tmp) {
        tmp = strstri(buf," (");
        if (tmp) *tmp++ = '\0';
    }

    Sprintf(buf2, "%s\n", buf);
    if (tmp) Sprintf(eos(buf2), "%s\n", tmp);
    raw_printf("%s", buf2);

    if (pastebuf) {
#ifdef RUNTIME_PASTEBUF_SUPPORT
        /*
         * Call a platform/port-specific routine to insert the
         * version information into a paste buffer. Useful for
         * easy inclusion in bug reports.
         */
        port_insert_pastebuf(buf2);
#else
        raw_printf("%s", "Paste buffer copy is not available.\n");
#endif
    }
}

extern const char regex_id[];

/*
 * makedefs should put the first token into dat/options; we'll substitute
 * the second value for it.  The token must contain at least one colon
 * so that we can spot it, and should not contain spaces so that makedefs
 * won't split it across lines.  Ideally the length should be close to
 * that of the substituted value since we don't do phrase-splitting/line-
 * wrapping when displaying it.
 */
STATIC_VAR const struct rt_opt {
    const char *token, *value;
} rt_opts[] = {
    { ":PATMATCH:", regex_id },
};

/*
 * 3.6.0
 * Some optional stuff is no longer available to makedefs because
 * it depends which of several object files got linked into the
 * game image, so we insert those options here.
 */
STATIC_OVL void
insert_rtoption(buf)
char *buf;
{
    int i;

    for (i = 0; i < SIZE(rt_opts); ++i) {
        if (strstri(buf, rt_opts[i].token))
            (void) strsubst(buf, rt_opts[i].token, rt_opts[i].value);
        /* we don't break out of the loop after a match; there might be
           other matches on the same line */
    }
}

#ifdef MICRO
boolean
comp_times(filetime)
int64_t filetime;
{
    /* BUILD_TIME is constant but might have L suffix rather than UL;
       'filetime' is historically signed but ought to have been unsigned */
    return (boolean) ((uint64_t) filetime < (uint64_t) BUILD_TIME);
}
#endif

boolean
check_version(version_data, filename, complain)
struct version_info *version_data;
const char *filename;
boolean complain;
{
    uint64_t vsan1 = VERSION_SANITY1;
    uint64_t vsan2 = VERSION_SANITY2;
    uint64_t vsan3 = VERSION_SANITY3;

    unsigned char shortsize = VERSION_SHORTSIZE;
    unsigned char intsize = VERSION_INTSIZE;
    unsigned char longsize = VERSION_LONGSIZE;
    unsigned char ptrsize = VERSION_PTRSIZE;

    uint64_t versioncompat = EARLIEST_COMPATIBLE_VERSION_NUMBER;

    boolean incompatibleversioninfo = FALSE;

    if (
        version_data->incarnation < versioncompat
        || (incompatibleversioninfo = ((version_data->incarnation >= NEW_VERSION_INFO_VERSION && VERSION_NUMBER < NEW_VERSION_INFO_VERSION) || (version_data->incarnation < NEW_VERSION_INFO_VERSION && VERSION_NUMBER >= NEW_VERSION_INFO_VERSION)))
        || version_data->version_compatibility > VERSION_NUMBER
        ) {
        if (complain)
        {
            if(incompatibleversioninfo)
                pline("Version mismatch for file \"%s\" (Incompatible version info format: %lu, %lu, %lu, %lu).", filename, version_data->incarnation, version_data->incarnation < NEW_VERSION_INFO_VERSION ? 0UL : version_data->version_compatibility, VERSION_NUMBER, versioncompat);
            else
                pline("Version mismatch for file \"%s\" (%lu, %lu, %lu, %lu).", filename, version_data->incarnation, version_data->incarnation < NEW_VERSION_INFO_VERSION ? 0 : version_data->version_compatibility, VERSION_NUMBER, versioncompat);
        }
        return FALSE;
    } 
    else if (
#ifndef IGNORED_FEATURES
        version_data->feature_set != VERSION_FEATURES
#else
        (version_data->feature_set & ~IGNORED_FEATURES)
            != (VERSION_FEATURES & ~IGNORED_FEATURES)
#endif
        || version_data->entity_count != vsan1
        || version_data->struct_sizes1 != vsan2
        || version_data->struct_sizes2 != vsan3) {
        if (complain)
            pline("Configuration incompatibility for file \"%s\". (feat %lu, %lu, ecnt %lu, %lu, str1 %lu, %lu, str2 %lu, %lu, short %u, %u, int %u, %u, long %u, %u, ptr %u, %u)", filename, 
                (version_data->feature_set & ~IGNORED_FEATURES), (VERSION_FEATURES & ~IGNORED_FEATURES), version_data->entity_count, vsan1, version_data->struct_sizes1, vsan2, version_data->struct_sizes2, vsan3,
                version_data->short_size, shortsize, version_data->int_size, intsize, version_data->long_size, longsize, version_data->ptr_size, ptrsize);
        return FALSE;
    }
    return TRUE;
}

/* this used to be based on file date and somewhat OS-dependant,
   but now examines the initial part of the file's contents */
boolean
uptodate(fd, name)
int fd;
const char *name;
{
    int rlen;
    struct version_info vers_info;
    boolean verbose = name ? TRUE : FALSE;

    rlen = (int)read(fd, (genericptr_t) &vers_info, (readLenType)sizeof vers_info);
    minit(); /* ZEROCOMP */
    if (rlen == 0) {
        if (verbose) {
            pline("File \"%s\" is empty?", name);
            wait_synch();
        }
        return FALSE;
    }
    if (!check_version(&vers_info, name, verbose)) {
        if (verbose)
            wait_synch();
        return FALSE;
    }
    return TRUE;
}

void
store_version(fd)
int fd;
{
    static const struct version_info version_data = {
        VERSION_NUMBER, VERSION_FEATURES,
        VERSION_SANITY1, VERSION_SANITY2, VERSION_SANITY3,
        VERSION_SHORTSIZE, VERSION_INTSIZE, VERSION_LONGSIZE, VERSION_PTRSIZE,
        EARLIEST_COMPATIBLE_VERSION_NUMBER
    };

    bufoff(fd);
    /* bwrite() before bufon() uses plain write() */
    bwrite(fd, (genericptr_t) &version_data, sizeof version_data);
    Sprintf(priority_debug_buf_4, "store_savefileinfo (fd=%d)", fd);
    bufon(fd);
    return;
}

#ifdef AMIGA
const char amiga_version_string[] = AMIGA_VERSION_STRING;
#endif

uint64_t
get_feature_notice_ver(str)
char *str;
{
    char buf[BUFSZ];
    int ver_maj, ver_min, patch;
    char *istr[3];
    int j = 0;

    if (!str)
        return 0L;
    str = strcpy(buf, str);
    istr[j] = str;
    while (*str) {
        if (*str == '.') {
            *str++ = '\0';
            j++;
            istr[j] = str;
            if (j == 2)
                break;
        } else if (index("0123456789", *str) != 0) {
            str++;
        } else
            return 0L;
    }
    if (j != 2)
        return 0L;
    ver_maj = atoi(istr[0]);
    ver_min = atoi(istr[1]);
    patch = atoi(istr[2]);
    return FEATURE_NOTICE_VER(ver_maj, ver_min, patch);
    /* macro from hack.h */
}

uint64_t
get_current_feature_ver()
{
    return FEATURE_NOTICE_VER(VERSION_MAJOR, VERSION_MINOR, PATCHLEVEL);
}

/*ARGUSED*/
const char *
copyright_banner_line(indx)
int indx;
{
#ifdef COPYRIGHT_BANNER_A
    if (indx == 1)
        return COPYRIGHT_BANNER_A;
#endif
#ifdef COPYRIGHT_BANNER_B
    if (indx == 2)
        return COPYRIGHT_BANNER_B;
#endif
#ifdef COPYRIGHT_BANNER_C
    if (indx == 3)
        return COPYRIGHT_BANNER_C;
#endif
#ifdef COPYRIGHT_BANNER_D
    if (indx == 4)
        return COPYRIGHT_BANNER_D;
#endif
    return "";
}

/*version.c*/

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    sys.h    $NHDT-Date: 1449296291 2015/12/05 06:18:11 $  $NHDT-Branch: NetHack-3.6.0 $:$NHDT-Revision: 1.27 $ */
/* Copyright (c) Kenneth Lorber, Kensington, Maryland, 2008. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SYS_H
#define SYS_H

struct sysopt {
    char *support; /* local support contact */
    char *recover; /* how to run recover - may be overridden by win port */
    char *wizards; /* space-separated list of usernames */
    char *fmtd_wizard_list; /* formatted version of wizards; null or "one"
                               or "one or two" or "one, two, or three", &c */
    char *explorers;  /* like wizards, but for access to explore mode */
    char *shellers;   /* like wizards, for ! command (-DSHELL); also ^Z */
    char *genericusers; /* usernames that prompt for user name */
    char *debugfiles; /* files to show debugplines in. '*' is all. */
#if defined (DUMPLOG)
    char *dumplogfile; /* where the dump file is saved */
    char* dumplogurl;  /* url path for the above */
#endif
#ifdef DUMPHTML
    char* dumphtmlfile; /* where the html dump is saved */
    char* dumphtmlfontname; /* font name for html dump */
#ifdef DUMPHTML_WEBFONT_LINK
    char* dumphtmlfontlink; /* css web link for the font */
#endif
    char* dumphtml_css_fontface_normal; /* css @fontface definition for normal font */
    char* dumphtml_css_fontface_bold; /* css @fontface definition for bold font */
    char* dumphtml_css_fontface_italic; /* css @fontface definition for italic font */
    char* dumphtml_css_fontface_bolditalic; /* css @fontface definition for bold italic font */
#endif
    int env_dbgfl;    /*  1: debugfiles comes from getenv("DEBUGFILES")
                       *     so sysconf's DEBUGFILES shouldn't override it;
                       *  0: getenv() hasn't been attempted yet;
                       * -1: getenv() didn't find a value for DEBUGFILES.
                       */
    int maxplayers;
    int seduce;
    unsigned int livelog;  /* event types to livelog */
    int ll_conduct_turns;  /* do not livelog conducts before this turncount */
    int check_save_uid; /* restoring savefile checks UID? */
    int check_plname; /* use plname for checking wizards/explorers/shellers */
    int bones_pools;
    int min_difficulty;
    int max_difficulty;
    int select_pet_details;

    /* record file */
    int persmax;
    int pers_is_uid;
    int entrymax;
    int pointsmin;
    int tt_oname_maxrank;

    /* panic options */
    char *gdbpath;
    char *greppath;
    int panictrace_gdb;
    int panictrace_libc;
};

extern struct sysopt sysopt;

#define SYSOPT_SEDUCE sysopt.seduce

#endif /* SYS_H */

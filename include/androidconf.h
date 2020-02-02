/*	SCCS Id: @(#)androidconf.h	3.4	2011/03/31	*/
/* Copyright (c) Kenneth Lorber, Bethesda, Maryland, 1990, 1991, 1992, 1993. */
/* NetHack may be freely redistributed.  See license for details. */

#ifdef ANDROID
#ifndef ANDROIDCONF_H
#define ANDROIDCONF_H

#define error debuglog

#define NO_FILE_LINKS /* if no hard links */
#define LOCKDIR "." /* where to put locks */ 
//#define HOLD_LOCKFILE_OPEN	/* Keep an exclusive lock on the .0 file */
#define SELF_RECOVER		/* Allow the game itself to recover from an aborted game */ 

#ifdef getchar
#  undef getchar
#endif
#define getchar nhgetch
#undef tgetch
#define tgetch nhgetch
#define getuid() 1

#undef SHELL				/* we do not support the '!' command */

#ifdef DUMPLOG
#undef DUMPLOG_FILE
#define DUMPLOG_FILE        "%n.%d.dumplog.txt"
#endif

#define CONFIG_FILE "defaults.nh"

//#undef MAIL
//#undef DEF_PAGER
#undef DEF_MAILREADER

#define ASCIIGRAPH

#define NO_SIGNAL

#define SELECTSAVED

#define USER_SOUNDS

#define CHANGE_COLOR

# endif /* ANDROIDCONF_H */
#endif /* ANDROID */

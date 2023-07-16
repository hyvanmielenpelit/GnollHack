/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/*	SCCS Id: @(#)droidconf.h	3.4	2011/03/31	*/
/* Copyright (c) Kenneth Lorber, Bethesda, Maryland, 1990, 1991, 1992, 1993. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifdef GNH_IOS
#ifndef IOSCONF_H
#define IOSCONF_H

#define error debuglog

#define NO_FILE_LINKS /* if no hard links */
#define LOCKDIR "." /* where to put locks */ 
//#define HOLD_LOCKFILE_OPEN	/* Keep an exclusive lock on the .0 file */
#define SELF_RECOVER		/* Allow the game itself to recover from an aborted game */ 

#ifdef getchar
#undef getchar
#endif
#define getchar nhgetch
#undef tgetch
#define tgetch nhgetch
#define getuid() 1

#undef SHELL				/* we do not support the '!' command */

#if defined (DUMPLOG) || defined (DUMPHTML)
#undef DUMPLOG_FILE
#define DUMPLOG_FILE        "dumplog/gnollhack.%n.%d.txt" /* Note: Actually the one in sysconf is used, not this one */
#undef DUMPLOG_DIR
#define DUMPLOG_DIR        "dumplog" /* Note: this is just used to create a directory, DUMPLOG_FILE still needs to have the full path */
#endif

//#undef MAIL
//#undef DEF_PAGER
#undef DEF_MAILREADER

#define ASCIIGRAPH

#define NO_SIGNAL

#define SELECTSAVED

#define USER_SOUNDS

#define CHANGE_COLOR

#endif /* IOSCONF_H */
#endif /* GNH_IOS */

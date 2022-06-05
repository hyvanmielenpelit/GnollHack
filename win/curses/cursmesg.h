/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* vim:set cin ft=c sw=4 sts=4 ts=8 et ai cino=Ls\:0t0(0 : -*- mode:c;fill-column:80;tab-width:8;c-basic-offset:4;indent-tabs-mode:nil;c-file-style:"k&r" -*-*/
/* GnollHack 4.0 cursmesg.h */
/* Copyright (c) Karl Garrison, 2010. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef CURSMESG_H
# define CURSMESG_H


/* Global declarations */

void curses_message_win_puts(const char *message, boolean recursed);
void curses_got_input(void);
int curses_block(boolean require_tab);
int curses_more(void);
void curses_clear_unhighlight_message_window(void);
void curses_message_win_getline_ex(int attr, int color, const char *prompt, char *answer, int buffer);
void curses_last_messages(void);
void curses_init_mesg_history(void);
void curses_prev_mesg(void);
void curses_count_window(const char *count_text);
char *curses_getmsghistory_ex(int*, int*, BOOLEAN_P);
void curses_putmsghistory_ex(const char *, int, int, BOOLEAN_P);

#endif /* CURSMESG_H */

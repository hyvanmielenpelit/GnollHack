/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* vim:set cin ft=c sw=4 sts=4 ts=8 et ai cino=Ls\:0t0(0 : -*- mode:c;fill-column:80;tab-width:8;c-basic-offset:4;indent-tabs-mode:nil;c-file-style:"k&r" -*-*/
/* GnollHack 4.0 cursdial.h */
/* Copyright (c) Karl Garrison, 2010. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef CURSDIAL_H
# define CURSDIAL_H

/* Global declarations */

void curses_line_input_dialog(int style, int attr, int color, const char *prompt, char *answer, int buffer);
int curses_character_input_dialog(int attr UNUSED, int color UNUSED, const char *prompt, const char *choices,
                                  CHAR_P def);
int curses_ext_cmd(void);
void curses_create_nhmenu(winid wid);
void curses_add_nhmenu_item(winid wid, int glyph, const ANY_P *identifier,
                            CHAR_P accelerator, CHAR_P group_accel, int attr, int color,
                            const char *str, BOOLEAN_P presel);
void curs_menu_set_bottom_heavy(winid);
void curses_finalize_nhmenu(winid wid, const char *prompt, const char *subtitle);
int curses_display_nhmenu(winid wid, int how, MENU_ITEM_P **_selected);
boolean curses_menu_exists(winid wid);
void curses_del_menu(winid, boolean);

#endif /* CURSDIAL_H */

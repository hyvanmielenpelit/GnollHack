/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0	gnmenu.h	$NHDT-Date: 1432512805 2015/05/25 00:13:25 $  $NHDT-Branch: master $:$NHDT-Revision: 1.9 $ */
/* Copyright (C) 1998 by Erik Andersen <andersee@debian.org> */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef GnomeHackMenuWindow_h
#define GnomeHackMenuWindow_h

#include <gnome.h>
#include "config.h"
#include "global.h"
#include "gnomeprv.h"

GtkWidget *ghack_init_menu_window(void);

struct _GHackMenuItem {
    int glyph;
    const ANY_P *identifier;
    CHAR_P accelerator;
    CHAR_P group_accel;
    int attr;
    const char *str;
    BOOLEAN_P presel;
};

typedef struct _GHackMenuItem GHackMenuItem;

int ghack_menu_window_select_menu(GtkWidget *menuWin, MENU_ITEM_P **_selected,
                                  gint how);
int ghack_menu_ext_cmd(void);

#endif /* GnomeHackMenuWindow_h */

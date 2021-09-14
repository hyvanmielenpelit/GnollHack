/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    mhmain.h    $NHDT-Date: 1432512811 2015/05/25 00:13:31 $  $NHDT-Branch: master $:$NHDT-Revision: 1.14 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MSWINMainWindow_h
#define MSWINMainWindow_h


/* this is a main appliation window */

#include "winMS.h"

#define MAIN_TIMER_INTERVAL 10

typedef struct mswin_GnollHack_main_window {
    int mapAcsiiModeSave;
} NHMainWindow, *PNHMainWindow;


HWND mswin_init_main_window(void);
void mswin_layout_main_window(HWND changed_child);
void mswin_select_map_mode(int map_mode);
void mswin_menu_check_intf_mode(void);

#endif /* MSWINMainWindow_h */

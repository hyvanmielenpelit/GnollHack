/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mhdlg.h    $NHDT-Date: 1432512812 2015/05/25 00:13:32 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MSWINDlgWindow_h
#define MSWINDlgWindow_h

#include "winMS.h"
#include "config.h"

int mswin_getlin_window(int style, int attr, int color, const char *question, char *result,
                        size_t result_size);
int mswin_ext_cmd_window(int *selection);
boolean mswin_player_selection_window();

#endif /* MSWINDlgWindow_h */

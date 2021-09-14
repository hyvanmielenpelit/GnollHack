/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0	mhmsgwnd.h	$NHDT-Date: 1432512798 2015/05/25 00:13:18 $  $NHDT-Branch: master $:$NHDT-Revision: 1.9 $ */
/* Copyright (C) 2001 by Alex Kompel 	 */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MSWINMessageWindow_h
#define MSWINMessageWindow_h

#include "winMS.h"
#include "config.h"
#include "global.h"

HWND mswin_init_message_window();
void mswin_message_window_size(HWND hWnd, LPSIZE sz);

#endif /* MSWINMessageWindow_h */

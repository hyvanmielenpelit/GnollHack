/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    mhmsg.h    $NHDT-Date: 1432512811 2015/05/25 00:13:31 $  $NHDT-Branch: master $:$NHDT-Revision: 1.15 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#include "layer.h"
#include "general.h"

#ifndef MHGnollHackMessages_H
#define MHGnollHackMessages_H

/* GnollHack messages */
#define WM_MSNH_COMMAND (WM_APP + 1)

#define MSNH_MSG_ADDWND 100
#define MSNH_MSG_PUTSTR 101
#define MSNH_MSG_PRINT_GLYPH 102
#define MSNH_MSG_CLEAR_WINDOW 103
#define MSNH_MSG_CLIPAROUND 104
#define MSNH_MSG_STARTMENU 105
#define MSNH_MSG_ADDMENU 106
#define MSNH_MSG_CURSOR 107
#define MSNH_MSG_ENDMENU 108
#define MSNH_MSG_DIED 109
#define MSNH_MSG_CARET 110
#define MSNH_MSG_GETTEXT 111
#define MSNH_MSG_UPDATE_STATUS 112
#define MSNH_MSG_RANDOM_INPUT 113
#define MSNH_MSG_STRETCH_MAP 114
#define MSNH_MSG_UPDATE_ALL_STATUSES_ON_MAP 115
#define MSNH_MSG_SET_ANIMATION_TIMER 116

typedef struct mswin_nhmsg_add_wnd {
    winid wid;
} MSNHMsgAddWnd, *PMSNHMsgAddWnd;

typedef struct mswin_nhmsg_putstr {
    int attr;
    const char *text;
    int append;
    int color;
    const char* attrs;
    const char* colors;
} MSNHMsgPutstr, *PMSNHMsgPutstr;

typedef struct mswin_nhmsg_print_glyph {
    XCHAR_P x;
    XCHAR_P y;
    struct layer_info layers;
} MSNHMsgPrintGlyph, *PMSNHMsgPrintGlyph;

typedef struct mswin_nhmsg_cliparound {
    int x;
    int y;
    boolean force;
} MSNHMsgClipAround, *PMSNHMsgClipAround;

typedef struct mswin_nhmsg_add_menu {
    int glyph;
    const ANY_P *identifier;
    struct obj* object;
    CHAR_P accelerator;
    CHAR_P group_accel;
    int attr;
    int color;
    const char *str;
    BOOLEAN_P presel;
    unsigned long miflags;
} MSNHMsgAddMenu, *PMSNHMsgAddMenu;

typedef struct mswin_nhmsg_cursor {
    int x;
    int y;
} MSNHMsgCursor, *PMSNHMsgCursor;

typedef struct mswin_nhmsg_end_menu_ex {
    const char *text;
    const char *subtitle;
} MSNHMsgEndMenu, *PMSNHMsgEndMenu;


#define TEXT_BUFFER_SIZE 4096

typedef struct mswin_nhmsg_get_text {
    size_t max_size;
    char buffer[TEXT_BUFFER_SIZE];
    char attrs[TEXT_BUFFER_SIZE];
    char colors[TEXT_BUFFER_SIZE];
} MSNHMsgGetText, *PMSNHMsgGetText;

typedef struct mswin_nhmsg_update_status {
    struct mswin_status_lines * status_lines;
} MSNHMsgUpdateStatus, *PMSNHMsgUpdateStatus;

#endif

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mhstatus.h    $NHDT-Date: 1432512812 2015/05/25 00:13:32 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MSWINStatusWindow_h
#define MSWINStatusWindow_h

#include "winMS.h"
#include "config.h"
#include "botl.h"

#define NHSW_LINES MAX_STATUS_LINES

#define MSWIN_MAX_LINE2_2STATUSLINES_STRINGS (MAX_STATUS_LINE_ITEMS + BL_MASK_BITS)
#define MSWIN_MAX_LINE_STRINGS MSWIN_MAX_LINE2_2STATUSLINES_STRINGS

#define MSWIN_LINE2_2STATUSLINES_FIELDS (MAX_STATUS_LINE_ITEMS)
#define MSWIN_MAX_LINE_FIELDS MSWIN_LINE2_2STATUSLINES_FIELDS

/* when status hilites are enabled, we use an array of mswin_status_strings
 * to represent what needs to be rendered. */
typedef struct mswin_status_string  {
    const char * str; /* ascii string to be displayed */
    boolean space_in_front; /* render with a space in front of string */
    int color; /* string text color index */
    int attribute; /* string text attributes */
    boolean draw_bar; /* draw a percentage bar  */
    int bar_percent; /* a percentage to indicate */
    int bar_color; /* color index of percentage bar */
    int bar_attribute; /* attributes of percentage bar */
} mswin_status_string;

typedef struct mswin_status_strings
{
    int count;
    mswin_status_string * status_strings[MSWIN_MAX_LINE_STRINGS];
} mswin_status_strings;

typedef struct mswin_status_field {
    int field_index; // field index
    boolean enabled; // whether the field is enabled
    const char * name; // name of status field
    const char * format; // format of field
    boolean space_in_front; // add a space in front of the field

    int percent;
    int color;
    int attribute;
    char string[BUFSZ];

} mswin_status_field;

typedef struct mswin_condition_field {
    uint64_t mask;
    const char * name;
    int bit_position;
} mswin_condition_field;

typedef struct mswin_status_fields {
    int count;
    mswin_status_field * status_fields[MSWIN_MAX_LINE_FIELDS];
} mswin_status_fields;

typedef struct mswin_status_line {
    mswin_status_strings status_strings;
    mswin_status_fields status_fields;
} mswin_status_line;

typedef struct mswin_status_lines {
    mswin_status_line lines[NHSW_LINES]; /* number of strings to be rendered on each line */
} mswin_status_lines;

HWND mswin_init_status_window(void);
void mswin_status_window_size(HWND hWnd, LPSIZE sz);

#endif /* MSWINStatusWindow_h */

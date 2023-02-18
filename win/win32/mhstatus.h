/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2021-09-14 */

/* GnollHack 4.0    mhstatus.h    $NHDT-Date: 1432512812 2015/05/25 00:13:32 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (C) 2001 by Alex Kompel      */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MSWINStatusWindow_h
#define MSWINStatusWindow_h

#include "winMS.h"
#include "config.h"
#include "global.h"

#define NHSW_LINES MAX_STATUS_LINES

static const int fieldorder1[] = { BL_TITLE, BL_STR, BL_DX,    BL_CO,    BL_IN,
                             BL_WI,    BL_CH, BL_GOLD,  /*BL_ALIGN,*/ BL_FLUSH };
static const int fieldorder2[] = { BL_MODE, BL_LEVELDESC, BL_HP,   BL_HPMAX,
                             BL_ENE,       BL_ENEMAX,    BL_AC,  BL_MC_LVL, BL_MC_PCT,    BL_MOVE, BL_UWEP, BL_UWEP2,  BL_XP,
                             BL_EXP,       BL_HD,        BL_TIME, BL_REALTIME, BL_SCORE, BL_FLUSH };
static const int fieldorder2_2statuslines[] = { BL_MODE, BL_LEVELDESC, BL_HP,   BL_HPMAX,
                             BL_ENE,       BL_ENEMAX,    BL_AC,    BL_MC_LVL, BL_MC_PCT,    BL_MOVE, BL_UWEP, BL_UWEP2,   BL_XP,
                             BL_EXP,       BL_HD,        BL_TIME,  BL_REALTIME, BL_2WEP,   BL_SKILL,     BL_HUNGER, BL_CAP,  BL_CONDITION, BL_FLUSH };
static const int fieldorder3[] = { BL_2WEP, BL_SKILL,     BL_HUNGER,
                             BL_CAP,       BL_CONDITION, BL_FLUSH };

static const int fieldorder1_alt[] = { BL_TITLE, BL_STR, BL_DX,    BL_CO,    BL_IN,
                             BL_WI,    BL_CH, BL_ALIGN, BL_FLUSH };
static const int fieldorder2_alt[] = { BL_LEVELDESC, BL_HP,   BL_HPMAX,
                             BL_ENE,       BL_ENEMAX,    BL_AC,  BL_MC_LVL, BL_MC_PCT,    BL_MOVE, BL_UWEP, BL_UWEP2,  BL_XP,
                             BL_EXP,       BL_HD,        BL_FLUSH };
static const int fieldorder3_alt[] = { BL_MODE, BL_GOLD, BL_TIME, BL_REALTIME,  BL_SCORE,  BL_2WEP, BL_SKILL,     BL_HUNGER,
                             BL_CAP,       BL_CONDITION, BL_FLUSH };

static const int fieldorder4[] = { BL_PARTYSTATS, BL_FLUSH };
static const int fieldorder5[] = { BL_PARTYSTATS2, BL_FLUSH };
static const int fieldorder6[] = { BL_PARTYSTATS3, BL_FLUSH };
static const int fieldorder7[] = { BL_PARTYSTATS4, BL_FLUSH };
static const int fieldorder8[] = { BL_PARTYSTATS5, BL_FLUSH };

static const int* fieldorders_2statuslines[NHSW_LINES + 1] = { fieldorder1, fieldorder2_2statuslines, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
static const int fieldcounts_2statuslines[NHSW_LINES] = {
    SIZE(fieldorder1) - 1, SIZE(fieldorder2_2statuslines) - 1, 0, 0, 0, 0, 0, 0
};
static const int *fieldorders[NHSW_LINES + 1] = { fieldorder1, fieldorder2, fieldorder3, fieldorder4, fieldorder5, fieldorder6, fieldorder7, fieldorder8, NULL };
static const int fieldcounts[NHSW_LINES] = { 
    SIZE(fieldorder1) - 1, SIZE(fieldorder2) - 1, SIZE(fieldorder3) - 1 , SIZE(fieldorder4) - 1,
    SIZE(fieldorder5) - 1, SIZE(fieldorder6) - 1, SIZE(fieldorder7) - 1 , SIZE(fieldorder8) - 1
};
//#define MSWIN_MAX_LINE1_STRINGS (SIZE(fieldorder1) - 1)
//#define MSWIN_MAX_LINE2_STRINGS (SIZE(fieldorder2) - 1)
#define MSWIN_MAX_LINE2_2STATUSLINES_STRINGS (SIZE(fieldorder2_2statuslines) - 1 + BL_MASK_BITS)
//#define MSWIN_MAX_LINE3_STRINGS (SIZE(fieldorder3) - 1 + BL_MASK_BITS)
#define MSWIN_MAX_LINE_STRINGS MSWIN_MAX_LINE2_2STATUSLINES_STRINGS

//#define MSWIN_LINE1_FIELDS (SIZE(fieldorder1) - 1)
//#define MSWIN_LINE2_FIELDS (SIZE(fieldorder2) - 1)
#define MSWIN_LINE2_2STATUSLINES_FIELDS (SIZE(fieldorder2_2statuslines) - 1)
//#define MSWIN_LINE3_FIELDS (SIZE(fieldorder3) - 1)
#define MSWIN_MAX_LINE_FIELDS MSWIN_LINE2_2STATUSLINES_FIELDS

static const int* fieldorders_alt[NHSW_LINES + 1] = { fieldorder1_alt, fieldorder2_alt, fieldorder3_alt, fieldorder4, fieldorder5, fieldorder6, fieldorder7, fieldorder8, NULL };
static const int fieldcounts_alt[NHSW_LINES] = {
    SIZE(fieldorder1_alt) - 1, SIZE(fieldorder2_alt) - 1, SIZE(fieldorder3_alt) - 1 , SIZE(fieldorder4) - 1,
    SIZE(fieldorder5) - 1, SIZE(fieldorder6) - 1, SIZE(fieldorder7) - 1 , SIZE(fieldorder8) - 1
};

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
    unsigned long mask;
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

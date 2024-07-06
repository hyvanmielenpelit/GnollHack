/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0  wintype.h       $NHDT-Date: 1549327486 2019/02/05 00:44:46 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.19 $ */
/* Copyright (c) David Cohrs, 1991                                */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef WINTYPE_H
#define WINTYPE_H

#include "config.h"

typedef int winid; /* a window identifier */

/* generic parameter - must not be any larger than a pointer */
typedef union any {
    genericptr_t a_void;
    struct obj *a_obj;
    struct monst *a_monst;
    int a_int;
    char a_char;
    schar a_schar;
    uchar a_uchar;
    unsigned int a_uint;
    long a_long;
    unsigned long a_ulong;
    long long a_longlong;
    unsigned long long a_ulonglong;
    int64_t a_int64;
    uint64_t a_uint64;
    int *a_iptr;
    long *a_lptr;
    unsigned long *a_ulptr;
    unsigned *a_uptr;
    const char *a_string;
    int NDECL((*a_nfunc));
    uint64_t a_mask64; /* used by status highlighting */
    coord a_coord;
    struct nhregion *a_nhregion;
    float a_float;
    double a_double;
    struct trap* a_trap;
    /* add types as needed */
    
} anything;
#define ANY_P union any /* avoid typedef in prototypes */
                        /* (buggy old Ultrix compiler) */

/* symbolic names for the data types housed in anything */
enum any_types {
    ANY_VOID = 1,
    ANY_OBJ,         /* struct obj */
    ANY_MONST,       /* struct monst (not used) */
    ANY_INT,         /* int */
    ANY_CHAR,        /* char */
    ANY_UCHAR,       /* unsigned char */
    ANY_SCHAR,       /* signed char */
    ANY_UINT,        /* unsigned int */
    ANY_LONG,        /* long */
    ANY_ULONG,       /* unsigned long */
    ANY_INT64,       /* int64_t */
    ANY_UINT64,      /* uint64_t */
    ANY_IPTR,        /* pointer to int */
    ANY_UPTR,        /* pointer to unsigned int */
    ANY_LPTR,        /* pointer to long */
    ANY_ULPTR,       /* pointer to unsigned long */
    ANY_STR,         /* pointer to null-terminated char string */
    ANY_NFUNC,       /* pointer to function taking no args, returning int */
    ANY_MASK64       /* 64-bit mask (stored as uint64_t) */
};

/* menu return list */
typedef struct mi {
    anything item; /* identifier */
    int64_t count;    /* count */
} menu_item;
#define MENU_ITEM_P struct mi

/* select_menu() "how" argument types */
/* [MINV_PICKMASK in monst.h assumes these have values of 0, 1, 2] */
#define PICK_NONE 0 /* user picks nothing (display only) */
#define PICK_ONE 1  /* only pick one */
#define PICK_ANY 2  /* can pick any amount */

/* window types */
/* OBSOLETE (all collected here): any additional port specific types should be defined in win*.h */
#define NHW_NONE 0 /* Unallocated window type.  Must be    */
                   /* different from any other NHW_* type. From winX.h */

#define NHW_MESSAGE 1
#define NHW_STATUS 2
#define NHW_MAP 3
#define NHW_MENU 4
#define NHW_TEXT 5

/* Window types added in GnollHack; these may not exist in most windowing systems, so use with care */
#define NHW_BASE 6
#define NHW_HERE 7     /* new - window for "things that are here" */
#define NHW_INVEN 8    /* from winMS.h in WinCE and Win32 */

#if defined(DUMPLOG) || defined(DUMPHTML)
#define NHW_DUMPTXT 9
#define NHW_DUMPHTML 10
#endif

/* Various esoteric window types from various ports; numbering has been changed */
#define NHW_RIP 12     /* from winMS.h in WinCE and Win32 */
#define NHW_KEYPAD 13  /* from winMS.h in WinCE and Win32 */
#define NHW_OVER 14    /* overview window; from amiga */
#define NHW_WORN 15    /* from winGnome.h */

/* attribute types for putstr; the same as the ANSI value, for convenience */
#define ATR_NONE            0x0000
#define ATR_BOLD            0x0001
#define ATR_DIM             0x0002
#define ATR_ULINE           0x0004
#define ATR_BLINK           0x0005
#define ATR_INVERSE         0x0007
#define ATR_ATTR_MASK       0x000F

/* not a display attribute but passed to putstr() as an attribute;
   can be masked with one regular display attribute */
#define ATR_URGENT          0x0010
#define ATR_NOHISTORY       0x0020
#define ATR_STAY_ON_LINE    0x0040
#define ATR_NOTABS          0x0080
#define ATR_TITLE           0x0100
#define ATR_HEADING         0x0200
#define ATR_SUB             0x0400
#define ATR_SUBTITLE        (ATR_SUB | ATR_TITLE)
#define ATR_SUBHEADING      (ATR_SUB | ATR_HEADING)
#define ATR_INDENT_AT_DASH      0x00000800 /* With automatic wrap, indents at the first dash (-) + spaces after it */
#define ATR_INDENT_AT_COLON     0x00001000 /* With automatic wrap, indents at the first colon (:) + spaces after it */
#define ATR_INDENT_AT_ASTR      0x00002000 /* With automatic wrap, indents at the first asterisk (*) + spaces after it */
#define ATR_INDENT_AT_SPACE     0x00004000 /* With automatic wrap, indents at the first space ( ) + spaces after it */
#define ATR_INDENT_AT_BRACKET   0x00008000 /* With automatic wrap, indents at the first closing bracket (]) + spaces after it */
#define ATR_INDENT_AT_PERIOD (ATR_INDENT_AT_DASH | ATR_INDENT_AT_COLON) /* With automatic wrap, indents at the first space (.) + spaces after it */
#define ATR_INDENT_AT_DOUBLE_SPACE (ATR_INDENT_AT_DASH | ATR_INDENT_AT_SPACE) /* With automatic wrap, indents at the first double space (  ) + spaces after it */
#define ATR_INDENT_AT_BRACKET_OR_DOUBLE_SPACE (ATR_INDENT_AT_BRACKET | ATR_INDENT_AT_DASH | ATR_INDENT_AT_SPACE) /* With automatic wrap, indents at the first double space (  ) + spaces after it */
#define ATR_INDENT_MASK         (ATR_INDENT_AT_DASH | ATR_INDENT_AT_COLON | ATR_INDENT_AT_ASTR | ATR_INDENT_AT_SPACE | ATR_INDENT_AT_BRACKET)
#define ATR_ALIGN_CENTER        0x00010000
#define ATR_ALIGN_RIGHT         0x00020000
#define ATR_INACTIVE            0x00040000
#define ATR_HALF_SIZE           0x00080000
#define ATR_ALT_COLORS          0x00100000
#define ATR_ALT_DIVISORS        0x00200000
#define ATR_ORDERED_LIST        0x00400000
#define ATR_START_TABLE         0x00800000
#define ATR_END_TABLE           0x01000000
#define ATR_TABLE_ROW           0x02000000
#define ATR_TABLE_HEADER        0x04000000
#define ATR_START_TABLE_BODY    0x08000000
#define ATR_END_TABLE_BODY      0x10000000
#define ATR_PARAGRAPH_LINE      0x20000000
#define ATR_LINE_ATTR_MASK      0x7FFFFFF0 /* because this is signed integer */

/* nh_poskey() modifier types */
#define CLICK_1 1
#define CLICK_2 2
#define CLICK_3 3 /* Precision mode click, move only single squares */

/* invalid winid */
#define WIN_ERR ((winid) -1)

/* menu window keyboard commands (may be mapped) */
/* clang-format off */
#define MENU_FIRST_PAGE         '^'
#define MENU_LAST_PAGE          '|'
#define MENU_NEXT_PAGE          '>'
#define MENU_PREVIOUS_PAGE      '<'
#define MENU_SELECT_ALL         '.'
#define MENU_UNSELECT_ALL       '-'
#define MENU_INVERT_ALL         '@'
#define MENU_SELECT_PAGE        ','
#define MENU_UNSELECT_PAGE      '\\'
#define MENU_INVERT_PAGE        '~'
#define MENU_SEARCH             ':'
#define MENU_START_COUNT        '#'
/* clang-format on */

#endif /* WINTYPE_H */

/* GnollHack 4.0  wintype.h       $NHDT-Date: 1549327486 2019/02/05 00:44:46 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.19 $ */
/* Copyright (c) David Cohrs, 1991                                */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef WINTYPE_H
#define WINTYPE_H

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
    int *a_iptr;
    long *a_lptr;
    unsigned long *a_ulptr;
    unsigned *a_uptr;
    const char *a_string;
    int NDECL((*a_nfunc));
    unsigned long a_mask32; /* used by status highlighting */
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
    ANY_IPTR,        /* pointer to int */
    ANY_UPTR,        /* pointer to unsigned int */
    ANY_LPTR,        /* pointer to long */
    ANY_ULPTR,       /* pointer to unsigned long */
    ANY_STR,         /* pointer to null-terminated char string */
    ANY_NFUNC,       /* pointer to function taking no args, returning int */
    ANY_MASK32       /* 32-bit mask (stored as unsigned long) */
};

/* menu return list */
typedef struct mi {
    anything item; /* identifier */
    long count;    /* count */
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

/* Added in GnollHack */
#define NHW_BASE 6
#define NHW_OVER 7 /* overview window; from amiga */
#define NHW_WORN 8 /* from winGnome.h, changed from 6 to 8 */
#define NHW_HERE 9 /* Window for "things that are here" */

/* From winMS.h in WinCE and WinMS */
#define NHW_INVEN 11   /* originally 33 */
#define NHW_RIP 12     /* originally 32 */
#define NHW_KEYPAD 13  /* originally 33 */

/* attribute types for putstr; the same as the ANSI value, for convenience */
#define ATR_NONE       0
#define ATR_BOLD       1
#define ATR_DIM        2
#define ATR_ULINE      4
#define ATR_BLINK      5
#define ATR_INVERSE    7
/* not a display attribute but passed to putstr() as an attribute;
   can be masked with one regular display attribute */
#define ATR_URGENT    16
#define ATR_NOHISTORY 32
#define ATR_STAY_ON_LINE 64
#define ATR_NOTABS    128

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

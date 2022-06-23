/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    winMS.h    $NHDT-Date: 1434804346 2015/06/20 12:45:46 $  $NHDT-Branch: win32-x64-working $:$NHDT-Revision: 1.41 $ */
/* Copyright (C) 2001 by Alex Kompel */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef WINMS_H
#define WINMS_H

#ifdef _MSC_VER
#if _MSC_VER >= 1400
/* Visual C 8 warning elimination */
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#endif
#endif

//#undef Protection 
                    /* We have a global name space collision.  No source file
                     using win32api.h should be using the Protection macro
                     from youprop.h.
                     A better fix would be to ensure we include all window
                     header files before we start clobbering the global name
                     space with GnollHack specific macros. */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include "hack.h"
#include "color.h"
#include "soundfx.h"
#include "mhwingdi.h"
#include "lev.h"


/* Create an array to keep track of the various windows */

#ifndef MAXWINDOWS
#define MAXWINDOWS 15
#endif

/* tile background color */
#define TILE_BK_COLOR RGB(TILE_BK_COLOR_RED, TILE_BK_COLOR_GREEN, TILE_BK_COLOR_BLUE)

#define TILE_SHEET_IDX(ntile) \
    (min(GetNHApp()->mapTileSheets - 1, max(0, (ntile / NUM_TILES_PER_SHEET))))
#define TILEBMP_X(ntile) \
    (((ntile % NUM_TILES_PER_SHEET) % GetNHApp()->mapTilesPerLine[TILE_SHEET_IDX(ntile)]) * GetNHApp()->mapTile_X)
#define TILEBMP_Y(ntile) \
    (((ntile % NUM_TILES_PER_SHEET) / GetNHApp()->mapTilesPerLine[TILE_SHEET_IDX(ntile)]) * GetNHApp()->mapTile_Y)


/* minimum/maximum font size (in points - 1/72 inch) */
#define NHFONT_DEFAULT_SIZE 9
#define NHFONT_SIZE_MIN 3
#define NHFONT_SIZE_MAX 20

#define MAX_LOADSTRING 100

#define ANIMATION_TIMER_INTERVAL ANIMATION_FRAME_INTERVAL // milliseconds (40 FPS)


typedef struct mswin_nhwindow_data {
    HWND win;
    int type;
    int dead;
} MSNHWinData, *PMSNHWinData;

typedef BOOL(WINAPI *LPTRANSPARENTBLT)(HDC, int, int, int, int, HDC, int, int,
                                       int, int, UINT);

typedef struct mswin_nhwindow_app {
    HINSTANCE hApp;
    HWND hMainWnd;
    HACCEL hAccelTable;
    HWND hPopupWnd; /* current popup window  */

    MSNHWinData windowlist[MAXWINDOWS];

    HICON iconTiles;
    HBITMAP bmpTiles[MAX_TILE_SHEETS];
    HBITMAP bmpMapTiles[MAX_TILE_SHEETS]; /* custom tiles bitmap */
    HBITMAP bmpRip;
    HBITMAP bmpSplash;
    HBITMAP bmpFMOD;
    int mapTile_X;       /* tile width */
    int mapTile_Y;       /* tile height */
    int mapTilesPerLine[MAX_TILE_SHEETS]; /* number of tile per row in the bitmap */
    int mapTileSheets;   /* number of used tile sheets */

    boolean bNoHScroll; /* disable cliparound for horizontal grid (map) */
    boolean bNoVScroll; /* disable cliparound for vertical grid (map) */

    int mapDisplayModeSave; /* saved map display mode */

    char *saved_text;

    DWORD saveRegistrySettings; /* Flag if we should save this time */
    DWORD
        regGnollHackMode; /* GnollHack mode means no Windows keys in some places
                           */

    COLORREF regMapColors[CLR_MAX];

    LONG regMainMinX;
    LONG regMainMinY;
    LONG regMainMaxX;
    LONG regMainMaxY;
    LONG regMainLeft;
    LONG regMainTop;
    LONG regMainBottom;
    LONG regMainRight;
    DWORD regMainShowState;

    BOOL bAutoLayout;
    RECT rtMapWindow;
    RECT rtMsgWindow;
    RECT rtStatusWindow;
    RECT rtMenuWindow;
    RECT rtTextWindow;
    RECT rtInvenWindow;
    BOOL bWindowsLocked; /* TRUE if windows are "locked" - no captions */

    BOOL bNoSounds; /* disable sounds */

    LPTRANSPARENTBLT lpfnTransparentBlt; /* transparent blt function */
} NHWinApp, *PNHWinApp;

#define E extern

E PNHWinApp GetNHApp(void);
E struct window_procs mswin_procs;

#undef E

/* Some prototypes */
void mswin_init_nhwindows(int *argc, char **argv);
void mswin_player_selection(void);
void mswin_askname(void);
void mswin_get_nh_event(void);
void mswin_exit_nhwindows(const char *);
void mswin_suspend_nhwindows(const char *);
void mswin_resume_nhwindows(void);
winid mswin_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info);
void mswin_clear_nhwindow(winid wid);
void mswin_display_nhwindow(winid wid, BOOLEAN_P block);
void mswin_destroy_nhwindow(winid wid);
void mswin_curs(winid wid, int x, int y);
void mswin_putstr(winid wid, int attr, const char *text);
void mswin_putstr_ex(winid wid, int attr, const char *text, int app, int color);
void mswin_display_file(const char *filename, BOOLEAN_P must_exist);
void mswin_start_menu_ex(winid wid, int style);
void mswin_add_menu(winid wid, int glyph, const ANY_P *identifier,
                    CHAR_P accelerator, CHAR_P group_accel, int attr,
                    const char *str, BOOLEAN_P presel);
void mswin_add_extended_menu(winid wid, int glyph, const ANY_P* identifier, struct extended_menu_info info,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel);
void mswin_end_menu_ex(winid wid, const char *prompt, const char* subtitle);
int mswin_select_menu(winid wid, int how, MENU_ITEM_P **selected);
void mswin_update_inventory(void);
void mswin_mark_synch(void);
void mswin_wait_synch(void);
void mswin_cliparound(int x, int y, BOOLEAN_P force);
void mswin_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers);
void mswin_issue_gui_command(int initid);
void mswin_raw_print(const char *str);
void mswin_raw_print_bold(const char *str);
void mswin_raw_print_flush();
int mswin_nhgetch(void);
int mswin_nh_poskey(int *x, int *y, int *mod);
void mswin_nhbell(void);
int mswin_doprev_message(void);
char mswin_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char *question, const char *choices, CHAR_P def, const char* resp_desc, unsigned long ynflags);
void mswin_getlin_ex(int style, int attr, int color, const char *question, char *input, const char* placeholder, const char* linesuffix, const char* introline);
int mswin_get_ext_cmd(void);
void mswin_number_pad(int state);
void mswin_delay_output(void);
void mswin_delay_output_milliseconds(int interval);
void mswin_delay_output_intervals(int intervals);
void mswin_change_color(void);
char *mswin_get_color_string(void);
void mswin_start_screen(void);
void mswin_end_screen(void);
void mswin_outrip(winid wid, int how, time_t when);
void mswin_preference_update(const char *pref);
char *mswin_getmsghistory_ex(int* attr_ptr, int* color_ptr, BOOLEAN_P init);
void mswin_putmsghistory_ex(const char *msg, int attr, int color, BOOLEAN_P restoring);

void mswin_status_init(void);
void mswin_statuslines_init(void);
void mswin_status_finish(void);
void mswin_status_enablefield(int fieldidx, const char *nm, const char *fmt,
                              boolean enable);
void mswin_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long *colormasks);
void mswin_stretch_window(void);
void mswin_set_animation_timer_interval(unsigned int);
int mswin_open_special_view(struct special_view_info info);
void mswin_stop_all_sounds(struct stop_all_info info);
void mswin_play_immediate_ghsound(struct ghsound_immediate_info info);
void mswin_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info);
void mswin_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info);
void mswin_set_effect_ambient_volume(struct effect_ambient_volume_info info);
void mswin_play_ghsound_music(struct ghsound_music_info info);
void mswin_play_ghsound_level_ambient(struct ghsound_level_ambient_info info);
void mswin_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info);
void mswin_adjust_ghsound_general_volumes(VOID_ARGS);
void mswin_add_ambient_ghsound(struct soundsource_t* soundsource);
void mswin_delete_ambient_ghsound(struct soundsource_t* soundsource);
void mswin_set_ambient_ghsound_volume(struct soundsource_t* soundsource);
void mswin_clear_context_menu(VOID_ARGS);
void mswin_add_context_menu(int cmd_def_char, int cmd_cur_char, int style, int glyph, const char* cmd_text, const char* target_text, int attr, int color);
void mswin_update_status_button(int cmd, int btn, int val, unsigned long bflags);
void mswin_toggle_animation_timer(int timertype, int timerid, int state, int x, int y, int layer, unsigned long tflags);
void mswin_display_floating_text(int x, int y, const char* text, int style, int attr, int color, unsigned long tflags);
void mswin_display_screen_text(const char* text, const char* supertext, const char* subtext, int style, int attr, int color, unsigned long tflags);
void mswin_display_popup_text(const char* text, const char* title, int style, int attr, int color, int glyph, unsigned long tflags);
void mswin_display_gui_effect(int x, int y, int style, unsigned long tflags);
void mswin_update_cursor(int style, int force_paint, int show_on_u);
int mswin_ui_has_input(VOID_ARGS);
void mswin_exit_hack(int status);

/* helper function */
HWND mswin_hwnd_from_winid(winid wid);
winid mswin_winid_from_type(int type);
winid mswin_winid_from_handle(HWND hWnd);
void mswin_window_mark_dead(winid wid);
void bail(const char *mesg);

void mswin_popup_display(HWND popup, int *done_indicator, boolean close_on_any_key);
void mswin_popup_destroy(HWND popup);

void mswin_read_reg(void);
void mswin_destroy_reg(void);
void mswin_write_reg(void);

void mswin_get_window_placement(int type, LPRECT rt);
void mswin_update_window_placement(int type, LPRECT rt);
void mswin_apply_window_style(HWND hwnd);

void mswin_init_platform(VOID_ARGS);
void mswin_exit_platform(int);
void convertUTF8toTCHAR(char*, size_t);

int NHMessageBox(HWND hWnd, LPCTSTR text, UINT type);

extern HBRUSH menu_bg_brush;
extern HBRUSH menu_fg_brush;
extern HBRUSH text_bg_brush;
extern HBRUSH text_fg_brush;
extern HBRUSH status_bg_brush;
extern HBRUSH status_fg_brush;
extern HBRUSH message_bg_brush;
extern HBRUSH message_fg_brush;

extern COLORREF menu_bg_color;
extern COLORREF menu_fg_color;
extern COLORREF text_bg_color;
extern COLORREF text_fg_color;
extern COLORREF status_bg_color;
extern COLORREF status_fg_color;
extern COLORREF message_bg_color;
extern COLORREF message_fg_color;

#define SYSCLR_TO_BRUSH(x) ((HBRUSH)(((intptr_t) x) + 1))

/* unicode stuff */
#define NH_CODEPAGE (SYMHANDLING(H_IBM) ? GetOEMCP() : GetACP())
#ifdef _UNICODE
#define NH_W2A(w, a, cb) \
    (WideCharToMultiByte(NH_CODEPAGE, 0, (w), -1, (a), (cb), NULL, NULL), (a))

#define NH_A2W(a, w, cb) \
    (MultiByteToWideChar(NH_CODEPAGE, 0, (a), -1, (w), (cb)), (w))
#else
#define NH_W2A(w, a, cb) (strncpy((a), (w), (cb)))

#define NH_A2W(a, w, cb) (strncpy((w), (a), (cb)))
#endif

/* map mode macros */
#define IS_MAP_FIT_TO_SCREEN(mode)          \
    ((mode) == MAP_MODE_ASCII_FIT_TO_SCREEN \
     || (mode) == MAP_MODE_TILES_FIT_TO_SCREEN)

#define IS_MAP_ASCII(mode) \
    ((mode) != MAP_MODE_TILES && (mode) != MAP_MODE_TILES_FIT_TO_SCREEN)


extern struct animation_timer_list animation_timers;


#endif /* WINMS_H */

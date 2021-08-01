/* GnollHack 4.0    wsdlproc.h    $NHDT-Date: 1434804346 2015/06/20 12:45:46 $  $NHDT-Branch: win32-x64-working $:$NHDT-Revision: 1.41 $ */
/* Copyright (C) 2001 by Alex Kompel */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SDLPROC_H
#define SDLPROC_H

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

#if defined(SDL_GRAPHICS)
#include <SDL.h>
#if defined(GL2_GRAPHICS)
#include <SDL_opengl.h>
#elif defined(GLES2_GRAPHICS)
#include <SDL_opengles2.h>
#endif
#endif

#define SDL_TILE_SHEET_IDX(ntile) \
    (min(GetGHSdlApp()->mapTileSheets - 1, max(0, (ntile / NUM_TILES_PER_SHEET))))
#define SDL_TILEBMP_X(ntile) \
    (((ntile % NUM_TILES_PER_SHEET) % GetGHSdlApp()->mapTilesPerLine[SDL_TILE_SHEET_IDX(ntile)]) * GetGHSdlApp()->mapTile_X)
#define SDL_TILEBMP_Y(ntile) \
    (((ntile % NUM_TILES_PER_SHEET) / GetGHSdlApp()->mapTilesPerLine[SDL_TILE_SHEET_IDX(ntile)]) * GetGHSdlApp()->mapTile_Y)


typedef struct sdl_ghwindow_app {
    /* Platform */
    HINSTANCE hApp;
    SDL_Window* win;
    int win_width, win_height;
    int running;
    HBITMAP bmpTiles[MAX_TILE_SHEETS];
    HBITMAP bmpMapTiles[MAX_TILE_SHEETS];
    int mapTile_X;       /* tile width */
    int mapTile_Y;       /* tile height */
    int totalMapTiles;
    int mapTilesPerLine[MAX_TILE_SHEETS]; /* number of tile per row in the bitmap */
    int mapTileLines[MAX_TILE_SHEETS];
    int mapTileSheets;

} GHSdlApp, * PGHSdlApp;


extern PGHSdlApp GetGHSdlApp(void);
#if defined(NUKLEAR_GRAPHICS)
extern struct window_procs nuklear_procs;
#endif

int SDL_NHMessageBox(HWND hWnd, LPCTSTR text, UINT type);
void sdl_bail(const char* mesg);

/* Some prototypes */
void sdl_init_nhwindows(int *argc, char **argv);
void sdl_player_selection(void);
void sdl_askname(void);
void sdl_get_nh_event(void);
void sdl_exit_nhwindows(const char *);
void sdl_suspend_nhwindows(const char *);
void sdl_resume_nhwindows(void);
winid sdl_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info);
void sdl_clear_nhwindow(winid wid);
void sdl_display_nhwindow(winid wid, BOOLEAN_P block);
void sdl_destroy_nhwindow(winid wid);
void sdl_curs(winid wid, int x, int y);
void sdl_putstr(winid wid, int attr, const char *text);
void sdl_putstr_ex(winid wid, int attr, const char *text, int, int);
void sdl_display_file(const char *filename, BOOLEAN_P must_exist);
void sdl_start_menu_ex(winid wid, int style);
void sdl_add_menu(winid wid, int glyph, const ANY_P *identifier,
                    CHAR_P accelerator, CHAR_P group_accel, int attr,
                    const char *str, BOOLEAN_P presel);
void sdl_add_extended_menu(winid wid, int glyph, const ANY_P* identifier, struct extended_menu_info info,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel);
void sdl_end_menu_ex(winid wid, const char *prompt, const char* subtitle);
int sdl_select_menu(winid wid, int how, MENU_ITEM_P **selected);
void sdl_update_inventory(void);
void sdl_mark_synch(void);
void sdl_wait_synch(void);
void sdl_cliparound(int x, int y, BOOLEAN_P force);
void sdl_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers);
void sdl_init_print_glyph(int initid);
void sdl_raw_print(const char *str);
void sdl_raw_print_bold(const char *str);
void sdl_raw_print_flush();
int sdl_nhgetch(void);
int sdl_nh_poskey(int *x, int *y, int *mod);
void sdl_nhbell(void);
int sdl_doprev_message(void);
char sdl_yn_function_ex(int attr, int color, const char *question, const char *choices, CHAR_P def);
void sdl_getlin_ex(int attr, int color, const char *question, char *input);
int sdl_get_ext_cmd(void);
void sdl_number_pad(int state);
void sdl_delay_output(void);
void sdl_delay_output_milliseconds(int interval);
void sdl_delay_output_intervals(int intervals);
void sdl_change_color(void);
char *sdl_get_color_string(void);
void sdl_start_screen(void);
void sdl_end_screen(void);
void sdl_outrip(winid wid, int how, time_t when);
void sdl_preference_update(const char *pref);
char *sdl_getmsghistory(BOOLEAN_P init);
void sdl_putmsghistory(const char *msg, BOOLEAN_P);

void sdl_status_init(void);
void sdl_statuslines_init(void);
void sdl_status_finish(void);
void sdl_status_enablefield(int fieldidx, const char *nm, const char *fmt,
                              boolean enable);
void sdl_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long *colormasks);
void sdl_stretch_window(void);
void sdl_set_animation_timer_interval(unsigned int);
void sdl_open_special_view(struct special_view_info info);
void sdl_stop_all_sounds(struct stop_all_info info);
void sdl_play_immediate_ghsound(struct ghsound_immediate_info info);
void sdl_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info);
void sdl_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info);
void sdl_set_effect_ambient_volume(struct effect_ambient_volume_info info);
void sdl_play_ghsound_music(struct ghsound_music_info info);
void sdl_play_ghsound_level_ambient(struct ghsound_level_ambient_info info);
void sdl_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info);
void sdl_adjust_ghsound_general_volumes(VOID_ARGS);
void sdl_add_ambient_ghsound(struct soundsource_t* soundsource);
void sdl_delete_ambient_ghsound(struct soundsource_t* soundsource);
void sdl_set_ambient_ghsound_volume(struct soundsource_t* soundsource);
void sdl_clear_context_menu(VOID_ARGS);
void sdl_add_context_menu(int cmd_def_char, int cmd_cur_char, int dir, int glyph, const char* cmd_text, const char* target_text, int attr, int color);
void sdl_toggle_animation_timer(int timertype, int timerid, int state, int x, int y, int layer, int tflags);
void sdl_display_floating_text(int x, int y, const char* text, int style, int attr, int color, unsigned long tflags);
void sdl_display_screen_text(const char* text, const char* subtext, int style, int attr, int color, unsigned long tflags);
void sdl_update_cursor(int style, int force_paint, int show_on_you);
void sdl_exit_hack(int status);

/* helper function */
HWND sdl_hwnd_from_winid(winid wid);
winid sdl_winid_from_type(int type);
winid sdl_winid_from_handle(HWND hWnd);
void sdl_window_mark_dead(winid wid);
void bail(const char *mesg);

void sdl_popup_display(HWND popup, int *done_indicator);
void sdl_popup_destroy(HWND popup);

void sdl_read_reg(void);
void sdl_destroy_reg(void);
void sdl_write_reg(void);

void sdl_get_window_placement(int type, LPRECT rt);
void sdl_update_window_placement(int type, LPRECT rt);
void sdl_apply_window_style(HWND hwnd);

void sdl_init_platform(VOID_ARGS);
void sdl_exit_platform(int);

#endif /* SDLPROC_H */

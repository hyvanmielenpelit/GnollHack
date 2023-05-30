/*
 * dllproc.h
 * Copyright (c) Janne Gustafsson, 2021
 */
 /* GnollHack may be freely redistributed.  See license for details. */

#ifndef DLLPROC_H
#define DLLPROC_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "hack.h"
#include "color.h"
#include "soundfx.h"
#include "lev.h"

/* Create an array to keep track of the various windows */


/* unicode stuff */
#ifdef _UNICODE
#define GNH_W2A(w, a, cb) \
    (WideCharToMultiByte(NH_CODEPAGE, 0, (w), -1, (a), (cb), NULL, NULL), (a))

#define GNH_A2W(a, w, cb) \
    (MultiByteToWideChar(NH_CODEPAGE, 0, (a), -1, (w), (cb)), (w))
#else
#define GNH_W2A(w, a, cb) (strncpy((a), (w), (cb)))

#define GNH_A2W(a, w, cb) (strncpy((w), (a), (cb)))
#endif

#define E extern

E struct window_procs dll_procs;

#undef E

/* Some prototypes */
void dll_init_nhwindows(int *argc, char **argv);
void dll_player_selection(void);
void dll_askname(void);
void dll_get_nh_event(void);
void dll_exit_nhwindows(const char *);
void dll_suspend_nhwindows(const char *);
void dll_resume_nhwindows(void);
winid dll_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info);
void dll_clear_nhwindow(winid wid);
void dll_display_nhwindow(winid wid, BOOLEAN_P block);
void dll_destroy_nhwindow(winid wid);
void dll_curs(winid wid, int x, int y);
void dll_putstr(winid wid, int attr, const char *text);
void dll_putstr_ex(winid wid, int attr, const char *text, int, int);
void dll_putstr_ex2(winid wid, const char* text, const char* attrs, const char* colors, int attr, int color, int app);
void dll_display_file(const char *filename, BOOLEAN_P must_exist);
void dll_start_menu_ex(winid wid, int style);
void dll_add_menu(winid wid, int glyph, const ANY_P *identifier,
                    CHAR_P accelerator, CHAR_P group_accel, int attr, int color,
                    const char *str, BOOLEAN_P presel);
void dll_add_extended_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr, int color,
    const char* str, BOOLEAN_P presel, struct extended_menu_info info);
void dll_end_menu_ex(winid wid, const char *prompt, const char* subtitle);
int dll_select_menu(winid wid, int how, MENU_ITEM_P **selected);
void dll_update_inventory(void);
void dll_mark_synch(void);
void dll_wait_synch(void);
void dll_cliparound(int x, int y, BOOLEAN_P force);
void dll_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers);
void dll_issue_gui_command(int cmd_id, int cmd_param, const char* cmd_str);
void dll_raw_print(const char *str);
void dll_raw_print_bold(const char *str);
void dll_raw_print_flush();
int dll_nhgetch(void);
int dll_nh_poskey(int *x, int *y, int *mod);
void dll_nhbell(void);
int dll_doprev_message(void);
char dll_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char *question, const char *choices, CHAR_P def, const char* resp_desc, const char* introline, unsigned long ynflags);
void dll_getlin_ex(int style, int attr, int color, const char *question, char *input, const char* placeholder, const char* linesuffix, const char* introline);
int dll_get_ext_cmd(void);
void dll_number_pad(int state);
void dll_delay_output(void);
void dll_delay_output_milliseconds(int interval);
void dll_delay_output_intervals(int intervals);
void dll_change_color(void);
char *dll_get_color_string(void);
void dll_start_screen(void);
void dll_end_screen(void);
void dll_outrip(winid wid, int how, time_t when);
void dll_preference_update(const char *pref);
char *dll_getmsghistory_ex(char** attrs_ptr, char** colors_ptr, BOOLEAN_P init);
void dll_putmsghistory_ex(const char *msg, const char* attrs, const char* colors, BOOLEAN_P restoring);

void dll_status_init(int);
void dll_status_finish(void);
void dll_status_enablefield(int fieldidx, const char *nm, const char *fmt,
                              boolean enable);
void dll_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long *colormasks);
void dll_stretch_window(void);
void dll_set_animation_timer_interval(unsigned int);
int dll_open_special_view(struct special_view_info info);
void dll_stop_all_sounds(struct stop_all_info info);
void dll_play_immediate_ghsound(struct ghsound_immediate_info info);
void dll_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info);
void dll_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info);
void dll_set_effect_ambient_volume(struct effect_ambient_volume_info info);
void dll_play_ghsound_music(struct ghsound_music_info info);
void dll_play_ghsound_level_ambient(struct ghsound_level_ambient_info info);
void dll_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info);
void dll_adjust_ghsound_general_volumes(VOID_ARGS);
void dll_add_ambient_ghsound(struct soundsource_t* soundsource);
void dll_delete_ambient_ghsound(struct soundsource_t* soundsource);
void dll_set_ambient_ghsound_volume(struct soundsource_t* soundsource);
void dll_clear_context_menu(VOID_ARGS);
void dll_add_context_menu(int cmd_def_char, int cmd_cur_char, int style, int glyph, const char* cmd_text, const char* target_text, int attr, int color);
void dll_update_status_button(int cmd, int btn, int val, unsigned long bflags);
void dll_toggle_animation_timer(int timertype, int timerid, int state, int x, int y, int layer, unsigned long tflags);
void dll_display_floating_text(int x, int y, const char* text, int style, int attr, int color, unsigned long tflags);
void dll_display_screen_text(const char* text, const char* supertext, const char* subtext, int style, int attr, int color, unsigned long tflags);
void dll_display_popup_text(const char* text, const char* title, int style, int attr, int color, int glyph, unsigned long tflags);
void dll_display_gui_effect(int x, int y,int style, unsigned long tflags);
void dll_update_cursor(int style, int force_paint, int show_on_you);
int dll_ui_has_input(VOID_ARGS);
void dll_exit_hack(int status);

/* Helper functions */
void dll_bail(const char *mesg);
void dll_init_platform(VOID_ARGS);
void dll_exit_platform(int);
int GNHMessageBox(char* text, unsigned int type);


#endif /* DLLPROC_H */

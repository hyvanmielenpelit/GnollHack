/*
    libproc.h
    Copyright (c) Janne Gustafsson, 2022
*/
#ifndef LIBPROC_H
#define LIBPROC_H

extern struct window_procs lib_procs;

/* Prototypes */
void lib_init_nhwindows(int* argc, char** argv);
void lib_player_selection(void);
void lib_askname(void);
void lib_get_nh_event(void);
void lib_exit_nhwindows(const char*);
void lib_suspend_nhwindows(const char*);
void lib_resume_nhwindows(void);
winid lib_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info);
void lib_clear_nhwindow(winid wid);
void lib_display_nhwindow(winid wid, BOOLEAN_P block);
void lib_destroy_nhwindow(winid wid);
void lib_curs(winid wid, int x, int y);
void lib_putstr_ex(winid wid, int attr, const char* text, int app, int color);
void lib_putstr_ex2(winid wid,const char* text, const char* attrs, const char* colors, int attr, int color, int append);
void lib_display_file(const char* filename, BOOLEAN_P must_exist);
void lib_start_menu_ex(winid wid, int style);
void lib_add_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel);
void lib_add_extended_menu(winid wid, int glyph, const ANY_P* identifier, struct extended_menu_info info,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel);
void lib_end_menu_ex(winid wid, const char* prompt, const char* subtitle);
int lib_select_menu(winid wid, int how, MENU_ITEM_P** selected);
void lib_update_inventory(void);
void lib_mark_synch(void);
void lib_wait_synch(void);
void lib_cliparound(int x, int y, BOOLEAN_P force);
void lib_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers);
void lib_issue_gui_command(int initid);
void lib_raw_print(const char* str);
void lib_raw_print_bold(const char* str);
int lib_nhgetch(void);
int lib_nh_poskey(int* x, int* y, int* mod);
void lib_nhbell(void);
int lib_doprev_message(void);
char lib_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char* question, const char* choices, CHAR_P def, const char* resp_desc, const char* introline, unsigned long ynflags);
void lib_getlin_ex(int style, int attr, int color, const char* question, char* input, const char* placeholder, const char* linesuffix, const char* introline);
int lib_get_ext_cmd(void);
void lib_number_pad(int state);
void lib_delay_output(void);
void lib_delay_output_milliseconds(int interval);
void lib_delay_output_intervals(int intervals);
void lib_change_color(int, long, int);
char* lib_change_background(void);
char* lib_get_color_string(void);
void lib_start_screen(void);
void lib_end_screen(void);
void lib_outrip(winid wid, int how, time_t when);
void lib_preference_update(const char* pref);
char* lib_getmsghistory_ex(char** attrs_ptr, char** colors_ptr, BOOLEAN_P init);
void lib_putmsghistory_ex(const char* msg, const char* attrs, const char* colors, BOOLEAN_P restoring);

void lib_status_init(int);
void lib_status_finish(void);
void lib_status_enablefield(int fieldidx, const char* nm, const char* fmt, BOOLEAN_P enable);
void lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks);
void lib_stretch_window(void);
void lib_set_animation_timer_interval(unsigned int);
int lib_open_special_view(struct special_view_info info);
void lib_stop_all_sounds(struct stop_all_info info);
void lib_play_immediate_ghsound(struct ghsound_immediate_info info);
void lib_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info);
void lib_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info);
void lib_set_effect_ambient_volume(struct effect_ambient_volume_info info);
void lib_play_ghsound_music(struct ghsound_music_info info);
void lib_play_ghsound_level_ambient(struct ghsound_level_ambient_info info);
void lib_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info);
void lib_adjust_ghsound_general_volumes(VOID_ARGS);
void lib_add_ambient_ghsound(struct soundsource_t* soundsource);
void lib_delete_ambient_ghsound(struct soundsource_t* soundsource);
void lib_set_ambient_ghsound_volume(struct soundsource_t* soundsource);
void lib_clear_context_menu(VOID_ARGS);
void lib_add_context_menu(int cmd_def_char, int cmd_cur_char, int style, int glyph, const char* cmd_text, const char* target_text, int attr, int color);
void lib_update_status_button(int cmd, int btn, int val, unsigned long bflags);
void lib_toggle_animation_timer(int timertype, int timerid, int state, int x, int y, int layer, unsigned long tflags);
void lib_display_floating_text(int x, int y, const char* text, int style, int attr, int color, unsigned long tflags);
void lib_display_screen_text(const char* text, const char* supertext, const char* subtext, int style, int attr, int color, unsigned long tflags);
void lib_display_popup_text(const char* text, const char* title, int style, int attr, int color, int glyph, unsigned long tflags);
void lib_display_gui_effect(int x, int y, int style, unsigned long tflags);
void lib_update_cursor(int style, int force_paint, int show_on_u);
int lib_ui_has_input(VOID_ARGS);
void lib_exit_hack(int status);

/* Helper functions */
void lib_bail(const char* mesg);
void lib_init_platform(VOID_ARGS);
void lib_exit_platform(int);
void libdef_raw_print(const char*);
void libdef_raw_print_bold(const char*);
void libdef_wait_synch(VOID_ARGS);

#endif
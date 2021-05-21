/*
 * libproc.c  
 * Copyright (c) Janne Gustafsson, 2021 
 */

#include "hack.h"
#include "libproc.h"
#include "callback.h"

struct window_procs lib_procs = {
    "LIB",
    WC_COLOR | WC_HILITE_PET | WC_ALIGN_MESSAGE | WC_ALIGN_STATUS | WC_INVERSE
        | WC_SCROLL_AMOUNT | WC_SCROLL_MARGIN | WC_MAP_MODE | WC_FONT_MESSAGE
        | WC_FONT_STATUS | WC_FONT_MENU | WC_FONT_TEXT | WC_FONT_MAP
        | WC_FONTSIZ_MESSAGE | WC_FONTSIZ_STATUS | WC_FONTSIZ_MENU
        | WC_FONTSIZ_TEXT | WC_TILE_WIDTH | WC_TILE_HEIGHT | WC_TILE_FILE
        | WC_VARY_MSGCOUNT | WC_WINDOWCOLORS | WC_PLAYER_SELECTION
        | WC_SPLASH_SCREEN | WC_POPUP_DIALOG | WC_MOUSE_SUPPORT,
#ifdef STATUS_HILITES
    WC2_HITPOINTBAR | WC2_FLUSH_STATUS | WC2_RESET_STATUS | WC2_HILITE_STATUS |
#endif
    WC2_PREFERRED_SCREEN_SCALE, lib_init_nhwindows, lib_player_selection, lib_askname,
    lib_get_nh_event, lib_exit_nhwindows, lib_suspend_nhwindows,
    lib_resume_nhwindows, lib_create_nhwindow, lib_clear_nhwindow,
    lib_display_nhwindow, lib_destroy_nhwindow, lib_curs, lib_putstr,
    genl_putmixed, lib_display_file, lib_start_menu, lib_add_menu, lib_add_extended_menu,
    lib_end_menu, lib_select_menu,
    genl_message_menu, /* no need for X-specific handling */
    lib_update_inventory, lib_mark_synch, lib_wait_synch,
#ifdef CLIPPING
    lib_cliparound,
#endif
#ifdef POSITIONBAR
    donull,
#endif
    lib_print_glyph, lib_init_print_glyph, lib_raw_print, lib_raw_print_bold, lib_nhgetch,
    lib_nh_poskey, lib_nhbell, lib_doprev_message, lib_yn_function,
    lib_getlin, lib_get_ext_cmd, lib_number_pad, lib_delay_output, lib_delay_output_milliseconds, lib_delay_output_intervals,
#ifdef CHANGE_COLOR /* only a Mac option currently */
    lib_change_color, lib_change_background,
#endif
    /* other defs that really should go away (they're tty specific) */
    lib_start_screen, lib_end_screen, lib_outrip,
    lib_preference_update, lib_getmsghistory, lib_putmsghistory,
    lib_status_init, lib_status_finish, lib_status_enablefield,
    lib_status_update,
    genl_can_suspend_yes,
    lib_stretch_window,
    lib_set_animation_timer,
    lib_open_special_view,
    lib_stop_all_sounds,
    lib_play_immediate_ghsound,
    lib_play_ghsound_occupation_ambient,
    lib_play_ghsound_effect_ambient,
    lib_set_effect_ambient_volume,
    lib_play_ghsound_music,
    lib_play_ghsound_level_ambient,
    lib_play_ghsound_environment_ambient,
    lib_adjust_ghsound_general_volumes,
    lib_add_ambient_ghsound,
    lib_delete_ambient_ghsound,
    lib_set_ambient_ghsound_volume,
    lib_exit_hack,
};

struct callback_procs lib_callbacks = { 0 }; /* To be set by RunGnollHack in gnollhackdroid.c */


/* Function definitions */
void lib_init_nhwindows(int* argc, char** argv)
{

}

void lib_player_selection(void)
{

}

void lib_askname(void)
{

}

void lib_get_nh_event(void)
{

}

void lib_exit_nhwindows(const char* param)
{

}

void lib_suspend_nhwindows(const char* param)
{

}

void lib_resume_nhwindows(void)
{

}

winid lib_create_nhwindow(int type)
{
    return 0;
}

void lib_clear_nhwindow(winid wid)
{

}

void lib_display_nhwindow(winid wid, BOOLEAN_P block)
{

}

void lib_destroy_nhwindow(winid wid)
{

}

void lib_curs(winid wid, int x, int y)
{

}

void lib_putstr(winid wid, int attr, const char* text)
{

}

void lib_putstr_ex(winid wid, int attr, const char* text, int param)
{

}

void lib_display_file(const char* filename, BOOLEAN_P must_exist)
{

}

void lib_start_menu(winid wid)
{

}

void lib_add_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel)
{

}

void lib_add_extended_menu(winid wid, int glyph, const ANY_P* identifier, struct extended_menu_info info,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel)
{

}

void lib_end_menu(winid wid, const char* prompt)
{

}

int lib_select_menu(winid wid, int how, MENU_ITEM_P** selected)
{
    return 0;
}

void lib_update_inventory(void)
{

}

void lib_mark_synch(void)
{

}

void lib_wait_synch(void)
{
    libdef_wait_synch();
}

void lib_cliparound(int x, int y)
{

}

void lib_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers)
{

}

void lib_init_print_glyph(int initid)
{

}

void lib_raw_print(const char* str)
{
    libdef_raw_print(str);
}

void lib_raw_print_bold(const char* str)
{
    libdef_raw_print_bold(str);
}

void lib_raw_print_flush()
{

}

int lib_nhgetch(void)
{
    return '\033';
}

int lib_nh_poskey(int* x, int* y, int* mod)
{
    return '\033';
}

void lib_nhbell(void)
{

}

int lib_doprev_message(void)
{
    return 0;
}

char lib_yn_function(const char* question, const char* choices, CHAR_P def)
{
    return 'y';
}

void lib_getlin(const char* question, char* input)
{

}

int lib_get_ext_cmd(void)
{
    return 0;
}


void lib_number_pad(int state)
{

}


void lib_delay_output(void)
{

}

void lib_delay_output_milliseconds(int interval)
{

}


void lib_delay_output_intervals(int intervals)
{

}

void lib_change_color(int param1, long param2, int param3)
{

}

char* lib_change_background(void)
{
    return "";
}

char* lib_get_color_string(void)
{
    return "";
}

void lib_start_screen(void)
{

}

void lib_end_screen(void)
{

}

void lib_outrip(winid wid, int how, time_t when)
{

}

void lib_preference_update(const char* pref)
{

}

char* lib_getmsghistory(BOOLEAN_P init)
{
    return "";
}

void lib_putmsghistory(const char* msg, BOOLEAN_P param)
{

}

void lib_status_init(void)
{

}

void lib_status_finish(void)
{

}

void lib_status_enablefield(int fieldidx, const char* nm, const char* fmt,
    int enable)
{

}

void lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks)
{

}

void lib_stretch_window(void)
{

}

void lib_set_animation_timer(unsigned int param)
{

}

void lib_open_special_view(struct special_view_info info)
{

}

void lib_stop_all_sounds(struct stop_all_info info)
{

}

void lib_play_immediate_ghsound(struct ghsound_immediate_info info)
{

}

void lib_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info)
{

}

void lib_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info)
{

}

void lib_set_effect_ambient_volume(struct effect_ambient_volume_info info)
{

}

void lib_play_ghsound_music(struct ghsound_music_info info)
{

}

void lib_play_ghsound_level_ambient(struct ghsound_level_ambient_info info)
{

}

void lib_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info)
{

}

void lib_adjust_ghsound_general_volumes(VOID_ARGS)
{

}

void lib_add_ambient_ghsound(struct soundsource_t* soundsource)
{

}

void lib_delete_ambient_ghsound(struct soundsource_t* soundsource)
{

}

void lib_set_ambient_ghsound_volume(struct soundsource_t* soundsource)
{

}

void lib_exit_hack(int status)
{

}


/* Helper functions */
void lib_bail(const char* mesg)
{

}

void lib_init_platform(VOID_ARGS)
{

}

void lib_exit_platform(int status)
{

}







#ifdef USER_SOUNDS

void play_usersound(const char* filename, int volume)
{
	return;
}

#endif

void set_username()
{
	strcpy(plname, "Adventurer");
}


void
libdef_raw_print(const char* s)
{
    puts(s);
}

void
libdef_raw_print_bold(const char* s)
{
    puts(s);
}

void
libdef_wait_synch(VOID_ARGS)
{
    /* Config file error handling routines
     * call wait_sync() without checking to
     * see if it actually has a value,
     * leading to spectacular violations
     * when you try to execute address zero.
     * The existence of this allows early
     * processing to have something to execute
     * even though it essentially does nothing
     */
    return;
}

/*
 * libproc.c  
 * Copyright (c) Janne Gustafsson, 2021 
 */

#include "hack.h"
#include "libproc.h"
#include "callback.h"
#include "dlb.h"

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

char convert_gnhch(int ch);


/* Function definitions */
void lib_init_nhwindows(int* argc, char** argv)
{
    lib_callbacks.callback_init_nhwindows();
}

void lib_player_selection(void)
{
    int res = common_player_selection();
    if (res == 1)
    {
        res = lib_callbacks.callback_player_selection();
        if (res != 0)
            lib_bail((char*)0);
    }
    else if (res == 2)
    {
        lib_bail((char*)0);
    }
}

void lib_askname(void)
{
    char* name = lib_callbacks.callback_askname();
    strcpy(plname, name);
}

void lib_get_nh_event(void)
{
    lib_callbacks.callback_get_nh_event();
}

void lib_exit_nhwindows(const char* param)
{
    lib_callbacks.callback_exit_nhwindows(param);
}

void lib_suspend_nhwindows(const char* param)
{
    lib_callbacks.callback_suspend_nhwindows(param);
}

void lib_resume_nhwindows(void)
{
    lib_callbacks.callback_resume_nhwindows();
}

winid lib_create_nhwindow(int type)
{
    return lib_callbacks.callback_create_nhwindow(type);
}

void lib_clear_nhwindow(winid wid)
{
    lib_callbacks.callback_clear_nhwindow(wid);
}

void lib_display_nhwindow(winid wid, BOOLEAN_P block)
{
    lib_callbacks.callback_display_nhwindow(wid, block);
}

void lib_destroy_nhwindow(winid wid)
{
    lib_callbacks.callback_destroy_nhwindow(wid);
    return;
}

void lib_curs(winid wid, int x, int y)
{
    lib_callbacks.callback_curs(wid, x, y);
}

void lib_putstr(winid wid, int attr, const char* text)
{
    lib_callbacks.callback_putstr_ex(wid, attr, text, 0);
}

void lib_putstr_ex(winid wid, int attr, const char* text, int param)
{
    lib_callbacks.callback_putstr_ex(wid, attr, text, param);
}

void lib_display_file(const char* filename, BOOLEAN_P must_exist)
{
    dlb* f;
    //lib_callbacks.callback_display_file(filename, must_exist);

    f = dlb_fopen(filename, RDTMODE);
    if (!f) 
    {
        if (must_exist) 
        {
            char* message[BUFSZ];
            Sprintf(message, "Warning! Could not find file: %s", filename);
            raw_print(message);
        }
    }
    else 
    {
        winid text;
#define LLEN 128
        char line[LLEN];

        text = lib_create_nhwindow(NHW_TEXT);

        while (dlb_fgets(line, LLEN, f)) 
        {
            size_t len;
            len = strlen(line);
            if (line[len - 1] == '\n')
                line[len - 1] = '\x0';
            lib_putstr(text, ATR_NONE, line);
        }
        (void)dlb_fclose(f);

        lib_display_nhwindow(text, 1);
        lib_destroy_nhwindow(text);
    }

}

void lib_start_menu(winid wid)
{
    lib_callbacks.callback_start_menu(wid);
}

void lib_add_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel)
{
    
    lib_callbacks.callback_add_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr, str, presel);
}

void lib_add_extended_menu(winid wid, int glyph, const ANY_P* identifier, struct extended_menu_info info,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel)
{
    lib_add_menu(wid, glyph, identifier, accelerator, group_accel, attr, str, presel);
}

void lib_end_menu(winid wid, const char* prompt)
{
    lib_callbacks.callback_end_menu(wid, prompt);
}

int lib_select_menu(winid wid, int how, MENU_ITEM_P** selected)
{
    long long* picklist = 0;
    int picklistsize = 0;
    int cnt = lib_callbacks.callback_select_menu(wid, how, &picklist, &picklistsize);
    int i;

    if (!selected)
        return 0;

    *selected = 0;

    if (cnt > 0)
    {
        if (!picklist)
            return -1;

        *selected = (MENU_ITEM_P*)malloc(sizeof(MENU_ITEM_P) * cnt);
        for (i = 0; i < cnt; i++)
        {
            (*selected)[i].item.a_longlong = *picklist++;;
            (*selected)[i].count = *picklist++;
        }
    }

    lib_callbacks.callback_free_memory(&picklist);

    return cnt;
}

void lib_update_inventory(void)
{
    lib_callbacks.callback_update_inventory();
}

void lib_mark_synch(void)
{
    lib_callbacks.callback_mark_synch();
}

void lib_wait_synch(void)
{
    libdef_wait_synch();
}

void lib_cliparound(int x, int y)
{
    lib_callbacks.callback_cliparound(x, y);
}

extern const nhsym cp437toUnicode[256];

void lib_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers)
{
    long symbol;
    nhsym sym = 0;
    int ocolor = 0;
    unsigned long special = 0UL;
    int res = mapglyph(layers, &sym, &ocolor, &special, x, y);
    symbol = SYMHANDLING(H_IBM) && sym >= 0 && sym < 256 ? (long)cp437toUnicode[sym] : (long)sym;
    lib_callbacks.callback_print_glyph(wid, x, y, layers.glyph, layers.bkglyph, symbol, ocolor, special);
}

void lib_init_print_glyph(int initid)
{
    lib_callbacks.callback_init_print_glyph(initid);
}

void lib_raw_print(const char* str)
{
    lib_callbacks.callback_raw_print(str);
}

void lib_raw_print_bold(const char* str)
{
    lib_callbacks.callback_raw_print_bold(str);
}

int lib_nhgetch(void)
{
    return (int)convert_gnhch(lib_callbacks.callback_nhgetch());
}

int lib_nh_poskey(int* x, int* y, int* mod)
{
    return (int)convert_gnhch(lib_callbacks.callback_nh_poskey(x, y, mod));
}

void lib_nhbell(void)
{
    lib_callbacks.callback_nhbell();
}

int lib_doprev_message(void)
{
    return 0;
}

char lib_yn_function(const char* question, const char* choices, CHAR_P def)
{
    char defs[2] = { 0,0 };
    defs[0] = def;
    return convert_gnhch(lib_callbacks.callback_yn_function(question, choices, defs));
}

void lib_getlin(const char* question, char* input)
{
    char* res = lib_callbacks.callback_getlin(question);
    if(res && input) 
       strcpy(input, res);
}

int lib_get_ext_cmd(void)
{
    char* res = lib_callbacks.callback_getlin("Type an Extended Command");
    if (!res)
        return -1;

    char buf[BUFSZ];
    strncpy(buf, res, BUFSZ - 1);
    buf[BUFSZ - 1] = 0;
    mungspaces(buf);
    int extcmd = ext_cmd_from_txt(buf);
    if (extcmd == -1)
        pline("'%s' is an invalid extended command.", buf);

    return extcmd;
}


void lib_number_pad(int state)
{
    return;
}


void lib_delay_output(void)
{
    lib_callbacks.callback_delay_output();
}

void lib_delay_output_milliseconds(int interval)
{
    lib_callbacks.callback_delay_output_milliseconds(interval);
}


void lib_delay_output_intervals(int intervals)
{
    lib_callbacks.callback_delay_output_intervals(intervals);
}

void lib_change_color(int param1, long param2, int param3)
{
    return;
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
    return;
}

void lib_end_screen(void)
{
    return;
}

void lib_outrip(winid wid, int how, time_t when)
{
    return;
}

void lib_preference_update(const char* pref)
{
    lib_callbacks.callback_preference_update(pref);
}

char* lib_getmsghistory(BOOLEAN_P init)
{
    return (char*)0;
}

void lib_putmsghistory(const char* msg, BOOLEAN_P param)
{
    return;
}

void lib_status_init(void)
{
    lib_callbacks.callback_status_init();
}

void lib_status_finish(void)
{
    lib_callbacks.callback_status_finish();
}

void lib_status_enablefield(int fieldidx, const char* nm, const char* fmt,
    int enable)
{
    lib_callbacks.callback_status_enablefield(fieldidx, nm, fmt, enable);
}

void lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks)
{
    //lib_callbacks.callback_status_update(idx, ptr, chg, percent, color, colormasks);
    lib_curs(WIN_MAP, 0, 0);
    char* line1 = do_statusline1();
    char* loc;
    if ((loc = strstr(line1, "\\G")) != 0)
    {
        *loc = '\0';
    }
    lib_putstr(WIN_MAP, 0, line1);
    lib_curs(WIN_MAP, 0, 1);
    char* line2 = do_statusline2();
    if ((loc = strstr(line2, "\\G")) != 0)
    {
        *loc = '\0';
    }
    lib_putstr(WIN_MAP, 0, line2);
    lib_curs(WIN_MAP, u.ux, u.uy);
}

void lib_stretch_window(void)
{
    return;
}

void lib_set_animation_timer(unsigned int param)
{
    lib_callbacks.callback_set_animation_timer((unsigned long)param);
    return;
}

void lib_open_special_view(struct special_view_info info)
{
    return;
}

void lib_stop_all_sounds(struct stop_all_info info)
{
    return;
}

void lib_play_immediate_ghsound(struct ghsound_immediate_info info)
{
    return;
}

void lib_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info)
{
    return;
}

void lib_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info)
{
    return;
}

void lib_set_effect_ambient_volume(struct effect_ambient_volume_info info)
{
    return;
}

void lib_play_ghsound_music(struct ghsound_music_info info)
{
    return;
}

void lib_play_ghsound_level_ambient(struct ghsound_level_ambient_info info)
{
    return;
}

void lib_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info)
{
    return;
}

void lib_adjust_ghsound_general_volumes(VOID_ARGS)
{
    return;
}

void lib_add_ambient_ghsound(struct soundsource_t* soundsource)
{
    return;
}

void lib_delete_ambient_ghsound(struct soundsource_t* soundsource)
{
    return;
}

void lib_set_ambient_ghsound_volume(struct soundsource_t* soundsource)
{
    return;
}

void lib_exit_hack(int status)
{
    lib_callbacks.callback_exit_hack(status);
}


/* Helper functions */
void lib_bail(const char* mesg)
{
    gnollhack_exit(EXIT_SUCCESS);
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


void
set_wincaps(wincap1, wincap2)
unsigned long wincap1, wincap2;
{
    if(wincap1 > 0)
        lib_procs.wincap = wincap1;
    if (wincap2 > 0)
        lib_procs.wincap2 = wincap2;
}

char convert_gnhch(int ch)
{
    char key = 0;
    switch (ch)
    {
    case -1:
        key = Cmd.move_SW;
        break;
    case -2:
        key = Cmd.move_S;
        break;
    case -3:
        key = Cmd.move_SE;
        break;
    case -4:
        key = Cmd.move_W;
        break;
    case -6:
        key = Cmd.move_E;
        break;
    case -7:
        key = Cmd.move_NW;
        break;
    case -8:
        key = Cmd.move_N;
        break;
    case -9:
        key = Cmd.move_NE;
        break;
    default:
        key = ch;
        break;
    }

    return key;
}

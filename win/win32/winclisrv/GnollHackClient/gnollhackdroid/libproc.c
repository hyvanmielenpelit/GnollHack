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
    WC2_PREFERRED_SCREEN_SCALE | WC2_STATUSLINES | WC2_AUTOSTATUSLINES | WC2_HEREWINDOW,
    lib_init_nhwindows, lib_player_selection, lib_askname,
    lib_get_nh_event, lib_exit_nhwindows, lib_suspend_nhwindows,
    lib_resume_nhwindows, lib_create_nhwindow, lib_clear_nhwindow,
    lib_display_nhwindow, lib_destroy_nhwindow, lib_curs, lib_putstr_ex,
    genl_putmixed_ex, lib_display_file, lib_start_menu, lib_add_menu, lib_add_extended_menu,
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
    lib_nh_poskey, lib_nhbell, lib_doprev_message, lib_yn_function_ex,
    lib_getlin_ex, lib_get_ext_cmd, lib_number_pad, lib_delay_output, lib_delay_output_milliseconds, lib_delay_output_intervals,
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
    lib_clear_context_menu,
    lib_add_context_menu,
    lib_exit_hack,
};

struct callback_procs lib_callbacks = { 0 }; /* To be set by RunGnollHack in gnollhackdroid.c */

char convert_gnhch(int ch);
void __lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks);

/* Function definitions */
void lib_init_nhwindows(int* argc, char** argv)
{
    lib_callbacks.callback_init_nhwindows();
    iflags.window_inited = 1;
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
    strncpy(plname, name, PL_NSIZ - 1);
    plname[PL_NSIZ - 1] = '\0';
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

void lib_putstr_ex(winid wid, int attr, const char* text, int param, int color)
{
    char buf[BUFSIZ];
    if (text)
        write_text2buf_utf8(buf, BUFSIZ, text);
    lib_callbacks.callback_putstr_ex(wid, attr, text ? buf : 0, param, color);
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
            char message[BUFSZ];
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
            putstr(text, ATR_NONE, line);
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
    if (!str)
        return;

    char buf[BUFSIZ];
    if (str)
        write_text2buf_utf8(buf, BUFSIZ, str);
    int color = CLR_WHITE;
#ifdef TEXTCOLOR
    get_menu_coloring(str, &color, &attr);
#endif
    lib_callbacks.callback_add_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr, str ? buf : 0, presel, color);
}

void lib_add_extended_menu(winid wid, int glyph, const ANY_P* identifier, struct extended_menu_info info,
    CHAR_P accelerator, CHAR_P group_accel, int attr,
    const char* str, BOOLEAN_P presel)
{
    if (!str)
        return;

    char buf[BUFSIZ];
    if(str)
        write_text2buf_utf8(buf, BUFSIZ, str);

    int color = info.color;
#ifdef TEXTCOLOR
    get_menu_coloring(str, &color, &attr);
#endif
    lib_callbacks.callback_add_extended_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr, str ? buf : 0, presel, color, (info.object && !(info.menu_flags & MENU_FLAGS_COUNT_DISALLOWED) ? info.object->quan : 0),
        (unsigned long long)(info.object ? info.object->o_id : 0), (unsigned long long)(info.monster ? info.monster->m_id : 0), info.heading_for_group_accelerator, info.menu_flags);
}

void lib_end_menu(winid wid, const char* prompt)
{
    char buf[BUFSIZ];
    if (prompt)
        write_text2buf_utf8(buf, BUFSIZ, prompt);

    lib_callbacks.callback_end_menu(wid, prompt ? buf : 0);
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

void lib_cliparound(int x, int y, BOOLEAN_P force)
{
    lib_callbacks.callback_cliparound(x, y, force);
}

extern const nhsym cp437toUnicode[256];

void lib_print_glyph(winid wid, XCHAR_P x, XCHAR_P y, struct layer_info layers)
{
    long symbol;
    nhsym sym = 0;
    int ocolor = 0;
    unsigned long special = 0UL;
    (void)mapglyph(layers, &sym, &ocolor, &special, x, y);
    symbol = SYMHANDLING(H_IBM) && sym >= 0 && sym < 256 ? (long)cp437toUnicode[sym] : (long)sym;
    lib_callbacks.callback_print_glyph(wid, x, y, layers.glyph, layers.bkglyph, symbol, ocolor, special);
}

void lib_init_print_glyph(int initid)
{
    lib_callbacks.callback_init_print_glyph(initid);
}

void lib_raw_print(const char* str)
{
    char buf[BUFSIZ];
    if(str)
        write_text2buf_utf8(buf, BUFSIZ, str);
    lib_callbacks.callback_raw_print(str ? buf : 0);
}

void lib_raw_print_bold(const char* str)
{
    char buf[BUFSIZ];
    if(str)
        write_text2buf_utf8(buf, BUFSIZ, str);
    lib_callbacks.callback_raw_print_bold(str ? buf : 0);
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

char lib_yn_function_ex(int attr, int color, const char* question, const char* choices, CHAR_P def)
{
    char buf[BUFSIZ];
    if(question)
        write_text2buf_utf8(buf, BUFSIZ, question);
    char defs[2] = { 0,0 };
    defs[0] = def;
    int res = lib_callbacks.callback_yn_function_ex(attr, color, question ? buf : 0, choices, defs);
    return convert_gnhch(res);
}

void lib_getlin_ex(int attr, int color, const char* question, char* input)
{
    char buf[BUFSIZ];
    if (question)
        write_text2buf_utf8(buf, BUFSIZ, question);
    char* res = lib_callbacks.callback_getlin_ex(attr, color, question ? buf : 0);
    if (res && input)
    {
        char msgbuf[BUFSZ] = "";
        strcpy(msgbuf, res);
        convertUTF8toCP437(msgbuf, BUFSZ);
        strcpy(input, msgbuf);
    }
}

int lib_get_ext_cmd(void)
{
    char* res = lib_callbacks.callback_getlin_ex(ATR_NONE, NO_COLOR, "Type an Extended Command");
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
    if (!strcmp(pref, "statuslines"))
    {
        context.botl = context.botlx = 1;
    }
}

char* lib_getmsghistory(BOOLEAN_P init)
{
    char* res = lib_callbacks.callback_getmsghistory((int)init);
    static char buf[BUFSIZ] = "";
    if (res)
    {
        strncpy(buf, res, BUFSIZ - 1);
        buf[BUFSIZ - 1] = '\0';
    }
    return res ? buf : 0;
}

void lib_putmsghistory(const char* msg, BOOLEAN_P is_restoring)
{
    lib_callbacks.callback_putmsghistory(msg, is_restoring);
}




extern const char* status_fieldnm[MAXBLSTATS];
extern const char* status_fieldfmt[MAXBLSTATS];
extern boolean status_activefields[MAXBLSTATS];
extern char* status_vals[MAXBLSTATS];
static int status_colors[MAXBLSTATS];
static unsigned long* cond_hilites;
static unsigned long active_conditions;
static const char* cond_names[NUM_BL_CONDITIONS] = {
    "Stone", "Slime", "Strngl", "Suffoc", "FoodPois", "TermIll", "Blind",
    "Deaf", "Stun", "Conf", "Hallu", "Lev", "Fly", "Ride", "Slow", "Paral", "Fear", "Sleep", "Cancl", "Silent", "Grab", "Rot", "Lyca"
};

void lib_status_init(void)
{
    lib_callbacks.callback_status_init();
    genl_status_init();

}

void lib_status_finish(void)
{
    lib_callbacks.callback_status_finish();
    genl_status_finish();
}

void lib_status_enablefield(int fieldidx, const char* nm, const char* fmt,
    BOOLEAN_P enable)
{
    lib_callbacks.callback_status_enablefield(fieldidx, nm, fmt, enable);
    genl_status_enablefield(fieldidx, nm, fmt, enable);
}

void lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks)
{
    __lib_status_update(idx, ptr, chg, percent, color, colormasks);

#if 0
    char* txt = (char*)0;
    long condbits = 0L;
    if (ptr)
    {
        if (idx != BL_CONDITION)
            txt = ptr;
        else
        {
            long* bits_ptr = (long*)ptr;
            condbits = *bits_ptr;
        }
    }

    lib_callbacks.callback_status_update(idx, txt, condbits, chg, percent, color, colormasks);
#endif
}


int hl_attridx_to_attrmask(int idx)
{
    switch (idx)
    {
    case HL_ATTCLR_DIM: 	return (1 << ATR_DIM);
    case HL_ATTCLR_BLINK:	return (1 << ATR_BLINK);
    case HL_ATTCLR_ULINE:   return (1 << ATR_ULINE);
    case HL_ATTCLR_INVERSE:	return (1 << ATR_INVERSE);
    case HL_ATTCLR_BOLD:	return (1 << ATR_BOLD);
    }
    return 0;
}

int hl_attrmask_to_attrmask(int mask)
{
    int attr = 0;
    if (mask & HL_DIM) attr |= (1 << ATR_DIM);
    if (mask & HL_BLINK) attr |= (1 << ATR_BLINK);
    if (mask & HL_ULINE) attr |= (1 << ATR_ULINE);
    if (mask & HL_INVERSE) attr |= (1 << ATR_INVERSE);
    if (mask & HL_BOLD) attr |= (1 << ATR_BOLD);
    return attr;
}

void lib_set_health_color(int nhcolor)
{
    //Callback set health color    
}

void lib_bot_updated()
{
    //Callback redraw status??    
}


int get_condition_color(int cond_mask)
{
    int i;
    for (i = 0; i < CLR_MAX; i++)
        if (cond_hilites[i] & cond_mask)
            return i;
    return CLR_WHITE;
}

int get_condition_attr(int cond_mask)
{
    int i;
    int attr = 0;
    for (i = CLR_MAX; i < BL_ATTCLR_MAX; i++)
        if (cond_hilites[i] & cond_mask)
            attr |= hl_attridx_to_attrmask(i);
    return attr;
}

void print_conditions(const char** names)
{
    int i;
    for (i = 0; i < NUM_BL_CONDITIONS; i++) {
        int cond_mask = 1 << i;
        if (active_conditions & cond_mask)
        {
            const char* name = names[i];
            int color = get_condition_color(cond_mask);
            int attr = get_condition_attr(cond_mask);
            //debuglog("cond '%s' active. col=%s attr=%x", name, colname(color), attr);
            lib_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
            lib_putstr_ex(WIN_STATUS, attr, name, 0, color);
        }
    }
}

void print_status_field(int idx, boolean first_field)
{
    if (!status_activefields[idx])
        return;

    const char* val = status_vals[idx];

    if (first_field && *val == ' ')
    {
        // Remove leading space of first field
        val++;
    }
    else if (!strcmp(val, " "))
    {
        // No extra spaces if the field is just a space
        val++;
    }
    else if (idx == BL_LEVELDESC && !first_field)
    {
        /* leveldesc has no leading space, so if we've moved
           it past the first position, provide one */
        lib_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
    }
    else if (iflags.wc2_statuslines >= 3)
    {
        /* looks like first field */
        if (idx == BL_SKILL && *val == ' ')
        {
            if (!strcmp(status_vals[BL_2WEP], " "))
                val++;
        }
        else if (idx == BL_HUNGER && *val == ' ')
        {
            if (!strcmp(status_vals[BL_2WEP], " ") && !strcmp(status_vals[BL_SKILL], " "))
                val++;
        }
        else if (idx == BL_CAP && *val == ' ')
        {
            if (!strcmp(status_vals[BL_2WEP], " ") && !strcmp(status_vals[BL_SKILL], " ") && !strcmp(status_vals[BL_HUNGER], " "))
                val++;
        }
        else if (idx == BL_CONDITION && *val == ' ')
        {
            if (!strcmp(status_vals[BL_2WEP], " ") && !strcmp(status_vals[BL_SKILL], " ") && !strcmp(status_vals[BL_HUNGER], " ") && !strcmp(status_vals[BL_CAP], " "))
                val++;
        }
    }

    // Don't want coloring on leading spaces (ATR_INVERSE would show), so print those first
    while (*val == ' ')
    {
        lib_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
        val++;
    }

    if (idx == BL_CONDITION)
    {
        print_conditions(cond_names);
    }
    else
    {
        int attr = (status_colors[idx] >> 8) & 0xFF;
        int color = status_colors[idx] & 0xFF;
        if (idx == BL_HP)
        {
            lib_set_health_color(color);
        }
        else if (idx == BL_HPMAX)
        {
            // Set hp-max to same color as hp if it's not explicitly defined
            if (color == NO_COLOR && attr == ATR_NONE && status_activefields[BL_HP])
            {
                attr = (status_colors[BL_HP] >> 8) & 0xFF;
                color = status_colors[BL_HP] & 0xFF;
            }
        }
        else if (idx == BL_ENEMAX)
        {
            // Set power-max to same color as power if it's not explicitly defined
            if (color == NO_COLOR && attr == ATR_NONE && status_activefields[BL_ENE])
            {
                attr = (status_colors[BL_ENE] >> 8) & 0xFF;
                color = status_colors[BL_ENE] & 0xFF;
            }
        }
        lib_putstr_ex(WIN_STATUS, hl_attrmask_to_attrmask(attr), val, 0, color);
        //	debuglog("field %d: %s color %s", idx+1, val, colname(color));
    }
}

void lib_status_flush(void)
{
    enum statusfields idx, * fieldlist;
    register int i, j;

    static const int fieldorder1[] = { BL_TITLE, BL_STR, BL_DX,    BL_CO,    BL_IN,
                             BL_WI,    BL_CH, BL_GOLD,  /*BL_ALIGN,*/ BL_FLUSH, };
    static const int fieldorder2[] = { BL_LEVELDESC, BL_HP,   BL_HPMAX,
                                 BL_ENE,       BL_ENEMAX,    BL_AC,  BL_MC_LVL, BL_MC_PCT,    BL_MOVE, BL_UWEP, BL_UWEP2,  BL_XP,
                                 BL_EXP,       BL_HD,        BL_TIME, BL_SCORE, BL_FLUSH };
    static const int fieldorder2_2statuslines[] = { BL_LEVELDESC, BL_HP,   BL_HPMAX,
                                 BL_ENE,       BL_ENEMAX,    BL_AC,    BL_MC_LVL, BL_MC_PCT,    BL_MOVE, BL_UWEP, BL_UWEP2,   BL_XP,
                                 BL_EXP,       BL_HD,        BL_TIME,  BL_2WEP,   BL_SKILL,     BL_HUNGER, BL_CAP,  BL_CONDITION, BL_FLUSH };
    static const int fieldorder3[] = { BL_2WEP, BL_SKILL,     BL_HUNGER,
                                 BL_CAP,       BL_CONDITION, BL_FLUSH };
    static const int fieldorder4[] = { BL_PARTYSTATS, BL_FLUSH };
    static const int fieldorder5[] = { BL_PARTYSTATS2, BL_FLUSH };
    static const int fieldorder6[] = { BL_PARTYSTATS3, BL_FLUSH };
    static const int fieldorder7[] = { BL_PARTYSTATS4, BL_FLUSH };
    static const int fieldorder8[] = { BL_PARTYSTATS5, BL_FLUSH };

    static const int* fieldorders_2statuslines[MAX_STATUS_LINES + 1] = { fieldorder1, fieldorder2_2statuslines, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    static const int* fieldorders[MAX_STATUS_LINES + 1] = { fieldorder1, fieldorder2, fieldorder3, fieldorder4, fieldorder5, fieldorder6, fieldorder7, fieldorder8, NULL };

    const int** fieldorder = iflags.wc2_statuslines == 2 ? fieldorders_2statuslines : fieldorders;

    for (j = 0; fieldorder[j] != NULL && j < iflags.wc2_statuslines; j++)
    {
        curs(WIN_STATUS, 1, j);
        for (i = 0; (idx = fieldorder[j][i]) != BL_FLUSH; i++)
            print_status_field(idx, i == 0);
    }

    lib_bot_updated();
}

void __lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks)
{
    long *condptr = (long*)ptr;
    char *text = (char*)ptr;

    if (idx == BL_FLUSH || idx == BL_RESET)
    {
//        if (cond_hilites)
            lib_status_flush();
    }
    else if (status_activefields[idx])
    {
        if (idx == BL_CONDITION)
        {
            cond_hilites = colormasks;
            active_conditions = condptr ? *condptr : 0L;
            *status_vals[idx] = 0;
        }
        else
        {
            // Remove encoded glyph value. (This might break in the future if the format is changed in botl.c)
            if (idx == BL_GOLD && *text == '\\' && strlen(text) > 10)
            {
                char gbuf[BUFSZ];
                text += 10;
                Sprintf(gbuf, "$%s", text);
                Sprintf(status_vals[idx], status_fieldfmt[idx] ? status_fieldfmt[idx] : "%s", gbuf);
            }
            else
            {
                Sprintf(status_vals[idx], status_fieldfmt[idx] ? status_fieldfmt[idx] : "%s", text ? text : "");
            }
            status_colors[idx] = color;
        }
    }
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
    lib_exit_platform(status);
}


void
lib_clear_context_menu(VOID_ARGS)
{
    lib_callbacks.callback_clear_context_menu();
}

void
lib_add_context_menu(int cmd_def_char, int cmd_cur_char, int dir, int glyph, const char* cmd_text, const char* target_text, int attr, int color)
{
    lib_callbacks.callback_add_context_menu(cmd_def_char, cmd_cur_char, dir, glyph, cmd_text, target_text, attr, color);
}

/* Helper functions */
void lib_bail(const char* mesg)
{
    gnollhack_exit(EXIT_SUCCESS);
}

void lib_init_platform(VOID_ARGS)
{
    /* Tiles */
#ifdef USE_TILES
    process_tiledata(1, (const char*)0, glyph2tile, glyphtileflags);
#endif

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
    case -5:
        key = Cmd.spkeys[NHKF_GETDIR_SELF];
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
    case -10:
        key = '0';
        break;
    default:
        if (ch >= -19 && ch <= -10)
            key = abs(ch) - 10 + '0';
        else
            key = ch;
        break;
    }

    return key;
}

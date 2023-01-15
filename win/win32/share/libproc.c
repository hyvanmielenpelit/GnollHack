/*
 * libproc.c  
 * Copyright (c) Janne Gustafsson, 2022
 */

#include "hack.h"
#include "libproc.h"
#include "callback.h"
#include "dlb.h"
#include <stdarg.h>

struct window_procs lib_procs = {
    "LIB",
    WC_COLOR | WC_HILITE_PET | WC_ALIGN_MESSAGE | WC_ALIGN_STATUS | WC_INVERSE
        | WC_TILE_WIDTH | WC_TILE_HEIGHT | WC_TILE_FILE
        | WC_VARY_MSGCOUNT | WC_WINDOWCOLORS | WC_PLAYER_SELECTION
        | WC_SPLASH_SCREEN | WC_POPUP_DIALOG | WC_MOUSE_SUPPORT,
#ifdef STATUS_HILITES
    WC2_HITPOINTBAR | WC2_FLUSH_STATUS | WC2_RESET_STATUS | WC2_HILITE_STATUS |
#endif
    WC2_SELECTSAVED | WC2_STATUSLINES | WC2_HEREWINDOW | WC2_SCREEN_TEXT |
    WC2_PLAY_GHSOUNDS | WC2_PREPROCESS_REPLACEMENTS | WC2_SPECIAL_SYMBOLS | WC2_MENU_SUFFIXES,
    lib_init_nhwindows, lib_player_selection, lib_askname,
    lib_get_nh_event, lib_exit_nhwindows, lib_suspend_nhwindows,
    lib_resume_nhwindows, lib_create_nhwindow_ex, lib_clear_nhwindow,
    lib_display_nhwindow, lib_destroy_nhwindow, lib_curs, lib_putstr_ex, lib_putstr_ex2,
    genl_putmixed_ex, lib_display_file, lib_start_menu_ex, lib_add_menu, lib_add_extended_menu,
    lib_end_menu_ex, lib_select_menu,
    genl_message_menu, /* no need for X-specific handling */
    lib_update_inventory, lib_mark_synch, lib_wait_synch,
#ifdef CLIPPING
    lib_cliparound,
#endif
#ifdef POSITIONBAR
    donull,
#endif
    lib_print_glyph, lib_issue_gui_command, lib_raw_print, lib_raw_print_bold, lib_nhgetch,
    lib_nh_poskey, lib_nhbell, lib_doprev_message, lib_yn_function_ex,
    lib_getlin_ex, lib_get_ext_cmd, lib_number_pad, lib_delay_output, lib_delay_output_milliseconds, lib_delay_output_intervals,
#ifdef CHANGE_COLOR /* only a Mac option currently */
    lib_change_color, lib_change_background,
#endif
    /* other defs that really should go away (they're tty specific) */
    lib_start_screen, lib_end_screen, lib_outrip,
    lib_preference_update, lib_getmsghistory_ex, lib_putmsghistory_ex,
    lib_status_init, lib_status_finish, lib_status_enablefield,
    lib_status_update,
    genl_can_suspend_yes,
    lib_stretch_window,
    lib_set_animation_timer_interval,
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
    lib_update_status_button,
    lib_toggle_animation_timer,
    lib_display_floating_text,
    lib_display_screen_text,
    lib_display_popup_text,
    lib_display_gui_effect,
    lib_update_cursor,
    lib_ui_has_input,
    lib_exit_hack,
};

struct callback_procs lib_callbacks = { 0 }; /* To be set by RunGnollHack in gnhapi.c */

char convert_gnhch(int ch);
void __lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks);
void monst_to_info(struct monst*, struct monst_info*);
int get_condition_color(int cond_mask);


/* Function definitions */
void lib_init_nhwindows(int* argc, char** argv)
{
    struct ghsound_music_info mi = { 0 };
    mi.ghsound = GHSOUND_MUSIC_PLAYER_SELECTION;
    mi.volume = 1.0f;
    play_ghsound_music(mi);

    lib_callbacks.callback_init_nhwindows();
    iflags.window_inited = 1;
    iflags.using_gui_sounds = 1;
    iflags.using_gui_tiles = 1;
}

void lib_player_selection(void)
{
    struct ghsound_music_info mi = { 0 };
    mi.ghsound = GHSOUND_MUSIC_PLAYER_SELECTION;
    mi.volume = 1.0f;
    play_ghsound_music(mi);

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
    char* name = 0;
    char mdbuf[BUFSZ] = "";
    char modenamebuf[BUFSZ] = "";
    char modedescbuf[BUFSZ] = "";
    Sprintf(mdbuf, "%s mode", get_game_mode_text(FALSE));
    strcpy_capitalized_for_title(modenamebuf, mdbuf);
    strcpy(modedescbuf, get_game_mode_description());
    *modedescbuf = highc(*modedescbuf);

#ifdef SELECTSAVED
    if (iflags.wc2_selectsaved && !iflags.renameinprogress)
    {
        do
        {
            boolean repeataskname = FALSE;
            int menures = restore_menu(WIN_ERR);
            switch (menures)
            {
            case -1:
                lib_bail((char*)0); // "Until next time then..."); /* quit */
                /*NOTREACHED*/
                return;
            case 2:
            case 0: /* no game chosen; start new game */
                do
                {
                    repeataskname = FALSE;
                    name = lib_callbacks.callback_askname(modenamebuf, modedescbuf);
                    if (name && *name != 0)
                    {
                        strncpy(plname, name, PL_NSIZ - 1);
                        plname[PL_NSIZ - 1] = '\0';
                        if (check_saved_game_exists())
                        {
                            char qbuf[BUFSZ] = "";
                            Sprintf(qbuf, "There is already a saved game for a character named \'%s\'. Do you want to overwrite the save file and delete the existing character?", plname);
                            char ans = lib_yn_function_ex(YN_STYLE_GENERAL, ATR_NONE, CLR_RED, NO_GLYPH, "Overwrite Existing Character?",
                                qbuf, "ynql", 'n', "Yes\nNo\nQuit\nLoad", (const char*)0, 0UL);
                            if (ans == 'l')
                            {
                                return;
                            }
                            else if (ans == 'y')
                            {
                                set_savefile_name(TRUE);
                                delete_savefile();
                                return;
                            }
                            else
                            {
                                name = 0;
                                *plname = 0;
                                if (ans == 'n')
                                    repeataskname = TRUE;
                            }
                        }
                        else
                        {
                            return;
                        }
                    }
                    else
                    {
                        if (menures == 2)
                            lib_bail((char*)0); // Cannot return to restore_menu, so terminate the program; otherwise the game will repeat askname until a name is given, which is not the intention
                    }
                } while (repeataskname);
                break;
            case 1:
                return; /* plname[] has been set */
            }
        } while (name == 0 || *name == 0);
        return;
    }
#endif /* SELECTSAVED */

    name = lib_callbacks.callback_askname(modenamebuf, modedescbuf);
    if (name == 0 || *name == 0)
        lib_bail((char*)0); /* quit */
    else
    {
        strncpy(plname, name, PL_NSIZ - 1);
        plname[PL_NSIZ - 1] = '\0';
    }
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

winid lib_create_nhwindow_ex(int type, int style, int glyph, struct extended_create_window_info info)
{
    struct objclassdata ocdata = get_objclassdata(info.object);
    return lib_callbacks.callback_create_nhwindow_ex(type, style, glyph,
        (info.object ? 1 : 0) | (info.monster ? 2 : 0) | (Hallucination ? 4 : 0) | (info.create_flags & WINDOW_CREATE_FLAGS_ACTIVE ? 8 : 0)
        | (info.create_flags & WINDOW_CREATE_FLAGS_USE_SPECIAL_SYMBOLS ? 16 : 0),
        info.object, &ocdata);
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

/* text is supposed to be in CP437; if text is UTF8 encoding, call callback_putstr_ex directly */
void lib_putstr_ex(winid wid, int attr, const char* text, int append, int color)
{
    char buf[BUFSIZ];
    if (text)
        write_text2buf_utf8(buf, BUFSIZ, text);
    lib_callbacks.callback_putstr_ex(wid, attr, text ? buf : 0, append, color);
}

void lib_putstr_ex2(winid wid, const char* text, const char* attrs, const char* colors, int append)
{
    char buf[BUFSIZ];
    if (text)
        write_text2buf_utf8(buf, BUFSIZ, text);
    lib_callbacks.callback_putstr_ex(wid, attrs ? attrs[0] : ATR_NONE, text ? buf : 0, append, colors ? colors[0] : NO_COLOR);
    //lib_callbacks.callback_putstr_ex2(wid, text ? buf : 0, attrs, colors, append);
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
        winid textwin;
#define LLEN 128
        char line[LLEN];

        textwin = lib_create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_DISPLAY_FILE, NO_GLYPH, zerocreatewindowinfo);

        while (dlb_fgets(line, LLEN, f)) 
        {
            size_t len;
            len = strlen(line);
            if (line[len - 1] == '\n')
                line[len - 1] = '\x0';

            /* The files are already in UTF8 encoding, so do not convert again by using lib_putstr_ex */
            lib_callbacks.callback_putstr_ex(textwin, ATR_NONE, line, 0, NO_COLOR);
        }
        (void)dlb_fclose(f);

        lib_display_nhwindow(textwin, 1);
        lib_destroy_nhwindow(textwin);
    }

}

void lib_start_menu_ex(winid wid, int style)
{
    lib_callbacks.callback_start_menu_ex(wid, style);
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
    if (info.object)
        set_obj_glyph(info.object);

    struct objclassdata ocdata = get_objclassdata(info.object);
    lib_callbacks.callback_add_extended_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr, str ? buf : 0, presel, color, (info.object && !(info.menu_flags & MENU_FLAGS_COUNT_DISALLOWED) ? (int)info.object->quan : 0),
        (unsigned long long)(info.object ? info.object->o_id : 0), (unsigned long long)(info.monster ? info.monster->m_id : 0), info.heading_for_group_accelerator, info.special_mark, info.menu_flags,
        (info.object ? MENU_DATAFLAGS_HAS_OBJECT_DATA : 0) | (info.monster ? MENU_DATAFLAGS_HAS_MONSTER_DATA : 0) | (Hallucination ? MENU_DATAFLAGS_HALLUCINATED : 0) | (info.monster && info.monster->female ? MENU_DATAFLAGS_FEMALE : 0),
        info.style, info.object, &ocdata);
}

void lib_end_menu_ex(winid wid, const char* prompt, const char* subtitle)
{
    char buf[BUFSIZ];
    char buf2[BUFSIZ];
    if (prompt)
        write_text2buf_utf8(buf, BUFSIZ, prompt);
    if (subtitle)
        write_text2buf_utf8(buf2, BUFSIZ, subtitle);

    lib_callbacks.callback_end_menu_ex(wid, prompt ? buf : 0, subtitle ? buf2 : 0);
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

    lib_callbacks.callback_print_glyph(wid, x, y, layers.glyph, layers.bkglyph, symbol, ocolor, special, &layers);

    /* Now send all object data */
    /* Note: print_glyph clears all object data */
    boolean showing_detection = !!(layers.layer_flags & LFLAGS_SHOWING_DETECTION);

    int i;
    for (i = 0; i <= 1; i++)
    {
        int basewhere = 0;
        boolean use_nexthere = FALSE;
        struct obj* otmp = 0;
        struct obj mimic_obj = zeroobj;

        switch (i)
        {
        case 0:
        {
            if(!covers_objects(x, y))
                otmp = level.objects[x][y];

            /* Object mimic handling */
            boolean has_obj_mimic = FALSE;
            struct monst* mtmp = m_at(x, y);
            if (mtmp && (M_AP_TYPE(mtmp) == M_AP_OBJECT))
            {
                int sensed = (Protection_from_shape_changers || sensemon(mtmp));
                if (!sensed)
                {
                    has_obj_mimic = TRUE;
                    mimic_obj.otyp = mtmp->mappearance;
                    mimic_obj.corpsenm = has_mcorpsenm(mtmp) ? MCORPSENM(mtmp) : PM_TENGU;
                    mimic_obj.ox = x;
                    mimic_obj.oy = y;
                    mimic_obj.glyph = obj_to_glyph(&mimic_obj, newsym_rn2);
                    mimic_obj.gui_glyph = maybe_get_replaced_glyph(mimic_obj.glyph, x, y, data_to_replacement_info(mimic_obj.glyph,
                        is_obj_drawn_in_front(&mimic_obj) ? LAYER_COVER_OBJECT : LAYER_OBJECT, &mimic_obj, (struct monst*)0, 0UL, 0UL));
                }
            }
            if (has_obj_mimic)
            {
                mimic_obj.nexthere = otmp;
                otmp = &mimic_obj;
            }

            use_nexthere = TRUE;
            basewhere = OBJ_FLOOR;
            break;
        }
        case 1:
            /* A possible mimic has been added to memory objects */
            otmp = level.locations[x][y].hero_memory_layers.memory_objchn;
            use_nexthere = TRUE;
            basewhere = OBJ_HEROMEMORY;
            break;
        }

        for (; otmp; otmp = (use_nexthere ? otmp->nexthere : otmp->nobj))
        {
            if (showing_detection && !(otmp->speflags & SPEFLAGS_DETECTED))
                continue;

            unsigned long oflags = 0UL;
            if(is_obj_drawn_in_front(otmp))
                oflags |= OBJDATA_FLAGS_DRAWN_IN_FRONT;
            if (Hallucination)
                oflags |= OBJDATA_FLAGS_HALLUCINATION;
            if (uchain && otmp == uchain)
                oflags |= OBJDATA_FLAGS_UCHAIN;
            if (uball && otmp == uball)
                oflags |= OBJDATA_FLAGS_UBALL;

            set_obj_glyph(otmp);

            struct objclassdata ocdata = get_objclassdata(otmp);
            lib_callbacks.callback_send_object_data(x, y, otmp, 2, basewhere, &ocdata, oflags);
            if (otmp->cobj)
            {
                struct obj* cotmp;
                for (cotmp = otmp->cobj; cotmp; cotmp = cotmp->nobj)
                {
                    unsigned long coflags = 0UL;
                    if (is_obj_drawn_in_front(otmp)) /* otmp to find the right object chain */
                        coflags |= OBJDATA_FLAGS_DRAWN_IN_FRONT;
                    if (Hallucination)
                        coflags |= OBJDATA_FLAGS_HALLUCINATION;
                    if (uchain && cotmp == uchain)
                        coflags |= OBJDATA_FLAGS_UCHAIN;
                    if (uball && cotmp == uball)
                        coflags |= OBJDATA_FLAGS_UBALL;

                    set_obj_glyph(cotmp);

                    struct objclassdata cocdata = get_objclassdata(cotmp);
                    lib_callbacks.callback_send_object_data(x, y, cotmp, 3, basewhere, &cocdata, coflags);
                }
            }
        }
    }
}

void lib_issue_gui_command(int initid)
{
    lib_callbacks.callback_issue_gui_command(initid);

    switch (initid)
    {
    case GUI_CMD_CLEAR_PET_DATA:
    {
        struct monst_info mi = { 0 };
        struct monst* mtmp;
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (!DEADMONSTER(mtmp) && is_tame(mtmp))
            {
                monst_to_info(mtmp, &mi);
                lib_callbacks.callback_send_monster_data(0, 0, 0, &mi, 0UL); /* Add a pet */
            }
        }
        break;
    }
    case GUI_CMD_REPORT_PLAY_TIME:
    {
        long timePassed = (long)(urealtime.finish_time - urealtime.start_timing);
        lib_callbacks.callback_report_play_time(timePassed, urealtime.realtime);
        break;
    }
    case GUI_CMD_GAME_START:
    {
        lib_callbacks.callback_report_player_name(plname);
        break;
    }
    case GUI_CMD_GAME_ENDED:
        break;
    default:
        break;
    }
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

char lib_yn_function_ex(int style, int attr, int color, int glyph, const char* title, const char* question, const char* choices, CHAR_P def, const char* resp_desc, const char* introline, unsigned long ynflags)
{
    char buf[BUFSIZ] = "", tbuf[BUFSIZ] = "", ibuf[BUFSIZ] = "";
    if(question)
        write_text2buf_utf8(buf, BUFSIZ, question);
    if (title)
        write_text2buf_utf8(tbuf, BUFSIZ, title);
    if (introline)
        write_text2buf_utf8(ibuf, BUFSIZ, introline);
    char defs[2] = { 0,0 };
    defs[0] = def;
    int res = lib_callbacks.callback_yn_function_ex(style, attr, color, glyph, title ? tbuf : 0, question ? buf : 0, choices, defs, resp_desc, introline ? ibuf : 0, ynflags);
    return convert_gnhch(res);
}

void lib_getlin_ex(int style, int attr, int color, const char* question, char* input, const char* placeholder, const char* linesuffix, const char* introline)
{
    char buf[BUFSIZ] = "";
    char phbuf[BUFSIZ] = "";
    char dvbuf[BUFSIZ] = "";
    char ibuf[BUFSIZ] = "";

    if (question)
        write_text2buf_utf8(buf, BUFSIZ, question);
    if (placeholder)
        write_text2buf_utf8(phbuf, BUFSIZ, placeholder);
    if (linesuffix)
        write_text2buf_utf8(dvbuf, BUFSIZ, linesuffix);
    if (introline)
        write_text2buf_utf8(ibuf, BUFSIZ, introline);

    char* res = lib_callbacks.callback_getlin_ex(style, attr, color, buf, placeholder ? phbuf : 0, linesuffix ? dvbuf : 0, introline ? ibuf : 0);
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
    char* res = lib_callbacks.callback_getlin_ex(GETLINE_EXTENDED_COMMAND, ATR_NONE, NO_COLOR, "Type an Extended Command", 0, 0, 0);
    if (!res)
        return -1;

    if(*res == 27 || *res == 0)
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
    reduce_counters_intervals(2);
}

void lib_delay_output_milliseconds(int interval)
{
    lib_callbacks.callback_delay_output_milliseconds(interval);
    reduce_counters(interval);
}


void lib_delay_output_intervals(int intervals)
{
    lib_callbacks.callback_delay_output_intervals(intervals);
    reduce_counters_intervals(intervals);
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
    char kbuf[BUFSZ];
    formatkiller(kbuf, sizeof kbuf, how, FALSE);

    char tbuf[BUFSZ];
    long year = yyyymmdd(when) / 10000L;
    Sprintf(tbuf, "%4ld", year);

    int points = (int)u.u_gamescore;

    lib_callbacks.callback_outrip(wid, plname, points, kbuf, tbuf);
}

void lib_preference_update(const char* pref)
{
    lib_callbacks.callback_preference_update(pref);
    if (!strcmp(pref, "statuslines"))
    {
        context.botl = context.botlx = 1;
    }
}

char* lib_getmsghistory_ex(char** attrs_ptr, char** colors_ptr, BOOLEAN_P init)
{
    int attr = ATR_NONE, color = NO_COLOR;
    char* res = lib_callbacks.callback_getmsghistory(&attr, &color, (int)init);
    static char buf[BUFSIZ] = "";
    static char attrs[BUFSIZ];
    static char colors[BUFSIZ];
    if (res)
    {
        strncpy(buf, res, BUFSIZ - 1);
        buf[BUFSIZ - 1] = '\0';
        size_t len = strlen(buf);
        if (attrs_ptr)
        {
            memset(attrs, (char)attr, len);
            attrs[len] = 0;
            *attrs_ptr = attrs;
        }
        if (colors_ptr)
        {
            memset(colors, (char)color, len);
            colors[len] = 0;
            *colors_ptr = attrs;
        }
    }
    return res ? buf : 0;
}

void lib_putmsghistory_ex(const char* msg, const char* attrs, const char* colors, BOOLEAN_P is_restoring)
{
    lib_callbacks.callback_putmsghistory(msg, attrs ? attrs[0] : ATR_NONE, colors ? colors[0] : NO_COLOR, is_restoring);
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

void lib_status_enablefield(int fieldidx, const char* nm, const char* fmt, BOOLEAN_P enable)
{
    lib_callbacks.callback_status_enablefield(fieldidx, nm, fmt, enable);
    genl_status_enablefield(fieldidx, nm, fmt, enable);
}


static short condcolors[NUM_BL_CONDITIONS] = { 0 };

void lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks)
{
    __lib_status_update(idx, ptr, chg, percent, color, colormasks);

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

    if (idx == BL_CONDITION)
    {
        int i;
        for (i = 0; i < NUM_BL_CONDITIONS; i++)
        {
            int cond_mask = 1 << i;
            short color = get_condition_color(cond_mask);
            condcolors[i] = color;
        }
    }

    lib_callbacks.callback_status_update(idx, txt, condbits, chg, percent, color, !colormasks ? NULL : condcolors);
}

void monst_to_info(struct monst* mtmp, struct monst_info* mi_ptr)
{
    if (!mtmp || !mi_ptr)
        return;

    mi_ptr->glyph = any_mon_to_glyph(mtmp, rn2_on_display_rng);
    mi_ptr->gui_glyph = maybe_get_replaced_glyph(mi_ptr->glyph, mtmp->mx, mtmp->my, data_to_replacement_info(mi_ptr->glyph, LAYER_MONSTER, (struct obj*)0, mtmp, 0UL, 0UL));

    char tempbuf[BUFSIZ] = "";
    if (mtmp->mextra && UMNAME(mtmp))
    {
        char umnbuf[BUFSIZ];
        strcpy(umnbuf, UMNAME(mtmp));
        umnbuf[16] = '\0'; /* Limit the length of the name */
        strcat(tempbuf, umnbuf);
    }
    else if (mtmp->mextra && MNAME(mtmp) && mtmp->u_know_mname)
    {
        char mnbuf[BUFSIZ];
        strcpy(mnbuf, MNAME(mtmp));
        mnbuf[16] = '\0'; /* Limit the length of the name */
        strcat(tempbuf, mnbuf);
    }
    else
    {
        char buf[BUFSZ];
        strcpy(buf, mon_monster_name(mtmp));
        *buf = highc(*buf);
        strcat(tempbuf, buf);
    }
    strncpy(mi_ptr->name, tempbuf, BUFSZ - 1);
    mi_ptr->name[BUFSZ - 1] = '\0';

    mi_ptr->m_id = mtmp->m_id;
    mi_ptr->mhp = mtmp->mhp;
    mi_ptr->mhpmax = mtmp->mhpmax;

    mi_ptr->status_bits = get_m_status_bits(mtmp, FALSE, is_peaceful(mtmp) && !is_tame(mtmp), is_tame(mtmp), FALSE);
    mi_ptr->condition_bits = get_m_condition_bits(mtmp);
    get_m_buff_bits(mtmp, mi_ptr->buff_bits, FALSE);

    mi_ptr->monster_flags = 0UL;
}

int hl_attridx_to_attrmask(int idx)
{
    switch (idx)
    {
    case HL_ATTCLR_DIM:     return (1 << ATR_DIM);
    case HL_ATTCLR_BLINK:    return (1 << ATR_BLINK);
    case HL_ATTCLR_ULINE:   return (1 << ATR_ULINE);
    case HL_ATTCLR_INVERSE:    return (1 << ATR_INVERSE);
    case HL_ATTCLR_BOLD:    return (1 << ATR_BOLD);
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
    if (!cond_hilites)
        return CLR_WHITE;

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
    else if (idx == BL_MODE && !first_field)
    {
        /* game mode has no leading space, so if we've moved
           it past the first position, provide one */
        lib_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
    }
#if 0
    else if (idx == BL_LEVELDESC && !first_field)
    {
        /* leveldesc has no leading space, so if we've moved
           it past the first position, provide one */
        lib_putstr_ex(WIN_STATUS, ATR_NONE, " ", 0, CLR_WHITE);
    }
#endif
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
        //    debuglog("field %d: %s color %s", idx+1, val, colname(color));
    }
}

void lib_status_flush(void)
{
    enum statusfields idx;
    register int i, j;

    static const int fieldorder1[] = { BL_TITLE, BL_STR, BL_DX,    BL_CO,    BL_IN,
                             BL_WI,    BL_CH, BL_GOLD,  /*BL_ALIGN,*/ BL_FLUSH, };
    static const int fieldorder2[] = { BL_MODE, BL_LEVELDESC, BL_HP,   BL_HPMAX,
                                 BL_ENE,       BL_ENEMAX,    BL_AC,  BL_MC_LVL, BL_MC_PCT,    BL_MOVE, BL_UWEP, BL_UWEP2,  BL_XP,
                                 BL_EXP,       BL_HD,        BL_TIME, BL_SCORE, BL_FLUSH };
    static const int fieldorder2_2statuslines[] = { BL_MODE, BL_LEVELDESC, BL_HP,   BL_HPMAX,
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

void lib_set_animation_timer_interval(unsigned int param)
{
    lib_callbacks.callback_set_animation_timer_interval((unsigned long)param);
    return;
}

int lib_open_special_view(struct special_view_info info)
{
    char buf[BUFSIZ], buf2[BUFSIZ];
    if (info.text)
        write_text2buf_utf8(buf, BUFSIZ, info.text);
    if (info.title)
        write_text2buf_utf8(buf2, BUFSIZ, info.title);

    return lib_callbacks.callback_open_special_view(info.viewtype, info.text ? buf : 0, info.title ? buf2 : 0, info.attr, info.color);
}

void lib_stop_all_sounds(struct stop_all_info info)
{
    unsigned int saflags = info.stop_flags;
    unsigned int dialogue_mid = info.dialogue_mid;
    (void)lib_callbacks.callback_stop_all_sounds(saflags, dialogue_mid);
}

void lib_play_immediate_ghsound(struct ghsound_immediate_info info)
{
    if (info.ghsound == GHSOUND_NONE)
        return;

    int ghsound = info.ghsound;
    const char* eventPath = ghsound2event[info.ghsound].eventPath;
    int eventBank = ghsound2event[info.ghsound].bank_id;
    double eventVolume = (double)ghsound2event[info.ghsound].volume;
    double soundVolume = (double)info.volume;
    const char** parameter_names = info.parameter_names;
    float* parameter_values = info.parameter_values;
    int parameterarraysize = MAX_SOUND_PARAMETERS;
    int sound_type = info.sound_type;
    int play_group = info.play_group;
    unsigned int dialogue_mid = info.dialogue_mid;
    unsigned long play_flags = info.play_flags;

    (void)lib_callbacks.callback_play_immediate_ghsound(ghsound, eventPath, eventBank, eventVolume, soundVolume,
        parameter_names, parameter_values, parameterarraysize, sound_type, play_group, dialogue_mid, play_flags);

}

void lib_play_ghsound_occupation_ambient(struct ghsound_occupation_ambient_info info)
{
    int ghsound = info.ghsound;
    const char* eventPath = ghsound2event[info.ghsound].eventPath;
    int eventBank = ghsound2event[info.ghsound].bank_id;
    double eventVolume = (double)ghsound2event[info.ghsound].volume;
    double soundVolume = (double)info.volume;

    if (lib_callbacks.callback_play_ghsound_occupation_ambient(ghsound, eventPath, eventBank, eventVolume, soundVolume) != 0)
    {
        /* Error */
    }
}

void lib_play_ghsound_effect_ambient(struct ghsound_effect_ambient_info info)
{
    int ghsound = info.ghsound;
    const char* eventPath = ghsound2event[info.ghsound].eventPath;
    int eventBank = ghsound2event[info.ghsound].bank_id;
    double eventVolume = (double)ghsound2event[info.ghsound].volume;
    double soundVolume = (double)info.volume;

    if (lib_callbacks.callback_play_ghsound_effect_ambient(ghsound, eventPath, eventBank, eventVolume, soundVolume) != 0)
    {
        /* Error */
    }
}

void lib_set_effect_ambient_volume(struct effect_ambient_volume_info info)
{
    if (lib_callbacks.callback_set_effect_ambient_volume((double)info.volume) != 0)
    {
        /* Error */
    }
}

void lib_play_ghsound_music(struct ghsound_music_info info)
{
    if (info.ghsound == GHSOUND_NONE)
        return;

    int ghsound = info.ghsound;
    const char* eventPath = ghsound2event[info.ghsound].eventPath;
    int eventBank = ghsound2event[info.ghsound].bank_id;
    double eventVolume = (double)ghsound2event[info.ghsound].volume;
    double soundVolume = (double)info.volume;

    if (lib_callbacks.callback_play_ghsound_music(ghsound, eventPath, eventBank, eventVolume, soundVolume) != 0)
    {
        /* Error */
    }
}

void lib_play_ghsound_level_ambient(struct ghsound_level_ambient_info info)
{
    int ghsound = info.ghsound;
    const char* eventPath = ghsound2event[info.ghsound].eventPath;
    int eventBank = ghsound2event[info.ghsound].bank_id;
    double eventVolume = (double)ghsound2event[info.ghsound].volume;
    double soundVolume = (double)info.volume;

    if (lib_callbacks.callback_play_ghsound_level_ambient(ghsound, eventPath, eventBank, eventVolume, soundVolume) != 0)
    {
        /* Error */
    }
}

void lib_play_ghsound_environment_ambient(struct ghsound_environment_ambient_info info)
{
    int ghsound = info.ghsound;
    const char* eventPath = ghsound2event[info.ghsound].eventPath;
    int eventBank = ghsound2event[info.ghsound].bank_id;
    double eventVolume = (double)ghsound2event[info.ghsound].volume;
    double soundVolume = (double)info.volume;

    if (lib_callbacks.callback_play_ghsound_environment_ambient(ghsound, eventPath, eventBank, eventVolume, soundVolume))
    {
        /* Error */
    }
}

void lib_adjust_ghsound_general_volumes(VOID_ARGS)
{
    return;
}

void lib_add_ambient_ghsound(struct soundsource_t* soundsource)
{
    if (!soundsource)
        return;

    int ghsound = soundsource->ghsound;
    const char* eventPath = ghsound2event[ghsound].eventPath;
    int eventBank = ghsound2event[ghsound].bank_id;
    double eventVolume = (double)ghsound2event[ghsound].volume;
    double soundVolume = (double)soundsource->heard_volume;
    unsigned long long idptr = 0;

    if (lib_callbacks.callback_add_ambient_ghsound(ghsound, eventPath, eventBank, eventVolume, (double)soundVolume, &idptr))
    {
        /* Error */
    }

    soundsource->ambient_ghsound_ptr = (void*)idptr;
}

void lib_delete_ambient_ghsound(struct soundsource_t* soundsource)
{
    if (!soundsource)
        return;

    lib_callbacks.callback_delete_ambient_ghsound((unsigned long long)soundsource->ambient_ghsound_ptr);

}

void lib_set_ambient_ghsound_volume(struct soundsource_t* soundsource)
{
    if (!soundsource)
        return;
    lib_callbacks.callback_set_ambient_ghsound_volume((unsigned long long)soundsource->ambient_ghsound_ptr, (double)soundsource->heard_volume);

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
lib_add_context_menu(int cmd_def_char, int cmd_cur_char, int style, int glyph, const char* cmd_text, const char* target_text, int attr, int color)
{
    lib_callbacks.callback_add_context_menu(cmd_def_char, cmd_cur_char, style, glyph, cmd_text, target_text, attr, color);
}

void
lib_update_status_button(int cmd, int btn, int val, unsigned long bflags)
{
    lib_callbacks.callback_update_status_button(cmd, btn, val, bflags);
}

void
lib_toggle_animation_timer(int timertype, int timerid, int state, int x, int y, int layer, unsigned long tflags)
{
    lib_callbacks.callback_toggle_animation_timer(timertype, timerid, state, x, y, layer, tflags);
}

void
lib_display_floating_text(int x, int y, const char* text, int style, int attr, int color, unsigned long tflags)
{
    lib_callbacks.callback_display_floating_text(x, y, text, style, attr, color, tflags);
}

void
lib_display_screen_text(const char* text, const char* supertext, const char* subtext, int style, int attr, int color, unsigned long tflags)
{
    lib_callbacks.callback_display_screen_text(text, supertext, subtext, style, attr, color, tflags);
}

void
lib_display_popup_text(const char* text, const char* title, int style, int attr, int color, int glyph, unsigned long tflags)
{
    lib_callbacks.callback_display_popup_text(text, title, style, attr, color, glyph, tflags);
}

void
lib_display_gui_effect(int x, int y, int style, unsigned long tflags)
{
    lib_callbacks.callback_display_gui_effect(x, y, style, tflags);
}

void
lib_update_cursor(int style, int force_paint, int show_on_u)
{
    lib_callbacks.callback_update_cursor(style, force_paint, show_on_u);
}

int
lib_ui_has_input(VOID_ARGS)
{
    return lib_callbacks.callback_ui_has_input();
}


/* Helper functions */
void lib_bail(const char* mesg)
{
    nh_bail(EXIT_SUCCESS, mesg, FALSE);
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
        key = (char)'0';
        break;
    default:
        if (ch >= -19 && ch <= -10)
            key = (char)(abs(ch) - 10 + '0');
        else
            key = (char)ch;
        break;
    }

    return key;
}

void debuglog(const char* fmt, ...)
{
    char buf[256];

    if (fmt != 0)
    {
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
    }
    else
    {
        strcpy(buf, "(null)");
    }

    if (lib_callbacks.callback_raw_print)
        lib_raw_print(buf);
}



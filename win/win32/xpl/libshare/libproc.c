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
    lib_change_color, 
#ifdef MAC
    lib_change_background,
    lib_set_font_name,
#endif
    lib_get_color_string,
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
    char mdbuf[BUFSZ] = "";
    char modenamebuf[BUFSZ] = "";
    char modedescbuf[BUFSZ] = "";
    char utf8buf[PL_NSIZ * 4 + UTF8BUFSZ] = "";
    Sprintf(mdbuf, "%s mode", get_game_mode_text(FALSE));
    strcpy_capitalized_for_title(modenamebuf, mdbuf);
    Strcpy(modedescbuf, get_game_mode_description());
    *modedescbuf = highc(*modedescbuf);
    int res = 0;

    struct ghsound_music_info mi = { 0 };
    mi.ghsound = GHSOUND_MUSIC_PLAYER_SELECTION;
    mi.volume = 1.0f;
    play_ghsound_music(mi);

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
                    *utf8buf = 0;
                    res = lib_callbacks.callback_askname(modenamebuf, modedescbuf, utf8buf);
                    if (res)
                    {
                        char ansbuf[PL_NSIZ + BUFSZ] = "";
                        copyUTF8toCP437(ansbuf, sizeof(ansbuf), utf8buf, sizeof(utf8buf));
                        ansbuf[sizeof(ansbuf) - 1] = 0;
                        int i, len = (int)strlen(ansbuf);
                        for (i = 0; i < len; i++)
                            if (ansbuf[i] > 127 || ansbuf[i] < 0)
                                ansbuf[i] = '_';

                        Strncpy(plname, ansbuf, PL_NSIZ - 1);
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
                                *utf8buf = 0;
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
        } while (*utf8buf == 0);
        return;
    }
#endif /* SELECTSAVED */

    *utf8buf = 0;
    res = lib_callbacks.callback_askname(modenamebuf, modedescbuf, utf8buf);
    if (!res)
        lib_bail((char*)0); /* quit */
    else
    {
        char ansbuf[PL_NSIZ + BUFSZ] = "";
        copyUTF8toCP437(ansbuf, sizeof(ansbuf), utf8buf, sizeof(utf8buf));
        ansbuf[sizeof(ansbuf) - 1] = 0;
        int i, len = (int)strlen(ansbuf);
        for (i = 0; i < len; i++)
            if (ansbuf[i] > 127 || ansbuf[i] < 0)
                ansbuf[i] = '_';

        Strncpy(plname, ansbuf, PL_NSIZ - 1);
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
    iflags.window_inited = 0;
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
void lib_putstr_ex(winid wid, const char* text, int attr, int color, int append)
{
    char buf[UTF8BUFSZ] = "";
    if (text)
        write_text2buf_utf8(buf, UTF8BUFSZ, text);
    lib_callbacks.callback_putstr_ex(wid, text ? buf : 0, attr, color, append);
}

void lib_putstr_ex2(winid wid, const char* text, const char* attrs, const char* colors, int attr, int color, int append)
{
    char buf[UTF8BUFSZ];
    if (text)
        write_text2buf_utf8(buf, UTF8BUFSZ, text);
    //lib_callbacks.callback_putstr_ex(wid, attrs ? attrs[0] : attr, text ? buf : 0, append, colors ? colors[0] : color);
    lib_callbacks.callback_putstr_ex2(wid, text ? buf : 0, attrs, colors, attr, color, append);
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
            lib_callbacks.callback_putstr_ex(textwin, line, ATR_NONE, NO_COLOR, 0);
        }
        (void)dlb_fclose(f);

        lib_display_nhwindow(textwin, 1);
        lib_destroy_nhwindow(textwin);
    }

}

static int last_menu_style = 0;
void lib_start_menu_ex(winid wid, int style)
{
    last_menu_style = style;
    lib_callbacks.callback_start_menu_ex(wid, style);
}

void lib_add_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr, int color,
    const char* str, BOOLEAN_P presel)
{
    if (!str)
        return;

    char buf[UTF8BUFSZ];
    if (str)
        write_text2buf_utf8(buf, UTF8BUFSZ, str);
#ifdef TEXTCOLOR
    if(iflags.use_menu_color && menu_style_allows_menu_coloring(last_menu_style))
        get_menu_coloring(str, &color, &attr);
#endif
    lib_callbacks.callback_add_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr, color, str ? buf : 0, presel);
}

void lib_add_extended_menu(winid wid, int glyph, const ANY_P* identifier,
    CHAR_P accelerator, CHAR_P group_accel, int attr, int color,
    const char* str, BOOLEAN_P presel, struct extended_menu_info info)
{
    if (!str)
        return;

    char buf[UTF8BUFSZ];
    if(str)
        write_text2buf_utf8(buf, UTF8BUFSZ, str);

#ifdef TEXTCOLOR
    if (iflags.use_menu_color && menu_style_allows_menu_coloring(last_menu_style))
        get_menu_coloring(str, &color, &attr);
#endif
    if (info.object)
        set_obj_glyph(info.object);

    struct objclassdata ocdata = get_objclassdata(info.object);
    lib_callbacks.callback_add_extended_menu(wid, glyph, identifier->a_longlong, accelerator, group_accel, attr, color, str ? buf : 0, presel, (info.object && !(info.menu_flags & MENU_FLAGS_COUNT_DISALLOWED) ? (int)info.object->quan : 0),
        (unsigned long long)(info.object ? info.object->o_id : 0), (unsigned long long)(info.monster ? info.monster->m_id : 0), info.heading_for_group_accelerator, info.special_mark, info.menu_flags,
        (info.object ? MENU_DATAFLAGS_HAS_OBJECT_DATA : 0) | (info.monster ? MENU_DATAFLAGS_HAS_MONSTER_DATA : 0) | (Hallucination ? MENU_DATAFLAGS_HALLUCINATED : 0) | (info.monster && info.monster->female ? MENU_DATAFLAGS_FEMALE : 0),
        info.style, info.object, &ocdata);
}

void lib_end_menu_ex(winid wid, const char* prompt, const char* subtitle)
{
    char buf[UTF8BUFSZ];
    char buf2[UTF8BUFSZ];
    if (prompt)
        write_text2buf_utf8(buf, UTF8BUFSZ, prompt);
    if (subtitle)
        write_text2buf_utf8(buf2, UTF8BUFSZ, subtitle);

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

    if (program_state.in_bones)
        return;

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
                    if (has_mobj(mtmp))
                        mimic_obj = *MOBJ(mtmp);
                    else
                    {
                        mimic_obj.otyp = mtmp->mappearance;
                        mimic_obj.corpsenm = has_mcorpsenm(mtmp) ? MCORPSENM(mtmp) : PM_TENGU;
                        mimic_obj.quan = 1L;
                    }
                    mimic_obj.ox = x;
                    mimic_obj.oy = y;
                    mimic_obj.glyph = obj_to_glyph(&mimic_obj, newsym_rn2);
                    mimic_obj.gui_glyph = maybe_get_replaced_glyph(mimic_obj.glyph, x, y, data_to_replacement_info(mimic_obj.glyph,
                        is_obj_drawn_in_front(&mimic_obj) ? LAYER_COVER_OBJECT : LAYER_OBJECT, &mimic_obj, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
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
            if (is_obj_found_this_turn(otmp))
                oflags |= OBJDATA_FLAGS_FOUND_THIS_TURN;
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

void lib_issue_gui_command(int cmd_id, int cmd_param, int cmd_param2, const char* cmd_str)
{
    char utf8buf[UTF8BUFSZ];
    if (cmd_str)
        write_text2buf_utf8(utf8buf, UTF8BUFSZ, cmd_str);

    lib_callbacks.callback_issue_gui_command(cmd_id, cmd_param, cmd_param2, cmd_str ? utf8buf : 0);

    switch (cmd_id)
    {
    case GUI_CMD_START_FLUSH:
    {
        lib_callbacks.callback_send_object_data(0, 0, 0, 4, 0, 0, 0UL); /* Clear out uchain and uball */
        if (uchain)
        {
            unsigned long oflags = 0UL;
            if (Hallucination)
                oflags |= OBJDATA_FLAGS_HALLUCINATION;
            oflags |= OBJDATA_FLAGS_UCHAIN;
            set_obj_glyph(uchain);

            struct objclassdata ocdata = get_objclassdata(uchain);
            lib_callbacks.callback_send_object_data(uchain->ox, uchain->oy, uchain, 5, uchain->where, &ocdata, oflags);
        }
        if (uball)
        {
            unsigned long oflags = 0UL;
            if (Hallucination)
                oflags |= OBJDATA_FLAGS_HALLUCINATION;
            oflags |= OBJDATA_FLAGS_UBALL;
            set_obj_glyph(uball);

            struct objclassdata ocdata = get_objclassdata(uball);
            lib_callbacks.callback_send_object_data(uball->ox, uchain->oy, uball, 5, uball->where, &ocdata, oflags);
        }
        break;
    }
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
        lock_thread_lock();
        long timePassed = (long)(urealtime.finish_time - urealtime.start_timing);
        long realtime = urealtime.realtime;
        unlock_thread_lock();
        lib_callbacks.callback_report_play_time(timePassed, realtime);
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
    char buf[UTF8BUFSZ];
    if(str)
        write_text2buf_utf8(buf, UTF8BUFSZ, str);
    lib_callbacks.callback_raw_print(str ? buf : 0);
}

void lib_raw_print_bold(const char* str)
{
    char buf[UTF8BUFSZ];
    if(str)
        write_text2buf_utf8(buf, UTF8BUFSZ, str);
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
    reset_found_this_turn(); /* Otherwise, after user input, the animation might play again upon flush_screen */

    char buf[UTF8QBUFSZ] = "", tbuf[UTF8QBUFSZ] = "", ibuf[UTF8IBUFSZ] = "";
    if(question)
        write_text2buf_utf8(buf, UTF8QBUFSZ, question);
    if (title)
        write_text2buf_utf8(tbuf, UTF8QBUFSZ, title);
    if (introline)
        write_text2buf_utf8(ibuf, UTF8IBUFSZ, introline);
    char defs[2] = { 0,0 };
    defs[0] = def;
    int res = lib_callbacks.callback_yn_function_ex(style, attr, color, glyph, title ? tbuf : 0, question ? buf : 0, choices, defs, resp_desc, introline ? ibuf : 0, ynflags);
    return convert_gnhch(res);
}

void lib_getlin_ex(int style, int attr, int color, const char* question, char* input, const char* placeholder, const char* linesuffix, const char* introline)
{
    reset_found_this_turn(); /* Otherwise, after user input, the animation might play again upon flush_screen */

    char buf[UTF8QBUFSZ] = "";
    char phbuf[UTF8BUFSZ] = "";
    char dvbuf[UTF8BUFSZ] = "";
    char ibuf[UTF8IBUFSZ] = "";

    char utf8buf[UTF8BUFSZ] = "";

    if (question)
        write_text2buf_utf8(buf, UTF8QBUFSZ, question);
    if (placeholder)
        write_text2buf_utf8(phbuf, UTF8BUFSZ, placeholder);
    if (linesuffix)
        write_text2buf_utf8(dvbuf, UTF8BUFSZ, linesuffix);
    if (introline)
        write_text2buf_utf8(ibuf, UTF8IBUFSZ, introline);

    int res = lib_callbacks.callback_getlin_ex(style, attr, color, buf, placeholder ? phbuf : 0, linesuffix ? dvbuf : 0, introline ? ibuf : 0, utf8buf);
    if (res && input)
    {
        char msgbuf[BUFSZ] = "";
        copyUTF8toCP437(msgbuf, sizeof(msgbuf), utf8buf, sizeof(utf8buf));
        Strncpy(input, msgbuf, BUFSZ - 1);
        input[BUFSZ - 1] = '\0';
    }
}

int lib_get_ext_cmd(void)
{
    char utf8buf[UTF8BUFSZ] = "";
    int res = lib_callbacks.callback_getlin_ex(GETLINE_EXTENDED_COMMAND, ATR_NONE, NO_COLOR, "Type an Extended Command", 0, 0, 0, utf8buf);
    if (!res)
        return -1;

    if(*utf8buf == 27 || *utf8buf == 0)
        return -1;

    char buf[BUFSZ];
    copyUTF8toCP437(buf, sizeof(buf), utf8buf, sizeof(utf8buf));
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

#ifdef CHANGE_COLOR
void lib_change_color(int param1, long param2, int param3)
{
    return;
}

#ifdef MAC
/*ARGUSED*/
STATIC_OVL void
lib_change_background(arg)
int arg UNUSED;
{
    return;
}

/*ARGSUSED*/
STATIC_OVL short
lib_set_font_name(window, fontname)
winid window;
char* fontname;
{
    return 0;
}
#endif /* MAC */

char* lib_get_color_string(void)
{
    return "";
}
#endif

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
    static char buf[BUFSIZ * 2] = "";
    static char attrs[BUFSIZ * 2] = "";
    static char colors[BUFSIZ * 2] = "";
    char utf8buf[UTF8BUFSZ * 4] = "";
    int res = lib_callbacks.callback_getmsghistory(utf8buf, attrs, colors, (int)init);
    if (res)
    {
        copyUTF8toCP437(buf, sizeof(buf), utf8buf, sizeof(utf8buf));
        buf[sizeof(buf) - 1] = 0; /* Insurance */

        if (attrs_ptr)
        {
            *attrs_ptr = attrs;
        }
        if (colors_ptr)
        {
            *colors_ptr = colors;
        }
    }
    return res ? buf : 0;
}

void lib_putmsghistory_ex(const char* msg, const char* attrs, const char* colors, BOOLEAN_P is_restoring)
{
    char buf[UTF8BUFSZ * 4] = "";
    if (msg)
        write_text2buf_utf8(buf, sizeof(buf), msg);

    lib_callbacks.callback_putmsghistory(buf, attrs, colors, is_restoring);
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

void lib_status_init(int reassessment)
{
    lib_callbacks.callback_status_init(reassessment);
    genl_status_init(reassessment);

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

    if (!enable)
    {
        if (fieldidx == BL_UWEP || fieldidx == BL_UWEP2)
        {
            unsigned long owepflags = 0UL;
            if (fieldidx == BL_UWEP)
                owepflags |= OBJDATA_FLAGS_UWEP;
            else if (fieldidx == BL_UWEP2)
                owepflags |= OBJDATA_FLAGS_UWEP2;

            lib_callbacks.callback_send_object_data(0, 0, 0, 1, 0, 0, owepflags);
            if(fieldidx == BL_UWEP)
                lib_callbacks.callback_send_object_data(0, 0, 0, 1, 0, 0, OBJDATA_FLAGS_UQUIVER);
        }
    }
}


static short condcolors[NUM_BL_CONDITIONS] = { 0 };

void lib_status_update(int idx, genericptr_t ptr, int chg, int percent, int color, unsigned long* colormasks)
{
    __lib_status_update(idx, ptr, chg, percent, color, colormasks);

    char* txt = (char*)0;
    char utf8buf[UTF8BUFSZ] = "";
    long condbits = 0L;
    if (ptr)
    {
        if (idx != BL_CONDITION)
        {
            txt = ptr;
        }
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

    if (txt)
        write_text2buf_utf8(utf8buf, sizeof(utf8buf), txt);

    lib_callbacks.callback_status_update(idx, txt ? utf8buf : 0, condbits, chg, percent, color, !colormasks ? NULL : condcolors);

    if (idx == BL_UWEP || idx == BL_UWEP2)
    {
        struct obj* wep = idx == BL_UWEP ? (uwep ? uwep : uarmg) : (uwep2 ? uwep2 : uarmg);
        unsigned long oflags = 0UL;
        if (Hallucination)
            oflags |= OBJDATA_FLAGS_HALLUCINATION;

        unsigned long owepflags = 0UL;
        boolean islauncher = wep && is_launcher(wep);
        struct obj* ammo = idx == BL_UWEP ? uquiver : uquiver;

        if (idx == BL_UWEP)
            owepflags |= OBJDATA_FLAGS_UWEP;
        else if (idx == BL_UWEP2)
        {
            owepflags |= OBJDATA_FLAGS_UWEP2;
            if(wep && (is_weapon(wep) && !is_shield(wep)) && !u.twoweap)
                owepflags |= OBJDATA_FLAGS_NOT_BEING_USED2;
            if (wep && !is_weapon(wep) && u.twoweap)
                owepflags |= OBJDATA_FLAGS_NOT_WEAPON2;
        }

        if (islauncher)
        {
            if (!ammo)
                owepflags |= (idx == BL_UWEP ? OBJDATA_FLAGS_OUT_OF_AMMO1 : OBJDATA_FLAGS_OUT_OF_AMMO2);
            else if (!ammo_and_launcher(ammo, wep))
                owepflags |= (idx == BL_UWEP ? OBJDATA_FLAGS_WRONG_AMMO_TYPE1 : OBJDATA_FLAGS_WRONG_AMMO_TYPE2);
        }

        if (wep)
            set_obj_glyph(wep);

        struct objclassdata ocdata = get_objclassdata(wep);
        lib_callbacks.callback_send_object_data(0, 0, wep, wep ? 2 : 1, wep ? wep->where : 0, &ocdata, oflags | owepflags);

        if (idx == BL_UWEP)
        {
            unsigned long oammoflags = OBJDATA_FLAGS_UQUIVER;
            struct obj* ammo = uquiver;

            if(ammo)
                set_obj_glyph(ammo);
            ocdata = get_objclassdata(ammo);
            lib_callbacks.callback_send_object_data(0, 0, ammo, ammo ? 2 : 1, ammo ? ammo->where : 0, &ocdata, oflags | oammoflags);
        }
    }
}

void monst_to_info(struct monst* mtmp, struct monst_info* mi_ptr)
{
    if (!mtmp || !mi_ptr)
        return;

    mi_ptr->glyph = any_mon_to_glyph(mtmp, rn2_on_display_rng);
    mi_ptr->gui_glyph = maybe_get_replaced_glyph(mi_ptr->glyph, mtmp->mx, mtmp->my, data_to_replacement_info(mi_ptr->glyph, LAYER_MONSTER, (struct obj*)0, mtmp, 0UL, 0UL, MAT_NONE, 0));

    char tempbuf[BUFSIZ] = "";
    if (has_umname(mtmp))
    {
        char umnbuf[BUFSIZ];
        strcpy(umnbuf, UMNAME(mtmp));
        umnbuf[16] = '\0'; /* Limit the length of the name */
        strcat(tempbuf, umnbuf);
    }
    else if (has_mname(mtmp) && mtmp->u_know_mname)
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
    write_text2buf_utf8(mi_ptr->name, sizeof(mi_ptr->name), tempbuf);
    //Strncpy(mi_ptr->name, tempbuf, BUFSZ - 1);
    mi_ptr->name[sizeof(mi_ptr->name) - 1] = '\0';

    mi_ptr->m_id = mtmp->m_id;
    mi_ptr->mhp = mtmp->mhp;
    mi_ptr->mhpmax = mtmp->mhpmax;

    mi_ptr->status_bits = get_m_status_bits(mtmp, FALSE, is_peaceful(mtmp) && !is_tame(mtmp), is_tame(mtmp), FALSE);
    mi_ptr->condition_bits = get_m_condition_bits(mtmp);
    get_m_buff_bits(mtmp, mi_ptr->buff_bits, FALSE);

    mi_ptr->monster_flags = 0UL;
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

void lib_print_conditions(const char** names)
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
            lib_putstr_ex(WIN_STATUS, " ", ATR_NONE, CLR_WHITE, 0);
            lib_putstr_ex(WIN_STATUS, name, hl_attrmask_to_atr(attr), color, 0);
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
        lib_putstr_ex(WIN_STATUS, " ", ATR_NONE, CLR_WHITE, 0);
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
        lib_putstr_ex(WIN_STATUS, " ", ATR_NONE, CLR_WHITE, 0);
        val++;
    }

    if (idx == BL_CONDITION)
    {
        lib_print_conditions(cond_names);
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
        lib_putstr_ex(WIN_STATUS, val, hl_attrmask_to_atr(attr), color, 0);
        //    debuglog("field %d: %s color %s", idx+1, val, colname(color));
    }
}

void lib_status_flush(void)
{
    enum statusfields idx;
    register int i, j;

    const enum statusfields** fieldorder = iflags.wc2_statuslines < 3 ? fieldorders_2statuslines : flags.fullstatuslineorder ? fieldorders_alt : fieldorders;

    for (j = 0; fieldorder[j] != NULL && j < iflags.wc2_statuslines; j++)
    {
        curs(WIN_STATUS, 1, j);
        for (i = 0; i < MAX_STATUS_LINE_ITEMS && (idx = fieldorder[j][i]) != BL_FLUSH; i++)
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
    char buf[UTF8BUFSZ], buf2[UTF8BUFSZ];
    if (info.text)
        write_text2buf_utf8(buf, UTF8BUFSZ, info.text);
    if (info.title)
        write_text2buf_utf8(buf2, UTF8BUFSZ, info.title);

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
    char cmdutf8buf[UTF8BUFSZ] = "";
    char targetutf8buf[UTF8BUFSZ] = "";
    if (cmd_text)
        write_text2buf_utf8(cmdutf8buf, sizeof(cmdutf8buf), cmd_text);
    if (target_text)
        write_text2buf_utf8(targetutf8buf, sizeof(targetutf8buf), target_text);
    lib_callbacks.callback_add_context_menu(cmd_def_char, cmd_cur_char, style, glyph, cmd_text ? cmdutf8buf : 0, target_text ? targetutf8buf : 0, attr, color);
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
    char utf8buf[UTF8BUFSZ] = "";
    if (text)
        write_text2buf_utf8(utf8buf, sizeof(utf8buf), text);
    lib_callbacks.callback_display_floating_text(x, y, text ? utf8buf : 0, style, attr, color, tflags);
}

void
lib_display_screen_text(const char* text, const char* supertext, const char* subtext, int style, int attr, int color, unsigned long tflags)
{
    char utf8buf[UTF8BUFSZ] = "";
    char superutf8buf[UTF8BUFSZ] = "";
    char subutf8buf[UTF8BUFSZ] = "";
    if (text)
        write_text2buf_utf8(utf8buf, sizeof(utf8buf), text);
    if (supertext)
        write_text2buf_utf8(superutf8buf, sizeof(superutf8buf), supertext);
    if (subtext)
        write_text2buf_utf8(subutf8buf, sizeof(subutf8buf), subtext);
    lib_callbacks.callback_display_screen_text(text ? utf8buf : 0, supertext ? superutf8buf : 0, subtext ? subutf8buf : 0, style, attr, color, tflags);
}

void
lib_display_popup_text(const char* text, const char* title, int style, int attr, int color, int glyph, unsigned long tflags)
{
    char utf8buf[UTF8BUFSZ] = "";
    char titleutf8buf[UTF8BUFSZ] = "";
    if (text)
        write_text2buf_utf8(utf8buf, sizeof(utf8buf), text);
    if (title)
        write_text2buf_utf8(titleutf8buf, sizeof(titleutf8buf), title);
    lib_callbacks.callback_display_popup_text(text ? utf8buf : 0, title ? titleutf8buf : 0, style, attr, color, glyph, tflags);
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
    nh_bail(EXIT_SUCCESS, mesg, TRUE);
}

void lib_init_platform(VOID_ARGS)
{
    thread_lock_init();
    /* Tiles */
#ifdef USE_TILES
    process_tiledata(1, (const char*)0, glyph2tile, glyphtileflags);
#endif

}

void lib_exit_platform(int status)
{
    thread_lock_destroy();
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



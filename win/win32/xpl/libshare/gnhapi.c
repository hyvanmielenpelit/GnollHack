/* GnollHack 4.1.0 gnhapi.c */
/* Copyright (c) Janne Gustafsson, 2022. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "gnhapi.h"
#include "hack.h"
#include "func_tab.h"
#include "libproc.h"
#include "callback.h"
#ifndef WIN32
#include "date_unix.h"
#include <sys/stat.h>
#else
#include "date.h"
#endif

extern int FDECL(GnollHackMain, (int, char**));
extern void FDECL(set_wincaps, (uint64_t, uint64_t));

DLLEXPORT void GetGlyph2Tile(int** gl2ti_ptr, int* size_ptr)
{
    if (!gl2ti_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *gl2ti_ptr = glyph2tile;
    *size_ptr = SIZE(glyph2tile);
#else
    *gl2ti_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetGlyphTileFlags(uchar** gltifl_ptr, int* size_ptr)
{
    if (!gltifl_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *gltifl_ptr = glyphtileflags;
    *size_ptr = SIZE(glyphtileflags);
#else
    *gltifl_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetTile2Animation(short** ti2an_ptr, int* size_ptr)
{
    if (!ti2an_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *ti2an_ptr = tile2animation;
    *size_ptr = SIZE(tile2animation);
#else
    *ti2an_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetTile2Enlargement(short** ti2en_ptr, int* size_ptr)
{
    if (!ti2en_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *ti2en_ptr = tile2enlargement;
    *size_ptr = SIZE(tile2enlargement);
#else
    *ti2en_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetTile2Autodraw(short** ti2ad_ptr, int* size_ptr)
{
    if (!ti2ad_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *ti2ad_ptr = tile2autodraw;
    *size_ptr = SIZE(tile2autodraw);
#else
    *ti2ad_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetAnimationOffsets(int** anoff_ptr, int* size_ptr)
{
    if (!anoff_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *anoff_ptr = animation_offsets;
    *size_ptr = SIZE(animation_offsets);
#else
    *anoff_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetEnlargementOffsets(int** enoff_ptr, int* size_ptr)
{
    if (!enoff_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *enoff_ptr = enlargement_offsets;
    *size_ptr = SIZE(enlargement_offsets);
#else
    *enoff_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT void GetReplacementOffsets(int** reoff_ptr, int* size_ptr)
{
    if (!reoff_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *reoff_ptr = replacement_offsets;
    *size_ptr = SIZE(replacement_offsets);
#else
    *reoff_ptr = 0;
    *size_ptr = 0;
#endif
}

DLLEXPORT int CountTotalTiles(VOID_ARGS)
{
    return process_tiledata(2, (const char*)0, (int*)0, (uchar*)0);
}

DLLEXPORT void LibSetGlyphArrays(int* gl2ti, int size_gl2ti, uchar* gltifl, int size_gltifl)
{
#ifdef USE_TILES
    int i;
    if (gl2ti)
    {
        int arrsiz_gl2ti = min(size_gl2ti, (int)SIZE(glyph2tile));
        for (i = 0; i < arrsiz_gl2ti; i++)
        {
            glyph2tile[i] = gl2ti[i];
        }
    }
    if (gltifl)
    {
        int arrsiz_gltifl = min(size_gltifl, (int)SIZE(glyphtileflags));
        for (i = 0; i < arrsiz_gltifl; i++)
        {
            glyphtileflags[i] = gltifl[i];
        }
    }
#endif
}

DLLEXPORT void LibSetTile2AnimationArray(short* ti2an, int size_ti2an)
{
#ifdef USE_TILES
    int i;
    if (ti2an)
    {
        int arrsiz_ti2an = min(size_ti2an, (int)SIZE(tile2animation));
        for (i = 0; i < arrsiz_ti2an; i++)
        {
            tile2animation[i] = ti2an[i];
        }
    }
#endif
}

DLLEXPORT int LibGetUnexploredGlyph(VOID_ARGS)
{
    return base_cmap_to_glyph(S_unexplored);
}

DLLEXPORT int LibGetNoGlyph(VOID_ARGS)
{
    return NO_GLYPH;
}

DLLEXPORT int LibGetAnimationOff(VOID_ARGS)
{
    return GLYPH_ANIMATION_OFF;
}

DLLEXPORT int LibGetEnlargementOff(VOID_ARGS)
{
    return GLYPH_ENLARGEMENT_OFF;
}

DLLEXPORT int LibGetReplacementOff(VOID_ARGS)
{
    return GLYPH_ANIMATION_OFF;
}

DLLEXPORT int LibGetGeneralTileOff(VOID_ARGS)
{
    return GLYPH_GENERAL_TILE_OFF;
}

DLLEXPORT int LibGetHitTileOff(VOID_ARGS)
{
    return GLYPH_HIT_TILE_OFF;
}

DLLEXPORT int LibGetUITileOff(VOID_ARGS)
{
    return GLYPH_UI_TILE_OFF;
}

DLLEXPORT int LibGetSpellTileOff(VOID_ARGS)
{
    return GLYPH_SPELL_TILE_OFF;
}

DLLEXPORT int LibGetSkillTileOff(VOID_ARGS)
{
    return GLYPH_SKILL_TILE_OFF;
}

DLLEXPORT int LibGetCommandTileOff(VOID_ARGS)
{
    return GLYPH_COMMAND_TILE_OFF;
}

DLLEXPORT int LibGetBuffTileOff(VOID_ARGS)
{
    return GLYPH_BUFF_OFF;
}

DLLEXPORT int LibGetCursorOff(VOID_ARGS)
{
    return GLYPH_CURSOR_OFF;
}

DLLEXPORT int LibTest(VOID_ARGS)
{
    return 5;
}

DLLEXPORT const char* LibGetVersionString(VOID_ARGS)
{
    return VERSION_STRING;
}

DLLEXPORT const char* LibGetVersionId(VOID_ARGS)
{
    return VERSION_ID;
}

DLLEXPORT uint64_t LibGetVersionNumber(VOID_ARGS)
{
    return VERSION_NUMBER;
}

DLLEXPORT uint64_t LibGetVersionCompatibility(VOID_ARGS)
{
    return EARLIEST_COMPATIBLE_VERSION_NUMBER;
}

static const char* extcmdnames[256] = { 0 };

DLLEXPORT const char** LibGetExtendedCommands(VOID_ARGS)
{
    int i = 0, j = 0;
    for (i = 0; i < 256; i++)
        extcmdnames[i] = 0;

    i = 0;
    while (extcmdlist[i].ef_txt != 0)
    {
        if ((extcmdlist[i].flags & AUTOCOMPLETE) != 0 &&
            (wizard || (extcmdlist[i].flags & WIZMODECMD) == 0) &&
            (wizard || discover || CasualMode || (extcmdlist[i].flags & CASUALMODECMD) == 0)
           )
        {
            extcmdnames[j] = extcmdlist[i].ef_txt;
            j++;
        }
        i++;
    }

    return extcmdnames;
}

DLLEXPORT void LibSetPetMID(m_id)
unsigned int m_id;
{
    context.view_pet_mid = m_id;
}

DLLEXPORT const char* LibGetPropertyName(prop_index)
int prop_index;
{
    return get_property_name(prop_index);
}

char _dumplogbuf[BUFSZ];

DLLEXPORT char* LibDumplogDateString(startdate)
long startdate;
{
    Sprintf(_dumplogbuf, "%08lld%06lld",
        (long long)yyyymmdd(startdate), (long long)hhmmss(startdate));

    return _dumplogbuf;
}

DLLEXPORT int LibGetAnimationArraySize(VOID_ARGS)
{
    return MAX_ANIMATIONS;
}

DLLEXPORT struct animation_definition LibGetAnimationArrayElement(idx)
int idx;
{
    return animations[idx];
}

DLLEXPORT int LibGetEnlargementArraySize(VOID_ARGS)
{
    return MAX_ENLARGEMENTS;
}

DLLEXPORT struct enlargement_definition LibGetEnlargementArrayElement(idx)
int idx;
{
    return enlargements[idx];
}

DLLEXPORT int LibGetReplacementArraySize(VOID_ARGS)
{
    return MAX_REPLACEMENTS;
}

DLLEXPORT struct replacement_definition LibGetReplacementArrayElement(idx)
int idx;
{
    return replacements[idx];
}

DLLEXPORT int LibGetAutoDrawArraySize(VOID_ARGS)
{
    return MAX_AUTODRAWS;
}

DLLEXPORT struct autodraw_definition LibGetAutoDrawArrayElement(idx)
int idx;
{
    return autodraws[idx];
}

DLLEXPORT boolean LibGlyphIsExplosion(glyph)
int glyph;
{
    return glyph_is_explosion(glyph);
}

DLLEXPORT boolean LibGlyphIsZap(glyph)
int glyph;
{
    return glyph_is_zap(glyph);
}

DLLEXPORT boolean LibGlyphIsAnyDying(glyph)
int glyph;
{
    return glyph_is_dying_monster(glyph) || glyph_is_female_dying_monster(glyph) || glyph_is_dying_player(glyph);
}

DLLEXPORT int LibZapGlyphToCornerGlyph(adjglyph, adjflags, source_dir)
int adjglyph;
uint64_t adjflags;
int source_dir;
{
    return zap_glyph_to_corner_glyph(adjglyph, adjflags, source_dir);
}

DLLEXPORT int LibChmod(filename, mode)
const char* filename;
unsigned int mode;
{
#ifdef WIN32
    return 0;
#else
    return chmod(filename, (mode_t)mode);
#endif
}

DLLEXPORT void LibSaveAndRestoreSavedGame(save_style)
int save_style;
{
    if (context.game_started && program_state.something_worth_saving && !program_state.gameover 
        && !program_state.panicking && !program_state.in_tricked
        && !program_state.exiting && !program_state.freeing_dynamic_data
        && !saving && !restoring && !reseting && !check_pointing && !ignore_onsleep_autosave)
    {
        switch (save_style)
        {
        case 2: /* Checkpoint only and no wait */
#ifdef INSURANCE
            save_currentstate();
#endif
            break;
        case 1: /* Checkpoint only and wait for resume */
#ifdef INSURANCE
            save_currentstate();
#endif
            issue_parametered_gui_command(GUI_CMD_WAIT_FOR_RESUME, 0);
            break;
        default:
        case 0: /* Save game with checkpoint as a backup */
            /* Reset certain animations so they do not play upon loading */
            reset_monster_origin_coordinates(&youmonst);
            reset_all_monster_origin_coordinates();
            reset_all_object_origin_coordinates();
            reset_found_this_turn();

#ifdef INSURANCE
            save_currentstate(); /* In the case save fails */
#endif
            int saveres = dosave0(TRUE);
            issue_parametered_gui_command(GUI_CMD_WAIT_FOR_RESUME, saveres);
            if (saveres)
            {
                exit_hack_code = 1; /* reload upon exit */
                u.uhp = -1; /* universal game's over indicator */
                /* make sure they see the Saving message */
                display_nhwindow(WIN_MESSAGE, TRUE);
                exit_nhwindows((char*)0);
                nh_terminate(EXIT_SUCCESS);
            }
            else
            {
                (void)doredraw();
            }
            break;
        }
    }
    return;
}

DLLEXPORT void
LibTallyRealTime(VOID_ARGS)
{
    tally_realtime();
}

DLLEXPORT int
LibGetMaxManuals(VOID_ARGS)
{
    return MAX_MANUAL_TYPES;
}

DLLEXPORT int
LibGetFirstCatalogue()
{
    return FIRST_CATALOGUE;
}

DLLEXPORT int
LibGetNumCatalogues()
{
    return NUM_CATALOGUES;
}

DLLEXPORT int
LibIsDebug(VOID_ARGS)
{
#ifdef DEBUG
    return 1;
#else
    return 0;
#endif
}

DLLEXPORT int
LibGetTileAnimationIndexFromGlyph(int glyph)
{
    return get_tile_animation_index_from_glyph(glyph);
}

DLLEXPORT int
LibMaybeGetAnimatedTile(ntile, tile_animation_idx, play_type, interval_counter, frame_idx_ptr, main_tile_idx_ptr, mapAnimated, autodraw_ptr)
int ntile;
int tile_animation_idx;
enum animation_play_types play_type;
long interval_counter;
int* frame_idx_ptr, * main_tile_idx_ptr;
char* mapAnimated;
enum autodraw_types* autodraw_ptr; 
{
    return maybe_get_animated_tile(ntile, tile_animation_idx, play_type, interval_counter, frame_idx_ptr, main_tile_idx_ptr, mapAnimated, autodraw_ptr);
}


char gnhapi_putstr_buffer[BUFSZ * 4];

DLLEXPORT void gnhapi_raw_print(const char* text)
{
    char buf[UTF8BUFSZ] = "";
    if (text)
    {
        write_text2buf_utf8(buf, UTF8BUFSZ, text);
        if (*gnhapi_putstr_buffer)
            Strcat(gnhapi_putstr_buffer, "\n");
        Strcpy(eos(gnhapi_putstr_buffer), buf);
    }
}

DLLEXPORT void gnhapi_putstr_ex(winid wid, const char* text, int attr, int color, int append)
{
    gnhapi_raw_print(text);
}

DLLEXPORT void gnhapi_putstr_ex2(winid wid, const char* text, const char* attrs, const char* colors, int attr, int color, int append)
{
    gnhapi_raw_print(text);
}

DLLEXPORT void
gnhapi_issue_gui_command(int cmd_id, int cmd_param, int cmd_param2, const char* cmd_str)
{
    if (cmd_id == GUI_CMD_DEBUGLOG)
    {
        char buf[BUFSZ];
        Sprintf(buf, "DebugLog: %d, %s", cmd_param2, cmd_str);
        gnhapi_raw_print(buf);
    }
}

DLLEXPORT int
LibValidateSaveFile(const char* filename, char* output_str)
{
    int fd;
    int res = 0;
    *gnhapi_putstr_buffer = 0;

    struct window_procs oldprocs = windowprocs;
#ifdef SAFEPROCS
    windowprocs = *get_safe_procs(0);
#endif
    windowprocs.win_putstr_ex = gnhapi_putstr_ex;
    windowprocs.win_putstr_ex2 = gnhapi_putstr_ex2;
    windowprocs.win_raw_print = gnhapi_raw_print;
    windowprocs.win_issue_gui_command = gnhapi_issue_gui_command;

    Strcpy(SAVEF, filename);
#ifdef COMPRESS_EXTENSION
    SAVEF[strlen(SAVEF) - strlen(COMPRESS_EXTENSION)] = '\0';
#endif
    nh_uncompress(SAVEF);
    if ((fd = open_savefile()) >= 0) 
    {
        if (validate(fd, filename) == 0) 
        {
            res = 1;
        }
        (void)nhclose(fd);
    }
    nh_compress(SAVEF);
    windowprocs = oldprocs;

    if (output_str && *gnhapi_putstr_buffer)
    {
        char buf[UTF8BUFSZ * 4] = "";
        write_text2buf_utf8(buf, sizeof(buf), gnhapi_putstr_buffer);
        Strcpy(output_str, buf);
    }
    return res;
}

DLLEXPORT int
LibCheckCurrentFileDescriptor(const char* dir)
{
    if (!chdir(dir))
        return check_current_fd((const char*)0);
    else
        return -4;
}

DLLEXPORT void
LibReportFileDescriptors(VOID_ARGS)
{
    maybe_report_file_descriptors("File Descriptor Report Requested", 24);
}

DLLEXPORT int
LibIncreaseFileDescriptorLimitToAtLeast(int min_cur)
{
#if defined(UNIX) && defined (GNH_MOBILE)
    return increase_file_descriptor_limit_to_at_least((uint64_t)min_cur);
#else
    return 0;
#endif
}

DLLEXPORT int
LibGetFileDescriptorLimit(int is_max_limit)
{
#if defined(UNIX) && defined (GNH_MOBILE)
    return get_file_descriptor_limit((boolean)is_max_limit);
#else
    return 0;
#endif
}

DLLEXPORT void
LibSetCharacterClickAction(int new_value)
{
    flags.self_click_action = new_value != 0;
}

DLLEXPORT int
LibGetCharacterClickAction(VOID_ARGS)
{
    return (int)flags.self_click_action;
}

DLLEXPORT void
LibSetMouseCommand(int new_value, int is_middle)
{
    if (is_middle)
        flags.middle_click_command = (uchar)new_value;
    else
        flags.right_click_command = (uchar)new_value;
}

DLLEXPORT int
LibGetMouseCommand(int is_middle)
{
    if (is_middle)
        return flags.middle_click_command;
    else
        return flags.right_click_command;
}

DLLEXPORT const char*
LibGetEventPathForGHSound(int ghsound)
{
    if (ghsound < 0 || ghsound >= MAX_GHSOUNDS)
        return "";

    return ghsound2event[ghsound].eventPath;
}

DLLEXPORT float
LibGetVolumeForGHSound(int ghsound)
{
    if (ghsound < 0 || ghsound >= MAX_GHSOUNDS)
        return 0.0f;

    return ghsound2event[ghsound].volume;
}


DLLEXPORT int GnollHackStart(cmdlineargs)
char* cmdlineargs;
{
#define MAX_CMD_PARAMS 24

    char cmdbuf[BUFSZ + MAX_CMD_PARAMS * PL_NSIZ];
    Strcpy(cmdbuf, cmdlineargs);
    char parambufs[MAX_CMD_PARAMS][PL_NSIZ + 10] = { "" };

    Strcpy(parambufs[0], "gnollhack");

    int curparamcnt = 1;
    if (cmdlineargs)
    {
        boolean isend = FALSE;
        char* bp = cmdbuf;
        char* ebp = bp;
        while (*bp)
        {         
            do
            {
                ebp++;
            } while (*ebp != 0 && *ebp != ' ');

            if (!*ebp)
                isend = TRUE;
            else
                *ebp = 0;

            if (*bp)
            {
                Strcpy(parambufs[curparamcnt], bp);
                curparamcnt++;
            }

            if (isend || curparamcnt >= MAX_CMD_PARAMS)
                break;

            ebp++;
            bp = ebp;
        }
    }

    /* Make a null terminated array just in case */
    char* params[MAX_CMD_PARAMS + 1] = { 0 };
    int i;
    for (i = 0; i < curparamcnt; i++)
    {
        params[i] = parambufs[i];
    }    

    return GnollHackMain(curparamcnt, params);
}

extern struct callback_procs lib_callbacks;

DLLEXPORT int RunGnollHack(
    char* gnhdir,
    char* cmdlineargs,
    char* preset_player_name,
    char* last_used_player_name,
    uint64_t runflags,
    uint64_t wincap1,
    uint64_t wincap2,
    InitWindowsCallback callback_init_nhwindows,
    PlayerSelectionCallback callback_player_selection,
    AskNameCallback callback_askname,
    GetEventCallback callback_get_nh_event,
    ExitWindowsCallback callback_exit_nhwindows,
    SuspendWindowsCallback callback_suspend_nhwindows,
    ResumeWindowsCallback callback_resume_nhwindows,
    CreateWindowCallback callback_create_nhwindow_ex,
    ClearWindowCallback callback_clear_nhwindow,
    DisplayWindowCallback callback_display_nhwindow,
    DestroyWindowCallback callback_destroy_nhwindow,
    CursCallback callback_curs,
    PutStrExCallback callback_putstr_ex,
    PutStrEx2Callback callback_putstr_ex2,
    PutMixedCallback callback_putmixed_ex,
    DisplayFileCallback callback_display_file,
    StartMenuCallback callback_start_menu_ex,
    AddMenuCallback callback_add_menu,
    AddExtendedMenuCallback callback_add_extended_menu,
    EndMenuCallback callback_end_menu_ex,
    SelectMenuCallback callback_select_menu,
    MessageMenuCallback callback_message_menu,
    UpdateInventoryCallback callback_update_inventory,
    MarkSynchCallback callback_mark_synch,
    WaitSynchCallback callback_wait_synch,

    /* If clipping is on */
    ClipAroundCallback callback_cliparound,
    /* If positionbar is on */
    UpdatePositionBarCallback callback_update_positionbar,

    PrintGlyphCallback callback_print_glyph,
    IssueGuiCommandCallback callback_issue_gui_command,
    RawPrintCallback callback_raw_print,
    RawPrintBoldCallback callback_raw_print_bold,
    GetChCallback callback_nhgetch,
    PosKeyCallback callback_nh_poskey,
    BellCallback callback_nhbell,
    DoPrevMessageCallback callback_doprev_message,
    YnFunctionCallback callback_yn_function_ex,
    GetLineCallback callback_getlin_ex,
    GetExtCmdCallback callback_get_ext_cmd,
    NumberPadCallback callback_number_pad,
    DelayOutputCallback callback_delay_output,
    DelayOutputMillisecondsCallback callback_delay_output_milliseconds,
    DelayOutputIntervalsCallback callback_delay_output_intervals,

    ChangeColorCallback callback_change_color,
    ChangeBackgroundCallback callback_change_background,
    SetFontNameCallback callback_set_font_name,
    GetColorStringCallback callback_get_color_string,

    StartScreenCallback callback_start_screen,
    EndScreenCallback callback_end_screen,
    OutRipCallback callback_outrip,
    PreferenceUpdateCallback callback_preference_update,
    GetMsgHistoryCallback callback_getmsghistory,
    PutMsgHistoryCallback callback_putmsghistory,
    StatusInitCallback callback_status_init,
    StatusFinishCallback callback_status_finish,
    StatusEnableFieldCallback callback_status_enablefield,
    StatusUpdateCallback callback_status_update,
    CanSuspendYesCallback callback_can_suspend_yes,
    StretchWindowCallback callback_stretch_window,
    SetAnimationTimerCallback callback_set_animation_timer_interval,
    OpenSpecialViewCallback callback_open_special_view,
    StopAllSoundsCallback callback_stop_all_sounds,
    PlayImmediateSoundCallback callback_play_immediate_ghsound,
    PlayOccupationAmbientCallback callback_play_ghsound_occupation_ambient,
    PlayEffectAmbientCallback callback_play_ghsound_effect_ambient,
    SetEffectAmbientVolumeCallback callback_set_effect_ambient_volume,
    PlayMusicCallback callback_play_ghsound_music,
    PlayLevelAmbientCallback callback_play_ghsound_level_ambient,
    PlayEnvironmentAmbientCallback callback_play_ghsound_environment_ambient,
    AdjustGeneralVolumesCallback callback_adjust_ghsound_general_volumes,
    AddAmbientSoundCallback callback_add_ambient_ghsound,
    DeleteAmbientSoundCallback callback_delete_ambient_ghsound,
    SetAmbientVolumeCallback callback_set_ambient_ghsound_volume,
    ClearContextMenuCallback callback_clear_context_menu,
    AddContextMenuCallback callback_add_context_menu,
    UpdateStatusButtonCallback callback_update_status_button,
    ToggleAnimationTimerCallback callback_toggle_animation_timer,
    DisplayFloatingTextCallback callback_display_floating_text,
    DisplayScreenTextCallback callback_display_screen_text,
    DisplayPopupTextCallback callback_display_popup_text,
    DisplayGUIEffectCallback callback_display_gui_effect,
    UpdateCursorCallback callback_update_cursor,
    UIHasInputCallback callback_ui_has_input,
    ExitHackCallback callback_exit_hack,

    GetCwdCallback callback_getcwd,
    MessageBoxCallback callback_messagebox,
    FreeMemoryCallback callback_free_memory,
    ReportPlayerNameCallback callback_report_player_name,
    ReportPlayTimeCallback callback_report_play_time,
    SendObjectDataCallback callback_send_object_data,
    SendMonsterDataCallback callback_send_monster_data,
    SendEngravingDataCallback callback_send_engraving_data
)
{
    char cmdbuf[BUFSZ] = "";
    if(cmdlineargs)
        Strcpy(cmdbuf, cmdlineargs);

    /* Set wincaps */
    if(runflags & GHRUNFLAGS_SET_WINCAPS)
        set_wincaps(wincap1, wincap2);

    /* Set names */
    /* Authorize wizard mode */
    if (runflags & GHRUNFLAGS_WIZARD_MODE)
    {
        //wizard = TRUE, discover = FALSE;
        //strcpy(plname, "wizard");
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Sprintf(eos(cmdbuf), "-D -u %s", "wizard");
    }
    else if (preset_player_name && strcmp(preset_player_name, ""))
    {
        char plbuf[PL_NSIZ];
        Strncpy(plbuf, preset_player_name, PL_NSIZ - 1);
        plbuf[PL_NSIZ - 1] = '\0';

        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Sprintf(eos(cmdbuf), "-u %s", plbuf);
    }
    else if ((runflags & GHRUNFLAGS_FORCE_LAST_PLAYER_NAME) && last_used_player_name && strcmp(last_used_player_name, ""))
    {
        char plbuf[PL_NSIZ];
        Strncpy(plbuf, last_used_player_name, PL_NSIZ - 1);
        plbuf[PL_NSIZ - 1] = '\0';

        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Sprintf(eos(cmdbuf), "-u %s", plbuf);
    }

    if (runflags & GHRUNFLAGS_MODERN_MODE)
    {
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-M");
    }

    if (runflags & GHRUNFLAGS_CASUAL_MODE)
    {
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-C");
    }

    if (runflags & GHRUNFLAGS_DISABLE_BONES)
    {
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-b");
    }

    if (runflags & GHRUNFLAGS_TOURNAMENT_MODE)
    {
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-T");
    }

    /* The following does the same as the default options file, but accessible easier from the GUI settings */
    memset(&initial_flags, 0, sizeof(initial_flags));
    if (runflags & GHRUNFLAGS_CHARACTER_CLICK_ACTION)
    {
        initial_flags.click_action_set = TRUE;
        initial_flags.click_action_value = TRUE;
    }

    initial_flags.right_click_action = (uchar)((runflags & GHRUNFLAGS_RIGHT_MOUSE_BIT_MASK) >> GHRUNFLAGS_RIGHT_MOUSE_BIT_INDEX);
    initial_flags.middle_click_action = (uchar)((runflags & GHRUNFLAGS_MIDDLE_MOUSE_BIT_MASK) >> GHRUNFLAGS_MIDDLE_MOUSE_BIT_INDEX);

    if (runflags & GHRUNFLAGS_NO_PET)
    {
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-P");
    }

    /* Set directly, as other parts of GnollHack do not purposedly set this */
    if (last_used_player_name && strcmp(last_used_player_name, ""))
    {
        Strncpy(recovery_plname, last_used_player_name, PL_NSIZ - 1);
        recovery_plname[PL_NSIZ - 1] = '\0';
    }

    is_gui_in_debug_mode = (runflags & GHRUNFLAGS_GUI_DEBUG_MODE) != 0;

    /* Set callback function pointers here */
    lib_callbacks.callback_init_nhwindows = callback_init_nhwindows;
    lib_callbacks.callback_player_selection = callback_player_selection;
    lib_callbacks.callback_askname = callback_askname;
    lib_callbacks.callback_get_nh_event = callback_get_nh_event;
    lib_callbacks.callback_exit_nhwindows = callback_exit_nhwindows;
    lib_callbacks.callback_suspend_nhwindows = callback_suspend_nhwindows;
    lib_callbacks.callback_resume_nhwindows = callback_resume_nhwindows;
    lib_callbacks.callback_create_nhwindow_ex = callback_create_nhwindow_ex;
    lib_callbacks.callback_clear_nhwindow = callback_clear_nhwindow;
    lib_callbacks.callback_display_nhwindow = callback_display_nhwindow;
    lib_callbacks.callback_destroy_nhwindow = callback_destroy_nhwindow;
    lib_callbacks.callback_curs = callback_curs;
    lib_callbacks.callback_putstr_ex = callback_putstr_ex;
    lib_callbacks.callback_putstr_ex2 = callback_putstr_ex2;
    lib_callbacks.callback_putmixed_ex = callback_putmixed_ex;
    lib_callbacks.callback_display_file = callback_display_file;
    lib_callbacks.callback_start_menu_ex = callback_start_menu_ex;
    lib_callbacks.callback_add_menu = callback_add_menu;
    lib_callbacks.callback_add_extended_menu = callback_add_extended_menu;
    lib_callbacks.callback_end_menu_ex = callback_end_menu_ex;
    lib_callbacks.callback_select_menu = callback_select_menu;
    lib_callbacks.callback_message_menu = callback_message_menu;
    lib_callbacks.callback_update_inventory = callback_update_inventory;
    lib_callbacks.callback_mark_synch = callback_mark_synch;
    lib_callbacks.callback_wait_synch = callback_wait_synch;
    lib_callbacks.callback_cliparound = callback_cliparound;
    lib_callbacks.callback_update_positionbar = callback_update_positionbar;
    lib_callbacks.callback_print_glyph = callback_print_glyph;
    lib_callbacks.callback_issue_gui_command = callback_issue_gui_command;
    lib_callbacks.callback_raw_print = callback_raw_print;
    lib_callbacks.callback_raw_print_bold = callback_raw_print_bold;
    lib_callbacks.callback_nhgetch = callback_nhgetch;
    lib_callbacks.callback_nh_poskey = callback_nh_poskey;
    lib_callbacks.callback_nhbell = callback_nhbell;
    lib_callbacks.callback_nhbell = callback_nhbell;
    lib_callbacks.callback_doprev_message = callback_doprev_message;
    lib_callbacks.callback_yn_function_ex = callback_yn_function_ex;
    lib_callbacks.callback_getlin_ex = callback_getlin_ex;
    lib_callbacks.callback_yn_function_ex = callback_yn_function_ex;
    lib_callbacks.callback_get_ext_cmd = callback_get_ext_cmd;
    lib_callbacks.callback_number_pad = callback_number_pad;
    lib_callbacks.callback_delay_output = callback_delay_output;
    lib_callbacks.callback_delay_output_milliseconds = callback_delay_output_milliseconds;
    lib_callbacks.callback_delay_output_intervals = callback_delay_output_intervals;
    lib_callbacks.callback_change_color = callback_change_color;
    lib_callbacks.callback_change_background = callback_change_background;
    lib_callbacks.callback_set_font_name = callback_set_font_name;
    lib_callbacks.callback_get_color_string = callback_get_color_string;
    lib_callbacks.callback_start_screen = callback_start_screen;
    lib_callbacks.callback_end_screen = callback_end_screen;
    lib_callbacks.callback_outrip = callback_outrip;
    lib_callbacks.callback_preference_update = callback_preference_update;
    lib_callbacks.callback_getmsghistory = callback_getmsghistory;
    lib_callbacks.callback_putmsghistory = callback_putmsghistory;
    lib_callbacks.callback_status_init = callback_status_init;
    lib_callbacks.callback_status_finish = callback_status_finish;
    lib_callbacks.callback_status_enablefield = callback_status_enablefield;
    lib_callbacks.callback_status_update = callback_status_update;
    lib_callbacks.callback_can_suspend_yes = callback_can_suspend_yes;
    lib_callbacks.callback_stretch_window = callback_stretch_window;
    lib_callbacks.callback_set_animation_timer_interval = callback_set_animation_timer_interval;
    lib_callbacks.callback_open_special_view = callback_open_special_view;
    lib_callbacks.callback_stop_all_sounds = callback_stop_all_sounds;
    lib_callbacks.callback_play_immediate_ghsound = callback_play_immediate_ghsound;
    lib_callbacks.callback_play_ghsound_occupation_ambient = callback_play_ghsound_occupation_ambient;
    lib_callbacks.callback_play_ghsound_effect_ambient = callback_play_ghsound_effect_ambient;
    lib_callbacks.callback_set_effect_ambient_volume = callback_set_effect_ambient_volume;
    lib_callbacks.callback_play_ghsound_music = callback_play_ghsound_music;
    lib_callbacks.callback_play_ghsound_level_ambient = callback_play_ghsound_level_ambient;
    lib_callbacks.callback_play_ghsound_environment_ambient = callback_play_ghsound_environment_ambient;
    lib_callbacks.callback_adjust_ghsound_general_volumes = callback_adjust_ghsound_general_volumes;
    lib_callbacks.callback_add_ambient_ghsound = callback_add_ambient_ghsound;
    lib_callbacks.callback_delete_ambient_ghsound = callback_delete_ambient_ghsound;
    lib_callbacks.callback_set_ambient_ghsound_volume = callback_set_ambient_ghsound_volume;
    lib_callbacks.callback_clear_context_menu = callback_clear_context_menu;
    lib_callbacks.callback_add_context_menu = callback_add_context_menu;
    lib_callbacks.callback_update_status_button = callback_update_status_button;
    lib_callbacks.callback_toggle_animation_timer = callback_toggle_animation_timer;
    lib_callbacks.callback_display_floating_text = callback_display_floating_text;
    lib_callbacks.callback_display_screen_text = callback_display_screen_text;
    lib_callbacks.callback_display_popup_text = callback_display_popup_text;
    lib_callbacks.callback_display_gui_effect = callback_display_gui_effect;
    lib_callbacks.callback_update_cursor = callback_update_cursor;
    lib_callbacks.callback_ui_has_input = callback_ui_has_input;
    lib_callbacks.callback_exit_hack = callback_exit_hack;
    lib_callbacks.callback_getcwd = callback_getcwd;
    lib_callbacks.callback_messagebox = callback_messagebox;

    lib_callbacks.callback_free_memory = callback_free_memory;
    lib_callbacks.callback_report_player_name = callback_report_player_name;
    lib_callbacks.callback_report_play_time = callback_report_play_time;
    lib_callbacks.callback_send_object_data = callback_send_object_data;
    lib_callbacks.callback_send_monster_data = callback_send_monster_data;
    lib_callbacks.callback_send_engraving_data = callback_send_engraving_data;

    windowprocs.win_raw_print = libdef_raw_print;
    windowprocs.win_raw_print_bold = libdef_raw_print_bold;
    windowprocs.win_wait_synch = libdef_wait_synch;

    int res = chdir(gnhdir);
    if (res != 0)
    {
        /* Failed to change to right directory */
        return 0;
    }

    /* Start GnollHack by calling main */
    return GnollHackStart(cmdbuf);
}

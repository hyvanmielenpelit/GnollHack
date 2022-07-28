/* GnollHack 4.1.0 gnhapi.c */
/* Copyright (c) Janne Gustafsson, 2022. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "gnhapi.h"
#include "hack.h"
#include "func_tab.h"
#include "libproc.h"
#include "callback.h"
#include "date_unix.h"
#include <sys/stat.h>

extern int FDECL(GnollHackMain, (int, char**));
extern void FDECL(set_wincaps, (unsigned long, unsigned long));

void GetGlyph2Tile(int** gl2ti_ptr, int* size_ptr)
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

void GetGlyphTileFlags(uchar** gltifl_ptr, int* size_ptr)
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

void GetTile2Animation(short** ti2an_ptr, int* size_ptr)
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

void GetTile2Enlargement(short** ti2en_ptr, int* size_ptr)
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

void GetTile2Replacement(short** ti2re_ptr, int* size_ptr)
{
    if (!ti2re_ptr || !size_ptr)
        return;
#ifdef USE_TILES
    *ti2re_ptr = tile2replacement;
    *size_ptr = SIZE(tile2replacement);
#else
    *ti2re_ptr = 0;
    *size_ptr = 0;
#endif
}

void GetTile2Autodraw(short** ti2ad_ptr, int* size_ptr)
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

void GetAnimationOffsets(int** anoff_ptr, int* size_ptr)
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

void GetEnlargementOffsets(int** enoff_ptr, int* size_ptr)
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

void GetReplacementOffsets(int** reoff_ptr, int* size_ptr)
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
int CountTotalTiles()
{
    return process_tiledata(2, (const char*)0, (int*)0, (uchar*)0);
}

int LibGetUnexploredGlyph()
{
    return base_cmap_to_glyph(S_unexplored);
}

int LibGetNoGlyph()
{
    return NO_GLYPH;
}

int LibGetAnimationOff()
{
    return GLYPH_ANIMATION_OFF;
}

int LibGetEnlargementOff()
{
    return GLYPH_ENLARGEMENT_OFF;
}

int LibGetReplacementOff()
{
    return GLYPH_ANIMATION_OFF;
}

int LibGetGeneralTileOff()
{
    return GLYPH_GENERAL_TILE_OFF;
}

int LibGetHitTileOff()
{
    return GLYPH_HIT_TILE_OFF;
}

int LibGetUITileOff()
{
    return GLYPH_UI_TILE_OFF;
}

int LibGetSpellTileOff()
{
    return GLYPH_SPELL_TILE_OFF;
}

int LibGetSkillTileOff()
{
    return GLYPH_SKILL_TILE_OFF;
}

int LibGetBuffTileOff()
{
    return GLYPH_BUFF_OFF;
}

int LibGetCursorOff()
{
    return GLYPH_CURSOR_OFF;
}

int LibTest()
{
    return 5;
}

const char* LibGetVersionString()
{
    return VERSION_STRING;
}

const char* LibGetVersionId()
{
    return VERSION_ID;
}

static const char* extcmdnames[256] = { 0 };
const char** LibGetExtendedCommands()
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


void LibSetPetMID(unsigned int m_id)
{
    context.view_pet_mid = m_id;
}

const char* LibGetPropertyName(prop_index)
int prop_index;
{
    return get_property_name(prop_index);
}

char _dumplogbuf[BUFSZ];

char* LibDumplogDateString(startdate)
long startdate;
{
    Sprintf(_dumplogbuf, "%08ld%06ld",
        yyyymmdd(startdate), hhmmss(startdate));

    return _dumplogbuf;
}


int LibGetAnimationArraySize()
{
    return MAX_ANIMATIONS;
}

struct animation_definition LibGetAnimationArrayElement(idx)
int idx;
{
    return animations[idx];
}

int LibGetEnlargementArraySize()
{
    return MAX_ENLARGEMENTS;
}

struct enlargement_definition LibGetEnlargementArrayElement(idx)
int idx;
{
    return enlargements[idx];
}

int LibGetReplacementArraySize()
{
    return MAX_REPLACEMENTS;
}

struct replacement_definition LibGetReplacementArrayElement(idx)
int idx;
{
    return replacements[idx];
}

int LibGetAutoDrawArraySize()
{
    return MAX_AUTODRAWS;
}

struct autodraw_definition LibGetAutoDrawArrayElement(idx)
int idx;
{
    return autodraws[idx];
}

boolean LibGlyphIsExplosion(glyph)
int glyph;
{
    return glyph_is_explosion(glyph);
}

boolean LibGlyphIsZap(glyph)
int glyph;
{
    return glyph_is_zap(glyph);
}

boolean LibGlyphIsAnyDying(int glyph)
{
    return glyph_is_dying_monster(glyph) || glyph_is_female_dying_monster(glyph) || glyph_is_dying_player(glyph);
}

int LibZapGlyphToCornerGlyph(int adjglyph, unsigned long adjflags, int source_dir)
{
    return zap_glyph_to_corner_glyph(adjglyph, adjflags, source_dir);
}

void
LibSwitchDemoVersion(int state)
{
    In_Demo = (state != 0);
}

int
LibChmod(const char* filename, unsigned int mode)
{
    return chmod(filename, (mode_t)mode);
}

void
LibSaveAndRestoreSavedGame(void)
{
    if (dosave0(TRUE))
    {
        issue_gui_command(GUI_CMD_WAIT_FOR_RESUME);
        if (!load_saved_game(2))
        {
            u.uhp = -1; /* universal game's over indicator */
            /* make sure they see the Saving message */
            display_nhwindow(WIN_MESSAGE, TRUE);
            exit_nhwindows("Cannot continue the game...");
            nh_terminate(EXIT_SUCCESS);
        }
    }
    else
        (void)doredraw();
}

int GnollHackStart(cmdlineargs)
char* cmdlineargs;
{
#define MAX_CMD_PARAMS 16

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

int RunGnollHack(
    char* gnhdir,
    char* cmdlineargs,
    char* preset_player_name,
    char* recovery_name, /* Player name for which recovery is tested first; effectively should be the last used player name */
    unsigned long runflags,
    unsigned long wincap1,
    unsigned long wincap2,
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
    //OutRipBeginCallback callback_outrip_begin,
    //OutRipEndCallback callback_outrip_end,

    FreeMemoryCallback callback_free_memory,
    ReportPlayerNameCallback callback_report_player_name,
    SendObjectDataCallback callback_send_object_data,
    SendMonsterDataCallback callback_send_monster_data
)
{
    char cmdbuf[BUFSZ] = "";
    if(cmdlineargs)
        strcpy(cmdbuf, cmdlineargs);

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
        strncpy(plbuf, preset_player_name, PL_NSIZ - 1);
        plbuf[PL_NSIZ - 1] = '\0';

        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Sprintf(eos(cmdbuf), "-u %s", plbuf);
    }

    if (runflags & GHRUNFLAGS_MODERN_MODE)
    {
        //ModernMode = TRUE;
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-M");
    }

    if (runflags & GHRUNFLAGS_CASUAL_MODE)
    {
        //CasualMode = TRUE;
        if (*cmdbuf)
            Strcat(cmdbuf, " ");
        Strcat(cmdbuf, "-C");
    }


    /* Set directly, as other parts of GnollHack do not set these */
    if (recovery_name && strcmp(recovery_name, ""))
    {
        strncpy(recovery_plname, recovery_name, PL_NSIZ - 1);
        recovery_plname[PL_NSIZ - 1] = '\0';
    }
    if (!(runflags & GHRUNFLAGS_FULL_VERSION))
    {
        In_Demo = TRUE;
    }

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
    //lib_callbacks.callback_outrip_begin = callback_outrip_begin;
    //lib_callbacks.callback_outrip_end = callback_outrip_end;

    lib_callbacks.callback_free_memory = callback_free_memory;
    lib_callbacks.callback_report_player_name = callback_report_player_name;
    lib_callbacks.callback_send_object_data = callback_send_object_data;
    lib_callbacks.callback_send_monster_data = callback_send_monster_data;

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

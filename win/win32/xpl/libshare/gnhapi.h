/* 
 * gnhapi.h 
 * Copyright (c) Janne Gustafsson, 2022
 */

#ifndef GNHAPI_H
#define GNHAPI_H

#include "callback.h"
#include "tradstdc.h"

DLLEXPORT int FDECL(GnollHackStart, (char*));

#define GHRUNFLAGS_NONE                     0x00000000UL
#define GHRUNFLAGS_SET_WINCAPS              0x00000001UL
#define GHRUNFLAGS_WIZARD_MODE              0x00000002UL
#define GHRUNFLAGS_FULL_VERSION             0x00000004UL
#define GHRUNFLAGS_MODERN_MODE              0x00000008UL
#define GHRUNFLAGS_CASUAL_MODE              0x00000010UL
#define GHRUNFLAGS_DISABLE_BONES            0x00000020UL
#define GHRUNFLAGS_FORCE_LAST_PLAYER_NAME   0x00000040UL
#define GHRUNFLAGS_PLAYING_REPLAY           0x00000080UL
#define GHRUNFLAGS_TOURNAMENT_MODE          0x00000100UL
#define GHRUNFLAGS_GUI_DEBUG_MODE           0x00000200UL
#define GHRUNFLAGS_CHARACTER_CLICK_ACTION   0x00000400UL
#define GHRUNFLAGS_NO_PET                   0x00000800UL
#define GHRUNFLAGS_DICE_AS_RANGES           0x00001000UL
#define GHRUNFLAGS_GETPOS_ARROWS            0x00002000UL
#define GHRUNFLAGS_SAVE_FILE_TRACKING_SUPPORTED 0x00004000UL
#define GHRUNFLAGS_SAVE_FILE_TRACKING_NEEDED    0x00008000UL
#define GHRUNFLAGS_SAVE_FILE_TRACKING_ON    0x00010000UL
#define GHRUNFLAGS_AUTO_DIG                 0x00020000UL
#define GHRUNFLAGS_IGNORE_STOPPING          0x00040000UL

#define GHRUNFLAGS_RIGHT_MOUSE_BIT_1        0x00400000UL
#define GHRUNFLAGS_RIGHT_MOUSE_BIT_2        0x00800000UL
#define GHRUNFLAGS_RIGHT_MOUSE_BIT_3        0x01000000UL
#define GHRUNFLAGS_RIGHT_MOUSE_BIT_4        0x02000000UL
#define GHRUNFLAGS_RIGHT_MOUSE_BIT_5        0x04000000UL

#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_1       0x08000000UL
#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_2       0x10000000UL
#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_3       0x20000000UL
#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_4       0x40000000UL
#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_5       0x80000000UL

#define GHRUNFLAGS_RIGHT_MOUSE_BIT_MASK     (0x07000000UL | GHRUNFLAGS_RIGHT_MOUSE_BIT_1 | GHRUNFLAGS_RIGHT_MOUSE_BIT_2)
#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_MASK    0xF8000000UL
#define GHRUNFLAGS_RIGHT_MOUSE_BIT_INDEX    22
#define GHRUNFLAGS_MIDDLE_MOUSE_BIT_INDEX   27

/* Main GnollHack routine with all the necessary callback functions */
DLLEXPORT int RunGnollHack(
    char* gnhdir,
    char* cmdlineargs,
    char* preset_player_name,
    char* recovery_name,
    uint64_t runflags,
    uint64_t foundmanuals,
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
);

#endif
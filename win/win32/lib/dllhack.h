/*
 * dllhack.h
 * Copyright (c) Janne Gustafsson, 2021
 */

#include <windows.h>
#include "callback.h"
#include "tradstdc.h"

#ifndef DLLHACK_H
#define DLLHACK_H

/* Functions in dllproc.c */
DLL void FDECL(dll_reduce_counters_intervals, (int));

/* Test functions*/
DLL int NDECL(DoSomeCalc2);

/* Utility functions in dllproc.c */
DLL unsigned char FDECL(dll_validrole, (int));
DLL unsigned char FDECL(dll_validrace, (int, int));
DLL unsigned char FDECL(dll_validgend, (int, int, int));
DLL unsigned char FDECL(dll_validalign, (int, int, int));
DLL int FDECL(dll_randrole, (BOOLEAN_P));
DLL int FDECL(dll_randrace, (int));
DLL int FDECL(dll_randgend, (int, int));
DLL int FDECL(dll_randalign, (int, int));
DLL int FDECL(dll_str2role, (const char*));
DLL int FDECL(dll_str2race, (const char*));
DLL int FDECL(dll_str2gend, (const char*));
DLL int FDECL(dll_str2align, (const char*));
DLL unsigned char FDECL(dll_ok_role, (int, int, int, int));
DLL int FDECL(dll_pick_role, (int, int, int, int));
DLL unsigned char FDECL(dll_ok_race, (int, int, int, int));
DLL int FDECL(dll_pick_race, (int, int, int, int));
DLL unsigned char FDECL(dll_ok_gend, (int, int, int, int));
DLL int FDECL(dll_pick_gend, (int, int, int, int));
DLL unsigned char FDECL(dll_ok_align, (int, int, int, int));
DLL int FDECL(dll_pick_align, (int, int, int, int));


/* Main GnollHack routine with all the necessary callback functions in dllmain.c */
DLL int RunGnollHack(
    char* gnhdir,
    char* cmdlineargs,
    char* preset_player_name,
    char* recovery_name,
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
    PrintGlyphSimpleCallback callback_print_glyph_simple,
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

);

DLL int RunGnollHackSimple(
    unsigned long wincap1,
    unsigned long wincap2);

DLL int RunGnollHackSimple2(
    unsigned long wincap1,
    unsigned long wincap2,
    InitWindowsCallback callback_init_nhwindows);


#endif //DLLHACK_H

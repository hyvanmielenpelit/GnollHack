/* GnollHack 4.1.0 callback.h */
/* Copyright (c) Janne Gustafsson, 2021. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef CALLBACK_H
#define CALLBACK_H

#include "config.h"
#include "general.h"
#include "layer.h"

#ifdef WIN32
#define __callconv __stdcall
#define DLLEXPORT __declspec(dllexport)
#define OSAPI WINAPI
#else
#define __callconv 
#define DLLEXPORT
#define OSAPI
#endif

typedef char cschar; /* For use with C# chars in callback functions; assuming (default) value of CharSet.Ansi */

struct objclassdata
{
    int tile_height;
    short special_quality;
    short max_charges;
    uchar nh_color;

    /* Temporary extra data */
    uchar lamplit;
    uchar poisoned;
    uchar eroded;
    uchar eroded2;

    /* Calculated values */
    uchar corrodeable;
    uchar rottable;
    uchar flammable;
    uchar rustprone;
    uchar poisonable;

    uchar is_uchain;
    uchar is_uball;
    schar obj_loc_x;
    schar obj_loc_y;
};

 /* General callback types */
typedef void(__callconv* VoidVoidCallback)();
typedef void(__callconv* VoidConstCharCallback)(const char*);
typedef void(__callconv* VoidIntCallback)(int);
typedef int(__callconv* IntVoidCallback)();
typedef char*(__callconv* CharVoidCallback)();

/* Specific callback types */
typedef VoidVoidCallback InitWindowsCallback;
typedef IntVoidCallback PlayerSelectionCallback;
typedef int (__callconv* AskNameCallback)(const char*, const char*, char*);
typedef VoidVoidCallback GetEventCallback;
typedef VoidConstCharCallback ExitWindowsCallback;
typedef VoidConstCharCallback SuspendWindowsCallback;
typedef VoidVoidCallback ResumeWindowsCallback;
typedef int(__callconv* CreateWindowCallback)(int, int, int, uchar, struct obj*, struct objclassdata*);
typedef VoidIntCallback ClearWindowCallback;
typedef void(__callconv* DisplayWindowCallback)(int, uchar);
typedef VoidIntCallback DestroyWindowCallback;
typedef void(__callconv* CursCallback)(int, int, int);
typedef void(__callconv* PutStrExColorCallback)(int, const char*, int, int, int);
typedef void(__callconv* PutStrEx2ColorCallback)(int, const char*, const char*, const char*, int, int, int);
typedef PutStrExColorCallback PutStrExCallback;
typedef PutStrEx2ColorCallback PutStrEx2Callback;
typedef PutStrExColorCallback PutMixedCallback;
typedef void(__callconv* DisplayFileCallback)(const char*, uchar);
typedef void(__callconv* StartMenuCallback)(int, int);
typedef void(__callconv* AddMenuCallback)(int, int, int64_t, cschar, cschar, int, int, const char*, uchar);
typedef void(__callconv* AddExtendedMenuCallback)(int, int, int64_t, cschar, cschar, int, int, const char*, uchar, int,
    uint64_t, uint64_t, cschar, cschar, uint64_t, uchar, int, struct obj*, struct objclassdata*, const char*, const char*);
typedef void(__callconv* EndMenuCallback)(int, const char*, const char*);
typedef int(__callconv* SelectMenuCallback)(int, int, int64_t**, int*);
typedef void(__callconv* MessageMenuCallback)(int);
typedef VoidVoidCallback UpdateInventoryCallback;
typedef VoidVoidCallback MarkSynchCallback;
typedef VoidVoidCallback WaitSynchCallback;
typedef void(__callconv* ClipAroundCallback)(int, int, uchar);
typedef void(__callconv* UpdatePositionBarCallback)(char*);
typedef void(__callconv* PrintGlyphCallback)(int, int, int, int, int, int32_t, int, uint32_t, struct layer_info*);
typedef void(__callconv* IssueGuiCommandCallback)(int, int, int, const char*);
typedef VoidConstCharCallback RawPrintCallback;
typedef VoidConstCharCallback RawPrintBoldCallback;
typedef IntVoidCallback GetChCallback;
typedef int(__callconv* PosKeyCallback)(int*, int*, int*);
typedef VoidVoidCallback BellCallback;
typedef IntVoidCallback DoPrevMessageCallback;
typedef int(__callconv* GetLineCallback)(int, int, int, const char*, const char*, const char*, const char*, char*);
typedef int(__callconv* YnFunctionCallback)(int, int, int, int, const char*, const char*, const char*, const char*, const char*, const char*, uint64_t);
typedef IntVoidCallback GetExtCmdCallback;
typedef VoidIntCallback NumberPadCallback;
typedef VoidVoidCallback DelayOutputCallback;
typedef VoidIntCallback DelayOutputMillisecondsCallback;
typedef VoidIntCallback DelayOutputIntervalsCallback;
typedef VoidVoidCallback ChangeColorCallback;
typedef VoidIntCallback ChangeBackgroundCallback;
typedef void(__callconv* SetFontNameCallback)(int);
typedef CharVoidCallback GetColorStringCallback;
typedef VoidVoidCallback StartScreenCallback;
typedef VoidVoidCallback EndScreenCallback;
typedef void(__callconv* OutRipCallback)(int, char*, int, char*, char*);
typedef VoidConstCharCallback PreferenceUpdateCallback;
typedef int (__callconv* GetMsgHistoryCallback)(char*, char*, char*, uchar);
typedef void(__callconv* PutMsgHistoryCallback)(const char*, const char*, const char*, uchar);
typedef VoidIntCallback StatusInitCallback;
typedef VoidVoidCallback StatusFinishCallback;
typedef void(__callconv* StatusEnableFieldCallback)(int, const char*, const char*, uchar);
typedef void(__callconv* StatusUpdateCallback)(int, char*, int64_t, int, int, int, short*);
typedef uchar(__callconv* CanSuspendYesCallback)();
typedef VoidVoidCallback StretchWindowCallback;
typedef void(__callconv* SetAnimationTimerCallback)(uint64_t);
typedef int(__callconv* OpenSpecialViewCallback)(int, const char*, const char*, int, int);
typedef int(__callconv* StopAllSoundsCallback)(unsigned int, unsigned int);
typedef int(__callconv* PlayImmediateSoundCallback)(int, const char*, int, double, double, const char**, float*, int, int, int, uint32_t, uint32_t);
typedef int(__callconv* PlayMusicCallback)(int, const char*, int, double, double);
typedef PlayMusicCallback PlayOccupationAmbientCallback;
typedef PlayMusicCallback PlayEffectAmbientCallback;
typedef int(__callconv* SetEffectAmbientVolumeCallback)(double);
typedef PlayMusicCallback PlayLevelAmbientCallback;
typedef PlayMusicCallback PlayEnvironmentAmbientCallback;
typedef uchar(__callconv* AdjustGeneralVolumesCallback)(double, double, double, double, double, double);
typedef int(__callconv* AddAmbientSoundCallback)(int, const char*, int, double, double, uint64_t*);
typedef int(__callconv* DeleteAmbientSoundCallback)(uint64_t);
typedef int(__callconv* SetAmbientVolumeCallback)(uint64_t, double);
typedef void(__callconv* ClearContextMenuCallback)(void);
typedef void(__callconv* AddContextMenuCallback)(int, int, int, int, const char*, const char*, int, int);
typedef void(__callconv* UpdateStatusButtonCallback)(int, int, int, uint64_t);
typedef void(__callconv* ToggleAnimationTimerCallback)(int, int, int, int, int, int, uint64_t);
typedef void(__callconv* DisplayFloatingTextCallback)(int, int, const char*, int, int, int, uint64_t);
typedef void(__callconv* DisplayScreenTextCallback)(const char*, const char*, const char*, int, int, int, uint64_t);
typedef void(__callconv* DisplayPopupTextCallback)(const char*, const char*, int, int, int, int, uint64_t);
typedef void(__callconv* DisplayGUIEffectCallback)(int, int, int, int, int, int, uint64_t);
typedef void(__callconv* UpdateCursorCallback)(int, int, int);
typedef int(__callconv* UIHasInputCallback)(void);
typedef void(__callconv* ExitHackCallback)(int);

typedef char*(__callconv* GetCwdCallback)();
typedef int (__callconv* MessageBoxCallback)(char*, char*, unsigned int);
typedef void(__callconv* FreeMemoryCallback)(int64_t**);
typedef void (__callconv* ReportPlayerNameCallback)(char*);
typedef void (__callconv* ReportPlayTimeCallback)(int64_t, int64_t);
typedef void(__callconv* SendObjectDataCallback)(int, int, struct obj*, int, int, struct objclassdata*, uint64_t);
typedef void(__callconv* SendMonsterDataCallback)(int, int, int, struct monst_info*, uint64_t);
typedef void(__callconv* SendEngravingDataCallback)(int, int, int, const char*, int, uint64_t, uint64_t);

struct callback_procs {
    InitWindowsCallback callback_init_nhwindows;
    PlayerSelectionCallback callback_player_selection;
    AskNameCallback callback_askname;
    GetEventCallback callback_get_nh_event;
    ExitWindowsCallback callback_exit_nhwindows;
    SuspendWindowsCallback callback_suspend_nhwindows;
    ResumeWindowsCallback callback_resume_nhwindows;
    CreateWindowCallback callback_create_nhwindow_ex;
    ClearWindowCallback callback_clear_nhwindow;
    DisplayWindowCallback callback_display_nhwindow;
    DestroyWindowCallback callback_destroy_nhwindow;
    CursCallback callback_curs;
    PutStrExCallback callback_putstr_ex;
    PutStrEx2Callback callback_putstr_ex2;
    PutMixedCallback callback_putmixed_ex;
    DisplayFileCallback callback_display_file;
    StartMenuCallback callback_start_menu_ex;
    AddMenuCallback callback_add_menu;
    AddExtendedMenuCallback callback_add_extended_menu;
    EndMenuCallback callback_end_menu_ex;
    SelectMenuCallback callback_select_menu;
    MessageMenuCallback callback_message_menu;
    UpdateInventoryCallback callback_update_inventory;
    MarkSynchCallback callback_mark_synch;
    WaitSynchCallback callback_wait_synch;

    ClipAroundCallback callback_cliparound;
    UpdatePositionBarCallback callback_update_positionbar;
    
    PrintGlyphCallback callback_print_glyph;
    IssueGuiCommandCallback callback_issue_gui_command;
    RawPrintCallback callback_raw_print;
    RawPrintBoldCallback callback_raw_print_bold;
    GetChCallback callback_nhgetch;
    PosKeyCallback callback_nh_poskey;
    BellCallback callback_nhbell;
    DoPrevMessageCallback callback_doprev_message;
    YnFunctionCallback callback_yn_function_ex;
    GetLineCallback callback_getlin_ex;
    GetExtCmdCallback callback_get_ext_cmd;
    NumberPadCallback callback_number_pad;
    DelayOutputCallback callback_delay_output;
    DelayOutputMillisecondsCallback callback_delay_output_milliseconds;
    DelayOutputIntervalsCallback callback_delay_output_intervals;

    ChangeColorCallback callback_change_color;
    ChangeBackgroundCallback callback_change_background;
    SetFontNameCallback callback_set_font_name;
    GetColorStringCallback callback_get_color_string;
    
    StartScreenCallback callback_start_screen;
    EndScreenCallback callback_end_screen;
    OutRipCallback callback_outrip;
    PreferenceUpdateCallback callback_preference_update;
    GetMsgHistoryCallback callback_getmsghistory;
    PutMsgHistoryCallback callback_putmsghistory;
    StatusInitCallback callback_status_init;
    StatusFinishCallback callback_status_finish;
    StatusEnableFieldCallback callback_status_enablefield;
    StatusUpdateCallback callback_status_update;
    CanSuspendYesCallback callback_can_suspend_yes;
    StretchWindowCallback callback_stretch_window;
    SetAnimationTimerCallback callback_set_animation_timer_interval;
    OpenSpecialViewCallback callback_open_special_view;
    StopAllSoundsCallback callback_stop_all_sounds;
    PlayImmediateSoundCallback callback_play_immediate_ghsound;
    PlayOccupationAmbientCallback callback_play_ghsound_occupation_ambient;
    PlayEffectAmbientCallback callback_play_ghsound_effect_ambient;
    SetEffectAmbientVolumeCallback callback_set_effect_ambient_volume;
    PlayMusicCallback callback_play_ghsound_music;
    PlayLevelAmbientCallback callback_play_ghsound_level_ambient;
    PlayEnvironmentAmbientCallback callback_play_ghsound_environment_ambient;
    AdjustGeneralVolumesCallback callback_adjust_ghsound_general_volumes;
    AddAmbientSoundCallback callback_add_ambient_ghsound;
    DeleteAmbientSoundCallback callback_delete_ambient_ghsound;
    SetAmbientVolumeCallback callback_set_ambient_ghsound_volume;
    ClearContextMenuCallback callback_clear_context_menu;
    AddContextMenuCallback callback_add_context_menu;
    UpdateStatusButtonCallback callback_update_status_button;
    ToggleAnimationTimerCallback callback_toggle_animation_timer;
    DisplayFloatingTextCallback callback_display_floating_text;
    DisplayScreenTextCallback callback_display_screen_text;
    DisplayPopupTextCallback callback_display_popup_text;
    DisplayGUIEffectCallback callback_display_gui_effect;
    UpdateCursorCallback callback_update_cursor;
    UIHasInputCallback callback_ui_has_input;
    ExitHackCallback callback_exit_hack;

    GetCwdCallback callback_getcwd;
    MessageBoxCallback callback_messagebox;
    FreeMemoryCallback callback_free_memory;
    ReportPlayerNameCallback callback_report_player_name;
    ReportPlayTimeCallback callback_report_play_time;
    SendObjectDataCallback callback_send_object_data;
    SendMonsterDataCallback callback_send_monster_data;
    SendEngravingDataCallback callback_send_engraving_data;
};

extern struct objclassdata get_objclassdata(struct obj* otmp);

#endif /* CALLBACK_H */
/* GnollHack 4.1.0 callback.h */
/* Copyright (c) Janne Gustafsson, 2021. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "config.h"
#include "layer.h"
#include "general.h"

#ifndef CALLBACK_H
#define CALLBACK_H

#ifdef WIN32
#define __callconv __stdcall
#define DLL __declspec(dllexport)
#define OSAPI WINAPI
#else
#define __callconv 
#define DLL
#define OSAPI
#endif

struct objclassdata
{
    int tile_height;
    int special_quality;

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
};

 /* General callback types */
typedef void(__callconv* VoidVoidCallback)();
typedef void(__callconv* VoidCharCallback)(char*);
typedef void(__callconv* VoidConstCharCallback)(const char*);
typedef int(__callconv* IntIntCallback)(int);
typedef void(__callconv* VoidIntCallback)(int);
typedef void(__callconv* VoidIntIntBooleanCallback)(int, int, BOOLEAN_P);
typedef void(__callconv* VoidIntIntIntCallback)(int, int, int);
typedef void(__callconv* VoidIntBooleanCallback)(int, UCHAR_P);
typedef void(__callconv* VoidIntIntConstCharCallback)(int, int, const char*);
typedef void(__callconv* PutStrExColorCallback)(int, int, const char*, int, int);
typedef void(__callconv* PutStrEx2ColorCallback)(int, const char*, const char*, const char*, int);
typedef void(__callconv* VoidConstCharIntCallback)(const char*, int);
typedef void(__callconv* VoidConstCharBooleanCallback)(const char*, BOOLEAN_P);
typedef int(__callconv* IntVoidCallback)();
typedef unsigned char(__callconv* BooleanVoidCallback)();
typedef char*(__callconv* CharVoidCallback)();
typedef unsigned char(__callconv* BooleanIntDoubleCallback)(int, double);
typedef unsigned char(__callconv* BooleanIntDoubleVoidPtrCallback)(int, double, void*);
typedef unsigned char(__callconv* BooleanVoidPtrDoubleCallback)(void* , double);
typedef unsigned char(__callconv* BooleanVoidPtrCallback)(void*);
typedef char*(__callconv* GetLineCallback)(int, int, int, const char*, const char*, const char*, const char*);
typedef char*(__callconv* CharPtrBooleanCallback)(BOOLEAN_P);
typedef void(__callconv* VoidIntConstCharPtrConstCharPtrBooleanCallback)(int, const char*, const char*, UCHAR_P);

/* Specific callback types */
typedef VoidVoidCallback InitWindowsCallback;
typedef IntVoidCallback PlayerSelectionCallback;
typedef char* (__callconv* AskNameCallback)(const char*, const char*);
typedef VoidVoidCallback GetEventCallback;
typedef VoidConstCharCallback ExitWindowsCallback;
typedef VoidConstCharCallback SuspendWindowsCallback;
typedef VoidVoidCallback ResumeWindowsCallback;
typedef int(__callconv* CreateWindowCallback)(int, int, int, UCHAR_P, struct obj*, struct objclassdata*);
typedef VoidIntCallback ClearWindowCallback;
typedef VoidIntBooleanCallback DisplayWindowCallback;
typedef VoidIntCallback DestroyWindowCallback;
typedef VoidIntIntIntCallback CursCallback;
typedef PutStrExColorCallback PutStrExCallback;
typedef PutStrEx2ColorCallback PutStrEx2Callback;
typedef PutStrExColorCallback PutMixedCallback;
typedef VoidConstCharBooleanCallback DisplayFileCallback;
typedef void(__callconv* StartMenuCallback)(int, int);
typedef void(__callconv* AddMenuCallback)(int, int, long long, CHAR_P, CHAR_P, int, const char*, UCHAR_P, int);
typedef void(__callconv* AddExtendedMenuCallback)(int, int, long long, CHAR_P, CHAR_P, int, const char*, UCHAR_P, int, int,
    unsigned long long, unsigned long long, CHAR_P, CHAR_P, unsigned long, UCHAR_P, int, struct obj*, struct objclassdata*);
typedef void(__callconv* EndMenuCallback)(int, const char*, const char*);
typedef int(__callconv* SelectMenuCallback)(int, int, long long**, int*);
typedef void(__callconv* MessageMenuCallback)(int);
typedef VoidVoidCallback UpdateInventoryCallback;
typedef VoidVoidCallback MarkSynchCallback;
typedef VoidVoidCallback WaitSynchCallback;
typedef VoidIntIntBooleanCallback ClipAroundCallback;
typedef VoidCharCallback UpdatePositionBarCallback;
typedef void(__callconv* PrintGlyphCallback)(int, int, int, int, int, long, int, unsigned long, struct layer_info*);
typedef VoidIntCallback IssueGuiCommandCallback;
typedef VoidConstCharCallback RawPrintCallback;
typedef VoidConstCharCallback RawPrintBoldCallback;
typedef IntVoidCallback GetChCallback;
typedef int(__callconv* PosKeyCallback)(int*, int*, int*);
typedef VoidVoidCallback BellCallback;
typedef IntVoidCallback DoPrevMessageCallback;
typedef int(__callconv* YnFunctionCallback)(int, int, int, int, const char*, const char*, const char*, const char*, const char*, const char*, unsigned long);
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
typedef char* (__callconv* GetMsgHistoryCallback)(char**, char**, BOOLEAN_P);
typedef void(__callconv* PutMsgHistoryCallback)(const char*, const char*, const char*, BOOLEAN_P);
typedef VoidVoidCallback StatusInitCallback;
typedef VoidVoidCallback StatusFinishCallback;
typedef VoidIntConstCharPtrConstCharPtrBooleanCallback StatusEnableFieldCallback;
typedef void(__callconv* StatusUpdateCallback)(int, char*, long, int, int, int, short*);
typedef BooleanVoidCallback CanSuspendYesCallback;
typedef VoidVoidCallback StretchWindowCallback;
typedef void(__callconv* SetAnimationTimerCallback)(unsigned long);
typedef int(__callconv* OpenSpecialViewCallback)(int, const char*, const char*, int, int);
typedef int(__callconv* StopAllSoundsCallback)(unsigned int, unsigned int);
typedef int(__callconv* PlayImmediateSoundCallback)(int, const char*, int, double, double, const char**, float*, int, int, int, unsigned int, unsigned long);
typedef int(__callconv* PlayMusicCallback)(int, const char*, int, double, double);
typedef PlayMusicCallback PlayOccupationAmbientCallback;
typedef PlayMusicCallback PlayEffectAmbientCallback;
typedef int(__callconv* SetEffectAmbientVolumeCallback)(double);
typedef PlayMusicCallback PlayLevelAmbientCallback;
typedef PlayMusicCallback PlayEnvironmentAmbientCallback;
typedef unsigned char(__callconv* AdjustGeneralVolumesCallback)(double, double, double, double, double, double);
typedef int(__callconv* AddAmbientSoundCallback)(int, const char*, int, double, double, unsigned long long*);
typedef int(__callconv* DeleteAmbientSoundCallback)(unsigned long long);
typedef int(__callconv* SetAmbientVolumeCallback)(unsigned long long, double);
typedef void(__callconv* ClearContextMenuCallback)(void);
typedef void(__callconv* AddContextMenuCallback)(int, int, int, int, const char*, const char*, int, int);
typedef void(__callconv* UpdateStatusButtonCallback)(int, int, int, unsigned long);
typedef void(__callconv* ToggleAnimationTimerCallback)(int, int, int, int, int, int, unsigned long);
typedef void(__callconv* DisplayFloatingTextCallback)(int, int, const char*, int, int, int, unsigned long);
typedef void(__callconv* DisplayScreenTextCallback)(const char*, const char*, const char*, int, int, int, unsigned long);
typedef void(__callconv* DisplayPopupTextCallback)(const char*, const char*, int, int, int, int, unsigned long);
typedef void(__callconv* DisplayGUIEffectCallback)(int, int, int, unsigned long);
typedef void(__callconv* UpdateCursorCallback)(int, int, int);
typedef int(__callconv* UIHasInputCallback)(void);
typedef void(__callconv* ExitHackCallback)(int);

typedef char*(__callconv* GetCwdCallback)();
typedef int (__callconv* MessageBoxCallback)(char*, char*, unsigned int);
typedef VoidIntCallback OutRipBeginCallback;
typedef VoidIntCallback OutRipEndCallback;
typedef void(__callconv* FreeMemoryCallback)(long long**);
typedef void (__callconv* ReportPlayerNameCallback)(char*);
typedef void (__callconv* ReportPlayTimeCallback)(long, long);
typedef void(__callconv* SendObjectDataCallback)(int, int, struct obj*, int, int, struct objclassdata*, unsigned long);
typedef void(__callconv* SendMonsterDataCallback)(int, int, int, struct monst_info*, unsigned long);

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
    //OutRipBeginCallback callback_outrip_begin;
    //OutRipEndCallback callback_outrip_end;

    FreeMemoryCallback callback_free_memory;
    ReportPlayerNameCallback callback_report_player_name;
    ReportPlayTimeCallback callback_report_play_time;
    SendObjectDataCallback callback_send_object_data;
    SendMonsterDataCallback callback_send_monster_data;
};


extern int common_player_selection();
extern struct objclassdata get_objclassdata(struct obj* otmp);

#endif /* CALLBACK_H */
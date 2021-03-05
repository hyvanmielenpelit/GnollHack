/*
 * dllcallback.h
 * Copyright (c) Janne Gustafsson, 2021
 */

#ifndef DLLCALLBACK_H
#define DLLCALLBACK_H

#define DLL __declspec(dllexport)

 /* General callback types */
typedef void(__stdcall* VoidVoidCallback)();
typedef void(__stdcall* VoidCharCallback)(char*);
typedef void(__stdcall* VoidConstCharCallback)(const char*);
typedef int(__stdcall* IntIntCallback)(int);
typedef void(__stdcall* VoidIntCallback)(int);
typedef void(__stdcall* VoidIntIntCallback)(int, int);
typedef void(__stdcall* VoidIntIntIntCallback)(int, int, int);
typedef void(__stdcall* VoidIntBooleanCallback)(int, unsigned char);
typedef void(__stdcall* VoidIntIntConstCharCallback)(int, const char*);
typedef void(__stdcall* VoidIntIntConstCharIntCallback)(int, int, const char*, int);
typedef void(__stdcall* VoidConstCharIntCallback)(const char*, int);
typedef void(__stdcall* VoidConstCharBooleanCallback)(const char*, unsigned char);
typedef int(__stdcall* IntVoidCallback)();
typedef unsigned char(__stdcall* BooleanVoidCallback)();
typedef char*(__stdcall* CharVoidCallback)();
typedef unsigned char(__stdcall* BooleanIntDoubleCallback)(int, double);
typedef unsigned char(__stdcall* BooleanDoubleDoubleDoubleDoubleDoubleCallback)(double, double, double, double, double);
typedef unsigned char(__stdcall* BooleanIntDoubleVoidPtrCallback)(int, double, void*);
typedef unsigned char(__stdcall* BooleanVoidPtrDoubleCallback)(void* , double);
typedef unsigned char(__stdcall* BooleanVoidPtrCallback)(void*);
typedef char(__stdcall* CharConstCharPtrConstCharPtrCharCallback)(const char*, const char*, char);


/* Specific callback types */
typedef VoidVoidCallback InitWindowsCallback;
typedef IntVoidCallback PlayerSelectionCallback;
typedef CharVoidCallback AskNameCallback;
typedef VoidVoidCallback GetEventCallback;
typedef VoidConstCharCallback ExitWindowsCallback;
typedef VoidConstCharCallback SuspendWindowsCallback;
typedef VoidVoidCallback ResumeWindowsCallback;
typedef IntIntCallback CreateWindowCallback;
typedef VoidIntCallback ClearWindowCallback;
typedef VoidIntBooleanCallback DisplayWindowCallback;
typedef VoidIntCallback DestroyWindowCallback;
typedef VoidIntIntIntCallback CursCallback;
typedef VoidIntIntConstCharIntCallback PutStrExCallback;
typedef VoidIntIntConstCharCallback PutMixedCallback;
typedef VoidConstCharBooleanCallback DisplayFileCallback;
typedef VoidIntCallback StartMenuCallback;
typedef void(__stdcall* AddMenuCallback)(int);
typedef void(__stdcall* AddExtendedMenuCallback)(int);
typedef VoidIntIntConstCharCallback EndMenuCallback;
typedef int(__stdcall* SelectMenuCallback)(int, int, void*);
typedef void(__stdcall* MessageMenuCallback)(int);
typedef VoidVoidCallback UpdateInventoryCallback;
typedef VoidVoidCallback MarkSynchCallback;
typedef VoidVoidCallback WaitSynchCallback;
typedef VoidIntIntCallback ClipAroundCallback;
typedef VoidCharCallback UpdatePositionBarCallback;
typedef void(__stdcall* PrintGlyphCallback)(int);
typedef VoidConstCharCallback RawPrintCallback;
typedef VoidConstCharCallback RawPrintBoldCallback;
typedef IntVoidCallback GetChCallback;
typedef int(__stdcall* PosKeyCallback)(int*, int*, int*);
typedef VoidVoidCallback BellCallback;
typedef IntVoidCallback DoPrevMessageCallback;
typedef CharConstCharPtrConstCharPtrCharCallback YnFunctionCallback;
typedef void(__stdcall* GetLinCallback)(int);
typedef IntVoidCallback GetExtCmdCallback;
typedef VoidIntCallback NumberPadCallback;
typedef VoidVoidCallback DelayOutputCallback;
typedef VoidIntCallback DelayOutputMillisecondsCallback;
typedef VoidIntCallback DelayOutputIntervalsCallback;
typedef void(__stdcall* ChangeColorCallback)(int);
typedef VoidIntCallback ChangeBackgroundCallback;
typedef void(__stdcall* SetFontNameCallback)(int);
typedef const char* (__stdcall* GetColorStringCallback)(int);
typedef VoidVoidCallback StartScreenCallback;
typedef VoidVoidCallback EndScreenCallback;
typedef void(__stdcall* OutRipCallback)(int);
typedef VoidConstCharCallback PreferenceUpdateCallback;
typedef void(__stdcall* GetMsgHistoryCallback)(int);
typedef VoidConstCharBooleanCallback PutMsgHistoryCallback;
typedef VoidVoidCallback StatusInitCallback;
typedef VoidVoidCallback StatusFinishCallback;
typedef void(__stdcall* StatusEnableFieldCallback)(int);
typedef void(__stdcall* StatusUpdateCallback)(int, void*, int, int, int, unsigned long*);
typedef BooleanVoidCallback CanSuspendYesCallback;
typedef VoidVoidCallback StretchWindowCallback;
typedef void(__stdcall* SetAnimationTimerCallback)(unsigned long);
typedef void(__stdcall* OpenSpecialViewCallback)(int);
typedef BooleanVoidCallback StopAllSoundsCallback;
typedef BooleanIntDoubleCallback PlayImmediateSoundCallback;
typedef BooleanIntDoubleCallback PlayOccupationAmbientCallback;
typedef BooleanIntDoubleCallback PlayEffectAmbientCallback;
typedef unsigned char(__stdcall* SetEffectAmbientVolumeCallback)(double);
typedef BooleanIntDoubleCallback PlayMusicCallback;
typedef BooleanIntDoubleCallback PlayLevelAmbientCallback;
typedef BooleanIntDoubleCallback PlayEnvironmentAmbientCallback;
typedef BooleanDoubleDoubleDoubleDoubleDoubleCallback AdjustGeneralVolumesCallback;
typedef BooleanIntDoubleVoidPtrCallback AddAmbientSoundCallback;
typedef BooleanVoidPtrCallback DeleteAmbientSoundCallback;
typedef BooleanVoidPtrDoubleCallback SetAmbientVolumeCallback;
typedef void(__stdcall* ExitHackCallback)(int);

typedef char*(__stdcall* GetCwdCallback)();
typedef int (__stdcall* MessageBoxCallback)(char*, char*, unsigned int);

struct callback_procs {
    InitWindowsCallback callback_init_nhwindows;
    PlayerSelectionCallback callback_player_selection;
    AskNameCallback callback_askname;
    GetEventCallback callback_get_nh_event;
    ExitWindowsCallback callback_exit_nhwindows;
    SuspendWindowsCallback callback_suspend_nhwindows;
    ResumeWindowsCallback callback_resume_nhwindows;
    CreateWindowCallback callback_create_nhwindow;
    ClearWindowCallback callback_clear_nhwindow;
    DisplayWindowCallback callback_display_nhwindow;
    DestroyWindowCallback callback_destroy_nhwindow;
    CursCallback callback_curs;
    PutStrExCallback callback_putstr_ex;
    PutMixedCallback callback_putmixed;
    DisplayFileCallback callback_display_file;
    StartMenuCallback callback_start_menu;
    AddMenuCallback callback_add_menu;
    AddExtendedMenuCallback callback_add_extended_menu;
    EndMenuCallback callback_end_menu;
    SelectMenuCallback callback_select_menu;
    MessageMenuCallback callback_message_menu;
    UpdateInventoryCallback callback_update_inventory;
    MarkSynchCallback callback_mark_synch;
    WaitSynchCallback callback_wait_synch;

    ClipAroundCallback callback_cliparound;
    UpdatePositionBarCallback callback_update_positionbar;
    
    PrintGlyphCallback callback_print_glyph;
    RawPrintCallback callback_raw_print;
    RawPrintBoldCallback callback_raw_print_bold;
    GetChCallback callback_nhgetch;
    PosKeyCallback callback_nh_poskey;
    BellCallback callback_nhbell;
    DoPrevMessageCallback callback_doprev_message;
    YnFunctionCallback callback_yn_function;
    GetLinCallback callback_getlin;
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
    SetAnimationTimerCallback callback_set_animation_timer;
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
    ExitHackCallback callback_exit_hack;

    GetCwdCallback callback_getcwd;
    MessageBoxCallback callback_messagebox;
};

#endif /* DLLCALLBACK_H */
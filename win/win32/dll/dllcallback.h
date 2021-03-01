#pragma once

/*
 * * dllcallback.h
 * Copyright (c) Janne Gustafsson, 2020
 */

#ifndef DLLCALLBACK_H
#define DLLCALLBACK_H

 /* General callback types */
typedef void(__stdcall* VoidVoidCallback)();
typedef void(__stdcall* VoidCharCallback)(char*);
typedef void(__stdcall* VoidConstCharCallback)(const char*);
typedef int(__stdcall* IntIntCallback)(int);
typedef void(__stdcall* VoidIntCallback)(int);
typedef void(__stdcall* VoidIntIntCallback)(int, int);
typedef void(__stdcall* VoidIntIntIntCallback)(int, int, int);
//typedef void(__stdcall* VoidIntBooleanCallback)(int, boolean);
typedef VoidIntIntCallback VoidIntBooleanCallback;
typedef void(__stdcall* VoidIntIntConstCharCallback)(int, const char*);
typedef void(__stdcall* VoidConstCharIntCallback)(const char*, int);
typedef VoidConstCharIntCallback VoidConstCharBooleanCallback;
//typedef void(__stdcall* VoidConstCharBooleanCallback)(const char*, boolean);
typedef int(__stdcall* IntVoidCallback)();
typedef int(__stdcall* BooleanVoidCallback)();


/* Specific callback types */
typedef void(__stdcall* InitWindowsCallback)(int);
typedef VoidVoidCallback PlayerSelectionCallback;
typedef VoidVoidCallback AskNameCallback;
typedef VoidVoidCallback GetEventCallback;
typedef VoidConstCharCallback ExitWindowsCallback;
typedef VoidConstCharCallback SuspendWindowsCallback;
typedef VoidVoidCallback ResumeWindowsCallback;
typedef IntIntCallback CreateWindowCallback;
typedef VoidIntCallback ClearWindowCallback;
typedef VoidIntBooleanCallback DisplayWindowCallback;
typedef VoidIntCallback DestroyWindowCallback;
typedef VoidIntIntIntCallback CursCallback;
typedef VoidIntIntConstCharCallback PutStrCallback;
typedef VoidIntIntConstCharCallback PutMixedCallback;
typedef VoidConstCharBooleanCallback DisplayFileCallback;
typedef VoidIntCallback StartMenuCallback;
typedef void(__stdcall* AddMenuCallback)(int);
typedef void(__stdcall* AddExtendedMenuCallback)(int);
typedef VoidIntIntConstCharCallback EndMenuCallback;
typedef void(__stdcall* SelectMenuCallback)(int);
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
typedef VoidVoidCallback BellCallback;
typedef void(__stdcall* PosKeyCallback)(int);
typedef IntVoidCallback DoPrevMessageCallback;
typedef void(__stdcall* YnFunctionCallback)(int);
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
typedef void(__stdcall* StatusUpdateCallback)(int);
typedef BooleanVoidCallback CanSuspendYesCallback;
typedef VoidVoidCallback StretchWindowCallback;
typedef void(__stdcall* SetAnimationTimerCallback)(int);
typedef void(__stdcall* OpenSpecialViewCallback)(int);
typedef void(__stdcall* StopAllSoundsCallback)(int);
typedef void(__stdcall* PlayImmediateSoundCallback)(int);
typedef void(__stdcall* PlayOccupationAmbientCallback)(int);
typedef void(__stdcall* PlayEffectAmbientCallback)(int);
typedef void(__stdcall* SetEffectAmbientVolumeCallback)(int);
typedef void(__stdcall* PlayMusicCallback)(int);
typedef void(__stdcall* PlayLevelAmbientCallback)(int);
typedef void(__stdcall* PlayEnvironmentAmbientCallback)(int);
typedef void(__stdcall* AdjustGeneralVolumesCallback)(int);
typedef void(__stdcall* AddAmbientSoundCallback)(int);
typedef void(__stdcall* DeleteAmbientSoundCallback)(int);
typedef void(__stdcall* SetAmbientVolumeCallback)(int);
typedef void(__stdcall* ExitHackCallback)(int);

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
    PutStrCallback callback_putstr;
    PutMixedCallback callback_putmixed;
    DisplayFileCallback callback_display_file;
    StartMenuCallback callback_start_menu;
    AddMenuCallback callback_add_menu;
    AddExtendedMenuCallback callback_add_extended_menu;
    EndMenuCallback callback_end_menu;
    SelectMenuCallback callback_select_menu;
    MessageMenuCallback callback_message_menu; /* no need for X-specific handling */
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
    
    /* other defs that really should go away (they're tty specific) */
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
};

#endif /* DLLCALLBACK_H */
#include "gnollhackdroid.h"
#include "hack.h"
#include "libproc.h"
#include "callback.h"

extern int GnollHackMain(int argc, char** argv);

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "gnollhackdroid", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "gnollhackdroid", __VA_ARGS__))

/* This trivial function returns the platform ABI for which this dynamic native library is compiled.*/
const char *getPlatformABI()
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
	#define ABI "armeabi-v7a/NEON"
#else
	#define ABI "armeabi-v7a"
#endif
#else
	#define ABI "armeabi"
#endif
#elif defined(__i386__)
	#define ABI "x86"
#else
	#define ABI "unknown"
#endif
	LOGI("This dynamic shared library is compiled with ABI: %s", ABI);
	return "This native library is compiled with ABI: %s" ABI ".";
}

extern void FDECL(set_wincaps, (unsigned long, unsigned long));

void gnollhackdroid()
{

}

int DoSomeCalcDroid()
{
	return 5;
}

int RunGnollHackTest(char* gnhdir)
{
	char* params[2] = { 0 };

	params[0] = "gnollhack";
	params[1] = 0;

	windowprocs.win_raw_print = libdef_raw_print;
	windowprocs.win_raw_print_bold = libdef_raw_print_bold;
	windowprocs.win_wait_synch = libdef_wait_synch;


#if 0
	{
		char cwbuf[BUFSIZ] = "";
		char* curwd = getcwd(cwbuf, sizeof(cwbuf));
		char* curwd2 = argv[2];
		int res = chdir(curwd2);
		int e = errno;

		char dirbuf[BUFSZ];
		Sprintf(dirbuf, "%s%s", curwd2, "");
		int cnt = 0;
		DIR* dir;
		struct dirent* ent;
		if ((dir = opendir(dirbuf)) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL) {
				Sprintf(dirbuf, "%s\n", ent->d_name);
				cnt++;
			}
			closedir(dir);
		}
		else {
			/* could not open directory */
			e = errno;
			return EXIT_FAILURE;
		}
		char cwbuf[BUFSIZ] = "";
		char* curwd = getcwd(cwbuf, sizeof(cwbuf));
		char* curwd2 = argv[2];
		int res = chdir(curwd2);
		int e = errno;

		char dirbuf[BUFSZ];
		Sprintf(dirbuf, "%s%s", curwd2, "");
		int cnt = 0;
		DIR* dir;
		struct dirent* ent;
		if ((dir = opendir(dirbuf)) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL) {
				Sprintf(dirbuf, "%s\n", ent->d_name);
				cnt++;
			}
			closedir(dir);
		}
		else {
			/* could not open directory */
			e = errno;
			return EXIT_FAILURE;
		}
	}
#endif


	int res = chdir(gnhdir);
	if (res != 0)
	{
		/* Failed to change to right directory */
		return 0;
	}

	return GnollHackMain(1, params);
}

int GnollHackStart(cmdlineargs)
char* cmdlineargs;
{

	char* params[2] = { 0 };

	params[0] = "gnollhack";
	params[1] = 0;

	/* Parse more args here */

	return GnollHackMain(1, params);
}

extern struct callback_procs lib_callbacks;

int RunGnollHack(
    char* gnhdir,
    char* cmdlineargs,
    unsigned long wincap1,
    unsigned long wincap2,
    InitWindowsCallback callback_init_nhwindows,
    PlayerSelectionCallback callback_player_selection,
    AskNameCallback callback_askname,
    GetEventCallback callback_get_nh_event,
    ExitWindowsCallback callback_exit_nhwindows,
    SuspendWindowsCallback callback_suspend_nhwindows,
    ResumeWindowsCallback callback_resume_nhwindows,
    CreateWindowCallback callback_create_nhwindow,
    ClearWindowCallback callback_clear_nhwindow,
    DisplayWindowCallback callback_display_nhwindow,
    DestroyWindowCallback callback_destroy_nhwindow,
    CursCallback callback_curs,
    PutStrExCallback callback_putstr_ex,
    PutMixedCallback callback_putmixed,
    DisplayFileCallback callback_display_file,
    StartMenuCallback callback_start_menu,
    AddMenuCallback callback_add_menu,
    AddExtendedMenuCallback callback_add_extended_menu,
    EndMenuCallback callback_end_menu,
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
    InitPrintGlyphCallback callback_init_print_glyph,
    RawPrintCallback callback_raw_print,
    RawPrintBoldCallback callback_raw_print_bold,
    GetChCallback callback_nhgetch,
    PosKeyCallback callback_nh_poskey,
    BellCallback callback_nhbell,
    DoPrevMessageCallback callback_doprev_message,
    YnFunctionCallback callback_yn_function,
    GetLinCallback callback_getlin,
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
    SetAnimationTimerCallback callback_set_animation_timer,
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
    ExitHackCallback callback_exit_hack,

    GetCwdCallback callback_getcwd,
    MessageBoxCallback callback_messagebox,
    OutRipBeginCallback callback_outrip_begin,
    OutRipEndCallback callback_outrip_end,

    FreeMemoryCallback callback_free_memory
)
{
    /* Set wincaps */
    set_wincaps(wincap1, wincap2);

    /* Set callback function pointers here */
    lib_callbacks.callback_init_nhwindows = callback_init_nhwindows;
    lib_callbacks.callback_player_selection = callback_player_selection;
    lib_callbacks.callback_askname = callback_askname;
    lib_callbacks.callback_get_nh_event = callback_get_nh_event;
    lib_callbacks.callback_exit_nhwindows = callback_exit_nhwindows;
    lib_callbacks.callback_suspend_nhwindows = callback_suspend_nhwindows;
    lib_callbacks.callback_resume_nhwindows = callback_resume_nhwindows;
    lib_callbacks.callback_create_nhwindow = callback_create_nhwindow;
    lib_callbacks.callback_clear_nhwindow = callback_clear_nhwindow;
    lib_callbacks.callback_display_nhwindow = callback_display_nhwindow;
    lib_callbacks.callback_destroy_nhwindow = callback_destroy_nhwindow;
    lib_callbacks.callback_display_file = callback_display_file;
    lib_callbacks.callback_curs = callback_curs;
    lib_callbacks.callback_putstr_ex = callback_putstr_ex;
    lib_callbacks.callback_putmixed = callback_putmixed;
    lib_callbacks.callback_display_file = callback_display_file;
    lib_callbacks.callback_start_menu = callback_start_menu;
    lib_callbacks.callback_add_menu = callback_add_menu;
    lib_callbacks.callback_add_extended_menu = callback_add_extended_menu;
    lib_callbacks.callback_end_menu = callback_end_menu;
    lib_callbacks.callback_select_menu = callback_select_menu;
    lib_callbacks.callback_message_menu = callback_message_menu;
    lib_callbacks.callback_update_inventory = callback_update_inventory;
    lib_callbacks.callback_mark_synch = callback_mark_synch;
    lib_callbacks.callback_wait_synch = callback_wait_synch;
    lib_callbacks.callback_cliparound = callback_cliparound;
    lib_callbacks.callback_update_positionbar = callback_update_positionbar;
    lib_callbacks.callback_print_glyph = callback_print_glyph;
    lib_callbacks.callback_init_print_glyph = callback_init_print_glyph;
    lib_callbacks.callback_raw_print = callback_raw_print;
    lib_callbacks.callback_raw_print_bold = callback_raw_print_bold;
    lib_callbacks.callback_nhgetch = callback_nhgetch;
    lib_callbacks.callback_nh_poskey = callback_nh_poskey;
    lib_callbacks.callback_nhbell = callback_nhbell;
    lib_callbacks.callback_nhbell = callback_nhbell;
    lib_callbacks.callback_doprev_message = callback_doprev_message;
    lib_callbacks.callback_yn_function = callback_yn_function;
    lib_callbacks.callback_getlin = callback_getlin;
    lib_callbacks.callback_yn_function = callback_yn_function;
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
    lib_callbacks.callback_set_animation_timer = callback_set_animation_timer;
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
    lib_callbacks.callback_exit_hack = callback_exit_hack;
    lib_callbacks.callback_getcwd = callback_getcwd;
    lib_callbacks.callback_messagebox = callback_messagebox;
    lib_callbacks.callback_outrip_begin = callback_outrip_begin;
    lib_callbacks.callback_outrip_end = callback_outrip_end;

    lib_callbacks.callback_free_memory = callback_free_memory;


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
    return GnollHackStart(cmdlineargs);
}

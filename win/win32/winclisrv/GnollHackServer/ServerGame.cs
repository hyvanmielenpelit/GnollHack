using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;
using GnollHackCommon;

namespace GnollHackServer
{
    public class ServerGame
    {
        [DllImport(@"gnollhackwin.dll")]
        public static extern int RunGnollHack(
            [MarshalAs(UnmanagedType.LPStr)] string gnhdir,
            [MarshalAs(UnmanagedType.LPStr)] string cmdlineargs,
            [MarshalAs(UnmanagedType.LPStr)] string preset_player_name,
            [MarshalAs(UnmanagedType.LPStr)] string recovery_name,
            ulong runflags,
            ulong wincaps1,
            ulong wincaps2,
            VoidVoidCallback callback_init_nhwindows,
            IntVoidCallback callback_player_selection,
            AskNameCallback callback_askname,
            VoidVoidCallback callback_get_nh_event,
            VoidConstCharCallback callback_exit_nhwindows,
            VoidConstCharCallback callback_suspend_nhwindows,
            VoidVoidCallback callback_resume_nhwindows,
            CreateGHWindowCallback callback_create_nhwindow_ex,
            VoidIntCallback callback_clear_nhwindow,
            VoidIntBooleanCallback callback_display_nhwindow,
            VoidIntCallback callback_destroy_nhwindow,
            VoidIntIntIntCallback callback_curs,
            PutStrExColorCallback callback_putstr_ex,
            PutStrEx2ColorCallback callback_putstr_ex2,
            PutStrExColorCallback callback_putmixed_ex,
            VoidConstCharBooleanCallback callback_display_file,
            StartMenuCallback callback_start_menu_ex,
            AddMenuCallback callback_add_menu,
            AddExtendedMenuCallback callback_add_extended_menu,
            EndMenuCallback callback_end_menu_ex,
            IntIntIntOutIntCallback callback_select_menu,
            VoidIntCallback callback_message_menu, /* no need for X-specific handling */
            VoidVoidCallback callback_update_inventory,
            VoidVoidCallback callback_mark_synch,
            VoidVoidCallback callback_wait_synch,
            /* If clipping is on */
            VoidIntIntBooleanCallback callback_cliparound,
            /* If positionbar is on */
            VoidCharCallback callback_update_positionbar,
            PrintGlyphCallback callback_print_glyph,
            IssueGuiCommandCallback callback_issue_gui_command,
            VoidConstCharCallback callback_raw_print,
            VoidConstCharCallback callback_raw_print_bold,
            IntVoidCallback callback_nhgetch,
            NhPosKeyCallback callback_nh_poskey,
            VoidVoidCallback callback_nhbell,
            IntVoidCallback callback_doprev_message,
            YnFunctionCallback callback_yn_function_ex,
            GetLineCallback callback_getlin_ex,
            IntVoidCallback callback_get_ext_cmd,
            VoidIntCallback callback_number_pad,
            VoidVoidCallback callback_delay_output,
            VoidIntCallback callback_delay_output_milliseconds,
            VoidIntCallback callback_delay_output_intervals,

            VoidVoidCallback callback_change_color,
            VoidIntCallback callback_change_background,
            VoidIntCallback callback_set_font_name,
            CharVoidCallback callback_get_color_string,

            VoidVoidCallback callback_start_screen,
            VoidVoidCallback callback_end_screen,
            OutRipCallback callback_outrip,
            VoidConstCharCallback callback_preference_update,
            GetMsgHistoryCallback callback_getmsghistory,
            PutMsgHistoryCallback callback_putmsghistory,
            VoidIntCallback callback_status_init,
            VoidVoidCallback callback_status_finish,
            VoidIntConstCharPtrConstCharPtrBooleanCallback callback_status_enablefield,
            StatusUpdateCallback callback_status_update,
            BooleanVoidCallback callback_can_suspend_yes,
            VoidVoidCallback callback_stretch_window,
            VoidUlongCallback callback_set_animation_timer_interval,
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
            SetAmbientSoundVolumeCallback callback_set_ambient_ghsound_volume,
            VoidVoidCallback callback_clear_context_menu,
            AddContextMenuCallback callback_add_context_menu,
            UpdateStatusButtonCallback callback_update_status_button,
            ToggleAnimationTimerCallback callback_toggle_animation_timer,
            DisplayFloatingTextCallback callback_display_floating_text,
            DisplayScreenTextCallback callback_display_screen_text,
            DisplayPopupTextCallback callback_display_popup_text,
            DisplayGUIEffectCallback callback_display_gui_effect,
            UpdateCursorCallback callback_update_cursor,
            UIHasInputCallback callback_ui_has_input,
            VoidIntCallback callback_exit_hack,
            CharVoidCallback callback_getcwd,
            IntCharCharUintCallback callback_messagebox,

            FreeMemoryCallback callback_free_memory,
            ReportPlayerNameCallback callback_report_player_name,
            ReportPlayTimeCallback callback_report_play_time,
            SendObjectDataCallback callback_send_object_data,
            SendMonsterDataCallback callback_send_monster_data
        );

        [DllImport(@"gnollhackwin.dll")]
        public static extern int RunGnollHackSimple(
            ulong wincaps1,
            ulong wincaps2
        );

        [DllImport(@"gnollhackwin.dll")]
        public static extern int RunGnollHackSimple2(
            ulong wincaps1,
            ulong wincaps2,
            VoidVoidCallback callback_init_nhwindows);

        [DllImport(@"gnollhackwin.dll")]
        public static extern byte dll_validrole(int role);

        [DllImport(@"gnollhackwin.dll")]
        public static extern byte dll_str2role([MarshalAs(UnmanagedType.LPStr)] string role_str);

        [DllImport(@"gnollhackwin.dll")]
        public static extern int DoSomeCalc2();

        private Thread _gnhthread;
        private ServerGameCenter _serverGameCenter;
        public bool WizardMode { get; set; }

        public ServerGame(bool wizardMode)
        {
            WizardMode = wizardMode;
            Thread t = new Thread(new ThreadStart(GNHThreadProc));
            _gnhthread = t;
            _serverGameCenter = ServerGameCenter.Instance;
        }
        public void StartGame()
        {
            _gnhthread.Start();
        }
        public bool IsGameAlive()
        {
            return _gnhthread.IsAlive;
        }





        protected void GNHThreadProc()
        {
            int res = DoSomeCalc2();
            //RunGnollHackSimple2(0, 0, GameCallback_InitWindows);

            //Thread.Sleep(5000);

            string curdir = Directory.GetCurrentDirectory() + "\\..\\..\\..\\..\\bin\\Debug\\Server\\netcoreapp3.1";
            Directory.SetCurrentDirectory(curdir);
//            System.Environment.SetEnvironmentVariable("GNOLLHACKDIR", curdir, EnvironmentVariableTarget.Process);

            RunGnollHack(
                curdir,
                "",
                "",
                "",
                (ulong)(WizardMode ? RunGnollHackFlags.WizardMode : 0),
                0,
                0,
                GameCallback_InitWindows,
                GameCallback_PlayerSelection,
                GameCallback_AskName,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidConstCharDummy,
                GameCallback_VoidConstCharDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_CreateGHWindow,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntBooleanDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntIntIntDummy,
                GameCallback_PutStrExColor,
                GameCallback_PutStrEx2Color,
                GameCallback_PutStrExColor,
                GameCallback_VoidConstCharBooleanDummy,
                GameCallback_StartMenu,
                GameCallback_AddMenu,
                GameCallback_AddExtendedMenu,
                GameCallback_EndMenu,
                GameCallback_IntIntIntOutIntDummy,
                GameCallback_VoidIntDummy, /* no need for X-specific handling */
                GameCallback_VoidVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidVoidDummy,
                /* If clipping is on */
                GameCallback_VoidIntIntBooleanDummy,
                /* If positionbar is on */
                GameCallback_VoidCharDummy,
                GameCallback_PrintGlyph,
                GameCallback_IssueGuiCommand,
                GameCallback_VoidConstCharDummy,
                GameCallback_VoidConstCharDummy,
                GameCallback_IntVoidDummy,
                GmaeCallback_nh_poskey,
                GameCallback_VoidVoidDummy,
                GameCallback_IntVoidDummy,
                GameCallback_YnFunction,
                GameCallback_GetLineCallback,
                GameCallback_IntVoidDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntDummy,

                GameCallback_VoidVoidDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntDummy,
                GameCallback_CharVoidDummy,

                GameCallback_VoidVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_OutRip,
                GameCallback_VoidConstCharDummy,
                GameCallback_GetMsgHistory,
                GameCallback_PutMsgHistory,
                GameCallback_VoidIntDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidIntConstCharPtrConstCharPtrBooleanDummy,
                GameCallback_StatusUpdate,
                GameCallback_BooleanVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidUlongDummy,
                GameCallback_OpenSpecialView,
                GameCallback_StopAllSounds,
                GameCallback_PlayImmediateSound,
                GameCallback_PlayOccupationAmbient,
                GameCallback_PlayEffectAmbient,
                GameCallback_SetEffectAmbientVolume,
                GameCallback_PlayMusic,
                GameCallback_PlayLevelAmbient,
                GameCallback_PlayEnvironmentAmbient,
                GameCallback_AdjustGeneralVolumes,
                GameCallback_AddAmbientSound,
                GameCallback_DeleteAmbientSound,
                GameCallback_SetAmbientSoundVolume,
                GameCallback_ClearContextMenu,
                GameCallback_AddContextMenu,
                GameCallback_UpdateStatusButton,
                GameCallback_ToggleAnimationTimer,
                GameCallback_DisplayFloatingText,
                GameCallback_DisplayScreenText,
                GameCallback_DisplayPopupText,
                GameCallback_DisplayGUIEffect,
                GameCallback_UpdateCursor,
                GameCallback_UIHasInput,
                GameCallback_ExitHack,
                GameCallback_GetCwd,
                GameCallback_MessageBox,
                //GameCallback_VoidIntDummy,
                //GameCallback_VoidIntDummy,
                GameCallback_FreeMemory,
                GameCallback_ReportPlayerName,
                GameCallback_ReportPlayTime,
                GameCallback_SendObjectData,
                GameCallback_SendMonsterData
            );
        }

        protected void GameCallback_InitWindows()
        {
            Debug.WriteLine("GameCallback_InitWindows");
        }
        protected int GameCallback_PlayerSelection()
        {
            Debug.WriteLine("GameCallback_PlayerSelection");
            _serverGameCenter.ServerCenter_PlayerSelection(this);
            System.Threading.Thread.Sleep(3000);
            return 0;
        }


        protected int GameCallback_AskName(string modeName, string modeDescription, IntPtr out_string_ptr)
        {
            Debug.WriteLine("GameCallback_AskName");
            return 0;
        }

        protected void GameCallback_ExitHack(int status)
        {
            Debug.WriteLine("GameCallback_ExitHack");
            _serverGameCenter.ServerCenter_ExitHack(this, status);
        }

        public void GameCallback_ClearContextMenu()
        {

        }

        public void GameCallback_AddContextMenu(int cmd_def_char, int cmd_cur_char, int style, int glyph, string cmd_text, string target_text, int attr, int color)
        {

        }

        public void GameCallback_UpdateStatusButton(int cmd, int btn, int val, ulong bflags)
        {

        }

        public void GameCallback_ToggleAnimationTimer(int timertype, int timerid, int state, int x, int y, int layer, ulong tflags)
        {

        }

        public void GameCallback_DisplayFloatingText(int x, int y, string text, int style, int attr, int color, ulong tflags)
        {

        }

        public void GameCallback_DisplayScreenText(string text, string supertext, string subtext, int style, int attr, int color, ulong tflags)
        {

        }

        public void GameCallback_DisplayPopupText(string text, string title, int style, int attr, int color, int glyph, ulong tflags)
        {

        }

        public void GameCallback_DisplayGUIEffect(int x, int y, int style, ulong tflags)
        {

        }

        public void GameCallback_UpdateCursor(int style, int force_paint, int show_on_u)
        {

        }
        protected void GameCallback_StartMenu(int winid, int style)
        {

        }

        protected void GameCallback_AddMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, int color, string text, byte presel)
        {

        }

        protected void GameCallback_AddExtendedMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, int color, string text, byte presel, int maxcount,
            UInt64 oid, UInt64 mid, char heading_group_accel, char special_mark, ulong menu_flags, byte dataflags, int style, IntPtr otmpdata_ptr, IntPtr otypdata_ptr)
        {

        }

        protected void GameCallback_EndMenu(int value1, string value2, string value3)
        {

        }

        public int GameCallback_GetMsgHistory(IntPtr text_ptr, IntPtr attributes_ptr, IntPtr colors_ptr, byte init)
        {
            return 0;
        }

        public void GameCallback_PutMsgHistory(string msg, IntPtr attributes_ptr, IntPtr colors_ptr, byte is_restoring)
        {

        }

        protected void GameCallback_FreeMemory(ref IntPtr ptr)
        {

        }

        protected void GameCallback_ReportPlayerName(string used_player_name)
        {

        }

        protected void GameCallback_ReportPlayTime(long timePassed, long currentPlayTime)
        {

        }

        public void GameCallback_SendObjectData(int x, int y, IntPtr otmp_ptr, int cmdtype, int where, IntPtr otypdata_ptr, ulong oflags)
        {

        }
        public void GameCallback_SendMonsterData(int cmdtype, int x, int y, IntPtr monster_data_ptr, ulong oflags)
        {

        }

        protected void GameCallback_OutRip(int winid, string plname, int points, string killer, string time)
        {

        }

        public int GameCallback_PlayImmediateSound(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, ulong play_flags)
        {
            return 0;
        }
        public int GameCallback_PlayMusic(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            return 0;
        }
        public int GameCallback_AddAmbientSound(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume, out UInt64 soundSourceId)
        {
            soundSourceId = 0;
            return 0;
        }
        public int GameCallback_DeleteAmbientSound(UInt64 soundSourceId)
        {
            return 0;
        }
        public int GameCallback_SetAmbientSoundVolume(UInt64 soundSourceId, double soundVolume)
        {
            return 0;
        }
        public int GameCallback_PlayLevelAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            return 0;
        }
        public int GameCallback_PlayEnvironmentAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            return 0;
        }
        public int GameCallback_PlayOccupationAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            return 0;
        }
        public int GameCallback_PlayEffectAmbient(int ghsound, string eventPath, int bankid, double eventVolume, double soundVolume)
        {
            return 0;
        }

        public int GameCallback_SetEffectAmbientVolume(double soundVolume)
        {
            return 0;
        }

        public int GameCallback_StopAllSounds(uint flags, uint dialogue_mid)
        {
            return 0;
        }

        protected int GameCallback_CreateGHWindow(int widid, int style, int glyph, byte dataflags, IntPtr objdata, IntPtr otypdata)
        {
            return 0;
        }

        public int GameCallback_UIHasInput()
        {
            return 0;
        }

        protected int GameCallback_OpenSpecialView(int viewtype, string text, string title, int attr, int color)
        {
            return 1;
        }

        protected void GameCallback_IssueGuiCommand(int value1, int value2, string cmd_str)
        {

        }


        protected void GameCallback_VoidVoidDummy()
        {

        }
        protected void GameCallback_VoidCharDummy(string value)
        {

        }
        protected void GameCallback_VoidConstCharDummy(string value)
        {

        }
        protected int GameCallback_IntIntDummy(int value1)
        {
            return 0;
        }
        protected void GameCallback_VoidIntDummy(int value1)
        {

        }
        protected void GameCallback_VoidIntIntBooleanDummy(int value1, int value2, byte value3)
        {

        }
        protected void GameCallback_VoidIntIntIntDummy(int value1, int value2, int value3)
        {

        }
        protected void GameCallback_VoidIntBooleanDummy(int value1, byte value2)
        {

        }
        protected void GameCallback_VoidIntCharDummy(int value1, string value2)
        {

        }
        protected void GameCallback_VoidIntIntConstCharDummy(int value1, int value2, string value3)
        {

        }
        protected void GameCallback_PutStrExColor(int value1, string value2, int value3, int value4, int value5)
        {

        }
        protected void GameCallback_PutStrEx2Color(int value1, string value2, IntPtr value3, IntPtr value4, int value5, int value6, int value7)
        {

        }
        protected void GameCallback_VoidConstCharIntDummy(string value1, int value2)
        {

        }
        protected void GameCallback_VoidConstCharBooleanDummy(string value1, byte value2)
        {

        }
        protected int GameCallback_IntVoidDummy()
        {
            return 0;
        }
        protected byte GameCallback_BooleanVoidDummy()
        {
            return 0;
        }

        protected string GameCallback_GetCwd()
        {
            return Directory.GetCurrentDirectory();
        }
        protected int GameCallback_MessageBox(string text, string title, uint type)
        {
            return 1;
        }
        protected int GameCallback_IntIntIntOutIntDummy(int value1, int value2, /* out int[] */ out IntPtr value3, out int value4)
        {
            value3 = (IntPtr)0; // new int[2];
            value4 = 0;
            return 0;
        }
        protected int GmaeCallback_nh_poskey(out int x, out int y, out int mod)
        { 
            x = 0;
            y = 0;
            mod = 0;
            return 0;
        }
        protected void GameCallback_StatusUpdate(int idx, string str, long condbits, int chg, int percent, int color, IntPtr colormasks)
        {
            return;
        }

        protected byte GameCallback_AdjustGeneralVolumes(double value1, double value2, double value3, double value4, double value5, double value6)
        {
            /* Unused */
            return 0;
        }


        /* Dummies */
        protected void GameCallback_VoidUlongDummy(ulong value1)
        {

        }
        protected byte GameCallback_BooleanIntDoubleDummy(int value1, double value2)
        {
            return 0;
        }
        protected byte GameCallback_BooleanDoubleDummy(double value1)
        {
            return 0;
        }
        protected byte GameCallback_BooleanIntDoubleVoidPtrDummy(int value1, double value2, IntPtr value3)
        {
            return 0;
        }
        protected byte GameCallback_BooleanDoubleVoidPtrDummy(double value1, IntPtr value2)
        {
            return 0;
        }
        protected byte GameCallback_BooleanVoidPtrDummy(IntPtr value1)
        {
            return 0;
        }
        protected int GameCallback_YnFunction(int style, int attr, int color, int glyph, string title, string query, string responses, string def, string descriptions, string introline, ulong ynflags)
        {
            return 0;
        }
        protected int GameCallback_GetLineCallback(int style, int attr, int color, string query, string placeholder, string linesuffix, string introline, IntPtr out_string_ptr)
        {
            return 0;
        }
        protected string GameCallback_CharPtrBooleanDummy(byte value1)
        {
            return "message here";
        }
        protected string GameCallback_CharVoidDummy()
        {
            return "";
        }
        protected void GameCallback_PrintGlyph(int value1, int value2, int value3, int value4, int value5, int value6, int value7, uint value8, IntPtr layers_ptr)
        {
            return;
        }
        protected void GameCallback_VoidIntConstCharPtrConstCharPtrBooleanDummy(int value1, string value2, string value3, byte value4)
        {
            return;
        }
    }
}

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
        [DllImport(@"gnollhacklib.dll")]
        public static extern int RunGnollHack(
            [MarshalAs(UnmanagedType.LPStr)] string gnhdir,
            [MarshalAs(UnmanagedType.LPStr)] string cmdlineargs,
            ulong runflags,
            ulong wincaps1,
            ulong wincaps2,
            VoidVoidCallback callback_init_nhwindows,
            IntVoidCallback callback_player_selection,
            CharVoidCallback callback_askname,
            VoidVoidCallback callback_get_nh_event,
            VoidConstCharCallback callback_exit_nhwindows,
            VoidConstCharCallback callback_suspend_nhwindows,
            VoidVoidCallback callback_resume_nhwindows,
            IntIntCallback callback_create_nhwindow,
            VoidIntCallback callback_clear_nhwindow,
            VoidIntBooleanCallback callback_display_nhwindow,
            VoidIntCallback callback_destroy_nhwindow,
            VoidIntIntIntCallback callback_curs,
            PutStrExColorCallback callback_putstr_ex,
            PutStrExColorCallback callback_putmixed_ex,
            VoidConstCharBooleanCallback callback_display_file,
            VoidIntCallback callback_start_menu,
            AddMenuCallback callback_add_menu,
            AddExtendedMenuCallback callback_add_extended_menu,
            VoidIntConstCharCallback callback_end_menu,
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
            VoidIntCallback callback_init_print_glyph,
            VoidConstCharCallback callback_raw_print,
            VoidConstCharCallback callback_raw_print_bold,
            IntVoidCallback callback_nhgetch,
            NhPosKeyCallback callback_nh_poskey,
            VoidVoidCallback callback_nhbell,
            IntVoidCallback callback_doprev_message,
            IntIntIntConstCharPtrConstCharPtrCharPtrCallback callback_yn_function_ex,
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
            VoidIntCallback callback_outrip,
            VoidConstCharCallback callback_preference_update,
            CharPtrBooleanCallback callback_getmsghistory,
            VoidConstCharBooleanCallback callback_putmsghistory,
            VoidVoidCallback callback_status_init,
            VoidVoidCallback callback_status_finish,
            VoidIntConstCharPtrConstCharPtrBooleanCallback callback_status_enablefield,
            StatusUpdateCallback callback_status_update,
            BooleanVoidCallback callback_can_suspend_yes,
            VoidVoidCallback callback_stretch_window,
            VoidUlongCallback callback_set_animation_timer_interval,
            VoidIntCallback callback_open_special_view,
            StopAllSoundsCallback callback_stop_all_sounds,
            PlayImmediateSoundCallback callback_play_immediate_ghsound,
            BooleanIntDoubleCallback callback_play_ghsound_occupation_ambient,
            BooleanIntDoubleCallback callback_play_ghsound_effect_ambient,
            BooleanDoubleCallback callback_set_effect_ambient_volume,
            PlayMusicCallback callback_play_ghsound_music,
            BooleanDoubleCallback callback_play_ghsound_level_ambient,
            BooleanDoubleCallback callback_play_ghsound_environment_ambient,
            BooleanDoubleDoubleDoubleDoubleDoubleCallback callback_adjust_ghsound_general_volumes,
            BooleanIntDoubleVoidPtrCallback callback_add_ambient_ghsound,
            BooleanVoidPtrCallback callback_delete_ambient_ghsound,
            BooleanDoubleVoidPtrCallback callback_set_ambient_ghsound_volume,
            VoidVoidCallback callback_clear_context_menu,
            AddContextMenuCallback callback_add_context_menu,
            ToggleAnimationTimerCallback callback_toggle_animation_timer,
            SetAnimationWaitCallback callback_set_animation_wait,
            GetAnimationWaitCallback callback_get_animation_wait,
            VoidIntCallback callback_exit_hack,
            CharVoidCallback callback_getcwd,
            IntCharCharUintCallback callback_messagebox,
            VoidIntCallback callback_outrip_begin,
            VoidIntCallback callback_outrip_end,

            FreeMemoryCallback callback_free_memory
        );

        [DllImport(@"gnollhacklib.dll")]
        public static extern int RunGnollHackSimple(
            ulong wincaps1,
            ulong wincaps2
        );

        [DllImport(@"gnollhacklib.dll")]
        public static extern int RunGnollHackSimple2(
            ulong wincaps1,
            ulong wincaps2,
            VoidVoidCallback callback_init_nhwindows);

        [DllImport(@"gnollhacklib.dll")]
        public static extern byte dll_validrole(int role);

        [DllImport(@"gnollhacklib.dll")]
        public static extern byte dll_str2role([MarshalAs(UnmanagedType.LPStr)] string role_str);

        [DllImport(@"gnollhacklib.dll")]
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
                GameCallback_IntIntDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntBooleanDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntIntIntDummy,
                GameCallback_PutStrExColor,
                GameCallback_PutStrExColor,
                GameCallback_VoidConstCharBooleanDummy,
                GameCallback_VoidIntDummy,
                GameCallback_AddMenu,
                GameCallback_AddExtendedMenu,
                GameCallback_VoidIntConstCharDummy,
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
                GameCallback_VoidIntDummy,
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
                GameCallback_VoidIntDummy,
                GameCallback_VoidConstCharDummy,
                GameCallback_CharPtrBooleanDummy,
                GameCallback_VoidConstCharBooleanDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidIntConstCharPtrConstCharPtrBooleanDummy,
                GameCallback_StatusUpdate,
                GameCallback_BooleanVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidUlongDummy,
                GameCallback_VoidIntDummy,
                GameCallback_StopAllSounds,
                GameCallback_PlayImmediateSound,
                GameCallback_BooleanIntDoubleDummy,
                GameCallback_BooleanIntDoubleDummy,
                GameCallback_BooleanDoubleDummy,
                GameCallback_PlayMusic,
                GameCallback_BooleanDoubleDummy,
                GameCallback_BooleanDoubleDummy,
                GameCallback_BooleanDoubleDoubleDoubleDoubleDoubleDummy,
                GameCallback_BooleanIntDoubleVoidPtrDummy,
                GameCallback_BooleanVoidPtrDummy,
                GameCallback_BooleanDoubleVoidPtrDummy,
                GameCallback_ClearContextMenu,
                GameCallback_AddContextMenu,
                GameCallback_ToggleAnimationTimer,
                GameCallback_SetAnimationWait,
                GameCallback_GetAnimationWait,
                GameCallback_ExitHack,
                GameCallback_GetCwd,
                GameCallback_MessageBox,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntDummy,
                GameCallback_FreeMemory
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


        protected string GameCallback_AskName()
        {
            Debug.WriteLine("GameCallback_AskName");
            return "Janne Test";
        }

        protected void GameCallback_ExitHack(int status)
        {
            Debug.WriteLine("GameCallback_ExitHack");
            _serverGameCenter.ServerCenter_ExitHack(this, status);
        }

        public void GameCallback_ClearContextMenu()
        {

        }

        public void GameCallback_AddContextMenu(int cmd_def_char, int cmd_cur_char, int dir, int glyph, string cmd_text, string target_text, int attr, int color)
        {

        }

        public void GameCallback_ToggleAnimationTimer(int timertype, int timerid, int state, int x, int y, int layer, ulong tflags)
        {

        }
        public void GameCallback_SetAnimationWait(int timertype, int timerid, int waittype, ulong val)
        {

        }
        public ulong GameCallback_GetAnimationWait(int timertype, int timerid, int waittype)
        {
            return 0UL;
        }

        protected void GameCallback_AddMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, string text, byte presel, int color)
        {

        }

        protected void GameCallback_AddExtendedMenu(int winid, int glyph, Int64 identifier, char accel, char groupaccel, int attributes, string text, byte presel, int color, int maxcount,
            UInt64 oid, UInt64 mid, char heading_group_accel, ulong menu_flags)
        {

        }

        protected void GameCallback_FreeMemory(ref IntPtr ptr)
        {

        }

        public int GameCallback_PlayImmediateSound(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume, string[] parameterNames, float[] parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid)
        {
            return 0;
        }
        public int GameCallback_PlayMusic(int ghsound, string eventPath, int bankid, float eventVolume, float soundVolume)
        {
            return 0;
        }

        public int GameCallback_StopAllSounds(uint flags, uint dialogue_mid)
        {
            return 0;
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
        protected void GameCallback_VoidIntConstCharDummy(int value1, string value2)
        {

        }
        protected void GameCallback_VoidIntIntConstCharDummy(int value1, int value2, string value3)
        {

        }
        protected void GameCallback_PutStrExColor(int value1, int value2, string value3, int value4, int value5)
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
        protected byte GameCallback_BooleanDoubleDoubleDoubleDoubleDoubleDummy(double value1, double value2, double value3, double value4, double value5)
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
        protected int GameCallback_YnFunction(int attr, int color, string value1, string value2, string value3)
        {
            return 0;
        }
        protected string GameCallback_GetLineCallback(int attr, int color, string value1)
        {
            return "";
        }
        protected string GameCallback_CharPtrBooleanDummy(byte value1)
        {
            return "message here";
        }
        protected string GameCallback_CharVoidDummy()
        {
            return "";
        }
        protected void GameCallback_PrintGlyph(int value1, int value2, int value3, int value4, int value5, int value6, int value7, uint value8, LayerInfo layers)
        {
            return;
        }
        protected void GameCallback_VoidIntConstCharPtrConstCharPtrBooleanDummy(int value1, string value2, string value3, byte value4)
        {
            return;
        }
    }
}

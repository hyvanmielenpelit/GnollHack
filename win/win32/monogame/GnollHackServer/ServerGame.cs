using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;

namespace GnollHackServer
{
    public class ServerGame
    {

        /* General */
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidVoidCallback();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidCharCallback([MarshalAs(UnmanagedType.LPStr)] string value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidConstCharCallback([MarshalAs(UnmanagedType.LPStr)] string value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate int IntIntCallback(int value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntCallback(int value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntIntCallback(int value1, int value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntIntIntCallback(int value1, int value2, int value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntBooleanCallback(int value1, byte value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntIntConstCharCallback(int value1, int value2, [MarshalAs(UnmanagedType.LPStr)] string value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntIntConstCharIntCallback(int value1, int value2, [MarshalAs(UnmanagedType.LPStr)] string value3, int value4);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidConstCharIntCallback([MarshalAs(UnmanagedType.LPStr)] string value1, int value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidConstCharBooleanCallback([MarshalAs(UnmanagedType.LPStr)] string value1, byte value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate int IntVoidCallback();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanVoidCallback();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanIntDoubleCallback(int value1, double value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanDoubleCallback(double value1);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanDoubleDoubleDoubleDoubleDoubleCallback(double value1, double value2, double value3, double value4, double value5);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanIntDoubleVoidPtrCallback(int value1, double value2, IntPtr value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanDoubleVoidPtrCallback(double value1, IntPtr value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanVoidPtrCallback(IntPtr value1);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntIntIntIntIntCallback(int value1, int value2, int value3, int value4, int value5);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public delegate string CharVoidCallback();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public delegate string CharPtrBooleanCallback(byte value1);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate sbyte CharConstCharPtrConstCharPtrCharCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, sbyte value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidConstCharPtrCharPtrCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate int IntCharCharUintCallback([MarshalAs(UnmanagedType.LPStr)] string value1, [MarshalAs(UnmanagedType.LPStr)] string value2, uint value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate int IntIntIntVoidPtrCallback(int value1, int value2, IntPtr value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate int IntIntPtrIntPtrIntPtrCallback(ref int value1, ref int value2, ref int value3);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntIntPtrIntIntIntUlongPtrCallback(int value1, ref int value2, int value3, int value4, int value5, ref UInt32 value6);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidUlongCallback(UInt32 value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidIntConstCharPtrConstCharPtrBooleanCallback(int value1, string value2, string value3, byte value4);

        [DllImport(@"libgnollhack.dll")]
        public static extern int RunGnollHack(
            UInt32 wincaps1,
            UInt32 wincaps2,
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
            VoidIntIntConstCharIntCallback callback_putstr_ex,
            VoidIntIntConstCharCallback callback_putmixed,
            VoidConstCharBooleanCallback callback_display_file,
            VoidIntCallback callback_start_menu,
            VoidIntCallback callback_add_menu,
            VoidIntCallback callback_add_extended_menu,
            VoidIntIntConstCharCallback callback_end_menu,
            IntIntIntVoidPtrCallback callback_select_menu,
            VoidIntCallback callback_message_menu, /* no need for X-specific handling */
            VoidVoidCallback callback_update_inventory,
            VoidVoidCallback callback_mark_synch,
            VoidVoidCallback callback_wait_synch,
            /* If clipping is on */
            VoidIntIntCallback callback_cliparound,
            /* If positionbar is on */
            VoidCharCallback callback_update_positionbar,
            VoidIntIntIntIntIntCallback callback_print_glyph,
            VoidConstCharCallback callback_raw_print,
            VoidConstCharCallback callback_raw_print_bold,
            IntVoidCallback callback_nhgetch,
            IntIntPtrIntPtrIntPtrCallback callback_nh_poskey,
            VoidVoidCallback callback_nhbell,
            IntVoidCallback callback_doprev_message,
            CharConstCharPtrConstCharPtrCharCallback callback_yn_function,
            VoidConstCharPtrCharPtrCallback callback_getlin,
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
            VoidIntIntPtrIntIntIntUlongPtrCallback callback_status_update,
            BooleanVoidCallback callback_can_suspend_yes,
            VoidVoidCallback callback_stretch_window,
            VoidUlongCallback callback_set_animation_timer,
            VoidIntCallback callback_open_special_view,
            BooleanVoidCallback callback_stop_all_sounds,
            BooleanIntDoubleCallback callback_play_immediate_ghsound,
            BooleanIntDoubleCallback callback_play_ghsound_occupation_ambient,
            BooleanIntDoubleCallback callback_play_ghsound_effect_ambient,
            BooleanDoubleCallback callback_set_effect_ambient_volume,
            BooleanDoubleCallback callback_play_ghsound_music,
            BooleanDoubleCallback callback_play_ghsound_level_ambient,
            BooleanDoubleCallback callback_play_ghsound_environment_ambient,
            BooleanDoubleDoubleDoubleDoubleDoubleCallback callback_adjust_ghsound_general_volumes,
            BooleanIntDoubleVoidPtrCallback callback_add_ambient_ghsound,
            BooleanVoidPtrCallback callback_delete_ambient_ghsound,
            BooleanDoubleVoidPtrCallback callback_set_ambient_ghsound_volume,
            VoidIntCallback callback_exit_hack,
            CharVoidCallback callback_getcwd,
            IntCharCharUintCallback callback_messagebox,
            VoidIntCallback callback_outrip_begin,
            VoidIntCallback callback_outrip_end
        );

        [DllImport(@"libgnollhack.dll")]
        public static extern int RunGnollHackSimple(
            UInt32 wincaps1,
            UInt32 wincaps2
        );

        [DllImport(@"libgnollhack.dll")]
        public static extern int RunGnollHackSimple2(
            UInt32 wincaps1,
            UInt32 wincaps2,
            VoidVoidCallback callback_init_nhwindows);

        [DllImport(@"libgnollhack.dll")]
        public static extern byte dll_validrole(int role);

        [DllImport(@"libgnollhack.dll")]
        public static extern byte dll_str2role([MarshalAs(UnmanagedType.LPStr)] string role_str);

        [DllImport(@"libgnollhack.dll")]
        public static extern int DoSomeCalc2();

        private Thread _gnhthread;
        private ServerGameCenter _serverGameCenter;

        public ServerGame()
        {
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
                GameCallback_VoidIntIntConstCharIntDummy,
                GameCallback_VoidIntIntConstCharDummy,
                GameCallback_VoidConstCharBooleanDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntIntConstCharDummy,
                GameCallback_IntIntIntVoidPtrDummy,
                GameCallback_VoidIntDummy, /* no need for X-specific handling */
                GameCallback_VoidVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidVoidDummy,
                /* If clipping is on */
                GameCallback_VoidIntIntDummy,
                /* If positionbar is on */
                GameCallback_VoidCharDummy,
                GameCallback_VoidIntIntIntIntIntDummy,
                GameCallback_VoidConstCharDummy,
                GameCallback_VoidConstCharDummy,
                GameCallback_IntVoidDummy,
                GameCallback_IntIntPtrIntPtrIntPtrDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_IntVoidDummy,
                GameCallback_CharConstCharPtrConstCharPtrCharDummy,
                GameCallback_VoidConstCharPtrCharPtrDummy,
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
                GameCallback_VoidIntIntPtrIntIntIntUlongPtrDummy,
                GameCallback_BooleanVoidDummy,
                GameCallback_VoidVoidDummy,
                GameCallback_VoidUlongDummy,
                GameCallback_VoidIntDummy,
                GameCallback_BooleanVoidDummy,
                GameCallback_BooleanIntDoubleDummy,
                GameCallback_BooleanIntDoubleDummy,
                GameCallback_BooleanIntDoubleDummy,
                GameCallback_BooleanDoubleDummy,
                GameCallback_BooleanDoubleDummy,
                GameCallback_BooleanDoubleDummy,
                GameCallback_BooleanDoubleDummy,
                GameCallback_BooleanDoubleDoubleDoubleDoubleDoubleDummy,
                GameCallback_BooleanIntDoubleVoidPtrDummy,
                GameCallback_BooleanVoidPtrDummy,
                GameCallback_BooleanDoubleVoidPtrDummy,
                GameCallback_ExitHack,
                GameCallback_GetCwd,
                GameCallback_MessageBox,
                GameCallback_VoidIntDummy,
                GameCallback_VoidIntDummy
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
            return 1;
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

        /*
        typedef void (__stdcall* VoidVoidCallback) ();
        typedef void (__stdcall* VoidCharCallback) (char*);
        typedef void (__stdcall* VoidConstCharCallback) (const char*);
        typedef int (__stdcall* IntIntCallback) (int);
        typedef void (__stdcall* VoidIntCallback) (int);
        typedef void (__stdcall* VoidIntIntCallback) (int, int);
        typedef void (__stdcall* VoidIntIntIntCallback) (int, int, int);
        typedef void (__stdcall* VoidIntBooleanCallback) (int, unsigned char);
        typedef void (__stdcall* VoidIntIntConstCharCallback) (int, const char*);
        typedef void (__stdcall* VoidConstCharIntCallback) (const char*, int);
        typedef void (__stdcall* VoidConstCharBooleanCallback) (const char*, unsigned char);
        typedef int (__stdcall* IntVoidCallback) ();
        typedef int (__stdcall* BooleanVoidCallback) ();
         */
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
        protected void GameCallback_VoidIntIntDummy(int value1, int value2)
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
        protected void GameCallback_VoidIntIntConstCharIntDummy(int value1, int value2, string value3, int value4)
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
        protected int GameCallback_IntIntIntVoidPtrDummy(int value1, int value2, IntPtr value3)
        {
            return 0;
        }
        protected int GameCallback_IntIntPtrIntPtrIntPtrDummy(ref int value1, ref int value2, ref int value3)
        {
            return 0;
        }
        protected void GameCallback_VoidIntIntPtrIntIntIntUlongPtrDummy(int value1, ref int value2, int value3, int value4, int value5, ref UInt32 value6)
        {
            return;
        }
        protected void GameCallback_VoidUlongDummy(UInt32 value1)
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
        protected sbyte GameCallback_CharConstCharPtrConstCharPtrCharDummy(string value1, string value2, sbyte value3)
        {
            return 0;
        }
        protected void GameCallback_VoidConstCharPtrCharPtrDummy(string value1, string value2)
        {
            return;
        }
        protected string GameCallback_CharPtrBooleanDummy(byte value1)
        {
            return "message here";
        }
        protected string GameCallback_CharVoidDummy()
        {
            return "";
        }
        protected void GameCallback_VoidIntIntIntIntIntDummy(int value1, int value2, int value3, int value4, int value5)
        {
            return;
        }
        protected void GameCallback_VoidIntConstCharPtrConstCharPtrBooleanDummy(int value1, string value2, string value3, byte value4)
        {
            return;
        }
    }
}

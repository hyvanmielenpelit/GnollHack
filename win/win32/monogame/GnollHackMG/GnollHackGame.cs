using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Runtime.InteropServices;
using Microsoft.AspNetCore.SignalR.Client;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authentication.Cookies;
using System.Buffers.Text;
using System.Text.Encodings.Web;
using System.Collections.Generic;
using System.Net.Http;
using System.IO;
using System.Threading;
using System.Diagnostics;
using GnollHackCommon;

namespace GnollHackMG
{

    public class GnollHackGame : Game
    {
        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;
        private SpriteFont _spriteFont;
        private HubConnection connection;
        private string _message = "";
        private string _message2 = "";
        private string _message3 = "";
        private string _message4 = "";
        private string _message5 = "";
        private int _result = 0;
        private int _result2 = 0;
        private string _accessToken = "MyAccessToken";
        private Thread _gnhthread;

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
        
        [DllImport(@"libgnollhack.dll", CharSet = CharSet.Unicode)]
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
        public static extern byte dll_validrole(int role);

        [DllImport(@"libgnollhack.dll")]
        public static extern byte dll_str2role([MarshalAs(UnmanagedType.LPStr)] string role_str);

        [DllImport(@"libgnollhack.dll", CharSet = CharSet.Unicode)]
        public static extern int DoSomeCalc2();



        public GnollHackGame()
        {
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;

            // TODO: use this.Content to load your game content here
            connection = new HubConnectionBuilder()
                .WithUrl("https://localhost:44333/gnollhack", options =>
                {
                    var cookie = new System.Net.Cookie(".AspNetCore.Cookies", _accessToken, "/", "localhost");
                    options.Cookies.Add(cookie); 
                })
                .Build();

            connection.Closed += async (error) =>
            {
                await Task.Delay(new Random().Next(0, 5) * 1000);
                await connection.StartAsync();
            };

            connection.On<string, string>("ReceiveMessage", (user, message) =>
            {
                _message = message;
            });

            connection.On<int>("CalcResult", (result) =>
            {
                _result = result;
            });

            connection.On<string, string>("LoginMessage", (user, message) =>
            {
                _message2 = message;
            });

            connection.On<int>("AddNewGameResult", (result) =>
            {
                _message3 = "New Game Added: "+ result;
            });

            connection.On<bool>("GameAliveResult", (result) =>
            {
                _message4 = "Game Alive: " + result.ToString();
            });
            connection.On<int, int>("Client_ExitHack", (hash, status) =>
            {
                _message5 = "ExitHack: Hash: " + hash +", Status: " + status;
            });
            connection.On<int>("Client_PlayerSelection", (hash) =>
            {
                _message5 = "PlayerSelection: Hash: " + hash;
            });
        }


        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            base.Initialize();
        }

        protected async override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);
            _spriteFont = Content.Load<SpriteFont>("Font1");

            _result2 = DoSomeCalc2();

            try
            {
                using var client = new HttpClient();

                var content = new FormUrlEncodedContent(new[]
                {
                    new KeyValuePair<string, string>("Input.UserName", "Tommi"),
                    new KeyValuePair<string, string>("Input.Password", "HMPTommi1!")
                });

                //var result = await client.PostAsync("https://localhost:44333/Identity/Account/LoginRemote", content);
                var s = "https://localhost:44333/Identity/Account/LoginRemote?UserName=Tommi&Password=HMPTommi1!";
                var result = await client.GetAsync(s);

                await connection.StartAsync();

                await connection.InvokeAsync("SendMessage",
                    "user", "My message");

                await connection.InvokeAsync("DoCalc");

                await connection.InvokeAsync("AddNewServerGame");
            }
            catch (Exception ex)
            {
                //Error
            }
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Add your update logic here

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            _spriteBatch.Begin();

            byte fromDLLvalue2 = dll_validrole(2);
            int fromDLLvalue3 = dll_str2role("knight");
            int fromDLL2value1 = _result2;

            // define a progress callback delegate

            _spriteBatch.DrawString(_spriteFont, "Hello World!", new Vector2(10, 10), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message, new Vector2(10, 70), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message2, new Vector2(10, 90), Color.White);
            _spriteBatch.DrawString(_spriteFont, _result.ToString(), new Vector2(10, 110), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLLvalue2.ToString(), new Vector2(10, 150), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLLvalue3.ToString(), new Vector2(10, 170), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLL2value1.ToString(), new Vector2(10, 190), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message3, new Vector2(10, 210), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message4, new Vector2(10, 230), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message5, new Vector2(10, 250), Color.White);

            _spriteBatch.End();

            base.Draw(gameTime);
        }

        protected void GNHThreadProc()
        {
            if (0 == 1)
            {
                RunGnollHack(
                    0,
                    0,
                    MG_InitWindows,
                    MG_IntVoidDummy,
                    MG_AskName,
                    MG_VoidVoidDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidVoidDummy,
                    MG_IntIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntBooleanDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntIntIntDummy,
                    MG_VoidIntIntConstCharIntDummy,
                    MG_VoidIntIntConstCharDummy,
                    MG_VoidConstCharBooleanDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntIntConstCharDummy,
                    MG_IntIntIntVoidPtrDummy,
                    MG_VoidIntDummy, /* no need for X-specific handling */
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    /* If clipping is on */
                    MG_VoidIntIntDummy,
                    /* If positionbar is on */
                    MG_VoidCharDummy,
                    MG_VoidIntIntIntIntIntDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidConstCharDummy,
                    MG_IntVoidDummy,
                    MG_IntIntPtrIntPtrIntPtrDummy,
                    MG_VoidVoidDummy,
                    MG_IntVoidDummy,
                    MG_CharConstCharPtrConstCharPtrCharDummy,
                    MG_VoidConstCharPtrCharPtrDummy,
                    MG_IntVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,

                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_CharVoidDummy,

                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidConstCharDummy,
                    MG_CharPtrBooleanDummy,
                    MG_VoidConstCharBooleanDummy,
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntConstCharPtrConstCharPtrBooleanDummy,
                    MG_VoidIntIntPtrIntIntIntUlongPtrDummy,
                    MG_BooleanVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidUlongDummy,
                    MG_VoidIntDummy,
                    MG_BooleanVoidDummy,
                    MG_BooleanIntDoubleDummy,
                    MG_BooleanIntDoubleDummy,
                    MG_BooleanIntDoubleDummy,
                    MG_BooleanDoubleDummy,
                    MG_BooleanDoubleDummy,
                    MG_BooleanDoubleDummy,
                    MG_BooleanDoubleDummy,
                    MG_BooleanDoubleDoubleDoubleDoubleDoubleDummy,
                    MG_BooleanIntDoubleVoidPtrDummy,
                    MG_BooleanVoidPtrDummy,
                    MG_BooleanDoubleVoidPtrDummy,
                    MG_ExitHack,
                    MG_GetCwd,
                    MG_MessageBox,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy
                );
            }
        }
        protected override void BeginRun()
        {
            base.BeginRun();

            Thread t = new Thread(GNHThreadProc);
            _gnhthread = t;
            t.Start();
            Debug.WriteLine("Thread started");

        }


        protected void MG_InitWindows()
        {

        }


        protected string MG_AskName()
        {
            return "Janne Test";
        }

        protected void MG_ExitHack(int status)
        {
            Debug.WriteLine("ExitHack called");
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
        protected void MG_VoidVoidDummy()
        {

        }
        protected void MG_VoidCharDummy(string value)
        {

        }
        protected void MG_VoidConstCharDummy(string value)
        {

        }
        protected int MG_IntIntDummy(int value1)
        {
            return 0;
        }
        protected void MG_VoidIntDummy(int value1)
        {

        }
        protected void MG_VoidIntIntDummy(int value1, int value2)
        {

        }
        protected void MG_VoidIntIntIntDummy(int value1, int value2, int value3)
        {

        }
        protected void MG_VoidIntBooleanDummy(int value1, byte value2)
        {

        }
        protected void MG_VoidIntCharDummy(int value1, string value2)
        {

        }
        protected void MG_VoidIntIntConstCharDummy(int value1, int value2, string value3)
        {

        }
        protected void MG_VoidIntIntConstCharIntDummy(int value1, int value2, string value3, int value4)
        {

        }
        protected void MG_VoidConstCharIntDummy(string value1, int value2)
        {

        }
        protected void MG_VoidConstCharBooleanDummy(string value1, byte value2)
        {

        }
        protected int MG_IntVoidDummy()
        {
            return 0;
        }
        protected byte MG_BooleanVoidDummy()
        {
            return 0;
        }

        protected string MG_GetCwd()
        {
            return Directory.GetCurrentDirectory();
        }
        protected int MG_MessageBox(string text, string title, uint type)
        {
            return 1;
        }
        protected int MG_IntIntIntVoidPtrDummy(int value1, int value2, IntPtr value3)
        {
            return 0;
        }
        protected int MG_IntIntPtrIntPtrIntPtrDummy(ref int value1, ref int value2, ref int value3)
        {
            return 0;
        }
        protected void MG_VoidIntIntPtrIntIntIntUlongPtrDummy(int value1, ref int value2, int value3, int value4, int value5, ref UInt32 value6)
        {
            return;
        }
        protected void MG_VoidUlongDummy(UInt32 value1)
        {

        }
        protected byte MG_BooleanIntDoubleDummy(int value1, double value2)
        {
            return 0;
        }
        protected byte MG_BooleanDoubleDummy(double value1)
        {
            return 0;
        }
        protected byte MG_BooleanDoubleDoubleDoubleDoubleDoubleDummy(double value1, double value2, double value3, double value4, double value5)
        {
            return 0;
        }
        protected byte MG_BooleanIntDoubleVoidPtrDummy(int value1, double value2, IntPtr value3)
        {
            return 0;
        }
        protected byte MG_BooleanDoubleVoidPtrDummy(double value1, IntPtr value2)
        {
            return 0;
        }
        protected byte MG_BooleanVoidPtrDummy(IntPtr value1)
        {
            return 0;
        }
        protected sbyte MG_CharConstCharPtrConstCharPtrCharDummy(string value1, string value2, sbyte value3)
        {
            return 0;
        }
        protected void MG_VoidConstCharPtrCharPtrDummy(string value1, string value2)
        {
            return;
        }
        protected string MG_CharPtrBooleanDummy(byte value1)
        {
            return "message here";
        }
        protected string MG_CharVoidDummy()
        {
            return "";
        }
        protected void MG_VoidIntIntIntIntIntDummy(int value1, int value2, int value3, int value4, int value5)
        {
            return;
        }
        protected void MG_VoidIntConstCharPtrConstCharPtrBooleanDummy(int value1, string value2, string value3, byte value4)
        {
            return;
        }
    }
}

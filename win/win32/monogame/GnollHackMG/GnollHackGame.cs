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
        private string _accessToken = "MyAccessToken";

        /* All callback delegates */
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
        public delegate void VoidConstCharIntCallback([MarshalAs(UnmanagedType.LPStr)] string value1, int value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidConstCharBooleanCallback([MarshalAs(UnmanagedType.LPStr)] string value1, byte value2);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate int IntVoidCallback();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate byte BooleanVoidCallback();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        public delegate string CharVoidCallback();


        /* Specific */
        /*
        typedef VoidVoidCallback InitWindowsCallback;
        typedef IntVoidCallback PlayerSelectionCallback;
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
        typedef void (__stdcall* AddMenuCallback) (int);
        typedef void (__stdcall* AddExtendedMenuCallback) (int);
        typedef VoidIntIntConstCharCallback EndMenuCallback;
        typedef void (__stdcall* SelectMenuCallback) (int);
        typedef void (__stdcall* MessageMenuCallback) (int);
        typedef VoidVoidCallback UpdateInventoryCallback;
        typedef VoidVoidCallback MarkSynchCallback;
        typedef VoidVoidCallback WaitSynchCallback;
        typedef VoidIntIntCallback ClipAroundCallback;
        typedef VoidCharCallback UpdatePositionBarCallback;
        typedef void (__stdcall* PrintGlyphCallback) (int);
        typedef VoidConstCharCallback RawPrintCallback;
        typedef VoidConstCharCallback RawPrintBoldCallback;
        typedef IntVoidCallback GetChCallback;
        typedef VoidVoidCallback BellCallback;
        typedef void (__stdcall* PosKeyCallback) (int);
        typedef IntVoidCallback DoPrevMessageCallback;
        typedef void (__stdcall* YnFunctionCallback) (int);
        typedef void (__stdcall* GetLinCallback) (int);
        typedef IntVoidCallback GetExtCmdCallback;
        typedef VoidIntCallback NumberPadCallback;
        typedef VoidVoidCallback DelayOutputCallback;
        typedef VoidIntCallback DelayOutputMillisecondsCallback;
        typedef VoidIntCallback DelayOutputIntervalsCallback;
        typedef void (__stdcall* ChangeColorCallback) (int);
        typedef VoidIntCallback ChangeBackgroundCallback;
        typedef void (__stdcall* SetFontNameCallback) (int);
        typedef const char* (__stdcall * GetColorStringCallback)(int);
        typedef VoidVoidCallback StartScreenCallback;
        typedef VoidVoidCallback EndScreenCallback;
        typedef void (__stdcall* OutRipCallback) (int);
        typedef VoidConstCharCallback PreferenceUpdateCallback;
        typedef void (__stdcall* GetMsgHistoryCallback) (int);
        typedef VoidConstCharBooleanCallback PutMsgHistoryCallback;
        typedef VoidVoidCallback StatusInitCallback;
        typedef VoidVoidCallback StatusFinishCallback;
        typedef void (__stdcall* StatusEnableFieldCallback) (int);
        typedef void (__stdcall* StatusUpdateCallback) (int);
        typedef BooleanVoidCallback CanSuspendYesCallback;
        typedef VoidVoidCallback StretchWindowCallback;
        typedef void (__stdcall* SetAnimationTimerCallback) (int);
        typedef void (__stdcall* OpenSpecialViewCallback) (int);
        typedef void (__stdcall* StopAllSoundsCallback) (int);
        typedef void (__stdcall* PlayImmediateSoundCallback) (int);
        typedef void (__stdcall* PlayOccupationAmbientCallback) (int);
        typedef void (__stdcall* PlayEffectAmbientCallback) (int);
        typedef void (__stdcall* SetEffectAmbientVolumeCallback) (int);
        typedef void (__stdcall* PlayMusicCallback) (int);
        typedef void (__stdcall* PlayLevelAmbientCallback) (int);
        typedef void (__stdcall* PlayEnvironmentAmbientCallback) (int);
        typedef void (__stdcall* AdjustGeneralVolumesCallback) (int);
        typedef void (__stdcall* AddAmbientSoundCallback) (int);
        typedef void (__stdcall* DeleteAmbientSoundCallback) (int);
        typedef void (__stdcall* SetAmbientVolumeCallback) (int);
        typedef void (__stdcall* ExitHackCallback) (int);         
        */
        [DllImport(@"GnollHackDLL.dll", CharSet = CharSet.Unicode)]
        public static extern int RunGnollHack(
            UInt32 wincaps1,
            UInt32 wincaps2,
            VoidVoidCallback callback_init_nhwindows,
            IntVoidCallback callback_player_selection,
            VoidVoidCallback callback_askname,
            VoidVoidCallback callback_get_nh_event,
            VoidConstCharCallback callback_exit_nhwindows,
            VoidConstCharCallback callback_suspend_nhwindows,
            VoidVoidCallback callback_resume_nhwindows,
            IntIntCallback callback_create_nhwindow,
            VoidIntCallback callback_clear_nhwindow,
            VoidIntBooleanCallback callback_display_nhwindow,
            VoidIntCallback callback_destroy_nhwindow,
            VoidIntIntIntCallback callback_curs,
            VoidIntIntConstCharCallback callback_putstr,
            VoidIntIntConstCharCallback callback_putmixed,
            VoidConstCharBooleanCallback callback_display_file,
            VoidIntCallback callback_start_menu,
            VoidIntCallback callback_add_menu,
            VoidIntCallback callback_add_extended_menu,
            VoidIntIntConstCharCallback callback_end_menu,
            VoidIntCallback callback_select_menu,
            VoidIntCallback callback_message_menu, /* no need for X-specific handling */
            VoidVoidCallback callback_update_inventory,
            VoidVoidCallback callback_mark_synch,
            VoidVoidCallback callback_wait_synch,
            /* If clipping is on */
            VoidIntIntCallback callback_cliparound,
            /* If positionbar is on */
            VoidCharCallback callback_update_positionbar,
            VoidIntCallback callback_print_glyph,
            VoidConstCharCallback callback_raw_print,
            VoidConstCharCallback callback_raw_print_bold,
            IntVoidCallback callback_nhgetch,
            VoidIntCallback callback_nh_poskey,
            VoidVoidCallback callback_nhbell,
            IntVoidCallback callback_doprev_message,
            VoidIntCallback callback_yn_function,
            VoidIntCallback callback_getlin,
            IntVoidCallback callback_get_ext_cmd,
            VoidIntCallback callback_number_pad,
            VoidVoidCallback callback_delay_output,
            VoidIntCallback callback_delay_output_milliseconds,
            VoidIntCallback callback_delay_output_intervals,

            VoidIntCallback callback_change_color,
            VoidIntCallback callback_change_background,
            VoidIntCallback callback_set_font_name,
            VoidIntCallback callback_get_color_string,

            VoidVoidCallback callback_start_screen,
            VoidVoidCallback callback_end_screen,
            VoidIntCallback callback_outrip,
            VoidConstCharCallback callback_preference_update,
            VoidIntCallback callback_getmsghistory,
            VoidConstCharBooleanCallback callback_putmsghistory,
            VoidVoidCallback callback_status_init,
            VoidVoidCallback callback_status_finish,
            VoidIntCallback callback_status_enablefield,
            VoidIntCallback callback_status_update,
            BooleanVoidCallback callback_can_suspend_yes,
            VoidVoidCallback callback_stretch_window,
            VoidIntCallback callback_set_animation_timer,
            VoidIntCallback callback_open_special_view,
            VoidIntCallback callback_stop_all_sounds,
            VoidIntCallback callback_play_immediate_ghsound,
            VoidIntCallback callback_play_ghsound_occupation_ambient,
            VoidIntCallback callback_play_ghsound_effect_ambient,
            VoidIntCallback callback_set_effect_ambient_volume,
            VoidIntCallback callback_play_ghsound_music,
            VoidIntCallback callback_play_ghsound_level_ambient,
            VoidIntCallback callback_play_ghsound_environment_ambient,
            VoidIntCallback callback_adjust_ghsound_general_volumes,
            VoidIntCallback callback_add_ambient_ghsound,
            VoidIntCallback callback_delete_ambient_ghsound,
            VoidIntCallback callback_set_ambient_ghsound_volume,
            VoidIntCallback callback_exit_hack,
            CharVoidCallback callback_getcwd
        );

        [DllImport(@"GnollHackDLL.dll", CharSet = CharSet.Unicode)]
        public static extern int DoSomeCalc();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void ProgressCallback(int value);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern void DoWork([MarshalAs(UnmanagedType.FunctionPtr)] ProgressCallback callbackPointer);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern byte dll_validrole(int role);

        [DllImport(@"GnollHackDLL.dll")]
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

            connection.On<string, string>("LoginMessage", (user, message) =>
            {
                _message2 = message;
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

            int fromDLLvalue = DoSomeCalc();
            byte fromDLLvalue2 = dll_validrole(2);
            int fromDLLvalue3 = dll_str2role("knight");

            int fromDLL2value1 = DoSomeCalc2();

            // define a progress callback delegate
            ProgressCallback callback =
                (value) =>
                {
                    _spriteBatch.DrawString(_spriteFont, value.ToString(), new Vector2(10, 50), Color.White);
                };

            _spriteBatch.DrawString(_spriteFont, "Hello World!", new Vector2(10, 10), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLLvalue.ToString(), new Vector2(10, 30), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message, new Vector2(10, 70), Color.White);
            _spriteBatch.DrawString(_spriteFont, _message2, new Vector2(10, 110), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLLvalue2.ToString(), new Vector2(10, 150), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLLvalue3.ToString(), new Vector2(10, 190), Color.White);
            _spriteBatch.DrawString(_spriteFont, fromDLL2value1.ToString(), new Vector2(10, 230), Color.White);

            DoWork(callback);

            _spriteBatch.End();

            base.Draw(gameTime);
        }

        protected override void BeginRun()
        {
            base.BeginRun();

            if (0 == 1)
            {
                RunGnollHack(
                    0,
                    0,
                    MG_InitWindows,
                    MG_IntVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidVoidDummy,
                    MG_IntIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntBooleanDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntIntIntDummy,
                    MG_VoidIntIntConstCharDummy,
                    MG_VoidIntIntConstCharDummy,
                    MG_VoidConstCharBooleanDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntIntConstCharDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy, /* no need for X-specific handling */
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    /* If clipping is on */
                    MG_VoidIntIntDummy,
                    /* If positionbar is on */
                    MG_VoidCharDummy,
                    MG_VoidIntDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidConstCharDummy,
                    MG_IntVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidVoidDummy,
                    MG_IntVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_IntVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,

                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,

                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidConstCharDummy,
                    MG_VoidIntDummy,
                    MG_VoidConstCharBooleanDummy,
                    MG_VoidVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_BooleanVoidDummy,
                    MG_VoidVoidDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_VoidIntDummy,
                    MG_GetCwd
                );
            }
        }


        protected void MG_InitWindows()
        {

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
    }
}

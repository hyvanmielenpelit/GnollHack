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
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidVoidCallback();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void VoidCharCallback([MarshalAs(UnmanagedType.LPStr)] string value);
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
        public static extern int DoSomeCalc();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void ProgressCallback(int value);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern void DoWork([MarshalAs(UnmanagedType.FunctionPtr)] ProgressCallback callbackPointer);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern byte dll_validrole(int role);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern byte dll_str2role([MarshalAs(UnmanagedType.LPStr)] string role_str);

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

            DoWork(callback);

            _spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}

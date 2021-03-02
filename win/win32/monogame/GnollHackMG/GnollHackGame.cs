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

        [DllImport(@"GnollHackDLL.dll", CharSet = CharSet.Unicode)]
        public static extern int DoSomeCalc();

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void ProgressCallback(int value);

        [DllImport(@"GnollHackDLL.dll")]
        public static extern void DoWork([MarshalAs(UnmanagedType.FunctionPtr)] ProgressCallback callbackPointer);


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
                await connection.StartAsync();

                await connection.InvokeAsync("Login",
                    "Tommi", "HMPTommi1");
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

            DoWork(callback);

            _spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}

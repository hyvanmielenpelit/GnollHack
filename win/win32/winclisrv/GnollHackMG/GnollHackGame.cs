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

        public GnollHackGame()
        {
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;

            // TODO: use this.Content to load your game content here
            connection = new HubConnectionBuilder()
                .WithUrl("http://127.0.0.1:57061/gnollhack", options =>
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
            connection.On<GHCommandFromServer>("CommandFromServer", (command) =>
            {
                _message5 = "CommandFromServer: "+ command.CommandName +", GUID: " + command.Id.ToString();
            });
            connection.On<Guid, int>("ResponseFromClientResult", (guid, result) =>
            {
                _message5 = "ResponseFromClientResult: " + result + ", GUID: " + guid;
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

            _result2 = 0; // DoSomeCalc2();

            try
            {
                var client = new HttpClient();
                var content = new FormUrlEncodedContent(new[]
                {
                    new KeyValuePair<string, string>("Input.UserName", "Tommi"),
                    new KeyValuePair<string, string>("Input.Password", "HMPTommi1!")
                });

                //var result = await client.PostAsync("https://localhost:44333/Identity/Account/LoginRemote", content);
                //var s = "https://localhost:44333/Identity/Account/LoginRemote?UserName=Tommi&Password=HMPTommi1!";
                //var result = await client.GetAsync(s);

                await connection.StartAsync();

                await connection.InvokeAsync("SendMessage",
                    "user", "My message");

                await connection.InvokeAsync("DoCalc");

                await connection.InvokeAsync("AddNewServerGame");
            }
            catch
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

            byte fromDLLvalue2 = 0; // dll_validrole(2);
            int fromDLLvalue3 = 0; // dll_str2role("knight");
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

        protected async void ClientPlayerSelectionResponse(GHCommandFromServer command, int selectedRole, int selectedRace, int selectedGender, int selectedAlignment)
        {
            GHResponseFromClient response = new GHResponseFromClient(command.Id);
            response.ReturnValues.Add("selectedRole", selectedRole);
            response.ReturnValues.Add("selectedRace", selectedRace);
            response.ReturnValues.Add("selectedGender", selectedGender);
            response.ReturnValues.Add("selectedAlignment", selectedAlignment);
            await connection.InvokeAsync("ResponseFromClient", response);
        }
    }
}

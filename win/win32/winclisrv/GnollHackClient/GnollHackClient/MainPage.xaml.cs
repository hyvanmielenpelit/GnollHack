using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using SkiaSharp;
using SkiaSharp.Views.Forms;

using System.Runtime.InteropServices;
using Microsoft.AspNetCore.SignalR.Client;
using Microsoft.AspNetCore.Authentication;
using System.Buffers.Text;
using System.Text.Encodings.Web;
using System.Net.Http;
using System.IO;
using System.Threading;
using System.Diagnostics;
using GnollHackCommon;

namespace GnollHackClient
{
    public partial class MainPage : ContentPage
    {
        private HubConnection connection;
        private string _message = "";
        private string _message2 = "";
        private string _message3 = "";
        private string _message4 = "";
        private string _message5 = "";
        private int _result = 0;
        private int _result2 = 0;
        private string _accessToken = "MyAccessToken";

        public MainPage()
        {
            InitializeComponent();

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
                _message3 = "New Game Added: " + result;
            });

            connection.On<bool>("GameAliveResult", (result) =>
            {
                _message4 = "Game Alive: " + result.ToString();
            });
            connection.On<int, int>("Client_ExitHack", (hash, status) =>
            {
                _message5 = "ExitHack: Hash: " + hash + ", Status: " + status;
            });
            connection.On<int>("Client_PlayerSelection", (hash) =>
            {
                _message5 = "PlayerSelection: Hash: " + hash;
            });
            connection.On<GHCommandFromServer>("CommandFromServer", (command) =>
            {
                _message5 = "CommandFromServer: " + command.CommandName + ", GUID: " + command.Id.ToString();
            });
            connection.On<Guid, int>("ResponseFromClientResult", (guid, result) =>
            {
                _message5 = "ResponseFromClientResult: " + result + ", GUID: " + guid;
            });

            LoadContent();
        }

        protected async void LoadContent()
        {

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

        private void canvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;

            canvas.Clear(SKColors.CornflowerBlue);

            string str = "This is GnollHack!";

            // Create an SKPaint object to display the text
            SKPaint textPaint = new SKPaint
            {
                Color = SKColors.Chocolate
            };

            // Adjust TextSize property so text is 90% of screen width
            float textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.9f * info.Width * textPaint.TextSize / textWidth;

            // Find the text bounds
            SKRect textBounds = new SKRect();
            textPaint.MeasureText(str, ref textBounds);

            float xText = info.Width / 2 - textBounds.MidX;
            float yText = info.Height / 2 - textBounds.MidY;

            // And draw the text
            canvas.DrawText(str, xText, yText, textPaint);

            str = "Message = " + _message;
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 50;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message2;
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 100;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _result.ToString();
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 150;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _result2.ToString();
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 200;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message3;
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 250;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message4;
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 300;
            canvas.DrawText(str, xText, yText, textPaint);

            str = _message5;
            textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.5f * info.Width * textPaint.TextSize / textWidth;
            xText = info.Width / 2 - textBounds.MidX;
            yText = info.Height / 2 - textBounds.MidY + 350;
            canvas.DrawText(str, xText, yText, textPaint);

        }
    }
}

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
using System.Reflection;
using GnollHackCommon.Authentication;
using Newtonsoft.Json;
using System.Net;
using System.Net.Http.Headers;
using GnollHackClient.Pages.Game;

namespace GnollHackClient
{
    public partial class MainPage : ContentPage
    {
        SKBitmap _background_bitmap;
        SKBitmap _logo_bitmap;
        private bool _canClickButton = true;
        private bool _serverButtonClicked = false;
        private NavigationPage _loginNavPage = null;
        private IGnollHackService _mainGnollHackService = null;

        public MainPage()
        {
            InitializeComponent();

            _mainGnollHackService = DependencyService.Get<IGnollHackService>();
            _mainGnollHackService.LoadLibrary();
            int testres = _mainGnollHackService.Test1();

            Assembly assembly = GetType().GetTypeInfo().Assembly;

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.window_background.png"))
            {
                _background_bitmap = SKBitmap.Decode(stream);
            }

            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.gnollhack-logo-test-2.png"))
            {
                _logo_bitmap = SKBitmap.Decode(stream);
            }

            //firstButton.ImageSource = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            myImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            myImage2.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
        }

        private void canvasView_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKImageInfo info = e.Info;
            SKSurface surface = e.Surface;
            SKCanvas canvas = surface.Canvas;

            canvas.Clear(SKColors.DarkGray);

            float x = info.Width;
            float y = info.Height;

            SKRect bkdest = new SKRect(0f, 0f, x, y);
            canvas.DrawBitmap(_background_bitmap, bkdest);

            float logo_width = (float)info.Width;
            float logo_height = (float)_logo_bitmap.Height * logo_width / ((float)_logo_bitmap.Width);
            x = (info.Width - logo_width) / 2;
            y = (float)info.Height * 0.2f;
            SKRect dest = new SKRect(x, y, x + logo_width, y + logo_height);
            canvas.DrawBitmap(_logo_bitmap, dest);

            string str = "GnollHack 4.1.0";

            // Create an SKPaint object to display the text
            SKPaint textPaint = new SKPaint
            {
                Color = SKColors.Black
            };

            textPaint.Typeface = App.DiabloTypeface;

            // Adjust TextSize property so text is 90% of screen width
            float textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.65f * info.Width * textPaint.TextSize / textWidth;

            // Find the text bounds
            SKRect textBounds = new SKRect();
            textPaint.MeasureText(str, ref textBounds);

            float xText = info.Width / 2 - textBounds.MidX;
            float yText = y + logo_height + 30; // info.Height / 2 - textBounds.MidY;

            // And draw the text
            canvas.DrawText(str, xText, yText, textPaint);

            str = "Android Version";
            yText = yText + textPaint.TextSize + 5;
            textPaint.MeasureText(str, ref textBounds);
            xText = info.Width / 2 - textBounds.MidX;
            canvas.DrawText(str, xText, yText, textPaint);
        }

        public void HideLocalGameButton()
        {
            StartLocalGameButton.IsVisible = false;
            ExitAppButton.IsVisible = true;
        }
        private async void localButton_Clicked(object sender, EventArgs e)
        {
            StartLocalGameButton.IsEnabled = false;
            var gamePage = new GamePage(this);
            gamePage.EnableWizardMode = wizardModeSwitch.IsToggled;
            await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
            gamePage.StartGame();
        }

        private async void serverButton_Clicked(object sender, EventArgs e)
        {
            if(_canClickButton == false)
            {
                return;
            }

            _serverButtonClicked = true;
            _canClickButton = false;

            var loginPage = new LoginPage(this);
            _loginNavPage = new NavigationPage(loginPage);

            await Navigation.PushAsync(_loginNavPage);
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {

        }

        private void ExitAppButton_Clicked(object sender, EventArgs e)
        {
            App.AppCloseService.CloseApplication();
            Thread.Sleep(50);
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }
    }
}

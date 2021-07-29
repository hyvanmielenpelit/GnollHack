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
using FFImageLoading;
using Xamarin.Essentials;

namespace GnollHackClient
{
    public partial class MainPage : ContentPage
    {
        SKBitmap _background_bitmap;
        SKBitmap _logo_bitmap;
        private bool _canClickButton = true;
        private bool _serverButtonClicked = false;
        private NavigationPage _loginNavPage = null;

        public MainPage()
        {
            InitializeComponent();
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

            float scale = info.Width < info.Height ? 1.0f : (float)info.Height / (float)info.Width;
            float scalesquared = (float)Math.Pow((double)scale, 2.0);
            float logo_width = (float)info.Width * scalesquared;
            float logo_height = (float)_logo_bitmap.Height * logo_width / ((float)_logo_bitmap.Width);
            x = (info.Width - logo_width) / 2;
            y = (float)info.Height * 0.175f;
            SKRect dest = new SKRect(x, y, x + logo_width, y + logo_height);
            canvas.DrawBitmap(_logo_bitmap, dest);

            string str = scale == 1.0f ? "GnollHack " + App.GHVersionString : "GnollHack " + App.GHVersionString + " Android";

            // Create an SKPaint object to display the text
            SKPaint textPaint = new SKPaint
            {
                Color = SKColors.Black
            };

            textPaint.Typeface = App.DiabloTypeface;

            // Adjust TextSize property so text is 90% of screen width
            float textWidth = textPaint.MeasureText(str);
            textPaint.TextSize = 0.65f * scale * info.Width * textPaint.TextSize / textWidth;

            // Find the text bounds
            SKRect textBounds = new SKRect();
            textPaint.MeasureText(str, ref textBounds);
            float xText = info.Width / 2 - textBounds.MidX;
            float yText = y + logo_height + 30; // info.Height / 2 - textBounds.MidY;

            canvas.DrawText(str, xText, yText, textPaint);


            if (info.Width < info.Height)
            {
                str = "Android Version";
                yText = yText + textPaint.TextSize + 5;
                textPaint.MeasureText(str, ref textBounds);
                xText = info.Width / 2 - textBounds.MidX;
                canvas.DrawText(str, xText, yText, textPaint);
            }

            str = App.GHVersionId;
            textPaint.Typeface = App.LatoRegular;
            textPaint.TextSize = 15;
            textPaint.Color = SKColors.White;
            yText = 5 - textPaint.FontMetrics.Ascent;
            xText = 5;
            canvas.DrawText(str, xText, yText, textPaint);

        }

        public void HideLocalGameButton()
        {
            StartLocalGameButton.IsVisible = false;
            StartLocalGameImage.IsVisible = false;
            StartLocalGameButton.TextColor = Color.Gray;
        }
        private async void localButton_Clicked(object sender, EventArgs e)
        {
            StartLocalGameButton.IsEnabled = false;
            StartLocalGameButton.TextColor = Color.Gray;
            var gamePage = new GamePage(this);
            gamePage.EnableWizardMode = wizardModeSwitch.IsToggled;
            await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
            gamePage.StartGame();
        }

        private async void serverButton_Clicked(object sender, EventArgs e)
        {
            if (_canClickButton == false)
            {
                return;
            }

            _serverButtonClicked = true;
            _canClickButton = false;

            var loginPage = new LoginPage(this);
            _loginNavPage = new NavigationPage(loginPage);

            await Navigation.PushAsync(_loginNavPage);
        }

        private bool _firsttime = true;
        private int starttimercount = 0;
        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            wizardModeGrid.IsVisible = App.DeveloperMode;
            if (!App.DeveloperMode)
                wizardModeSwitch.IsToggled = false;

            if (_firsttime)
            {
                _firsttime = false;
                StartFadeLogoIn();
                Assembly thisassembly = GetType().GetTypeInfo().Assembly;
                FmodLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.FMOD-Logo-32-White.png", thisassembly);
                StartLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                MainLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                Device.StartTimer(TimeSpan.FromSeconds(1f / 4), () =>
                {
                    bool res = false;
                    switch (starttimercount)
                    {
                        case 0:
                            App.LoadServices();
                            string verstr = App.GnollHackService.GetVersionString();
                            string verid = App.GnollHackService.GetVersionId();
                            string path = App.GnollHackService.GetGnollHackPath();
                            App.GHVersionString = verstr;
                            App.GHVersionId = verid;
                            App.GHPath = path;
                            VersionLabel.Text = verid;
                            GnollHackLabel.Text = "GnollHack"; // + verstr;

                            Assembly assembly = GetType().GetTypeInfo().Assembly;
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.window_background.png"))
                            {
                                _background_bitmap = SKBitmap.Decode(stream);
                            }

                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.gnollhack-logo-test-2.png"))
                            {
                                _logo_bitmap = SKBitmap.Decode(stream);
                            }

                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.diablo_h.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.DiabloTypeface = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.uwch.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.UnderwoodTypeface = SKTypeface.FromStream(stream);
                                }
                            }

                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.endr.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.EndorTypeface = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.imrl.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.ImmortalTypeface = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.shxi.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.XizorTypeface = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.DejaVuSansMono.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.DejaVuSansMonoTypeface = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.DejaVuSansMono-Bold.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.DejaVuSansMonoBoldTypeface = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Lato-Regular.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.LatoRegular = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.Lato-Bold.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.LatoBold = SKTypeface.FromStream(stream);
                                }
                            }
                            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.archristy.ttf"))
                            {
                                if (stream != null)
                                {
                                    App.ARChristyTypeface = SKTypeface.FromStream(stream);
                                }
                            }

                            //firstButton.ImageSource = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            StartLocalGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            StartServerGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            clearImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            optionsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            settingsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            creditsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            exitImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            res = true;
                            break;
                        case 1:
                            App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);
                            res = true;
                            break;
                        case 2:
                            StartFadeIn();
                            res = true;
                            break;
                        default:
                            StartLogoImage.IsVisible = false;
                            FmodLogoImage.IsVisible = false;
                            break;
                    }
                    starttimercount++;
                    return res;
                });
            }
            else
            {
                videoView.Play();
                App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);
            }
        }
        private async void StartFadeLogoIn()
        {
            await StartLogoImage.FadeTo(1, 250);
            await FmodLogoImage.FadeTo(1, 250);
        }

        private async void StartFadeIn()
        {
            await StartLogoImage.FadeTo(0, 250);
            await FmodLogoImage.FadeTo(0, 250);
            UpperButtonGrid.IsVisible = true;
            await UpperButtonGrid.FadeTo(1, 250);
            videoView.IsVisible = true;
            await videoView.FadeTo(1, 250);
            videoView.Play();
            StartButtonGrid.IsVisible = true;
            await StartButtonGrid.FadeTo(1, 250);
            LogoGrid.IsVisible = true;
            await LogoGrid.FadeTo(1, 250);
        }

        private void ExitAppButton_Clicked(object sender, EventArgs e)
        {
            App.AppCloseService.CloseApplication();
            Thread.Sleep(50);
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }

        private async void ClearFilesButton_Clicked(object sender, EventArgs e)
        {
            bool answer = await DisplayAlert("Reset GnollHack?", "Are you sure to reset GnollHack?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearFiles();
                Preferences.Clear();
                App.GnollHackService.InitializeGnollHack();
                ClearFilesButton.Text = "Done";
                ClearFilesButton.TextColor = Color.Red;
            }
        }

        private async void SettingsButton_Clicked(object sender, EventArgs e)
        {
            var settingsPage = new SettingsPage(null);
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
        }

        private async void OptionsButton_Clicked(object sender, EventArgs e)
        {
            string fulltargetpath = Path.Combine(App.GHPath, "defaults.gnh");
            var editorPage = new EditorPage(fulltargetpath, "Default Options File");
            string errormsg = "";
            if (!editorPage.ReadFile(out errormsg))
            {
                ErrorLabel.Text = errormsg;
            }
            else
            {
                ErrorLabel.Text = "";
                await App.Current.MainPage.Navigation.PushModalAsync(editorPage);
            }
        }

        private async void CreditsButton_Clicked(object sender, EventArgs e)
        {
            string fulltargetpath = Path.Combine(App.GHPath, "credits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Credits");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                ErrorLabel.Text = errormsg;
            }
            else
            {
                ErrorLabel.Text = "";
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            videoView.Stop();
        }

        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;
                videoView.Stop();
                videoView.Source = null;
                if (Device.RuntimePlatform == Device.UWP)
                    videoView.Source = new Uri($"ms-appx:///Assets/splashvideo.mp4");
                else
                    videoView.Source = new Uri($"ms-appx:///splashvideo.mp4");
                videoView.WidthRequest = width;
                videoView.HeightRequest = height;
                videoView.Play();
            }
        }
    }
}

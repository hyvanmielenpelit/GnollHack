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
        private bool _canClickButton = true;
        private bool _serverButtonClicked = false;
        private NavigationPage _loginNavPage = null;
        public bool GameStarted { get; set; }

        public MainPage()
        {
            InitializeComponent();
        }

        public void HideLocalGameButton()
        {
            StartLocalGameButton.IsVisible = false;
            StartLocalGameImage.IsVisible = false;
            StartLocalGameButton.TextColor = Color.Gray;
        }
        private async void localButton_Clicked(object sender, EventArgs e)
        {
            StartServerGrid.IsEnabled = false;
            StartLocalGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
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

            App.PlayButtonClickedSound();
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
            ResetGrid.IsVisible = App.DeveloperMode;
            if (!App.DeveloperMode)
                wizardModeSwitch.IsToggled = false;

            if (_firsttime)
            {
                _firsttime = false;
                StartFadeLogoIn();
                Assembly thisassembly = GetType().GetTypeInfo().Assembly;
                FmodLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.FMOD-Logo-192-White.png", thisassembly);
                StartLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                MainLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                Device.StartTimer(TimeSpan.FromSeconds(1f / 4), () =>
                {
                    bool res = false;
                    switch (starttimercount)
                    {
                        case 0:
                            App.LoadServices();
                            App.IsModernAndroid = App.AppCloseService.IsModernAndroid();
                            string verstr = App.GnollHackService.GetVersionString();
                            string verid = App.GnollHackService.GetVersionId();
                            string path = App.GnollHackService.GetGnollHackPath();
                            App.GHVersionString = verstr;
                            App.GHVersionId = verid;
                            App.GHPath = path;
                            VersionLabel.Text = verid;
                            GnollHackLabel.Text = "GnollHack"; // + verstr;

                            Assembly assembly = GetType().GetTypeInfo().Assembly;
                            App.InitTypefaces(assembly);

                            //firstButton.ImageSource = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            StartLocalGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            StartServerGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            clearImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            topScoreImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            optionsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            settingsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            creditsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            exitImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                            StillImage.Source = ImageSource.FromResource("GnollHackClient.Assets.main-menu-portrait-snapshot.jpg", assembly);
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
            else if(!GameStarted && videoView.IsVisible == false)
            {
                PlayMainScreenVideoAndMusic();
            }

            StartServerGrid.IsEnabled = true;
            UpperButtonGrid.IsEnabled = true;
        }

        public void PlayMainScreenVideoAndMusic()
        {
            videoView.IsVisible = true;
            videoView.Play();
            StillImage.IsVisible = false;

            App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);

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
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            App.AppCloseService.CloseApplication();
            Thread.Sleep(50);
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }

        private async void ClearFilesButton_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
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
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            var settingsPage = new SettingsPage(null);
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
        }

        private async void OptionsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "defaults.gnh");
            var editorPage = new EditorPage(fulltargetpath, "Default Options File");
            string errormsg = "";
            if (!editorPage.ReadFile(out errormsg))
            {
                UpperButtonGrid.IsEnabled = true;
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
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "credits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Credits");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                UpperButtonGrid.IsEnabled = true;
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
            StillImage.IsVisible = true;
            videoView.Stop();
            videoView.IsVisible = false;
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
                if(width > height)
                {
                    if (Device.RuntimePlatform == Device.UWP)
                        videoView.Source = new Uri($"ms-appx:///Assets/mainmenulandscape.mp4");
                    else
                        videoView.Source = new Uri($"ms-appx:///mainmenulandscape.mp4");
                }
                else
                {
                    if (Device.RuntimePlatform == Device.UWP)
                        videoView.Source = new Uri($"ms-appx:///Assets/mainmenuportrait.mp4");
                    else
                        videoView.Source = new Uri($"ms-appx:///mainmenuportrait.mp4");
                }
                videoView.WidthRequest = width;
                videoView.HeightRequest = height;
                videoView.Play();
            }
        }

        private async void TopScoreButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "xlogfile");
            if(File.Exists(fulltargetpath))
            {
                var topScorePage = new TopScorePage(fulltargetpath);
                string errormsg = "";
                if (!topScorePage.ReadFile(out errormsg))
                {
                    UpperButtonGrid.IsEnabled = false;
                    ErrorLabel.Text = errormsg;
                }
                else
                {
                    ErrorLabel.Text = "";
                    await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
                }
            }
            else
            {
                /* No top scores */
                var topScorePage = new TopScorePage();
                await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
            }
        }
    }
}

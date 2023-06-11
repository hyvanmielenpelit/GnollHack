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
using System.Collections;
using System.Collections.Concurrent;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Plugin.StoreReview;

namespace GnollHackClient
{
    public partial class MainPage : ContentPage
    {
        private bool _canClickButton = true;
        private bool _serverButtonClicked = false;
        private Xamarin.Forms.NavigationPage _loginNavPage = null;
        public bool GameStarted { get; set; }

        public MainPage()
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
        }

        public void HideLocalGameButton()
        {
            StartLocalGameButton.IsVisible = false;
            //StartLocalGameImage.IsVisible = false;
            StartLocalGameButton.TextColor = Color.Gray;
            StartLocalGoneLabel.IsVisible = true;
        }

        public void ActivateLocalGameButton()
        {
            StartLocalGrid.IsEnabled = true;
            StartLocalGameButton.TextColor = Color.White;
            StartLocalGameButton.IsVisible = true;
            StartLocalGoneLabel.IsVisible = false;
        }

        private async void localButton_Clicked(object sender, EventArgs e)
        {
            StartServerGrid.IsEnabled = false;
            StartLocalGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            StartLocalGameButton.TextColor = Color.Gray;
            if (App.UsesCarousel)
                carouselView.Stop();

            long numberofgames = Preferences.Get("NumberOfGames", 0L);
            Preferences.Set("NumberOfGames", numberofgames + 1L);

            var gamePage = new GamePage(this);
            gamePage.EnableWizardMode = wizardModeSwitch.IsToggled;
            gamePage.EnableCasualMode = casualModeSwitch.IsToggled;
            gamePage.EnableModernMode = !classicModeSwitch.IsToggled;
            //gamePage.Disappearing += (sender2, e2) =>
            //{
            //    if (App.UsesCarousel)
            //        carouselView.Play();
            //};
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
            _loginNavPage = new Xamarin.Forms.NavigationPage(loginPage);
            
            await Navigation.PushAsync(_loginNavPage);
        }

        private bool _firsttime = true;
        private bool _mainScreenMusicStarted = false;

        public void UpdateLayout()
        {
            wizardModeGrid.IsVisible = App.DeveloperMode;
            if (!App.DeveloperMode)
                wizardModeSwitch.IsToggled = false;
            ResetButton.IsVisible = App.DeveloperMode;
            OptionsButton.IsVisible = App.DeveloperMode;
            StartServerGrid.IsVisible = App.ServerGameAvailable;

            classicModeSwitch.IsToggled = App.ClassicMode;
            casualModeSwitch.IsToggled = App.CasualMode;

            UpdateMobileVersionLabel();
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            UpdateLayout();
            if (_firsttime)
            {
                App.DebugWriteProfilingStopwatchTimeAndRestart("MainPage First Time");
                _firsttime = false;
                Assembly thisassembly = GetType().GetTypeInfo().Assembly;
                FmodLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.FMOD-Logo-192-White.png", thisassembly);
                StartLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                MainLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                if (App.PlatformService != null)
                {
                    bool removeanimationson = App.PlatformService.IsRemoveAnimationsOn();
                    if(removeanimationson)
                    {
                        await DisplayAlert("Invalid Animator Duration Scale",
                            "GnollHack has detected invalid animator settings. If your device has a setting named \"Remove Animations\" under Settings -> Accessibility -> Visibility Enhancements, this setting must be set to Off. If your device does not have this setting, please manually adjust the value of \"Animator duration scale\" to 1x under Settings -> Developer Options -> Animator duration scale. ", "OK");
                        CloseApp();
                    }
                    else
                    {
                        float scalesetting = App.PlatformService.GetAnimatorDurationScaleSetting();
                        float scalecurrent = App.PlatformService.GetCurrentAnimatorDurationScale();
                        if (scalecurrent == 0.0f)
                        {
                            if (scalesetting == 0.0f)
                                await DisplayAlert("Invalid Animator Duration Scale",
                                    "GnollHack failed to automatically adjust Animator Duration Scale and it remains set to Off. Please manually adjust the value to 1x under Settings -> Developer Options -> Animator duration scale. If your device has a setting named \"Remove Animations\" under Settings -> Accessibility -> Visibility Enhancements, this setting needs to be disabled, too.", "OK");
                            else
                                await DisplayAlert("Invalid Animator Duration Scale",
                                    "GnollHack failed to automatically adjust Animator Duration Scale and it has become turned Off. Please check that the value is 1x under Settings -> Developer Options -> Animator duration scale. If your device has a setting named \"Remove Animations\" under Settings -> Accessibility -> Visibility Enhancements, this setting needs to be disabled, too.", "OK");
                            CloseApp();
                        }
                        else if (scalecurrent == -1.0f)
                        {
                            /* GnollHack could determine current animator duration scale */
                        }
                    }
                }

                await StartFadeLogoIn();
                await StartUpTasks();
                await StartFadeIn();
                StartLogoImage.IsVisible = false;
                FmodLogoImage.IsVisible = false;

                if (App.InformAboutGameTermination)
                {
                    App.InformAboutGameTermination = false;
                    await DisplayAlert("Unexpected Game Termination", "GnollHack was unexpectedly terminated when running on background. This may have been instructed by the operating system or the user. Your game may have been saved before the termination.", "OK");
                }
            }
            else if (!GameStarted)
            {
                if (/*(!App.UsesCarousel && !videoView.IsVisible) ||*/ (App.UsesCarousel && !_mainScreenMusicStarted))
                    PlayMainScreenVideoAndMusic();
            }

            StartServerGrid.IsEnabled = true;
            UpperButtonGrid.IsEnabled = true;
            LogoGrid.IsEnabled = true;
        }
        public async Task InitializeServices()
        {
            bool resetFiles = Preferences.Get("ResetAtStart", true);
            if (resetFiles)
            {
                App.GnollHackService.ClearFiles();
                Preferences.Set("ResetAtStart", false);
                Preferences.Set("ResetExternalFiles", true);
            }
            App.ResetAcquiredFiles();

            await TryInitializeGnollHack();
            await TryInitializeFMOD();

        }

        public async Task TryReadSecrets()
        {
            try
            {
                App.ReadSecrets();
                Array.Sort<SecretsFile>(App.CurrentSecrets.files, new SecretsFileSizeComparer());

            }
            catch (Exception ex)
            {
                await DisplayAlert("Reading Secrets File Failed", "GnollHack failed to read secrets file: " + ex.Message, "OK");
            }
        }

        public async Task TryInitializeGnollHack()
        {
            try
            {
                App.GnollHackService.InitializeGnollHack(App.CurrentSecrets);
            }
            catch (Exception ex)
            {
                await DisplayAlert("GnollHack Initialization Failed", "Initializing GnollHack failed: " + ex.Message, "OK");
            }
        }

        public async Task TryInitializeFMOD()
        {
            try
            {
                App.FmodService.InitializeFmod();
            }
            catch (Exception ex)
            {
                await DisplayAlert("FMOD Initialization Failed", "Initializing FMOD failed: " + ex.Message, "OK");
            }
        }

        public async Task TryGetFilesFromResources()
        {
            try
            {
                GetFilesFromResources();
            }
            catch (Exception ex)
            {
                await DisplayAlert("File Unpacking Failure", "GnollHack failed to unpack core files: " + ex.Message, "OK");
            }
        }

        public async Task TryClearCoreFiles()
        {
            try
            {
                App.GnollHackService.ClearCoreFiles();
            }
            catch (Exception ex)
            {
                await DisplayAlert("File Clearing Failure", "GnollHack failed to clear core files: " + ex.Message, "OK");
            }
        }

        public async Task TryLoadBanks(int subType)
        {
            try
            {
                App.FmodService.LoadBanks(subType);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Loading FMOD Banks Failed", "GnollHack failed to load FMOD banks of type " + subType + ": " + ex.Message, "OK");
            }
        }

        private async Task StartUpTasks()
        {
            await TryReadSecrets();
            await InitializeServices();

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            App.InitAdditionalTypefaces(assembly);
            App.InitSymbolBitmaps(assembly);
            App.InitGameBitmaps(assembly);
            carouselView.Init();

            string verstr = "?";
            string verid = "?";
            string path = ".";
            string fmodverstr = "?";
            string skiaverstr = "?";
            string skiasharpverstr = "?";
            ulong vernum = 0UL;
            try
            {
                verstr = App.GnollHackService.GetVersionString();
                verid = App.GnollHackService.GetVersionId();
                vernum = App.GnollHackService.GetVersionNumber();
                path = App.GnollHackService.GetGnollHackPath();
                fmodverstr = App.FmodService.GetVersionString();
                skiaverstr = SkiaSharpVersion.Native.ToString();
                Assembly skiaSharpAssem = typeof(SkiaSharpVersion).Assembly;
                AssemblyName skiaSharpAssemName = skiaSharpAssem.GetName();
                Version ver = skiaSharpAssemName.Version;
                skiasharpverstr = ver.Major + "." + ver.Minor;
                var attr = skiaSharpAssem
                    .GetCustomAttributes(typeof(AssemblyInformationalVersionAttribute), false)
                    as AssemblyInformationalVersionAttribute[];
                if (attr != null && attr.Length > 0)
                {
                    string fullverid = attr[0].InformationalVersion;
                    if (!string.IsNullOrWhiteSpace(fullverid))
                    {
                        int dashpos = fullverid.IndexOf("-");
                        if (dashpos >= 0)
                            skiasharpverstr = fullverid.Substring(0, dashpos);
                        else
                            skiasharpverstr = fullverid;
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            App.GHVersionString = verstr;
            App.GHVersionNumber = vernum;
            App.GHVersionId = verid;
            App.GHPath = path;
            App.FMODVersionString = fmodverstr;
            App.SkiaVersionString = skiaverstr;
            App.SkiaSharpVersionString = skiasharpverstr;

            VersionLabel.Text = verid;
            GnollHackLabel.Text = "GnollHack";

            string prev_version = Preferences.Get("VersionId", "");
            if(prev_version != verid)
            {
                await TryClearCoreFiles();
                await TryInitializeGnollHack();
            }
            Preferences.Set("VersionId", verid);

            try
            {
                //StartLocalGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //StartServerGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //clearImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //topScoreImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //optionsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //settingsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //creditsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                //exitImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
                StillImage.Source = ImageSource.FromResource("GnollHackClient.Assets.main-menu-portrait-snapshot.jpg", assembly);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            if (!App.IsiOS)
            {
                /* Download or copy files to a subdirectory */
                await TryGetFilesFromResources();
                await DownloadAndCheckFiles();
            }
            else
            {
                /* Access banks directly from an assets directory or the like */
                AddLoadableSoundBanksFromAssets();
                DeleteBanksFromDisk();
            }

            if (App.LoadBanks)
            {
                await TryLoadBanks(0);
                await TryLoadBanks(2);
            }

            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume;
            generalVolume = Preferences.Get("GeneralVolume", GHConstants.DefaultGeneralVolume);
            musicVolume = Preferences.Get("MusicVolume", GHConstants.DefaultMusicVolume);
            ambientVolume = Preferences.Get("AmbientVolume", GHConstants.DefaultAmbientVolume);
            dialogueVolume = Preferences.Get("DialogueVolume", GHConstants.DefaultDialogueVolume);
            effectsVolume = Preferences.Get("EffectsVolume", GHConstants.DefaultEffectsVolume);
            UIVolume = Preferences.Get("UIVolume", GHConstants.DefaultUIVolume);
            try
            {
                App.FmodService.AdjustVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume);
                if (App.LoadBanks)
                    App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);

                /* Check silent mode; this also mutes everything if need be */
                App.SilentMode = Preferences.Get("SilentMode", false);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private void AddLoadableSoundBanksFromAssets()
        {
            foreach (SecretsFile sf in App.CurrentSecrets.files)
            {
                if (sf.type == "sound_bank")
                {
                    string sdir = Path.Combine(App.PlatformService.GetAssetsPath(), sf.source_directory);
                    string sfile = Path.Combine(sdir, sf.name);

                    //string rfile = Path.Combine(sf.source_directory, sf.name);
                    //if (readtomemoryfromresources)
                    //    App.FmodService.AddLoadableSoundBank(rfile, sf.subtype_id, true, true);
                    //else

                    App.FmodService.AddLoadableSoundBank(sfile, sf.subtype_id, true, false);
                }
            }
        }

        private void DeleteBanksFromDisk()
        {
            string ghdir = App.GHPath;
            List<string> target_directories = new List<string>();
            foreach (SecretsFile sf in App.CurrentSecrets.files)
            {
                if (sf.type == "sound_bank")
                {
                    try
                    {
                        string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory); ;
                        string sfile = Path.Combine(sdir, sf.name);
                        if (File.Exists(sfile))
                            File.Delete(sfile);

                        bool containsstring = false;
                        for(int i = 0; i < target_directories.Count; i++)
                            if (target_directories[i] == sdir)
                            {
                                containsstring = true;
                                break;
                            }

                        if(!containsstring)
                            target_directories.Add(sdir);
                    }
                    catch (Exception ex) 
                    {
                        Debug.WriteLine(ex.Message);
                    }
                }
            }
            foreach(string dir in target_directories)
            {
                try
                {
                    if (Directory.Exists(dir))
                    {
                        Directory.Delete(dir, true);
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }
        }

        private object _abortLock = new object();
        private bool _abortDisplayed = false;

        private async void UpdateDownloadProgress()
        {
            bool dl;
            bool ver;
            float percentage;
            long bytesDownloaded;
            long? totalFileSize;
            string fileDescription;
            long desiredFileSize;
            string verifyFileDescription;
            lock (_downloadProgressLock)
            {
                dl = _downloading;
                ver = _verifying;
                percentage = _downloadProgressPercentage;
                bytesDownloaded = _downloadProgressTotalBytesDownloaded;
                totalFileSize = _downloadProgressTotalFileSize;
                fileDescription = _downloadProgressFileDescription;
                desiredFileSize = _downloadProgressDesiredFileSize;
                verifyFileDescription = _verifyingFileDescription;
            }

            if (!dl && !ver)
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    DownloadGrid.IsVisible = false;
                });
                return;
            }

            Device.BeginInvokeOnMainThread(() =>
            {
                if (dl && ver)
                {
                    DownloadTitleLabel.Text = "Downloading; Verifying...";
                    DownloadFileNameLabel.Text = fileDescription + "; " + verifyFileDescription;
                }
                else if(dl)
                {
                    DownloadTitleLabel.Text = "Downloading...";
                    DownloadFileNameLabel.Text = fileDescription;
                }
                else
                {
                    DownloadTitleLabel.Text = "Verifying...";
                    DownloadFileNameLabel.Text = verifyFileDescription;
                }

                if (totalFileSize <= 0 || !dl)
                    DownloadBytesLabel.Text = "Please wait...";
                else
                    DownloadBytesLabel.Text = string.Format("{0:0.00}", (double)bytesDownloaded / 1000000) + " / " + string.Format("{0:0.00}", (double)totalFileSize / 1000000) + " MB";

                DownloadGrid.IsVisible = true;
            });
            await DownloadProgressBar.ProgressTo((float)percentage / 100, 100, Easing.Linear);

            if (dl && totalFileSize != null && totalFileSize > 0 && desiredFileSize > 0 && totalFileSize != desiredFileSize && _cancellationTokenSource.Token.CanBeCanceled)
            {
                _cancellationTokenSource.Cancel();
                bool displayAbort = false;
                lock(_abortLock)
                {
                    if (!_abortDisplayed)
                    {
                        _abortDisplayed = true;
                        displayAbort = true;
                    }
                }
                if(displayAbort)
                    await DisplayAlert("Downloadable File Invalid", "The specified download file does not match the required file description for " + fileDescription + ". Aborting.", "OK");
                ;
            }
        }

        public void PlayMainScreenVideoAndMusic()
        {
            /*
            if (!App.UsesCarousel)
            {
                videoView.IsVisible = true;
                videoView.Play();
                StillImage.IsVisible = false;
            }
            else // if (App.IsiOS)
            */
            {
                carouselView.IsVisible = true;
                carouselView.Play();
                StillImage.IsVisible = false;
            }

            try
            {
                App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);
                _mainScreenMusicStarted = true;
            }
            catch(Exception ex)
            {
                Debug.WriteLine("Playing music failed: " + ex.Message);
            }
        }

        private async Task StartFadeLogoIn()
        {
            await StartLogoImage.FadeTo(1, 250);
            StartLogoImage.Opacity = 1.0; /* To make sure */
            await FmodLogoImage.FadeTo(1, 250);
            FmodLogoImage.Opacity = 1.0; /* To make sure */
            //List<Task> tasklist = new List<Task> { t1, t2 };
            //await Task.WhenAll(tasklist);
        }

        private async Task StartFadeIn()
        {
            await StartLogoImage.FadeTo(0, 250);
            StartLogoImage.Opacity = 0.0; /* To make sure */
            await FmodLogoImage.FadeTo(0, 250);
            FmodLogoImage.Opacity = 0.0; /* To make sure */
            //Task[] tasklist1 = new Task[2] { t1, t2 };
            //Task.WaitAll(tasklist1);

            /*
            if (!App.UsesCarousel)
            {
                videoView.IsVisible = true;
                await videoView.FadeTo(1, 250);
                videoView.Play();
            }
            else
            */
            {
                //StillImage.IsVisible = true;
                //await StillImage.FadeTo(1, 250);
                carouselView.IsVisible = true;
                carouselView.InvalidateSurface();
                await carouselView.FadeTo(1, 250);
                carouselView.Opacity = 1.0; /* To make sure */
                carouselView.Play();
            }

            UpperButtonGrid.IsVisible = true;
            await UpperButtonGrid.FadeTo(1, 250);
            UpperButtonGrid.Opacity = 1.0;  /* To make sure */
            StartButtonLayout.IsVisible = true;
            await StartButtonLayout.FadeTo(1, 250);
            StartButtonLayout.Opacity = 1.0;  /* To make sure */
            LogoGrid.IsVisible = true;
            await LogoGrid.FadeTo(1, 250);
            LogoGrid.Opacity = 1.0;  /* To make sure */

            //List<Task> tasklist2 = new List<Task> { t3, t4, t5 };
            //await Task.WhenAll(tasklist2);
        }

        private async void ExitAppButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            bool hideautoupdatealert = Preferences.Get("HideAutoUpdateAlert", false);
            bool isfromgoogleplay = true;
            if(!hideautoupdatealert)
            {
                _popupStyle = popup_style.DisableAutoUpdate;
                PopupCheckBoxLayout.IsVisible = true;
                PopupTitleLabel.TextColor = Color.Red;
                PopupTitleLabel.Text = "Auto-Update Warning";
                if (App.IsAndroid && isfromgoogleplay)
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you turn off Auto-Update from Google Play Store for GnollHack and manually apply updates when you have no saved games.";
                else if (App.IsiOS)
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates by toggling off App Updates under App Store section in the Settings app, and manually apply updates when you have no saved games.";
                else
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates in your device settings and manually apply updates when you have no saved games.";
                PopupGrid.IsVisible = true;
            }
            else
            {
                bool ReviewRequested = Preferences.Get("StoreReviewRequested", false);
                long NumberOfGames = Preferences.Get("NumberOfGames", 0L);
                long TotalPlayTime = Preferences.Get("RealPlayTime", 0L);
                if (!ReviewRequested && NumberOfGames >= GHConstants.StoreReviewRequestNumberOfGames && TotalPlayTime >= GHConstants.StoreReviewRequestTotalPlayTime)
                {
                    Preferences.Set("StoreReviewRequested", true);
                    await CrossStoreReview.Current.RequestReview(false);
                }
                CloseApp();
            }
        }

        private void CloseApp()
        {
            App.PlatformService.CloseApplication();
            Thread.Sleep(50);
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }

        private async void ClearFilesButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (App.UsesCarousel)
                carouselView.Stop();
            var resetPage = new ResetPage();
            resetPage.Disappearing += (sender2, e2) =>
            {
                if (App.UsesCarousel)
                    carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(resetPage);
            UpperButtonGrid.IsEnabled = true;
        }

        private async void SettingsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (App.UsesCarousel)
                carouselView.Stop();
            var settingsPage = new SettingsPage(null, this);
            settingsPage.Disappearing += (sender2, e2) =>
            {
                //if (App.UsesCarousel)
                //    carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
            UpperButtonGrid.IsEnabled = true;
        }

        private async void OptionsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (App.UsesCarousel)
                carouselView.Stop();
            string fulltargetpath = Path.Combine(App.GHPath, "defaults.gnh");
            var editorPage = new EditorPage(fulltargetpath, "Default Options File");
            string errormsg = "";
            if (!editorPage.ReadFile(out errormsg))
            {
                ErrorLabel.Text = errormsg;
                if (App.UsesCarousel)
                    carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            }
            else
            {
                ErrorLabel.Text = "";
                editorPage.Disappearing += (sender2, e2) =>
                {
                    if (App.UsesCarousel)
                        carouselView.Play();
                    UpperButtonGrid.IsEnabled = true;
                };
                await App.Current.MainPage.Navigation.PushModalAsync(editorPage);
            }
            UpperButtonGrid.IsEnabled = true;
        }

        private async void CreditsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (App.UsesCarousel)
                carouselView.Stop();
            var aboutPage = new AboutPage();
            aboutPage.Disappearing += (sender2, e2) =>
            {
                if (App.UsesCarousel)
                    carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(aboutPage);
            UpperButtonGrid.IsEnabled = true;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            /*
            if (!App.UsesCarousel)
            {
                StillImage.IsVisible = true;
                videoView.Stop();
                videoView.IsVisible = false;
            }
            else
            */
            {
                carouselView.Stop();
            }
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
                UpdateMainScreenBackgroundElement(true);
            }
        }

        public void UpdateMainScreenBackgroundStyle()
        {
            /* Change the situation as it would have been from the start */
            if (App.UsesCarousel)
            {
                //videoView.Stop();
                //videoView.IsVisible = false;
                StillImage.IsVisible = false;
                carouselView.IsVisible = true;
            }
            else
            {
                carouselView.Stop();
                carouselView.IsVisible = false;
                StillImage.IsVisible = true;
                UpdateMainScreenBackgroundElement(false);
            }
        }

        public void UpdateMainScreenBackgroundElement(bool playAfterUpdate)
        {
            /*
            if (!App.UsesCarousel)
            {
                videoView.Stop();
                videoView.Source = null;
                if (_currentPageWidth > _currentPageHeight)
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
                videoView.WidthRequest = _currentPageWidth;
                videoView.HeightRequest = _currentPageHeight;
                if(playAfterUpdate)
                    videoView.Play();
            }
            else
            */
            {
                carouselView.InvalidateSurface();
            }
        }

        private async void TopScoreButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (App.UsesCarousel)
                carouselView.Stop();
            string fulltargetpath = Path.Combine(App.GHPath, "xlogfile");
            if(File.Exists(fulltargetpath))
            {
                var topScorePage = new TopScorePage(fulltargetpath);
                string errormsg = "";
                if (!topScorePage.ReadFile(out errormsg))
                {
                    ErrorLabel.Text = errormsg;
                    if (App.UsesCarousel)
                        carouselView.Play();
                    UpperButtonGrid.IsEnabled = true;
                }
                else
                {
                    ErrorLabel.Text = "";
                    topScorePage.Disappearing += (sender2, e2) =>
                    {
                        if (App.UsesCarousel)
                            carouselView.Play();
                        UpperButtonGrid.IsEnabled = true;
                    };
                    await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
                }
            }
            else
            {
                /* No top scores */
                var topScorePage = new TopScorePage();
                topScorePage.Disappearing += (sender2, e2) =>
                {
                    if (App.UsesCarousel)
                        carouselView.Play();
                    UpperButtonGrid.IsEnabled = true;
                };
                await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
            }
            UpperButtonGrid.IsEnabled = true;
        }

        public void PlayCarouselView()
        {
            carouselView.Play();
        }

        private void GetFilesFromResources()
        {
            string ghdir = App.GnollHackService.GetGnollHackPath();
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            foreach (SecretsFile f in App.CurrentSecrets.files)
            {
                try
                {
                    string sdir = string.IsNullOrWhiteSpace(f.target_directory) ? ghdir : Path.Combine(ghdir, f.target_directory);
                    string sfile = Path.Combine(sdir, f.name);
                    if(File.Exists(sfile))
                    {
                        FileInfo file = new FileInfo(sfile);
                        DateTime moddate = Preferences.Get("Verify_" + f.id + "_LastWriteTime", DateTime.MinValue);
                        string version = Preferences.Get("Verify_" + f.id + "_Version", "");
                        if (version != f.version || moddate != file.LastWriteTimeUtc || file.Length != f.length)
                        {
                            File.Delete(sfile);
                            if (Preferences.ContainsKey("Verify_" + f.id + "_Version"))
                                Preferences.Remove("Verify_" + f.id + "_Version");
                            if (Preferences.ContainsKey("Verify_" + f.id + "_LastWriteTime"))
                                Preferences.Remove("Verify_" + f.id + "_LastWriteTime");
                        }
                    }

                    if (!File.Exists(sfile))
                    {
                        using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets." + f.source_directory + "." + f.name))
                        {
                            if(stream != null)
                            {
                                using (var fileStream = File.Create(sfile))
                                {
                                    stream.CopyTo(fileStream);
                                }
                                FileInfo newfileinfo = new FileInfo(sfile);
                                Preferences.Set("Verify_" + f.id + "_LastWriteTime", newfileinfo.LastWriteTimeUtc);
                                Preferences.Set("Verify_" + f.id + "_Version", f.version);
                            }
                        }
                    }
                }
                catch
                {
                    //Probably did not exist
                }
            }

        }

        private object _downloadProgressLock = new object();
        private float _downloadProgressPercentage = 0.0f;
        private long _downloadProgressTotalBytesDownloaded = 0;
        private long? _downloadProgressTotalFileSize = 0;
        private string _downloadProgressFileDescription = "";
        private string _downloadProgressFilePath = "";
        private long _downloadProgressDesiredFileSize = 0;
        private bool _downloading = false;
        private bool _verifying = false;
        private string _verifyingFileDescription = "";
        private ConcurrentDictionary<SecretsFile, downloaded_file_check_results> _downloadResultConcurrentDictionary = new ConcurrentDictionary<SecretsFile, downloaded_file_check_results>();

        private async Task DownloadAndCheckFiles()
        {
            string ghdir = App.GnollHackService.GetGnollHackPath();
            Assembly assembly = GetType().GetTypeInfo().Assembly;

            foreach(SecretsDirectory sd in App.CurrentSecrets.directories)
            {
                if (string.IsNullOrWhiteSpace(sd.name))
                    continue;

                string sdir = Path.Combine(ghdir, sd.name);

                /* Make the relevant directory */
                if (!Directory.Exists(sdir))
                {
                    Directory.CreateDirectory(sdir);
                }
            }

            App.DebugWriteProfilingStopwatchTimeAndStart("Start Acquiring Banks");
            App.FmodService.ClearLoadableSoundBanks();
            SecretsFile prev_sf = null;
            _downloadResultConcurrentDictionary.Clear();
            foreach (SecretsFile sf in App.CurrentSecrets.files)
            {
                Task<downloaded_file_check_results> dltask = null;
                Task restask = null;
                Task checktask = null;

                // sf.source == "url" is deprecated
                dltask = DownloadFileFromWebServer(assembly, sf, ghdir);

                if(prev_sf != null)
                {
                    downloaded_file_check_results dres = downloaded_file_check_results.OK;
                    bool dfound = _downloadResultConcurrentDictionary.TryGetValue(prev_sf, out dres);
                    if (dfound && dres == downloaded_file_check_results.Cancelled)
                        checktask = DisplayAlert("File Download Cancelled", "Download of " + prev_sf.description + " was cancelled.", "OK");
                    else
                        checktask = CheckSecretsFile(prev_sf);
                }

                List<Task> tasks = new List<Task>();
                if(dltask != null)
                    tasks.Add(dltask);
                if (restask != null)
                    tasks.Add(restask);
                if (checktask != null)
                    tasks.Add(checktask);
                await Task.WhenAll(tasks);

                if (sf.type == "sound_bank")
                {
                    string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory); ;
                    string sfile = Path.Combine(sdir, sf.name);
                    App.FmodService.AddLoadableSoundBank(sfile, sf.subtype_id, false, false);
                }

                if (sf == App.CurrentSecrets.files.Last<SecretsFile>())
                {
                    await CheckSecretsFile(sf);
                }
                prev_sf = sf;
            }
        }

        private async Task CheckSecretsFile(SecretsFile sf)
        {
            string ghdir = App.GnollHackService.GetGnollHackPath();
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            downloaded_file_check_results res = await CheckDownloadedFile(assembly, sf, ghdir);
            if (res != downloaded_file_check_results.OK)
            {
                bool diddelete = false;
                if (res == downloaded_file_check_results.VerificationFailed)
                {
                    string target_path = Path.Combine(ghdir, sf.target_directory, sf.name);
                    if (File.Exists(target_path))
                    {
                        FileInfo file = new FileInfo(target_path);
                        file.Delete();
                        diddelete = true;
                    }
                }
                await DisplayAlert("File Verification Failure", "File verification of " + sf.description + " failed with error code " + (int)res + "." + (diddelete ? " Deleting the file." : ""), "OK");
            }
        }

        //public async Task<downloaded_file_check_results> DownloadFileFromResources(Assembly assembly, SecretsFile f, string ghdir)
        //{
        //    if (f == null)
        //        return downloaded_file_check_results.NoSecretsFile;

        //    downloaded_file_check_results dres = await VerifySecretsFile(assembly, f, ghdir);
        //    if (dres == downloaded_file_check_results.OK)
        //    {
        //        _downloadResultConcurrentDictionary.TryAdd(f, dres);
        //        return dres;
        //    }

        //    string sdir = string.IsNullOrWhiteSpace(f.target_directory) ? ghdir : Path.Combine(ghdir, f.target_directory);
        //    string sfile = Path.Combine(sdir, f.name);

        //    if (!File.Exists(sfile))
        //    {
        //        using (Stream stream = assembly.GetManifestResourceStream(f.source_url))
        //        {
        //            using (var fileStream = File.Create(sfile))
        //            {
        //                stream.CopyTo(fileStream);
        //            }
        //        }
        //        dres = downloaded_file_check_results.OK;
        //        _downloadResultConcurrentDictionary.TryAdd(f, dres);
        //        return dres;
        //    }
        //    else
        //    {
        //        dres = downloaded_file_check_results.FileAlreadyExists;
        //        _downloadResultConcurrentDictionary.TryAdd(f, dres);
        //        return dres;
        //    }
        //}

        private void CancelDownloadButton_Clicked(object sender, EventArgs e)
        {
            _cancellationTokenSource.Cancel();
        }

        //private static HttpClient _httpClient = new HttpClient();
        CancellationTokenSource _cancellationTokenSource = new CancellationTokenSource();

        public async Task<downloaded_file_check_results> DownloadFileFromWebServer(Assembly assembly, SecretsFile f, string ghdir)
        {
            if (f == null)
                return downloaded_file_check_results.NoSecretsFile;

            downloaded_file_check_results dres = await VerifySecretsFile(assembly, f, ghdir);
            if (dres == downloaded_file_check_results.OK)
            {
                _downloadResultConcurrentDictionary.TryAdd(f, dres);
                return dres;
            }

            /* Set version to be downloaded, and download */
            Preferences.Set("Verify_" + f.id + "_Version", f.version);
            Device.BeginInvokeOnMainThread(() =>
            {
                DownloadGrid.IsVisible = true;
            });

            string url = f.source_url;
            string target_path = Path.Combine(ghdir, f.target_directory, f.name);
            lock (_downloadProgressLock)
            {
                _downloading = false;
                _downloadProgressPercentage = 0.0f;
                _downloadProgressTotalBytesDownloaded = 0;
                _downloadProgressTotalFileSize = 0;
                _downloadProgressFileDescription = f.description;
                _downloadProgressFilePath = target_path;
                _downloadProgressDesiredFileSize = f.length;
            }
            lock (_abortLock)
            {
                _abortDisplayed = false;
            }

            App.DebugWriteProfilingStopwatchTimeAndStart("Begin Http Download " + f.name);
            using (var client = new HttpClientDownloadWithProgress(url, target_path, _cancellationTokenSource))
            {
                client.ProgressChanged += (totalFileSize, totalBytesDownloaded, progressPercentage) => {
                    lock(_downloadProgressLock)
                    {
                        _downloadProgressPercentage = (float)progressPercentage;
                        _downloadProgressTotalBytesDownloaded = totalBytesDownloaded;
                        _downloadProgressTotalFileSize = totalFileSize;
                    }
                    UpdateDownloadProgress();
                };

                try
                {
                    lock(_downloadProgressLock)
                    {
                        _downloading = true;
                    }
                    await client.StartDownload();
                }
                catch(OperationCanceledException)
                {
                    lock (_downloadProgressLock)
                    {
                        _downloading = false;
                    }
                    /* Download was cancelled */
                }
                catch (Exception e)
                {
                    lock (_downloadProgressLock)
                    {
                        _downloading = false;
                    }
                    if (!_cancellationTokenSource.Token.IsCancellationRequested)
                        await DisplayAlert("Download Error", "A download error occurred: " + e.Message, "OK");
                }
            }

            lock (_downloadProgressLock)
            {
                _downloading = false;
            }

            /* Finished download */
            UpdateDownloadProgress();

            downloaded_file_check_results res = downloaded_file_check_results.OK;
            if (_cancellationTokenSource.Token.IsCancellationRequested)
            {
                /* Delete cancelled files */
                if (File.Exists(_downloadProgressFilePath))
                {
                    FileInfo file = new FileInfo(_downloadProgressFilePath);
                    file.Delete();
                }
                res = downloaded_file_check_results.Cancelled;
            }

            _cancellationTokenSource.Dispose();
            _cancellationTokenSource = new CancellationTokenSource();

            _downloadResultConcurrentDictionary.TryAdd(f, res);
            return res;
        }

        public async Task<downloaded_file_check_results> VerifySecretsFile(Assembly assembly, SecretsFile f, string ghdir)
        {
            if (f == null)
                return downloaded_file_check_results.NoSecretsFile;

            /* Check if ok */
            string target_path = Path.Combine(ghdir, f.target_directory, f.name);
            bool correctversion = true;
            string curfileversion = Preferences.Get("Verify_" + f.id + "_Version", "");
            if (curfileversion != "" && curfileversion != f.version)
                correctversion = false;

            if (File.Exists(target_path))
            {
                downloaded_file_check_results dres = downloaded_file_check_results.FileNotInSecretsList;
                FileInfo file = new FileInfo(target_path);
                if (correctversion)
                {
                    dres = await CheckDownloadedFile(assembly, f, ghdir);
                    if (dres == downloaded_file_check_results.OK)
                        return dres; /* OK, no need to download */
                }
                /* Wrong, delete and download */
                file.Delete();
            }
            return downloaded_file_check_results.FileDoesNotExist;
        }

        public async Task<downloaded_file_check_results> CheckDownloadedFile(Assembly assembly, SecretsFile f, string ghdir)
        {
            if (f == null)
                return downloaded_file_check_results.NoSecretsFile;

            string target_directory = f.target_directory;
            string target_path = Path.Combine(ghdir, target_directory, f.name);

            /* Check if ok */
            if (File.Exists(target_path))
            {
                FileInfo file = new FileInfo(target_path);
                bool isfileok = true;
                if (file.Length != f.length)
                    isfileok = false;
                if (isfileok)
                {
                    DateTime moddate = Preferences.Get("Verify_" + f.id + "_LastWriteTime", DateTime.MinValue);
                    if (moddate != file.LastWriteTimeUtc)
                    {
                        isfileok = await VerifyDownloadedFile(target_path, f.sha256, f.description);
                        if (isfileok)
                            Preferences.Set("Verify_" + f.id + "_LastWriteTime", file.LastWriteTimeUtc);
                    }
                }

                if (isfileok)
                    return downloaded_file_check_results.OK;
                else
                    return downloaded_file_check_results.VerificationFailed;
            }
            else
                return downloaded_file_check_results.FileDoesNotExist;
        }

        private object checksumlock = new object();
        string _checksum;
        public async Task<bool> VerifyDownloadedFile(string target_path, string sha256, string desc)
        {
            App.DebugWriteProfilingStopwatchTimeAndStart("Begin Checksum");

            lock (_downloadProgressLock)
            {
                _verifying = true;
                _verifyingFileDescription = desc;
            }
            UpdateDownloadProgress();

            await Task.Run(() => {
                string checksum = ChecksumUtil.GetChecksum(HashingAlgoTypes.SHA256, target_path);
                lock (checksumlock)
                {
                    _checksum = checksum;
                };
            });

            lock (_downloadProgressLock)
            {
                _verifying = false;
                _verifyingFileDescription = "";
            }
            UpdateDownloadProgress();

            App.DebugWriteProfilingStopwatchTimeAndStart("Finish Checksum");
            string chksum = "";
            lock (checksumlock)
            {
                chksum = _checksum;
            };
            return chksum.ToLower() == sha256.ToLower();
        }

        private void UpdateMobileVersionLabel()
        {
            MobileVersionLabel.Text = Device.RuntimePlatform + " Version";
        }

        private void ClassicModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            App.ClassicMode = classicModeSwitch.IsToggled;
            Preferences.Set("ClassicMode", App.ClassicMode);
            if(classicModeSwitch.IsToggled)
            {

            }
            else
            {
                //wizardModeSwitch.IsToggled = false;
            }
        }

        private void CasualModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            App.CasualMode = casualModeSwitch.IsToggled;
            Preferences.Set("CasualMode", App.CasualMode);
            if (casualModeSwitch.IsToggled)
            {
                //wizardModeSwitch.IsToggled = false;
            }
            else
            {

            }
        }

        private enum popup_style
        {
            GeneralDialog = 0,
            DisableAutoUpdate
        }

        private popup_style _popupStyle = popup_style.GeneralDialog;
        private async void PopupOkButton_Clicked(object sender, EventArgs e)
        {
            if(_popupStyle == popup_style.DisableAutoUpdate)
            {
                if (PopupNoAgainCheckBox.IsChecked)
                {
                    Preferences.Set("HideAutoUpdateAlert", true);
                    await Task.Delay(50);
                }

                CloseApp();
            }
            PopupGrid.IsVisible = false;
        }

        private void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = ClientUtils.NHColor2XColor((int)nhcolor.NO_COLOR, 0, false, true);
            PopupTitleLabel.Text = "Classic Mode";
            PopupLabel.Text = "In Classic Mode, a character's death is permanent. The resulting score is listed in top scores. A dead character may arise as a ghost.\n\nIn Modern Mode, your god will revive you at the starting altar, or at another special location. Each such revival will reduce your game score, which will be recorded in top scores upon quitting or winning the game.";
            PopupGrid.IsVisible = true;
        }

        private void CasualTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = ClientUtils.NHColor2XColor((int)nhcolor.NO_COLOR, 0, false, true);
            PopupTitleLabel.Text = "Casual Mode";
            PopupLabel.Text = "Casual Mode is a non-scoring game mode in which your saved games will not be deleted after loading, enabling you to load them again after quitting or dying. Games in Casual Mode are recorded in top scores with zero score upon winning the game.";
            PopupGrid.IsVisible = true;
        }

        private void WizTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = Color.LightBlue;
            PopupTitleLabel.Text = "Wizard Mode";
            PopupLabel.Text = "Wizard Mode is a debug mode that makes you immortal and enables you to issue special wizard mode commands.";
            PopupGrid.IsVisible = true;
        }

        private void wizardModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (wizardModeSwitch.IsToggled)
            {
                //classicModeSwitch.IsToggled = true;
                //casualModeSwitch.IsToggled = false;
            }
            else
            {

            }
        }

        private void PopupLabelTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            PopupNoAgainCheckBox.IsChecked = !PopupNoAgainCheckBox.IsChecked;
        }
    }

}

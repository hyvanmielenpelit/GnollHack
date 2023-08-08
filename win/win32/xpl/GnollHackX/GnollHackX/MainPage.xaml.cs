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
using System.Buffers.Text;
using System.Net.Http;
using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Reflection;
using Newtonsoft.Json;
using System.Net;
using System.Net.Http.Headers;
using GnollHackX.Pages.Game;
using GnollHackX.Pages.MainScreen;
using Xamarin.Essentials;
using System.Collections;
using System.Collections.Concurrent;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace GnollHackX
{
    public partial class MainPage : ContentPage
    {
        public bool GameStarted { get; set; }

        public MainPage()
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
        }

        public void ActivateLocalGameButton()
        {
            StartLocalGrid.IsEnabled = true;
            StartLocalGameButton.TextColor = Color.White;
        }

        private async void StartLocalGameButton_Clicked(object sender, EventArgs e)
        {
            StartLocalGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            StartLocalGameButton.TextColor = Color.Gray;
            carouselView.Stop();

            long numberofgames = Preferences.Get("NumberOfGames", 0L);
            Preferences.Set("NumberOfGames", numberofgames + 1L);

            var gamePage = new GamePage(this);
            App.CurrentGamePage = gamePage;
            gamePage.EnableWizardMode = wizardModeSwitch.IsToggled;
            gamePage.EnableCasualMode = casualModeSwitch.IsToggled;
            gamePage.EnableModernMode = !classicModeSwitch.IsToggled;
            await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
            gamePage.StartGame();
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
                //Assembly thisassembly = GetType().GetTypeInfo().Assembly;
                //FmodLogoImage.Source = "resource://GnollHackX.Assets.FMOD-Logo-192-White.png"; // ImageSource.FromResource("GnollHackX.Assets.FMOD-Logo-192-White.png", thisassembly);
                //StartLogoImage.Source = "resource://GnollHackX.Assets.gnollhack-logo-test-2.png"; //  ImageSource.FromResource("GnollHackX.Assets.gnollhack-logo-test-2.png", thisassembly);
                //MainLogoImage.Source = "resource://GnollHackX.Assets.gnollhack-logo-test-2.png"; // ImageSource.FromResource("GnollHackX.Assets.gnollhack-logo-test-2.png", thisassembly);
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

                bool previousInformationShown = false;
                if (App.InformAboutGameTermination)
                {
                    App.InformAboutGameTermination = false;
                    await DisplayAlert("Unexpected Game Termination", "GnollHack was unexpectedly terminated when running on background. This may have been instructed by the operating system or the user. Your game may have been saved before the termination.", "OK");
                    previousInformationShown = true;
                }
                if (App.InformAboutIncompatibleSavedGames)
                {
                    App.InformAboutIncompatibleSavedGames = false;
                    await DisplayAlert("Incompatible Saved Games", "GnollHack has been updated to a newer version, for which your existing saved games are incompatible. To downgrade back to the earlier version, back up first your save files using About -> Export Saved Games and then follow the instructions at About -> Downgrade.", "OK");
                    previousInformationShown = true;
                }
                if (!previousInformationShown)
                {
                    bool ReviewRequested = Preferences.Get("StoreReviewRequested", false);
                    long NumberOfGames = Preferences.Get("NumberOfGames", 0L);
                    long TotalPlayTime = Preferences.Get("RealPlayTime", 0L);

                    if (!ReviewRequested && ((NumberOfGames >= GHConstants.StoreReviewRequestNumberOfGames && TotalPlayTime >= GHConstants.StoreReviewRequestTotalPlayTime) || App.DeveloperMode))
                    {
                        Preferences.Set("StoreReviewRequested", true);
                        UpperButtonGrid.IsEnabled = true; /* Just in case of a hangup */
                        LogoGrid.IsEnabled = true; /* Just in case of a hangup */
                        App.PlatformService?.RequestAppReview(this); /* Platform implementation is async, so this should return immediately */
                    }
                }
            }
            else if (!GameStarted)
            {
                if ((!_mainScreenMusicStarted))
                    PlayMainScreenVideoAndMusic();
            }

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
            await TryInitializeSecrets();
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
            try
            {
                App.ReadUserSecrets();
            }
            catch (Exception ex)
            {
                await DisplayAlert("Reading User Secrets File Failed", "GnollHack failed to read user secrets file: " + ex.Message, "OK");
            }
        }

        public async Task TryInitializeGnollHack()
        {
            try
            {
                App.GnollHackService.InitializeGnollHack();
            }
            catch (Exception ex)
            {
                await DisplayAlert("GnollHack Initialization Failed", "Initializing GnollHack failed: " + ex.Message, "OK");
            }
        }
        public async Task TryInitializeSecrets()
        {
            try
            {
                App.GnollHackService.InitializeSecrets(App.CurrentSecrets);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Secrets Initialization Failed", "Initializing secrets failed: " + ex.Message, "OK");
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
            ulong vercompat = 0UL;
            try
            {
                verstr = App.GnollHackService.GetVersionString();
                verid = App.GnollHackService.GetVersionId();
                vernum = App.GnollHackService.GetVersionNumber();
                vercompat = App.GnollHackService.GetVersionCompatibility();
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
            App.GHVersionCompatibility = vercompat;
            App.GHVersionId = verid;
            App.GHPath = path;
            App.FMODVersionString = fmodverstr;
            App.SkiaVersionString = skiaverstr;
            App.SkiaSharpVersionString = skiasharpverstr;

            VersionLabel.Text = verid;
            GnollHackLabel.Text = "GnollHack";

            string prev_version = Preferences.Get("VersionId", "");
            ulong prev_vernum = (ulong)Preferences.Get("VersionNumber", 0L);
            App.GHPreviousVersionNumber = prev_vernum;

            if (prev_version != verid || prev_vernum != vernum)
            {
                await TryClearCoreFiles();
                await TryInitializeGnollHack();
                await TryInitializeSecrets();
            }
            if (prev_vernum > 0 && prev_vernum < vercompat)
                App.CheckForIncompatibleSavedGames();

            Preferences.Set("VersionId", verid);
            Preferences.Set("VersionNumber", (long)vernum);

            App.PlatformService.OnDemandPackStatusNotification += OnDemandPackEventHandler;
            StartFetchOnDemandFiles();
            App.SetSoundBanksUpForLoading();
 
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

        private void OnDemandPackEventHandler(object sender, AssetPackStatusEventArgs e)
        {
            int status = e.Status;
            switch (status)
            {
                case GHAssetPackStatus.Pending:
                    DownloadLabel.Text = "On-demand pack is pending";
                    break;
                case GHAssetPackStatus.Downloading:
                    DownloadLabel.Text = "On-demand pack is downloading: " + e.DownloadedBytes + " / " + e.TotalBytes + " (" + Math.Round(e.PercentDownloaded, 1) + ")";
                    break;
                case GHAssetPackStatus.Transferring:
                    DownloadLabel.Text = "On-demand pack is transferring";
                    break;
                case GHAssetPackStatus.Completed:
                    DownloadLabel.Text = "On-demand pack has completed download: " + e.CompletedAssetPackPath;
                    break;
                case GHAssetPackStatus.Failed:
                    DownloadLabel.Text = "On-demand pack download failed";
                    break;
                case GHAssetPackStatus.Canceled:
                    DownloadLabel.Text = "On-demand pack download was cancelled";
                    break;
                case GHAssetPackStatus.WaitingForWifi:
                    DownloadLabel.Text = "On-demand pack download is waiting for wifi";
                    break;
                case GHAssetPackStatus.NotInstalled:
                    DownloadLabel.Text = "On-demand pack is not installed";
                    break;
            }
        }

        private void StartFetchOnDemandFiles()
        {
            if(App.IsAndroid && App.DownloadOnDemandPackage)
            {
                DownloadLabel.Text = "";
                string path = App.PlatformService.GetAbsoluteOnDemandAssetPath(GHConstants.OnDemandPackName);
                if(path == null) 
                {
                    int res = App.PlatformService.FetchOnDemandPack(GHConstants.OnDemandPackName);
                }
            }
        }

        public void PlayMainScreenVideoAndMusic()
        {
            carouselView.IsVisible = true;
            carouselView.Play();

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

            carouselView.IsVisible = true;
            carouselView.InvalidateSurface();
            await carouselView.FadeTo(1, 250);
            carouselView.Opacity = 1.0; /* To make sure */
            carouselView.Play();

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

        private void ExitAppButton_Clicked(object sender, EventArgs e)
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
                CloseApp();
            }
        }

        private void CloseApp()
        {
            App.PlatformService.CloseApplication();
            Thread.Sleep(75);
            System.Diagnostics.Process.GetCurrentProcess().Kill();
        }

        private async void ClearFilesButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            carouselView.Stop();
            var resetPage = new ResetPage();
            resetPage.Disappearing += (sender2, e2) =>
            {
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
            carouselView.Stop();
            var settingsPage = new SettingsPage(null, this);
            settingsPage.Disappearing += (sender2, e2) =>
            {
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
            UpperButtonGrid.IsEnabled = true;
        }

        private async void OptionsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            carouselView.Stop();
            string fulltargetpath = Path.Combine(App.GHPath, "defaults.gnh");
            var editorPage = new EditorPage(fulltargetpath, "Default Options File");
            string errormsg = "";
            if (!editorPage.ReadFile(out errormsg))
            {
                ErrorLabel.Text = errormsg;
                carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            }
            else
            {
                ErrorLabel.Text = "";
                editorPage.Disappearing += (sender2, e2) =>
                {
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
            carouselView.Stop();
            var aboutPage = new AboutPage();
            aboutPage.Disappearing += (sender2, e2) =>
            {
                carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(aboutPage);
            UpperButtonGrid.IsEnabled = true;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            carouselView.Stop();
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
            carouselView.IsVisible = true;
        }

        public void UpdateMainScreenBackgroundElement(bool playAfterUpdate)
        {
            carouselView.InvalidateSurface();
        }

        private async void TopScoreButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            carouselView.Stop();
            string fulltargetpath = Path.Combine(App.GHPath, "xlogfile");
            if(File.Exists(fulltargetpath))
            {
                var topScorePage = new TopScorePage(fulltargetpath);
                string errormsg = "";
                if (!topScorePage.ReadFile(out errormsg))
                {
                    ErrorLabel.Text = errormsg;
                    carouselView.Play();
                    UpperButtonGrid.IsEnabled = true;
                }
                else
                {
                    ErrorLabel.Text = "";
                    topScorePage.Disappearing += (sender2, e2) =>
                    {
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

        public void Suspend()
        {
            carouselView.Stop();
        }

        public void Resume()
        {
            if(App.CurrentGamePage == null)
            {
                carouselView.Play();
            }
        }
    }

}

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
using Plugin.InAppBilling;
using System.Collections;
using System.Collections.Concurrent;

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
            StartLocalGoneLabel.IsVisible = true;
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
        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            wizardModeGrid.IsVisible = App.DeveloperMode;
            if (!App.DeveloperMode)
                wizardModeSwitch.IsToggled = false;
            ResetGrid.IsVisible = App.DeveloperMode;
            OptionsGrid.IsVisible = App.DeveloperMode;
            StartServerGrid.IsVisible = App.ServerGameAvailable;

            beginnerModeSwitch.IsToggled = App.BeginnerMode;

            UpdateSponsor();

            if (_firsttime)
            {
                App.DebugWriteProfilingStopwatchTimeAndRestart("MainPage First Time");
                _firsttime = false;
                Assembly thisassembly = GetType().GetTypeInfo().Assembly;
                FmodLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.FMOD-Logo-192-White.png", thisassembly);
                StartLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                MainLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                await StartFadeLogoIn();
                await StartUpTasks();
                await StartFadeIn();
                StartLogoImage.IsVisible = false;
                FmodLogoImage.IsVisible = false;
            }
            else if (!GameStarted && videoView.IsVisible == false)
            {
                PlayMainScreenVideoAndMusic();
            }

            StartServerGrid.IsEnabled = true;
            UpperButtonGrid.IsEnabled = true;
            LogoGrid.IsEnabled = true;
        }

        private async Task StartUpTasks()
        {
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            App.InitializeServices();
            App.InitTypefaces(assembly);
            App.IsModernAndroid = App.AppCloseService.IsModernAndroid();
            string verstr = App.GnollHackService.GetVersionString();
            string verid = App.GnollHackService.GetVersionId();
            string path = App.GnollHackService.GetGnollHackPath();
            App.GHVersionString = verstr;
            App.GHVersionId = verid;
            App.GHPath = path;
            VersionLabel.Text = verid;
            GnollHackLabel.Text = "GnollHack"; // + verstr;

            string prev_version = Preferences.Get("VersionId", "");
            if(prev_version != verid)
            {
                App.GnollHackService.ClearCoreFiles();
                App.GnollHackService.InitializeGnollHack();
            }
            Preferences.Set("VersionId", verid);

            if (VersionTracking.IsFirstLaunchEver)
            {
                // Do something
            }
            else if (VersionTracking.IsFirstLaunchForCurrentVersion)
            {
                // Do something
            }
            else if (VersionTracking.IsFirstLaunchForCurrentBuild)
            {
                // Do something
            }


            StartLocalGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            StartServerGameImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            clearImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            topScoreImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            optionsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            settingsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            creditsImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            exitImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            StillImage.Source = ImageSource.FromResource("GnollHackClient.Assets.main-menu-portrait-snapshot.jpg", assembly);

            ResetAcquiredFiles();
            await AcquireAndCheckFiles();
            App.FmodService.LoadBanks();

            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume;
            generalVolume = Preferences.Get("GeneralVolume", 1.0f);
            musicVolume = Preferences.Get("MusicVolume", 0.5f);
            ambientVolume = Preferences.Get("AmbientVolume", 0.5f);
            dialogueVolume = Preferences.Get("DialogueVolume", 0.5f);
            effectsVolume = Preferences.Get("EffectsVolume", 0.5f);
            UIVolume = Preferences.Get("UIVolume", 0.5f);
            App.FmodService.AdjustVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume);
            App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);
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
            videoView.IsVisible = true;
            videoView.Play();
            StillImage.IsVisible = false;

            App.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);

        }

        private async Task StartFadeLogoIn()
        {
            await StartLogoImage.FadeTo(1, 250);
            await FmodLogoImage.FadeTo(1, 250);
            //List<Task> tasklist = new List<Task> { t1, t2 };
            //await Task.WhenAll(tasklist);
        }

        private async Task StartFadeIn()
        {
            await StartLogoImage.FadeTo(0, 250);
            await FmodLogoImage.FadeTo(0, 250);
            //Task[] tasklist1 = new Task[2] { t1, t2 };
            //Task.WaitAll(tasklist1);

            videoView.IsVisible = true;
            await videoView.FadeTo(1, 250);
            videoView.Play();

            UpperButtonGrid.IsVisible = true;
            await UpperButtonGrid.FadeTo(1, 250);
            StartButtonGrid.IsVisible = true;
            await StartButtonGrid.FadeTo(1, 250);
            LogoGrid.IsVisible = true;
            await LogoGrid.FadeTo(1, 250);

            //List<Task> tasklist2 = new List<Task> { t3, t4, t5 };
            //await Task.WhenAll(tasklist2);
        }

        //public async Task CheckPurchaseStatus(bool atappstart)
        //{
        //    int res = await IsProductPurchased(GHConstants.DistributionFeeProductName);
        //    if(res >= 0)
        //    {
        //        if (Preferences.ContainsKey("CheckPurchase_FirstConnectFail"))
        //        {
        //            Preferences.Remove("CheckPurchase_FirstConnectFail");
        //        }
        //        if (Preferences.ContainsKey("CheckPurchase_ConnectFail_GameStartCount"))
        //        {
        //            Preferences.Remove("CheckPurchase_ConnectFail_GameStartCount");
        //        }
        //    }

        //    if (res == 0 && App.FullVersionMode)
        //    {
        //        App.FullVersionMode = false;
        //        Preferences.Set("FullVersion", false);
        //    }
        //    else if (res == 1 && !App.FullVersionMode)
        //    {
        //        App.FullVersionMode = true;
        //        Preferences.Set("FullVersion", true);
        //        if(!atappstart)
        //        {
        //            await DisplayAlert("Existing Purchase Found", "Your existing purchase of the full version was found and has been activated.", "OK");
        //        }
        //    }
        //    else if(res == -1 && App.FullVersionMode && atappstart)
        //    {
        //        if (!Preferences.ContainsKey("CheckPurchase_FirstConnectFail"))
        //            Preferences.Set("CheckPurchase_FirstConnectFail", DateTime.Now);

        //        DateTime firstfaildate = Preferences.Get("CheckPurchase_FirstConnectFail", DateTime.MinValue);
        //        TimeSpan ts = DateTime.Now - firstfaildate;
        //        double ddays = ts.TotalDays;

        //        int game_start_count = Preferences.Get("CheckPurchase_ConnectFail_GameStartCount", 0);
        //        game_start_count++;
        //        Preferences.Set("CheckPurchase_ConnectFail_GameStartCount", game_start_count);

        //        if (ddays > 30 || game_start_count > 30)
        //        {
        //            App.FullVersionMode = false;
        //            await DisplayAlert("Verification Connection Failure",
        //                "GnollHack has been unable to verify the full version purchase for more than " + (ddays > 30 ? "30 days" : "30 app starts") + ". Only demo version features are accessible until verification is successful. Please check your internet connection.", "OK");
        //        }
        //        else
        //        {
        //            await DisplayAlert("Verification Connection Failure", 
        //                "GnollHack is unable to connect to verify your full version. Your are able to use the full version still for "
        //                + (ddays < 29 ? string.Format("{0:0}", ddays) : string.Format("{0:0.0}", ddays))
        //                + " days" + (game_start_count < 29 ? " up to " + (30 - game_start_count) + " times." : game_start_count == 29 ? " one more time." : ". This is the last time you can use the full version."), "OK");
        //        }
        //    }
        //    UpdateSponsor();
        //}


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
            UpperButtonGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            var resetPage = new ResetPage();
            await App.Current.MainPage.Navigation.PushModalAsync(resetPage);

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
            var creditsPage = new CreditsPage();
            await App.Current.MainPage.Navigation.PushModalAsync(creditsPage);
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


        private void ResetAcquiredFiles()
        {
            string ghdir = App.GnollHackService.GetGnollHackPath();
            bool resetfiles = Preferences.Get("ResetExternalFiles", true);
            if (resetfiles)
            {
                foreach (SecretsFile sf in App.CurrentSecrets.files)
                {
                    if (resetfiles)
                    {
                        string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory);
                        string sfile = Path.Combine(sdir, sf.name);
                        if (File.Exists(sfile))
                        {
                            FileInfo file = new FileInfo(sfile);
                            file.Delete();
                        }
                        if (Preferences.ContainsKey("Verify_" + sf.id + "_Version"))
                            Preferences.Remove("Verify_" + sf.id + "_Version");
                        if (Preferences.ContainsKey("Verify_" + sf.id + "_LastWriteTime"))
                            Preferences.Remove("Verify_" + sf.id + "_LastWriteTime");
                    }
                }
                Preferences.Set("ResetExternalFiles", false);
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

        private async Task AcquireAndCheckFiles()
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

                if ((sf.flags & (int)secrets_flags.IncludedInFullVersion) != 0 && !App.FullVersionMode)
                    continue;
                
                if (sf.source == "url")
                {
                    dltask = DownloadFileFromWebServer(assembly, sf, ghdir);
                }
                else if (sf.source == "resource")
                {
                    restask = DownloadFileFromResources(assembly, sf, ghdir);
                }

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
                    string sdir = string.IsNullOrWhiteSpace(sf.target_directory) ? ghdir : Path.Combine(ghdir, sf.target_directory);
                    string sfile = Path.Combine(sdir, sf.name);
                    App.FmodService.AddLoadableSoundBank(sfile);
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

        public async Task<downloaded_file_check_results> DownloadFileFromResources(Assembly assembly, SecretsFile f, string ghdir)
        {
            if (f == null)
                return downloaded_file_check_results.NoSecretsFile;

            downloaded_file_check_results dres = await VerifySecretsFile(assembly, f, ghdir);
            if (dres == downloaded_file_check_results.OK)
            {
                _downloadResultConcurrentDictionary.TryAdd(f, dres);
                return dres;
            }

            string sdir = string.IsNullOrWhiteSpace(f.target_directory) ? ghdir : Path.Combine(ghdir, f.target_directory);
            string sfile = Path.Combine(sdir, f.name);

            if (!File.Exists(sfile))
            {
                using (Stream stream = assembly.GetManifestResourceStream(f.source_path))
                {
                    using (var fileStream = File.Create(sfile))
                    {
                        stream.CopyTo(fileStream);
                    }
                }
                dres = downloaded_file_check_results.OK;
                _downloadResultConcurrentDictionary.TryAdd(f, dres);
                return dres;
            }
            else
            {
                dres = downloaded_file_check_results.FileAlreadyExists;
                _downloadResultConcurrentDictionary.TryAdd(f, dres);
                return dres;
            }
        }

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

            string url = f.source_path;
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
            return chksum == sha256;
        }

        public async Task<bool> PurchaseNonConsumable(string productId)
        {
            if (!CrossInAppBilling.IsSupported)
            {
                await DisplayAlert("Purchasing Not Supported", "Purchasing an upgrade is not supported on your platform.", "OK");
                return false;
            }

            var billing = CrossInAppBilling.Current;
            try
            {
                var connected = await billing.ConnectAsync();
                if (!connected)
                {
                    await DisplayAlert("Connection Failed", "There was an error in connecting to the store.", "OK");
                    return false;
                }

                //check purchases
                var purchase = await billing.PurchaseAsync(productId, ItemType.InAppPurchase);

                //possibility that a null came through.
                if (purchase == null)
                {
                    //did not purchase
                    await DisplayAlert("Purchase Failed", "Your purchase failed.", "OK");
                    return false;
                }
                else if (purchase.State == PurchaseState.Purchased)
                {
                    //purchased!
                    if (Device.RuntimePlatform == Device.Android)
                    {
                        // Must call AcknowledgePurchaseAsync else the purchase will be refunded
                        await billing.AcknowledgePurchaseAsync(purchase.PurchaseToken);
                    }
                    return true;
                }
                else
                {
                    await DisplayAlert("Upgrade Not Purchased", "The upgrade was not purchased.", "OK");
                    return false;
                }
            }
            catch (InAppBillingPurchaseException purchaseEx)
            {
                //Billing Exception handle this based on the type
                Debug.WriteLine("Error: " + purchaseEx);
                await DisplayAlert("Error in Purchasing Upgrade", "There was an error purchasing the full version upgrade.", "OK");
                return false;
            }
            catch (Exception ex)
            {
                //Something else has gone wrong, log it
                Debug.WriteLine("Issue connecting: " + ex);
                await DisplayAlert("Error", "There was a general error in the transaction.", "OK");
                return false;
            }
            finally
            {
                await billing.DisconnectAsync();
            }
        }


        public async Task<int> IsProductPurchased(string productId)
        {
            var billing = CrossInAppBilling.Current;
            try
            {
                var connected = await billing.ConnectAsync();

                if (!connected)
                {
                    //Couldn't connect
                    return -1;
                }

                //check purchases
                var purchases = await billing.GetPurchasesAsync(ItemType.InAppPurchase);

                //check for null just in case
                if (purchases?.Any(p => p.ProductId == productId) ?? false)
                {
                    foreach(InAppBillingPurchase purchase in purchases)
                    {
                        if(purchase.ProductId == productId)
                        {
                            if (purchase.State == PurchaseState.Purchased)
                                return 1;
                        }
                    }
                    //No purchased purchases left
                    return 0;
                }
                else
                {
                    //no purchases found
                    return 0;
                }
            }
            catch (InAppBillingPurchaseException purchaseEx)
            {
                //Billing Exception handle this based on the type
                Debug.WriteLine("Error: " + purchaseEx);
                return -1;
            }
            catch (Exception ex)
            {
                //Something has gone wrong
                return -1;
            }
            finally
            {
                await billing.DisconnectAsync();
            }
        }

        private async void SponsorButton_Clicked(object sender, EventArgs e)
        {
            SponsorButton.IsEnabled = false;
            Uri uri = new Uri(GHConstants.GnollHackSponsorPage);
            App.PlayButtonClickedSound();
            try
            {
                await Browser.OpenAsync(uri, BrowserLaunchMode.SystemPreferred);
                App.SponsorButtonVisited = true;
                Preferences.Set("SponsorButtonVisited", true);
                App.ShowSpecialEffect = false;
                Preferences.Set("ShowSpecialEffect", false);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Cannot Open Web Page", "GnollHack cannot open the webpage at " + uri.OriginalString + ". Error: " + ex.Message, "OK");
            }
            SponsorButton.IsEnabled = true;
        }

        //public async Task<bool> PurchaseConsumable(string productId)
        //{
        //    if (!CrossInAppBilling.IsSupported)
        //    {
        //        await DisplayAlert("Purchasing Not Supported", "Purchasing an extra life is not supported on your platform.", "OK");
        //        return false;
        //    }

        //    var billing = CrossInAppBilling.Current;
        //    try
        //    {
        //        var connected = await billing.ConnectAsync();
        //        if (!connected)
        //        {
        //            await DisplayAlert("Connection Failed", "There was an error in connecting to the store.", "OK");
        //            return false;
        //        }

        //        //check purchases
        //        var purchase = await billing.PurchaseAsync(productId, ItemType.InAppPurchase);

        //        //possibility that a null came through.
        //        if (purchase == null)
        //        {
        //            //did not purchase
        //            await DisplayAlert("Purchase Failed", "Your purchase failed.", "OK");
        //            return false;
        //        }
        //        else if (purchase.State == PurchaseState.Purchased)
        //        {
        //            //purchased!
        //            if (Device.RuntimePlatform == Device.Android)
        //            {
        //                // Must call AcknowledgePurchaseAsync else the purchase will be refunded
        //                await billing.AcknowledgePurchaseAsync(purchase.PurchaseToken);
        //            }

        //            if (Device.RuntimePlatform == Device.iOS)
        //                return true;

        //            /* Consumption */
        //            bool success = await CrossInAppBilling.Current.ConsumePurchaseAsync(purchase.ProductId, purchase.PurchaseToken);
        //            if (success)
        //            {
        //                return true;
        //            }
        //            await DisplayAlert("Extra Life Purchased But Not Consumed", "The extra life was purchased but not consumed.", "OK");
        //            return false;
        //        }
        //        else
        //        {
        //            await DisplayAlert("Extra Life Not Purchased", "The extra life was not purchased.", "OK");
        //            return false;
        //        }
        //    }
        //    catch (InAppBillingPurchaseException purchaseEx)
        //    {
        //        //Billing Exception handle this based on the type
        //        Debug.WriteLine("Error: " + purchaseEx);
        //        await DisplayAlert("Error in Purchasing Extra Life", "There was an error purchasing an extra life upgrade.", "OK");
        //        return false;
        //    }
        //    catch (Exception ex)
        //    {
        //        //Something else has gone wrong, log it
        //        Debug.WriteLine("Issue connecting: " + ex);
        //        await DisplayAlert("Error", "There was a general error in the transaction.", "OK");
        //        return false;
        //    }
        //    finally
        //    {
        //        await billing.DisconnectAsync();
        //    }
        //}

        //public async Task<InAppBillingProduct> GetProductDetails(string productid)
        //{
        //    InAppBillingProduct res = null;
        //    var billing = CrossInAppBilling.Current;
        //    try
        //    {

        //        var productIds = new string[] { productid };
        //        //You must connect
        //        var connected = await billing.ConnectAsync();

        //        if (!connected)
        //        {
        //            //Couldn't connect
        //            return res;
        //        }

        //        //check purchases

        //        var items = await billing.GetProductInfoAsync(ItemType.InAppPurchase, productIds);

        //        foreach (var item in items)
        //        {
        //            if(item != null && item.ProductId == productid)
        //            {
        //                res = item;
        //                break;
        //            }
        //        }
        //    }
        //    catch (InAppBillingPurchaseException pEx)
        //    {
        //        //Handle IAP Billing Exception
        //    }
        //    catch (Exception ex)
        //    {
        //        //Something has gone wrong
        //    }
        //    finally
        //    {
        //        await billing.DisconnectAsync();
        //    }
        //    return res;
        //}

        private void UpdateSponsor()
        {
            AndroidLabel.Text = "Android Version";
        }

        private void hardcoreModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            App.BeginnerMode = beginnerModeSwitch.IsToggled;
            Preferences.Set("BeginnerMode", App.BeginnerMode);
        }

        private async void StoreButton_Clicked(object sender, EventArgs e)
        {
            LogoGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            var storepage = new StorePage(this);
            await App.Current.MainPage.Navigation.PushModalAsync(storepage);

        }

        private void PopupOkButton_Clicked(object sender, EventArgs e)
        {
            PopupGrid.IsVisible = false;
        }

        private void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            PopupTitleLabel.Text = "Beginner Mode";
            PopupLabel.Text = "In Beginner Mode, you [enter the Beginner Mode description].";
            PopupGrid.IsVisible = true;
        }
    }

}

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
            ResetGrid.IsVisible = App.DeveloperMode;
            OptionsGrid.IsVisible = App.DeveloperMode;
            if (!App.DeveloperMode)
                wizardModeSwitch.IsToggled = false;
            UpdateBuyNow();

            if (_firsttime)
            {
                App.DebugWriteProfilingStopwatchTimeAndRestart("MainPage First Time");
                _firsttime = false;
                if (App.FullVersionMode)
                    await CheckPurchaseStatus(true);
                _banksAcquired = 0;
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
        }

        private async Task StartUpTasks()
        {
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            App.LoadServices();
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
                Preferences.Set("ResetLocalBanks", true);
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

            await AcquireBanks();
            await CheckBanks();
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
            float percentage;
            long bytesDownloaded;
            long? totalFileSize;
            string fileDescription;
            long desiredFileSize;
            lock (_downloadProgressLock)
            {
                percentage = _downloadProgressPercentage;
                bytesDownloaded = _downloadProgressTotalBytesDownloaded;
                totalFileSize = _downloadProgressTotalFileSize;
                fileDescription = _downloadProgressFileDescription;
                desiredFileSize = _downloadProgressDesiredFileSize;           
            }

            if (totalFileSize != null)
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    DownloadTitleLabel.Text = "Downloading...";
                    DownloadFileNameLabel.Text = fileDescription;
                    if (totalFileSize <= 0)
                        DownloadBytesLabel.Text = "Calculating file size...";
                    else
                        DownloadBytesLabel.Text = string.Format("{0:0.00}", (double)bytesDownloaded / 1000000) + " / " + string.Format("{0:0.00}", totalFileSize / 1000000) + " MB";
                });
                await DownloadProgressBar.ProgressTo((float)percentage / 100, 100, Easing.Linear);
            }

            if (totalFileSize != null && totalFileSize > 0 && desiredFileSize > 0 && totalFileSize != desiredFileSize && _cancellationTokenSource.Token.CanBeCanceled)
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
                    await DisplayAlert("Downloadable File Invalid", "The specified download file does not match the required file description. Aborting.", "OK");
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
        }

        private async Task StartFadeIn()
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

        public async Task CheckPurchaseStatus(bool atappstart)
        {
            int res = await IsUpgradePurchased(GHConstants.FullVersionProductName);
            if(res >= 0)
            {
                if (Preferences.ContainsKey("CheckPurchase_FirstConnectFail"))
                {
                    Preferences.Remove("CheckPurchase_FirstConnectFail");
                }
                if (Preferences.ContainsKey("CheckPurchase_ConnectFail_GameStartCount"))
                {
                    Preferences.Remove("CheckPurchase_ConnectFail_GameStartCount");
                }
            }

            if (res == 0 && App.FullVersionMode)
            {
                App.FullVersionMode = false;
                Preferences.Set("FullVersion", false);
            }
            else if (res == 1 && !App.FullVersionMode)
            {
                App.FullVersionMode = true;
                Preferences.Set("FullVersion", true);
                if(!atappstart)
                {
                    await DisplayAlert("Existing Purchase Found", "Your existing purchase of the full version was found and has been activated.", "OK");
                }
            }
            else if(res == -1 && App.FullVersionMode && atappstart)
            {
                if (!Preferences.ContainsKey("CheckPurchase_FirstConnectFail"))
                    Preferences.Set("CheckPurchase_FirstConnectFail", DateTime.Now);

                DateTime firstfaildate = Preferences.Get("CheckPurchase_FirstConnectFail", DateTime.MinValue);
                TimeSpan ts = DateTime.Now - firstfaildate;
                double ddays = ts.TotalDays;

                int game_start_count = Preferences.Get("CheckPurchase_ConnectFail_GameStartCount", 0);
                game_start_count++;
                Preferences.Set("CheckPurchase_ConnectFail_GameStartCount", game_start_count);

                if (ddays > 30 || game_start_count > 30)
                {
                    App.FullVersionMode = false;
                    await DisplayAlert("Verification Connection Failure",
                        "GnollHack has been unable to verify the full version purchase for more than " + (ddays > 30 ? "30 days" : "30 app starts") + ". Only demo version features are accessible until verification is successful. Please check your internet connection.", "OK");
                }
                else
                {
                    await DisplayAlert("Verification Connection Failure", 
                        "GnollHack is unable to connect to verify your full version. Your are able to use the full version still for "
                        + (ddays < 29 ? string.Format("{0:0}", ddays) : string.Format("{0:0.0}", ddays))
                        + " days" + (game_start_count < 29 ? " up to " + (30 - game_start_count) + " times." : game_start_count == 29 ? " one more time." : ". This is the last time you can use the full version."), "OK");
                }
            }
            UpdateBuyNow();
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

        private int _banksAcquired = 0;
        private object _downloadProgressLock = new object();
        private float _downloadProgressPercentage = 0.0f;
        private long _downloadProgressTotalBytesDownloaded = 0;
        private long? _downloadProgressTotalFileSize = 0;
        private string _downloadProgressFileDescription = "";
        private string _downloadProgressFilePath = "";
        private long _downloadProgressDesiredFileSize = 0;

        private async Task AcquireBanks()
        {
            string bank_dir = App.FmodService.GetBankDir();
            Assembly assembly = GetType().GetTypeInfo().Assembly;

            /* Make the relevant directory */
            if (!Directory.Exists(bank_dir))
            {
                Directory.CreateDirectory(bank_dir);
            }

            bool resetbanks = Preferences.Get("ResetBanks", true);
            if (resetbanks)
            {
                DirectoryInfo di = new DirectoryInfo(bank_dir);
                foreach (FileInfo file in di.GetFiles())
                {
                    file.Delete();
                }
                if (Preferences.ContainsKey("VerifyBank_Version"))
                    Preferences.Remove("VerifyBank_Version");
                if (Preferences.ContainsKey("VerifyBank_LastWriteTime"))
                    Preferences.Remove("VerifyBank_LastWriteTime");

                Preferences.Set("ResetBanks", false);
            }

            bool resetlocalbanks = Preferences.Get("ResetLocalBanks", true);
            if (resetlocalbanks)
            {
                if(!resetbanks)
                {
                    DirectoryInfo di = new DirectoryInfo(bank_dir);
                    foreach (FileInfo file in di.GetFiles())
                    {
                        bool docontinue = false;
                        for (int i = 0; i < App.BankWebDownloadList.Length; i++)
                        {
                            if (App.BankWebDownloadList[i] && file.Name == App.BankNameList[i])
                            {
                                docontinue = true;
                                break;
                            }
                        }
                        if (docontinue)
                            continue;
                        file.Delete();
                    }
                }
                Preferences.Set("ResetLocalBanks", false);
            }

            App.DebugWriteProfilingStopwatchTimeAndStart("Start Acquiring Banks");
            for (int idx = 0; idx < App.BankNameList.Length; idx++)
            {
                string bank_path = Path.Combine(bank_dir, App.BankNameList[idx]);
                if (App.BankWebDownloadList[idx])
                {
                    downloaded_file_check_results res = await DownloadFileFromWebServer(assembly, App.BankNameList[idx], bank_dir);
                }
                else
                {
                    if (!File.Exists(bank_path))
                    {
                        using (Stream stream = assembly.GetManifestResourceStream(App.BankResourceList[idx]))
                        {
                            using (var fileStream = File.Create(bank_path))
                            {
                                stream.CopyTo(fileStream);
                            }
                        }
                    }
                }
                _banksAcquired++;
                App.DebugWriteProfilingStopwatchTimeAndStart("Acquired Bank "+ idx);
            }
        }

        private async Task CheckBanks()
        {
            string bank_dir = App.FmodService.GetBankDir();
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            for (int idx = 0; idx < App.BankNameList.Length; idx++)
            {
                string bank_path = Path.Combine(bank_dir, App.BankNameList[idx]);
                if (App.BankWebDownloadList[idx])
                {
                    downloaded_file_check_results res = await CheckDownloadedFile(assembly, App.BankNameList[idx], bank_dir);
                    if(res != downloaded_file_check_results.OK)
                    {
                        bool diddelete = false;
                        if (res == downloaded_file_check_results.VerificationFailed)
                        {
                            string target_path = Path.Combine(bank_dir, App.BankNameList[idx]);
                            if(File.Exists(target_path))
                            {
                                FileInfo file = new FileInfo(target_path);
                                file.Delete();
                                diddelete = true;
                            }
                        }
                        await DisplayAlert("Sound Bank Verification Failure", "Sound bank verification failed with error code " + (int)res + "." + (diddelete ? " Deleting the sound bank." : ""), "OK");
                    }
                }
            }
        }

        private void CancelDownloadButton_Clicked(object sender, EventArgs e)
        {
            _cancellationTokenSource.Cancel();
        }

        //private static HttpClient _httpClient = new HttpClient();
        CancellationTokenSource _cancellationTokenSource = new CancellationTokenSource();

        public async Task<downloaded_file_check_results> DownloadFileFromWebServer(Assembly assembly, string filename, string target_directory)
        {
            string json = "";
            App.DebugWriteProfilingStopwatchTimeAndStart("Start Downloading Bank " + filename);
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.secrets.jsons"))
            {
                if (stream != null)
                {
                    using (StreamReader sr = new StreamReader(stream))
                    {
                        json = sr.ReadToEnd();
                    }
                }
                else
                {
                    return downloaded_file_check_results.NoSecretsFile;
                }
            }

            string target_path = Path.Combine(target_directory, filename);

            DownloadableFileList dflist = JsonConvert.DeserializeObject<DownloadableFileList>(json);
            DownloadableFile f = null;
            if(dflist != null && dflist.files != null)
            {
                foreach (DownloadableFile df in dflist.files)
                {
                    if (df.name == filename)
                    {
                        f = df;
                        break;
                    }
                }
            }

            if (f == null)
            {
                return downloaded_file_check_results.FileNotInSecretsList;
            }

            /* Check if ok */
            bool correctversion = true;
            string bankversion = Preferences.Get("VerifyBank_Version", "");
            if (bankversion != "" && bankversion != f.version)
                correctversion = false;

            if (File.Exists(target_path))
            {
                FileInfo file = new FileInfo(target_path);
                bool isfileok = true;
                bool isdateok = false;
                if(!correctversion)
                    isfileok = false;
                if (file.Length != f.length)
                    isfileok = false;
                if(isfileok)
                {
                    long moddatelong = Preferences.Get("VerifyBank_LastWriteTime", 0L);
                    DateTime moddate = DateTime.FromBinary(moddatelong);
                    if(moddate == file.LastWriteTimeUtc)
                    {
                        isdateok = true;
                    }
                    else
                    {
                        isfileok = await VerifyDownloadedFile(target_path, f.sha256);
                        if(isfileok)
                            Preferences.Set("VerifyBank_LastWriteTime", file.LastWriteTimeUtc.ToBinary());
                    }
                }
                if (isfileok)
                {
                    /* Ok, no need to download */
                    App.DebugWriteProfilingStopwatchTimeAndStart(isdateok ? "Length and date ok, exiting" : "Length and checksum ok, exiting");
                    return downloaded_file_check_results.OK;
                }
                else
                {
                    /* Wrong, delete and download */
                    file.Delete();
                }
            }

            /* Set version to be downloaded, and download */
            Preferences.Set("VerifyBank_Version", f.version);
            Device.BeginInvokeOnMainThread(() =>
            {
                DownloadGrid.IsVisible = true;
            });

            string url = f.download_url;
            lock (_downloadProgressLock)
            {
                _downloadProgressFileDescription = f.description;
                _downloadProgressFilePath = target_path;
                _downloadProgressDesiredFileSize = f.length;
            }

            App.DebugWriteProfilingStopwatchTimeAndStart("Begin Http Download " + filename);
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
                    await client.StartDownload();
                }
                catch(OperationCanceledException)
                {
                    /* Download was cancelled */
                }
                catch(Exception e)
                {
                    if(!_cancellationTokenSource.Token.IsCancellationRequested)
                        await DisplayAlert("Download Error", "A download error occurred: " + e.Message, "OK");
                }
            }

            /* Finished download */
            DownloadGrid.IsVisible = false;
            if (_cancellationTokenSource.Token.IsCancellationRequested)
            {
                /* Delete cancelled files */
                if (File.Exists(_downloadProgressFilePath))
                {
                    FileInfo file = new FileInfo(_downloadProgressFilePath);
                    file.Delete();
                }
            }

            _cancellationTokenSource.Dispose();
            _cancellationTokenSource = new CancellationTokenSource();

            return downloaded_file_check_results.OK;
        }


        public async Task<downloaded_file_check_results> CheckDownloadedFile(Assembly assembly, string filename, string target_directory)
        {
            string json = "";
            using (Stream stream = assembly.GetManifestResourceStream("GnollHackClient.Assets.secrets.jsons"))
            {
                if (stream != null)
                {
                    using (StreamReader sr = new StreamReader(stream))
                    {
                        json = sr.ReadToEnd();
                    }
                }
                else
                    return downloaded_file_check_results.NoSecretsFile;
            }

            string target_path = Path.Combine(target_directory, filename);

            DownloadableFileList dflist = JsonConvert.DeserializeObject<DownloadableFileList>(json);
            DownloadableFile f = null;
            if (dflist != null && dflist.files != null)
            {
                foreach (DownloadableFile df in dflist.files)
                {
                    if (df.name == filename)
                    {
                        f = df;
                        break;
                    }
                }
            }

            if (f == null)
                return downloaded_file_check_results.FileNotInSecretsList;

            /* Check if ok */
            if (File.Exists(target_path))
            {
                FileInfo file = new FileInfo(target_path);
                bool isfileok = true;
                if (file.Length != f.length)
                    isfileok = false;
                if (isfileok)
                {
                    long moddatelong = Preferences.Get("VerifyBank_LastWriteTime", 0L);
                    DateTime moddate = DateTime.FromBinary(moddatelong);
                    if (moddate != file.LastWriteTimeUtc)
                    {
                        isfileok = await VerifyDownloadedFile(target_path, f.sha256);
                        if (isfileok)
                            Preferences.Set("VerifyBank_LastWriteTime", file.LastWriteTimeUtc.ToBinary());

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
        public async Task<bool> VerifyDownloadedFile(string target_path, string sha256)
        {
            App.DebugWriteProfilingStopwatchTimeAndStart("Begin Checksum");

            Device.BeginInvokeOnMainThread(() =>
            {
                DownloadTitleLabel.Text = "Verifying...";
                DownloadFileNameLabel.Text = "Downloaded files";
                DownloadBytesLabel.Text = "Please wait...";
                DownloadProgressBar.IsVisible = false;
                DownloadButtonGrid.IsVisible = false;
                DownloadGrid.IsVisible = true;
            });
            await Task.Run(() => {
                string checksum = ChecksumUtil.GetChecksum(HashingAlgoTypes.SHA256, target_path);
                lock (checksumlock)
                {
                    _checksum = checksum;
                };
            });
            Device.BeginInvokeOnMainThread(() =>
            {
                DownloadGrid.IsVisible = false;
                DownloadButtonGrid.IsVisible = true;
                DownloadProgressBar.IsVisible = true;
            });

            App.DebugWriteProfilingStopwatchTimeAndStart("Finish Checksum");
            string chksum ="";
            lock (checksumlock)
            {
                chksum = _checksum;
            };
            return chksum == sha256;
        }

        public async Task<bool> MakePurchase(string productId)
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


        public async Task<int> IsUpgradePurchased(string productId)
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

        private async void BuyNowButton_Clicked(object sender, EventArgs e)
        {
            BuyNowButton.IsEnabled = false;
            await CheckPurchaseStatus(false);
            await PurchaseUpgrade();
            BuyNowButton.IsEnabled = true;
        }

        public async Task PurchaseUpgrade()
        {
            if (!App.FullVersionMode)
            {
                bool result = await MakePurchase(GHConstants.FullVersionProductName);
                if (result)
                {
                    Preferences.Set("FullVersion", true);
                    App.FullVersionMode = true;
                    UpdateBuyNow();
                }
            }
        }

        private void UpdateBuyNow()
        {
            if(App.FullVersionMode)
            {
                AndroidLabel.Text = "Full Version";
                BuyNowGrid.IsVisible = false;
            }
            else
            {
                AndroidLabel.Text = "Demo Version";
                BuyNowGrid.IsVisible = true;
            }
        }
    }
}

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
                App.DebugWriteProfilingStopwatchTimeAndRestart("MainPage First Time");
                _firsttime = false;
                _banksAcquired = 0;
                _downloadresult = -1;
                StartFadeLogoIn();
                Assembly thisassembly = GetType().GetTypeInfo().Assembly;
                FmodLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.FMOD-Logo-192-White.png", thisassembly);
                StartLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                MainLogoImage.Source = ImageSource.FromResource("GnollHackClient.Assets.gnollhack-logo-test-2.png", thisassembly);
                App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Start Timer");
                Device.StartTimer(TimeSpan.FromSeconds(1f / 4), () =>
                {
                    bool res = false;
                    switch (starttimercount)
                    {
                        case 0:
                            App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer "+ starttimercount);
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
                            starttimercount++;
                            res = true;
                            break;
                        case 1:
                            App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer " + starttimercount);
                            AcquireBanks();
                            starttimercount++;
                            res = true;
                            break;
                        case 2:
                            App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer " + starttimercount);
                            if (_banksAcquired >= GHConstants.NumBanks || _downloadresult > 0)
                            {
                                Preferences.Set("ResetBanks", false);
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
                                App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer Done Banks Ok" + starttimercount);
                                starttimercount++;
                            }
                            else
                            {
                                if (DownloadGrid.IsVisible)
                                {
                                    UpdateDownloadProgress();
                                    App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer Done Update Progress" + starttimercount);
                                }
                                else
                                {
                                    App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer Done Update Nothing" + starttimercount);
                                }
                            }
                            res = true;
                            break;
                        case 3:
                            App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer " + starttimercount);
                            StartFadeIn();
                            res = true;
                            starttimercount++;
                            break;
                        default:
                            App.DebugWriteProfilingStopwatchTimeAndStart("MainPage Timer " + starttimercount);
                            StartLogoImage.IsVisible = false;
                            FmodLogoImage.IsVisible = false;
                            starttimercount++;
                            break;
                    }
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

        private async void UpdateDownloadProgress()
        {
            float percentage;
            long bytesDownloaded;
            long? totalFileSize;
            string fileDescription;
            lock (_downloadProgressLock)
            {
                percentage = _downloadProgressPercentage;
                bytesDownloaded = _downloadProgressTotalBytesDownloaded;
                totalFileSize = _downloadProgressTotalFileSize;
                fileDescription = _downloadProgressFileDescription;
            }

            if (totalFileSize != null)
            {
                await DownloadProgressBar.ProgressTo((float)percentage / 100, 100, Easing.Linear);
                Device.BeginInvokeOnMainThread(() =>
                {
                    DownloadFileNameLabel.Text = fileDescription;
                    if (totalFileSize <= 0)
                        DownloadBytesLabel.Text = "Calculating file size...";
                    else
                        DownloadBytesLabel.Text = bytesDownloaded + " / " + totalFileSize + " bytes";
                });
            }

            if ((((totalFileSize > 0 && bytesDownloaded >= totalFileSize) || totalFileSize == null || (_cancellationToken.CanBeCanceled && _cancellationToken.IsCancellationRequested)) && DownloadGrid.IsVisible))
            {
                Device.BeginInvokeOnMainThread(() =>
                {
                    DownloadGrid.IsVisible = false;
                });
                _banksAcquired++;
                if(_cancellationToken.IsCancellationRequested)
                {
                    /* Delete cancelled files */
                    if(File.Exists(_downloadProgressFilePath))
                    {
                        FileInfo file = new FileInfo(_downloadProgressFilePath);
                        file.Delete();
                    }
                }
                if(_cancellationTokenSource != null)
                {
                    _cancellationTokenSource.Dispose();
                    _cancellationTokenSource = null;
                }
            }
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
                Preferences.Set("ResetBanks", true);
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

        private int _banksAcquired = 0;
        private object _downloadProgressLock = new object();
        private float _downloadProgressPercentage = 0.0f;
        private long _downloadProgressTotalBytesDownloaded = 0;
        private long? _downloadProgressTotalFileSize = 0;
        private string _downloadProgressFileDescription = "";
        private string _downloadProgressFilePath = "";

        private void AcquireBanks()
        {
            string bank_dir = App.FmodService.GetBankDir();
            Assembly assembly = GetType().GetTypeInfo().Assembly;

            /* Make the relevant directory */
            if (!Directory.Exists(bank_dir))
            {
                Directory.CreateDirectory(bank_dir);
            }

            bool reset = Preferences.Get("ResetBanks", true);
            if (reset)
            {
                DirectoryInfo di = new DirectoryInfo(bank_dir);
                foreach (FileInfo file in di.GetFiles())
                {
                    file.Delete();
                }
            }

            string[] banknamelist = { "Master.bank", "Master.strings.bank", "Auxiliary.bank" };
            string[] bankresourcelist = { "GnollHackClient.Assets.Master.bank", "GnollHackClient.Assets.Master.strings.bank", "GnollHackClient.Assets.Auxiliary.bank" };
#if DEBUG
            bool[] bankwebdownloadlist = { false, false, GHConstants.DownloadFromWebInDebugMode };
#else
            bool[] bankwebdownloadlist = { false, false, true };
#endif
            App.DebugWriteProfilingStopwatchTimeAndStart("Start Acquiring Banks");
            for (int idx = 0; idx < banknamelist.Length; idx++)
            {
                string bank_path = Path.Combine(bank_dir, banknamelist[idx]);
                if (bankwebdownloadlist[idx])
                {
                    DownloadFileFromWebServer(assembly, banknamelist[idx], bank_dir);
                }
                else
                {
                    if (!File.Exists(bank_path))
                    {
                        using (Stream stream = assembly.GetManifestResourceStream(bankresourcelist[idx]))
                        {
                            using (var fileStream = File.Create(bank_path))
                            {
                                stream.CopyTo(fileStream);
                            }
                        }
                    }
                    _banksAcquired++;
                }
                App.DebugWriteProfilingStopwatchTimeAndStart("Acquired Bank "+ idx);
            }
        }


        private void CancelDownloadButton_Clicked(object sender, EventArgs e)
        {
            _cancellationTokenSource?.Cancel();
        }

        //private static HttpClient _httpClient = new HttpClient();
        public static int _downloadresult = -1;
        CancellationTokenSource _cancellationTokenSource= null;
        CancellationToken _cancellationToken;

        public async void DownloadFileFromWebServer(Assembly assembly, string filename, string target_directory)
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
                    _downloadresult = 1; /* cannot find secrets.json */
                    _banksAcquired++;
                    return;
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
                _downloadresult = 2;
                _banksAcquired++;
                return;
            }

            /* Check if ok */
            if(File.Exists(target_path))
            {
                FileInfo file = new FileInfo(target_path);
                bool isfileok = true;
                if (file.Length != f.length)
                    isfileok = false;
                if(isfileok)
                {
                    App.DebugWriteProfilingStopwatchTimeAndStart("Begin Checksum");
                    string checksum = ChecksumUtil.GetChecksum(HashingAlgoTypes.SHA256, target_path);
                    App.DebugWriteProfilingStopwatchTimeAndStart("Finish Checksum");
                    if(checksum != f.sha256)
                        isfileok = false;
                }
                if (isfileok)
                {
                    /* Ok, no need to download */
                    _banksAcquired++;
                    _downloadresult = 0;
                    App.DebugWriteProfilingStopwatchTimeAndStart("Checksum ok, exiting");
                    return;
                }
                else
                {
                    /* Wrong, delete and download */
                    file.Delete();
                }
            }

            /* Download */
            Device.BeginInvokeOnMainThread(() =>
            {
                DownloadGrid.IsVisible = true;
            });

            string url = f.download_url;
            lock (_downloadProgressLock)
            {
                _downloadProgressFileDescription = f.description;
                _downloadProgressFilePath = target_path;
            }
            _cancellationTokenSource = new CancellationTokenSource();
            _cancellationToken = _cancellationTokenSource.Token;

            App.DebugWriteProfilingStopwatchTimeAndStart("Begin Http Download " + filename);
            using (var client = new HttpClientDownloadWithProgress(url, target_path, _cancellationToken))
            {
                client.ProgressChanged += (totalFileSize, totalBytesDownloaded, progressPercentage) => {
                    lock(_downloadProgressLock)
                    {
                        _downloadProgressPercentage = (float)progressPercentage;
                        _downloadProgressTotalBytesDownloaded = totalBytesDownloaded;
                        _downloadProgressTotalFileSize = totalFileSize;
                    }
                };

                await client.StartDownload();
            }

            //var response = await _httpClient.GetAsync(url);
            //using (var stream = await response.Content.ReadAsStreamAsync())
            //{
            //    var fileInfo = new FileInfo(target_path);
            //    using (var fileStream = fileInfo.OpenWrite())
            //    {
            //        await stream.CopyToAsync(fileStream);
            //    }
            //}

            _downloadresult = 0;
        }


        public async Task<bool> MakePurchase(string productId)
        {
            if (!CrossInAppBilling.IsSupported)
                return false;

            var billing = CrossInAppBilling.Current;
            try
            {
                var connected = await billing.ConnectAsync();
                if (!connected)
                    return false;

                //check purchases
                var purchase = await billing.PurchaseAsync(productId, ItemType.InAppPurchase);

                //possibility that a null came through.
                if (purchase == null)
                {
                    //did not purchase
                }
                else if (purchase.State == PurchaseState.Purchased)
                {
                    //purchased!
                    if (Device.RuntimePlatform == Device.Android)
                    {
                        // Must call AcknowledgePurchaseAsync else the purchase will be refunded
                        await billing.AcknowledgePurchaseAsync(purchase.PurchaseToken);
                    }
                }
            }
            catch (InAppBillingPurchaseException purchaseEx)
            {
                //Billing Exception handle this based on the type
                Debug.WriteLine("Error: " + purchaseEx);
            }
            catch (Exception ex)
            {
                //Something else has gone wrong, log it
                Debug.WriteLine("Issue connecting: " + ex);
            }
            finally
            {
                await billing.DisconnectAsync();
            }
            return true;
        }
    }
}

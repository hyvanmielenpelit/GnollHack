using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SkiaSharp;

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
using System.Collections;
using System.Collections.Concurrent;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Forms;
using SkiaSharp.Views.Forms;
using GnollHackX.Pages.Game;
using GnollHackX.Pages.MainScreen;
using Xamarin.Essentials;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace GnollHackX
#endif
{
    public partial class MainPage : ContentPage
    {
        private object _gameStartedLock = new object();
        private bool _gameStarted = false;
        public bool GameStarted { get { lock (_gameStartedLock) { return _gameStarted; } } set { lock (_gameStartedLock) { _gameStarted = value; } } }

        private object _generalTimerLock = new object();
        private bool _generaTimerIsOn = false;
        public bool GeneralTimerIsOn { get { lock (_generalTimerLock) { return _generaTimerIsOn; } } set { lock (_generalTimerLock) { _generaTimerIsOn = value; } } }
        public bool CheckAndSetGeneralTimerIsOn { get { lock (_generalTimerLock) { bool oldval = _generaTimerIsOn; _generaTimerIsOn = true; return oldval; } } }

        private object _generalTimerWorkOnTasksLock = new object();
        private bool _generaTimerWorkOnTasks = false;
        public bool GeneralTimerWorkOnTasks { get { lock (_generalTimerWorkOnTasksLock) { return _generaTimerWorkOnTasks; } } set { lock (_generalTimerWorkOnTasksLock) { _generaTimerWorkOnTasks = value; } } }
        public bool CheckAndSetGeneralTimerWorkOnTasks { get { lock (_generalTimerWorkOnTasksLock) { bool oldval = _generaTimerWorkOnTasks; _generaTimerWorkOnTasks = true; return oldval; } } }

        private object _stopGeneralTimerLock = new object();
        private bool _stopGeneraTimerIsOn = false;
        public bool StopGeneralTimer { get { lock (_stopGeneralTimerLock) { return _stopGeneraTimerIsOn; } } set { lock (_stopGeneralTimerLock) { _stopGeneraTimerIsOn = value; } } }
#if GNH_MAUI
        IDispatcherTimer _generalTimer;
#endif

        public MainPage()
        {
            InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(true);
            Shell.SetNavBarIsVisible(this, false);
            _generalTimer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
            _generalTimer.Interval = TimeSpan.FromSeconds(GHConstants.MainScreenGeneralCounterIntervalInSeconds);
            _generalTimer.IsRepeating = true;
            _generalTimer.Tick += (s, e) => { if(!DoGeneralTimerTick()) _generalTimer.Stop(); };
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif
        }

        public void StartGeneralTimer()
        {
            if(!CheckAndSetGeneralTimerIsOn)
            {
                GeneralTimerTasks();
#if GNH_MAUI
                _generalTimer.Start();
#else
                Device.StartTimer(TimeSpan.FromSeconds(GHConstants.MainScreenGeneralCounterIntervalInSeconds), () =>
                {
                    return DoGeneralTimerTick();
                });
#endif
            }
        }

        private bool DoGeneralTimerTick()
        {
            if (GameStarted || StopGeneralTimer)
            {
                GeneralTimerIsOn = false;
                StopGeneralTimer = false;
                return false;
            }
            MainThread.BeginInvokeOnMainThread(() =>
            {
                GeneralTimerTasks();
            });
            return true;
        }

        private async void GeneralTimerTasks()
        {
            await GeneralTimerTasksAsync();
        }

        private async Task GeneralTimerTasksAsync()
        {
            bool hasinternet = GHApp.HasInternetAccess;
            if (!CheckAndSetGeneralTimerWorkOnTasks)
            {
                bool dopostbones = GHApp.PostingBonesFiles && GHApp.AllowBones;
                bool dopostreplays = GHApp.AutoUploadReplays;
                string directory = Path.Combine(GHApp.GHPath, GHConstants.ForumPostQueueDirectory);
                string directory2 = Path.Combine(GHApp.GHPath, GHConstants.XlogPostQueueDirectory);
                string directory3 = Path.Combine(GHApp.GHPath, GHConstants.BonesPostQueueDirectory);
                string directory4 = Path.Combine(GHApp.GHPath, GHConstants.ReplayPostQueueDirectory);
                bool has_files = Directory.Exists(directory) && Directory.GetFiles(directory)?.Length > 0;
                bool has_files2 = Directory.Exists(directory2) && Directory.GetFiles(directory2)?.Length > 0;
                bool has_files3 = Directory.Exists(directory3) && Directory.GetFiles(directory3)?.Length > 0;
                bool has_files4 = Directory.Exists(directory4) && Directory.GetFiles(directory4)?.Length > 0;
                bool missingcredentials = string.IsNullOrEmpty(GHApp.XlogUserName);
                bool incorrectcredentials = GHApp.XlogCredentialsIncorrect;
                bool missingorincorrectcredentials = missingcredentials || incorrectcredentials;
                bool postingqueueempty = _postingQueue.Count == 0;
                if (postingqueueempty
                    && (GHApp.XlogUserNameVerified || missingorincorrectcredentials || (!GHApp.PostingXlogEntries && !dopostbones && !dopostreplays))
                    && (!has_files || !GHApp.PostingGameStatus) 
                    && (!has_files2 || !GHApp.PostingXlogEntries || missingorincorrectcredentials) 
                    && (!has_files3 || !dopostbones || GameStarted || missingorincorrectcredentials)
                    && (!has_files4 || !dopostreplays || incorrectcredentials)
                    )
                {
                    StopGeneralTimer = true;
                }
                else
                {
                    if (_postingQueue.Count > 0)
                        ProcessPostingQueue(); //Saves now posts first to disk in the case app is switched off very quickly before sending is finished
                    if (hasinternet && !GHApp.XlogUserNameVerified && (GHApp.PostingXlogEntries || dopostbones || dopostreplays) && !missingorincorrectcredentials)
                        await GHApp.TryVerifyXlogUserNameAsync();
                    if (hasinternet && has_files && GHApp.PostingGameStatus)
                        await ProcessSavedPosts(0, directory, GHConstants.ForumPostFileNamePrefix);
                    if (hasinternet && has_files2 && GHApp.PostingXlogEntries && !missingorincorrectcredentials)
                        await ProcessSavedPosts(1, directory2, GHConstants.XlogPostFileNamePrefix);
                    if (hasinternet && has_files3 && dopostbones && !GameStarted && !missingorincorrectcredentials) // Do not fetch bones files while the game is on
                        await ProcessSavedPosts(2, directory3, GHConstants.BonesPostFileNamePrefix);
                    if (hasinternet && has_files4 && dopostreplays && !incorrectcredentials)
                        await ProcessSavedPosts(3, directory4, GHConstants.ReplayPostFileNamePrefix);
                }
                GeneralTimerWorkOnTasks = false;
            }
        }

        private async Task ProcessSavedPosts(int post_type, string dir, string fileprefix)
        {
            if(dir != null && Directory.Exists(dir))
            {
                string[] filepaths = Directory.GetFiles(dir);
                if (filepaths != null)
                {
                    GHApp.MaybeWriteGHLog("ProcessSavedPosts in " + dir + ": " + filepaths.Length);
                    foreach (string str in filepaths)
                    {
                        GHApp.MaybeWriteGHLog(str);
                    }
                    foreach (string filepath in filepaths)
                    {
                        if (filepath != null)
                        {
                            FileInfo fileinfo = new FileInfo(filepath);
                            if (fileinfo != null && fileinfo.Exists && (fileprefix == null || fileprefix == "" || fileinfo.Name.StartsWith(fileprefix)))
                            {
                                GHPost post = null;
                                try
                                {
                                    string json = "";
                                    using (Stream stream = File.OpenRead(filepath))
                                    {
                                        if (stream != null)
                                        {
                                            using (StreamReader sr = new StreamReader(stream))
                                            {
                                                json = sr.ReadToEnd();
                                            }
                                            post = JsonConvert.DeserializeObject<GHPost>(json);
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {
                                    GHApp.MaybeWriteGHLog("Exception occurred while reading " + filepath + ": " + ex.Message);
                                    File.Delete(filepath); // Assume corrupted
                                    post = null;
                                }

                                if (post != null)
                                {
                                    string typestr = "";
                                    try
                                    {
                                        SendResult res;
                                        switch(post_type)
                                        {
                                            case 3:
                                                typestr = "Replay";
                                                res = await GHApp.SendReplayFile(post.status_string, post.status_type, post.status_datatype, true);
                                                break;
                                            case 2:
                                                typestr = "Bones file";
                                                res = await GHApp.SendBonesFile(post.status_string, post.status_type, post.status_datatype, true);
                                                break;
                                            case 1:
                                                typestr = "Xlog";
                                                res = await GHApp.SendXLogEntry(post.status_string, post.status_datatype, post.status_type, post.attachments, true);
                                                break;
                                            case 0:
                                            default:
                                                typestr = "Forum post";
                                                res = await GHApp.SendForumPost(post.is_game_status, post.status_string, post.status_type, post.status_datatype, post.attachments, post.forcesend, true);
                                                break;
                                        }

                                        if (res.IsSuccess)
                                        {
                                            GHApp.MaybeWriteGHLog(typestr + " was sent successfully: " + filepath);
                                            File.Delete(filepath);
                                        }
                                        else
                                        {
                                            GHApp.MaybeWriteGHLog("Sending " + typestr.ToLower() + " failed: " + filepath +
                                                (!res.HasHttpStatusCode ? "" : ", StatusCode: " + (int)res.StatusCode) + " (" + res.StatusCode.ToString() + ")" +
                                                (res.Message == null ? "" : ", Message: " + res.Message));
                                        }
                                    }
                                    catch (Exception ex)
                                    {
                                        GHApp.MaybeWriteGHLog("Exception occurred while sending " + typestr == "" ? "post of unknown type" : typestr.ToLower() + ": " + ex.Message);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        private readonly ConcurrentQueue<GHPost> _postingQueue = new ConcurrentQueue<GHPost>();

        public void EnqueuePost(GHPost post)
        {
            _postingQueue.Enqueue(post);
            StartGeneralTimer();
        }

        private void ProcessPostingQueue()
        {
            while(_postingQueue.TryDequeue(out var post))
            {
                switch(post.post_type)
                {
                    case 3:
                        GHApp.SaveReplayPostToDisk(post.status_type, post.status_datatype, post.status_string);
                        break;
                    case 2:
                        //await PostBonesFileAsync(post.status_type, post.status_datatype, post.status_string);
                        GHApp.SaveBonesPostToDisk(post.status_type, post.status_datatype, post.status_string);
                        break;
                    case 1:
                        if(post.status_type == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT)
                        {
                            switch (post.status_datatype)
                            {
                                case (int)game_status_data_types.GAME_STATUS_ATTACHMENT_GENERIC:
                                    _xlogPostAttachments.Add(new GHPostAttachment(post.status_string, "application/zip", "game data", false, post.status_type, false));
                                    break;
                                case (int)game_status_data_types.GAME_STATUS_ATTACHMENT_DUMPLOG_TEXT:
                                    _xlogPostAttachments.Add(new GHPostAttachment(post.status_string, "text/plain", "dumplog", false, post.status_type, false));
                                    break;
                                case (int)game_status_data_types.GAME_STATUS_ATTACHMENT_DUMPLOG_HTML:
                                    _xlogPostAttachments.Add(new GHPostAttachment(post.status_string, "text/html", "HTML dumplog", false, post.status_type, false));
                                    break;
                            }
                        }
                        else
                        {
                            GHApp.SaveXLogEntryToDisk(post.status_type, post.status_datatype, post.status_string, _xlogPostAttachments);
                            _xlogPostAttachments.Clear();
                        }
                        break;
                    case 0:
                    default:
                        if (post.is_game_status && post.status_type == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT
                            || (!post.is_game_status && post.status_type == (int)diagnostic_data_types.DIAGNOSTIC_DATA_ATTACHMENT)
                            || (!post.is_game_status && post.status_type == (int)diagnostic_data_types.DIAGNOSTIC_DATA_CREATE_ATTACHMENT_FROM_TEXT))
                        {
                            AddForumPostAttachment(post.is_game_status, post.status_type, post.status_datatype, post.status_string);
                        }
                        else
                        {
                            GHApp.SaveForumPostToDisk(post.is_game_status, post.status_type, post.status_datatype, 
                                post.is_game_status ? GHApp.AddForumPostInfo(post.status_string) : GHApp.AddDiagnosticInfo(GHApp.AddForumPostInfo(post.status_string), post.status_type), 
                                _forumPostAttachments, post.forcesend);
                            _forumPostAttachments.Clear();
                        }
                        break;
                }
            }
        }

        private void AddForumPostAttachment(bool is_game_status, int status_type, int status_datatype, string status_string)
        {
            if (is_game_status && status_string != null && status_string != "" && status_type == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT)
            {
                switch (status_datatype)
                {
                    case (int)game_status_data_types.GAME_STATUS_ATTACHMENT_GENERIC:
                        _forumPostAttachments.Add(new GHPostAttachment(status_string, "application/zip", "game data", !is_game_status, status_type, false));
                        break;
                    case (int)game_status_data_types.GAME_STATUS_ATTACHMENT_DUMPLOG_TEXT:
                        _forumPostAttachments.Add(new GHPostAttachment(status_string, "text/plain", "dumplog", !is_game_status, status_type, false));
                        break;
                    case (int)game_status_data_types.GAME_STATUS_ATTACHMENT_DUMPLOG_HTML:
                        _forumPostAttachments.Add(new GHPostAttachment(status_string, "text/html", "HTML dumplog", !is_game_status, status_type, false));
                        break;
                }
            }
            else if (!is_game_status && status_string != null && status_string != "" && status_type == (int)diagnostic_data_types.DIAGNOSTIC_DATA_ATTACHMENT)
            {
                switch (status_datatype)
                {
                    case (int)diagnostic_data_attachment_types.DIAGNOSTIC_DATA_ATTACHMENT_GENERIC:
                        _forumPostAttachments.Add(new GHPostAttachment(status_string, "application/zip", "diagnostic data", !is_game_status, status_type, false));
                        break;
                    case (int)diagnostic_data_attachment_types.DIAGNOSTIC_DATA_ATTACHMENT_FILE_DESCRIPTOR_LIST:
                        _forumPostAttachments.Add(new GHPostAttachment(status_string, "text/plain", "file descriptor list", !is_game_status, status_type, true));
                        break;
                }
            }
            else if (!is_game_status && status_string != null && status_string != "" && status_type == (int)diagnostic_data_types.DIAGNOSTIC_DATA_CREATE_ATTACHMENT_FROM_TEXT)
            {
                if (status_datatype == (int)diagnostic_data_attachment_types.DIAGNOSTIC_DATA_ATTACHMENT_FILE_DESCRIPTOR_LIST)
                {
                    status_string = status_string.Replace(" | ", Environment.NewLine);
                    status_string = status_string.Replace("◙", Environment.NewLine);
                }

                string tempdirpath = Path.Combine(GHApp.GHPath, GHConstants.UploadDirectory);
                if (!Directory.Exists(tempdirpath))
                    GHApp.CheckCreateDirectory(tempdirpath);
                int number = 0;
                string temp_string;
                do
                {
                    temp_string = Path.Combine(tempdirpath, "tmp_attachment_" + number + ".txt");
                    number++;
                } while (File.Exists(temp_string));

                GHApp.GnollHackService.Chmod(tempdirpath, (uint)ChmodPermissions.S_IALL);
                try
                {
                    using (FileStream fs = new FileStream(temp_string, FileMode.Create))
                    {
                        using (StreamWriter sw = new StreamWriter(fs))
                        {
                            sw.Write(status_string);
                        }
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
                _forumPostAttachments.Add(new GHPostAttachment(temp_string, "text/plain", "diagnostic data", !is_game_status, status_type, true));
            }
        }

        private List<GHPostAttachment> _forumPostAttachments = new List<GHPostAttachment>();
        private List<GHPostAttachment> _xlogPostAttachments = new List<GHPostAttachment>();

        public void ActivateLocalGameButton()
        {
            StartLocalGrid.IsEnabled = true;
            StartLocalGameButton.TextColor = GHColors.White;
        }

        private async void StartLocalGameButton_Clicked(object sender, EventArgs e)
        {
            StartLocalGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            StartLocalGameButton.TextColor = GHColors.Gray;
            carouselView.Stop();

            long numberofgames = Preferences.Get("NumberOfGames", 0L);
            Preferences.Set("NumberOfGames", numberofgames + 1L);

            var gamePage = new GamePage(this);
            GHApp.CurrentGamePage = gamePage;
            gamePage.EnableWizardMode = wizardModeSwitch.IsToggled;
            gamePage.EnableCasualMode = casualModeSwitch.IsToggled;
            gamePage.EnableModernMode = !classicModeSwitch.IsToggled;
            await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
            gamePage.StartNewGame();
        }

        private bool _firsttime = true;
        private bool _mainScreenMusicStarted = false;

        public void UpdateLayout()
        {
            wizardModeGrid.IsVisible = GHApp.DeveloperMode;
            if (!GHApp.DeveloperMode)
                wizardModeSwitch.IsToggled = false;
            ResetButton.IsVisible = GHApp.DeveloperMode;
            OptionsButton.IsVisible = GHApp.DeveloperMode;
            classicModeSwitch.IsToggled = GHApp.ClassicMode;
            casualModeSwitch.IsToggled = GHApp.CasualMode;

            UpdateMobileVersionLabel();
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            UpdateLayout();
            if (_firsttime)
            {
                GHApp.DebugWriteProfilingStopwatchTimeAndRestart("MainPage First Time");
                _firsttime = false;
                if (GHApp.PlatformService != null)
                {
                    bool removeanimationson = GHApp.PlatformService.IsRemoveAnimationsOn();
                    if(removeanimationson)
                    {
                        await DisplayAlert("Invalid Animator Duration Scale",
                            "GnollHack has detected invalid animator settings. If your device has a setting named \"Remove Animations\" under Settings -> Accessibility -> Visibility Enhancements, this setting must be set to Off. If your device does not have this setting, please manually adjust the value of \"Animator duration scale\" to 1x under Settings -> Developer Options -> Animator duration scale. ", "OK");
                        await CloseApp();
                    }
                    else
                    {
                        float scalesetting = GHApp.PlatformService.GetAnimatorDurationScaleSetting();
                        float scalecurrent = GHApp.PlatformService.GetCurrentAnimatorDurationScale();
                        if (scalecurrent == 0.0f)
                        {
                            if (scalesetting == 0.0f)
                                await DisplayAlert("Invalid Animator Duration Scale",
                                    "GnollHack failed to automatically adjust Animator Duration Scale and it remains set to Off. Please manually adjust the value to 1x under Settings -> Developer Options -> Animator duration scale. If your device has a setting named \"Remove Animations\" under Settings -> Accessibility -> Visibility Enhancements, this setting needs to be disabled, too.", "OK");
                            else
                                await DisplayAlert("Invalid Animator Duration Scale",
                                    "GnollHack failed to automatically adjust Animator Duration Scale and it has become turned Off. Please check that the value is 1x under Settings -> Developer Options -> Animator duration scale. If your device has a setting named \"Remove Animations\" under Settings -> Accessibility -> Visibility Enhancements, this setting needs to be disabled, too.", "OK");
                            await CloseApp();
                        }
                        else if (scalecurrent == -1.0f)
                        {
                            /* GnollHack could not determine current animator duration scale */
                        }
                    }
                }

                await StartFadeLogoIn();
                await StartUpTasks();
                await StartFadeIn();
                StartLogoImage.IsVisible = false;
                FmodLogoImage.IsVisible = false;

                bool previousInformationShown = false;
                if (GHApp.InformAboutGameTermination && (GHApp.DebugLogMessages ||  GHApp.GameSaveStatus == 0))
                {
                    GHApp.InformAboutGameTermination = false;
                    await DisplayAlert("Unexpected Game Termination", "GnollHack was unexpectedly terminated when running on background. This may have been instructed by the operating system or the user." + (GHApp.GameSaveStatus == 0 ? " Your game may be recoverable from the crash." : " Your game was saved before the termination."), "OK");
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutIncompatibleSavedGames)
                {
                    GHApp.InformAboutIncompatibleSavedGames = false;
                    await DisplayAlert("Incompatible Saved Games", "GnollHack has been updated to a newer version, for which your existing saved games are incompatible. To downgrade back to the earlier version, back up first your save files using About -> Export Saved Games and then follow the instructions at About -> Downgrade.", "OK");
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutSlowSounds)
                {
                    await DisplayAlert("Slow Sounds", "GnollHack is running on Android in Debug Mode using the APK format, which causes sounds to play slow. Please switch Streaming Banks to Memory on in Settings.", "OK");
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutRecordingSetOff)
                {
                    await DisplayAlert("Recording Switched Off", string.Format("You are are running low on free disk space ({0:0.00} GB). Game recording has been switched off in Settings." + (GHApp.InformAboutFreeDiskSpace ? " Please consider freeing disk space on your device." : ""), (double)GHConstants.LowFreeDiskSpaceThresholdInBytes / (1024 * 1024 * 1024)), "OK");
                    previousInformationShown = true;
                }
                else if (GHApp.InformAboutFreeDiskSpace)
                {
                    await DisplayAlert("Very Low Free Disk Space", string.Format("You are are running very low on free disk space ({0:0.00} GB). Please consider freeing disk space on your device.", (double)GHConstants.VeryLowFreeDiskSpaceThresholdInBytes / (1024 * 1024 * 1024)), "OK");
                    previousInformationShown = true;
                }
                if (!previousInformationShown)
                {
                    bool ReviewRequested = Preferences.Get("StoreReviewRequested", false);
                    long NumberOfGames = Preferences.Get("NumberOfGames", 0L);
                    long TotalPlayTime = Preferences.Get("RealPlayTime", 0L);

                    if (!ReviewRequested && ((NumberOfGames >= GHConstants.StoreReviewRequestNumberOfGames && TotalPlayTime >= GHConstants.StoreReviewRequestTotalPlayTime) || GHApp.DeveloperMode))
                    {
                        Preferences.Set("StoreReviewRequested", true);
                        UpperButtonGrid.IsEnabled = true; /* Just in case of a hangup */
                        LogoGrid.IsEnabled = true; /* Just in case of a hangup */
                        GHApp.PlatformService?.RequestAppReview(this); /* Platform implementation is async, so this should return immediately */
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

            GHApp.InitializeConnectivity();
            StartGeneralTimer();
        }

        public async Task InitializeServices()
        {
            bool resetFiles = Preferences.Get("ResetAtStart", true);
            if (resetFiles)
            {
                GHApp.GnollHackService.ClearFiles();
                Preferences.Set("ResetAtStart", false);
                Preferences.Set("ResetExternalFiles", true);
            }
            GHApp.ResetAcquiredFiles();

            await TryInitializeGnollHack();
            await TryInitializeSecrets();
            await TryInitializeFMOD();
        }

        public async Task TryReadSecrets()
        {
            try
            {
                GHApp.ReadSecrets();
                Array.Sort<SecretsFile>(GHApp.CurrentSecrets.files, new SecretsFileSizeComparer());
            }
            catch (Exception ex)
            {
                await DisplayAlert("Reading Secrets File Failed", "GnollHack failed to read secrets file: " + ex.Message, "OK");
            }
            try
            {
                GHApp.ReadUserSecrets();
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
                GHApp.GnollHackService.InitializeGnollHack();
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
                GHApp.GnollHackService.InitializeSecrets(GHApp.CurrentSecrets);
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
                GHApp.FmodService.InitializeFmod();
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
                GHApp.GnollHackService.ClearCoreFiles();
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
                GHApp.FmodService.LoadBanks(subType);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Loading FMOD Banks Failed", "GnollHack failed to load FMOD banks of type " + subType + ": " + ex.Message, "OK");
            }
        }

        private async Task StartUpTasks()
        {
            GHApp.InitFileDescriptors();

            await TryReadSecrets();
            await InitializeServices();

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            GHApp.InitAdditionalTypefaces(assembly);
            GHApp.InitAdditionalCachedBitmaps(assembly);
            GHApp.InitSymbolBitmaps(assembly);
            GHApp.InitGameBitmaps(assembly);
            carouselView.Init();

            string verstr = "?";
            string verid = "?";
            string fmodverstr = "?";
            string skiaverstr = "?";
            string skiasharpverstr = "?";
            ulong vernum = 0UL;
            ulong vercompat = 0UL;
            bool isdebug = false;
            try
            {
                verstr = GHApp.GnollHackService.GetVersionString();
                verid = GHApp.GnollHackService.GetVersionId();
                vernum = GHApp.GnollHackService.GetVersionNumber();
                vercompat = GHApp.GnollHackService.GetVersionCompatibility();
                isdebug = GHApp.GnollHackService.IsDebug();
                fmodverstr = GHApp.FmodService.GetVersionString();
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
            GHApp.GHVersionString = verstr;
            GHApp.GHVersionNumber = vernum;
            GHApp.GHVersionCompatibility = vercompat;
            GHApp.GHVersionId = verid;
            GHApp.GHDebug = isdebug;
            GHApp.FMODVersionString = fmodverstr;
            GHApp.SkiaVersionString = skiaverstr;
            GHApp.SkiaSharpVersionString = skiasharpverstr;

            VersionLabel.Text = verid;
            GnollHackLabel.Text = "GnollHack";

            string prev_version = Preferences.Get("VersionId", "");
            ulong prev_vernum = (ulong)Preferences.Get("VersionNumber", 0L);
            GHApp.GHPreviousVersionNumber = prev_vernum;

            if (prev_version != verid || prev_vernum != vernum)
            {
                await TryClearCoreFiles();
                await TryInitializeGnollHack();
                await TryInitializeSecrets();
            }
            if (prev_vernum > 0 && prev_vernum < vercompat)
                GHApp.CheckForIncompatibleSavedGames();

            Preferences.Set("VersionId", verid);
            Preferences.Set("VersionNumber", (long)vernum);

            try
            {
                /* Clean the archive temp directory */
                string archive_path = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
                if (Directory.Exists(archive_path))
                    Directory.Delete(archive_path, true);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            GHApp.PlatformService.OnDemandPackStatusNotification += OnDemandPackEventHandler;
            StartFetchOnDemandFiles();

            GHApp.SetSoundBanksUpForLoading();
            if (GHApp.LoadBanks)
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
                GHApp.FmodService.AdjustVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume);
                if (GHApp.LoadBanks)
                    GHApp.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);

                /* Check silent mode; this also mutes everything if need be */
                GHApp.SilentMode = Preferences.Get("SilentMode", false);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            GHApp.DebugCheckCurrentFileDescriptor("StartUpTasksFinish");
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
            if(GHApp.IsAndroid && GHApp.DownloadOnDemandPackage)
            {
                DownloadLabel.Text = "";
                string path = GHApp.PlatformService.GetAbsoluteOnDemandAssetPath(GHConstants.OnDemandPackName);
                if(path == null) 
                {
                    int res = GHApp.PlatformService.FetchOnDemandPack(GHConstants.OnDemandPackName);
                }
            }
        }

        public void PlayMainScreenVideoAndMusic()
        {
            carouselView.IsVisible = true;
            carouselView.Play();

            try
            {
                GHApp.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);
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

        private async void ExitAppButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool hideautoupdatealert = Preferences.Get("HideAutoUpdateAlert", false);
            bool isfromgoogleplay = true;
            if(!hideautoupdatealert)
            {
                _popupStyle = popup_style.DisableAutoUpdate;
                PopupCheckBoxLayout.IsVisible = true;
                PopupTitleLabel.TextColor = GHColors.Red;
                PopupTitleLabel.Text = "Auto-Update Warning";
                if (GHApp.IsAndroid && isfromgoogleplay)
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you turn off Auto-Update from Google Play Store for GnollHack and manually apply updates when you have no saved games.";
                else if (GHApp.IsiOS)
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates by toggling off App Updates under App Store section in the Settings app, and manually apply updates when you have no saved games.";
                else
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates in your device settings and manually apply updates when you have no saved games.";
                PopupGrid.IsVisible = true;
            }
            else
            {
                ExitAppButton.IsEnabled = false;
                ExitAppButton.TextColor = GHColors.Red;
                await CloseApp();
                /* Should not come here */
                ExitAppButton.TextColor = GHColors.White;
                ExitAppButton.IsEnabled = true;
            }
        }

        private async Task CloseApp()
        {
            StopGeneralTimer = true; /* Stop timer */
            Task t1 = GeneralTimerTasksAsync(); /* Make sure outstanding queues are processed before closing application */
            Task t2 = Task.Delay(1000); /* Give 1 second to close at maximum */
            await Task.WhenAny(t1, t2);
            GHApp.FmodService.StopAllSounds((uint)StopSoundFlags.All, 0U);
            await Task.Delay(200);
            GHApp.PlatformService.CloseApplication();

#if !GNH_MAUI
            await Task.Delay(100);
            try
            {
                Xamarin.Forms.Application.Current.Quit();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            await Task.Delay(100);
            try
            {
                Process.GetCurrentProcess().Kill();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
#endif
        }

        private async void ResetButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            StopGeneralTimer = true;
            var resetPage = new ResetPage();
            resetPage.Disappearing += (sender2, e2) =>
            {
                carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(resetPage);
            StopGeneralTimer = false;
            StartGeneralTimer();
            UpperButtonGrid.IsEnabled = true;
        }

        private async void SettingsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
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
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "defaults.gnh");
            var editorPage = new EditorPage(fulltargetpath, "Default Options File");
            string errormsg = "";
            if (!editorPage.ReadFile(out errormsg))
            {
                await DisplayAlert("Reading Options File Failed", "GnollHack failed to read the options file: " + errormsg, "OK");
                UpperButtonGrid.IsEnabled = true;
            }
            else
            {
                editorPage.Disappearing += (sender2, e2) =>
                {
                    carouselView.Play();
                    UpperButtonGrid.IsEnabled = true;
                };
                carouselView.Stop();
                await App.Current.MainPage.Navigation.PushModalAsync(editorPage);
            }
            UpperButtonGrid.IsEnabled = true;
        }

        private async void CreditsButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            StopGeneralTimer = true;
            var aboutPage = new AboutPage(this);
            aboutPage.Disappearing += (sender2, e2) =>
            {
                carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(aboutPage);
            StopGeneralTimer = false;
            StartGeneralTimer();
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

        private async void VaultButton_Clicked(object sender, EventArgs e)
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            var vaultPage = new VaultPage(this);
            vaultPage.Disappearing += (sender2, e2) =>
            {
                carouselView.Play();
                UpperButtonGrid.IsEnabled = true;
            };
            await App.Current.MainPage.Navigation.PushModalAsync(vaultPage);
            UpperButtonGrid.IsEnabled = true;
        }

        public void PlayCarouselView()
        {
            carouselView.Play();
        }

        private void UpdateMobileVersionLabel()
        {
            MobileVersionLabel.Text = GHApp.RuntimePlatform + " Version";
        }

        private void ClassicModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            GHApp.ClassicMode = e.Value;
            Preferences.Set("ClassicMode", GHApp.ClassicMode);
        }

        private void CasualModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            GHApp.CasualMode = e.Value;
            Preferences.Set("CasualMode", GHApp.CasualMode);
        }

        private enum popup_style
        {
            GeneralDialog = 0,
            DisableAutoUpdate
        }

        private popup_style _popupStyle = popup_style.GeneralDialog;
        private async void PopupOkButton_Clicked(object sender, EventArgs e)
        {
            PopupOkButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if(_popupStyle == popup_style.DisableAutoUpdate)
            {
                if (PopupNoAgainCheckBox.IsChecked)
                {
                    Preferences.Set("HideAutoUpdateAlert", true);
                    await Task.Delay(50);
                }

                await CloseApp();
            }
            PopupGrid.IsVisible = false;
            PopupOkButton.IsEnabled = true;
        }

        private void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
            PopupTitleLabel.Text = "Classic Mode";
            PopupLabel.Text = "In Classic Mode, a character's death is permanent. The resulting score is listed in top scores. A dead character may arise as a ghost.\n\nIn Modern Mode, your god will revive you at the starting altar, or at another special location. Each such revival will reduce your game score, which will be recorded in top scores upon quitting or winning the game.";
            PopupGrid.IsVisible = true;
        }

        private void CasualTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
            PopupTitleLabel.Text = "Casual Mode";
            PopupLabel.Text = "Casual Mode is a non-scoring game mode in which your saved games will not be deleted after loading, enabling you to load them again after quitting or dying. Games in Casual Mode are recorded in top scores with zero score upon winning the game.";
            PopupGrid.IsVisible = true;
        }

        private void WizTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = GHColors.LightBlue;
            PopupTitleLabel.Text = "Wizard Mode";
            PopupLabel.Text = "Wizard Mode is a debug mode that makes you immortal and enables you to issue special wizard mode commands.";
            PopupGrid.IsVisible = true;
        }

        private void wizardModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {

        }

        private void PopupLabelTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            PopupNoAgainCheckBox.IsChecked = !PopupNoAgainCheckBox.IsChecked;
        }

        public void Suspend()
        {
            carouselView.Stop();
            StopGeneralTimer = true;
        }

        public void Resume()
        {
            if(GHApp.CurrentGamePage == null)
            {
                carouselView.Play();
            }
            StopGeneralTimer = false;
            StartGeneralTimer();
        }
    }

}

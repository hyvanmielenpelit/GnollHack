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
using System.Text.Json;



#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;
using Newtonsoft.Json.Linq;

#if WINDOWS
using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using GnollHackM.Platforms.Windows;
#endif

namespace GnollHackM
#else
using Xamarin.Forms;
using SkiaSharp.Views.Forms;
using GnollHackX.Pages.Game;
using GnollHackX.Pages.MainScreen;
using Xamarin.Essentials;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;

namespace GnollHackX
#endif
{
    public partial class MainPage : ContentPage
    {
        //private readonly object _generalTimerLock = new object();
        private int _generaTimerIsOn = 0;
        public bool GeneralTimerIsOn { get { return Interlocked.CompareExchange(ref _generaTimerIsOn, 0, 0) != 0; } set { Interlocked.Exchange(ref _generaTimerIsOn, value ? 1 : 0); } }
        public bool CheckAndSetGeneralTimerIsOn { get { return Interlocked.Exchange(ref _generaTimerIsOn, 1) != 0; } } // { get { lock (_generalTimerLock) { bool oldval = _generaTimerIsOn; _generaTimerIsOn = true; return oldval; } } }

        //private readonly object _generalTimerWorkOnTasksLock = new object();
        private int _generaTimerWorkOnTasks = 0;
        public bool GeneralTimerWorkOnTasks { get { return Interlocked.CompareExchange(ref _generaTimerWorkOnTasks, 0, 0) != 0; } set { Interlocked.Exchange(ref _generaTimerWorkOnTasks, value ? 1 : 0); } }
        public bool CheckAndSetGeneralTimerWorkOnTasks { get { return Interlocked.Exchange(ref _generaTimerWorkOnTasks, 1) != 0; } }

        //private readonly object _stopGeneralTimerLock = new object();
        private int _stopGeneraTimerIsOn = 0;
        public bool StopGeneralTimer { get { return Interlocked.CompareExchange(ref _stopGeneraTimerIsOn, 0, 0) != 0; } set { Interlocked.Exchange(ref _stopGeneraTimerIsOn, value ? 1 : 0); } }
#if GNH_MAUI
        IDispatcherTimer _generalTimer;
#endif

        public MainPage()
        {
            GHApp.MaybeWriteGHLog("MainPage constructor.", true, GHConstants.SentryGnollHackGeneralCategoryName);
            InitializeComponent();
            GHApp.CurrentMainPage = this;
            GamePage gamePage = GHApp.CurrentGamePage;
            if (gamePage != null) /* Might be non-null and stale after a restart */
            {
                gamePage.StopCanvasAnimations();
                gamePage.StopTimers();
                gamePage.ShutDownCanvasViews();
                GHApp.DisconnectIViewHandlers(gamePage);
                GHApp.CurrentGamePage = null;
            }
            GHApp.IncrementMainConstructorRunNumber();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            if (GHApp.CurrentGHGame != null)
            {
                WaitLabel.Text = "Please Wait...";
                WaitLayout.IsVisible = true;
            }
#if WINDOWS
            classicModeGrid.HeightRequest = 40;
            casualModeGrid.HeightRequest = 40;
            wizardModeGrid.HeightRequest = 40;
            tournamentModeGrid.HeightRequest = 40;
#endif
#if GNH_MAUI
#if WINDOWS
            classicModeSwitch.HandlerChanged += (s, e) =>
            {
                var pv = classicModeSwitch.Handler?.PlatformView as ToggleSwitch;
                if (pv != null)
                {
                    pv.IsTabStop = false;
                }
            };
            casualModeSwitch.HandlerChanged += (s, e) =>
            {
                var pv = casualModeSwitch.Handler?.PlatformView as ToggleSwitch;
                if (pv != null)
                {
                    pv.IsTabStop = false;
                }
            };
            wizardModeSwitch.HandlerChanged += (s, e) =>
            {
                var pv = wizardModeSwitch.Handler?.PlatformView as ToggleSwitch;
                if (pv != null)
                {
                    pv.IsTabStop = false;
                }
            };
#endif
            Shell.SetNavBarIsVisible(this, false);
            _generalTimer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
            _generalTimer.Interval = TimeSpan.FromSeconds(GHConstants.MainScreenGeneralCounterIntervalInSeconds);
            _generalTimer.IsRepeating = true;
            _generalTimer.Tick += (s, e) => 
            { 
                bool res = DoGeneralTimerTick();
                if (!res) 
                    _generalTimer.Stop(); 
            };

            FinishedLogoFadeIn = false;
            StartStartUpTimer();

            HandlerChanged += (s, e) => {
                GHApp.DisplayRefreshRate = Math.Max(60.0f, DeviceDisplay.Current.MainDisplayInfo.RefreshRate);
                GHApp.DisplayDensity = DeviceDisplay.Current.MainDisplayInfo.Density <= 0.0 ? 1.0f : (float)DeviceDisplay.Current.MainDisplayInfo.Density;
                GHApp.LoadCustomCursor();
                UIUtils.ChangeElementCursor(RootGrid, GameCursorType.Normal);
                UIUtils.ChangeElementCursor(classicModeLabel, GameCursorType.Info);
                UIUtils.ChangeElementCursor(casualModeLabel, GameCursorType.Info);
                UIUtils.ChangeElementCursor(wizardModeLabel, GameCursorType.Info);
            };
#endif
        }

        public void StartGeneralTimer()
        {
            if (!CheckAndSetGeneralTimerIsOn)
            {
                GeneralTimerTasks();
                StartTimerCore();
            }
        }

        public async Task StartGeneralTimerAsync()
        {
            if (!CheckAndSetGeneralTimerIsOn)
            {
                await GeneralTimerTasksAsync();
                StartTimerCore();
            }
        }

        private void StartTimerCore()
        {
#if GNH_MAUI
            _generalTimer.Start();
#else
            Device.StartTimer(TimeSpan.FromSeconds(GHConstants.MainScreenGeneralCounterIntervalInSeconds), () =>
            {
                return DoGeneralTimerTick();
            });
#endif
        }

        private bool DoGeneralTimerTick()
        {
            if (GHApp.GameStarted || StopGeneralTimer)
            {
                GeneralTimerIsOn = false;
                StopGeneralTimer = false;
                return false;
            }
            GeneralTimerTasks();
            return true;
        }

        //private async Task<bool> DoGeneralTimerTickAsync()
        //{
        //    if (GameStarted || StopGeneralTimer)
        //    {
        //        GeneralTimerIsOn = false;
        //        StopGeneralTimer = false;
        //        return false;
        //    }
        //    await GeneralTimerTasksAsync();
        //    return true;
        //}

        private void GeneralTimerTasks()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        await GeneralTimerTasksAsync();
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private int _pendingGeneralTimerTasks = 0;
        private int PendingGeneralTimerTasks
        {
            get { return Interlocked.CompareExchange(ref _pendingGeneralTimerTasks, 0, 0); }
            set { Interlocked.Exchange(ref _pendingGeneralTimerTasks, value); }
        }

        private async Task GeneralTimerTasksAsync()
        {
            bool workingOnTasks = false;
            try
            {
                bool hasinternet = GHApp.HasInternetAccess;
                if (!CheckAndSetGeneralTimerWorkOnTasks)
                {
                    workingOnTasks = true;
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

                    PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
                    UpdateGeneralTimerTasksLabel(false);

                    if (postingqueueempty
                        && (GHApp.XlogUserNameVerified || missingorincorrectcredentials || (!GHApp.PostingXlogEntries && !dopostbones && !dopostreplays))
                        && (!has_files || !GHApp.PostingGameStatus)
                        && (!has_files2 || !GHApp.PostingXlogEntries || missingorincorrectcredentials)
                        && (!has_files3 || !dopostbones || GHApp.GameStarted || missingorincorrectcredentials)
                        && (!has_files4 || !dopostreplays || incorrectcredentials)
                        )
                    {
                        StopGeneralTimer = true;
                    }
                    else
                    {
                        if (_postingQueue.Count > 0)
                        {
                            ProcessPostingQueue(); /* Saves now posts first to disk in the case app is switched off very quickly before sending is finished */
                            has_files = Directory.Exists(directory) && Directory.GetFiles(directory)?.Length > 0;
                            has_files2 = Directory.Exists(directory2) && Directory.GetFiles(directory2)?.Length > 0;
                            has_files3 = Directory.Exists(directory3) && Directory.GetFiles(directory3)?.Length > 0;
                            has_files4 = Directory.Exists(directory4) && Directory.GetFiles(directory4)?.Length > 0;
                        }

                        if (hasinternet && !GHApp.XlogUserNameVerified && (GHApp.PostingXlogEntries || dopostbones || dopostreplays) && !missingorincorrectcredentials)
                            await GHApp.TryVerifyXlogUserNameAsync();

                        PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
                        UpdateGeneralTimerTasksLabel(false);

                        if (hasinternet && has_files && GHApp.PostingGameStatus)
                        {
                            await ProcessSavedPosts(0, directory, GHConstants.ForumPostFileNamePrefix);
                            PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
                            UpdateGeneralTimerTasksLabel(false);
                        }

                        if (hasinternet && has_files2 && GHApp.PostingXlogEntries && !missingorincorrectcredentials)
                        {
                            await ProcessSavedPosts(1, directory2, GHConstants.XlogPostFileNamePrefix);
                            PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
                            UpdateGeneralTimerTasksLabel(false);
                        }

                        if (hasinternet && has_files3 && dopostbones && !GHApp.GameStarted && !missingorincorrectcredentials) // Do not fetch bones files while the game is on
                        {
                            await ProcessSavedPosts(2, directory3, GHConstants.BonesPostFileNamePrefix);
                            PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
                            UpdateGeneralTimerTasksLabel(false);
                        }

                        if (hasinternet && has_files4 && dopostreplays && !incorrectcredentials)
                        {
                            await ProcessSavedPosts(3, directory4, GHConstants.ReplayPostFileNamePrefix);
                            PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
                            UpdateGeneralTimerTasksLabel(false);
                        }
                    }
                    GeneralTimerWorkOnTasks = false;
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("Exception (GeneralTimerTasks): " + ex.Message);
                if(workingOnTasks)
                    GeneralTimerWorkOnTasks = false;
            }
        }

        private int CalculatePendingGeneralTimerTasks()
        {
            bool hasinternet = GHApp.HasInternetAccess;
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

            int tasks1 = _postingQueue.Count;
            //int tasks2 = hasinternet && !GHApp.XlogUserNameVerified && (GHApp.PostingXlogEntries || dopostbones || dopostreplays) && !missingorincorrectcredentials ? 1 : 0;
            int tasks3 = hasinternet && has_files && GHApp.PostingGameStatus ? Directory.GetFiles(directory).Length : 0;
            int tasks4 = hasinternet && has_files2 && GHApp.PostingXlogEntries && !missingorincorrectcredentials ? Directory.GetFiles(directory2).Length : 0;
            int tasks5 = hasinternet && has_files3 && dopostbones && !GHApp.GameStarted && !missingorincorrectcredentials ? Directory.GetFiles(directory3).Length : 0;
            int tasks6 = hasinternet && has_files4 && dopostreplays && !incorrectcredentials ? Directory.GetFiles(directory4).Length : 0;

            return tasks1 + /* tasks2 + */ tasks3 + tasks4 + tasks5 + tasks6;
        }

        private async Task ProcessSavedPosts(int post_type, string dir, string fileprefix)
        {
            if (dir != null && Directory.Exists(dir))
            {
                string[] filepaths = Directory.GetFiles(dir);
                if (filepaths != null)
                {
                    bool firstReplay = true;
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
                                    PendingGeneralTimerTasks--;
                                    UpdateGeneralTimerTasksLabel(false);
                                }

                                if (post != null)
                                {
                                    string typestr = "";
                                    try
                                    {
                                        SendResult res;
                                        switch (post_type)
                                        {
                                            case 3:
                                                {
                                                    typestr = "Replay";
                                                    if (firstReplay)
                                                        await GHApp.CheckCreateReplayContainer(GHApp.GetAzureBlobStorageReplayContainerName());
                                                    firstReplay = false;
                                                    res = await GHApp.SendReplayFile(post.status_string, post.status_type, post.status_datatype, true);
                                                }
                                                break;
                                            case 2:
                                                typestr = "Bones file";
                                                res = await GHApp.SendBonesFile(post.status_string, post.status_type, post.status_datatype, true);
                                                break;
                                            case 1:
                                                {
                                                    typestr = "Xlog";
                                                    string adjusted_string = post.status_string;
                                                    if (!string.IsNullOrWhiteSpace(adjusted_string))
                                                        adjusted_string = adjusted_string.Replace("○", "\t").Replace("◙", Environment.NewLine).Replace(Environment.NewLine, "");
                                                    res = await GHApp.SendXLogEntry(adjusted_string, post.status_datatype, post.status_type, post.attachments, true);
                                                    CheckTopScore(res, adjusted_string);
                                                }
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
                                            PendingGeneralTimerTasks--;
                                            UpdateGeneralTimerTasksLabel(false);
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

        private void CheckTopScore(SendResult res, string xlogentry_string)
        {
            if (res.IsSuccess && res.PostResponseInfo != null && res.PostResponseInfo.TopScoreDisplayIndex > 0)
            {
                if (res.PostResponseInfo.TopScoreDisplayIndex <= 10)
                {
                    GHTopScoreItem tsi = new GHTopScoreItem(xlogentry_string);
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        _popupStyle = popup_style.GeneralDialog;
                        _popupViewUrl = res.PostResponseInfo.TopScorePageUrl;
                        _popupViewTitle = "Top Scores";
                        PopupCheckBoxLayout.IsVisible = false;
                        PopupOkButton.IsVisible = false;
                        PopupButtonGrid.IsVisible = true;
                        PopupTitleLabel.TextColor = GHColors.BrighterGreen;
                        PopupTitleLabel.Text = "Top Ten List";
                        PopupLabel.Text =
                            "You made the top ten list!" +
                            Environment.NewLine + Environment.NewLine +
                            "#" + res.PostResponseInfo.TopScoreDisplayIndex + ". " + tsi.Name + ": " + tsi.Score;
                        PopupGrid.IsVisible = true;
                    });
                }
            }
        }

        private void UpdateGeneralTimerTasksLabel(bool force)
        {
            if (force || PendingTasksGrid.IsVisible)
            {
                try
                {
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        int tasks = PendingGeneralTimerTasks;
                        PendingTasksLabel.Text = "There " + (tasks == 1 ? "is" : "are") + " " + tasks + " task" + (tasks == 1 ? "" : "s") + " pending.";
                        if (tasks <= 0)
                        {
                            PendingTasksGrid.IsEnabled = false;
                            PendingTasksGrid.IsVisible = false;
                        }
                    });
                    MainThread.BeginInvokeOnMainThread(async () =>
                    {
                        try
                        {
                            if (PendingGeneralTimerTasks <= 0)
                            {
                                await CloseApp();
                            }
                        }
                        catch (Exception ex)
                        {
                            Debug.WriteLine(ex);
                        }
                    });
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
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
            while (_postingQueue.TryDequeue(out var post))
            {
                switch (post.post_type)
                {
                    case 3:
                        GHApp.SaveReplayPostToDisk(post.status_type, post.status_datatype, post.status_string);
                        break;
                    case 2:
                        //await PostBonesFileAsync(post.status_type, post.status_datatype, post.status_string);
                        GHApp.SaveBonesPostToDisk(post.status_type, post.status_datatype, post.status_string);
                        break;
                    case 1:
                        if (post.status_type == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT)
                        {
                            lock (_attachmentLock)
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
                        }
                        else
                        {
                            List<GHPostAttachment> forumPosts = new List<GHPostAttachment>();
                            lock (_attachmentLock)
                            {
                                forumPosts.AddRange(_xlogPostAttachments);
                                _xlogPostAttachments.Clear();
                            }
                            GHApp.SaveXLogEntryToDisk(post.status_type, post.status_datatype, post.status_string, forumPosts);
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
                            List <GHPostAttachment> forumPosts = new List < GHPostAttachment >();
                            lock (_attachmentLock)
                            {
                                forumPosts.AddRange(_forumPostAttachments);
                                _forumPostAttachments.Clear();
                            }
                            GHApp.SaveForumPostToDisk(post.is_game_status, post.status_type, post.status_datatype,
                                post.is_game_status ? GHApp.AddForumPostInfo(post.status_string) : GHApp.AddDiagnosticInfo(GHApp.AddForumPostInfo(post.status_string), post.status_type),
                                forumPosts, post.forcesend);
                        }
                        break;
                }
                UpdateGeneralTimerTasksLabel(false);
            }
        }

        private void AddForumPostAttachment(bool is_game_status, int status_type, int status_datatype, string status_string)
        {
            if (is_game_status && status_string != null && status_string != "" && status_type == (int)game_status_types.GAME_STATUS_RESULT_ATTACHMENT)
            {
                lock (_attachmentLock)
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
            }
            else if (!is_game_status && status_string != null && status_string != "" && status_type == (int)diagnostic_data_types.DIAGNOSTIC_DATA_ATTACHMENT)
            {
                lock (_attachmentLock)
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
                lock (_attachmentLock)
                {
                    _forumPostAttachments.Add(new GHPostAttachment(temp_string, "text/plain", "diagnostic data", !is_game_status, status_type, true));
                }
            }
        }

        private readonly object _attachmentLock = new object();
        private List<GHPostAttachment> _forumPostAttachments = new List<GHPostAttachment>();
        private List<GHPostAttachment> _xlogPostAttachments = new List<GHPostAttachment>();

        public void ActivateLocalGameButton()
        {
            StartLocalGrid.IsEnabled = true;
            StartLocalGameButton.TextColor = GHColors.White;
        }

        private async void StartLocalGameButton_Clicked(object sender, EventArgs e)
        {
            await StartLocalGame();
        }

        public async Task StartLocalGame()
        {
            try
            {
                StartLocalGrid.IsEnabled = false;
                GHApp.PlayButtonClickedSound();

                if (GHApp.TournamentMode)
                {
                    if (!GHApp.HasInternetAccess)
                    {
                        _popupStyle = popup_style.GeneralDialog;
                        PopupCheckBoxLayout.IsVisible = false;
                        PopupTitleLabel.TextColor = GHColors.Orange;
                        PopupTitleLabel.Text = "No Internet";
                        PopupLabel.Text = "You must be connected to internet to start a Tournament game. Please make sure you have an internet connection.";
                        PopupGrid.IsVisible = true;
                        StartLocalGrid.IsEnabled = true;
                        return;
                    }

                    if (!GHApp.XlogUserNameVerified)
                    {
                        bool missingcredentials = string.IsNullOrEmpty(GHApp.XlogUserName);
                        bool incorrectcredentials = GHApp.XlogCredentialsIncorrect;
                        bool missingorincorrectcredentials = missingcredentials || incorrectcredentials;
                        if (!missingorincorrectcredentials)
                        {
                            DisplayAlertGrid("Credentials Verification", "Verifying credentials... Please wait.", "OK", GHColors.TitleGoldColor);
                            await GHApp.TryVerifyXlogUserNameAsync();
                            AlertGrid.IsVisible = false;
                        }
                        if (!GHApp.XlogUserNameVerified)
                        {
                            _popupStyle = popup_style.GeneralDialog;
                            PopupCheckBoxLayout.IsVisible = false;
                            PopupTitleLabel.TextColor = GHColors.Orange;
                            PopupTitleLabel.Text = "Tournament Verification Failed";
                            PopupLabel.Text = "User name and password for Server Posting have not been verified. Please set and verify these in Settings in the Server Posting section.";
                            PopupGrid.IsVisible = true;
                            StartLocalGrid.IsEnabled = true;
                            return;
                        }
                    }
                }

                StartLocalGameButton.TextColor = GHColors.Gray;
                //carouselView.Stop();
                carouselView.ShutDown();

                //GHGame curGame = GHApp.CurrentGHGame;
                //if (curGame != null)
                //{
                //    await TerminateAndRestartGame(curGame);
                //}
                //else
                {
                    long numberofgames = Preferences.Get("NumberOfGames", 0L);
                    Preferences.Set("NumberOfGames", numberofgames + 1L);

                    var gamePage = new GamePage(this);
                    gamePage.EnableWizardMode = wizardModeSwitch.IsToggled;
                    gamePage.EnableCasualMode = casualModeSwitch.IsToggled;
                    gamePage.EnableModernMode = !classicModeSwitch.IsToggled;
                    await GHApp.Navigation.PushModalAsync(gamePage);
                    await gamePage.StartNewGame();
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("StartLocalGameButton_Clicked: " + ex.Message);
#if SENTRY
                SentrySdk.CaptureException(ex);
#endif
                DisplayAlertGrid("Error", "Error occurred when starting the game: " + ex.Message, "OK", GHColors.Red);
            }
        }

        private bool _firsttime = true;

        public void UpdateLayout()
        {
            wizardModeGrid.IsVisible = GHApp.DeveloperMode && !GHApp.TournamentMode;
            if (!GHApp.DeveloperMode || GHApp.TournamentMode)
                wizardModeSwitch.IsToggled = false;

            tournamentModeGrid.IsVisible = GHApp.TournamentMode;

            classicModeGrid.IsVisible = !GHApp.TournamentMode;
            classicModeSwitch.IsToggled = GHApp.ClassicMode || GHApp.TournamentMode;
            casualModeGrid.IsVisible = !GHApp.TournamentMode;
            casualModeSwitch.IsToggled = GHApp.CasualMode && !GHApp.TournamentMode;

            ResetButton.IsVisible = GHApp.DeveloperMode;
            OptionsButton.IsVisible = GHApp.DeveloperMode;

            UpdateMobileVersionLabel();
        }

        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (ExitButton.IsEnabled && ExitButton.IsVisible && StartButtonLayout.IsVisible)
                await ExitApp();
            return false;
        }

        private void TerminateAndRestartGame()
        {
            GHGame curGame = GHApp.CurrentGHGame;
            if (curGame != null) 
            {
                //if (WasGameSaved)
                //{
                    carouselView.ShutDown();
                    var gamePage = new GamePage(this);
                    gamePage.EnableWizardMode = curGame.WizardMode;
                    gamePage.EnableCasualMode = curGame.CasualMode;
                    gamePage.EnableModernMode = curGame.ModernMode;
                    GHApp.CancelSaveGame = true;
                    curGame.TerminateGnollHackAndRestartGameInNewGamePage();
                //}
                //else
                //{
                    //await GHApp.Navigation.PushModalAsync(gamePage, false);
                    //gamePage.StartExistingGame();
                    //curGame.ReactivateGame();
                    //ShowWaitAndCloseApp();
                //}
            }
            WaitLayout.IsVisible = false;
        }

        private bool WasGameSaved
        {
            get
            {
                GHGame curGame = GHApp.CurrentGHGame;
                bool wenttosleep = false;
                try
                {
                    wenttosleep = Preferences.Get("WentToSleepWithGameOn", false);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }
                bool wasSaved = GHApp.IsAutoSaveUponSwitchingAppsOn
                    && curGame != null && !curGame.PlayingReplay
                    && wenttosleep && (GHApp.GameSaved || GHApp.SavingGame);
                return wasSaved;
            }
        }
        private bool AllowStartExistingGame 
        {  
            get 
            {
                return GHConstants.AllowRestartGameUponActivityDestruction && WasGameSaved && GHApp.GameStarted; 
            } 
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            UpdateLayout();
            GHGame curGame = GHApp.CurrentGHGame;
            if (_firsttime && (curGame != null || GHApp.MainPageConstructorRunNumber > 1)) // Restart
            {
                _firsttime = false;
                GHApp.SetWindowFocus();
                GnollHackLabel.Text = "GnollHack";
                FinishedLogoFadeIn = true;
                StartLogoImage.Opacity = 0.0; /* To make sure */
                StartLogoImage.IsVisible = false;
                FmodLogoImage.Opacity = 0.0; /* To make sure */
                FmodLogoImage.IsVisible = false;

                carouselView.Init();
                carouselView.IsVisible = true;
                carouselView.InvalidateSurface();
                carouselView.Opacity = 1.0; /* To make sure */
                UpperButtonGrid.IsVisible = true;
                UpperButtonGrid.Opacity = 1.0;  /* To make sure */
                StartButtonLayout.IsVisible = true;
                StartButtonLayout.Opacity = 1.0;  /* To make sure */
                LogoGrid.IsVisible = true;
                LogoGrid.Opacity = 1.0;  /* To make sure */

                if (curGame != null)
                {
                    StartLocalGameButton.TextColor = GHColors.Gray;
                    StartLocalGrid.IsEnabled = false;
                    WaitLayout.IsVisible = false;
                    carouselView.Play();

                    if (AllowStartExistingGame)
                    {
                        Preferences.Set("WentToSleepWithGameOn", false);
                        Preferences.Set("GameSaveResult", 0);
                        DisplayAlertGrid(GHApp.IsAndroid ? "Android Activity Restart" : "Game Screen Restart",
                            "Android destroyed GnollHack's " + (GHApp.IsAndroid ? "activity" : "game screen") + " when backgrounded. Please press OK to return to your game.",
                                "OK", GHColors.Orange, AppCloseStyle.ReturnToGame, true);
                    }
                    else
                    {
                        Preferences.Set("WentToSleepWithGameOn", false);
                        Preferences.Set("GameSaveResult", 0);
                        if (GHApp.IsAndroid)
                        {
                            DisplayAlertGrid("Android Activity Restart",
                                "Android destroyed GnollHack's activity when backgrounded, which may lead to unstable performance. Please press OK to exit the app and then restart GnollHack.",
                                    "OK", GHColors.Orange, AppCloseStyle.ExitApp);
                        }
                        else
                        {
                            DisplayAlertGrid("Unexpected App Restart",
                                "GnollHack experienced an unexpected app window restart, which may lead to unstable performance. Please press OK to exit the app and then restart GnollHack.",
                                    "OK", GHColors.Orange, AppCloseStyle.ExitApp);
                        }
                    }

                    return;
                }
                else
                {
                    GHApp.FmodService?.StopAllUISounds();
                    PlayMainScreenVideoAndMusic();
                }
            }
            else if (_firsttime)
            {
                _firsttime = false;
                GHApp.DebugWriteProfilingStopwatchTimeAndRestart("MainPage First Time");
                GHApp.InitializeConnectivity();
                GHApp.SetWindowFocus();

                if (GHApp.IsAndroid && !GHApp.UsePlatformRenderLoop && GHApp.PlatformService != null)
                {
                    string animationSettingName = GHApp.OneUIAnimationSettingName;
                    bool removeanimationson = GHApp.PlatformService.IsRemoveAnimationsOn();
                    if (removeanimationson)
                    {
                        DisplayAlertGrid(GHApp.IsSamsung ? animationSettingName + " is On" : "Invalid Animator Duration Scale",
                            "GnollHack has detected invalid animation settings." 
                            + (GHApp.IsPlatformRenderLoopAvailable ? " You can either turn on Platform Render Loop in GnollHack settings or adjust your device animation settings." : "") 
                            + (GHApp.IsSamsung ? " In the Android Settings app, please switch off \"" + animationSettingName + "\" under Accessibility > Visibility Enhancements." : " In the Android settings app, please adjust the value of \"Animator duration scale\" to 1x under Developer Options > Animator duration scale."),
                            "OK", GHColors.Orange, GHApp.IsPlatformRenderLoopAvailable ? AppCloseStyle.Normal : AppCloseStyle.ExitAppOnHangupAndRemoveAnimations);
                    }
                    else
                    {
                        float scalesetting = GHApp.PlatformService.GetAnimatorDurationScaleSetting();
                        float scalecurrent = GHApp.PlatformService.GetCurrentAnimatorDurationScale();
                        if (scalecurrent == 0.0f)
                        {
                            if (scalesetting == 0.0f)
                                DisplayAlertGrid("Invalid Animator Duration Scale",
                                    "GnollHack failed to automatically adjust Animator Duration Scale and it remains switched off." 
                                    + (GHApp.IsPlatformRenderLoopAvailable ? " You can either turn on Platform Render Loop in GnollHack settings or adjust your device animation settings." : "") 
                                    + (GHApp.IsAndroid ? " In the Android Settings app, please adjust the value to 1x under Developer Options > Animator duration scale. If your device has a setting named \"" + animationSettingName + "\" under Accessibility > Visibility Enhancements, this setting needs to be disabled, too." : ""),
                                    "OK", GHColors.Orange, GHApp.IsPlatformRenderLoopAvailable ? AppCloseStyle.Normal : AppCloseStyle.ExitAppOnHangupAndAnimatorDuration);
                            else
                                DisplayAlertGrid("Invalid Animator Duration Scale",
                                    "GnollHack failed to automatically adjust Animator Duration Scale and it has become turned off." 
                                    + (GHApp.IsPlatformRenderLoopAvailable ? " You can either turn on Platform Render Loop in GnollHack settings or adjust your device animation settings." : "") 
                                    + (GHApp.IsAndroid ? " In the Android Settings app, please check that the value is 1x under Developer Options > Animator duration scale. If your device has a setting named \"" + animationSettingName + "\" under Accessibility > Visibility Enhancements, this setting needs to be disabled, too." : ""),
                                    "OK", GHColors.Orange, GHApp.IsPlatformRenderLoopAvailable ? AppCloseStyle.Normal : AppCloseStyle.ExitAppOnHangupAndAnimatorDuration);
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
                if (GHApp.InformAboutGameTermination && (GHApp.DebugLogMessages || GHApp.GameSaveStatus == 0))
                {
                    GHApp.InformAboutGameTermination = false;
                    DisplayAlertGrid("Unexpected Game Termination", "GnollHack was unexpectedly terminated when running on background. This may have been instructed by the operating system or the user." + (GHApp.GameSaveStatus == 0 ? " Your game may be recoverable from the crash." : " Your game was saved before the termination."), "OK", GHColors.Orange);
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutIncompatibleSavedGames)
                {
                    GHApp.InformAboutIncompatibleSavedGames = false;
                    DisplayAlertGrid("Incompatible Saved Games", "GnollHack has been updated to a newer version, for which your existing saved games are incompatible.", "OK", GHColors.Orange); // To downgrade back to the earlier version, back up first your save files using About -> Export Saved Games and then follow the instructions at About -> Downgrade.
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutSlowSounds)
                {
                    DisplayAlertGrid("Slow Sounds", "GnollHack is running on Android in Debug Mode using the APK format, which causes sounds to play slow. Please switch Streaming Banks to Memory on in Settings.", "OK", GHColors.Orange);
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutGPU)
                {
                    if (GHApp.IsSteam)
                    {
                        DisplayAlertGrid("Multiple GPUs", "GnollHack is currently not using a high-performance GPU, which may impact performance. Please switch to a high performance graphics preference in Windows Settings App at System > Display > Graphics > Add Desktop App > the .exe file under C:\\Program Files (x86)\\Steam\\steamapps\\common\\GnollHack.", "OK", GHColors.Orange);
                    }
                    else
                    {
                        DisplayAlertGrid("Multiple GPUs", "GnollHack is currently not using a high-performance GPU, which may impact performance. Please switch to a high performance graphics preference in Windows Settings App at System > Display > Graphics > Add App > Microsoft Store App > Options.", "OK", GHColors.Orange);
                    }
                    GHApp.HasInformedAboutGPU = true;
                    previousInformationShown = true;
                }
                if (GHApp.InformAboutRecordingSetOff)
                {
                    DisplayAlertGrid("Recording Switched Off", string.Format("You are are running low on free disk space ({0:0.00} GB). Game recording has been switched off in Settings." + (GHApp.InformAboutFreeDiskSpace ? " Please consider freeing disk space on your device." : ""), (double)GHConstants.LowFreeDiskSpaceThresholdInBytes / (1024 * 1024 * 1024)), "OK", GHColors.Orange);
                    previousInformationShown = true;
                }
                else if (GHApp.InformAboutFreeDiskSpace)
                {
                    DisplayAlertGrid("Very Low Free Disk Space", string.Format("You are are running very low on free disk space ({0:0.00} GB). Please consider freeing disk space on your device.", (double)GHConstants.VeryLowFreeDiskSpaceThresholdInBytes / (1024 * 1024 * 1024)), "OK", GHColors.Orange);
                    previousInformationShown = true;
                }
                if (!previousInformationShown && GHConstants.EventAskForStarGazers && GHApp.HasInternetAccess)
                {
                    bool showStarGazers = false;
                    int star_gazers = 0;
                    try
                    {
                        bool StarGazersRequested = Preferences.Get("StarGazersRequested", false);
                        long NumberOfGames = Preferences.Get("NumberOfGames", 0L);
                        long TotalPlayTime = GHApp.RealPlayTime;

                        if (!StarGazersRequested && ((NumberOfGames >= GHConstants.StarGazerRequestNumberOfGames && TotalPlayTime >= GHConstants.StarGazerRequestTotalPlayTime) || GHApp.DeveloperMode))
                        {
                            using (HttpClient client = new HttpClient { Timeout = TimeSpan.FromSeconds(10) })
                            {
                                client.DefaultRequestHeaders.Add("User-Agent", "GnollHack");
                                HttpResponseMessage message = await client.GetAsync(new Uri(GHConstants.GnollHackiGitHubApiPage));
                                if (message != null && message.IsSuccessStatusCode && message.Content != null)
                                {
                                    string content = await message.Content.ReadAsStringAsync();
                                    if (!string.IsNullOrWhiteSpace(content))
                                    {
                                        using (JsonDocument doc = JsonDocument.Parse(content))
                                        {
                                            JsonElement root = doc.RootElement;
                                            star_gazers = root.GetProperty("stargazers_count").GetInt32();
                                            if (star_gazers > 0)
                                            {
                                                Preferences.Set("StarGazersRequested", true);
                                                if (star_gazers < 500)
                                                {
                                                    showStarGazers = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                    if (showStarGazers)
                    {
                        DisplayEventGrid("Give Us a Star on GitHub", 
                            "You can help our GitHub wiki to gain more search engine visibility by starring the GnollHack repository! When we reach 500 stars, search engines are allowed to index the wiki.\n\nWe have currently " + star_gazers + " stars.",
                            "Give Star", "Later", 1);
                    }
                }

                /* This last, since the store review may not show up */
                if (!previousInformationShown && GHApp.HasInternetAccess)
                {
                    try
                    {
                        bool ReviewRequested = Preferences.Get("StoreReviewRequested", false);
                        long NumberOfGames = Preferences.Get("NumberOfGames", 0L);
                        long TotalPlayTime = GHApp.RealPlayTime;

                        if (!ReviewRequested && ((NumberOfGames >= GHConstants.StoreReviewRequestNumberOfGames && TotalPlayTime >= GHConstants.StoreReviewRequestTotalPlayTime) || GHApp.DeveloperMode))
                        {
                            previousInformationShown = true;
                            Preferences.Set("StoreReviewRequested", true);
                            UpperButtonGrid.IsEnabled = true; /* Just in case of a hangup */
                            LogoGrid.IsEnabled = true; /* Just in case of a hangup */
                            await GHApp.PlatformService?.RequestAppReview(this); /* Platform implementation is async, so this should return immediately */
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                }
            }
            else if (!GHApp.GameStarted)
            {
                if ((!GHApp.MainScreenMusicStarted))
                    PlayMainScreenVideoAndMusic();
            }

            UpperButtonGrid.IsEnabled = true;
            LogoGrid.IsEnabled = true;

            await StartGeneralTimerAsync();
        }

        public async Task InitializeServices()
        {
            try
            {
                bool resetFiles = Preferences.Get("ResetAtStart", true);
                if (resetFiles)
                {
                    GHApp.GnollHackService.ClearFiles();
                    Preferences.Set("ResetAtStart", false);
                    Preferences.Set("ResetExternalFiles", true);
                }
                GHApp.ResetAcquiredFiles();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }

            await TryInitializeGnollHack();
            await TryInitializeSecrets();
            TryInitializeFMOD();
        }

        public void TryReadSecrets()
        {
            try
            {
                GHApp.ReadSecrets();
                Array.Sort<SecretsFile>(GHApp.CurrentSecrets.files, new SecretsFileSizeComparer());
            }
            catch (Exception ex)
            {
                DisplayAlertGrid("Reading Secrets File Failed", "GnollHack failed to read secrets file: " + ex.Message, "OK", GHColors.Red);
            }
            try
            {
                GHApp.ReadUserSecrets();
            }
            catch (Exception ex)
            {
                DisplayAlertGrid("Reading User Secrets File Failed", "GnollHack failed to read user secrets file: " + ex.Message, "OK", GHColors.Red);
            }
        }

        public async Task TryInitializeGnollHack()
        {
            try
            {
                await GHApp.GnollHackService.InitializeGnollHack();
            }
            catch (Exception ex)
            {
                DisplayAlertGrid("GnollHack Initialization Failed", "Initializing GnollHack failed: " + ex.Message, "OK", GHColors.Red);
            }
        }
        public async Task TryInitializeSecrets()
        {
            try
            {
                await GHApp.GnollHackService.InitializeSecrets(GHApp.CurrentSecrets);
            }
            catch (Exception ex)
            {
                DisplayAlertGrid("Secrets Initialization Failed", "Initializing secrets failed: " + ex.Message, "OK", GHColors.Red);
            }
        }

        public void TryInitializeFMOD()
        {
            try
            {
                GHApp.FmodService.InitializeFmod();
            }
            catch (Exception ex)
            {
                DisplayAlertGrid("FMOD Initialization Failed", "Initializing FMOD failed: " + ex.Message, "OK", GHColors.Red);
            }
        }

        public void TryClearCoreFiles()
        {
            try
            {
                GHApp.GnollHackService.ClearCoreFiles();
            }
            catch (Exception ex)
            {
                DisplayAlertGrid("File Clearing Failure", "GnollHack failed to clear core files: " + ex.Message, "OK", GHColors.Orange);
            }
        }

        private async Task StartUpTasks()
        {
            GHApp.InitFileDescriptors();

            TryReadSecrets();
            await InitializeServices();

            GHApp.InitAdditionalTypefaces();
            GHApp.InitAdditionalCachedBitmaps();
            GHApp.InitSymbolBitmaps();
            GHApp.InitGameBitmaps();

            GHApp.ReadUserData();
            GHApp.CheckUserData();
            GHApp.DiscoveredMusicBits = GHApp.GetDiscoveredTracks();
            GHApp.AddPreDiscoveredMusic();

            carouselView.Init();

            string verstr = "?";
            string verid = "?";
            string fmodverstr = "?";
            string skiaverstr = "?";
            string skiasharpverstr = "?";
            string frameworkverstr = "?";
            string frameworkUIverstr = "?";
            string runtimeverstr = "?";
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
                frameworkverstr = RuntimeInformation.FrameworkDescription ?? "?";
                int pidx = frameworkverstr.IndexOf(" (");
                if (pidx > 0)
                {
                    frameworkverstr = frameworkverstr.Substring(0, pidx);
                }
                //string pverstr_detailed = (Attribute.GetCustomAttribute(typeof(Button).Assembly, typeof(AssemblyFileVersionAttribute), false) as AssemblyFileVersionAttribute)?.Version;
                string pverstr = "";
#if GNH_MAUI
                pverstr = (Attribute.GetCustomAttribute(typeof(Microsoft.Maui.Controls.Button).Assembly, typeof(AssemblyInformationalVersionAttribute), false) as AssemblyInformationalVersionAttribute)?.InformationalVersion;
#else
                pverstr = (Attribute.GetCustomAttribute(typeof(Xamarin.Forms.Button).Assembly, typeof(AssemblyInformationalVersionAttribute), false) as AssemblyInformationalVersionAttribute)?.InformationalVersion;
#endif
                if (!string.IsNullOrWhiteSpace(pverstr))
                {
                    int plusIdx = pverstr.IndexOf("+");
                    if (plusIdx > 0)
                    {
                        pverstr = pverstr.Substring(0, plusIdx);
                    }
                    int minusIdx = pverstr.IndexOf("-");
                    if (minusIdx > 0)
                    {
                        pverstr = pverstr.Substring(0, minusIdx);
                    }

                    frameworkUIverstr = pverstr;
                }
                runtimeverstr = Environment.Version.ToString() ?? "?";
                pidx = runtimeverstr.IndexOf(" (");
                if (pidx > 0)
                {
                    runtimeverstr = runtimeverstr.Substring(0, pidx);
                }
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
                        skiasharpverstr = GHApp.ParseSkiaSharpVersionString(fullverid);
                    }
                }
                GHApp.ListGPUs();
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
            GHApp.FrameworkVersionString = frameworkverstr;
            GHApp.UIFrameworkVersionString = frameworkUIverstr;
            GHApp.RuntimeVersionString = runtimeverstr;

            //VersionLabel.Text = verid;
            GnollHackLabel.Text = "GnollHack";

            try
            {
                string prev_version = Preferences.Get("VersionId", "");
                ulong prev_vernum = (ulong)Preferences.Get("VersionNumber", 0L);
                GHApp.GHPreviousVersionNumber = prev_vernum;

                if (prev_version != verid || prev_vernum != vernum)
                {
                    TryClearCoreFiles();
                    await TryInitializeGnollHack();
                    await TryInitializeSecrets();
                }
                if (prev_vernum > 0 && prev_vernum < vercompat)
                    GHApp.CheckForIncompatibleSavedGames();

                Preferences.Set("VersionId", verid);
                Preferences.Set("VersionNumber", (long)vernum);
            }
            catch (Exception ex)
            { 
                Debug.WriteLine(ex.Message); 
            }

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

            await GHApp.SetSoundBanksUpForLoading();
            if (GHApp.LoadBanks)
            {
                GHApp.FmodService.LoadBanks(sound_bank_loading_type.Master);
                GHApp.FmodService.LoadBanks(sound_bank_loading_type.Preliminary);
            }

            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume;
            generalVolume = Preferences.Get("GeneralVolume", GHConstants.DefaultGeneralVolume);
            musicVolume = Preferences.Get("MusicVolume", GHConstants.DefaultMusicVolume);
            ambientVolume = Preferences.Get("AmbientVolume", GHConstants.DefaultAmbientVolume);
            dialogueVolume = Preferences.Get("DialogueVolume", GHConstants.DefaultDialogueVolume);
            effectsVolume = Preferences.Get("EffectsVolume", GHConstants.DefaultEffectsVolume);
            uiVolume = Preferences.Get("UIVolume", GHConstants.DefaultUIVolume);
            try
            {
                /* Adjust first UI volumes */
                GHApp.FmodService?.AdjustUIVolumes(generalVolume, musicVolume, uiVolume);
                GHGame curGame = GHApp.CurrentGHGame;
                if (curGame != null)
                {
                    /* Game is ongoing, so ask GHGame to handle this */
                    curGame.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.SetVolume, generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume));
                }
                else
                {
                    /* Adjust directly, since there's no game thread */
                    GHApp.FmodService?.AdjustGameVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume);
                }
                if (GHApp.LoadBanks)
                    GHApp.FmodService?.PlayUIMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);

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
            if (GHApp.IsAndroid && GHApp.DownloadOnDemandPackage)
            {
                DownloadLabel.Text = "";
                string path = GHApp.PlatformService.GetAbsoluteOnDemandAssetPath(GHConstants.OnDemandPackName);
                if (path == null)
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
                GHApp.FmodService?.PlayUIMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);
                GHApp.MainScreenMusicStarted = true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Playing music failed: " + ex.Message);
            }
        }

        //private readonly object _startUpLock = new object();
        private int _finishedLogoFadeIn = 0;
        private bool FinishedLogoFadeIn { get { return Interlocked.CompareExchange(ref _finishedLogoFadeIn, 0, 0) != 0; } set { Interlocked.Exchange(ref _finishedLogoFadeIn, value ? 1 : 0); } }
#if GNH_MAUI
        IDispatcherTimer _startUpTimer;
#endif

        private async Task StartFadeLogoIn()
        {
            await StartLogoImage.FadeTo(1, 250);
            StartLogoImage.Opacity = 1.0; /* To make sure */
            await FmodLogoImage.FadeTo(1, 250);
            FmodLogoImage.Opacity = 1.0; /* To make sure */
            FinishedLogoFadeIn = true;

            //List<Task> tasklist = new List<Task> { t1, t2 };
            //await Task.WhenAll(tasklist);
        }

        private void StartStartUpTimer()
        {
#if GNH_MAUI
            _startUpTimer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
            _startUpTimer.Interval = TimeSpan.FromSeconds(GHConstants.StartUpTimeOut);
            _startUpTimer.IsRepeating = false;
            _startUpTimer.Tick += (s, e) =>
            {
                if (!FinishedLogoFadeIn)
                    ShowStartUpTimeOutAlert();
            };
            _startUpTimer.Start();
#else
            Device.StartTimer(TimeSpan.FromSeconds(GHConstants.StartUpTimeOut), () =>
            {
                if (!FinishedLogoFadeIn)
                    ShowStartUpTimeOutAlert();
                return false;
            });
#endif
        }

        private void ShowStartUpTimeOutAlert()
        {
            DisplayAlertGrid("Startup Timeout", 
                "GnollHack has exceeded its startup timeout limit, possibly due to animations being turned off." 
                    + (GHApp.IsAndroid && GHApp.IsSamsung ? " In Android Settings, please switch off Accessibility > Visual Enhancements > " 
                    + GHApp.OneUIAnimationSettingName + "." 
                : " Please check your device animation settings."), 
                "OK", GHColors.Orange, AppCloseStyle.ExitAppOnHangup);
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
            GHApp.AddSentryBreadcrumb("Exit button clicked.", GHConstants.SentryGnollHackGeneralCategoryName);
            await ExitApp();
        }

        private async Task ExitApp()
        {
            GHApp.AddSentryBreadcrumb("ExitApp", GHConstants.SentryGnollHackGeneralCategoryName);
            UpperButtonGrid.IsEnabled = false;
            ExitButton.IsEnabled = false;
            ExitButton.TextColor = GHColors.Red;
            GHApp.PlayButtonClickedSound();

            bool hideautoupdatealert = Preferences.Get("HideAutoUpdateAlert", false);
            bool isfromgoogleplay = true;
            if (!hideautoupdatealert && !GHApp.IsNoStore)
            {
                _popupStyle = popup_style.DisableAutoUpdate;
                PopupCheckBoxLayout.IsVisible = true;
                PopupTitleLabel.TextColor = GHColors.Red;
                PopupTitleLabel.Text = "Auto-Update Warning";
                if (GHApp.IsAndroid && isfromgoogleplay)
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you turn off Auto-Update from Google Play Store for GnollHack and manually apply updates when you have no saved games.";
                else if (GHApp.IsiOS)
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates by toggling off App Updates under App Store section in the Settings app, and manually apply updates when you have no saved games.";
                else if (GHApp.IsWindows)
                {
                    if (GHApp.IsPackaged)
                        PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates by toggling off App Updates under Profile > Settings in the Microsoft Store app, and manually apply updates when you have no saved games.";
                    else
                    {
                        if (GHApp.IsSteam)
                        {
                            PopupLabel.Text = "Some updates of GnollHack may be incompatible with your saved games. In such a case, you can revert to an old version of the game by going to the Betas tab under GnollHack Properties and activating a compatible version therein.";
                        }
                        else
                        {
                            PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you do not use an automatic update of GnollHack, and manually apply updates only when you have no saved games.";
                        }
                    }
                }
                else
                    PopupLabel.Text = "Updating GnollHack may cause your saved games to become invalid. We recommend that you disable automatic updates in your device settings and manually apply updates when you have no saved games.";
                PopupGrid.IsVisible = true;
            }
            else
            {
                await CheckPendingTasksAndExit();
            }
        }

        public void ForceCloseApp()
        {
            _popupStyle = popup_style.ForceCloseApp;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = GHColors.Red;
            PopupTitleLabel.Text = "GnollHack Needs to Close";
            PopupLabel.Text = "A panic occurred during loading a saved game, and the app state may have become unstable. Restarting GnollHack is required.";
            PopupGrid.IsVisible = true;
        }

        private async Task CheckPendingTasksAndExit()
        {
            GHApp.AddSentryBreadcrumb("CheckPendingTasksAndExit", GHConstants.SentryGnollHackGeneralCategoryName);
            PendingGeneralTimerTasks = CalculatePendingGeneralTimerTasks();
            if (PendingGeneralTimerTasks > 0)
            {
                await StartGeneralTimerAsync();
                UpdateGeneralTimerTasksLabel(true);
                PendingTasksGrid.IsEnabled = true;
                PendingTasksGrid.IsVisible = true;
            }
            else
            {
                await CloseApp();
                ExitButton.TextColor = GHColors.White;
                ExitButton.IsEnabled = true;
                UpperButtonGrid.IsEnabled = true;
            }
        }

        //private readonly object _closingAppLock = new object();
        private int _closingApp = 0;
        private bool CheckCloseAndSetTrue { get { return Interlocked.Exchange(ref _closingApp, 1) != 0; } }

        private async Task CloseApp()
        {
            if (CheckCloseAndSetTrue)
                return;

            GHApp.AddSentryBreadcrumb("CloseApp", GHConstants.SentryGnollHackGeneralCategoryName);
            DoStopGeneralTimer();
            GHApp.StopPlatformRenderLoop();
            GHApp.CheckCloseGnhThread(); /* Close GnhThread if a game is still ongoing for some reason */
            Task t1 = GeneralTimerTasksAsync(); /* Make sure outstanding queues are processed before closing application */
            Task t2 = Task.Delay(1000); /* Give 1 second to close at maximum */
            await Task.WhenAny(t1, t2);
            carouselView.Stop();
            GHApp.FmodService?.StopAllGameSounds((uint)StopSoundFlags.All, 0U);
            await Task.Delay(100);
            GHApp.FmodService?.StopAllUISounds();
            await Task.Delay(100);
            GHApp.FmodService?.ShutdownFmod();
            await Task.Delay(100);
            GHApp.BeforeExitApp();
            GHApp.PlatformService?.CloseApplication();
            GHApp.AddSentryBreadcrumb("Post CloseApplication", GHConstants.SentryGnollHackGeneralCategoryName);

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

        private void DoStopGeneralTimer()
        {
#if GNH_MAUI
            _generalTimer?.Stop();
            StopGeneralTimer = false;
            GeneralTimerIsOn = false;
#else
            StopGeneralTimer = true; /* Stop timer */
#endif
        }

        private async void ResetButton_Clicked(object sender, EventArgs e)
        {
            await OpenResetPage();
        }

        private async Task OpenResetPage()
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            StopGeneralTimer = true;
            var resetPage = new ResetPage(this);
            await GHApp.Navigation.PushModalAsync(resetPage);
            StopGeneralTimer = false;
            await StartGeneralTimerAsync();
            UpperButtonGrid.IsEnabled = true;
        }

        private async void SettingsButton_Clicked(object sender, EventArgs e)
        {
            await OpenSettingsPage();
        }

        public async Task OpenSettingsPage()
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            var settingsPage = new SettingsPage(null, this);
            await GHApp.Navigation.PushModalAsync(settingsPage);
            UpperButtonGrid.IsEnabled = true;
        }

        private async void OptionsButton_Clicked(object sender, EventArgs e)
        {
            await OpenOptionsPage();
        }

        public async Task OpenOptionsPage()
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "defaults.gnh");
            var editorPage = new EditorPage(this, fulltargetpath, "Default Options File");
            try
            {
                editorPage.ReadFile();
                carouselView.Stop();
                await GHApp.Navigation.PushModalAsync(editorPage);
            }
            catch (Exception ex)
            {
                editorPage.ClearTextEditor();
                DisplayAlertGrid("Reading Options File Failed", "GnollHack failed to read the options file: " + ex.Message, "OK", GHColors.Red);
            }
            UpperButtonGrid.IsEnabled = true;
        }

        private async void CreditsButton_Clicked(object sender, EventArgs e)
        {
            await OpenAboutPage();
        }

        private async Task OpenAboutPage()
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            StopGeneralTimer = true;
            var aboutPage = new AboutPage(this);
            await GHApp.Navigation.PushModalAsync(aboutPage);
            StopGeneralTimer = false;
            await StartGeneralTimerAsync();
            UpperButtonGrid.IsEnabled = true;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            carouselView.Stop();
        }

        protected override bool OnBackButtonPressed()
        {
            return true;
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
            await OpenVaultPage();
        }

        private async Task OpenVaultPage()
        {
            UpperButtonGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            carouselView.Stop();
            var vaultPage = new VaultPage(this);
            await GHApp.Navigation.PushModalAsync(vaultPage);
            UpperButtonGrid.IsEnabled = true;
        }

        public void StartCarouselViewAndEnableButtons()
        {
            carouselView.Play();
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
            DisableAutoUpdate,
            ForceCloseApp,
        }

        private string _popupViewUrl = string.Empty;
        private string _popupViewTitle = string.Empty;
        private popup_style _popupStyle = popup_style.GeneralDialog;
        private async void PopupOkButton_Clicked(object sender, EventArgs e)
        {
            await ClosePopup();
        }

        private async Task ClosePopup()
        {
            PopupOkButton.IsEnabled = false;
            PopupOkButton2.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            PopupGrid.IsVisible = false;
            PopupOkButton.IsVisible = true;
            PopupButtonGrid.IsVisible = false;
            if (_popupStyle == popup_style.DisableAutoUpdate)
            {
                if (PopupNoAgainCheckBox.IsChecked)
                {
                    Preferences.Set("HideAutoUpdateAlert", true);
                    await Task.Delay(50);
                }

                await CheckPendingTasksAndExit();
            }
            else if (_popupStyle == popup_style.ForceCloseApp)
            {
                await CheckPendingTasksAndExit();
            }
            PopupOkButton.IsEnabled = true;
            PopupOkButton2.IsEnabled = true;
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

        private void PopupNoAgainTapGestureRecognizer_Tapped(object sender, EventArgs e)
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
            if (GHApp.CurrentGamePage == null)
            {
                carouselView.Play();
            }
            StopGeneralTimer = false;
            StartGeneralTimer();
            //DoKeyboardFocus();
        }

        public void DoKeyboardFocus()
        {
#if WINDOWS
            Microsoft.UI.Xaml.Controls.Button fbtn = KeyboardFocusButton.Handler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
            if (fbtn != null)
            {
                fbtn.Focus(FocusState.Programmatic);
            }
#endif
        }

        private async void PendingTasksOkButton_Clicked(object sender, EventArgs e)
        {
            await PendingTasksOk();
        }

        private async Task PendingTasksOk()
        {
            PendingTasksGrid.IsEnabled = false;
            await CloseApp();
            PendingTasksGrid.IsVisible = false; /* Insurance */
            ExitButton.TextColor = GHColors.White;
            ExitButton.IsEnabled = true;
            UpperButtonGrid.IsEnabled = true;
        }

        private void PendingTasksCancelButton_Clicked(object sender, EventArgs e)
        {
            PendingTasksGrid.IsEnabled = false;
            PendingTasksGrid.IsVisible = false;
            ExitButton.TextColor = GHColors.White;
            ExitButton.IsEnabled = true;
            UpperButtonGrid.IsEnabled = true;
        }

        public void InvalidateCarousel()
        {
            carouselView.InvalidateSurface();
        }

        private void TournamentTapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            _popupStyle = popup_style.GeneralDialog;
            PopupCheckBoxLayout.IsVisible = false;
            PopupTitleLabel.TextColor = GHColors.LightGreen;
            PopupTitleLabel.Text = "Tournament Mode";
            PopupLabel.Text = "Tournament Mode is Classic Mode with minimum difficulty at Expert. It also forces on game progress and top score reporting, bones sharing, and game recording.";
            PopupGrid.IsVisible = true;
        }

        private void KeyboardFocusButton_Clicked(object sender, EventArgs e)
        {
            //if (GHApp.CurrentGamePage != null)
            //{
            //    GHApp.CurrentGamePage.SendEnterPressed();
            //}
        }

        private async void PopupViewButton_Clicked(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(_popupViewUrl) && Uri.IsWellFormedUriString(_popupViewUrl, UriKind.Absolute))
                await GHApp.OpenBrowser(this, _popupViewTitle, new Uri(_popupViewUrl));
            else
                DisplayAlertGrid("Malformed URL", "The URL was malformed: " + _popupViewUrl, "OK", GHColors.Orange);
        }

        private async void AlertOkButton_Clicked(object sender, EventArgs e)
        {
            AlertOkButton.IsEnabled = false;
            AlertOkButton2.IsEnabled = false;
            AlertExitButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            AlertGrid.IsVisible = false;
            switch (_alertGridCloseAppStyle)
            {
                default:
                case AppCloseStyle.Normal:
                    /* Nothing special */
                    break;
                case AppCloseStyle.ExitAppOnHangup:
                    if (!FinishedLogoFadeIn)
                        await ShowWaitAndCloseApp();
                    break;
                case AppCloseStyle.ExitAppOnHangupAndRemoveAnimations:
                    if (!FinishedLogoFadeIn || (GHApp.PlatformService?.IsRemoveAnimationsOn() ?? true))
                        await ShowWaitAndCloseApp();
                    break;
                case AppCloseStyle.ExitAppOnHangupAndAnimatorDuration:
                    if (!FinishedLogoFadeIn || ((GHApp.PlatformService?.GetCurrentAnimatorDurationScale() ?? -1.0f) <= 0.0f))
                        await ShowWaitAndCloseApp();
                    break;
                case AppCloseStyle.ExitApp:
                    await ShowWaitAndCloseApp();
                    break;
                case AppCloseStyle.ReturnToGame:
                    TerminateAndRestartGame();
                    break;
            }
            AlertOkButton.IsEnabled = true;
            AlertOkButton2.IsEnabled = true;
            AlertExitButton.IsEnabled = true;
            DisplayNewAlert(); //Show next alert if there are more in the queue
        }

        private async void AlertExitButton_Clicked(object sender, EventArgs e)
        {
            AlertOkButton.IsEnabled = false;
            AlertOkButton2.IsEnabled = false;
            AlertExitButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await ShowWaitAndCloseApp();
        }

        private async Task ShowWaitAndCloseApp()
        {
            WaitLabel.Text = "Exiting...";
            WaitLayout.IsVisible = true;
            await CloseApp();
        }

        private class DisplayAlertGridItem
        {
            public DisplayAlertGridItem()
            {

            }
            public DisplayAlertGridItem(string title, string message, string buttonText, Color titleColor, AppCloseStyle closeAppStyle = AppCloseStyle.Normal, bool showExitButton = false)
            {
                Title = title;
                Message = message;
                ButtonText = buttonText;
                TitleColor = titleColor;
                CloseAppStyle = closeAppStyle;
                ShowExitButton = showExitButton;
            }
            public string Title;
            public string Message;
            public string ButtonText;
            public Color TitleColor;
            public AppCloseStyle CloseAppStyle;
            public bool ShowExitButton;
        }

        private readonly Queue<DisplayAlertGridItem> _alertQueue = new Queue<DisplayAlertGridItem>();
        private enum AppCloseStyle
        {
            Normal = 0,
            ExitAppOnHangup = 1,
            ExitAppOnHangupAndRemoveAnimations = 2,
            ExitAppOnHangupAndAnimatorDuration = 3,
            ExitApp = 4,
            ReturnToGame = 5,
        }
        private AppCloseStyle _alertGridCloseAppStyle = AppCloseStyle.Normal;

        private void DisplayAlertGrid(string title, string message, string buttonText, Color titleColor, AppCloseStyle closeAppStyle = AppCloseStyle.Normal, bool showExitButton = false)
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                _alertQueue.Enqueue(new DisplayAlertGridItem(title, message, buttonText, titleColor, closeAppStyle, showExitButton));
                if (!AlertGrid.IsVisible)
                    DisplayNewAlert();
            });
        }

        private void DisplayNewAlert()
        {
            if (_alertQueue.Count > 0)
            {
                try
                {
                    DisplayAlertGridItem alert = _alertQueue.Dequeue();
                    if (alert != null)
                    {
                        _alertGridCloseAppStyle = alert.CloseAppStyle;
                        AlertTitleLabel.Text = alert.Title;
                        AlertTitleLabel.TextColor = alert.TitleColor;
                        AlertLabel.Text = alert.Message;
                        AlertOkButton.Text = alert.ButtonText;
                        AlertOkButton2.Text = alert.ButtonText;
                        AlertOkButton.IsVisible = !alert.ShowExitButton;
                        AlertButtonGrid.IsVisible = alert.ShowExitButton;
                        AlertOkButton.IsEnabled = true;
                        AlertOkButton2.IsEnabled = true;
                        AlertExitButton.IsEnabled = true;
                        AlertGrid.IsVisible = true;
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }
            }
        }

        //private async Task DisplayAlertOrGrid(string title, string message, string buttonText, Color titleColor)
        //{
        //    if (AlertGrid.IsVisible)
        //        await GHApp.DisplayMessageBox(this, title, message, buttonText);
        //    else
        //        DisplayAlertGrid(title, message, buttonText, titleColor);
        //}

        public bool HandleSpecialKeyPress(GHSpecialKey key, bool isCtrl, bool isMeta, bool isShift)
        {
            bool handled = false;
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (key == GHSpecialKey.Escape || key == GHSpecialKey.Enter || key == GHSpecialKey.Space)
                        {
                            if (AlertGrid.IsVisible && AlertOkButton.IsEnabled)
                            {
                                AlertOkButton_Clicked(this, EventArgs.Empty);
                                handled = true;
                            }
                            else if (PopupGrid.IsVisible && ((PopupOkButton.IsVisible && PopupOkButton.IsEnabled) || (PopupButtonGrid.IsVisible && PopupOkButton2.IsVisible && PopupOkButton2.IsEnabled)))
                            {
                                await ClosePopup();
                                handled = true;
                            }
                            else if (PendingTasksGrid.IsVisible && key == GHSpecialKey.Escape && PendingTasksCancelButton.IsVisible && PendingTasksCancelButton.IsEnabled)
                            {
                                PendingTasksCancelButton_Clicked(this, EventArgs.Empty);
                                handled = true;
                            }
                            else if (PendingTasksGrid.IsVisible && (key == GHSpecialKey.Enter || key == GHSpecialKey.Space) && PendingTasksOkButton.IsVisible && PendingTasksOkButton.IsEnabled)
                            {
                                await PendingTasksOk();
                                handled = true;
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return handled;
        }

        public bool HandleKeyPress(int key, bool isCtrl, bool isMeta)
        {
            bool handled = false;
            if (AlertGrid.IsVisible || PopupGrid.IsVisible || PendingTasksGrid.IsVisible)
                return false;

            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        switch (key)
                        {
                            case (int)'p':
                                if (StartLocalGameButton.IsEnabled && StartLocalGrid.IsEnabled && StartLocalGameButton.IsVisible && StartLocalGrid.IsVisible && UpperButtonGrid.IsVisible)
                                    await StartLocalGame();
                                handled = true;
                                break;
                            case (int)'s':
                                if (SettingsButton.IsEnabled && SettingsButton.IsVisible && UpperButtonGrid.IsVisible)
                                    await OpenSettingsPage();
                                handled = true;
                                break;
                            case (int)'o':
                                if (OptionsButton.IsEnabled && OptionsButton.IsVisible && UpperButtonGrid.IsVisible)
                                    await OpenOptionsPage();
                                handled = true;
                                break;
                            case (int)'r':
                                if (ResetButton.IsEnabled && ResetButton.IsVisible && UpperButtonGrid.IsVisible)
                                    await OpenResetPage();
                                handled = true;
                                break;
                            case (int)'a':
                                if (AboutButton.IsEnabled && AboutButton.IsVisible && UpperButtonGrid.IsVisible)
                                    await OpenAboutPage();
                                handled = true;
                                break;
                            case (int)'v':
                                if (VaultButton.IsEnabled && VaultButton.IsVisible && UpperButtonGrid.IsVisible)
                                    await OpenVaultPage();
                                handled = true;
                                break;
                            case (int)'w': //Wizard mode
                                if (wizardModeSwitch.IsEnabled && wizardModeGrid.IsVisible && StartButtonLayout.IsVisible)
                                    wizardModeSwitch.IsToggled = !wizardModeSwitch.IsToggled;
                                handled = true;
                                break;
                            case (int)'c': //Classic mode
                                if (classicModeSwitch.IsEnabled && classicModeGrid.IsVisible && StartButtonLayout.IsVisible)
                                    classicModeSwitch.IsToggled = !classicModeSwitch.IsToggled;
                                handled = true;
                                break;
                            case (int)'C': //Casual mode
                                if (casualModeSwitch.IsEnabled && casualModeGrid.IsVisible && StartButtonLayout.IsVisible)
                                    casualModeSwitch.IsToggled = !casualModeSwitch.IsToggled;
                                handled = true;
                                break;
                            case (int)'e':
                            case (int)'x':
                            case (int)'q':
                                if (ExitButton.IsEnabled && ExitButton.IsVisible && StartButtonLayout.IsVisible)
                                    await ExitApp();
                                handled = true;
                                break;
                            default:
                                break;
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return handled;
        }

        private async void EventOkButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            EventButtonGrid.IsEnabled = false;
            EventGrid.IsVisible = false;
            switch (_eventStyle)
            {
                case 1:
                    await GHApp.OpenBrowser(this, "Give Us a Star on GitHub", new Uri(GHConstants.GnollHackGitHubStarPage), true);
                    break;
                default:
                    break;
            }
        }

        private void EventCancelButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            EventButtonGrid.IsEnabled = false;
            EventGrid.IsVisible = false;
        }

        private int _eventStyle = 0;

        private void DisplayEventGrid(string title, string text, string ok, string cancel, int eventStyle)
        {
            EventTitleLabel.Text = title;
            EventLabel.Text = text;
            EventOkButton.Text = ok;
            EventCancelButton.Text = cancel;
            _eventStyle = eventStyle;
            switch (eventStyle)
            {
                case 1:
                    EventTitleLabel.TextColor = GHColors.TitleGoldColor;
                    EventOkButton.TextColor = GHColors.Yellow;
                    break;
                default:
                    EventTitleLabel.TextColor = GHColors.White;
                    EventOkButton.TextColor = GHColors.White;
                    break;
            }

            EventButtonGrid.IsEnabled = true;
            EventGrid.IsVisible = true;
        }
    }
}

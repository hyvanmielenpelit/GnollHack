using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using GnollHackX.Pages.MainScreen;
using static System.Net.Mime.MediaTypeNames;

namespace GnollHackX.Pages.Game
#endif
{
    public class DumplogEntry
    {
        public string FilePath { get; set; }
        public string DisplayName { get; set; }
        public string Outcome { get; set; }
        public int Score { get; set; }
        public string DeathDate { get; set; }
        public long FileSize { get; set; }
    }

    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class OverseerPage : CustomModalPage, ICloseablePage
    {
#if GNH_MAUI
        IDispatcherTimer _timer = null;
#endif
        private string _baseOverseerUrl;
        private string _snapshotHtml;
        private List<CheckBox> _dumplogCheckBoxes = new List<CheckBox>();
        private List<DumplogEntry> _dumplogEntries = new List<DumplogEntry>();
        private string _sessionId = "";
        private bool _overseerLoaded = false;
        private bool _bridgeInitialized = false;

        public OverseerPage(string title, string baseOverseerUrl,
                            string snapshotHtml)
        {
            InitializeComponent();
            bool isDarkMode = GHApp.DarkMode;
            if (isDarkMode)
            {
                AttachTypeFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
                DumplogPickerFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
            }

            _baseOverseerUrl = baseOverseerUrl;
            _snapshotHtml = snapshotHtml;

            /* We no longer set an initial HtmlWebViewSource here to avoid rapid 
             * double-navigation crashes (winrt::hresult_error) in WinUI 3 WebView2 
             * when the final URL is set shortly after. The ProgressOverlay is 
             * sufficient for the connecting UI. */

            UpdateNavigationButtons(true);
#if GNH_MAUI && WINDOWS
            if (!string.IsNullOrWhiteSpace(title))
            {
                TitleLabel.Text = title;
                TitleLabel.IsVisible = true;
            }
            ButtonRowDefinition.Height = 82;
            Appearing += (s, e) =>
            {
                _timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                _timer.Interval = TimeSpan.FromSeconds(0.5);
                _timer.IsRepeating = true;
                _timer.Tick += (s, e) =>
                {
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        UpdateNavigationButtons();
                    });
                };
                _timer.Start();
            };
            Disappearing += (s, e) =>
            {
                if (_timer != null)
                {
                    _timer.Stop();
                }
            };
#endif
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            await UploadAndConnect();
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            CleanupJsBridge();
        }

        private async Task UploadAndConnect()
        {
            string overseerUrl = _baseOverseerUrl;

            try
            {
                ProgressStatusLabel.Text = "Uploading game data...";
                UploadProgressBar.Progress = 0.3;

                using (var httpClient = new HttpClient())
                {
                    httpClient.Timeout = TimeSpan.FromSeconds(10);
                    using (var content = new MultipartFormDataContent())
                    {
                        content.Add(new StringContent(GHApp.XlogUserName ?? ""), "UserName");
                        content.Add(new StringContent(GHApp.XlogPassword ?? ""), "Password");
                        content.Add(new StringContent(GHApp.XlogAntiForgeryToken ?? ""), "AntiForgeryToken");

                        if (!string.IsNullOrEmpty(_snapshotHtml))
                            content.Add(new StringContent(_snapshotHtml, Encoding.UTF8, "text/html"), "SnapshotHtml");

                        /* Send unified Environment Data (Version Info, Settings, Debug Info) */
                        bool isGameOn = GHApp.CurrentGamePage?.IsGameOn ?? false;
                        int overseerMode = (GHApp.DeveloperMode && GHApp.DebugLogMessages) ? 2 : (isGameOn ? 0 : 1);
                        
                        string sessionTitle = "GnollHack Assistance";
                        if (GHApp.DeveloperMode && GHApp.DebugLogMessages)
                        {
                            sessionTitle = "GnollHack Developer Console";
                        }
                        else if (isGameOn)
                        {
                            string characterName = GHApp.TournamentMode ? GHApp.LastUsedTournamentPlayerName : GHApp.LastUsedPlayerName;
                            sessionTitle = string.IsNullOrWhiteSpace(characterName) ? "GnollHack Gameplay" : $"GnollHack Gameplay ({characterName})";
                        }
                        content.Add(new StringContent(sessionTitle), "Title");

                        var envData = GHApp.GetEnvironmentData();
                        envData.BoolData["allowSpoilers"] = GHApp.OverseerAllowSpoilers;
                        envData.BoolData["verboseResponses"] = GHApp.OverseerVerboseResponses;
                        envData.BoolData["sendGameContext"] = GHApp.OverseerSendGameContext;
                        envData.BoolData["isGameOn"] = isGameOn;
                        envData.IntData["overseerMode"] = overseerMode;

                        string settingsJson = Newtonsoft.Json.JsonConvert.SerializeObject(envData);
                        content.Add(new StringContent(settingsJson, Encoding.UTF8, "application/json"),
                                    "OverseerSettings");

                        /* Mode-aware initial prompt for greeting */
                        string initialPrompt;
                        switch (overseerMode)
                        {
                        case 2:
                            initialPrompt = "Greet me in debug mode and briefly summarize the debug data you see.";
                            break;
                        case 1:
                            initialPrompt = "Greet me and let me know how you can help with technical issues and help learn more about GnollHack game mechanics.";
                            break;
                        default:
                            initialPrompt = isGameOn
                                ? "Greet me and give a brief observation about my current situation from the game snapshot."
                                : "Greet me and let me know how you can help.";
                            break;
                        }
                        content.Add(new StringContent(initialPrompt), "InitialPrompt");

                        ProgressStatusLabel.Text = "Contacting Gnoll Overseer...";
                        UploadProgressBar.Progress = 0.6;

                        var response = await httpClient.PostAsync(
                            _baseOverseerUrl + "/api/session/create", content);

                        if (response.IsSuccessStatusCode)
                        {
                            string jsonResponse = await response.Content.ReadAsStringAsync();
                            var result = JObject.Parse(jsonResponse);
                            string sessionId = result?["sessionId"]?.ToString() ?? "";
                            _sessionId = sessionId;
                            string handoffToken = result?["handoffToken"]?.ToString() ?? "";

                            overseerUrl = _baseOverseerUrl +
                                          $"/api/auth/handoff?token={handoffToken}&sessionId={sessionId}";

                            MainThread.BeginInvokeOnMainThread(() =>
                            {
                                ProgressStatusLabel.Text = "Connected!";
                                UploadProgressBar.Progress = 1.0;
                            });
                        }
                        else
                        {
                            string msg = "Overseer session failed: HTTP " + (int)response.StatusCode;
                            GHApp.WriteGHLog(msg);
                            MainThread.BeginInvokeOnMainThread(() =>
                            {
                                ProgressStatusLabel.Text = "Connection failed. Opening without game context.";
                                UploadProgressBar.Progress = 1.0;
                            });
                            await Task.Delay(2000);
                        }
                    }
                }
            }
            catch (TaskCanceledException)
            {
                GHApp.WriteGHLog("Overseer upload timed out after 10 seconds.");
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    ProgressStatusLabel.Text = "Connection timed out. Opening without game context.";
                    UploadProgressBar.Progress = 1.0;
                });
                await Task.Delay(2000);
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Overseer upload failed: " + ex.Message);
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    ProgressStatusLabel.Text = "Upload failed. Opening without game context.";
                    UploadProgressBar.Progress = 1.0;
                });
                await Task.Delay(2000);
            }

            MainThread.BeginInvokeOnMainThread(() =>
            {
                /* Hide overlay and navigate to the final URL */
                ProgressOverlay.IsVisible = false;
                DisplayWebView.Source = new UrlWebViewSource { Url = overseerUrl };
            });

            /* Free the data references - they can be large */
            _snapshotHtml = null;
        }

        private void DisplayWebView_Navigating(object sender, WebNavigatingEventArgs e)
        {
            NavigationLabel.Text = "Loading...";
            UpdateNavigationButtons();
        }

        private void DisplayWebView_Navigated(object sender, WebNavigatedEventArgs e)
        {
            NavigationLabel.Text = "";
            UpdateNavigationButtons();

            /* Enable Attach button once the real Overseer page has loaded successfully */
            if (!_overseerLoaded && e.Result == WebNavigationResult.Success
                && e.Url != null && e.Url.StartsWith("http", StringComparison.OrdinalIgnoreCase))
            {
                _overseerLoaded = true;
                AttachButton.IsEnabled = true;
                AttachButton.TextColor = GHColors.White;
            }

            /* v2: Initialize JS bridge for client tool support (once only) */
            if (!_bridgeInitialized && _overseerLoaded && GHApp.OverseerEnableClientTools)
            {
                SetupJsBridge();
                _bridgeInitialized = true;
            }

#if GNH_MAUI
            if(_timer == null)
            {
                var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                timer.Interval = TimeSpan.FromSeconds(0.5);
                timer.IsRepeating = false;
                timer.Tick += (s, e) =>
                {
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        UpdateNavigationButtons();
                    });
                };
                timer.Start();
            }
#else
            Device.StartTimer(TimeSpan.FromSeconds(0.5), () =>
            {
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    UpdateNavigationButtons();
                });
                return false;
            });
#endif
        }

        private void UpdateNavigationButtons(bool force = false)
        {
            if (force || BackButton.IsEnabled != DisplayWebView.CanGoBack)
            {
                BackButton.IsEnabled = DisplayWebView.CanGoBack;
                BackButton.TextColor = BackButton.IsEnabled ? GHColors.White : GHColors.Gray;
            }

            if (force || ForwardButton.IsEnabled != DisplayWebView.CanGoForward)
            {
                ForwardButton.IsEnabled = DisplayWebView.CanGoForward;
                ForwardButton.TextColor = ForwardButton.IsEnabled ? GHColors.White : GHColors.Gray;
            }
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync(true);
        }

        private async Task ClosePageAsync(bool playClickSound)
        {
            CloseButton.IsEnabled = false;
            _backPressed = true;
            if (playClickSound)
                GHApp.PlayButtonClickedSound();
            await GHApp.PopModalPageAsync();
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (CloseButton.IsEnabled)
                            await ClosePageAsync(true);
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }

                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        private void BackButton_Clicked(object sender, EventArgs e)
        {
            if (DisplayWebView.CanGoBack)
            {
                BackButton.IsEnabled = false;
                ForwardButton.IsEnabled = false;
                GHApp.PlayButtonClickedSound();
                DisplayWebView.GoBack();
                UpdateNavigationButtons();
            }
        }

        private void ForwardButton_Clicked(object sender, EventArgs e)
        {
            if (DisplayWebView.CanGoForward)
            {
                BackButton.IsEnabled = false;
                ForwardButton.IsEnabled = false;
                GHApp.PlayButtonClickedSound();
                DisplayWebView.GoForward();
                UpdateNavigationButtons();
            }
        }

        /* --- Attach Type Selector --- */
        private void AttachButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();

            /* Show/hide log buttons based on Developer Mode */
            if (GHApp.DeveloperMode)
            {
                string appLogPath = Path.Combine(GHApp.GHPath, GHConstants.AppLogDirectory, GHConstants.AppLogFileName);
                string panicLogPath = Path.Combine(GHApp.GHPath, "paniclog");

                bool appLogExists = File.Exists(appLogPath);
                bool panicLogExists = File.Exists(panicLogPath);

                AttachAppLogButton.IsVisible = true;
                AttachAppLogButton.IsEnabled = appLogExists;
                AttachAppLogButton.Text = appLogExists ? "App Log" : "No App Log";
                AttachAppLogButton.TextColor = appLogExists ? GHColors.White : GHColors.Gray;

                AttachPanicLogButton.IsVisible = true;
                AttachPanicLogButton.IsEnabled = panicLogExists;
                AttachPanicLogButton.Text = panicLogExists ? "Panic Log" : "No Panic Log";
                AttachPanicLogButton.TextColor = panicLogExists ? GHColors.White : GHColors.Gray;
            }
            else
            {
                AttachAppLogButton.IsVisible = false;
                AttachPanicLogButton.IsVisible = false;
            }

            AttachTypeGrid.IsVisible = true;
        }

        private void AttachTypeCancelButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            AttachTypeGrid.IsVisible = false;
        }

        /* --- Dumplog Picker --- */
        private void AttachDumplogsButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            AttachTypeGrid.IsVisible = false;

            _dumplogEntries = LoadDumplogEntries();
            if (_dumplogEntries.Count == 0)
            {
                NavigationLabel.Text = "No dumplogs found.";
                return;
            }

            DumplogListLayout.Children.Clear();
            _dumplogCheckBoxes.Clear();

            foreach (var entry in _dumplogEntries)
            {
                var checkBox = new CheckBox { Color = GHColors.White, IsChecked = false };
                checkBox.BindingContext = entry;

                var nameLabel = new Label
                {
                    Text = entry.DisplayName,
                    TextColor = GHColors.White,
                    FontFamily = "Underwood",
                    FontSize = 14,
                    VerticalOptions = LayoutOptions.Center
                };
                var detailLabel = new Label
                {
                    Text = entry.Outcome + " \u2014 Score: " + entry.Score.ToString("N0")
                           + " \u2014 " + entry.DeathDate
                           + " (" + FormatFileSize(entry.FileSize) + ")",
                    TextColor = GHColors.Gray,
                    FontFamily = "Underwood",
                    FontSize = 12,
                    VerticalOptions = LayoutOptions.Center,
                    Margin = new Thickness(28, 0, 0, 0)
                };

                var row = new StackLayout { Spacing = 2, Padding = new Thickness(4, 2) };
                var headerRow = new StackLayout
                {
                    Orientation = StackOrientation.Horizontal,
                    Spacing = 6
                };
                headerRow.Children.Add(checkBox);
                headerRow.Children.Add(nameLabel);
                row.Children.Add(headerRow);
                row.Children.Add(detailLabel);

                _dumplogCheckBoxes.Add(checkBox);
                DumplogListLayout.Children.Add(row);
            }

            /* Auto-select the newest */
            if (_dumplogCheckBoxes.Count > 0)
                _dumplogCheckBoxes[0].IsChecked = true;

            DumplogPickerGrid.IsVisible = true;
        }

        private async void DumplogSendButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            DumplogPickerGrid.IsVisible = false;

            var selected = _dumplogCheckBoxes
                .Where(cb => cb.IsChecked && cb.BindingContext is DumplogEntry)
                .Select(cb => cb.BindingContext as DumplogEntry)
                .ToList();

            if (selected.Count == 0)
                return;

            ProgressTitleLabel.Text = "Attaching Dumplogs";
            ProgressStatusLabel.Text = "Reading dumplog files...";
            UploadProgressBar.Progress = 0.2;
            ProgressOverlay.IsVisible = true;
            AttachButton.IsEnabled = false;

            try
            {
                var content = new MultipartFormDataContent();
                var sb = new StringBuilder();
                foreach (var entry in selected)
                {
                    sb.AppendLine("=== Dumplog: " + entry.DisplayName + " ===");
                    sb.AppendLine(File.ReadAllText(entry.FilePath));
                    sb.AppendLine();
                }
                content.Add(new StringContent(sb.ToString(), Encoding.UTF8, "text/html"), "DumplogContent");
                if (!string.IsNullOrEmpty(_sessionId))
                    content.Add(new StringContent(_sessionId), "SessionId");

                ProgressStatusLabel.Text = "Uploading " + selected.Count + " dumplog"
                                           + (selected.Count > 1 ? "s" : "") + "...";
                UploadProgressBar.Progress = 0.5;

                var cts = new CancellationTokenSource();
                using (var uploader = new HttpClientUploadWithProgress(
                    _baseOverseerUrl + "/api/session/attach", content, cts))
                {
                    uploader.ProgressChanged += (totalSize, uploaded, pct) =>
                    {
                        MainThread.BeginInvokeOnMainThread(() =>
                        {
                            if (pct.HasValue)
                            {
                                UploadProgressBar.Progress = 0.5 + (pct.Value / 100.0) * 0.45;
                                ProgressStatusLabel.Text = "Uploading... " + Math.Round(pct.Value) + "%";
                            }
                        });
                    };

                    await uploader.StartUpload();

                    if (uploader.Response.IsSuccessStatusCode)
                    {
                        ProgressStatusLabel.Text = "Dumplogs attached!";
                        UploadProgressBar.Progress = 1.0;
                    }
                    else
                    {
                        GHApp.WriteGHLog("Dumplog attach failed: HTTP " + (int)uploader.Response.StatusCode);
                        ProgressStatusLabel.Text = "Attach failed (HTTP " + (int)uploader.Response.StatusCode + ").";
                        UploadProgressBar.Progress = 1.0;
                    }
                }
            }
            catch (TaskCanceledException)
            {
                GHApp.WriteGHLog("Dumplog attach timed out.");
                ProgressStatusLabel.Text = "Upload timed out.";
                UploadProgressBar.Progress = 1.0;
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Dumplog attach failed: " + ex.Message);
                ProgressStatusLabel.Text = "Attach failed.";
                UploadProgressBar.Progress = 1.0;
            }

            await Task.Delay(1500);
            ProgressOverlay.IsVisible = false;
            ProgressTitleLabel.Text = "Gnoll Overseer";
            AttachButton.IsEnabled = true;
        }

        private void DumplogCancelButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            DumplogPickerGrid.IsVisible = false;
        }

        /* --- Screenshot Picker --- */
        private async void AttachScreenshotsButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            AttachTypeGrid.IsVisible = false;

            await GHApp.CheckAndRequestReadPermission(this);

            try
            {
                var pickOptions = new PickOptions
                {
                    PickerTitle = "Select screenshots to attach",
                    FileTypes = FilePickerFileType.Images
                };

#if GNH_MAUI
                var files = await FilePicker.Default.PickMultipleAsync(pickOptions);
#else
                var file = await FilePicker.PickAsync(pickOptions);
                var files = file != null ? new[] { file } : System.Array.Empty<FileResult>();
#endif

                if (files == null || !files.Any())
                    return;

                await UploadScreenshots(files.ToList());
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Screenshot pick failed: " + ex.Message);
                NavigationLabel.Text = "Failed to pick screenshots.";
            }
        }

        private async Task UploadScreenshots(List<FileResult> files)
        {
            ProgressTitleLabel.Text = "Attaching Screenshots";
            ProgressStatusLabel.Text = "Reading " + files.Count + " image"
                                       + (files.Count > 1 ? "s" : "") + "...";
            UploadProgressBar.Progress = 0.1;
            ProgressOverlay.IsVisible = true;
            AttachButton.IsEnabled = false;

            try
            {
                var content = new MultipartFormDataContent();
                foreach (var file in files)
                {
                    using (var stream = await file.OpenReadAsync())
                    {
                        var ms = new MemoryStream();
                        await stream.CopyToAsync(ms);
                        var byteContent = new ByteArrayContent(ms.ToArray());
                        byteContent.Headers.ContentType =
                            new System.Net.Http.Headers.MediaTypeHeaderValue(
                                file.ContentType ?? "image/png");
                        content.Add(byteContent, "Screenshots", file.FileName);
                    }
                }
                if (!string.IsNullOrEmpty(_sessionId))
                    content.Add(new StringContent(_sessionId), "SessionId");

                ProgressStatusLabel.Text = "Uploading...";
                UploadProgressBar.Progress = 0.3;

                var cts = new CancellationTokenSource();
                using (var uploader = new HttpClientUploadWithProgress(
                    _baseOverseerUrl + "/api/session/attach", content, cts))
                {
                    uploader.ProgressChanged += (totalSize, uploaded, pct) =>
                    {
                        MainThread.BeginInvokeOnMainThread(() =>
                        {
                            if (pct.HasValue)
                            {
                                UploadProgressBar.Progress = 0.3 + (pct.Value / 100.0) * 0.65;
                                ProgressStatusLabel.Text = "Uploading... "
                                    + Math.Round(pct.Value) + "%";
                            }
                        });
                    };

                    await uploader.StartUpload();

                    if (uploader.Response.IsSuccessStatusCode)
                    {
                        ProgressStatusLabel.Text = "Screenshots attached!";
                        UploadProgressBar.Progress = 1.0;
                    }
                    else
                    {
                        GHApp.WriteGHLog("Screenshot attach failed: HTTP "
                            + (int)uploader.Response.StatusCode);
                        ProgressStatusLabel.Text = "Attach failed.";
                        UploadProgressBar.Progress = 1.0;
                    }
                }
            }
            catch (TaskCanceledException)
            {
                GHApp.WriteGHLog("Screenshot upload timed out.");
                ProgressStatusLabel.Text = "Upload timed out.";
                UploadProgressBar.Progress = 1.0;
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Screenshot attach failed: " + ex.Message);
                ProgressStatusLabel.Text = "Attach failed.";
                UploadProgressBar.Progress = 1.0;
            }

            await Task.Delay(1500);
            ProgressOverlay.IsVisible = false;
            ProgressTitleLabel.Text = "Gnoll Overseer";
            AttachButton.IsEnabled = true;
        }

        /* --- Xlog/Dumplog Helpers --- */
        private static List<DumplogEntry> LoadDumplogEntries()
        {
            var entries = new List<DumplogEntry>();
            string xlogPath = Path.Combine(GHApp.GHPath, "xlogfile");
            string dumplogDir = Path.Combine(GHApp.GHPath, GHConstants.DumplogDirectory);

            if (!File.Exists(xlogPath) || !Directory.Exists(dumplogDir))
                return entries;

            try
            {
                string[] lines = File.ReadAllLines(xlogPath);
                foreach (string line in lines)
                {
                    if (string.IsNullOrWhiteSpace(line))
                        continue;

                    var tsi = new GHTopScoreItem(line);
                    string htmlFile = Path.Combine(dumplogDir, tsi.GetHTMLDumplogFileName());
                    string txtFile = Path.Combine(dumplogDir, tsi.GetDumplogFileName());

                    string filePath = File.Exists(htmlFile) ? htmlFile :
                                      File.Exists(txtFile) ? txtFile : null;

                    if (filePath != null)
                    {
                        entries.Add(new DumplogEntry
                        {
                            FilePath = filePath,
                            DisplayName = tsi.Name + " \u2014 " + tsi.CharacterString,
                            Outcome = tsi.Outcome ?? "",
                            Score = tsi.Score,
                            DeathDate = tsi.DeathDateString,
                            FileSize = new FileInfo(filePath).Length
                        });
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Failed to load dumplog entries: " + ex.Message);
            }

            entries.Reverse(); /* newest first */
            return entries;
        }

        private static string FormatFileSize(long bytes)
        {
            if (bytes < 1024) return bytes + " B";
            if (bytes < 1024 * 1024) return (bytes / 1024) + " KB";
            return (bytes / (1024 * 1024)) + " MB";
        }

        /* --- App Log / Panic Log Attach (Developer Mode only) --- */
        private async void AttachAppLogButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            AttachTypeGrid.IsVisible = false;
            string appLogPath = Path.Combine(GHApp.GHPath, GHConstants.AppLogDirectory, GHConstants.AppLogFileName);
            await UploadLogFile(appLogPath, "App Log");
        }

        private async void AttachPanicLogButton_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            AttachTypeGrid.IsVisible = false;
            string panicLogPath = Path.Combine(GHApp.GHPath, "paniclog");
            await UploadLogFile(panicLogPath, "Panic Log");
        }

        private async Task UploadLogFile(string filePath, string logName)
        {
            ProgressTitleLabel.Text = "Attaching " + logName;
            ProgressStatusLabel.Text = "Reading " + logName + "...";
            UploadProgressBar.Progress = 0.2;
            ProgressOverlay.IsVisible = true;
            AttachButton.IsEnabled = false;

            try
            {
                string logContent = File.ReadAllText(filePath);

                if (!(GHApp.DeveloperMode && GHApp.DebugLogMessages) && logContent.Length > GHConstants.MaxOverseerLogLengthNonDebug)
                {
                    logContent = logContent.Substring(logContent.Length - GHConstants.MaxOverseerLogLengthNonDebug);
                }

                var content = new MultipartFormDataContent();
                content.Add(new StringContent(logContent, Encoding.UTF8, "text/plain"),
                            "LogContent", Path.GetFileName(filePath));
                content.Add(new StringContent(logName), "LogName");
                if (!string.IsNullOrEmpty(_sessionId))
                    content.Add(new StringContent(_sessionId), "SessionId");

                ProgressStatusLabel.Text = "Uploading " + logName + "...";
                UploadProgressBar.Progress = 0.5;

                var cts = new CancellationTokenSource();
                using (var uploader = new HttpClientUploadWithProgress(
                    _baseOverseerUrl + "/api/session/attach", content, cts))
                {
                    uploader.ProgressChanged += (totalSize, uploaded, pct) =>
                    {
                        MainThread.BeginInvokeOnMainThread(() =>
                        {
                            if (pct.HasValue)
                            {
                                UploadProgressBar.Progress = 0.5 + (pct.Value / 100.0) * 0.45;
                                ProgressStatusLabel.Text = "Uploading... " + Math.Round(pct.Value) + "%";
                            }
                        });
                    };

                    await uploader.StartUpload();

                    if (uploader.Response.IsSuccessStatusCode)
                    {
                        ProgressStatusLabel.Text = logName + " attached!";
                        UploadProgressBar.Progress = 1.0;
                    }
                    else
                    {
                        GHApp.WriteGHLog(logName + " attach failed: HTTP " + (int)uploader.Response.StatusCode);
                        ProgressStatusLabel.Text = "Attach failed.";
                        UploadProgressBar.Progress = 1.0;
                    }
                }
            }
            catch (TaskCanceledException)
            {
                GHApp.WriteGHLog(logName + " upload timed out.");
                ProgressStatusLabel.Text = "Upload timed out.";
                UploadProgressBar.Progress = 1.0;
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog(logName + " attach failed: " + ex.Message);
                ProgressStatusLabel.Text = "Attach failed.";
                UploadProgressBar.Progress = 1.0;
            }

            await Task.Delay(1500);
            ProgressOverlay.IsVisible = false;
            ProgressTitleLabel.Text = "Gnoll Overseer";
            AttachButton.IsEnabled = true;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                await ClosePageAsync(false);
            }
            return false;
        }

        /* ===================================================================
         * v2: Client-Side Tool Bridge
         * ===================================================================
         * Allows the Overseer backend to request data from the running game
         * via the Angular SPA inside the WebView. The bridge uses platform-
         * specific mechanisms (WebView2 on Windows, JavascriptInterface on
         * Android, WKScriptMessageHandler on iOS) to receive tool requests
         * and returns results via EvaluateJavaScriptAsync.
         * =================================================================== */

        /// <summary>
        /// Sets up the platform-specific JS bridge on the WebView.
        /// All Handler-based code is wrapped in #if GNH_MAUI since
        /// Handler.PlatformView is a MAUI-only API.
        /// </summary>
        private void SetupJsBridge()
        {
#if GNH_MAUI
#if WINDOWS
            var webView2 = DisplayWebView.Handler?.PlatformView
                as Microsoft.UI.Xaml.Controls.WebView2;
            if (webView2?.CoreWebView2 != null)
            {
                webView2.CoreWebView2.WebMessageReceived += OnWebMessageReceived;
            }
#elif ANDROID
            var androidWebView = DisplayWebView.Handler?.PlatformView
                as Android.Webkit.WebView;
            if (androidWebView != null)
            {
                androidWebView.Settings.JavaScriptEnabled = true;
                androidWebView.AddJavascriptInterface(
                    new OverseerJsBridge(this), "GnollHackBridge");
            }
#elif IOS || MACCATALYST
            var wkWebView = DisplayWebView.Handler?.PlatformView
                as WebKit.WKWebView;
            if (wkWebView != null)
            {
                wkWebView.Configuration.UserContentController
                    .AddScriptMessageHandler(
                        new OverseerScriptMessageHandler(this),
                        "gnollhackBridge");
            }
#endif
#endif // GNH_MAUI
        }

        /// <summary>
        /// Unregisters the JS bridge on all platforms to prevent memory leaks.
        /// iOS WKUserContentController holds a strong reference to the handler;
        /// failing to remove it would leak the page.
        /// </summary>
        private void CleanupJsBridge()
        {
            if (!_bridgeInitialized)
                return;

#if GNH_MAUI
#if WINDOWS
            var webView2 = DisplayWebView.Handler?.PlatformView
                as Microsoft.UI.Xaml.Controls.WebView2;
            if (webView2?.CoreWebView2 != null)
            {
                webView2.CoreWebView2.WebMessageReceived -= OnWebMessageReceived;
            }
#elif ANDROID
            var androidWebView = DisplayWebView.Handler?.PlatformView
                as Android.Webkit.WebView;
            if (androidWebView != null)
            {
                androidWebView.RemoveJavascriptInterface("GnollHackBridge");
            }
#elif IOS || MACCATALYST
            var wkWebView = DisplayWebView.Handler?.PlatformView
                as WebKit.WKWebView;
            if (wkWebView != null)
            {
                wkWebView.Configuration.UserContentController
                    .RemoveScriptMessageHandler("gnollhackBridge");
            }
#endif
#endif // GNH_MAUI
            _bridgeInitialized = false;
        }

#if GNH_MAUI && WINDOWS
        private void OnWebMessageReceived(object sender,
            Microsoft.Web.WebView2.Core.CoreWebView2WebMessageReceivedEventArgs e)
        {
            string json = e.WebMessageAsJson;
            HandleToolRequest(json);
        }
#endif

        /// <summary>
        /// Public entry point for platform bridge classes (OverseerJsBridge,
        /// OverseerScriptMessageHandler) to forward tool requests.
        /// </summary>
        public void HandleToolRequestFromBridge(string json)
        {
            HandleToolRequest(json);
        }

        private static readonly HashSet<string> AllowedClientTools = new HashSet<string>
        {
            "get_full_message_history",
            "get_directory_listing",
            "refresh_snapshot",
            "get_save_info"
        };

        /// <summary>
        /// Parses and validates an incoming tool request from the Angular SPA,
        /// then dispatches execution to a background thread.
        /// </summary>
        private async void HandleToolRequest(string json)
        {
            try
            {
                var request = JsonConvert.DeserializeObject<ClientToolRequest>(json);

                if (request?.Type != "tool_client_request")
                    return;

                /* URL origin validation (awaited to avoid race condition) */
                string currentUrl = await MainThread.InvokeOnMainThreadAsync(() =>
                {
                    return (DisplayWebView.Source as UrlWebViewSource)?.Url;
                });

                if (currentUrl != null && !currentUrl.StartsWith(
                        _baseOverseerUrl, StringComparison.OrdinalIgnoreCase))
                {
                    GHApp.WriteGHLog("Tool request rejected: URL mismatch.");
                    return;
                }

                if (!AllowedClientTools.Contains(request.ToolName))
                {
                    SendToolResponse(request.RequestId, false, null,
                        "Unknown tool: " + request.ToolName);
                    return;
                }

                /* Dispatch to background thread for non-blocking execution */
                _ = Task.Run(async () =>
                {
                    try
                    {
                        string result = await DispatchToolCallAsync(
                            request.ToolName, request.Parameters);
                        SendToolResponse(request.RequestId, true, result);
                    }
                    catch (Exception ex)
                    {
                        SendToolResponse(request.RequestId, false, null,
                                         ex.Message);
                    }
                });
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Tool request parse error: " + ex.Message);
            }
        }

        private const int DefaultMessageLimit = 250;

        /// <summary>
        /// Executes the requested tool. Pure C# tools run on the background
        /// thread directly; P/Invoke tools are dispatched to the main thread
        /// via MainThread.InvokeOnMainThreadAsync for C core safety.
        /// </summary>
        /// <remarks>
        /// Threading rationale: Ideally native C library calls during gameplay
        /// would be dispatched to the GameThread (which owns the C core state).
        /// However, when Overseer is open via GameMenuPage the GameThread is in
        /// a Thread.Sleep loop waiting for user input, so native state is stable
        /// and reads from the UI thread are safe. When accessed via the About
        /// page there is no GameThread, so UI thread is the only option.
        /// </remarks>
        private async Task<string> DispatchToolCallAsync(string toolName,
            JObject parameters)
        {
            var currentGame = GHApp.CurrentGHGame;

            switch (toolName)
            {
            case "get_full_message_history":
                if (currentGame == null)
                    throw new InvalidOperationException("No active game");
                string history = currentGame.ExportFullMessageHistory();
                /* Optional search filtering */
                string searchTerm = parameters?["search_term"]?.ToString();
                if (!string.IsNullOrEmpty(searchTerm))
                {
                    var filtered = history.Split('\n')
                        .Where(l => l.IndexOf(searchTerm,
                            StringComparison.OrdinalIgnoreCase) >= 0);
                    history = string.Join("\n", filtered);
                }
                /* Limit message count (default 250 to avoid oversized JS payloads) */
                int lastN = DefaultMessageLimit;
                string lastNStr = parameters?["last_n"]?.ToString();
                if (!string.IsNullOrEmpty(lastNStr)
                    && int.TryParse(lastNStr, out int n) && n > 0)
                {
                    lastN = n;
                }
                var allLines = history.Split('\n');
                if (allLines.Length > lastN)
                    history = string.Join("\n",
                        allLines.Skip(allLines.Length - lastN));
                return history;

            case "get_directory_listing":
                /* Pure C# — safe on background thread */
                return GHGame.GenerateDirectoryManifest();

            case "refresh_snapshot":
                /* P/Invoke — dispatch to main thread for C core safety */
                return await MainThread.InvokeOnMainThreadAsync(() =>
                {
                    return GHApp.GnollHackService.GenerateAiSnapshot();
                });

            case "get_save_info":
                string savePath = parameters?["filename"]?.ToString();
                if (string.IsNullOrEmpty(savePath))
                    throw new ArgumentException("filename required");
                if (!savePath.StartsWith(GHApp.GnollHackService.GetGnollHackPath()))
                    throw new UnauthorizedAccessException(
                        "Invalid save file path");
                /* P/Invoke — dispatch to main thread for C core safety */
                return await MainThread.InvokeOnMainThreadAsync(() =>
                {
                    if (GHApp.GnollHackService.GetSaveFileDescription(savePath,
                            out string charDesc, out string locDesc,
                            out string modeDesc))
                    {
                        return "Character: " + charDesc
                            + "\nLocation: " + locDesc
                            + "\nMode: " + modeDesc;
                    }
                    throw new FileNotFoundException(
                        "Save file not found or invalid");
                });

            default:
                throw new NotSupportedException(
                    "Tool not implemented: " + toolName);
            }
        }

        /// <summary>
        /// Sends a tool response back to the Angular SPA via EvaluateJavaScriptAsync.
        /// Uses JsonConvert double-serialization for safe JS string injection.
        /// The Angular side must JSON.parse() the received string.
        /// </summary>
        private async void SendToolResponse(string requestId, bool success,
            string content, string errorMessage = null)
        {
            try
            {
                var response = new
                {
                    type = "tool_response",
                    requestId = requestId,
                    success = success,
                    content = content ?? "",
                    errorMessage = errorMessage
                };

                string json = JsonConvert.SerializeObject(response);
                /* Double-serialize to get a safely escaped JS string literal
                 * (includes surrounding quotes) */
                string jsStringLiteral = JsonConvert.SerializeObject(json);

                await MainThread.InvokeOnMainThreadAsync(async () =>
                {
                    await DisplayWebView.EvaluateJavaScriptAsync(
                        "window.onGnollHackToolResponse(" + jsStringLiteral + ")");
                });
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("SendToolResponse failed: " + ex.Message);
            }
        }
    }
}

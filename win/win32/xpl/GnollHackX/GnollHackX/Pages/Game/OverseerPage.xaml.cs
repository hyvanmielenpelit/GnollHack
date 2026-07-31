using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
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
        private string _messageHistory;
        private string _directoryManifest;
        private string _debugData;
        private List<CheckBox> _dumplogCheckBoxes = new List<CheckBox>();
        private List<DumplogEntry> _dumplogEntries = new List<DumplogEntry>();
        private string _sessionId = "";
        private bool _overseerLoaded = false;

        public OverseerPage(string title, string baseOverseerUrl,
                            string snapshotHtml, string messageHistory,
                            string directoryManifest, string debugData)
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
            _messageHistory = messageHistory;
            _directoryManifest = directoryManifest;
            _debugData = debugData;

            /* Show a simple loading page in the WebView while the overlay is visible */
            DisplayWebView.Source = new HtmlWebViewSource
            {
                Html = "<html><body style='background:#1a1a1a;color:#666;" +
                       "font-family:sans-serif;display:flex;align-items:center;" +
                       "justify-content:center;height:100vh;margin:0;'>" +
                       "<p>Connecting to Gnoll Overseer...</p></body></html>"
            };

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
                        if (!string.IsNullOrEmpty(_messageHistory))
                            content.Add(new StringContent(_messageHistory), "MessageHistory");
                        if (!string.IsNullOrEmpty(_directoryManifest))
                            content.Add(new StringContent(_directoryManifest), "DirectoryManifest");
                        if (!string.IsNullOrEmpty(_debugData))
                            content.Add(new StringContent(_debugData), "DebugData");

                        /* Send Overseer settings as typed dictionaries */
                        bool isGameOn = GHApp.CurrentGamePage?.IsGameOn ?? false;
                        var boolSettings = new Dictionary<string, bool>
                        {
                            { "allowSpoilers", GHApp.OverseerAllowSpoilers },
                            { "verboseResponses", GHApp.OverseerVerboseResponses },
                            { "sendGameContext", GHApp.OverseerSendGameContext },
                            { "isGameOn", isGameOn },
                            { "developerMode", GHApp.DeveloperMode },
                            { "debugLogMessages", GHApp.DebugLogMessages }
                        };
                        int overseerMode = (GHApp.DeveloperMode && GHApp.DebugLogMessages) ? 2 : (isGameOn ? 0 : 1);
                        var intSettings = new Dictionary<string, int>
                        {
                            { "overseerMode", overseerMode }
                        };
                        var stringSettings = new Dictionary<string, string>();
                        var overseerSettings = new
                        {
                            boolSettings = boolSettings,
                            intSettings = intSettings,
                            stringSettings = stringSettings
                        };
                        string settingsJson = Newtonsoft.Json.JsonConvert.SerializeObject(overseerSettings);
                        content.Add(new StringContent(settingsJson, Encoding.UTF8, "application/json"),
                                    "OverseerSettings");

                        /* Default initial prompt */
                        content.Add(new StringContent(
                            "Analyze my current game state and suggest what I should do next."),
                            "InitialPrompt");

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

                            ProgressStatusLabel.Text = "Connected!";
                            UploadProgressBar.Progress = 1.0;
                        }
                        else
                        {
                            string msg = "Overseer session failed: HTTP " + (int)response.StatusCode;
                            GHApp.WriteGHLog(msg);
                            ProgressStatusLabel.Text = "Connection failed. Opening without game context.";
                            UploadProgressBar.Progress = 1.0;
                            await Task.Delay(2000);
                        }
                    }
                }
            }
            catch (TaskCanceledException)
            {
                GHApp.WriteGHLog("Overseer upload timed out after 10 seconds.");
                ProgressStatusLabel.Text = "Connection timed out. Opening without game context.";
                UploadProgressBar.Progress = 1.0;
                await Task.Delay(2000);
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Overseer upload failed: " + ex.Message);
                ProgressStatusLabel.Text = "Upload failed. Opening without game context.";
                UploadProgressBar.Progress = 1.0;
                await Task.Delay(2000);
            }

            /* Hide overlay and navigate to the final URL */
            ProgressOverlay.IsVisible = false;
            DisplayWebView.Source = new UrlWebViewSource { Url = overseerUrl };

            /* Free the data references - they can be large */
            _snapshotHtml = null;
            _messageHistory = null;
            _directoryManifest = null;
            _debugData = null;
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
    }
}

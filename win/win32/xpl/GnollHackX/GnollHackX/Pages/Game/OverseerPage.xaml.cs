using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Diagnostics;

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
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class OverseerPage : CustomModalPage, ICloseablePage, IMessagePopupPage
    {
        private string _baseOverseerUrl;
        private string _snapshotHtml;

        private string _sessionId = "";
        private bool _overseerLoaded = false;
        private bool _bridgeInitialized = false;
        private DateTime _lastFailedNavigatedTime = DateTime.MinValue;
        private string _lastFailedNavigatedUrl = null;
        private bool _navigatedAwayFromSpa = false;
        private string _lastSpaUrl = null;
        private bool _handoffSucceeded = false;
        private CancellationTokenSource _connectCts;
#if DEBUG
#if IOS || MACCATALYST
        private object _iosNavigationDelegate = null;
#endif
#endif
#if IOS || MACCATALYST
        private object _iosPickerDelegate = null;
#endif

        public OverseerPage(string baseOverseerUrl, string snapshotHtml)
        {
            InitializeComponent();

            _baseOverseerUrl = baseOverseerUrl;
            _snapshotHtml = snapshotHtml;

#if GNH_MAUI && WINDOWS
            /* Highlight the close area on pointer hover (Windows only) */
            var pointerGesture = new PointerGestureRecognizer();
            pointerGesture.PointerEntered += (s, e) =>
            {
                CloseArea.BackgroundColor = GHColors.OverseerPointerOverlayBkgColor;
            };
            pointerGesture.PointerExited += (s, e) =>
            {
                CloseArea.BackgroundColor = Color.FromRgba(0, 0, 0, 0);
            };
            CloseArea.GestureRecognizers.Add(pointerGesture);
#endif
            GHGame curGame = GHApp.CurrentGHGame;
            if (curGame != null)
            {
                curGame.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.PlayMusic, GHConstants.OverseerGHSound, GHConstants.OverseerEventPath, GHConstants.IntroBankId, GHConstants.OverseerVolume, 1.0f));
            }
            else
            {
                GHApp.FmodService.StopAllUISounds();
                //GHApp.FmodService.LoadBanks(sound_bank_loading_type.Music);
                GHApp.FmodService.PlayUIMusic(GHConstants.OverseerGHSound, GHConstants.OverseerEventPath, GHConstants.IntroBankId, GHConstants.OverseerVolume, 1.0f);
            }
        }

        private async void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            GHApp.SetPlatformResizeAdjustment(true);
            try
            {
                await UploadAndConnect();
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Unhandled exception in OverseerPage: " + ex.Message);
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            _connectCts?.Cancel();
            GHApp.SetPlatformResizeAdjustment(false);
            GHApp.BackButtonPressed -= BackButtonPressed;
            CleanupJsBridge();

            /* Navigate the WebView to blank to release WKWebView content
             * process memory. Without this, each Overseer session accumulates
             * web content that can exhaust GPU memory on memory-constrained
             * devices (e.g. old iPads), causing SkiaSharp surfaces to fail
             * allocation. */
            try
            {
                DisplayWebView.Source = new UrlWebViewSource { Url = "about:blank" };
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(
                    "WebView blank navigation failed: " + ex.Message);
            }
        }
        protected override bool OnBackButtonPressed()
        {
            return true;
        }

        private async Task UploadAndConnect()
        {
            _connectCts?.Dispose();
            _connectCts = new CancellationTokenSource();

            _handoffSucceeded = false;
            RetryButtonsPanel.IsVisible = false;
            CancelButton.IsVisible = true;
            ProgressOverlay.IsVisible = true;
            ProgressStatusLabel.Text = "Uploading game data...";
            UploadProgressBar.Progress = 0.3;

            string overseerUrl = _baseOverseerUrl;
            bool isLocalDev = UIUtils.IsLocalUrl(_baseOverseerUrl);
            string connectAddress;
#if DEBUG
            try
            {
                Uri uri = new Uri(_baseOverseerUrl);
                connectAddress = $"{uri.Host}:{uri.Port}";
            }
            catch
            {
                connectAddress = _baseOverseerUrl;
            }
#else
            connectAddress = GHApp.UseDebugPostChannel ? "Test Overseer Server" : "Overseer Server";
#endif

            (int Timeout, int Delay)[] attemptConfig = 
            {
                (10, 1),  /* Attempt 1: 10s timeout (for large snapshots), then wait 1s */
                (10, 5),  /* Attempt 2: 10s timeout, then wait 5s */
                (10, 10), /* Attempt 3: 10s timeout, then wait 10s */
                (15, 15), /* Attempt 4: 15s timeout, then wait 15s */
                (15, 20), /* Attempt 5: 15s timeout, then wait 20s */
                (20, 30), /* Attempt 6: 20s timeout, then wait 30s */
                (25, 0)   /* Attempt 7: 25s timeout, then give up (no delay needed) */
            };

            for (int attempt = 0; attempt < attemptConfig.Length; attempt++)
            {
                System.Diagnostics.Stopwatch stopwatch = new System.Diagnostics.Stopwatch();
                try
                {
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        ProgressStatusLabel.Text = attempt == 0
                            ? $"Contacting {connectAddress}"
                            : ($"Retrying to contact {connectAddress}" + (attempt >= 2 ? (" (" + attempt + ")") : ""));
                        
                        double progress = 0.3 + (0.6 * (attempt + 1.0) / attemptConfig.Length);
                        UploadProgressBar.Progress = progress;
                        
                        if (isLocalDev)
                        {
                            ErrorDetailsLabel.Text = "...";
                        }
                    });

#if DEBUG
                    using (var httpClient = UIUtils.CreateHttpClientForUrl(
                        _baseOverseerUrl, TimeSpan.FromSeconds(attemptConfig[attempt].Timeout)))
#else
                    using (var httpClient = new HttpClient() { Timeout = TimeSpan.FromSeconds(attemptConfig[attempt].Timeout) })
#endif
                    {
                        using (var content = new MultipartFormDataContent())
                        {
#if DEBUG
                            bool useLocalCreds = GHApp.OverseerUseLocalAddress
                                && !string.IsNullOrEmpty(GHApp.LocalOverseerAddress);
                            string userName = useLocalCreds
                                ? (GHApp.LocalOverseerUserName ?? "")
                                : (GHApp.XlogUserName ?? "");
                            string password = useLocalCreds
                                ? (GHApp.LocalOverseerPassword ?? "")
                                : (GHApp.XlogPassword ?? "");
#else
                            string userName = GHApp.XlogUserName ?? "";
                            string password = GHApp.XlogPassword ?? "";
#endif
                            content.Add(new StringContent(userName), "UserName");
                            content.Add(new StringContent(password), "Password");
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
                            content.Add(new StringContent("true"), "IsGnollHackSession");

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

                            stopwatch.Start();
                            var response = await httpClient.PostAsync(
                                _baseOverseerUrl + "/api/session/create", content, _connectCts.Token);
                            stopwatch.Stop();

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
                                _handoffSucceeded = true;
                                break;
                            }
                            else
                            {
                                string msg = "Overseer session failed: HTTP " + (int)response.StatusCode;
                                if (response.StatusCode == System.Net.HttpStatusCode.BadRequest || response.StatusCode == System.Net.HttpStatusCode.Unauthorized)
                                {
                                    msg += " (Please check your credentials in Settings)";
                                    GHApp.WriteGHLog(msg);
                                    MainThread.BeginInvokeOnMainThread(() =>
                                    {
                                        ProgressStatusLabel.Text = "Connection failed. Please check your credentials in Settings.";
                                        UploadProgressBar.Progress = 1.0;
                                    });
                                    break;
                                }
                                GHApp.WriteGHLog(msg);
                                
                                string errorMsg = $"HTTP {(int)response.StatusCode} ({stopwatch.ElapsedMilliseconds / 1000.0:F1}s)";
                                if (isLocalDev)
                                {
                                    MainThread.BeginInvokeOnMainThread(() => ErrorDetailsLabel.Text = errorMsg);
                                }
                                
                                if (attempt < attemptConfig.Length - 1)
                                {
                                    int delaySeconds = attemptConfig[attempt].Delay;
                                    for (int remaining = delaySeconds; remaining > 0; remaining--)
                                    {
                                        if (isLocalDev)
                                        {
                                            MainThread.BeginInvokeOnMainThread(() =>
                                            {
                                                ProgressStatusLabel.Text = $"{errorMsg}... Retrying in {remaining}s";
                                            });
                                        }
                                        await Task.Delay(1000, _connectCts.Token);
                                    }
                                    continue;
                                }
                                MainThread.BeginInvokeOnMainThread(() =>
                                {
                                    ProgressStatusLabel.Text = $"Connection failed (HTTP {(int)response.StatusCode}). Please try again.";
                                    UploadProgressBar.Progress = 1.0;
                                    if (isLocalDev)
                                        ErrorDetailsLabel.Text = $"{errorMsg} - Failed";
                                });
                            }
                        }
                    }
                }
                catch (OperationCanceledException) when (_connectCts.IsCancellationRequested)
                {
                    /* User pressed Cancel or page was closed — exit the loop silently */
                    break;
                }
                catch (TaskCanceledException)
                {
                    string errorMsg = $"Timed out ({stopwatch.ElapsedMilliseconds / 1000.0:F1}s)";
                    if (isLocalDev)
                    {
                        MainThread.BeginInvokeOnMainThread(() => ErrorDetailsLabel.Text = errorMsg);
                    }
                    if (attempt < attemptConfig.Length - 1)
                    {
                        int delaySeconds = attemptConfig[attempt].Delay;
                        try
                        {
                            for (int remaining = delaySeconds; remaining > 0; remaining--)
                            {
                                if (isLocalDev)
                                {
                                    MainThread.BeginInvokeOnMainThread(() =>
                                    {
                                        ErrorDetailsLabel.Text = $"{errorMsg}... Retrying in {remaining}s";
                                    });
                                }
                                await Task.Delay(1000, _connectCts.Token);
                            }
                        }
                        catch (OperationCanceledException)
                        {
                            break;
                        }
                        continue;
                    }
                    GHApp.WriteGHLog("Overseer upload timed out after " + (attempt + 1) + " attempts.");
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        ProgressStatusLabel.Text = "Connection timed out. The server may be starting up.";
                        UploadProgressBar.Progress = 1.0;
                        if (isLocalDev)
                            ErrorDetailsLabel.Text = $"{errorMsg} - Failed";
                    });
                }
                catch (Exception ex)
                {
                    string errorMsg = $"{ex.Message} ({stopwatch.ElapsedMilliseconds / 1000.0:F1}s)";
                    if (isLocalDev)
                    {
                        MainThread.BeginInvokeOnMainThread(() => ErrorDetailsLabel.Text = errorMsg);
                    }
                    if (attempt < attemptConfig.Length - 1)
                    {
                        int delaySeconds = attemptConfig[attempt].Delay;
                        try
                        {
                            for (int remaining = delaySeconds; remaining > 0; remaining--)
                            {
                                if (isLocalDev)
                                {
                                    MainThread.BeginInvokeOnMainThread(() =>
                                    {
                                        ErrorDetailsLabel.Text = $"{errorMsg}... Retrying in {remaining}s";
                                    });
                                }
                                await Task.Delay(1000, _connectCts.Token);
                            }
                        }
                        catch (OperationCanceledException)
                        {
                            break;
                        }
                        continue;
                    }
                    GHApp.WriteGHLog("Overseer upload failed: " + ex.Message);
                    MainThread.BeginInvokeOnMainThread(() =>
                    {
                        ProgressStatusLabel.Text = "Connection failed. Please try again.";
                        UploadProgressBar.Progress = 1.0;
                        if (isLocalDev)
                            ErrorDetailsLabel.Text = $"{errorMsg} - Failed";
                    });
                }
            }

            MainThread.BeginInvokeOnMainThread(() =>
            {
                ConfigureSslBypass();
                CancelButton.IsVisible = false;

                if (_handoffSucceeded)
                {
                    /* Hide overlay and navigate to the final URL */
                    ProgressOverlay.IsVisible = false;
                    DisplayWebView.Source = new UrlWebViewSource { Url = overseerUrl };
                }
                else
                {
                    /* Don't navigate to bare URL — show retry/close buttons instead */
                    RetryButtonsPanel.IsVisible = true;
                }
            });

            /* Free the data references - they can be large */
            _snapshotHtml = null;
        }

        private async void RetryButton_Clicked(object sender, EventArgs e)
        {
            try
            {
                await UploadAndConnect();
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Unhandled exception in OverseerPage retry: " + ex.Message);
            }
        }

        private async void CancelButton_Clicked(object sender, EventArgs e)
        {
            _connectCts?.Cancel();
            await ClosePageAsync(true);
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync(false);
        }

        private void DisplayWebView_Navigating(object sender, WebNavigatingEventArgs e)
        {
            if (_lastFailedNavigatedUrl != null && (DateTime.UtcNow - _lastFailedNavigatedTime).TotalSeconds < 1.0 && _lastFailedNavigatedUrl == e.Url)
            {
                LoadingIndicator.IsRunning = false;
                LoadingIndicator.IsVisible = false;
                return;
            }

            LoadingIndicator.IsRunning = true;
            LoadingIndicator.IsVisible = true;
        }

        private void DisplayWebView_Navigated(object sender, WebNavigatedEventArgs e)
        {
            LoadingIndicator.IsRunning = false;
            LoadingIndicator.IsVisible = false;

            if (e.Result != WebNavigationResult.Success)
            {
                _lastFailedNavigatedTime = DateTime.UtcNow;
                _lastFailedNavigatedUrl = e.Url;
            }

            /* Track when the real Overseer page has loaded successfully */
            if (!_overseerLoaded && e.Result == WebNavigationResult.Success
                && e.Url != null && e.Url.StartsWith("http", StringComparison.OrdinalIgnoreCase))
            {
                _overseerLoaded = true;
                
                FocusDisplayWebView();
#if GNH_MAUI
                var focusTimer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                focusTimer.Interval = TimeSpan.FromMilliseconds(200);
                focusTimer.IsRepeating = false;
                focusTimer.Tick += (s, ev) =>
                { 
                    FocusDisplayWebView(); 
                };
                focusTimer.Start();
#else
                Device.StartTimer(TimeSpan.FromMilliseconds(200), () =>
                {
                    FocusDisplayWebView();
                    return false;
                });
#endif
            }

            /* v2: Initialize JS bridge for client tool support (once only) */
            if (!_bridgeInitialized && _overseerLoaded && GHApp.OverseerEnableClientTools)
            {
                SetupJsBridge();
                _bridgeInitialized = true;
            }

            /* Track when the user navigates away from the Overseer SPA
             * to server-rendered pages (e.g. /Identity/Account/Register).
             * The title label becomes a "return" button in this state. */
            if (_overseerLoaded && e.Result == WebNavigationResult.Success
                && e.Url != null)
            {
                bool isAwayFromSpa = false;
                try
                {
                    var uri = new Uri(e.Url);
                    var baseUri = new Uri(_baseOverseerUrl);
                    
                    if (!string.Equals(uri.Host, baseUri.Host, StringComparison.OrdinalIgnoreCase) || 
                        uri.Port != baseUri.Port)
                    {
                        isAwayFromSpa = true;
                    }
                    else if (uri.AbsolutePath.StartsWith("/Identity/", StringComparison.OrdinalIgnoreCase))
                    {
                        isAwayFromSpa = true;
                    }
                }
                catch (UriFormatException) { }

                if (_navigatedAwayFromSpa != isAwayFromSpa)
                {
                    _navigatedAwayFromSpa = isAwayFromSpa;
                    UpdateTitleAppearance();
                }

                if (!isAwayFromSpa)
                {
                    if (e.Url != null && !e.Url.Contains("/api/auth/handoff"
#if GNH_MAUI
                        , StringComparison.OrdinalIgnoreCase
#endif
                        ))
                    {
                        _lastSpaUrl = e.Url;
                    }
                }
            }
        }

#if GNH_MAUI && WINDOWS
        [DllImport("user32.dll")]
        private static extern IntPtr SetFocus(IntPtr hWnd);

        private delegate bool EnumChildWindowsProc(IntPtr hWnd, IntPtr lParam);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool EnumChildWindows(
            IntPtr hwndParent, EnumChildWindowsProc lpEnumFunc, IntPtr lParam);

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        private static extern int GetClassName(
            IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);
#endif

        private void FocusDisplayWebView()
        {
#if GNH_MAUI && WINDOWS
            try
            {
                var mauiWindow = Microsoft.Maui.Controls.Application.Current?.Windows
                    ?.FirstOrDefault();
                var nativeWindow = mauiWindow?.Handler?.PlatformView
                    as Microsoft.UI.Xaml.Window;
                if (nativeWindow != null)
                {
                    var mainHwnd = WinRT.Interop.WindowNative
                        .GetWindowHandle(nativeWindow);
                    if (mainHwnd != IntPtr.Zero)
                    {
                        System.Diagnostics.Debug.WriteLine($"[OverseerPage] FocusDisplayWebView: Found mainHwnd: {mainHwnd}");
                        IntPtr chromeHwnd = IntPtr.Zero;
                        EnumChildWindows(mainHwnd, (hWnd, lParam) =>
                        {
                            var sb = new StringBuilder(256);
                            GetClassName(hWnd, sb, 256);
                            string className = sb.ToString();
                            System.Diagnostics.Debug.WriteLine($"[OverseerPage] Child Window HWND: {hWnd}, Class: '{className}'");
                            
                            if (className.Contains("Chrome_WidgetWin") || className.Contains("WebView2"))
                            {
                                chromeHwnd = hWnd;
                                return false; /* stop enumeration */
                            }
                            return true;
                        }, IntPtr.Zero);

                        System.Diagnostics.Debug.WriteLine($"[OverseerPage] FocusDisplayWebView: Found chromeHwnd: {chromeHwnd}");
                        if (chromeHwnd != IntPtr.Zero)
                        {
                            IntPtr prevHwnd = SetFocus(chromeHwnd);
                            System.Diagnostics.Debug.WriteLine($"[OverseerPage] FocusDisplayWebView: SetFocus called. prevHwnd: {prevHwnd}");
                            return;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(
                    "FocusWebView2Native failed: " + ex.Message);
            }
#endif
            /* Fallback for non-Windows platforms */
            DisplayWebView.Focus();
        }

        private void TitleArea_Tapped(object sender, EventArgs e)
        {
            if (!_navigatedAwayFromSpa)
                return; /* Do nothing if already on the Overseer SPA */

            /* Navigate back to the active chat session in the Overseer SPA,
             * or fallback to base /chat URL if no session is known */
            string returnUrl;
            if (!string.IsNullOrEmpty(_sessionId))
            {
                returnUrl = _baseOverseerUrl.TrimEnd('/') + "/chat?sessionId=" + _sessionId;
            }
            else if (!string.IsNullOrEmpty(_lastSpaUrl))
            {
                returnUrl = _lastSpaUrl;
            }
            else
            {
                returnUrl = _baseOverseerUrl.TrimEnd('/') + "/chat";
            }

            DisplayWebView.Source = new UrlWebViewSource
            {
                Url = returnUrl
            };
        }

        private void UpdateTitleAppearance()
        {
            /* Visual hint: show left arrow and underline title when
             * navigated away, hide arrow and remove underline when
             * back on the SPA */
            if (_navigatedAwayFromSpa)
            {
                TitleArrowLabel.IsVisible = true;
                TitleLabel.TextDecorations = TextDecorations.Underline;
            }
            else
            {
                TitleArrowLabel.IsVisible = false;
                TitleLabel.TextDecorations = TextDecorations.None;
            }
        }

        private async void CloseArea_Tapped(object sender, EventArgs e)
        {
            if (_backPressed)
                return;

            if (_overseerLoaded)
            {
                bool confirmed = await GHApp.DisplayMessageBox(
                    this, "Leave Gnoll Overseer",
                    "Are you sure you want to leave Gnoll Overseer?",
                    "Leave", "Cancel");
                if (!confirmed)
                    return;
            }

            await ClosePageAsync(true);
        }

        private async Task ClosePageAsync(bool playClickSound)
        {
            if (_backPressed)
                return;
            _backPressed = true;
            if (playClickSound)
                GHApp.PlayButtonClickedSound();
            GHGame curGame = GHApp.CurrentGHGame;
            if (curGame != null)
            {
                curGame.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.UpdateGameMusic));
            }
            else
            {
                GHApp.FmodService.PlayUIMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);
                //GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Music);
            }
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
                        if (!_backPressed)
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



        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                if (_overseerLoaded)
                {
                    bool confirmed = await GHApp.DisplayMessageBox(
                        this, "Leave Gnoll Overseer",
                        "Are you sure you want to leave Gnoll Overseer?",
                        "Leave", "Cancel");
                    if (!confirmed)
                        return false;
                }
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
        /// Configures the platform WebView to accept self-signed SSL certificates
        /// for local/private Overseer URLs. Uses custom MauiWebViewClient (Android)
        /// and MauiWebViewNavigationDelegate (iOS) subclasses to preserve MAUI's
        /// Navigating/Navigated event routing.
        /// </summary>
        private void ConfigureSslBypass()
        {
#if DEBUG
            if (!UIUtils.IsLocalUrl(_baseOverseerUrl))
                return;

#if GNH_MAUI
#if ANDROID
            var webViewHandler = DisplayWebView.Handler
                as Microsoft.Maui.Handlers.WebViewHandler;
            var androidWebView = webViewHandler?.PlatformView;
            if (androidWebView != null && webViewHandler != null)
            {
                androidWebView.SetWebViewClient(
                    new GnollHackWebViewClient(webViewHandler));
            }
#elif IOS || MACCATALYST
            var webViewHandler = DisplayWebView.Handler
                as Microsoft.Maui.Handlers.WebViewHandler;
            var wkWebView = DisplayWebView.Handler?.PlatformView
                as WebKit.WKWebView;
            if (wkWebView != null && webViewHandler != null)
            {
                _iosNavigationDelegate =
                    new GnollHackNavigationDelegate(webViewHandler);
                wkWebView.NavigationDelegate =
                    (WebKit.IWKNavigationDelegate)_iosNavigationDelegate;
            }
#endif
#endif // GNH_MAUI
#endif // DEBUG
        }

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
            //if (webView2 != null)
            //{
            //    webView2.GotFocus += (s, e) => { System.Diagnostics.Debug.WriteLine("[OverseerPage] PlatformView (WebView2) GotFocus"); };
            //    webView2.LostFocus += (s, e) => { System.Diagnostics.Debug.WriteLine("[OverseerPage] PlatformView (WebView2) LostFocus"); };
            //}

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
            HandleWebMessage(json);
        }
#endif

        /// <summary>
        /// Public entry point for platform bridge classes (OverseerJsBridge,
        /// OverseerScriptMessageHandler) to forward messages from the Angular SPA.
        /// </summary>
        public void HandleWebMessageFromBridge(string json)
        {
            HandleWebMessage(json);
        }

        /// <summary>
        /// Central dispatcher for all incoming web messages from the Angular SPA.
        /// Routes file share requests to <see cref="HandleFileShareRequest"/>
        /// and AI tool requests to <see cref="HandleToolRequest"/>.
        /// </summary>
        private void HandleWebMessage(string json)
        {
            try
            {
                var jObject = JObject.Parse(json);
                string type = jObject["type"]?.ToString();

                if (type == "share_text_file")
                {
                    string filename = jObject["filename"]?.ToString();
                    string content = jObject["content"]?.ToString();

                    if (!string.IsNullOrEmpty(filename) && content != null)
                    {
                        HandleFileShareRequest(filename, content);
                    }
                    return;
                }

                if (type == "pick_files")
                {
                    double srcX = 0, srcY = 0, srcW = 0, srcH = 0;
                    var srcRect = jObject["sourceRect"];
                    if (srcRect != null)
                    {
                        srcX = srcRect["x"]?.Value<double>() ?? 0;
                        srcY = srcRect["y"]?.Value<double>() ?? 0;
                        srcW = srcRect["width"]?.Value<double>() ?? 0;
                        srcH = srcRect["height"]?.Value<double>() ?? 0;
                    }
                    HandlePickFilesRequest(srcX, srcY, srcW, srcH);
                    return;
                }

                if (type == "session_changed")
                {
                    string newSessionId = jObject["sessionId"]?.ToString();
                    if (!string.IsNullOrEmpty(newSessionId))
                    {
                        _sessionId = newSessionId;
                    }
                    return;
                }

                HandleToolRequest(json);
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("Web message parse error: " + ex.Message);
            }
        }

        private static readonly HashSet<string> AllowedClientTools = new HashSet<string>
        {
            "get_full_message_history",
            "get_directory_listing",
            "refresh_snapshot",
            "get_save_info",
            "get_player_library",
            "get_oracle_consultations",
            "get_player_xlog",
            "get_player_dumplogs",
            "get_app_log",
            "get_panic_log"
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

        /// <summary>
        /// Handles a file share request from the Angular SPA.
        /// Writes the provided text content to a temporary file and invokes
        /// the native OS share dialog via <see cref="GHApp.ShareFile"/>.
        /// </summary>
        private void HandleFileShareRequest(string filename, string content)
        {
            _ = Task.Run(async () =>
            {
                try
                {
                    string tempPath = Path.Combine(FileSystem.CacheDirectory, filename);
                    File.WriteAllText(tempPath, content);
                    await MainThread.InvokeOnMainThreadAsync(async () =>
                    {
                        await GHApp.ShareFile(this, tempPath, "Overseer Debug Log");
                    });
                }
                catch (Exception ex)
                {
                    GHApp.WriteGHLog("Error sharing file: " + ex.Message);
                }
            });
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

            case "get_player_library":
                return GetPlayerLibraryResult(parameters);

            case "get_oracle_consultations":
                return GetOracleConsultationsResult(parameters);

            case "get_player_xlog":
                return GetPlayerXlogResult(parameters);

            case "get_player_dumplogs":
                return GetPlayerDumplogsResult(parameters);

            case "get_app_log":
                return GetAppLogResult(parameters);

            case "get_panic_log":
                return GetPanicLogResult(parameters);

            default:
                throw new NotSupportedException(
                    "Tool not implemented: " + toolName);
            }
        }

        /// <summary>
        /// Returns the player's discovered manuals and catalogues from the game vault.
        /// List mode (no item_id): returns just names and IDs.
        /// Read mode (item_id specified): returns full text of that specific manual.
        /// </summary>
        private string GetPlayerLibraryResult(JObject parameters)
        {
            var manuals = new Dictionary<int, StoredManual>();
            GHApp.PopulateManuals(manuals);

            if (manuals.Count == 0)
                return "[]  /* No manuals or catalogues found in the player's library. */";

            string itemIdStr = parameters?["item_id"]?.ToString();
            if (string.IsNullOrEmpty(itemIdStr))
            {
                /* List mode — return just names and IDs */
                var listing = new List<object>();
                foreach (var kvp in manuals)
                {
                    listing.Add(new
                    {
                        id = kvp.Key,
                        name = kvp.Value.Name ?? ""
                    });
                }
                return JsonConvert.SerializeObject(listing);
            }
            else
            {
                /* Read mode — return full text of a specific manual */
                if (!int.TryParse(itemIdStr, out int itemId))
                    throw new ArgumentException("item_id must be an integer");

                if (!manuals.TryGetValue(itemId, out StoredManual manual))
                    throw new ArgumentException(
                        "No manual found with item_id " + itemId);

                return JsonConvert.SerializeObject(new
                {
                    id = itemId,
                    name = manual.Name ?? "",
                    text = manual.Text ?? ""
                });
            }
        }

        /// <summary>
        /// Returns the Oracle of Delphi major consultations the player has received.
        /// List mode (no item_id): returns just names and IDs.
        /// Read mode (item_id specified): returns full text of that consultation.
        /// </summary>
        private string GetOracleConsultationsResult(JObject parameters)
        {
            var consultations = new Dictionary<int, StoredManual>();
            GHApp.PopulateHints(consultations,
                GHConstants.OracleMajorConsultationFilePrefix);

            if (consultations.Count == 0)
                return "[]  /* No Oracle consultations received yet. */";

            string itemIdStr = parameters?["item_id"]?.ToString();
            if (string.IsNullOrEmpty(itemIdStr))
            {
                /* List mode — return just names and IDs */
                var listing = new List<object>();
                foreach (var kvp in consultations)
                {
                    listing.Add(new
                    {
                        id = kvp.Key,
                        name = kvp.Value.Name ?? ""
                    });
                }
                return JsonConvert.SerializeObject(listing);
            }
            else
            {
                /* Read mode — return full text of a specific consultation */
                if (!int.TryParse(itemIdStr, out int itemId))
                    throw new ArgumentException("item_id must be an integer");

                if (!consultations.TryGetValue(itemId, out StoredManual consultation))
                    throw new ArgumentException(
                        "No consultation found with item_id " + itemId);

                return JsonConvert.SerializeObject(new
                {
                    id = itemId,
                    name = consultation.Name ?? "",
                    text = consultation.Text ?? ""
                });
            }
        }

        private const int DefaultMaxDumplogChars = 4000;
        private const int DefaultXlogLimit = 50;

        /// <summary>
        /// Returns entries from the player's local xlogfile with rich metadata.
        /// Supports pagination via limit/offset parameters. Newest entries first.
        /// </summary>
        private string GetPlayerXlogResult(JObject parameters)
        {
            string xlogPath = Path.Combine(GHApp.GHPath, "xlogfile");
            string dumplogDir = Path.Combine(GHApp.GHPath,
                GHConstants.DumplogDirectory);

            if (!File.Exists(xlogPath))
                return "[]  /* No xlogfile found on this device. */";

            int limit = DefaultXlogLimit;
            int offset = 0;
            string limitStr = parameters?["limit"]?.ToString();
            string offsetStr = parameters?["offset"]?.ToString();
            if (!string.IsNullOrEmpty(limitStr))
            {
                if (int.TryParse(limitStr, out int parsedLimit)
                    && parsedLimit > 0)
                    limit = parsedLimit;
            }
            if (!string.IsNullOrEmpty(offsetStr))
            {
                if (int.TryParse(offsetStr, out int parsedOffset)
                    && parsedOffset >= 0)
                    offset = parsedOffset;
            }

            string[] allLines;
            try
            {
                allLines = File.ReadAllLines(xlogPath);
            }
            catch (Exception ex)
            {
                return "Error reading xlogfile: " + ex.Message;
            }

            /* Filter blanks, reverse so newest is first */
            var validLines = allLines
                .Where(l => !string.IsNullOrWhiteSpace(l))
                .Reverse()
                .ToArray();

            int totalCount = validLines.Length;
            var page = validLines.Skip(offset).Take(limit).ToArray();

            bool dirExists = Directory.Exists(dumplogDir);
            var result = new List<object>();
            foreach (string line in page)
            {
                var tsi = new GHTopScoreItem(line);

                string dumplogFilename = null;
                bool hasDumplog = false;
                if (dirExists)
                {
                    string txtFile = Path.Combine(dumplogDir,
                        tsi.GetDumplogFileName());
                    string htmlFile = Path.Combine(dumplogDir,
                        tsi.GetHTMLDumplogFileName());
                    if (File.Exists(txtFile))
                    {
                        dumplogFilename = tsi.GetDumplogFileName();
                        hasDumplog = true;
                    }
                    else if (File.Exists(htmlFile))
                    {
                        dumplogFilename = tsi.GetHTMLDumplogFileName();
                        hasDumplog = true;
                    }
                }

                result.Add(new
                {
                    name = tsi.Name ?? "",
                    role = tsi.Role ?? "",
                    race = tsi.Race ?? "",
                    gender = tsi.Gender ?? "",
                    alignment = tsi.Alignment ?? "",
                    xp_level = tsi.XPLevel,
                    hp = tsi.HP,
                    hp_max = tsi.HPMax,
                    mode = tsi.Mode ?? "",
                    turns = tsi.Turns,
                    score = tsi.Score,
                    outcome = tsi.Outcome ?? "",
                    death_date = tsi.DeathDateString,
                    real_time = tsi.RealTimeString,
                    start_time = tsi.StartTime,
                    has_dumplog = hasDumplog,
                    dumplog_filename = dumplogFilename
                });
            }

            string json = JsonConvert.SerializeObject(result);
            int remaining = totalCount - offset - page.Length;
            if (remaining > 0)
                json += "  /* " + remaining
                    + " more older entries available. */";

            return json;
        }

        /// <summary>
        /// Lists and reads dumplog files that actually exist on disk.
        /// List mode (no filename): scans dumplog directory, deduplicates
        /// .txt/.html pairs, matches to xlog entries.
        /// Read mode (filename specified): reads that specific dumplog file.
        /// </summary>
        private string GetPlayerDumplogsResult(JObject parameters)
        {
            string dumplogDir = Path.Combine(GHApp.GHPath,
                GHConstants.DumplogDirectory);
            string filenameParam = parameters?["filename"]?.ToString();

            /* Configurable truncation limit via max_length parameter */
            int maxChars = DefaultMaxDumplogChars;
            string maxLenStr = parameters?["max_length"]?.ToString();
            if (!string.IsNullOrEmpty(maxLenStr)
                && int.TryParse(maxLenStr, out int ml) && ml > 0)
            {
                maxChars = ml;
            }

            if (string.IsNullOrEmpty(filenameParam))
            {
                /* ======== List mode ======== */
                if (!Directory.Exists(dumplogDir))
                    return "[]  /* No dumplog directory found. */";

                /* Gather all dumplog files */
                var allFiles = Directory.GetFiles(dumplogDir)
                    .Where(f =>
                    {
                        string ext = Path.GetExtension(f).ToLowerInvariant();
                        return ext == ".txt" || ext == ".html";
                    })
                    .ToArray();

                if (allFiles.Length == 0)
                    return "[]  /* No dumplog files found on this device. */";

                /* Group by base name (without extension) to deduplicate
                   .txt/.html pairs into a single entry per game */
                var groups = allFiles
                    .GroupBy(f => Path.GetFileNameWithoutExtension(f))
                    .Select(g =>
                    {
                        string txtFile = g.FirstOrDefault(
                            f => Path.GetExtension(f)
                                .Equals(".txt",
                                    StringComparison.OrdinalIgnoreCase));
                        string htmlFile = g.FirstOrDefault(
                            f => Path.GetExtension(f)
                                .Equals(".html",
                                    StringComparison.OrdinalIgnoreCase));
                        string format = (txtFile != null && htmlFile != null)
                            ? "both"
                            : (txtFile != null ? "txt" : "html");
                        /* Prefer txt for the primary filename */
                        string primaryFile = txtFile ?? htmlFile;
                        return new
                        {
                            PrimaryFile = primaryFile,
                            Filename = Path.GetFileName(primaryFile),
                            Format = format,
                            LastWrite = File.GetLastWriteTimeUtc(
                                primaryFile)
                        };
                    })
                    .OrderByDescending(x => x.LastWrite)
                    .Take(50)
                    .ToArray();

                /* Build reverse lookup from xlog entries */
                var xlogLookup =
                    new Dictionary<string, GHTopScoreItem>();
                string xlogPath = Path.Combine(GHApp.GHPath, "xlogfile");
                if (File.Exists(xlogPath))
                {
                    try
                    {
                        string[] lines = File.ReadAllLines(xlogPath);
                        foreach (string line in lines)
                        {
                            if (string.IsNullOrWhiteSpace(line))
                                continue;
                            var tsi = new GHTopScoreItem(line);
                            string txtKey = tsi.GetDumplogFileName();
                            string htmlKey =
                                tsi.GetHTMLDumplogFileName();
                            if (!xlogLookup.ContainsKey(txtKey))
                                xlogLookup[txtKey] = tsi;
                            if (!xlogLookup.ContainsKey(htmlKey))
                                xlogLookup[htmlKey] = tsi;
                        }
                    }
                    catch (Exception)
                    {
                        /* xlog parse errors are non-fatal */
                    }
                }

                var listing = new List<object>();
                foreach (var grp in groups)
                {
                    GHTopScoreItem matched = null;
                    xlogLookup.TryGetValue(grp.Filename, out matched);

                    listing.Add(new
                    {
                        filename = grp.Filename,
                        format = grp.Format,
                        display_name = matched != null
                            ? (matched.Name + " \u2014 "
                                + matched.CharacterString)
                            : (string)null,
                        outcome = matched?.Outcome,
                        score = matched != null
                            ? (int?)matched.Score : null,
                        file_size = new FileInfo(
                            grp.PrimaryFile).Length,
                        is_orphaned = matched == null
                    });
                }

                return JsonConvert.SerializeObject(listing);
            }
            else
            {
                /* ======== Read mode ======== */
                /* Path traversal protection */
                string safeName = Path.GetFileName(filenameParam);
                string fullPath = Path.Combine(dumplogDir, safeName);

                if (!File.Exists(fullPath))
                    throw new FileNotFoundException(
                        "Dumplog file not found: " + safeName);

                string content = File.ReadAllText(fullPath);

                /* Strip HTML tags if reading an HTML dumplog */
                if (fullPath.EndsWith(".html",
                    StringComparison.OrdinalIgnoreCase))
                {
                    content = Regex.Replace(content, "<[^>]*>", "");
                    /* Collapse excessive whitespace from tag removal */
                    content = Regex.Replace(content, @"\n{3,}", "\n\n");
                }

                if (content.Length > maxChars)
                {
                    content = content.Substring(0, maxChars)
                        + "\n\n[DUMPLOG TRUNCATED at " + maxChars
                        + " characters. Full file is "
                        + content.Length
                        + " characters. Use max_length parameter"
                        + " for more.]";
                }

                return "=== Dumplog: " + safeName
                    + " ===\n" + content;
            }
        }
        /// <summary>
        /// Reads the GnollHack application log (ghlog.txt) from the device.
        /// Supports optional filtering by search_term and limiting to last_n lines.
        /// </summary>
        private string GetAppLogResult(JObject parameters)
        {
            string logPath = Path.Combine(GHApp.GHPath,
                GHConstants.AppLogDirectory, GHConstants.AppLogFileName);

            if (!File.Exists(logPath))
                return "No application log file found.";

            string[] allLines;
            try
            {
                allLines = File.ReadAllLines(logPath);
            }
            catch (Exception ex)
            {
                return "Failed to read application log: " + ex.Message;
            }

            /* Optional search filtering */
            string searchTerm = parameters?["search_term"]?.ToString();
            if (!string.IsNullOrEmpty(searchTerm))
            {
                allLines = allLines
                    .Where(l => l.IndexOf(searchTerm,
                        StringComparison.OrdinalIgnoreCase) >= 0)
                    .ToArray();
            }

            /* Optional line limit (from end of log) */
            string lastNStr = parameters?["last_n"]?.ToString();
            if (!string.IsNullOrEmpty(lastNStr)
                && int.TryParse(lastNStr, out int n) && n > 0
                && allLines.Length > n)
            {
                allLines = allLines.Skip(allLines.Length - n).ToArray();
            }

            if (allLines.Length == 0)
            {
                return string.IsNullOrEmpty(searchTerm)
                    ? "Application log is empty."
                    : "No log lines match the search term '" + searchTerm
                        + "'.";
            }

            return string.Join("\n", allLines);
        }

        /// <summary>
        /// Reads the GnollHack panic log from the device.
        /// Contains C core engine panic/crash information.
        /// </summary>
        private string GetPanicLogResult(JObject parameters)
        {
            string panicPath = Path.Combine(GHApp.GHPath, "paniclog");

            if (!File.Exists(panicPath))
                return "No panic log found. No engine panics have occurred.";

            try
            {
                string content = File.ReadAllText(panicPath);
                if (string.IsNullOrWhiteSpace(content))
                    return "Panic log exists but is empty.";
                return content;
            }
            catch (Exception ex)
            {
                return "Failed to read panic log: " + ex.Message;
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
        private void HandlePickFilesRequest(
            double srcX = 0, double srcY = 0,
            double srcW = 0, double srcH = 0)
        {
#if GNH_MAUI
#if IOS || MACCATALYST
            MainThread.BeginInvokeOnMainThread(() =>
            {
                UIKit.UIWindow window = null;
                if (UIKit.UIApplication.SharedApplication?.ConnectedScenes != null)
                {
                    foreach (var scene in UIKit.UIApplication.SharedApplication.ConnectedScenes)
                    {
                        if (scene is UIKit.UIWindowScene windowScene)
                        {
                            foreach (var w in windowScene.Windows)
                            {
                                if (w.IsKeyWindow)
                                {
                                    window = w;
                                    break;
                                }
                            }
                        }
                        if (window != null) break;
                    }
                }
                var rootVc = window?.RootViewController;
                if (rootVc == null) return;
                var topVc = GetTopViewController(rootVc);

                var alert = UIKit.UIAlertController.Create(
                    null, null, UIKit.UIAlertControllerStyle.ActionSheet);

                alert.AddAction(UIKit.UIAlertAction.Create(
                    "Photo Library", UIKit.UIAlertActionStyle.Default,
                    _ => PresentPhotoPicker(topVc)));

                alert.AddAction(UIKit.UIAlertAction.Create(
                    "Browse", UIKit.UIAlertActionStyle.Default,
                    _ => PresentDocumentPicker(topVc)));

                alert.AddAction(UIKit.UIAlertAction.Create(
                    "Cancel", UIKit.UIAlertActionStyle.Cancel, null));

                /* iPad popover anchor — required or UIAlertController crashes on iPad */
                if (alert.PopoverPresentationController != null)
                {
                    var wkWebView = DisplayWebView.Handler?.PlatformView
                        as WebKit.WKWebView;
                    if (wkWebView != null && (srcW > 0 || srcH > 0))
                    {
                        /* Use the WKWebView as SourceView so the rect coordinates
                         * (CSS pixels from getBoundingClientRect) map directly
                         * to the WebView's point coordinate system. */
                        alert.PopoverPresentationController.SourceView = wkWebView;
                        alert.PopoverPresentationController.SourceRect =
                            new CoreGraphics.CGRect(srcX, srcY, srcW, srcH);
                    }
                    else
                    {
                        /* Fallback for old Angular clients that don't send sourceRect */
                        alert.PopoverPresentationController.SourceView = topVc.View;
                        alert.PopoverPresentationController.SourceRect =
                            new CoreGraphics.CGRect(
                                topVc.View.Bounds.Width / 2,
                                topVc.View.Bounds.Height, 0, 0);
                    }
                }

                topVc.PresentViewController(alert, true, null);
            });
#endif
#endif
        }

#if GNH_MAUI && (IOS || MACCATALYST)
        private void PresentPhotoPicker(UIKit.UIViewController presenter)
        {
            var config = new PhotosUI.PHPickerConfiguration();
            config.SelectionLimit = 5;
            config.Filter = PhotosUI.PHPickerFilter.ImagesFilter;

            var picker = new PhotosUI.PHPickerViewController(config);
            var del = new PhotoPickerDelegate(this);
            picker.Delegate = del;
            /* Store strong reference to prevent GC */
            _iosPickerDelegate = del;
            presenter.PresentViewController(picker, true, null);
        }

        private void PresentDocumentPicker(UIKit.UIViewController presenter)
        {
            var types = new UniformTypeIdentifiers.UTType[]
            {
                UniformTypeIdentifiers.UTTypes.PlainText,
                UniformTypeIdentifiers.UTTypes.Html,
                UniformTypeIdentifiers.UTTypes.Png,
                UniformTypeIdentifiers.UTTypes.Jpeg,
                UniformTypeIdentifiers.UTTypes.WebP
            };

            var docPicker = new UIKit.UIDocumentPickerViewController(types, false);
            docPicker.AllowsMultipleSelection = true;
            var del = new DocumentPickerDelegate(this);
            docPicker.Delegate = del;
            _iosPickerDelegate = del;
            presenter.PresentViewController(docPicker, true, null);
        }

        private class PhotoPickerDelegate : PhotosUI.PHPickerViewControllerDelegate
        {
#if IOS
            [System.Runtime.InteropServices.DllImport("libc", EntryPoint = "umask")]
            private static extern ushort umask(ushort mask);
#endif

            private readonly WeakReference<OverseerPage> _pageRef;

            public PhotoPickerDelegate(OverseerPage page)
            {
                _pageRef = new WeakReference<OverseerPage>(page);
            }

            public override void DidFinishPicking(
                PhotosUI.PHPickerViewController picker,
                PhotosUI.PHPickerResult[] results)
            {
                picker.DismissViewController(true, null);
                if (results == null || results.Length == 0) return;
                if (!_pageRef.TryGetTarget(out var page)) return;

                /* Load each image and send back to JS */
                var remaining = results.Length;
                var fileEntries = new List<object>();

                /* The game engine's gnh_umask() sets the process-wide umask to 0113, 
                 * which strips the execute bit from newly created directories. This breaks
                 * NSItemProvider loading methods ("Cannot load representation...") because 
                 * it prevents traversal into its own temporary staging directories. 
                 * Temporarily restore the default iOS umask (0022 octal = 0x12 hex = 18 dec).
                 * The game thread is blocked (paused at the menu) while Overseer is open, 
                 * so it's safe to change this process-wide setting temporarily. */
                ushort savedUmask = 0;
#if IOS
                try
                {
                    savedUmask = umask(0x12);
                }
                catch (Exception ex) 
                {
                    GHApp.MaybeWriteGHLog(ex.Message);
                }
#endif

                try
                {
                    for (int i = 0; i < results.Length; i++)
                    {
                        var provider = results[i].ItemProvider;
                        if (provider.CanLoadObject(typeof(UIKit.UIImage)))
                        {
                            provider.LoadObject<UIKit.UIImage>((obj, error) =>
                            {
                                if (obj is UIKit.UIImage uiImage && error == null)
                                {
                                    var pngData = uiImage.AsPNG();
                                    if (pngData != null)
                                    {
                                        string base64 = pngData.GetBase64EncodedString(
                                            Foundation.NSDataBase64EncodingOptions.None);
                                        lock (fileEntries)
                                        {
                                            fileEntries.Add(new
                                            {
                                                name = $"photo_{fileEntries.Count + 1}.png",
                                                type = "image/png",
                                                dataUrl = $"data:image/png;base64,{base64}"
                                            });
                                        }
                                    }
                                }
                                if (Interlocked.Decrement(ref remaining) == 0)
                                {
                                    /* Restore the game engine's umask */
#if IOS
                                    try
                                    {
                                        umask(savedUmask);
                                    }
                                    catch (Exception ex)
                                    {
                                        GHApp.MaybeWriteGHLog(ex.Message);
                                    }
#endif
                                    page.SendPickedFilesToJs(fileEntries);
                                }
                            });
                        }
                        else
                        {
                            if (Interlocked.Decrement(ref remaining) == 0)
                            {
#if IOS
                                try
                                {
                                    umask(savedUmask);
                                }
                                catch (Exception ex)
                                {
                                    GHApp.MaybeWriteGHLog(ex.Message);
                                }
#endif
                                page.SendPickedFilesToJs(fileEntries);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog(ex.Message);
#if IOS
                    try
                    {
                        umask(savedUmask);
                    }
                    catch (Exception ex2)
                    {
                        GHApp.MaybeWriteGHLog(ex2.Message);
                    }
#endif
                }
            }
        }

        private class DocumentPickerDelegate : UIKit.UIDocumentPickerDelegate
        {
            private readonly WeakReference<OverseerPage> _pageRef;

            public DocumentPickerDelegate(OverseerPage page)
            {
                _pageRef = new WeakReference<OverseerPage>(page);
            }

            public override void DidPickDocument(UIKit.UIDocumentPickerViewController controller, Foundation.NSUrl[] urls)
            {
                if (urls == null || urls.Length == 0) return;
                if (!_pageRef.TryGetTarget(out var page)) return;

                var fileEntries = new List<object>();

                foreach (var url in urls)
                {
                    var isAccessing = url.StartAccessingSecurityScopedResource();
                    try
                    {
                        var data = Foundation.NSData.FromUrl(url);
                        if (data != null)
                        {
                            string ext = url.PathExtension?.ToLowerInvariant();
                            string mime = "application/octet-stream";
                            if (ext == "html" || ext == "htm") mime = "text/html";
                            else if (ext == "txt") mime = "text/plain";
                            else if (ext == "md") mime = "text/markdown";
                            else if (ext == "png") mime = "image/png";
                            else if (ext == "jpg" || ext == "jpeg") mime = "image/jpeg";
                            else if (ext == "webp") mime = "image/webp";

                            string base64 = data.GetBase64EncodedString(Foundation.NSDataBase64EncodingOptions.None);
                            fileEntries.Add(new
                            {
                                name = url.LastPathComponent,
                                type = mime,
                                dataUrl = $"data:{mime};base64,{base64}"
                            });
                        }
                    }
                    finally
                    {
                        if (isAccessing)
                        {
                            url.StopAccessingSecurityScopedResource();
                        }
                    }
                }
                
                page.SendPickedFilesToJs(fileEntries);
            }
        }

        private static UIKit.UIViewController GetTopViewController(UIKit.UIViewController rootVc)
        {
            if (rootVc.PresentedViewController != null)
                return GetTopViewController(rootVc.PresentedViewController);
            if (rootVc is UIKit.UINavigationController navController
                && navController.VisibleViewController != null)
                return GetTopViewController(navController.VisibleViewController);
            if (rootVc is UIKit.UITabBarController tabController
                && tabController.SelectedViewController != null)
                return GetTopViewController(tabController.SelectedViewController);
            return rootVc;
        }
#endif

        private async void SendPickedFilesToJs(List<object> fileEntries)
        {
            try
            {
                string json = JsonConvert.SerializeObject(fileEntries);
                string jsLiteral = JsonConvert.SerializeObject(json);
                await MainThread.InvokeOnMainThreadAsync(async () =>
                {
                    await DisplayWebView.EvaluateJavaScriptAsync(
                        "window.__gnollhackReceiveFiles(" + jsLiteral + ")");
                });
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("SendPickedFilesToJs failed: " + ex.Message);
            }
        }

        /* IMessagePopupPage implementation */
        public bool IsPopupOpen => MessagePopup.IsPopupOpen;
        public void ClosePopup() => MessagePopup.ClosePopup();
        public bool SendKeyToPopup(int key, bool isCtrl, bool isMeta) => MessagePopup.SendKeyToPopup(key, isCtrl, isMeta);
        public bool SendSpecialKeyToPopup(GHSpecialKey spkey, bool isCtrl, bool isMeta, bool isShift) => MessagePopup.SendSpecialKeyToPopup(spkey, isCtrl, isMeta, isShift);
#if GNH_MAUI
        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
             Microsoft.Maui.Graphics.Color titleColor = null, bool acceptEnterSpaceForOkCancel = false) => MessagePopup.ShowMessagePopupAsync(title, message, okButtonText, cancelButtonText, titleColor, acceptEnterSpaceForOkCancel);
#else
        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
             Xamarin.Forms.Color? titleColor = null, bool acceptEnterSpaceForOkCancel = false) => MessagePopup.ShowMessagePopupAsync(title, message, okButtonText, cancelButtonText, titleColor, acceptEnterSpaceForOkCancel);
#endif
    }
}

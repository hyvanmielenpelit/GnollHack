using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
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

        public OverseerPage(string title, string baseOverseerUrl,
                            string snapshotHtml, string messageHistory,
                            string directoryManifest, string debugData)
        {
            InitializeComponent();

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

                        /* Default initial prompt */
                        content.Add(new StringContent(
                            "Analyze my current game state and suggest what I should do next."),
                            "InitialPrompt");

                        ProgressStatusLabel.Text = "Contacting Overseer server...";
                        UploadProgressBar.Progress = 0.6;

                        var response = await httpClient.PostAsync(
                            _baseOverseerUrl + "/api/session/create", content);

                        if (response.IsSuccessStatusCode)
                        {
                            string jsonResponse = await response.Content.ReadAsStringAsync();
                            var result = JObject.Parse(jsonResponse);
                            string sessionId = result?["sessionId"]?.ToString() ?? "";
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

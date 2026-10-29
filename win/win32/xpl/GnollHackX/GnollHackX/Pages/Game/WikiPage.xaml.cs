using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SkiaSharp;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;
using SkiaSharp.Views.Maui;

namespace GnollHackM
#else
using SkiaSharp.Views.Forms;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using GnollHackX.Controls;
using GnollHackX.Pages.MainScreen;
using static System.Net.Mime.MediaTypeNames;

namespace GnollHackX.Pages.Game
#endif
{
    public partial class WikiPage : CustomModalPage, ICloseablePage, IGHGamepadHandlingPage
    {
#if GNH_MAUI
        IDispatcherTimer _timer = null;
#endif
        private const float GamepadHintFontSizeDips = 13f;     /* the hint strip's label size */
        private const int GamepadHintMinimumCount = 3;         /* hints kept when dropping ones that do not fit */

        private readonly string _startUrl;
        private readonly GHWebGamepadNavigator _webNavigator;
#if GNH_AVALONIA
        private GHAvaloniaWebScriptHost _webScriptHost;
#endif
        private GHGamepadHintRenderer _gamepadHintRenderer;
        private readonly List<GHGamepadHint> _gamepadPaintHints = new List<GHGamepadHint>();
        /* Between Appearing and Disappearing; the hint strip draws nothing otherwise */
        private bool _gamepadHintsShown = false;

        public WikiPage(string title, string wikiUrl)
        {
            InitializeComponent();
            _startUrl = wikiUrl;
//#if GNH_MAUI
//            SafeAreaEdges = SafeAreaEdges.All;
//#else
//            On<iOS>().SetUseSafeArea(true);
//#endif
            //UIUtils.AdjustRootLayout(RootGrid);
            //UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            //UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            UrlWebViewSource Source = new UrlWebViewSource
            {
                Url = wikiUrl,
            };
            DisplayWebView.Source = Source;
            UpdateNavigationButtons(true);
            InitializeGamepadNavigation();
            _webNavigator = new GHWebGamepadNavigator(_gamepadNavigator, DisplayWebView);
            _webNavigator.CanGoBack = () => DisplayWebView.CanGoBack;
            _webNavigator.GoBack = GoBackInHistory;
            _webNavigator.ClosePage = ClosePage;
            _webNavigator.GoHome = GoToStartPage;
#if GNH_AVALONIA
            _webScriptHost = new GHAvaloniaWebScriptHost(DisplayWebView);
            _webNavigator.Host = _webScriptHost;
#endif
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
                    /* Dispatcher timer ticks on the UI thread */
                    UpdateNavigationButtons();
                    _webNavigator.RefreshHints();
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

        private void DisplayWebView_Navigating(object sender, WebNavigatingEventArgs e)
        {
            LoadingIndicator.IsRunning = true;
            LoadingIndicator.IsVisible = true;
            UpdateNavigationButtons();
            _webNavigator?.OnNavigating();
        }

        private void DisplayWebView_Navigated(object sender, WebNavigatedEventArgs e)
        {
            LoadingIndicator.IsRunning = false;
            LoadingIndicator.IsVisible = false;
            UpdateNavigationButtons();
            _webNavigator?.OnNavigated();
#if GNH_MAUI
            if(_timer == null)
            {
                var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                timer.Interval = TimeSpan.FromSeconds(0.5);
                timer.IsRepeating = false;
                timer.Tick += (s, e) =>
                {
                    /* Dispatcher timer ticks on the UI thread */
                    UpdateNavigationButtons();
                    _webNavigator?.RefreshHints();
                };
                timer.Start();
            }
#else
            Device.StartTimer(TimeSpan.FromSeconds(0.5), () =>
            {
                /* Device.StartTimer fires on the main thread */
                UpdateNavigationButtons();
                _webNavigator?.RefreshHints();
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
                GHApp.PlayButtonClickedSound();
                GoBackInHistory();
            }
        }

        /* Without the click sound */
        private void GoBackInHistory()
        {
            if (DisplayWebView.CanGoBack)
            {
                BackButton.IsEnabled = false;
                ForwardButton.IsEnabled = false;
                DisplayWebView.GoBack();
                UpdateNavigationButtons();
            }
        }

        private void GoToStartPage()
        {
            DisplayWebView.Source = new UrlWebViewSource
            {
                Url = _startUrl,
            };
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

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
#if GNH_AVALONIA
            if (_webScriptHost == null)
            {
                _webScriptHost = new GHAvaloniaWebScriptHost(DisplayWebView);
                _webNavigator.Host = _webScriptHost;
            }
#endif
            _gamepadHintsShown = true;
            _webNavigator.HintsChanged -= WebNavigator_HintsChanged;
            _webNavigator.HintsChanged += WebNavigator_HintsChanged;
            _webNavigator.Attach();
            WebNavigator_HintsChanged();
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            _webNavigator.HintsChanged -= WebNavigator_HintsChanged;
            _webNavigator.Detach();
#if GNH_AVALONIA
            _webNavigator.Host = null;
            if (_webScriptHost != null)
            {
                _webScriptHost.Dispose();
                _webScriptHost = null;
            }
#endif
            _gamepadHintsShown = false;
            GamepadHintCanvas.IsVisible = false;
            if (_gamepadHintRenderer != null)
            {
                _gamepadHintRenderer.Dispose();
                _gamepadHintRenderer = null;
            }
        }

        /* Gamepad: _webNavigator moves a focus ring over the page's links and controls, with Close, back and forward
           in the button row as its toolbar mode; without a script host the button row is all it navigates. */
        private readonly GHGamepadPageNavigator _gamepadNavigator = new GHGamepadPageNavigator();

        private void InitializeGamepadNavigation()
        {
            GHGamepadPageNavigator.Layer main = _gamepadNavigator.Main;
            main.AddButton(CloseButton);
            main.AddButton(BackButton);
            main.AddButton(ForwardButton);
            main.HorizontalMoves = true;
            main.SetInitialFocus(CloseButton);
            main.Back = null;
        }

        public bool HandleGamepadButton(GHGamepadButtonEvent e)
        {
            return _webNavigator.HandleButton(e);
        }

        public void UpdateGamepadSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            _webNavigator.UpdateSticks(sticks, dtSeconds);
        }

        private void WebNavigator_HintsChanged()
        {
            IList<GHGamepadHint> hints = _webNavigator.CurrentHints;
            bool show = _gamepadHintsShown && GHGamepadService.IsActive && hints != null && hints.Count > 0;
            if (GamepadHintCanvas.IsVisible != show)
                GamepadHintCanvas.IsVisible = show;
            GamepadHintCanvas.InvalidateSurface();
        }

        private GHGamepadHintRenderer EnsureGamepadHintRenderer()
        {
            if (!_gamepadHintsShown)
                return null;
            if (_gamepadHintRenderer == null)
            {
                GHGamepadHintRenderer renderer = new GHGamepadHintRenderer();
                renderer.LabelTypeface = GHApp.LatoRegular;
                renderer.BoldTypeface = GHApp.LatoBold;
                renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
                _gamepadHintRenderer = renderer;
            }
            return _gamepadHintRenderer;
        }

        /* The gamepad hint strip, painted on the UI thread (UseGL is off): one line centered in the canvas, dropping
           the last hints, down to GamepadHintMinimumCount, while the row needs two lines */
        private void GamepadHintCanvas_PaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            SKCanvas canvas = e.Surface.Canvas;
            canvas.Clear(SKColors.Transparent);
            IList<GHGamepadHint> hints = _webNavigator != null ? _webNavigator.CurrentHints : null;
            if (hints == null || hints.Count == 0 || !_gamepadHintsShown)
                return;
            GHGamepadHintRenderer renderer = EnsureGamepadHintRenderer();
            if (renderer == null)
                return;

            int pixelWidth = e.Info.Width;
            int pixelHeight = e.Info.Height;
            double dipWidth = GamepadHintCanvas.Width;
            float scale;
            if (dipWidth > 0 && pixelWidth > 0)
                scale = (float)(pixelWidth / dipWidth);
            else
                scale = GHApp.DisplayDensity > 0 ? GHApp.DisplayDensity : 1.0f;

            renderer.UseTextBlobCaching = GHApp.UseTextBlobCaching;
            float fontSize = GamepadHintFontSizeDips * scale;
            float maxWidth = pixelWidth - 2f * fontSize * GHGamepadHintRenderer.BackingPadOfFont;
            GHGamepadStyle style = GHGamepadService.Style;
            List<GHGamepadHint> paintHints = _gamepadPaintHints;
            paintHints.Clear();
            paintHints.AddRange(hints);
            renderer.LayoutRow(paintHints, style, fontSize, maxWidth);
            while (renderer.LineCount > 1 && paintHints.Count > GamepadHintMinimumCount)
            {
                paintHints.RemoveAt(paintHints.Count - 1);
                renderer.LayoutRow(paintHints, style, fontSize, maxWidth);
            }
            if (renderer.LineCount == 0)
                return;

            float bottomY = pixelHeight * 0.5f + renderer.RowHeight * 0.5f;
            renderer.DrawRow(canvas, paintHints, style, pixelWidth * 0.5f, bottomY, 1.0f, true);
        }
        //protected override bool OnBackButtonPressed()
        //{
        //    return true;
        //}
    }
}
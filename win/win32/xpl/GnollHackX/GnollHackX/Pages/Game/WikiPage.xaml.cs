using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
        public WikiPage(string title, string wikiUrl)
        {
            InitializeComponent();
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
        }

        private void DisplayWebView_Navigated(object sender, WebNavigatedEventArgs e)
        {
            LoadingIndicator.IsRunning = false;
            LoadingIndicator.IsVisible = false;
            UpdateNavigationButtons();
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
                };
                timer.Start();
            }
#else
            Device.StartTimer(TimeSpan.FromSeconds(0.5), () =>
            {
                /* Device.StartTimer fires on the main thread */
                UpdateNavigationButtons();
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

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            _gamepadNavigator.Attach();
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            _gamepadNavigator.Detach();
        }

        /* Gamepad focus: Close, back and forward in the button row; B closes the page. The web page itself
           takes touch, mouse and keyboard only. */
        private readonly GHGamepadPageNavigator _gamepadNavigator = new GHGamepadPageNavigator();

        private void InitializeGamepadNavigation()
        {
            GHGamepadPageNavigator.Layer main = _gamepadNavigator.Main;
            main.AddButton(CloseButton);
            main.AddButton(BackButton);
            main.AddButton(ForwardButton);
            main.HorizontalMoves = true;
            main.SetInitialFocus(CloseButton);
            main.Back = () =>
            {
                ClosePage();
                return true;
            };
        }

        public bool HandleGamepadButton(GHGamepadButtonEvent e)
        {
            return _gamepadNavigator.HandleButton(e);
        }

        public void UpdateGamepadSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            _gamepadNavigator.UpdateSticks(sticks, dtSeconds);
        }
        //protected override bool OnBackButtonPressed()
        //{
        //    return true;
        //}
    }
}
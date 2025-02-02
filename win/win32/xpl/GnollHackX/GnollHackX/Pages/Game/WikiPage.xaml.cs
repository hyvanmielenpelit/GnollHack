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
using GnollHackX.Pages.MainScreen;
using static System.Net.Mime.MediaTypeNames;

namespace GnollHackX.Pages.Game
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class WikiPage : ContentPage
    {
#if GNH_MAUI
        IDispatcherTimer _timer = null;
#endif
        public WikiPage(string title, string wikiUrl)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            UrlWebViewSource Source = new UrlWebViewSource
            {
                Url = wikiUrl,
            };
            DisplayWebView.Source = Source;
            UpdateNavigationButtons();
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

        private void UpdateNavigationButtons()
        {
            BackButton.IsEnabled = DisplayWebView.CanGoBack;
            BackButton.TextColor = BackButton.IsEnabled ? GHColors.White : GHColors.Gray;

            ForwardButton.IsEnabled = DisplayWebView.CanGoForward;
            ForwardButton.TextColor = ForwardButton.IsEnabled ? GHColors.White : GHColors.Gray;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var page = await GHApp.Navigation.PopModalAsync();
            GHApp.DisconnectIViewHandlers(page);
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
    }
}
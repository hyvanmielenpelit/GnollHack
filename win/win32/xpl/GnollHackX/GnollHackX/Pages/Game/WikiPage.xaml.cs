using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;


#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
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
        public WikiPage(string title, string wikiUrl)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UrlWebViewSource Source = new UrlWebViewSource
            {
                Url = wikiUrl,
            };
            DisplayWebView.Source = Source;
            UpdateNavigationButtons();
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
            BackButton.TextColor = BackButton.IsEnabled ? Color.White : Color.Gray;

            ForwardButton.IsEnabled = DisplayWebView.CanGoForward;
            ForwardButton.TextColor = ForwardButton.IsEnabled ? Color.White : Color.Gray;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
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
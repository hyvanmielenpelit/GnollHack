using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CreditsPage : ContentPage
    {
        public CreditsPage()
        {
            InitializeComponent();
        }

        private async void btnCreditsX_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            CreditsTableView.IsEnabled = false;
            string fulltargetpath = Path.Combine(App.GHPath, "xcredits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Cross-Platform Credits");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                CreditsTableView.IsEnabled = true;
                await DisplayAlert("Error Opening File", "GnollHack cannot open the xcredits file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }

        private async void btnCreditsW_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            CreditsTableView.IsEnabled = false;
            string fulltargetpath = Path.Combine(App.GHPath, "credits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Windows Credits");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                CreditsTableView.IsEnabled = true;
                await DisplayAlert("Error Opening File", "GnollHack cannot open the credits file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }

        private async void btnLicense_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            CreditsTableView.IsEnabled = false;
            string fulltargetpath = Path.Combine(App.GHPath, "license");
            var displFilePage = new DisplayFilePage(fulltargetpath, "License");
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                CreditsTableView.IsEnabled = true;
                await DisplayAlert("Error Opening File", "GnollHack cannot open the license file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
        }
        private async void btnGitHub_Clicked(object sender, EventArgs e)
        {
            btnGitHub.IsEnabled = false;
            await OpenBrowser(new Uri(GHConstants.GnollHackGitHubPage));
            btnGitHub.IsEnabled = true;
        }

        public async Task OpenBrowser(Uri uri)
        {
            try
            {
                await Browser.OpenAsync(uri, BrowserLaunchMode.SystemPreferred);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Cannot Open Web Page", "GnollHack cannot open the webpage at " + uri.OriginalString + ". Error: " + ex.Message, "OK");
            }
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
            CreditsTableView.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

    }
}
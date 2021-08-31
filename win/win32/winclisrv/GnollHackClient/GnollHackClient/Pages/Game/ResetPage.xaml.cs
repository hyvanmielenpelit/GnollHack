using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class ResetPage : ContentPage
	{
		public ResetPage ()
		{
			InitializeComponent ();
		}

        private async void btnDeleteFiles_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete GnollHack Files?", "Are you sure to delete all GnollHack files?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearFiles();
                App.GnollHackService.InitializeGnollHack();
                btnDeleteFiles.Text = "Done";
                btnDeleteFiles.TextColor = Color.Red;
            }
        }

        private async void btnDeleteDownloads_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete Downloaded Files?", "Are you sure to delete all downloaded files on restart?", "Yes", "No");
            if (answer)
            {
                Preferences.Set("ResetBanks", true);
                btnDeleteDownloads.Text = "Deletion on restart";
                btnDeleteDownloads.TextColor = Color.Red;
            }
        }

        private async void btnDeletePreferences_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete App Preferences?", "Are you sure to delete all app preference settings?", "Yes", "No");
            if (answer)
            {
                Preferences.Clear();
                Preferences.Set("FullVersion", App.FullVersionMode);
                btnDeletePreferences.Text = "Done";
                btnDeletePreferences.TextColor = Color.Red;
            }
        }

        private async void btnRevertToDemo_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Revert to Demo Version?", "Are you sure to revert to demo version?", "Yes", "No");
            if (answer)
            {
                Preferences.Set("FullVersion", false);
                App.FullVersionMode = false;
                btnRevertToDemo.Text = "Done";
                btnRevertToDemo.TextColor = Color.Red;
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
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

    }
}
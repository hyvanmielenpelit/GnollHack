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
            bool answer = await DisplayAlert("Delete GnollHack Core Files?", "Are you sure to delete GnollHack core files?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearCoreFiles();
                App.GnollHackService.InitializeGnollHack();
                btnDeleteFiles.Text = "Done";
                btnDeleteFiles.TextColor = Color.Red;
            }
        }

        private async void btnDeleteSavedGames_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete Saved Games?", "Are you sure to delete all saved games?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearSavedGames();
                btnDeleteSavedGames.Text = "Done";
                btnDeleteSavedGames.TextColor = Color.Red;
            }
        }

        private async void btnDeleteDumplogs_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete Dumplogs?", "Are you sure to delete all dumplogs?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearDumplogs();
                btnDeleteDumplogs.Text = "Done";
                btnDeleteDumplogs.TextColor = Color.Red;
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

                bool has_resetbanks = Preferences.ContainsKey("ResetBanks");
                bool resetbanks = Preferences.Get("ResetBanks", false);
                bool has_fcf = Preferences.ContainsKey("CheckPurchase_FirstConnectFail");
                DateTime fcf = Preferences.Get("CheckPurchase_FirstConnectFail", DateTime.MinValue);
                bool has_gsc = Preferences.ContainsKey("CheckPurchase_ConnectFail_GameStartCount");
                int gsc = Preferences.Get("CheckPurchase_ConnectFail_GameStartCount", 0);
                bool has_vbv = Preferences.ContainsKey("VerifyBank_Version");
                string vbv = Preferences.Get("VerifyBank_Version", "");
                bool has_vblwt = Preferences.ContainsKey("VerifyBank_LastWriteTime");
                long vblwt = Preferences.Get("VerifyBank_LastWriteTime", 0);

                Preferences.Clear();

                Preferences.Set("FullVersion", App.FullVersionMode);
                if(has_resetbanks)
                    Preferences.Set("ResetBanks", resetbanks);
                if(has_fcf)
                    Preferences.Set("CheckPurchase_FirstConnectFail", fcf);
                if (has_gsc)
                    Preferences.Set("CheckPurchase_ConnectFail_GameStartCount", fcf);
                if (has_vbv)
                    Preferences.Set("VerifyBank_Version", vbv);
                if (has_vblwt)
                    Preferences.Set("VerifyBank_LastWriteTime", vblwt);
                
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
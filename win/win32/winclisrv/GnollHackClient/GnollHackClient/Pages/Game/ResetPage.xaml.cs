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
                App.GnollHackService.InitializeGnollHack(null);
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
            bool answer = await DisplayAlert("Delete Top Scores?", "Are you sure to delete all top scores and associated dumplogs?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearTopScores();
                App.GnollHackService.ClearDumplogs();
                btnDeleteDumplogs.Text = "Done";
                btnDeleteDumplogs.TextColor = Color.Red;
            }
        }

        private async void btnDeleteAllMainFiles_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete All Main Files?", "Are you sure to delete all files in the main directory?", "Yes", "No");
            if (answer)
            {
                App.GnollHackService.ClearAllFilesInMainDirectory();
                btnDeleteAllMainFiles.Text = "Done";
                btnDeleteAllMainFiles.TextColor = Color.Red;
            }
        }

        private async void btnDeleteDownloads_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Delete Downloaded Files?", "Are you sure to delete all downloaded files on restart?", "Yes", "No");
            if (answer)
            {
                Preferences.Set("ResetExternalFiles", true);
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
                bool has_resetatstart = Preferences.ContainsKey("ResetAtStart");
                bool resetatstart = Preferences.Get("ResetAtStart", true);
                bool has_resetbanks = Preferences.ContainsKey("ResetExternalFiles");
                bool resetbanks = Preferences.Get("ResetExternalFiles", false);
                bool has_sse = Preferences.ContainsKey("ShowSpecialEffect");
                bool sse = Preferences.Get("ShowSpecialEffect", true);
                bool has_sbv = Preferences.ContainsKey("SponsorButtonVisited");
                bool sbv = Preferences.Get("SponsorButtonVisited", true);
                bool has_fcf = Preferences.ContainsKey("CheckPurchase_FirstConnectFail");
                DateTime fcf = Preferences.Get("CheckPurchase_FirstConnectFail", DateTime.MinValue);
                bool has_gsc = Preferences.ContainsKey("CheckPurchase_ConnectFail_GameStartCount");
                int gsc = Preferences.Get("CheckPurchase_ConnectFail_GameStartCount", 0);
                bool has_verid = Preferences.ContainsKey("VersionId");
                string verid = Preferences.Get("VersionId", "");

                Dictionary<string, string> saveverdict = new Dictionary<string, string>();
                Dictionary<string, DateTime> savetimedict = new Dictionary<string, DateTime>();
                foreach(SecretsFile sf in App.CurrentSecrets.files)
                {
                    bool has_vbv = Preferences.ContainsKey("Verify_" + sf.id + "_Version");
                    string vbv = Preferences.Get("Verify_" + sf.id + "_Version", "");
                    if(has_vbv)
                        saveverdict.Add("Verify_" + sf.id + "_Version", vbv);
                    bool has_vblwt = Preferences.ContainsKey("Verify_" + sf.id + "_LastWriteTime");
                    DateTime vblwt = Preferences.Get("Verify_" + sf.id + "_LastWriteTime", DateTime.MinValue);
                    if (has_vblwt)
                        savetimedict.Add("Verify_" + sf.id + "_LastWriteTime", vblwt);
                }

                Preferences.Clear();

                Preferences.Set("FullVersion", App.FullVersionMode);
                if (has_resetatstart)
                    Preferences.Set("ResetAtStart", resetatstart);
                if (has_resetbanks)
                    Preferences.Set("ResetExternalFiles", resetbanks);
                if (has_sse)
                    Preferences.Set("ShowSpecialEffect", sse);
                if (has_sbv)
                    Preferences.Set("SponsorButtonVisited", sbv);
                if (has_fcf)
                    Preferences.Set("CheckPurchase_FirstConnectFail", fcf);
                if (has_gsc)
                    Preferences.Set("CheckPurchase_ConnectFail_GameStartCount", fcf);

                KeyValuePair<string, string>[] arr = saveverdict.ToArray<KeyValuePair<string, string>>();
                foreach (KeyValuePair<string, string> kvp in arr)
                {
                    Preferences.Set(kvp.Key, kvp.Value);

                }
                KeyValuePair<string, DateTime>[] arr2 = savetimedict.ToArray<KeyValuePair<string, DateTime>>();
                foreach (KeyValuePair<string, DateTime> kvp2 in arr2)
                {
                    Preferences.Set(kvp2.Key, kvp2.Value);

                }
                if (has_verid)
                    Preferences.Set("VersionId", verid);

                btnDeletePreferences.Text = "Done";
                btnDeletePreferences.TextColor = Color.Red;
            }
        }

        private async void btnRevertToDemo_Clicked(object sender, EventArgs e)
        {
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Activate Effect?", "Are you sure to activate sponsor button special effect?", "Yes", "No");
            if (answer)
            {
                App.ShowSpecialEffect = true;
                App.SponsorButtonVisited = false;
                Preferences.Set("ShowSpecialEffect", App.ShowSpecialEffect);
                Preferences.Set("SponsorButtonVisited", App.SponsorButtonVisited);
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
            GameTableView.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private async void btnViewPanicLog_Clicked(object sender, EventArgs e)
        {
            GameTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "paniclog");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Panic Log");
            string errormsg = "";
            if (!File.Exists(fulltargetpath))
            {
                await DisplayAlert("No Panic Log", "Panic Log does not exist.", "OK");
            }
            else if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the paniclog file.", "OK");
            }
            else
            {
                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
            }
            GameTableView.IsEnabled = true;
        }

        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;

                lblHeader.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseGrid.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

    }
}
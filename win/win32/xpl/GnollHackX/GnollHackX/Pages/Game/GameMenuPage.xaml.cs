using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;

using System.IO;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using GnollHackX.Pages.MainScreen;

namespace GnollHackX.Pages.Game
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GameMenuPage : ContentPage
    {
        public GamePage _gamePage;
 
        public GameMenuPage(GamePage gamePage)
        {
            InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(false);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif

            _gamePage = gamePage;

            if (gamePage.EnableCasualMode)
            {
                btnSave.Text = "Save Game";
                btnQuit.Text = "Quit Game";
            }

            btnOptions.IsVisible = btnMessages.IsVisible = GHApp.DeveloperMode;
            btnGC.IsVisible = btnDebug.IsVisible = btnRunTests.IsVisible =
                GHApp.DeveloperMode && GHApp.DebugLogMessages;
        }

        private async void btnSave_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('s'));
        }

        private async void btnQuit_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('q'));
        }

        private async void btnBackToGame_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void btnOptions_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.DebugWriteRestart("ProfilingStopwatch.Restart: Options");
            _gamePage.GenericButton_Clicked(sender, e, 'O');
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void btnSettings_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var settingsPage = new SettingsPage(this, null);
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
            MainLayout.IsEnabled = true;
        }

        private async void btnLibrary_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var libPage = new LibraryPage();
            libPage.ReadLibrary();
            await App.Current.MainPage.Navigation.PushModalAsync(libPage);
            MainLayout.IsEnabled = true;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }

        public void UpdateLayout()
        {
            MainLayout.IsEnabled = true;
            btnOptions.IsVisible = btnMessages.IsVisible = GHApp.DeveloperMode;
            btnGC.IsVisible = btnDebug.IsVisible = btnRunTests.IsVisible =
                GHApp.DeveloperMode && GHApp.DebugLogMessages;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if(!_backPressed)
            {
                _backPressed = true;
                MainLayout.IsEnabled = false;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }

        private void btnGC_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            btnGC.Text = "Collecting...";
            btnGC.TextColor = GHColors.Yellow;

            GHApp.DebugWriteRestart("Garbage Collection Start");
            GHApp.CollectGarbage();
            GHApp.DebugWriteProfilingStopwatchTimeAndStop("Garbage Collection End");

            btnGC.Text = "Done";
            btnGC.TextColor = GHColors.Red;
            MainLayout.IsEnabled = true;
        }

        private async void btnDebug_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            await GHApp.ListFileDescriptors(this);
            MainLayout.IsEnabled = true;
        }

        private async void btnRunTests_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
            await _gamePage.RunPerformanceTests();
            MainLayout.IsEnabled = true;
        }

        private async void btnTips_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if(_gamePage.ShownTip == -1)
                _gamePage.ShowGUITips(false);
            await App.Current.MainPage.Navigation.PopModalAsync();
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

                lblHeader.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private async void btnVersion_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var verPage = new VersionPage(_gamePage);
            await App.Current.MainPage.Navigation.PushModalAsync(verPage);
            MainLayout.IsEnabled = true;
        }

        private async void btnMessages_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);

            try
            {
                string ghdir = GHApp.GnollHackService.GetGnollHackPath();
                string targetpath = Path.Combine(ghdir, GHConstants.ArchiveDirectory);
                if (!Directory.Exists(targetpath))
                    GHApp.CheckCreateDirectory(targetpath);

                ulong vernum = GHApp.GHVersionNumber;
                ulong majorver = (vernum >> 24) & 0xFFUL;
                ulong minorver = (vernum >> 16) & 0xFFUL;
                ulong patchlvl = (vernum >> 8) & 0xFFUL;
                ulong editlvl = (vernum) & 0xFFUL;
                string versionstring = majorver.ToString() + minorver.ToString() + patchlvl.ToString() + "-" + editlvl;
                string filepath = Path.Combine(targetpath, "messages-" + versionstring + ".txt");
                if (File.Exists(filepath))
                    File.Delete(filepath);

                var curgame = _gamePage.CurrentGame;
                List<GHMsgHistoryItem> messages = curgame.GetMessages();
                using (StreamWriter sw = File.CreateText(filepath))
                {
                    foreach (GHMsgHistoryItem msg in messages)
                    {
                        sw.WriteLine(msg.Text);
                    }
                }

                if (File.Exists(filepath))
                {
                    await GHApp.ShareFile(this, filepath, "GnollHack Messages");
                    File.Delete(filepath);
                }
                else
                {
                    await DisplayAlert("Message File Not Found", "GnollHack could not find " + filepath + ".", "OK");
                }
            }
            catch (Exception ex) 
            {
                Debug.WriteLine(ex.Message);
                await DisplayAlert("Error Creating Message File", "An error occurred while creating the message file: " + ex.Message, "OK");
            }

            MainLayout.IsEnabled = true;
        }
    }
}
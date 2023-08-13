using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using GnollHackX.Pages.MainScreen;

namespace GnollHackX.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GameMenuPage : ContentPage
    {
        public GamePage _gamePage;
        //private int _optionsViewCellIndex = 1;
        //private int _gcViewCellIndex = 2;

        public GameMenuPage(GamePage gamePage)
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);

            _gamePage = gamePage;

            if (gamePage.EnableCasualMode)
            {
                btnSave.Text = "Save Game";
                btnQuit.Text = "Quit Game";
            }

            btnGC.IsVisible = GHApp.DeveloperMode;
            btnOptions.IsVisible = GHApp.DeveloperMode;

            //_gcViewCellIndex = InfoSection.IndexOf(GCViewCell);
            //_optionsViewCellIndex = InfoSection.IndexOf(OptionsViewCell);

            //if (!GHApp.DeveloperMode)
            //{
            //    InfoSection.Remove(GCViewCell);
            //    InfoSection.Remove(OptionsViewCell);
            //}
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

        private async void DoQuit()
        {
            MainLayout.IsEnabled = false;
            //GHApp.FmodService.StopTestSound();
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game Menu
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game
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
            btnGC.IsVisible = GHApp.DeveloperMode;
            btnOptions.IsVisible = GHApp.DeveloperMode;

            //if (GHApp.DeveloperMode && !InfoSection.Contains(OptionsViewCell) && _optionsViewCellIndex >= 0)
            //    InfoSection.Insert(_optionsViewCellIndex, OptionsViewCell);
            //if (!GHApp.DeveloperMode && InfoSection.Contains(OptionsViewCell))
            //    InfoSection.Remove(OptionsViewCell);
            //if (GHApp.DeveloperMode && !InfoSection.Contains(GCViewCell) && _gcViewCellIndex >= 0)
            //    InfoSection.Insert(_gcViewCellIndex, GCViewCell);
            //if (!GHApp.DeveloperMode && InfoSection.Contains(GCViewCell))
            //    InfoSection.Remove(GCViewCell);
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
            btnGC.TextColor = Color.Yellow;

            GHApp.DebugWriteRestart("Garbage Collection Start");
            GHApp.CollectGarbage();
            GHApp.DebugWriteProfilingStopwatchTimeAndStop("Garbage Collection End");

            btnGC.Text = "Done";
            btnGC.TextColor = Color.Red;
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

                lblHeader.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
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
    }
}
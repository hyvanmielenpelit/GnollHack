using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GameMenuPage : ContentPage
    {
        public GamePage _gamePage;
        private ViewCell _optionsViewCell;
        private ViewCell _gcViewCell;

        public GameMenuPage(GamePage gamePage)
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);

            _gamePage = gamePage;

            if (gamePage.EnableCasualMode)
            {
                btnSave.Text = "Save";
            }

            _gcViewCell = GCViewCell;
            _optionsViewCell = OptionsViewCell;
            if (!App.DeveloperMode)
            {
                BaseSection.Remove(GCViewCell);
                BaseSection.Remove(OptionsViewCell);
            }
        }

        private async void btnSave_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('s'));
        }

        private async void btnQuit_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('q'));
        }

        private async void btnBackToGame_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void DoQuit()
        {
            MainLayout.IsEnabled = false;
            //App.FmodService.StopTestSound();
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game Menu
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game
            if (App.IsServerGame)
            {
                await App.Current.MainPage.Navigation.PopAsync(); //Login
            }
        }

        private async void btnOptions_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
            App.DebugWriteRestart("ProfilingStopwatch.Restart: Options");
            _gamePage.GenericButton_Clicked(sender, e, 'O');
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void btnSettings_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
            var settingsPage = new SettingsPage(this, null);
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
            MainLayout.IsEnabled = true;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
        }

        public void UpdateLayout()
        {
            MainLayout.IsEnabled = true;
            if (App.DeveloperMode && !BaseSection.Contains(_optionsViewCell))
                BaseSection.Insert(2, _optionsViewCell);
            if (!App.DeveloperMode && BaseSection.Contains(_optionsViewCell))
                BaseSection.Remove(_optionsViewCell);
            if (App.DeveloperMode && !BaseSection.Contains(GCViewCell))
                BaseSection.Insert(4, GCViewCell);
            if (!App.DeveloperMode && BaseSection.Contains(GCViewCell))
                BaseSection.Remove(GCViewCell);
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if(!_backPressed)
            {
                _backPressed = true;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private void btnGC_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
            btnGC.Text = "Collecting...";
            btnGC.TextColor = Color.Yellow;

            App.DebugWriteRestart("Garbage Collection Start");
            
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();

            App.DebugWriteProfilingStopwatchTimeAndStop("Garbage Collection End");

            btnGC.Text = "Done";
            btnGC.TextColor = Color.Red;
            MainLayout.IsEnabled = true;
        }

        private async void btnTips_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            App.PlayButtonClickedSound();
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
            App.PlayButtonClickedSound();
            var verPage = new VersionPage(_gamePage);
            await App.Current.MainPage.Navigation.PushModalAsync(verPage);
            MainLayout.IsEnabled = true;
        }
    }
}
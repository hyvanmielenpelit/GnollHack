using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using GnollHackX.Controls;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class VaultPage : ContentPage
    {
        private MainPage _mainPage;

        public VaultPage(MainPage mainPage)
        {
            _mainPage = mainPage;

            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
            }

            LabeledImageButton rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.conduct.png";
            rib.LblText = "Top Scores";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 120;
            rib.ImgHeight = 120;
            rib.GridWidth = 120;
            rib.GridHeight = 150;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.BtnClicked += btnTopScores_Clicked;
            VaultLayout.Children.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.examine.png";
            rib.LblText = "Library";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 120;
            rib.ImgHeight = 120;
            rib.GridWidth = 120;
            rib.GridHeight = 150;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.BtnClicked += btnLibrary_Clicked;
            VaultLayout.Children.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.chronicle.png";
            rib.LblText = "Replays";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 120;
            rib.ImgHeight = 120;
            rib.GridWidth = 120;
            rib.GridHeight = 150;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.BtnClicked += btnReplays_Clicked;
            VaultLayout.Children.Add(rib);
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CurrentMainPage?.InvalidateCarousel();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                GHApp.CurrentMainPage?.InvalidateCarousel();
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            VaultLayout.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
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
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private async void btnTopScores_Clicked(object sender, EventArgs e)
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "xlogfile");
            if (File.Exists(fulltargetpath))
            {
                var topScorePage = new TopScorePage(fulltargetpath);
                string errormsg = "";
                if (!topScorePage.ReadFile(out errormsg))
                {
                    await DisplayAlert("Reading Top Scores Failed", "GnollHack failed to read the top scores file: " + errormsg, "OK");
                    VaultLayout.IsEnabled = true;
                }
                else
                {
                    await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
                }
            }
            else
            {
                /* No top scores */
                var topScorePage = new TopScorePage();
                await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
            }
            VaultLayout.IsEnabled = true;
        }

        private async void btnLibrary_Clicked(object sender, EventArgs e)
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var libPage = new LibraryPage();
            libPage.ReadLibrary();
            await App.Current.MainPage.Navigation.PushModalAsync(libPage);
            VaultLayout.IsEnabled = true;
        }

        private async void btnReplays_Clicked(object sender, EventArgs e)
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            ReplayPage selectFilePage = new ReplayPage(_mainPage);
            await App.Current.MainPage.Navigation.PushModalAsync(selectFilePage);

            VaultLayout.IsEnabled = true;
        }
    }
}
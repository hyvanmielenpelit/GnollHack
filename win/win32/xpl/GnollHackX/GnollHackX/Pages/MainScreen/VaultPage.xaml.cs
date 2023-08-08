using GnollHackX.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class VaultPage : ContentPage
    {
        public VaultPage()
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);

            LabeledImageButton rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://GnollHackX.Assets.UI.conduct.png";
            rib.LblText = "Top Scores";
            rib.LblFontSize = 20;
            rib.LblFontColor = Color.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 120;
            rib.ImgHeight = 120;
            rib.GridWidth = 120;
            rib.GridHeight = 150;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.BtnClicked += btnTopScores_Clicked;
            VaultLayout.Children.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://GnollHackX.Assets.UI.examine.png";
            rib.LblText = "Library";
            rib.LblFontSize = 20;
            rib.LblFontColor = Color.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 120;
            rib.ImgHeight = 120;
            rib.GridWidth = 120;
            rib.GridHeight = 150;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.BtnClicked += btnLibrary_Clicked;
            VaultLayout.Children.Add(rib);
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
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
            VaultTableView.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
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
                CloseButton.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private async void btnTopScores_Clicked(object sender, EventArgs e)
        {
            VaultTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(App.GHPath, "xlogfile");
            if (File.Exists(fulltargetpath))
            {
                var topScorePage = new TopScorePage(fulltargetpath);
                string errormsg = "";
                if (!topScorePage.ReadFile(out errormsg))
                {
                    await DisplayAlert("Reading Top Scores Failed", "GnollHack failed to read the top scores file: " + errormsg, "OK");
                    VaultTableView.IsEnabled = true;
                }
                else
                {
                    topScorePage.Disappearing += (sender2, e2) =>
                    {
                        VaultTableView.IsEnabled = true;
                    };
                    await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
                }
            }
            else
            {
                /* No top scores */
                var topScorePage = new TopScorePage();
                topScorePage.Disappearing += (sender2, e2) =>
                {
                    VaultTableView.IsEnabled = true;
                };
                await App.Current.MainPage.Navigation.PushModalAsync(topScorePage);
            }
            VaultTableView.IsEnabled = true;
        }

        private async void btnLibrary_Clicked(object sender, EventArgs e)
        {
            VaultTableView.IsEnabled = false;
            App.PlayButtonClickedSound();
            var libPage = new LibraryPage();
            libPage.ReadLibrary();
            await App.Current.MainPage.Navigation.PushModalAsync(libPage);
            VaultTableView.IsEnabled = true;

        }

    }
}
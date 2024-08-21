﻿using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Net.WebRequestMethods;
using static System.Net.Mime.MediaTypeNames;


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

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AboutPage : ContentPage
    {
        MainPage _mainPage = null;
        public AboutPage(MainPage mainPage)
        {
            _mainPage = mainPage;
            Disappearing += (s, e) => { _mainPage.StartCarouselViewAndEnableButtons(); };

            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
            }
        }

        private async void btnCreditsX_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "xcredits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Credits", 77);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the xcredits file.", "OK");
            }
            else
            {
                await GHApp.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }

        //private async void btnCreditsW_Clicked(object sender, EventArgs e)
        //{
        //    AboutGrid.IsEnabled = false;
        //    GHApp.PlayButtonClickedSound();
        //    string fulltargetpath = Path.Combine(GHApp.GHPath, "credits");
        //    var displFilePage = new DisplayFilePage(fulltargetpath, "Credits", 77);
        //    string errormsg = "";
        //    if (!displFilePage.ReadFile(out errormsg))
        //    {
        //        await DisplayAlert("Error Opening File", "GnollHack cannot open the credits file.", "OK");
        //    }
        //    else
        //    {
        //        await GHApp.Navigation.PushModalAsync(displFilePage);
        //    }
        //    AboutGrid.IsEnabled = true;
        //}

        private async void btnLicense_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "license");
            var displFilePage = new DisplayFilePage(fulltargetpath, "License", 78);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the license file.", "OK");
            }
            else
            {
                await GHApp.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }
        private async void btnGitHub_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Source", new Uri(GHConstants.GnollHackGitHubPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnWebPage_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "GnollHack.com", new Uri(GHConstants.GnollHackWebPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnWiki_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Wiki", new Uri(GHConstants.GnollHackWikiPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnDowngrade_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Downgrade", new Uri(GHApp.IsAndroid ? GHConstants.GnollHackAndroidDowngradePage : GHApp.IsiOS ? GHConstants.GnollHackiOSDowngradePage :  GHConstants.GnollHackGeneralDowngradePage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnSponsor_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Sponsor", new Uri(GHConstants.GnollHackSponsorPage));
            AboutGrid.IsEnabled = true;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CurrentMainPage?.InvalidateCarousel();
            await GHApp.Navigation.PopModalAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                AboutGrid.IsEnabled = false;
                GHApp.CurrentMainPage?.InvalidateCarousel();
                await GHApp.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            AboutGrid.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }

        private async void btnCrashReport_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Send Crash Report?", "This will create a zip archive of the files in your game directory and ask it to be shared further.", "Yes", "No");
            if (answer)
            {
                await GHApp.CreateCrashReport(this);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnViewPanicLog_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "paniclog");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Panic Log", 0, true);
            string errormsg;
            if (!System.IO.File.Exists(fulltargetpath))
            {
                await DisplayAlert("No Panic Log", "Panic Log does not exist.", "OK");
            }
            else if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the paniclog file: " + errormsg, "OK");
            }
            else
            {
                await GHApp.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnViewGHLog_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, GHConstants.AppLogDirectory, GHConstants.AppLogFileName);
            var displFilePage = new DisplayFilePage(fulltargetpath, "App Log", 0, true, false, true);
            string errormsg;
            
            if (!System.IO.File.Exists(fulltargetpath))
            {
                await DisplayAlert("No App Log", "App Log does not exist.", "OK");
            }
            else if (!displFilePage.ReadFile(out errormsg))
            {
                await DisplayAlert("Error Opening File", "GnollHack cannot open the App Log file: " + errormsg, "OK");
            }
            else
            {
                await GHApp.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
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

        private async void btnVersion_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var verPage = new VersionPage(null);
            await GHApp.Navigation.PushModalAsync(verPage);
            AboutGrid.IsEnabled = true;

        }

        private async void btnReplays_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            ReplayPage selectFilePage = new ReplayPage(_mainPage);
            await GHApp.Navigation.PushModalAsync(selectFilePage);

            AboutGrid.IsEnabled = true;
        }

        private async void btnImportExport_Clicked(object sender, EventArgs e)
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            ImportExportPage manageFilesPage = new ImportExportPage();
            await GHApp.Navigation.PushModalAsync(manageFilesPage);

            AboutGrid.IsEnabled = true;
        }
    }
}
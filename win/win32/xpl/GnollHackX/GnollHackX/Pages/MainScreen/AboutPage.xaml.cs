using System;
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
using Xamarin.Essentials;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AboutPage : ContentPage, ICloseablePage
    {
        MainPage _mainPage = null;
        public AboutPage(MainPage mainPage)
        {
            _mainPage = mainPage;
            Disappearing += (s, e) => { _mainPage.StartCarouselViewAndEnableButtons(); };

            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
            }
            if (GHApp.IsSteam)
            {
                btnSponsor.IsVisible = false;
            }
        }

        private async void btnCreditsX_Clicked(object sender, EventArgs e)
        {
            await OpenCreditsPage();
        }

        private async Task OpenCreditsPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "xcredits");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Credits", 77);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await GHApp.DisplayMessageBox(this, "Error Opening File", "GnollHack cannot open the xcredits file.", "OK");
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
        //        await GHApp.DisplayMessageBox(this, "Error Opening File", "GnollHack cannot open the credits file.", "OK");
        //    }
        //    else
        //    {
        //        await GHApp.Navigation.PushModalAsync(displFilePage);
        //    }
        //    AboutGrid.IsEnabled = true;
        //}

        private async void btnLicense_Clicked(object sender, EventArgs e)
        {
            await OpenLicensePage();
        }

        private async Task OpenLicensePage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "license");
            var displFilePage = new DisplayFilePage(fulltargetpath, "License", 78);
            string errormsg = "";
            if (!displFilePage.ReadFile(out errormsg))
            {
                await GHApp.DisplayMessageBox(this, "Error Opening File", "GnollHack cannot open the license file.", "OK");
            }
            else
            {
                await GHApp.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnGitHub_Clicked(object sender, EventArgs e)
        {
            await OpenGitHubPage();
        }

        private async Task OpenGitHubPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Source", new Uri(GHConstants.GnollHackGitHubPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnWebPage_Clicked(object sender, EventArgs e)
        {
            await OpenWebPage();
        }

        private async Task OpenWebPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "GnollHack.com", new Uri(GHConstants.GnollHackWebPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnWiki_Clicked(object sender, EventArgs e)
        {
            await OpenWikiPage();
        }

        private async Task OpenWikiPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Wiki", new Uri(GHConstants.GnollHackWikiPage));
            AboutGrid.IsEnabled = true;
        }

        private async void btnSponsor_Clicked(object sender, EventArgs e)
        {
            await OpenSponsorPage();
        }

        private async Task OpenSponsorPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Sponsor", new Uri(GHConstants.GnollHackSponsorPage));
            AboutGrid.IsEnabled = true;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync();
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (AboutGrid.IsEnabled)
                            await ClosePageAsync();
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }

                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        private async Task ClosePageAsync()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CurrentMainPage?.InvalidateCarousel();
            var page = await GHApp.Navigation.PopModalAsync();
            GHApp.DisconnectIViewHandlers(page);
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                AboutGrid.IsEnabled = false;
                GHApp.CurrentMainPage?.InvalidateCarousel();
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
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
            await DoCrashReport();
        }

        private async Task DoCrashReport()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool answer = await GHApp.DisplayMessageBox(this, "Send Crash Report?", "This will create a zip archive of the files in your game directory and ask it to be shared further.", "Yes", "No");
            if (answer)
            {
                await GHApp.CreateCrashReport(this);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnViewPanicLog_Clicked(object sender, EventArgs e)
        {
            await OpenPanicLog();
        }

        private async Task OpenPanicLog()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "paniclog");
            var displFilePage = new DisplayFilePage(fulltargetpath, "Panic Log", 0, true);
            string errormsg;
            if (!System.IO.File.Exists(fulltargetpath))
            {
                await GHApp.DisplayMessageBox(this, "No Panic Log", "Panic Log does not exist.", "OK");
            }
            else if (!displFilePage.ReadFile(out errormsg))
            {
                await GHApp.DisplayMessageBox(this, "Error Opening File", "GnollHack cannot open the paniclog file: " + errormsg, "OK");
            }
            else
            {
                await GHApp.Navigation.PushModalAsync(displFilePage);
            }
            AboutGrid.IsEnabled = true;
        }

        private async void btnViewGHLog_Clicked(object sender, EventArgs e)
        {
            await OpenAppLog();
        }

        private async Task OpenAppLog()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, GHConstants.AppLogDirectory, GHConstants.AppLogFileName);
            var displFilePage = new DisplayFilePage(fulltargetpath, "App Log", 0, true, false, true);
            string errormsg;

            if (!System.IO.File.Exists(fulltargetpath))
            {
                await GHApp.DisplayMessageBox(this, "No App Log", "App Log does not exist.", "OK");
            }
            else if (!displFilePage.ReadFile(out errormsg))
            {
                await GHApp.DisplayMessageBox(this, "Error Opening File", "GnollHack cannot open the App Log file: " + errormsg, "OK");
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
            await OpenVersionPage();
        }

        private async Task OpenVersionPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var verPage = new VersionPage(null);
            await GHApp.Navigation.PushModalAsync(verPage);
            AboutGrid.IsEnabled = true;
        }

        private async void btnImportExport_Clicked(object sender, EventArgs e)
        {
            await OpenImportExportPage();
        }

        private async Task OpenImportExportPage()
        {
            AboutGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            ImportExportPage manageFilesPage = new ImportExportPage();
            await GHApp.Navigation.PushModalAsync(manageFilesPage);

            AboutGrid.IsEnabled = true;
        }

        public bool HandleKeyPress(int key, bool isCtrl, bool isMeta)
        {
            bool handled = false;
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        switch (key)
                        {
                            case (int)'v':
                                if (btnVersion.IsEnabled && btnVersion.IsVisible && AboutGrid.IsEnabled)
                                    await OpenVersionPage();
                                handled = true;
                                break;
                            case (int)'w':
                                if (btnWiki.IsEnabled && btnWiki.IsVisible && AboutGrid.IsEnabled)
                                    await OpenWikiPage();
                                handled = true;
                                break;
                            case (int)'c':
                                if (btnCreditsX.IsEnabled && btnCreditsX.IsVisible && AboutGrid.IsEnabled)
                                    await OpenCreditsPage();
                                handled = true;
                                break;
                            case (int)'l':
                                if (btnLicense.IsEnabled && btnLicense.IsVisible && AboutGrid.IsEnabled)
                                    await OpenLicensePage();
                                handled = true;
                                break;
                            case (int)'s':
                                if (btnGitHub.IsEnabled && btnGitHub.IsVisible && AboutGrid.IsEnabled)
                                    await OpenGitHubPage();
                                handled = true;
                                break;
                            case (int)'g':
                                if (btnWebPage.IsEnabled && btnWebPage.IsVisible && AboutGrid.IsEnabled)
                                    await OpenWebPage();
                                handled = true;
                                break;
                            case (int)'d':
                            case (int)'S':
                                if (btnSponsor.IsEnabled && btnSponsor.IsVisible && AboutGrid.IsEnabled)
                                    await OpenSponsorPage();
                                handled = true;
                                break;
                            case (int)'a':
                                if (btnViewGHLog.IsEnabled && btnViewGHLog.IsVisible && AboutGrid.IsEnabled)
                                    await OpenAppLog();
                                handled = true;
                                break;
                            case (int)'p':
                                if (btnViewPanicLog.IsEnabled && btnViewPanicLog.IsVisible && AboutGrid.IsEnabled)
                                    await OpenPanicLog();
                                handled = true;
                                break;
                            case (int)'m':
                                if (btnImportExport.IsEnabled && btnImportExport.IsVisible && AboutGrid.IsEnabled)
                                    await OpenImportExportPage();
                                handled = true;
                                break;
                            case (int)'r':
                                if (btnCrashReport.IsEnabled && btnCrashReport.IsVisible && AboutGrid.IsEnabled)
                                    await DoCrashReport();
                                handled = true;
                                break;
                            default:
                                break;
                        }
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
            return handled;
        }

    }
}
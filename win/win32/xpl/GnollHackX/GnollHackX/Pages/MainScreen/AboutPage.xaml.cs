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
    public partial class AboutPage : CustomModalPage, ICloseablePage, IKeyPressHandlingPage
    {
        MainPage _mainPage = null;
        public AboutPage(MainPage mainPage)
        {
            _mainPage = mainPage;
            Disappearing += (s, e) => { _mainPage.StartCarouselViewAndEnableButtons(); };

            InitializeComponent();
//#if GNH_MAUI
//            SafeAreaEdges = SafeAreaEdges.All;
//#else
//            On<iOS>().SetUseSafeArea(true);
//#endif
            //UIUtils.AdjustRootLayout(RootGrid);
            //UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            //UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                HeaderLabel.TextColor = GHColors.White;
            }
            if (GHApp.IsSteam)
            {
                SponsorButton.IsVisible = false;
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
                await GHApp.PushModalPageAsync(displFilePage);
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
        //        await GHApp.PushModalPageAsync(displFilePage);
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
                await GHApp.PushModalPageAsync(displFilePage);
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
            await ClosePageAsync(true);
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
                            await ClosePageAsync(true);
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

        private async Task ClosePageAsync(bool playClickSound)
        {
            AboutGrid.IsEnabled = false;
            _backPressed = true;
            if (playClickSound)
                GHApp.PlayButtonClickedSound();
            GHApp.CurrentMainPage?.InvalidateCarousel();
            await GHApp.PopModalPageAsync();
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                await ClosePageAsync(false);
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
        //protected override bool OnBackButtonPressed()
        //{
        //    return true;
        //}


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
                await GHApp.PushModalPageAsync(displFilePage);
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
                await GHApp.PushModalPageAsync(displFilePage);
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

                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
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
            await GHApp.PushModalPageAsync(verPage);
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
            await GHApp.PushModalPageAsync(manageFilesPage);

            AboutGrid.IsEnabled = true;
        }

        public bool HandleKeyPress(int key, bool isCtrl, bool isMeta)
        {
            if (GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen) /* Ignore key presses when opening a page or using a system browser */
                return true;

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
                                if (VersionButton.IsEnabled && VersionButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenVersionPage();
                                handled = true;
                                break;
                            case (int)'w':
                                if (WikiButton.IsEnabled && WikiButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenWikiPage();
                                handled = true;
                                break;
                            case (int)'c':
                                if (CreditsXButton.IsEnabled && CreditsXButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenCreditsPage();
                                handled = true;
                                break;
                            case (int)'l':
                                if (LicenseButton.IsEnabled && LicenseButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenLicensePage();
                                handled = true;
                                break;
                            case (int)'s':
                                if (GitHubButton.IsEnabled && GitHubButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenGitHubPage();
                                handled = true;
                                break;
                            case (int)'g':
                                if (WebPageButton.IsEnabled && WebPageButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenWebPage();
                                handled = true;
                                break;
                            case (int)'d':
                            case (int)'S':
                                if (SponsorButton.IsEnabled && SponsorButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenSponsorPage();
                                handled = true;
                                break;
                            case (int)'a':
                                if (ViewGHLogButton.IsEnabled && ViewGHLogButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenAppLog();
                                handled = true;
                                break;
                            case (int)'p':
                                if (ViewPanicLogButton.IsEnabled && ViewPanicLogButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenPanicLog();
                                handled = true;
                                break;
                            case (int)'m':
                                if (ImportExportButton.IsEnabled && ImportExportButton.IsVisible && AboutGrid.IsEnabled)
                                    await OpenImportExportPage();
                                handled = true;
                                break;
                            case (int)'r':
                                if (CrashReportButton.IsEnabled && CrashReportButton.IsVisible && AboutGrid.IsEnabled)
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
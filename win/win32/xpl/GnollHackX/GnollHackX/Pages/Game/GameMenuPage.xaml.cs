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
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using Xamarin.Essentials;
using GnollHackX.Pages.MainScreen;

namespace GnollHackX.Pages.Game
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GameMenuPage : ContentPage, ICloseablePage
    {
        public GamePage _gamePage;
 
        public GameMenuPage(GamePage gamePage)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _gamePage = gamePage;
            if (gamePage.EnableCasualMode)
            {
                btnSave.Text = "Save Game";
                btnQuit.Text = "Quit Game";
            }
            else if (!gamePage.EnableWizardMode)
            {
                btnQuit.Text = "Delete Character";
            }

            if (gamePage.GameEnded)
            {
                btnQuit.Text = "Finish Game Over";
                btnSave.TextColor = GHColors.Gray;
                btnSave.IsEnabled = false;
                btnOptions.TextColor = GHColors.Gray;
                btnOptions.IsEnabled = false;
            }

            btnOptions.IsVisible = btnMessages.IsVisible = GHApp.DeveloperMode;
            btnGC.IsVisible = GHApp.DeveloperMode && GHApp.DebugLogMessages;
            UpdateDarknessMode();
        }

        public GameMenuPage(GamePage gamePage, bool isLimited) : this(gamePage)
        {
            if(isLimited)
            {
                btnSave.IsVisible = false;
                btnQuit.IsVisible = false;
                btnSettings.IsVisible = false;
                btnOptions.IsVisible = false;
                btnMessages.IsVisible = false;
                btnGC.IsVisible = false;
                btnVersion.IsVisible = false;
                btnTips.IsVisible = false;
            }
        }

        public void UpdateDarknessMode()
        {
            lblHeader.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            bkgView.InvalidateSurface();
        }

        private async void btnSave_Clicked(object sender, EventArgs e)
        {
            await CloseAndSaveGame();
        }

        private async Task CloseAndSaveGame()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CollectNursery();
            var page = await GHApp.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(btnSave, EventArgs.Empty, GHUtils.Meta('s'));
            GHApp.DisconnectIViewHandlers(page);
        }

        private async void btnQuit_Clicked(object sender, EventArgs e)
        {
            await CloseAndQuitGame();
        }

        private async Task CloseAndQuitGame()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CollectNursery();
            var page = await GHApp.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(btnQuit, EventArgs.Empty, _gamePage.GameEnded ? 'q' : GHUtils.Meta('q'));
            GHApp.DisconnectIViewHandlers(page);
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    if (MainLayout.IsEnabled)
                        await BackToGame();
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
    }

        private async void btnBackToGame_Clicked(object sender, EventArgs e)
        {
            await BackToGame();
        }
        private async Task BackToGame()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CollectNursery();
            var page = await GHApp.Navigation.PopModalAsync();
            GHApp.DisconnectIViewHandlers(page);
        }

        private async void btnOptions_Clicked(object sender, EventArgs e)
        {
            await CloseAndShowOptions();
        }

        private async Task CloseAndShowOptions()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.DebugWriteRestart("ProfilingStopwatch.Restart: Options");
            GHApp.CollectNursery();
            var page = await GHApp.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(btnOptions, EventArgs.Empty, 'O');
            GHApp.DisconnectIViewHandlers(page);
        }

        private async void btnSnapshot_Clicked(object sender, EventArgs e)
        {
            await TakeSnapshot();
        }

        private async Task TakeSnapshot()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.CollectNursery();
            var page = await GHApp.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(btnSnapshot, EventArgs.Empty, GHUtils.Meta(28));
            GHApp.DisconnectIViewHandlers(page);
        }

        private async void btnSettings_Clicked(object sender, EventArgs e)
        {
            await OpenSettingsPage();
        }

        private async Task OpenSettingsPage()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var settingsPage = new SettingsPage(this, null);
            await GHApp.Navigation.PushModalAsync(settingsPage);
            MainLayout.IsEnabled = true;
        }

        private async void btnLibrary_Clicked(object sender, EventArgs e)
        {
            await OpenLibraryPage();
        }

        private async Task OpenLibraryPage()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var libPage = new LibraryPage();
            libPage.ReadLibrary();
            await GHApp.Navigation.PushModalAsync(libPage);
            MainLayout.IsEnabled = true;
        }

        private async void btnDelphi_Clicked(object sender, EventArgs e)
        {
            await OpenOraclePage();
        }

        private async Task OpenOraclePage()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var oraclePage = new OraclePage();
            oraclePage.ReadConsultations();
            await GHApp.Navigation.PushModalAsync(oraclePage);
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
            btnGC.IsVisible = GHApp.DeveloperMode && GHApp.DebugLogMessages;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if(!_backPressed)
            {
                _backPressed = true;
                MainLayout.IsEnabled = false;
                GHApp.CollectNursery();
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
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

        private async void btnTips_Clicked(object sender, EventArgs e)
        {
            await CloseAndShowGUITips();
        }

        private async Task CloseAndShowGUITips()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if (_gamePage.ShownTip == -1)
                _gamePage.ShowGUITips(false);
            GHApp.CollectNursery();
            var page = await GHApp.Navigation.PopModalAsync();
            GHApp.DisconnectIViewHandlers(page);
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
            await OpenVersionPage();
        }

        private async Task OpenVersionPage()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var verPage = new VersionPage(_gamePage);
            await GHApp.Navigation.PushModalAsync(verPage);
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
                    await GHApp.DisplayMessageBox(this, "Message File Not Found", "GnollHack could not find " + filepath + ".", "OK");
                }
            }
            catch (Exception ex) 
            {
                Debug.WriteLine(ex.Message);
                await GHApp.DisplayMessageBox(this, "Error Creating Message File", "An error occurred while creating the message file: " + ex.Message, "OK");
            }

            MainLayout.IsEnabled = true;
        }

        private async void btnWiki_Clicked(object sender, EventArgs e)
        {
            await OpenWikiPage();
        }

        private async Task OpenWikiPage()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if (GHApp.IsiOS)
            {
                /* Navigated event does not trigger with WebView on iOS, making navigation buttons inactivate all the time; use OpenBrowser instead (does not trigger OnSleep / OnResume on iOS) */
                await GHApp.OpenBrowser(this, "Wiki", new Uri(GHConstants.GnollHackWikiPage));
            }
            else
            {
                /* Android seems to trigger app switching (OnSleep / OnResume) using OpenBrowser; use WebView instead */
                var wikiPage = new WikiPage("Wiki", GHConstants.GnollHackWikiPage);
                await GHApp.Navigation.PushModalAsync(wikiPage);
            }
            MainLayout.IsEnabled = true;
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
                            case (int)'s':
                                if(isMeta)
                                {
                                    if (btnSave.IsEnabled && btnSave.IsVisible && MainLayout.IsEnabled)
                                        await CloseAndSaveGame();
                                    handled = true;
                                }
                                else if (!isCtrl)
                                {
                                    if (btnSettings.IsEnabled && btnSettings.IsVisible && MainLayout.IsEnabled)
                                        await OpenSettingsPage();
                                    handled = true;
                                }
                                break;
                            case (int)'o':
                                if (btnOptions.IsEnabled && btnOptions.IsVisible && MainLayout.IsEnabled)
                                    await CloseAndShowOptions();
                                handled = true;
                                break;
                            case (int)'v':
                                if (btnVersion.IsEnabled && btnVersion.IsVisible && MainLayout.IsEnabled)
                                    await OpenVersionPage();
                                handled = true;
                                break;
                            case (int)'t':
                                if (btnSnapshot.IsEnabled && btnSnapshot.IsVisible && MainLayout.IsEnabled)
                                    await TakeSnapshot();
                                handled = true;
                                break;
                            case (int)'l':
                                if (btnLibrary.IsEnabled && btnLibrary.IsVisible && MainLayout.IsEnabled)
                                    await OpenLibraryPage();
                                handled = true;
                                break;
                            case (int)'d':
                                if (btnDelphi.IsEnabled && btnDelphi.IsVisible && MainLayout.IsEnabled)
                                    await OpenOraclePage();
                                handled = true;
                                break;
                            case (int)'w':
                                if (btnWiki.IsEnabled && btnWiki.IsVisible && MainLayout.IsEnabled)
                                    await OpenWikiPage();
                                handled = true;
                                break;
                            case (int)'u':
                                if (btnTips.IsEnabled && btnTips.IsVisible && MainLayout.IsEnabled)
                                    await CloseAndShowGUITips();
                                handled = true;
                                break;
                            default:
                                break;
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return handled;
        }

        public bool HandleSpecialKeyPress(GHSpecialKey key, bool isCtrl, bool isMeta, bool isShift)
        {
            bool handled = false;
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (key == GHSpecialKey.Escape)
                        {
                            if (btnBackToGame.IsEnabled && btnBackToGame.IsVisible && MainLayout.IsEnabled)
                                await BackToGame();
                        }
                        else if (isMeta && key == GHSpecialKey.A + 's' - 'a')
                        {
                            if (btnSave.IsEnabled && btnSave.IsVisible && MainLayout.IsEnabled)
                                await CloseAndSaveGame();
                            handled = true;
                        }
                        else if (isMeta && key == GHSpecialKey.A + 'q' - 'a')
                        {
                            if (btnQuit.IsEnabled && btnQuit.IsVisible && MainLayout.IsEnabled)
                                await CloseAndQuitGame();
                            handled = true;
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return handled;
        }

    }
}
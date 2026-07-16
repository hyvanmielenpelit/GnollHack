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
    public partial class GameMenuPage : CustomModalPage, ICloseablePage, IKeyPressHandlingPage, ISpecialKeyPressHandlingPage
    {
        public GamePage _gamePage;
 
        public GameMenuPage(GamePage gamePage)
        {
            InitializeComponent();
//#if GNH_MAUI
//            SafeAreaEdges = SafeAreaEdges.All;
//#else
//            On<iOS>().SetUseSafeArea(true);
//#endif
            //UIUtils.AdjustRootLayout(RootGrid);
            //UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            //UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _gamePage = gamePage;
            if (gamePage.EnableCasualMode)
            {
                SaveButton.Text = "Save Game";
                QuitButton.Text = "Quit Game";
            }
            else if (!gamePage.EnableWizardMode)
            {
                QuitButton.Text = "Delete Character";
            }

            if (gamePage.GameEnded)
            {
                QuitButton.Text = "Finish Game Over";
                SaveButton.TextColor = GHColors.Gray;
                SaveButton.IsEnabled = false;
                OptionsButton.TextColor = GHColors.Gray;
                OptionsButton.IsEnabled = false;
            }

            OptionsButton.IsVisible = MessagesButton.IsVisible = GHApp.DeveloperMode;
            GCButton.IsVisible = GHApp.DeveloperMode && GHApp.DebugLogMessages && GHApp.LowLevelLogging; /* Just do not show almost ever, since there is little space  */
            UpdateDarknessMode();
        }

        public GameMenuPage(GamePage gamePage, bool isLimited) : this(gamePage)
        {
            if(isLimited)
            {
                SaveButton.IsVisible = false;
                QuitButton.IsVisible = false;
                SettingsButton.IsVisible = false;
                OptionsButton.IsVisible = false;
                MessagesButton.IsVisible = false;
                GCButton.IsVisible = false;
                VersionButton.IsVisible = false;
                TipsButton.IsVisible = false;
            }
        }

        public void UpdateDarknessMode()
        {
            HeaderLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
            //GHApp.AddSentryBreadcrumb(SaveButton.Text + " menu button pressed.", GHConstants.SentryGnollHackButtonClickCategoryName);
            _gamePage.GenericButton_Clicked(SaveButton, EventArgs.Empty, GHApp.MapCommand(GHUtils.Meta('s')));
            await GHApp.PopModalPageAsync();
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
            //GHApp.AddSentryBreadcrumb(QuitButton.Text + " menu button pressed.", GHConstants.SentryGnollHackButtonClickCategoryName);
            _gamePage.GenericButton_Clicked(QuitButton, EventArgs.Empty, _gamePage.GameEnded ? 'q' : GHApp.MapCommand(GHUtils.Meta('q')));
            await GHApp.PopModalPageAsync();
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
            _backPressed = true;
            GHApp.PlayButtonClickedSound();
            GHApp.CollectNursery();
            await GHApp.PopModalPageAsync();
            GHApp.UpdateFreeDiskSpace();
            GHApp.UpdateUsedMemory();
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
            _gamePage.GenericButton_Clicked(OptionsButton, EventArgs.Empty, GHApp.MapCommand('O'));
            await GHApp.PopModalPageAsync();
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
            _gamePage.GenericButton_Clicked(SnapshotButton, EventArgs.Empty, GHApp.MapCommand(GHUtils.Meta(29)));
            await GHApp.PopModalPageAsync();
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
            await GHApp.PushModalPageAsync(settingsPage);
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
            await GHApp.PushModalPageAsync(libPage);
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
            await GHApp.PushModalPageAsync(oraclePage);
            MainLayout.IsEnabled = true;
        }

        public void UpdateLayout()
        {
            MainLayout.IsEnabled = true;
            OptionsButton.IsVisible = MessagesButton.IsVisible = GHApp.DeveloperMode;
            GCButton.IsVisible = GHApp.DeveloperMode && GHApp.DebugLogMessages;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if(!_backPressed)
            {
                _backPressed = true;
                MainLayout.IsEnabled = false;
                GHApp.CollectNursery();
                await GHApp.PopModalPageAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }

        //protected override bool OnBackButtonPressed()
        //{
        //    return true;
        //}

        private void btnGC_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GCButton.Text = "Collecting...";
            GCButton.TextColor = GHColors.Yellow;

            GHApp.DebugWriteRestart("Garbage Collection Start");
            GHApp.CollectGarbage();
            GHApp.DebugWriteProfilingStopwatchTimeAndStop("Garbage Collection End");

            GCButton.Text = "Done";
            GCButton.TextColor = GHColors.Red;
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
            await GHApp.PopModalPageAsync();
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
            await GHApp.PushModalPageAsync(verPage);
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

                var curgame = GHApp.CurrentGHGame;
                GHMsgHistorySpan messages = curgame.GetMessages();
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
                await GHApp.PushModalPageAsync(wikiPage);
            }
            MainLayout.IsEnabled = true;
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
                            case (int)'s':
                                if(isMeta)
                                {
                                    if (SaveButton.IsEnabled && SaveButton.IsVisible && MainLayout.IsEnabled)
                                        await CloseAndSaveGame();
                                    handled = true;
                                }
                                else if (!isCtrl)
                                {
                                    if (SettingsButton.IsEnabled && SettingsButton.IsVisible && MainLayout.IsEnabled)
                                        await OpenSettingsPage();
                                    handled = true;
                                }
                                break;
                            case (int)'o':
                                if (OptionsButton.IsEnabled && OptionsButton.IsVisible && MainLayout.IsEnabled)
                                    await CloseAndShowOptions();
                                handled = true;
                                break;
                            case (int)'v':
                                if (VersionButton.IsEnabled && VersionButton.IsVisible && MainLayout.IsEnabled)
                                    await OpenVersionPage();
                                handled = true;
                                break;
                            case (int)'t':
                                if (SnapshotButton.IsEnabled && SnapshotButton.IsVisible && MainLayout.IsEnabled)
                                    await TakeSnapshot();
                                handled = true;
                                break;
                            case (int)'l':
                                if (LibraryButton.IsEnabled && LibraryButton.IsVisible && MainLayout.IsEnabled)
                                    await OpenLibraryPage();
                                handled = true;
                                break;
                            case (int)'d':
                                if (DelphiButton.IsEnabled && DelphiButton.IsVisible && MainLayout.IsEnabled)
                                    await OpenOraclePage();
                                handled = true;
                                break;
                            case (int)'w':
                                if (WikiButton.IsEnabled && WikiButton.IsVisible && MainLayout.IsEnabled)
                                    await OpenWikiPage();
                                handled = true;
                                break;
                            case (int)'u':
                                if (TipsButton.IsEnabled && TipsButton.IsVisible && MainLayout.IsEnabled)
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
            if (GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen) /* Ignore key presses when opening a page or using a system browser */
                return true;

            bool handled = false;
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (key == GHSpecialKey.Escape)
                        {
                            if (BackToGameButton.IsEnabled && BackToGameButton.IsVisible && MainLayout.IsEnabled)
                                await BackToGame();
                        }
                        else if (isMeta && key == GHSpecialKey.A + 's' - 'a')
                        {
                            if (SaveButton.IsEnabled && SaveButton.IsVisible && MainLayout.IsEnabled)
                                await CloseAndSaveGame();
                            handled = true;
                        }
                        else if (isMeta && key == GHSpecialKey.A + 'q' - 'a')
                        {
                            if (QuitButton.IsEnabled && QuitButton.IsVisible && MainLayout.IsEnabled)
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
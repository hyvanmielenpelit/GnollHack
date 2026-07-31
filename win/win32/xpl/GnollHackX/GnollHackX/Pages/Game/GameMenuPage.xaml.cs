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
    public partial class GameMenuPage : CustomModalPage, ICloseablePage, IMessagePopupPage, IKeyPressHandlingPage, ISpecialKeyPressHandlingPage
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
                btnDevOptions.TextColor = GHColors.Gray;
                btnDevOptions.IsEnabled = false;
            }

            btnDeveloper.IsVisible = GHApp.DeveloperMode;
            UpdateDarknessMode();

            FrameTimeProfiler.MarkPauseEvent();
        }

        public GameMenuPage(GamePage gamePage, bool isLimited) : this(gamePage)
        {
            if(isLimited)
            {
                btnSave.IsVisible = false;
                btnQuit.IsVisible = false;
                btnSettings.IsVisible = false;
                btnDeveloper.IsVisible = false;
                btnVersion.IsVisible = false;
                btnTips.IsVisible = false;
            }
        }

        public void UpdateDarknessMode()
        {
            bool isDarkMode = GHApp.DarkMode;
            lblHeader.TextColor = isDarkMode ? GHColors.White : GHColors.Black;
            if (isDarkMode)
                DeveloperPopupFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
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
            FrameTimeProfiler.MarkPauseEvent();
            GHApp.CollectNursery();
            //GHApp.AddSentryBreadcrumb(btnSave.Text + " menu button pressed.", GHConstants.SentryGnollHackButtonClickCategoryName);
            _gamePage.GenericButton_Clicked(btnSave, EventArgs.Empty, GHApp.MapCommand(GHUtils.Meta('s')));
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
            FrameTimeProfiler.MarkPauseEvent();
            GHApp.CollectNursery();
            //GHApp.AddSentryBreadcrumb(btnQuit.Text + " menu button pressed.", GHConstants.SentryGnollHackButtonClickCategoryName);
            _gamePage.GenericButton_Clicked(btnQuit, EventArgs.Empty, _gamePage.GameEnded ? 'q' : GHApp.MapCommand(GHUtils.Meta('q')));
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
            FrameTimeProfiler.MarkPauseEvent();
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
            DeveloperPopupGrid.IsVisible = false;
            GHApp.PlayButtonClickedSound();
            GHApp.DebugWriteRestart("ProfilingStopwatch.Restart: Options");
            FrameTimeProfiler.MarkPauseEvent();
            GHApp.CollectNursery();
            _gamePage.GenericButton_Clicked(btnDevOptions, EventArgs.Empty, GHApp.MapCommand('O'));
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
            FrameTimeProfiler.MarkPauseEvent();
            GHApp.CollectNursery();
            _gamePage.GenericButton_Clicked(btnSnapshot, EventArgs.Empty, GHApp.MapCommand(GHUtils.Meta(29)));
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
            btnDeveloper.IsVisible = GHApp.DeveloperMode;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if(!_backPressed)
            {
                _backPressed = true;
                MainLayout.IsEnabled = false;
                FrameTimeProfiler.MarkPauseEvent();
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

        private void btnDeveloper_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            btnDevDumpFrameLog.IsVisible = FrameTimeProfiler.IsEnabled;
            DeveloperPopupGrid.IsVisible = true;
        }

        private void CloseDeveloperPopup()
        {
            GHApp.PlayButtonClickedSound();
            DeveloperPopupGrid.IsVisible = false;
        }

        private void btnDevClose_Clicked(object sender, EventArgs e)
        {
            CloseDeveloperPopup();
        }

        private void DeveloperPopupOverlay_Tapped(object sender, EventArgs e)
        {
            CloseDeveloperPopup();
        }

        private void btnGC_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            btnDevGC.Text = "Collecting...";
            btnDevGC.TextColor = GHColors.Yellow;

            GHApp.DebugWriteRestart("Garbage Collection Start");
            GHApp.CollectGarbage();
            GHApp.DebugWriteProfilingStopwatchTimeAndStop("Garbage Collection End");

            btnDevGC.Text = "Done";
            btnDevGC.TextColor = GHColors.Red;
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
            FrameTimeProfiler.MarkPauseEvent();
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

        private async void btnDumpFrameLog_Clicked(object sender, EventArgs e)
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

                string filepath = Path.Combine(targetpath, "framelog.csv");
                if (File.Exists(filepath))
                    File.Delete(filepath);

                FrameTimeProfiler.DumpToCsv(filepath);

                if (File.Exists(filepath))
                {
                    await GHApp.ShareFile(this, filepath, "GnollHack Frame Log");
                }
                else
                {
                    await GHApp.DisplayMessageBox(this, "Frame Log File Not Found", "GnollHack could not find " + filepath + ".", "OK");
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                await GHApp.DisplayMessageBox(this, "Error Creating Frame Log", "An error occurred while creating the frame log: " + ex.Message, "OK");
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

        private async void btnOverseer_Clicked(object sender, EventArgs e)
        {
            await OpenOverseerPage();
        }

        private async Task OpenOverseerPage()
        {
            MainLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            if (!GHApp.HasInternetAccess)
            {
                await ShowMessagePopupAsync("Internet Connection Required", "Internet access is required to use Gnoll Overseer.", "OK");
                MainLayout.IsEnabled = true;
                return;
            }

            if (!GHApp.XlogUserNameVerified && !string.IsNullOrEmpty(GHApp.XlogUserName))
            {
                MessagePopup.ShowNonBlockingPopup("Credentials Verification", "Verifying credentials... Please wait.");
                await GHApp.TryVerifyXlogUserNameAsync(true);
                MessagePopup.HideNonBlockingPopup();
            }

            if (string.IsNullOrEmpty(GHApp.XlogUserName) || !GHApp.XlogUserNameVerified)
            {
                await ShowMessagePopupAsync("Verification Required", "Registering a GnollHack Account is required for Gnoll Overseer. Please go to Server Posting section in Settings to set this up.", "OK");
                MainLayout.IsEnabled = true;
                return;
            }

            string snapshotHtml = "";
            string messageHistory = "";
            string directoryManifest = "";

            /* 1. Generate AI snapshot via native call (safe: game thread is idle) */
            try
            {
                string filePath = GHApp.GnollHackService.GenerateAiSnapshot();
                if (filePath != null)
                {
                    if (File.Exists(filePath))
                        snapshotHtml = File.ReadAllText(filePath);
                    else
                        GHApp.WriteGHLog("AI snapshot file " + filePath + " does not exist.");
                }
                else
                {
                    GHApp.WriteGHLog("AI snapshot file is null.");
                }
            }
            catch (Exception ex)
            {
                GHApp.WriteGHLog("AI snapshot generation failed: " + ex.Message);
            }

            /* 2. Collect message history from the current game */
            var currentGame = GHApp.CurrentGHGame;
            if (currentGame != null)
            {
                messageHistory = currentGame.ExportFullMessageHistory();
                if (!(GHApp.DeveloperMode && GHApp.DebugLogMessages) && !string.IsNullOrEmpty(messageHistory) && messageHistory.Length > GHConstants.MaxOverseerLogLengthNonDebug)
                {
                    messageHistory = messageHistory.Substring(messageHistory.Length - GHConstants.MaxOverseerLogLengthNonDebug);
                }
            }

            /* 3. Developer mode extras — only when both flags are on */
            if (GHApp.DeveloperMode && GHApp.DebugLogMessages)
            {
                directoryManifest = GHGame.GenerateDirectoryManifest();
            }

            /* 4. Open OverseerPage — it handles the upload + progress display */
            var overseerPage = new OverseerPage("Gnoll Overseer",
                GHApp.OverseerAddress,
                GHApp.OverseerSendGameContext ? snapshotHtml : "",
                GHApp.OverseerSendGameContext ? messageHistory : "",
                GHApp.OverseerSendGameContext ? directoryManifest : "");
            await GHApp.PushModalPageAsync(overseerPage);

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
                        if (DeveloperPopupGrid.IsVisible)
                        {
                            /* Developer popup is open — handle popup-specific keys */
                            switch (key)
                            {
                                case (int)'o':
                                    if (btnDevOptions.IsEnabled && btnDevOptions.IsVisible)
                                        await CloseAndShowOptions();
                                    handled = true;
                                    break;
                                case (int)'m':
                                    if (btnDevMessages.IsEnabled && btnDevMessages.IsVisible)
                                        btnMessages_Clicked(btnDevMessages, EventArgs.Empty);
                                    handled = true;
                                    break;
                                case (int)'f':
                                    if (btnDevDumpFrameLog.IsEnabled && btnDevDumpFrameLog.IsVisible)
                                        btnDumpFrameLog_Clicked(btnDevDumpFrameLog, EventArgs.Empty);
                                    handled = true;
                                    break;
                                case (int)'g':
                                    if (btnDevGC.IsEnabled && btnDevGC.IsVisible)
                                        btnGC_Clicked(btnDevGC, EventArgs.Empty);
                                    handled = true;
                                    break;
                                case ' ':
                                case 13: /* Enter */
                                    CloseDeveloperPopup();
                                    handled = true;
                                    break;
                                default:
                                    break;
                            }
                        }
                        else
                        {
                            /* Main menu keys */
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
                                case (int)'D':
                                    if (btnDeveloper.IsEnabled && btnDeveloper.IsVisible && MainLayout.IsEnabled)
                                        btnDeveloper_Clicked(btnDeveloper, EventArgs.Empty);
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
                                case (int)'o':
                                    if (btnOverseer.IsEnabled && btnOverseer.IsVisible && MainLayout.IsEnabled)
                                        await OpenOverseerPage();
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
                        if (DeveloperPopupGrid.IsVisible)
                        {
                            /* Developer popup is open — ESC/Enter/Space closes it */
                            if (key == GHSpecialKey.Escape || key == GHSpecialKey.Enter || key == GHSpecialKey.Space)
                            {
                                CloseDeveloperPopup();
                                handled = true;
                            }
                        }
                        else
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

        /* IMessagePopupPage implementation */
        public bool IsPopupOpen => MessagePopup.IsPopupOpen;
        public void ClosePopup() => MessagePopup.ClosePopup();
        public bool SendKeyToPopup(int key, bool isCtrl, bool isMeta) => MessagePopup.SendKeyToPopup(key, isCtrl, isMeta);
        public bool SendSpecialKeyToPopup(GHSpecialKey spkey, bool isCtrl, bool isMeta, bool isShift) => MessagePopup.SendSpecialKeyToPopup(spkey, isCtrl, isMeta, isShift);
#if GNH_MAUI
        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
             Microsoft.Maui.Graphics.Color titleColor = null, bool acceptEnterSpaceForOkCancel = false) => MessagePopup.ShowMessagePopupAsync(title, message, okButtonText, cancelButtonText, titleColor, acceptEnterSpaceForOkCancel);
#else
        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
             Xamarin.Forms.Color? titleColor = null, bool acceptEnterSpaceForOkCancel = false) => MessagePopup.ShowMessagePopupAsync(title, message, okButtonText, cancelButtonText, titleColor, acceptEnterSpaceForOkCancel);
#endif
    }
}
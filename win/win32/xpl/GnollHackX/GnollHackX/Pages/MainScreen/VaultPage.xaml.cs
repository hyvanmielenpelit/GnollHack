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
using Xamarin.Forms.PlatformConfiguration.AndroidSpecific;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class VaultPage : ContentPage, ICloseablePage
    {
        private MainPage _mainPage;
        List<LabeledImageButton> _buttons = new List<LabeledImageButton>();

        public VaultPage(MainPage mainPage)
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

            LabeledImageButton rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.conduct.png";
            rib.ImgHighFilterQuality = true;
            rib.LblText = "Top Scores";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 110;
            rib.ImgHeight = 110;
            rib.GridWidth = 200;
            rib.GridHeight = 140;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.BtnClicked += btnTopScores_Clicked;
            _buttons.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.manual.png";
            rib.ImgHighFilterQuality = true;
            rib.LblText = "Library";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 110;
            rib.ImgHeight = 110;
            rib.GridWidth = 200;
            rib.GridHeight = 140;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.BtnClicked += btnLibrary_Clicked;
            _buttons.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.chronicle.png";
            rib.ImgHighFilterQuality = true;
            rib.LblText = "Replays";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 110;
            rib.ImgHeight = 110;
            rib.GridWidth = 200;
            rib.GridHeight = 140;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.BtnClicked += btnReplays_Clicked;
            _buttons.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.music.png";
            rib.ImgHighFilterQuality = true;
            rib.LblText = "Sound Tracks";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 110;
            rib.ImgHeight = 110;
            rib.GridWidth = 200;
            rib.GridHeight = 140;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.BtnClicked += btnSoundTracks_Clicked;
            _buttons.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.you.png";
            rib.ImgHighFilterQuality = true;
            rib.LblText = "Snapshots";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 110;
            rib.ImgHeight = 110;
            rib.GridWidth = 200;
            rib.GridHeight = 140;
            rib.GridMargin = new Thickness(rib.ImgWidth / 10, 0);
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.BtnClicked += btnSnapshots_Clicked;
            _buttons.Add(rib);

            VaultScrollView.HorizontalScrollBarVisibility =  ScrollBarVisibility.Never;
            VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Never;
            foreach (LabeledImageButton button in _buttons)
                VaultLayout.Children.Add(button);

            _buttonsAdded = true;
            _usingFlex = false;
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
                        if (CloseButton.IsEnabled)
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
            CloseButton.IsEnabled = false;
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
                GHApp.CurrentMainPage?.InvalidateCarousel();
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
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
        private bool _buttonsAdded = false;
        private bool _usingFlex = false;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;

                lblHeader.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);

                if (width > 0)
                {
                    bool shouldUseFlex = width >= 460 && !GHApp.IsWindows;
                    bool widerWidth = height < 280 + 6 + 24 + 60 + lblHeader.Margin.Top + lblHeader.Margin.Bottom + CloseButton.Margin.Top + CloseButton.Margin.Bottom;
                    double usedWidth = widerWidth ? 2048 : 720;
#if GNH_MAUI
                    ViewGrid.MaximumWidthRequest = usedWidth;
#else
                    ViewGrid.WidthRequest = usedWidth;
#endif

                    if (!_buttonsAdded || _usingFlex != shouldUseFlex)
                    {
                        if(_buttonsAdded)
                        {
                            if (_usingFlex)
                                VaultFlexLayout.Children.Clear();
                            else
                                VaultLayout.Children.Clear();
                        }

                        if (shouldUseFlex)
                        {
                            foreach (LabeledImageButton button in _buttons)
                                VaultFlexLayout.Children.Add(button);
                        }
                        else
                        {
                            foreach (LabeledImageButton button in _buttons)
                                VaultLayout.Children.Add(button);
                        }
                        _buttonsAdded = true;
                        _usingFlex = shouldUseFlex;
                        VaultFlexLayout.IsVisible = shouldUseFlex;
                        VaultLayout.IsVisible = !shouldUseFlex;
                    }
                    if(!_usingFlex)
                        VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Default;
                }
            }
        }

        private async void btnTopScores_Clicked(object sender, EventArgs e)
        {
            await OpenTopScorePage();
        }

        private async Task OpenTopScorePage()
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
                    await GHApp.DisplayMessageBox(this, "Reading Top Scores Failed", "GnollHack failed to read the top scores file: " + errormsg, "OK");
                    VaultLayout.IsEnabled = true;
                }
                else
                {
                    await GHApp.Navigation.PushModalAsync(topScorePage);
                }
            }
            else
            {
                /* No top scores */
                var topScorePage = new TopScorePage();
                await GHApp.Navigation.PushModalAsync(topScorePage);
            }
            VaultLayout.IsEnabled = true;
        }

        private async void btnLibrary_Clicked(object sender, EventArgs e)
        {
            await OpenLibraryPage();
        }

        private async Task OpenLibraryPage()
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var libPage = new LibraryPage();
            libPage.ReadLibrary();
            await GHApp.Navigation.PushModalAsync(libPage);
            VaultLayout.IsEnabled = true;
        }

        private async void btnSoundTracks_Clicked(object sender, EventArgs e)
        {
            await OpenMusicPage();
        }

        private async Task OpenMusicPage()
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var musicPage = new MusicPage();
            await GHApp.Navigation.PushModalAsync(musicPage);
            VaultLayout.IsEnabled = true;
        }

        private async void btnSnapshots_Clicked(object sender, EventArgs e)
        {
            await OpenSnapshotPage();
        }

        private async Task OpenSnapshotPage()
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var snapPage = new SnapshotPage();
            snapPage.LoadSnapshots();
            await GHApp.Navigation.PushModalAsync(snapPage);
            VaultLayout.IsEnabled = true;
        }

        private async void btnReplays_Clicked(object sender, EventArgs e)
        {
            await OpenReplayPage();
        }

        private async Task OpenReplayPage()
        {
            VaultLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            ReplayPage selectFilePage = new ReplayPage(_mainPage);
            await GHApp.Navigation.PushModalAsync(selectFilePage);
            VaultLayout.IsEnabled = true;
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
                            case (int)'t':
                            case (int)'T':
                                if (VaultLayout.IsEnabled)
                                    await OpenTopScorePage();
                                handled = true;
                                break;
                            case (int)'l':
                            case (int)'L':
                                if (VaultLayout.IsEnabled)
                                    await OpenLibraryPage();
                                handled = true;
                                break;
                            case (int)'m':
                            case (int)'M':
                                if (VaultLayout.IsEnabled)
                                    await OpenMusicPage();
                                handled = true;
                                break;
                            case (int)'s':
                            case (int)'S':
                                if (VaultLayout.IsEnabled)
                                    await OpenSnapshotPage();
                                handled = true;
                                break;
                            case (int)'r':
                            case (int)'R':
                                if (VaultLayout.IsEnabled)
                                    await OpenReplayPage();
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
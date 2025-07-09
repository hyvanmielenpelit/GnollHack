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
using Microsoft.Maui.Layouts;

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
            Thickness gridMargin = new Thickness(110.0 / 10, 0);
            Thickness margin = new Thickness(110.0 / 8, 0, 110.0 / 8, 110.0 / 8);

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
            rib.GridMargin = gridMargin;
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.Margin = margin;
            rib.HorizontalOptions = LayoutOptions.Center;
            rib.VerticalOptions = LayoutOptions.Center;
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
            rib.GridMargin = gridMargin;
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.Margin = margin;
            rib.HorizontalOptions = LayoutOptions.Center;
            rib.VerticalOptions = LayoutOptions.Center;
            rib.BtnClicked += btnLibrary_Clicked;
            _buttons.Add(rib);

            rib = new LabeledImageButton();
            rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.delphi.png";
            rib.ImgHighFilterQuality = true;
            rib.LblText = "Delphi";
            rib.LblFontSize = 20;
            rib.LblFontColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            rib.LblFontFamily = "Immortal";
            rib.ImgWidth = 110;
            rib.ImgHeight = 110;
            rib.GridWidth = 200;
            rib.GridHeight = 140;
            rib.GridMargin = gridMargin;
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.Margin = margin;
            rib.HorizontalOptions = LayoutOptions.Center;
            rib.VerticalOptions = LayoutOptions.Center;
            rib.BtnClicked += btnOracle_Clicked;
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
            rib.GridMargin = gridMargin;
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.Margin = margin;
            rib.HorizontalOptions = LayoutOptions.Center;
            rib.VerticalOptions = LayoutOptions.Center;
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
            rib.GridMargin = gridMargin;
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.Margin = margin;
            rib.HorizontalOptions = LayoutOptions.Center;
            rib.VerticalOptions = LayoutOptions.Center;
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
            rib.GridMargin = gridMargin;
            rib.WidthRequest = 200 + rib.ImgWidth / 5;
            rib.HeightRequest = 140;
            rib.Margin = margin;
            rib.HorizontalOptions = LayoutOptions.Center;
            rib.VerticalOptions = LayoutOptions.Center;
            rib.BtnClicked += btnSnapshots_Clicked;
            _buttons.Add(rib);
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
            //VaultGrid.IsEnabled = true;
            VaultGrid.IsEnabled = true;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }

        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;
        private bool _buttonsAdded = false;
        //private bool _usingFlex = false;

        private void UpdateVaultGrid(double width, double height)
        {
            bool _currentLandscape = _currentPageWidth > _currentPageHeight;
            bool isLandscape = width > height;

            lblHeader.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
            CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);

            if (width > 0)
            {
                //bool shouldUseFlex = width >= 460 && height >= 600; // && !GHApp.IsWindows;
                //bool widerWidth = height < 280 + 6 + 24 + 60 + lblHeader.Margin.Top + lblHeader.Margin.Bottom + CloseButton.Margin.Top + CloseButton.Margin.Bottom;
                double usedWidth = 720; // widerWidth ? 2048 : 720;
#if GNH_MAUI
                ViewGrid.MaximumWidthRequest = usedWidth;
#else
                    ViewGrid.WidthRequest = usedWidth;
#endif

                //if (!_buttonsAdded || isLandscape != _currentLandscape) //_usingFlex != shouldUseFlex || 
                {
                    if (_buttonsAdded)
                    {
                        VaultGrid.Children.Clear();
                        VaultGrid.ColumnDefinitions.Clear();
                        VaultGrid.RowDefinitions.Clear();

                        //if (_usingFlex)
                        //    VaultFlexLayout.Children.Clear();
                        //else
                        //    VaultLayout.Children.Clear();
                    }

                    double widthAvailableFull = Math.Max(0, usedWidth - 2 * 10);
                    int noOfColumnsFull = Math.Max(1, (int)(widthAvailableFull / 230));
                    int noOfRowsFull = 1 + (_buttons.Count - 1) / noOfColumnsFull;

                    double widthAvailable = Math.Max(0, Math.Min(width, usedWidth) - 2 * 10);
                    double heightAvailable = height - 60 - 80;
                    int noOfColumnsScreen = Math.Max(1, (int)(widthAvailable / 230));
                    int noOfRowsScreen = 1 + (_buttons.Count - 1) / noOfColumnsScreen;
                    double heightTaken = 160 * noOfRowsScreen;

                    int noOfRows = noOfRowsScreen;
                    int noOfColumns = noOfColumnsScreen;
                    if (heightTaken > heightAvailable)
                    {
                        int rowsAvailable = (int)heightAvailable / 160;
                        if (rowsAvailable == 1 && isLandscape) /* Special case scrolling sideways */
                        {
                            noOfRows = 1;
                            noOfColumns = _buttons.Count;
                            VaultGrid.HorizontalOptions = LayoutOptions.Fill;
                            VaultGrid.VerticalOptions = LayoutOptions.Center;
                            VaultScrollView.VerticalOptions = LayoutOptions.Center;
                            VaultScrollView.Orientation = ScrollOrientation.Horizontal;
                            VaultScrollView.HorizontalScrollBarVisibility = ScrollBarVisibility.Default;
                            VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Never;
                        }
                        else /*Normal case scrolling vertically */
                        {
                            VaultGrid.HorizontalOptions = LayoutOptions.Fill;
                            VaultGrid.VerticalOptions = LayoutOptions.Fill;
                            VaultScrollView.VerticalOptions = LayoutOptions.Fill;
                            VaultScrollView.Orientation = ScrollOrientation.Vertical;
                            VaultScrollView.HorizontalScrollBarVisibility = ScrollBarVisibility.Never;
                            VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Default;
                        }
                    }
                    else
                    {
                        VaultGrid.HorizontalOptions = LayoutOptions.Center;
                        VaultGrid.VerticalOptions = LayoutOptions.Center;
                        VaultScrollView.VerticalOptions = LayoutOptions.Fill;
                        VaultScrollView.Orientation = ScrollOrientation.Vertical;
                        VaultScrollView.HorizontalScrollBarVisibility = ScrollBarVisibility.Never;
                        VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Never;
                    }


                    for (int i = 0; i < noOfColumns; i++)
                    {
                        VaultGrid.ColumnDefinitions.Add(new ColumnDefinition());
                    }
                    for (int i = 0; i < noOfRows; i++)
                    {
                        VaultGrid.RowDefinitions.Add(new RowDefinition());
                    }
                    int j = 0;
                    foreach (LabeledImageButton button in _buttons)
                    {
                        int column = j % noOfColumns;
                        int row = j / noOfColumns;
                        VaultGrid.Children.Add(button);
                        Grid.SetColumn(button, column);
                        Grid.SetRow(button, row);
                        j++;
                    }
                    VaultGrid.WidthRequest = noOfColumns * 230;

                    //if (widthAvailable < noOfColumns * 230)
                    //    VaultScrollView.HorizontalScrollBarVisibility = ScrollBarVisibility.Default;
                    //else
                    //    VaultScrollView.HorizontalScrollBarVisibility = ScrollBarVisibility.Never;

                    //if (heightAvailable < noOfRows * 160)
                    //    VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Default;
                    //else
                    //    VaultScrollView.VerticalScrollBarVisibility = ScrollBarVisibility.Never;

                    //if (shouldUseFlex)
                    //{
                    //    foreach (LabeledImageButton button in _buttons)
                    //        VaultFlexLayout.Children.Add(button);
                    //}
                    //else
                    //{
                    //    VaultLayout.Orientation = isLandscape ? StackOrientation.Horizontal : StackOrientation.Vertical;
                    //    VaultScrollView.HorizontalScrollBarVisibility = isLandscape ? ScrollBarVisibility.Default : ScrollBarVisibility.Never;
                    //    VaultScrollView.VerticalScrollBarVisibility = isLandscape ? ScrollBarVisibility.Never : ScrollBarVisibility.Default;
                    //    VaultScrollView.Orientation = isLandscape ? ScrollOrientation.Horizontal : ScrollOrientation.Vertical;
                    //    VaultScrollView.VerticalOptions = isLandscape ? LayoutOptions.Center : LayoutOptions.Fill;
                    //    foreach (LabeledImageButton button in _buttons)
                    //        VaultLayout.Children.Add(button);
                    //}
                    _buttonsAdded = true;
                    //_usingFlex = shouldUseFlex;
                    //VaultFlexLayout.IsVisible = shouldUseFlex;
                    //VaultScrollView.IsVisible = !shouldUseFlex;
                }
            }

        }

        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;
                UpdateVaultGrid(width, height);
            }
        }

        private async void btnTopScores_Clicked(object sender, EventArgs e)
        {
            await OpenTopScorePage();
        }

        private async Task OpenTopScorePage()
        {
            VaultGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            string fulltargetpath = Path.Combine(GHApp.GHPath, "xlogfile");
            if (File.Exists(fulltargetpath))
            {
                var topScorePage = new TopScorePage(fulltargetpath);
                string errormsg = "";
                if (!topScorePage.ReadFile(out errormsg))
                {
                    await GHApp.DisplayMessageBox(this, "Reading Top Scores Failed", "GnollHack failed to read the top scores file: " + errormsg, "OK");
                    VaultGrid.IsEnabled = true;
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
            VaultGrid.IsEnabled = true;
        }

        private async void btnLibrary_Clicked(object sender, EventArgs e)
        {
            await OpenLibraryPage();
        }

        private async Task OpenLibraryPage()
        {
            VaultGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var libPage = new LibraryPage();
            libPage.ReadLibrary();
            await GHApp.Navigation.PushModalAsync(libPage);
            VaultGrid.IsEnabled = true;
        }

        private async void btnOracle_Clicked(object sender, EventArgs e)
        {
            await OpenOraclePage();
        }

        private async Task OpenOraclePage()
        {
            VaultGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var oraclePage = new OraclePage();
            oraclePage.ReadConsultations();
            await GHApp.Navigation.PushModalAsync(oraclePage);
            VaultGrid.IsEnabled = true;
        }

        private async void btnSoundTracks_Clicked(object sender, EventArgs e)
        {
            await OpenMusicPage();
        }

        private async Task OpenMusicPage()
        {
            VaultGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var musicPage = new MusicPage();
            await GHApp.Navigation.PushModalAsync(musicPage);
            VaultGrid.IsEnabled = true;
        }

        private async void btnSnapshots_Clicked(object sender, EventArgs e)
        {
            await OpenSnapshotPage();
        }

        private async Task OpenSnapshotPage()
        {
            VaultGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var snapPage = new SnapshotPage();
            snapPage.LoadSnapshots();
            await GHApp.Navigation.PushModalAsync(snapPage);
            VaultGrid.IsEnabled = true;
        }

        private async void btnReplays_Clicked(object sender, EventArgs e)
        {
            await OpenReplayPage();
        }

        private async Task OpenReplayPage()
        {
            VaultGrid.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            ReplayPage selectFilePage = new ReplayPage(_mainPage);
            await GHApp.Navigation.PushModalAsync(selectFilePage);
            VaultGrid.IsEnabled = true;
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
                                if (VaultGrid.IsEnabled)
                                    await OpenTopScorePage();
                                handled = true;
                                break;
                            case (int)'l':
                            case (int)'L':
                                if (VaultGrid.IsEnabled)
                                    await OpenLibraryPage();
                                handled = true;
                                break;
                            case (int)'m':
                            case (int)'M':
                                if (VaultGrid.IsEnabled)
                                    await OpenMusicPage();
                                handled = true;
                                break;
                            case (int)'s':
                            case (int)'S':
                                if (VaultGrid.IsEnabled)
                                    await OpenSnapshotPage();
                                handled = true;
                                break;
                            case (int)'r':
                            case (int)'R':
                                if (VaultGrid.IsEnabled)
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
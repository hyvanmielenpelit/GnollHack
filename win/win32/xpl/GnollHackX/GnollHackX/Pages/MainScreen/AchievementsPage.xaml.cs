using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
using GnollHackX.Controls;
using Xamarin.Essentials;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AchievementsPage : ContentPage, ICloseablePage
    {
        public AchievementsPage()
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
                lblSubtitle.TextColor = GHColors.White;
                lblSubtitle2.TextColor = GHColors.White;
                EmptyLabel.TextColor = GHColors.White;
#if GNH_MAUI
                TierFrame.Stroke = GHColors.TitleGoldColor;
#else
                TierFrame.BorderColor = GHColors.TitleGoldColor;
#endif
            }
            lblSubtitle.TextColor = GHApp.DarkMode ? GHColors.TitleGoldColor : GHColors.DarkGreen;
            lblSubtitle2.TextColor = GHApp.DarkMode ? GHColors.LightYellow : GHColors.SemiDarkGreen;
            TierTitleLabel.TextColor = GHColors.TitleGoldColor;
            TierSubNameLabel.TextColor = GHColors.LightYellow;
        }
        private async void Button_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync(true);
        }

        private async Task ClosePageAsync(bool playClickedSound)
        {
            CloseButton.IsEnabled = false;
            _backPressed = true;
            if (playClickedSound)
                GHApp.PlayButtonClickedSound();
            await GHApp.PopModalPageAsync();
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
                AchievementLayout.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
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
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }


        public void ReadAchievements()
        {
            AchievementTier tier = GHApp.GetAchievementTier();
            if (tier != null)
            {
                lblSubtitle.Text = tier.Name;
                if (tier.SubName != null)
                {
                    lblSubtitle2.Text = tier.SubName;
                    lblSubtitle2.IsVisible = true;
                }
                if (GHApp.IsNewAchievementTierGained())
                {
                    TierNameLabel.Text = tier.Name;
                    if (tier.SubName != null)
                    {
                        TierSubNameLabel.Text = tier.SubName;
                        TierSubNameLabel.IsVisible = true;
                    }
                    ImageLeft.Source = "resource://" + GHApp.AppResourceName + ".Assets.UI.levelchange.png";
                    ImageRight.Source = "resource://" + GHApp.AppResourceName + ".Assets.UI.levelchange.png";
                    TierGrid.IsVisible = true;
                    GHApp.ClearAchievementTierGained();
                }
            }

            AchievementCategory[] achievementList = GHApp.AchievementCategories;
            //lblSubtitle.Text = "Found " + manuallist.Count + " of " + maxManuals + " manuals";
            if (achievementList.Length > 0)
            {
                for(int i = 0; i < achievementList.Length; i++)
                {
                    AchievementCategory ac = achievementList[i];
                    int numCategoryAchievementsGained, numCategoryAchievementsVisible;
                    GHApp.CalculateAchievementsInCategory(i, out numCategoryAchievementsGained, out numCategoryAchievementsVisible);
                    RowImageButton rib = new RowImageButton();
                    rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.sit.png";
                    rib.ImgHighFilterQuality = true;
                    rib.LblText = ac.Name + " Achievements";
                    rib.LblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.LblFontSize = 17;
                    rib.SubLblText = numCategoryAchievementsGained + " of " + numCategoryAchievementsVisible + " achievement" + (numCategoryAchievementsVisible != 1 ? "s" : "") + " gained";
                    rib.SubLblTextColor = GHApp.DarkMode ? GHColors.LighterGray : GHColors.DarkerGray;
                    rib.SubLblFontSize = 14;
                    rib.IsSubLblVisible = true;
                    rib.ImgWidth = 80;
                    rib.ImgHeight = 80;
                    rib.GridWidth = 480;
                    rib.GridHeight = 80;
#if GNH_MAUI
                    rib.MaximumWidthRequest = 480;
#else
                    rib.WidthRequest = 480;
#endif
                    rib.HeightRequest = 80;
                    rib.GridMargin = new Thickness(rib.ImgWidth / 15, 0);
                    rib.BtnCommand = i;
                    rib.BtnClicked += AchievementButton_Clicked;
                    AchievementLayout.Children.Add(rib);
                }
            }
            else
                EmptyLabel.IsVisible = true;
        }

        private async void AchievementButton_Clicked(object sender, EventArgs e)
        {
            AchievementLayout.IsEnabled = false;
            RowImageButton ghbutton = sender as RowImageButton;
            if (ghbutton != null)
            {
                GHApp.PlayButtonClickedSound();
                var dispAchievementPage = new AchievementsDisplayPage();
                dispAchievementPage.ReadAchievementCategory(ghbutton.BtnCommand);
                await GHApp.PushModalPageAsync(dispAchievementPage);
            }
            AchievementLayout.IsEnabled = true;
        }

        private void TierOkButton_Clicked(object sender, EventArgs e)
        {
            TierOkButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            TierGrid.IsVisible = false;
            TierOkButton.IsEnabled = true;

        }

        private void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            TierOkButton_Clicked(sender, e);
        }
    }
}
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
    public partial class AchievementsDisplayPage : ContentPage, ICloseablePage
    {
        public AchievementsDisplayPage()
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
                EmptyLabel.TextColor = GHColors.White;
            }
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

        public void ReadAchievementCategory(int categoryId)
        {
            if (categoryId < 0 || categoryId >= (int)gui_achievement_categories.NumberOfCategories)
                return;

            int gained, visible;
            GHApp.CalculateAchievementsInCategory(categoryId, out gained, out visible);
            lblHeader.Text = GHApp.AchievementCategories[categoryId].Name;
            lblSubtitle.Text = "Gained " + gained + " of " + visible + " achievement" + (visible != 1 ? "s" : "");
            
            Achievement[] achievementList = GHApp.AchievementDefinitions;
            if (achievementList.Length > 0)
            {
                for (int i = 0; i < achievementList.Length; i++)
                {
                    if (achievementList[i] == null)
                        continue;
                    if (categoryId >= 0 && achievementList[i].CategoryId != categoryId)
                        continue;
                    bool isAchieved = GHApp.IsAchievementGained(i);
                    if (!achievementList[i].IsVisible && !isAchieved)
                        continue;

                    Achievement achievement = achievementList[i];
                    bool isKnown = achievementList[i].IsKnown || isAchieved;
                    bool isLocked = achievementList[i].IsLocked;
                    RowImageButton rib = new RowImageButton();
                    rib.ImgSourcePath = "resource://" + GHApp.AppResourceName 
                        + ".Assets.UI." 
                        + (isLocked ? "forcelock" : isAchieved ? "conduct" : !isKnown ? "help" : "loot") 
                        + ".png";
                    rib.ImgHighFilterQuality = true;
                    rib.LblText = isLocked ? "Locked" : !isKnown && !isAchieved ? "Unknown" : achievement.Name;
                    rib.LblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.LblFontSize = 17;
                    rib.SubLblText = achievement.Description;
                    rib.SubLblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.SubLblFontSize = 14;
                    rib.IsSubLblVisible = isKnown && !isLocked;
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
                    //rib.BtnClicked += AchievementButton_Clicked;
                    AchievementLayout.Children.Add(rib);
                }
            }
            else
                EmptyLabel.IsVisible = true;

        }
    }
}
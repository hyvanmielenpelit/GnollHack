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


        Dictionary<int, AchievementCategory> _achievementCategories = new Dictionary<int, AchievementCategory>()
        {
            { 0, new AchievementCategory("Gameplay Achievements", 0) },
            { 1, new AchievementCategory("Exploration Achievements", 1) },
            { 2, new AchievementCategory("Playthrough Achievements", 2) },
            { 3, new AchievementCategory("Ascension Achievements", 3) },
        };

        public void ReadAchievements()
        {
            List<AchievementCategory> achievementList = _achievementCategories.Values.ToList();
            //lblSubtitle.Text = "Found " + manuallist.Count + " of " + maxManuals + " manuals";
            if (achievementList.Count > 0)
            {
                foreach (AchievementCategory ac in achievementList)
                {
                    int numCategoryAchievements = 0;
                    RowImageButton rib = new RowImageButton();
                    rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.sit.png";
                    rib.ImgHighFilterQuality = true;
                    rib.LblText = ac.Name;
                    rib.LblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.LblFontSize = 17;
                    rib.SubLblText = numCategoryAchievements + " achievement" + (numCategoryAchievements != 1 ? "s" : "");
                    rib.SubLblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
                    rib.BtnCommand = ac.Id;
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
                int categoryId = ghbutton.BtnCommand;
                var dispfilepage = new AchievementsDisplayPage();
                await GHApp.PushModalPageAsync(dispfilepage);
            }
            AchievementLayout.IsEnabled = true;
        }
    }
}
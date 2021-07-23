using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GnollHackCommon;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class CommandPage : ContentPage
    {
        private GamePage _gamePage;
        public CommandPage(GamePage gamePage)
        {
            InitializeComponent();
            _gamePage = gamePage;
            if (_gamePage.EnableWizardMode)
                WizCmdRow.IsVisible = true;
            else
                WizCmdRow.IsVisible = false;
        }

        private async void DownButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '>');
        }

        private async void UpButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '<');
        }

        private async void CmdButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('c'));
        }

        private async void SkillButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'S');
        }

        private async void AbilityButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'A');
        }

        private async void EatButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'e');
        }

        private async void LootButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'l');
        }

        private async void OpenButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'o');
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'c');
        }

        private async void YellButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('y'));
        }

        private async void ChatButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'C');
        }

        private async void TwoWeapButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('x'));
        }

        private async void SwapWeapButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'x');
        }

        private async void SpellsButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '+');
        }

        private async void PrayButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('p'));
        }

        private async void SacrificeButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('o'));
        }
        private async void NameButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('N'));
        }
        private async void QuaffButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'q');
        }
        private async void PayButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'p');
        }
        private async void DigButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('g'));
        }
        private async void ForceFightButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'F');
        }
        private async void LightButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('l'));
        }
        private async void TravelButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '_');
        }
        private async void ExtendedCommandButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '#');
        }

        private async void BackButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void WhatIsButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '/');
        }

        private async void LookFarButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, ';');
        }

        private async void HelpButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, '?');
        }

        private async void EngraveButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'E');
        }

        private async void DropManyButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'D');
        }

        private async void CountButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'n');
            _gamePage.ShowNumberPad();
        }

        private async void Search20Button_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'n');
            _gamePage.GenericButton_Clicked(sender, e, -12);
            _gamePage.GenericButton_Clicked(sender, e, -10);
            _gamePage.GenericButton_Clicked(sender, e, 's');
        }

        private async void Search200Button_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'n');
            _gamePage.GenericButton_Clicked(sender, e, -12);
            _gamePage.GenericButton_Clicked(sender, e, -10);
            _gamePage.GenericButton_Clicked(sender, e, -10);
            _gamePage.GenericButton_Clicked(sender, e, 's');
        }

        private async void WizWishButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('w'));

        }

        private async void WizMapButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('f'));
        }

        private async void WizGenesisButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('m'));
        }

        private async void WizLevelportButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('v'));
        }

        private async void WizTeleportButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('t'));
        }

        private async void OverviewButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('o'));
        }

        private async void WizIdentifyButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Ctrl('i'));
        }

        private void ContentPage_SizeChanged(object sender, EventArgs e)
        {

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
                StackLayout firstChild = (StackLayout)MainLayout.Children[0];
                int numcols = firstChild.Children.Count;
                int numrows = MainLayout.Children.Count - (WizCmdRow.IsVisible ? 0 : 1);
                if (width > height)
                {
                    /* Landscape */
                    double btnwidth = ((width - MainLayout.Padding.Left - MainLayout.Padding.Right - MainLayout.Margin.Left - MainLayout.Margin.Right) / numrows) - firstChild.Padding.Left - firstChild.Padding.Right - firstChild.Margin.Left - firstChild.Margin.Right - MainLayout.Spacing;
                    double btnheight = ((height - MainLayout.Padding.Top - MainLayout.Padding.Bottom - MainLayout.Margin.Top - MainLayout.Margin.Bottom) / numcols) - firstChild.Padding.Top - firstChild.Padding.Bottom - firstChild.Margin.Top - firstChild.Margin.Bottom - firstChild.Spacing;
                    MainLayout.Orientation = StackOrientation.Horizontal;
                    foreach (View col in MainLayout.Children)
                    {
                        StackLayout collayout = (StackLayout)col;
                        collayout.Orientation = StackOrientation.Vertical;
                        foreach (View btn in collayout.Children)
                        {
                            btn.WidthRequest = btnwidth;
                            btn.HeightRequest = btnheight;
                        }
                    }
                }
                else
                {
                    /* Portrait */
                    double btnwidth = ((width - MainLayout.Padding.Left - MainLayout.Padding.Right - MainLayout.Margin.Left - MainLayout.Margin.Right) / numcols) - firstChild.Padding.Left- firstChild.Padding.Right - firstChild.Margin.Left - firstChild.Margin.Right - firstChild.Spacing;
                    double btnheight = ((height- MainLayout.Padding.Top - MainLayout.Padding.Bottom - MainLayout.Margin.Top - MainLayout.Margin.Bottom) / numrows) - firstChild.Padding.Top - firstChild.Padding.Bottom - firstChild.Margin.Top - firstChild.Margin.Bottom - MainLayout.Spacing;
                    MainLayout.Orientation = StackOrientation.Vertical;
                    foreach (View col in MainLayout.Children)
                    {
                        StackLayout collayout = (StackLayout)col;
                        collayout.Orientation = StackOrientation.Horizontal;
                        foreach (View btn in collayout.Children)
                        {
                            btn.WidthRequest = btnwidth;
                            btn.HeightRequest = btnheight;
                        }
                    }
                }

            }
        }

        private async void PopupMenuBackButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.ShowGameMenu(sender, e);
        }

        private async void WieldButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'w');
        }

        private async void WearButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'W');
        }

        private async void PutOnButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'P');
        }

        private async void TakeOffButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'T');
        }
    }
}
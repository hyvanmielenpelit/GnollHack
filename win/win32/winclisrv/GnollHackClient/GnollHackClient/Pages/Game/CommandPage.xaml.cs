using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GnollHackClient.Controls;
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

        private async void BackButton_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void CountButton_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'n');
            _gamePage.DoShowNumberPad();
        }

        private async void Search20Button_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'n');
            _gamePage.GenericButton_Clicked(sender, e, -12);
            _gamePage.GenericButton_Clicked(sender, e, -10);
            _gamePage.GenericButton_Clicked(sender, e, 's');
        }

        private async void Search200Button_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 'n');
            _gamePage.GenericButton_Clicked(sender, e, -12);
            _gamePage.GenericButton_Clicked(sender, e, -10);
            _gamePage.GenericButton_Clicked(sender, e, -10);
            _gamePage.GenericButton_Clicked(sender, e, 's');
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
                foreach(View s in MainLayout.Children)
                {
                    StackLayout sl = s as StackLayout;
                    if(sl != null)
                    {
                        foreach (View v in sl.Children)
                        {
                            Grid g = v as Grid;
                            if (g != null && g.Children.Count > 0)
                            {
                                LabeledImageButton lib = g.Children[0] as LabeledImageButton;
                                if (lib != null)
                                {
                                    lib.SetSideSize(width, height);
                                }
                            }
                        }
                    }
                }
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
            MainLayout.IsEnabled = false;
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.ShowGameMenu(sender, e);
        }

        private async void ESCButton_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, 27);
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private async void GHButton_Clicked(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = false;
            LabeledImageButton btn = (LabeledImageButton)sender;
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, btn.GHCommand);
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            MainLayout.IsEnabled = true;
            App.BackButtonPressed += BackButtonPressed;

        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

    }
}
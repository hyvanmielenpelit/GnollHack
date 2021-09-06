using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class StorePage : ContentPage
    {
        MainPage _mainPage = null;
        public StorePage(MainPage mainPage)
        {
            InitializeComponent();
            _mainPage = mainPage;
        }

        void UpdateExtraLives()
        {
            if(StoreTableView.IsEnabled == false)
                ExtraLifeLabel.Text = "(Please wait...)";
            else
                ExtraLifeLabel.Text = "Extra Lives: " + App.ExtraLives;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
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

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
            StoreTableView.IsEnabled = true;
            UpdateExtraLives();
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
        }

        private async void btnExtraLife_Clicked(object sender, EventArgs e)
        {
            StoreTableView.IsEnabled = false;
            UpdateExtraLives();
            await _mainPage.PurchaseExtraLife();
            StoreTableView.IsEnabled = true;
            UpdateExtraLives();
        }

        private async void btn3ExtraLives_Clicked(object sender, EventArgs e)
        {
            StoreTableView.IsEnabled = false;
            UpdateExtraLives();
            await _mainPage.Purchase3ExtraLives();
            StoreTableView.IsEnabled = true;
            UpdateExtraLives();
        }
    }
}
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
    public partial class GameMenuPage : ContentPage
    {
        private GamePage _gamePage;

        public GameMenuPage(GamePage gamePage)
        {
            InitializeComponent();

            _gamePage = gamePage;
        }

        private void btnSave_Clicked(object sender, EventArgs e)
        {
            DoQuit();
        }

        private void btnQuit_Clicked(object sender, EventArgs e)
        {
            DoQuit();
        }

        private async void btnBackToGame_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void DoQuit()
        {
            _gamePage.FModService.StopTestSound();
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game Menu
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game
            if (App.IsServerGame)
            {
                await App.Current.MainPage.Navigation.PopAsync(); //Login
            }
        }
    }
}
﻿using GnollHackCommon;
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

        private async void btnSave_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('s'));
        }

        private async void btnQuit_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
            _gamePage.GenericButton_Clicked(sender, e, GHUtils.Meta('q'));
        }

        private async void btnBackToGame_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void DoQuit()
        {
            //App.FmodService.StopTestSound();
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game Menu
            await App.Current.MainPage.Navigation.PopModalAsync(); // Game
            if (App.IsServerGame)
            {
                await App.Current.MainPage.Navigation.PopAsync(); //Login
            }
        }

        private async void btnOptions_Clicked(object sender, EventArgs e)
        {
            _gamePage.GenericButton_Clicked(sender, e, 'O');
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void btnSettings_Clicked(object sender, EventArgs e)
        {
            var settingsPage = new SettingsPage(this._gamePage);
            await App.Current.MainPage.Navigation.PushModalAsync(settingsPage);
        }
    }
}
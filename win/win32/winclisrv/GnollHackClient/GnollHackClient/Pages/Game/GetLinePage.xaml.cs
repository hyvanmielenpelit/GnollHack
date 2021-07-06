using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GetLinePage : ContentPage
    {
        private GamePage _gamePage;
        private ClientGame _clientGame;
        private string _result = "";
        public GetLinePage(GamePage gamePage, string query)
        {
            InitializeComponent();
            _gamePage = gamePage;
            _clientGame = gamePage.ClientGame;
            labelCaption.Text = query;
        }

        private async void btnOK_Clicked(object sender, EventArgs e)
        {
            string res = entryText.Text;
            if (string.IsNullOrWhiteSpace(entryText.Text))
            {
                res = "";
            }
            _result = res;
            await _gamePage.Navigation.PopModalAsync();
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();

            entryText.Focus();
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_clientGame, GHRequestType.GetLine, _result));
            }

        }
    }
}
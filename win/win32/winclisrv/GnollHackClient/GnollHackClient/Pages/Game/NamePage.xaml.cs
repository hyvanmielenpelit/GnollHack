using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class NamePage : ContentPage
    {
        public Regex ValidationExpression { get; set; }
        private ClientGame _clientGame;
        private GamePage _gamePage;

        public NamePage(GamePage gamepage)
        {
            InitializeComponent();
            ValidationExpression = new Regex(@"^[A-Za-z0-9_]{1,32}$");
            _clientGame = gamepage.ClientGame;
            _gamePage = gamepage;
        }

        private async void btnOK_Clicked(object sender, EventArgs e)
        {
            if(string.IsNullOrWhiteSpace(eName.Text))
            {
                lblError.TextColor = Color.Red;
                lblError.Text = "Please enter a name.";
                return;
            }

            if (!ValidationExpression.IsMatch(eName.Text))
            {
                lblError.TextColor = Color.Red;
                lblError.Text = "Name is invalid.";
                return;
            }

            //_clientGame.CharacterName = eName.Text;
            App.IsServerGame = false;
            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_clientGame, GHRequestType.AskName, eName.Text));
                await _gamePage.Navigation.PopModalAsync();
            }
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();

            eName.Focus();
        }
    }
}
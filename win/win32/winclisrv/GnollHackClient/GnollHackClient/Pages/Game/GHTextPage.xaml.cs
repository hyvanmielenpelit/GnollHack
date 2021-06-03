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
    public partial class GHTextPage : ContentPage
    {
        private List<GHPutStrItem> _putStrItems = new List<GHPutStrItem>();

        public List<GHPutStrItem> PutStrItems { get; }
        private GamePage _gamePage;
        private ClientGame _clientGame;

        public GHTextPage(GamePage gamePage, List<GHPutStrItem> list)
        {
            InitializeComponent();
            _gamePage = gamePage;
            _clientGame = _gamePage.ClientGame;
            TextView.ItemsSource = list;
        }

        private void GHTextPage_Disappearing(object sender, EventArgs e)
        {
            _gamePage.GenericButton_Clicked(sender, e, 27);
        }

        private async void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }
    }
}

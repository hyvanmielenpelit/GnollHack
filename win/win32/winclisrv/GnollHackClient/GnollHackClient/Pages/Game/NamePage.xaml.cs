using System;
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

        public NamePage()
        {
            InitializeComponent();
            ValidationExpression = new Regex(@"^[A-Za-z0-9_]{1,32}$");
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

            App.UserName = eName.Text;
            App.IsServerGame = false;

            await App.Current.MainPage.Navigation.PopModalAsync();

            var gamePage = new GamePage();
            await App.Current.MainPage.Navigation.PushModalAsync(gamePage);
        }
    }
}
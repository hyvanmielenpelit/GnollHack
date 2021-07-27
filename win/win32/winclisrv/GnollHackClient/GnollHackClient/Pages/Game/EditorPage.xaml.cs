using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class EditorPage : ContentPage
    {
        private string _fileName;

        public EditorPage(string fileName, string header)
        {
            InitializeComponent();
            _fileName = fileName;
            HeaderLabel.Text = header;
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            OKButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            CancelButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
        }

        public bool ReadFile(out string errorMessage)
        {
            string res = "";
            try
            {
                TextEditor.Text = File.ReadAllText(_fileName);
            }
            catch (Exception e)
            {
                errorMessage = e.Message;
                return false;
            }
            errorMessage = res;
            return true;
        }

        private async void OKButton_Clicked(object sender, EventArgs e)
        {
            try
            {
                File.WriteAllText(_fileName, TextEditor.Text);
            }
            catch (Exception ex)
            {
                ErrorLabel.Text = ex.Message;
                return;
            }
            ErrorLabel.Text = "";
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private async void CancelButton_Clicked(object sender, EventArgs e)
        {
            ErrorLabel.Text = "";
            await App.Current.MainPage.Navigation.PopModalAsync();
        }
    }
}
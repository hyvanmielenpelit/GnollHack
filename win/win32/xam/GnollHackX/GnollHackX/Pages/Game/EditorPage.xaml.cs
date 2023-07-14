using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class EditorPage : ContentPage
    {
        private string _fileName;

        public EditorPage(string fileName, string header)
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);

            _fileName = fileName;
            HeaderLabel.Text = header;
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            //OKButtonImage.Source = ImageSource.FromResource("GnollHackX.Assets.button_normal.png", assembly);
            //CancelButtonImage.Source = ImageSource.FromResource("GnollHackX.Assets.button_normal.png", assembly);
            //ResetButtonImage.Source = ImageSource.FromResource("GnollHackX.Assets.button_normal.png", assembly);
        }

        public bool ReadFile(out string errorMessage)
        {
            string res = "";
            TextEditor.Text = "(Reading file)";
            try
            {
                TextEditor.Text = File.ReadAllText(_fileName, Encoding.UTF8);
                TextEditor.IsEnabled = true;
            }
            catch (Exception e)
            {
                TextEditor.Text = "";
                errorMessage = e.Message;
                return false;
            }
            errorMessage = res;
            _registerChanges = true;
 
            return true;
        }

        private async void OKButton_Clicked(object sender, EventArgs e)
        {
            OKButton.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (_textChanged)
            {
                bool answer = await DisplayAlert("Save Changes?", "Are you sure to save changes to the options file?", "Yes", "No");
                if (answer)
                {
                    try
                    {
                        File.WriteAllText(_fileName, TextEditor.Text, Encoding.UTF8);
                    }
                    catch (Exception ex)
                    {
                        ErrorLabel.Text = ex.Message;
                        OKButton.IsEnabled = true;
                        return;
                    }
                    ErrorLabel.Text = "";
                    await App.Current.MainPage.Navigation.PopModalAsync();
                }
                else
                {
                    OKButton.IsEnabled = true;
                }
            }
            else
            {
                ErrorLabel.Text = "";
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
        }

        private async void CancelButton_Clicked(object sender, EventArgs e)
        {
            CancelButton.IsEnabled = false;
            App.PlayButtonClickedSound();
            if (_textChanged)
            {
                bool answer = await DisplayAlert("Close without Saving?", "Are you sure to close without saving changes?", "Yes", "No");
                if (answer)
                {
                    ErrorLabel.Text = "";
                    await App.Current.MainPage.Navigation.PopModalAsync();
                }
                else 
                {
                    CancelButton.IsEnabled = true;
                }
            }
            else
            {
                ErrorLabel.Text = "";
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
        }

        private async void ResetButton_Clicked(object sender, EventArgs e)
        {
            ResetButton.IsEnabled = false;
            App.PlayButtonClickedSound();
            bool answer = await DisplayAlert("Reset Options File?", "Are you sure to reset the options file?", "Yes", "No");
            if(answer)
            {
                ErrorLabel.Text = "";
                App.GnollHackService.ResetDefaultsFile();
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            else
            {
                ResetButton.IsEnabled = true;
            }
        }

        private bool _registerChanges = false;
        private bool _textChanged = false;
        private void TextEditor_TextChanged(object sender, TextChangedEventArgs e)
        {
            if(_registerChanges)
                _textChanged = true;
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

                HeaderLabel.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseGrid.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
                double bordermargin = ClientUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                TextEditor.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
            }
        }

    }
}
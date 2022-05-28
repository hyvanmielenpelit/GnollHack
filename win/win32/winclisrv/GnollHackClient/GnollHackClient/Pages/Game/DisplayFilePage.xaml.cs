using SkiaSharp;
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
    public partial class DisplayFilePage : ContentPage
    {
        public DisplayFilePage(string fileName, string header)
        {
            InitializeComponent();
            _fileName = fileName;
            HeaderLabel.Text = header;
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            CloseButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private string _fileName;

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
            return true;
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

                double bordermargin = ClientUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                MainGrid.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
                double target_width = (Math.Min(width, MainGrid.WidthRequest) - MainGrid.Margin.Left - MainGrid.Margin.Right 
                    - MainGrid.Padding.Left - MainGrid.Padding.Right - TextEditor.Margin.Left - TextEditor.Margin.Right);
                double newsize = 12 * target_width / 640;
                TextEditor.FontSize = newsize;

                HeaderLabel.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseGrid.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);

            }
        }

    }
}
using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class DisplayFilePage : ContentPage
    {
        private string _fileName;
        private int _fixedWidth;

        public DisplayFilePage(string fileName, string header) : this(fileName, header, 0)
        {
        }

        public DisplayFilePage(string fileName, string header, int fixedWidth) : this(fileName, header, fixedWidth, false)
        {
        }

        public DisplayFilePage(string fileName, string header, int fixedWidth, bool displayshare)
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);

            _fileName = fileName;
            _fixedWidth = fixedWidth;
            HeaderLabel.Text = header;
            BottomLayout.IsVisible = displayshare;
            CloseGrid.IsVisible = !displayshare;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        public bool ReadFile(out string errorMessage)
        {
            string res = "";
            try
            {
                string text = File.ReadAllText(_fileName, Encoding.UTF8);
                if(_fixedWidth > 0)
                {
                    int firstLineBreak = text.IndexOf(Environment.NewLine);
                    int len = 0;
                    if (firstLineBreak < 0)
                    {
                        len = text.Length;
                        if (len < _fixedWidth)
                        {
                            text = text + new string(' ', _fixedWidth - len);
                        }
                    }
                    else
                    {
                        len = firstLineBreak;
                        if (len < _fixedWidth)
                        {
                            text = text.Substring(0, firstLineBreak) + new string(' ', _fixedWidth - len) + text.Substring(firstLineBreak);
                        }
                    }

                }
                TextLabel.Text = text;
            }
            catch (Exception e)
            {
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
                Thickness margins = new Thickness();
                margins = TextLabel.Margin;

                double bordermargin = ClientUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                MainGrid.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
                double target_width = (Math.Min(width, MainGrid.WidthRequest) - MainGrid.Margin.Left - MainGrid.Margin.Right 
                    - MainGrid.Padding.Left - MainGrid.Padding.Right - margins.Left - margins.Right);
                double newsize = 12.5 * target_width / 640;

                TextLabel.FontSize = newsize;

                HeaderLabel.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                BottomLayout.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseGrid.Margin = BottomLayout.Margin;
            }
        }

        private async void ShareButton_Clicked(object sender, EventArgs e)
        {
            ShareGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            try
            {
                await Share.RequestAsync(new ShareFileRequest
                {
                    Title = HeaderLabel.Text,
                    File = new ShareFile(_fileName)
                });
            }
            catch (Exception ex)
            {
                await DisplayAlert("Share File Failure", "GnollHack failed to share " + HeaderLabel.Text + ": " + ex.Message, "OK");
            }
            ShareGrid.IsEnabled = true;
        }
    }
}
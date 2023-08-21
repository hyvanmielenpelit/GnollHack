using SkiaSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class DisplayFilePage : ContentPage
    {
        private string _fileName;
        private int _fixedWidth;
        private bool _isHtml;

        public DisplayFilePage(string fileName, string header) : this(fileName, header, 0)
        {
        }

        public DisplayFilePage(string fileName, string header, int fixedWidth) : this(fileName, header, fixedWidth, false, false)
        {
        }

        public DisplayFilePage(string fileName, string header, int fixedWidth, bool displayshare) : this(fileName, header, fixedWidth, displayshare, false)
        {

        }

        public string OverridingText { get; set; } = null;
        public bool UseFixedFontSize { get; set; }
        public double FontSize { get; set; } = 13.0;

        public DisplayFilePage(string fileName, string header, int fixedWidth, bool displayshare, bool isHtml)
        {
            InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(true);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif

            _fileName = fileName;
            _fixedWidth = fixedWidth;
            HeaderLabel.Text = header;
            BottomLayout.IsVisible = displayshare;
            CloseButton.IsVisible = !displayshare;
            _isHtml = isHtml;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        public bool ReadFile(out string errorMessage)
        {
            string res = "";
            try
            {
                if (_isHtml)
                {
                    string text = OverridingText != null ? OverridingText : File.ReadAllText(_fileName, Encoding.UTF8);
                    var htmlSource = new HtmlWebViewSource();
                    htmlSource.Html = text;
                    htmlSource.BaseUrl = GHApp.PlatformService.GetBaseUrl();
                    DisplayWebView.Source = htmlSource;
                    if(GHApp.IsiOS)
                        DisplayWebView.Opacity = 0;
                    DisplayWebView.IsVisible = true;
                }
                else
                {
                    string text = OverridingText != null ? OverridingText : File.ReadAllText(_fileName, Encoding.UTF8);
                    if (_fixedWidth > 0)
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
                    TextLabel.IsVisible = true;
                }
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

                if (UseFixedFontSize)
                {
                    TextLabel.FontSize = FontSize;
                    TextLabel.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                }
                else
                {
                    Thickness safearea = new Thickness();
#if GNH_MAUI
                    bool usingsafe = On<iOS>().UsingSafeArea();
#else
                    bool usingsafe = On<Xamarin.Forms.PlatformConfiguration.iOS>().UsingSafeArea();
#endif
                    if (usingsafe)
                    {
#if GNH_MAUI
                        safearea = On<iOS>().SafeAreaInsets();
#else
                        safearea = On<Xamarin.Forms.PlatformConfiguration.iOS>().SafeAreaInsets();
#endif
                    }
                    Thickness usedpadding = usingsafe ? safearea : MainGrid.Padding;
                    double bordermargin = UIUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                    MainGrid.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
                    double limited_width = Math.Min(Math.Min(width, MainGrid.WidthRequest), DeviceDisplay.MainDisplayInfo.Width);
                    double target_width = limited_width - MainGrid.Margin.Left - MainGrid.Margin.Right
                        - usedpadding.Left - usedpadding.Right - margins.Left - margins.Right;
                    double testsize = 12.5;
                    double newsize = testsize * target_width / 640;
                    if (_fixedWidth > 0)
                    {
                        TextLabel.FontSize = testsize;
                        double textwidth = TextLabel.MeasureWidth(new string('A', _fixedWidth));
                        if (textwidth > 0)
                            newsize = testsize * target_width / textwidth;
                    }
                    TextLabel.FontSize = newsize;
                    TextLabel.Margin = new Thickness();
                }
                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                BottomLayout.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = BottomLayout.Margin;
            }
        }

        private async void ShareButton_Clicked(object sender, EventArgs e)
        {
            ShareButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
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
            ShareButton.IsEnabled = true;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            if (GHApp.IsiOS && _isHtml)
            {
                Device.StartTimer(TimeSpan.FromSeconds(1.0 / 20), () =>
                {
                    Animation displayFileAnimation = new Animation(v => DisplayWebView.Opacity = (double)v, 0.0, 1.0);
                        displayFileAnimation.Commit(MainGrid, "DisplayFileShowAnimation", length: 256,
                    rate: 16, repeat: () => false);
                    return false;
                });
            }
        }
    }
 }
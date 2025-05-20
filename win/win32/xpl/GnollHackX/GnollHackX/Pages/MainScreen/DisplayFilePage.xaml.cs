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
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class DisplayFilePage : ContentPage, ICloseablePage
    {
        private string _fileName;
        private int _fixedWidth;
        private bool _isHtml;

        public DisplayFilePage(string fileName, string header) : this(fileName, header, 0)
        {
        }

        public DisplayFilePage(string fileName, string header, int fixedWidth) : this(fileName, header, fixedWidth, false, false, false)
        {
        }

        public DisplayFilePage(string fileName, string header, int fixedWidth, bool displayshare) : this(fileName, header, fixedWidth, displayshare, false, false)
        {

        }

        public string OverridingText { get; set; } = null;
        public bool UseFixedFontSize { get; set; }
        public double FontSize { get; set; } = 13.0;

        public DisplayFilePage(string fileName, string header, int fixedWidth, bool displayshare, bool isHtml, bool isScrolledDown)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _fileName = fileName;
            _fixedWidth = fixedWidth;
            HeaderLabel.Text = header;
            BottomLayout.IsVisible = displayshare;
            CloseButton.IsVisible = !displayshare;
            _isHtml = isHtml;
            TextLabel.InitiallyRolledDown = isScrolledDown;
            if(GHApp.DarkMode)
            {
                HeaderLabel.TextColor = GHColors.White;
                TextLabel.TextColor = GHColors.White;
            }
            if (GHApp.IsiOS && _isHtml)
                DisplayWebView.Opacity = 0.0;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync();
        }

        private async Task ClosePageAsync()
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            var page = await GHApp.Navigation.PopModalAsync();
            GHApp.DisconnectIViewHandlers(page);
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (CloseButton.IsEnabled)
                            await ClosePageAsync();
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }

                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
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
                    double limited_width = Math.Min(Math.Min(width,
#if GNH_MAUI
                        MainGrid.MaximumWidthRequest
#else
                        MainGrid.WidthRequest
#endif
                        ), DeviceDisplay.MainDisplayInfo.Width);
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
                await GHApp.DisplayMessageBox(this, "Share File Failure", "GnollHack failed to share " + HeaderLabel.Text + ": " + ex.Message, "OK");
            }
            ShareButton.IsEnabled = true;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            if (GHApp.IsiOS && _isHtml)
            {
#if GNH_MAUI
                var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                timer.Interval = TimeSpan.FromSeconds(1.0 / 2);
                timer.IsRepeating = false;
                timer.Tick += (s, e) => { DoiOSShowGrid(); };
                timer.Start();
#else
                Device.StartTimer(TimeSpan.FromSeconds(1.0 / 20), () =>
                {
                    DoiOSShowGrid();
                    return false;
                });
#endif
            }
        }

        private void DoiOSShowGrid()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        await DisplayWebView.FadeTo(1.0, 512);
                        //Animation displayFileAnimation = new Animation(v => DisplayWebView.Opacity = (double)v, 0.01, 1.00);
                        //displayFileAnimation.Commit(MainGrid, "DisplayFileShowAnimation", length: 512, rate: 16, repeat: () => false);
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }
    }
 }
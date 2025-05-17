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
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using Xamarin.Essentials;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class EditorPage : ContentPage
    {
        private string _fileName;
        private MainPage _mainPage;
        public EditorPage(MainPage mainPage, string fileName, string header)
        {
            _mainPage = mainPage;
            Disappearing += (s, e) => { _mainPage.StartCarouselViewAndEnableButtons(); };

            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _fileName = fileName;
            HeaderLabel.Text = header;
            if(GHApp.DarkMode)
            {
                HeaderLabel.TextColor = GHColors.White;
                TextEditor.TextColor = GHColors.White;
            }
        }

        public void ReadFile()
        {
            TextEditor.Text = "(Reading file)";
            string str = File.ReadAllText(_fileName, Encoding.UTF8);
            TextEditor.Text = str; //Note that in UWP on Windows each Environment.NewLine gets here changed into \r only (!)
            TextEditor.IsEnabled = true;
            _registerChanges = true;
        }

        public void ClearTextEditor()
        {
            TextEditor.Text = "";
        }

        private async void OKButton_Clicked(object sender, EventArgs e)
        {
            OKButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if (_textChanged)
            {
                bool answer = await GHApp.DisplayMessageBox(this, "Save Changes?", "Are you sure to save changes to the options file?", "Yes", "No");
                if (answer)
                {
                    try
                    {
#if WINDOWS
                        // In UWP on Windows line endings are just \r so they need to be changed back to Environment.NewLine
                        string str = TextEditor.Text.Replace("\r", Environment.NewLine);
#else
                        string str = TextEditor.Text;
#endif
                        byte[] data = Encoding.UTF8.GetBytes(str);
                        File.WriteAllBytes(_fileName, data);
                        //File.WriteAllText(_fileName, str4, Encoding.UTF8); //WriteAllText seems to add 3 bytes in the front of the file on Windows, so cannot be used
                    }
                    catch (Exception ex)
                    {
                        ErrorLabel.Text = ex.Message;
                        OKButton.IsEnabled = true;
                        return;
                    }
                    ErrorLabel.Text = "";
                    GHApp.CurrentMainPage?.InvalidateCarousel();
                    var page = await GHApp.Navigation.PopModalAsync();
                    GHApp.DisconnectIViewHandlers(page);
                }
                else
                {
                    OKButton.IsEnabled = true;
                }
            }
            else
            {
                ErrorLabel.Text = "";
                GHApp.CurrentMainPage?.InvalidateCarousel();
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
            }
        }

        private async void CancelButton_Clicked(object sender, EventArgs e)
        {
            await CloseCore();
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (CancelButton.IsEnabled)
                            await CloseCore();
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

        private async Task CloseCore()
        {
            CancelButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if (_textChanged)
            {
                bool answer = await GHApp.DisplayMessageBox(this, "Close without Saving?", "Are you sure to close without saving changes?", "Yes", "No");
                if (answer)
                {
                    ErrorLabel.Text = "";
                    GHApp.CurrentMainPage?.InvalidateCarousel();
                    var page = await GHApp.Navigation.PopModalAsync();
                    GHApp.DisconnectIViewHandlers(page);
                }
                else
                {
                    CancelButton.IsEnabled = true;
                }
            }
            else
            {
                ErrorLabel.Text = "";
                GHApp.CurrentMainPage?.InvalidateCarousel();
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
            }
        }

        private async void ResetButton_Clicked(object sender, EventArgs e)
        {
            ResetButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool answer = await GHApp.DisplayMessageBox(this, "Reset Options File?", "Are you sure to reset the options file?", "Yes", "No");
            if(answer)
            {
                ErrorLabel.Text = "";
                await GHApp.GnollHackService.ResetDefaultsFile();
                GHApp.CurrentMainPage?.InvalidateCarousel();
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
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

                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseGrid.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
                double bordermargin = UIUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                TextEditor.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
            }
        }

    }
}
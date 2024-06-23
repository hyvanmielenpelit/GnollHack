using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
using GnollHackX.Controls;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class LibraryPage : ContentPage
	{
		public LibraryPage ()
		{
			InitializeComponent ();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetPageLayoutCursorOnHandler(this, RootGrid);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
                EmptyLabel.TextColor = GHColors.White;
            }
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        Dictionary<int, StoredManual> _manuals = new Dictionary<int, StoredManual>();

        public void ReadLibrary()
        {
            _manuals.Clear();
            string datadir = Path.Combine(GHApp.GHPath, GHConstants.UserDataDirectory);
            if (Directory.Exists(datadir))
            {
                string[] files = Directory.GetFiles(datadir);
                foreach (string file in files)
                {
                    bool fileexists = File.Exists(file);
                    FileInfo fileinfo = new FileInfo(file);
                    if (fileinfo.Name.Length > GHConstants.ManualFilePrefix.Length &&
                        fileinfo.Name.Substring(0, GHConstants.ManualFilePrefix.Length) == GHConstants.ManualFilePrefix &&
                        fileexists)
                    {
                        StoredManual sm = null;
                        try
                        {
                            using(FileStream fs = File.OpenRead(file)) 
                            {
                                using(StreamReader sr = new StreamReader(fs))
                                {
                                    string json = sr.ReadToEnd();
                                    sm = JsonConvert.DeserializeObject<StoredManual>(json);
                                }
                            }
                        }
                        catch 
                        {

                        }
                        if(sm != null)
                            _manuals.Add(sm.Id, sm);
                    }
                }
            }

            int maxManuals = GHApp.GnollHackService.GetMaxManuals();
            List<StoredManual> manuallist = _manuals.Values.ToList();
            if(manuallist.Count > 0)
            {
                Comparison<StoredManual> comp = new Comparison<StoredManual>((m1, m2) => { return string.Compare(m1.Name, m2.Name); });
                manuallist.Sort(comp);
                foreach (StoredManual sm in manuallist)
                {
                    RowImageButton rib = new RowImageButton();
                    rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.examine.png";
                    rib.LblText = sm.Name;
                    rib.LblFontSize = 17;
                    rib.ImgWidth = 80;
                    rib.ImgHeight = 80;
                    rib.GridWidth = 480;
                    rib.GridHeight = 80;
                    rib.WidthRequest = 480;
                    rib.HeightRequest = 80;
                    rib.GridMargin = new Thickness(rib.ImgWidth / 15, 0);
                    rib.BtnCommand = sm.Id;
                    rib.BtnClicked += LibraryButton_Clicked;
                    LibraryLayout.Children.Add(rib);
                }
                lblSubtitle.Text = "Found " + manuallist.Count + " of " + maxManuals + " manuals";
            }
            else
                EmptyLabel.IsVisible = true;
        }

        private async void LibraryButton_Clicked(object sender, EventArgs e)
        {
            LibraryLayout.IsEnabled = false;
            RowImageButton ghbutton = sender as RowImageButton;
            if (ghbutton != null)
            {
                StoredManual sm;
                if (_manuals.TryGetValue(ghbutton.BtnCommand, out sm))
                {
                    string errormsg;
                    var dispfilepage = new DisplayFilePage(null, sm.Name, 0, false, false, false);
                    dispfilepage.OverridingText = sm.Text;
                    dispfilepage.UseFixedFontSize = true;
                    dispfilepage.FontSize = 14;
                    if (dispfilepage.ReadFile(out errormsg))
                        await App.Current.MainPage.Navigation.PushModalAsync(dispfilepage);
                    else
                        await DisplayAlert("Error Reading Manual", "Reading the manual entitled " + sm.Name + " failed: " + errormsg, "OK");
                }
            }
            LibraryLayout.IsEnabled = true;
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

                lblHeader.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                LibraryLayout.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }
    }

}
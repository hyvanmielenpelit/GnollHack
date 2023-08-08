using GnollHackX.Controls;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
{
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class LibraryPage : ContentPage
	{
		public LibraryPage ()
		{
			InitializeComponent ();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        Dictionary<int, StoredManual> manuals = new Dictionary<int, StoredManual>();

        public void ReadLibrary()
        {
            string datadir = Path.Combine(App.GHPath, GHConstants.UserDataDirectory);
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
                        {
                            manuals.Add(sm.Id, sm);
                            LabeledImageButton lib = new LabeledImageButton();
                            lib.ImgSourcePath = "resource://GnollHackX.Assets.UI.examine.png";
                            lib.LargerFont = false;
                            lib.LblText = sm.Name;
                            lib.SetSideSize(2000, 2000);
                            lib.GridMargin = new Thickness(lib.ImgWidth / 15, lib.ImgWidth / 30);
                            lib.BtnCommand = sm.Id;
                            lib.BtnClicked += LibraryButton_Clicked;
                            LibraryLayout.Children.Add(lib);
                        }
                    }
                }
            }
        }

        private async void LibraryButton_Clicked(object sender, EventArgs e)
        {
            LabeledImageButton ghbutton = sender as LabeledImageButton;
            if (ghbutton != null)
            {
                StoredManual sm;
                if (manuals.TryGetValue(ghbutton.BtnCommand, out sm))
                {
                    string errormsg;
                    var dispfilepage = new DisplayFilePage(null, sm.Name, 0, false, false);
                    dispfilepage.OverridingText = sm.Text;
                    dispfilepage.UseFixedFontSize = true;
                    dispfilepage.FontSize = 14;
                    if (dispfilepage.ReadFile(out errormsg))
                        await App.Current.MainPage.Navigation.PushModalAsync(dispfilepage);
                    else
                        await DisplayAlert("Error Reading Manual", "Reading the manual entitled " + sm.Name + " failed: " + errormsg, "OK");
                }
            }
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

                lblHeader.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                LibraryLayout.Margin = ClientUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

    }
}
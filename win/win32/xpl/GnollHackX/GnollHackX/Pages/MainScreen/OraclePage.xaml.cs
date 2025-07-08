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
using Xamarin.Essentials;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class OraclePage : ContentPage, ICloseablePage
    {
		public OraclePage ()
		{
			InitializeComponent ();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
                lblSubtitle.TextColor = GHColors.White;
                EmptyLabel.TextColor = GHColors.White;
            }
        }

        private async void Button_Clicked(object sender, EventArgs e)
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

        Dictionary<int, StoredManual> _consultations = new Dictionary<int, StoredManual>();

        public void ReadConsultations()
        {
            GHApp.PopulateHints(_consultations, GHConstants.OracleMajorConsultationFilePrefix);

            int maxConsultations = GHApp.GnollHackService.GetMaxMajorConsultations();
            List<StoredManual> consultationlist = _consultations.Values.ToList();
            lblSubtitle.Text = "Received " + consultationlist.Count + " of " + maxConsultations + " consultations";
            if (consultationlist.Count > 0)
            {
                Comparison<StoredManual> comp = new Comparison<StoredManual>((m1, m2) => { return string.Compare(m1.Name, m2.Name); });
                consultationlist.Sort(comp);
                foreach (StoredManual sm in consultationlist)
                {
                    RowImageButton rib = new RowImageButton();
                    rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.oracle.png";
                    rib.ImgHighFilterQuality = true;
                    rib.LblText = sm.Name;
                    rib.LblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.LblFontSize = 17;
                    rib.ImgWidth = 80;
                    rib.ImgHeight = 80;
                    rib.GridWidth = 480;
                    rib.GridHeight = 80;
#if GNH_MAUI
                    rib.MaximumWidthRequest = 480;
#else
                    rib.WidthRequest = 480;
#endif
                    rib.HeightRequest = 80;
                    rib.GridMargin = new Thickness(rib.ImgWidth / 15, 0);
                    rib.BtnCommand = sm.Id;
                    rib.BtnClicked += LibraryButton_Clicked;
                    OracleLayout.Children.Add(rib);
                }
            }
            else
                EmptyLabel.IsVisible = true;

        }

        private async void LibraryButton_Clicked(object sender, EventArgs e)
        {
            OracleLayout.IsEnabled = false;
            RowImageButton ghbutton = sender as RowImageButton;
            if (ghbutton != null)
            {
                StoredManual sm;
                if (_consultations.TryGetValue(ghbutton.BtnCommand, out sm))
                {
                    string errormsg;
                    var dispfilepage = new DisplayFilePage(null, sm.Name, 0, false, false, false);
                    dispfilepage.OverridingText = sm.Text;
                    dispfilepage.UseFixedFontSize = true;
                    dispfilepage.FontSize = 14;
                    if (dispfilepage.ReadFile(out errormsg))
                        await GHApp.Navigation.PushModalAsync(dispfilepage);
                    else
                        await GHApp.DisplayMessageBox(this, "Error Reading Consultation", "Reading the consultation entitled " + sm.Name + " failed: " + errormsg, "OK");
                }
            }
            OracleLayout.IsEnabled = true;
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
                OracleLayout.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                var page = await GHApp.Navigation.PopModalAsync();
                GHApp.DisconnectIViewHandlers(page);
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
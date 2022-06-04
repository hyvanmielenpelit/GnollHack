using System;
using System.Collections.Generic;
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
    public partial class VersionPage : ContentPage
    {
        public VersionPage()
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
            //string platver = App.PlatformService.GetVersionString();
            //string text = "";
            string manufacturer = DeviceInfo.Manufacturer;
            if(manufacturer.Length > 0)
                manufacturer = manufacturer.Substring(0,1).ToUpper() + manufacturer.Substring(1);

            ulong TotalMemInBytes = App.PlatformService.GetDeviceMemoryInBytes();
            ulong TotalMemInMB = (TotalMemInBytes / 1024) / 1024;
            PortVersionTitleLabel.Text = Device.RuntimePlatform + " Port Version:";
            PortBuildTitleLabel.Text = Device.RuntimePlatform + " Port Build:";

            GnollHackVersionLabel.Text = App.GHVersionString;
            PortVersionLabel.Text = VersionTracking.CurrentVersion;
            PortBuildLabel.Text = VersionTracking.CurrentBuild;
            FMODVersionLabel.Text = App.FMODVersionString;
            PlatformLabel.Text = DeviceInfo.Platform + " " + DeviceInfo.VersionString;
            DeviceLabel.Text = manufacturer + " " + DeviceInfo.Model;
            TotalMemoryLabel.Text = TotalMemInMB + " MB";

            //text += Environment.NewLine + VersionTracking.CurrentVersion;
            //text += Environment.NewLine + VersionTracking.CurrentBuild;
            //text += Environment.NewLine + App.FMODVersionString;
            //text += Environment.NewLine + DeviceInfo.Platform + " " + DeviceInfo.VersionString;
            //text += Environment.NewLine + manufacturer + " " + DeviceInfo.Model;

            //text += "GnollHack Version: ";
            //text += Environment.NewLine + Device.RuntimePlatform + " Port Version: ";
            //text += Environment.NewLine + Device.RuntimePlatform + " Port Build: ";
            //text += Environment.NewLine + "FMOD Version: ";
            //text += Environment.NewLine + "Platform: ";
            //text += Environment.NewLine + "Device: ";
            //KeyLabel.Text = text;

            //text = "";
            //text += App.GHVersionString;
            //text += Environment.NewLine + VersionTracking.CurrentVersion;
            //text += Environment.NewLine + VersionTracking.CurrentBuild;
            //text += Environment.NewLine + App.FMODVersionString;
            //text += Environment.NewLine + DeviceInfo.Platform + " " + DeviceInfo.VersionString;
            //text += Environment.NewLine + manufacturer + " " + DeviceInfo.Model;
            //ValueLabel.Text = text;

            //text = Environment.NewLine + "GnollHack Long Version Identifier: " + Environment.NewLine + App.GHVersionId;
            LongTitleLabel.Text = Environment.NewLine + "GnollHack Long Version Identifier:";
            LongLabel.Text = App.GHVersionId;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
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
                margins = LongLabel.Margin;

                double bordermargin = ClientUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                MainGrid.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
                double target_width = (Math.Min(width, MainGrid.WidthRequest) - MainGrid.Margin.Left - MainGrid.Margin.Right
                    - MainGrid.Padding.Left - MainGrid.Padding.Right - margins.Left - margins.Right);
                double newsize = 15 * target_width / 400;

                foreach(View v in VersionInfoGrid.Children)
                {
                    if(v is Label)
                    {
                        ((Label)v).FontSize = newsize;
                    }
                }
                //KeyLabel.FontSize = newsize;
                //ValueLabel.FontSize = newsize;
                LongTitleLabel.FontSize = newsize;
                LongLabel.FontSize = newsize;

                HeaderLabel.Margin = ClientUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseGrid.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

    }
}
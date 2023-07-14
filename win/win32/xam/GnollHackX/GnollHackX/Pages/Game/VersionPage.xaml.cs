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

namespace GnollHackX.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class VersionPage : ContentPage
    {
        private GamePage _gamePage = null;
        public VersionPage(GamePage gamePage)
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);

            _gamePage = gamePage;

            //string platver = App.PlatformService.GetVersionString();
            //string text = "";
            string manufacturer = DeviceInfo.Manufacturer;
            if(manufacturer.Length > 0)
                manufacturer = manufacturer.Substring(0,1).ToUpper() + manufacturer.Substring(1);

            ulong TotalMemInBytes = App.PlatformService.GetDeviceMemoryInBytes();
            ulong TotalMemInMB = (TotalMemInBytes / 1024) / 1024;
            ulong FreeDiskSpaceInBytes = App.PlatformService.GetDeviceFreeDiskSpaceInBytes();
            ulong FreeDiskSpaceInGB = ((FreeDiskSpaceInBytes / 1024) / 1024) / 1024;
            ulong TotalDiskSpaceInBytes = App.PlatformService.GetDeviceTotalDiskSpaceInBytes();
            ulong TotalDiskSpaceInGB = ((TotalDiskSpaceInBytes / 1024) / 1024) / 1024;

            long TotalPlayTime = Preferences.Get("RealPlayTime", 0L);
            long TotalPlayHours = TotalPlayTime / 3600;
            long TotalPlayMinutes = (TotalPlayTime % 3600) / 60;
            long TotalPlaySeconds = TotalPlayTime - TotalPlayHours * 3600 - TotalPlayMinutes * 60;

            long CurrentPlayTime = App.AggregateSessionPlayTime;
            long CurrentPlayHours = CurrentPlayTime / 3600;
            long CurrentPlayMinutes = (CurrentPlayTime % 3600) / 60;
            long CurrentPlaySeconds = CurrentPlayTime - CurrentPlayHours * 3600 - CurrentPlayMinutes * 60;

            if (_gamePage != null && _gamePage.ClientGame != null)
            {
                long GameDurationTime = _gamePage.ClientGame.GamePlayTime;
                long GameDurationHours = GameDurationTime / 3600;
                long GameDurationMinutes = (GameDurationTime % 3600) / 60;
                long GameDurationSeconds = GameDurationTime - GameDurationHours * 3600 - GameDurationMinutes * 60;
                GameDurationLabel.Text = GameDurationHours + " h " + GameDurationMinutes + " min " + GameDurationSeconds + " s";

                long SessionPlayTime = _gamePage.ClientGame.SessionPlayTime;
                long SessionPlayHours = SessionPlayTime / 3600;
                long SessionPlayMinutes = (SessionPlayTime % 3600) / 60;
                long SessionPlaySeconds = SessionPlayTime - SessionPlayHours * 3600 - SessionPlayMinutes * 60;
                SessionTimeLabel.Text = SessionPlayHours + " h " + SessionPlayMinutes + " min " + SessionPlaySeconds + " s";
            }
            else
            {
                GameDurationLabel.Text = "";
                GameDurationLabel.IsVisible = false;
                GameDurationTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(GameDurationLabel);
                VersionInfoGrid.Children.Remove(GameDurationTitleLabel);
                VersionInfoGrid.RowDefinitions.Remove(GameDurationRowDefinition);

                SessionTimeLabel.Text = "";
                SessionTimeLabel.IsVisible = false;
                SessionTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(SessionTimeLabel);
                VersionInfoGrid.Children.Remove(SessionTitleLabel);
                VersionInfoGrid.RowDefinitions.Remove(SessionPlayTimeRowDefinition);
            }

            PortVersionTitleLabel.Text = Device.RuntimePlatform + " Port Version:";
            PortBuildTitleLabel.Text = Device.RuntimePlatform + " Port Build:";

            GnollHackVersionLabel.Text = App.GHVersionString;
            PortVersionLabel.Text = VersionTracking.CurrentVersion;
            PortBuildLabel.Text = VersionTracking.CurrentBuild;
            FMODVersionLabel.Text = App.FMODVersionString;
            SkiaVersionLabel.Text = App.SkiaVersionString + " (# " + App.SkiaSharpVersionString + ")";
            PlatformLabel.Text = DeviceInfo.Platform + " " + DeviceInfo.VersionString;
            DeviceLabel.Text = manufacturer + " " + DeviceInfo.Model;
            TotalMemoryLabel.Text = TotalMemInMB + " MB";
            DiskSpaceLabel.Text = FreeDiskSpaceInGB + " GB" + " / " + TotalDiskSpaceInGB + " GB";
            TotalPlayTimeLabel.Text = TotalPlayHours + " h " + TotalPlayMinutes + " min " + TotalPlaySeconds + " s";
            CurrentPlayTimeLabel.Text = CurrentPlayHours + " h " + CurrentPlayMinutes + " min " + CurrentPlaySeconds + " s";

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
            CloseButton.IsEnabled = false;
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
                CloseButton.Margin = ClientUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

    }
}
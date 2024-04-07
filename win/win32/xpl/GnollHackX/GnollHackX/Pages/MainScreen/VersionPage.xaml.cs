using System;
using System.Collections.Generic;
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
using GnollHackX.Pages.Game;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class VersionPage : ContentPage
    {
        private GamePage _gamePage = null;
        public VersionPage(GamePage gamePage)
        {
            InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(false);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif

            _gamePage = gamePage;

            string compatstr = GHApp.GHVersionCompatibilityString;
            string manufacturer = DeviceInfo.Manufacturer;
            if (manufacturer.Length > 0)
                manufacturer = manufacturer.Substring(0, 1).ToUpper() + manufacturer.Substring(1);

            ulong TotalMemInBytes = GHApp.PlatformService.GetDeviceMemoryInBytes();
            ulong TotalMemInMB = (TotalMemInBytes / 1024) / 1024;
            ulong FreeDiskSpaceInBytes = GHApp.PlatformService.GetDeviceFreeDiskSpaceInBytes();
            ulong FreeDiskSpaceInGB = ((FreeDiskSpaceInBytes / 1024) / 1024) / 1024;
            ulong TotalDiskSpaceInBytes = GHApp.PlatformService.GetDeviceTotalDiskSpaceInBytes();
            ulong TotalDiskSpaceInGB = ((TotalDiskSpaceInBytes / 1024) / 1024) / 1024;

            long TotalPlayTime = Preferences.Get("RealPlayTime", 0L);
            long TotalPlayHours = TotalPlayTime / 3600;
            long TotalPlayMinutes = (TotalPlayTime % 3600) / 60;
            long TotalPlaySeconds = TotalPlayTime - TotalPlayHours * 3600 - TotalPlayMinutes * 60;

            long CurrentPlayTime = GHApp.AggregateSessionPlayTime;
            long CurrentPlayHours = CurrentPlayTime / 3600;
            long CurrentPlayMinutes = (CurrentPlayTime % 3600) / 60;
            long CurrentPlaySeconds = CurrentPlayTime - CurrentPlayHours * 3600 - CurrentPlayMinutes * 60;

            if (_gamePage != null && _gamePage.CurrentGame != null)
            {
                long GameDurationTime = _gamePage.CurrentGame.GamePlayTime;
                long GameDurationHours = GameDurationTime / 3600;
                long GameDurationMinutes = (GameDurationTime % 3600) / 60;
                long GameDurationSeconds = GameDurationTime - GameDurationHours * 3600 - GameDurationMinutes * 60;
                GameDurationLabel.Text = GameDurationHours + " h " + GameDurationMinutes + " min " + GameDurationSeconds + " s";

                long SessionPlayTime = _gamePage.CurrentGame.SessionPlayTime;
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

            if(GHApp.GPUBackend != null)
            {
                GPUBackendLabel.Text = GHApp.GPUBackend;
            }
            else
            {
                GPUBackendLabel.Text = "";
                GPUBackendLabel.IsVisible = false;
                GPUBackendTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(GPUBackendLabel);
                VersionInfoGrid.Children.Remove(GPUBackendTitleLabel);
                VersionInfoGrid.RowDefinitions.Remove(GPUBackendRowDefinition);
            }

            if (GHApp.GPUCacheSize >= 0)
            {
                long TotalGPUCacheInBytes = GHApp.GPUCacheSize;
                long TotalGPUCacheInMB = (TotalGPUCacheInBytes / 1024) / 1024;
                GPUCacheSizeLabel.Text = TotalGPUCacheInMB + " MB";
            }
            else
            {
                GPUCacheSizeLabel.Text = "";
                GPUCacheSizeLabel.IsVisible = false;
                GPUCacheSizeTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(GPUCacheSizeLabel);
                VersionInfoGrid.Children.Remove(GPUCacheSizeTitleLabel);
                VersionInfoGrid.RowDefinitions.Remove(GPUCacheSizeRowDefinition);
            }

            PortVersionTitleLabel.Text = GHApp.RuntimePlatform + " Port Version:";
            PortBuildTitleLabel.Text = GHApp.RuntimePlatform + " Port Build:";
            PortConfigurationTitleLabel.Text ="Port Configuration:";

            GnollHackVersionLabel.Text = GHApp.GHVersionString;
            GnollHackConfigurationLabel.Text = GHApp.GHDebug ? "Debug" : "Release";
            PortVersionLabel.Text = VersionTracking.CurrentVersion;
            PortBuildLabel.Text = VersionTracking.CurrentBuild;
            PortConfigurationLabel.Text =
#if DEBUG
                "Debug";
#else
                "Release";
#endif
            GnollHackCompatibilityLabel.Text = compatstr == "" ? "" : "From " + compatstr;
            FMODVersionLabel.Text = GHApp.FMODVersionString;
            SkiaVersionLabel.Text = GHApp.SkiaVersionString + " (# " + GHApp.SkiaSharpVersionString + ")";
            PlatformLabel.Text = DeviceInfo.Platform + " " + DeviceInfo.VersionString;
            DeviceLabel.Text = manufacturer + " " + DeviceInfo.Model;
            TotalMemoryLabel.Text = TotalMemInMB + " MB";
            DiskSpaceLabel.Text = FreeDiskSpaceInGB + " GB" + " / " + TotalDiskSpaceInGB + " GB";
            TotalPlayTimeLabel.Text = TotalPlayHours + " h " + TotalPlayMinutes + " min " + TotalPlaySeconds + " s";
            CurrentPlayTimeLabel.Text = CurrentPlayHours + " h " + CurrentPlayMinutes + " min " + CurrentPlaySeconds + " s";
            LongTitleLabel.Text = Environment.NewLine + "GnollHack Long Version Identifier:";
            LongLabel.Text = GHApp.GHVersionId;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
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

                double bordermargin = UIUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                MainGrid.Margin = new Thickness(bordermargin, 0, bordermargin, 0);
                double target_width = (Math.Min(width,
#if GNH_MAUI
                        MainGrid.MaximumWidthRequest
#else
                        MainGrid.WidthRequest
#endif
                    ) - MainGrid.Margin.Left - MainGrid.Margin.Right
                    - MainGrid.Padding.Left - MainGrid.Padding.Right - margins.Left - margins.Right);
                double newsize = 15 * target_width / 400;

                foreach (View v in VersionInfoGrid.Children)
                {
                    if (v is Label)
                    {
                        ((Label)v).FontSize = newsize;
                    }
                }
                LongTitleLabel.FontSize = newsize;
                LongLabel.FontSize = newsize;

                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

    }
}
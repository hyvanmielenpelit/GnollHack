using System;
using System.Collections.Generic;
using System.Diagnostics;
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
using GnollHackX.Pages.Game;
using GnollHackX.Controls;

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
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _gamePage = gamePage;

            if (GHApp.DarkMode)
            {
                HeaderLabel.TextColor = GHColors.White;
                LongTitleLabel.TextColor = GHColors.White;
                LongLabel.TextColor = GHColors.White;
                foreach (View child in VersionInfoGrid.Children)
                {
                    if (child != null && child is Label)
                    {
                        Label label = (Label)child;
                        label.TextColor = GHColors.White;
                    }
                }
            }


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

            long TotalPlayTime = GHApp.RealPlayTime;
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
                //VersionInfoGrid.RowDefinitions.Remove(GameDurationRowDefinition);
                GameDurationRowDefinition.Height = 0;

                SessionTimeLabel.Text = "";
                SessionTimeLabel.IsVisible = false;
                SessionTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(SessionTimeLabel);
                VersionInfoGrid.Children.Remove(SessionTitleLabel);
                //VersionInfoGrid.RowDefinitions.Remove(SessionPlayTimeRowDefinition);
                SessionPlayTimeRowDefinition.Height = 0;
            }

            /* Update GPU resource usage to latest value */
            if(_gamePage != null)
            {
                GHApp.CurrentGPUCacheUsage = _gamePage.GetPrimaryCanvasResourceCacheUsage();
            }

            string backend = GHApp.GPUBackend;
            if (backend != null)
            {
                GPUBackendLabel.Text = backend;
            }
            else
            {
                GPUBackendLabel.Text = "";
                GPUBackendLabel.IsVisible = false;
                GPUBackendTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(GPUBackendLabel);
                VersionInfoGrid.Children.Remove(GPUBackendTitleLabel);
                //VersionInfoGrid.RowDefinitions.Remove(GPUBackendRowDefinition);
                GPUBackendRowDefinition.Height = 0;
            }

            long cacheSize = GHApp.CurrentGPUCacheSize;
            if (cacheSize >= 0)
            {
                long TotalGPUCacheInBytes = cacheSize;
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
                //VersionInfoGrid.RowDefinitions.Remove(GPUCacheSizeRowDefinition);
                GPUCacheSizeRowDefinition.Height = 0;
            }

            CacheUsageInfo cacheUsage = GHApp.CurrentGPUCacheUsage;
            int cacheUsageResources = cacheUsage.MaxResources;
            long cacheUsageBytes = cacheUsage.MaxResourceBytes;
            if (cacheUsageBytes >= 0 || cacheUsageResources >= 0)
            {
                string str = "";
                if(cacheUsageBytes >= 0)
                {
                    long TotalCacheUsageInBytes = cacheUsageBytes;
                    long TotalCacheUsageInMB = (TotalCacheUsageInBytes / 1024) / 1024;
                    str += TotalCacheUsageInMB + " MB";
                }
                if (cacheUsageResources >= 0)
                {
                    if (str != "")
                        str += " / ";
                    str += cacheUsageResources + " #";
                }
                GPUCacheUsageLabel.Text = str;
            }
            else
            {
                GPUCacheUsageLabel.Text = "";
                GPUCacheUsageLabel.IsVisible = false;
                GPUCacheUsageTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(GPUCacheUsageLabel);
                VersionInfoGrid.Children.Remove(GPUCacheUsageTitleLabel);
                //VersionInfoGrid.RowDefinitions.Remove(GPUCacheUsageRowDefinition);
                GPUCacheUsageRowDefinition.Height = 0;
            }

#if WINDOWS
            string winRTAssemblyVersion = GetAssemblyInformationalVersion(typeof(WinRT.AgileReference));
            WinRTLabel.Text = !string.IsNullOrEmpty(winRTAssemblyVersion) ? winRTAssemblyVersion : "?";

            string winUIAssemblyVersion = GetAssemblyFileVersion(typeof(Microsoft.UI.Xaml.Application));
            WinUILabel.Text = !string.IsNullOrEmpty(winUIAssemblyVersion) ? winUIAssemblyVersion : "?";

            string winSDKAssemblyVersion = GetAssemblyInformationalVersion(typeof(Windows.UI.Color));
            WinSDKLabel.Text = !string.IsNullOrEmpty(winSDKAssemblyVersion) ? winSDKAssemblyVersion : "?";

            string winAppSDKAssemblyVersion = typeof(Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap)?.Assembly?.GetName()?.Version?.ToString();
            WinAppSDKLabel.Text = !string.IsNullOrEmpty(winAppSDKAssemblyVersion) ? winAppSDKAssemblyVersion : "?";

            if (GHApp.DeviceGPUs.Count > 1)
            {
                string activeGPU = GHApp.GetActiveGPU();
                ActiveGPULabel.Text = activeGPU + " (" + GHApp.DeviceGPUs.Count + " GPUs)";
            }
            else
            {
                ActiveGPULabel.Text = "";
                ActiveGPULabel.IsVisible = false;
                ActiveGPUTitleLabel.IsVisible = false;
                VersionInfoGrid.Children.Remove(ActiveGPULabel);
                VersionInfoGrid.Children.Remove(ActiveGPUTitleLabel);
                ActiveGPURowDefinition.Height = 0;
            }
#else
            WinRTLabel.Text = "";
            WinRTLabel.IsVisible = false;
            WinRTTitleLabel.IsVisible = false;
            VersionInfoGrid.Children.Remove(WinRTLabel);
            VersionInfoGrid.Children.Remove(WinRTTitleLabel);
            WinRTRowDefinition.Height = 0;

            WinUILabel.Text = "";
            WinUILabel.IsVisible = false;
            WinUITitleLabel.IsVisible = false;
            VersionInfoGrid.Children.Remove(WinUILabel);
            VersionInfoGrid.Children.Remove(WinUITitleLabel);
            WinUIRowDefinition.Height = 0;

            WinSDKLabel.Text = "";
            WinSDKLabel.IsVisible = false;
            WinSDKTitleLabel.IsVisible = false;
            VersionInfoGrid.Children.Remove(WinSDKLabel);
            VersionInfoGrid.Children.Remove(WinSDKTitleLabel);
            WinSDKRowDefinition.Height = 0;

            WinAppSDKLabel.Text = "";
            WinAppSDKLabel.IsVisible = false;
            WinAppSDKTitleLabel.IsVisible = false;
            VersionInfoGrid.Children.Remove(WinAppSDKLabel);
            VersionInfoGrid.Children.Remove(WinAppSDKTitleLabel);
            WinAppSDKRowDefinition.Height = 0;

            ActiveGPULabel.Text = "";
            ActiveGPULabel.IsVisible = false;
            ActiveGPUTitleLabel.IsVisible = false;
            VersionInfoGrid.Children.Remove(ActiveGPULabel);
            VersionInfoGrid.Children.Remove(ActiveGPUTitleLabel);
            ActiveGPURowDefinition.Height = 0;
#endif

#if GNH_MAUI
            Version ver = AppInfo.Current.Version;
            string portVersion = (ver?.Major.ToString() ?? "?") + "." + (ver?.Minor.ToString() ?? "?");
            PortVersionLabel.Text = portVersion; // GetAssemblyInformationalVersion(Assembly.GetEntryAssembly()); //This can also be AppInfo.Current.VersionString, but it is longer and the build number
#if WINDOWS
            PortBuildLabel.Text = AppInfo.Current.Version.Build.ToString();
#else
            PortBuildLabel.Text = AppInfo.Current.BuildString;
#endif
#else
            PortVersionLabel.Text = VersionTracking.CurrentVersion;
            PortBuildLabel.Text = VersionTracking.CurrentBuild;
#endif
            PortVersionTitleLabel.Text = GHApp.RuntimePlatform + " Port Version:";
            PortBuildTitleLabel.Text = GHApp.RuntimePlatform + " Port Build:";
            PortConfigurationTitleLabel.Text ="Port Configuration:";
#if BETA
            PortVersionLabel.Text += " (Beta)";
#endif

            GnollHackVersionLabel.Text = GHApp.GHVersionString;
            GnollHackConfigurationLabel.Text = GHApp.GHDebug ? "Debug" : "Release";
            PortConfigurationLabel.Text =
#if DEBUG
                "Debug";
#else
                "Release";
#endif

            PackagingModelLabel.Text = GHApp.IsPackaged ? "Packaged" : "Unpackaged";

            GnollHackCompatibilityLabel.Text = compatstr == "" ? "" : "From " + compatstr;
            FMODVersionLabel.Text = GHApp.FMODVersionString;
            SkiaVersionLabel.Text = GHApp.SkiaVersionString + " (# " + GHApp.SkiaSharpVersionString + ")";
            FrameworkVersionLabel.Text = GHApp.FrameworkVersionString;
#if GNH_MAUI
            FrameworkVersionLabel.Text += " / MAUI " + GHApp.FrameworkMAUIVersionString;
#endif
            RuntimeVersionLabel.Text = GHApp.RuntimeVersionString;
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
            var page = await GHApp.Navigation.PopModalAsync();
            GHApp.DisconnectIViewHandlers(page);
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
                        TextScrollGrid.MaximumWidthRequest
#else
                        TextScrollGrid.WidthRequest
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

#if WINDOWS
        private string GetAssemblyInformationalVersion(Type type)
        {
            return GetAssemblyInformationalVersion(type?.Assembly);
        }

        private string GetAssemblyInformationalVersion(Assembly assembly)
        {
            if (assembly == null)
            {
                return null;
            }

            return TrimVersion(assembly?.GetCustomAttribute<AssemblyInformationalVersionAttribute>()?.InformationalVersion);
        }

        private string GetAssemblyFileVersion(Type type)
        {
            return GetAssemblyFileVersion(type?.Assembly);
        }
        private string GetAssemblyFileVersion(Assembly assembly)
        {
            if (assembly == null)
            {
                return null;
            }

            return assembly?.GetCustomAttribute<AssemblyFileVersionAttribute>()?.Version ?? "";
        }

        private string TrimVersion(string version)
        {
            if (string.IsNullOrEmpty(version))
            {
                return version;
            }

            var plusIndex = version.IndexOf('+');
            if (plusIndex > 0)
            {
                return version.Substring(0, plusIndex);
            }

            return version;
        }
#endif
    }
}
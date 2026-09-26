using System;
using System.Collections.Generic;
#if GNH_MAUI
using GnollHackM;
using Microsoft.Maui.Devices;
using Microsoft.Maui.Storage;
#else
using Xamarin.Essentials;
#endif

namespace GnollHackX.Performance
{
    /* Snapshot of the environment a performance run was measured in: the device, the
       versions, the display and the feature-toggle vector in effect. */
    public struct GHPerformanceEnvironmentFacts
    {
        public string Platform;
        public string DeviceId;
        public string DeviceModel;
        public string DeviceOs;
        public string AppVersion;
        public string RuntimeVersion;
        public string FrameworkVersion;
        public string UiFrameworkVersion;
        public string SkiaSharpVersion;
        public string FmodVersion;
        public string GpuBackend;
        public long GpuCacheSize;
        public bool MainCanvasUsesGpu;
        public string MapRefreshRateSetting;
        public double ReportedRefreshHz;
        public string BuildConfiguration;
        public string GitCommit;
        public Dictionary<string, object> Configuration;
    }

    /* Reads GHApp's static state into a GHPerformanceEnvironmentFacts snapshot. Every
       read is guarded, so a missing service or platform API leaves its field null or
       default rather than throwing. Must compile under C# 7.3. */
    public static class GHPerformanceEnvironment
    {
        public static GHPerformanceEnvironmentFacts Capture()
        {
            GHPerformanceEnvironmentFacts f = new GHPerformanceEnvironmentFacts();
            f.Configuration = new Dictionary<string, object>();

            try
            {
                f.Platform = GHApp.IsAndroid ? "Android" : GHApp.IsiOS ? "iOS" : GHApp.IsWindows ? "Windows" : GHApp.RuntimePlatform;
            }
            catch { }

            try
            {
                string manufacturer = DeviceInfo.Manufacturer;
                if (manufacturer != null && manufacturer.Length > 0)
                    manufacturer = manufacturer.Substring(0, 1).ToUpper() + manufacturer.Substring(1);
                f.DeviceModel = (manufacturer + " " + DeviceInfo.Model).Trim();
                f.DeviceOs = DeviceInfo.Platform + " " + DeviceInfo.VersionString;
                f.DeviceId = f.DeviceModel + " / " + f.DeviceOs;
            }
            catch { }

            try
            {
                f.AppVersion = GHApp.GHVersionString;
                f.RuntimeVersion = GHApp.RuntimeVersionString;
                f.FrameworkVersion = GHApp.FrameworkVersionString;
                f.UiFrameworkVersion = GHApp.UIFrameworkVersionString;
                f.SkiaSharpVersion = GHApp.SkiaSharpVersionString;
                f.FmodVersion = GHApp.FMODVersionString;
                f.GpuBackend = GHApp.GPUBackend;
                f.GpuCacheSize = GHApp.CurrentGPUCacheSize;
            }
            catch { }

            try
            {
                f.MainCanvasUsesGpu = GHApp.UseGPU;
            }
            catch { }

#if DEBUG
            f.BuildConfiguration = "Debug";
#else
            f.BuildConfiguration = "Release";
#endif
            f.GitCommit = null;

            try
            {
                f.ReportedRefreshHz = GHApp.RoundedReconciledRefreshRate;
            }
            catch { }

            try
            {
                int value = Preferences.Get("MapRefreshRate", -1);
                MapRefreshRateStyle style = value < 0 ? UIUtils.GetDefaultMapFPS() : (MapRefreshRateStyle)value;
                f.MapRefreshRateSetting = style.ToString();
            }
            catch { }

            AddToggle(f.Configuration, "useTileBatching", () => GHApp.UseTileBatching);
            AddToggle(f.Configuration, "useTextBlobCaching", () => GHApp.UseTextBlobCaching);
            AddToggle(f.Configuration, "runtimeEffects", () => GHApp.RuntimeEffects);
            AddToggle(f.Configuration, "usePlatformRenderLoop", () => GHApp.UsePlatformRenderLoop);
            AddToggle(f.Configuration, "useMainGLCanvas", () => GHApp.UseGPU);
            AddToggle(f.Configuration, "useAuxiliaryGLCanvas", () => GHApp.UseAuxGPU);
            AddToggle(f.Configuration, "useMainMipMap", () => GHApp.UseMipMap);
            AddToggle(f.Configuration, "fixRects", () => GHApp.FixRects);
            AddToggle(f.Configuration, "fixFiltering", () => GHApp.FixFiltering);
            AddToggle(f.Configuration, "primaryGPUCacheLimit", () => GHApp.PrimaryGPUCacheLimit);
            AddToggle(f.Configuration, "secondaryGPUCacheLimit", () => GHApp.SecondaryGPUCacheLimit);
            AddToggle(f.Configuration, "frameTimeProfiler", () => FrameTimeProfiler.IsEnabled);
            AddToggle(f.Configuration, "screenLogging", () => GHApp.ScreenLogging);
            AddToggle(f.Configuration, "debugLogMessages", () => GHApp.DebugLogMessages);
            AddToggle(f.Configuration, "developerMode", () => GHApp.DeveloperMode);
            return f;
        }

        private static void AddToggle(Dictionary<string, object> configuration, string key, Func<object> read)
        {
            try
            {
                configuration[key] = read();
            }
            catch
            {
                configuration[key] = null;
            }
        }
    }
}

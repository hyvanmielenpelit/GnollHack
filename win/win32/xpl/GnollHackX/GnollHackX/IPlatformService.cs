using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using GnollHackX.Performance;

#if GNH_MAUI
#else
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    public interface IPlatformService
    {
        void InitializePlatform();
        void EnsureWindowFocus();
        void CloseApplication();
        void HideKeyboard();
        void SetAdjustResize(bool adjustResize);
        float GetPlatformScreenScale();
        Task<Stream> GetPlatformAssetsStreamAsync(string directory, string fileName);
        bool IsRunningOnDesktop();

        string GetVersionString();
        ulong GetUsedMemoryInBytes();
        ulong GetDeviceMemoryInBytes();
        ulong GetDeviceFreeDiskSpaceInBytes();
        ulong GetDeviceTotalDiskSpaceInBytes();
        void SetStatusBarHidden(bool ishidden);
        bool GetStatusBarHidden();
        void HideOsNavigationBar();
        void ShowOsNavigationBar();
        void CollectGarbage();
        bool GetKeyboardConnected();

        float GetAnimatorDurationScaleSetting();
        float GetTransitionAnimationScaleSetting();
        float GetWindowAnimationScaleSetting();
        bool IsRemoveAnimationsOn();
        float GetCurrentAnimatorDurationScale();
        void OverrideAnimatorDuration();
        void RevertAnimatorDuration(bool isfinal);
        string GetBaseUrl();
        string GetAssetsPath();
        string GetCanonicalPath(string fileName);
        string GetAbsoluteOnDemandAssetPath(string assetPack);
        string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath);
        Task RequestAppReview(ContentPage page);
        int FetchOnDemandPack(string pack);
        event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification;

        /* Thermal and power state for performance runs; never throws, Unknown when unsupported */
        GHThermalReading GetThermalReading();
        /* Android sustained performance mode; false where unsupported */
        bool SetSustainedPerformanceMode(bool enabled);

        /* One system load sample; NaN or -1 fields where unsupported. Never throws.
           Called about once a second from a background thread. */
        bool TryGetSystemLoadSample(ref GHSystemLoadSample sample);
        /* Per-process CPU and GPU averaged between the begin and end collects; begin=true
           starts an interval and returns no rows. Background or post-window thread only.
           Never throws; false where unsupported. */
        bool TryCollectProcessInterval(bool begin, List<GHProcessLoad> rows, out float otherGpuPct);
        /* Adds os.*, driver.*, hardware.*, settings.power* and platform component.* keys.
           May be slow on first call (WMI); results are cached, refresh=true re-reads. */
        void AddEnvironmentFingerprint(Dictionary<string, string> fingerprint, bool refresh);
    }
}

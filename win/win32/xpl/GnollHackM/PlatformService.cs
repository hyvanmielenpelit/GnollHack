using GnollHackX;
using GnollHackX.Performance;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if GNH_AVALONIA
namespace GnollHackM
{
    /* Platform service of the Avalonia desktop target (Linux; also Windows through Avalonia Win32) */
    public class PlatformService : IPlatformService
    {
        public event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification { add { } remove { } }

        public Task<Stream> GetPlatformAssetsStreamAsync(string directory, string fileName)
        {
            return Task.FromResult<Stream>(File.OpenRead(Path.Combine(GHAvaloniaPaths.AssetDirectory, directory, fileName)));
        }

        public void CloseApplication()
        {
            Avalonia.Threading.Dispatcher.UIThread.Post(() =>
            {
                (Avalonia.Application.Current?.ApplicationLifetime as Avalonia.Controls.ApplicationLifetimes.IClassicDesktopStyleApplicationLifetime)?.Shutdown();
            });
        }

        public float GetPlatformScreenScale()
        {
            return 1.0f;
        }

        public int FetchOnDemandPack(string pack)
        {
            return -2; /* No need to load */
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack)
        {
            return null;
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath)
        {
            return null;
        }

        public float GetAnimatorDurationScaleSetting()
        {
            return 1.0f;
        }

        public string GetAssetsPath()
        {
            return GHAvaloniaPaths.AssetDirectory;
        }

        public string GetBaseUrl()
        {
            return Path.Combine(GHAvaloniaPaths.AssetDirectory, "Assets");
        }

        public float GetCurrentAnimatorDurationScale()
        {
            return 1.0f;
        }

        public ulong GetDeviceFreeDiskSpaceInBytes()
        {
            try
            {
                DriveInfo drive = new DriveInfo(GHAvaloniaPaths.DataDirectory);
                return (ulong)drive.AvailableFreeSpace;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetUsedMemoryInBytes()
        {
            try
            {
                return (ulong)Environment.WorkingSet;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceMemoryInBytes()
        {
            try
            {
                return (ulong)GC.GetGCMemoryInfo().TotalAvailableMemoryBytes;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceTotalDiskSpaceInBytes()
        {
            try
            {
                DriveInfo drive = new DriveInfo(GHAvaloniaPaths.DataDirectory);
                return (ulong)drive.TotalSize;
            }
            catch
            {
                return 0;
            }
        }

        public bool GetStatusBarHidden()
        {
            return true;
        }

        public float GetTransitionAnimationScaleSetting()
        {
            return 1.0f;
        }

        public string GetVersionString()
        {
            try
            {
                System.Reflection.Assembly entry = System.Reflection.Assembly.GetEntryAssembly();
                System.Reflection.AssemblyInformationalVersionAttribute attr = entry == null ? null :
                    (System.Reflection.AssemblyInformationalVersionAttribute)Attribute.GetCustomAttribute(entry, typeof(System.Reflection.AssemblyInformationalVersionAttribute));
                return attr?.InformationalVersion ?? "";
            }
            catch
            {
                return "";
            }
        }

        public bool IsRunningOnDesktop()
        {
            return true;
        }

        public float GetWindowAnimationScaleSetting()
        {
            return 1.0f;
        }

        public void InitializePlatform()
        {

        }

        public void EnsureWindowFocus()
        {

        }

        public bool IsRemoveAnimationsOn()
        {
            return false;
        }

        public void OverrideAnimatorDuration()
        {

        }

        public Task RequestAppReview(ContentPage page)
        {
            return Task.CompletedTask;
        }

        public void RevertAnimatorDuration(bool isfinal)
        {

        }

        public void SetStatusBarHidden(bool ishidden)
        {

        }

        public string GetCanonicalPath(string fileName)
        {
            try
            {
                return Path.GetFullPath(fileName);
            }
            catch
            {
                return fileName;
            }
        }
        public void HideKeyboard()
        {

        }
        public void SetAdjustResize(bool adjustResize)
        {

        }
        public void HideOsNavigationBar()
        {

        }
        public void ShowOsNavigationBar()
        {

        }
        public void CollectGarbage()
        {
            GC.Collect();
        }
        public bool GetKeyboardConnected()
        {
            return true;
        }
        public GHThermalReading GetThermalReading()
        {
            return GHThermalProbe.Unknown;
        }
        public bool SetSustainedPerformanceMode(bool enabled)
        {
            return false;
        }
        public bool TryGetSystemLoadSample(ref GHSystemLoadSample sample)
        {
            return false;
        }
        public bool TryCollectProcessInterval(bool begin, List<GHProcessLoad> rows, out float otherGpuPct)
        {
            otherGpuPct = float.NaN;
            return false;
        }
        public void AddEnvironmentFingerprint(Dictionary<string, string> fingerprint, bool refresh)
        {
        }
    }
}
#endif

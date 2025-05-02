#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
using GnollHackX;
using System.Runtime.Intrinsics.Arm;
using Windows.Services.Store;
using Windows.System;
using System.IO;

namespace GnollHackM
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    internal class MEMORYSTATUSEX
    {
        public uint dwLength;
        public uint dwMemoryLoad;
        public ulong ullTotalPhys;
        public ulong ullAvailPhys;
        public ulong ullTotalPageFile;
        public ulong ullAvailPageFile;
        public ulong ullTotalVirtual;
        public ulong ullAvailVirtual;
        public ulong ullAvailExtendedVirtual;

        public MEMORYSTATUSEX()
        {
            dwLength = (uint)Marshal.SizeOf(typeof(MEMORYSTATUSEX));
        }
    }

    public class PlatformService : IPlatformService
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GlobalMemoryStatusEx([In, Out] MEMORYSTATUSEX lpBuffer);

        private MemoryStatus GetMemoryStatus()
        {
            MemoryStatus res = new MemoryStatus();
            MEMORYSTATUSEX memStatusEx = new MEMORYSTATUSEX();
            if (GlobalMemoryStatusEx(memStatusEx))
            {
                res.TotalPhysical = memStatusEx.ullTotalPhys;
                res.AvailablePhysical = memStatusEx.ullAvailPhys;
                res.TotalPageFile = memStatusEx.ullTotalPageFile;
                res.AvailablePageFile = memStatusEx.ullAvailPageFile;
                res.TotalVirtual = memStatusEx.ullTotalVirtual;
                res.AvailableVirtual = memStatusEx.ullAvailVirtual;
                res.AvailableExtendedVirtual = memStatusEx.ullAvailExtendedVirtual;
            }

            return res;
        }


        public string GetVersionString()
        {

            return DeviceInfo.Current.VersionString;
        }

        public bool IsRunningOnDesktop()
        {
            return true;
        }

        public ulong GetDeviceMemoryInBytes()
        {
            try
            {
                //var ci = new Microsoft.VisualBasic.Devices.ComputerInfo();
                //return ci.TotalPhysicalMemory;
                MemoryStatus memoryStatus = GetMemoryStatus();
                return memoryStatus.TotalPhysical;
            }
            catch
            {
                return 0;
            }
        }

        public ulong GetDeviceFreeDiskSpaceInBytes()
        {
            try
            {
                string curDir = Directory.GetCurrentDirectory();
                DirectoryInfo di = new DirectoryInfo(curDir);
                DriveInfo drive = new DriveInfo(di.Root.FullName);
                ulong freeSize = (ulong)drive.AvailableFreeSpace;
                return freeSize;
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
                string curDir = Directory.GetCurrentDirectory();
                DirectoryInfo di = new DirectoryInfo(curDir);
                DriveInfo drive = new DriveInfo(di.Root.FullName);
                ulong totalSize = (ulong)drive.TotalSize;
                return totalSize;
            }
            catch
            {
                return 0;
            }
        }

        public float GetPlatformScreenScale()
        {
            try
            {
                return 1.0f;
            }
            catch
            {
                return 1.0f;
            }
        }

        public void CloseApplication()
        {
            RevertAnimatorDuration(true);
            GHApp.SaveWindowPosition();
            if (GHApp.WindowsApp != null)
            {
                GHApp.WindowsApp?.Exit();
                GHApp.WindowsApp = null;
            }
            Application.Current?.Quit();
            Environment.Exit(0);
        }

        public Task<Stream> GetPlatformAssetsStreamAsync(string directory, string fileName)
        {
            string relativePath = string.IsNullOrEmpty(directory) ? fileName : Path.Combine(directory, fileName);
            return FileSystem.Current.OpenAppPackageFileAsync(relativePath);
        }

        public void SetStatusBarHidden(bool ishidden)
        {

        }

        public bool GetStatusBarHidden()
        {
            return true;
        }

        public float GetAnimatorDurationScaleSetting()
        {
            return 1.0f;
        }
        public float GetCurrentAnimatorDurationScale()
        {
            return 1.0f;
        }

        public float GetTransitionAnimationScaleSetting()
        {
            return 1.0f;
        }

        public float GetWindowAnimationScaleSetting()
        {
            return 1.0f;
        }

        public bool IsRemoveAnimationsOn()
        {
            var scale1 = GetAnimatorDurationScaleSetting();
            var scale2 = GetTransitionAnimationScaleSetting();
            var scale3 = GetWindowAnimationScaleSetting();

            return scale1 == 0 && scale2 == 0 && scale3 == 0;
        }
        public void OverrideAnimatorDuration()
        {

        }
        public void RevertAnimatorDuration(bool isfinal)
        {

        }

        public async void RequestAppReview(ContentPage page)
        {
            if (GHApp.IsPackaged) /* Microsoft Store */
            {
                try
                {
                    await PromptUserToRateApp(page);
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine(ex.Message);
                }
                await System.Threading.Tasks.Task.Delay(50);
            }
        }


        private StoreContext _storeContext;

        public void InitializeStoreReview()
        {
            try
            {
                _storeContext = StoreContext.GetDefault();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        private async Task PromptUserToRateApp(ContentPage page)
        {
            if (_storeContext == null)
                InitializeStoreReview();

            if (_storeContext == null || GHApp.WindowsXamlWindow == null)
                return;

            try
            {
                var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(GHApp.WindowsXamlWindow);
                WinRT.Interop.InitializeWithWindow.Initialize(_storeContext, hwnd);

                StoreRateAndReviewResult result = await _storeContext.RequestRateAndReviewAppAsync();
                if (result != null)
                {
                    switch (result.Status)
                    {
                        case StoreRateAndReviewStatus.Succeeded:
                            // Was this an updated review or a new review, if Updated is false it means it was a users first time reviewing
                            if (result.WasUpdated)
                            {
                                // This was an updated review thank user
                            }
                            else
                            {
                                // This was a new review, thank user for reviewing and give some free in app tokens
                            }
                            // Keep track that we prompted user and don’t do it again for a while
                            break;

                        case StoreRateAndReviewStatus.CanceledByUser:
                            // Keep track that we prompted user and don’t prompt again for a while
                            break;

                        case StoreRateAndReviewStatus.NetworkError:
                            // User is probably not connected, so we’ll try again, but keep track so we don’t try too often
                            break;

                        // Something else went wrong
                        case StoreRateAndReviewStatus.Error:
                        default:
                            if (result.ExtendedError?.Message != null)
                                System.Diagnostics.Debug.WriteLine(result.ExtendedError.Message);
                            if (result.ExtendedJsonData != null)
                                System.Diagnostics.Debug.WriteLine(result.ExtendedJsonData);
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        private string GetAssemblyDirectory()
        {
            try
            {
                string assemblyFullPath = System.Reflection.Assembly.GetExecutingAssembly().Location;
                string assemblyDirectory = Path.GetDirectoryName(assemblyFullPath);
                return assemblyDirectory;
            }
            catch (Exception)
            {
                return ".";
            }
        }

        public string GetBaseUrl()
        {
            return Path.Combine(GetAssemblyDirectory(), "Assets");
        }
        public string GetAssetsPath()
        {
            return GetAssemblyDirectory();
        }

        public string GetCanonicalPath(string fileName)
        {
            try
            {
                return Path.Combine(GetAssemblyDirectory(), fileName);
            }
            catch (Exception)
            {
                return fileName;
            }
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack)
        {
            return null;
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath)
        {
            return null;
        }

        public int FetchOnDemandPack(string pack)
        {
            return -2; /* No need to load */
        }

        public event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification;

        private void OnDemandPackStatusNotified(object sender, AssetPackStatusEventArgs e)
        {
            OnDemandPackStatusNotification?.Invoke(this, e);
        }

        public void InitializePlatform()
        {

        }

        public void HideKeyboard()
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

        }
        public bool GetKeyboardConnected()
        {
            return true;
        }

    }

    public class MemoryStatus
    {
        /// <summary>
        /// The amount of actual physical memory, in bytes.
        /// </summary>
        public ulong TotalPhysical { get; set; }

        /// <summary>
        /// The amount of physical memory currently available, in bytes. 
        /// This is the amount of physical memory that can be immediately reused without having to write its contents to disk first. 
        /// It is the sum of the size of the standby, free, and zero lists.
        /// </summary>
        public ulong AvailablePhysical { get; set; }

        /// <summary>
        /// The current committed memory limit for the system or the current process, whichever is smaller, in bytes.
        /// </summary>
        public ulong TotalPageFile { get; set; }

        /// <summary>
        /// The maximum amount of memory the current process can commit, in bytes. 
        /// This value is equal to or smaller than the system-wide available commit value.
        /// </summary>
        public ulong AvailablePageFile { get; set; }

        /// <summary>
        /// The size of the user-mode portion of the virtual address space of the calling process, in bytes. 
        /// This value depends on the type of process, the type of processor, and the configuration of the operating system.
        /// </summary>
        public ulong TotalVirtual { get; set; }

        /// <summary>
        /// The amount of unreserved and uncommitted memory currently in the user-mode portion of the virtual address space of the calling process, in bytes.
        /// </summary>
        public ulong AvailableVirtual { get; set; }

        /// <summary>
        /// Reserved. This value is always 0.
        /// </summary>
        public ulong AvailableExtendedVirtual { get; set; }

        /// <summary>
        /// Write all property values to a string
        /// </summary>
        /// <returns>Each property on a new line</returns>
        public override string ToString()
        {
            return
                "TotalPhysical: " + TotalPhysical + Environment.NewLine +
                "AvailablePhysical: " + AvailablePhysical + Environment.NewLine +
                "TotalPageFile: " + TotalPageFile + Environment.NewLine +
                "AvailablePageFile: " + AvailablePageFile + Environment.NewLine +
                "TotalVirtual: " + TotalVirtual + Environment.NewLine +
                "AvailableVirtual: " + AvailableVirtual + Environment.NewLine +
                "AvailableExtendedVirtual: " + AvailableExtendedVirtual + Environment.NewLine;
        }
    }
}
#endif
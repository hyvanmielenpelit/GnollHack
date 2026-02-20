using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
#if GNH_MAUI
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
#endif
using System.Runtime.InteropServices;
using GnollHackX;

using Foundation;
using UIKit;
using StoreKit;
using GameController;
using System.IO;
using System.Threading.Tasks;

#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.iOS.PlatformService))]
namespace GnollHackX.iOS
#endif
{
    public class PlatformService : IPlatformService
    {
        public string GetVersionString()
        {
            NSObject ver = NSBundle.MainBundle.InfoDictionary["CFBundleShortVersionString"];
            return ver.ToString();
        }

        public bool IsRunningOnDesktop()
        {
            try
            {
                return NSProcessInfo.ProcessInfo?.IsiOSApplicationOnMac ?? false;
            }
            catch 
            { 
                return false;
            }
        }

        private const int TASK_VM_INFO = 22;

        [StructLayout(LayoutKind.Sequential)]
        struct task_vm_info_data_t
        {
            public ulong virtual_size;
            public ulong region_count;
            public ulong page_size;
            public ulong resident_size;
            public ulong resident_size_peak;
            public ulong device;
            public ulong device_peak;
            public ulong internal_bytes;
            public ulong internal_peak;
            public ulong external_bytes;
            public ulong external_peak;
            public ulong reusable;
            public ulong reusable_peak;
            public ulong purgeable_volatile_pmap;
            public ulong purgeable_volatile_resident;
            public ulong purgeable_volatile_virtual;
            public ulong compressed;
            public ulong compressed_peak;
            public ulong compressed_lifetime;
        }

        [DllImport("/usr/lib/libSystem.dylib")]
        private static extern int mach_task_self();

        [DllImport("/usr/lib/libSystem.dylib")]
        private static extern int task_info(
            int task,
            int flavor,
            ref task_vm_info_data_t info,
            ref uint count);

        public static ulong GetMemory()
        {
            var info = new task_vm_info_data_t();
            uint count = (uint)(Marshal.SizeOf(info) / sizeof(int));

            int result = task_info(
                mach_task_self(),
                TASK_VM_INFO,
                ref info,
                ref count);

            if (result != 0)
                return 0;

            return info.resident_size;
        }

        public ulong GetUsedMemoryInBytes()
        {
            try
            {
                return GetMemory();
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
                NSProcessInfo info = NSProcessInfo.ProcessInfo;
                ulong memory = info.PhysicalMemory;
                return memory;
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
                ulong freesize = NSFileManager.DefaultManager.GetFileSystemAttributes(Environment.GetFolderPath(Environment.SpecialFolder.Personal)).FreeSize;
                return freesize;
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
                ulong totalsize = NSFileManager.DefaultManager.GetFileSystemAttributes(Environment.GetFolderPath(Environment.SpecialFolder.Personal)).Size;
                return totalsize;
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
                //var nativeScale = UIKit.UIScreen.MainScreen.NativeScale;
                //var scale = UIKit.UIScreen.MainScreen.Scale;

                //if (nativeScale == scale || nativeScale == 0 || scale == 0)
                //    return 1.0f;

                //float platformScale = (float)(scale / nativeScale);
                //return platformScale;
                return 1.0f;
            }
            catch
            {
                return 1.0f;
            }
        }

        public void CloseApplication()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                GHApp.AddSentryBreadcrumb("CloseApplication", GHConstants.SentryGnollHackGeneralCategoryName);
                RevertAnimatorDuration(true);
#if GNH_MAUI
                Environment.Exit(0);
#else
                /* Do nothing; fall back to Xamarin.Forms termination after this call */
#endif
            });
        }

        public Task<Stream> GetPlatformAssetsStreamAsync(string directory, string fileName)
        {
            return Task.Run(() => GetPlatformAssetsStream(directory, fileName));
        }

        private Stream GetPlatformAssetsStream(string directory, string fileName)
        {
            string extension = Path.GetExtension(fileName);
            if (extension != null && extension.Length > 0)
                extension = extension.Substring(1); /* Remove . from the start */
            string fname = Path.GetFileNameWithoutExtension(fileName);
            string fullFilePath = NSBundle.MainBundle.PathForResource(fname, extension, directory);
            return File.OpenRead(fullFilePath);
        }

        public void SetStatusBarHidden(bool ishidden)
        {
            UIApplication.SharedApplication.StatusBarHidden = ishidden;
        }

        public bool GetStatusBarHidden()
        {
            return UIApplication.SharedApplication.StatusBarHidden;
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

        public async Task RequestAppReview(ContentPage page)
        {
            try
            {
                bool IsiOS10_3 = UIDevice.CurrentDevice.CheckSystemVersion(10, 3);
                bool IsiOS14_0 = UIDevice.CurrentDevice.CheckSystemVersion(14, 0);
                if (IsiOS10_3)
                {
                    if (IsiOS14_0)
                    {
                        UIWindow window = UIApplication.SharedApplication?.Delegate?.GetWindow();
                        if (window != null)
                        {
                            UIWindowScene windowscene = window.WindowScene;
                            if (windowscene != null)
                            {
                                SKStoreReviewController.RequestReview(window.WindowScene);
                            }
                        }
                    }
                    else
                    {
                        SKStoreReviewController.RequestReview();
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
                //await page.DisplayAlert("App Review Exception", "Exception occurred in App Review: " + ex.Message, "OK");
            }
            await System.Threading.Tasks.Task.Delay(50);
        }

        public string GetBaseUrl()
        {
            return NSBundle.MainBundle.BundlePath;
        }
        public string GetAssetsPath()
        {
            return NSBundle.MainBundle.BundlePath;
        }

        public string GetCanonicalPath(string fileName)
        {
            try
            {
                NSFileManager fm = new NSFileManager();
                string destination = fm.GetSymbolicLinkDestination(fileName, out var error);
                return destination;
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
            UIApplication.SharedApplication.KeyWindow.EndEditing(true);
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
            return GCKeyboard.CoalescedKeyboard != null;
        }
    }
}
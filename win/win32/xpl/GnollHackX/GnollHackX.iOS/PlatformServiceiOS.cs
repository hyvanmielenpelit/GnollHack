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

        public void CloseApplication()
        {
            RevertAnimatorDuration(true);
#if GNH_MAUI
            Environment.Exit(0);
#else
            /* Do nothing; fall back to Xamarin.Forms termination after this call */
#endif
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

        public async void RequestAppReview(ContentPage page)
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

        //private void OnDemandPackStatusNotified(object sender, AssetPackStatusEventArgs e)
        //{
        //    OnDemandPackStatusNotification?.Invoke(this, e);
        //}

        public void InitializePlatform()
        {

        }


        public void HideKeyboard()
        {
            UIApplication.SharedApplication.KeyWindow.EndEditing(true);
        }
    }
}
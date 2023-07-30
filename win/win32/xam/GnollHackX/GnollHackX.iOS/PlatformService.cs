using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xamarin.Forms;
using System.Runtime.InteropServices;
using GnollHackX;

using Foundation;
using UIKit;
using Xamarin.Forms.PlatformConfiguration;
using StoreKit;

[assembly: Dependency(typeof(GnollHackX.iOS.PlatformService))]
namespace GnollHackX.iOS
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
            //MainActivity.CurrentMainActivity.Finish();
        }

        public void SetStatusBarHidden(bool ishidden)
        {
            UIApplication.SharedApplication.StatusBarHidden = ishidden;
        }

        public bool GetStatusBarHidden()
        {
            return UIApplication.SharedApplication.StatusBarHidden;
        }

        //public void SaveFileToDownloads(byte[] data, string name)
        //{
        //if (MainActivity.CurrentMainActivity?.CheckSelfPermission(Manifest.Permission.WriteExternalStorage) != Android.Content.PM.Permission.Granted)
        //{
        //    MainActivity.CurrentMainActivity?.RequestPermissions(new string[] { Manifest.Permission.WriteExternalStorage }, 0);
        //}

        //if (MainActivity.CurrentMainActivity?.CheckSelfPermission(Manifest.Permission.WriteExternalStorage) == Android.Content.PM.Permission.Granted)
        //{
        //    string path = Android.OS.Environment.GetExternalStoragePublicDirectory(Android.OS.Environment.DirectoryDownloads).AbsolutePath;
        //    string filePath = System.IO.Path.Combine(path, name);

        //    if (System.IO.File.Exists(filePath))
        //    {
        //        System.IO.FileInfo file = new System.IO.FileInfo(filePath);
        //        file.Delete();
        //    }
        //    FileOutputStream fileOutputStream = new FileOutputStream(new Java.IO.File(filePath));
        //    fileOutputStream.Write(data);
        //    fileOutputStream.Close();
        //}
        //}

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

        public void RequestAppReview()
        {
            try
            {
                var myv = UIDevice.CurrentDevice.CheckSystemVersion(14, 0);
                if (myv)
                {
                    UIWindow window = UIApplication.SharedApplication.Delegate.GetWindow();
                    if (window != null)
                    {
                        UIWindowScene windowscene = window.WindowScene;
                        if (windowscene != null)
                            SKStoreReviewController.RequestReview(window.WindowScene);
                    }
                }
                else
                {
                    SKStoreReviewController.RequestReview();
                }
            }
            catch
            {
                //Error
            }
        }

        public string GetBaseUrl()
        {
            return NSBundle.MainBundle.BundlePath;
        }
        public string GetAssetsPath()
        {
            return NSBundle.MainBundle.BundlePath;
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

        public void InitOnDemandPackStatusNotificationEventHandler()
        {
            //Add OnDemandPackStatusNotified to relevant event to iOS somewhere
        }

        private void OnDemandPackStatusNotified(object sender, AssetPackStatusEventArgs e)
        {
            OnDemandPackStatusNotification?.Invoke(this, e);
        }
    }
}
#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
using GnollHackX;

namespace GnollHackM
{
    public class PlatformService : IPlatformService
    {
        public string GetVersionString()
        {

            return DeviceInfo.Current.VersionString;
        }

        public ulong GetDeviceMemoryInBytes()
        {
            try
            {
                var ci = new Microsoft.VisualBasic.Devices.ComputerInfo();
                return ci.TotalPhysicalMemory;
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

        public void CloseApplication()
        {
            RevertAnimatorDuration(true);
            Environment.Exit(0);
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
            try
            {

            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
            await System.Threading.Tasks.Task.Delay(50);
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

        //private void OnDemandPackStatusNotified(object sender, AssetPackStatusEventArgs e)
        //{
        //    OnDemandPackStatusNotification?.Invoke(this, e);
        //}

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
    }
}
#endif
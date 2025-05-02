using GnollHackX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if !(__ANDROID__ || __IOS__ || WINDOWS)
namespace GnollHackM
{
    public class PlatformService : IPlatformService
    {
        public event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification;

        public Task<Stream> GetPlatformAssetsStreamAsync(string directory, string fileName)
        {
            throw new NotImplementedException();
        }

        public void CloseApplication()
        {
            throw new NotImplementedException();
        }

        public int FetchOnDemandPack(string pack)
        {
            throw new NotImplementedException();
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack)
        {
            throw new NotImplementedException();
        }

        public string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath)
        {
            throw new NotImplementedException();
        }

        public float GetAnimatorDurationScaleSetting()
        {
            throw new NotImplementedException();
        }

        public string GetAssetsPath()
        {
            throw new NotImplementedException();
        }

        public string GetBaseUrl()
        {
            throw new NotImplementedException();
        }

        public float GetCurrentAnimatorDurationScale()
        {
            throw new NotImplementedException();
        }

        public ulong GetDeviceFreeDiskSpaceInBytes()
        {
            throw new NotImplementedException();
        }

        public ulong GetDeviceMemoryInBytes()
        {
            throw new NotImplementedException();
        }

        public ulong GetDeviceTotalDiskSpaceInBytes()
        {
            throw new NotImplementedException();
        }

        public bool GetStatusBarHidden()
        {
            throw new NotImplementedException();
        }

        public float GetTransitionAnimationScaleSetting()
        {
            throw new NotImplementedException();
        }

        public string GetVersionString()
        {
            throw new NotImplementedException();
        }

        public bool IsRunningOnDesktop()
        {
            throw new NotImplementedException();
        }

        public float GetWindowAnimationScaleSetting()
        {
            throw new NotImplementedException();
        }

        public void InitializePlatform()
        {
            throw new NotImplementedException();
        }

        public bool IsRemoveAnimationsOn()
        {
            throw new NotImplementedException();
        }

        public void OverrideAnimatorDuration()
        {
            throw new NotImplementedException();
        }

        public void RequestAppReview(ContentPage page)
        {
            throw new NotImplementedException();
        }

        public void RevertAnimatorDuration(bool isfinal)
        {
            throw new NotImplementedException();
        }

        public void SetStatusBarHidden(bool ishidden)
        {
            throw new NotImplementedException();
        }

        public string GetCanonicalPath(string fileName)
        {
            throw new NotImplementedException();
        }    
        public void HideKeyboard()
        {
            throw new NotImplementedException();
        }    
        public void HideOsNavigationBar()
        {
            throw new NotImplementedException();
        }
        public void ShowOsNavigationBar()
        {
            throw new NotImplementedException();
        }
        public void CollectGarbage()
        {
            throw new NotImplementedException();
        }
        public bool GetKeyboardConnected()
        {
            return false;
        }
    }
}
#endif

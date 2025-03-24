using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
#else
using Xamarin.Forms;
#endif

namespace GnollHackX
{
    public interface IPlatformService
    {
        void InitializePlatform();
        void CloseApplication();
        void HideKeyboard();
        float GetPlatformScreenScale();

        string GetVersionString();
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
        void RequestAppReview(ContentPage page);
        int FetchOnDemandPack(string pack);
        event EventHandler<AssetPackStatusEventArgs> OnDemandPackStatusNotification;
    }
}

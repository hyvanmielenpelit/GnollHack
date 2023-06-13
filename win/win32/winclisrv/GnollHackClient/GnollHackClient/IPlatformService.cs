using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IPlatformService
    {
        string GetVersionString();
        ulong GetDeviceMemoryInBytes();
        ulong GetDeviceFreeDiskSpaceInBytes();
        ulong GetDeviceTotalDiskSpaceInBytes();
        void CloseApplication();
        void SetStatusBarHidden(bool ishidden);
        bool GetStatusBarHidden();
        float GetAnimatorDurationScaleSetting();
        float GetTransitionAnimationScaleSetting();
        float GetWindowAnimationScaleSetting();
        bool IsRemoveAnimationsOn();
        float GetCurrentAnimatorDurationScale();
        void OverrideAnimatorDuration();
        void RevertAnimatorDuration(bool isfinal);
        string GetBaseUrl();
        string GetAssetsPath();

        string GetAbsoluteOnDemandAssetPath(string assetPack, string relativeAssetPath);
        int FetchOnDemandPack(string pack);

        //void SaveFileToDownloads(byte[] data, string name);
    }
}

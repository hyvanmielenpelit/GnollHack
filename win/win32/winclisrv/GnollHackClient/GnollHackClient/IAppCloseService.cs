using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IPlatformService
    {
        string GetVersionString();
        ulong GetDeviceMemoryInBytes();
        void CloseApplication();
        void SetStatusBarHidden(bool ishidden);
        bool GetStatusBarHidden();
        void OverrideAnimationDuration();
        void RevertAnimationDuration();

        //void SaveFileToDownloads(byte[] data, string name);
    }
}

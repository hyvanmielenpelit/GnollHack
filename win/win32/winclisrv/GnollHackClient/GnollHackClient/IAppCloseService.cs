using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IPlatformService
    {
        void CloseApplication();
        void SetStatusBarHidden(bool ishidden);
        bool GetStatusBarHidden();
        void SaveFileToDownloads(byte[] data, string name);
        void OverrideAnimationDuration();
        void RevertAnimationDuration();
    }
}

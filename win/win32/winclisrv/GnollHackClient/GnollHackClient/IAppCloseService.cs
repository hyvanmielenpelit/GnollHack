using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IPlatformService
    {
        void CloseApplication();
        bool IsModernAndroid();
        void SaveFileToDownloads(byte[] data, string name);
        void OverrideAnimationDuration();
        void RevertAnimationDuration();
    }
}

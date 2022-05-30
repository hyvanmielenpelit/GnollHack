using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IPlatformService
    {
        void CloseApplication();
        void SaveFileToDownloads(byte[] data, string name);
        void OverrideAnimationDuration();
        void RevertAnimationDuration();
    }
}

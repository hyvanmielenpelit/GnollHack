using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public interface IAppCloseService
    {
        void CloseApplication();
        bool IsModernAndroid();
    }
}

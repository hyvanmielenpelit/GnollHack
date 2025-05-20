using System;
using System.Collections.Generic;
using System.Text;

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX
#endif
{
    public interface ICloseablePage
    {
        void ClosePage();
    }
}

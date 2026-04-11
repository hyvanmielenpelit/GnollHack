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
    public interface IKeyPressHandlingPage
    {
        bool HandleKeyPress(int key, bool isCtrl, bool isMeta);
    }
    public interface ISpecialKeyPressHandlingPage
    {
        bool HandleSpecialKeyPress(GHSpecialKey key, bool isCtrl, bool isMeta, bool isShift);
    }
}

using System;
using System.Collections.Generic;
using System.Text;

#if GNH_MAUI
namespace GnollHackM
#else
using System.Threading.Tasks;

namespace GnollHackX
#endif
{
    public interface ICloseablePage
    {
        void ClosePage();
    }
    public interface IMessagePopupPage
    {
#if GNH_MAUI
        Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null, Microsoft.Maui.Graphics.Color titleColor = null);
#else
        Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null, Xamarin.Forms.Color? titleColor = null);
#endif
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

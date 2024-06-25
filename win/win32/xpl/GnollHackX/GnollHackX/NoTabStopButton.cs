using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    public class NoTabStopButton : Button
    {
        public NoTabStopButton() : base()
        {
#if WINDOWS
            HandlerChanged += (s, e) =>
            {
                if (Handler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                {
                    var platformView = Handler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                    platformView.IsTabStop = false;
                }
            };
#endif

        }
    }
}

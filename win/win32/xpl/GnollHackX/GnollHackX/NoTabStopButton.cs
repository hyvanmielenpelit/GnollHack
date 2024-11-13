using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
using Microsoft.Maui.Handlers;
using SkiaSharp.Views.Maui.Handlers;
#if ANDROID
using Google.Android.Material.Button;
#endif
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
                    if (platformView != null)
                    {
                        platformView.IsTabStop = false;
                    }
                }
            };
#endif

        }
    }

#if GNH_MAUI
    internal class NoFocusButtonHandler : ButtonHandler
    {
#if ANDROID
        protected override void ConnectHandler(MaterialButton platformView)
        {
            base.ConnectHandler(platformView);
            platformView.Focusable = false;
            platformView.SetBackgroundColor(Android.Graphics.Color.Transparent);
        }
#endif
    }
#endif
}

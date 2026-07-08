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
            Pressed += NoTabStopButton_Pressed;
            Released += NoTabStopButton_Released;
        }

        private bool _enableLongTap = false;
        public bool EnableLongTap { get { return _enableLongTap; } set { _enableLongTap = value; if (!value) IsLongTap = false; } }
        public TimeSpan LongTapDuration = new TimeSpan(0);

        private DateTime _pressStart = default;
        public DateTime PressStartTime => _pressStart;
        public bool IsLongTap { get; private set; }

        private void NoTabStopButton_Pressed(object sender, EventArgs e)
        {
            if (EnableLongTap)
            {
                IsLongTap = false;
                _pressStart = DateTime.UtcNow;
            }
        }
        private void NoTabStopButton_Released(object sender, EventArgs e)
        {
            if (EnableLongTap)
                IsLongTap = (DateTime.UtcNow - _pressStart) > LongTapDuration;
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

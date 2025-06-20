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
    public class PointableButton : Button
    {
        public PointableButton() : base()
        {
#if WINDOWS
            HandlerChanged += (s, e) => 
            {
                if (Handler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                {
                    var platformView = Handler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                    if (platformView != null)
                    {
                        platformView.PointerEntered += PlatformView_PointerEntered;
                        platformView.PointerExited += PlatformView_PointerExited;
                        platformView.PointerCanceled += PlatformView_PointerExited;
                        platformView.Background = new Microsoft.UI.Xaml.Media.SolidColorBrush();
                    }
                }
            };
            HandlerChanging += (s, e) =>
            {
                if(e.OldHandler != null && e.NewHandler == null)
                {
                    if (e.OldHandler?.PlatformView is Microsoft.UI.Xaml.Controls.Button)
                    {
                        var platformView = e.OldHandler?.PlatformView as Microsoft.UI.Xaml.Controls.Button;
                        if (platformView != null)
                        {
                            platformView.PointerEntered -= PlatformView_PointerEntered;
                            platformView.PointerExited -= PlatformView_PointerExited;
                            platformView.PointerCanceled -= PlatformView_PointerExited;
                        }
                    }
                }
            };
#endif
        }

#pragma warning disable 67
        public event EventHandler PointerEntered;
        public event EventHandler PointerExited;
#pragma warning restore 67

#if WINDOWS

        private void PlatformView_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if (PointerEntered != null)
            {
                e.Handled = true;
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    PointerEntered?.Invoke(sender, EventArgs.Empty);
                });
            }
        }

        private void PlatformView_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if(PointerExited != null)
            {
                e.Handled = true;
                MainThread.BeginInvokeOnMainThread(() =>
                {
                    PointerExited?.Invoke(sender, EventArgs.Empty);
                });
            }
        }
#endif
    }
}

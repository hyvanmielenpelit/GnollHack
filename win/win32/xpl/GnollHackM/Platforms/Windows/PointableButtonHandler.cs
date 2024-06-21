using Microsoft.Maui.Platform;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GnollHackM.Platforms.Windows
{
    public partial class PointableButtonHandler : Microsoft.Maui.Handlers.ButtonHandler
    {
        protected override void ConnectHandler(Microsoft.UI.Xaml.Controls.Button platformView)
        {
            base.ConnectHandler(platformView);
            platformView.PointerEntered += PlatformView_PointerEntered;
            platformView.PointerExited += PlatformView_PointerExited;
            platformView.Background = new Microsoft.UI.Xaml.Media.SolidColorBrush();
        }

        public event EventHandler PointerEntered;
        public event EventHandler PointerExited;

        private void PlatformView_PointerEntered(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            PointerEntered?.Invoke(sender, new EventArgs());
        }

        private void PlatformView_PointerExited(object sender, Microsoft.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            e.Handled = true;
            PointerExited?.Invoke(sender, new EventArgs());
        }

    }
}

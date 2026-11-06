#if GNH_AVALONIA
using Microsoft.Maui;
using Microsoft.Maui.Graphics;
using AcmButtonHandler = Avalonia.Controls.Maui.Handlers.ButtonHandler;

namespace GnollHackM
{
    /* Colors of Avalonia controls that ACM leaves to the Fluent theme, set as the WinUI build shows them */
    internal static class GHAvaloniaControlTheming
    {
        /* Before any handler is created */
        public static void Register()
        {
            AcmButtonHandler.Mapper.AppendToMapping(nameof(IView.Background), MapButtonDisabledBackground);
        }

        // WORKAROUND(WA-AVALONIA-0008): ACM leaves some control colors to the Fluent theme
        /* A disabled button's background in its own background, as MAUI's WinUI handler sets it. ACM sets the
           pointer-over and pressed backgrounds only, which left the Fluent theme's translucent disabled background
           over transparent buttons. Runs after ACM's mapping has set the platform background. */
        private static void MapButtonDisabledBackground(AcmButtonHandler handler, IButton button)
        {
            Avalonia.Controls.Button platformButton = ((IElementHandler)handler).PlatformView as Avalonia.Controls.Button;
            if (platformButton == null)
                return;

            Avalonia.Media.IBrush brush = button.Background == null ? null : platformButton.Background;
            if (brush == null)
                platformButton.Resources.Remove("ButtonBackgroundDisabled");
            else
                platformButton.Resources["ButtonBackgroundDisabled"] = brush;
        }

        internal static Avalonia.Media.IBrush ToBrush(Color color)
        {
            color.ToRgba(out byte r, out byte g, out byte b, out byte a);
            return new Avalonia.Media.SolidColorBrush(Avalonia.Media.Color.FromArgb(a, r, g, b));
        }
    }
}
#endif

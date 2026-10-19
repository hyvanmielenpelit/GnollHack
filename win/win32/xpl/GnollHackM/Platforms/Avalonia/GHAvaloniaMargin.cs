#if GNH_AVALONIA
using Microsoft.Maui;
using AcmViewHandler = Avalonia.Controls.Maui.Handlers.ViewHandler;

namespace GnollHackM
{
    /* MAUI margins on Avalonia. MAUI subtracts a view's margin from the constraints before asking its handler for
       a desired size, and arranges the view in a frame already inside the margin. ACM also copies the margin to
       the Avalonia control, whose Measure subtracts it a second time, so a view with a horizontal margin was
       measured too narrow (text wrapped, or vanished when the margin grew with the window). The Avalonia controls
       keep a zero margin: ACM then measures with MAUI's constraints and arranges in MAUI's frame. */
    internal static class GHAvaloniaMargin
    {
        /* Before any handler is created, while no mapper has cached its entries */
        public static void Register()
        {
            AcmViewHandler.ViewMapper.ReplaceMapping<IView, IViewHandler>(nameof(IView.Margin), MapMargin);
        }

        private static void MapMargin(IViewHandler handler, IView view)
        {
            Avalonia.Controls.Control container = handler.ContainerView as Avalonia.Controls.Control;
            if (container != null)
                container.Margin = default(Avalonia.Thickness);
            Avalonia.Controls.Control platformView = handler.PlatformView as Avalonia.Controls.Control;
            if (platformView != null)
                platformView.Margin = default(Avalonia.Thickness);
        }
    }
}
#endif

#if GNH_AVALONIA
using Microsoft.Maui;
using AcmLayoutHandler = Avalonia.Controls.Maui.Handlers.LayoutHandler;

namespace GnollHackM
{
    /* MAUI input transparency for layouts on Avalonia. An input-transparent layout lets pointer input
       through its own area; with CascadeInputTransparent its children let it through as well. */
    internal static class GHAvaloniaLayoutInput
    {
        public static void Register()
        {
            AcmLayoutHandler.Mapper.AppendToMapping(nameof(IView.InputTransparent), Apply);
            AcmLayoutHandler.Mapper.AppendToMapping(nameof(Microsoft.Maui.ILayout.Background), Apply);
        }

        private static void Apply(AcmLayoutHandler handler, Microsoft.Maui.ILayout layout)
        {
            Avalonia.Controls.Panel panel = ((IElementHandler)handler).PlatformView as Avalonia.Controls.Panel;
            if (panel == null || layout == null)
                return;

            bool cascade = (layout as Microsoft.Maui.Controls.Layout)?.CascadeInputTransparent ?? true;
            if (layout.InputTransparent && cascade)
            {
                panel.IsHitTestVisible = false;
                return;
            }

            panel.IsHitTestVisible = true;
            if (layout.InputTransparent)
                panel.Background = null; /* An Avalonia panel with any brush, even transparent, is hit-testable */
            else
                AcmLayoutHandler.MapBackground(handler, layout);
        }
    }
}
#endif

#if GNH_AVALONIA
using System.Linq;
using Avalonia.VisualTree;
using Microsoft.Maui;
using AcmScrollViewHandler = Avalonia.Controls.Maui.Handlers.ScrollViewHandler;

namespace GnollHackM
{
    /* Scroll bar space that the Avalonia layout, not MAUI's, decides on */
    internal static class GHAvaloniaAlignment
    {
        /* Before any handler is created */
        public static void Register()
        {
            AcmScrollViewHandler.Mapper.AppendToMapping(nameof(IContentView.Content), MapScrollBarSpace);
        }

        /* Room for the vertical scroll bar while the content scrolls vertically. The Fluent ScrollViewer shows
           auto-hiding scroll bars over its content, where the bar covered the controls at the edge (SettingsPage);
           padding the content by the bar's width keeps the auto-hide and leaves the bar its own space. */
        private const double DefaultScrollBarWidth = 12.0;

        private static void MapScrollBarSpace(AcmScrollViewHandler handler, IScrollView scrollView)
        {
            Avalonia.Controls.ScrollViewer scrollViewer = ((IElementHandler)handler).PlatformView as Avalonia.Controls.ScrollViewer;
            if (scrollViewer == null)
                return;

            scrollViewer.PropertyChanged -= OnScrollViewerPropertyChanged;
            scrollViewer.PropertyChanged += OnScrollViewerPropertyChanged;
            UpdateScrollBarSpace(scrollViewer);
        }

        private static void OnScrollViewerPropertyChanged(object sender, Avalonia.AvaloniaPropertyChangedEventArgs e)
        {
            if (e.Property == Avalonia.Controls.ScrollViewer.ExtentProperty
                || e.Property == Avalonia.Controls.ScrollViewer.ViewportProperty
                || e.Property == Avalonia.Controls.ScrollViewer.VerticalScrollBarVisibilityProperty)
            {
                UpdateScrollBarSpace((Avalonia.Controls.ScrollViewer)sender);
            }
        }

        private static void UpdateScrollBarSpace(Avalonia.Controls.ScrollViewer scrollViewer)
        {
            Avalonia.Controls.Primitives.ScrollBarVisibility visibility = scrollViewer.VerticalScrollBarVisibility;
            bool scrolls = (visibility == Avalonia.Controls.Primitives.ScrollBarVisibility.Auto && scrollViewer.Extent.Height > scrollViewer.Viewport.Height + 0.5)
                || visibility == Avalonia.Controls.Primitives.ScrollBarVisibility.Visible;

            double width = 0;
            if (scrolls)
            {
                foreach (Avalonia.Controls.Primitives.ScrollBar bar in scrollViewer.GetVisualDescendants().OfType<Avalonia.Controls.Primitives.ScrollBar>())
                {
                    if (bar.Orientation == Avalonia.Layout.Orientation.Vertical)
                    {
                        width = bar.Bounds.Width;
                        break;
                    }
                }
                if (width <= 0)
                    width = DefaultScrollBarWidth;
            }

            Avalonia.Thickness padding = new Avalonia.Thickness(0, 0, width, 0);
            if (scrollViewer.Padding != padding)
                scrollViewer.Padding = padding;
        }
    }
}
#endif

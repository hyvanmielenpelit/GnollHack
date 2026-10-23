#if GNH_AVALONIA
using System.ComponentModel;
using System.Linq;
using Avalonia.VisualTree;
using Microsoft.Maui;
using AcmCheckBoxHandler = Avalonia.Controls.Maui.Handlers.CheckBoxHandler;
using AcmScrollViewHandler = Avalonia.Controls.Maui.Handlers.ScrollViewHandler;
using MauiView = Microsoft.Maui.Controls.View;

namespace GnollHackM
{
    /* Alignments and scroll bar space that the Avalonia layout, not MAUI's, decides on */
    internal static class GHAvaloniaAlignment
    {
        /* Before any handler is created */
        public static void Register()
        {
            AcmScrollViewHandler.Mapper.AppendToMapping(nameof(IContentView.Content), MapScrollBarSpace);
            AcmScrollViewHandler.Mapper.AppendToMapping(nameof(IContentView.Content), MapScrollContent);
            AcmCheckBoxHandler.Mapper.AppendToMapping(nameof(ICheckBox.IsChecked), MapCheckBoxTemplate);
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

        // WORKAROUND(WA-AVALONIA-0006): Scroll view content keeps its first alignment
        /* ACM hands a scroll view's content to an Avalonia ScrollViewer, which aligns it by the Avalonia control's
           alignment. ACM sets that from the MAUI options only when the handler is created, and MAUI does not remap
           them when they change, so content whose options change later (VaultPage) keeps the original alignment. */
        private static void MapScrollContent(AcmScrollViewHandler handler, IScrollView scrollView)
        {
            MauiView content = scrollView.PresentedContent as MauiView;
            if (content == null)
                return;

            content.PropertyChanged -= OnScrollContentPropertyChanged;
            content.PropertyChanged += OnScrollContentPropertyChanged;
        }

        private static void OnScrollContentPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            MauiView content = sender as MauiView;
            if (content?.Handler == null)
                return;

            if (e.PropertyName == MauiView.VerticalOptionsProperty.PropertyName)
                content.Handler.UpdateValue(nameof(IView.VerticalLayoutAlignment));
            else if (e.PropertyName == MauiView.HorizontalOptionsProperty.PropertyName)
                content.Handler.UpdateValue(nameof(IView.HorizontalLayoutAlignment));
        }

        // WORKAROUND(WA-AVALONIA-0007): Check box sits above the center of its label
        /* The Fluent template keeps the box at the top of a CheckBoxMinHeight-high area, while ACM's check box is
           taller (MAUI's 44-unit minimum), so the box sat above the center of the label next to it */
        private static void MapCheckBoxTemplate(AcmCheckBoxHandler handler, ICheckBox checkBox)
        {
            Avalonia.Controls.CheckBox box = ((IElementHandler)handler).PlatformView as Avalonia.Controls.CheckBox;
            if (box == null)
                return;

            box.TemplateApplied -= OnCheckBoxTemplateApplied;
            box.TemplateApplied += OnCheckBoxTemplateApplied;
        }

        private static void OnCheckBoxTemplateApplied(object sender, Avalonia.Controls.Primitives.TemplateAppliedEventArgs e)
        {
            Avalonia.Controls.Border rectangle = e.NameScope.Find("NormalRectangle") as Avalonia.Controls.Border;
            Avalonia.Layout.Layoutable indicator = rectangle?.GetVisualParent() as Avalonia.Layout.Layoutable;
            if (indicator != null)
                indicator.VerticalAlignment = Avalonia.Layout.VerticalAlignment.Center;
        }
    }
}
#endif

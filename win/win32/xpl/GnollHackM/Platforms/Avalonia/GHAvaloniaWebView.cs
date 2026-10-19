#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Avalonia.VisualTree;
using Microsoft.Maui;
using AcmWebViewHandler = Avalonia.Controls.Maui.Handlers.WebViewHandler;

namespace GnollHackM
{
    /* Avalonia's NativeWebView, which ACM uses for a MAUI WebView */
    internal static class GHAvaloniaWebView
    {
        private static readonly Lazy<bool> _isAvailable = new Lazy<bool>(CheckAvailable);
        private static readonly ConditionalWeakTable<Avalonia.Controls.Control, PositionTracker> _trackers = new ConditionalWeakTable<Avalonia.Controls.Control, PositionTracker>();

        /* Whether the web view can show pages. On Linux it needs GTK 3 and WebKitGTK, which not every system has; on
           Windows it uses WebView2, as WinUI does. */
        public static bool IsAvailable
        {
            get { return _isAvailable.Value; }
        }

        /* Before any handler is created */
        public static void Register()
        {
            AcmWebViewHandler.Mapper.AppendToMapping(nameof(IWebView.Source), MapPositionTracking);
        }

        private static bool CheckAvailable()
        {
            if (!OperatingSystem.IsLinux())
                return true;

            /* The library names NativeWebView loads */
            return CanLoad("libgtk-3.so.0", "libgtk-3.so")
                && CanLoad("libwebkit2gtk-4.1.so.0", "libwebkit2gtk-4.1.so", "libwebkit2gtk-4.0.so.37", "libwebkit2gtk-4.0.so");
        }

        private static bool CanLoad(params string[] names)
        {
            foreach (string name in names)
            {
                IntPtr handle;
                if (NativeLibrary.TryLoad(name, out handle))
                {
                    NativeLibrary.Free(handle);
                    return true;
                }
            }
            return false;
        }

        private static void MapPositionTracking(AcmWebViewHandler handler, IWebView webView)
        {
            Avalonia.Controls.Control platformView = ((IElementHandler)handler).PlatformView as Avalonia.Controls.Control;
            if (platformView != null)
                _trackers.GetValue(platformView, view => new PositionTracker(view));
        }

        /* The web page is a native window that Avalonia's NativeControlHost moves when the bounds of the host or an
           ancestor change. Its position includes render transforms, but a transform change does not move it, so a
           page in a modal, which ACM slides in with a render transform, stayed where the slide was when it was
           placed. This moves it again whenever an ancestor's render transform changes, such as at the slide's end. */
        private sealed class PositionTracker
        {
            private readonly Avalonia.Controls.Control _view;
            private readonly List<Avalonia.Visual> _ancestors = new List<Avalonia.Visual>();
            private bool _updateQueued;

            public PositionTracker(Avalonia.Controls.Control view)
            {
                _view = view;
                _view.AttachedToVisualTree += (sender, e) => Subscribe();
                _view.DetachedFromVisualTree += (sender, e) => Unsubscribe();
                if (Avalonia.Controls.TopLevel.GetTopLevel(_view) != null)
                    Subscribe();
            }

            private void Subscribe()
            {
                Unsubscribe();
                for (Avalonia.Visual visual = _view; visual != null; visual = visual.GetVisualParent())
                {
                    visual.PropertyChanged += OnAncestorPropertyChanged;
                    _ancestors.Add(visual);
                }
            }

            private void Unsubscribe()
            {
                foreach (Avalonia.Visual visual in _ancestors)
                    visual.PropertyChanged -= OnAncestorPropertyChanged;
                _ancestors.Clear();
            }

            private void OnAncestorPropertyChanged(object sender, Avalonia.AvaloniaPropertyChangedEventArgs e)
            {
                if (e.Property != Avalonia.Visual.RenderTransformProperty || _updateQueued)
                    return;

                _updateQueued = true;
                Avalonia.Threading.Dispatcher.UIThread.Post(UpdatePosition, Avalonia.Threading.DispatcherPriority.Normal);
            }

            private void UpdatePosition()
            {
                _updateQueued = false;
                foreach (Avalonia.Controls.NativeControlHost host in _view.GetVisualDescendants().OfType<Avalonia.Controls.NativeControlHost>())
                    host.TryUpdateNativeControlPosition();
            }
        }
    }
}
#endif

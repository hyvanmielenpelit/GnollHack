#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using Avalonia.VisualTree;
using GnollHackX;
using Microsoft.Maui;
using AcmWebViewHandler = Avalonia.Controls.Maui.Handlers.WebViewHandler;

namespace GnollHackM
{
    /* Avalonia's NativeWebView, which ACM uses for a MAUI WebView: WebView2 on Windows, WebKitGTK on Linux */
    internal static class GHAvaloniaWebView
    {
        private static readonly Lazy<bool> _isAvailable = new Lazy<bool>(CheckAvailable);
        private static readonly ConditionalWeakTable<Avalonia.Controls.Control, PositionTracker> _trackers = new ConditionalWeakTable<Avalonia.Controls.Control, PositionTracker>();
        private static readonly ConditionalWeakTable<Avalonia.Controls.NativeWebView, object> _configuredViews = new ConditionalWeakTable<Avalonia.Controls.NativeWebView, object>();

        /* Whether the pinned adapter can show pages, from Avalonia's adapter information. On Linux WebKitGTK (4.1 or 4.0)
           must be installed, which not every system has; on Windows the WebView2 runtime, as for WinUI. */
        public static bool IsAvailable
        {
            get { return _isAvailable.Value; }
        }

        private static Avalonia.Platform.WebViewAdapterType PinnedAdapter
        {
            get { return OperatingSystem.IsLinux() ? Avalonia.Platform.WebViewAdapterType.WebKitGtk : Avalonia.Platform.WebViewAdapterType.WebView2; }
        }

        /* Before any handler is created */
        public static void Register()
        {
            AcmWebViewHandler.Mapper.AppendToMapping(nameof(IWebView.Source), MapPositionTracking);
            AcmWebViewHandler.Mapper.AppendToMapping(nameof(IWebView.Source), MapConfiguration);
        }

        private static bool CheckAvailable()
        {
            try
            {
                Avalonia.Platform.DetailedWebViewAdapterInfo info = Avalonia.Platform.WebViewAdapterInfo.GetAdapterInfo(PinnedAdapter);
                return info.IsSupported && info.IsInstalled;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWebView: cannot read the web view adapter information: " + ex.Message);
                return false;
            }
        }

        private static void MapPositionTracking(AcmWebViewHandler handler, IWebView webView)
        {
            Avalonia.Controls.Control platformView = ((IElementHandler)handler).PlatformView as Avalonia.Controls.Control;
            if (platformView != null)
                _trackers.GetValue(platformView, view => new PositionTracker(view));
        }

        /* Once per view, while the handler is created and before the view is attached, when NativeWebView reads its
           adapter preference and environment. WebKitGTK keeps its shared default context: data directories would give
           each view a web context of its own. */
        private static void MapConfiguration(AcmWebViewHandler handler, IWebView webView)
        {
            Avalonia.Controls.NativeWebView view = ((IElementHandler)handler).PlatformView as Avalonia.Controls.NativeWebView;
            if (view == null)
                return;

            object configured;
            if (_configuredViews.TryGetValue(view, out configured))
                return;
            _configuredViews.Add(view, null);

            view.AdapterPreference = new[] { PinnedAdapter };
            view.EnvironmentRequested += OnEnvironmentRequested;
        }

        private static void OnEnvironmentRequested(object sender, Avalonia.Controls.WebViewEnvironmentRequestedEventArgs e)
        {
            try
            {
                Avalonia.Platform.WindowsWebView2EnvironmentRequestedEventArgs webView2 = e as Avalonia.Platform.WindowsWebView2EnvironmentRequestedEventArgs;
                if (webView2 != null)
                {
                    /* WebView2's default is a folder next to the executable, which may not be writable */
                    string folder = Path.Combine(GHAvaloniaPaths.DataDirectory, "WebView2");
                    Directory.CreateDirectory(folder);
                    webView2.UserDataFolder = folder;
                    GHApp.MaybeWriteGHLog("GHAvaloniaWebView: WebView2 user data folder " + folder);
                }
                else
                {
                    GHApp.MaybeWriteGHLog("GHAvaloniaWebView: web view environment " + e.GetType().Name);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWebView: cannot configure the web view environment: " + ex.Message);
            }
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

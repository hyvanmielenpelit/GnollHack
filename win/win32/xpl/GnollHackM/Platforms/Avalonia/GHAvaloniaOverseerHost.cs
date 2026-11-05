#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using GnollHackX;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace GnollHackM
{
    /* A web view host for the Overseer page: Avalonia's NativeWebView, or on Linux the CEF control */
    internal interface IGHAvaloniaWebHost
    {
        /* Whether SPA messages other than the host's own reach the bridge callback */
        bool BridgeEnabled { get; set; }
        string DocumentUrl { get; }
        bool IgnoresNavigationCancel { get; }
        /* Whether the page is a native window drawn above all Avalonia content */
        bool DrawsAboveAvalonia { get; }
        void SetBackground(byte r, byte g, byte b);
        Task InvokeScriptAsync(string script);
        void OnPageLoaded();
        bool GoBack();
        void Detach();
    }

    /* The Overseer page's web view host, following the MAUI web view's current handler. Every member is a no-op while
       no view is attached. UI thread only. */
    internal sealed class GHAvaloniaOverseerHost
    {
        private readonly Microsoft.Maui.Controls.WebView _webView;
        private readonly Action<string, string> _bridgeMessageReceived;
        private readonly Action _escapePressed;
        private readonly Action<Uri> _newWindowRequested;
        private object _platformView;
        private IGHAvaloniaWebHost _host;
        private bool _bridgeEnabled;

        /* bridgeMessageReceived gets the message and the sending frame's URL, or null when the host cannot tell */
        public GHAvaloniaOverseerHost(Microsoft.Maui.Controls.WebView webView,
            Action<string, string> bridgeMessageReceived, Action escapePressed, Action<Uri> newWindowRequested)
        {
            _webView = webView;
            _bridgeMessageReceived = bridgeMessageReceived;
            _escapePressed = escapePressed;
            _newWindowRequested = newWindowRequested;
        }

        public bool BridgeEnabled
        {
            get { return _bridgeEnabled; }
            set
            {
                _bridgeEnabled = value;
                if (_host != null)
                    _host.BridgeEnabled = value;
            }
        }

        public string DocumentUrl
        {
            get { return _host?.DocumentUrl; }
        }

        public bool IgnoresNavigationCancel
        {
            get { return _host?.IgnoresNavigationCancel ?? false; }
        }

        /* True until a view is attached, so that an unknown view is treated as a native window */
        public bool DrawsAboveAvalonia
        {
            get { return _host?.DrawsAboveAvalonia ?? true; }
        }

        /* Follows the MAUI web view's current handler */
        public void Attach()
        {
            try
            {
                object view = _webView.Handler?.PlatformView;
                if (view == null)
                {
                    Detach();
                    return;
                }
                if (ReferenceEquals(view, _platformView))
                    return;

                Detach();
                Avalonia.Controls.NativeWebView nativeWebView = view as Avalonia.Controls.NativeWebView;
                if (nativeWebView != null)
                    _host = new NativeWebViewHost(nativeWebView, _bridgeMessageReceived, _escapePressed, _newWindowRequested);
#if LINUX
                GHCefWebViewControl cefControl = view as GHCefWebViewControl;
                if (cefControl != null)
                    _host = new GHCefOverseerHost(cefControl, _bridgeMessageReceived, _newWindowRequested);
#endif
                if (_host != null)
                {
                    _platformView = view;
                    _host.BridgeEnabled = _bridgeEnabled;
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaOverseerHost: cannot attach to the web view: " + ex.Message);
            }
        }

        public void Detach()
        {
            _host?.Detach();
            _host = null;
            _platformView = null;
        }

        public void SetBackground(byte r, byte g, byte b)
        {
            _host?.SetBackground(r, g, b);
        }

        public Task InvokeScriptAsync(string script)
        {
            return _host != null ? _host.InvokeScriptAsync(script) : Task.CompletedTask;
        }

        /* After each completed page load */
        public void OnPageLoaded()
        {
            _host?.OnPageLoaded();
        }

        public bool GoBack()
        {
            return _host?.GoBack() ?? false;
        }
    }

    /* Avalonia's NativeWebView: web messages, new-window requests, host script injection and script evaluation without
       MAUI's eval wrapper, which WebKit refuses under the Overseer's Content Security Policy */
    internal sealed class NativeWebViewHost : IGHAvaloniaWebHost
    {
        private const string HostKeyMessageType = "gnollhack_host_key";
        private const string AcmMessageTypePrefix = "__AvaloniaControlsMaui";

        /* Esc forwarding for a focused page, trusted key presses only; synthetic Escapes are left to the page. With the
           WebKitGTK and WKWebView adapters, also the SPA's GnollHackBridge transport, which posts through the adapter's
           invokeCSharpAction. Host-injected and free of eval, so the page's CSP allows it. */
        private const string HostKeyScript =
            "if (!window.__gnollhackHostKeys) {" +
            " window.__gnollhackHostKeys = true;" +
            " document.addEventListener('keydown', function (e) {" +
            "  if (e.isTrusted && e.key === 'Escape' && window.invokeCSharpAction) {" +
            "   window.invokeCSharpAction(JSON.stringify({ type: 'gnollhack_host_key', key: 'Escape' }));" +
            "  }" +
            " }, true);" +
            "}";
        private const string BridgeScript =
            "if (!window.GnollHackBridge && window.invokeCSharpAction) {" +
            " window.GnollHackBridge = { onWebMessage: function (s) { window.invokeCSharpAction(String(s)); } };" +
            "}";

        // WORKAROUND(WA-AVALONIA-0026): Avalonia's WKWebView adapter answers no file input
        /* WKWebView: a click on a file input, the SPA's programmatic one included, asks the host for files instead;
           PickAndDeliverFilesAsync hands them to the SPA's __gnollhackReceiveFiles */
        private const string FilePickMessageType = "gnollhack_host_pick_files";
        private const string FileInputScript =
            "if (!window.__gnollhackFileInput && window.invokeCSharpAction) {" +
            " window.__gnollhackFileInput = true;" +
            " document.addEventListener('click', function (e) {" +
            "  var t = e.target;" +
            "  if (t && t.tagName === 'INPUT' && t.type === 'file') {" +
            "   e.preventDefault();" +
            "   window.invokeCSharpAction(JSON.stringify({ type: 'gnollhack_host_pick_files', multiple: !!t.multiple, accept: t.accept || '' }));" +
            "  }" +
            " }, true);" +
            "}";
        private const int MaxAttachedFiles = 5;
        private const long MaxAttachedFileBytes = 20L * 1024 * 1024;

        private readonly Action<string, string> _bridgeMessageReceived;
        private readonly Action _escapePressed;
        private readonly Action<Uri> _newWindowRequested;
        private Avalonia.Controls.NativeWebView _view;

        public NativeWebViewHost(Avalonia.Controls.NativeWebView view,
            Action<string, string> bridgeMessageReceived, Action escapePressed, Action<Uri> newWindowRequested)
        {
            _bridgeMessageReceived = bridgeMessageReceived;
            _escapePressed = escapePressed;
            _newWindowRequested = newWindowRequested;
            _view = view;
            _view.WebMessageReceived += OnWebMessageReceived;
            _view.NewWindowRequested += OnNewWindowRequested;
        }

        public bool BridgeEnabled { get; set; }

        public string DocumentUrl
        {
            get { return _view?.Source?.AbsoluteUri; }
        }

        // WORKAROUND(WA-AVALONIA-0018): WebKitGTK adapter ignores a cancelled navigation
        /* The WebKitGTK adapter does not honor a cancelled navigation */
        public bool IgnoresNavigationCancel
        {
            get { return IsWebKitGtk; }
        }

        public bool DrawsAboveAvalonia
        {
            get { return true; }
        }

        private bool IsWebKitGtk
        {
            get { return _view?.AdapterInfo?.Type == Avalonia.Platform.WebViewAdapterType.WebKitGtk; }
        }

        private bool IsWkWebView
        {
            get { return _view?.AdapterInfo?.Type == Avalonia.Platform.WebViewAdapterType.WkWebView; }
        }

        public void Detach()
        {
            if (_view == null)
                return;

            _view.WebMessageReceived -= OnWebMessageReceived;
            _view.NewWindowRequested -= OnNewWindowRequested;
            _view = null;
        }

        /* Applies to WebView2; the WebKitGTK adapter ignores it */
        public void SetBackground(byte r, byte g, byte b)
        {
            if (_view == null)
                return;

            _view.Background = new Avalonia.Media.SolidColorBrush(Avalonia.Media.Color.FromRgb(r, g, b));
        }

        // WORKAROUND(WA-MAUI-0027): EvaluateJavaScriptAsync wraps scripts in eval, which a CSP refuses
        public async Task InvokeScriptAsync(string script)
        {
            if (_view == null)
                return;

            try
            {
                await _view.InvokeScript(script);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaOverseerHost: script evaluation failed: " + ex.Message);
            }
        }

        /* The host script does not survive a new document */
        public void OnPageLoaded()
        {
            if (_view == null)
                return;

            string script = BridgeEnabled && (IsWebKitGtk || IsWkWebView) ? HostKeyScript + BridgeScript : HostKeyScript;
            if (BridgeEnabled && IsWkWebView)
                script += FileInputScript;
            _ = InvokeScriptAsync("(function () {" + script + "})();");
        }

        public bool GoBack()
        {
            return _view?.GoBack() ?? false;
        }

        private void OnWebMessageReceived(object sender, Avalonia.Controls.WebMessageReceivedEventArgs e)
        {
            string body = e.Body;
            if (string.IsNullOrEmpty(body))
                return;

            string type = null;
            string key = null;
            bool multiple = false;
            string accept = null;
            try
            {
                JObject message = JObject.Parse(body);
                type = message["type"]?.ToString();
                key = message["key"]?.ToString();
                multiple = message["multiple"]?.Type == JTokenType.Boolean && message["multiple"].Value<bool>();
                accept = message["accept"]?.ToString();
            }
            catch (Exception)
            {
                return;
            }

            if (type != null && type.StartsWith(AcmMessageTypePrefix, StringComparison.Ordinal))
                return;

            if (type == HostKeyMessageType)
            {
                if (key == "Escape")
                    _escapePressed();
                return;
            }

            if (type == FilePickMessageType)
            {
                if (BridgeEnabled && IsWkWebView)
                    Avalonia.Threading.Dispatcher.UIThread.Post(() => { _ = PickAndDeliverFilesAsync(multiple, accept); });
                return;
            }

            if (BridgeEnabled)
                _bridgeMessageReceived(body, null);
        }

        private void OnNewWindowRequested(object sender, Avalonia.Controls.WebViewNewWindowRequestedEventArgs e)
        {
            e.Handled = true;
            if (e.Request != null)
                _newWindowRequested(e.Request);
        }

        /* GnollHack's file dialog when a controller is in use, else the system's, as GHCefWebViewControl chooses; the
           files go to the SPA as { name, type, dataUrl } entries, as from the iOS picker. UI thread. Never throws. */
        private async Task PickAndDeliverFilesAsync(bool multiple, string accept)
        {
            try
            {
                List<string> extensions = AcceptExtensions(accept);
                int maxFiles = multiple ? MaxAttachedFiles : 1;
                IList<string> paths;
                if (GHFileDialogHost.UseCustomDialog)
                {
                    paths = await GHFileDialogHost.PickFilesAsync("Attach Files", "Attach", multiple, maxFiles, extensions, false);
                }
                else
                {
                    Avalonia.Platform.Storage.IStorageProvider storage = _view != null ? Avalonia.Controls.TopLevel.GetTopLevel(_view)?.StorageProvider : null;
                    if (storage == null)
                        return;
                    List<Avalonia.Platform.Storage.FilePickerFileType> filters = new List<Avalonia.Platform.Storage.FilePickerFileType>();
                    if (extensions.Count > 0)
                    {
                        List<string> patterns = new List<string>();
                        foreach (string extension in extensions)
                            patterns.Add("*" + extension);
                        filters.Add(new Avalonia.Platform.Storage.FilePickerFileType("Accepted files") { Patterns = patterns });
                    }
                    filters.Add(Avalonia.Platform.Storage.FilePickerFileTypes.All);
                    IReadOnlyList<Avalonia.Platform.Storage.IStorageFile> files = await storage.OpenFilePickerAsync(
                        new Avalonia.Platform.Storage.FilePickerOpenOptions { Title = "Attach Files", AllowMultiple = multiple, FileTypeFilter = filters });
                    List<string> localPaths = new List<string>();
                    if (files != null)
                    {
                        foreach (Avalonia.Platform.Storage.IStorageFile file in files)
                        {
                            string localPath = Avalonia.Platform.Storage.StorageProviderExtensions.TryGetLocalPath(file);
                            if (!string.IsNullOrEmpty(localPath))
                                localPaths.Add(localPath);
                        }
                    }
                    paths = localPaths;
                }

                if (paths == null || paths.Count == 0 || _view == null)
                    return;

                JArray entries = new JArray();
                foreach (string path in paths)
                {
                    if (entries.Count >= maxFiles)
                        break;
                    FileInfo info = new FileInfo(path);
                    if (!info.Exists)
                        continue;
                    if (info.Length > MaxAttachedFileBytes)
                    {
                        GHApp.MaybeWriteGHLog("GHAvaloniaOverseerHost: " + info.Name + " is too large to attach (" + info.Length + " bytes)");
                        continue;
                    }
                    string mime = MimeTypeOf(info.Extension);
                    byte[] data = await File.ReadAllBytesAsync(path);
                    JObject entry = new JObject();
                    entry["name"] = info.Name;
                    entry["type"] = mime;
                    entry["dataUrl"] = "data:" + mime + ";base64," + Convert.ToBase64String(data);
                    entries.Add(entry);
                }
                if (entries.Count == 0)
                    return;

                string literal = JsonConvert.SerializeObject(entries.ToString(Formatting.None));
                await InvokeScriptAsync("if (window.__gnollhackReceiveFiles) { window.__gnollhackReceiveFiles(" + literal + "); }");
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaOverseerHost: attaching files failed: " + ex.Message);
            }
        }

        /* The extensions in an accept attribute, such as ".txt,.png,image/*", with their dots */
        private static List<string> AcceptExtensions(string accept)
        {
            List<string> extensions = new List<string>();
            if (string.IsNullOrEmpty(accept))
                return extensions;
            foreach (string part in accept.Split(','))
            {
                string trimmed = part.Trim().ToLowerInvariant();
                if (trimmed.Length > 1 && trimmed[0] == '.' && !extensions.Contains(trimmed))
                    extensions.Add(trimmed);
            }
            return extensions;
        }

        private static string MimeTypeOf(string extension)
        {
            switch ((extension ?? "").ToLowerInvariant())
            {
            case ".txt":
                return "text/plain";
            case ".md":
                return "text/markdown";
            case ".html":
            case ".htm":
                return "text/html";
            case ".png":
                return "image/png";
            case ".jpg":
            case ".jpeg":
                return "image/jpeg";
            case ".webp":
                return "image/webp";
            case ".gif":
                return "image/gif";
            case ".json":
                return "application/json";
            default:
                return "application/octet-stream";
            }
        }
    }
}
#endif

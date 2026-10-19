#if GNH_AVALONIA
using System;
using System.Threading.Tasks;
using GnollHackX;
using Newtonsoft.Json.Linq;

namespace GnollHackM
{
    /* The Overseer page's Avalonia NativeWebView: web messages, new-window requests, host script injection and script
       evaluation without MAUI's eval wrapper, which WebKit refuses under the Overseer's Content Security Policy. Every
       member is a no-op while no view is attached. UI thread only. */
    internal sealed class GHAvaloniaOverseerHost
    {
        private const string HostKeyMessageType = "gnollhack_host_key";
        private const string AcmMessageTypePrefix = "__AvaloniaControlsMaui";

        /* Esc forwarding for a focused page. With the WebKitGTK adapter, also the SPA's GnollHackBridge transport, which
           posts through the adapter's invokeCSharpAction. Host-injected and free of eval, so the page's CSP allows it. */
        private const string HostKeyScript =
            "if (!window.__gnollhackHostKeys) {" +
            " window.__gnollhackHostKeys = true;" +
            " document.addEventListener('keydown', function (e) {" +
            "  if (e.key === 'Escape' && window.invokeCSharpAction) {" +
            "   window.invokeCSharpAction(JSON.stringify({ type: 'gnollhack_host_key', key: 'Escape' }));" +
            "  }" +
            " }, true);" +
            "}";
        private const string BridgeScript =
            "if (!window.GnollHackBridge && window.invokeCSharpAction) {" +
            " window.GnollHackBridge = { onWebMessage: function (s) { window.invokeCSharpAction(String(s)); } };" +
            "}";

        private readonly Microsoft.Maui.Controls.WebView _webView;
        private readonly Action<string> _bridgeMessageReceived;
        private readonly Action _escapePressed;
        private readonly Action<Uri> _newWindowRequested;
        private Avalonia.Controls.NativeWebView _view;

        public GHAvaloniaOverseerHost(Microsoft.Maui.Controls.WebView webView,
            Action<string> bridgeMessageReceived, Action escapePressed, Action<Uri> newWindowRequested)
        {
            _webView = webView;
            _bridgeMessageReceived = bridgeMessageReceived;
            _escapePressed = escapePressed;
            _newWindowRequested = newWindowRequested;
        }

        /* Whether SPA messages other than the host's own reach the bridge callback */
        public bool BridgeEnabled { get; set; }

        public string DocumentUrl
        {
            get { return _view?.Source?.AbsoluteUri; }
        }

        /* The WebKitGTK adapter does not honor a cancelled navigation */
        public bool IgnoresNavigationCancel
        {
            get { return IsWebKitGtk; }
        }

        private bool IsWebKitGtk
        {
            get { return _view?.AdapterInfo?.Type == Avalonia.Platform.WebViewAdapterType.WebKitGtk; }
        }

        /* Follows the MAUI web view's current handler */
        public void Attach()
        {
            try
            {
                Avalonia.Controls.NativeWebView view = _webView.Handler?.PlatformView as Avalonia.Controls.NativeWebView;
                if (view == null)
                {
                    Detach();
                    return;
                }
                if (ReferenceEquals(view, _view))
                    return;

                Detach();
                _view = view;
                _view.WebMessageReceived += OnWebMessageReceived;
                _view.NewWindowRequested += OnNewWindowRequested;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaOverseerHost: cannot attach to the web view: " + ex.Message);
            }
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

        /* After each completed page load: the host script does not survive a new document */
        public void OnPageLoaded()
        {
            if (_view == null)
                return;

            string script = BridgeEnabled && IsWebKitGtk ? HostKeyScript + BridgeScript : HostKeyScript;
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
            try
            {
                JObject message = JObject.Parse(body);
                type = message["type"]?.ToString();
                key = message["key"]?.ToString();
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

            if (BridgeEnabled)
                _bridgeMessageReceived(body);
        }

        private void OnNewWindowRequested(object sender, Avalonia.Controls.WebViewNewWindowRequestedEventArgs e)
        {
            e.Handled = true;
            if (e.Request != null)
                _newWindowRequested(e.Request);
        }
    }
}
#endif

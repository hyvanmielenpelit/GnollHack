#if GNH_AVALONIA && LINUX
using System;
using System.Threading.Tasks;

namespace GnollHackM
{
    /* The Overseer page's CEF web view. The renderer defines window.GnollHackBridge at document start and reports the
       sending frame's URL; Escape never reaches the page, because the window's key handling takes it first. */
    internal sealed class GHCefOverseerHost : IGHAvaloniaWebHost
    {
        /* Console lines of the page pointer probe */
        public const string ProbePrefix = "GNHPROBE ";

        /* While the pointer log is on: trusted pointer, mouse, click and touch events as the page receives them, logged
           to the console in the capture phase without affecting them */
        private const string ProbeScript =
            "(function () {" +
            " if (window.__gnhProbe) return;" +
            " window.__gnhProbe = true;" +
            " function log(e) {" +
            "  if (!e.isTrusted) return;" +
            "  var t = e.target;" +
            "  var name = t && t.tagName ? t.tagName.toLowerCase() : '-';" +
            "  var cls = t && typeof t.className === 'string' && t.className.trim() ? '.' + t.className.trim().split(/\\s+/).slice(0, 2).join('.') : '';" +
            "  var p = e.touches && e.touches.length ? e.touches[0] : e;" +
            "  var x = typeof p.clientX === 'number' ? Math.round(p.clientX) : -1;" +
            "  var y = typeof p.clientY === 'number' ? Math.round(p.clientY) : -1;" +
            "  console.log('" + ProbePrefix + "' + e.type + ' ' + x + ',' + y + ' ' + name + cls);" +
            " }" +
            " ['pointerdown', 'mousedown', 'click', 'touchstart'].forEach(function (k) {" +
            "  document.addEventListener(k, log, { capture: true, passive: true });" +
            " });" +
            "})();";

        private readonly Action<string, string> _bridgeMessageReceived;
        private readonly Action<Uri> _newWindowRequested;
        private GHCefWebViewControl _control;

        public GHCefOverseerHost(GHCefWebViewControl control, Action<string, string> bridgeMessageReceived, Action<Uri> newWindowRequested)
        {
            _bridgeMessageReceived = bridgeMessageReceived;
            _newWindowRequested = newWindowRequested;
            _control = control;
            _control.BridgeMessageReceived += OnBridgeMessageReceived;
            _control.NewWindowRequested += OnNewWindowRequested;
        }

        public bool BridgeEnabled { get; set; }

        public string DocumentUrl
        {
            get { return _control?.Address; }
        }

        public bool IgnoresNavigationCancel
        {
            get { return false; }
        }

        public bool DrawsAboveAvalonia
        {
            get { return false; }
        }

        public void SetBackground(byte r, byte g, byte b)
        {
            if (_control != null)
                _control.BackgroundColor = Avalonia.Media.Color.FromRgb(r, g, b);
        }

        public Task InvokeScriptAsync(string script)
        {
            _control?.ExecuteScript(script);
            return Task.CompletedTask;
        }

        public void OnPageLoaded()
        {
            if (_control != null && GHAvaloniaPointerLog.IsEnabled)
                _control.ExecuteScript(ProbeScript);
        }

        public bool GoBack()
        {
            if (_control == null || !_control.CanGoBack)
                return false;

            _control.GoBack();
            return true;
        }

        public void Detach()
        {
            if (_control == null)
                return;

            _control.BridgeMessageReceived -= OnBridgeMessageReceived;
            _control.NewWindowRequested -= OnNewWindowRequested;
            _control = null;
        }

        private void OnBridgeMessageReceived(string text, string frameUrl)
        {
            if (BridgeEnabled)
                _bridgeMessageReceived(text, frameUrl);
        }

        private void OnNewWindowRequested(Uri target)
        {
            _newWindowRequested(target);
        }
    }
}
#endif

#if GNH_AVALONIA && LINUX
using System;
using System.Threading.Tasks;

namespace GnollHackM
{
    /* The Overseer page's CEF web view. The renderer defines window.GnollHackBridge at document start and reports the
       sending frame's URL; Escape never reaches the page, because the window's key handling takes it first. */
    internal sealed class GHCefOverseerHost : IGHAvaloniaWebHost
    {
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

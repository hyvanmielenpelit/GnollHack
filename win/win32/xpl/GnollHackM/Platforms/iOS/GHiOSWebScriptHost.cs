using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Foundation;
using GnollHackX;
using UIKit;
using WebKit;
using MauiWebView = Microsoft.Maui.Controls.WebView;

namespace GnollHackM
{
    /* The in-app browser's script host for controller navigation: follows the MAUI web view's current handler to the
       WKWebView and evaluates scripts there directly. Every member is a no-op while no view is attached. UI thread
       only. */
    internal sealed class GHiOSWebScriptHost : IGHWebScriptHost, IDisposable
    {
        private const int EvaluateTimeoutMilliseconds = 2000;
        private const string WebKitErrorDomain = "WKErrorDomain";
        private const long ResultTypeIsUnsupportedCode = 5;     /* WKErrorJavaScriptResultTypeIsUnsupported */

        private readonly MauiWebView _webView;
        private WKWebView _nativeView;
        private bool _failureLogged;
        private bool _disposed;

        public GHiOSWebScriptHost(MauiWebView webView)
        {
            _webView = webView;
            if (_webView != null)
                _webView.HandlerChanged += WebView_HandlerChanged;
            ResolveView();
        }

        public bool IsReady
        {
            get { return !_disposed && _nativeView != null && _nativeView.Handle != IntPtr.Zero; }
        }

        public event Action ViewChanged;

        /* The scale of the view's screen; 1 when unknown. A CSS pixel of the page is one point. */
        public double RenderScaling
        {
            get
            {
                try
                {
                    UIWindow window = _nativeView != null ? _nativeView.Window : null;
                    UIScreen screen = window != null ? window.Screen : null;
                    double scale = screen != null ? (double)screen.Scale : 1.0;
                    return double.IsNaN(scale) || double.IsInfinity(scale) || scale <= 0.0 ? 1.0 : scale;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                    return 1.0;
                }
            }
        }

        public string PageScript
        {
            get { return GHWebPageScript.Load(); }
        }

        /* The script's result as text, raced against a 2 s timeout; null on timeout, failure, a result that is not
           text or a number, or without a view */
        public async Task<string> EvaluateAsync(string script)
        {
            if (!IsReady || string.IsNullOrEmpty(script))
                return null;

            TaskCompletionSource<string> completion =
                new TaskCompletionSource<string>(TaskCreationOptions.RunContinuationsAsynchronously);
            try
            {
                _nativeView.EvaluateJavaScript(script, (result, error) => OnEvaluated(completion, result, error));
            }
            catch (Exception ex)
            {
                LogFailure("script evaluation failed: " + ex.Message);
                return null;
            }

            Task<string> scriptTask = completion.Task;
            Task finished = await Task.WhenAny(scriptTask, Task.Delay(EvaluateTimeoutMilliseconds));
            if (!ReferenceEquals(finished, scriptTask))
            {
                LogFailure("script evaluation timed out");
                return null;
            }
            return scriptTask.Result;
        }

        public Task PrepareUserGestureAsync()
        {
            return Task.CompletedTask;
        }

        public void FocusNative()
        {
            try
            {
                if (IsReady && !_nativeView.IsFirstResponder)
                    _nativeView.BecomeFirstResponder();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        public void ResetFailureLog()
        {
            _failureLogged = false;
        }

        public void Dispose()
        {
            if (_disposed)
                return;
            _disposed = true;
            if (_webView != null)
                _webView.HandlerChanged -= WebView_HandlerChanged;
            _nativeView = null;
        }

        /* Called from native code: nothing may escape. A script whose value WebKit cannot return, such as the page
           script's own, gives no result. */
        private void OnEvaluated(TaskCompletionSource<string> completion, NSObject result, NSError error)
        {
            try
            {
                if (error != null)
                {
                    if (!(error.Domain == WebKitErrorDomain && (long)error.Code == ResultTypeIsUnsupportedCode))
                        LogFailure("script evaluation failed: " + error.LocalizedDescription);
                    completion.TrySetResult(null);
                    return;
                }
                completion.TrySetResult(ToText(result));
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                completion.TrySetResult(null);
            }
        }

        private static string ToText(NSObject result)
        {
            if (result == null || result is NSNull)
                return null;
            NSString text = result as NSString;
            if (text != null)
                return text.ToString();
            NSNumber number = result as NSNumber;
            return number != null ? number.StringValue : null;
        }

        private void WebView_HandlerChanged(object sender, EventArgs e)
        {
            WKWebView previous = _nativeView;
            ResolveView();
            if (!ReferenceEquals(previous, _nativeView))
                ViewChanged?.Invoke();
        }

        /* The MAUI web view's current platform view; handler getters can throw instead of returning null */
        private void ResolveView()
        {
            WKWebView view = null;
            try
            {
                view = _webView?.Handler?.PlatformView as WKWebView;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHiOSWebScriptHost: cannot attach to the web view: " + ex.Message);
            }
            _nativeView = view;
        }

        private void LogFailure(string message)
        {
            if (_failureLogged)
                return;
            _failureLogged = true;
            GHApp.MaybeWriteGHLog("GHiOSWebScriptHost: " + message);
        }
    }
}

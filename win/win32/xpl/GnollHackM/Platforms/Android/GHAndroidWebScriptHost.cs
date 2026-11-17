using System;
using System.Diagnostics;
using System.Threading.Tasks;
using Android.Webkit;
using GnollHackX;
using AndroidWebView = Android.Webkit.WebView;
using MauiWebView = Microsoft.Maui.Controls.WebView;

namespace GnollHackM
{
    /* The in-app browser's script host for controller navigation: follows the MAUI web view's current handler to the
       Android WebView and evaluates scripts there directly. Every member is a no-op while no view is attached. UI
       thread only. */
    internal sealed class GHAndroidWebScriptHost : IGHWebScriptHost, IDisposable
    {
        private const int EvaluateTimeoutMilliseconds = 2000;

        private readonly MauiWebView _webView;
        private AndroidWebView _nativeView;
        private bool _failureLogged;
        private bool _disposed;

        public GHAndroidWebScriptHost(MauiWebView webView)
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

        /* The view's display density; 1 when unknown */
        public double RenderScaling
        {
            get
            {
                try
                {
                    Android.Util.DisplayMetrics metrics = _nativeView?.Resources?.DisplayMetrics;
                    double scale = metrics != null ? metrics.Density : 1.0;
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

        /* The script's result as JSON from the WebView's evaluateJavascript, raced against a 2 s timeout; null on
           timeout, failure or without a view */
        public async Task<string> EvaluateAsync(string script)
        {
            if (!IsReady || string.IsNullOrEmpty(script))
                return null;

            TaskCompletionSource<string> completion =
                new TaskCompletionSource<string>(TaskCreationOptions.RunContinuationsAsynchronously);
            try
            {
                _nativeView.EvaluateJavascript(script, new ScriptResultCallback(completion));
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
                if (IsReady)
                    _nativeView.RequestFocus();
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

        private void WebView_HandlerChanged(object sender, EventArgs e)
        {
            AndroidWebView previous = _nativeView;
            ResolveView();
            if (!ReferenceEquals(previous, _nativeView))
                ViewChanged?.Invoke();
        }

        /* The MAUI web view's current platform view; handler getters can throw instead of returning null */
        private void ResolveView()
        {
            AndroidWebView view = null;
            try
            {
                view = _webView?.Handler?.PlatformView as AndroidWebView;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAndroidWebScriptHost: cannot attach to the web view: " + ex.Message);
            }
            _nativeView = view;
        }

        private void LogFailure(string message)
        {
            if (_failureLogged)
                return;
            _failureLogged = true;
            GHApp.MaybeWriteGHLog("GHAndroidWebScriptHost: " + message);
        }

        /* Completes the evaluation's task with the result's JSON text */
        private sealed class ScriptResultCallback : Java.Lang.Object, IValueCallback
        {
            private readonly TaskCompletionSource<string> _completion;

            public ScriptResultCallback(TaskCompletionSource<string> completion)
            {
                _completion = completion;
            }

            public void OnReceiveValue(Java.Lang.Object value)
            {
                try
                {
                    _completion.TrySetResult(value?.ToString());
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                }
            }
        }
    }
}

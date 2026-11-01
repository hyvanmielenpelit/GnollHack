using System;
using System.Diagnostics;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.UI.Xaml;
using Microsoft.Web.WebView2.Core;
using MauiWebView = Microsoft.Maui.Controls.WebView;
using XamlWebView2 = Microsoft.UI.Xaml.Controls.WebView2;

namespace GnollHackM
{
    /* The in-app browser's script host for controller navigation: follows the MAUI web view's current handler to WinUI's
       WebView2 and evaluates scripts there directly. CoreWebView2 is created after the handler connects, so
       CoreWebView2Initialized also raises ViewChanged. Owns the view's GHWebView2FileChooser. Every member is a no-op
       while no view is attached. UI thread only. */
    internal sealed class GHWinUIWebScriptHost : IGHWebScriptHost, IDisposable
    {
        private const int EvaluateTimeoutMilliseconds = 2000;

        private readonly MauiWebView _webView;
        private XamlWebView2 _nativeView;
        private GHWebView2FileChooser _fileChooser;
        private bool _failureLogged;
        private bool _disposed;

        public GHWinUIWebScriptHost(MauiWebView webView)
        {
            _webView = webView;
            if (_webView != null)
                _webView.HandlerChanged += WebView_HandlerChanged;
            ResolveView();
        }

        public bool IsReady
        {
            get { return !_disposed && _nativeView != null && _nativeView.CoreWebView2 != null; }
        }

        public event Action ViewChanged;

        /* The rasterization scale of the view's XAML root; 1 when unknown */
        public double RenderScaling
        {
            get
            {
                try
                {
                    double scale = _nativeView?.XamlRoot != null ? _nativeView.XamlRoot.RasterizationScale : 1.0;
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

        /* The script's result as JSON, raced against a 2 s timeout; null on timeout, failure or without a view. The
           script task's exception is always observed. */
        // WORKAROUND(WA-MAUI-0027): EvaluateJavaScriptAsync wraps scripts in eval, which a CSP refuses
        public async Task<string> EvaluateAsync(string script)
        {
            CoreWebView2 core = _disposed ? null : _nativeView?.CoreWebView2;
            if (core == null || string.IsNullOrEmpty(script))
                return null;

            Task<string> scriptTask;
            try
            {
                scriptTask = core.ExecuteScriptAsync(script).AsTask();
            }
            catch (Exception ex)
            {
                LogFailure("script evaluation failed: " + ex.Message);
                return null;
            }

            _ = scriptTask.ContinueWith(t =>
            {
                Exception ignored = t.Exception;
            }, TaskContinuationOptions.OnlyOnFaulted | TaskContinuationOptions.ExecuteSynchronously);
            Task finished = await Task.WhenAny(scriptTask, Task.Delay(EvaluateTimeoutMilliseconds));
            if (!ReferenceEquals(finished, scriptTask))
            {
                LogFailure("script evaluation timed out");
                return null;
            }
            if (scriptTask.IsFaulted || scriptTask.IsCanceled)
            {
                Exception ex = scriptTask.Exception != null ? scriptTask.Exception.GetBaseException() : null;
                LogFailure("script evaluation failed: " + (ex != null ? ex.Message : "canceled"));
                return null;
            }
            return scriptTask.Result;
        }

        public Task PrepareUserGestureAsync()
        {
            return _fileChooser != null ? _fileChooser.PrepareAsync() : Task.CompletedTask;
        }

        public void FocusNative()
        {
            try
            {
                _nativeView?.Focus(FocusState.Programmatic);
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
            SetNativeView(null);
        }

        private void WebView_HandlerChanged(object sender, EventArgs e)
        {
            XamlWebView2 previous = _nativeView;
            ResolveView();
            if (!ReferenceEquals(previous, _nativeView))
                ViewChanged?.Invoke();
        }

        /* The MAUI web view's current platform view; handler getters can throw instead of returning null */
        private void ResolveView()
        {
            XamlWebView2 view = null;
            try
            {
                view = _webView?.Handler?.PlatformView as XamlWebView2;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWinUIWebScriptHost: cannot attach to the web view: " + ex.Message);
            }
            SetNativeView(view);
        }

        private void SetNativeView(XamlWebView2 view)
        {
            if (ReferenceEquals(view, _nativeView))
                return;
            try
            {
                if (_nativeView != null)
                    _nativeView.CoreWebView2Initialized -= NativeView_CoreWebView2Initialized;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            _fileChooser?.Dispose();
            _fileChooser = null;
            _nativeView = view;
            if (_nativeView != null)
            {
                _nativeView.CoreWebView2Initialized += NativeView_CoreWebView2Initialized;
                if (_nativeView.CoreWebView2 != null)
                    _fileChooser = new GHWebView2FileChooser(_nativeView.CoreWebView2);
            }
        }

        private void NativeView_CoreWebView2Initialized(XamlWebView2 sender, Microsoft.UI.Xaml.Controls.CoreWebView2InitializedEventArgs args)
        {
            _fileChooser?.Dispose();
            _fileChooser = sender.CoreWebView2 != null ? new GHWebView2FileChooser(sender.CoreWebView2) : null;
            ViewChanged?.Invoke();
        }

        private void LogFailure(string message)
        {
            if (_failureLogged)
                return;
            _failureLogged = true;
            GHApp.MaybeWriteGHLog("GHWinUIWebScriptHost: " + message);
        }
    }
}

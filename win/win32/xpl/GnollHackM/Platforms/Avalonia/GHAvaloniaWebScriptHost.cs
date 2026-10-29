#if GNH_AVALONIA
using System;
using System.IO;
using System.Threading.Tasks;
using GnollHackX;

namespace GnollHackM
{
    /* The in-app browser's script host for controller navigation: follows the MAUI web view's current handler to
       Avalonia's NativeWebView, or on Linux the CEF control, and evaluates scripts there directly. Every member is a
       no-op while no view is attached. UI thread only. */
    internal sealed class GHAvaloniaWebScriptHost : IGHWebScriptHost, IDisposable
    {
        private const int EvaluateTimeoutMilliseconds = 2000;
        private const string PageScriptResourceName = "GnollHackM.GamepadWeb.js";

        /* The page script, read from the manifest resource on first use; null when it cannot be read */
        private static string _pageScript;
        private static bool _pageScriptLoaded;

        private readonly Microsoft.Maui.Controls.WebView _webView;
        private Avalonia.Controls.NativeWebView _nativeView;
#if LINUX
        private GHCefWebViewControl _cefView;
#endif
        private bool _failureLogged;
        private bool _disposed;

        public GHAvaloniaWebScriptHost(Microsoft.Maui.Controls.WebView webView)
        {
            _webView = webView;
            if (_webView != null)
                _webView.HandlerChanged += WebView_HandlerChanged;
            ResolveView();
        }

        public bool IsReady
        {
            get { return PlatformControl != null; }
        }

        public event Action ViewChanged;

        /* The render scaling of the platform control's window; 1 when unknown */
        public double RenderScaling
        {
            get
            {
                try
                {
                    Avalonia.Controls.Control control = PlatformControl;
                    if (control == null)
                        return 1.0;
                    Avalonia.Controls.TopLevel topLevel = Avalonia.Controls.TopLevel.GetTopLevel(control);
                    double scale = topLevel != null ? topLevel.RenderScaling : 1.0;
                    if (double.IsNaN(scale) || double.IsInfinity(scale) || scale <= 0.0)
                        return 1.0;
                    return scale;
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine(ex);
                    return 1.0;
                }
            }
        }

        public string PageScript
        {
            get { return LoadPageScript(); }
        }

        private Avalonia.Controls.Control PlatformControl
        {
            get
            {
                if (_nativeView != null)
                    return _nativeView;
#if LINUX
                if (_cefView != null)
                    return _cefView;
#endif
                return null;
            }
        }

        /* The script's result as the platform returns it, raced against a 2 s timeout; null on timeout, failure or
           without a view. The script task's exception is always observed. */
        // WORKAROUND(WA-MAUI-0027): EvaluateJavaScriptAsync wraps scripts in eval, which a CSP refuses
        public async Task<string> EvaluateAsync(string script)
        {
            if (_disposed || string.IsNullOrEmpty(script))
                return null;

            Task<string> scriptTask;
            try
            {
                scriptTask = StartScript(script);
            }
            catch (Exception ex)
            {
                LogFailure("script evaluation failed: " + ex.Message);
                return null;
            }
            if (scriptTask == null)
                return null;

            ObserveFault(scriptTask);
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

        public void FocusNative()
        {
            try
            {
                Avalonia.Controls.Control control = PlatformControl;
                if (control != null)
                    control.Focus();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
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
#if LINUX
            _cefView = null;
#endif
        }

        private Task<string> StartScript(string script)
        {
            if (_nativeView != null)
                return _nativeView.InvokeScript(script);
#if LINUX
            if (_cefView != null)
                return _cefView.EvaluateScriptAsync(script);
#endif
            return null;
        }

        /* InvokeScript faults before the first load; an unobserved fault would reach the task scheduler */
        private static void ObserveFault(Task task)
        {
            task.ContinueWith(t =>
            {
                Exception ignored = t.Exception;
            }, TaskContinuationOptions.OnlyOnFaulted | TaskContinuationOptions.ExecuteSynchronously);
        }

        private void WebView_HandlerChanged(object sender, EventArgs e)
        {
            Avalonia.Controls.Control previous = PlatformControl;
            ResolveView();
            if (!ReferenceEquals(previous, PlatformControl))
                ViewChanged?.Invoke();
        }

        /* The MAUI web view's current platform view; handler getters can throw instead of returning null */
        private void ResolveView()
        {
            _nativeView = null;
#if LINUX
            _cefView = null;
#endif
            if (_disposed || _webView == null)
                return;

            object view = null;
            try
            {
                Microsoft.Maui.IViewHandler handler = _webView.Handler;
                view = handler != null ? handler.PlatformView : null;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWebScriptHost: cannot attach to the web view: " + ex.Message);
                return;
            }

            _nativeView = view as Avalonia.Controls.NativeWebView;
#if LINUX
            if (_nativeView == null)
                _cefView = view as GHCefWebViewControl;
#endif
        }

        private void LogFailure(string message)
        {
            if (_failureLogged)
                return;
            _failureLogged = true;
            GHApp.MaybeWriteGHLog("GHAvaloniaWebScriptHost: " + message);
        }

        private static string LoadPageScript()
        {
            if (_pageScriptLoaded)
                return _pageScript;
            _pageScriptLoaded = true;
            try
            {
                System.Reflection.Assembly assembly = typeof(GHAvaloniaWebScriptHost).Assembly;
                using (Stream stream = assembly.GetManifestResourceStream(PageScriptResourceName))
                {
                    if (stream == null)
                    {
                        GHApp.MaybeWriteGHLog("GHAvaloniaWebScriptHost: page script resource " + PageScriptResourceName
                            + " not found");
                        return null;
                    }
                    using (StreamReader reader = new StreamReader(stream))
                    {
                        _pageScript = reader.ReadToEnd();
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWebScriptHost: cannot read the page script: " + ex.Message);
                _pageScript = null;
            }
            return _pageScript;
        }
    }
}
#endif

#if GNH_AVALONIA && LINUX
using System;
using System.Threading.Tasks;
using Microsoft.Maui;
using AcmViewHandler = Avalonia.Controls.Maui.Handlers.ViewHandler;

namespace GnollHackM
{
    /* The MAUI WebView handler over GHCefWebViewControl, following ACM's WebViewHandler. Navigating is raised
       synchronously from CEF's navigation callback, so a page can cancel a navigation before it starts; Navigated and
       the history state arrive through the dispatcher. Disconnecting closes the browser for good. */
    public sealed class GHCefWebViewHandler : Avalonia.Controls.Maui.Handlers.ViewHandler<IWebView, GHCefWebViewControl>, IWebViewDelegate
    {
        /* Chained to ACM's view mappers: MAUI's own view mappers do nothing on Avalonia */
        public static IPropertyMapper<IWebView, GHCefWebViewHandler> Mapper =
            new Microsoft.Maui.PropertyMapper<IWebView, GHCefWebViewHandler>(AcmViewHandler.ViewMapper)
            {
                [nameof(IWebView.Source)] = MapSource,
                [nameof(IWebView.UserAgent)] = MapUserAgent,
                [nameof(IWebView.Cookies)] = MapCookies,
            };

        public static Microsoft.Maui.CommandMapper<IWebView, GHCefWebViewHandler> CommandMapper =
            new Microsoft.Maui.CommandMapper<IWebView, GHCefWebViewHandler>(AcmViewHandler.ViewCommandMapper)
            {
                [nameof(IWebView.GoBack)] = MapGoBack,
                [nameof(IWebView.GoForward)] = MapGoForward,
                [nameof(IWebView.Reload)] = MapReload,
                [nameof(IWebView.Eval)] = MapEval,
                [nameof(IWebView.EvaluateJavaScriptAsync)] = MapEvaluateJavaScriptAsync,
            };

        /* The kind of the navigation in progress: NewPage unless a command started it; back to NewPage once it has
           completed or been cancelled */
        private WebNavigationEvent _currentNavigationEvent = WebNavigationEvent.NewPage;

        public GHCefWebViewHandler() : base(Mapper, CommandMapper)
        {
        }

        public GHCefWebViewHandler(IPropertyMapper mapper, Microsoft.Maui.CommandMapper commandMapper)
            : base(mapper ?? Mapper, commandMapper ?? CommandMapper)
        {
        }

        /* ElementHandler's own getters throw when unset */
        private IWebView CurrentWebView
        {
            get { return ((IElementHandler)this).VirtualView as IWebView; }
        }

        private GHCefWebViewControl CefControl
        {
            get { return ((IElementHandler)this).PlatformView as GHCefWebViewControl; }
        }

        protected override GHCefWebViewControl CreatePlatformView()
        {
            return new GHCefWebViewControl();
        }

        protected override void ConnectHandler(GHCefWebViewControl platformView)
        {
            base.ConnectHandler(platformView);
            if (platformView == null)
                return;

            platformView.NavigationStarting = OnNavigationStarting;
            platformView.NavigationCompleted += OnNavigationCompleted;
            platformView.HistoryChanged += OnHistoryChanged;
        }

        protected override void DisconnectHandler(GHCefWebViewControl platformView)
        {
            if (platformView != null)
            {
                platformView.NavigationStarting = null;
                platformView.NavigationCompleted -= OnNavigationCompleted;
                platformView.HistoryChanged -= OnHistoryChanged;
                try
                {
                    platformView.CloseBrowser();
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefWebViewHandler: closing the browser failed: " + ex.Message);
                }
            }
            base.DisconnectHandler(platformView);
        }

        /* Called synchronously inside CEF's navigation callback; true cancels */
        private bool OnNavigationStarting(string url)
        {
            try
            {
                IWebView webView = CurrentWebView;
                if (webView == null)
                    return false;

                bool cancel = webView.Navigating(_currentNavigationEvent, url ?? "");
                if (cancel)
                    _currentNavigationEvent = WebNavigationEvent.NewPage;
                return cancel;
            }
            catch (Exception ex)
            {
                GHCefRuntime.Log("GHCefWebViewHandler: Navigating failed: " + ex.Message);
                return false;
            }
        }

        private void OnNavigationCompleted(string url, bool success)
        {
            try
            {
                IWebView webView = CurrentWebView;
                if (webView == null)
                    return;

                WebNavigationEvent navigationEvent = _currentNavigationEvent;
                _currentNavigationEvent = WebNavigationEvent.NewPage;
                webView.Navigated(navigationEvent, url ?? "", success ? WebNavigationResult.Success : WebNavigationResult.Failure);
            }
            catch (Exception ex)
            {
                GHCefRuntime.Log("GHCefWebViewHandler: Navigated failed: " + ex.Message);
            }
        }

        private void OnHistoryChanged(bool canGoBack, bool canGoForward)
        {
            try
            {
                IWebView webView = CurrentWebView;
                if (webView == null)
                    return;

                webView.CanGoBack = canGoBack;
                webView.CanGoForward = canGoForward;
            }
            catch (Exception ex)
            {
                GHCefRuntime.Log("GHCefWebViewHandler: history update failed: " + ex.Message);
            }
        }

        /* The source calls back into LoadUrl or LoadHtml */
        public static void MapSource(GHCefWebViewHandler handler, IWebView webView)
        {
            if (handler == null || webView == null || handler.CefControl == null)
                return;

            webView.Source?.Load(handler);
        }

        /* CEF's user agent is set for the whole runtime, not per view */
        public static void MapUserAgent(GHCefWebViewHandler handler, IWebView webView)
        {
            string userAgent = webView?.UserAgent;
            if (!string.IsNullOrEmpty(userAgent))
                GHCefRuntime.Log("GHCefWebViewHandler: a user agent was requested but is not applied: " + userAgent);
        }

        /* Cookies are not synchronized with CEF's cookie store */
        public static void MapCookies(GHCefWebViewHandler handler, IWebView webView)
        {
        }

        public static void MapGoBack(GHCefWebViewHandler handler, IWebView webView, object arg)
        {
            GHCefWebViewControl control = handler?.CefControl;
            if (control == null)
                return;

            if (control.CanGoBack)
            {
                handler._currentNavigationEvent = WebNavigationEvent.Back;
                control.GoBack();
            }
            else
            {
                handler._currentNavigationEvent = WebNavigationEvent.NewPage;
            }
        }

        public static void MapGoForward(GHCefWebViewHandler handler, IWebView webView, object arg)
        {
            GHCefWebViewControl control = handler?.CefControl;
            if (control == null)
                return;

            if (control.CanGoForward)
            {
                handler._currentNavigationEvent = WebNavigationEvent.Forward;
                control.GoForward();
            }
            else
            {
                handler._currentNavigationEvent = WebNavigationEvent.NewPage;
            }
        }

        public static void MapReload(GHCefWebViewHandler handler, IWebView webView, object arg)
        {
            GHCefWebViewControl control = handler?.CefControl;
            if (control == null)
                return;

            handler._currentNavigationEvent = WebNavigationEvent.Refresh;
            control.Reload();
        }

        public static void MapEval(GHCefWebViewHandler handler, IWebView webView, object arg)
        {
            GHCefWebViewControl control = handler?.CefControl;
            string script = arg as string;
            if (control != null && script != null)
                control.ExecuteScript(script);
        }

        /* Completes the request with the result as JSON, or "null" when there is none */
        public static void MapEvaluateJavaScriptAsync(GHCefWebViewHandler handler, IWebView webView, object arg)
        {
            EvaluateJavaScriptAsyncRequest request = arg as EvaluateJavaScriptAsyncRequest;
            if (request == null)
                return;

            GHCefWebViewControl control = handler?.CefControl;
            if (control == null)
            {
                request.TrySetResult("null");
                return;
            }

            EvaluateJavaScript(control, request);
        }

        private static async void EvaluateJavaScript(GHCefWebViewControl control, EvaluateJavaScriptAsyncRequest request)
        {
            try
            {
                string result = await control.EvaluateScriptAsync(request.Script);
                request.TrySetResult(result ?? "null");
            }
            catch (Exception ex)
            {
                request.TrySetException(ex);
            }
        }

        void IWebViewDelegate.LoadUrl(string url)
        {
            if (url == null)
                return;

            GHCefWebViewControl control = CefControl;
            if (control == null)
                return;

            _currentNavigationEvent = WebNavigationEvent.NewPage;
            control.Navigate(url);
        }

        void IWebViewDelegate.LoadHtml(string html, string baseUrl)
        {
            if (html == null)
                return;

            GHCefWebViewControl control = CefControl;
            if (control == null)
                return;

            _currentNavigationEvent = WebNavigationEvent.NewPage;
            control.LoadHtml(html, baseUrl);
        }
    }
}
#endif

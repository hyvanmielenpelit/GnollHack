#if GNH_AVALONIA && LINUX
using System;
using Xilium.CefGlue;

namespace GnollHackM
{
    /* The CEF client of one GHCefWebViewControl. Its handlers are created once and forward to the control. Every
       callback runs on the Avalonia UI thread inside CefRuntime.DoMessageLoopWork, except GetResourceRequestHandler
       (IO thread). An exception escaping a callback would end the process, so each one catches and logs. */
    internal sealed class GHCefClient : CefClient
    {
        private readonly IGHCefClientTarget _target;
        private readonly GHCefRenderHandler _renderHandler;
        private readonly GHCefLifeSpanHandler _lifeSpanHandler;
        private readonly GHCefLoadHandler _loadHandler;
        private readonly GHCefRequestHandler _requestHandler;
        private readonly GHCefDisplayHandler _displayHandler;
        private readonly GHCefJSDialogHandler _jsDialogHandler;
        private readonly GHCefContextMenuHandler _contextMenuHandler;
        private readonly GHCefDialogHandler _dialogHandler;

        internal GHCefClient(IGHCefClientTarget target)
        {
            _dialogHandler = new GHCefDialogHandler(target);
            _target = target;
            _renderHandler = new GHCefRenderHandler(target);
            _lifeSpanHandler = new GHCefLifeSpanHandler(target);
            _loadHandler = new GHCefLoadHandler(target);
            _requestHandler = new GHCefRequestHandler(target);
            _displayHandler = new GHCefDisplayHandler(target);
            _jsDialogHandler = new GHCefJSDialogHandler();
            _contextMenuHandler = new GHCefContextMenuHandler();
        }

        protected override CefRenderHandler GetRenderHandler()
        {
            return _renderHandler;
        }

        protected override CefLifeSpanHandler GetLifeSpanHandler()
        {
            return _lifeSpanHandler;
        }

        protected override CefLoadHandler GetLoadHandler()
        {
            return _loadHandler;
        }

        protected override CefRequestHandler GetRequestHandler()
        {
            return _requestHandler;
        }

        protected override CefDisplayHandler GetDisplayHandler()
        {
            return _displayHandler;
        }

        protected override CefDialogHandler GetDialogHandler()
        {
            return _dialogHandler;
        }

        protected override CefJSDialogHandler GetJSDialogHandler()
        {
            return _jsDialogHandler;
        }

        protected override CefContextMenuHandler GetContextMenuHandler()
        {
            return _contextMenuHandler;
        }

        /* The renderer's bridge messages (main frame only) and script evaluation results */
        protected override bool OnProcessMessageReceived(CefBrowser browser, CefFrame frame, CefProcessId sourceProcess, CefProcessMessage message)
        {
            try
            {
                if (message == null)
                    return false;

                string name = message.Name;
                if (name == GHCefRenderProcessHandler.BridgeMessageName)
                {
                    if (frame == null || !frame.IsMain)
                        return true;

                    string text;
                    string frameUrl;
                    using (CefListValue arguments = message.Arguments)
                    {
                        if (arguments == null || arguments.Count < 2)
                            return true;
                        text = arguments.GetString(0) ?? "";
                        frameUrl = arguments.GetString(1) ?? "";
                    }
                    _target.OnBridgeMessage(text, frameUrl);
                    return true;
                }

                if (name == GHCefRenderProcessHandler.EvalResultMessageName)
                {
                    int id;
                    bool success;
                    string json;
                    using (CefListValue arguments = message.Arguments)
                    {
                        if (arguments == null || arguments.Count < 3)
                            return true;
                        id = arguments.GetInt(0);
                        success = arguments.GetBool(1);
                        json = arguments.GetString(2);
                    }
                    _target.OnEvalResult(id, success, json);
                    return true;
                }

                return false;
            }
            catch (Exception ex)
            {
                GHCefRuntime.Log("GHCefClient: OnProcessMessageReceived failed: " + ex.Message);
                return false;
            }
            finally
            {
                message?.Dispose();
            }
        }

        /* Off-screen rendering: the view's size and scale, and the pixels CEF paints */
        private sealed class GHCefRenderHandler : CefRenderHandler
        {
            private readonly IGHCefClientTarget _target;

            public GHCefRenderHandler(IGHCefClientTarget target)
            {
                _target = target;
            }

            protected override CefAccessibilityHandler GetAccessibilityHandler()
            {
                return null;
            }

            /* Never empty, which would crash CEF */
            protected override void GetViewRect(CefBrowser browser, out CefRectangle rect)
            {
                rect = new CefRectangle(0, 0, 1, 1);
                try
                {
                    _target.GetViewRect(out rect);
                    if (rect.Width < 1 || rect.Height < 1)
                        rect = new CefRectangle(0, 0, Math.Max(1, rect.Width), Math.Max(1, rect.Height));
                }
                catch (Exception ex)
                {
                    rect = new CefRectangle(0, 0, 1, 1);
                    GHCefRuntime.Log("GHCefClient: GetViewRect failed: " + ex.Message);
                }
            }

            protected override bool GetScreenInfo(CefBrowser browser, CefScreenInfo screenInfo)
            {
                try
                {
                    return _target.GetScreenInfo(screenInfo);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: GetScreenInfo failed: " + ex.Message);
                    return false;
                }
            }

            protected override void OnPaint(CefBrowser browser, CefPaintElementType type, CefRectangle[] dirtyRects, IntPtr buffer, int width, int height)
            {
                try
                {
                    _target.OnPaint(type, dirtyRects, buffer, width, height);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnPaint failed: " + ex.Message);
                }
            }

            protected override void OnPopupShow(CefBrowser browser, bool show)
            {
                try
                {
                    _target.OnPopupShow(show);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnPopupShow failed: " + ex.Message);
                }
            }

            protected override void OnPopupSize(CefBrowser browser, CefRectangle rect)
            {
                try
                {
                    _target.OnPopupSize(rect);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnPopupSize failed: " + ex.Message);
                }
            }

            protected override void OnVirtualKeyboardRequested(CefBrowser browser, CefTextInputMode inputMode)
            {
                try
                {
                    _target.OnVirtualKeyboardRequested(inputMode);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnVirtualKeyboardRequested failed: " + ex.Message);
                }
            }

            /* Shared textures are not used: the GPU is off */
            protected override void OnAcceleratedPaint(CefBrowser browser, CefPaintElementType type, CefRectangle[] dirtyRects, CefAcceleratedPaintInfo info)
            {
            }

            protected override void OnScrollOffsetChanged(CefBrowser browser, double x, double y)
            {
            }

            protected override void OnImeCompositionRangeChanged(CefBrowser browser, CefRange selectedRange, CefRectangle[] characterBounds)
            {
            }

            protected override void OnTextSelectionChanged(CefBrowser browser, string selectedText, CefRange selectedRange)
            {
                try
                {
                    _target.OnTextSelectionChanged(selectedText);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: text selection change failed: " + ex.Message);
                }
            }
        }

        /* Browser creation and destruction; popups are never created */
        private sealed class GHCefLifeSpanHandler : CefLifeSpanHandler
        {
            private readonly IGHCefClientTarget _target;

            public GHCefLifeSpanHandler(IGHCefClientTarget target)
            {
                _target = target;
            }

            /* The control keeps the browser, so it is passed on as it is */
            protected override void OnAfterCreated(CefBrowser browser)
            {
                try
                {
                    _target.OnBrowserCreated(browser);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnAfterCreated failed: " + ex.Message);
                }
            }

            /* A windowless browser is destroyed at once */
            protected override bool DoClose(CefBrowser browser)
            {
                return false;
            }

            protected override void OnBeforeClose(CefBrowser browser)
            {
                try
                {
                    _target.OnBrowserClosed();
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnBeforeClose failed: " + ex.Message);
                }
            }

            /* Cancelled; an absolute http(s) target is reported as a new window request */
            protected override bool OnBeforePopup(CefBrowser browser, CefFrame frame, int popupId, string targetUrl, string targetFrameName,
                CefWindowOpenDisposition targetDisposition, bool userGesture, CefPopupFeatures popupFeatures, CefWindowInfo windowInfo,
                ref CefClient client, CefBrowserSettings settings, ref CefDictionaryValue extraInfo, ref bool noJavascriptAccess)
            {
                try
                {
                    if (IsHttpUrl(targetUrl))
                        _target.OnNewWindowRequested(targetUrl);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnBeforePopup failed: " + ex.Message);
                }
                return true;
            }
        }

        /* Main-frame load results and the history state */
        private sealed class GHCefLoadHandler : CefLoadHandler
        {
            private readonly IGHCefClientTarget _target;

            /* The main-frame URL whose load failed; an error page committed for it does not also count as a success */
            private string _failedUrl = null;

            public GHCefLoadHandler(IGHCefClientTarget target)
            {
                _target = target;
            }

            protected override void OnLoadingStateChange(CefBrowser browser, bool isLoading, bool canGoBack, bool canGoForward)
            {
                try
                {
                    if (isLoading)
                        _failedUrl = null;
                    _target.OnHistoryChanged(canGoBack, canGoForward);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnLoadingStateChange failed: " + ex.Message);
                }
            }

            protected override void OnLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode)
            {
                try
                {
                    if (frame == null || !frame.IsMain)
                        return;

                    string url = frame.Url;
                    string failedUrl = _failedUrl;
                    _failedUrl = null;
                    if (failedUrl != null && failedUrl == url)
                        return;

                    _target.OnLoadCompleted(url, true);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnLoadEnd failed: " + ex.Message);
                }
            }

            /* Aborted loads, among them navigations cancelled by OnBeforeBrowse or superseded by another, are not
               reported */
            protected override void OnLoadError(CefBrowser browser, CefFrame frame, CefErrorCode errorCode, string errorText, string failedUrl)
            {
                try
                {
                    if (frame == null || !frame.IsMain || errorCode == CefErrorCode.Aborted)
                        return;

                    GHCefRuntime.Log("GHCefClient: main frame load failed: " + errorCode.ToString() + " " + (errorText ?? ""));
                    _failedUrl = failedUrl;
                    _target.OnLoadCompleted(failedUrl, false);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnLoadError failed: " + ex.Message);
                }
            }
        }

        /* Navigation filtering */
        private sealed class GHCefRequestHandler : CefRequestHandler
        {
            private readonly IGHCefClientTarget _target;

            public GHCefRequestHandler(IGHCefClientTarget target)
            {
                _target = target;
            }

            /* Main-frame navigations other than redirects go to the target, which may cancel them */
            protected override bool OnBeforeBrowse(CefBrowser browser, CefFrame frame, CefRequest request, bool userGesture, bool isRedirect)
            {
                try
                {
                    if (frame == null || request == null || !frame.IsMain || isRedirect)
                        return false;

                    return _target.OnNavigationStarting(request.Url);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnBeforeBrowse failed: " + ex.Message);
                    return false;
                }
            }

            /* IO thread; the default resource handling */
            protected override CefResourceRequestHandler GetResourceRequestHandler(CefBrowser browser, CefFrame frame, CefRequest request,
                bool isNavigation, bool isDownload, string requestInitiator, ref bool disableDefaultHandling)
            {
                return null;
            }

            /* Middle-click, Ctrl+click and similar: cancelled; an absolute http(s) target is reported as a new window
               request */
            protected override bool OnOpenUrlFromTab(CefBrowser browser, CefFrame frame, string targetUrl, CefWindowOpenDisposition targetDisposition, bool userGesture)
            {
                try
                {
                    if (IsHttpUrl(targetUrl))
                        _target.OnNewWindowRequested(targetUrl);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnOpenUrlFromTab failed: " + ex.Message);
                }
                return true;
            }
        }

        /* Address, cursor and console */
        private sealed class GHCefDisplayHandler : CefDisplayHandler
        {
            private readonly IGHCefClientTarget _target;

            public GHCefDisplayHandler(IGHCefClientTarget target)
            {
                _target = target;
            }

            protected override void OnAddressChange(CefBrowser browser, CefFrame frame, string url)
            {
                try
                {
                    if (frame != null && frame.IsMain)
                        _target.OnAddressChanged(url);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnAddressChange failed: " + ex.Message);
                }
            }

            protected override bool OnCursorChange(CefBrowser browser, IntPtr cursorHandle, CefCursorType type, CefCursorInfo customCursorInfo)
            {
                try
                {
                    _target.OnCursorChange(type);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnCursorChange failed: " + ex.Message);
                }
                return true;
            }

            /* Logged only with debug log messages on, and page pointer probe and file chooser lines while the pointer
               log is on; the console output is left to CEF */
            protected override bool OnConsoleMessage(CefBrowser browser, CefLogSeverity level, string message, string source, int line)
            {
                try
                {
                    if (GHAvaloniaPointerLog.IsEnabled && message != null)
                    {
                        if (message.StartsWith(GHCefOverseerHost.ProbePrefix, StringComparison.Ordinal))
                            GHAvaloniaPointerLog.Log("PAGE " + message.Substring(GHCefOverseerHost.ProbePrefix.Length));
                        else if (message.IndexOf("user activation", StringComparison.OrdinalIgnoreCase) >= 0
                            || message.IndexOf("file chooser", StringComparison.OrdinalIgnoreCase) >= 0)
                            GHAvaloniaPointerLog.Log("CONSOLE " + message);
                    }
                    if (GnollHackX.GHApp.DebugLogMessages)
                        GHCefRuntime.Log("GHCefClient: console " + level.ToString() + ": " + (message ?? "") + " (" + (source ?? "") + ":" + line.ToString() + ")");
                }
                catch (Exception)
                {
                    /* Logging only */
                }
                return false;
            }
        }

        /* JavaScript dialogs are answered at once: alert with OK, confirm and prompt with Cancel; leaving the page is
           always allowed */
        private sealed class GHCefJSDialogHandler : CefJSDialogHandler
        {
            private const int MaxLoggedMessageLength = 200;

            protected override bool OnJSDialog(CefBrowser browser, string originUrl, CefJSDialogType dialogType, string message_text,
                string default_prompt_text, CefJSDialogCallback callback, out bool suppress_message)
            {
                suppress_message = false;
                try
                {
                    string text = message_text ?? "";
                    if (text.Length > MaxLoggedMessageLength)
                        text = text.Substring(0, MaxLoggedMessageLength) + "...";
                    GHCefRuntime.Log("GHCefClient: JavaScript " + dialogType.ToString() + " from " + (originUrl ?? "") + " answered automatically: " + text);
                    callback?.Continue(dialogType == CefJSDialogType.Alert, null);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnJSDialog failed: " + ex.Message);
                }
                return true;
            }

            protected override bool OnBeforeUnloadDialog(CefBrowser browser, string messageText, bool isReload, CefJSDialogCallback callback)
            {
                try
                {
                    callback?.Continue(true, null);
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnBeforeUnloadDialog failed: " + ex.Message);
                }
                return true;
            }

            protected override void OnResetDialogState(CefBrowser browser)
            {
            }

            protected override void OnDialogClosed(CefBrowser browser)
            {
            }
        }

        /* File dialogs through the control, which answers the callback later */
        private sealed class GHCefDialogHandler : CefDialogHandler
        {
            private readonly IGHCefClientTarget _target;

            public GHCefDialogHandler(IGHCefClientTarget target)
            {
                _target = target;
            }

            protected override bool OnFileDialog(CefBrowser browser, CefFileDialogMode mode, string title, string defaultFilePath,
                string[] acceptFilters, string[] acceptExtensions, string[] acceptDescriptions, CefFileDialogCallback callback)
            {
                try
                {
                    _target.OnFileDialog(mode, title, defaultFilePath, acceptFilters, acceptExtensions, callback);
                    return true;
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: file dialog failed: " + ex.Message);
                    return false;
                }
            }
        }

        /* No context menus */
        private sealed class GHCefContextMenuHandler : CefContextMenuHandler
        {
            protected override void OnBeforeContextMenu(CefBrowser browser, CefFrame frame, CefContextMenuParams state, CefMenuModel model)
            {
                try
                {
                    model?.Clear();
                }
                catch (Exception ex)
                {
                    GHCefRuntime.Log("GHCefClient: OnBeforeContextMenu failed: " + ex.Message);
                }
            }
        }

        private static bool IsHttpUrl(string url)
        {
            Uri uri;
            return !string.IsNullOrEmpty(url) && Uri.TryCreate(url, UriKind.Absolute, out uri)
                && (uri.Scheme == Uri.UriSchemeHttp || uri.Scheme == Uri.UriSchemeHttps);
        }
    }
}
#endif

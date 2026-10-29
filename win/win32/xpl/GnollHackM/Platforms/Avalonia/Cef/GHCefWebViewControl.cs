#if GNH_AVALONIA && LINUX
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using Avalonia.VisualTree;
using GnollHackX;
using Xilium.CefGlue;
using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvDispatcherPriority = Avalonia.Threading.DispatcherPriority;
using AvRect = Avalonia.Rect;
using AvWriteableBitmap = Avalonia.Media.Imaging.WriteableBitmap;

namespace GnollHackM
{
    /* What GHCefClient reports to its control. Every member is called on the Avalonia UI thread, inside
       CefRuntime.DoMessageLoopWork, so none may block or re-enter CEF's message loop. */
    internal interface IGHCefClientTarget
    {
        /* The view in device-independent units, at least 1 x 1 */
        void GetViewRect(out CefRectangle rect);
        bool GetScreenInfo(CefScreenInfo info);
        /* BGRA, premultiplied, upper-left origin; size and dirty rectangles in pixels; the buffer is valid only
           during the call */
        void OnPaint(CefPaintElementType type, CefRectangle[] dirtyRects, IntPtr buffer, int width, int height);
        void OnPopupShow(bool show);
        void OnPopupSize(CefRectangle rect);
        void OnCursorChange(CefCursorType type);
        void OnVirtualKeyboardRequested(CefTextInputMode mode);
        /* The control keeps the browser until OnBrowserClosed */
        void OnBrowserCreated(CefBrowser browser);
        void OnBrowserClosed();
        /* Main-frame navigations that are not redirects; true cancels */
        bool OnNavigationStarting(string url);
        void OnLoadCompleted(string url, bool success);
        void OnHistoryChanged(bool canGoBack, bool canGoForward);
        void OnAddressChanged(string url);
        void OnNewWindowRequested(string url);
        void OnBridgeMessage(string text, string frameUrl);
        void OnEvalResult(int id, bool success, string json);
        /* The page's current text selection, empty when nothing is selected */
        void OnTextSelectionChanged(string selectedText);
        /* A page's file dialog; the callback is answered exactly once, later */
        void OnFileDialog(CefFileDialogMode mode, string title, string defaultFilePath, string[] acceptFilters,
            string[] acceptExtensions, CefFileDialogCallback callback);
    }

    /* A Chromium page rendered off screen by CEF and drawn as an Avalonia control. Every public member is UI
       thread only. Calls made before the browser exists are queued: the last navigation, then scripts and
       evaluations in order. The browser is closed only by CloseBrowser, after which the control stays empty. */
    public sealed class GHCefWebViewControl : Avalonia.Controls.Control, IGHCefClientTarget
    {
        private const int EvalTimeoutMilliseconds = 10000;
        private const int WheelDeltaPerUnit = 40;
        private const int KeptHtmlFiles = 4;
        private const string UnavailableText = "Web view unavailable";

        private static readonly Regex s_baseTagRegex = new Regex(@"<base[\s/>]", RegexOptions.IgnoreCase | RegexOptions.CultureInvariant);
        private static readonly Regex s_headTagRegex = new Regex(@"<head(\s[^>]*)?>", RegexOptions.IgnoreCase | RegexOptions.CultureInvariant);
        private static readonly Regex s_htmlTagRegex = new Regex(@"<html(\s[^>]*)?>", RegexOptions.IgnoreCase | RegexOptions.CultureInvariant);
        private static readonly Dictionary<CefCursorType, Avalonia.Input.Cursor> s_cursors = new Dictionary<CefCursorType, Avalonia.Input.Cursor>();
        private static int s_htmlFileCounter = 0;

        /* Browser */
        private GHCefClient _client = null;
        private CefBrowser _browser = null;
        private CefBrowserHost _host = null;
        /* The page's text selection as CEF last reported it, for copy and cut */
        private string _selectedText = "";
        private bool _creating = false;
        private bool _startPosted = false;
        private bool _startPending = false;
        private bool _closeRequested = false;
        private bool _closeSent = false;
        private bool _unavailable = false;
        private bool _hidden = false;
        private string _address = null;

        /* Calls made before the browser exists */
        private string _pendingUrl = null;
        private readonly List<string> _pendingScripts = new List<string>();
        private readonly List<KeyValuePair<int, string>> _pendingEvals = new List<KeyValuePair<int, string>>();
        private readonly Dictionary<int, PendingEval> _evals = new Dictionary<int, PendingEval>();
        private int _nextEvalId = 0;

        /* Visual tree */
        private bool _attached = false;
        private bool _visibilityUpdatePosted = false;
        private Avalonia.Controls.TopLevel _topLevel = null;
        private double _scaling = 1.0;
        private readonly List<Avalonia.Visual> _visibilityVisuals = new List<Avalonia.Visual>();

        /* Painting */
        private AvWriteableBitmap _viewBitmap = null;
        private AvWriteableBitmap _popupBitmap = null;
        private bool _popupShown = false;
        private CefRectangle _popupRect;
        private Avalonia.Media.Color _backgroundColor = Avalonia.Media.Color.FromRgb(0x12, 0x12, 0x12);
        private Avalonia.Media.IBrush _backgroundBrush;

        /* Input */
        private readonly Dictionary<int, Avalonia.Point> _activeTouches = new Dictionary<int, Avalonia.Point>();
        private int _lastClickCount = 1;

        /* Spike counters */
        private long _statPaints = 0;
        private long _statDirtyPixels = 0;
        private long _statCopyTicks = 0;
        private long _statMaxCopyTicks = 0;
        private long _createTimestamp = 0;
        private double _firstPaintMilliseconds = -1.0;
        private bool _firstPaintReported = false;

        public GHCefWebViewControl()
        {
            Focusable = true;
            ClipToBounds = true;
            _backgroundBrush = new Avalonia.Media.Immutable.ImmutableSolidColorBrush(_backgroundColor);
        }

        /* The main frame's URL as CEF last reported it */
        public string Address
        {
            get { return _address ?? ""; }
        }

        public bool CanGoBack { get; private set; }

        public bool CanGoForward { get; private set; }

        /* Whether a CEF browser exists: from its creation until CEF reports it closed */
        public bool HasBrowser
        {
            get { return _browser != null; }
        }

        /* Drawn under the page; given to the browser when it is created */
        public Avalonia.Media.Color BackgroundColor
        {
            get { return _backgroundColor; }
            set
            {
                _backgroundColor = value;
                _backgroundBrush = new Avalonia.Media.Immutable.ImmutableSolidColorBrush(value);
                InvalidateVisual();
            }
        }

        /* Called synchronously with the URL of a main-frame navigation; returning true cancels it */
        public Func<string, bool> NavigationStarting { get; set; }

        /* The events below are posted to the dispatcher, never raised inside a CEF callback */
        public event Action<string, bool> NavigationCompleted;
        public event Action<bool, bool> HistoryChanged;
        public event Action<Uri> NewWindowRequested;
        public event Action<string, string> BridgeMessageReceived;

        public void Navigate(string url)
        {
            if (_closeRequested || string.IsNullOrEmpty(url))
                return;

            if (_browser == null)
            {
                _pendingUrl = url;
                return;
            }

            try
            {
                using (CefFrame frame = _browser.GetMainFrame())
                {
                    frame?.LoadUrl(url);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: navigation failed: " + ex.Message);
            }
        }

        /* Writes the page to a file in the data folder and navigates to it, so that relative links resolve
           against baseUrl through an inserted <base> element */
        public void LoadHtml(string html, string baseUrl)
        {
            if (_closeRequested)
                return;

            string url = null;
            try
            {
                url = WriteHtmlFile(html ?? "", baseUrl);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: cannot write the HTML page: " + ex.Message);
            }

            if (url != null)
                Navigate(url);
        }

        // WORKAROUND(WA-CEF-0004): CefBrowser.GoBack does nothing after links followed without a user gesture
        /* Through the page's own history.back(), else the browser's GoBack when there is no main frame */
        public void GoBack()
        {
            NavigateHistory(-1);
        }

        public void GoForward()
        {
            NavigateHistory(1);
        }

        private void NavigateHistory(int offset)
        {
            try
            {
                if (_browser == null || _closeRequested)
                    return;
                using (CefFrame frame = _browser.GetMainFrame())
                {
                    if (frame != null && frame.IsValid)
                    {
                        frame.ExecuteJavaScript(offset < 0 ? "history.back();" : "history.forward();", frame.Url, 1);
                        return;
                    }
                }
                if (offset < 0)
                    _browser.GoBack();
                else
                    _browser.GoForward();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: " + (offset < 0 ? "GoBack" : "GoForward") + " failed: "
                    + ex.Message);
            }
        }

        public void Reload()
        {
            try
            {
                if (_browser != null && !_closeRequested)
                    _browser.Reload();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: Reload failed: " + ex.Message);
            }
        }

        public void StopLoad()
        {
            try
            {
                if (_browser != null && !_closeRequested)
                    _browser.StopLoad();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: StopLoad failed: " + ex.Message);
            }
        }

        /* Runs a script in the main frame without waiting for a result */
        public void ExecuteScript(string script)
        {
            if (_closeRequested || string.IsNullOrEmpty(script))
                return;

            if (_browser == null)
            {
                _pendingScripts.Add(script);
                return;
            }

            RunScript(script);
        }

        /* The script's result as JSON (a string result as is), or null on failure or after 10 s */
        public Task<string> EvaluateScriptAsync(string script)
        {
            TaskCompletionSource<string> source = new TaskCompletionSource<string>(TaskCreationOptions.RunContinuationsAsynchronously);
            if (_closeRequested || string.IsNullOrEmpty(script))
            {
                source.TrySetResult(null);
                return source.Task;
            }

            int id = ++_nextEvalId;
            PendingEval pending = new PendingEval(source);
            _evals[id] = pending;
            pending.Timeout = Avalonia.Threading.DispatcherTimer.RunOnce(() => CompleteEval(id, null),
                TimeSpan.FromMilliseconds(EvalTimeoutMilliseconds), AvDispatcherPriority.Background);

            if (_browser == null)
                _pendingEvals.Add(new KeyValuePair<int, string>(id, script));
            else if (!SendEval(id, script))
                CompleteEval(id, null);

            return source.Task;
        }

        /* Closes the browser for good; before it exists, it is closed as soon as CEF reports it created */
        public void CloseBrowser()
        {
            _closeRequested = true;
            _pendingUrl = null;
            _pendingScripts.Clear();
            _pendingEvals.Clear();

            if (_host != null)
            {
                if (_closeSent)
                    return;
                _closeSent = true;
                try
                {
                    _host.CloseBrowser(true);
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("GHCefWebViewControl: CloseBrowser failed: " + ex.Message);
                }
            }
            else if (!_creating)
            {
                CompleteAllEvals();
            }
        }

        /* Counters since the last call, for the runtime's periodic log line */
        public string GetStatsAndReset()
        {
            long paints = _statPaints;
            double dirtyMegapixels = _statDirtyPixels / 1000000.0;
            double meanMilliseconds = paints > 0 ? TicksToMilliseconds(_statCopyTicks) / paints : 0.0;
            double maxMilliseconds = TicksToMilliseconds(_statMaxCopyTicks);
            _statPaints = 0;
            _statDirtyPixels = 0;
            _statCopyTicks = 0;
            _statMaxCopyTicks = 0;

            string stats = string.Format(CultureInfo.InvariantCulture, "paints {0}, dirty {1:0.00} MP, copy mean {2:0.000} ms, max {3:0.000} ms",
                paints, dirtyMegapixels, meanMilliseconds, maxMilliseconds);
            if (_firstPaintMilliseconds >= 0 && !_firstPaintReported)
            {
                _firstPaintReported = true;
                stats += string.Format(CultureInfo.InvariantCulture, ", first paint {0:0} ms after creation", _firstPaintMilliseconds);
            }
            return stats;
        }

        /* Lifecycle */

        protected override void OnAttachedToVisualTree(Avalonia.VisualTreeAttachmentEventArgs e)
        {
            base.OnAttachedToVisualTree(e);
            _attached = true;
            _topLevel = Avalonia.Controls.TopLevel.GetTopLevel(this);
            if (_topLevel != null)
            {
                _scaling = _topLevel.RenderScaling;
                _topLevel.ScalingChanged += TopLevel_ScalingChanged;
            }
            SubscribeVisibility();
            PostStart();
        }

        protected override void OnDetachedFromVisualTree(Avalonia.VisualTreeAttachmentEventArgs e)
        {
            base.OnDetachedFromVisualTree(e);
            _attached = false;
            if (_topLevel != null)
            {
                _topLevel.ScalingChanged -= TopLevel_ScalingChanged;
                _topLevel = null;
            }
            UnsubscribeVisibility();
            _activeTouches.Clear();
            UpdateVisibility();
        }

        protected override void OnSizeChanged(Avalonia.Controls.SizeChangedEventArgs e)
        {
            base.OnSizeChanged(e);
            if (_host != null)
            {
                try
                {
                    _host.NotifyMoveOrResizeStarted();
                    _host.WasResized();
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("GHCefWebViewControl: resize failed: " + ex.Message);
                }
            }
            else if (_startPending)
            {
                PostStart();
            }
        }

        private void TopLevel_ScalingChanged(object sender, EventArgs e)
        {
            Avalonia.Controls.TopLevel topLevel = _topLevel;
            if (topLevel != null)
                _scaling = topLevel.RenderScaling;
            if (_host == null)
                return;

            try
            {
                _host.NotifyScreenInfoChanged();
                _host.WasResized();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: scaling change failed: " + ex.Message);
            }
        }

        /* Browser creation runs from the dispatcher, never inside a layout pass */
        private void PostStart()
        {
            if (_startPosted)
                return;
            _startPosted = true;
            AvDispatcher.UIThread.Post(StartBrowser, AvDispatcherPriority.Loaded);
        }

        private void StartBrowser()
        {
            _startPosted = false;
            if (!_attached || _closeRequested || _unavailable || _creating)
                return;

            if (_browser != null)
            {
                /* Attached again */
                UpdateVisibility();
                return;
            }

            AvRect bounds = Bounds;
            if (bounds.Width < 1 || bounds.Height < 1)
            {
                _startPending = true;
                return;
            }
            _startPending = false;

            bool registered = false;
            try
            {
                if (!GHCefRuntime.EnsureInitialized())
                {
                    _unavailable = true;
                    InvalidateVisual();
                    return;
                }

                string url = _pendingUrl ?? "about:blank";
                _pendingUrl = null;

                CefWindowInfo windowInfo = CefWindowInfo.Create();
                windowInfo.SetAsWindowless(IntPtr.Zero, false);
                CefBrowserSettings settings = new CefBrowserSettings
                {
                    WindowlessFrameRate = 60,
                    BackgroundColor = new CefColor(255, _backgroundColor.R, _backgroundColor.G, _backgroundColor.B),
                };

                _client = new GHCefClient(this);
                _creating = true;
                _createTimestamp = Stopwatch.GetTimestamp();
                _firstPaintMilliseconds = -1.0;
                _firstPaintReported = false;
                GHCefRuntime.Register(this);
                registered = true;
                CefBrowserHost.CreateBrowser(windowInfo, _client, settings, url);
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: browser creation requested");
            }
            catch (Exception ex)
            {
                _creating = false;
                _client = null;
                if (registered)
                    GHCefRuntime.Unregister(this);
                _unavailable = true;
                InvalidateVisual();
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: cannot create a browser: " + ex.ToString());
            }
        }

        // WORKAROUND(WA-AVALONIA-0017): IsEffectivelyVisible has no change event
        /* Visibility: CEF stops painting a hidden browser. IsEffectivelyVisible has no public change event, so
           IsVisible is watched on the control and its ancestors while attached. */
        private void SubscribeVisibility()
        {
            UnsubscribeVisibility();
            for (Avalonia.Visual visual = this; visual != null; visual = visual.GetVisualParent())
            {
                visual.PropertyChanged += Visual_PropertyChanged;
                _visibilityVisuals.Add(visual);
            }
        }

        private void UnsubscribeVisibility()
        {
            foreach (Avalonia.Visual visual in _visibilityVisuals)
                visual.PropertyChanged -= Visual_PropertyChanged;
            _visibilityVisuals.Clear();
        }

        private void Visual_PropertyChanged(object sender, Avalonia.AvaloniaPropertyChangedEventArgs e)
        {
            if (e.Property != Avalonia.Visual.IsVisibleProperty || _visibilityUpdatePosted)
                return;

            /* IsEffectivelyVisible is up to date once the change has been processed */
            _visibilityUpdatePosted = true;
            AvDispatcher.UIThread.Post(() =>
            {
                _visibilityUpdatePosted = false;
                UpdateVisibility();
            }, AvDispatcherPriority.Normal);
        }

        private void UpdateVisibility()
        {
            if (_host == null)
                return;

            bool hidden = !_attached || !IsEffectivelyVisible;
            if (hidden == _hidden)
                return;
            _hidden = hidden;

            try
            {
                _host.WasHidden(hidden);
                if (!hidden)
                    _host.Invalidate(CefPaintElementType.View);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: visibility change failed: " + ex.Message);
            }
        }

        /* Painting */

        public override void Render(Avalonia.Media.DrawingContext context)
        {
            AvRect bounds = new AvRect(Bounds.Size);
            if (bounds.Width <= 0 || bounds.Height <= 0)
                return;

            context.FillRectangle(_backgroundBrush, bounds);

            if (_unavailable)
            {
                Avalonia.Media.FormattedText text = new Avalonia.Media.FormattedText(UnavailableText, CultureInfo.CurrentUICulture,
                    Avalonia.Media.FlowDirection.LeftToRight, Avalonia.Media.Typeface.Default, 16, Avalonia.Media.Brushes.Gray);
                context.DrawText(text, new Avalonia.Point((bounds.Width - text.Width) / 2, (bounds.Height - text.Height) / 2));
                return;
            }

            /* The bitmaps are in pixels; drawn at their own size in device-independent units, without stretching */
            double scaling = _scaling > 0 ? _scaling : 1.0;
            AvWriteableBitmap view = _viewBitmap;
            if (view != null)
            {
                Avalonia.PixelSize size = view.PixelSize;
                context.DrawImage(view, new AvRect(0, 0, size.Width, size.Height), new AvRect(0, 0, size.Width / scaling, size.Height / scaling));
            }

            AvWriteableBitmap popup = _popupBitmap;
            if (_popupShown && popup != null)
            {
                Avalonia.PixelSize size = popup.PixelSize;
                context.DrawImage(popup, new AvRect(0, 0, size.Width, size.Height),
                    new AvRect(_popupRect.X, _popupRect.Y, size.Width / scaling, size.Height / scaling));
            }
        }

        private void GetViewRectCore(out CefRectangle rect)
        {
            AvRect bounds = Bounds;
            int width = Math.Max(1, (int)Math.Ceiling(bounds.Width));
            int height = Math.Max(1, (int)Math.Ceiling(bounds.Height));
            rect = new CefRectangle(0, 0, width, height);
        }

        void IGHCefClientTarget.GetViewRect(out CefRectangle rect)
        {
            rect = new CefRectangle(0, 0, 1, 1);
            try
            {
                GetViewRectCore(out rect);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: GetViewRect failed: " + ex.Message);
            }
        }

        /* The screen is reported as the view, which keeps CEF's popups inside it */
        bool IGHCefClientTarget.GetScreenInfo(CefScreenInfo info)
        {
            try
            {
                Avalonia.Controls.TopLevel topLevel = Avalonia.Controls.TopLevel.GetTopLevel(this);
                if (topLevel != null)
                    _scaling = topLevel.RenderScaling;

                CefRectangle view;
                GetViewRectCore(out view);
                info.DeviceScaleFactor = (float)_scaling;
                info.Rectangle = view;
                info.AvailableRectangle = view;
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: GetScreenInfo failed: " + ex.Message);
                return false;
            }
        }

        void IGHCefClientTarget.OnPaint(CefPaintElementType type, CefRectangle[] dirtyRects, IntPtr buffer, int width, int height)
        {
            try
            {
                if (buffer == IntPtr.Zero || width <= 0 || height <= 0)
                    return;

                long start = Stopwatch.GetTimestamp();
                bool isPopup = type == CefPaintElementType.Popup;
                AvWriteableBitmap bitmap = isPopup ? _popupBitmap : _viewBitmap;
                bool whole = false;
                if (bitmap == null || bitmap.PixelSize.Width != width || bitmap.PixelSize.Height != height)
                {
                    AvWriteableBitmap previous = bitmap;
                    bitmap = new AvWriteableBitmap(new Avalonia.PixelSize(width, height), new Avalonia.Vector(96, 96),
                        Avalonia.Platform.PixelFormats.Bgra8888, Avalonia.Platform.AlphaFormat.Premul);
                    if (isPopup)
                        _popupBitmap = bitmap;
                    else
                        _viewBitmap = bitmap;
                    previous?.Dispose();
                    whole = true;
                }

                long pixels = CopyPixels(bitmap, buffer, width, height, whole ? null : dirtyRects);

                long elapsed = Stopwatch.GetTimestamp() - start;
                _statPaints++;
                _statDirtyPixels += pixels;
                _statCopyTicks += elapsed;
                if (elapsed > _statMaxCopyTicks)
                    _statMaxCopyTicks = elapsed;
                if (!isPopup && _firstPaintMilliseconds < 0 && _createTimestamp != 0)
                    _firstPaintMilliseconds = TicksToMilliseconds(Stopwatch.GetTimestamp() - _createTimestamp);

                InvalidateVisual();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnPaint failed: " + ex.Message);
            }
        }

        /* Copies the dirty rectangles, or the whole buffer when there are none, row by row; returns the pixel count */
        private static unsafe long CopyPixels(AvWriteableBitmap bitmap, IntPtr buffer, int width, int height, CefRectangle[] dirtyRects)
        {
            long pixels = 0;
            using (Avalonia.Platform.ILockedFramebuffer framebuffer = bitmap.Lock())
            {
                byte* source = (byte*)buffer;
                byte* destination = (byte*)framebuffer.Address;
                int sourceStride = width * 4;
                int destinationStride = framebuffer.RowBytes;
                int maxWidth = Math.Min(width, framebuffer.Size.Width);
                int maxHeight = Math.Min(height, framebuffer.Size.Height);

                if (dirtyRects == null || dirtyRects.Length == 0)
                {
                    pixels += CopyRows(source, sourceStride, destination, destinationStride, 0, 0, maxWidth, maxHeight);
                }
                else
                {
                    int i;
                    for (i = 0; i < dirtyRects.Length; i++)
                    {
                        CefRectangle rect = dirtyRects[i];
                        int x0 = Math.Max(0, rect.X);
                        int y0 = Math.Max(0, rect.Y);
                        int x1 = Math.Min(maxWidth, rect.X + rect.Width);
                        int y1 = Math.Min(maxHeight, rect.Y + rect.Height);
                        if (x1 > x0 && y1 > y0)
                            pixels += CopyRows(source, sourceStride, destination, destinationStride, x0, y0, x1, y1);
                    }
                }
            }
            return pixels;
        }

        private static unsafe long CopyRows(byte* source, int sourceStride, byte* destination, int destinationStride, int x0, int y0, int x1, int y1)
        {
            long rowBytes = (long)(x1 - x0) * 4;
            long destinationAvailable = destinationStride - (long)x0 * 4;
            int y;
            for (y = y0; y < y1; y++)
            {
                Buffer.MemoryCopy(source + (long)y * sourceStride + (long)x0 * 4, destination + (long)y * destinationStride + (long)x0 * 4,
                    destinationAvailable, rowBytes);
            }
            return (long)(x1 - x0) * (y1 - y0);
        }

        void IGHCefClientTarget.OnPopupShow(bool show)
        {
            try
            {
                if (show)
                    return;

                _popupShown = false;
                _popupRect = new CefRectangle(0, 0, 0, 0);
                AvWriteableBitmap popup = _popupBitmap;
                _popupBitmap = null;
                popup?.Dispose();
                InvalidateVisual();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnPopupShow failed: " + ex.Message);
            }
        }

        /* The popup rectangle, moved into the view as far as it fits */
        void IGHCefClientTarget.OnPopupSize(CefRectangle rect)
        {
            try
            {
                CefRectangle view;
                GetViewRectCore(out view);
                int x = rect.X;
                int y = rect.Y;
                if (x + rect.Width > view.Width)
                    x = view.Width - rect.Width;
                if (y + rect.Height > view.Height)
                    y = view.Height - rect.Height;
                if (x < 0)
                    x = 0;
                if (y < 0)
                    y = 0;

                _popupRect = new CefRectangle(x, y, rect.Width, rect.Height);
                _popupShown = rect.Width > 0 && rect.Height > 0;
                InvalidateVisual();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnPopupSize failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnCursorChange(CefCursorType type)
        {
            try
            {
                Cursor = GetCursor(type);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnCursorChange failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnVirtualKeyboardRequested(CefTextInputMode mode)
        {
            try
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: virtual keyboard requested, input mode " + mode.ToString());
            }
            catch (Exception)
            {
                /* Logging only */
            }
        }

        /* Avalonia's file pickers, which use the desktop portal: the page's extensions as glob patterns, plus all
           files. Chromium's own portal dialog listed folders but no files for extension filters. */
        void IGHCefClientTarget.OnFileDialog(CefFileDialogMode mode, string title, string defaultFilePath, string[] acceptFilters,
            string[] acceptExtensions, CefFileDialogCallback callback)
        {
            _ = ShowFileDialogAsync(mode, title, defaultFilePath, acceptFilters, acceptExtensions, callback);
        }

        void IGHCefClientTarget.OnTextSelectionChanged(string selectedText)
        {
            _selectedText = selectedText ?? "";
        }

        private async Task ShowFileDialogAsync(CefFileDialogMode mode, string title, string defaultFilePath, string[] acceptFilters,
            string[] acceptExtensions, CefFileDialogCallback callback)
        {
            string[] paths = null;
            try
            {
                Avalonia.Platform.Storage.IStorageProvider storage = Avalonia.Controls.TopLevel.GetTopLevel(this)?.StorageProvider;
                if (storage != null)
                {
                    if (mode == CefFileDialogMode.OpenFolder)
                    {
                        IReadOnlyList<Avalonia.Platform.Storage.IStorageFolder> folders = await storage.OpenFolderPickerAsync(
                            new Avalonia.Platform.Storage.FolderPickerOpenOptions { Title = title, AllowMultiple = false });
                        paths = LocalPaths(folders);
                    }
                    else if (mode == CefFileDialogMode.Save)
                    {
                        Avalonia.Platform.Storage.IStorageFile file = await storage.SaveFilePickerAsync(
                            new Avalonia.Platform.Storage.FilePickerSaveOptions
                            {
                                Title = title,
                                SuggestedFileName = string.IsNullOrEmpty(defaultFilePath) ? null : Path.GetFileName(defaultFilePath),
                                FileTypeChoices = FileTypeFilters(acceptFilters, acceptExtensions),
                            });
                        paths = file != null ? LocalPaths(new[] { file }) : null;
                    }
                    else
                    {
                        IReadOnlyList<Avalonia.Platform.Storage.IStorageFile> files = await storage.OpenFilePickerAsync(
                            new Avalonia.Platform.Storage.FilePickerOpenOptions
                            {
                                Title = title,
                                AllowMultiple = mode == CefFileDialogMode.OpenMultiple,
                                FileTypeFilter = FileTypeFilters(acceptFilters, acceptExtensions),
                            });
                        paths = LocalPaths(files);
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: file dialog failed: " + ex.Message);
                paths = null;
            }

            try
            {
                if (paths != null && paths.Length > 0)
                    callback.Continue(paths);
                else
                    callback.Cancel();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: file dialog result failed: " + ex.Message);
            }
            finally
            {
                callback.Dispose();
            }
        }

        private static string[] LocalPaths(IEnumerable<Avalonia.Platform.Storage.IStorageItem> items)
        {
            if (items == null)
                return null;

            List<string> paths = new List<string>();
            foreach (Avalonia.Platform.Storage.IStorageItem item in items)
            {
                string path = item != null ? Avalonia.Platform.Storage.StorageProviderExtensions.TryGetLocalPath(item) : null;
                if (!string.IsNullOrEmpty(path))
                    paths.Add(path);
            }
            return paths.ToArray();
        }

        /* One filter with the page's extensions ("*.png") and MIME types, then all files. CEF gives each filter's
           extensions as a ';'-separated list; a filter that is itself an extension is used as is. */
        private static List<Avalonia.Platform.Storage.FilePickerFileType> FileTypeFilters(string[] acceptFilters, string[] acceptExtensions)
        {
            List<string> patterns = new List<string>();
            List<string> mimeTypes = new List<string>();
            if (acceptExtensions != null)
            {
                foreach (string entry in acceptExtensions)
                {
                    if (string.IsNullOrEmpty(entry))
                        continue;
                    foreach (string extension in entry.Split(';', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries))
                        AddExtensionPattern(patterns, extension);
                }
            }
            if (acceptFilters != null)
            {
                foreach (string filter in acceptFilters)
                {
                    if (string.IsNullOrEmpty(filter))
                        continue;
                    if (filter.StartsWith(".", StringComparison.Ordinal))
                        AddExtensionPattern(patterns, filter);
                    else if (filter.Contains('/') && !mimeTypes.Contains(filter))
                        mimeTypes.Add(filter);
                }
            }

            List<Avalonia.Platform.Storage.FilePickerFileType> filters = new List<Avalonia.Platform.Storage.FilePickerFileType>();
            if (patterns.Count > 0 || mimeTypes.Count > 0)
            {
                filters.Add(new Avalonia.Platform.Storage.FilePickerFileType("Accepted files")
                {
                    Patterns = patterns.Count > 0 ? patterns : null,
                    MimeTypes = mimeTypes.Count > 0 ? mimeTypes : null,
                });
            }
            filters.Add(Avalonia.Platform.Storage.FilePickerFileTypes.All);
            return filters;
        }

        private static void AddExtensionPattern(List<string> patterns, string extension)
        {
            string pattern = "*." + extension.TrimStart('.', '*');
            if (pattern.Length > 2 && !patterns.Contains(pattern, StringComparer.OrdinalIgnoreCase))
                patterns.Add(pattern);
        }

        /* Cursors are created once per type, on the UI thread */
        private static Avalonia.Input.Cursor GetCursor(CefCursorType type)
        {
            Avalonia.Input.Cursor cursor;
            if (!s_cursors.TryGetValue(type, out cursor))
            {
                cursor = new Avalonia.Input.Cursor(MapCursor(type));
                s_cursors[type] = cursor;
            }
            return cursor;
        }

        private static Avalonia.Input.StandardCursorType MapCursor(CefCursorType type)
        {
            switch (type)
            {
            case CefCursorType.Cross:
                return Avalonia.Input.StandardCursorType.Cross;
            case CefCursorType.Hand:
            case CefCursorType.Grab:
            case CefCursorType.Grabbing:
                return Avalonia.Input.StandardCursorType.Hand;
            case CefCursorType.IBeam:
            case CefCursorType.VerticalText:
                return Avalonia.Input.StandardCursorType.Ibeam;
            case CefCursorType.Wait:
                return Avalonia.Input.StandardCursorType.Wait;
            case CefCursorType.Progress:
                return Avalonia.Input.StandardCursorType.AppStarting;
            case CefCursorType.Help:
                return Avalonia.Input.StandardCursorType.Help;
            case CefCursorType.EastResize:
                return Avalonia.Input.StandardCursorType.RightSide;
            case CefCursorType.NorthResize:
                return Avalonia.Input.StandardCursorType.TopSide;
            case CefCursorType.NorthEastResize:
                return Avalonia.Input.StandardCursorType.TopRightCorner;
            case CefCursorType.NorthWestResize:
                return Avalonia.Input.StandardCursorType.TopLeftCorner;
            case CefCursorType.SouthResize:
                return Avalonia.Input.StandardCursorType.BottomSide;
            case CefCursorType.SouthEastResize:
                return Avalonia.Input.StandardCursorType.BottomRightCorner;
            case CefCursorType.SouthWestResize:
                return Avalonia.Input.StandardCursorType.BottomLeftCorner;
            case CefCursorType.WestResize:
                return Avalonia.Input.StandardCursorType.LeftSide;
            case CefCursorType.NorthSouthResize:
            case CefCursorType.RowResize:
                return Avalonia.Input.StandardCursorType.SizeNorthSouth;
            case CefCursorType.EastWestResize:
            case CefCursorType.ColumnResize:
                return Avalonia.Input.StandardCursorType.SizeWestEast;
            case CefCursorType.Move:
            case CefCursorType.MiddlePanning:
            case CefCursorType.MiddlePanningVertical:
            case CefCursorType.MiddlePanningHorizontal:
                return Avalonia.Input.StandardCursorType.SizeAll;
            case CefCursorType.NoDrop:
            case CefCursorType.NotAllowed:
            case CefCursorType.DragAndDropNone:
                return Avalonia.Input.StandardCursorType.No;
            case CefCursorType.DragAndDropMove:
                return Avalonia.Input.StandardCursorType.DragMove;
            case CefCursorType.Copy:
            case CefCursorType.DragAndDropCopy:
                return Avalonia.Input.StandardCursorType.DragCopy;
            case CefCursorType.Alias:
            case CefCursorType.DragAndDropLink:
                return Avalonia.Input.StandardCursorType.DragLink;
            case CefCursorType.None:
                return Avalonia.Input.StandardCursorType.None;
            default:
                return Avalonia.Input.StandardCursorType.Arrow;
            }
        }

        /* Browser lifetime and navigation, from GHCefClient */

        void IGHCefClientTarget.OnBrowserCreated(CefBrowser browser)
        {
            try
            {
                _creating = false;
                if (browser == null)
                    return;

                /* Kept beyond the callback, so it must not be disposed with a tracking scope around the callback */
                CefObjectTracker.Untrack(browser);

                if (_browser != null)
                {
                    GHApp.MaybeWriteGHLog("GHCefWebViewControl: a second browser was created for one control");
                    return;
                }

                _browser = browser;
                _host = browser.GetHost();
                if (_createTimestamp != 0)
                {
                    GHApp.MaybeWriteGHLog(string.Format(CultureInfo.InvariantCulture, "GHCefWebViewControl: browser created in {0:0} ms",
                        TicksToMilliseconds(Stopwatch.GetTimestamp() - _createTimestamp)));
                }

                if (_closeRequested)
                {
                    _closeSent = true;
                    _host.CloseBrowser(true);
                    return;
                }

                _hidden = false;
                UpdateVisibility();
                if (IsFocused)
                    _host.SetFocus(true);
                _host.WasResized();

                if (_pendingUrl != null)
                {
                    string url = _pendingUrl;
                    _pendingUrl = null;
                    Navigate(url);
                }

                if (_pendingScripts.Count > 0)
                {
                    List<string> scripts = new List<string>(_pendingScripts);
                    _pendingScripts.Clear();
                    foreach (string script in scripts)
                        RunScript(script);
                }

                if (_pendingEvals.Count > 0)
                {
                    List<KeyValuePair<int, string>> evals = new List<KeyValuePair<int, string>>(_pendingEvals);
                    _pendingEvals.Clear();
                    foreach (KeyValuePair<int, string> pending in evals)
                    {
                        if (_evals.ContainsKey(pending.Key) && !SendEval(pending.Key, pending.Value))
                            CompleteEval(pending.Key, null);
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnBrowserCreated failed: " + ex.ToString());
            }
        }

        void IGHCefClientTarget.OnBrowserClosed()
        {
            try
            {
                CefBrowserHost host = _host;
                CefBrowser browser = _browser;
                _host = null;
                _browser = null;
                _client = null;
                _creating = false;
                _closeRequested = true;
                _popupShown = false;
                _activeTouches.Clear();
                _pendingUrl = null;
                _pendingScripts.Clear();
                _pendingEvals.Clear();

                host?.Dispose();
                browser?.Dispose();
                GHCefRuntime.Unregister(this);
                CompleteAllEvals();

                AvWriteableBitmap view = _viewBitmap;
                AvWriteableBitmap popup = _popupBitmap;
                _viewBitmap = null;
                _popupBitmap = null;
                view?.Dispose();
                popup?.Dispose();
                InvalidateVisual();
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: browser closed");
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnBrowserClosed failed: " + ex.ToString());
            }
        }

        bool IGHCefClientTarget.OnNavigationStarting(string url)
        {
            try
            {
                Func<string, bool> handler = NavigationStarting;
                return handler != null && handler(url);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnNavigationStarting failed: " + ex.ToString());
                return false;
            }
        }

        void IGHCefClientTarget.OnLoadCompleted(string url, bool success)
        {
            try
            {
                PostEvent(() => NavigationCompleted?.Invoke(url, success));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnLoadCompleted failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnHistoryChanged(bool canGoBack, bool canGoForward)
        {
            try
            {
                CanGoBack = canGoBack;
                CanGoForward = canGoForward;
                PostEvent(() => HistoryChanged?.Invoke(canGoBack, canGoForward));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnHistoryChanged failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnAddressChanged(string url)
        {
            try
            {
                _address = url;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnAddressChanged failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnNewWindowRequested(string url)
        {
            try
            {
                Uri uri;
                if (!Uri.TryCreate(url, UriKind.Absolute, out uri))
                {
                    GHApp.MaybeWriteGHLog("GHCefWebViewControl: new window request with an invalid URL ignored");
                    return;
                }
                PostEvent(() => NewWindowRequested?.Invoke(uri));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnNewWindowRequested failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnBridgeMessage(string text, string frameUrl)
        {
            try
            {
                PostEvent(() => BridgeMessageReceived?.Invoke(text, frameUrl));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnBridgeMessage failed: " + ex.Message);
            }
        }

        void IGHCefClientTarget.OnEvalResult(int id, bool success, string json)
        {
            try
            {
                CompleteEval(id, success ? json : null);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: OnEvalResult failed: " + ex.Message);
            }
        }

        /* Raises an event from the dispatcher, outside CEF's message loop work */
        private static void PostEvent(Action raise)
        {
            AvDispatcher.UIThread.Post(() =>
            {
                try
                {
                    raise();
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("GHCefWebViewControl: event handler failed: " + ex.ToString());
                }
            }, AvDispatcherPriority.Normal);
        }

        /* Scripts */

        private void RunScript(string script)
        {
            try
            {
                using (CefFrame frame = _browser.GetMainFrame())
                {
                    frame?.ExecuteJavaScript(script, Address, 1);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: script execution failed: " + ex.Message);
            }
        }

        /* Sends "gnollhack_eval" (id, script) to the main frame's renderer */
        private bool SendEval(int id, string script)
        {
            try
            {
                using (CefFrame frame = _browser.GetMainFrame())
                {
                    if (frame == null)
                        return false;

                    using (CefProcessMessage message = CefProcessMessage.Create("gnollhack_eval"))
                    {
                        using (CefListValue arguments = message.Arguments)
                        {
                            if (arguments == null)
                                return false;
                            arguments.SetInt(0, id);
                            arguments.SetString(1, script);
                        }
                        frame.SendProcessMessage(CefProcessId.Renderer, message);
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: script evaluation failed: " + ex.Message);
                return false;
            }
        }

        private void CompleteEval(int id, string result)
        {
            PendingEval pending;
            if (!_evals.TryGetValue(id, out pending))
                return;
            _evals.Remove(id);
            pending.Timeout?.Dispose();
            pending.Source.TrySetResult(result);
        }

        private void CompleteAllEvals()
        {
            foreach (int id in _evals.Keys.ToList())
                CompleteEval(id, null);
        }

        private sealed class PendingEval
        {
            public readonly TaskCompletionSource<string> Source;
            public IDisposable Timeout;

            public PendingEval(TaskCompletionSource<string> source)
            {
                Source = source;
            }
        }

        /* HTML pages */

        /* Writes the page as page_<n>.html, keeping the newest few, and returns its file URI. UTF-8 with a byte
           order mark, so that Chromium decodes a page without a charset declaration correctly. */
        private static string WriteHtmlFile(string html, string baseUrl)
        {
            string folder = Path.Combine(GHAvaloniaPaths.DataDirectory, "cef", "html");
            Directory.CreateDirectory(folder);
            int number = Interlocked.Increment(ref s_htmlFileCounter);
            string path = Path.Combine(folder, "page_" + number.ToString(CultureInfo.InvariantCulture) + ".html");
            File.WriteAllText(path, InsertBaseElement(html, baseUrl), new UTF8Encoding(true));
            DeleteOldHtmlFiles(folder);
            return new Uri(path).AbsoluteUri;
        }

        private static void DeleteOldHtmlFiles(string folder)
        {
            try
            {
                List<FileInfo> files = new DirectoryInfo(folder).GetFiles("page_*.html")
                    .OrderByDescending(f => f.LastWriteTimeUtc)
                    .Skip(KeptHtmlFiles)
                    .ToList();
                foreach (FileInfo file in files)
                {
                    try
                    {
                        file.Delete();
                    }
                    catch (Exception ex)
                    {
                        GHApp.MaybeWriteGHLog("GHCefWebViewControl: cannot delete " + file.Name + ": " + ex.Message);
                    }
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: cannot list the HTML pages: " + ex.Message);
            }
        }

        /* Inserts <base href> right after <head> when a base URL is given and the page has no <base>. An absolute
           path becomes a file:///.../ URI. */
        private static string InsertBaseElement(string html, string baseUrl)
        {
            if (string.IsNullOrEmpty(baseUrl) || s_baseTagRegex.IsMatch(html))
                return html;

            string href;
            try
            {
                href = new Uri(baseUrl.TrimEnd('/') + "/").AbsoluteUri;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: invalid base URL: " + ex.Message);
                return html;
            }

            string element = "<base href=\"" + WebUtility.HtmlEncode(href) + "\">";
            Match head = s_headTagRegex.Match(html);
            if (head.Success)
                return html.Insert(head.Index + head.Length, element);

            Match htmlTag = s_htmlTagRegex.Match(html);
            if (htmlTag.Success)
                return html.Insert(htmlTag.Index + htmlTag.Length, "<head>" + element + "</head>");

            return element + html;
        }

        /* Input; coordinates in device-independent units relative to the control. While a <select> popup is
           shown, events over it still go to the view, which CEF maps to the popup. */

        protected override void OnGotFocus(Avalonia.Input.FocusChangedEventArgs e)
        {
            base.OnGotFocus(e);
            try
            {
                _host?.SetFocus(true);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: focus failed: " + ex.Message);
            }
        }

        protected override void OnLostFocus(Avalonia.Input.FocusChangedEventArgs e)
        {
            base.OnLostFocus(e);
            try
            {
                _host?.SetFocus(false);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: focus loss failed: " + ex.Message);
            }
        }

        protected override void OnPointerPressed(Avalonia.Input.PointerPressedEventArgs e)
        {
            base.OnPointerPressed(e);
            try
            {
                Focus();
                CefBrowserHost host = _host;
                if (host == null)
                    return;
                host.SetFocus(true);

                Avalonia.Input.PointerPoint point = e.GetCurrentPoint(this);
                if (e.Pointer.Type == Avalonia.Input.PointerType.Touch)
                {
                    _activeTouches[e.Pointer.Id] = point.Position;
                    host.SendTouchEvent(CreateTouchEvent(e, point, CefTouchEventType.Pressed));
                }
                else
                {
                    CefMouseButtonType button;
                    if (!TryGetMouseButton(point.Properties.PointerUpdateKind, out button))
                        return;
                    _lastClickCount = Math.Max(1, e.ClickCount);
                    host.SendMouseClickEvent(CreateMouseEvent(e, point), button, false, _lastClickCount);
                }

                e.Pointer.Capture(this);
                e.Handled = true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: pointer press failed: " + ex.Message);
            }
        }

        protected override void OnPointerReleased(Avalonia.Input.PointerReleasedEventArgs e)
        {
            base.OnPointerReleased(e);
            try
            {
                CefBrowserHost host = _host;
                if (host == null)
                    return;

                Avalonia.Input.PointerPoint point = e.GetCurrentPoint(this);
                bool releaseCapture;
                if (e.Pointer.Type == Avalonia.Input.PointerType.Touch)
                {
                    if (_activeTouches.Remove(e.Pointer.Id))
                        host.SendTouchEvent(CreateTouchEvent(e, point, CefTouchEventType.Released));
                    releaseCapture = true;
                }
                else
                {
                    CefMouseButtonType button;
                    if (TryGetMouseButton(point.Properties.PointerUpdateKind, out button))
                        host.SendMouseClickEvent(CreateMouseEvent(e, point), button, true, _lastClickCount);
                    Avalonia.Input.PointerPointProperties properties = point.Properties;
                    releaseCapture = !properties.IsLeftButtonPressed && !properties.IsMiddleButtonPressed && !properties.IsRightButtonPressed;
                }

                if (releaseCapture && e.Pointer.Captured == this)
                    e.Pointer.Capture(null);
                e.Handled = true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: pointer release failed: " + ex.Message);
            }
        }

        protected override void OnPointerMoved(Avalonia.Input.PointerEventArgs e)
        {
            base.OnPointerMoved(e);
            try
            {
                CefBrowserHost host = _host;
                if (host == null)
                    return;

                Avalonia.Input.PointerPoint point = e.GetCurrentPoint(this);
                if (e.Pointer.Type == Avalonia.Input.PointerType.Touch)
                {
                    if (_activeTouches.ContainsKey(e.Pointer.Id))
                    {
                        _activeTouches[e.Pointer.Id] = point.Position;
                        host.SendTouchEvent(CreateTouchEvent(e, point, CefTouchEventType.Moved));
                        e.Handled = true;
                    }
                }
                else
                {
                    host.SendMouseMoveEvent(CreateMouseEvent(e, point), false);
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: pointer move failed: " + ex.Message);
            }
        }

        protected override void OnPointerExited(Avalonia.Input.PointerEventArgs e)
        {
            base.OnPointerExited(e);
            try
            {
                CefBrowserHost host = _host;
                if (host == null || e.Pointer.Type == Avalonia.Input.PointerType.Touch)
                    return;
                host.SendMouseMoveEvent(CreateMouseEvent(e, e.GetCurrentPoint(this)), true);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: pointer exit failed: " + ex.Message);
            }
        }

        protected override void OnPointerCaptureLost(Avalonia.Input.PointerCaptureLostEventArgs e)
        {
            base.OnPointerCaptureLost(e);
            try
            {
                Avalonia.Point position;
                if (e.Pointer.Type != Avalonia.Input.PointerType.Touch || !_activeTouches.TryGetValue(e.Pointer.Id, out position))
                    return;
                _activeTouches.Remove(e.Pointer.Id);

                CefBrowserHost host = _host;
                if (host == null)
                    return;
                host.SendTouchEvent(new CefTouchEvent
                {
                    Id = e.Pointer.Id,
                    X = (float)position.X,
                    Y = (float)position.Y,
                    Type = CefTouchEventType.Cancelled,
                    Modifiers = CefEventFlags.None,
                    PointerType = CefPointerType.Touch,
                });
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: pointer capture loss failed: " + ex.Message);
            }
        }

        protected override void OnPointerWheelChanged(Avalonia.Input.PointerWheelEventArgs e)
        {
            base.OnPointerWheelChanged(e);
            try
            {
                CefBrowserHost host = _host;
                if (host == null)
                    return;
                host.SendMouseWheelEvent(CreateMouseEvent(e, e.GetCurrentPoint(this)),
                    (int)(e.Delta.X * WheelDeltaPerUnit), (int)(e.Delta.Y * WheelDeltaPerUnit));
                e.Handled = true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: wheel failed: " + ex.Message);
            }
        }

        // WORKAROUND(WA-CEF-0003): Chromium's X11 clipboard blocks off-screen pages
        /* RAWKEYDOWN for every key, plus CHAR '\r' for Enter; text comes from TextInput, which Avalonia raises
           only when the key is not handled, so only navigation keys are marked handled. Clipboard keys are handled
           here through Avalonia's clipboard: Chromium's own X11 clipboard blocks the page on paste and does not reach
           the system clipboard on copy. */
        protected override void OnKeyDown(Avalonia.Input.KeyEventArgs e)
        {
            base.OnKeyDown(e);
            try
            {
                CefBrowserHost host = _host;
                if (host == null)
                    return;

                if (IsPasteKey(e))
                {
                    e.Handled = true;
                    _ = PasteClipboardTextAsync();
                    return;
                }
                bool cut = IsCutKey(e);
                if (cut || IsCopyKey(e))
                {
                    e.Handled = true;
                    _ = CopySelectionAsync(cut);
                    return;
                }

                CefEventFlags flags = GHCefInput.Modifiers(e.KeyModifiers) | GHCefInput.KeyLocationFlags(e.PhysicalKey);
                int nativeKeyCode = GHCefInput.X11KeyCode(e.PhysicalKey);
                host.SendKeyEvent(new CefKeyEvent
                {
                    EventType = CefKeyEventType.RawKeyDown,
                    WindowsKeyCode = GHCefInput.WindowsKeyCode(e.Key),
                    NativeKeyCode = nativeKeyCode,
                    Modifiers = flags,
                });

                if (e.Key == Avalonia.Input.Key.Enter)
                {
                    host.SendKeyEvent(new CefKeyEvent
                    {
                        EventType = CefKeyEventType.Char,
                        WindowsKeyCode = '\r',
                        NativeKeyCode = nativeKeyCode,
                        Character = '\r',
                        UnmodifiedCharacter = '\r',
                        Modifiers = flags,
                    });
                }

                if (IsNavigationKey(e.Key))
                    e.Handled = true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: key down failed: " + ex.Message);
            }
        }

        protected override void OnKeyUp(Avalonia.Input.KeyEventArgs e)
        {
            base.OnKeyUp(e);
            try
            {
                CefBrowserHost host = _host;
                if (host == null || IsPasteKey(e) || IsCopyKey(e) || IsCutKey(e))
                    return;

                host.SendKeyEvent(new CefKeyEvent
                {
                    EventType = CefKeyEventType.KeyUp,
                    WindowsKeyCode = GHCefInput.WindowsKeyCode(e.Key),
                    NativeKeyCode = GHCefInput.X11KeyCode(e.PhysicalKey),
                    Modifiers = GHCefInput.Modifiers(e.KeyModifiers) | GHCefInput.KeyLocationFlags(e.PhysicalKey),
                });
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: key up failed: " + ex.Message);
            }
        }

        /* Ctrl+V, Ctrl+Shift+V and Shift+Insert */
        private static bool IsPasteKey(Avalonia.Input.KeyEventArgs e)
        {
            Avalonia.Input.KeyModifiers modifiers = e.KeyModifiers;
            if ((modifiers & Avalonia.Input.KeyModifiers.Alt) != 0)
                return false;
            if (e.Key == Avalonia.Input.Key.V)
                return (modifiers & Avalonia.Input.KeyModifiers.Control) != 0;
            return e.Key == Avalonia.Input.Key.Insert && modifiers == Avalonia.Input.KeyModifiers.Shift;
        }

        /* Ctrl+C and Ctrl+Insert */
        private static bool IsCopyKey(Avalonia.Input.KeyEventArgs e)
        {
            Avalonia.Input.KeyModifiers modifiers = e.KeyModifiers;
            if ((modifiers & (Avalonia.Input.KeyModifiers.Alt | Avalonia.Input.KeyModifiers.Shift)) != 0)
                return false;
            return (modifiers & Avalonia.Input.KeyModifiers.Control) != 0 && (e.Key == Avalonia.Input.Key.C || e.Key == Avalonia.Input.Key.Insert);
        }

        /* Ctrl+X and Shift+Delete */
        private static bool IsCutKey(Avalonia.Input.KeyEventArgs e)
        {
            Avalonia.Input.KeyModifiers modifiers = e.KeyModifiers;
            if (e.Key == Avalonia.Input.Key.X)
                return modifiers == Avalonia.Input.KeyModifiers.Control;
            return e.Key == Avalonia.Input.Key.Delete && modifiers == Avalonia.Input.KeyModifiers.Shift;
        }

        // WORKAROUND(WA-CEF-0003): Chromium's X11 clipboard blocks off-screen pages
        /* The selection CEF reported, onto the clipboard through Avalonia's X11 connection. A cut then deletes the
           selection with a Delete key, which does nothing outside editable fields. */
        private async Task CopySelectionAsync(bool cut)
        {
            try
            {
                string text = _selectedText;
                if (string.IsNullOrEmpty(text))
                    return;

                Avalonia.Input.Platform.IClipboard clipboard = Avalonia.Controls.TopLevel.GetTopLevel(this)?.Clipboard;
                if (clipboard == null)
                    return;

                await Avalonia.Input.Platform.ClipboardExtensions.SetTextAsync(clipboard, text);

                CefBrowserHost host = _host;
                if (cut && host != null && !_closeRequested)
                {
                    int deleteKey = GHCefInput.WindowsKeyCode(Avalonia.Input.Key.Delete);
                    host.SendKeyEvent(new CefKeyEvent { EventType = CefKeyEventType.RawKeyDown, WindowsKeyCode = deleteKey });
                    host.SendKeyEvent(new CefKeyEvent { EventType = CefKeyEventType.KeyUp, WindowsKeyCode = deleteKey });
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: copy failed: " + ex.Message);
            }
        }

        // WORKAROUND(WA-CEF-0003): Chromium's X11 clipboard blocks off-screen pages
        /* The clipboard text through Avalonia's own X11 connection, inserted at the caret of the focused field */
        private async Task PasteClipboardTextAsync()
        {
            try
            {
                Avalonia.Input.Platform.IClipboard clipboard = Avalonia.Controls.TopLevel.GetTopLevel(this)?.Clipboard;
                if (clipboard == null)
                    return;

                string text = await Avalonia.Input.Platform.ClipboardExtensions.TryGetTextAsync(clipboard);
                CefBrowserHost host = _host;
                if (string.IsNullOrEmpty(text) || host == null || _closeRequested)
                    return;

                /* An invalid range (UINT32_MAX in CEF) inserts at the caret */
                host.ImeCommitText(text, new CefRange(-1, -1), 0);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: paste failed: " + ex.Message);
            }
        }

        /* One CHAR event per UTF-16 unit */
        protected override void OnTextInput(Avalonia.Input.TextInputEventArgs e)
        {
            base.OnTextInput(e);
            try
            {
                CefBrowserHost host = _host;
                string text = e.Text;
                if (host == null || string.IsNullOrEmpty(text))
                    return;

                int i;
                for (i = 0; i < text.Length; i++)
                {
                    char c = text[i];
                    if (c < ' ' || c == '\x7F')
                        continue;
                    host.SendKeyEvent(new CefKeyEvent
                    {
                        EventType = CefKeyEventType.Char,
                        WindowsKeyCode = c,
                        Character = c,
                        UnmodifiedCharacter = c,
                    });
                }
                e.Handled = true;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHCefWebViewControl: text input failed: " + ex.Message);
            }
        }

        private static bool IsNavigationKey(Avalonia.Input.Key key)
        {
            switch (key)
            {
            case Avalonia.Input.Key.Tab:
            case Avalonia.Input.Key.Left:
            case Avalonia.Input.Key.Up:
            case Avalonia.Input.Key.Right:
            case Avalonia.Input.Key.Down:
            case Avalonia.Input.Key.Home:
            case Avalonia.Input.Key.End:
            case Avalonia.Input.Key.PageUp:
            case Avalonia.Input.Key.PageDown:
                return true;
            default:
                return false;
            }
        }

        private static bool TryGetMouseButton(Avalonia.Input.PointerUpdateKind kind, out CefMouseButtonType button)
        {
            switch (kind)
            {
            case Avalonia.Input.PointerUpdateKind.LeftButtonPressed:
            case Avalonia.Input.PointerUpdateKind.LeftButtonReleased:
                button = CefMouseButtonType.Left;
                return true;
            case Avalonia.Input.PointerUpdateKind.MiddleButtonPressed:
            case Avalonia.Input.PointerUpdateKind.MiddleButtonReleased:
                button = CefMouseButtonType.Middle;
                return true;
            case Avalonia.Input.PointerUpdateKind.RightButtonPressed:
            case Avalonia.Input.PointerUpdateKind.RightButtonReleased:
                button = CefMouseButtonType.Right;
                return true;
            default:
                button = CefMouseButtonType.Left;
                return false;
            }
        }

        private static CefMouseEvent CreateMouseEvent(Avalonia.Input.PointerEventArgs e, Avalonia.Input.PointerPoint point)
        {
            return new CefMouseEvent((int)Math.Floor(point.Position.X), (int)Math.Floor(point.Position.Y),
                GHCefInput.Modifiers(e.KeyModifiers, point.Properties));
        }

        private static CefTouchEvent CreateTouchEvent(Avalonia.Input.PointerEventArgs e, Avalonia.Input.PointerPoint point, CefTouchEventType type)
        {
            Avalonia.Rect contact = point.Properties.ContactRect;
            return new CefTouchEvent
            {
                Id = e.Pointer.Id,
                X = (float)point.Position.X,
                Y = (float)point.Position.Y,
                RadiusX = (float)(contact.Width / 2),
                RadiusY = (float)(contact.Height / 2),
                Pressure = point.Properties.Pressure,
                Type = type,
                Modifiers = GHCefInput.Modifiers(e.KeyModifiers),
                PointerType = CefPointerType.Touch,
            };
        }

        private static double TicksToMilliseconds(long ticks)
        {
            return ticks * 1000.0 / Stopwatch.Frequency;
        }
    }
}
#endif

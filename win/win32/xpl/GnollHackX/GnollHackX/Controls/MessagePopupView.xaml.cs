using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using GnollHackX;


#if GNH_MAUI
using Microsoft.Maui;
using Microsoft.Maui.Controls;
using Microsoft.Maui.Controls.Internals;
using Microsoft.Maui.Graphics;
using Microsoft.Maui.ApplicationModel;
#if IOS
using CoreGraphics;
using Foundation;
using UIKit;
#endif

namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX.Controls
#endif
{
    public partial class MessagePopupView : ContentView
    {
        private TaskCompletionSource<bool> _messagePopupTcs;
        private bool _acceptEnterSpaceForOkCancel = false;
        private CancellationTokenSource _popupTimeoutCts;

        public MessagePopupView()
        {
            InitializeComponent();
#if GNH_MAUI
            Loaded += OnLoaded;
            Unloaded += OnUnloaded;
#endif
            IsVisible = false;
            if (GHApp.DarkMode)
            {
                MessagePopupFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
            }
        }

        public bool IsPopupOpen => IsVisible;
        public bool IsPopupOkCancel => MessagePopupCancelButton.IsVisible;

        private Page _parentPage = null;

#if GNH_MAUI
        private void OnLoaded(object sender, EventArgs e)
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.OnLoaded");
            try
            {
                if (_parentPage == null)
                {
                    _parentPage = GetParentPage(this);
                }
            }
            catch { }
        }

        private void OnUnloaded(object sender, EventArgs e)
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.OnUnloaded");
            try
            {
                if (_parentPage != null)
                {
                    //CleanPopup();
                    _parentPage = null;
                }
            }
            catch { }
        }
#else
        protected override void OnParentSet()
        {
            base.OnParentSet();

            if (Parent != null)
            {
                GHApp.MaybeWriteGHLog("MessagePopupView.OnParentSet: True");
                _parentPage = GetParentPage(this);
                if (_parentPage != null)
                    _parentPage.Disappearing += OnPageDisappearing;
            }
            else
            {
                GHApp.MaybeWriteGHLog("MessagePopupView.OnParentSet: False");
                if (_parentPage != null)
                {
                    CleanPopup();
                    _parentPage.Disappearing -= OnPageDisappearing;
                    _parentPage = null;
                }
            }
        }

        private void OnPageDisappearing(object sender, EventArgs e)
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.OnPageDisappearing");
            CleanPopup();
        }
#endif

        private Page GetParentPage(Element element)
        {
            Element current = element;

            while (current != null)
            {
                if (current is Page page)
                    return page;

                current = current.Parent;
            }

            return null;
        }

        private void DismissPopup()
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.DismissPopup");
            CancelTimeout();
            IsVisible = false;
            _messagePopupTcs?.TrySetResult(false);
        }

        private void AcceptPopup()
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.AcceptPopup");
            CancelTimeout();
            IsVisible = false;
            _messagePopupTcs?.TrySetResult(true);
        }

        private void CleanPopup()
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.CleanPopup");
            CancelTimeout();
            try
            {
                _messagePopupTcs?.TrySetResult(false);
            }
            catch { }
            finally
            {
                _messagePopupTcs = null;
            }
        }

        private void CancelTimeout()
        {
            _popupTimeoutCts?.Dispose();
            _popupTimeoutCts = null;
        }

        private void StartTimeout()
        {
            CancelTimeout();
            _popupTimeoutCts = new CancellationTokenSource();
            _popupTimeoutCts.CancelAfter(TimeSpan.FromMinutes(5));
            _popupTimeoutCts.Token.Register(() =>
            {
#if GNH_MAUI
                MainThread.BeginInvokeOnMainThread(() => DismissPopup());
#else
                Device.BeginInvokeOnMainThread(() => DismissPopup());
#endif
            });
        }

        /// <summary>
        /// Closes the popup, accepting it if only OK is shown, or cancelling if Cancel is visible.
        /// Useful for programmatic page-close scenarios.
        /// </summary>
        public void ClosePopup()
        {
            if (IsPopupOpen)
            {
                if (IsPopupOkCancel)
                    DismissPopup();
                else
                    AcceptPopup();
            }
        }

        private Thickness _savedFramePadding;

        /// <summary>
        /// Shows a non-blocking informational popup with no buttons (e.g. a spinner/wait message).
        /// Call <see cref="HideNonBlockingPopup"/> to hide it when the operation is complete.
        /// </summary>
        public void ShowNonBlockingPopup(string title, string message,
#if GNH_MAUI
            Color titleColor = null)
#else
            Color? titleColor = null)
#endif
        {
            GHApp.MaybeWriteGHLog("MessagePopupView.ShowNonBlockingPopup");
            _messagePopupTcs?.TrySetResult(false);
            _messagePopupTcs = null;
            CancelTimeout();

            MessagePopupTitleLabel.Text = title;
            MessagePopupTitleLabel.TextColor = titleColor ?? GHColors.TitleGoldColor;
            MessagePopupLabel.Text = message;
            MessagePopupOkButton.IsVisible = false;
            MessagePopupCancelButton.IsVisible = false;
            _savedFramePadding = MessagePopupFrame.Padding;
            MessagePopupFrame.Padding = new Thickness(12, 18, 12, 30);
            IsVisible = true;
        }

        /// <summary>
        /// Hides a non-blocking popup previously shown with <see cref="ShowNonBlockingPopup"/>.
        /// Restores button visibility and frame padding.
        /// </summary>
        public void HideNonBlockingPopup()
        {
            MessagePopupOkButton.IsVisible = true;
            MessagePopupFrame.Padding = _savedFramePadding;
            IsVisible = false;
        }

        private Span _linkSpan = null;
#if GNH_MAUI
        private PointerGestureRecognizer _labelPointerGesture = null;
#endif

        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Color titleColor = null,
#else
            Color? titleColor = null,
#endif
            bool acceptEnterSpaceForOkCancel = false)
        {
            if (!MessagePopupOkButton.IsVisible)
                HideNonBlockingPopup();
            GHApp.MaybeWriteGHLog("MessagePopupView.ShowMessagePopupAsync");
            _acceptEnterSpaceForOkCancel = acceptEnterSpaceForOkCancel;
            _messagePopupTcs?.TrySetResult(false);
            _messagePopupTcs = new TaskCompletionSource<bool>();
            StartTimeout();

            MessagePopupTitleLabel.Text = title;
            MessagePopupTitleLabel.TextColor = titleColor ?? GHColors.TitleGoldColor;
            MessagePopupLabel.FormattedText = null;
            MessagePopupLabel.Text = message;
            DetachLabelPointerGesture();

            if (string.IsNullOrEmpty(cancelButtonText))
            {
                MessagePopupOkButton.Text = okButtonText;
                MessagePopupCancelButton.IsVisible = false;
                MessagePopupOkButton.HorizontalOptions = LayoutOptions.Center;
            }
            else
            {
                MessagePopupOkButton.Text = okButtonText;
                MessagePopupCancelButton.Text = cancelButtonText;
                MessagePopupCancelButton.IsVisible = true;
                MessagePopupOkButton.HorizontalOptions = LayoutOptions.End;
                MessagePopupCancelButton.HorizontalOptions = LayoutOptions.Start;
            }

            IsVisible = true;
            MessagePopupScrollView.ScrollToAsync(0, 0, false);
            return _messagePopupTcs.Task;

        }

        public Task<bool> ShowMessagePopupAsync(string title, FormattedString message, Span linkSpan, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Color titleColor = null,
#else
            Color? titleColor = null,
#endif
            bool acceptEnterSpaceForOkCancel = false)
        {
            if (!MessagePopupOkButton.IsVisible)
                HideNonBlockingPopup();
            _acceptEnterSpaceForOkCancel = acceptEnterSpaceForOkCancel;
            GHApp.MaybeWriteGHLog("MessagePopupView.ShowMessagePopupAsync (FormattedString)");
            _messagePopupTcs?.TrySetResult(false);
            _messagePopupTcs = new TaskCompletionSource<bool>();
            StartTimeout();

            MessagePopupTitleLabel.Text = title;
            MessagePopupTitleLabel.TextColor = titleColor ?? GHColors.TitleGoldColor;
            MessagePopupLabel.Text = null;
            MessagePopupLabel.FormattedText = message;
            _linkSpan = linkSpan;
            if (_linkSpan != null)
                AttachLabelPointerGesture();
            else
                DetachLabelPointerGesture();
            ScheduleSpanPositionRecalculation(message);

            if (string.IsNullOrEmpty(cancelButtonText))
            {
                MessagePopupOkButton.Text = okButtonText;
                MessagePopupCancelButton.IsVisible = false;
                MessagePopupOkButton.HorizontalOptions = LayoutOptions.Center;
            }
            else
            {
                MessagePopupOkButton.Text = okButtonText;
                MessagePopupCancelButton.Text = cancelButtonText;
                MessagePopupCancelButton.IsVisible = true;
                MessagePopupOkButton.HorizontalOptions = LayoutOptions.End;
                MessagePopupCancelButton.HorizontalOptions = LayoutOptions.Start;
            }

            IsVisible = true;
            MessagePopupScrollView.ScrollToAsync(0, 0, false);
            return _messagePopupTcs.Task;

        }

        private void AttachLabelPointerGesture()
        {
#if GNH_MAUI && WINDOWS
            if (_labelPointerGesture != null)
                return;

            _labelPointerGesture = new PointerGestureRecognizer();
            _labelPointerGesture.PointerMoved += OnLabelPointerMoved;
            _labelPointerGesture.PointerExited += OnLabelPointerExited;
            MessagePopupLabel.GestureRecognizers.Add(_labelPointerGesture);
#endif
        }

        private void DetachLabelPointerGesture()
        {
#if GNH_MAUI && WINDOWS
            _linkSpan = null;
            if (_labelPointerGesture != null)
            {
                _labelPointerGesture.PointerMoved -= OnLabelPointerMoved;
                _labelPointerGesture.PointerExited -= OnLabelPointerExited;
                MessagePopupLabel.GestureRecognizers.Remove(_labelPointerGesture);
                _labelPointerGesture = null;
            }
            UIUtils.ChangeElementCursor(MessagePopupLabel, GameCursorType.Normal);
#endif
        }

#if GNH_MAUI && WINDOWS
        private bool _isOverLink = false;

        private void OnLabelPointerMoved(object sender, PointerEventArgs e)
        {
            if (_linkSpan == null)
                return;

            var pos = e.GetPosition(MessagePopupLabel);
            if (pos == null)
                return;

            var spatial = _linkSpan as Microsoft.Maui.Controls.Internals.ISpatialElement;
            bool overLink = spatial?.Region.Contains(pos.Value) ?? false;

            if (overLink != _isOverLink)
            {
                _isOverLink = overLink;
                UIUtils.ChangeElementCursor(MessagePopupLabel, overLink ? GameCursorType.Info : GameCursorType.Normal);
            }
        }

        private void OnLabelPointerExited(object sender, PointerEventArgs e)
        {
            if (_isOverLink)
            {
                _isOverLink = false;
                UIUtils.ChangeElementCursor(MessagePopupLabel, GameCursorType.Normal);
            }
        }
#endif

        private void ScheduleSpanPositionRecalculation(FormattedString formatted)
        {
#if GNH_MAUI && IOS
            /* Unsubscribe any previous handler to avoid stacking */
            MessagePopupLabel.SizeChanged -= OnLabelSizeChangedForSpanRecalc;
            _pendingFormattedString = formatted;
            MessagePopupLabel.SizeChanged += OnLabelSizeChangedForSpanRecalc;
#endif
        }

#if GNH_MAUI && IOS
        private FormattedString _pendingFormattedString;

        private void OnLabelSizeChangedForSpanRecalc(object sender, EventArgs e)
        {
            MessagePopupLabel.SizeChanged -= OnLabelSizeChangedForSpanRecalc;

            if (_pendingFormattedString == null)
                return;

            GHApp.MaybeWriteGHLog("SpanRecalc: SizeChanged fired. MAUI Label W=" + MessagePopupLabel.Width + " H=" + MessagePopupLabel.Height);

            /* Defer to next run loop iteration so UILabel bounds are finalized */
            Dispatcher.Dispatch(() =>
            {
                if (_pendingFormattedString == null)
                    return;

                if (MessagePopupLabel.Handler?.PlatformView is UILabel nativeLabel)
                {
                    GHApp.MaybeWriteGHLog("SpanRecalc (deferred): UILabel Bounds=" + nativeLabel.Bounds + " Frame=" + nativeLabel.Frame);
                    GHApp.MaybeWriteGHLog("SpanRecalc (deferred): MAUI Label W=" + MessagePopupLabel.Width + " H=" + MessagePopupLabel.Height);

                    /* Prefer UILabel bounds; fall back to MAUI dimensions if still zero */
                    Size containerSize;
                    if (nativeLabel.Bounds.Width > 0 && nativeLabel.Bounds.Height > 0)
                        containerSize = new Size(nativeLabel.Bounds.Width, nativeLabel.Bounds.Height);
                    else
                        containerSize = new Size(MessagePopupLabel.Width, MessagePopupLabel.Height);

                    RecalculateSpanPositionsFromNative(nativeLabel, _pendingFormattedString, containerSize);
                }
                else
                {
                    GHApp.MaybeWriteGHLog("SpanRecalc (deferred): Handler or PlatformView is null");
                }
                _pendingFormattedString = null;
            });
        }

        /// <summary>
        /// Recomputes ISpatialElement.Region for each span using the native UILabel's
        /// actual bounds, fixing the MAUI bug where ArrangeOverride runs before
        /// UILabel.Bounds are finalized.
        /// </summary>
        private static void RecalculateSpanPositionsFromNative(UILabel control, FormattedString formatted, Size mauiSize)
        {
            if (formatted?.Spans == null || formatted.Spans.Count == 0)
                return;

            var attributedText = control.AttributedText;
            if (attributedText == null || attributedText.Length == 0)
                return;

            var spans = formatted.Spans;

            nint NSMaxRange(NSRange range) => range.Location + range.Length;

            using var textStorage = new NSTextStorage();
            using var layoutManager = new NSLayoutManager();
            using var textContainer = new NSTextContainer { LineFragmentPadding = 0 };

            textStorage.AddLayoutManager(layoutManager);
            layoutManager.AddTextContainer(textContainer);

            /* UILabel.Bounds may still be zero at SizeChanged time, so use MAUI dimensions */
            var containerWidth = (nfloat)mauiSize.Width;
            var containerHeight = (nfloat)mauiSize.Height;
            if (containerWidth <= 0 || containerHeight <= 0)
            {
                GHApp.MaybeWriteGHLog("SpanRecalc: Container size invalid: W=" + containerWidth + " H=" + containerHeight);
                return;
            }

            textContainer.Size = new(containerWidth, control.Lines == 0 ? nfloat.MaxValue : containerHeight);
            GHApp.MaybeWriteGHLog("SpanRecalc: TextContainer W=" + containerWidth + " H=" + (control.Lines == 0 ? "MaxValue" : containerHeight.ToString()) + " Lines=" + control.Lines);

            textStorage.SetString(attributedText);
            layoutManager.EnsureLayoutForTextContainer(textContainer);

            /* Diagnostic: compare our layout height with UILabel's actual text height */
            var usedRect = layoutManager.GetUsedRect(textContainer);
            GHApp.MaybeWriteGHLog("SpanRecalc: NSLayoutManager usedRect=" + usedRect);

            var boundingOptions = NSStringDrawingOptions.UsesLineFragmentOrigin | NSStringDrawingOptions.UsesFontLeading;
            var boundingRect = attributedText.GetBoundingRect(
                new CGSize(containerWidth, nfloat.MaxValue), boundingOptions, null);
            GHApp.MaybeWriteGHLog("SpanRecalc: BoundingRectWithSize=" + boundingRect);

            var sizeThatFits = control.SizeThatFits(new CGSize(containerWidth, nfloat.MaxValue));
            GHApp.MaybeWriteGHLog("SpanRecalc: UILabel.SizeThatFits=" + sizeThatFits);

            /* UILabel (TextKit 2) vs NSLayoutManager (TextKit 1) may differ in height.
             * Scale Y coordinates to match the actual UILabel rendering. */
            var usedHeight = usedRect.Height;
            var actualHeight = sizeThatFits.Height;
            var yScale = (usedHeight > 0 && actualHeight > 0 && Math.Abs(usedHeight - actualHeight) > 1)
                ? actualHeight / usedHeight
                : 1.0;
            GHApp.MaybeWriteGHLog("SpanRecalc: yScale=" + ((double)yScale).ToString("F4"));

            int currentLocation = 0;
            int spanIndex = 0;

            foreach (var span in spans)
            {
                if (string.IsNullOrEmpty(span?.Text))
                    continue;

                var spanRects = new List<CGRect>();
                var spanStartIndex = currentLocation;
                var spanEndIndex = currentLocation + span.Text.Length - 1;

                var startGlyphRange = layoutManager.GetGlyphRange(new NSRange(spanStartIndex, 1));
                var endGlyphRange = layoutManager.GetGlyphRange(new NSRange(spanEndIndex, 1));

                GHApp.MaybeWriteGHLog("SpanRecalc: Span[" + spanIndex + "] text='" + (span.Text.Length > 30 ? span.Text.Substring(0, 30) + "..." : span.Text)
                    + "' charRange=[" + spanStartIndex + ".." + spanEndIndex + "]"
                    + " startGlyph=" + startGlyphRange.Location + " endGlyph=" + endGlyphRange.Location);

                void EnumerateCallback(CGRect rect, CGRect usedRect, NSTextContainer container,
                    NSRange lineGlyphRange, out bool stop)
                {
                    /* Whole span is within the line and bigger than the line */
                    if (lineGlyphRange.Location >= startGlyphRange.Location &&
                        NSMaxRange(lineGlyphRange) <= endGlyphRange.Location)
                    {
                        spanRects.Add(usedRect);
                    }
                    /* Whole span is within the line and smaller than the line */
                    else if (lineGlyphRange.Location <= startGlyphRange.Location &&
                             endGlyphRange.Location <= NSMaxRange(lineGlyphRange))
                    {
                        var spanBoundingRect = layoutManager.GetBoundingRect(
                            new(startGlyphRange.Location, endGlyphRange.Location - startGlyphRange.Location + 1),
                            textContainer);
                        spanRects.Add(spanBoundingRect);
                    }
                    /* Span starts on current line and ends on next lines */
                    else if (lineGlyphRange.Location <= startGlyphRange.Location &&
                             NSMaxRange(lineGlyphRange) <= endGlyphRange.Location)
                    {
                        var spanBoundingRect = layoutManager.GetBoundingRect(
                            new(startGlyphRange.Location, NSMaxRange(lineGlyphRange) - startGlyphRange.Location),
                            textContainer);
                        spanRects.Add(spanBoundingRect);
                    }
                    /* Span starts on previous lines and ends on current line */
                    else if (lineGlyphRange.Location >= startGlyphRange.Location &&
                             NSMaxRange(lineGlyphRange) >= endGlyphRange.Location)
                    {
                        var spanBoundingRect = layoutManager.GetBoundingRect(
                            new(lineGlyphRange.Location, endGlyphRange.Location - lineGlyphRange.Location),
                            textContainer);
                        spanRects.Add(spanBoundingRect);
                    }

                    stop = false;
                }

                layoutManager.EnumerateLineFragments(
                    new(startGlyphRange.Location, endGlyphRange.Location - startGlyphRange.Location + 1),
                    EnumerateCallback);

                if (span is ISpatialElement spatialElement)
                {
                    var rects = new List<Rect>();
                    foreach (var r in spanRects)
                    {
                        var scaledY = r.Y * yScale;
                        var scaledH = r.Height * yScale;
                        rects.Add(new Rect(r.X, scaledY, r.Width, scaledH));
                        GHApp.MaybeWriteGHLog("SpanRecalc: Span[" + spanIndex + "] rect: X=" + r.X.ToString("F1") + " Y=" + scaledY.ToString("F1")
                            + " W=" + r.Width.ToString("F1") + " H=" + scaledH.ToString("F1")
                            + " (raw Y=" + r.Y.ToString("F1") + ")");
                    }
                    spatialElement.Region = Region.FromRectangles(rects);
                    GHApp.MaybeWriteGHLog("SpanRecalc: Span[" + spanIndex + "] set " + rects.Count + " region rects");
                }
                else
                {
                    GHApp.MaybeWriteGHLog("SpanRecalc: Span[" + spanIndex + "] is NOT ISpatialElement");
                }
                currentLocation += span.Text.Length;
                spanIndex++;
            }
        }
#endif

        private void MessagePopupOkButton_Clicked(object sender, EventArgs e)
        {
            AcceptPopup();
        }

        private void MessagePopupCancelButton_Clicked(object sender, EventArgs e)
        {
            DismissPopup();
        }

        public bool SendKeyToPopup(int key, bool isCtrl, bool isMeta)
        {
            if (IsPopupOpen)
            {
                if (key == ' ' || key == '\r' || key == '\n')
                {
                    if (!MessagePopupCancelButton.IsVisible || _acceptEnterSpaceForOkCancel)
                    {
                        AcceptPopup();
                    }
                }
                else
                {
                    char c = (char)key;
                    string okText = MessagePopupOkButton.Text;
                    if (!string.IsNullOrEmpty(okText) && char.ToLowerInvariant(okText[0]) == char.ToLowerInvariant(c))
                    {
                        AcceptPopup();
                    }
                    else if (MessagePopupCancelButton.IsVisible)
                    {
                        string cancelText = MessagePopupCancelButton.Text;
                        if (!string.IsNullOrEmpty(cancelText) && char.ToLowerInvariant(cancelText[0]) == char.ToLowerInvariant(c))
                        {
                            DismissPopup();
                        }
                    }
                }
                return true;
            }
            return false;
        }

        public bool SendSpecialKeyToPopup(GHSpecialKey spkey, bool isCtrl, bool isMeta, bool isShift)
        {
            if (IsPopupOpen)
            {
                if (spkey == GHSpecialKey.Enter || spkey == GHSpecialKey.Space)
                {
                    if (!MessagePopupCancelButton.IsVisible || _acceptEnterSpaceForOkCancel)
                    {
                        AcceptPopup();
                    }
                }
                else if (spkey == GHSpecialKey.Escape)
                {
                    if (MessagePopupCancelButton.IsVisible)
                        DismissPopup();
                    else
                        AcceptPopup();
                }
                return true;
            }
            return false;
        }
    }
}

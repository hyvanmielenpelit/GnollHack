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

namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX.Controls
#endif
{
    public partial class MessagePopupView : ContentView
    {
        /* A link span in the message and what activating it does, by tap or gamepad */
        public sealed class PopupLink
        {
            public Span Span;
            public Func<Task> Open;
        }

        private TaskCompletionSource<bool> _messagePopupTcs;
        private bool _acceptEnterSpaceForOkCancel = false;
        private CancellationTokenSource _popupTimeoutCts;

        /* Gamepad focus over the message's links and [OK, Cancel]; A activates only a press that began while this
           popup was open */
        private readonly GHGamepadFocusList _gamepadFocus = new GHGamepadFocusList();
        private bool _gamepadAArmed = false;
        private int _gamepadAPressId = 0;
        private bool _gamepadSubscribed = false;

        /* The focused link in _links, or -1 while [OK, Cancel] have the focus; _gamepadFocus keeps its index */
        private int _linkFocus = -1;
        private bool _focusFirstLink = false;
        /* A link's Open is running; A does nothing meanwhile */
        private bool _openingLink = false;

        /* The link span tinted as focused, and its own colors to restore */
        private Span _highlightedSpan = null;
        private bool _spanTextColorSet = false;
        private object _spanTextColor = null;
        private bool _spanBackgroundSet = false;
        private object _spanBackground = null;

        public MessagePopupView()
        {
            InitializeComponent();
            _gamepadFocus.Add(MessagePopupOkButton, AcceptPopup, () => MessagePopupOkButton.IsVisible && MessagePopupOkButton.IsEnabled);
            _gamepadFocus.Add(MessagePopupCancelButton, DismissPopup, () => MessagePopupCancelButton.IsVisible && MessagePopupCancelButton.IsEnabled);
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
            UpdateGamepadSubscription(false);
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
                UpdateGamepadSubscription(false);
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
            RestoreSpanHighlight();
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
            _linkFocus = -1;
            RestoreSpanHighlight();

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

        private List<PopupLink> _links = null;
#if GNH_MAUI && WINDOWS
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
            _focusFirstLink = false;

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

            ResetGamepadFocus();
            IsVisible = true;
            MessagePopupScrollView.ScrollToAsync(0, 0, false);
            return _messagePopupTcs.Task;

        }

        public Task<bool> ShowMessagePopupAsync(string title, FormattedString message, PopupLink link, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Color titleColor = null,
#else
            Color? titleColor = null,
#endif
            bool acceptEnterSpaceForOkCancel = false)
        {
            return ShowMessagePopupAsync(title, message, link != null ? new List<PopupLink> { link } : null, okButtonText, cancelButtonText,
                titleColor, acceptEnterSpaceForOkCancel);
        }

        /* focusFirstLink: the gamepad focus starts on the first link instead of the buttons */
        public Task<bool> ShowMessagePopupAsync(string title, FormattedString message, IList<PopupLink> links, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Color titleColor = null,
#else
            Color? titleColor = null,
#endif
            bool acceptEnterSpaceForOkCancel = false, bool focusFirstLink = false)
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
            _links = CopyLinks(links);
            if (_links != null)
                AttachLabelPointerGesture();
            else
                DetachLabelPointerGesture();
            _focusFirstLink = focusFirstLink;

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

            ResetGamepadFocus();
            IsVisible = true;
            MessagePopupScrollView.ScrollToAsync(0, 0, false);
            return _messagePopupTcs.Task;

        }

        /* The links that have a span and an action, or null if none has */
        private static List<PopupLink> CopyLinks(IList<PopupLink> links)
        {
            if (links == null)
                return null;
            List<PopupLink> result = new List<PopupLink>();
            foreach (PopupLink link in links)
            {
                if (link != null && link.Span != null && link.Open != null)
                    result.Add(link);
            }
            return result.Count > 0 ? result : null;
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
            _links = null;
#if GNH_MAUI && WINDOWS
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
            if (_links == null)
                return;

            var pos = e.GetPosition(MessagePopupLabel);
            if (pos == null)
                return;

            bool overLink = false;
            foreach (PopupLink link in _links)
            {
                var spatial = link.Span as Microsoft.Maui.Controls.Internals.ISpatialElement;
                if (spatial != null && spatial.Region.Contains(pos.Value))
                {
                    overLink = true;
                    break;
                }
            }

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

        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);
            if (propertyName == nameof(IsVisible))
            {
                if (!IsVisible)
                    RestoreSpanHighlight();
                UpdateGamepadSubscription(IsVisible);
            }
        }

        /* Subscribed only while the popup is shown, so the focus ring follows the input mode */
        private void UpdateGamepadSubscription(bool subscribe)
        {
            if (subscribe == _gamepadSubscribed)
                return;
            _gamepadSubscribed = subscribe;
            if (subscribe)
            {
                GHGamepadService.StateChanged += GamepadService_StateChanged;
                RefreshGamepadVisuals(true);
            }
            else
            {
                GHGamepadService.StateChanged -= GamepadService_StateChanged;
            }
        }

        private void GamepadService_StateChanged()
        {
            if (IsPopupOpen)
                RefreshGamepadVisuals(true);
        }

        /* Initial focus follows the Enter key: Cancel when Enter would not accept, else OK; the first link instead
           when the popup was shown with focusFirstLink */
        private void ResetGamepadFocus()
        {
            _gamepadAArmed = false;
            RestoreSpanHighlight();
            _gamepadFocus.SetFocus(MessagePopupCancelButton.IsVisible && !_acceptEnterSpaceForOkCancel ? 1 : 0);
            _linkFocus = _focusFirstLink && HasLinks ? 0 : -1;
            RefreshGamepadVisuals(false);
        }

        private bool HasLinks { get { return _links != null && _links.Count > 0; } }

        /* Shows the focus on the focused link or button, only while the gamepad is the active input */
        private void RefreshGamepadVisuals(bool scrollLink)
        {
            if (!HasLinks || _linkFocus < 0 || _linkFocus >= _links.Count)
            {
                _linkFocus = -1;
                RestoreSpanHighlight();
                _gamepadFocus.RefreshVisuals();
                return;
            }
            _gamepadFocus.ClearVisuals();
            PopupLink link = _links[_linkFocus];
            if (GHGamepadService.IsActive)
            {
                HighlightSpan(link.Span);
                if (scrollLink)
                    ScrollLinkIntoView(link);
            }
            else
            {
                RestoreSpanHighlight();
            }
        }

        /* index -1 gives the focus back to the button that had it */
        private void SetLinkFocus(int index)
        {
            _linkFocus = index;
            RefreshGamepadVisuals(true);
        }

        /* Up/Down: through the links in order, then the buttons below the last one; no wrap */
        private void MoveFocusVertical(int dir)
        {
            if (!HasLinks)
                return;
            if (_linkFocus < 0)
            {
                if (dir < 0)
                    SetLinkFocus(_links.Count - 1);
                return;
            }
            int next = _linkFocus + dir;
            if (next < 0)
                return;
            SetLinkFocus(next < _links.Count ? next : -1);
        }

        /* Left/Right: between the links while one has the focus, else between the buttons */
        private void MoveFocusHorizontal(int dir)
        {
            if (_linkFocus >= 0 && HasLinks)
            {
                int next = _linkFocus + dir;
                if (next >= 0 && next < _links.Count)
                    SetLinkFocus(next);
                return;
            }
            _gamepadFocus.Move(dir, false);
        }

        private void OpenLink(PopupLink link)
        {
            if (_openingLink || link == null || link.Open == null)
                return;
            _openingLink = true;
            Action open = async () =>
            {
                try
                {
                    await link.Open();
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine(ex);
                }
                finally
                {
                    _openingLink = false;
                }
            };
#if GNH_MAUI
            MainThread.BeginInvokeOnMainThread(open);
#else
            Device.BeginInvokeOnMainThread(open);
#endif
        }

        /* Tints span as the focused link, restoring the previously tinted one */
        private void HighlightSpan(Span span)
        {
            if (ReferenceEquals(span, _highlightedSpan))
                return;
            RestoreSpanHighlight();
            if (span == null)
                return;
            try
            {
                _spanTextColorSet = span.IsSet(Span.TextColorProperty);
                _spanTextColor = _spanTextColorSet ? span.GetValue(Span.TextColorProperty) : null;
                _spanBackgroundSet = span.IsSet(Span.BackgroundColorProperty);
                _spanBackground = _spanBackgroundSet ? span.GetValue(Span.BackgroundColorProperty) : null;
                _highlightedSpan = span;
                span.TextColor = GHColors.White;
                span.BackgroundColor = UIUtils.GamepadFocusBackgroundColor;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        /* Restores the tinted span's own colors, or their unset state */
        private void RestoreSpanHighlight()
        {
            Span span = _highlightedSpan;
            if (span == null)
                return;
            _highlightedSpan = null;
            try
            {
                if (_spanTextColorSet)
                    span.SetValue(Span.TextColorProperty, _spanTextColor);
                else
                    span.ClearValue(Span.TextColorProperty);
                if (_spanBackgroundSet)
                    span.SetValue(Span.BackgroundColorProperty, _spanBackground);
                else
                    span.ClearValue(Span.BackgroundColorProperty);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        /* DIPs kept between a scrolled-to link and the viewport edge */
        private const double LinkScrollMargin = 8.0;
        /* Spacing of the points tested against a span's region */
        private const double LinkProbeStepX = 8.0;
        private const double LinkProbeStepY = 2.0;
        /* A text line in font sizes, for the estimated bounds */
        private const double LinkLineHeightFactor = 1.5;

        /* Scrolls the message minimally so that the link shows */
        private void ScrollLinkIntoView(PopupLink link)
        {
            try
            {
                double top, bottom;
                if (!TryGetLinkVerticalBounds(link, out top, out bottom))
                    return;
                ScrollView scrollView = MessagePopupScrollView;
                double viewport = scrollView.Height;
                if (viewport <= 0.0)
                    return;
                top += MessagePopupLabel.Y;
                bottom += MessagePopupLabel.Y;
                double scrollY = scrollView.ScrollY;
                double target;
                if (top - LinkScrollMargin < scrollY)
                    target = top - LinkScrollMargin;
                else if (bottom + LinkScrollMargin > scrollY + viewport)
                    target = bottom + LinkScrollMargin - viewport;
                else
                    return;
                double maxY = Math.Max(0.0, scrollView.ContentSize.Height - viewport);
                target = Math.Max(0.0, Math.Min(maxY, target));
                if (target != scrollY)
                    _ = scrollView.ScrollToAsync(0, target, false);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        /* The link's top and bottom in label coordinates: from its span's region where the platform fills one
           (not on Avalonia), else estimated from its character offsets in the message */
        private bool TryGetLinkVerticalBounds(PopupLink link, out double top, out double bottom)
        {
            top = 0.0;
            bottom = 0.0;
            double width = MessagePopupLabel.Width;
            double height = MessagePopupLabel.Height;
            if (link == null || link.Span == null || width <= 0.0 || height <= 0.0)
                return false;

#if GNH_MAUI
            var spatial = link.Span as Microsoft.Maui.Controls.Internals.ISpatialElement;
#else
            var spatial = link.Span as Xamarin.Forms.Internals.ISpatialElement;
#endif
            if (spatial != null)
            {
                Region region = spatial.Region;
                bool found = false;
                double x, y;
                for (y = 0.0; y < height; y += LinkProbeStepY)
                {
                    for (x = 0.0; x < width; x += LinkProbeStepX)
                    {
                        if (region.Contains(new Point(x, y)))
                        {
                            if (!found)
                            {
                                top = y;
                                found = true;
                            }
                            bottom = y + LinkProbeStepY;
                            break;
                        }
                    }
                }
                if (found)
                    return true;
            }

            FormattedString formatted = MessagePopupLabel.FormattedText;
            if (formatted == null)
                return false;
            int total = 0;
            int start = -1;
            int end = -1;
            foreach (Span span in formatted.Spans)
            {
                int length = span.Text != null ? span.Text.Length : 0;
                if (ReferenceEquals(span, link.Span))
                {
                    start = total;
                    end = total + length;
                }
                total += length;
            }
            if (start < 0 || total <= 0)
                return false;
            top = height * start / total;
            bottom = Math.Min(height, height * end / total + MessagePopupLabel.FontSize * LinkLineHeightFactor);
            return true;
        }

        /* Returns true while the popup is open */
        public bool HandleGamepadButton(GHGamepadButtonEvent e)
        {
            if (!IsPopupOpen)
                return false;
            if (!MessagePopupOkButton.IsVisible && !MessagePopupCancelButton.IsVisible)
                return true; /* Non-blocking popup */
            if (GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen) /* A link opened a page or the system browser */
                return true;

            if (e.RevealsUi)
            {
                if (e.Button == GHGamepadButton.A)
                    _gamepadAArmed = false;
                RefreshGamepadVisuals(true);
                return true;
            }

            bool isPressOrRepeat = e.Kind == GHGamepadButtonEventKind.Pressed || e.Kind == GHGamepadButtonEventKind.Repeat;
            switch (e.Button)
            {
                case GHGamepadButton.DPadUp:
                case GHGamepadButton.LeftStickUp:
                    if (isPressOrRepeat)
                        MoveFocusVertical(-1);
                    break;
                case GHGamepadButton.DPadDown:
                case GHGamepadButton.LeftStickDown:
                    if (isPressOrRepeat)
                        MoveFocusVertical(1);
                    break;
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.LeftStickLeft:
                    if (isPressOrRepeat)
                        MoveFocusHorizontal(-1);
                    break;
                case GHGamepadButton.DPadRight:
                case GHGamepadButton.LeftStickRight:
                    if (isPressOrRepeat)
                        MoveFocusHorizontal(1);
                    break;
                case GHGamepadButton.A:
                    if (e.Kind == GHGamepadButtonEventKind.Pressed)
                    {
                        _gamepadAArmed = true;
                        _gamepadAPressId = e.PressId;
                    }
                    else if (e.Kind == GHGamepadButtonEventKind.Released)
                    {
                        if (e.IsCancelled && e.PressId == _gamepadAPressId)
                            _gamepadAArmed = false;
                    }
                    else if (e.Kind == GHGamepadButtonEventKind.ShortPressed)
                    {
                        if (_gamepadAArmed && e.PressId == _gamepadAPressId)
                        {
                            _gamepadAArmed = false;
                            if (_linkFocus >= 0 && HasLinks && _linkFocus < _links.Count)
                                OpenLink(_links[_linkFocus]);
                            else
                                _gamepadFocus.Activate();
                        }
                    }
                    break;
                case GHGamepadButton.B:
                    if (e.Kind == GHGamepadButtonEventKind.Pressed)
                    {
                        /* Escape semantics */
                        if (MessagePopupCancelButton.IsVisible)
                            DismissPopup();
                        else
                            AcceptPopup();
                    }
                    break;
                default:
                    break;
            }
            return true;
        }
    }
}

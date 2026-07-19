using System;
using System.Threading.Tasks;
using GnollHackX;


#if GNH_MAUI
using Microsoft.Maui;
using Microsoft.Maui.Controls;
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
        private TaskCompletionSource<bool> _messagePopupTcs;
        private bool _acceptEnterSpaceForOkCancel = false;

        public MessagePopupView()
        {
            InitializeComponent();
            IsVisible = false;
            if (GHApp.DarkMode)
            {
                MessagePopupFrame.BackgroundColor = GHColors.MsgBoxDarkModeBkgColor;
            }
        }

        public bool IsPopupOpen => IsVisible;
        public bool IsPopupOkCancel => MessagePopupCancelButton.IsVisible;

        public void ClosePopup()
        {
            _messagePopupTcs?.TrySetResult(false);
            IsVisible = false;
        }

        public void AcceptPopup()
        {
            _messagePopupTcs?.TrySetResult(true);
            IsVisible = false;
        }

        public void CleanPopup()
        {
            _messagePopupTcs?.TrySetResult(false);
        }

        /// <summary>
        /// Closes the popup, accepting it if only OK is shown, or cancelling if Cancel is visible.
        /// Useful for programmatic page-close scenarios.
        /// </summary>
        public void ForceClosePopup()
        {
            if (IsPopupOpen)
            {
                if (IsPopupOkCancel)
                    ClosePopup();
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
            _messagePopupTcs?.TrySetResult(false);
            _messagePopupTcs = null;

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

        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Color titleColor = null,
#else
            Color? titleColor = null,
#endif
            bool acceptEnterSpaceForOkCancel = false)
        {
            _acceptEnterSpaceForOkCancel = acceptEnterSpaceForOkCancel;
            _messagePopupTcs?.TrySetResult(false);
            _messagePopupTcs = new TaskCompletionSource<bool>();

            MessagePopupTitleLabel.Text = title;
            MessagePopupTitleLabel.TextColor = titleColor ?? GHColors.TitleGoldColor;
            MessagePopupLabel.Text = message;

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
            return _messagePopupTcs.Task;

        }

        private void MessagePopupOkButton_Clicked(object sender, EventArgs e)
        {
            AcceptPopup();
        }

        private void MessagePopupCancelButton_Clicked(object sender, EventArgs e)
        {
            ClosePopup();
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
                            ClosePopup();
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
                        ClosePopup();
                    else
                        AcceptPopup();
                }
                return true;
            }
            return false;
        }
    }
}

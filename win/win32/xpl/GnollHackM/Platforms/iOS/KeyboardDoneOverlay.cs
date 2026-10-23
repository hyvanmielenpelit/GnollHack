using System;
using System.Runtime.CompilerServices;
using CoreGraphics;
using Foundation;
using GnollHackX;
using Microsoft.Maui;
using Microsoft.Maui.Handlers;
using UIKit;

namespace GnollHackM.Platforms.iOS
{
    /// <summary>
    /// On iOS 27 and later, replaces MAUI's Done accessory on Entry and Picker with a close button in the app
    /// window, floating over the page just above the keyboard. The accessory itself is a transparent strip
    /// 44 points tall that iOS counts as part of the keyboard, so pages padded above it showed an empty band.
    /// The close button ends editing, so an Entry gets no Completed event. A Picker takes the row on its wheel
    /// whenever editing ends, including the close button and MAUI's tap outside the picker.
    /// Earlier iOS versions run with UIDesignRequiresCompatibility and keep the accessory. UI thread only.
    /// </summary>
    public static class KeyboardDoneOverlay
    {
        private const string MapperKey = "GHKeyboardDoneOverlay";
        private const double ButtonSize = 44;
        private const double ButtonBottomSpacing = 4;
        private const double ButtonTrailingMargin = 8;

        /* Values are the Picker, or EntryMarker for an Entry */
        private static readonly ConditionalWeakTable<UITextField, object> _inputs = new ConditionalWeakTable<UITextField, object>();
        private static readonly object EntryMarker = new object();
        private static WeakReference<UITextField> _input;
        private static UIButton _button;
        private static CGRect _keyboardFrame = CGRect.Empty;
        private static bool _keyboardShowing;
        private static double _animationDuration = 0.25;
        private static NSObject[] _observers;

        public static void Register()
        {
            if (!OperatingSystem.IsIOSVersionAtLeast(27) || _observers != null)
                return;

            EntryHandler.Mapper.AppendToMapping(MapperKey, (handler, entry) => DetachAccessory(handler.PlatformView, EntryMarker));
            PickerHandler.Mapper.AppendToMapping(MapperKey, (handler, picker) => DetachAccessory(handler.PlatformView, picker));

            NSNotificationCenter center = NSNotificationCenter.DefaultCenter;
            _observers = new NSObject[]
            {
                center.AddObserver(UITextField.TextDidBeginEditingNotification, OnBeginEditing),
                center.AddObserver(UITextField.TextDidEndEditingNotification, OnEndEditing),
                center.AddObserver(UIKeyboard.WillShowNotification, OnKeyboardWillShow),
                center.AddObserver(UIKeyboard.WillChangeFrameNotification, OnKeyboardWillChangeFrame),
                center.AddObserver(UIKeyboard.WillHideNotification, OnKeyboardWillHide),
            };
        }

        private static void DetachAccessory(UITextField field, object input)
        {
            try
            {
                if (field?.InputAccessoryView == null)
                    return;

                _inputs.AddOrUpdate(field, input);
                field.InputAccessoryView = null;
                if (field.IsFirstResponder)
                    field.ReloadInputViews();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void OnBeginEditing(NSNotification notification)
        {
            try
            {
                UITextField field = notification.Object as UITextField;
                _input = field != null && _inputs.TryGetValue(field, out object _) ? new WeakReference<UITextField>(field) : null;
                UpdateButton();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void OnEndEditing(NSNotification notification)
        {
            try
            {
                if (notification.Object is UITextField field && _inputs.TryGetValue(field, out object value) && value is IPicker picker)
                    ApplyPickerWheel(field, picker);

                if (GetInput() == notification.Object)
                {
                    _input = null;
                    UpdateButton();
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void OnKeyboardWillShow(NSNotification notification)
        {
            try
            {
                _keyboardShowing = true;
                ReadKeyboardInfo(notification);
                UpdateButton();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void OnKeyboardWillChangeFrame(NSNotification notification)
        {
            try
            {
                /* Also posted on hide, possibly after WillHide */
                if (!_keyboardShowing)
                    return;

                ReadKeyboardInfo(notification);
                UpdateButton();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void OnKeyboardWillHide(NSNotification notification)
        {
            try
            {
                _keyboardShowing = false;
                ReadKeyboardInfo(notification);
                UpdateButton();
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        private static void ReadKeyboardInfo(NSNotification notification)
        {
            NSDictionary userInfo = notification.UserInfo;
            if (userInfo == null)
                return;

            if (userInfo[UIKeyboard.FrameEndUserInfoKey] is NSValue frameValue)
                _keyboardFrame = frameValue.CGRectValue;
            if (userInfo[UIKeyboard.AnimationDurationUserInfoKey] is NSNumber durationNumber && durationNumber.DoubleValue > 0)
                _animationDuration = durationNumber.DoubleValue;
        }

        private static UITextField GetInput()
        {
            if (_input != null && _input.TryGetTarget(out UITextField field))
                return field;
            return null;
        }

        private static void UpdateButton()
        {
            UITextField input = GetInput();
            UIWindow window = input?.Window;
            if (!_keyboardShowing || window == null || _keyboardFrame.IsEmpty)
            {
                HideButton();
                return;
            }

            /* The keyboard frame is in screen coordinates */
            CGRect keyboard = window.ConvertRectFromCoordinateSpace(_keyboardFrame, window.Screen.CoordinateSpace);
            CGRect bounds = window.Bounds;
            if (keyboard.Y >= bounds.Height)
            {
                HideButton();
                return;
            }

            UIButton button = GetButton();
            if (button == null)
                return;

            CGRect frame = new CGRect(
                bounds.Width - window.SafeAreaInsets.Right - ButtonTrailingMargin - ButtonSize,
                keyboard.Y - ButtonBottomSpacing - ButtonSize,
                ButtonSize,
                ButtonSize);

            if (button.Superview != window)
            {
                button.RemoveFromSuperview();
                window.AddSubview(button);
            }
            window.BringSubviewToFront(button);

            if (button.Hidden)
            {
                button.Frame = frame;
                button.Alpha = 0;
                button.Hidden = false;
                UIView.Animate(_animationDuration, () => button.Alpha = 1);
            }
            else
            {
                UIView.Animate(_animationDuration, () =>
                {
                    button.Frame = frame;
                    button.Alpha = 1;
                });
            }
        }

        private static void HideButton()
        {
            UIButton button = _button;
            if (button == null || button.Hidden)
                return;

            UIView.Animate(_animationDuration, () => button.Alpha = 0, () =>
            {
                if (button.Alpha == 0)
                    button.Hidden = true;
            });
        }

        private static UIButton GetButton()
        {
            if (_button != null || !OperatingSystem.IsIOSVersionAtLeast(26))
                return _button;

            /* Matches the look of MAUI's glass Done button */
            UIButtonConfiguration configuration = UIButtonConfiguration.GlassButtonConfiguration;
            configuration.Image = UIImage.GetSystemImage("xmark");
            configuration.CornerStyle = UIButtonConfigurationCornerStyle.Capsule;
            configuration.ButtonSize = UIButtonConfigurationSize.Medium;
            configuration.BaseForegroundColor = UIColor.Label;

            UIButton button = UIButton.FromType(UIButtonType.System);
            button.Configuration = configuration;
            button.AccessibilityLabel = NSBundle.FromIdentifier("com.apple.UIKit")?.GetLocalizedString("Done") ?? "Done";
            button.Hidden = true;
            button.TouchUpInside += OnButtonTouchUpInside;
            _button = button;
            return button;
        }

        private static void OnButtonTouchUpInside(object sender, EventArgs e)
        {
            try
            {
                UITextField input = GetInput();
                if (input == null)
                {
                    HideButton();
                    return;
                }

                input.EndEditing(true);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("KeyboardDoneOverlay: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        /// <summary>
        /// Applies the row on the wheel to the Picker, as MAUI's Done does; MAUI's end of editing alone does not.
        /// </summary>
        private static void ApplyPickerWheel(UITextField input, IPicker picker)
        {
            if (!(input.InputView is UIPickerView pickerView))
                return;

            int row = (int)pickerView.SelectedRowInComponent(0);
            if (row >= 0 && row < picker.GetCount() && row != picker.SelectedIndex)
                picker.SelectedIndex = row;
        }
    }
}

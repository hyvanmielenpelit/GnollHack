using System;
using System.Collections.Generic;
using System.Threading;
using GnollHackX;
using SkiaSharp;

#if GNH_MAUI
using Microsoft.Maui.Controls;
using SkiaSharp.Views.Maui;

namespace GnollHackM
#else
using Xamarin.Forms;
using SkiaSharp.Views.Forms;
using GnollHackX.Controls;

namespace GnollHackX.Pages.Game
#endif
{
    /* GamePage's dialogs in visual z-order, topmost first; None when no dialog takes gamepad input */
    internal enum GHGamepadDialogContext
    {
        None = 0,
        Tips,
        MoreCommandsTouch,
        Popup,
        Yn,
        GetLine,
        Text,
        MenuCount,
        Menu,
    }

    /* Gamepad navigation of GamePage's dialogs: context detection, press arming, the menu focus and its scrolling into
       view, the per-dialog button tables and the dialog hints. While a dialog is shown it takes every button. A acts on
       an armed ShortPressed (or LongPressed); the other buttons act on Pressed, and the D-pad, LB, RB and the left
       stick also on armed Repeat. Everything runs on the UI thread except the menu paint, which reads the focus index
       through MenuGamepadFocusIndex. */
    public partial class GamePage
    {
        private const float GamepadDialogAxisDominance = 1.2f;   /* the stick axis must exceed the other this much */
        private const float GamepadDialogNeutralAxis = 0.05f;    /* below this on both axes: no direction */
        private const float GamepadDialogStickScrollFactor = 1.6f; /* canvas heights per second at full deflection */
        private const int GamepadMenuPageFallbackRows = 8;       /* page step before the rows have been painted */
        private const int GamepadMenuStepDelta = 120;            /* the keyboard's arrow, page and end amounts */
        private const int GamepadMenuPageDelta = 1200;
        private const int GamepadMenuEndDelta = 1200000;

        /* Bumped by every dialog show; with the context it forms the token a press is armed with */
        private int _gamepadDialogSerial = 0;
        private readonly int[] _gamepadDialogArmedToken = NewGamepadIndexArray((int)GHGamepadButton.Count);
        private readonly int[] _gamepadDialogArmedPressId = new int[(int)GHGamepadButton.Count];
        private int _gamepadDialogAnsweredToken = -1;           /* a y/n, popup, text window or menu already answered */
        private GHGamepadSticks _gamepadDialogSticks;            /* the latest stick snapshot */
        private bool _gamepadDialogVisualActive = false;
        private int _gamepadDialogHintSignature = int.MinValue;
        private GHGamepadFocusList _ynGamepadFocus = null;

        /* Menu focus, separate from the selection; -1 for none */
        private int _menuGamepadFocusIndex = -1;
        private int MenuGamepadFocusIndex
        {
            get { return Interlocked.CompareExchange(ref _menuGamepadFocusIndex, 0, 0); }
            set { Interlocked.Exchange(ref _menuGamepadFocusIndex, value); }
        }
        /* Under _menuPositionLock */
        private readonly int[] _savedMenuFocusIndex = NewGamepadIndexArray((int)ghmenu_styles.MAX_GHMENU_STYLES);
        private bool _menuGamepadFocusResolved = true;
        private int _menuGamepadFocusPending = 0;               /* scroll the focus into view once rows are painted */
        private bool IsMenuGamepadFocusPending
        {
            get { return Interlocked.CompareExchange(ref _menuGamepadFocusPending, 0, 0) != 0; }
        }

        private static int[] NewGamepadIndexArray(int length)
        {
            int[] array = new int[length];
            int i;
            for (i = 0; i < length; i++)
                array[i] = -1;
            return array;
        }

        /* Context and arming */

        internal GHGamepadDialogContext GetGamepadDialogContext()
        {
            if (PlayingReplay || LoadingGrid.IsVisible)
                return GHGamepadDialogContext.None;
            if (TipView.IsVisible)
                return GHGamepadDialogContext.Tips;
            if (MoreCommandsGrid.IsVisible)
                return GHGamepadDialogContext.MoreCommandsTouch;
            if (PopupGrid.IsVisible)
                return GHGamepadDialogContext.Popup;
            if (YnGrid.IsVisible)
                return GHGamepadDialogContext.Yn;
            if (GetLineGrid.IsVisible)
                return GHGamepadDialogContext.GetLine;
            if (TextGrid.IsVisible)
                return GHGamepadDialogContext.Text;
            if (MenuGrid.IsVisible)
                return MenuCountGrid.IsVisible ? GHGamepadDialogContext.MenuCount : GHGamepadDialogContext.Menu;
            return GHGamepadDialogContext.None;
        }

        private void BumpGamepadDialogSerial()
        {
            Interlocked.Increment(ref _gamepadDialogSerial);
            _gamepadHintsDirty = true;
        }

        private int GetGamepadDialogToken(GHGamepadDialogContext ctx)
        {
            return ((int)ctx << 24) | (Interlocked.CompareExchange(ref _gamepadDialogSerial, 0, 0) & 0xFFFFFF);
        }

        /* A dialog that is fading out or has already been answered swallows input */
        private bool IsGamepadDialogInTransition(GHGamepadDialogContext ctx, int token)
        {
            if (_gamepadDialogAnsweredToken == token)
                return true;
            switch (ctx)
            {
                case GHGamepadDialogContext.Menu:
                case GHGamepadDialogContext.MenuCount:
                    return MenuDrawOnlyClear;
                case GHGamepadDialogContext.Text:
                    return DelayedTextHideOn && !DelayedTextHideCancelled;
                default:
                    return false;
            }
        }

        private void MarkGamepadDialogAnswered(int token)
        {
            _gamepadDialogAnsweredToken = token;
        }

        /* R25: the left stick's virtual directions count only along the dominant axis of the latest snapshot */
        private bool IsGamepadLeftStickVertical()
        {
            float ax = Math.Abs(_gamepadDialogSticks.LeftX);
            float ay = Math.Abs(_gamepadDialogSticks.LeftY);
            if (ax < GamepadDialogNeutralAxis && ay < GamepadDialogNeutralAxis)
                return true;
            return ay > GamepadDialogAxisDominance * ax;
        }

        private bool IsGamepadLeftStickHorizontal()
        {
            float ax = Math.Abs(_gamepadDialogSticks.LeftX);
            float ay = Math.Abs(_gamepadDialogSticks.LeftY);
            if (ax < GamepadDialogNeutralAxis && ay < GamepadDialogNeutralAxis)
                return true;
            return ax > GamepadDialogAxisDominance * ay;
        }

        /* -1 up, 1 down, 0 otherwise; the left stick's horizontal directions never count */
        private int GetGamepadDialogVerticalStep(GHGamepadButton button, out bool isDPad)
        {
            isDPad = false;
            switch (button)
            {
                case GHGamepadButton.DPadUp:
                    isDPad = true;
                    return -1;
                case GHGamepadButton.DPadDown:
                    isDPad = true;
                    return 1;
                case GHGamepadButton.LeftStickUp:
                    return IsGamepadLeftStickVertical() ? -1 : 0;
                case GHGamepadButton.LeftStickDown:
                    return IsGamepadLeftStickVertical() ? 1 : 0;
                default:
                    return 0;
            }
        }

        internal bool TryHandleDialogGamepadButton(GHGamepadButtonEvent e)
        {
            GHGamepadDialogContext ctx = GetGamepadDialogContext();
            if (ctx == GHGamepadDialogContext.None)
                return false;

            int b = (int)e.Button;
            if (b < 0 || b >= _gamepadDialogArmedToken.Length)
                return true;

            /* A press is armed in the dialog instance that got its Pressed; a reveal press is never armed */
            int token = GetGamepadDialogToken(ctx);
            if (e.Kind == GHGamepadButtonEventKind.Pressed)
            {
                _gamepadDialogArmedToken[b] = e.RevealsUi ? -1 : token;
                _gamepadDialogArmedPressId[b] = e.PressId;
            }
            bool armed = _gamepadDialogArmedToken[b] == token && _gamepadDialogArmedPressId[b] == e.PressId;
            if (e.Kind == GHGamepadButtonEventKind.ShortPressed
                || (e.Kind == GHGamepadButtonEventKind.Released && e.IsCancelled))
                _gamepadDialogArmedToken[b] = -1;

            /* R22: the press that makes the gamepad the active input only shows the focus */
            if (e.RevealsUi)
            {
                RevealGamepadDialogFocus(ctx);
                _gamepadHintsDirty = true;
                return true;
            }

            if (IsGamepadDialogInTransition(ctx, token))
                return true;

            bool isPress = e.Kind == GHGamepadButtonEventKind.Pressed;
            bool isStep = isPress || (e.Kind == GHGamepadButtonEventKind.Repeat && armed);
            bool isTap = e.Kind == GHGamepadButtonEventKind.ShortPressed && armed;
            bool isLong = e.Kind == GHGamepadButtonEventKind.LongPressed && armed;
            /* Only A has a long-press action; the other buttons stay armed, so their repeats continue past it */
            if (isLong && e.Button == GHGamepadButton.A)
                _gamepadDialogArmedToken[b] = -1;

            switch (ctx)
            {
                case GHGamepadDialogContext.Tips:
                    HandleTipsGamepadButton(e.Button, isPress, isTap);
                    break;
                case GHGamepadDialogContext.MoreCommandsTouch:
                    HandleMoreCommandsGamepadButton(e.Button, isPress, isStep);
                    break;
                case GHGamepadDialogContext.Popup:
                    if ((e.Button == GHGamepadButton.A && isTap) || (e.Button == GHGamepadButton.B && isPress))
                    {
                        MarkGamepadDialogAnswered(token);
                        PopupOkButton_Clicked(null, null);
                    }
                    break;
                case GHGamepadDialogContext.Yn:
                    HandleYnGamepadButton(e.Button, token, isPress, isStep, isTap);
                    break;
                case GHGamepadDialogContext.GetLine:
                    HandleGetLineGamepadButton(e.Button, isPress, isTap);
                    break;
                case GHGamepadDialogContext.Text:
                    HandleTextGamepadButton(e.Button, token, isPress, isStep, isTap);
                    break;
                case GHGamepadDialogContext.MenuCount:
                    HandleMenuCountGamepadButton(e.Button, isPress, isStep, isTap);
                    break;
                case GHGamepadDialogContext.Menu:
                    HandleMenuGamepadButton(e.Button, token, isPress, isStep, isTap, isLong);
                    break;
                default:
                    break;
            }

            UpdateGamepadDialogHintSignature(GetGamepadDialogContext());
            return true;
        }

        internal void UpdateDialogGamepadSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            _gamepadDialogSticks = sticks;
            bool active = GHGamepadService.IsActive;
            if (active != _gamepadDialogVisualActive)
            {
                _gamepadDialogVisualActive = active;
                if (_ynGamepadFocus != null)
                    _ynGamepadFocus.RefreshVisuals();
            }

            GHGamepadDialogContext ctx = GetGamepadDialogContext();
            UpdateGamepadDialogHintSignature(ctx);
            if (ctx != GHGamepadDialogContext.Menu && ctx != GHGamepadDialogContext.Text)
                return;
            if (_gamepadRightStickDisarmed || dtSeconds <= 0 || sticks.RightMagnitude <= 0f)
                return;
            if (IsGamepadDialogInTransition(ctx, GetGamepadDialogToken(ctx)))
                return;

            /* The right stick scrolls like a wheel, faster the further it is pushed */
            float y = sticks.RightY;
            if (Math.Abs(y) < GamepadDialogNeutralAxis)
                return;
            float factor = -Math.Sign(y) * y * y * GamepadDialogStickScrollFactor * (float)dtSeconds;
            if (ctx == GHGamepadDialogContext.Menu)
            {
                ScrollMenuByPixels(factor * GetMenuCanvasHeight());
                if (!MenuEquipmentSideShown)
                    ClampMenuGamepadFocusIntoView();
            }
            else
            {
                float canvasheight;
                lock (_savedTextCanvasLock)
                {
                    canvasheight = _savedTextCanvasHeight;
                }
                ScrollTextWindowByPixels(factor * canvasheight);
            }
        }

        private void RevealGamepadDialogFocus(GHGamepadDialogContext ctx)
        {
            switch (ctx)
            {
                case GHGamepadDialogContext.Yn:
                    if (_ynGamepadFocus != null)
                        _ynGamepadFocus.RefreshVisuals();
                    break;
                case GHGamepadDialogContext.Menu:
                    RevealMenuGamepadFocus();
                    break;
                default:
                    break;
            }
        }

        /* Small dialogs */

        private void HandleTipsGamepadButton(GHGamepadButton button, bool isPress, bool isTap)
        {
            if (button == GHGamepadButton.A && isTap)
            {
                TipView_Touch(this, new SKTouchEventArgs(0L, SKTouchAction.Released, new SKPoint(0, 0), true));
            }
            else if (button == GHGamepadButton.B && isPress)
            {
                ShownTip = 100; /* Move to end */
                TipView_Touch(this, new SKTouchEventArgs(0L, SKTouchAction.Released, new SKPoint(0, 0), true));
            }
        }

        /* The touch More Commands canvas: B closes it and the shoulder buttons and D-pad page as the keyboard does */
        private void HandleMoreCommandsGamepadButton(GHGamepadButton button, bool isPress, bool isStep)
        {
            switch (button)
            {
                case GHGamepadButton.B:
                    if (isPress)
                        CommandCanvas_Pressed(null, null);
                    break;
                case GHGamepadButton.LB:
                case GHGamepadButton.DPadLeft:
                    if (isStep)
                        HandleSpecialKeyPress(GHSpecialKey.PageUp, false, false, false);
                    break;
                case GHGamepadButton.RB:
                case GHGamepadButton.DPadRight:
                    if (isStep)
                        HandleSpecialKeyPress(GHSpecialKey.PageDown, false, false, false);
                    break;
                default:
                    break;
            }
        }

        /* R77: focus starts on the button of the question's default answer, else on the leftmost button */
        private void InitYnGamepadFocus()
        {
            BumpGamepadDialogSerial();
            LabeledImageButton[] buttons = { ZeroButton, FirstButton, SecondButton, ThirdButton, FourthButton };
            if (_ynGamepadFocus == null)
            {
                _ynGamepadFocus = new GHGamepadFocusList();
                AddYnGamepadFocusEntry(ZeroButton);
                AddYnGamepadFocusEntry(FirstButton);
                AddYnGamepadFocusEntry(SecondButton);
                AddYnGamepadFocusEntry(ThirdButton);
                AddYnGamepadFocusEntry(FourthButton);
            }

            int start = 0;
            if (_ynDefaultResponse != 0)
            {
                int i;
                for (i = 0; i < buttons.Length; i++)
                {
                    if (buttons[i].IsVisible && (int)buttons[i].BtnLetter == _ynDefaultResponse)
                    {
                        start = i;
                        break;
                    }
                }
            }
            _ynGamepadFocus.SetFocus(start);
        }

        private void AddYnGamepadFocusEntry(LabeledImageButton button)
        {
            _ynGamepadFocus.Add(button, () => YnButton_Pressed(null, null, button.GHCommand),
                () => button.IsVisible && button.IsEnabled);
        }

        private void HandleYnGamepadButton(GHGamepadButton button, int token, bool isPress, bool isStep, bool isTap)
        {
            if (_ynGamepadFocus == null)
                return;
            int dir = 0;
            bool isDPad = false;
            switch (button)
            {
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.DPadUp:
                    dir = -1;
                    isDPad = true;
                    break;
                case GHGamepadButton.DPadRight:
                case GHGamepadButton.DPadDown:
                    dir = 1;
                    isDPad = true;
                    break;
                case GHGamepadButton.LeftStickLeft:
                    dir = IsGamepadLeftStickHorizontal() ? -1 : 0;
                    break;
                case GHGamepadButton.LeftStickRight:
                    dir = IsGamepadLeftStickHorizontal() ? 1 : 0;
                    break;
                case GHGamepadButton.A:
                    if (isTap && _ynGamepadFocus.IsAvailableAt(_ynGamepadFocus.FocusIndex))
                    {
                        MarkGamepadDialogAnswered(token);
                        _ynGamepadFocus.Activate();
                    }
                    return;
                case GHGamepadButton.B:
                    if (isPress)
                    {
                        /* As the Escape key: the core turns it into q, n or the default answer */
                        MarkGamepadDialogAnswered(token);
                        YnButton_Pressed(null, null, GHConstants.CancelChar);
                    }
                    return;
                case GHGamepadButton.Y:
                case GHGamepadButton.X:
                    /* Y answers yes and X no when the prompt offers that answer */
                    if (isPress)
                    {
                        LabeledImageButton answer = FindYnGamepadAnswerButton(button == GHGamepadButton.Y ? 'y' : 'n');
                        if (answer != null)
                        {
                            MarkGamepadDialogAnswered(token);
                            YnButton_Pressed(null, null, answer.GHCommand);
                        }
                    }
                    return;
                default:
                    return;
            }
            if (dir != 0 && isStep)
                _ynGamepadFocus.Move(dir, isDPad && isPress);
        }

        /* The visible and enabled y/n button whose response is the letter, or null */
        private LabeledImageButton FindYnGamepadAnswerButton(char letter)
        {
            LabeledImageButton[] buttons = { ZeroButton, FirstButton, SecondButton, ThirdButton, FourthButton };
            int i;
            for (i = 0; i < buttons.Length; i++)
            {
                if (buttons[i].IsVisible && buttons[i].IsEnabled && buttons[i].BtnLetter == letter)
                    return buttons[i];
            }
            return null;
        }

        private void HandleGetLineGamepadButton(GHGamepadButton button, bool isPress, bool isTap)
        {
            switch (button)
            {
                case GHGamepadButton.A:
                    if (isTap && GetLineOkButton.IsEnabled)
                        GetLineOkButton_Clicked(null, null);
                    break;
                case GHGamepadButton.B:
                    /* Cancel, or Random when wishing */
                    if (isPress && GetLineCancelButton.IsEnabled)
                        GetLineCancelButton_Clicked(null, null);
                    break;
                case GHGamepadButton.X:
                    /* "?" opens the core's list of choices */
                    if (isPress && GetLineQuestionMarkButton.IsVisible && GetLineQuestionMarkButton.IsEnabled)
                        GetLineQuestionMarkButton_Clicked(null, null);
                    break;
                case GHGamepadButton.Y:
                    /* Focuses the entry and reopens the on-screen keyboard */
                    if (isPress)
                        ShowGetLineScreenKeyboard();
                    break;
                default:
                    break;
            }
        }

        private void HandleTextGamepadButton(GHGamepadButton button, int token, bool isPress, bool isStep, bool isTap)
        {
            bool isDPad;
            int step = GetGamepadDialogVerticalStep(button, out isDPad);
            if (step != 0)
            {
                if (isStep)
                    ScrollTextWindow(-step * GamepadMenuStepDelta);
                return;
            }
            switch (button)
            {
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.LB:
                    if (isStep)
                        ScrollTextWindow(GamepadMenuPageDelta);
                    break;
                case GHGamepadButton.DPadRight:
                case GHGamepadButton.RB:
                    if (isStep)
                        ScrollTextWindow(-GamepadMenuPageDelta);
                    break;
                case GHGamepadButton.LT:
                    if (isPress)
                        ScrollTextWindow(GamepadMenuEndDelta);
                    break;
                case GHGamepadButton.RT:
                    if (isPress)
                        ScrollTextWindow(-GamepadMenuEndDelta);
                    break;
                case GHGamepadButton.A:
                case GHGamepadButton.B:
                    if ((button == GHGamepadButton.A && isTap) || (button == GHGamepadButton.B && isPress))
                    {
                        MarkGamepadDialogAnswered(token);
                        TextCanvasPressed();
                    }
                    break;
                default:
                    break;
            }
        }

        /* The count dialog; the entry is never focused, so no on-screen keyboard opens */
        private void HandleMenuCountGamepadButton(GHGamepadButton button, bool isPress, bool isStep, bool isTap)
        {
            bool isDPad;
            int step = GetGamepadDialogVerticalStep(button, out isDPad);
            if (step != 0)
            {
                if (isStep)
                    AdjustGamepadMenuCount(-step);
                return;
            }
            switch (button)
            {
                case GHGamepadButton.DPadLeft:
                    if (isStep)
                        AdjustGamepadMenuCount(-10);
                    break;
                case GHGamepadButton.DPadRight:
                    if (isStep)
                        AdjustGamepadMenuCount(10);
                    break;
                case GHGamepadButton.LB:
                    if (isStep)
                        AdjustGamepadMenuCount(-100);
                    break;
                case GHGamepadButton.RB:
                    if (isStep)
                        AdjustGamepadMenuCount(100);
                    break;
                case GHGamepadButton.Y:
                    /* As in the menu, Y is All and X is OK */
                    if (isPress)
                        SetGamepadMenuCountAll();
                    break;
                case GHGamepadButton.A:
                case GHGamepadButton.X:
                    if (((button == GHGamepadButton.A && isTap) || (button == GHGamepadButton.X && isPress))
                        && MenuCountOkButton.IsEnabled)
                        MenuCountOkButton_Clicked(null, null);
                    break;
                case GHGamepadButton.B:
                    if (isPress && MenuCountCancelButton.IsEnabled)
                        MenuCountCancelButton_Clicked(null, null);
                    break;
                default:
                    break;
            }
        }

        /* Changes the count by delta within 0..MaxCount; "All" counts as MaxCount */
        private void AdjustGamepadMenuCount(int delta)
        {
            GHMenuItem item = _countMenuItem;
            if (item == null)
                return;
            int max = Math.Max(0, item.MaxCount);
            if (CountPicker.IsVisible)
            {
                int n = _countPickList.Count;
                if (n < 2)
                    return;
                int index = CountPicker.SelectedIndex;
                int value = index == 0 ? max : index < 0 ? 0 : index - 1;
                value = Math.Max(0, Math.Min(max, value + delta));
                CountPicker.SelectedIndex = Math.Min(n - 1, value + 1);
            }
            else if (MenuCountEntry.IsVisible)
            {
                int value;
                if (!int.TryParse(MenuCountEntry.Text, out value) || value < 0 || value > max)
                    value = max;
                value = Math.Max(0, Math.Min(max, value + delta));
                MenuCountEntry.Text = value.ToString();
            }
        }

        private void SetGamepadMenuCountAll()
        {
            GHMenuItem item = _countMenuItem;
            if (item == null)
                return;
            if (CountPicker.IsVisible)
            {
                if (_countPickList.Count > 0)
                    CountPicker.SelectedIndex = 0;
            }
            else if (MenuCountEntry.IsVisible)
            {
                MenuCountEntry.Text = Math.Max(0, item.MaxCount).ToString();
            }
        }

        /* Menu focus model */

        private static bool IsMenuSectionHeader(GHMenuItem mi)
        {
            ulong headerFlags = (ulong)MenuFlags.MENU_FLAGS_IS_HEADING | (ulong)MenuFlags.MENU_FLAGS_IS_GROUP_HEADING;
            return (mi.Flags & headerFlags) != 0;
        }

        private static bool IsInventoryMenuStyle(ghmenu_styles style)
        {
            return (style >= ghmenu_styles.GHMENU_STYLE_INVENTORY
                    && style <= ghmenu_styles.GHMENU_STYLE_OTHERS_INVENTORY)
                || style == ghmenu_styles.GHMENU_STYLE_INVENTORY_EQUIPMENT;
        }

        /* Selectable rows, and group headings in multi-select menus; plain headings and text are skipped */
        private static bool IsMenuItemGamepadFocusable(IList<GHMenuItem> items, SelectionMode how, int idx)
        {
            if (items == null || idx < 0 || idx >= items.Count || how == SelectionMode.None)
                return false;
            GHMenuItem mi = items[idx];
            if (mi == null)
                return false;
            return mi.Identifier != 0
                || (how == SelectionMode.Multiple && (mi.Flags & (ulong)MenuFlags.MENU_FLAGS_IS_GROUP_HEADING) != 0);
        }

        /* The next focusable row after start in direction dir, or -1 */
        private static int FindMenuGamepadFocusable(IList<GHMenuItem> items, SelectionMode how, int start, int dir)
        {
            if (items == null)
                return -1;
            int i;
            for (i = start + dir; i >= 0 && i < items.Count; i += dir)
            {
                if (IsMenuItemGamepadFocusable(items, how, i))
                    return i;
            }
            return -1;
        }

        private float GetMenuCanvasHeight()
        {
            lock (_savedMenuCanvasLock)
            {
                return _savedMenuCanvasHeight;
            }
        }

        /* A row's painted top and bottom with the scroll offset of that paint taken out; false before it is painted */
        private bool TryGetMenuRowBounds0(int idx, out float top0, out float bottom0)
        {
            top0 = 0;
            bottom0 = 0;
            lock (_menuUIDrawBoundsLock)
            {
                DrawBoundInfos bounds = MenuUIDrawBounds;
                if (bounds == null || bounds.DrawBoundList == null || idx < 0 || idx >= bounds.DrawBoundList.Length)
                    return false;
                SKRect r = bounds.DrawBoundList[idx].DrawBounds;
                if (r.Bottom <= r.Top)
                    return false;
                top0 = r.Top - bounds.ScrollOffset;
                bottom0 = r.Bottom - bounds.ScrollOffset;
                return true;
            }
        }

        private bool IsMenuPainted()
        {
            lock (_menuUIDrawBoundsLock)
            {
                DrawBoundInfos bounds = MenuUIDrawBounds;
                return bounds != null && bounds.DrawBoundList != null && bounds.FirstDrawnMenuItemIdx >= 0;
            }
        }

        /* Whether the row lies fully inside the canvas at the current scroll offset; true if it is not painted yet */
        private bool IsMenuRowFullyVisibleOrUnknown(int idx)
        {
            float top0, bottom0;
            if (!TryGetMenuRowBounds0(idx, out top0, out bottom0))
                return true;
            float offset = InterlockedMenuScrollOffset;
            return top0 + offset >= 0 && bottom0 + offset <= GetMenuCanvasHeight();
        }

        /* The first (dir 1) or last (dir -1) focusable row fully inside the canvas, scanning from start; -1 if none */
        private int FindFullyVisibleMenuGamepadFocusable(IList<GHMenuItem> items, SelectionMode how, int start, int dir)
        {
            if (items == null)
                return -1;
            float height = GetMenuCanvasHeight();
            float offset = InterlockedMenuScrollOffset;
            lock (_menuUIDrawBoundsLock)
            {
                DrawBoundInfos bounds = MenuUIDrawBounds;
                if (bounds == null || bounds.DrawBoundList == null)
                    return -1;
                int count = Math.Min(items.Count, bounds.DrawBoundList.Length);
                int i;
                for (i = start; i >= 0 && i < count; i += dir)
                {
                    if (!IsMenuItemGamepadFocusable(items, how, i))
                        continue;
                    SKRect r = bounds.DrawBoundList[i].DrawBounds;
                    if (r.Bottom <= r.Top)
                        continue;
                    float top = r.Top - bounds.ScrollOffset + offset;
                    float bottom = r.Bottom - bounds.ScrollOffset + offset;
                    if (top >= 0 && bottom <= height)
                        return i;
                }
            }
            return -1;
        }

        /* Called by ShowMenuCanvas: the focus is resolved once the rows are painted, or on the first gamepad event */
        private void InitMenuGamepadFocus()
        {
            BumpGamepadDialogSerial();
            MenuGamepadFocusIndex = -1;
            _menuGamepadFocusResolved = false;
            Interlocked.Exchange(ref _menuGamepadFocusPending, GHGamepadService.IsActive ? 1 : 0);
        }

        private void ClearSavedMenuGamepadFocus()
        {
            int i;
            for (i = 0; i < _savedMenuFocusIndex.Length; i++)
                _savedMenuFocusIndex[i] = -1;
        }

        /* A tap on a row moves the focus there */
        private void SetMenuGamepadFocusFromTap(int idx)
        {
            if (!IsMenuItemGamepadFocusable(MenuCanvas.MenuItems, MenuCanvas.SelectionHow, idx))
                return;
            MenuGamepadFocusIndex = idx;
            _menuGamepadFocusResolved = true;
            _gamepadHintsDirty = true;
        }

        /* Saved position, else the preselected row, else the first fully visible row of a scrolled menu, else the
           first focusable row; -1 leaves the menu in scroll mode */
        private void ResolveMenuGamepadFocus()
        {
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int style = (int)MenuCanvas.MenuStyle;
            int saved = -1;
            lock (_menuPositionLock)
            {
                if (style >= 0 && style < _menuPositionSavingOn.Length && _menuPositionSavingOn[style])
                    saved = _savedMenuFocusIndex[style];
            }

            int focus = -1;
            if (IsMenuItemGamepadFocusable(items, how, saved))
                focus = saved;
            else if (how == SelectionMode.Single && IsMenuItemGamepadFocusable(items, how, MenuCanvas.SelectionIndex))
                focus = MenuCanvas.SelectionIndex;
            else if (InterlockedMenuScrollOffset != 0 && IsMenuPainted())
                focus = FindFullyVisibleMenuGamepadFocusable(items, how, 0, 1);
            if (focus < 0)
                focus = FindMenuGamepadFocusable(items, how, -1, 1);

            _menuGamepadFocusResolved = true;
            SetMenuGamepadFocus(focus);
        }

        private void EnsureMenuGamepadFocusResolved()
        {
            if (!_menuGamepadFocusResolved)
            {
                ResolveMenuGamepadFocus();
                return;
            }
            int focus = MenuGamepadFocusIndex;
            if (focus >= 0 && !IsMenuItemGamepadFocusable(MenuCanvas.MenuItems, MenuCanvas.SelectionHow, focus))
                ResolveMenuGamepadFocus();
        }

        /* On reveal, a focus that touch scrolling has left off screen moves to the first fully visible row */
        private void RevealMenuGamepadFocus()
        {
            if (MenuEquipmentSideShown)
                return;
            if (!_menuGamepadFocusResolved)
            {
                ResolveMenuGamepadFocus();
                return;
            }
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int focus = MenuGamepadFocusIndex;
            bool valid = IsMenuItemGamepadFocusable(items, how, focus);
            if (valid && IsMenuRowFullyVisibleOrUnknown(focus))
                return;
            int visible = FindFullyVisibleMenuGamepadFocusable(items, how, 0, 1);
            if (visible >= 0)
            {
                MenuGamepadFocusIndex = visible;
                _gamepadHintsDirty = true;
            }
            else if (!valid)
            {
                SetMenuGamepadFocus(FindMenuGamepadFocusable(items, how, -1, 1));
            }
        }

        private void SetMenuGamepadFocus(int idx)
        {
            MenuGamepadFocusIndex = idx;
            _gamepadHintsDirty = true;
            EnsureMenuGamepadFocusVisible();
        }

        /* Called from UpdateMenuCanvas while the focus waits for painted rows */
        private void ProcessPendingMenuGamepadFocus()
        {
            if (MenuDrawOnlyClear || MenuEquipmentSideShown)
                return;
            var items = MenuCanvas.MenuItems;
            if (items != null && items.Count > 0 && !IsMenuPainted())
                return;
            Interlocked.Exchange(ref _menuGamepadFocusPending, 0);
            if (!_menuGamepadFocusResolved)
            {
                if (GHGamepadService.IsActive)
                    ResolveMenuGamepadFocus();
                return;
            }
            EnsureMenuGamepadFocusVisible();
        }

        /* Scrolls the menu so that the focused row is inside the canvas with a margin; the jump is immediate */
        private void EnsureMenuGamepadFocusVisible()
        {
            int focus = MenuGamepadFocusIndex;
            if (focus < 0 || MenuEquipmentSideShown)
            {
                Interlocked.Exchange(ref _menuGamepadFocusPending, 0);
                return;
            }
            float height = GetMenuCanvasHeight();
            float top0, bottom0;
            if (height <= 0 || !TryGetMenuRowBounds0(focus, out top0, out bottom0))
            {
                Interlocked.Exchange(ref _menuGamepadFocusPending, 1);
                return;
            }
            Interlocked.Exchange(ref _menuGamepadFocusPending, 0);

            float rowHeight = bottom0 - top0;
            float margin = Math.Max(0f, Math.Min(0.5f * rowHeight, height / 4));
            float offset = InterlockedMenuScrollOffset;
            float want = offset;
            if (rowHeight > height)
                want = -top0;
            else if (top0 + want < margin)
                want = margin - top0;
            else if (bottom0 + want > height - margin)
                want = height - margin - bottom0;
            want = Math.Max(Math.Min(0f, height - TotalMenuHeight), Math.Min(0f, want));
            if (want != offset)
                ScrollMenuByPixels(want - offset);
        }

        /* After a right-stick scroll, a focus that left the canvas moves to the nearest fully visible row */
        private void ClampMenuGamepadFocusIntoView()
        {
            int focus = MenuGamepadFocusIndex;
            if (focus < 0)
                return;
            float top0, bottom0;
            if (!TryGetMenuRowBounds0(focus, out top0, out bottom0))
                return;
            float offset = InterlockedMenuScrollOffset;
            float height = GetMenuCanvasHeight();
            bool above = top0 + offset < 0;
            bool below = bottom0 + offset > height;
            if (!above && !below)
                return;
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int target = above ? FindFullyVisibleMenuGamepadFocusable(items, how, focus + 1, 1)
                : FindFullyVisibleMenuGamepadFocusable(items, how, focus - 1, -1);
            if (target >= 0 && target != focus)
            {
                MenuGamepadFocusIndex = target;
                _gamepadHintsDirty = true;
            }
        }

        /* Up or down to the next focusable row; wraps at the ends only when allowed */
        private void MoveMenuGamepadFocus(int dir, bool allowWrap)
        {
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int focus = MenuGamepadFocusIndex;
            int target = FindMenuGamepadFocusable(items, how, focus, dir);
            if (target < 0 && allowWrap && items != null)
                target = FindMenuGamepadFocusable(items, how, dir > 0 ? -1 : items.Count, dir);
            if (target >= 0 && target != focus)
                SetMenuGamepadFocus(target);
        }

        /* A page up or down: the furthest focusable row within one canvas height of the focused row */
        private void PageMenuGamepadFocus(int dir)
        {
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int focus = MenuGamepadFocusIndex;
            if (items == null || focus < 0)
                return;
            int target = focus;
            float height = GetMenuCanvasHeight();
            bool havePaint = false;
            lock (_menuUIDrawBoundsLock)
            {
                DrawBoundInfos bounds = MenuUIDrawBounds;
                int count = bounds != null && bounds.DrawBoundList != null
                    ? Math.Min(items.Count, bounds.DrawBoundList.Length) : 0;
                if (focus < count && height > 0)
                {
                    SKRect cur = bounds.DrawBoundList[focus].DrawBounds;
                    if (cur.Bottom > cur.Top)
                    {
                        havePaint = true;
                        float page = height - (cur.Bottom - cur.Top);
                        if (page <= 0)
                            page = height;
                        int i;
                        for (i = focus + dir; i >= 0 && i < count; i += dir)
                        {
                            if (!IsMenuItemGamepadFocusable(items, how, i))
                                continue;
                            SKRect r = bounds.DrawBoundList[i].DrawBounds;
                            if (r.Bottom <= r.Top)
                                break;
                            if (dir > 0 ? r.Top <= cur.Top + page : r.Top >= cur.Top - page)
                                target = i;
                            else
                                break;
                        }
                    }
                }
            }

            if (!havePaint)
            {
                int steps;
                for (steps = 0; steps < GamepadMenuPageFallbackRows; steps++)
                {
                    int next = FindMenuGamepadFocusable(items, how, target, dir);
                    if (next < 0)
                        break;
                    target = next;
                }
            }
            if (target == focus)
                target = FindMenuGamepadFocusable(items, how, focus, dir);
            if (target >= 0 && target != focus)
                SetMenuGamepadFocus(target);
        }

        private static bool MenuHasSectionHeaders(IList<GHMenuItem> items)
        {
            if (items == null)
                return false;
            foreach (GHMenuItem mi in items)
            {
                if (mi != null && IsMenuSectionHeader(mi))
                    return true;
            }
            return false;
        }

        /* The index of the last section header at or before idx, or -1 */
        private static int FindMenuSectionHeader(IList<GHMenuItem> items, int idx)
        {
            int i;
            for (i = Math.Min(idx, items.Count - 1); i >= 0; i--)
            {
                if (items[i] != null && IsMenuSectionHeader(items[i]))
                    return i;
            }
            return -1;
        }

        /* RB: the first focusable row of the next section; LB: the start of this section, or of the previous one */
        private void JumpMenuGamepadFocusSection(int dir)
        {
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int focus = MenuGamepadFocusIndex;
            if (items == null || focus < 0)
                return;
            int target = -1;
            if (dir > 0)
            {
                int i;
                for (i = focus + 1; i < items.Count; i++)
                {
                    if (items[i] != null && IsMenuSectionHeader(items[i]))
                    {
                        target = FindMenuGamepadFocusable(items, how, i - 1, 1);
                        break;
                    }
                }
            }
            else
            {
                int header = FindMenuSectionHeader(items, focus);
                int start = FindMenuGamepadFocusable(items, how, (header >= 0 ? header : 0) - 1, 1);
                if (start >= 0 && start < focus)
                {
                    target = start;
                }
                else if (header > 0)
                {
                    int previous = FindMenuSectionHeader(items, header - 1);
                    int previousStart = FindMenuGamepadFocusable(items, how, (previous >= 0 ? previous : 0) - 1, 1);
                    if (previousStart >= 0 && previousStart < focus)
                        target = previousStart;
                }
            }
            if (target >= 0 && target != focus)
                SetMenuGamepadFocus(target);
        }

        /* Hold A on a row: the count dialog or the menu's special action; false if neither applies */
        private bool OpenGamepadMenuItemCountOrSpecial(int focus)
        {
            if (focus < 0 || !MenuCanvas.AllowLongTap)
                return false;
            _menuCountNumber = -1;
            return OpenMenuItemCountOrSpecial(focus);
        }

        private void ActivateGamepadMenuItem(int focus, int token)
        {
            _menuCountNumber = -1;
            if (ActivateMenuItem(focus, false))
                MarkGamepadDialogAnswered(token);
        }

        private void HandleMenuGamepadButton(GHGamepadButton button, int token, bool isPress, bool isStep, bool isTap,
            bool isLong)
        {
            /* The equipment side has no gamepad navigation: Menu turns to the list side, B cancels the menu and the
               other buttons are swallowed */
            if (MenuEquipmentSideShown)
            {
                if (button == GHGamepadButton.Menu && isPress && IsMenuGamepadFlipAvailable())
                {
                    FlipMenuCanvas();
                    /* The focus resolves, or scrolls into view, once the list side has been painted */
                    Interlocked.Exchange(ref _menuGamepadFocusPending, 1);
                }
                else if (button == GHGamepadButton.B && isPress && MenuCancelButton.IsEnabled)
                {
                    MarkGamepadDialogAnswered(token);
                    PressMenuCancelButton();
                }
                return;
            }

            EnsureMenuGamepadFocusResolved();
            int focus = MenuGamepadFocusIndex;
            bool scrollMode = focus < 0;
            SelectionMode how = MenuCanvas.SelectionHow;

            bool isDPad;
            int step = GetGamepadDialogVerticalStep(button, out isDPad);
            if (step != 0)
            {
                if (isStep)
                {
                    if (scrollMode)
                        ScrollMenu(-step * GamepadMenuStepDelta);
                    else
                        MoveMenuGamepadFocus(step, isDPad && isPress);
                }
                return;
            }

            switch (button)
            {
                case GHGamepadButton.DPadLeft:
                case GHGamepadButton.DPadRight:
                    if (isStep)
                    {
                        int dir = button == GHGamepadButton.DPadLeft ? -1 : 1;
                        if (scrollMode)
                            ScrollMenu(-dir * GamepadMenuPageDelta);
                        else
                            PageMenuGamepadFocus(dir);
                    }
                    break;
                case GHGamepadButton.LB:
                case GHGamepadButton.RB:
                    if (isStep)
                    {
                        int dir = button == GHGamepadButton.LB ? -1 : 1;
                        if (scrollMode)
                            ScrollMenu(-dir * GamepadMenuPageDelta);
                        else if (MenuHasSectionHeaders(MenuCanvas.MenuItems))
                            JumpMenuGamepadFocusSection(dir);
                        else
                            PageMenuGamepadFocus(dir);
                    }
                    break;
                case GHGamepadButton.LT:
                case GHGamepadButton.RT:
                    if (isPress)
                    {
                        if (scrollMode)
                        {
                            ScrollMenu(button == GHGamepadButton.LT ? GamepadMenuEndDelta : -GamepadMenuEndDelta);
                        }
                        else
                        {
                            var items = MenuCanvas.MenuItems;
                            int target = button == GHGamepadButton.LT ? FindMenuGamepadFocusable(items, how, -1, 1)
                                : FindMenuGamepadFocusable(items, how, items != null ? items.Count : 0, -1);
                            if (target >= 0 && target != focus)
                                SetMenuGamepadFocus(target);
                        }
                    }
                    break;
                case GHGamepadButton.A:
                    if (isTap || isLong)
                    {
                        if (scrollMode)
                        {
                            if (MenuOKButton.IsEnabled)
                            {
                                MarkGamepadDialogAnswered(token);
                                PressMenuOKButton();
                            }
                        }
                        else if (!isLong || !OpenGamepadMenuItemCountOrSpecial(focus))
                        {
                            /* A long press without a count or special action selects as a tap does */
                            ActivateGamepadMenuItem(focus, token);
                        }
                    }
                    break;
                case GHGamepadButton.B:
                    if (isPress && MenuCancelButton.IsEnabled)
                    {
                        MarkGamepadDialogAnswered(token);
                        PressMenuCancelButton();
                    }
                    break;
                case GHGamepadButton.X:
                    if (isPress && MenuOKButton.IsEnabled)
                    {
                        MarkGamepadDialogAnswered(token);
                        PressMenuOKButton();
                    }
                    break;
                case GHGamepadButton.Y:
                    /* Select all or unselect all, as the '.' key; a row's count and actions are on hold A */
                    if (isPress && how == SelectionMode.Multiple)
                        MenuTapGestureRecognizer_Tapped(null, null);
                    break;
                case GHGamepadButton.View:
                    /* View closes an inventory menu */
                    if (isPress && IsInventoryMenuStyle(MenuCanvas.MenuStyle) && MenuCancelButton.IsEnabled)
                    {
                        MarkGamepadDialogAnswered(token);
                        PressMenuCancelButton();
                    }
                    break;
                case GHGamepadButton.Menu:
                    /* Menu turns to the equipment side, as the flip button does */
                    if (isPress && IsMenuGamepadFlipAvailable())
                        FlipMenuCanvas();
                    break;
                default:
                    break;
            }
        }

        /* The menu's flip button is shown and not in the middle of a flip */
        private bool IsMenuGamepadFlipAvailable()
        {
            return MenuFlipButton.IsVisible && MenuFlipButton.IsEnabled;
        }

        /* Hints */

        /* Changes in what the hints show mark the hint strip for a rebuild */
        private void UpdateGamepadDialogHintSignature(GHGamepadDialogContext ctx)
        {
            int signature = (int)ctx;
            switch (ctx)
            {
                case GHGamepadDialogContext.Menu:
                    signature = signature * 31 + MenuGamepadFocusIndex;
                    signature = signature * 31 + (MenuOKButton.IsEnabled ? 1 : 0) + (_unselectOnTap ? 2 : 0)
                        + (MenuEquipmentSideShown ? 4 : 0) + (MenuFlipButton.IsVisible ? 8 : 0);
                    break;
                case GHGamepadDialogContext.Yn:
                    signature = signature * 31 + (FindYnGamepadAnswerButton('y') != null ? 1 : 0)
                        + (FindYnGamepadAnswerButton('n') != null ? 2 : 0);
                    break;
                case GHGamepadDialogContext.GetLine:
                    signature = signature * 31 + (GetLineQuestionMarkButton.IsVisible ? 1 : 0)
                        + (GetLineCancelButton.Text == "Random" ? 2 : 0) + (GHScreenKeyboard.IsAvailable ? 4 : 0);
                    break;
                default:
                    break;
            }
            if (signature != _gamepadDialogHintSignature)
            {
                _gamepadDialogHintSignature = signature;
                _gamepadHintsDirty = true;
            }
        }

        internal void GetDialogGamepadHints(List<GHGamepadHint> hints)
        {
            if (hints == null)
                return;
            switch (GetGamepadDialogContext())
            {
                case GHGamepadDialogContext.Tips:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Next", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Skip", false));
                    break;
                case GHGamepadDialogContext.MoreCommandsTouch:
                    hints.Add(new GHGamepadHint(GHGamepadButton.LB, "Previous page", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.RB, "Next page", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Close", false));
                    break;
                case GHGamepadDialogContext.Popup:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "OK", false));
                    break;
                case GHGamepadDialogContext.Yn:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Select", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                    if (FindYnGamepadAnswerButton('y') != null)
                        hints.Add(new GHGamepadHint(GHGamepadButton.Y, "Yes", false));
                    if (FindYnGamepadAnswerButton('n') != null)
                        hints.Add(new GHGamepadHint(GHGamepadButton.X, "No", false));
                    break;
                case GHGamepadDialogContext.GetLine:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "OK", false));
                    string cancelLabel = GetLineCancelButton.Text == "Random" ? "Random" : "Cancel";
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, cancelLabel, false));
                    if (GetLineQuestionMarkButton.IsVisible)
                        hints.Add(new GHGamepadHint(GHGamepadButton.X, "List", false));
                    if (GHScreenKeyboard.IsAvailable)
                        hints.Add(new GHGamepadHint(GHGamepadButton.Y, "Keyboard", false));
                    break;
                case GHGamepadDialogContext.Text:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Close", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Close", false));
                    break;
                case GHGamepadDialogContext.MenuCount:
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "OK", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.Y, "All", false));
                    hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                    break;
                case GHGamepadDialogContext.Menu:
                    AddMenuGamepadHints(hints);
                    break;
                default:
                    break;
            }
        }

        private void AddMenuGamepadHints(List<GHGamepadHint> hints)
        {
            /* The flip button's visibility, not its state, so that the hints do not change during a flip */
            bool canFlip = MenuFlipButton.IsVisible;
            if (MenuEquipmentSideShown)
            {
                if (canFlip)
                    hints.Add(new GHGamepadHint(GHGamepadButton.Menu, "Inventory", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                return;
            }
            var items = MenuCanvas.MenuItems;
            SelectionMode how = MenuCanvas.SelectionHow;
            int focus = MenuGamepadFocusIndex;
            bool hasFocusable = FindMenuGamepadFocusable(items, how, -1, 1) >= 0;
            if (!hasFocusable)
            {
                if (MenuOKButton.IsEnabled)
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "OK", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.B, "Close", false));
                if (canFlip)
                    hints.Add(new GHGamepadHint(GHGamepadButton.Menu, "Equipment", false));
                return;
            }

            GHMenuItem focused = items != null && focus >= 0 && focus < items.Count ? items[focus] : null;
            bool canHold = focused != null && focused.Identifier != 0 && MenuCanvas.AllowLongTap;
            bool canCount = canHold && focused.MaxCount > 1;
            bool canSpecial = canHold && !canCount && MenuCanvas.SpecialClickOnLongTap;
            if (how == SelectionMode.Multiple)
            {
                hints.Add(new GHGamepadHint(GHGamepadButton.A, "Toggle", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.X, "OK", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.Y, _unselectOnTap ? "None" : "All", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                if (canCount)
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Count", true));
                else if (canSpecial)
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Actions", true));
            }
            else
            {
                hints.Add(new GHGamepadHint(GHGamepadButton.A, "Select", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.B, "Cancel", false));
                if (canCount)
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Count", true));
                else if (canSpecial)
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Actions", true));
                if (MenuOKButton.IsEnabled)
                    hints.Add(new GHGamepadHint(GHGamepadButton.X, "OK", false));
                if (MenuHasSectionHeaders(items))
                    hints.Add(new GHGamepadHint(GHGamepadButton.RB, "Section", false));
            }
            if (canFlip)
                hints.Add(new GHGamepadHint(GHGamepadButton.Menu, "Equipment", false));
        }
    }
}

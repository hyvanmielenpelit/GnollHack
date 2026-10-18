#if GNH_AVALONIA
using System;
using Avalonia.Input;
using Avalonia.Interactivity;
using GnollHackX;

namespace GnollHackM
{
    /* Routes the window's keys to the game, as the WinUI build's low-level keyboard hook does. Tunnel handlers
       on the window see every key before Avalonia's focus navigation (Tab, arrows), access keys (Alt) and back
       navigation (Alt+Left). Special keys and Ctrl or Alt combinations go to the game on KeyDown; characters
       come from TextInput, which follows the keyboard layout, dead keys and AltGr. Avalonia raises no
       TextInput for a KeyDown marked handled, and a key the game declines continues to the focused control.
       UI thread only. */
    internal static class GHAvaloniaKeyboard
    {
        /* The current keystroke was offered to the game on KeyDown, so its TextInput is not */
        private static bool _keyDownRouted = false;

        public static void Attach(Avalonia.Controls.Window window)
        {
            if (window == null)
                return;
            window.AddHandler(InputElement.KeyDownEvent, OnKeyDown, RoutingStrategies.Tunnel, true);
            window.AddHandler(InputElement.KeyUpEvent, OnKeyUp, RoutingStrategies.Tunnel, true);
            window.AddHandler(InputElement.TextInputEvent, OnTextInput, RoutingStrategies.Tunnel, true);
            window.Deactivated += OnDeactivated;
        }

        public static void Detach(Avalonia.Controls.Window window)
        {
            if (window == null)
                return;
            window.RemoveHandler(InputElement.KeyDownEvent, OnKeyDown);
            window.RemoveHandler(InputElement.KeyUpEvent, OnKeyUp);
            window.RemoveHandler(InputElement.TextInputEvent, OnTextInput);
            window.Deactivated -= OnDeactivated;
        }

        private static void UpdateModifiers(KeyModifiers modifiers)
        {
            GHApp.ShiftDown = (modifiers & KeyModifiers.Shift) != 0;
            GHApp.CtrlDown = (modifiers & KeyModifiers.Control) != 0;
            GHApp.AltDown = (modifiers & KeyModifiers.Alt) != 0;
        }

        private static void OnDeactivated(object sender, EventArgs e)
        {
            GHApp.ShiftDown = false;
            GHApp.CtrlDown = false;
            GHApp.AltDown = false;
        }

        private static void OnKeyUp(object sender, KeyEventArgs e)
        {
            UpdateModifiers(e.KeyModifiers);
        }

        private static void OnKeyDown(object sender, KeyEventArgs e)
        {
            UpdateModifiers(e.KeyModifiers);
            _keyDownRouted = false;

            if (!GHApp.IsKeyboardHookEnabled || IsModifierKey(e.Key))
                return;

            KeyModifiers modifiers = e.KeyModifiers;
            if ((modifiers & KeyModifiers.Meta) != 0) /* Windows and Super key combinations stay with the system */
                return;

            bool ctrl = (modifiers & KeyModifiers.Control) != 0;
            bool alt = (modifiers & KeyModifiers.Alt) != 0;
            bool shift = (modifiers & KeyModifiers.Shift) != 0;
            if (ctrl && alt) /* AltGr: the character comes through TextInput */
                return;

            bool handled;
            GHSpecialKey spkey;
            if (alt)
            {
                if (e.Key == Key.Tab || e.Key == Key.F4) /* Alt+Tab and Alt+F4 stay with the system */
                    return;

                spkey = GetAltSpecialKey(e.Key);
                if (spkey != GHSpecialKey.None)
                {
                    handled = GHApp.SendSpecialKeyPress(spkey, false, true, shift);
                }
                else
                {
                    string symbol = e.KeySymbol;
                    if (string.IsNullOrEmpty(symbol) || symbol.Length != 1)
                        return;
                    handled = GHApp.SendKeyPress(symbol[0], false, true);
                }
                _keyDownRouted = true;
                e.Handled = handled;
                return;
            }

            spkey = GetSpecialKey(e.Key);
            if (spkey != GHSpecialKey.None)
            {
                handled = GHApp.SendSpecialKeyPress(spkey, ctrl, false, shift);
                _keyDownRouted = true;
                e.Handled = handled;
                return;
            }

            if (ctrl && e.Key >= Key.A && e.Key <= Key.Z)
            {
                /* The control character, as the hook gets from ToUnicodeEx: menus match accelerators on the raw key */
                handled = GHApp.SendKeyPress(1 + (e.Key - Key.A), true, false);
                _keyDownRouted = true;
                e.Handled = handled;
            }
        }

        private static void OnTextInput(object sender, TextInputEventArgs e)
        {
            if (_keyDownRouted || !GHApp.IsKeyboardHookEnabled)
                return;

            string text = e.Text;
            if (string.IsNullOrEmpty(text))
                return;

            bool handled = false;
            int i;
            for (i = 0; i < text.Length; i++)
            {
                if (GHApp.SendKeyPress(text[i], false, false))
                    handled = true;
            }
            if (handled)
                e.Handled = true;
        }

        private static bool IsModifierKey(Key key)
        {
            switch (key)
            {
            case Key.LeftShift:
            case Key.RightShift:
            case Key.LeftCtrl:
            case Key.RightCtrl:
            case Key.LeftAlt:
            case Key.RightAlt:
            case Key.LWin:
            case Key.RWin:
                return true;
            default:
                return false;
            }
        }

        /* The WinUI hook's WM_KEYDOWN table */
        private static GHSpecialKey GetSpecialKey(Key key)
        {
            switch (key)
            {
            case Key.Space:
                return GHSpecialKey.Space;
            case Key.PageUp:
                return GHSpecialKey.PageUp;
            case Key.PageDown:
                return GHSpecialKey.PageDown;
            case Key.End:
                return GHSpecialKey.End;
            case Key.Home:
                return GHSpecialKey.Home;
            case Key.Left:
                return GHSpecialKey.Left;
            case Key.Up:
                return GHSpecialKey.Up;
            case Key.Right:
                return GHSpecialKey.Right;
            case Key.Down:
                return GHSpecialKey.Down;
            case Key.Escape:
                return GHSpecialKey.Escape;
            case Key.Enter:
                return GHSpecialKey.Enter;
            case Key.NumPad0:
                return GHSpecialKey.NumberPad0;
            case Key.Add:
                return GHSpecialKey.Add;
            case Key.Subtract:
                return GHSpecialKey.Subtract;
            case Key.Decimal:
                return GHSpecialKey.Decimal;
            case Key.Tab:
                return GHSpecialKey.Tab;
            case Key.F1:
                return GHSpecialKey.F1;
            case Key.F8:
                return GHSpecialKey.F8;
            case Key.F10:
                return GHSpecialKey.F10;
            default:
                return GHSpecialKey.None;
            }
        }

        /* The WinUI hook's WM_SYSKEYDOWN table */
        private static GHSpecialKey GetAltSpecialKey(Key key)
        {
            if (key >= Key.A && key <= Key.Z)
                return GHSpecialKey.A + (key - Key.A);
            if (key >= Key.D0 && key <= Key.D9)
                return GHSpecialKey.Number0 + (key - Key.D0);
            if (key >= Key.NumPad0 && key <= Key.NumPad9)
                return GHSpecialKey.NumberPad0 + (key - Key.NumPad0);

            switch (key)
            {
            case Key.Left:
                return GHSpecialKey.Left;
            case Key.Up:
                return GHSpecialKey.Up;
            case Key.Right:
                return GHSpecialKey.Right;
            case Key.Down:
                return GHSpecialKey.Down;
            case Key.OemPlus:
                return GHSpecialKey.Plus;
            case Key.OemComma:
                return GHSpecialKey.Comma;
            case Key.OemMinus:
                return GHSpecialKey.Minus;
            case Key.OemPeriod:
                return GHSpecialKey.Period;
            default:
                return GHSpecialKey.None;
            }
        }
    }
}
#endif

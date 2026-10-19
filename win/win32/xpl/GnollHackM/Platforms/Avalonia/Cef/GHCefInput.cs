#if GNH_AVALONIA && LINUX
using System;
using Xilium.CefGlue;

namespace GnollHackM
{
    /* Avalonia keyboard and pointer state as CEF expects it on Linux: a Windows virtual-key code, the X11 keycode
       (evdev + 8) and CEF event flags */
    internal static class GHCefInput
    {
        /* Windows virtual-key code of an Avalonia key; Avalonia's Key uses WPF's values. The left and right
           modifier keys give the generic VK_SHIFT, VK_CONTROL and VK_MENU, as Chromium's own X11 conversion does;
           the side is in the IsLeft and IsRight flags. Returns 0 for a key without a code. */
        public static int WindowsKeyCode(Avalonia.Input.Key key)
        {
            if (key >= Avalonia.Input.Key.D0 && key <= Avalonia.Input.Key.D9)
                return 0x30 + (key - Avalonia.Input.Key.D0);
            if (key >= Avalonia.Input.Key.A && key <= Avalonia.Input.Key.Z)
                return 0x41 + (key - Avalonia.Input.Key.A);
            if (key >= Avalonia.Input.Key.NumPad0 && key <= Avalonia.Input.Key.NumPad9)
                return 0x60 + (key - Avalonia.Input.Key.NumPad0);
            if (key >= Avalonia.Input.Key.F1 && key <= Avalonia.Input.Key.F24)
                return 0x70 + (key - Avalonia.Input.Key.F1);

            switch (key)
            {
            case Avalonia.Input.Key.Cancel: return 0x03;
            case Avalonia.Input.Key.Back: return 0x08;
            case Avalonia.Input.Key.Tab: return 0x09;
            case Avalonia.Input.Key.Clear: return 0x0C;
            case Avalonia.Input.Key.Return: return 0x0D;
            case Avalonia.Input.Key.Pause: return 0x13;
            case Avalonia.Input.Key.CapsLock: return 0x14;
            case Avalonia.Input.Key.KanaMode: return 0x15;
            case Avalonia.Input.Key.JunjaMode: return 0x17;
            case Avalonia.Input.Key.FinalMode: return 0x18;
            case Avalonia.Input.Key.KanjiMode: return 0x19;
            case Avalonia.Input.Key.Escape: return 0x1B;
            case Avalonia.Input.Key.ImeConvert: return 0x1C;
            case Avalonia.Input.Key.ImeNonConvert: return 0x1D;
            case Avalonia.Input.Key.ImeAccept: return 0x1E;
            case Avalonia.Input.Key.ImeModeChange: return 0x1F;
            case Avalonia.Input.Key.Space: return 0x20;
            case Avalonia.Input.Key.PageUp: return 0x21;
            case Avalonia.Input.Key.PageDown: return 0x22;
            case Avalonia.Input.Key.End: return 0x23;
            case Avalonia.Input.Key.Home: return 0x24;
            case Avalonia.Input.Key.Left: return 0x25;
            case Avalonia.Input.Key.Up: return 0x26;
            case Avalonia.Input.Key.Right: return 0x27;
            case Avalonia.Input.Key.Down: return 0x28;
            case Avalonia.Input.Key.Select: return 0x29;
            case Avalonia.Input.Key.Print: return 0x2A;
            case Avalonia.Input.Key.Execute: return 0x2B;
            case Avalonia.Input.Key.PrintScreen: return 0x2C;
            case Avalonia.Input.Key.Insert: return 0x2D;
            case Avalonia.Input.Key.Delete: return 0x2E;
            case Avalonia.Input.Key.Help: return 0x2F;
            case Avalonia.Input.Key.LWin: return 0x5B;
            case Avalonia.Input.Key.RWin: return 0x5C;
            case Avalonia.Input.Key.Apps: return 0x5D;
            case Avalonia.Input.Key.Sleep: return 0x5F;
            case Avalonia.Input.Key.Multiply: return 0x6A;
            case Avalonia.Input.Key.Add: return 0x6B;
            case Avalonia.Input.Key.Separator: return 0x6C;
            case Avalonia.Input.Key.Subtract: return 0x6D;
            case Avalonia.Input.Key.Decimal: return 0x6E;
            case Avalonia.Input.Key.Divide: return 0x6F;
            case Avalonia.Input.Key.NumLock: return 0x90;
            case Avalonia.Input.Key.Scroll: return 0x91;
            case Avalonia.Input.Key.LeftShift: return 0x10;
            case Avalonia.Input.Key.RightShift: return 0x10;
            case Avalonia.Input.Key.LeftCtrl: return 0x11;
            case Avalonia.Input.Key.RightCtrl: return 0x11;
            case Avalonia.Input.Key.LeftAlt: return 0x12;
            case Avalonia.Input.Key.RightAlt: return 0x12;
            case Avalonia.Input.Key.System: return 0x12;
            case Avalonia.Input.Key.BrowserBack: return 0xA6;
            case Avalonia.Input.Key.BrowserForward: return 0xA7;
            case Avalonia.Input.Key.BrowserRefresh: return 0xA8;
            case Avalonia.Input.Key.BrowserStop: return 0xA9;
            case Avalonia.Input.Key.BrowserSearch: return 0xAA;
            case Avalonia.Input.Key.BrowserFavorites: return 0xAB;
            case Avalonia.Input.Key.BrowserHome: return 0xAC;
            case Avalonia.Input.Key.VolumeMute: return 0xAD;
            case Avalonia.Input.Key.VolumeDown: return 0xAE;
            case Avalonia.Input.Key.VolumeUp: return 0xAF;
            case Avalonia.Input.Key.MediaNextTrack: return 0xB0;
            case Avalonia.Input.Key.MediaPreviousTrack: return 0xB1;
            case Avalonia.Input.Key.MediaStop: return 0xB2;
            case Avalonia.Input.Key.MediaPlayPause: return 0xB3;
            case Avalonia.Input.Key.LaunchMail: return 0xB4;
            case Avalonia.Input.Key.SelectMedia: return 0xB5;
            case Avalonia.Input.Key.LaunchApplication1: return 0xB6;
            case Avalonia.Input.Key.LaunchApplication2: return 0xB7;
            case Avalonia.Input.Key.OemSemicolon: return 0xBA;
            case Avalonia.Input.Key.OemPlus: return 0xBB;
            case Avalonia.Input.Key.OemComma: return 0xBC;
            case Avalonia.Input.Key.OemMinus: return 0xBD;
            case Avalonia.Input.Key.OemPeriod: return 0xBE;
            case Avalonia.Input.Key.OemQuestion: return 0xBF;
            case Avalonia.Input.Key.OemTilde: return 0xC0;
            case Avalonia.Input.Key.AbntC1: return 0xC1;
            case Avalonia.Input.Key.AbntC2: return 0xC2;
            case Avalonia.Input.Key.OemOpenBrackets: return 0xDB;
            case Avalonia.Input.Key.OemPipe: return 0xDC;
            case Avalonia.Input.Key.OemCloseBrackets: return 0xDD;
            case Avalonia.Input.Key.OemQuotes: return 0xDE;
            case Avalonia.Input.Key.Oem8: return 0xDF;
            case Avalonia.Input.Key.OemBackslash: return 0xE2;
            case Avalonia.Input.Key.ImeProcessed: return 0xE5;
            case Avalonia.Input.Key.OemAttn: return 0xF0;
            case Avalonia.Input.Key.OemFinish: return 0xF1;
            case Avalonia.Input.Key.OemCopy: return 0xF2;
            case Avalonia.Input.Key.OemAuto: return 0xF3;
            case Avalonia.Input.Key.OemEnlw: return 0xF4;
            case Avalonia.Input.Key.OemBackTab: return 0xF5;
            case Avalonia.Input.Key.Attn: return 0xF6;
            case Avalonia.Input.Key.CrSel: return 0xF7;
            case Avalonia.Input.Key.ExSel: return 0xF8;
            case Avalonia.Input.Key.EraseEof: return 0xF9;
            case Avalonia.Input.Key.Play: return 0xFA;
            case Avalonia.Input.Key.Zoom: return 0xFB;
            case Avalonia.Input.Key.NoName: return 0xFC;
            case Avalonia.Input.Key.Pa1: return 0xFD;
            case Avalonia.Input.Key.OemClear: return 0xFE;
            default: return 0;
            }
        }

        /* X11 keycode (evdev + 8) of a physical key: the inverse of Avalonia.X11's scan-code table. Returns 0 for
           a key without a code. */
        public static int X11KeyCode(Avalonia.Input.PhysicalKey key)
        {
            switch (key)
            {
            case Avalonia.Input.PhysicalKey.Backquote: return 49;
            case Avalonia.Input.PhysicalKey.Backslash: return 51;
            case Avalonia.Input.PhysicalKey.BracketLeft: return 34;
            case Avalonia.Input.PhysicalKey.BracketRight: return 35;
            case Avalonia.Input.PhysicalKey.Comma: return 59;
            case Avalonia.Input.PhysicalKey.Digit0: return 19;
            case Avalonia.Input.PhysicalKey.Digit1: return 10;
            case Avalonia.Input.PhysicalKey.Digit2: return 11;
            case Avalonia.Input.PhysicalKey.Digit3: return 12;
            case Avalonia.Input.PhysicalKey.Digit4: return 13;
            case Avalonia.Input.PhysicalKey.Digit5: return 14;
            case Avalonia.Input.PhysicalKey.Digit6: return 15;
            case Avalonia.Input.PhysicalKey.Digit7: return 16;
            case Avalonia.Input.PhysicalKey.Digit8: return 17;
            case Avalonia.Input.PhysicalKey.Digit9: return 18;
            case Avalonia.Input.PhysicalKey.Equal: return 21;
            case Avalonia.Input.PhysicalKey.IntlBackslash: return 94;
            case Avalonia.Input.PhysicalKey.IntlRo: return 97;
            case Avalonia.Input.PhysicalKey.IntlYen: return 132;
            case Avalonia.Input.PhysicalKey.A: return 38;
            case Avalonia.Input.PhysicalKey.B: return 56;
            case Avalonia.Input.PhysicalKey.C: return 54;
            case Avalonia.Input.PhysicalKey.D: return 40;
            case Avalonia.Input.PhysicalKey.E: return 26;
            case Avalonia.Input.PhysicalKey.F: return 41;
            case Avalonia.Input.PhysicalKey.G: return 42;
            case Avalonia.Input.PhysicalKey.H: return 43;
            case Avalonia.Input.PhysicalKey.I: return 31;
            case Avalonia.Input.PhysicalKey.J: return 44;
            case Avalonia.Input.PhysicalKey.K: return 45;
            case Avalonia.Input.PhysicalKey.L: return 46;
            case Avalonia.Input.PhysicalKey.M: return 58;
            case Avalonia.Input.PhysicalKey.N: return 57;
            case Avalonia.Input.PhysicalKey.O: return 32;
            case Avalonia.Input.PhysicalKey.P: return 33;
            case Avalonia.Input.PhysicalKey.Q: return 24;
            case Avalonia.Input.PhysicalKey.R: return 27;
            case Avalonia.Input.PhysicalKey.S: return 39;
            case Avalonia.Input.PhysicalKey.T: return 28;
            case Avalonia.Input.PhysicalKey.U: return 30;
            case Avalonia.Input.PhysicalKey.V: return 55;
            case Avalonia.Input.PhysicalKey.W: return 25;
            case Avalonia.Input.PhysicalKey.X: return 53;
            case Avalonia.Input.PhysicalKey.Y: return 29;
            case Avalonia.Input.PhysicalKey.Z: return 52;
            case Avalonia.Input.PhysicalKey.Minus: return 20;
            case Avalonia.Input.PhysicalKey.Period: return 60;
            case Avalonia.Input.PhysicalKey.Quote: return 48;
            case Avalonia.Input.PhysicalKey.Semicolon: return 47;
            case Avalonia.Input.PhysicalKey.Slash: return 61;
            case Avalonia.Input.PhysicalKey.AltLeft: return 64;
            case Avalonia.Input.PhysicalKey.AltRight: return 108;
            case Avalonia.Input.PhysicalKey.Backspace: return 22;
            case Avalonia.Input.PhysicalKey.CapsLock: return 66;
            case Avalonia.Input.PhysicalKey.ContextMenu: return 135;
            case Avalonia.Input.PhysicalKey.ControlLeft: return 37;
            case Avalonia.Input.PhysicalKey.ControlRight: return 105;
            case Avalonia.Input.PhysicalKey.Enter: return 36;
            case Avalonia.Input.PhysicalKey.MetaLeft: return 133;
            case Avalonia.Input.PhysicalKey.MetaRight: return 134;
            case Avalonia.Input.PhysicalKey.ShiftLeft: return 50;
            case Avalonia.Input.PhysicalKey.ShiftRight: return 62;
            case Avalonia.Input.PhysicalKey.Space: return 65;
            case Avalonia.Input.PhysicalKey.Tab: return 23;
            case Avalonia.Input.PhysicalKey.Convert: return 100;
            case Avalonia.Input.PhysicalKey.KanaMode: return 101;
            case Avalonia.Input.PhysicalKey.Lang1: return 130;
            case Avalonia.Input.PhysicalKey.Lang2: return 131;
            case Avalonia.Input.PhysicalKey.Lang3: return 98;
            case Avalonia.Input.PhysicalKey.Lang4: return 99;
            case Avalonia.Input.PhysicalKey.Lang5: return 93;
            case Avalonia.Input.PhysicalKey.NonConvert: return 102;
            case Avalonia.Input.PhysicalKey.Delete: return 119;
            case Avalonia.Input.PhysicalKey.End: return 115;
            case Avalonia.Input.PhysicalKey.Help: return 146;
            case Avalonia.Input.PhysicalKey.Home: return 110;
            case Avalonia.Input.PhysicalKey.Insert: return 118;
            case Avalonia.Input.PhysicalKey.PageDown: return 117;
            case Avalonia.Input.PhysicalKey.PageUp: return 112;
            case Avalonia.Input.PhysicalKey.ArrowDown: return 116;
            case Avalonia.Input.PhysicalKey.ArrowLeft: return 113;
            case Avalonia.Input.PhysicalKey.ArrowRight: return 114;
            case Avalonia.Input.PhysicalKey.ArrowUp: return 111;
            case Avalonia.Input.PhysicalKey.NumLock: return 77;
            case Avalonia.Input.PhysicalKey.NumPad0: return 90;
            case Avalonia.Input.PhysicalKey.NumPad1: return 87;
            case Avalonia.Input.PhysicalKey.NumPad2: return 88;
            case Avalonia.Input.PhysicalKey.NumPad3: return 89;
            case Avalonia.Input.PhysicalKey.NumPad4: return 83;
            case Avalonia.Input.PhysicalKey.NumPad5: return 84;
            case Avalonia.Input.PhysicalKey.NumPad6: return 85;
            case Avalonia.Input.PhysicalKey.NumPad7: return 79;
            case Avalonia.Input.PhysicalKey.NumPad8: return 80;
            case Avalonia.Input.PhysicalKey.NumPad9: return 81;
            case Avalonia.Input.PhysicalKey.NumPadAdd: return 86;
            case Avalonia.Input.PhysicalKey.NumPadComma: return 129;
            case Avalonia.Input.PhysicalKey.NumPadDecimal: return 91;
            case Avalonia.Input.PhysicalKey.NumPadDivide: return 106;
            case Avalonia.Input.PhysicalKey.NumPadEnter: return 104;
            case Avalonia.Input.PhysicalKey.NumPadEqual: return 125;
            case Avalonia.Input.PhysicalKey.NumPadMultiply: return 63;
            case Avalonia.Input.PhysicalKey.NumPadParenLeft: return 187;
            case Avalonia.Input.PhysicalKey.NumPadParenRight: return 188;
            case Avalonia.Input.PhysicalKey.NumPadSubtract: return 82;
            case Avalonia.Input.PhysicalKey.Escape: return 9;
            case Avalonia.Input.PhysicalKey.F1: return 67;
            case Avalonia.Input.PhysicalKey.F2: return 68;
            case Avalonia.Input.PhysicalKey.F3: return 69;
            case Avalonia.Input.PhysicalKey.F4: return 70;
            case Avalonia.Input.PhysicalKey.F5: return 71;
            case Avalonia.Input.PhysicalKey.F6: return 72;
            case Avalonia.Input.PhysicalKey.F7: return 73;
            case Avalonia.Input.PhysicalKey.F8: return 74;
            case Avalonia.Input.PhysicalKey.F9: return 75;
            case Avalonia.Input.PhysicalKey.F10: return 76;
            case Avalonia.Input.PhysicalKey.F11: return 95;
            case Avalonia.Input.PhysicalKey.F12: return 96;
            case Avalonia.Input.PhysicalKey.F13: return 191;
            case Avalonia.Input.PhysicalKey.F14: return 192;
            case Avalonia.Input.PhysicalKey.F15: return 193;
            case Avalonia.Input.PhysicalKey.F16: return 194;
            case Avalonia.Input.PhysicalKey.F17: return 195;
            case Avalonia.Input.PhysicalKey.F18: return 196;
            case Avalonia.Input.PhysicalKey.F19: return 197;
            case Avalonia.Input.PhysicalKey.F20: return 198;
            case Avalonia.Input.PhysicalKey.F21: return 199;
            case Avalonia.Input.PhysicalKey.F22: return 200;
            case Avalonia.Input.PhysicalKey.F23: return 201;
            case Avalonia.Input.PhysicalKey.F24: return 202;
            case Avalonia.Input.PhysicalKey.PrintScreen: return 107;
            case Avalonia.Input.PhysicalKey.ScrollLock: return 78;
            case Avalonia.Input.PhysicalKey.Pause: return 127;
            case Avalonia.Input.PhysicalKey.BrowserBack: return 166;
            case Avalonia.Input.PhysicalKey.BrowserFavorites: return 164;
            case Avalonia.Input.PhysicalKey.BrowserForward: return 167;
            case Avalonia.Input.PhysicalKey.BrowserHome: return 180;
            case Avalonia.Input.PhysicalKey.BrowserRefresh: return 181;
            case Avalonia.Input.PhysicalKey.BrowserSearch: return 225;
            case Avalonia.Input.PhysicalKey.BrowserStop: return 136;
            case Avalonia.Input.PhysicalKey.Eject: return 169;
            case Avalonia.Input.PhysicalKey.LaunchApp1: return 152;
            case Avalonia.Input.PhysicalKey.LaunchApp2: return 148;
            case Avalonia.Input.PhysicalKey.LaunchMail: return 163;
            case Avalonia.Input.PhysicalKey.MediaPlayPause: return 172;
            case Avalonia.Input.PhysicalKey.MediaSelect: return 179;
            case Avalonia.Input.PhysicalKey.MediaStop: return 174;
            case Avalonia.Input.PhysicalKey.MediaTrackNext: return 171;
            case Avalonia.Input.PhysicalKey.MediaTrackPrevious: return 173;
            case Avalonia.Input.PhysicalKey.Power: return 124;
            case Avalonia.Input.PhysicalKey.Sleep: return 150;
            case Avalonia.Input.PhysicalKey.AudioVolumeDown: return 122;
            case Avalonia.Input.PhysicalKey.AudioVolumeMute: return 121;
            case Avalonia.Input.PhysicalKey.AudioVolumeUp: return 123;
            case Avalonia.Input.PhysicalKey.WakeUp: return 151;
            case Avalonia.Input.PhysicalKey.Again: return 137;
            case Avalonia.Input.PhysicalKey.Copy: return 141;
            case Avalonia.Input.PhysicalKey.Cut: return 145;
            case Avalonia.Input.PhysicalKey.Find: return 144;
            case Avalonia.Input.PhysicalKey.Open: return 142;
            case Avalonia.Input.PhysicalKey.Paste: return 143;
            case Avalonia.Input.PhysicalKey.Select: return 140;
            case Avalonia.Input.PhysicalKey.Undo: return 139;
            default: return 0;
            }
        }

        /* Keypad and left or right side flags of a physical key */
        public static CefEventFlags KeyLocationFlags(Avalonia.Input.PhysicalKey key)
        {
            switch (key)
            {
            case Avalonia.Input.PhysicalKey.ShiftLeft:
            case Avalonia.Input.PhysicalKey.ControlLeft:
            case Avalonia.Input.PhysicalKey.AltLeft:
            case Avalonia.Input.PhysicalKey.MetaLeft:
                return CefEventFlags.IsLeft;
            case Avalonia.Input.PhysicalKey.ShiftRight:
            case Avalonia.Input.PhysicalKey.ControlRight:
            case Avalonia.Input.PhysicalKey.AltRight:
            case Avalonia.Input.PhysicalKey.MetaRight:
                return CefEventFlags.IsRight;
            case Avalonia.Input.PhysicalKey.NumPad0:
            case Avalonia.Input.PhysicalKey.NumPad1:
            case Avalonia.Input.PhysicalKey.NumPad2:
            case Avalonia.Input.PhysicalKey.NumPad3:
            case Avalonia.Input.PhysicalKey.NumPad4:
            case Avalonia.Input.PhysicalKey.NumPad5:
            case Avalonia.Input.PhysicalKey.NumPad6:
            case Avalonia.Input.PhysicalKey.NumPad7:
            case Avalonia.Input.PhysicalKey.NumPad8:
            case Avalonia.Input.PhysicalKey.NumPad9:
            case Avalonia.Input.PhysicalKey.NumPadAdd:
            case Avalonia.Input.PhysicalKey.NumPadClear:
            case Avalonia.Input.PhysicalKey.NumPadComma:
            case Avalonia.Input.PhysicalKey.NumPadDecimal:
            case Avalonia.Input.PhysicalKey.NumPadDivide:
            case Avalonia.Input.PhysicalKey.NumPadEnter:
            case Avalonia.Input.PhysicalKey.NumPadEqual:
            case Avalonia.Input.PhysicalKey.NumPadMultiply:
            case Avalonia.Input.PhysicalKey.NumPadParenLeft:
            case Avalonia.Input.PhysicalKey.NumPadParenRight:
            case Avalonia.Input.PhysicalKey.NumPadSubtract:
                return CefEventFlags.IsKeyPad;
            default:
                return CefEventFlags.None;
            }
        }

        /* CEF flags of the keyboard modifiers and, for pointer events, the pressed buttons */
        public static CefEventFlags Modifiers(Avalonia.Input.KeyModifiers keys, Avalonia.Input.PointerPointProperties pointer = default)
        {
            CefEventFlags flags = CefEventFlags.None;
            if ((keys & Avalonia.Input.KeyModifiers.Shift) != 0)
                flags |= CefEventFlags.ShiftDown;
            if ((keys & Avalonia.Input.KeyModifiers.Control) != 0)
                flags |= CefEventFlags.ControlDown;
            if ((keys & Avalonia.Input.KeyModifiers.Alt) != 0)
                flags |= CefEventFlags.AltDown;
            if ((keys & Avalonia.Input.KeyModifiers.Meta) != 0)
                flags |= CefEventFlags.CommandDown;
            if (pointer.IsLeftButtonPressed)
                flags |= CefEventFlags.LeftMouseButton;
            if (pointer.IsMiddleButtonPressed)
                flags |= CefEventFlags.MiddleMouseButton;
            if (pointer.IsRightButtonPressed)
                flags |= CefEventFlags.RightMouseButton;
            return flags;
        }
    }
}
#endif

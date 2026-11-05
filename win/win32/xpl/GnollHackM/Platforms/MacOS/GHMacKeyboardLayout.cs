#if GNH_AVALONIA && MACOS
using System;
using System.Runtime.InteropServices;
using Avalonia.Input;

namespace GnollHackM
{
    /* The character a key types with no modifiers in the current keyboard layout, through the Text Input Sources and
       UCKeyTranslate, as Avalonia.Native computes key symbols. Avalonia reports only the Option-modified symbol, and a
       key from it, so this tells which letter an Option combination is pressed on. Main thread. */
    internal static class GHMacKeyboardLayout
    {
        private const string HIToolboxLibrary = "/System/Library/Frameworks/Carbon.framework/Frameworks/HIToolbox.framework/HIToolbox";
        private const string CoreServicesLibrary = "/System/Library/Frameworks/CoreServices.framework/CoreServices";
        private const string CoreFoundationLibrary = "/System/Library/Frameworks/CoreFoundation.framework/CoreFoundation";
        private const ushort kUCKeyActionDown = 0;
        private const uint kUCKeyTranslateNoDeadKeysMask = 1;

        [DllImport(HIToolboxLibrary)]
        private static extern IntPtr TISCopyCurrentKeyboardLayoutInputSource();

        [DllImport(HIToolboxLibrary)]
        private static extern IntPtr TISGetInputSourceProperty(IntPtr inputSource, IntPtr propertyKey);

        [DllImport(HIToolboxLibrary)]
        private static extern byte LMGetKbdType();

        [DllImport(CoreServicesLibrary)]
        private static extern int UCKeyTranslate(IntPtr keyLayoutPtr, ushort virtualKeyCode, ushort keyAction, uint modifierKeyState,
            uint keyboardType, uint keyTranslateOptions, ref uint deadKeyState, nuint maxStringLength, out nuint actualStringLength,
            [Out] char[] unicodeString);

        [DllImport(CoreFoundationLibrary)]
        private static extern IntPtr CFDataGetBytePtr(IntPtr data);

        [DllImport(CoreFoundationLibrary)]
        private static extern void CFRelease(IntPtr value);

        private static IntPtr _layoutDataKey = IntPtr.Zero;
        private static bool _failed = false;

        /* '\0' when the key is not on the main block or the layout cannot be read */
        public static char GetBaseCharacter(PhysicalKey physicalKey)
        {
            int scanCode = GetScanCode(physicalKey);
            if (scanCode < 0 || _failed)
                return '\0';

            IntPtr source = IntPtr.Zero;
            try
            {
                if (_layoutDataKey == IntPtr.Zero)
                {
                    IntPtr library = NativeLibrary.Load(HIToolboxLibrary);
                    _layoutDataKey = Marshal.ReadIntPtr(NativeLibrary.GetExport(library, "kTISPropertyUnicodeKeyLayoutData"));
                }

                source = TISCopyCurrentKeyboardLayoutInputSource();
                if (source == IntPtr.Zero)
                    return '\0';
                IntPtr data = TISGetInputSourceProperty(source, _layoutDataKey);
                if (data == IntPtr.Zero)
                    return '\0';
                IntPtr layout = CFDataGetBytePtr(data);
                if (layout == IntPtr.Zero)
                    return '\0';

                char[] buffer = new char[4];
                uint deadKeyState = 0;
                nuint length;
                if (UCKeyTranslate(layout, (ushort)scanCode, kUCKeyActionDown, 0, LMGetKbdType(), kUCKeyTranslateNoDeadKeysMask,
                    ref deadKeyState, (nuint)buffer.Length, out length, buffer) != 0 || deadKeyState != 0 || length != 1)
                    return '\0';
                return buffer[0];
            }
            catch (Exception ex)
            {
                /* A missing symbol fails the same way every time: stop asking */
                _failed = true;
                Console.Error.WriteLine("GHMacKeyboardLayout: " + ex.Message);
                return '\0';
            }
            finally
            {
                if (source != IntPtr.Zero)
                    CFRelease(source);
            }
        }

        /* macOS virtual key codes of the main block (Events.h), as Avalonia.Native's KeyTransform.mm maps them */
        private static int GetScanCode(PhysicalKey key)
        {
            switch (key)
            {
            case PhysicalKey.A: return 0x00;
            case PhysicalKey.S: return 0x01;
            case PhysicalKey.D: return 0x02;
            case PhysicalKey.F: return 0x03;
            case PhysicalKey.H: return 0x04;
            case PhysicalKey.G: return 0x05;
            case PhysicalKey.Z: return 0x06;
            case PhysicalKey.X: return 0x07;
            case PhysicalKey.C: return 0x08;
            case PhysicalKey.V: return 0x09;
            case PhysicalKey.IntlBackslash: return 0x0A;
            case PhysicalKey.B: return 0x0B;
            case PhysicalKey.Q: return 0x0C;
            case PhysicalKey.W: return 0x0D;
            case PhysicalKey.E: return 0x0E;
            case PhysicalKey.R: return 0x0F;
            case PhysicalKey.Y: return 0x10;
            case PhysicalKey.T: return 0x11;
            case PhysicalKey.Digit1: return 0x12;
            case PhysicalKey.Digit2: return 0x13;
            case PhysicalKey.Digit3: return 0x14;
            case PhysicalKey.Digit4: return 0x15;
            case PhysicalKey.Digit6: return 0x16;
            case PhysicalKey.Digit5: return 0x17;
            case PhysicalKey.Equal: return 0x18;
            case PhysicalKey.Digit9: return 0x19;
            case PhysicalKey.Digit7: return 0x1A;
            case PhysicalKey.Minus: return 0x1B;
            case PhysicalKey.Digit8: return 0x1C;
            case PhysicalKey.Digit0: return 0x1D;
            case PhysicalKey.BracketRight: return 0x1E;
            case PhysicalKey.O: return 0x1F;
            case PhysicalKey.U: return 0x20;
            case PhysicalKey.BracketLeft: return 0x21;
            case PhysicalKey.I: return 0x22;
            case PhysicalKey.P: return 0x23;
            case PhysicalKey.L: return 0x25;
            case PhysicalKey.J: return 0x26;
            case PhysicalKey.Quote: return 0x27;
            case PhysicalKey.K: return 0x28;
            case PhysicalKey.Semicolon: return 0x29;
            case PhysicalKey.Backslash: return 0x2A;
            case PhysicalKey.Comma: return 0x2B;
            case PhysicalKey.Slash: return 0x2C;
            case PhysicalKey.N: return 0x2D;
            case PhysicalKey.M: return 0x2E;
            case PhysicalKey.Period: return 0x2F;
            case PhysicalKey.Backquote: return 0x32;
            default: return -1;
            }
        }
    }
}
#endif

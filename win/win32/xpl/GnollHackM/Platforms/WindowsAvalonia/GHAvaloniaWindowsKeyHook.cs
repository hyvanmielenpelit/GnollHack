#if GNH_AVALONIA && WINDOWS_AVALONIA
using System;
using System.Runtime.InteropServices;
using GnollHackX;
using AvWindow = Avalonia.Controls.Window;

namespace GnollHackM
{
    /* A low-level keyboard hook that swallows the Windows keys while Disable Windows Key is on, the keyboard hook is
       enabled and the game's window is in the foreground, so that the Start menu does not open over the game. Every
       other key, and the Windows keys otherwise, pass on to the system; GHAvaloniaKeyboard handles the game's keys.
       The callback runs on the UI thread, which installed the hook, and must return quickly: Windows skips, and may
       remove, a hook that exceeds LowLevelHooksTimeout. */
    internal static class GHAvaloniaWindowsKeyHook
    {
        private const int WH_KEYBOARD_LL = 13;
        private const int WM_KEYDOWN = 0x0100;
        private const int WM_KEYUP = 0x0101;
        private const int WM_SYSKEYDOWN = 0x0104;
        private const int WM_SYSKEYUP = 0x0105;
        private const int VK_LWIN = 0x5B;
        private const int VK_RWIN = 0x5C;

        private delegate IntPtr LowLevelKeyboardProc(int nCode, IntPtr wParam, IntPtr lParam);

        /* Held for the life of the process, so that the delegate the hook calls is never collected */
        private static readonly LowLevelKeyboardProc _proc = HookCallback;
        private static IntPtr _hookId = IntPtr.Zero;
        /* The game window's HWND; read by the callback */
        private static IntPtr _windowHandle = IntPtr.Zero;
        private static AvWindow _window = null;

        /* UI thread */
        public static void Start(AvWindow window)
        {
            if (window == null)
                return;

            _window = window;
            UpdateWindowHandle(window);
            window.Opened += Window_Opened;

            if (_hookId != IntPtr.Zero)
                return;
            _hookId = SetWindowsHookEx(WH_KEYBOARD_LL, _proc, GetModuleHandleW(null), 0);
            if (_hookId == IntPtr.Zero)
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowsKeyHook: SetWindowsHookEx failed with error "
                    + Marshal.GetLastWin32Error());
        }

        /* UI thread */
        public static void Stop()
        {
            IntPtr hookId = _hookId;
            _hookId = IntPtr.Zero;
            _windowHandle = IntPtr.Zero;
            if (_window != null)
            {
                _window.Opened -= Window_Opened;
                _window = null;
            }
            if (hookId != IntPtr.Zero)
                UnhookWindowsHookEx(hookId);
        }

        private static void Window_Opened(object sender, EventArgs e)
        {
            UpdateWindowHandle(sender as AvWindow);
        }

        /* The HWND, once the platform window has one */
        private static void UpdateWindowHandle(AvWindow window)
        {
            try
            {
                IntPtr handle = window?.TryGetPlatformHandle()?.Handle ?? IntPtr.Zero;
                if (handle != IntPtr.Zero)
                    _windowHandle = handle;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaWindowsKeyHook: cannot get the window handle: " + ex.Message);
            }
        }

        /* No game work here: only the Windows key decision, then the next hook */
        private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
        {
            if (nCode >= 0)
            {
                long message = wParam.ToInt64();
                if (message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
                {
                    int vkCode = Marshal.ReadInt32(lParam);
                    if ((vkCode == VK_LWIN || vkCode == VK_RWIN) && ShouldSwallowWindowsKey())
                        return (IntPtr)1;
                }
            }
            return CallNextHookEx(_hookId, nCode, wParam, lParam);
        }

        private static bool ShouldSwallowWindowsKey()
        {
            IntPtr windowHandle = _windowHandle;
            return windowHandle != IntPtr.Zero
                && GHApp.DisableWindowsKey
                && GHApp.IsKeyboardHookEnabled
                && GetForegroundWindow() == windowHandle;
        }

        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr SetWindowsHookEx(int idHook, LowLevelKeyboardProc lpfn, IntPtr hMod,
            uint dwThreadId);

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool UnhookWindowsHookEx(IntPtr hhk);

        [DllImport("user32.dll")]
        private static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        private static extern IntPtr GetForegroundWindow();

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
        private static extern IntPtr GetModuleHandleW(string lpModuleName);
    }
}
#endif

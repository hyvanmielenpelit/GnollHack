#if GNH_AVALONIA && (LINUX || MACOS || WINDOWS_AVALONIA)
using System;
using System.Diagnostics;
using System.IO;
#if WINDOWS_AVALONIA
using System.Runtime.InteropServices;
#endif

namespace GnollHackM
{
    /* One process per data folder. The operating system releases the lock when the process ends, however it ends. */
    internal static class GHAvaloniaInstanceLock
    {
#if MACOS
        private const int LockHeldHResult = 35; /* EWOULDBLOCK, as .NET reports a lock held elsewhere on macOS */
#elif WINDOWS_AVALONIA
        private const int LockHeldHResult = unchecked((int)0x80070020); /* ERROR_SHARING_VIOLATION */
#else
        private const int LockHeldHResult = 11; /* EWOULDBLOCK, as .NET reports a lock held elsewhere on Linux */
#endif
        private const string AlreadyRunningMessage = "GnollHack is already running. Close the other GnollHack window first.";

        private static FileStream _lock;

        /* False only when another process holds the lock. Any other failure lets the game start: being unable to
           create a lock file must not stop someone from playing. */
        public static bool TryAcquire()
        {
            try
            {
                _lock = new FileStream(Path.Combine(GHAvaloniaPaths.DataDirectory, "instance.lock"),
                    FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
                return true;
            }
            catch (IOException ex) when (ex.HResult == LockHeldHResult)
            {
                return false;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaInstanceLock: " + ex.Message);
                return true;
            }
        }

        public static void ReportAlreadyRunning()
        {
            Console.Error.WriteLine(AlreadyRunningMessage);
#if MACOS
            ShowMacAlert(AlreadyRunningMessage);
#elif WINDOWS_AVALONIA
            ShowWindowsMessage(AlreadyRunningMessage);
#else
            NativeLibraryPreflightLinux.ShowDialog(AlreadyRunningMessage);
#endif
        }

#if WINDOWS_AVALONIA
        private const uint MB_OK = 0x00000000;
        private const uint MB_ICONINFORMATION = 0x00000040;
        private const uint MB_SETFOREGROUND = 0x00010000;

        [DllImport("user32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
        private static extern int MessageBoxW(IntPtr hWnd, string lpText, string lpCaption, uint uType);

        /* A Win32 message box, since Avalonia is not started yet; a failure leaves only the standard error line */
        private static void ShowWindowsMessage(string message)
        {
            try
            {
                if (MessageBoxW(IntPtr.Zero, message, "GnollHack", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND) == 0)
                    Console.Error.WriteLine("GHAvaloniaInstanceLock: MessageBoxW failed with error "
                        + Marshal.GetLastWin32Error());
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaInstanceLock: " + ex.Message);
            }
        }
#endif

#if MACOS
        /* Through osascript, since Avalonia is not started yet; a failure leaves only the standard error line */
        private static void ShowMacAlert(string message)
        {
            try
            {
                ProcessStartInfo info = new ProcessStartInfo("/usr/bin/osascript");
                info.ArgumentList.Add("-e");
                info.ArgumentList.Add("display alert \"GnollHack\" message \"" + message.Replace("\\", "\\\\").Replace("\"", "\\\"") + "\"");
                info.UseShellExecute = false;
                using (Process process = Process.Start(info))
                {
                    process?.WaitForExit();
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaInstanceLock: " + ex.Message);
            }
        }
#endif
    }
}
#endif

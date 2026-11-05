#if GNH_AVALONIA && (LINUX || MACOS)
using System;
using System.Diagnostics;
using System.IO;

namespace GnollHackM
{
    /* One process per data folder. The operating system releases the lock when the process ends, however it ends. */
    internal static class GHAvaloniaInstanceLock
    {
#if MACOS
        private const int WouldBlockHResult = 35; /* EWOULDBLOCK, as .NET reports a lock held elsewhere on macOS */
#else
        private const int WouldBlockHResult = 11; /* EWOULDBLOCK, as .NET reports a lock held elsewhere on Linux */
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
            catch (IOException ex) when (ex.HResult == WouldBlockHResult)
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
#else
            NativeLibraryPreflightLinux.ShowDialog(AlreadyRunningMessage);
#endif
        }

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

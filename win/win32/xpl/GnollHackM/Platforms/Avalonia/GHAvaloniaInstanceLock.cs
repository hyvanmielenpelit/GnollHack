#if GNH_AVALONIA && LINUX
using System;
using System.IO;

namespace GnollHackM
{
    /* One process per data folder. The operating system releases the lock when the process ends, however it ends. */
    internal static class GHAvaloniaInstanceLock
    {
        private const int WouldBlockHResult = 11; /* EWOULDBLOCK, as .NET reports a lock held elsewhere on Linux */
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
            NativeLibraryPreflightLinux.ShowDialog(AlreadyRunningMessage);
        }
    }
}
#endif

#if GNH_AVALONIA
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace GnollHackM
{
    /* Whether Steam has injected its overlay into this process. Windows: GameOverlayRenderer64.dll is loaded. Linux:
       gameoverlayrenderer.so, which Steam preloads, shows in LD_PRELOAD or, once loaded, in the process's memory
       map. Always false on macOS. Once found, the answer stays true; until then it is checked again at most every
       RecheckIntervalMs, since Steam may inject the overlay after the start. Any thread; never blocks. */
    public static class GHAvaloniaSteamOverlay
    {
        public const int RecheckIntervalMs = 10000;

        private const string WindowsOverlayLibrary = "GameOverlayRenderer64.dll";
        private const string LinuxOverlayLibrary = "gameoverlayrenderer.so";

        private static volatile bool _present = false;

        /* Guarded by _checkLock */
        private static readonly object _checkLock = new object();
        private static bool _checked = false;
        private static long _lastCheckTicks = 0;

        public static bool IsPresent
        {
            get
            {
                if (_present)
                    return true;
                if (!OperatingSystem.IsWindows() && !OperatingSystem.IsLinux())
                    return false;

                /* Another thread is checking; its answer arrives with the next call */
                if (!Monitor.TryEnter(_checkLock))
                    return false;
                try
                {
                    long now = Stopwatch.GetTimestamp();
                    if (_checked && (now - _lastCheckTicks) * 1000 < (long)RecheckIntervalMs * Stopwatch.Frequency)
                        return false;
                    bool first = !_checked;
                    _checked = true;
                    _lastCheckTicks = now;

                    bool found = Detect();
                    if (found)
                    {
                        _present = true;
                        Console.Error.WriteLine("GHAvaloniaSteamOverlay: Steam overlay detected");
                    }
                    else if (first)
                    {
                        Console.Error.WriteLine("GHAvaloniaSteamOverlay: Steam overlay not detected; checking again"
                            + " every " + (RecheckIntervalMs / 1000) + " s");
                    }
                    return found;
                }
                finally
                {
                    Monitor.Exit(_checkLock);
                }
            }
        }

        private static bool Detect()
        {
            try
            {
                if (OperatingSystem.IsWindows())
                    return GetModuleHandleW(WindowsOverlayLibrary) != IntPtr.Zero;

                string preload = Environment.GetEnvironmentVariable("LD_PRELOAD");
                if (preload != null && preload.Contains(LinuxOverlayLibrary, StringComparison.Ordinal))
                    return true;
                foreach (string line in File.ReadLines("/proc/self/maps"))
                {
                    if (line.Contains(LinuxOverlayLibrary, StringComparison.Ordinal))
                        return true;
                }
                return false;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaSteamOverlay: cannot check for the Steam overlay: " + ex.Message);
                return false;
            }
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern IntPtr GetModuleHandleW(string lpModuleName);
    }
}
#endif

#if GNH_AVALONIA
using System;
using GnollHackX;

namespace GnollHackM
{
    /* Avalonia window lifecycle hooks, registered through ACM's AddWindows lifecycle builder */
    public static class GHAvaloniaWindowHooks
    {
        private static readonly TimeSpan FrameReportInterval = TimeSpan.FromSeconds(10);

        /* Called before the window is shown */
        public static void OnWindowCreated(Avalonia.Controls.Window window)
        {
            if (window == null)
                return;

            if (IsFullScreenRequested())
                window.WindowState = Avalonia.Controls.WindowState.FullScreen;

            /* Writes the frame loop report to the log while a game is running */
            Avalonia.Threading.DispatcherTimer reportTimer = new Avalonia.Threading.DispatcherTimer();
            reportTimer.Interval = FrameReportInterval;
            reportTimer.Tick += (sender, e) =>
            {
                string report = GHAvaloniaFrameLoop.GetReport();
                if (GHApp.CurrentGamePage != null)
                    GHApp.MaybeWriteGHLog(report);
            };

            window.Opened += (sender, e) =>
            {
                GHAvaloniaDeviceDisplay.Current?.OnWindowOpened(window);
                GHAvaloniaFrameLoop.Start(window);
                reportTimer.Start();
            };

            window.Closed += (sender, e) =>
            {
                reportTimer.Stop();
                GHAvaloniaFrameLoop.Stop();
            };
        }

        /* Steam's Gaming Mode sets SteamGamepadUI=1 */
        private static bool IsFullScreenRequested()
        {
            if (Environment.GetEnvironmentVariable("SteamGamepadUI") == "1")
                return true;

            string[] args = Environment.GetCommandLineArgs();
            if (args != null)
            {
                foreach (string arg in args)
                {
                    if (string.Equals(arg, "--fullscreen", StringComparison.OrdinalIgnoreCase))
                        return true;
                }
            }
            return false;
        }
    }
}
#endif

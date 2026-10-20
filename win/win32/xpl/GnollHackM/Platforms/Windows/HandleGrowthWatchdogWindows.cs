#if WINDOWS && GNH_MAUI && SENTRY
using System;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Threading;
using GnollHackX;
using Sentry;

namespace GnollHackM
{
    /* Checks the handle count every minute and, the first time it exceeds the startup baseline by
       GHConstants.HandleGrowthWarningThreshold, sends one Sentry warning event and asks the main page
       to show a restart notice. The event passes BeforeSend like any other, so it carries the full
       Process Memory context and handle census while system commit is still available. */
    internal static class HandleGrowthWatchdogWindows
    {
        private const int CheckIntervalMilliseconds = 60 * 1000;
        private const string ThresholdEnvironmentVariable = "GNH_HANDLE_WARNING_THRESHOLD";

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetCurrentProcess();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetProcessHandleCount(IntPtr process, out uint handleCount);

        private static Timer _timer;
        private static int _started;
        private static int _fired;
        private static uint _baselineHandles;
        private static int _threshold;
        private static long _startTimestamp;

        public static void Start(uint baselineHandles)
        {
            try
            {
                if (Interlocked.Exchange(ref _started, 1) != 0)
                    return;
                _baselineHandles = baselineHandles;
                _threshold = GHConstants.HandleGrowthWarningThreshold;
#if DEBUG
                int overrideThreshold;
                string overrideText = Environment.GetEnvironmentVariable(ThresholdEnvironmentVariable);
                if (!string.IsNullOrEmpty(overrideText)
                    && int.TryParse(overrideText, NumberStyles.Integer, CultureInfo.InvariantCulture, out overrideThreshold)
                    && overrideThreshold > 0)
                {
                    _threshold = overrideThreshold;
                }
#endif
                _startTimestamp = Stopwatch.GetTimestamp();
                _timer = new Timer(OnTimer, null, CheckIntervalMilliseconds, CheckIntervalMilliseconds);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnTimer(object state)
        {
            try
            {
                if (Interlocked.CompareExchange(ref _fired, 0, 0) != 0)
                    return;
                uint handles;
                if (!GetProcessHandleCount(GetCurrentProcess(), out handles))
                    return;
                long growth = (long)handles - _baselineHandles;
                if (growth < _threshold)
                    return;
                if (Interlocked.Exchange(ref _fired, 1) != 0)
                    return;

                long minutes = (long)((Stopwatch.GetTimestamp() - _startTimestamp) / (double)Stopwatch.Frequency / 60.0);
                bool gameRunning = GHApp.CurrentGamePage != null;
                GHApp.AddSentryBreadcrumb("Handle growth warning: handles " + handles + " (+" + growth + ") after "
                    + minutes + " min, threshold " + _threshold, GHConstants.SentryGnollHackGeneralCategoryName);
                SentrySdk.CaptureMessage("Handle growth warning", scope =>
                {
                    scope.SetFingerprint(new[] { "gnollhack-handle-growth-warning" });
                    scope.SetExtra("Handle Growth", growth);
                    scope.SetExtra("Handles", (long)handles);
                    scope.SetExtra("Baseline Handles", (long)_baselineHandles);
                    scope.SetExtra("Minutes Since Start", minutes);
                    scope.SetExtra("Game Running", gameRunning);
                }, SentryLevel.Warning);
                GHApp.ShowResourceWarningNotice();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
    }
}
#endif

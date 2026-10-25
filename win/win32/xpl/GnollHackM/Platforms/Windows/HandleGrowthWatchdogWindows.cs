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
    /* Checks the handle count and the committed pagefile-backed section bytes every minute and, the
       first time the handles exceed the startup baseline by GHConstants.HandleGrowthWarningThreshold or
       the section bytes exceed the first census by GHConstants.SectionCommitGrowthWarningBytes, sends
       one Sentry warning event and asks for a restart notice, which is retried every minute until a page
       shows it. The event passes BeforeSend like any other, so it carries the full Process Memory
       context and handle census while system commit is still available. */
    internal static class HandleGrowthWatchdogWindows
    {
        private const int CheckIntervalMilliseconds = 60 * 1000;
        private const string ThresholdEnvironmentVariable = "GNH_HANDLE_WARNING_THRESHOLD";
        private const string SectionThresholdEnvironmentVariable = "GNH_SECTION_WARNING_MB";
        private const string HandlesTrigger = "handles";
        private const string SectionCommitTrigger = "section commit";

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
        private static long _sectionThreshold;
        private static long _startTimestamp;

        /* Timer thread only */
        private static bool _hasBaselineSectionBytes;
        private static long _baselineSectionBytes;

        public static void Start(uint baselineHandles)
        {
            try
            {
                if (Interlocked.Exchange(ref _started, 1) != 0)
                    return;
                _baselineHandles = baselineHandles;
                _threshold = GHConstants.HandleGrowthWarningThreshold;
                _sectionThreshold = GHConstants.SectionCommitGrowthWarningBytes;
#if DEBUG
                int overrideThreshold;
                string overrideText = Environment.GetEnvironmentVariable(ThresholdEnvironmentVariable);
                if (!string.IsNullOrEmpty(overrideText)
                    && int.TryParse(overrideText, NumberStyles.Integer, CultureInfo.InvariantCulture, out overrideThreshold)
                    && overrideThreshold > 0)
                {
                    _threshold = overrideThreshold;
                }
                int overrideSectionMiB;
                string overrideSectionText = Environment.GetEnvironmentVariable(SectionThresholdEnvironmentVariable);
                if (!string.IsNullOrEmpty(overrideSectionText)
                    && int.TryParse(overrideSectionText, NumberStyles.Integer, CultureInfo.InvariantCulture, out overrideSectionMiB)
                    && overrideSectionMiB > 0)
                {
                    _sectionThreshold = (long)overrideSectionMiB * 1024L * 1024L;
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
                {
                    GHApp.TryShowPendingResourceWarning();
                    return;
                }
                uint handles;
                if (!GetProcessHandleCount(GetCurrentProcess(), out handles))
                    return;
                long growth = (long)handles - _baselineHandles;
                string trigger = growth >= _threshold ? HandlesTrigger : null;

                /* Window classes are not needed: windowBytes 0 still sums PagefileSectionBytes */
                bool hasSectionGrowth = false;
                long sectionBytes = 0;
                long sectionGrowth = 0;
                HandleCensus census;
                if (HandleCensusWindows.TryTake(true, 0, out census) && census.HasSectionSizes)
                {
                    sectionBytes = census.PagefileSectionBytes;
                    if (!_hasBaselineSectionBytes)
                    {
                        _baselineSectionBytes = sectionBytes;
                        _hasBaselineSectionBytes = true;
                    }
                    else
                    {
                        sectionGrowth = sectionBytes - _baselineSectionBytes;
                        hasSectionGrowth = true;
                        if (trigger == null && sectionGrowth >= _sectionThreshold)
                            trigger = SectionCommitTrigger;
                    }
                }
                if (trigger == null)
                    return;
                if (Interlocked.Exchange(ref _fired, 1) != 0)
                    return;

                long minutes = (long)((Stopwatch.GetTimestamp() - _startTimestamp) / (double)Stopwatch.Frequency / 60.0);
                bool gameRunning = GHApp.CurrentGamePage != null;
                GHApp.AddSentryBreadcrumb("Handle growth warning: handles " + handles + " (+" + growth + ") after "
                    + minutes + " min, threshold " + _threshold
                    + (hasSectionGrowth ? ", section commit +" + (sectionGrowth / (1024L * 1024L)) + " MiB" : "")
                    + ", trigger " + trigger, GHConstants.SentryGnollHackGeneralCategoryName);
                SentrySdk.CaptureMessage("Handle growth warning", scope =>
                {
                    scope.SetFingerprint(new[] { "gnollhack-handle-growth-warning" });
                    scope.SetExtra("Trigger", trigger);
                    scope.SetExtra("Handle Growth", growth);
                    scope.SetExtra("Handles", (long)handles);
                    scope.SetExtra("Baseline Handles", (long)_baselineHandles);
                    if (hasSectionGrowth)
                    {
                        scope.SetExtra("Section Commit Growth", sectionGrowth);
                        scope.SetExtra("Pagefile Section Bytes", sectionBytes);
                        scope.SetExtra("Baseline Pagefile Section Bytes", _baselineSectionBytes);
                    }
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

#if SENTRY
using System.Globalization;
using System.Runtime.Versioning;
using System.Text;
using Android.App;
using Android.Content;
using GnollHackX;

namespace GnollHackM;

// WORKAROUND(WA-SENTRY-0010): Sentry's Android ANR events omit the ApplicationExitInfo description
/* Reports the ANR exits and the other foreground process deaths that Android recorded since the last
   report, each as a Sentry message. The ANR messages carry the exit description and trace header that
   Sentry's own ANR event lacks. */
public static class AndroidExitInfoReporter
{
    private const string LastReportedTimestampKey = "AndroidExitInfoLastReportedAnrTimestamp";
    private const string LastReportedExitTimestampKey = "AndroidExitInfoLastReportedExitTimestamp";
    /* When this device first ran a build that marks CloseApplication exits */
    private const string CloseMarkerSinceTimestampKey = "AndroidExitInfoCloseMarkerSinceTimestamp";
    /* Set as the process state summary just before CloseApplication kills the process */
    private const string CloseApplicationStateSummary = "gnh-close-application";

    /* ApplicationExitInfo.REASON_* values; a reason not named here is reported by number */
    private const int ReasonExitSelf = 1;
    private const int ReasonSignaled = 2;
    private const int ReasonLowMemory = 3;
    private const int ReasonCrash = 4;
    private const int ReasonCrashNative = 5;
    private const int ReasonAnr = 6;
    private const int ReasonInitializationFailure = 7;
    private const int ReasonPermissionChange = 8;
    private const int ReasonExcessiveResourceUsage = 9;
    private const int ReasonUserRequested = 10;
    private const int ReasonUserStopped = 11;
    private const int ReasonDependencyDied = 12;
    private const int ReasonOther = 13;
    private const int ReasonFreezer = 14;
    private const int ReasonPackageStateChange = 15;
    private const int ReasonPackageUpdated = 16;
    private const int SignalKill = 9;
    private const long FirstRunLookbackMilliseconds = 7L * 24L * 60L * 60L * 1000L;
    private const int MaxExitInfosRead = 16;
    private const int MaxReportsPerLaunch = 5;
    /* The header before the thread dump can run to dozens of lines; reading stops at the dump line */
    private const int MaxTraceHeaderLines = 1000;
    private const int MaxExtraLength = 1000;

    private static int _started = 0;

    public static void StartReporting(Context context)
    {
        if (Interlocked.Exchange(ref _started, 1) != 0)
            return;
        if (context == null)
            return;

        Context appContext = context.ApplicationContext ?? context;
        Task.Run(() =>
        {
            if (OperatingSystem.IsAndroidVersionAtLeast(30))
                ReportNewExitInfos(appContext);
        });
    }

    /* Called on the UI thread just before CloseApplication ends the process, so that exit is not reported */
    public static void MarkCloseApplication()
    {
        if (!OperatingSystem.IsAndroidVersionAtLeast(30))
            return;
        try
        {
            ActivityManager activityManager = Android.App.Application.Context.GetSystemService(Context.ActivityService) as ActivityManager;
            activityManager?.SetProcessStateSummary(Encoding.ASCII.GetBytes(CloseApplicationStateSummary));
        }
        catch (Exception ex)
        {
            GHApp.MaybeWriteGHLog("AndroidExitInfoReporter mark: " + ex.Message);
        }
    }

    /* Exits recorded before this time come from builds that did not mark CloseApplication exits */
    private static long GetCloseMarkerSince()
    {
        long since = Preferences.Get(CloseMarkerSinceTimestampKey, 0L);
        if (since <= 0)
        {
            since = Java.Lang.JavaSystem.CurrentTimeMillis();
            Preferences.Set(CloseMarkerSinceTimestampKey, since);
        }
        return since;
    }

    [SupportedOSPlatform("android30.0")]
    private static void ReportNewExitInfos(Context context)
    {
        try
        {
            long closeMarkerSince = GetCloseMarkerSince();
            if (!SentrySdk.IsEnabled)
                return;

            ActivityManager activityManager = context.GetSystemService(Context.ActivityService) as ActivityManager;
            if (activityManager == null)
                return;

            IList<ApplicationExitInfo> exitInfos = activityManager.GetHistoricalProcessExitReasons(context.PackageName, 0, MaxExitInfosRead);
            if (exitInfos == null || exitInfos.Count == 0)
                return;

            try
            {
                ReportNewAnrs(exitInfos);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("AndroidExitInfoReporter: " + ex.Message);
            }

            try
            {
                ReportNewProcessDeaths(exitInfos, closeMarkerSince);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("AndroidExitInfoReporter deaths: " + ex.Message);
            }
        }
        catch (Exception ex)
        {
            GHApp.MaybeWriteGHLog("AndroidExitInfoReporter: " + ex.Message);
        }
    }

    [SupportedOSPlatform("android30.0")]
    private static void ReportNewAnrs(IList<ApplicationExitInfo> exitInfos)
    {
        long lastReported = Preferences.Get(LastReportedTimestampKey, 0L);
        long cutoff = lastReported > 0 ? lastReported : Java.Lang.JavaSystem.CurrentTimeMillis() - FirstRunLookbackMilliseconds;
        List<ApplicationExitInfo> anrs = new List<ApplicationExitInfo>();
        foreach (ApplicationExitInfo exitInfo in exitInfos)
        {
            if (exitInfo != null && exitInfo.Reason == (int)ApplicationExitInfoReason.Anr && exitInfo.Timestamp > cutoff)
                anrs.Add(exitInfo);
        }
        if (anrs.Count == 0)
            return;

        /* Oldest first; only the newest MaxReportsPerLaunch are sent */
        anrs.Sort((a, b) => a.Timestamp.CompareTo(b.Timestamp));
        int i;
        for (i = Math.Max(0, anrs.Count - MaxReportsPerLaunch); i < anrs.Count; i++)
        {
            ReportAnr(anrs[i]);
        }
        Preferences.Set(LastReportedTimestampKey, anrs[anrs.Count - 1].Timestamp);
    }

    // WORKAROUND(WA-SENTRY-0012): Sentry reports no Android process deaths other than native crashes and ANRs
    /* Reports each process death in the foreground or visible state that Android recorded since the
       last report, other than ANRs, user and package actions, clean exits, CloseApplication, and SIGKILLs
       recorded before CloseApplication exits were marked */
    [SupportedOSPlatform("android30.0")]
    private static void ReportNewProcessDeaths(IList<ApplicationExitInfo> exitInfos, long closeMarkerSince)
    {
        long lastReported = Preferences.Get(LastReportedExitTimestampKey, 0L);
        long cutoff = lastReported > 0 ? lastReported : Java.Lang.JavaSystem.CurrentTimeMillis() - FirstRunLookbackMilliseconds;
        List<ApplicationExitInfo> deaths = new List<ApplicationExitInfo>();
        foreach (ApplicationExitInfo exitInfo in exitInfos)
        {
            if (exitInfo != null && exitInfo.Timestamp > cutoff && IsReportedProcessDeath(exitInfo, closeMarkerSince))
                deaths.Add(exitInfo);
        }
        if (deaths.Count == 0)
            return;

        /* Oldest first; only the newest MaxReportsPerLaunch are sent */
        deaths.Sort((a, b) => a.Timestamp.CompareTo(b.Timestamp));
        int i;
        for (i = Math.Max(0, deaths.Count - MaxReportsPerLaunch); i < deaths.Count; i++)
        {
            ReportProcessDeath(deaths[i]);
        }
        Preferences.Set(LastReportedExitTimestampKey, deaths[deaths.Count - 1].Timestamp);
    }

    [SupportedOSPlatform("android30.0")]
    private static bool IsReportedProcessDeath(ApplicationExitInfo exitInfo, long closeMarkerSince)
    {
        switch (exitInfo.Reason)
        {
            case ReasonAnr:
            case ReasonUserRequested:
            case ReasonUserStopped:
            case ReasonPermissionChange:
            case ReasonPackageStateChange:
            case ReasonPackageUpdated:
                return false;
            case ReasonExitSelf:
                if (exitInfo.Status == 0)
                    return false;
                break;
            default:
                break;
        }
        /* Before the marker, an exit through CloseApplication cannot be told from any other SIGKILL */
        if (exitInfo.Reason == ReasonSignaled && exitInfo.Status == SignalKill && exitInfo.Timestamp < closeMarkerSince)
            return false;
        if (exitInfo.Importance > (int)Importance.Visible)
            return false;
        byte[] summary = exitInfo.GetProcessStateSummary();
        if (summary != null && Encoding.ASCII.GetString(summary) == CloseApplicationStateSummary)
            return false;
        return true;
    }

    [SupportedOSPlatform("android30.0")]
    private static void ReportProcessDeath(ApplicationExitInfo exitInfo)
    {
        string reason = ReasonName(exitInfo.Reason);
        string status = exitInfo.Status.ToString(CultureInfo.InvariantCulture);
        string importance = exitInfo.Importance.ToString(CultureInfo.InvariantCulture);
        string exitTime = DateTimeOffset.FromUnixTimeMilliseconds(exitInfo.Timestamp).UtcDateTime
            .ToString("yyyy-MM-ddTHH:mm:ss.fffZ", CultureInfo.InvariantCulture);
        /* A signal's number separates kills (9) from faults (6, 11), so each gets its own issue */
        string issueKey = exitInfo.Reason == ReasonSignaled ? reason + " " + status : reason;
        string lowMemoryKillReportSupported = ActivityManager.IsLowMemoryKillReportSupported ? "yes" : "no";
        byte[] stateSummaryBytes = exitInfo.GetProcessStateSummary();
        string stateSummary = stateSummaryBytes != null && stateSummaryBytes.Length > 0 ? Encoding.ASCII.GetString(stateSummaryBytes) : null;

        SentrySdk.CaptureMessage("Android process exit info: " + issueKey, scope =>
        {
            scope.Fingerprint = new[] { "android-process-exit-info", issueKey };
            scope.SetTag("exit.reason", reason);
            scope.SetTag("exit.status", status);
            scope.SetTag("exit.importance", importance);
            scope.SetTag("exit.time", exitTime);
            /* No null extras: see WA-SENTRY-0008 */
            scope.SetExtra("Exit Description", Shorten(exitInfo.Description));
            scope.SetExtra("Exit Status", status);
            scope.SetExtra("Exit Importance", importance);
            scope.SetExtra("Exit Time", exitTime);
            scope.SetExtra("Exit Pss KB", exitInfo.Pss.ToString(CultureInfo.InvariantCulture));
            scope.SetExtra("Exit Rss KB", exitInfo.Rss.ToString(CultureInfo.InvariantCulture));
            scope.SetExtra("Exit Process Name", Shorten(exitInfo.ProcessName));
            scope.SetTag("exit.lmk_report_supported", lowMemoryKillReportSupported);
            scope.SetExtra("Exit State Summary", Shorten(stateSummary));
        }, SentryLevel.Warning);
    }

    private static string ReasonName(int reason)
    {
        switch (reason)
        {
            case ReasonExitSelf: return "exit-self";
            case ReasonSignaled: return "signaled";
            case ReasonLowMemory: return "low-memory";
            case ReasonCrash: return "crash";
            case ReasonCrashNative: return "crash-native";
            case ReasonInitializationFailure: return "initialization-failure";
            case ReasonExcessiveResourceUsage: return "excessive-resource-usage";
            case ReasonDependencyDied: return "dependency-died";
            case ReasonOther: return "other";
            case ReasonFreezer: return "freezer";
            default: return "reason-" + reason.ToString(CultureInfo.InvariantCulture);
        }
    }

    [SupportedOSPlatform("android30.0")]
    private static void ReportAnr(ApplicationExitInfo exitInfo)
    {
        string description = exitInfo.Description;
        string subject;
        string dumpLine;
        ReadTraceHeader(exitInfo, out subject, out dumpLine);

        string anrType = ClassifyAnr(!string.IsNullOrEmpty(description) ? description : subject);
        bool isForeground = exitInfo.Importance == (int)Importance.Foreground;
        string exitTime = DateTimeOffset.FromUnixTimeMilliseconds(exitInfo.Timestamp).UtcDateTime
            .ToString("yyyy-MM-ddTHH:mm:ss.fffZ", CultureInfo.InvariantCulture);

        SentrySdk.CaptureMessage("Android ANR exit info: " + anrType, scope =>
        {
            scope.Fingerprint = new[] { "android-anr-exit-info", anrType };
            scope.SetTag("anr.type", anrType);
            scope.SetTag("anr.foreground", isForeground ? "yes" : "no");
            scope.SetTag("anr.exit_time", exitTime);
            /* No null extras: see WA-SENTRY-0008 */
            scope.SetExtra("ANR Description", Shorten(description));
            scope.SetExtra("ANR Trace Subject", Shorten(subject));
            scope.SetExtra("ANR Trace Dump Line", Shorten(dumpLine));
            scope.SetExtra("ANR Exit Time", exitTime);
            scope.SetExtra("ANR Importance", exitInfo.Importance.ToString(CultureInfo.InvariantCulture));
            scope.SetExtra("ANR Pss KB", exitInfo.Pss.ToString(CultureInfo.InvariantCulture));
            scope.SetExtra("ANR Rss KB", exitInfo.Rss.ToString(CultureInfo.InvariantCulture));
            scope.SetExtra("ANR Process Name", Shorten(exitInfo.ProcessName));
        }, SentryLevel.Warning);
    }

    [SupportedOSPlatform("android30.0")]
    private static void ReadTraceHeader(ApplicationExitInfo exitInfo, out string subject, out string dumpLine)
    {
        subject = null;
        dumpLine = null;
        try
        {
            using (Stream stream = exitInfo.TraceInputStream)
            {
                if (stream == null)
                    return;
                using (StreamReader reader = new StreamReader(stream, Encoding.UTF8))
                {
                    int lineCount;
                    for (lineCount = 0; lineCount < MaxTraceHeaderLines; lineCount++)
                    {
                        string line = reader.ReadLine();
                        if (line == null)
                            break;
                        if (subject == null && line.StartsWith("Subject:", StringComparison.Ordinal))
                        {
                            subject = line.Substring(8).Trim();
                        }
                        else if (line.StartsWith("----- pid ", StringComparison.Ordinal))
                        {
                            dumpLine = line;
                            break;
                        }
                    }
                }
            }
        }
        catch (Exception ex)
        {
            GHApp.MaybeWriteGHLog("AndroidExitInfoReporter trace: " + ex.Message);
        }
    }

    private static string ClassifyAnr(string text)
    {
        if (string.IsNullOrEmpty(text))
            return "unknown";
        if (text.IndexOf("Input dispatching timed out", StringComparison.OrdinalIgnoreCase) >= 0)
        {
            if (text.IndexOf("does not have a focused window", StringComparison.OrdinalIgnoreCase) >= 0
                || text.IndexOf("no focused window", StringComparison.OrdinalIgnoreCase) >= 0)
                return "input-no-focused-window";
            return "input";
        }
        if (text.IndexOf("startForegroundService", StringComparison.Ordinal) >= 0)
            return "foreground-service-start";
        if (text.IndexOf("executing service", StringComparison.OrdinalIgnoreCase) >= 0)
            return "service";
        if (text.IndexOf("Broadcast of Intent", StringComparison.OrdinalIgnoreCase) >= 0)
            return "broadcast";
        if (text.IndexOf("ContentProvider", StringComparison.OrdinalIgnoreCase) >= 0)
            return "content-provider";
        return "other";
    }

    private static string Shorten(string text)
    {
        if (string.IsNullOrEmpty(text))
            return "(none)";
        return text.Length <= MaxExtraLength ? text : text.Substring(0, MaxExtraLength) + " [truncated]";
    }
}
#endif

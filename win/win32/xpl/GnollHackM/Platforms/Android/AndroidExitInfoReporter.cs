#if SENTRY
using System.Globalization;
using System.Runtime.Versioning;
using System.Text;
using Android.App;
using Android.Content;
using GnollHackX;

namespace GnollHackM;

// WORKAROUND(WA-SENTRY-0010): Sentry's Android ANR events omit the ApplicationExitInfo description
/* Reports each ANR exit that Android recorded since the last report as a Sentry message with the
   exit description and the trace header, which Sentry's own ANR event for that exit lacks */
public static class AndroidExitInfoReporter
{
    private const string LastReportedTimestampKey = "AndroidExitInfoLastReportedAnrTimestamp";
    private const long FirstRunLookbackMilliseconds = 7L * 24L * 60L * 60L * 1000L;
    private const int MaxExitInfosRead = 16;
    private const int MaxReportsPerLaunch = 5;
    private const int MaxTraceHeaderLines = 40;
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
                ReportNewAnrs(appContext);
        });
    }

    [SupportedOSPlatform("android30.0")]
    private static void ReportNewAnrs(Context context)
    {
        try
        {
            if (!SentrySdk.IsEnabled)
                return;

            ActivityManager activityManager = context.GetSystemService(Context.ActivityService) as ActivityManager;
            if (activityManager == null)
                return;

            IList<ApplicationExitInfo> exitInfos = activityManager.GetHistoricalProcessExitReasons(context.PackageName, 0, MaxExitInfosRead);
            if (exitInfos == null || exitInfos.Count == 0)
                return;

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
        catch (Exception ex)
        {
            GHApp.MaybeWriteGHLog("AndroidExitInfoReporter: " + ex.Message);
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

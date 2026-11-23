#if SENTRY
using System.Runtime.CompilerServices;
using Android.Content;
using Android.OS;
using GnollHackX;

namespace GnollHackM;

/* Developer diagnostics: blocks the UI thread in a known way, so that the resulting ANR trace shows what
   that wait looks like. Started by the launch intent extra gnh_anr_calibration ("lock" or "native") and
   run only in Developer Mode, a few seconds after the main menu has faded in. */
public static class AndroidAnrCalibration
{
    public const string IntentExtraName = "gnh_anr_calibration";
    public const string DelayExtraName = "gnh_anr_calibration_delay_s"; /* optional int, 0-3600 */
    public const string ModeManagedLock = "lock";
    public const string ModeFinalizerWait = "native";
    private const int DefaultDelaySeconds = 5;
    private const int MaxDelaySeconds = 3600;
    private const int PollMilliseconds = 1000;
    private const int MaxMainMenuWaitMilliseconds = 120000;
    private const int BlockMilliseconds = 60000;

    public static void MaybeSchedule(Intent intent)
    {
        try
        {
            if (intent == null)
                return;
            string mode = intent.GetStringExtra(IntentExtraName);
            if (mode == null)
                return;
            int delaySeconds = intent.GetIntExtra(DelayExtraName, DefaultDelaySeconds);
            intent.RemoveExtra(IntentExtraName);
            intent.RemoveExtra(DelayExtraName);
            if (mode != ModeManagedLock && mode != ModeFinalizerWait)
            {
                Log("ANR calibration ignored: unknown mode");
                return;
            }
            Handler handler = new Handler(Looper.MainLooper);
            WaitForMainMenu(handler, mode, Math.Clamp(delaySeconds, 0, MaxDelaySeconds), 0);
        }
        catch (Exception ex)
        {
            Log("ANR calibration: " + ex.Message);
        }
    }

    private static void WaitForMainMenu(Handler handler, string mode, int delaySeconds, int waitedMilliseconds)
    {
        handler.PostDelayed(() =>
        {
            try
            {
                if (GHStartupTimeline.Current.HasReached(GHStartupMilestone.MainMenu))
                    handler.PostDelayed(() => Run(mode), delaySeconds * 1000L);
                else if (waitedMilliseconds + PollMilliseconds >= MaxMainMenuWaitMilliseconds)
                    Log("ANR calibration skipped: the main menu did not appear");
                else
                    WaitForMainMenu(handler, mode, delaySeconds, waitedMilliseconds + PollMilliseconds);
            }
            catch (Exception ex)
            {
                Log("ANR calibration: " + ex.Message);
            }
        }, PollMilliseconds);
    }

    private static void Run(string mode)
    {
        try
        {
            if (!GHApp.DeveloperMode)
            {
                Log("ANR calibration skipped: Developer Mode is off");
                return;
            }
            AndroidExitInfoReporter.MarkAnrCalibration(mode);
            /* Scope sync persists the tag, and sentry-android restores it onto its own ANR event */
            SentrySdk.ConfigureScope(scope => scope.SetTag("anr.calibration", mode));
            Log("ANR calibration: blocking the UI thread for " + (BlockMilliseconds / 1000) + " s, mode " + mode);
        }
        catch (Exception ex)
        {
            Log("ANR calibration: " + ex.Message);
            return;
        }

        if (mode == ModeManagedLock)
            BlockOnManagedLock();
        else
            BlockOnFinalizerWait();
    }

    /* Managed Lock contention: the wait goes through the managed wait subsystem and LowLevelMonitor */
    private static void BlockOnManagedLock()
    {
        object gate = new object();
        ManualResetEventSlim held = new ManualResetEventSlim(false);
        Task.Run(() =>
        {
            lock (gate)
            {
                held.Set();
                Thread.Sleep(BlockMilliseconds);
            }
        });
        held.Wait();
        Monitor.Enter(gate);
        Monitor.Exit(gate);
    }

    /* A native runtime wait: GC.WaitForPendingFinalizers waits on a CLREvent inside libcoreclr.so */
    private static void BlockOnFinalizerWait()
    {
        AbandonSleepingFinalizable();
        GC.Collect();
        GC.WaitForPendingFinalizers();
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void AbandonSleepingFinalizable()
    {
        _ = new SleepingFinalizable();
    }

    private sealed class SleepingFinalizable
    {
        ~SleepingFinalizable()
        {
            Thread.Sleep(BlockMilliseconds);
        }
    }

    private static void Log(string text)
    {
        GHApp.MaybeWriteGHLog(text, addBreadcrumb: true, forceMessage: true);
    }
}
#endif

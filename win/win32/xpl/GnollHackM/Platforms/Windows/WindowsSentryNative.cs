#if SENTRY && WINDOWS
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using GnollHackX;
using Microsoft.Maui.Storage;

namespace GnollHackM
{
    /* Native crash capture for the Windows build through GHSentryNative and gnhsentry.dll, whose vectored exception
       handler captures faults in GnollHack's own native modules and whose filter wrapper keeps the runtime's
       unhandled-exception filter in the chain. Initialized once in CreateMauiApp; the Sentry.Maui options callback
       reads the stored results. */
    internal static class WindowsSentryNative
    {
        private static int _closeSubscribed = 0;

        internal static string Dsn { get; private set; }
        internal static string Release { get; private set; }
        internal static string Environment { get; private set; }
        internal static bool NativeInitialized { get; private set; }
        internal static bool CrashedLastRun { get; private set; }

        /* Before sentry_init: records the current unhandled-exception filter */
        [DllImport("gnhsentry")]
        internal static extern int gnh_crash_prepare();

        /* After sentry_init: puts the filter wrapper in front of sentry-native's filter and adds the vectored handler;
           0 when sentry-native's filter is not the current one */
        [DllImport("gnhsentry")]
        internal static extern int gnh_crash_install();

        /* Before sentry_close */
        [DllImport("gnhsentry")]
        internal static extern void gnh_crash_uninstall();

        /* Never throws; native crash capture stays off when it cannot start */
        internal static void Init()
        {
            try
            {
                /* SENTRY_DSN overrides the secrets file, as in Sentry's own convention */
                string dsn = System.Environment.GetEnvironmentVariable("SENTRY_DSN");
                if (string.IsNullOrWhiteSpace(dsn))
                    dsn = GHApp.CurrentUserSecrets?.DefaultSentryDNS;
                Dsn = dsn;

                /* Set explicitly on both SDKs, so that native crash events match the managed ones */
                Release = GHSentryNative.GetRelease();
                Environment = GHSentryNative.GetEnvironment();
                if (string.IsNullOrEmpty(dsn))
                    return;

#if DEBUG
                bool debug = true;
#else
                bool debug = false;
#endif
                /* Not the cache directory, which Windows may clean */
                string databaseDirectory = Path.Combine(FileSystem.AppDataDirectory, "sentry-native");
                NativeInitialized = GHSentryNative.Init(dsn, Release, Environment, debug, databaseDirectory, out bool crashedLastRun);
                CrashedLastRun = crashedLastRun;
            }
            catch (Exception ex)
            {
                NativeInitialized = false;
                CrashedLastRun = false;
                GHSentryNative.Log("WindowsSentryNative: cannot initialize native crash capture: " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        /* Closing takes the filter wrapper out of the chain before CoreCLR re-raises a terminating managed exception,
           which keeps one re-raised with a hardware code (a NullReferenceException or DivideByZeroException) away from
           sentry-native, and shuts the handler thread down in order. Called after builder.Build(), so Sentry .NET's own
           UnhandledException handler runs first. */
        internal static void SubscribeClose()
        {
            if (Interlocked.Exchange(ref _closeSubscribed, 1) != 0)
                return;

            AppDomain.CurrentDomain.UnhandledException += OnUnhandledException;
            AppDomain.CurrentDomain.ProcessExit += OnProcessExit;
        }

        private static void OnUnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            if (e.IsTerminating)
                GHSentryNative.Close();
        }

        private static void OnProcessExit(object sender, EventArgs e)
        {
            GHSentryNative.Close();
        }
    }
}
#endif

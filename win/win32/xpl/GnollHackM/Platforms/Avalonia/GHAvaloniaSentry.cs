#if GNH_AVALONIA && LINUX && SENTRY
using System;
using System.IO;
using GnollHackX;

namespace GnollHackM
{
    /* Sentry for the Linux build, through the core SDK: Sentry.Maui is not used on the Avalonia backend */
    internal static class GHAvaloniaSentry
    {
        public static void Init()
        {
            try
            {
                /* SENTRY_DSN overrides the secrets file, as in Sentry's own convention */
                string dsn = Environment.GetEnvironmentVariable("SENTRY_DSN");
                if (string.IsNullOrWhiteSpace(dsn))
                    dsn = GHApp.CurrentUserSecrets?.DefaultSentryDNS;
                if (string.IsNullOrEmpty(dsn))
                    return;

                /* Set explicitly on both SDKs, so that native crash events match the managed ones */
                string release = GHSentryNative.GetRelease();
                string environment = GHSentryNative.GetEnvironment();
#if DEBUG
                bool debug = true;
#else
                bool debug = false;
#endif
                /* Explicit: the default, .sentry-native in the current directory, is inside the read-only AppImage
                   mount, because AppRun changes into $APPDIR/gnollhack */
                string databaseDirectory = Path.Combine(GHAvaloniaPaths.DataDirectory, "sentry-native");
                /* Before SentrySdk.Init, which reads CrashedLastRun */
                bool nativeInitialized = GHSentryNative.Init(dsn, release, environment, debug, databaseDirectory, out bool crashedLastRun);

                SentrySdk.Init(options =>
                {
                    options.Dsn = dsn;
                    options.Release = release;
                    options.Environment = environment;
                    /* One scope shared by all threads, and release-health sessions, as Sentry.Maui sets them */
                    options.IsGlobalModeEnabled = true;
                    options.AutoSessionTracking = true;
                    options.CaptureFailedRequests = false;
                    /* Persists the session, which the next start ends as crashed (CrashedLastRun), abnormal or exited,
                       and keeps events that could not be sent */
                    options.CacheDirectoryPath = Path.Combine(GHAvaloniaPaths.DataDirectory, "sentry-dotnet");
#if DEBUG
                    options.Debug = true;
                    options.TracesSampleRate = 1.0;
#endif
                    if (nativeInitialized)
                    {
                        options.ScopeObserver = new GHSentryNativeScopeObserver();
                        options.EnableScopeSync = true;
                        options.CrashedLastRun = () => crashedLastRun;
                    }
                    // WORKAROUND(WA-AVALONIA-0023): IBus's Destroy reply reaches Sentry as an unobserved exception
                    options.AddExceptionFilter(new IBusDestroyReplyExceptionFilter());
                    options.SetBeforeSend(@event =>
                    {
                        if (@event == null)
                            return null;
                        GHSentryEventFallback.Apply(@event);
                        return @event;
                    });
                });

                string package = GetPackage();
                string container = GetContainer();
                string session = GetSessionType();
                string glibc = NativeLibraryPreflightLinux.GlibcVersion;
                SentrySdk.ConfigureScope(scope =>
                {
                    scope.SetTag(GHConstants.SentryTagLinuxPackage, package);
                    scope.SetTag(GHConstants.SentryTagLinuxContainer, container);
                    scope.SetTag(GHConstants.SentryTagLinuxSession, session);
                    scope.SetTag(GHConstants.SentryTagLinuxGlibc, glibc);
                });

                if (nativeInitialized)
                {
                    AppDomain.CurrentDomain.UnhandledException += OnUnhandledException;
                    AppDomain.CurrentDomain.ProcessExit += OnProcessExit;
                    GHSentryNative.StartOutboxUpload(dsn);
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaSentry: cannot initialize Sentry: " + ex.Message);
            }
        }

        /* A terminating managed exception is reported by Sentry .NET, whose handler, subscribed earlier, runs first.
           CoreCLR's abort path then bypasses sentry-native's SIGABRT handler; closing sentry-native uninstalls its
           handlers, so no native duplicate can follow even if that changes. */
        private static void OnUnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            if (e.IsTerminating)
                GHSentryNative.Close();
        }

        private static void OnProcessExit(object sender, EventArgs e)
        {
            GHSentryNative.Close();
        }

        private sealed class IBusDestroyReplyExceptionFilter : Sentry.Extensibility.IExceptionFilter
        {
            public bool Filter(Exception ex)
            {
                return GHIBusDestroyReplyFilter.IsIBusDestroyReply(ex);
            }
        }

        /* APPIMAGE first: an AppImage started from Steam as a non-Steam game also sees Steam's variables */
        private static string GetPackage()
        {
            try
            {
                if (!string.IsNullOrEmpty(Environment.GetEnvironmentVariable("APPIMAGE")))
                    return "appimage";
                string steamAppId = Environment.GetEnvironmentVariable("SteamAppId");
                if (!string.IsNullOrEmpty(steamAppId) && steamAppId != "0")
                    return "steam";
                return "other";
            }
            catch (Exception)
            {
                return "unknown";
            }
        }

        /* The Steam Linux Runtime's pressure-vessel container mounts /run/pressure-vessel */
        private static string GetContainer()
        {
            try
            {
                return Directory.Exists("/run/pressure-vessel") ? "pressure-vessel" : "none";
            }
            catch (Exception)
            {
                return "unknown";
            }
        }

        private static string GetSessionType()
        {
            try
            {
                string sessionType = Environment.GetEnvironmentVariable("XDG_SESSION_TYPE");
                return string.IsNullOrEmpty(sessionType) ? "unknown" : sessionType;
            }
            catch (Exception)
            {
                return "unknown";
            }
        }
    }
}
#endif

#if GNH_AVALONIA && (LINUX || MACOS || WINDOWS_AVALONIA) && SENTRY
using System;
using System.IO;
using GnollHackX;

namespace GnollHackM
{
    /* Sentry for the Linux, macOS and Windows builds, through the core SDK: Sentry.Maui is not used on the Avalonia
       backend. All three also run sentry-native for native crashes, and Sentry .NET's own handler reports a
       terminating managed exception. */
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
                   mount on Linux, because AppRun changes into $APPDIR/gnollhack, and / for an app started from
                   Finder on macOS */
                string databaseDirectory = Path.Combine(GHAvaloniaPaths.DataDirectory, "sentry-native");
                // WORKAROUND(WA-SENTRY-0003): Sentry .NET captures no native crashes in JIT desktop builds
                /* Before SentrySdk.Init, which reads CrashedLastRun */
                bool nativeInitialized = GHSentryNative.Init(dsn, release, environment, debug, databaseDirectory, out bool crashedLastRun);

                SentrySdk.Init(options =>
                {
                    options.Dsn = dsn;
                    if (release != null)
                        options.Release = release;
                    if (environment != null)
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
#if LINUX
                    // WORKAROUND(WA-AVALONIA-0023): IBus's Destroy reply reaches Sentry as an unobserved exception
                    options.AddExceptionFilter(new IBusDestroyReplyExceptionFilter());
#endif
                    options.SetBeforeSend(@event =>
                    {
                        if (@event == null)
                            return null;
                        GHSentryEventFallback.Apply(@event);
#if WINDOWS_AVALONIA
                        NativeLibraryPreflightWindows.AddToEvent(@event);
#endif
                        return @event;
                    });
                });

#if LINUX
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
#elif MACOS
                string macPackage = GetMacPackage();
                SentrySdk.ConfigureScope(scope =>
                {
                    scope.SetTag(GHConstants.SentryTagMacPackage, macPackage);
                });
#elif WINDOWS_AVALONIA
                string windowsPackage = GetWindowsPackage();
                SentrySdk.ConfigureScope(scope =>
                {
                    scope.SetTag(GHConstants.SentryTagWindowsPackage, windowsPackage);
                });
#endif

                if (nativeInitialized)
                {
                    AppDomain.CurrentDomain.UnhandledException += OnUnhandledException;
                    AppDomain.CurrentDomain.ProcessExit += OnProcessExit;
                    GHSentryNative.StartOutboxUpload(dsn);
                }

                /* GNOLLHACK_SENTRY_TEST=1 sends one message, to check delivery without a crash */
                if (Environment.GetEnvironmentVariable("GNOLLHACK_SENTRY_TEST") == "1")
                    SentrySdk.CaptureMessage("Sentry test from " + GHApp.PlatformDisplayName, SentryLevel.Info);
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

#if LINUX
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
#elif MACOS
        /* steam when Steam started the game, bundle for an .app, unbundled for the development copy */
        private static string GetMacPackage()
        {
            try
            {
                string steamAppId = Environment.GetEnvironmentVariable("SteamAppId");
                if (!string.IsNullOrEmpty(steamAppId) && steamAppId != "0")
                    return "steam";
                return AppContext.BaseDirectory.IndexOf(".app/Contents/MacOS/", StringComparison.OrdinalIgnoreCase) >= 0 ? "bundle" : "unbundled";
            }
            catch (Exception)
            {
                return "unknown";
            }
        }
#elif WINDOWS_AVALONIA
        /* steam when Steam started the game, otherwise unbundled */
        private static string GetWindowsPackage()
        {
            try
            {
                string steamAppId = Environment.GetEnvironmentVariable("SteamAppId");
                if (!string.IsNullOrEmpty(steamAppId) && steamAppId != "0")
                    return "steam";
                return "unbundled";
            }
            catch (Exception)
            {
                return "unknown";
            }
        }
#endif
    }
}
#endif

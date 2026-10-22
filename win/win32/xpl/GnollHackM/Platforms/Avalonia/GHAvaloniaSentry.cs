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
                string dsn = GHApp.CurrentUserSecrets?.DefaultSentryDNS;
                if (string.IsNullOrEmpty(dsn))
                    return;

                SentrySdk.Init(options =>
                {
                    options.Dsn = dsn;
                    /* One scope shared by all threads, and release-health sessions, as Sentry.Maui sets them */
                    options.IsGlobalModeEnabled = true;
                    options.AutoSessionTracking = true;
                    options.CaptureFailedRequests = false;
#if DEBUG
                    options.Debug = true;
                    options.TracesSampleRate = 1.0;
#endif
                });

                string package = GetPackage();
                string container = GetContainer();
                string session = GetSessionType();
                SentrySdk.ConfigureScope(scope =>
                {
                    scope.SetTag(GHConstants.SentryTagLinuxPackage, package);
                    scope.SetTag(GHConstants.SentryTagLinuxContainer, container);
                    scope.SetTag(GHConstants.SentryTagLinuxSession, session);
                });
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaSentry: cannot initialize Sentry: " + ex.Message);
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

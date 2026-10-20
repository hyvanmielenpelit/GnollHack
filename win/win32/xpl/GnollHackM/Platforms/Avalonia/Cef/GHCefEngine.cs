#if GNH_AVALONIA && LINUX
using System;
using System.IO;
using Microsoft.Maui;

namespace GnollHackM
{
    /* The web view engine of the Avalonia build on Linux, chosen once at start-up: the environment variable
       GNOLLHACK_WEBVIEW (cef or native), else the preference AvaloniaWebViewEngine in developer mode, else CEF.
       CEF is chosen only when its files are next to the executable; when it then does not start at the first web
       view, web views use WebKitGTK instead (CreateWebViewHandler). References no CefGlue type, so it is safe to call
       anywhere; Decide does not use GHApp, which is not ready when it runs. */
    internal static class GHCefEngine
    {
        public const string EnvironmentVariableName = "GNOLLHACK_WEBVIEW";
        public const string PreferenceKey = "AvaloniaWebViewEngine";
        public const string CefChoice = "cef";
        public const string NativeChoice = "native";

        private static string _decisionReason = "GHCefEngine: no decision made";
        private static bool _fallbackLogged = false;

        /* libcef.so next to the executable */
        public static bool IsPayloadPresent
        {
            get { return File.Exists(Path.Combine(AppContext.BaseDirectory, "libcef.so")); }
        }

        /* Whether MAUI web views use CEF; set once by Decide */
        public static bool IsSelected { get; private set; }

        /* How Decide chose, for a later log line */
        public static string DecisionReason
        {
            get { return _decisionReason; }
        }

        /* Called from MauiProgram.CreateMauiApp after the Essentials are installed */
        public static void Decide()
        {
            try
            {
                string choice = CefChoice;
                string source;
                string environmentValue = Environment.GetEnvironmentVariable(EnvironmentVariableName);
                if (!string.IsNullOrWhiteSpace(environmentValue))
                {
                    choice = environmentValue.Trim().ToLowerInvariant();
                    source = "environment variable " + EnvironmentVariableName + "=" + environmentValue.Trim();
                }
                else if (Microsoft.Maui.Storage.Preferences.Get("DeveloperMode", GHConstants.DefaultDeveloperMode))
                {
                    string preferenceValue = Microsoft.Maui.Storage.Preferences.Get(PreferenceKey, "");
                    if (!string.IsNullOrWhiteSpace(preferenceValue))
                    {
                        choice = preferenceValue.Trim().ToLowerInvariant();
                        source = "preference " + PreferenceKey + "=" + preferenceValue.Trim();
                    }
                    else
                    {
                        source = "default (preference " + PreferenceKey + " not set)";
                    }
                }
                else
                {
                    source = "default (developer mode off)";
                }

                bool payloadPresent = IsPayloadPresent;
                IsSelected = choice == CefChoice && payloadPresent;

                string engine = IsSelected ? CefChoice : NativeChoice;
                string reason = "GHCefEngine: " + engine + " from " + source;
                if (choice == CefChoice && !payloadPresent)
                    reason += "; libcef.so is missing from " + AppContext.BaseDirectory;
                else if (choice != CefChoice && choice != NativeChoice)
                    reason += "; unrecognized value";
                _decisionReason = reason;
            }
            catch (Exception ex)
            {
                IsSelected = false;
                _decisionReason = "GHCefEngine: native, the choice could not be read: " + ex.Message;
            }
        }

        /* The MAUI WebView handler when CEF is selected: CEF's while CEF starts, else ACM's, which shows pages through
           WebKitGTK. A failed start is not retried, so after it every web view gets ACM's handler. */
        public static IElementHandler CreateWebViewHandler()
        {
            if (GHCefRuntime.EnsureInitialized())
                return new GHCefWebViewHandler();

            if (!_fallbackLogged)
            {
                _fallbackLogged = true;
                GnollHackX.GHApp.MaybeWriteGHLog("GHCefEngine: CEF did not start; web views use WebKitGTK");
            }
            return new Avalonia.Controls.Maui.Handlers.WebViewHandler();
        }
    }
}
#endif

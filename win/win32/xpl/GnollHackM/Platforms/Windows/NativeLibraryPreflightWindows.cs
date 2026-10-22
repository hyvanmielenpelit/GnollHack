#if WINDOWS && GNH_MAUI
using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.Win32;
#if SENTRY
using Sentry;
#endif

namespace GnollHackM
{
    /* Diagnoses a native library that fails to load. The runtime's DllNotFoundException and
       BadImageFormatException do not carry the Win32 error in HResult, so the file is loaded again
       directly to recover it, and the failure is reported to Sentry. A failure of the game library
       is fatal: the player gets a message box and the process exits. Every Windows Sentry event is
       tagged with the Smart App Control state. */
    internal static class NativeLibraryPreflightWindows
    {
        private const int ERROR_MOD_NOT_FOUND = 126;
        private const int ERROR_ACCESS_DISABLED_BY_POLICY = 1260;          /* AppLocker or Software Restriction Policies */
        private const int ERROR_SYSTEM_INTEGRITY_POLICY_VIOLATION = 4551;  /* Smart App Control or App Control for Business */
        private const uint LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR = 0x00000100;
        private const uint LOAD_LIBRARY_SEARCH_DEFAULT_DIRS = 0x00001000;
        private const uint SEM_FAILCRITICALERRORS = 0x0001;
        private const uint SEM_NOOPENFILEERRORBOX = 0x8000;
        private const uint MB_OK = 0x00000000;
        private const uint MB_ICONERROR = 0x00000010;
        private const uint MB_SETFOREGROUND = 0x00010000;
        private const string CodeIntegrityPolicyKey = @"SYSTEM\CurrentControlSet\Control\CI\Policy";
        private const string SmartAppControlStateValue = "VerifiedAndReputablePolicyState";
        private const string SmartAppControlStateTag = "windows.sac_state";

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr LoadLibraryExW(string lpLibFileName, IntPtr hFile, uint dwFlags);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FreeLibrary(IntPtr hLibModule);

        [DllImport("kernel32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool SetThreadErrorMode(uint dwNewMode, out uint lpOldMode);

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        private static extern int MessageBoxW(IntPtr hWnd, string lpText, string lpCaption, uint uType);

        private static readonly Lazy<string> _smartAppControlState = new Lazy<string>(ReadSmartAppControlState);

        /* "Off", "On", "Evaluation", "Unavailable", or the raw registry value */
        public static string SmartAppControlState => _smartAppControlState.Value;

        private static string ReadSmartAppControlState()
        {
            try
            {
                using (RegistryKey key = Registry.LocalMachine.OpenSubKey(CodeIntegrityPolicyKey, writable: false))
                {
                    object value = key?.GetValue(SmartAppControlStateValue);
                    if (!(value is int state))
                        return "Unavailable";
                    if (state == 0)
                        return "Off";
                    if (state == 1)
                        return "On";
                    if (state == 2)
                        return "Evaluation";
                    return state.ToString(CultureInfo.InvariantCulture);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return "Unavailable";
            }
        }

#if SENTRY
        /* Called from Sentry's BeforeSend; must never throw */
        public static void AddToEvent(SentryEvent sentryEvent)
        {
            if (sentryEvent == null)
                return;
            try
            {
                sentryEvent.SetTag(SmartAppControlStateTag, SmartAppControlState);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
#endif

        /* Win32 error from loading fileName out of the application folder, or 0 if it loads.
           The loader's own "Bad Image" box is suppressed, as the runtime does for its loads. */
        private static int ProbeLoadError(string fileName)
        {
            string path = Path.Combine(AppContext.BaseDirectory, fileName);
            if (!File.Exists(path))
                return ERROR_MOD_NOT_FOUND;
            uint oldMode;
            bool revertMode = SetThreadErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX, out oldMode);
            try
            {
                IntPtr module = LoadLibraryExW(path, IntPtr.Zero, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
                if (module == IntPtr.Zero)
                    return Marshal.GetLastPInvokeError();
                FreeLibrary(module);
                return 0;
            }
            finally
            {
                if (revertMode)
                    SetThreadErrorMode(oldMode, out _);
            }
        }

        /* Sentry may not be initialized this early; must never throw */
        private static void CaptureLoadFailure(Exception ex, string fileName, int win32Error)
        {
#if SENTRY
            try
            {
                string errorText = win32Error.ToString(CultureInfo.InvariantCulture);
                SentrySdk.CaptureException(ex, scope =>
                {
                    scope.SetFingerprint(new[] { "native-library-load-failure", fileName, errorText });
                    scope.SetTag("native_load.win32_error", errorText);
                    scope.SetTag("native_load.file", fileName);
                    scope.SetTag(SmartAppControlStateTag, SmartAppControlState);
                });
            }
            catch (Exception captureEx)
            {
                Debug.WriteLine(captureEx.Message);
            }
#endif
        }

        /* The game library failed to load. Returns only if the file itself loads, so that the caller
           rethrows; otherwise reports, explains and exits. */
        public static void HandleNativeLoadFailure(Exception ex)
        {
            int error = ProbeLoadError(PlatformConstants.dll);
            if (error == 0)
                return;

            CaptureLoadFailure(ex, PlatformConstants.dll, error);
#if SENTRY
            try
            {
                SentrySdk.Flush(TimeSpan.FromSeconds(3));
            }
            catch (Exception flushEx)
            {
                Debug.WriteLine(flushEx.Message);
            }
#endif
            /* No XAML window exists yet; MessageBoxW runs its own message loop */
            MessageBoxW(IntPtr.Zero, GetFailureMessage(error), "GnollHack", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
            Environment.Exit(1);
        }

        /* For libraries whose failure the app survives: reports the first of fileNames that does not
           load, and nothing if all load. Never throws. */
        public static void ReportNonFatalLoadFailure(Exception ex, params string[] fileNames)
        {
            if (fileNames == null)
                return;
            try
            {
                foreach (string fileName in fileNames)
                {
                    int error = ProbeLoadError(fileName);
                    if (error != 0)
                    {
                        CaptureLoadFailure(ex, fileName, error);
                        return;
                    }
                }
            }
            catch (Exception reportEx)
            {
                Debug.WriteLine(reportEx.Message);
            }
        }

        private static string GetFailureMessage(int error)
        {
            string library = PlatformConstants.dll;
            string reinstall = "Please reinstall GnollHack. If you installed it through Steam, you can also verify the integrity of its game files.";
            if (error == ERROR_SYSTEM_INTEGRITY_POLICY_VIOLATION || error == ERROR_ACCESS_DISABLED_BY_POLICY)
            {
                string text = "Windows blocked GnollHack's game library (" + library + ") because of an application control policy."
                    + "\n\nThis is usually Smart App Control (Windows Security > App & browser control) or a policy set by this computer's administrator.";
                if (SmartAppControlState == "On")
                    text += "\n\nSmart App Control is enabled on this computer.";
                text += "\n\nPlease update GnollHack to the latest version. If an organization manages this computer, contact its administrator."
                    + "\n\n(Windows error " + error.ToString(CultureInfo.InvariantCulture) + ")";
                return text;
            }
            if (error == ERROR_MOD_NOT_FOUND)
            {
                return "GnollHack could not find its game library (" + library + "). The installation appears to be incomplete."
                    + "\n\n" + reinstall;
            }
            return "GnollHack could not load its game library (" + library + ", Windows error " + error.ToString(CultureInfo.InvariantCulture) + ")."
                + "\n\n" + reinstall;
        }
    }
}
#endif

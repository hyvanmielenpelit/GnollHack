#if GNH_AVALONIA && LINUX
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using GnollHackX;
#if SENTRY
using Sentry;
#endif

namespace GnollHackM
{
    /* Handles a game library that fails to load on Linux: the failure is reported to Sentry, explained on standard
       error and in a dialog when a dialog tool is installed, and the process exits. No Avalonia window exists yet at
       that point. Also reads the system's glibc version for the linux.glibc tag. */
    internal static class NativeLibraryPreflightLinux
    {
        private const string ReasonGlibc = "glibc";
        private const string ReasonMissing = "missing";
        private const string ReasonOther = "other";

        [DllImport("libc.so.6")]
        private static extern IntPtr gnu_get_libc_version();

        private static readonly Lazy<string> _glibcVersion = new Lazy<string>(ReadGlibcVersion);

        /* "2.31", or "unknown" when not running on glibc */
        public static string GlibcVersion => _glibcVersion.Value;

        private static string ReadGlibcVersion()
        {
            try
            {
                string version = Marshal.PtrToStringAnsi(gnu_get_libc_version());
                return string.IsNullOrEmpty(version) ? "unknown" : version;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return "unknown";
            }
        }

        /* The game library failed to load. Reports, explains and exits; never returns. */
        public static void HandleNativeLoadFailure(Exception ex)
        {
            string message = ex?.Message ?? "";
            string reason = GetReason(message);
            CaptureLoadFailure(ex, reason);
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
            string text = GetFailureMessage(reason, message);
            try
            {
                Console.Error.WriteLine(text);
            }
            catch (Exception writeEx)
            {
                Debug.WriteLine(writeEx.Message);
            }
            ShowDialog(text);
            Environment.Exit(1);
        }

        private static string GetReason(string message)
        {
            if (message.Contains("version `GLIBC_", StringComparison.Ordinal))
                return ReasonGlibc;
            try
            {
                if (!File.Exists(Path.Combine(AppContext.BaseDirectory, PlatformConstants.dll)))
                    return ReasonMissing;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            return ReasonOther;
        }

        /* Sentry may not be initialized this early; must never throw */
        private static void CaptureLoadFailure(Exception ex, string reason)
        {
#if SENTRY
            try
            {
                string glibc = GlibcVersion;
                SentrySdk.CaptureException(ex, scope =>
                {
                    scope.SetFingerprint(new[] { "native-library-load-failure", PlatformConstants.dll, reason });
                    scope.SetTag("native_load.reason", reason);
                    scope.SetTag("native_load.file", PlatformConstants.dll);
                    scope.SetTag(GHConstants.SentryTagLinuxGlibc, glibc);
                });
            }
            catch (Exception captureEx)
            {
                Debug.WriteLine(captureEx.Message);
            }
#endif
        }

        private static string GetFailureMessage(string reason, string message)
        {
            string library = PlatformConstants.dll;
            string reinstall = "Please download GnollHack again. If you installed it through Steam, you can also verify the integrity of its game files.";
            if (reason == ReasonGlibc)
            {
                Match match = Regex.Match(message, @"GLIBC_(\d+(\.\d+)+)");
                string required = match.Success ? match.Groups[1].Value : "unknown";
                return "GnollHack needs a newer GNU C Library (glibc) than this system has."
                    + "\n\nIts game library (" + library + ") needs glibc " + required + " or newer, and this system has glibc " + GlibcVersion + "."
                    + "\n\nPlease update your Linux distribution to a newer release.";
            }
            if (reason == ReasonMissing)
            {
                return "GnollHack could not find its game library (" + library + "). The installation appears to be incomplete."
                    + "\n\n" + reinstall;
            }
            string text = "GnollHack could not load its game library (" + library + ").";
            string loaderLine = GetFirstLoaderLine(message);
            if (!string.IsNullOrEmpty(loaderLine))
                text += "\n\n" + loaderLine;
            return text + "\n\n" + reinstall;
        }

        /* The runtime's DllNotFoundException message is its own sentence, then one line per load attempt */
        private static string GetFirstLoaderLine(string message)
        {
            string[] lines = message.Split('\n', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
            return lines.Length > 1 ? lines[1] : "";
        }

        /* The first dialog tool found on PATH shows the text; nothing is shown without a display or a tool */
        internal static void ShowDialog(string text)
        {
            try
            {
                if (string.IsNullOrEmpty(Environment.GetEnvironmentVariable("DISPLAY"))
                    && string.IsNullOrEmpty(Environment.GetEnvironmentVariable("WAYLAND_DISPLAY")))
                    return;

                string[][] commands =
                {
                    new[] { "zenity", "--error", "--no-markup", "--title=GnollHack", "--text=" + text },
                    new[] { "kdialog", "--title", "GnollHack", "--error", text },
                    new[] { "xmessage", "-title", "GnollHack", "-center", text },
                };
                foreach (string[] command in commands)
                {
                    string path = FindOnPath(command[0]);
                    if (path == null)
                        continue;
                    try
                    {
                        ProcessStartInfo startInfo = new ProcessStartInfo(path) { UseShellExecute = false };
                        for (int i = 1; i < command.Length; i++)
                            startInfo.ArgumentList.Add(command[i]);
                        using (Process process = Process.Start(startInfo))
                        {
                            if (process == null)
                                continue;
                            process.WaitForExit();
                        }
                        return;
                    }
                    catch (Exception startEx)
                    {
                        Debug.WriteLine(startEx.Message);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static string FindOnPath(string fileName)
        {
            string pathVariable = Environment.GetEnvironmentVariable("PATH");
            if (string.IsNullOrEmpty(pathVariable))
                return null;
            foreach (string directory in pathVariable.Split(':', StringSplitOptions.RemoveEmptyEntries))
            {
                string candidate = Path.Combine(directory, fileName);
                if (File.Exists(candidate))
                    return candidate;
            }
            return null;
        }
    }
}
#endif

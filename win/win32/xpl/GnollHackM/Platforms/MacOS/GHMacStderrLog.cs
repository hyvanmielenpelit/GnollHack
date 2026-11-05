#if GNH_AVALONIA && MACOS
using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace GnollHackM
{
    /* Started from Finder, the Dock or Steam, the process has no terminal, and what it writes to standard output and
       error would be lost. Descriptors 1 and 2 then go to ~/Library/Logs/GnollHack/GnollHack.log, native writes
       included; the previous run's log is kept as GnollHack.previous.log. Started from a terminal, nothing changes. */
    internal static class GHMacStderrLog
    {
        private const string LogFileName = "GnollHack.log";
        private const string PreviousLogFileName = "GnollHack.previous.log";

        private const int StandardOutput = 1;
        private const int StandardError = 2;

        [DllImport("libc", SetLastError = true)]
        private static extern int isatty(int fd);

        /* open(2) is variadic, which arm64 macOS passes differently from a fixed P/Invoke signature, so the file is
           opened through FileStream and only its descriptor is duplicated */
        [DllImport("libc", SetLastError = true)]
        private static extern int dup2(int fd, int fd2);

        public static string LogPath { get; private set; }

        /* First thing in Main, before anything writes to standard error. Never throws. */
        public static void Install()
        {
            try
            {
                if (isatty(StandardError) == 1)
                    return;

                string home = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
                if (string.IsNullOrEmpty(home))
                    return;
                string folder = Path.Combine(home, "Library", "Logs", "GnollHack");
                Directory.CreateDirectory(folder);
                string path = Path.Combine(folder, LogFileName);
                if (File.Exists(path))
                    File.Move(path, Path.Combine(folder, PreviousLogFileName), true);

                FileStreamOptions options = new FileStreamOptions
                {
                    Mode = FileMode.Create,
                    Access = FileAccess.Write,
                    Share = FileShare.ReadWrite | FileShare.Delete,
                    UnixCreateMode = UnixFileMode.UserRead | UnixFileMode.UserWrite | UnixFileMode.GroupRead | UnixFileMode.OtherRead,
                };
                bool redirected;
                using (FileStream stream = new FileStream(path, options))
                {
                    int fd = (int)stream.SafeFileHandle.DangerousGetHandle();
                    redirected = dup2(fd, StandardError) >= 0;
                    if (redirected)
                        dup2(fd, StandardOutput);
                }
                if (!redirected)
                    return;

                LogPath = path;
                string version = Assembly.GetEntryAssembly()?.GetCustomAttribute<AssemblyInformationalVersionAttribute>()?.InformationalVersion ?? "unknown";
                Console.Error.WriteLine("GnollHack " + version + " started " + DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss") + " UTC, process "
                    + Environment.ProcessId);
            }
            catch (Exception)
            {
                /* Standard error stays where it was */
            }
        }
    }
}
#endif

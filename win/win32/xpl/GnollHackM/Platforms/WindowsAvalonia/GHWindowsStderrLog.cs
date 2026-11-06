#if GNH_AVALONIA && WINDOWS_AVALONIA
using System;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.Win32.SafeHandles;

namespace GnollHackM
{
    /* The executable is a GUI program, so its standard output and error go nowhere unless the process was given a
       console. Without one, both standard handles go to Logs\GnollHack.log in the data directory, native writes
       included; the previous run's log is kept as GnollHack.previous.log. A log that another process is still
       writing, as when a second instance starts, is left alone, and so are the standard handles. */
    internal static class GHWindowsStderrLog
    {
        private const string LogFolderName = "Logs";
        private const string LogFileName = "GnollHack.log";
        private const string PreviousLogFileName = "GnollHack.previous.log";

        private const int STD_OUTPUT_HANDLE = -11;
        private const int STD_ERROR_HANDLE = -12;
        private const uint FILE_APPEND_DATA = 0x00000004;
        private const uint SYNCHRONIZE = 0x00100000;
        private const uint FILE_SHARE_READ = 0x00000001;
        private const uint FILE_SHARE_WRITE = 0x00000002;
        private const uint FILE_SHARE_DELETE = 0x00000004;
        private const uint OPEN_ALWAYS = 4;
        private const uint FILE_ATTRIBUTE_NORMAL = 0x00000080;
        private const int SharingViolationHResult = unchecked((int)0x80070020); /* ERROR_SHARING_VIOLATION */

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetConsoleWindow();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool SetStdHandle(int nStdHandle, IntPtr hHandle);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
        private static extern SafeFileHandle CreateFileW(string lpFileName, uint dwDesiredAccess, uint dwShareMode,
            IntPtr lpSecurityAttributes, uint dwCreationDisposition, uint dwFlagsAndAttributes, IntPtr hTemplateFile);

        /* Open for the life of the process: the standard handles refer to it */
        private static SafeFileHandle _logHandle = null;

        public static string LogPath { get; private set; }

        /* First thing in Main, before gnollhackwin.dll loads: the engine links the static CRT, which takes the
           process's standard handles when it loads. Never throws. */
        public static void Install()
        {
            try
            {
                if (GetConsoleWindow() != IntPtr.Zero)
                    return;

                string folder = Path.Combine(GHAvaloniaPaths.DataDirectory, LogFolderName);
                Directory.CreateDirectory(folder);
                string path = Path.Combine(folder, LogFileName);
                if (File.Exists(path))
                {
                    if (IsWrittenByAnotherProcess(path))
                        return;
                    try
                    {
                        File.Move(path, Path.Combine(folder, PreviousLogFileName), true);
                    }
                    catch (Exception)
                    {
                        /* This run appends to the existing log */
                    }
                }

                /* Append-only access: every write, from managed or native code, goes to the end of the file */
                SafeFileHandle handle = CreateFileW(path, FILE_APPEND_DATA | SYNCHRONIZE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, IntPtr.Zero, OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, IntPtr.Zero);
                if (handle.IsInvalid)
                {
                    handle.Dispose();
                    return;
                }
                IntPtr rawHandle = handle.DangerousGetHandle();
                if (!SetStdHandle(STD_ERROR_HANDLE, rawHandle))
                {
                    handle.Dispose();
                    return;
                }
                SetStdHandle(STD_OUTPUT_HANDLE, rawHandle);
                _logHandle = handle;

                /* Console.OpenStandardError writes through the standard handle with no file offset of its own */
                StreamWriter writer = new StreamWriter(Console.OpenStandardError(), new UTF8Encoding(false));
                writer.AutoFlush = true;
                TextWriter synchronizedWriter = TextWriter.Synchronized(writer);
                Console.SetOut(synchronizedWriter);
                Console.SetError(synchronizedWriter);

                LogPath = path;
                string version = Assembly.GetEntryAssembly()?.GetCustomAttribute<AssemblyInformationalVersionAttribute>()
                    ?.InformationalVersion ?? "unknown";
                Console.Error.WriteLine("GnollHack " + version + " started "
                    + DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture)
                    + " UTC, process " + Environment.ProcessId);
            }
            catch (Exception)
            {
                /* Standard output and error stay where they were */
            }
        }

        /* True when another handle has the file open for writing, which a read that denies writers then fails on */
        private static bool IsWrittenByAnotherProcess(string path)
        {
            try
            {
                using (new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read | FileShare.Delete))
                {
                }
                return false;
            }
            catch (IOException ex) when (ex.HResult == SharingViolationHResult)
            {
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}
#endif

#if GNH_AVALONIA && LINUX
using System;
using System.Globalization;
using System.Runtime.CompilerServices;
using Xilium.CefGlue;

namespace GnollHackM
{
    /* CEF's helper processes (renderer, GPU, utility) re-enter our executable with --type=<kind>. Main runs this before
       anything else, so it never touches GHApp, GHAvaloniaPaths, Avalonia, MAUI or Preferences, and logs only to
       standard error. */
    internal static class GHCefSubprocess
    {
        /* Added to every child's command line by the browser process */
        public const string ParentPidSwitch = "gnollhack-parent-pid";

        /* After the parent exits; a normal CEF shutdown ends the child well within it */
        private const int ParentExitGraceMs = 3000;

        internal static bool IsSubprocess(string[] args)
        {
            if (args == null)
                return false;

            foreach (string arg in args)
            {
                if (arg != null && arg.StartsWith("--type=", StringComparison.Ordinal))
                    return true;
            }
            return false;
        }

        /* Blocks until CEF ends the process's work; returns its exit code */
        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static int Run(string[] args)
        {
            try
            {
                /* CEF parses argv, whose first element is the program; .NET's args omit it */
                string[] argv = new string[args.Length + 1];
                argv[0] = Environment.ProcessPath ?? "GnollHackM";
                Array.Copy(args, 0, argv, 1, args.Length);

                StartParentWatchdog(args);
                return ExecuteProcess(argv);
            }
            catch (Exception ex)
            {
                WriteError("GHCefSubprocess: " + ex);
                return 1;
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static int ExecuteProcess(string[] argv)
        {
            CefRuntime.Load();
            GHCefApp app = new GHCefApp(false);
            int exitCode = CefRuntime.ExecuteProcess(new CefMainArgs(argv), app, IntPtr.Zero);
            GC.KeepAlive(app);
            return exitCode < 0 ? 0 : exitCode;
        }

        /* Ends this process 3 s after the browser process exits, so a crashed game leaves no CEF processes behind */
        private static void StartParentWatchdog(string[] args)
        {
            string prefix = "--" + ParentPidSwitch + "=";
            int parentPid = 0;
            foreach (string arg in args)
            {
                if (arg != null && arg.StartsWith(prefix, StringComparison.Ordinal))
                {
                    int.TryParse(arg.Substring(prefix.Length), NumberStyles.Integer, CultureInfo.InvariantCulture, out parentPid);
                    break;
                }
            }

            if (parentPid <= 0)
            {
                WriteError("GHCefSubprocess: no --" + ParentPidSwitch + "; parent watchdog not started");
                return;
            }

            System.Threading.Thread thread = new System.Threading.Thread(() => WatchParent(parentPid));
            thread.Name = "GHCefParentWatchdog";
            thread.IsBackground = true;
            thread.Start();
        }

        private static void WatchParent(int parentPid)
        {
            try
            {
                try
                {
                    using (System.Diagnostics.Process parent = System.Diagnostics.Process.GetProcessById(parentPid))
                    {
                        parent.WaitForExit();
                    }
                }
                catch (ArgumentException)
                {
                    /* The parent is already gone */
                }

                System.Threading.Thread.Sleep(ParentExitGraceMs);
                Environment.Exit(0);
            }
            catch (Exception ex)
            {
                WriteError("GHCefSubprocess: parent watchdog failed: " + ex.Message);
            }
        }

        internal static void WriteError(string message)
        {
            try
            {
                Console.Error.WriteLine(message);
            }
            catch
            {
                /* Standard error may be closed */
            }
        }
    }
}
#endif

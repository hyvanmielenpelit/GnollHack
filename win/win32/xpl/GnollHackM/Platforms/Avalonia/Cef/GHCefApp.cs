#if GNH_AVALONIA && LINUX
using System;
using System.Globalization;
using System.Runtime.CompilerServices;
using Xilium.CefGlue;

namespace GnollHackM
{
    /* The CefApp of the browser process (the game) and of CEF's helper processes. Only the browser process has a
       browser-process handler, so code that runs in a helper never reaches GHCefRuntime, Avalonia or GHApp. */
    internal sealed class GHCefApp : CefApp
    {
        private readonly bool _isBrowserProcess;
        private readonly GHCefBrowserProcessHandler _browserProcessHandler;
        private readonly GHCefRenderProcessHandler _renderProcessHandler;

        public GHCefApp(bool isBrowserProcess)
        {
            _isBrowserProcess = isBrowserProcess;
            _renderProcessHandler = new GHCefRenderProcessHandler();
            if (isBrowserProcess)
                _browserProcessHandler = new GHCefBrowserProcessHandler();
        }

        // WORKAROUND(WA-LINUX-0003): Chromium's sandbox cannot run in Steam's container
        /* Switches of the browser process, which CEF passes on to its helpers as needed. No zygote and no sandbox,
           which cannot work in Steam's container; no GPU, since the package has no ANGLE and pages are painted on the
           CPU for off-screen rendering. A switch with a value uses the two-argument form: a single "name=value" string
           would become a switch of that whole name. */
        protected override void OnBeforeCommandLineProcessing(string processType, CefCommandLine commandLine)
        {
            try
            {
                if (!string.IsNullOrEmpty(processType))
                    return;

                commandLine.AppendSwitch("no-zygote");
                commandLine.AppendSwitch("disable-gpu");
                commandLine.AppendSwitch("disable-gpu-compositing");
                commandLine.AppendSwitch("disable-component-update");
                commandLine.AppendSwitch("disable-background-networking");
                commandLine.AppendSwitch("ozone-platform", "x11");
                commandLine.AppendSwitch("password-store", "basic");
            }
            catch (Exception ex)
            {
                LogError("GHCefApp: cannot set the command line switches: " + ex.Message);
            }
        }

        protected override CefBrowserProcessHandler GetBrowserProcessHandler()
        {
            return _browserProcessHandler;
        }

        protected override CefRenderProcessHandler GetRenderProcessHandler()
        {
            return _renderProcessHandler;
        }

        private void LogError(string message)
        {
            if (_isBrowserProcess)
                GHCefBrowserProcessHandler.Log(message);
            else
                GHCefSubprocess.WriteError(message);
        }
    }

    /* Browser process only: drives the external message pump and passes our process id to every child */
    internal sealed class GHCefBrowserProcessHandler : CefBrowserProcessHandler
    {
        /* Any thread */
        protected override void OnScheduleMessagePumpWork(long delayMs)
        {
            try
            {
                GHCefRuntime.OnScheduleMessagePumpWork(delayMs);
            }
            catch (Exception ex)
            {
                Log("GHCefBrowserProcessHandler: cannot schedule message pump work: " + ex.Message);
            }
        }

        /* The UI thread for a renderer, the IO thread for the GPU process */
        protected override void OnBeforeChildProcessLaunch(CefCommandLine commandLine)
        {
            try
            {
                commandLine.AppendSwitch(GHCefSubprocess.ParentPidSwitch, Environment.ProcessId.ToString(CultureInfo.InvariantCulture));
            }
            catch (Exception ex)
            {
                Log("GHCefBrowserProcessHandler: cannot add the parent process id: " + ex.Message);
            }
        }

        /* A second instance started with the same cache path; CEF's default would open a browser window in this one */
        protected override bool OnAlreadyRunningAppRelaunch(CefCommandLine commandLine, string currentDirectory)
        {
            try
            {
                Log("GHCefBrowserProcessHandler: another instance tried to start CEF with the same cache path; ignored");
            }
            catch
            {
                /* Logging must not fail the callback */
            }
            return true;
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static void Log(string message)
        {
            GHCefRuntime.Log(message);
        }
    }
}
#endif

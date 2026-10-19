#if GNH_AVALONIA && LINUX
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Loader;
using System.Text;
using GnollHackX;
using Xilium.CefGlue;

namespace GnollHackM
{
    /* CEF in the game process: loaded and initialized lazily, when the first CEF web view needs a browser, because
       CefRuntime's static constructor installs a process-wide native library resolver that scans the disk on every
       miss. CEF's UI-thread work runs on the Avalonia UI thread through the external message pump, so every client
       callback runs there inside DoMessageLoopWork. UI thread unless noted. The static fields create no CefGlue
       objects, so a session without CEF never runs CefRuntime's static constructor. */
    internal static class GHCefRuntime
    {
        /* Longest wait between pump passes, as in CEF's MainMessageLoopExternalPump */
        private const long MaxPumpDelayMs = 33;
        private const int ShutdownWaitMs = 2000;
        private const int ShutdownPollMs = 10;
        private static readonly TimeSpan StatsInterval = TimeSpan.FromSeconds(10);

        private static volatile bool _initialized = false;
        private static volatile bool _failed = false;
        private static volatile bool _shutDown = false;
        private static GHCefApp _app = null;

        /* Message pump */
        private static readonly Action _scheduleImmediate = ScheduleImmediate;
        private static readonly Action _onPumpTimer = OnPumpTimer;
        private static int _immediatePosted = 0;
        private static IDisposable _pumpTimer = null;
        private static bool _pumpActive = false;
        private static bool _pumpReentered = false;

        /* Live controls; a control registers after creating its browser and unregisters when it has closed */
        private static readonly object _registryLock = new object();
        private static readonly HashSet<GHCefWebViewControl> _controls = new HashSet<GHCefWebViewControl>();
        private static IDisposable _statsTimer = null;

        /* Native library resolution misses after CEF has loaded */
        [ThreadStatic]
        private static long _resolveStartTimestamp;
        [ThreadStatic]
        private static bool _inResolverLog;

        internal static bool IsInitialized
        {
            get { return _initialized; }
        }

        internal static bool HasFailed
        {
            get { return _failed; }
        }

        /* Loads and initializes CEF once; false when that failed now or before, or after shutdown */
        internal static bool EnsureInitialized()
        {
            if (_initialized)
                return true;
            if (_failed || _shutDown)
                return false;

            Log(GHCefEngine.DecisionReason);
            Stopwatch stopwatch = Stopwatch.StartNew();
            try
            {
                LoadAndInitialize();
                _initialized = true;
                Log("GHCefRuntime: CEF " + GetChromeVersion() + " initialized in " + stopwatch.ElapsedMilliseconds + " ms");
                return true;
            }
            catch (Exception ex)
            {
                _failed = true;
                Log("GHCefRuntime: CEF initialization failed after " + stopwatch.ElapsedMilliseconds + " ms: " + ex.GetType().Name + ": " + ex.Message);
                return false;
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static void LoadAndInitialize()
        {
            string cacheDirectory = Path.Combine(GHAvaloniaPaths.DataDirectory, "cef");
            Directory.CreateDirectory(cacheDirectory);

            /* Before CefRuntime's resolver, which the first use of CefRuntime installs; the second handler follows it */
            AssemblyLoadContext.Default.ResolvingUnmanagedDll += OnResolvingUnmanagedDllFirst;
            try
            {
                CefRuntime.Load();
            }
            finally
            {
                AssemblyLoadContext.Default.ResolvingUnmanagedDll += OnResolvingUnmanagedDllLast;
            }

            CefSettings settings = new CefSettings();
            settings.WindowlessRenderingEnabled = true;
            settings.MultiThreadedMessageLoop = false;
            settings.ExternalMessagePump = true;
            settings.NoSandbox = true;
            settings.DisableSignalHandlers = true;
            /* A unique root; the cache path must equal it or be a child of it */
            settings.RootCachePath = cacheDirectory;
            settings.CachePath = cacheDirectory;
            /* ICU data and the V8 snapshot load from libcef.so's directory; the paks from here */
            settings.ResourcesDirPath = AppContext.BaseDirectory;
            settings.LocalesDirPath = Path.Combine(AppContext.BaseDirectory, "locales");
            settings.LogFile = Path.Combine(cacheDirectory, "cef_debug.log");
            settings.LogSeverity = GHApp.DebugLogMessages ? CefLogSeverity.Info : CefLogSeverity.Warning;
            settings.BackgroundColor = new CefColor(255, 0x12, 0x12, 0x12);
            settings.PersistSessionCookies = false;

            /* argv[0] must not be empty */
            string[] argv = new string[] { Environment.ProcessPath ?? "GnollHackM", "--disable-in-process-stack-traces" };
            GHCefApp app = new GHCefApp(true);
            _app = app;
            CefRuntime.Initialize(new CefMainArgs(argv), settings, app, IntPtr.Zero);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static string GetChromeVersion()
        {
            try
            {
                return CefRuntime.ChromeVersion;
            }
            catch
            {
                return "(unknown version)";
            }
        }

        /* Any thread, from GHCefBrowserProcessHandler. Immediate requests are coalesced into one pending post. */
        internal static void OnScheduleMessagePumpWork(long delayMs)
        {
            if (_shutDown)
                return;

            if (delayMs <= 0)
            {
                if (System.Threading.Interlocked.Exchange(ref _immediatePosted, 1) != 0)
                    return;
                Avalonia.Threading.Dispatcher.UIThread.Post(_scheduleImmediate, Avalonia.Threading.DispatcherPriority.Default);
            }
            else
            {
                Avalonia.Threading.Dispatcher.UIThread.Post(() => Schedule(delayMs), Avalonia.Threading.DispatcherPriority.Default);
            }
        }

        private static void ScheduleImmediate()
        {
            System.Threading.Interlocked.Exchange(ref _immediatePosted, 0);
            Schedule(0);
        }

        private static void Schedule(long delayMs)
        {
            try
            {
                if (_shutDown || !_initialized)
                    return;

                StopPumpTimer();
                if (delayMs <= 0)
                    DoWork();
                else
                    _pumpTimer = Avalonia.Threading.DispatcherTimer.RunOnce(_onPumpTimer, TimeSpan.FromMilliseconds(Math.Min(delayMs, MaxPumpDelayMs)), Avalonia.Threading.DispatcherPriority.Default);
            }
            catch (Exception ex)
            {
                Log("GHCefRuntime: cannot schedule message pump work: " + ex.Message);
            }
        }

        private static void OnPumpTimer()
        {
            _pumpTimer = null;
            DoWork();
        }

        private static void StopPumpTimer()
        {
            IDisposable timer = _pumpTimer;
            _pumpTimer = null;
            if (timer != null)
                timer.Dispose();
        }

        /* One CEF pass, never re-entrantly; afterwards a safety pass is always pending */
        private static void DoWork()
        {
            try
            {
                if (_shutDown || !_initialized)
                    return;

                if (_pumpActive)
                {
                    _pumpReentered = true;
                    return;
                }

                _pumpReentered = false;
                _pumpActive = true;
                try
                {
                    DoMessageLoopWork();
                }
                finally
                {
                    _pumpActive = false;
                }

                if (_pumpReentered)
                {
                    _pumpReentered = false;
                    OnScheduleMessagePumpWork(0);
                }
                else if (_pumpTimer == null && !_shutDown)
                {
                    _pumpTimer = Avalonia.Threading.DispatcherTimer.RunOnce(_onPumpTimer, TimeSpan.FromMilliseconds(MaxPumpDelayMs), Avalonia.Threading.DispatcherPriority.Default);
                }
            }
            catch (Exception ex)
            {
                Log("GHCefRuntime: message pump work failed: " + ex.Message);
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static void DoMessageLoopWork()
        {
            CefRuntime.DoMessageLoopWork();
        }

        internal static void Register(GHCefWebViewControl control)
        {
            if (control == null)
                return;

            lock (_registryLock)
            {
                _controls.Add(control);
            }
            StartStatsTimer();
        }

        internal static void Unregister(GHCefWebViewControl control)
        {
            if (control == null)
                return;

            lock (_registryLock)
            {
                _controls.Remove(control);
            }
        }

        private static GHCefWebViewControl[] GetControls()
        {
            lock (_registryLock)
            {
                GHCefWebViewControl[] controls = new GHCefWebViewControl[_controls.Count];
                _controls.CopyTo(controls);
                return controls;
            }
        }

        /* Asks every live browser to close; each control unregisters when its browser has closed */
        internal static void CloseAllBrowsers()
        {
            foreach (GHCefWebViewControl control in GetControls())
            {
                try
                {
                    control.CloseBrowser();
                }
                catch (Exception ex)
                {
                    Log("GHCefRuntime: cannot close a browser: " + ex.Message);
                }
            }
        }

        /* From the window's Closed handler */
        [MethodImpl(MethodImplOptions.NoInlining)]
        public static void CloseAllBrowsersIfInitialized()
        {
            if (!_initialized || _shutDown)
                return;

            CloseAllBrowsers();
        }

        /* Once, from Main on the main thread after the Avalonia lifetime has ended. The dispatcher has shut down and
           drops posts, so this pumps CEF directly; MAUI may be torn down, so it logs only to standard error. CEF's
           Shutdown requires every browser closed: when they do not all close in time, Shutdown is skipped, and the
           process exits right after. */
        [MethodImpl(MethodImplOptions.NoInlining)]
        public static void ShutdownIfInitialized()
        {
            if (!_initialized)
                return;

            try
            {
                Shutdown();
            }
            catch (Exception ex)
            {
                WriteError("GHCefRuntime: shutdown failed: " + ex);
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static void Shutdown()
        {
            _shutDown = true;
            WriteError("GHCefRuntime: shutting down CEF");
            CloseAllBrowsers();

            /* A control stays registered until its browser has closed, including one still being created, which
               closes as soon as it exists */
            Stopwatch stopwatch = Stopwatch.StartNew();
            bool allClosed = false;
            int liveCount = 0;
            while (true)
            {
                try
                {
                    CefRuntime.DoMessageLoopWork();
                }
                catch (Exception ex)
                {
                    WriteError("GHCefRuntime: message pump work failed during shutdown: " + ex.Message);
                }

                GHCefWebViewControl[] controls = GetControls();
                liveCount = 0;
                foreach (GHCefWebViewControl control in controls)
                {
                    if (control.HasBrowser)
                        liveCount++;
                }

                if (controls.Length == 0)
                {
                    allClosed = true;
                    break;
                }

                if (stopwatch.ElapsedMilliseconds >= ShutdownWaitMs)
                    break;

                System.Threading.Thread.Sleep(ShutdownPollMs);
            }

            if (!allClosed)
            {
                WriteError("GHCefRuntime: " + liveCount + " browser(s) still open after " + stopwatch.ElapsedMilliseconds + " ms; CEF shutdown skipped");
                return;
            }

            WriteError("GHCefRuntime: browsers closed in " + stopwatch.ElapsedMilliseconds + " ms");
            try
            {
                CefRuntime.Shutdown();
                WriteError("GHCefRuntime: CEF shut down");
            }
            catch (Exception ex)
            {
                WriteError("GHCefRuntime: CEF shutdown failed: " + ex);
            }
        }

        /* Every 10 s while a control is registered and debug logging is on: live browsers and each control's paint
           statistics */
        private static void StartStatsTimer()
        {
            try
            {
                if (_statsTimer != null || _shutDown || !GHApp.DebugLogMessages)
                    return;

                _statsTimer = Avalonia.Threading.DispatcherTimer.Run(OnStatsTimer, StatsInterval, Avalonia.Threading.DispatcherPriority.Background);
            }
            catch (Exception ex)
            {
                Log("GHCefRuntime: cannot start the statistics timer: " + ex.Message);
            }
        }

        private static bool OnStatsTimer()
        {
            try
            {
                GHCefWebViewControl[] controls = GetControls();
                if (controls.Length == 0 || _shutDown || !GHApp.DebugLogMessages)
                {
                    _statsTimer = null;
                    return false;
                }

                int liveCount = 0;
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < controls.Length; i++)
                {
                    GHCefWebViewControl control = controls[i];
                    if (control.HasBrowser)
                        liveCount++;
                    sb.Append("; [");
                    sb.Append(i);
                    sb.Append("] ");
                    sb.Append(control.GetStatsAndReset());
                }
                Log("GHCefRuntime: " + liveCount + " live browser(s)" + sb.ToString());
                return true;
            }
            catch (Exception ex)
            {
                Log("GHCefRuntime: statistics failed: " + ex.Message);
                _statsTimer = null;
                return false;
            }
        }

        /* Runs before CefRuntime's resolver, which scans the application directory and up to five parents */
        private static IntPtr OnResolvingUnmanagedDllFirst(Assembly assembly, string libraryName)
        {
            _resolveStartTimestamp = Stopwatch.GetTimestamp();
            return IntPtr.Zero;
        }

        /* Runs after CefRuntime's resolver, so reached only when it found nothing either */
        private static IntPtr OnResolvingUnmanagedDllLast(Assembly assembly, string libraryName)
        {
            if (_inResolverLog)
                return IntPtr.Zero;

            _inResolverLog = true;
            try
            {
                long start = _resolveStartTimestamp;
                double elapsedMs = start != 0 ? (Stopwatch.GetTimestamp() - start) * 1000.0 / Stopwatch.Frequency : -1.0;
                string message = "GHCefRuntime: native library '" + libraryName + "' unresolved"
                    + (assembly != null ? " for " + assembly.GetName().Name : "")
                    + "; resolver scan " + elapsedMs.ToString("0.0", System.Globalization.CultureInfo.InvariantCulture) + " ms";
                Log(message);
            }
            catch
            {
                /* A resolver must not throw */
            }
            finally
            {
                _resolveStartTimestamp = 0;
                _inResolverLog = false;
            }
            return IntPtr.Zero;
        }

        /* GHApp's log, or standard error once shutdown has begun */
        internal static void Log(string message)
        {
            if (_shutDown)
            {
                WriteError(message);
                return;
            }

            try
            {
                GHApp.MaybeWriteGHLog(message);
            }
            catch
            {
                WriteError(message);
            }
        }

        private static void WriteError(string message)
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

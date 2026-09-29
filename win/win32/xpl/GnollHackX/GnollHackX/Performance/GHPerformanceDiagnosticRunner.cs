using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackM;
using Microsoft.Maui.Controls;
#else
using Xamarin.Forms;
using GnollHackX.Pages.Game;
using GnollHackX.Pages.MainScreen;
#endif

namespace GnollHackX.Performance
{
    /* The in-game performance test behind Menu > Developer > Test Performance: measures
       the live game map for WindowSeconds, collects every external factor the app can
       see, and opens a plain-text diagnosis (GHPerformanceDiagnosis) naming the most
       likely cause and where it lies.

       RunAsync runs on the UI thread as one async task:
         1. Refuses on the host page when the frame timeline is off, a test, suite or
            measurement window is already running, a window command is pending, the game
            has ended, or the platform render loop is off.
         2. Asks for confirmation on the host page. On OK: holds the background load
            sampler (so the pre-window has samples), starts the countdown and closes
            the menu.
         3. Hides the frame marker (GHFrameMarker.Suppressed) until the window is saved.
            Settles for SettleSeconds, so the menu's pause mark and collection fall
            outside the window; starts the per-process interval on the thread pool,
            waits for its begin collect at most ProcessIntervalBeginWaitMs, captures the
            scene (level, zoom, map font), then opens the measurement window, which on
            Windows also starts the render adapter probe
            (GHPerformanceRunRecord.TryBeginWindow). A window the window gate refuses
            aborts the test with "another measurement is in progress".
         4. Measures for WindowSeconds, polling every 100 ms for an abort: the app went
            to the background, a page opened over the game page, a menu or window
            opened over the map during the window, or the game ended. An aborted
            window is discarded unsaved and no report is written; the report retention
            still runs, deleting folders left by a test the app was killed during.
         5. Saves the window into ReportsDirectory/<stamp>/, which on Windows also ends
            the render adapter probe, and takes the window's allocation rate; reads what
            must be read on the UI thread (GPU context, recent hitches), then on the
            thread pool reads the probe's last result and the GPU preference and
            re-captures the environment fingerprint.
         6. Builds GHDiagnosisFacts, diagnoses, writes
            ReportsDirectory/perftest_<stamp>.txt, keeps the newest MaxKeptReports
            reports and this test's folder, and opens the report page.
       Refusals before the running flag is taken are awaited on the host page. The
       test's final message, if any, including a refusal after the confirmation, is
       shown only after the running flag is released, on the page at the top of the
       modal stack (the game page when none), and is not awaited, so a popup the player
       cannot see never holds the flag. RunAsync never throws.
       The pure parts are in GHPerformanceDiagnosticSupport.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHPerformanceDiagnosticRunner
    {
        public const int SettleSeconds = 3;
        public const int WindowSeconds = 30;
        public const int MaxKeptReports = 30;
        public const string ReportFilePrefix = "perftest_";
        public const string ReportFileExtension = ".txt";
        public const string StampFormat = "yyyyMMdd_HHmmss";

        private const string ReportsFolderName = "diagnostics";
        private const string ScenarioName = "diagnostic";
        private const string ArmName = "current";
        private const string MessageTitle = "Performance Test";
        private const string ReportPageTitle = "Performance Test";
        private const string ConfirmText = "Performance test: the menu closes and the game is measured for about 35 s. "
            + "A countdown shows at the top. Please do not touch the game until the report opens.";
        private const int PollMs = 100;
        private const int BeginPollMs = 16;

        private static int _isRunning = 0;

        public static bool IsRunning { get { return Interlocked.CompareExchange(ref _isRunning, 0, 0) != 0; } }

        /* GHPath/performance/diagnostics: the reports and, per test, a <stamp> folder with
           the window's run record. Persistent, not cleaned at start. */
        public static string ReportsDirectory
        {
            get { return Path.Combine(GHApp.GHPath, GHConstants.PerformanceDirectory, ReportsFolderName); }
        }

        /* Call on the UI thread. hostForMessages shows the refusals and the confirmation;
           it is popped when it is on top of the game page. Returns once the test has
           ended; its final message may still be showing. Never throws. */
        public static async Task RunAsync(GamePage gamePage, Page hostForMessages)
        {
            Page host = hostForMessages != null ? hostForMessages : gamePage;
            try
            {
                string refusal = IsRunning ? "A performance test is already running." : CheckPreconditions(gamePage);
                if (refusal != null)
                {
                    await ShowMessageAsync(host, refusal);
                    return;
                }
                if (Interlocked.CompareExchange(ref _isRunning, 1, 0) != 0)
                {
                    await ShowMessageAsync(host, "A performance test is already running.");
                    return;
                }
                string message = null;
                try
                {
                    message = await RunTestAsync(gamePage, host);
                }
                finally
                {
                    GHDiagnosticCountdown.Stop();
                    Interlocked.Exchange(ref _isRunning, 0);
                }
                /* The host while the menu is still open; else the game page or whatever
                   opened over it */
                if (message != null)
                {
                    Page top = GHApp.PageFromTopOfModalNavigationStack();
                    _ = ShowMessageAsync(top != null ? top : gamePage, message);
                }
            }
            catch (Exception ex)
            {
                Log("failed: " + ex.Message);
            }
        }

        /* A short user-facing refusal, or null when the test can start. This test's own
           IsRunning flag is checked by the caller. */
        private static string CheckPreconditions(GamePage gamePage)
        {
            if (gamePage == null)
                return "The game is not available.";
            if (!FrameTimeProfiler.IsEnabled)
                return "The performance test needs the frame timeline. Turn on Settings > Frame Time Profiler first.";
            if (GHPerformanceSuiteRunner.IsRunning)
                return "A performance suite is running.";
            if (GHPerformanceRunRecord.IsWindowOpen || GHPerformanceRunRecord.IsWindowCommandPending)
                return "Another performance measurement is running.";
            if (gamePage.GameEnded || !GHApp.GameStarted)
                return "The game has ended.";
            if (!GHApp.UsePlatformRenderLoop)
                return "Frame timing requires the platform render loop (Settings).";
            return null;
        }

        /* The confirmation and the test. Returns the final message for the caller to show,
           or null for none; shows no message itself. */
        private static async Task<string> RunTestAsync(GamePage gamePage, Page host)
        {
            bool confirmed = await GHApp.DisplayMessageBox(host, MessageTitle, ConfirmText, "Start", "Cancel");
            if (!confirmed)
                return null;
            /* Anything may have changed while the confirmation was shown */
            string refusal = CheckPreconditions(gamePage);
            if (refusal != null)
                return refusal;

            TestState s = new TestState();
            s.GamePage = gamePage;
            try
            {
                bool completed = false;
                string abortReason = null;
                try
                {
                    string root = Path.Combine(GHApp.GHPath, GHConstants.PerformanceDirectory);
                    if (!Directory.Exists(root))
                        GHApp.CheckCreateDirectory(root);
                    string reportsDirectory = ReportsDirectory;
                    if (!Directory.Exists(reportsDirectory))
                        GHApp.CheckCreateDirectory(reportsDirectory);

                    /* Primes rate counters such as the Windows processor performance
                       counter, so the window's first reading covers the settle */
                    GHThermalProbe.Read();
                    if (GHSystemLoadSampler.Enabled)
                    {
                        GHSystemLoadSampler.Acquire();
                        s.SamplerHeld = true;
                    }
                    /* No window end yet: the settle text stays until the window opens */
                    long now = DateTime.UtcNow.Ticks;
                    long settleEnd = now + SettleSeconds * TimeSpan.TicksPerSecond;
                    GHDiagnosticCountdown.Start(settleEnd, 0);

                    /* As the menu's Back to Game closes it */
                    FrameTimeProfiler.MarkPauseEvent();
                    GHApp.CollectNursery();
                    if (host != gamePage && GHApp.PageFromTopOfModalNavigationStack() == host)
                        await GHApp.PopModalPageAsync();

                    await MeasureAsync(s);
                    completed = true;
                }
                catch (DiagnosticAbortException ex)
                {
                    abortReason = ex.Reason;
                }
                catch (Exception ex)
                {
                    abortReason = string.IsNullOrEmpty(ex.Message) ? ex.GetType().Name : ex.Message;
                    Log("measurement failed: " + abortReason);
                }
                finally
                {
                    if (!completed)
                        DiscardOpenWindow(s);
                }

                if (!completed)
                {
                    GHDiagnosticCountdown.Stop();
                    ReleaseSampler(s);
                    string reportsDirectory = ReportsDirectory;
                    await Task.Run(delegate { ApplyRetention(reportsDirectory, null); });
                    Log("cancelled: " + (abortReason ?? "unknown reason"));
                    return "Performance test cancelled"
                        + (string.IsNullOrEmpty(abortReason) ? "." : ": " + abortReason + ".");
                }

                return await CompleteAsync(s);
            }
            finally
            {
                GHDiagnosticCountdown.Stop();
                ReleaseSampler(s);
            }
        }

        /* Settle, begin collects, the measurement window, and its save, with the frame
           marker hidden throughout. Throws DiagnosticAbortException on an abort. */
        private static async Task MeasureAsync(TestState s)
        {
            if (GHApp.PageFromTopOfModalNavigationStack() != s.GamePage)
                throw new DiagnosticAbortException("the menu did not close");

            GHFrameMarker.Suppressed = true;
            s.FrameMarkerSuppressed = true;
            try
            {
                await WaitAsync(s, SettleSeconds * 1000L);

                /* The per-process begin collect runs on the thread pool; the window opens
                   once it is done, or after ProcessIntervalBeginWaitMs at most */
                Task begin = GHSystemLoadSampler.StartProcessIntervalAsync();
                await WaitUntilAsync(s, delegate { return begin.IsCompleted; },
                    BeginPollMs, GHSystemLoadSampler.ProcessIntervalBeginWaitMs);

                s.SceneBefore = CaptureScene(s.GamePage);
                GHPerformanceRunContext ctx = new GHPerformanceRunContext();
                ctx.Notes = "in-game performance test";
                if (GHPerformanceRunRecord.BeginRefusal(GHWindowOwner.Diagnostic) != null)
                    throw new DiagnosticAbortException("another measurement is in progress");
                if (!GHPerformanceRunRecord.TryBeginWindow(ScenarioName, ArmName, ctx, GHWindowOwner.Diagnostic))
                    throw new DiagnosticAbortException("the measurement window could not be opened");
                s.OpenWindow = ctx;
                s.WindowClock = Stopwatch.StartNew();
                s.AllocatedBytesBegin = TotalAllocatedBytes();
                long windowStart = DateTime.UtcNow.Ticks;
                GHDiagnosticCountdown.Start(windowStart, windowStart + WindowSeconds * TimeSpan.TicksPerSecond);

                await WaitAsync(s, WindowSeconds * 1000L);

                SaveWindow(s);
            }
            finally
            {
                GHFrameMarker.Suppressed = false;
            }
        }

        /* Ends the open window, if any, and saves it into ReportsDirectory/<stamp>/ */
        private static void SaveWindow(TestState s)
        {
            GHPerformanceRunContext ctx = s.OpenWindow;
            if (ctx == null)
                return;
            s.OpenWindow = null;
            s.EndFrameId = GHFrameTimeline.LastFrameId;
            s.WindowElapsedSeconds = s.WindowClock != null ? s.WindowClock.Elapsed.TotalSeconds : double.NaN;
            s.AllocatedBytesEnd = TotalAllocatedBytes();
            s.LocalStamp = DateTime.Now;
            s.StampText = s.LocalStamp.ToString(StampFormat, CultureInfo.InvariantCulture);
            GHPerformanceRunResult result;
            GHPerformanceRunRecord.EndWindowAndSave(Path.Combine(ReportsDirectory, s.StampText), out result);
            s.Result = result;
            if (result == null)
                Log("the window of " + s.StampText + " could not be saved");
        }

        /* UI thread: the level description, zoom mode and map font size. The level text
           stays unknown when the status fields are locked by another thread. */
        private static GHDiagnosisScene CaptureScene(GamePage gamePage)
        {
            GHDiagnosisScene scene = new GHDiagnosisScene();
            try
            {
                scene.ZoomMode = gamePage.ZoomMiniMode ? GHDiagnosisScene.ZoomMinimap
                    : gamePage.ZoomAlternateMode ? GHDiagnosisScene.ZoomAlternate : GHDiagnosisScene.ZoomNormal;
                scene.MapFontSize = gamePage.MapFontSize;
                GHGame game = GHApp.CurrentGHGame;
                if (game != null && Monitor.TryEnter(game.StatusFieldLock))
                {
                    try
                    {
                        GHStatusField[] fields = game.StatusFields;
                        int i = (int)NhStatusFields.BL_LEVELDESC;
                        if (fields != null && i < fields.Length && !string.IsNullOrWhiteSpace(fields[i].Text))
                            scene.LevelText = fields[i].Text.Trim();
                    }
                    finally
                    {
                        Monitor.Exit(game.StatusFieldLock);
                    }
                }
            }
            catch (Exception ex)
            {
                Log("capturing the scene failed: " + ex.Message);
            }
            return scene;
        }

        /* Discards a window left open by an abort; nothing is saved */
        private static void DiscardOpenWindow(TestState s)
        {
            if (s.OpenWindow == null)
                return;
            s.OpenWindow = null;
            GHPerformanceRunRecord.DiscardWindow();
        }

        /* After the window: the facts, the report and its page. Returns the final message,
           or null when the report page opened. */
        private static async Task<string> CompleteAsync(TestState s)
        {
            GamePage gamePage = s.GamePage;
            GHDiagnosisFacts f = new GHDiagnosisFacts();

            /* UI thread only: the canvas's GPU context and the recent hitches report */
            bool glRequested = false;
            bool? gpuContextLive = null;
            try
            {
                glRequested = gamePage.UseMainGLCanvas;
                gpuContextLive = gamePage.MainCanvasGpuContextLive;
            }
            catch (Exception ex)
            {
                Log("reading the main canvas failed: " + ex.Message);
            }
            string frameDetail = GHPerformanceRunRecord.BuildRecentHitchesReport(s.EndFrameId, WindowSeconds);

            FillWindowFacts(f, s);
            f.SceneBefore = s.SceneBefore;
            FillBackgroundFacts(f, s.Result != null ? s.Result.Background : null);
            f.OwnMemoryBytes = GHApp.GetUsedMemoryInBytes();
            f.DeviceMemoryBytes = GHApp.TotalMemory > 0 ? (long)GHApp.TotalMemory : -1;
            f.AllocationRateMBPerSec = GHPerformanceDiagnosticSupport.ToFloat(
                GHPerformanceDiagnosticSupport.AllocationRateMBPerSec(s.AllocatedBytesBegin, s.AllocatedBytesEnd,
                    s.WindowElapsedSeconds));
            f.MainCanvasGlRequested = glRequested;
            f.MainCanvasGpuContextLive = gpuContextLive;
            f.GpuBackend = string.IsNullOrEmpty(GHApp.GPUBackend) ? null : GHApp.GPUBackend;
            FillDeviceGpuFacts(f);
            f.NonDefaultPerformanceSettings = GHPerformanceDiagnosticSupport.NonDefaultPerformanceSettings(
                GHApp.IsTileBatchingAvailable, GHConstants.DefaultTileBatching, GHApp.UseTileBatching,
                GHConstants.DefaultTextBlobCaching, GHApp.UseTextBlobCaching);
#if DEBUG
            f.IsDebugBuild = true;
#else
            f.IsDebugBuild = false;
#endif
            f.DebuggerAttached = Debugger.IsAttached;
            f.VerboseLoggingOn = GHApp.DebugLogMessages || GHApp.ScreenLogging || GHApp.LowLevelLogging;
            f.PlatformRenderLoopOn = GHApp.UsePlatformRenderLoop;
            f.CountdownShown = true;
            f.FrameMarkerOff = s.FrameMarkerSuppressed;
            f.FrameDetailText = frameDetail;

            /* Thread pool: the adapter probe's last result, the GPU preference (registry)
               and the environment fingerprint (WMI) */
            PostWindowData post = null;
            try
            {
                post = await Task.Run(delegate { return CollectPostWindow(); });
            }
            catch (Exception ex)
            {
                Log("the post-window collection failed: " + ex.Message);
            }
            if (post != null)
            {
                f.Fingerprint = post.Fingerprint;
                f.GpuPreference = post.GpuPreference;
                f.RenderAdapterName = post.RenderAdapterName;
                f.RenderAdapterIsIntegrated = post.RenderAdapterIsIntegrated;
                f.RenderAdapterIsSoftware = post.RenderAdapterIsSoftware;
                if (post.Adapters != null && post.Adapters.Count > 0)
                    f.Adapters = post.Adapters;
            }

            string reportsDirectory = ReportsDirectory;
            string stampText = s.StampText;
            DateTime localStamp = s.LocalStamp;
            string reportPath = null;
            string writeError = null;
            await Task.Run(delegate
            {
                try
                {
                    GHDiagnosisResult r = GHPerformanceDiagnosis.Diagnose(f);
                    string text = GHPerformanceDiagnosis.BuildReport(f, r, localStamp);
                    string path = Path.Combine(reportsDirectory, ReportFilePrefix + stampText + ReportFileExtension);
                    File.WriteAllText(path, text, new UTF8Encoding(false));
                    reportPath = path;
                }
                catch (Exception ex)
                {
                    writeError = ex.Message;
                }
                ApplyRetention(reportsDirectory, stampText);
            });
            if (reportPath == null)
            {
                Log("writing the report failed: " + writeError);
                return "The performance report could not be written: " + writeError;
            }
            Log("report " + Path.GetFileName(reportPath) + " written");

            GHDiagnosticCountdown.Stop();
            DisplayFilePage page = new DisplayFilePage(reportPath, ReportPageTitle, GHPerformanceTextReport.MaxLineWidth, true);
            string errorMessage;
            if (!page.ReadFile(out errorMessage))
                return "GnollHack cannot open the report file: " + errorMessage;
            await GHApp.PushModalPageAsync(page);
            return null;
        }

        /* The smoothness summary and the thermal readings of the saved window */
        private static void FillWindowFacts(GHDiagnosisFacts f, TestState s)
        {
            f.NominalWindowSeconds = WindowSeconds;
            GHPerformanceRunResult result = s.Result;
            if (result == null)
            {
                f.WindowSeconds = GHPerformanceDiagnosticSupport.ToFloat(s.WindowElapsedSeconds);
                f.ExcludedReason = "the measurement could not be saved";
                return;
            }

            GHPerformanceDiagnosticSupport.FillSummaryFacts(f, result.Summary, s.WindowElapsedSeconds,
                result.OnScreenIntervalCount);

            GHThermalReading before = result.ThermalBefore;
            GHThermalReading after = result.ThermalAfter;
            f.ThermalRankBefore = (int)before.Status;
            f.ThermalRankAfter = (int)after.Status;
            f.HeadroomAfter = after.HeadroomFraction;
            f.CpuPerformancePctBefore = before.CpuPerformancePct;
            f.CpuPerformancePctAfter = after.CpuPerformancePct;
            f.PowerStateKnown = after.PowerStateKnown;
            f.IsCharging = after.IsCharging;
            f.IsLowPower = after.IsLowPower;

            /* Of the run record's exclusion reasons, only a power state change makes the
               measurement itself unreliable; throttling and a busy background are
               diagnosed as findings instead */
            if (before.PowerStateKnown && after.PowerStateKnown && before.IsCharging != after.IsCharging)
                f.ExcludedReason = "power state changed";
            if (!string.IsNullOrEmpty(result.ExcludedReason))
                Log("run record exclusion: " + result.ExcludedReason);
        }

        /* The window's background report; null when the sampler is off or failed */
        private static void FillBackgroundFacts(GHDiagnosisFacts f, GHBackgroundReport bg)
        {
            f.BackgroundSamplerDisabled = !GHSystemLoadSampler.Enabled;
            if (bg == null)
                return;
            f.BackgroundVerdict = bg.Verdict;
            f.BackgroundReason = bg.Reason;
            GHBackgroundSummary w = bg.Window;
            if (w != null)
            {
                f.OtherCpuP90Pct = w.OtherCpuP90Pct;
                f.DiskBusyP90Pct = w.DiskBusyP90Pct;
                f.AvailableMemoryMinPct = w.AvailableMemoryMinPct;
                f.AvailableMemoryMinMB = w.AvailableMemoryMinMB;
                f.HardFaultsP90PerSec = w.HardFaultsP90PerSec;
                f.LowMemory = w.LowMemory;
                f.MemoryPressureEvents = w.MemoryPressureEvents;
            }
            f.OtherGpuPct = bg.OtherGpuPct;
            if (bg.HasSignal(GHBackgroundReport.SignalProcesses))
                f.Processes = new List<GHProcessLoad>(bg.Processes);
        }

        /* GHApp.DeviceGPUs (Windows WMI; empty elsewhere): whether a discrete adapter is
           present, the adapter lines when the adapter probe gives none, and on Windows the
           display's maximum refresh when exactly one adapter drives a display */
        private static void FillDeviceGpuFacts(GHDiagnosisFacts f)
        {
            List<DeviceGPU> gpus = GHApp.DeviceGPUs;
            if (gpus == null || gpus.Count == 0)
                return;
            bool discrete = false;
            List<string> lines = new List<string>();
            for (int i = 0; i < gpus.Count; i++)
            {
                DeviceGPU g = gpus[i];
                if (g == null)
                    continue;
                if (!g.IsIntegratedGraphics)
                    discrete = true;
                lines.Add((string.IsNullOrEmpty(g.Description) ? "unnamed adapter" : g.Description)
                    + " (" + (g.IsIntegratedGraphics ? "integrated" : "discrete") + (g.IsCurrent ? ", drives a display" : "") + ")");
            }
            f.DiscreteAdapterPresent = discrete;
            f.Adapters = lines;
#if GNH_MAUI && WINDOWS
            /* Unknown when several adapters drive displays: which one shows the app is not known */
            int currentCount = 0;
            int currentMaxRefresh = -1;
            for (int i = 0; i < gpus.Count; i++)
            {
                DeviceGPU g = gpus[i];
                if (g != null && g.IsCurrent)
                {
                    currentCount++;
                    currentMaxRefresh = g.MaxRefreshRate;
                }
            }
            if (currentCount == 1 && currentMaxRefresh > 0)
                f.DisplayMaxRefreshHz = currentMaxRefresh;
#endif
        }

        /* Thread pool only. The adapter facts are the probe's last result, which the run
           record's EndWindowAndSave has just refreshed; none when no probe of this process
           has named a render adapter. */
        private static PostWindowData CollectPostWindow()
        {
            PostWindowData d = new PostWindowData();
#if GNH_MAUI && WINDOWS
            try
            {
                List<GHRenderAdapter> all = new List<GHRenderAdapter>();
                GHRenderAdapter render;
                bool known = GHRenderAdapterProbeWindows.TryGetLast(out render, all);
                if (known && render != null)
                {
                    d.RenderAdapterName = render.Name;
                    d.RenderAdapterIsIntegrated = render.IsIntegrated;
                    d.RenderAdapterIsSoftware = render.IsSoftware;
                }
                /* Hardware adapters only: the software adapter (Microsoft Basic Render
                   Driver) is always listed by DXGI */
                List<string> lines = new List<string>();
                for (int i = 0; i < all.Count; i++)
                {
                    GHRenderAdapter a = all[i];
                    if (a == null || a.IsSoftware)
                        continue;
                    string kind = a.IsIntegrated == true ? "integrated" : a.IsIntegrated == false ? "discrete" : "kind unknown";
                    lines.Add((string.IsNullOrEmpty(a.Name) ? "unnamed adapter" : a.Name) + " (" + kind + ", "
                        + a.DedicatedVideoMemoryMB.ToString(CultureInfo.InvariantCulture) + " MB"
                        + (known && render != null && a.Luid == render.Luid ? ", renders the map" : "") + ")");
                }
                d.Adapters = lines;
            }
            catch (Exception ex)
            {
                Log("the adapter probe failed: " + ex.Message);
            }
            try
            {
                string preference = GHApp.GetActiveGPU();
                d.GpuPreference = string.IsNullOrEmpty(preference) ? null : preference;
            }
            catch (Exception ex)
            {
                Log("reading the GPU preference failed: " + ex.Message);
            }
#endif
            try
            {
                d.Fingerprint = GHPerformanceEnvironment.CaptureFingerprint(true);
            }
            catch (Exception ex)
            {
                Log("capturing the environment failed: " + ex.Message);
            }
            return d;
        }

        /* Deletes what GHPerformanceDiagnosticSupport.SelectRetentionDeletions selects for
           MaxKeptReports: the older reports and the <stamp> folders without a kept
           report, except protectedStamp's (the test just saved, or null). Thread pool. */
        private static void ApplyRetention(string directory, string protectedStamp)
        {
            try
            {
                if (!Directory.Exists(directory))
                    return;
                string[] files = Directory.GetFiles(directory);
                string[] folders = Directory.GetDirectories(directory);
                List<string> fileNames = new List<string>(files.Length);
                for (int i = 0; i < files.Length; i++)
                    fileNames.Add(Path.GetFileName(files[i]));
                List<string> folderNames = new List<string>(folders.Length);
                for (int i = 0; i < folders.Length; i++)
                    folderNames.Add(Path.GetFileName(folders[i]));
                List<string> filesToDelete = new List<string>();
                List<string> foldersToDelete = new List<string>();
                GHPerformanceDiagnosticSupport.SelectRetentionDeletions(fileNames, folderNames, MaxKeptReports,
                    protectedStamp, filesToDelete, foldersToDelete);

                for (int i = 0; i < filesToDelete.Count; i++)
                {
                    try
                    {
                        File.Delete(Path.Combine(directory, filesToDelete[i]));
                    }
                    catch (Exception ex)
                    {
                        Log("deleting " + filesToDelete[i] + " failed: " + ex.Message);
                    }
                }
                for (int i = 0; i < foldersToDelete.Count; i++)
                {
                    try
                    {
                        Directory.Delete(Path.Combine(directory, foldersToDelete[i]), true);
                    }
                    catch (Exception ex)
                    {
                        Log("deleting the folder " + foldersToDelete[i] + " failed: " + ex.Message);
                    }
                }
            }
            catch (Exception ex)
            {
                Log("applying the report retention failed: " + ex.Message);
            }
        }

        /* Waits about ms milliseconds in steps of at most 100 ms, checking for an abort
           before every step and after the last */
        private static async Task WaitAsync(TestState s, long ms)
        {
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                CheckAbort(s);
                long remaining = ms - sw.ElapsedMilliseconds;
                if (remaining <= 0)
                    return;
                await Task.Delay((int)Math.Min(PollMs, remaining));
            }
        }

        /* Polls condition every pollMs, checking for an abort first, until it holds
           (true) or timeoutMs passes (false) */
        private static async Task<bool> WaitUntilAsync(TestState s, Func<bool> condition, int pollMs, int timeoutMs)
        {
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                CheckAbort(s);
                if (condition())
                    return true;
                if (sw.ElapsedMilliseconds >= timeoutMs)
                    return false;
                await Task.Delay(pollMs);
            }
        }

        /* The app went to the background, a page opened over the game page, the game
           ended, or, once the window is open, a menu or window opened over the map */
        private static void CheckAbort(TestState s)
        {
            if (GHApp.IsSuspended)
                throw new DiagnosticAbortException("the app went to the background");
            if (s.GamePage.GameEnded || !GHApp.GameStarted)
                throw new DiagnosticAbortException("the game ended");
            if (GHApp.PageFromTopOfModalNavigationStack() != s.GamePage)
                throw new DiagnosticAbortException("a page opened over the game");
            if (s.WindowClock != null && s.GamePage.GetActiveCanvas() != CanvasTypes.MainCanvas)
                throw new DiagnosticAbortException("a menu or window opened over the map");
        }

        /* Releases the test's sampler hold, once */
        private static void ReleaseSampler(TestState s)
        {
            if (!s.SamplerHeld)
                return;
            s.SamplerHeld = false;
            GHSystemLoadSampler.Release();
        }

        private static async Task ShowMessageAsync(Page page, string message)
        {
            if (page == null)
                return;
            try
            {
                await GHApp.DisplayMessageBox(page, MessageTitle, message, "OK");
            }
            catch (Exception ex)
            {
                Log("showing a message failed: " + ex.Message);
            }
        }

        /* The process's total allocated bytes, or -1 where the runtime has no such counter */
        private static long TotalAllocatedBytes()
        {
#if GNH_MAUI
            try
            {
                return GC.GetTotalAllocatedBytes(false);
            }
            catch (Exception ex)
            {
                Log("reading the allocated bytes failed: " + ex.Message);
                return -1;
            }
#else
            return -1;
#endif
        }

        private static void Log(string text)
        {
            try
            {
                Debug.WriteLine("Performance test: " + text);
                GHApp.MaybeWriteGHLog("Performance test: " + text);
            }
            catch
            {
                /* Logging must never break the test */
            }
        }

        private sealed class DiagnosticAbortException : Exception
        {
            public readonly string Reason;

            public DiagnosticAbortException(string reason) : base(reason)
            {
                Reason = reason;
            }
        }

        private sealed class TestState
        {
            public GamePage GamePage;
            public bool SamplerHeld;                   /* the test's GHSystemLoadSampler acquire is outstanding */
            public bool FrameMarkerSuppressed;         /* GHFrameMarker was hidden for the window */
            public GHPerformanceRunContext OpenWindow; /* non-null while the window is open */
            public Stopwatch WindowClock;              /* started when the window opened */
            public GHDiagnosisScene SceneBefore;       /* captured just before the window opened */
            public double WindowElapsedSeconds = double.NaN;
            public long AllocatedBytesBegin = -1;      /* the process's allocated bytes at the window's open; -1 when unknown */
            public long AllocatedBytesEnd = -1;        /* the same at its save */
            public long EndFrameId;
            public DateTime LocalStamp;
            public string StampText;                   /* StampFormat, invariant culture, local time */
            public GHPerformanceRunResult Result;      /* null when the window could not be saved */
        }

        /* What the thread pool collects after the window; the adapter fields are set on
           Windows only and stay unknown elsewhere */
        private sealed class PostWindowData
        {
            public Dictionary<string, string> Fingerprint;
            public string GpuPreference = null;
            public string RenderAdapterName = null;
            public bool? RenderAdapterIsIntegrated = null;
            public bool? RenderAdapterIsSoftware = null;
            public List<string> Adapters = null;
        }
    }
}

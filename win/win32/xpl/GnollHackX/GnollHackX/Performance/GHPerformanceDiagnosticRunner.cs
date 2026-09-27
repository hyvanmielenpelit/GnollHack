using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
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
            measurement window is already running, the game has ended, or the platform
            render loop is off.
         2. Asks for confirmation on the host page. On OK: holds the background load
            sampler (so the pre-window has samples), starts the countdown and closes
            the menu.
         3. Settles for SettleSeconds, so the menu's pause mark and collection fall
            outside the window; starts the per-process interval (and on Windows the
            render adapter probe) on the thread pool, waits for its begin collect at
            most ProcessIntervalBeginWaitMs, then opens the measurement window.
         4. Measures for WindowSeconds, polling every 100 ms for an abort: the app went
            to the background, a page opened over the game page, or the game ended.
            An aborted window is still saved into the test's folder, excluded with the
            abort reason, as saving is what closes it; no report is written, and the
            report retention then deletes that folder with any other orphaned one.
         5. Saves the window into ReportsDirectory/<stamp>/, reads what must be read on
            the UI thread (GPU context, profiler statistics, recent hitches), then on
            the thread pool ends the adapter probe and re-captures the environment
            fingerprint.
         6. Builds GHDiagnosisFacts, diagnoses, writes
            ReportsDirectory/perftest_<stamp>.txt, keeps the newest MaxKeptReports
            reports, and opens the report page.
       Messages after the menu has closed are shown on the game page. RunAsync never
       throws.

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

        private static readonly Regex _reportFileRegex = new Regex(@"^perftest_([0-9]{8}_[0-9]{6})\.txt\z",
            RegexOptions.CultureInvariant);
        private static readonly Regex _testFolderRegex = new Regex(@"^[0-9]{8}_[0-9]{6}\z", RegexOptions.CultureInvariant);

        private static int _isRunning = 0;

        public static bool IsRunning { get { return Interlocked.CompareExchange(ref _isRunning, 0, 0) != 0; } }

        /* GHPath/performance/diagnostics: the reports and, per test, a <stamp> folder with
           the window's run record. Persistent, not cleaned at start. */
        public static string ReportsDirectory
        {
            get { return Path.Combine(GHApp.GHPath, GHConstants.PerformanceDirectory, ReportsFolderName); }
        }

        /* Call on the UI thread. hostForMessages shows the refusals and the confirmation;
           it is popped when it is on top of the game page. Never throws. */
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
                try
                {
                    await RunTestAsync(gamePage, host);
                }
                finally
                {
                    GHDiagnosticCountdown.Stop();
                    Interlocked.Exchange(ref _isRunning, 0);
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
            if (GHPerformanceRunRecord.IsWindowOpen)
                return "Another performance measurement is running.";
            if (gamePage.GameEnded || !GHApp.GameStarted)
                return "The game has ended.";
            if (!GHApp.UsePlatformRenderLoop)
                return "Frame timing requires the platform render loop (Settings).";
            return null;
        }

        private static async Task RunTestAsync(GamePage gamePage, Page host)
        {
            bool confirmed = await GHApp.DisplayMessageBox(host, MessageTitle, ConfirmText, "Start", "Cancel");
            if (!confirmed)
                return;
            /* Anything may have changed while the confirmation was shown */
            string refusal = CheckPreconditions(gamePage);
            if (refusal != null)
            {
                await ShowMessageAsync(host, refusal);
                return;
            }

            TestState s = new TestState();
            s.GamePage = gamePage;
            try
            {
                string completedMessage = null;
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
                    long now = DateTime.UtcNow.Ticks;
                    long settleEnd = now + SettleSeconds * TimeSpan.TicksPerSecond;
                    GHDiagnosticCountdown.Start(settleEnd, settleEnd + WindowSeconds * TimeSpan.TicksPerSecond);

                    /* As the menu's Back to Game closes it */
                    FrameTimeProfiler.MarkPauseEvent();
                    GHApp.CollectNursery();
                    if (host != gamePage && GHApp.PageFromTopOfModalNavigationStack() == host)
                        await GHApp.PopModalPageAsync();
                    s.MenuClosed = true;

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
                        CloseOpenWindow(s, abortReason);
                }

                if (!completed)
                {
                    GHDiagnosticCountdown.Stop();
                    ReleaseSampler(s);
                    await EndAdapterProbeQuietlyAsync(s);
                    string reportsDirectory = ReportsDirectory;
                    await Task.Run(delegate { ApplyRetention(reportsDirectory); });
                    Log("cancelled: " + (abortReason ?? "unknown reason"));
                    await ShowMessageAsync(s.MenuClosed ? gamePage : host, "Performance test cancelled"
                        + (string.IsNullOrEmpty(abortReason) ? "." : ": " + abortReason + "."));
                    return;
                }

                completedMessage = await CompleteAsync(s);
                GHDiagnosticCountdown.Stop();
                ReleaseSampler(s);
                if (completedMessage != null)
                    await ShowMessageAsync(gamePage, completedMessage);
            }
            finally
            {
                GHDiagnosticCountdown.Stop();
                ReleaseSampler(s);
            }
        }

        /* Settle, begin collects, the measurement window, and its save. Throws
           DiagnosticAbortException on an abort. */
        private static async Task MeasureAsync(TestState s)
        {
            if (GHApp.PageFromTopOfModalNavigationStack() != s.GamePage)
                throw new DiagnosticAbortException("the menu did not close");

            await WaitAsync(s, SettleSeconds * 1000L);

            /* The per-process begin collect (and the adapter probe's) run on the thread
               pool; the window opens once they are done, or after
               ProcessIntervalBeginWaitMs at most */
            Task begin = GHSystemLoadSampler.StartProcessIntervalAsync();
            Task probeBegin = null;
#if GNH_MAUI && WINDOWS
            probeBegin = Task.Run(delegate { return GHRenderAdapterProbeWindows.Begin(); });
            s.AdapterProbeStarted = true;
#endif
            await WaitUntilAsync(s, delegate { return begin.IsCompleted && (probeBegin == null || probeBegin.IsCompleted); },
                BeginPollMs, GHSystemLoadSampler.ProcessIntervalBeginWaitMs);

            GHPerformanceRunContext ctx = new GHPerformanceRunContext();
            ctx.Notes = "in-game performance test";
            GHPerformanceRunRecord.BeginWindow(ScenarioName, ArmName, ctx);
            if (!GHPerformanceRunRecord.IsWindowOpen)
                throw new DiagnosticAbortException("the measurement window could not be opened");
            s.OpenWindow = ctx;
            s.WindowClock = Stopwatch.StartNew();
            long windowStart = DateTime.UtcNow.Ticks;
            GHDiagnosticCountdown.Start(windowStart, windowStart + WindowSeconds * TimeSpan.TicksPerSecond);

            await WaitAsync(s, WindowSeconds * 1000L);

            SaveWindow(s);
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
            s.LocalStamp = DateTime.Now;
            s.StampText = s.LocalStamp.ToString(StampFormat, CultureInfo.InvariantCulture);
            GHPerformanceRunResult result;
            GHPerformanceRunRecord.EndWindowAndSave(Path.Combine(ReportsDirectory, s.StampText), out result);
            s.Result = result;
            if (result == null)
                Log("the window of " + s.StampText + " could not be saved");
        }

        /* Saves a window left open by an abort, excluded with the abort reason */
        private static void CloseOpenWindow(TestState s, string reason)
        {
            if (s.OpenWindow == null)
                return;
            try
            {
                s.OpenWindow.ExcludedReason = "aborted: " + (string.IsNullOrEmpty(reason) ? "unknown reason" : reason);
                SaveWindow(s);
            }
            catch (Exception ex)
            {
                s.OpenWindow = null;
                Log("saving the aborted window failed: " + ex.Message);
            }
        }

        /* Closes the Windows adapter probe's query after an abort */
        private static async Task EndAdapterProbeQuietlyAsync(TestState s)
        {
#if GNH_MAUI && WINDOWS
            if (!s.AdapterProbeStarted)
                return;
            s.AdapterProbeStarted = false;
            try
            {
                await Task.Run(delegate
                {
                    GHRenderAdapter render;
                    GHRenderAdapterProbeWindows.End(out render, null);
                });
            }
            catch (Exception ex)
            {
                Log("ending the adapter probe failed: " + ex.Message);
            }
#else
            await Task.FromResult(0);
#endif
        }

        /* After the window: the facts, the report and its page. Returns a message to show
           on the game page, or null when the report page opened. */
        private static async Task<string> CompleteAsync(TestState s)
        {
            GamePage gamePage = s.GamePage;
            GHDiagnosisFacts f = new GHDiagnosisFacts();

            /* UI thread only: the canvas's GPU context, the profiler statistics (not
               re-entrant) and the recent hitches report */
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
            float allocationRate = float.NaN;
            try
            {
                FrameTimeStatistics stats = FrameTimeProfiler.GetStatistics();
                if (stats.SampleCount > 0)
                    allocationRate = stats.AllocationRateMBPerSec;
            }
            catch (Exception ex)
            {
                Log("reading the profiler statistics failed: " + ex.Message);
            }
            string frameDetail = GHPerformanceRunRecord.BuildRecentHitchesReport(s.EndFrameId, WindowSeconds);

            FillWindowFacts(f, s);
            FillBackgroundFacts(f, s.Result != null ? s.Result.Background : null);
            f.OwnMemoryBytes = GHApp.GetUsedMemoryInBytes();
            f.DeviceMemoryBytes = GHApp.TotalMemory > 0 ? (long)GHApp.TotalMemory : -1;
            f.AllocationRateMBPerSec = allocationRate;
            f.MainCanvasGlRequested = glRequested;
            f.MainCanvasGpuContextLive = gpuContextLive;
            f.GpuBackend = string.IsNullOrEmpty(GHApp.GPUBackend) ? null : GHApp.GPUBackend;
            FillDeviceGpuFacts(f);
            f.NonDefaultPerformanceSettings = NonDefaultPerformanceSettings();
#if DEBUG
            f.IsDebugBuild = true;
#else
            f.IsDebugBuild = false;
#endif
            f.DebuggerAttached = Debugger.IsAttached;
            f.VerboseLoggingOn = GHApp.DebugLogMessages || GHApp.ScreenLogging || GHApp.LowLevelLogging;
            f.PlatformRenderLoopOn = GHApp.UsePlatformRenderLoop;
            f.CountdownShown = true;
            f.FrameDetailText = frameDetail;

            /* Thread pool: the adapter probe's second collect and the DXGI enumeration,
               the GPU preference (registry) and the environment fingerprint (WMI) */
            PostWindowData post = null;
            try
            {
                bool probeStarted = s.AdapterProbeStarted;
                s.AdapterProbeStarted = false;
                post = await Task.Run(delegate { return CollectPostWindow(probeStarted); });
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
                ApplyRetention(reportsDirectory);
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
            GHPerformanceRunResult result = s.Result;
            if (result == null)
            {
                f.WindowSeconds = ToFloat(s.WindowElapsedSeconds);
                f.ExcludedReason = "the measurement could not be saved";
                return;
            }

            GHSmoothnessSummary summary = result.Summary;
            if (summary != null)
            {
                bool haveWindow = summary.WindowMs > 0 && summary.DisplayedCount >= 2;
                f.WindowSeconds = summary.WindowMs > 0 ? (float)(summary.WindowMs / 1000.0) : ToFloat(s.WindowElapsedSeconds);
                f.TargetFps = Positive(summary.TargetFps);
                f.MeasuredRefreshHz = Positive(summary.MeasuredRefreshHz);
                f.AssumedRefreshMismatch = summary.AssumedRefreshMismatch;
                f.DisplayedFps = haveWindow ? (float)summary.DisplayedFps : float.NaN;
                f.HitchRatioMsPerSec = haveWindow ? (float)summary.HitchRatioMsPerSec : float.NaN;
                f.HitchCount = summary.HitchCount;
                f.PacingErrorRmsMs = haveWindow ? (float)summary.PacingErrorRmsMs : float.NaN;
                f.PaintP50Ms = summary.PaintedCount > 0 ? (float)summary.PaintP50Ms : float.NaN;
                f.PaintP99Ms = summary.PaintedCount > 0 ? (float)summary.PaintP99Ms : float.NaN;
                f.GcCount = summary.GcCount;
                f.GcPauseMs = summary.GcPauseDataAvailable ? (float)summary.GcPauseMs : float.NaN;
                f.CauseMs = (double[])summary.CauseMs.Clone();
                int contentEvents = 0;
                for (int i = 0; i < summary.EventGapCount.Length; i++)
                    contentEvents += summary.EventGapCount[i];
                f.ContentEventCount = contentEvents;
            }
            else
            {
                f.WindowSeconds = ToFloat(s.WindowElapsedSeconds);
            }

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

        /* The performance features switched off although their default, as used by the
           preference loads in GHApp, is on; turning a feature on is never listed */
        private static List<string> NonDefaultPerformanceSettings()
        {
            List<string> list = new List<string>();
            /* The defaults are per-platform constants; locals keep every branch reachable */
            bool platformLoopDefault = GHApp.IsPlatformRenderLoopAvailable && GHConstants.IsPlatformRenderLoopDefault;
            bool tileBatchingDefault = GHConstants.DefaultTileBatching;
            bool textBlobCachingDefault = GHConstants.DefaultTextBlobCaching;
            if (platformLoopDefault && !GHApp.UsePlatformRenderLoop)
                list.Add("platform render loop off");
            /* Forced off where unavailable (iOS) */
            if (GHApp.IsTileBatchingAvailable && tileBatchingDefault && !GHApp.UseTileBatching)
                list.Add("tile batching off");
            if (textBlobCachingDefault && !GHApp.UseTextBlobCaching)
                list.Add("text blob caching off");
            return list;
        }

        /* Thread pool only */
        private static PostWindowData CollectPostWindow(bool adapterProbeStarted)
        {
            PostWindowData d = new PostWindowData();
#if GNH_MAUI && WINDOWS
            try
            {
                List<GHRenderAdapter> all = new List<GHRenderAdapter>();
                GHRenderAdapter render = null;
                bool known = false;
                if (adapterProbeStarted)
                    known = GHRenderAdapterProbeWindows.End(out render, all);
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
                        + (known && ReferenceEquals(a, render) ? ", renders the map" : "") + ")");
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

        /* Keeps the newest MaxKeptReports perftest_<stamp>.txt reports; deletes the older
           ones and every <stamp> folder whose report is not kept, such as an aborted
           test's. Only names matching those two patterns are touched. Thread pool. */
        private static void ApplyRetention(string directory)
        {
            try
            {
                if (!Directory.Exists(directory))
                    return;
                List<string> stamps = new List<string>();
                string[] files = Directory.GetFiles(directory);
                for (int i = 0; i < files.Length; i++)
                {
                    Match m = _reportFileRegex.Match(Path.GetFileName(files[i]));
                    if (m.Success)
                        stamps.Add(m.Groups[1].Value);
                }
                stamps.Sort(StringComparer.Ordinal);
                HashSet<string> kept = new HashSet<string>(StringComparer.Ordinal);
                for (int i = Math.Max(0, stamps.Count - MaxKeptReports); i < stamps.Count; i++)
                    kept.Add(stamps[i]);

                for (int i = 0; i < files.Length; i++)
                {
                    Match m = _reportFileRegex.Match(Path.GetFileName(files[i]));
                    if (!m.Success || kept.Contains(m.Groups[1].Value))
                        continue;
                    try
                    {
                        File.Delete(files[i]);
                    }
                    catch (Exception ex)
                    {
                        Log("deleting " + Path.GetFileName(files[i]) + " failed: " + ex.Message);
                    }
                }

                string[] folders = Directory.GetDirectories(directory);
                for (int i = 0; i < folders.Length; i++)
                {
                    string name = Path.GetFileName(folders[i]);
                    if (!_testFolderRegex.IsMatch(name) || kept.Contains(name))
                        continue;
                    try
                    {
                        Directory.Delete(folders[i], true);
                    }
                    catch (Exception ex)
                    {
                        Log("deleting the folder " + name + " failed: " + ex.Message);
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

        /* The app went to the background, a page opened over the game page, or the
           game ended */
        private static void CheckAbort(TestState s)
        {
            if (GHApp.IsSuspended)
                throw new DiagnosticAbortException("the app went to the background");
            if (s.GamePage.GameEnded || !GHApp.GameStarted)
                throw new DiagnosticAbortException("the game ended");
            if (GHApp.PageFromTopOfModalNavigationStack() != s.GamePage)
                throw new DiagnosticAbortException("a page opened over the game");
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

        private static float Positive(double value)
        {
            return value > 0 && !double.IsInfinity(value) ? (float)value : float.NaN;
        }

        private static float ToFloat(double value)
        {
            return double.IsNaN(value) || double.IsInfinity(value) ? float.NaN : (float)value;
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
            public bool MenuClosed;                    /* messages go to the game page from here on */
            public bool SamplerHeld;                   /* the test's GHSystemLoadSampler acquire is outstanding */
            public bool AdapterProbeStarted;           /* Windows: the adapter probe's Begin was started */
            public GHPerformanceRunContext OpenWindow; /* non-null while the window is open */
            public Stopwatch WindowClock;              /* started when the window opened */
            public double WindowElapsedSeconds = double.NaN;
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

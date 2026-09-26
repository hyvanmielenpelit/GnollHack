using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackM;
using Microsoft.Maui.Controls;
#else
using Xamarin.Forms;
using GnollHackX.Pages.Game;
#endif

namespace GnollHackX.Performance
{
    /* Runs a performance suite: a series of measurement windows over one replay file,
       each started from the same turn, saved through GHPerformanceRunRecord and
       registered in GHPerformanceSuiteStore.

       Run 0 is the optional warm-up run (saved but excluded); runs 1..Runs are
       measured. A new game page plays the replay from its beginning as the Replay page
       does when the start turn is 1 or less, and otherwise seeks to StartTurn - 1 and
       lets it play the last turn at normal speed; a shared page restarts in place by
       seeking back to StartTurn - 1. Per run: wait for the start turn and for any
       replayed window or prompt to close, apply the scenario (idle pauses
       the replay, minimap pauses it and switches to the minimap, playback lets it
       play), warm up, measure one window, and save it. Between runs: cool down, wait
       for the thermal state to be no worse than at the suite start (at most 300 s),
       then either seek the same game page back (shared) or close it and open a new
       one (fresh; the cool-down then happens on the page below the game page).

       Everything runs on the UI thread as one async task. Every wait polls at least
       every 100 ms and aborts the suite when the replay ended (GHApp.GameStarted went
       false), the user stopped the replay, or the app went to the background; a
       window open at that moment is still saved, excluded with the abort reason.
       GHApp.CurrentGHGame is never used as an end signal: an in-place replay restart
       sets it to null for a while. No message boxes are shown here; errors are
       returned to the caller.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHPerformanceSuiteRunner
    {
        private const int SeekAllowanceSeconds = 15;
        private const int StartTurnPollMs = 16;
        private const int PollMs = 100;
        private const int StartTurnTimeoutMs = 120 * 1000;
        private const int PageGoneTimeoutMs = 30 * 1000;
        private const int ThermalPrimeMs = 600;
        private const int ThermalGatePollMs = 15 * 1000;
        private const int ThermalGateTimeoutMs = 300 * 1000;
        private const int AfterGarbageCollectionMs = 500;
        private const float CpuThrottleDropPct = 10.0f;
        private const int MaxSeconds = 24 * 60 * 60;

        private const string ScenarioIdle = "idle";
        private const string ScenarioMinimap = "minimap";
        private const string ScenarioPlayback = "playback";
        private const string PageModeShared = "shared";
        private const string PageModeFresh = "fresh";

        private static int _isRunning = 0;
        private static string _currentSuiteId = null;

        public static bool IsRunning { get { return Interlocked.CompareExchange(ref _isRunning, 0, 0) != 0; } }

        public static string CurrentSuiteId
        {
            get { return Interlocked.CompareExchange(ref _currentSuiteId, null, null); }
            private set { Interlocked.Exchange(ref _currentSuiteId, value); }
        }

        /* Estimated wall time of the setup: (runs + warm-up run) * (seek allowance 15 s
           + warm-up + window + cool-down) */
        public static TimeSpan EstimateDuration(GHPerformanceSuiteSetup setup)
        {
            if (setup == null)
                return TimeSpan.Zero;
            int runCount = Math.Max(0, setup.Runs) + (setup.WarmUpRun ? 1 : 0);
            double perRunSeconds = SeekAllowanceSeconds + Math.Max(0, setup.WarmUpSeconds)
                + Math.Max(0, setup.WindowSeconds) + Math.Max(0, setup.CooldownSeconds);
            return TimeSpan.FromSeconds(runCount * perRunSeconds);
        }

        /* Call on the UI thread. Returns null once the suite has run (complete or
           aborted), or a short user-facing error when it could not start (already
           running, invalid setup, a game already running, replay missing or invalid).
           onFinished(suiteId) is awaited on the UI thread after the suite ends and its
           game page is gone; it is not called when the suite could not start. */
        public static async Task<string> RunAsync(GHPerformanceSuiteSetup setup, Func<string, Task> onFinished)
        {
            if (IsRunning)
                return "A performance suite is already running.";
            string error = ValidateSetup(setup);
            if (error != null)
                return error;
            if (GHApp.GameStarted)
                return "A game is already running.";
            if (GHApp.CurrentMainPage == null)
                return "The main page is not available.";
            string replayPath = setup.ReplayPath;
            if (!File.Exists(replayPath))
                return "The replay file does not exist.";
            string validationMessage;
            if (!GHApp.ValidateReplayFile(replayPath, out validationMessage))
                return string.IsNullOrEmpty(validationMessage) ? "The replay file is not valid." : validationMessage;
            if (Interlocked.CompareExchange(ref _isRunning, 1, 0) != 0)
                return "A performance suite is already running.";

            SuiteState s = new SuiteState();
            try
            {
                s.Setup = setup;
                s.ReplayPath = replayPath;
                s.ReplayFileName = Path.GetFileName(replayPath);
                s.Scenario = Normalize(setup.Scenario);
                s.PageMode = Normalize(setup.PageMode);
                s.IsShared = s.PageMode == PageModeShared;
                s.Arm = setup.ArmLabel ?? "";
                s.StartTurn = setup.StartTurn;
                s.SeekTurn = Math.Max(0, setup.StartTurn - 1);
                s.InitialFromTurn = setup.StartTurn <= 1 ? -1 : s.SeekTurn;

                /* The first reading primes rate counters (Windows needs two samples
                   500 ms apart); the second is the suite-start reading */
                GHThermalProbe.Read();
                await Task.Delay(ThermalPrimeMs);
                s.StartThermal = GHThermalProbe.Read();
                s.StartThermalRank = ThermalRank(s.StartThermal, s.StartThermal);

                s.ReplaySha256 = GHPerformanceSuiteStore.ComputeSha256(replayPath);
                s.ReplayBytes = new FileInfo(replayPath).Length;
                s.SuiteId = GHPerformanceSuiteStore.CreateSuite(setup, s.ReplaySha256, s.ReplayBytes);
            }
            catch (Exception ex)
            {
                Interlocked.Exchange(ref _isRunning, 0);
                return "Could not create the performance suite: " + ex.Message;
            }
            if (string.IsNullOrEmpty(s.SuiteId))
            {
                Interlocked.Exchange(ref _isRunning, 0);
                return "Could not create the performance suite.";
            }
            CurrentSuiteId = s.SuiteId;
            Log("started " + s.SuiteId + " (" + s.Scenario + ", " + s.PageMode + ", " + setup.Runs + " runs"
                + (setup.WarmUpRun ? " + warm-up" : "") + ", thermal class " + ThermalClass(s.StartThermal, s.StartThermal) + ")");

            bool profilerWasEnabled = FrameTimeProfiler.IsEnabled;
            FrameTimeProfiler.IsEnabled = true;
            bool aborted = false;
            string abortReason = null;
            try
            {
                await RunSuiteAsync(s);
            }
            catch (SuiteAbortException ex)
            {
                aborted = true;
                abortReason = ex.Reason;
            }
            catch (Exception ex)
            {
                aborted = true;
                abortReason = string.IsNullOrEmpty(ex.Message) ? ex.GetType().Name : ex.Message;
            }
            finally
            {
                if (aborted)
                {
                    Log("aborted " + s.SuiteId + ": " + abortReason);
                    CloseOpenWindow(s, abortReason);
                }
                try
                {
                    await CloseGamePageAsync(s);
                }
                catch (Exception ex)
                {
                    Log("closing the game page failed: " + ex.Message);
                }
                FrameTimeProfiler.IsEnabled = profilerWasEnabled;
                try
                {
                    GHPerformanceSuiteStore.FinishSuite(s.SuiteId, aborted ? "aborted" : "complete", aborted ? abortReason : null);
                }
                catch (Exception ex)
                {
                    Log("finishing " + s.SuiteId + " failed: " + ex.Message);
                }
                CurrentSuiteId = null;
                Interlocked.Exchange(ref _isRunning, 0);
            }

            if (onFinished != null)
            {
                try
                {
                    await onFinished(s.SuiteId);
                }
                catch (Exception ex)
                {
                    Log("onFinished failed: " + ex.Message);
                }
            }
            return null;
        }

        private static string ValidateSetup(GHPerformanceSuiteSetup setup)
        {
            if (setup == null)
                return "No suite setup was given.";
            if (string.IsNullOrWhiteSpace(setup.ReplayPath))
                return "No replay file was selected.";
            string scenario = Normalize(setup.Scenario);
            if (scenario != ScenarioIdle && scenario != ScenarioMinimap && scenario != ScenarioPlayback)
                return "Unknown scenario: " + (setup.Scenario ?? "(none)") + ".";
            string pageMode = Normalize(setup.PageMode);
            if (pageMode != PageModeShared && pageMode != PageModeFresh)
                return "Unknown page mode: " + (setup.PageMode ?? "(none)") + ".";
            if (setup.Runs < 1)
                return "The number of runs must be at least 1.";
            if (setup.StartTurn < 0)
                return "The start turn cannot be negative.";
            if (setup.WindowSeconds < 1 || setup.WindowSeconds > MaxSeconds)
                return "The window must be between 1 and " + MaxSeconds + " seconds.";
            if (setup.WarmUpSeconds < 0 || setup.WarmUpSeconds > MaxSeconds)
                return "The warm-up must be between 0 and " + MaxSeconds + " seconds.";
            if (setup.CooldownSeconds < 0 || setup.CooldownSeconds > MaxSeconds)
                return "The cool-down must be between 0 and " + MaxSeconds + " seconds.";
            return null;
        }

        private static string Normalize(string value)
        {
            return value == null ? "" : value.Trim().ToLowerInvariant();
        }

        private static async Task RunSuiteAsync(SuiteState s)
        {
            GHPerformanceSuiteSetup setup = s.Setup;
            int firstRunIndex = setup.WarmUpRun ? 0 : 1;
            int lastRunIndex = setup.Runs;

            await OpenGamePageAsync(s);
            for (int runIndex = firstRunIndex; runIndex <= lastRunIndex; runIndex++)
            {
                bool isWarmUp = runIndex == 0;

                /* Pausing while a replayed window or prompt is open would freeze it on screen
                   (its hide never comes) and hide the map for the whole window */
                bool reached = await WaitUntilAsync(s,
                    delegate { return !GHApp.IsReplaySearching && GHApp.ReplayTurn >= s.StartTurn && !s.ActivePage.IsOverlayOpen; },
                    StartTurnPollMs, StartTurnTimeoutMs, true);
                if (!reached)
                    throw new SuiteAbortException("could not reach the start turn");

                if (s.Scenario == ScenarioIdle || s.Scenario == ScenarioMinimap)
                {
                    /* Pause at the next replayed input record: the recorded player is being
                       prompted for a command, so the map is drawn and nothing else is pending.
                       The replay thread sleeps there for ReplayStandardDelay and checks the pause
                       before its next record, so the replay freezes at that prompt. The start
                       turn alone can come before the map is first drawn, e.g. turn 1 before the
                       game's intro. An input record that dismisses a window does not count, so
                       the count restarts while a window or prompt is open. */
                    long inputRecords = GHApp.ReplayInputRecordCount;
                    bool prompted = await WaitUntilAsync(s,
                        delegate
                        {
                            if (s.ActivePage.IsOverlayOpen)
                            {
                                inputRecords = GHApp.ReplayInputRecordCount;
                                return false;
                            }
                            return GHApp.ReplayInputRecordCount > inputRecords;
                        },
                        StartTurnPollMs, StartTurnTimeoutMs, true);
                    if (!prompted)
                        throw new SuiteAbortException("the replay did not reach a player prompt");
                }

                if (s.Scenario == ScenarioIdle)
                {
                    s.ActivePage.SetReplayPaused(true);
                }
                else if (s.Scenario == ScenarioMinimap)
                {
                    s.ActivePage.SetReplayPaused(true);
                    s.ActivePage.SetZoomMini();
                }

                s.ActivePage.SetReplayHeaderOverride(isWarmUp ? "Performance suite: warm-up run"
                    : "Performance suite: run " + runIndex + " of " + setup.Runs);

                await WaitAsync(s, setup.WarmUpSeconds * 1000L, true);
                await MeasureAsync(s, runIndex, isWarmUp);

                if (runIndex < lastRunIndex)
                    await PrepareNextRunAsync(s);
            }
        }

        private static async Task MeasureAsync(SuiteState s, int runIndex, bool isWarmUp)
        {
            GHPerformanceRunContext ctx = new GHPerformanceRunContext();
            ctx.SuiteId = s.SuiteId;
            ctx.RunIndex = runIndex;
            ctx.PageMode = s.PageMode;
            ctx.IsWarmUp = isWarmUp;
            ctx.ReplayFileName = s.ReplayFileName;
            ctx.ReplayBytes = s.ReplayBytes;
            ctx.ReplaySha256 = s.ReplaySha256;
            ctx.StartTurn = s.StartTurn;
            if (isWarmUp)
                ctx.ExcludedReason = "warm-up run";

            GHPerformanceRunRecord.BeginWindow(s.Scenario, s.Arm, ctx);
            if (!GHPerformanceRunRecord.IsWindowOpen)
                throw new SuiteAbortException("the measurement window could not be opened");
            s.OpenWindow = ctx;

            await WaitAsync(s, s.Setup.WindowSeconds * 1000L, true);

            s.OpenWindow = null;
            ctx.TurnReached = GHApp.ReplayTurn;
            SaveWindow(s, ctx);
        }

        /* Cool-down, thermal gate and the seek (shared) or page swap (fresh) that
           precede the next run */
        private static async Task PrepareNextRunAsync(SuiteState s)
        {
            GHPerformanceSuiteSetup setup = s.Setup;
            if (s.IsShared)
            {
                s.ActivePage.SetReplayHeaderOverride("Performance suite: cooling down");
                s.ActivePage.SetReplayPaused(true);
                await WaitAsync(s, setup.CooldownSeconds * 1000L, true);
                await ThermalGateAsync(s, true);

                s.ActivePage.SetReplayPaused(false);
                if (s.Scenario == ScenarioMinimap)
                    s.ActivePage.ExitZoomMini();
                s.ActivePage.SetReplayHeaderOverride(null);
                /* A turn earlier than the current one restarts the replay in place
                   through GamePage.RestartReplay, which also collects garbage */
                GHApp.GoToTurn = s.SeekTurn;
            }
            else
            {
                s.ActivePage.SetReplayPaused(false);
                if (!await StopGamePageAsync(s))
                    throw new SuiteAbortException("the replay did not stop");
                await WaitAsync(s, setup.CooldownSeconds * 1000L, false);
                await ThermalGateAsync(s, false);

                GHApp.CollectGarbage();
                await WaitAsync(s, AfterGarbageCollectionMs, false);
                await OpenGamePageAsync(s);
            }
        }

        private static async Task OpenGamePageAsync(SuiteState s)
        {
            var mainPage = GHApp.CurrentMainPage;
            if (mainPage == null)
                throw new SuiteAbortException("the main page is not available");

            s.PageBelow = GHApp.PageFromTopOfModalNavigationStack();
            GamePage page = new GamePage(mainPage);
            await GHApp.PushModalPageAsync(page);
            if (GHApp.PageFromTopOfModalNavigationStack() != page)
                throw new SuiteAbortException("the game page could not be opened");
            s.ActivePage = page;

            /* From the beginning without a search, as the Replay page starts, when the
               start turn is 1 or less; otherwise one turn before the start turn, whose
               last turn then plays at normal speed */
            await page.StartReplay(s.ReplayPath, s.InitialFromTurn);
            s.RunnerStopping = false;
            if (!GHApp.GameStarted || GHApp.StopReplay)
                throw new SuiteAbortException("the replay could not be started");
        }

        /* Stops the replay if it is still running and waits, with no abort checks,
           until the game page has left the modal stack. Returns false on timeout. */
        private static async Task<bool> StopGamePageAsync(SuiteState s)
        {
            if (s.ActivePage == null)
                return true;
            if (GHApp.GameStarted)
            {
                s.RunnerStopping = true;
                GHApp.StopReplay = true;
            }
            Page pageBelow = s.PageBelow;
            bool gone = await WaitUntilAsync(s,
                delegate { return !GHApp.GameStarted && GHApp.PageFromTopOfModalNavigationStack() == pageBelow; },
                PollMs, PageGoneTimeoutMs, false);
            if (gone)
                s.ActivePage = null;
            return gone;
        }

        private static async Task CloseGamePageAsync(SuiteState s)
        {
            if (s.ActivePage == null)
                return;
            s.ActivePage.SetReplayHeaderOverride(null);
            s.ActivePage.SetReplayPaused(false);
            if (!await StopGamePageAsync(s))
            {
                Log("the game page did not close within " + (PageGoneTimeoutMs / 1000) + " s");
                s.ActivePage = null;
            }
        }

        /* Waits until the thermal state is no worse than at the suite start, polling
           every 15 s for at most 300 s; on timeout the next run goes ahead anyway */
        private static async Task ThermalGateAsync(SuiteState s, bool gameExpected)
        {
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                GHThermalReading now = GHThermalProbe.Read();
                if (ThermalRank(now, s.StartThermal) <= s.StartThermalRank)
                    return;
                if (sw.ElapsedMilliseconds >= ThermalGateTimeoutMs)
                {
                    Log("thermal gate timed out in " + s.SuiteId + ": " + ThermalClass(now, s.StartThermal)
                        + " vs " + ThermalClass(s.StartThermal, s.StartThermal) + " at the suite start");
                    return;
                }
                await WaitAsync(s, ThermalGatePollMs, gameExpected);
            }
        }

        /* The platform thermal status when known; otherwise (Windows) "throttled" when
           the CPU performance percent is more than 10 points below the suite start,
           else "nominal" */
        private static string ThermalClass(GHThermalReading reading, GHThermalReading suiteStart)
        {
            if (reading.Status != GHThermalStatus.Unknown)
                return GHThermalProbe.StatusName(reading.Status);
            return IsCpuThrottled(reading, suiteStart) ? "throttled" : "nominal";
        }

        /* ThermalClass as an ordinal comparable across both kinds of class: a known
           status by its enum value, "nominal" as Nominal, "throttled" as Severe */
        private static int ThermalRank(GHThermalReading reading, GHThermalReading suiteStart)
        {
            if (reading.Status != GHThermalStatus.Unknown)
                return (int)reading.Status;
            return IsCpuThrottled(reading, suiteStart) ? (int)GHThermalStatus.Severe : (int)GHThermalStatus.Nominal;
        }

        private static bool IsCpuThrottled(GHThermalReading reading, GHThermalReading suiteStart)
        {
            float now = reading.CpuPerformancePct;
            float start = suiteStart.CpuPerformancePct;
            if (float.IsNaN(now) || float.IsNaN(start))
                return false;
            return now < start - CpuThrottleDropPct;
        }

        /* Ends a window left open by an abort, excluded with the abort reason */
        private static void CloseOpenWindow(SuiteState s, string reason)
        {
            GHPerformanceRunContext ctx = s.OpenWindow;
            if (ctx == null)
                return;
            s.OpenWindow = null;
            try
            {
                ctx.ExcludedReason = string.IsNullOrEmpty(reason) ? "aborted" : reason;
                ctx.TurnReached = GHApp.ReplayTurn;
                SaveWindow(s, ctx);
            }
            catch (Exception ex)
            {
                Log("saving the open window failed: " + ex.Message);
            }
        }

        private static void SaveWindow(SuiteState s, GHPerformanceRunContext ctx)
        {
            GHPerformanceRunResult result;
            GHPerformanceRunRecord.EndWindowAndSave(GHPerformanceSuiteStore.SuiteDirectory(s.SuiteId), out result);
            if (result == null)
            {
                Log("run " + ctx.RunIndex + " of " + s.SuiteId + " could not be saved");
                return;
            }
            GHPerformanceSuiteStore.AddRun(s.SuiteId, ctx, result);
        }

        /* Waits about ms milliseconds in steps of at most 100 ms, checking for an abort
           before every step and after the last */
        private static async Task WaitAsync(SuiteState s, long ms, bool gameExpected)
        {
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                CheckAbort(s, gameExpected);
                long remaining = ms - sw.ElapsedMilliseconds;
                if (remaining <= 0)
                    return;
                await Task.Delay((int)Math.Min(PollMs, remaining));
            }
        }

        /* Polls condition every pollMs until it holds (true) or timeoutMs passes
           (false). With checkAbort, a game is expected and every poll first checks for
           an abort; without it nothing aborts the wait. */
        private static async Task<bool> WaitUntilAsync(SuiteState s, Func<bool> condition, int pollMs, int timeoutMs,
            bool checkAbort)
        {
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                if (checkAbort)
                    CheckAbort(s, true);
                if (condition())
                    return true;
                if (sw.ElapsedMilliseconds >= timeoutMs)
                    return false;
                await Task.Delay(pollMs);
            }
        }

        /* With a game expected: the user stopping the replay (StopReplay set by
           someone other than the runner), the replay ending, or the app going to the
           background. Between fresh pages, with no game expected: the app going to
           the background, or a game or page started over the one below the game
           page. */
        private static void CheckAbort(SuiteState s, bool gameExpected)
        {
            if (gameExpected)
            {
                if (!s.RunnerStopping && GHApp.StopReplay)
                    throw new SuiteAbortException("stopped by the user");
                if (!GHApp.GameStarted)
                    throw new SuiteAbortException("replay ended");
                if (GHApp.IsSuspended)
                    throw new SuiteAbortException("app went to the background");
            }
            else
            {
                if (GHApp.IsSuspended)
                    throw new SuiteAbortException("app went to the background");
                if (GHApp.GameStarted || GHApp.PageFromTopOfModalNavigationStack() != s.PageBelow)
                    throw new SuiteAbortException("interrupted by the user");
            }
        }

        private static void Log(string text)
        {
            try
            {
                GHApp.MaybeWriteGHLog("Performance suite: " + text);
            }
            catch
            {
                /* Logging must never break the suite */
            }
        }

        private sealed class SuiteAbortException : Exception
        {
            public readonly string Reason;

            public SuiteAbortException(string reason) : base(reason)
            {
                Reason = reason;
            }
        }

        private sealed class SuiteState
        {
            public GHPerformanceSuiteSetup Setup;
            public string ReplayPath;
            public string ReplayFileName;
            public string Scenario;
            public string PageMode;
            public bool IsShared;
            public string Arm;
            public int StartTurn;
            public int SeekTurn;                       /* target of a shared page's seek back */
            public int InitialFromTurn;                /* -1 plays from the beginning without a search */
            public string SuiteId;
            public string ReplaySha256;
            public long ReplayBytes;
            public GHThermalReading StartThermal;
            public int StartThermalRank;

            public GamePage ActivePage;                 /* null when no game page of the suite is open */
            public Page PageBelow;                     /* top of the modal stack before the game page was pushed */
            public bool RunnerStopping;                /* StopReplay was set by the runner */
            public GHPerformanceRunContext OpenWindow; /* non-null while a window is open */
        }
    }
}

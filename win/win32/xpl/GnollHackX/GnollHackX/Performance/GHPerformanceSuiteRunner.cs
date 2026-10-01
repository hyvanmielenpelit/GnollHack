using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
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
    /* What a performance suite is waiting for while no game page of it is open, as
       GHPerformanceSuiteRunner.RunAsync hands it to its onStatus: a title, a detail line,
       the progress from 0 to 1 (-1 when unknown), and whether SkipWait ends the wait */
    public sealed class GHSuiteWaitStatus
    {
        public string Title;
        public string Detail;
        public double Progress = -1;
        public bool CanSkip;
    }

    /* Runs a performance suite: a series of measurement windows over one replay file,
       each started from the same turn, saved through GHPerformanceRunRecord and
       registered in GHPerformanceSuiteStore.

       Run 0 is the optional warm-up run (saved but excluded); runs 1..Runs are
       measured; with the warm-up run off, run 1 is saved but excluded as a cold first
       run. A new game page plays the replay from its beginning as the Replay page
       does when the start turn is 1 or less, and otherwise seeks to StartTurn - 1 and
       lets it play the last turn at normal speed; a shared page restarts in place by
       seeking back to StartTurn - 1. Per run: wait for the start turn and for any
       replayed window or prompt to close, apply the scenario (idle pauses
       the replay, minimap pauses it and switches to the minimap, playback lets it
       play), warm up, measure one window, and save it; a window the window gate
       refuses is registered as a run excluded with "measurement window refused", and
       the suite goes on. Before the first run, with no game page open: cool down, wait
       for the platform thermal status to be Light or better (at most the setup's thermal
       wait seconds; none when 0; no wait where the platform reports none), take the
       suite-start thermal reading, and wait for a quiet system. Between runs: cool down, wait
       for the platform thermal status to be no worse than at the suite start, or Light or
       better, as the setup's thermal gate says (at most the setup's thermal wait seconds;
       none when 0; no wait where the platform reports none), wait for a quiet system (at most
       30 s; once it has timed out or been skipped, the later gates do not wait), then
       either seek the same game page back
       (shared) or close it and open a new one (fresh; the cool-down then happens on the
       page below the game page). The replay header shows the run and its phase; while no
       game page is open, the caller's onStatus gets each wait (GHSuiteWaitStatus), and
       the user can skip a wait (SkipWait) or cancel the suite (CancelSuite). Each game
       page runs in performance suite mode (GamePage.EnterPerformanceSuiteMode): the view is
       fixed at the device's default zoom with auto-center on and no overlays, and the viewer
       cannot change it; the frame time profiler and the debug dashboard are on for the whole
       suite, whatever the settings say, and the dashboard and the frame marker are hidden
       during each warm-up and measurement window. The background load sampler runs for the whole
       suite; the environment fingerprint is captured at its start (CreateSuite), replaced
       once its first window is saved (UpdateEnvironment), and captured again at its end
       (FinishSuite).

       Everything runs on the UI thread as one async task. Every wait polls at least
       every 100 ms and aborts the suite when the replay ended (GHApp.GameStarted went
       false), the user stopped the replay or cancelled the suite, or the app went to the
       background; a
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
        private const int QuietGatePollMs = 1000;
        private const int AfterGarbageCollectionMs = 500;
        private const int MaxSeconds = 24 * 60 * 60;

        private const string ScenarioIdle = "idle";
        private const string ScenarioMinimap = "minimap";
        private const string ScenarioPlayback = "playback";
        private const string PageModeShared = "shared";
        private const string PageModeFresh = "fresh";

        private static int _isRunning = 0;
        private static int _skipRequested = 0;          /* 1 once SkipWait asked to end the current wait */
        private static int _cancelRequested = 0;        /* 1 once CancelSuite asked to end the suite */

        /* The abort reason of a suite the user cancelled (CancelSuite) */
        public const string CancelledReason = "cancelled by the user";
        private static string _currentSuiteId = null;

        public static bool IsRunning { get { return Interlocked.CompareExchange(ref _isRunning, 0, 0) != 0; } }

        public static string CurrentSuiteId
        {
            get { return Interlocked.CompareExchange(ref _currentSuiteId, null, null); }
            private set { Interlocked.Exchange(ref _currentSuiteId, value); }
        }

        /* Estimated wall time of the setup: (runs + warm-up run) * (seek allowance 15 s
           + warm-up + window + cool-down), the cool-downs being the one before the first
           run and those between runs; thermal and quiet gate waits are not included */
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
           running, another measurement in progress, invalid setup, a game already
           running, replay missing or invalid).
           onFinished(suiteId) is awaited on the UI thread after the suite ends and its
           game page is gone; it is not called when the suite could not start. */
        public static Task<string> RunAsync(GHPerformanceSuiteSetup setup, Func<string, Task> onFinished)
        {
            return RunAsync(setup, onFinished, null);
        }

        /* Same as RunAsync(setup, onFinished); onStatus, on the UI thread, receives what
           the suite waits for while no game page of it is open (the preparation, the
           cool-down and the gates before the first run, and between fresh pages), and
           null when a game page opens or the suite has ended */
        public static async Task<string> RunAsync(GHPerformanceSuiteSetup setup, Func<string, Task> onFinished,
            Action<GHSuiteWaitStatus> onStatus)
        {
            if (IsRunning)
                return "A performance suite is already running.";
            if (GHPerformanceDiagnosticRunner.IsRunning || GHPerformanceRunRecord.IsWindowOpen
                || GHPerformanceRunRecord.IsWindowCommandPending)
                return "Another performance measurement is in progress.";
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

            Interlocked.Exchange(ref _cancelRequested, 0);
            SuiteState s = new SuiteState();
            s.OnStatus = onStatus;
            ReportStatus(s, "Preparing");
            /* Before the first environment capture, so every capture of the suite sees them */
            ApplyOverrides(s);
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
                s.ThermalGate = GHPerformanceSuiteLogic.NormalizeThermalGate(setup.ThermalGate);
                s.ThermalWaitMs = setup.ThermalWaitSeconds * 1000L;

                /* The first environment capture can block for seconds (WMI); later ones,
                   CreateSuite's, UpdateEnvironment's and every run record's, read its cache */
                await Task.Run(delegate { GHPerformanceEnvironment.CaptureFingerprint(false); });

                /* The first reading primes rate counters (Windows needs two samples
                   500 ms apart); the suite-start reading is taken after the first-run
                   gate (FirstRunGateAsync) */
                GHThermalProbe.Read();
                await Task.Delay(ThermalPrimeMs);

                /* Held for the whole suite, so the quiet gate and every pre-window have
                   samples */
                if (GHSystemLoadSampler.Enabled)
                {
                    GHSystemLoadSampler.Acquire();
                    s.SamplerHeld = true;
                    s.SamplerClock = Stopwatch.StartNew();
                }

                s.ReplaySha256 = GHPerformanceSuiteStore.ComputeSha256(replayPath);
                s.ReplayBytes = new FileInfo(replayPath).Length;
                s.SuiteId = GHPerformanceSuiteStore.CreateSuite(setup, s.ReplaySha256, s.ReplayBytes);
            }
            catch (Exception ex)
            {
                ReleaseSampler(s);
                RestoreOverrides(s);
                HideStatus(s);
                Interlocked.Exchange(ref _isRunning, 0);
                return "Could not create the performance suite: " + ex.Message;
            }
            if (string.IsNullOrEmpty(s.SuiteId))
            {
                ReleaseSampler(s);
                RestoreOverrides(s);
                HideStatus(s);
                Interlocked.Exchange(ref _isRunning, 0);
                return "Could not create the performance suite.";
            }
            CurrentSuiteId = s.SuiteId;
            Log("started " + s.SuiteId + " (" + s.Scenario + ", " + s.PageMode + ", " + setup.Runs + " runs"
                + (setup.WarmUpRun ? " + warm-up" : "") + ", thermal gate " + s.ThermalGate + " " + setup.ThermalWaitSeconds + " s)");

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
                /* Re-read, WMI included, off the UI thread, with the overrides still applied
                   as they were at the suite-start capture */
                Dictionary<string, string> fingerprintAtEnd = null;
                try
                {
                    fingerprintAtEnd = await Task.Run(delegate { return GHPerformanceEnvironment.CaptureFingerprint(true); });
                }
                catch (Exception ex)
                {
                    Log("capturing the environment at the end of " + s.SuiteId + " failed: " + ex.Message);
                }
                try
                {
                    GHPerformanceSuiteStore.FinishSuite(s.SuiteId, aborted ? "aborted" : "complete", aborted ? abortReason : null,
                        fingerprintAtEnd);
                }
                catch (Exception ex)
                {
                    Log("finishing " + s.SuiteId + " failed: " + ex.Message);
                }
                RestoreOverrides(s);
                ReleaseSampler(s);
                HideStatus(s);
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
            string windowRefusal = WindowLengthRefusal(setup.WindowSeconds);
            if (windowRefusal != null)
                return windowRefusal;
            if (setup.WarmUpSeconds < 0 || setup.WarmUpSeconds > MaxSeconds)
                return "The warm-up must be between 0 and " + MaxSeconds + " seconds.";
            if (setup.CooldownSeconds < 0 || setup.CooldownSeconds > MaxSeconds)
                return "The cool-down must be between 0 and " + MaxSeconds + " seconds.";
            if (GHPerformanceSuiteLogic.NormalizeThermalGate(setup.ThermalGate) == null)
                return "Unknown thermal gate: " + setup.ThermalGate + ".";
            if (setup.ThermalWaitSeconds < 0 || setup.ThermalWaitSeconds > MaxSeconds)
                return "The thermal wait must be between 0 and " + MaxSeconds + " seconds.";
            return null;
        }

        /* Why a window of windowSeconds cannot be measured, or null when it can: the frame
           timeline holds at most GHFrameTimeline.MaxWindowSeconds at the current refresh
           rate (GHApp.ReconciledRefreshRate). The Performance Suite page checks the same. */
        public static string WindowLengthRefusal(double windowSeconds)
        {
            double hz = GHApp.ReconciledRefreshRate;
            double maxSeconds = Math.Floor(GHFrameTimeline.MaxWindowSeconds(hz));
            if (windowSeconds <= maxSeconds)
                return null;
            return "The window can be at most " + maxSeconds.ToString(CultureInfo.InvariantCulture)
                + " seconds at the current refresh rate of " + Math.Round(hz).ToString(CultureInfo.InvariantCulture)
                + " Hz, which is as long as the frame timeline holds.";
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

            /* No game page yet: the waits abort when another page opens over this one */
            s.PageBelow = GHApp.PageFromTopOfModalNavigationStack();
            await FirstRunGateAsync(s);
            await QuietGateAsync(s, false);

            ReportStatus(s, "Opening the replay");
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

                s.RunLabel = isWarmUp ? "warm-up run" : "run " + runIndex + " of " + setup.Runs;
                /* The frame marker and the dashboard stay hidden through the warm-up and
                   the window, so the measured frames draw neither */
                GHFrameMarker.Suppressed = true;
                GHDebugDashboard.Suppressed = true;
                try
                {
                    SetPhase(s, "warming up " + setup.WarmUpSeconds + " s");
                    await WaitAsync(s, setup.WarmUpSeconds * 1000L, true);

                    /* Set before the window opens, so the label is unchanged throughout it */
                    SetPhase(s, "measuring " + setup.WindowSeconds + " s");
                    await MeasureAsync(s, runIndex, isWarmUp);
                }
                finally
                {
                    GHFrameMarker.Suppressed = false;
                    GHDebugDashboard.Suppressed = false;
                }

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
            else if (!s.Setup.WarmUpRun && runIndex == 1)
                ctx.ExcludedReason = "cold first run (warm-up run off)";
            ctx.Notes = s.PendingNote;
            s.PendingNote = null;

            /* The per-process begin collect runs on the thread pool; the window opens
               once it is done, or after ProcessIntervalBeginWaitMs at most */
            Task begin = GHSystemLoadSampler.StartProcessIntervalAsync();
            await WaitUntilAsync(s, delegate { return begin.IsCompleted; }, StartTurnPollMs,
                GHSystemLoadSampler.ProcessIntervalBeginWaitMs, true);

            /* A window the gate refuses (another measurement's) is recorded as an excluded
               run, and the suite goes on */
            string refusal = GHPerformanceRunRecord.BeginRefusal(GHWindowOwner.Suite);
            if (refusal != null)
            {
                Log("run " + runIndex + " of " + s.SuiteId + ": measurement window refused (" + refusal + ")");
                ctx.ExcludedReason = "measurement window refused";
                ctx.TurnReached = GHApp.ReplayTurn;
                GHPerformanceSuiteStore.AddRun(s.SuiteId, ctx, null);
                return;
            }
            ctx.TurnAtWindowStart = GHApp.ReplayTurn;
            s.InputRecordsAtWindowStart = GHApp.ReplayInputRecordCount;
            if (!GHPerformanceRunRecord.TryBeginWindow(s.Scenario, s.Arm, ctx, GHWindowOwner.Suite))
                throw new SuiteAbortException("the measurement window could not be opened");
            s.OpenWindow = ctx;

            await WaitAsync(s, s.Setup.WindowSeconds * 1000L, true);

            s.OpenWindow = null;
            SetWindowEnd(s, ctx);
            SaveWindow(s, ctx);
        }

        /* The turn reached and the input records played since the window opened; the
           latter is left unknown when the count went backwards */
        private static void SetWindowEnd(SuiteState s, GHPerformanceRunContext ctx)
        {
            ctx.TurnReached = GHApp.ReplayTurn;
            long played = GHApp.ReplayInputRecordCount - s.InputRecordsAtWindowStart;
            ctx.InputRecordsInWindow = played >= 0 ? played : -1;
        }

        /* Cool-down, thermal gate and the seek (shared) or page swap (fresh) that
           precede the next run */
        private static async Task PrepareNextRunAsync(SuiteState s)
        {
            GHPerformanceSuiteSetup setup = s.Setup;
            if (s.IsShared)
            {
                SetPhase(s, "cooling down " + setup.CooldownSeconds + " s");
                s.ActivePage.SetReplayPaused(true);
                await WaitAsync(s, setup.CooldownSeconds * 1000L, true);
                await ThermalGateAsync(s, true);
                await QuietGateAsync(s, true);

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
                await CountdownAsync(s, setup.CooldownSeconds);
                await ThermalGateAsync(s, false);
                await QuietGateAsync(s, false);

                GHApp.CollectGarbage();
                await WaitAsync(s, AfterGarbageCollectionMs, false);
                ReportStatus(s, "Opening the replay");
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
            page.EnterPerformanceSuiteMode();
            await GHApp.PushModalPageAsync(page);
            if (GHApp.PageFromTopOfModalNavigationStack() != page)
                throw new SuiteAbortException("the game page could not be opened");
            s.ActivePage = page;
            HideStatus(s);

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

        /* Before the first run, the warm-up run included, with no game page open: waits
           the cool-down, then the thermal gate against Light (many devices never report
           Nominal, e.g. while charging), and then takes the suite-start thermal reading
           the gate between runs compares against. */
        private static async Task FirstRunGateAsync(SuiteState s)
        {
            await CountdownAsync(s, s.Setup.CooldownSeconds);
            await ThermalGateAsync(s, GHThermalStatus.Light, "Light before the first run", false);
            s.StartThermal = GHThermalProbe.Read();
            Log("thermal status at the start of " + s.SuiteId + ": " + GHThermalProbe.StatusName(s.StartThermal.Status));
        }

        /* The thermal gate between runs: no worse than the status at the suite start, or
           Light or better, as the setup's thermal gate says */
        private static Task ThermalGateAsync(SuiteState s, bool gameExpected)
        {
            if (s.ThermalGate == GHPerformanceSuiteLogic.ThermalGateLight)
                return ThermalGateAsync(s, GHThermalStatus.Light, "Light (setup)", gameExpected);
            return ThermalGateAsync(s, s.StartThermal.Status,
                GHThermalProbe.StatusName(s.StartThermal.Status) + " at the suite start", gameExpected);
        }

        /* Waits until the platform thermal status is no worse than limit, polling every
           15 s for at most the setup's thermal wait seconds (none when 0); on timeout the
           next run goes ahead anyway and carries a note saying so. Without a
           platform thermal status (Windows) there is no gate: the processor performance
           percent mostly reflects turbo boost, which falls whenever the replay is paused,
           so it cannot tell a throttled machine from an idle one. Runs measured while
           throttled are still excluded by the per-run rule. The user can skip the wait
           (SkipWait). limitText names the limit in the log. */
        private static async Task ThermalGateAsync(SuiteState s, GHThermalStatus limit, string limitText, bool gameExpected)
        {
            if (s.ThermalWaitMs <= 0 || limit == GHThermalStatus.Unknown)
                return;
            int waitSeconds = (int)(s.ThermalWaitMs / 1000);
            ClearSkip();
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                GHThermalReading now = GHThermalProbe.Read();
                if (now.Status == GHThermalStatus.Unknown || now.Status <= limit)
                    return;
                if (sw.ElapsedMilliseconds >= s.ThermalWaitMs)
                {
                    Log("thermal gate timed out in " + s.SuiteId + ": " + GHThermalProbe.StatusName(now.Status)
                        + " vs " + limitText);
                    AddPendingNote(s, "thermal gate timed out after " + waitSeconds.ToString(CultureInfo.InvariantCulture)
                        + " s (" + GHThermalProbe.StatusName(now.Status) + " vs " + GHThermalProbe.StatusName(limit) + ")");
                    return;
                }
                /* Reports the progress each second between the 15 s thermal readings; the last
                   reading is taken when the wait runs out */
                long pollEnd = Math.Min(sw.ElapsedMilliseconds + ThermalGatePollMs, s.ThermalWaitMs);
                while (sw.ElapsedMilliseconds < pollEnd)
                {
                    long elapsedSeconds = sw.ElapsedMilliseconds / 1000;
                    SetPhase(s, GHPerformanceSuiteLogic.ThermalGatePhase(GHThermalProbe.StatusName(now.Status),
                        GHThermalProbe.StatusName(limit), elapsedSeconds, waitSeconds));
                    string detail = "Thermal status " + GHThermalProbe.StatusName(now.Status) + ", waiting for "
                        + GHThermalProbe.StatusName(limit) + " or better ("
                        + elapsedSeconds.ToString(CultureInfo.InvariantCulture) + "/"
                        + waitSeconds.ToString(CultureInfo.InvariantCulture) + " s)";
                    ReportStatus(s, "Waiting for the device to cool", detail,
                        Math.Min(1.0, sw.ElapsedMilliseconds / (double)s.ThermalWaitMs), true);
                    if (await WaitOrSkipAsync(s, 1000L, gameExpected))
                    {
                        Log("thermal gate skipped by the user in " + s.SuiteId + ": " + GHThermalProbe.StatusName(now.Status)
                            + " vs " + limitText);
                        return;
                    }
                }
            }
        }

        /* Waits until the last GHBackgroundLoad.QuietWindowSeconds average other CPU and
           disk busy below the quiet thresholds (GHSystemLoadSampler.IsQuiet), polling
           every second for at most GHBackgroundLoad.QuietGateTimeoutSeconds; on timeout,
           or when the user skips the wait (SkipWait), the next run goes ahead and carries
           a note saying so, and the later gates of the suite do not wait: a machine that
           stayed busy once is taken to stay busy, and each of those runs is noted instead
           while the system is not quiet. The runs' background verdicts still flag or
           exclude a busy run. No gate without the sampler, or when it has produced no CPU
           sample QuietWindowSeconds + 1 s after the suite acquired it (no whole-machine
           CPU on the platform). */
        private static async Task QuietGateAsync(SuiteState s, bool gameExpected)
        {
            if (!s.SamplerHeld || s.SamplerClock == null)
                return;
            if (s.QuietGateGaveUp)
            {
                float busyCpu, busyDisk;
                if (GHSystemLoadSampler.HasCpuSignal && !GHSystemLoadSampler.IsQuiet(out busyCpu, out busyDisk))
                    AddPendingNote(s, "quiet gate skipped: the system stayed busy (other CPU " + FormatPercent(busyCpu) + " %)");
                return;
            }
            ClearSkip();
            ReportStatus(s, "Checking system load");
            long settleMs = (GHBackgroundLoad.QuietWindowSeconds + 1) * 1000L - s.SamplerClock.ElapsedMilliseconds;
            if (!GHSystemLoadSampler.HasCpuSignal && settleMs > 0)
                await WaitAsync(s, settleMs, gameExpected);
            if (!GHSystemLoadSampler.HasCpuSignal)
                return;

            Stopwatch sw = Stopwatch.StartNew();
            string shown = null;
            long timeoutMs = GHBackgroundLoad.QuietGateTimeoutSeconds * 1000L;
            while (true)
            {
                float otherCpuMean, diskBusyMean;
                if (GHSystemLoadSampler.IsQuiet(out otherCpuMean, out diskBusyMean))
                    return;
                string otherText = FormatPercent(otherCpuMean);
                if (sw.ElapsedMilliseconds >= timeoutMs)
                {
                    Log("quiet gate timed out in " + s.SuiteId + ": other CPU " + otherText + " %, disk busy "
                        + FormatPercent(diskBusyMean) + " %");
                    AddPendingNote(s, "quiet gate timed out (other CPU " + otherText + " %)");
                    s.QuietGateGaveUp = true;
                    return;
                }
                if (otherText != shown)
                {
                    SetPhase(s, "waiting for a quiet system (other CPU " + otherText + " %)");
                    shown = otherText;
                }
                ReportStatus(s, "Waiting for a quiet system", "Other CPU " + otherText + " %, waiting for under "
                    + GHBackgroundLoad.QuietOtherCpuPct.ToString("F0", CultureInfo.InvariantCulture) + " %",
                    Math.Min(1.0, sw.ElapsedMilliseconds / (double)timeoutMs), true);
                if (await WaitOrSkipAsync(s, QuietGatePollMs, gameExpected))
                {
                    Log("quiet gate skipped by the user in " + s.SuiteId + ": other CPU " + otherText + " %");
                    AddPendingNote(s, "quiet gate skipped by the user (other CPU " + otherText + " %)");
                    s.QuietGateGaveUp = true;
                    return;
                }
            }
        }

        /* Sets the next run's note, or appends to it after "; " when one is already pending */
        private static void AddPendingNote(SuiteState s, string note)
        {
            if (string.IsNullOrEmpty(s.PendingNote))
                s.PendingNote = note;
            else
                s.PendingNote += "; " + note;
        }

        private static string FormatPercent(float value)
        {
            if (float.IsNaN(value) || float.IsInfinity(value))
                return "?";
            return value.ToString("F0", CultureInfo.InvariantCulture);
        }

        /* The frame time profiler and the debug dashboard are on for the whole suite, whatever the
           settings say, with the dashboard hidden during each warm-up and window; runtime only,
           nothing is saved */
        private static void ApplyOverrides(SuiteState s)
        {
            FrameTimeProfiler.IsEnabled = true;
            GHApp.ForceDebugScreenLogging = true;
            s.OverridesApplied = true;
        }

        /* Returns the profiler to the Settings value, ends the dashboard override and shows the
           frame marker and the dashboard again, once */
        private static void RestoreOverrides(SuiteState s)
        {
            if (!s.OverridesApplied)
                return;
            s.OverridesApplied = false;
            GHFrameMarker.Suppressed = false;
            GHDebugDashboard.Suppressed = false;
            GHApp.ForceDebugScreenLogging = false;
            FrameTimeProfiler.IsEnabled = GHApp.IsFrameTimeProfilerOn;
        }

        /* Releases the suite's sampler hold, once */
        private static void ReleaseSampler(SuiteState s)
        {
            if (!s.SamplerHeld)
                return;
            s.SamplerHeld = false;
            GHSystemLoadSampler.Release();
        }

        /* The cool-down: waits seconds with no game page open, reporting the time left each
           second; the user can skip the rest (SkipWait) */
        private static async Task CountdownAsync(SuiteState s, int seconds)
        {
            ClearSkip();
            for (int left = seconds; left > 0; left--)
            {
                ReportStatus(s, "Cooling down", left.ToString(CultureInfo.InvariantCulture) + " s left",
                    (seconds - left) / (double)seconds, true);
                if (await WaitOrSkipAsync(s, 1000L, false))
                {
                    Log("cool-down skipped by the user in " + s.SuiteId + " with " + left.ToString(CultureInfo.InvariantCulture)
                        + " s left");
                    return;
                }
            }
        }

        /* Ends the current skippable wait (a cool-down, the thermal or the quiet gate),
           as offered through onStatus; ignored when no suite runs. Call on the UI thread. */
        public static void SkipWait()
        {
            if (IsRunning)
                Interlocked.Exchange(ref _skipRequested, 1);
        }

        /* Ends the running suite at its next abort check, as an abort with the reason
           CancelledReason: finished runs are kept, and an open window is saved excluded.
           Ignored when no suite runs. Call on the UI thread. */
        public static void CancelSuite()
        {
            if (IsRunning)
                Interlocked.Exchange(ref _cancelRequested, 1);
        }

        /* Drops a skip requested before the current wait began */
        private static void ClearSkip()
        {
            Interlocked.Exchange(ref _skipRequested, 0);
        }

        /* Waits about ms milliseconds as WaitAsync does; true, and the request consumed,
           as soon as the user skips the wait */
        private static async Task<bool> WaitOrSkipAsync(SuiteState s, long ms, bool gameExpected)
        {
            Stopwatch sw = Stopwatch.StartNew();
            while (true)
            {
                if (Interlocked.Exchange(ref _skipRequested, 0) == 1)
                    return true;
                long remaining = ms - sw.ElapsedMilliseconds;
                if (remaining <= 0)
                    return false;
                await WaitAsync(s, Math.Min(PollMs, remaining), gameExpected);
            }
        }

        /* Passes what the suite waits for to the caller's onStatus, only while no game
           page of the suite is open; never throws */
        private static void ReportStatus(SuiteState s, string title)
        {
            ReportStatus(s, title, null, -1, false);
        }

        private static void ReportStatus(SuiteState s, string title, string detail, double progress, bool canSkip)
        {
            if (s.ActivePage != null)
                return;
            GHSuiteWaitStatus status = new GHSuiteWaitStatus();
            status.Title = title;
            status.Detail = detail;
            status.Progress = progress;
            status.CanSkip = canSkip;
            SendStatus(s, status);
        }

        /* Tells the caller's onStatus that nothing is being waited for on its page: a game
           page opened, or the suite ended */
        private static void HideStatus(SuiteState s)
        {
            SendStatus(s, null);
        }

        private static void SendStatus(SuiteState s, GHSuiteWaitStatus status)
        {
            if (s.OnStatus == null)
                return;
            try
            {
                s.OnStatus(status);
            }
            catch (Exception ex)
            {
                Log("reporting the status failed: " + ex.Message);
            }
        }

        /* Shows the suite's progress in the replay header; set once per phase, so the label
           does not change during a measurement window */
        private static void SetPhase(SuiteState s, string phase)
        {
            if (s.ActivePage == null || string.IsNullOrEmpty(s.RunLabel))
                return;
            s.ActivePage.SetReplayHeaderOverride(char.ToUpperInvariant(s.RunLabel[0]) + s.RunLabel.Substring(1) + ": " + phase);
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
                SetWindowEnd(s, ctx);
                SaveWindow(s, ctx);
            }
            catch (Exception ex)
            {
                Log("saving the open window failed: " + ex.Message);
            }
        }

        /* Ends the window and registers the run. The suite's first saved window, warm-up
           or run 1, also replaces the provisional environment CreateSuite stored: by then
           the page has painted and the run record has captured the render adapter. */
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
            if (!s.EnvironmentUpdated)
            {
                s.EnvironmentUpdated = true;
                GHPerformanceSuiteStore.UpdateEnvironment(s.SuiteId);
            }
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
            if (Interlocked.CompareExchange(ref _cancelRequested, 0, 0) != 0)
                throw new SuiteAbortException(CancelledReason);
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
            public string ThermalGate;                 /* GHPerformanceSuiteLogic.ThermalGateStart or ThermalGateLight */
            public long ThermalWaitMs;                 /* longest wait of each thermal gate; 0 for none */
            public string RunLabel;                    /* "warm-up run" or "run i of n", for the header */

            public GamePage ActivePage;                 /* null when no game page of the suite is open */
            public Page PageBelow;                     /* top of the modal stack before the game page was pushed */
            public bool RunnerStopping;                /* StopReplay was set by the runner */
            public GHPerformanceRunContext OpenWindow; /* non-null while a window is open */
            public long InputRecordsAtWindowStart;     /* GHApp.ReplayInputRecordCount when the last window opened */
            public bool EnvironmentUpdated;            /* UpdateEnvironment has run after the first saved window */

            public bool OverridesApplied;              /* ApplyOverrides has run and RestoreOverrides has not */
            public bool SamplerHeld;                   /* the suite's GHSystemLoadSampler acquire is outstanding */
            public Stopwatch SamplerClock;             /* started at that acquire */
            public string PendingNote;                 /* for the next run's context, e.g. a quiet gate timeout */
            public Action<GHSuiteWaitStatus> OnStatus; /* what the suite waits for while no game page is open, null to hide; may be null */
            public bool QuietGateGaveUp;               /* a quiet gate timed out or was skipped; later gates do not wait */
        }
    }
}

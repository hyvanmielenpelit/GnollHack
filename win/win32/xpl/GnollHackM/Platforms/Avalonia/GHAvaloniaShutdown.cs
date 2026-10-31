#if GNH_AVALONIA
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using GnollHackX;

namespace GnollHackM
{
    /* Ends the process with the game saved, whichever way it was asked to end */
    internal static class GHAvaloniaShutdown
    {
        /* Long enough for the game thread to reach its next queue poll; a save already running gets the longer limit */
        private const int SaveStartTimeoutMs = 4000;
        private const int SaveFinishTimeoutMs = GHConstants.SavePollingTimeoutCount * GHConstants.PollingInterval;

        private static readonly object _saveLock = new object();
        private static bool _saveAttempted = false;
#if LINUX
        private const int SignalExitCode = 143;
        /* How long the normal shutdown may take after a signal; Main ends the process well before this */
        private const int SignalShutdownTimeoutMs = 5000;

        private static PosixSignalRegistration[] _signalRegistrations;
        private static int _signalReceived = 0;

        public static void RegisterSignals()
        {
            _signalRegistrations = new[]
            {
                PosixSignalRegistration.Create(PosixSignal.SIGTERM, OnSignal),
                PosixSignalRegistration.Create(PosixSignal.SIGINT, OnSignal),
                PosixSignalRegistration.Create(PosixSignal.SIGHUP, OnSignal),
            };
        }

        /* On a thread pool thread; only the first signal acts. The lifetime shuts down as for the Exit button, so that
           rendering has stopped when Main ends the process: exiting during GL calls can crash. Environment.Exit here
           covers a UI thread that does not respond. */
        private static void OnSignal(PosixSignalContext context)
        {
            context.Cancel = true;
            if (Interlocked.Exchange(ref _signalReceived, 1) != 0)
                return;
            SaveGameBeforeExit();
            try
            {
                Avalonia.Threading.Dispatcher.UIThread.Post(() =>
                {
                    (Avalonia.Application.Current?.ApplicationLifetime as Avalonia.Controls.ApplicationLifetimes.IClassicDesktopStyleApplicationLifetime)?.Shutdown(SignalExitCode);
                });
                Thread.Sleep(SignalShutdownTimeoutMs);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("GHAvaloniaShutdown: " + ex.Message);
            }
            Environment.Exit(SignalExitCode);
        }
#endif

        /* Saves a game in progress with a full save and leaves the game thread parked. Only the first caller saves;
           any other waits here until that save is over, so that no caller ends the process during it. Works without
           the UI thread. Never throws. */
        public static void SaveGameBeforeExit()
        {
            lock (_saveLock)
            {
                if (_saveAttempted)
                    return;
                _saveAttempted = true;
                try
                {
                    GHGame game = GHApp.CurrentGHGame;
                    if (game == null || game.PlayingReplay || game.IsWaitingForResume || !GHApp.GameStarted
                        || !(game.ActiveGamePage?.IsGameOn ?? false) || !(GHApp.GnhThread?.IsAlive ?? false))
                        return;

                    GHApp.AddSentryBreadcrumb("SaveGameBeforeExit", GHConstants.SentryGnollHackGeneralCategoryName);
                    GHApp.CancelSaveGame = false;
                    GHApp.AppSwitchSaveStyle = 0;
                    GHApp.BackgroundSaveInProgress = true;
                    game.SaveDoneConfirmed = false;
                    game.SaveGameAndWaitForResume();

                    Stopwatch stopwatch = Stopwatch.StartNew();
                    while (!game.SaveDoneConfirmed)
                    {
                        long limit = GHApp.SavingGame ? SaveFinishTimeoutMs : SaveStartTimeoutMs;
                        if (stopwatch.ElapsedMilliseconds >= limit)
                        {
                            GHApp.AddSentryBreadcrumb("SaveGameBeforeExit: timed out", GHConstants.SentryGnollHackGeneralCategoryName);
                            break;
                        }
                        Thread.Sleep(GHConstants.PollingInterval);
                    }
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine("GHAvaloniaShutdown: " + ex.Message);
                }
            }
        }
    }
}
#endif

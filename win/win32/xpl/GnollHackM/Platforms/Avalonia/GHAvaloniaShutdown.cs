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
        private static PosixSignalRegistration[] _signalRegistrations;

        public static void RegisterSignals()
        {
            _signalRegistrations = new[]
            {
                PosixSignalRegistration.Create(PosixSignal.SIGTERM, OnSignal),
                PosixSignalRegistration.Create(PosixSignal.SIGINT, OnSignal),
                PosixSignalRegistration.Create(PosixSignal.SIGHUP, OnSignal),
            };
        }

        /* On a thread pool thread. A further signal waits for the first one's save inside SaveGameBeforeExit. */
        private static void OnSignal(PosixSignalContext context)
        {
            context.Cancel = true;
            SaveGameBeforeExit();
            Environment.Exit(143);
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

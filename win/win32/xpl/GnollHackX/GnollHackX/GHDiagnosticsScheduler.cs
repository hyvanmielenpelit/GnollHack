using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace GnollHackX
{
    /* How much a diagnostics job may cost where it runs: light jobs run at any spot, heavy jobs only at heavy
       spots, or at a light spot once they are overdue by their staleness ceiling */
    public enum GHDiagnosticsCost
    {
        Light = 0,
        Heavy = 1,
    }

    public static class GHDiagnosticsSpotNames
    {
        /* A job's own timer outside a game, where nothing visible needs protecting; counts as heavy */
        public const string Timer = "timer";

        /* Heavy spots */
        public const string LevelChange = "level-change";
        public const string GameMenu = "game-menu";
        public const string GameStart = "game-start";
        public const string GameEnd = "game-end";
        public const string Background = "background";

        /* Light spots */
        public const string YnAnswer = "yn-answer";
        public const string GetLineAnswer = "getline-answer";
        public const string MenuClose = "menu-close";
        public const string TextClose = "text-close";
    }

    /* Where a diagnostics job runs */
    public struct GHDiagnosticsSpot
    {
        public readonly string Name;
        public readonly bool IsHeavy;

        public GHDiagnosticsSpot(string name, bool isHeavy)
        {
            Name = name;
            IsHeavy = isHeavy;
        }
    }

    /* One piece of diagnostic work. Its own timer calls Request; the job then runs at once outside a game, and
       during one at the next spot that its cost allows. The run delegate receives the spot and whether the job
       was due; a job made to run at every heavy spot is also called there when it was not due. */
    public sealed class GHDiagnosticsJob
    {
        private readonly long _stalenessCeilingSeconds;
        private readonly Action<GHDiagnosticsSpot, bool> _run;
        private int _due = 0;
        private long _dueSinceUtcTicks = 0L;
        private int _running = 0;
        private long _lastRunUtcTicks = 0L;

        public GHDiagnosticsJob(string name, GHDiagnosticsCost cost, long stalenessCeilingSeconds, bool runAtEveryHeavySpot,
            Action<GHDiagnosticsSpot, bool> run)
        {
            Name = name;
            Cost = cost;
            _stalenessCeilingSeconds = stalenessCeilingSeconds;
            RunAtEveryHeavySpot = runAtEveryHeavySpot;
            _run = run;
        }

        public string Name { get; }
        public GHDiagnosticsCost Cost { get; }
        public bool RunAtEveryHeavySpot { get; }
        public long LastRunUtcTicks { get { return Interlocked.Read(ref _lastRunUtcTicks); } }

        /* Any thread: runs the job on the calling thread outside a game, and marks it due during one */
        public void Request()
        {
            if (!GHDiagnosticsScheduler.IsInGame)
            {
                Run(new GHDiagnosticsSpot(GHDiagnosticsSpotNames.Timer, true), true);
                return;
            }
            MarkDue();
        }

        internal bool IsDue { get { return Interlocked.CompareExchange(ref _due, 0, 0) != 0; } }

        internal bool IsOverdue(long nowUtcTicks)
        {
            return _stalenessCeilingSeconds > 0 && IsDue
                && nowUtcTicks - Interlocked.Read(ref _dueSinceUtcTicks) >= _stalenessCeilingSeconds * TimeSpan.TicksPerSecond;
        }

        internal bool TakeDue()
        {
            return Interlocked.Exchange(ref _due, 0) == 1;
        }

        private void MarkDue()
        {
            if (Interlocked.Exchange(ref _due, 1) == 0)
                Interlocked.Exchange(ref _dueSinceUtcTicks, GHDiagnosticsScheduler.UtcNow());
        }

        /* One run at a time; a due run that finds the job running leaves it due */
        internal void Run(GHDiagnosticsSpot spot, bool wasDue)
        {
            if (Interlocked.Exchange(ref _running, 1) != 0)
            {
                if (wasDue)
                    MarkDue();
                return;
            }
            try
            {
                _run?.Invoke(spot, wasDue);
            }
            catch (Exception ex)
            {
                GHDiagnosticsScheduler.LogError("Diagnostics job " + Name + " at " + spot.Name + ": " + ex.Message);
            }
            finally
            {
                Interlocked.Exchange(ref _lastRunUtcTicks, GHDiagnosticsScheduler.UtcNow());
                Interlocked.Exchange(ref _running, 0);
            }
        }
    }

    /* Runs diagnostic work during a game only where a pause cannot be seen. Heavy spots (a level change, a
       game-menu close, the loading-screen hide, the end of a game, going to the background) run every due job
       and every job registered for all heavy spots. Light spots (a prompt answered, a menu or text window
       closed) run due light jobs and overdue heavy ones, and do nothing outside a game or during a replay. */
    public static class GHDiagnosticsScheduler
    {
        private static readonly object _registrationLock = new object();
        private static GHDiagnosticsJob[] _jobs = new GHDiagnosticsJob[0];

        private static Func<bool> _isInGame;
        private static Func<bool> _isPlayingReplay;
        private static Action<GHDiagnosticsSpot> _beforeHeavySpotJobs;
        private static Action<GHDiagnosticsSpot> _afterHeavySpotJobs;
        private static Action<string> _logError;

        /* Replaced by the unit tests */
        internal static Func<long> UtcNow = () => DateTime.UtcNow.Ticks;
        internal static Action<Action> RunInBackground = action => Task.Run(action);

        public static void Configure(Func<bool> isInGame, Func<bool> isPlayingReplay, Action<GHDiagnosticsSpot> beforeHeavySpotJobs,
            Action<GHDiagnosticsSpot> afterHeavySpotJobs, Action<string> logError)
        {
            Volatile.Write(ref _isInGame, isInGame);
            Volatile.Write(ref _isPlayingReplay, isPlayingReplay);
            Volatile.Write(ref _beforeHeavySpotJobs, beforeHeavySpotJobs);
            Volatile.Write(ref _afterHeavySpotJobs, afterHeavySpotJobs);
            Volatile.Write(ref _logError, logError);
        }

        /* False until configured */
        public static bool IsInGame
        {
            get { return Evaluate(Volatile.Read(ref _isInGame)); }
        }

        private static bool IsPlayingReplay
        {
            get { return Evaluate(Volatile.Read(ref _isPlayingReplay)); }
        }

        private static bool Evaluate(Func<bool> predicate)
        {
            if (predicate == null)
                return false;
            try
            {
                return predicate();
            }
            catch (Exception ex)
            {
                LogError("Diagnostics scheduler: " + ex.Message);
                return false;
            }
        }

        internal static void LogError(string message)
        {
            try
            {
                Action<string> logError = Volatile.Read(ref _logError);
                if (logError != null)
                    logError(message);
                else
                    System.Diagnostics.Debug.WriteLine(message);
            }
            catch
            {
                /* Logging must not fail a job */
            }
        }

        public static void Register(GHDiagnosticsJob job)
        {
            if (job == null)
                return;
            lock (_registrationLock)
            {
                GHDiagnosticsJob[] jobs = _jobs;
                if (Array.IndexOf(jobs, job) >= 0)
                    return;
                GHDiagnosticsJob[] newJobs = new GHDiagnosticsJob[jobs.Length + 1];
                Array.Copy(jobs, newJobs, jobs.Length);
                newJobs[jobs.Length] = job;
                Volatile.Write(ref _jobs, newJobs);
            }
        }

        public static void Unregister(GHDiagnosticsJob job)
        {
            if (job == null)
                return;
            lock (_registrationLock)
            {
                GHDiagnosticsJob[] jobs = _jobs;
                int index = Array.IndexOf(jobs, job);
                if (index < 0)
                    return;
                GHDiagnosticsJob[] newJobs = new GHDiagnosticsJob[jobs.Length - 1];
                Array.Copy(jobs, 0, newJobs, 0, index);
                Array.Copy(jobs, index + 1, newJobs, index, jobs.Length - index - 1);
                Volatile.Write(ref _jobs, newJobs);
            }
        }

        /* Any thread; the work runs on the thread pool, whether or not a game is on */
        public static void RaiseHeavySpot(string name)
        {
            GHDiagnosticsSpot spot = new GHDiagnosticsSpot(name, true);
            try
            {
                RunInBackground(() => RunHeavySpot(spot));
            }
            catch (Exception ex)
            {
                LogError("Diagnostics spot " + name + ": " + ex.Message);
            }
        }

        private static void RunHeavySpot(GHDiagnosticsSpot spot)
        {
            InvokeSpotCallback(Volatile.Read(ref _beforeHeavySpotJobs), spot);
            GHDiagnosticsJob[] jobs = Volatile.Read(ref _jobs);
            for (int i = 0; i < jobs.Length; i++)
            {
                GHDiagnosticsJob job = jobs[i];
                bool due = job.TakeDue();
                if (due || job.RunAtEveryHeavySpot)
                    job.Run(spot, due);
            }
            InvokeSpotCallback(Volatile.Read(ref _afterHeavySpotJobs), spot);
        }

        private static void InvokeSpotCallback(Action<GHDiagnosticsSpot> callback, GHDiagnosticsSpot spot)
        {
            if (callback == null)
                return;
            try
            {
                callback(spot);
            }
            catch (Exception ex)
            {
                LogError("Diagnostics spot " + spot.Name + ": " + ex.Message);
            }
        }

        /* Usually the UI thread: without a qualifying job it only scans the registered jobs */
        public static void RaiseLightSpot(string name)
        {
            try
            {
                if (!IsInGame || IsPlayingReplay)
                    return;
                long now = UtcNow();
                GHDiagnosticsJob[] jobs = Volatile.Read(ref _jobs);
                int i;
                bool any = false;
                for (i = 0; i < jobs.Length; i++)
                {
                    if (QualifiesForLightSpot(jobs[i], now))
                    {
                        any = true;
                        break;
                    }
                }
                if (!any)
                    return;

                List<GHDiagnosticsJob> selected = new List<GHDiagnosticsJob>(jobs.Length);
                for (; i < jobs.Length; i++)
                {
                    if (QualifiesForLightSpot(jobs[i], now))
                        selected.Add(jobs[i]);
                }
                GHDiagnosticsSpot spot = new GHDiagnosticsSpot(name, false);
                RunInBackground(() =>
                {
                    int j;
                    for (j = 0; j < selected.Count; j++)
                    {
                        if (selected[j].TakeDue())
                            selected[j].Run(spot, true);
                    }
                });
            }
            catch (Exception ex)
            {
                LogError("Diagnostics spot " + name + ": " + ex.Message);
            }
        }

        private static bool QualifiesForLightSpot(GHDiagnosticsJob job, long nowUtcTicks)
        {
            if (job.Cost == GHDiagnosticsCost.Light)
                return job.IsDue;
            return job.IsOverdue(nowUtcTicks);
        }
    }
}

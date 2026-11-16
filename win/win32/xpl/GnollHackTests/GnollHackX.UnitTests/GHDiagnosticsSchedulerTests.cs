using System;
using System.Collections.Generic;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHDiagnosticsScheduler: a requested job runs at once outside a game and waits for a spot during
       one, heavy spots run due jobs and jobs registered for all heavy spots between the before and after
       callbacks, light spots run due light jobs and heavy jobs past their staleness ceiling, and do nothing
       outside a game or during a replay, and a failing job neither stops the others nor loses its error. */
    [Collection("GHDiagnosticsScheduler")]
    public class GHDiagnosticsSchedulerTests : IDisposable
    {
        private readonly List<GHDiagnosticsJob> _registered = new List<GHDiagnosticsJob>();
        private readonly List<string> _events = new List<string>();
        private readonly List<string> _errors = new List<string>();
        private readonly Func<long> _originalUtcNow;
        private readonly Action<Action> _originalRunInBackground;
        private bool _inGame;
        private bool _playingReplay;
        private long _now = 1000L * TimeSpan.TicksPerSecond;

        public GHDiagnosticsSchedulerTests()
        {
            _originalUtcNow = GHDiagnosticsScheduler.UtcNow;
            _originalRunInBackground = GHDiagnosticsScheduler.RunInBackground;
            GHDiagnosticsScheduler.UtcNow = () => _now;
            GHDiagnosticsScheduler.RunInBackground = action => action();
            GHDiagnosticsScheduler.Configure(() => _inGame, () => _playingReplay,
                spot => _events.Add("before " + spot.Name), spot => _events.Add("after " + spot.Name), message => _errors.Add(message));
        }

        public void Dispose()
        {
            foreach (GHDiagnosticsJob job in _registered)
                GHDiagnosticsScheduler.Unregister(job);
            GHDiagnosticsScheduler.Configure(null, null, null, null, null);
            GHDiagnosticsScheduler.UtcNow = _originalUtcNow;
            GHDiagnosticsScheduler.RunInBackground = _originalRunInBackground;
        }

        private GHDiagnosticsJob AddJob(string name, GHDiagnosticsCost cost, long ceilingSeconds = 0, bool runAtEveryHeavySpot = false,
            Action<GHDiagnosticsSpot, bool> extra = null)
        {
            GHDiagnosticsJob job = new GHDiagnosticsJob(name, cost, ceilingSeconds, runAtEveryHeavySpot, (spot, wasDue) =>
            {
                _events.Add(name + " " + spot.Name + " " + (spot.IsHeavy ? "heavy" : "light") + " " + (wasDue ? "due" : "not-due"));
                if (extra != null)
                    extra(spot, wasDue);
            });
            GHDiagnosticsScheduler.Register(job);
            _registered.Add(job);
            return job;
        }

        [Fact]
        public void Request_OutsideGame_RunsAtOnceAsHeavyTimerSpot()
        {
            GHDiagnosticsJob job = AddJob("a", GHDiagnosticsCost.Heavy);
            job.Request();
            Assert.Equal(new[] { "a timer heavy due" }, _events);
        }

        [Fact]
        public void Request_DuringGame_WaitsForTheNextHeavySpot()
        {
            _inGame = true;
            GHDiagnosticsJob job = AddJob("a", GHDiagnosticsCost.Heavy);
            job.Request();
            Assert.Empty(_events);

            GHDiagnosticsScheduler.RaiseHeavySpot(GHDiagnosticsSpotNames.LevelChange);
            Assert.Equal(new[] { "before level-change", "a level-change heavy due", "after level-change" }, _events);

            _events.Clear();
            GHDiagnosticsScheduler.RaiseHeavySpot(GHDiagnosticsSpotNames.LevelChange);
            Assert.Equal(new[] { "before level-change", "after level-change" }, _events);
        }

        [Fact]
        public void HeavySpot_RunsJobsRegisteredForEveryHeavySpot_WhenNotDue()
        {
            _inGame = true;
            AddJob("every", GHDiagnosticsCost.Light, runAtEveryHeavySpot: true);
            GHDiagnosticsScheduler.RaiseHeavySpot(GHDiagnosticsSpotNames.GameStart);
            Assert.Equal(new[] { "before game-start", "every game-start heavy not-due", "after game-start" }, _events);
        }

        [Fact]
        public void LightSpot_RunsOnlyDueLightJobs()
        {
            _inGame = true;
            GHDiagnosticsJob due = AddJob("due", GHDiagnosticsCost.Light);
            AddJob("idle", GHDiagnosticsCost.Light, runAtEveryHeavySpot: true);
            due.Request();

            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.YnAnswer);
            Assert.Equal(new[] { "due yn-answer light due" }, _events);

            _events.Clear();
            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.YnAnswer);
            Assert.Empty(_events);
        }

        [Fact]
        public void LightSpot_RunsHeavyJobOnlyPastItsCeiling()
        {
            _inGame = true;
            GHDiagnosticsJob heavy = AddJob("heavy", GHDiagnosticsCost.Heavy, ceilingSeconds: 180);
            heavy.Request();

            _now += 179 * TimeSpan.TicksPerSecond;
            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.MenuClose);
            Assert.Empty(_events);

            _now += 1 * TimeSpan.TicksPerSecond;
            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.MenuClose);
            Assert.Equal(new[] { "heavy menu-close light due" }, _events);
        }

        [Fact]
        public void LightSpot_DoesNothingOutsideGameOrDuringReplay()
        {
            _inGame = true;
            GHDiagnosticsJob job = AddJob("a", GHDiagnosticsCost.Light);
            job.Request();

            _playingReplay = true;
            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.TextClose);
            Assert.Empty(_events);

            _playingReplay = false;
            _inGame = false;
            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.TextClose);
            Assert.Empty(_events);

            _inGame = true;
            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.TextClose);
            Assert.Equal(new[] { "a text-close light due" }, _events);
        }

        [Fact]
        public void HeavySpot_ThrowingJobDoesNotStopTheNextOne()
        {
            _inGame = true;
            AddJob("bad", GHDiagnosticsCost.Heavy, runAtEveryHeavySpot: true, extra: (spot, wasDue) => { throw new InvalidOperationException("boom"); });
            AddJob("good", GHDiagnosticsCost.Heavy, runAtEveryHeavySpot: true);

            GHDiagnosticsScheduler.RaiseHeavySpot(GHDiagnosticsSpotNames.GameMenu);
            Assert.Equal(new[] { "before game-menu", "bad game-menu heavy not-due", "good game-menu heavy not-due", "after game-menu" }, _events);
            Assert.Single(_errors);
            Assert.Contains("bad", _errors[0]);
            Assert.Contains("boom", _errors[0]);
        }

        [Fact]
        public void Run_WhileAlreadyRunning_KeepsTheJobDue()
        {
            _inGame = true;
            GHDiagnosticsJob job = null;
            int depth = 0;
            job = AddJob("a", GHDiagnosticsCost.Light, extra: (spot, wasDue) =>
            {
                depth++;
                if (depth == 1)
                {
                    job.Request();
                    GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.YnAnswer);
                }
            });
            job.Request();

            GHDiagnosticsScheduler.RaiseHeavySpot(GHDiagnosticsSpotNames.LevelChange);
            Assert.Equal(1, depth);
            Assert.True(job.IsDue);

            GHDiagnosticsScheduler.RaiseLightSpot(GHDiagnosticsSpotNames.YnAnswer);
            Assert.Equal(2, depth);
        }

        [Fact]
        public void Register_SameJobTwice_RunsItOnce()
        {
            _inGame = true;
            GHDiagnosticsJob job = AddJob("a", GHDiagnosticsCost.Light, runAtEveryHeavySpot: true);
            GHDiagnosticsScheduler.Register(job);
            GHDiagnosticsScheduler.RaiseHeavySpot(GHDiagnosticsSpotNames.Background);
            Assert.Equal(new[] { "before background", "a background heavy not-due", "after background" }, _events);
        }
    }
}

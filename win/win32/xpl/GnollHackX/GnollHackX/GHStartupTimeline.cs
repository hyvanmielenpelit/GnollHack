using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Text;

namespace GnollHackX
{
    /* Startup milestones. The codes are stored in Android exit snapshots and never change. New
       milestones are appended. A code names a milestone, not its place in time: MAUI raises the main page's Appearing inside
       MainActivity.OnCreate, so MainPage can be reached before Activity. */
    public enum GHStartupMilestone
    {
        None = 0,
        AppCreate = 1,          /* MainApplication.OnCreate entered (Android) */
        AppInit = 2,            /* GHApp.Initialize entered */
        Credentials = 3,        /* Secure-store credential load entered */
        CredentialsDone = 4,    /* Secure-store credential load finished */
        AppReady = 5,           /* GHApp.Initialize finished */
        Activity = 6,           /* MainActivity.OnCreate finished (Android) */
        Focused = 7,            /* The activity window first gained focus (Android) */
        MainPage = 8,           /* The main page's first-time startup entered */
        StartUpTasks = 9,       /* MainPage.StartUpTasks finished */
        MainMenu = 10,          /* The main menu has faded in */
        Runtime = 11,           /* The managed MainApplication constructor entered (Android) */
        NativeLoaded = 12,      /* The engine library loaded, before FMOD's initialization (Android) */
        MauiAppBuilt = 13,      /* MauiProgram.CreateMauiApp built the app */
        BaseAssets = 14,        /* GHApp.Initialize loaded the base typefaces and bitmaps */
        FirstDraw = 15,         /* The activity window's first draw pass began (Android) */
    }

    /* When each startup milestone was first reached, in milliseconds since process start. The default
       clock is Stopwatch, which does not advance in deep sleep, so a process that has slept reads
       younger than it is; startup itself runs awake. */
    public sealed class GHStartupTimeline
    {
        public const int MaxMilestone = (int)GHStartupMilestone.FirstDraw;

        /* The highest code of builds whose exit snapshots do not record the writer's highest code */
        public const int LegacyMaxMilestone = (int)GHStartupMilestone.MainMenu;

        public static readonly GHStartupTimeline Current = new GHStartupTimeline(null);

        /* Indexed by milestone code. The names reach Sentry, whose server-side scrubbing discards any
           value containing a word such as credentials, auth, token, password or secret. */
        private static readonly string[] _names =
        {
            "none", "app-create", "app-init", "secure-store", "secure-store-done", "app-ready",
            "activity", "focused", "main-page", "startup-tasks", "main-menu",
            "runtime", "native-loaded", "maui-app", "base-assets", "first-draw"
        };

        private readonly object _lock = new object();
        private readonly Func<long> _clockMilliseconds;
        private readonly long[] _milestoneMilliseconds = new long[MaxMilestone + 1];
        private bool _anchored = false;
        private long _anchorClockMilliseconds = 0L;
        private long _anchorProcessAgeMilliseconds = 0L;
        private int _lastMilestone = 0;
        private long _lastMilestoneMilliseconds = -1L;
        private int _reachedMask = 0;

        /* Raised on the marking thread, once per milestone, after the timeline is updated */
        public event Action<GHStartupMilestone> MilestoneReached;

        /* clockMilliseconds is a monotonic clock; null uses Stopwatch */
        public GHStartupTimeline(Func<long> clockMilliseconds)
        {
            _clockMilliseconds = clockMilliseconds ?? StopwatchMilliseconds;
            int i;
            for (i = 0; i < _milestoneMilliseconds.Length; i++)
                _milestoneMilliseconds[i] = -1L;
        }

        private static long StopwatchMilliseconds()
        {
            return (long)(Stopwatch.GetTimestamp() * 1000.0 / Stopwatch.Frequency);
        }

        /* Process age from the process start time, 0 when the platform cannot tell */
        private static long ReadProcessAgeMilliseconds()
        {
            try
            {
                using (Process process = Process.GetCurrentProcess())
                {
                    TimeSpan age = DateTime.Now - process.StartTime;
                    return age.Ticks > 0 ? (long)age.TotalMilliseconds : 0L;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHStartupTimeline process age: " + ex.Message);
                return 0L;
            }
        }

        /* Anchors the timeline: the process is processAgeMilliseconds old now. Only the first anchor counts. */
        public void SetProcessAgeNow(long processAgeMilliseconds)
        {
            lock (_lock)
            {
                if (!_anchored)
                    AnchorLocked(processAgeMilliseconds);
            }
        }

        private void AnchorLocked(long processAgeMilliseconds)
        {
            _anchorClockMilliseconds = _clockMilliseconds();
            _anchorProcessAgeMilliseconds = processAgeMilliseconds < 0 ? 0L : processAgeMilliseconds;
            _anchored = true;
        }

        private long ProcessAgeLocked()
        {
            return _anchorProcessAgeMilliseconds + (_clockMilliseconds() - _anchorClockMilliseconds);
        }

        /* Records the first time a milestone is reached; later marks of the same milestone do nothing.
           Without an anchor, the first mark anchors from the process start time. */
        public void Mark(GHStartupMilestone milestone)
        {
            int code = (int)milestone;
            if (code <= 0 || code > MaxMilestone)
                return;

            bool anchored;
            lock (_lock)
            {
                anchored = _anchored;
            }
            long fallbackAge = anchored ? 0L : ReadProcessAgeMilliseconds();

            bool isNew = false;
            lock (_lock)
            {
                if (!_anchored)
                    AnchorLocked(fallbackAge);
                if (_milestoneMilliseconds[code] < 0)
                {
                    long now = ProcessAgeLocked();
                    _milestoneMilliseconds[code] = now;
                    _lastMilestone = code;
                    _lastMilestoneMilliseconds = now;
                    _reachedMask |= 1 << code;
                    isNew = true;
                }
            }
            if (!isNew)
                return;

            Action<GHStartupMilestone> handlers = MilestoneReached;
            if (handlers == null)
                return;
            foreach (Delegate handler in handlers.GetInvocationList())
            {
                try
                {
                    ((Action<GHStartupMilestone>)handler)(milestone);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("GHStartupTimeline handler: " + ex.Message);
                }
            }
        }

        /* -1 before the timeline is anchored */
        public long ProcessAgeMilliseconds
        {
            get
            {
                lock (_lock)
                {
                    return _anchored ? ProcessAgeLocked() : -1L;
                }
            }
        }

        public GHStartupMilestone LastMilestone
        {
            get
            {
                lock (_lock)
                {
                    return (GHStartupMilestone)_lastMilestone;
                }
            }
        }

        /* -1 before the first mark */
        public long LastMilestoneMilliseconds
        {
            get
            {
                lock (_lock)
                {
                    return _lastMilestoneMilliseconds;
                }
            }
        }

        /* Bit (1 << code) is set for each milestone reached */
        public int ReachedMask
        {
            get
            {
                lock (_lock)
                {
                    return _reachedMask;
                }
            }
        }

        /* A consistent read of the timeline; processAgeMilliseconds is -1 before the timeline is anchored */
        public void GetState(out long processAgeMilliseconds, out GHStartupMilestone lastMilestone,
            out long lastMilestoneMilliseconds, out int reachedMask)
        {
            lock (_lock)
            {
                processAgeMilliseconds = _anchored ? ProcessAgeLocked() : -1L;
                lastMilestone = (GHStartupMilestone)_lastMilestone;
                lastMilestoneMilliseconds = _lastMilestoneMilliseconds;
                reachedMask = _reachedMask;
            }
        }

        public bool HasReached(GHStartupMilestone milestone)
        {
            return MilestoneMilliseconds(milestone) >= 0;
        }

        /* Milliseconds since process start, -1 if not reached */
        public long MilestoneMilliseconds(GHStartupMilestone milestone)
        {
            int code = (int)milestone;
            if (code <= 0 || code > MaxMilestone)
                return -1L;
            lock (_lock)
            {
                return _milestoneMilliseconds[code];
            }
        }

        public static string Name(GHStartupMilestone milestone)
        {
            int code = (int)milestone;
            return code >= 0 && code < _names.Length ? _names[code] : "unknown";
        }

        /* The reached milestones in order of time, ties in code order, as "app-create 0.21 s, app-init 1.02 s" */
        public string FormatTimings()
        {
            long[] times = new long[MaxMilestone + 1];
            lock (_lock)
            {
                Array.Copy(_milestoneMilliseconds, times, times.Length);
            }

            List<int> reached = new List<int>();
            int code;
            for (code = 1; code <= MaxMilestone; code++)
            {
                if (times[code] >= 0)
                    reached.Add(code);
            }
            reached.Sort((a, b) => times[a] != times[b] ? times[a].CompareTo(times[b]) : a.CompareTo(b));

            StringBuilder sb = new StringBuilder();
            foreach (int c in reached)
            {
                if (sb.Length > 0)
                    sb.Append(", ");
                sb.Append(_names[c]).Append(' ')
                    .Append((times[c] / 1000.0).ToString("0.00", CultureInfo.InvariantCulture)).Append(" s");
            }
            return sb.ToString();
        }
    }
}

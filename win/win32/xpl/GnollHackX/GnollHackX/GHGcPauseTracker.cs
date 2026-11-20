using System;

namespace GnollHackX
{
    /* Turns cumulative garbage collection counters, sampled at irregular passes, into per-interval
       figures and a window of the last WindowSeconds. Has no locks: the caller serializes calls (the
       Android sampler calls it only inside RunPass, which runs on one thread at a time). */
    public sealed class GHGcPauseTracker
    {
        public const long WindowSeconds = 600;               /* x= covers intervals that ended this recently */
        public const int MaxIntervals = 64;                  /* the oldest is dropped beyond this */
        public const long LongCollectionMilliseconds = 1000; /* a Long GC: breadcrumb at or above this */

        private struct Interval
        {
            public long EndUtcTicks;
            public long NetLongestMs;
            public long JavaLongestMs;
            public long JavaCount;
            public long BridgeCount;
        }

        private readonly Interval[] _intervals = new Interval[MaxIntervals];
        private int _intervalCount = 0;
        private int _nextInterval = 0;

        private bool _hasBaseline = false;
        private long _previousUtcTicks = 0L;
        private long _previousNetCount = -1;
        private long _previousNetPauseMs = -1;
        private long _previousJavaCount = -1;
        private long _previousJavaTimeMs = -1;
        private long _previousBridgeGeneration = -1;

        public GHGcPauseTracker()
        {
            LastIntervalSeconds = -1;
            LastIntervalNetLongestMs = -1;
            LastIntervalJavaLongestMs = -1;
            LastIntervalJavaCount = -1;
            LastIntervalBridgeCount = -1;
        }

        public long LastIntervalSeconds { get; private set; }        /* -1 before the second sample */
        public long LastIntervalNetLongestMs { get; private set; }
        public long LastIntervalJavaLongestMs { get; private set; }
        public long LastIntervalJavaCount { get; private set; }
        public long LastIntervalBridgeCount { get; private set; }

        /* Either longest at or above LongCollectionMilliseconds */
        public bool LastIntervalIsLong
        {
            get { return LastIntervalNetLongestMs >= LongCollectionMilliseconds || LastIntervalJavaLongestMs >= LongCollectionMilliseconds; }
        }

        /* Cumulative counters and the bridge generation; -1 means unknown. netNewestPauseMs is the
           longest pause among the newest collection of each kind, when that collection completed after
           the caller's last sample */
        public void AddSample(long utcTicks, long netCount, long netPauseMs, long netNewestPauseMs,
            long javaCount, long javaTimeMs, long bridgeGeneration)
        {
            if (_hasBaseline)
            {
                LastIntervalSeconds = Math.Max(0L, (utcTicks - _previousUtcTicks) / TimeSpan.TicksPerSecond);

                long netLongest = netNewestPauseMs >= 0 ? netNewestPauseMs : -1;
                long netCountDelta, netPauseDelta;
                if (TryDelta(_previousNetCount, netCount, out netCountDelta)
                    && TryDelta(_previousNetPauseMs, netPauseMs, out netPauseDelta)
                    && netCountDelta > 0)
                {
                    netLongest = Math.Max(netLongest, netPauseDelta / netCountDelta);
                }

                long javaLongest = -1;
                long javaCountInInterval = -1;
                long javaCountDelta, javaTimeDelta;
                if (TryDelta(_previousJavaCount, javaCount, out javaCountDelta)
                    && TryDelta(_previousJavaTimeMs, javaTimeMs, out javaTimeDelta))
                {
                    javaCountInInterval = javaCountDelta;
                    if (javaCountDelta > 0)
                        javaLongest = javaTimeDelta / javaCountDelta;
                }

                long bridgeDelta;
                long bridgeCountInInterval = TryDelta(_previousBridgeGeneration, bridgeGeneration, out bridgeDelta) ? bridgeDelta : -1;

                LastIntervalNetLongestMs = netLongest;
                LastIntervalJavaLongestMs = javaLongest;
                LastIntervalJavaCount = javaCountInInterval;
                LastIntervalBridgeCount = bridgeCountInInterval;

                if (netLongest >= 0 || javaCountInInterval > 0 || bridgeCountInInterval > 0)
                {
                    Interval interval = new Interval();
                    interval.EndUtcTicks = utcTicks;
                    interval.NetLongestMs = netLongest;
                    interval.JavaLongestMs = javaLongest;
                    interval.JavaCount = javaCountInInterval;
                    interval.BridgeCount = bridgeCountInInterval;
                    _intervals[_nextInterval] = interval;
                    _nextInterval = (_nextInterval + 1) % MaxIntervals;
                    if (_intervalCount < MaxIntervals)
                        _intervalCount++;
                }
            }

            /* A counter that went down simply re-baselines */
            _previousUtcTicks = utcTicks;
            _previousNetCount = netCount;
            _previousNetPauseMs = netPauseMs;
            _previousJavaCount = javaCount;
            _previousJavaTimeMs = javaTimeMs;
            _previousBridgeGeneration = bridgeGeneration;
            _hasBaseline = true;
        }

        /* Longest values are -1 when none is known. A count is 0 for a quiet window once its series is
           known, and -1 before that */
        public void GetWindow(long nowUtcTicks, out long netLongestMs, out long javaLongestMs,
            out long javaCount, out long bridgeCount)
        {
            netLongestMs = -1;
            javaLongestMs = -1;
            javaCount = -1;
            bridgeCount = -1;
            long oldestEnd = nowUtcTicks - WindowSeconds * TimeSpan.TicksPerSecond;
            int i;
            for (i = 0; i < _intervalCount; i++)
            {
                Interval interval = _intervals[i];
                if (interval.EndUtcTicks < oldestEnd)
                    continue;
                if (interval.NetLongestMs > netLongestMs)
                    netLongestMs = interval.NetLongestMs;
                if (interval.JavaLongestMs > javaLongestMs)
                    javaLongestMs = interval.JavaLongestMs;
                if (interval.JavaCount >= 0)
                    javaCount = Math.Max(0L, javaCount) + interval.JavaCount;
                if (interval.BridgeCount >= 0)
                    bridgeCount = Math.Max(0L, bridgeCount) + interval.BridgeCount;
            }
            if (javaCount < 0 && _hasBaseline && _previousJavaCount >= 0)
                javaCount = 0;
            if (bridgeCount < 0 && _hasBaseline && _previousBridgeGeneration >= 0)
                bridgeCount = 0;
        }

        /* Known when both values are known and the counter did not go down */
        private static bool TryDelta(long previous, long current, out long delta)
        {
            delta = 0;
            if (previous < 0 || current < 0 || current < previous)
                return false;
            delta = current - previous;
            return true;
        }
    }
}

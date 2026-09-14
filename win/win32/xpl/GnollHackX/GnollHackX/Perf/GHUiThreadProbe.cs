using System;
using System.Diagnostics;
using System.Threading;
#if GNH_MAUI
using Microsoft.Maui.Dispatching;
#else
using Xamarin.Forms;
#endif

namespace GnollHackX.Perf
{
    /* UI-thread latency probe. A repeating dispatcher timer fires every IntervalMs on
       the UI thread, and each tick measures how late it fired relative to its schedule.
       On an idle UI thread the lateness is near zero; while something expensive holds
       the thread (layout, a logging burst, a sort, a GC pause) the tick fires late by
       that amount. This separates "the paint was slow" from "the UI thread was busy
       with something else", which frame timing alone cannot tell apart.

       Lateness is measured against an anchor re-set on every tick to the actual fire
       time plus the interval, so one late tick is counted once and never bleeds into
       the next. Dispatcher timers coalesce and may fire slightly early, hence the
       clamp at zero. Samples are added to the histogram only while a measurement
       window is open (BeginWindow / EndWindow), matching the other probes; ticks
       outside a window still re-anchor.

       Cost: one timer callback per 10 ms doing a handful of arithmetic operations and
       one histogram bin increment; the tick handler allocates nothing after the first
       tick. The observer-effect calibration run verifies that this is negligible.

       Threading: the tick handler is the only writer and runs on the UI thread. Scalar
       counters go through Interlocked and are exact from any thread. The histogram
       getters may be read from any thread; a read that overlaps a tick can see a
       partially updated histogram (a torn mean or a count one ahead of the bins),
       which is acceptable for diagnostic display and never faults. */
    public static class GHUiThreadProbe
    {
        public const int IntervalMs = 10;

        /* Lateness above this counts as a late tick; settable per display refresh rate. */
        public static float VsyncMs = 16.667f;

        private static readonly GHPerfStats.StreamingHistogram _latency = new GHPerfStats.StreamingHistogram();
        private static readonly double _msPerTick = 1000.0 / Stopwatch.Frequency;
        private static readonly long _intervalTicks = (long)(IntervalMs * Stopwatch.Frequency / 1000.0);

        private static long _expectedNextTicks;
        private static int _anchored;       /* 0 until the first tick after Start */
        private static int _running;
        private static int _windowOpen;
        private static long _tickCount;
        private static long _lateTickCount;
        private static long _lastLatencyBits;   /* float bits, written via Interlocked */

#if GNH_MAUI
        private static IDispatcherTimer _timer;
        private static readonly EventHandler _tickHandler = OnTick;
#else
        private static readonly Func<bool> _tickCallback = OnTimerCallback;
#endif

        public static bool IsRunning { get { return Interlocked.CompareExchange(ref _running, 0, 0) != 0; } }
        public static bool IsWindowOpen { get { return Interlocked.CompareExchange(ref _windowOpen, 0, 0) != 0; } }

        public static long TickCount { get { return Interlocked.Read(ref _tickCount); } }
        public static long LateTickCount { get { return Interlocked.Read(ref _lateTickCount); } }

        public static float LastLatencyMs
        {
            get { return BitsToFloat(Interlocked.Read(ref _lastLatencyBits)); }
        }

        /* Histogram readers; single-writer, reads tolerate tearing as described above. */
        public static float LatencyMeanMs { get { return (float)_latency.Mean; } }
        public static float LatencyP50Ms { get { return _latency.Percentile(50); } }
        public static float LatencyP95Ms { get { return _latency.Percentile(95); } }
        public static float LatencyP99Ms { get { return _latency.Percentile(99); } }
        public static float LatencyMaxMs { get { return _latency.Max; } }

#if GNH_MAUI
        /* Starts the probe on the given dispatcher; the timer ticks on that dispatcher's
           thread. A second call while running is ignored. */
        public static void Start(IDispatcher dispatcher)
        {
            if (dispatcher == null)
                return;
            if (Interlocked.CompareExchange(ref _running, 1, 0) != 0)
                return;
            Interlocked.Exchange(ref _anchored, 0);
            IDispatcherTimer timer = dispatcher.CreateTimer();
            timer.Interval = TimeSpan.FromMilliseconds(IntervalMs);
            timer.IsRepeating = true;
            timer.Tick += _tickHandler;
            _timer = timer;
            timer.Start();
        }

        public static void Stop()
        {
            if (Interlocked.CompareExchange(ref _running, 0, 1) != 1)
                return;
            IDispatcherTimer timer = _timer;
            _timer = null;
            if (timer != null)
            {
                timer.Stop();
                timer.Tick -= _tickHandler;
            }
        }

        private static void OnTick(object sender, EventArgs e)
        {
            Tick();
        }
#else
        /* Starts the probe on the Xamarin.Forms device timer, which ticks on the UI
           thread and keeps repeating while the callback returns true. A second call
           while running is ignored. */
        public static void Start()
        {
            if (Interlocked.CompareExchange(ref _running, 1, 0) != 0)
                return;
            Interlocked.Exchange(ref _anchored, 0);
            Device.StartTimer(TimeSpan.FromMilliseconds(IntervalMs), _tickCallback);
        }

        public static void Stop()
        {
            Interlocked.Exchange(ref _running, 0);
        }

        private static bool OnTimerCallback()
        {
            if (!IsRunning)
                return false;
            Tick();
            return IsRunning;
        }
#endif

        /* Clears the histogram and counters; the timer keeps running and re-anchors on
           its next tick. */
        public static void Reset()
        {
            Interlocked.Exchange(ref _anchored, 0);
            Interlocked.Exchange(ref _tickCount, 0);
            Interlocked.Exchange(ref _lateTickCount, 0);
            Interlocked.Exchange(ref _lastLatencyBits, 0);
            _latency.Reset();
        }

        public static void BeginWindow()
        {
            Interlocked.Exchange(ref _windowOpen, 1);
        }

        public static void EndWindow()
        {
            Interlocked.Exchange(ref _windowOpen, 0);
        }

        /* Runs on the UI thread once per timer tick; allocation-free. */
        private static void Tick()
        {
            long now = Stopwatch.GetTimestamp();
            if (Interlocked.CompareExchange(ref _anchored, 1, 0) == 0)
            {
                _expectedNextTicks = now + _intervalTicks;
                return;
            }
            long lateTicks = now - _expectedNextTicks;
            float lateMs = lateTicks > 0 ? (float)(lateTicks * _msPerTick) : 0f;
            _expectedNextTicks = now + _intervalTicks;

            Interlocked.Increment(ref _tickCount);
            Interlocked.Exchange(ref _lastLatencyBits, FloatToBits(lateMs));
            if (lateMs > VsyncMs)
                Interlocked.Increment(ref _lateTickCount);
            if (IsWindowOpen)
                _latency.Add(lateMs);
        }

        private static long FloatToBits(float value)
        {
            return BitConverter.DoubleToInt64Bits(value);
        }

        private static float BitsToFloat(long bits)
        {
            return (float)BitConverter.Int64BitsToDouble(bits);
        }
    }
}

using System.Diagnostics;
using System.Threading;

namespace GnollHackX.Perf
{
    /* Measures, on the game thread, how long the C core spends processing between two
       input requests: from the moment an input callback returns a key or position to
       the core, until the core next asks for input. Long processing holds the locks the
       render thread reads under Monitor.TryEnter, so a hitch can be attributed to the
       game thread from these numbers.

       Producer: InputWaitStart and InputWaitEnd are called only from the game thread,
       at the entry and every exit of ClientCallback_nhgetch and ClientCallback_nh_poskey.
       Those callbacks are re-entered from other callbacks (direction prompts, yn
       functions), so a depth counter makes only the outermost entry/exit pair count.

       Readers: the scalar properties may be read from any thread and never allocate;
       they are published with Interlocked. The histogram is single-writer and its
       getters are read without a lock, so a reader may observe a torn value between
       two Adds; that is accepted for a display and only affects the last sample.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHGameTurnTimer
    {
        private static readonly GHPerfStats.StreamingHistogram _processing = new GHPerfStats.StreamingHistogram();
        private static readonly double _ticksToMs = 1000.0 / Stopwatch.Frequency;

        /* Game-thread-only state */
        private static int _depth;
        private static long _processingStartTicks;
        private static bool _hasProcessingStart;

        /* Cross-thread scalars; durations are held as Stopwatch ticks and converted
           to milliseconds on read */
        private static long _lastProcessingTicks;
        private static long _totalProcessingTicks;
        private static long _waitingCount;
        private static int _windowOpen;
        private static int _publishedDepth;
        private static int _publishedHasStart;

        /* Called at the entry of an input callback: the core has finished processing
           and the interval that began at the previous InputWaitEnd ends now. */
        public static void InputWaitStart()
        {
            _depth++;
            Interlocked.Exchange(ref _publishedDepth, _depth);
            if (_depth != 1)
                return;
            if (!_hasProcessingStart)
                return;
            long now = Stopwatch.GetTimestamp();
            long elapsedTicks = now - _processingStartTicks;
            if (elapsedTicks < 0)
                elapsedTicks = 0;
            Interlocked.Exchange(ref _lastProcessingTicks, elapsedTicks);
            if (Interlocked.CompareExchange(ref _windowOpen, 0, 0) != 0)
            {
                _processing.Add((float)(elapsedTicks * _ticksToMs));
                Interlocked.Add(ref _totalProcessingTicks, elapsedTicks);
            }
            _hasProcessingStart = false;
            Interlocked.Exchange(ref _publishedHasStart, 0);
        }

        /* Called immediately before an input callback returns: input has been
           delivered and the core starts processing. */
        public static void InputWaitEnd()
        {
            if (_depth > 0)
                _depth--;
            Interlocked.Exchange(ref _publishedDepth, _depth);
            if (_depth != 0)
                return;
            _processingStartTicks = Stopwatch.GetTimestamp();
            _hasProcessingStart = true;
            Interlocked.Exchange(ref _publishedHasStart, 1);
            Interlocked.Increment(ref _waitingCount);
        }

        /* Clears everything, including the depth counter; call before a game thread
           starts. The window stays closed. */
        public static void Reset()
        {
            _depth = 0;
            _hasProcessingStart = false;
            _processingStartTicks = 0;
            Interlocked.Exchange(ref _publishedDepth, 0);
            Interlocked.Exchange(ref _publishedHasStart, 0);
            Interlocked.Exchange(ref _lastProcessingTicks, 0);
            Interlocked.Exchange(ref _totalProcessingTicks, 0);
            Interlocked.Exchange(ref _waitingCount, 0);
            Interlocked.Exchange(ref _windowOpen, 0);
            _processing.Reset();
        }

        /* Opens a measurement window: the histogram and counters restart and Add is
           enabled. While the window is closed only LastProcessingMs updates. */
        public static void BeginWindow()
        {
            _processing.Reset();
            Interlocked.Exchange(ref _totalProcessingTicks, 0);
            Interlocked.Exchange(ref _waitingCount, 0);
            Interlocked.Exchange(ref _windowOpen, 1);
        }

        public static void EndWindow()
        {
            Interlocked.Exchange(ref _windowOpen, 0);
        }

        public static bool IsWindowOpen
        {
            get { return Interlocked.CompareExchange(ref _windowOpen, 0, 0) != 0; }
        }

        public static int CurrentDepth
        {
            get { return Interlocked.CompareExchange(ref _publishedDepth, 0, 0); }
        }

        /* True while the core is processing: no input wait is open and a start
           timestamp exists. */
        public static bool IsProcessing
        {
            get
            {
                return Interlocked.CompareExchange(ref _publishedDepth, 0, 0) == 0
                    && Interlocked.CompareExchange(ref _publishedHasStart, 0, 0) != 0;
            }
        }

        public static float LastProcessingMs
        {
            get { return (float)(Interlocked.Read(ref _lastProcessingTicks) * _ticksToMs); }
        }

        /* Outer input waits completed since the last Reset or BeginWindow. */
        public static long WaitingCount
        {
            get { return Interlocked.Read(ref _waitingCount); }
        }

        public static long ProcessingCount
        {
            get { return _processing.Count; }
        }

        public static float ProcessingMeanMs
        {
            get { return (float)_processing.Mean; }
        }

        public static float ProcessingP50Ms
        {
            get { return _processing.Percentile(50); }
        }

        public static float ProcessingP95Ms
        {
            get { return _processing.Percentile(95); }
        }

        public static float ProcessingP99Ms
        {
            get { return _processing.Percentile(99); }
        }

        public static float ProcessingMaxMs
        {
            get { return _processing.Max; }
        }

        public static float TotalProcessingMs
        {
            get { return (float)(Interlocked.Read(ref _totalProcessingTicks) * _ticksToMs); }
        }
    }
}

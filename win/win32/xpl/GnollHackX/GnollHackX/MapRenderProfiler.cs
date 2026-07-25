using System;
using System.Diagnostics;

namespace GnollHackX
{
    /// <summary>
    /// Section identifiers for PaintMainGamePage profiling.
    /// Order matches the call sequence in the method.
    /// </summary>
    public enum MapProfilerSection
    {
        Init = 0,
        Locks,
        FontSetup,
        DrawingLoop,
        DL_LockAcquire,
        DL_Setup,
        DL_TileRender,
        DL_AscensionFX,
        PostDraw,
        WindowStrings,
        Count /* Must be last — gives array size */
    }

    /// <summary>
    /// Per-tile sub-operation accumulators.
    /// Called inside PaintMapTile and the iteration loop thousands of times per frame.
    /// Each accumulator has its own start timestamp so they can nest/overlap.
    /// </summary>
    public enum TileProfilerAccum
    {
        RectCalc = 0,
        FlagsAnim,
        CanvasTransform,
        DrawBitmap,
        AutoDraw,
        IterLogic,
        PmtTotal,
        Count /* Must be last */
    }

    /// <summary>
    /// Lightweight per-frame section profiler for PaintMainGamePage.
    /// Uses an enum + flat array for zero-allocation, zero-lookup overhead.
    /// Accumulates timing data in memory and logs averages to Debug output
    /// every LogIntervalSeconds.
    /// </summary>
    public class MapRenderProfiler
    {
        /// <summary>How often to log averages (in seconds).</summary>
        public double LogIntervalSeconds { get; set; } = 3.0;

        private const int SectionCount = (int)MapProfilerSection.Count;
        private const int AccumCount = (int)TileProfilerAccum.Count;
        private static readonly string[] SectionNames;
        private static readonly string[] AccumNames;

        private readonly Stopwatch _frameStopwatch = new Stopwatch();
        private readonly Stopwatch _logIntervalStopwatch = new Stopwatch();

        /* Accumulated ticks per section across multiple frames */
        private readonly long[] _accumulatedTicks = new long[SectionCount];
        private long _accumulatedFrameTicks;
        private int _frameCount;

        /* Per-tile accumulators — accumulated across all tiles in all frames */
        private readonly long[] _accumTicks = new long[AccumCount];
        /* Per-accumulator start timestamps — allows independent nesting */
        private readonly long[] _accumStartTicks = new long[AccumCount];
        private int _tileCallCount;
        private int _iterCount;

        /* Current frame tracking */
        private int _currentSection = -1;
        private long _currentSectionStartTicks;
        private bool _inFrame;

        static MapRenderProfiler()
        {
            SectionNames = new string[SectionCount];
            for (int i = 0; i < SectionCount; i++)
                SectionNames[i] = ((MapProfilerSection)i).ToString();

            AccumNames = new string[AccumCount];
            for (int i = 0; i < AccumCount; i++)
                AccumNames[i] = ((TileProfilerAccum)i).ToString();
        }

        public MapRenderProfiler()
        {
            _logIntervalStopwatch.Start();
        }

        /// <summary>
        /// Call at the very start of PaintMainGamePage.
        /// </summary>
        public void BeginFrame()
        {
            _inFrame = true;
            _currentSection = -1;
            _frameStopwatch.Restart();
        }

        /// <summary>
        /// Mark the start of a new section. Automatically closes the previous one.
        /// </summary>
        public void Mark(MapProfilerSection section)
        {
            if (!_inFrame)
                return;

            long nowTicks = _frameStopwatch.ElapsedTicks;

            /* Close previous section */
            if (_currentSection >= 0)
            {
                _accumulatedTicks[_currentSection] += nowTicks - _currentSectionStartTicks;
            }

            /* Begin new section */
            _currentSection = (int)section;
            _currentSectionStartTicks = nowTicks;
        }

        /// <summary>
        /// Start timing a sub-operation. Each accumulator has its own start timestamp
        /// so different accumulators can be independently started/stopped without
        /// clobbering each other.
        /// </summary>
        public void StartAccum(TileProfilerAccum accum)
        {
            _accumStartTicks[(int)accum] = _frameStopwatch.ElapsedTicks;
        }

        /// <summary>
        /// Stop timing and add elapsed ticks to the given accumulator.
        /// </summary>
        public void StopAccum(TileProfilerAccum accum)
        {
            _accumTicks[(int)accum] += _frameStopwatch.ElapsedTicks - _accumStartTicks[(int)accum];
        }

        /// <summary>
        /// Increment the per-tile call counter (called inside PaintMapTile).
        /// </summary>
        public void CountTile()
        {
            _tileCallCount++;
        }

        /// <summary>
        /// Increment the iteration counter (called per inner loop iteration).
        /// </summary>
        public void CountIter()
        {
            _iterCount++;
        }

        /// <summary>
        /// Call at the very end of the profiled region.
        /// </summary>
        public void EndFrame()
        {
            if (!_inFrame)
                return;

            long nowTicks = _frameStopwatch.ElapsedTicks;

            /* Close last section */
            if (_currentSection >= 0)
            {
                _accumulatedTicks[_currentSection] += nowTicks - _currentSectionStartTicks;
            }

            _accumulatedFrameTicks += nowTicks;
            _frameCount++;
            _inFrame = false;

            /* Log if interval elapsed */
            if (_logIntervalStopwatch.Elapsed.TotalSeconds >= LogIntervalSeconds)
            {
                LogAndReset();
            }
        }

        private void LogAndReset()
        {
            if (_frameCount == 0)
            {
                _logIntervalStopwatch.Restart();
                return;
            }

            double ticksPerMs = Stopwatch.Frequency / 1000.0;
            double avgFrameMs = (_accumulatedFrameTicks / (double)_frameCount) / ticksPerMs;
            double avgTilesPerFrame = _tileCallCount / (double)_frameCount;
            double avgItersPerFrame = _iterCount / (double)_frameCount;

            System.Text.StringBuilder sb = new System.Text.StringBuilder(512);
            sb.AppendLine($"[MapProfiler] {_frameCount} frames, avg total: {avgFrameMs:F2}ms ({(1000.0 / avgFrameMs):F1} fps)");
            sb.AppendLine($"  tiles/frame: {avgTilesPerFrame:F0}, iters/frame: {avgItersPerFrame:F0}");

            for (int i = 0; i < SectionCount; i++)
            {
                double avgMs = (_accumulatedTicks[i] / (double)_frameCount) / ticksPerMs;
                double pct = (avgFrameMs > 0) ? (avgMs / avgFrameMs * 100.0) : 0;
                sb.AppendLine($"  {SectionNames[i],-20} {avgMs,8:F2}ms  ({pct,5:F1}%)");
            }

            sb.AppendLine($"  -- PaintMapTile + Iteration breakdown --");
            for (int i = 0; i < AccumCount; i++)
            {
                double avgMs = (_accumTicks[i] / (double)_frameCount) / ticksPerMs;
                double pct = (avgFrameMs > 0) ? (avgMs / avgFrameMs * 100.0) : 0;
                sb.AppendLine($"    {AccumNames[i],-18} {avgMs,8:F2}ms  ({pct,5:F1}%)");
            }

            GHApp.MaybeWriteGHLog(sb.ToString());

            /* Reset accumulators */
            Array.Clear(_accumulatedTicks, 0, SectionCount);
            Array.Clear(_accumTicks, 0, AccumCount);
            _accumulatedFrameTicks = 0;
            _frameCount = 0;
            _tileCallCount = 0;
            _iterCount = 0;

            _logIntervalStopwatch.Restart();
        }
    }
}

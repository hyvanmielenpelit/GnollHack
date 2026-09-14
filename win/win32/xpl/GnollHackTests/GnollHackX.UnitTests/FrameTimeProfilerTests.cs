using System.Diagnostics;
using GnollHackX.Perf;
using Xunit;
using Xunit.Abstractions;

namespace GnollHackX.UnitTests
{
    /* FrameTimeProfiler is static and every test drives the same ring and window,
       so no two of these tests may run at once. xunit runs the tests of one class
       sequentially; the collection definition additionally keeps the class from
       running in parallel with anything else in the assembly. */
    [CollectionDefinition("FrameTimeProfiler", DisableParallelization = true)]
    public class FrameTimeProfilerCollection
    {
    }

    /* Timing tests against real Stopwatch time. Gaps are created with a spin-wait,
       which on a loaded machine overshoots but never undershoots: every lower bound
       below is the requested duration times 0.8 (the 0.8 covers only float rounding
       and the profiler's own stamping overhead), every upper bound is 2.5 times it,
       and nothing depends on the machine's absolute speed beyond that. */
    [Collection("FrameTimeProfiler")]
    public class FrameTimeProfilerTests
    {
        private const float VsyncMs = 5f;
        private readonly ITestOutputHelper _output;
        private long _frameNumber;

        public FrameTimeProfilerTests(ITestOutputHelper output)
        {
            _output = output;
        }

        /* Disabling ends the window and clears the ring; enabling opens a fresh
           rolling window, so each test starts from an empty profiler. */
        private void ResetProfiler()
        {
            FrameTimeProfiler.IsEnabled = false;
            FrameTimeProfiler.IsEnabled = true;
            FrameTimeProfiler.SetVsyncPeriodMs(VsyncMs);
            _frameNumber = 0;
        }

        /* Busy-waits for at least the given time. Thread.Sleep is quantized to the
           scheduler tick (up to 15.6 ms on Windows), far coarser than the gaps the
           tests need, so the wait polls the same clock the profiler stamps with. */
        private static void SpinUntil(double ms)
        {
            long start = Stopwatch.GetTimestamp();
            long waitTicks = (long)Math.Ceiling(ms * Stopwatch.Frequency / 1000.0);
            long target = start + waitTicks;
            while (Stopwatch.GetTimestamp() < target)
            {
            }
        }

        /* A frame that enters the render path with every stamp set, painting for
           paintMs (zero means no paint wait). */
        private void RunFullFrame(double paintMs)
        {
            FrameTimeProfiler.BeginFrame(_frameNumber++);
            FrameTimeProfiler.StampUpdate();
            FrameTimeProfiler.StampLockAttempt();
            FrameTimeProfiler.StampLockResult(true);
            FrameTimeProfiler.StampPaintStart();
            if (paintMs > 0)
                SpinUntil(paintMs);
            FrameTimeProfiler.StampPaintEnd();
            FrameTimeProfiler.StampFlushStart();
            FrameTimeProfiler.StampFlushEnd();
            FrameTimeProfiler.EndFrame();
        }

        /* count rendered frames with no paint wait, gapMs apart; the gap precedes
           every frame but the first so the caller controls the gap into the first. */
        private void RunFrames(int count, double gapMs)
        {
            int i;
            for (i = 0; i < count; i++)
            {
                if (i > 0)
                    SpinUntil(gapMs);
                RunFullFrame(0);
            }
        }

        /* Test 1. Thirty rendered frames with a 1 ms paint and a 2 ms gap after
           each. The streaming counters are exact, so the counts are asserted
           exactly; PaintCount allows one less because phases fold one frame late
           and the last frame's phases arrive only at EndWindow. The frame-to-frame
           gap is nominally 3 ms (paint plus gap), so its P50 lies in [2, 8]: 3 ms
           at 0.8 rounds down to 2, and 3 ms at 2.5 is 7.5. The paint mean is at
           least 1 ms times 0.8. */
        [Fact]
        public void RenderedFrames_AreCountedAndPhasesFolded()
        {
            ResetProfiler();
            int i;
            for (i = 0; i < 30; i++)
            {
                RunFullFrame(1);
                SpinUntil(2);
            }

            FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();
            Assert.Equal(30L, s.RenderedFrameCount);
            Assert.Equal(30L, s.TickCount);
            Assert.Equal(29L, s.InterFrameCount);
            Assert.True(s.PaintCount >= 28, "PaintCount " + s.PaintCount);
            Assert.Equal(0L, s.LockFailCount);
            Assert.InRange(s.InterFrameP50Ms, 2f, 8f);
            Assert.True(s.PaintMeanMs >= 0.8f, "PaintMeanMs " + s.PaintMeanMs);

            float[] intervals = new float[1800];
            int copied = FrameTimeProfiler.CopyWindowIntervals(intervals);
            Assert.Equal(29, copied);
            for (i = 0; i < copied; i++)
                Assert.True(intervals[i] > 0f, "interval " + i + " = " + intervals[i]);
        }

        /* Test 2. With a 5 ms vsync the hitch thresholds are 7.5, 10 and 20 ms. The
           2 ms gaps stay below 7.5 ms even at 2.5x overshoot, so only the two 25 ms
           gaps count, at every threshold. HitchSumMs is the sum of (gap - vsync)
           over the 2x hitches, nominally 2 * 20 = 40 ms, bounded by 0.8 and 2.5
           times that. Max is exact in the histogram; P99 of 51 gaps is the largest,
           which the histogram reports to within its 4 percent bin width, so 20 is a
           safe floor for a 25 ms value. GetStatistics sorts the ring itself, so its
           Max must agree. */
        [Fact]
        public void Hitches_AreCountedAgainstVsync()
        {
            ResetProfiler();
            RunFrames(40, 2);
            SpinUntil(25);
            RunFullFrame(0);
            SpinUntil(25);
            RunFullFrame(0);
            int i;
            for (i = 0; i < 10; i++)
            {
                SpinUntil(2);
                RunFullFrame(0);
            }

            FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();
            Assert.Equal(2L, s.Hitch2xCount);
            Assert.Equal(2L, s.Hitch4xCount);
            Assert.True(s.Hitch15xCount >= 2, "Hitch15xCount " + s.Hitch15xCount);
            Assert.InRange(s.HitchSumMs, 2f * (25f - VsyncMs) * 0.8f, 2f * (25f - VsyncMs) * 2.5f);
            Assert.True(s.InterFrameMaxMs >= 25f, "InterFrameMaxMs " + s.InterFrameMaxMs);
            Assert.True(s.InterFrameP99Ms >= 20f, "InterFrameP99Ms " + s.InterFrameP99Ms);

            FrameTimeStatistics exact = FrameTimeProfiler.GetStatistics();
            Assert.True(exact.InterFrameMaxMs >= 25f, "exact InterFrameMaxMs " + exact.InterFrameMaxMs);
        }

        /* Test 3. A pause event inside a 30 ms gap removes that gap from the
           inter-frame series: it is counted as excluded, it raises no 4x hitch
           (threshold 20 ms), and it is absent from the copied intervals, whose
           largest value is then a 2 ms gap, below 20 ms even at 2.5x. The 31
           rendered frames give 30 gaps, one of them excluded. */
        [Fact]
        public void PauseEvent_ExcludesTheGapContainingIt()
        {
            ResetProfiler();
            RunFrames(20, 2);
            FrameTimeProfiler.MarkPauseEvent();
            SpinUntil(30);
            RunFullFrame(0);
            int i;
            for (i = 0; i < 10; i++)
            {
                SpinUntil(2);
                RunFullFrame(0);
            }

            FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();
            Assert.Equal(1L, s.PauseExcludedCount);
            Assert.Equal(0L, s.Hitch4xCount);
            Assert.Equal(29L, s.InterFrameCount);

            float[] intervals = new float[1800];
            int copied = FrameTimeProfiler.CopyWindowIntervals(intervals);
            Assert.Equal((int)s.InterFrameCount, copied);
            float max = 0f;
            for (i = 0; i < copied; i++)
            {
                if (intervals[i] > max)
                    max = intervals[i];
            }
            Assert.True(max < 20f, "max copied interval " + max);
        }

        /* Test 4. Every frame reports a failed lock. Lock failures fold one frame
           late like the other phases, so after ten frames the window holds nine;
           the exact path, which scans the ring directly, already sees ten.
           Disabling closes the rolling window, whose closing pass folds the last
           frame, and the closed window's counters stay readable: then the
           streaming count is ten as well. */
        [Fact]
        public void LockFailures_AreCounted()
        {
            ResetProfiler();
            int i;
            for (i = 0; i < 10; i++)
            {
                FrameTimeProfiler.BeginFrame(_frameNumber++);
                FrameTimeProfiler.StampUpdate();
                FrameTimeProfiler.StampLockAttempt();
                FrameTimeProfiler.StampLockResult(false);
                FrameTimeProfiler.EndFrame();
            }
            Assert.Equal(9L, FrameTimeProfiler.GetStreamingStatistics().LockFailCount);
            Assert.Equal(10, FrameTimeProfiler.GetStatistics().LockFailCount);

            FrameTimeProfiler.IsEnabled = false;
            Assert.Equal(10L, FrameTimeProfiler.GetStreamingStatistics().LockFailCount);
        }

        /* Test 5. Frames that never call StampUpdate did not enter the render path:
           they tick the window but render nothing and form no inter-frame gap. */
        [Fact]
        public void UnrenderedTicks_CountAsTicksOnly()
        {
            ResetProfiler();
            int i;
            for (i = 0; i < 10; i++)
            {
                FrameTimeProfiler.BeginFrame(_frameNumber++);
                FrameTimeProfiler.EndFrame();
            }

            FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();
            Assert.Equal(10L, s.TickCount);
            Assert.Equal(0L, s.RenderedFrameCount);
            Assert.Equal(0L, s.InterFrameCount);
        }

        /* Test 6. Enabling opens a rolling window; BeginWindow replaces it with an
           explicit one; EndWindow on an explicit window opens a new empty rolling
           window while the profiler is enabled; ResetRollingWindow empties the
           rolling window; disabling closes whatever is open. */
        [Fact]
        public void Windows_ImplicitAndExplicitLifecycle()
        {
            ResetProfiler();
            Assert.True(FrameTimeProfiler.IsWindowOpen);
            Assert.True(FrameTimeProfiler.IsImplicitWindow);

            FrameTimeProfiler.BeginWindow();
            Assert.True(FrameTimeProfiler.IsWindowOpen);
            Assert.False(FrameTimeProfiler.IsImplicitWindow);
            RunFrames(5, 0.5);
            Assert.Equal(5L, FrameTimeProfiler.GetStreamingStatistics().RenderedFrameCount);

            FrameTimeProfiler.EndWindow();
            Assert.True(FrameTimeProfiler.IsWindowOpen);
            Assert.True(FrameTimeProfiler.IsImplicitWindow);
            Assert.Equal(0L, FrameTimeProfiler.GetStreamingStatistics().RenderedFrameCount);

            RunFrames(3, 0.5);
            Assert.Equal(3L, FrameTimeProfiler.GetStreamingStatistics().RenderedFrameCount);
            FrameTimeProfiler.ResetRollingWindow();
            Assert.Equal(0L, FrameTimeProfiler.GetStreamingStatistics().RenderedFrameCount);
            Assert.True(FrameTimeProfiler.IsWindowOpen);

            FrameTimeProfiler.IsEnabled = false;
            Assert.False(FrameTimeProfiler.IsWindowOpen);
        }

        /* Test 7. BeginWindow records a WindowBegin marker at or after the window
           start, and CopyMarkers returns only markers from that start on, oldest
           first. The 1 ms wait before BeginWindow keeps the previous rolling
           window's WindowBegin marker strictly before the new window start, so
           only three markers are inside it. */
        [Fact]
        public void Markers_AreCopiedFromWindowStartInOrder()
        {
            ResetProfiler();
            SpinUntil(1);
            FrameTimeProfiler.BeginWindow();
            FrameTimeProfiler.MarkEvent(FrameTimeProfiler.MarkerCodes.PhaseStart);
            SpinUntil(1);
            FrameTimeProfiler.MarkEvent(FrameTimeProfiler.MarkerCodes.PhaseEnd);

            FrameTimeMarker[] markers = new FrameTimeMarker[256];
            int count = FrameTimeProfiler.CopyMarkers(markers);
            Assert.Equal(3, count);
            Assert.Equal(FrameTimeProfiler.MarkerCodes.WindowBegin, markers[0].Code);
            Assert.Equal(FrameTimeProfiler.MarkerCodes.PhaseStart, markers[1].Code);
            Assert.Equal(FrameTimeProfiler.MarkerCodes.PhaseEnd, markers[2].Code);
            Assert.True(markers[0].Timestamp <= markers[1].Timestamp);
            Assert.True(markers[1].Timestamp <= markers[2].Timestamp);
            Assert.Equal(3, FrameTimeProfiler.GetStreamingStatistics().MarkerCount);
        }

        /* Test 8. The vsync period round-trips through the double bit pattern it is
           stored as, so a float survives exactly; 1e-3 is a formality. Zero, a
           negative value, NaN and infinity are rejected and leave it unchanged. */
        [Fact]
        public void VsyncPeriod_AcceptsPositiveFiniteOnly()
        {
            ResetProfiler();
            FrameTimeProfiler.SetVsyncPeriodMs(8.333f);
            Assert.Equal(8.333, (double)FrameTimeProfiler.VsyncPeriodMs, 1e-3);
            FrameTimeProfiler.SetVsyncPeriodMs(0f);
            Assert.Equal(8.333, (double)FrameTimeProfiler.VsyncPeriodMs, 1e-3);
            FrameTimeProfiler.SetVsyncPeriodMs(-1f);
            Assert.Equal(8.333, (double)FrameTimeProfiler.VsyncPeriodMs, 1e-3);
            FrameTimeProfiler.SetVsyncPeriodMs(float.NaN);
            Assert.Equal(8.333, (double)FrameTimeProfiler.VsyncPeriodMs, 1e-3);
            FrameTimeProfiler.SetVsyncPeriodMs(float.PositiveInfinity);
            Assert.Equal(8.333, (double)FrameTimeProfiler.VsyncPeriodMs, 1e-3);
        }

        /* Test 9. The frame path must not allocate: the ring, histograms and marker
           ring are pre-allocated and the statistics struct is filled in place. The
           one documented exception is the frame after a GC, which reads
           GCMemoryInfo. A gen-0 collection during a loop that allocates nothing is
           not expected, but if the runtime performs one anyway that frame allocates
           legitimately, so the test reports and returns instead of failing. xunit
           2.x has no dynamic skip, which is why this is a return. */
        [Fact]
        public void FramePath_DoesNotAllocate()
        {
            ResetProfiler();
            float sink = 0f;

            /* Warm-up: JIT and first-call setup of every member the loop uses */
            sink += RunAllocationLoop(50);

            int gen0Before = GC.CollectionCount(0);
            long before = GC.GetAllocatedBytesForCurrentThread();
            sink += RunAllocationLoop(200);
            long after = GC.GetAllocatedBytesForCurrentThread();
            int gen0After = GC.CollectionCount(0);

            if (gen0After != gen0Before)
            {
                _output.WriteLine("Skipped: a gen-0 collection ran during the measured loop ("
                    + (after - before) + " bytes allocated), so the GC-info read is expected.");
                return;
            }

            Assert.Equal(0L, after - before);
            Assert.True(sink >= 0f);
        }

        private float RunAllocationLoop(int frames)
        {
            float sink = 0f;
            int i;
            for (i = 0; i < frames; i++)
            {
                RunFullFrame(0);
                if (i % 10 == 0)
                    FrameTimeProfiler.MarkEvent(FrameTimeProfiler.MarkerCodes.FeltHitch);
            }
            for (i = 0; i < 20; i++)
            {
                FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();
                sink += s.InterFrameP99Ms + s.PaintMeanMs + s.FpsMean;
            }
            return sink;
        }

        /* Test 10. 300 frames alternating 1 and 3 ms gaps plus three frames after
           20 ms gaps: 302 gaps. Both paths pick the 299th smallest as P99 (the
           histogram uses ceil(0.99 * 302) = 299 one-based, the exact path
           (int)(302 * 0.99) = 298 zero-based), which is the largest of the 3 ms
           gaps, so the only difference is the histogram's bin interpolation of
           about 4 percent; 10 percent covers it. Max is exact in the histogram and
           must match to the float. */
        [Fact]
        public void StreamingPercentiles_AgreeWithExactPath()
        {
            ResetProfiler();
            RunFullFrame(0);
            int i;
            for (i = 1; i < 300; i++)
            {
                SpinUntil(i % 2 == 0 ? 1 : 3);
                RunFullFrame(0);
            }
            for (i = 0; i < 3; i++)
            {
                SpinUntil(20);
                RunFullFrame(0);
            }

            FrameTimeStreamingStatistics s = FrameTimeProfiler.GetStreamingStatistics();
            FrameTimeStatistics exact = FrameTimeProfiler.GetStatistics();
            Assert.Equal(302L, s.InterFrameCount);
            Assert.Equal(302, exact.SampleCount);
            Assert.InRange(s.InterFrameP99Ms, exact.InterFrameP99Ms * 0.9f, exact.InterFrameP99Ms * 1.1f);
            Assert.Equal(exact.InterFrameMaxMs, s.InterFrameMaxMs);
        }

        /* Test 11. The CSV has a header whose PaintMs column precedes FlushMs, and
           one data row per sample in the ring. */
        [Fact]
        public void DumpToCsv_WritesHeaderAndOneRowPerFrame()
        {
            ResetProfiler();
            RunFrames(20, 0.5);

            string path = Path.GetTempFileName();
            try
            {
                FrameTimeProfiler.DumpToCsv(path);
                string[] lines = File.ReadAllLines(path);
                Assert.True(lines.Length >= 21, "line count " + lines.Length);
                int paintIdx = lines[0].IndexOf("PaintMs", StringComparison.Ordinal);
                int flushIdx = lines[0].IndexOf("FlushMs", StringComparison.Ordinal);
                Assert.True(paintIdx >= 0);
                Assert.True(flushIdx > paintIdx);
                Assert.True(lines.Length - 1 >= 20, "row count " + (lines.Length - 1));
            }
            finally
            {
                File.Delete(path);
            }
        }

        /* Test 12. PublishDashboardSnapshot hands the streaming window to the
           dashboard along with the probe readings; after 20 rendered frames the
           window has elapsed time and frames, so its mean FPS is positive. The
           probe values are passed through unchanged. */
        [Fact]
        public void PublishDashboardSnapshot_ReachesTheDashboard()
        {
            ResetProfiler();
            GHDebugDashboard.ResetRecording();
            GHUiThreadProbe.LatencyP99Ms = 7.5f;
            GHUiThreadProbe.LatencyMaxMs = 12.25f;
            GHUiThreadProbe.LateTickCount = 3;
            GHUiThreadProbe.TickCount = 400;
            RunFrames(20, 0.5);

            FrameTimeProfiler.PublishDashboardSnapshot();

            Assert.Equal(1, GHDebugDashboard.PublishCount);
            Assert.True(GHDebugDashboard.LastProfilerEnabled);
            Assert.True(GHDebugDashboard.LastStats.FpsMean > 0f, "FpsMean " + GHDebugDashboard.LastStats.FpsMean);
            Assert.Equal(20L, GHDebugDashboard.LastStats.RenderedFrameCount);
            Assert.Equal(7.5f, GHDebugDashboard.LastUiLatencyP99Ms);
            Assert.Equal(12.25f, GHDebugDashboard.LastUiLatencyMaxMs);
            Assert.Equal(3L, GHDebugDashboard.LastUiLateTicks);
            Assert.Equal(400L, GHDebugDashboard.LastUiTicks);
            Assert.True(GHDebugDashboard.LastMemory.HeapSizeBytes > 0);
        }

        /* Test 13. GHGameTurnTimer measures from InputWaitEnd (input delivered,
           core processing) to the next outermost InputWaitStart. After the first
           cycle the timer sits at depth 1, inside an input wait, as it does in the
           game between turns; a nested callback entry and exit from there must not
           begin a processing interval, and only the outermost exit does. With the
           window closed the histogram stops but LastProcessingMs still updates.
           Durations use the same 0.8 floor as the frame tests. */
        [Fact]
        public void GameTurnTimer_CountsOutermostProcessingOnly()
        {
            GHGameTurnTimer.Reset();
            GHGameTurnTimer.BeginWindow();
            Assert.True(GHGameTurnTimer.IsWindowOpen);

            GHGameTurnTimer.InputWaitEnd();
            Assert.True(GHGameTurnTimer.IsProcessing);
            SpinUntil(3);
            GHGameTurnTimer.InputWaitStart();
            Assert.False(GHGameTurnTimer.IsProcessing);
            Assert.Equal(1L, GHGameTurnTimer.ProcessingCount);
            Assert.True(GHGameTurnTimer.LastProcessingMs >= 3f * 0.8f, "LastProcessingMs " + GHGameTurnTimer.LastProcessingMs);
            Assert.True(GHGameTurnTimer.ProcessingMaxMs >= 3f * 0.8f, "ProcessingMaxMs " + GHGameTurnTimer.ProcessingMaxMs);
            Assert.Equal(1, GHGameTurnTimer.CurrentDepth);

            /* Nested: re-entered callback (depth 2), inner exit (depth 1, no
               interval), outer exit (depth 0, interval begins), then the next
               outermost entry closes it */
            GHGameTurnTimer.InputWaitStart();
            Assert.Equal(2, GHGameTurnTimer.CurrentDepth);
            GHGameTurnTimer.InputWaitEnd();
            Assert.Equal(1, GHGameTurnTimer.CurrentDepth);
            Assert.False(GHGameTurnTimer.IsProcessing);
            Assert.Equal(1L, GHGameTurnTimer.ProcessingCount);
            GHGameTurnTimer.InputWaitEnd();
            Assert.True(GHGameTurnTimer.IsProcessing);
            SpinUntil(2);
            GHGameTurnTimer.InputWaitStart();
            Assert.Equal(2L, GHGameTurnTimer.ProcessingCount);
            Assert.True(GHGameTurnTimer.LastProcessingMs >= 2f * 0.8f, "LastProcessingMs " + GHGameTurnTimer.LastProcessingMs);
            Assert.Equal(2L, GHGameTurnTimer.WaitingCount);

            /* Window closed: a cycle updates the last value but not the series. Two
               independent spin-waits practically never measure the same number of
               100 ns ticks, so a changed value shows the update; the bounds are
               1 ms at 0.8 and 2.5. */
            float previousLastMs = GHGameTurnTimer.LastProcessingMs;
            GHGameTurnTimer.EndWindow();
            Assert.False(GHGameTurnTimer.IsWindowOpen);
            GHGameTurnTimer.InputWaitEnd();
            SpinUntil(1);
            GHGameTurnTimer.InputWaitStart();
            Assert.Equal(2L, GHGameTurnTimer.ProcessingCount);
            Assert.NotEqual(previousLastMs, GHGameTurnTimer.LastProcessingMs);
            Assert.InRange(GHGameTurnTimer.LastProcessingMs, 1f * 0.8f, 1f * 2.5f);
        }
    }
}

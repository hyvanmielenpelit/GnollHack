using System.Diagnostics;
using System.IO;
using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* GHFrameTimeline and GHCadenceMonitor are static; every test class that touches them
       joins one collection, which xunit runs sequentially. */
    [Collection(StaticStateCollection)]
    public class GHFrameTimelineTests
    {
        public const string StaticStateCollection = "GHFrameTimeline static state";

        private static readonly long Frequency = Stopwatch.Frequency;

        private static void Restart()
        {
            GHFrameTimeline.IsEnabled = false;
            GHFrameTimeline.IsEnabled = true;
        }

        private static GHFrameRecord[] Snapshot(out int count)
        {
            GHFrameRecord[] records = new GHFrameRecord[GHFrameTimeline.Capacity];
            count = GHFrameTimeline.CopyRecords(records);
            return records;
        }

        /* One tick that renders and invalidates, as the render loop does */
        private static long RenderedTick(long mainCounter)
        {
            long id = GHFrameTimeline.BeginTick();
            GHFrameTimeline.StampTarget(60, 120);
            GHFrameTimeline.StampUpdate(mainCounter, mainCounter / 2);
            GHFrameTimeline.StampInvalidate(GHInvalidateOutcome.Invalidated);
            GHFrameTimeline.EndTick(GHPacingDecision.Rendered);
            return id;
        }

        [Fact]
        public void Disabled_RecordsNothing()
        {
            GHFrameTimeline.IsEnabled = false;
            Assert.Equal(0, GHFrameTimeline.BeginTick());
            Assert.Equal(0, GHFrameTimeline.BeginPaint(true));
            Restart();
            int n;
            Snapshot(out n);
            Assert.Equal(0, n);
        }

        [Fact]
        public void Paint_IsAttributedToTheTickThatInvalidated()
        {
            Restart();
            long id = RenderedTick(10);
            long painted = GHFrameTimeline.BeginPaint(false);
            Assert.Equal(id, painted);
            GHFrameTimeline.StampLock(1, 2, true);
            GHFrameTimeline.StampDrawEnd(painted);
            GHFrameTimeline.EndPaint(painted, 10, 5, 3);

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(1, n);
            Assert.Equal(GHPacingDecision.Rendered, r[0].Pacing);
            Assert.Equal(GHInvalidateOutcome.Invalidated, r[0].Invalidate);
            Assert.Equal(GHPaintOutcome.Painted, r[0].Paint);
            Assert.False(r[0].PaintOnUiThread);
            Assert.True(r[0].LockAcquired);
            Assert.Equal(10, r[0].MainCounter);
            Assert.Equal(10, r[0].PaintedMainCounter);
            Assert.Equal(5, r[0].PaintedGeneralCounter);
            Assert.Equal(3, r[0].PaintedMapGeneration);
            Assert.Equal((short)60, r[0].TargetFps);
            Assert.Equal((short)120, r[0].AssumedRefreshHz);
            Assert.True(r[0].FlushEndTicks >= r[0].PaintStartTicks);
        }

        [Fact]
        public void LaterInvalidation_BeforeAnyPaint_CoalescesTheEarlierOne()
        {
            Restart();
            long first = RenderedTick(1);
            long second = RenderedTick(2);
            long painted = GHFrameTimeline.BeginPaint(false);
            Assert.Equal(second, painted);
            GHFrameTimeline.EndPaint(painted, 2, 1, 0);

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(2, n);
            Assert.Equal(first, r[0].FrameId);
            Assert.Equal(GHPaintOutcome.Coalesced, r[0].Paint);
            Assert.Equal(GHPaintOutcome.Painted, r[1].Paint);
            Assert.Equal(1, GHFrameTimeline.CoalescedCount);
        }

        [Fact]
        public void SkippedTicks_AreNotCoalesced()
        {
            Restart();
            RenderedTick(1);
            GHFrameTimeline.BeginTick();
            GHFrameTimeline.EndTick(GHPacingDecision.SkippedDivisor);
            long painted = GHFrameTimeline.BeginPaint(true);
            Assert.Equal(1, painted);
            Assert.Equal(0, GHFrameTimeline.CoalescedCount);

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(GHPacingDecision.SkippedDivisor, r[1].Pacing);
            Assert.Equal(GHPaintOutcome.None, r[1].Paint);
        }

        [Fact]
        public void RequestWork_IsAttachedToTheNextTick()
        {
            Restart();
            RenderedTick(1);
            GHFrameTimeline.AddRequestWork(GHContentEvent.FloatingText, 100);
            GHFrameTimeline.AddRequestWork(GHContentEvent.Message, 50);
            RenderedTick(2);
            RenderedTick(3);

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(GHContentEvent.None, r[0].ContentEvents);
            Assert.Equal(GHContentEvent.FloatingText | GHContentEvent.Message, r[1].ContentEvents);
            Assert.Equal(150, r[1].RequestTicks);
            Assert.Equal(GHContentEvent.None, r[2].ContentEvents);
            Assert.Equal(0, r[2].RequestTicks);
        }

        [Fact]
        public void PaintOfNewMapData_IsMarkedMapUpdate()
        {
            Restart();
            long[] generations = { 5, 5, 6 };
            for (int i = 0; i < generations.Length; i++)
            {
                RenderedTick(i + 1);
                long painted = GHFrameTimeline.BeginPaint(true);
                GHFrameTimeline.EndPaint(painted, i + 1, 0, generations[i]);
            }

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(GHContentEvent.None, r[0].ContentEvents & GHContentEvent.MapUpdate);
            Assert.Equal(GHContentEvent.None, r[1].ContentEvents & GHContentEvent.MapUpdate);
            Assert.Equal(GHContentEvent.MapUpdate, r[2].ContentEvents & GHContentEvent.MapUpdate);
        }

        [Fact]
        public void PaintWithoutInvalidation_IsCountedAsOrphan()
        {
            Restart();
            Assert.Equal(0, GHFrameTimeline.BeginPaint(true));
            Assert.Equal(1, GHFrameTimeline.OrphanPaintCount);
        }

        [Fact]
        public void EarlyReturn_RecordsItsOutcome()
        {
            Restart();
            RenderedTick(1);
            long painted = GHFrameTimeline.BeginPaint(true);
            GHFrameTimeline.SetCurrentPaintOutcome(GHPaintOutcome.CanvasTooSmall);
            GHFrameTimeline.EndPaint(painted, 0, 0, 0);

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(GHPaintOutcome.CanvasTooSmall, r[0].Paint);
        }

        [Fact]
        public void Ring_KeepsTheNewestCapacityTicksInOrder()
        {
            Restart();
            int total = GHFrameTimeline.Capacity + 10;
            for (int i = 0; i < total; i++)
            {
                GHFrameTimeline.BeginTick();
                GHFrameTimeline.EndTick(GHPacingDecision.Rendered);
            }

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(GHFrameTimeline.Capacity, n);
            Assert.Equal(11, r[0].FrameId);
            Assert.Equal(total, r[n - 1].FrameId);
            for (int i = 1; i < n; i++)
                Assert.Equal(r[i - 1].FrameId + 1, r[i].FrameId);
        }

        [Fact]
        public void RefreshPeriod_IsTheMedianOfPlatformFrameDeltas()
        {
            Restart();
            long period = Frequency / 120;
            long t = 1000 * Frequency;
            for (int i = 0; i < 20; i++)
            {
                /* One missed vsync in the middle must not move the median */
                t += i == 10 ? 2 * period : period;
                GHFrameTimeline.SetPendingPlatformFrame(0, 0, t);
                GHFrameTimeline.BeginTick();
                GHFrameTimeline.EndTick(GHPacingDecision.Rendered);
            }
            Assert.Equal(period, GHFrameTimeline.MeasuredRefreshPeriodTicks);
            Assert.Equal(1000.0 / 120.0, GHFrameTimeline.MeasuredRefreshPeriodMs, 3);
        }

        [Fact]
        public void RefreshPeriod_FollowsARateChange()
        {
            Restart();
            long t = 1000 * Frequency;
            for (int i = 0; i < 20; i++)
            {
                t += Frequency / 120;
                GHFrameTimeline.SetPendingPlatformFrame(0, 0, t);
                GHFrameTimeline.BeginTick();
            }
            for (int i = 0; i < 20; i++)
            {
                t += Frequency / 60;
                GHFrameTimeline.SetPendingPlatformFrame(0, 0, t);
                GHFrameTimeline.BeginTick();
            }
            Assert.Equal(Frequency / 60, GHFrameTimeline.MeasuredRefreshPeriodTicks);
        }

        [Fact]
        public void RepeatedPlatformFrameTime_IsFlaggedAsDuplicate()
        {
            Restart();
            GHFrameTimeline.SetPendingPlatformFrame(0, 0, 5000);
            GHFrameTimeline.BeginTick();
            GHFrameTimeline.SetPendingPlatformFrame(0, 0, 5000);
            GHFrameTimeline.BeginTick();

            int n;
            GHFrameRecord[] r = Snapshot(out n);
            Assert.Equal(GHFrameFlags.None, r[0].Flags & GHFrameFlags.DuplicateCallback);
            Assert.Equal(GHFrameFlags.DuplicateCallback, r[1].Flags & GHFrameFlags.DuplicateCallback);
        }

        [Fact]
        public void ClockAnchor_ConvertsPlatformNanosToStopwatchTicks()
        {
            Restart();
            Assert.Equal(0, GHFrameTimeline.PlatformNanosToTicks(123));
            GHFrameTimeline.UpdatePlatformClockAnchor(1000000000L, 5000);
            Assert.Equal(5000 + Frequency, GHFrameTimeline.PlatformNanosToTicks(2000000000L));
            Assert.Equal(5000 - Frequency / 2, GHFrameTimeline.PlatformNanosToTicks(500000000L));
            Assert.Equal(1000000000L, GHFrameTimeline.FirstClockAnchor.PlatformNanos);
        }

        [Fact]
        public void Dump_WritesOneRowPerTick()
        {
            Restart();
            RenderedTick(1);
            RenderedTick(2);
            string path = Path.Combine(Path.GetTempPath(), "ghframetimeline_test_" + System.Guid.NewGuid().ToString("N") + ".csv");
            try
            {
                GHFrameTimeline.DumpToCsv(path);
                string[] lines = File.ReadAllLines(path);
                int dataRows = 0;
                bool header = false;
                foreach (string line in lines)
                {
                    if (line.StartsWith("#"))
                        continue;
                    if (!header)
                    {
                        Assert.StartsWith("FrameId,", line);
                        header = true;
                        continue;
                    }
                    dataRows++;
                }
                Assert.Equal(2, dataRows);
            }
            finally
            {
                if (File.Exists(path))
                    File.Delete(path);
            }
        }
    }
}

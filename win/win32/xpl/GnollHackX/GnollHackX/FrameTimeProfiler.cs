using System;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace GnollHackX
{
    public struct FrameTimeSample
    {
        public long FrameNumber;
        public long TicksFrameStart;
        public long TicksUpdateStart;
        public long TicksLockAttempt;
        public long TicksLockResult;
        public bool LockAcquired;
        public long TicksPaintStart;
        public long TicksPaintEnd;
        public long TicksFrameEnd;
    }

    public struct FrameTimeStatistics
    {
        public float InterFrameAvgMs;
        public float InterFrameStdDevMs;
        public float InterFrameP95Ms;
        public float InterFrameP99Ms;
        public float InterFrameMaxMs;
        public float AvgUpdateMs;
        public float AvgLockWaitMs;
        public float AvgPaintMs;
        public int DroppedFrameCount;
        public float DroppedFramePct;
        public int LockFailCount;
        public float LockFailPct;
        public float FPS;
        public int SampleCount;
    }

    public static class FrameTimeProfiler
    {
        private const int BufferSize = 1800;
        private static readonly FrameTimeSample[] _buffer = new FrameTimeSample[BufferSize];
        private static long _writeIndex = -1;
        private static readonly double _msPerTick = 1000.0 / Stopwatch.Frequency;

        /* Pre-allocated sort buffer to avoid GC pressure on mobile (Bug 5 fix) */
        private static readonly float[] _sortBuffer = new float[BufferSize];

        /* Wrap-safe modulo that always returns a non-negative index (Bug 1 fix) */
        private static int SafeIndex(long value)
        {
            return (int)((value % BufferSize + BufferSize) % BufferSize);
        }

        public static void BeginFrame(long frameNumber)
        {
            long idx = Interlocked.Increment(ref _writeIndex);
            int index = SafeIndex(idx);
            _buffer[index] = new FrameTimeSample
            {
                FrameNumber = frameNumber,
                TicksFrameStart = Stopwatch.GetTimestamp()
            };
        }

        public static void StampUpdate()
        {
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx)].TicksUpdateStart = Stopwatch.GetTimestamp();
        }

        public static void StampLockAttempt()
        {
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx)].TicksLockAttempt = Stopwatch.GetTimestamp();
        }

        public static void StampLockResult(bool acquired)
        {
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            int index = SafeIndex(idx);
            _buffer[index].TicksLockResult = Stopwatch.GetTimestamp();
            _buffer[index].LockAcquired = acquired;
        }

        public static void StampPaintStart()
        {
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx)].TicksPaintStart = Stopwatch.GetTimestamp();
        }

        public static void StampPaintEnd()
        {
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx)].TicksPaintEnd = Stopwatch.GetTimestamp();
        }

        public static void EndFrame()
        {
            long idx = Interlocked.Read(ref _writeIndex);
            if (idx < 0) return;
            _buffer[SafeIndex(idx)].TicksFrameEnd = Stopwatch.GetTimestamp();
        }

        public static FrameTimeStatistics GetStatistics()
        {
            long currentWriteIndex = Interlocked.Read(ref _writeIndex);
            if (currentWriteIndex < 0)
                return new FrameTimeStatistics();

            /* Bug 2 fix: +1 because _writeIndex is 0-based */
            int sampleCount = (int)Math.Min(currentWriteIndex + 1, BufferSize);
            long startIndex = currentWriteIndex >= BufferSize ? currentWriteIndex - BufferSize + 1 : 0;

            /*
             * Bug 4 fix: only compute inter-frame statistics for frames that
             * actually entered the render path (TicksUpdateStart > 0).
             * Skipped frames (refresh-rate divisor) are excluded so that FPS
             * and jitter metrics reflect actual rendering, not platform
             * callback frequency.
             */
            int interFrameCount = 0;
            double totalInterFrameTime = 0;
            double totalUpdateMs = 0;
            double totalLockWaitMs = 0;
            double totalPaintMs = 0;
            int droppedCount = 0;
            int lockAttemptCount = 0;
            int lockFailCount = 0;
            int renderedCount = 0;

            float targetFrameTimeMs = 1000f / 60f; /* Approx 16.67ms */
            float droppedThresholdMs = targetFrameTimeMs * 1.5f;

            long prevRenderedFrameStart = 0;
            bool hasPrevRendered = false;

            for (int i = 0; i < sampleCount; i++)
            {
                int currIdx = SafeIndex(startIndex + i);
                FrameTimeSample curr = _buffer[currIdx];

                /* Skip frames that did not enter the render path */
                if (curr.TicksUpdateStart == 0)
                    continue;

                renderedCount++;

                if (hasPrevRendered)
                {
                    float interFrameMs = (float)((curr.TicksFrameStart - prevRenderedFrameStart) * _msPerTick);
                    _sortBuffer[interFrameCount] = interFrameMs;
                    interFrameCount++;
                    totalInterFrameTime += interFrameMs;

                    if (interFrameMs > droppedThresholdMs)
                        droppedCount++;
                }

                if (curr.TicksUpdateStart > 0 && curr.TicksLockResult > 0)
                    totalUpdateMs += (curr.TicksLockResult - curr.TicksUpdateStart) * _msPerTick;

                /* Bug 3 fix: only count lock fails when a lock was actually attempted */
                if (curr.TicksLockAttempt > 0)
                {
                    lockAttemptCount++;
                    if (curr.TicksLockResult > 0)
                        totalLockWaitMs += (curr.TicksLockResult - curr.TicksLockAttempt) * _msPerTick;
                    if (!curr.LockAcquired)
                        lockFailCount++;
                }

                if (curr.TicksPaintStart > 0 && curr.TicksPaintEnd > 0)
                    totalPaintMs += (curr.TicksPaintEnd - curr.TicksPaintStart) * _msPerTick;

                prevRenderedFrameStart = curr.TicksFrameStart;
                hasPrevRendered = true;
            }

            if (interFrameCount == 0) return new FrameTimeStatistics();

            float avgInterFrame = (float)(totalInterFrameTime / interFrameCount);

            /* Variance */
            double sumSqDiff = 0;
            for (int i = 0; i < interFrameCount; i++)
            {
                double diff = _sortBuffer[i] - avgInterFrame;
                sumSqDiff += diff * diff;
            }
            float stdDev = (float)Math.Sqrt(sumSqDiff / interFrameCount);

            Array.Sort(_sortBuffer, 0, interFrameCount);

            int p95Idx = (int)(interFrameCount * 0.95);
            int p99Idx = (int)(interFrameCount * 0.99);

            return new FrameTimeStatistics
            {
                InterFrameAvgMs = avgInterFrame,
                InterFrameStdDevMs = stdDev,
                InterFrameP95Ms = _sortBuffer[Math.Min(p95Idx, interFrameCount - 1)],
                InterFrameP99Ms = _sortBuffer[Math.Min(p99Idx, interFrameCount - 1)],
                InterFrameMaxMs = _sortBuffer[interFrameCount - 1],
                AvgUpdateMs = renderedCount > 0 ? (float)(totalUpdateMs / renderedCount) : 0,
                AvgLockWaitMs = renderedCount > 0 ? (float)(totalLockWaitMs / renderedCount) : 0,
                AvgPaintMs = renderedCount > 0 ? (float)(totalPaintMs / renderedCount) : 0,
                DroppedFrameCount = droppedCount,
                DroppedFramePct = interFrameCount > 0 ? (float)droppedCount / interFrameCount * 100f : 0,
                LockFailCount = lockFailCount,
                LockFailPct = lockAttemptCount > 0 ? (float)lockFailCount / lockAttemptCount * 100f : 0,
                FPS = avgInterFrame > 0 ? 1000f / avgInterFrame : 0,
                SampleCount = interFrameCount
            };
        }

        public static string GetScreenLogSummary()
        {
            var stats = GetStatistics();
            if (stats.SampleCount == 0) return "FT: No data";
            
            return $"FT: {stats.InterFrameAvgMs:0.0}ms SD:{stats.InterFrameStdDevMs:0.0} P95:{stats.InterFrameP95Ms:0.0} P99:{stats.InterFrameP99Ms:0.0} Max:{stats.InterFrameMaxMs:0.0} Drop:{stats.DroppedFramePct:0.0}% Lock:{stats.LockFailPct:0.0}% Upd:{stats.AvgUpdateMs:0.0} Pnt:{stats.AvgPaintMs:0.0}";
        }

        public static void DumpToCsv(string path)
        {
            long currentWriteIndex = Interlocked.Read(ref _writeIndex);
            if (currentWriteIndex < 0) return;

            /* Bug 2 fix: +1 because _writeIndex is 0-based */
            int sampleCount = (int)Math.Min(currentWriteIndex + 1, BufferSize);
            long startIndex = currentWriteIndex >= BufferSize ? currentWriteIndex - BufferSize + 1 : 0;

            using (StreamWriter writer = new StreamWriter(path))
            {
                writer.WriteLine("FrameNumber,Rendered,InterFrameMs,UpdateMs,LockWaitMs,LockAcquired,PaintMs,TotalFrameMs");

                FrameTimeSample prev = default;
                bool hasPrev = false;

                for (int i = 0; i < sampleCount; i++)
                {
                    int currIdx = SafeIndex(startIndex + i);
                    FrameTimeSample curr = _buffer[currIdx];

                    bool rendered = curr.TicksUpdateStart > 0;

                    float interFrameMs = 0;
                    if (hasPrev)
                    {
                        interFrameMs = (float)((curr.TicksFrameStart - prev.TicksFrameStart) * _msPerTick);
                    }

                    float updateMs = curr.TicksUpdateStart > 0 && curr.TicksLockResult > 0 
                        ? (float)((curr.TicksLockResult - curr.TicksUpdateStart) * _msPerTick) : 0;
                    
                    float lockWaitMs = curr.TicksLockAttempt > 0 && curr.TicksLockResult > 0 
                        ? (float)((curr.TicksLockResult - curr.TicksLockAttempt) * _msPerTick) : 0;
                    
                    float paintMs = curr.TicksPaintStart > 0 && curr.TicksPaintEnd > 0 
                        ? (float)((curr.TicksPaintEnd - curr.TicksPaintStart) * _msPerTick) : 0;
                    
                    float totalFrameMs = curr.TicksFrameStart > 0 && curr.TicksFrameEnd > 0 
                        ? (float)((curr.TicksFrameEnd - curr.TicksFrameStart) * _msPerTick) : 0;

                    writer.WriteLine($"{curr.FrameNumber},{rendered},{interFrameMs:0.00},{updateMs:0.00},{lockWaitMs:0.00},{curr.LockAcquired},{paintMs:0.00},{totalFrameMs:0.00}");

                    prev = curr;
                    hasPrev = true;
                }
            }
        }
    }
}

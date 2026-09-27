using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
#if GNH_MAUI
using Microsoft.Maui.Storage;
#else
using Xamarin.Essentials;
#endif

namespace GnollHackX.Performance
{
    /* One sample of a background report's series. T is seconds from the window start,
       negative before it; the other fields are percentages (Faults per second), NaN
       where the platform does not report the signal. */
    public struct GHBackgroundReportSample
    {
        public float T;
        public float Sys;
        public float Own;
        public float Other;
        public float Disk;
        public float AvailPct;
        public float Faults;
    }

    /* Everything a run record's "background" block holds for one measurement window:
       the window and pre-window summaries, the named processes and activities of the
       process interval, the sample series and the verdict. */
    public sealed class GHBackgroundReport
    {
        public const string SourceInApp = "in-app";

        public const string SignalSystemCpu = "systemCpu";
        public const string SignalDisk = "disk";
        public const string SignalMemory = "memory";
        public const string SignalHardFaults = "hardFaults";
        public const string SignalProcesses = "processes";
        public const string SignalGpu = "gpu";

        public int SamplerVersion = GHBackgroundLoad.SamplerVersion;
        public string Source = SourceInApp;
        public int IntervalMs = GHBackgroundLoad.SampleIntervalMs;
        public int LogicalProcessors;
        public float Coverage;
        public float SamplerBusyMs;
        public readonly List<string> Signals = new List<string>();
        public GHBackgroundSummary Window;
        public GHBackgroundSummary PreWindow;
        public float OtherGpuPct = float.NaN;
        public readonly List<GHProcessLoad> Processes = new List<GHProcessLoad>();
        public List<GHBackgroundActivity> Activities = new List<GHBackgroundActivity>();
        public readonly List<GHBackgroundReportSample> Samples = new List<GHBackgroundReportSample>();
        public GHBackgroundVerdict Verdict;
        public string Reason;

        public string VerdictName
        {
            get { return GHBackgroundLoad.VerdictName(Verdict); }
        }

        public bool HasSignal(string signal)
        {
            return Signals.Contains(signal);
        }
    }

    /* Whole-machine load sampler for performance runs. A dedicated background thread
       reads IPlatformService.TryGetSystemLoadSample once a second on a Stopwatch
       schedule (ticks stay on multiples of the interval from the thread start, so late
       ticks never accumulate drift) into a ring of the last RingCapacity samples. The
       thread runs while at least one Acquire is outstanding; the first sample after a
       start only primes the platform's rate counters and is dropped.

       Sample timestamps are DateTime.UtcNow.Ticks, the base of every window range passed
       in. The per-process interval is collected at window boundaries only: its begin on
       the thread pool before a window, its end after the window has closed.

       The tick allocates nothing after the first one. The ring lock is held only while
       copying. Every public member swallows exceptions. Must compile under C# 7.3. */
    public static class GHSystemLoadSampler
    {
        public const string PreferenceKey = "PerformanceBackgroundSampler";
        public const int RingCapacity = 900;
        public const int PreWindowSeconds = 10;
        public const int ProcessIntervalBeginWaitMs = 5000;

        private static readonly object _ringLock = new object();
        private static readonly GHSystemLoadSample[] _ring = new GHSystemLoadSample[RingCapacity];
        private static readonly float[] _ringBusyMs = new float[RingCapacity];
        private static int _ringNext;
        private static int _ringCount;

        private static readonly object _lifetimeLock = new object();
        private static int _refCount;
        private static ManualResetEvent _stopEvent;
        private static int _hasCpuSignal;

        private static readonly object _intervalLock = new object();
        private static Task _intervalBeginTask;
        private static long _intervalStartTicks;
        private static bool _lastIntervalValid;
        private static long _lastIntervalStartTicks;
        private static long _lastIntervalEndTicks;
        private static List<GHProcessLoad> _lastIntervalRows;
        private static float _lastIntervalOtherGpuPct = float.NaN;

        /* The PreferenceKey preference, default true. When false, Acquire does nothing
           and BuildReport returns null. */
        public static bool Enabled
        {
            get
            {
                try
                {
                    return Preferences.Get(PreferenceKey, true);
                }
                catch
                {
                    return true;
                }
            }
        }

        public static bool IsRunning
        {
            get
            {
                lock (_lifetimeLock)
                {
                    return _stopEvent != null;
                }
            }
        }

        /* True once any sample has carried a whole-machine CPU value. */
        public static bool HasCpuSignal
        {
            get { return Interlocked.CompareExchange(ref _hasCpuSignal, 0, 0) != 0; }
        }

        /* Starts the sampler thread on the first outstanding acquire. */
        public static void Acquire()
        {
            try
            {
                if (!Enabled)
                    return;
                lock (_lifetimeLock)
                {
                    _refCount++;
                    if (_refCount == 1)
                        StartThread();
                }
            }
            catch { }
        }

        /* Stops the sampler thread when the last acquire is released; a later Acquire
           starts a new one. Extra releases are ignored. */
        public static void Release()
        {
            try
            {
                lock (_lifetimeLock)
                {
                    if (_refCount <= 0)
                        return;
                    _refCount--;
                    if (_refCount == 0 && _stopEvent != null)
                    {
                        ManualResetEvent stop = _stopEvent;
                        _stopEvent = null;
                        try
                        {
                            stop.Set();
                        }
                        catch { }
                    }
                }
            }
            catch { }
        }

        /* Caller holds _lifetimeLock. Each thread owns its stop event, so a thread that is
           still finishing a tick after Release cannot be confused with its successor. */
        private static void StartThread()
        {
            ManualResetEvent stop = new ManualResetEvent(false);
            Thread thread = new Thread(ThreadMain);
            thread.IsBackground = true;
            thread.Priority = ThreadPriority.Normal;
            thread.Name = "GHSystemLoadSampler";
            try
            {
                thread.Start(stop);
                _stopEvent = stop;
            }
            catch
            {
                _refCount = 0;
                stop.Dispose();
            }
        }

        private static void ThreadMain(object state)
        {
            ManualResetEvent stop = state as ManualResetEvent;
            if (stop == null)
                return;
            try
            {
                long intervalTicks = (long)GHBackgroundLoad.SampleIntervalMs * Stopwatch.Frequency / 1000;
                long startTimestamp = Stopwatch.GetTimestamp();
                long tick = 0;
                bool primed = false;
                while (true)
                {
                    SampleOnce(primed);
                    primed = true;

                    long elapsed = Stopwatch.GetTimestamp() - startTimestamp;
                    long nextTick = elapsed / intervalTicks + 1;
                    if (nextTick <= tick)
                        nextTick = tick + 1;
                    tick = nextTick;
                    long waitTicks = tick * intervalTicks - elapsed;
                    int waitMs = (int)Math.Max(0, Math.Min(int.MaxValue, waitTicks * 1000 / Stopwatch.Frequency));
                    if (stop.WaitOne(waitMs))
                        break;
                }
            }
            catch { }
            finally
            {
                try
                {
                    stop.Dispose();
                }
                catch { }
            }
        }

        private static void SampleOnce(bool record)
        {
            GHSystemLoadSample sample = GHSystemLoadSample.Empty;
            bool ok = false;
            long before = Stopwatch.GetTimestamp();
            try
            {
                IPlatformService service = GHApp.PlatformService;
                if (service != null)
                    ok = service.TryGetSystemLoadSample(ref sample);
            }
            catch
            {
                ok = false;
            }
            long after = Stopwatch.GetTimestamp();
            if (!ok || !record)
                return;

            sample.TimestampTicks = DateTime.UtcNow.Ticks;
            if (!float.IsNaN(sample.SystemCpuPct))
                Interlocked.Exchange(ref _hasCpuSignal, 1);
            float busyMs = (float)((after - before) * 1000.0 / Stopwatch.Frequency);
            lock (_ringLock)
            {
                _ring[_ringNext] = sample;
                _ringBusyMs[_ringNext] = busyMs;
                _ringNext = (_ringNext + 1) % RingCapacity;
                if (_ringCount < RingCapacity)
                    _ringCount++;
            }
        }

        /* Copies, oldest first, the samples whose timestamps are at or after fromTicks;
           busyMs may be null. */
        private static int CopySince(long fromTicks, out GHSystemLoadSample[] samples, out float[] busyMs, bool withBusy)
        {
            lock (_ringLock)
            {
                int oldest = (_ringNext - _ringCount + RingCapacity) % RingCapacity;
                int n = 0;
                for (int i = 0; i < _ringCount; i++)
                {
                    if (_ring[(oldest + i) % RingCapacity].TimestampTicks >= fromTicks)
                        n++;
                }
                samples = new GHSystemLoadSample[n];
                busyMs = withBusy ? new float[n] : null;
                int k = 0;
                for (int i = 0; i < _ringCount && k < n; i++)
                {
                    int index = (oldest + i) % RingCapacity;
                    if (_ring[index].TimestampTicks < fromTicks)
                        continue;
                    samples[k] = _ring[index];
                    if (withBusy)
                        busyMs[k] = _ringBusyMs[index];
                    k++;
                }
                return k;
            }
        }

        /* The quiet-gate test (GHBackgroundLoad.IsQuiet) over the most recent samples. */
        public static bool IsQuiet(out float otherCpuMean, out float diskBusyMean)
        {
            otherCpuMean = float.NaN;
            diskBusyMean = float.NaN;
            try
            {
                long now = DateTime.UtcNow.Ticks;
                GHSystemLoadSample[] samples;
                float[] busy;
                int n = CopySince(now - (GHBackgroundLoad.QuietWindowSeconds + 1) * TimeSpan.TicksPerSecond, out samples, out busy, false);
                return GHBackgroundLoad.IsQuiet(samples, n, now, out otherCpuMean, out diskBusyMean);
            }
            catch
            {
                return false;
            }
        }

        /* Runs the begin collect of a per-process interval on the thread pool and records
           when it happened. The returned task never faults. */
        public static Task StartProcessIntervalAsync()
        {
            try
            {
                if (!Enabled)
                    return Task.FromResult(false);
                lock (_intervalLock)
                {
                    _intervalStartTicks = 0;
                    Task task = Task.Run(new Action(BeginProcessInterval));
                    _intervalBeginTask = task;
                    return task;
                }
            }
            catch
            {
                return Task.FromResult(false);
            }
        }

        private static void BeginProcessInterval()
        {
            try
            {
                IPlatformService service = GHApp.PlatformService;
                if (service == null)
                    return;
                float otherGpuPct;
                bool ok = service.TryCollectProcessInterval(true, null, out otherGpuPct);
                lock (_intervalLock)
                {
                    _intervalStartTicks = ok ? DateTime.UtcNow.Ticks : 0;
                }
            }
            catch { }
        }

        /* Ends the per-process interval that StartProcessIntervalAsync began, waiting up
           to ProcessIntervalBeginWaitMs for its begin collect. rows is never null. False,
           with no rows and a NaN otherGpuPct, when no interval was started or the
           platform has no per-process data. The result is also kept for BuildReport. */
        public static bool EndProcessInterval(out List<GHProcessLoad> rows, out float otherGpuPct)
        {
            rows = new List<GHProcessLoad>();
            otherGpuPct = float.NaN;
            try
            {
                Task beginTask;
                lock (_intervalLock)
                {
                    beginTask = _intervalBeginTask;
                    _intervalBeginTask = null;
                }
                if (beginTask == null)
                    return false;
                try
                {
                    beginTask.Wait(ProcessIntervalBeginWaitMs);
                }
                catch { }

                long startTicks;
                lock (_intervalLock)
                {
                    startTicks = _intervalStartTicks;
                    _intervalStartTicks = 0;
                }
                if (startTicks == 0)
                    return false;

                IPlatformService service = GHApp.PlatformService;
                if (service == null)
                    return false;
                List<GHProcessLoad> collected = new List<GHProcessLoad>();
                float gpu;
                if (!service.TryCollectProcessInterval(false, collected, out gpu))
                    return false;

                lock (_intervalLock)
                {
                    _lastIntervalValid = true;
                    _lastIntervalStartTicks = startTicks;
                    _lastIntervalEndTicks = DateTime.UtcNow.Ticks;
                    _lastIntervalRows = new List<GHProcessLoad>(collected);
                    _lastIntervalOtherGpuPct = gpu;
                }
                rows = collected;
                otherGpuPct = gpu;
                return true;
            }
            catch
            {
                rows = new List<GHProcessLoad>();
                otherGpuPct = float.NaN;
                return false;
            }
        }

        /* The report for the window [windowStartTicks, windowEndTicks), with the processes
           of the last ended process interval when that interval overlaps the window.
           Null when the sampler is disabled or on failure. */
        public static GHBackgroundReport BuildReport(long windowStartTicks, long windowEndTicks)
        {
            try
            {
                List<GHProcessLoad> rows = null;
                float otherGpuPct = float.NaN;
                lock (_intervalLock)
                {
                    if (_lastIntervalValid && _lastIntervalRows != null
                        && _lastIntervalStartTicks < windowEndTicks && _lastIntervalEndTicks > windowStartTicks)
                    {
                        rows = new List<GHProcessLoad>(_lastIntervalRows);
                        otherGpuPct = _lastIntervalOtherGpuPct;
                    }
                }
                return BuildReport(windowStartTicks, windowEndTicks, rows, otherGpuPct);
            }
            catch
            {
                return null;
            }
        }

        /* The report for the window [windowStartTicks, windowEndTicks) with the given
           per-process result; processes null means the interval produced no data. The
           pre-window covers the PreWindowSeconds before the window start. Null when the
           sampler is disabled or on failure. */
        public static GHBackgroundReport BuildReport(long windowStartTicks, long windowEndTicks,
                                                     IList<GHProcessLoad> processes, float otherGpuPct)
        {
            try
            {
                if (!Enabled)
                    return null;
                long preStartTicks = windowStartTicks - PreWindowSeconds * TimeSpan.TicksPerSecond;
                GHSystemLoadSample[] samples;
                float[] busyMs;
                int n = CopySince(preStartTicks, out samples, out busyMs, true);

                GHBackgroundReport r = new GHBackgroundReport();
                r.LogicalProcessors = Environment.ProcessorCount;
                r.Window = GHBackgroundLoad.Summarize(samples, n, windowStartTicks, windowEndTicks, GHBackgroundLoad.SampleIntervalMs);
                r.PreWindow = GHBackgroundLoad.Summarize(samples, n, preStartTicks, windowStartTicks, GHBackgroundLoad.SampleIntervalMs);
                r.Coverage = r.Window.Coverage;

                bool hasSystemCpu = false, hasDisk = false, hasMemory = false, hasFaults = false;
                double busySum = 0;
                for (int i = 0; i < n; i++)
                {
                    GHSystemLoadSample x = samples[i];
                    if (x.TimestampTicks < preStartTicks || x.TimestampTicks >= windowEndTicks)
                        continue;
                    GHBackgroundReportSample s = new GHBackgroundReportSample();
                    s.T = (float)((double)(x.TimestampTicks - windowStartTicks) / TimeSpan.TicksPerSecond);
                    s.Sys = x.SystemCpuPct;
                    s.Own = x.OwnCpuPct;
                    s.Other = x.OtherCpuPct;
                    s.Disk = x.DiskBusyPct;
                    s.AvailPct = x.AvailableMemoryPct;
                    s.Faults = x.HardFaultsPerSec;
                    r.Samples.Add(s);

                    if (x.TimestampTicks < windowStartTicks)
                        continue;
                    busySum += busyMs[i];
                    if (!float.IsNaN(x.SystemCpuPct))
                        hasSystemCpu = true;
                    if (!float.IsNaN(x.DiskBusyPct))
                        hasDisk = true;
                    if (!float.IsNaN(x.AvailableMemoryPct) || x.AvailableMemoryMB >= 0)
                        hasMemory = true;
                    if (!float.IsNaN(x.HardFaultsPerSec))
                        hasFaults = true;
                }
                r.Samples.Sort(CompareSamples);
                r.SamplerBusyMs = (float)busySum;

                if (processes != null)
                {
                    for (int i = 0; i < processes.Count; i++)
                        r.Processes.Add(processes[i]);
                    r.OtherGpuPct = otherGpuPct;
                }

                if (hasSystemCpu)
                    r.Signals.Add(GHBackgroundReport.SignalSystemCpu);
                if (hasDisk)
                    r.Signals.Add(GHBackgroundReport.SignalDisk);
                if (hasMemory)
                    r.Signals.Add(GHBackgroundReport.SignalMemory);
                if (hasFaults)
                    r.Signals.Add(GHBackgroundReport.SignalHardFaults);
                if (processes != null)
                    r.Signals.Add(GHBackgroundReport.SignalProcesses);
                if (processes != null && !float.IsNaN(r.OtherGpuPct))
                    r.Signals.Add(GHBackgroundReport.SignalGpu);

                r.Activities = GHBackgroundLoad.BuildActivities(r.Processes);
                string reason;
                r.Verdict = GHBackgroundLoad.Classify(r.Window, r.Processes, r.OtherGpuPct, out reason);
                r.Reason = reason;
                return r;
            }
            catch
            {
                return null;
            }
        }

        private static int CompareSamples(GHBackgroundReportSample x, GHBackgroundReportSample y)
        {
            return x.T.CompareTo(y.T);
        }
    }
}

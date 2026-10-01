using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX.Performance
{
    /* One measurement window, in the shape DEVEL/performance/schema/run-record.schema.json
       (schema v2) describes and the offline analyzer reads: a scenario and arm label, the
       environment the run was measured in, thermal readings taken before and after, the
       smoothness summary and on-screen pacing metrics GHSmoothnessMetrics computes, the
       raw on-screen interval and pacing-error series, the UI thread latency probe, and
       four optional parts: a "suite" object for a run that is part of a suite, a
       "marks" array of the frames the user marked in the saved range, a "background"
       block of the machine's load around the window (GHSystemLoadSampler), and a
       "timeline" block of how many of the requested ticks the frame timeline held.

       TryBeginWindow and EndWindowAndSave bracket a measurement: TryBeginWindow remembers
       whether the frame timeline was already running, enables it, notes the first frame of
       the window, takes the first thermal reading and, on Windows, starts the render
       adapter probe; EndWindowAndSave takes the second reading, ends the probe, saves the
       window's frames (the JSON and the two CSV dumps), and restores the timeline to
       whatever state it was in before TryBeginWindow. DiscardWindow ends a window without
       saving it. TryBeginWindow names the window's owner (GHWindowOwner) and refuses
       when GHWindowGate does, so one measurement never replaces another's window.
       TryBeginWindow takes a GHPerformanceRunContext, and EndWindowAndSave has an
       overload that hands back a GHPerformanceRunResult, for a suite runner; the plain
       overload discards the result. SaveRecent writes the same document from everything
       the timeline still holds, with no window. BuildRecentHitchesReport renders the last
       seconds the timeline holds as plain text instead, saving nothing. None of these
       throws.

       Window commands let a script bracket a window without touching the device: while
       the timeline is enabled the app polls ExportDirectory for window.cmd, and on Android
       also receives them as a broadcast (PresentFeedbackAndroid). HandleCommand describes
       the syntax. After a commanded window is saved, window.done in the same directory
       names the record's JSON file; a begin the window gate refuses writes window.refused
       there instead, naming the reason.

       Must compile under C# 7.3 (the legacy netstandard2.0 project). */
    /* A performance suite's run-level context, folded into a saved record's "suite"
       object and consulted for the run result's ExcludedReason. TurnReached,
       InputRecordsInWindow and ExcludedReason are meant to be set by the caller on the
       same instance passed to TryBeginWindow, any time before EndWindowAndSave, and
       TurnAtWindowStart before TryBeginWindow: ExcludedReason preset here (e.g.
       "warm-up run", "replay ended") always wins over the reasons EndWindowAndSave
       derives on its own (throttling, a power state change, too few on-screen
       intervals). */
    public sealed class GHPerformanceRunContext
    {
        public string SuiteId;
        public int RunIndex;
        public string PageMode;        /* shared, fresh */
        public bool IsWarmUp;
        public string ReplayFileName;
        public long ReplayBytes;
        public string ReplaySha256;
        public int StartTurn;
        public int TurnReached = -1;   /* set by the caller before EndWindowAndSave */
        public int TurnAtWindowStart = -1;         /* replay turn when the window opened; -1 when unknown */
        public long InputRecordsInWindow = -1;     /* replay input records played during the window; -1 when unknown */
        public string ExcludedReason;  /* preset by the caller, e.g. "warm-up run", "replay ended" */
        public string Notes;           /* e.g. a quiet gate timeout before the run; the record's "suite.notes" */
    }

    /* What EndWindowAndSave(directory, out result) hands back alongside the JSON path,
       so a suite runner does not need to re-read or re-parse the file it just wrote. */
    public sealed class GHPerformanceRunResult
    {
        public string JsonPath;
        public GHSmoothnessSummary Summary;
        public float[] OnScreenIntervalsMs;
        public GHThermalReading ThermalBefore;
        public GHThermalReading ThermalAfter;
        public int OnScreenIntervalCount;
        public string ExcludedReason;  /* null when the run is usable */
        public GHBackgroundReport Background;  /* null when the sampler is off or failed */
    }

    public static class GHPerformanceRunRecord
    {
        public const int CurrentSchemaVersion = 2;

        public const string CommandFileName = "window.cmd";
        public const string DoneFileName = "window.done";
        public const string RefusedFileName = "window.refused";
        private const int CommandPollMs = 500;

        private static int _windowOpen = 0;
        private static GHWindowOwner _windowOwner = GHWindowOwner.None;   /* main thread only */
        private static string _scenario = "";
        private static string _arm = "";
        private static bool _wasTimelineEnabledBeforeWindow;
        private static GHThermalReading _thermalBefore;
        private static DateTime _startedUtc;
        private static long _windowFromFrameId = 1;
        private static GHPerformanceRunContext _context = null;
        private static long _windowStartTicks;         /* DateTime.UtcNow.Ticks at TryBeginWindow */
        private static int _windowSamplerHeld = 0;     /* 1 while the window's sampler acquire is outstanding */

#if GNH_MAUI && WINDOWS
        /* The render adapter probe (GHRenderAdapterProbeWindows). Every probe call runs on
           the thread pool, queued after the previous one so a late End never closes a
           newer window's query. _adapterProbeBegin is the open window's Begin, null when
           none is outstanding. Both fields are guarded by _adapterProbeLock. */
        private const int AdapterProbeBeginWaitMs = 1000;
        private const int AdapterProbeEndWaitMs = 2000;
        private static readonly object _adapterProbeLock = new object();
        private static Task _adapterProbeTail = null;
        private static Task<bool> _adapterProbeBegin = null;
#endif

        /* The reading taken when the timeline was last enabled: SaveRecent's "before" */
        private static GHThermalReading _thermalAtActivation;
        private static bool _hasThermalAtActivation = false;

        /* Window command state. The poll timer runs on the thread pool and only posts; the
           window timer, also on the thread pool, reads _windowPhase and _windowGeneration
           before it posts. Every other use of the members below is on the main thread. */
        private static Timer _pollTimer = null;
        private static string _commandDirectory = null;
        private static Timer _windowTimer = null;
        private static int _windowGeneration = 0;
        private static int _windowPhase = 0;            /* 0 none, 1 waiting to begin, 2 open */
        private static string _scheduledScenario = "";
        private static string _scheduledArm = "";
        private static double _scheduledWindowSeconds = 0;

        private static readonly JsonSerializerSettings _jsonSettings = new JsonSerializerSettings
        {
            Formatting = Formatting.Indented,
            Culture = CultureInfo.InvariantCulture,
            NullValueHandling = NullValueHandling.Include
        };

        public static bool IsWindowOpen { get { return Interlocked.CompareExchange(ref _windowOpen, 0, 0) != 0; } }

        /* True while a window command is scheduled and its window has not begun */
        public static bool IsWindowCommandPending { get { return Volatile.Read(ref _windowPhase) == 1; } }

        private static bool IsSuiteOrDiagnosticRunning
        {
            get { return GHPerformanceSuiteRunner.IsRunning || GHPerformanceDiagnosticRunner.IsRunning; }
        }

        public static string CurrentScenario { get { return _scenario; } }

        /* The archive directory's performance subdirectory, created when missing */
        public static string DefaultDirectory
        {
            get
            {
                string ghdir = GHApp.GnollHackService.GetGnollHackPath();
                string archive = Path.Combine(ghdir, GHConstants.ArchiveDirectory);
                if (!Directory.Exists(archive))
                    GHApp.CheckCreateDirectory(archive);
                string performance = Path.Combine(archive, "performance");
                if (!Directory.Exists(performance))
                    GHApp.CheckCreateDirectory(performance);
                return performance;
            }
        }

        /* Where commanded windows are saved and window.cmd is looked for: on Android the
           app's external files directory, which adb can read, else DefaultDirectory */
        public static string ExportDirectory
        {
            get
            {
#if GNH_MAUI && ANDROID
                try
                {
                    Java.IO.File external = Android.App.Application.Context.GetExternalFilesDir(null);
                    if (external != null)
                    {
                        string performance = Path.Combine(external.AbsolutePath, "performance");
                        if (!Directory.Exists(performance))
                            Directory.CreateDirectory(performance);
                        return performance;
                    }
                }
                catch
                {
                    /* Fall back to internal storage */
                }
#endif
                return DefaultDirectory;
            }
        }

        /* Why a window for owner may not begin now (GHWindowGate.BeginRefusal over the
           current state), or null when it may. Call on the UI thread. */
        public static string BeginRefusal(GHWindowOwner owner)
        {
            return GHWindowGate.BeginRefusal(owner, IsWindowOpen, IsWindowCommandPending, IsSuiteOrDiagnosticRunning);
        }

        /* Starts a measurement window owned by owner, unless the window gate refuses it
           (BeginRefusal): holds the background load sampler until the window ends,
           remembers whether the frame timeline was already enabled, enables it, notes the
           window's first frame, restarts the UI thread probe's statistics, reads the
           thermal state, notes the start time and, once the window is open, starts the
           Windows render adapter probe's Begin on the thread pool without waiting for it.
           It never collects the per-process interval: callers start that on the thread
           pool before calling this (GHSystemLoadSampler.StartProcessIntervalAsync).
           context, null when there is none, is remembered by reference: fields the caller
           sets on it up to EndWindowAndSave (TurnReached, InputRecordsInWindow,
           ExcludedReason) are included in
           the saved record's "suite" object and in the run result's ExcludedReason.
           Call on the UI thread. False when refused or when the window could not be
           opened; never throws. */
        public static bool TryBeginWindow(string scenario, string arm, GHPerformanceRunContext context,
            GHWindowOwner owner)
        {
            if (BeginRefusal(owner) != null)
                return false;
            try
            {
                AcquireWindowSampler();
                _scenario = scenario ?? "";
                _arm = arm ?? "";
                _context = context;
                _wasTimelineEnabledBeforeWindow = GHFrameTimeline.IsEnabled;
                FrameTimeProfiler.IsEnabled = true;
                _windowFromFrameId = GHFrameTimeline.LastFrameId + 1;
                GHUiThreadProbe.Reset();
                GHUiThreadProbe.BeginWindow();
                _thermalBefore = GHThermalProbe.Read();
                _startedUtc = DateTime.UtcNow;
                _windowStartTicks = _startedUtc.Ticks;
                _windowOwner = owner;
                Interlocked.Exchange(ref _windowOpen, 1);
                StartAdapterProbe();
                return true;
            }
            catch
            {
                _context = null;
                _windowOwner = GHWindowOwner.None;
                Interlocked.Exchange(ref _windowOpen, 0);
                ReleaseWindowSampler();
                AbandonAdapterProbe();
                return false;
            }
        }

        /* Queues the render adapter probe's Begin for the window that just opened; a
           Begin still outstanding is superseded, since Begin restarts the interval.
           Windows only; never throws. */
        private static void StartAdapterProbe()
        {
#if GNH_MAUI && WINDOWS
            try
            {
                lock (_adapterProbeLock)
                {
                    _adapterProbeBegin = QueueAdapterProbeLocked(delegate { return GHRenderAdapterProbeWindows.Begin(); });
                }
            }
            catch
            {
                /* The window works without the probe */
            }
#endif
        }

        /* Ends the window's render adapter probe, if one was begun, and waits at most
           AdapterProbeBeginWaitMs for its Begin and then AdapterProbeEndWaitMs for its End,
           so the probe's last result (GHRenderAdapterProbeWindows.TryGetLast) is current
           when the record is built. The result itself is not used here. Windows only;
           never throws. */
        private static void EndAdapterProbe()
        {
#if GNH_MAUI && WINDOWS
            try
            {
                Task<bool> begin;
                Task<bool> end;
                lock (_adapterProbeLock)
                {
                    begin = _adapterProbeBegin;
                    _adapterProbeBegin = null;
                    if (begin == null)
                        return;
                    end = QueueAdapterProbeLocked(EndAdapterProbeStep);
                }
                begin.Wait(AdapterProbeBeginWaitMs);
                end.Wait(AdapterProbeEndWaitMs);
            }
            catch
            {
                /* The record is saved without a fresh probe result */
            }
#endif
        }

        /* Ends the probe of a window that closes without reaching EndAdapterProbe
           (discarded, lost to a failed TryBeginWindow, or a save that threw first), without
           waiting, so its PDH query closes. Windows only; never throws. */
        private static void AbandonAdapterProbe()
        {
#if GNH_MAUI && WINDOWS
            try
            {
                lock (_adapterProbeLock)
                {
                    if (_adapterProbeBegin == null)
                        return;
                    _adapterProbeBegin = null;
                    QueueAdapterProbeLocked(EndAdapterProbeStep);
                }
            }
            catch
            {
            }
#endif
        }

#if GNH_MAUI && WINDOWS
        private static bool EndAdapterProbeStep()
        {
            GHRenderAdapter render;
            return GHRenderAdapterProbeWindows.End(out render, null);
        }

        /* Runs step on the thread pool after every probe call queued before it, whatever
           their outcome. Call under _adapterProbeLock. */
        private static Task<bool> QueueAdapterProbeLocked(Func<bool> step)
        {
            Task<bool> next;
            if (_adapterProbeTail == null)
                next = Task.Run(step);
            else
                next = _adapterProbeTail.ContinueWith(delegate (Task previous) { return step(); }, TaskScheduler.Default);
            _adapterProbeTail = next;
            return next;
        }
#endif

        /* At most one sampler acquire is outstanding for the window, whatever the
           sequence of TryBeginWindow, EndWindowAndSave and cancellations; none is taken
           while the sampler is disabled, since Acquire then counts nothing */
        private static void AcquireWindowSampler()
        {
            if (!GHSystemLoadSampler.Enabled)
                return;
            if (Interlocked.Exchange(ref _windowSamplerHeld, 1) == 0)
                GHSystemLoadSampler.Acquire();
        }

        private static void ReleaseWindowSampler()
        {
            if (Interlocked.Exchange(ref _windowSamplerHeld, 0) == 1)
                GHSystemLoadSampler.Release();
        }

        /* Ends the window, writes run_<stamp>_<scenario>_<arm>.json plus the frame
           timeline and compositor frame CSVs of the window's frames into directory (created
           if missing), restores the timeline's previous enabled state, and returns the JSON
           path, or null on any failure. */
        public static string EndWindowAndSave(string directory)
        {
            GHPerformanceRunResult result;
            return EndWindowAndSave(directory, out result);
        }

        /* Same as EndWindowAndSave(directory), but also hands back the computed
           smoothness summary, the on-screen interval series, both thermal readings and
           the interval count, so a suite runner does not need to re-read or re-parse the
           JSON it just wrote. result is non-null whenever a JSON was written; on failure
           result may be null and the method returns null, as EndWindowAndSave(directory)
           always did. Once the window has closed, the per-process interval is ended, the
           window's background report built from the sampler, and on Windows the render
           adapter probe ended before the record is built, waiting at most 3 s in all; the
           sampler hold taken by TryBeginWindow is released and the probe ended whatever
           happens. */
        public static string EndWindowAndSave(string directory, out GHPerformanceRunResult result)
        {
            result = null;
            if (Interlocked.CompareExchange(ref _windowOpen, 0, 1) != 1)
                return null;
            GHPerformanceRunContext context = _context;
            _context = null;
            _windowOwner = GHWindowOwner.None;
            try
            {
                long windowEndTicks = DateTime.UtcNow.Ticks;
                GHThermalReading thermalAfter = GHThermalProbe.Read();
                long toFrameId = GHFrameTimeline.LastFrameId;
                GHBackgroundReport background = BuildWindowBackground(_windowStartTicks, windowEndTicks);
                EndAdapterProbe();
                return Save(directory, _scenario, _arm, _startedUtc, _thermalBefore, thermalAfter,
                    _windowFromFrameId, toFrameId, context, background, false, out result);
            }
            catch
            {
                result = null;
                return null;
            }
            finally
            {
                RestoreTimeline();
                ReleaseWindowSampler();
                AbandonAdapterProbe();
            }
        }

        /* Ends the window without writing anything: restores the timeline's previous enabled
           state, releases the window's sampler hold and ends the render adapter probe
           without waiting. False when no window was open. */
        public static bool DiscardWindow()
        {
            if (Interlocked.CompareExchange(ref _windowOpen, 0, 1) != 1)
                return false;
            _context = null;
            _windowOwner = GHWindowOwner.None;
            try
            {
                RestoreTimeline();
            }
            finally
            {
                ReleaseWindowSampler();
                AbandonAdapterProbe();
            }
            return true;
        }

        /* Ends the per-process interval and builds the report for the window; null when
           the sampler is disabled or fails */
        private static GHBackgroundReport BuildWindowBackground(long windowStartTicks, long windowEndTicks)
        {
            try
            {
                List<GHProcessLoad> rows;
                float otherGpuPct;
                bool haveRows = GHSystemLoadSampler.EndProcessInterval(out rows, out otherGpuPct);
                return GHSystemLoadSampler.BuildReport(windowStartTicks, windowEndTicks,
                    haveRows ? rows : null, haveRows ? otherGpuPct : float.NaN);
            }
            catch
            {
                return null;
            }
        }

        private static void RestoreTimeline()
        {
            try
            {
                if (!_wasTimelineEnabledBeforeWindow)
                    FrameTimeProfiler.IsEnabled = false;
            }
            catch { }
        }

        /* Saves what the timeline already holds, up to its last Capacity ticks, without
           opening a window: for capturing a stutter just after it was seen. The window start
           is derived from the retained ticks; the "before" reading is the one taken when the
           timeline was enabled, whose timestamp the record carries. Returns the JSON path, or
           null when the timeline is off or holds nothing; never throws. */
        public static string SaveRecent(string directory, string scenario, string arm)
        {
            try
            {
                if (!GHFrameTimeline.IsEnabled)
                    return null;
                long last = GHFrameTimeline.LastFrameId;
                if (last <= 0)
                    return null;
                /* Frame ids are contiguous, so the oldest retained id is known without
                   scanning for it; only its record and the last record's are needed for
                   the span, not a copy of the whole ring. The span starts
                   RecentSaveMarginTicks later, since the ring may overwrite those ticks
                   while it copies. */
                long first = Math.Max(1, last - GHFrameTimeline.Capacity + 1 + GHFrameTimeline.RecentSaveMarginTicks);
                GHFrameRecord[] endpoint = new GHFrameRecord[1];
                long firstTicks = 0, lastTicks = 0;
                bool haveEndpoints = GHFrameTimeline.CopyRecords(endpoint, first, first) == 1;
                if (haveEndpoints)
                {
                    firstTicks = endpoint[0].CallbackStartTicks;
                    haveEndpoints = GHFrameTimeline.CopyRecords(endpoint, last, last) == 1;
                    lastTicks = endpoint[0].CallbackStartTicks;
                }
                if (!haveEndpoints)
                {
                    /* An endpoint slot did not hold its id; take the span from every
                       retained record instead */
                    GHFrameRecord[] probe = new GHFrameRecord[GHFrameTimeline.Capacity];
                    int n = GHFrameTimeline.CopyRecords(probe, 1, last);
                    if (n == 0)
                        return null;
                    firstTicks = probe[0].CallbackStartTicks;
                    lastTicks = probe[n - 1].CallbackStartTicks;
                }
                double spanSeconds = (double)(lastTicks - firstTicks) / Stopwatch.Frequency;
                DateTime startedUtc = DateTime.UtcNow - TimeSpan.FromSeconds(Math.Max(0, spanSeconds));
                GHThermalReading now = GHThermalProbe.Read();
                GHThermalReading before = _hasThermalAtActivation ? _thermalAtActivation : now;
                /* Whatever the sampler's ring holds over the span, without processes */
                GHBackgroundReport background = null;
                if (GHSystemLoadSampler.IsRunning)
                    background = GHSystemLoadSampler.BuildReport(startedUtc.Ticks, DateTime.UtcNow.Ticks, null, float.NaN);
                return Save(directory, scenario ?? "", arm ?? "", startedUtc, before, now, first, last, background);
            }
            catch
            {
                return null;
            }
        }

        /* The SaveRecent path: no context, result discarded, long stalls excluded as a
           retrospective report does. */
        private static string Save(string directory, string scenario, string arm, DateTime startedUtc,
            GHThermalReading thermalBefore, GHThermalReading thermalAfter, long fromFrameId, long toFrameId,
            GHBackgroundReport background)
        {
            GHPerformanceRunResult result;
            return Save(directory, scenario, arm, startedUtc, thermalBefore, thermalAfter, fromFrameId, toFrameId,
                null, background, true, out result);
        }

        /* Builds the summary and both CSVs from one copy of the ring so they agree, writes
           the JSON, and hands back the pieces a suite runner needs (result) alongside the
           JSON path. context is folded into the JSON's "suite" object and into
           result.ExcludedReason; both are null when the caller passes no context.
           background, null when there is none, becomes the JSON's "background" block.
           excludeLongStalls is passed to GHSmoothnessMetrics.Analyze: false for a
           measurement window, true for a retrospective save. The JSON's "timeline" block
           counts the requested ticks the ring had already overwritten. */
        private static string Save(string directory, string scenario, string arm, DateTime startedUtc,
            GHThermalReading thermalBefore, GHThermalReading thermalAfter, long fromFrameId, long toFrameId,
            GHPerformanceRunContext context, GHBackgroundReport background, bool excludeLongStalls,
            out GHPerformanceRunResult result)
        {
            result = null;
            DateTime endedUtc = DateTime.UtcNow;

            /* One copy of the ring feeds the summary and both CSVs, so they agree */
            GHFrameRecord[] recordBuffer = new GHFrameRecord[GHFrameTimeline.Capacity];
            long missingTicks;
            int n = GHFrameTimeline.CopyRecords(recordBuffer, fromFrameId, toFrameId, out missingTicks);
            TimelineJson timeline = BuildTimeline(fromFrameId, toFrameId, missingTicks);
            long originTicks = n > 0 ? recordBuffer[0].CallbackStartTicks : 0;
            GHCompositorFrame[] compositorBuffer = new GHCompositorFrame[GHFrameTimeline.Capacity];
            int m = CopyCompositorFramesFor(recordBuffer, n, compositorBuffer);
            GHDisplayedFrame[] displayed = new GHDisplayedFrame[n];
            int displayedCount;
            GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(recordBuffer, n, compositorBuffer, m,
                displayed, out displayedCount, excludeLongStalls);

            if (!Directory.Exists(directory))
                Directory.CreateDirectory(directory);

            string stamp = endedUtc.ToString("yyyyMMdd_HHmmss_fff", CultureInfo.InvariantCulture);
            string stem = "run_" + stamp + "_" + SafeFileToken(scenario) + "_" + SafeFileToken(arm);
            string timelineCsvName = "frametimeline_" + stem + ".csv";
            string compositorCsvName = "compositorframes_" + stem + ".csv";
            string jsonPath = Path.Combine(directory, stem + ".json");

            GHFrameTimeline.WriteCsv(Path.Combine(directory, timelineCsvName), recordBuffer, n);
            GHFrameTimeline.WriteCompositorCsv(Path.Combine(directory, compositorCsvName), compositorBuffer, m, originTicks);

            SeriesJson series = BuildSeries(displayed, displayedCount);
            object doc = BuildDocument(stem, scenario, arm, startedUtc, endedUtc, thermalBefore, thermalAfter,
                summary, series, timelineCsvName, compositorCsvName, context, BuildMarks(recordBuffer, n), background,
                timeline);

            string json = JsonConvert.SerializeObject(doc, _jsonSettings);
            GHAtomicFile.WriteAllText(jsonPath, json + Environment.NewLine, null);

            result = new GHPerformanceRunResult();
            result.JsonPath = jsonPath;
            result.Summary = summary;
            result.OnScreenIntervalsMs = ToFloatArray(series.OnScreenIntervalsMs);
            result.OnScreenIntervalCount = result.OnScreenIntervalsMs.Length;
            result.ThermalBefore = thermalBefore;
            result.ThermalAfter = thermalAfter;
            result.Background = background;
            result.ExcludedReason = ComputeExcludedReason(context, thermalBefore, thermalAfter, result.OnScreenIntervalCount,
                background, timeline);
            return jsonPath;
        }

        /* The "timeline" block: the ticks requested from fromFrameId to toFrameId, capped
           at the latest tick, and how many of them the ring still held */
        private static TimelineJson BuildTimeline(long fromFrameId, long toFrameId, long missingTicks)
        {
            long last = Math.Min(GHFrameTimeline.LastFrameId, toFrameId);
            long requested = Math.Max(0, last - Math.Max(1, fromFrameId) + 1);
            long missing = Math.Min(Math.Max(0, missingTicks), requested);
            TimelineJson j = new TimelineJson();
            j.RequestedTicks = requested;
            j.RetainedTicks = requested - missing;
            j.Truncated = missing > 0;
            return j;
        }

        /* Compositor frames within a quarter second of the records' callback starts, so
           the frames around both ends are joined too; returns the count copied */
        private static int CopyCompositorFramesFor(GHFrameRecord[] records, int n, GHCompositorFrame[] destination)
        {
            if (n <= 0)
                return 0;
            long margin = Stopwatch.Frequency / 4;
            return GHFrameTimeline.CopyCompositorFrames(destination, records[0].CallbackStartTicks - margin,
                records[n - 1].CallbackStartTicks + margin);
        }

        /* The user marks whose frame is among records[0..n), oldest first, with their
           frame ids and UTC times (DateTime ticks); returns the count */
        private static int CopyMarksIn(GHFrameRecord[] records, int n, long[] frameIds, long[] utcTicks)
        {
            if (n <= 0)
                return 0;
            long first = records[0].FrameId;
            long last = records[n - 1].FrameId;
            int total = GHFrameTimeline.CopyMarks(frameIds, utcTicks);
            int kept = 0;
            for (int i = 0; i < total; i++)
            {
                if (frameIds[i] < first || frameIds[i] > last)
                    continue;
                frameIds[kept] = frameIds[i];
                utcTicks[kept] = utcTicks[i];
                kept++;
            }
            return kept;
        }

        /* The saved window's marks for the JSON, null when there are none. A mark whose
           record is missing from the copy (a torn read) is left out. */
        private static MarkJson[] BuildMarks(GHFrameRecord[] records, int n)
        {
            long[] frameIds = new long[GHFrameTimeline.MaxMarks];
            long[] utcTicks = new long[GHFrameTimeline.MaxMarks];
            int count = CopyMarksIn(records, n, frameIds, utcTicks);
            if (count == 0)
                return null;
            long originTicks = records[0].CallbackStartTicks;
            List<MarkJson> marks = new List<MarkJson>();
            for (int i = 0; i < count; i++)
            {
                int idx = FindRecord(records, n, frameIds[i]);
                if (idx < 0)
                    continue;
                MarkJson j = new MarkJson();
                j.FrameId = frameIds[i];
                j.Utc = new DateTime(utcTicks[i], DateTimeKind.Utc).ToString("o", CultureInfo.InvariantCulture);
                j.MsFromWindowStart = R((records[idx].CallbackStartTicks - originTicks) * 1000.0 / Stopwatch.Frequency);
                marks.Add(j);
            }
            return marks.Count > 0 ? marks.ToArray() : null;
        }

        /* Index of the record with frameId, -1 when absent; records are in FrameId order */
        private static int FindRecord(GHFrameRecord[] records, int n, long frameId)
        {
            int lo = 0, hi = n - 1;
            while (lo <= hi)
            {
                int mid = (lo + hi) / 2;
                long id = records[mid].FrameId;
                if (id == frameId)
                    return mid;
                if (id < frameId)
                    lo = mid + 1;
                else
                    hi = mid - 1;
            }
            return -1;
        }

        /* A plain-text report of the hitches in the last `seconds` up to toFrameId (every
           retained tick when seconds is not positive), with the user marks in that range:
           see GHPerformanceTextReport.RecentHitchesReport. Returns null when the timeline
           is off or holds nothing up to toFrameId; never throws. */
        public static string BuildRecentHitchesReport(long toFrameId, double seconds)
        {
            try
            {
                if (!GHFrameTimeline.IsEnabled)
                    return null;
                GHFrameRecord[] all = new GHFrameRecord[GHFrameTimeline.Capacity];
                int total = GHFrameTimeline.CopyRecords(all, 1, toFrameId);
                if (total == 0)
                    return null;

                int first = 0;
                if (seconds > 0)
                {
                    double fromTicks = all[total - 1].CallbackStartTicks - seconds * Stopwatch.Frequency;
                    while (first < total - 1 && all[first].CallbackStartTicks < fromTicks)
                        first++;
                }
                int n = total - first;
                GHFrameRecord[] records = new GHFrameRecord[n];
                Array.Copy(all, first, records, 0, n);

                GHCompositorFrame[] compositor = new GHCompositorFrame[GHFrameTimeline.Capacity];
                int m = CopyCompositorFramesFor(records, n, compositor);
                GHDisplayedFrame[] displayed = new GHDisplayedFrame[n];
                int displayedCount;
                GHSmoothnessSummary summary = GHSmoothnessMetrics.Analyze(records, n, compositor, m,
                    displayed, out displayedCount, true);

                long[] markFrameIds = new long[GHFrameTimeline.MaxMarks];
                long[] markUtcTicks = new long[GHFrameTimeline.MaxMarks];
                int markCount = CopyMarksIn(records, n, markFrameIds, markUtcTicks);
                return GHPerformanceTextReport.RecentHitchesReport(records, n, displayed, displayedCount, summary,
                    markFrameIds, markUtcTicks, markCount);
            }
            catch
            {
                return null;
            }
        }

        private static float[] ToFloatArray(double[] values)
        {
            if (values == null)
                return new float[0];
            float[] result = new float[values.Length];
            for (int i = 0; i < values.Length; i++)
                result[i] = (float)values[i];
            return result;
        }

        /* The exclusion verdict for a saved run, first match wins: the context's own
           preset reason; a window longer than the frame timeline held; throttling, from
           the thermal status ranks and the CPU performance percent
           GHPerformanceComparison.ClassifyThrottle already weighs; a power state change
           between the two readings; a busy background verdict, as its reason
           ("background load: ..."); too few on-screen intervals to be meaningful.
           Returns null when none apply, including when context is null. */
        private static string ComputeExcludedReason(GHPerformanceRunContext context, GHThermalReading before,
            GHThermalReading after, int onScreenIntervalCount, GHBackgroundReport background, TimelineJson timeline)
        {
            if (context != null && !string.IsNullOrEmpty(context.ExcludedReason))
                return context.ExcludedReason;
            if (timeline != null && timeline.Truncated)
                return "window longer than the frame timeline holds (kept "
                    + timeline.RetainedTicks.ToString(CultureInfo.InvariantCulture) + " of "
                    + timeline.RequestedTicks.ToString(CultureInfo.InvariantCulture) + " ticks)";
            try
            {
                var throttle = GHPerformanceComparison.ClassifyThrottle((int)before.Status, (int)after.Status,
                    before.CpuPerformancePct, after.CpuPerformancePct);
                if (throttle.Throttled)
                    return "throttled";
            }
            catch { }
            if (before.PowerStateKnown && after.PowerStateKnown && before.IsCharging != after.IsCharging)
                return "power state changed";
            if (background != null && background.Verdict == GHBackgroundVerdict.Busy)
                return !string.IsNullOrEmpty(background.Reason)
                    ? background.Reason : GHBackgroundLoad.ReasonPrefix + GHBackgroundLoad.VerdictBusyName;
            if (onScreenIntervalCount < 100)
                return "fewer than 100 on-screen intervals";
            return null;
        }

        /* ---- Window commands ---- */

        /* Called on the main thread whenever the frame timeline starts or stops recording:
           takes the reading SaveRecent uses as "before" (which also primes rate counters
           such as the Windows processor performance counter), warms the environment
           fingerprint cache on the thread pool, since its first capture can block, and runs
           the window command poller exactly while the timeline records. Stopping cancels a
           commanded window that has not begun or not ended. */
        public static void OnTimelineActivated(bool active)
        {
            try
            {
                if (active)
                {
                    _thermalAtActivation = GHThermalProbe.Read();
                    _hasThermalAtActivation = true;
                    Task.Run(delegate { GHPerformanceEnvironment.CaptureFingerprint(false); });
                    _commandDirectory = ExportDirectory;
                    if (_pollTimer == null)
                        _pollTimer = new Timer(PollCommandFile, null, CommandPollMs, CommandPollMs);
                }
                else
                {
                    Timer poll = _pollTimer;
                    _pollTimer = null;
                    if (poll != null)
                        poll.Dispose();
                    CancelWindowCommand();
                }
            }
            catch
            {
                /* The poller is a convenience; the timeline works without it */
            }
        }

        private static void PollCommandFile(object state)
        {
            try
            {
                string dir = _commandDirectory;
                if (dir == null)
                    return;
                string path = Path.Combine(dir, CommandFileName);
                if (!File.Exists(path))
                    return;
                string text = File.ReadAllText(path);
                File.Delete(path);
                PostToMainThread(delegate { HandleCommand(text); });
            }
            catch
            {
                /* A file still being written is picked up on the next poll */
            }
        }

        /* Runs a window command on the main thread. One line of space-separated tokens:
             begin <scenario> <arm> <delaySeconds> <windowSeconds>
                 open a window after the delay, save it after the window, and write
                 window.done naming the saved JSON into ExportDirectory; when the
                 window gate refuses the command or its window (a suite or in-game test
                 is running, or another measurement's window is open), write
                 window.refused with the reason there instead
             end      save the open window now
             cancel   drop a scheduled or open window without saving
           end and cancel only ever end or discard a window a command began. Unknown or
           malformed input is ignored. */
        public static void HandleCommand(string line)
        {
            try
            {
                string[] tokens = (line ?? "").Split(new char[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                if (tokens.Length == 0)
                    return;
                switch (tokens[0].ToLowerInvariant())
                {
                case "begin":
                    ScheduleWindow(tokens.Length > 1 ? tokens[1] : "window", tokens.Length > 2 ? tokens[2] : "",
                        tokens.Length > 3 ? ParseSeconds(tokens[3], 0) : 0,
                        tokens.Length > 4 ? ParseSeconds(tokens[4], 60) : 60);
                    break;
                case "end":
                    if (_windowPhase == 2 && GHWindowGate.CommandOwnsWindow(_windowOwner))
                    {
                        StopWindowTimer();
                        SaveCommandedWindow();
                    }
                    else
                    {
                        CancelWindowCommand();
                    }
                    break;
                case "cancel":
                    CancelWindowCommand();
                    break;
                }
            }
            catch
            {
                /* A command must never break the app */
            }
        }

        private static double ParseSeconds(string text, double fallback)
        {
            double v;
            if (double.TryParse(text, NumberStyles.Float, CultureInfo.InvariantCulture, out v) && v >= 0 && v < 86400)
                return v;
            return fallback;
        }

        private static int DueMs(double seconds)
        {
            return (int)Math.Max(0, Math.Round(seconds * 1000.0));
        }

        /* Replaces a scheduled or open command window with a new schedule, unless the
           window gate refuses the command. A window longer than the frame timeline holds
           at the current refresh rate (GHFrameTimeline.MaxWindowSeconds of
           GHApp.ReconciledRefreshRate) is shortened to that limit and logged. */
        private static void ScheduleWindow(string scenario, string arm, double delaySeconds, double windowSeconds)
        {
            string refusal = GHWindowGate.ScheduleRefusal(IsWindowOpen, _windowOwner, IsSuiteOrDiagnosticRunning);
            if (refusal != null)
            {
                WriteRefusedNotice(scenario, refusal);
                return;
            }
            double hz = GHApp.ReconciledRefreshRate;
            double maxWindowSeconds = Math.Floor(GHFrameTimeline.MaxWindowSeconds(hz));
            if (windowSeconds > maxWindowSeconds)
            {
                Log("window command for " + scenario + ": " + windowSeconds.ToString(CultureInfo.InvariantCulture)
                    + " s shortened to " + maxWindowSeconds.ToString(CultureInfo.InvariantCulture)
                    + " s, the frame timeline's limit at " + Math.Round(hz).ToString(CultureInfo.InvariantCulture) + " Hz");
                windowSeconds = maxWindowSeconds;
            }
            CancelWindowCommand();
            int generation = Interlocked.Increment(ref _windowGeneration);
            _scheduledScenario = scenario;
            _scheduledArm = arm;
            _scheduledWindowSeconds = windowSeconds;
            _windowPhase = 1;
            _windowTimer = new Timer(OnWindowTimer, generation, DueMs(delaySeconds), Timeout.Infinite);
        }

        /* Runs on the thread pool. Before a window begins, the per-process interval's
           begin collect runs here, so the main thread never waits for it; it is skipped
           while a suite or in-game test runs, whose own interval it would restart, and
           whose run refuses the window anyway. */
        private static void OnWindowTimer(object state)
        {
            int generation = (int)state;
            if (Volatile.Read(ref _windowPhase) == 1 && generation == Volatile.Read(ref _windowGeneration)
                && !IsSuiteOrDiagnosticRunning)
            {
                try
                {
                    GHSystemLoadSampler.StartProcessIntervalAsync().Wait(GHSystemLoadSampler.ProcessIntervalBeginWaitMs);
                }
                catch { }
            }
            PostToMainThread(delegate { StepWindow(generation); });
        }

        private static void StepWindow(int generation)
        {
            if (generation != Volatile.Read(ref _windowGeneration))
                return;
            if (_windowPhase == 1)
            {
                string refusal = BeginRefusal(GHWindowOwner.Command);
                if (refusal != null)
                {
                    CancelWindowCommand();
                    WriteRefusedNotice(_scheduledScenario, refusal);
                    return;
                }
                if (!TryBeginWindow(_scheduledScenario, _scheduledArm, null, GHWindowOwner.Command))
                {
                    CancelWindowCommand();
                    return;
                }
                _windowPhase = 2;
                if (_windowTimer != null)
                    _windowTimer.Change(DueMs(_scheduledWindowSeconds), Timeout.Infinite);
            }
            else if (_windowPhase == 2)
            {
                StopWindowTimer();
                SaveCommandedWindow();
            }
        }

        private static void SaveCommandedWindow()
        {
            _windowPhase = 0;
            if (!GHWindowGate.CommandOwnsWindow(_windowOwner))
                return;
            string directory = ExportDirectory;
            string jsonPath = EndWindowAndSave(directory);
            if (jsonPath == null)
                return;
            try
            {
                File.WriteAllText(Path.Combine(directory, DoneFileName), Path.GetFileName(jsonPath) + Environment.NewLine,
                    new UTF8Encoding(false));
            }
            catch
            {
                /* The record is saved; only the notice is missing */
            }
        }

        private static void StopWindowTimer()
        {
            Interlocked.Increment(ref _windowGeneration);
            Timer timer = _windowTimer;
            _windowTimer = null;
            if (timer != null)
                timer.Dispose();
        }

        /* Drops a scheduled window, and discards an open one that a command began; a
           window another owner opened, and its run context, are left alone */
        private static void CancelWindowCommand()
        {
            StopWindowTimer();
            if (_windowPhase == 2 && GHWindowGate.CommandOwnsWindow(_windowOwner))
                DiscardWindow();
            _windowPhase = 0;
        }

        /* Writes window.refused into ExportDirectory: the refused scenario and the reason */
        private static void WriteRefusedNotice(string scenario, string reason)
        {
            try
            {
                File.WriteAllText(Path.Combine(ExportDirectory, RefusedFileName),
                    (scenario ?? "") + ": " + reason + Environment.NewLine, new UTF8Encoding(false));
            }
            catch
            {
                /* The refusal stands; only the notice is missing */
            }
        }

        private static void Log(string text)
        {
            try
            {
                GHApp.MaybeWriteGHLog("Performance run record: " + text);
            }
            catch
            {
                /* Logging must never break a window */
            }
        }

        private static void PostToMainThread(Action action)
        {
#if GNH_MAUI
            Microsoft.Maui.ApplicationModel.MainThread.BeginInvokeOnMainThread(action);
#else
            Xamarin.Forms.Device.BeginInvokeOnMainThread(action);
#endif
        }

        private static object BuildDocument(string runId, string scenario, string arm, DateTime startedUtc, DateTime endedUtc,
            GHThermalReading before, GHThermalReading after, GHSmoothnessSummary summary,
            SeriesJson series, string timelineCsvName, string compositorCsvName, GHPerformanceRunContext context,
            MarkJson[] marks, GHBackgroundReport background, TimelineJson timeline)
        {
            GHPerformanceEnvironmentFacts facts = GHPerformanceEnvironment.Capture();

            DocRoot doc = new DocRoot();
            doc.SchemaVersion = CurrentSchemaVersion;
            doc.RunId = runId;
            doc.Scenario = scenario;
            doc.Arm = arm;
            doc.StartedUtc = startedUtc.ToString("o", CultureInfo.InvariantCulture);
            doc.EndedUtc = endedUtc.ToString("o", CultureInfo.InvariantCulture);
            doc.WindowSeconds = R((endedUtc - startedUtc).TotalSeconds);

            doc.Environment = BuildEnvironment(facts);
            doc.Thermal = new ThermalJson { Before = ReadingJson.From(before), After = ReadingJson.From(after) };
            doc.Clock = BuildClock();
            doc.Display = BuildDisplay(summary);
            doc.Smoothness = BuildSmoothness(summary);
            doc.OnScreenPacing = BuildPacing(summary.OnScreenPacing);
            doc.Series = series;
            doc.UiThread = BuildUiThread();
            doc.Files = new FilesJson { FrameTimeline = timelineCsvName, CompositorFrames = compositorCsvName };
            doc.Suite = BuildSuite(context);
            doc.Marks = marks;
            doc.Background = BuildBackground(background);
            doc.Timeline = timeline;
            return doc;
        }

        /* The "background" block; null (omitted) when there is no report. NaN values are
           written as null. */
        private static BackgroundJson BuildBackground(GHBackgroundReport r)
        {
            if (r == null)
                return null;
            BackgroundJson j = new BackgroundJson();
            j.SamplerVersion = r.SamplerVersion;
            j.Source = r.Source;
            j.IntervalMs = r.IntervalMs;
            j.LogicalProcessors = r.LogicalProcessors;
            j.Coverage = N(r.Coverage);
            j.SamplerBusyMs = N(r.SamplerBusyMs);
            j.Signals = r.Signals.ToArray();
            j.Window = BuildBackgroundSummary(r.Window);
            j.PreWindow = BuildBackgroundSummary(r.PreWindow);
            j.OtherGpuPct = N(r.OtherGpuPct);

            j.Processes = new BackgroundProcessJson[r.Processes.Count];
            for (int i = 0; i < r.Processes.Count; i++)
            {
                GHProcessLoad p = r.Processes[i];
                BackgroundProcessJson pj = new BackgroundProcessJson();
                pj.Name = p.Name;
                pj.CpuPct = N(p.CpuPct);
                pj.GpuPct = N(p.GpuPct);
                pj.Category = !string.IsNullOrEmpty(p.Category) ? p.Category : GHBackgroundLoad.CategoryOf(p.Name);
                j.Processes[i] = pj;
            }

            int activityCount = r.Activities != null ? r.Activities.Count : 0;
            j.Activities = new BackgroundActivityJson[activityCount];
            for (int i = 0; i < activityCount; i++)
            {
                GHBackgroundActivity a = r.Activities[i];
                BackgroundActivityJson aj = new BackgroundActivityJson();
                aj.Category = a.Category;
                aj.Processes = a.Processes.ToArray();
                aj.CpuPct = N(a.CpuPct);
                j.Activities[i] = aj;
            }

            j.Samples = new BackgroundSampleJson[r.Samples.Count];
            for (int i = 0; i < r.Samples.Count; i++)
            {
                GHBackgroundReportSample s = r.Samples[i];
                BackgroundSampleJson sj = new BackgroundSampleJson();
                sj.T = N(s.T);
                sj.Sys = N(s.Sys);
                sj.Own = N(s.Own);
                sj.Other = N(s.Other);
                sj.Disk = N(s.Disk);
                sj.AvailPct = N(s.AvailPct);
                sj.Faults = N(s.Faults);
                j.Samples[i] = sj;
            }

            j.Verdict = r.VerdictName;
            j.Reason = r.Reason;
            return j;
        }

        private static BackgroundSummaryJson BuildBackgroundSummary(GHBackgroundSummary s)
        {
            if (s == null)
                return null;
            BackgroundSummaryJson j = new BackgroundSummaryJson();
            j.Samples = s.Samples;
            j.OtherCpuMeanPct = N(s.OtherCpuMeanPct);
            j.OtherCpuP90Pct = N(s.OtherCpuP90Pct);
            j.OtherCpuMaxPct = N(s.OtherCpuMaxPct);
            j.OtherCpuSpikeShare = N(s.OtherCpuSpikeShare);
            j.DiskBusyMeanPct = N(s.DiskBusyMeanPct);
            j.DiskBusyP90Pct = N(s.DiskBusyP90Pct);
            j.AvailableMemoryMinPct = N(s.AvailableMemoryMinPct);
            j.AvailableMemoryMinMB = s.AvailableMemoryMinMB >= 0 ? (long?)s.AvailableMemoryMinMB : null;
            j.HardFaultsP90PerSec = N(s.HardFaultsP90PerSec);
            j.MemoryPressureEvents = s.MemoryPressureEvents;
            j.LowMemory = s.LowMemory;
            return j;
        }

        /* Null when the run was not part of a suite, so the JSON carries no "suite"
           property at all; ExcludedReason here is only the caller's own preset reason,
           never the reasons ComputeExcludedReason derives (those follow from fields the
           document already carries elsewhere: thermal, on-screen interval count). */
        private static SuiteJson BuildSuite(GHPerformanceRunContext context)
        {
            if (context == null)
                return null;
            SuiteJson j = new SuiteJson();
            j.SuiteId = context.SuiteId;
            j.RunIndex = context.RunIndex;
            j.PageMode = context.PageMode;
            j.IsWarmUp = context.IsWarmUp;
            j.ReplayFileName = context.ReplayFileName;
            j.ReplayBytes = context.ReplayBytes;
            j.ReplaySha256 = context.ReplaySha256;
            j.StartTurn = context.StartTurn;
            j.TurnReached = context.TurnReached;
            if (context.TurnAtWindowStart >= 0)
                j.TurnAtWindowStart = context.TurnAtWindowStart;
            if (context.InputRecordsInWindow >= 0)
                j.InputRecordsInWindow = context.InputRecordsInWindow;
            j.ExcludedReason = context.ExcludedReason;
            j.Notes = context.Notes;
            return j;
        }

        private static EnvironmentJson BuildEnvironment(GHPerformanceEnvironmentFacts f)
        {
            EnvironmentJson e = new EnvironmentJson();
            e.Platform = f.Platform;
            e.DeviceId = f.DeviceId;
            e.DeviceModel = f.DeviceModel;
            e.DeviceOs = f.DeviceOs;
            e.AppVersion = f.AppVersion;
            e.RuntimeVersion = f.RuntimeVersion;
            e.FrameworkVersion = f.FrameworkVersion;
            e.UiFrameworkVersion = f.UiFrameworkVersion;
            e.SkiaSharpVersion = f.SkiaSharpVersion;
            e.FmodVersion = f.FmodVersion;
            e.GpuBackend = f.GpuBackend;
            e.GpuCacheSize = f.GpuCacheSize;
            e.MainCanvasUsesGpu = f.MainCanvasUsesGpu;
            e.MapRefreshRateSetting = f.MapRefreshRateSetting;
            e.ReportedRefreshHz = R(f.ReportedRefreshHz);
            e.BuildConfiguration = f.BuildConfiguration;
            e.GitCommit = f.GitCommit;
            e.Configuration = f.Configuration;
            e.Fingerprint = f.Fingerprint;
            return e;
        }

        private static ClockJson BuildClock()
        {
            ClockJson c = new ClockJson();
            c.StopwatchFrequency = Stopwatch.Frequency;
            if (GHFrameTimeline.HasClockAnchor)
            {
                GHClockAnchor first = GHFrameTimeline.FirstClockAnchor;
                GHClockAnchor latest = GHFrameTimeline.LatestClockAnchor;
                c.FirstAnchor = new AnchorJson { PlatformNanos = first.PlatformNanos, StopwatchTicks = first.StopwatchTicks };
                c.LatestAnchor = new AnchorJson { PlatformNanos = latest.PlatformNanos, StopwatchTicks = latest.StopwatchTicks };
            }
            return c;
        }

        private static DisplayJson BuildDisplay(GHSmoothnessSummary s)
        {
            DisplayJson d = new DisplayJson();
            d.MeasuredRefreshHz = R(s.MeasuredRefreshHz);
            d.CallbackRefreshHz = R(s.CallbackRefreshHz);
            d.AssumedRefreshHz = R(s.AssumedRefreshHz);
            d.TargetFps = R(s.TargetFps);
            d.AssumedRefreshMismatch = s.AssumedRefreshMismatch;
            return d;
        }

        private static SmoothnessJson BuildSmoothness(GHSmoothnessSummary s)
        {
            SmoothnessJson j = new SmoothnessJson();
            j.MetricsVersion = s.MetricsVersion;
            j.TickCount = s.TickCount;
            j.PaintedCount = s.PaintedCount;
            j.DisplayedCount = s.DisplayedCount;
            j.DroppedCount = s.DroppedCount;
            j.CoalescedCount = s.CoalescedCount;
            j.NotRunCount = s.NotRunCount;
            j.PausedGapCount = s.PausedGapCount;
            j.CompositorReportsLost = s.CompositorReportsLost;
            j.CompositorCoverage = s.CompositorCoverage < 0 ? (double?)null : R(s.CompositorCoverage);
            j.SyncOffsetP50Ms = R(s.SyncOffsetP50Ms);
            j.LongStallCount = s.LongStallCount;
            j.LongStallMs = R(s.LongStallMs);
            j.LongStallsExcluded = s.LongStallsExcluded;
            j.WindowMs = R(s.WindowMs);
            j.MeasuredRefreshHz = R(s.MeasuredRefreshHz);
            j.AssumedRefreshHz = R(s.AssumedRefreshHz);
            j.TargetFps = R(s.TargetFps);
            j.AssumedRefreshMismatch = s.AssumedRefreshMismatch;
            j.DisplayedFps = R(s.DisplayedFps);
            j.TileAnimationFps = R(s.TileAnimationFps);
            j.HitchCount = s.HitchCount;
            j.HitchRatioMsPerSec = R(s.HitchRatioMsPerSec);
            j.JudderCount = s.JudderCount;
            j.JudderPct = R(s.JudderPct);
            j.PacingErrorRmsMs = R(s.PacingErrorRmsMs);
            j.PacingErrorP99Ms = R(s.PacingErrorP99Ms);
            j.RepeatedRefreshesPerSec = R(s.RepeatedRefreshesPerSec);
            j.LatencyP50Ms = R(s.LatencyP50Ms);
            j.LatencyP99Ms = R(s.LatencyP99Ms);
            j.CallbackLatenessP99Ms = R(s.CallbackLatenessP99Ms);
            j.PaintP50Ms = R(s.PaintP50Ms);
            j.PaintP99Ms = R(s.PaintP99Ms);
            j.GcCount = s.GcCount;
            j.GcPauseMs = R(s.GcPauseMs);
            j.GcPauseDataAvailable = s.GcPauseDataAvailable;
            j.PresentSource = s.PresentSource.ToString();
            j.UnattributedShare = R(s.UnattributedShare);

            j.Causes = new Dictionary<string, CauseJson>();
            for (int i = 1; i < GHSmoothnessMetrics.CauseCount; i++)
            {
                GHHitchCause cause = (GHHitchCause)i;
                CauseJson cj = new CauseJson();
                cj.Count = s.CauseCount[i];
                cj.HitchCount = s.CauseHitchCount[i];
                cj.HitchMs = R(s.CauseMs[i]);
                j.Causes[GHSmoothnessMetrics.CauseName(cause)] = cj;
            }

            j.ContentEvents = new Dictionary<string, ContentEventJson>();
            for (int k = 0; k < GHSmoothnessMetrics.ContentEventKinds; k++)
            {
                ContentEventJson ej = new ContentEventJson();
                ej.Gaps = s.EventGapCount[k];
                ej.Hitches = s.EventHitchCount[k];
                j.ContentEvents[GHSmoothnessMetrics.ContentEventName(k)] = ej;
            }
            ContentEventJson quiet = new ContentEventJson();
            quiet.Gaps = s.QuietGapCount;
            quiet.Hitches = s.QuietHitchCount;
            j.ContentEvents["None"] = quiet;
            return j;
        }

        private static PacingJson BuildPacing(GHPerformanceStatistics.PacingMetrics p)
        {
            PacingJson j = new PacingJson();
            j.Count = p.Count;
            j.WindowMs = R(p.WindowMs);
            j.TargetPeriodMs = R(p.TargetPeriodMs);
            j.RefreshPeriodMs = R(p.RefreshPeriodMs);
            j.FpsMean = R(p.FpsMean);
            j.Fps1PctLow = R(p.Fps1PctLow);
            j.Fps01PctLow = R(p.Fps01PctLow);
            j.P50 = R(p.P50);
            j.P90 = R(p.P90);
            j.P95 = R(p.P95);
            j.P99 = R(p.P99);
            j.Max = R(p.Max);
            j.Min = R(p.Min);
            j.MeanMs = R(p.MeanMs);
            j.StdDevMs = R(p.StdDevMs);
            j.StutterIndex = R(p.StutterIndex);
            j.JankPct = R(p.JankPct);
            j.HitchRatioMsPerSec = R(p.HitchRatioMsPerSec);
            j.Hitch2xCount = p.Hitch2xCount;
            j.Hitch4xCount = p.Hitch4xCount;
            j.Hitch2xPerMin = R(p.Hitch2xPerMin);
            j.Hitch4xPerMin = R(p.Hitch4xPerMin);
            j.OverrunP50 = R(p.OverrunP50);
            j.OverrunP95 = R(p.OverrunP95);
            j.OverrunP99 = R(p.OverrunP99);
            return j;
        }

        /* Gaps and pacing errors of displayed frames after the first, excluding paused
           gaps, in the ms domain, in display order. */
        private static SeriesJson BuildSeries(GHDisplayedFrame[] displayed, int displayedCount)
        {
            SeriesJson j = new SeriesJson();
            List<double> intervals = new List<double>();
            List<double> pacingErrors = new List<double>();
            double msPerTick = 1000.0 / Stopwatch.Frequency;
            for (int i = 1; i < displayedCount; i++)
            {
                if (displayed[i].IsPausedGap)
                    continue;
                intervals.Add(Math.Round(displayed[i].GapTicks * msPerTick, 3));
                pacingErrors.Add(Math.Round(displayed[i].PacingErrorTicks * msPerTick, 3));
            }
            j.OnScreenIntervalsMs = intervals.ToArray();
            j.PacingErrorMs = pacingErrors.ToArray();
            return j;
        }

        private static UiThreadJson BuildUiThread()
        {
            UiThreadJson j = new UiThreadJson();
            try
            {
                j.LatencyP50Ms = R(GHUiThreadProbe.LatencyP50Ms);
                j.LatencyP95Ms = R(GHUiThreadProbe.LatencyP95Ms);
                j.LatencyP99Ms = R(GHUiThreadProbe.LatencyP99Ms);
                j.LatencyMaxMs = R(GHUiThreadProbe.LatencyMaxMs);
                j.TickCount = GHUiThreadProbe.TickCount;
                j.LateTickCount = GHUiThreadProbe.LateTickCount;
            }
            catch { }
            return j;
        }

        private static double R(double value)
        {
            if (double.IsNaN(value) || double.IsInfinity(value))
                return 0;
            return Math.Round(value, 3);
        }

        /* The background block's rounding: NaN and Infinity become null, not 0 */
        private static double? N(float value)
        {
            if (float.IsNaN(value) || float.IsInfinity(value))
                return null;
            return Math.Round((double)value, 3);
        }

        private static string SafeFileToken(string value)
        {
            if (string.IsNullOrEmpty(value))
                return "none";
            char[] invalid = Path.GetInvalidFileNameChars();
            StringBuilder sb = new StringBuilder(value.Length);
            for (int i = 0; i < value.Length; i++)
            {
                char c = value[i];
                bool bad = c <= ' ' || c == '.' || Array.IndexOf(invalid, c) >= 0;
                sb.Append(bad ? '_' : c);
            }
            return sb.ToString();
        }

        /* ---- JSON document shape (schema v2) ---- */

        private sealed class DocRoot
        {
            [JsonProperty("schemaVersion")]
            public int SchemaVersion;

            [JsonProperty("runId")]
            public string RunId;

            [JsonProperty("scenario")]
            public string Scenario;

            [JsonProperty("arm")]
            public string Arm;

            [JsonProperty("startedUtc")]
            public string StartedUtc;

            [JsonProperty("endedUtc")]
            public string EndedUtc;

            [JsonProperty("windowSeconds")]
            public double WindowSeconds;

            [JsonProperty("environment")]
            public EnvironmentJson Environment;

            [JsonProperty("thermal")]
            public ThermalJson Thermal;

            [JsonProperty("clock")]
            public ClockJson Clock;

            [JsonProperty("display")]
            public DisplayJson Display;

            [JsonProperty("smoothness")]
            public SmoothnessJson Smoothness;

            [JsonProperty("onScreenPacing")]
            public PacingJson OnScreenPacing;

            [JsonProperty("series")]
            public SeriesJson Series;

            [JsonProperty("uiThread")]
            public UiThreadJson UiThread;

            [JsonProperty("files")]
            public FilesJson Files;

            /* Omitted entirely (not even as null) when the run was not part of a suite */
            [JsonProperty("suite", NullValueHandling = NullValueHandling.Ignore)]
            public SuiteJson Suite;

            /* The user marks in the saved range; omitted entirely when there are none */
            [JsonProperty("marks", NullValueHandling = NullValueHandling.Ignore)]
            public MarkJson[] Marks;

            /* The background load around the window; omitted entirely when there is no report */
            [JsonProperty("background", NullValueHandling = NullValueHandling.Ignore)]
            public BackgroundJson Background;

            /* The requested ticks and how many the frame timeline still held */
            [JsonProperty("timeline", NullValueHandling = NullValueHandling.Ignore)]
            public TimelineJson Timeline;
        }

        private sealed class TimelineJson
        {
            [JsonProperty("requestedTicks")]
            public long RequestedTicks;

            [JsonProperty("retainedTicks")]
            public long RetainedTicks;

            /* The ring had overwritten some requested ticks */
            [JsonProperty("truncated")]
            public bool Truncated;
        }

        /* The "background" block (DEVEL/performance/schema/run-record.schema.json):
           signals a platform does not report are null and missing from "signals" */
        private sealed class BackgroundJson
        {
            [JsonProperty("samplerVersion")]
            public int SamplerVersion;

            [JsonProperty("source")]
            public string Source;

            [JsonProperty("intervalMs")]
            public int IntervalMs;

            [JsonProperty("logicalProcessors")]
            public int LogicalProcessors;

            /* Fraction of the expected window samples present */
            [JsonProperty("coverage")]
            public double? Coverage;

            /* Time the sampler's ticks took during the window */
            [JsonProperty("samplerBusyMs")]
            public double? SamplerBusyMs;

            [JsonProperty("signals")]
            public string[] Signals;

            [JsonProperty("window")]
            public BackgroundSummaryJson Window;

            /* The up to 10 s before the window start */
            [JsonProperty("preWindow")]
            public BackgroundSummaryJson PreWindow;

            [JsonProperty("otherGpuPct")]
            public double? OtherGpuPct;

            [JsonProperty("processes")]
            public BackgroundProcessJson[] Processes;

            [JsonProperty("activities")]
            public BackgroundActivityJson[] Activities;

            [JsonProperty("samples")]
            public BackgroundSampleJson[] Samples;

            /* quiet, elevated, busy or unknown */
            [JsonProperty("verdict")]
            public string Verdict;

            /* Set for elevated and busy; at most 100 characters */
            [JsonProperty("reason")]
            public string Reason;
        }

        private sealed class BackgroundSummaryJson
        {
            [JsonProperty("samples")]
            public int Samples;

            [JsonProperty("otherCpuMeanPct")]
            public double? OtherCpuMeanPct;

            [JsonProperty("otherCpuP90Pct")]
            public double? OtherCpuP90Pct;

            [JsonProperty("otherCpuMaxPct")]
            public double? OtherCpuMaxPct;

            [JsonProperty("otherCpuSpikeShare")]
            public double? OtherCpuSpikeShare;

            [JsonProperty("diskBusyMeanPct")]
            public double? DiskBusyMeanPct;

            [JsonProperty("diskBusyP90Pct")]
            public double? DiskBusyP90Pct;

            [JsonProperty("availableMemoryMinPct")]
            public double? AvailableMemoryMinPct;

            [JsonProperty("availableMemoryMinMB")]
            public long? AvailableMemoryMinMB;

            [JsonProperty("hardFaultsP90PerSec")]
            public double? HardFaultsP90PerSec;

            [JsonProperty("memoryPressureEvents")]
            public int MemoryPressureEvents;

            [JsonProperty("lowMemory")]
            public bool LowMemory;
        }

        private sealed class BackgroundProcessJson
        {
            [JsonProperty("name")]
            public string Name;

            [JsonProperty("cpuPct")]
            public double? CpuPct;

            [JsonProperty("gpuPct")]
            public double? GpuPct;

            [JsonProperty("category")]
            public string Category;
        }

        private sealed class BackgroundActivityJson
        {
            [JsonProperty("category")]
            public string Category;

            [JsonProperty("processes")]
            public string[] Processes;

            [JsonProperty("cpuPct")]
            public double? CpuPct;
        }

        /* One sample; t is seconds from the window start, negative before it */
        private sealed class BackgroundSampleJson
        {
            [JsonProperty("t")]
            public double? T;

            [JsonProperty("sys")]
            public double? Sys;

            [JsonProperty("own")]
            public double? Own;

            [JsonProperty("other")]
            public double? Other;

            [JsonProperty("disk")]
            public double? Disk;

            [JsonProperty("availPct")]
            public double? AvailPct;

            [JsonProperty("faults")]
            public double? Faults;
        }

        /* A frame the user marked as a felt stutter */
        private sealed class MarkJson
        {
            [JsonProperty("frameId")]
            public long FrameId;

            /* When the mark was made, ISO 8601 round-trip format */
            [JsonProperty("utc")]
            public string Utc;

            /* The marked tick's callback start, from the first saved tick's */
            [JsonProperty("msFromWindowStart")]
            public double MsFromWindowStart;
        }

        private sealed class EnvironmentJson
        {
            [JsonProperty("platform")]
            public string Platform;

            [JsonProperty("deviceId")]
            public string DeviceId;

            [JsonProperty("deviceModel")]
            public string DeviceModel;

            [JsonProperty("deviceOs")]
            public string DeviceOs;

            [JsonProperty("appVersion")]
            public string AppVersion;

            [JsonProperty("runtimeVersion")]
            public string RuntimeVersion;

            [JsonProperty("frameworkVersion")]
            public string FrameworkVersion;

            [JsonProperty("uiFrameworkVersion")]
            public string UiFrameworkVersion;

            [JsonProperty("skiaSharpVersion")]
            public string SkiaSharpVersion;

            [JsonProperty("fmodVersion")]
            public string FmodVersion;

            [JsonProperty("gpuBackend")]
            public string GpuBackend;

            [JsonProperty("gpuCacheSize")]
            public long GpuCacheSize;

            [JsonProperty("mainCanvasUsesGpu")]
            public bool MainCanvasUsesGpu;

            [JsonProperty("mapRefreshRateSetting")]
            public string MapRefreshRateSetting;

            [JsonProperty("reportedRefreshHz")]
            public double ReportedRefreshHz;

            [JsonProperty("buildConfiguration")]
            public string BuildConfiguration;

            [JsonProperty("gitCommit")]
            public string GitCommit;

            [JsonProperty("configuration")]
            public Dictionary<string, object> Configuration;

            /* Flat "<category>.<name>" string map (GHEnvironmentFingerprint) */
            [JsonProperty("fingerprint", NullValueHandling = NullValueHandling.Ignore)]
            public Dictionary<string, string> Fingerprint;
        }

        private sealed class ThermalJson
        {
            [JsonProperty("before")]
            public ReadingJson Before;

            [JsonProperty("after")]
            public ReadingJson After;
        }

        /* A GHThermalReading in the schema's shape: status by name, NaN fields as null */
        private sealed class ReadingJson
        {
            [JsonProperty("status")]
            public string Status;

            [JsonProperty("headroomFraction")]
            public double? HeadroomFraction;

            [JsonProperty("batteryTempC")]
            public double? BatteryTempC;

            [JsonProperty("cpuPerformancePct")]
            public double? CpuPerformancePct;

            /* On external power; null when the platform did not say */
            [JsonProperty("isCharging")]
            public bool? IsCharging;

            [JsonProperty("isLowPower")]
            public bool? IsLowPower;

            [JsonProperty("detail")]
            public string Detail;

            [JsonProperty("timestampUtc")]
            public string TimestampUtc;

            public static ReadingJson From(GHThermalReading r)
            {
                ReadingJson j = new ReadingJson();
                j.Status = GHThermalProbe.StatusName(r.Status);
                j.HeadroomFraction = NullIfNaN(r.HeadroomFraction);
                j.BatteryTempC = NullIfNaN(r.BatteryTempC);
                j.CpuPerformancePct = NullIfNaN(r.CpuPerformancePct);
                j.IsCharging = r.PowerStateKnown ? (bool?)r.IsCharging : null;
                j.IsLowPower = r.PowerStateKnown ? (bool?)r.IsLowPower : null;
                j.Detail = r.Detail;
                j.TimestampUtc = r.TimestampTicks > 0
                    ? new DateTime(r.TimestampTicks, DateTimeKind.Utc).ToString("o", CultureInfo.InvariantCulture)
                    : null;
                return j;
            }

            private static double? NullIfNaN(float value)
            {
                if (float.IsNaN(value) || float.IsInfinity(value))
                    return null;
                return Math.Round((double)value, 3);
            }
        }

        private sealed class ClockJson
        {
            [JsonProperty("stopwatchFrequency")]
            public long StopwatchFrequency;

            [JsonProperty("firstAnchor")]
            public AnchorJson FirstAnchor;

            [JsonProperty("latestAnchor")]
            public AnchorJson LatestAnchor;
        }

        private sealed class AnchorJson
        {
            [JsonProperty("platformNanos")]
            public long PlatformNanos;

            [JsonProperty("stopwatchTicks")]
            public long StopwatchTicks;
        }

        private sealed class DisplayJson
        {
            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            /* The display callback rate; below measuredRefreshHz when callbacks are skipped */
            [JsonProperty("callbackRefreshHz")]
            public double CallbackRefreshHz;

            [JsonProperty("assumedRefreshHz")]
            public double AssumedRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("assumedRefreshMismatch")]
            public bool AssumedRefreshMismatch;
        }

        private sealed class SmoothnessJson
        {
            /* GHSmoothnessMetrics.MetricsVersion of the analysis; absent means 1 */
            [JsonProperty("metricsVersion")]
            public int MetricsVersion;

            [JsonProperty("tickCount")]
            public int TickCount;

            [JsonProperty("paintedCount")]
            public int PaintedCount;

            [JsonProperty("displayedCount")]
            public int DisplayedCount;

            [JsonProperty("droppedCount")]
            public int DroppedCount;

            [JsonProperty("coalescedCount")]
            public int CoalescedCount;

            [JsonProperty("notRunCount")]
            public int NotRunCount;

            [JsonProperty("pausedGapCount")]
            public int PausedGapCount;

            /* FrameMetrics reports the listener missed; not display drops */
            [JsonProperty("compositorReportsLost")]
            public int CompositorReportsLost;

            /* Share of GL-thread paints carried by a FrameMetrics report, 0-1; null where not applicable */
            [JsonProperty("compositorCoverage")]
            public double? CompositorCoverage;

            /* Median FrameMetrics sync offset */
            [JsonProperty("syncOffsetP50Ms")]
            public double SyncOffsetP50Ms;

            /* Gaps with display callbacks stopped for GHSmoothnessMetrics.LongStallSeconds or more */
            [JsonProperty("longStallCount")]
            public int LongStallCount;

            [JsonProperty("longStallMs")]
            public double LongStallMs;

            /* Long stalls were paused gaps, not hitches */
            [JsonProperty("longStallsExcluded")]
            public bool LongStallsExcluded;

            [JsonProperty("windowMs")]
            public double WindowMs;

            [JsonProperty("measuredRefreshHz")]
            public double MeasuredRefreshHz;

            [JsonProperty("assumedRefreshHz")]
            public double AssumedRefreshHz;

            [JsonProperty("targetFps")]
            public double TargetFps;

            [JsonProperty("assumedRefreshMismatch")]
            public bool AssumedRefreshMismatch;

            [JsonProperty("displayedFps")]
            public double DisplayedFps;

            [JsonProperty("tileAnimationFps")]
            public double TileAnimationFps;

            [JsonProperty("hitchCount")]
            public int HitchCount;

            [JsonProperty("hitchRatioMsPerSec")]
            public double HitchRatioMsPerSec;

            [JsonProperty("judderCount")]
            public int JudderCount;

            [JsonProperty("judderPct")]
            public double JudderPct;

            [JsonProperty("pacingErrorRmsMs")]
            public double PacingErrorRmsMs;

            [JsonProperty("pacingErrorP99Ms")]
            public double PacingErrorP99Ms;

            [JsonProperty("repeatedRefreshesPerSec")]
            public double RepeatedRefreshesPerSec;

            [JsonProperty("latencyP50Ms")]
            public double LatencyP50Ms;

            [JsonProperty("latencyP99Ms")]
            public double LatencyP99Ms;

            [JsonProperty("callbackLatenessP99Ms")]
            public double CallbackLatenessP99Ms;

            [JsonProperty("paintP50Ms")]
            public double PaintP50Ms;

            [JsonProperty("paintP99Ms")]
            public double PaintP99Ms;

            [JsonProperty("gcCount")]
            public int GcCount;

            [JsonProperty("gcPauseMs")]
            public double GcPauseMs;

            /* False when the runtime reports no pause time; GC attribution then uses counts */
            [JsonProperty("gcPauseDataAvailable")]
            public bool GcPauseDataAvailable;

            [JsonProperty("presentSource")]
            public string PresentSource;

            [JsonProperty("unattributedShare")]
            public double UnattributedShare;

            [JsonProperty("causes")]
            public Dictionary<string, CauseJson> Causes;

            /* Per content event kind, and "None" for gaps without any: unpaused gaps and how many were hitches */
            [JsonProperty("contentEvents")]
            public Dictionary<string, ContentEventJson> ContentEvents;
        }

        private sealed class CauseJson
        {
            [JsonProperty("count")]
            public int Count;

            /* Of count, the hitches alone */
            [JsonProperty("hitchCount")]
            public int HitchCount;

            [JsonProperty("hitchMs")]
            public double HitchMs;
        }

        private sealed class ContentEventJson
        {
            [JsonProperty("gaps")]
            public int Gaps;

            [JsonProperty("hitches")]
            public int Hitches;
        }

        private sealed class PacingJson
        {
            [JsonProperty("count")]
            public int Count;

            [JsonProperty("windowMs")]
            public double WindowMs;

            [JsonProperty("targetPeriodMs")]
            public double TargetPeriodMs;

            [JsonProperty("refreshPeriodMs")]
            public double RefreshPeriodMs;

            [JsonProperty("fpsMean")]
            public double FpsMean;

            [JsonProperty("fps1PctLow")]
            public double Fps1PctLow;

            [JsonProperty("fps01PctLow")]
            public double Fps01PctLow;

            [JsonProperty("p50")]
            public double P50;

            [JsonProperty("p90")]
            public double P90;

            [JsonProperty("p95")]
            public double P95;

            [JsonProperty("p99")]
            public double P99;

            [JsonProperty("max")]
            public double Max;

            [JsonProperty("min")]
            public double Min;

            [JsonProperty("meanMs")]
            public double MeanMs;

            [JsonProperty("stdDevMs")]
            public double StdDevMs;

            [JsonProperty("stutterIndex")]
            public double StutterIndex;

            [JsonProperty("jankPct")]
            public double JankPct;

            [JsonProperty("hitchRatioMsPerSec")]
            public double HitchRatioMsPerSec;

            [JsonProperty("hitch2xCount")]
            public int Hitch2xCount;

            [JsonProperty("hitch4xCount")]
            public int Hitch4xCount;

            [JsonProperty("hitch2xPerMin")]
            public double Hitch2xPerMin;

            [JsonProperty("hitch4xPerMin")]
            public double Hitch4xPerMin;

            [JsonProperty("overrunP50")]
            public double OverrunP50;

            [JsonProperty("overrunP95")]
            public double OverrunP95;

            [JsonProperty("overrunP99")]
            public double OverrunP99;
        }

        private sealed class SeriesJson
        {
            [JsonProperty("onScreenIntervalsMs")]
            public double[] OnScreenIntervalsMs;

            [JsonProperty("pacingErrorMs")]
            public double[] PacingErrorMs;
        }

        private sealed class UiThreadJson
        {
            [JsonProperty("latencyP50Ms")]
            public double LatencyP50Ms;

            [JsonProperty("latencyP95Ms")]
            public double LatencyP95Ms;

            [JsonProperty("latencyP99Ms")]
            public double LatencyP99Ms;

            [JsonProperty("latencyMaxMs")]
            public double LatencyMaxMs;

            [JsonProperty("tickCount")]
            public long TickCount;

            [JsonProperty("lateTickCount")]
            public long LateTickCount;
        }

        private sealed class FilesJson
        {
            [JsonProperty("frameTimeline")]
            public string FrameTimeline;

            [JsonProperty("compositorFrames")]
            public string CompositorFrames;
        }

        /* Present only when the run was part of a suite (see DocRoot.Suite) */
        private sealed class SuiteJson
        {
            [JsonProperty("suiteId")]
            public string SuiteId;

            [JsonProperty("runIndex")]
            public int RunIndex;

            [JsonProperty("pageMode")]
            public string PageMode;

            [JsonProperty("isWarmUp")]
            public bool IsWarmUp;

            [JsonProperty("replayFileName")]
            public string ReplayFileName;

            [JsonProperty("replayBytes")]
            public long ReplayBytes;

            [JsonProperty("replaySha256")]
            public string ReplaySha256;

            [JsonProperty("startTurn")]
            public int StartTurn;

            [JsonProperty("turnReached")]
            public int TurnReached;

            /* Replay turn when the window opened; omitted when unknown */
            [JsonProperty("turnAtWindowStart", NullValueHandling = NullValueHandling.Ignore)]
            public int? TurnAtWindowStart;

            /* Replay input records played during the window; omitted when unknown */
            [JsonProperty("inputRecordsInWindow", NullValueHandling = NullValueHandling.Ignore)]
            public long? InputRecordsInWindow;

            /* Only the caller's preset reason; omitted when not set */
            [JsonProperty("excludedReason", NullValueHandling = NullValueHandling.Ignore)]
            public string ExcludedReason;

            /* e.g. a quiet gate timeout before the run; omitted when not set */
            [JsonProperty("notes", NullValueHandling = NullValueHandling.Ignore)]
            public string Notes;
        }
    }
}

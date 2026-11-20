#if SENTRY
using System.Globalization;
using System.Runtime.Versioning;
using GnollHackX;

namespace GnollHackM;

// WORKAROUND(WA-SENTRY-0012): Sentry reports no Android process deaths other than native crashes and ANRs
/* Keeps the process state summary that AndroidExitInfoReporter reads from the exit record on the next
   launch, and leaves an "Android memory:" breadcrumb with each full memory sample. Starts in
   MainApplication.OnCreate, before GHApp.Initialize, so nothing here reads GHApp until the
   AppReady startup milestone. Outside a game it writes every 15 s. During one its timer marks a
   GHDiagnosticsScheduler job due. The summary is written at the next heavy or light spot, and on
   memory warnings. The full sample runs only at heavy spots, or at a light spot once it is three
   intervals old. */
[SupportedOSPlatform("android30.0")]
public static class AndroidMemorySampler
{
    private const long IdleRewriteSeconds = 60;

    private static int _started = 0;
    private static int _busy = 0;
    private static int _fullSampleRequested = 0;
    private static int _snapshotRequested = 0;
    private static int _fullPassRequested = 0;
    private static int _appEventsSubscribed = 0;
    private static System.Threading.Timer _timer = null;
    private static GHDiagnosticsJob _job = null;
    private static long _startUtcTicks = 0L;

    /* Touched only inside RunPass, which _busy keeps to one thread at a time; _lastFullSampleUtcTicks is also
       read by IsFullSampleOverdue, so it is written atomically */
    private static GHExitStateSummary _lastWritten = null;
    private static long _lastWriteUtcTicks = 0L;
    private static long _lastFullSampleUtcTicks = 0L;
    private static long _lastSampledGameLoadUtcTicks = 0L;
    private static long _samplePssMB = -1;
    private static long _sampleGraphicsMB = -1;
    private static long _sampleNativeHeapMB = -1;
    private static readonly GHGcPauseTracker _gcTracker = new GHGcPauseTracker();
    private static long _lastEphemeralGcIndex = 0L;
    private static long _lastFullBlockingGcIndex = 0L;
    private static long _lastBackgroundGcIndex = 0L;
    private static long _gcNetCount = -1;     /* the last sample's cumulative figures, for the breadcrumb */
    private static long _gcNetPauseMs = -1;
    private static long _gcJavaCount = -1;
    private static long _gcJavaTimeMs = -1;
    private static long _gcBridgeGeneration = -1;

    /* The caller checks for API 30 */
    public static void Start()
    {
        if (Interlocked.Exchange(ref _started, 1) != 0)
            return;
        try
        {
            _startUtcTicks = DateTime.UtcNow.Ticks;
            GHStartupTimeline.Current.MilestoneReached += OnMilestoneReached;
            _timer = new System.Threading.Timer(OnTimerTick, null, 0, GHConstants.ExitStateSnapshotIntervalSeconds * 1000);
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine("AndroidMemorySampler start: " + ex.Message);
        }
    }

    private static bool IsAppReady
    {
        get { return GHStartupTimeline.Current.HasReached(GHStartupMilestone.AppReady); }
    }

    private static void OnMilestoneReached(GHStartupMilestone milestone)
    {
        Task.Run(() => RunPass(!(IsAppReady && GHApp.IsInGameForDiagnostics)));
    }

    /* During a game the summary is written at once and the full sample waits for a heavy spot or the ceiling */
    private static void OnMemoryPressure(MemoryPressureLevel level)
    {
        if (level == MemoryPressureLevel.Critical || level == MemoryPressureLevel.Complete)
            Interlocked.Exchange(ref _fullSampleRequested, 1);
        Task.Run(() => RunPass(!GHApp.IsInGameForDiagnostics));
    }

    /* On the spot's pool task, or inline in Request outside a game with a heavy timer spot */
    private static void OnJobRun(GHDiagnosticsSpot spot, bool wasDue)
    {
        RunPass(spot.IsHeavy || IsFullSampleOverdue());
    }

    /* The ceiling for the full sample: more than three intervals since the last one */
    private static bool IsFullSampleOverdue()
    {
        long last = Interlocked.Read(ref _lastFullSampleUtcTicks);
        return last != 0L
            && DateTime.UtcNow.Ticks - last > 3L * GHConstants.AndroidMemorySampleIntervalSeconds * TimeSpan.TicksPerSecond;
    }

    private static void OnTimerTick(object state)
    {
        GHDiagnosticsJob job = Volatile.Read(ref _job);
        if (!IsAppReady || job == null)
        {
            RunPass(true);
            return;
        }
        job.Request();
    }

    /* A request made while a pass runs makes that pass go round again, with the request's permission for a
       full sample, so no request waits for the timer */
    private static void RunPass(bool allowFullSample)
    {
        if (allowFullSample)
            Interlocked.Exchange(ref _fullPassRequested, 1);
        Interlocked.Exchange(ref _snapshotRequested, 1);
        do
        {
            bool appReady = IsAppReady;
            if (appReady && GHApp.IsAppClosing)
                return;
            if (Interlocked.Exchange(ref _busy, 1) == 1)
                return;
            try
            {
                Interlocked.Exchange(ref _snapshotRequested, 0);
                bool allowFull = Interlocked.Exchange(ref _fullPassRequested, 0) == 1;
                if (appReady && Interlocked.Exchange(ref _appEventsSubscribed, 1) == 0)
                {
                    GHApp.MemoryPressureReceived += OnMemoryPressure;
                    GHDiagnosticsJob job = new GHDiagnosticsJob("android-memory", GHDiagnosticsCost.Light, 0, true, OnJobRun);
                    Volatile.Write(ref _job, job);
                    GHDiagnosticsScheduler.Register(job);
                }
                SampleGarbageCollections();
                WriteSnapshot();
                if (allowFull && IsFullSampleDue())
                {
                    TakeFullSample();
                    WriteSnapshot();
                }
            }
            catch (Exception ex)
            {
                if (appReady)
                    GHApp.MaybeWriteGHLog("AndroidMemorySampler: " + ex.Message);
                else
                    System.Diagnostics.Debug.WriteLine("AndroidMemorySampler: " + ex.Message);
            }
            finally
            {
                Interlocked.Exchange(ref _busy, 0);
            }
        } while (Interlocked.CompareExchange(ref _snapshotRequested, 0, 0) == 1);
    }

    private static bool IsFullSampleDue()
    {
        if (!IsAppReady)
            return false;
        long now = DateTime.UtcNow.Ticks;

        /* One sample per loaded game, at the first pass after loading, whatever the spacing */
        long loaded = GHApp.GameLoadedUtcTicks;
        if (loaded != 0L && loaded != _lastSampledGameLoadUtcTicks)
        {
            _lastSampledGameLoadUtcTicks = loaded;
            return true;
        }

        if (_lastFullSampleUtcTicks != 0L && now - _lastFullSampleUtcTicks < GHConstants.AndroidMemorySampleMinimumSpacingSeconds * TimeSpan.TicksPerSecond)
            return false;
        if (Interlocked.Exchange(ref _fullSampleRequested, 0) == 1)
            return true;

        if (_lastFullSampleUtcTicks == 0L)
            return now - _startUtcTicks >= GHConstants.AndroidMemorySampleFirstDelaySeconds * TimeSpan.TicksPerSecond;
        return now - _lastFullSampleUtcTicks >= GHConstants.AndroidMemorySampleIntervalSeconds * TimeSpan.TicksPerSecond;
    }

    /* Before AppReady only the fields that do not read GHApp are filled */
    private static void WriteSnapshot()
    {
        long now = DateTime.UtcNow.Ticks;
        GHExitStateSummary summary = new GHExitStateSummary();
        summary.UnixSeconds = DateTimeOffset.UtcNow.ToUnixTimeSeconds();

        long processAgeMilliseconds, lastMilestoneMilliseconds;
        GHStartupMilestone lastMilestone;
        int reachedMask;
        GHStartupTimeline.Current.GetState(out processAgeMilliseconds, out lastMilestone, out lastMilestoneMilliseconds, out reachedMask);
        if (processAgeMilliseconds >= 0)
        {
            summary.ProcessAgeSeconds = processAgeMilliseconds / 1000;
            summary.StartupMilestone = (int)lastMilestone;
            summary.StartupMilestoneAgeSeconds = lastMilestoneMilliseconds < 0 ? -1 : Math.Max(0L, (processAgeMilliseconds - lastMilestoneMilliseconds) / 1000);
            summary.StartupMilestoneMask = reachedMask;
        }

        long rssKB, swapKB;
        if (!IsAppReady)
        {
            summary.GameState = 0;
            summary.MemoryWarningLevel = 0;
            ReadProcStatus(out rssKB, out swapKB);
            summary.RssMB = rssKB < 0 ? -1 : rssKB / 1024;
            summary.SwapMB = swapKB < 0 ? -1 : swapKB / 1024;
            summary.ManagedHeapMB = GC.GetGCMemoryInfo().HeapSizeBytes / (1024 * 1024);
            FillGcWindow(summary, now);
            WriteIfChanged(summary, now);
            return;
        }

        long gameStart = GHApp.GameStartedUtcTicks;
        long gameLoaded = GHApp.GameLoadedUtcTicks;
        summary.GameState = CurrentGameState();
        if (summary.GameState != 0 && gameStart != 0L)
            summary.SecondsInGame = Math.Max(0L, (now - gameStart) / TimeSpan.TicksPerSecond);
        if (summary.GameState != 0 && gameLoaded != 0L)
            summary.SecondsSinceLoaded = Math.Max(0L, (now - gameLoaded) / TimeSpan.TicksPerSecond);

        summary.MemoryWarningLevel = GHApp.LastMemoryPressureLevel;
        long warningTicks = GHApp.LastMemoryPressureUtcTicks;
        if (summary.MemoryWarningLevel > 0 && warningTicks != 0L)
            summary.MemoryWarningAgeSeconds = Math.Max(0L, (now - warningTicks) / TimeSpan.TicksPerSecond);
        summary.HighestMemoryWarningLevel = GHApp.HighestMemoryPressureLevel;

        ReadProcStatus(out rssKB, out swapKB);
        summary.RssMB = rssKB < 0 ? -1 : rssKB / 1024;
        summary.SwapMB = swapKB < 0 ? -1 : swapKB / 1024;

        if (_lastFullSampleUtcTicks != 0L)
        {
            summary.PssMB = _samplePssMB;
            summary.GraphicsMB = _sampleGraphicsMB;
            summary.NativeHeapMB = _sampleNativeHeapMB;
            summary.PssSampleAgeSeconds = Math.Max(0L, (now - _lastFullSampleUtcTicks) / TimeSpan.TicksPerSecond);
        }

        CacheUsageInfo gpuUsage = GHApp.CurrentGPUCacheUsage;
        long gpuLimit = GHApp.CurrentGPUCacheSize;
        summary.GpuCacheUsedMB = gpuUsage.MaxResourceBytes < 0 ? -1 : gpuUsage.MaxResourceBytes / (1024 * 1024);
        summary.GpuCacheLimitMB = gpuLimit < 0 ? -1 : gpuLimit / (1024 * 1024);
        long gpuSampleTicks = GHApp.CurrentGPUCacheUsageUtcTicks;
        if (gpuSampleTicks != 0L)
            summary.GpuCacheSampleAgeSeconds = Math.Max(0L, (now - gpuSampleTicks) / TimeSpan.TicksPerSecond);
        summary.BitmapMB = (long)(GHApp.UsedBitmapBytes / (1024UL * 1024UL));
        summary.TileSheetMB = Math.Max(0L, GHApp.TileSheetBytes) / (1024L * 1024L);

        int soundCurrent = -1, soundMax = -1;
        GHApp.FmodService?.GetStats(out soundCurrent, out soundMax);
        summary.SoundMB = soundCurrent < 0 ? -1 : soundCurrent / (1024 * 1024);
        summary.ManagedHeapMB = GC.GetGCMemoryInfo().HeapSizeBytes / (1024 * 1024);
        FillGcWindow(summary, now);
        WriteIfChanged(summary, now);
    }

    private static void FillGcWindow(GHExitStateSummary summary, long nowUtcTicks)
    {
        long gcNet, gcJava, gcJavaCount, gcBridgeCount;
        _gcTracker.GetWindow(nowUtcTicks, out gcNet, out gcJava, out gcJavaCount, out gcBridgeCount);
        summary.GcNetLongestMs = gcNet;
        summary.GcJavaLongestMs = gcJava;
        summary.GcJavaCount = gcJavaCount;
    }

    /* Cumulative .NET and Java collection figures and the GC bridge generation, at each pass */
    private static void SampleGarbageCollections()
    {
        long netCount = -1, netPauseMs = -1, netNewestPauseMs = -1, bridgeGeneration = -1;
#if CORECLR
        long ephemeralIndex, fullBlockingIndex, backgroundIndex;
        long ephemeralPause = NewPauseMs(GCKind.Ephemeral, ref _lastEphemeralGcIndex, out ephemeralIndex);
        long fullBlockingPause = NewPauseMs(GCKind.FullBlocking, ref _lastFullBlockingGcIndex, out fullBlockingIndex);
        long backgroundPause = NewPauseMs(GCKind.Background, ref _lastBackgroundGcIndex, out backgroundIndex);
        netNewestPauseMs = Math.Max(ephemeralPause, Math.Max(fullBlockingPause, backgroundPause));
        /* The newest collection's number; GC.CollectionCount(0) misses background collections */
        netCount = Math.Max(ephemeralIndex, Math.Max(fullBlockingIndex, backgroundIndex));
        netPauseMs = (long)GC.GetTotalPauseDuration().TotalMilliseconds;
        /* Completed bridge passes; a field read, no JNI call */
        bridgeGeneration = Android.Runtime.JNIEnv.BridgeProcessingGeneration;
#endif
        long javaCount = RuntimeStat("art.gc.gc-count");
        long javaTimeMs = RuntimeStat("art.gc.gc-time");
        _gcTracker.AddSample(DateTime.UtcNow.Ticks, netCount, netPauseMs, netNewestPauseMs, javaCount, javaTimeMs, bridgeGeneration);
        _gcNetCount = netCount;
        _gcNetPauseMs = netPauseMs;
        _gcJavaCount = javaCount;
        _gcJavaTimeMs = javaTimeMs;
        _gcBridgeGeneration = bridgeGeneration;

        if (IsAppReady && _gcTracker.LastIntervalIsLong)
        {
            GHApp.AddSentryBreadcrumb("Long GC: longest .NET pause " + Number(_gcTracker.LastIntervalNetLongestMs)
                + " ms; Java GC " + Number(_gcTracker.LastIntervalJavaLongestMs) + " ms per collection over " + Number(_gcTracker.LastIntervalJavaCount)
                + "; " + Number(_gcTracker.LastIntervalBridgeCount) + " bridge passes; in the " + Number(_gcTracker.LastIntervalSeconds)
                + " s before this sample", GHConstants.SentryGnollHackGeneralCategoryName);
        }
    }

#if CORECLR
    /* The kind's newest collection's total pause when it is newer than the one seen last time, else -1 */
    private static long NewPauseMs(GCKind kind, ref long lastIndex, out long index)
    {
        GCMemoryInfo info = GC.GetGCMemoryInfo(kind);
        index = info.Index;
        if (info.Index <= lastIndex)
            return -1;
        double pauseMs = 0;
        foreach (TimeSpan pause in info.PauseDurations)
            pauseMs += pause.TotalMilliseconds;
        lastIndex = info.Index;
        return (long)pauseMs;
    }
#endif

    /* An ART runtime statistic as a number, -1 when unavailable */
    private static long RuntimeStat(string name)
    {
        try
        {
            string text = Android.OS.Debug.GetRuntimeStat(name);
            long value;
            return text != null && long.TryParse(text, NumberStyles.None, CultureInfo.InvariantCulture, out value) ? value : -1;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine("AndroidMemorySampler runtime stat " + name + ": " + ex.Message);
            return -1;
        }
    }

    /* GHExitStateSummary.GameState coding; only after AppReady */
    private static int CurrentGameState()
    {
        if (!GHApp.GameStarted)
            return 0;
        int state = GHApp.CurrentGamePage?.PlayingReplay == true ? 2 : 1;
        if (GHApp.GameLoadedUtcTicks == 0L)
            state += 2;
        return state;
    }

    private static void WriteIfChanged(GHExitStateSummary summary, long nowUtcTicks)
    {
        if (summary.ContentEquals(_lastWritten) && nowUtcTicks - _lastWriteUtcTicks < IdleRewriteSeconds * TimeSpan.TicksPerSecond)
            return;
        AndroidExitInfoReporter.WriteStateSummary(summary.Format());
        _lastWritten = summary;
        _lastWriteUtcTicks = nowUtcTicks;
    }

    /* VmRSS and VmSwap in kB, -1 when unavailable */
    private static void ReadProcStatus(out long rssKB, out long swapKB)
    {
        rssKB = -1;
        swapKB = -1;
        try
        {
            foreach (string line in File.ReadAllLines("/proc/self/status"))
            {
                if (line.StartsWith("VmRSS:", StringComparison.Ordinal))
                    rssKB = ParseKB(line.Substring(6));
                else if (line.StartsWith("VmSwap:", StringComparison.Ordinal))
                    swapKB = ParseKB(line.Substring(7));
            }
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine("AndroidMemorySampler status: " + ex.Message);
        }
    }

    /* "   1234 kB" */
    private static long ParseKB(string text)
    {
        string trimmed = text.Trim();
        int space = trimmed.IndexOf(' ');
        if (space > 0)
            trimmed = trimmed.Substring(0, space);
        long value;
        return long.TryParse(trimmed, NumberStyles.None, CultureInfo.InvariantCulture, out value) ? value : -1;
    }

    private static void TakeFullSample()
    {
        Android.OS.Debug.MemoryInfo info = new Android.OS.Debug.MemoryInfo();
        Android.OS.Debug.GetMemoryInfo(info);
        long pssKB = MemoryStatKB(info, "summary.total-pss");
        long graphicsKB = MemoryStatKB(info, "summary.graphics");
        long nativeKB = MemoryStatKB(info, "summary.native-heap");
        long javaKB = MemoryStatKB(info, "summary.java-heap");
        long codeKB = MemoryStatKB(info, "summary.code");
        long otherKB = MemoryStatKB(info, "summary.private-other");
        long swapKB = MemoryStatKB(info, "summary.total-swap");

        _samplePssMB = ToMB(pssKB);
        _sampleGraphicsMB = ToMB(graphicsKB);
        _sampleNativeHeapMB = ToMB(nativeKB);
        Interlocked.Exchange(ref _lastFullSampleUtcTicks, DateTime.UtcNow.Ticks);

        long rssKB, statusSwapKB;
        ReadProcStatus(out rssKB, out statusSwapKB);
        CacheUsageInfo gpuUsage = GHApp.CurrentGPUCacheUsage;
        long gpuLimit = GHApp.CurrentGPUCacheSize;
        int soundCurrent = -1, soundMax = -1;
        GHApp.FmodService?.GetStats(out soundCurrent, out soundMax);
        long gameStart = GHApp.GameStartedUtcTicks;
        int gameState = CurrentGameState();
        string game = GHExitStateSummary.GameStateName(gameState);
        if (gameState != 0 && gameStart != 0L)
            game += " " + ((DateTime.UtcNow.Ticks - gameStart) / TimeSpan.TicksPerSecond).ToString(CultureInfo.InvariantCulture) + " s";

        string message = "Android memory: pss " + Mb(ToMB(pssKB)) + " MB (swap " + Mb(ToMB(swapKB))
            + "), native " + Mb(ToMB(nativeKB)) + ", java " + Mb(ToMB(javaKB))
            + ", graphics " + Mb(ToMB(graphicsKB)) + ", code " + Mb(ToMB(codeKB)) + ", other " + Mb(ToMB(otherKB))
            + "; rss " + Mb(ToMB(rssKB))
            + "; GPU cache " + Mb(gpuUsage.MaxResourceBytes < 0 ? -1 : gpuUsage.MaxResourceBytes / (1024 * 1024))
            + "/" + Mb(gpuLimit < 0 ? -1 : gpuLimit / (1024 * 1024)) + " MB"
            + "; bitmaps " + Mb((long)(GHApp.UsedBitmapBytes / (1024UL * 1024UL))) + " MB"
            + ", tile sheets " + Mb(Math.Max(0L, GHApp.TileSheetBytes) / (1024L * 1024L)) + " MB"
            + "; sound " + Mb(soundCurrent < 0 ? -1 : soundCurrent / (1024 * 1024)) + " MB";
        long gcNet, gcJava, gcJavaCount, gcBridgeCount;
        _gcTracker.GetWindow(DateTime.UtcNow.Ticks, out gcNet, out gcJava, out gcJavaCount, out gcBridgeCount);
        message += "; GC .NET " + Number(_gcNetCount) + " (" + Number(_gcNetPauseMs) + " ms paused), Java " + Number(_gcJavaCount)
            + " (" + Number(_gcJavaTimeMs) + " ms), bridge passes " + Number(_gcBridgeGeneration)
            + ", in " + (GHGcPauseTracker.WindowSeconds / 60).ToString(CultureInfo.InvariantCulture) + " min: longest .NET " + Number(gcNet) + " ms, Java " + Number(gcJava) + " ms, " + Number(gcBridgeCount) + " bridge passes"
            + "; " + game;
        GHApp.AddSentryBreadcrumb(message, GHConstants.SentryGnollHackGeneralCategoryName);
    }

    private static long MemoryStatKB(Android.OS.Debug.MemoryInfo info, string name)
    {
        try
        {
            string text = info.GetMemoryStat(name);
            long value;
            return text != null && long.TryParse(text, NumberStyles.None, CultureInfo.InvariantCulture, out value) ? value : -1;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine("AndroidMemorySampler stat " + name + ": " + ex.Message);
            return -1;
        }
    }

    private static long ToMB(long kB)
    {
        return kB < 0 ? -1 : kB / 1024;
    }

    private static string Mb(long value)
    {
        return value < 0 ? "?" : value.ToString(CultureInfo.InvariantCulture);
    }

    private static string Number(long value)
    {
        return value < 0 ? "?" : value.ToString(CultureInfo.InvariantCulture);
    }
}
#endif

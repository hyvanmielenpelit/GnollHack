#if SENTRY
using System.Globalization;
using System.Runtime.Versioning;
using GnollHackX;

namespace GnollHackM;

// WORKAROUND(WA-SENTRY-0012): Sentry reports no Android process deaths other than native crashes and ANRs
/* Keeps the process state summary that AndroidExitInfoReporter reads from the exit record on the next
   launch, and leaves an "Android memory:" breadcrumb with each full memory sample. Starts in
   MainApplication.OnCreate, before GHApp.Initialize, so nothing here reads GHApp until the
   AppReady startup milestone. */
[SupportedOSPlatform("android30.0")]
public static class AndroidMemorySampler
{
    private const long IdleRewriteSeconds = 60;
    private const long GameLoadedSampleDelaySeconds = 30;

    private static int _started = 0;
    private static int _busy = 0;
    private static int _fullSampleRequested = 0;
    private static int _snapshotRequested = 0;
    private static int _memoryPressureSubscribed = 0;
    private static System.Threading.Timer _timer = null;
    private static long _startUtcTicks = 0L;

    /* Touched only inside OnTick, which _busy keeps to one thread at a time */
    private static GHExitStateSummary _lastWritten = null;
    private static long _lastWriteUtcTicks = 0L;
    private static long _lastFullSampleUtcTicks = 0L;
    private static long _lastSampledGameStartUtcTicks = 0L;
    private static long _samplePssMB = -1;
    private static long _sampleGraphicsMB = -1;
    private static long _sampleNativeHeapMB = -1;

    /* The caller checks for API 30 */
    public static void Start()
    {
        if (Interlocked.Exchange(ref _started, 1) != 0)
            return;
        try
        {
            _startUtcTicks = DateTime.UtcNow.Ticks;
            GHStartupTimeline.Current.MilestoneReached += OnMilestoneReached;
            _timer = new System.Threading.Timer(OnTick, null, 0, GHConstants.ExitStateSnapshotIntervalSeconds * 1000);
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
        Interlocked.Exchange(ref _snapshotRequested, 1);
        Task.Run(() => OnTick(null));
    }

    private static void OnMemoryPressure(MemoryPressureLevel level)
    {
        if (level == MemoryPressureLevel.Critical || level == MemoryPressureLevel.Complete)
            Interlocked.Exchange(ref _fullSampleRequested, 1);
        Task.Run(() => OnTick(null));
    }

    /* A snapshot requested while a pass runs makes that pass go round again, so no milestone waits for the timer */
    private static void OnTick(object state)
    {
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
                if (appReady && Interlocked.Exchange(ref _memoryPressureSubscribed, 1) == 0)
                    GHApp.MemoryPressureReceived += OnMemoryPressure;
                WriteSnapshot();
                if (IsFullSampleDue())
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
        if (_lastFullSampleUtcTicks != 0L && now - _lastFullSampleUtcTicks < GHConstants.AndroidMemorySampleMinimumSpacingSeconds * TimeSpan.TicksPerSecond)
            return false;
        if (Interlocked.Exchange(ref _fullSampleRequested, 0) == 1)
            return true;

        long gameStart = GHApp.GameStartedUtcTicks;
        if (gameStart != 0L && gameStart != _lastSampledGameStartUtcTicks
            && now - gameStart >= GameLoadedSampleDelaySeconds * TimeSpan.TicksPerSecond)
        {
            _lastSampledGameStartUtcTicks = gameStart;
            return true;
        }

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
            WriteIfChanged(summary, now);
            return;
        }

        long gameStart = GHApp.GameStartedUtcTicks;
        if (!GHApp.GameStarted)
            summary.GameState = 0;
        else
            summary.GameState = GHApp.CurrentGamePage?.PlayingReplay == true ? 2 : 1;
        if (summary.GameState != 0 && gameStart != 0L)
            summary.SecondsInGame = Math.Max(0L, (now - gameStart) / TimeSpan.TicksPerSecond);

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
        summary.BitmapMB = (long)(GHApp.UsedBitmapBytes / (1024UL * 1024UL));

        int soundCurrent = -1, soundMax = -1;
        GHApp.FmodService?.GetStats(out soundCurrent, out soundMax);
        summary.SoundMB = soundCurrent < 0 ? -1 : soundCurrent / (1024 * 1024);
        summary.ManagedHeapMB = GC.GetGCMemoryInfo().HeapSizeBytes / (1024 * 1024);
        WriteIfChanged(summary, now);
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
        _lastFullSampleUtcTicks = DateTime.UtcNow.Ticks;

        long rssKB, statusSwapKB;
        ReadProcStatus(out rssKB, out statusSwapKB);
        CacheUsageInfo gpuUsage = GHApp.CurrentGPUCacheUsage;
        long gpuLimit = GHApp.CurrentGPUCacheSize;
        int soundCurrent = -1, soundMax = -1;
        GHApp.FmodService?.GetStats(out soundCurrent, out soundMax);
        long gameStart = GHApp.GameStartedUtcTicks;
        int gameState = !GHApp.GameStarted ? 0 : GHApp.CurrentGamePage?.PlayingReplay == true ? 2 : 1;
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
            + "; sound " + Mb(soundCurrent < 0 ? -1 : soundCurrent / (1024 * 1024)) + " MB"
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
}
#endif

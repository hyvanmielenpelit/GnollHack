#if WINDOWS && GNH_MAUI && SENTRY
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using GnollHackX;
using Sentry;

namespace GnollHackM
{
    /* Process and system memory figures for Windows Sentry events, and a periodic breadcrumb.
       Samples come from direct Win32 calls, which neither enumerate other processes nor allocate
       much, so they still work when the system is short of memory. Video memory comes from DXGI
       through VideoMemoryWindows, the handle census from HandleCensusWindows, and the render
       surface counters from RenderSurfaceDiagnostics. Growth figures are relative to a baseline
       taken in Start. */
    internal static class ProcessMemoryWindows
    {
        private const int FirstSampleDelayMilliseconds = 60 * 1000;
#if DEBUG
        private const int SampleIntervalMilliseconds = 2 * 60 * 1000;
#else
        private const int SampleIntervalMilliseconds = 30 * 60 * 1000;
#endif
        private const uint GR_GDIOBJECTS = 0;
        private const uint GR_USEROBJECTS = 1;
        private const uint GR_GDIOBJECTS_PEAK = 2;
        private const uint GR_USEROBJECTS_PEAK = 4;
        private const ulong MiB = 1024UL * 1024UL;
        private const ulong GiB = 1024UL * MiB;

        [StructLayout(LayoutKind.Sequential)]
        private struct PROCESS_MEMORY_COUNTERS_EX
        {
            public uint cb;
            public uint PageFaultCount;
            public nuint PeakWorkingSetSize;
            public nuint WorkingSetSize;
            public nuint QuotaPeakPagedPoolUsage;
            public nuint QuotaPagedPoolUsage;
            public nuint QuotaPeakNonPagedPoolUsage;
            public nuint QuotaNonPagedPoolUsage;
            public nuint PagefileUsage;
            public nuint PeakPagefileUsage;
            public nuint PrivateUsage;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;
        }

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetCurrentProcess();

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool K32GetProcessMemoryInfo(IntPtr process, ref PROCESS_MEMORY_COUNTERS_EX counters,
            uint cb);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetProcessHandleCount(IntPtr process, out uint handleCount);

        [DllImport("user32.dll")]
        private static extern uint GetGuiResources(IntPtr process, uint flags);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GlobalMemoryStatusEx([In, Out] MEMORYSTATUSEX buffer);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsWindow(IntPtr hwnd);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsIconic(IntPtr hwnd);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsZoomed(IntPtr hwnd);

        [DllImport("user32.dll")]
        private static extern IntPtr GetForegroundWindow();

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetClientRect(IntPtr hwnd, out RECT rect);

        private struct Sample
        {
            public bool HasProcess;
            public ulong PrivateBytes;
            public ulong PeakPrivateBytes;
            public ulong WorkingSetBytes;
            public ulong PeakWorkingSetBytes;
            public uint Handles;
            public uint GdiObjects;
            public uint UserObjects;
            public uint PeakGdiObjects;
            public uint PeakUserObjects;
            public bool HasSystem;
            public ulong CommitUsedBytes;
            public ulong CommitLimitBytes;
            public uint PhysicalLoadPercent;
            public List<VideoMemorySample> Video;   /* null when no adapter answered */
            public int Gen0Collections;
            public int Gen1Collections;
            public int Gen2Collections;
            public bool HasWindow;
            public int WindowWidth;                 /* last nonzero client size, physical pixels */
            public int WindowHeight;
            public string WindowState;              /* minimized, maximized or normal */
            public bool WindowForeground;
            public bool HasCensus;
            public HandleCensus Census;
        }

        private static Timer _timer;
        private static IntPtr _mainWindowHandle = IntPtr.Zero;
        private static long _lastClientSize = 0;    /* (width << 32) | height */

        /* Written once in Start, before the timer exists */
        private static bool _hasBaselineHandles;
        private static uint _baselineHandles;
        private static bool _hasBaselineCommit;
        private static ulong _baselineCommitUsedBytes;
        private static long _baselineTimestamp;

        /* Timer thread only */
        private static string _lastSurfaceSummary;

        public static void Start()
        {
            if (_timer != null)
                return;
            TakeBaseline();
            _timer = new Timer(OnTimer, null, FirstSampleDelayMilliseconds, SampleIntervalMilliseconds);
            /* Builds the DXGI adapter cache before the first event or breadcrumb needs it */
            ThreadPool.QueueUserWorkItem(delegate { VideoMemoryWindows.TrySample(new List<VideoMemorySample>(2)); });
        }

        public static void SetMainWindowHandle(IntPtr hwnd)
        {
            Interlocked.Exchange(ref _mainWindowHandle, hwnd);
        }

        private static void TakeBaseline()
        {
            try
            {
                _baselineTimestamp = Stopwatch.GetTimestamp();
                uint handles;
                if (GetProcessHandleCount(GetCurrentProcess(), out handles))
                {
                    _baselineHandles = handles;
                    _hasBaselineHandles = true;
                }
                MEMORYSTATUSEX status = new MEMORYSTATUSEX();
                if (GlobalMemoryStatusEx(status) && status.ullTotalPageFile >= status.ullAvailPageFile)
                {
                    _baselineCommitUsedBytes = status.ullTotalPageFile - status.ullAvailPageFile;
                    _hasBaselineCommit = true;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnTimer(object state)
        {
            try
            {
                Sample s = TakeSample();
                GCMemoryInfo gcInfo = GC.GetGCMemoryInfo();
                long managedBytes = GC.GetTotalMemory(false);
                string text = BuildBreadcrumbText(s, gcInfo, managedBytes);
                GHApp.AddSentryBreadcrumb(text, GHConstants.SentryGnollHackGeneralCategoryName);
                string summary = RenderSurfaceDiagnostics.BuildSummary();
                if (!string.IsNullOrEmpty(summary) && summary != _lastSurfaceSummary)
                {
                    _lastSurfaceSummary = summary;
                    GHApp.AddSentryBreadcrumb(summary, GHConstants.SentryGnollHackGeneralCategoryName);
                }
#if DEBUG
                Debug.WriteLine(text);
                Debug.WriteLine(summary);
                List<KeyValuePair<string, string>> tags = new List<KeyValuePair<string, string>>();
                Dictionary<string, object> context = BuildContext(s, tags);
                foreach (KeyValuePair<string, object> kv in context)
                    Debug.WriteLine("Process Memory context: " + kv.Key + " = " + Convert.ToString(kv.Value, CultureInfo.InvariantCulture));
                foreach (KeyValuePair<string, string> tag in tags)
                    Debug.WriteLine("Process Memory tag: " + tag.Key + " = " + tag.Value);
#endif
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* Called from Sentry's BeforeSend; must never throw */
        public static void AddToEvent(SentryEvent sentryEvent)
        {
            if (sentryEvent == null)
                return;
            try
            {
                Sample s = TakeSample();
                List<KeyValuePair<string, string>> tags = new List<KeyValuePair<string, string>>();
                Dictionary<string, object> context = BuildContext(s, tags);
                foreach (KeyValuePair<string, string> tag in tags)
                    sentryEvent.SetTag(tag.Key, tag.Value);
                sentryEvent.Contexts[GHConstants.SentryContextProcessMemory] = context;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static Dictionary<string, object> BuildContext(Sample s, List<KeyValuePair<string, string>> tags)
        {
            Dictionary<string, object> context = new Dictionary<string, object>();
            if (s.HasProcess)
            {
                context["private_bytes"] = (long)s.PrivateBytes;
                context["peak_private_bytes"] = (long)s.PeakPrivateBytes;
                context["working_set_bytes"] = (long)s.WorkingSetBytes;
                context["peak_working_set_bytes"] = (long)s.PeakWorkingSetBytes;
                context["handle_count"] = (long)s.Handles;
                context["gdi_objects"] = (long)s.GdiObjects;
                context["user_objects"] = (long)s.UserObjects;
                context["peak_gdi_objects"] = (long)s.PeakGdiObjects;
                context["peak_user_objects"] = (long)s.PeakUserObjects;
                tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagProcessPrivate, SizeBucket(s.PrivateBytes)));
                tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagProcessHandles, HandleBucket(s.Handles)));
                if (_hasBaselineHandles)
                    context["handle_growth"] = (long)s.Handles - _baselineHandles;
            }
            if (s.HasSystem)
            {
                context["system_commit_used_bytes"] = (long)s.CommitUsedBytes;
                context["system_commit_limit_bytes"] = (long)s.CommitLimitBytes;
                context["system_physical_load_percent"] = (long)s.PhysicalLoadPercent;
                if (s.CommitLimitBytes > 0)
                {
                    tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagSystemCommitLoad,
                        PercentBucket(s.CommitUsedBytes, s.CommitLimitBytes)));
                }
                if (s.HasProcess && s.CommitUsedBytes > 0)
                {
                    /* Private bytes only; pagefile-backed sections the process holds are in
                       process.section_commit_share */
                    tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagProcessCommitShare,
                        PercentBucket(s.PrivateBytes, s.CommitUsedBytes)));
                    if (s.HasCensus && s.Census.HasSectionSizes)
                    {
                        tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagProcessSectionCommitShare,
                            PercentBucket(s.PrivateBytes + (ulong)Math.Max(0L, s.Census.PagefileSectionBytes),
                                s.CommitUsedBytes)));
                    }
                }
                if (_hasBaselineCommit)
                {
                    long growth = CommitGrowth(s);
                    context["system_commit_growth_bytes"] = growth;
                    tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagSystemCommitGrowth, GrowthBucket(growth)));
                }
            }
            context["baseline_age_seconds"] =
                (long)((Stopwatch.GetTimestamp() - _baselineTimestamp) / (double)Stopwatch.Frequency);
            if (s.Video != null)
            {
                context["gpu_count"] = (long)s.Video.Count;
                ulong worstUsage = 0;
                ulong worstBudget = 0;
                double worstRatio = -1.0;
                for (int i = 0; i < s.Video.Count; i++)
                {
                    VideoMemorySample v = s.Video[i];
                    string prefix = "gpu" + i.ToString(CultureInfo.InvariantCulture) + "_";
                    context[prefix + "name"] = v.Name;
                    context[prefix + "luid"] = "0x" + v.Luid.ToString("X16", CultureInfo.InvariantCulture);
                    context[prefix + "local_budget_bytes"] = (long)v.LocalBudgetBytes;
                    context[prefix + "local_usage_bytes"] = (long)v.LocalUsageBytes;
                    context[prefix + "nonlocal_budget_bytes"] = (long)v.NonLocalBudgetBytes;
                    context[prefix + "nonlocal_usage_bytes"] = (long)v.NonLocalUsageBytes;
                    KeepWorstSegment(v.LocalUsageBytes, v.LocalBudgetBytes,
                        ref worstUsage, ref worstBudget, ref worstRatio);
                    KeepWorstSegment(v.NonLocalUsageBytes, v.NonLocalBudgetBytes,
                        ref worstUsage, ref worstBudget, ref worstRatio);
                }
                if (worstBudget > 0)
                {
                    tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagGpuBudgetLoad,
                        PercentBucket(worstUsage, worstBudget)));
                }
            }
            context["decoded_bitmap_bytes"] = (long)GHApp.UsedBitmapBytes;
            context["gc_gen0_collections"] = (long)s.Gen0Collections;
            context["gc_gen1_collections"] = (long)s.Gen1Collections;
            context["gc_gen2_collections"] = (long)s.Gen2Collections;
            if (s.HasWindow)
            {
                context["window_client_width"] = (long)s.WindowWidth;
                context["window_client_height"] = (long)s.WindowHeight;
                context["window_state"] = s.WindowState;
                context["window_foreground"] = s.WindowForeground;
                tags.Add(new KeyValuePair<string, string>(GHConstants.SentryTagWindowState, s.WindowState));
            }
            context["windowed_mode"] = GHApp.WindowedMode;
            if (s.HasCensus)
            {
                HandleCensus c = s.Census;
                context["census_milliseconds"] = Math.Round(c.Milliseconds, 1);
                context["handle_types_top"] = c.TopTypes;
                context["section_handles"] = (long)c.SectionHandles;
                context["dxgk_shared_resource_handles"] = (long)c.DxgkSharedResourceHandles;
                if (c.HasSectionSizes)
                {
                    context["window_sections"] = (long)c.WindowSections;
                    context["window_section_bytes"] = c.WindowSectionBytes;
                    context["window_reserved_sections"] = (long)c.WindowReservedSections;
                    context["large_pagefile_sections"] = (long)c.LargePagefileSections;
                    context["large_pagefile_section_bytes"] = c.LargePagefileSectionBytes;
                    context["pagefile_section_bytes"] = c.PagefileSectionBytes;
                    context["large_reserved_sections"] = (long)c.LargeReservedSections;
                    context["large_reserved_section_bytes"] = c.LargeReservedSectionBytes;
                    context["unreadable_sections"] = (long)c.UnreadableSections;
                }
            }
            else
            {
                context["census_available"] = false;
            }
            RenderSurfaceDiagnostics.AddToContext(context);
            return context;
        }

        private static Sample TakeSample()
        {
            Sample s = new Sample();
            IntPtr process = GetCurrentProcess();

            PROCESS_MEMORY_COUNTERS_EX counters = new PROCESS_MEMORY_COUNTERS_EX();
            counters.cb = (uint)Marshal.SizeOf<PROCESS_MEMORY_COUNTERS_EX>();
            if (K32GetProcessMemoryInfo(process, ref counters, counters.cb))
            {
                s.HasProcess = true;
                s.PrivateBytes = counters.PrivateUsage;
                s.PeakPrivateBytes = counters.PeakPagefileUsage;
                s.WorkingSetBytes = counters.WorkingSetSize;
                s.PeakWorkingSetBytes = counters.PeakWorkingSetSize;
                uint handles;
                if (GetProcessHandleCount(process, out handles))
                    s.Handles = handles;
                s.GdiObjects = GetGuiResources(process, GR_GDIOBJECTS);
                s.UserObjects = GetGuiResources(process, GR_USEROBJECTS);
                s.PeakGdiObjects = GetGuiResources(process, GR_GDIOBJECTS_PEAK);
                s.PeakUserObjects = GetGuiResources(process, GR_USEROBJECTS_PEAK);
            }

            MEMORYSTATUSEX status = new MEMORYSTATUSEX();
            if (GlobalMemoryStatusEx(status))
            {
                s.HasSystem = true;
                s.CommitLimitBytes = status.ullTotalPageFile;
                s.CommitUsedBytes = status.ullTotalPageFile >= status.ullAvailPageFile
                    ? status.ullTotalPageFile - status.ullAvailPageFile : 0;
                s.PhysicalLoadPercent = status.dwMemoryLoad;
            }

            s.Gen0Collections = GC.CollectionCount(0);
            s.Gen1Collections = GC.CollectionCount(1);
            s.Gen2Collections = GC.CollectionCount(2);

            SampleWindow(ref s);
            long windowBytes = s.HasWindow ? 4L * s.WindowWidth * s.WindowHeight : 0;
            HandleCensus census;
            if (HandleCensusWindows.TryTake(true, windowBytes, out census))
            {
                s.HasCensus = true;
                s.Census = census;
            }

            List<VideoMemorySample> video = new List<VideoMemorySample>(2);
            if (VideoMemoryWindows.TrySample(video))
                s.Video = video;
            return s;
        }

        /* A minimized window's client rect is empty, so the last nonzero size is reported */
        private static void SampleWindow(ref Sample s)
        {
            IntPtr hwnd = Interlocked.CompareExchange(ref _mainWindowHandle, IntPtr.Zero, IntPtr.Zero);
            if (hwnd == IntPtr.Zero || !IsWindow(hwnd))
                return;
            bool minimized = IsIconic(hwnd);
            RECT rect;
            if (!minimized && GetClientRect(hwnd, out rect) && rect.Right > rect.Left && rect.Bottom > rect.Top)
            {
                long packed = ((long)(rect.Right - rect.Left) << 32) | (uint)(rect.Bottom - rect.Top);
                Interlocked.Exchange(ref _lastClientSize, packed);
            }
            long size = Interlocked.Read(ref _lastClientSize);
            s.HasWindow = true;
            s.WindowWidth = (int)(size >> 32);
            s.WindowHeight = (int)(size & 0xFFFFFFFFL);
            s.WindowState = minimized ? "minimized" : IsZoomed(hwnd) ? "maximized" : "normal";
            s.WindowForeground = GetForegroundWindow() == hwnd;
        }

        private static long CommitGrowth(Sample s)
        {
            return (long)s.CommitUsedBytes - (long)_baselineCommitUsedBytes;
        }

        private static string BuildBreadcrumbText(Sample s, GCMemoryInfo gcInfo, long managedBytes)
        {
            StringBuilder sb = new StringBuilder(512);
            sb.Append("Process memory: ");
            if (s.HasProcess)
            {
                sb.Append("handles ").Append(s.Handles);
                if (_hasBaselineHandles)
                    sb.Append(" (").Append(FormatSigned((long)s.Handles - _baselineHandles)).Append(')');
            }
            else
            {
                sb.Append("process counters unavailable");
            }
            if (s.HasSystem)
            {
                sb.Append(", commit ").Append(FormatGiB(s.CommitUsedBytes))
                    .Append('/').Append(FormatGiB(s.CommitLimitBytes)).Append(" GB");
                if (_hasBaselineCommit)
                    sb.Append(" (").Append(FormatSignedGiB(CommitGrowth(s))).Append(')');
            }
            else
            {
                sb.Append(", system counters unavailable");
            }
            if (s.HasCensus)
            {
                HandleCensus c = s.Census;
                if (c.HasSectionSizes)
                {
                    if (s.HasWindow && s.WindowWidth > 0 && s.WindowHeight > 0)
                    {
                        sb.Append(", window sections ").Append(c.WindowSections)
                            .Append(" (").Append(FormatGiB((ulong)Math.Max(0L, c.WindowSectionBytes))).Append(" GB)");
                        if (c.WindowReservedSections > 0)
                            sb.Append(" (+").Append(c.WindowReservedSections).Append(" reserved)");
                    }
                    sb.Append(", large ").Append(c.LargePagefileSections);
                }
                else
                {
                    sb.Append(", sections ").Append(c.SectionHandles);
                }
                sb.Append(", dxgk ").Append(c.DxgkSharedResourceHandles);
            }
            else
            {
                sb.Append(", census unavailable");
            }
            sb.Append(", GC ").Append(s.Gen0Collections).Append('/').Append(s.Gen1Collections)
                .Append('/').Append(s.Gen2Collections);
            if (s.HasWindow)
            {
                sb.Append(", window ").Append(s.WindowWidth).Append('x').Append(s.WindowHeight)
                    .Append(' ').Append(s.WindowState)
                    .Append(s.WindowForeground ? " foreground" : " background");
            }
            if (s.HasProcess)
            {
                sb.Append(", private ").Append(s.PrivateBytes / MiB)
                    .Append(" MB, working set ").Append(s.WorkingSetBytes / MiB).Append(" MB");
            }
            sb.Append(", managed ").Append((ulong)managedBytes / MiB)
                .Append(" MB, finalizers ").Append(gcInfo.FinalizationPendingCount);
            if (s.HasProcess)
                sb.Append(", GDI ").Append(s.GdiObjects).Append(", USER ").Append(s.UserObjects);
            if (s.HasSystem)
                sb.Append(", physical load ").Append(s.PhysicalLoadPercent).Append('%');
            if (s.HasCensus)
            {
                sb.Append(", census ")
                    .Append(s.Census.Milliseconds.ToString("F0", CultureInfo.InvariantCulture)).Append(" ms");
            }
            if (s.Video != null)
            {
                for (int i = 0; i < s.Video.Count; i++)
                {
                    VideoMemorySample v = s.Video[i];
                    sb.Append(", gpu").Append(i).Append(' ').Append(v.Name)
                        .Append(": local ").Append(FormatGiB(v.LocalUsageBytes))
                        .Append('/').Append(FormatGiB(v.LocalBudgetBytes))
                        .Append(" GB, non-local ").Append(FormatGiB(v.NonLocalUsageBytes))
                        .Append('/').Append(FormatGiB(v.NonLocalBudgetBytes)).Append(" GB");
                }
            }
            return sb.ToString();
        }

        /* Keeps the usage and budget pair with the largest usage/budget; a zero budget never counts */
        private static void KeepWorstSegment(ulong usage, ulong budget, ref ulong worstUsage, ref ulong worstBudget,
            ref double worstRatio)
        {
            if (budget == 0)
                return;
            double ratio = (double)usage / budget;
            if (ratio > worstRatio)
            {
                worstRatio = ratio;
                worstUsage = usage;
                worstBudget = budget;
            }
        }

        private static string FormatGiB(ulong bytes)
        {
            return ((double)bytes / GiB).ToString("F1", CultureInfo.InvariantCulture);
        }

        private static string FormatSignedGiB(long bytes)
        {
            return (bytes < 0 ? "-" : "+") + ((double)Math.Abs(bytes) / GiB).ToString("F1", CultureInfo.InvariantCulture);
        }

        private static string FormatSigned(long value)
        {
            return (value < 0 ? "" : "+") + value.ToString(CultureInfo.InvariantCulture);
        }

        private static string SizeBucket(ulong bytes)
        {
            if (bytes < 256 * MiB)
                return "<256MB";
            if (bytes < GiB)
                return "256MB-1GB";
            if (bytes < 4 * GiB)
                return "1-4GB";
            if (bytes < 16 * GiB)
                return "4-16GB";
            return "16GB+";
        }

        private static string HandleBucket(uint handles)
        {
            if (handles < 5000)
                return "<5k";
            if (handles < 20000)
                return "5-20k";
            return "20k+";
        }

        /* A negative growth counts as <4GB */
        private static string GrowthBucket(long bytes)
        {
            if (bytes < 4L * (long)GiB)
                return "<4GB";
            if (bytes < 16L * (long)GiB)
                return "4-16GB";
            if (bytes < 64L * (long)GiB)
                return "16-64GB";
            return "64GB+";
        }

        private static string PercentBucket(ulong part, ulong whole)
        {
            double percent = 100.0 * part / whole;
            if (percent < 25.0)
                return "<25%";
            if (percent < 50.0)
                return "25-50%";
            if (percent < 75.0)
                return "50-75%";
            if (percent < 90.0)
                return "75-90%";
            return "90%+";
        }
    }
}
#endif

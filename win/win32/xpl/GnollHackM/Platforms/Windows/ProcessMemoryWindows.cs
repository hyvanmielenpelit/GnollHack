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
       through VideoMemoryWindows. */
    internal static class ProcessMemoryWindows
    {
        private const int FirstSampleDelayMilliseconds = 60 * 1000;
        private const int SampleIntervalMilliseconds = 30 * 60 * 1000;
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
        }

        private static Timer _timer;

        public static void Start()
        {
            if (_timer != null)
                return;
            _timer = new Timer(OnTimer, null, FirstSampleDelayMilliseconds, SampleIntervalMilliseconds);
            /* Builds the DXGI adapter cache before the first event or breadcrumb needs it */
            ThreadPool.QueueUserWorkItem(delegate { VideoMemoryWindows.TrySample(new List<VideoMemorySample>(2)); });
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
                    sentryEvent.SetTag(GHConstants.SentryTagProcessPrivate, SizeBucket(s.PrivateBytes));
                }
                if (s.HasSystem)
                {
                    context["system_commit_used_bytes"] = (long)s.CommitUsedBytes;
                    context["system_commit_limit_bytes"] = (long)s.CommitLimitBytes;
                    context["system_physical_load_percent"] = (long)s.PhysicalLoadPercent;
                    if (s.CommitLimitBytes > 0)
                    {
                        sentryEvent.SetTag(GHConstants.SentryTagSystemCommitLoad,
                            PercentBucket(s.CommitUsedBytes, s.CommitLimitBytes));
                    }
                    if (s.HasProcess && s.CommitUsedBytes > 0)
                    {
                        sentryEvent.SetTag(GHConstants.SentryTagProcessCommitShare,
                            PercentBucket(s.PrivateBytes, s.CommitUsedBytes));
                    }
                }
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
                        sentryEvent.SetTag(GHConstants.SentryTagGpuBudgetLoad, PercentBucket(worstUsage, worstBudget));
                }
                context["decoded_bitmap_bytes"] = (long)GHApp.UsedBitmapBytes;
                sentryEvent.Contexts[GHConstants.SentryContextProcessMemory] = context;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
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

            List<VideoMemorySample> video = new List<VideoMemorySample>(2);
            if (VideoMemoryWindows.TrySample(video))
                s.Video = video;
            return s;
        }

        private static string BuildBreadcrumbText(Sample s, GCMemoryInfo gcInfo, long managedBytes)
        {
            StringBuilder sb = new StringBuilder(512);
            sb.Append("Process memory: ");
            if (s.HasProcess)
            {
                sb.Append("private ").Append(s.PrivateBytes / MiB)
                    .Append(" MB (peak ").Append(s.PeakPrivateBytes / MiB)
                    .Append("), working set ").Append(s.WorkingSetBytes / MiB)
                    .Append(" MB (peak ").Append(s.PeakWorkingSetBytes / MiB)
                    .Append("), handles ").Append(s.Handles)
                    .Append(", GDI ").Append(s.GdiObjects).Append(" (peak ").Append(s.PeakGdiObjects)
                    .Append("), USER ").Append(s.UserObjects).Append(" (peak ").Append(s.PeakUserObjects)
                    .Append(')');
            }
            else
            {
                sb.Append("process counters unavailable");
            }
            sb.Append(", managed ").Append((ulong)managedBytes / MiB)
                .Append(" MB, finalizers pending ").Append(gcInfo.FinalizationPendingCount);
            if (s.HasSystem)
            {
                sb.Append(", commit ").Append(FormatGiB(s.CommitUsedBytes))
                    .Append('/').Append(FormatGiB(s.CommitLimitBytes))
                    .Append(" GB, physical load ").Append(s.PhysicalLoadPercent).Append('%');
            }
            else
            {
                sb.Append(", system counters unavailable");
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

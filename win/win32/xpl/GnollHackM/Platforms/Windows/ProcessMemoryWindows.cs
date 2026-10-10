#if WINDOWS && GNH_MAUI && SENTRY
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using GnollHackX;
using Sentry;

namespace GnollHackM
{
    /* Process and system memory figures for Windows Sentry events, and a periodic breadcrumb.
       Samples come from direct Win32 calls, which neither enumerate other processes nor allocate
       much, so they still work when the system is short of memory. */
    internal static class ProcessMemoryWindows
    {
        private const int FirstSampleDelayMilliseconds = 60 * 1000;
        private const int SampleIntervalMilliseconds = 30 * 60 * 1000;
        private const string ContextName = "Process Memory";
        private const uint GR_GDIOBJECTS = 0;
        private const uint GR_USEROBJECTS = 1;
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
        private static extern bool K32GetProcessMemoryInfo(IntPtr process, ref PROCESS_MEMORY_COUNTERS_EX counters, uint cb);

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
            public bool HasSystem;
            public ulong CommitUsedBytes;
            public ulong CommitLimitBytes;
            public uint PhysicalLoadPercent;
        }

        private static Timer _timer;

        public static void Start()
        {
            if (_timer != null)
                return;
            _timer = new Timer(OnTimer, null, FirstSampleDelayMilliseconds, SampleIntervalMilliseconds);
        }

        private static void OnTimer(object state)
        {
            try
            {
                Sample s = TakeSample();
                GCMemoryInfo gcInfo = GC.GetGCMemoryInfo();
                long managedBytes = GC.GetTotalMemory(false);
                string text = "Process memory: private " + (s.PrivateBytes / MiB) + " MB (peak " + (s.PeakPrivateBytes / MiB)
                    + "), working set " + (s.WorkingSetBytes / MiB) + " MB (peak " + (s.PeakWorkingSetBytes / MiB)
                    + "), managed " + ((ulong)managedBytes / MiB) + " MB, finalizers pending " + gcInfo.FinalizationPendingCount
                    + ", handles " + s.Handles + ", GDI " + s.GdiObjects + ", USER " + s.UserObjects
                    + ", commit " + FormatGiB(s.CommitUsedBytes) + "/" + FormatGiB(s.CommitLimitBytes) + " GB"
                    + ", physical load " + s.PhysicalLoadPercent + "%";
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
                    sentryEvent.SetTag("process.private", SizeBucket(s.PrivateBytes));
                }
                if (s.HasSystem)
                {
                    context["system_commit_used_bytes"] = (long)s.CommitUsedBytes;
                    context["system_commit_limit_bytes"] = (long)s.CommitLimitBytes;
                    context["system_physical_load_percent"] = (long)s.PhysicalLoadPercent;
                    if (s.CommitLimitBytes > 0)
                        sentryEvent.SetTag("system.commit_load", PercentBucket(s.CommitUsedBytes, s.CommitLimitBytes));
                    if (s.HasProcess && s.CommitUsedBytes > 0)
                        sentryEvent.SetTag("process.commit_share", PercentBucket(s.PrivateBytes, s.CommitUsedBytes));
                }
                context["decoded_bitmap_bytes"] = (long)GHApp.UsedBitmapBytes;
                sentryEvent.Contexts[ContextName] = context;
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
            }

            MEMORYSTATUSEX status = new MEMORYSTATUSEX();
            if (GlobalMemoryStatusEx(status))
            {
                s.HasSystem = true;
                s.CommitLimitBytes = status.ullTotalPageFile;
                s.CommitUsedBytes = status.ullTotalPageFile >= status.ullAvailPageFile ? status.ullTotalPageFile - status.ullAvailPageFile : 0;
                s.PhysicalLoadPercent = status.dwMemoryLoad;
            }
            return s;
        }

        private static string FormatGiB(ulong bytes)
        {
            return ((double)bytes / GiB).ToString("F1", System.Globalization.CultureInfo.InvariantCulture);
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

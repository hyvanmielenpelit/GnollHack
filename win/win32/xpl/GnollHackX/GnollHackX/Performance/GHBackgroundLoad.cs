using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace GnollHackX.Performance
{
    /* Background load around a measurement window: the whole-machine sample series, its
       summary over a time range, the named processes active in it, and the verdict
       (quiet, elevated, busy) drawn from pre-registered thresholds. Shared by the in-app
       sampler and the offline analyzer, so that both classify the same data the same way.

       The file must compile under C# 7.3 (the legacy netstandard2.0 project) and must not
       depend on LINQ, JSON, GHApp, GHConstants or any MAUI type. */

    /* One whole-machine sample. Timestamps are TimeSpan ticks (100 ns) from an origin
       shared by every sample and range of a series. Float signals are NaN when the
       platform does not report them; AvailableMemoryMB is -1 then. MemoryPressureEvents
       is a cumulative counter, so a range's event count is a difference of two samples. */
    public struct GHSystemLoadSample
    {
        public long TimestampTicks;
        public float SystemCpuPct;          /* whole machine, of total logical capacity */
        public float OwnCpuPct;             /* this process, of total logical capacity */
        public float DiskBusyPct;           /* 100 - % Idle Time */
        public float AvailableMemoryPct;    /* of total physical memory */
        public float HardFaultsPerSec;      /* hard page reads */
        public long AvailableMemoryMB;
        public bool LowMemory;
        public int MemoryPressureEvents;

        /* Machine CPU not spent by this process, never negative; NaN when either input
           is NaN. */
        public float OtherCpuPct
        {
            get
            {
                if (float.IsNaN(SystemCpuPct) || float.IsNaN(OwnCpuPct))
                    return float.NaN;
                return Math.Max(0f, SystemCpuPct - OwnCpuPct);
            }
        }

        /* A sample with every signal unsupported. */
        public static GHSystemLoadSample Empty
        {
            get
            {
                GHSystemLoadSample s = new GHSystemLoadSample();
                s.SystemCpuPct = float.NaN;
                s.OwnCpuPct = float.NaN;
                s.DiskBusyPct = float.NaN;
                s.AvailableMemoryPct = float.NaN;
                s.HardFaultsPerSec = float.NaN;
                s.AvailableMemoryMB = -1;
                return s;
            }
        }
    }

    /* One named process over a window. CpuPct is of total logical capacity; GpuPct is
       NaN when the platform reports no per-process GPU use. */
    public struct GHProcessLoad
    {
        public string Name;
        public float CpuPct;
        public float GpuPct;
        public string Category;

        public GHProcessLoad(string name, float cpuPct, float gpuPct)
        {
            Name = name;
            CpuPct = cpuPct;
            GpuPct = gpuPct;
            Category = GHBackgroundLoad.CategoryOf(name);
        }
    }

    /* The known processes of one category active in a window, with their summed CPU. */
    public sealed class GHBackgroundActivity
    {
        public string Category;
        public readonly List<string> Processes = new List<string>();
        public float CpuPct;
    }

    /* The summary of a sample series over a time range: the "window" and "preWindow"
       objects of a run record's background block. Signals with no data are NaN, and
       AvailableMemoryMinMB is -1. Coverage is the fraction of the expected samples
       present; a summary rebuilt from a record takes the record's top-level coverage. */
    public sealed class GHBackgroundSummary
    {
        public int Samples;
        public float OtherCpuMeanPct = float.NaN;
        public float OtherCpuP90Pct = float.NaN;
        public float OtherCpuMaxPct = float.NaN;
        public float OtherCpuSpikeShare = float.NaN;
        public float DiskBusyMeanPct = float.NaN;
        public float DiskBusyP90Pct = float.NaN;
        public float AvailableMemoryMinPct = float.NaN;
        public long AvailableMemoryMinMB = -1;
        public float HardFaultsP90PerSec = float.NaN;
        public int MemoryPressureEvents;
        public bool LowMemory;
        public float Coverage = 1f;

        public bool HasCpuSignal
        {
            get { return !float.IsNaN(OtherCpuP90Pct); }
        }

        public bool HasMemorySignal
        {
            get { return !float.IsNaN(AvailableMemoryMinPct) || AvailableMemoryMinMB >= 0; }
        }
    }

    /* Ordered from least to most severe after Unknown, so the worst known verdict of a
       list is its maximum. */
    public enum GHBackgroundVerdict
    {
        Unknown = 0,
        Quiet = 1,
        Elevated = 2,
        Busy = 3
    }

    public static class GHBackgroundLoad
    {
        public const int SamplerVersion = 1;
        public const int SampleIntervalMs = 1000;

        /* Verdict thresholds. "Other CPU" is machine CPU minus this process, as a
           percentage of total logical capacity. Elevated annotates a run; busy excludes
           it. Memory thresholds are strict (below), all others inclusive (at or above). */
        public const float ElevatedOtherCpuP90Pct = 10f;
        public const float BusyOtherCpuP90Pct = 25f;
        public const float SpikeOtherCpuPct = 50f;
        public const float BusySpikeShare = 0.10f;
        public const float ElevatedDiskBusyP90Pct = 50f;
        public const float ElevatedAvailableMemoryMinPct = 10f;
        public const float BusyAvailableMemoryMinPct = 5f;
        public const float ElevatedHardFaultsP90PerSec = 200f;
        public const float BusyHardFaultsP90PerSec = 1000f;
        public const float ElevatedOtherGpuPct = 10f;
        public const float BusyOtherGpuPct = 30f;
        public const float KnownActivityMinCpuPct = 2f;
        public const int ElevatedMemoryPressureEvents = 1;

        /* Below this coverage the verdict is Unknown. */
        public const float MinCoverage = 0.5f;

        /* Quiet gate: the last QuietWindowSeconds must average other CPU and disk busy
           below these, polled once a second for at most QuietGateTimeoutSeconds. */
        public const int QuietWindowSeconds = 5;
        public const float QuietOtherCpuPct = 10f;
        public const float QuietDiskBusyPct = 50f;
        public const int QuietGateTimeoutSeconds = 120;

        /* CPU samples the quiet window needs before it can pass: MinCoverage of the
           samples QuietWindowSeconds holds at SampleIntervalMs. */
        public const int QuietMinSamples = 3;

        public const int MaxReasonLength = 100;
        public const string ReasonPrefix = "background load: ";

        public const string VerdictUnknownName = "unknown";
        public const string VerdictQuietName = "quiet";
        public const string VerdictElevatedName = "elevated";
        public const string VerdictBusyName = "busy";

        public const string CategoryWindowsUpdate = "windows-update";
        public const string CategoryAntivirus = "antivirus";
        public const string CategoryIndexer = "indexer";
        public const string CategoryWslVm = "wsl-vm";
        public const string CategoryBuildTools = "build-tools";
        public const string CategorySync = "sync";
        public const string CategoryTelemetry = "telemetry";
        public const string CategoryMeasurement = "measurement";
        public const string CategoryCompositor = "compositor";
        public const string CategoryOther = "other";

        private static readonly Dictionary<string, string> KnownProcesses = BuildKnownProcesses();

        private static Dictionary<string, string> BuildKnownProcesses()
        {
            Dictionary<string, string> d = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            AddCategory(d, CategoryWindowsUpdate, new string[] { "TiWorker", "TrustedInstaller", "MoUsoCoreWorker",
                "usocoreworker", "wuauclt", "WaaSMedicAgent", "SIHClient", "msiexec" });
            AddCategory(d, CategoryAntivirus, new string[] { "MsMpEng", "MpCmdRun", "NisSrv", "MpDefenderCoreService" });
            AddCategory(d, CategoryIndexer, new string[] { "SearchIndexer", "SearchProtocolHost", "SearchFilterHost" });
            AddCategory(d, CategoryWslVm, new string[] { "vmmem", "vmmemWSL", "VmmemWSA", "vmwp" });
            AddCategory(d, CategoryBuildTools, new string[] { "devenv", "MSBuild", "VBCSCompiler", "cl", "link",
                "clang", "lld-link", "dotnet", "ServiceHub.Host.dotnet.x64", "ServiceHub.RoslynCodeAnalysisService" });
            AddCategory(d, CategorySync, new string[] { "OneDrive", "Dropbox", "GoogleDriveFS" });
            AddCategory(d, CategoryTelemetry, new string[] { "CompatTelRunner", "DiagTrack" });
            AddCategory(d, CategoryMeasurement, new string[] { "PresentMon", "typeperf", "powershell", "pwsh", "adb" });
            AddCategory(d, CategoryCompositor, new string[] { "dwm" });
            return d;
        }

        /* True for the categories that are never an activity or a suspect: the measurement
           tooling, and the desktop compositor, whose load follows the frames this app
           presents. */
        public static bool IsIgnoredCategory(string category)
        {
            return category == CategoryMeasurement || category == CategoryCompositor;
        }

        /* Other processes' GPU use minus the compositor's, never negative; NaN when
           otherGpuPct is NaN. The compositor rows' GPU is subtracted as reported, so a
           compositor spread over several adapters can take the result to 0. */
        public static float OtherGpuWithoutCompositor(float otherGpuPct, IList<GHProcessLoad> processes)
        {
            if (float.IsNaN(otherGpuPct))
                return float.NaN;
            float compositor = 0f;
            if (processes != null)
            {
                for (int i = 0; i < processes.Count; i++)
                {
                    /* By name too: older records store the compositor as "other" */
                    GHProcessLoad p = processes[i];
                    if (EffectiveCategory(p) == CategoryCompositor || CategoryOf(p.Name) == CategoryCompositor)
                        compositor += OrZero(p.GpuPct);
                }
            }
            return Math.Max(0f, otherGpuPct - compositor);
        }

        private static void AddCategory(Dictionary<string, string> d, string category, string[] names)
        {
            for (int i = 0; i < names.Length; i++)
                d[names[i]] = category;
        }

        public static string VerdictName(GHBackgroundVerdict verdict)
        {
            switch (verdict)
            {
            case GHBackgroundVerdict.Quiet:
                return VerdictQuietName;
            case GHBackgroundVerdict.Elevated:
                return VerdictElevatedName;
            case GHBackgroundVerdict.Busy:
                return VerdictBusyName;
            default:
                return VerdictUnknownName;
            }
        }

        /* The verdict a VerdictName string names; Unknown for anything else. */
        public static GHBackgroundVerdict ParseVerdict(string name)
        {
            if (name == VerdictQuietName)
                return GHBackgroundVerdict.Quiet;
            if (name == VerdictElevatedName)
                return GHBackgroundVerdict.Elevated;
            if (name == VerdictBusyName)
                return GHBackgroundVerdict.Busy;
            return GHBackgroundVerdict.Unknown;
        }

        /* The known-process category of a process name, matched case-insensitively and
           exactly after stripping a trailing "#N" instance suffix and a trailing ".exe";
           CategoryOther when the name is not in the table. */
        public static string CategoryOf(string processName)
        {
            if (string.IsNullOrEmpty(processName))
                return CategoryOther;
            string name = processName.Trim();
            int hash = name.LastIndexOf('#');
            if (hash >= 0 && hash < name.Length - 1)
            {
                bool digits = true;
                for (int i = hash + 1; i < name.Length; i++)
                {
                    if (name[i] < '0' || name[i] > '9')
                    {
                        digits = false;
                        break;
                    }
                }
                if (digits)
                    name = name.Substring(0, hash);
            }
            if (name.EndsWith(".exe", StringComparison.OrdinalIgnoreCase))
                name = name.Substring(0, name.Length - 4);
            string category;
            if (KnownProcesses.TryGetValue(name, out category))
                return category;
            return CategoryOther;
        }

        private static string EffectiveCategory(GHProcessLoad p)
        {
            return string.IsNullOrEmpty(p.Category) ? CategoryOf(p.Name) : p.Category;
        }

        private static float OrZero(float v)
        {
            return float.IsNaN(v) ? 0f : v;
        }

        /* The known activities among processes: one entry per category other than
           CategoryOther and the ignored categories (IsIgnoredCategory), with its process
           names and summed CPU, ordered by CPU descending, then category. */
        public static List<GHBackgroundActivity> BuildActivities(IList<GHProcessLoad> processes)
        {
            List<GHBackgroundActivity> result = new List<GHBackgroundActivity>();
            if (processes == null)
                return result;
            for (int i = 0; i < processes.Count; i++)
            {
                GHProcessLoad p = processes[i];
                string category = EffectiveCategory(p);
                if (category == CategoryOther || IsIgnoredCategory(category))
                    continue;
                GHBackgroundActivity activity = null;
                for (int j = 0; j < result.Count; j++)
                {
                    if (result[j].Category == category)
                    {
                        activity = result[j];
                        break;
                    }
                }
                if (activity == null)
                {
                    activity = new GHBackgroundActivity();
                    activity.Category = category;
                    result.Add(activity);
                }
                activity.Processes.Add(p.Name);
                activity.CpuPct += OrZero(p.CpuPct);
            }
            result.Sort(CompareActivities);
            return result;
        }

        private static int CompareActivities(GHBackgroundActivity x, GHBackgroundActivity y)
        {
            int c = y.CpuPct.CompareTo(x.CpuPct);
            if (c != 0)
                return c;
            return string.CompareOrdinal(x.Category, y.Category);
        }

        /* Summarizes the samples whose timestamps fall in [fromTicks, toTicks). The
           samples need not be in order. Each signal skips its NaN values; percentiles
           are nearest-rank. Memory pressure events are the counter of the latest sample
           in range minus that of the earliest. expectedIntervalMs <= 0 means
           SampleIntervalMs. */
        public static GHBackgroundSummary Summarize(GHSystemLoadSample[] samples, int count, long fromTicks, long toTicks,
                                                    int expectedIntervalMs)
        {
            GHBackgroundSummary s = new GHBackgroundSummary();
            if (samples == null)
                count = 0;
            else if (count > samples.Length)
                count = samples.Length;
            if (count < 0)
                count = 0;

            float[] other = new float[count];
            float[] disk = new float[count];
            float[] faults = new float[count];
            int nOther = 0, nDisk = 0, nFaults = 0, nSpikes = 0;
            double otherSum = 0, diskSum = 0;
            float memMinPct = float.NaN;
            long memMinMB = -1;
            bool lowMemory = false;
            int inRange = 0;
            long firstTicks = 0, lastTicks = 0;
            int firstPressure = 0, lastPressure = 0;

            for (int i = 0; i < count; i++)
            {
                GHSystemLoadSample x = samples[i];
                if (x.TimestampTicks < fromTicks || x.TimestampTicks >= toTicks)
                    continue;

                if (inRange == 0 || x.TimestampTicks < firstTicks)
                {
                    firstTicks = x.TimestampTicks;
                    firstPressure = x.MemoryPressureEvents;
                }
                if (inRange == 0 || x.TimestampTicks >= lastTicks)
                {
                    lastTicks = x.TimestampTicks;
                    lastPressure = x.MemoryPressureEvents;
                }
                inRange++;

                float o = x.OtherCpuPct;
                if (!float.IsNaN(o))
                {
                    other[nOther++] = o;
                    otherSum += o;
                    if (o >= SpikeOtherCpuPct)
                        nSpikes++;
                }
                if (!float.IsNaN(x.DiskBusyPct))
                {
                    disk[nDisk++] = x.DiskBusyPct;
                    diskSum += x.DiskBusyPct;
                }
                if (!float.IsNaN(x.HardFaultsPerSec))
                    faults[nFaults++] = x.HardFaultsPerSec;
                if (!float.IsNaN(x.AvailableMemoryPct) && (float.IsNaN(memMinPct) || x.AvailableMemoryPct < memMinPct))
                    memMinPct = x.AvailableMemoryPct;
                if (x.AvailableMemoryMB >= 0 && (memMinMB < 0 || x.AvailableMemoryMB < memMinMB))
                    memMinMB = x.AvailableMemoryMB;
                if (x.LowMemory)
                    lowMemory = true;
            }

            s.Samples = inRange;
            if (nOther > 0)
            {
                Array.Sort(other, 0, nOther);
                s.OtherCpuMeanPct = (float)(otherSum / nOther);
                s.OtherCpuP90Pct = GHPerformanceStatistics.PercentileSorted(other, nOther, 90);
                s.OtherCpuMaxPct = other[nOther - 1];
                s.OtherCpuSpikeShare = (float)nSpikes / nOther;
            }
            if (nDisk > 0)
            {
                Array.Sort(disk, 0, nDisk);
                s.DiskBusyMeanPct = (float)(diskSum / nDisk);
                s.DiskBusyP90Pct = GHPerformanceStatistics.PercentileSorted(disk, nDisk, 90);
            }
            if (nFaults > 0)
            {
                Array.Sort(faults, 0, nFaults);
                s.HardFaultsP90PerSec = GHPerformanceStatistics.PercentileSorted(faults, nFaults, 90);
            }
            s.AvailableMemoryMinPct = memMinPct;
            s.AvailableMemoryMinMB = memMinMB;
            s.LowMemory = lowMemory;
            s.MemoryPressureEvents = inRange > 0 ? Math.Max(0, lastPressure - firstPressure) : 0;

            int intervalMs = expectedIntervalMs > 0 ? expectedIntervalMs : SampleIntervalMs;
            double expected = (double)(toTicks - fromTicks) / ((double)intervalMs * TimeSpan.TicksPerMillisecond);
            if (expected > 0)
                s.Coverage = (float)Math.Min(1.0, inRange / expected);
            else
                s.Coverage = inRange > 0 ? 1f : 0f;
            return s;
        }

        /* The quiet-gate test over the samples in (nowTicks - QuietWindowSeconds,
           nowTicks]: true when at least QuietMinSamples carry a CPU signal, their other
           CPU averages below QuietOtherCpuPct, and disk busy (ignored when unreported)
           averages below QuietDiskBusyPct. otherCpuMean is NaN, and the result false,
           when no sample in range carries a CPU signal. */
        public static bool IsQuiet(GHSystemLoadSample[] samples, int count, long nowTicks,
                                   out float otherCpuMean, out float diskBusyMean)
        {
            otherCpuMean = float.NaN;
            diskBusyMean = float.NaN;
            if (samples == null)
                return false;
            if (count > samples.Length)
                count = samples.Length;
            long fromTicks = nowTicks - QuietWindowSeconds * TimeSpan.TicksPerSecond;
            double otherSum = 0, diskSum = 0;
            int nOther = 0, nDisk = 0;
            for (int i = 0; i < count; i++)
            {
                GHSystemLoadSample x = samples[i];
                if (x.TimestampTicks <= fromTicks || x.TimestampTicks > nowTicks)
                    continue;
                float o = x.OtherCpuPct;
                if (!float.IsNaN(o))
                {
                    otherSum += o;
                    nOther++;
                }
                if (!float.IsNaN(x.DiskBusyPct))
                {
                    diskSum += x.DiskBusyPct;
                    nDisk++;
                }
            }
            if (nOther == 0)
                return false;
            otherCpuMean = (float)(otherSum / nOther);
            if (nDisk > 0)
                diskBusyMean = (float)(diskSum / nDisk);
            if (nOther < QuietMinSamples)
                return false;
            if (otherCpuMean >= QuietOtherCpuPct)
                return false;
            if (nDisk > 0 && diskBusyMean >= QuietDiskBusyPct)
                return false;
            return true;
        }

        /* The verdict for a summary and the processes of the same range. otherGpuPct is
           the other processes' 3D GPU engine average, NaN when unreported; the GPU rules
           use it without the compositor's share (OtherGpuWithoutCompositor). Unknown when
           coverage is below MinCoverage, or when there is no CPU signal and no memory
           rule (low memory, available memory, hard faults, memory pressure) fires.
           Otherwise Busy when a busy rule fires, else Elevated when an elevated rule
           fires, else Quiet. reason is null unless the verdict is Elevated or Busy; it
           then names the facts of that level and up to two suspect processes, never one
           of an ignored category (IsIgnoredCategory), in at most MaxReasonLength
           characters. */
        public static GHBackgroundVerdict Classify(GHBackgroundSummary s, IList<GHProcessLoad> processes, float otherGpuPct,
                                                   out string reason)
        {
            reason = null;
            if (s == null || float.IsNaN(s.Coverage) || s.Coverage < MinCoverage)
                return GHBackgroundVerdict.Unknown;

            List<string> busy = new List<string>();
            List<string> elevated = new List<string>();
            bool memoryRuleFired = false;

            if (!float.IsNaN(s.OtherCpuP90Pct))
            {
                string fact = "other CPU P90 " + Pct(s.OtherCpuP90Pct) + " %";
                if (s.OtherCpuP90Pct >= BusyOtherCpuP90Pct)
                    busy.Add(fact);
                else if (s.OtherCpuP90Pct >= ElevatedOtherCpuP90Pct)
                    elevated.Add(fact);
            }
            if (!float.IsNaN(s.OtherCpuSpikeShare) && s.OtherCpuSpikeShare >= BusySpikeShare)
                busy.Add("other CPU >= " + Pct(SpikeOtherCpuPct) + " % in " + Pct(s.OtherCpuSpikeShare * 100f) + " % of samples");
            float appGpuPct = OtherGpuWithoutCompositor(otherGpuPct, processes);
            if (!float.IsNaN(appGpuPct))
            {
                string fact = "other GPU " + Pct(appGpuPct) + " %";
                if (appGpuPct >= BusyOtherGpuPct)
                    busy.Add(fact);
                else if (appGpuPct >= ElevatedOtherGpuPct)
                    elevated.Add(fact);
            }
            if (s.LowMemory)
            {
                busy.Add("low memory");
                memoryRuleFired = true;
            }
            if (!float.IsNaN(s.AvailableMemoryMinPct))
            {
                string fact = "available memory min " + Pct(s.AvailableMemoryMinPct) + " %";
                if (s.AvailableMemoryMinPct < BusyAvailableMemoryMinPct)
                {
                    busy.Add(fact);
                    memoryRuleFired = true;
                }
                else if (s.AvailableMemoryMinPct < ElevatedAvailableMemoryMinPct)
                {
                    elevated.Add(fact);
                    memoryRuleFired = true;
                }
            }
            if (!float.IsNaN(s.HardFaultsP90PerSec))
            {
                string fact = "hard faults P90 " + Pct(s.HardFaultsP90PerSec) + "/s";
                if (s.HardFaultsP90PerSec >= BusyHardFaultsP90PerSec)
                {
                    busy.Add(fact);
                    memoryRuleFired = true;
                }
                else if (s.HardFaultsP90PerSec >= ElevatedHardFaultsP90PerSec)
                {
                    elevated.Add(fact);
                    memoryRuleFired = true;
                }
            }
            if (s.MemoryPressureEvents >= ElevatedMemoryPressureEvents)
            {
                elevated.Add("memory pressure events " + s.MemoryPressureEvents.ToString(CultureInfo.InvariantCulture));
                memoryRuleFired = true;
            }
            if (!float.IsNaN(s.DiskBusyP90Pct) && s.DiskBusyP90Pct >= ElevatedDiskBusyP90Pct)
                elevated.Add("disk busy P90 " + Pct(s.DiskBusyP90Pct) + " %");

            List<GHBackgroundActivity> activities = BuildActivities(processes);
            for (int i = 0; i < activities.Count; i++)
            {
                if (activities[i].CpuPct >= KnownActivityMinCpuPct)
                    elevated.Add(activities[i].Category + " " + Pct(activities[i].CpuPct) + " %");
            }

            if (!s.HasCpuSignal && !memoryRuleFired)
                return GHBackgroundVerdict.Unknown;

            GHBackgroundVerdict verdict;
            List<string> facts;
            if (busy.Count > 0)
            {
                verdict = GHBackgroundVerdict.Busy;
                facts = busy;
            }
            else if (elevated.Count > 0)
            {
                verdict = GHBackgroundVerdict.Elevated;
                facts = elevated;
            }
            else
            {
                return GHBackgroundVerdict.Quiet;
            }

            reason = BuildReason(facts, processes);
            return verdict;
        }

        private static string BuildReason(List<string> facts, IList<GHProcessLoad> processes)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(ReasonPrefix);
            for (int i = 0; i < facts.Count; i++)
            {
                if (i > 0)
                    sb.Append(", ");
                sb.Append(facts[i]);
            }

            List<GHProcessLoad> suspects = new List<GHProcessLoad>();
            if (processes != null)
            {
                for (int i = 0; i < processes.Count; i++)
                {
                    GHProcessLoad p = processes[i];
                    if (IsIgnoredCategory(EffectiveCategory(p)) || IsIgnoredCategory(CategoryOf(p.Name)))
                        continue;
                    if (SuspectScore(p) < KnownActivityMinCpuPct)
                        continue;
                    suspects.Add(p);
                }
            }
            suspects.Sort(CompareSuspects);

            int named = 0;
            for (int i = 0; i < suspects.Count && named < 2; i++)
            {
                string text = FormatSuspect(suspects[i]);
                string separator = named == 0 ? "; " : ", ";
                if (sb.Length + separator.Length + text.Length > MaxReasonLength)
                    continue;
                sb.Append(separator);
                sb.Append(text);
                named++;
            }

            string reason = sb.ToString();
            if (reason.Length > MaxReasonLength)
                reason = reason.Substring(0, MaxReasonLength - 3) + "...";
            return reason;
        }

        private static float SuspectScore(GHProcessLoad p)
        {
            return Math.Max(OrZero(p.CpuPct), OrZero(p.GpuPct));
        }

        private static int CompareSuspects(GHProcessLoad x, GHProcessLoad y)
        {
            int c = SuspectScore(y).CompareTo(SuspectScore(x));
            if (c != 0)
                return c;
            return string.CompareOrdinal(x.Name, y.Name);
        }

        private static string FormatSuspect(GHProcessLoad p)
        {
            float cpu = OrZero(p.CpuPct);
            float gpu = OrZero(p.GpuPct);
            string value = cpu >= gpu ? Pct(cpu) + " %" : "GPU " + Pct(gpu) + " %";
            return EffectiveCategory(p) + " (" + p.Name + " " + value + ")";
        }

        private static string Pct(float v)
        {
            return v.ToString("F0", CultureInfo.InvariantCulture);
        }
    }
}

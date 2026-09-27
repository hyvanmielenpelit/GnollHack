using System.Globalization;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* The CSV that typeperf -f CSV writes, and the one-row file the suite's Get-Counter
       job writes in the same shape:

         "(PDH-CSV 4.0) (FLE Daylight Time)(-180)","\\HOST\Processor(_Total)\% Processor Time",...
         "09/27/2026 13:56:10.920","15.500861239803809255",...

       The first header cell may end with the UTC bias in minutes in parentheses (UTC =
       local + bias); timestamps are local MM/dd/yyyy HH:mm:ss.fff and are converted to
       UTC with that bias, or with this machine's time zone when the header has none. A
       blank value (" ") is NaN: typeperf leaves the first row of a rate counter blank,
       and a process counter blank while the process is not running. Counter paths are
       kept without their \\HOST prefix and looked up without regard to case. A last
       line that is incomplete (the sampler was killed mid-write) is ignored.

       load_system.csv holds, in this order, \Processor(_Total)\% Processor Time, the
       app's \Process(<name>)\% Processor Time when the name was known, \PhysicalDisk
       (_Total)\% Idle Time, \Memory\Available MBytes and \Memory\Pages Input/sec; the
       columns are found by path, never by position. load_processes.csv holds one row
       of \Process(*)\% Processor Time and \GPU Engine(*)\Utilization Percentage, each
       value the average over the capture (warm-up and window). */
    public sealed class TypeperfTable
    {
        public string HeaderInfo;
        public int? BiasMinutes;
        public readonly List<string> Paths = new List<string>();
        public readonly List<long> TimestampTicks = new List<long>();
        public readonly List<double[]> Rows = new List<double[]>();

        /* The column of a counter path (without host), or -1 */
        public int Column(string path)
        {
            for (int i = 0; i < Paths.Count; i++)
            {
                if (string.Equals(Paths[i], path, StringComparison.OrdinalIgnoreCase))
                    return i;
            }
            return -1;
        }
    }

    public static class TypeperfCsv
    {
        public const string ProcessorTotalPath = @"\Processor(_Total)\% Processor Time";
        public const string DiskIdlePath = @"\PhysicalDisk(_Total)\% Idle Time";
        public const string AvailableMBytesPath = @"\Memory\Available MBytes";
        public const string PagesInputPath = @"\Memory\Pages Input/sec";
        public const string ProcessObject = "Process";
        public const string ProcessCpuCounter = "% Processor Time";
        public const string GpuEngineObject = "GPU Engine";
        public const string GpuUtilizationCounter = "Utilization Percentage";

        private static readonly string[] TimestampFormats = { "MM/dd/yyyy HH:mm:ss.fff", "MM/dd/yyyy HH:mm:ss" };

        public static TypeperfTable Read(string path)
        {
            return Parse(File.ReadAllText(path));
        }

        public static TypeperfTable Parse(string text)
        {
            TypeperfTable t = new TypeperfTable();
            string[] lines = text.Replace("\r\n", "\n").Replace('\r', '\n').Split('\n');
            int last = lines.Length - 1;
            while (last >= 0 && lines[last].Trim().Length == 0)
                last--;
            bool endsWithNewline = text.EndsWith("\n") || text.EndsWith("\r");
            int headerLine = -1;
            for (int i = 0; i <= last; i++)
            {
                if (lines[i].Trim().Length > 0)
                {
                    headerLine = i;
                    break;
                }
            }
            if (headerLine < 0)
                return t;

            string[] header = Csv.Split(lines[headerLine]);
            t.HeaderInfo = header.Length > 0 ? header[0] : "";
            t.BiasMinutes = ParseBias(t.HeaderInfo);
            for (int c = 1; c < header.Length; c++)
                t.Paths.Add(StripHost(header[c]));

            for (int i = headerLine + 1; i <= last; i++)
            {
                string line = lines[i];
                if (line.Trim().Length == 0)
                    continue;
                string[] f = Csv.Split(line);
                if (f.Length != header.Length)
                    continue;
                if (i == last && !endsWithNewline && !line.TrimEnd().EndsWith("\""))
                    continue;
                if (!TryParseTimestamp(f[0], t.BiasMinutes, out long ticks))
                    continue;
                double[] row = new double[t.Paths.Count];
                for (int c = 0; c < row.Length; c++)
                    row[c] = Csv.TryDouble(f[c + 1], out double d) ? d : double.NaN;
                t.TimestampTicks.Add(ticks);
                t.Rows.Add(row);
            }
            return t;
        }

        /* The integer in the last parentheses of the first header cell, e.g. -180 in
           "(PDH-CSV 4.0) (FLE Daylight Time)(-180)"; null when there is none */
        public static int? ParseBias(string headerInfo)
        {
            if (string.IsNullOrEmpty(headerInfo))
                return null;
            int close = headerInfo.LastIndexOf(')');
            int open = close > 0 ? headerInfo.LastIndexOf('(', close - 1) : -1;
            if (open < 0)
                return null;
            string inner = headerInfo.Substring(open + 1, close - open - 1).Trim();
            return int.TryParse(inner, NumberStyles.Integer, CultureInfo.InvariantCulture, out int bias) ? bias : null;
        }

        /* UTC DateTime ticks of a local timestamp: local + bias minutes, or this machine's
           zone when the bias is unknown */
        public static bool TryParseTimestamp(string text, int? biasMinutes, out long utcTicks)
        {
            utcTicks = 0;
            if (text == null || !DateTime.TryParseExact(text.Trim(), TimestampFormats, CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime local))
                return false;
            if (biasMinutes.HasValue)
                utcTicks = local.Ticks + biasMinutes.Value * TimeSpan.TicksPerMinute;
            else
                utcTicks = DateTime.SpecifyKind(local, DateTimeKind.Local).ToUniversalTime().Ticks;
            return true;
        }

        /* "\\HOST\Object(instance)\Counter" -> "\Object(instance)\Counter" */
        public static string StripHost(string path)
        {
            if (path == null)
                return "";
            string p = path.Trim();
            if (p.StartsWith(@"\\"))
            {
                int next = p.IndexOf('\\', 2);
                return next < 0 ? p : p.Substring(next);
            }
            return p;
        }

        /* Splits "\Object(instance)\Counter" into its parts; instance is null when the
           object has no parentheses */
        public static bool SplitPath(string path, out string obj, out string instance, out string counter)
        {
            obj = null;
            instance = null;
            counter = null;
            if (string.IsNullOrEmpty(path))
                return false;
            int lastSlash = path.LastIndexOf('\\');
            if (lastSlash <= 0)
                return false;
            counter = path.Substring(lastSlash + 1);
            string objectPart = path.Substring(path.StartsWith("\\") ? 1 : 0, lastSlash - (path.StartsWith("\\") ? 1 : 0));
            int open = objectPart.IndexOf('(');
            int close = objectPart.LastIndexOf(')');
            if (open >= 0 && close > open)
            {
                obj = objectPart.Substring(0, open);
                instance = objectPart.Substring(open + 1, close - open - 1);
            }
            else
            {
                obj = objectPart;
            }
            return true;
        }

        /* A process name without a trailing ".exe" */
        public static string BaseProcessName(string name)
        {
            if (string.IsNullOrEmpty(name))
                return name;
            string n = name.Trim();
            return n.EndsWith(".exe", StringComparison.OrdinalIgnoreCase) ? n.Substring(0, n.Length - 4) : n;
        }

        /* An instance name without its "#N" suffix */
        public static string StripInstanceSuffix(string instance)
        {
            if (string.IsNullOrEmpty(instance))
                return instance;
            int hash = instance.LastIndexOf('#');
            if (hash < 0 || hash == instance.Length - 1)
                return instance;
            for (int i = hash + 1; i < instance.Length; i++)
            {
                if (instance[i] < '0' || instance[i] > '9')
                    return instance;
            }
            return instance.Substring(0, hash);
        }

        /* The whole-machine samples of load_system.csv. Own CPU is the named process's
           counter (per-core percent) divided by logicalProcessors, NaN when the process
           column is missing or blank. Disk busy is 100 - idle, clamped to 0..100.
           Available memory percent needs totalMemoryMB (> 0), else it is NaN. */
        public static GHSystemLoadSample[] ToSystemSamples(TypeperfTable t, string processName, int logicalProcessors, double totalMemoryMB)
        {
            int cpuCol = t.Column(ProcessorTotalPath);
            int diskCol = t.Column(DiskIdlePath);
            int memCol = t.Column(AvailableMBytesPath);
            int faultCol = t.Column(PagesInputPath);
            int ownCol = -1;
            string own = BaseProcessName(processName);
            if (!string.IsNullOrEmpty(own))
                ownCol = t.Column(@"\" + ProcessObject + "(" + own + @")\" + ProcessCpuCounter);
            int cores = Math.Max(1, logicalProcessors);

            GHSystemLoadSample[] samples = new GHSystemLoadSample[t.Rows.Count];
            for (int i = 0; i < samples.Length; i++)
            {
                double[] row = t.Rows[i];
                GHSystemLoadSample s = GHSystemLoadSample.Empty;
                s.TimestampTicks = t.TimestampTicks[i];
                s.SystemCpuPct = Value(row, cpuCol);
                float ownRaw = Value(row, ownCol);
                s.OwnCpuPct = float.IsNaN(ownRaw) ? float.NaN : ownRaw / cores;
                float idle = Value(row, diskCol);
                s.DiskBusyPct = float.IsNaN(idle) ? float.NaN : Math.Min(100f, Math.Max(0f, 100f - idle));
                float availMB = Value(row, memCol);
                if (!float.IsNaN(availMB))
                {
                    s.AvailableMemoryMB = (long)Math.Round(availMB);
                    if (totalMemoryMB > 0)
                        s.AvailableMemoryPct = (float)(100.0 * availMB / totalMemoryMB);
                }
                s.HardFaultsPerSec = Value(row, faultCol);
                samples[i] = s;
            }
            return samples;
        }

        /* True when the table has any value in the column of the path */
        public static bool HasValues(TypeperfTable t, string path)
        {
            int c = t.Column(path);
            return c >= 0 && t.Rows.Any(r => !double.IsNaN(r[c]));
        }

        /* The per-process table of load_processes.csv (its last row): \Process(*) CPU in
           per-core percent divided by logicalProcessors, "_Total" and "Idle" skipped,
           "name#N" instances summed into one entry per base name, and the own process
           (with its #N instances) dropped. GpuPct is NaN, since the GPU engine instances
           name a pid the process counters do not carry. otherGpuPct is the largest, over
           adapters (luid), of the summed 3D-engine utilization of every process: the
           app itself is included, so it is informational only; NaN when there are no
           3D engine columns. */
        public static List<GHProcessLoad> ToProcessLoads(TypeperfTable t, string ownProcessName, int logicalProcessors, out float otherGpuPct)
        {
            otherGpuPct = float.NaN;
            List<GHProcessLoad> result = new List<GHProcessLoad>();
            if (t.Rows.Count == 0)
                return result;
            double[] row = t.Rows[t.Rows.Count - 1];
            int cores = Math.Max(1, logicalProcessors);
            string own = BaseProcessName(ownProcessName);

            Dictionary<string, double> cpuByName = new Dictionary<string, double>(StringComparer.OrdinalIgnoreCase);
            List<string> order = new List<string>();
            Dictionary<string, double> gpuByLuid = new Dictionary<string, double>(StringComparer.OrdinalIgnoreCase);
            for (int c = 0; c < t.Paths.Count; c++)
            {
                if (!SplitPath(t.Paths[c], out string obj, out string instance, out string counter) || instance == null)
                    continue;
                double v = row[c];
                if (string.Equals(obj, ProcessObject, StringComparison.OrdinalIgnoreCase)
                    && string.Equals(counter, ProcessCpuCounter, StringComparison.OrdinalIgnoreCase))
                {
                    string name = StripInstanceSuffix(instance);
                    if (name.Equals("_Total", StringComparison.OrdinalIgnoreCase) || name.Equals("Idle", StringComparison.OrdinalIgnoreCase))
                        continue;
                    if (!string.IsNullOrEmpty(own) && name.Equals(own, StringComparison.OrdinalIgnoreCase))
                        continue;
                    if (double.IsNaN(v))
                        continue;
                    if (!cpuByName.ContainsKey(name))
                    {
                        cpuByName[name] = 0;
                        order.Add(name);
                    }
                    cpuByName[name] += v;
                }
                else if (string.Equals(obj, GpuEngineObject, StringComparison.OrdinalIgnoreCase)
                    && string.Equals(counter, GpuUtilizationCounter, StringComparison.OrdinalIgnoreCase))
                {
                    if (double.IsNaN(v) || !instance.EndsWith("_engtype_3D", StringComparison.OrdinalIgnoreCase))
                        continue;
                    string luid = GpuLuid(instance);
                    gpuByLuid[luid] = (gpuByLuid.TryGetValue(luid, out double sum) ? sum : 0) + v;
                }
            }
            /* Each name keeps the spelling of its first instance */
            foreach (string name in order)
                result.Add(new GHProcessLoad(name, (float)(cpuByName[name] / cores), float.NaN));
            if (gpuByLuid.Count > 0)
                otherGpuPct = (float)Math.Min(100.0, gpuByLuid.Values.Max());
            return result;
        }

        /* "pid_10356_luid_0x00000000_0x0000E353_phys_0_eng_0_engtype_3D" -> "0x00000000_0x0000E353" */
        private static string GpuLuid(string instance)
        {
            int start = instance.IndexOf("luid_", StringComparison.OrdinalIgnoreCase);
            if (start < 0)
                return "";
            start += 5;
            int end = instance.IndexOf("_phys", start, StringComparison.OrdinalIgnoreCase);
            return end < 0 ? instance.Substring(start) : instance.Substring(start, end - start);
        }

        private static float Value(double[] row, int col)
        {
            return col < 0 || col >= row.Length ? float.NaN : (float)row[col];
        }
    }
}

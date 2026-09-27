using System.Globalization;
using GnollHackX.Performance;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* load_android.txt, the suite's once-a-second adb shell loop, one block per sample:

         T <device epoch ms>
         cpu  <user> <nice> <system> <idle> <iowait> <irq> <softirq> <steal> ...
         P <contents of /proc/<pid>/stat>        ("P " alone when the pid was unknown)
         M MemAvailable:  <kB> kB
         L MemTotal:  <kB> kB

       System CPU is 1 - (idle + iowait) / total over the first eight /proc/stat fields,
       as the delta from the previous block. Own CPU is the delta of the app's utime +
       stime (fields 14 and 15 of /proc/<pid>/stat) over the /proc/stat total delta:
       both count clock ticks of all cores, so the ratio is the share of total capacity
       and needs neither USER_HZ nor the core count. The first block has no delta, so its
       CPU is NaN. Available memory is MemAvailable, as a percentage of MemTotal.

       When /proc/stat is not readable the file starts with "FALLBACK top" and holds the
       raw output of toybox top -b -d 1 -n N instead. Each iteration's header line
       ("800%cpu  12%user ... 700%idle 3%iow ...") gives system busy =
       (cpu - idle - iow) / cpu; the iterations are taken to be one second apart from an
       arbitrary origin (top prints no time), the first iteration is dropped as top
       reports no delta for it, and own CPU and memory are NaN.

       Lines may end with CR; every line is trimmed. */
    public sealed class AndroidLoad
    {
        public GHSystemLoadSample[] Samples = new GHSystemLoadSample[0];
        public bool Fallback;
        public readonly List<string> Signals = new List<string>();
    }

    public static class AndroidLoadLog
    {
        public const string FallbackMarker = "FALLBACK top";

        private sealed class Block
        {
            public long EpochMs;
            public long[] Cpu;               /* null when absent */
            public long PidTicks = -1;       /* utime + stime, -1 when absent */
            public long MemAvailableKb = -1;
            public long MemTotalKb = -1;
        }

        public static AndroidLoad Read(string path)
        {
            return Parse(File.ReadAllText(path));
        }

        public static AndroidLoad Parse(string text)
        {
            List<string> lines = text.Replace("\r\n", "\n").Split('\n').Select(l => l.Trim()).ToList();
            int first = lines.FindIndex(l => l.Length > 0);
            if (first >= 0 && lines[first].Equals(FallbackMarker, StringComparison.OrdinalIgnoreCase))
                return ParseTop(lines.Skip(first + 1));
            return ParseProc(lines);
        }

        private static AndroidLoad ParseProc(List<string> lines)
        {
            List<Block> blocks = new List<Block>();
            Block cur = null;
            foreach (string line in lines)
            {
                if (line.Length == 0)
                    continue;
                if (line.StartsWith("T ") || line == "T")
                {
                    string[] t = line.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
                    cur = null;
                    if (t.Length >= 2 && long.TryParse(t[1], NumberStyles.Integer, CultureInfo.InvariantCulture, out long ms))
                    {
                        cur = new Block { EpochMs = ms };
                        blocks.Add(cur);
                    }
                    continue;
                }
                if (cur == null)
                    continue;
                if (line.StartsWith("cpu ") || line.StartsWith("cpu\t"))
                {
                    cur.Cpu = ParseCpuLine(line);
                }
                else if (line.StartsWith("P"))
                {
                    cur.PidTicks = ParsePidTicks(line.Substring(1).Trim());
                }
                else if (line.StartsWith("M "))
                {
                    cur.MemAvailableKb = ParseMemKb(line.Substring(2));
                }
                else if (line.StartsWith("L "))
                {
                    cur.MemTotalKb = ParseMemKb(line.Substring(2));
                }
            }

            AndroidLoad result = new AndroidLoad();
            GHSystemLoadSample[] samples = new GHSystemLoadSample[blocks.Count];
            bool anyCpu = false, anyMem = false;
            for (int i = 0; i < blocks.Count; i++)
            {
                Block b = blocks[i];
                GHSystemLoadSample s = GHSystemLoadSample.Empty;
                s.TimestampTicks = DateTime.UnixEpoch.Ticks + b.EpochMs * TimeSpan.TicksPerMillisecond;
                if (i > 0 && b.Cpu != null && blocks[i - 1].Cpu != null)
                {
                    Block p = blocks[i - 1];
                    long total = Sum(b.Cpu) - Sum(p.Cpu);
                    long idle = (b.Cpu[3] + b.Cpu[4]) - (p.Cpu[3] + p.Cpu[4]);
                    if (total > 0 && idle >= 0 && idle <= total)
                    {
                        s.SystemCpuPct = (float)(100.0 * (total - idle) / total);
                        anyCpu = true;
                        long own = b.PidTicks - p.PidTicks;
                        if (b.PidTicks >= 0 && p.PidTicks >= 0 && own >= 0)
                            s.OwnCpuPct = (float)Math.Min(100.0, 100.0 * own / total);
                    }
                }
                if (b.MemAvailableKb >= 0)
                {
                    s.AvailableMemoryMB = b.MemAvailableKb / 1024;
                    if (b.MemTotalKb > 0)
                        s.AvailableMemoryPct = (float)(100.0 * b.MemAvailableKb / b.MemTotalKb);
                    anyMem = true;
                }
                samples[i] = s;
            }
            result.Samples = samples;
            if (anyCpu)
                result.Signals.Add("systemCpu");
            if (anyMem)
                result.Signals.Add("memory");
            return result;
        }

        /* The first eight numeric fields after "cpu", missing ones as 0 */
        private static long[] ParseCpuLine(string line)
        {
            string[] t = line.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            long[] v = new long[8];
            for (int i = 0; i < 8 && i + 1 < t.Length; i++)
                v[i] = long.TryParse(t[i + 1], NumberStyles.Integer, CultureInfo.InvariantCulture, out long x) ? x : 0;
            return v;
        }

        /* utime + stime from a /proc/<pid>/stat line; the command name in parentheses
           may hold spaces, so fields are counted from its closing parenthesis (field 3
           onwards). -1 when the line is empty or too short. */
        public static long ParsePidTicks(string stat)
        {
            if (string.IsNullOrEmpty(stat))
                return -1;
            int close = stat.LastIndexOf(')');
            string rest = close >= 0 ? stat.Substring(close + 1) : stat;
            string[] f = rest.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            /* rest[0] is field 3, so field 14 is rest[11] and field 15 rest[12] */
            int offset = close >= 0 ? 11 : 13;
            if (f.Length <= offset + 1)
                return -1;
            if (!long.TryParse(f[offset], NumberStyles.Integer, CultureInfo.InvariantCulture, out long utime)
                || !long.TryParse(f[offset + 1], NumberStyles.Integer, CultureInfo.InvariantCulture, out long stime))
                return -1;
            return utime + stime;
        }

        /* The number in "MemAvailable:  123456 kB"; -1 when there is none */
        private static long ParseMemKb(string text)
        {
            string[] t = text.Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in t)
            {
                if (long.TryParse(s, NumberStyles.Integer, CultureInfo.InvariantCulture, out long kb))
                    return kb;
            }
            return -1;
        }

        private static long Sum(long[] v)
        {
            long s = 0;
            for (int i = 0; i < v.Length; i++)
                s += v[i];
            return s;
        }

        private static AndroidLoad ParseTop(IEnumerable<string> lines)
        {
            List<float> busy = new List<float>();
            foreach (string line in lines)
            {
                if (line.IndexOf("%cpu", StringComparison.OrdinalIgnoreCase) < 0 || line.IndexOf("%idle", StringComparison.OrdinalIgnoreCase) < 0)
                    continue;
                double cpu = double.NaN, idle = 0, iow = 0;
                foreach (string token in line.Split((char[])null, StringSplitOptions.RemoveEmptyEntries))
                {
                    int pct = token.IndexOf('%');
                    if (pct <= 0 || !double.TryParse(token.Substring(0, pct), NumberStyles.Float, CultureInfo.InvariantCulture, out double v))
                        continue;
                    string name = token.Substring(pct + 1).ToLowerInvariant();
                    if (name == "cpu")
                        cpu = v;
                    else if (name == "idle")
                        idle = v;
                    else if (name == "iow")
                        iow = v;
                }
                busy.Add(double.IsNaN(cpu) || cpu <= 0 ? float.NaN : (float)Math.Min(100.0, Math.Max(0.0, 100.0 * (cpu - idle - iow) / cpu)));
            }

            AndroidLoad result = new AndroidLoad { Fallback = true };
            int n = Math.Max(0, busy.Count - 1);
            result.Samples = new GHSystemLoadSample[n];
            for (int i = 0; i < n; i++)
            {
                GHSystemLoadSample s = GHSystemLoadSample.Empty;
                s.TimestampTicks = (i + 1) * TimeSpan.TicksPerSecond;
                s.SystemCpuPct = busy[i + 1];
                result.Samples[i] = s;
            }
            if (result.Samples.Any(s => !float.IsNaN(s.SystemCpuPct)))
                result.Signals.Add("systemCpu");
            return result;
        }
    }
}

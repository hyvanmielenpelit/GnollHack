using System.Globalization;
using GnollHack.PerformanceAnalyzer.Model;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* One present of the captured process. Timestamp is the QPC column's raw value, in
       ticks of the capturing machine (or in ms when the column says so); the offsets are
       milliseconds after it. */
    public sealed class PresentMonPresent
    {
        public string SwapChain;
        public double Timestamp;
        /* When the application presented: 0, or MsCPUBusy when the timestamp is PresentMon
           2.x's CPU start of the frame */
        public double PresentOffsetMs;
        /* When the present reached the screen (MsUntilDisplayed); NaN when never displayed */
        public double DisplayedOffsetMs = double.NaN;
        public bool Displayed { get { return !double.IsNaN(DisplayedOffsetMs); } }
    }

    public sealed class PresentMonCapture
    {
        public string Path;
        public List<PresentMonPresent> Presents = new List<PresentMonPresent>();
        public string TimeColumn;               /* null when the capture has no QPC column */
        public bool TimesInMs;                  /* the QPC column holds milliseconds, not ticks */
        public string DisplayColumn;
        public string KeptSwapChain;
        public List<KeyValuePair<string, int>> OtherSwapChains = new List<KeyValuePair<string, int>>();
        public int RowsAfterProcessFilter;
        public bool HasQpc { get { return TimeColumn != null; } }
    }

    /* Reads a PresentMon CSV. PresentMon 1.x and 2.x name their columns differently, so
       every column is chosen from a preference list and the choice is recorded.

       Rows are filtered to the named process, then to one swap chain: a process can
       present to several (a XAML island, an overlay, a secondary window), and mixing their
       presents makes nonsense of intervals and joins. When a SwapChainAddress column
       exists, the swap chain with the most presents is kept and the others are reported.

       Read builds the external interval series. The preferred interval column is the one
       closest to what the viewer perceives: the time between consecutive display changes
       when the tool reports it, else the time between presents. ReadPresents keeps each
       present's QPC time and display time for joining to the in-app frame timeline;
       Capture-PresentMon.ps1 passes --qpc_time so that the QPC column exists. */
    public static class PresentMonCsv
    {
        private static readonly string[] IntervalPreference =
        {
            "MsBetweenDisplayChange",   /* 2.x: time between the displayed frames */
            "MsBetweenPresents",        /* 1.x and 2.x: time between application presents */
            "msBetweenPresents"
        };

        private static readonly string[] ProcessColumns = { "Application", "ProcessName" };
        private static readonly string[] DroppedColumns = { "Dropped", "FrameDropped" };
        private static readonly string[] SwapChainColumns = { "SwapChainAddress", "SwapChain" };

        /* 2.x --qpc_time writes CPUStartQPC; 1.x writes a QPC present time under one of
           the other names */
        private static readonly string[] QpcColumns = { "CPUStartQPC", "TimeInQPC", "QPCTime", "CPUStartQPCTime" };
        private static readonly string[] DisplayedColumns = { "MsUntilDisplayed", "MsDisplayLatency" };

        private sealed class Table
        {
            public string[] Header;
            public Dictionary<string, int> Col;
            public List<string[]> Rows = new List<string[]>();
            public int RowsAfterProcessFilter;
            public string KeptSwapChain;
            public List<KeyValuePair<string, int>> OtherSwapChains = new List<KeyValuePair<string, int>>();

            public int Find(string[] names)
            {
                foreach (string n in names)
                {
                    if (Col.TryGetValue(n, out int i))
                        return i;
                }
                return -1;
            }
        }

        private static Table Load(string path, string processNameFilter)
        {
            string[] lines = File.ReadAllLines(path);
            int h = 0;
            while (h < lines.Length && (lines[h].Trim().Length == 0 || lines[h].TrimStart().StartsWith("#")))
                h++;
            if (h >= lines.Length)
                throw new InvalidDataException("PresentMon CSV is empty: " + path);
            Table t = new Table();
            t.Header = Csv.Split(lines[h]);
            t.Col = Csv.HeaderIndex(t.Header);
            int cp = t.Find(ProcessColumns);
            List<string[]> rows = new List<string[]>(lines.Length);
            for (int k = h + 1; k < lines.Length; k++)
            {
                if (lines[k].Length == 0)
                    continue;
                string[] f = Csv.Split(lines[k]);
                if (cp >= 0 && !string.IsNullOrEmpty(processNameFilter) && f.Length > cp)
                {
                    string proc = f[cp].Trim();
                    if (!proc.Equals(processNameFilter, StringComparison.OrdinalIgnoreCase)
                        && !proc.Equals(processNameFilter + ".exe", StringComparison.OrdinalIgnoreCase))
                        continue;
                }
                rows.Add(f);
            }
            t.RowsAfterProcessFilter = rows.Count;

            int cs = t.Find(SwapChainColumns);
            if (cs < 0)
            {
                t.Rows = rows;
                return t;
            }
            Dictionary<string, int> counts = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);
            foreach (string[] f in rows)
            {
                string sc = SwapChainOf(f, cs);
                counts[sc] = counts.TryGetValue(sc, out int n) ? n + 1 : 1;
            }
            if (counts.Count == 0)
            {
                t.Rows = rows;
                return t;
            }
            List<KeyValuePair<string, int>> ordered = counts.OrderByDescending(kv => kv.Value)
                .ThenBy(kv => kv.Key, StringComparer.Ordinal).ToList();
            t.KeptSwapChain = ordered[0].Key;
            t.OtherSwapChains = ordered.Skip(1).ToList();
            t.Rows = rows.Where(f => string.Equals(SwapChainOf(f, cs), t.KeptSwapChain, StringComparison.OrdinalIgnoreCase)).ToList();
            return t;
        }

        private static string SwapChainOf(string[] f, int cs)
        {
            return cs < f.Length ? f[cs].Trim() : "";
        }

        private static void RecordSwapChains(Table t, Dictionary<string, string> info)
        {
            if (t.KeptSwapChain == null)
                return;
            info["swapChain"] = t.KeptSwapChain;
            if (t.OtherSwapChains.Count > 0)
            {
                info["otherSwapChains"] = string.Join("; ", t.OtherSwapChains.Select(kv => kv.Key + ": " + kv.Value.ToString(CultureInfo.InvariantCulture)));
                info["otherSwapChainPresents"] = t.OtherSwapChains.Sum(kv => kv.Value).ToString(CultureInfo.InvariantCulture);
            }
        }

        public static Series Read(string path, string processNameFilter)
        {
            Table t = Load(path, processNameFilter);
            string intervalCol = IntervalPreference.FirstOrDefault(c => t.Col.ContainsKey(c));
            if (intervalCol == null)
                throw new InvalidDataException("PresentMon CSV has no interval column (looked for " + string.Join(", ", IntervalPreference) + "): " + path);
            int ci = t.Col[intervalCol];
            int cd = t.Find(DroppedColumns);

            List<float> intervals = new List<float>(t.Rows.Count);
            int dropped = 0;
            int rows = 0;
            bool first = true;
            foreach (string[] f in t.Rows)
            {
                if (f.Length <= ci)
                    continue;
                rows++;
                if (cd >= 0 && f.Length > cd && f[cd].Trim() == "1")
                    dropped++;
                if (!float.TryParse(f[ci].Trim(), NumberStyles.Float, CultureInfo.InvariantCulture, out float v))
                    continue;
                /* The first row's interval is measured against nothing meaningful */
                if (first)
                {
                    first = false;
                    continue;
                }
                if (v <= 0)
                    continue;
                intervals.Add(v);
            }

            Series s = new Series
            {
                Kind = "external",
                Source = "presentmon",
                Column = intervalCol,
                IntervalsMs = intervals.ToArray(),
                DroppedCount = dropped
            };
            s.Info["rows"] = rows.ToString(CultureInfo.InvariantCulture);
            s.Info["file"] = Path.GetFileName(path);
            RecordSwapChains(t, s.Info);
            return s;
        }

        public static PresentMonCapture ReadPresents(string path, string processNameFilter)
        {
            Table t = Load(path, processNameFilter);
            PresentMonCapture cap = new PresentMonCapture
            {
                Path = path,
                KeptSwapChain = t.KeptSwapChain,
                OtherSwapChains = t.OtherSwapChains,
                RowsAfterProcessFilter = t.RowsAfterProcessFilter
            };
            int ct = t.Find(QpcColumns);
            if (ct < 0)
            {
                /* Any other QPC-named column, e.g. a later release's spelling */
                for (int i = 0; i < t.Header.Length && ct < 0; i++)
                {
                    if (t.Header[i].IndexOf("QPC", StringComparison.OrdinalIgnoreCase) >= 0)
                        ct = i;
                }
            }
            if (ct < 0)
                return cap;
            cap.TimeColumn = t.Header[ct].Trim();
            cap.TimesInMs = cap.TimeColumn.EndsWith("Ms", StringComparison.OrdinalIgnoreCase)
                || cap.TimeColumn.IndexOf("InMs", StringComparison.OrdinalIgnoreCase) >= 0;
            bool cpuStart = cap.TimeColumn.StartsWith("CPUStart", StringComparison.OrdinalIgnoreCase);
            int cBusy = cpuStart && t.Col.TryGetValue("MsCPUBusy", out int b) ? b : -1;
            int cDisp = t.Find(DisplayedColumns);
            cap.DisplayColumn = cDisp >= 0 ? t.Header[cDisp].Trim() : null;
            int cd = t.Find(DroppedColumns);
            int cs = t.Find(SwapChainColumns);

            foreach (string[] f in t.Rows)
            {
                if (!Csv.TryDouble(Csv.Field(f, ct), out double ts))
                    continue;
                PresentMonPresent p = new PresentMonPresent
                {
                    SwapChain = cs >= 0 ? SwapChainOf(f, cs) : null,
                    Timestamp = ts
                };
                if (cBusy >= 0 && Csv.TryDouble(Csv.Field(f, cBusy), out double busy))
                    p.PresentOffsetMs = busy;
                /* NA, an empty field or a Dropped flag: the present never reached the screen */
                bool droppedFlag = cd >= 0 && Csv.Field(f, cd) == "1";
                if (!droppedFlag && cDisp >= 0 && Csv.TryDouble(Csv.Field(f, cDisp), out double untilDisplayed) && untilDisplayed >= 0)
                    p.DisplayedOffsetMs = untilDisplayed;
                cap.Presents.Add(p);
            }
            return cap;
        }
    }
}

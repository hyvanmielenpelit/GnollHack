using System.Globalization;
using GnollHack.PerfAnalyzer.Model;

namespace GnollHack.PerfAnalyzer.Readers
{
    /* Reads a PresentMon CSV into an external series. PresentMon 1.x and 2.x name their
       columns differently, so the interval column is chosen from a preference list and
       the choice is recorded in Series.Column. The preferred column is the one closest
       to what the viewer perceives: the time between consecutive display changes when
       the tool reports it, else the time between presents. */
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

        public static Series Read(string path, string processNameFilter)
        {
            using StreamReader reader = new StreamReader(path);
            string headerLine = reader.ReadLine();
            if (headerLine == null)
                throw new InvalidDataException("PresentMon CSV is empty: " + path);
            string[] header = headerLine.Split(',');
            Dictionary<string, int> col = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);
            for (int i = 0; i < header.Length; i++)
                col[header[i].Trim()] = i;

            string intervalCol = IntervalPreference.FirstOrDefault(c => col.ContainsKey(c));
            if (intervalCol == null)
                throw new InvalidDataException("PresentMon CSV has no interval column (looked for " + string.Join(", ", IntervalPreference) + "): " + path);
            int ci = col[intervalCol];
            int cp = ProcessColumns.Where(c => col.ContainsKey(c)).Select(c => col[c]).DefaultIfEmpty(-1).First();
            int cd = DroppedColumns.Where(c => col.ContainsKey(c)).Select(c => col[c]).DefaultIfEmpty(-1).First();

            List<float> intervals = new List<float>(8192);
            int dropped = 0;
            int rows = 0;
            bool first = true;
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                if (line.Length == 0)
                    continue;
                string[] f = line.Split(',');
                if (f.Length <= ci)
                    continue;
                if (cp >= 0 && !string.IsNullOrEmpty(processNameFilter))
                {
                    string proc = f[cp].Trim();
                    if (!proc.Equals(processNameFilter, StringComparison.OrdinalIgnoreCase)
                        && !proc.Equals(processNameFilter + ".exe", StringComparison.OrdinalIgnoreCase))
                        continue;
                }
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
            return s;
        }
    }
}

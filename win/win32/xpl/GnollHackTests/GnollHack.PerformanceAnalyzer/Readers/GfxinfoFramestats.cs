using System.Globalization;
using GnollHack.PerformanceAnalyzer.Model;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* Reads `dumpsys gfxinfo <package> framestats` output. The command returns only the
       last 120 or so frames, so Capture-AndroidFrames.ps1 polls it repeatedly and writes
       one file per poll; this reader merges every file in a directory and de-duplicates
       frames by their IntendedVsync timestamp, which is unique per frame.

       Frames rendered between two polls can be lost when a poll comes too late to see
       them. Such a span is detected where a poll does not overlap the frames already seen
       (its earliest IntendedVsync lies after the latest one of every earlier poll) and the
       step to its first new frame exceeds 1.5 times the median frame interval, so frames
       are missing. No interval is computed across that boundary, since it would be the
       length of the unseen span rather than a frame; the boundaries are counted in
       Info["lostGapBoundaries"].

       Columns are the ones Android documents: Flags, IntendedVsync, Vsync, ...,
       FrameCompleted, and on newer releases GpuCompleted and more. Frames whose Flags
       field is non-zero are excluded from timing, as Google's own guidance says: they are
       the first frame after a window change or a surface recreation, and are not
       representative. */
    public static class GfxinfoFramestats
    {
        private const string SectionStart = "---PROFILEDATA---";
        private const double LostGapFactor = 1.5;

        public static Series ReadDirectory(string directory)
        {
            string[] files = Directory.GetFiles(directory, "framestats_*.txt");
            Array.Sort(files, StringComparer.Ordinal);
            SortedDictionary<long, FrameRow> frames = new SortedDictionary<long, FrameRow>();
            List<List<long>> polls = new List<List<long>>();
            foreach (string file in files)
                polls.Add(ReadInto(file, frames));
            return Build(frames, polls);
        }

        public static Series ReadFile(string path)
        {
            SortedDictionary<long, FrameRow> frames = new SortedDictionary<long, FrameRow>();
            List<List<long>> polls = new List<List<long>> { ReadInto(path, frames) };
            return Build(frames, polls);
        }

        private sealed class FrameRow
        {
            public long Flags;
            public long IntendedVsync;
            public long Vsync;
            public long FrameCompleted;
            public long GpuCompleted;
        }

        /* Returns the IntendedVsync of every frame in the file, in file order */
        private static List<long> ReadInto(string path, SortedDictionary<long, FrameRow> frames)
        {
            List<long> seen = new List<long>();
            string[] lines = File.ReadAllLines(path);
            int i = Array.FindIndex(lines, l => l.Trim() == SectionStart);
            if (i < 0 || i + 1 >= lines.Length)
                return seen;
            string[] header = lines[i + 1].Split(',');
            int cFlags = Array.IndexOf(header, "Flags");
            int cIv = Array.IndexOf(header, "IntendedVsync");
            int cV = Array.IndexOf(header, "Vsync");
            int cFc = Array.IndexOf(header, "FrameCompleted");
            int cGc = Array.IndexOf(header, "GpuCompleted");
            if (cFlags < 0 || cIv < 0 || cFc < 0)
                return seen;
            for (int k = i + 2; k < lines.Length; k++)
            {
                string l = lines[k].Trim();
                if (l == SectionStart || l.Length == 0)
                    break;
                string[] f = l.Split(',');
                if (f.Length <= cFc)
                    continue;
                if (!long.TryParse(f[cIv], NumberStyles.Integer, CultureInfo.InvariantCulture, out long iv))
                    continue;
                seen.Add(iv);
                if (frames.ContainsKey(iv))
                    continue;
                FrameRow row = new FrameRow { IntendedVsync = iv };
                long.TryParse(f[cFlags], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.Flags);
                if (cV >= 0) long.TryParse(f[cV], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.Vsync);
                long.TryParse(f[cFc], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.FrameCompleted);
                if (cGc >= 0 && f.Length > cGc) long.TryParse(f[cGc], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.GpuCompleted);
                frames[iv] = row;
            }
            return seen;
        }

        /* Median step between consecutive IntendedVsync values within one poll, where the
           frames are known to be contiguous; the merged set is the fallback */
        private static double MedianFrameInterval(List<List<long>> polls, IList<long> merged)
        {
            List<long> deltas = new List<long>();
            foreach (List<long> p in polls)
            {
                List<long> sorted = p.Distinct().OrderBy(v => v).ToList();
                for (int k = 1; k < sorted.Count; k++)
                    deltas.Add(sorted[k] - sorted[k - 1]);
            }
            if (deltas.Count == 0)
            {
                for (int k = 1; k < merged.Count; k++)
                    deltas.Add(merged[k] - merged[k - 1]);
            }
            if (deltas.Count == 0)
                return 0;
            deltas.Sort();
            return deltas[deltas.Count / 2];
        }

        /* IntendedVsync values of the frames that follow a span of lost frames */
        private static HashSet<long> LostGapStarts(List<List<long>> polls, List<long> keys, double medianInterval)
        {
            HashSet<long> starts = new HashSet<long>();
            if (medianInterval <= 0)
                return starts;
            long coveredMax = long.MinValue;
            foreach (List<long> p in polls)
            {
                if (p.Count == 0)
                    continue;
                long min = p.Min(), max = p.Max();
                if (coveredMax != long.MinValue && min > coveredMax)
                {
                    int idx = keys.BinarySearch(coveredMax);
                    int next = idx >= 0 ? idx + 1 : ~idx;
                    if (next < keys.Count && keys[next] - coveredMax > LostGapFactor * medianInterval)
                        starts.Add(keys[next]);
                }
                coveredMax = Math.Max(coveredMax, max);
            }
            return starts;
        }

        private static Series Build(SortedDictionary<long, FrameRow> frames, List<List<long>> polls)
        {
            List<long> keys = frames.Keys.ToList();
            double medianInterval = MedianFrameInterval(polls, keys);
            HashSet<long> gapStarts = LostGapStarts(polls, keys, medianInterval);

            List<float> intervals = new List<float>(frames.Count);
            List<float> durations = new List<float>(frames.Count);
            FrameRow prev = null;
            int flagged = 0;
            int lostGaps = 0;
            foreach (FrameRow r in frames.Values)
            {
                if (gapStarts.Contains(r.IntendedVsync))
                {
                    lostGaps++;
                    prev = null;
                }
                if (r.Flags != 0)
                {
                    flagged++;
                    prev = null;
                    continue;
                }
                long completed = r.GpuCompleted > 0 ? r.GpuCompleted : r.FrameCompleted;
                durations.Add((completed - r.IntendedVsync) / 1_000_000f);
                if (prev != null)
                {
                    long prevCompleted = prev.GpuCompleted > 0 ? prev.GpuCompleted : prev.FrameCompleted;
                    float dt = (completed - prevCompleted) / 1_000_000f;
                    if (dt > 0)
                        intervals.Add(dt);
                }
                prev = r;
            }
            Series s = new Series
            {
                Kind = "external",
                Source = "gfxinfo",
                Column = "FrameCompleted (GpuCompleted when present)",
                IntervalsMs = intervals.ToArray(),
                FrameDurationsMs = durations.ToArray()
            };
            s.Info["polls"] = polls.Count.ToString(CultureInfo.InvariantCulture);
            s.Info["uniqueFrames"] = frames.Count.ToString(CultureInfo.InvariantCulture);
            s.Info["flaggedFrames"] = flagged.ToString(CultureInfo.InvariantCulture);
            s.Info["lostGapBoundaries"] = lostGaps.ToString(CultureInfo.InvariantCulture);
            s.Info["medianFrameIntervalMs"] = (medianInterval / 1_000_000.0).ToString("0.###", CultureInfo.InvariantCulture);
            return s;
        }
    }
}

using System.Globalization;
using GnollHack.PerfAnalyzer.Model;

namespace GnollHack.PerfAnalyzer.Readers
{
    /* Reads `dumpsys gfxinfo <package> framestats` output. The command returns only the
       last 120 or so frames, so Capture-AndroidFrames.ps1 polls it repeatedly and writes
       one file per poll; this reader merges every file in a directory and de-duplicates
       frames by their IntendedVsync timestamp, which is unique per frame.

       Columns are the ones Android documents: Flags, IntendedVsync, Vsync, ...,
       FrameCompleted, and on newer releases GpuCompleted and more. Frames whose Flags
       field is non-zero are excluded from timing, as Google's own guidance says: they are
       the first frame after a window change or a surface recreation, and are not
       representative. */
    public static class GfxinfoFramestats
    {
        private const string SectionStart = "---PROFILEDATA---";

        public static Series ReadDirectory(string directory)
        {
            string[] files = Directory.GetFiles(directory, "framestats_*.txt");
            Array.Sort(files, StringComparer.Ordinal);
            SortedDictionary<long, FrameRow> frames = new SortedDictionary<long, FrameRow>();
            int polls = 0;
            foreach (string file in files)
            {
                polls++;
                ReadInto(file, frames);
            }
            return Build(frames, polls);
        }

        public static Series ReadFile(string path)
        {
            SortedDictionary<long, FrameRow> frames = new SortedDictionary<long, FrameRow>();
            ReadInto(path, frames);
            return Build(frames, 1);
        }

        private sealed class FrameRow
        {
            public long Flags;
            public long IntendedVsync;
            public long Vsync;
            public long FrameCompleted;
            public long GpuCompleted;
        }

        private static void ReadInto(string path, SortedDictionary<long, FrameRow> frames)
        {
            string[] lines = File.ReadAllLines(path);
            int i = Array.FindIndex(lines, l => l.Trim() == SectionStart);
            if (i < 0)
                return;
            string[] header = lines[i + 1].Split(',');
            int cFlags = Array.IndexOf(header, "Flags");
            int cIv = Array.IndexOf(header, "IntendedVsync");
            int cV = Array.IndexOf(header, "Vsync");
            int cFc = Array.IndexOf(header, "FrameCompleted");
            int cGc = Array.IndexOf(header, "GpuCompleted");
            if (cFlags < 0 || cIv < 0 || cFc < 0)
                return;
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
                if (frames.ContainsKey(iv))
                    continue;
                FrameRow row = new FrameRow { IntendedVsync = iv };
                long.TryParse(f[cFlags], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.Flags);
                if (cV >= 0) long.TryParse(f[cV], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.Vsync);
                long.TryParse(f[cFc], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.FrameCompleted);
                if (cGc >= 0 && f.Length > cGc) long.TryParse(f[cGc], NumberStyles.Integer, CultureInfo.InvariantCulture, out row.GpuCompleted);
                frames[iv] = row;
            }
        }

        private static Series Build(SortedDictionary<long, FrameRow> frames, int polls)
        {
            List<float> intervals = new List<float>(frames.Count);
            List<float> durations = new List<float>(frames.Count);
            FrameRow prev = null;
            int flagged = 0;
            foreach (FrameRow r in frames.Values)
            {
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
            s.Info["polls"] = polls.ToString(CultureInfo.InvariantCulture);
            s.Info["uniqueFrames"] = frames.Count.ToString(CultureInfo.InvariantCulture);
            s.Info["flaggedFrames"] = flagged.ToString(CultureInfo.InvariantCulture);
            return s;
        }
    }
}
